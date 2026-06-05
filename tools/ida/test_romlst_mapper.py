#!/usr/bin/env python3
from __future__ import annotations

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))

from ida import shared_lib as ccm  # type: ignore
from ida.walk_source_and_rom import (
    collect_literal_macro_refs,
    iter_expanded_lines,
    parse_src_ops,
    parse_rom_ops_from_addr,
    parse_int_token,
    parse_rom_operand_addr,
    parse_address_map,
    parse_vunit_hardware_symbols,
    source_rom_data_words_between,
    source_set_symbol_addr,
    lookup_address_map_symbol,
    source_comment_tag,
    canonical_module_for_label,
    read_rom_word,
    reorder_same_address_parallel_rom_match,
)

ROM = ROOT / 'roms' / 'test.rom.lst'


def find_src_start(module: str, label: str) -> int:
    p = ROOT / f'{module}.ASM'
    lines = p.read_text(errors='ignore').splitlines()
    for i, ln in enumerate(lines):
        l, _ = ccm.split_optional_label(ccm.strip_comment(ln))
        if l == label:
            return i
    raise AssertionError(f'Could not find {label} in {module}.ASM')


def pair_until_symbol(module: str, label: str, addr: int, symbol: str) -> int:
    macros = ccm.parse_macros(ROOT)
    symbols = ccm.parse_set_symbols(ROOT)

    p = ROOT / f'{module}.ASM'
    lines = p.read_text(errors='ignore').splitlines()
    start = find_src_start(module, label)
    expanded = iter_expanded_lines(lines[start:], macros, symbols)
    sops = parse_src_ops(expanded)
    rops = parse_rom_ops_from_addr(ROM, addr)

    n = min(len(sops), len(rops))
    for i in range(n):
        sop, stoks = sops[i]
        rea, rop, rtoks = rops[i]
        if not ccm.mnemonics_equivalent([sop.lower()], [rop.lower()]):
            raise AssertionError(f'{label}: desync at step {i}: src={sop} rom={rop} ea=0x{rea:08X}')
        for idx, stok in enumerate(stoks[1:]):
            if f'@{symbol}'.upper() not in stok.upper():
                continue
            ridx = idx + 1
            if ridx >= len(rtoks):
                raise AssertionError(f'{label}: symbol {symbol} missing ROM operand at step {i}')
            ea = parse_int_token(rtoks[ridx])
            if ea is None:
                raise AssertionError(f'{label}: symbol {symbol} ROM token not numeric: {rtoks[ridx]}')
            return ea
    raise AssertionError(f'{label}: did not find symbol {symbol} in paired stream')


def assert_rom_has_label(label: str, addr: int) -> None:
    pat = re.compile(rf"^CODE:{addr:08X}\s+{re.escape(label)}:")
    for ln in ROM.read_text(errors='ignore').splitlines():
        if pat.match(ln):
            return
    raise AssertionError(f'ROM missing label {label} at 0x{addr:08X}')


def resolve_literal_macro_target(module: str, label: str, addr: int, macro: str, wanted: str) -> tuple[int, int]:
    macros = ccm.parse_macros(ROOT)
    symbols = ccm.parse_set_symbols(ROOT)

    p = ROOT / f'{module}.ASM'
    lines = p.read_text(errors='ignore').splitlines()
    start = find_src_start(module, label)
    src_lines = lines[start:]
    expanded = iter_expanded_lines(src_lines, macros, symbols)
    sops = parse_src_ops(expanded)
    rops = parse_rom_ops_from_addr(ROM, addr)
    refs = collect_literal_macro_refs(src_lines, start + 1, symbols)
    ref_idx = 0

    n = min(len(sops), len(rops))
    for i in range(n):
        sop, stoks = sops[i]
        _rea, rop, rtoks = rops[i]
        if not ccm.mnemonics_equivalent([sop.lower()], [rop.lower()]):
            raise AssertionError(f'{label}: desync at step {i}: src={sop} rom={rop}')
        if sop.upper() != 'LDI' or len(stoks) < 2 or len(rtoks) < 2:
            continue
        src_lit = stoks[1].strip().strip(',').upper()
        kind = ''
        if src_lit == '@VALUEI?':
            kind = 'LDL'
        elif src_lit == '@IT?':
            kind = 'TEXTIT'
        if not kind:
            continue
        if ref_idx >= len(refs):
            raise AssertionError(f'{label}: literal macro ref stream exhausted')
        _lineno, ref_kind, ref_label = refs[ref_idx]
        ref_idx += 1
        if ref_kind != kind:
            raise AssertionError(f'{label}: literal macro mismatch at step {i}: {kind} vs {ref_kind}')
        if ref_kind == macro and ref_label == wanted:
            lit_ea = parse_int_token(rtoks[1])
            if lit_ea is None:
                raise AssertionError(f'{label}: literal cell token is not numeric: {rtoks[1]}')
            target_ea = read_rom_word(lit_ea)
            if target_ea is None:
                raise AssertionError(f'{label}: literal cell 0x{lit_ea:08X} has no binary ROM word value')
            return lit_ea, target_ea
    raise AssertionError(f'{label}: did not resolve {macro} {wanted}')


def main() -> None:
    # 0) Literal pointer cells are read from the canonical bswap32 binary,
    #    not from .lst .word lines.
    assert read_rom_word(0xB0BF) == 0xA130, f'word 0xB0BF expected 0xA130, got {read_rom_word(0xB0BF)!r}'
    assert read_rom_word(0xB124) == 0xA130, f'word 0xB124 expected 0xA130, got {read_rom_word(0xB124)!r}'
    assert read_rom_word(0xB0C5) == 0xC10253, f'word 0xB0C5 expected 0xC10253, got {read_rom_word(0xB0C5)!r}'
    assert read_rom_word(0xC737) == 0xA15E, f'word 0xC737 expected 0xA15E, got {read_rom_word(0xC737)!r}'

    # 1) MESSAGE1 must pair and resolve ACTIVE_SCREEN to 0xCE43.
    ea = pair_until_symbol('CUSA', 'MESSAGE1', 0x4F43, 'ACTIVE_SCREEN')
    assert ea == 0xCE43, f'MESSAGE1 ACTIVE_SCREEN expected 0xCE43, got 0x{ea:08X}'

    # 2) _pixel must pair and resolve ACTIVE_SCREEN to 0xCE43.
    ea2 = pair_until_symbol('FONT1A', '_pixel', 0xA84F, 'ACTIVE_SCREEN')
    assert ea2 == 0xCE43, f'_pixel ACTIVE_SCREEN expected 0xCE43, got 0x{ea2:08X}'

    # 3) LOAD_SECTION_REQ anchor must exist at 0xA3ED.
    assert_rom_has_label('LOAD_SECTION_REQ', 0xA3ED)

    # 4) DIRQ must resolve OACTIVEI pointer-cell address to 0x40.
    ea3 = pair_until_symbol('DIRQ', 'DIRQ', 0x0064, 'OACTIVEI')
    assert ea3 == 0x40, f'DIRQ OACTIVEI expected 0x40, got 0x{ea3:08X}'

    # 5) DIRQ must resolve OACTIVE_PRIORITYI pointer-cell address to 0x42.
    ea4 = pair_until_symbol('DIRQ', 'DIRQ', 0x0064, 'OACTIVE_PRIORITYI')
    assert ea4 == 0x42, f'DIRQ OACTIVE_PRIORITYI expected 0x42, got 0x{ea4:08X}'

    # 6) DIRQ parallel-issue block must stay aligned:
    #    mpyf3 || stf / addf3 sequence around 0xB0 must not desync on "||".
    macros = ccm.parse_macros(ROOT)
    symbols = ccm.parse_set_symbols(ROOT)
    dlines = (ROOT / 'DIRQ.ASM').read_text(errors='ignore').splitlines()
    dstart = find_src_start('DIRQ', 'DIRQ')
    dexp = iter_expanded_lines(dlines[dstart:], macros, symbols)
    dsops = parse_src_ops(dexp)
    drops = parse_rom_ops_from_addr(ROM, 0x64)
    for i in range(76, 83):
        sop = dsops[i][0]
        rop = drops[i][1]
        assert ccm.mnemonics_equivalent([sop.lower()], [rop.lower()]), (
            f'DIRQ parallel block desync at step {i}: src={sop} rom={rop}'
        )

    # 7) LDL _SECshared must resolve literal cell 0xB0BF -> descriptor 0xA130.
    sec_ptr, sec_target = resolve_literal_macro_target('CUSA', '_c_int00', 0x4AE6, 'LDL', '_SECshared')
    assert sec_ptr == 0xB0BF, f'_SECshared pointer expected 0xB0BF, got 0x{sec_ptr:08X}'
    assert sec_target == 0xA130, f'_SECshared descriptor expected 0xA130, got 0x{sec_target:08X}'

    # 8) TEXTIT startup0 must resolve literal cell 0xB0C5 -> string 0xC10253.
    text_ptr, text_target = resolve_literal_macro_target('CUSA', 'MESSAGE1', 0x4F43, 'TEXTIT', 'startup0')
    assert text_ptr == 0xB0C5, f'startup0 pointer expected 0xB0C5, got 0x{text_ptr:08X}'
    assert text_target == 0xC10253, f'startup0 target expected 0xC10253, got 0x{text_target:08X}'

    # 9) SPTR emits one inline text pointer word after its THEDATA string;
    #    this accounts for the five .word gap at 0x4FBB..0x4FBF before MSG3.
    cusa_lines = (ROOT / 'CUSA.ASM').read_text(errors='ignore').splitlines()
    sptr_words = source_rom_data_words_between(cusa_lines, 1967, 1974, macros, symbols)
    assert sptr_words == 5, f'SPTR gap expected 5 text words, got {sptr_words}'

    # 10) If IDA has already been labelled, operands can become symbolic
    #     without a definition line in the listing (for example @CPU_WS@CMOS).
    #     In that case, absolute .set symbols must still resolve from source.
    assert parse_rom_operand_addr('@CPU_WS@CMOS') is None
    cpu_ws = source_set_symbol_addr('CPU_WS', symbols)
    assert cpu_ws == 0x808064, f'CPU_WS .set expected 0x808064, got {cpu_ws!r}'
    fifo_control = source_set_symbol_addr('FIFO_CONTROL', symbols)
    assert fifo_control == 0x980080, f'FIFO_CONTROL .set expected 0x980080, got {fifo_control!r}'
    assert canonical_module_for_label('FIFO_STATUS', 'CUSA', set(), symbols) == ''
    assert canonical_module_for_label('startup0_ptr_0000B0C5', 'CUSA', set(), symbols) == ''

    # 11) VUNIT.EQU hardware/window addresses should be emitted even when a
    #     port is only referenced numerically in the listing/source walk.
    vunit_hw = parse_vunit_hardware_symbols(ROOT)
    assert vunit_hw['SWITCH2'] == 0x991050, f'SWITCH2 expected 0x991050, got {vunit_hw.get("SWITCH2")!r}'
    assert vunit_hw['FIFO_ADDR'] == 0x600000, f'FIFO_ADDR expected 0x600000, got {vunit_hw.get("FIFO_ADDR")!r}'
    assert 'WDOG' not in vunit_hw, 'WDOG bit mask should not be treated as a hardware address'

    # 12) address.map preserves name -> address facts that disappear once IDA
    #     renders operands symbolically in a regenerated listing.
    address_map = parse_address_map(ROOT / 'tools' / 'ida' / 'address.map')
    commintm = lookup_address_map_symbol('COMMINTM', 'CUSA', address_map)
    assert commintm == 0x8099DF, f'COMMINTM map expected 0x8099DF, got {commintm!r}'

    # 13) Non-macro source pseudo-ops like LDP should still leave a useful
    #     breadcrumb on their generated machine instruction.
    assert source_comment_tag('        LDP\t@FASTSTKI', macros) == 'LDP @FASTSTKI'

    # 14) IDA may list same-word parallel issue instructions in the opposite
    #     order from source.  The walker should reorder only within the same
    #     ROM address group when source has a "||" continuation.
    fake_lines = ['\tSTI\tR0,*AR0', ' ||\tLDI\t*AR1++,R0']
    fake_src_lno = [(1, 'STI', ['STI', 'R0', '*AR0']), (2, 'LDI', ['LDI', '*AR1++', 'R0'])]
    fake_rom_ops = [
        (0xAF1B, 'LDI', ['ldi', '*ar1++(1)', 'r0']),
        (0xAF1B, 'STI', ['sti', 'r0', '*ar0']),
    ]
    changed = reorder_same_address_parallel_rom_match(fake_lines, fake_src_lno, 0, 'STI', fake_rom_ops, 0)
    assert changed, 'parallel ROM same-address ops should reorder to match source order'
    assert fake_rom_ops[0][1] == 'STI' and fake_rom_ops[1][1] == 'LDI'

    print('ok: MESSAGE1 ACTIVE_SCREEN=0xCE43')
    print('ok: _pixel ACTIVE_SCREEN=0xCE43')
    print('ok: LOAD_SECTION_REQ @0x0000A3ED')
    print('ok: DIRQ OACTIVEI=0x00000040')
    print('ok: DIRQ OACTIVE_PRIORITYI=0x00000042')
    print('ok: DIRQ parallel || block aligns (steps 76..82)')
    print('ok: LDL _SECshared pointer=0x0000B0BF target=0x0000A130')
    print('ok: TEXTIT startup0 pointer=0x0000B0C5 target=0x00C10253')
    print('ok: CUSA SPTR data gap accounts for 0x4FBB..0x4FBF')
    print('ok: source .set fallback resolves CPU_WS/FIFO_CONTROL when ROM operand is symbolic')
    print('ok: .set labels canonicalize as globals')
    print('ok: VUNIT hardware symbols include SWITCH2 and skip bit masks')
    print('ok: address.map fallback resolves COMMINTM when ROM operand is symbolic')
    print('ok: LDP source lines emit semantic comments')
    print('ok: same-address parallel ROM ops reorder to source order')


if __name__ == '__main__':
    main()

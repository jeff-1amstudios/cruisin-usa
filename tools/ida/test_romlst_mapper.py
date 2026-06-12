#!/usr/bin/env python3
from __future__ import annotations

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))

from ida import shared_lib as ccm  # type: ignore
from ida.walk_source_and_rom import (
    assign_final_label_names,
    collect_include_gap_comments,
    collect_text_gap_word_symbol_rows,
    collect_word_block_symbol_rows,
    collect_literal_macro_refs,
    iter_expanded_lines,
    maybe_discovered_define,
    maybe_discovered_word_define,
    normalize_word_rom_read_ea,
    parse_src_ops,
    parse_rom_ops_from_addr,
    parse_int_token,
    parse_rom_operand_addr,
    parse_address_map,
    parse_globals_equ,
    parse_symbol_modules,
    parse_vunit_hardware_symbols,
    parse_source_label_lines,
    parse_source_storage_symbols,
    source_rom_data_words_between,
    source_symbol_starts_word_block,
    source_set_symbol_addr,
    parse_high_set_symbols,
    lookup_address_map_symbol,
    source_comment_tag,
    canonical_module_for_label,
    read_rom_word,
    reorder_same_address_parallel_rom_match,
    should_preserve_existing_word_block_row,
)

ROM = ROOT / 'roms' / 'test.rom.lst'


def find_src_start(module: str, label: str) -> int:
    p = find_source_module(module)
    lines = p.read_text(errors='ignore').splitlines()
    for i, ln in enumerate(lines):
        l, _ = ccm.split_optional_label(ccm.strip_comment(ln))
        if l == label:
            return i
    raise AssertionError(f'Could not find {label} in {p}')


def find_source_module(module: str) -> pathlib.Path:
    p = ccm.iter_source_files(ROOT, ('.ASM',))
    for candidate in p:
        if candidate.stem.upper() == module.upper():
            return candidate
    raise FileNotFoundError(f'missing source module: {module}')


def pair_until_symbol(module: str, label: str, addr: int, symbol: str) -> int:
    macros = ccm.parse_macros(ROOT)
    symbols = ccm.parse_set_symbols(ROOT)

    p = find_source_module(module)
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

    p = find_source_module(module)
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
    dlines = find_source_module('DIRQ').read_text(errors='ignore').splitlines()
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
    cusa_lines = find_source_module('CUSA').read_text(errors='ignore').splitlines()
    sptr_words = source_rom_data_words_between(cusa_lines, 1967, 1974, macros, symbols)
    assert sptr_words == 5, f'SPTR gap expected 5 text words, got {sptr_words}'

    # 10) Source .set symbols are still parsed as values, but are no longer
    #     used as an operand-address fallback during source<->ROM pairing.
    assert parse_rom_operand_addr('@CPU_WS@CMOS') is None
    assert parse_rom_operand_addr('INV_F30') is None
    assert maybe_discovered_define('bottom_gtmp_p', '#0') == ('bottom_gtmp_p', 0)
    assert source_set_symbol_addr('ADJ_COIN1_COUNTER', symbols) == 19
    assert maybe_discovered_define('@CPU_WS', '@808064h') is None
    assert maybe_discovered_define('@FASTSTKI', '#0') is None
    assert maybe_discovered_define('*FASTSTKI', '#0') is None
    assert maybe_discovered_define('R0', '#0') is None
    assert maybe_discovered_word_define('cvette_p', 15) == ('cvette_p', 15)
    assert maybe_discovered_word_define('hotrod_p', 18) == ('hotrod_p', 18)
    assert maybe_discovered_word_define('VETTTAB', 0x00C11643) is None
    menuentry_toks = ccm.parse_line_tokens('MENUENTRY "SPEED IN MPH OR KPH",MILES_OR_KILO')
    assert menuentry_toks == ['MENUENTRY', '"SPEED IN MPH OR KPH"', 'MILES_OR_KILO'], (
        f'MENUENTRY tokenization should preserve quoted strings, got {menuentry_toks!r}'
    )
    assert parse_rom_operand_addr('sub_64') == 0x64
    assert parse_rom_operand_addr('loc_106') == 0x106
    assert parse_rom_operand_addr('byte_40') == 0x40
    cpu_ws = source_set_symbol_addr('CPU_WS', symbols)
    assert cpu_ws == 0x808064, f'CPU_WS .set expected 0x808064, got {cpu_ws!r}'
    fifo_control = source_set_symbol_addr('FIFO_CONTROL', symbols)
    assert fifo_control == 0x980080, f'FIFO_CONTROL .set expected 0x980080, got {fifo_control!r}'
    assert canonical_module_for_label('FIFO_STATUS', 'CUSA', set(), symbols) == 'CUSA'
    assert canonical_module_for_label('startup0_ptr_0000B0C5', 'CUSA', set(), symbols) == 'CUSA'

    active_label_lines = parse_source_label_lines(ROOT)
    storage_symbols = parse_source_storage_symbols(ROOT)
    assert active_label_lines[('VERSION_STAMP', 'CUSA')][1] == 81, (
        f'active VERSION_STAMP line expected 81, got {active_label_lines[("VERSION_STAMP", "CUSA")]!r}'
    )
    assert 'BIGBUFFER' in storage_symbols, 'BIGBUFFER .bss declaration should count as source-defined'

    # 11) VUNIT.EQU hardware/window addresses are exportable only in the
    #     high MMIO/window range at 0x00600000 and above.
    vunit_hw = parse_vunit_hardware_symbols(ROOT)
    high_sets = parse_high_set_symbols(ROOT, symbols)
    assert vunit_hw['SWITCH2'] == 0x991050, f'SWITCH2 expected 0x991050, got {vunit_hw.get("SWITCH2")!r}'
    assert vunit_hw['FIFO_ADDR'] == 0x600000, f'FIFO_ADDR expected 0x600000, got {vunit_hw.get("FIFO_ADDR")!r}'
    assert 'WDOG' not in vunit_hw, 'WDOG bit mask should not be treated as a hardware address'
    assert high_sets['COMMPAL'] == 0x990000, f'COMMPAL expected 0x990000, got {high_sets.get("COMMPAL")!r}'
    assert high_sets['WHEEL'] == 0x995000, f'WHEEL expected 0x995000, got {high_sets.get("WHEEL")!r}'
    assert high_sets['COMMDP'] == 0x997000, f'COMMDP expected 0x997000, got {high_sets.get("COMMDP")!r}'
    assert high_sets['COMM_CTL'] == 0x997001, f'COMM_CTL expected 0x997001, got {high_sets.get("COMM_CTL")!r}'

    # 12) address.map is regenerated from seed anchors each run, so the
    #     VERSION_STAMP pointer targets must land on the live string and
    #     not on the dead .if 0 copy later in the source block.
    address_map = parse_address_map(ROOT / 'tools' / 'ida' / 'address.map')
    version_stamp = lookup_address_map_symbol('VERSION_STAMP', 'CUSA', address_map)
    assert version_stamp == 0x00C10288, f'VERSION_STAMP map expected 0x00C10288, got {version_stamp!r}'
    assert 0x00C102DC not in address_map.values(), 'stale dead-branch VERSION_STAMP address should not be regenerated'
    inv_f30 = lookup_address_map_symbol('INV_F30', 'ROUTS', address_map)
    assert inv_f30 == 0x0000A5B0, f'INV_F30 map expected 0x0000A5B0, got {inv_f30!r}'
    camera_pos_i = lookup_address_map_symbol('_CAMERAPOSI', 'ATTRDRNE', address_map)
    assert camera_pos_i == 0x0000B0DB, f'_CAMERAPOSI map expected 0x0000B0DB, got {camera_pos_i!r}'
    assert '_CAMERAPOS' not in address_map, '_CAMERAPOS .set alias should not be regenerated into address.map'
    assert '_CAMERARAD' not in address_map, '_CAMERARAD .set alias should not be regenerated into address.map'
    assert '_CAMERAMATRIX' not in address_map, '_CAMERAMATRIX .set alias should not be regenerated into address.map'
    assert address_map['FIFO_ADDR'] == 0x00600000, f'FIFO_ADDR map expected 0x00600000, got {address_map.get("FIFO_ADDR")!r}'
    assert address_map['SCREEN'] == 0x00900000, f'SCREEN map expected 0x00900000, got {address_map.get("SCREEN")!r}'
    assert address_map['COLORAM'] == 0x009E0000, f'COLORAM map expected 0x009E0000, got {address_map.get("COLORAM")!r}'
    assert address_map['COMMPAL'] == 0x00990000, f'COMMPAL map expected 0x00990000, got {address_map.get("COMMPAL")!r}'
    assert address_map['WHEEL'] == 0x00995000, f'WHEEL map expected 0x00995000, got {address_map.get("WHEEL")!r}'
    assert address_map['COMMDP'] == 0x00997000, f'COMMDP map expected 0x00997000, got {address_map.get("COMMDP")!r}'
    assert address_map['COMM_CTL'] == 0x00997001, f'COMM_CTL map expected 0x00997001, got {address_map.get("COMM_CTL")!r}'
    assert address_map['pi'] == 0x00CD195B, f'pi map expected 0x00CD195B, got {address_map.get("pi")!r}'
    assert 'COLORAM@MEMTEST' not in address_map, 'COLORAM .set should not be regenerated as a module-scoped operand alias'
    display_high_scores = lookup_address_map_symbol('DISPLAY_HIGH_SCORES', 'HSTDP', address_map)
    assert display_high_scores == 0x00003859, f'DISPLAY_HIGH_SCORES map expected 0x00003859, got {display_high_scores!r}'
    scroll_white_i = lookup_address_map_symbol('scroll_whiteI', 'HSTDP', address_map)
    assert scroll_white_i == 0x000038CC, f'scroll_whiteI map expected 0x000038CC, got {scroll_white_i!r}'
    scroll_white = lookup_address_map_symbol('scroll_white', 'HSTDP', address_map)
    assert scroll_white == 0x00C10A04, f'scroll_white map expected 0x00C10A04, got {scroll_white!r}'
    assert 'scroll_white@HSTDP' not in address_map, 'scroll_white@HSTDP should not be regenerated from dereferenced table data'
    ungh1_blue = lookup_address_map_symbol('ungh1_blue', 'BACKGRND', address_map)
    assert ungh1_blue == 0x00C103A2, f'ungh1_blue map expected 0x00C103A2, got {ungh1_blue!r}'
    exact_map = {'CAMERAPOSI': 0x45, '_CAMERAPOSI': 0xB0DB}
    assert lookup_address_map_symbol('_CAMERAPOSI', 'ATTRDRNE', exact_map) == 0xB0DB
    assert lookup_address_map_symbol('CAMERAPOSI', 'DIRQ', exact_map) == 0x45

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

    # 15) Source .include lines inside a source-only gap should emit a
    #     ``!include ...`` comment at the inferred ROM address where the
    #     included payload begins.
    include_gap_lines = [
        '\tLDI\t#1,R0',
        '\t.word\t1234h',
        '\t.include\tfoo.pal',
        '\t.word\t5678h',
        '\tLDI\t#2,R0',
    ]
    include_cmts = collect_include_gap_comments(include_gap_lines, 1, 5, 0x1001, symbols)
    assert include_cmts == [(0x1002, '!include foo.pal')], f'include gap comments mismatch: {include_cmts!r}'

    include_header_lines = [
        '\tLDI\t#1,R0',
        '\t.include\tDELTA.EQU',
        '\t.include\tRACER.EQU',
        '\tCLRI',
    ]
    include_header_cmts = collect_include_gap_comments(include_header_lines, 1, 4, 0x196A, symbols)
    assert include_header_cmts == [], f'non-payload include comments should be skipped: {include_header_cmts!r}'

    include_romdata_lines = [
        '\tLDI\t#1,R0',
        '\tromdata',
        '\t.include\tbabe.pal',
        '\t.text',
        '\tLDI\t#2,R0',
    ]
    include_romdata_cmts = collect_include_gap_comments(include_romdata_lines, 1, 5, 0x43AE, symbols)
    assert include_romdata_cmts == [(0x43AE, '!include babe.pal')], f'romdata include comments mismatch: {include_romdata_cmts!r}'

    globals_set = parse_globals_equ(ROOT)
    symbol_modules = parse_symbol_modules(ROOT)
    backgrnd_lines = find_source_module('BACKGRND').read_text(errors='ignore').splitlines()
    babe_rows = collect_text_gap_word_symbol_rows(
        backgrnd_lines, 1701, 1705, 0x4376, 'BACKGRND', macros, symbols, symbol_modules, globals_set, ROOT / 'roms' / 'crusnusa45_maindata_interleaved_bswap32.bin'
    )
    assert ('BABE_PALISTI', 'BACKGRND', 0x4376, False) in babe_rows, f'BABE_PALISTI gap row missing: {babe_rows!r}'
    assert ('BABE_PALIST', 'BACKGRND', 0x4376, True) in babe_rows, f'BABE_PALIST ref row missing: {babe_rows!r}'
    babe_block_rows = collect_word_block_symbol_rows(
        backgrnd_lines, 1705, 0x00C10398, 'BACKGRND', symbols, symbol_modules, globals_set, ROOT / 'roms' / 'crusnusa45_maindata_interleaved_bswap32.bin'
    )
    assert normalize_word_rom_read_ea(0x00C10398) == 0x10398
    assert ('BABE_PALIST', 'BACKGRND', 0x00C10398) in babe_block_rows, f'BABE_PALIST block row missing: {babe_block_rows!r}'
    assert ('ungh1_blue', 'BACKGRND', 0x00C10398) in babe_block_rows, f'ungh1_blue row missing: {babe_block_rows!r}'
    assert ('logo_p', 'BACKGRND', 0x00C10399) in babe_block_rows, f'logo_p row missing: {babe_block_rows!r}'

    diag_lines = find_source_module('DIAG').read_text(errors='ignore').splitlines()
    diag_gap_rows = collect_text_gap_word_symbol_rows(
        diag_lines, 1733, 1986, 0x00000BBA, 'DIAG', macros, symbols, symbol_modules, globals_set, ROOT / 'roms' / 'crusnusa45_maindata_interleaved_bswap32.bin'
    )
    assert ('COUNTRY_MENUTABI', 'DIAG', 0x00000BDB, False) in diag_gap_rows, (
        f'DIAG gap rows should advance to COUNTRY_MENUTABI cell at 0x00000BDB, got {diag_gap_rows!r}'
    )
    assert ('COUNTRY_MENUTAB', 'DIAG', 0x00000BDB, True) in diag_gap_rows, (
        f'DIAG gap rows should emit COUNTRY_MENUTAB from COUNTRY_MENUTABI cell, got {diag_gap_rows!r}'
    )

    intro_lines = find_source_module('INTRO').read_text(errors='ignore').splitlines()
    carsrcpal_rows = collect_word_block_symbol_rows(
        intro_lines, 2044, 0x00C10DBC, 'INTRO', symbols, symbol_modules, globals_set, ROOT / 'roms' / 'crusnusa45_maindata_interleaved_bswap32.bin'
    )
    assert ('cvette_p', 'INTRO', 0x00C10DBC) in carsrcpal_rows, f'cvette_p row missing: {carsrcpal_rows!r}'
    assert ('hotrod_p', 'INTRO', 0x00C10DBD) in carsrcpal_rows, f'hotrod_p row missing: {carsrcpal_rows!r}'
    cvette_p_value = read_rom_word(normalize_word_rom_read_ea(0x00C10DBC))
    hotrod_p_value = read_rom_word(normalize_word_rom_read_ea(0x00C10DBD))
    assert cvette_p_value == 15, f'cvette_p expected 15, got {cvette_p_value!r}'
    assert hotrod_p_value == 18, f'hotrod_p expected 18, got {hotrod_p_value!r}'
    assert maybe_discovered_word_define('cvette_p', cvette_p_value) == ('cvette_p', 15)
    assert maybe_discovered_word_define('hotrod_p', hotrod_p_value) == ('hotrod_p', 18)

    assert source_symbol_starts_word_block(ROOT, 'scroll_whiteI', 'HSTDP', active_label_lines, {}, symbols)
    assert source_symbol_starts_word_block(ROOT, 'FLASH_PALSI', 'HSTDP', active_label_lines, {}, symbols)
    assert not source_symbol_starts_word_block(ROOT, 'scroll_white', 'HSTDP', active_label_lines, {}, symbols)

    source_label_lines = {
        ('SPIN_CARTABI', 'ATTRACTA'): ('SPIN_CARTABI', 245),
        ('SPIN_CARTAB', 'ATTRACTA'): ('SPIN_CARTAB', 247),
    }
    existing_rows = {
        ('SPIN_CARTAB', 'ATTRACTA'): ('SPIN_CARTAB', 'SPIN_CARTAB@ATTRACTA', 'ATTRACTA', 'data', '0x00C10381', 'OK', '', ''),
    }
    assert should_preserve_existing_word_block_row(
        existing_rows,
        source_label_lines,
        globals_set,
        symbols,
        'SPIN_CARTABI',
        'SPIN_CARTAB',
        'ATTRACTA',
        0x0000A9F7,
    ), 'SPIN_CARTAB should keep its own word-block address instead of being overwritten by SPIN_CARTABI'

    final_names = assign_final_label_names({
        ('jeff', 'MODA'): ('jeff', 'jeff@MODA', 'MODA', 'label', '0x00000001', 'OK', '', ''),
        ('jeff', 'MODB'): ('jeff', 'jeff@MODB', 'MODB', 'label', '0x00000002', 'OK', '', ''),
        ('mike', 'MODC'): ('mike', 'mike@MODC', 'MODC', 'label', '0x00000003', 'OK', '', ''),
    })
    assert final_names[('jeff', 'MODA')] == 'jeff@MODA', f'jeff first duplicate expected jeff@MODA, got {final_names!r}'
    assert final_names[('jeff', 'MODB')] == 'jeff@MODB', f'jeff second duplicate expected jeff@MODB, got {final_names!r}'
    assert final_names[('mike', 'MODC')] == 'mike', f'mike unique label should stay unsuffixed, got {final_names!r}'

    country_menu_ptr = lookup_address_map_symbol('COUNTRY_MENUTABI', 'DIAG', address_map)
    assert country_menu_ptr == 0x00000BDB, f'COUNTRY_MENUTABI expected 0x00000BDB, got {country_menu_ptr!r}'
    country_menu = lookup_address_map_symbol('COUNTRY_MENUTAB', 'DIAG', address_map)
    assert country_menu == 0x00C131F9, f'COUNTRY_MENUTAB expected 0x00C131F9, got {country_menu!r}'
    assert 'COUNTRY_MENUTAB@DIAG' not in address_map, 'COUNTRY_MENUTAB@DIAG should not be emitted at wrong gap-derived address'

    overrides_path = ROOT / 'tools' / 'ida' / 'log' / 'romlst_overrides.tsv'
    overrides_text = overrides_path.read_text(errors='ignore')
    assert 'COUNTRY_MENUTAB' not in overrides_text, 'COUNTRY_MENUTAB should not require gap override after DIAG drift fix'

    print('ok: MESSAGE1 ACTIVE_SCREEN=0xCE43')
    print('ok: _pixel ACTIVE_SCREEN=0xCE43')
    print('ok: LOAD_SECTION_REQ @0x0000A3ED')
    print('ok: DIRQ OACTIVEI=0x00000040')
    print('ok: DIRQ OACTIVE_PRIORITYI=0x00000042')
    print('ok: DIRQ parallel || block aligns (steps 76..82)')
    print('ok: LDL _SECshared pointer=0x0000B0BF target=0x0000A130')
    print('ok: TEXTIT startup0 pointer=0x0000B0C5 target=0x00C10253')
    print('ok: CUSA SPTR data gap accounts for 0x4FBB..0x4FBF')
    print('ok: source .set values still parse, but are not used as ROM operand fallback')
    print('ok: parse_rom_operand_addr does not misread INV_F30 as 0xF30')
    print('ok: .set labels canonicalize as globals')
    print('ok: active-only source label scan keeps VERSION_STAMP on the live .else branch')
    print('ok: VUNIT MMIO symbols include SWITCH2/FIFO_ADDR and skip low-range entries/bit masks')
    print('ok: high non-VUNIT .set globals like COMMPAL/WHEEL/COMMDP/COMM_CTL are exportable')
    print('ok: regenerated address.map keeps VERSION_STAMP on 0x00C10288 and drops 0x00C102DC')
    print('ok: regenerated address.map keeps INV_F30 and _CAMERAPOSI at their pointer-cell addresses')
    print('ok: regenerated address.map keeps only .set exports at 0x00600000+ and skips DIRQ.EQU aliases')
    print('ok: label-only executable entries like DISPLAY_HIGH_SCORES are emitted from covered line mapping')
    print('ok: LDP source lines emit semantic comments')
    print('ok: same-address parallel ROM ops reorder to source order')
    print('ok: .include gap lines emit !include comments at inferred address')
    print('ok: .include EQU/ASM/INC lines do not emit !include comments')
    print('ok: romdata-wrapped payload includes still emit !include comments')
    print('ok: BABE_PALIST gap and block .word labels bind to table cell addresses')
    print('ok: DIAG gap follower descends through MENUENTRY macro output and reaches COUNTRY_MENUTABI at 0x00000BDB')
    print('ok: SPIN_CARTABI pointer cells do not overwrite SPIN_CARTAB table labels')
    print('ok: final emitted names keep @MODULE for duplicate labels')


if __name__ == '__main__':
    main()

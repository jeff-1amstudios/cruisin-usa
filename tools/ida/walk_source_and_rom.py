#!/usr/bin/env python3
from __future__ import annotations

import bisect
import csv
import datetime
import functools
import pathlib
import re
import sys
from dataclasses import dataclass
from typing import Dict, Iterable, List, Optional, Set, Tuple

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
import build_ida_dp0_script  # type: ignore
import build_ida_import_script  # type: ignore
import shared_lib as ccm  # type: ignore

ROM_LINE_RE = re.compile(r"^CODE:([0-9A-Fa-f]{8})\s+(.*)$")
LABEL_TOKEN_RE = re.compile(r"^[A-Za-z_.$?@][A-Za-z0-9_.$?@]*$")
NUMERIC_TOKEN_RE = re.compile(r"^(?:-?\d+|0x[0-9A-Fa-f]+|[0-9A-Fa-f]+h)$")
CALL_RE = re.compile(r"^(CALL(?:[A-Z]+)?)$", re.IGNORECASE)
# Branch-like control-flow mnemonics for label binding.
# Includes unconditional/direct forms, delayed forms, and common conditional
# Bxx variants. On the C3x, delayed branches execute the following
# instruction before the control transfer takes effect, but they still bind
# a normal branch target label.
BRANCH_RE = re.compile(
    r"^(?:B|BU|BR|BD|BUD|BRD|DB|DBU|DBR|DBD|"
    r"BEQ|BNE|BGT|BGE|BLT|BLE|BHI|BHS|BLO|BLS|BNV|BNN|BNZ|BZ|BC|BNC|"
    r"BEQD|BNED|BGTD|BGED|BLTD|BLED|BHID|BHSD|BLOD|BLSD|BNVD|BNND|BNZD|BZD|BCD|BNCD)$",
    re.IGNORECASE,
)
RPT_RE = re.compile(r"^(RPTB|RPTS)$", re.IGNORECASE)
ADDRESS_MAP_RE = re.compile(r"^\s*[0-9A-Fa-f]{4}:([0-9A-Fa-f]{8})\s+(\S+)\s*$")
SET_RE = re.compile(r"^\s*([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s+\.set\s+(.+)$", re.IGNORECASE)
GLOBL_RE = re.compile(r"^\.globl\s+(.+)$", re.IGNORECASE)
WORD_VALUE_RE = re.compile(r"\.word\s+([^\s;]+)", re.IGNORECASE)
PTR_SUFFIX_RE = re.compile(r"_ptr_[0-9A-Fa-f]{8}$")
PTR_NAME_RE = re.compile(r"^(.*)_ptr_([0-9A-Fa-f]{8})$")
PARALLEL_PREFIX_RE = re.compile(r"^\s*\|\|\s*")
LABEL_LIKE_RE = re.compile(r"^[A-Z_.$?][A-Z0-9_.$?]*$", re.IGNORECASE)
HEX_H_SUFFIX_RE = re.compile(r"^[0-9a-fA-F]+h$")
DECIMAL_RE = re.compile(r"^-?\d+$")
ADDR_SUFFIX_RE = re.compile(r"_([0-9A-Fa-f]{3,8})$")
IF_RE = re.compile(r"^\.if\s+(.+)$", re.IGNORECASE)
ELSE_RE = re.compile(r"^\.else\b", re.IGNORECASE)
ENDIF_RE = re.compile(r"^\.endif\b", re.IGNORECASE)
INCLUDE_RE = re.compile(r"^\.include\b", re.IGNORECASE)
TOKEN_SPLIT_RE = re.compile(r"[\s,]+")
REGISTER_RE = re.compile(r"^(AR[0-7]|R[0-7]|DP|IR[01]|BK|SP|ST|RS|RC|IE|IOF)$")
MACRO_START_RE = re.compile(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*\s+\.MACRO\b", re.IGNORECASE)
MACRO_END_RE = re.compile(r"^\.ENDM\b", re.IGNORECASE)
DEFAULT_WORD_ROM = pathlib.Path(__file__).resolve().parents[2] / "roms" / "crusnusa45_maindata_interleaved_bswap32.bin"
NON_PAYLOAD_INCLUDE_SUFFIXES = {".asm", ".equ", ".inc"}


@dataclass
class Anchor:
    src_label: str
    module: str
    rom_label: str
    addr: int


def is_auto_sub_name(name: str) -> bool:
    return name.lower().startswith("sub_")


def is_pseudo_local_label(name: str) -> bool:
    n = (name or "").strip()
    return n.startswith("$")


def is_payload_include_target(name: str) -> bool:
    target = (name or "").strip().strip('"').strip("'")
    if not target:
        return False
    return pathlib.PurePosixPath(target.lower()).suffix not in NON_PAYLOAD_INCLUDE_SUFFIXES


def normalize_word_rom_read_ea(ea: int, binary_path: pathlib.Path = DEFAULT_WORD_ROM) -> int:
    """Map high ROM-backed EAs to a readable linear word-ROM alias."""
    candidates: List[int] = []
    for cand in (ea, ea & 0x1FFFF, ea & 0xFFFFF, ea & 0xFFFF):
        if cand < 0 or cand in candidates:
            continue
        candidates.append(cand)
    for cand in candidates:
        if read_rom_word(cand, binary_path) is not None:
            return cand
    return ea


def symbol_key(name: Optional[str]) -> str:
    if not name:
        return ""
    return name.lstrip("_").upper()


def map_symbol_alias(name: str) -> str:
    return name[1:] if name.startswith("_") else name


def split_rom_qualified_label(name: str) -> Tuple[str, str]:
    if "@" not in name:
        return name, ""
    label, mod = name.rsplit("@", 1)
    if label and mod and LABEL_TOKEN_RE.match(label) and LABEL_TOKEN_RE.match(mod):
        return label, mod.upper()
    return name, ""


def parse_globals_equ(root: pathlib.Path) -> Set[str]:
    out: Set[str] = set()
    for p in ccm.iter_source_files(root, (".ASM", ".EQU")):
        for ln in p.read_text(errors="ignore").splitlines():
            code = ccm.strip_comment(ln).strip()
            m = GLOBL_RE.match(code)
            if not m:
                continue
            for tok in [t.strip() for t in m.group(1).split(",")]:
                if tok and LABEL_TOKEN_RE.match(tok):
                    out.add(tok.upper())
                    out.add(symbol_key(tok))
    return out


def parse_symbol_modules(root: pathlib.Path) -> Dict[str, Set[str]]:
    out: Dict[str, Set[str]] = {}
    for p in ccm.iter_source_files(root, (".ASM",)):
        mod = p.stem
        for ln in p.read_text(errors="ignore").splitlines():
            code = ccm.strip_comment(ln)
            lbl, _ = ccm.split_optional_label(code)
            if lbl and LABEL_TOKEN_RE.match(lbl):
                if is_pseudo_local_label(lbl):
                    continue
                out.setdefault(lbl.upper(), set()).add(mod)
                out.setdefault(symbol_key(lbl), set()).add(mod)
    return out


def symbol_modules_lookup(symbol_modules: Dict[str, Set[str]], sym: str) -> Set[str]:
    mods: Set[str] = set()
    mods.update(symbol_modules.get(sym.upper(), set()))
    mods.update(symbol_modules.get(symbol_key(sym), set()))
    return mods


def parse_rom_word_values(rom_path: pathlib.Path) -> Dict[int, int]:
    out: Dict[int, int] = {}
    for ln in rom_path.read_text(errors="ignore").splitlines():
        m = ROM_LINE_RE.match(ln)
        if not m:
            continue
        rhs = m.group(2)
        wm = WORD_VALUE_RE.search(rhs)
        if not wm:
            continue
        val = parse_int_token(wm.group(1))
        if val is not None:
            out[int(m.group(1), 16)] = val
    return out


@functools.lru_cache(maxsize=4)
def _read_binary_bytes(binary_path: str) -> bytes:
    return pathlib.Path(binary_path).read_bytes()


def read_rom_word(word_addr: int, binary_path: pathlib.Path = DEFAULT_WORD_ROM) -> Optional[int]:
    """Read a 32-bit big-endian TMS word from the canonical linear ROM.

    MAME/IDA addresses in this project are TMS word addresses, so word N is
    stored at byte offset N * 4 in the bswap32 binary.
    """
    if word_addr < 0:
        return None
    try:
        data = _read_binary_bytes(str(binary_path))
    except OSError:
        return None
    off = word_addr * 4
    if off + 4 > len(data):
        return None
    return int.from_bytes(data[off:off + 4], "big")


def parse_address_map(map_path: pathlib.Path) -> Dict[str, int]:
    """Parse an IDA map file into an exact name -> effective address map."""
    out: Dict[str, int] = {}
    if not map_path.exists():
        return out
    for ln in map_path.read_text(errors="ignore").splitlines():
        m = ADDRESS_MAP_RE.match(ln)
        if not m:
            continue
        ea = int(m.group(1), 16)
        name = m.group(2).strip()
        if not name:
            continue
        out.setdefault(name, ea)
    return out


def parse_vunit_hardware_symbols(root: pathlib.Path) -> Dict[str, int]:
    """Return memory-mapped VUNIT hardware symbols from VUNIT.EQU.

    VUNIT.EQU also contains many bit masks and small constants. Keep only
    absolute hardware/window addresses; skip low-valued flags and FASTRAM's
    zero alias so vector labels remain authoritative at address 0.
    """
    out: Dict[str, int] = {}
    vunit = next((p for p in ccm.iter_source_files(root, (".EQU",)) if p.stem.upper() == "VUNIT"), None)
    if vunit is None:
        return out
    symbols = ccm.parse_set_symbols(root)
    for raw in vunit.read_text(errors="ignore").splitlines():
        code = ccm.strip_comment(raw).strip()
        m = SET_RE.match(code)
        if not m:
            continue
        name = m.group(1)
        expr = m.group(2).strip()
        try:
            ea = ccm.eval_if_expr(expr, symbols)
        except Exception:
            continue
        if ea < 0x00600000:
            continue
        out.setdefault(name, ea)
    return out


def address_map_segment_for_ea(ea: int) -> str:
    # Keep ROM-backed labels visually grouped with the second IDA segment.
    return "0002" if ea >= 0x00C00000 else "0000"


def upsert_address_map(
    map_path: pathlib.Path,
    entries: Iterable[Tuple[str, int]],
    *,
    prune_scoped_aliases: bool = False,
) -> Tuple[int, int, int]:
    """Merge discovered name -> address facts into address.map.

    Returns (existing_count, inserted_count, updated_count).
    """
    existing: Dict[str, Tuple[str, int, str]] = {}
    header: List[str] = []
    saw_entry = False

    if map_path.exists():
        for ln in map_path.read_text(errors="ignore").splitlines():
            m = ADDRESS_MAP_RE.match(ln)
            if not m:
                if not saw_entry:
                    header.append(ln)
                continue
            saw_entry = True
            seg = ln.strip().split(":", 1)[0]
            ea = int(m.group(1), 16)
            name = m.group(2).strip()
            existing[name] = (name, ea, seg)

    if not header:
        header = [
            "",
            " Start         Length     Name                   Class",
            " 0000:00000000 000A00000H CODE                   ",
            " 0002:00000000 000A00000H seg001                 ",
            "",
            "",
            "  Address         Publics by Value",
            "",
        ]

    before = len(existing)
    inserted = 0
    updated = 0
    bare_entries: List[Tuple[str, int]] = []
    for name, ea in entries:
        if not name:
            continue
        if "@" not in name:
            bare_entries.append((name, ea))
        key = name
        seg = address_map_segment_for_ea(ea)
        old = existing.get(key)
        if old is None:
            existing[key] = (name, ea, seg)
            inserted += 1
        elif old[1] != ea:
            existing[key] = (old[0], ea, seg)
            updated += 1

    if prune_scoped_aliases:
        # If a bare/global name exists at an address, remove stale scoped aliases
        # for that same base name at the same address (e.g. _SECshared@CUSA).
        for name, ea in bare_entries:
            prefix = f"{name}@"
            for old_name, (_stored_name, old_ea, _seg) in list(existing.items()):
                if old_ea == ea and old_name.startswith(prefix):
                    del existing[old_name]

    # IDA can bake an assumed DP page into synthetic LDL/TEXTIT pointer-cell
    # names (e.g. CHOPPERANI_ptr_0080B14E) even though the real cell is low
    # FASTRAM (e.g. CHOPPERANI_ptr_0000B14E). If the canonical low-address
    # entry exists, drop the stale high-page alias from the persistent map.
    for old_name, (_stored_name, old_ea, _seg) in list(existing.items()):
        m = PTR_NAME_RE.match(old_name)
        if not m:
            continue
        base = m.group(1)
        canonical_ea = old_ea & 0x1FFFF
        if canonical_ea == old_ea:
            continue
        canonical_name = f"{base}_ptr_{canonical_ea:08X}"
        canonical = existing.get(canonical_name)
        if canonical and canonical[1] == canonical_ea:
            del existing[old_name]

    lines = list(header)
    if lines and lines[-1] != "":
        lines.append("")
    for name, ea, seg in sorted(existing.values(), key=lambda item: (item[1], item[0].upper())):
        lines.append(f" {seg}:{ea:08X}       {name}")
    map_path.write_text("\n".join(lines) + "\n")
    return before, inserted, updated


def lookup_address_map_symbol(sym: str, module: str, address_map: Dict[str, int]) -> Optional[int]:
    found = lookup_address_map_entry(sym, module, address_map)
    return found[1] if found is not None else None


def lookup_address_map_entry(sym: str, module: str, address_map: Dict[str, int]) -> Optional[Tuple[str, int]]:
    raw = sym.strip().strip(",").lstrip("@*#+-").strip("()")
    if not raw:
        return None
    candidates = [raw]
    skey = map_symbol_alias(raw)
    if skey and skey != raw:
        candidates.append(skey)
    if module:
        candidates.append(f"{raw}@{module}")
        if skey and skey != raw:
            candidates.append(f"{skey}@{module}")
    base, qmod = split_rom_qualified_label(raw)
    if qmod:
        candidates.append(f"{base}@{qmod}")
        bkey = map_symbol_alias(base)
        if bkey and bkey != base:
            candidates.append(f"{bkey}@{qmod}")
    seen_candidates: Set[str] = set()
    for key in candidates:
        if key in seen_candidates:
            continue
        seen_candidates.add(key)
        if key in address_map:
            return key, address_map[key]
    return None


def resolve_module_for_symbol(sym: str, fallback_mod: str, symbol_modules: Dict[str, Set[str]], globals_set: Set[str]) -> str:
    if sym.upper() in globals_set or symbol_key(sym) in globals_set:
        mods = symbol_modules_lookup(symbol_modules, sym)
        if len(mods) == 1:
            return next(iter(mods))
        return fallback_mod
    mods = symbol_modules_lookup(symbol_modules, sym)
    if len(mods) == 1:
        return next(iter(mods))
    if fallback_mod in mods:
        return fallback_mod
    return fallback_mod


def canonical_module_for_label(lbl: str, mod: str, globals_set: Set[str], set_symbols: Dict[str, int]) -> str:
    # Synthetic pointer-cell labels include the cell address, so they are
    # already globally unique (e.g. startup0_ptr_0000B0C5).
    if PTR_SUFFIX_RE.search(lbl):
        return ""
    # .EQU/.ASM .set values are absolute symbols. Treat them as global
    # even when referenced from module-scoped code.
    if lbl.upper() in set_symbols:
        return ""
    # Globals should collapse to a single namespace key so we don't emit
    # one row per caller module (e.g. ENABLEGIE duplicated across modules).
    if lbl.upper() in globals_set or symbol_key(lbl) in globals_set:
        return ""
    return mod


@functools.lru_cache(maxsize=512)
def find_module_source_path(root: pathlib.Path, module: str) -> Optional[pathlib.Path]:
    umod = module.upper()
    for p in ccm.iter_source_files(root, (".ASM",)):
        if p.stem.upper() == umod:
            return p
    return None


def find_function_start_in_module(root: pathlib.Path, module: str, label: str) -> Optional[int]:
    p = find_module_source_path(root, module)
    if p is None:
        return None
    lines = p.read_text(errors="ignore").splitlines()
    for i, ln in enumerate(lines):
        code = ccm.strip_comment(ln)
        lbl, _ = ccm.split_optional_label(code)
        if lbl == label or symbol_key(lbl) == symbol_key(label):
            return i
    return None


def extract_function_lines(root: pathlib.Path, module: str, label: str) -> Optional[List[str]]:
    start = find_function_start_in_module(root, module, label)
    if start is None:
        return None
    p = find_module_source_path(root, module)
    if p is None:
        return None
    return ccm.extract_source_function_lines(p, label, start)


def extract_module_tail_lines(root: pathlib.Path, module: str, label: str) -> Optional[List[str]]:
    start = find_function_start_in_module(root, module, label)
    if start is None:
        return None
    p = find_module_source_path(root, module)
    if p is None:
        return None
    lines = p.read_text(errors="ignore").splitlines()
    return lines[start:]


def is_column0_label_only(code: str, toks: List[str]) -> bool:
    return (
        bool(code)
        and not code[:1].isspace()
        and len(toks) == 1
        and LABEL_TOKEN_RE.match(toks[0]) is not None
    )


def iter_expanded_lines(lines: List[str], macros: Dict[str, ccm.MacroDef], symbols: Dict[str, int], recursion_limit: int = 10) -> List[str]:
    out: List[str] = []
    expander = ccm.MacroExpander(macros, symbols)

    def walk(code: str, depth: int, active: Set[str]) -> None:
        if depth > recursion_limit:
            return
        code = ccm.strip_comment(code)
        if not code.strip() or code.strip().startswith("*"):
            return
        if code.lstrip().startswith("||"):
            code = PARALLEL_PREFIX_RE.sub("", code)
            if not code.strip():
                return
            code = "\t" + code
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks:
            return
        if is_column0_label_only(code, toks):
            out.append(code)
            return
        op = toks[op_idx].upper()
        if (
            op_idx == 1
            and len(toks) >= 3
            and LABEL_LIKE_RE.match(toks[0])
            and not toks[0].endswith(":")
        ):
            op = toks[1].upper()
        if op in macros:
            if op in active:
                return
            active.add(op)
            args = expander.parse_invocation_args(op_idx, toks)
            expanded = expander.expand(op, args)
            for b in ccm.iter_active_lines(expanded, symbols):
                walk(b, depth + 1, active)
            active.remove(op)
            return
        out.append(code)

    for ln in ccm.iter_active_lines(lines, symbols):
        walk(ln, 0, set())
    return out


def parse_int_token(tok: str) -> Optional[int]:
    t = tok.strip().strip(",")
    t = t.lstrip("@")
    if t.lower().startswith("0x"):
        try:
            return int(t, 16)
        except Exception:
            return None
    if t.lower().endswith("h") and HEX_H_SUFFIX_RE.match(t):
        try:
            return int(t[:-1], 16)
        except Exception:
            return None
    if DECIMAL_RE.match(t):
        try:
            return int(t, 10)
        except Exception:
            return None
    return None


def parse_rom_operand_addr(tok: str) -> Optional[int]:
    s = tok.strip().strip(",").lstrip("@")
    v = parse_int_token(s)
    if v is not None:
        return v
    m = ADDR_SUFFIX_RE.search(s)
    if m:
        try:
            return int(m.group(1), 16)
        except Exception:
            return None
    return None


def normalize_literal_cell_ea(ea: int, binary_path: pathlib.Path) -> int:
    """Canonicalize LDL/TEXTIT pointer-cell addresses polluted by IDA DP state.

    IDA sometimes renders a literal-cell operand as e.g. ``@byte_80B14E`` or
    ``@foo_ptr_0098C67D`` when the real cell lives in low FASTRAM at
    ``0x0000B14E`` / ``0x0000C67D``. For literal cells we prefer the lowest
    readable canonical address.
    """
    # Temporarily disabled so the mapper exposes raw .lst operand addresses
    # again. This lets us inspect wrong assumed-DP cases instead of silently
    # canonicalizing them down into low FASTRAM.
    #
    # candidates: List[int] = []
    # for cand in (ea, ea & 0x1FFFF, ea & 0xFFFF):
    #     if cand < 0 or cand in candidates:
    #         continue
    #     candidates.append(cand)
    # for cand in candidates:
    #     if read_rom_word(cand, binary_path) is not None:
    #         return cand
    return ea


def source_set_symbol_addr(sym: str, symbols: Dict[str, int]) -> Optional[int]:
    """Resolve absolute source .set symbols when IDA hid the numeric operand."""
    key = sym.strip().strip(",").lstrip("@*#+-").strip("()").upper()
    if key in symbols:
        return symbols[key]
    return None


def resolve_paired_operand_addr(
    rom_tok: str,
    source_sym: str,
    module: str,
    symbols: Dict[str, int],
    address_map: Dict[str, int],
    *,
    allow_source_set: bool,
) -> Optional[int]:
    ea = parse_rom_operand_addr(rom_tok)
    if ea is not None:
        return ea
    if allow_source_set:
        ea = source_set_symbol_addr(source_sym, symbols)
        if ea is not None:
            return ea
    ea = lookup_address_map_symbol(source_sym, module, address_map)
    if ea is not None:
        return ea
    return lookup_address_map_symbol(rom_tok, module, address_map)


def macro_invocation_tag(raw_line: str, macros: Dict[str, ccm.MacroDef]) -> Optional[str]:
    code = ccm.strip_comment(raw_line)
    if not code.strip():
        return None
    lbl, _rest = ccm.split_optional_label(code)
    op_idx, toks = ccm.split_label_and_tokens(code)
    if not toks or op_idx >= len(toks):
        return None
    op = toks[op_idx].upper()
    if (
        op_idx == 1
        and len(toks) >= 3
        and LABEL_LIKE_RE.match(toks[0])
        and not toks[0].endswith(":")
    ):
        op = toks[1].upper()
    if op in macros:
        if lbl and LABEL_TOKEN_RE.match(lbl) and not is_pseudo_local_label(lbl):
            return lbl
        return op
    return None


def source_comment_tag(raw_line: str, macros: Dict[str, ccm.MacroDef]) -> Optional[str]:
    mname = macro_invocation_tag(raw_line, macros)
    if mname:
        return mname
    code = ccm.strip_comment(raw_line)
    if not code.strip():
        return None
    op_idx, toks = ccm.split_label_and_tokens(code)
    if not toks or op_idx >= len(toks):
        return None
    op = toks[op_idx].upper()
    if op == "LDP" and len(toks) > op_idx + 1:
        return f"LDP {toks[op_idx + 1].strip().strip(',')}"
    return None


def collect_include_gap_comments(
    lines: List[str],
    start_line: int,
    end_line: int,
    start_ea: int,
    symbols: Dict[str, int],
) -> List[Tuple[int, str]]:
    """Collect ``!include`` comments and their inferred addresses in a source gap.

    We walk raw source lines between two matched source instruction lines and
    maintain a best-effort ROM-address cursor. ``.include`` itself does not
    consume words, so the comment lands at the current cursor where the
    included payload begins.
    """

    def _split_asm_args(s: str) -> List[str]:
        out: List[str] = []
        cur: List[str] = []
        quote = ""
        i = 0
        while i < len(s):
            ch = s[i]
            if quote:
                cur.append(ch)
                if ch == quote:
                    quote = ""
                i += 1
                continue
            if ch in {"'", '"'}:
                quote = ch
                cur.append(ch)
                i += 1
                continue
            if ch == ",":
                arg = "".join(cur).strip()
                if arg:
                    out.append(arg)
                cur = []
                i += 1
                continue
            cur.append(ch)
            i += 1
        arg = "".join(cur).strip()
        if arg:
            out.append(arg)
        return out

    def _string_words_from_code(code: str) -> int:
        m = re.search(r"\.string\b(.*)$", code, re.IGNORECASE)
        if not m:
            return 1
        byte_count = 0
        for arg in _split_asm_args(m.group(1)):
            a = arg.strip()
            if len(a) >= 2 and a[0] in {"'", '"'} and a[-1] == a[0]:
                byte_count += len(a[1:-1])
            else:
                byte_count += 1
        return max(1, (byte_count + 3) // 4)

    out: List[Tuple[int, str]] = []
    lo = max(1, start_line + 1)
    hi = max(lo, end_line)
    if lo > len(lines):
        return out

    cursor = start_ea
    section = "text"
    for _lineno, raw in iter_active_raw_with_lineno(lines[lo - 1:hi - 1], lo, symbols):
        code = ccm.strip_comment(raw).strip()
        if not code:
            continue
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].lower()
        if op == ".text" or (op == ".sect" and len(toks) > op_idx + 1 and toks[op_idx + 1].strip('"').upper() == "THECODE"):
            section = "text"
            continue
        if op in {".data", ".const", "romdata"}:
            section = "data"
            continue
        if op == ".sect" and len(toks) > op_idx + 1:
            sec = toks[op_idx + 1].strip('"').upper()
            section = "text" if sec == "THECODE" else "data"
            continue
        if op in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
            section = "bss"
            continue
        if INCLUDE_RE.match(code):
            arg = toks[op_idx + 1].strip().strip(",") if len(toks) > op_idx + 1 else ""
            if is_payload_include_target(arg):
                out.append((cursor, f"!include {arg}"))
            continue
        if section == "bss":
            continue
        if op in {".word", ".float"}:
            cursor += max(1, len(toks) - op_idx - 1)
        elif op == ".double":
            cursor += 2 * max(1, len(toks) - op_idx - 1)
        elif op == ".string":
            cursor += _string_words_from_code(code)
    return out


def iter_active_raw_with_lineno(lines: List[str], base_lineno: int, symbols: Dict[str, int]) -> List[Tuple[int, str]]:
    out: List[Tuple[int, str]] = []
    active_stack: List[bool] = [True]
    in_macro = 0
    skip_longrout_body = False
    for i, ln in enumerate(lines):
        code = ccm.strip_comment(ln).strip()
        if not code:
            continue
        if code.startswith("*"):
            continue
        if in_macro:
            if MACRO_END_RE.match(code):
                in_macro -= 1
            elif MACRO_START_RE.match(code):
                in_macro += 1
            continue
        if MACRO_START_RE.match(code):
            in_macro += 1
            continue
        ifm = IF_RE.match(code)
        if ifm:
            cond = ccm.eval_if_expr(ifm.group(1), symbols) != 0
            active_stack.append(active_stack[-1] and cond)
            continue
        if ELSE_RE.match(code):
            if len(active_stack) > 1:
                parent = active_stack[-2]
                current = active_stack[-1]
                active_stack[-1] = parent and (not current)
            continue
        if ENDIF_RE.match(code):
            if len(active_stack) > 1:
                active_stack.pop()
            continue
        if active_stack[-1]:
            if skip_longrout_body:
                if ccm.is_text_directive(code) or ccm.is_longrout_end(code):
                    skip_longrout_body = False
                continue
            out.append((base_lineno + i, ln))
            if ccm.is_longrout_start(code):
                skip_longrout_body = True
    return out


def iter_expanded_lines_with_lineno(
    lines: List[str],
    base_lineno: int,
    macros: Dict[str, ccm.MacroDef],
    symbols: Dict[str, int],
    recursion_limit: int = 10,
) -> List[Tuple[int, str]]:
    out: List[Tuple[int, str]] = []
    expander = ccm.MacroExpander(macros, symbols)

    def walk(lineno: int, code: str, depth: int, active: Set[str]) -> None:
        if depth > recursion_limit:
            return
        code = ccm.strip_comment(code)
        if not code.strip() or code.strip().startswith("*"):
            return
        if code.lstrip().startswith("||"):
            code = PARALLEL_PREFIX_RE.sub("", code)
            if not code.strip():
                return
            code = "\t" + code
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks:
            return
        if is_column0_label_only(code, toks):
            out.append((lineno, code))
            return
        op = toks[op_idx].upper()
        if (
            op_idx == 1
            and len(toks) >= 3
            and LABEL_LIKE_RE.match(toks[0])
            and not toks[0].endswith(":")
        ):
            op = toks[1].upper()
        if op in macros:
            if op in active:
                return
            active.add(op)
            args = expander.parse_invocation_args(op_idx, toks)
            expanded = expander.expand(op, args)
            for b in ccm.iter_active_lines(expanded, symbols):
                walk(lineno, b, depth + 1, active)
            active.remove(op)
            return
        out.append((lineno, code))

    for lineno, ln in iter_active_raw_with_lineno(lines, base_lineno, symbols):
        walk(lineno, ln, 0, set())
    return out


def parse_raw_ops_with_lineno(lines: List[str], base_lineno: int, symbols: Dict[str, int]) -> List[Tuple[int, str, List[str]]]:
    out: List[Tuple[int, str, List[str]]] = []
    for lineno, ln in iter_active_raw_with_lineno(lines, base_lineno, symbols):
        code = ccm.strip_comment(ln)
        if not code.strip():
            continue
        if code.lstrip().startswith("||"):
            code = PARALLEL_PREFIX_RE.sub("", code)
            if not code.strip():
                continue
            code = "\t" + code
        lbl, rest = ccm.split_optional_label(code)
        if lbl is not None:
            if not rest:
                continue
            # Avoid column-0 relabeling of the remainder (e.g. "NOP ...").
            code = "\t" + rest
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].upper()
        if op.startswith("||"):
            op = op[2:]
        if op.startswith(".") or op == "EQU":
            continue
        out.append((lineno, op, toks[op_idx:]))
    return out


def parse_ops_with_lineno(expanded_with_lineno: List[Tuple[int, str]]) -> List[Tuple[int, str, List[str]]]:
    out: List[Tuple[int, str, List[str]]] = []
    for lineno, code_in in expanded_with_lineno:
        code = ccm.strip_comment(code_in)
        if not code.strip():
            continue
        if code.lstrip().startswith("||"):
            code = PARALLEL_PREFIX_RE.sub("", code)
            if not code.strip():
                continue
            code = "\t" + code
        lbl, rest = ccm.split_optional_label(code)
        if lbl is not None:
            if not rest:
                continue
            code = "\t" + rest
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].upper()
        if op.startswith("||"):
            op = op[2:]
        if op.startswith(".") or op == "EQU":
            continue
        out.append((lineno, op, toks[op_idx:]))
    return out


def collect_literal_macro_refs(lines: List[str], base_lineno: int, symbols: Dict[str, int]) -> List[Tuple[int, str, str]]:
    refs: List[Tuple[int, str, str]] = []
    for lineno, ln in iter_active_raw_with_lineno(lines, base_lineno, symbols):
        code = ccm.strip_comment(ln)
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].upper()
        if op not in {"LDL", "TEXTIT"} or op_idx + 1 >= len(toks):
            continue
        label = toks[op_idx + 1].strip().strip(",")
        if LABEL_TOKEN_RE.match(label) and not NUMERIC_TOKEN_RE.match(label):
            refs.append((lineno, op, label))
        else:
            refs.append((lineno, op, ""))
    return refs


def collect_dp0_op_indexes(src_ops_lno: List[Tuple[int, str, List[str]]]) -> Set[int]:
    out: Set[int] = set()
    for i, (_lineno, op_in, toks) in enumerate(src_ops_lno):
        op = op_in.upper()
        if op == "SETDP":
            out.add(i)
            continue
        if op == "CLRI" and len(toks) >= 2 and toks[1].strip().strip(",").upper() == "DP":
            out.add(i)
            continue
        if op == "POP" and len(toks) >= 2 and toks[1].strip().strip(",").upper() == "DP":
            out.add(i)
            continue
        if (
            op == "SUBI"
            and len(toks) >= 3
            and toks[1].strip().strip(",").upper() == "DP"
            and toks[2].strip().strip(",").upper() == "DP"
        ):
                out.add(i)
    return out


def source_line_is_parallel_continuation(module_lines: List[str], lineno: int) -> bool:
    if lineno <= 0 or lineno > len(module_lines):
        return False
    code = ccm.strip_comment(module_lines[lineno - 1])
    return code.lstrip().startswith("||")


def source_has_parallel_context(
    module_lines: List[str],
    src_ops_lno: List[Tuple[int, str, List[str]]],
    si: int,
) -> bool:
    if si >= len(src_ops_lno):
        return False
    curr_line = src_ops_lno[si][0]
    if source_line_is_parallel_continuation(module_lines, curr_line):
        return True
    if si + 1 < len(src_ops_lno):
        next_line = src_ops_lno[si + 1][0]
        if source_line_is_parallel_continuation(module_lines, next_line):
            return True
    return False


def reorder_same_address_parallel_rom_match(
    module_lines: List[str],
    src_ops_lno: List[Tuple[int, str, List[str]]],
    si: int,
    sop: str,
    rom_ops: List[Tuple[int, str, List[str]]],
    ri: int,
) -> bool:
    """Put the same-address ROM op matching source at ri.

    IDA may print C3x parallel issue words in the opposite order from source,
    e.g. source "STI / || LDI" versus listing "LDI / ||STI".  Treat only
    adjacent same-address ROM ops as reorderable, and only when source context
    shows a parallel issue line.
    """
    if ri >= len(rom_ops):
        return False
    if not source_has_parallel_context(module_lines, src_ops_lno, si):
        return False
    ea = rom_ops[ri][0]
    end = ri + 1
    while end < len(rom_ops) and rom_ops[end][0] == ea:
        end += 1
    if end == ri + 1:
        return False
    for mj in range(ri + 1, end):
        if ccm.mnemonics_equivalent([sop.lower()], [rom_ops[mj][1].lower()]):
            rom_ops[ri], rom_ops[mj] = rom_ops[mj], rom_ops[ri]
            return True
    return False


def collect_align_lines(lines: List[str], base_lineno: int, symbols: Dict[str, int]) -> List[int]:
    out: List[int] = []
    for lineno, ln in iter_active_raw_with_lineno(lines, base_lineno, symbols):
        code = ccm.strip_comment(ln)
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        if toks[op_idx].upper() == ".ALIGN":
            out.append(lineno)
    return out


def macro_emits_executable(
    macro_name: str,
    macros: Dict[str, ccm.MacroDef],
    symbols: Dict[str, int],
    cache: Dict[str, bool],
    active: Set[str],
) -> bool:
    name = macro_name.upper()
    if name in cache:
        return cache[name]
    if name in active or name not in macros:
        return False
    active.add(name)
    macro = macros[name]
    expander = ccm.MacroExpander(macros, symbols)
    expanded = expander.expand(name, ["0"] * len(macro.params))
    for ln in ccm.iter_active_lines(expanded, symbols):
        code = ccm.strip_comment(ln)
        if not code.strip():
            continue
        lbl, rest = ccm.split_optional_label(code)
        if lbl is not None:
            if not rest:
                continue
            code = "\t" + rest
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].upper()
        if op in macros:
            if macro_emits_executable(op, macros, symbols, cache, active):
                cache[name] = True
                active.remove(name)
                return True
            continue
        if op.startswith(".") or op == "EQU":
            continue
        if op in {"FBSS", "PBSS", "HIBSS", "LOBSS", "PHIBSS", "ROMDATA", "FSECT", "PSECT", "HSECT", "DIAGTEXT"}:
            continue
        cache[name] = True
        active.remove(name)
        return True
    active.remove(name)
    cache[name] = False
    return False


def collect_instruction_line_numbers(
    lines: List[str],
    symbols: Dict[str, int],
    macros: Dict[str, ccm.MacroDef],
) -> Set[int]:
    out: Set[int] = set()
    non_exec_ops = {
        "FBSS",
        "PBSS",
        "HIBSS",
        "LOBSS",
        "PHIBSS",
        "ROMDATA",
        "FSECT",
        "PSECT",
        "HSECT",
        "DIAGTEXT",
    }
    macro_exec_cache: Dict[str, bool] = {}
    for lineno, op, _toks in parse_raw_ops_with_lineno(lines, 1, symbols):
        opu = op.upper()
        if opu in non_exec_ops:
            continue
        if opu in macros and not macro_emits_executable(opu, macros, symbols, macro_exec_cache, set()):
            continue
        out.add(lineno)
    return out


def source_code_label_index(
    root: pathlib.Path,
    symbols: Dict[str, int],
    macros: Dict[str, ccm.MacroDef],
) -> Set[Tuple[str, str]]:
    """Return (symbol_key(label), MODULE) pairs that bind to executable source.

    This mirrors the coverage instruction classifier, but carries pending
    label-only lines forward so aliases like:

        FOO
        BAR
            LDI 1,R0

    are all treated as code labels.
    """
    non_exec_ops = {
        "FBSS",
        "PBSS",
        "HIBSS",
        "LOBSS",
        "PHIBSS",
        "ROMDATA",
        "FSECT",
        "PSECT",
        "HSECT",
        "DIAGTEXT",
    }
    data_ops = {
        ".BYTE",
        ".WORD",
        ".FLOAT",
        ".DOUBLE",
        ".STRING",
        ".SPACE",
        ".USECT",
        ".BSS",
        ".SET",
        "EQU",
        "RGB",
    }
    out: Set[Tuple[str, str]] = set()
    macro_exec_cache: Dict[str, bool] = {}

    def op_is_code(op: str) -> bool:
        opu = op.upper()
        if opu in non_exec_ops or opu in data_ops:
            return False
        if opu.startswith("."):
            return False
        if opu in macros:
            return macro_emits_executable(opu, macros, symbols, macro_exec_cache, set())
        return True

    for p in ccm.iter_source_files(root, (".ASM",)):
        mod = p.stem
        pending: List[str] = []
        lines = p.read_text(errors="ignore").splitlines()
        for _lineno, raw in iter_active_raw_with_lineno(lines, 1, symbols):
            code = ccm.strip_comment(raw)
            if not code.strip():
                continue
            if code.lstrip().startswith("||"):
                code = PARALLEL_PREFIX_RE.sub("", code)
                if not code.strip():
                    continue
                code = "\t" + code
            lbl, rest = ccm.split_optional_label(code)
            if lbl is not None:
                if is_pseudo_local_label(lbl):
                    lbl = None
                elif not rest:
                    pending.append(lbl)
                    continue
                else:
                    pending.append(lbl)
                    code = "\t" + rest
            op_idx, toks = ccm.split_label_and_tokens(code)
            if not toks or op_idx >= len(toks):
                continue
            op = toks[op_idx].upper()
            if op.startswith("||"):
                op = op[2:]
            if op_is_code(op):
                for plbl in pending:
                    out.add((symbol_key(plbl), mod.upper()))
            pending.clear()
    return out


def source_code_label_order(
    root: pathlib.Path,
    symbols: Dict[str, int],
    macros: Dict[str, ccm.MacroDef],
) -> List[Tuple[str, str, int]]:
    """Return executable source labels in module/source order as (MODULE, label, line)."""
    non_exec_ops = {
        "FBSS",
        "PBSS",
        "HIBSS",
        "LOBSS",
        "PHIBSS",
        "ROMDATA",
        "FSECT",
        "PSECT",
        "HSECT",
        "DIAGTEXT",
    }
    data_ops = {
        ".BYTE",
        ".WORD",
        ".FLOAT",
        ".DOUBLE",
        ".STRING",
        ".SPACE",
        ".USECT",
        ".BSS",
        ".SET",
        "EQU",
        "RGB",
    }
    out: List[Tuple[str, str, int]] = []
    macro_exec_cache: Dict[str, bool] = {}

    def op_is_code(op: str) -> bool:
        opu = op.upper()
        if opu in non_exec_ops or opu in data_ops:
            return False
        if opu.startswith("."):
            return False
        if opu in macros:
            return macro_emits_executable(opu, macros, symbols, macro_exec_cache, set())
        return True

    for p in ccm.iter_source_files(root, (".ASM",)):
        mod = p.stem.upper()
        pending: List[Tuple[str, int]] = []
        lines = p.read_text(errors="ignore").splitlines()
        for lineno, raw in iter_active_raw_with_lineno(lines, 1, symbols):
            code = ccm.strip_comment(raw)
            if not code.strip():
                continue
            if code.lstrip().startswith("||"):
                code = PARALLEL_PREFIX_RE.sub("", code)
                if not code.strip():
                    continue
                code = "\t" + code
            lbl, rest = ccm.split_optional_label(code)
            if lbl is not None:
                if is_pseudo_local_label(lbl):
                    lbl = None
                elif not rest:
                    pending.append((lbl, lineno))
                    continue
                else:
                    pending.append((lbl, lineno))
                    code = "\t" + rest
            op_idx, toks = ccm.split_label_and_tokens(code)
            if not toks or op_idx >= len(toks):
                continue
            op = toks[op_idx].upper()
            if op.startswith("||"):
                op = op[2:]
            if op_is_code(op):
                out.extend((mod, plbl, pline) for plbl, pline in pending)
            pending.clear()
    return out


def collect_source_call_target_keys(root: pathlib.Path, symbols: Dict[str, int]) -> Set[str]:
    out: Set[str] = set()
    for p in ccm.iter_source_files(root, (".ASM",)):
        lines = p.read_text(errors="ignore").splitlines()
        for _lineno, op, toks in parse_raw_ops_with_lineno(lines, 1, symbols):
            if not CALL_RE.match(op) or len(toks) < 2:
                continue
            target = toks[1].strip().strip(",")
            if LABEL_TOKEN_RE.match(target) and not NUMERIC_TOKEN_RE.match(target):
                out.add(symbol_key(target))
    out.add(symbol_key("_c_int00"))
    return out


def source_rom_data_words_between(
    lines: List[str],
    start_line: int,
    end_line: int,
    macros: Dict[str, ccm.MacroDef],
    symbols: Dict[str, int],
) -> int:
    def _split_asm_args(s: str) -> List[str]:
        out: List[str] = []
        cur: List[str] = []
        quote = ""
        i = 0
        while i < len(s):
            ch = s[i]
            if quote:
                cur.append(ch)
                if ch == quote:
                    quote = ""
                i += 1
                continue
            if ch in {"'", '"'}:
                quote = ch
                cur.append(ch)
                i += 1
                continue
            if ch == ",":
                arg = "".join(cur).strip()
                if arg:
                    out.append(arg)
                cur = []
                i += 1
                continue
            cur.append(ch)
            i += 1
        arg = "".join(cur).strip()
        if arg:
            out.append(arg)
        return out

    def _string_words_from_code(code: str) -> int:
        m = re.search(r"\.string\b(.*)$", code, re.IGNORECASE)
        if not m:
            return 1
        byte_count = 0
        for arg in _split_asm_args(m.group(1)):
            a = arg.strip()
            if len(a) >= 2 and a[0] in {"'", '"'} and a[-1] == a[0]:
                byte_count += len(a[1:-1])
            else:
                # TI .string accepts byte-valued numeric operands such as the
                # trailing zero in: .string "HELLO",0.
                byte_count += 1
        return max(1, (byte_count + 3) // 4)

    total = 0
    lo = max(1, start_line + 1)
    hi = max(lo, end_line)
    if lo > len(lines):
        return 0
    section = "text"
    expanded = iter_expanded_lines_with_lineno(lines[lo - 1:hi - 1], lo, macros, symbols)
    for _lineno, ln in expanded:
        code = ccm.strip_comment(ln)
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].lower()
        if op == ".text" or (op == ".sect" and len(toks) > op_idx + 1 and toks[op_idx + 1].strip('"').upper() == "THECODE"):
            section = "text"
            continue
        if op in {".data", ".const"}:
            section = "data"
            continue
        if op == ".sect" and len(toks) > op_idx + 1:
            section = "text" if toks[op_idx + 1].strip('"').upper() == "THECODE" else "data"
            continue
        if section != "text":
            continue
        if op == ".word" or op == ".float":
            total += max(1, len(toks) - op_idx - 1)
        elif op == ".double":
            total += 2 * max(1, len(toks) - op_idx - 1)
        elif op == ".string":
            total += _string_words_from_code(code)
    return total


def source_rom_data_words_on_line(
    lines: List[str],
    line: int,
    macros: Dict[str, ccm.MacroDef],
    symbols: Dict[str, int],
) -> int:
    if line <= 0 or line > len(lines):
        return 0
    # Macro bodies can emit inline data between executable instructions while
    # all expanded ops still carry the same original source line number.  LCALL
    # does this with its local target pointer word.
    return source_rom_data_words_between(lines, line - 1, line + 1, macros, symbols)


def source_has_unknown_data_between(
    lines: List[str],
    start_line: int,
    end_line: int,
    symbols: Dict[str, int],
) -> bool:
    lo = max(1, start_line + 1)
    hi = max(lo, end_line)
    if lo > len(lines):
        return False
    for _lineno, ln in iter_active_raw_with_lineno(lines[lo - 1:hi - 1], lo, symbols):
        code = ccm.strip_comment(ln).strip()
        if INCLUDE_RE.match(code):
            return True
    return False


def parse_source_label_lines(root: pathlib.Path) -> Dict[Tuple[str, str], Tuple[str, int]]:
    out: Dict[Tuple[str, str], Tuple[str, int]] = {}
    for p in ccm.iter_source_files(root, (".ASM",)):
        mod = p.stem.upper()
        for lineno, raw in enumerate(p.read_text(errors="ignore").splitlines(), start=1):
            code = ccm.strip_comment(raw)
            lbl, _rest = ccm.split_optional_label(code)
            if not lbl or not LABEL_TOKEN_RE.match(lbl) or is_pseudo_local_label(lbl):
                continue
            out.setdefault((symbol_key(lbl), mod), (lbl, lineno))
    return out


def should_preserve_existing_word_block_row(
    rows: Dict[Tuple[str, str], Tuple[str, str, str, str, str, str, str, str]],
    source_label_lines: Dict[Tuple[str, str], Tuple[str, int]],
    globals_set: Set[str],
    symbols: Dict[str, int],
    parent_lbl: str,
    child_lbl: str,
    child_mod: str,
    child_ea: int,
) -> bool:
    mod = canonical_module_for_label(child_lbl, child_mod, globals_set, symbols)
    existing = rows.get((child_lbl, mod))
    if existing is None or int(existing[4], 16) == child_ea:
        return False
    if symbol_key(child_lbl) == symbol_key(parent_lbl):
        return False
    return (symbol_key(child_lbl), mod.upper()) in source_label_lines


def source_label_starts_word_block(lines: List[str], line: int, symbols: Dict[str, int]) -> bool:
    if line <= 0 or line > len(lines):
        return False
    for _lineno, raw in iter_active_raw_with_lineno(lines[line - 1:], line, symbols):
        code = ccm.strip_comment(raw)
        if not code.strip():
            continue
        lbl, rest = ccm.split_optional_label(code)
        if lbl is not None and not rest:
            continue
        if lbl is not None and rest:
            code = "\t" + rest
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            return False
        op = toks[op_idx].lower()
        return op in {".word", ".float", ".double", ".string"}
    return False


def _collect_text_gap_word_symbol_rows_ex(
    lines: List[str],
    start_line: int,
    end_line: int,
    start_ea: int,
    module: str,
    symbols: Dict[str, int],
    symbol_modules: Dict[str, Set[str]],
    globals_set: Set[str],
    word_rom: pathlib.Path,
) -> List[Tuple[str, str, int, bool, bool]]:
    out: List[Tuple[str, str, int, bool, bool]] = []
    lo = max(1, start_line + 1)
    hi = max(lo, end_line)
    if lo > len(lines):
        return out

    cursor = start_ea
    section = "text"
    pending: List[str] = []
    for _lineno, raw in iter_active_raw_with_lineno(lines[lo - 1:hi - 1], lo, symbols):
        code = ccm.strip_comment(raw)
        if not code.strip():
            continue
        lbl, rest = ccm.split_optional_label(code)
        if lbl and LABEL_TOKEN_RE.match(lbl) and not is_pseudo_local_label(lbl):
            pending.append(lbl)
            if not rest:
                continue
            code = "\t" + rest
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].lower()
        if op == ".text" or (op == ".sect" and len(toks) > op_idx + 1 and toks[op_idx + 1].strip('"').upper() == "THECODE"):
            section = "text"
            pending.clear()
            continue
        if op in {".data", ".const", "romdata"}:
            section = "data"
            pending.clear()
            continue
        if op == ".sect" and len(toks) > op_idx + 1:
            section = "text" if toks[op_idx + 1].strip('"').upper() == "THECODE" else "data"
            pending.clear()
            continue
        if op in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
            pending.clear()
            continue
        if section != "text":
            continue
        if op == ".word":
            pending_aliases = {
                symbol_key(plbl[:-1])
                for plbl in pending
                if len(plbl) > 1 and plbl.upper().endswith("I")
            }
            for plbl in pending:
                lmod = resolve_module_for_symbol(plbl, module, symbol_modules, globals_set)
                out.append((plbl, lmod, cursor, False, False))
            pending.clear()
            args = [tok.strip().strip(",") for tok in toks[op_idx + 1:]]
            for idx, arg in enumerate(args):
                if not arg or NUMERIC_TOKEN_RE.match(arg) or not LABEL_TOKEN_RE.match(arg):
                    continue
                tmod = resolve_module_for_symbol(arg, module, symbol_modules, globals_set)
                out.append((arg, tmod, cursor + idx, True, symbol_key(arg) in pending_aliases))
            cursor += max(1, len(args))
            continue
        if op == ".float":
            for plbl in pending:
                lmod = resolve_module_for_symbol(plbl, module, symbol_modules, globals_set)
                out.append((plbl, lmod, cursor, False, False))
            pending.clear()
            cursor += max(1, len(toks) - op_idx - 1)
            continue
        if op == ".double":
            for plbl in pending:
                lmod = resolve_module_for_symbol(plbl, module, symbol_modules, globals_set)
                out.append((plbl, lmod, cursor, False, False))
            pending.clear()
            cursor += 2 * max(1, len(toks) - op_idx - 1)
            continue
        if op == ".string":
            for plbl in pending:
                lmod = resolve_module_for_symbol(plbl, module, symbol_modules, globals_set)
                out.append((plbl, lmod, cursor, False, False))
            pending.clear()
            cursor += source_rom_data_words_on_line(lines, _lineno, {}, symbols)
            continue
        pending.clear()
    return out


def collect_text_gap_word_symbol_rows(
    lines: List[str],
    start_line: int,
    end_line: int,
    start_ea: int,
    module: str,
    symbols: Dict[str, int],
    symbol_modules: Dict[str, Set[str]],
    globals_set: Set[str],
    word_rom: pathlib.Path,
) -> List[Tuple[str, str, int, bool]]:
    return [
        (lbl, mod, ea, is_word_ref)
        for lbl, mod, ea, is_word_ref, _has_inline_alias in _collect_text_gap_word_symbol_rows_ex(
            lines,
            start_line,
            end_line,
            start_ea,
            module,
            symbols,
            symbol_modules,
            globals_set,
            word_rom,
        )
    ]


def _collect_word_block_symbol_rows_ex(
    lines: List[str],
    start_line: int,
    start_ea: int,
    module: str,
    symbols: Dict[str, int],
    symbol_modules: Dict[str, Set[str]],
    globals_set: Set[str],
    word_rom: pathlib.Path,
    *,
    include_word_refs: bool = True,
) -> List[Tuple[str, str, int, bool, bool]]:
    out: List[Tuple[str, str, int, bool, bool]] = []
    if start_line <= 0 or start_line > len(lines):
        return out

    cursor = start_ea
    pending: List[str] = []
    consumed_any = False
    for _lineno, raw in iter_active_raw_with_lineno(lines[start_line - 1:], start_line, symbols):
        code = ccm.strip_comment(raw)
        if not code.strip():
            continue
        lbl, rest = ccm.split_optional_label(code)
        if lbl and LABEL_TOKEN_RE.match(lbl) and not is_pseudo_local_label(lbl):
            pending.append(lbl)
            if not rest:
                continue
            code = "\t" + rest
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].lower()
        if op == ".word":
            pending_aliases = {
                symbol_key(plbl[:-1])
                for plbl in pending
                if len(plbl) > 1 and plbl.upper().endswith("I")
            }
            for plbl in pending:
                lmod = resolve_module_for_symbol(plbl, module, symbol_modules, globals_set)
                out.append((plbl, lmod, cursor, False, False))
            pending.clear()
            args = [tok.strip().strip(",") for tok in toks[op_idx + 1:]]
            if include_word_refs:
                for idx, arg in enumerate(args):
                    if not arg or NUMERIC_TOKEN_RE.match(arg) or not LABEL_TOKEN_RE.match(arg):
                        continue
                    tmod = resolve_module_for_symbol(arg, module, symbol_modules, globals_set)
                    out.append((arg, tmod, cursor + idx, True, symbol_key(arg) in pending_aliases))
            cursor += max(1, len(args))
            consumed_any = True
            continue
        if op in {".float"}:
            for plbl in pending:
                lmod = resolve_module_for_symbol(plbl, module, symbol_modules, globals_set)
                out.append((plbl, lmod, cursor, False, False))
            pending.clear()
            cursor += max(1, len(toks) - op_idx - 1)
            consumed_any = True
            continue
        if op == ".double":
            for plbl in pending:
                lmod = resolve_module_for_symbol(plbl, module, symbol_modules, globals_set)
                out.append((plbl, lmod, cursor, False, False))
            pending.clear()
            cursor += 2 * max(1, len(toks) - op_idx - 1)
            consumed_any = True
            continue
        if op == ".string":
            for plbl in pending:
                lmod = resolve_module_for_symbol(plbl, module, symbol_modules, globals_set)
                out.append((plbl, lmod, cursor, False, False))
            pending.clear()
            cursor += source_rom_data_words_on_line(lines, _lineno, {}, symbols)
            consumed_any = True
            continue
        if op in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
            pending.clear()
            continue
        if op in {".text", ".sect", ".data", ".const", "romdata"}:
            if consumed_any:
                break
            pending.clear()
            continue
        if consumed_any:
            break
        pending.clear()
    return out


def collect_word_block_symbol_rows(
    lines: List[str],
    start_line: int,
    start_ea: int,
    module: str,
    symbols: Dict[str, int],
    symbol_modules: Dict[str, Set[str]],
    globals_set: Set[str],
    word_rom: pathlib.Path,
    *,
    include_word_refs: bool = True,
) -> List[Tuple[str, str, int]]:
    return [
        (lbl, mod, ea)
        for lbl, mod, ea, _is_word_ref, _has_inline_alias in _collect_word_block_symbol_rows_ex(
            lines,
            start_line,
            start_ea,
            module,
            symbols,
            symbol_modules,
            globals_set,
            word_rom,
            include_word_refs=include_word_refs,
        )
    ]


def line_set_to_ranges(lines: Set[int]) -> str:
    if not lines:
        return ""
    out: List[str] = []
    s = sorted(lines)
    lo = s[0]
    hi = s[0]
    for n in s[1:]:
        if n == hi + 1:
            hi = n
            continue
        out.append(f"{lo}-{hi}" if lo != hi else f"{lo}")
        lo = hi = n
    out.append(f"{lo}-{hi}" if lo != hi else f"{lo}")
    return ", ".join(out)


def find_next_matching_src_op(
    src_ops: List[Tuple[str, List[str]]],
    start_idx: int,
    rom_op: str,
    max_scan: int = 64,
) -> Optional[int]:
    end = min(len(src_ops), start_idx + max_scan + 1)
    for idx in range(max(0, start_idx), end):
        sop, _stoks = src_ops[idx]
        if ccm.mnemonics_equivalent([sop.lower()], [rom_op.lower()]):
            return idx
    return None


def find_gap_resync_src_op(
    src_ops: List[Tuple[str, List[str]]],
    src_ops_lno: List[Tuple[int, str, List[str]]],
    lines: List[str],
    start_idx: int,
    prev_line: int,
    gap_words: int,
    rom_op: str,
    macros: Dict[str, ccm.MacroDef],
    symbols: Dict[str, int],
    max_scan: int = 128,
) -> Optional[Tuple[int, int, int]]:
    end = min(len(src_ops), start_idx + max_scan + 1)
    for idx in range(max(0, start_idx), end):
        sop, _stoks = src_ops[idx]
        if not ccm.mnemonics_equivalent([sop.lower()], [rom_op.lower()]):
            continue
        curr_line = src_ops_lno[idx][0] if idx < len(src_ops_lno) else prev_line
        skipped_ops = idx - start_idx
        inline_words = source_rom_data_words_between(lines, prev_line, curr_line, macros, symbols)
        if skipped_ops + inline_words == gap_words:
            return idx, skipped_ops, inline_words
    return None


def source_has_end_after(lines: List[str], line: int) -> bool:
    for ln in lines[max(0, line - 1):]:
        code = ccm.strip_comment(ln)
        op_idx, toks = ccm.split_label_and_tokens(code)
        if toks and op_idx < len(toks) and toks[op_idx].upper() == ".END":
            return True
    return False


def parse_src_ops(lines: List[str]) -> List[Tuple[str, List[str]]]:
    out: List[Tuple[str, List[str]]] = []
    for ln in lines:
        code = ccm.strip_comment(ln)
        if not code.strip():
            continue
        if code.lstrip().startswith("||"):
            code = PARALLEL_PREFIX_RE.sub("", code)
            if not code.strip():
                continue
            # Keep column-0 label heuristics from misclassifying this as a label.
            code = "\t" + code
        lbl, rest = ccm.split_optional_label(code)
        if lbl is not None:
            if not rest:
                continue
            # Avoid column-0 relabeling of the remainder (e.g. "NOP ...").
            code = "\t" + rest
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].upper()
        if op.startswith("||"):
            op = op[2:]
        if op.startswith(".") or op == "EQU":
            continue
        out.append((op, toks[op_idx:]))
    return out


def parse_rom_ops(rom_lines: List[str]) -> List[Tuple[int, str, List[str]]]:
    out: List[Tuple[int, str, List[str]]] = []
    for ln in rom_lines:
        m = ROM_LINE_RE.match(ln)
        if not m:
            continue
        ea = int(m.group(1), 16)
        rhs = m.group(2).strip()
        if not rhs:
            continue
        if ":" in rhs:
            l, r = rhs.split(":", 1)
            if LABEL_TOKEN_RE.match(l.strip()):
                rhs = r.strip()
        if not rhs or rhs.startswith(";") or rhs.startswith("."):
            continue
        toks = [t for t in TOKEN_SPLIT_RE.split(rhs) if t]
        if not toks:
            continue
        op = toks[0].upper()
        if op.startswith("||"):
            op = op[2:]
        out.append((ea, op, toks))
    return out


def parse_rom_ops_from_addr(rom_path: pathlib.Path, start_addr: int) -> List[Tuple[int, str, List[str]]]:
    out: List[Tuple[int, str, List[str]]] = []
    started = False
    for ln in rom_path.read_text(errors="ignore").splitlines():
        m = ROM_LINE_RE.match(ln)
        if not m:
            continue
        ea = int(m.group(1), 16)
        if not started:
            if ea < start_addr:
                continue
            started = True
        rhs = m.group(2).strip()
        if not rhs:
            continue
        if ":" in rhs:
            l, r = rhs.split(":", 1)
            if LABEL_TOKEN_RE.match(l.strip()):
                rhs = r.strip()
        if not rhs or rhs.startswith(";") or rhs.startswith("."):
            continue
        toks = [t for t in TOKEN_SPLIT_RE.split(rhs) if t]
        if not toks:
            continue
        op = toks[0].upper()
        if op.startswith("||"):
            op = op[2:]
        out.append((ea, op, toks))
    return out


def parse_all_rom_ops(rom_path: pathlib.Path) -> List[Tuple[int, str, List[str]]]:
    return parse_rom_ops_from_addr(rom_path, 0)


def collect_source_operand_symbols(root: pathlib.Path, symbols: Dict[str, int]) -> Set[Tuple[str, str]]:
    out: Set[Tuple[str, str]] = set()
    for p in ccm.iter_source_files(root, (".ASM",)):
        mod = p.stem
        lines = p.read_text(errors="ignore").splitlines()
        for ln in ccm.iter_active_lines(lines, symbols):
            code = ccm.strip_comment(ln)
            op_idx, toks = ccm.split_label_and_tokens(code)
            if not toks or op_idx >= len(toks):
                continue
            for tok in toks[op_idx + 1:]:
                if "@" not in tok:
                    continue
                s = tok.strip().strip(",")
                s = s.lstrip("@*#+-").strip("()")
                if not s or NUMERIC_TOKEN_RE.match(s) or not LABEL_TOKEN_RE.match(s):
                    continue
                up = s.upper()
                if REGISTER_RE.match(up):
                    continue
                out.add((s, mod))
    return out


def main() -> None:
    here = pathlib.Path(__file__).resolve().parent
    log_dir = here / "log"
    log_dir.mkdir(parents=True, exist_ok=True)
    root = pathlib.Path(__file__).resolve().parents[2]
    rom = pathlib.Path("/Users/j.harris/Downloads/carma/cruisin/crusnusa45_maindata_interleaved_bswap32.bin.lst")
    word_rom = root / "roms" / "crusnusa45_maindata_interleaved_bswap32.bin"
    outp = log_dir / "romlst_labels.tsv"
    unp = log_dir / "romlst_unresolved.tsv"
    py_out = here / "ida_label_import.py"
    run_log = log_dir / "romlst_run_log.tsv"
    canonical_map = here / "address.map"
    ida_map = pathlib.Path("~/Downloads/carma/cruisin/ida-address.map").expanduser()

    globals_set = parse_globals_equ(root)
    symbol_modules = parse_symbol_modules(root)
    macros = ccm.parse_macros(root)
    symbols = ccm.parse_set_symbols(root)
    vunit_hardware_symbols = parse_vunit_hardware_symbols(root)
    ida_map_before = 0
    ida_map_inserted = 0
    ida_map_updated = 0
    if ida_map.exists():
        ida_entries = parse_address_map(ida_map)
        ida_map_before, ida_map_inserted, ida_map_updated = upsert_address_map(canonical_map, ida_entries.items())
    address_map = parse_address_map(canonical_map)
    rom_ops_all = parse_all_rom_ops(rom)
    rom_ops_addrs = [ea for ea, _op, _toks in rom_ops_all]
    rom_ops_addr_set = set(rom_ops_addrs)

    def has_decoded_rom_op(ea: int) -> bool:
        return ea in rom_ops_addr_set

    source_code_labels = source_code_label_index(root, symbols, macros)
    source_labels_ordered = source_code_label_order(root, symbols, macros)
    source_call_target_keys = collect_source_call_target_keys(root, symbols)
    source_label_lines = parse_source_label_lines(root)

    module_anchor_candidates: Dict[str, Anchor] = {}
    for mod, src_fn, _line in source_labels_ordered:
        if mod in module_anchor_candidates:
            continue
        if is_auto_sub_name(src_fn) or is_pseudo_local_label(src_fn) or src_fn.endswith("?"):
            continue
        skey = symbol_key(src_fn)
        if not mod or (skey, mod.upper()) not in source_code_labels:
            continue
        # Initial module walks start at the first executable source label in
        # that module that already has an address. Other functions are still
        # enqueued when a paired walk encounters CALL operands.
        found = lookup_address_map_entry(src_fn, mod, address_map)
        if found is None:
            continue
        map_name, ea = found
        if not has_decoded_rom_op(ea):
            continue
        module_anchor_candidates[mod] = Anchor(src_label=src_fn, module=mod, rom_label=map_name, addr=ea)

    anchors: Dict[Tuple[str, str], Anchor] = {
        (a.src_label, a.module): a
        for a in sorted(module_anchor_candidates.values(), key=lambda a: (a.module, a.addr, a.src_label.upper()))
    }

    rows: Dict[Tuple[str, str], Tuple[str, str, str, str, str, str, str, str]] = {}
    unresolved: List[Tuple[str, str, str, str, str]] = []
    dp0_marks: Set[int] = set()
    macro_comments: Dict[int, Set[str]] = {}

    def add_row(lbl: str, mod: str, kind: str, ea: int, *, overwrite: bool = False) -> None:
        if is_auto_sub_name(lbl):
            return
        if is_pseudo_local_label(lbl):
            return
        if lbl.endswith("?"):
            return
        mod = canonical_module_for_label(lbl, mod, globals_set, symbols)
        name = lbl if (lbl.upper() in globals_set or not mod) else f"{lbl}@{mod}"
        row = (lbl, name, mod, kind, f"0x{ea:08X}", "OK", "", "")
        if overwrite:
            rows[(lbl, mod)] = row
        else:
            rows.setdefault((lbl, mod), row)

    def has_row(lbl: str, mod: str) -> bool:
        mod = canonical_module_for_label(lbl, mod, globals_set, symbols)
        return (lbl, mod) in rows

    def classify_address_map_entry(lbl: str, mod: str) -> str:
        skey = symbol_key(lbl)
        umod = mod.upper() if mod else ""
        if PTR_SUFFIX_RE.search(lbl):
            return "data"
        if umod and (skey, umod) in source_code_labels:
            if skey in source_call_target_keys or skey in globals_set:
                return "code"
            return "label"
        if not umod:
            code_mods = {m for key, m in source_code_labels if key == skey}
            if code_mods:
                if skey in source_call_target_keys or skey in globals_set:
                    return "code"
                return "label"
        return "data"

    def promote_same_address_module_aliases() -> int:
        grouped: Dict[Tuple[str, int], List[Tuple[Tuple[str, str], Tuple[str, str, str, str, str, str, str, str]]]] = {}
        for key, row in rows.items():
            grouped.setdefault((row[0], int(row[4], 16)), []).append((key, row))

        promoted = 0
        kind_rank = {"data": 0, "label": 1, "code": 2}
        for (lbl, ea), vals in grouped.items():
            names = {row[1] for _key, row in vals}
            mods = {row[2] for _key, row in vals}
            if len(vals) < 2:
                continue
            if len(mods) < 2 and "" not in mods:
                continue
            if names == {lbl}:
                continue

            best = max((row for _key, row in vals), key=lambda r: kind_rank.get(r[3], 0))
            promoted_row = (lbl, lbl, "", best[3], f"0x{ea:08X}", "OK", best[6], best[7])
            for key, _row in vals:
                rows.pop(key, None)
            rows[(lbl, "")] = promoted_row
            promoted += len(vals) - 1
        return promoted

    # address.map is the persistent name -> address database. Seed rows from it
    # so regenerated output does not depend on IDA's current .lst label text.
    for map_name, ea in sorted(address_map.items(), key=lambda kv: (kv[1], kv[0].upper())):
        if is_auto_sub_name(map_name):
            continue
        lbl, mod = split_rom_qualified_label(map_name)
        if is_auto_sub_name(lbl) or is_pseudo_local_label(lbl) or lbl.endswith("?"):
            continue
        add_row(lbl, mod.upper() if mod else "", classify_address_map_entry(lbl, mod), ea)

    for (_fn, _mod), a in anchors.items():
        add_row(a.src_label, a.module, "code", a.addr)

    # Iterate through anchored functions and harvest call targets + @symbol operands.
    queue: List[Anchor] = sorted(anchors.values(), key=lambda a: a.addr)
    seen: Set[Tuple[str, str]] = set()
    module_lines_cache: Dict[str, List[str]] = {}
    module_covered_lines: Dict[str, Set[int]] = {}
    module_walked_lines: Dict[str, Set[int]] = {}
    module_line_rom_ea: Dict[Tuple[str, int], int] = {}
    module_walk_ranges: Dict[str, List[Tuple[int, int, str]]] = {}
    module_instruction_lines: Dict[str, Set[int]] = {}

    while queue:
        a = queue.pop(0)
        k = (a.src_label, a.module)
        if k in seen:
            continue
        seen.add(k)

        module = a.module
        if not module:
            mods = symbol_modules.get(a.src_label.upper(), set())
            if len(mods) == 1:
                module = next(iter(mods))
        if not module:
            unresolved.append((a.src_label, "", "function_module_unresolved", f"0x{a.addr:08X}", ""))
            continue

        start = find_function_start_in_module(root, module, a.src_label)
        if start is None:
            unresolved.append((a.src_label, module, "source_function_not_found", f"0x{a.addr:08X}", ""))
            continue
        start_line = start + 1
        if module not in module_lines_cache:
            p = find_module_source_path(root, module)
            if p is None:
                unresolved.append((a.src_label, module, "source_module_not_found", f"0x{a.addr:08X}", ""))
                continue
            module_lines_cache[module] = p.read_text(errors="ignore").splitlines()
            module_instruction_lines[module] = collect_instruction_line_numbers(module_lines_cache[module], symbols, macros)
        src_lines = module_lines_cache[module][start:]
        if not src_lines:
            unresolved.append((a.src_label, module, "source_function_empty", f"0x{a.addr:08X}", ""))
            continue

        expanded = iter_expanded_lines(src_lines, macros, symbols)
        src_ops = parse_src_ops(expanded)
        exp_ln = iter_expanded_lines_with_lineno(src_lines, start + 1, macros, symbols)
        src_ops_lno = parse_ops_with_lineno(exp_ln)
        literal_refs = collect_literal_macro_refs(src_lines, start + 1, symbols)
        align_lines = collect_align_lines(src_lines, start + 1, symbols)
        dp0_op_indexes = collect_dp0_op_indexes(src_ops_lno)
        literal_ref_idx = 0
        align_idx = 0
        prev_matched_line = 0
        prev_matched_ea: Optional[int] = None
        ridx = bisect.bisect_left(rom_ops_addrs, a.addr)
        rom_ops = rom_ops_all[ridx:]

        # Bind CALL/branch targets and @symbol operands from matched
        # source<->ROM instruction pairs.
        matched_n = 0
        walk_stop_line = start_line
        si = 0
        ri = 0
        while si < len(src_ops) and ri < len(rom_ops):
            sop, stoks = src_ops[si]
            curr_src_line = src_ops_lno[si][0] if si < len(src_ops_lno) else 0
            prev_src_line = src_ops_lno[si - 1][0] if si > 0 and (si - 1) < len(src_ops_lno) else 0
            # If source had .align between previous and current source op,
            # consume zero-or-more ROM NOPs before matching mnemonics.
            saw_align = False
            while align_idx < len(align_lines):
                al = align_lines[align_idx]
                if al <= prev_src_line:
                    align_idx += 1
                    continue
                if curr_src_line and al < curr_src_line:
                    saw_align = True
                    align_idx += 1
                    continue
                break
            if saw_align:
                while ri < len(rom_ops) and rom_ops[ri][1].upper() == "NOP":
                    ri += 1
                if ri >= len(rom_ops):
                    exhausted_at = (rom_ops[-1][0] + 1) if rom_ops else a.addr
                    unresolved.append((
                        a.src_label,
                        module,
                        "module_walk_rom_exhausted",
                        f"after .align near line={curr_src_line} rom_exhausted_at=0x{exhausted_at:08X}",
                        str(curr_src_line),
                    ))
                    break
            rea, rop, rtoks = rom_ops[ri]
            if curr_src_line and curr_src_line in module_walked_lines.get(module, set()):
                previous_ea = module_line_rom_ea.get((module, curr_src_line))
                if previous_ea is not None and previous_ea != rea:
                    unresolved.append((
                        a.src_label,
                        module,
                        "source_line_rom_conflict",
                        f"line={curr_src_line} previous=0x{previous_ea:08X} current=0x{rea:08X}",
                        str(curr_src_line),
                    ))
                break
            if ri > 0:
                prev_rea = rom_ops[ri - 1][0]
                if rea > prev_rea + 1:
                    gap_words = rea - prev_rea - 1
                    prev_line = src_ops_lno[si - 1][0] if si > 0 and si - 1 < len(src_ops_lno) else 0
                    curr_line = src_ops_lno[si][0] if si < len(src_ops_lno) else prev_line
                    inline_words = source_rom_data_words_between(module_lines_cache[module], prev_line, curr_line, macros, symbols)
                    if inline_words == 0 and prev_line and prev_line == curr_line:
                        inline_words = source_rom_data_words_on_line(module_lines_cache[module], curr_line, macros, symbols)
                    unknown_data = source_has_unknown_data_between(module_lines_cache[module], prev_line, curr_line, symbols)
                    if inline_words < gap_words and not unknown_data:
                        gap_line = str(curr_line or si)
                        unresolved.append((
                            a.src_label,
                            module,
                            "rom_data_in_code",
                            f"line={gap_line} src={sop} rom_words={gap_words} source_words={inline_words} rom_data_at=0x{prev_rea + 1:08X} next_code=0x{rea:08X}",
                            gap_line,
                        ))
                        break
            if not ccm.mnemonics_equivalent([sop.lower()], [rop.lower()]):
                if reorder_same_address_parallel_rom_match(
                    module_lines_cache[module],
                    src_ops_lno,
                    si,
                    sop,
                    rom_ops,
                    ri,
                ):
                    rea, rop, rtoks = rom_ops[ri]
                else:
                    desync_line = ""
                    if si < len(src_ops_lno):
                        desync_line = str(src_ops_lno[si][0])
                        walk_stop_line = src_ops_lno[si][0]
                    unresolved.append((
                        a.src_label,
                        module,
                        "module_walk_desync",
                        f"line={desync_line or si} src={sop} rom={rop} at=0x{rea:08X}",
                        desync_line,
                    ))
                    break
            matched_n += 1
            if si < len(src_ops_lno):
                matched_line = src_ops_lno[si][0]
                if prev_matched_ea is not None and matched_line > prev_matched_line + 1:
                    for cmt_ea, cmt in collect_include_gap_comments(
                        module_lines_cache[module],
                        prev_matched_line,
                        matched_line,
                        prev_matched_ea + 1,
                        symbols,
                    ):
                        macro_comments.setdefault(cmt_ea, set()).add(cmt)
                walk_stop_line = matched_line
                if matched_line in module_instruction_lines.get(module, set()):
                    previous_ea = module_line_rom_ea.get((module, matched_line))
                    if previous_ea is None:
                        module_line_rom_ea[(module, matched_line)] = rea
                    module_covered_lines.setdefault(module, set()).add(matched_line)
                    next_line = src_ops_lno[si + 1][0] if (si + 1) < len(src_ops_lno) else 0
                    if next_line != matched_line:
                        module_walked_lines.setdefault(module, set()).add(matched_line)
            if si < len(src_ops_lno):
                src_line = src_ops_lno[si][0]
                if 1 <= src_line <= len(module_lines_cache[module]):
                    raw_src_line = module_lines_cache[module][src_line - 1]
                    raw_code = ccm.strip_comment(raw_src_line)
                    src_lbl, _src_rest = ccm.split_optional_label(raw_code)
                    if src_lbl and LABEL_TOKEN_RE.match(src_lbl):
                        lmod = resolve_module_for_symbol(src_lbl, module, symbol_modules, globals_set)
                        add_row(
                            src_lbl,
                            lmod,
                            "code" if (symbol_key(src_lbl), lmod.upper()) in source_code_labels else "label",
                            rea,
                        )
                    cmt = source_comment_tag(raw_src_line, macros)
                    if cmt:
                        macro_comments.setdefault(rea, set()).add(cmt)
                prev_matched_line = matched_line
                prev_matched_ea = rea
            if si in dp0_op_indexes:
                dp0_marks.add(rea)
                dp0_marks.add(rea + 1)
            # Temporarily disabled: post-call DP0 injection for specific callees.
            # Keep only direct SETDP-derived DP marks for now.
            if sop.upper() == "LDI" and len(stoks) >= 2 and len(rtoks) >= 2 and literal_ref_idx < len(literal_refs):
                src_lit = stoks[1].strip().strip(",").upper()
                if src_lit in {"@VALUEI?", "@IT?"}:
                    curr_line = src_ops_lno[si][0] if si < len(src_ops_lno) else 0
                    while literal_ref_idx < len(literal_refs) and literal_refs[literal_ref_idx][0] < curr_line:
                        literal_ref_idx += 1
                    if literal_ref_idx < len(literal_refs):
                        ref_line, _macro, semantic_label = literal_refs[literal_ref_idx]
                        # Match literal macro refs by source line, not by exact expanded
                        # placeholder text (which varies between macro styles/listings).
                        if curr_line == ref_line:
                            literal_ref_idx += 1
                            lit_ea = parse_rom_operand_addr(rtoks[1])
                            if lit_ea is None:
                                lit_ea = lookup_address_map_symbol(rtoks[1], module, address_map)
                            if lit_ea is not None and semantic_label:
                                lit_ea = normalize_literal_cell_ea(lit_ea, word_rom)
                                target_ea = read_rom_word(lit_ea, word_rom)
                                smod = resolve_module_for_symbol(semantic_label, module, symbol_modules, globals_set)
                                add_row(f"{semantic_label}_ptr_{lit_ea:08X}", smod, "data", lit_ea)
                                if target_ea is not None:
                                    add_row(semantic_label, smod, "data", target_ea)
            # CALL target mapping by operand pairing:
            #   source: CALL MYFUNC
            #   rom:    call @1234h / call sub_xxx / call 1234h
            # Enqueue MYFUNC at mapped ROM address, independent of ROM symbol name.
            if CALL_RE.match(sop) and len(stoks) >= 2 and len(rtoks) >= 2:
                st = stoks[1].strip().strip(",")
                if LABEL_TOKEN_RE.match(st):
                    tmod = resolve_module_for_symbol(st, module, symbol_modules, globals_set)
                    tea = resolve_paired_operand_addr(
                        rtoks[1],
                        st,
                        tmod,
                        symbols,
                        address_map,
                        allow_source_set=False,
                    )
                    if tea is not None:
                        add_row(st, tmod, "code", tea)
                        tk = (st, tmod)
                        if has_decoded_rom_op(tea) and tk not in anchors:
                            na = Anchor(src_label=st, module=tmod, rom_label=st, addr=tea)
                            anchors[tk] = na
                            queue.append(na)
            # Branch label mapping by operand pairing:
            #   source: B/BU/BR/... TARGET
            #   rom:    b/bu/br/... @ADDR or ADDR
            # Branch targets are labels (not functions), so do not recurse.
            if BRANCH_RE.match(sop) and len(stoks) >= 2 and len(rtoks) >= 2:
                st = stoks[1].strip().strip(",")
                if LABEL_TOKEN_RE.match(st):
                    tmod = resolve_module_for_symbol(st, module, symbol_modules, globals_set)
                    tea = resolve_paired_operand_addr(
                        rtoks[1],
                        st,
                        tmod,
                        symbols,
                        address_map,
                        allow_source_set=False,
                    )
                    if tea is not None:
                        add_row(st, tmod, "label", tea)
            # Repeat-block target mapping by operand pairing:
            #   source: RPTB TARGET
            #   rom:    rptb TARGET_ADDR_OR_LABEL
            # Treat as intra-function label target (non-function).
            if RPT_RE.match(sop) and len(stoks) >= 2 and len(rtoks) >= 2:
                st = stoks[1].strip().strip(",")
                if LABEL_TOKEN_RE.match(st):
                    tmod = resolve_module_for_symbol(st, module, symbol_modules, globals_set)
                    tea = resolve_paired_operand_addr(
                        rtoks[1],
                        st,
                        tmod,
                        symbols,
                        address_map,
                        allow_source_set=False,
                    )
                    if tea is not None:
                        add_row(st, tmod, "label", tea)
            for idx, stok in enumerate(stoks[1:]):
                if "@" not in stok:
                    continue
                dmod = module
                s = stok.strip().strip(",")
                s = s.lstrip("@*#+-").strip("()")
                if not s or NUMERIC_TOKEN_RE.match(s) or not LABEL_TOKEN_RE.match(s):
                    continue
                up = s.upper()
                if REGISTER_RE.match(up):
                    continue
                ridx = idx + 1
                if ridx >= len(rtoks):
                    continue
                dea = resolve_paired_operand_addr(
                    rtoks[ridx],
                    s,
                    module,
                    symbols,
                    address_map,
                    allow_source_set=True,
                )
                if dea is None:
                    continue
                dmod = resolve_module_for_symbol(s, module, symbol_modules, globals_set)
                add_row(s, dmod, "data", dea)
            si += 1
            ri += 1

        if ri >= len(rom_ops) and si < len(src_ops):
            ex_line = ""
            if si < len(src_ops_lno):
                ex_line = str(src_ops_lno[si][0])
                walk_stop_line = src_ops_lno[si][0]
            exhausted_at = (rom_ops[-1][0] + 1) if rom_ops else a.addr
            unresolved.append((
                a.src_label,
                module,
                "module_walk_rom_exhausted",
                f"line={ex_line or si} matched={matched_n} src_ops={len(src_ops)} rom_ops={len(rom_ops)} rom_exhausted_at=0x{exhausted_at:08X}",
                ex_line,
            ))

        # Walk ranges are diagnostic only. Coverage is tracked per matched
        # instruction line above.
        if module in module_lines_cache and matched_n > 0:
            mlen = len(module_lines_cache[module])
            lo = max(1, min(start_line, mlen))
            hi = max(1, min(walk_stop_line, mlen))
            if hi < lo:
                hi = lo
            module_walk_ranges.setdefault(module, []).append((lo, hi, a.src_label))

    pending_word_blocks: List[Tuple[str, str, int, bool]] = []
    seen_word_blocks: Dict[Tuple[str, str], bool] = {}

    def enqueue_word_block(lbl: str, mod: str, ea: int, *, include_word_refs: bool = True) -> None:
        if is_auto_sub_name(lbl) or is_pseudo_local_label(lbl) or lbl.endswith("?"):
            return
        key = (symbol_key(lbl), mod.upper())
        prev = seen_word_blocks.get(key)
        if prev is not None and (prev or not include_word_refs):
            return
        found = source_label_lines.get(key)
        if found is None:
            return
        _exact_lbl, line = found
        if mod not in module_lines_cache:
            p = find_module_source_path(root, mod)
            if p is None:
                return
            module_lines_cache[mod] = p.read_text(errors="ignore").splitlines()
        if not source_label_starts_word_block(module_lines_cache[mod], line, symbols):
            return
        seen_word_blocks[key] = include_word_refs
        pending_word_blocks.append((lbl, mod, ea, include_word_refs))

    def seed_word_blocks_from_rows() -> None:
        for _key, row in list(rows.items()):
            lbl = row[0]
            ea = int(row[4], 16)
            mod = row[2]
            if not mod:
                mods = symbol_modules_lookup(symbol_modules, lbl)
                if len(mods) != 1:
                    continue
                mod = next(iter(mods))
            enqueue_word_block(lbl, mod, ea, include_word_refs=True)

    seed_word_blocks_from_rows()

    for module, lines in list(module_lines_cache.items()):
        mapped_lines = sorted(
            (line, ea)
            for (mod, line), ea in module_line_rom_ea.items()
            if mod == module
        )
        for idx in range(1, len(mapped_lines)):
            prev_line, prev_ea = mapped_lines[idx - 1]
            curr_line, _curr_ea = mapped_lines[idx]
            if curr_line <= prev_line + 1:
                continue
            for lbl, mod, ea, is_word_ref, has_inline_alias in _collect_text_gap_word_symbol_rows_ex(
                lines,
                prev_line,
                curr_line,
                prev_ea + 1,
                module,
                symbols,
                symbol_modules,
                globals_set,
                word_rom,
            ):
                key = (symbol_key(lbl), mod.upper())
                if is_word_ref:
                    target_ea = read_rom_word(normalize_word_rom_read_ea(ea, word_rom), word_rom)
                    if target_ea is None:
                        continue
                    if 0x00C00000 <= target_ea <= 0x00CFFFFF:
                        add_row(lbl, "", "data", target_ea)
                        if key in source_label_lines:
                            enqueue_word_block(lbl, mod, target_ea, include_word_refs=False)
                        continue
                    if not has_inline_alias:
                        continue
                    add_row(lbl, mod, "data", target_ea)
                    continue
                add_row(lbl, mod, "data", ea)
                if key in source_label_lines:
                    enqueue_word_block(lbl, mod, ea)

    while pending_word_blocks:
        lbl, mod, ea, include_word_refs = pending_word_blocks.pop()
        key = (symbol_key(lbl), mod.upper())
        found = source_label_lines.get(key)
        if found is None:
            continue
        _exact_lbl, line = found
        if mod not in module_lines_cache:
            p = find_module_source_path(root, mod)
            if p is None:
                continue
            module_lines_cache[mod] = p.read_text(errors="ignore").splitlines()
        for child_lbl, child_mod, child_ea, child_is_word_ref, child_has_inline_alias in _collect_word_block_symbol_rows_ex(
            module_lines_cache[mod],
            line,
            ea,
            mod,
            symbols,
            symbol_modules,
            globals_set,
            word_rom,
            include_word_refs=include_word_refs,
        ):
            if child_is_word_ref:
                target_ea = read_rom_word(normalize_word_rom_read_ea(child_ea, word_rom), word_rom)
                if target_ea is None:
                    continue
                if 0x00C00000 <= target_ea <= 0x00CFFFFF:
                    add_row(child_lbl, "", "data", target_ea, overwrite=True)
                    enqueue_word_block(child_lbl, child_mod, target_ea, include_word_refs=False)
                    continue
                if not child_has_inline_alias:
                    continue
                add_row(child_lbl, child_mod, "data", target_ea, overwrite=True)
                continue
            if should_preserve_existing_word_block_row(
                rows,
                source_label_lines,
                globals_set,
                symbols,
                lbl,
                child_lbl,
                child_mod,
                child_ea,
            ):
                continue
            add_row(child_lbl, child_mod, "data", child_ea, overwrite=True)
            enqueue_word_block(child_lbl, child_mod, child_ea, include_word_refs=include_word_refs)

    for lbl, ea in sorted(vunit_hardware_symbols.items(), key=lambda kv: (kv[1], kv[0].upper())):
        add_row(lbl, "", "data", ea)

    promoted_aliases = promote_same_address_module_aliases()

    map_before, map_inserted, map_updated = upsert_address_map(
        canonical_map,
        ((r[1], int(r[4], 16)) for r in rows.values()),
        prune_scoped_aliases=True,
    )

    with outp.open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow(["label", "label_norm", "module", "kind", "address", "status", "src_first5", "rom_first5"])
        for _k, r in sorted(rows.items(), key=lambda kv: (int(kv[1][4], 16), kv[1][2], kv[1][0])):
            w.writerow(r)

    seed_out = log_dir / "romlst_initial_anchors.tsv"
    with seed_out.open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow(["module", "label", "label_norm", "address", "map_name"])
        for mod, a in sorted(module_anchor_candidates.items(), key=lambda kv: (kv[0], kv[1].addr, kv[1].src_label.upper())):
            label_norm = a.src_label if (a.src_label.upper() in globals_set or not a.module) else f"{a.src_label}@{a.module}"
            w.writerow([mod, a.src_label, label_norm, f"0x{a.addr:08X}", a.rom_label])

    # Keep only where module walk stopped/desynced: one row per module.
    def unresolved_rom_address(reason: str, ctx: str) -> str:
        patterns = [
            r"rom_data_at=(0x[0-9A-Fa-f]+)",
            r"rom_exhausted_at=(0x[0-9A-Fa-f]+)",
            r"\bat=(0x[0-9A-Fa-f]+)",
            r"current=(0x[0-9A-Fa-f]+)",
        ]
        for pat in patterns:
            m = re.search(pat, ctx)
            if m:
                return m.group(1)
        return ""

    unresolved_unique: List[Tuple[str, str, str, str, str, str]] = []
    stop_reasons = {"module_walk_desync", "module_walk_rom_exhausted", "rom_data_in_code"}
    seen_stop_modules: Set[str] = set()
    for lbl, mod, reason, ctx, src_line in unresolved:
        if reason not in stop_reasons:
            continue
        if not mod:
            continue
        if mod in seen_stop_modules:
            continue
        seen_stop_modules.add(mod)
        unresolved_unique.append((lbl, mod, reason, ctx, src_line, unresolved_rom_address(reason, ctx)))

    with unp.open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow(["label", "module", "reason", "context", "source_line", "rom_address"])
        for r in sorted(unresolved_unique, key=lambda row: (row[1], row[0], row[4])):
            w.writerow(r)

    cov_out = outp.parent / (outp.stem.replace("_labels", "_module_coverage") + outp.suffix)
    cov_rows: List[Tuple[str, int, int, int, float, str, str, str]] = []
    all_modules = sorted({p.stem.upper() for p in ccm.iter_source_files(root, (".ASM",))})
    for mod in all_modules:
        if mod not in module_lines_cache:
            p = find_module_source_path(root, mod)
            if p is None:
                continue
            module_lines_cache[mod] = p.read_text(errors="ignore").splitlines()
        if mod not in module_instruction_lines:
            module_instruction_lines[mod] = collect_instruction_line_numbers(module_lines_cache[mod], symbols, macros)
        instruction_lines = module_instruction_lines.get(mod, set())
        total = len(instruction_lines)
        covered_n = len(module_covered_lines.get(mod, set()) & instruction_lines)
        pct = (covered_n * 100.0 / total) if total else 0.0
        uncovered = instruction_lines - (module_covered_lines.get(mod, set()) & instruction_lines)
        uncovered_n = len(uncovered)
        uncovered_ranges = line_set_to_ranges(uncovered)
        ranges = module_walk_ranges.get(mod, [])
        ranges_s = ", ".join(f"{lo}-{hi}:{seed}" for lo, hi, seed in ranges)
        walk_complete = "yes" if uncovered_n == 0 else "no"
        cov_rows.append((mod, total, covered_n, uncovered_n, pct, uncovered_ranges, ranges_s, walk_complete))

    # Highest coverage first, then module name for stable ordering.
    cov_rows.sort(key=lambda r: (-r[4], r[0]))

    with cov_out.open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow([
            "module",
            "total_instruction_lines",
            "covered_instruction_lines",
            "uncovered_instruction_lines",
            "coverage_pct",
            "walk_complete",
            "uncovered_ranges",
            "walk_ranges",
        ])
        for mod, total, covered_n, uncovered_n, pct, uncovered_ranges, ranges_s, walk_complete in cov_rows:
            w.writerow([mod, total, covered_n, uncovered_n, f"{pct:.2f}", walk_complete, uncovered_ranges, ranges_s])

    # Always emit DP anchors beside labels output.
    dpout = outp.with_name(outp.stem.replace("_labels", "_dp0") + outp.suffix)
    with dpout.open("w", newline="") as f:
        f.write("address\n")
        for ea in sorted(dp0_marks):
            f.write(f"0x{ea:08X}\n")

    cmout = outp.with_name(outp.stem.replace("_labels", "_macro_comments") + outp.suffix)
    with cmout.open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow(["address", "comment"])
        for ea in sorted(macro_comments.keys()):
            names = ", ".join(sorted(macro_comments[ea]))
            w.writerow([f"0x{ea:08X}", names])

    code_n = sum(1 for r in rows.values() if r[3] == "code")
    data_n = sum(1 for r in rows.values() if r[3] == "data")
    print(f"wrote {outp}")
    print(f"wrote {unp}")
    print(f"wrote {cov_out}")
    print(f"wrote {dpout}")
    print(f"wrote {cmout}")
    print(f"rows={len(rows)} code={code_n} data={data_n} anchors={len(anchors)} unresolved={len(unresolved_unique)}")
    print(f"promoted_aliases={promoted_aliases}")
    if ida_map.exists():
        print(f"imported_ida_map={ida_map} existing={ida_map_before} inserted={ida_map_inserted} updated={ida_map_updated}")
    print(f"address_map={canonical_map} existing={map_before} inserted={map_inserted} updated={map_updated}")

    # Automatically generate standalone IDAPython apply script.
    labels_emitted, dp0_emitted, comments_emitted = build_ida_import_script.build_import_script(
        outp,
        py_out,
        dp0_tsv=dpout,
        comments_tsv=cmout,
    )
    dp_py_out = py_out.with_name("ida_dp0_import.py")
    dp_only_marks = build_ida_dp0_script.build_dp0_script(dpout, dp_py_out)
    print(f"wrote {py_out}")
    print(f"labels={labels_emitted} dp0_marks={dp0_emitted} comments={comments_emitted}")
    print(f"wrote {dp_py_out}")
    print(f"dp0_only_marks={dp_only_marks}")

    # Append high-level run metrics for regression tracking.
    run_log.parent.mkdir(parents=True, exist_ok=True)
    run_header = [
        "timestamp_utc",
        "rom_path",
        "labels_tsv",
        "apply_py",
        "rows_total",
        "rows_code",
        "rows_data",
        "anchors",
        "unresolved",
        "dp0_marks",
        "labels_emitted",
        "promoted_aliases",
        "address_map_existing",
        "address_map_inserted",
        "address_map_updated",
        "ida_map",
        "ida_map_existing",
        "ida_map_inserted",
        "ida_map_updated",
    ]
    if run_log.exists() and run_log.stat().st_size:
        old_rows = list(csv.reader(run_log.open(newline=""), delimiter="\t"))
        if old_rows and old_rows[0] != run_header:
            old_width = len(old_rows[0])
            new_width = len(run_header)
            with run_log.open("w", newline="") as f:
                w = csv.writer(f, delimiter="\t")
                w.writerow(run_header)
                for row in old_rows[1:]:
                    if len(row) < new_width:
                        row = row + [""] * (new_width - len(row))
                    w.writerow(row[:new_width])
    write_header = not run_log.exists() or run_log.stat().st_size == 0
    ts = datetime.datetime.now(datetime.timezone.utc).isoformat()
    with run_log.open("a", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        if write_header:
            w.writerow(run_header)
        w.writerow([
            ts,
            str(rom),
            str(outp),
            str(py_out),
            len(rows),
            code_n,
            data_n,
            len(anchors),
            len(unresolved_unique),
            len(dp0_marks),
            len(rows),  # one label row per emitted symbol candidate in TSV
            promoted_aliases,
            map_before,
            map_inserted,
            map_updated,
            str(ida_map) if ida_map.exists() else "",
            ida_map_before,
            ida_map_inserted,
            ida_map_updated,
        ])
    print(f"appended {run_log}")


if __name__ == "__main__":
    main()

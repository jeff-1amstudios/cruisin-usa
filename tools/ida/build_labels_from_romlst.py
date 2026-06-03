#!/usr/bin/env python3
from __future__ import annotations

import bisect
import csv
import datetime
import functools
import pathlib
import re
import subprocess
import sys
from dataclasses import dataclass
from typing import Dict, Iterable, List, Optional, Set, Tuple

import sys
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
import cint_call_match as ccm  # type: ignore

ROM_LINE_RE = re.compile(r"^CODE:([0-9A-Fa-f]{8})\s+(.*)$")
LABEL_TOKEN_RE = re.compile(r"^[A-Za-z_.$?@][A-Za-z0-9_.$?@]*$")
NUMERIC_TOKEN_RE = re.compile(r"^(?:-?\d+|0x[0-9A-Fa-f]+|[0-9A-Fa-f]+h)$")
CALL_RE = re.compile(r"^(CALL(?:[A-Z]+)?)$", re.IGNORECASE)
# Branch-like control-flow mnemonics for label binding.
# Includes unconditional/direct forms and common conditional Bxx forms.
BRANCH_RE = re.compile(
    r"^(?:B|BU|BR|BD|BUD|BRD|DB|DBU|DBR|DBD|"
    r"BEQ|BNE|BGT|BGE|BLT|BLE|BHI|BHS|BLO|BLS|BNV|BNN|BNZ|BZ|BC|BNC)$",
    re.IGNORECASE,
)
RPT_RE = re.compile(r"^(RPTB|RPTS)$", re.IGNORECASE)
ADDRESS_MAP_RE = re.compile(r"^\s*[0-9A-Fa-f]{4}:([0-9A-Fa-f]{8})\s+(\S+)\s*$")
SET_RE = re.compile(r"^\s*([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s+\.set\s+(.+)$", re.IGNORECASE)
GLOBL_RE = re.compile(r"^\.globl\s+(.+)$", re.IGNORECASE)
WORD_VALUE_RE = re.compile(r"\.word\s+([^\s;]+)", re.IGNORECASE)
PTR_SUFFIX_RE = re.compile(r"_ptr_[0-9A-Fa-f]{8}$")
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
DEFAULT_WORD_ROM = pathlib.Path(__file__).resolve().parents[2] / "roms" / "crusnusa45_maindata_interleaved_bswap32.bin"


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
    for p in sorted(root.glob("*.ASM")) + sorted(root.glob("*.EQU")):
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
    for p in sorted(root.glob("*.ASM")):
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
    vunit = root / "VUNIT.EQU"
    if not vunit.exists():
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


def find_function_start_in_module(root: pathlib.Path, module: str, label: str) -> Optional[int]:
    p = root / f"{module}.ASM"
    if not p.exists():
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
    p = root / f"{module}.ASM"
    return ccm.extract_source_function_lines(p, label, start)


def extract_module_tail_lines(root: pathlib.Path, module: str, label: str) -> Optional[List[str]]:
    start = find_function_start_in_module(root, module, label)
    if start is None:
        return None
    p = root / f"{module}.ASM"
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


def iter_active_raw_with_lineno(lines: List[str], base_lineno: int, symbols: Dict[str, int]) -> List[Tuple[int, str]]:
    out: List[Tuple[int, str]] = []
    active_stack: List[bool] = [True]
    for i, ln in enumerate(lines):
        code = ccm.strip_comment(ln).strip()
        if not code:
            continue
        if code.startswith("*"):
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
            out.append((base_lineno + i, ln))
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

    for p in sorted(root.glob("*.ASM")):
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

    for p in sorted(root.glob("*.ASM")):
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
    for p in sorted(root.glob("*.ASM")):
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
    def _string_words(tok: str) -> int:
        s = tok.strip()
        if len(s) >= 2 and s[0] == "'" and s[-1] == "'":
            return 1
        if len(s) >= 2 and s[0] == '"' and s[-1] == '"':
            n = len(s[1:-1])
            return max(1, (n + 3) // 4)
        return 1

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
            args = toks[op_idx + 1 :]
            if not args:
                total += 1
            else:
                total += sum(_string_words(a) for a in args)
    return total


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
    for p in sorted(root.glob("*.ASM")):
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
    root = pathlib.Path(__file__).resolve().parents[2]
    rom = pathlib.Path("/Users/j.harris/Downloads/carma/cruisin/crusnusa45_maindata_interleaved_bswap32.bin.lst")
    word_rom = root / "roms" / "crusnusa45_maindata_interleaved_bswap32.bin"
    outp = here / "romlst_labels.tsv"
    unp = here / "romlst_unresolved.tsv"
    py_out = here / "apply_romlst_labels.py"
    run_log = here / "romlst_run_log.tsv"
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
    rom_word_values = parse_rom_word_values(rom)
    rom_ops_all = parse_all_rom_ops(rom)
    rom_ops_addrs = [ea for ea, _op, _toks in rom_ops_all]
    source_code_labels = source_code_label_index(root, symbols, macros)
    source_labels_ordered = source_code_label_order(root, symbols, macros)
    source_call_target_keys = collect_source_call_target_keys(root, symbols)

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
        module_anchor_candidates[mod] = Anchor(src_label=src_fn, module=mod, rom_label=map_name, addr=ea)

    anchors: Dict[Tuple[str, str], Anchor] = {
        (a.src_label, a.module): a
        for a in sorted(module_anchor_candidates.values(), key=lambda a: (a.module, a.addr, a.src_label.upper()))
    }

    rows: Dict[Tuple[str, str], Tuple[str, str, str, str, str, str, str, str]] = {}
    unresolved: List[Tuple[str, str, str, str, str]] = []
    dp0_marks: Set[int] = set()
    macro_comments: Dict[int, Set[str]] = {}

    def add_row(lbl: str, mod: str, kind: str, ea: int) -> None:
        if is_auto_sub_name(lbl):
            return
        if is_pseudo_local_label(lbl):
            return
        # Macro-local/temp labels (e.g. VALUEI?) are noisy and not useful in IDA output.
        if lbl.endswith("?"):
            return
        mod = canonical_module_for_label(lbl, mod, globals_set, symbols)
        name = lbl if (lbl.upper() in globals_set or not mod) else f"{lbl}@{mod}"
        rows.setdefault((lbl, mod), (lbl, name, mod, kind, f"0x{ea:08X}", "OK", "", ""))

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
            p = root / f"{module}.ASM"
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
                    unresolved.append((a.src_label, module, "module_walk_rom_exhausted", f"after .align near line={curr_src_line}", str(curr_src_line)))
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
                if rea > prev_rea + 1 and (prev_rea + 1) in rom_word_values:
                    gap_words = rea - prev_rea - 1
                    prev_line = src_ops_lno[si - 1][0] if si > 0 and si - 1 < len(src_ops_lno) else 0
                    curr_line = src_ops_lno[si][0] if si < len(src_ops_lno) else prev_line
                    inline_words = source_rom_data_words_between(module_lines_cache[module], prev_line, curr_line, macros, symbols)
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
                    cmt = source_comment_tag(module_lines_cache[module][src_line - 1], macros)
                    if cmt:
                        macro_comments.setdefault(rea, set()).add(cmt)
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
                        if tk not in anchors:
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
            unresolved.append((
                a.src_label,
                module,
                "module_walk_rom_exhausted",
                f"line={ex_line or si} matched={matched_n} src_ops={len(src_ops)} rom_ops={len(rom_ops)}",
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

    seed_out = here / "romlst_initial_anchors.tsv"
    with seed_out.open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow(["module", "label", "label_norm", "address", "map_name"])
        for mod, a in sorted(module_anchor_candidates.items(), key=lambda kv: (kv[0], kv[1].addr, kv[1].src_label.upper())):
            label_norm = a.src_label if (a.src_label.upper() in globals_set or not a.module) else f"{a.src_label}@{a.module}"
            w.writerow([mod, a.src_label, label_norm, f"0x{a.addr:08X}", a.rom_label])

    # Keep only where module walk stopped/desynced: one row per module.
    unresolved_unique: List[Tuple[str, str, str, str, str]] = []
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
        unresolved_unique.append((lbl, mod, reason, ctx, src_line))

    with unp.open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t")
        w.writerow(["label", "module", "reason", "context", "source_line"])
        for r in sorted(unresolved_unique, key=lambda row: (row[1], row[0], row[4])):
            w.writerow(r)

    cov_out = outp.parent / (outp.stem.replace("_labels", "_module_coverage") + outp.suffix)
    cov_rows: List[Tuple[str, int, int, int, float, str, str, str]] = []
    all_modules = sorted({p.stem.upper() for p in root.glob("*.ASM")})
    for mod in all_modules:
        if mod not in module_lines_cache:
            p = root / f"{mod}.ASM"
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
    gen = pathlib.Path(__file__).resolve().parent / "generate_spider_labels_py.py"
    cmd = [
        sys.executable,
        str(gen),
        "--labels-tsv",
        str(outp),
        "--out",
        str(py_out),
        "--dp0-tsv",
        str(dpout),
        "--comments-tsv",
        str(cmout),
    ]
    subprocess.run(cmd, check=True)

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

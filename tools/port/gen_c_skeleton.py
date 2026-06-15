#!/usr/bin/env python3
"""Generate first-pass C scaffolds from asm modules."""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Iterable


INCLUDE_RE = re.compile(r"^\s*\.include\s+([A-Za-z0-9_.]+)\s*$", re.IGNORECASE)
SET_RE = re.compile(r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s*\.set\s*(.*?)\s*$", re.IGNORECASE)
IF_RE = re.compile(r"^\s*\.if\s+(.*?)\s*$", re.IGNORECASE)
ELSE_RE = re.compile(r"^\s*\.else\s*$", re.IGNORECASE)
ENDIF_RE = re.compile(r"^\s*\.endif\s*$", re.IGNORECASE)
MACRO_START_RE = re.compile(
    r"^\s*(?:[_A-Za-z.$?@][_A-Za-z0-9.$?@]*\s+)?\.macro\b",
    re.IGNORECASE,
)
MACRO_END_RE = re.compile(r"^\s*\.endm\b", re.IGNORECASE)
STRUCT_START_RE = re.compile(r"^\s*\*+\s*STRUCT\b", re.IGNORECASE)
STRUCT_END_RE = re.compile(r"^\s*\*+\s*ENDSTRUCT\b", re.IGNORECASE)
LABEL_RE = re.compile(r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s*:(.*)$")
DATA_LABEL_RE = re.compile(r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s+((?:\.\w+)|EQU|equ|fbss|pbss|hibss)\b(.*)$")
INLINE_LABEL_RE = re.compile(r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s+([A-Za-z.][A-Za-z0-9.]*)\b(.*)$")
BARE_LABEL_RE = re.compile(r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s*$")
BRANCH_TARGET_RE = re.compile(r"\b([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\b")
SEPARATOR_RE = re.compile(r"^\*[-]{8,}\s*$")
HEX_SUFFIX_RE = re.compile(r"\b([0-9A-Fa-f]+)[Hh]\b")
SIMPLE_STRING_RE = re.compile(r'^\s*("([^"\\]|\\.)*")\s*(?:,\s*0+\s*)?$')
SPTR_LABEL_RE = re.compile(r'^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s+SPTR\b(.*)$', re.IGNORECASE)
STANDALONE_STORAGE_RE = re.compile(r"^\s*(?:\.bss|\.usect|fbss|pbss|hibss|lobss|phibss)\b", re.IGNORECASE)
STANDALONE_STORAGE_FULL_RE = re.compile(
    r"^\s*((?:\.bss|\.usect|fbss|pbss|hibss|lobss|phibss))\s+([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\b(.*)$",
    re.IGNORECASE,
)
STANDALONE_STORAGE_LABEL_RE = re.compile(
    r"^\s*(?:\.bss|\.usect|fbss|pbss|hibss|lobss|phibss)\s+([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\b",
    re.IGNORECASE,
)
NUMERIC_WORD_RE = re.compile(r"^(?:0x[0-9A-Fa-f]+|[0-9]+|[0-9A-Fa-f]+[Hh])$")
STANDALONE_DATA_DIRECTIVE_RE = re.compile(
    r"^\s*(\.word|\.float|\.string|\.bss|\.usect|fbss|pbss|hibss|lobss|phibss)\b(.*)$",
    re.IGNORECASE,
)
CPP_DEFINE_RE = re.compile(r"^\s*#\s*define\s+([_A-Za-z][_A-Za-z0-9]*)\b")

CONTROL_KEYWORDS = {
    ".if",
    ".else",
    ".endif",
    ".sect",
    ".text",
    ".data",
    ".bss",
    ".globl",
    ".file",
    ".version",
    ".sym",
    ".func",
    ".endfunc",
    ".word",
    ".string",
    ".float",
    ".set",
    "fbss",
    "pbss",
    "hibss",
    "romdata",
    "equ",
}
CONTROL_KEYWORDS_LOWER = {kw.lower() for kw in CONTROL_KEYWORDS}

FUNCTION_END_MNEMONICS = {
    "RETS",
    "RETI",
    "RETSC",
    "RETSCNZ",
    "RETSNZ",
    "RETSZ",
    "RETSNC",
    "RETSN",
    "RETSP",
    "RETSD",
    "RET",
    "B",
    "BU",
    "BR",
    "DB",
}

INSTRUCTION_PREFIXES = (
    ".",
    "LD",
    "ST",
    "PUSH",
    "POP",
    "CALL",
    "RET",
    "B",
    "ADD",
    "SUB",
    "MPY",
    "CMP",
    "TST",
    "AND",
    "OR",
    "XOR",
    "LS",
    "RS",
    "RPT",
    "SET",
    "CLR",
    "NOP",
    "FIX",
    "FLOAT",
    "LDF",
    "STF",
    "DINT",
    "EINT",
    "SOFT",
    "ERR",
)


@dataclass
class FunctionBlock:
    name: str
    aliases: list[str] = field(default_factory=list)
    lines: list[str] = field(default_factory=list)
    raw_lines: list[str] = field(default_factory=list)
    line_numbers: list[int] = field(default_factory=list)
    labels: set[str] = field(default_factory=set)
    start_index: int = -1
    end_index: int = -1


@dataclass
class WordVariable:
    name: str
    values: list[str]
    asm_lines: list[str]
    resolved_values: dict[str, int] = field(default_factory=dict)


@dataclass
class SetDefine:
    name: str
    expr: str
    origin: str = "set"
    comment: str = ""


@dataclass
class DefineEntry:
    name: str
    expr: str
    module: str


@dataclass
class LabelEntry:
    name: str
    addr: int


@dataclass
class StringVariable:
    name: str
    c_string: str


@dataclass
class NumericVariable:
    name: str
    directive: str
    values: list[str]
    asm_lines: list[str]
    string_literal_values: list[str] | None = None


@dataclass
class StorageVariable:
    name: str
    addr: int
    module: str
    asm_line: str
    size_expr: str


@dataclass
class StandaloneLabeledData:
    name: str
    directive: str
    rest: str
    asm_lines: list[str]


@dataclass
class SymbolInfo:
    name: str
    kind: str
    module: str
    c_type: str = ""
    array_expr: str | None = None
    expr: str | None = None


@dataclass
class TypeOverride:
    name: str
    c_type: str
    array_expr: str | None = None
    omit: bool = False
    force_function: bool = False


def sanitize_identifier(name: str) -> str:
    out = []
    for ch in name:
        if ch.isalnum() or ch == "_":
            out.append(ch)
        elif ch in ".?$@":
            out.append("_")
        else:
            out.append("_")
    ident = "".join(out)
    if not ident:
        ident = "label"
    if ident[0].isdigit():
        ident = "_" + ident
    return ident


def split_comment(line: str) -> tuple[str, str]:
    in_quote = False
    for i, ch in enumerate(line):
        if ch == '"':
            in_quote = not in_quote
        elif ch == ";" and not in_quote:
            return line[:i].rstrip(), line[i:]
    return line.rstrip(), ""


def parse_address_map(map_path: Path) -> dict[str, int]:
    out: dict[str, int] = {}
    if not map_path.exists():
        return out
    for raw in map_path.read_text(errors="ignore").splitlines():
        line = raw.strip()
        if not line or line.startswith(";"):
            continue
        parts = line.split()
        if len(parts) < 2 or ":" not in parts[0]:
            continue
        _seg, off = parts[0].split(":", 1)
        try:
            out[parts[1]] = int(off, 16)
        except ValueError:
            continue
    return out


def parse_int_token(tok: str) -> int | None:
    text = tok.strip()
    neg = text.startswith("-")
    if neg:
        text = text[1:].strip()
    if text.lower().startswith("0x"):
        try:
            value = int(text, 16)
        except ValueError:
            return None
        return -value if neg else value
    if text.lower().endswith("h"):
        try:
            value = int(text[:-1], 16)
        except ValueError:
            return None
        return -value if neg else value
    if re.fullmatch(r"[0-9]+", text):
        value = int(text, 10)
        return -value if neg else value
    return None


def parse_discovered_defines_file(defines_path: Path) -> dict[str, DefineEntry]:
    out: dict[str, DefineEntry] = {}
    if not defines_path.exists():
        return out
    for raw in defines_path.read_text(errors="ignore").splitlines():
        if not raw.strip():
            continue
        parts = raw.split("\t")
        if len(parts) < 2:
            continue
        name = parts[0]
        expr = parts[1].strip()
        if not expr:
            continue
        module = parts[2].strip().upper() if len(parts) >= 3 else ""
        out.setdefault(
            name,
            DefineEntry(
                name=name,
                expr=expr,
                module=module,
            ),
        )
    return out


def parse_instruction_addresses_file(addresses_path: Path) -> dict[str, dict[int, int]]:
    out: dict[str, dict[int, int]] = {}
    if not addresses_path.exists():
        return out
    for raw in addresses_path.read_text(errors="ignore").splitlines():
        if not raw.strip():
            continue
        parts = raw.split("\t")
        if len(parts) < 3 or parts[0].strip().lower() == "module":
            continue
        module = parts[0].strip().upper()
        try:
            line_no = int(parts[1].strip(), 10)
            address = int(parts[2].strip(), 16)
        except ValueError:
            continue
        out.setdefault(module, {})[line_no] = address
    return out


def parse_type_overrides_file(overrides_path: Path) -> dict[str, TypeOverride]:
    out: dict[str, TypeOverride] = {}
    if not overrides_path.exists():
        return out
    for raw in overrides_path.read_text(errors="ignore").splitlines():
        line = raw.strip()
        if not line or line.startswith("#") or line.startswith("//"):
            continue
        if line.startswith("-"):
            name = line[1:].strip()
            if re.fullmatch(r"[_A-Za-z.$?@][_A-Za-z0-9.$?@]*", name):
                out[name] = TypeOverride(name=name, c_type="", omit=True)
            continue
        function_match = re.fullmatch(r"(?P<name>[_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\(\)", line)
        if function_match:
            name = function_match.group("name")
            out[name] = TypeOverride(name=name, c_type="", force_function=True)
            continue
        if line.endswith(";"):
            line = line[:-1].rstrip()
        match = re.match(
            r"^(?P<prefix>.*?)(?P<name>[_A-Za-z.$?@][_A-Za-z0-9.$?@]*)(?:\[(?P<array>[^\]]*)\])?$",
            line,
        )
        if not match:
            continue
        c_type = match.group("prefix").strip()
        name = match.group("name")
        if not c_type:
            continue
        if not re.search(r"[A-Za-z_]", c_type):
            continue
        if re.search(r"[^A-Za-z0-9_\s*]", c_type):
            continue
        array_expr = match.group("array")
        out[name] = TypeOverride(name=name, c_type=c_type, array_expr=array_expr.strip() if array_expr is not None else None)
    return out


def is_omitted_symbol(name: str, type_overrides: dict[str, TypeOverride] | None) -> bool:
    if type_overrides is None:
        return False
    override = type_overrides.get(name)
    return bool(override is not None and override.omit)


def apply_type_override(symbol: SymbolInfo, type_overrides: dict[str, TypeOverride] | None) -> SymbolInfo:
    if type_overrides is None:
        return symbol
    override = type_overrides.get(symbol.name)
    if override is None or override.omit:
        return symbol
    if override.force_function:
        return SymbolInfo(name=symbol.name, kind="function", module=symbol.module)
    if symbol.kind != "variable":
        return symbol
    return SymbolInfo(
        name=symbol.name,
        kind=symbol.kind,
        module=symbol.module,
        c_type=override.c_type,
        array_expr=symbol.array_expr if override.array_expr is None else override.array_expr,
        expr=symbol.expr,
    )


def lookup_address_map_symbol(address_map: dict[str, int], name: str, module: str) -> int | None:
    if name in address_map:
        return address_map[name]
    scoped = f"{name}@{module.upper()}"
    if scoped in address_map:
        return address_map[scoped]
    scoped = f"{name}@{module}"
    return address_map.get(scoped)


def parse_storage_size(rest: str) -> str:
    operands, _comment = split_comment(rest.strip())
    if not operands:
        return "1"
    parts = [part.strip() for part in operands.split(",") if part.strip()]
    return parts[0] if parts else "1"


def variable_declaration(name: str, c_type: str, array_expr: str | None = None, is_extern: bool = False) -> str:
    ident = sanitize_identifier(name)
    prefix = "extern " if is_extern else ""
    if "(*)" in c_type:
        if array_expr is None or array_expr.strip() in {"", "0", "1"}:
            return f"{prefix}{c_type.replace('(*)', f'(*{ident})')};"
        return f"{prefix}{c_type.replace('(*)', f'(*{ident}[{array_expr.strip()}])')};"
    sep = "" if c_type.endswith("*") else " "
    if array_expr is None or array_expr.strip() in {"", "0", "1"}:
        return f"{prefix}{c_type}{sep}{ident};"
    return f"{prefix}{c_type}{sep}{ident}[{array_expr.strip()}];"


def variable_definition_prefix(
    name: str,
    c_type: str,
    array_expr: str | None = None,
    omit_array_size: bool = False,
) -> str:
    ident = sanitize_identifier(name)
    if "(*)" in c_type:
        if array_expr is None or array_expr.strip() in {"", "0", "1"}:
            return c_type.replace("(*)", f"(*{ident})")
        if omit_array_size:
            return c_type.replace("(*)", f"(*{ident}[])")
        return c_type.replace("(*)", f"(*{ident}[{array_expr.strip()}])")
    sep = "" if c_type.endswith("*") else " "
    if array_expr is None or array_expr.strip() in {"", "0", "1"}:
        return f"{c_type}{sep}{ident}"
    if omit_array_size:
        return f"{c_type}{sep}{ident}[]"
    return f"{c_type}{sep}{ident}[{array_expr.strip()}]"


def storage_declaration(name: str, size_expr: str, is_extern: bool = False) -> str:
    expr = convert_expr(size_expr).strip()
    if expr in {"", "0", "1"}:
        return variable_declaration(name, "int", None, is_extern=is_extern)
    return variable_declaration(name, "int", expr, is_extern=is_extern)


def strip_macro_definition_blocks(lines: list[str]) -> list[str]:
    out: list[str] = []
    macro_depth = 0
    for raw in lines:
        if MACRO_START_RE.match(raw):
            macro_depth += 1
            continue
        if MACRO_END_RE.match(raw):
            if macro_depth > 0:
                macro_depth -= 1
            continue
        if macro_depth > 0:
            continue
        out.append(raw)
    return out


def strip_struct_definition_blocks(lines: list[str]) -> list[str]:
    out: list[str] = []
    struct_depth = 0
    for raw in lines:
        if STRUCT_START_RE.match(raw):
            struct_depth += 1
            continue
        if STRUCT_END_RE.match(raw):
            if struct_depth > 0:
                struct_depth -= 1
            continue
        if struct_depth > 0:
            continue
        out.append(raw)
    return out


def strip_noncode_definition_blocks(lines: list[str]) -> list[str]:
    return strip_struct_definition_blocks(strip_macro_definition_blocks(lines))


def collect_module_storage_defines(
    src_path: Path,
    address_map: dict[str, int],
    type_overrides: dict[str, TypeOverride] | None = None,
) -> list[StorageVariable]:
    lines = strip_noncode_definition_blocks(src_path.read_text(errors="ignore").splitlines())
    module = src_path.stem.upper()
    out: list[StorageVariable] = []
    seen: set[str] = set()
    for raw in lines:
        code, _comment = split_comment(raw)
        storage_match = STANDALONE_STORAGE_FULL_RE.match(code)
        if not storage_match:
            continue
        _directive, storage_name, rest = storage_match.groups()
        if is_omitted_symbol(storage_name, type_overrides):
            continue
        if storage_name in seen:
            continue
        storage_addr = lookup_address_map_symbol(address_map, storage_name, module)
        if storage_addr is None:
            continue
        out.append(
            StorageVariable(
                storage_name,
                storage_addr,
                module,
                raw.rstrip(),
                parse_storage_size(rest),
            )
        )
        seen.add(storage_name)
    return out


def render_storage_header(src_path: Path, defines: list[StorageVariable]) -> str:
    guard = sanitize_identifier(src_path.stem).upper() + "_H"
    out: list[str] = []
    out.append(f"#ifndef {guard}")
    out.append(f"#define {guard}")
    out.append("")
    out.append(f"/* Generated from asm/{src_path.name}. */")
    out.append("")
    for entry in defines:
        out.append(f"// asm: {entry.asm_line}")
        out.append(storage_declaration(entry.name, entry.size_expr, is_extern=True))
    out.append("")
    out.append(f"#endif /* {guard} */")
    out.append("")
    return "\n".join(out)


def storage_header_name(module: str, include_dir: Path) -> str:
    base = module.lower() + ".h"
    base_path = include_dir / base
    if not base_path.exists():
        return base
    existing = base_path.read_text(errors="ignore")
    if f"Generated from asm/{module.upper()}.ASM." in existing:
        return base
    return module.lower() + "_defs.h"


def collect_word_symbol_dependencies(lines: list[str], owner_headers: dict[str, str], current_module: str) -> set[str]:
    deps: set[str] = set()
    for raw in lines:
        if not is_flush_left(raw):
            continue
        data_match = DATA_LABEL_RE.match(raw)
        if not data_match:
            continue
        _label, directive, rest = data_match.groups()
        if directive.lower() != ".word":
            continue
        operands, _comment = split_comment(rest.strip())
        values = [part.strip() for part in operands.split(",") if part.strip()] if operands else []
        if len(values) != 1 or NUMERIC_WORD_RE.match(values[0]):
            continue
        owner = owner_headers.get(values[0])
        if owner:
            deps.add(owner)
    return deps


def is_flush_left(raw: str) -> bool:
    return bool(raw) and not raw[:1].isspace()


def leading_label_name(line: str) -> str | None:
    m = LABEL_RE.match(line)
    if not m:
        return None
    return m.group(1)


def parse_include_headers(lines: list[str]) -> list[str]:
    headers: list[str] = []
    for raw in lines:
        m = INCLUDE_RE.match(raw)
        if not m:
            continue
        name = m.group(1)
        if name.upper().endswith(".EQU"):
            headers.append(name[:-4].lower() + ".h")
    return headers


def convert_expr(expr: str) -> str:
    expr = expr.strip()
    expr = HEX_SUFFIX_RE.sub(lambda m: f"0x{m.group(1)}", expr)
    expr = re.sub(r"(?<![<>!=])=(?!=)", "==", expr)
    expr = re.sub(r"\s+", " ", expr)
    return expr


def format_float_expr(expr: str) -> str:
    rendered = convert_expr(expr)
    parsed = parse_int_token(rendered)
    if parsed is not None:
        return f"{parsed}.0f"
    if rendered.lower().endswith("f"):
        return rendered
    if re.search(r"[.eE]", rendered):
        return rendered if rendered.lower().endswith("f") else f"{rendered}f"
    return rendered


def render_conditional_line(raw: str) -> str | None:
    match = IF_RE.match(raw)
    if match:
        return f"#if {convert_expr(match.group(1))}"
    if ELSE_RE.match(raw):
        return "#else"
    if ENDIF_RE.match(raw):
        return "#endif"
    return None


def render_asm_comment(raw: str, strip_asm_comment_markers: bool = False) -> str | None:
    stripped = raw.rstrip()
    if not stripped:
        return None
    lstripped = stripped.lstrip()
    if lstripped.startswith("*") or lstripped.startswith(";"):
        if strip_asm_comment_markers:
            comment_body = lstripped[1:]
            if comment_body.startswith(" "):
                comment_body = comment_body[1:]
            return f"// {comment_body}"
        return f"// {stripped}"
    return None


def is_context_line(raw: str) -> bool:
    if not raw.strip():
        return True
    if render_asm_comment(raw) is not None:
        return True
    return False


def is_comment_line(raw: str) -> bool:
    return render_asm_comment(raw) is not None


def needs_parens(expr: str) -> bool:
    if " " in expr:
        return True
    return any(op in expr for op in ("+", "-", "*", "/", "|", "&", "^", "<<", ">>"))


def is_comment_or_blank(line: str) -> bool:
    stripped = line.strip()
    return not stripped or stripped.startswith("*") or stripped.startswith(";")


def parse_simple_string_operand(operand_text: str) -> str | None:
    operands, _comment = split_comment(operand_text.strip())
    string_match = SIMPLE_STRING_RE.match(operands)
    if not string_match:
        return None
    return string_match.group(1)


def parse_sptr_label(raw: str) -> tuple[str, str] | None:
    if not is_flush_left(raw):
        return None
    match = SPTR_LABEL_RE.match(raw)
    if not match:
        return None
    label, rest = match.groups()
    c_string = parse_simple_string_operand(rest)
    if c_string is None:
        return None
    return label, c_string


def parse_numeric_directive(stripped: str) -> tuple[str, list[str]] | None:
    lower = stripped.lower()
    if lower.startswith(".word"):
        operands = stripped[5:].strip()
        return ".word", [part.strip() for part in operands.split(",") if part.strip()] if operands else []
    if lower.startswith(".float"):
        operands = stripped[6:].strip()
        return ".float", [part.strip() for part in operands.split(",") if part.strip()] if operands else []
    return None


def parse_numeric_data_line(raw: str, label_name: str | None = None) -> tuple[list[str], str] | None:
    code, comment = split_comment(raw)
    stripped = code.strip()
    if not stripped:
        return None

    label_match = LABEL_RE.match(stripped)
    if label_match is not None:
        label, rest = label_match.groups()
        if label_name is not None and label != label_name:
            return None
        numeric_directive = parse_numeric_directive(rest.strip())
        if numeric_directive is not None:
            _directive, values = numeric_directive
            return values, comment

    numeric_directive = parse_numeric_directive(stripped)
    if numeric_directive is not None:
        _directive, values = numeric_directive
        return values, comment

    data_match = DATA_LABEL_RE.match(stripped)
    if data_match is None:
        return None
    label, directive, rest = data_match.groups()
    if label_name is not None and label != label_name:
        return None
    numeric_directive = parse_numeric_directive(f"{directive}{rest}")
    if numeric_directive is None:
        return None
    _directive, values = numeric_directive
    return values, comment


def is_top_level_data_line(raw: str) -> bool:
    code, _comment = split_comment(raw)
    if STANDALONE_STORAGE_FULL_RE.match(code):
        return True
    if not is_flush_left(raw):
        return False
    if DATA_LABEL_RE.match(raw):
        return True
    if parse_sptr_label(raw) is not None:
        return True
    return False


def previous_significant_index(lines: list[str], start_idx: int) -> int | None:
    idx = start_idx - 1
    while idx >= 0:
        if not is_comment_or_blank(lines[idx]):
            return idx
        idx -= 1
    return None


def next_significant_index(lines: list[str], start_idx: int) -> int | None:
    idx = start_idx
    while idx < len(lines):
        if not is_comment_or_blank(lines[idx]):
            return idx
        idx += 1
    return None


def line_ends_function(raw: str) -> bool:
    label_match = LABEL_RE.match(raw)
    if label_match:
        is_instr, code = classify_instruction_text(label_match.group(2))
    else:
        is_instr, code = classify_instruction_text(raw)
    if not is_instr:
        return False
    mnemonic = code.split(None, 1)[0].upper()
    return mnemonic in FUNCTION_END_MNEMONICS


def line_is_data_context(raw: str) -> bool:
    code, _comment = split_comment(raw)
    stripped = code.strip()
    if not stripped:
        return False
    if parse_sptr_label(raw) is not None:
        return True
    if STANDALONE_STORAGE_RE.match(stripped):
        return True
    if DATA_LABEL_RE.match(raw):
        return True
    if parse_numeric_directive(stripped) is not None:
        return True
    if stripped.lower().startswith("romdata"):
        return True
    return False


def bare_label_has_code_body(lines: list[str], start_idx: int) -> bool:
    probe_idx = start_idx + 1
    while probe_idx < len(lines):
        probe_raw = lines[probe_idx]
        if is_comment_or_blank(probe_raw):
            probe_idx += 1
            continue
        if is_top_level_data_line(probe_raw):
            return False
        label_match = LABEL_RE.match(probe_raw)
        if label_match:
            is_instr, _code = classify_instruction_text(label_match.group(2))
            return is_instr
        inline_label_match = INLINE_LABEL_RE.match(probe_raw)
        if inline_label_match and is_flush_left(probe_raw) and not looks_like_instruction_token(inline_label_match.group(1)):
            is_instr, _code = classify_instruction_text(f"{inline_label_match.group(2)}{inline_label_match.group(3)}")
            return is_instr
        bare_label_match = BARE_LABEL_RE.match(probe_raw)
        if bare_label_match and is_flush_left(probe_raw) and not looks_like_instruction_token(bare_label_match.group(1)):
            probe_idx += 1
            continue
        is_instr, _code = classify_instruction_text(probe_raw)
        return is_instr
    return False


def colon_label_has_code_body(lines: list[str], start_idx: int) -> bool:
    raw = lines[start_idx]
    label_match = LABEL_RE.match(raw)
    if label_match is None:
        return False
    rest = label_match.group(2)
    is_instr, _code = classify_instruction_text(rest)
    if is_instr:
        return True
    rest_stripped = rest.strip()
    if rest_stripped:
        if parse_numeric_directive(rest_stripped) is not None:
            return False
        if STANDALONE_DATA_DIRECTIVE_RE.match(rest_stripped) is not None:
            return False

    probe_idx = start_idx + 1
    while probe_idx < len(lines):
        probe_raw = lines[probe_idx]
        if is_comment_or_blank(probe_raw):
            probe_idx += 1
            continue
        if render_conditional_line(probe_raw) is not None:
            probe_idx += 1
            continue
        probe_code, _probe_comment = split_comment(probe_raw)
        probe_stripped = probe_code.strip()
        if probe_stripped.lower().startswith((".globl", ".line", ".sym", ".func", ".endfunc", ".text", ".data", ".bss", ".sect", "romdata")):
            probe_idx += 1
            continue
        if parse_sptr_label(probe_raw) is not None:
            probe_idx += 1
            continue
        if DATA_LABEL_RE.match(probe_raw):
            probe_idx += 1
            continue
        if STANDALONE_STORAGE_RE.match(probe_stripped) is not None:
            probe_idx += 1
            continue
        if parse_numeric_directive(probe_stripped) is not None:
            probe_idx += 1
            continue
        probe_label_match = LABEL_RE.match(probe_raw)
        if probe_label_match:
            is_instr, _code = classify_instruction_text(probe_label_match.group(2))
            if is_instr:
                return True
            probe_idx += 1
            continue
        inline_label_match = INLINE_LABEL_RE.match(probe_raw)
        if inline_label_match and is_flush_left(probe_raw) and not looks_like_instruction_token(inline_label_match.group(1)):
            is_instr, _code = classify_instruction_text(f"{inline_label_match.group(2)}{inline_label_match.group(3)}")
            return is_instr
        bare_label_match = BARE_LABEL_RE.match(probe_raw)
        if bare_label_match and is_flush_left(probe_raw) and not looks_like_instruction_token(bare_label_match.group(1)):
            probe_idx += 1
            continue
        is_instr, _code = classify_instruction_text(probe_raw)
        return is_instr

    return False


def classify_instruction_text(text: str) -> tuple[bool, str]:
    stripped = text.strip()
    if not stripped:
        return False, ""
    if stripped.startswith("*") or stripped.startswith(";"):
        return False, ""

    code, _comment = split_comment(stripped)
    code = code.strip()
    if not code:
        return False, ""

    data_label_match = DATA_LABEL_RE.match(code)
    if data_label_match:
        return False, ""

    first = code.split()[0]
    upper = first.upper()
    if first.lower() in CONTROL_KEYWORDS_LOWER:
        return False, ""

    if first.startswith(".") and upper not in {".WORD"}:
        return False, ""

    if upper == "EQU":
        return False, ""

    if upper.startswith(INSTRUCTION_PREFIXES) or upper.isupper():
        return True, code

    return False, ""


def looks_like_instruction_token(token: str) -> bool:
    upper = token.upper()
    if token.lower() in CONTROL_KEYWORDS_LOWER:
        return True
    if token.startswith("."):
        return True
    return upper.startswith(INSTRUCTION_PREFIXES)


def function_has_executable_lines(fn: FunctionBlock) -> bool:
    for raw in fn.lines:
        label_match = LABEL_RE.match(raw)
        if label_match:
            is_instr, _code = classify_instruction_text(label_match.group(2))
            if is_instr:
                return True
            continue

        inline_label_match = INLINE_LABEL_RE.match(raw)
        if inline_label_match and is_flush_left(raw) and not looks_like_instruction_token(inline_label_match.group(1)):
            is_instr, _code = classify_instruction_text(f"{inline_label_match.group(2)}{inline_label_match.group(3)}")
            if is_instr:
                return True
            continue

        is_instr, _code = classify_instruction_text(raw)
        if is_instr:
            return True

    return False


def collect_branch_targets(lines: list[str]) -> set[str]:
    targets: set[str] = set()
    for raw in lines:
        label_match = LABEL_RE.match(raw)
        if label_match:
            text = label_match.group(2)
        else:
            inline_label_match = INLINE_LABEL_RE.match(raw)
            if is_flush_left(raw) and inline_label_match and not looks_like_instruction_token(inline_label_match.group(1)):
                text = f"{inline_label_match.group(2)}{inline_label_match.group(3)}"
            else:
                text = raw
        is_instr, code = classify_instruction_text(text)
        if not is_instr:
            continue

        tokens = code.split(None, 1)
        mnemonic = tokens[0].upper()
        if not mnemonic.startswith("B"):
            continue

        operands = tokens[1] if len(tokens) > 1 else ""
        operands = operands.replace(",", " ")
        for tok in operands.split():
            if tok.startswith("@"):
                continue
            if tok.startswith("*"):
                continue
            if re.fullmatch(r"[0-9][0-9A-Fa-f]*H?", tok):
                continue
            if tok in {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "AR0", "AR1", "AR2", "AR3", "AR4", "AR5", "AR6", "AR7", "RC", "RS", "RE", "BK", "DP", "SP", "ST", "IE", "IF", "IR0", "IR1", "IOF"}:
                continue
            targets.add(tok)
    return targets


def collect_top_level_functions(
    lines: list[str],
    branch_targets: set[str],
    force_function_names: set[str] | None = None,
) -> list[FunctionBlock]:
    if force_function_names is None:
        force_function_names = set()
    functions: list[FunctionBlock] = []
    current: FunctionBlock | None = None
    seen_separator = False

    for idx, raw in enumerate(lines):
        if SEPARATOR_RE.match(raw.strip()):
            seen_separator = True
            current = None
            continue

        standalone_data, _next_idx = collect_standalone_labeled_data(lines, idx, branch_targets)
        if standalone_data is not None:
            current = None
            seen_separator = False
            continue

        label_match = LABEL_RE.match(raw)
        if label_match:
            label = label_match.group(1)
            if not colon_label_has_code_body(lines, idx):
                current = None
                seen_separator = False
                continue
            start_new = False
            prev_idx = previous_significant_index(lines, idx)
            prev_raw = lines[prev_idx] if prev_idx is not None else ""
            prev_ends = prev_idx is None or seen_separator or line_ends_function(prev_raw)

            if label in force_function_names:
                start_new = True
            elif current is None:
                start_new = True
            elif seen_separator:
                start_new = True
            elif prev_ends:
                start_new = True
            elif not function_has_executable_lines(current):
                start_new = False
            elif label not in branch_targets and current.name != label:
                start_new = True

            seen_separator = False

            if start_new:
                current = FunctionBlock(name=label, start_index=idx, end_index=idx)
                functions.append(current)

            if current is None:
                continue

            current.labels.add(label)
            current.lines.append(raw.rstrip())
            current.raw_lines.append(raw.rstrip())
            current.line_numbers.append(idx + 1)
            current.end_index = idx
            continue

        if current is not None and is_top_level_data_line(raw):
            current = None
            seen_separator = False
            continue

        inline_label_match = INLINE_LABEL_RE.match(raw)
        if inline_label_match:
            label = inline_label_match.group(1)
            if is_flush_left(raw) and label in force_function_names:
                current = FunctionBlock(name=label, start_index=idx, end_index=idx)
                current.labels.add(label)
                current.lines.append(f"{label}: {inline_label_match.group(2)}{inline_label_match.group(3)}")
                current.raw_lines.append(raw.rstrip())
                current.line_numbers.append(idx + 1)
                current.end_index = idx
                functions.append(current)
                seen_separator = False
                continue
            if is_flush_left(raw) and not looks_like_instruction_token(label):
                prev_idx = previous_significant_index(lines, idx)
                prev_raw = lines[prev_idx] if prev_idx is not None else ""
                prev_ends = prev_idx is None or seen_separator or line_ends_function(prev_raw)
                prev_is_data = prev_idx is not None and line_is_data_context(prev_raw)
                is_instr, _code = classify_instruction_text(f"{inline_label_match.group(2)}{inline_label_match.group(3)}")
                if is_instr and (prev_ends or (current is None and not prev_is_data)):
                    current = FunctionBlock(name=label, start_index=idx, end_index=idx)
                    current.labels.add(label)
                    current.lines.append(f"{label}: {inline_label_match.group(2)}{inline_label_match.group(3)}")
                    current.raw_lines.append(raw.rstrip())
                    current.line_numbers.append(idx + 1)
                    current.end_index = idx
                    functions.append(current)
                    seen_separator = False
                    continue
            if is_flush_left(raw) and not looks_like_instruction_token(label) and label in branch_targets and current is not None:
                current.labels.add(label)
                current.lines.append(f"{label}: {inline_label_match.group(2)}{inline_label_match.group(3)}")
                current.raw_lines.append(raw.rstrip())
                current.line_numbers.append(idx + 1)
                current.end_index = idx
                continue

        if current is not None:
            current.lines.append(raw.rstrip())
            current.raw_lines.append(raw.rstrip())
            current.line_numbers.append(idx + 1)
            current.end_index = idx
        else:
            seen_separator = False

    return functions


def attach_leading_context(lines: list[str], functions: list[FunctionBlock]) -> None:
    for i in range(len(functions)):
        fn = functions[i]
        prev_end = functions[i - 1].end_index if i > 0 else -1
        start = fn.start_index
        if start <= 0:
            continue

        j = start - 1
        while j > prev_end and is_comment_line(lines[j]):
            j -= 1
        new_start = j + 1
        if new_start >= start:
            continue

        fn.start_index = new_start
        fn.lines = [line.rstrip() for line in lines[new_start:start]] + fn.lines
        fn.raw_lines = [line.rstrip() for line in lines[new_start:start]] + fn.raw_lines
        fn.line_numbers = list(range(new_start + 1, start + 1)) + fn.line_numbers


def split_leading_comment_lines(lines: list[str]) -> tuple[list[str], list[str]]:
    idx = 0
    while idx < len(lines) and is_comment_line(lines[idx]):
        idx += 1
    return lines[:idx], lines[idx:]


def collect_function_colon_entry_labels(fn: FunctionBlock) -> list[str]:
    _leading_comment_lines, body_lines = split_leading_comment_lines(fn.lines)
    _leading_raw_comment_lines, body_raw_lines = split_leading_comment_lines(fn.raw_lines)
    entry_labels: list[str] = []
    for raw, raw_source in zip(body_lines, body_raw_lines):
        if is_comment_or_blank(raw_source):
            continue
        if LABEL_RE.match(raw_source) is None:
            break
        label_match = LABEL_RE.match(raw)
        if label_match:
            entry_labels.append(label_match.group(1))
            is_instr, _code = classify_instruction_text(label_match.group(2))
            if is_instr:
                break
            continue

        break

    return entry_labels


def assign_function_aliases(functions: list[FunctionBlock]) -> None:
    for fn in functions:
        entry_labels = collect_function_colon_entry_labels(fn)
        if len(entry_labels) <= 1:
            fn.aliases = []
            continue
        fn.name = entry_labels[-1]
        fn.aliases = entry_labels[:-1]


def render_leading_comment_block(lines: list[str]) -> list[str]:
    if not lines:
        return []
    out = [f"/* {lines[0].rstrip()}"]
    for raw in lines[1:]:
        out.append(raw.rstrip())
    out.append(" */")
    return out


def render_function_asm_comment(raw: str, address: int | None) -> str | None:
    asm_comment = render_asm_comment(raw)
    if asm_comment is None:
        return None
    if address is None:
        return asm_comment
    prefix = "// asm: "
    if asm_comment.startswith(prefix):
        return f"// asm {address:08X}: {asm_comment[len(prefix):]}"
    return asm_comment


def lookup_instruction_address_for_line(
    raw: str,
    line_no: int,
    body_line_numbers: list[int],
    instruction_addresses: dict[int, int] | None,
) -> int | None:
    if instruction_addresses is None:
        return None
    direct = instruction_addresses.get(line_no)
    if direct is not None:
        return direct
    if not is_flush_left(raw):
        return None
    for candidate in body_line_numbers:
        if candidate <= line_no:
            continue
        mapped = instruction_addresses.get(candidate)
        if mapped is not None:
            return mapped
    return None


def render_function(fn: FunctionBlock, instruction_addresses: dict[int, int] | None = None) -> list[str]:
    out: list[str] = []
    leading_comment_lines, body_lines = split_leading_comment_lines(fn.lines)
    out.extend(render_leading_comment_block(leading_comment_lines))
    body_line_numbers = fn.line_numbers[len(leading_comment_lines):]
    fn_ident = sanitize_identifier(fn.name)
    out.append(f"void {fn_ident}(void)")
    out.append("{")

    emitted_any = False
    first_label_emitted = False
    pending_entry_labels = [*fn.aliases, fn.name]
    for raw, line_no in zip(body_lines, body_line_numbers):
        if pending_entry_labels:
            label_match = LABEL_RE.match(raw)
            if label_match and label_match.group(1) == pending_entry_labels[0]:
                pending_entry_labels.pop(0)
                line_address = lookup_instruction_address_for_line(raw, line_no, body_line_numbers, instruction_addresses)
                is_instr, code = classify_instruction_text(label_match.group(2))
                if is_instr:
                    if line_address is None:
                        out.append(f"    // asm: {code}")
                    else:
                        out.append(f"    // asm {line_address:08X}: {code}")
                    emitted_any = True
                continue

            inline_label_match = INLINE_LABEL_RE.match(raw)
            if (
                inline_label_match
                and is_flush_left(raw)
                and not looks_like_instruction_token(inline_label_match.group(1))
                and inline_label_match.group(1) == pending_entry_labels[0]
            ):
                pending_entry_labels.pop(0)
                line_address = lookup_instruction_address_for_line(raw, line_no, body_line_numbers, instruction_addresses)
                code = f"{inline_label_match.group(2)}{inline_label_match.group(3)}".rstrip()
                is_instr, code = classify_instruction_text(code)
                if is_instr:
                    if line_address is None:
                        out.append(f"    // asm: {code}")
                    else:
                        out.append(f"    // asm {line_address:08X}: {code}")
                    emitted_any = True
                continue

            bare_label_match = BARE_LABEL_RE.match(raw)
            if (
                is_flush_left(raw)
                and bare_label_match
                and bare_label_match.group(1) in fn.labels
                and bare_label_match.group(1) == pending_entry_labels[0]
            ):
                pending_entry_labels.pop(0)
                continue

            pending_entry_labels = []

        line_address = lookup_instruction_address_for_line(raw, line_no, body_line_numbers, instruction_addresses)
        asm_comment = render_function_asm_comment(raw, line_address)
        if asm_comment is not None:
            out.append(f"    {asm_comment}")
            continue

        conditional = render_conditional_line(raw)
        if conditional is not None:
            out.append(conditional)
            continue

        label_match = LABEL_RE.match(raw)
        if label_match:
            label = sanitize_identifier(label_match.group(1))
            if not first_label_emitted and label == fn_ident:
                first_label_emitted = True
            else:
                out.append(f"{label}:")
                first_label_emitted = True
            rest = label_match.group(2)
            is_instr, code = classify_instruction_text(rest)
            if is_instr:
                if line_address is None:
                    out.append(f"    // asm: {rest.lstrip().rstrip()}")
                else:
                    out.append(f"    // asm {line_address:08X}: {rest.lstrip().rstrip()}")
                emitted_any = True
            continue

        inline_label_match = INLINE_LABEL_RE.match(raw)
        if inline_label_match:
            label = inline_label_match.group(1)
            code = f"{inline_label_match.group(2)}{inline_label_match.group(3)}".rstrip()
            is_instr, code = classify_instruction_text(code)
            if is_flush_left(raw) and not looks_like_instruction_token(label) and is_instr:
                label_ident = sanitize_identifier(label)
                if not first_label_emitted and label_ident == fn_ident:
                    first_label_emitted = True
                else:
                    out.append(f"{label_ident}:")
                    first_label_emitted = True
                if line_address is None:
                    out.append(f"    // asm: {code}")
                else:
                    out.append(f"    // asm {line_address:08X}: {code}")
                emitted_any = True
                continue

        bare_label_match = BARE_LABEL_RE.match(raw)
        if is_flush_left(raw) and bare_label_match and bare_label_match.group(1) in fn.labels:
            label_ident = sanitize_identifier(bare_label_match.group(1))
            if not first_label_emitted and label_ident == fn_ident:
                first_label_emitted = True
            else:
                out.append(f"{label_ident}:")
                first_label_emitted = True
            continue

        is_instr, code = classify_instruction_text(raw)
        if not is_instr:
            continue
        if line_address is None:
            out.append(f"    // asm: {raw.rstrip()}")
        else:
            out.append(f"    // asm {line_address:08X}: {raw.rstrip()}")
        emitted_any = True

    if emitted_any:
        out.append(f'    TRACE_EVENT(&g_crusn_machine->trace, "function", "{fn.name}", 0, 0);')
    else:
        out.append("    /* no executable asm lines detected */")
    out.append("    UNIMPL();")
    out.append("}")
    return out


def infer_word_symbol(var: WordVariable, module: str, symbol_table: dict[str, SymbolInfo] | None = None) -> SymbolInfo:
    if len(var.values) == 1:
        value = var.values[0].strip()
        rendered_value = convert_expr(value)
        if var.name.lower().endswith("i") and BRANCH_TARGET_RE.fullmatch(value) and parse_int_token(value) is None:
            return SymbolInfo(name=var.name, kind="define", module=module, expr=rendered_value)
        if var.name.endswith("I") and value == var.name[:-1]:
            return SymbolInfo(name=var.name, kind="define", module=module, expr=rendered_value)
        if symbol_table is not None:
            target = symbol_table.get(value)
            if target is not None and target.kind == "function":
                return SymbolInfo(name=var.name, kind="variable", module=module, c_type="void (*)(void)")
            if target is not None and target.kind == "variable" and target.c_type:
                if target.c_type == "const char *":
                    return SymbolInfo(name=var.name, kind="variable", module=module, c_type="const char *")
                return SymbolInfo(name=var.name, kind="variable", module=module, c_type=f"{target.c_type} *")
        return SymbolInfo(name=var.name, kind="variable", module=module, c_type="int")

    if symbol_table is not None and all(parse_int_token(value) is None for value in var.values):
        targets = [symbol_table.get(value) for value in var.values]
        if all(target is not None and target.kind == "function" for target in targets):
            return SymbolInfo(
                name=var.name,
                kind="variable",
                module=module,
                c_type="void (*)(void)",
                array_expr=str(len(var.values)),
            )
        if all(target is not None and target.kind == "variable" and target.c_type for target in targets):
            c_types = {target.c_type for target in targets if target is not None}
            if len(c_types) == 1:
                only_type = next(iter(c_types))
                if only_type == "const char *":
                    return SymbolInfo(
                        name=var.name,
                        kind="variable",
                        module=module,
                        c_type="const char *",
                        array_expr=str(len(var.values)),
                    )
                return SymbolInfo(
                    name=var.name,
                    kind="variable",
                    module=module,
                    c_type=f"{only_type} *",
                    array_expr=str(len(var.values)),
                )

    return SymbolInfo(name=var.name, kind="variable", module=module, c_type="int", array_expr=str(len(var.values)))


def infer_numeric_symbol(var: NumericVariable, module: str, symbol_table: dict[str, SymbolInfo] | None = None) -> SymbolInfo:
    if var.directive == ".float":
        return SymbolInfo(name=var.name, kind="variable", module=module, c_type="float", array_expr=str(len(var.values)))
    if var.directive in {".word", ".intdata"}:
        return infer_word_symbol(WordVariable(name=var.name, values=var.values, asm_lines=var.asm_lines), module, symbol_table)
    return SymbolInfo(name=var.name, kind="variable", module=module, c_type="int", array_expr=str(len(var.values)))


def infer_string_symbol(name: str, module: str) -> SymbolInfo:
    return SymbolInfo(name=name, kind="variable", module=module, c_type="const char *")


def infer_storage_symbol(name: str, size_expr: str, module: str) -> SymbolInfo:
    expr = convert_expr(size_expr).strip()
    if expr in {"", "0", "1"}:
        return SymbolInfo(name=name, kind="variable", module=module, c_type="int")
    return SymbolInfo(name=name, kind="variable", module=module, c_type="int", array_expr=expr)


def render_numeric_variable(var: NumericVariable, symbol_table: dict[str, SymbolInfo] | None = None) -> list[str]:
    out: list[str] = []
    inferred_symbol = infer_numeric_symbol(var, "", symbol_table)
    existing_symbol = symbol_table.get(var.name) if symbol_table is not None else None
    if existing_symbol is None:
        symbol = inferred_symbol
    elif existing_symbol.kind == "define" or inferred_symbol.kind == "define":
        symbol = existing_symbol if existing_symbol.kind == "define" else inferred_symbol
    else:
        symbol = SymbolInfo(
            name=existing_symbol.name,
            kind=existing_symbol.kind,
            module=existing_symbol.module,
            c_type=inferred_symbol.c_type if existing_symbol.c_type == "int" and inferred_symbol.c_type != "int" else existing_symbol.c_type,
            array_expr=existing_symbol.array_expr if existing_symbol.array_expr is not None else inferred_symbol.array_expr,
            expr=existing_symbol.expr,
        )
    if symbol.kind == "define" and symbol.expr is not None and symbol.expr == var.name[:-1] and var.name.endswith("I"):
        return out
    if var.asm_lines:
        out.append(f"/* asm: {var.asm_lines[0].strip()} */")
        for extra_line in var.asm_lines[1:]:
            out.append(f"/* asm: \t{extra_line.strip()} */")
    ident = sanitize_identifier(var.name)
    if symbol.kind == "define" and symbol.expr is not None:
        out.append(f"#define {ident} {symbol.expr}")
    elif len(var.values) == 1:
        value = var.values[0].strip()
        rendered_value = format_float_expr(value) if var.directive == ".float" else convert_expr(value)
        out.append(f"{variable_definition_prefix(var.name, symbol.c_type)} = {rendered_value};")
    else:
        if var.string_literal_values is not None:
            prefix = variable_definition_prefix(var.name, symbol.c_type, symbol.array_expr, omit_array_size=True)
            values = ", ".join(var.string_literal_values)
            out.append(f"{prefix} = {{ {values} }};")
            return out
        structured_rows: list[str] = []
        if var.asm_lines:
            for asm_line in var.asm_lines:
                parsed_row = parse_numeric_data_line(asm_line, var.name)
                if parsed_row is None:
                    continue
                row_values, asm_comment = parsed_row
                if var.directive == ".float":
                    rendered_values = ", ".join(format_float_expr(value) for value in row_values)
                else:
                    rendered_values = ", ".join(convert_expr(value) for value in row_values)
                comment_text = asm_comment[1:].strip() if asm_comment.startswith(";") else asm_comment.strip()
                if comment_text:
                    structured_rows.append(f"    {rendered_values}, // {comment_text}")
                else:
                    structured_rows.append(f"    {rendered_values},")
        if structured_rows:
            prefix = variable_definition_prefix(var.name, symbol.c_type, symbol.array_expr, omit_array_size=True)
            out.append(f"{prefix} = {{")
            out.extend(structured_rows)
            out.append("};")
        else:
            if var.directive == ".float":
                values = ", ".join(format_float_expr(value) for value in var.values)
            else:
                values = ", ".join(convert_expr(value) for value in var.values)
            prefix = variable_definition_prefix(var.name, symbol.c_type, symbol.array_expr, omit_array_size=True)
            out.append(f"{prefix} = {{ {values} }};")
    return out


def render_word_variable(var: WordVariable, symbol_table: dict[str, SymbolInfo] | None = None) -> list[str]:
    return render_numeric_variable(
        NumericVariable(name=var.name, directive=".word", values=var.values, asm_lines=var.asm_lines),
        symbol_table,
    )


def render_set_define(entry: SetDefine) -> list[str]:
    expr = f"({entry.expr})" if needs_parens(entry.expr) else entry.expr
    rendered = f"#define {entry.name} {expr}"
    comment = entry.comment.strip()
    if comment:
        if comment.startswith(";"):
            comment = comment[1:].lstrip()
        rendered += f" //{comment}"
    return [rendered]


def render_discovered_defines_header(defines: list[DefineEntry]) -> str:
    out = [
        "#ifndef DISCOVERED_DEFINES_H",
        "#define DISCOVERED_DEFINES_H",
        "",
        "/* Generated from tools/ida/discovered_defines.txt. */",
        "",
    ]
    for entry in defines:
        out.extend(render_set_define(SetDefine(name=entry.name, expr=entry.expr, origin="discovered")))
    out.extend([
        "",
        "#endif /* DISCOVERED_DEFINES_H */",
        "",
    ])
    return "\n".join(out)


def render_discovered_labels_header(labels: list[LabelEntry]) -> str:
    out = [
        "#ifndef DISCOVERED_LABELS_H",
        "#define DISCOVERED_LABELS_H",
        "",
        "/* Generated from tools/ida/address.map. */",
        "",
    ]
    for entry in labels:
        out.append(f"#define {entry.name} 0x{entry.addr:08X}")
    out.extend([
        "",
        "#endif /* DISCOVERED_LABELS_H */",
        "",
    ])
    return "\n".join(out)


def render_string_variable(var: StringVariable) -> list[str]:
    return [f"{variable_definition_prefix(var.name, 'const char *')} = {var.c_string};"]


def render_sptr_variable(name: str, c_string: str, asm_line: str) -> list[str]:
    ident = sanitize_identifier(name)
    return [
        f"/* asm: {asm_line.strip()} */",
        f"const char *{ident} = {c_string};",
    ]


def render_storage_variable(name: str, size_expr: str, asm_line: str) -> list[str]:
    return [
        f"/* asm: {name}\t{asm_line.strip()} */",
        storage_declaration(name, size_expr),
    ]


def filter_renderable_asm_lines(lines: list[str]) -> list[str]:
    return [line for line in lines if line.strip() and not is_comment_line(line)]


def render_local_function_prototypes(functions: list[FunctionBlock]) -> list[str]:
    out: list[str] = []
    seen: set[str] = set()
    for fn in functions:
        for alias in fn.aliases:
            alias_ident = sanitize_identifier(alias)
            fn_ident = sanitize_identifier(fn.name)
            if alias_ident in seen:
                continue
            out.append(f"#define {alias_ident} {fn_ident}")
            seen.add(alias_ident)
        if fn.name in seen:
            continue
        out.append(f"void {sanitize_identifier(fn.name)}(void);")
        seen.add(fn.name)
    return out


def collect_standalone_labeled_data(
    lines: list[str],
    start_idx: int,
    branch_targets: set[str] | None = None,
) -> tuple[StandaloneLabeledData | None, int]:
    if branch_targets is None:
        branch_targets = set()
    raw = lines[start_idx]
    label_match = LABEL_RE.match(raw)
    if label_match:
        label, rest = label_match.groups()
        rest_code, _rest_comment = split_comment(rest)
        if rest_code.strip():
            stripped = rest_code.strip()
            numeric_directive = parse_numeric_directive(stripped)
            if numeric_directive is not None:
                directive_name, values = numeric_directive
                return StandaloneLabeledData(label, directive_name, ",".join(values), [raw.rstrip()]), start_idx + 1
            directive_match = STANDALONE_DATA_DIRECTIVE_RE.match(stripped)
            if directive_match is not None:
                directive, directive_rest = directive_match.groups()
                return StandaloneLabeledData(label, directive, directive_rest, [raw.rstrip()]), start_idx + 1
            return None, start_idx
    else:
        raw_code, _raw_comment = split_comment(raw)
        bare_label_match = BARE_LABEL_RE.match(raw_code)
        if not bare_label_match or not is_flush_left(raw):
            return None, start_idx
        label = bare_label_match.group(1)
        if label in branch_targets:
            return None, start_idx
        if looks_like_instruction_token(label):
            probe_idx = start_idx + 1
            next_data_like = False
            while probe_idx < len(lines):
                probe_raw = lines[probe_idx]
                if is_comment_or_blank(probe_raw):
                    probe_idx += 1
                    continue
                if is_flush_left(probe_raw):
                    break
                probe_code, _probe_comment = split_comment(probe_raw)
                probe_stripped = probe_code.strip()
                next_data_like = (
                    parse_numeric_directive(probe_stripped) is not None
                    or parse_simple_string_operand(probe_stripped) is not None
                    or STANDALONE_STORAGE_RE.match(probe_stripped) is not None
                )
                break
            if not next_data_like:
                return None, start_idx

    asm_lines = [raw.rstrip()]
    next_idx = start_idx + 1
    numeric_values: list[str] = []
    saw_numeric_directive = False
    numeric_directive_name: str | None = None
    mixed_numeric_directives = False
    while next_idx < len(lines):
        next_raw = lines[next_idx]
        if is_comment_or_blank(next_raw):
            asm_lines.append(next_raw.rstrip())
            next_idx += 1
            continue
        if is_flush_left(next_raw):
            if saw_numeric_directive:
                return StandaloneLabeledData(label, ".intdata", ",".join(numeric_values), asm_lines), next_idx
            return None, start_idx
        next_code, _next_comment = split_comment(next_raw)
        stripped = next_code.strip()
        if not stripped:
            asm_lines.append(next_raw.rstrip())
            next_idx += 1
            continue
        numeric_directive = parse_numeric_directive(stripped)
        if numeric_directive is not None:
            directive_name, values = numeric_directive
            asm_lines.append(next_raw.rstrip())
            numeric_values.extend(values)
            saw_numeric_directive = True
            if numeric_directive_name is None:
                numeric_directive_name = directive_name
            elif numeric_directive_name != directive_name:
                mixed_numeric_directives = True
            next_idx += 1
            continue
        directive_match = STANDALONE_DATA_DIRECTIVE_RE.match(stripped)
        if directive_match is None:
            break
        directive, directive_rest = directive_match.groups()
        if saw_numeric_directive:
            normalized = ".intdata" if mixed_numeric_directives or numeric_directive_name is None else numeric_directive_name
            return StandaloneLabeledData(label, normalized, ",".join(numeric_values), asm_lines), next_idx
        asm_lines.append(next_raw.rstrip())
        return StandaloneLabeledData(label, directive, directive_rest, asm_lines), next_idx + 1
    if saw_numeric_directive:
        normalized = ".intdata" if mixed_numeric_directives or numeric_directive_name is None else numeric_directive_name
        return StandaloneLabeledData(label, normalized, ",".join(numeric_values), asm_lines), next_idx
    return None, start_idx


def collect_source_label_names(lines: list[str]) -> set[str]:
    labels: set[str] = set()
    for raw in lines:
        code, _comment = split_comment(raw)
        if not code.strip():
            continue
        storage_label_match = STANDALONE_STORAGE_LABEL_RE.match(code)
        if storage_label_match:
            labels.add(storage_label_match.group(1))
            continue
        label_match = LABEL_RE.match(code)
        if label_match:
            labels.add(label_match.group(1))
            continue
        if not is_flush_left(raw):
            continue
        bare_label_match = BARE_LABEL_RE.match(code)
        if bare_label_match:
            labels.add(bare_label_match.group(1))
            continue
        data_match = DATA_LABEL_RE.match(code)
        if data_match:
            labels.add(data_match.group(1))
            continue
        inline_label_match = INLINE_LABEL_RE.match(code)
        if inline_label_match and not looks_like_instruction_token(inline_label_match.group(1)):
            labels.add(inline_label_match.group(1))
    return labels


def render_top_level_items(
    lines: list[str],
    functions: list[FunctionBlock],
    branch_targets: set[str],
    address_map: dict[str, int],
    module: str,
    symbol_table: dict[str, SymbolInfo] | None = None,
    type_overrides: dict[str, TypeOverride] | None = None,
    strip_asm_comment_markers: bool = False,
    emit_set_asm_comments: bool = False,
    emit_set_defines: bool = True,
    collapsed_string_tables: dict[str, list[str]] | None = None,
    skipped_string_labels: set[str] | None = None,
) -> list[str]:
    if collapsed_string_tables is None:
        collapsed_string_tables = {}
    if skipped_string_labels is None:
        skipped_string_labels = set()

    def flush_pending_comments() -> None:
        nonlocal pending_comment_lines
        if pending_comment_lines:
            out.extend(render_leading_comment_block(pending_comment_lines))
            pending_comment_lines = []

    function_line_indexes: set[int] = set()
    for fn in functions:
        if fn.start_index < 0 or fn.end_index < 0:
            continue
        function_line_indexes.update(range(fn.start_index, fn.end_index + 1))

    out: list[str] = []
    emitted_sets: set[str] = set()
    pending_comment_lines: list[str] = []
    idx = 0
    while idx < len(lines):
        raw = lines[idx]
        if idx in function_line_indexes:
            pending_comment_lines = []
            idx += 1
            continue

        if not raw.strip():
            pending_comment_lines = []
            idx += 1
            continue

        asm_comment = render_asm_comment(raw, strip_asm_comment_markers=strip_asm_comment_markers)
        if asm_comment is not None and strip_asm_comment_markers:
            out.append(asm_comment)
            idx += 1
            continue

        if render_asm_comment(raw) is not None:
            pending_comment_lines.append(raw.rstrip())
            idx += 1
            continue

        conditional = render_conditional_line(raw)
        if conditional is not None:
            flush_pending_comments()
            out.append(conditional)
            idx += 1
            continue

        code, comment = split_comment(raw)
        set_match = SET_RE.match(code)
        if set_match:
            name, expr = set_match.groups()
            define_entry = SetDefine(name=name, expr=convert_expr(expr), comment=comment)
            flush_pending_comments()
            if emit_set_asm_comments:
                out.append(f"// asm: {raw.rstrip()}")
            if emit_set_defines:
                if name in emitted_sets:
                    out.append(f"#undef {name}")
                out.extend(render_set_define(define_entry))
                emitted_sets.add(name)
            idx += 1
            continue

        storage_match = STANDALONE_STORAGE_FULL_RE.match(code)
        if storage_match:
            directive, label, rest = storage_match.groups()
            if is_omitted_symbol(label, type_overrides):
                pending_comment_lines = []
                idx += 1
                continue
            flush_pending_comments()
            out.extend(render_storage_variable(label, parse_storage_size(rest), f"{directive}\t{label}{rest}"))
            idx += 1
            continue

        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx, branch_targets)
        if standalone_data is not None:
            if is_omitted_symbol(standalone_data.name, type_overrides):
                pending_comment_lines = []
                idx = next_idx
                continue
            flush_pending_comments()
            directive_lower = standalone_data.directive.lower()
            if directive_lower in {".word", ".intdata", ".float"}:
                values: list[str] = []
                if directive_lower in {".intdata", ".float"}:
                    values = [part.strip() for part in standalone_data.rest.split(",") if part.strip()]
                else:
                    first_row = parse_numeric_data_line(standalone_data.asm_lines[0], standalone_data.name)
                    if first_row is not None:
                        row_values, _comment = first_row
                        values.extend(row_values)
                    for asm_line in standalone_data.asm_lines[1:]:
                        asm_code, _asm_comment = split_comment(asm_line)
                        stripped = asm_code.strip()
                        if not stripped.lower().startswith(".word"):
                            continue
                        operands = stripped[5:].strip()
                        if operands:
                            values.extend(part.strip() for part in operands.split(",") if part.strip())
                if not values:
                    values = ["0"]
                if (
                    len(values) == 1
                    and next_idx < len(lines)
                    and is_flush_left(lines[next_idx])
                ):
                    next_code, _next_comment = split_comment(lines[next_idx])
                    next_label_match = BARE_LABEL_RE.match(next_code)
                    if next_label_match and next_label_match.group(1) == values[0]:
                        out.append(f"/* asm: {standalone_data.asm_lines[0].strip()} */")
                        out.append(f"#define {sanitize_identifier(standalone_data.name)} {convert_expr(values[0])}")
                        idx = next_idx
                        continue
                out.extend(
                    render_numeric_variable(
                        NumericVariable(
                            name=standalone_data.name,
                            directive=directive_lower,
                            values=values,
                            asm_lines=filter_renderable_asm_lines(standalone_data.asm_lines),
                            string_literal_values=collapsed_string_tables.get(standalone_data.name),
                        ),
                        symbol_table,
                    )
                )
            elif directive_lower == ".string":
                c_string = parse_simple_string_operand(standalone_data.rest)
                if c_string is not None and standalone_data.name not in skipped_string_labels:
                    out.extend(render_string_variable(StringVariable(name=standalone_data.name, c_string=c_string)))
            elif directive_lower in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
                out.extend(
                    render_storage_variable(
                        standalone_data.name,
                        parse_storage_size(standalone_data.rest),
                        "\n".join(line for line in standalone_data.asm_lines if line.strip()),
                    )
                )
            idx = next_idx
            continue

        if not is_flush_left(raw):
            pending_comment_lines = []
            idx += 1
            continue

        sptr_match = parse_sptr_label(raw)
        if sptr_match is not None:
            label, c_string = sptr_match
            flush_pending_comments()
            out.extend(render_sptr_variable(label, c_string, raw))
            idx += 1
            continue

        data_match = DATA_LABEL_RE.match(raw)
        if not data_match:
            pending_comment_lines = []
            idx += 1
            continue

        label, directive, rest = data_match.groups()
        if is_omitted_symbol(label, type_overrides):
            pending_comment_lines = []
            idx += 1
            continue
        directive_lower = directive.lower()
        if directive_lower in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
            flush_pending_comments()
            out.extend(render_storage_variable(label, parse_storage_size(rest), f"{directive}{rest}"))
            idx += 1
            continue
        if directive_lower == ".string":
            c_string = parse_simple_string_operand(rest)
            if c_string is not None and label not in skipped_string_labels:
                flush_pending_comments()
                out.extend(render_string_variable(StringVariable(name=label, c_string=c_string)))
            idx += 1
            continue

        if directive_lower == ".word":
            flush_pending_comments()
            asm_lines = [f"{label}\t.word{rest}"]
            values: list[str] = []
            first_row = parse_numeric_data_line(asm_lines[0], label)
            if first_row is not None:
                row_values, _comment = first_row
                values.extend(row_values)

            next_idx = idx + 1
            while next_idx < len(lines):
                next_raw = lines[next_idx]
                if next_idx in function_line_indexes:
                    break
                if is_comment_or_blank(next_raw):
                    break
                if is_flush_left(next_raw):
                    break

                next_code, _next_comment = split_comment(next_raw)
                next_stripped = next_code.strip()
                if not next_stripped:
                    break
                parsed_row = parse_numeric_data_line(next_raw)
                if parsed_row is not None:
                    asm_lines.append(next_raw.rstrip())
                    row_values, _comment = parsed_row
                    values.extend(row_values)
                    next_idx += 1
                    continue
                break

            if not values:
                values = ["0"]
            if (
                len(values) == 1
                and next_idx < len(lines)
                and is_flush_left(lines[next_idx])
            ):
                next_code, _next_comment = split_comment(lines[next_idx])
                next_label_match = BARE_LABEL_RE.match(next_code)
                if next_label_match and next_label_match.group(1) == values[0]:
                    out.append(f"/* asm: {asm_lines[0].strip()} */")
                    out.append(f"#define {sanitize_identifier(label)} {convert_expr(values[0])}")
                    idx = next_idx
                    continue
            out.extend(
                render_numeric_variable(
                    NumericVariable(
                        name=label,
                        directive=".word",
                        values=values,
                        asm_lines=asm_lines,
                        string_literal_values=collapsed_string_tables.get(label),
                    ),
                    symbol_table,
                )
            )
            idx = next_idx
            continue

        if directive_lower == ".float":
            flush_pending_comments()
            asm_lines = [f"{label}\t.float{rest}"]
            values: list[str] = []
            first_row = parse_numeric_data_line(asm_lines[0], label)
            if first_row is not None:
                row_values, _comment = first_row
                values.extend(row_values)

            next_idx = idx + 1
            while next_idx < len(lines):
                next_raw = lines[next_idx]
                if next_idx in function_line_indexes:
                    break
                if is_comment_or_blank(next_raw):
                    break
                if is_flush_left(next_raw):
                    break

                next_code, _next_comment = split_comment(next_raw)
                next_stripped = next_code.strip()
                if not next_stripped:
                    break
                parsed_row = parse_numeric_data_line(next_raw)
                if parsed_row is not None:
                    asm_lines.append(next_raw.rstrip())
                    row_values, _comment = parsed_row
                    values.extend(row_values)
                    next_idx += 1
                    continue
                break

            if not values:
                values = ["0"]
            out.extend(
                render_numeric_variable(
                    NumericVariable(name=label, directive=".float", values=values, asm_lines=asm_lines),
                    symbol_table,
                )
            )
            idx = next_idx
            continue

        pending_comment_lines = []
        idx += 1

    while out and out[-1] == "":
        out.pop()
    return out


def render_module(
    src_path: Path,
    address_map: dict[str, int],
    owner_headers: dict[str, str],
    own_header: str | None,
    discovered_header_needed: bool = False,
    type_overrides: dict[str, TypeOverride] | None = None,
    discovered_labels_needed: bool = False,
    instruction_addresses: dict[int, int] | None = None,
) -> str:
    lines = strip_noncode_definition_blocks(src_path.read_text(errors="ignore").splitlines())
    is_equ_source = src_path.suffix.upper() == ".EQU"
    force_function_names = {
        name
        for name, override in (type_overrides or {}).items()
        if override.force_function
    }
    headers = parse_include_headers(lines)
    if own_header is not None:
        headers.append(own_header)
    if discovered_header_needed:
        headers.append("discovered_defines.h")
    if discovered_labels_needed:
        headers.append("discovered_labels.h")
    headers.extend(sorted(collect_word_symbol_dependencies(lines, owner_headers, src_path.stem.upper())))
    headers = list(dict.fromkeys(headers))
    branch_targets = collect_branch_targets(lines)
    functions = collect_top_level_functions(lines, branch_targets, force_function_names)
    attach_leading_context(lines, functions)
    assign_function_aliases(functions)
    symbol_table = collect_module_symbol_table(src_path, address_map, type_overrides)
    collapsed_string_tables, skipped_string_labels = collect_collapsible_string_tables(lines, branch_targets, symbol_table)
    top_level_items = render_top_level_items(
        lines,
        functions,
        branch_targets,
        address_map,
        src_path.stem.upper(),
        symbol_table,
        type_overrides,
        strip_asm_comment_markers=is_equ_source,
        emit_set_asm_comments=is_equ_source,
        emit_set_defines=not is_equ_source,
        collapsed_string_tables=collapsed_string_tables,
        skipped_string_labels=skipped_string_labels,
    )

    out: list[str] = []
    out.append('#include "../core/cpu.h"')
    out.append('#include "../core/machine.h"')
    for header in headers:
        out.append(f'#include "{header}"')
    out.append("")
    out.append("/*")
    out.append(f" * Source module: asm/{src_path.name}")
    out.append(" */")
    local_prototypes = render_local_function_prototypes(functions)
    if local_prototypes:
        out.append("")
        out.extend(local_prototypes)
    out.append("")

    if top_level_items:
        out.extend(top_level_items)
        out.append("")

    rendered_count = 0
    seen_names: set[str] = set()
    for fn in functions:
        ident = sanitize_identifier(fn.name)
        if ident in seen_names:
            continue
        seen_names.add(ident)
        out.extend(render_function(fn, instruction_addresses))
        out.append("")
        rendered_count += 1

    if rendered_count == 0 and not (is_equ_source and top_level_items):
        fallback = sanitize_identifier(src_path.stem.lower())
        out.append(f"void {fallback}(void)")
        out.append("{")
        out.append("    UNIMPL();")
        out.append("}")
        out.append("")

    return "\n".join(out).rstrip() + "\n"


def parse_romlst_label_types(labels_path: Path) -> dict[str, str]:
    out: dict[str, str] = {}
    if not labels_path.exists():
        return out
    for raw in labels_path.read_text(errors="ignore").splitlines():
        if not raw.strip():
            continue
        parts = raw.split("\t")
        if len(parts) < 4:
            continue
        out.setdefault(parts[0], parts[3].lower())
    return out


def iter_operand_symbol_tokens(text: str) -> Iterable[str]:
    stripped = text.strip()
    if not stripped:
        return ()
    parts = stripped.split(None, 1)
    if len(parts) < 2:
        return ()
    operands = parts[1]
    return BRANCH_TARGET_RE.findall(operands)


def extract_reference_text(raw: str) -> str:
    label_match = LABEL_RE.match(raw)
    if label_match:
        return label_match.group(2)
    inline_label_match = INLINE_LABEL_RE.match(raw)
    if is_flush_left(raw) and inline_label_match and not looks_like_instruction_token(inline_label_match.group(1)):
        return f"{inline_label_match.group(2)}{inline_label_match.group(3)}"
    return raw


def collect_defined_data_symbols(lines: list[str]) -> set[str]:
    defined: set[str] = set()
    branch_targets = collect_branch_targets(lines)
    idx = 0
    while idx < len(lines):
        raw = lines[idx]
        code, _comment = split_comment(raw)
        if STANDALONE_STORAGE_FULL_RE.match(code):
            defined.add(STANDALONE_STORAGE_FULL_RE.match(code).group(2))  # type: ignore[union-attr]
            idx += 1
            continue
        if parse_sptr_label(raw) is not None:
            defined.add(parse_sptr_label(raw)[0])  # type: ignore[index]
            idx += 1
            continue
        data_match = DATA_LABEL_RE.match(raw)
        if data_match:
            defined.add(data_match.group(1))
            idx += 1
            continue
        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx, branch_targets)
        if standalone_data is not None:
            defined.add(standalone_data.name)
            idx = next_idx
            continue
        idx += 1
    return defined


def collect_referenced_data_symbols(lines: list[str], label_types: dict[str, str]) -> set[str]:
    refs: set[str] = set()
    for raw in lines:
        if is_comment_or_blank(raw):
            continue
        text = extract_reference_text(raw)
        code, _comment = split_comment(text)
        code = code.strip()
        if not code:
            continue
        first = code.split()[0]
        first_lower = first.lower()
        if first_lower in {
            ".globl",
            ".include",
            ".if",
            ".else",
            ".endif",
            ".file",
            ".version",
            ".text",
            ".data",
            ".bss",
            ".usect",
            "romdata",
            ".sect",
            ".func",
            ".endfunc",
        }:
            continue
        for tok in iter_operand_symbol_tokens(code):
            if tok.upper() in {
                "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
                "AR0", "AR1", "AR2", "AR3", "AR4", "AR5", "AR6", "AR7",
                "RC", "RS", "RE", "BK", "DP", "SP", "ST", "IE", "IF", "IR0", "IR1", "IOF",
            }:
                continue
            if tok.lower() in CONTROL_KEYWORDS_LOWER:
                continue
            if tok.endswith("?"):
                continue
            if "_ptr" in tok.lower():
                continue
            if label_types.get(tok) == "data":
                refs.add(tok)
    return refs


def collect_referenced_define_symbols(lines: list[str], define_names: set[str]) -> set[str]:
    refs: set[str] = set()
    for raw in lines:
        if is_comment_or_blank(raw):
            continue
        text = extract_reference_text(raw)
        code, _comment = split_comment(text)
        code = code.strip()
        if not code:
            continue
        for tok in BRANCH_TARGET_RE.findall(code):
            if tok in define_names:
                refs.add(tok)
    return refs


def collect_data_symbol_ref_sources(lines: list[str], label_types: dict[str, str]) -> dict[str, set[str | None]]:
    refs: dict[str, set[str | None]] = {}
    current_owner: str | None = None
    for raw in lines:
        if is_comment_or_blank(raw):
            continue
        owner_for_refs: str | None = None
        if is_flush_left(raw):
            current_owner = None
            data_match = DATA_LABEL_RE.match(raw)
            if data_match:
                current_owner = data_match.group(1)
                owner_for_refs = current_owner
            else:
                standalone = parse_sptr_label(raw)
                if standalone is not None:
                    current_owner = standalone[0]
                    owner_for_refs = current_owner
        else:
            code, _comment = split_comment(raw)
            stripped = code.strip()
            if current_owner is not None and (
                parse_numeric_directive(stripped) is not None
                or stripped.lower().startswith((".string", ".word", ".float"))
            ):
                owner_for_refs = current_owner
        text = extract_reference_text(raw)
        code, _comment = split_comment(text)
        code = code.strip()
        if not code:
            continue
        first = code.split()[0]
        if first.lower() in {
            ".globl",
            ".include",
            ".if",
            ".else",
            ".endif",
            ".file",
            ".version",
            ".text",
            ".data",
            ".bss",
            ".usect",
            "romdata",
            ".sect",
            ".func",
            ".endfunc",
        }:
            continue
        for tok in iter_operand_symbol_tokens(code):
            if tok.upper() in {
                "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
                "AR0", "AR1", "AR2", "AR3", "AR4", "AR5", "AR6", "AR7",
                "RC", "RS", "RE", "BK", "DP", "SP", "ST", "IE", "IF", "IR0", "IR1", "IOF",
            }:
                continue
            if tok.lower() in CONTROL_KEYWORDS_LOWER:
                continue
            if tok.endswith("?"):
                continue
            if "_ptr" in tok.lower():
                continue
            if label_types.get(tok) == "data":
                refs.setdefault(tok, set()).add(owner_for_refs)
    return refs


def collect_collapsible_string_tables(
    lines: list[str],
    branch_targets: set[str],
    symbol_table: dict[str, SymbolInfo],
) -> tuple[dict[str, list[str]], set[str]]:
    label_types = {
        name: ("data" if info.kind == "variable" else "code" if info.kind == "function" else "other")
        for name, info in symbol_table.items()
    }
    ref_sources = collect_data_symbol_ref_sources(lines, label_types)
    string_literals: dict[str, str] = {}
    word_tables: dict[str, list[str]] = {}

    idx = 0
    while idx < len(lines):
        raw = lines[idx]
        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx, branch_targets)
        if standalone_data is not None:
            if standalone_data.directive.lower() == ".string":
                c_string = parse_simple_string_operand(standalone_data.rest)
                if c_string is not None:
                    string_literals[standalone_data.name] = c_string
            elif standalone_data.directive.lower() == ".word":
                values: list[str] = []
                first_row = parse_numeric_data_line(standalone_data.asm_lines[0], standalone_data.name)
                if first_row is not None:
                    row_values, _comment = first_row
                    values.extend(row_values)
                for asm_line in standalone_data.asm_lines[1:]:
                    parsed_row = parse_numeric_data_line(asm_line)
                    if parsed_row is None:
                        continue
                    row_values, _comment = parsed_row
                    values.extend(row_values)
                word_tables[standalone_data.name] = values
            idx = next_idx
            continue

        data_match = DATA_LABEL_RE.match(raw) if is_flush_left(raw) else None
        if data_match:
            label, directive, rest = data_match.groups()
            directive_lower = directive.lower()
            if directive_lower == ".string":
                c_string = parse_simple_string_operand(rest)
                if c_string is not None:
                    string_literals[label] = c_string
            elif directive_lower == ".word":
                values: list[str] = []
                first_row = parse_numeric_data_line(f"{label}\t.word{rest}", label)
                if first_row is not None:
                    row_values, _comment = first_row
                    values.extend(row_values)
                next_idx = idx + 1
                while next_idx < len(lines):
                    next_raw = lines[next_idx]
                    if is_comment_or_blank(next_raw) or is_flush_left(next_raw):
                        break
                    parsed_row = parse_numeric_data_line(next_raw)
                    if parsed_row is None:
                        break
                    row_values, _comment = parsed_row
                    values.extend(row_values)
                    next_idx += 1
                word_tables[label] = values
                idx = next_idx
                continue
        idx += 1

    collapsed_tables: dict[str, list[str]] = {}
    skipped_labels: set[str] = set()
    for table_name, values in word_tables.items():
        if not values:
            continue
        if any(parse_int_token(value) is not None for value in values):
            continue
        targets = [symbol_table.get(value) for value in values]
        if not all(target is not None and target.kind == "variable" and target.c_type == "const char *" for target in targets):
            continue
        if not all(value in string_literals for value in values):
            continue
        if not all(ref_sources.get(value) == {table_name} for value in values):
            continue
        collapsed_tables[table_name] = [string_literals[value] for value in values]
        skipped_labels.update(values)

    return collapsed_tables, skipped_labels


def collect_module_symbol_table(
    src_path: Path,
    address_map: dict[str, int],
    type_overrides: dict[str, TypeOverride] | None = None,
) -> dict[str, SymbolInfo]:
    lines = strip_noncode_definition_blocks(src_path.read_text(errors="ignore").splitlines())
    module = src_path.stem.upper()
    symbol_table: dict[str, SymbolInfo] = {}
    force_function_names = {
        name
        for name, override in (type_overrides or {}).items()
        if override.force_function
    }

    branch_targets = collect_branch_targets(lines)
    functions = collect_top_level_functions(lines, branch_targets, force_function_names)
    attach_leading_context(lines, functions)
    assign_function_aliases(functions)
    for fn in functions:
        symbol_table.setdefault(fn.name, SymbolInfo(name=fn.name, kind="function", module=module))
        for alias in fn.aliases:
            symbol_table.setdefault(alias, SymbolInfo(name=alias, kind="define", module=module, expr=fn.name))

    function_line_indexes: set[int] = set()
    for fn in functions:
        if fn.start_index < 0 or fn.end_index < 0:
            continue
        function_line_indexes.update(range(fn.start_index, fn.end_index + 1))

    idx = 0
    while idx < len(lines):
        raw = lines[idx]
        if idx in function_line_indexes:
            idx += 1
            continue

        code, _comment = split_comment(raw)
        storage_match = STANDALONE_STORAGE_FULL_RE.match(code)
        if storage_match:
            directive, label, rest = storage_match.groups()
            if is_omitted_symbol(label, type_overrides):
                idx += 1
                continue
            symbol_table.setdefault(
                label,
                apply_type_override(infer_storage_symbol(label, parse_storage_size(rest), module), type_overrides),
            )
            idx += 1
            continue

        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx, branch_targets)
        if standalone_data is not None:
            if is_omitted_symbol(standalone_data.name, type_overrides):
                idx = next_idx
                continue
            directive_lower = standalone_data.directive.lower()
            if directive_lower in {".word", ".intdata", ".float"}:
                values: list[str] = []
                if directive_lower in {".intdata", ".float"}:
                    values = [part.strip() for part in standalone_data.rest.split(",") if part.strip()]
                else:
                    for asm_line in standalone_data.asm_lines[1:]:
                        asm_code, _asm_comment = split_comment(asm_line)
                        stripped = asm_code.strip()
                        if not stripped.lower().startswith(".word"):
                            continue
                        operands = stripped[5:].strip()
                        if operands:
                            values.extend(part.strip() for part in operands.split(",") if part.strip())
                if not values:
                    values = ["0"]
                symbol_table.setdefault(
                    standalone_data.name,
                    apply_type_override(
                        infer_numeric_symbol(
                            NumericVariable(
                                name=standalone_data.name,
                                directive=directive_lower,
                                values=values,
                                asm_lines=filter_renderable_asm_lines(standalone_data.asm_lines),
                            ),
                            module,
                            symbol_table,
                        ),
                        type_overrides,
                    ),
                )
            elif directive_lower == ".string":
                c_string = parse_simple_string_operand(standalone_data.rest)
                if c_string is not None:
                    symbol_table.setdefault(
                        standalone_data.name,
                        apply_type_override(infer_string_symbol(standalone_data.name, module), type_overrides),
                    )
            elif directive_lower in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
                symbol_table.setdefault(
                    standalone_data.name,
                    apply_type_override(
                        infer_storage_symbol(standalone_data.name, parse_storage_size(standalone_data.rest), module),
                        type_overrides,
                    ),
                )
            idx = next_idx
            continue

        if not is_flush_left(raw):
            idx += 1
            continue

        sptr_match = parse_sptr_label(raw)
        if sptr_match is not None:
            label, _c_string = sptr_match
            if is_omitted_symbol(label, type_overrides):
                idx += 1
                continue
            symbol_table.setdefault(label, apply_type_override(infer_string_symbol(label, module), type_overrides))
            idx += 1
            continue

        data_match = DATA_LABEL_RE.match(raw)
        if not data_match:
            idx += 1
            continue

        label, directive, rest = data_match.groups()
        if is_omitted_symbol(label, type_overrides):
            idx += 1
            continue
        directive_lower = directive.lower()
        if directive_lower in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
            symbol_table.setdefault(
                label,
                apply_type_override(infer_storage_symbol(label, parse_storage_size(rest), module), type_overrides),
            )
            idx += 1
            continue
        if directive_lower == ".string":
            c_string = parse_simple_string_operand(rest)
            if c_string is not None:
                symbol_table.setdefault(label, apply_type_override(infer_string_symbol(label, module), type_overrides))
            idx += 1
            continue
        if directive_lower == ".word":
            asm_lines = [f"{label}\t.word{rest}"]
            values: list[str] = []
            first_row = parse_numeric_data_line(asm_lines[0], label)
            if first_row is not None:
                row_values, _row_comment = first_row
                values.extend(row_values)

            next_idx = idx + 1
            while next_idx < len(lines):
                next_raw = lines[next_idx]
                if next_idx in function_line_indexes or is_comment_or_blank(next_raw) or is_flush_left(next_raw):
                    break
                parsed_row = parse_numeric_data_line(next_raw)
                if parsed_row is None:
                    break
                asm_lines.append(next_raw.rstrip())
                row_values, _row_comment = parsed_row
                values.extend(row_values)
                next_idx += 1

            if not values:
                values = ["0"]
            symbol_table.setdefault(
                label,
                apply_type_override(
                    infer_word_symbol(WordVariable(name=label, values=values, asm_lines=asm_lines), module, symbol_table),
                    type_overrides,
                ),
            )
            idx = next_idx
            continue
        if directive_lower == ".float":
            asm_lines = [f"{label}\t.float{rest}"]
            values: list[str] = []
            first_row = parse_numeric_data_line(asm_lines[0], label)
            if first_row is not None:
                row_values, _row_comment = first_row
                values.extend(row_values)

            next_idx = idx + 1
            while next_idx < len(lines):
                next_raw = lines[next_idx]
                if next_idx in function_line_indexes or is_comment_or_blank(next_raw) or is_flush_left(next_raw):
                    break
                parsed_row = parse_numeric_data_line(next_raw)
                if parsed_row is None:
                    break
                asm_lines.append(next_raw.rstrip())
                row_values, _row_comment = parsed_row
                values.extend(row_values)
                next_idx += 1

            if not values:
                values = ["0"]
            symbol_table.setdefault(
                label,
                apply_type_override(
                    infer_numeric_symbol(
                        NumericVariable(name=label, directive=".float", values=values, asm_lines=asm_lines),
                        module,
                        symbol_table,
                    ),
                    type_overrides,
                ),
            )
            idx = next_idx
            continue

        idx += 1

    for name in force_function_names:
        symbol_table.setdefault(name, SymbolInfo(name=name, kind="function", module=module))

    return symbol_table


def collect_existing_macro_names(include_dir: Path) -> set[str]:
    names: set[str] = set()
    if not include_dir.exists():
        return names
    for header_path in sorted(include_dir.glob("*.h")):
        if header_path.name == "discovered_labels.h":
            continue
        for raw in header_path.read_text(errors="ignore").splitlines():
            match = CPP_DEFINE_RE.match(raw)
            if match:
                names.add(match.group(1))
    return names


def main() -> int:
    root = Path(__file__).resolve().parents[2]
    asm_dir = root / "asm"
    out_dir = root / "src" / "game"
    include_dir = root / "src" / "game"
    address_map = parse_address_map(root / "tools" / "ida" / "address.map")
    define_entries = parse_discovered_defines_file(root / "tools" / "ida" / "discovered_defines.txt")
    instruction_addresses_by_module = parse_instruction_addresses_file(
        root / "tools" / "ida" / "log" / "romlst_instruction_addresses.tsv"
    )
    type_overrides = parse_type_overrides_file(root / "tools" / "port" / "type-overrides.txt")
    label_types = parse_romlst_label_types(root / "tools" / "ida" / "log" / "romlst_labels.tsv")
    out_dir.mkdir(parents=True, exist_ok=True)
    include_dir.mkdir(parents=True, exist_ok=True)
    log_dir = root / "tools" / "port" / "log"
    log_dir.mkdir(parents=True, exist_ok=True)
    discovered_define_entries = sorted(
        list(define_entries.values()),
        key=lambda item: item.name.upper(),
    )
    discovered_define_names = {entry.name for entry in discovered_define_entries}
    if discovered_define_entries:
        (include_dir / "discovered_defines.h").write_text(render_discovered_defines_header(discovered_define_entries))
    global_source_symbols: set[str] = set()
    source_label_names: set[str] = set()
    for src_path in sorted(asm_dir.glob("*.ASM")):
        lines = src_path.read_text(errors="ignore").splitlines()
        global_source_symbols.update(collect_module_symbol_table(src_path, address_map, type_overrides))
        source_label_names.update(collect_source_label_names(lines))
    existing_macro_names = collect_existing_macro_names(include_dir)
    blocked_discovered_label_names = (
        global_source_symbols
        | source_label_names
        | existing_macro_names
        | discovered_define_names
        | {"NULL"}
    )

    storage_defines_by_module: dict[str, list[StorageVariable]] = {}
    owner_headers: dict[str, str] = {}
    skipped_data_symbols: set[str] = set()
    discovered_label_names: set[str] = set()
    for src_path in sorted(asm_dir.glob("*.ASM")):
        defines = collect_module_storage_defines(src_path, address_map, type_overrides)
        if not defines:
            continue
        storage_defines_by_module[src_path.stem.upper()] = defines
        header_name = storage_header_name(src_path.stem, include_dir)
        for entry in defines:
            owner_headers[entry.name] = header_name
        (include_dir / header_name).write_text(render_storage_header(src_path, defines))

    for src_path in sorted(asm_dir.glob("*.ASM")):
        lines = src_path.read_text(errors="ignore").splitlines()
        missing_data_symbols = collect_referenced_data_symbols(lines, label_types) - collect_defined_data_symbols(lines)
        skipped_data_symbols.update(missing_data_symbols)
        mapped_missing_data_symbols = {
            name
            for name in missing_data_symbols
            if name in address_map and name not in blocked_discovered_label_names
        }
        discovered_label_names.update(mapped_missing_data_symbols)
        discovered_header_needed = bool(collect_referenced_define_symbols(lines, discovered_define_names))
        discovered_labels_needed = bool(mapped_missing_data_symbols)
        out_path = out_dir / (src_path.stem.lower() + ".c")
        own_header = owner_headers.get(next(iter([d.name for d in storage_defines_by_module.get(src_path.stem.upper(), [])]), ""), None)
        if src_path.stem.upper() in storage_defines_by_module:
            own_header = storage_header_name(src_path.stem, include_dir)
        else:
            own_header = None
        out_path.write_text(
            render_module(
                src_path,
                address_map,
                owner_headers,
                own_header,
                discovered_header_needed,
                type_overrides,
                discovered_labels_needed,
                instruction_addresses_by_module.get(src_path.stem.upper()),
            )
        )

    discovered_label_entries = [
        LabelEntry(name=name, addr=address_map[name])
        for name in sorted(discovered_label_names, key=str.upper)
    ]
    if discovered_label_entries:
        (include_dir / "discovered_labels.h").write_text(render_discovered_labels_header(discovered_label_entries))

    skipped_log = log_dir / "skipped_data_symbols.txt"
    skipped_rows = sorted(skipped_data_symbols)
    skipped_log.write_text("".join(f"{name}\n" for name in skipped_rows))
    print(f"generated {len(list(asm_dir.glob('*.ASM')))} C modules in {out_dir.relative_to(root)}")
    print(f"logged {len(skipped_rows)} skipped data symbols to {skipped_log.relative_to(root)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""Generate first-pass C scaffolds from asm modules."""

from __future__ import annotations

import ast
from collections import ChainMap
import re
import sys
from dataclasses import dataclass, field
from functools import lru_cache
from pathlib import Path
from typing import Iterable

IDA_DIR = Path(__file__).resolve().parents[1] / "ida"
if str(IDA_DIR) not in sys.path:
    sys.path.append(str(IDA_DIR))

import shared_lib as ccm


INCLUDE_RE = re.compile(r"^\s*\.include\s+([A-Za-z0-9_.]+)\s*$", re.IGNORECASE)
SET_RE = re.compile(r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s*\.set\s*(.*?)\s*$", re.IGNORECASE)
GLOBL_RE = re.compile(r"^\s*\.globl\s+(.*?)\s*$", re.IGNORECASE)
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
BIN_SUFFIX_RE = re.compile(r"\b([01]+)[Bb]\b")
SIMPLE_STRING_RE = re.compile(r'^\s*("([^"\\]|\\.)*")\s*(?:,\s*0+\s*)?$')
SPTR_LABEL_RE = re.compile(r'^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s+SPTR\b(.*)$', re.IGNORECASE)
SPTR_ROW_RE = re.compile(r'^\s*SPTR\b(.*)$', re.IGNORECASE)
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
RENDER_OVERRIDE_START_RE = re.compile(r"^\s*%%\s+([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s*$")
RENDER_OVERRIDE_END_RE = re.compile(r"^\s*%%\s+END\s*$", re.IGNORECASE)
NOEDIT_SENTINEL = "// NOEDIT"
SKIPPED_ASM_MODULES = {
    "H2HOBJ",
    "OBJECTS",
}

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
}

REGISTER_NAMES = {
    "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
    "AR0", "AR1", "AR2", "AR3", "AR4", "AR5", "AR6", "AR7",
    "RC", "RS", "RE", "BK", "DP", "SP", "ST", "IE", "IF", "IR0", "IR1", "IOF",
}

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
class EquSetEntry:
    name: str
    expr: str
    comment: str
    asm_line: str


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
    inner_labels: list[tuple[str, int]] = field(default_factory=list)


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
    rename_to: str | None = None
    omit: bool = False
    force_function: bool = False


@dataclass
class RenderBlock:
    kind: str
    lines: list[str] = field(default_factory=list)


def output_symbol_name(name: str, type_overrides: dict[str, TypeOverride] | None = None) -> str:
    if type_overrides is not None:
        override = type_overrides.get(name)
        if override is not None and override.rename_to:
            return override.rename_to
    return name


def with_discovered_label_overrides(
    type_overrides: dict[str, TypeOverride] | None,
    discovered_label_names: set[str] | None,
) -> dict[str, TypeOverride] | None:
    if not discovered_label_names:
        return type_overrides
    merged = dict(type_overrides or {})
    for name in discovered_label_names:
        override = merged.get(name)
        if override is None:
            merged[name] = TypeOverride(name=name, c_type="", rename_to=f"{name}_ROM")
            continue
        if not override.rename_to:
            merged[name] = TypeOverride(
                name=override.name,
                c_type=override.c_type,
                array_expr=override.array_expr,
                rename_to=f"{name}_ROM",
                omit=override.omit,
                force_function=override.force_function,
            )
    return merged


def render_identifier(name: str, type_overrides: dict[str, TypeOverride] | None = None) -> str:
    return sanitize_identifier(output_symbol_name(name, type_overrides))


def build_symbol_rename_map(type_overrides: dict[str, TypeOverride] | None) -> dict[str, str]:
    if type_overrides is None:
        return {}
    return {
        name: override.rename_to
        for name, override in type_overrides.items()
        if override.rename_to
    }


def file_has_noedit_guard(path: Path) -> bool:
    if not path.exists():
        return False
    with path.open("r", errors="ignore") as handle:
        first_line = handle.readline()
    return first_line.removeprefix("\ufeff").strip() == NOEDIT_SENTINEL


def resolve_generated_output_path(target_path: Path, generated_dir: Path) -> Path:
    if file_has_noedit_guard(target_path):
        return generated_dir / target_path.name
    return target_path


def eval_constant_expr(expr: str) -> int | None:
    try:
        parsed = ast.parse(expr, mode="eval")
    except SyntaxError:
        return None

    def _eval(node: ast.AST) -> int:
        if isinstance(node, ast.Expression):
            return _eval(node.body)
        if isinstance(node, ast.Constant) and isinstance(node.value, int):
            return node.value
        if isinstance(node, ast.UnaryOp) and isinstance(node.op, (ast.UAdd, ast.USub, ast.Invert)):
            value = _eval(node.operand)
            if isinstance(node.op, ast.UAdd):
                return value
            if isinstance(node.op, ast.USub):
                return -value
            return ~value
        if isinstance(node, ast.BinOp) and isinstance(
            node.op,
            (ast.Add, ast.Sub, ast.Mult, ast.Div, ast.FloorDiv, ast.LShift, ast.RShift, ast.BitOr, ast.BitAnd, ast.BitXor, ast.Mod),
        ):
            left = _eval(node.left)
            right = _eval(node.right)
            if isinstance(node.op, ast.Add):
                return left + right
            if isinstance(node.op, ast.Sub):
                return left - right
            if isinstance(node.op, ast.Mult):
                return left * right
            if isinstance(node.op, (ast.Div, ast.FloorDiv)):
                return left // right
            if isinstance(node.op, ast.LShift):
                return left << right
            if isinstance(node.op, ast.RShift):
                return left >> right
            if isinstance(node.op, ast.BitOr):
                return left | right
            if isinstance(node.op, ast.BitAnd):
                return left & right
            if isinstance(node.op, ast.BitXor):
                return left ^ right
            return left % right
        raise ValueError("unsupported constant expression")

    try:
        return _eval(parsed)
    except (ValueError, ZeroDivisionError):
        return None


def try_fold_constant_expr(
    expr: str,
    symbol_table: dict[str, SymbolInfo] | None = None,
    global_symbol_table: dict[str, SymbolInfo] | None = None,
) -> str:
    const_values: dict[str, str] = {}
    for table in (global_symbol_table, symbol_table):
        if table is None:
            continue
        for name, symbol in table.items():
            if symbol.expr is not None and eval_constant_expr(symbol.expr) is not None:
                const_values[name] = symbol.expr

    if not const_values:
        return expr

    replaced = BRANCH_TARGET_RE.sub(lambda m: const_values.get(m.group(1), m.group(1)), expr)
    if replaced != expr and BRANCH_TARGET_RE.search(replaced):
        return replaced
    if BRANCH_TARGET_RE.search(replaced):
        return expr
    value = eval_constant_expr(replaced)
    if value is None:
        return replaced
    return str(value)


def render_set_expr(
    expr: str,
    type_overrides: dict[str, TypeOverride] | None = None,
    symbol_table: dict[str, SymbolInfo] | None = None,
    global_symbol_table: dict[str, SymbolInfo] | None = None,
) -> str:
    converted = convert_expr(expr, type_overrides)
    if parse_int_token(converted) is not None:
        return converted
    return try_fold_constant_expr(converted, symbol_table, global_symbol_table)


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


def sanitize_guard_stem(name: str) -> str:
    return re.sub(r"[^A-Za-z0-9]+", "_", name).upper().strip("_")


def normalize_banner_comments(lines: list[str]) -> list[str]:
    normalized: list[str] = []
    pending_blank = False
    started = False
    for line in lines:
        if not line:
            if started:
                pending_blank = True
            continue
        if pending_blank and normalized:
            normalized.append("")
        normalized.append(line)
        pending_blank = False
        started = True
    return normalized


def normalize_emitted_lines(lines: list[str]) -> list[str]:
    normalized: list[str] = []
    blank_pending = False
    for line in lines:
        if line == "":
            if normalized:
                blank_pending = True
            continue
        if blank_pending and normalized:
            normalized.append("")
        normalized.append(line)
        blank_pending = False
    return normalized


def join_render_blocks(blocks: list[RenderBlock]) -> list[str]:
    out: list[str] = []
    first = True
    for block in blocks:
        if not block.lines:
            continue
        if not first:
            out.append("")
        out.extend(block.lines)
        first = False
    return out


def source_root_for_path(src_path: Path) -> Path:
    resolved = src_path.resolve()
    for parent in (resolved.parent, *resolved.parents):
        if (parent / "asm").is_dir():
            return parent
        if parent.name.upper() == "ASM":
            return parent.parent
    return resolved.parent


def should_skip_module(src_path: Path) -> bool:
    return src_path.stem.upper() in SKIPPED_ASM_MODULES


def iter_module_paths(asm_dir: Path, pattern: str) -> list[Path]:
    return [src_path for src_path in sorted(asm_dir.glob(pattern)) if not should_skip_module(src_path)]


@lru_cache(maxsize=32)
def collect_data_only_macro_names(root: Path) -> frozenset[str]:
    macros = ccm.parse_macros(root)
    symbols = ccm.parse_set_symbols(root)
    symbols["DEBUG"] = 0
    cache: dict[str, bool] = {}
    out: set[str] = set()
    for name in macros:
        if not ccm.macro_emits_executable(name, macros, symbols, cache, set()):
            out.add(name.upper())
    return frozenset(out)


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
    if text.lower().endswith("b") and re.fullmatch(r"[01]+[Bb]", text):
        try:
            value = int(text[:-1], 2)
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
        rename_match = re.fullmatch(
            r"(?P<name>[_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s*=\s*(?P<rename>[_A-Za-z.$?@][_A-Za-z0-9.$?@]*)",
            line,
        )
        if rename_match:
            name = rename_match.group("name")
            rename_to = rename_match.group("rename")
            existing = out.get(name)
            if existing is not None:
                existing.rename_to = rename_to
            else:
                out[name] = TypeOverride(name=name, c_type="", rename_to=rename_to)
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
        existing = out.get(name)
        rename_to = existing.rename_to if existing is not None else None
        omit = existing.omit if existing is not None else False
        force_function = existing.force_function if existing is not None else False
        out[name] = TypeOverride(
            name=name,
            c_type=c_type,
            array_expr=array_expr.strip() if array_expr is not None else None,
            rename_to=rename_to,
            omit=omit,
            force_function=force_function,
        )
    return out


def parse_render_overrides_file(overrides_path: Path) -> dict[str, list[str]]:
    out: dict[str, list[str]] = {}
    if not overrides_path.exists():
        return out

    current_name: str | None = None
    current_lines: list[str] = []
    for raw in overrides_path.read_text(errors="ignore").splitlines():
        if current_name is None:
            match = RENDER_OVERRIDE_START_RE.match(raw)
            if match is not None:
                current_name = match.group(1)
                current_lines = []
            continue

        if RENDER_OVERRIDE_END_RE.match(raw):
            while current_lines and not current_lines[-1].strip():
                current_lines.pop()
            out[current_name] = current_lines[:]
            current_name = None
            current_lines = []
            continue

        current_lines.append(raw.rstrip())

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
        return SymbolInfo(name=output_symbol_name(symbol.name, type_overrides), kind="function", module=symbol.module)
    if symbol.kind != "variable":
        return symbol
    override_declares_shape = bool(override.c_type)
    return SymbolInfo(
        name=output_symbol_name(symbol.name, type_overrides),
        kind=symbol.kind,
        module=symbol.module,
        c_type=override.c_type if override.c_type else symbol.c_type,
        array_expr=symbol.array_expr if override.array_expr is None else override.array_expr,
        expr=symbol.expr,
    )


def render_symbol_override(
    name: str,
    render_overrides: dict[str, list[str]] | None,
) -> list[str] | None:
    if render_overrides is None:
        return None
    override_lines = render_overrides.get(name)
    if override_lines is None:
        return None
    return override_lines[:]


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


def storage_array_expr_for_symbol(
    name: str,
    size_expr: str,
    type_overrides: dict[str, TypeOverride] | None = None,
) -> str | None:
    override = type_overrides.get(name) if type_overrides is not None else None
    if override is not None and override.c_type and override.array_expr is None:
        return None
    expr = convert_expr(size_expr, type_overrides).strip()
    if expr in {"", "0", "1"}:
        return None
    return expr


def variable_declaration(name: str, c_type: str, array_expr: str | None = None, is_extern: bool = False) -> str:
    ident = sanitize_identifier(name)
    prefix = "extern " if is_extern else ""
    if "(*)" in c_type:
        if array_expr is None or array_expr.strip() in {"0", "1"}:
            return f"{prefix}{c_type.replace('(*)', f'(*{ident})')};"
        if array_expr.strip() == "":
            return f"{prefix}{c_type.replace('(*)', f'(*{ident}[])')};"
        if is_extern:
            return f"{prefix}{c_type.replace('(*)', f'(*{ident}[])')};"
        return f"{prefix}{c_type.replace('(*)', f'(*{ident}[{array_expr.strip()}])')};"
    sep = "" if c_type.endswith("*") else " "
    if array_expr is None or array_expr.strip() in {"0", "1"}:
        return f"{prefix}{c_type}{sep}{ident};"
    if array_expr.strip() == "":
        return f"{prefix}{c_type}{sep}{ident}[];"
    if is_extern:
        return f"{prefix}{c_type}{sep}{ident}[];"
    return f"{prefix}{c_type}{sep}{ident}[{array_expr.strip()}];"


def variable_definition_prefix(
    name: str,
    c_type: str,
    array_expr: str | None = None,
    omit_array_size: bool = False,
) -> str:
    ident = sanitize_identifier(name)
    if "(*)" in c_type:
        if array_expr is None or array_expr.strip() in {"0", "1"}:
            return c_type.replace("(*)", f"(*{ident})")
        if array_expr.strip() == "":
            return c_type.replace("(*)", f"(*{ident}[])")
        if omit_array_size:
            return c_type.replace("(*)", f"(*{ident}[])")
        return c_type.replace("(*)", f"(*{ident}[{array_expr.strip()}])")
    sep = "" if c_type.endswith("*") else " "
    if array_expr is None or array_expr.strip() in {"0", "1"}:
        return f"{c_type}{sep}{ident}"
    if array_expr.strip() == "":
        return f"{c_type}{sep}{ident}[]"
    if omit_array_size:
        return f"{c_type}{sep}{ident}[]"
    return f"{c_type}{sep}{ident}[{array_expr.strip()}]"


def is_header_exported_symbol(name: str, exported_header_names: set[str] | None) -> bool:
    return name in (exported_header_names or set())


def storage_declaration(name: str, size_expr: str, is_extern: bool = False, type_overrides: dict[str, TypeOverride] | None = None) -> str:
    array_expr = storage_array_expr_for_symbol(name, size_expr, type_overrides)
    c_type = "int"
    if type_overrides is not None:
        override = type_overrides.get(name)
        if override is not None and override.c_type:
            c_type = override.c_type
    return variable_declaration(name, c_type, array_expr, is_extern=is_extern)


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
    for idx, raw in enumerate(lines):
        if STRUCT_START_RE.match(raw):
            if not any(STRUCT_END_RE.match(candidate) for candidate in lines[idx + 1:]):
                out.append(raw)
                continue
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


def strip_noncode_definition_blocks_with_line_numbers(lines: list[str]) -> tuple[list[str], list[int]]:
    numbered = list(enumerate(lines, start=1))

    def strip_macro_blocks(entries: list[tuple[int, str]]) -> list[tuple[int, str]]:
        out: list[tuple[int, str]] = []
        macro_depth = 0
        for line_no, raw in entries:
            if MACRO_START_RE.match(raw):
                macro_depth += 1
                continue
            if MACRO_END_RE.match(raw):
                if macro_depth > 0:
                    macro_depth -= 1
                continue
            if macro_depth > 0:
                continue
            out.append((line_no, raw))
        return out

    def strip_struct_blocks(entries: list[tuple[int, str]]) -> list[tuple[int, str]]:
        out: list[tuple[int, str]] = []
        struct_depth = 0
        for idx, (line_no, raw) in enumerate(entries):
            if STRUCT_START_RE.match(raw):
                if not any(STRUCT_END_RE.match(candidate_raw) for _candidate_line_no, candidate_raw in entries[idx + 1:]):
                    out.append((line_no, raw))
                    continue
                struct_depth += 1
                continue
            if STRUCT_END_RE.match(raw):
                if struct_depth > 0:
                    struct_depth -= 1
                continue
            if struct_depth > 0:
                continue
            out.append((line_no, raw))
        return out

    stripped = strip_struct_blocks(strip_macro_blocks(numbered))
    return [raw for _line_no, raw in stripped], [line_no for line_no, _raw in stripped]


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


def render_storage_header_lines(
    src_path: Path,
    defines: list[StorageVariable],
    type_overrides: dict[str, TypeOverride] | None = None,
    exported_functions: list[str] | None = None,
) -> list[str]:
    out: list[str] = []
    out.append(f"// {src_path.name}")
    for entry in defines:
        out.append(f"// asm: {entry.asm_line}")
        out.append(storage_declaration(entry.name, entry.size_expr, is_extern=True, type_overrides=type_overrides))
    for name in exported_functions or []:
        out.append(f"void {sanitize_identifier(name)}(void);")
    return out


def render_storage_header(
    src_path: Path,
    defines: list[StorageVariable],
    type_overrides: dict[str, TypeOverride] | None = None,
    exported_functions: list[str] | None = None,
) -> str:
    guard = sanitize_identifier(src_path.stem).upper() + "_H"
    out: list[str] = []
    out.append(f"#ifndef {guard}")
    out.append(f"#define {guard}")
    out.append("")
    out.append('#include "port.h"')
    out.append("")
    out.append(f"/* Generated from asm/{src_path.name}. */")
    out.append("")
    out.extend(render_storage_header_lines(src_path, defines, type_overrides, exported_functions))
    out.append("")
    out.append(f"#endif /* {guard} */")
    out.append("")
    return "\n".join(out)


def storage_header_name(module: str, _include_dir: Path) -> str:
    return module.lower() + ".h"


def collect_module_globl_function_names(
    lines: list[str],
    symbol_table: dict[str, SymbolInfo] | None = None,
) -> list[str]:
    names: list[str] = []
    seen: set[str] = set()
    for raw in lines:
        code, _comment = split_comment(raw)
        match = GLOBL_RE.match(code)
        if match is None:
            continue
        for name in [part.strip() for part in match.group(1).split(",") if part.strip()]:
            if name in seen:
                continue
            symbol = symbol_table.get(name) if symbol_table is not None else None
            if symbol is None or symbol.kind != "function":
                continue
            names.append(name)
            seen.add(name)
    return names


def merge_header_bodies(existing: str, rendered_storage: str) -> str:
    endif_idx = existing.rfind("#endif")
    if endif_idx == -1:
        return rendered_storage
    storage_lines = rendered_storage.splitlines()
    body_start = 0
    for idx, line in enumerate(storage_lines):
        if line.startswith("/* Generated from "):
            body_start = idx + 1
            break
    while body_start < len(storage_lines) and storage_lines[body_start].startswith("#"):
        body_start += 1
    while body_start < len(storage_lines) and not storage_lines[body_start].strip():
        body_start += 1
    body_end = len(storage_lines)
    while body_end > body_start and not storage_lines[body_end - 1].strip():
        body_end -= 1
    if body_end > body_start and storage_lines[body_end - 1].startswith("#endif"):
        body_end -= 1
    storage_body = "\n".join(storage_lines[body_start:body_end]).rstrip()
    if not storage_body:
        return existing
    prefix = existing[:endif_idx].rstrip()
    suffix = existing[endif_idx:]
    return f"{prefix}\n\n{storage_body}\n\n{suffix.lstrip()}"


HEADER_DEFINE_RE = re.compile(r"^\s*#define\s+([A-Za-z_][A-Za-z0-9_]*)\b")
HEADER_EXTERN_RE = re.compile(r"^\s*extern\b.*?\b([A-Za-z_][A-Za-z0-9_]*)\s*(?:\[[^\]]*\])?;")
HEADER_VOID_PROTO_RE = re.compile(r"^\s*void\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(void\)\s*;")


def collect_declared_header_names(existing: str) -> set[str]:
    names: set[str] = set()
    for raw in existing.splitlines():
        for regex in (HEADER_DEFINE_RE, HEADER_EXTERN_RE, HEADER_VOID_PROTO_RE):
            match = regex.match(raw)
            if match:
                names.add(match.group(1))
                break
    return names


def collect_defined_header_names(existing: str) -> set[str]:
    names: set[str] = set()
    for raw in existing.splitlines():
        match = HEADER_DEFINE_RE.match(raw)
        if match:
            names.add(match.group(1))
    return names


def collect_exported_header_names(include_dir: Path) -> set[str]:
    names: set[str] = set()
    if not include_dir.exists():
        return names
    for header_path in sorted(include_dir.glob("*.h")):
        for raw in header_path.read_text(errors="ignore").splitlines():
            for regex in (HEADER_DEFINE_RE, HEADER_EXTERN_RE, HEADER_VOID_PROTO_RE):
                match = regex.match(raw)
                if match:
                    names.add(match.group(1))
                    break
    return names


def merge_storage_into_header(existing: str, src_path: Path, defines: list[StorageVariable]) -> str:
    marker = f"// {src_path.name}"
    end_guard = f"#endif /* {sanitize_identifier(src_path.stem).upper()}_H */"
    lines = existing.splitlines()
    try:
        end_idx = next(i for i, line in enumerate(lines) if line.strip() == end_guard)
    except StopIteration:
        return render_storage_header(src_path, defines)

    body = lines[:end_idx]
    if marker in body:
        body = body[: body.index(marker)]

    equ_marker = f"// {src_path.stem.upper()}.EQU"
    if f"Generated from asm/{src_path.stem.upper()}.EQU." in existing and equ_marker not in body:
        insert_at = 0
        for idx, line in enumerate(body):
            insert_at = idx + 1
            if line.startswith("/* Generated from asm/"):
                break
        while insert_at < len(body) and body[insert_at] == "":
            insert_at += 1
        body[insert_at:insert_at] = ["", equ_marker]

    existing_names = collect_declared_header_names("\n".join(body))
    filtered_defines = [entry for entry in defines if entry.name not in existing_names]
    if not filtered_defines:
        body.append("")
        body.append(end_guard)
        body.append("")
        return "\n".join(body)

    if body and body[-1] != "":
        body.append("")
    body.extend(render_storage_header_lines(src_path, filtered_defines))
    body.append("")
    body.append(end_guard)
    body.append("")
    return "\n".join(body)


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
            module = name[:-4].upper()
            if module in SKIPPED_ASM_MODULES:
                continue
            headers.append(name[:-4].lower() + ".h")
    return headers


def convert_expr(expr: str, type_overrides: dict[str, TypeOverride] | None = None) -> str:
    expr = expr.strip()
    expr = HEX_SUFFIX_RE.sub(lambda m: f"0x{m.group(1)}", expr)
    expr = BIN_SUFFIX_RE.sub(lambda m: str(int(m.group(1), 2)), expr)
    rename_map = build_symbol_rename_map(type_overrides)
    if rename_map:
        expr = re.sub(
            BRANCH_TARGET_RE,
            lambda m: output_symbol_name(m.group(1), type_overrides),
            expr,
        )
    expr = re.sub(r"(?<![<>!=])=(?!=)", "==", expr)
    expr = re.sub(r"\s+", " ", expr)
    return expr


def format_float_expr(expr: str, type_overrides: dict[str, TypeOverride] | None = None) -> str:
    rendered = convert_expr(expr, type_overrides)
    parsed = parse_int_token(rendered)
    if parsed is not None:
        return f"{parsed}.0f"
    if rendered.lower().endswith("f"):
        return rendered
    if re.search(r"[.eE]", rendered):
        return rendered if rendered.lower().endswith("f") else f"{rendered}f"
    return rendered


def render_conditional_line(raw: str, type_overrides: dict[str, TypeOverride] | None = None) -> str | None:
    match = IF_RE.match(raw)
    if match:
        return f"#if {convert_expr(match.group(1), type_overrides)}"
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


def strip_asm_comment_marker(raw: str) -> str | None:
    stripped = raw.strip()
    if not stripped or stripped[0] not in {"*", ";"}:
        return None
    return stripped[1:].lstrip()


def comment_looks_like_disabled_data(raw: str) -> bool:
    body = strip_asm_comment_marker(raw)
    if body is None or not body:
        return False
    if parse_numeric_directive(body) is not None:
        return True
    if DATA_LABEL_RE.match(body) is not None:
        return True
    return STANDALONE_DATA_DIRECTIVE_RE.match(body) is not None


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


def split_string_operands(rest: str) -> list[str]:
    tokens: list[str] = []
    current: list[str] = []
    in_string = False
    escape = False
    for ch in rest:
        if in_string:
            current.append(ch)
            if escape:
                escape = False
                continue
            if ch == "\\":
                escape = True
                continue
            if ch == '"':
                in_string = False
            continue
        if ch == '"':
            in_string = True
            current.append(ch)
            continue
        if ch == ",":
            token = "".join(current).strip()
            if token:
                tokens.append(token)
            current = []
            continue
        current.append(ch)
    token = "".join(current).strip()
    if token:
        tokens.append(token)
    return tokens


def render_c_string_literal_from_bytes(byte_values: list[int]) -> str:
    out = ['"']
    for value in byte_values:
        if value == 0x0D:
            out.append(r"\n")
        elif value == 0x0A:
            out.append(r"\n")
        elif value == 0x09:
            out.append(r"\t")
        elif value == 0x5C:
            out.append(r"\\")
        elif value == 0x22:
            out.append(r"\"")
        elif 0x20 <= value <= 0x7E:
            out.append(chr(value))
        else:
            out.append(f"\\x{value:02X}")
    out.append('"')
    return "".join(out)


def decode_string_token(token: str) -> list[int] | None:
    if not token:
        return None
    if token[0] not in {"'", '"'} or token[-1] != token[0]:
        return None
    try:
        decoded = ast.literal_eval(token)
    except (SyntaxError, ValueError):
        return None
    if not isinstance(decoded, str):
        return None
    return [ord(ch) for ch in decoded]


def parse_string_blob_operand(operand_text: str) -> str | None:
    operands, _comment = split_comment(operand_text.strip())
    tokens = split_string_operands(operands)
    if not tokens:
        return None

    saw_quoted = False
    byte_values: list[int] = []
    for token in tokens:
        decoded_bytes = decode_string_token(token)
        if decoded_bytes is not None:
            saw_quoted = True
            byte_values.extend(decoded_bytes)
            continue

        value = parse_int_token(token)
        if value is None or not 0 <= value <= 0xFF:
            return None
        byte_values.append(value)

    if not saw_quoted:
        return None
    if byte_values and byte_values[-1] == 0:
        byte_values.pop()
    return render_c_string_literal_from_bytes(byte_values)


def pack_string_directive(operand_text: str) -> list[str] | None:
    operands, _comment = split_comment(operand_text.strip())
    if parse_simple_string_operand(operands) is not None:
        return None

    byte_values: list[int] = []
    for token in split_string_operands(operands):
        decoded_bytes = decode_string_token(token)
        if decoded_bytes is not None:
            byte_values.extend(decoded_bytes)
            continue
        value = parse_int_token(token)
        if value is None or not 0 <= value <= 0xFF:
            return None
        byte_values.append(value)

    if not byte_values:
        return []

    packed_words: list[str] = []
    for idx in range(0, len(byte_values), 4):
        word = 0
        for shift, value in enumerate(byte_values[idx:idx + 4]):
            word |= value << (shift * 8)
        packed_words.append(f"0x{word:08X}")
    return packed_words


def pack_string_directive_any(operand_text: str) -> list[str] | None:
    operands, _comment = split_comment(operand_text.strip())
    byte_values: list[int] = []
    for token in split_string_operands(operands):
        decoded_bytes = decode_string_token(token)
        if decoded_bytes is not None:
            byte_values.extend(decoded_bytes)
            continue
        value = parse_int_token(token)
        if value is None or not 0 <= value <= 0xFF:
            return None
        byte_values.append(value)

    if not byte_values:
        return []

    packed_words: list[str] = []
    for idx in range(0, len(byte_values), 4):
        word = 0
        for shift, value in enumerate(byte_values[idx:idx + 4]):
            word |= value << (shift * 8)
        packed_words.append(f"0x{word:08X}")
    return packed_words


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


def parse_sptr_row(raw: str) -> str | None:
    code, _comment = split_comment(raw)
    match = SPTR_ROW_RE.match(code)
    if not match:
        return None
    return parse_simple_string_operand(match.group(1))


def parse_numeric_directive(stripped: str) -> tuple[str, list[str]] | None:
    lower = stripped.lower()
    if lower.startswith(".word"):
        operands = stripped[5:].strip()
        return ".word", [part.strip() for part in operands.split(",") if part.strip()] if operands else []
    if lower.startswith(".float"):
        operands = stripped[6:].strip()
        return ".float", [part.strip() for part in operands.split(",") if part.strip()] if operands else []
    if lower.startswith(".string"):
        operands = stripped[7:].strip()
        packed_words = pack_string_directive(operands)
        if packed_words is not None:
            return ".intdata", packed_words
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


def asm_lines_form_string_blob(asm_lines: list[str]) -> bool:
    saw_string = False
    for raw in asm_lines:
        code, _comment = split_comment(raw)
        stripped = code.strip().lower()
        if not stripped:
            continue
        if ".word" in stripped or ".float" in stripped:
            return False
        if ".string" in stripped:
            saw_string = True
    return saw_string


def next_significant_line_index(lines: list[str], start_idx: int) -> int | None:
    idx = start_idx
    while idx < len(lines):
        if not is_comment_or_blank(lines[idx]):
            return idx
        idx += 1
    return None


def line_starts_data_only_macro(raw: str, data_only_macros: frozenset[str] | None) -> bool:
    if not data_only_macros:
        return False
    code, _comment = split_comment(raw)
    if not code.strip():
        return False
    op_idx, toks = ccm.split_label_and_tokens(code)
    if not toks or op_idx >= len(toks):
        return False
    return toks[op_idx].upper() in data_only_macros


def expand_data_only_macro_invocation(
    raw: str,
    data_only_macros: frozenset[str] | None,
    macros: dict[str, ccm.MacroDef] | None,
    symbols: dict[str, int] | None,
) -> list[str] | None:
    if not data_only_macros or not macros or symbols is None:
        return None

    code, comment = split_comment(raw)
    if not code.strip():
        return None

    op_idx, toks = ccm.split_label_and_tokens(code)
    if not toks or op_idx >= len(toks):
        return None

    op = toks[op_idx].upper()
    if op not in data_only_macros or op not in macros:
        return None

    expander = ccm.MacroExpander(macros, symbols)
    args = expander.parse_invocation_args(op_idx, toks)
    expanded = ccm.iter_active_lines(expander.expand(op, args), symbols)

    out: list[str] = []
    for expanded_raw in expanded:
        nested = expand_data_only_macro_invocation(expanded_raw, data_only_macros, macros, symbols)
        if nested is not None:
            out.extend(nested)
            continue
        out.append(expanded_raw.rstrip())

    if comment:
        for idx, line in enumerate(out):
            if not line.strip() or render_asm_comment(line) is not None:
                continue
            out[idx] = f"{line.rstrip()}\t{comment}"
            break

    return out


def is_top_level_data_line(raw: str, data_only_macros: frozenset[str] | None = None) -> bool:
    code, _comment = split_comment(raw)
    if STANDALONE_STORAGE_FULL_RE.match(code):
        return True
    if line_starts_data_only_macro(raw, data_only_macros):
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
    return instruction_terminates_function(code)


def instruction_terminates_function(code: str) -> bool:
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


def top_level_data_belongs_to_current_function(
    lines: list[str],
    start_idx: int,
    branch_targets: set[str] | None = None,
) -> bool:
    if branch_targets is None:
        branch_targets = set()
    prev_exec_idx: int | None = None

    scan_idx = start_idx - 1
    while scan_idx >= 0:
        scan_raw = lines[scan_idx]
        if is_comment_or_blank(scan_raw):
            scan_idx -= 1
            continue
        scan_code, _scan_comment = split_comment(scan_raw)
        scan_stripped = scan_code.strip()
        if scan_stripped.lower().startswith((".data", ".text", ".bss", ".usect", ".sect", "romdata", ".globl", ".line", ".sym", ".if", ".else", ".endif")):
            scan_idx -= 1
            continue
        if parse_numeric_directive(scan_stripped) is not None:
            scan_idx -= 1
            continue
        if is_top_level_data_line(scan_raw):
            scan_idx -= 1
            continue
        if instruction_text_for_top_level_line(scan_raw) is not None:
            prev_exec_idx = scan_idx
            break
        scan_idx -= 1

    prev_exec_ends = prev_exec_idx is not None and line_ends_function(lines[prev_exec_idx])
    saw_branch_target_label = False

    scan_idx = start_idx + 1
    while scan_idx < len(lines):
        scan_raw = lines[scan_idx]
        if is_comment_or_blank(scan_raw):
            scan_idx += 1
            continue
        scan_code, _scan_comment = split_comment(scan_raw)
        scan_stripped = scan_code.strip()
        if scan_stripped.lower().startswith((".data", ".text", ".bss", ".usect", ".sect", "romdata", ".globl", ".line", ".sym", ".if", ".else", ".endif")):
            scan_idx += 1
            continue
        if parse_numeric_directive(scan_stripped) is not None:
            scan_idx += 1
            continue
        if is_top_level_data_line(scan_raw):
            scan_idx += 1
            continue
        label_match = LABEL_RE.match(scan_raw)
        if label_match:
            if label_match.group(1) in branch_targets:
                saw_branch_target_label = True
                scan_idx += 1
                continue
            return not prev_exec_ends and instruction_text_for_top_level_line(scan_raw) is not None
        inline_label_match = INLINE_LABEL_RE.match(scan_raw)
        if inline_label_match and is_flush_left(scan_raw):
            if inline_label_match.group(1) in branch_targets:
                saw_branch_target_label = True
                scan_idx += 1
                continue
            return not prev_exec_ends and instruction_text_for_top_level_line(scan_raw) is not None
        bare_label_match = BARE_LABEL_RE.match(scan_code)
        if bare_label_match and is_flush_left(scan_raw):
            if bare_label_match.group(1) in branch_targets:
                saw_branch_target_label = True
            scan_idx += 1
            continue
        if instruction_text_for_top_level_line(scan_raw) is not None:
            return saw_branch_target_label or not prev_exec_ends
        return False

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
        if inline_label_match and is_flush_left(probe_raw):
            is_instr, _code = classify_instruction_text(f"{inline_label_match.group(2)}{inline_label_match.group(3)}")
            return is_instr
        bare_label_match = BARE_LABEL_RE.match(probe_raw)
        if bare_label_match and is_flush_left(probe_raw):
            probe_idx += 1
            continue
        is_instr, _code = classify_instruction_text(probe_raw)
        return is_instr
    return False


def colon_label_has_code_body(
    lines: list[str],
    start_idx: int,
    data_only_macros: frozenset[str] | None = None,
) -> bool:
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
        if line_starts_data_only_macro(probe_raw, data_only_macros):
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
        if inline_label_match and is_flush_left(probe_raw):
            is_instr, _code = classify_instruction_text(f"{inline_label_match.group(2)}{inline_label_match.group(3)}")
            return is_instr
        bare_label_match = BARE_LABEL_RE.match(probe_raw)
        if bare_label_match and is_flush_left(probe_raw):
            probe_idx += 1
            continue
        is_instr, _code = classify_instruction_text(probe_raw)
        return is_instr

    return False


def classify_instruction_text(text: str) -> tuple[bool, str]:
    if not text.strip():
        return False, ""
    if text.lstrip().startswith("*") or text.lstrip().startswith(";"):
        return False, ""

    code, _comment = split_comment(text)
    if not code.strip():
        return False, ""

    label, remainder = split_flush_left_label(code)
    if label is not None and remainder_begins_with_operation(remainder):
        code = remainder.strip()
        if not code:
            return False, ""
    else:
        code = code.strip()

    first = code.split()[0]
    upper = first.upper()
    if first.lower() in CONTROL_KEYWORDS_LOWER:
        return False, ""

    if first.startswith("."):
        return False, ""

    if upper == "EQU":
        return False, ""

    return True, code


def remainder_begins_with_operation(text: str) -> bool:
    toks = ccm.parse_line_tokens(text)
    if not toks:
        return False
    first = toks[0]
    upper = first.upper()
    if first == "||":
        return True
    if first.startswith("."):
        return True
    if first.lower() in CONTROL_KEYWORDS_LOWER or upper == "EQU":
        return True
    if upper in REGISTER_NAMES:
        return False
    return bool(re.fullmatch(r"[A-Za-z][A-Za-z0-9.]*", first))


def split_flush_left_label(code: str) -> tuple[str | None, str]:
    if not is_flush_left(code):
        return None, code
    label_match = LABEL_RE.match(code)
    if label_match is not None:
        return label_match.group(1), label_match.group(2)

    inline_label_match = re.match(r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*)\s+(.*)$", code)
    if inline_label_match is not None:
        return inline_label_match.group(1), inline_label_match.group(2)

    bare_label_match = BARE_LABEL_RE.match(code)
    if bare_label_match is not None:
        return bare_label_match.group(1), ""
    return None, code


def function_has_executable_lines(fn: FunctionBlock) -> bool:
    for raw in fn.lines:
        label_match = LABEL_RE.match(raw)
        if label_match:
            is_instr, _code = classify_instruction_text(label_match.group(2))
            if is_instr:
                return True
            continue

        inline_label_match = INLINE_LABEL_RE.match(raw)
        if inline_label_match and is_flush_left(raw):
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
            if is_flush_left(raw) and inline_label_match:
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
    data_only_macros: frozenset[str] | None = None,
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

        standalone_data, _next_idx = collect_standalone_labeled_data(lines, idx, branch_targets, data_only_macros)
        if standalone_data is not None:
            if current is not None and (
                not function_has_executable_lines(current)
                or top_level_data_belongs_to_current_function(lines, idx, branch_targets)
            ):
                current.lines.append(raw.rstrip())
                current.raw_lines.append(raw.rstrip())
                current.line_numbers.append(idx + 1)
                current.end_index = idx
                continue
            current = None
            seen_separator = False
            continue

        label_match = LABEL_RE.match(raw)
        if label_match:
            label = label_match.group(1)
            if not colon_label_has_code_body(lines, idx, data_only_macros):
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

        if current is not None and is_top_level_data_line(raw, data_only_macros):
            if (
                not function_has_executable_lines(current)
                or top_level_data_belongs_to_current_function(lines, idx, branch_targets)
            ):
                current.lines.append(raw.rstrip())
                current.raw_lines.append(raw.rstrip())
                current.line_numbers.append(idx + 1)
                current.end_index = idx
                continue
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
            if is_flush_left(raw) and label in branch_targets and current is not None:
                current.labels.add(label)
                current.lines.append(f"{label}: {inline_label_match.group(2)}{inline_label_match.group(3)}")
                current.raw_lines.append(raw.rstrip())
                current.line_numbers.append(idx + 1)
                current.end_index = idx
                continue

        bare_label_match = BARE_LABEL_RE.match(raw)
        if (
            bare_label_match
            and is_flush_left(raw)
            and current is not None
            and bare_label_match.group(1) in branch_targets
        ):
            label = bare_label_match.group(1)
            current.labels.add(label)
            current.lines.append(raw.rstrip())
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
    if len(lines) == 1:
        return [f"// {lines[0].rstrip()}"]
    out = ["/*"]
    for raw in lines:
        out.append(raw.rstrip())
    out.append("*/")
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


def render_function(
    fn: FunctionBlock,
    instruction_addresses: dict[int, int] | None = None,
    exported_header_names: set[str] | None = None,
    exported_function_names: set[str] | None = None,
) -> list[str]:
    out: list[str] = []
    leading_comment_lines, body_lines = split_leading_comment_lines(fn.lines)
    out.extend(render_leading_comment_block(leading_comment_lines))
    body_line_numbers = fn.line_numbers[len(leading_comment_lines):]
    fn_ident = sanitize_identifier(fn.name)
    is_exported = (
        fn.name in (exported_header_names or set())
        or fn.name in (exported_function_names or set())
        or any(alias in (exported_header_names or set()) for alias in fn.aliases)
        or any(alias in (exported_function_names or set()) for alias in fn.aliases)
    )
    linkage = "" if is_exported else "static "
    out.append(f"{linkage}void {fn_ident}(void)")
    out.append("{")

    emitted_any = False
    last_executable_code: str | None = None
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
                    last_executable_code = code
                continue

            inline_label_match = INLINE_LABEL_RE.match(raw)
            if (
                inline_label_match
                and is_flush_left(raw)
                and inline_label_match.group(1) == pending_entry_labels[0]
            ):
                pending_entry_labels.pop(0)
                line_address = lookup_instruction_address_for_line(raw, line_no, body_line_numbers, instruction_addresses)
                raw_code = f"{inline_label_match.group(2)}{inline_label_match.group(3)}".rstrip()
                is_instr, code = classify_instruction_text(raw_code)
                if is_instr:
                    if line_address is None:
                        out.append(f"    // asm: {raw_code}")
                    else:
                        out.append(f"    // asm {line_address:08X}: {raw_code}")
                    emitted_any = True
                    last_executable_code = code
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
            if label_match.group(1) in fn.aliases:
                continue
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
                last_executable_code = code
            continue

        inline_label_match = INLINE_LABEL_RE.match(raw)
        if inline_label_match:
            label = inline_label_match.group(1)
            raw_code = f"{inline_label_match.group(2)}{inline_label_match.group(3)}".rstrip()
            is_instr, code = classify_instruction_text(raw_code)
            if is_flush_left(raw) and is_instr:
                label_ident = sanitize_identifier(label)
                if label in fn.aliases:
                    continue
                if not first_label_emitted and label_ident == fn_ident:
                    first_label_emitted = True
                else:
                    out.append(f"{label_ident}:")
                    first_label_emitted = True
                if line_address is None:
                    out.append(f"    // asm: {raw_code}")
                else:
                    out.append(f"    // asm {line_address:08X}: {raw_code}")
                emitted_any = True
                last_executable_code = code
                continue

        bare_label_match = BARE_LABEL_RE.match(raw)
        if is_flush_left(raw) and bare_label_match and bare_label_match.group(1) in fn.labels:
            if bare_label_match.group(1) in fn.aliases:
                continue
            label_ident = sanitize_identifier(bare_label_match.group(1))
            if not first_label_emitted and label_ident == fn_ident:
                first_label_emitted = True
            else:
                out.append(f"{label_ident}:")
                first_label_emitted = True
            continue

        if is_flush_left(raw) and bare_label_match:
            if line_address is None:
                out.append(f"    // asm: {raw.rstrip()}")
            else:
                out.append(f"    // asm {line_address:08X}: {raw.rstrip()}")
            continue

        is_instr, code = classify_instruction_text(raw)
        if not is_instr:
            continue
        if line_address is None:
            out.append(f"    // asm: {raw.rstrip()}")
        else:
            out.append(f"    // asm {line_address:08X}: {raw.rstrip()}")
        emitted_any = True
        last_executable_code = code

    if emitted_any:
        if last_executable_code is not None and not instruction_terminates_function(last_executable_code):
            out.append("    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION")
        out.append(f'    TRACE_EVENT(&g_crusn_machine->trace, "function", "{fn.name}", 0, 0);')
    else:
        out.append("    /* no executable asm lines detected */")
    out.append("    UNIMPL();")
    out.append("}")
    return out


def infer_word_symbol(
    var: WordVariable,
    module: str,
    symbol_table: dict[str, SymbolInfo] | None = None,
    type_overrides: dict[str, TypeOverride] | None = None,
) -> SymbolInfo:
    def is_string_pointer_type(c_type: str) -> bool:
        normalized = " ".join(c_type.split())
        return normalized in {"char *", "const char *"}

    if len(var.values) == 1:
        value = var.values[0].strip()
        rendered_value = convert_expr(value, type_overrides)
        if var.name.lower().endswith("i") and BRANCH_TARGET_RE.fullmatch(value) and parse_int_token(value) is None:
            return SymbolInfo(name=var.name, kind="define", module=module, expr=rendered_value)
        if var.name.endswith("I") and value == var.name[:-1]:
            return SymbolInfo(name=var.name, kind="define", module=module, expr=rendered_value)
        if symbol_table is not None:
            target = symbol_table.get(value)
            if target is not None and target.kind == "define" and target.expr is not None:
                return SymbolInfo(name=var.name, kind="define", module=module, expr=rendered_value)
            if target is not None and target.kind == "function":
                return SymbolInfo(name=var.name, kind="variable", module=module, c_type="void (*)(void)")
            if target is not None and target.kind == "variable" and target.c_type:
                return SymbolInfo(name=var.name, kind="variable", module=module, c_type=f"{target.c_type} *")
        return SymbolInfo(name=var.name, kind="variable", module=module, c_type="int")

    if symbol_table is not None and all(parse_int_token(value) is None for value in var.values):
        targets = [symbol_table.get(value) for value in var.values]
        if all(target is not None and target.kind == "define" for target in targets):
            return SymbolInfo(name=var.name, kind="variable", module=module, c_type="int", array_expr=str(len(var.values)))
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
                return SymbolInfo(
                    name=var.name,
                    kind="variable",
                    module=module,
                    c_type=f"{only_type} *",
                    array_expr=str(len(var.values)),
                )
        if all(target is not None for target in targets):
            return SymbolInfo(name=var.name, kind="variable", module=module, c_type="uintptr_t", array_expr=str(len(var.values)))

    return SymbolInfo(name=var.name, kind="variable", module=module, c_type="int", array_expr=str(len(var.values)))


def infer_numeric_symbol(
    var: NumericVariable,
    module: str,
    symbol_table: dict[str, SymbolInfo] | None = None,
    type_overrides: dict[str, TypeOverride] | None = None,
) -> SymbolInfo:
    if var.directive == ".float":
        if len(var.values) == 1:
            return SymbolInfo(name=var.name, kind="variable", module=module, c_type="float")
        return SymbolInfo(name=var.name, kind="variable", module=module, c_type="float", array_expr=str(len(var.values)))
    if var.directive in {".word", ".intdata"}:
        return infer_word_symbol(
            WordVariable(name=var.name, values=var.values, asm_lines=var.asm_lines),
            module,
            symbol_table,
            type_overrides,
        )
    return SymbolInfo(name=var.name, kind="variable", module=module, c_type="int", array_expr=str(len(var.values)))


def infer_string_symbol(name: str, module: str) -> SymbolInfo:
    return SymbolInfo(name=name, kind="variable", module=module, c_type="const char", array_expr="")


def standalone_numeric_values(data: StandaloneLabeledData) -> list[str]:
    directive_lower = data.directive.lower()
    values: list[str] = []
    if directive_lower in {".intdata", ".float"}:
        values = [part.strip() for part in data.rest.split(",") if part.strip()]
    elif directive_lower == ".word":
        first_row = parse_numeric_data_line(data.asm_lines[0], data.name)
        if first_row is not None:
            row_values, _comment = first_row
            values.extend(row_values)
        for asm_line in data.asm_lines[1:]:
            asm_code, _asm_comment = split_comment(asm_line)
            stripped = asm_code.strip()
            if not stripped.lower().startswith(".word"):
                continue
            operands = stripped[5:].strip()
            if operands:
                values.extend(part.strip() for part in operands.split(",") if part.strip())
    if not values:
        return ["0"]
    return values


def standalone_numeric_variable(data: StandaloneLabeledData) -> NumericVariable:
    return NumericVariable(
        name=data.name,
        directive=data.directive.lower(),
        values=standalone_numeric_values(data),
        asm_lines=data.asm_lines,
    )


def standalone_string_value(data: StandaloneLabeledData) -> str | None:
    return parse_simple_string_operand(data.rest) or parse_string_blob_operand(data.rest)


def standalone_storage_asm_text(data: StandaloneLabeledData) -> str:
    return "\n".join(line for line in data.asm_lines if line.strip())


def infer_storage_symbol(name: str, size_expr: str, module: str, type_overrides: dict[str, TypeOverride] | None = None) -> SymbolInfo:
    c_type = "int"
    if type_overrides is not None:
        override = type_overrides.get(name)
        if override is not None and override.c_type:
            c_type = override.c_type
    array_expr = storage_array_expr_for_symbol(name, size_expr, type_overrides)
    return SymbolInfo(name=name, kind="variable", module=module, c_type=c_type, array_expr=array_expr)


def render_numeric_variable(
    var: NumericVariable,
    symbol_table: dict[str, SymbolInfo] | None = None,
    type_overrides: dict[str, TypeOverride] | None = None,
    global_symbol_table: dict[str, SymbolInfo] | None = None,
    exported_header_names: set[str] | None = None,
) -> list[str]:
    def lookup_symbol(name: str) -> SymbolInfo | None:
        target = None
        if symbol_table is not None:
            target = symbol_table.get(name)
        if target is None and global_symbol_table is not None:
            target = global_symbol_table.get(name)
        return target

    def resolve_target_symbol(name: str) -> SymbolInfo | None:
        seen: set[str] = set()
        current_name = name
        while current_name not in seen:
            seen.add(current_name)
            target = lookup_symbol(current_name)
            if target is None:
                return None
            if target.kind != "define" or target.expr is None:
                return target
            current_name = target.expr.strip()
        return None

    def render_word_value(value: str) -> str:
        rendered = convert_expr(value, type_overrides)
        if symbol.c_type == "uintptr_t":
            stripped_value = value.strip()
            target = resolve_target_symbol(stripped_value)
            if target is not None and target.kind == "function":
                return f"(uintptr_t){rendered}"
            if target is not None and target.kind == "variable" and BRANCH_TARGET_RE.fullmatch(stripped_value):
                return f"(uintptr_t)&{rendered}"
            if target is not None and target.kind == "variable" and target.c_type == "const char *":
                return f"(uintptr_t)&{rendered}"
            if parse_int_token(stripped_value) is None:
                return f"(uintptr_t)({rendered})"
        return rendered

    out: list[str] = []
    inferred_symbol = infer_numeric_symbol(var, "", symbol_table, type_overrides)
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
        renderable_asm_lines = [line for line in var.asm_lines if render_asm_comment(line) is None]
        if renderable_asm_lines:
            out.append(f"/* asm: {renderable_asm_lines[0].strip()} */")
            for extra_line in renderable_asm_lines[1:]:
                out.append(f"/* asm: \t{extra_line.strip()} */")
    ident = sanitize_identifier(var.name)
    if symbol.kind == "define" and symbol.expr is not None:
        out.append(f"#define {ident} {symbol.expr}")
    elif len(var.values) == 1:
        value = var.values[0].strip()
        rendered_value = format_float_expr(value, type_overrides) if var.directive == ".float" else render_word_value(value)
        linkage = "" if is_header_exported_symbol(symbol.name, exported_header_names) else "static "
        out.append(f"{linkage}{variable_definition_prefix(symbol.name, symbol.c_type)} = {rendered_value};")
    else:
        structured_rows: list[str] = []
        if var.asm_lines:
            for asm_line in var.asm_lines:
                asm_comment = render_asm_comment(asm_line, strip_asm_comment_markers=True)
                if asm_comment is not None:
                    structured_rows.append(f"    {asm_comment}")
                    continue
                asm_code, asm_comment_text = split_comment(asm_line)
                asm_stripped = asm_code.strip()
                if var.directive != ".float" and asm_stripped.lower().startswith(".string"):
                    packed_words = pack_string_directive_any(asm_stripped[7:].strip())
                    if packed_words is not None:
                        rendered_values = ", ".join(render_word_value(value) for value in packed_words)
                        comment_text = asm_comment_text[1:].strip() if asm_comment_text.startswith(";") else asm_comment_text.strip()
                        if comment_text:
                            structured_rows.append(f"    {rendered_values}, // {comment_text}")
                        else:
                            structured_rows.append(f"    {rendered_values},")
                        continue
                parsed_row = parse_numeric_data_line(asm_line, var.name)
                if parsed_row is None:
                    continue
                row_values, asm_comment = parsed_row
                if var.directive == ".float":
                    rendered_values = ", ".join(format_float_expr(value, type_overrides) for value in row_values)
                else:
                    rendered_values = ", ".join(render_word_value(value) for value in row_values)
                comment_text = asm_comment[1:].strip() if asm_comment.startswith(";") else asm_comment.strip()
                if comment_text:
                    structured_rows.append(f"    {rendered_values}, // {comment_text}")
                else:
                    structured_rows.append(f"    {rendered_values},")
        if structured_rows:
            prefix = variable_definition_prefix(symbol.name, symbol.c_type, symbol.array_expr, omit_array_size=True)
            linkage = "" if is_header_exported_symbol(symbol.name, exported_header_names) else "static "
            out.append(f"{linkage}{prefix} = {{")
            out.extend(structured_rows)
            out.append("};")
        else:
            if var.directive == ".float":
                values = ", ".join(format_float_expr(value, type_overrides) for value in var.values)
            else:
                values = ", ".join(render_word_value(value) for value in var.values)
            prefix = variable_definition_prefix(symbol.name, symbol.c_type, symbol.array_expr, omit_array_size=True)
            linkage = "" if is_header_exported_symbol(symbol.name, exported_header_names) else "static "
            out.append(f"{linkage}{prefix} = {{ {values} }};")
    return out


def render_word_variable(
    var: WordVariable,
    symbol_table: dict[str, SymbolInfo] | None = None,
    type_overrides: dict[str, TypeOverride] | None = None,
    global_symbol_table: dict[str, SymbolInfo] | None = None,
    exported_header_names: set[str] | None = None,
) -> list[str]:
    return render_numeric_variable(
        NumericVariable(name=var.name, directive=".word", values=var.values, asm_lines=var.asm_lines),
        symbol_table,
        type_overrides,
        global_symbol_table,
        exported_header_names,
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


def render_inner_label_defines(inner_labels: list[tuple[str, int]]) -> list[str]:
    return [f"#define {sanitize_identifier(name)} {offset}" for name, offset in inner_labels]


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


def parse_equ_file(path: Path) -> tuple[list[str], list[str], list[EquSetEntry]]:
    banner_comments: list[str] = []
    globls: list[str] = []
    sets: list[EquSetEntry] = []
    in_banner = True

    for raw in strip_struct_definition_blocks(path.read_text(errors="ignore").splitlines()):
        stripped = raw.strip()
        if not stripped:
            if in_banner:
                banner_comments.append("")
            continue
        if stripped.startswith("*"):
            if in_banner:
                banner_comments.append(stripped[1:].strip())
            continue
        if stripped.startswith(";"):
            if in_banner:
                banner_comments.append(stripped[1:].strip())
            continue

        in_banner = False
        code, comment = split_comment(raw)
        match = GLOBL_RE.match(code)
        if match:
            names = [part.strip() for part in match.group(1).split(",") if part.strip()]
            globls.extend(names)
            continue

        match = SET_RE.match(code)
        if match:
            name, expr = match.groups()
            parsed_comment = comment[1:].strip() if comment.startswith(";") else comment
            sets.append(EquSetEntry(name=name, expr=render_set_expr(expr), comment=parsed_comment, asm_line=raw.rstrip()))
            continue

    return normalize_banner_comments(banner_comments), globls, sets


def render_globl_symbol(symbol: SymbolInfo | None, name: str, sizeless_extern_arrays: bool = False) -> str | None:
    if sizeless_extern_arrays and name == "NULL":
        return None
    if symbol is None:
        return None
    if symbol.kind == "define" and symbol.expr is not None:
        return f"#define {sanitize_identifier(symbol.name)} {sanitize_identifier(symbol.expr)}"
    if symbol.kind == "function":
        return f"void {sanitize_identifier(symbol.name)}(void);"
    if symbol.kind == "variable":
        if symbol.array_expr is not None:
            ident = sanitize_identifier(symbol.name)
            sep = "" if symbol.c_type.endswith("*") else " "
            return f"extern {symbol.c_type}{sep}{ident}[];"
        return variable_declaration(symbol.name, symbol.c_type, symbol.array_expr, is_extern=True)
    return None


def render_equ_header(
    src_path: Path,
    banner_comments: list[str],
    globls: list[str],
    sets: list[EquSetEntry],
    symbol_table: dict[str, SymbolInfo] | None = None,
) -> str:
    guard = f"{sanitize_guard_stem(src_path.stem)}_H"
    out: list[str] = []
    out.append(f"#ifndef {guard}")
    out.append(f"#define {guard}")
    out.append("")
    out.append('#include "port.h"')
    out.append("")
    out.append(f"/* Generated from asm/{src_path.name}. */")
    if banner_comments:
        out.append("")
        for line in banner_comments:
            out.append(f"// {line}")

    define_blocks: list[RenderBlock] = []
    extern_blocks: list[RenderBlock] = []
    function_blocks: list[RenderBlock] = []
    emitted_sets: dict[str, str] = {}
    emitted_globls: set[str] = set()
    in_banner = True
    set_index = 0
    pending_comments: list[str] = []
    if symbol_table is None:
        symbol_table = {}
    for raw in strip_struct_definition_blocks(src_path.read_text(errors="ignore").splitlines()):
        stripped = raw.strip()
        if not stripped:
            if not in_banner and pending_comments:
                pending_comments.append("")
            continue
        if stripped.startswith("*") or stripped.startswith(";"):
            if in_banner:
                continue
            comment_body = stripped[1:].strip()
            pending_comments.append(f"// {comment_body}")
            continue

        in_banner = False
        code, _comment = split_comment(raw)
        globl_match = GLOBL_RE.match(code)
        if globl_match:
            comment_lines = [*pending_comments, f"// asm: {raw.rstrip()}"]
            pending_comments = []
            names = [part.strip() for part in globl_match.group(1).split(",") if part.strip()]
            rendered_any = False
            for name in names:
                if name in emitted_globls:
                    continue
                rendered = render_globl_symbol(symbol_table.get(name), name, src_path.stem.upper() == "GLOBALS")
                if rendered is not None:
                    block = RenderBlock(kind="function" if rendered.startswith("void ") else "extern")
                    if not rendered_any:
                        block.lines.extend(comment_lines)
                        rendered_any = True
                    block.lines.append(rendered)
                    if block.kind == "function":
                        function_blocks.append(block)
                    else:
                        extern_blocks.append(block)
                    emitted_globls.add(name)
            continue

        set_match = SET_RE.match(code)
        if not set_match:
            continue

        entry = sets[set_index]
        set_index += 1
        prior_expr = emitted_sets.get(entry.name)
        if prior_expr == entry.expr:
            continue
        block = RenderBlock(kind="define", lines=[*pending_comments])
        pending_comments = []
        if prior_expr is not None:
            block.lines.append(f"#undef {entry.name}")
        block.lines.append(f"// asm: {entry.asm_line}")
        expr = f"({entry.expr})" if needs_parens(entry.expr) else entry.expr
        line = f"#define {entry.name} {expr}"
        if entry.comment:
            line += f" //{entry.comment}"
        block.lines.append(line)
        define_blocks.append(block)
        emitted_sets[entry.name] = entry.expr

    rendered_body = join_render_blocks(define_blocks + extern_blocks + function_blocks)
    if rendered_body:
        out.append("")
        out.extend(normalize_emitted_lines(rendered_body))

    out.append("")
    out.append(f"#endif /* {guard} */")
    out.append("")
    return "\n".join(out)


def render_discovered_labels_header(
    labels: list[LabelEntry],
    type_overrides: dict[str, TypeOverride] | None = None,
) -> str:
    effective_type_overrides = with_discovered_label_overrides(
        type_overrides,
        {entry.name for entry in labels},
    )
    out = [
        "#ifndef DISCOVERED_LABELS_H",
        "#define DISCOVERED_LABELS_H",
        "",
        "/* Generated from tools/ida/address.map. */",
        "",
    ]
    for entry in labels:
        out.append(f"#define {output_symbol_name(entry.name, effective_type_overrides)} 0x{entry.addr:08X}")
    out.extend([
        "",
        "#endif /* DISCOVERED_LABELS_H */",
        "",
    ])
    return "\n".join(out)


def render_port_header() -> str:
    out = [
        "#ifndef PORT_H",
        "#define PORT_H",
        "",
        '#include "discovered_defines.h"',
        '#include "discovered_labels.h"',
        '#include "types.h"',
        "",
        "#endif /* PORT_H */",
        "",
    ]
    return "\n".join(out)


def render_string_variable(var: StringVariable, exported_header_names: set[str] | None = None) -> list[str]:
    linkage = "" if is_header_exported_symbol(var.name, exported_header_names) else "static "
    return [f"{linkage}{variable_definition_prefix(var.name, 'const char', '', omit_array_size=True)} = {var.c_string};"]


def render_sptr_variable(name: str, c_string: str, asm_line: str, exported_header_names: set[str] | None = None) -> list[str]:
    ident = sanitize_identifier(name)
    linkage = "" if is_header_exported_symbol(name, exported_header_names) else "static "
    return [
        f"/* asm: {asm_line.strip()} */",
        f"{linkage}const char {ident}[] = {c_string};",
    ]


def render_sptr_table(name: str, asm_lines: list[str], exported_header_names: set[str] | None = None) -> list[str]:
    out = [f"/* asm: {asm_lines[0].strip()} */"]
    values: list[tuple[str, str]] = []
    for extra_line in asm_lines[1:]:
        out.append(f"/* asm: {extra_line.strip()} */")
        c_string = parse_sptr_row(extra_line)
        if c_string is not None:
            _code, comment = split_comment(extra_line)
            values.append((c_string, comment))
    prefix = variable_definition_prefix(name, "const char *", str(len(values)), omit_array_size=True)
    linkage = "" if is_header_exported_symbol(name, exported_header_names) else "static "
    out.append(f"{linkage}{prefix} = {{")
    for value, comment in values:
        comment_text = comment[1:].strip() if comment.startswith(";") else comment.strip()
        if comment_text:
            out.append(f"    {value}, // {comment_text}")
        else:
            out.append(f"    {value},")
    out.append("};")
    return out


def render_storage_variable(
    name: str,
    size_expr: str,
    asm_line: str,
    exported_header_names: set[str] | None = None,
) -> list[str]:
    linkage = "" if is_header_exported_symbol(name, exported_header_names) else "static "
    return [
        f"/* asm: {name}\t{asm_line.strip()} */",
        f"{linkage}{storage_declaration(name, size_expr)}",
    ]


def render_macro_data_placeholder(name: str, asm_lines: list[str], exported_header_names: set[str] | None = None) -> list[str]:
    out: list[str] = []
    for asm_line in filter_renderable_asm_lines(asm_lines):
        out.append(f"/* asm: {asm_line.strip()} */")
    linkage = "" if is_header_exported_symbol(name, exported_header_names) else "static "
    out.append(f"{linkage}int {sanitize_identifier(name)};")
    return out


def filter_renderable_asm_lines(lines: list[str]) -> list[str]:
    return [line for line in lines if line.strip() and not is_comment_line(line)]


def render_local_function_prototypes(
    functions: list[FunctionBlock],
    exported_header_names: set[str] | None = None,
    exported_function_names: set[str] | None = None,
) -> list[str]:
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
        is_exported = (
            fn.name in (exported_header_names or set())
            or fn.name in (exported_function_names or set())
            or any(alias in (exported_header_names or set()) for alias in fn.aliases)
            or any(alias in (exported_function_names or set()) for alias in fn.aliases)
        )
        linkage = "" if is_exported else "static "
        out.append(f"{linkage}void {sanitize_identifier(fn.name)}(void);")
        seen.add(fn.name)
    return out


def render_external_symbol_declarations(
    lines: list[str],
    functions: list[FunctionBlock],
    symbol_table: dict[str, SymbolInfo],
    global_symbol_table: dict[str, SymbolInfo] | None,
    predeclared_names: set[str] | None = None,
) -> list[str]:
    if global_symbol_table is None:
        return []

    local_names = set(symbol_table)
    for fn in functions:
        local_names.add(fn.name)
        local_names.update(fn.aliases)
    if predeclared_names:
        local_names.update(predeclared_names)

    out: list[str] = []
    seen: set[str] = set()
    for raw in lines:
        if is_comment_or_blank(raw):
            continue
        text = extract_reference_text(raw)
        code, _comment = split_comment(text)
        code = code.strip()
        if not code:
            continue
        for tok in iter_operand_symbol_tokens(code):
            if tok in local_names or tok in seen:
                continue
            symbol = global_symbol_table.get(tok)
            if symbol is None:
                continue
            rendered = render_globl_symbol(symbol, tok, sizeless_extern_arrays=True)
            if rendered is None:
                continue
            out.append(rendered)
            seen.add(tok)
    return out


def render_local_variable_declarations(
    lines: list[str],
    symbol_table: dict[str, SymbolInfo],
    exported_header_names: set[str] | None = None,
    predeclared_names: set[str] | None = None,
    data_only_macros: frozenset[str] | None = None,
) -> list[str]:
    def alias_word_target_defined_before_meaningful_use(curr_idx: int, alias_name: str, target_name: str) -> bool:
        next_idx = curr_idx + 1
        while next_idx < len(lines):
            next_raw = lines[next_idx]
            if is_comment_or_blank(next_raw):
                next_idx += 1
                continue
            next_code, _next_comment = split_comment(next_raw)
            storage_match = STANDALONE_STORAGE_FULL_RE.match(next_code)
            if storage_match is not None and storage_match.group(2) == target_name:
                return True
            data_match = DATA_LABEL_RE.match(next_raw)
            if data_match is not None and data_match.group(1) == target_name:
                return True
            sptr_match = parse_sptr_label(next_raw)
            if sptr_match is not None and sptr_match[0] == target_name:
                return True

            text = extract_reference_text(next_raw)
            ref_code, _ref_comment = split_comment(text)
            ref_code = ref_code.strip()
            if ref_code:
                ref_toks = set(iter_operand_symbol_tokens(ref_code))
                if alias_name in ref_toks or target_name in ref_toks:
                    return False
            next_idx += 1
        return False

    branch_targets = collect_branch_targets(lines)
    variable_names = {name for name, symbol in symbol_table.items() if symbol.kind == "variable"}
    defined_names = set(predeclared_names or set())
    needed_names: list[str] = []
    seen_needed: set[str] = set()

    def is_single_symbol_word_alias(raw: str, curr_idx: int) -> bool:
        data_match = DATA_LABEL_RE.match(raw)
        if data_match is None or data_match.group(2).lower() != ".word":
            return False
        operands = [part.strip() for part in data_match.group(3).split(",") if part.strip()]
        return (
            len(operands) == 1
            and BRANCH_TARGET_RE.fullmatch(operands[0]) is not None
            and alias_word_target_defined_before_meaningful_use(curr_idx, data_match.group(1), operands[0])
        )

    idx = 0
    while idx < len(lines):
        raw = lines[idx]
        if not is_comment_or_blank(raw):
            text = extract_reference_text(raw)
            code, _comment = split_comment(text)
            code = code.strip()
            if code:
                skip_tokens: set[str] = set()
                storage_match = STANDALONE_STORAGE_FULL_RE.match(code)
                if storage_match is not None:
                    skip_tokens.add(storage_match.group(2))
                data_match = DATA_LABEL_RE.match(raw)
                if data_match is not None:
                    skip_tokens.add(data_match.group(1))
                sptr_match = parse_sptr_label(raw)
                if sptr_match is not None:
                    skip_tokens.add(sptr_match[0])
                if is_single_symbol_word_alias(raw, idx):
                    code = ""
                for tok in iter_operand_symbol_tokens(code):
                    if tok in skip_tokens:
                        continue
                    if tok in variable_names and tok not in defined_names and tok not in seen_needed:
                        needed_names.append(tok)
                        seen_needed.add(tok)

        code, _comment = split_comment(raw)
        if STANDALONE_STORAGE_FULL_RE.match(code):
            defined_names.add(STANDALONE_STORAGE_FULL_RE.match(code).group(2))  # type: ignore[union-attr]
            idx += 1
            continue
        sptr_match = parse_sptr_label(raw)
        if sptr_match is not None:
            defined_names.add(sptr_match[0])
            idx += 1
            continue
        data_match = DATA_LABEL_RE.match(raw)
        if data_match:
            defined_names.add(data_match.group(1))
            idx += 1
            continue
        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx, branch_targets, data_only_macros)
        if standalone_data is not None:
            if not is_single_symbol_word_alias(lines[idx], idx):
                for asm_line in standalone_data.asm_lines:
                    text = extract_reference_text(asm_line)
                    code, _comment = split_comment(text)
                    code = code.strip()
                    if not code:
                        continue
                    for tok in iter_operand_symbol_tokens(code):
                        if tok == standalone_data.name:
                            continue
                        if tok in variable_names and tok not in defined_names and tok not in seen_needed:
                            needed_names.append(tok)
                            seen_needed.add(tok)
            defined_names.add(standalone_data.name)
            idx = next_idx
            continue
        idx += 1

    out: list[str] = []
    seen: set[str] = set()
    for name in needed_names:
        symbol = symbol_table.get(name)
        if symbol is None or symbol.kind != "variable":
            continue
        ident = sanitize_identifier(symbol.name)
        if ident in seen:
            continue
        if is_header_exported_symbol(symbol.name, exported_header_names):
            out.append(variable_declaration(symbol.name, symbol.c_type, symbol.array_expr, is_extern=True))
        else:
            out.append(f"static {variable_declaration(symbol.name, symbol.c_type, symbol.array_expr)}")
        seen.add(ident)
    return out


def render_local_define_declarations(symbol_table: dict[str, SymbolInfo]) -> list[str]:
    out: list[str] = []
    seen: set[str] = set()
    for symbol in symbol_table.values():
        if symbol.kind != "define" or symbol.expr is None:
            continue
        ident = sanitize_identifier(symbol.name)
        if ident in seen:
            continue
        out.append(f"#define {ident} {symbol.expr}")
        seen.add(ident)
    return out


def collect_standalone_labeled_data(
    lines: list[str],
    start_idx: int,
    branch_targets: set[str] | None = None,
    data_only_macros: frozenset[str] | None = None,
    macros: dict[str, ccm.MacroDef] | None = None,
    symbols: dict[str, int] | None = None,
) -> tuple[StandaloneLabeledData | None, int]:
    if branch_targets is None:
        branch_targets = set()
    raw = lines[start_idx]
    raw_code, _raw_comment = split_comment(raw)
    label_match = LABEL_RE.match(raw)
    data_label_match = DATA_LABEL_RE.match(raw_code) if is_flush_left(raw) else None
    if label_match or data_label_match:
        if data_label_match is not None:
            label, directive, rest = data_label_match.groups()
            rest = f"{directive}{rest}"
        else:
            assert label_match is not None
            label, rest = label_match.groups()
        rest_code, _rest_comment = split_comment(rest)
        if rest_code.strip():
            if line_starts_data_only_macro(rest_code, data_only_macros):
                expanded = expand_data_only_macro_invocation(raw, data_only_macros, macros, symbols)
                if expanded:
                    nested_data, _nested_next_idx = collect_standalone_labeled_data(
                        [label, *expanded],
                        0,
                        set(),
                        data_only_macros,
                        macros,
                        symbols,
                    )
                    if nested_data is not None:
                        return nested_data, start_idx + 1
                return StandaloneLabeledData(label, ".macrodata", "", [raw.rstrip()]), start_idx + 1
            stripped = rest_code.strip()
            if stripped.lower().startswith(".string"):
                string_operands = stripped[7:].strip()
                c_string = parse_simple_string_operand(string_operands) or parse_string_blob_operand(string_operands)
                if c_string is not None:
                    return StandaloneLabeledData(label, ".string", string_operands, [raw.rstrip()]), start_idx + 1
            numeric_directive = parse_numeric_directive(stripped)
            if numeric_directive is not None:
                directive_name, values = numeric_directive
                asm_lines = [raw.rstrip()]
                numeric_values = [*values]
                next_idx = start_idx + 1
                mixed_numeric_directives = False
                while next_idx < len(lines):
                    next_raw = lines[next_idx]
                    if not next_raw.strip():
                        asm_lines.append(next_raw.rstrip())
                        next_idx += 1
                        continue
                    if render_asm_comment(next_raw) is not None:
                        following_idx = next_significant_line_index(lines, next_idx + 1)
                        if following_idx is not None and is_flush_left(lines[following_idx]):
                            break
                        asm_lines.append(next_raw.rstrip())
                        next_idx += 1
                        continue
                    if is_flush_left(next_raw):
                        break
                    next_code, _next_comment = split_comment(next_raw)
                    next_stripped = next_code.strip()
                    if not next_stripped:
                        asm_lines.append(next_raw.rstrip())
                        next_idx += 1
                        continue
                    if next_stripped.lower().startswith(".string"):
                        packed_words = pack_string_directive_any(next_stripped[7:].strip())
                        if packed_words is not None:
                            asm_lines.append(next_raw.rstrip())
                            numeric_values.extend(packed_words)
                            mixed_numeric_directives = True
                            next_idx += 1
                            continue
                    parsed_row = parse_numeric_data_line(next_raw)
                    if parsed_row is None:
                        break
                    asm_lines.append(next_raw.rstrip())
                    row_values, _row_comment = parsed_row
                    numeric_values.extend(row_values)
                    row_directive = parse_numeric_directive(next_stripped)
                    if row_directive is not None and row_directive[0] != directive_name:
                        mixed_numeric_directives = True
                    next_idx += 1
                normalized = ".intdata" if mixed_numeric_directives else directive_name
                return StandaloneLabeledData(label, normalized, ",".join(numeric_values), asm_lines), next_idx
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
                parse_sptr_row(probe_raw) is not None
                or parse_numeric_directive(probe_stripped) is not None
                or parse_simple_string_operand(probe_stripped) is not None
                or STANDALONE_STORAGE_RE.match(probe_stripped) is not None
                or line_starts_data_only_macro(probe_raw, data_only_macros)
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
    saw_sptr_rows = False
    inner_labels: list[tuple[str, int]] = []
    while next_idx < len(lines):
        next_raw = lines[next_idx]
        if not next_raw.strip():
            asm_lines.append(next_raw.rstrip())
            next_idx += 1
            continue
        if render_asm_comment(next_raw) is not None:
            following_idx = next_significant_line_index(lines, next_idx + 1)
            if following_idx is not None and is_flush_left(lines[following_idx]):
                break
            asm_lines.append(next_raw.rstrip())
            next_idx += 1
            continue
        if parse_sptr_row(next_raw) is not None:
            asm_lines.append(next_raw.rstrip())
            saw_sptr_rows = True
            next_idx += 1
            continue
        if saw_sptr_rows:
            return StandaloneLabeledData(label, ".sptrtable", "", asm_lines, inner_labels), next_idx
        if line_starts_data_only_macro(next_raw, data_only_macros):
            expanded = expand_data_only_macro_invocation(next_raw, data_only_macros, macros, symbols)
            if expanded:
                nested_data, _nested_next_idx = collect_standalone_labeled_data(
                    ["_macro_expand_tmp", *expanded],
                    0,
                    set(),
                    data_only_macros,
                    macros,
                    symbols,
                )
                if nested_data is not None:
                    if nested_data.directive == ".sptrtable":
                        if saw_numeric_directive:
                            normalized = ".intdata" if mixed_numeric_directives or numeric_directive_name is None else numeric_directive_name
                            return StandaloneLabeledData(label, normalized, ",".join(numeric_values), asm_lines, inner_labels), next_idx
                        asm_lines.extend(nested_data.asm_lines[1:])
                        saw_sptr_rows = True
                        next_idx += 1
                        continue

                    if nested_data.directive in {".word", ".float", ".intdata"}:
                        nested_values = standalone_numeric_values(nested_data)
                        nested_offset = len(numeric_values)
                        asm_lines.extend(nested_data.asm_lines[1:])
                        numeric_values.extend(nested_values)
                        for inner_name, inner_offset in nested_data.inner_labels:
                            inner_labels.append((inner_name, nested_offset + inner_offset))
                        saw_numeric_directive = True
                        if numeric_directive_name is None:
                            numeric_directive_name = nested_data.directive
                        elif numeric_directive_name != nested_data.directive:
                            mixed_numeric_directives = True
                        next_idx += 1
                        continue

            if saw_numeric_directive:
                normalized = ".intdata" if mixed_numeric_directives or numeric_directive_name is None else numeric_directive_name
                return StandaloneLabeledData(label, normalized, ",".join(numeric_values), asm_lines, inner_labels), next_idx

            asm_lines.append(next_raw.rstrip())
            next_idx += 1
            while next_idx < len(lines):
                more_raw = lines[next_idx]
                if is_comment_or_blank(more_raw):
                    asm_lines.append(more_raw.rstrip())
                    next_idx += 1
                    continue
                if line_starts_data_only_macro(more_raw, data_only_macros):
                    asm_lines.append(more_raw.rstrip())
                    next_idx += 1
                    continue
                break
            return StandaloneLabeledData(label, ".macrodata", "", asm_lines, inner_labels), next_idx
        if is_flush_left(next_raw):
            data_match = DATA_LABEL_RE.match(next_raw)
            if data_match is not None:
                nested_name, nested_directive, _nested_rest = data_match.groups()
                nested_directive_lower = nested_directive.lower()
                if nested_directive_lower in {".word", ".float", ".string"} and nested_name not in branch_targets:
                    nested_lines = [next_raw.rstrip()]
                    nested_values: list[str] = []
                    nested_has_string = nested_directive_lower == ".string"
                    parsed_first_row = parse_numeric_data_line(next_raw, nested_name)
                    if parsed_first_row is not None:
                        row_values, _row_comment = parsed_first_row
                        nested_values.extend(row_values)
                        probe_idx = next_idx + 1
                        while probe_idx < len(lines):
                            probe_raw = lines[probe_idx]
                            if not probe_raw.strip():
                                nested_lines.append(probe_raw.rstrip())
                                probe_idx += 1
                                continue
                            if render_asm_comment(probe_raw) is not None:
                                following_idx = next_significant_line_index(lines, probe_idx + 1)
                                if following_idx is not None and is_flush_left(lines[following_idx]):
                                    break
                                nested_lines.append(probe_raw.rstrip())
                                probe_idx += 1
                                continue
                            if is_flush_left(probe_raw):
                                break
                            probe_code, _probe_comment = split_comment(probe_raw)
                            probe_stripped = probe_code.strip()
                            if ".string" in probe_stripped.lower():
                                nested_has_string = True
                            parsed_probe_row = parse_numeric_data_line(probe_raw)
                            if parsed_probe_row is None:
                                break
                            nested_lines.append(probe_raw.rstrip())
                            probe_values, _probe_row_comment = parsed_probe_row
                            nested_values.extend(probe_values)
                            probe_idx += 1
                        if nested_values and nested_has_string and asm_lines_form_string_blob(nested_lines):
                            nested_offset = len(numeric_values)
                            inner_labels.append((nested_name, nested_offset))
                            asm_lines.extend(nested_lines)
                            numeric_values.extend(nested_values)
                            saw_numeric_directive = True
                            if numeric_directive_name is None:
                                numeric_directive_name = ".intdata"
                            elif numeric_directive_name != ".intdata":
                                mixed_numeric_directives = True
                            next_idx = probe_idx
                            continue
            nested_data, nested_next_idx = collect_standalone_labeled_data(
                lines,
                next_idx,
                branch_targets,
                data_only_macros,
                macros,
                symbols,
            )
            if (
                nested_data is not None
                and nested_data.directive == ".intdata"
                and nested_data.name not in branch_targets
                and asm_lines_form_string_blob(nested_data.asm_lines)
            ):
                nested_values = [part.strip() for part in nested_data.rest.split(",") if part.strip()]
                if nested_values:
                    nested_offset = len(numeric_values)
                    inner_labels.append((nested_data.name, nested_offset))
                    for inner_name, inner_offset in nested_data.inner_labels:
                        inner_labels.append((inner_name, nested_offset + inner_offset))
                    asm_lines.extend(nested_data.asm_lines)
                    numeric_values.extend(nested_values)
                    saw_numeric_directive = True
                    if numeric_directive_name is None:
                        numeric_directive_name = nested_data.directive
                    elif numeric_directive_name != nested_data.directive:
                        mixed_numeric_directives = True
                    next_idx = nested_next_idx
                    continue
            if saw_numeric_directive:
                normalized = ".intdata" if mixed_numeric_directives or numeric_directive_name is None else numeric_directive_name
                return StandaloneLabeledData(label, normalized, ",".join(numeric_values), asm_lines, inner_labels), next_idx
            return None, start_idx
        next_code, _next_comment = split_comment(next_raw)
        stripped = next_code.strip()
        if not stripped:
            asm_lines.append(next_raw.rstrip())
            next_idx += 1
            continue
        if saw_numeric_directive and stripped.lower().startswith(".string"):
            packed_words = pack_string_directive_any(stripped[7:].strip())
            if packed_words is not None:
                asm_lines.append(next_raw.rstrip())
                numeric_values.extend(packed_words)
                mixed_numeric_directives = True
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
            return StandaloneLabeledData(label, normalized, ",".join(numeric_values), asm_lines, inner_labels), next_idx
        asm_lines.append(next_raw.rstrip())
        return StandaloneLabeledData(label, directive, directive_rest, asm_lines, inner_labels), next_idx + 1
    if saw_numeric_directive:
        normalized = ".intdata" if mixed_numeric_directives or numeric_directive_name is None else numeric_directive_name
        return StandaloneLabeledData(label, normalized, ",".join(numeric_values), asm_lines, inner_labels), next_idx
    if saw_sptr_rows:
        return StandaloneLabeledData(label, ".sptrtable", "", asm_lines, inner_labels), next_idx
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
        if inline_label_match and is_flush_left(raw):
            labels.add(inline_label_match.group(1))
    return labels


def instruction_text_for_top_level_line(raw: str) -> str | None:
    code, _comment = split_comment(raw)
    stripped = code.strip()
    if not stripped:
        return None
    if STANDALONE_STORAGE_FULL_RE.match(code):
        return None
    if parse_sptr_label(raw) is not None:
        return None
    if DATA_LABEL_RE.match(raw):
        return None
    if parse_numeric_directive(stripped) is not None:
        return None

    _label, remainder = split_flush_left_label(code)
    if _label is not None and not remainder.strip():
        return None

    is_instr, code = classify_instruction_text(code)
    return code if is_instr else None


def render_top_level_items(
    lines: list[str],
    functions: list[FunctionBlock],
    branch_targets: set[str],
    address_map: dict[str, int],
    module: str,
    symbol_table: dict[str, SymbolInfo] | None = None,
    global_symbol_table: dict[str, SymbolInfo] | None = None,
    type_overrides: dict[str, TypeOverride] | None = None,
    strip_asm_comment_markers: bool = False,
    emit_set_asm_comments: bool = False,
    emit_set_defines: bool = True,
    render_overrides: dict[str, list[str]] | None = None,
    data_only_macros: frozenset[str] | None = None,
    macros: dict[str, ccm.MacroDef] | None = None,
    symbols: dict[str, int] | None = None,
    exported_header_names: set[str] | None = None,
    initial_seen_function_lines: bool = False,
    line_number_offset: int = 0,
) -> list[str]:
    def should_preserve_pending_comments_across_blank(blank_idx: int) -> bool:
        if not pending_comment_lines:
            return False
        if not all(comment_looks_like_disabled_data(line) for line in pending_comment_lines):
            return False
        next_idx = next_significant_line_index(lines, blank_idx + 1)
        if next_idx is None:
            return False
        next_raw = lines[next_idx]
        next_code, _next_comment = split_comment(next_raw)
        return bool(
            is_flush_left(next_raw)
            and (
                LABEL_RE.match(next_code)
                or DATA_LABEL_RE.match(next_code)
                or BARE_LABEL_RE.match(next_code)
                or STANDALONE_STORAGE_FULL_RE.match(next_code)
            )
        )

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
    seen_function_lines = initial_seen_function_lines
    saw_top_level_data_after_function = False
    idx = 0
    while idx < len(lines):
        raw = lines[idx]
        if idx in function_line_indexes:
            seen_function_lines = True
            saw_top_level_data_after_function = False
            pending_comment_lines = []
            idx += 1
            continue

        if not raw.strip():
            if not should_preserve_pending_comments_across_blank(idx):
                had_pending_comments = bool(pending_comment_lines)
                flush_pending_comments()
                if had_pending_comments and out and out[-1] != "":
                    out.append("")
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
            define_entry = SetDefine(
                name=output_symbol_name(name, type_overrides),
                expr=render_set_expr(expr, type_overrides, symbol_table, global_symbol_table),
                comment=comment,
            )
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
            override_lines = render_symbol_override(label, render_overrides)
            if override_lines is not None:
                flush_pending_comments()
                out.extend(override_lines)
                if seen_function_lines:
                    saw_top_level_data_after_function = True
                idx += 1
                continue
            if is_omitted_symbol(label, type_overrides):
                if seen_function_lines:
                    saw_top_level_data_after_function = True
                pending_comment_lines = []
                idx += 1
                continue
            flush_pending_comments()
            out.extend(render_storage_variable(label, parse_storage_size(rest), f"{directive}\t{label}{rest}", exported_header_names))
            if seen_function_lines:
                saw_top_level_data_after_function = True
            idx += 1
            continue

        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx, branch_targets, data_only_macros, macros, symbols)
        if standalone_data is not None:
            override_lines = render_symbol_override(standalone_data.name, render_overrides)
            if override_lines is not None:
                flush_pending_comments()
                out.extend(override_lines)
                if seen_function_lines:
                    saw_top_level_data_after_function = True
                idx = next_idx
                continue
            if is_omitted_symbol(standalone_data.name, type_overrides):
                if seen_function_lines:
                    saw_top_level_data_after_function = True
                pending_comment_lines = []
                idx = next_idx
                continue
            flush_pending_comments()
            directive_lower = standalone_data.directive.lower()
            if directive_lower in {".word", ".intdata", ".float"}:
                if standalone_data.inner_labels:
                    out.extend(render_inner_label_defines(standalone_data.inner_labels))
                numeric_var = standalone_numeric_variable(standalone_data)
                values = numeric_var.values
                if (
                    len(values) == 1
                    and next_idx < len(lines)
                    and is_flush_left(lines[next_idx])
                ):
                    next_code, _next_comment = split_comment(lines[next_idx])
                    next_label_match = BARE_LABEL_RE.match(next_code)
                    if next_label_match and next_label_match.group(1) == values[0]:
                        out.append(f"/* asm: {standalone_data.asm_lines[0].strip()} */")
                        out.append(f"#define {render_identifier(standalone_data.name, type_overrides)} {convert_expr(values[0], type_overrides)}")
                        idx = next_idx
                        continue
                out.extend(
                    render_numeric_variable(
                        numeric_var,
                        symbol_table,
                        type_overrides,
                        global_symbol_table,
                        exported_header_names,
                    )
                )
            elif directive_lower == ".string":
                c_string = standalone_string_value(standalone_data)
                if c_string is not None:
                    out.extend(render_string_variable(StringVariable(name=standalone_data.name, c_string=c_string), exported_header_names))
            elif directive_lower == ".sptrtable":
                out.extend(render_sptr_table(standalone_data.name, standalone_data.asm_lines, exported_header_names))
            elif directive_lower == ".macrodata":
                out.extend(render_macro_data_placeholder(standalone_data.name, standalone_data.asm_lines, exported_header_names))
            elif directive_lower in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
                out.extend(
                    render_storage_variable(
                        standalone_data.name,
                        parse_storage_size(standalone_data.rest),
                        standalone_storage_asm_text(standalone_data),
                        exported_header_names,
                    )
                )
            if seen_function_lines:
                saw_top_level_data_after_function = True
            idx = next_idx
            continue

        top_level_instruction = instruction_text_for_top_level_line(raw)
        if (
            saw_top_level_data_after_function
            and not is_flush_left(raw)
            and not line_starts_data_only_macro(raw, data_only_macros)
            and top_level_instruction is not None
        ):
            raise ValueError(
                f"unexpected top-level code at line {line_number_offset + idx + 1}: {raw.rstrip()}"
            )

        if not is_flush_left(raw):
            pending_comment_lines = []
            idx += 1
            continue

        sptr_match = parse_sptr_label(raw)
        if sptr_match is not None:
            label, c_string = sptr_match
            override_lines = render_symbol_override(label, render_overrides)
            if override_lines is not None:
                flush_pending_comments()
                out.extend(override_lines)
                if seen_function_lines:
                    saw_top_level_data_after_function = True
                idx += 1
                continue
            flush_pending_comments()
            out.extend(render_sptr_variable(label, c_string, raw, exported_header_names))
            if seen_function_lines:
                saw_top_level_data_after_function = True
            idx += 1
            continue

        data_match = DATA_LABEL_RE.match(raw)
        if not data_match:
            pending_comment_lines = []
            idx += 1
            continue

        top_level_instruction = instruction_text_for_top_level_line(raw)
        if saw_top_level_data_after_function and top_level_instruction is not None:
            raise ValueError(
                f"unexpected top-level code at line {line_number_offset + idx + 1}: {raw.rstrip()}"
            )

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
    render_overrides: dict[str, list[str]] | None = None,
    global_symbol_table: dict[str, SymbolInfo] | None = None,
    discovered_label_names: set[str] | None = None,
) -> str:
    effective_type_overrides = with_discovered_label_overrides(type_overrides, discovered_label_names)
    root = source_root_for_path(src_path)
    lines, original_line_numbers = strip_noncode_definition_blocks_with_line_numbers(
        src_path.read_text(errors="ignore").splitlines()
    )
    is_equ_source = src_path.suffix.upper() == ".EQU"
    data_only_macros = collect_data_only_macro_names(root)
    macros = ccm.parse_macros(root)
    symbols = ccm.parse_set_symbols(root)
    symbols["DEBUG"] = 0
    force_function_names = {
        name
        for name, override in (effective_type_overrides or {}).items()
        if override.force_function
    }
    headers = parse_include_headers(lines)
    if own_header is not None:
        headers.append(own_header)
    headers.extend(sorted(collect_word_symbol_dependencies(lines, owner_headers, src_path.stem.upper())))
    headers = list(dict.fromkeys(headers))
    predeclared_header_names: set[str] = set()
    predeclared_define_names: set[str] = set()
    include_dir = root / "src" / "game"
    exported_header_names = collect_exported_header_names(include_dir)
    for header in headers:
        header_path = include_dir / header
        if not header_path.exists():
            continue
        header_text = header_path.read_text(errors="ignore")
        predeclared_header_names.update(collect_declared_header_names(header_text))
        predeclared_define_names.update(collect_defined_header_names(header_text))
    branch_targets = collect_branch_targets(lines)
    functions = collect_top_level_functions(lines, branch_targets, force_function_names, data_only_macros)
    attach_leading_context(lines, functions)
    assign_function_aliases(functions)
    for fn in functions:
        fn.line_numbers = [original_line_numbers[line_no - 1] for line_no in fn.line_numbers]
    symbol_table = collect_module_symbol_table(src_path, address_map, effective_type_overrides, predeclared_define_names)
    exported_function_names = set(collect_module_globl_function_names(lines, symbol_table))
    out: list[str] = []
    out.append('#include "../core/cpu.h"')
    out.append('#include "../core/machine.h"')
    for header in headers:
        out.append(f'#include "{header}"')
    out.append("")
    out.append("/*")
    out.append(f" * Source module: asm/{src_path.name}")
    out.append(" */")
    local_prototypes = render_local_function_prototypes(functions, exported_header_names, exported_function_names)
    if local_prototypes:
        out.append("")
        out.extend(local_prototypes)
    local_define_declarations = render_local_define_declarations(symbol_table)
    if local_define_declarations:
        out.append("")
        out.extend(local_define_declarations)
    external_declarations = render_external_symbol_declarations(
        lines,
        functions,
        symbol_table,
        global_symbol_table,
        predeclared_header_names,
    )
    if external_declarations:
        out.append("")
        out.extend(external_declarations)
    local_variable_declarations = render_local_variable_declarations(
        lines,
        symbol_table,
        exported_header_names,
        predeclared_header_names,
        data_only_macros,
    )
    if local_variable_declarations:
        out.append("")
        out.extend(local_variable_declarations)
    out.append("")

    rendered_count = 0
    seen_names: set[str] = set()
    cursor = 0
    saw_any_rendered_body = False

    def append_top_level_segment(start: int, end: int, seen_function_context: bool) -> None:
        nonlocal saw_any_rendered_body
        if start >= end:
            return
        segment = render_top_level_items(
            lines[start:end],
            [],
            branch_targets,
            address_map,
            src_path.stem.upper(),
            symbol_table,
            global_symbol_table,
            effective_type_overrides,
            strip_asm_comment_markers=is_equ_source,
            emit_set_asm_comments=is_equ_source,
            emit_set_defines=not is_equ_source,
            render_overrides=render_overrides,
            data_only_macros=data_only_macros,
            macros=macros,
            symbols=symbols,
            exported_header_names=exported_header_names,
            initial_seen_function_lines=seen_function_context,
            line_number_offset=start,
        )
        if not segment:
            return
        if saw_any_rendered_body:
            out.append("")
        out.extend(segment)
        saw_any_rendered_body = True

    for fn in functions:
        append_top_level_segment(cursor, fn.start_index, rendered_count > 0)
        ident = sanitize_identifier(fn.name)
        if ident in seen_names:
            cursor = fn.end_index + 1
            continue
        seen_names.add(ident)
        if saw_any_rendered_body:
            out.append("")
        out.extend(render_function(fn, instruction_addresses, exported_header_names, exported_function_names))
        saw_any_rendered_body = True
        rendered_count += 1
        cursor = fn.end_index + 1

    append_top_level_segment(cursor, len(lines), rendered_count > 0)

    if saw_any_rendered_body:
        out.append("")

    if rendered_count == 0 and not saw_any_rendered_body:
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
    code, _comment = split_comment(raw)
    _label, remainder = split_flush_left_label(code)
    if _label is not None:
        return remainder
    return raw


def collect_defined_data_symbols(
    lines: list[str],
    data_only_macros: frozenset[str] | None = None,
    macros: dict[str, ccm.MacroDef] | None = None,
    symbols: dict[str, int] | None = None,
) -> set[str]:
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
        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx, branch_targets, data_only_macros, macros, symbols)
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

def collect_module_symbol_table(
    src_path: Path,
    address_map: dict[str, int],
    type_overrides: dict[str, TypeOverride] | None = None,
    predefined_define_names: set[str] | None = None,
) -> dict[str, SymbolInfo]:
    lines = strip_noncode_definition_blocks(src_path.read_text(errors="ignore").splitlines())
    module = src_path.stem.upper()
    root = source_root_for_path(src_path)
    data_only_macros = collect_data_only_macro_names(source_root_for_path(src_path))
    macros = ccm.parse_macros(root)
    symbols = ccm.parse_set_symbols(root)
    symbols["DEBUG"] = 0
    symbol_table: dict[str, SymbolInfo] = {}
    predefined_symbols = {
        name: SymbolInfo(name=name, kind="define", module=module, expr=name)
        for name in (predefined_define_names or set())
    }
    resolution_symbols = ChainMap(symbol_table, predefined_symbols)
    word_variables: list[WordVariable] = []
    force_function_names = {
        name
        for name, override in (type_overrides or {}).items()
        if override.force_function
    }

    branch_targets = collect_branch_targets(lines)
    functions = collect_top_level_functions(lines, branch_targets, force_function_names, data_only_macros)
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
                apply_type_override(infer_storage_symbol(label, parse_storage_size(rest), module, type_overrides), type_overrides),
            )
            idx += 1
            continue

        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx, branch_targets, data_only_macros, macros, symbols)
        if standalone_data is not None:
            if is_omitted_symbol(standalone_data.name, type_overrides):
                idx = next_idx
                continue
            directive_lower = standalone_data.directive.lower()
            if directive_lower in {".word", ".intdata", ".float"}:
                numeric_var = standalone_numeric_variable(standalone_data)
                if directive_lower == ".word":
                    word_variables.append(
                        WordVariable(
                            name=standalone_data.name,
                            values=numeric_var.values,
                            asm_lines=filter_renderable_asm_lines(standalone_data.asm_lines),
                        )
                    )
                symbol_table.setdefault(
                    standalone_data.name,
                    apply_type_override(
                        infer_numeric_symbol(
                            NumericVariable(
                                name=numeric_var.name,
                                directive=numeric_var.directive,
                                values=numeric_var.values,
                                asm_lines=filter_renderable_asm_lines(numeric_var.asm_lines),
                            ),
                            module,
                            resolution_symbols,
                            type_overrides,
                        ),
                        type_overrides,
                    ),
                )
                if standalone_data.inner_labels:
                    base_symbol = symbol_table.get(standalone_data.name)
                    if base_symbol is not None and base_symbol.expr is None:
                        base_symbol.expr = "0"
                    for inner_name, offset in standalone_data.inner_labels:
                        symbol_table.setdefault(
                            inner_name,
                            SymbolInfo(name=inner_name, kind="define", module=module, expr=str(offset)),
                        )
            elif directive_lower == ".string":
                c_string = standalone_string_value(standalone_data)
                if c_string is not None:
                    symbol_table.setdefault(
                        standalone_data.name,
                        apply_type_override(infer_string_symbol(standalone_data.name, module), type_overrides),
                    )
            elif directive_lower == ".sptrtable":
                entry_count = sum(1 for line in standalone_data.asm_lines[1:] if parse_sptr_row(line) is not None)
                symbol_table.setdefault(
                    standalone_data.name,
                    apply_type_override(
                        SymbolInfo(
                            name=standalone_data.name,
                            kind="variable",
                            module=module,
                            c_type="const char *",
                            array_expr=str(entry_count),
                        ),
                        type_overrides,
                    ),
                )
            elif directive_lower == ".macrodata":
                symbol_table.setdefault(
                    standalone_data.name,
                    apply_type_override(SymbolInfo(name=standalone_data.name, kind="variable", module=module, c_type="int"), type_overrides),
                )
            elif directive_lower in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
                symbol_table.setdefault(
                    standalone_data.name,
                    apply_type_override(
                        infer_storage_symbol(standalone_data.name, parse_storage_size(standalone_data.rest), module, type_overrides),
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

        idx += 1

    for word_var in word_variables:
        existing = symbol_table.get(word_var.name)
        if existing is None or existing.kind != "variable":
            continue
        refined = apply_type_override(
            infer_word_symbol(word_var, module, resolution_symbols, type_overrides),
            type_overrides,
        )
        if refined.kind == "define":
            symbol_table[word_var.name] = refined
            continue
        if refined.kind != "variable":
            continue
        if existing.c_type == "int" and refined.c_type != "int":
            existing.c_type = refined.c_type
        if existing.array_expr is None and refined.array_expr is not None:
            existing.array_expr = refined.array_expr

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


def build_global_symbol_table(
    asm_dir: Path,
    address_map: dict[str, int],
    type_overrides: dict[str, TypeOverride] | None = None,
) -> dict[str, SymbolInfo]:
    symbol_table: dict[str, SymbolInfo] = {}
    for src_path in iter_module_paths(asm_dir, "*.ASM"):
        module_symbol_table = collect_module_symbol_table(src_path, address_map, type_overrides)
        for name, symbol in module_symbol_table.items():
            symbol_table.setdefault(name, symbol)
    return symbol_table


def main() -> int:
    generated_only = "--generated-only" in sys.argv[1:]
    root = Path(__file__).resolve().parents[2]
    asm_dir = root / "asm"
    generated_dir = root / "src" / "generated"
    reference_include_dir = root / "src" / "game"
    out_dir = generated_dir if generated_only else reference_include_dir
    include_dir = generated_dir if generated_only else reference_include_dir
    address_map = parse_address_map(root / "tools" / "ida" / "address.map")
    define_entries = parse_discovered_defines_file(root / "tools" / "ida" / "discovered_defines.txt")
    instruction_addresses_by_module = parse_instruction_addresses_file(
        root / "tools" / "ida" / "log" / "romlst_instruction_addresses.tsv"
    )
    type_overrides = parse_type_overrides_file(root / "tools" / "port" / "type-overrides.txt")
    render_overrides = parse_render_overrides_file(root / "tools" / "port" / "render-overrides.txt")
    label_types = parse_romlst_label_types(root / "tools" / "ida" / "log" / "romlst_labels.tsv")
    out_dir.mkdir(parents=True, exist_ok=True)
    generated_dir.mkdir(parents=True, exist_ok=True)
    include_dir.mkdir(parents=True, exist_ok=True)
    log_dir = root / "tools" / "port" / "log"
    log_dir.mkdir(parents=True, exist_ok=True)
    discovered_define_entries = sorted(
        list(define_entries.values()),
        key=lambda item: item.name.upper(),
    )
    discovered_define_names = {entry.name for entry in discovered_define_entries}
    resolve_generated_output_path(include_dir / "discovered_defines.h", generated_dir).write_text(
        render_discovered_defines_header(discovered_define_entries)
    )
    resolve_generated_output_path(include_dir / "discovered_labels.h", generated_dir).write_text(
        render_discovered_labels_header([], type_overrides)
    )
    resolve_generated_output_path(include_dir / "port.h", generated_dir).write_text(render_port_header())
    global_symbol_table = build_global_symbol_table(asm_dir, address_map, type_overrides)
    source_label_names: set[str] = set()
    for src_path in iter_module_paths(asm_dir, "*.ASM"):
        lines = src_path.read_text(errors="ignore").splitlines()
        source_label_names.update(collect_source_label_names(lines))
    existing_macro_names = collect_existing_macro_names(reference_include_dir)
    blocked_discovered_label_names = (
        set(global_symbol_table)
        | source_label_names
        | existing_macro_names
        | discovered_define_names
        | {"NULL"}
    )

    for src_path in iter_module_paths(asm_dir, "*.EQU"):
        banner_comments, globls, sets = parse_equ_file(src_path)
        out_path = resolve_generated_output_path(include_dir / (src_path.stem.lower() + ".h"), generated_dir)
        out_path.write_text(render_equ_header(src_path, banner_comments, globls, sets, global_symbol_table))

    storage_defines_by_module: dict[str, list[StorageVariable]] = {}
    owner_headers: dict[str, str] = {}
    skipped_data_symbols: set[str] = set()
    discovered_label_names: set[str] = set()
    for skipped_name in SKIPPED_ASM_MODULES:
        for stale_path in (
            include_dir / f"{skipped_name.lower()}.h",
            out_dir / f"{skipped_name.lower()}.c",
            generated_dir / f"{skipped_name.lower()}.h",
            generated_dir / f"{skipped_name.lower()}.c",
        ):
            if stale_path.exists():
                stale_path.unlink()

    for src_path in iter_module_paths(asm_dir, "*.ASM"):
        defines = collect_module_storage_defines(src_path, address_map, type_overrides)
        module_lines = src_path.read_text(errors="ignore").splitlines()
        module_symbol_table = collect_module_symbol_table(src_path, address_map, type_overrides)
        exported_functions = collect_module_globl_function_names(module_lines, module_symbol_table)
        header_name = storage_header_name(src_path.stem, include_dir)
        if not defines and not exported_functions:
            continue
        if defines:
            storage_defines_by_module[src_path.stem.upper()] = defines
        for entry in defines:
            owner_headers[entry.name] = header_name
        for name in exported_functions:
            owner_headers[name] = header_name
        header_path = resolve_generated_output_path(include_dir / header_name, generated_dir)
        rendered = render_storage_header(src_path, defines, type_overrides, exported_functions)
        if header_path.exists():
            existing = header_path.read_text()
            if f"Generated from asm/{src_path.stem}.EQU" in existing:
                rendered = merge_header_bodies(existing, rendered)
        header_path.write_text(rendered)
        obsolete_defs_path = include_dir / f"{src_path.stem.lower()}_defs.h"
        if obsolete_defs_path.exists():
            obsolete_defs_path.unlink()

    for src_path in iter_module_paths(asm_dir, "*.ASM"):
        lines = src_path.read_text(errors="ignore").splitlines()
        root = source_root_for_path(src_path)
        data_only_macros = collect_data_only_macro_names(root)
        macros = ccm.parse_macros(root)
        symbols = ccm.parse_set_symbols(root)
        symbols["DEBUG"] = 0
        missing_data_symbols = collect_referenced_data_symbols(lines, label_types) - collect_defined_data_symbols(
            lines,
            data_only_macros,
            macros,
            symbols,
        )
        skipped_data_symbols.update(missing_data_symbols)
        mapped_missing_data_symbols = {
            name
            for name in missing_data_symbols
            if name in address_map and name not in blocked_discovered_label_names
        }
        discovered_label_names.update(mapped_missing_data_symbols)
        discovered_header_needed = bool(collect_referenced_define_symbols(lines, discovered_define_names))
        discovered_labels_needed = bool(mapped_missing_data_symbols)
        out_path = resolve_generated_output_path(out_dir / (src_path.stem.lower() + ".c"), generated_dir)
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
                render_overrides,
                global_symbol_table,
                mapped_missing_data_symbols,
            )
        )

    discovered_label_entries = [
        LabelEntry(name=name, addr=address_map[name])
        for name in sorted(discovered_label_names, key=str.upper)
    ]
    resolve_generated_output_path(include_dir / "discovered_labels.h", generated_dir).write_text(
        render_discovered_labels_header(discovered_label_entries, type_overrides)
    )

    skipped_log = log_dir / "skipped_data_symbols.txt"
    skipped_rows = sorted(skipped_data_symbols)
    skipped_log.write_text("".join(f"{name}\n" for name in skipped_rows))
    print(f"generated {len(iter_module_paths(asm_dir, '*.ASM'))} C modules in {out_dir.relative_to(root)}")
    print(f"logged {len(skipped_rows)} skipped data symbols to {skipped_log.relative_to(root)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

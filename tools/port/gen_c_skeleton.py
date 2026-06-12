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
    lines: list[str] = field(default_factory=list)
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


@dataclass
class DefineEntry:
    name: str
    expr: str
    module: str


@dataclass
class StringVariable:
    name: str
    c_string: str


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


def storage_declaration(name: str, size_expr: str, is_extern: bool = False) -> str:
    ident = sanitize_identifier(name)
    expr = convert_expr(size_expr).strip()
    prefix = "extern " if is_extern else ""
    if expr in {"", "0", "1"}:
        return f"{prefix}int {ident};"
    return f"{prefix}int {ident}[{expr}];"


def collect_module_storage_defines(src_path: Path, address_map: dict[str, int]) -> list[StorageVariable]:
    lines = src_path.read_text(errors="ignore").splitlines()
    module = src_path.stem.upper()
    out: list[StorageVariable] = []
    seen: set[str] = set()
    for raw in lines:
        code, _comment = split_comment(raw)
        storage_match = STANDALONE_STORAGE_FULL_RE.match(code)
        if not storage_match:
            continue
        _directive, storage_name, rest = storage_match.groups()
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


def render_conditional_line(raw: str) -> str | None:
    match = IF_RE.match(raw)
    if match:
        return f"#if {convert_expr(match.group(1))}"
    if ELSE_RE.match(raw):
        return "#else"
    if ENDIF_RE.match(raw):
        return "#endif"
    return None


def render_asm_comment(raw: str) -> str | None:
    stripped = raw.rstrip()
    if not stripped:
        return None
    if stripped.lstrip().startswith("*") or stripped.lstrip().startswith(";"):
        return f"// {stripped}"
    return None


def is_context_line(raw: str) -> bool:
    if not raw.strip():
        return True
    if render_asm_comment(raw) is not None:
        return True
    return False


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
    if not is_flush_left(raw):
        return False
    if DATA_LABEL_RE.match(raw):
        return True
    if parse_sptr_label(raw) is not None:
        return True
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


def collect_top_level_functions(lines: list[str], branch_targets: set[str]) -> list[FunctionBlock]:
    functions: list[FunctionBlock] = []
    current: FunctionBlock | None = None
    seen_separator = False

    for idx, raw in enumerate(lines):
        if SEPARATOR_RE.match(raw.strip()):
            seen_separator = True
            current = None
            continue

        standalone_data, _next_idx = collect_standalone_labeled_data(lines, idx)
        if standalone_data is not None:
            current = None
            seen_separator = False
            continue

        label_match = LABEL_RE.match(raw)
        if label_match:
            label = label_match.group(1)
            start_new = False

            if current is None:
                start_new = True
            elif seen_separator:
                start_new = True
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
            current.end_index = idx
            continue

        if current is not None and is_top_level_data_line(raw):
            current = None
            seen_separator = False
            continue

        inline_label_match = INLINE_LABEL_RE.match(raw)
        if inline_label_match:
            label = inline_label_match.group(1)
            if is_flush_left(raw) and not looks_like_instruction_token(label) and label in branch_targets and current is not None:
                current.labels.add(label)
                current.lines.append(f"{label}: {inline_label_match.group(2)}{inline_label_match.group(3)}")
                current.end_index = idx
                continue

        bare_label_match = BARE_LABEL_RE.match(raw)
        if bare_label_match:
            label = bare_label_match.group(1)
            if is_flush_left(raw) and label in branch_targets and current is not None:
                current.labels.add(label)
                current.lines.append(f"{label}:")
                current.end_index = idx
                continue

        if current is not None:
            current.lines.append(raw.rstrip())
            current.end_index = idx
        else:
            seen_separator = False

    return functions


def attach_leading_context(lines: list[str], functions: list[FunctionBlock]) -> None:
    for i in range(1, len(functions)):
        fn = functions[i]
        prev_end = functions[i - 1].end_index
        start = fn.start_index
        if start <= 0:
            continue

        j = start - 1
        while j > prev_end and is_context_line(lines[j]):
            j -= 1
        new_start = j + 1
        if new_start >= start:
            continue

        fn.start_index = new_start
        fn.lines = [line.rstrip() for line in lines[new_start:start]] + fn.lines


def render_function(fn: FunctionBlock) -> list[str]:
    out: list[str] = []
    fn_ident = sanitize_identifier(fn.name)
    out.append(f"void {fn_ident}(void)")
    out.append("{")

    emitted_any = False
    first_label_emitted = False
    for raw in fn.lines:
        asm_comment = render_asm_comment(raw)
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
                out.append(f"    // asm: {rest.lstrip().rstrip()}")
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
                out.append(f"    // asm: {code}")
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
        out.append(f"    // asm: {raw.rstrip()}")
        emitted_any = True

    if emitted_any:
        out.append(f'    TRACE_EVENT(&g_crusn_machine->trace, "function", "{fn.name}", 0, 0);')
    else:
        out.append("    /* no executable asm lines detected */")
    out.append("    UNIMPL();")
    out.append("}")
    return out


def render_word_variable(var: WordVariable) -> list[str]:
    out: list[str] = []
    if len(var.values) == 1 and var.name.endswith("I") and var.values[0] == var.name[:-1]:
        return out
    if var.asm_lines:
        out.append(f"/* asm: {var.name}\t{var.asm_lines[0].strip()} */")
        for extra_line in var.asm_lines[1:]:
            out.append(f"/* asm: \t{extra_line.strip()} */")
    ident = sanitize_identifier(var.name)
    if len(var.values) == 1:
        value = var.values[0].strip()
        rendered_value = convert_expr(value)
        if var.name.lower().endswith("i") and BRANCH_TARGET_RE.fullmatch(value) and parse_int_token(value) is None:
            out.append(f"#define {ident} {rendered_value}")
        else:
            out.append(f"int {ident} = {rendered_value};")
    else:
        structured_rows: list[str] = []
        if var.asm_lines:
            for asm_line in var.asm_lines:
                parsed_row = parse_numeric_data_line(asm_line, var.name)
                if parsed_row is None:
                    continue
                row_values, asm_comment = parsed_row
                rendered_values = ", ".join(convert_expr(value) for value in row_values)
                comment_text = asm_comment[1:].strip() if asm_comment.startswith(";") else asm_comment.strip()
                if comment_text:
                    structured_rows.append(f"    {rendered_values}, // {comment_text}")
                else:
                    structured_rows.append(f"    {rendered_values},")
        if structured_rows:
            out.append(f"int {ident}[] = {{")
            out.extend(structured_rows)
            out.append("};")
        else:
            values = ", ".join(convert_expr(value) for value in var.values)
            out.append(f"int {ident}[] = {{ {values} }};")
    return out


def render_set_define(entry: SetDefine) -> list[str]:
    expr = f"({entry.expr})" if needs_parens(entry.expr) else entry.expr
    return [f"#define {entry.name} {expr}"]


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


def render_string_variable(var: StringVariable) -> list[str]:
    ident = sanitize_identifier(var.name)
    return [f"const char *{ident} = {var.c_string};"]


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


def collect_standalone_labeled_data(lines: list[str], start_idx: int) -> tuple[StandaloneLabeledData | None, int]:
    raw = lines[start_idx]
    label_match = LABEL_RE.match(raw)
    if label_match:
        label, rest = label_match.groups()
        rest_code, _rest_comment = split_comment(rest)
        if rest_code.strip():
            return None, start_idx
    else:
        raw_code, _raw_comment = split_comment(raw)
        bare_label_match = BARE_LABEL_RE.match(raw_code)
        if not bare_label_match or not is_flush_left(raw):
            return None, start_idx
        label = bare_label_match.group(1)
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
            _directive, values = numeric_directive
            asm_lines.append(next_raw.rstrip())
            numeric_values.extend(values)
            saw_numeric_directive = True
            next_idx += 1
            continue
        directive_match = re.match(
            r"^\s*((?:\.\w+)|EQU|equ|fbss|pbss|hibss|lobss|phibss)\b(.*)$",
            stripped,
        )
        if directive_match is None:
            break
        directive, directive_rest = directive_match.groups()
        if saw_numeric_directive:
            return StandaloneLabeledData(label, ".intdata", ",".join(numeric_values), asm_lines), next_idx
        asm_lines.append(next_raw.rstrip())
        return StandaloneLabeledData(label, directive, directive_rest, asm_lines), next_idx + 1
    if saw_numeric_directive:
        return StandaloneLabeledData(label, ".intdata", ",".join(numeric_values), asm_lines), next_idx
    return None, start_idx


def render_top_level_items(
    lines: list[str],
    functions: list[FunctionBlock],
    address_map: dict[str, int],
    module: str,
) -> list[str]:
    function_line_indexes: set[int] = set()
    for fn in functions:
        if fn.start_index < 0 or fn.end_index < 0:
            continue
        function_line_indexes.update(range(fn.start_index, fn.end_index + 1))

    out: list[str] = []
    emitted_sets: set[str] = set()
    idx = 0
    while idx < len(lines):
        raw = lines[idx]
        if idx in function_line_indexes:
            idx += 1
            continue

        asm_comment = render_asm_comment(raw)
        if asm_comment is not None:
            out.append(asm_comment)
            idx += 1
            continue

        conditional = render_conditional_line(raw)
        if conditional is not None:
            out.append(conditional)
            idx += 1
            continue

        code, _comment = split_comment(raw)
        set_match = SET_RE.match(code)
        if set_match:
            name, expr = set_match.groups()
            define_entry = SetDefine(name=name, expr=convert_expr(expr))
            if name in emitted_sets:
                out.append(f"#undef {name}")
            out.extend(render_set_define(define_entry))
            emitted_sets.add(name)
            idx += 1
            continue

        storage_match = STANDALONE_STORAGE_FULL_RE.match(code)
        if storage_match:
            directive, label, rest = storage_match.groups()
            out.extend(render_storage_variable(label, parse_storage_size(rest), f"{directive}\t{label}{rest}"))
            idx += 1
            continue

        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx)
        if standalone_data is not None:
            directive_lower = standalone_data.directive.lower()
            if directive_lower in {".word", ".intdata"}:
                values: list[str] = []
                if directive_lower == ".intdata":
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
                out.extend(
                    render_word_variable(
                        WordVariable(
                            name=standalone_data.name,
                            values=values,
                            asm_lines=[line for line in standalone_data.asm_lines if line.strip()],
                        )
                    )
                )
            elif directive_lower == ".string":
                c_string = parse_simple_string_operand(standalone_data.rest)
                if c_string is not None:
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
            idx += 1
            continue

        sptr_match = parse_sptr_label(raw)
        if sptr_match is not None:
            label, c_string = sptr_match
            out.extend(render_sptr_variable(label, c_string, raw))
            idx += 1
            continue

        data_match = DATA_LABEL_RE.match(raw)
        if not data_match:
            idx += 1
            continue

        label, directive, rest = data_match.groups()
        directive_lower = directive.lower()
        if directive_lower in {".bss", ".usect", "fbss", "pbss", "hibss", "lobss", "phibss"}:
            out.extend(render_storage_variable(label, parse_storage_size(rest), f"{directive}{rest}"))
            idx += 1
            continue
        if directive_lower == ".string":
            c_string = parse_simple_string_operand(rest)
            if c_string is not None:
                out.extend(render_string_variable(StringVariable(name=label, c_string=c_string)))
            idx += 1
            continue

        if directive_lower == ".word":
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
            resolved_values = {}
            for value in values:
                addr = lookup_address_map_symbol(address_map, value, module)
                if addr is not None:
                    resolved_values[value] = addr
            out.extend(
                render_word_variable(
                    WordVariable(name=label, values=values, asm_lines=asm_lines, resolved_values=resolved_values)
                )
            )
            idx = next_idx
            continue

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
) -> str:
    lines = src_path.read_text(errors="ignore").splitlines()
    headers = parse_include_headers(lines)
    if own_header is not None:
        headers.append(own_header)
    if discovered_header_needed:
        headers.append("discovered_defines.h")
    headers.extend(sorted(collect_word_symbol_dependencies(lines, owner_headers, src_path.stem.upper())))
    headers = list(dict.fromkeys(headers))
    branch_targets = collect_branch_targets(lines)
    functions = collect_top_level_functions(lines, branch_targets)
    attach_leading_context(lines, functions)
    top_level_items = render_top_level_items(
        lines,
        functions,
        address_map,
        src_path.stem.upper(),
    )

    out: list[str] = []
    out.append('#include "../../core/cpu.h"')
    out.append('#include "../../core/machine.h"')
    for header in headers:
        out.append(f'#include "../include/{header}"')
    out.append("")
    out.append("/*")
    out.append(f" * Source module: asm/{src_path.name}")
    out.append(" */")
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
        out.extend(render_function(fn))
        out.append("")
        rendered_count += 1

    if rendered_count == 0:
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
        standalone_data, next_idx = collect_standalone_labeled_data(lines, idx)
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


def main() -> int:
    root = Path(__file__).resolve().parents[2]
    asm_dir = root / "asm"
    out_dir = root / "src" / "game" / "modules"
    include_dir = root / "src" / "game" / "include"
    address_map = parse_address_map(root / "tools" / "ida" / "address.map")
    define_entries = parse_discovered_defines_file(root / "tools" / "ida" / "discovered_defines.txt")
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

    storage_defines_by_module: dict[str, list[StorageVariable]] = {}
    owner_headers: dict[str, str] = {}
    skipped_data_symbols: set[str] = set()
    for src_path in sorted(asm_dir.glob("*.ASM")):
        defines = collect_module_storage_defines(src_path, address_map)
        if not defines:
            continue
        storage_defines_by_module[src_path.stem.upper()] = defines
        header_name = storage_header_name(src_path.stem, include_dir)
        for entry in defines:
            owner_headers[entry.name] = header_name
        (include_dir / header_name).write_text(render_storage_header(src_path, defines))

    for src_path in sorted(asm_dir.glob("*.ASM")):
        lines = src_path.read_text(errors="ignore").splitlines()
        skipped_data_symbols.update(
            collect_referenced_data_symbols(lines, label_types) - collect_defined_data_symbols(lines)
        )
        discovered_header_needed = bool(collect_referenced_define_symbols(lines, discovered_define_names))
        out_path = out_dir / (src_path.stem.lower() + ".c")
        own_header = owner_headers.get(next(iter([d.name for d in storage_defines_by_module.get(src_path.stem.upper(), [])]), ""), None)
        if src_path.stem.upper() in storage_defines_by_module:
            own_header = storage_header_name(src_path.stem, include_dir)
        else:
            own_header = None
        out_path.write_text(render_module(src_path, address_map, owner_headers, own_header, discovered_header_needed))

    skipped_log = log_dir / "skipped_data_symbols.txt"
    skipped_rows = sorted(skipped_data_symbols)
    skipped_log.write_text("".join(f"{name}\n" for name in skipped_rows))
    print(f"generated {len(list(asm_dir.glob('*.ASM')))} C modules in {out_dir.relative_to(root)}")
    print(f"logged {len(skipped_rows)} skipped data symbols to {skipped_log.relative_to(root)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

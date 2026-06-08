#!/usr/bin/env python3
"""Generate first-pass C scaffolds from asm modules."""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path


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
STANDALONE_STORAGE_RE = re.compile(r"^\s*(?:\.bss|\.usect|fbss|pbss|hibss|lobss|phibss)\b", re.IGNORECASE)
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


@dataclass
class StringVariable:
    name: str
    c_string: str


@dataclass
class StorageDefine:
    name: str
    addr: int
    module: str
    asm_line: str


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


def lookup_address_map_symbol(address_map: dict[str, int], name: str, module: str) -> int | None:
    if name in address_map:
        return address_map[name]
    scoped = f"{name}@{module.upper()}"
    if scoped in address_map:
        return address_map[scoped]
    scoped = f"{name}@{module}"
    return address_map.get(scoped)


def collect_module_storage_defines(src_path: Path, address_map: dict[str, int]) -> list[StorageDefine]:
    lines = src_path.read_text(errors="ignore").splitlines()
    module = src_path.stem.upper()
    out: list[StorageDefine] = []
    seen: set[str] = set()
    for raw in lines:
        code, _comment = split_comment(raw)
        storage_match = STANDALONE_STORAGE_LABEL_RE.match(code)
        if not storage_match:
            continue
        storage_name = storage_match.group(1)
        if storage_name in seen:
            continue
        storage_addr = lookup_address_map_symbol(address_map, storage_name, module)
        if storage_addr is None:
            continue
        out.append(StorageDefine(storage_name, storage_addr, module, raw.rstrip()))
        seen.add(storage_name)
    return out


def render_storage_header(src_path: Path, defines: list[StorageDefine]) -> str:
    guard = sanitize_identifier(src_path.stem).upper() + "_H"
    out: list[str] = []
    out.append(f"#ifndef {guard}")
    out.append(f"#define {guard}")
    out.append("")
    out.append(f"/* Generated from asm/{src_path.name}. */")
    out.append("")
    for entry in defines:
        out.append(f"// asm: {entry.asm_line}")
        out.append(f"#define {sanitize_identifier(entry.name)} 0x{entry.addr:08X}")
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

        label_match = LABEL_RE.match(raw)
        if label_match:
            label = label_match.group(1)
            rest = label_match.group(2)
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

            if rest.strip():
                current.lines.append(" " + rest.lstrip())
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
    if var.asm_lines:
        out.append(f"/* asm: {var.name}\t{var.asm_lines[0].strip()} */")
        for extra_line in var.asm_lines[1:]:
            out.append(f"/* asm: \t{extra_line.strip()} */")
    ident = sanitize_identifier(var.name)
    if len(var.values) == 1:
        out.append(f"int {ident} = (int)({convert_expr(var.values[0])});")
    else:
        values = ", ".join(convert_expr(value) for value in var.values)
        out.append(f"int {ident}[] = {{ {values} }};")
    return out


def render_set_define(entry: SetDefine) -> list[str]:
    expr = f"({entry.expr})" if needs_parens(entry.expr) else entry.expr
    return [f"#define {entry.name} {expr}"]


def render_string_variable(var: StringVariable) -> list[str]:
    ident = sanitize_identifier(var.name)
    return [f"const char {ident}[] = {var.c_string};"]


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
            if name in emitted_sets:
                out.append(f"#undef {name}")
            out.extend(render_set_define(SetDefine(name=name, expr=convert_expr(expr))))
            emitted_sets.add(name)
            idx += 1
            continue

        if STANDALONE_STORAGE_RE.match(code):
            idx += 1
            continue

        if not is_flush_left(raw):
            idx += 1
            continue
        data_match = DATA_LABEL_RE.match(raw)
        if not data_match:
            idx += 1
            continue

        label, directive, rest = data_match.groups()
        directive_lower = directive.lower()
        if directive_lower == ".string":
            operands, _comment = split_comment(rest.strip())
            string_match = SIMPLE_STRING_RE.match(operands)
            if string_match:
                out.extend(render_string_variable(StringVariable(name=label, c_string=string_match.group(1))))
            idx += 1
            continue

        if directive_lower == ".word":
            asm_lines = [f".word{rest}"]
            values: list[str] = []
            operands, _comment = split_comment(rest.strip())
            if operands:
                values.extend(part.strip() for part in operands.split(",") if part.strip())

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
                if next_stripped.lower().startswith(".word"):
                    asm_lines.append(next_stripped)
                    cont_operands = next_stripped[5:].strip()
                    if cont_operands:
                        values.extend(part.strip() for part in cont_operands.split(",") if part.strip())
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


def render_module(src_path: Path, address_map: dict[str, int], owner_headers: dict[str, str], own_header: str | None) -> str:
    lines = src_path.read_text(errors="ignore").splitlines()
    headers = parse_include_headers(lines)
    if own_header is not None:
        headers.append(own_header)
    headers.extend(sorted(collect_word_symbol_dependencies(lines, owner_headers, src_path.stem.upper())))
    headers = list(dict.fromkeys(headers))
    branch_targets = collect_branch_targets(lines)
    functions = collect_top_level_functions(lines, branch_targets)
    attach_leading_context(lines, functions)
    top_level_items = render_top_level_items(lines, functions, address_map, src_path.stem.upper())

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


def main() -> int:
    root = Path(__file__).resolve().parents[2]
    asm_dir = root / "asm"
    out_dir = root / "src" / "game" / "modules"
    include_dir = root / "src" / "game" / "include"
    address_map = parse_address_map(root / "tools" / "ida" / "address.map")
    out_dir.mkdir(parents=True, exist_ok=True)
    include_dir.mkdir(parents=True, exist_ok=True)

    storage_defines_by_module: dict[str, list[StorageDefine]] = {}
    owner_headers: dict[str, str] = {}
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
        out_path = out_dir / (src_path.stem.lower() + ".c")
        own_header = owner_headers.get(next(iter([d.name for d in storage_defines_by_module.get(src_path.stem.upper(), [])]), ""), None)
        if src_path.stem.upper() in storage_defines_by_module:
            own_header = storage_header_name(src_path.stem, include_dir)
        else:
            own_header = None
        out_path.write_text(render_module(src_path, address_map, owner_headers, own_header))

    print(f"generated {len(list(asm_dir.glob('*.ASM')))} C modules in {out_dir.relative_to(root)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3

# mame crusnusa -window -sound none -debug -log -skip_gameinfo -debugscript tools/mame/output/mame_validate_breakpoints.txt
from __future__ import annotations

import argparse
import collections
import os
import pathlib
import re
from dataclasses import dataclass
from typing import DefaultDict, Dict, Iterable, List, Optional

ROOT = pathlib.Path(__file__).resolve().parents[2]

VALIDATE_ARG_RE = re.compile(
    r'(?:MAME_ASSERT_ARG|mame_validate_arg)(?:_sym)?\(\s*"(?P<label>[^"]+)"\s*,\s*(?P<expr>[^)]+)\)\s*;'
)
VALIDATE_ARG_FLOAT_RE = re.compile(
    r'(?:MAME_ASSERT_ARG_FLOAT|mame_validate_arg_float)\(\s*"(?P<label>[^"]+)"\s*,\s*(?P<expr>[^)]+)\)\s*;'
)
VALIDATE_FUNCTION_ENTRY_RE = re.compile(
    r'(?:MAME_ASSERT_FUNCTION_ENTRY|MAME_VALIDATE_FUNCTION_ENTRY|mame_validate_function_entry)\(\s*\)\s*;'
)
VALIDATE_EXIT_RE = re.compile(r'(?:MAME_VALIDATOR_EXIT|MAME_VALIDATE_EXIT|mame_validate_exit)\(\s*\)\s*;')
VALIDATE_REGION_AT_ADDR_RE = re.compile(
    r'(?:MAME_ASSERT_REGION_AT_ADDR|mame_validate_region_at_addr)\(\s*(?P<instr_addr>[^,]+)\s*,\s*"(?P<label>[^"]+)"\s*,\s*(?P<region_addr>[^,]+)\s*,\s*(?P<ptr>[^,]+)\s*,\s*(?P<word_count>[^)]+)\)\s*;'
)
ADDRESS_MAP_RE = re.compile(r"^\s*[0-9A-Fa-f]{4}:([0-9A-Fa-f]{8})\s+(.+?)\s*$")
DEFINE_RE = re.compile(r"^\s*#define\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s+(?P<value>.+?)\s*$", re.MULTILINE)
ENUM_VALUE_RE = re.compile(r"^\s*(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?P<value>[^,]+)\s*,?\s*$", re.MULTILINE)
IDENT_RE = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\b")
HEX_RE = re.compile(r"\b0[xX][0-9A-Fa-f]+\b")
FUNCTION_DEF_RE = re.compile(
    r"^\s*.*?(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*\([^;{}]*\)\s*\{\s*$"
)
MACRO_NAME_RE = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\s*\(")


@dataclass(frozen=True)
class BreakpointEntry:
    label: str
    variable_name: str
    variable_address: int
    instruction_address: int
    array_length: Optional[int]
    source_path: pathlib.Path
    source_line: int
    command_override: Optional[str] = None
    action_override: Optional[str] = None

    def _format_source_suffix(self) -> str:
        return f", {self.source_path.name}:{self.source_line}"

    def _format_scalar_command(self) -> str:
        return f'logerror "validate {self.label}: 0x%08X{self._format_source_suffix()}\\n", d@{self.variable_address:08X}'

    def _format_small_array_command(self) -> str:
        assert self.array_length is not None
        fmt = " ".join(["0x%08X"] * self.array_length)
        reads = ", ".join(
            f"d@{self.variable_address + index:08X}" for index in range(self.array_length)
        )
        return f'logerror "validate {self.variable_name}[{self.array_length}]: {fmt}{self._format_source_suffix()}\\n", {reads}'

    def _format_large_array_command(self, save_index: int) -> str:
        assert self.array_length is not None
        dump_name = f"{self.instruction_address:08X}-{save_index}.bin"
        return (
            f'save {dump_name}, {self.variable_address:08X}, 0x{self.array_length:X}; '
            f'logerror "validate {self.variable_name}: file={dump_name}{self._format_source_suffix()}\\n"'
        )

    def format_command(self, save_index: int = 0) -> str:
        if self.command_override is not None:
            return self.command_override
        if self.array_length is None:
            return self._format_scalar_command()
        if self.array_length <= 5:
            return self._format_small_array_command()
        return self._format_large_array_command(save_index)

    def format_mame(self, action: str = "g", save_index: int = 0) -> str:
        command = self.format_command(save_index=save_index)
        effective_action = self.action_override or action
        return f"bpset {self.instruction_address:08X}, 1, {{ {command}; {effective_action} }}"


def parse_address_map(map_path: pathlib.Path) -> Dict[str, int]:
    out: Dict[str, int] = {}
    if not map_path.exists():
        return out
    for line in map_path.read_text(errors="ignore").splitlines():
        match = ADDRESS_MAP_RE.match(line)
        if not match:
            continue
        out.setdefault(match.group(2), int(match.group(1), 16))
    return out


def lookup_label_address(label: str, address_map: Dict[str, int]) -> Optional[int]:
    if label in address_map:
        return address_map[label]

    offset_match = re.fullmatch(r"(?P<base>.+)\+(?P<offset>\d+)", label)
    if offset_match:
        base_address = lookup_label_address(offset_match.group("base"), address_map)
        if base_address is not None:
            return base_address + int(offset_match.group("offset"))

    folded = {name.upper(): ea for name, ea in address_map.items()}
    return folded.get(label.upper())


def find_containing_function_name(lines: List[str], start_index: int) -> Optional[str]:
    for index in range(start_index, -1, -1):
        match = FUNCTION_DEF_RE.match(lines[index])
        if match:
            return match.group("name")
    return None


def strip_cpp_line_comment(line: str) -> str:
    return line.split("//", 1)[0]


def find_matching_paren(text: str, open_index: int) -> Optional[int]:
    depth = 0
    in_string = False
    escape = False

    for index in range(open_index, len(text)):
        ch = text[index]
        if in_string:
            if escape:
                escape = False
            elif ch == "\\":
                escape = True
            elif ch == '"':
                in_string = False
            continue

        if ch == '"':
            in_string = True
        elif ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                return index

    return None


def split_top_level_args(text: str) -> List[str]:
    args: List[str] = []
    start = 0
    paren_depth = 0
    bracket_depth = 0
    brace_depth = 0
    in_string = False
    escape = False

    for index, ch in enumerate(text):
        if in_string:
            if escape:
                escape = False
            elif ch == "\\":
                escape = True
            elif ch == '"':
                in_string = False
            continue

        if ch == '"':
            in_string = True
        elif ch == "(":
            paren_depth += 1
        elif ch == ")":
            paren_depth -= 1
        elif ch == "[":
            bracket_depth += 1
        elif ch == "]":
            bracket_depth -= 1
        elif ch == "{":
            brace_depth += 1
        elif ch == "}":
            brace_depth -= 1
        elif ch == "," and paren_depth == 0 and bracket_depth == 0 and brace_depth == 0:
            args.append(text[start:index].strip())
            start = index + 1

    tail = text[start:].strip()
    if tail:
        args.append(tail)
    return args


def parse_macro_invocation_args(
    line: str, macro_names: List[str], expected_counts: List[int]
) -> Optional[List[str]]:
    stripped = strip_cpp_line_comment(line)

    for macro_name in macro_names:
        match = re.search(rf"\b{re.escape(macro_name)}\s*\(", stripped)
        if not match:
            continue

        open_index = stripped.find("(", match.start())
        close_index = find_matching_paren(stripped, open_index)
        if close_index is None:
            continue

        trailing = stripped[close_index + 1 :].strip()
        if trailing != ";":
            continue

        args = split_top_level_args(stripped[open_index + 1 : close_index])
        if len(args) not in expected_counts:
            raise ValueError(
                f"could not parse {macro_name} argument list from line: {line.strip()}"
            )
        return args

    return None


def parse_explicit_value_validation(
    line: str, macro_names: List[str], expected_counts: Optional[List[int]] = None
) -> Optional[tuple[str, str, str]]:
    if expected_counts is None:
        expected_counts = [3]

    args = parse_macro_invocation_args(line, macro_names, expected_counts)
    if args is None:
        return None

    addr_expr, label_expr, value_expr = args[:3]
    label_match = re.fullmatch(r'"([^"]+)"', label_expr.strip())
    if label_match is None:
        raise ValueError(f"expected quoted label/expression argument, got {label_expr!r}")

    value_expr = value_expr.strip()
    if not value_expr.startswith("&"):
        raise ValueError(f"expected address-of value expression, got {value_expr!r}")

    return addr_expr.strip(), label_match.group(1), value_expr[1:].strip()


def lookup_containing_function_instruction_address(
    lines: List[str], index: int, path: pathlib.Path, address_map: Dict[str, int], macro_name: str
) -> tuple[str, int]:
    function_name = find_containing_function_name(lines, index)
    if function_name is None:
        raise ValueError(f"{path}:{index + 1}: could not find containing function for {macro_name}")

    instruction_address = lookup_label_address(function_name, address_map)
    if instruction_address is None:
        raise ValueError(f"{path}:{index + 1}: function {function_name!r} not found in address map")

    return function_name, instruction_address


def parse_constant_defines(source_root: pathlib.Path) -> Dict[str, int]:
    defines: Dict[str, int] = {}
    for path in sorted(source_root.rglob("*.[ch]")):
        text = path.read_text(errors="ignore")
        for match in DEFINE_RE.finditer(text):
            name = match.group("name")
            value = strip_cpp_line_comment(match.group("value")).strip()
            resolved = evaluate_constant_expr(value, defines)
            if resolved is not None:
                defines[name] = resolved
        for match in ENUM_VALUE_RE.finditer(text):
            name = match.group("name")
            value = strip_cpp_line_comment(match.group("value")).strip()
            resolved = evaluate_constant_expr(value, defines)
            if resolved is not None:
                defines[name] = resolved
    return defines


def evaluate_constant_expr(expr: str, defines: Dict[str, int]) -> Optional[int]:
    text = expr.strip()
    if not text:
        return None
    if re.fullmatch(r"(?:0[xX][0-9A-Fa-f]+|\d+)", text):
        return int(text, 0)
    if not re.fullmatch(r"[A-Za-z0-9_xX()+\-*/<>\s]+", text):
        return None

    def replace_ident(match: re.Match[str]) -> str:
        token = match.group(0)
        if token.isdigit():
            return token
        if token not in defines:
            raise KeyError(token)
        return str(defines[token])

    try:
        substituted = HEX_RE.sub(lambda match: str(int(match.group(0), 0)), text)
        substituted = IDENT_RE.sub(replace_ident, substituted)
    except KeyError:
        return None

    if not re.fullmatch(r"[0-9()+\-*/<>\s]+", substituted):
        return None

    try:
        return int(eval(substituted, {"__builtins__": {}}, {}))
    except Exception:
        return None


def collect_breakpoints_for_file(
    path: pathlib.Path, address_map: Dict[str, int], defines: Optional[Dict[str, int]] = None
) -> List[BreakpointEntry]:
    lines = path.read_text(errors="ignore").splitlines()
    entries: List[BreakpointEntry] = []
    resolved_defines = defines or {}

    for index, line in enumerate(lines):
        match = VALIDATE_ARG_RE.search(strip_cpp_line_comment(line))
        if not match:
            continue

        function_name, instruction_address = lookup_containing_function_instruction_address(
            lines, index, path, address_map, "mame_validate_arg"
        )

        entries.append(
            BreakpointEntry(
                label=match.group("label"),
                variable_name=match.group("expr").strip(),
                variable_address=0,
                instruction_address=instruction_address,
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=(
                    f'logerror "validate {match.group("label")}: 0x%08X, {path.name}:{index + 1}\\n", {match.group("label").lower()}'
                ),
            )
        )

    for index, line in enumerate(lines):
        match = VALIDATE_ARG_FLOAT_RE.search(strip_cpp_line_comment(line))
        if not match:
            continue

        function_name, instruction_address = lookup_containing_function_instruction_address(
            lines, index, path, address_map, "mame_validate_arg_float"
        )

        entries.append(
            BreakpointEntry(
                label=f'{match.group("label")}F',
                variable_name=match.group("expr").strip(),
                variable_address=0,
                instruction_address=instruction_address,
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=(
                    f'logerror "validate {match.group("label")}F: 0x%08X, {path.name}:{index + 1}\\n", {match.group("label").lower()}f'
                ),
            )
        )

    for index, line in enumerate(lines):
        match = VALIDATE_FUNCTION_ENTRY_RE.search(strip_cpp_line_comment(line))
        if not match:
            continue

        function_name, instruction_address = lookup_containing_function_instruction_address(
            lines, index, path, address_map, "MAME_ASSERT_FUNCTION_ENTRY"
        )

        entries.append(
            BreakpointEntry(
                label=f"function {function_name}",
                variable_name=function_name,
                variable_address=0,
                instruction_address=instruction_address,
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=f'logerror "function {function_name}\\n"',
            )
        )

    for index, line in enumerate(lines):
        match = VALIDATE_EXIT_RE.search(strip_cpp_line_comment(line))
        if not match:
            continue

        function_name, instruction_address = lookup_containing_function_instruction_address(
            lines, index, path, address_map, "MAME_ASSERT_EXIT"
        )

        entries.append(
            BreakpointEntry(
                label="exit",
                variable_name=function_name,
                variable_address=0,
                instruction_address=instruction_address,
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=f'logerror "exit, {path.name}:{index + 1}\\n"',
                action_override="exit; g",
            )
        )

    for index, line in enumerate(lines):
        match = VALIDATE_REGION_AT_ADDR_RE.search(strip_cpp_line_comment(line))
        if not match:
            continue

        instruction_address = evaluate_constant_expr(match.group("instr_addr").strip(), resolved_defines)
        if instruction_address is None:
            raise ValueError(
                f"{path}:{index + 1}: could not resolve instruction address {match.group('instr_addr')!r}"
            )

        region_expr = match.group("region_addr").strip()
        variable_address = lookup_label_address(region_expr, address_map)
        if variable_address is None:
            variable_address = evaluate_constant_expr(region_expr, resolved_defines)
        if variable_address is None:
            raise ValueError(f"{path}:{index + 1}: could not resolve region address {region_expr!r}")

        word_count = evaluate_constant_expr(match.group("word_count").strip(), resolved_defines)
        if word_count is None:
            raise ValueError(
                f"{path}:{index + 1}: could not resolve word count {match.group('word_count')!r}"
            )

        entries.append(
            BreakpointEntry(
                label=match.group("label"),
                variable_name=match.group("label"),
                variable_address=variable_address,
                instruction_address=instruction_address,
                array_length=word_count,
                source_path=path,
                source_line=index + 1,
            )
        )

    for index, line in enumerate(lines):
        parsed = parse_explicit_value_validation(
            line,
            ["MAME_ASSERT_REG_FLOAT", "MAME_ASSERT_REG_FLOAT_WIGGLE", "MAME_ASSERT_REG_AT_ADDR_FLOAT", "mame_validate_reg_at_addr_float"],
            [3, 4],
        )
        if parsed is None:
            continue

        instruction_expr, reg_name, value_expr = parsed
        instruction_address = evaluate_constant_expr(instruction_expr, resolved_defines)
        if instruction_address is None:
            raise ValueError(f"{path}:{index + 1}: could not resolve instruction address {instruction_expr!r}")

        entries.append(
            BreakpointEntry(
                label=f"{reg_name}F",
                variable_name=value_expr,
                variable_address=0,
                instruction_address=instruction_address,
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=(
                    f'logerror "validate {reg_name}F: 0x%08X, {path.name}:{index + 1}\\n", {reg_name.lower()}f'
                ),
            )
        )

    for index, line in enumerate(lines):
        parsed = parse_explicit_value_validation(
            line,
            ["MAME_ASSERT_REG", "MAME_ASSERT_REG_WIGGLE", "MAME_ASSERT_REG_AT_ADDR", "mame_validate_reg_at_addr"],
            [3, 4],
        )
        if parsed is None:
            continue

        instruction_expr, reg_name, value_expr = parsed
        instruction_address = evaluate_constant_expr(instruction_expr, resolved_defines)
        if instruction_address is None:
            raise ValueError(f"{path}:{index + 1}: could not resolve instruction address {instruction_expr!r}")

        entries.append(
            BreakpointEntry(
                label=reg_name,
                variable_name=value_expr,
                variable_address=0,
                instruction_address=instruction_address,
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=(
                    f'logerror "validate {reg_name}: 0x%08X, {path.name}:{index + 1}\\n", {reg_name.lower()}'
                ),
            )
        )

    for index, line in enumerate(lines):
        parsed = parse_explicit_value_validation(line, ["MAME_ASSERT_MEM", "mame_validate_mem_at_addr"])
        if parsed is None:
            continue

        instruction_expr, mem_expr, value_expr = parsed
        instruction_address = evaluate_constant_expr(instruction_expr, resolved_defines)
        if instruction_address is None:
            raise ValueError(f"{path}:{index + 1}: could not resolve instruction address {instruction_expr!r}")

        entries.append(
            BreakpointEntry(
                label=mem_expr,
                variable_name=value_expr,
                variable_address=0,
                instruction_address=instruction_address,
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=(
                    f'logerror "validate {mem_expr}: 0x%08X, {path.name}:{index + 1}\\n", {mem_expr}'
                ),
            )
        )

    for index, line in enumerate(lines):
        parsed = parse_explicit_value_validation(line, ["MAME_ASSERT_MEM_FLOAT", "mame_validate_mem_at_addr_float"])
        if parsed is None:
            continue

        instruction_expr, mem_expr, value_expr = parsed
        instruction_address = evaluate_constant_expr(instruction_expr, resolved_defines)
        if instruction_address is None:
            raise ValueError(f"{path}:{index + 1}: could not resolve instruction address {instruction_expr!r}")

        entries.append(
            BreakpointEntry(
                label=f"{mem_expr}F",
                variable_name=value_expr,
                variable_address=0,
                instruction_address=instruction_address,
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=(
                    f'logerror "validate {mem_expr}F: 0x%08X, {path.name}:{index + 1}\\n", {mem_expr}'
                ),
            )
        )

    entries.sort(key=lambda entry: (entry.source_line, entry.label, entry.variable_name))
    return entries


def collect_breakpoints(source_root: pathlib.Path, address_map: Dict[str, int]) -> List[BreakpointEntry]:
    defines = parse_constant_defines(source_root)
    entries: List[BreakpointEntry] = []
    for path in sorted(source_root.rglob("*.c")):
        entries.extend(collect_breakpoints_for_file(path, address_map, defines))
    return entries


def render_output(entries: Iterable[BreakpointEntry]) -> str:
    entry_list = list(entries)
    save_counts: DefaultDict[int, int] = collections.defaultdict(int)
    grouped_entries: "collections.OrderedDict[int, List[BreakpointEntry]]" = collections.OrderedDict()
    for entry in entry_list:
        grouped_entries.setdefault(entry.instruction_address, []).append(entry)

    grouped_items = list(grouped_entries.items())
    rows = [
    ]
    for group_index, (instruction_address, group) in enumerate(grouped_items):
        commands: List[str] = []
        action = "g"
        for entry in group:
            save_index = save_counts[instruction_address]
            commands.append(entry.format_command(save_index=save_index))
            if entry.array_length is not None and entry.array_length > 5:
                save_counts[instruction_address] += 1
            if entry.action_override is not None:
                action = entry.action_override
        rows.append(f'bpset {instruction_address:08X}, 1, {{ {"; ".join(commands)}; {action} }}')
    if os.environ.get("CRUSN_VALIDATE_FORCE_FULL_ZSORT") == "1":
        rows.append('bpset 000071BC, 1, { d@0000C96F=1; g }')
    if os.environ.get("CRUSN_VALIDATE_SINGLE_FRAME") == "1":
        rows.append('bpset 00004BED, 1, { r0=1; g }')
    if os.environ.get("CRUSN_VALIDATE_CLEAR_WATER_R0") == "1":
        # WATER_INFINITY consumes R0 as a float even though its current value
        # came from unrelated integer work. Clear the full extended register
        # immediately before that multiply.
        rows.append('bpset 00008428, 1, { r0f=0; g }')
    if os.environ.get("CRUSN_VALIDATE_FREEZE_COUNTDOWN") == "1":
        # INT0 can occur at different points relative to process dispatch in
        # MAME and the portable loop. Keep timer-dependent process behavior
        # deterministic by presenting the same nonzero value at dispatch.
        rows.append('bpset 0000A89D, 1, { d@0000E634=4B; g }')
    rows.append('bpset 00008EE1, 1, { r0=0; g }')
    rows.append("")
    return "\n".join(rows) + "g"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate MAME debugger breakpoints for MAME validation call sites."
    )
    parser.add_argument(
        "--source-root",
        type=pathlib.Path,
        default=ROOT / "src",
        help="Root directory to scan for C sources (default: %(default)s)",
    )
    parser.add_argument(
        "--address-map",
        type=pathlib.Path,
        default=ROOT / "tools" / "ida" / "address.map",
        help="Path to tools/ida/address.map (default: %(default)s)",
    )
    parser.add_argument(
        "--output",
        type=pathlib.Path,
        default=ROOT / "tools" / "mame" / "output" / "mame_validate_breakpoints.txt",
        help="Output debugger command file (default: %(default)s)",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    address_map = parse_address_map(args.address_map)
    entries = collect_breakpoints(args.source_root, address_map)
    output = render_output(entries)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(output)
    print(f"wrote {len(entries)} breakpoint entries to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3

# mame crusnusa -window -sound none -debug -log -skip_gameinfo -debugscript tools/mame/output/mame_validate_word_breakpoints.txt
from __future__ import annotations

import argparse
import collections
import pathlib
import re
from dataclasses import dataclass
from typing import DefaultDict, Dict, Iterable, List, Optional

ROOT = pathlib.Path(__file__).resolve().parents[2]

VALIDATE_RE = re.compile(
    r'mame_validate_word\(\s*"(?P<label>[^"]+)"\s*,\s*&(?P<var>[A-Za-z_][A-Za-z0-9_]*)(?:\[(?P<index>\d+)\])?\s*\)\s*;'
)
VALIDATE_ARG_RE = re.compile(
    r'mame_validate_arg\(\s*"(?P<label>[^"]+)"\s*,\s*(?P<expr>[^)]+)\)\s*;'
)
VALIDATE_REGION_AT_ADDR_RE = re.compile(
    r'mame_validate_region_at_addr\(\s*(?P<instr_addr>[^,]+)\s*,\s*"(?P<label>[^"]+)"\s*,\s*(?P<region_addr>[^,]+)\s*,\s*(?P<ptr>[^,]+)\s*,\s*(?P<word_count>[^)]+)\)\s*;'
)
VALIDATE_REG_AT_ADDR_RE = re.compile(
    r'mame_validate_reg_at_addr\(\s*(?P<addr>0x[0-9A-Fa-f]+|\d+)\s*,\s*"(?P<reg>[^"]+)"\s*,\s*&(?P<var>[A-Za-z_][A-Za-z0-9_]*)\s*\)\s*;'
)
ASM_ADDR_RE = re.compile(r"^\s*//\s*asm\s+(?P<addr>[0-9A-Fa-f]{8}):")
ADDRESS_MAP_RE = re.compile(r"^\s*[0-9A-Fa-f]{4}:([0-9A-Fa-f]{8})\s+(.+?)\s*$")
ARRAY_DECL_RE = re.compile(
    r"^\s*(?:extern\s+)?(?:static\s+)?(?:const\s+)?(?:[A-Za-z_][A-Za-z0-9_]*\s+)*[*\s]*(?P<var>[A-Za-z_][A-Za-z0-9_]*)\s*\[\s*(?P<size_expr>[^\]]+)\s*\]",
    re.MULTILINE,
)
DEFINE_RE = re.compile(r"^\s*#define\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s+(?P<value>.+?)\s*$", re.MULTILINE)
ENUM_VALUE_RE = re.compile(r"^\s*(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?P<value>[^,]+)\s*,?\s*$", re.MULTILINE)
IDENT_RE = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\b")
HEX_RE = re.compile(r"\b0[xX][0-9A-Fa-f]+\b")
FUNCTION_DEF_RE = re.compile(
    r"^\s*.*?(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*\([^;{}]*\)\s*\{\s*$"
)


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

    def _format_scalar_command(self) -> str:
        return f'logerror "validate {self.label}: 0x%08X\\n", d@{self.variable_address:08X}'

    def _format_small_array_command(self) -> str:
        assert self.array_length is not None
        fmt = " ".join(["0x%08X"] * self.array_length)
        reads = ", ".join(
            f"d@{self.variable_address + index:08X}" for index in range(self.array_length)
        )
        return f'logerror "validate {self.variable_name}[{self.array_length}]: {fmt}\\n", {reads}'

    def _format_large_array_command(self, save_index: int) -> str:
        assert self.array_length is not None
        dump_name = f"{self.instruction_address:08X}-{save_index}.bin"
        return (
            f'save {dump_name}, {self.variable_address:08X}, 0x{self.array_length:X}; '
            f'logerror "validate {self.variable_name}: file={dump_name}\\n"'
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
        return f"bpset {self.instruction_address:08X}, 1, {{ {command}; {action} }}"


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


def find_next_instruction_address(lines: List[str], start_index: int) -> Optional[int]:
    for line in lines[start_index + 1 :]:
        match = ASM_ADDR_RE.match(line)
        if match:
            return int(match.group("addr"), 16)
    return None


def find_containing_function_name(lines: List[str], start_index: int) -> Optional[str]:
    for index in range(start_index, -1, -1):
        match = FUNCTION_DEF_RE.match(lines[index])
        if match:
            return match.group("name")
    return None


def strip_cpp_line_comment(line: str) -> str:
    return line.split("//", 1)[0]


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


def infer_array_lengths(source_root: pathlib.Path) -> Dict[str, int]:
    defines = parse_constant_defines(source_root)
    array_lengths: Dict[str, int] = {}
    for path in sorted(source_root.rglob("*.[ch]")):
        text = path.read_text(errors="ignore")
        for match in ARRAY_DECL_RE.finditer(text):
            var_name = match.group("var")
            size = evaluate_constant_expr(match.group("size_expr"), defines)
            if size is None:
                continue
            array_lengths[var_name] = max(array_lengths.get(var_name, 0), size)
    return array_lengths


def collect_breakpoints_for_file(
    path: pathlib.Path, address_map: Dict[str, int], array_lengths: Dict[str, int], defines: Optional[Dict[str, int]] = None
) -> List[BreakpointEntry]:
    lines = path.read_text(errors="ignore").splitlines()
    entries: List[BreakpointEntry] = []
    seen_arrays: set[tuple[int, str]] = set()
    resolved_defines = defines or {}

    for index, line in enumerate(lines):
        match = VALIDATE_RE.search(strip_cpp_line_comment(line))
        if not match:
            continue

        label = match.group("label")
        variable_name = match.group("var")
        instruction_address = find_next_instruction_address(lines, index)
        if instruction_address is None:
            raise ValueError(
                f"{path}:{index + 1}: could not find next original instruction address after mame_validate_word"
            )

        variable_address = lookup_label_address(label, address_map)
        if variable_address is None:
            raise ValueError(f"{path}:{index + 1}: label {label!r} not found in address map")

        array_length = array_lengths.get(variable_name)
        if array_length is not None:
            dedupe_key = (instruction_address, variable_name)
            if dedupe_key in seen_arrays:
                continue
            seen_arrays.add(dedupe_key)

        entries.append(
            BreakpointEntry(
                label=label,
                variable_name=variable_name,
                variable_address=variable_address,
                instruction_address=instruction_address,
                array_length=array_length,
                source_path=path,
                source_line=index + 1,
            )
        )

        continue

    for index, line in enumerate(lines):
        match = VALIDATE_ARG_RE.search(strip_cpp_line_comment(line))
        if not match:
            continue

        function_name = find_containing_function_name(lines, index)
        if function_name is None:
            raise ValueError(f"{path}:{index + 1}: could not find containing function for mame_validate_arg")

        instruction_address = lookup_label_address(function_name, address_map)
        if instruction_address is None:
            raise ValueError(f"{path}:{index + 1}: function {function_name!r} not found in address map")

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
                    f'logerror "validate {match.group("label")}: 0x%08X\\n", {match.group("label").lower()}'
                ),
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
        match = VALIDATE_REG_AT_ADDR_RE.search(strip_cpp_line_comment(line))
        if not match:
            continue

        entries.append(
            BreakpointEntry(
                label=match.group("reg"),
                variable_name=match.group("var"),
                variable_address=0,
                instruction_address=int(match.group("addr"), 0),
                array_length=None,
                source_path=path,
                source_line=index + 1,
                command_override=f'logerror "validate {match.group("reg")}: 0x%08X\\n", {match.group("reg").lower()}',
            )
        )

    entries.sort(key=lambda entry: (entry.source_line, entry.label, entry.variable_name))
    return entries


def collect_breakpoints(source_root: pathlib.Path, address_map: Dict[str, int]) -> List[BreakpointEntry]:
    defines = parse_constant_defines(source_root)
    array_lengths = infer_array_lengths(source_root)
    entries: List[BreakpointEntry] = []
    for path in sorted(source_root.rglob("*.c")):
        entries.extend(collect_breakpoints_for_file(path, address_map, array_lengths, defines))
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
        for entry in group:
            save_index = save_counts[instruction_address]
            commands.append(entry.format_command(save_index=save_index))
            if entry.array_length is not None and entry.array_length > 5:
                save_counts[instruction_address] += 1
        rows.append(f'bpset {instruction_address:08X}, 1, {{ {"; ".join(commands)}; g }}')
    rows.append("")
    return "\n".join(rows) + "g"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate MAME debugger breakpoints for mame_validate_word call sites."
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
        default=ROOT / "tools" / "mame" / "output" / "mame_validate_word_breakpoints.txt",
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

#!/usr/bin/env python3
"""Check that ASM comments inside C functions mirror their ASM functions.

Both ``// asm:`` and ``// asm ADDRESS:`` comments are compared in order.
Function-local C labels join the same event stream, allowing labels attached to
instructions in the ASM source to be checked too.  This catches omissions and
reorderings as well as assembly invented during translation.
"""

from __future__ import annotations

import argparse
import bisect
import difflib
import re
import sys
from dataclasses import dataclass
from pathlib import Path


SOURCE_MODULE_RE = re.compile(r"Source module:\s*(asm/[A-Za-z0-9_.-]+)", re.IGNORECASE)
C_ASM_RE = re.compile(r"^\s*//\s*asm:(.*)$")
C_NUMBERED_ASM_RE = re.compile(r"^\s*//\s*asm\s+[0-9A-Fa-f]{8}:(.*)$")
C_LABEL_RE = re.compile(r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*):\s*$")
BARE_LABEL_RE = re.compile(r"^[_A-Za-z.$?@][_A-Za-z0-9.$?@]*:?\s*$")
LABEL_WITH_BODY_RE = re.compile(
    r"^([_A-Za-z.$?@][_A-Za-z0-9.$?@]*):?\s+(.+)$"
)

# These are assembler controls or data declarations, not translated CPU code.
NON_CODE_OPERATIONS = {
    "equ",
    "fbss",
    "hibss",
    "lobss",
    "pbss",
    "phibss",
}


@dataclass(frozen=True)
class SourceLine:
    path: Path
    number: int
    text: str

    def location(self, repo_root: Path) -> str:
        try:
            path = self.path.relative_to(repo_root)
        except ValueError:
            path = self.path
        return f"{path}:{self.number}"


@dataclass(frozen=True)
class CFunction:
    name: str
    open_line: int
    close_line: int


def strip_asm_inline_comment(line: str) -> str:
    """Remove a TMS320 comment without treating semicolons in strings as comments."""
    in_string = False
    escaped = False
    for index, char in enumerate(line):
        if char == '"' and not escaped:
            in_string = not in_string
        if char == ";" and not in_string:
            return line[:index]
        escaped = char == "\\" and not escaped
        if char != "\\":
            escaped = False
    return line


def is_asm_code_line(raw_line: str) -> bool:
    stripped = raw_line.strip()
    if not stripped or stripped.startswith(("*", ";")):
        return False

    code = strip_asm_inline_comment(raw_line).strip()
    if not code:
        return False

    # Dot-prefixed assembler directives are never executable instructions.
    if code.startswith("."):
        return False

    if raw_line[:1].isspace():
        operation = code.split(None, 1)[0].lower()
        return not operation.startswith(".") and operation not in NON_CODE_OPERATIONS

    # Bare labels need surrounding context to distinguish function/branch
    # labels from data labels; read_asm_code_lines handles those.  A label and
    # an executable operation on one line remains one comparison unit.
    if BARE_LABEL_RE.fullmatch(code):
        return False
    match = LABEL_WITH_BODY_RE.fullmatch(code)
    if match is None:
        return True
    operation = match.group(2).split(None, 1)[0].lower()
    return not operation.startswith(".") and operation not in NON_CODE_OPERATIONS


def bare_label_precedes_code(lines: list[str], label_index: int) -> bool:
    """Return whether a bare label names executable code rather than data."""
    for raw in lines[label_index + 1 :]:
        stripped = raw.strip()
        if not stripped or stripped.startswith(("*", ";")):
            continue
        code = strip_asm_inline_comment(raw).strip()
        if not code:
            continue
        if BARE_LABEL_RE.fullmatch(code):
            continue
        if code.startswith("."):
            # Conditional/section controls do not decide what the label names;
            # data declarations do.
            operation = code.split(None, 1)[0].lower()
            if operation in {".word", ".float", ".string", ".bss", ".usect"}:
                return False
            continue
        match = LABEL_WITH_BODY_RE.fullmatch(code) if not raw[:1].isspace() else None
        if match is not None:
            operation = match.group(2).split(None, 1)[0].lower()
            return not operation.startswith(".") and operation not in NON_CODE_OPERATIONS
        operation = code.split(None, 1)[0].lower()
        return not operation.startswith(".") and operation not in NON_CODE_OPERATIONS
    return False


def read_asm_code_lines(path: Path) -> list[SourceLine]:
    raw_lines = path.read_text(encoding="utf-8").splitlines()
    return [
        SourceLine(path, number, raw.strip())
        for number, raw in enumerate(raw_lines, 1)
        if is_asm_code_line(raw)
        or (BARE_LABEL_RE.fullmatch(raw.strip()) is not None and bare_label_precedes_code(raw_lines, number - 1))
    ]


def mask_c_comments_and_literals(text: str) -> str:
    """Blank C comments and literals while retaining byte offsets and newlines."""
    result = list(text)
    index = 0
    state = "code"
    while index < len(text):
        char = text[index]
        next_char = text[index + 1] if index + 1 < len(text) else ""
        if state == "code":
            if char == "/" and next_char == "/":
                result[index] = result[index + 1] = " "
                index += 2
                state = "line_comment"
                continue
            if char == "/" and next_char == "*":
                result[index] = result[index + 1] = " "
                index += 2
                state = "block_comment"
                continue
            if char in {'"', "'"}:
                result[index] = " "
                state = "string" if char == '"' else "char"
        elif state == "line_comment":
            if char == "\n":
                state = "code"
            else:
                result[index] = " "
        elif state == "block_comment":
            if char == "*" and next_char == "/":
                result[index] = result[index + 1] = " "
                index += 2
                state = "code"
                continue
            if char != "\n":
                result[index] = " "
        else:
            if char == "\\" and next_char:
                result[index] = " "
                if next_char != "\n":
                    result[index + 1] = " "
                index += 2
                continue
            quote = '"' if state == "string" else "'"
            if char == quote:
                state = "code"
            if char != "\n":
                result[index] = " "
        index += 1
    return "".join(result)


def find_c_functions(text: str) -> list[CFunction]:
    masked = mask_c_comments_and_literals(text)
    line_offsets = [0]
    line_offsets.extend(index + 1 for index, char in enumerate(masked) if char == "\n")
    functions: list[CFunction] = []
    depth = 0
    active_name: str | None = None
    active_open_line = 0
    top_level_boundary = 0

    for index, char in enumerate(masked):
        if char == "{" and depth == 0:
            header = masked[top_level_boundary:index]
            match = re.search(r"([_A-Za-z][_A-Za-z0-9]*)\s*\([^;{}]*\)\s*$", header, re.S)
            if match is not None and match.group(1) not in {"if", "for", "while", "switch"}:
                active_name = match.group(1)
                active_open_line = bisect.bisect_right(line_offsets, index)
            depth = 1
            continue
        if char == "{" and depth > 0:
            depth += 1
            continue
        if char == "}" and depth > 0:
            depth -= 1
            if depth == 0:
                if active_name is not None:
                    functions.append(
                        CFunction(active_name, active_open_line, bisect.bisect_right(line_offsets, index))
                    )
                active_name = None
                top_level_boundary = index + 1
            continue
        if depth == 0 and char == ";":
            top_level_boundary = index + 1
    return functions


def asm_label_name(raw: str) -> str | None:
    if raw[:1].isspace():
        return None
    code = strip_asm_inline_comment(raw).strip()
    match = re.match(r"^([_A-Za-z.$?@][_A-Za-z0-9.$?@]*):?(?:\s+.*)?$", code)
    return match.group(1) if match is not None else None


def asm_operation_names(raw_lines: list[str]) -> set[str]:
    operations: set[str] = set()
    for raw in raw_lines:
        code = strip_asm_inline_comment(raw).strip()
        if not code or code.startswith(".") or code.startswith(("*", ";")):
            continue
        if raw[:1].isspace():
            operations.add(code.split(None, 1)[0].upper())
            continue
        match = LABEL_WITH_BODY_RE.fullmatch(code)
        if match is not None:
            operation = match.group(2).split(None, 1)[0]
            if not operation.startswith("."):
                operations.add(operation.upper())
    return operations


def parse_asm_events(raw: str, path: Path, number: int) -> list[SourceLine]:
    stripped = raw.strip()
    if not stripped or stripped.startswith(("*", ";")):
        return []
    code = strip_asm_inline_comment(raw).strip()
    if not code or code.startswith("."):
        return []
    if raw[:1].isspace():
        operation = code.split(None, 1)[0].lower()
        if operation in NON_CODE_OPERATIONS:
            return []
        return [SourceLine(path, number, stripped)]
    if BARE_LABEL_RE.fullmatch(code):
        return [SourceLine(path, number, code.removesuffix(":"))]
    match = LABEL_WITH_BODY_RE.fullmatch(stripped)
    if match is None:
        return [SourceLine(path, number, stripped)]
    operation = match.group(2).split(None, 1)[0].lower()
    if operation.startswith(".") or operation in NON_CODE_OPERATIONS:
        return []
    return [
        SourceLine(path, number, match.group(1)),
        SourceLine(path, number, match.group(2).strip()),
    ]


def parse_c_comment_events(
    payload: str,
    path: Path,
    number: int,
    operations: set[str],
) -> list[SourceLine]:
    # The comment format contributes one space after ':'.  Preserve a following
    # tab because it distinguishes an instruction from a flush-left label.
    if payload.startswith(" "):
        payload = payload[1:]
    if payload.startswith("\t"):
        return [SourceLine(path, number, payload.strip())]
    stripped = payload.strip()
    if not stripped:
        return [SourceLine(path, number, "")]
    code = strip_asm_inline_comment(stripped).strip()
    if BARE_LABEL_RE.fullmatch(code):
        return [SourceLine(path, number, code.removesuffix(":"))]
    match = LABEL_WITH_BODY_RE.fullmatch(stripped)
    if match is None or match.group(1).upper() in operations:
        return [SourceLine(path, number, stripped)]
    return [
        SourceLine(path, number, match.group(1)),
        SourceLine(path, number, match.group(2).strip()),
    ]


def c_events_for_function(
    path: Path,
    raw_lines: list[str],
    function: CFunction,
    operations: set[str],
) -> tuple[list[SourceLine], int]:
    events: list[SourceLine] = []
    asm_comment_count = 0
    previous_was_c_label = False
    previous_was_comment_label = False
    for number in range(function.open_line, function.close_line + 1):
        raw = raw_lines[number - 1]
        match = C_NUMBERED_ASM_RE.match(raw) or C_ASM_RE.match(raw)
        if match is not None:
            asm_comment_count += 1
            comment_events = parse_c_comment_events(match.group(1), path, number, operations)
            if (
                previous_was_c_label
                and comment_events
                and events
                and asm_key(comment_events[0].text) == asm_key(events[-1].text)
            ):
                comment_events = comment_events[1:]
            events.extend(comment_events)
            previous_was_comment_label = bool(
                comment_events and BARE_LABEL_RE.fullmatch(comment_events[-1].text)
            )
            previous_was_c_label = False
            continue
        label_match = C_LABEL_RE.match(raw)
        if label_match is not None and label_match.group(1) not in {"case", "default"}:
            if not (
                previous_was_comment_label
                and events
                and asm_key(events[-1].text) == asm_key(label_match.group(1))
            ):
                events.append(SourceLine(path, number, label_match.group(1)))
            previous_was_c_label = True
            previous_was_comment_label = False
            continue
        if raw.strip():
            previous_was_c_label = False
            previous_was_comment_label = False
    return events, asm_comment_count


def asm_key(text: str) -> str:
    """Normalize insignificant source formatting without changing operands."""
    result: list[str] = []
    pending_space = False
    in_string = False
    escaped = False
    for char in text.strip():
        if char == '"' and not escaped:
            if pending_space and result:
                result.append(" ")
            pending_space = False
            in_string = not in_string
            result.append(char)
        elif char.isspace() and not in_string:
            pending_space = True
        else:
            if pending_space and result:
                result.append(" ")
            pending_space = False
            result.append(char)
        escaped = char == "\\" and not escaped
        if char != "\\":
            escaped = False
    return "".join(result)


def source_module_for_c(c_path: Path, repo_root: Path) -> Path:
    text = c_path.read_text(encoding="utf-8")
    match = SOURCE_MODULE_RE.search(text)
    if match is None:
        raise ValueError("no 'Source module: asm/FILE.ASM' marker")
    return repo_root / match.group(1)


def c_files_for_input(path: Path, repo_root: Path) -> list[Path]:
    if path.suffix.lower() == ".c":
        return [path]
    if path.suffix.lower() != ".asm":
        raise ValueError("expected a .c or .asm file")

    wanted = path.resolve()
    matches = []
    for c_path in (repo_root / "src").rglob("*.c"):
        try:
            if source_module_for_c(c_path, repo_root).resolve() == wanted:
                matches.append(c_path)
        except ValueError:
            continue
    if not matches:
        raise ValueError("no associated C file found")
    return sorted(matches)


def discover_c_files(repo_root: Path) -> list[Path]:
    result = []
    for path in (repo_root / "src").rglob("*.c"):
        try:
            source_module_for_c(path, repo_root)
        except ValueError:
            continue
        result.append(path)
    return sorted(result)


def compare_pair(c_path: Path, asm_path: Path, repo_root: Path) -> list[str]:
    c_text = c_path.read_text(encoding="utf-8")
    c_raw_lines = c_text.splitlines()
    functions = find_c_functions(c_text)
    asm_raw_lines = asm_path.read_text(encoding="utf-8").splitlines()
    operations = asm_operation_names(asm_raw_lines)

    asm_lines: list[SourceLine] = []
    event_index_at_line: list[int] = []
    separator_indexes: list[int] = []
    for index, raw in enumerate(asm_raw_lines):
        event_index_at_line.append(len(asm_lines))
        if re.match(r"^\s*\*[-]{8,}\s*$", raw):
            separator_indexes.append(len(asm_lines))
        asm_lines.extend(parse_asm_events(raw, asm_path, index + 1))

    audited: list[tuple[CFunction, list[SourceLine]]] = []
    for function in functions:
        c_lines, comment_count = c_events_for_function(
            c_path, c_raw_lines, function, operations
        )
        if comment_count:
            audited.append((function, c_lines))

    name_label_indexes: dict[str, int] = {}
    wanted_names = {function.name for function, _lines in audited}
    for line_index, raw in enumerate(asm_raw_lines):
        name = asm_label_name(raw)
        if name in wanted_names and name not in name_label_indexes:
            name_label_indexes[name] = event_index_at_line[line_index]

    # Map each C function to its ASM entry.  Translation-only helpers used for
    # shared tails have no same-named ASM label, so anchor those at their first
    # C label/comment event when that event is unique in the module.
    anchors: dict[str, tuple[int, int]] = {}
    errors: list[str] = []
    asm_keys = [asm_key(line.text) for line in asm_lines]
    for function, c_lines in audited:
        label_index = name_label_indexes.get(function.name)
        if label_index is not None:
            anchors[function.name] = (label_index + 1, label_index)
            continue
        if not c_lines:
            errors.append(
                f"{c_path.relative_to(repo_root)}:{function.open_line}: "
                f"{function.name}: has asm comments but no comparable events"
            )
            continue
        first_key = asm_key(c_lines[0].text)
        candidates = [index for index, key in enumerate(asm_keys) if key == first_key]
        if len(candidates) != 1:
            errors.append(
                f"{c_path.relative_to(repo_root)}:{function.open_line}: {function.name}: "
                f"no matching ASM function label and first event {c_lines[0].text!r} "
                f"has {len(candidates)} possible source locations"
            )
            continue
        anchors[function.name] = (candidates[0], candidates[0])

    boundary_indexes = sorted({boundary for _begin, boundary in anchors.values()})
    for function, c_lines in audited:
        anchor = anchors.get(function.name)
        if anchor is None:
            continue
        begin, boundary = anchor
        later_boundaries = [index for index in boundary_indexes if index > boundary]
        later_separators = [index for index in separator_indexes if index > begin]
        end = min(
            later_boundaries[0] if later_boundaries else len(asm_lines),
            later_separators[0] if later_separators else len(asm_lines),
        )
        expected_lines = asm_lines[begin:end]

        matcher = difflib.SequenceMatcher(
            None,
            [asm_key(line.text) for line in expected_lines],
            [asm_key(line.text) for line in c_lines],
            autojunk=False,
        )
        for tag, asm_start, asm_end, c_start, c_end in matcher.get_opcodes():
            if tag == "equal":
                continue
            if tag in {"delete", "replace"}:
                for line in expected_lines[asm_start:asm_end]:
                    errors.append(
                        f"{line.location(repo_root)}: {function.name}: "
                        f"missing or out-of-order asm line: {line.text}"
                    )
            if tag in {"insert", "replace"}:
                for line in c_lines[c_start:c_end]:
                    errors.append(
                        f"{line.location(repo_root)}: {function.name}: "
                        f"extra, invented, or out-of-order asm/C label: {line.text}"
                    )
    return errors


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Compare ordered ASM comments in C functions with their ASM source."
    )
    parser.add_argument(
        "files",
        nargs="*",
        type=Path,
        help=".c or .asm files to check (default: all associated C files under src)",
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="repository root (default: inferred from this script)",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    repo_root = args.repo_root.resolve()
    c_paths: list[Path] = []

    try:
        if args.files:
            for supplied in args.files:
                path = supplied if supplied.is_absolute() else repo_root / supplied
                if not path.is_file():
                    raise ValueError(f"file not found: {supplied}")
                c_paths.extend(c_files_for_input(path.resolve(), repo_root))
        else:
            c_paths = discover_c_files(repo_root)

        c_paths = sorted(set(c_paths))
        if not c_paths:
            raise ValueError("no associated C files found")

        errors: list[str] = []
        for c_path in c_paths:
            asm_path = source_module_for_c(c_path, repo_root)
            if not asm_path.is_file():
                errors.append(
                    f"{c_path.relative_to(repo_root)}: associated ASM file not found: "
                    f"{asm_path.relative_to(repo_root)}"
                )
                continue
            errors.extend(compare_pair(c_path, asm_path, repo_root))
    except (OSError, UnicodeError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    if errors:
        print("\n".join(errors))
        print(
            f"\nASM source sync failed with {len(errors)} difference(s) "
            f"across {len(c_paths)} file pair(s).",
            file=sys.stderr,
        )
        return 1

    print(f"ASM source sync passed across {len(c_paths)} file pair(s).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

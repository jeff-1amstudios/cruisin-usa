#!/usr/bin/env python3
"""Check that C translations mirror the ordered event stream of their ASM source.

Both ``// asm:`` and ``// asm ADDRESS:`` comments are compared in order.  Only
instructions and executable-code labels are compared; data declarations and
data labels are deliberately left out of the stream.  An ASM label must be a
real C label or a same-named C function, not merely a comment.  C function
boundaries have no bearing on the comparison: each associated ASM module and
its C translation form one ordered stream.
"""

from __future__ import annotations

import argparse
import bisect
import difflib
import re
import sys
from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path

IDA_DIR = Path(__file__).resolve().parent / "ida"
if str(IDA_DIR) not in sys.path:
    sys.path.append(str(IDA_DIR))

import shared_lib as ccm


SOURCE_MODULE_RE = re.compile(r"Source module:\s*(asm/[A-Za-z0-9_.-]+)", re.IGNORECASE)
C_ASM_RE = re.compile(r"^\s*//\s*asm:(.*)$")
C_NUMBERED_ASM_RE = re.compile(r"^\s*//\s*asm\s+[0-9A-Fa-f]{8}:(.*)$")
C_LABEL_RE = re.compile(
    r"^\s*([_A-Za-z.$?@][_A-Za-z0-9.$?@]*):\s*(?:;\s*)?(?://.*)?$"
)
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
    "romdata",
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


def bare_label_precedes_code(
    lines: list[str],
    label_index: int,
    data_only_macros: frozenset[str] = frozenset(),
) -> bool:
    """Return whether a bare label names executable code rather than data."""
    for raw in lines[label_index + 1 :]:
        stripped = raw.strip()
        if not stripped or stripped.startswith(("*", ";")):
            continue
        code = strip_asm_inline_comment(raw).strip()
        if not code:
            continue
        if not raw[:1].isspace() and BARE_LABEL_RE.fullmatch(code):
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
            return (
                not operation.startswith(".")
                and operation not in NON_CODE_OPERATIONS
                and operation.upper() not in data_only_macros
            )
        operation = code.split(None, 1)[0].lower()
        return (
            not operation.startswith(".")
            and operation not in NON_CODE_OPERATIONS
            and operation.upper() not in data_only_macros
        )
    return False


def read_asm_code_lines(path: Path) -> list[SourceLine]:
    raw_lines = path.read_text(encoding="utf-8").splitlines()
    return [
        SourceLine(path, number, raw.strip())
        for number, raw in enumerate(raw_lines, 1)
        if is_asm_code_line(raw)
        or (
            not raw[:1].isspace()
            and BARE_LABEL_RE.fullmatch(raw.strip()) is not None
            and bare_label_precedes_code(raw_lines, number - 1)
        )
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
            header = masked[top_level_boundary:index].rstrip()
            name: str | None = None
            if header.endswith(")"):
                paren_depth = 0
                for header_index in range(len(header) - 1, -1, -1):
                    if header[header_index] == ")":
                        paren_depth += 1
                    elif header[header_index] == "(":
                        paren_depth -= 1
                        if paren_depth == 0:
                            match = re.search(
                                r"([_A-Za-z][_A-Za-z0-9]*)\s*$",
                                header[:header_index],
                            )
                            if match is not None:
                                name = match.group(1)
                            break
            if name is not None and name not in {"if", "for", "while", "switch"}:
                active_name = name
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


@lru_cache(maxsize=None)
def data_only_macro_names(repo_root: Path) -> frozenset[str]:
    """Return source macros whose expansion contains no executable operation."""
    macros = ccm.parse_macros(repo_root)
    symbols = ccm.parse_set_symbols(repo_root)
    symbols["DEBUG"] = 0
    cache: dict[str, bool] = {}
    return frozenset(
        name.upper()
        for name in macros
        if not ccm.macro_emits_executable(name, macros, symbols, cache, set())
    )


def asm_operation_names(
    raw_lines: list[str], data_only_macros: frozenset[str] = frozenset()
) -> set[str]:
    operations: set[str] = set()
    for raw in raw_lines:
        code = strip_asm_inline_comment(raw).strip()
        if not code or code.startswith(".") or code.startswith(("*", ";")):
            continue
        if raw[:1].isspace():
            operation = code.split(None, 1)[0].upper()
            if operation.lower() not in NON_CODE_OPERATIONS and operation not in data_only_macros:
                operations.add(operation)
            continue
        match = LABEL_WITH_BODY_RE.fullmatch(code)
        if match is not None:
            operation = match.group(2).split(None, 1)[0]
            if (
                not operation.startswith(".")
                and operation.lower() not in NON_CODE_OPERATIONS
                and operation.upper() not in data_only_macros
            ):
                operations.add(operation.upper())
    return operations


def executable_asm_labels(
    repo_root: Path,
    asm_path: Path,
    raw_lines: list[str],
    data_only_macros: frozenset[str] = frozenset(),
) -> frozenset[str]:
    """Return labels known to name code, preferring the ROM-list classification."""
    labels_path = repo_root / "tools" / "ida" / "log" / "romlst_labels.tsv"
    module = asm_path.stem.upper()
    labels: set[str] = set()
    found_module = False
    if labels_path.is_file():
        for raw in labels_path.read_text(encoding="utf-8").splitlines()[1:]:
            columns = raw.split("\t")
            if len(columns) < 4 or columns[2].upper() != module:
                continue
            found_module = True
            if columns[3].lower() in {"code", "label"}:
                labels.add(columns[0])
    if found_module:
        return frozenset(labels)

    # Standalone/test sources do not have a ROM-list log. Infer labels from
    # the first substantive line following them, while still excluding data.
    for index, raw in enumerate(raw_lines):
        if raw[:1].isspace():
            continue
        code = strip_asm_inline_comment(raw).strip()
        if BARE_LABEL_RE.fullmatch(code) and bare_label_precedes_code(
            raw_lines, index, data_only_macros
        ):
            labels.add(code.removesuffix(":"))
            continue
        match = LABEL_WITH_BODY_RE.fullmatch(code)
        if match is None:
            continue
        operation = match.group(2).split(None, 1)[0]
        if (
            not operation.startswith(".")
            and operation.lower() not in NON_CODE_OPERATIONS
            and operation.upper() not in data_only_macros
        ):
            labels.add(match.group(1))
    return frozenset(labels)


def parse_asm_events(
    raw: str,
    path: Path,
    number: int,
    data_only_macros: frozenset[str] = frozenset(),
    code_labels: frozenset[str] = frozenset(),
) -> list[SourceLine]:
    stripped = raw.strip()
    if not stripped or stripped.startswith(("*", ";")):
        return []
    code = strip_asm_inline_comment(raw).strip()
    if not code or code.startswith("."):
        return []
    if raw[:1].isspace():
        operation = code.split(None, 1)[0].lower()
        if operation in NON_CODE_OPERATIONS or operation.upper() in data_only_macros:
            return []
        return [SourceLine(path, number, code)]
    if BARE_LABEL_RE.fullmatch(code):
        label = code.removesuffix(":")
        if label in code_labels:
            return [SourceLine(path, number, label)]
        return []
    match = LABEL_WITH_BODY_RE.fullmatch(code)
    if match is None:
        return [SourceLine(path, number, code)]
    operation = match.group(2).split(None, 1)[0].lower()
    if (
        operation.startswith(".")
        or operation in NON_CODE_OPERATIONS
        or operation.upper() in data_only_macros
    ):
        return []
    events = []
    if match.group(1) in code_labels:
        events.append(SourceLine(path, number, match.group(1)))
    events.append(SourceLine(path, number, match.group(2).strip()))
    return events


def parse_c_comment_events(
    payload: str,
    path: Path,
    number: int,
    operations: set[str],
    data_only_macros: frozenset[str] = frozenset(),
) -> list[SourceLine]:
    # The comment format contributes one space after ':'.  Preserve a following
    # tab because it distinguishes an instruction from a flush-left label.
    if payload.startswith(" "):
        payload = payload[1:]
    stripped = payload.strip()
    if not stripped:
        return []
    code = strip_asm_inline_comment(stripped).strip()
    if not code or code.startswith("."):
        return []
    operation = code.split(None, 1)[0]
    if operation.lower() in NON_CODE_OPERATIONS or operation.upper() in data_only_macros:
        return []
    if payload.startswith("\t"):
        return [SourceLine(path, number, code)]
    if BARE_LABEL_RE.fullmatch(code):
        # A label written in an asm comment is still only a comment.  Labels
        # must be represented by a real C label or by the C function itself.
        return []
    match = LABEL_WITH_BODY_RE.fullmatch(code)
    if match is None or match.group(1).upper() in operations:
        return [SourceLine(path, number, code)]
    return [SourceLine(path, number, match.group(2).strip())]


def c_events_for_file(
    path: Path,
    raw_lines: list[str],
    operations: set[str],
    code_labels: frozenset[str],
    data_only_macros: frozenset[str] = frozenset(),
) -> list[SourceLine]:
    """Build the C event stream without assigning events to functions."""
    events: list[SourceLine] = []
    functions_by_line: dict[int, list[CFunction]] = {}
    real_labels = {
        match.group(1)
        for raw in raw_lines
        if (match := C_LABEL_RE.match(raw)) is not None
    }
    for function in find_c_functions("\n".join(raw_lines)):
        functions_by_line.setdefault(function.open_line, []).append(function)

    for number, raw in enumerate(raw_lines, 1):
        for function in functions_by_line.get(number, []):
            if function.name in code_labels and function.name not in real_labels:
                events.append(SourceLine(path, number, function.name))

        match = C_NUMBERED_ASM_RE.match(raw) or C_ASM_RE.match(raw)
        if match is not None:
            events.extend(
                parse_c_comment_events(
                    match.group(1), path, number, operations, data_only_macros
                )
            )
            continue
        label_match = C_LABEL_RE.match(raw)
        if label_match is not None and label_match.group(1) in code_labels:
            events.append(SourceLine(path, number, label_match.group(1)))
    return events


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


def compare_module(c_paths: list[Path], asm_path: Path, repo_root: Path) -> list[str]:
    """Compare an ASM module with the global stream from its associated C files."""
    asm_raw_lines = asm_path.read_text(encoding="utf-8").splitlines()
    data_macros = data_only_macro_names(repo_root)
    operations = asm_operation_names(asm_raw_lines, data_macros)
    code_labels = executable_asm_labels(
        repo_root, asm_path, asm_raw_lines, data_macros
    )

    asm_lines: list[SourceLine] = []
    for index, raw in enumerate(asm_raw_lines):
        asm_lines.extend(
            parse_asm_events(
                raw,
                asm_path,
                index + 1,
                data_macros,
                code_labels,
            )
        )

    c_lines: list[SourceLine] = []
    for c_path in sorted(c_paths):
        c_lines.extend(
            c_events_for_file(
                c_path,
                c_path.read_text(encoding="utf-8").splitlines(),
                operations,
                code_labels,
                data_macros,
            )
        )

    errors: list[str] = []
    matcher = difflib.SequenceMatcher(
        None,
        [asm_key(line.text) for line in asm_lines],
        [asm_key(line.text) for line in c_lines],
        autojunk=False,
    )
    for tag, asm_start, asm_end, c_start, c_end in matcher.get_opcodes():
        if tag == "equal":
            continue
        if tag in {"delete", "replace"}:
            for line in asm_lines[asm_start:asm_end]:
                errors.append(
                    f"{line.location(repo_root)}: "
                    f"missing or out-of-order asm label/instruction: {line.text}"
                )
        if tag in {"insert", "replace"}:
            for line in c_lines[c_start:c_end]:
                errors.append(
                    f"{line.location(repo_root)}: "
                    f"extra, invented, or out-of-order asm label/instruction: {line.text}"
                )
    return errors


def compare_pair(c_path: Path, asm_path: Path, repo_root: Path) -> list[str]:
    return compare_module([c_path], asm_path, repo_root)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Compare each ASM module's ordered labels/instructions with its C translation."
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

        modules: dict[Path, list[Path]] = {}
        errors: list[str] = []
        for c_path in c_paths:
            asm_path = source_module_for_c(c_path, repo_root)
            if not asm_path.is_file():
                errors.append(
                    f"{c_path.relative_to(repo_root)}: associated ASM file not found: "
                    f"{asm_path.relative_to(repo_root)}"
                )
                continue
            modules.setdefault(asm_path.resolve(), []).append(c_path)
        for asm_path, associated_c_paths in sorted(modules.items()):
            errors.extend(compare_module(associated_c_paths, asm_path, repo_root))
    except (OSError, UnicodeError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    if errors:
        print("\n".join(errors))
        print(
            f"\nASM source sync failed with {len(errors)} difference(s) "
            f"across {len(modules)} module(s).",
            file=sys.stderr,
        )
        return 1

    print(f"ASM source sync passed across {len(modules)} module(s).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

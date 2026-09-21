#!/usr/bin/env python3
"""Check mechanical C3X translation rules that are easy to regress."""

from __future__ import annotations

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCE_ROOT = ROOT / "src"
SOURCE_FILES = sorted((*SOURCE_ROOT.rglob("*.c"), *SOURCE_ROOT.rglob("*.h")))

DECIMAL = re.compile(r"(?<![\w.])[-+]?(?:\d+\.\d*|\.\d+)(?:[eE][-+]?\d+)?[fFlL]?(?![\w.])")
C3X_ARITHMETIC = re.compile(r"\bC3X_(?:ADD|SUB|MUL|DIV|EQ|NE|LT|LE|GT|GE)\s*\(")
ALLOWED_VALUE = re.compile(
    r"\bC3X_(?:IMM_F32|REG_FROM_DOUBLE|F32_INIT|LDF|FROM_INT)\s*\((?:[^()]|\([^()]*\))*\)"
)
ASM_FLOAT_SYMBOL = re.compile(
    r"//\s*asm\b.*\b(?:"
    r"LDF|ADDF|SUBF|SUBRF|MPYF|CMPF"
    r")[A-Z]*\s+([+-]?[A-Za-z_.$][A-Za-z0-9_.$]*)\s*(?:,|$)"
)
ASM_REGISTER = re.compile(r"^(?:A?R\d+|IR\d+|BK|RC|RS|RE)$", re.IGNORECASE)
IMM_ARGUMENT_INDEX = {
    **{
        f"C3X_{operation}_IMM": 1
        for operation in ("ADD", "SUB", "MUL", "DIV", "EQ", "NE", "LT", "LE", "GT", "GE")
    },
    "C3X_RSUB_IMM": 0,
    "C3X_STF_IMM": 0,
}


def strip_comments_and_strings(text: str) -> str:
    pattern = re.compile(
        r"//[^\n]*|/\*.*?\*/|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'",
        re.DOTALL,
    )
    return pattern.sub(lambda match: "\n" * match.group(0).count("\n"), text)


def line_number(text: str, offset: int) -> int:
    return text.count("\n", 0, offset) + 1


def matching_paren(text: str, opening: int) -> int | None:
    depth = 0
    for index in range(opening, len(text)):
        if text[index] == "(":
            depth += 1
        elif text[index] == ")":
            depth -= 1
            if depth == 0:
                return index
    return None


def argument_ranges(text: str, opening: int, closing: int) -> list[tuple[int, int]]:
    ranges: list[tuple[int, int]] = []
    depth = 0
    start = opening + 1
    for index in range(start, closing):
        if text[index] == "(":
            depth += 1
        elif text[index] == ")":
            depth -= 1
        elif text[index] == "," and depth == 0:
            ranges.append((start, index))
            start = index + 1
    ranges.append((start, closing))
    return ranges


def mask_immediate_arguments(text: str) -> str:
    """Blank operands whose enclosing API explicitly gives them immediate semantics."""
    result = list(text)
    names = "|".join(map(re.escape, IMM_ARGUMENT_INDEX))
    for match in re.finditer(rf"\b({names})\s*\(", text):
        opening = text.find("(", match.start())
        closing = matching_paren(text, opening)
        if closing is None:
            continue
        ranges = argument_ranges(text, opening, closing)
        argument_index = IMM_ARGUMENT_INDEX[match.group(1)]
        if argument_index >= len(ranges):
            continue
        start, end = ranges[argument_index]
        for index in range(start, end):
            if result[index] != "\n":
                result[index] = " "
    return "".join(result)


def report(errors: list[str], path: Path, line: int, message: str) -> None:
    errors.append(f"{path.relative_to(ROOT)}:{line}: {message}")


def check_file(path: Path, errors: list[str]) -> None:
    original = path.read_text(encoding="utf-8")
    code = strip_comments_and_strings(original)

    deprecated = {
        "C3X_SHORT_F32": "C3X_IMM_F32",
        "C3X_F32": "C3X_REG_FROM_DOUBLE",
        "C3X_LOAD": "C3X_FROM_RAW32",
        "C3X_STORE": "C3X_TO_RAW32",
        "C3X_MPYF": "an explicit C3X_MUL assignment",
        "C3X_MPYF3": "an explicit C3X_MUL assignment",
        "C3X_ADDF": "an explicit C3X_ADD assignment",
        "C3X_ADDF3": "an explicit C3X_ADD assignment",
        "C3X_SUBF": "an explicit C3X_SUB assignment",
        "C3X_SUBF3": "an explicit C3X_SUB assignment",
    }
    for old_name, replacement in deprecated.items():
        for match in re.finditer(rf"\b{old_name}\b", code):
            report(
                errors,
                path,
                line_number(code, match.start()),
                f"use {replacement} instead of {old_name}",
            )

    allow_full_precision_file = "c3x-lint: allow-reg-from-double" in original
    if path.is_relative_to(ROOT / "src" / "game") and not allow_full_precision_file:
        for match in re.finditer(r"\bC3X_REG_FROM_DOUBLE\s*\(", code):
            line = line_number(code, match.start())
            source_line = original.splitlines()[line - 1]
            if "c3x-lint: full-precision" not in source_line:
                report(
                    errors,
                    path,
                    line,
                    "C3X_REG_FROM_DOUBLE requires an explicit full-precision lint annotation; "
                    "instruction immediates use C3X_IMM_F32",
                )

    # A word-like floating instruction operand without '@' is an instruction
    # immediate. This applies to every assembler symbol, not just well-known
    # constants such as HALFPI. If translated code in the following asm block
    # uses that symbol (or its conventional memory-storage alias with a trailing
    # I), it must do so through C3X_IMM_F32.
    lines = original.splitlines()
    for index, source_line in enumerate(lines):
        match = ASM_FLOAT_SYMBOL.search(source_line)
        if match is None:
            continue
        operand = match.group(1)
        symbol = operand.lstrip("+-")
        if ASM_REGISTER.fullmatch(symbol):
            continue

        following: list[str] = []
        asm_count = 0
        for candidate in lines[index + 1 : index + 41]:
            if re.search(r"//\s*asm\b", candidate):
                asm_count += 1
            if asm_count >= 12:
                break
            following.append(candidate)
        translated = strip_comments_and_strings("\n".join(following))
        if not translated.strip() or not re.search(r"[;{}]", translated):
            continue

        immediate = re.compile(
            rf"\bC3X_IMM_F32\s*\(\s*{re.escape(operand)}\s*\)"
        )
        symbol_use = re.compile(rf"(?<![\w.]){re.escape(operand)}(?![\w.])")
        storage_alias_use = re.compile(
            rf"(?<![\w.]){re.escape(operand)}I(?![\w.])"
        )
        masked = mask_immediate_arguments(immediate.sub("", translated))
        if symbol_use.search(masked) or storage_alias_use.search(masked):
            report(
                errors,
                path,
                index + 1,
                f"assembly {operand} is a floating instruction immediate; "
                f"use an _IMM operation or C3X_IMM_F32({operand})",
            )

    # Examine complete semicolon-terminated statements so multiline arithmetic
    # expressions are covered. Mask explicit constructors before looking for a
    # raw decimal operand.
    statement_start = 0
    for statement_match in re.finditer(r";", code):
        statement_end = statement_match.end()
        statement = code[statement_start:statement_end]
        if C3X_ARITHMETIC.search(statement):
            masked = ALLOWED_VALUE.sub("", mask_immediate_arguments(statement))
            decimal = DECIMAL.search(masked)
            if decimal:
                report(
                    errors,
                    path,
                    line_number(code, statement_start + decimal.start()),
                    f"raw floating literal {decimal.group(0)!r} in C3X arithmetic; "
                    "use the operation's _IMM form for an instruction immediate",
                )
        statement_start = statement_end


def main() -> int:
    errors: list[str] = []
    for source_file in SOURCE_FILES:
        check_file(source_file, errors)

    if errors:
        print("\n".join(errors), file=sys.stderr)
        print(f"C3X translation check failed with {len(errors)} error(s).", file=sys.stderr)
        return 1

    print(f"C3X translation check passed across {len(SOURCE_FILES)} source files.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

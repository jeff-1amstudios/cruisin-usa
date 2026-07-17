#!/usr/bin/env python3
"""Replace unimplemented repo functions with generated function bodies."""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path


FUNCTION_NAME_RE = re.compile(r"([A-Za-z_][A-Za-z0-9_]*)\s*\(")
UNIMPL_END_RE = re.compile(r"\bUNIMPL(?:_TODO)?\s*\(\s*\)\s*;\s*\}\s*$", re.DOTALL)


@dataclass
class FunctionDef:
    name: str
    start_line: int
    end_line: int
    text: str
    header: str
    body: str


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Replace repo function definitions ending in UNIMPL()/UNIMPL_TODO() "
            "with the generated function bodies from cruisin-usa-gen."
        )
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="Path to the repository root.",
    )
    parser.add_argument(
        "--gen-root",
        type=Path,
        default=Path.home() / "code" / "cruisin-usa-gen",
        help="Path to the generated source directory.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Report replacements without writing files.",
    )
    return parser.parse_args()


def strip_comments_for_braces(line: str, in_block_comment: bool) -> tuple[str, bool]:
    out: list[str] = []
    i = 0
    while i < len(line):
        if in_block_comment:
            end = line.find("*/", i)
            if end == -1:
                return "".join(out), True
            i = end + 2
            in_block_comment = False
            continue

        if line.startswith("//", i):
            break
        if line.startswith("/*", i):
            in_block_comment = True
            i += 2
            continue
        if line[i] in ("'", '"'):
            quote = line[i]
            out.append(" ")
            i += 1
            while i < len(line):
                if line[i] == "\\":
                    i += 2
                    continue
                if line[i] == quote:
                    i += 1
                    break
                i += 1
            continue
        out.append(line[i])
        i += 1
    return "".join(out), in_block_comment


def is_control_statement(header: str) -> bool:
    stripped = header.strip()
    return stripped.startswith(("if ", "if(", "for ", "for(", "while ", "while(", "switch ", "switch("))


def extract_function_name(header: str) -> str | None:
    if is_control_statement(header):
        return None
    before_paren = header.split("(", 1)[0]
    tokens = re.findall(r"[A-Za-z_][A-Za-z0-9_]*", before_paren)
    if not tokens:
        return None
    return tokens[-1]


def parse_functions(text: str) -> list[FunctionDef]:
    lines = text.splitlines(keepends=True)
    functions: list[FunctionDef] = []
    i = 0
    scan_in_block_comment = False
    while i < len(lines):
        line = lines[i]
        sanitized_line, scan_in_block_comment = strip_comments_for_braces(line, scan_in_block_comment)
        stripped = sanitized_line.strip()
        if not stripped or stripped.startswith("#") or stripped.startswith("//"):
            i += 1
            continue

        if "(" not in sanitized_line:
            i += 1
            continue

        header_lines = [line]
        j = i
        header_text = sanitized_line
        header_scan_in_block_comment = scan_in_block_comment
        while "{" not in header_text and ";" not in header_text:
            j += 1
            if j >= len(lines):
                break
            header_lines.append(lines[j])
            sanitized_header_line, header_scan_in_block_comment = strip_comments_for_braces(
                lines[j],
                header_scan_in_block_comment,
            )
            header_text += sanitized_header_line

        if "{" not in header_text or header_text.find(";") < header_text.find("{") if ";" in header_text else False:
            i += 1
            continue

        name = extract_function_name(header_text)
        if name is None:
            i += 1
            continue

        brace_depth = 0
        in_block_comment = False
        end_line = j
        started = False
        for k in range(i, len(lines)):
            sanitized, in_block_comment = strip_comments_for_braces(lines[k], in_block_comment)
            for ch in sanitized:
                if ch == "{":
                    brace_depth += 1
                    started = True
                elif ch == "}":
                    brace_depth -= 1
            if started and brace_depth == 0:
                end_line = k
                break
        else:
            raise ValueError(f"unbalanced braces while parsing function {name}")

        func_text = "".join(lines[i : end_line + 1])
        body_start = func_text.find("{")
        body_end = func_text.rfind("}")
        if body_start == -1 or body_end == -1 or body_end < body_start:
            raise ValueError(f"could not split function {name} into header/body")
        header = func_text[:body_start]
        body = func_text[body_start:]
        functions.append(
            FunctionDef(
                name=name,
                start_line=i + 1,
                end_line=end_line + 1,
                text=func_text,
                header=header,
                body=body,
            )
        )
        i = end_line + 1

    return functions


def iter_repo_c_files(repo_root: Path) -> list[Path]:
    return sorted((repo_root / "src").rglob("*.c"))


def replace_functions(repo_path: Path, gen_path: Path, dry_run: bool) -> int:
    repo_text = repo_path.read_text(encoding="utf-8")
    gen_text = gen_path.read_text(encoding="utf-8")

    repo_functions = parse_functions(repo_text)
    gen_functions = {func.name: func for func in parse_functions(gen_text)}

    lines = repo_text.splitlines(keepends=True)
    replacements: list[tuple[int, int, str, str]] = []

    for func in repo_functions:
        if not UNIMPL_END_RE.search(func.text):
            continue
        gen_func = gen_functions.get(func.name)
        if gen_func is None:
            print(f"warning: no generated definition for {repo_path}:{func.name}", file=sys.stderr)
            continue
        new_text = func.header + gen_func.body
        if func.text == new_text:
            continue
        replacements.append((func.start_line, func.end_line, func.name, new_text))

    if not replacements:
        return 0

    print(f"{repo_path.relative_to(repo_path.parents[1])}: replacing {len(replacements)} functions")
    for start_line, end_line, name, _ in replacements:
        print(f"  {name} ({start_line}-{end_line})")

    if dry_run:
        return len(replacements)

    for start_line, end_line, _, new_text in reversed(replacements):
        lines[start_line - 1 : end_line] = [new_text]

    repo_path.write_text("".join(lines), encoding="utf-8")
    return len(replacements)


def main() -> int:
    args = parse_args()
    repo_root = args.repo_root.resolve()
    gen_root = args.gen_root.resolve()

    total = 0
    for repo_path in iter_repo_c_files(repo_root):
        gen_path = gen_root / repo_path.name
        if not gen_path.is_file():
            continue
        total += replace_functions(repo_path, gen_path, args.dry_run)

    if total == 0:
        print("no unimplemented functions needed replacement")
        return 0

    print(f"replaced {total} functions" if not args.dry_run else f"would replace {total} functions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

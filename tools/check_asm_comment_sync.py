#!/usr/bin/env python3
"""Report asm comment lines that do not match the generated source."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


ASM_LINE_RE = re.compile(r"^\s*// asm [0-9A-Fa-f]{8}:")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Check that each numbered '// asm ...' line in the repo exists "
            "verbatim in the generated counterpart file."
        )
    )
    parser.add_argument(
        "--repo-root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="Path to the repository root (default: current repo).",
    )
    parser.add_argument(
        "--gen-root",
        type=Path,
        default=Path.home() / "code" / "cruisin-usa-gen",
        help="Path to the generated source directory.",
    )
    return parser.parse_args()


def build_gen_line_set(gen_root: Path) -> set[str]:
    line_set: set[str] = set()

    for path in gen_root.rglob("*"):
        if not path.is_file():
            continue
        try:
            lines = path.read_text(encoding="utf-8").splitlines()
        except UnicodeDecodeError:
            continue
        for line in lines:
            if ASM_LINE_RE.match(line):
                line_set.add(line.strip())

    return line_set


def iter_repo_files(repo_root: Path) -> list[Path]:
    source_suffixes = {".c", ".h"}
    source_roots = (repo_root / "src",)
    files: list[Path] = []

    for source_root in source_roots:
        if not source_root.is_dir():
            continue
        files.extend(
            path
            for path in source_root.rglob("*")
            if path.is_file() and path.suffix in source_suffixes
        )

    return sorted(files)


def main() -> int:
    args = parse_args()
    repo_root = args.repo_root.resolve()
    gen_root = args.gen_root.resolve()

    if not gen_root.is_dir():
        print(f"generated directory not found: {gen_root}", file=sys.stderr)
        return 2

    gen_lines = build_gen_line_set(gen_root)
    mismatches: list[tuple[Path, int, str]] = []
    checked_files = 0
    checked_lines = 0

    for repo_path in iter_repo_files(repo_root):
        if gen_root in repo_path.parents:
            continue

        rel_path = repo_path.relative_to(repo_root)
        try:
            lines = repo_path.read_text(encoding="utf-8").splitlines()
        except UnicodeDecodeError:
            continue

        asm_lines = [
            (line_number, line)
            for line_number, line in enumerate(lines, start=1)
            if ASM_LINE_RE.match(line)
        ]
        if not asm_lines:
            continue

        checked_files += 1
        checked_lines += len(asm_lines)

        for line_number, line in asm_lines:
            if line.strip() not in gen_lines:
                mismatches.append((rel_path, line_number, line))

    if mismatches:
        for rel_path, line_number, line in mismatches:
            print(f"{rel_path}:{line_number}: {line}")
        print()
        print(
            f"{len(mismatches)} mismatched asm comment lines "
            f"across {checked_files} files ({checked_lines} checked)."
        )
        return 1

    print(f"all {checked_lines} asm comment lines matched across {checked_files} files.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

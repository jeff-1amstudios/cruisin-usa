#!/usr/bin/env python3
"""Advance successive UNIMPL hits to UNIMPL_TODO() until a stop condition."""

from __future__ import annotations

import os
import re
import subprocess
import sys
from pathlib import Path


UNIMPL_RE = re.compile(r"UNIMPL reached at (.+):(\d+) in ([A-Za-z_][A-Za-z0-9_]*)")


def run_build(repo_root: Path) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        ["cmake", "--build", "build", "--clean-first"],
        cwd=repo_root,
        text=True,
        capture_output=True,
    )


def run_binary(repo_root: Path) -> subprocess.CompletedProcess[str]:
    env = os.environ.copy()
    env["SDL_VIDEODRIVER"] = "dummy"
    env["SDL_AUDIODRIVER"] = "dummy"
    env["SDL_RENDER_DRIVER"] = "software"
    return subprocess.run(
        ["./build/crusn"],
        cwd=repo_root,
        text=True,
        capture_output=True,
        env=env,
    )


def patch_unimpl(file_path: Path, line_number: int) -> bool:
    lines = file_path.read_text(encoding="utf-8").splitlines(keepends=True)
    idx = line_number - 1
    if idx < 0 or idx >= len(lines):
        raise ValueError(f"line out of range: {file_path}:{line_number}")

    for candidate in (idx, idx - 1, idx + 1):
        if 0 <= candidate < len(lines) and "UNIMPL();" in lines[candidate]:
            lines[candidate] = lines[candidate].replace("UNIMPL();", "UNIMPL_TODO();")
            file_path.write_text("".join(lines), encoding="utf-8")
            return True
    return False


def main() -> int:
    repo_root = Path(__file__).resolve().parents[1]
    steps = 0

    while True:
        build = run_build(repo_root)
        if build.returncode != 0:
            sys.stdout.write(build.stdout)
            sys.stderr.write(build.stderr)
            print("STOP: build failed")
            return 2

        run = run_binary(repo_root)
        combined = run.stdout + run.stderr

        match = UNIMPL_RE.search(combined)
        if match is None:
            tail_lines = [line for line in combined.splitlines() if line.strip()][-20:]
            print(f"STOP: different runtime result (exit {run.returncode})")
            if tail_lines:
                print("RUNTIME TAIL:")
                for line in tail_lines:
                    print(line)
            return 0

        file_name, line_text, func = match.groups()
        line_number = int(line_text)
        print(f"UNIMPL: {func} at {file_name}:{line_number}")

        if func == "_stuff_fpga":
            print("STOP: reached _stuff_fpga")
            return 0

        file_path = Path(file_name)
        if not patch_unimpl(file_path, line_number):
            print(f"STOP: could not patch UNIMPL at {file_path}:{line_number}")
            return 1

        steps += 1
        print(f"PATCHED: {file_path}:{line_number} ({func}) -> UNIMPL_TODO()")


if __name__ == "__main__":
    raise SystemExit(main())

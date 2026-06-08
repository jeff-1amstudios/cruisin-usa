#!/usr/bin/env python3
"""Compare native and MAME trace files."""

from pathlib import Path


def main() -> int:
    root = Path(__file__).resolve().parents[2]
    print(f"stub: compare traces under {root}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

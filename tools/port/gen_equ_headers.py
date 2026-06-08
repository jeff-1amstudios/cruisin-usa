#!/usr/bin/env python3
"""Generate C headers from .EQU files."""

from __future__ import annotations

import argparse
import re
from dataclasses import dataclass
from pathlib import Path


SET_RE = re.compile(r"^\s*([A-Za-z_.$?@][A-Za-z0-9_.$?@]*)\s*\.set\s*(.*?)\s*$", re.IGNORECASE)
GLOBL_RE = re.compile(r"^\s*\.globl\s+(.*?)\s*$", re.IGNORECASE)
HEX_SUFFIX_RE = re.compile(r"\b([0-9A-Fa-f]+)[Hh]\b")


@dataclass
class SetEntry:
    name: str
    expr: str
    comment: str


def split_comment(line: str) -> tuple[str, str]:
    in_quote = False
    for i, ch in enumerate(line):
        if ch == "'":
            in_quote = not in_quote
        elif ch == ";" and not in_quote:
            return line[:i].rstrip(), line[i + 1 :].strip()
    return line.rstrip(), ""


def convert_expr(expr: str) -> str:
    expr = expr.strip()
    expr = HEX_SUFFIX_RE.sub(lambda m: f"0x{m.group(1)}", expr)
    expr = re.sub(r"\s+", " ", expr)
    return expr


def needs_parens(expr: str) -> bool:
    if " " in expr:
        return True
    return any(op in expr for op in ("+", "-", "*", "/", "|", "&", "^", "<<", ">>"))


def sanitize_guard_stem(name: str) -> str:
    return re.sub(r"[^A-Za-z0-9]+", "_", name).upper().strip("_")


def sanitize_include_name(path: Path) -> str:
    return path.stem.lower() + ".h"


def parse_equ_file(path: Path) -> tuple[list[str], list[str], list[SetEntry]]:
    banner_comments: list[str] = []
    globls: list[str] = []
    sets: list[SetEntry] = []

    for raw in path.read_text(errors="ignore").splitlines():
        stripped = raw.strip()
        if not stripped:
            banner_comments.append("")
            continue
        if stripped.startswith("*"):
            banner_comments.append(stripped[1:].strip())
            continue
        if stripped.startswith(";"):
            banner_comments.append(stripped[1:].strip())
            continue

        code, comment = split_comment(raw)
        match = GLOBL_RE.match(code)
        if match:
            names = [part.strip() for part in match.group(1).split(",") if part.strip()]
            globls.extend(names)
            continue

        match = SET_RE.match(code)
        if match:
            name, expr = match.groups()
            sets.append(SetEntry(name=name, expr=convert_expr(expr), comment=comment))
            continue

    return banner_comments, globls, sets


def render_header(src_path: Path, banner_comments: list[str], globls: list[str], sets: list[SetEntry]) -> str:
    guard = f"{sanitize_guard_stem(src_path.stem)}_H"
    out: list[str] = []
    out.append(f"#ifndef {guard}")
    out.append(f"#define {guard}")
    out.append("")
    out.append(f"/* Generated from asm/{src_path.name}. */")

    cleaned_banner = [line for line in banner_comments if line]
    if cleaned_banner:
        out.append("/*")
        for line in cleaned_banner[:8]:
            out.append(f" * {line}")
        if len(cleaned_banner) > 8:
            out.append(" * ...")
        out.append(" */")

    if globls:
        out.append("")
        out.append("/* Original .globl symbols in this module:")
        for name in globls:
            out.append(f" *   {name}")
        out.append(" */")

    if sets:
        out.append("")
        emitted: dict[str, str] = {}
        for entry in sets:
            prior_expr = emitted.get(entry.name)
            if prior_expr == entry.expr:
                continue
            if prior_expr is not None:
                out.append(f"#undef {entry.name}")
            expr = f"({entry.expr})" if needs_parens(entry.expr) else entry.expr
            line = f"#define {entry.name} {expr}"
            if entry.comment:
                line += f" /* {entry.comment} */"
            out.append(line)
            emitted[entry.name] = entry.expr

    out.append("")
    out.append(f"#endif /* {guard} */")
    out.append("")
    return "\n".join(out)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--check", action="store_true", help="Validate generated output without writing files")
    args = ap.parse_args()

    root = Path(__file__).resolve().parents[2]
    asm_dir = root / "asm"
    out_dir = root / "src" / "game" / "include"
    out_dir.mkdir(parents=True, exist_ok=True)

    generated: list[Path] = []
    pending: dict[Path, str] = {}

    for src_path in sorted(asm_dir.glob("*.EQU")):
        banner_comments, globls, sets = parse_equ_file(src_path)
        out_name = sanitize_include_name(src_path)
        out_path = out_dir / out_name
        pending[out_path] = render_header(src_path, banner_comments, globls, sets)
        generated.append(out_path)

    if args.check:
        failed = False
        for out_path, content in pending.items():
            existing = out_path.read_text() if out_path.exists() else None
            if existing != content:
                print(f"out of date: {out_path.relative_to(root)}")
                failed = True
        return 1 if failed else 0

    expected_names = {path.name for path in pending}
    for existing in out_dir.glob("*.h"):
        if existing.name == "types.h":
            continue
        if existing.name not in expected_names:
            existing.unlink()

    for out_path, content in pending.items():
        out_path.write_text(content)
    print(f"generated {len(generated)} headers in {out_dir.relative_to(root)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

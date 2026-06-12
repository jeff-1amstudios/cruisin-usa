#!/usr/bin/env python3
"""Generate C headers from .EQU files."""

from __future__ import annotations

import argparse
import re
from dataclasses import dataclass, field
from pathlib import Path

from gen_c_skeleton import SymbolInfo, collect_module_symbol_table, sanitize_identifier, variable_declaration


SET_RE = re.compile(r"^\s*([A-Za-z_.$?@][A-Za-z0-9_.$?@]*)\s*\.set\s*(.*?)\s*$", re.IGNORECASE)
GLOBL_RE = re.compile(r"^\s*\.globl\s+(.*?)\s*$", re.IGNORECASE)
HEX_SUFFIX_RE = re.compile(r"\b([0-9A-Fa-f]+)[Hh]\b")


@dataclass
class SetEntry:
    name: str
    expr: str
    comment: str
    asm_line: str


@dataclass
class RenderBlock:
    kind: str
    lines: list[str] = field(default_factory=list)


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


def normalize_banner_comments(lines: list[str]) -> list[str]:
    normalized: list[str] = []
    pending_blank = False
    started = False
    for line in lines:
        if not line:
            if started:
                pending_blank = True
            continue
        if pending_blank and normalized:
            normalized.append("")
        normalized.append(line)
        pending_blank = False
        started = True
    return normalized


def normalize_emitted_lines(lines: list[str]) -> list[str]:
    normalized: list[str] = []
    blank_pending = False
    for line in lines:
        if line == "":
            if normalized:
                blank_pending = True
            continue
        if blank_pending and normalized:
            normalized.append("")
        normalized.append(line)
        blank_pending = False
    return normalized


def join_render_blocks(blocks: list[RenderBlock]) -> list[str]:
    out: list[str] = []
    first = True
    for block in blocks:
        if not block.lines:
            continue
        if not first:
            out.append("")
        out.extend(block.lines)
        first = False
    return out


def parse_equ_file(path: Path) -> tuple[list[str], list[str], list[SetEntry]]:
    banner_comments: list[str] = []
    globls: list[str] = []
    sets: list[SetEntry] = []
    in_banner = True

    for raw in path.read_text(errors="ignore").splitlines():
        stripped = raw.strip()
        if not stripped:
            if in_banner:
                banner_comments.append("")
            continue
        if stripped.startswith("*"):
            if in_banner:
                banner_comments.append(stripped[1:].strip())
            continue
        if stripped.startswith(";"):
            if in_banner:
                banner_comments.append(stripped[1:].strip())
            continue

        in_banner = False
        code, comment = split_comment(raw)
        match = GLOBL_RE.match(code)
        if match:
            names = [part.strip() for part in match.group(1).split(",") if part.strip()]
            globls.extend(names)
            continue

        match = SET_RE.match(code)
        if match:
            name, expr = match.groups()
            sets.append(SetEntry(name=name, expr=convert_expr(expr), comment=comment, asm_line=raw.rstrip()))
            continue

    return normalize_banner_comments(banner_comments), globls, sets


def render_header(
    src_path: Path,
    banner_comments: list[str],
    globls: list[str],
    sets: list[SetEntry],
    symbol_table: dict[str, SymbolInfo] | None = None,
) -> str:
    guard = f"{sanitize_guard_stem(src_path.stem)}_H"
    out: list[str] = []
    out.append(f"#ifndef {guard}")
    out.append(f"#define {guard}")
    out.append("")
    out.append(f"/* Generated from asm/{src_path.name}. */")
    if banner_comments:
        out.append("")
        for line in banner_comments:
            out.append(f"// {line}")

    define_blocks: list[RenderBlock] = []
    extern_blocks: list[RenderBlock] = []
    function_blocks: list[RenderBlock] = []
    emitted_sets: dict[str, str] = {}
    emitted_globls: set[str] = set()
    in_banner = True
    set_index = 0
    pending_comments: list[str] = []
    if symbol_table is None:
        symbol_table = {}
    for raw in src_path.read_text(errors="ignore").splitlines():
        stripped = raw.strip()
        if not stripped:
            if not in_banner and pending_comments:
                pending_comments.append("")
            continue
        if stripped.startswith("*") or stripped.startswith(";"):
            if in_banner:
                continue
            comment_body = stripped[1:].strip()
            pending_comments.append(f"// {comment_body}")
            continue

        in_banner = False
        code, _comment = split_comment(raw)
        globl_match = GLOBL_RE.match(code)
        if globl_match:
            comment_lines = [*pending_comments, f"// asm: {raw.rstrip()}"]
            pending_comments = []
            names = [part.strip() for part in globl_match.group(1).split(",") if part.strip()]
            rendered_any = False
            for name in names:
                if name in emitted_globls:
                    continue
                rendered = render_globl_symbol(symbol_table.get(name), name, src_path.stem.upper() == "GLOBALS")
                if rendered is not None:
                    block = RenderBlock(kind="function" if rendered.startswith("void ") else "extern")
                    if not rendered_any:
                        block.lines.extend(comment_lines)
                        rendered_any = True
                    block.lines.append(rendered)
                    if block.kind == "function":
                        function_blocks.append(block)
                    else:
                        extern_blocks.append(block)
                    emitted_globls.add(name)
            continue

        set_match = SET_RE.match(code)
        if not set_match:
            continue

        entry = sets[set_index]
        set_index += 1
        prior_expr = emitted_sets.get(entry.name)
        if prior_expr == entry.expr:
            continue
        block = RenderBlock(kind="define", lines=[*pending_comments])
        pending_comments = []
        if prior_expr is not None:
            block.lines.append(f"#undef {entry.name}")
        block.lines.append(f"// asm: {entry.asm_line}")
        expr = f"({entry.expr})" if needs_parens(entry.expr) else entry.expr
        line = f"#define {entry.name} {expr}"
        if entry.comment:
            line += f" //{entry.comment}"
        block.lines.append(line)
        define_blocks.append(block)
        emitted_sets[entry.name] = entry.expr

    rendered_body = join_render_blocks(define_blocks + extern_blocks + function_blocks)
    if rendered_body:
        out.append("")
        out.extend(normalize_emitted_lines(rendered_body))

    out.append("")
    out.append(f"#endif /* {guard} */")
    out.append("")
    return "\n".join(out)


def render_globl_symbol(symbol: SymbolInfo | None, name: str, sizeless_extern_arrays: bool = False) -> str | None:
    if sizeless_extern_arrays and name == "NULL":
        return None
    if symbol is None:
        return None
    if symbol.kind == "function":
        return f"void {sanitize_identifier(name)}(void);"
    if symbol.kind == "variable":
        if sizeless_extern_arrays and symbol.array_expr is not None:
            ident = sanitize_identifier(symbol.name)
            sep = "" if symbol.c_type.endswith("*") else " "
            return f"extern {symbol.c_type}{sep}{ident}[];"
        return variable_declaration(symbol.name, symbol.c_type, symbol.array_expr, is_extern=True)
    return None


def build_global_symbol_table(root: Path) -> dict[str, SymbolInfo]:
    asm_dir = root / "asm"
    symbol_table: dict[str, SymbolInfo] = {}
    for src_path in sorted(asm_dir.glob("*.ASM")):
        for name, symbol in collect_module_symbol_table(src_path, {}).items():
            symbol_table.setdefault(name, symbol)
    return symbol_table


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--check", action="store_true", help="Validate generated output without writing files")
    args = ap.parse_args()

    root = Path(__file__).resolve().parents[2]
    asm_dir = root / "asm"
    out_dir = root / "src" / "game"
    out_dir.mkdir(parents=True, exist_ok=True)

    generated: list[Path] = []
    pending: dict[Path, str] = {}
    symbol_table = build_global_symbol_table(root)

    for src_path in sorted(asm_dir.glob("*.EQU")):
        banner_comments, globls, sets = parse_equ_file(src_path)
        out_name = sanitize_include_name(src_path)
        out_path = out_dir / out_name
        pending[out_path] = render_header(src_path, banner_comments, globls, sets, symbol_table)
        generated.append(out_path)

    if args.check:
        failed = False
        for out_path, content in pending.items():
            existing = out_path.read_text() if out_path.exists() else None
            if existing != content:
                print(f"out of date: {out_path.relative_to(root)}")
                failed = True
        return 1 if failed else 0

    for out_path, content in pending.items():
        out_path.write_text(content)
    print(f"generated {len(generated)} headers in {out_dir.relative_to(root)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

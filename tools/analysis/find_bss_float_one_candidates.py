#!/usr/bin/env python3
"""
Find single-word BSS globals that likely want `= 1.0f` in the C port.

Heuristic:
- start from asm globals declared via `.bss`, `fbss`, `pbss`, `hibss`, or `lobss`
- keep only declarations whose storage size can be conservatively resolved to 1 word
- scan asm usage sites across `asm/*.ASM`
- flag symbols whose detected asm data reads all use `*F` mnemonics
- reject or mark for review when any non-`*F` read is found, when the size is not a
  single word, when C translation is missing, or when the matching C global is not a
  scalar `float`

Limitations:
- this is intentionally conservative and only understands simple top-level asm and C
  patterns used in this repo
- unresolved size expressions, ambiguous asm operands, and unusual C declarations are
  marked for review instead of guessed
- asm address-like uses are treated conservatively as reads unless they are clearly
  store destinations
"""

from __future__ import annotations

import argparse
import ast
import json
import re
import sys
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any


REPO_ROOT = Path(__file__).resolve().parents[2]
ASM_DIR = REPO_ROOT / "asm"
SRC_DIR = REPO_ROOT / "src"
DEFAULT_REPORT_PATH = REPO_ROOT / "tools" / "analysis" / "bss_float_one_candidates_report.json"
DEFAULT_TEXT_REPORT_PATH = REPO_ROOT / "tools" / "analysis" / "bss_float_one_candidates_report.txt"
DECL_DIRECTIVES = {"bss", "fbss", "pbss", "hibss", "lobss"}
WRITE_PREFIXES = ("ST",)


@dataclass
class Site:
    asm_file: str
    line: int
    mnemonic: str
    text: str


@dataclass
class CMatch:
    file: str | None = None
    line: int | None = None
    decl: str | None = None
    type_class: str = "missing"


@dataclass
class SymbolRecord:
    symbol: str
    directive: str
    asm_file: str
    asm_line: int
    asm_decl: str
    size_expr: str
    size_words: int | None
    reads: list[Site] = field(default_factory=list)
    writes: list[Site] = field(default_factory=list)
    non_float_reads: list[Site] = field(default_factory=list)
    sti_writes: list[Site] = field(default_factory=list)
    c_match: CMatch = field(default_factory=CMatch)
    final_bucket: str = "needs_review"
    final_candidate: bool = False
    notes: list[str] = field(default_factory=list)


class SafeExprEvaluator(ast.NodeVisitor):
    def __init__(self, names: dict[str, int]) -> None:
        self.names = names

    def visit_Expression(self, node: ast.Expression) -> int:
        return self.visit(node.body)

    def visit_Constant(self, node: ast.Constant) -> int:
        if not isinstance(node.value, int):
            raise ValueError("non-int constant")
        return node.value

    def visit_Name(self, node: ast.Name) -> int:
        if node.id not in self.names:
            raise ValueError(f"unknown name {node.id}")
        return self.names[node.id]

    def visit_UnaryOp(self, node: ast.UnaryOp) -> int:
        value = self.visit(node.operand)
        if isinstance(node.op, ast.UAdd):
            return value
        if isinstance(node.op, ast.USub):
            return -value
        raise ValueError("unsupported unary op")

    def visit_BinOp(self, node: ast.BinOp) -> int:
        left = self.visit(node.left)
        right = self.visit(node.right)
        if isinstance(node.op, ast.Add):
            return left + right
        if isinstance(node.op, ast.Sub):
            return left - right
        if isinstance(node.op, ast.Mult):
            return left * right
        if isinstance(node.op, ast.FloorDiv):
            return left // right
        if isinstance(node.op, ast.Div):
            return left // right
        if isinstance(node.op, ast.Mod):
            return left % right
        if isinstance(node.op, ast.LShift):
            return left << right
        if isinstance(node.op, ast.RShift):
            return left >> right
        if isinstance(node.op, ast.BitOr):
            return left | right
        if isinstance(node.op, ast.BitAnd):
            return left & right
        raise ValueError("unsupported binary op")

    def generic_visit(self, node: ast.AST) -> int:
        raise ValueError(f"unsupported node {type(node).__name__}")


def strip_asm_comment(line: str) -> str:
    if ";" in line:
        return line.split(";", 1)[0]
    return line


def normalize_expr(expr: str) -> str:
    expr = expr.strip()
    expr = re.sub(r"\b([0-9A-F]+)H\b", lambda m: f"0x{m.group(1)}", expr, flags=re.IGNORECASE)
    return expr


def eval_expr(expr: str, names: dict[str, int]) -> int | None:
    expr = normalize_expr(expr)
    try:
        tree = ast.parse(expr, mode="eval")
        return SafeExprEvaluator(names).visit(tree)
    except Exception:
        return None


def collect_set_constants(asm_paths: list[Path]) -> dict[str, int]:
    constants: dict[str, int] = {}
    patterns = [
        re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s+\.set\s+(.+?)\s*$", re.IGNORECASE),
        re.compile(r"^\s*\.set\s+([A-Za-z_][A-Za-z0-9_]*),\s*(.+?)\s*$", re.IGNORECASE),
    ]
    progress = True
    while progress:
        progress = False
        for path in asm_paths:
            for raw_line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
                line = strip_asm_comment(raw_line)
                for pattern in patterns:
                    match = pattern.match(line)
                    if not match:
                        continue
                    name = match.group(1)
                    if name in constants:
                        continue
                    value = eval_expr(match.group(2), constants)
                    if value is None:
                        continue
                    constants[name] = value
                    progress = True
    return constants


def parse_declarations(asm_paths: list[Path], constants: dict[str, int]) -> dict[str, SymbolRecord]:
    records: dict[str, SymbolRecord] = {}
    decl_pattern = re.compile(
        r"^\s*(?:([A-Za-z_][A-Za-z0-9_]*):\s*)?(?:\.)?(bss|fbss|pbss|hibss|lobss)\s+([A-Za-z_][A-Za-z0-9_]*),\s*(.+?)\s*$",
        re.IGNORECASE,
    )
    for path in asm_paths:
        for line_no, raw_line in enumerate(path.read_text(encoding="utf-8", errors="ignore").splitlines(), start=1):
            line = strip_asm_comment(raw_line)
            match = decl_pattern.match(line)
            if not match:
                continue
            directive = match.group(2).lower()
            symbol = match.group(3)
            size_expr = match.group(4).strip()
            size_words = eval_expr(size_expr, constants)
            records[symbol] = SymbolRecord(
                symbol=symbol,
                directive=directive,
                asm_file=str(path.relative_to(REPO_ROOT)),
                asm_line=line_no,
                asm_decl=raw_line.rstrip(),
                size_expr=size_expr,
                size_words=size_words,
            )
    return records


def classify_c_decl(decl: str) -> str:
    stripped = decl.strip().rstrip(";").strip()
    if not stripped:
        return "unknown"
    if "(" in stripped:
        return "unknown"
    if "float" in stripped:
        if "*" in stripped:
            return "float_pointer"
        if "[" in stripped:
            return "float_array"
        return "float_scalar"
    if "*" in stripped:
        return "pointer"
    if "[" in stripped:
        return "array"
    return "non_float_scalar"


def index_c_definitions() -> dict[str, CMatch]:
    definitions: dict[str, CMatch] = {}
    comment_symbol: str | None = None
    decl_pattern = re.compile(
        r"^\s*(?!extern\b)(?!typedef\b)(?!return\b)(?:static\s+)?[A-Za-z_][A-Za-z0-9_\s\*]*\b([A-Za-z_][A-Za-z0-9_]*)\b(?:\s*(?:\[.*\])?)\s*(?:=\s*.+)?;\s*$"
    )
    comment_pattern = re.compile(r"^\s*/\*\s*asm:\s*([A-Za-z_][A-Za-z0-9_]*)\b")
    for path in sorted(SRC_DIR.rglob("*.c")):
        lines = path.read_text(encoding="utf-8", errors="ignore").splitlines()
        for idx, line in enumerate(lines, start=1):
            comment_match = comment_pattern.search(line)
            if comment_match:
                comment_symbol = comment_match.group(1)
                continue
            decl_match = decl_pattern.match(line)
            if not decl_match:
                continue
            decl = line.strip()
            symbol = decl_match.group(1)
            type_class = classify_c_decl(decl)
            match = CMatch(
                file=str(path.relative_to(REPO_ROOT)),
                line=idx,
                decl=decl,
                type_class=type_class,
            )
            definitions.setdefault(symbol, match)
            if comment_symbol and comment_symbol not in definitions:
                definitions[comment_symbol] = match
            comment_symbol = None
    return definitions


def tokenize_operands(operand_text: str) -> list[str]:
    return [token.strip() for token in operand_text.split(",") if token.strip()]


def extract_referenced_symbols(operand: str) -> set[str]:
    return set(re.findall(r"@?([A-Za-z_][A-Za-z0-9_]*)", operand))


def classify_usage(symbol: str, mnemonic: str, operands: list[str]) -> tuple[bool, bool]:
    symbol_indexes = [idx for idx, operand in enumerate(operands) if symbol in extract_referenced_symbols(operand)]
    if not symbol_indexes:
        return False, False
    if any(op.upper().startswith("@AR") or op.upper().startswith("*AR") for op in operands):
        return False, False

    is_write = False
    if mnemonic.upper().startswith(WRITE_PREFIXES):
        last_index = len(operands) - 1
        if last_index in symbol_indexes:
            is_write = True

    is_read = False
    for idx in symbol_indexes:
        if is_write and idx == len(operands) - 1:
            continue
        is_read = True
        break
    return is_read, is_write


def scan_usage(records: dict[str, SymbolRecord], asm_paths: list[Path]) -> None:
    if not records:
        return
    instruction_pattern = re.compile(r"^\s*(?:[A-Za-z_][A-Za-z0-9_]*:\s*)?(?:\|\|\s*)?([A-Za-z][A-Za-z0-9]*)\b(.*)$")
    for path in asm_paths:
        lines = path.read_text(encoding="utf-8", errors="ignore").splitlines()
        for line_no, raw_line in enumerate(lines, start=1):
            line = strip_asm_comment(raw_line).rstrip()
            if not line or line.lstrip().startswith("*"):
                continue
            match = instruction_pattern.match(line)
            if not match:
                continue
            mnemonic = match.group(1).upper()
            operands = tokenize_operands(match.group(2))
            if not operands:
                continue
            referenced_symbols: set[str] = set()
            for operand in operands:
                referenced_symbols.update(extract_referenced_symbols(operand))
            for symbol in referenced_symbols.intersection(records):
                record = records[symbol]
                is_read, is_write = classify_usage(symbol, mnemonic, operands)
                if not is_read and not is_write:
                    continue
                site = Site(
                    asm_file=str(path.relative_to(REPO_ROOT)),
                    line=line_no,
                    mnemonic=mnemonic,
                    text=raw_line.rstrip(),
                )
                if is_read:
                    record.reads.append(site)
                    if not mnemonic.endswith("F"):
                        record.non_float_reads.append(site)
                if is_write:
                    record.writes.append(site)
                    if mnemonic.startswith("STI"):
                        record.sti_writes.append(site)


def evaluate_records(records: dict[str, SymbolRecord], c_index: dict[str, CMatch]) -> dict[str, list[SymbolRecord]]:
    buckets: dict[str, list[SymbolRecord]] = {
        "candidates": [],
        "raw_review_non_float_c": [],
        "excluded_non_single_word": [],
        "excluded_non_float_reads": [],
        "excluded_sti_writes": [],
        "excluded_no_reads_or_writes": [],
        "needs_review": [],
    }
    for record in records.values():
        record.c_match = c_index.get(record.symbol, CMatch(type_class="missing"))

        if record.size_words != 1:
            if record.size_words is None:
                record.notes.append("size expression could not be resolved conservatively")
            else:
                record.notes.append(f"size is {record.size_words} words")
            record.final_bucket = "excluded_non_single_word"
            buckets[record.final_bucket].append(record)
            continue

        if record.sti_writes:
            record.notes.append("found STI asm writes")
            record.final_bucket = "excluded_sti_writes"
            buckets[record.final_bucket].append(record)
            continue

        if not record.reads and not record.writes:
            record.notes.append("no asm reads or writes found")
            record.final_bucket = "excluded_no_reads_or_writes"
            buckets[record.final_bucket].append(record)
            continue

        if not record.reads:
            record.notes.append("no asm read sites found")
            record.final_bucket = "needs_review"
            buckets[record.final_bucket].append(record)
            continue

        if record.non_float_reads:
            record.notes.append("found non-*F asm reads")
            record.final_bucket = "excluded_non_float_reads"
            buckets[record.final_bucket].append(record)
            continue

        if record.c_match.type_class == "float_scalar":
            record.final_candidate = True
            record.final_bucket = "candidates"
            buckets[record.final_bucket].append(record)
            continue

        if record.c_match.type_class in {"non_float_scalar", "pointer", "array", "float_array", "float_pointer"}:
            record.notes.append(f"matching C declaration is {record.c_match.type_class}")
            record.final_bucket = "raw_review_non_float_c"
            buckets[record.final_bucket].append(record)
            continue

        record.notes.append("matching C declaration not found or ambiguous")
        record.final_bucket = "needs_review"
        buckets[record.final_bucket].append(record)
    return buckets


def record_to_json(record: SymbolRecord) -> dict[str, Any]:
    return {
        "symbol": record.symbol,
        "directive": record.directive,
        "asm_file": record.asm_file,
        "asm_declaration_line": record.asm_line,
        "asm_declaration": record.asm_decl,
        "size_expr": record.size_expr,
        "size_words": record.size_words,
        "c_file": record.c_match.file,
        "c_line": record.c_match.line,
        "c_declaration": record.c_match.decl,
        "c_type_class": record.c_match.type_class,
        "reads": [asdict(site) for site in record.reads],
        "writes": [asdict(site) for site in record.writes],
        "has_non_float_reads": bool(record.non_float_reads),
        "non_float_reads": [asdict(site) for site in record.non_float_reads],
        "has_sti_writes": bool(record.sti_writes),
        "sti_writes": [asdict(site) for site in record.sti_writes],
        "final_candidate": record.final_candidate,
        "final_bucket": record.final_bucket,
        "notes": record.notes,
    }


def build_report(buckets: dict[str, list[SymbolRecord]], asm_paths: list[Path]) -> dict[str, Any]:
    return {
        "repo_root": str(REPO_ROOT),
        "asm_files_scanned": [str(path.relative_to(REPO_ROOT)) for path in asm_paths],
        "summary": {
            "candidate_count": len(buckets["candidates"]),
            "raw_review_non_float_c_count": len(buckets["raw_review_non_float_c"]),
            "excluded_non_single_word_count": len(buckets["excluded_non_single_word"]),
            "excluded_non_float_reads_count": len(buckets["excluded_non_float_reads"]),
            "excluded_sti_writes_count": len(buckets["excluded_sti_writes"]),
            "excluded_no_reads_or_writes_count": len(buckets["excluded_no_reads_or_writes"]),
            "needs_review_count": len(buckets["needs_review"]),
        },
        "candidates": [record_to_json(record) for record in sorted(buckets["candidates"], key=lambda item: item.symbol)],
        "raw_review_non_float_c": [record_to_json(record) for record in sorted(buckets["raw_review_non_float_c"], key=lambda item: item.symbol)],
        "excluded_non_single_word": [record_to_json(record) for record in sorted(buckets["excluded_non_single_word"], key=lambda item: item.symbol)],
        "excluded_non_float_reads": [record_to_json(record) for record in sorted(buckets["excluded_non_float_reads"], key=lambda item: item.symbol)],
        "excluded_sti_writes": [record_to_json(record) for record in sorted(buckets["excluded_sti_writes"], key=lambda item: item.symbol)],
        "excluded_no_reads_or_writes": [record_to_json(record) for record in sorted(buckets["excluded_no_reads_or_writes"], key=lambda item: item.symbol)],
        "needs_review": [record_to_json(record) for record in sorted(buckets["needs_review"], key=lambda item: item.symbol)],
    }


def format_site(site: dict[str, Any]) -> str:
    return f"{site['asm_file']}:{site['line']} {site['mnemonic']}  {site['text'].strip()}"


def build_text_report(report: dict[str, Any]) -> str:
    summary = report["summary"]
    lines: list[str] = []
    lines.append("BSS float-one candidate analysis")
    lines.append(f"repo: {report['repo_root']}")
    lines.append("")
    lines.append("Summary")
    lines.append(f"  candidates: {summary['candidate_count']}")
    lines.append(f"  raw review (non-float C): {summary['raw_review_non_float_c_count']}")
    lines.append(f"  excluded non-single-word: {summary['excluded_non_single_word_count']}")
    lines.append(f"  excluded non-*F reads: {summary['excluded_non_float_reads_count']}")
    lines.append(f"  excluded STI writes: {summary['excluded_sti_writes_count']}")
    lines.append(f"  excluded no reads or writes: {summary['excluded_no_reads_or_writes_count']}")
    lines.append(f"  needs review: {summary['needs_review_count']}")

    bucket_labels = [
        ("candidates", "Candidates"),
        ("raw_review_non_float_c", "Raw Review: Non-Float C"),
        ("needs_review", "Needs Review"),
    ]

    for bucket_name, title in bucket_labels:
        items = report[bucket_name]
        lines.append("")
        lines.append(f"{title} ({len(items)})")
        lines.append("=" * (len(title) + len(f" ({len(items)})")))
        if not items:
            lines.append("  none")
            continue

        for item in items:
            lines.append("")
            lines.append(f"- {item['symbol']} [{'yes' if item['final_candidate'] else 'no'}]")
            lines.append(
                f"  asm: {item['asm_file']}:{item['asm_declaration_line']}  {item['asm_declaration'].strip()}"
            )
            lines.append(f"  size: expr={item['size_expr']} words={item['size_words']}")
            if item["c_file"] is None:
                lines.append("  c: missing")
            else:
                lines.append(
                    f"  c: {item['c_file']}:{item['c_line']}  {item['c_declaration']}  ({item['c_type_class']})"
                )
            lines.append(f"  has_non_float_reads: {item['has_non_float_reads']}")
            lines.append(f"  has_sti_writes: {item['has_sti_writes']}")
            lines.append(f"  final_bucket: {item['final_bucket']}")
            if item["notes"]:
                lines.append(f"  notes: {'; '.join(item['notes'])}")

            lines.append("  reads:")
            if item["reads"]:
                for site in item["reads"]:
                    lines.append(f"    {format_site(site)}")
            else:
                lines.append("    none")

            lines.append("  writes:")
            if item["writes"]:
                for site in item["writes"]:
                    lines.append(f"    {format_site(site)}")
            else:
                lines.append("    none")

            if item["non_float_reads"]:
                lines.append("  non-*F reads:")
                for site in item["non_float_reads"]:
                    lines.append(f"    {format_site(site)}")

            if item["sti_writes"]:
                lines.append("  STI writes:")
                for site in item["sti_writes"]:
                    lines.append(f"    {format_site(site)}")

    lines.append("")
    return "\n".join(lines)


def print_summary(report: dict[str, Any], report_path: Path) -> None:
    summary = report["summary"]
    print("BSS float-one candidate analysis", file=sys.stderr)
    print(f"repo: {report['repo_root']}", file=sys.stderr)
    print(f"candidates: {summary['candidate_count']}", file=sys.stderr)
    print(f"raw review (non-float C): {summary['raw_review_non_float_c_count']}", file=sys.stderr)
    print(f"excluded non-single-word: {summary['excluded_non_single_word_count']}", file=sys.stderr)
    print(f"excluded non-*F reads: {summary['excluded_non_float_reads_count']}", file=sys.stderr)
    print(f"excluded STI writes: {summary['excluded_sti_writes_count']}", file=sys.stderr)
    print(f"excluded no reads or writes: {summary['excluded_no_reads_or_writes_count']}", file=sys.stderr)
    print(f"needs review: {summary['needs_review_count']}", file=sys.stderr)
    print(f"json report: {report_path}", file=sys.stderr)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--json",
        action="store_true",
        help="emit the full JSON report to stdout; the human summary is printed to stderr",
    )
    parser.add_argument(
        "--report-path",
        type=Path,
        default=DEFAULT_REPORT_PATH,
        help="path to write the JSON report",
    )
    parser.add_argument(
        "--text-report-path",
        type=Path,
        default=DEFAULT_TEXT_REPORT_PATH,
        help="path to write the human-readable text report",
    )
    args = parser.parse_args()

    asm_paths = sorted(ASM_DIR.glob("*.ASM"))
    constants = collect_set_constants(asm_paths)
    records = parse_declarations(asm_paths, constants)
    scan_usage(records, asm_paths)
    c_index = index_c_definitions()
    buckets = evaluate_records(records, c_index)
    report = build_report(buckets, asm_paths)

    args.report_path.parent.mkdir(parents=True, exist_ok=True)
    args.report_path.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    text_report = build_text_report(report)
    args.text_report_path.parent.mkdir(parents=True, exist_ok=True)
    args.text_report_path.write_text(text_report, encoding="utf-8")

    print_summary(report, args.report_path)
    print(f"text report: {args.text_report_path}", file=sys.stderr)
    if args.json:
        json.dump(report, sys.stdout, indent=2, sort_keys=True)
        sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

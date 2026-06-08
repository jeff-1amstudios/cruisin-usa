#!/usr/bin/env python3
from __future__ import annotations

import csv
import pathlib
import sys
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))

import walk_source_and_rom as mapper  # type: ignore
import shared_lib as ccm  # type: ignore

ROOT = pathlib.Path(__file__).resolve().parents[2]
LOG_DIR = pathlib.Path(__file__).resolve().parent / "log"
OUT = LOG_DIR / "romlst_inferred_first_code_labels.tsv"


@dataclass
class SourceLabel:
    module: str
    label: str
    line: int
    op_offset: int
    code_words_until_next_label: int = 0
    executable_start: bool = False
    known_name: str = ""
    known_addr: Optional[int] = None

    @property
    def is_code_label(self) -> bool:
        return self.executable_start


def source_labels_with_code_offsets(
    asm_path: pathlib.Path,
    macros: Dict[str, ccm.MacroDef],
    symbols: Dict[str, int],
) -> List[SourceLabel]:
    """Return source labels positioned in the expanded code-op stream.

    This deliberately counts executable source operations only. It is useful
    for calculating labels inside code ranges from nearby known code labels,
    but it is not a linker-section model for .bss/.usect/.string/.word data.
    """
    module = asm_path.stem
    lines = asm_path.read_text(errors="ignore").splitlines()
    expanded = mapper.iter_expanded_lines_with_lineno(lines, 1, macros, symbols)
    ops = mapper.parse_ops_with_lineno(expanded)
    ops_by_line: Dict[int, int] = {}
    for lineno, _op, _toks in ops:
        ops_by_line[lineno] = ops_by_line.get(lineno, 0) + 1

    labels: List[SourceLabel] = []
    op_offset = 0
    active_lines = mapper.iter_active_raw_with_lineno(lines, 1, symbols)
    for lineno, raw in active_lines:
        code = ccm.strip_comment(raw)
        lbl, _rest = ccm.split_optional_label(code)
        if lbl and mapper.LABEL_TOKEN_RE.match(lbl):
            if not mapper.is_pseudo_local_label(lbl) and not lbl.endswith("?"):
                labels.append(SourceLabel(module=module, label=lbl, line=lineno, op_offset=op_offset))
        op_offset += ops_by_line.get(lineno, 0)

    label_lines = [item.line for item in labels]
    active_line_numbers = [lineno for lineno, _raw in active_lines]
    active_line_set = set(active_line_numbers)
    for idx, item in enumerate(labels):
        next_line = label_lines[idx + 1] if idx + 1 < len(label_lines) else len(lines) + 1
        item.code_words_until_next_label = sum(
            count for lineno, count in ops_by_line.items() if item.line <= lineno < next_line
        )
        if ops_by_line.get(item.line, 0) > 0:
            item.executable_start = True
            continue
        # Label-only function labels are executable only when the next active
        # source line in the same label block emits an instruction/macro op.
        # Data directives such as .word/.string/.bss do not qualify.
        for lineno in active_line_numbers:
            if lineno <= item.line:
                continue
            if lineno >= next_line:
                break
            if lineno not in active_line_set:
                continue
            item.executable_start = ops_by_line.get(lineno, 0) > 0
            break
    return labels


def attach_known_addresses(
    labels: List[SourceLabel],
    address_map: Dict[str, int],
) -> None:
    for item in labels:
        found = mapper.lookup_address_map_entry(item.label, item.module, address_map)
        if found is None:
            continue
        item.known_name, item.known_addr = found


def infer_first_for_module(labels: List[SourceLabel]) -> List[Dict[str, str]]:
    code_labels = [item for item in labels if item.is_code_label]
    known = [item for item in code_labels if item.known_addr is not None]
    rows: List[Dict[str, str]] = []
    if not code_labels or not known:
        return rows

    item = code_labels[0]
    known_by_offset = sorted(known, key=lambda x: (x.op_offset, x.line, x.label))
    before: Optional[SourceLabel] = None
    after: Optional[SourceLabel] = None
    for candidate in known_by_offset:
        if candidate.op_offset <= item.op_offset:
            before = candidate
        if candidate.op_offset >= item.op_offset:
            after = candidate
            break

    from_before: Optional[int] = None
    from_after: Optional[int] = None
    if before is not None and before.known_addr is not None:
        from_before = before.known_addr + (item.op_offset - before.op_offset)
    if after is not None and after.known_addr is not None:
        from_after = after.known_addr - (after.op_offset - item.op_offset)

    if item.known_addr is not None:
        inferred = item.known_addr
        status = "known"
    elif from_before is not None and from_after is not None:
        inferred = from_before
        status = "bounded_ok" if from_before == from_after else "bounded_conflict"
    elif from_before is not None:
        inferred = from_before
        status = "forward_extrapolated"
    elif from_after is not None:
        inferred = from_after
        status = "backward_extrapolated"
    else:
        return rows
    if status == "known":
        return rows

    nearest = before if after is None else after if before is None else (
        before if abs(item.op_offset - before.op_offset) <= abs(after.op_offset - item.op_offset) else after
    )
    rows.append(
        {
            "module": item.module,
            "label": item.label,
            "line": str(item.line),
            "op_offset": f"0x{item.op_offset:X}",
            "code_words_until_next_label": f"0x{item.code_words_until_next_label:X}",
            "executable_start": "1" if item.executable_start else "0",
            "inferred_addr": f"0x{inferred:08X}",
            "status": status,
            "known_name": item.known_name,
            "known_addr": "" if item.known_addr is None else f"0x{item.known_addr:08X}",
            "before_label": "" if before is None else before.label,
            "before_addr": "" if before is None or before.known_addr is None else f"0x{before.known_addr:08X}",
            "before_delta": "" if before is None else f"0x{item.op_offset - before.op_offset:X}",
            "after_label": "" if after is None else after.label,
            "after_addr": "" if after is None or after.known_addr is None else f"0x{after.known_addr:08X}",
            "after_delta": "" if after is None else f"0x{after.op_offset - item.op_offset:X}",
            "nearest_anchor": "" if nearest is None else nearest.label,
        }
    )
    return rows


def main() -> int:
    LOG_DIR.mkdir(parents=True, exist_ok=True)
    macros = ccm.parse_macros(ROOT)
    symbols = ccm.parse_set_symbols(ROOT)
    address_map = mapper.parse_address_map(pathlib.Path(__file__).resolve().parent / "address.map")

    all_rows: List[Dict[str, str]] = []
    modules_seen = 0
    modules_with_known = 0
    for asm_path in ccm.iter_source_files(ROOT, (".ASM",)):
        modules_seen += 1
        labels = source_labels_with_code_offsets(asm_path, macros, symbols)
        attach_known_addresses(labels, address_map)
        if not any(item.known_addr is not None for item in labels):
            continue
        modules_with_known += 1
        all_rows.extend(infer_first_for_module(labels))

    fields = [
        "module",
        "label",
        "line",
        "op_offset",
        "code_words_until_next_label",
        "executable_start",
        "inferred_addr",
        "status",
        "known_name",
        "known_addr",
        "before_label",
        "before_addr",
        "before_delta",
        "after_label",
        "after_addr",
        "after_delta",
        "nearest_anchor",
    ]
    with OUT.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fields, delimiter="\t")
        writer.writeheader()
        writer.writerows(all_rows)

    by_status: Dict[str, int] = {}
    for row in all_rows:
        by_status[row["status"]] = by_status.get(row["status"], 0) + 1
    print(f"modules_seen={modules_seen} modules_with_known={modules_with_known} rows={len(all_rows)} out={OUT}")
    for status, count in sorted(by_status.items()):
        print(f"{status}={count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

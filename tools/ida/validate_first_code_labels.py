#!/usr/bin/env python3
from __future__ import annotations

import csv
import pathlib
import sys
from typing import Dict, List, Optional, Tuple

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))

import walk_source_and_rom as mapper  # type: ignore
import shared_lib as ccm  # type: ignore

ROOT = pathlib.Path(__file__).resolve().parents[2]
TOOLS_IDA = pathlib.Path(__file__).resolve().parent
LOG_DIR = TOOLS_IDA / "log"
INFERRED = LOG_DIR / "romlst_inferred_first_code_labels.tsv"
OUT = LOG_DIR / "romlst_inferred_first_code_labels_validation.tsv"
ROM_LST = pathlib.Path("/Users/j.harris/Downloads/carma/cruisin/crusnusa45_maindata_interleaved_bswap32.bin.lst")
CHECK_WORDS = 8


def parse_addr(s: str) -> int:
    return int(s, 16) if s.lower().startswith("0x") else int(s, 10)


def rom_lines_by_addr(path: pathlib.Path) -> Dict[int, str]:
    out: Dict[int, str] = {}
    for ln in path.read_text(errors="ignore").splitlines():
        m = mapper.ROM_LINE_RE.match(ln)
        if not m:
            continue
        out[int(m.group(1), 16)] = m.group(2).strip()
    return out


def rom_op_at(rhs: str) -> Optional[str]:
    if not rhs:
        return None
    if ":" in rhs:
        l, r = rhs.split(":", 1)
        if mapper.LABEL_TOKEN_RE.match(l.strip()):
            rhs = r.strip()
    if not rhs or rhs.startswith(";") or rhs.startswith("."):
        return None
    toks = [t for t in mapper.TOKEN_SPLIT_RE.split(rhs) if t]
    if not toks:
        return None
    op = toks[0].upper()
    if op.startswith("||"):
        op = op[2:]
    return op


def source_ops_from(module: str, line: int, macros: Dict[str, ccm.MacroDef], symbols: Dict[str, int]) -> List[Tuple[int, str]]:
    p = mapper.find_module_source_path(ROOT, module)
    if p is None:
        raise FileNotFoundError(f"missing source module: {module}")
    lines = p.read_text(errors="ignore").splitlines()
    expanded = mapper.iter_expanded_lines_with_lineno(lines[line - 1:], line, macros, symbols)
    return [(lineno, op) for lineno, op, _toks in mapper.parse_ops_with_lineno(expanded)]


def main() -> int:
    LOG_DIR.mkdir(parents=True, exist_ok=True)
    if not INFERRED.exists():
        raise SystemExit(f"missing inferred report: {INFERRED}")
    if not ROM_LST.exists():
        raise SystemExit(f"missing rom listing: {ROM_LST}")

    macros = ccm.parse_macros(ROOT)
    symbols = ccm.parse_set_symbols(ROOT)
    raw_rom = rom_lines_by_addr(ROM_LST)

    fields = [
        "module",
        "label",
        "line",
        "inferred_addr",
        "status",
        "checked",
        "result",
        "first_mismatch_addr",
        "first_mismatch_source_line",
        "source_op",
        "rom_op",
        "rom_text",
    ]
    rows_out: List[Dict[str, str]] = []
    with INFERRED.open(newline="") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            module = row["module"]
            label = row["label"]
            line = int(row["line"])
            addr = parse_addr(row["inferred_addr"])
            src_ops = source_ops_from(module, line, macros, symbols)[:CHECK_WORDS]
            result = "ok"
            checked = 0
            mismatch_addr = ""
            mismatch_line = ""
            source_op = ""
            rom_op = ""
            rom_text = ""
            for idx, (src_line, sop) in enumerate(src_ops):
                ea = addr + idx
                rhs = raw_rom.get(ea, "")
                rop = rom_op_at(rhs)
                if rop is None:
                    result = "rom_not_code"
                    mismatch_addr = f"0x{ea:08X}"
                    mismatch_line = str(src_line)
                    source_op = sop
                    rom_op = ""
                    rom_text = rhs
                    break
                checked += 1
                if not ccm.mnemonics_equivalent([sop.lower()], [rop.lower()]):
                    result = "mismatch"
                    mismatch_addr = f"0x{ea:08X}"
                    mismatch_line = str(src_line)
                    source_op = sop
                    rom_op = rop
                    rom_text = rhs
                    break
            rows_out.append(
                {
                    "module": module,
                    "label": label,
                    "line": str(line),
                    "inferred_addr": row["inferred_addr"],
                    "status": row["status"],
                    "checked": f"0x{checked:X}",
                    "result": result,
                    "first_mismatch_addr": mismatch_addr,
                    "first_mismatch_source_line": mismatch_line,
                    "source_op": source_op,
                    "rom_op": rom_op,
                    "rom_text": rom_text,
                }
            )

    with OUT.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fields, delimiter="\t")
        writer.writeheader()
        writer.writerows(rows_out)

    counts: Dict[str, int] = {}
    for row in rows_out:
        counts[row["result"]] = counts.get(row["result"], 0) + 1
    print(f"rows={len(rows_out)} out={OUT}")
    for result, count in sorted(counts.items()):
        print(f"{result}={count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

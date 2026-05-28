#!/usr/bin/env python3
from __future__ import annotations

import argparse
import pathlib
import re
import sys
from collections import defaultdict, deque
from dataclasses import dataclass
from typing import Dict, List, Set, Tuple

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import cint_call_match as ccm  # type: ignore

@dataclass
class Func:
    name: str
    module: str
    lines: List[str]


def parse_functions(root: pathlib.Path) -> Dict[str, Func]:
    funcs: Dict[str, Func] = {}
    # Reuse canonical source-function index and extractor logic.
    idx = ccm._source_function_index(root)  # type: ignore[attr-defined]
    for fname, entries in idx.items():
        if not entries:
            continue
        # Use first definition; multi-def labels are typically local alias collisions.
        module_path, start_idx = entries[0]
        lines = ccm.extract_source_function_lines(module_path, fname, start_idx)
        funcs.setdefault(fname, Func(fname, module_path.stem, lines))
    return funcs


def rom_label_addr_map(rom_path: pathlib.Path) -> Dict[str, int]:
    out: Dict[str, int] = {}
    lab_re = re.compile(r"^CODE:([0-9A-Fa-f]{8})\s+([A-Za-z_.$?][A-Za-z0-9_.$?]*):")
    for raw in rom_path.read_text(errors="ignore").splitlines():
        m = lab_re.match(raw)
        if not m:
            continue
        out.setdefault(m.group(2), int(m.group(1), 16))
    return out


def main() -> None:
    ap = argparse.ArgumentParser(description="Transitive call-web spider from seed labels")
    ap.add_argument("--root", default=".", help="Project root")
    ap.add_argument("--rom", default="roms/rom.lst", help="ROM listing path relative to root")
    ap.add_argument("--seeds", default="_c_int00", help="Comma-separated seed labels")
    ap.add_argument("--out-prefix", default="/private/tmp/call_web", help="Output prefix path")
    args = ap.parse_args()

    root = pathlib.Path(args.root).resolve()
    rom_path = (root / args.rom).resolve()
    macros = ccm.parse_macros(root)
    symbols = ccm.parse_set_symbols(root)
    symbols["MEMTESTS"] = symbols.get("MEMTESTS", 1)
    symbols["STATISTICS"] = symbols.get("STATISTICS", 0)

    funcs = parse_functions(root)
    rom_addrs = rom_label_addr_map(rom_path) if rom_path.exists() else {}

    edges: Dict[str, Set[str]] = defaultdict(set)
    unresolved: Dict[str, Set[str]] = defaultdict(set)
    for fname, f in funcs.items():
        calls = ccm.extract_calls_from_lines(f.lines, macros, symbols)
        for c in calls:
            if c in funcs:
                edges[fname].add(c)
            else:
                unresolved[fname].add(c)

    seeds = [s.strip() for s in args.seeds.split(",") if s.strip()]
    reachable: Set[str] = set()
    q: deque[str] = deque([s for s in seeds if s in funcs])
    while q:
        cur = q.popleft()
        if cur in reachable:
            continue
        reachable.add(cur)
        for nxt in edges.get(cur, set()):
            if nxt not in reachable:
                q.append(nxt)

    out_prefix = pathlib.Path(args.out_prefix)
    nodes_path = out_prefix.with_suffix(".nodes.tsv")
    edges_path = out_prefix.with_suffix(".edges.tsv")
    unresolved_path = out_prefix.with_suffix(".unresolved.tsv")
    summary_path = out_prefix.with_suffix(".summary.txt")

    with nodes_path.open("w") as f:
        f.write("label\tmodule\trom_address\treachable\n")
        for name in sorted(funcs.keys()):
            func = funcs[name]
            addr = rom_addrs.get(name)
            addr_txt = f"0x{addr:08X}" if addr is not None else ""
            f.write(f"{name}\t{func.module}\t{addr_txt}\t{1 if name in reachable else 0}\n")

    with edges_path.open("w") as f:
        f.write("caller\tcaller_module\tcallee\tcallee_module\tcaller_rom\tcallee_rom\treachable_caller\n")
        for caller in sorted(edges.keys()):
            cm = funcs[caller].module
            caddr = rom_addrs.get(caller)
            caddr_txt = f"0x{caddr:08X}" if caddr is not None else ""
            for callee in sorted(edges[caller]):
                mm = funcs[callee].module
                a2 = rom_addrs.get(callee)
                a2_txt = f"0x{a2:08X}" if a2 is not None else ""
                f.write(
                    f"{caller}\t{cm}\t{callee}\t{mm}\t{caddr_txt}\t{a2_txt}\t{1 if caller in reachable else 0}\n"
                )

    with unresolved_path.open("w") as f:
        f.write("caller\tcaller_module\ttarget\n")
        for caller in sorted(unresolved.keys()):
            cm = funcs[caller].module
            for tgt in sorted(unresolved[caller]):
                f.write(f"{caller}\t{cm}\t{tgt}\n")

    with summary_path.open("w") as f:
        f.write(f"seed_count={len(seeds)}\n")
        f.write(f"functions_total={len(funcs)}\n")
        f.write(f"functions_reachable={len(reachable)}\n")
        f.write(f"edges_total={sum(len(v) for v in edges.values())}\n")
        f.write(f"unresolved_targets_total={sum(len(v) for v in unresolved.values())}\n")

    print(f"wrote {nodes_path}")
    print(f"wrote {edges_path}")
    print(f"wrote {unresolved_path}")
    print(f"wrote {summary_path}")


if __name__ == "__main__":
    main()

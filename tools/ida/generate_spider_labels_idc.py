#!/usr/bin/env python3
"""Generate an IDC script that applies spider-discovered labels in IDA 7.6.

Usage:
  python3 tools/ida/generate_spider_labels_idc.py \
    --labels-tsv /private/tmp/spider_addr.tsv \
    --out /private/tmp/apply_spider_labels.idc

Optional data marking:
  --data-tsv file with tab-separated columns:
    address<TAB>label
  address may be hex like 0x00007AD1 or decimal.
"""

from __future__ import annotations

import argparse
import csv
import pathlib
import re
from dataclasses import dataclass
from typing import Dict, List, Set, Tuple


@dataclass(frozen=True)
class LabelRow:
    raw_label: str
    norm_label: str
    module: str
    kind: str
    address: int
    status: str


def parse_int(s: str) -> int:
    s = s.strip()
    if s.lower().startswith("0x"):
        return int(s, 16)
    return int(s, 10)


def sanitize_ida_name(name: str) -> str:
    # Keep names deterministic and IDA-friendly.
    out = re.sub(r"[^A-Za-z0-9_.$?]", "_", name)
    if not out:
        out = "lbl"
    if out[0].isdigit():
        out = "_" + out
    return out


def load_labels(path: pathlib.Path) -> List[LabelRow]:
    rows: List[LabelRow] = []
    with path.open(newline="") as f:
        r = csv.DictReader(f, delimiter="\t")
        for rec in r:
            status = (rec.get("status") or "").strip()
            if status != "OK":
                continue
            addr_s = (rec.get("address") or "").strip()
            if not addr_s:
                continue
            addr = parse_int(addr_s)
            raw_label = (rec.get("label") or "").strip()
            norm_label = (rec.get("label_norm") or raw_label).strip()
            module = (rec.get("module") or "").strip()
            kind = (rec.get("kind") or "code").strip().lower()
            rows.append(
                LabelRow(
                    raw_label=raw_label,
                    norm_label=norm_label,
                    module=module,
                    kind=kind,
                    address=addr,
                    status=status,
                )
            )
    return rows


def load_data_marks(path: pathlib.Path | None) -> Dict[int, str]:
    out: Dict[int, str] = {}
    if path is None:
        return out
    with path.open(newline="") as f:
        for ln in f:
            s = ln.strip()
            if not s or s.startswith("#"):
                continue
            parts = s.split("\t")
            addr = parse_int(parts[0])
            label = parts[1].strip() if len(parts) > 1 else ""
            out[addr] = label
    return out


def load_dp0_marks(path: pathlib.Path | None) -> List[int]:
    out: List[int] = []
    if path is None:
        return out
    with path.open(newline="") as f:
        for ln in f:
            s = ln.strip()
            if not s or s.startswith("#") or s.lower() == "address":
                continue
            out.append(parse_int(s))
    return sorted(set(out))


def choose_unique_names(rows: List[LabelRow]) -> List[Tuple[int, str, str, str]]:
    # Returns tuples: (address, ida_name, kind, comment_text)
    used: Set[str] = set()
    out: List[Tuple[int, str, str, str]] = []
    by_addr: Dict[int, List[LabelRow]] = {}
    for r in rows:
        by_addr.setdefault(r.address, []).append(r)

    for addr in sorted(by_addr.keys()):
        group = by_addr[addr]
        best = group[0]
        base = sanitize_ida_name(best.norm_label)
        candidate = base
        i = 2
        while candidate in used:
            candidate = f"{base}_{i}"
            i += 1
        used.add(candidate)

        comment = ""
        if len(group) > 1:
            alts = ", ".join(sorted({g.norm_label for g in group[1:]}))
            comment = f"alts: {alts}"
        # If any source row at this EA says data, prefer data.
        kinds = {g.kind for g in group}
        kind = "data" if "data" in kinds else "code"
        out.append((addr, candidate, kind, comment))
    return out


def generate_idc(labels: List[Tuple[int, str, str, str]], data_marks: Dict[int, str], dp0_marks: List[int]) -> str:
    lines: List[str] = []
    lines.append("#include <idc.idc>")
    lines.append("")
    lines.append("static in_mapped_memory(ea)")
    lines.append("{")
    lines.append("  return get_segm_start(ea) != BADADDR;")
    lines.append("}")
    lines.append("")
    lines.append("static set_name_safe(ea, name)")
    lines.append("{")
    lines.append("  set_name(ea, name, SN_FORCE);")
    lines.append("}")
    lines.append("")
    lines.append("static clear_name_safe(ea)")
    lines.append("{")
    lines.append("  // Remove existing user label at this address, if any.")
    lines.append('  set_name(ea, "", SN_FORCE);')
    lines.append("}")
    lines.append("")
    lines.append("static force_data_word(ea)")
    lines.append("{")
    lines.append("  // On C31 in IDA, one address unit is a 32-bit wide byte.")
    lines.append("  del_items(ea, DELIT_SIMPLE, 1);")
    lines.append("  create_byte(ea);")
    lines.append("}")
    lines.append("")
    lines.append("static force_code_word(ea)")
    lines.append("{")
    lines.append("  del_items(ea, DELIT_SIMPLE, 1);")
    lines.append("  create_insn(ea);")
    lines.append("}")
    lines.append("")
    lines.append("static bswap32(v)")
    lines.append("{")
    lines.append("  auto b0, b1, b2, b3;")
    lines.append("  b0 = (v >> 24) & 0xFF;")
    lines.append("  b1 = (v >> 16) & 0xFF;")
    lines.append("  b2 = (v >> 8) & 0xFF;")
    lines.append("  b3 = v & 0xFF;")
    lines.append("  return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;")
    lines.append("}")
    lines.append("")
    lines.append("static maybe_make_func(ea)")
    lines.append("{")
    lines.append("  if (!in_mapped_memory(ea))")
    lines.append("    return 0;")
    lines.append("  if (get_func_attr(ea, FUNCATTR_START) != BADADDR)")
    lines.append("    return 1;")
    lines.append("  return add_func(ea, BADADDR);")
    lines.append("}")
    lines.append("")
    lines.append("static vec_name(idx)")
    lines.append("{")
    lines.append('  if (idx == 0x00) return "vec_reset__c_int00";')
    lines.append('  if (idx == 0x01) return "vec_int0_display";')
    lines.append('  if (idx == 0x02) return "vec_int1_tv30";')
    lines.append('  if (idx == 0x03) return "vec_int2_comm";')
    lines.append('  if (idx == 0x04) return "vec_int3_atod";')
    lines.append('  if (idx == 0x05) return "vec_xint0";')
    lines.append('  if (idx == 0x06) return "vec_rint0";')
    lines.append('  if (idx == 0x07) return "vec_xint1";')
    lines.append('  if (idx == 0x08) return "vec_rint1";')
    lines.append('  if (idx == 0x09) return "vec_tint0";')
    lines.append('  if (idx == 0x0A) return "vec_tint1";')
    lines.append('  if (idx == 0x0B) return "vec_dint";')
    lines.append('  return form("vec_%02X", idx);')
    lines.append("}")
    lines.append("")
    lines.append("static main()")
    lines.append("{")
    lines.append("  auto named, marked_data, code_words, vec_named, vec_funcs, cleared, dp_set;")
    lines.append("  auto ea, idx, vec_ea, target, vname, ptr_name, dp_reg;")
    lines.append("  named = 0;")
    lines.append("  marked_data = 0;")
    lines.append("  code_words = 0;")
    lines.append("  vec_named = 0;")
    lines.append("  vec_funcs = 0;")
    lines.append("  cleared = 0;")
    lines.append("  dp_set = 0;")
    lines.append("")
    lines.append("  // 0) Clear existing names in target section so reruns are clean.")
    lines.append("  for (ea = 0x00000000; ea <= 0x0001EFC0; ea = ea + 1)")
    lines.append("  {")
    lines.append("    if (in_mapped_memory(ea))")
    lines.append("    {")
    lines.append("      clear_name_safe(ea);")
    lines.append("      cleared = cleared + 1;")
    lines.append("    }")
    lines.append("  }")
    lines.append("")
    lines.append("  // 1) Force main mapped section to code first. (disabled)")
    lines.append("  // for (ea = 0x00000000; ea <= 0x0001EFC0; ea = ea + 1)")
    lines.append("  // {")
    lines.append("  //   if (in_mapped_memory(ea))")
    lines.append("  //   {")
    lines.append("  //     force_code_word(ea);")
    lines.append("  //     code_words = code_words + 1;")
    lines.append("  //   }")
    lines.append("  // }")
    lines.append("")
    lines.append("  // 2) Hardcode vector entries and vector targets.")
    lines.append("  for (idx = 0; idx < 0x40; idx = idx + 1)")
    lines.append("  {")
    lines.append("    vec_ea = idx;")
    lines.append("    if (!in_mapped_memory(vec_ea))")
    lines.append("      continue;")
    lines.append("    force_data_word(vec_ea);")
    lines.append("    target = bswap32(get_wide_byte(vec_ea));")
    lines.append("    vname = vec_name(idx);")
    lines.append("    set_name_safe(vec_ea, vname);")
    lines.append("    vec_named = vec_named + 1;")
    lines.append('    ptr_name = form("%s_target_%08X", vname, target);')
    lines.append("    if (in_mapped_memory(target))")
    lines.append("    {")
    lines.append("      set_name_safe(target, ptr_name);")
    lines.append("      if (maybe_make_func(target))")
    lines.append("        vec_funcs = vec_funcs + 1;")
    lines.append("    }")
    lines.append("  }")
    lines.append("  if (in_mapped_memory(0x00000000))")
    lines.append("    maybe_make_func(bswap32(get_wide_byte(0x00000000)));")
    lines.append("")

    if dp0_marks:
        lines.append("  // 3) Apply DP=0 assumptions at SETDP-derived points.")
        lines.append('  dp_reg = str2reg("dp");')
        lines.append("  if (dp_reg != -1)")
        lines.append("  {")
        for ea in sorted(dp0_marks):
            lines.append(f"    if (in_mapped_memory(0x{ea:08X})) {{")
            lines.append(f"      split_sreg_range(0x{ea:08X}, dp_reg, 0, SR_user);")
            lines.append("      dp_set = dp_set + 1;")
            lines.append("    }")
        lines.append("  }")
        lines.append("")

    if data_marks:
        lines.append("  // 4) Explicit data marks.")
        for ea in sorted(data_marks.keys()):
            lines.append(f"  if (in_mapped_memory(0x{ea:08X})) {{")
            lines.append(f"    force_data_word(0x{ea:08X});")
            dname = data_marks[ea].strip()
            if dname:
                dname = sanitize_ida_name(dname)
                lines.append(f'    set_name_safe(0x{ea:08X}, "{dname}");')
            lines.append("    marked_data = marked_data + 1;")
            lines.append("  }")
        lines.append("")

    lines.append("  // 5) Labels from spider output.")
    for ea, name, kind, comment in labels:
        lines.append(f"  if (in_mapped_memory(0x{ea:08X})) {{")
        if kind == "data":
            lines.append(f"    force_data_word(0x{ea:08X});")
            lines.append("    marked_data = marked_data + 1;")
        lines.append(f'    set_name_safe(0x{ea:08X}, "{name}");')
        if comment:
            c = comment.replace('"', "'")
            lines.append(f'    set_cmt(0x{ea:08X}, "{c}", 0);')
        lines.append("    named = named + 1;")
        lines.append("  }")
    lines.append("")
    lines.append('  msg("[crusnusa] cleared=%d code_words=%d vectors=%d vec_funcs=%d dp0=%d labels=%d data=%d\\n", cleared, code_words, vec_named, vec_funcs, dp_set, named, marked_data);')
    lines.append("}")
    lines.append("")
    return "\n".join(lines) + "\n"


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--labels-tsv", required=True, type=pathlib.Path)
    ap.add_argument("--out", required=True, type=pathlib.Path)
    ap.add_argument("--data-tsv", type=pathlib.Path, default=None)
    ap.add_argument("--dp0-tsv", type=pathlib.Path, default=None)
    args = ap.parse_args()

    rows = load_labels(args.labels_tsv)
    labels = choose_unique_names(rows)
    data_marks = load_data_marks(args.data_tsv)
    dp0_marks = load_dp0_marks(args.dp0_tsv)
    text = generate_idc(labels, data_marks, dp0_marks)
    args.out.write_text(text)
    print(f"wrote {args.out}")
    print(f"labels={len(labels)} data_marks={len(data_marks)} dp0_marks={len(dp0_marks)}")


if __name__ == "__main__":
    main()

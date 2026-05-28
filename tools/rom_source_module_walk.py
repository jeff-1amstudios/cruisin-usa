#!/usr/bin/env python3
from __future__ import annotations

import argparse
from collections import deque
import pathlib
import re
import sys
from dataclasses import dataclass
from typing import Dict, List, Optional, Sequence, Set, Tuple

# Reuse existing macro/mnemonic logic from cint_call_match.py
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import cint_call_match as ccm  # type: ignore


ROM_LINE_RE = re.compile(r"^CODE:([0-9A-Fa-f]{8})\s+(.*)$")
CMD_OBJ_RE = re.compile(r"^\s*e:([A-Za-z0-9_.$-]+)\.obj\b", re.IGNORECASE)


@dataclass
class SrcRow:
    module: str
    line_no: int
    text: str


@dataclass
class RomRow:
    addr: int
    text: str


def parse_cusa_modules(cmd_path: pathlib.Path) -> List[str]:
    mods: List[str] = []
    for raw in cmd_path.read_text(errors="ignore").splitlines():
        line = raw.split("/*", 1)[0].strip()
        m = CMD_OBJ_RE.match(line)
        if not m:
            continue
        mods.append(m.group(1).lower())
    return mods


def build_label_to_module(root: pathlib.Path) -> Dict[str, str]:
    out: Dict[str, str] = {}
    label_re = re.compile(r"^\s*([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s*:\s*$")
    bare_re = re.compile(r"^([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s*$")
    for asm in sorted(root.glob("*.ASM")):
        module = asm.stem
        for raw in asm.read_text(errors="ignore").splitlines():
            code = raw.split(";", 1)[0].rstrip()
            if not code.strip():
                continue
            m = label_re.match(code)
            if m:
                out.setdefault(m.group(1), module)
                continue
            # Col-1 bare labels are used in this codebase and can be call targets.
            if not raw[:1].isspace():
                bm = bare_re.match(code)
                if bm:
                    tok = bm.group(1)
                    if not tok.startswith("."):
                        out.setdefault(tok, module)
    return out


def discover_modules_by_flood(
    root: pathlib.Path,
    seed_labels: Sequence[str],
    macros,
    symbols,
) -> Tuple[List[str], List[str], List[Tuple[str, str]], List[str]]:
    label_to_module = build_label_to_module(root)
    q: deque[str] = deque()
    visited: Set[str] = set()
    order: List[str] = []
    unresolved_labels: List[str] = []
    discovered_edges: List[Tuple[str, str]] = []
    unresolved_call_targets: List[str] = []

    for s in seed_labels:
        m = label_to_module.get(s)
        if not m:
            unresolved_labels.append(s)
            continue
        q.append(m)

    while q:
        mod = q.popleft()
        if mod in visited:
            continue
        visited.add(mod)
        order.append(mod.lower())

        asm = find_asm_for_module(root, mod.lower())
        if asm is None:
            continue
        lines = asm.read_text(errors="ignore").splitlines()
        calls = ccm.extract_calls_from_lines(lines, macros, symbols)
        for tgt in calls:
            tm = label_to_module.get(tgt)
            if not tm:
                unresolved_call_targets.append(tgt)
                continue
            discovered_edges.append((tgt, tm))
            if tm in visited:
                continue
            q.append(tm)

    return order, unresolved_labels, discovered_edges, unresolved_call_targets


def find_asm_for_module(root: pathlib.Path, module_stem_lc: str) -> Optional[pathlib.Path]:
    candidates = list(root.glob("*.ASM"))
    for p in candidates:
        if p.stem.lower() == module_stem_lc:
            return p
    return None


def source_rows_for_module(asm_path: pathlib.Path) -> List[SrcRow]:
    out: List[SrcRow] = []
    module = asm_path.stem
    for i, raw in enumerate(asm_path.read_text(errors="ignore").splitlines(), start=1):
        # keep full line intent (labels/instructions/data/directives), skip pure comments/blank
        stripped = raw.strip()
        if not stripped:
            continue
        if stripped.startswith("*") or stripped.startswith(";"):
            continue
        # trim inline ';' comments for easier side-by-side matching
        text = raw.split(";", 1)[0].rstrip()
        if not text.strip():
            continue
        out.append(SrcRow(module=module, line_no=i, text=text))
    return out


def is_source_label_only(text: str) -> bool:
    code = text.split(";", 1)[0].strip()
    if not code:
        return False
    return bool(re.match(r"^([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s*:?\s*$", code))


def source_data_word_count(text: str) -> int:
    code = text.split(";", 1)[0].strip()
    if not code:
        return 0
    # Drop optional leading label token.
    toks = [t for t in re.split(r"[\s]+", code) if t]
    if not toks:
        return 0
    idx = 0
    if toks[0].endswith(":"):
        idx = 1
    elif not text[:1].isspace() and len(toks) >= 2 and re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", toks[0]):
        idx = 1
    if idx >= len(toks):
        return 0
    op = toks[idx].lower()
    rest = " ".join(toks[idx + 1 :]).strip()
    if op == ".word":
        if not rest:
            return 1
        return max(1, len([x for x in rest.split(",") if x.strip()]))
    if op in {".float", ".double", ".string", ".set", ".data", ".bss", "fbss", "pbss", "lobss", "hibss"}:
        return 1
    return 0


def source_rows_from_label(src_rows: Sequence[SrcRow], label: str) -> List[SrcRow]:
    label_lc = label.lower()
    for i, r in enumerate(src_rows):
        code = r.text.split(";", 1)[0].strip()
        if not code:
            continue
        m = re.match(r"^([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s*:?\s*$", code)
        if m and m.group(1).lower() == label_lc:
            return list(src_rows[i + 1 :])
    return list(src_rows)


def source_label_index(src_rows: Sequence[SrcRow], label: str) -> int:
    label_lc = label.lower()
    for i, r in enumerate(src_rows):
        code = r.text.split(";", 1)[0].strip()
        if not code:
            continue
        m = re.match(r"^([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s*:?\s*$", code)
        if m and m.group(1).lower() == label_lc:
            return i
    return -1


def parse_rom_rows(rom_path: pathlib.Path) -> List[RomRow]:
    out: List[RomRow] = []
    for raw in rom_path.read_text(errors="ignore").splitlines():
        m = ROM_LINE_RE.match(raw)
        if not m:
            continue
        txt = m.group(2).rstrip()
        # Ignore IDA comment-only expansion rows at same address.
        if not txt.strip():
            continue
        if txt.lstrip().startswith(";"):
            continue
        out.append(RomRow(addr=int(m.group(1), 16), text=txt))
    return out


def build_rom_label_addr_map(rom_path: pathlib.Path) -> Dict[str, int]:
    out: Dict[str, int] = {}
    lab_re = re.compile(r"^CODE:([0-9A-Fa-f]{8})\s+([A-Za-z_.$?][A-Za-z0-9_.$?]*):")
    for raw in rom_path.read_text(errors="ignore").splitlines():
        m = lab_re.match(raw)
        if not m:
            continue
        out.setdefault(m.group(2), int(m.group(1), 16))
    return out


def rom_rows_from_addr(rom_rows: Sequence[RomRow], start_addr: Optional[int]) -> List[RomRow]:
    if start_addr is None:
        return list(rom_rows)
    for i, r in enumerate(rom_rows):
        if r.addr >= start_addr:
            return list(rom_rows[i:])
    return []


def first_n_modules(mods: Sequence[str], n: int) -> List[str]:
    if n <= 0:
        return []
    return list(mods[: min(n, len(mods))])


def emit_side_by_side(
    rom_rows: Sequence[RomRow],
    src_rows: Sequence[SrcRow],
    out_path: Optional[pathlib.Path],
    limit: Optional[int],
) -> None:
    n = min(len(rom_rows), len(src_rows))
    if limit is not None:
        n = min(n, limit)

    header = (
        "idx\trom_addr\trom_text\tmodule\tsrc_line\tsrc_text\n"
    )

    lines: List[str] = [header]
    for i in range(n):
        rr = rom_rows[i]
        sr = src_rows[i]
        lines.append(
            f"{i}\t{rr.addr:08X}\t{rr.text}\t{sr.module}\t{sr.line_no}\t{sr.text}\n"
        )

    body = "".join(lines)
    if out_path:
        out_path.write_text(body)
    else:
        print(body, end="")


def extract_mnemonic_from_source(text: str) -> Optional[str]:
    has_leading_ws = text[:1].isspace()
    s = text.strip()
    if not s:
        return None
    if s.startswith("||"):
        s = s[2:].strip()
        has_leading_ws = True
        if not s:
            return None
    # Drop optional leading label token ("LABEL:" or "LABEL OP ...")
    toks = [t for t in re.split(r"[\s,]+", s) if t]
    if not toks:
        return None
    if toks[0].endswith(":"):
        toks = toks[1:]
    elif not has_leading_ws and len(toks) >= 2 and re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", toks[0]):
        # "LABEL OP ..." style.
        toks = toks[1:]
    if not toks:
        return None
    m = toks[0].lower()
    # Non-emitting/source-only directives: skip for strict byte-level stepping.
    if m in {
        ".file",
        ".include",
        ".globl",
        ".def",
        ".ref",
        ".macro",
        ".endm",
        ".if",
        ".endif",
        ".asg",
        "lobss",
        "fbss",
        "pbss",
        "hibss",
        ".bss",
        ".word",
        ".float",
        ".double",
        ".string",
        ".set",
        ".equ",
        "equ",
        ".text",
        ".data",
        "romdata",
    }:
        return None
    return m


def extract_mnemonic_from_rom(text: str) -> Optional[str]:
    s = text.strip()
    if not s:
        return None
    # Drop leading label before colon if present.
    if ":" in s:
        left, right = s.split(":", 1)
        if re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", left.strip()):
            s = right.strip()
    if not s or s.startswith(";"):
        return None
    toks = [t for t in re.split(r"[\s,]+", s) if t]
    if not toks:
        return None
    m = toks[0].lower()
    if m.startswith("||"):
        m = m[2:]
        if not m:
            return None
    # IDA listing scaffolding directives can be skipped.
    if m in {
        ".text",
        ".data",
        ".bss",
        ".word",
        ".float",
        ".double",
        ".string",
        ".set",
    }:
        return None
    return m


def is_rom_data_directive(text: str) -> bool:
    s = text.strip()
    if not s:
        return False
    if ":" in s:
        left, right = s.split(":", 1)
        if re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", left.strip()):
            s = right.strip()
    if not s:
        return False
    tok = s.split()[0].lower()
    return tok in {".word", ".float", ".double", ".string", ".set", ".data", ".bss", ".text"}


def mnemonic_equivalent(a: str, b: str) -> bool:
    # Delegate to canonical equivalence logic already used elsewhere.
    return ccm.mnemonics_equivalent([a], [b])


def strict_compare_or_die(
    rom_rows: Sequence[RomRow],
    src_rows: Sequence[SrcRow],
    limit: Optional[int],
    macros,
    symbols,
) -> None:
    # Apply full-stream conditional filtering first (e.g. .if/.endif STATISTICS blocks).
    src_texts = [r.text for r in src_rows]
    active_texts = ccm.iter_active_lines(src_texts, symbols)
    active_src_rows: List[SrcRow] = []
    p = 0
    for r in src_rows:
        if p >= len(active_texts):
            break
        if r.text == active_texts[p]:
            active_src_rows.append(r)
            p += 1

    i = 0  # rom index
    j = 0  # source index
    matched = 0
    max_match = limit if limit is not None else (10**9)
    src_pending: List[str] = []
    src_pending_row: Optional[SrcRow] = None
    src_data_pending_words = 0
    data_code_warnings = 0

    def mnemonics_for_src_row(sr: SrcRow) -> List[str]:
        if sr.text.lstrip().startswith("||"):
            sm = extract_mnemonic_from_source(sr.text)
            return [sm] if sm else []
        return ccm.extract_mnemonics_from_lines(
            [sr.text],
            macros,
            symbols,
            expand_macros=True,
        )

    while i < len(rom_rows) and j < len(active_src_rows) and matched < max_match:
        rr = rom_rows[i]
        rm = extract_mnemonic_from_rom(rr.text)

        if rm is None:
            # Tolerated case: ROM listing marks bytes as data/directive while
            # source stream expects code. Consume one source mnemonic and
            # continue; when ROM code resumes, strict validation resumes.
            if is_rom_data_directive(rr.text):
                consumed_src_mn = False
                if src_data_pending_words > 0:
                    src_data_pending_words -= 1
                    matched += 1
                    i += 1
                    continue
                # Consume source stream 1:1 for data rows when possible.
                # This prevents skipping multiple source data rows for a single ROM .word.
                if not src_pending and j < len(active_src_rows):
                    while j < len(active_src_rows):
                        sr0 = active_src_rows[j]
                        s0 = mnemonics_for_src_row(sr0)
                        if s0:
                            src_pending = s0
                            src_pending_row = sr0
                            j += 1
                            break
                        # Skip label-only rows in-place; they do not map to bytes.
                        if is_source_label_only(sr0.text):
                            j += 1
                            continue
                        # Non-emitting data row: consume as many ROM data words
                        # as the row likely emits.
                        wc = source_data_word_count(sr0.text)
                        if wc > 0:
                            src_data_pending_words = wc - 1
                            j += 1
                            matched += 1
                            break
                        # Non-emitting non-label row (data/directive): consume 1:1.
                        j += 1
                        matched += 1
                        break

                # If source is truly expecting code at this point, treat as a hole warning.
                if src_pending:
                    sm = src_pending.pop(0)
                    sr_for_warn = src_pending_row if src_pending_row is not None else active_src_rows[min(j, len(active_src_rows) - 1)]
                    data_code_warnings += 1
                    consumed_src_mn = True
                    print(
                        "warning: rom-data/source-code hole at compare_index "
                        f"{matched}: rom={rr.addr:08X} src_mn={sm} "
                        f"module={sr_for_warn.module} src_line={sr_for_warn.line_no}\n"
                        f"  ROM: {rr.text}\n"
                        f"  SRC: {sr_for_warn.text}"
                    )
                if consumed_src_mn:
                    matched += 1
            i += 1
            continue

        # Ensure we have source mnemonics queued for comparison. A single source
        # line can expand to multiple mnemonics (e.g. LDIL macro), and strict
        # compare must consume all of them before advancing to next source line.
        while not src_pending and j < len(active_src_rows):
            sr = active_src_rows[j]
            src_pending = mnemonics_for_src_row(sr)
            src_pending_row = sr
            j += 1

        if not src_pending:
            break

        sm = src_pending[0]
        sr_for_err = src_pending_row if src_pending_row is not None else active_src_rows[min(j, len(active_src_rows) - 1)]

        if not mnemonic_equivalent(sm, rm):
            raise SystemExit(
                "mnemonic mismatch at compare_index "
                f"{matched}: rom={rr.addr:08X} rom_mn={rm} src_mn={sm} "
                f"module={sr_for_err.module} src_line={sr_for_err.line_no}\n"
                f"  ROM: {rr.text}\n"
                f"  SRC: {sr_for_err.text}"
            )

        matched += 1
        i += 1
        src_pending.pop(0)
    print(
        "strict_compare_done "
        f"compares={matched} rom_data_source_code_warnings={data_code_warnings}"
    )


def strict_compare_bidir_or_die(
    rom_rows: Sequence[RomRow],
    src_rows: Sequence[SrcRow],
    anchor_idx: int,
    limit: Optional[int],
    macros,
    symbols,
) -> None:
    if anchor_idx < 0 or anchor_idx >= len(src_rows):
        raise SystemExit("Invalid source anchor index for bidirectional strict compare")

    # Downward pass: rows after anchor to end.
    down_rows = list(src_rows[anchor_idx + 1 :])
    print(f"bidir_pass=down source_rows={len(down_rows)}")
    strict_compare_or_die(rom_rows, down_rows, limit, macros, symbols)
    print("bidir_down=OK")

    # Upward pass: reset pointers and walk upward by reversing rows above anchor.
    up_rows = list(reversed(src_rows[:anchor_idx]))
    print(f"bidir_pass=up source_rows={len(up_rows)}")
    strict_compare_or_die(rom_rows, up_rows, limit, macros, symbols)
    print("bidir_up=OK")


def main() -> None:
    ap = argparse.ArgumentParser(
        description=(
            "Walk ROM listing and source modules linearly in CUSA.CMD order. "
            "Designed for linear full-build alignment experiments."
        )
    )
    ap.add_argument("--root", default=".", help="Project root (default: current dir)")
    ap.add_argument("--cmd", default="CUSA.CMD", help="Path to linker cmd file, relative to root")
    ap.add_argument(
        "--rom",
        default="roms/rom.lst",
        help="Path to ROM asm listing with CODE:XXXXXXXX lines, relative to root",
    )
    ap.add_argument(
        "--limit",
        type=int,
        default=1000,
        help="Max paired rows / strict mnemonic compares (default: 1000)",
    )
    ap.add_argument(
        "--out",
        default=None,
        help="Optional output TSV path; prints to stdout when omitted",
    )
    ap.add_argument(
        "--strict-match",
        action="store_true",
        help="Step row-by-row and exit on first mnemonic mismatch",
    )
    ap.add_argument(
        "--flood-seed",
        default="_c_int00",
        help="Comma-separated source labels to seed module flood-fill order (default: _c_int00)",
    )
    ap.add_argument(
        "--flood-modules",
        action="store_true",
        help="Use cross-module CALL-target flood-fill order instead of raw CUSA.CMD order",
    )
    ap.add_argument(
        "--only-modules",
        default=None,
        help="Optional comma-separated module names to restrict source collection (e.g. CUSA,DIRQ)",
    )
    ap.add_argument(
        "--discovered-out",
        default=None,
        help="Optional path to write discovered symbol->module pairs from flood mode",
    )
    ap.add_argument(
        "--source-start-label",
        default=None,
        help="Optional source label anchor; when set, source comparison starts after this label",
    )
    ap.add_argument(
        "--rom-start-addr",
        default=None,
        help="Optional ROM start address (hex like 0x4AE6 or decimal) for comparison anchor",
    )
    ap.add_argument(
        "--source-bidir",
        action="store_true",
        help="When used with --source-start-label, walk source rows downward then upward from the label",
    )
    args = ap.parse_args()

    root = pathlib.Path(args.root).resolve()
    cmd_path = root / args.cmd
    rom_path = root / args.rom
    out_path = pathlib.Path(args.out).resolve() if args.out else None

    if not cmd_path.exists():
        raise SystemExit(f"Missing cmd file: {cmd_path}")
    if not rom_path.exists():
        raise SystemExit(f"Missing ROM listing: {rom_path}")
    macros = ccm.parse_macros(root)
    symbols = ccm.parse_set_symbols(root)
    symbols["MEMTESTS"] = symbols.get("MEMTESTS", 1)
    symbols["STATISTICS"] = symbols.get("STATISTICS", 0)

    if args.flood_modules:
        seed_labels = [s.strip() for s in args.flood_seed.split(",") if s.strip()]
        use_mods, unresolved_seeds, discovered_edges, unresolved_calls = discover_modules_by_flood(
            root, seed_labels, macros, symbols
        )
        if unresolved_seeds:
            print("warning: unresolved flood seed labels:", ", ".join(unresolved_seeds))
        if args.discovered_out:
            dpath = pathlib.Path(args.discovered_out).resolve()
            addr_map = build_rom_label_addr_map(rom_path)
            lines = ["label\tmodule\taddress\n"]
            seen: Set[Tuple[str, str]] = set()
            for sym, mod in discovered_edges:
                k = (sym, mod)
                if k in seen:
                    continue
                seen.add(k)
                a = addr_map.get(sym)
                a_txt = f"0x{a:08X}" if a is not None else ""
                lines.append(f"{sym}\t{mod}\t{a_txt}\n")
            if unresolved_calls:
                lines.append("\n# unresolved_call_targets\n")
                for t in sorted(set(unresolved_calls)):
                    lines.append(f"{t}\n")
            dpath.write_text("".join(lines))
    else:
        use_mods = parse_cusa_modules(cmd_path)
    if not use_mods:
        raise SystemExit("No modules found for selected mode")
    if args.only_modules:
        allow = {m.strip().lower() for m in args.only_modules.split(",") if m.strip()}
        use_mods = [m for m in use_mods if m.lower() in allow]
        if not use_mods:
            raise SystemExit("No modules remain after --only-modules filter")

    src_rows: List[SrcRow] = []
    missing: List[str] = []
    for m in use_mods:
        asm = find_asm_for_module(root, m)
        if asm is None:
            missing.append(m)
            continue
        src_rows.extend(source_rows_for_module(asm))

    if missing:
        print("warning: missing ASM for modules:", ", ".join(missing))
    if not src_rows:
        raise SystemExit("No source rows collected for selected modules")

    rom_rows = parse_rom_rows(rom_path)
    if not rom_rows:
        raise SystemExit("No CODE:XXXXXXXX rows parsed from ROM listing")

    start_addr: Optional[int] = None
    if args.rom_start_addr:
        start_addr = int(args.rom_start_addr, 0)
    rom_rows = rom_rows_from_addr(rom_rows, start_addr)
    anchor_idx = -1
    if args.source_start_label:
        anchor_idx = source_label_index(src_rows, args.source_start_label)
        if args.source_bidir:
            if anchor_idx < 0:
                raise SystemExit(f"Source start label not found: {args.source_start_label}")
        else:
            src_rows = source_rows_from_label(src_rows, args.source_start_label)

    print(
        f"selected_modules={len(use_mods)} "
        f"source_rows={len(src_rows)} rom_rows={len(rom_rows)} "
        f"first_modules={','.join(use_mods[:8])}"
    )
    if args.strict_match:
        if args.source_bidir and args.source_start_label:
            strict_compare_bidir_or_die(rom_rows, src_rows, anchor_idx, args.limit, macros, symbols)
        else:
            strict_compare_or_die(rom_rows, src_rows, args.limit, macros, symbols)
        print("strict_match=OK")
    emit_side_by_side(rom_rows, src_rows, out_path, args.limit)


if __name__ == "__main__":
    main()

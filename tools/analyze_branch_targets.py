#!/usr/bin/env python3
import argparse
import csv
import glob
import os
import re
from collections import defaultdict

RET_TOKENS = {"rets", "retsu", "retp", "b", "bu", "br"}

OPCODES = {
    "addi","addf","addf3","and","andn","b","be","bne","bnz","bz","bu","bud","br",
    "dbu","dbud","call","calleq","callge","clri","clrf","cmpi","cmpf","create","createc",
    "dec","die","dint","fix","float","fifo_clrp","dma_wt","jsrp","lcall","ldf","ldi","ldiu",
    "ldil","ldl","ldin","ldp","ls","lsh","mpyf","mpyf3","neg","negf","nop","not","or",
    "orm","pop","popf","push","readaud","retp","rets","retsu","rptb","setaud","setdp",
    "slockon","sondfx","sti","stf","stpi","subi","subf","tstb","xor","ifi","ldieq","ldine",
    "ldilt","ldigt","ldinz","ldiz","bge","bgt","blt","ble","randn","rand","randi"
}

CALL_RE = re.compile(r"^\s*CALL\s+([A-Za-z_][A-Za-z0-9_.$?]*)\b", re.IGNORECASE)
B_RE = re.compile(r"^\s*B\s+([A-Za-z_][A-Za-z0-9_.$?]*)\b", re.IGNORECASE)
LABEL_COLON_RE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_.$?]*)\s*:\s*(.*)$")
LABEL_BARE_ONLY_RE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_.$?]*)\s*$")
LABEL_BARE_WITH_CODE_RE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_.$?]*)\s+(.+)$")
GLOBL_RE = re.compile(r"^\s*\.globl\s+(.+)$", re.IGNORECASE)


def clean_line(raw: str) -> str:
    return raw.split(";", 1)[0].rstrip("\n")


def load_asm_files(root: str):
    files = glob.glob(os.path.join(root, "**", "*.ASM"), recursive=True)
    out = []
    for p in files:
        if "/port-sdl/" in p or "/docs/" in p or "/tools/" in p:
            continue
        out.append(p)
    return sorted(out)


def _extract_globls_from_file(path: str):
    out = set()
    for raw in open(path, errors="ignore"):
        line = clean_line(raw)
        s = line.strip()
        if not s or s.startswith("*") or s.startswith(";"):
            continue
        m = GLOBL_RE.match(line)
        if not m:
            continue
        rhs = m.group(1).split(";", 1)[0]
        for tok in rhs.split(","):
            sym = tok.strip()
            if not sym:
                continue
            # Ignore macro placeholders like :VARIABLE:I
            if ":" in sym:
                continue
            if re.match(r"^[A-Za-z_][A-Za-z0-9_.$?]*$", sym):
                out.add(sym)
    return out


def load_equ_globls(root: str):
    out = set()
    files = sorted(glob.glob(os.path.join(root, "*.EQU")))
    for p in files:
        out |= _extract_globls_from_file(p)
    return out


def load_asm_globls(files):
    out = set()
    for p in files:
        out |= _extract_globls_from_file(p)
    return out


def previous_instruction(lines, i):
    j = i - 1
    while j >= 0:
        s = lines[j].strip()
        if not s or s.startswith("*") or s.startswith(";"):
            j -= 1
            continue
        # Skip directives (e.g. .globl) and pure label lines while walking back.
        if s.startswith("."):
            j -= 1
            continue
        m = re.match(r"^([A-Za-z_][A-Za-z0-9_.$?]*)\s*:?\s*$", s)
        if m:
            tok = m.group(1).lower()
            # Skip pure label lines, but do not skip opcode-only lines like RETS.
            if tok not in OPCODES:
                j -= 1
                continue
        return s
    return ""


def parse(files, include_call=True, include_b=True):
    targets = defaultdict(list)  # symbol -> list[(file,line,kind)]
    defs = defaultdict(list)     # symbol -> list[(file,line,label_kind,prev_line)]

    for f in files:
        raw_lines = open(f, errors="ignore").readlines()
        lines = [clean_line(r) for r in raw_lines]

        for i, line in enumerate(lines):
            s = line.strip()
            if not s or s.startswith("*"):
                continue

            if include_call:
                m = CALL_RE.match(line)
                if m:
                    targets[m.group(1)].append((f, i + 1, "CALL"))

            if include_b:
                m = B_RE.match(line)
                if m:
                    targets[m.group(1)].append((f, i + 1, "B"))

            name = None
            label_kind = None
            m = LABEL_COLON_RE.match(line)
            if m:
                name = m.group(1)
                label_kind = "colon"
            else:
                m = LABEL_BARE_ONLY_RE.match(line)
                if m and m.group(1).lower() not in OPCODES:
                    name = m.group(1)
                    label_kind = "bare_only"
                else:
                    m = LABEL_BARE_WITH_CODE_RE.match(line)
                    if m:
                        tok = m.group(1)
                        if tok.lower() not in OPCODES and not tok.startswith("."):
                            name = tok
                            label_kind = "bare_with_code"

            if name:
                prev = previous_instruction(lines, i)
                defs[name].append((f, i + 1, label_kind, prev))

    return targets, defs


def filter_within_function(targets, defs, equ_globls=None):
    """
    Keep targets where at least one definition site is NOT immediately preceded by RETS/RETSU/RETP.
    """
    rows = []
    equ_globls = equ_globls or set()
    for sym in sorted(targets):
        # User rule: if exported in .EQU via .globl, treat as function symbol.
        if sym in equ_globls:
            continue
        if sym not in defs:
            continue

        good_defs = []
        for f, ln, label_kind, prev in defs[sym]:
            # If there's no previous instruction, treat as a function/file entry boundary.
            if not prev:
                continue
            ptok = prev.split()[0].lower() if prev else ""
            if ptok in RET_TOKENS:
                continue
            good_defs.append((f, ln, label_kind, prev))

        if not good_defs:
            continue

        use_sites = targets[sym]
        for uf, uln, ukind in use_sites:
            for df, dln, dkind, prev in good_defs:
                rows.append({
                    "symbol": sym,
                    "use_kind": ukind,
                    "use_file": uf,
                    "use_line": uln,
                    "def_file": df,
                    "def_line": dln,
                    "def_label_kind": dkind,
                    "def_prev": prev,
                })
    return rows


def main():
    ap = argparse.ArgumentParser(description="Analyze CALL/B targets that resolve to labels within functions.")
    ap.add_argument("--root", default="/Users/j.harris/code/cruisin-usa", help="Repo root")
    ap.add_argument("--calls", action="store_true", help="Include CALL targets")
    ap.add_argument("--branches", action="store_true", help="Include unconditional B targets")
    ap.add_argument("--csv", help="Write detailed CSV to this path")
    ap.add_argument("--symbols-only", action="store_true", help="Print unique symbols only")
    args = ap.parse_args()

    include_call = args.calls or (not args.calls and not args.branches)
    include_b = args.branches

    files = load_asm_files(args.root)
    equ_globls = load_equ_globls(args.root)
    asm_globls = load_asm_globls(files)
    targets, defs = parse(files, include_call=include_call, include_b=include_b)
    rows = filter_within_function(targets, defs, equ_globls=(equ_globls | asm_globls))

    symbols = sorted({r["symbol"] for r in rows})
    print(f"files_scanned={len(files)}")
    print(f"symbols={len(symbols)}")

    if args.symbols_only:
        for s in symbols:
            print(s)
    else:
        for s in symbols:
            print(s)

    if args.csv:
        with open(args.csv, "w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=[
                "symbol", "use_kind", "use_file", "use_line",
                "def_file", "def_line", "def_label_kind", "def_prev"
            ])
            w.writeheader()
            for r in rows:
                w.writerow(r)


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
from __future__ import annotations
import argparse, pathlib, re, sys, traceback
from collections import deque
from typing import Dict, List, Tuple, Optional, Set

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import cint_call_match as ccm  # type: ignore

ROM_LINE_RE = re.compile(r"^CODE:([0-9A-Fa-f]{8})\s+(.*)$")
ROM_CALL_RE = re.compile(r"\bcall[a-z]*\s+([^\s;]+)", re.IGNORECASE)
LABEL_RE = re.compile(r"^([A-Za-z_.$?][A-Za-z0-9_.$?]*):")
SRC_LABEL_COLON_RE = re.compile(r"^\s*([A-Za-z_.$?][A-Za-z0-9_.$?]*):")
SKIP = {'.text','.data','.bss','.word','.float','.double','.string','.set'}
LABEL_TOKEN_RE = re.compile(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$")
NUMERIC_TOKEN_RE = re.compile(r"^(?:-?\d+|0x[0-9A-Fa-f]+|[0-9A-Fa-f]+h)$")
DATA_DECL_OPS = {".word", ".byte", ".float", ".double", ".string", ".bss"}
RomOp = Tuple[str, Optional[str], int, bool, List[str]]


def parse_globals_equ(root: pathlib.Path) -> Set[str]:
    out: Set[str] = set()
    p = root / "GLOBALS.EQU"
    if not p.exists():
        return out
    for ln in p.read_text(errors="ignore").splitlines():
        code = ccm.strip_comment(ln).strip()
        if not code:
            continue
        m = re.match(r"^\s*\.globl\s+(.+)$", code, re.IGNORECASE)
        if not m:
            continue
        rest = m.group(1)
        for tok in [t.strip() for t in rest.split(",")]:
            if tok and LABEL_TOKEN_RE.match(tok):
                out.add(tok.upper())
    return out


def parse_word_symbol_defs(root: pathlib.Path) -> List[Tuple[str, str, str]]:
    out: List[Tuple[str, str, str]] = []
    for p in sorted(root.glob("*.ASM")):
        module = p.stem
        for ln in p.read_text(errors="ignore").splitlines():
            code = ccm.strip_comment(ln)
            lbl, _ = ccm.split_optional_label(code)
            if not lbl:
                continue
            op_idx, toks = ccm.split_label_and_tokens(code)
            if not toks or op_idx >= len(toks):
                continue
            if toks[op_idx].lower() != ".word" or len(toks) < op_idx + 2:
                continue
            rhs = toks[op_idx + 1].strip().strip(",")
            if LABEL_TOKEN_RE.match(rhs):
                out.append((lbl, rhs, module))
    return out


def build_rom_maps(rom_path: pathlib.Path):
    l2a: Dict[str,int] = {}
    wvals: Dict[int,int] = {}
    lines = rom_path.read_text(errors='ignore').splitlines()
    for ln in lines:
        m = ROM_LINE_RE.match(ln)
        if not m:
            continue
        a = int(m.group(1),16)
        rhs = m.group(2).strip()
        lm = LABEL_RE.match(rhs)
        if lm and lm.group(1) not in l2a:
            l2a[lm.group(1)] = a
        if ".word" in rhs.lower():
            rm = re.search(r"\.word\s+([^\s;]+)", rhs, re.IGNORECASE)
            if rm:
                v = parse_rom_number(rm.group(1))
                if v is not None:
                    wvals[a] = v
    return lines, l2a, wvals


def source_first5(root: pathlib.Path, fn: str, macros, symbols) -> List[str]:
    mp, si = ccm.find_source_module_for_function(root, fn)
    sl = ccm.extract_source_function_lines(mp, fn, si)
    return ccm.extract_mnemonics_from_lines(sl, macros, symbols, limit=5, expand_macros=True)


def rom_first5(lines: List[str], start_addr: int, need=5) -> List[str]:
    out=[]
    start_idx=None
    for i,ln in enumerate(lines):
        m=ROM_LINE_RE.match(ln)
        if m and int(m.group(1),16)>=start_addr:
            start_idx=i; break
    if start_idx is None:
        return out
    i=start_idx
    while i<len(lines) and len(out)<need:
        m=ROM_LINE_RE.match(lines[i]); i+=1
        if not m:
            continue
        rhs=m.group(2).strip()
        if ':' in rhs:
            l,r=rhs.split(':',1)
            if re.match(r'^[A-Za-z_.$?][A-Za-z0-9_.$?]*$', l.strip()):
                rhs=r.strip()
        if not rhs or rhs.startswith(';'):
            continue
        toks=[t for t in re.split(r'[\s,]+', rhs) if t]
        if not toks:
            continue
        mn=toks[0].lower()
        if mn.startswith('||'):
            mn=mn[2:]
        if mn in SKIP:
            continue
        out.append(mn)
    return out


def source_module_tail_lines(root: pathlib.Path, fn: str) -> Tuple[List[str], int]:
    mp, si = ccm.find_source_module_for_function(root, fn)
    lines = mp.read_text(errors='ignore').splitlines()
    out: List[str] = []
    base = si + 2
    m = re.match(r"^\s*[A-Za-z_.$?][A-Za-z0-9_.$?]*:\s+(.+)$", ccm.strip_comment(lines[si]))
    if m and m.group(1).strip():
        out.append("\t" + m.group(1).strip())
        base = si + 1
    out.extend(lines[si + 1 :])
    return out, base


def source_module_head_lines(root: pathlib.Path, fn: str) -> Tuple[List[str], int]:
    mp, si = ccm.find_source_module_for_function(root, fn)
    lines = mp.read_text(errors='ignore').splitlines()
    return list(lines[:si]), 1


def source_ops_stream(lines_in: List[str], macros, symbols, base_line_no: int = 1, recursion_limit: int = 10) -> List[Tuple[str, Optional[str], int, str, bool]]:
    out: List[Tuple[str, Optional[str], int, str, bool]] = []
    expander = ccm.MacroExpander(macros, symbols)
    active_lines = ccm.iter_active_lines(lines_in, symbols)

    def walk_line(code: str, depth: int, active: set, src_line_no: int, src_raw: str) -> None:
        if depth > recursion_limit:
            return
        code = ccm.strip_comment(code)
        if not code.strip():
            return
        if code.lstrip().startswith("||"):
            code = re.sub(r"^\s*\|\|\s*", "", code)
            if not code.strip():
                return
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks:
            return
        if len(toks) == 1 and re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", toks[0]) and not code[:1].isspace():
            return
        op = toks[op_idx].upper()
        if op_idx == 1 and len(toks) >= 3 and re.match(r"^[A-Z_.$?][A-Z0-9_.$?]*$", toks[0], re.IGNORECASE) and not toks[0].endswith(":"):
            op = toks[1].upper()
        if op.endswith(":"):
            return
        if op in macros:
            if op in active:
                return
            active.add(op)
            args = expander.parse_invocation_args(op_idx, toks)
            for b in ccm.iter_active_lines(expander.expand(op, args), symbols):
                walk_line(b, depth + 1, active, src_line_no, src_raw)
            active.remove(op)
            return
        if op.startswith(".") or op == "EQU":
            # Keep data/directive rows in the stream for unified stepping.
            out.append((op.lower(), None, src_line_no, src_raw.rstrip(), False))
            return
        norm = {"CLRI":"subi","STPI":"sti","RETS":"retsu","RET":"retsu","RETI":"retiu"}.get(op, op.lower())
        tgt = None
        if len(toks) >= (op_idx + 2):
            cand = toks[op_idx + 1]
            # Capture transfer targets: CALL*, B*, DB* (decrement-and-branch variants).
            if ccm.CALL_MNEMONIC_RE.match(op) or op.startswith("B") or op.startswith("DB"):
                if LABEL_TOKEN_RE.match(cand):
                    tgt = cand
        out.append((norm, tgt, src_line_no, src_raw.rstrip(), True))

    for idx, ln in enumerate(active_lines):
        walk_line(ln, 0, set(), base_line_no + idx, ln)
    return out


def rom_index_at_or_after_addr(lines: List[str], start_addr: int) -> int:
    for i, ln in enumerate(lines):
        m = ROM_LINE_RE.match(ln)
        if m and int(m.group(1), 16) >= start_addr:
            return i
    return len(lines)


def rom_function_end_index(lines: List[str], start_idx: int) -> int:
    i = start_idx
    while i < len(lines):
        if i > start_idx and '; End of function ' in lines[i]:
            return i
        i += 1
    return len(lines)


def parse_rom_number(tok: str) -> Optional[int]:
    t = tok.strip().strip(",")
    if not t:
        return None
    t = t.lstrip("#@")
    t = t.strip("()")
    if not t:
        return None
    if t.lower().endswith("h"):
        try:
            return int(t[:-1], 16)
        except ValueError:
            return None
    if t.lower().startswith("0x"):
        try:
            return int(t, 16)
        except ValueError:
            return None
    if re.match(r"^-?\d+$", t):
        return int(t, 10)
    return None


def rom_ops_stream(lines: List[str], start_addr: int, reverse: bool = False) -> List[RomOp]:
    out: List[RomOp] = []
    if reverse:
        i = rom_index_at_or_after_addr(lines, start_addr) - 1
        while i >= 0:
            ln = lines[i]
            if '; End of function ' in ln and out:
                break
            m = ROM_LINE_RE.match(ln); i -= 1
            if not m:
                continue
            rhs = m.group(2).strip()
            if ':' in rhs:
                l,r = rhs.split(':',1)
                if re.match(r'^[A-Za-z_.$?][A-Za-z0-9_.$?]*$', l.strip()):
                    rhs = r.strip()
            if not rhs or rhs.startswith(';'):
                continue
            toks=[t for t in re.split(r'[\s,]+', rhs) if t]
            if not toks:
                continue
            mn=toks[0].lower()
            if mn.startswith('||'):
                mn=mn[2:]
            is_code = mn not in SKIP
            tgt = None
            if is_code and len(toks) >= 2 and (mn.startswith("call") or mn.startswith("b") or mn.startswith("db")):
                cand = toks[1]
                if LABEL_TOKEN_RE.match(cand):
                    tgt = cand
            out.append((mn, tgt, int(m.group(1),16), is_code, toks))
        return out

    i = rom_index_at_or_after_addr(lines, start_addr)
    end = rom_function_end_index(lines, i)
    while i < end:
        m = ROM_LINE_RE.match(lines[i]); i += 1
        if not m:
            continue
        rhs = m.group(2).strip()
        if ':' in rhs:
            l,r = rhs.split(':',1)
            if re.match(r'^[A-Za-z_.$?][A-Za-z0-9_.$?]*$', l.strip()):
                rhs = r.strip()
        if not rhs or rhs.startswith(';'):
            continue
        toks=[t for t in re.split(r'[\s,]+', rhs) if t]
        if not toks:
            continue
        mn=toks[0].lower()
        if mn.startswith('||'):
            mn=mn[2:]
        is_code = mn not in SKIP
        tgt = None
        if is_code and len(toks) >= 2 and (mn.startswith("call") or mn.startswith("b") or mn.startswith("db")):
            cand = toks[1]
            if LABEL_TOKEN_RE.match(cand):
                tgt = cand
        out.append((mn, tgt, int(m.group(1),16), is_code, toks))
    return out


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--root', default='.')
    ap.add_argument('--rom', default='roms/rom.lst')
    ap.add_argument('--seed-label', default='_c_int00')
    ap.add_argument('--seed-addr', default='0x00004AE6')
    ap.add_argument('--out', default='/private/tmp/spider_addr.tsv')
    ap.add_argument('--stops-out', default='/private/tmp/spider_stops.tsv')
    ap.add_argument('--conflicts-out', default='/private/tmp/spider_conflicts.tsv')
    ap.add_argument('--dp0-out', default='/private/tmp/spider_dp0.tsv')
    args=ap.parse_args()

    root=pathlib.Path(args.root).resolve()
    lines,l2a,wvals=build_rom_maps((root/args.rom).resolve())
    macros=ccm.parse_macros(root)
    symbols=ccm.parse_set_symbols(root)
    symbols['MEMTESTS']=symbols.get('MEMTESTS',1)
    symbols['STATISTICS']=symbols.get('STATISTICS',0)
    global_symbols = parse_globals_equ(root)
    word_symbol_defs = parse_word_symbol_defs(root)

    q=deque([(args.seed_label, int(args.seed_addr,0))])
    seen=set(); rows=[]; stops=[]
    discovered_labels: Dict[Tuple[str, str], int] = {}
    conflicts=[]
    dp0_eas: Set[int] = set()
    label_addr_first: Dict[Tuple[str, str], int] = {}
    map_ok_cache: Dict[Tuple[str, int], bool] = {}
    qlabel_counts: Dict[str, int] = {}
    norm_map: Dict[str, str] = {}
    module_labels_cache: Dict[pathlib.Path, set[str]] = {}
    operand_ref_labels: Set[Tuple[str, str]] = set()
    code_ref_labels: Set[Tuple[str, str]] = set()
    symbol_modules: Dict[str, Set[str]] = {}

    for p in sorted(root.glob("*.ASM")):
        mod = p.stem
        for ln in p.read_text(errors="ignore").splitlines():
            c = ccm.strip_comment(ln)
            l, _ = ccm.split_optional_label(c)
            if l and LABEL_TOKEN_RE.match(l):
                symbol_modules.setdefault(l.upper(), set()).add(mod)

    def preferred_name(lbl: str, mod: str) -> str:
        if lbl.upper() in global_symbols or not mod:
            return lbl
        return f"{mod}_{lbl}"

    def norm_label(lbl: str, mod: str) -> str:
        base_name = preferred_name(lbl, mod)
        if not base_name.endswith('?'):
            return base_name
        if base_name in norm_map:
            return norm_map[base_name]
        base = base_name[:-1]
        qlabel_counts[base] = qlabel_counts.get(base, 0) + 1
        n = f"{base}{qlabel_counts[base]}"
        norm_map[base_name] = n
        return n

    def module_for_symbol(sym: str, fallback_module: str = "") -> str:
        mods = symbol_modules.get(sym.upper(), set())
        if len(mods) == 1:
            return next(iter(mods))
        return fallback_module

    def mapping_looks_valid(lbl: str, cand_addr: int) -> bool:
        k = (lbl, cand_addr)
        if k in map_ok_cache:
            return map_ok_cache[k]
        try:
            s5 = source_first5(root, lbl, macros, symbols)
            r5 = rom_first5(lines, cand_addr, need=max(1, len(s5)))
            ok = bool(s5) and len(r5) >= len(s5) and ccm.mnemonics_equivalent(s5, r5[:len(s5)])
        except Exception:
            ok = False
        map_ok_cache[k] = ok
        return ok

    def labels_in_module(mp: pathlib.Path) -> set[str]:
        if mp in module_labels_cache:
            return module_labels_cache[mp]
        labs: set[str] = set()
        for ln in mp.read_text(errors='ignore').splitlines():
            code = ccm.strip_comment(ln)
            m = SRC_LABEL_COLON_RE.match(code)
            if m:
                labs.add(m.group(1))
                continue
            # Also capture TI-style label field without colon, including
            # "LABEL OP ..." (e.g. "NXTCHAR ADDI R7,R2").
            lbl, _rest = ccm.split_optional_label(code)
            if lbl and not lbl.startswith('.'):
                labs.add(lbl)
                continue
            if code and not code[:1].isspace():
                toks = ccm.parse_line_tokens(code)
                if len(toks) == 1 and LABEL_TOKEN_RE.match(toks[0]) and not toks[0].startswith('.'):
                    labs.add(toks[0])
        module_labels_cache[mp] = labs
        return labs

    def resolve_target_module(st: str, current_mp: Optional[pathlib.Path]) -> str:
        # Prefer local/module labels first to avoid cross-module homonym drift
        # (e.g. NXTCHAR in FONT1A vs NXTCHAR in TEXT).
        if current_mp is not None and st in labels_in_module(current_mp):
            return current_mp.stem
        try:
            smp,_=ccm.find_source_module_for_function(root,st)
            return smp.stem
        except Exception:
            return ''

    def classify_label_kind(lbl: str, mod: str) -> str:
        if (lbl, mod) in code_ref_labels:
            return "code"
        if (lbl, mod) in operand_ref_labels:
            return "data"
        mp = None
        try:
            mp, si = ccm.find_source_module_for_function(root, lbl)
            lines = mp.read_text(errors='ignore').splitlines()
        except Exception:
            # Fallback: resolve by module-local scan when global symbol lookup misses
            # declaration labels like "M1 .string ...".
            if not mod:
                return "unknown"
            mp2 = root / f"{mod}.ASM"
            if not mp2.exists():
                return "unknown"
            lines = mp2.read_text(errors='ignore').splitlines()
            si = -1
            for i, ln in enumerate(lines):
                code = ccm.strip_comment(ln)
                l, r = ccm.split_optional_label(code)
                if l == lbl:
                    si = i
                    break
            if si < 0:
                return "unknown"
        candidates: List[str] = []

        # 1) Inline label with opcode/directive.
        code = ccm.strip_comment(lines[si])
        _label, rest = ccm.split_optional_label(code)
        if rest:
            candidates.append(rest.strip())

        # 2) Next meaningful line after label.
        j = si + 1
        while j < len(lines):
            s = ccm.strip_comment(lines[j]).strip()
            if not s or s.startswith("*"):
                j += 1
                continue
            candidates.append(s)
            break

        # Only declaration-style rule: labels on data directives are data.
        for c in candidates:
            op_idx, toks = ccm.split_label_and_tokens(ccm.strip_comment(c))
            if not toks:
                continue
            op = toks[op_idx].lower()
            if op in DATA_DECL_OPS:
                return "data"
            return "code"
        return "unknown"

    def source_row_decl_size(src_raw: str) -> int:
        """Return data words emitted by a non-code source row, else 0.
        We only advance synthetic data cursor for real storage declarations.
        """
        code = ccm.strip_comment(src_raw)
        _l, rest = ccm.split_optional_label(code)
        if not rest:
            return 0
        op_idx, toks = ccm.split_label_and_tokens(rest)
        if not toks:
            return 0
        op = toks[op_idx].lower()
        if op in {".word", ".byte", ".float", ".double", ".string"}:
            return 1
        if op == ".bss":
            # .bss LABEL, N -> N words
            if len(toks) >= op_idx + 2:
                n = toks[op_idx + 1].strip(",")
                try:
                    if n.lower().startswith("0x"):
                        return max(0, int(n, 16))
                    if n.lower().endswith("h"):
                        return max(0, int(n[:-1], 16))
                    return max(0, int(n, 10))
                except Exception:
                    return 1
            return 1
        return 0

    def find_label_source_line(lbl: str, mod: str) -> str:
        if mod:
            mp2 = root / f"{mod}.ASM"
            if mp2.exists():
                for ln in mp2.read_text(errors='ignore').splitlines():
                    c = ccm.strip_comment(ln)
                    l, _ = ccm.split_optional_label(c)
                    if l == lbl:
                        return c
        try:
            mp, si = ccm.find_source_module_for_function(root, lbl)
            return ccm.strip_comment(mp.read_text(errors='ignore').splitlines()[si])
        except Exception:
            return ""

    def is_excludable_symbol_decl(lbl: str, mod: str) -> bool:
        # Exclude non-address declaration symbols (.set/equ/.include/.globl/...).
        # These are assembler symbols, not ROM-location labels.
        code = find_label_source_line(lbl, mod)
        if not code:
            return False
        _l, rest = ccm.split_optional_label(code)
        if not rest:
            return False
        op_idx, toks = ccm.split_label_and_tokens(rest)
        if not toks:
            return False
        op = toks[op_idx].lower()
        return op in {".set", "equ", ".include", ".globl", ".macro", ".if", ".else", ".endif", "fbss", ".usect"}

    def maybe_record_inline_label(src_raw: str, module: str, rom_ea: int) -> None:
        code = ccm.strip_comment(src_raw)
        lbl, rest = ccm.split_optional_label(code)
        if not lbl or lbl.startswith("."):
            return
        if rest and source_row_decl_size(src_raw) > 0:
            # Declaration labels may live in a linker-selected section that does
            # not correspond to this walker's current ROM cursor. Bind these
            # from instruction operands instead.
            return
        key = (lbl, module)
        if key not in discovered_labels:
            discovered_labels[key] = rom_ea

    def extract_symbol_operands(src_raw: str, module: str, current_mp: Optional[pathlib.Path]) -> List[Tuple[int, str, str]]:
        code = ccm.strip_comment(src_raw)
        op_idx, toks = ccm.split_label_and_tokens(code)
        if not toks:
            return []
        op = toks[op_idx].upper()
        # fbss -> .usect (oncram/FRAM), not inline FASTRAM address progression.
        # Do not synthesize inline labels from these operands.
        if op == "FBSS":
            return []
        # Transfer instructions already resolve targets via control-flow mapping;
        # do not also treat their operands as data labels.
        if ccm.CALL_MNEMONIC_RE.match(op) or op.startswith("B") or op.startswith("DB"):
            return []
        out: List[Tuple[int, str, str]] = []
        for idx, t in enumerate(toks[op_idx + 1:]):
            # Bind symbol operands from any argument position by matching
            # source operand tokens with ROM numeric operands at the same index.
            if "@" not in t:
                continue
            tok = t.strip().strip(",")
            if not tok:
                continue
            # remove wrappers like @symbol, *+AR0(SYM), (SYM)
            tok = tok.lstrip("@*#+-")
            tok = tok.strip("()")
            if not tok:
                continue
            if tok.startswith("."):
                continue
            if NUMERIC_TOKEN_RE.match(tok):
                continue
            if not LABEL_TOKEN_RE.match(tok):
                continue
            # Skip obvious register names
            up = tok.upper()
            if re.match(r"^(AR[0-7]|R[0-7]|DP|IR[01]|BK|SP|ST|RS|RC|IE|IOF)$", up):
                continue
            # Address-binding is only valid for symbols declared in the module
            # whose source/ROM streams are currently being stepped.
            tok_module = module_for_symbol(tok, resolve_target_module(tok, current_mp) or module)
            if not tok_module:
                continue
            if is_excludable_symbol_decl(tok, tok_module):
                continue
            out.append((idx, tok, tok_module))
        return out

    def maybe_record_operand_labels(src_raw: str, module: str, current_mp: Optional[pathlib.Path], rom_toks: List[str]) -> None:
        for operand_idx, tok, tok_module in extract_symbol_operands(src_raw, module, current_mp):
            rom_operand_idx = operand_idx + 1
            if rom_operand_idx >= len(rom_toks):
                continue
            ea = parse_rom_number(rom_toks[rom_operand_idx])
            if ea is None:
                continue
            key = (tok, tok_module)
            prev = discovered_labels.get(key)
            if prev is not None and prev != ea:
                conflicts.append((
                    tok,
                    tok_module,
                    f'0x{prev:08X}',
                    'operand',
                    tok,
                    tok_module,
                    f'0x{prev:08X}',
                    f'0x{ea:08X}',
                    '-1',
                    src_raw.rstrip(),
                    ' '.join(rom_toks),
                ))
                continue
            discovered_labels[key] = ea
            operand_ref_labels.add(key)

    def run_pass(
        *,
        direction: str,
        src_ops: List[Tuple[str, Optional[str], int, str, bool]],
        rom_ops: List[RomOp],
        start_addr: int,
        module: str,
        current_mp: Optional[pathlib.Path],
        root_label: str,
        root_addr: int,
    ) -> Tuple[str, int, str, str]:
        reason='complete'
        idx=-1
        src=''
        rom=''
        si = 0
        ri = 0
        n_src = len(src_ops)
        n_rom = len(rom_ops)
        last_raddr = start_addr
        addr_step = 1 if direction == 'forward' else -1
        while si < n_src:
            smn, st, sline, sraw, s_is_code = src_ops[si]
            if s_is_code:
                if ri >= n_rom:
                    reason='rom_exhausted'; idx=si; src=f'{sline}:{sraw}'; rom=''; break
                rmn, rt, raddr, r_is_code, rtoks = rom_ops[ri]
                last_raddr = raddr
                if not r_is_code or not ccm.mnemonics_equivalent([smn],[rmn]):
                    reason='mnemonic_mismatch'; idx=si; src=f'{sline}:{sraw}'; rom=f'0x{raddr:08X}:{rmn}'; break
                maybe_record_inline_label(sraw, module, raddr)
                maybe_record_operand_labels(sraw, module, current_mp, rtoks)
                if ccm.strip_comment(sraw).strip().upper().startswith("SETDP"):
                    dp0_eas.add(raddr)
                ri += 1
            else:
                dsz = source_row_decl_size(sraw)
                if dsz > 0:
                    last_raddr = (last_raddr + (addr_step * dsz)) & 0xFFFFFFFF
                maybe_record_inline_label(sraw, module, last_raddr)
                si += 1
                continue
            if st is not None:
                if rt is None:
                    reason='call_target_missing_in_rom'; idx=si; src=f'{sline}:{sraw}'; rom=f'0x{raddr:08X}:{rmn}'; break
                ra=l2a.get(rt)
                st_module = resolve_target_module(st, current_mp)
                code_ref_labels.add((st, st_module))
                if ra is not None:
                    prev = label_addr_first.get((st, st_module))
                    if prev is None:
                        if mapping_looks_valid(st, ra):
                            label_addr_first[(st, st_module)] = ra
                    elif prev != ra:
                        conflicts.append((root_label, module, f'0x{root_addr:08X}', direction, st, st_module, f'0x{prev:08X}', f'0x{ra:08X}', str(si), f'{sline}:{sraw}', f'0x{raddr:08X}:{rt}'))
                        reason='target_address_conflict'
                        idx=si
                        src=f'{sline}:{sraw}'
                        rom=f'0x{raddr:08X}:{rt}'
                        break
                if ra is not None and mapping_looks_valid(st, ra) and (st,st_module) not in seen:
                    q.append((st,ra))
                if ra is None:
                    reason='rom_call_target_unresolved'; idx=si; src=f'{sline}:{sraw}'; rom=f'0x{raddr:08X}:{rt}'; break
            si += 1
        else:
            if ri < n_rom:
                reason='source_exhausted'; idx=si
        return reason, idx, src, rom

    while q:
        label,addr=q.popleft()
        module=''
        current_mp: Optional[pathlib.Path] = None
        try:
            mp,_=ccm.find_source_module_for_function(root,label)
            current_mp = mp
            module=mp.stem
        except Exception:
            module=''
        key=(label,module)
        if key in seen:
            continue
        seen.add(key)

        status='UNVERIFIED'; sm=[]; rm=[]
        try:
            sm=source_first5(root,label,macros,symbols)
            rm=rom_first5(lines,addr,need=max(1,len(sm)))
            status='OK' if sm and len(rm)>=len(sm) and ccm.mnemonics_equivalent(sm,rm[:len(sm)]) else 'MISMATCH'
        except Exception:
            status='UNVERIFIED'

        try:
            sl, sb = source_module_tail_lines(root,label)
            sops = source_ops_stream(sl, macros, symbols, base_line_no=sb)
            rops = rom_ops_stream(lines, addr)
            f_reason, f_idx, f_src, f_rom = run_pass(
                direction='forward',
                src_ops=sops,
                rom_ops=rops,
                start_addr=addr,
                module=module,
                current_mp=current_mp,
                root_label=label,
                root_addr=addr,
            )
            stops.append((label,module,f'0x{addr:08X}','forward',f_reason,str(f_idx),f_src,f_rom,str(len(sops)),str(len(rops))))

            hl, hb = source_module_head_lines(root,label)
            hsops=list(reversed(source_ops_stream(hl, macros, symbols, base_line_no=hb)))
            hrops=rom_ops_stream(lines, addr, reverse=True)
            r_reason, r_idx, r_src, r_rom = run_pass(
                direction='reverse',
                src_ops=hsops,
                rom_ops=hrops,
                start_addr=addr,
                module=module,
                current_mp=current_mp,
                root_label=label,
                root_addr=addr,
            )
            stops.append((label,module,f'0x{addr:08X}','reverse',r_reason,str(r_idx),r_src,r_rom,str(len(hsops)),str(len(hrops))))
        except Exception as e:
            stops.append((label,module,f'0x{addr:08X}','both',f'exception:{type(e).__name__}:{e}','-1',traceback.format_exc().replace('\n', '\\n'),'','',''))

        discovered_labels.setdefault((label, module), addr)
        if not is_excludable_symbol_decl(label, module):
            kind = classify_label_kind(label, module)
            rows.append((label,norm_label(label,module),module,kind,f'0x{addr:08X}',status,' '.join(sm),' '.join(rm[:len(sm)] if sm else rm)))

    # Resolve one-level pointer cells: LABEL_X .word LABEL_Y
    # If LABEL_X has a discovered address and a ROM word value there, bind LABEL_Y.
    for lhs, rhs, mod in word_symbol_defs:
        lkey = (lhs, mod)
        lea = discovered_labels.get(lkey)
        if lea is None:
            continue
        rv = wvals.get(lea)
        if rv is None:
            continue
        rmod = module_for_symbol(rhs, mod)
        rkey = (rhs, rmod)
        if rkey not in discovered_labels:
            discovered_labels[rkey] = rv
            operand_ref_labels.add(rkey)

    # Emit additional discovered labels harvested from matched source lines.
    existing={(r[0], r[2]) for r in rows}
    for (lbl, mod), ea in sorted(discovered_labels.items(), key=lambda kv: (kv[1], kv[0][1], kv[0][0])):
        if (lbl, mod) in existing:
            continue
        if is_excludable_symbol_decl(lbl, mod):
            continue
        kind = classify_label_kind(lbl, mod)
        rows.append((lbl, norm_label(lbl,mod), mod, kind, f'0x{ea:08X}', 'OK', '', ''))

    out=pathlib.Path(args.out)
    out.write_text('label\tlabel_norm\tmodule\tkind\taddress\tstatus\tsrc_first5\trom_first5\n' + '\n'.join('\t'.join(r) for r in rows) + '\n')
    sout=pathlib.Path(args.stops_out)
    sout.write_text('label\tmodule\taddress\tdirection\treason\tstep_idx\tsource_line\trom_at_step\tlen_src\tlen_rom\n' + '\n'.join('\t'.join(s) for s in stops) + '\n')
    cout=pathlib.Path(args.conflicts_out)
    cout.write_text(
        'parent_label\tparent_module\tparent_addr\tdirection\tlabel\tcallee_module\tfirst_addr\tnew_addr\tstep_idx\tsource_line\trom_at_step\n'
        + '\n'.join('\t'.join(c) for c in conflicts)
        + '\n'
    )
    dpout=pathlib.Path(args.dp0_out)
    dpout.write_text('address\n' + '\n'.join(f'0x{ea:08X}' for ea in sorted(dp0_eas)) + '\n')

    ok=sum(1 for r in rows if r[5]=='OK')
    mm=sum(1 for r in rows if r[5]=='MISMATCH')
    uv=sum(1 for r in rows if r[5]=='UNVERIFIED')
    print(f'wrote {out}')
    print(f'wrote {sout}')
    print(f'wrote {cout}')
    print(f'wrote {dpout}')
    print(f'total={len(rows)} ok={ok} mismatch={mm} unverified={uv}')
    print(f'conflicts={len(conflicts)}')

if __name__=='__main__':
    main()

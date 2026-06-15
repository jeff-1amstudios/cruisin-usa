#!/usr/bin/env python3
from __future__ import annotations

import argparse
import pathlib
import re
from collections import deque
from dataclasses import dataclass
from functools import lru_cache
from typing import Dict, List, Optional, Set, Tuple


CALL_MNEMONIC_RE = re.compile(r"^(CALL(?:[A-Z]+)?)\b", re.IGNORECASE)
FUNC_LABEL_RE = re.compile(r"^([A-Za-z_.$?][A-Za-z0-9_.$?]*):")
ROM_LINE_RE = re.compile(r"^CODE:([0-9A-Fa-f]{8})\s+(.*)$")
ROM_CALL_RE = re.compile(r"\bcall[a-z]*\s+([A-Za-z_.$?][A-Za-z0-9_.$?]*)", re.IGNORECASE)
ROM_CALL_ANY_RE = re.compile(r"\b(call[a-z]*)\s+([^\s;]+)", re.IGNORECASE)


@dataclass
class MacroDef:
    name: str
    params: List[str]
    body: List[str]


def iter_source_files(root: pathlib.Path, suffixes: Tuple[str, ...]) -> Tuple[pathlib.Path, ...]:
    wanted = {s.upper() for s in suffixes}
    bases = [root]
    asm_dir = root / "asm"
    if asm_dir.exists():
        bases.append(asm_dir)
    out: List[pathlib.Path] = []
    seen: Set[pathlib.Path] = set()
    for base in bases:
        for p in sorted(base.iterdir()):
            if not p.is_file() or p.suffix.upper() not in wanted:
                continue
            rp = p.resolve()
            if rp in seen:
                continue
            seen.add(rp)
            out.append(p)
    return tuple(out)


def strip_comment(line: str) -> str:
    quote = ""
    for i, ch in enumerate(line):
        if quote:
            if ch == quote:
                quote = ""
            continue
        if ch in {"'", '"'}:
            quote = ch
            continue
        if ch == ";":
            return line[:i]
    return line


def normalize_token(tok: str) -> str:
    return tok.strip().rstrip(",")


def parse_macros(root: pathlib.Path) -> Dict[str, MacroDef]:
    macros: Dict[str, MacroDef] = {}
    files = iter_source_files(root, (".ASM", ".EQU"))

    macro_start = re.compile(
        r"^\s*([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s+\.MACRO\b(.*)$", re.IGNORECASE
    )
    macro_end = re.compile(r"^\s*\.ENDM\b", re.IGNORECASE)

    for p in files:
        lines = p.read_text(errors="ignore").splitlines()
        i = 0
        while i < len(lines):
            raw = lines[i]
            m = macro_start.match(raw)
            if not m:
                i += 1
                continue
            name = m.group(1).upper()
            params_raw = m.group(2).strip()
            params = [p.strip().upper() for p in params_raw.split(",") if p.strip()]
            body: List[str] = []
            i += 1
            while i < len(lines) and not macro_end.match(lines[i]):
                body.append(lines[i])
                i += 1
            macros[name] = MacroDef(name=name, params=params, body=body)
            i += 1

    return macros


def parse_set_symbols(root: pathlib.Path) -> Dict[str, int]:
    symbols: Dict[str, int] = {}
    files = iter_source_files(root, (".ASM", ".EQU"))
    set_re = re.compile(r"^\s*([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s+\.set\s+(.+)$", re.IGNORECASE)
    for p in files:
        for raw in p.read_text(errors="ignore").splitlines():
            code = strip_comment(raw).strip()
            m = set_re.match(code)
            if not m:
                continue
            name = m.group(1).upper()
            expr = m.group(2).strip()
            try:
                symbols[name] = eval_if_expr(expr, symbols)
            except Exception:
                continue
    return symbols


def resolve_source_function_name(user_func: str) -> str:
    aliases = {
        "cint": "_c_int00",
    }
    return aliases.get(user_func, user_func)


def resolve_rom_function_name(source_func: str, user_override: Optional[str]) -> str:
    if user_override:
        return user_override
    special = {
        "_c_int00": "vec_reset__c_int00_target_00004AE6",
    }
    return special.get(source_func, source_func)


@lru_cache(maxsize=8)
def _all_asm_files(root: pathlib.Path) -> Tuple[pathlib.Path, ...]:
    return iter_source_files(root, (".ASM",))


@lru_cache(maxsize=1)
def _source_function_index(root: pathlib.Path) -> Dict[str, List[Tuple[pathlib.Path, int]]]:
    out: Dict[str, List[Tuple[pathlib.Path, int]]] = {}
    seen_pairs: Dict[str, set[Tuple[pathlib.Path, int]]] = {}
    # Accept both label-only and inline-label forms:
    #   FOO:
    #   BAR: LDI 1,R0
    label_re = re.compile(r"^\s*([A-Za-z_.$?][A-Za-z0-9_.$?]*):(?:\s+.*)?$")
    bare_label_re = re.compile(r"^\s*([A-Za-z_.$?][A-Za-z0-9_.$?]*)\s*$")
    macros = {k.upper() for k in parse_macros(root).keys()}
    op_like = {
        "RET","RETS","RETSU","RETI","CALL","B","BU","BR","BD","BUD","BRD",
        "LDI","LDIU","LDP","STI","ADDI","SUBI","NOP","PUSH","POP","CMP","CMPI",
        "AND","OR","XOR","ASH","LSH","MPYF","MPYI","LDF","STF","RPTS","RPTB",
    }
    for asm in _all_asm_files(root):
        lines = asm.read_text(errors="ignore").splitlines()
        for idx, line in enumerate(lines):
            m = label_re.match(line)
            if not m:
                # Also support column-1 bare labels used throughout this codebase:
                #   WAITMSEC
                #       LDI ...
                code = strip_comment(line)
                if code and not code[:1].isspace():
                    toks = parse_line_tokens(code)
                    if len(toks) == 1:
                        t = toks[0]
                        tu = t.upper()
                        if (
                            re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", t)
                            and not t.startswith(".")
                            and tu not in macros
                            and tu not in op_like
                        ):
                            if (asm, idx) not in seen_pairs.setdefault(t, set()):
                                out.setdefault(t, []).append((asm, idx))
                                seen_pairs[t].add((asm, idx))
                continue
            primary = m.group(1)
            if (asm, idx) not in seen_pairs.setdefault(primary, set()):
                out.setdefault(primary, []).append((asm, idx))
                seen_pairs[primary].add((asm, idx))

            # Some modules use a public/function label alias style:
            #   PRC_SLEEP:
            #   SLEEP
            # Record immediately-following bare labels as aliases to this entry.
            j = idx + 1
            while j < len(lines):
                s = strip_comment(lines[j]).strip()
                if not s:
                    j += 1
                    continue
                if s.startswith("*"):
                    j += 1
                    continue
                if s.startswith("."):
                    break
                if label_re.match(lines[j]):
                    break
                bm = bare_label_re.match(lines[j])
                if not bm:
                    break
                alias = bm.group(1)
                if (asm, j) not in seen_pairs.setdefault(alias, set()):
                    out.setdefault(alias, []).append((asm, j))
                    seen_pairs[alias].add((asm, j))
                j += 1
    return out


@lru_cache(maxsize=256)
def find_source_module_for_function(root: pathlib.Path, function_name: str) -> Tuple[pathlib.Path, int]:
    candidates = _source_function_index(root).get(function_name, [])
    if not candidates:
        raise RuntimeError(f"Could not find function '{function_name}' in any top-level *.ASM file")
    if len(candidates) > 1:
        files = ", ".join(str(c[0].name) for c in candidates)
        raise RuntimeError(f"Function '{function_name}' appears in multiple modules: {files}")
    return candidates[0]


@lru_cache(maxsize=256)
def _read_text_lines(path: pathlib.Path) -> Tuple[str, ...]:
    return tuple(path.read_text(errors="ignore").splitlines())


@lru_cache(maxsize=512)
def extract_source_function_lines(module_path: pathlib.Path, function_name: str, start_idx: int) -> List[str]:
    lines = _read_text_lines(module_path)

    # _c_int00 is a special long function region in this codebase.
    if function_name == "_c_int00":
        end = len(lines)
        for i in range(start_idx + 1, len(lines)):
            if re.match(r"^\s*ENABLEGIE:\s*$", lines[i]):
                end = i
                break
        return list(lines[start_idx + 1 : end])

    out: List[str] = []
    terminators = {
        "RET",
        "RETS",
        "RETSU",
        "RETI",
        "B",
        "BU",
        "BR",
        "BD",
        "BUD",
        "BRD",
    }

    # If the function label is inline with an instruction (e.g. "MSG1: LDI ..."),
    # include that first instruction so mnemonic/call extraction stays aligned.
    start_raw = lines[start_idx]
    start_code = strip_comment(start_raw)
    m_inline = re.match(r"^\s*[A-Za-z_.$?][A-Za-z0-9_.$?]*:\s+(.+)$", start_code)
    if m_inline:
        first = m_inline.group(1).strip()
        if first:
            # Keep leading whitespace so token splitter treats this as an opcode line.
            out.append("\t" + first)
            op_idx, toks = split_label_and_tokens(first)
            if toks:
                op = toks[op_idx].upper()
                if op in terminators:
                    return out

    for i in range(start_idx + 1, len(lines)):
        raw = lines[i]
        out.append(raw)
        code = strip_comment(raw)
        if not code.strip():
            continue
        op_idx, toks = split_label_and_tokens(code)
        if not toks:
            continue
        op = toks[op_idx].upper()
        if op in terminators:
            break

    return out


def is_label_only(code: str) -> bool:
    s = code.strip()
    if not s:
        return False
    return bool(FUNC_LABEL_RE.match(s))


def parse_line_tokens(code: str) -> List[str]:
    s = code.strip()
    if not s:
        return []
    out: List[str] = []
    cur: List[str] = []
    quote = ""
    for ch in s:
        if quote:
            cur.append(ch)
            if ch == quote:
                quote = ""
            continue
        if ch in {"'", '"'}:
            quote = ch
            cur.append(ch)
            continue
        if ch.isspace() or ch == ",":
            tok = normalize_token("".join(cur))
            if tok:
                out.append(tok)
            cur = []
            continue
        cur.append(ch)
    tok = normalize_token("".join(cur))
    if tok:
        out.append(tok)
    return out


def split_label_and_tokens(code: str) -> Tuple[int, List[str]]:
    toks = parse_line_tokens(code)
    if not toks:
        return 0, toks
    op_idx = 0
    op = toks[op_idx].upper()
    if (
        len(toks) >= 3
        and not code[:1].isspace()
        and not toks[0].endswith(":")
        and re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", toks[0])
        and re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", toks[1])
    ):
        # "LABEL OP ..." style (label has no trailing colon in some source files)
        op_idx = 1
        return op_idx, toks
    if (
        len(toks) >= 2
        and not code[:1].isspace()
        and not CALL_MNEMONIC_RE.match(op)
        and not op.startswith(".")
    ):
        op_idx = 1
    return op_idx, toks


def parse_int_literal(tok: str) -> Optional[int]:
    t = tok.strip()
    if not t:
        return None
    if t.lower().endswith("h"):
        t = t[:-1]
        try:
            return int(t, 16)
        except ValueError:
            return None
    try:
        return int(t, 10)
    except ValueError:
        return None


def eval_if_expr(expr: str, symbols: Dict[str, int]) -> int:
    s = expr.strip()
    m = re.match(r"^(.+?)\s*(=|==)\s*(.+)$", s)
    if m:
        l = eval_if_expr(m.group(1), symbols)
        r = eval_if_expr(m.group(3), symbols)
        return 1 if l == r else 0
    v = parse_int_literal(s)
    if v is not None:
        return v
    return symbols.get(s.upper(), 0)


def source_op_name(code: str) -> str:
    op_idx, toks = split_label_and_tokens(code)
    if not toks or op_idx >= len(toks):
        return ""
    return toks[op_idx].upper()


def is_text_directive(code: str) -> bool:
    return bool(re.match(r"^\.text\b", code.strip(), re.IGNORECASE))


def is_longrout_start(code: str) -> bool:
    return source_op_name(code) == "LONGROUT"


def is_longrout_end(code: str) -> bool:
    return source_op_name(code) == "END_LONGROUT"


def iter_active_lines(lines: List[str], symbols: Dict[str, int]) -> List[str]:
    out: List[str] = []
    active_stack: List[bool] = [True]
    in_macro = 0
    skip_longrout_body = False
    for ln in lines:
        code = strip_comment(ln).strip()
        if not code:
            continue
        if code.startswith("*"):
            continue
        if in_macro:
            if re.match(r"^\.ENDM\b", code, re.IGNORECASE):
                in_macro -= 1
            elif re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*\s+\.MACRO\b", code, re.IGNORECASE):
                in_macro += 1
            continue
        if re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*\s+\.MACRO\b", code, re.IGNORECASE):
            in_macro += 1
            continue
        ifm = re.match(r"^\.if\s+(.+)$", code, re.IGNORECASE)
        if ifm:
            cond = eval_if_expr(ifm.group(1), symbols) != 0
            active_stack.append(active_stack[-1] and cond)
            continue
        if re.match(r"^\.else\b", code, re.IGNORECASE):
            if len(active_stack) > 1:
                parent = active_stack[-2]
                current = active_stack[-1]
                active_stack[-1] = parent and (not current)
            continue
        if re.match(r"^\.endif\b", code, re.IGNORECASE):
            if len(active_stack) > 1:
                active_stack.pop()
            continue
        if active_stack[-1]:
            if skip_longrout_body:
                if is_text_directive(code) or is_longrout_end(code):
                    skip_longrout_body = False
                continue
            out.append(ln)
            if is_longrout_start(code):
                skip_longrout_body = True
    return out


class MacroExpander:
    def __init__(self, macros: Dict[str, MacroDef], symbols: Dict[str, int]) -> None:
        self.macros = macros
        self.symbols = symbols
        self._expand_counter = 0

    def parse_invocation_args(self, op_idx: int, toks: List[str]) -> List[str]:
        if op_idx + 1 >= len(toks):
            return []
        return [normalize_token(t) for t in toks[op_idx + 1 :] if normalize_token(t)]

    def substitute_params(self, line: str, params: List[str], args: List[str]) -> str:
        out = line
        for i, p in enumerate(params):
            arg = args[i] if i < len(args) else ""
            out = re.sub(
                rf":{re.escape(p)}:",
                lambda _m, a=arg: a,
                out,
                flags=re.IGNORECASE,
            )
        return out

    def uniquify_q_labels(self, lines_in: List[str]) -> List[str]:
        self._expand_counter += 1
        suffix = f"__mx{self._expand_counter}"
        q_pat = re.compile(r"\b([A-Za-z_.$][A-Za-z0-9_.$]*\?)\b")
        mapping: Dict[str, str] = {}

        def repl(m: re.Match[str]) -> str:
            k = m.group(1)
            if k not in mapping:
                mapping[k] = f"{k[:-1]}{suffix}"
            return mapping[k]

        return [q_pat.sub(repl, ln) for ln in lines_in]

    def expand(self, op: str, args: List[str]) -> List[str]:
        if op == "PUSHM":
            return [f"\tPUSH\t{a}" for a in args]
        if op == "POPM":
            return [f"\tPOP\t{a}" for a in args]
        if op == "PUSHFM":
            return [f"\tPUSHF\t{a}" for a in args]
        if op == "POPFM":
            return [f"\tPOPF\t{a}" for a in args]
        md = self.macros[op]
        expanded = [self.substitute_params(ln, md.params, args) for ln in md.body]
        return self.uniquify_q_labels(expanded)


def extract_calls_from_lines(
    lines: List[str],
    macros: Dict[str, MacroDef],
    symbols: Dict[str, int],
    recursion_limit: int = 10,
) -> List[str]:
    out: List[str] = []
    active_stack: List[bool] = [True]
    expander = MacroExpander(macros, symbols)

    def walk_line(code: str, depth: int, active: Set[str]) -> None:
        if depth > recursion_limit:
            return

        code = strip_comment(code)
        if not code.strip():
            return
        if is_label_only(code):
            return

        op_idx, toks = split_label_and_tokens(code)
        if not toks:
            return

        op = toks[op_idx].upper()
        if op not in macros and op_idx == 1 and len(toks) > 1:
            op = toks[1].upper()

        callm = CALL_MNEMONIC_RE.match(op)
        if callm and len(toks) >= (op_idx + 2):
            target = toks[op_idx + 1]
            out.append(target)
            return

        if op in macros:
            if op in active:
                return
            active.add(op)
            args = expander.parse_invocation_args(op_idx, toks)
            expanded = expander.expand(op, args)
            for b in iter_active_lines(expanded, symbols):
                walk_line(b, depth + 1, active)
            active.remove(op)

    for ln in lines:
        code = strip_comment(ln).strip()
        if not code:
            continue

        ifm = re.match(r"^\.if\s+(.+)$", code, re.IGNORECASE)
        if ifm:
            cond = eval_if_expr(ifm.group(1), symbols) != 0
            active_stack.append(active_stack[-1] and cond)
            continue

        if re.match(r"^\.else\b", code, re.IGNORECASE):
            if len(active_stack) > 1:
                parent = active_stack[-2]
                current = active_stack[-1]
                active_stack[-1] = parent and (not current)
            continue

        if re.match(r"^\.endif\b", code, re.IGNORECASE):
            if len(active_stack) > 1:
                active_stack.pop()
            continue

        if not active_stack[-1]:
            continue

        walk_line(ln, 0, set())

    return out


def macro_emits_executable(
    macro_name: str,
    macros: Dict[str, MacroDef],
    symbols: Dict[str, int],
    cache: Dict[str, bool],
    active: Set[str],
) -> bool:
    name = macro_name.upper()
    if name in cache:
        return cache[name]
    if name in active or name not in macros:
        return False
    active.add(name)
    macro = macros[name]
    expander = MacroExpander(macros, symbols)
    expanded = expander.expand(name, ["0"] * len(macro.params))
    for ln in expanded:
        code = strip_comment(ln)
        if not code.strip():
            continue
        stripped = code.strip()
        if re.match(r"^\.if\b", stripped, re.IGNORECASE):
            continue
        if re.match(r"^\.else\b", stripped, re.IGNORECASE):
            continue
        if re.match(r"^\.endif\b", stripped, re.IGNORECASE):
            continue
        lbl, rest = split_optional_label(code)
        if lbl is not None:
            if not rest:
                continue
            code = "\t" + rest
        op_idx, toks = split_label_and_tokens(code)
        if not toks or op_idx >= len(toks):
            continue
        op = toks[op_idx].upper()
        if op in macros:
            if macro_emits_executable(op, macros, symbols, cache, active):
                cache[name] = True
                active.remove(name)
                return True
            continue
        if op.startswith(".") or op == "EQU":
            continue
        if op in {"FBSS", "PBSS", "HIBSS", "LOBSS", "PHIBSS", "ROMDATA", "FSECT", "PSECT", "HSECT", "DIAGTEXT"}:
            continue
        cache[name] = True
        active.remove(name)
        return True
    active.remove(name)
    cache[name] = False
    return False


def extract_mnemonics_from_lines(
    lines: List[str],
    macros: Dict[str, MacroDef],
    symbols: Dict[str, int],
    limit: Optional[int] = None,
    recursion_limit: int = 10,
    expand_macros: bool = False,
) -> List[str]:
    out: List[str] = []
    expander = MacroExpander(macros, symbols)

    def walk_line(code: str, depth: int, active: Set[str]) -> None:
        if (limit is not None and len(out) >= limit) or depth > recursion_limit:
            return
        code = strip_comment(code)
        if not code.strip():
            return
        # Parallel issue form: "|| OP ..." should parse as mnemonic OP.
        if code.lstrip().startswith("||"):
            code = re.sub(r"^\s*\|\|\s*", "", code)
            if not code.strip():
                return
        op_idx, toks = split_label_and_tokens(code)
        if not toks:
            return
        if (
            len(toks) == 1
            and re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", toks[0])
            and not code[:1].isspace()
        ):
            return
        op = toks[op_idx].upper()
        if (
            op_idx == 1
            and len(toks) >= 3
            and re.match(r"^[A-Z_.$?][A-Z0-9_.$?]*$", toks[0], re.IGNORECASE)
            and not toks[0].endswith(":")
        ):
            # Handle "label  MNEMONIC operands..." form without a trailing colon.
            op = toks[1].upper()
        if op.endswith(":"):
            return
        if op in macros and expand_macros:
            if op in active:
                return
            active.add(op)
            args = expander.parse_invocation_args(op_idx, toks)
            expanded = expander.expand(op, args)
            for b in iter_active_lines(expanded, symbols):
                walk_line(b, depth + 1, active)
                if limit is not None and len(out) >= limit:
                    break
            active.remove(op)
            return
        if op.startswith(".") or op == "EQU":
            return
        # Normalize common assembler pseudo-ops/macros to core mnemonics seen in ROM.
        norm = {
            "CLRI": "subi",
            "STPI": "sti",
            "RETS": "retsu",
            "RET": "retsu",
            "RETI": "reti",
        }.get(op, op.lower())
        out.append(norm)

    for ln in iter_active_lines(lines, symbols):
        if limit is not None and len(out) >= limit:
            break
        walk_line(ln, 0, set())
    return out


@lru_cache(maxsize=256)
def extract_rom_function_lines(rom_path: pathlib.Path, rom_function_name: str) -> List[str]:
    lines = _read_text_lines(rom_path)
    start_label_re = re.compile(rf"^CODE:[0-9A-Fa-f]{{8}}\s+{re.escape(rom_function_name)}:\s")
    start_idx: Optional[int] = None
    for i, line in enumerate(lines):
        if start_label_re.match(line):
            start_idx = i
            break
    if start_idx is None:
        raise RuntimeError(f"Could not find ROM function '{rom_function_name}' in rom.lst")
    end_idx: Optional[int] = None
    for i in range(start_idx + 1, len(lines)):
        if f"End of function {rom_function_name}" in lines[i]:
            end_idx = i
            break
    if end_idx is None:
        raise RuntimeError(f"Could not find end marker for ROM function '{rom_function_name}' in rom.lst")
    return list(lines[start_idx:end_idx])


def extract_rom_mnemonics(
    rom_path: pathlib.Path, rom_function_name: str, limit: int = 5
) -> Tuple[List[str], bool]:
    out: List[str] = []
    lines = _read_text_lines(rom_path)
    start_label_re = re.compile(rf"^CODE:[0-9A-Fa-f]{{8}}\s+{re.escape(rom_function_name)}:\s")
    start_idx: Optional[int] = None
    end_idx: Optional[int] = None
    for i, line in enumerate(lines):
        if start_label_re.match(line):
            start_idx = i
            break
    if start_idx is None:
        raise RuntimeError(f"Could not find ROM function '{rom_function_name}' in rom.lst")
    for i in range(start_idx + 1, len(lines)):
        if f"End of function {rom_function_name}" in lines[i]:
            end_idx = i
            break
    if end_idx is None:
        raise RuntimeError(f"Could not find end marker for ROM function '{rom_function_name}' in rom.lst")

    crossed_end = False
    i = start_idx
    max_scan = min(len(lines), start_idx + 300)
    while i < max_scan and len(out) < limit:
        if i > end_idx:
            crossed_end = True
        m = ROM_LINE_RE.match(lines[i])
        if not m:
            i += 1
            continue
        rhs = m.group(2)
        if ":" in rhs:
            i += 1
            continue
        t = rhs.strip()
        if not t or t.startswith(";") or t.startswith("."):
            i += 1
            continue
        tok = t.split()[0].lower()
        if re.match(r"^[a-z][a-z0-9_]*$", tok):
            out.append(tok)
        i += 1

    return out, crossed_end


def extract_rom_instruction_stream(
    rom_path: pathlib.Path, rom_function_name: str, limit: int
) -> Tuple[List[Tuple[str, str]], bool]:
    out: List[Tuple[str, str]] = []
    lines = _read_text_lines(rom_path)
    start_label_re = re.compile(rf"^CODE:[0-9A-Fa-f]{{8}}\s+{re.escape(rom_function_name)}:\s")
    start_idx: Optional[int] = None
    end_idx: Optional[int] = None
    for i, line in enumerate(lines):
        if start_label_re.match(line):
            start_idx = i
            break
    if start_idx is None:
        raise RuntimeError(f"Could not find ROM function '{rom_function_name}' in rom.lst")
    for i in range(start_idx + 1, len(lines)):
        if f"End of function {rom_function_name}" in lines[i]:
            end_idx = i
            break
    if end_idx is None:
        raise RuntimeError(f"Could not find end marker for ROM function '{rom_function_name}' in rom.lst")

    crossed_end = False
    i = start_idx
    max_scan = min(len(lines), start_idx + 2000)
    while i < max_scan and len(out) < limit:
        if i > end_idx:
            crossed_end = True
        m = ROM_LINE_RE.match(lines[i])
        if not m:
            i += 1
            continue
        addr = m.group(1)
        rhs = m.group(2)
        if ":" in rhs:
            i += 1
            continue
        t = rhs.strip()
        if not t or t.startswith(";") or t.startswith("."):
            i += 1
            continue
        tok = t.split()[0].lower()
        if re.match(r"^[a-z][a-z0-9_]*$", tok):
            out.append((addr, tok))
        i += 1
    return out, crossed_end


def split_optional_label(code: str) -> Tuple[Optional[str], str]:
    s = code.strip()
    if not s:
        return None, s
    m = re.match(r"^([A-Za-z_.$?][A-Za-z0-9_.$?]*):\s*(.*)$", s)
    if m:
        return m.group(1), m.group(2).strip()

    # TI assembler rule: label field is in column 1.
    if not code[:1].isspace():
        toks = parse_line_tokens(s)
        if len(toks) >= 1 and re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", toks[0]):
            if len(toks) == 1:
                return toks[0], ""
            return toks[0], " ".join(toks[1:])
    return None, s


def extract_source_label_mapping(
    source_lines: List[str],
    macros: Dict[str, MacroDef],
    symbols: Dict[str, int],
    recursion_limit: int = 12,
) -> Tuple[Dict[int, List[str]], int]:
    labels_by_instr: Dict[int, List[str]] = {}
    instr_count = 0
    pending_labels: List[str] = []
    expander = MacroExpander(macros, symbols)

    def add_pending_to_index(idx: int) -> None:
        if not pending_labels:
            return
        labels_by_instr.setdefault(idx, [])
        for l in pending_labels:
            if l.startswith("."):
                continue
            if l not in labels_by_instr[idx]:
                labels_by_instr[idx].append(l)
        pending_labels.clear()

    def walk_line(code: str, depth: int, active: Set[str]) -> None:
        nonlocal instr_count
        if depth > recursion_limit:
            return
        code = strip_comment(code)
        if not code.strip():
            return
        if code.strip().startswith("*"):
            return

        label, rest = split_optional_label(code)
        if label and not (rest == "" and label.upper() in macros):
            if label.startswith("."):
                label = None
            else:
                pending_labels.append(label)
            code = rest
            if not code:
                return
        elif label and rest == "" and label.upper() in macros:
            code = label

        op_idx, toks = split_label_and_tokens(code)
        if not toks:
            return
        if (
            len(toks) == 1
            and re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", toks[0])
            and toks[0].upper() not in macros
            and not code[:1].isspace()
        ):
            if not toks[0].startswith("."):
                pending_labels.append(toks[0])
            return
        op = toks[op_idx].upper()
        if op.endswith(":") or op.startswith("."):
            return

        if op in macros:
            if op in active:
                return
            active.add(op)
            args = expander.parse_invocation_args(op_idx, toks)
            expanded = expander.expand(op, args)
            for b in iter_active_lines(expanded, symbols):
                walk_line(b, depth + 1, active)
            active.remove(op)
            return

        add_pending_to_index(instr_count)
        instr_count += 1

    for ln in iter_active_lines(source_lines, symbols):
        walk_line(ln, 0, set())

    return labels_by_instr, instr_count


def apply_label_annotations_to_rom(
    rom_path: pathlib.Path,
    annotations: List[Tuple[str, str]],
) -> int:
    lines = rom_path.read_text(errors="ignore").splitlines()
    by_addr: Dict[str, List[str]] = {}
    auto_idx: Dict[str, int] = {}

    def normalize_label(label: str) -> str:
        if label.endswith("?"):
            base = label[:-1]
            auto_idx[base] = auto_idx.get(base, 0) + 1
            return f"{base}{auto_idx[base]}"
        return label

    for label, addr in annotations:
        if label.startswith("."):
            continue
        label = normalize_label(label)
        by_addr.setdefault(addr.upper(), [])
        if label not in by_addr[addr.upper()]:
            by_addr[addr.upper()].append(label)

    inserted = 0
    out: List[str] = []
    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r"^(CODE:([0-9A-Fa-f]{8}))\b", line)
        if m:
            addr = m.group(2).upper()
            labels = by_addr.get(addr, [])
            if labels:
                next_line = lines[i + 1] if i + 1 < len(lines) else ""
                for lab in labels:
                    label_line = f"{m.group(1)} {lab}:"
                    if label_line != line and label_line != next_line:
                        out.append(label_line)
                        inserted += 1
                by_addr.pop(addr, None)
        out.append(line)
        i += 1

    rom_path.write_text("\n".join(out) + "\n")
    return inserted


def mnemonics_equivalent(src: List[str], rom: List[str]) -> bool:
    three_operand_aliases = {
        "addc",
        "addi",
        "addf",
        "and",
        "andn",
        "ash",
        "lsh",
        "subf",
        "mpyf",
        "mpyi",
        "mpyshi",
        "mpyui",
        "or",
        "subb",
        "subi",
        "tstb",
        "xor",
    }

    def canon(m: str) -> str:
        x = m.lower()
        direct = {
            "ret": "retsu",
            "rets": "retsu",
            "reti": "retiu",
            "retsz": "retse",
            "retsnz": "retsne",
            "b": "bu",
            "br": "bu",
            "bp": "bgt",
            "db": "dbu",
            "bd": "bud",
            "brd": "bud",
            "beq": "be",
            "bc": "blo",
            "bnc": "bhs",
            "bn": "blt",
            "bnn": "bge",
            "bnnd": "bged",
            "bnz": "bne",
            "bnzd": "bned",
            "bz": "be",
            "bzd": "bed",
            "callnz": "callne",
            "calleq": "calle",
            "bnd": "bltd",
            "retseq": "retse",
            "stik": "sti",
            "inc": "addi",
            "dec": "subi",
            # LDP is documented as a special/alternate form of LDI* for page loads.
            "ldp": "ldiu",
            # Conditional immediate pseudo-op alias.
            "ldin": "ldilt",
            "ldiz": "ldie",
            "ldinz": "ldine",
            # Return-condition aliases seen between source/ROM listings.
            "retsc": "retslo",
            "retsnc": "retshs",
            "retsn": "retslt",
            # Float-condition aliases seen in source vs ROM mnemonics.
            "ldfeq": "ldfe",
            "ldfz": "ldfe",
            "ldfnz": "ldfne",
            "ldfn": "ldflt",
            "ldfnn": "ldfge",
        }
        x = direct.get(x, x)
        # Condition-code aliases from C3x docs: Z==E, NZ==NE, C==LO, NC==HS, N==LT, NN==GE.
        # Apply generically for call*/ldi* conditional mnemonics.
        for pfx in ("call", "ldi", "db"):
            if x.startswith(pfx) and len(x) > len(pfx):
                suf = x[len(pfx) :]
                suf_alias = {
                    "eq": "e",
                    "z": "e",
                    "nz": "ne",
                    "c": "lo",
                    "nc": "hs",
                    "n": "lt",
                    "nn": "ge",
                }.get(suf)
                if suf_alias is not None:
                    x = pfx + suf_alias
                break
        if x.endswith("3") and x[:-1] in three_operand_aliases:
            x = x[:-1]
        if x == "cmpi3":
            x = "cmpi"
        return x

    s = [canon(x) for x in src]
    r = [canon(x) for x in rom]
    return len(s) == len(r) and s == r


def extract_rom_calls_for_function(rom_path: pathlib.Path, rom_function_name: str) -> List[Tuple[str, str]]:
    lines = extract_rom_function_lines(rom_path, rom_function_name)

    out: List[Tuple[str, str]] = []
    for line in lines:
        m = ROM_LINE_RE.match(line)
        if not m:
            continue
        addr = m.group(1)
        rhs = m.group(2)
        cm = ROM_CALL_ANY_RE.search(rhs)
        if cm:
            target = cm.group(2)
            out.append((addr, target))

    return out


def analyze_one_function(
    root: pathlib.Path,
    rom: pathlib.Path,
    source_function: str,
    rom_function_override: Optional[str],
    macros: Dict[str, MacroDef],
    symbols: Dict[str, int],
    do_mnemonic_check: bool,
    annotate_labels: bool,
    apply_label_annotations: bool,
) -> Tuple[List[str], List[Tuple[str, str]]]:
    rom_function = resolve_rom_function_name(source_function, rom_function_override)
    module_path, start_idx = find_source_module_for_function(root, source_function)
    source_lines = extract_source_function_lines(module_path, source_function, start_idx)
    src_calls = extract_calls_from_lines(source_lines, macros, symbols)
    rom_calls = extract_rom_calls_for_function(rom, rom_function)

    print(f"Source function: {source_function} (module: {module_path.name})")
    print(f"ROM function: {rom_function}")
    print(f"Source calls (including macro-expanded calls): {len(src_calls)}")
    print(f"ROM calls: {len(rom_calls)}")
    unresolved = [(a, r) for a, r in rom_calls if not re.match(r"^[A-Za-z_.$?][A-Za-z0-9_.$?]*$", r)]
    if unresolved:
        print(f"ROM unresolved/non-symbol call targets: {len(unresolved)}")
        for a, r in unresolved:
            print(f"  CODE:{a} -> {r}")
    if len(src_calls) != len(rom_calls):
        raise RuntimeError(
            f"Call-count mismatch for {source_function}: source={len(src_calls)} rom={len(rom_calls)}"
        )
    print()
    print("Idx  SourceCall                      ROM@Addr   ROMCall")
    print("---  -----------------------------  ---------  -----------------------------")

    max_n = max(len(src_calls), len(rom_calls))
    for i in range(max_n):
        s = src_calls[i] if i < len(src_calls) else ""
        if i < len(rom_calls):
            a, r = rom_calls[i]
        else:
            a, r = "", ""
        marker = " " if (s and r) else "!"
        print(f"{i+1:>3}{marker}  {s:<29}  {a:<9}  {r}")

    print()
    print("Legend: '!' marks missing side at this index (count mismatch or drift).")
    if do_mnemonic_check:
        print()
        print("Discovery mnemonic checks (first 5 mnemonics):")
        any_check = False
        for s, _, r in [
            (
                src_calls[i] if i < len(src_calls) else "",
                rom_calls[i][0] if i < len(rom_calls) else "",
                rom_calls[i][1] if i < len(rom_calls) else "",
            )
            for i in range(max_n)
        ]:
            if not s or not r.startswith("sub_"):
                continue
            try:
                src_mod, src_start = find_source_module_for_function(root, s)
                src_lines = extract_source_function_lines(src_mod, s, src_start)
                src_m = extract_mnemonics_from_lines(
                    src_lines, macros, symbols, limit=5, expand_macros=True
                )
                rom_m, crossed_end = extract_rom_mnemonics(rom, r, limit=len(src_m))
                ok = mnemonics_equivalent(src_m, rom_m)
                status = "OK" if ok else "MISMATCH"
                warn = " [WARNING: crossed IDA function end]" if crossed_end else ""
                print(f"  {s} -> {r}: {status}{warn}  src={src_m} rom={rom_m}")
                any_check = True
            except Exception as ex:
                print(f"  {s} -> {r}: ERROR ({ex})")
                any_check = True
        if not any_check:
            print("  (no sub_ discoveries in this function mapping)")

    if annotate_labels:
        print()
        print("Source label annotation proposals:")
        labels_by_instr, src_instr_count = extract_source_label_mapping(source_lines, macros, symbols)
        rom_stream, crossed = extract_rom_instruction_stream(rom, rom_function, limit=src_instr_count)
        ann: List[Tuple[str, str]] = []
        auto_idx: Dict[str, int] = {}

        def normalize_label(label: str) -> str:
            if label.endswith("?"):
                base = label[:-1]
                auto_idx[base] = auto_idx.get(base, 0) + 1
                return f"{base}{auto_idx[base]}"
            return label

        for idx, labels in sorted(labels_by_instr.items()):
            if idx >= len(rom_stream):
                continue
            addr = rom_stream[idx][0]
            for lab in labels:
                if lab.startswith("."):
                    continue
                lab = normalize_label(lab)
                ann.append((lab, addr))
        for lab, addr in ann:
            print(f"  {lab} -> CODE:{addr}")
        if crossed:
            print("  [WARNING: crossed IDA function end while collecting ROM instruction stream]")
        if apply_label_annotations and ann:
            inserted = apply_label_annotations_to_rom(rom, ann)
            print(f"  Applied {inserted} label line(s) to {rom}")

    return src_calls, rom_calls


def fingerprint_check(
    root: pathlib.Path,
    rom: pathlib.Path,
    source_function: str,
    rom_function: str,
    macros: Dict[str, MacroDef],
    symbols: Dict[str, int],
) -> None:
    src_mod, src_start = find_source_module_for_function(root, source_function)
    src_lines = extract_source_function_lines(src_mod, source_function, src_start)
    src_m = extract_mnemonics_from_lines(src_lines, macros, symbols, limit=5, expand_macros=True)
    rom_m, crossed_end = extract_rom_mnemonics(rom, rom_function, limit=len(src_m))
    ok = mnemonics_equivalent(src_m, rom_m)
    status = "OK" if ok else "MISMATCH"
    warn = " [WARNING: crossed IDA function end]" if crossed_end else ""

    print(f"Source function: {source_function} (module: {src_mod.name})")
    print(f"ROM function: {rom_function}")
    print("Instruction fingerprint check (first 5 mnemonics, macro-expanded source):")
    print(f"  {status}{warn}")
    print(f"  src={src_m}")
    print(f"  rom={rom_m}")


def main() -> None:
    ap = argparse.ArgumentParser(description="Match source function call order with rom.lst calls")
    ap.add_argument("function", nargs="?", default=None, help="Legacy positional source function name")
    ap.add_argument("--start-function", default=None, help="Starting source function (default: cint)")
    ap.add_argument("--depth", type=int, default=0, help="Recursion depth from start function (default: 0)")
    ap.add_argument("--root", default=".", help="Project root (default: current dir)")
    ap.add_argument("--rom", default="roms/rom.lst", help="Path to rom listing relative to root")
    ap.add_argument("--rom-function", default=None, help="Override ROM function label name")
    ap.add_argument("--mnemonic-check", action="store_true", help="Check first 5 mnemonics for new sub_ discoveries")
    ap.add_argument("--annotate-labels", action="store_true", help="Propose source-label to ROM-address annotations by stepping instruction streams")
    ap.add_argument("--apply-label-annotations", action="store_true", help="Apply proposed labels directly into rom.lst")
    args = ap.parse_args()

    root = pathlib.Path(args.root).resolve()
    rom = root / args.rom
    start_name = args.start_function or args.function or "cint"
    source_function = resolve_source_function_name(start_name)
    macros = parse_macros(root)
    symbols = parse_set_symbols(root)
    symbols["MEMTESTS"] = symbols.get("MEMTESTS", 1)
    q: deque[Tuple[str, int, Optional[str]]] = deque()
    q.append((source_function, 0, args.rom_function))
    visited: Set[str] = set()

    while q:
        func, level, rom_override = q.popleft()
        if func in visited:
            continue
        visited.add(func)
        if level > 0:
            print()
            print("=" * 78)
            print(f"Depth {level}: {func}")
            print("=" * 78)
        if level == 0:
            try:
                src_calls, rom_calls = analyze_one_function(
                    root,
                    rom,
                    func,
                    rom_override,
                    macros,
                    symbols,
                args.mnemonic_check,
                args.annotate_labels,
                args.apply_label_annotations,
            )
            except Exception as ex:
                print(f"ERROR analyzing {func}: {ex}")
                raise SystemExit(1)
        else:
            try:
                # Recursive callee checks validate mapped function identity by instructions,
                # not by comparing each callee's outgoing call graph.
                target = rom_override or resolve_rom_function_name(func, None)
                fingerprint_check(root, rom, func, target, macros, symbols)
                src_mod, src_start = find_source_module_for_function(root, func)
                src_lines = extract_source_function_lines(src_mod, func, src_start)
                src_calls = extract_calls_from_lines(src_lines, macros, symbols)
                rom_calls = extract_rom_calls_for_function(rom, target)
                if len(src_calls) != len(rom_calls):
                    raise RuntimeError(
                        f"Call-count mismatch for {func}: source={len(src_calls)} rom={len(rom_calls)}"
                    )
            except Exception as ex:
                print(f"ERROR analyzing {func}: {ex}")
                raise SystemExit(1)

        if level >= args.depth:
            continue

        max_n = min(len(src_calls), len(rom_calls))
        for i in range(max_n):
            child = src_calls[i]
            child_rom = rom_calls[i][1]
            if not child:
                continue
            try:
                find_source_module_for_function(root, child)
            except Exception:
                continue
            if child not in visited:
                q.append((child, level + 1, child_rom))


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
import re
from pathlib import Path

repo = Path('/Users/j.harris/code/cruisin-usa')
out_root = repo / 'port-sdl' / 'src' / 'scaffold'

exclude_parts = {'.git', 'port-sdl', 'build'}

label_re = re.compile(r'^([A-Za-z_][A-Za-z0-9_]*)\s*:\s*(?:;.*)?$')

# Heuristics to skip obvious loop/local/data labels.
skip_prefixes = (
    'L', 'LP', 'LOOP', 'CNT', 'NO', 'END', 'WT', 'REG', 'DOD', 'NOD', 'CHK', 'ERR',
)

# Explicit keep-list for common entry-style names even if short.
force_keep = {
    '_c_int00', 'INT0', 'MESSAGE1', 'MSG1', 'MSG2', 'MSG3', 'MAINLOOP', 'COLD_ENTER',
}

asm_files = []
for p in repo.rglob('*.ASM'):
    if any(part in exclude_parts for part in p.parts):
        continue
    asm_files.append(p)

for asm in sorted(asm_files):
    rel = asm.relative_to(repo)
    out = out_root / rel.with_suffix('.c')
    out.parent.mkdir(parents=True, exist_ok=True)

    text = asm.read_text(errors='ignore').splitlines()

    labels = []
    seen = set()
    for line in text:
        m = label_re.match(line)
        if not m:
            continue
        name = m.group(1)
        if name in seen:
            continue

        if name.startswith('.'):
            continue
        if name.upper() in {'IF', 'ELSE', 'ENDIF'}:
            continue

        is_forced = name in force_keep
        if not is_forced:
            up = name.upper()
            if len(name) <= 3 and not name.startswith('_'):
                continue
            if any(up.startswith(prefix) for prefix in skip_prefixes):
                continue

        seen.add(name)
        labels.append(name)

    with out.open('w') as f:
        f.write('#include "../../NOT_IMPLEMENTED.h"\n\n')
        f.write(f'/* Scaffold generated from {rel.as_posix()} */\n\n')

        if not labels:
            module_fn = rel.stem.replace('-', '_') + '_MODULE_STUB'
            f.write(f'void {module_fn}(void) {{\n')
            f.write('    NOT_IMPLEMENTED();\n')
            f.write('}\n')
            continue

        for name in labels:
            f.write(f'void {name}(void) {{\n')
            f.write('    NOT_IMPLEMENTED();\n')
            f.write('}\n\n')

print(f"Generated scaffolds for {len(asm_files)} ASM modules under {out_root}")

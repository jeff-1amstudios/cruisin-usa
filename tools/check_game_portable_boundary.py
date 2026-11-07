"""Reject host dependencies in game code and its transitive project headers."""
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
SEARCH = [ROOT / 'src', ROOT / 'src/portable', ROOT / 'src/platform', ROOT / 'src/game']
INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"\n]+)[>"]', re.MULTILINE)


def check(path, chain, seen, errors):
    path = path.resolve()
    if path in seen:
        return
    seen.add(path)
    source = path.read_text()
    for include in INCLUDE.findall(source):
        trail = chain + [str(path.relative_to(ROOT)), include]
        if include.startswith(('SDL', 'emscripten/')) or include == 'emscripten.h':
            errors.add(' -> '.join(trail))
        resolved = next((p.resolve() for p in [path.parent / include] +
                         [folder / include for folder in SEARCH] if p.is_file()), None)
        if resolved:
            if ROOT / 'src/platform' in resolved.parents:
                errors.add(' -> '.join(trail))
            else:
                check(resolved, trail[:-1], seen, errors)
    if ROOT / 'src/game' in path.parents and '__EMSCRIPTEN__' in source:
        errors.add(f'{path.relative_to(ROOT)}: use CRUISN_PORTABLE for game divergences')


def main():
    seen, errors = set(), set()
    for path in sorted((ROOT / 'src/game').glob('*')):
        if path.suffix in ('.c', '.h'):
            check(path, [], seen, errors)
    if errors:
        print('\n'.join(sorted(errors)), file=sys.stderr)
        return 1
    print(f'Portable boundary verified ({len(seen)} game files and reachable project headers)')
    return 0


if __name__ == '__main__':
    sys.exit(main())

# Cruisin USA arcade port decomp
We are translating the original TMS320C30 asm source code (in asm dir) for this game into portable c (in src dir), using SDL.

## Function translation
Follow docs/function-translation.md for guidance on how to translate a function

## Debugging
We can validate behavior through MAME instrumentation. Read docs/debugging.md

## TMS320c3 docs
See docs/tms320 folder

## Portable and platform boundary
Game code in `src/game` must depend only on interfaces in `src/portable` for
host services. Do not include headers from `src/platform`, SDL, or Emscripten
in game code, including through transitive headers. Host-specific backends
implement the portable interfaces in `src/platform`. Guard any required
portable divergence in `src/game` with `CRUSIN_PORTABLE`.
Run `cmake --build <build-directory> --target check-game-portable-boundary`
after changing this boundary.

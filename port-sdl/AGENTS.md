# Cruis'n USA C Port Plan (SDL2)

## Scope of Current Milestone

Port the original startup path faithfully up to and including the first text screen, and continue startup sequencing without inventing behavior. Only platform boundaries (video/input/sound/hardware I/O) are stubbed.

## Current Status (Implemented)

### File/Module Naming Convention

We are using strict module naming parity:

- `X.ASM -> X.c`
- optional `X.h` only when needed for declarations.
- Functions in X.ASM file must be placed/implemented in X.c file

Build files:

- `CMakeLists.txt` (SDL2 build)

## Ground Rules (Agreed)

- Faithful logic/order to original code.
- Only platform/hardware boundaries may be stubbed.
- No invented waits/timing/flow changes.
- Keep module names aligned with original `.ASM` files.

## How to Build

```bash
cd /Users/j.harris/code/cruisin-usa/port-sdl
cmake -S . -B build
cmake --build build -j4
```

## Current Translation Policy (Locked)

- Implemented functions must be exact translations of original ASM behavior.
- Only platform-touching behavior may be substituted, and must be explicitly named/documented as platform shims.
- Non-translated game logic functions should use `NOT_IMPLEMENTED()` and abort, so the next required function is discovered by execution trace.

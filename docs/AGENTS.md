# C Port Plan

## Goals

- Port the original TMS320C3x codebase to portable C with hooks to compare behavior against MAME.
- Build a cross-platform host runtime around SDL, including a memory-backed 2D screen model.
- Automated translation of asm to C


## Repository Shape

Proposed directory layout:

- `asm`: Original assembly source code. Do not edit
- `src/game`: Translation of the .asm files
- `src/core`: Helper functionality for portability and MAME validation
- `src/platform`: video, audio, networking subsystems

## Translation

See docs/function-translation.md

## SDL Runtime

The SDL layer should wrap a memory-backed screen, not drive rendering directly from gameplay code.

### Video design

Keep a software framebuffer in emulated memory:

- canonical internal surface owned by the machine model
- explicit width, height, stride, pixel format
- gameplay/rendering routines write into this memory
- SDL uploads that memory to a texture each frame

Recommended first target:

- 16-bit or 32-bit internal pixel format
- one conversion/upload path in SDL
- optional debug overlay for watchpoints, dirty rects, and trace info

This preserves the original "draw into memory" behavior and makes MAME comparison easier.

### Input design

SDL input should write into emulated input registers/state buffers, not call gameplay code directly.

### Audio design

Stub first unless a specific subsystem is being ported.

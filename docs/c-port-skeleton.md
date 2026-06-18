# C Port Plan

## Goals

- Port the original TMS320C3x codebase to portable C in a way that stays easy to compare against MAME.
- Preserve a close source-to-source relationship with the original ASM so stepping, tracing, and symbol lookup stay practical.
- Keep each translated C file as the single working file for that module. Exception: if an existing generated `.c` or `.h` starts with `// NOEDIT`, regeneration writes the fresh skeleton to `src/generated/` instead of overwriting the hand-edited file.
- Build a cross-platform host runtime around SDL, including a memory-backed 2D screen model.
- Automated translation of asm to C

## Non-Goals

- Do not optimize for idiomatic modern C early.
- Do not rewrite control flow for readability before behavior is verified.

## Repository Shape

Proposed directory layout:

```text
asm/
  ATTRACTA.ASM
  ...
  SYS.EQU
  ...

src/
  core/
    machine.h
    machine.c
    memory.h
    memory.c
    trace.h
    trace.c
    cpu_model.h
    cpu_model.c
  platform/
    sdl_main.c
    sdl_video.c
    sdl_audio.c
    input.c
  game/
    attracta.c
    attrdrne.c
    ...
    globals.h
    sys.h
    obj.h
    ...
  tools/
    generated/
      module_index.json
      translation_report.json

tools/
  port/
    gen_c_skeleton.py
    sync_module_layout.py
    compare_trace.py
```

Notes:

- Move original `.ASM`, `.EQU`, and `.INC` files into `asm/`.
- Keep generated C output in its final destination under `src/game/`.
- Keep generated headers in `src/game/`.
- If a checked-in `.c` or `.h` begins with `// NOEDIT`, emit the regenerated version to `src/generated/` with the same basename.
- Keep generator metadata out of the source tree so regeneration does not force source layout changes.

## Translation Format

Initial generated function format:

1. Original labels remain as C labels.
2. Original asm instructions become verbatim `// asm:` comments copied exactly from the asm source text.
3. Each instruction line is emitted as its own single-line `// asm:` comment.
4. Do not bundle instruction streams into block comments.
5. Each asm instruction initially lowers to either:
   - a placeholder helper call, or
   - a directly translated C statement when the lowering is mechanical and safe.
6. Branches lower to `goto`.
7. Delay-slot behavior must be expressed explicitly in the lowered code order.

Example:

```c
void Example(void)
{
L100:
    // asm:  LDI	@VALUE,R0
    R0 = mem_rd_s32(sym_VALUE);

    // asm:  CMPI	0,R0
    cc = c3x_cmp_i32(R0, 0);

    // asm:  BEQD	L200
    delay_branch = BR_EQ(L200, cc);

    // asm:  STI	R0,@OUT
    mem_wr_s32(sym_OUT, R0);

    if (delay_branch.taken) goto L200;
L200:
    return;
}
```

Do not generate pretty C first. Generate traceable C first.

## Header Conversion Rules

### `.EQU` files

Convert `.EQU` files into generated headers under `src/game/`.

Rules:

- `.set` becomes `#define` or `enum` depending on usage.
- Preserve source order where practical.
- Preserve comments.
- Keep assembler aliases as aliases.
- Sanitize names only when required for valid C.
- Generate one header per source file, not one giant constants header.

### `.INC` files

Treat include files based on content:

- constant/macro-oriented `.INC` files become headers
- data-table includes may remain binary/data assets if that fits better
- assembler-only macro includes should become either:
  - C macros, or
  - helper inline functions, or
  - generator-side expansion rules

## Machine Model

This has to be defined before broad translation starts.

### Types

Use fixed-width types everywhere:

- `int32_t`, `uint32_t`, `uint16_t`, `float`

`types.h` should eventually become a portable layout header built on `<stdint.h>`.

### Addressing

The original code is word-oriented. Host code must not blur byte and word addresses.

Define:

- word address type
- byte address type where needed
- explicit conversion helpers

Example:

```c
typedef uint32_t word_addr_t;
typedef uint32_t byte_addr_t;
```

### Memory spaces

Model memory spaces explicitly:

- ROM
- RAM
- fast/shared/scratch regions as needed
- memory-mapped IO
- framebuffer memory

Do not replace symbolic addresses with raw host pointers in translated code. Use helpers so reads/writes can be traced and compared.

### Registers and machine state

Even if the final code becomes less CPU-shaped, the validation phase benefits from a machine-state layer that can represent:

- integer registers
- floating-point registers if needed
- condition flags/branch state
- DP-like addressing context
- process scheduler state

Not every translated function must literally operate on a full CPU struct forever, but the compare/debug build should be able to.

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

## Verification Strategy

The critical requirement is incremental comparison against MAME.

### Symbol alignment

- Keep using the current symbol generation work.
- Produce a symbol file that both MAME tools and the C port can use.
- Preserve original module/function names where known.

### Trace points

Need a common trace format for:

- function entry/exit
- selected label hits
- memory reads/writes
- branch decisions
- important global state snapshots

Recommended format:

- newline-delimited JSON, or
- TSV if simpler and lower overhead

### Comparison modes

1. Function-level:
   compare inputs/outputs for selected routines.
2. Basic-block or label-level:
   compare branch path and key writes.
3. Frame-level:
   compare framebuffer hashes, selected object tables, and major globals.

### Golden test cases

Build deterministic scenarios:

- boot sequence checkpoints
- attract mode frame N
- menu transitions
- one car on one track with fixed inputs
- deterministic math/geometry micro-tests

## Translation Order

Do not start with the hardest live systems first.

Suggested order:

1. shared machine/types/memory layer
2. `.EQU`/`.INC` header generation
3. C skeleton generator
4. trace emitter and MAME trace comparison
5. small deterministic routines
6. math helpers
7. utility/data walking code
8. 2D rendering helpers and framebuffer path
9. scheduler/process code
10. larger gameplay modules

Candidate early pilot modules:

- `LINE`
- `HPMATH`
- `TEXTTAB`
- narrowly scoped parts of `FONT`
- utility routines from `MATH` or `UTIL`

Avoid early full-port attempts for:

- `DIRQ`
- `MPROC`
- `OBJECTS`
- top-level game loop and interrupt-heavy code

## Build Modes

Have at least two build modes from the start:

### Match/Debug

- strict floating-point behavior
- heavy asserts
- trace enabled
- conservative optimization
- optional CPU-shaped execution helpers

### Native/Clean

- fewer traces
- higher optimization
- cleaner platform integration

Do not try to use one build configuration for both goals.

## Coding Rules For Ported Modules

- One C source file per original ASM module.
- Function names stay as close to recovered symbols as possible.
- Labels remain visible in the function body.
- Original asm instruction line comments are preserved verbatim until the function is considered fully understood.
- Non-instruction source lines such as `.set` and `.include` do not need to be preserved as `// asm:` comments in the C file.
- Recovered intent may be added as normal comments, but the asm breadcrumbs stay.
- No silent replacement of address-based logic with direct pointers.
- No collapsing multiple machine operations into one helper until the behavior is already verified.

## Tooling To Build Next

1. `tools/port/sync_module_layout.py`
   - moves/validates source layout
   - maps `asm/*.ASM` to `src/game/*.c`

2. `tools/port/gen_c_skeleton.py`
   - parses ASM
   - converts `.EQU` files to headers
   - emits or refreshes block-based module C files
   - emits or refreshes generated headers

3. `tools/port/compare_trace.py`
   - compares MAME and native traces

4. MAME trace helpers
   - function/label hit logging
   - memory write capture for selected ranges
   - framebuffer dump/hash support

## Immediate Next Steps

1. Move original source files under `asm/` without changing contents.
2. Define the machine/runtime headers:
   - `machine.h`
   - `memory.h`
   - `trace.h`
3. Implement `.EQU` to header generation.
4. Implement one-module C skeleton generation using block regeneration.
5. Pick one pilot module and make the first end-to-end compare loop work in MAME and native C.

## Decisions Already Locked In

- Cross-platform C, not a platform-specific compiler resurrection path.
- SDL for host runtime.
- Memory-backed 2D screen emulation.
- One working C file per original ASM module.
- Original asm lines remain embedded as comments during the translation phase.
- Validation against MAME is a first-class requirement, not a cleanup task for later.

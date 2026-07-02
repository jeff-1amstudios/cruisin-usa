# Cruisin USA arcade port debugging

We are translating the original TMS320C30 asm source code into portable C. At this stage the main task is usually to debug behavior in translated `.c` code against the original arcade implementation.

Validation exists so we can compare the port's behavior against values produced by MAME running the original game. The validator lives in `src/core/validator`.

You can build and run with `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy SDL_RENDER_DRIVER=software`. When validation fails, you will usually see output like `/Users/j.harris/code/cruisin-usa/src/game/text.c:692: validation(R0) failed; expected 0x00000050 but was 0x00000059`.

## What MAME validation is for

The validation flow is used to find the first point where the portable C port diverges from the original arcade code.

It helps answer questions like:
- Did this translated function receive the same inputs as the original code?
- Did a register, local value, or memory write change unexpectedly?
- Did control flow diverge earlier than expected?
- Is the current validation call consuming the wrong expected MAME log entry because execution order changed?

This is especially useful when translating a function from asm to C or when fixing a translated function that is close to correct but still produces wrong state later in execution.

## Validation flow

The system has two sides: a MAME-side trace generator and a port-side validator.

1. Add validation macros in the translated C code.
2. Run `tools/mame/generate_mame_validate_breakpoints.py` to scan those macros and generate a MAME debugger script.
3. Run MAME with that debugger script against the original game.
4. MAME hits the generated breakpoints and writes `logerror` lines, and sometimes binary dumps, describing the original code's state at those points.
5. Save that output in `mame_validate/mame.log`.
6. Run the SDL port. `src/core/validator.c` opens `mame_validate/mame.log` and consumes entries in order as the port reaches each validation call.
7. If the current port value does not match the next expected MAME value, the validator reports the mismatch and usually aborts immediately.

The key point is that the validator is not recomputing anything itself. It is replaying a previously captured trace from the original game and checking that the port reaches the same observable state in the same order.

## How breakpoints are generated

`tools/mame/generate_mame_validate_breakpoints.py` scans the C sources for validation macros and turns them into debugger breakpoints.

It uses two main ways to place those breakpoints:
- Function-entry validations such as `MAME_VALIDATE_ARG(...)`, `MAME_VALIDATE_ARG_FLOAT(...)`, `MAME_VALIDATE_FUNCTION_ENTRY()`, and `MAME_VALIDATE_EXIT()` use the containing C function name and look up its original address in `tools/ida/address.map`.
- Explicit-address validations such as `MAME_VALIDATE_REGION_AT_ADDR(...)`, `MAME_VALIDATE_REG_AT_ADDR(...)`, and `MAME_VALIDATE_REG_AT_ADDR_FLOAT(...)` use the address passed in the macro call.

The generated MAME script emits lines such as:
```text
function SET_SOMETHING
validate R0: 0x000000D5, text.c:692
validate R1: 0x00000002, text.c:699
function STRLEN
```

The `function ...` lines are trace markers for readability. They make the MAME log easier to scan when several validations share function-entry breakpoints.

For larger validations, the generator may emit `save` commands instead of printing all words inline. In that case the log references a dumped file and the validator compares against that file-backed data.

## Validation macros

Common validation markers:
- `MAME_VALIDATE_ARG(name, ptr)`: validate an integer register argument at original function entry.
- `MAME_VALIDATE_ARG_FLOAT(name, ptr)`: validate a float register argument at original function entry.
- `MAME_VALIDATE_FUNCTION_ENTRY()`: emit a `function <NAME>` marker at the original function entry in the MAME log.
- `MAME_VALIDATE_EXIT()`: emit an exit breakpoint when you want MAME to stop at that function entry.
- `MAME_VALIDATE_REGION_AT_ADDR(...)`: validate a memory region at an explicit original instruction address.
- `MAME_VALIDATE_REG_AT_ADDR(...)`: validate an integer register at an explicit original instruction address.
- `MAME_VALIDATE_REG_AT_ADDR_FLOAT(...)`: validate a float register at an explicit original instruction address.

## What the runtime validator checks

`src/core/validator.c` reads `mame_validate/mame.log` and consumes one validation entry at a time.

It checks:
- The next expected entry has the right kind of data for the current validation call.
- The expected value matches the current value produced by the SDL port.
- The writer context recorded in the MAME log matches the C callsite consuming it. This helps catch cases where execution order changed and a validation line is being consumed by the wrong place.

On failure, the validator reports:
- The current C file and line.
- The MAME log line number.
- The expected value from MAME.
- The actual value produced by the port.

By default, it aborts on the first error.

## When to refresh the MAME log

You need a fresh MAME run whenever you add, remove, reorder, or materially change `mame_validate_*` callsites.

That is because the validator consumes `mame_validate/mame.log` in runtime order. If the callsites change, the old log may no longer line up with the port's validation sequence even if the underlying gameplay code is unchanged.

## Manual hand over

You can add extra debugging `printf` lines to trace the C code, but if you need to add or edit a `mame_validate(...)` call, you'll have to stop and ask me to re-run MAME to generate a new `mame.log` to run against.

I'll now give you more context on the specific bug.

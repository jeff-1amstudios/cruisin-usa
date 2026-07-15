# Cruisin USA arcade port debugging

We are translating the original TMS320C30 asm source code into portable C. At this stage the main task is usually to debug behavior in translated `.c` code against the original arcade implementation.

Validation exists so we can compare the port's behavior against values produced by MAME running the original game. The validator lives in `src/core/validator`.

When validation fails, you will usually see output like `/Users/j.harris/code/cruisin-usa/src/game/text.c:692: validation(R0) failed; expected 0x00000050 but was 0x00000059`.

## What MAME validation is for

The validation flow is used to find the first point where the portable C port diverges from the original arcade code.

It helps answer questions like:
- Did this translated function receive the same inputs as the original code?
- Did a register, local value, or memory write change unexpectedly?
- Did control flow diverge earlier than expected?
- Is the current validation call consuming the wrong expected MAME log entry because execution order changed?

This is especially useful when translating a function from asm to C or when fixing a translated function that is close to correct but still produces wrong state later in execution.

## How to

1. This works best for validating scalar register and memory values
2. You cannot compare pointer addresses - addresses are not comparable between MAME and our port

## Memory reading
- You must use hex offsets when adding to registers - example `MAME_ASSERT_MEM(0x000020A9, "d@(ar2+f)", &road_obj->id);`

## Validation flow

The system has two sides: a MAME-side trace generator and a port-side validator.

1. Fix code / add validation macros in the translated C code.
2. Run `tools/debug.sh`.
  -  `tools/mame/output/mame_validate_breakpoints.txt` is updated and MAME runs with that debugger script
  - Our port is build and run. `src/core/validator.c` opens `mame_validate/mame.log` and consumes entries in order as the port reaches each validation call.
  - If the current port value does not match the next expected MAME value, the validator reports the mismatch and usually aborts immediately.

The key point is that the validator is not recomputing anything itself. It is replaying a previously captured trace from the original game and checking that the port reaches the same observable state in the same order.

## How breakpoints are generated

`tools/mame/generate_mame_validate_breakpoints.py` scans the C sources for validation macros and turns them into debugger breakpoints.

It uses two main ways to place those breakpoints:
- Function-entry validations such as `MAME_ASSERT_ARG(...)`, `MAME_ASSERT_ARG_FLOAT(...)`, `MAME_ASSERT_FUNCTION_ENTRY()`, and `MAME_VALIDATOR_EXIT()` use the containing C function name and look up its original address in `tools/ida/address.map`.
- Explicit-address validations such as `MAME_ASSERT_REGION_AT_ADDR(...)`, `MAME_ASSERT_REG(...)`, `MAME_ASSERT_REG_FLOAT(...)`, and `MAME_ASSERT_MEM(...)` use the address passed in the macro call.

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

Canonical validation markers:
- `MAME_ASSERT_ARG(name, ptr)`: validate an integer register argument at original function entry.
- `MAME_ASSERT_ARG_FLOAT(name, ptr)`: validate a float register argument at original function entry.
- `mame_validate_arg_sym(name, ptr)`: validate a symbol-valued argument at original function entry.
- `MAME_ASSERT_FUNCTION_ENTRY()`: emit a `function <NAME>` marker at the original function entry in the MAME log.
- `MAME_VALIDATOR_EXIT()`: emit an exit breakpoint when you want MAME to stop at that function entry.
- `MAME_ASSERT_REGION_AT_ADDR(...)`: validate a memory region at an explicit original instruction address.
- `MAME_ASSERT_REG(...)`: validate an integer register at an explicit original instruction address.
- `MAME_ASSERT_REG_FLOAT(...)`: validate a float register at an explicit original instruction address.
- `MAME_ASSERT_MEM(...)`: validate an arbitrary MAME memory expression at an explicit original instruction address.

The generator still accepts some older `mame_validate_*` and `MAME_VALIDATE_*` spellings, but new code should use the names above.

## Usage

- When using the explicit-address forms, pick the _next_ address after the register or memory value you are looking at has been set. The breakpoint fires _before_ the attached instruction is executed.

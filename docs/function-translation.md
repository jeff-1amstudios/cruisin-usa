# Per function assembly translation

Translate the specified assembly function into c. Where practical, keep the c code simple so its close to line for line with the assembly for ease of debugging. However, you may write the c code in idiomatic style for ease of readability.

## Rules
- - *Interleave* the translated c pieces of code _underneath_ each relevant // asm: comment line. *THIS IS NOT OPTIONAL*. It is WRONG if you do not follow this.
- Read the associated .ASM file for context
- First, determine the function prototype - args and return value. Use developer comments along with the code to help understand and name the arguments.
- Correctness is your number 1 job. You must not invent anything except translating the assembly code into C.
- You must NEVER re-order or remove the commented asm lines.
- Preserve function boundaries exactly. Do not move work across caller/callee boundaries, even if the resulting C would look cleaner.
- Do not precompute a callee's result in the caller, do not move helper calls from callee to caller (or vice versa), and do not "inline by hand" behavior across function boundaries.
- You can introduce new function variables (declare at top of function)
- Do not preserve assembly register names as C locals by default.
- Only introduce a local variable when it represents real intermediate state needed for correctness, readability, or control flow. If an instruction sequence only reads, transforms, and writes a single C variable, collapse it to direct operations on that variable instead of inventing `r0`, `r1`, etc.
- Register-shaped locals like `r0`, `r1`, `ar2` should be rare, and only used when the value must be preserved separately from the source variable or when it materially clarifies the dataflow.
- Dont invent new functions, other that to avoid large duplication (eg if 2 functions are branching to the same code tail)
- Dont invent new global/module level variables
- Ignore DP and CPU wait state related instructions. Ignore push/pop.
- Retain the original developer comments in the assembly code as c comments
- If a translated function call in asm produces a value or side effect used by the caller, keep that call in the translated function at the same boundary. Do not replace it by passing a transformed value unless the original asm does that transformation before the call.
- Choose a C3X conversion from the assembly instruction that produces the value, not from whether the C expression looks like an integer or a float. Follow the table in [C3X floating-point translation](#c3x-floating-point-translation).

If you get stuck, stop, and explain the problem. Don't start inventing things in order to keep progressing.

## C3X floating-point translation

| Assembly source | C translation |
|---|---|
| `LDF 0.25,R0` | `value = C3X_IMM_F32(0.25);` |
| `ADDF 0.25,R0` | `value = C3X_ADD(value, C3X_IMM_F32(0.25));` |
| `SUBF 0.25,R0` | `value = C3X_SUB(value, C3X_IMM_F32(0.25));` |
| `MPYF 0.25,R0` | `value = C3X_MUL(value, C3X_IMM_F32(0.25));` |
| `CMPF 0.25,R0` | Compare against `C3X_IMM_F32(0.25)` |
| `LDF @VALUE,R0`, where `VALUE` is `.float` data | `value = C3X_LDF(VALUE);` |
| `FLOAT R0,R1` | `value = C3X_FROM_INT(integer_value);` |
| `STF R0,*AR0` | `destination = C3X_STF(value);` |
| Static `.float` data | `static const c3x_f32_t VALUE = C3X_F32_INIT(readable_value);` |
| A runtime high-precision algorithm constant | `C3X_F32(value)` |

All floating instruction immediates use `C3X_IMM_F32`, including:

- integer-looking operands such as `LDF 1,R0`;
- symbolic constants such as `LDF PI,R0`;
- conditional forms such as `LDFGT 0.5,R0`;
- reverse and three-operand forms.

Do not translate a floating instruction immediate using a raw C literal, `C3X_F32`, or `C3X_FROM_INT`. `C3X_FROM_INT` is only for an actual `FLOAT` instruction or an equivalent integer-to-floating conversion. `C3X_F32` is not the default literal wrapper; use it only when the source genuinely requires full C3X register precision rather than the instruction-immediate encoding.

Use `c3x_f32_t` for static `.float` data and `c3x_reg_t` for values held at
register precision. Convert memory data with `C3X_LDF` before using it in
floating-point arithmetic. Keeping the types distinct makes arithmetic on an
unloaded table or constant a compile-time error.

`C3X_STF` represents the precision change caused by storing a register through the C30 single-memory format. Use it on the value assigned by every translated `STF`; do not spell it as `C3X_LOAD(C3X_STORE(...))`.

Run `cmake --build <build-directory> --target check-c3x-translation` after translating floating-point code.

## Types
We put our types in types.h. If the asm function for example uses AR2 as an input, pointing to 2 ints, you should create a type
```typedef struct <FUNCTIONNAME>_ARG {
    int a;
    int b;
} <FUNCTIONNAME>_ARG
```
and prototype the function as FUNCTIONNAME(FUNCTIONNAME_ARG x /*AR2*/) { .. }

## Examples
- The `STRLEN` function in `text.c` is a great example to follow
- Prefer collapsing simple read/modify/write register sequences into direct C updates of the real variable, for example:

```c
OFFROAD_TMR -= 1;
if (OFFROAD_TMR < 0) {
    OFFROAD_TMR = 0;
}
```

instead of:

```c
r0 = OFFROAD_TMR;
r0 -= 1;
if (r0 < 0) {
    r0 = 0;
}
OFFROAD_TMR = r0;
```

## Emulated hardware
`core/input.h` and `core/output.h` are what we should use to replace direct reads/writte of MMIO memory addresses like `@SWITCH1`, `@COMMPAL`, `@FIFO_ADDR`

## `PROC` functions
Functions which take a `PROC *` argument are executed as primitive co-routines. Follow [PROC translation](proc-translation.md) for resume-stack, `SLEEP`, `JSRP`, cross-function branch, shared-tail, and re-entry rules. All local variables needed across a possible sleep must be persisted in a function-specific `PROC_CONTEXT` field.

## Stub only files
For sound, motion and network play modules, when you encounter their functions, prototype them correctly but dont implement. Return dummy values where needed. Mark them as UNIMPL_TODO().

## Verification
After implementing a significant function, if possible, add a mame assertion to catch any drift from the original code. See `debugging.md`.

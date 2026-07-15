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

If you get stuck, stop, and explain the problem. Don't start inventing things in order to keep progressing.

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
Functions which take a `PROC *` argument are a executed as primitive co-routines. All local variables should be persisted in a function-specific `PROC_CONTEXT` field, and special care must be taken to make them correctly re-entrant. See `REQWAIT` in `comp.c` for an example of how to structure it.


## Verification
After implementing a significant function, if possible, add a mame assertion to catch any drift from the original code. See `debugging.md`.

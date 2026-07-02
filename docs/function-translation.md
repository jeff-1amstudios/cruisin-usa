# Per function assembly translation

Translate the specified assembly function into c. Where practical, keep the c code simple so its close to line for line with the assembly for ease of debugging. However, you may write the c code in idiomatic style for ease of readability.

## Rules
- Read the associated .ASM file for context
- First, determine the function prototype - args and return value. Use developer comments along with the code to help understand and name the arguments.
- Correctness is your number 1 job. You must not invent anything except translating the assembly code into C.
- You can introduce new function variables (declare at top of function)
- Dont invent new functions, other that to avoid large duplication (eg if 2 functions are branching to the same code tail)
- Dont invent new global/module level variables
- Ignore DP and CPU wait state related instructions. Ignore push/pop.
- Write your c code underneath the // asm: comment lines already in the function.
- Retain the original developer comments in the assembly code as c comments

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

<!-- ## Validation
1. Add a mame_validate_word("var_name", &var, 0xORIGINAL_ADDRESS) macro call after each store/set in C code. Dont worry that we havent defined the macro implementation yet. -->

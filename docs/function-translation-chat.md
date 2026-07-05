# Per function assembly translation

Translate the specified assembly function into c. Where practical, keep the c code simple so its close to line for line with the assembly for ease of debugging. However, you may write the c code in idiomatic style for ease of readability.

## Rules
- First, determine the function prototype - args and return value. Use developer comments along with the code to help understand and name the arguments.
- Correctness is your number 1 job. You must not invent anything except translating the assembly code into C.
- You can introduce new function variables (declare at top of function)
- Do not preserve assembly register names as C locals by default.
- Only introduce a local variable when it represents real intermediate state needed for correctness, readability, or control flow.
- If an instruction sequence only reads, transforms, and writes a single C variable, collapse it to direct operations on that variable instead of inventing `r0`, `r1`, etc.
- Register-shaped locals like `r0`, `r1`, `ar2` should be rare, and only used when the value must be preserved separately from the source variable or when it materially clarifies the dataflow.
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
The `STRLEN` function is a great example to follow:
```
static int STRLEN(const tTEXT* text /*AR2*/) {
    FONTENTRY* font;
    const char* str;
    int length;
    int ch;
    int char_width;

    // asm 00007A4B: 	PUSH	RS
    // asm 00007A4C: 	PUSH	R1
    // asm 00007A4D: 	PUSH	R2
    // asm 00007A4E: 	PUSH	AR0
    // asm 00007A4F: 	PUSH	AR1
    // asm 00007A50: 	PUSH	AR2
    // asm 00007A51: 	CLRI	R0			;length of string
    // asm 00007A52: 	CLRI	RS
    str = text->ptr;
    length = 0; // ;length of string
STRLP:
    ch = (unsigned char)*str++;

    // asm 00007A5B: 	CMPI	0,AR0
    // asm 00007A5C: 	BZ	STRLENX
    if (ch == 0) {
        goto STRLENX;
    }

    // asm 00007A5D: 	CMPI	' ',AR0
    // asm 00007A5E: 	BEQ	STRLENNCHAR
    if (ch == ' ') {
        goto STRLENNCHAR;
    }

    // asm 00007A5F: 	SUBI	'0',AR0     ;THE START OF THE FONT
    ch -= '0'; // ;THE START OF THE FONT

    // asm 00007A60: 	LDI	AR0,AR1
    // asm 00007A61: 	MPYI	FONTENT_SIZE,AR1
    // asm 00007A62: 	ADDI	*+AR4(TEXT_ADDR),AR1
    font = &text->text_addr[ch];

    // asm 00007A63: 	LDI	*+AR1(FONTENT_XEND),R1
    // asm 00007A64: 	SUBI	*+AR1(FONTENT_XSTART),R1
    char_width = (int)font->x_end - (int)font->x_start;

    // asm 00007A65: 	LDI	*+AR1(FONTENT_PRE),R2
    // asm 00007A66: 	LS	16,R2
    // asm 00007A67: 	ASH	-16,R2			;MUST SIGN EXTEND THIS DUDE
    // asm 00007A68: 	ADDI	R2,R1
    char_width += TEXT_FONT_PRE(font);

    // asm 00007A69: 	LDI	*+AR1(FONTENT_TRAIL),R2
    // asm 00007A6A: 	RS	16,R2
    // asm 00007A6B: 	ADDI	R2,R1
    char_width += TEXT_FONT_TRAIL(font);

STRLENNCHAR:
    // asm 00007A6C: 	ADDI	R1,R0			;INCREASE STRING LENGTH
    length += char_width; // ;INCREASE STRING LENGTH
    // asm 00007A6D: 	BU	STRLP
    goto STRLP;
STRLENX:
    // asm 00007A6E: 	POP	AR2
    // asm 00007A6F: 	POP	AR1
    // asm 00007A70: 	POP	AR0
    // asm 00007A71: 	POP	R2
    // asm 00007A72: 	POP	R1
    // asm 00007A73: 	POP	RS
    // asm 00007A74: 	RETS
    return length;
}
```
Prefer collapsing simple read/modify/write register sequences into direct C updates of the real variable, for example:

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


## `PROC` functions
Functions which take a `PROC *` argument are a executed as primitive co-routines. All local variables should be persisted in a function-specific `PROC_CONTEXT` field, and special care must be taken to make them correctly re-entrant. See `REQWAIT` in `comp.c` for an example of how to structure it.
Resume labels should be called `PROC_STATE_<N>` to make them obviously different from the original code labels.

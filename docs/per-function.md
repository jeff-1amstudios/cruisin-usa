# Per function plan

Translate the commented "// asm:" lines to c.

## Before starting

Collect all variables read or written to in the target function. Discard variables that are not defined in a .c file. The final list should be written at the top of the function like:

`VALIDATE_VAR_<TYPE>("var_name", &var, 0xORIGINAL_ADDRESS, ENTRY);`

- `0xORIGINAL_ADDRESS` is found in `ida/tools/address.map`.
- `TYPE` is the variable type as declared in c.

A corresponding `VALIDATE_VAR_<TYPE>("var_name", &var, 0xORIGINAL_ADDRESS, EXIT);` list should written to the bottom of the function.

## Rules

- Don't invent function arguments, everything is passed through registers for now
- Ignore DP and CPU wait state related instructions
- If you find an instruction writing directly to memory, look up what that memory address refers to in vunit.h. If you cannot figure it out, stop. Do not assume.

## Example translation

```
// asm: 	LDI	02h,IOF			;TV30 KLUDGE
// asm: 	CLRI	R0			;must fix COMM setup asap
R0 = 0;
// asm: 	LDP	@COMMINTM
// asm: 	STI	R0,@COMMINTM
COMMINTM = R0;
// asm: 	LDP	@FIFO_CONTROL
// asm: 	LDI	FIFO_CONTROL_INIT,R0
R0 = FIFO_CONTROL_INIT;
// asm: 	STI	R0,@FIFO_CONTROL
FIFO_CONTROL = R0;
// asm:     TEXTIT	startup0,1,40
TEXTIT(startup0, 1, 40);
// asm:     CALL	READIO
READIO();
// asm:     LDI	240,R0			;X MIN
R0 = 240;
// asm:     STI	R0,@PREVX
PREVX = RO;
```

## Validation

1. Generate a mame .lua script that dumps the variables in the list at function entry and exit into two separate files. We know the mame memory address of all vars in tools/ida/address.map At function exit, mame should exit.
2. Run mame, wait for exit
3. Run our port. `VALIDATE_VAR_...` will abort and log to stdout if any variable is not correct

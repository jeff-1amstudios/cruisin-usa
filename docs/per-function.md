# Per function plan

Translate the commented "// asm:" lines to c. Each translated portion of C code should be placed under the asm line(s) that it translated from

## Rules
- Correctness is your number 1 job. You must not invent anything except translating the assembly code into C.
- Each translated C statement must be placed immediately after the `// asm:` line or lines it translates. Do not collect translated code later in the function or below unrelated asm comments.
- Don't invent function arguments, everything is passed through registers for now
- Dont invent functions
- Ignore DP and CPU wait state related instructions


## Validation
1. Add a mame_validate_word("var_name", &var, 0xORIGINAL_ADDRESS) macro call after each store/set in C code. Dont worry that we havent defined the macro implementation yet.
<!-- 1. Generate a mame debugger script that dumps the variables in the list at function entry and exit into two separate files. We know the mame memory address of all vars in tools/ida/address.map At function exit, mame should exit.
2. Run mame, wait for exit
3. Run our port. `VALIDATE_VAR_...` will abort and log to stdout if any variable is not correct -->

## Example translation

```
// asm: 	LDI	02h,IOF			;TV30 KLUDGE
// asm: 	CLRI	R0			;must fix COMM setup asap
R0 = 0;
// asm: 	LDP	@COMMINTM
// asm: 	STI	R0,@COMMINTM
COMMINTM = R0;
VALIDATE_WORD("COMMINTM", &COMMINTM);
// asm: 	LDP	@FIFO_CONTROL
// asm: 	LDI	FIFO_CONTROL_INIT,R0
R0 = FIFO_CONTROL_INIT;
// asm: 	STI	R0,@FIFO_CONTROL
FIFO_CONTROL = R0;
VALIDATE_WORD("FIFO_CONTROL", &FIFO_CONTROL);
// asm:     TEXTIT	startup0,1,40
TEXTIT(startup0, 1, 40);
// asm:     CALL	READIO
READIO();
// asm:     LDI	240,R0			;X MIN
R0 = 240;
// asm:     STI	R0,@PREVX
PREVX = RO;
VALIDATE_WORD("PREVX", &PREVX);
```

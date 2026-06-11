#include "../../core/cpu.h"
#include "../../core/machine.h"

/*
 * Source module: asm/HPMATH.ASM
 */

// ******************************************************
// *    TMS320C30 C COMPILER     Version 4.50
// ******************************************************
// ;	C:\BIN\ac30.exe -mr -ic:\c30tools -x -q hpmath.c e:\hpmath.if
// ;	C:\BIN\opt30.exe -m -r -a -q -O2 e:\hpmath.if e:\hpmath.opt
// ;	C:\BIN\cg30.exe -o -n -b -m -p -a -c -q e:\hpmath.opt hpmath.asm e:\hpmath.tmp
#define FP AR3
// ******************************************************
// * FUNCTION DEF : _HPsin
// ******************************************************
// ******************************************************
// * DEFINE CONSTANTS                                   *
// ******************************************************
// ******************************************************
// * UNDEFINED REFERENCES                               *
// ******************************************************

void _HPsin(void)
{
    // asm: 	PUSH	R4
    // *
    // * R2	assigned to parameter x
    // * R3	assigned to variable  sgn
    // * R4	assigned to variable  xn
    // *
    // asm: 	CMPF	0,R2
    // asm: 	LDFLT	-1.0,R3
    // asm: 	LDFGE	1.0,R3
    // asm: 	ABSF	R2
    // asm: 	LDP	@CONST+0
    // asm: 	LDF	@CONST+0,R0
    // asm: 	MPYF	R0,R2,R1
    // asm: 	ADDF	5.0e-1,R1
    // asm: 	FIX	R1,RC
    // asm: 	FLOAT	RC,R4
    // asm: 	LDI	RC,R0
    // asm: 	LDI	2,R1
    // asm: 	CALL	MOD_I30
    // asm: 	BZ	LL3
    // asm: 	NEGF	R3
LL3:
    // asm: 	LDF	3.140625,R0
    // asm: 	MPYF	R0,R4,R1
    // asm: 	SUBF	R1,R2,R1
    // asm: 	LDP	@CONST+1
    // asm: 	MPYF	@CONST+1,R4
    // asm: 	SUBF	R4,R1,R4
    // asm: 	MPYF	R4,R4,R2
    // asm: 	LDP	@CONST+2
    // asm: 	LDF	@CONST+2,R1
    // asm: 	MPYF	R1,R2,R0
    // asm: 	LDP	@CONST+3
    // asm: 	ADDF	@CONST+3,R0
    // asm: 	MPYF	R2,R0
    // asm: 	LDP	@CONST+4
    // asm: 	ADDF	@CONST+4,R0
    // asm: 	MPYF	R2,R0
    // asm: 	LDP	@CONST+5
    // asm: 	ADDF	@CONST+5,R0
    // asm: 	MPYF	R2,R0
    // asm: 	MPYF	R4,R0
    // asm: 	ADDF	R4,R0
    // asm: 	MPYF	R3,R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_HPsin", 0, 0);
    UNIMPL();
}

void _HPcos(void)
{
    // ******************************************************
    // * FUNCTION DEF : _HPcos
    // ******************************************************
    // asm: 	PUSH	R4
    // *
    // * R2	assigned to variable  xn
    // * R3	assigned to variable  x
    // * R4	assigned to variable  sgn
    // *
    // asm: 	ABSF	R2,R3
    // asm: 	LDP	@CONST+6
    // asm: 	LDF	@CONST+6,R0
    // asm: 	ADDF	R0,R3,R1
    // asm: 	LDP	@CONST+0
    // asm: 	MPYF	@CONST+0,R1
    // asm: 	LDF	5.0e-1,R2
    // asm: 	ADDF	R2,R1
    // asm: 	FIX	R1,RC
    // asm: 	FLOAT	RC,R1
    // asm: 	SUBF	R2,R1,R2
    // asm: 	LDI	RC,R0
    // asm: 	LDI	2,R1
    // asm: 	CALL	MOD_I30
    // asm: 	LDFNZ	-1.0,R4
    // asm: 	LDFZ	1.0,R4
    // asm: 	LDF	3.140625,R0
    // asm: 	MPYF	R0,R2,R1
    // asm: 	SUBF	R1,R3
    // asm: 	LDP	@CONST+1
    // asm: 	MPYF	@CONST+1,R2
    // asm: 	SUBF	R2,R3
    // asm: 	MPYF	R3,R3,R2
    // asm: 	LDP	@CONST+2
    // asm: 	LDF	@CONST+2,R1
    // asm: 	MPYF	R1,R2,R0
    // asm: 	LDP	@CONST+3
    // asm: 	ADDF	@CONST+3,R0
    // asm: 	MPYF	R2,R0
    // asm: 	LDP	@CONST+4
    // asm: 	ADDF	@CONST+4,R0
    // asm: 	MPYF	R2,R0
    // asm: 	LDP	@CONST+5
    // asm: 	ADDF	@CONST+5,R0
    // asm: 	MPYF	R2,R0
    // asm: 	MPYF	R3,R0
    // asm: 	ADDF	R3,R0
    // asm: 	MPYF	R4,R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_HPcos", 0, 0);
    UNIMPL();
}

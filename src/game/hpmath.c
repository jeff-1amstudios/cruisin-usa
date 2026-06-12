#include "../core/cpu.h"
#include "../core/machine.h"

/*
 * Source module: asm/HPMATH.ASM
 */

void _HPsin(void);
void EPI0_1(void);
void _HPcos(void);
void EPI0_2(void);

#define FP AR3
/* asm: CONST: */
/* asm: 	.float	3.1830988618379067154e-1;0 */
/* asm: 	.float	9.67653589793e-4 ;1 */
/* asm: 	.float	2.601903036e-6   ;2 */
/* asm: 	.float	-1.980741872e-4  ;3 */
/* asm: 	.float	8.333025139e-3   ;4 */
/* asm: 	.float	-1.666665668e-1  ;5 */
/* asm: 	.float	1.57079632679489661923;6 */
float CONST[] = {
    3.1830988618379067154e-1f, // 0
    9.67653589793e-4f, // 1
    2.601903036e-6f, // 2
    -1.980741872e-4f, // 3
    8.333025139e-3f, // 4
    -1.666665668e-1f, // 5
    1.57079632679489661923f, // 6
};

/* ******************************************************
* FUNCTION DEF : _HPsin
******************************************************
 */
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

void EPI0_1(void)
{
    // asm: 	POP	R4
    // asm: 	RETS
    // ******************************************************
    // * FUNCTION DEF : _HPcos
    // ******************************************************
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_1", 0, 0);
    UNIMPL();
}

void _HPcos(void)
{
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

void EPI0_2(void)
{
    // asm: 	POP	R4
    // asm: 	RETS
    // ******************************************************
    // * DEFINE CONSTANTS                                   *
    // ******************************************************
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_2", 0, 0);
    UNIMPL();
}

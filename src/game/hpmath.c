
#include "../core/machine.h"

/*
 * Source module: asm/HPMATH.ASM
 */

void _HPsin(void);
static void EPI0_1(void);
void _HPcos(void);
static void EPI0_2(void);

void MOD_I30(void);

static float CONST[7];

#define FP AR3

/*
******************************************************
* FUNCTION DEF : _HPsin
******************************************************
*/
void _HPsin(void)
{
    // asm 0000B04A: 	PUSH	R4
    // *
    // * R2	assigned to parameter x
    // * R3	assigned to variable  sgn
    // * R4	assigned to variable  xn
    // *
    // asm 0000B04B: 	CMPF	0,R2
    // asm 0000B04C: 	LDFLT	-1.0,R3
    // asm 0000B04D: 	LDFGE	1.0,R3
    // asm 0000B04E: 	ABSF	R2
    // asm 0000B04F: 	LDP	@CONST+0
    // asm 0000B050: 	LDF	@CONST+0,R0
    // asm 0000B051: 	MPYF	R0,R2,R1
    // asm 0000B052: 	ADDF	5.0e-1,R1
    // asm 0000B053: 	FIX	R1,RC
    // asm 0000B054: 	FLOAT	RC,R4
    // asm 0000B055: 	LDI	RC,R0
    // asm 0000B056: 	LDI	2,R1
    // asm 0000B057: 	CALL	MOD_I30
    // asm 0000B058: 	BZ	LL3
    // asm 0000B059: 	NEGF	R3
LL3:
    // asm 0000B05A: 	LDF	3.140625,R0
    // asm 0000B05B: 	MPYF	R0,R4,R1
    // asm 0000B05C: 	SUBF	R1,R2,R1
    // asm 0000B05D: 	LDP	@CONST+1
    // asm 0000B05E: 	MPYF	@CONST+1,R4
    // asm 0000B05F: 	SUBF	R4,R1,R4
    // asm 0000B060: 	MPYF	R4,R4,R2
    // asm 0000B061: 	LDP	@CONST+2
    // asm 0000B062: 	LDF	@CONST+2,R1
    // asm 0000B063: 	MPYF	R1,R2,R0
    // asm 0000B064: 	LDP	@CONST+3
    // asm 0000B065: 	ADDF	@CONST+3,R0
    // asm 0000B066: 	MPYF	R2,R0
    // asm 0000B067: 	LDP	@CONST+4
    // asm 0000B068: 	ADDF	@CONST+4,R0
    // asm 0000B069: 	MPYF	R2,R0
    // asm 0000B06A: 	LDP	@CONST+5
    // asm 0000B06B: 	ADDF	@CONST+5,R0
    // asm 0000B06C: 	MPYF	R2,R0
    // asm 0000B06D: 	MPYF	R4,R0
    // asm 0000B06E: 	ADDF	R4,R0
    // asm 0000B06F: 	MPYF	R3,R0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_HPsin", 0, 0);
    UNIMPL();
}

static void EPI0_1(void)
{
    // asm 0000B070: 	POP	R4
    // asm 0000B071: 	RETS
    // ******************************************************
    // * FUNCTION DEF : _HPcos
    // ******************************************************
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_1", 0, 0);
    UNIMPL();
}

void _HPcos(void)
{
    // asm 0000B072: 	PUSH	R4
    // *
    // * R2	assigned to variable  xn
    // * R3	assigned to variable  x
    // * R4	assigned to variable  sgn
    // *
    // asm 0000B073: 	ABSF	R2,R3
    // asm 0000B074: 	LDP	@CONST+6
    // asm 0000B075: 	LDF	@CONST+6,R0
    // asm 0000B076: 	ADDF	R0,R3,R1
    // asm 0000B077: 	LDP	@CONST+0
    // asm 0000B078: 	MPYF	@CONST+0,R1
    // asm 0000B079: 	LDF	5.0e-1,R2
    // asm 0000B07A: 	ADDF	R2,R1
    // asm 0000B07B: 	FIX	R1,RC
    // asm 0000B07C: 	FLOAT	RC,R1
    // asm 0000B07D: 	SUBF	R2,R1,R2
    // asm 0000B07E: 	LDI	RC,R0
    // asm 0000B07F: 	LDI	2,R1
    // asm 0000B080: 	CALL	MOD_I30
    // asm 0000B081: 	LDFNZ	-1.0,R4
    // asm 0000B082: 	LDFZ	1.0,R4
    // asm 0000B083: 	LDF	3.140625,R0
    // asm 0000B084: 	MPYF	R0,R2,R1
    // asm 0000B085: 	SUBF	R1,R3
    // asm 0000B086: 	LDP	@CONST+1
    // asm 0000B087: 	MPYF	@CONST+1,R2
    // asm 0000B088: 	SUBF	R2,R3
    // asm 0000B089: 	MPYF	R3,R3,R2
    // asm 0000B08A: 	LDP	@CONST+2
    // asm 0000B08B: 	LDF	@CONST+2,R1
    // asm 0000B08C: 	MPYF	R1,R2,R0
    // asm 0000B08D: 	LDP	@CONST+3
    // asm 0000B08E: 	ADDF	@CONST+3,R0
    // asm 0000B08F: 	MPYF	R2,R0
    // asm 0000B090: 	LDP	@CONST+4
    // asm 0000B091: 	ADDF	@CONST+4,R0
    // asm 0000B092: 	MPYF	R2,R0
    // asm 0000B093: 	LDP	@CONST+5
    // asm 0000B094: 	ADDF	@CONST+5,R0
    // asm 0000B095: 	MPYF	R2,R0
    // asm 0000B096: 	MPYF	R3,R0
    // asm 0000B097: 	ADDF	R3,R0
    // asm 0000B098: 	MPYF	R4,R0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_HPcos", 0, 0);
    UNIMPL();
}

static void EPI0_2(void)
{
    // asm 0000B099: 	POP	R4
    // asm 0000B09A: 	RETS
    // ******************************************************
    // * DEFINE CONSTANTS                                   *
    // ******************************************************
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_2", 0, 0);
    UNIMPL();
}

/* asm: CONST: */
/* asm: 	.float	3.1830988618379067154e-1;0 */
/* asm: 	.float	9.67653589793e-4 ;1 */
/* asm: 	.float	2.601903036e-6   ;2 */
/* asm: 	.float	-1.980741872e-4  ;3 */
/* asm: 	.float	8.333025139e-3   ;4 */
/* asm: 	.float	-1.666665668e-1  ;5 */
/* asm: 	.float	1.57079632679489661923;6 */
static float CONST[] = {
    3.1830988618379067154e-1f, // 0
    9.67653589793e-4f,         // 1
    2.601903036e-6f,           // 2
    -1.980741872e-4f,          // 3
    8.333025139e-3f,           // 4
    -1.666665668e-1f,          // 5
    1.57079632679489661923f,   // 6
    // *****************************************************
    // UNDEFINED REFERENCES                               *
    // *****************************************************
};

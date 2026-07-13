
#include "effects.h"
#include "../core/machine.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "sysid.h"

/*
 * Source module: asm/EFFECTS.ASM
 */

void SCREENWIPE_OPEN(void);
void SCREENWIPE_CLOSE(void);
void SCREENWIPE_OPEN_PRC_FAST(void);
void SCREENWIPE_OPEN_PRC(void);
void SCREENWIPE_CLOSE_PRC(void);
static void GET_EFFECTS_OBJS(void);
static void MEMCPY(void);

#define MONKEYOBJRVI mnkrv
#define MONKEYOBJLVI mnklv
#define MONKEYOBJUHI mnkuh
#define MONKEYOBJLHI mnklh

/*
 *----------------------------------------------------------------------------
 *ADVANCED FEATURES
 *
 *it would be best to do open and close via the same process
 *also, sideways wipes  //  or \\  up, down, left or right can be specified
 *via on word ala:
 *
 *	SCW_OPEN	.set	0
 *	SCW_CLOSE	.set	1
 *	SCW_L		.set	10h	;these specifiy the
 *	SCW_R		.set	20h	;single panels
 *	SCW_T		.set	40h	;
 *	SCW_B		.set	80h	;
 *
 *for convience:
 *	SCW_OPEN_H	.set	030h
 *	SCW_CLOSE_H	.set	031h
 *	SCW_OPEN_V	.set	0C0h
 *	SCW_CLOSE_V	.set	0C1h
 *
 *also can be added:	(S = straight instead of diagonal)
 *
 *	SCW_LS		.set	12h	;these specifiy the
 *	SCW_RS		.set	22h	;single panels
 *	SCW_TS		.set	42h	;
 *	SCW_BS		.set	82h	;
 *
 *
 *these parameters would be encoded into AR2 when calling SCREEN_WIPE
 *and from there it would be moved to AR6, and checked each tik.
 *
 */

/* asm: MONKEYOBJRVI	.word	mnkrv */
#define MONKEYOBJRVI mnkrv
/* asm: MONKEYOBJLVI	.word	mnklv */
#define MONKEYOBJLVI mnklv
/* asm: MONKEYOBJUHI	.word	mnkuh */
#define MONKEYOBJUHI mnkuh
/* asm: MONKEYOBJLHI	.word	mnklh */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
#define MONKEYOBJLHI mnklh
/* asm: SCREENWIPE_DONE	.bss	SCREENWIPE_DONE,1 */
int SCREENWIPE_DONE;

// *----------------------------------------------------------------------------
void SCREENWIPE_OPEN(void) {
    // asm 0000B09F: 	CLRI	R0
    // asm 0000B0A0: 	STPI	R0,@SCREENWIPE_DONE
    // asm 0000B0A1: 	CREATEC	SCREENWIPE_OPEN_PRC,UTIL_C|MONKEY_T
    // asm 0000B0A4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_OPEN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SCREENWIPE_CLOSE(void) {
    // asm 0000B0A5: 	CLRI	R0
    // asm 0000B0A6: 	STPI	R0,@SCREENWIPE_DONE
    // asm 0000B0A7: 	CREATE	SCREENWIPE_CLOSE_PRC,UTIL_C|MONKEY_T
    // asm 0000B0AA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_CLOSE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SCREENWIPE_OPEN_PRC_FAST(void) {
    // asm 0000B0AB: 	LONGROUT
    // asm: 	LDF	1.9,R0
    // asm: 	BR	L55
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_OPEN_PRC_FAST", 0, 0);
    UNIMPL();
}

void SCREENWIPE_OPEN_PRC(void) {
    // asm 0000B0AC: 	LONGROUT
    // asm: 	LDF	1.1,R0
L55:
    // asm: STF	R0,*+AR7(PDATA+7)
    // asm: 	CALL	GET_EFFECTS_OBJS
    // asm: 	SLEEP	19
    // asm: 	LDI	64,AR5
    // asm: 	CLRF	R6
    // asm: 	LDF	2,R7
    // asm: SWCL1
    // asm: 	MPYF	*+AR7(PDATA+7),R7
    // ;	MPYF	1.1,R7
    // asm: 	ADDF	0.2,R6
    // asm: 	LDF	R6,R2
    // asm: 	LDPI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	LDI	*+AR7(PDATA),AR0
    // asm: 	LDF	*+AR0(OUSR1),R0
    // asm: 	ADDF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR1)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm: 	LDF	*+AR0(OUSR1),R0
    // asm: 	SUBF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR1)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDPI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    // asm: 	LDF	*+AR0(OUSR2),R0
    // asm: 	SUBF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR2)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	LDF	*+AR0(OUSR2),R0
    // asm: 	ADDF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR2)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,SWCL1
    // asm: 	LDI	*+AR7(PDATA),AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	*+AR7(PDATA+1),AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	*+AR7(PDATA+2),AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	*+AR7(PDATA+3),AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@SCREENWIPE_DONE
    // asm: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_OPEN_PRC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SCREENWIPE_CLOSE_PRC(void) {
    // asm 0000B0AD: 	LONGROUT
    // asm: 	CALL	GET_EFFECTS_OBJS
    // asm: 	LDI	100,AR5
    // asm: 	LDF	12.8,R6
    // asm: 	LDLFH	28530.66038,R7
    // asm: SWCL1C
    // asm: 	MPYF	0.91,R7
    // asm: 	SUBF	0.2,R6
    // asm: 	LDF	R6,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	LDI	*+AR7(PDATA),AR0
    // asm: 	STF	R7,*+AR0(OUSR1)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm: 	NEGF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR1)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    // asm: 	NEGF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR2)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R7,*+AR0(OUSR2)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,SWCL1C
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@SCREENWIPE_DONE
    // asm: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_CLOSE_PRC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void GET_EFFECTS_OBJS(void) {
    // asm 0000B0AE: 	LONGROUT
    // asm: 	LDI	@MONKEYOBJRVI,AR1
    // asm: 	CALL	OBJ_GET
    // asm: 	LDI	AR0,AR4
    // asm: 	LDLH	O_3DROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	STI	AR0,*+AR7(PDATA)
    // asm: 	STI	AR1,*+AR4(OROMDATA)
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OUSR2)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OUSR3)
    // asm: 	LDI	@MONKEYOBJLVI,AR1
    // asm: 	CALL	OBJ_GET
    // asm: 	LDI	AR0,AR4
    // asm: 	LDLH	O_3DROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	STI	AR0,*+AR7(PDATA+1)
    // asm: 	STI	AR1,*+AR4(OROMDATA)
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OUSR2)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OUSR3)
    // asm: 	LDI	@MONKEYOBJUHI,AR1
    // asm: 	CALL	OBJ_GET
    // asm: 	LDI	AR0,AR4
    // asm: 	LDLH	O_3DROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	STI	AR0,*+AR7(PDATA+2)
    // asm: 	STI	AR1,*+AR4(OROMDATA)
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OUSR2)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OUSR3)
    // asm: 	LDI	@MONKEYOBJLHI,AR1
    // asm: 	CALL	OBJ_GET
    // asm: 	LDI	AR0,AR4
    // asm: 	LDLH	O_3DROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	STI	AR0,*+AR7(PDATA+3)
    // asm: 	STI	AR1,*+AR4(OROMDATA)
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OUSR2)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OUSR3)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_EFFECTS_OBJS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *MEMCPY
 *
 *PARAMETERS
 *	AR0	SOURCE POINTER
 *	AR1	DEST POINTER
 *	RC	LENGTH -1
 *
 */
static void MEMCPY(void) {
    // asm 0000B0AF: 	LONGROUT
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	R0
    // asm: 	RPTB	MEMCPYL
    // asm: 	LDI	*AR0++,R0
MEMCPYL:
    // asm: STI	R0,*AR1++
    // asm: 	POP	R0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MEMCPY", 0, 0);
    UNIMPL();
}

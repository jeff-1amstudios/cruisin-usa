#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "sysid.h"
#include "globals.h"
#include "objects.h"
#include "effects.h"

/*
 * Source module: asm/EFFECTS.ASM
 */

/* asm: MONKEYOBJRVI	.word	mnkrv */
#define MONKEYOBJRVI mnkrv
/* asm: MONKEYOBJLVI	.word	mnklv */
#define MONKEYOBJLVI mnklv
/* asm: MONKEYOBJUHI	.word	mnkuh */
#define MONKEYOBJUHI mnkuh
/* asm: MONKEYOBJLHI	.word	mnklh */
#define MONKEYOBJLHI mnklh
/* asm: SCREENWIPE_DONE	.bss	SCREENWIPE_DONE,1 */
int SCREENWIPE_DONE;

/* *----------------------------------------------------------------------------
 */
void SCREENWIPE_OPEN(void)
{
    // asm: 	CLRI	R0
    // asm: 	STPI	R0,@SCREENWIPE_DONE
    // asm: 	CREATEC	SCREENWIPE_OPEN_PRC,UTIL_C|MONKEY_T
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_OPEN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SCREENWIPE_CLOSE(void)
{
    // asm: 	CLRI	R0
    // asm: 	STPI	R0,@SCREENWIPE_DONE
    // asm: 	CREATE	SCREENWIPE_CLOSE_PRC,UTIL_C|MONKEY_T
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_CLOSE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SCREENWIPE_OPEN_PRC_FAST(void)
{
    // asm: 	LONGROUT
    // asm: 	LDF	1.9,R0
    // asm: 	BR	L55
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_OPEN_PRC_FAST", 0, 0);
    UNIMPL();
}

void SCREENWIPE_OPEN_PRC(void)
{
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_OPEN_PRC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SCREENWIPE_CLOSE_PRC(void)
{
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREENWIPE_CLOSE_PRC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_EFFECTS_OBJS(void)
{
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_EFFECTS_OBJS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*MEMCPY
*
*PARAMETERS
*	AR0	SOURCE POINTER
*	AR1	DEST POINTER
*	RC	LENGTH -1
*
 */
void MEMCPY(void)
{
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MEMCPY", 0, 0);
    UNIMPL();
}

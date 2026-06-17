#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "vunit.h"
#include "sys.h"
#include "globals.h"
#include "objects.h"
#include "port.h"
#include "leg.h"

/*
 * Source module: asm/LEG.ASM
 */

void LEG_INIT(void);
void ELEMENT_INIT(void);
void ELEMENT_GET(void);
void ELEMENT_ADD(void);
void ELEMENT_DUMP_INTO_LEGMAP(void);
void LEG_GENERATE_MAP(void);
void LEG_ADD_GROUP(void);
void GENERATE_LINEAR_DISTANCE(void);

extern int FINISH_ID;
extern int LEG_ELEMENTS;
extern int LEG_MAP[];
extern int LAST_END_CACHE;
extern int LAST_END_INDEX;
extern int LAST_ORIENTATION;
extern int LEG_SSLL[];
extern int LEGFREE;
extern int LEGLL;
extern int LEG_FLAG;
extern int LEG_RADY;
extern int LEG_SECTIONIDX;

#define USEFIXED 1
#define USEEXTENDED 0
/* asm: FINISH_ID	.bss	FINISH_ID,1 */
int FINISH_ID;
/* asm: LEG_ELEMENTS	.bss	LEG_ELEMENTS,1 */
int LEG_ELEMENTS;
/* asm: LEG_MAP	hibss	LEG_MAP,MAX_LEG_ELEMENTS*LEG_SIZE */
int LEG_MAP[MAX_LEG_ELEMENTS*LEG_SIZE];
/* asm: LAST_END_CACHE	.bss	LAST_END_CACHE,1 */
int LAST_END_CACHE;
/* asm: LAST_END_INDEX	.bss	LAST_END_INDEX,1 */
int LAST_END_INDEX;
/* asm: LAST_ORIENTATION	.bss	LAST_ORIENTATION,1 */
int LAST_ORIENTATION;

/* *----------------------------------------------------------------------------
*
*
*THIS INITIALIZATION IS FOR EACH GAME, NOT
*EACH LEG
*
*
 */
void LEG_INIT(void)
{
    // asm: 	PUSH	R0
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@LEG_ELEMENTS
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@LAST_END_INDEX
    // asm: 	LDI	@TYCO_TRKI,R0
    // asm 0000AA60: 	STI	R0,@LAST_END_CACHE
    // asm 0000AA61: 	POP	R0
    // asm 0000AA62: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEG_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
#define MINILL_SIZE 60
/* asm: LEG_SSLL	hibss	LEG_SSLL,MINILL_SIZE*(LEG_SIZE+1) */
int LEG_SSLL[MINILL_SIZE*(LEG_SIZE+1)];
/* asm: LEGFREE	.bss	LEGFREE,1 */
int LEGFREE;
/* asm: LEGLL	.bss	LEGLL,1 */
int LEGLL;

/* *
*NOT A TRUE LINKED LIST, BECAUSE THERE IS NO DYNAMIC
*DEALLOCATION (TO ALLOW REALLOCATION)  THE SYSTEM IS
*RESET WHEN PIECES ARE REALLOCATED.
*
*THIS IS FOR THE SAKE OF SPEED
*
 */
void ELEMENT_INIT(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDL	LEG_SSLL,R0
    // asm: 	STI	R0,@LEGFREE
    // asm: 	CLRI	R0
    // asm 0000AA69: 	STI	R0,@LEGLL
#if DEBUG
    // asm: 	STI	R0,@DBG_LEGCNT
#endif
    // asm 0000AA6D: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ELEMENT_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*RETURNS
*	AR3	PTR TO ELEMENT
*
 */
void ELEMENT_GET(void)
{
    // asm: 	PUSH	AR0
#if DEBUG
    // asm: 	LDI	@DBG_LEGCNT,AR0		;CHECK TO SEE IF WE ARE ALLOCATING TO MANY
    // asm: 	INC	AR0			;PIECES
    // asm: 	CMPI	MINILL_SIZE,AR0
    // asm 0000AA70: 	BGE	$
    // asm: 	STI	AR0,@DBG_LEGCNT
#endif
    // asm: 	LDI	@LEGFREE,AR3
    // asm: 	LDI	AR3,AR0
    // asm: 	ADDI	LEG_SIZE+1,AR0
    // asm: 	STI	AR0,@LEGFREE
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ELEMENT_GET", 0, 0);
    UNIMPL();
}

void ELEMENT_ADD(void)
{
    // asm: 	LDI	*+AR3(1+LEG_ID),R0
    // asm: 	LDI	@LEGLLI,AR0
R65:
    // asm 0000AA78: LDI	*AR0,AR1
    // asm: 	CMPI	0,AR1
    // asm 0000AA78: 	BEQ	QT
    // asm: 	CMPI	*+AR1(1+LEG_ID),R0
    // asm 0000AA79: 	BLT	QT
    // asm 0000AA7A: 	LDI	AR1,AR0
    // asm 0000AA7B: 	BU	R65
QT:
    // asm 0000AA7D: 	STI	AR1,*AR3
    // asm 0000AA7E: 	STI	AR3,*AR0
    // asm 0000AA80: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ELEMENT_ADD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	AR3	PTR TO LEG MAP ENTRY
*
 */
void ELEMENT_DUMP_INTO_LEGMAP(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDI	@LEGLL,AR0
EDILL:
    // asm 0000AA84: CMPI	0,AR0
    // asm 0000AA84: 	BEQ	ENDIT
    // asm 0000AA86: 	LDI	AR0,AR1
    // asm: 	LDI	*AR0,AR0
    // asm 0000AA87: 	INC	AR1
    // asm 0000AA89: 	LDI	LEG_SIZE-1,RC
    // asm: 	RPTB	K99
    // asm 0000AA8A: 	LDI	*AR1++,R0
K99:
    // asm 0000AA8B: STI	R0,*AR3++
    // asm 0000AA8C: 	BU	EDILL
ENDIT:
    // asm 0000AA8E: 	POP	AR1
    // asm 0000AA8F: 	POP	AR0
    // asm 0000AA90: 	POP	R0
    // asm 0000AA91: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ELEMENT_DUMP_INTO_LEGMAP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	AR0	START INDEX
*	AR1	END INDEX
*
*
 */
void LEG_GENERATE_MAP(void)
{
    // asm: 	LDI	AR1,R1
    // asm: 	ADDI	2,R1
    // asm: 	STI	R1,@FINISH_ID
    // asm: 	LDI	@LAST_END_CACHE,AR2
    // asm: 	LDI	@LAST_END_INDEX,AR4
    // 	;GROUP LOOP
    // asm 0000AA99: GENMAPLP
    // *
    // *FIND THE TYCOMAP ENTRY APPROPRIATE TO THIS LEG
    // *
    // *
    // *
LPP:
    // asm 0000AA9B: CMPI	AR0,AR4
    // asm: 	BEQ	DNFND
    // asm: 	LDI	*AR2++(6),R1
    // asm: 	TSTB	1,R1
    // asm: 	BZ	NOOVERA
    // asm 0000AA9B: 	ADDI	1,AR2
NOOVERA:
    // asm 0000AA9C: TSTB	SC_REVERSE,R1
    // asm: 	BZ	CNTA
    // asm 0000AA9D: 	NOP	*AR2++(4)
CNTA:
    // asm 0000AA9E: TSTB	SC_OVER2,R1
    // asm 0000AA9F: 	BZ	CNTNA
    // asm 0000AAA0: 	NOP	*AR2++			;OVERLAY 2
CNTNA:
    // asm 0000AAA2: 	INC	AR4			;INCREMENT INDEX
    // asm 0000AAA3: 	BU	LPP
DNFND:
    // asm: 	LDI	@LEG_MAPI,AR3
    // *
    // *LOAD TYCO ENTRIES UNTIL LAST LEG IS FOUND
    // *
    // *
LOAD_LP:
    // 	;AR2	TYCO ENTRY
    // 	;AR3	MAP ENTRY
    // asm: 	CALL	LEG_ADD_GROUP
    // asm: 	LDI	*AR2++(6),R1
    // asm: 	TSTB	1,R1
    // asm: 	BZ	NOOVERB
    // asm: 	ADDI	1,AR2
NOOVERB:
    // asm 0000AAAA: TSTB	SC_REVERSE,R1
    // asm: 	BZ	CNTB
    // asm 0000AAAB: 	NOP	*AR2++(4)
CNTB:
    // asm 0000AAAC: TSTB	SC_OVER2,R1
    // asm 0000AAAD: 	BZ	CNTNB
    // asm 0000AAAE: 	NOP	*AR2++			;OVERLAY 2
CNTNB:
    // asm 0000AAB0: 	INC	AR4			;INCREMENT INDEX
    // asm 0000AAB1: 	CMPI	AR1,AR4
    // asm 0000AAB2: 	BLE	LOAD_LP
    // asm: 	STI	AR2,@LAST_END_CACHE
    // asm 0000AAB5: 	STI	AR4,@LAST_END_INDEX
    // *
    // *LOAD THREE EXTRA LEGS ONTO THE END
    // *
    // asm 0000AAB8: 	ADDI	3,AR1
LOAD_LP2:
    // 	;AR2	TYCO ENTRY
    // 	;AR3	MAP ENTRY
    // asm: 	CALL	LEG_ADD_GROUP
    // asm: 	LDI	*AR2++(6),R1
    // asm: 	TSTB	1,R1
    // asm: 	BZ	NOOVERB2
    // asm: 	ADDI	1,AR2
NOOVERB2:
    // asm: 	TSTB	SC_REVERSE,R1
    // asm 0000AABC: 	BZ	CNTB2
    // asm 0000AABD: 	NOP	*AR2++(4)
CNTB2:
    // asm 0000AABE: TSTB	SC_OVER2,R1
    // asm 0000AABF: 	BZ	CNTNB2
    // asm: 	NOP	*AR2++			;OVERLAY 2
CNTNB2:
    // asm 0000AAC1: 	INC	AR4			;INCREMENT INDEX
    // asm 0000AAC2: 	CMPI	AR1,AR4
    // asm 0000AAC3: 	BLE	LOAD_LP2
    // *
    // *FLAG THE END OF THE LIST
    // *
    // asm: 	LDI	-1,R0
    // asm: 	RPTS	LEG_SIZE-1
    // asm: 	STI	R0,*AR3++
    // *
    // *AND RETURN TO GAME...
    // *
    // asm 0000AACB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEG_GENERATE_MAP", 0, 0);
    UNIMPL();
}

/* asm: LEG_FLAG	.bss	LEG_FLAG,1 */
int LEG_FLAG;
/* asm: LEG_RADY	.bss	LEG_RADY,1 */
int LEG_RADY;
/* asm: LEG_SECTIONIDX	.bss	LEG_SECTIONIDX,1 */
int LEG_SECTIONIDX;

/* *----------------------------------------------------------------------------
*
*
*
*PARAMETERS
*	AR2	PTR TO TYCO ROM ENTRY
*	AR3	PTR TO LEG MAP ENTRY
*	AR4	INDEX
*
*
 */
void LEG_ADD_GROUP(void)
{
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	R6
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm 0000AACD: 	PUSH	AR3
    // asm 0000AACE: 	PUSH	AR4
    // asm 0000AACF: 	PUSH	AR5
    // asm 0000AAD0: 	PUSH	AR7
    // asm 0000AAD2: 	STI	AR4,@LEG_SECTIONIDX
    // asm 0000AAD4: 	CALL	ELEMENT_INIT
    // asm 0000AAD5: 	CALL	ELEMENT_GET
    // asm 0000AAD6: 	INC	AR3
    // asm: 	LDI	AR2,AR7			;DEDICATED POINTER
    // asm 0000AAD8: 	LDI	*AR7,R5			;LOAD FLAG
    // asm 0000AADA: 	LDI	R5,R6			;LANE COUNT
    // asm: 	RS	7,R6
    // asm: 	AND	1,R6			;THIS IS THE LANES
    // asm 0000AADE: 	LDF	*+AR7(TB_RADY),R2
    // asm 0000AADF: 	STF	R2,@LEG_RADY
    // asm: 	LDI	@MATRIXAI,AR2		;Group rotation matrix
    // asm: 	CALL	HPFIND_YMATRIX		;require High Precision
    // asm 0000AAE0: 	LDI	*+AR7(TB_GROUP),AR5	;Group pointer
    // asm 0000AAE1: 	ADDI	1,AR5			;skip radius
    // asm 0000AAE3: 	LDI	*AR5++,R4		;get number of objects to load
    // asm: 	SUBI	1,R4
L12:
    // asm 0000AAE5: LDI	*AR5++,AR2		;GET MODEL PTR
    // asm: 	FLOAT	*AR5++,R1		;GET X POSITION
    // asm: 	STF	R1,*+AR3(LEG_POSX)
    // asm 0000AAE6: 	FLOAT	*AR5++,R1		;GET Y POSITION
    // asm: 	TSTB	SC_REVERSE,R5
    // asm 0000AAE8: 	BZD	NOTREVERSED
    // asm 0000AAE9: 	STF	R1,*+AR3(LEG_POSY)
    // asm 0000AAEA: 	FLOAT	*AR5++,R1		;GET Z POSITION
    // asm 0000AAEB: 	STF	R1,*+AR3(LEG_POSZ)
    // 	;---->BZD	NOTREVERSED
    // 	;SPECIAL REVERSED CASE
    // 	;
    // asm 0000AAF0: 	PUSH	AR7
    // asm: 	TSTB	SC_OVERLAY,R5
    // asm: 	BNZ	ISOVER
    // asm: 	DEC	AR7
ISOVER:
    // asm 0000AAF1: 	LDF	*+AR7(TB_RVS_POSX),R0	;TRANSLATE BY THE NEGATIVE OFFSET
    // asm 0000AAF2: 	ADDF	*+AR3(LEG_POSX),R0	;POSITION (THIS BLOCKS ENDING POSITION)
    // asm 0000AAF3: 	STF	R0,*+AR3(LEG_POSX)
    // asm: 	LDF	*+AR7(TB_RVS_POSY),R0
    // asm: 	ADDF	*+AR3(LEG_POSY),R0
    // asm 0000AAF5: 	STF	R0,*+AR3(LEG_POSY)
    // asm 0000AAF7: 	LDF	*+AR7(TB_RVS_POSZ),R0
    // asm: 	ADDF	*+AR3(LEG_POSZ),R0
    // asm 0000AAF8: 	STF	R0,*+AR3(LEG_POSZ)
    // asm 0000AAF9: 	POP	AR7
    // asm 0000AAFA: ISOVERA
    // asm 0000AAFB: 	LDI	@MATRIXAI,R2
    // asm 0000AAFC: 	LDI	AR3,AR2
    // asm 0000AAFD: 	ADDI	LEG_POSX,AR2
    // asm 0000AAFE: 	LDI	@VECTORAI,R3
    // asm: 	CALL	MATRIX_MUL		;rotation by occurance matrix
    // asm 0000AB00: 	LDI	@VECTORAI,AR0
    // asm 0000AB01: 	LDF	*AR0++,R1
    // asm 0000AB02: 	ADDF	*+AR7(TB_POSX),R1
    // asm 0000AB03: 	STF	R1,*+AR3(LEG_POSX)
    // asm: 	LDF	*AR0++,R1
    // asm 0000AB04: 	ADDF	*+AR7(TB_POSY),R1
    // asm 0000AB05: 	STF	R1,*+AR3(LEG_POSY)
    // asm 0000AB07: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR7(TB_POSZ),R1
    // asm 0000AB08: 	STF	R1,*+AR3(LEG_POSZ)
    // asm 0000AB0A: 	BU	JOIN_UP
NOTREVERSED:
    // asm 0000AB0C: 	LDI	@MATRIXAI,R2
    // asm 0000AB0D: 	LDI	AR3,AR2
    // asm: 	ADDI	LEG_POSX,AR2
    // asm 0000AB0E: 	LDI	R2,R3
    // asm: 	LDI	@VECTORAI,R3
    // asm: 	CALL	MATRIX_MUL		;ROTATION BY OCCURANCE MATRIX
    // asm 0000AB11: 	LDI	@VECTORAI,AR0
    // asm 0000AB12: 	LDF	*AR0++,R1
    // asm 0000AB13: 	ADDF	*+AR7(TB_POSX),R1
    // asm 0000AB14: 	STF	R1,*+AR3(LEG_POSX)
    // asm: 	LDF	*AR0++,R1
    // asm 0000AB15: 	ADDF	*+AR7(TB_POSY),R1
    // asm 0000AB16: 	STF	R1,*+AR3(LEG_POSY)
    // asm 0000AB18: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR7(TB_POSZ),R1
    // asm 0000AB19: 	STF	R1,*+AR3(LEG_POSZ)
JOIN_UP:
    // asm 0000AB1C: LDF	*AR5++,R2		;LOAD OBJECTS RADIANS
    // asm 0000AB1C: 	LDI	*AR5++,R1		;LOAD OBJECT ID (GENV STYLE)
    // asm: 	LDI	R1,R0
    // asm: 	AND	00F00h,R0
    // asm 0000AB1F: 	CMPI	0B00h,R0
    // asm 0000AB20: 	BNE	NOTDYNAROAD
    // 	;*******************
    // 	;*******************
    // 	;
    // 	;THIS IS THE CASE WE ARE LOOKING FOR
    // 	;
    // 	;*******************
    // 	;*******************
    // 	;
    // 	;add this element to dynamic fLEX list
    // 	;
    // asm: 	LDI	R1,R3
    // asm: 	LDI	@LEG_SECTIONIDX,R1
    // asm: 	LS	8,R1
    // asm: 	LDI	R3,R0
    // asm: 	AND	0FFh,R0
    // asm 0000AB26: 	TSTB	SC_REVERSE,R5			;in the case of reversed track
    // asm 0000AB27: 	BZ	NOTRVSTRK			;we say the index value is
    // asm: 	SUBRI	255,R0				;255 - index
NOTRVSTRK:
    // asm 0000AB29: 	OR	R1,R0
    // asm: 	STI	R0,*+AR3(LEG_ID)
#if USEFIXED
    // asm 0000AB2C: 	FIX	*+AR3(LEG_POSX),R0
    // asm: 	STI	R0,*+AR3(LEG_POSX)
    // asm 0000AB2E: 	FIX	*+AR3(LEG_POSY),R0
    // asm: 	ANDN	1,R0
    // asm: 	OR	R6,R0				;# of LANES
    // asm 0000AB2F: 	STI	R0,*+AR3(LEG_POSY)
    // asm: 	FIX	*+AR3(LEG_POSZ),R0
    // asm 0000AB31: 	STI	R0,*+AR3(LEG_POSZ)
#endif
    // asm: 	DEC	AR3
    // asm 0000AB35: 	CALL	ELEMENT_ADD
    // asm 0000AB36: 	CALL	ELEMENT_GET
    // asm: 	INC	AR3
    // ;	NOP	*AR3++(LEG_SIZE)
NOTDYNAROAD:
    // asm 0000AB3A: 	SUBI	1,R4
    // asm: 	BGE	L12
    // asm 0000AB3B: ACTIVATE_X
    // asm 0000AB3B: 	POP	AR7
    // asm 0000AB3C: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm 0000AB3D: 	POP	AR0
    // asm 0000AB3E: 	POP	R6
    // asm 0000AB3F: 	POP	R5
    // asm 0000AB40: 	POP	R4
    // asm 0000AB42: 	CALL	ELEMENT_DUMP_INTO_LEGMAP
    // asm 0000AB43: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEG_ADD_GROUP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*POST PROCESS LEG MAP
*
*
*
*
 */
void GENERATE_LINEAR_DISTANCE(void)
{
#if USEEXTENDED
    // asm: 	LDL	LEG_MAP,AR2
    // asm: 	FLOAT	*+AR2(LEG_POSX),R6	;STARTING POSITION
    // asm: 	FLOAT	*+AR2(LEG_POSZ),R7
    // asm: 	CLRF	R4			;ACCUMULATED DISTANCE
GLDLP:
    // asm: LDI	*AR2,R0
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	GLDX
    // asm: 	FLOAT	*+AR2(LEG_POSX),R2	;STARTING POSITION
    // asm: 	FLOAT	*+AR2(LEG_POSZ),R1
    // asm: 	SUBF	R5,R2
    // asm: 	SUBF	R6,R1
    // asm: 	MPYF	R2,R2
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	CALL	SQRT
    // asm: 	ADDF	R0,R4
    // asm: 	FIX	R4,R0
    // asm: 	LS	16,R0
    // asm: 	LDI	*+AR2(LEG_POSY),R6
    // asm: 	LS	16,R6
    // asm: 	RS	16,R6
    // asm: 	OR	R0,R6
    // asm: 	STI	R6,*+AR2(LEG_POSY)
    // asm: 	FLOAT	*+AR2(LEG_POSX),R6	;STARTING POSITION
    // asm: 	FLOAT	*+AR2(LEG_POSZ),R7
    // asm: 	NOP	*AR3++(LEG_SIZE)
    // asm: 	BU	GLDLP
GLDX:
#endif
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GENERATE_LINEAR_DISTANCE", 0, 0);
    UNIMPL();
}

#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/obj.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/smoke.h"

/*
 * Source module: asm/SMOKE.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: TIRE_SMOKE_COUNT	.bss	TIRE_SMOKE_COUNT,1 */
int TIRE_SMOKE_COUNT;
/* asm: SMOKEANI	SMOKEANI */
/* asm: 	.word	bnout2,bnout4,bnout6,bnout8,bnout10,bnout12,bnout14,bnout16,-1 */
int SMOKEANI[] = {
    bnout2, bnout4, bnout6, bnout8, bnout10, bnout12, bnout14, bnout16, -1,
};
/* asm: SMOKE2ANI	SMOKE2ANI */
/* asm: 	.word	bnout1,bnout2,bnout3,bnout4,bnout5,bnout6,bnout7,bnout8,bnout9,bnout10 */
/* asm: 	.word	bnout11,bnout12,bnout14,bnout15,bnout16,-1 */
int SMOKE2ANI[] = {
    bnout1, bnout2, bnout3, bnout4, bnout5, bnout6, bnout7, bnout8, bnout9, bnout10,
    bnout11, bnout12, bnout14, bnout15, bnout16, -1,
};
// *----------------------------------------------------------------------------
// * SMOKE	PROC
// *INPUT	AR4 = CAROBJ
// *	AR5 = CARBLK
// *Creates and maintains several puffs of smoke
#define NUM_SMOKES PDATA
#define SMOKE_PAL (PDATA+1)
#define SMOKE_OBJS (PDATA+2)
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: SPARKANI	SPARKANI */
/* asm: 	;	.word	sparc10,sparc10,sparc11,sparc11,sparc12,sparc12,-1 */
/* asm: 	.word	x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,-1 */
/* asm: 	;	.word	x2,x4,x6,x8,x10,-1 */
int SPARKANI[] = {
    x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, -1,
};
// *----------------------------------------------------------------------------
// * INIT_SPARK
// *INPUT	AR4 = object of parent
// *Creates and maintains several spark animations
#define COLL_X PDATA
#define COLL_Y (PDATA+1)
#define COLL_Z (PDATA+2)
#define FRAME_ON (PDATA+3)
#define CAR_OBJ (PDATA+4)
#define CAR_BLOCK (PDATA+5)
#define NUM_SPARKS (PDATA+6)
#define SPARK_OBJS (PDATA+7)

void SMOKE_PROC(void)
{
    // asm: 	LDI	@TIRE_SMOKE_COUNT,R0
    // asm: 	CMPI	1,R0
    // asm: 	BGE	SMOKE_DIE
    // asm: 	LDF	0,R7
    // asm: 	LDL	bnout1_smoke,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR7(SMOKE_PAL)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(NUM_SMOKES)
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@TIRE_SMOKE_COUNT
    // asm: 	BR	SMOKELP_ENTRY
SMOKE_PUFFLP:
    // asm: 	LDI	0,R6
    // asm: 	LDI	0,R5
    // asm: 	LDI	SMOKE_OBJS,IR0
SMPUFFLP1:
    // asm: 	LDI	*+AR7(IR0),AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BEQ	SMPUFF1
    // asm: 	LDI	*+AR0(OUSR1),AR1
    // asm: 	LDI	*AR1++,R0
    // asm: 	BN	SMOKE_DONE
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	STI	AR1,*+AR0(OUSR1)
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	@NFRAMES,R0
    // asm: 	MPYF	*+AR5(CARSPEED),R0
    // asm: 	MPYF	1.51,R0
    // asm: 	SUBF	50,R0		;Constant speed less than the player is moving
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORAI,AR1
    // asm: 	LDF	*+AR1(X),R0
    // asm: 	ADDF	*+AR0(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR1(Z),R0
    // asm: 	ADDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	PUSH	AR4
    // asm: 	PUSH	IR0
    // asm: 	PUSH	R5
    // asm: 	LDI	AR0,AR4
    // asm: 	ADDI	OPOSX,AR4
    // asm: 	CALL	CAMSCAN
    // asm: 	POP	R5
    // asm: 	POP	IR0
    // asm: 	POP	AR4
    // asm: 	LDI	*+AR7(IR0),AR0
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	ADDI	1,R6
SMPUFF1:
    // asm: 	ADDI	1,IR0
    // asm: 	ADDI	1,R5
    // asm: 	CMPI	*+AR7(NUM_SMOKES),R5
    // asm: 	BNE	SMPUFFLP1
    // asm: SMOKE_CONT
    // asm: 	CMPI	0,R6
    // asm: 	BEQ	SMOKEX
SMOKELP_ENTRY:
    // asm: 	LDF	0,R5
    // asm: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm: 	BNE	SMOKEN
    // asm: 	CMPF	0,R7
    // asm: 	BNE	KLUDGE_MO
    // asm: 	LDF	2,R7			;wait N more frames
SMOKEN:
    // asm: 	CALL	CREATE_SMOKE_OBJ
KLUDGE_MO:
    // asm: 	SUBF	1.0,R7
    // asm: 	SLEEP	1
    // asm: 	BR	SMOKE_PUFFLP
SMOKE_DONE:
    // asm: 	LDI	AR0,AR2
    // asm: 	PUSH	IR0
    // asm: 	CALL	OBJ_DELETE
    // asm: 	POP	IR0
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(IR0)		;make null on list
    // asm: 	BR	SMPUFFLP1
SMOKEX:
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@TIRE_SMOKE_COUNT
SMOKE_DIE:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOKE_PROC", 0, 0);
    UNIMPL();
}

void CREATE_SMOKE_OBJ(void)
{
    // *----------------------------------------------------------------------------
    // * CREATE_SMOKE_OBJ
    // *INPUT	AR4 = CAROBJ
    // *	AR5 = CARBLK
    // *	R5 = ZOFFSET
    // *OUTPUT	SETC if no obj made; CLRC if obj made
    // *Creates a puff of smoke
    // asm: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm: 	CMPI	1,R0
    // asm: 	BLT	DO_SMOKE
    // asm: 	LDF	*+AR5(CARBRAKE),R0
    // asm: 	CMPF	0.4,R0
    // asm: 	BLT	CHECK_SKID
    // asm: 	LDF	*+AR5(CARSPEED),R0
    // asm: 	CMPF	20,R0
    // asm: 	BGT	OK_SMOKE
CHECK_SKID:
    // asm: 	LDF	*+AR5(CARSKID),R0
    // asm: 	CMPF	0.25,R0
    // asm: 	BLT	NO_SMOKE			;NO SKID ACTIVE
OK_SMOKE:
    // asm: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm: 	CMPI	20,R0
    // asm: 	BGE	NO_SMOKE
DO_SMOKE:
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	GETCARBODY
    // asm: 	LDI	AR0,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	@MATRIXAI,R3
    // asm: 	CALL	CONCATMATV
    // asm: 	LDI	@SMOKEANII,AR1
    // asm: 	LDF	*+AR5(CARSKID),R0
    // asm: 	CMPF	0.5,R0
    // asm: 	LDIGT	@SMOKE2ANII,AR1
    // asm: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm: 	LDINE	@SMOKE2ANII,AR1
    // asm: 	LDI	*AR1,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	CSOX
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	GET_REAR
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR5(RR_PCOL),AR3
    // asm: 	LDI	*+AR3(OID),R4
    // asm: 	CALL	INIT_SMOKE
    // asm: 	LDI	*AR1,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	POP	AR2
    // asm: 	BC	CSOX
    // asm: 	CALL	GET_OTHER_REAR
    // asm: 	LDI	*+AR5(LR_PCOL),AR3
    // asm: 	LDI	*+AR3(OID),R4
    // asm: 	CALL	INIT_SMOKE
CSOX:
    // asm: 	RETS
NO_SMOKE:
    // asm: 	SETC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CREATE_SMOKE_OBJ", 0, 0);
    UNIMPL();
}

void GET_OTHER_REAR(void)
{
    // *----------------------------------------------------------------------------
    // *FIND THE WHEEL IN THE DYNALIST OF THE CAR POINTED TO BY AR2
    // *INPUT	AR2 = CARS OBJECT
    // *OUTPUT AR0 = MATRIX OF THE BODY
    // * SCRAMBLES R0,R1
    // asm: 	LDI	*AR2,R0
    // asm: 	BR	FBLOOP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_OTHER_REAR", 0, 0);
    UNIMPL();
}

void GET_REAR(void)
{
    // asm: 	LDI	*+AR2(ODYNALIST),R0
    // asm: 	SLOCKON	Z,"UTIL\CARPROC   dynamic objects not found"
FBLOOP:
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	*+AR2(DYNAFLAG),R1
    // asm: 	CMPI	1,R1
    // asm: 	BZ	FOUND_REAR		;1 = rear tire
    // asm: 	LDI	*AR2,R0
    // asm: 	BR	FBLOOP
FOUND_REAR:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_REAR", 0, 0);
    UNIMPL();
}

void INIT_SMOKE(void)
{
    // *----------------------------------------------------------------------------
    // *INPUT	AR0 = obj to init
    // *	AR1 = ANI to init with
    // *	AR2 = wheel obj
    // *	MATRIXAI - MATRIX of car
    // *	R4 = OID of object over
    // *	R5 = ZOFFSET
    // asm: 	PUSH	AR1
    // asm: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm: 	ADDI	SMOKE_OBJS,R0
    // asm: 	LDI	R0,IR0
    // asm: 	STI	AR0,*+AR7(IR0)
    // ;	LDI	@SMOKEANII,AR1
    // asm: 	STI	AR1,*+AR0(OUSR1)	;pointer to animation table
    // asm: 	CMPI	300h,R4
    // asm: 	BNE	INSM1
    // asm: 	LDI	*+AR7(SMOKE_PAL),R0
    // asm: 	STI	R0,*+AR0(OPAL)
INSM1:
    // asm: 	LDI	*AR1,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	LDI	PLYR_C|PLYR_SMOKE_S,R0
    // asm: 	STI	R0,*+AR0(OID)
    // asm: 	LDF	40,R0
    // asm: 	CALL	SFRAND
    // asm: 	ADDF	*+AR2(DYNACENTERX),R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	130,R0
    // asm: 	ADDF	*+AR2(DYNACENTERY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR2(DYNACENTERZ),R0
    // asm: 	ADDF	R5,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	LDI	R3,AR2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	ORM	O_POSTER|O_NOCOLL,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm: 	ADDI	1,R0			;NOTE this instruction clears the CARRY
    // asm: 	STI	R0,*+AR7(NUM_SMOKES)
    // asm: 	POP	AR1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_SMOKE", 0, 0);
    UNIMPL();
}

void SORT_SMOKE(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // asm: 	LDI	PLYR_C,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	BNC	SORT_SMOKEX
    // asm: 	LDI	AR0,AR5
    // asm: 	LDI	AR5,AR6			;Stop when AR6 is reached
    // asm: 	LDI	@OACTIVE,AR4
    // asm: 	LDI	AR4,AR2
SSLOOP:
    // asm: 	CMPI	AR4,AR6
    // asm: 	BEQ	SORT_SMOKEX		;Reached the object we linked behind
    // asm: 	CMPI	0,AR4
    // asm: 	BEQ	SORT_SMOKEX
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	CMPI	PLYR_C|PLYR_SMOKE_S,R0
    // asm: 	BNE	SSLOOPEND
    // asm: 	CMPI	@OACTIVE,AR4		;First item in list?
    // asm: 	BNE	UNLINK
    // asm: 	LDI	*AR4,R0
#if DEBUG
    // asm: 	BEQ	$		;The smoke should never be the only item displayed
#endif
    // asm: 	STI	R0,@OACTIVE
    // asm: 	BR	LINK
UNLINK:
    // asm: 	LDI	*AR4,R0
    // asm: 	STI	R0,*AR2
LINK:
    // asm: 	LDI	*AR5,R1		;get object after AR5
    // asm: 	STI	AR4,*AR5	;Link this object to AR5
    // asm: 	STI	R1,*AR4		;Link the next object to AR4
    // asm: 	LDI	AR4,AR5
    // asm: 	LDI	R0,AR4
    // asm: 	BR	SSLOOP
SSLOOPEND:
    // asm: 	LDI	AR4,AR2		;AR2 = last object for unlinking
    // asm: 	LDI	*AR4,AR4
    // asm: 	BR	SSLOOP
SORT_SMOKEX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SORT_SMOKE", 0, 0);
    UNIMPL();
}

void INIT_SPARK(void)
{
    // asm: 	CALL	PUSHALL
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	INIT_SPARKX
    // asm: 	CALL	TOO_MANY_SPARKS
    // asm: 	BC	INIT_SPARKX
    // asm: 	LDI	@PLYPROC,AR7
    // asm: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    // asm: 	BC	INIT_SPARKX
    // asm: 	LDI	AR0,AR7
    // asm: 	STI	AR4,*+AR7(CAR_OBJ)
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	STI	AR5,*+AR7(CAR_BLOCK)
    // asm: 	LDI	0,R5
IS_LOOP:
    // asm: 	LDI	@SPARKANII,AR0
    // asm: 	LDI	*AR0,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	IS_LOOPX
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	OR	O_NOCOLL|O_POSTER,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	ADD_RDDEBRIS
    // asm: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	R5,IR0
    // asm: 	ADDI	SPARK_OBJS,IR0
    // asm: 	STI	AR4,*+AR7(IR0)
    // asm: 	LDI	*+AR7(CAR_OBJ),AR2	;Use the body of the cars matrix
    // asm: 	CALL	GETCARBODY		;get the over rotation of the car body
    // asm: 	LDI	AR0,AR2
    // asm: 	LDI	*+AR7(CAR_OBJ),R2	;MATRIX = OVERROT+ROT of cars body
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	AR4,R3
    // asm: 	ADDI	OMATRIX,R3
    // asm: 	CALL	CONCATMATV
    // asm: 	LDF	*+AR5(CARXPLUS),R0
    // asm: 	SUBF	*+AR5(CARXMINUS),R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	*+AR5(CARXMINUS),R0
    // asm: 	LDF	R0,R1
    // asm: 	LDI	*+AR7(CAR_OBJ),AR0
    // asm: 	LDF	*+AR0(OPOSX),R0		;Set the start point of the spark
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	FLOAT	180,R0
    // asm: 	STF	R0,*+AR4(OVELY)		;bottom of car
    // asm: 	LDF	*+AR5(CARZMINUS),R0	;BACK
    // asm: 	STF	R0,*+AR4(OVELZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	LDF	1,R1			;absolute move(note frame rate dependant)
    // asm: 	CALL	OBJ_MOVE		;Offset the starting point
    // asm: 	LDI	@SPARKANII,R0
    // asm: 	STI	R0,*+AR4(OVELZ)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR4(OVELY)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	ADDI	1,R5
    // asm: 	CMPI	6,R5
    // asm: 	BLT	IS_LOOP
IS_LOOPX:
    // asm: 	CMPI	0,R5
    // asm: 	BEQ	INIT_SPARK_KILL
    // asm: 	STI	R5,*+AR7(NUM_SPARKS)
INIT_SPARKX:
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_SPARK", 0, 0);
    UNIMPL();
}

void REPLICATE_SPARK(void)
{
    // *----------------------------------------------------------------------------
    // *Called from within SPARK_PROC. Will make a new spark based on...
    // *INPUT	AR7 = PROCESS BLOCK OF SPARK_PROC
    // *	R7  = FL, NUMBER OF FRAMES SINCE LAST SPARK
    // asm: 	LDI	*+AR7(NUM_SPARKS),R0
    // asm: 	CMPI	4,R0
    // asm: 	BGE	REPSPARKX
    // asm: 	LDI	AR7,AR0
    // asm: 	ADDI	SPARK_OBJS,AR0
    // asm: 	LDI	0,R0
FIND_LAST_SPARKLP:
    // asm: 	LDI	*AR0++,R1		;This loop will leave AR4 = to last NONZERO SPARK
    // asm: 	LDINE	R1,AR4
    // asm: 	ADDI	1,R0
    // asm: 	CMPI	*+AR7(NUM_SPARKS),R0
    // asm: 	BLT	FIND_LAST_SPARKLP
    // asm: 	LDI	@SPARKANII,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	REPSPARKX
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	OR	O_NOCOLL|O_POSTER,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDF	40,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	90,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	FLOAT	@NFRAMES,R0
    // asm: 	MPYF	50,R0			;Constant speed less than the player is moving
    // asm: 	MPYF	R7,R0			;NUMBER OF FRAMES SINCE LAST SPARK CREATED
    // asm: 	NEGF	R0			;OFFSET FROM LAST SPARK
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OPOSX,AR2
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR0(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	*+AR7(NUM_SPARKS),AR2
    // asm: 	ADDI	SPARK_OBJS,AR2
    // asm: 	ADDI	AR7,AR2
    // asm: 	STI	AR0,*AR2
    // asm: 	LDF	*+AR4(OVELY),R0
    // asm: 	STF	R0,*+AR0(OVELY)
    // asm: 	LDI	*+AR7(NUM_SPARKS),R0
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,*+AR7(NUM_SPARKS)
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	ADD_RDDEBRIS
    // asm: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	@SPARKANII,R0
    // asm: 	STI	R0,*+AR4(OVELZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
REPSPARKX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REPLICATE_SPARK", 0, 0);
    UNIMPL();
}

void SPARK_PROC(void)
{
    // *----------------------------------------------------------------------------
    // * SPARK_PROC	PROC
    // *INPUT	PDATA = SETUP see equates at begining of file
    // *Maintains several spark animations
    // asm: 	LDF	0,R7
    // asm: 	LDI	*+AR7(CAR_BLOCK),AR5
SSANI_LOOP:
    // asm: 	LDI	0,R5		;SPARK ON
    // asm: 	LDI	0,R6		;NUMBER OF SPARKS STILL ACTIVE
SS1:
    // asm: 	LDI	R5,IR0
    // asm: 	ADDI	SPARK_OBJS,IR0
    // asm: 	LDI	*+AR7(IR0),AR4
    // asm: 	CMPI	0,AR4
    // asm: 	BEQ	NEXT_SPARK		;SPARK is no longer with us
    // asm: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY OFF LIST
    // asm: 	TSTB	O_LIST_M,R0
    // asm: 	BZ	SPARK_ANIX			;YES, DELETE FROM LIST
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm: 	BNE	SPARK_ANIX		;not a spark???!!!
    // asm: 	LDI	*+AR4(OVELZ),AR6		;ANIMATION POINTER
    // asm: 	LDI	*++AR6,R0
    // asm: 	BN	SPARK_ANIX
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	STI	AR6,*+AR4(OVELZ)
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	CMPI	1000,R0
    // asm: 	BLE	SPARK_ANIX
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	@NFRAMES,R0
    // asm: 	MPYF	*+AR5(CARSPEED),R0
    // asm: 	MPYF	1.5,R0
    // asm: 	SUBF	100,R0		;Constant speed less than the player is moving
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORAI,AR1
    // asm: 	LDF	*+AR1(X),R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR1(Z),R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	PUSH	AR4
    // asm: 	PUSH	IR0
    // asm: 	PUSH	R5
    // asm: 	ADDI	OPOSX,AR4
    // asm: 	CALL	CAMSCAN
    // asm: 	POP	R5
    // asm: 	POP	IR0
    // asm: 	POP	AR4
    // asm: 	BNC	SPARK_ANIX	;This spark is out of this world
    // asm: 	SUBF	*+AR4(OVELY),R0		;SPARKS OFFSET FROM THE GROUND
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	ADDI	1,R6
NEXT_SPARK:
    // asm: 	ADDI	1,R5
    // asm: 	CMPI	*+AR7(NUM_SPARKS),R5
    // asm: 	BLT	SS1
    // asm: 	CMPI	0,R6
    // asm: 	BEQ	SPARK_DIE
    // asm: 	SLEEP	1
    // asm: 	ADDF	1,R7
    // asm: 	CMPF	2,R7			;WAIT NFRAMES
    // asm: 	BNE	KLUDGE_MOFO
    // asm: 	CALL	REPLICATE_SPARK		;Will replicate the latest spark still active
    // asm: 	LDF	0,R7
KLUDGE_MOFO:
    // asm: 	BR	SSANI_LOOP
SPARK_DIE:
    // asm: 	DIE
SPARK_ANIX:
    // asm: 	LDI	R5,IR0
    // asm: 	ADDI	SPARK_OBJS,IR0
    // asm: 	LDI	*+AR7(IR0),AR2
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(IR0)		;COLOR THIS SPARK GONE
    // asm: 	LDI	*+AR2(OFLAGS),R0	;CHECK IF ALREADY OFF LIST
    // asm: 	TSTB	O_LIST_M,R0
    // asm: 	BZ	NO_OBJ
    // asm: 	LDI	*+AR2(OID),R0
    // asm: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm: 	BNE	NO_OBJ			;not a spark???!!!
    // asm: 	CALL	OBJ_DELETE
NO_OBJ:
    // asm: 	BR	NEXT_SPARK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPARK_PROC", 0, 0);
    UNIMPL();
}

void INIT_COLLA_OBJS(void)
{
    // *----------------------------------------------------------------------------
    // * INIT_COLLA_OBJS
    // *INPUT	AR0 points to proc memory
    // *Creates several spark animations
    // asm: 	LDI	0,R5
ICO_LOOP:
    // asm: 	LDI	@SPARKANII,AR0
    // asm: 	LDI	*AR0,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	ICO_LOOPX		;out of objects
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_NOCOLL|O_POSTER,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	CALL	ADD_RDDEBRIS
    // asm: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	R5,IR0
    // asm: 	ADDI	SPARK_OBJS,IR0
    // asm: 	STI	AR4,*+AR7(IR0)
    // asm: 	FLOAT	60,R0			;calculate the random offset that this obj will have
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R1
    // asm: 	FLOAT	-60,R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	20,R0
    // asm: 	LDF	R0,R2
    // asm: 	FLOAT	60,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R3
    // asm: 	LDI	*+AR7(CAR_OBJ),AR0
    // asm: 	LDF	*+AR7(COLL_X),R0		;Set the start point of the spark
    // asm: 	ADDF	R1,R0
    // asm: 	ADDF	*+AR0(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR7(COLL_Y),R0
    // asm: 	ADDF	R2,R0
    // asm: 	ADDF	-30,R0
    // asm: 	NEGF	R0,R1
    // asm: 	ADDF	180,R1				;AVERAGE car hight from road
    // asm: 	STF	R1,*+AR4(OVELY)			;This will be used as the y offset
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR7(COLL_Z),R0
    // asm: 	ADDF	R3,R0
    // asm: 	ADDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	@SPARKANII,R0
    // asm: 	STI	R0,*+AR4(OVELZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	ADDI	1,R5
    // asm: 	CMPI	2,R5
    // asm: 	BLT	ICO_LOOP
ICO_LOOPX:
    // asm: 	CMPI	0,R5
    // asm: 	BEQ	INIT_SPARK_KILL
    // asm: 	STI	R5,*+AR7(NUM_SPARKS)
    // asm: 	RETS
INIT_SPARK_KILL:
    // asm: 	LDI	AR7,AR2
    // asm: 	LDI	0,AR7		;Stupid thing thinks I'm commiting suicide!
    // asm: 	CALL	PRC_KILL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_COLLA_OBJS", 0, 0);
    UNIMPL();
}

void WALL_SPARK(void)
{
    // *----------------------------------------------------------------------------
    // *INPUT	AR4	Car object
    // *	AR5	Car block
    // *OUTPUT	NONE
    // *
    // * Creates sparks at position where the wall was hit
    // asm: 	CALL	PUSHALL
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	WALL_SPARKX
    // asm: 	CMPI	@PLYCAR,AR4
    // asm: 	BNE	WALL_SPARKX		;Only work for the players car
    // asm: 	LDI	*+AR5(CARTRAK),R0
    // asm: 	BZ	WALL_SPARKX		;DUDE IS NOT ON THE ROAD
    // asm: 	CALL	TOO_MANY_SPARKS
    // asm: 	BC	WALL_SPARKX
    // asm: 	LDI	@PLYPROC,AR7
    // asm: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    // asm: 	BC	WALL_SPARKX
    // asm: 	LDI	AR0,AR7
    // asm: 	STI	AR4,*+AR7(CAR_OBJ)
    // asm: 	STI	AR5,*+AR7(CAR_BLOCK)
    // asm: 	LDF	*+AR5(CARYROT),R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // ;find the angle of the car reletive to the track it is on
    // asm: 	LDI	*+AR5(CARTRAK),R0
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	GETRDIR
    // asm: 	LDF	R0,R2
    // ;	LDF	*+AR5(CARVROT),R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R2,R0
    // asm: 	LDF	*+AR5(CARYROT),R2
    // asm: 	CALL	NORMITS
    // asm: 	SUBF	R0,R2
    // asm: 	CALL	NORMITS
    // ;find the side of the road the car is on
    // asm: 	LDF	*+AR5(CARDIST2CNTR),R0		;- = right side ;+ = left side
    // ;determine which corner of the car hit the wall
    // asm: 	ABSF	R2,R1
    // asm: 	CMPF	HALFPI,R1
    // asm: 	BLT	FACINGFRONT
    // asm: 	NEGF	R0
    // asm: 	NEGF	R2
FACINGFRONT:
    // asm: 	CMPF	0,R0			;which side is the wall?
    // asm: 	BGT	LEFT_SIDE
    // asm: RIGHT_SIDE
    // asm: 	LDF	*+AR5(CARXPLUS),R1
    // asm: 	CMPF	0,R2
    // asm: 	LDFGT	*+AR5(CARZMINUS),R0	;BACK
    // asm: 	LDFLE	*+AR5(CARZPLUS),R0	;FRONT
    // asm: 	BR	WALLS1
LEFT_SIDE:
    // asm: 	LDF	*+AR5(CARXMINUS),R1
    // asm: 	CMPF	0,R2
    // asm: 	LDFGT	*+AR5(CARZPLUS),R0	;FRONT
    // asm: 	LDFLE	*+AR5(CARZMINUS),R0	;BACK
WALLS1:
    // ;get the coords of that corner
    // ;transform them based on the body of the car
    // asm: 	STF	R1,*+AR7(COLL_X)
    // asm: 	STF	R0,*+AR7(COLL_Z)
    // asm: 	LDF	-90,R0
    // asm: 	STF	R0,*+AR7(COLL_Y)
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	COLL_X,AR2
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	CALL	MATRIX_MUL
    // ;no go and make the spark objects
    // asm: 	CALL	INIT_COLLA_OBJS
WALL_SPARKX:
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WALL_SPARK", 0, 0);
    UNIMPL();
}

void IMPACT_SPARK(void)
{
    // *----------------------------------------------------------------------------
    // *IMPACT_SPARK
    // *	INPUT *-AR3(1) = XYZ
    // *	INPUT	AR0,AR1 = two objects
    // *
    // asm: 	CALL	PUSHALL
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	IMPACT_SPARKX
    // asm: 	CMPI	@PLYCAR,AR0
    // asm: 	LDIEQ	AR0,AR4
    // asm: 	BEQ	IMPACTED_PLAYER
    // asm: 	CMPI	@PLYCAR,AR1
    // asm: 	LDIEQ	AR1,AR4
    // asm: 	BNE	IMPACT_SPARKX		;Only work for the players car
IMPACTED_PLAYER:
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	LDI	*+AR5(CARTRAK),R0
    // asm: 	BZ	IMPACT_SPARKX		;DUDE IS NOT ON THE ROAD
    // asm: 	CALL	TOO_MANY_SPARKS
    // asm: 	BC	IMPACT_SPARKX
    // asm: 	LDI	@PLYPROC,AR7
    // asm: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    // asm: 	BC	IMPACT_SPARKX
    // asm: 	LDI	AR0,AR7
    // asm: 	STI	AR4,*+AR7(CAR_OBJ)
    // asm: 	STI	AR5,*+AR7(CAR_BLOCK)
    // asm: 	LDF	*-AR3(1),R0
    // asm: 	SUBF	*+AR4(OPOSX),R0		;Make offset from the car
    // asm: 	STF	R0,*+AR7(COLL_X)
    // ;	LDF	*AR3,R0
    // ;	SUBF	60,R0
    // ;	SUBF	*+AR4(OPOSY),R0		;Make offset from the car
    // asm: 	LDF	-80,R0
    // asm: 	STF	R0,*+AR7(COLL_Y)
    // asm: 	LDF	*+AR3(1),R0
    // asm: 	SUBF	*+AR4(OPOSZ),R0		;Make offset from the car
    // asm: 	STF	R0,*+AR0(COLL_Z)
    // asm: 	CALL	INIT_COLLA_OBJS
IMPACT_SPARKX:
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "IMPACT_SPARK", 0, 0);
    UNIMPL();
}

void ROAD_IMPACT_SPARK(void)
{
    // *----------------------------------------------------------------------------
    // *ROAD_IMPACT_SPARK ASSUMSE IT IS CALLED FROM A ROUTINE THAT USED BOXSCAN IN COLLA
    // *INPUT	R2 = distance to road bottom from the point the is bellow it
    // *
    // *
    // asm: 	RETS
    // ;	CALL	PUSHALL
    // ;
    // ;	CREATE	SPARK_COLLA_PROC,UTIL_C|SPARK_T
    // ;;find the lowest coordinate in the box
    // ;	LDI	@BOXSCRAMI,AR1		;Box endpoint coords, set above by BOXSCAN
    // ;	ADDI	24,AR1			;offset to world based
    // ;	LDF	*+AR1(Y),R0
    // ;	LDI	7,RC
    // ;	RPTB	SCANYS
    // ;	CMPF	*+AR1(Y),R0
    // ;	LDILT	AR1,AR2
    // ;	LDFLT	*+AR1(Y),R0
    // ;SCANYS	NOP	*++AR1(3)
    // ;
    // ;;Now set the point of impact t0 this
    // ;	SUBF	60,R2
    // ;	LDF	*+AR2(X),R0
    // ;	STF	R0,*+AR0(PDATA)
    // ;	LDF	*+AR2(Y),R0
    // ;	SUBF	R2,R0
    // ;	STF	R0,*+AR0(PDATA+1)
    // ;	LDF	*+AR2(Z),R0
    // ;	STF	R0,*+AR0(PDATA+2)
    // ;
    // ;;now go make the objects
    // ;	CALL	INIT_COLLA_OBJS
    // ;ROAD_IMPACT_SPARKX
    // ;	CALL	POPALL
    // ;	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROAD_IMPACT_SPARK", 0, 0);
    UNIMPL();
}

void SKID_SPARK(void)
{
    // *----------------------------------------------------------------------------
    // *INPUT	AR4 = CARS OBJECT
    // *	AR5 = CARBLK
    // asm: 	RETS
    // ;	CALL	PUSHALL
    // ;
    // ;	LDI	*+AR4(ODIST),R0
    // ;	CMPI	30000,R0
    // ;	BGT	SKID_SPARKX
    // ;
    // ;	CREATE	SPARK_COLLA_PROC,UTIL_C|SPARK_T
    // ;;Get the grounds elevation
    // ;	LDI	*+AR5(CARTRAK),AR1	;OK if on enbankment, grass dosn't make sparks
    // ;	LDF	*+AR1(OPOSY),R0
    // ;	SUBF	30,R0
    // ;	STF	R0,*+AR0(PDATA+1)
    // ;;Now use the velocity of the car to offset the spark behind it
    // ;	LDF	*+AR4(OVELX),R0
    // ;	MPYF	-0.5,R0
    // ;	ADDF	*+AR4(OPOSX),R0
    // ;	STF	R0,*+AR0(PDATA)
    // ;	LDF	*+AR4(OVELZ),R0
    // ;	MPYF	-0.5,R0
    // ;	ADDF	*+AR4(OPOSZ),R0
    // ;	STF	R0,*+AR0(PDATA+2)
    // ;;Go make the spark objects
    // ;	CALL	INIT_COLLA_OBJS
    // ;;done
    // ;SKID_SPARKX
    // ;	CALL	POPALL
    // ;	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SKID_SPARK", 0, 0);
    UNIMPL();
}

void TOO_MANY_SPARKS(void)
{
    // *----------------------------------------------------------------------------
    // *RETURNS	R2= number of spark procs
    // *		C=1 if too many
    // *		C=0 if ok
    // asm: 	LDI	0,R2
    // asm: 	LDI	@PACTIVEI,R0
    // asm: 	BZ	TMSXCC			;NULL LIST?
TMS_LP:
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(PID),R0
    // asm: 	TSTB	UTIL_C,R0
    // asm: 	BZ	TMSLPE
    // asm: 	AND	TYPE_M,R0
    // asm: 	CMPI	SPARK_T,R0
    // asm: 	BNE	TMSLPE
    // asm: 	ADDI	1,R2
TMSLPE:
    // asm: 	LDI	*AR0,R0
    // asm: 	BNZ	TMS_LP
    // ;	CMPI	5,R2
    // asm: 	CMPI	2,R2
    // asm: 	BLT	TMSXCC
    // asm: 	SETC
    // asm: 	RETS
TMSXCC:
    // asm: 	CLRC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOO_MANY_SPARKS", 0, 0);
    UNIMPL();
}

void OBJ_MOVE(void)
{
    // asm: 	LDI	AR2,AR1
    // asm: 	ADDI	OVELX,AR2
    // asm: 	LDI	AR1,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	@VECTORAI,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORAI,AR0
    // asm: 	LDF	*+AR0(X),R0
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR1(OPOSX),R0
    // asm: 	STF	R0,*+AR1(OPOSX)
    // asm: 	LDF	*+AR0(Y),R0
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR1(OPOSY),R0
    // asm: 	STF	R0,*+AR1(OPOSY)
    // asm: 	LDF	*+AR0(Z),R0
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR1(OPOSZ),R0
    // asm: 	STF	R0,*+AR1(OPOSZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_MOVE", 0, 0);
    UNIMPL();
}

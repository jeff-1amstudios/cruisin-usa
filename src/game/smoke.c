#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "obj.h"
#include "mproc.h"
#include "vunit.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "pall.h"
#include "objects.h"
#include "smoke.h"
#include "discovered_labels.h"

/*
 * Source module: asm/SMOKE.ASM
 */

void SMOKE_PROC(void);
void CREATE_SMOKE_OBJ(void);
void GET_OTHER_REAR(void);
void GET_REAR(void);
void INIT_SMOKE(void);
void SORT_SMOKE(void);
void INIT_SPARK(void);
void REPLICATE_SPARK(void);
void SPARK_PROC(void);
void INIT_COLLA_OBJS(void);
void WALL_SPARK(void);
void IMPACT_SPARK(void);
void ROAD_IMPACT_SPARK(void);
void SKID_SPARK(void);
void TOO_MANY_SPARKS(void);
void OBJ_MOVE(void);

/* asm: TIRE_SMOKE_COUNT	.bss	TIRE_SMOKE_COUNT,1 */
int TIRE_SMOKE_COUNT;
/* asm: SMOKEANI */
/* asm: 	.word	bnout2,bnout4,bnout6,bnout8,bnout10,bnout12,bnout14,bnout16,-1 */
int SMOKEANI[] = {
    bnout2, bnout4, bnout6, bnout8, bnout10, bnout12, bnout14, bnout16, -1,
};
/* asm: SMOKE2ANI */
/* asm: 	.word	bnout1,bnout2,bnout3,bnout4,bnout5,bnout6,bnout7,bnout8,bnout9,bnout10 */
/* asm: 	.word	bnout11,bnout12,bnout14,bnout15,bnout16,-1 */
int SMOKE2ANI[] = {
    bnout1, bnout2, bnout3, bnout4, bnout5, bnout6, bnout7, bnout8, bnout9, bnout10,
    bnout11, bnout12, bnout14, bnout15, bnout16, -1,
};
/* *----------------------------------------------------------------------------
* SMOKE	PROC
*INPUT	AR4 = CAROBJ
*	AR5 = CARBLK
*Creates and maintains several puffs of smoke
 */
#define NUM_SMOKES PDATA
#define SMOKE_PAL (PDATA+1)
#define SMOKE_OBJS (PDATA+2)
/* asm: SPARKANI */
/* asm: 	.word	x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,-1 */
int SPARKANI[] = {
    x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, -1,
};
/* *----------------------------------------------------------------------------
* INIT_SPARK
*INPUT	AR4 = object of parent
*Creates and maintains several spark animations
 */
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
    // asm 000084B8: 	LDI	@TIRE_SMOKE_COUNT,R0
    // asm 000084B9: 	CMPI	1,R0
    // asm 000084BA: 	BGE	SMOKE_DIE
    // asm 000084BB: 	LDF	0,R7
    // asm 000084BC: 	LDL	bnout1_smoke,AR2
    // asm 000084BD: 	CALL	PAL_FIND_RAW
    // asm 000084BE: 	STI	R0,*+AR7(SMOKE_PAL)
    // asm 000084BF: 	LDI	0,R0
    // asm 000084C0: 	STI	R0,*+AR7(NUM_SMOKES)
    // asm 000084C1: 	LDI	1,R0
    // asm 000084C2: 	STI	R0,@TIRE_SMOKE_COUNT
    // asm 000084C3: 	BR	SMOKELP_ENTRY
SMOKE_PUFFLP:
    // asm 000084C4: 	LDI	0,R6
    // asm 000084C5: 	LDI	0,R5
    // asm 000084C6: 	LDI	SMOKE_OBJS,IR0
SMPUFFLP1:
    // asm 000084C7: 	LDI	*+AR7(IR0),AR0
    // asm 000084C8: 	CMPI	0,AR0
    // asm 000084C9: 	BEQ	SMPUFF1
    // asm 000084CA: 	LDI	*+AR0(OUSR1),AR1
    // asm 000084CB: 	LDI	*AR1++,R0
    // asm 000084CC: 	BN	SMOKE_DONE
    // asm 000084CD: 	STI	R0,*+AR0(OROMDATA)
    // asm 000084CE: 	STI	AR1,*+AR0(OUSR1)
    // asm 000084CF: 	LDI	@MATRIXAI,AR2
    // asm 000084D0: 	LDF	*+AR5(CARVROT),R2
    // asm 000084D1: 	CALL	FIND_YMATRIX
    // asm 000084D2: 	CALL	CLR_VECTORA
    // asm 000084D3: 	FLOAT	@NFRAMES,R0
    // asm 000084D4: 	MPYF	*+AR5(CARSPEED),R0
    // asm 000084D5: 	MPYF	1.51,R0
    // asm 000084D6: 	SUBF	50,R0		;Constant speed less than the player is moving
    // asm 000084D7: 	STF	R0,*+AR2(Z)
    // asm 000084D8: 	LDI	@MATRIXAI,R2
    // asm 000084D9: 	LDI	AR2,R3
    // asm 000084DA: 	CALL	MATRIX_MUL
    // asm 000084DB: 	LDI	@VECTORAI,AR1
    // asm 000084DC: 	LDF	*+AR1(X),R0
    // asm 000084DD: 	ADDF	*+AR0(OPOSX),R0
    // asm 000084DE: 	STF	R0,*+AR0(OPOSX)
    // asm 000084DF: 	LDF	*+AR1(Z),R0
    // asm 000084E0: 	ADDF	*+AR0(OPOSZ),R0
    // asm 000084E1: 	STF	R0,*+AR0(OPOSZ)
    // asm 000084E2: 	PUSH	AR4
    // asm 000084E3: 	PUSH	IR0
    // asm 000084E4: 	PUSH	R5
    // asm 000084E5: 	LDI	AR0,AR4
    // asm 000084E6: 	ADDI	OPOSX,AR4
    // asm 000084E7: 	CALL	CAMSCAN
    // asm 000084E8: 	POP	R5
    // asm 000084E9: 	POP	IR0
    // asm 000084EA: 	POP	AR4
    // asm 000084EB: 	LDI	*+AR7(IR0),AR0
    // asm 000084EC: 	ADDF	*+AR0(OPOSY),R0
    // asm 000084ED: 	STF	R0,*+AR0(OPOSY)
    // asm 000084EE: 	ADDI	1,R6
SMPUFF1:
    // asm 000084EF: 	ADDI	1,IR0
    // asm 000084F0: 	ADDI	1,R5
    // asm 000084F1: 	CMPI	*+AR7(NUM_SMOKES),R5
    // asm 000084F2: 	BNE	SMPUFFLP1
    // asm 000084F3: SMOKE_CONT
    // asm 000084F3: 	CMPI	0,R6
    // asm 000084F4: 	BEQ	SMOKEX
SMOKELP_ENTRY:
    // asm 000084F5: 	LDF	0,R5
    // asm 000084F6: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm 000084F7: 	BNE	SMOKEN
    // asm 000084F8: 	CMPF	0,R7
    // asm 000084F9: 	BNE	KLUDGE_MO
    // asm 000084FA: 	LDF	2,R7			;wait N more frames
SMOKEN:
    // asm 000084FB: 	CALL	CREATE_SMOKE_OBJ
KLUDGE_MO:
    // asm 000084FC: 	SUBF	1.0,R7
    // asm 000084FD: 	SLEEP	1
    // asm 000084FF: 	BR	SMOKE_PUFFLP
SMOKE_DONE:
    // asm 00008500: 	LDI	AR0,AR2
    // asm 00008501: 	PUSH	IR0
    // asm 00008502: 	CALL	OBJ_DELETE
    // asm 00008503: 	POP	IR0
    // asm 00008504: 	LDI	0,R0
    // asm 00008505: 	STI	R0,*+AR7(IR0)		;make null on list
    // asm 00008506: 	BR	SMPUFFLP1
SMOKEX:
    // asm 00008507: 	LDI	0,R0
    // asm 00008508: 	STI	R0,@TIRE_SMOKE_COUNT
SMOKE_DIE:
    // asm 00008509: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOKE_PROC", 0, 0);
    UNIMPL();
}

void CREATE_SMOKE_OBJ(void)
{
    // asm 0000850A: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm 0000850B: 	CMPI	1,R0
    // asm 0000850C: 	BLT	DO_SMOKE
    // asm 0000850D: 	LDF	*+AR5(CARBRAKE),R0
    // asm 0000850E: 	CMPF	0.4,R0
    // asm 0000850F: 	BLT	CHECK_SKID
    // asm 00008510: 	LDF	*+AR5(CARSPEED),R0
    // asm 00008511: 	CMPF	20,R0
    // asm 00008512: 	BGT	OK_SMOKE
CHECK_SKID:
    // asm 00008513: 	LDF	*+AR5(CARSKID),R0
    // asm 00008514: 	CMPF	0.25,R0
    // asm 00008515: 	BLT	NO_SMOKE			;NO SKID ACTIVE
OK_SMOKE:
    // asm 00008516: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm 00008517: 	CMPI	20,R0
    // asm 00008518: 	BGE	NO_SMOKE
DO_SMOKE:
    // asm 00008519: 	LDI	AR4,AR2
    // asm 0000851A: 	CALL	GETCARBODY
    // asm 0000851B: 	LDI	AR0,AR2
    // asm 0000851C: 	LDI	AR4,R2
    // asm 0000851D: 	ADDI	OMATRIX,R2
    // asm 0000851E: 	LDI	@MATRIXAI,R3
    // asm 0000851F: 	CALL	CONCATMATV
    // asm 00008520: 	LDI	@SMOKEANII,AR1
    // asm 00008521: 	LDF	*+AR5(CARSKID),R0
    // asm 00008522: 	CMPF	0.5,R0
    // asm 00008523: 	LDIGT	@SMOKE2ANII,AR1
    // asm 00008524: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm 00008525: 	LDINE	@SMOKE2ANII,AR1
    // asm 00008526: 	LDI	*AR1,AR2
    // asm 00008527: 	CALL	OBJ_GETE
    // asm 00008528: 	BC	CSOX
    // asm 00008529: 	LDI	AR4,AR2
    // asm 0000852A: 	CALL	GET_REAR
    // asm 0000852B: 	PUSH	AR2
    // asm 0000852C: 	LDI	*+AR5(RR_PCOL),AR3
    // asm 0000852D: 	LDI	*+AR3(OID),R4
    // asm 0000852E: 	CALL	INIT_SMOKE
    // asm 0000852F: 	LDI	*AR1,AR2
    // asm 00008530: 	CALL	OBJ_GETE
    // asm 00008531: 	POP	AR2
    // asm 00008532: 	BC	CSOX
    // asm 00008533: 	CALL	GET_OTHER_REAR
    // asm 00008534: 	LDI	*+AR5(LR_PCOL),AR3
    // asm 00008535: 	LDI	*+AR3(OID),R4
    // asm 00008536: 	CALL	INIT_SMOKE
CSOX:
    // asm 00008537: 	RETS
NO_SMOKE:
    // asm 00008538: 	SETC
    // asm 00008539: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CREATE_SMOKE_OBJ", 0, 0);
    UNIMPL();
}

void GET_OTHER_REAR(void)
{
    // asm 0000853A: 	LDI	*AR2,R0
    // asm 0000853B: 	BR	FBLOOP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_OTHER_REAR", 0, 0);
    UNIMPL();
}

void GET_REAR(void)
{
    // asm 0000853C: 	LDI	*+AR2(ODYNALIST),R0
    // asm: 	SLOCKON	Z,"UTIL\CARPROC   dynamic objects not found"
FBLOOP:
    // asm 0000853D: 	LDI	R0,AR2
    // asm 0000853E: 	LDI	*+AR2(DYNAFLAG),R1
    // asm 0000853F: 	CMPI	1,R1
    // asm 00008540: 	BZ	FOUND_REAR		;1 = rear tire
    // asm 00008541: 	LDI	*AR2,R0
    // asm 00008542: 	BR	FBLOOP
FOUND_REAR:
    // asm 00008543: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_REAR", 0, 0);
    UNIMPL();
}

void INIT_SMOKE(void)
{
    // asm 00008544: 	PUSH	AR1
    // asm 00008545: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm 00008546: 	ADDI	SMOKE_OBJS,R0
    // asm 00008547: 	LDI	R0,IR0
    // asm 00008548: 	STI	AR0,*+AR7(IR0)
    // ;	LDI	@SMOKEANII,AR1
    // asm 00008549: 	STI	AR1,*+AR0(OUSR1)	;pointer to animation table
    // asm 0000854A: 	CMPI	300h,R4
    // asm 0000854B: 	BNE	INSM1
    // asm 0000854C: 	LDI	*+AR7(SMOKE_PAL),R0
    // asm 0000854D: 	STI	R0,*+AR0(OPAL)
INSM1:
    // asm 0000854E: 	LDI	*AR1,R0
    // asm 0000854F: 	STI	R0,*+AR0(OROMDATA)
    // asm 00008550: 	LDI	PLYR_C|PLYR_SMOKE_S,R0
    // asm 00008551: 	STI	R0,*+AR0(OID)
    // asm 00008552: 	LDF	40,R0
    // asm 00008553: 	CALL	SFRAND
    // asm 00008554: 	ADDF	*+AR2(DYNACENTERX),R0
    // asm 00008555: 	STF	R0,*+AR0(OPOSX)
    // asm 00008556: 	FLOAT	130,R0
    // asm 00008557: 	ADDF	*+AR2(DYNACENTERY),R0
    // asm 00008558: 	STF	R0,*+AR0(OPOSY)
    // asm 00008559: 	LDF	*+AR2(DYNACENTERZ),R0
    // asm 0000855A: 	ADDF	R5,R0
    // asm 0000855B: 	STF	R0,*+AR0(OPOSZ)
    // asm 0000855C: 	LDI	@MATRIXAI,R2
    // asm 0000855D: 	LDI	AR0,R3
    // asm 0000855E: 	ADDI	OPOSX,R3
    // asm 0000855F: 	LDI	R3,AR2
    // asm 00008560: 	CALL	MATRIX_MUL
    // asm 00008561: 	LDF	*+AR0(OPOSX),R0
    // asm 00008562: 	ADDF	*+AR4(OPOSX),R0
    // asm 00008563: 	STF	R0,*+AR0(OPOSX)
    // asm 00008564: 	LDF	*+AR0(OPOSY),R0
    // asm 00008565: 	ADDF	*+AR4(OPOSY),R0
    // asm 00008566: 	STF	R0,*+AR0(OPOSY)
    // asm 00008567: 	LDF	*+AR0(OPOSZ),R0
    // asm 00008568: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00008569: 	STF	R0,*+AR0(OPOSZ)
    // asm 0000856A: 	ORM	O_POSTER|O_NOCOLL,*+AR0(OFLAGS)
    // asm 0000856D: 	LDI	AR0,AR2
    // asm 0000856E: 	CALL	OBJ_INSERT
    // asm 0000856F: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm 00008570: 	ADDI	1,R0			;NOTE this instruction clears the CARRY
    // asm 00008571: 	STI	R0,*+AR7(NUM_SMOKES)
    // asm 00008572: 	POP	AR1
    // asm 00008573: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_SMOKE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
 */
void SORT_SMOKE(void)
{
    // asm 00008574: 	LDI	PLYR_C,AR2
    // asm 00008575: 	CALL	OBJ_FIND_FIRST
    // asm 00008576: 	BNC	SORT_SMOKEX
    // asm 00008577: 	LDI	AR0,AR5
    // asm 00008578: 	LDI	AR5,AR6			;Stop when AR6 is reached
    // asm 00008579: 	LDI	@OACTIVE,AR4
    // asm 0000857A: 	LDI	AR4,AR2
SSLOOP:
    // asm 0000857B: 	CMPI	AR4,AR6
    // asm 0000857C: 	BEQ	SORT_SMOKEX		;Reached the object we linked behind
    // asm 0000857D: 	CMPI	0,AR4
    // asm 0000857E: 	BEQ	SORT_SMOKEX
    // asm 0000857F: 	LDI	*+AR4(OID),R0
    // asm 00008580: 	CMPI	PLYR_C|PLYR_SMOKE_S,R0
    // asm 00008581: 	BNE	SSLOOPEND
    // asm 00008582: 	CMPI	@OACTIVE,AR4		;First item in list?
    // asm 00008583: 	BNE	UNLINK
    // asm 00008584: 	LDI	*AR4,R0
#if DEBUG
    // asm: 	BEQ	$		;The smoke should never be the only item displayed
#endif
    // asm 00008585: 	STI	R0,@OACTIVE
    // asm 00008586: 	BR	LINK
UNLINK:
    // asm 00008587: 	LDI	*AR4,R0
    // asm 00008588: 	STI	R0,*AR2
LINK:
    // asm 00008589: 	LDI	*AR5,R1		;get object after AR5
    // asm 0000858A: 	STI	AR4,*AR5	;Link this object to AR5
    // asm 0000858B: 	STI	R1,*AR4		;Link the next object to AR4
    // asm 0000858C: 	LDI	AR4,AR5
    // asm 0000858D: 	LDI	R0,AR4
    // asm 0000858E: 	BR	SSLOOP
SSLOOPEND:
    // asm 0000858F: 	LDI	AR4,AR2		;AR2 = last object for unlinking
    // asm 00008590: 	LDI	*AR4,AR4
    // asm 00008591: 	BR	SSLOOP
SORT_SMOKEX:
    // asm 00008592: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SORT_SMOKE", 0, 0);
    UNIMPL();
}

void INIT_SPARK(void)
{
    // asm 00008593: 	CALL	PUSHALL
    // asm 00008594: 	LDI	@_MODE,R0
    // asm 00008595: 	AND	MMODE,R0
    // asm 00008596: 	CMPI	MATTR,R0
    // asm 00008597: 	BEQ	INIT_SPARKX
    // asm 00008598: 	CALL	TOO_MANY_SPARKS
    // asm 00008599: 	BC	INIT_SPARKX
    // asm 0000859A: 	LDI	@PLYPROC,AR7
    // asm 0000859B: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    // asm 0000859E: 	BC	INIT_SPARKX
    // asm 0000859F: 	LDI	AR0,AR7
    // asm 000085A0: 	STI	AR4,*+AR7(CAR_OBJ)
    // asm 000085A1: 	LDI	*+AR4(OCARBLK),AR5
    // asm 000085A2: 	STI	AR5,*+AR7(CAR_BLOCK)
    // asm 000085A3: 	LDI	0,R5
IS_LOOP:
    // asm 000085A4: 	LDI	@SPARKANII,AR0
    // asm 000085A5: 	LDI	*AR0,AR2
    // asm 000085A6: 	CALL	OBJ_GETE
    // asm 000085A7: 	BC	IS_LOOPX
    // asm 000085A8: 	LDI	*+AR0(OFLAGS),R0
    // asm 000085A9: 	OR	O_NOCOLL|O_POSTER,R0
    // asm 000085AA: 	STI	R0,*+AR0(OFLAGS)
    // asm 000085AB: 	LDI	AR0,AR4
    // asm 000085AC: 	CALL	ADD_RDDEBRIS
    // asm 000085AD: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 000085AE: 	STI	R0,*+AR4(OID)
    // asm 000085AF: 	LDI	R5,IR0
    // asm 000085B0: 	ADDI	SPARK_OBJS,IR0
    // asm 000085B1: 	STI	AR4,*+AR7(IR0)
    // asm 000085B2: 	LDI	*+AR7(CAR_OBJ),AR2	;Use the body of the cars matrix
    // asm 000085B3: 	CALL	GETCARBODY		;get the over rotation of the car body
    // asm 000085B4: 	LDI	AR0,AR2
    // asm 000085B5: 	LDI	*+AR7(CAR_OBJ),R2	;MATRIX = OVERROT+ROT of cars body
    // asm 000085B6: 	ADDI	OMATRIX,R2
    // asm 000085B7: 	LDI	AR4,R3
    // asm 000085B8: 	ADDI	OMATRIX,R3
    // asm 000085B9: 	CALL	CONCATMATV
    // asm 000085BA: 	LDF	*+AR5(CARXPLUS),R0
    // asm 000085BB: 	SUBF	*+AR5(CARXMINUS),R0
    // asm 000085BC: 	CALL	FRAND
    // asm 000085BD: 	ADDF	*+AR5(CARXMINUS),R0
    // asm 000085BE: 	LDF	R0,R1
    // asm 000085BF: 	LDI	*+AR7(CAR_OBJ),AR0
    // asm 000085C0: 	LDF	*+AR0(OPOSX),R0		;Set the start point of the spark
    // asm 000085C1: 	ADDF	R1,R0
    // asm 000085C2: 	STF	R0,*+AR4(OPOSX)
    // asm 000085C3: 	LDF	*+AR0(OPOSY),R0
    // asm 000085C4: 	STF	R0,*+AR4(OPOSY)
    // asm 000085C5: 	LDF	*+AR0(OPOSZ),R0
    // asm 000085C6: 	STF	R0,*+AR4(OPOSZ)
    // asm 000085C7: 	FLOAT	180,R0
    // asm 000085C8: 	STF	R0,*+AR4(OVELY)		;bottom of car
    // asm 000085C9: 	LDF	*+AR5(CARZMINUS),R0	;BACK
    // asm 000085CA: 	STF	R0,*+AR4(OVELZ)
    // asm 000085CB: 	LDI	AR4,AR2
    // asm 000085CC: 	LDF	1,R1			;absolute move(note frame rate dependant)
    // asm 000085CD: 	CALL	OBJ_MOVE		;Offset the starting point
    // asm 000085CE: 	LDI	@SPARKANII,R0
    // asm 000085CF: 	STI	R0,*+AR4(OVELZ)
    // asm 000085D0: 	LDF	0,R0
    // asm 000085D1: 	STF	R0,*+AR4(OVELY)
    // asm 000085D2: 	LDI	AR4,AR2
    // asm 000085D3: 	CALL	OBJ_INSERT
    // asm 000085D4: 	ADDI	1,R5
    // asm 000085D5: 	CMPI	6,R5
    // asm 000085D6: 	BLT	IS_LOOP
IS_LOOPX:
    // asm 000085D7: 	CMPI	0,R5
    // asm 000085D8: 	BEQ	INIT_SPARK_KILL
    // asm 000085D9: 	STI	R5,*+AR7(NUM_SPARKS)
INIT_SPARKX:
    // asm 000085DA: 	CALL	POPALL
    // asm 000085DB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_SPARK", 0, 0);
    UNIMPL();
}

void REPLICATE_SPARK(void)
{
    // asm 000085DC: 	LDI	*+AR7(NUM_SPARKS),R0
    // asm 000085DD: 	CMPI	4,R0
    // asm 000085DE: 	BGE	REPSPARKX
    // asm 000085DF: 	LDI	AR7,AR0
    // asm 000085E0: 	ADDI	SPARK_OBJS,AR0
    // asm 000085E1: 	LDI	0,R0
FIND_LAST_SPARKLP:
    // asm 000085E2: 	LDI	*AR0++,R1		;This loop will leave AR4 = to last NONZERO SPARK
    // asm 000085E3: 	LDINE	R1,AR4
    // asm 000085E4: 	ADDI	1,R0
    // asm 000085E5: 	CMPI	*+AR7(NUM_SPARKS),R0
    // asm 000085E6: 	BLT	FIND_LAST_SPARKLP
    // asm 000085E7: 	LDI	@SPARKANII,AR2
    // asm 000085E8: 	LDI	*AR2,AR2
    // asm 000085E9: 	CALL	OBJ_GETE
    // asm 000085EA: 	BC	REPSPARKX
    // asm 000085EB: 	LDI	*+AR0(OFLAGS),R0
    // asm 000085EC: 	OR	O_NOCOLL|O_POSTER,R0
    // asm 000085ED: 	STI	R0,*+AR0(OFLAGS)
    // asm 000085EE: 	LDI	@MATRIXAI,AR2
    // asm 000085EF: 	LDF	*+AR5(CARVROT),R2
    // asm 000085F0: 	CALL	FIND_YMATRIX
    // asm 000085F1: 	LDF	40,R0
    // asm 000085F2: 	CALL	SFRAND
    // asm 000085F3: 	STF	R0,*+AR0(OPOSX)
    // asm 000085F4: 	LDF	90,R0
    // asm 000085F5: 	CALL	SFRAND
    // asm 000085F6: 	STF	R0,*+AR0(OPOSY)
    // asm 000085F7: 	FLOAT	@NFRAMES,R0
    // asm 000085F8: 	MPYF	50,R0			;Constant speed less than the player is moving
    // asm 000085F9: 	MPYF	R7,R0			;NUMBER OF FRAMES SINCE LAST SPARK CREATED
    // asm 000085FA: 	NEGF	R0			;OFFSET FROM LAST SPARK
    // asm 000085FB: 	STF	R0,*+AR0(OPOSZ)
    // asm 000085FC: 	LDI	AR0,AR2
    // asm 000085FD: 	ADDI	OPOSX,AR2
    // asm 000085FE: 	LDI	AR2,R3
    // asm 000085FF: 	LDI	@MATRIXAI,R2
    // asm 00008600: 	CALL	MATRIX_MUL
    // asm 00008601: 	LDF	*+AR4(OPOSX),R0
    // asm 00008602: 	ADDF	*+AR0(OPOSX),R0
    // asm 00008603: 	STF	R0,*+AR0(OPOSX)
    // asm 00008604: 	LDF	*+AR4(OPOSY),R0
    // asm 00008605: 	ADDF	*+AR0(OPOSY),R0
    // asm 00008606: 	STF	R0,*+AR0(OPOSY)
    // asm 00008607: 	LDF	*+AR4(OPOSZ),R0
    // asm 00008608: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00008609: 	STF	R0,*+AR0(OPOSZ)
    // asm 0000860A: 	LDI	*+AR7(NUM_SPARKS),AR2
    // asm 0000860B: 	ADDI	SPARK_OBJS,AR2
    // asm 0000860C: 	ADDI	AR7,AR2
    // asm 0000860D: 	STI	AR0,*AR2
    // asm 0000860E: 	LDF	*+AR4(OVELY),R0
    // asm 0000860F: 	STF	R0,*+AR0(OVELY)
    // asm 00008610: 	LDI	*+AR7(NUM_SPARKS),R0
    // asm 00008611: 	ADDI	1,R0
    // asm 00008612: 	STI	R0,*+AR7(NUM_SPARKS)
    // asm 00008613: 	LDI	AR0,AR4
    // asm 00008614: 	CALL	ADD_RDDEBRIS
    // asm 00008615: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 00008616: 	STI	R0,*+AR4(OID)
    // asm 00008617: 	LDI	@SPARKANII,R0
    // asm 00008618: 	STI	R0,*+AR4(OVELZ)
    // asm 00008619: 	LDI	AR4,AR2
    // asm 0000861A: 	CALL	OBJ_INSERT
REPSPARKX:
    // asm 0000861B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REPLICATE_SPARK", 0, 0);
    UNIMPL();
}

/* *INPUT	PDATA = SETUP see equates at begining of file
*Maintains several spark animations
 */
void SPARK_PROC(void)
{
    // asm 0000861C: 	LDF	0,R7
    // asm 0000861D: 	LDI	*+AR7(CAR_BLOCK),AR5
SSANI_LOOP:
    // asm 0000861E: 	LDI	0,R5		;SPARK ON
    // asm 0000861F: 	LDI	0,R6		;NUMBER OF SPARKS STILL ACTIVE
SS1:
    // asm 00008620: 	LDI	R5,IR0
    // asm 00008621: 	ADDI	SPARK_OBJS,IR0
    // asm 00008622: 	LDI	*+AR7(IR0),AR4
    // asm 00008623: 	CMPI	0,AR4
    // asm 00008624: 	BEQ	NEXT_SPARK		;SPARK is no longer with us
    // asm 00008625: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY OFF LIST
    // asm 00008626: 	TSTB	O_LIST_M,R0
    // asm 00008627: 	BZ	SPARK_ANIX			;YES, DELETE FROM LIST
    // asm 00008628: 	LDI	*+AR4(OID),R0
    // asm 00008629: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 0000862A: 	BNE	SPARK_ANIX		;not a spark???!!!
    // asm 0000862B: 	LDI	*+AR4(OVELZ),AR6		;ANIMATION POINTER
    // asm 0000862C: 	LDI	*++AR6,R0
    // asm 0000862D: 	BN	SPARK_ANIX
    // asm 0000862E: 	STI	R0,*+AR4(OROMDATA)
    // asm 0000862F: 	STI	AR6,*+AR4(OVELZ)
    // asm 00008630: 	LDI	*+AR4(ODIST),R0
    // asm 00008631: 	CMPI	1000,R0
    // asm 00008632: 	BLE	SPARK_ANIX
    // asm 00008633: 	LDI	@MATRIXAI,AR2
    // asm 00008634: 	LDF	*+AR5(CARVROT),R2
    // asm 00008635: 	CALL	FIND_YMATRIX
    // asm 00008636: 	CALL	CLR_VECTORA
    // asm 00008637: 	FLOAT	@NFRAMES,R0
    // asm 00008638: 	MPYF	*+AR5(CARSPEED),R0
    // asm 00008639: 	MPYF	1.5,R0
    // asm 0000863A: 	SUBF	100,R0		;Constant speed less than the player is moving
    // asm 0000863B: 	STF	R0,*+AR2(Z)
    // asm 0000863C: 	LDI	@MATRIXAI,R2
    // asm 0000863D: 	LDI	AR2,R3
    // asm 0000863E: 	CALL	MATRIX_MUL
    // asm 0000863F: 	LDI	@VECTORAI,AR1
    // asm 00008640: 	LDF	*+AR1(X),R0
    // asm 00008641: 	ADDF	*+AR4(OPOSX),R0
    // asm 00008642: 	STF	R0,*+AR4(OPOSX)
    // asm 00008643: 	LDF	*+AR1(Z),R0
    // asm 00008644: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00008645: 	STF	R0,*+AR4(OPOSZ)
    // asm 00008646: 	PUSH	AR4
    // asm 00008647: 	PUSH	IR0
    // asm 00008648: 	PUSH	R5
    // asm 00008649: 	ADDI	OPOSX,AR4
    // asm 0000864A: 	CALL	CAMSCAN
    // asm 0000864B: 	POP	R5
    // asm 0000864C: 	POP	IR0
    // asm 0000864D: 	POP	AR4
    // asm 0000864E: 	BNC	SPARK_ANIX	;This spark is out of this world
    // asm 0000864F: 	SUBF	*+AR4(OVELY),R0		;SPARKS OFFSET FROM THE GROUND
    // asm 00008650: 	ADDF	*+AR4(OPOSY),R0
    // asm 00008651: 	STF	R0,*+AR4(OPOSY)
    // asm 00008652: 	ADDI	1,R6
NEXT_SPARK:
    // asm 00008653: 	ADDI	1,R5
    // asm 00008654: 	CMPI	*+AR7(NUM_SPARKS),R5
    // asm 00008655: 	BLT	SS1
    // asm 00008656: 	CMPI	0,R6
    // asm 00008657: 	BEQ	SPARK_DIE
    // asm 00008658: 	SLEEP	1
    // asm 0000865A: 	ADDF	1,R7
    // asm 0000865B: 	CMPF	2,R7			;WAIT NFRAMES
    // asm 0000865C: 	BNE	KLUDGE_MOFO
    // asm 0000865D: 	CALL	REPLICATE_SPARK		;Will replicate the latest spark still active
    // asm 0000865E: 	LDF	0,R7
KLUDGE_MOFO:
    // asm 0000865F: 	BR	SSANI_LOOP
SPARK_DIE:
    // asm 00008660: 	DIE
SPARK_ANIX:
    // asm 00008661: 	LDI	R5,IR0
    // asm 00008662: 	ADDI	SPARK_OBJS,IR0
    // asm 00008663: 	LDI	*+AR7(IR0),AR2
    // asm 00008664: 	LDI	0,R0
    // asm 00008665: 	STI	R0,*+AR7(IR0)		;COLOR THIS SPARK GONE
    // asm 00008666: 	LDI	*+AR2(OFLAGS),R0	;CHECK IF ALREADY OFF LIST
    // asm 00008667: 	TSTB	O_LIST_M,R0
    // asm 00008668: 	BZ	NO_OBJ
    // asm 00008669: 	LDI	*+AR2(OID),R0
    // asm 0000866A: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 0000866B: 	BNE	NO_OBJ			;not a spark???!!!
    // asm 0000866C: 	CALL	OBJ_DELETE
NO_OBJ:
    // asm 0000866D: 	BR	NEXT_SPARK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPARK_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
* INIT_COLLA_OBJS
*INPUT	AR0 points to proc memory
*Creates several spark animations
 */
void INIT_COLLA_OBJS(void)
{
    // asm 0000866E: 	LDI	0,R5
ICO_LOOP:
    // asm 0000866F: 	LDI	@SPARKANII,AR0
    // asm 00008670: 	LDI	*AR0,AR2
    // asm 00008671: 	CALL	OBJ_GETE
    // asm 00008672: 	BC	ICO_LOOPX		;out of objects
    // asm 00008673: 	LDI	AR0,AR4
    // asm 00008674: 	LDI	*+AR4(OFLAGS),R0
    // asm 00008675: 	OR	O_NOCOLL|O_POSTER,R0
    // asm 00008676: 	STI	R0,*+AR4(OFLAGS)
    // asm 00008677: 	CALL	ADD_RDDEBRIS
    // asm 00008678: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 00008679: 	STI	R0,*+AR4(OID)
    // asm 0000867A: 	LDI	R5,IR0
    // asm 0000867B: 	ADDI	SPARK_OBJS,IR0
    // asm 0000867C: 	STI	AR4,*+AR7(IR0)
    // asm 0000867D: 	FLOAT	60,R0			;calculate the random offset that this obj will have
    // asm 0000867E: 	CALL	SFRAND
    // asm 0000867F: 	LDF	R0,R1
    // asm 00008680: 	FLOAT	-60,R0
    // asm 00008681: 	CALL	FRAND
    // asm 00008682: 	ADDF	20,R0
    // asm 00008683: 	LDF	R0,R2
    // asm 00008684: 	FLOAT	60,R0
    // asm 00008685: 	CALL	SFRAND
    // asm 00008686: 	LDF	R0,R3
    // asm 00008687: 	LDI	*+AR7(CAR_OBJ),AR0
    // asm 00008688: 	LDF	*+AR7(COLL_X),R0		;Set the start point of the spark
    // asm 00008689: 	ADDF	R1,R0
    // asm 0000868A: 	ADDF	*+AR0(OPOSX),R0
    // asm 0000868B: 	STF	R0,*+AR4(OPOSX)
    // asm 0000868C: 	LDF	*+AR7(COLL_Y),R0
    // asm 0000868D: 	ADDF	R2,R0
    // asm 0000868E: 	ADDF	-30,R0
    // asm 0000868F: 	NEGF	R0,R1
    // asm 00008690: 	ADDF	180,R1				;AVERAGE car hight from road
    // asm 00008691: 	STF	R1,*+AR4(OVELY)			;This will be used as the y offset
    // asm 00008692: 	ADDF	*+AR0(OPOSY),R0
    // asm 00008693: 	STF	R0,*+AR4(OPOSY)
    // asm 00008694: 	LDF	*+AR7(COLL_Z),R0
    // asm 00008695: 	ADDF	R3,R0
    // asm 00008696: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00008697: 	STF	R0,*+AR4(OPOSZ)
    // asm 00008698: 	LDI	@SPARKANII,R0
    // asm 00008699: 	STI	R0,*+AR4(OVELZ)
    // asm 0000869A: 	LDI	AR4,AR2
    // asm 0000869B: 	CALL	OBJ_INSERT
    // asm 0000869C: 	ADDI	1,R5
    // asm 0000869D: 	CMPI	2,R5
    // asm 0000869E: 	BLT	ICO_LOOP
ICO_LOOPX:
    // asm 0000869F: 	CMPI	0,R5
    // asm 000086A0: 	BEQ	INIT_SPARK_KILL
    // asm 000086A1: 	STI	R5,*+AR7(NUM_SPARKS)
    // asm 000086A2: 	RETS
INIT_SPARK_KILL:
    // asm 000086A3: 	LDI	AR7,AR2
    // asm 000086A4: 	LDI	0,AR7		;Stupid thing thinks I'm commiting suicide!
    // asm 000086A5: 	CALL	PRC_KILL
    // asm 000086A6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_COLLA_OBJS", 0, 0);
    UNIMPL();
}

void WALL_SPARK(void)
{
    // asm 000086A7: 	CALL	PUSHALL
    // asm 000086A8: 	LDI	@_MODE,R0
    // asm 000086A9: 	AND	MMODE,R0
    // asm 000086AA: 	CMPI	MATTR,R0
    // asm 000086AB: 	BEQ	WALL_SPARKX
    // asm 000086AC: 	CMPI	@PLYCAR,AR4
    // asm 000086AD: 	BNE	WALL_SPARKX		;Only work for the players car
    // asm 000086AE: 	LDI	*+AR5(CARTRAK),R0
    // asm 000086AF: 	BZ	WALL_SPARKX		;DUDE IS NOT ON THE ROAD
    // asm 000086B0: 	CALL	TOO_MANY_SPARKS
    // asm 000086B1: 	BC	WALL_SPARKX
    // asm 000086B2: 	LDI	@PLYPROC,AR7
    // asm 000086B3: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    // asm 000086B6: 	BC	WALL_SPARKX
    // asm 000086B7: 	LDI	AR0,AR7
    // asm 000086B8: 	STI	AR4,*+AR7(CAR_OBJ)
    // asm 000086B9: 	STI	AR5,*+AR7(CAR_BLOCK)
    // asm 000086BA: 	LDF	*+AR5(CARYROT),R2
    // asm 000086BB: 	LDI	@MATRIXAI,AR2
    // asm 000086BC: 	CALL	FIND_YMATRIX
    // ;find the angle of the car reletive to the track it is on
    // asm 000086BD: 	LDI	*+AR5(CARTRAK),R0
    // asm 000086BE: 	LDI	R0,AR2
    // asm 000086BF: 	CALL	GETRDIR
    // asm 000086C0: 	LDF	R0,R2
    // ;	LDF	*+AR5(CARVROT),R2
    // asm 000086C1: 	CALL	NORMITS
    // asm 000086C2: 	LDF	R2,R0
    // asm 000086C3: 	LDF	*+AR5(CARYROT),R2
    // asm 000086C4: 	CALL	NORMITS
    // asm 000086C5: 	SUBF	R0,R2
    // asm 000086C6: 	CALL	NORMITS
    // ;find the side of the road the car is on
    // asm 000086C7: 	LDF	*+AR5(CARDIST2CNTR),R0		;- = right side ;+ = left side
    // ;determine which corner of the car hit the wall
    // asm 000086C8: 	ABSF	R2,R1
    // asm 000086C9: 	CMPF	HALFPI,R1
    // asm 000086CA: 	BLT	FACINGFRONT
    // asm 000086CB: 	NEGF	R0
    // asm 000086CC: 	NEGF	R2
FACINGFRONT:
    // asm 000086CD: 	CMPF	0,R0			;which side is the wall?
    // asm 000086CE: 	BGT	LEFT_SIDE
    // asm 000086CF: RIGHT_SIDE
    // asm 000086CF: 	LDF	*+AR5(CARXPLUS),R1
    // asm 000086D0: 	CMPF	0,R2
    // asm 000086D1: 	LDFGT	*+AR5(CARZMINUS),R0	;BACK
    // asm 000086D2: 	LDFLE	*+AR5(CARZPLUS),R0	;FRONT
    // asm 000086D3: 	BR	WALLS1
LEFT_SIDE:
    // asm 000086D4: 	LDF	*+AR5(CARXMINUS),R1
    // asm 000086D5: 	CMPF	0,R2
    // asm 000086D6: 	LDFGT	*+AR5(CARZPLUS),R0	;FRONT
    // asm 000086D7: 	LDFLE	*+AR5(CARZMINUS),R0	;BACK
WALLS1:
    // ;get the coords of that corner
    // ;transform them based on the body of the car
    // asm 000086D8: 	STF	R1,*+AR7(COLL_X)
    // asm 000086D9: 	STF	R0,*+AR7(COLL_Z)
    // asm 000086DA: 	LDF	-90,R0
    // asm 000086DB: 	STF	R0,*+AR7(COLL_Y)
    // asm 000086DC: 	LDI	AR7,AR2
    // asm 000086DD: 	ADDI	COLL_X,AR2
    // asm 000086DE: 	LDI	AR2,R3
    // asm 000086DF: 	LDI	@MATRIXAI,R2
    // asm 000086E0: 	CALL	MATRIX_MUL
    // ;no go and make the spark objects
    // asm 000086E1: 	CALL	INIT_COLLA_OBJS
WALL_SPARKX:
    // asm 000086E2: 	CALL	POPALL
    // asm 000086E3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WALL_SPARK", 0, 0);
    UNIMPL();
}

void IMPACT_SPARK(void)
{
    // asm 000086E4: 	CALL	PUSHALL
    // asm 000086E5: 	LDI	@_MODE,R0
    // asm 000086E6: 	AND	MMODE,R0
    // asm 000086E7: 	CMPI	MATTR,R0
    // asm 000086E8: 	BEQ	IMPACT_SPARKX
    // asm 000086E9: 	CMPI	@PLYCAR,AR0
    // asm 000086EA: 	LDIEQ	AR0,AR4
    // asm 000086EB: 	BEQ	IMPACTED_PLAYER
    // asm 000086EC: 	CMPI	@PLYCAR,AR1
    // asm 000086ED: 	LDIEQ	AR1,AR4
    // asm 000086EE: 	BNE	IMPACT_SPARKX		;Only work for the players car
IMPACTED_PLAYER:
    // asm 000086EF: 	LDI	*+AR4(OCARBLK),AR5
    // asm 000086F0: 	LDI	*+AR5(CARTRAK),R0
    // asm 000086F1: 	BZ	IMPACT_SPARKX		;DUDE IS NOT ON THE ROAD
    // asm 000086F2: 	CALL	TOO_MANY_SPARKS
    // asm 000086F3: 	BC	IMPACT_SPARKX
    // asm 000086F4: 	LDI	@PLYPROC,AR7
    // asm 000086F5: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    // asm 000086F8: 	BC	IMPACT_SPARKX
    // asm 000086F9: 	LDI	AR0,AR7
    // asm 000086FA: 	STI	AR4,*+AR7(CAR_OBJ)
    // asm 000086FB: 	STI	AR5,*+AR7(CAR_BLOCK)
    // asm 000086FC: 	LDF	*-AR3(1),R0
    // asm 000086FD: 	SUBF	*+AR4(OPOSX),R0		;Make offset from the car
    // asm 000086FE: 	STF	R0,*+AR7(COLL_X)
    // ;	LDF	*AR3,R0
    // ;	SUBF	60,R0
    // ;	SUBF	*+AR4(OPOSY),R0		;Make offset from the car
    // asm 000086FF: 	LDF	-80,R0
    // asm 00008700: 	STF	R0,*+AR7(COLL_Y)
    // asm 00008701: 	LDF	*+AR3(1),R0
    // asm 00008702: 	SUBF	*+AR4(OPOSZ),R0		;Make offset from the car
    // asm 00008703: 	STF	R0,*+AR0(COLL_Z)
    // asm 00008704: 	CALL	INIT_COLLA_OBJS
IMPACT_SPARKX:
    // asm 00008705: 	CALL	POPALL
    // asm 00008706: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "IMPACT_SPARK", 0, 0);
    UNIMPL();
}

void ROAD_IMPACT_SPARK(void)
{
    // asm 00008707: 	RETS
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

/* *----------------------------------------------------------------------------
*INPUT	AR4 = CARS OBJECT
*	AR5 = CARBLK
 */
void SKID_SPARK(void)
{
    // asm 00008708: 	RETS
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

/* *----------------------------------------------------------------------------
*RETURNS	R2= number of spark procs
*		C=1 if too many
*		C=0 if ok
 */
void TOO_MANY_SPARKS(void)
{
    // asm 00008709: 	LDI	0,R2
    // asm 0000870A: 	LDI	@PACTIVEI,R0
    // asm 0000870B: 	BZ	TMSXCC			;NULL LIST?
TMS_LP:
    // asm 0000870C: 	LDI	R0,AR0
    // asm 0000870D: 	LDI	*+AR0(PID),R0
    // asm 0000870E: 	TSTB	UTIL_C,R0
    // asm 0000870F: 	BZ	TMSLPE
    // asm 00008710: 	AND	TYPE_M,R0
    // asm 00008711: 	CMPI	SPARK_T,R0
    // asm 00008712: 	BNE	TMSLPE
    // asm 00008713: 	ADDI	1,R2
TMSLPE:
    // asm 00008714: 	LDI	*AR0,R0
    // asm 00008715: 	BNZ	TMS_LP
    // ;	CMPI	5,R2
    // asm 00008716: 	CMPI	2,R2
    // asm 00008717: 	BLT	TMSXCC
    // asm 00008718: 	SETC
    // asm 00008719: 	RETS
TMSXCC:
    // asm 0000871A: 	CLRC
    // asm 0000871B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOO_MANY_SPARKS", 0, 0);
    UNIMPL();
}

void OBJ_MOVE(void)
{
    // asm 0000871C: 	LDI	AR2,AR1
    // asm 0000871D: 	ADDI	OVELX,AR2
    // asm 0000871E: 	LDI	AR1,R2
    // asm 0000871F: 	ADDI	OMATRIX,R2
    // asm 00008720: 	LDI	@VECTORAI,R3
    // asm 00008721: 	CALL	MATRIX_MUL
    // asm 00008722: 	LDI	@VECTORAI,AR0
    // asm 00008723: 	LDF	*+AR0(X),R0
    // asm 00008724: 	MPYF	R1,R0
    // asm 00008725: 	ADDF	*+AR1(OPOSX),R0
    // asm 00008726: 	STF	R0,*+AR1(OPOSX)
    // asm 00008727: 	LDF	*+AR0(Y),R0
    // asm 00008728: 	MPYF	R1,R0
    // asm 00008729: 	ADDF	*+AR1(OPOSY),R0
    // asm 0000872A: 	STF	R0,*+AR1(OPOSY)
    // asm 0000872B: 	LDF	*+AR0(Z),R0
    // asm 0000872C: 	MPYF	R1,R0
    // asm 0000872D: 	ADDF	*+AR1(OPOSZ),R0
    // asm 0000872E: 	STF	R0,*+AR1(OPOSZ)
    // asm 0000872F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_MOVE", 0, 0);
    UNIMPL();
}

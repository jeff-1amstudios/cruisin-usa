#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/obj.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/delta.h"
#include "../include/chopper.h"

/*
 * Source module: asm/CHOPPER.ASM
 */

// *----------------------------------------------------------------------------
// *CHOPPER IN THE SKY
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: HELI_ABORT	.bss	HELI_ABORT,1 */
int HELI_ABORT;
// *
// *	!!!!NO JSRPing!!!!
// *
// *LOCALS  (AFTER DELTA EQUATES)
#define CD_ANIPROC (PDATA+18)
#define CD_MODE (PDATA+19)
#define CD_ACC (PDATA+20)
#define CD_SPEED (PDATA+21)
#define CD_BOMBTIK (PDATA+22)
#define CD_TSPEED (PDATA+23)
#define CD_TSPEED_OLD (PDATA+24)
#define CD_FLYTDIR (PDATA+25)
#define CD_DHEIGHT (PDATA+26)
#define CD_ODHEIGHT (PDATA+27)
#define CD_CLOSEROAD (PDATA+28)
#define CD_AHEADP (PDATA+29)
#define CD_PASS_COUNT (PDATA+30)
#define CD_BOMB_COUNT (PDATA+31)
#define CD_MAX_PASSES (PDATA+32)
#define CD_MAX_BOMBS (PDATA+33)
#define CD_LASTPASS (PDATA+34)
#define CD_DOATTACK (PDATA+35)
// *NO MORE UNLESS NOT JSRPing
// *CHOPPER MODE
#define CM_CU 1
#define CM_DB 2
#define CM_FA 3
#define CRADZ OUSR1
#define MAX_SPEED 667
// *----------------------------------------------------------------------------
/* asm: CHOPPERDYNA	CHOPPERDYNA */
/* asm: 	.word	1		;#OF DYNAS-1 */
/* asm: 	.float	0,-206,14	;blades */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	0,0,0		;main body */
/* asm: 	.word	94		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	*---------------------------------------------------------------------------- */
/* asm: 	*--------------------------------------------------------------------------- */
/* asm: 	* */
/* asm: 	*go straight for the plyrs car */
/* asm: 	* */
/* asm: 	*(already close by) */
/* asm: 	*branched to from oncoming buzz */
/* asm: 	* */
/* asm: 	* */
int CHOPPERDYNA[] = {
    1, // #OF DYNAS-1
    0, -206, 14, // blades
    3, // VERTS-1
    1, // DYNAFLAG
    0, 0, 0, // main body
    94, // VERTS-1
    1, // DYNAFLAG
};
// *----------------------------------------------------------------------------

void DIRECT_ATTACK(void)
{
    // 	;
    // 	;DEBUG	only attack player when he is in the first position
    // 	;
    // asm: 	LDI	@POSITION,R0
    // asm: 	CMPI	1,R0
    // asm: 	BNE	FLYAWAY
    // 	;
    // 	;
    // asm: 	CALL	DIST_TO_PLYR
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm: 	CALL	HELI_SND
    // 	;see if we should track the next piece
CHECK_DIST28:
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	RS	8,R0
    // asm: 	LDI	@SECTIONIDX,R1
    // asm: 	SUBPI	@DGROUP_COUNT,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BLE	FLYAWAY		;CHOPPER_DIE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OBLINK4),R0
    // asm: 	BZ	FLYAWAY	;CHOPPER_DIE
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	LDF	*+AR2(CARDIST2CNTR),R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm: 	CALL	GET_TRACK_POS_RVS_XLANE		;CHECK IF WE SHOULD ADVANCE
    // asm: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	THIS_PIECE28
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OBLINK4),R0
    // asm: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm: 	BU	CHECK_DIST28
THIS_PIECE28:
    // asm: 	FLOATP	@NFRAMES,R1
    // asm: 	MPYF	*+AR7(CD_TSPEED),R1
    // asm: 	LDFLE	30,R1			;if 0 or less assume 30 voxel per frame
    // asm: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm: 	FIX	R0,R7
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDP	@_VECTORA		;lane position
    // asm: 	LDF	*+AR2(OPOSX),R2		;X
    // asm: 	SUBF	*+AR4(OPOSX),R2
    // asm: 	ADDF	@_VECTORA+X,R2
    // asm: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm: 	SUBF	*+AR4(OPOSZ),R3
    // asm: 	ADDF	@_VECTORA+Z,R3
    // asm: 	SETDP
    // 	;find the theta delta to this position
    // 	;
    // asm: 	CALL	ARCTANF			;-> R0
    // asm: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm: 	SUBF	1,R1
    // asm: 	BLE	NODIV8
    // asm: 	CALL	DIV_F			;-> R0
NODIV8:
    // asm: 	CMPF	0.3926,R0		;PI/8 (maximum turning radius)
    // asm: 	LDFGT	0.3926,R0
    // asm: 	CMPF	-0.3926,R0
    // asm: 	LDFLT	-0.3926,R0
    // asm: 	STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm: 	PUSH	AR4
    // asm: 	ADDI	OPOSX,AR4
    // asm: 	CALL	CAMSCAN
    // asm: 	POP	AR4
    // asm: 	BNC	LLK28
    // asm: 	NEGF	R0
    // asm: 	FLOAT	-300,R1
    // asm: 	SUBF	R1,R0
    // asm: 	NEGF	R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR7(CD_DHEIGHT)
LLK28:
    // asm: 	LDF	*+AR7(CD_SPEED),R0
    // asm: 	ADDF	*+AR7(CD_ACC),R0
    // asm: 	FLOAT	MAX_SPEED,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFGT	R1,R0
    // asm: 	STF	R0,*+AR7(CD_SPEED)
    // asm: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm: 	LDF	*+AR7(CD_SPEED),R3
    // asm: 	CALL	FSL_MOVE
    // asm: 	SLEEP	1
    // asm: 	BU	DIRECT_ATTACK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIRECT_ATTACK", 0, 0);
    UNIMPL();
}

void ONCOMMING_BUZZ(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *Take the helicopter, and buzz (@ 200 MPH) right over the players car
    // *exit when start of the world is hit
    // *
    // *
    // *
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR4(ORADX)
    // asm: 	STF	R0,*+AR4(ORADY)
    // asm: 	STF	R0,*+AR4(CRADZ)
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	STF	R0,*+AR4(OVELY)
    // asm: 	STF	R0,*+AR4(OVELZ)
    // 	;
    // 	;find where player is
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	LDF	*+AR2(CARDIST2CNTR),R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm: 	LDI	@DYNALIST_END,AR2
    // asm: 	LDI	*+AR2(OBLINK4),AR2
    // asm: 	LDI	*+AR2(OBLINK4),AR2
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm: 	CALL	SUB_FUNCTION_RVS_XLANE		;MATRIXA,VECTORA,R2
    // asm: 	LDP	@_VECTORA
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	ADDF	@_VECTORA+X,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR2(OPOSY),R0
    // asm: 	FLOAT	1000,R1
    // asm: 	SUBF	R1,R0
    // asm: 	ADDF	@_VECTORA+Y,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	ADDF	@_VECTORA+Z,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	SETDP
    // 	;initialize Ytheta to the intentional direction
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
ONBUZZ_L:
    // asm: 	CALL	DIST_TO_PLYR
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm: 	FLOAT	22000,R2
    // asm: 	CMPF	R2,R0
    // asm: 	BLE	FLYAWAY
    // asm: 	CALL	HELI_SND
    // 	;see if we should track the next piece
CHECK_DIST2:
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	RS	8,R0
    // asm: 	LDI	@SECTIONIDX,R1
    // asm: 	SUBPI	@DGROUP_COUNT,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BLE	CHOPPER_DIE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OBLINK4),R0
    // asm: 	BZ	CHOPPER_DIE
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	LDF	*+AR2(CARDIST2CNTR),R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm: 	CALL	GET_TRACK_POS_RVS_XLANE		;CHECK IF WE SHOULD ADVANCE
    // asm: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	THIS_PIECE2
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OBLINK4),R0
    // asm: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm: 	BU	CHECK_DIST2
THIS_PIECE2:
    // asm: 	FLOATP	@NFRAMES,R1
    // asm: 	MPYF	*+AR7(CD_TSPEED),R1
    // asm: 	LDFLE	30,R1			;if 0 or less assume 30 voxel per frame
    // asm: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm: 	FIX	R0,R7
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDP	@_VECTORA		;lane position
    // asm: 	LDF	*+AR2(OPOSX),R2		;X
    // asm: 	SUBF	*+AR4(OPOSX),R2
    // asm: 	ADDF	@_VECTORA+X,R2
    // asm: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm: 	SUBF	*+AR4(OPOSZ),R3
    // asm: 	ADDF	@_VECTORA+Z,R3
    // asm: 	SETDP
    // 	;find the theta delta to this position
    // 	;
    // asm: 	CALL	ARCTANF			;-> R0
    // asm: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm: 	SUBF	1,R1
    // asm: 	BLE	NODIV
    // asm: 	CALL	DIV_F			;-> R0
NODIV:
    // asm: 	CMPF	0.3926,R0		;PI/8 (maximum turning radius)
    // asm: 	LDFGT	0.3926,R0
    // asm: 	CMPF	-0.3926,R0
    // asm: 	LDFLT	-0.3926,R0
    // asm: 	STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm: 	PUSH	AR4
    // asm: 	ADDI	OPOSX,AR4
    // asm: 	CALL	CAMSCAN
    // asm: 	POP	AR4
    // asm: 	BNC	LLK2
    // asm: 	NEGF	R0
    // asm: 	FLOAT	-1300,R1
    // asm: 	SUBF	R1,R0
    // asm: 	NEGF	R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR7(CD_DHEIGHT)
LLK2:
    // asm: 	LDF	*+AR7(CD_SPEED),R0
    // asm: 	ADDF	*+AR7(CD_ACC),R0
    // asm: 	FLOAT	MAX_SPEED,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFGT	R1,R0
    // asm: 	STF	R0,*+AR7(CD_SPEED)
    // asm: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm: 	LDF	*+AR7(CD_SPEED),R3
    // asm: 	CALL	FSL_MOVE
    // asm: 	SLEEP	1
    // asm: 	BU	ONBUZZ_L
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ONCOMMING_BUZZ", 0, 0);
    UNIMPL();
}

void CHOPPER(void)
{
    // *----------------------------------------------------------------------------
    // ;
    // ;*----------------------------------------------------------------------------
    // ;*
    // ;*
    // ;*
    // ;*
    // ;CIRCLE_BUZZ:
    // ;	LDF	0,R0
    // ;	STF	R0,*+AR4(ORADX)
    // ;	STF	R0,*+AR4(ORADY)
    // ;	STF	R0,*+AR4(CRADZ)
    // ;
    // ;	STF	R0,*+AR4(OVELX)
    // ;	STF	R0,*+AR4(OVELY)
    // ;	STF	R0,*+AR4(OVELZ)
    // ;
    // ;
    // ;	;
    // ;	;find where player is
    // ;
    // ;	LDI	@PLYCBLK,AR2
    // ;	NEGF	*+AR2(CARDIST2CNTR),R0
    // ;	STF	R0,*+AR7(DELTA_XLANE)
    // ;	LDI	*+AR2(CARTRAK),AR2
    // ;	LDI	*+AR2(OBLINK4),AR2
    // ;
    // ;	STI	AR2,*+AR7(DELTA_TPIECE)
    // ;	LDI	*+AR2(OUSR1),R0
    // ;	STI	R0,*+AR7(DELTA_LAST_OID)
    // ;	CALL	DELTA_SUB_FUNCTION			;MATRIXA,VECTORA,R2
    // ;
    // ;	LDP	@_VECTORA
    // ;	LDF	*+AR2(OPOSX),R0
    // ;	ADDF	@_VECTORA+X,R0
    // ;	STF	R0,*+AR4(OPOSX)
    // ;	LDF	*+AR2(OPOSY),R0
    // ;	FLOAT	1000,R1
    // ;	SUBF	R1,R0
    // ;	ADDF	@_VECTORA+Y,R0
    // ;	STF	R0,*+AR4(OPOSY)
    // ;	LDF	*+AR2(OPOSZ),R0
    // ;	ADDF	@_VECTORA+Z,R0
    // ;	STF	R0,*+AR4(OPOSZ)
    // ;	SETDP
    // ;
    // ;
    // ;	;initialize Ytheta to the intentional direction
    // ;	STF	R2,*+AR4(ORADY)
    // ;	STF	R2,*+AR7(DELTA_RADYDELTA)
    // ;
    // ;	LDI	AR4,AR2
    // ;	ADDI	OMATRIX,AR2
    // ;	CALL	FIND_YMATRIX
    // ;
    // ;
    // ;CIRCLE_JOIN
    // ;	CLRF	R6
    // ;	LDI	@CIRCLE_SI,AR5
    // ;
    // ;CIRCL_L
    // ;	LDI	@PLYCBLK,AR0
    // ;	LDF	*+AR0(CARSPEED),R0
    // ;	CMPF	40,R0
    // ;	BLT	ISOK55
    // ;	RETP
    // ;ISOK55
    // ;
    // ;
    // ;	CALL	DIST_TO_PLYR
    // ;	LDF	*+AR7(DELTA_PLYRDIST),R1
    // ;	STF	R0,*+AR7(DELTA_PLYRDIST)
    // ;	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // ;
    // ;	CALL	HELI_SND
    // ;
    // ;	.data
    // ;CIRCLE_SI	.word	CIRCLE_S
    // ;CIRCLE_S ;	X,Z
    // ;	.float	4000,2200
    // ;	.float	2500,5000
    // ;	.float	0,9000
    // ;	.float	-2500,5000
    // ;	.float	-4000,2200
    // ;	.float	-1
    // ;	.text
    // ;
    // ;
    // ;JKKU
    // ;	LDI	@PLYCBLK,AR2
    // ;	LDI	*+AR2(CARTRAK),AR2
    // ;	LDI	*+AR2(OLINK4),AR0
    // ;	LDF	*+AR0(OPOSX),R2
    // ;	SUBF	*+AR2(OPOSX),R2
    // ;	LDF	*+AR0(OPOSZ),R3
    // ;	SUBF	*+AR2(OPOSZ),R3
    // ;	CALL	ARCTANF
    // ;	SUBF	HALFPI,R0
    // ;	LDF	R0,R2				;FIND THETA
    // ;	LDI	@MATRIXAI,AR2
    // ;	CALL	FIND_YMATRIX
    // ;
    // ;
    // ;	CALL	CLR_VECTORA
    // ;	LDF	*AR5,R4
    // ;	LDF	*+AR5(1),R5
    // ;	STF	R4,*+AR2(X)
    // ;	STF	R5,*+AR2(Z)
    // ;
    // ;
    // ;	LDI	@MATRIXAI,R2
    // ;	LDI	AR2,R3
    // ;	CALL	MATRIX_MUL
    // ;
    // ;
    // ;	LDI	@PLYCAR,AR3
    // ;	LDF	*+AR3(OPOSX),R4
    // ;	ADDF	*+AR2(X),R4
    // ;	LDF	*+AR3(OPOSZ),R5
    // ;	ADDF	*+AR2(Z),R5		;R4,R5 contains X/Z position
    // ;
    // ;
    // ;	LDF	*+AR4(OPOSX),R2
    // ;	SUBF	R4,R2
    // ;	MPYF	R2,R2
    // ;	LDF	*+AR4(OPOSZ),R1
    // ;	SUBF	R5,R1
    // ;	ADDF	R1,R2
    // ;	CALL	SQRT			;R0 <- distance to new position
    // ;
    // ;
    // ;	FLOAT	2000,R1
    // ;	CMPF	R1,R0
    // ;	BGT	KKK
    // ;	LDF	*++AR5(2),R0
    // ;	CMPF	-1,R0
    // ;	BNE	JKKU
    // ;	LDI	@CIRCLE_SI,AR5
    // ;	BU	JKKU
    // ;KKK
    // ;
    // ;
    // ;
    // ;	PUSHF	R4
    // ;	PUSHF	R5
    // ;
    // ;	FLOAT	@NFRAMES,R1
    // ;	MPYF	*+AR7(CD_TSPEED),R1
    // ;	LDFLE	30,R1			;if 0 or less assume 30 voxel per frame
    // ;	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // ;	FIX	R0,R7
    // ;
    // ;	POPF	R5
    // ;	POPF	R4
    // ;
    // ;	LDF	R4,R2			;X
    // ;	SUBF	*+AR4(OPOSX),R2
    // ;	LDF	R5,R3			;Z
    // ;	SUBF	*+AR4(OPOSZ),R3
    // ;
    // ;
    // ;
    // ;	;find the theta delta to this position
    // ;	;
    // ;	CALL	ARCTANF			;-> R0
    // ;	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // ;
    // ; 	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // ;	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // ;	FLOAT	R7,R1			;theta / number of turns to achieve
    // ;	SUBF	1,R1
    // ;	BLE	NODIV3
    // ;	CALL	DIV_F			;-> R0
    // ;NODIV3
    // ;	CMPF	0.06926,R0		;PI/8 (maximum turning radius)
    // ;	LDFGT	0.06926,R0
    // ;	CMPF	-0.06926,R0
    // ;	LDFLT	-0.06926,R0
    // ;	STF	R0,*+AR7(DELTA_RADYDELTA)
    // ;
    // ;
    // ;	PUSH	AR4
    // ;	ADDI	OPOSX,AR4
    // ;	CALL	CAMSCAN
    // ;	POP	AR4
    // ;	BNC	LLK23
    // ;
    // ;	NEGF	R0
    // ;	FLOAT	-1300,R1
    // ;	SUBF	R1,R0
    // ;	NEGF	R0
    // ;	ADDF	*+AR4(OPOSY),R0
    // ;	STF	R0,*+AR7(CD_DHEIGHT)
    // ;LLK23
    // ;
    // ;
    // ;	LDF	*+AR7(CD_SPEED),R3
    // ;	ADDF	*+AR7(CD_ACC),R3
    // ;
    // ;	FLOAT	300,R1			;MAX speed
    // ;
    // ;	CMPF	R1,R3
    // ;	LDFGT	R1,R3
    // ;	STF	R3,*+AR7(CD_SPEED)
    // ;
    // ;
    // ;
    // ;	LDF	*+AR7(DELTA_RADYDELTA),R2
    // ;	CALL	FSL_MOVE
    // ;
    // ;
    // ;	SLEEP	1
    // ;	BU	CIRCL_L
    // ;*----------------------------------------------------------------------------
    // ;
    // *----------------------------------------------------------------------------
    // *CHOPPER SEQUENCE:
    // *
    // *
    // *	come from behind and overhead and zoom ahead of player
    // *	rotate tilted forward (moving with player)
    // *	radio noise and such
    // *	zoom off to left or right
    // *	pull obj and sleep for awhile
    // *	reinsert objects and redo sequence
    // *
    // *
    // *
    // *
    // 	;BEGIN INITIALIZATION CODE
    // 	;
    // asm: 	LDI	@(_plyr1+PLY_PROC),AR2	;FIRST SETUP THAT WE FOLLOW THE PLAYERS PROC
    // asm: 	CALL	PRC_FOLLOW
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	SUICIDE
    // asm: 	LDI	AR0,AR4
    // asm: 	LDIL	helli,R0
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	FLOAT	-20,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	FLOAT	3500,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	CALL	_CARV0
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	CALL	SETDYNAOBJ
    // asm: 	LDF	0,R2
    // asm: 	STF	R2,*+AR4(ORADX)
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	STF	R2,*+AR4(CRADZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(CD_PASS_COUNT)
    // asm: 	STI	R0,*+AR7(CD_BOMB_COUNT)
    // asm: 	LDI	DRONE_C|HELICOPTER,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	STI	R0,*+AR7(PID)
    // asm: 	STI	AR7,*+AR4(OPLINK)
    // asm: 	RANDN	2
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,*+AR7(CD_MAX_PASSES)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(CD_DOATTACK)
    // 	;
    // 	;END OF INITIALIZATION CODE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(CD_LASTPASS)
    // 	;what type of pass to make?
CHOPPER_PASS:
    // asm: 	LDI	@HELI_ABORT,R0
    // asm: 	BNZ	CHOPPER_DIE
    // asm: 	LDI	*+AR7(CD_LASTPASS),R0
    // asm: 	BNZ	CHOPPER_DIE
    // asm: 	LDI	*+AR7(CD_PASS_COUNT),R0
    // asm: 	INC	R0
    // asm: 	CMPI	*+AR7(CD_MAX_PASSES),R0
    // asm: 	BGT	DOTHEMOVE
    // asm: 	STI	R0,*+AR7(CD_PASS_COUNT)
    // asm: 	BU	ONCOMMING_BUZZ
DOTHEMOVE:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(CD_LASTPASS)
    // asm: 	RANDN	2
    // asm: 	STI	R0,*+AR7(CD_DOATTACK)
    // asm: 	LDI	R0,R0
    // asm: 	BNZ	ONCOMMING_BUZZ
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOPPER", 0, 0);
    UNIMPL();
}

void FORWARD_BUZZ(void)
{
    // 	;FORWARD BUZZ INIT. CODE
    // 	;
    // asm: 	RANDN	20
    // asm: 	ADDI	20,R0
    // asm: 	STI	R0,*+AR7(CD_MAX_BOMBS)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR4(ORADX)
    // asm: 	STF	R0,*+AR4(ORADY)
    // asm: 	STF	R0,*+AR4(CRADZ)
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	STF	R0,*+AR4(OVELY)
    // asm: 	STF	R0,*+AR4(OVELZ)
    // 	;
    // 	;find where player is
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	NEGF	*+AR2(CARDIST2CNTR),R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm: 	LDI	*+AR2(CARTRAK),AR2
    // asm: 	LDI	*+AR2(OBLINK4),AR2
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm: 	CALL	DELTA_SUB_FUNCTION			;MATRIXA,VECTORA,R2
    // asm: 	LDP	@_VECTORA
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	ADDF	@_VECTORA+X,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR2(OPOSY),R0
    // asm: 	FLOAT	1000,R1
    // asm: 	SUBF	R1,R0
    // asm: 	ADDF	@_VECTORA+Y,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	ADDF	@_VECTORA+Z,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	SETDP
    // 	;initialize Ytheta to the intentional direction
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	FLOAT	60,R0
    // asm: 	STF	R0,*+AR7(CD_ACC)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(CD_BOMBTIK)
    // asm: 	LDI	CM_DB,R0
    // asm: 	STI	R0,*+AR7(CD_MODE)
    // 	;
    // 	;END FORWARD BUZZ INIT. CODE
CHOPPER_L:
    // asm: 	LDI	@HELI_ABORT,R0
    // asm: 	BNZ	FLYAWAY
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	NEGF	*+AR2(CARDIST2CNTR),R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
    // ;	LDF	*+AR2(CARSPEED),R1
    // ;	CMPF	60,R1
    // ;	BGT	NOCIRC
    // ;	JSRP	CIRCLE_JOIN
    // ;NOCIRC
    // asm: 	CALL	DIST_TO_PLYR
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm: 	FLOAT	10000,R2
    // asm: 	CMPF	R2,R0
    // asm: 	BGT	FLYAWAY
    // asm: 	CALL	HELI_SND
    // 	;see if we should track the next piece
CHECK_DIST:
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	RS	8,R0
    // asm: 	LDI	@SECTIONIDX,R1
    // asm: 	SUBI	@DGROUP_COUNT,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BLE	CHOPPER_DIE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	BZ	CHOPPER_SLP
    // 	;old xlane determination code position
    // asm: 	CALL	DELTA_GET_TRACK_POS		;CHECK IF WE SHOULD ADVANCE
    // asm: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	THIS_PIECE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm: 	BU	CHECK_DIST
THIS_PIECE:
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	*+AR7(CD_TSPEED),R1
    // asm: 	LDFLE	30,R1			;if 0 or less assume 30 voxel per frame
    // asm: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm: 	FIX	R0,R7
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDP	@_VECTORA		;lane position
    // asm: 	LDF	*+AR2(OPOSX),R2		;X
    // asm: 	SUBF	*+AR4(OPOSX),R2
    // asm: 	ADDF	@_VECTORA+X,R2
    // asm: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm: 	SUBF	*+AR4(OPOSZ),R3
    // asm: 	ADDF	@_VECTORA+Z,R3
    // asm: 	SETDP
    // 	;find the theta delta to this position
    // 	;
    // asm: 	CALL	ARCTANF			;-> R0
    // asm: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm: 	SUBF	1,R1
    // asm: 	BLE	NODIV2
    // asm: 	CALL	DIV_F			;-> R0
NODIV2:
    // asm: 	CMPF	0.3926,R0		;PI/8 (maximum turning radius)
    // asm: 	LDFGT	0.3926,R0
    // asm: 	CMPF	-0.3926,R0
    // asm: 	LDFLT	-0.3926,R0
    // asm: 	STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm: 	CALL	GET_CLOSEST_TRAK
    // asm: 	STI	AR0,*+AR7(CD_CLOSEROAD)
    // asm: 	LDF	*+AR7(CD_ODHEIGHT),R0
    // asm: 	STF	R0,*+AR7(CD_DHEIGHT)
    // asm: 	LDI	*+AR7(CD_CLOSEROAD),AR0
    // asm: 	FIX	*+AR0(OPOSY),R0
    // asm: 	SUBI	1300,R0
    // asm: 	FLOAT	R0
    // asm: 	STF	R0,*+AR7(CD_DHEIGHT)
    // asm: 	LDI	*+AR7(CD_CLOSEROAD),AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	LDI	@PLYCBLK,AR1
    // asm: 	LDI	*+AR1(CARTRAK),AR2
    // asm: 	LDI	*+AR2(OUSR1),R1
    // asm: 	CMPI	R1,R0
    // asm: 	BLT	ISBEND
    // asm: 	BGT	ISAHED
    // 	;tied case, must be exact
    // 	;
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	LDI	@PLYCAR,AR0
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	SUBF	*+AR2(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR0(OPOSZ),R1
    // asm: 	SUBF	*+AR2(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R0
    // asm: 	LDF	*+AR4(OPOSX),R2
    // asm: 	SUBF	*+AR2(OPOSX),R2
    // asm: 	MPYF	R2,R2
    // asm: 	LDF	*+AR4(OPOSZ),R1
    // asm: 	SUBF	*+AR2(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R2,R1
    // asm: 	CMPF	R0,R1
    // asm: 	BGT	ISBEND
    // asm: ISAHED	;is ahead of
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(CD_AHEADP)
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm: 	FLOAT	1750,R2
    // asm: 	CMPF	R2,R1
    // asm: 	BLT	LITTLE
    // asm: 	LDF	*+AR7(CD_SPEED),R0
    // asm: 	LDF	R0,R2
    // asm: 	MPYF	1.1,R2
    // asm: 	LDF	*+AR1(CARSPEED),R1
    // asm: 	ADDF	*+AR7(CD_ACC),R0
    // asm: 	MPYF	1.5,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFGT	R1,R0
    // asm: 	CMPF	R2,R0				;never rad change on our vel.
    // asm: 	LDFGT	R2,R0
    // asm: 	STF	R0,*+AR7(CD_SPEED)
    // asm: 	BU	L88
ISBEND:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(CD_AHEADP)
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm: 	FLOAT	3500,R2
    // asm: 	CMPF	R2,R1
    // asm: 	BLT	LITTLE
    // asm: 	LDF	*+AR1(CARSPEED),R0
    // asm: 	MPYF	3,R0
    // asm: 	FLOAT	600,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,*+AR7(CD_SPEED)
    // asm: 	BU	L88
LITTLE:
    // asm: 	LDF	*+AR7(CD_SPEED),R2
    // asm: 	MPYF	1.1,R2
    // asm: 	LDF	*+AR1(CARSPEED),R0
    // asm: 	MPYF	2.0,R0
    // asm: 	FLOAT	500,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	CMPF	R2,R0
    // asm: 	LDFGT	R2,R0
    // asm: 	STF	R0,*+AR7(CD_SPEED)
L88:
    // asm: LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm: 	LDF	*+AR7(CD_SPEED),R3
    // asm: 	CALL	FSL_MOVE
    // asm: 	LDI	*+AR7(CD_MODE),R4
    // asm: 	CMPI	CM_DB,R4
    // asm: 	BNE	NO_BOMBS
    // asm: 	LDI	*+AR7(CD_BOMBTIK),R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,*+AR7(CD_BOMBTIK)
    // asm: 	BGT	NO_BOMBS
    // asm: 	LDI	*+AR7(CD_AHEADP),R0
    // asm: 	BZ	NO_BOMBS
    // asm: 	LDI	@POSITION,R0
    // asm: 	CMPI	1,R0
    // asm: 	BNE	FLYAWAY
    // asm: 	FLOAT	2300,R0
    // asm: 	CMPF	*+AR7(DELTA_PLYRDIST),R0
    // asm: 	BGT	NO_BOMBS
    // asm: 	RANDN	10
    // asm: 	ADDI	10,R0
    // asm: 	STI	R0,*+AR7(CD_BOMBTIK)
    // asm: 	LDI	R0,R4
    // asm: 	LDI	*+AR7(CD_BOMB_COUNT),R0
    // asm: 	INC	R0
    // asm: 	CMPI	*+AR7(CD_MAX_BOMBS),R0
    // asm: 	BGT	FLYAWAY		;NO_BOMBS
    // asm: 	STI	R0,*+AR7(CD_BOMB_COUNT)
    // ;	CREATE	BOMB_PROC,3434
NO_BOMBS:
CHOPPER_SLP:
    // asm: 	SLEEP	1
    // asm: 	B	CHOPPER_L
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FORWARD_BUZZ", 0, 0);
    UNIMPL();
}

void FLYAWAY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CLRI	AR6			;flag
    // asm: 	LDI	100,AR5
    // asm: 	LDF	-10,R7
    // asm: 	LDF	0.015,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R0
    // asm: 	BLT	KKII
    // asm: 	ADDF	0.01,R0
    // asm: 	BU	KKUU
KKII:
    // asm: SUBF	0.01,R0
KKUU:
    // asm: STF	R0,*+AR7(CD_FLYTDIR)
    // asm: FLYAWAY_LP
    // asm: 	CALL	DIST_TO_PLYR
    // asm: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm: 	CALL	HELI_SND
    // asm: 	SUBF	3,R7
    // asm: 	CMPF	-95,R7
    // asm: 	LDFLT	-95,R7
    // asm: 	LDF	*+AR7(CD_DHEIGHT),R1
    // asm: 	ADDF	R7,R1
    // asm: 	STF	R1,*+AR7(CD_DHEIGHT)
    // asm: 	LDF	*+AR7(CD_SPEED),R0
    // asm: 	ADDF	*+AR7(CD_ACC),R0
    // asm: 	FLOAT	MAX_SPEED,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFGT	R1,R0
    // asm: 	STF	R0,*+AR7(CD_SPEED)
    // asm: 	LDF	*+AR7(CD_SPEED),R3
    // asm: 	LDF	*+AR7(CD_FLYTDIR),R2
    // asm: 	CALL	FSL_MOVE
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,FLYAWAY_LP
    // asm: 	BU	CHOPPER_PASS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYAWAY", 0, 0);
    UNIMPL();
}

void CHOPPER_DIE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*+AR4(OCARBLK),AR2
    // asm: 	CALL	DELCAR
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	HELI_SNDLP,AR2		;may want to add in volume effects
    // asm: 	CALL	KILLSNDFX
    // asm: 	LDI	*+AR7(CD_ANIPROC),AR2
    // asm: 	CALL	PRC_KILL
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOPPER_DIE", 0, 0);
    UNIMPL();
}

void FIND_YX_MATRIX(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	AR4	OBJECT
    // *
    // asm: 	LDF	*+AR4(ORADY),R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	LDF	*+AR4(ORADX),R2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR4,R3
    // asm: 	ADDI	OMATRIX,R3
    // asm: 	LDI	R3,AR2
    // asm: 	CALL	CONCATMAT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_YX_MATRIX", 0, 0);
    UNIMPL();
}

void SETDYNAOBJ(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	O_DYNAMIC,R0	 	;MAKE PARENT OBJECT DYNAMIC
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDL	CHOPPERDYNA,AR2
    // asm: 	LDI	AR4,AR3
    // asm: 	ADDI	ODYNALIST,AR3
    // *INIT DYNA OBJECTS
    // asm: 	LDI	*AR2++,RC		;GET DYNAMIC OBJECT COUNT
    // asm: 	RPTB	CHOPLP
    // asm:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
    // asm: 	STI	AR0,*AR3
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERX)
    // asm: 	STF	R0,*+AR0(DYNATRANSX)
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERY)
    // asm: 	STF	R0,*+AR0(DYNATRANSY)
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERZ)
    // asm: 	STF	R0,*+AR0(DYNATRANSZ)
    // asm: 	LDI	*AR2++,R0
    // asm: 	STI	R0,*+AR0(DYNANVERTS)
    // asm: 	LDI	*AR2++,R0
    // asm: 	STI	R0,*+AR0(DYNAFLAG)
    // asm: 	STI	AR4,*+AR0(DYNAPARENT)
CHOPLP:
    // asm: LDI	AR0,AR3
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR3			;LAST LINK IS ZERO, DUDES
    // asm: 	LDI	AR3,AR0
    // asm: 	ADDI	DYNAMATRIX,AR0
    // asm: 	CALL	INITMAT
    // asm: 	LDL	CHOPPERANI,AR2
    // asm: 	LDI	DRONE_C|ANI_T,R2
    // asm: 	CALL	PRC_CREATE_CHILD
    // asm: 	STI	AR0,*+AR7(CD_ANIPROC)
    // asm: 	STI	AR0,*+AR4(ORADZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETDYNAOBJ", 0, 0);
    UNIMPL();
}

void CHOPPERANI(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LONGROUT
    // asm: 	CLRF	R6
CANILP:
    // asm: 	LDI	*+AR4(ODYNALIST),R0
    // asm: 	LDI	R0,AR0
    // asm: 	LDF	R6,R2
    // asm: 	FLOAT	@NFRAMES,R0
    // asm: 	MPYF	0.6,R0
    // asm: 	ADDF	R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R2,R6
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	DYNAMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	SLEEP	1
    // asm: 	BU	CANILP
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOPPERANI", 0, 0);
    UNIMPL();
}

void FSL_MOVE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *FSL_MOVE	Frictionless Movement
    // *
    // *PARAMETERS
    // *	R2	THETA DELTA (CHANGE IN THETA)
    // *	R3	SPEED
    // *
    // ;	LDP	@NFRAMES
    // ;	FLOAT	@NFRAMES,R0
    // ;	MPYF	R0,R3
    // 	;
    // 	;move to better y height
    // 	;
    // asm: 	LDF	*+AR7(CD_DHEIGHT),R0
    // asm: 	SUBF	*+AR4(OPOSY),R0
    // asm: 	LDF	*+AR4(OVELY),R1
    // asm: 	MPYF	0.85,R1
    // asm: 	MPYF	0.15,R0
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	60,R1
    // asm: 	LDFGT	60,R1
    // asm: 	CMPF	-60,R1
    // asm: 	LDFLT	-60,R1
    // asm: 	STF	R1,*+AR4(OVELY)
    // asm: 	PUSHF	R2
    // asm: 	ADDF	*+AR4(ORADY),R2
    // asm:  	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR2,R2
    // asm: 	CALL	CLR_VECTORA
    // asm: 	STF	R3,*+AR2(Z)
    // asm: 	LDF	*+AR4(OVELY),R0
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R0
    // asm: 	LDF	*+AR4(OVELX),R1
    // asm: 	MPYF	0.1,R0
    // asm: 	MPYF	0.9,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR2(Y),R0
    // asm: 	LDF	*+AR4(OVELY),R1
    // asm: 	MPYF	0.9,R0
    // asm: 	MPYF	0.1,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR4(OVELY)
    // ;	FLOAT	@NFRAMES,R1
    // ;	MPYF	R1,R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	LDF	*+AR4(OVELZ),R1
    // asm: 	MPYF	0.1,R0
    // asm: 	MPYF	0.9,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR4(OVELZ)
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDF	*+AR7(CD_TSPEED),R0
    // asm: 	LDF	*+AR4(ORADX),R2
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	POPF	R2
    // asm: 	MPYF	25,R2
    // asm: 	LDF	*+AR4(CRADZ),R0
    // asm: 	MPYF	0.1,R2
    // asm: 	MPYF	0.9,R0
    // asm: 	ADDF	R0,R2
    // asm: 	CMPF	0.4,R2
    // asm: 	LDFGT	0.4,R2
    // asm: 	CMPF	-0.4,R2
    // asm: 	LDFLT	-0.4,R2
    // asm: 	STF	R2,*+AR4(CRADZ)
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMATV		;X * Z -> matA
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMATV
    // 	;FIND WHAT OUR SPEED ACTUAL IS
    // asm: 	LDF	*+AR4(OVELX),R2
    // asm: 	MPYF	R2,R2
    // asm: 	LDF	*+AR4(OVELY),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	LDF	*+AR4(OVELZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	CALL	SQRT
    // asm: 	LDF	*+AR7(CD_TSPEED),R1
    // asm: 	STF	R0,*+AR7(CD_TSPEED)
    // asm: 	STF	R1,*+AR7(CD_TSPEED_OLD)
    // asm: 	CALL	DIV_F30
    // asm: 	CMPF	1.0,R0
    // asm: 	LDFGT	1.0,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	STF	R0,*+AR4(ORADX)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FSL_MOVE", 0, 0);
    UNIMPL();
}

void HELI_SND(void)
{
    // *----------------------------------------------------------------------------
    // ;
    // ;*----------------------------------------------------------------------------
    // ;*
    // ;*PARAMETERS
    // ;*	AR4	CHOPPER OBJECT
    // ;*
    // ;BOMB_PROC:
    // ;	LDI	AR4,AR5
    // ;	CALL	OBJ_GET
    // ;	BC	SUICIDE
    // ;
    // ;	LDI	AR0,AR4
    // ;	LDF	*+AR5(OPOSX),R0
    // ;	STF	R0,*+AR4(OPOSX)
    // ;	LDF	*+AR5(OPOSY),R0
    // ;	STF	R0,*+AR4(OPOSY)
    // ;	LDF	*+AR5(OPOSZ),R0
    // ;	STF	R0,*+AR4(OPOSZ)
    // ;
    // ;	STI	AR7,*+AR4(OPLINK)
    // ;
    // ;	LDF	*+AR2(ORADY),R2
    // ;	LDI	AR4,AR2
    // ;	ADDI	OMATRIX,AR2
    // ;	CALL	FIND_YMATRIX
    // ;
    // ;	LDIL	bomb,AR0
    // ;	STI	AR0,*+AR4(OROMDATA)
    // ;
    // ;	LDI	1,R1
    // ;	LS	O_PROC_B,R1
    // ;	LDI	1,R0
    // ;	LS	O_3DROT_B,R0
    // ;	OR	R1,R0
    // ;	OR	*+AR4(OFLAGS),R0
    // ;	STI	R0,*+AR4(OFLAGS)
    // ;
    // ;	LDF	HALFPI,R2
    // ;	LDI	AR4,AR2
    // ;	ADDI	OMATRIX,AR2
    // ;	CALL	FIND_XMATRIX
    // ;
    // ;	LDI	AR4,AR2
    // ;	CALL	OBJ_INSERT
    // ;
    // ;
    // ;	LDF	HALFPI,R0
    // ;	STF	R0,*+AR4(ORADX)
    // ;
    // ;
    // ;	LDF	*+AR5(OVELX),R0
    // ;	STF	R0,*+AR4(OVELX)
    // ;	LDF	*+AR5(OVELY),R0
    // ;	STF	R0,*+AR4(OVELY)
    // ;	LDF	*+AR5(OVELZ),R0
    // ;	STF	R0,*+AR4(OVELZ)
    // ;
    // ;	CALL	ADD_RDDEBRIS
    // ;	LDI	TSIGN_C|TSC_HARD,R0
    // ;	STI	R0,*+AR4(OID)
    // ;
    // ;
    // ;
    // ;	LDI	120,AR6
    // ;BOMB_L
    // ;	DEC	AR6
    // ;	CMPI	0,AR6
    // ;	BLE	BOMBDIE
    // ;
    // ;	PUSH	AR4
    // ;	ADDI	OPOSX,AR4
    // ;	CALL	CAMSCAN
    // ;	POP	AR4
    // ;	BNC	BOMBDIE
    // ;
    // ;	FLOAT	100,R1
    // ;	CMPF	R1,R0
    // ;	BLE	BOMBDIE
    // ;
    // ;
    // ;	LDF	*+AR4(OVELX),R0
    // ;	MPYF	0.99,R0
    // ;	STF	R0,*+AR4(OVELX)
    // ;	LDF	*+AR4(OVELY),R0
    // ;	ADDF	5,R0
    // ;	FLOAT	50,R1
    // ;	CMPF	R1,R0
    // ;	LDFGT	R1,R0
    // ;	STF	R0,*+AR4(OVELY)
    // ;	LDF	*+AR4(OVELZ),R0
    // ;	MPYF	0.99,R0
    // ;	STF	R0,*+AR4(OVELZ)
    // ;
    // ;	FLOAT	@NFRAMES,R2
    // ;
    // ;	LDF	*+AR4(OVELX),R0
    // ;	MPYF	R2,R0
    // ;	ADDF	*+AR4(OPOSX),R0
    // ;	STF	R0,*+AR4(OPOSX)
    // ;
    // ;	LDF	*+AR4(OVELY),R0
    // ;	MPYF	R2,R0
    // ;	ADDF	*+AR4(OPOSY),R0
    // ;	STF	R0,*+AR4(OPOSY)
    // ;
    // ;	LDF	*+AR4(OVELZ),R0
    // ;	MPYF	R2,R0
    // ;	ADDF	*+AR4(OPOSZ),R0
    // ;	STF	R0,*+AR4(OPOSZ)
    // ;
    // ;
    // ;	LDF	*+AR4(ORADX),R2
    // ;	MPYF	0.80,R2
    // ;	STF	R2,*+AR4(ORADX)
    // ;	LDI	@MATRIXAI,AR2
    // ;	CALL	FIND_XMATRIX
    // ;
    // ;
    // ;	LDF	*+AR4(ORADY),R2
    // ;	ADDF	0.3,R2
    // ;	CALL	NORMITS
    // ;	STF	R2,*+AR4(ORADY)
    // ;
    // ;	LDI	AR4,AR2
    // ;	ADDI	OMATRIX,AR2
    // ;	CALL	FIND_YMATRIX
    // ;
    // ;	LDI	@MATRIXAI,AR2
    // ;	LDI	AR4,R2
    // ;	ADDI	OMATRIX,R2
    // ;	LDI	R2,R3
    // ;	CALL	CONCATMATV
    // ;
    // ;	SLEEP	1
    // ;	BU	BOMB_L
    // ;
    // ;BOMBDIE
    // ;	LDI	@EXP_ANII,AR5
    // ;
    // ;	LDI	*AR5,AR0
    // ;	STI	AR0,*+AR4(OROMDATA)
    // ;
    // ;	ORM	O_POSTER,*+AR4(OFLAGS)
    // ;
    // ;	LDI	*+AR4(OFLAGS),R0
    // ;	ANDN	O_1PAL,R0
    // ;	OR	O_POSTER,R0
    // ;	STI	R0,*+AR4(OFLAGS)
    // ;
    // ;	BU	EXLJ
    // ;*----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // ;EXP_ANII	.word	EXP_ANI
    // ;EXP_ANI
    // ;	;.word	exp1,exp2,exp3,exp4,exp5,exp6,exp7,-1
    // ;	;.word	dexplo1,dexplo2,dexplo3,dexplo4,dexplo5
    // ;	;.word	dexplo6,dexplo7,dexplo8,dexplo9,dexplo10,dexplo11,-1
    // ;
    // ;	.word	blast1,blast2,blast3,blast4,blast5
    // ;	.word	blast6,blast7,blast8,blast9,blast10,-1
    // ;
    // ;
    // ;
    // ;EXPLO_SOUNDSI	.word	EXPLO_SOUNDS
    // ;EXPLO_SOUNDS	.word	EXP1,EXP2,EXP3
    // ;
    // ;EXP_NMPUFF:
    // ;	LDI	AR4,AR6
    // ;	CALL	OBJ_GET
    // ;	BC	SUICIDE
    // ;	LDI	AR0,AR4
    // ;	LDI	@EXP_ANII,AR5
    // ;
    // ;	LDI	*AR5,AR0
    // ;	STI	AR0,*+AR4(OROMDATA)
    // ;
    // ;	LDF	*+AR6(OPOSX),R0
    // ;	STF	R0,*+AR4(OPOSX)
    // ;	LDF	*+AR6(OPOSY),R0
    // ;	STF	R0,*+AR4(OPOSY)
    // ;	LDF	*+AR6(OPOSZ),R0
    // ;	STF	R0,*+AR4(OPOSZ)
    // ;	ORM	O_POSTER,*+AR4(OFLAGS)
    // ;	LDI	AR4,AR2
    // ;	CALL	OBJ_INSERT
    // ;EXLJ
    // ;
    // ;;explosions must get louder
    // ;;
    // ;;	RANDN	3
    // ;	LDI	@EXPLO_SOUNDSI,AR2
    // ;;	ADDI	R0,AR2
    // ;	LDI	*AR2,AR2
    // ;	CALL	ONESNDFX
    // ;
    // ;	;collidable explosion
    // ;
    // ;EXP_NMPUFFLP
    // ;	LDI	*AR5++,AR0
    // ;	CMPI	-1,AR0
    // ;	BEQ	EXP_DIE
    // ;	STI	AR0,*+AR4(OROMDATA)
    // ;
    // ;	SLEEP	1
    // ;	BU	EXP_NMPUFFLP
    // ;
    // ;EXP_DIE
    // ;	LDI	1,R1
    // ;	LS	O_PROC_B,R1
    // ;	NOT	R1
    // ;	LDI	*+AR4(OFLAGS),R0
    // ;	AND	R1,R0
    // ;	STI	R0,*+AR4(OFLAGS)
    // ;
    // ;
    // ;	LDI	AR4,AR2
    // ;	CALL	OBJ_DELETE
    // ;	DIE
    // ;*----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	HELI_SNDLP,AR2		;may want to add in volume effects
    // asm: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm: 	BEQ	IS_T1
    // asm: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm: 	BEQ	IS_T2
    // asm: 	CALL	ONESNDFX
    // asm: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm: 	BNE	NOT_T1
IS_T1:
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R0
    // asm: 	FLOAT	20000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFGT	R1,R0
    // asm: 	SUBRF	R1,R0
    // asm: 	MPYFL	0.00005,R0
    // ;	MPYF	64,R0
    // ;	ADDF	191,R0
    // asm: 	MPYF	127,R0
    // asm: 	ADDF	128,R0
    // asm: 	FIX	R0,R1
    // asm: 	LDI	1,R0
    // asm: 	CALL	SET_TRACK_VOL
    // asm: 	BU	HEND
NOT_T1:
    // asm: CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm: 	BNE	NOT_T2
IS_T2:
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R0
    // asm: 	FLOAT	20000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFGT	R1,R0
    // asm: 	SUBRF	R1,R0
    // asm: 	MPYFL	0.00005,R0
    // ;	MPYF	64,R0
    // ;	ADDF	191,R0
    // asm: 	MPYF	127,R0
    // asm: 	ADDF	128,R0
    // asm: 	FIX	R0,R1
    // asm: 	LDI	2,R0
    // asm: 	CALL	SET_TRACK_VOL
    // asm: 	BU	HEND
NOT_T2:
HEND:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HELI_SND", 0, 0);
    UNIMPL();
}

void GET_CLOSEST_TRAK(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *RETURNS
    // *	AR0	ROAD OBJECT
    // *
    // asm: 	LDI	@DRIVE_LIST,R0
    // asm: 	BZ	GETRKX  		;NULL LIST DUDES
    // asm: 	LDI	0,AR0			;CLOSEST ROAD SEGMENT INDEX
    // asm: 	FLOAT	7FFFH,R2		;INITIAL CLOSEST DISTANCE
    // asm: 	MPYF	R2,R2
    // asm: 	LDF	*+AR4(OPOSX),R3
    // asm: 	LDF	*+AR4(OPOSZ),R4
    // asm: 	LDI	R0,AR2
GETRK:
    // asm: 	LDI	*+AR2(OID),R0	    	;IS THIS REAL ROAD?
    // asm: 	AND	0FF0H,R0
    // asm: 	CMPI	300H,R0
    // asm: 	BNE	GETRKL			;NOPE...
    // asm: 	LDF	R3,R0
    // asm: 	SUBF	*+AR2(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	R4,R1
    // asm: 	SUBF	*+AR2(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	R1,R2
    // asm: 	BLE	GETRKL
    // asm: 	LDI	AR2,AR0
    // asm: 	LDF	R1,R2
GETRKL:
    // asm: 	LDI	*+AR2(OLINK3),R0
    // asm: 	BNZD	GETRK
    // asm: 	LDI	R0,AR2
    // asm: 	NOP
    // asm: 	NOP
    // 	;---->	BNZ	GETRK
    // ;	STI	AR0,*+AR5(CARTRAK)	;SAVE TRACK SECTION
GETRKX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_CLOSEST_TRAK", 0, 0);
    UNIMPL();
}

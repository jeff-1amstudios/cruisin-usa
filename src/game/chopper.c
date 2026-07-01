
#include "chopper.h"
#include "../core/machine.h"
#include "cmos.h"
#include "delta.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/CHOPPER.ASM
 */

static void DIRECT_ATTACK(void);
static void ONCOMMING_BUZZ(void);
void CHOPPER(void);
static void FORWARD_BUZZ(void);
static void FLYAWAY(void);
static void CHOPPER_DIE(void);
static void FIND_YX_MATRIX(void);
static void SETDYNAOBJ(void);
static void CHOPPERANI(void);
static void FSL_MOVE(void);
static void HELI_SND(void);
void GET_CLOSEST_TRAK(void);

/*
 *----------------------------------------------------------------------------
 *CHOPPER IN THE SKY
 *
 *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

/* asm: HELI_ABORT	.bss	HELI_ABORT,1 */
int HELI_ABORT;
/*
 *
 *	!!!!NO JSRPing!!!!
 *
 */

// *LOCALS  (AFTER DELTA EQUATES)
#define CD_ANIPROC (PDATA + 18)
#define CD_MODE (PDATA + 19)
#define CD_ACC (PDATA + 20)        // acceleration
#define CD_SPEED (PDATA + 21)      // current speed
#define CD_BOMBTIK (PDATA + 22)    // bomb pause
#define CD_TSPEED (PDATA + 23)     // true speed (x/y/z)
#define CD_TSPEED_OLD (PDATA + 24) // old true speed (x/y/z)
#define CD_FLYTDIR (PDATA + 25)    // breakaway theta
#define CD_DHEIGHT (PDATA + 26)    // FL desired height value
#define CD_ODHEIGHT (PDATA + 27)   // FL old desired height value
#define CD_CLOSEROAD (PDATA + 28)  // UD closest road object
#define CD_AHEADP (PDATA + 29)     // UD	flag ahead of player?
#define CD_PASS_COUNT (PDATA + 30)
#define CD_BOMB_COUNT (PDATA + 31)
#define CD_MAX_PASSES (PDATA + 32)
#define CD_MAX_BOMBS (PDATA + 33)
#define CD_LASTPASS (PDATA + 34) // P
#define CD_DOATTACK (PDATA + 35) // P
// *NO MORE UNLESS NOT JSRPing

// *CHOPPER MODE
#define CM_CU 1 // CATCH UP
#define CM_DB 2 // DROP BOMBS
#define CM_FA 3 // FLY AWAY
#define CRADZ OUSR1
#define MAX_SPEED 667 // about 180 mph
// *----------------------------------------------------------------------------
/* asm: CHOPPERDYNA */
/* asm: 	.word	1		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,-206,14	;blades */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	0,0,0		;main body */
/* asm: 	.word	94		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
static int CHOPPERDYNA[] = {
    1, // #OF DYNAS-1
    0,
    -206,
    14, // blades
    3,  // VERTS-1
    1,  // DYNAFLAG
    0,
    0,
    0,  // main body
    94, // VERTS-1
    1,  // DYNAFLAG
    // ----------------------------------------------------------------------------
};

/*
 *---------------------------------------------------------------------------
 *
 *go straight for the plyrs car
 *
 *(already close by)
 *branched to from oncoming buzz
 *
 *
 */
static void DIRECT_ATTACK(void) {
    // 	;
    // 	;DEBUG	only attack player when he is in the first position
    // 	;
    // asm 00007C6E: 	LDI	@POSITION,R0
    // asm 00007C6F: 	CMPI	1,R0
    // asm 00007C70: 	BNE	FLYAWAY
    // 	;
    // 	;
    // asm 00007C71: 	CALL	DIST_TO_PLYR
    // asm 00007C72: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm 00007C73: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm 00007C74: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm 00007C75: 	CALL	HELI_SND
    // 	;see if we should track the next piece
CHECK_DIST28:
    // asm 00007C76: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm 00007C77: 	RS	8,R0
    // asm 00007C78: 	LDI	@SECTIONIDX,R1
    // asm 00007C79: 	SUBPI	@DGROUP_COUNT,R1
    // asm 00007C7A: 	CMPI	R1,R0
    // asm 00007C7B: 	BLE	FLYAWAY		;CHOPPER_DIE
    // asm 00007C7C: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007C7D: 	LDI	*+AR2(OBLINK4),R0
    // asm 00007C7E: 	BZ	FLYAWAY	;CHOPPER_DIE
    // asm 00007C7F: 	LDI	@PLYCBLK,AR2
    // asm 00007C80: 	LDF	*+AR2(CARDIST2CNTR),R0
    // asm 00007C81: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm 00007C82: 	CALL	GET_TRACK_POS_RVS_XLANE		;CHECK IF WE SHOULD ADVANCE
    // asm 00007C83: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm 00007C84: 	CMPF	R1,R0
    // asm 00007C85: 	BGT	THIS_PIECE28
    // asm 00007C86: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007C87: 	LDI	*+AR2(OBLINK4),R0
    // asm 00007C88: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm 00007C89: 	LDI	R0,AR0
    // asm 00007C8A: 	LDI	*+AR0(OUSR1),R0
    // asm 00007C8B: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm 00007C8C: 	BU	CHECK_DIST28
THIS_PIECE28:
    // asm 00007C8D: 	FLOATP	@NFRAMES,R1
    // asm 00007C8E: 	MPYF	*+AR7(CD_TSPEED),R1
    // asm 00007C8F: 	LDFLE	30,R1			;if 0 or less assume 30 voxel per frame
    // asm 00007C90: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm 00007C91: 	FIX	R0,R7
    // asm 00007C92: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007C93: 	LDP	@_VECTORA		;lane position
    // asm 00007C94: 	LDF	*+AR2(OPOSX),R2		;X
    // asm 00007C95: 	SUBF	*+AR4(OPOSX),R2
    // asm 00007C96: 	ADDF	@_VECTORA+X,R2
    // asm 00007C97: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm 00007C98: 	SUBF	*+AR4(OPOSZ),R3
    // asm 00007C99: 	ADDF	@_VECTORA+Z,R3
    // asm 00007C9A: 	SETDP
    // 	;find the theta delta to this position
    // 	;
    // asm 00007C9B: 	CALL	ARCTANF			;-> R0
    // asm 00007C9C: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm 00007C9D:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm 00007C9E: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm 00007C9F: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm 00007CA0: 	SUBF	1,R1
    // asm 00007CA1: 	BLE	NODIV8
    // asm 00007CA2: 	CALL	DIV_F			;-> R0
NODIV8:
    // asm 00007CA3: 	CMPF	0.3926,R0		;PI/8 (maximum turning radius)
    // asm 00007CA4: 	LDFGT	0.3926,R0
    // asm 00007CA5: 	CMPF	-0.3926,R0
    // asm 00007CA6: 	LDFLT	-0.3926,R0
    // asm 00007CA7: 	STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm 00007CA8: 	PUSH	AR4
    // asm 00007CA9: 	ADDI	OPOSX,AR4
    // asm 00007CAA: 	CALL	CAMSCAN
    // asm 00007CAB: 	POP	AR4
    // asm 00007CAC: 	BNC	LLK28
    // asm 00007CAD: 	NEGF	R0
    // asm 00007CAE: 	FLOAT	-300,R1
    // asm 00007CAF: 	SUBF	R1,R0
    // asm 00007CB0: 	NEGF	R0
    // asm 00007CB1: 	ADDF	*+AR4(OPOSY),R0
    // asm 00007CB2: 	STF	R0,*+AR7(CD_DHEIGHT)
LLK28:
    // asm 00007CB3: 	LDF	*+AR7(CD_SPEED),R0
    // asm 00007CB4: 	ADDF	*+AR7(CD_ACC),R0
    // asm 00007CB5: 	FLOAT	MAX_SPEED,R1
    // asm 00007CB6: 	CMPF	R1,R0
    // asm 00007CB7: 	LDFGT	R1,R0
    // asm 00007CB8: 	STF	R0,*+AR7(CD_SPEED)
    // asm 00007CB9: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm 00007CBA: 	LDF	*+AR7(CD_SPEED),R3
    // asm 00007CBB: 	CALL	FSL_MOVE
    // asm 00007CBC: 	SLEEP	1
    // asm 00007CBE: 	BU	DIRECT_ATTACK
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIRECT_ATTACK", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *Take the helicopter, and buzz (@ 200 MPH) right over the players car
 *exit when start of the world is hit
 *
 *
 *
 */
static void ONCOMMING_BUZZ(void) {
    // asm 00007CBF: 	LDF	0,R0
    // asm 00007CC0: 	STF	R0,*+AR4(ORADX)
    // asm 00007CC1: 	STF	R0,*+AR4(ORADY)
    // asm 00007CC2: 	STF	R0,*+AR4(CRADZ)
    // asm 00007CC3: 	STF	R0,*+AR4(OVELX)
    // asm 00007CC4: 	STF	R0,*+AR4(OVELY)
    // asm 00007CC5: 	STF	R0,*+AR4(OVELZ)
    // 	;
    // 	;find where player is
    // asm 00007CC6: 	LDI	@PLYCBLK,AR2
    // asm 00007CC7: 	LDF	*+AR2(CARDIST2CNTR),R0
    // asm 00007CC8: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm 00007CC9: 	LDI	@DYNALIST_END,AR2
    // asm 00007CCA: 	LDI	*+AR2(OBLINK4),AR2
    // asm 00007CCB: 	LDI	*+AR2(OBLINK4),AR2
    // asm 00007CCC: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm 00007CCD: 	LDI	*+AR2(OUSR1),R0
    // asm 00007CCE: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm 00007CCF: 	CALL	SUB_FUNCTION_RVS_XLANE		;MATRIXA,VECTORA,R2
    // asm 00007CD0: 	LDP	@_VECTORA
    // asm 00007CD1: 	LDF	*+AR2(OPOSX),R0
    // asm 00007CD2: 	ADDF	@_VECTORA+X,R0
    // asm 00007CD3: 	STF	R0,*+AR4(OPOSX)
    // asm 00007CD4: 	LDF	*+AR2(OPOSY),R0
    // asm 00007CD5: 	FLOAT	1000,R1
    // asm 00007CD6: 	SUBF	R1,R0
    // asm 00007CD7: 	ADDF	@_VECTORA+Y,R0
    // asm 00007CD8: 	STF	R0,*+AR4(OPOSY)
    // asm 00007CD9: 	LDF	*+AR2(OPOSZ),R0
    // asm 00007CDA: 	ADDF	@_VECTORA+Z,R0
    // asm 00007CDB: 	STF	R0,*+AR4(OPOSZ)
    // asm 00007CDC: 	SETDP
    // 	;initialize Ytheta to the intentional direction
    // asm 00007CDD: 	STF	R2,*+AR4(ORADY)
    // asm 00007CDE: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm 00007CDF: 	LDI	AR4,AR2
    // asm 00007CE0: 	ADDI	OMATRIX,AR2
    // asm 00007CE1: 	CALL	FIND_YMATRIX
ONBUZZ_L:
    // asm 00007CE2: 	CALL	DIST_TO_PLYR
    // asm 00007CE3: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm 00007CE4: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm 00007CE5: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm 00007CE6: 	FLOAT	22000,R2
    // asm 00007CE7: 	CMPF	R2,R0
    // asm 00007CE8: 	BLE	FLYAWAY
    // asm 00007CE9: 	CALL	HELI_SND
    // 	;see if we should track the next piece
CHECK_DIST2:
    // asm 00007CEA: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm 00007CEB: 	RS	8,R0
    // asm 00007CEC: 	LDI	@SECTIONIDX,R1
    // asm 00007CED: 	SUBPI	@DGROUP_COUNT,R1
    // asm 00007CEE: 	CMPI	R1,R0
    // asm 00007CEF: 	BLE	CHOPPER_DIE
    // asm 00007CF0: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007CF1: 	LDI	*+AR2(OBLINK4),R0
    // asm 00007CF2: 	BZ	CHOPPER_DIE
    // asm 00007CF3: 	LDI	@PLYCBLK,AR2
    // asm 00007CF4: 	LDF	*+AR2(CARDIST2CNTR),R0
    // asm 00007CF5: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm 00007CF6: 	CALL	GET_TRACK_POS_RVS_XLANE		;CHECK IF WE SHOULD ADVANCE
    // asm 00007CF7: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm 00007CF8: 	CMPF	R1,R0
    // asm 00007CF9: 	BGT	THIS_PIECE2
    // asm 00007CFA: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007CFB: 	LDI	*+AR2(OBLINK4),R0
    // asm 00007CFC: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm 00007CFD: 	LDI	R0,AR0
    // asm 00007CFE: 	LDI	*+AR0(OUSR1),R0
    // asm 00007CFF: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm 00007D00: 	BU	CHECK_DIST2
THIS_PIECE2:
    // asm 00007D01: 	FLOATP	@NFRAMES,R1
    // asm 00007D02: 	MPYF	*+AR7(CD_TSPEED),R1
    // asm 00007D03: 	LDFLE	30,R1			;if 0 or less assume 30 voxel per frame
    // asm 00007D04: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm 00007D05: 	FIX	R0,R7
    // asm 00007D06: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007D07: 	LDP	@_VECTORA		;lane position
    // asm 00007D08: 	LDF	*+AR2(OPOSX),R2		;X
    // asm 00007D09: 	SUBF	*+AR4(OPOSX),R2
    // asm 00007D0A: 	ADDF	@_VECTORA+X,R2
    // asm 00007D0B: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm 00007D0C: 	SUBF	*+AR4(OPOSZ),R3
    // asm 00007D0D: 	ADDF	@_VECTORA+Z,R3
    // asm 00007D0E: 	SETDP
    // 	;find the theta delta to this position
    // 	;
    // asm 00007D0F: 	CALL	ARCTANF			;-> R0
    // asm 00007D10: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm 00007D11:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm 00007D12: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm 00007D13: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm 00007D14: 	SUBF	1,R1
    // asm 00007D15: 	BLE	NODIV
    // asm 00007D16: 	CALL	DIV_F			;-> R0
NODIV:
    // asm 00007D17: 	CMPF	0.3926,R0		;PI/8 (maximum turning radius)
    // asm 00007D18: 	LDFGT	0.3926,R0
    // asm 00007D19: 	CMPF	-0.3926,R0
    // asm 00007D1A: 	LDFLT	-0.3926,R0
    // asm 00007D1B: 	STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm 00007D1C: 	PUSH	AR4
    // asm 00007D1D: 	ADDI	OPOSX,AR4
    // asm 00007D1E: 	CALL	CAMSCAN
    // asm 00007D1F: 	POP	AR4
    // asm 00007D20: 	BNC	LLK2
    // asm 00007D21: 	NEGF	R0
    // asm 00007D22: 	FLOAT	-1300,R1
    // asm 00007D23: 	SUBF	R1,R0
    // asm 00007D24: 	NEGF	R0
    // asm 00007D25: 	ADDF	*+AR4(OPOSY),R0
    // asm 00007D26: 	STF	R0,*+AR7(CD_DHEIGHT)
LLK2:
    // asm 00007D27: 	LDF	*+AR7(CD_SPEED),R0
    // asm 00007D28: 	ADDF	*+AR7(CD_ACC),R0
    // asm 00007D29: 	FLOAT	MAX_SPEED,R1
    // asm 00007D2A: 	CMPF	R1,R0
    // asm 00007D2B: 	LDFGT	R1,R0
    // asm 00007D2C: 	STF	R0,*+AR7(CD_SPEED)
    // asm 00007D2D: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm 00007D2E: 	LDF	*+AR7(CD_SPEED),R3
    // asm 00007D2F: 	CALL	FSL_MOVE
    // asm 00007D30: 	SLEEP	1
    // asm 00007D32: 	BU	ONBUZZ_L
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ONCOMMING_BUZZ", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
;
;*----------------------------------------------------------------------------
;*
;*
;*
;*
;CIRCLE_BUZZ:
;	LDF	0,R0
;	STF	R0,*+AR4(ORADX)
;	STF	R0,*+AR4(ORADY)
;	STF	R0,*+AR4(CRADZ)
;
;	STF	R0,*+AR4(OVELX)
;	STF	R0,*+AR4(OVELY)
;	STF	R0,*+AR4(OVELZ)
;
;
;	;
;	;find where player is
;
;	LDI	@PLYCBLK,AR2
;	NEGF	*+AR2(CARDIST2CNTR),R0
;	STF	R0,*+AR7(DELTA_XLANE)
;	LDI	*+AR2(CARTRAK),AR2
;	LDI	*+AR2(OBLINK4),AR2
;
;	STI	AR2,*+AR7(DELTA_TPIECE)
;	LDI	*+AR2(OUSR1),R0
;	STI	R0,*+AR7(DELTA_LAST_OID)
;	CALL	DELTA_SUB_FUNCTION			;MATRIXA,VECTORA,R2
;
;	LDP	@_VECTORA
;	LDF	*+AR2(OPOSX),R0
;	ADDF	@_VECTORA+X,R0
;	STF	R0,*+AR4(OPOSX)
;	LDF	*+AR2(OPOSY),R0
;	FLOAT	1000,R1
;	SUBF	R1,R0
;	ADDF	@_VECTORA+Y,R0
;	STF	R0,*+AR4(OPOSY)
;	LDF	*+AR2(OPOSZ),R0
;	ADDF	@_VECTORA+Z,R0
;	STF	R0,*+AR4(OPOSZ)
;	SETDP
;
;
;	;initialize Ytheta to the intentional direction
;	STF	R2,*+AR4(ORADY)
;	STF	R2,*+AR7(DELTA_RADYDELTA)
;
;	LDI	AR4,AR2
;	ADDI	OMATRIX,AR2
;	CALL	FIND_YMATRIX
;
;
;CIRCLE_JOIN
;	CLRF	R6
;	LDI	@CIRCLE_SI,AR5
;
;CIRCL_L
;	LDI	@PLYCBLK,AR0
;	LDF	*+AR0(CARSPEED),R0
;	CMPF	40,R0
;	BLT	ISOK55
;	RETP
;ISOK55
;
;
;	CALL	DIST_TO_PLYR
;	LDF	*+AR7(DELTA_PLYRDIST),R1
;	STF	R0,*+AR7(DELTA_PLYRDIST)
;	STF	R1,*+AR7(DELTA_OPLYRDIST)
;
;	CALL	HELI_SND
;
;	.data
;CIRCLE_SI	.word	CIRCLE_S
;CIRCLE_S ;	X,Z
;	.float	4000,2200
;	.float	2500,5000
;	.float	0,9000
;	.float	-2500,5000
;	.float	-4000,2200
;	.float	-1
;	.text
;
;
;JKKU
;	LDI	@PLYCBLK,AR2
;	LDI	*+AR2(CARTRAK),AR2
;	LDI	*+AR2(OLINK4),AR0
;	LDF	*+AR0(OPOSX),R2
;	SUBF	*+AR2(OPOSX),R2
;	LDF	*+AR0(OPOSZ),R3
;	SUBF	*+AR2(OPOSZ),R3
;	CALL	ARCTANF
;	SUBF	HALFPI,R0
;	LDF	R0,R2				;FIND THETA
;	LDI	@MATRIXAI,AR2
;	CALL	FIND_YMATRIX
;
;
;	CALL	CLR_VECTORA
;	LDF	*AR5,R4
;	LDF	*+AR5(1),R5
;	STF	R4,*+AR2(X)
;	STF	R5,*+AR2(Z)
;
;
;	LDI	@MATRIXAI,R2
;	LDI	AR2,R3
;	CALL	MATRIX_MUL
;
;
;	LDI	@PLYCAR,AR3
;	LDF	*+AR3(OPOSX),R4
;	ADDF	*+AR2(X),R4
;	LDF	*+AR3(OPOSZ),R5
;	ADDF	*+AR2(Z),R5		;R4,R5 contains X/Z position
;
;
;	LDF	*+AR4(OPOSX),R2
;	SUBF	R4,R2
;	MPYF	R2,R2
;	LDF	*+AR4(OPOSZ),R1
;	SUBF	R5,R1
;	ADDF	R1,R2
;	CALL	SQRT			;R0 <- distance to new position
;
;
;	FLOAT	2000,R1
;	CMPF	R1,R0
;	BGT	KKK
;	LDF	*++AR5(2),R0
;	CMPF	-1,R0
;	BNE	JKKU
;	LDI	@CIRCLE_SI,AR5
;	BU	JKKU
;KKK
;
;
;
;	PUSHF	R4
;	PUSHF	R5
;
;	FLOAT	@NFRAMES,R1
;	MPYF	*+AR7(CD_TSPEED),R1
;	LDFLE	30,R1			;if 0 or less assume 30 voxel per frame
;	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
;	FIX	R0,R7
;
;	POPF	R5
;	POPF	R4
;
;	LDF	R4,R2			;X
;	SUBF	*+AR4(OPOSX),R2
;	LDF	R5,R3			;Z
;	SUBF	*+AR4(OPOSZ),R3
;
;
;
;	;find the theta delta to this position
;	;
;	CALL	ARCTANF			;-> R0
;	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
;
; 	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
;	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
;	FLOAT	R7,R1			;theta / number of turns to achieve
;	SUBF	1,R1
;	BLE	NODIV3
;	CALL	DIV_F			;-> R0
;NODIV3
;	CMPF	0.06926,R0		;PI/8 (maximum turning radius)
;	LDFGT	0.06926,R0
;	CMPF	-0.06926,R0
;	LDFLT	-0.06926,R0
;	STF	R0,*+AR7(DELTA_RADYDELTA)
;
;
;	PUSH	AR4
;	ADDI	OPOSX,AR4
;	CALL	CAMSCAN
;	POP	AR4
;	BNC	LLK23
;
;	NEGF	R0
;	FLOAT	-1300,R1
;	SUBF	R1,R0
;	NEGF	R0
;	ADDF	*+AR4(OPOSY),R0
;	STF	R0,*+AR7(CD_DHEIGHT)
;LLK23
;
;
;	LDF	*+AR7(CD_SPEED),R3
;	ADDF	*+AR7(CD_ACC),R3
;
;	FLOAT	300,R1			;MAX speed
;
;	CMPF	R1,R3
;	LDFGT	R1,R3
;	STF	R3,*+AR7(CD_SPEED)
;
;
;
;	LDF	*+AR7(DELTA_RADYDELTA),R2
;	CALL	FSL_MOVE
;
;
;	SLEEP	1
;	BU	CIRCL_L
;*----------------------------------------------------------------------------
;
*/

/*
 *----------------------------------------------------------------------------
 *CHOPPER SEQUENCE:
 *
 *
 *	come from behind and overhead and zoom ahead of player
 *	rotate tilted forward (moving with player)
 *	radio noise and such
 *	zoom off to left or right
 *	pull obj and sleep for awhile
 *	reinsert objects and redo sequence
 *
 *
 *
 *
 */
void CHOPPER(void) {
    // 	;BEGIN INITIALIZATION CODE
    // 	;
    // asm 00007D33: 	LDI	@(_plyr1+PLY_PROC),AR2	;FIRST SETUP THAT WE FOLLOW THE PLAYERS PROC
    // asm 00007D34: 	CALL	PRC_FOLLOW
    // asm 00007D35: 	CALL	OBJ_GET
    // asm 00007D36: 	BC	SUICIDE
    // asm 00007D37: 	LDI	AR0,AR4
    // asm 00007D38: 	LDIL	helli,R0
    // asm 00007D3B: 	STI	R0,*+AR4(OROMDATA)
    // asm 00007D3C: 	CLRF	R0
    // asm 00007D3D: 	STF	R0,*+AR4(OPOSX)
    // asm 00007D3E: 	FLOAT	-20,R0
    // asm 00007D3F: 	STF	R0,*+AR4(OPOSY)
    // asm 00007D40: 	FLOAT	3500,R0
    // asm 00007D41: 	STF	R0,*+AR4(OPOSZ)
    // asm 00007D42: 	LDI	AR4,AR2
    // asm 00007D43: 	CALL	OBJ_INSERT
    // asm 00007D44: 	CALL	_CARV0
    // asm 00007D45: 	LDI	*+AR4(OCARBLK),AR5
    // asm 00007D46: 	CALL	SETDYNAOBJ
    // asm 00007D47: 	LDF	0,R2
    // asm 00007D48: 	STF	R2,*+AR4(ORADX)
    // asm 00007D49: 	STF	R2,*+AR4(ORADY)
    // asm 00007D4A: 	STF	R2,*+AR4(CRADZ)
    // asm 00007D4B: 	LDI	AR4,AR2
    // asm 00007D4C: 	ADDI	OMATRIX,AR2
    // asm 00007D4D: 	CALL	FIND_YMATRIX
    // asm 00007D4E: 	CLRI	R0
    // asm 00007D4F: 	STI	R0,*+AR7(CD_PASS_COUNT)
    // asm 00007D50: 	STI	R0,*+AR7(CD_BOMB_COUNT)
    // asm 00007D51: 	LDI	DRONE_C|HELICOPTER,R0
    // asm 00007D52: 	STI	R0,*+AR4(OID)
    // asm 00007D53: 	STI	R0,*+AR7(PID)
    // asm 00007D54: 	STI	AR7,*+AR4(OPLINK)
    // asm 00007D55: 	RANDN	2
    // asm 00007D57: 	ADDI	1,R0
    // asm 00007D58: 	STI	R0,*+AR7(CD_MAX_PASSES)
    // asm 00007D59: 	CLRI	R0
    // asm 00007D5A: 	STI	R0,*+AR7(CD_DOATTACK)
    // 	;
    // 	;END OF INITIALIZATION CODE
    // asm 00007D5B: 	CLRI	R0
    // asm 00007D5C: 	STI	R0,*+AR7(CD_LASTPASS)
    // 	;what type of pass to make?
CHOPPER_PASS:
    // asm 00007D5D: 	LDI	@HELI_ABORT,R0
    // asm 00007D5E: 	BNZ	CHOPPER_DIE
    // asm 00007D5F: 	LDI	*+AR7(CD_LASTPASS),R0
    // asm 00007D60: 	BNZ	CHOPPER_DIE
    // asm 00007D61: 	LDI	*+AR7(CD_PASS_COUNT),R0
    // asm 00007D62: 	INC	R0
    // asm 00007D63: 	CMPI	*+AR7(CD_MAX_PASSES),R0
    // asm 00007D64: 	BGT	DOTHEMOVE
    // asm 00007D65: 	STI	R0,*+AR7(CD_PASS_COUNT)
    // asm 00007D66: 	BU	ONCOMMING_BUZZ
DOTHEMOVE:
    // asm 00007D67: 	LDI	1,R0
    // asm 00007D68: 	STI	R0,*+AR7(CD_LASTPASS)
    // asm 00007D69: 	RANDN	2
    // asm 00007D6B: 	STI	R0,*+AR7(CD_DOATTACK)
    // asm 00007D6C: 	LDI	R0,R0
    // asm 00007D6D: 	BNZ	ONCOMMING_BUZZ
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOPPER", 0, 0);
    UNIMPL();
}

static void FORWARD_BUZZ(void) {
    // 	;FORWARD BUZZ INIT. CODE
    // 	;
    // asm 00007D6E: 	RANDN	20
    // asm 00007D70: 	ADDI	20,R0
    // asm 00007D71: 	STI	R0,*+AR7(CD_MAX_BOMBS)
    // asm 00007D72: 	LDF	0,R0
    // asm 00007D73: 	STF	R0,*+AR4(ORADX)
    // asm 00007D74: 	STF	R0,*+AR4(ORADY)
    // asm 00007D75: 	STF	R0,*+AR4(CRADZ)
    // asm 00007D76: 	STF	R0,*+AR4(OVELX)
    // asm 00007D77: 	STF	R0,*+AR4(OVELY)
    // asm 00007D78: 	STF	R0,*+AR4(OVELZ)
    // 	;
    // 	;find where player is
    // asm 00007D79: 	LDI	@PLYCBLK,AR2
    // asm 00007D7A: 	NEGF	*+AR2(CARDIST2CNTR),R0
    // asm 00007D7B: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm 00007D7C: 	LDI	*+AR2(CARTRAK),AR2
    // asm 00007D7D: 	LDI	*+AR2(OBLINK4),AR2
    // asm 00007D7E: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm 00007D7F: 	LDI	*+AR2(OUSR1),R0
    // asm 00007D80: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm 00007D81: 	CALL	DELTA_SUB_FUNCTION			;MATRIXA,VECTORA,R2
    // asm 00007D82: 	LDP	@_VECTORA
    // asm 00007D83: 	LDF	*+AR2(OPOSX),R0
    // asm 00007D84: 	ADDF	@_VECTORA+X,R0
    // asm 00007D85: 	STF	R0,*+AR4(OPOSX)
    // asm 00007D86: 	LDF	*+AR2(OPOSY),R0
    // asm 00007D87: 	FLOAT	1000,R1
    // asm 00007D88: 	SUBF	R1,R0
    // asm 00007D89: 	ADDF	@_VECTORA+Y,R0
    // asm 00007D8A: 	STF	R0,*+AR4(OPOSY)
    // asm 00007D8B: 	LDF	*+AR2(OPOSZ),R0
    // asm 00007D8C: 	ADDF	@_VECTORA+Z,R0
    // asm 00007D8D: 	STF	R0,*+AR4(OPOSZ)
    // asm 00007D8E: 	SETDP
    // 	;initialize Ytheta to the intentional direction
    // asm 00007D8F: 	STF	R2,*+AR4(ORADY)
    // asm 00007D90: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm 00007D91: 	LDI	AR4,AR2
    // asm 00007D92: 	ADDI	OMATRIX,AR2
    // asm 00007D93: 	CALL	FIND_YMATRIX
    // asm 00007D94: 	FLOAT	60,R0
    // asm 00007D95: 	STF	R0,*+AR7(CD_ACC)
    // asm 00007D96: 	LDI	0,R0
    // asm 00007D97: 	STI	R0,*+AR7(CD_BOMBTIK)
    // asm 00007D98: 	LDI	CM_DB,R0
    // asm 00007D99: 	STI	R0,*+AR7(CD_MODE)
    // 	;
    // 	;END FORWARD BUZZ INIT. CODE
CHOPPER_L:
    // asm 00007D9A: 	LDI	@HELI_ABORT,R0
    // asm 00007D9B: 	BNZ	FLYAWAY
    // asm 00007D9C: 	LDI	@PLYCBLK,AR2
    // asm 00007D9D: 	NEGF	*+AR2(CARDIST2CNTR),R0
    // asm 00007D9E: 	STF	R0,*+AR7(DELTA_XLANE)
    // ;	LDF	*+AR2(CARSPEED),R1
    // ;	CMPF	60,R1
    // ;	BGT	NOCIRC
    // ;	JSRP	CIRCLE_JOIN
    // ;NOCIRC
    // asm 00007D9F: 	CALL	DIST_TO_PLYR
    // asm 00007DA0: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm 00007DA1: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm 00007DA2: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm 00007DA3: 	FLOAT	10000,R2
    // asm 00007DA4: 	CMPF	R2,R0
    // asm 00007DA5: 	BGT	FLYAWAY
    // asm 00007DA6: 	CALL	HELI_SND
    // 	;see if we should track the next piece
CHECK_DIST:
    // asm 00007DA7: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm 00007DA8: 	RS	8,R0
    // asm 00007DA9: 	LDI	@SECTIONIDX,R1
    // asm 00007DAA: 	SUBI	@DGROUP_COUNT,R1
    // asm 00007DAB: 	CMPI	R1,R0
    // asm 00007DAC: 	BLE	CHOPPER_DIE
    // asm 00007DAD: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007DAE: 	LDI	*+AR2(OLINK4),R0
    // asm 00007DAF: 	BZ	CHOPPER_SLP
    // 	;old xlane determination code position
    // asm 00007DB0: 	CALL	DELTA_GET_TRACK_POS		;CHECK IF WE SHOULD ADVANCE
    // asm 00007DB1: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm 00007DB2: 	CMPF	R1,R0
    // asm 00007DB3: 	BGT	THIS_PIECE
    // asm 00007DB4: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007DB5: 	LDI	*+AR2(OLINK4),R0
    // asm 00007DB6: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm 00007DB7: 	LDI	R0,AR0
    // asm 00007DB8: 	LDI	*+AR0(OUSR1),R0
    // asm 00007DB9: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm 00007DBA: 	BU	CHECK_DIST
THIS_PIECE:
    // asm 00007DBB: 	FLOAT	@NFRAMES,R1
    // asm 00007DBC: 	MPYF	*+AR7(CD_TSPEED),R1
    // asm 00007DBD: 	LDFLE	30,R1			;if 0 or less assume 30 voxel per frame
    // asm 00007DBE: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm 00007DBF: 	FIX	R0,R7
    // asm 00007DC0: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00007DC1: 	LDP	@_VECTORA		;lane position
    // asm 00007DC2: 	LDF	*+AR2(OPOSX),R2		;X
    // asm 00007DC3: 	SUBF	*+AR4(OPOSX),R2
    // asm 00007DC4: 	ADDF	@_VECTORA+X,R2
    // asm 00007DC5: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm 00007DC6: 	SUBF	*+AR4(OPOSZ),R3
    // asm 00007DC7: 	ADDF	@_VECTORA+Z,R3
    // asm 00007DC8: 	SETDP
    // 	;find the theta delta to this position
    // 	;
    // asm 00007DC9: 	CALL	ARCTANF			;-> R0
    // asm 00007DCA: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm 00007DCB:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm 00007DCC: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm 00007DCD: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm 00007DCE: 	SUBF	1,R1
    // asm 00007DCF: 	BLE	NODIV2
    // asm 00007DD0: 	CALL	DIV_F			;-> R0
NODIV2:
    // asm 00007DD1: 	CMPF	0.3926,R0		;PI/8 (maximum turning radius)
    // asm 00007DD2: 	LDFGT	0.3926,R0
    // asm 00007DD3: 	CMPF	-0.3926,R0
    // asm 00007DD4: 	LDFLT	-0.3926,R0
    // asm 00007DD5: 	STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm 00007DD6: 	CALL	GET_CLOSEST_TRAK
    // asm 00007DD7: 	STI	AR0,*+AR7(CD_CLOSEROAD)
    // asm 00007DD8: 	LDF	*+AR7(CD_ODHEIGHT),R0
    // asm 00007DD9: 	STF	R0,*+AR7(CD_DHEIGHT)
    // asm 00007DDA: 	LDI	*+AR7(CD_CLOSEROAD),AR0
    // asm 00007DDB: 	FIX	*+AR0(OPOSY),R0
    // asm 00007DDC: 	SUBI	1300,R0
    // asm 00007DDD: 	FLOAT	R0
    // asm 00007DDE: 	STF	R0,*+AR7(CD_DHEIGHT)
    // asm 00007DDF: 	LDI	*+AR7(CD_CLOSEROAD),AR0
    // asm 00007DE0: 	LDI	*+AR0(OUSR1),R0
    // asm 00007DE1: 	LDI	@PLYCBLK,AR1
    // asm 00007DE2: 	LDI	*+AR1(CARTRAK),AR2
    // asm 00007DE3: 	LDI	*+AR2(OUSR1),R1
    // asm 00007DE4: 	CMPI	R1,R0
    // asm 00007DE5: 	BLT	ISBEND
    // asm 00007DE6: 	BGT	ISAHED
    // 	;tied case, must be exact
    // 	;
    // asm 00007DE7: 	LDI	*+AR2(OLINK4),AR2
    // asm 00007DE8: 	LDI	@PLYCAR,AR0
    // asm 00007DE9: 	LDF	*+AR0(OPOSX),R0
    // asm 00007DEA: 	SUBF	*+AR2(OPOSX),R0
    // asm 00007DEB: 	MPYF	R0,R0
    // asm 00007DEC: 	LDF	*+AR0(OPOSZ),R1
    // asm 00007DED: 	SUBF	*+AR2(OPOSZ),R1
    // asm 00007DEE: 	MPYF	R1,R1
    // asm 00007DEF: 	ADDF	R1,R0
    // asm 00007DF0: 	LDF	*+AR4(OPOSX),R2
    // asm 00007DF1: 	SUBF	*+AR2(OPOSX),R2
    // asm 00007DF2: 	MPYF	R2,R2
    // asm 00007DF3: 	LDF	*+AR4(OPOSZ),R1
    // asm 00007DF4: 	SUBF	*+AR2(OPOSZ),R1
    // asm 00007DF5: 	MPYF	R1,R1
    // asm 00007DF6: 	ADDF	R2,R1
    // asm 00007DF7: 	CMPF	R0,R1
    // asm 00007DF8: 	BGT	ISBEND
    // asm 00007DF9: ISAHED	;is ahead of
    // asm 00007DF9: 	LDI	1,R0
    // asm 00007DFA: 	STI	R0,*+AR7(CD_AHEADP)
    // asm 00007DFB: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm 00007DFC: 	FLOAT	1750,R2
    // asm 00007DFD: 	CMPF	R2,R1
    // asm 00007DFE: 	BLT	LITTLE
    // asm 00007DFF: 	LDF	*+AR7(CD_SPEED),R0
    // asm 00007E00: 	LDF	R0,R2
    // asm 00007E01: 	MPYF	1.1,R2
    // asm 00007E02: 	LDF	*+AR1(CARSPEED),R1
    // asm 00007E03: 	ADDF	*+AR7(CD_ACC),R0
    // asm 00007E04: 	MPYF	1.5,R1
    // asm 00007E05: 	CMPF	R1,R0
    // asm 00007E06: 	LDFGT	R1,R0
    // asm 00007E07: 	CMPF	R2,R0				;never rad change on our vel.
    // asm 00007E08: 	LDFGT	R2,R0
    // asm 00007E09: 	STF	R0,*+AR7(CD_SPEED)
    // asm 00007E0A: 	BU	L88
ISBEND:
    // asm 00007E0B: 	CLRI	R0
    // asm 00007E0C: 	STI	R0,*+AR7(CD_AHEADP)
    // asm 00007E0D: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm 00007E0E: 	FLOAT	3500,R2
    // asm 00007E0F: 	CMPF	R2,R1
    // asm 00007E10: 	BLT	LITTLE
    // asm 00007E11: 	LDF	*+AR1(CARSPEED),R0
    // asm 00007E12: 	MPYF	3,R0
    // asm 00007E13: 	FLOAT	600,R1
    // asm 00007E14: 	CMPF	R1,R0
    // asm 00007E15: 	LDFLT	R1,R0
    // asm 00007E16: 	STF	R0,*+AR7(CD_SPEED)
    // asm 00007E17: 	BU	L88
LITTLE:
    // asm 00007E18: 	LDF	*+AR7(CD_SPEED),R2
    // asm 00007E19: 	MPYF	1.1,R2
    // asm 00007E1A: 	LDF	*+AR1(CARSPEED),R0
    // asm 00007E1B: 	MPYF	2.0,R0
    // asm 00007E1C: 	FLOAT	500,R1
    // asm 00007E1D: 	CMPF	R1,R0
    // asm 00007E1E: 	LDFLT	R1,R0
    // asm 00007E1F: 	CMPF	R2,R0
    // asm 00007E20: 	LDFGT	R2,R0
    // asm 00007E21: 	STF	R0,*+AR7(CD_SPEED)
L88:
    // asm 00007E22: LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm 00007E23: 	LDF	*+AR7(CD_SPEED),R3
    // asm 00007E24: 	CALL	FSL_MOVE
    // asm 00007E25: 	LDI	*+AR7(CD_MODE),R4
    // asm 00007E26: 	CMPI	CM_DB,R4
    // asm 00007E27: 	BNE	NO_BOMBS
    // asm 00007E28: 	LDI	*+AR7(CD_BOMBTIK),R0
    // asm 00007E29: 	DEC	R0
    // asm 00007E2A: 	STI	R0,*+AR7(CD_BOMBTIK)
    // asm 00007E2B: 	BGT	NO_BOMBS
    // asm 00007E2C: 	LDI	*+AR7(CD_AHEADP),R0
    // asm 00007E2D: 	BZ	NO_BOMBS
    // asm 00007E2E: 	LDI	@POSITION,R0
    // asm 00007E2F: 	CMPI	1,R0
    // asm 00007E30: 	BNE	FLYAWAY
    // asm 00007E31: 	FLOAT	2300,R0
    // asm 00007E32: 	CMPF	*+AR7(DELTA_PLYRDIST),R0
    // asm 00007E33: 	BGT	NO_BOMBS
    // asm 00007E34: 	RANDN	10
    // asm 00007E36: 	ADDI	10,R0
    // asm 00007E37: 	STI	R0,*+AR7(CD_BOMBTIK)
    // asm 00007E38: 	LDI	R0,R4
    // asm 00007E39: 	LDI	*+AR7(CD_BOMB_COUNT),R0
    // asm 00007E3A: 	INC	R0
    // asm 00007E3B: 	CMPI	*+AR7(CD_MAX_BOMBS),R0
    // asm 00007E3C: 	BGT	FLYAWAY		;NO_BOMBS
    // asm 00007E3D: 	STI	R0,*+AR7(CD_BOMB_COUNT)
    // ;	CREATE	BOMB_PROC,3434
NO_BOMBS:
CHOPPER_SLP:
    // asm 00007E3E: 	SLEEP	1
    // asm 00007E40: 	B	CHOPPER_L
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FORWARD_BUZZ", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FLYAWAY(void) {
    // asm 00007E41: 	CLRI	AR6			;flag
    // asm 00007E42: 	LDI	100,AR5
    // asm 00007E43: 	LDF	-10,R7
    // asm 00007E44: 	LDF	0.015,R0
    // asm 00007E45: 	CALL	SFRAND
    // asm 00007E46: 	LDF	R0,R0
    // asm 00007E47: 	BLT	KKII
    // asm 00007E48: 	ADDF	0.01,R0
    // asm 00007E49: 	BU	KKUU
KKII:
    // asm 00007E4A: SUBF	0.01,R0
KKUU:
    // asm 00007E4B: STF	R0,*+AR7(CD_FLYTDIR)
    // asm 00007E4C: FLYAWAY_LP
    // asm 00007E4C: 	CALL	DIST_TO_PLYR
    // asm 00007E4D: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm 00007E4E: 	CALL	HELI_SND
    // asm 00007E4F: 	SUBF	3,R7
    // asm 00007E50: 	CMPF	-95,R7
    // asm 00007E51: 	LDFLT	-95,R7
    // asm 00007E52: 	LDF	*+AR7(CD_DHEIGHT),R1
    // asm 00007E53: 	ADDF	R7,R1
    // asm 00007E54: 	STF	R1,*+AR7(CD_DHEIGHT)
    // asm 00007E55: 	LDF	*+AR7(CD_SPEED),R0
    // asm 00007E56: 	ADDF	*+AR7(CD_ACC),R0
    // asm 00007E57: 	FLOAT	MAX_SPEED,R1
    // asm 00007E58: 	CMPF	R1,R0
    // asm 00007E59: 	LDFGT	R1,R0
    // asm 00007E5A: 	STF	R0,*+AR7(CD_SPEED)
    // asm 00007E5B: 	LDF	*+AR7(CD_SPEED),R3
    // asm 00007E5C: 	LDF	*+AR7(CD_FLYTDIR),R2
    // asm 00007E5D: 	CALL	FSL_MOVE
    // asm 00007E5E: 	SLEEP	1
    // asm 00007E60: 	DBU	AR5,FLYAWAY_LP
    // asm 00007E61: 	BU	CHOPPER_PASS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYAWAY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CHOPPER_DIE(void) {
    // asm 00007E62: 	LDI	*+AR4(OCARBLK),AR2
    // asm 00007E63: 	CALL	DELCAR
    // asm 00007E64: 	LDI	AR4,AR2
    // asm 00007E65: 	CALL	OBJ_DELETE
    // asm 00007E66: 	LDI	HELI_SNDLP,AR2		;may want to add in volume effects
    // asm 00007E67: 	CALL	KILLSNDFX
    // asm 00007E68: 	LDI	*+AR7(CD_ANIPROC),AR2
    // asm 00007E69: 	CALL	PRC_KILL
    // asm 00007E6A: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOPPER_DIE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR4	OBJECT
 *
 */
static void FIND_YX_MATRIX(void) {
    // asm 00007E6B: 	LDF	*+AR4(ORADY),R2
    // asm 00007E6C: 	LDI	AR4,AR2
    // asm 00007E6D: 	ADDI	OMATRIX,AR2
    // asm 00007E6E: 	CALL	FIND_YMATRIX
    // asm 00007E6F: 	LDI	@MATRIXAI,AR2
    // asm 00007E70: 	LDF	*+AR4(ORADX),R2
    // asm 00007E71: 	CALL	FIND_XMATRIX
    // asm 00007E72: 	LDI	@MATRIXAI,R2
    // asm 00007E73: 	LDI	AR4,R3
    // asm 00007E74: 	ADDI	OMATRIX,R3
    // asm 00007E75: 	LDI	R3,AR2
    // asm 00007E76: 	CALL	CONCATMAT
    // asm 00007E77: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_YX_MATRIX", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void SETDYNAOBJ(void) {
    // asm 00007E78: 	LDI	O_DYNAMIC,R0	 	;MAKE PARENT OBJECT DYNAMIC
    // asm 00007E79: 	OR	*+AR4(OFLAGS),R0
    // asm 00007E7A: 	STI	R0,*+AR4(OFLAGS)
    // asm 00007E7B: 	LDL	CHOPPERDYNA,AR2
    // asm 00007E7C: 	LDI	AR4,AR3
    // asm 00007E7D: 	ADDI	ODYNALIST,AR3
    // *INIT DYNA OBJECTS
    // asm 00007E7E: 	LDI	*AR2++,RC		;GET DYNAMIC OBJECT COUNT
    // asm 00007E7F: 	RPTB	CHOPLP
    // asm 00007E80:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
    // asm 00007E81: 	STI	AR0,*AR3
    // asm 00007E82: 	LDF	*AR2++,R0
    // asm 00007E83: 	STF	R0,*+AR0(DYNACENTERX)
    // asm 00007E84: 	STF	R0,*+AR0(DYNATRANSX)
    // asm 00007E85: 	LDF	*AR2++,R0
    // asm 00007E86: 	STF	R0,*+AR0(DYNACENTERY)
    // asm 00007E87: 	STF	R0,*+AR0(DYNATRANSY)
    // asm 00007E88: 	LDF	*AR2++,R0
    // asm 00007E89: 	STF	R0,*+AR0(DYNACENTERZ)
    // asm 00007E8A: 	STF	R0,*+AR0(DYNATRANSZ)
    // asm 00007E8B: 	LDI	*AR2++,R0
    // asm 00007E8C: 	STI	R0,*+AR0(DYNANVERTS)
    // asm 00007E8D: 	LDI	*AR2++,R0
    // asm 00007E8E: 	STI	R0,*+AR0(DYNAFLAG)
    // asm 00007E8F: 	STI	AR4,*+AR0(DYNAPARENT)
CHOPLP:
    // asm 00007E90: LDI	AR0,AR3
    // asm 00007E91: 	LDI	0,R0
    // asm 00007E92: 	STI	R0,*AR3			;LAST LINK IS ZERO, DUDES
    // asm 00007E93: 	LDI	AR3,AR0
    // asm 00007E94: 	ADDI	DYNAMATRIX,AR0
    // asm 00007E95: 	CALL	INITMAT
    // asm 00007E96: 	LDL	CHOPPERANI,AR2
    // asm 00007E97: 	LDI	DRONE_C|ANI_T,R2
    // asm 00007E98: 	CALL	PRC_CREATE_CHILD
    // asm 00007E99: 	STI	AR0,*+AR7(CD_ANIPROC)
    // asm 00007E9A: 	STI	AR0,*+AR4(ORADZ)
    // asm 00007E9B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETDYNAOBJ", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CHOPPERANI(void) {
    // asm 00007E9C: 	LONGROUT
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
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOPPERANI", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *FSL_MOVE	Frictionless Movement
 *
 *PARAMETERS
 *	R2	THETA DELTA (CHANGE IN THETA)
 *	R3	SPEED
 *
 */
static void FSL_MOVE(void) {
    // ;	LDP	@NFRAMES
    // ;	FLOAT	@NFRAMES,R0
    // ;	MPYF	R0,R3
    // 	;
    // 	;move to better y height
    // 	;
    // asm 00007E9D: 	LDF	*+AR7(CD_DHEIGHT),R0
    // asm 00007E9E: 	SUBF	*+AR4(OPOSY),R0
    // asm 00007E9F: 	LDF	*+AR4(OVELY),R1
    // asm 00007EA0: 	MPYF	0.85,R1
    // asm 00007EA1: 	MPYF	0.15,R0
    // asm 00007EA2: 	ADDF	R0,R1
    // asm 00007EA3: 	CMPF	60,R1
    // asm 00007EA4: 	LDFGT	60,R1
    // asm 00007EA5: 	CMPF	-60,R1
    // asm 00007EA6: 	LDFLT	-60,R1
    // asm 00007EA7: 	STF	R1,*+AR4(OVELY)
    // asm 00007EA8: 	PUSHF	R2
    // asm 00007EA9: 	ADDF	*+AR4(ORADY),R2
    // asm 00007EAA:  	STF	R2,*+AR4(ORADY)
    // asm 00007EAB: 	LDI	@MATRIXAI,AR2
    // asm 00007EAC: 	CALL	FIND_YMATRIX
    // asm 00007EAD: 	LDI	AR2,R2
    // asm 00007EAE: 	CALL	CLR_VECTORA
    // asm 00007EAF: 	STF	R3,*+AR2(Z)
    // asm 00007EB0: 	LDF	*+AR4(OVELY),R0
    // asm 00007EB1: 	STF	R0,*+AR2(Y)
    // asm 00007EB2: 	LDI	AR2,R3
    // asm 00007EB3: 	CALL	MATRIX_MUL
    // asm 00007EB4: 	LDF	*+AR2(X),R0
    // asm 00007EB5: 	LDF	*+AR4(OVELX),R1
    // asm 00007EB6: 	MPYF	0.1,R0
    // asm 00007EB7: 	MPYF	0.9,R1
    // asm 00007EB8: 	ADDF	R1,R0
    // asm 00007EB9: 	STF	R0,*+AR4(OVELX)
    // asm 00007EBA: 	FLOAT	@NFRAMES,R1
    // asm 00007EBB: 	MPYF	R1,R0
    // asm 00007EBC: 	ADDF	*+AR4(OPOSX),R0
    // asm 00007EBD: 	STF	R0,*+AR4(OPOSX)
    // asm 00007EBE: 	LDF	*+AR2(Y),R0
    // asm 00007EBF: 	LDF	*+AR4(OVELY),R1
    // asm 00007EC0: 	MPYF	0.9,R0
    // asm 00007EC1: 	MPYF	0.1,R1
    // asm 00007EC2: 	ADDF	R1,R0
    // asm 00007EC3: 	STF	R0,*+AR4(OVELY)
    // ;	FLOAT	@NFRAMES,R1
    // ;	MPYF	R1,R0
    // asm 00007EC4: 	ADDF	*+AR4(OPOSY),R0
    // asm 00007EC5: 	STF	R0,*+AR4(OPOSY)
    // asm 00007EC6: 	LDF	*+AR2(Z),R0
    // asm 00007EC7: 	LDF	*+AR4(OVELZ),R1
    // asm 00007EC8: 	MPYF	0.1,R0
    // asm 00007EC9: 	MPYF	0.9,R1
    // asm 00007ECA: 	ADDF	R1,R0
    // asm 00007ECB: 	STF	R0,*+AR4(OVELZ)
    // asm 00007ECC: 	FLOAT	@NFRAMES,R1
    // asm 00007ECD: 	MPYF	R1,R0
    // asm 00007ECE: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00007ECF: 	STF	R0,*+AR4(OPOSZ)
    // asm 00007ED0:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm 00007ED1: 	LDI	AR4,AR2
    // asm 00007ED2: 	ADDI	OMATRIX,AR2
    // asm 00007ED3: 	CALL	FIND_YMATRIX
    // asm 00007ED4: 	LDF	*+AR7(CD_TSPEED),R0
    // asm 00007ED5: 	LDF	*+AR4(ORADX),R2
    // asm 00007ED6: 	LDI	@MATRIXBI,AR2
    // asm 00007ED7: 	CALL	FIND_XMATRIX
    // asm 00007ED8: 	POPF	R2
    // asm 00007ED9: 	MPYF	25,R2
    // asm 00007EDA: 	LDF	*+AR4(CRADZ),R0
    // asm 00007EDB: 	MPYF	0.1,R2
    // asm 00007EDC: 	MPYF	0.9,R0
    // asm 00007EDD: 	ADDF	R0,R2
    // asm 00007EDE: 	CMPF	0.4,R2
    // asm 00007EDF: 	LDFGT	0.4,R2
    // asm 00007EE0: 	CMPF	-0.4,R2
    // asm 00007EE1: 	LDFLT	-0.4,R2
    // asm 00007EE2: 	STF	R2,*+AR4(CRADZ)
    // asm 00007EE3: 	LDI	@MATRIXAI,AR2
    // asm 00007EE4: 	CALL	FIND_ZMATRIX
    // asm 00007EE5: 	LDI	@MATRIXBI,AR2
    // asm 00007EE6: 	LDI	@MATRIXAI,R2
    // asm 00007EE7: 	LDI	AR2,R3
    // asm 00007EE8: 	CALL	CONCATMATV		;X * Z -> matA
    // asm 00007EE9: 	LDI	AR4,R2
    // asm 00007EEA: 	ADDI	OMATRIX,R2
    // asm 00007EEB: 	LDI	R2,R3
    // asm 00007EEC: 	CALL	CONCATMATV
    // 	;FIND WHAT OUR SPEED ACTUAL IS
    // asm 00007EED: 	LDF	*+AR4(OVELX),R2
    // asm 00007EEE: 	MPYF	R2,R2
    // asm 00007EEF: 	LDF	*+AR4(OVELY),R1
    // asm 00007EF0: 	MPYF	R1,R1
    // asm 00007EF1: 	ADDF	R1,R2
    // asm 00007EF2: 	LDF	*+AR4(OVELZ),R1
    // asm 00007EF3: 	MPYF	R1,R1
    // asm 00007EF4: 	ADDF	R1,R2
    // asm 00007EF5: 	CALL	SQRT
    // asm 00007EF6: 	LDF	*+AR7(CD_TSPEED),R1
    // asm 00007EF7: 	STF	R0,*+AR7(CD_TSPEED)
    // asm 00007EF8: 	STF	R1,*+AR7(CD_TSPEED_OLD)
    // asm 00007EF9: 	CALL	DIV_F30
    // asm 00007EFA: 	CMPF	1.0,R0
    // asm 00007EFB: 	LDFGT	1.0,R0
    // asm 00007EFC: 	MPYF	0.2,R0
    // asm 00007EFD: 	STF	R0,*+AR4(ORADX)
    // asm 00007EFE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FSL_MOVE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
;
;*----------------------------------------------------------------------------
;*
;*PARAMETERS
;*	AR4	CHOPPER OBJECT
;*
;BOMB_PROC:
;	LDI	AR4,AR5
;	CALL	OBJ_GET
;	BC	SUICIDE
;
;	LDI	AR0,AR4
;	LDF	*+AR5(OPOSX),R0
;	STF	R0,*+AR4(OPOSX)
;	LDF	*+AR5(OPOSY),R0
;	STF	R0,*+AR4(OPOSY)
;	LDF	*+AR5(OPOSZ),R0
;	STF	R0,*+AR4(OPOSZ)
;
;	STI	AR7,*+AR4(OPLINK)
;
;	LDF	*+AR2(ORADY),R2
;	LDI	AR4,AR2
;	ADDI	OMATRIX,AR2
;	CALL	FIND_YMATRIX
;
;	LDIL	bomb,AR0
;	STI	AR0,*+AR4(OROMDATA)
;
;	LDI	1,R1
;	LS	O_PROC_B,R1
;	LDI	1,R0
;	LS	O_3DROT_B,R0
;	OR	R1,R0
;	OR	*+AR4(OFLAGS),R0
;	STI	R0,*+AR4(OFLAGS)
;
;	LDF	HALFPI,R2
;	LDI	AR4,AR2
;	ADDI	OMATRIX,AR2
;	CALL	FIND_XMATRIX
;
;	LDI	AR4,AR2
;	CALL	OBJ_INSERT
;
;
;	LDF	HALFPI,R0
;	STF	R0,*+AR4(ORADX)
;
;
;	LDF	*+AR5(OVELX),R0
;	STF	R0,*+AR4(OVELX)
;	LDF	*+AR5(OVELY),R0
;	STF	R0,*+AR4(OVELY)
;	LDF	*+AR5(OVELZ),R0
;	STF	R0,*+AR4(OVELZ)
;
;	CALL	ADD_RDDEBRIS
;	LDI	TSIGN_C|TSC_HARD,R0
;	STI	R0,*+AR4(OID)
;
;
;
;	LDI	120,AR6
;BOMB_L
;	DEC	AR6
;	CMPI	0,AR6
;	BLE	BOMBDIE
;
;	PUSH	AR4
;	ADDI	OPOSX,AR4
;	CALL	CAMSCAN
;	POP	AR4
;	BNC	BOMBDIE
;
;	FLOAT	100,R1
;	CMPF	R1,R0
;	BLE	BOMBDIE
;
;
;	LDF	*+AR4(OVELX),R0
;	MPYF	0.99,R0
;	STF	R0,*+AR4(OVELX)
;	LDF	*+AR4(OVELY),R0
;	ADDF	5,R0
;	FLOAT	50,R1
;	CMPF	R1,R0
;	LDFGT	R1,R0
;	STF	R0,*+AR4(OVELY)
;	LDF	*+AR4(OVELZ),R0
;	MPYF	0.99,R0
;	STF	R0,*+AR4(OVELZ)
;
;	FLOAT	@NFRAMES,R2
;
;	LDF	*+AR4(OVELX),R0
;	MPYF	R2,R0
;	ADDF	*+AR4(OPOSX),R0
;	STF	R0,*+AR4(OPOSX)
;
;	LDF	*+AR4(OVELY),R0
;	MPYF	R2,R0
;	ADDF	*+AR4(OPOSY),R0
;	STF	R0,*+AR4(OPOSY)
;
;	LDF	*+AR4(OVELZ),R0
;	MPYF	R2,R0
;	ADDF	*+AR4(OPOSZ),R0
;	STF	R0,*+AR4(OPOSZ)
;
;
;	LDF	*+AR4(ORADX),R2
;	MPYF	0.80,R2
;	STF	R2,*+AR4(ORADX)
;	LDI	@MATRIXAI,AR2
;	CALL	FIND_XMATRIX
;
;
;	LDF	*+AR4(ORADY),R2
;	ADDF	0.3,R2
;	CALL	NORMITS
;	STF	R2,*+AR4(ORADY)
;
;	LDI	AR4,AR2
;	ADDI	OMATRIX,AR2
;	CALL	FIND_YMATRIX
;
;	LDI	@MATRIXAI,AR2
;	LDI	AR4,R2
;	ADDI	OMATRIX,R2
;	LDI	R2,R3
;	CALL	CONCATMATV
;
;	SLEEP	1
;	BU	BOMB_L
;
;BOMBDIE
;	LDI	@EXP_ANII,AR5
;
;	LDI	*AR5,AR0
;	STI	AR0,*+AR4(OROMDATA)
;
;	ORM	O_POSTER,*+AR4(OFLAGS)
;
;	LDI	*+AR4(OFLAGS),R0
;	ANDN	O_1PAL,R0
;	OR	O_POSTER,R0
;	STI	R0,*+AR4(OFLAGS)
;
;	BU	EXLJ
;*----------------------------------------------------------------------------
*/

/*
*----------------------------------------------------------------------------
;EXP_ANII	.word	EXP_ANI
;EXP_ANI
;	;.word	exp1,exp2,exp3,exp4,exp5,exp6,exp7,-1
;	;.word	dexplo1,dexplo2,dexplo3,dexplo4,dexplo5
;	;.word	dexplo6,dexplo7,dexplo8,dexplo9,dexplo10,dexplo11,-1
;
;	.word	blast1,blast2,blast3,blast4,blast5
;	.word	blast6,blast7,blast8,blast9,blast10,-1
;
;
;
;EXPLO_SOUNDSI	.word	EXPLO_SOUNDS
;EXPLO_SOUNDS	.word	EXP1,EXP2,EXP3
;
;EXP_NMPUFF:
;	LDI	AR4,AR6
;	CALL	OBJ_GET
;	BC	SUICIDE
;	LDI	AR0,AR4
;	LDI	@EXP_ANII,AR5
;
;	LDI	*AR5,AR0
;	STI	AR0,*+AR4(OROMDATA)
;
;	LDF	*+AR6(OPOSX),R0
;	STF	R0,*+AR4(OPOSX)
;	LDF	*+AR6(OPOSY),R0
;	STF	R0,*+AR4(OPOSY)
;	LDF	*+AR6(OPOSZ),R0
;	STF	R0,*+AR4(OPOSZ)
;	ORM	O_POSTER,*+AR4(OFLAGS)
;	LDI	AR4,AR2
;	CALL	OBJ_INSERT
;EXLJ
;
;;explosions must get louder
;;
;;	RANDN	3
;	LDI	@EXPLO_SOUNDSI,AR2
;;	ADDI	R0,AR2
;	LDI	*AR2,AR2
;	CALL	ONESNDFX
;
;	;collidable explosion
;
;EXP_NMPUFFLP
;	LDI	*AR5++,AR0
;	CMPI	-1,AR0
;	BEQ	EXP_DIE
;	STI	AR0,*+AR4(OROMDATA)
;
;	SLEEP	1
;	BU	EXP_NMPUFFLP
;
;EXP_DIE
;	LDI	1,R1
;	LS	O_PROC_B,R1
;	NOT	R1
;	LDI	*+AR4(OFLAGS),R0
;	AND	R1,R0
;	STI	R0,*+AR4(OFLAGS)
;
;
;	LDI	AR4,AR2
;	CALL	OBJ_DELETE
;	DIE
;*----------------------------------------------------------------------------
*/

// *----------------------------------------------------------------------------
static void HELI_SND(void) {
    // asm 00007EFF: 	LDI	HELI_SNDLP,AR2		;may want to add in volume effects
    // asm 00007F00: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm 00007F01: 	BEQ	IS_T1
    // asm 00007F02: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm 00007F03: 	BEQ	IS_T2
    // asm 00007F04: 	CALL	ONESNDFX
    // asm 00007F05: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm 00007F06: 	BNE	NOT_T1
IS_T1:
    // asm 00007F07: 	LDF	*+AR7(DELTA_PLYRDIST),R0
    // asm 00007F08: 	FLOAT	20000,R1
    // asm 00007F09: 	CMPF	R1,R0
    // asm 00007F0A: 	LDFGT	R1,R0
    // asm 00007F0B: 	SUBRF	R1,R0
    // asm 00007F0C: 	MPYFL	0.00005,R0
    // ;	MPYF	64,R0
    // ;	ADDF	191,R0
    // asm 00007F0D: 	MPYF	127,R0
    // asm 00007F0E: 	ADDF	128,R0
    // asm 00007F0F: 	FIX	R0,R1
    // asm 00007F10: 	LDI	1,R0
    // asm 00007F11: 	CALL	SET_TRACK_VOL
    // asm 00007F12: 	BU	HEND
NOT_T1:
    // asm 00007F13: CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm 00007F14: 	BNE	NOT_T2
IS_T2:
    // asm 00007F15: 	LDF	*+AR7(DELTA_PLYRDIST),R0
    // asm 00007F16: 	FLOAT	20000,R1
    // asm 00007F17: 	CMPF	R1,R0
    // asm 00007F18: 	LDFGT	R1,R0
    // asm 00007F19: 	SUBRF	R1,R0
    // asm 00007F1A: 	MPYFL	0.00005,R0
    // ;	MPYF	64,R0
    // ;	ADDF	191,R0
    // asm 00007F1B: 	MPYF	127,R0
    // asm 00007F1C: 	ADDF	128,R0
    // asm 00007F1D: 	FIX	R0,R1
    // asm 00007F1E: 	LDI	2,R0
    // asm 00007F1F: 	CALL	SET_TRACK_VOL
    // asm 00007F20: 	BU	HEND
NOT_T2:
HEND:
    // asm 00007F21: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HELI_SND", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *RETURNS
 *	AR0	ROAD OBJECT
 *
 */
void GET_CLOSEST_TRAK(void) {
    // asm 00007F22: 	LDI	@DRIVE_LIST,R0
    // asm 00007F23: 	BZ	GETRKX  		;NULL LIST DUDES
    // asm 00007F24: 	LDI	0,AR0			;CLOSEST ROAD SEGMENT INDEX
    // asm 00007F25: 	FLOAT	7FFFH,R2		;INITIAL CLOSEST DISTANCE
    // asm 00007F26: 	MPYF	R2,R2
    // asm 00007F27: 	LDF	*+AR4(OPOSX),R3
    // asm 00007F28: 	LDF	*+AR4(OPOSZ),R4
    // asm 00007F29: 	LDI	R0,AR2
GETRK:
    // asm 00007F2A: 	LDI	*+AR2(OID),R0	    	;IS THIS REAL ROAD?
    // asm 00007F2B: 	AND	0FF0H,R0
    // asm 00007F2C: 	CMPI	300H,R0
    // asm 00007F2D: 	BNE	GETRKL			;NOPE...
    // asm 00007F2E: 	LDF	R3,R0
    // asm 00007F2F: 	SUBF	*+AR2(OPOSX),R0
    // asm 00007F30: 	MPYF	R0,R0
    // asm 00007F31: 	LDF	R4,R1
    // asm 00007F32: 	SUBF	*+AR2(OPOSZ),R1
    // asm 00007F33: 	MPYF	R1,R1
    // asm 00007F34: 	ADDF	R0,R1
    // asm 00007F35: 	CMPF	R1,R2
    // asm 00007F36: 	BLE	GETRKL
    // asm 00007F37: 	LDI	AR2,AR0
    // asm 00007F38: 	LDF	R1,R2
GETRKL:
    // asm 00007F39: 	LDI	*+AR2(OLINK3),R0
    // asm 00007F3A: 	BNZD	GETRK
    // asm 00007F3B: 	LDI	R0,AR2
    // asm 00007F3C: 	NOP
    // asm 00007F3D: 	NOP
    // 	;---->	BNZ	GETRK
    // ;	STI	AR0,*+AR5(CARTRAK)	;SAVE TRACK SECTION
GETRKX:
    // asm 00007F3E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_CLOSEST_TRAK", 0, 0);
    UNIMPL();
}

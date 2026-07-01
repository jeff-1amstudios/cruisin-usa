
#include "rho.h"
#include "../core/machine.h"
#include "cmos.h"
#include "comm.h"
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
 * Source module: asm/RHO.ASM
 */

void RHO_DRONE(void);
static void RHO_LP(void);
static void CKCAROFF(void);
void RHO_DIE(void);
static void RHO_ISHIT(void);
static void SEND_RHO_XSFER(void);
void DECODE_RHO_XSFER(void);
static void RHO_ACTIVE_XSFER(void);
static void SEND_RHO_KILL(void);
void DECODE_RHO_KILL(void);

#define B4HIT_TABLEI B4HIT_TABLE
#define RHO_TABLEI RHO_TABLE
#define RHO_ACTIVE_XSFERI RHO_ACTIVE_XSFER
#define RHO_DIE1I RHO_DIE1

void OM_DRONE(void);
void SEND_RHO_CREATE(void);
void SEND_RHO_POS(void);
void COMPTRAK(void);
void DECODE_RACER_XSFER(void);
void FIND_DRONE(void);
void FIND_DYNA(void);

static int RHO_TABLE[51];

/*
 *----------------------------------------------------------------------------
 *RHO	(ONCOMING TRAFFIC)
 *
 *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

#define RHO_WEAVER 1 // DRUNK ONCOMER
/*
 *
 *YELL B4 HIT TO PLAYER
 */
#define B4HIT_SIZE 6
/* asm: B4HIT_TABLE	.word	RH_GOFORIT,RH_TAKEHIM,RH_BABESCREAM1 */
/* asm: 	.word	RH_BABEWHOA,RH_HURRYUP,RH_GONNACRASH */
/* asm: 	 */
static int B4HIT_TABLE[] = {
    RH_GOFORIT,
    RH_TAKEHIM,
    RH_BABESCREAM1,
    RH_BABEWHOA,
    RH_HURRYUP,
    RH_GONNACRASH,
};
/*
 *RHO FLAGS (RF_)
 *
 *
 */
#define RF_WEAVER 0x001 // may this become a weaver (SUICIDE)
#define RF_LONG 0x002   // Eugenes system can't handle long vehicles under circumstances
#define RT_VEHIDX 0
#define RT_FLAG 1
#define RT_ONSCREAM 2
#define RT_SIZE 3
/* asm: NOLONG_VEHICLES	.bss	NOLONG_VEHICLES,1 */
int NOLONG_VEHICLES;
/*
*
*RHO_TABLE
*	.word	VEHICLE_INDEX,RHO_FLAG
*	.word	ONCOMESCREAM	;either fixed or general
*
;	.word	SBUS_MOD,RF_LONG,BUSDOPL
*/
#define RHO_TABLE_LENGTH 17
/* asm: RHO_TABLE: */
/* asm: 	.word	GTRUCK_MOD,0,DIESEL_DOPPLER */
/* asm: 	.word	FTRUCK_MOD,RF_LONG,TRUCKHORN_BLAST */
/* asm: 	.word	CBUS_MOD,RF_LONG,BUSDOPL */
/* asm: 	.word	COPCAR_MOD,0,CARDOPL2 */
/* asm: 	.word	MUSCLE_MOD,RF_WEAVER,CARDOPL3 */
/* asm: 	 */
/* asm: 	.word	CARAVAN_MOD,0,CARDOPL3 */
/* asm: 	.word	SBUS_MOD,RF_LONG,BUSDOPL */
/* asm: 	.word	PTRUCKG_MOD,0,SEMIDOPL */
/* asm: 	.word	MUSTANG_MOD,0,CARDOPL2 */
/* asm: 	.word	JEEP_MOD,0,CARDOPL1 */
/* asm: 	 */
/* asm: 	.word	GTRUCK_MOD,0,DIESEL_DOPPLER */
/* asm: 	.word	COPCAR_MOD,0,CARDOPL2 */
/* asm: 	.word	MUSCLE_MOD,RF_WEAVER,CARDOPL3 */
/* asm: 	.word	CARAVAN_MOD,0,CARDOPL3 */
/* asm: 	 */
/* asm: 	.word	PTRUCKG_MOD,0,SEMIDOPL */
/* asm: 	.word	MUSTANG_MOD,0,CARDOPL2 */
/* asm: 	.word	JEEP_MOD,0,CARDOPL1 */
/* asm: 	 */
/* asm: 	 */
static int RHO_TABLE[] = {
    GTRUCK_MOD,
    0,
    DIESEL_DOPPLER,
    FTRUCK_MOD,
    RF_LONG,
    TRUCKHORN_BLAST,
    CBUS_MOD,
    RF_LONG,
    BUSDOPL,
    COPCAR_MOD,
    0,
    CARDOPL2,
    MUSCLE_MOD,
    RF_WEAVER,
    CARDOPL3,
    CARAVAN_MOD,
    0,
    CARDOPL3,
    SBUS_MOD,
    RF_LONG,
    BUSDOPL,
    PTRUCKG_MOD,
    0,
    SEMIDOPL,
    MUSTANG_MOD,
    0,
    CARDOPL2,
    JEEP_MOD,
    0,
    CARDOPL1,
    GTRUCK_MOD,
    0,
    DIESEL_DOPPLER,
    // 	.word	CBUS_MOD,RF_LONG,BUSDOPL
    COPCAR_MOD,
    0,
    CARDOPL2,
    MUSCLE_MOD,
    RF_WEAVER,
    CARDOPL3,
    CARAVAN_MOD,
    0,
    CARDOPL3,
    PTRUCKG_MOD,
    0,
    SEMIDOPL,
    MUSTANG_MOD,
    0,
    CARDOPL2,
    // 	.word	SBUS_MOD,RF_LONG,BUSDOPL
    JEEP_MOD,
    0,
    CARDOPL1,
    // ----------------------------------------------------------------------------
    // RHO DRONE	ONCOMING TRAFFIC DRONE
    //
    // allocated standard drone
    //
    // starting position = end of the current universe (up ahead)
    //
    // in lane 0 or 1 accelerate past the player
    // when rho has reached the begining of the universe
    //
    //
    // RHO may be allocated as a suicide vehicle
    // but in general plays a straight game as an
    // oncomer.
    //
};
/* asm: RHOFLAG	.bss	RHOFLAG,1 */
int RHOFLAG;
/* asm: RHOPAL	.bss	RHOPAL,1 */
int RHOPAL;

void RHO_DRONE(void) {
    // asm 00009790: 	LDI	0,R5
    // asm 00009791: 	STI	R5,@RHOFLAG
    // asm 00009792: 	RANDN	RHO_TABLE_LENGTH	;INDEX #
IBOIBO:
    // asm 00009794: 	CLRI	R4
    // asm 00009795: 	LDI	R0,R5			;SAVE INDEX#
    // asm 00009796: 	LDI	R0,AR2
    // asm 00009797: 	STI	R0,*+AR7(RHO_INIT)
    // asm 00009798: 	MPYI	RT_SIZE,AR2
    // asm 00009799: 	ADDI	@RHO_TABLEI,AR2
    // asm 0000979A: 	LDI	@NOLONG_VEHICLES,R0
    // asm 0000979B: 	BZ	NONOLONG
    // asm 0000979C: 	LDI	*+AR2(RT_FLAG),R0
    // asm 0000979D: 	TSTB	RF_LONG,R0
    // asm 0000979E: 	BZ	NONOLONG
    // asm 0000979F: 	LDI	5,R0
    // asm 000097A0: 	BU	IBOIBO
NONOLONG:
    // asm 000097A1: 	LDI	*+AR2(RT_VEHIDX),R0
    // asm 000097A2: 	LDI	R0,AR2
    // asm 000097A3: 	STI	R0,*+AR7(DELTA_MODEL)
    // asm 000097A4: 	MPYI	VEHTAB_SIZE,AR2
    // asm 000097A5: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 000097A6: 	LDI	*+AR2(VEHTAB_MODEL),AR2
    // asm 000097A7: 	STI	R4,*+AR7(DELTA_INIT)
    // asm 000097A8: 	CALL	OBJ_GETE
    // asm 000097A9: 	BC	SUICIDE			;abort process if no object available
    // asm 000097AA: 	LDI	AR0,AR4
    // asm 000097AB: 	LDI	DRONE_C|VEHICLE_T|DRNE_RHO,R0
    // asm 000097AC: 	STI	R0,*+AR4(OID)
    // asm 000097AD: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm 000097AE: 	CALL	VEHICLE_ANI_INIT	;UTIL.ASM
    // asm 000097AF: 	CALL	DELTA_OINIT
    // asm 000097B0: 	LDF	MAX_ACCEL_INIT,R0
    // asm 000097B1: 	STF	R0,*+AR5(CARMAXACCEL)	;SET ACCEL POWER
    // asm 000097B2: 	LDI	*+AR4(OID),R0
    // asm 000097B3: 	STI	R0,*+AR5(CAR_ID)
    // asm 000097B4: 	STI	R0,*+AR7(PID)
    // asm 000097B5: 	CALL	SET_DRONE_PAL
    // asm 000097B6: 	RANDN	2
    // asm 000097B8: 	ADDI	2,R0
    // asm 000097B9: 	STI	R0,*+AR7(DELTA_STATUS)
    // 	;init position at end of universe
    // 	;
    // asm 000097BA: 	LDI	@DYNALIST_END,AR2
    // asm 000097BB: 	LDI	*+AR2(OBLINK4),AR2
    // asm 000097BC: 	LDI	*+AR2(OBLINK4),AR2
    // asm 000097BD: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm 000097BE: 	LDI	*+AR2(OUSR1),R0
    // asm 000097BF: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm 000097C0: 	CALL	SUB_FUNCTION_RVS
    // asm 000097C1: 	LDP	@_VECTORA
    // asm 000097C2: 	LDF	*+AR2(OPOSX),R0
    // asm 000097C3: 	ADDF	@_VECTORA+X,R0
    // asm 000097C4: 	STF	R0,*+AR4(OPOSX)
    // asm 000097C5: 	LDF	*+AR2(OPOSY),R0
    // asm 000097C6: 	SUBF	*+AR5(CARWHLTAB+1),R0
    // asm 000097C7: 	ADDF	@_VECTORA+Y,R0
    // asm 000097C8: 	STF	R0,*+AR4(OPOSY)
    // asm 000097C9: 	LDF	*+AR2(OPOSZ),R0
    // asm 000097CA: 	ADDF	@_VECTORA+Z,R0
    // asm 000097CB: 	STF	R0,*+AR4(OPOSZ)
    // asm 000097CC: 	SETDP
    // 	;initialize Ytheta to the intentional direction
    // asm 000097CD: 	STF	R2,*+AR4(ORADY)
    // asm 000097CE: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm 000097CF: 	STF	R2,*+AR5(CARYROT)
    // asm 000097D0: 	STF	R2,*+AR5(CARVROT)
    // asm 000097D1: 	LDI	AR4,AR2
    // asm 000097D2: 	ADDI	OMATRIX,AR2
    // asm 000097D3: 	CALL	FIND_YMATRIX
    // asm 000097D4: 	CLRI	R0
    // asm 000097D5: 	STI	R0,*+AR7(RHO_NOISE)
    // asm 000097D6: 	STI	R0,*+AR7(RHO_YELL)
    // asm 000097D7: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // 	;is this a weaver?  (drunk driver?)
    // 	;
    // 	;
    // 	;
    // asm 000097D8: 	LDI	*+AR7(RHO_INIT),AR2
    // asm 000097D9: 	MPYI	RT_SIZE,AR2
    // asm 000097DA: 	ADDI	@RHO_TABLEI,AR2
    // asm 000097DB: 	LDI	*+AR2(RT_FLAG),R0
    // asm 000097DC: 	TSTB	RF_WEAVER,R0
    // asm 000097DD: 	BZ	NOT_WEAVER
    // asm 000097DE: 	LDF	@GAME_TIMER,R0		;first minute - dont swerve
    // asm 000097DF: 	CMPF	1.1,R0
    // asm 000097E0: 	BLT	NOT_WEAVER
    // ;	RANDN	10	;1 in 10 chance
    // ;	CMPI	0,R0
    // ;	BNE	NOT_WEAVER
    // asm 000097E1: 	LDI	RHO_WEAVER,R0
    // asm 000097E2: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // asm 000097E3: 	FLOAT	576,R0
    // asm 000097E4: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm 000097E5: 	LDI	30,R0
    // asm 000097E6: 	STI	R0,*+AR7(DELTA_PSTAT)
    // asm 000097E7: 	LDF	0.1,R0
    // asm 000097E8: 	CALL	FRAND
    // asm 000097E9: 	ADDF	0.25,R0
    // asm 000097EA: 	STF	R0,*+AR7(RHO_THETA_DELTA)
    // asm 000097EB: 	FLOAT	520,R0
    // asm 000097EC: 	CALL	FRAND
    // asm 000097ED: 	FLOAT	2600,R1
    // asm 000097EE: 	ADDF	R1,R0
    // asm 000097EF: 	STF	R0,*+AR7(RHO_AMP)
    // asm 000097F0: 	FLOAT	576,R0
    // asm 000097F1: 	CALL	SFRAND
    // asm 000097F2: 	STF	R0,*+AR7(RHO_XHEAD)
    // asm 000097F3: 	CLRF	R6			;SIN
NOT_WEAVER:
    // asm 000097F4: 	LDI	@RHOFLAG,R0		;CREATED BY OTHER MACHINE?
    // asm 000097F5: 	BZ	RHOLL1			;NO...
    // asm 000097F6: 	STI	R0,*+AR5(CARNUM)	;YES, SAVE ID NUMBER
    // asm 000097F7: 	LDI	@RHOPAL,R0		;GET PALETTE
    // asm 000097F8: 	STI	R0,*+AR4(OPAL)
    // asm 000097F9: 	LDI	1,R0
    // asm 000097FA: 	STI	R0,*+AR5(CAR_OM)	;OTHER MACHINE IS IN CONTROL
    // asm 000097FB: 	B	OM_DRONE		;GO DRONE IT
RHOLL1:
    // asm 000097FC: 	CALL	DRONE_PTR_ADD
    // asm 000097FD: 	CALL	SEND_RHO_CREATE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_DRONE", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *
 *
 *----------------------------------------------------------------------------
 *
 *
 *
 */
static void RHO_LP(void) {
    // asm 000097FE: 	LDI	@SUSPEND_MODE,R0
    // asm 000097FF: 	CMPI	SM_HALT,R0
    // asm 00009800: 	BEQ	RHO_SLP
    // asm 00009801: 	CALL	AHEAD_OF_PLAYER_P
    // asm 00009802: 	LDIC	1,R0
    // asm 00009803: 	LDINC	0,R0
    // asm 00009804: 	STI	R0,*+AR7(DELTA_PSTAT)
    // ;	;CHECK TO SEE IF...
    // ;	;	WE ARE FAR ENOUGH BEHIND THE PLYR THAT
    // ;	;	WE CAN KILL OURSELVES
    // ;
    // ;	CMPI	0,R0
    // ;	BNE	NOTBEHIND_PLAYER
    // ;
    // ;	LDI	*+AR5(CARTRAK),AR0
    // ;	LDI	*+AR0(OUSR1),R0
    // ;	LDI	@PLYCBLK,AR1
    // ;	ADDI	4,R0
    // ;	CMPI	*+AR1(CARTRAK),AR1
    // ;	LDI	*+AR1(OUSR1),R0
    // ;	BLE	RHO_DIE
    // ;
    // ;NOTBEHIND_PLAYER
    // 	;
    // 	;DRUNK ONCOMER?
    // 	;
    // asm 00009805: 	LDI	*+AR7(DELTA_PLAYIT),R0
    // asm 00009806: 	CMPI	RHO_WEAVER,R0
    // asm 00009807: 	BNE	NOT_WEAVER_LP
    // ;
    // ;
    // ;
    // asm 00009808: 	LDI	@POSITION,R0
    // asm 00009809: 	CMPI	5,R0
    // asm 0000980A: 	BGT	ABORTWEAVE
    // ;
    // ;
    // asm 0000980B: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000980C: 	CALL	GET_LANES
    // asm 0000980D: 	CMPI	1,R0
    // asm 0000980E: 	BEQ	DONTABORT
    // 	;if at any point we come into a 2 lane
    // 	;situation, ABORT weaver and go into
    // 	;straight mode
ABORTWEAVE:
    // asm 0000980F: 	CLRI	R0
    // asm 00009810: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // asm 00009811: 	RANDN	2
    // asm 00009813: 	ADDI	2,R0
    // asm 00009814: 	STI	R0,*+AR7(DELTA_STATUS)
    // asm 00009815: 	BU	NOT_WEAVER_LP
DONTABORT:
    // asm 00009816: 	LDI	100,AR2
    // asm 00009817: 	CALL	RANDPER
    // asm 00009818: 	BNC	III
    // asm 00009819: 	LDF	0.1,R0
    // asm 0000981A: 	CALL	FRAND
    // asm 0000981B: 	ADDF	0.25,R0
    // asm 0000981C: 	STF	R0,*+AR7(RHO_THETA_DELTA)
    // asm 0000981D: 	FLOAT	520,R0
    // asm 0000981E: 	CALL	FRAND
    // asm 0000981F: 	FLOAT	2600,R1
    // asm 00009820: 	ADDF	R1,R0
    // asm 00009821: 	STF	R0,*+AR7(RHO_AMP)
III:
    // asm 00009822: 	LDI	200,AR2
    // asm 00009823: 	CALL	RANDPER
    // asm 00009824: 	BNC	III4
    // asm 00009825: 	FLOAT	576,R0
    // asm 00009826: 	CALL	SFRAND
    // asm 00009827: 	STF	R0,*+AR7(RHO_XHEAD)
III4:
    // asm 00009828: 	LDF	*+AR7(RHO_THETA),R2
    // asm 00009829: 	ADDF	*+AR7(RHO_THETA_DELTA),R2
    // asm 0000982A: 	CALL	NORMIT
    // asm 0000982B: 	STF	R2,*+AR7(RHO_THETA)
    // asm 0000982C: 	CALL	_SINE
    // asm 0000982D: 	MPYF	*+AR7(RHO_AMP),R0
    // asm 0000982E: 	ADDF	*+AR7(RHO_XHEAD),R0
    // asm 0000982F: 	STF	R0,*+AR7(DELTA_XLANE)
NOT_WEAVER_LP:
    // 	;check to see if collision has occurred
    // asm 00009830: 	LDI	*+AR5(CAR_BUMP),R0
    // asm 00009831: 	BNZ	RHO_ISHIT
    // 	;make sure to check for relative speed for
    // 	;2x sound calls
    // 	;(once sounds are received)
    // asm 00009832: 	LDI	*+AR7(RHO_NOISE),R0
    // asm 00009833: 	BNZ	NONOISE
    // asm 00009834: 	LDF	*+AR7(DELTA_PLYRDIST),R0
    // asm 00009835: 	STF	R0,*+AR7(DELTA_OPLYRDIST)
    // asm 00009836: 	CALL	DIST_TO_PLYR
    // asm 00009837: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm 00009838: 	LDI	*+AR7(DELTA_PSTAT),R3		;IN FRONT OF PLAYER?
    // asm 00009839: 	BZ	NONOISE
    // asm 0000983A: 	FLOAT	7500,R1
    // asm 0000983B: 	CMPF	R1,R0
    // asm 0000983C: 	BGT	NONOISE
    // asm 0000983D: 	LDI	1,R0
    // asm 0000983E: 	STI	R0,*+AR7(RHO_NOISE)
    // asm 0000983F: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm 00009840: 	MPYI	VEHTAB_SIZE,AR2
    // asm 00009841: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 00009842: 	LDI	*+AR2(VEHTAB_PASSBY),AR2
    // asm 00009843: 	CMPI	0,AR2
    // asm 00009844: 	BEQ	NONOISE
    // asm 00009845: 	CALL	ONESNDFX
NONOISE:
    // asm 00009846: 	CALL	CKCAROFF	;OFF THE UNIVERSE ???
    // asm 00009847: 	BZ	RHO_DIE		;YES
    // 	;
    // 	;simply drive towards the start of the universe by walking
    // 	;through the universe backwards
    // 	;
    // 	;see if we should track the next piece
CHECK_DIST:
    // asm 00009848: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
#if DEBUG
    // asm: 	BLT	$	;SEARCHING INVALID (/DELETED?) OBJECT
#endif
    // asm 00009849: 	RS	8,R0
    // asm 0000984A: 	LDI	@SECTIONIDX,R1
    // asm 0000984B: 	SUBI	@DGROUP_COUNT,R1
    // asm 0000984C: 	CMPI	R1,R0
    // asm 0000984D: 	BGT	ALLOK66
    // asm 0000984E: 	BU	$	;probably RHO_DIE
ALLOK66:
    // asm 0000984F: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00009850: 	LDI	*+AR2(OBLINK4),R0
    // asm 00009851: 	BZ	RHO_DIE				;should we kill ourselves
    // asm 00009852: 	LDI	*+AR7(DELTA_PLAYIT),R0
    // asm 00009853: 	CMPI	RHO_WEAVER,R0
    // asm 00009854: 	BNE	NOT_WEAVER_LP2
    // asm 00009855: 	CALL	GET_TRACK_POS_RVS_XLANE
    // asm 00009856: 	BU	L9999
NOT_WEAVER_LP2:
    // asm 00009857: 	CALL	GET_TRACK_POS_RVS		;CHECK IF WE SHOULD ADVANCE
L9999:
    // asm 00009858: FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm 00009859: 	CMPF	R1,R0
    // asm 0000985A: 	BGT	THIS_PIECE
    // asm 0000985B: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000985C: 	LDI	*+AR2(OBLINK4),R0
#if DEBUG
    // asm: 	BZ	$				;HOW DID WE MISS THIS?
#endif
    // asm 0000985D: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm 0000985E: 	LDI	R0,AR0
    // asm 0000985F: 	LDI	*+AR0(OUSR1),R0
#if DEBUG
    // asm: 	BLT	$
#endif
    // asm 00009860: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm 00009861: 	BU	CHECK_DIST
THIS_PIECE:
    // asm 00009862: 	LDF	*+AR5(CARSPEED),R1
    // asm 00009863: 	LDFLE	30,R1			;if 0 or less assume 30 mph
    // asm 00009864: 	FLOATP	@NFRAMES,R2
    // asm 00009865: 	MPYF	R2,R1
    // asm 00009866: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm 00009867: 	FIX	R0,R7
    // asm 00009868: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00009869: 	LDP	@_VECTORA		;lane position
    // asm 0000986A: 	LDF	*+AR2(OPOSX),R2		;X
    // asm 0000986B: 	SUBF	*+AR4(OPOSX),R2
    // asm 0000986C: 	ADDF	@_VECTORA+X,R2
    // asm 0000986D: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm 0000986E: 	SUBF	*+AR4(OPOSZ),R3
    // asm 0000986F: 	ADDF	@_VECTORA+Z,R3
    // asm 00009870: 	SETDP
    // asm 00009871: JOINUP998
    // 	;find the theta delta to this position
    // 	;
    // asm 00009871: 	CALL	ARCTANF			;-> R0
    // asm 00009872: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm 00009873:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm 00009874: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm 00009875: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm 00009876: 	SUBF	1,R1	;DBG
    // asm 00009877: 	BZ	NODIV
    // asm 00009878: 	CALL	DIV_F			;-> R0
NODIV:
    // asm 00009879: STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm 0000987A: 	LDI	*+AR7(RHO_YELL),R0
    // asm 0000987B: 	BNZ	NOTPRECOL
    // asm 0000987C: 	CALL	PRECOLLIDE_PLYR
    // asm 0000987D: 	BNC	NOTPRECOL
    // asm 0000987E: 	CALL	PLYR_RIDE_RIGHT
    // asm 0000987F: 	BC	NOTPRECOL
    // asm 00009880: 	LDI	*+AR7(DELTA_PSTAT),R0		;IN FRONT OF PLAYER?
    // asm 00009881: 	BZ	NOTPRECOL
    // asm 00009882: 	LDI	1,R0
    // asm 00009883: 	STI	R0,*+AR7(RHO_YELL)
    // asm 00009884: 	RANDN	10
    // asm 00009886: 	CMPI	7,R0
    // asm 00009887: 	BGT	IIIL
    // asm 00009888: 	LDI	*+AR7(RHO_INIT),AR2
    // asm 00009889: 	MPYI	RT_SIZE,AR2
    // asm 0000988A: 	ADDI	@RHO_TABLEI,AR2
    // asm 0000988B: 	LDI	*+AR2(RT_ONSCREAM),AR2
    // asm 0000988C: 	CMPI	0,AR2
    // asm 0000988D: 	CALL	ONESNDFX
    // asm 0000988E: 	LDIC	1,R0
    // asm 0000988F: 	LDINC	0,R0
    // asm 00009890: 	STI	R0,*+AR7(RHO_YELL)
    // asm 00009891: 	BU	NONOISE
    // asm 00009892: 	LDI	*+AR7(RHO_INIT),R0
    // asm 00009893: 	MPYI	RT_SIZE,R0
    // asm 00009894: 	ADDI	@RHO_TABLEI,R0
    // asm 00009895: 	LDI	R0,AR2
    // asm 00009896: 	LDI	*+AR2(RT_ONSCREAM),R0
    // asm 00009897: 	BZ	NOLOAD
    // asm 00009898: 	LDI	R0,AR2
    // asm 00009899: 	BU	LLL88
IIIL:
NOLOAD:
    // asm 0000989A: 	RANDN	B4HIT_SIZE
    // asm 0000989C: 	LDI	R0,AR2
    // asm 0000989D: 	ADDI	@B4HIT_TABLEI,AR2
    // asm 0000989E: 	LDI	*AR2,AR2
LLL88:
    // asm 0000989F: CALL	ONESNDFX
    // asm 000098A0: 	LDIC	1,R0
    // asm 000098A1: 	LDINC	0,R0
    // asm 000098A2: 	STI	R0,*+AR7(RHO_YELL)
    // asm 000098A3: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm 000098A4: 	MPYF	0.01,R2
    // asm 000098A5: 	STF	R2,*+AR5(CARTHROTTLE)
    // asm 000098A6: 	BU	L99
NOTPRECOL:
    // 	;set throttle
    // asm 000098A7: 	CLRF	R2
    // asm 000098A8: 	STF	R2,*+AR5(CARBRAKE)
    // asm 000098A9: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm 000098AA: 	MPYF	1.01,R2
    // asm 000098AB: 	CMPF	MIN_THROTTLE,R2
    // asm 000098AC: 	LDFLT	MIN_THROTTLE,R2
    // asm 000098AD: 	CMPF	MAX_THROTTLE,R2
    // asm 000098AE: 	LDFGT	MAX_THROTTLE,R2
    // asm 000098AF: 	STF	R2,*+AR7(DELTA_THROTTLE)
    // asm 000098B0: 	STF	R2,*+AR5(CARTHROTTLE)
L99:
    // asm 000098B1: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm 000098B2: 	MPYF	2.50,R2			;depending on plyr.asm this may have to
    // asm 000098B3: 	CALL	DRONEGO
    // asm 000098B4: 	CALL	GETTRAK
RHO_SLP:
    // asm 000098B5: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 000098B6: 	CALLNZ	SEND_RHO_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm 000098B7: 	CALL	CKCAROFF	;OFF THE UNIVERSE ???
    // asm 000098B8: 	BZ	RHO_DIE		;YES
    // asm 000098B9: 	SLEEP	1
    // asm 000098BB: 	BU	RHO_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_LP", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *
 *CHECK IF OFF THE UNIVERSE
 *RET EQ (R0=0) IF OFF UNIVERSE, NE (R0=1) IF IN BOUNDS
 *AR7=DRONE PROCESS
 *
 */
static void CKCAROFF(void) {
    // asm 000098BC: 	LDI	*+AR7(DELTA_TPIECE),R0
    // asm 000098BD: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm 000098BE: 	CMPI	AR0,R0
    // asm 000098BF: 	BEQ	CKCXFAIL
    // asm 000098C0: 	LDI	*+AR0(OUSR1),R0
    // asm 000098C1: 	LDI	*+AR7(DELTA_LAST_OID),R1
    // asm 000098C2: 	CMPI	R0,R1
    // asm 000098C3: 	BLE	CKCXFAIL
    // asm 000098C4: 	LDI	@DYNALIST_END,AR0		;GET FURTHEST ROAD ID
    // asm 000098C5: 	LDI	*+AR0(OUSR1),R0
    // asm 000098C6: 	CMPI	R0,R1
    // asm 000098C7: 	BLT	CKCXPASS
CKCXFAIL:
    // asm 000098C8: 	LDI	0,R0
    // asm 000098C9: 	RETS
CKCXPASS:
    // asm 000098CA: 	LDI	1,R0
    // asm 000098CB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKCAROFF", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *FREE UP THE ALLOCATED STRUCTURES, DELINK THE LINKS, AND
 *KILL THE PROCESS
 *
 */

void RHO_DIE(void) {
    // asm 000098CC: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 000098CD: 	BZ	RHO_DIE1 		;NO, BLOW OUT...
    // asm 000098CE: 	CALL	COMPTRAK
    // asm 000098CF: 	BLE	RHO_DIE0
    // asm 000098D0: 	CALL	SEND_RHO_XSFER
    // asm 000098D1: 	B	OM_DRONE		;CONTROL SWAPS TO OTHER MACHINE
RHO_DIE0:
    // asm 000098D2: 	LDI	*+AR5(CARNUM),R0	;GET ID
    // asm 000098D3: 	CALL	SEND_RHO_KILL		;KILL OFF THE UTHA MACHINES MUTHA
RHO_DIE1:
    // asm 000098D4: 	CALL	FREE_DRONE
    // asm 000098D5: 	LDI	AR5,AR2
    // asm 000098D6: 	CALL	DELCAR
    // asm 000098D7: 	LDI	1,R0
    // asm 000098D8: 	LS	O_PROC_B,R0
    // asm 000098D9: 	LDI	*+AR4(OFLAGS),R1
    // asm 000098DA: 	ANDN	R0,R1
    // asm 000098DB: 	STI	R1,*+AR4(OFLAGS)
    // asm 000098DC: 	LDI	*+AR4(OFLAGS),R0
    // asm 000098DD: 	TSTB	O_DYNAMIC,R0
    // asm 000098DE: 	BZ	NODYNALEAN
    // asm 000098DF: 	LDI	*+AR4(ORADZ),AR2
    // asm 000098E0: 	CALL	PRC_KILL
NODYNALEAN:
    // asm 000098E1: 	LDI	AR4,AR2
    // asm 000098E2: 	CALL	OBJ_DELETE
    // asm 000098E3: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_DIE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*THE RHO HAS BEEN HIT NOW CHOOSE
*
*	uneffected (if low intensity)?
*	smoke out death?
*	flame out?
*	flame burst?
*	total explosion?
*	geometric expansion?
*	star trek fadeout?
*	defender pixelate?
*	lawnmower-man ping pong?
*	toaster smear?
*	other ridiculas event?
*
*
;COLTABI	.word	COLTAB
;COLTAB	.word	SCOLLA,SCOLLB,SCOLLC
*/
static void RHO_ISHIT(void) {
    // asm 000098E4: 	LDI	*+AR4(OID),R0
    // asm 000098E5: 	ANDN	TYPE_M,R0
    // asm 000098E6: 	OR	DEAD_VEH_T,R0
    // asm 000098E7: 	STI	R0,*+AR4(OID)
    // asm 000098E8: 	STI	R0,*+AR5(CAR_ID)
    // asm 000098E9: 	STI	R0,*+AR7(PID)
    // ;	INCM	@CAR_COLLS
    // asm 000098EA: 	CREATEC	SMOKE_PUFF,SPAWNER_C
    // asm 000098ED: 	CREATEC	EXP_PUFF,SPAWNER_C
    // asm 000098F0: 	LDI	10,AR6
RHO_ISHITLP:
    // asm 000098F1: 	LDI	@SUSPEND_MODE,R0
    // asm 000098F2: 	CMPI	SM_HALT,R0
    // asm 000098F3: 	BEQ	RHOISHIT_SLP
    // asm 000098F4: 	DEC	AR6
    // asm 000098F5: 	CMPI	0,AR6
    // asm 000098F6: 	BLT	NOSMK
    // asm 000098F7: 	CREATEC	SMOKE_PUFF,SPAWNER_C
NOSMK:
    // 	;dont disolve
    // 	;
    // 	;
    // asm 000098FA: 	LDI	*+AR5(CARTRAK),R0
    // asm 000098FB: 	STI	R0,*+AR7(DELTA_TPIECE)
    // 	;find piece which points to piece if it is not found
    // 	;or it is the initial piece exit rho code and commit
    // 	;suicide
    // asm 000098FC: 	LDI	*+AR5(CARTRAK),AR1
    // asm 000098FD: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm 000098FE: 	CMPI	AR0,AR1
    // asm 000098FF: 	BEQ	RHO_DIE
    // asm 00009900: 	LDI	*+AR0(OUSR1),R0
    // asm 00009901: 	LDI	*+AR1(OUSR1),R1
    // asm 00009902: 	CMPI	R0,R1
    // asm 00009903: 	BLT	RHO_DIE
    // 	;for now we die out
    // asm 00009904: 	CLRF	R2
    // asm 00009905: 	STF	R2,*+AR7(DELTA_THROTTLE)
    // asm 00009906: 	STF	R2,*+AR5(CARTHROTTLE)
    // ;	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm 00009907: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm 00009908: 	CALL	DRONEGO
    // asm 00009909: 	CALL	GETTRAK
RHOISHIT_SLP:
    // asm 0000990A: 	SLEEP	1
    // asm 0000990C: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 0000990D: 	CALLNZ	SEND_RHO_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm 0000990E: 	BU	RHO_ISHITLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_ISHIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *
 *TRANSFER CAR TO OTHER MACHINE
 *AR4=CAR
 *AR5=CAR BLOCK
 *AR7=PROCESS
 *
 */

static void SEND_RHO_XSFER(void) {
    // asm 0000990F: 	LDI	CB_RHO_XSFER,R0 	;MESSAGE HEADER
    // *SEND MESSAGE HEADER
    // asm 00009910: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00009911: 	STI	R0,*AR2
    // *SEND CAR ID #
    // asm 00009912: 	LDI	*+AR5(CARNUM),R0
    // asm 00009913: 	STI	R0,*+AR2(1)
    // *SEND CAR STATE, 1=HIT
    // asm 00009914: 	LDI	*+AR4(OID),R0
    // asm 00009915: 	AND	DEAD_VEH_T,R0
    // asm 00009916: 	STI	R0,*+AR2(2)
    // asm 00009917: 	LDI	3-1,RC
    // asm 00009918: 	CALL	MESSAGE_ADD
    // asm 00009919: 	POP	R0			;CLEAN OFF STACK
    // asm 0000991A: 	LDI	1,R0
    // asm 0000991B: 	STI	R0,*+AR5(CAR_OM)
    // asm 0000991C: 	B 	OM_DRONE  		;NOW CONTROLLED BY OTHER MACHINE
    // *
    // *GET A CAR FROM OTHER MACHINE
    // *AR4=CAR
    // *AR5=CAR BLOCK
    // *AR7=PROCESS
    // *
    // *NOTE SHOULD ADD IN FUTURE: CAR_SPIN,CARSPRAD,CARGEAR,CARRPM,CARSKID
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RHO_XSFER", 0, 0);
    UNIMPL();
}

void DECODE_RHO_XSFER(void) {
    // asm 0000991D: 	LDI	@IGNORE_UPDATES,R0	;RACE OVER?
    // asm 0000991E: 	BNZ	DECRHOX2		;YES, NO UPDATES ALLOWED...
    // asm 0000991F: 	CALL	FIND_DRONE  		;FIND CAR ID #
    // asm 00009920: 	BNZ	DECRHOX			;CAR NOT FOUND
    // *INITIALIZE SHIT
    // asm 00009921: 	LDI	AR0,AR4			;SAVE CAR INDEX
    // asm 00009922: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR BLOCK
    // asm 00009923: 	LDI	*+AR4(OPLINK),AR7	;GET PROCESS
    // asm 00009924: 	LDI	@RHO_ACTIVE_XSFERI,R2
    // asm 00009925: 	STI	R2,*+AR7(PWAKE)		;CHANGE WAKE-UP ADDR
    // asm 00009926: 	LDI	*AR2++,R0		;GET CAR STATE 1=HIT, 0=ACTIVE
    // asm 00009927: 	STI	R0,*+AR7(PR4)		;SAVE CODE IN REGISTER
    // asm 00009928: 	RETS
DECRHOX2:
    // asm 00009929: 	ADDI	2,AR2			;UPDATES NOT ALLOWED
    // asm 0000992A: 	RETS
DECRHOX:
    // asm 0000992B: 	ADDI	1,AR2			;COULDN'T FIND CAR ERROR
    // asm 0000992C: 	RETS
    // *
    // *TRANSFER ACTIVE RHO
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RHO_XSFER", 0, 0);
    UNIMPL();
}

static void RHO_ACTIVE_XSFER(void) {
    // asm 0000992E: 	LDI	0,R0
    // asm 0000992F: 	STI	R0,*+AR5(CAR_OM)		;CAR BELONGS TO ME NOW!!!
    // asm 00009930: 	LDI	*+AR5(CARTRACK_ID),R2		;GET TRACK ID
    // asm 00009931: 	STI	R2,*+AR7(DELTA_LAST_OID)	;SAVE IT HERE ALSO
    // asm 00009932: 	LDI	@DYNALIST_END,AR0		;GET FURTHEST ROAD ID
    // asm 00009933: 	LDI	*+AR0(OUSR1),R0
    // asm 00009934: 	CMPI	R0,R2
    // asm 00009935: 	BGT	RHO_DIE0			;TOO FAR OUT, DIE
    // asm 00009936: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm 00009937: 	LDI	*+AR0(OUSR1),R0
    // asm 00009938: 	CMPI	R0,R2
    // asm 00009939: 	BLT	RHO_DIE0			;BEHIND US KILL HIM
    // asm 0000993A: 	CALL	FIND_DYNA			;GET TRACKING PIECE POINTER
    // asm 0000993B: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm 0000993C: 	LDF	0,R0
    // asm 0000993D: 	STF	R0,*+AR5(CARDROT)		;CLEAN UP REENTRY
    // asm 0000993E: 	STF	R0,*+AR5(CARSPRAD)
    // asm 0000993F: 	STF	R0,*+AR5(CARSKID)
    // asm 00009940: 	LDI	0,R0
    // asm 00009941: 	STI	R0,*+AR5(CAR_SPIN)
    // asm 00009942: 	LDI	R4,R4	 			;CHECK HIT RHO
    // asm 00009943: 	BZ	RHO_LP
    // asm 00009944: 	LDI	0,AR6				;WERE HIT, GO TO DEAD LOOP
    // asm 00009945: 	LDI	*+AR4(OID),R0			;MAKE 'EM DEAD
    // asm 00009946: 	ANDN	TYPE_M,R0
    // asm 00009947: 	OR	DEAD_VEH_T,R0
    // asm 00009948: 	STI	R0,*+AR4(OID)
    // asm 00009949: 	STI	R0,*+AR5(CAR_ID)
    // asm 0000994A: 	STI	R0,*+AR7(PID)
    // asm 0000994B: 	CALL	GETTRAK
    // asm 0000994C: 	LDF	0,R0
    // asm 0000994D: 	STF	R0,*+AR7(DELTA_RADYDELTA)	;CLEAR THIS BULLSHIT
    // asm 0000994E: 	B     	RHO_ISHITLP
    // *
    // *KILL OFF RHO MESSAGE
    // *R0=ID
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_ACTIVE_XSFER", 0, 0);
    UNIMPL();
}

static void SEND_RHO_KILL(void) {
    // asm 0000994F: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00009950: 	LDI	CB_RHO_KILL,R1
    // asm 00009951: 	STI	R1,*AR2
    // asm 00009952: 	STI	R0,*+AR2(1)
    // asm 00009953: 	LDI	2-1,RC
    // asm 00009954: 	CALL	MESSAGE_ADD
    // asm 00009955: 	RETS
    // *
    // *KILL OFF RHO
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RHO_KILL", 0, 0);
    UNIMPL();
}

void DECODE_RHO_KILL(void) {
    // asm 00009956: 	LDI	@IGNORE_UPDATES,R0
    // asm 00009957: 	BNZ	DRKXX
    // asm 00009958: 	CALL	FIND_DRONE  		;GET DRONE OBJ IN AR0
    // asm 00009959: 	BNZ	DRKX
    // asm 0000995A: 	LDI	*+AR0(OPLINK),AR7
    // asm 0000995B: 	LDI	@RHO_DIE1I,R2		;KILL THE SOMBITCH
    // asm 0000995C: 	STI	R2,*+AR7(PWAKE)		;CHANGE WAKE-UP ADDR
DRKX:
    // asm 0000995D: 	RETS
DRKXX:
    // asm 0000995E: 	INC	AR2
    // asm 0000995F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RHO_KILL", 0, 0);
    UNIMPL();
}

#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "obj.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "delta.h"
#include "comm.h"
#include "rho.h"

/*
 * Source module: asm/RHO.ASM
 */

void RHO_START(void);
void RHO_DRONE(void);
void NONOLONG(void);
void RHO_LP(void);
void DONTABORT(void);
void ALLOK66(void);
void NOT_WEAVER_LP2(void);
void THIS_PIECE(void);
void IIIL(void);
void NOTPRECOL(void);
void CKCAROFF(void);
void CKCXPASS(void);
void RHO_DIE(void);
void RHO_DIE0(void);
void RHO_ISHIT(void);
void SEND_RHO_XSFER(void);
void DECODE_RHO_XSFER(void);
void DECRHOX2(void);
void DECRHOX(void);
void RHO_ACTIVE_XSFER(void);
void SEND_RHO_KILL(void);
void DECODE_RHO_KILL(void);
void DRKXX(void);

#define RHO_WEAVER 1 //DRUNK ONCOMER
/* *
*YELL B4 HIT TO PLAYER
 */
#define B4HIT_SIZE 6
/* asm: B4HIT_TABLE	.word	RH_GOFORIT,RH_TAKEHIM,RH_BABESCREAM1 */
/* asm: 	.word	RH_BABEWHOA,RH_HURRYUP,RH_GONNACRASH */
int B4HIT_TABLE[] = {
    RH_GOFORIT, RH_TAKEHIM, RH_BABESCREAM1,
    RH_BABEWHOA, RH_HURRYUP, RH_GONNACRASH,
};
/* *RHO FLAGS (RF_)
*
*
 */
#define RF_WEAVER 0x001 //may this become a weaver (SUICIDE)
#define RF_LONG 0x002 //Eugenes system can't handle long vehicles under circumstances
#define RT_VEHIDX 0
#define RT_FLAG 1
#define RT_ONSCREAM 2
#define RT_SIZE 3
/* asm: NOLONG_VEHICLES	.bss	NOLONG_VEHICLES,1 */
int NOLONG_VEHICLES;
/* *
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
/* asm: 	.word	CARAVAN_MOD,0,CARDOPL3 */
/* asm: 	.word	SBUS_MOD,RF_LONG,BUSDOPL */
/* asm: 	.word	PTRUCKG_MOD,0,SEMIDOPL */
/* asm: 	.word	MUSTANG_MOD,0,CARDOPL2 */
/* asm: 	.word	JEEP_MOD,0,CARDOPL1 */
/* asm: 	.word	GTRUCK_MOD,0,DIESEL_DOPPLER */
/* asm: 	.word	COPCAR_MOD,0,CARDOPL2 */
/* asm: 	.word	MUSCLE_MOD,RF_WEAVER,CARDOPL3 */
/* asm: 	.word	CARAVAN_MOD,0,CARDOPL3 */
/* asm: 	.word	PTRUCKG_MOD,0,SEMIDOPL */
/* asm: 	.word	MUSTANG_MOD,0,CARDOPL2 */
/* asm: 	.word	JEEP_MOD,0,CARDOPL1 */
int RHO_TABLE[] = {
    GTRUCK_MOD, 0, DIESEL_DOPPLER,
    FTRUCK_MOD, RF_LONG, TRUCKHORN_BLAST,
    CBUS_MOD, RF_LONG, BUSDOPL,
    COPCAR_MOD, 0, CARDOPL2,
    MUSCLE_MOD, RF_WEAVER, CARDOPL3,
    CARAVAN_MOD, 0, CARDOPL3,
    SBUS_MOD, RF_LONG, BUSDOPL,
    PTRUCKG_MOD, 0, SEMIDOPL,
    MUSTANG_MOD, 0, CARDOPL2,
    JEEP_MOD, 0, CARDOPL1,
    GTRUCK_MOD, 0, DIESEL_DOPPLER,
    COPCAR_MOD, 0, CARDOPL2,
    MUSCLE_MOD, RF_WEAVER, CARDOPL3,
    CARAVAN_MOD, 0, CARDOPL3,
    PTRUCKG_MOD, 0, SEMIDOPL,
    MUSTANG_MOD, 0, CARDOPL2,
    JEEP_MOD, 0, CARDOPL1,
};
/* asm: RHOFLAG	.bss	RHOFLAG,1 */
int RHOFLAG;
/* asm: RHOPAL	.bss	RHOPAL,1 */
int RHOPAL;

/* *
*RHO STARTER
*R4= VEHICLE INDEX
*R5= VEHICLE ID
*
*ENTRY POINT FOR RHO START BY OTHER GAME
 */
void RHO_START(void)
{
    // asm: 	STI	AR6,@RHOPAL
    // asm: 	STI	R4,@RHOFLAG		;SAVE CAR ID #
    // asm: 	LDI	R5,R0			;INDEX #
    // asm: 	B	IBOIBO
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_START", 0, 0);
    UNIMPL();
}

void RHO_DRONE(void)
{
    // asm: 	LDI	0,R5
    // asm: 	STI	R5,@RHOFLAG
    // asm: 	RANDN	RHO_TABLE_LENGTH	;INDEX #
IBOIBO:
    // asm: 	CLRI	R4
    // asm: 	LDI	R0,R5			;SAVE INDEX#
    // asm: 	LDI	R0,AR2
    // asm: 	STI	R0,*+AR7(RHO_INIT)
    // asm: 	MPYI	RT_SIZE,AR2
    // asm: 	ADDI	@RHO_TABLEI,AR2
    // asm: 	LDI	@NOLONG_VEHICLES,R0
    // asm: 	BZ	NONOLONG
    // asm: 	LDI	*+AR2(RT_FLAG),R0
    // asm: 	TSTB	RF_LONG,R0
    // asm: 	BZ	NONOLONG
    // asm: 	LDI	5,R0
    // asm: 	BU	IBOIBO
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_DRONE", 0, 0);
    UNIMPL();
}

void NONOLONG(void)
{
    // asm: 	LDI	*+AR2(RT_VEHIDX),R0
    // asm: 	LDI	R0,AR2
    // asm: 	STI	R0,*+AR7(DELTA_MODEL)
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDI	*+AR2(VEHTAB_MODEL),AR2
    // asm: 	STI	R4,*+AR7(DELTA_INIT)
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	SUICIDE			;abort process if no object available
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	DRONE_C|VEHICLE_T|DRNE_RHO,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm: 	CALL	VEHICLE_ANI_INIT	;UTIL.ASM
    // asm: 	CALL	DELTA_OINIT
    // asm: 	LDF	MAX_ACCEL_INIT,R0
    // asm: 	STF	R0,*+AR5(CARMAXACCEL)	;SET ACCEL POWER
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	STI	R0,*+AR5(CAR_ID)
    // asm: 	STI	R0,*+AR7(PID)
    // asm: 	CALL	SET_DRONE_PAL
    // asm: 	RANDN	2
    // asm: 	ADDI	2,R0
    // asm: 	STI	R0,*+AR7(DELTA_STATUS)
    // 	;init position at end of universe
    // 	;
    // asm: 	LDI	@DYNALIST_END,AR2
    // asm: 	LDI	*+AR2(OBLINK4),AR2
    // asm: 	LDI	*+AR2(OBLINK4),AR2
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm: 	CALL	SUB_FUNCTION_RVS
    // asm: 	LDP	@_VECTORA
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	ADDF	@_VECTORA+X,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR2(OPOSY),R0
    // asm: 	SUBF	*+AR5(CARWHLTAB+1),R0
    // asm: 	ADDF	@_VECTORA+Y,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	ADDF	@_VECTORA+Z,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	SETDP
    // 	;initialize Ytheta to the intentional direction
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm: 	STF	R2,*+AR5(CARYROT)
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(RHO_NOISE)
    // asm: 	STI	R0,*+AR7(RHO_YELL)
    // asm: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // 	;is this a weaver?  (drunk driver?)
    // 	;
    // 	;
    // 	;
    // asm: 	LDI	*+AR7(RHO_INIT),AR2
    // asm: 	MPYI	RT_SIZE,AR2
    // asm: 	ADDI	@RHO_TABLEI,AR2
    // asm: 	LDI	*+AR2(RT_FLAG),R0
    // asm: 	TSTB	RF_WEAVER,R0
    // asm: 	BZ	NOT_WEAVER
    // asm: 	LDF	@GAME_TIMER,R0		;first minute - dont swerve
    // asm: 	CMPF	1.1,R0
    // asm: 	BLT	NOT_WEAVER
    // ;	RANDN	10	;1 in 10 chance
    // ;	CMPI	0,R0
    // ;	BNE	NOT_WEAVER
    // asm: 	LDI	RHO_WEAVER,R0
    // asm: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // asm: 	FLOAT	576,R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm: 	LDI	30,R0
    // asm: 	STI	R0,*+AR7(DELTA_PSTAT)
    // asm: 	LDF	0.1,R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	0.25,R0
    // asm: 	STF	R0,*+AR7(RHO_THETA_DELTA)
    // asm: 	FLOAT	520,R0
    // asm: 	CALL	FRAND
    // asm: 	FLOAT	2600,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR7(RHO_AMP)
    // asm: 	FLOAT	576,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(RHO_XHEAD)
    // asm: 	CLRF	R6			;SIN
NOT_WEAVER:
    // asm: 	LDI	@RHOFLAG,R0		;CREATED BY OTHER MACHINE?
    // asm: 	BZ	RHOLL1			;NO...
    // asm: 	STI	R0,*+AR5(CARNUM)	;YES, SAVE ID NUMBER
    // asm: 	LDI	@RHOPAL,R0		;GET PALETTE
    // asm: 	STI	R0,*+AR4(OPAL)
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR5(CAR_OM)	;OTHER MACHINE IS IN CONTROL
    // asm: 	B	OM_DRONE		;GO DRONE IT
RHOLL1:
    // asm: 	CALL	DRONE_PTR_ADD
    // asm: 	CALL	SEND_RHO_CREATE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NONOLONG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*----------------------------------------------------------------------------
*
*
*
 */
void RHO_LP(void)
{
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BEQ	RHO_SLP
    // asm: 	CALL	AHEAD_OF_PLAYER_P
    // asm: 	LDIC	1,R0
    // asm: 	LDINC	0,R0
    // asm: 	STI	R0,*+AR7(DELTA_PSTAT)
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
    // asm: 	LDI	*+AR7(DELTA_PLAYIT),R0
    // asm: 	CMPI	RHO_WEAVER,R0
    // asm: 	BNE	NOT_WEAVER_LP
    // ;
    // ;
    // ;
    // asm: 	LDI	@POSITION,R0
    // asm: 	CMPI	5,R0
    // asm: 	BGT	ABORTWEAVE
    // ;
    // ;
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	CALL	GET_LANES
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	DONTABORT
    // 	;if at any point we come into a 2 lane
    // 	;situation, ABORT weaver and go into
    // 	;straight mode
ABORTWEAVE:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // asm: 	RANDN	2
    // asm: 	ADDI	2,R0
    // asm: 	STI	R0,*+AR7(DELTA_STATUS)
    // asm: 	BU	NOT_WEAVER_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_LP", 0, 0);
    UNIMPL();
}

void DONTABORT(void)
{
    // asm: 	LDI	100,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BNC	III
    // asm: 	LDF	0.1,R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	0.25,R0
    // asm: 	STF	R0,*+AR7(RHO_THETA_DELTA)
    // asm: 	FLOAT	520,R0
    // asm: 	CALL	FRAND
    // asm: 	FLOAT	2600,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR7(RHO_AMP)
III:
    // asm: 	LDI	200,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BNC	III4
    // asm: 	FLOAT	576,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(RHO_XHEAD)
III4:
    // asm: 	LDF	*+AR7(RHO_THETA),R2
    // asm: 	ADDF	*+AR7(RHO_THETA_DELTA),R2
    // asm: 	CALL	NORMIT
    // asm: 	STF	R2,*+AR7(RHO_THETA)
    // asm: 	CALL	_SINE
    // asm: 	MPYF	*+AR7(RHO_AMP),R0
    // asm: 	ADDF	*+AR7(RHO_XHEAD),R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
NOT_WEAVER_LP:
    // 	;check to see if collision has occurred
    // asm: 	LDI	*+AR5(CAR_BUMP),R0
    // asm: 	BNZ	RHO_ISHIT
    // 	;make sure to check for relative speed for
    // 	;2x sound calls
    // 	;(once sounds are received)
    // asm: 	LDI	*+AR7(RHO_NOISE),R0
    // asm: 	BNZ	NONOISE
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R0
    // asm: 	STF	R0,*+AR7(DELTA_OPLYRDIST)
    // asm: 	CALL	DIST_TO_PLYR
    // asm: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm: 	LDI	*+AR7(DELTA_PSTAT),R3		;IN FRONT OF PLAYER?
    // asm: 	BZ	NONOISE
    // asm: 	FLOAT	7500,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	NONOISE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(RHO_NOISE)
    // asm: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDI	*+AR2(VEHTAB_PASSBY),AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	NONOISE
    // asm: 	CALL	ONESNDFX
NONOISE:
    // asm: 	CALL	CKCAROFF	;OFF THE UNIVERSE ???
    // asm: 	BZ	RHO_DIE		;YES
    // 	;
    // 	;simply drive towards the start of the universe by walking
    // 	;through the universe backwards
    // 	;
    // 	;see if we should track the next piece
CHECK_DIST:
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
#if DEBUG
    // asm: 	BLT	$	;SEARCHING INVALID (/DELETED?) OBJECT
#endif
    // asm: 	RS	8,R0
    // asm: 	LDI	@SECTIONIDX,R1
    // asm: 	SUBI	@DGROUP_COUNT,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BGT	ALLOK66
    // asm: 	BU	$	;probably RHO_DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DONTABORT", 0, 0);
    UNIMPL();
}

void ALLOK66(void)
{
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OBLINK4),R0
    // asm: 	BZ	RHO_DIE				;should we kill ourselves
    // asm: 	LDI	*+AR7(DELTA_PLAYIT),R0
    // asm: 	CMPI	RHO_WEAVER,R0
    // asm: 	BNE	NOT_WEAVER_LP2
    // asm: 	CALL	GET_TRACK_POS_RVS_XLANE
    // asm: 	BU	L9999
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ALLOK66", 0, 0);
    UNIMPL();
}

void NOT_WEAVER_LP2(void)
{
    // asm: 	CALL	GET_TRACK_POS_RVS		;CHECK IF WE SHOULD ADVANCE
L9999:
    // asm: FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	THIS_PIECE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OBLINK4),R0
#if DEBUG
    // asm: 	BZ	$				;HOW DID WE MISS THIS?
#endif
    // asm: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
#if DEBUG
    // asm: 	BLT	$
#endif
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm: 	BU	CHECK_DIST
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOT_WEAVER_LP2", 0, 0);
    UNIMPL();
}

void THIS_PIECE(void)
{
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm: 	LDFLE	30,R1			;if 0 or less assume 30 mph
    // asm: 	FLOATP	@NFRAMES,R2
    // asm: 	MPYF	R2,R1
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
    // asm: JOINUP998
    // 	;find the theta delta to this position
    // 	;
    // asm: 	CALL	ARCTANF			;-> R0
    // asm: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm: 	SUBF	1,R1	;DBG
    // asm: 	BZ	NODIV
    // asm: 	CALL	DIV_F			;-> R0
NODIV:
    // asm: STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm: 	LDI	*+AR7(RHO_YELL),R0
    // asm: 	BNZ	NOTPRECOL
    // asm: 	CALL	PRECOLLIDE_PLYR
    // asm: 	BNC	NOTPRECOL
    // asm: 	CALL	PLYR_RIDE_RIGHT
    // asm: 	BC	NOTPRECOL
    // asm: 	LDI	*+AR7(DELTA_PSTAT),R0		;IN FRONT OF PLAYER?
    // asm: 	BZ	NOTPRECOL
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(RHO_YELL)
    // asm: 	RANDN	10
    // asm: 	CMPI	7,R0
    // asm: 	BGT	IIIL
    // asm: 	LDI	*+AR7(RHO_INIT),AR2
    // asm: 	MPYI	RT_SIZE,AR2
    // asm: 	ADDI	@RHO_TABLEI,AR2
    // asm: 	LDI	*+AR2(RT_ONSCREAM),AR2
    // asm: 	CMPI	0,AR2
    // asm: 	CALL	ONESNDFX
    // asm: 	LDIC	1,R0
    // asm: 	LDINC	0,R0
    // asm: 	STI	R0,*+AR7(RHO_YELL)
    // asm: 	BU	NONOISE
    // asm: 	LDI	*+AR7(RHO_INIT),R0
    // asm: 	MPYI	RT_SIZE,R0
    // asm: 	ADDI	@RHO_TABLEI,R0
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	*+AR2(RT_ONSCREAM),R0
    // asm: 	BZ	NOLOAD
    // asm: 	LDI	R0,AR2
    // asm: 	BU	LLL88
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THIS_PIECE", 0, 0);
    UNIMPL();
}

void IIIL(void)
{
NOLOAD:
    // asm: 	RANDN	B4HIT_SIZE
    // asm: 	LDI	R0,AR2
    // asm: 	ADDI	@B4HIT_TABLEI,AR2
    // asm: 	LDI	*AR2,AR2
LLL88:
    // asm: CALL	ONESNDFX
    // asm: 	LDIC	1,R0
    // asm: 	LDINC	0,R0
    // asm: 	STI	R0,*+AR7(RHO_YELL)
    // asm: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm: 	MPYF	0.01,R2
    // asm: 	STF	R2,*+AR5(CARTHROTTLE)
    // asm: 	BU	L99
    TRACE_EVENT(&g_crusn_machine->trace, "function", "IIIL", 0, 0);
    UNIMPL();
}

void NOTPRECOL(void)
{
    // 	;set throttle
    // asm: 	CLRF	R2
    // asm: 	STF	R2,*+AR5(CARBRAKE)
    // asm: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm: 	MPYF	1.01,R2
    // asm: 	CMPF	MIN_THROTTLE,R2
    // asm: 	LDFLT	MIN_THROTTLE,R2
    // asm: 	CMPF	MAX_THROTTLE,R2
    // asm: 	LDFGT	MAX_THROTTLE,R2
    // asm: 	STF	R2,*+AR7(DELTA_THROTTLE)
    // asm: 	STF	R2,*+AR5(CARTHROTTLE)
L99:
    // asm: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm: 	MPYF	2.50,R2			;depending on plyr.asm this may have to
    // asm: 	CALL	DRONEGO
    // asm: 	CALL	GETTRAK
RHO_SLP:
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_RHO_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	CALL	CKCAROFF	;OFF THE UNIVERSE ???
    // asm: 	BZ	RHO_DIE		;YES
    // asm: 	SLEEP	1
    // asm: 	BU	RHO_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTPRECOL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*CHECK IF OFF THE UNIVERSE
*RET EQ (R0=0) IF OFF UNIVERSE, NE (R0=1) IF IN BOUNDS
*AR7=DRONE PROCESS
*
 */
void CKCAROFF(void)
{
    // asm: 	LDI	*+AR7(DELTA_TPIECE),R0
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm: 	CMPI	AR0,R0
    // asm: 	BEQ	CKCXFAIL
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R1
    // asm: 	CMPI	R0,R1
    // asm: 	BLE	CKCXFAIL
    // asm: 	LDI	@DYNALIST_END,AR0		;GET FURTHEST ROAD ID
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	CMPI	R0,R1
    // asm: 	BLT	CKCXPASS
CKCXFAIL:
    // asm: 	LDI	0,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKCAROFF", 0, 0);
    UNIMPL();
}

void CKCXPASS(void)
{
    // asm: 	LDI	1,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKCXPASS", 0, 0);
    UNIMPL();
}

void RHO_DIE(void)
{
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	BZ	RHO_DIE1 		;NO, BLOW OUT...
    // asm: 	CALL	COMPTRAK
    // asm: 	BLE	RHO_DIE0
    // asm: 	CALL	SEND_RHO_XSFER
    // asm: 	B	OM_DRONE		;CONTROL SWAPS TO OTHER MACHINE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_DIE", 0, 0);
    UNIMPL();
}

void RHO_DIE0(void)
{
    // asm: 	LDI	*+AR5(CARNUM),R0	;GET ID
    // asm: 	CALL	SEND_RHO_KILL		;KILL OFF THE UTHA MACHINES MUTHA
RHO_DIE1:
    // asm: 	CALL	FREE_DRONE
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	DELCAR
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	LDI	*+AR4(OFLAGS),R1
    // asm: 	ANDN	R0,R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	TSTB	O_DYNAMIC,R0
    // asm: 	BZ	NODYNALEAN
    // asm: 	LDI	*+AR4(ORADZ),AR2
    // asm: 	CALL	PRC_KILL
NODYNALEAN:
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_DIE0", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void RHO_ISHIT(void)
{
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	ANDN	TYPE_M,R0
    // asm: 	OR	DEAD_VEH_T,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	STI	R0,*+AR5(CAR_ID)
    // asm: 	STI	R0,*+AR7(PID)
    // ;	INCM	@CAR_COLLS
    // asm: 	CREATEC	SMOKE_PUFF,SPAWNER_C
    // asm: 	CREATEC	EXP_PUFF,SPAWNER_C
    // asm: 	LDI	10,AR6
RHO_ISHITLP:
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BEQ	RHOISHIT_SLP
    // asm: 	DEC	AR6
    // asm: 	CMPI	0,AR6
    // asm: 	BLT	NOSMK
    // asm: 	CREATEC	SMOKE_PUFF,SPAWNER_C
NOSMK:
    // 	;dont disolve
    // 	;
    // 	;
    // asm: 	LDI	*+AR5(CARTRAK),R0
    // asm: 	STI	R0,*+AR7(DELTA_TPIECE)
    // 	;find piece which points to piece if it is not found
    // 	;or it is the initial piece exit rho code and commit
    // 	;suicide
    // asm: 	LDI	*+AR5(CARTRAK),AR1
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm: 	CMPI	AR0,AR1
    // asm: 	BEQ	RHO_DIE
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	LDI	*+AR1(OUSR1),R1
    // asm: 	CMPI	R0,R1
    // asm: 	BLT	RHO_DIE
    // 	;for now we die out
    // asm: 	CLRF	R2
    // asm: 	STF	R2,*+AR7(DELTA_THROTTLE)
    // asm: 	STF	R2,*+AR5(CARTHROTTLE)
    // ;	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm: 	CALL	DRONEGO
    // asm: 	CALL	GETTRAK
RHOISHIT_SLP:
    // asm: 	SLEEP	1
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_RHO_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	BU	RHO_ISHITLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_ISHIT", 0, 0);
    UNIMPL();
}

void SEND_RHO_XSFER(void)
{
    // asm: 	LDI	CB_RHO_XSFER,R0 	;MESSAGE HEADER
    // *SEND MESSAGE HEADER
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	STI	R0,*AR2
    // *SEND CAR ID #
    // asm: 	LDI	*+AR5(CARNUM),R0
    // asm: 	STI	R0,*+AR2(1)
    // *SEND CAR STATE, 1=HIT
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	AND	DEAD_VEH_T,R0
    // asm: 	STI	R0,*+AR2(2)
    // asm: 	LDI	3-1,RC
    // asm: 	CALL	MESSAGE_ADD
    // asm: 	POP	R0			;CLEAN OFF STACK
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR5(CAR_OM)
    // asm: 	B 	OM_DRONE  		;NOW CONTROLLED BY OTHER MACHINE
    // *
    // *GET A CAR FROM OTHER MACHINE
    // *AR4=CAR
    // *AR5=CAR BLOCK
    // *AR7=PROCESS
    // *
    // *NOTE SHOULD ADD IN FUTURE: CAR_SPIN,CARSPRAD,CARGEAR,CARRPM,CARSKID
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RHO_XSFER", 0, 0);
    UNIMPL();
}

void DECODE_RHO_XSFER(void)
{
    // asm: 	LDI	@IGNORE_UPDATES,R0	;RACE OVER?
    // asm: 	BNZ	DECRHOX2		;YES, NO UPDATES ALLOWED...
    // asm: 	CALL	FIND_DRONE  		;FIND CAR ID #
    // asm: 	BNZ	DECRHOX			;CAR NOT FOUND
    // *INITIALIZE SHIT
    // asm: 	LDI	AR0,AR4			;SAVE CAR INDEX
    // asm: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR BLOCK
    // asm: 	LDI	*+AR4(OPLINK),AR7	;GET PROCESS
    // asm: 	LDI	@RHO_ACTIVE_XSFERI,R2
    // asm: 	STI	R2,*+AR7(PWAKE)		;CHANGE WAKE-UP ADDR
    // asm: 	LDI	*AR2++,R0		;GET CAR STATE 1=HIT, 0=ACTIVE
    // asm: 	STI	R0,*+AR7(PR4)		;SAVE CODE IN REGISTER
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RHO_XSFER", 0, 0);
    UNIMPL();
}

void DECRHOX2(void)
{
    // asm: 	ADDI	2,AR2			;UPDATES NOT ALLOWED
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECRHOX2", 0, 0);
    UNIMPL();
}

void DECRHOX(void)
{
    // asm: 	ADDI	1,AR2			;COULDN'T FIND CAR ERROR
    // asm: 	RETS
    // *
    // *TRANSFER ACTIVE RHO
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECRHOX", 0, 0);
    UNIMPL();
}

void RHO_ACTIVE_XSFER(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_OM)		;CAR BELONGS TO ME NOW!!!
    // asm: 	LDI	*+AR5(CARTRACK_ID),R2		;GET TRACK ID
    // asm: 	STI	R2,*+AR7(DELTA_LAST_OID)	;SAVE IT HERE ALSO
    // asm: 	LDI	@DYNALIST_END,AR0		;GET FURTHEST ROAD ID
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	CMPI	R0,R2
    // asm: 	BGT	RHO_DIE0			;TOO FAR OUT, DIE
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	CMPI	R0,R2
    // asm: 	BLT	RHO_DIE0			;BEHIND US KILL HIM
    // asm: 	CALL	FIND_DYNA			;GET TRACKING PIECE POINTER
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARDROT)		;CLEAN UP REENTRY
    // asm: 	STF	R0,*+AR5(CARSPRAD)
    // asm: 	STF	R0,*+AR5(CARSKID)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
    // asm: 	LDI	R4,R4	 			;CHECK HIT RHO
    // asm: 	BZ	RHO_LP
    // asm: 	LDI	0,AR6				;WERE HIT, GO TO DEAD LOOP
    // asm: 	LDI	*+AR4(OID),R0			;MAKE 'EM DEAD
    // asm: 	ANDN	TYPE_M,R0
    // asm: 	OR	DEAD_VEH_T,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	STI	R0,*+AR5(CAR_ID)
    // asm: 	STI	R0,*+AR7(PID)
    // asm: 	CALL	GETTRAK
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(DELTA_RADYDELTA)	;CLEAR THIS BULLSHIT
    // asm: 	B     	RHO_ISHITLP
    // *
    // *KILL OFF RHO MESSAGE
    // *R0=ID
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_ACTIVE_XSFER", 0, 0);
    UNIMPL();
}

void SEND_RHO_KILL(void)
{
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_RHO_KILL,R1
    // asm: 	STI	R1,*AR2
    // asm: 	STI	R0,*+AR2(1)
    // asm: 	LDI	2-1,RC
    // asm: 	CALL	MESSAGE_ADD
    // asm: 	RETS
    // *
    // *KILL OFF RHO
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RHO_KILL", 0, 0);
    UNIMPL();
}

void DECODE_RHO_KILL(void)
{
    // asm: 	LDI	@IGNORE_UPDATES,R0
    // asm: 	BNZ	DRKXX
    // asm: 	CALL	FIND_DRONE  		;GET DRONE OBJ IN AR0
    // asm: 	BNZ	DRKX
    // asm: 	LDI	*+AR0(OPLINK),AR7
    // asm: 	LDI	@RHO_DIE1I,R2		;KILL THE SOMBITCH
    // asm: 	STI	R2,*+AR7(PWAKE)		;CHANGE WAKE-UP ADDR
DRKX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RHO_KILL", 0, 0);
    UNIMPL();
}

void DRKXX(void)
{
    // asm: 	INC	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRKXX", 0, 0);
    UNIMPL();
}

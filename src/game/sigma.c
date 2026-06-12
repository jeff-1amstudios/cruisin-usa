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

/*
 * Source module: asm/SIGMA.ASM
 */

void SIGMA_DRONE(void);
void GOAHEAD(void);
void DONTWORRY(void);
void NOSL2DIE(void);
void THIS_PIECE(void);
void NOTPRECOL(void);
void BREAKDOWN(void);
void SIGMA_DIE(void);

/* *SIGMA_STARTUP STRUCT
 */
#define SS_MODEL 0
#define SS_FLAG 1
#define SS_COPCAR 1
#define SS_LONG 2
#define SIGT_GTRUCK 0
#define SIGT_CBUS 1
#define SIGT_COPCAR 2
#define SIGT_MUSCLE 3
#define SIGT_CARAVAN 4
#define SIGT_SBUS 5
#define SIGT_PTRUCKG 6
#define SIGT_JEEP 7
#define SIGMA_LIST_LEN 16
/* asm: SIGMA_LISTI	.word	SIGMA_LIST */
#define SIGMA_LISTI SIGMA_LIST
/* asm: SIGMA_LIST */
/* asm: 	.word	GTRUCK_MOD,0 */
/* asm: 	.word	CBUS_MOD,SS_LONG */
/* asm: 	.word	COPCAR_MOD,SS_COPCAR */
/* asm: 	.word	MUSTANG_MOD,0 */
/* asm: 	.word	MUSCLE_MOD,0 */
/* asm: 	.word	CARAVAN_MOD,0 */
/* asm: 	.word	SBUS_MOD,SS_LONG */
/* asm: 	.word	PTRUCKG_MOD,0 */
/* asm: 	.word	JEEP_MOD,0 */
/* asm: 	.word	GTRUCK_MOD,0 */
/* asm: 	.word	COPCAR_MOD,SS_COPCAR */
/* asm: 	.word	MUSTANG_MOD,0 */
/* asm: 	.word	MUSCLE_MOD,0 */
/* asm: 	.word	CARAVAN_MOD,0 */
/* asm: 	.word	PTRUCKG_MOD,0 */
/* asm: 	.word	JEEP_MOD,0 */
int SIGMA_LIST[] = {
    GTRUCK_MOD, 0,
    CBUS_MOD, SS_LONG,
    COPCAR_MOD, SS_COPCAR,
    MUSTANG_MOD, 0,
    MUSCLE_MOD, 0,
    CARAVAN_MOD, 0,
    SBUS_MOD, SS_LONG,
    PTRUCKG_MOD, 0,
    JEEP_MOD, 0,
    GTRUCK_MOD, 0,
    COPCAR_MOD, SS_COPCAR,
    MUSTANG_MOD, 0,
    MUSCLE_MOD, 0,
    CARAVAN_MOD, 0,
    PTRUCKG_MOD, 0,
    JEEP_MOD, 0,
};
#define SIGMA_PSYCHO 1

/* *----------------------------------------------------------------------------
*SIGMA IS A BIG FAT PIG THAT STARTS UP AHEAD OF THE PLAYER
*AND MOVES RATHER SLOWLY AKIN TO A CTA BUS, OR A COMBINE VEHICLE.
*IN ANY INSTANCE THE DRONE DISAPPEARS ONCE IT IS TRACKING BENEATH THE WORLD.
*THERE CAN ONLY BE ONE SIGMA IN THE UNIVERSE AT ONCE.
*SIGMA TRAVELS IN THE SAME DIRECTION AS THE PLAYER.
*
*
 */
void SIGMA_DRONE(void)
{
    // asm: 	LDI	@DD_MAX_DRONES,R0
    // asm: 	BNZ	GOAHEAD
    // asm: 	SLEEP	1
    // asm: 	BU	SIGMA_DRONE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIGMA_DRONE", 0, 0);
    UNIMPL();
}

void GOAHEAD(void)
{
    // asm: 	LDI	SIGMA_LIST_LEN,AR2
    // asm: 	CALL	RANDU0
    // asm: 	CMPI	SIGT_COPCAR,R0		;ONLY 1 COPCAR ACTIVE AT A TIME
    // asm: 	BNE	NOT_COP
    // ;	LDI	@COP_ACTIVE,R1
    // ;	LDINZ	SIGT_MUSCLE,R0
NOT_COP:
    // asm: WORIT	;RETURN FROM LONG VEHICLE IGNORE
    // ;	LDI	SIGT_COPCAR,R0		;COPCAR DEBUG
    // asm: 	STI	R0,*+AR7(SIGMA_MODEL)
    // asm: 	MPYI	2,R0
    // asm: 	LDI	@SIGMA_LISTI,AR2
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*+AR2(SS_FLAG),R0
    // asm: 	STI	R0,*+AR7(SIGMA_FLAG)
    // ;	CMPI	SS_COPCAR,R0
    // ;	BEQ	COPCAR_DRONE
    // asm: 	LDI	*AR2,AR2
    // asm: 	LDI	@NOLONG_VEHICLES,R1
    // asm: 	BZ	DONTWORRY
    // asm: 	TSTB	SS_LONG,R0
    // asm: 	BZ	DONTWORRY
    // asm: 	LDI	SIGT_GTRUCK,R0
    // asm: 	BU	WORIT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GOAHEAD", 0, 0);
    UNIMPL();
}

void DONTWORRY(void)
{
    // asm: 	STI	AR2,*+AR7(DELTA_MODEL)
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDI	*+AR2(VEHTAB_MODEL),AR2
    // asm: 	STI	R4,*+AR7(DELTA_INIT)
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	SUICIDE			;abort process if no object available
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm: 	CALL	VEHICLE_ANI_INIT
    // asm: 	CALL	DELTA_OINIT
    // asm: 	LDI	DRONE_C|VEHICLE_T|DRNE_SIGMA,R0
    // asm: 	STI	R0,*+AR5(CAR_ID)
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	STI	R0,*+AR7(PID)
    // asm: 	CALL	SET_DRONE_PAL
    // asm: 	RANDN	2
    // asm: 	ADDI	2,R0
    // asm: 	STI	R0,*+AR7(DELTA_STATUS)
    // 	;init position at two pieces before end of universe
    // 	;
    // asm: 	LDI	@DYNALIST_END,AR2
    // asm: 	LDI	*+AR2(OBLINK4),AR2
    // asm: 	LDI	*+AR2(OBLINK4),AR2
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm: 	CALL	SUB_FUNCTION			;MATRIXA,VECTORA,R2
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
    // asm: 	STI	R0,*+AR7(SIGMA_ONCE)
    // asm: 	STI	R0,*+AR7(SIGMA_YELL)
    // asm: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // 	;Weaving SIGMA????
    // asm: 	RANDN	10	;1 in 10 chance
    // asm: 	CMPI	0,R0
    // asm: 	BNE	NOTWEAVER
    // asm: 	LDI	SIGMA_PSYCHO,R0
    // asm: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // asm: 	RANDN	50
    // asm: 	ADDI	20,R0
    // asm: 	STI	R0,*+AR7(DELTA_PSTAT)
NOTWEAVER:
    // *
    // *
    // *
SIGMA_LP:
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BEQ	SIGMASLP
    // asm: 	CALL	AHEAD_OF_PLAYER_P
    // asm: 	LDIC	1,R0
    // asm: 	LDINC	0,R0
    // asm: 	STI	R0,*+AR7(DELTA_PSTAT)
    // ;	;CHECK TO SEE IF...
    // ;	;	WE ARE FAR ENOUGH BEHIND THE PLYR THAT
    // ;	;	WE CAN KILL OURSELVES
    // ;	;
    // ;	CMPI	0,R0
    // ;	BNE	NOTBEHIND_PLAYER
    // ;
    // ;	LDI	*+AR5(CARTRAK),AR0
    // ;	LDI	*+AR0(OUSR1),R0
    // ;	LDI	@PLYCBLK,AR1
    // ;	ADDI	4,R0
    // ;	LDI	*+AR1(CARTRAK),AR1
    // ;	CMPI	*+AR1(OUSR1),R0
    // ;	BLE	SIGMA_DIE
    // ;
    // ;NOTBEHIND_PLAYER
    // 	;Weaver?
    // asm: 	LDI	*+AR7(DELTA_PLAYIT),R0
    // asm: 	CMPI	SIGMA_PSYCHO,R0
    // asm: 	BNE	NOTPSYCHO_LP
    // asm: 	LDI	*+AR7(DELTA_PSTAT),R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,*+AR7(DELTA_PSTAT)
    // asm: 	CMPI	0,R0
    // asm: 	BGT	NOTPSYCHO_LP
    // asm: 	LDI	*+AR7(DELTA_STATUS),R0
    // asm: 	CMPI	2,R0
    // asm: 	LDIEQ	3,R0
    // asm: 	LDINE	2,R0
    // asm: 	STI	R0,*+AR7(DELTA_STATUS)
    // asm: 	RANDN	50
    // asm: 	ADDI	30,R0
    // asm: 	STI	R0,*+AR7(DELTA_PSTAT)
NOTPSYCHO_LP:
    // 	;if all 4 wheels are off then effective breakdown
    // asm: 	LDI	*+AR5(RF_PCOL),AR0
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm: 	LDI	*+AR5(LF_PCOL),AR0
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm: 	LDI	*+AR5(RR_PCOL),AR0
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm: 	LDI	*+AR5(LR_PCOL),AR0
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm: 	LDI	*+AR7(SIGMA_ONCE),R0
    // asm: 	BZ	NOSL2DIE2
    // asm: 	BU	BREAKDOWN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DONTWORRY", 0, 0);
    UNIMPL();
}

void NOSL2DIE(void)
{
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(SIGMA_ONCE)
NOSL2DIE2:
    // asm: 	LDI	*+AR5(CAR_BUMP),R0
    // asm: 	BZ	NOBUMP
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR5(CAR_BUMP)
    // ;	INCM	@CAR_COLLS
    // asm: 	CREATEC	EXP_PUFF,SPAWNER_C
NOBUMP:
    // 	;it the plyr is zooming by
    // 	;
    // asm: 	LDI	*+AR7(SIGMA_YELL),R0
    // asm: 	BNZ	NOYELL
    // asm: 	LDI	*+AR7(DELTA_PSTAT),R0		;IN FRONT OF PLAYER?
    // asm: 	BZ	NOYELL
    // asm: 	CALL	DIST_TO_PLYR
    // asm: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm: 	FLOAT	5000,R2
    // asm: 	CMPF	R2,R0		;<5000
    // asm: 	BGT	NOYELL
    // asm: 	CMPF	R0,R1
    // asm: 	BLT	NOYELL
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDF	*+AR0(CARSPEED),R0
    // asm: 	CMPF	127,R0
    // asm: 	BLT	NOYELL
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(SIGMA_YELL)
    // asm: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDI	*+AR2(VEHTAB_PASSBY),AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	NOYELL
    // asm: 	CALL	ONESNDFX
NOYELL:
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OLINK4),AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BEQ	SIGMASLP		;IF AT END OF WORLD DONT MOVE!
    // 	;find piece which points to piece if it is not found
    // 	;or it is the initial piece exit rho code and commit
    // 	;suicide
    // asm: 	LDI	*+AR7(DELTA_TPIECE),R0
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm: 	CMPI	AR0,R0			;BR-> WE ARE ATTACKING THE START OF UNIVERSE
    // asm: 	BEQ	SIGMA_DIE
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R1
    // asm: 	CMPI	R0,R1			;BR-> WE ARE UNDER THE START OF UNIVERSE
    // asm: 	BLT	SIGMA_DIE
    // 	;
    // 	;simply drive slowly forward until we are below the section list
    // 	;
    // 	;
    // 	;see if we should track the next piece
CHECK_DIST:
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	RS	8,R0
    // asm: 	LDI	@SECTIONIDX,R1
    // asm: 	SUBPI	@DGROUP_COUNT,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BLE	SIGMA_DIE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	BZ	SIGMASLP
    // asm: 	CALL	GET_TRACK_POS			;CHECK IF WE SHOULD ADVANCE
    // asm: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	THIS_PIECE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OLINK4),R0
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOSL2DIE", 0, 0);
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
    // asm: 	CALL	PRECOLLIDE_PLYR
    // asm: 	BNC	NOTPRECOL
    // asm: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm: 	MPYF	0.01,R2
    // asm: 	STF	R2,*+AR5(CARTHROTTLE)
    // asm: 	BU	L99
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THIS_PIECE", 0, 0);
    UNIMPL();
}

void NOTPRECOL(void)
{
    // 	;set throttle
    // asm: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm: 	MPYF	1.01,R2
    // asm: 	CMPF	MIN_THROTTLE,R2
    // asm: 	LDFLT	MIN_THROTTLE,R2
    // asm: 	CMPF	MAX_SIGMA_THROTTLE,R2
    // asm: 	LDFGT	MAX_SIGMA_THROTTLE,R2
    // asm: 	STF	R2,*+AR7(DELTA_THROTTLE)
    // asm: 	STF	R2,*+AR5(CARTHROTTLE)
L99:
    // asm: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm: 	MPYF	1.95,R2			;depending on plyr.asm this may have to
    // asm: 	CALL	DRONE_RIDE_RIGHT	;FIND DISTANCE TO CENTER OF ROAD
    // asm: 	STF	R0,*+AR5(CARDIST2CNTR)
    // asm: 	CALL	DRONEGO
    // asm: 	CALL	GETTRAK
    // ;	CALL	PTS
SIGMASLP:
    // asm: 	SLEEP	1
    // asm: 	B	SIGMA_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTPRECOL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*In the case of a 'breakdown' we simply wait until the universe has passed
*us up, and then goto SIGMA_DIE.
*
*
 */
void BREAKDOWN(void)
{
    // ;	LDI	*+AR4(OID),R0
    // ;	ANDN	TYPE_M,R0
    // ;	OR	DEAD_VEH_T,R0
    // ;	STI	R0,*+AR4(OID)
    // ;	STI	R0,*+AR5(CAR_ID)
    // ;	STI	R0,*+AR7(PID)
    // asm: 	CREATEC	SMOKE_PUFF,2
    // asm: 	LDI	10,AR6
BREAKDOWNLP:
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BEQ	BREAKDNSLP
    // asm: 	DEC	AR6
    // asm: 	CMPI	0,AR6
    // asm: 	BLT	NOSMK
    // asm: 	CREATEC	SMOKE_PUFF,2
NOSMK:
    // asm: 	LDI	*+AR5(CARTRAK),AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	RS	8,R0
    // asm: 	LDI	@(DGROUPS+DGRP_IDX),R1
    // asm: 	CMPI	R1,R0
    // asm: 	BLT	SIGMA_DIE
    // asm: 	CLRF	R2
    // asm: 	STF	R2,*+AR5(CARTHROTTLE)
    // asm: 	CALL	DRONEGO
    // asm: 	CALL	GETTRAK
BREAKDNSLP:
    // asm: 	SLEEP	1
    // asm: 	BU	BREAKDOWNLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BREAKDOWN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SIGMA_DIE(void)
{
    // asm: 	BU	RHO_DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIGMA_DIE", 0, 0);
    UNIMPL();
}

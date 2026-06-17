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
#include "port.h"

/*
 * Source module: asm/SIGMA.ASM
 */

void SIGMA_DRONE(void);
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
    // asm 0000A42A: 	BNZ	GOAHEAD
    // asm 0000A42B: 	SLEEP	1
    // asm 0000A42C: 	BU	SIGMA_DRONE
GOAHEAD:
    // asm: 	LDI	SIGMA_LIST_LEN,AR2
    // asm 0000A42F: 	CALL	RANDU0
    // asm: 	CMPI	SIGT_COPCAR,R0		;ONLY 1 COPCAR ACTIVE AT A TIME
    // asm 0000A431: 	BNE	NOT_COP
    // ;	LDI	@COP_ACTIVE,R1
    // ;	LDINZ	SIGT_MUSCLE,R0
NOT_COP:
    // asm 0000A433: WORIT	;RETURN FROM LONG VEHICLE IGNORE
    // ;	LDI	SIGT_COPCAR,R0		;COPCAR DEBUG
    // asm: 	STI	R0,*+AR7(SIGMA_MODEL)
    // asm 0000A433: 	MPYI	2,R0
    // asm 0000A434: 	LDI	@SIGMA_LISTI,AR2
    // asm 0000A435: 	ADDI	R0,AR2
    // asm 0000A436: 	LDI	*+AR2(SS_FLAG),R0
    // asm 0000A437: 	STI	R0,*+AR7(SIGMA_FLAG)
    // ;	CMPI	SS_COPCAR,R0
    // ;	BEQ	COPCAR_DRONE
    // asm: 	LDI	*AR2,AR2
    // asm: 	LDI	@NOLONG_VEHICLES,R1
    // asm 0000A43A: 	BZ	DONTWORRY
    // asm: 	TSTB	SS_LONG,R0
    // asm 0000A43C: 	BZ	DONTWORRY
    // asm: 	LDI	SIGT_GTRUCK,R0
    // asm 0000A43E: 	BU	WORIT
DONTWORRY:
    // asm: 	STI	AR2,*+AR7(DELTA_MODEL)
    // asm 0000A440: 	MPYI	VEHTAB_SIZE,AR2
    // asm 0000A441: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 0000A442: 	LDI	*+AR2(VEHTAB_MODEL),AR2
    // asm 0000A443: 	STI	R4,*+AR7(DELTA_INIT)
    // asm: 	CALL	OBJ_GETE
    // asm 0000A445: 	BC	SUICIDE			;abort process if no object available
    // asm 0000A446: 	LDI	AR0,AR4
    // asm: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm 0000A448: 	CALL	VEHICLE_ANI_INIT
    // asm: 	CALL	DELTA_OINIT
    // asm 0000A44A: 	LDI	DRONE_C|VEHICLE_T|DRNE_SIGMA,R0
    // asm 0000A44B: 	STI	R0,*+AR5(CAR_ID)
    // asm 0000A44C: 	STI	R0,*+AR4(OID)
    // asm 0000A44D: 	STI	R0,*+AR7(PID)
    // asm: 	CALL	SET_DRONE_PAL
    // asm: 	RANDN	2
    // asm 0000A450: 	ADDI	2,R0
    // asm 0000A452: 	STI	R0,*+AR7(DELTA_STATUS)
    // 	;init position at two pieces before end of universe
    // 	;
    // asm: 	LDI	@DYNALIST_END,AR2
    // asm 0000A454: 	LDI	*+AR2(OBLINK4),AR2
    // asm 0000A455: 	LDI	*+AR2(OBLINK4),AR2
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm 0000A457: 	LDI	*+AR2(OUSR1),R0
    // asm 0000A458: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm 0000A459: 	CALL	SUB_FUNCTION			;MATRIXA,VECTORA,R2
    // asm: 	LDP	@_VECTORA
    // asm 0000A45B: 	LDF	*+AR2(OPOSX),R0
    // asm 0000A45C: 	ADDF	@_VECTORA+X,R0
    // asm 0000A45D: 	STF	R0,*+AR4(OPOSX)
    // asm 0000A45E: 	LDF	*+AR2(OPOSY),R0
    // asm 0000A45F: 	SUBF	*+AR5(CARWHLTAB+1),R0
    // asm 0000A460: 	ADDF	@_VECTORA+Y,R0
    // asm 0000A461: 	STF	R0,*+AR4(OPOSY)
    // asm 0000A462: 	LDF	*+AR2(OPOSZ),R0
    // asm 0000A463: 	ADDF	@_VECTORA+Z,R0
    // asm 0000A464: 	STF	R0,*+AR4(OPOSZ)
    // asm 0000A465: 	SETDP
    // 	;initialize Ytheta to the intentional direction
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm 0000A467: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm 0000A468: 	STF	R2,*+AR5(CARYROT)
    // asm 0000A469: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDI	AR4,AR2
    // asm 0000A46B: 	ADDI	OMATRIX,AR2
    // asm 0000A46C: 	CALL	FIND_YMATRIX
    // asm: 	CLRI	R0
    // asm 0000A46E: 	STI	R0,*+AR7(SIGMA_ONCE)
    // asm 0000A46F: 	STI	R0,*+AR7(SIGMA_YELL)
    // asm 0000A470: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // 	;Weaving SIGMA????
    // asm: 	RANDN	10	;1 in 10 chance
    // asm 0000A472: 	CMPI	0,R0
    // asm 0000A474: 	BNE	NOTWEAVER
    // asm: 	LDI	SIGMA_PSYCHO,R0
    // asm 0000A476: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // asm: 	RANDN	50
    // asm 0000A478: 	ADDI	20,R0
    // asm 0000A47A: 	STI	R0,*+AR7(DELTA_PSTAT)
NOTWEAVER:
    // *
    // *
    // *
SIGMA_LP:
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm 0000A47C: 	CMPI	SM_HALT,R0
    // asm 0000A47D: 	BEQ	SIGMASLP
    // asm: 	CALL	AHEAD_OF_PLAYER_P
    // asm 0000A47F: 	LDIC	1,R0
    // asm 0000A480: 	LDINC	0,R0
    // asm 0000A481: 	STI	R0,*+AR7(DELTA_PSTAT)
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
    // asm 0000A483: 	CMPI	SIGMA_PSYCHO,R0
    // asm 0000A484: 	BNE	NOTPSYCHO_LP
    // asm: 	LDI	*+AR7(DELTA_PSTAT),R0
    // asm 0000A486: 	DEC	R0
    // asm 0000A487: 	STI	R0,*+AR7(DELTA_PSTAT)
    // asm 0000A488: 	CMPI	0,R0
    // asm 0000A489: 	BGT	NOTPSYCHO_LP
    // asm: 	LDI	*+AR7(DELTA_STATUS),R0
    // asm 0000A48B: 	CMPI	2,R0
    // asm 0000A48C: 	LDIEQ	3,R0
    // asm 0000A48D: 	LDINE	2,R0
    // asm 0000A48E: 	STI	R0,*+AR7(DELTA_STATUS)
    // asm: 	RANDN	50
    // asm 0000A490: 	ADDI	30,R0
    // asm 0000A492: 	STI	R0,*+AR7(DELTA_PSTAT)
NOTPSYCHO_LP:
    // 	;if all 4 wheels are off then effective breakdown
    // asm: 	LDI	*+AR5(RF_PCOL),AR0
    // asm 0000A494: 	LDI	*+AR0(OID),R0
    // asm 0000A495: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm 0000A496: 	LDI	*+AR5(LF_PCOL),AR0
    // asm 0000A498: 	LDI	*+AR0(OID),R0
    // asm 0000A499: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm 0000A49A: 	LDI	*+AR5(RR_PCOL),AR0
    // asm 0000A49C: 	LDI	*+AR0(OID),R0
    // asm 0000A49D: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm 0000A49E: 	LDI	*+AR5(LR_PCOL),AR0
    // asm 0000A4A0: 	LDI	*+AR0(OID),R0
    // asm 0000A4A1: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm 0000A4A2: 	LDI	*+AR7(SIGMA_ONCE),R0
    // asm 0000A4A4: 	BZ	NOSL2DIE2
    // asm 0000A4A5: 	BU	BREAKDOWN
    // asm 0000A4A6: NOSL2DIE
    // asm: 	LDI	1,R0
    // asm 0000A4A7: 	STI	R0,*+AR7(SIGMA_ONCE)
NOSL2DIE2:
    // asm: 	LDI	*+AR5(CAR_BUMP),R0
    // asm 0000A4A9: 	BZ	NOBUMP
    // asm 0000A4AA: 	CLRI	R0
    // asm 0000A4AB: 	STI	R0,*+AR5(CAR_BUMP)
    // ;	INCM	@CAR_COLLS
    // asm: 	CREATEC	EXP_PUFF,SPAWNER_C
NOBUMP:
    // 	;it the plyr is zooming by
    // 	;
    // asm: 	LDI	*+AR7(SIGMA_YELL),R0
    // asm 0000A4B0: 	BNZ	NOYELL
    // asm: 	LDI	*+AR7(DELTA_PSTAT),R0		;IN FRONT OF PLAYER?
    // asm 0000A4B2: 	BZ	NOYELL
    // asm: 	CALL	DIST_TO_PLYR
    // asm 0000A4B4: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm 0000A4B5: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm 0000A4B6: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm: 	FLOAT	5000,R2
    // asm 0000A4B8: 	CMPF	R2,R0		;<5000
    // asm 0000A4B9: 	BGT	NOYELL
    // asm: 	CMPF	R0,R1
    // asm 0000A4BB: 	BLT	NOYELL
    // asm: 	LDI	@PLYCBLK,AR0
    // asm 0000A4BD: 	LDF	*+AR0(CARSPEED),R0
    // asm 0000A4BE: 	CMPF	127,R0
    // asm 0000A4BF: 	BLT	NOYELL
    // asm: 	LDI	1,R0
    // asm 0000A4C1: 	STI	R0,*+AR7(SIGMA_YELL)
    // asm: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm 0000A4C3: 	MPYI	VEHTAB_SIZE,AR2
    // asm 0000A4C4: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 0000A4C5: 	LDI	*+AR2(VEHTAB_PASSBY),AR2
    // asm 0000A4C6: 	CMPI	0,AR2
    // asm 0000A4C7: 	BEQ	NOYELL
    // asm 0000A4C8: 	CALL	ONESNDFX
NOYELL:
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000A4CA: 	LDI	*+AR2(OLINK4),AR0
    // asm 0000A4CB: 	CMPI	0,AR0
    // asm 0000A4CC: 	BEQ	SIGMASLP		;IF AT END OF WORLD DONT MOVE!
    // 	;find piece which points to piece if it is not found
    // 	;or it is the initial piece exit rho code and commit
    // 	;suicide
    // asm: 	LDI	*+AR7(DELTA_TPIECE),R0
    // asm 0000A4CE: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm 0000A4CF: 	CMPI	AR0,R0			;BR-> WE ARE ATTACKING THE START OF UNIVERSE
    // asm 0000A4D0: 	BEQ	SIGMA_DIE
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm 0000A4D2: 	LDI	*+AR7(DELTA_LAST_OID),R1
    // asm 0000A4D3: 	CMPI	R0,R1			;BR-> WE ARE UNDER THE START OF UNIVERSE
    // asm 0000A4D4: 	BLT	SIGMA_DIE
    // 	;
    // 	;simply drive slowly forward until we are below the section list
    // 	;
    // 	;
    // 	;see if we should track the next piece
CHECK_DIST:
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm 0000A4D6: 	RS	8,R0
    // asm 0000A4D7: 	LDI	@SECTIONIDX,R1
    // asm 0000A4D8: 	SUBPI	@DGROUP_COUNT,R1
    // asm 0000A4D9: 	CMPI	R1,R0
    // asm 0000A4DA: 	BLE	SIGMA_DIE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000A4DC: 	LDI	*+AR2(OLINK4),R0
    // asm 0000A4DD: 	BZ	SIGMASLP
    // asm: 	CALL	GET_TRACK_POS			;CHECK IF WE SHOULD ADVANCE
    // asm 0000A4DF: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm 0000A4E0: 	CMPF	R1,R0
    // asm 0000A4E1: 	BGT	THIS_PIECE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000A4E3: 	LDI	*+AR2(OLINK4),R0
#if DEBUG
    // asm: 	BZ	$				;HOW DID WE MISS THIS?
#endif
    // asm: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm 0000A4E5: 	LDI	R0,AR0
    // asm 0000A4E6: 	LDI	*+AR0(OUSR1),R0
#if DEBUG
    // asm: 	BLT	$
#endif
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm 0000A4E8: 	BU	CHECK_DIST
THIS_PIECE:
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm 0000A4EA: 	LDFLE	30,R1			;if 0 or less assume 30 mph
    // asm 0000A4EB: 	FLOATP	@NFRAMES,R2
    // asm 0000A4EC: 	MPYF	R2,R1
    // asm 0000A4ED: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm 0000A4EE: 	FIX	R0,R7
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000A4F0: 	LDP	@_VECTORA		;lane position
    // asm 0000A4F1: 	LDF	*+AR2(OPOSX),R2		;X
    // asm 0000A4F2: 	SUBF	*+AR4(OPOSX),R2
    // asm 0000A4F3: 	ADDF	@_VECTORA+X,R2
    // asm 0000A4F4: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm 0000A4F5: 	SUBF	*+AR4(OPOSZ),R3
    // asm 0000A4F6: 	ADDF	@_VECTORA+Z,R3
    // asm 0000A4F7: 	SETDP
    // asm 0000A4F8: JOINUP998
    // 	;find the theta delta to this position
    // 	;
    // asm: 	CALL	ARCTANF			;-> R0
    // asm 0000A4F9: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm 0000A4FB: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm 0000A4FC: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm 0000A4FD: 	SUBF	1,R1	;DBG
    // asm 0000A4FE: 	BZ	NODIV
    // asm 0000A4FF: 	CALL	DIV_F			;-> R0
NODIV:
    // asm 0000A500: STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm: 	CALL	PRECOLLIDE_PLYR
    // asm 0000A502: 	BNC	NOTPRECOL
    // asm: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm 0000A504: 	MPYF	0.01,R2
    // asm 0000A505: 	STF	R2,*+AR5(CARTHROTTLE)
    // asm 0000A506: 	BU	L99
NOTPRECOL:
    // 	;set throttle
    // asm: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm 0000A508: 	MPYF	1.01,R2
    // asm 0000A509: 	CMPF	MIN_THROTTLE,R2
    // asm 0000A50A: 	LDFLT	MIN_THROTTLE,R2
    // asm 0000A50B: 	CMPF	MAX_SIGMA_THROTTLE,R2
    // asm 0000A50C: 	LDFGT	MAX_SIGMA_THROTTLE,R2
    // asm 0000A50D: 	STF	R2,*+AR7(DELTA_THROTTLE)
    // asm 0000A50E: 	STF	R2,*+AR5(CARTHROTTLE)
L99:
    // asm: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm 0000A510: 	MPYF	1.95,R2			;depending on plyr.asm this may have to
    // asm: 	CALL	DRONE_RIDE_RIGHT	;FIND DISTANCE TO CENTER OF ROAD
    // asm 0000A512: 	STF	R0,*+AR5(CARDIST2CNTR)
    // asm: 	CALL	DRONEGO
    // asm 0000A514: 	CALL	GETTRAK
    // ;	CALL	PTS
SIGMASLP:
    // asm: 	SLEEP	1
    // asm 0000A516: 	B	SIGMA_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIGMA_DRONE", 0, 0);
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
    // asm 0000A51C: BREAKDOWNLP
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm 0000A51D: 	CMPI	SM_HALT,R0
    // asm 0000A51E: 	BEQ	BREAKDNSLP
    // asm: 	DEC	AR6
    // asm 0000A520: 	CMPI	0,AR6
    // asm 0000A521: 	BLT	NOSMK
    // asm 0000A522: 	CREATEC	SMOKE_PUFF,2
NOSMK:
    // asm: 	LDI	*+AR5(CARTRAK),AR0
    // asm 0000A526: 	LDI	*+AR0(OUSR1),R0
    // asm 0000A527: 	RS	8,R0
    // asm 0000A528: 	LDI	@(DGROUPS+DGRP_IDX),R1
    // asm 0000A529: 	CMPI	R1,R0
    // asm 0000A52A: 	BLT	SIGMA_DIE
    // asm: 	CLRF	R2
    // asm 0000A52C: 	STF	R2,*+AR5(CARTHROTTLE)
    // asm 0000A52D: 	CALL	DRONEGO
    // asm 0000A52E: 	CALL	GETTRAK
    // asm 0000A530: BREAKDNSLP
    // asm: 	SLEEP	1
    // asm 0000A530: 	BU	BREAKDOWNLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BREAKDOWN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SIGMA_DIE(void)
{
    // asm: 	BU	RHO_DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIGMA_DIE", 0, 0);
    UNIMPL();
}

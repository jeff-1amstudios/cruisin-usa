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
void BREAKDOWN(void);
void SIGMA_DIE(void);

#define SIGMA_LISTI SIGMA_LIST

extern int SIGMA_LIST[];

/*
*----------------------------------------------------------------------------
*
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

// *SIGMA_STARTUP STRUCT
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
/* asm: 	 */
/* asm: 	.word	CARAVAN_MOD,0 */
/* asm: 	.word	SBUS_MOD,SS_LONG */
/* asm: 	.word	PTRUCKG_MOD,0 */
/* asm: 	.word	JEEP_MOD,0 */
/* asm: 	.word	GTRUCK_MOD,0 */
/* asm: 	 */
/* asm: 	.word	COPCAR_MOD,SS_COPCAR */
/* asm: 	.word	MUSTANG_MOD,0 */
/* asm: 	.word	MUSCLE_MOD,0 */
/* asm: 	.word	CARAVAN_MOD,0 */
/* asm: 	.word	PTRUCKG_MOD,0 */
/* asm: 	 */
/* asm: 	.word	JEEP_MOD,0 */
/* asm: 	 */
/* asm: 	 */
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

/*
*----------------------------------------------------------------------------
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
    // asm 0000A42A: 	LDI	@DD_MAX_DRONES,R0
    // asm 0000A42B: 	BNZ	GOAHEAD
    // asm 0000A42C: 	SLEEP	1
    // asm 0000A42E: 	BU	SIGMA_DRONE
GOAHEAD:
    // asm 0000A42F: 	LDI	SIGMA_LIST_LEN,AR2
    // asm 0000A430: 	CALL	RANDU0
    // asm 0000A431: 	CMPI	SIGT_COPCAR,R0		;ONLY 1 COPCAR ACTIVE AT A TIME
    // asm 0000A432: 	BNE	NOT_COP
    // ;	LDI	@COP_ACTIVE,R1
    // ;	LDINZ	SIGT_MUSCLE,R0
NOT_COP:
    // asm 0000A433: WORIT	;RETURN FROM LONG VEHICLE IGNORE
    // ;	LDI	SIGT_COPCAR,R0		;COPCAR DEBUG
    // asm 0000A433: 	STI	R0,*+AR7(SIGMA_MODEL)
    // asm 0000A434: 	MPYI	2,R0
    // asm 0000A435: 	LDI	@SIGMA_LISTI,AR2
    // asm 0000A436: 	ADDI	R0,AR2
    // asm 0000A437: 	LDI	*+AR2(SS_FLAG),R0
    // asm 0000A438: 	STI	R0,*+AR7(SIGMA_FLAG)
    // ;	CMPI	SS_COPCAR,R0
    // ;	BEQ	COPCAR_DRONE
    // asm 0000A439: 	LDI	*AR2,AR2
    // asm 0000A43A: 	LDI	@NOLONG_VEHICLES,R1
    // asm 0000A43B: 	BZ	DONTWORRY
    // asm 0000A43C: 	TSTB	SS_LONG,R0
    // asm 0000A43D: 	BZ	DONTWORRY
    // asm 0000A43E: 	LDI	SIGT_GTRUCK,R0
    // asm 0000A43F: 	BU	WORIT
DONTWORRY:
    // asm 0000A440: 	STI	AR2,*+AR7(DELTA_MODEL)
    // asm 0000A441: 	MPYI	VEHTAB_SIZE,AR2
    // asm 0000A442: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 0000A443: 	LDI	*+AR2(VEHTAB_MODEL),AR2
    // asm 0000A444: 	STI	R4,*+AR7(DELTA_INIT)
    // asm 0000A445: 	CALL	OBJ_GETE
    // asm 0000A446: 	BC	SUICIDE			;abort process if no object available
    // asm 0000A447: 	LDI	AR0,AR4
    // asm 0000A448: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm 0000A449: 	CALL	VEHICLE_ANI_INIT
    // asm 0000A44A: 	CALL	DELTA_OINIT
    // asm 0000A44B: 	LDI	DRONE_C|VEHICLE_T|DRNE_SIGMA,R0
    // asm 0000A44C: 	STI	R0,*+AR5(CAR_ID)
    // asm 0000A44D: 	STI	R0,*+AR4(OID)
    // asm 0000A44E: 	STI	R0,*+AR7(PID)
    // asm 0000A44F: 	CALL	SET_DRONE_PAL
    // asm 0000A450: 	RANDN	2
    // asm 0000A452: 	ADDI	2,R0
    // asm 0000A453: 	STI	R0,*+AR7(DELTA_STATUS)
    // 	;init position at two pieces before end of universe
    // 	;
    // asm 0000A454: 	LDI	@DYNALIST_END,AR2
    // asm 0000A455: 	LDI	*+AR2(OBLINK4),AR2
    // asm 0000A456: 	LDI	*+AR2(OBLINK4),AR2
    // asm 0000A457: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm 0000A458: 	LDI	*+AR2(OUSR1),R0
    // asm 0000A459: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm 0000A45A: 	CALL	SUB_FUNCTION			;MATRIXA,VECTORA,R2
    // asm 0000A45B: 	LDP	@_VECTORA
    // asm 0000A45C: 	LDF	*+AR2(OPOSX),R0
    // asm 0000A45D: 	ADDF	@_VECTORA+X,R0
    // asm 0000A45E: 	STF	R0,*+AR4(OPOSX)
    // asm 0000A45F: 	LDF	*+AR2(OPOSY),R0
    // asm 0000A460: 	SUBF	*+AR5(CARWHLTAB+1),R0
    // asm 0000A461: 	ADDF	@_VECTORA+Y,R0
    // asm 0000A462: 	STF	R0,*+AR4(OPOSY)
    // asm 0000A463: 	LDF	*+AR2(OPOSZ),R0
    // asm 0000A464: 	ADDF	@_VECTORA+Z,R0
    // asm 0000A465: 	STF	R0,*+AR4(OPOSZ)
    // asm 0000A466: 	SETDP
    // 	;initialize Ytheta to the intentional direction
    // asm 0000A467: 	STF	R2,*+AR4(ORADY)
    // asm 0000A468: 	STF	R2,*+AR7(DELTA_RADYDELTA)
    // asm 0000A469: 	STF	R2,*+AR5(CARYROT)
    // asm 0000A46A: 	STF	R2,*+AR5(CARVROT)
    // asm 0000A46B: 	LDI	AR4,AR2
    // asm 0000A46C: 	ADDI	OMATRIX,AR2
    // asm 0000A46D: 	CALL	FIND_YMATRIX
    // asm 0000A46E: 	CLRI	R0
    // asm 0000A46F: 	STI	R0,*+AR7(SIGMA_ONCE)
    // asm 0000A470: 	STI	R0,*+AR7(SIGMA_YELL)
    // asm 0000A471: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // 	;Weaving SIGMA????
    // asm 0000A472: 	RANDN	10	;1 in 10 chance
    // asm 0000A474: 	CMPI	0,R0
    // asm 0000A475: 	BNE	NOTWEAVER
    // asm 0000A476: 	LDI	SIGMA_PSYCHO,R0
    // asm 0000A477: 	STI	R0,*+AR7(DELTA_PLAYIT)
    // asm 0000A478: 	RANDN	50
    // asm 0000A47A: 	ADDI	20,R0
    // asm 0000A47B: 	STI	R0,*+AR7(DELTA_PSTAT)
NOTWEAVER:
    // *
    // *
    // *
SIGMA_LP:
    // asm 0000A47C: 	LDI	@SUSPEND_MODE,R0
    // asm 0000A47D: 	CMPI	SM_HALT,R0
    // asm 0000A47E: 	BEQ	SIGMASLP
    // asm 0000A47F: 	CALL	AHEAD_OF_PLAYER_P
    // asm 0000A480: 	LDIC	1,R0
    // asm 0000A481: 	LDINC	0,R0
    // asm 0000A482: 	STI	R0,*+AR7(DELTA_PSTAT)
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
    // asm 0000A483: 	LDI	*+AR7(DELTA_PLAYIT),R0
    // asm 0000A484: 	CMPI	SIGMA_PSYCHO,R0
    // asm 0000A485: 	BNE	NOTPSYCHO_LP
    // asm 0000A486: 	LDI	*+AR7(DELTA_PSTAT),R0
    // asm 0000A487: 	DEC	R0
    // asm 0000A488: 	STI	R0,*+AR7(DELTA_PSTAT)
    // asm 0000A489: 	CMPI	0,R0
    // asm 0000A48A: 	BGT	NOTPSYCHO_LP
    // asm 0000A48B: 	LDI	*+AR7(DELTA_STATUS),R0
    // asm 0000A48C: 	CMPI	2,R0
    // asm 0000A48D: 	LDIEQ	3,R0
    // asm 0000A48E: 	LDINE	2,R0
    // asm 0000A48F: 	STI	R0,*+AR7(DELTA_STATUS)
    // asm 0000A490: 	RANDN	50
    // asm 0000A492: 	ADDI	30,R0
    // asm 0000A493: 	STI	R0,*+AR7(DELTA_PSTAT)
NOTPSYCHO_LP:
    // 	;if all 4 wheels are off then effective breakdown
    // asm 0000A494: 	LDI	*+AR5(RF_PCOL),AR0
    // asm 0000A495: 	LDI	*+AR0(OID),R0
    // asm 0000A496: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm 0000A498: 	LDI	*+AR5(LF_PCOL),AR0
    // asm 0000A499: 	LDI	*+AR0(OID),R0
    // asm 0000A49A: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm 0000A49C: 	LDI	*+AR5(RR_PCOL),AR0
    // asm 0000A49D: 	LDI	*+AR0(OID),R0
    // asm 0000A49E: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm 0000A4A0: 	LDI	*+AR5(LR_PCOL),AR0
    // asm 0000A4A1: 	LDI	*+AR0(OID),R0
    // asm 0000A4A2: 	IFI	R0,EQ,300h,NOSL2DIE
    // asm 0000A4A4: 	LDI	*+AR7(SIGMA_ONCE),R0
    // asm 0000A4A5: 	BZ	NOSL2DIE2
    // asm 0000A4A6: 	BU	BREAKDOWN
    // asm 0000A4A7: NOSL2DIE
    // asm 0000A4A7: 	LDI	1,R0
    // asm 0000A4A8: 	STI	R0,*+AR7(SIGMA_ONCE)
NOSL2DIE2:
    // asm 0000A4A9: 	LDI	*+AR5(CAR_BUMP),R0
    // asm 0000A4AA: 	BZ	NOBUMP
    // asm 0000A4AB: 	CLRI	R0
    // asm 0000A4AC: 	STI	R0,*+AR5(CAR_BUMP)
    // ;	INCM	@CAR_COLLS
    // asm 0000A4AD: 	CREATEC	EXP_PUFF,SPAWNER_C
NOBUMP:
    // 	;it the plyr is zooming by
    // 	;
    // asm 0000A4B0: 	LDI	*+AR7(SIGMA_YELL),R0
    // asm 0000A4B1: 	BNZ	NOYELL
    // asm 0000A4B2: 	LDI	*+AR7(DELTA_PSTAT),R0		;IN FRONT OF PLAYER?
    // asm 0000A4B3: 	BZ	NOYELL
    // asm 0000A4B4: 	CALL	DIST_TO_PLYR
    // asm 0000A4B5: 	LDF	*+AR7(DELTA_PLYRDIST),R1
    // asm 0000A4B6: 	STF	R1,*+AR7(DELTA_OPLYRDIST)
    // asm 0000A4B7: 	STF	R0,*+AR7(DELTA_PLYRDIST)
    // asm 0000A4B8: 	FLOAT	5000,R2
    // asm 0000A4B9: 	CMPF	R2,R0		;<5000
    // asm 0000A4BA: 	BGT	NOYELL
    // asm 0000A4BB: 	CMPF	R0,R1
    // asm 0000A4BC: 	BLT	NOYELL
    // asm 0000A4BD: 	LDI	@PLYCBLK,AR0
    // asm 0000A4BE: 	LDF	*+AR0(CARSPEED),R0
    // asm 0000A4BF: 	CMPF	127,R0
    // asm 0000A4C0: 	BLT	NOYELL
    // asm 0000A4C1: 	LDI	1,R0
    // asm 0000A4C2: 	STI	R0,*+AR7(SIGMA_YELL)
    // asm 0000A4C3: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm 0000A4C4: 	MPYI	VEHTAB_SIZE,AR2
    // asm 0000A4C5: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 0000A4C6: 	LDI	*+AR2(VEHTAB_PASSBY),AR2
    // asm 0000A4C7: 	CMPI	0,AR2
    // asm 0000A4C8: 	BEQ	NOYELL
    // asm 0000A4C9: 	CALL	ONESNDFX
NOYELL:
    // asm 0000A4CA: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000A4CB: 	LDI	*+AR2(OLINK4),AR0
    // asm 0000A4CC: 	CMPI	0,AR0
    // asm 0000A4CD: 	BEQ	SIGMASLP		;IF AT END OF WORLD DONT MOVE!
    // 	;find piece which points to piece if it is not found
    // 	;or it is the initial piece exit rho code and commit
    // 	;suicide
    // asm 0000A4CE: 	LDI	*+AR7(DELTA_TPIECE),R0
    // asm 0000A4CF: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm 0000A4D0: 	CMPI	AR0,R0			;BR-> WE ARE ATTACKING THE START OF UNIVERSE
    // asm 0000A4D1: 	BEQ	SIGMA_DIE
    // asm 0000A4D2: 	LDI	*+AR2(OUSR1),R0
    // asm 0000A4D3: 	LDI	*+AR7(DELTA_LAST_OID),R1
    // asm 0000A4D4: 	CMPI	R0,R1			;BR-> WE ARE UNDER THE START OF UNIVERSE
    // asm 0000A4D5: 	BLT	SIGMA_DIE
    // 	;
    // 	;simply drive slowly forward until we are below the section list
    // 	;
    // 	;
    // 	;see if we should track the next piece
CHECK_DIST:
    // asm 0000A4D6: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm 0000A4D7: 	RS	8,R0
    // asm 0000A4D8: 	LDI	@SECTIONIDX,R1
    // asm 0000A4D9: 	SUBPI	@DGROUP_COUNT,R1
    // asm 0000A4DA: 	CMPI	R1,R0
    // asm 0000A4DB: 	BLE	SIGMA_DIE
    // asm 0000A4DC: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000A4DD: 	LDI	*+AR2(OLINK4),R0
    // asm 0000A4DE: 	BZ	SIGMASLP
    // asm 0000A4DF: 	CALL	GET_TRACK_POS			;CHECK IF WE SHOULD ADVANCE
    // asm 0000A4E0: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm 0000A4E1: 	CMPF	R1,R0
    // asm 0000A4E2: 	BGT	THIS_PIECE
    // asm 0000A4E3: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000A4E4: 	LDI	*+AR2(OLINK4),R0
#if DEBUG
    // asm: 	BZ	$				;HOW DID WE MISS THIS?
#endif
    // asm 0000A4E5: 	STI	R0,*+AR7(DELTA_TPIECE)
    // asm 0000A4E6: 	LDI	R0,AR0
    // asm 0000A4E7: 	LDI	*+AR0(OUSR1),R0
#if DEBUG
    // asm: 	BLT	$
#endif
    // asm 0000A4E8: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm 0000A4E9: 	BU	CHECK_DIST
THIS_PIECE:
    // asm 0000A4EA: 	LDF	*+AR5(CARSPEED),R1
    // asm 0000A4EB: 	LDFLE	30,R1			;if 0 or less assume 30 mph
    // asm 0000A4EC: 	FLOATP	@NFRAMES,R2
    // asm 0000A4ED: 	MPYF	R2,R1
    // asm 0000A4EE: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm 0000A4EF: 	FIX	R0,R7
    // asm 0000A4F0: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000A4F1: 	LDP	@_VECTORA		;lane position
    // asm 0000A4F2: 	LDF	*+AR2(OPOSX),R2		;X
    // asm 0000A4F3: 	SUBF	*+AR4(OPOSX),R2
    // asm 0000A4F4: 	ADDF	@_VECTORA+X,R2
    // asm 0000A4F5: 	LDF	*+AR2(OPOSZ),R3		;Z
    // asm 0000A4F6: 	SUBF	*+AR4(OPOSZ),R3
    // asm 0000A4F7: 	ADDF	@_VECTORA+Z,R3
    // asm 0000A4F8: 	SETDP
    // asm 0000A4F9: JOINUP998
    // 	;find the theta delta to this position
    // 	;
    // asm 0000A4F9: 	CALL	ARCTANF			;-> R0
    // asm 0000A4FA: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm 0000A4FB:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm 0000A4FC: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm 0000A4FD: 	FLOAT	R7,R1			;theta / number of turns to achieve
    // asm 0000A4FE: 	SUBF	1,R1	;DBG
    // asm 0000A4FF: 	BZ	NODIV
    // asm 0000A500: 	CALL	DIV_F			;-> R0
NODIV:
    // asm 0000A501: STF	R0,*+AR7(DELTA_RADYDELTA)
    // asm 0000A502: 	CALL	PRECOLLIDE_PLYR
    // asm 0000A503: 	BNC	NOTPRECOL
    // asm 0000A504: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm 0000A505: 	MPYF	0.01,R2
    // asm 0000A506: 	STF	R2,*+AR5(CARTHROTTLE)
    // asm 0000A507: 	BU	L99
NOTPRECOL:
    // 	;set throttle
    // asm 0000A508: 	LDF	*+AR7(DELTA_THROTTLE),R2
    // asm 0000A509: 	MPYF	1.01,R2
    // asm 0000A50A: 	CMPF	MIN_THROTTLE,R2
    // asm 0000A50B: 	LDFLT	MIN_THROTTLE,R2
    // asm 0000A50C: 	CMPF	MAX_SIGMA_THROTTLE,R2
    // asm 0000A50D: 	LDFGT	MAX_SIGMA_THROTTLE,R2
    // asm 0000A50E: 	STF	R2,*+AR7(DELTA_THROTTLE)
    // asm 0000A50F: 	STF	R2,*+AR5(CARTHROTTLE)
L99:
    // asm 0000A510: 	LDF	*+AR7(DELTA_RADYDELTA),R2
    // asm 0000A511: 	MPYF	1.95,R2			;depending on plyr.asm this may have to
    // asm 0000A512: 	CALL	DRONE_RIDE_RIGHT	;FIND DISTANCE TO CENTER OF ROAD
    // asm 0000A513: 	STF	R0,*+AR5(CARDIST2CNTR)
    // asm 0000A514: 	CALL	DRONEGO
    // asm 0000A515: 	CALL	GETTRAK
    // ;	CALL	PTS
SIGMASLP:
    // asm 0000A516: 	SLEEP	1
    // asm 0000A518: 	B	SIGMA_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIGMA_DRONE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
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
    // asm 0000A519: 	CREATEC	SMOKE_PUFF,2
    // asm 0000A51C: 	LDI	10,AR6
BREAKDOWNLP:
    // asm 0000A51D: 	LDI	@SUSPEND_MODE,R0
    // asm 0000A51E: 	CMPI	SM_HALT,R0
    // asm 0000A51F: 	BEQ	BREAKDNSLP
    // asm 0000A520: 	DEC	AR6
    // asm 0000A521: 	CMPI	0,AR6
    // asm 0000A522: 	BLT	NOSMK
    // asm 0000A523: 	CREATEC	SMOKE_PUFF,2
NOSMK:
    // asm 0000A526: 	LDI	*+AR5(CARTRAK),AR0
    // asm 0000A527: 	LDI	*+AR0(OUSR1),R0
    // asm 0000A528: 	RS	8,R0
    // asm 0000A529: 	LDI	@(DGROUPS+DGRP_IDX),R1
    // asm 0000A52A: 	CMPI	R1,R0
    // asm 0000A52B: 	BLT	SIGMA_DIE
    // asm 0000A52C: 	CLRF	R2
    // asm 0000A52D: 	STF	R2,*+AR5(CARTHROTTLE)
    // asm 0000A52E: 	CALL	DRONEGO
    // asm 0000A52F: 	CALL	GETTRAK
BREAKDNSLP:
    // asm 0000A530: 	SLEEP	1
    // asm 0000A532: 	BU	BREAKDOWNLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BREAKDOWN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SIGMA_DIE(void)
{
    // asm 0000A533: 	BU	RHO_DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIGMA_DIE", 0, 0);
    UNIMPL();
}

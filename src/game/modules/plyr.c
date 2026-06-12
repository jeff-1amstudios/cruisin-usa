#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/obj.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/dirq.h"
#include "../include/delta.h"
#include "../include/comm.h"
#include "../include/plyr.h"

/*
 * Source module: asm/PLYR.ASM
 */

// *----------------------------------------------------------------------------
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
#define JARVK 0
// *THESE ARE EDGE VARIABLES AND SHOULD NOT BE MESSED WITH
/* asm: OFFROAD_TMR	.bss	OFFROAD_TMR,1 */
int OFFROAD_TMR;
/* asm: ZOOMRAM	.bss	ZOOMRAM,0 */
int ZOOMRAM;
/* asm: ZOOMD	.bss	ZOOMD,1 */
int ZOOMD;
/* asm: ZOOMDD	.bss	ZOOMDD,1 */
int ZOOMDD;
/* asm: ZOOMDG	.bss	ZOOMDG,1 */
int ZOOMDG;
/* asm: ZOOMH	.bss	ZOOMH,1 */
int ZOOMH;
/* asm: ZOOMHD	.bss	ZOOMHD,1 */
int ZOOMHD;
/* asm: ZOOMHG	.bss	ZOOMHG,1 */
int ZOOMHG;
/* asm: CAMVIEW	.bss	CAMVIEW,1 */
int CAMVIEW;
/* asm: BRAKEON	.bss	BRAKEON,1 */
int BRAKEON;
/* asm: WRECKFLG	.bss	WRECKFLG,1 */
int WRECKFLG;
/* asm: REVFLG	.bss	REVFLG,1 */
int REVFLG;
/* asm: _plyr1	.bss	_plyr1,0 */
int _plyr1;
/* asm: PLYSTAT	.bss	PLYSTAT,1 */
int PLYSTAT;
/* asm: PLYCAR	.bss	PLYCAR,1 */
int PLYCAR;
/* asm: PLYPROC	.bss	PLYPROC,1 */
int PLYPROC;
/* asm: PLYCBLK	.bss	PLYCBLK,1 */
int PLYCBLK;
/* asm: OLDPLYSPD	.bss	OLDPLYSPD,1 */
int OLDPLYSPD;
/* asm: OLDPLYAIR	.bss	OLDPLYAIR,1 */
int OLDPLYAIR;
/* asm: PLYRFIRST	.bss	PLYRFIRST,1 */
int PLYRFIRST;
/* asm: PLAIRTIM	.bss	PLAIRTIM,1 */
int PLAIRTIM;
/* asm: CHEATACC	.bss	CHEATACC,1 */
int CHEATACC;
/* asm: CHEAT	.bss	CHEAT,1 */
int CHEAT;
// *PLAYER 1ST, 2ND, 3RD POSTION COORDS:
#define PLYPOS1Z 0
#define PLYPOS1Y (-150)
#if JARVK
#define PLYPOS2Z 3840
#define PLYPOS2Y (-700)
#define PLYPOS3Z 6000
#define PLYPOS3Y (-1000)
#else
#undef PLYPOS2Z
#define PLYPOS2Z 2200
#undef PLYPOS2Y
#define PLYPOS2Y (-400)
#undef PLYPOS3Z
#define PLYPOS3Z 3840
#undef PLYPOS3Y
#define PLYPOS3Y (-700)
#endif
// ;PLYPOS3Y	.set	-2700	;good heli height
#define ZOOMRATIO 0.05 //1/ZOOMTIME FOR VIEW CHANGE
// *SWITCH BIT VALUES
#define SHIFT 4 //LO-HI SHIFT LEVER
#define BRAKE 1 //BRAKE PEDAL
// *RAM VARIABLES
/* asm: PLMSAV	.bss	PLMSAV,15 */
int PLMSAV[15];
/* asm: PMSAV	.bss	PMSAV,9 */
int PMSAV[9];
/* asm: ZOOMI	.word	ZOOMRAM */
#define ZOOMI ZOOMRAM
#define GRAVITY 1.20
// *RPM MAX
#define NUM_RPMS 47
#define NUM_RPM 47.0
#define OVERREV 51.0
// *----------------------------------------------------------------------------
// *PLAYER CAR SPECIALIZED PARAMETER TABLE
// *
// *ACCELERATION, TRACTION (0=total traction), ONROAD DAMPING, OFFROAD DAMPING
// *
// *STDARD .float	0.82,1.00,0.0028,0.010
// *NEWSTD	.float	0.82,0.90,0.0028,0.0060
/* asm: CARPARAMTAB: */
/* asm: 	*#0 MUSCLE CAR */
/* asm: 	.float	0.91,0.60,0.0028,0.010		;ALL AROUND */
int CARPARAMTAB[] = {
    0.91, 0.60, 0.0028, 0.010, // ALL AROUND
};
/* asm: CARPARAMTAB1 */
/* asm: 	*#1 XXX */
/* asm: 	.float	0.98,0.50,0.0032,0.0042		;ACCEL */
/* asm: 	*#2 MISSILE */
/* asm: 	.float	0.88,0.70,0.0026,0.010	   	;TOP SPEED */
/* asm: 	*#3 FERRARI */
/* asm: 	.float	0.89,0.50,0.0028,0.010		;HANDLING */
/* asm: 	*HIDDEN VEHICLES */
/* asm: 	*#4 jeep */
/* asm: 	.float	0.95,0.60,0.0030,0.0039 */
/* asm: 	*#5 sbusp */
/* asm: 	.float	0.89,0.50,0.0028,0.010 */
/* asm: 	*#6 copcar */
/* asm: 	.float	0.91,0.65,0.0028,0.0050 */
/* asm: 	*#7 gtruck */
/* asm: 	.float	0.89,0.50,0.0028,0.010 */
int CARPARAMTAB1[] = {
    0.98, 0.50, 0.0032, 0.0042, // ACCEL
    0.88, 0.70, 0.0026, 0.010, // TOP SPEED
    0.89, 0.50, 0.0028, 0.010, // HANDLING
    0.95, 0.60, 0.0030, 0.0039,
    0.89, 0.50, 0.0028, 0.010,
    0.91, 0.65, 0.0028, 0.0050,
    0.89, 0.50, 0.0028, 0.010,
};
#define CARPARAMTABL (CARPARAMTAB1-CARPARAMTAB) //LENGTH OF ENTRY
// *----------------------------------------------------------------------------
// *GET CAR PARAMETERS FOR PLAYER
// *PARAMETERS
// *	R0	CAR # 0-3
// *	AR0	CAR BLOCK INDEX
// *LOADS PARAMETERS INTO CAR BLOCK
// *TRASHES R0,AR2
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *PLYR_CAR_INIT
// *PARAMETERS
// *	AR4	CAR (NOT INSERTED)
// *RETURNS
// *	AR4	CAR (INSERTED)
// *	AR5	CAR BLOCK
#define LANESIZE 1152
// 	;if a slave then ALWAYS appear on right side
// 	;
// 	;
// *SET CAMERA POSITION
// *CAMERA INIT
// ;	LDF	1.0,R0			;INIT DRAFT VALUE
// ;	STF	R0,@PLDRAFTVAL
/* asm: VIEW0I	.word	_VIEW0 */
#define VIEW0I _VIEW0
/* asm: VIEW1I	.word	_VIEW1 */
#define VIEW1I _VIEW1
/* asm: VIEW2I	.word	_VIEW2 */
#define VIEW2I _VIEW2
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *MIDWAY CHEAT IS HERE!!!!!!!!!!!!
// *MAY NEED MAX CHEAT WITH DISTANCE IN FUTURE
// *LIMIT ADVANTAGE AT CLOSE DISTANCE
// *JARV CHANGE  February 7,1995
// ;	ADDF	0.10,R0		;MAXIMUM VALUE
// *JARV END CHANGE
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *GEAR RATIO TABLE
/* asm: ENGVOL	.BSS	ENGVOL,1 */
int ENGVOL;
// *----------------------------------------------------------------------------
// *ENGINE ACCEL MULTIPLIER TABLE
// *
/* asm: GEARACTABI	.word	GEARACTAB */
#define GEARACTABI GEARACTAB
/* asm: GEARACTAB */
/* asm: 	.float	0.0,1.7,1.5,1.4,1.2  		;POWER FACTOR GEAR(0-4) */
int GEARACTAB[] = {
    0.0, 1.7, 1.5, 1.4, 1.2, // POWER FACTOR GEAR(0-4)
};
/* asm: ENGACTABI	.word	ENGACTAB */
#define ENGACTABI ENGACTAB
/* asm: ENGACTAB */
/* asm: 	.float	1.20,1.20,0.50,0.60,0.70	;0000,0300,0600,0900,1200 */
/* asm: 	.float	0.80,0.90,1.00,1.00,1.00	;1500,1800,2100,2400,2700 */
/* asm: 	.float	1.00,1.00,1.00,1.00,0.90	;3000,3300,3600,3900,4200 */
/* asm: 	.float	0.80,0.40,0.20,0.00,0.00	;4500,4800,5100,5400,5700 */
/* asm: 	*ENGINE FRICTION */
int ENGACTAB[] = {
    1.20, 1.20, 0.50, 0.60, 0.70, // 0000,0300,0600,0900,1200
    0.80, 0.90, 1.00, 1.00, 1.00, // 1500,1800,2100,2400,2700
    1.00, 1.00, 1.00, 1.00, 0.90, // 3000,3300,3600,3900,4200
    0.80, 0.40, 0.20, 0.00, 0.00, // 4500,4800,5100,5400,5700
};
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *CHECK IF DRIVING BACKWARDS
// *PARAMETERS
// *	AR4	CAR
// *	AR5	CAR STRUCTURE
// *----------------------------------------------------------------------------
/* asm: WHLTIM	.bss	WHLTIM,1 */
int WHLTIM;
/* asm: WHLOLD	.bss	WHLOLD,1 */
int WHLOLD;
// *----------------------------------------------------------------------------
// *SOUND TABLES
// *PLAYER COLLISION SOUND TABLE
/* asm: SCOLLTAB	.word	SCOLLA,SCOLLB,SCOLLC */
int SCOLLTAB[] = {
    SCOLLA, SCOLLB, SCOLLC,
};
// *WALL HIT SOUND TABLE
/* asm: WALLHITAB	.word	WALLHITA,WALLHITB,WALLHITC */
int WALLHITAB[] = {
    WALLHITA, WALLHITB, WALLHITC,
};
// *SKID SOUND TABLE
/* asm: SKIDTAB	.word	SKIDB,SKIDC */
int SKIDTAB[] = {
    SKIDB, SKIDC,
};
/* asm: PLAIRSND	.word 	RH_BABEWHOA,GL_WOOLAUGH,CHICKSCREAM */
int PLAIRSND[] = {
    RH_BABEWHOA, GL_WOOLAUGH, CHICKSCREAM,
};
/* asm: REVSNDTAB	.word	SINGLEREV5,SINGLEREV6 */
int REVSNDTAB[] = {
    SINGLEREV5, SINGLEREV6,
};
// *----------------------------------------------------------------------------
// *STEERING WHEEL AND GAS PEDAL PARAMETERS
// *(VALUES ARE READ FROM CMOS AND COPIED INTO RAM FOR EASY ACCESS)
// *(COPIED AT THE START OF EACH GAME)
// *ALL FLOATS
/* asm: PEDALMN	.bss	PEDALMN,1 */
int PEDALMN;
/* asm: PEDALMX	.bss	PEDALMX,1 */
int PEDALMX;
/* asm: STEERMN	.bss	STEERMN,1 */
int STEERMN;
/* asm: STEERMX	.bss	STEERMX,1 */
int STEERMX;
/* asm: STEERCT	.bss	STEERCT,1 */
int STEERCT;
/* asm: BRAKEMN	.bss	BRAKEMN,1 */
int BRAKEMN;
/* asm: BRAKEMX	.bss	BRAKEMX,1 */
int BRAKEMX;
/* asm: STEERFR	.bss	STEERFR,1 */
int STEERFR;
#define ADJ_COINMODE 0
#define ADJ_GASMIN 1
#define ADJ_GASMAX 2
#define ADJ_STEERMIN 3
#define ADJ_STEERMAX 4
#define ADJ_STEERCENTER 5
#define ADJ_BRAKEMIN 6
#define ADJ_BRAKEMAX 7
// *----------------------------------------------------------------------------

void GETCARPARAM(void)
{
    // asm: 	LDI	@CARPARAMTABI,AR2
    // asm: 	MPYI	CARPARAMTABL,R0
    // asm: 	ADDI	R0,AR2
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(CARMAXACCEL)
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(CARTRACTION)
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(CARRDFR)
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(CAROFRDFR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCARPARAM", 0, 0);
    UNIMPL();
}

void _CARV0(void)
{
    // *----------------------------------------------------------------------------
    // *INIT CAR 3 POINT SUSPENSION DATA STRUCTURE
    // *PARAMETERS
    // *	AR4	OBJECT BLOCK
    // *	R0	VEHICLE #
    // *RETURNS	AR0	POINTS TO CARBLOCK
    // *
    // *	NO CARRY ON FAILURE TO ALLOCATE CAR BLOCK
    // *
    // *TRASHED	R1-R7
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR1
    // asm: 	CALL	GETCAR
    // asm: 	BNC	CARV_ERR
    // asm: 	STI	AR0,*+AR4(OCARBLK)
    // asm: 	LDF	0,R0
    // asm: 	RPTS	CARSIZ-1
    // asm: 	STF  	R0,*AR0++		;CLEAR OUT THE BLOCK
    // asm: 	SUBI	CARSIZ,AR0	 	;RESTORE AR2
    // asm: 	CALL	_makbox			;GET YOUR CAR BOX
    // asm: 	LDI	*+AR4(OROMDATA),AR1	;TWEAK RADIUS SLIGHTLY
    // asm: 	FLOAT	*AR1,R0
    // asm: 	MPYF	1.1,R0
    // asm: 	FIX	R0
    // asm: 	STI	R0,*+AR4(ORAD)		;GET OBJECT RADIUS
    // asm: 	LDI	0,R0 			;INIT FLAGS
    // asm: 	STI	R0,*+AR0(CAR_SPIN)
    // asm: 	STI	R0,*+AR0(CAR_ONROAD)
    // asm: 	STI	R0,*+AR0(CAR_AIRF)
    // asm: 	STI	R0,*+AR0(CAR_AIRB)
    // asm: 	STI	R0,*+AR0(CARPTSTR)	;POSITION TRACKING SYSTEM
    // asm: 	STI	R0,*+AR0(CARPTSRANK)
    // asm: 	STI	R0,*+AR0(CAR_BUMP)
    // asm: 	STI	R0,*+AR0(CARGEAR)
    // asm: 	STI	R0,*+AR0(CARTRANS)  	;AUTO IS DEFAULT
    // asm: 	STI	R0,*+AR0(CARNUM)  	;CLEAR DISPATCH NUMBER
    // asm: 	STI	R0,*+AR0(CARTRACK_ID)	;CLEAR TRACK ID
    // asm: 	STI	R0,*+AR0(CAR_OM)	;CLEAR OTHER MACHINE FLAG
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR0(CARRPM)
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR0(CARSHAD)	;SHADOW ON
    // asm: 	LDF	@ROADFRICI,R0		;FRICTION COEFFICIENTS
    // asm: 	STF	R0,*+AR0(CARRDFR)
    // asm: 	LDF	@OFRDFRICI,R0
    // asm: 	STF	R0,*+AR0(CAROFRDFR)
    // asm: 	LDF	0.82,R0
    // asm: 	STF	R0,*+AR0(CARMAXACCEL)	;SET ACCEL POWER
    // asm: 	LDF	1.0,R0
    // asm: 	STF	R0,*+AR0(CARMASS)	;DEFAULT CAR MASS
    // asm: 	STF	R0,*+AR0(CARTRACTION)	;DEFAULT TRACTION COEFFICIENT
    // asm: 	LDI	1,R0
    // asm: 	LS	O_3DROT_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0	;FLAG SYSTEM AS NON-2D OPTIMIZABLE
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	SETC
CARV_ERR:
    // asm: 	POP	AR1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_CARV0", 0, 0);
    UNIMPL();
}

void BONUS_WAIT_LOOP(void)
{
    // asm: 	LDI	@DID_TIMED_OUT,R0
    // asm: 	BNZ	BWLX
    // asm: 	LDI	@PLYCAR,AR2
    // asm: 	LDI	*+AR2(OFLAGS),R0	;CHECK IF ALREADY ON LIST
    // asm: 	TSTB	O_LIST_M,R0
    // asm: 	BNZ	BWLX			;YES, DONT INSERT
    // asm: 	CALL	OBJ_INSERT		;INSERT PLAYER OBJECT
BWLX:
BONUS_WAIT_LP:
    // asm: 	LDI	@PLYCAR,AR4
    // asm: 	LDI	@PLYCBLK,AR5
    // asm: 	CALL	ZOOMUP			;UPDATE YOUR ZOOM
    // asm: 	SLEEP	1
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	BONUS_WAIT_LP
    // asm: 	BU	PLYR_ENTER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS_WAIT_LOOP", 0, 0);
    UNIMPL();
}

void PLYR_CAR_INIT(void)
{
    // asm: 	STI	AR4,@PLYCAR		;INIT CAR PLAYER STRUCT
    // asm: 	STI	AR7,@PLYPROC
    // asm: 	LDI	@CHOOSENCAR,AR1
    // asm: 	CMPI	4,AR1
    // asm: 	BLT	DOGENRLB
    // asm: 	LDI	AR1,R0
    // asm: 	CMPI	4,AR1
    // asm: 	LDIEQ	JEEP_MOD,R0
    // asm: 	CMPI	5,AR1
    // asm: 	LDIEQ	PLYR_SBUS_MOD,R0
    // asm: 	CMPI	6,AR1
    // asm: 	LDIEQ	PLYR_COPCAR_MOD,R0
    // asm: 	CMPI	7,AR1
    // asm: 	LDIEQ	3,R0
    // asm: 	LDI	R0,AR1
DOGENRLB:
    // asm: 	LDI	AR1,AR2
    // asm: 	MPYI	VEHTAB_SIZE,AR1
    // asm: 	ADDI	@VEHICLE_TABLEI,AR1
    // asm: 	LDI	*+AR1(VEHTAB_MODEL),R1
    // asm: 	STI	R1,*+AR4(OROMDATA)
    // asm: 	CALL	VEHICLE_ANI_INIT	;SETUP WHEEL ANIMATION
    // asm: 	LDF	-1,R0
    // asm: 	STF	R0,*+AR4(OUSR1)		;skid system flag
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@_MPH
    // asm: 	STI	R0,@OLDPLYAIR		;OLD AIRBORNE FLAG
    // asm: 	STI	R0,@PLAIRTIM		;CLEAR AIR TIMER
    // asm: 	STI	R0,@ENGVOL		;ENGINE SOUND VOLUME
    // asm: 	STI	AR7,*+AR4(OPLINK)	;SETUP OPLINK
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT	 	;INSERT SUCKER ON THE LIST
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,@OLDPLYSPD
    // asm: 	LDI	@CHOOSENCAR,R0
    // asm: 	CALL	_CARV0			;INIT CAR DATA STRUCT IN PROCESS
    // asm: 	CALL	GETCARPARAM		;GET SPECIAL PLAYER CAR PARAMETERS
    // asm: 	STI	AR0,@PLYCBLK
    // asm: 	LDI	PLYR_C,R0
    // asm: 	STI	R0,*+AR0(CAR_ID)
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm: 	STI	R0,*+AR0(CARTRANS)	;AUTO/MANUAL SWITCH
    // asm: 	LDF	2.0,R0
    // asm: 	STF	R0,*+AR0(CARMASS)	;SET CAR MASS
    // asm: 	LDI	11,R0
    // asm: 	STPI	R0,@OFFROAD_TMR
    // asm: 	LDF	@STEERCT,R0		;STEERING CENTER
    // asm: 	STF	R0,@WHEELPOS
    // asm: 	RETS	;DONE INITIALIZING PLYR CAR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_CAR_INIT", 0, 0);
    UNIMPL();
}

void PLYR_INTRO_ENTER(void)
{
    // *----------------------------------------------------------------------------
    // *PARAMETERS
    // *	AR4	CAR OBJECT
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	LDI	0,R0		 	;NEUTRAL, PLEASE
    // asm: 	STI	R0,*+AR5(CARGEAR)
    // asm: 	CALL	GETTRAK
    // asm: 	LDI	*+AR5(CARTRAK),AR2
    // 	;Set Appropriate Palette
    // 	;
    // asm: 	LDI	@CHOOSENCAR,R0
    // asm: 	LDI	R0,AR0
    // asm: 	CMPI	4,R0
    // asm: 	LDIEQ	JEEP_MOD,AR0
    // asm: 	CMPI	5,R0
    // asm: 	LDIEQ	PTRUCKG_MOD,AR0
    // asm: 	CMPI	6,AR0
    // asm: 	LDIEQ	COPCAR_MOD,AR0
    // asm: 	CMPI	7,AR0
    // asm: 	LDIGE	GTRUCK_MOD,AR0
    // asm: 	MPYI	VEHTAB_SIZE,AR0
    // asm: 	ADDI	@VEHICLE_TABLEI,AR0
    // asm: 	LDI	*+AR0(VEHTAB_PAL),AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR4(OPAL)
    // 	;
    // 	;INIT CAMERA
    // 	;
    // asm: 	LDF	@START_RADY,R2		;SETUP MATRIX
    // asm: 	STF	R2,*+AR5(CARYROT)
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // 	;SET CAMERA POSITION
    // 	;
    // 	;
    // asm: 	LDF	@START_RADY,R2		;SETUP MATRIX
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR2,R2
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	-20*FEET,R0
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	FLOAT	(-20*FEET),R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@CAMERAPOSI,AR3		;INIT CAMERA POSITION
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	STF	R0,*+AR3(X)		;CAMERA X
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR2(Y),R0
    // asm: 	STF	R0,*+AR3(Y)		;CAMERA Y
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR2(Z),R0
    // asm: 	STF	R0,*+AR3(Z)		;CAMERA Z
    // asm: 	CALL	RESCAN	     		;RESET ACTIVE OBJECT LIST
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CAMVIEW		;INIT CAMERA VIEW TO 3RD PERSON
    // asm: 	BU	PLYR_INTRO_JOIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_INTRO_ENTER", 0, 0);
    UNIMPL();
}

void _PLYR(void)
{
    // *----------------------------------------------------------------------------
PLYR_ENTER:
    // asm: 	CALL	OBJ_GET			;INIT PLAYER OBJECT
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	PLYR_CAR_INIT
    // asm: 	LDF	@START_POS+X,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	@START_POS+Y,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	@START_POS+Z,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	PLYR_C|PLYR1_T,R0
    // asm: 	STI	R0,*+AR7(PID)
    // *INITIALIZE PLAYER COORD, FACING ANGLE
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	CALL	GETTRAK
    // asm: 	LDI	*+AR5(CARTRAK),AR2
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR2
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	LDI	*+AR2(OLINK4),AR2	;SKIP FIRST GROUP
    // ;	ANDN	0FFh,R0
    // ;	ADDI	0100h,R0
    // ;L10	LDI	*+AR2(OLINK4),AR2	;SKIP FIRST GROUP
    // ;	CMPI	*+AR2(OUSR1),R0
    // ;	BGT	L10
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR2(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	CALL	GETRDIR			;GET ANGLE OF ROAD
    // *INIT CAMERA
    // asm: 	LDF	R0,R2
    // asm: 	STF	R2,*+AR5(CARYROT)
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	NEGF	R2
    // asm: 	CALL	NORMITS
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	STF	R2,@_CAMERARAD+Y	;UPDATE CAMERA RAD
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // *OFFSET THE CAR INTO LANE 1
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	10*FEET,R0
    // 	;
    // 	;if vehicle is a slave, then offset into lane #2
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_PLYR", 0, 0);
    UNIMPL();
}

void PLYRLP(void)
{
    // *
    // *PLAYER CAR LOOP
    // *
    // asm: LDI	@END_OF_GAMEP,R0
    // asm: 	BNZ	ENDPLAYER
    // asm: 	LDI	@NFRAMES,R2
    // asm: 	NEGI	R2,R1
    // asm: 	LDI    	@POSITION,R0  		;TIMER FOR HOW LONG PLAYER IN 1ST
    // asm: 	CMPI	1,R0
    // asm: 	LDIZ	@PLYRFIRST,R1
    // asm: 	ADDI	R2,R1
    // asm: 	STI	R1,@PLYRFIRST		;TIMER FOR PLAYER IN 1ST PLACE
    // asm: 	LDI	@_countdown,R1		;TIMEOUT?
    // asm: 	CALLLE	TIMED_OUT		;SETBACK?
    // asm: DONT_TIMEOUT
    // *CHECK FOR BONUS SCREEN
    // *	!!!! DO NOT REMOVE THIS CODE
    // *	!!!! EVEN IF YOU DON'T KNOW WHAT
    // *	!!!! IT DOES
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BEQ	BONUS_WAIT_LOOP
    // asm: 	LDI	@PLYCAR,AR4		;GET PLAYER CAR OBJECT
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: PLYRSPD00
    // asm: 	LDI	@WRECKFLG,R0		;WRECK?
    // asm: 	BZ	PLYRSPD
    // asm: 	CALL	WRECK			;DO YOUR WRECK THING
    // asm: 	CALL	INBOUNDZ		;KEEP IN BOUNDS
    // asm: 	B	PLYRCAM			;DONT CHANGE MATRIX OR CAMERA POS
PLYRSPD:
    // asm: 	CALL	CKOFRD			;CHECK YOUR OFFROAD COUNTER
    // asm: 	CMPI	0,R0
    // asm: 	CALLZ	PLYONRD			;RESET PLAYER ON ROAD
    // asm: 	LDF	*+AR4(OMAT11),R0	;IF PLAYER FLIPPED RESET 'EM
    // asm: 	CMPF	0.01,R0
    // asm: 	CALLLT	PLYONRD
    // *GET CAR SPEED
    // asm: 	CALL	GETGEAR			;GET CAR GEAR
    // asm: 	LDI	*+AR5(CARGEAR),R1
    // asm: 	STI	R0,*+AR5(CARGEAR)
    // asm: 	CMPI	R0,R1			;CHECK FOR UPSHIFT
    // asm: 	BGE	PLYRSPD0		;DOWNSHIFT OR NO SHIFT
    // ;	CMPI	4,R0			;4TH GEAR?
    // ;	BZ	PLYRSPD0		;YES, NO RUBBER
    // asm: 	CALL	GETPEDAL		;GET GAS PEDAL VALUE
    // asm: 	MPYF	2,R0			;GIVE A LITTLE JOLT
    // asm: 	CMPF	1.4,R0			;CHECK THROTTLE
    // asm: 	BLE 	PLYRSPD01		;NOT ENOUGH
    // asm: 	PUSHF	R0
    // asm: 	SONDFX	UPSHIFTSND		;MAKE YOUR UPSHIFT DUDES
    // asm: 	CREATEC	FLAME_PRC,UTIL_C	;make child flames
    // asm: 	CREATEC	SMOKE_PROC,UTIL_C	;make child smoke
    // ;	LDI	*+AR5(CARGEAR),AR2	;MAKE RIGHT REV ON SHIFT
    // ;	ADDI	@SHIFTSNDTABI,AR2
    // ;	LDI	*-AR2(1),AR2
    // ;	CALL	ONESND
    // asm: 	POPF	R0
    // asm: 	B	PLYRSPD01
PLYRSPD0:
    // asm: 	CALL	GETPEDAL		;GET GAS PEDAL VALUE
PLYRSPD01:
    // ;	NEGF	*+AR5(CARTHROTTLE),R1
    // ;	ADDF	R0,R1
    // ;	CMPF	0.3,R1
    // ;	BLT	NO_PEDAL_FLAME
    // ;	PUSHF	R0
    // ;	CREATEC	FLAME_PRC,UTIL_C	;make child flames
    // ;	POPF	R0
    // ;NO_PEDAL_FLAME
    // asm: 	LDI	@_MODE,R1
    // asm: 	TSTB	MGO,R1
    // asm: 	LDFZ	0,R0			;YIP, NO THROTTLE
    // asm: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm: 	CALL	GETBRAKE		;GET YOUR BRAKE, DUDES...
    // asm: 	STF	R0,*+AR5(CARBRAKE)   	;STORE IT
    // ;	LDF	@PLDRAFTVAL,R0		;SET DRAFTING VALUE
    // ;	STF	R0,@DRAFTVAL
    // asm: 	CALL	GETSPD
    // asm: 	LDF	*+AR5(CARSPEED),R0 	;OUTPUT IN MPH
    // asm: 	LDI	@_countdown,R1
    // asm: 	BGT	NOTGO
    // asm: 	LDI	SM_GO,R1		;we have to take care of the case when the plyr
    // asm: 	STI	R1,@SUSPEND_MODE	;just 'rolls' into the checkpoint
NOTGO:
    // asm: 	MPYF	MPH_CONVERSION,R0
    // asm: 	FIX	R0
    // asm: 	STI	R0,@_MPH
    // *GET YOUR SKID FACTOR
    // asm: 	CALL 	GETSKID
    // asm: 	LDF	*+AR5(CARSKID),R0
    // *DO YOUR TRACTION CHEAT...
    // asm: 	LDI	@HEAD2HEAD_ON,R1	;LINK?
    // asm: 	BZ	GSKD1			;FORGET IT, NOT HEAD2HEAD
    // asm: 	LDI	*+AR5(CAR_SPIN),R1  	;FULL SKID ON SPIN-OUT
    // asm: 	BNZ	GSKD1
    // asm:        	LDF	2.00,R1
    // asm: 	SUBF	@CHEAT,R1
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R0,*+AR5(CARSKID)	;CHEAT SKID VALUE
GSKD1:
    // asm: 	MPYF	R0,R0
    // asm: 	CMPF	1.0,R0
    // asm: 	LDFGT	1.0,R0
    // asm: 	SUBRF	1,R0
    // asm: 	CMPF	0.333,R0
    // asm: 	LDFLT	0.333,R0
    // asm: 	MPYF	2.5,R0			;STEERING WHEEL POWER
    // asm: 	STF	R0,@WHEELPWR
    // *GET CAR DIRECTION DELTA RADIANS
    // asm: 	CALL	GETTRAK			;GET CLOSEST TRAKC SECTION
    // asm: 	CALL	GETSTEER		;RET R0=STEERING DELTA RADIANS
    // asm: 	CALL	GETDIR			;ADJUST DIRECTION BASED ON SKID
    // asm: 	PUSHF	R0
    // asm: 	CALL	INBOUNDZ		;KEEP IN BOUNDS
    // asm: 	CALL	BACKCK			;KEEP IN RIGHT DIRECTION
    // asm: 	POPF	R0
    // *GET INCREMENTAL ROTATION MATRIX
    // asm: 	LDF	R0,R2
    // asm: 	ADDF	*+AR5(CARROT),R2    	;GET RID OF OLD OVERROTATION
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // *FORM NEW ROTATION MATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMAT
    // *FORM NEW VELOCITY MATRIX
    // asm: 	LDF	*+AR5(CARVROT),R2    	;GET VELOCITY MATRIX
    // asm: 	SUBF	*+AR5(CARYROT),R2
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDF  	*+AR5(CARDIST),R2	;GET DISTANCE
    // asm: 	MPYF	@CHEAT,R2		;DO THE CHEAT THING !!!
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	LDI	*+AR5(CAR_AIRB),R0
    // asm: 	BNZ	PAIRB	    		;WERE FLYING
    // *MOVE CAR FORWARD
    // asm: 	LDI	AR4,R3
    // asm: 	ADDI	OVELX,R3
    // asm: 	CALL	FORWARD
PAIRB:
    // asm: 	CALL	OVELADD
    // *GET ROAD MATRIX
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	CALL	CAR_ROAD_COLL
    // *GET NEW CAR MATRIX
    // asm: 	LDF	*+AR5(CARYROT),R2
    // asm: 	STF	R2,*+AR4(ORADY)		;STORE CAR OBJECT RADY
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMAT
    // *GET CAMERA ORIENTATION MATRIX
PLYRCAM:
    // *DO YOUR ZOOM JIVE
    // asm: 	CALL	ZOOMUP			;UPDATE YOUR ZOOM
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BNZ	CAM3RD
    // *FIRST PERSON CAMERA
    // asm: CAM1ST
    // ****************************
    // asm: 	CALL	CAMMATSAV
    // ****************************
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	NEGF	*+AR5(CARYROT),R2 	;FOLLOW CAR ROTATION
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	STF	R2,@_CAMERARAD+Y	;UPDATE CAMERA RAD
    // asm: 	SETDP				;RESTORE DP TO ZERO PAGE
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	CALL	CPYIMAT
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARROT)	;NO OVERROTATION IN CORNER
    // asm: 	LDF	*+AR5(CARXLEAN),R2  	;GET X LEAN FACTOR
    // asm: 	MPYF	-0.7,R2			;NEGATE AND SCALE
    // asm: 	LDP	@_CAMERARAD+X
    // asm: 	STF	R2,@_CAMERARAD+X
    // asm: 	SETDP
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	AR2,AR0
    // asm: 	LDI	@MATRIXCI,AR1
    // asm: 	PUSH	AR1
    // asm: 	LDF	*+AR5(CARZLEAN),R2  	;GET Z LEAN FACTOR
    // asm: 	MPYF	-0.5,R2			;NEGATE AND SCALE
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	CALL	CONCAT201    		;CONCAT YOUR MATRICES
    // asm: 	POP	R2		  	;GET MATRIXC POINTER
    // asm: 	LDI	@CAMERAMATRIXI,AR2	;GET SOURCE MATRIX
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // ****************************
    // asm: 	CALL	CAMMATAVG
    // *************************
    // asm: 	BR	CAM3RDX
    // 	;
    // 	;THIRD PERSON CAMERA
    // 	;
CAM3RD:
    // asm: 	CLRF	R2
    // asm: 	LDP	@_CAMERARAD+X
    // asm: 	STF	R2,@_CAMERARAD+X
    // asm: 	SETDP
    // asm: 	ABSF	*+AR4(OVELX),R0		;DONT CHANGE CAMERA DIR FOR SMALL VEL
    // asm: 	ABSF	*+AR4(OVELZ),R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	2,R1
    // asm: 	BLT	CAM3RD0
    // asm: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm: 	BNZ	CAM3RD0			;YES, DONT mess WITH CAMERA ANGLE
    // asm: 	LDF	*+AR4(OVELX),R3
    // asm: 	LDF	*+AR4(OVELZ),R2
    // asm: 	CALL	ARCTANF
    // *MAX CAMERA ANGLE CHANGE
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	SUBF	@_CAMERARAD+Y,R0	;GET OLD CAMERA
    // asm: 	LDF	0,R1 			;NORMALIZE DIFFERENCE
    // asm: 	CMPF	3.14,R0
    // asm: 	LDFGT	-6.28,R1
    // asm: 	CMPF	-3.14,R0
    // asm: 	LDFLT	6.28,R1
    // asm: 	ADDF	R1,R0
    // asm: 	MPYF	0.20,R0	    		;ANGLE SMOOTHING
    // asm: 	ADDF	@_CAMERARAD+Y,R0
    // asm: 	SETDP
    // asm: 	CALL	CAMCHK			;CHECK OUT NEW VALUE
    // asm: 	BZ	CAMOK			;ITS O.K...
    // *WE'RE OFF
    // asm: 	LDF	R0,R4	      		;SAVE NEW VALUE
    // asm: 	CALL 	CAMROT			;GET ADJUSTED ANGLE VALUE
    // asm: 	LDF	R0,R5			;SAVE NEW ADJUSTED VALUE
    // asm: 	LDP	@_CAMERARAD+Y,R6	;GET OLD VALUE
    // asm: 	LDF	@_CAMERARAD+Y,R6	;GET OLD VALUE
    // asm: 	SETDP
    // asm: 	SUBF	R6,R5,R2
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2,R3
    // asm: 	SUBF	R4,R5,R2
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2
    // asm: 	CMPF	R2,R3
    // asm: 	BLT	CAMBAD1
    // asm: 	LDF	R5,R0	      		;NEW ONE IS CLOSER, GO WITH CORRECTION
    // asm: 	B	CAMOK
CAMBAD1:
CAM3RD0:
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	LDF	@_CAMERARAD+Y,R0	;UPDATE CAMERA RAD
    // asm: 	SETDP
    // asm: 	CALL	CAMCHK			;CHECK IF OLDIE IS ON...
    // asm: 	BZ	CAMOK			;YES... KEEP IT
    // asm: 	CALL	CAMROT			;NO...GET CORRECTION
CAMOK:
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	STF	R0,@_CAMERARAD+Y	;UPDATE CAMERA RAD
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR2	;GET SOURCE MATRIX
    // asm: 	LDF	R0,R2
    // asm: 	CALL	FIND_YMATRIX 		;LOAD CAMERA MATRIX
    // asm: 	CALL 	GETCARROT   		;GET CAR OVERROTATE
    // asm: 	NEGF	R1,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX 		;GET CARROT MATRIX
    // asm: 	LDI	AR4,R2	  		;MULT INTO OBJ MATRIX
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMAT
CAM3RDX:
    // 	;
    // 	;GET NEW CAMERA POSITION
    // 	;GET NEW CAMERA OFFSET FROM CAR
    // 	;
    // asm: 	LDI	@CAMERAMATRIXI,AR2	;GET SOURCE MATRIX
    // asm: 	NEGF	@ZOOMD,R2		;GET ZOOM DISTANCE
    // asm: 	LDI	@VECTORAI,R3
    // asm: 	LDI	R3,AR3
    // asm: 	CALL	FORWARD
    // 	;
    // 	;ADD IT IN TO CAMERAPOS
    // 	;
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	SUBF	*AR3,R0		      	;INVERT X FOR SOME REASON
    // asm: 	LDI	@CAMERAPOSI,AR0
    // asm: 	STF	R0,*AR0			;X COORD
    // asm: 	LDF	0,R0			;ADJUST WRECK HEIGHT
    // asm: 	LDI	@WRECKFLG,R1
    // asm: 	LDFNZ	*+AR5(CT_PRDYD),R0
    // asm: 	MPYF	0.6,R0
    // asm: 	ADDF	@ZOOMH,R0
    // ;	LDF	@ZOOMH,R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR3(1),R0
    // asm: 	STF	R0,*+AR0(1)		;Y COORD
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR3(2),R0
    // asm: 	STF	R0,*+AR0(2)		;Z COORD
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BZ	PLYS1
    // asm: 	CALL	CAMYADJ			;YES, ADJUST CAMERA Y ABOVE ROAD
PLYS1:
    // asm: 	CALL	GETREV			;GET YOUR RPM'S, MAKE SOUND
    // asm: 	CALL	PLYR_SNDS		;HANDLE SOME PLYR SOUNDS
    // asm: 	CALL	PLMOTION
    // asm: 	CALL	PLYRWHL
    // asm: 	CALL	PLYR_RIDE_RIGHT		;FIND DISTANCE TO CENTER OF ROAD
    // asm: 	STF	R0,*+AR5(CARDIST2CNTR)
    // asm: PLYSLP
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	NOPLINK
    // asm: 	LDI	*+AR5(CARTRAK),AR0
    // asm: 	LDI	*+AR0(OUSR1),R0			;read road ID
    // asm: 	STI	R0,*+AR5(CARTRACK_ID)		;SAVE TRACK ID
    // asm: 	CALL	SEND_PLAYERS_POS
    // asm: 	CALL	CHEATCK
NOPLINK:
    // asm: 	SLEEP	1
    // asm: 	B	PLYRLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRLP", 0, 0);
    UNIMPL();
}

void CAMCHKL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CAMERA CHECK
    // *
    // *PARAMETERS
    // *	R0	CAMERA ANGLE
    // *	AR0	CAMERA POSITION XYZ POINTER
    // *	AR4	PLAYER CAR
    // *RETURNS
    // *	CARRY SET	COLLIDE WITH ROAD
    // asm: 	FLOAT	-170,R1	    		;GET LEFT CORNER DIST
    // asm: 	B	CAMCHK0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMCHKL", 0, 0);
    UNIMPL();
}

void CAMCHKR(void)
{
    // asm: 	FLOAT	170,R1	    		;GET RIGHT CORNER DIST
CAMCHK0:
    // asm: 	PUSHF	R0
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR4
    // asm:  	LDI	AR0,AR4
    // *CHECK RIGHT CORNER
    // *ADJUST CAMERA X,Z
    // asm: 	LDI	@VECTORAI,AR0  		;GET TEMP VECTOR, CORNER COORD STORE
    // asm: 	LDF	R0,R2			;GET CAMERA ANGLE
    // asm: 	CALL	_SINE
    // asm: 	NEGF	R0,R3
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R1,R0
    // asm: 	MPYF	R1,R3
    // asm: 	ADDF	*+AR4(0),R0	 	;GET X,Y,Z COORDS
    // asm: 	STF	R0,*AR0
    // asm: 	LDF	*+AR4(1),R0
    // asm: 	STF	R0,*+AR0(1)
    // asm: 	ADDF	*+AR4(2),R3
    // asm: 	STF	R3,*+AR0(2)
    // asm: 	LDI	AR0,AR4			;POINT TO XYZ OF CAMERA
    // asm: 	CALL	CAMSCAN
    // asm: CAMCHKRX
    // asm: 	POP	AR4
    // asm: 	POP	AR0
    // asm: 	POP	R3
    // asm: 	POPF	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMCHKR", 0, 0);
    UNIMPL();
}

void CAMCHKLR(void)
{
    // *----------------------------------------------------------------------------
    // *CAMCHKLR
    // *PARAMETERS
    // *	AR0	CAMERA POSITION XYZ POINTER
    // *	AR4	PLAYER CAR
    // *RETURNS
    // *	R3=0 CAMERA O.K.,
    // *	R0=1 LEFT OFF,
    // *	R0=2 RIGHT OFF,
    // *	R0=3 BOTH OFF
    // *RETURNS
    // *	Z=1 CAMERA O.K.
    // asm: 	CALL	CAMCHKL
    // asm: 	LDINC	1,R3
    // asm: 	LDIC	0,R3
    // asm: 	CALL	CAMCHKR
    // asm: 	LDINC	2,R1
    // asm: 	LDIC	0,R1
    // asm: 	ADDI	R1,R3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMCHKLR", 0, 0);
    UNIMPL();
}

void CAMCHK(void)
{
    // *----------------------------------------------------------------------------
    // *CAMERA	CHECKER
    // *PARAMETERS
    // *	R0	ANGLE
    // *	AR4	PLAYER CAR
    // *	AR5	CAR BLOCK
    // *RETURNS
    // *	R3=0 CAMERA O.K.
    // *	R0=1 LEFT OFF,
    // *	R0=2 RIGHT OFF,
    // *	R0=3 BOTH OFF
    // *RETURNS
    // *	Z=1 CAMERA O.K.
    // asm: 	PUSH	AR0
    // asm: 	CALL	GETCAMPOS
    // asm: 	LDI	AR3,AR0
    // asm: 	CALL	CAMCHKLR
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMCHK", 0, 0);
    UNIMPL();
}

void CAMROT(void)
{
    // *----------------------------------------------------------------------------
    // *CAMERA	ROTATER
    // *PARAMETERS
    // *	R0	CURRENT ANGLE
    // *	AR4	PLAYER CAR
    // *	AR5	CAR BLOCK
    // *RETURNS
    // *	R0	ADJUSTED ANGLE
    // asm: PUSH	AR0
    // asm:  	PUSH	AR2
    // asm: 	LDF	R0,R3
    // asm: 	CALL	ROADIR			;R0=ROADIR
    // asm: 	ADDF	R0,R3,R2
    // asm: 	CALL	NORMITS	      		;FIND DIRECTION
    // asm: 	ABSF	R2,R1		      	;STOP OSCILLATION AROUND
    // asm: 	CMPF	0.1,R1
    // asm: 	BGT	CAMROT1
    // asm: 	NEGF	R2,R1
    // asm: 	B 	CAMROT2
CAMROT1:
    // asm: 	LDF	R2,R2
    // asm: 	LDFN	0.1,R1
    // asm: 	LDFNN	-0.1,R1
CAMROT2:
    // asm: 	ADDF	R1,R3,R0		;RETURN R0=ADJUSTED ANGLE
    // asm:  	POP	AR2
    // asm:  	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMROT", 0, 0);
    UNIMPL();
}

void GETCAMPOS(void)
{
    // *----------------------------------------------------------------------------
    // *GET NEW CAMERA POSITION
    // *PARAMETERS
    // *	R0	CAMERARAD
    // *	AR4	PLAYER CAR
    // *RETURNS
    // *	AR3=VECTORA=CAMERAPOS X,Y,Z
    // asm: 	PUSHF	R0
    // asm: 	PUSH	AR2
    // asm: 	LDI	@MATRIXAI,AR2		;GET SOURCE MATRIX
    // asm: 	LDF	R0,R2
    // asm: 	CALL	FIND_YMATRIX 		;LOAD CAMERA MATRIX
    // asm: 	NEGF	@ZOOMD,R2		;GET ZOOM DISTANCE
    // asm: 	LDI	@VECTORAI,R3
    // asm: 	LDI	R3,AR3
    // asm: 	CALL	FORWARD
    // asm: 	LDF	*+AR4(OPOSX),R0		;ADD IN PLAYER CAR POSITION
    // asm: 	SUBF	*AR3,R0		      	;INVERT X FOR SOME REASON
    // asm: 	STF	R0,*AR3			;X COORD
    // asm: 	LDF	@ZOOMH,R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR3(1),R0
    // asm: 	STF	R0,*+AR3(1)		;Y COORD
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR3(2),R0
    // asm: 	STF	R0,*+AR3(2)		;Z COORD
    // asm: 	POP	AR2
    // asm: 	POPF	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCAMPOS", 0, 0);
    UNIMPL();
}

void CAMYADJ(void)
{
    // *----------------------------------------------------------------------------
    // *CAMERA ADJUST - KEEP IT OUT OF THE GROUND AND BUILDINGS
    // *PARAMETERS
    // *	AR0	CAMERA POSITION XYZ POINTER
    // *	AR4	PLAYER CAR
    // *ADJUST CAMERA Y
    // asm: 	PUSH	AR4
    // asm: 	LDI	AR0,AR4			;POINT TO XYZ OF CAMERA
    // asm: 	CALL	CAMSCAN
    // asm: 	LDI	AR4,AR0
    // asm: 	POP	AR4
    // asm: 	BNC	CAM1XX			;NO COLLISION, SKIP IT...
    // asm: 	FLOAT	133,R1
    // asm: 	SUBF	R0,R1
    // asm: 	BLE	CAM1XX
    // asm: 	FLOAT	500,R0		 	;OVERPASS BYPASS
    // asm: 	CMPF	R0,R1
    // asm: 	BGT	CAM1XX
    // asm: 	NEGF	R1
    // asm: 	ADDF	*+AR0(1),R1
    // asm: 	STF	R1,*+AR0(1)		;CAMERA Y COORD
CAM1XX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMYADJ", 0, 0);
    UNIMPL();
}

void PLYONRD(void)
{
    // *----------------------------------------------------------------------------
    // *PUT PLAYER ON ROAD
    // *PARAMETERS
    // *	AR4	PLAYER OBJECT
    // *	AR5	PLAYER CAR
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARSPRAD)
    // asm: 	STF	R0,*+AR5(CARDROT)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_SPIN)	;RESET SPIN FLAG
    // asm: 	STPI	R0,@WRECKFLG		;WRECK OFF
    // asm: 	LDI	AR4,AR0			;GET MATRIX BACK TO NORMAL
    // asm: 	ADDI	OMATRIX,AR0
    // asm: 	CALL	INITMAT
    // asm: 	CALL    GETTRAK			;GET CLOSEST TRACK SEGMENT
    // asm: 	LDF	*+AR0(OPOSX),R0		;NEW POSITION
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CALL  	ROADIR			;GET DIRECTION IN R0
    // asm: 	STF	R0,*+AR5(CARYROT)
    // asm: 	STF	R0,*+AR5(CARVROT)
    // asm: 	LDF	R0,R2
    // asm: 	NEGF	R0
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	STF	R0,@_CAMERARAD+Y	;UPDATE CAMERA YRAD
    // asm: 	SETDP
    // asm: 	CALL	_SINE
    // asm: 	LDF	R0,R3
    // asm: 	CALL	_COSI
    // asm: 	FLOAT	722,R1	    		;GET LANE DIST
    // asm: 	MPYF	R1,R0
    // asm: 	MPYF	R1,R3
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	ADDF	*+AR4(OPOSZ),R3
    // asm: 	STF	R3,*+AR4(OPOSZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYONRD", 0, 0);
    UNIMPL();
}

void DRONEGO(void)
{
    // *----------------------------------------------------------------------------
    // *DRONE LOOP
    // *PARAMETERS
    // *	R2	CURRENT STEERING DELTA ANGLE
    // *	AR4	CAR OBJECT
    // *	AR5	CAR DATA BLOCK
    // *		CARBLK STUFF TO LOAD
    // *		CARTURN      	;ANGLE OF FRONT WHEELS 		(FLOAT)
    // *		CARTRACTION  	;TRACTION COEFFICIENT OF TIRES	(FLOAT)
    // *		CARMAXACCEL  	;MAXIMUM ACCEL (PIX/16 MSEC)	(FLOAT)
    // *		CARTHROTTLE  	;THROTTLE VALUE 0-1.0 		(FLOAT)
    // *		CARBRAKE	;BRAKING FRICTION (0-1.0)	(FLOAT)
    // *
    // asm: 	PUSHF	R2
    // asm: 	CALL	GETAUTO			;GET AUTO TRANS VALUE
    // asm: 	STI	R0,*+AR5(CARGEAR) 	;DO THE GEAR
    // ;	LDF	1.0,R0
    // ;	STF	R0,@DRAFTVAL		;NO DRAFTING FOR DRONES...
    // asm: 	CALL	GETSPD
    // asm: 	CALL 	GETSKID
    // asm: 	CALL	GETRPM			;GET YOUR REVS
    // *GET CAR DIRECTION DELTA RADIANS
    // asm: 	POPF	R0			;GET STEERIN ANGLE
    // asm: 	CALL	GETDIR
    // asm: 	PUSHF	R0
    // asm: 	CALL	DRONINBZ
    // asm: 	POPF	R2
    // *GET INCREMENTAL ROTATION MATRIX
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // *FORM NEW ROTATION MATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMAT
    // *FORM NEW VELOCITY MATRIX
    // asm: 	LDF	*+AR5(CARVROT),R2    	;GET VELOCITY MATRIX
    // asm: 	SUBF	*+AR5(CARYROT),R2
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDF  	*+AR5(CARDIST),R2	;GET DISTANCE
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	LDI	*+AR5(CAR_AIRB),R0
    // asm: 	BNZ	DAIRB	    		;WERE FLYING
    // *MOVE CAR FORWARD
    // asm: 	LDI	AR4,R3
    // asm: 	ADDI	OVELX,R3
    // asm: 	CALL	FORWARD
DAIRB:
    // asm: 	CALL	OVELADD
    // *GET ROAD MATRIX
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONEGO", 0, 0);
    UNIMPL();
}

void DRONESTOP(void)
{
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	CALL	CAR_ROAD_COLL
    // *GET CAR MATRIX
    // asm: 	LDF	*+AR5(CARYROT),R2
    // asm: 	STF	R2,*+AR4(ORADY)		;STORE CAR OBJECT RADY
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // *FORM NEW ROTATION MATRIX FOR CAR
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	CALL	DRONE_RIDE_RIGHT	;GET DISTANCE TO CENTER OF LANE
    // asm: 	STF	R0,*+AR5(CARDIST2CNTR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONESTOP", 0, 0);
    UNIMPL();
}

void GETREV(void)
{
    // *----------------------------------------------------------------------------
    // *GET REV FOR PLAYER CAR
    // *PARAMETERS
    // *	AR4	PLAYER CAR OBJECT
    // *	AR5	PLAYER CAR STRUCTURE
    // asm: 	LDI	@_MODE,R1	       	;ON STARTING LINE?
    // asm: 	TSTB	MGO,R1
    // asm: 	BZ	REV0			;YES, NEUTRAL GEAR
    // asm: 	LDI	*+AR5(CARGEAR),R0
    // asm: 	BZ	REV0			;NEUTRAL GEAR
    // asm: 	LDI	*+AR5(CAR_AIRB),R0	;IN AIR?
    // asm: 	BZ	REV1	    		;NO... REGULAR REV STUFF
    // *FREE REV NEUTRAL CASE
REV0:
    // asm: 	CALL	GETPEDAL		;GET PEDAL 0-1
    // asm: 	MPYF	NUM_RPM,R0
    // asm: 	LDF	*+AR5(CARRPM),R1
    // asm: 	FLOATP	@NFRAMES,R3
    // asm: 	MPYF	2,R3
    // asm: 	ADDF	R3,R1
    // asm: 	LDF	R1,R2 			;MAX SLEW RATE LIMIT
    // asm: 	MPYF	2,R3
    // asm: 	SUBF	R3,R1			;MIN SLEW RATE LIMIT
    // asm: 	CMPF	R2,R0
    // asm: 	LDFGT	R2,R0
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	B	REV3
    // *REGULAR IN GEAR CASE
REV1:
    // asm: 	LDI	@GEARTABI,AR0
    // asm: 	ADDI	*+AR5(CARGEAR),AR0
    // asm: 	LDF	*+AR5(CARSPEED),R0	;GET SPEED
    // asm: 	MPYF	*AR0,R0			;MULTIPLY BY GEAR RATIO
REV3:
    // asm: 	FIX	*+AR5(CARRPM),R1
    // asm: 	STF	R0,*+AR5(CARRPM)
    // asm: 	FIX	R0,AR0
    // *R1=OLD REV
    // *AR0=NEW REV
    // *GET ENGINE VOLUME
    // *THROTTLE FACTOR
    // asm: 	LDF	*+AR5(CARTHROTTLE),R0
    // asm: 	MPYF	0.8,R0
    // asm: 	ADDF	0.2,R0
    // *SPEED FACTOR
    // asm: 	LDF	*+AR5(CARSPEED),R2
    // asm: 	MPYF	0.01,R2
    // asm: 	MPYF	0.333,R2
    // asm: 	SUBRF	1.0,R2
    // asm: 	MPYF	0.75,R2
    // asm: 	ADDF	0.25,R2
    // asm: 	CMPF	1.0,R2
    // asm: 	LDFGT	1.0,R2
    // asm: 	CMPF	0,R2
    // asm: 	LDFLT	0,R2
    // asm: 	MPYF	R2,R0
    // asm: 	FLOAT	225,R2
    // asm: 	MPYF	R2,R0
    // *REV FACTOR
    // asm: 	LDF	*+AR5(CARRPM),R2
    // asm: 	MPYF	0.02,R2
    // asm: 	MPYF	0.5,R2
    // asm: 	ADDF	0.5,R2
    // asm: 	MPYF	R2,R0
    // asm: 	FIX	R0		;NEW ENGINE VOLUME
    // *CHECK FOR CHANGE IN REV OR VOLUME
    // asm: 	CMPI	R1,AR0 		;NEW REV?
    // asm: 	BNE	REV4		;NO...
    // asm: 	LDI	@ENGVOL,R1	;NEW ENGINE VOLUME ?
    // asm: 	SUBI	R0,R1
    // asm: 	ABSI	R1
    // asm: 	CMPI	4,R1
    // asm: 	RETSLT			;NO... QUIT
REV4:
    // asm: 	LDI	R0,R1
    // asm: 	STI	R0,@ENGVOL	;STORE NEW VOLUME
    // asm: 	CMPI	0,AR0
    // asm: 	LDILT	0,AR0
    // asm: 	MPYI	5,AR0
    // asm: 	LDI	AR0,R0		;SPEED (OF OSCILLATION)
    // asm: 	CMPI	253,R0
    // asm: 	LDIGT	253,R0
    // asm: 	LDI	@_MODE,R2
    // asm: 	AND	MSLINE,R2
    // asm: 	BZ	REV5
    // asm: 	LSH	-4,R1		;CUT VOLUME ON START LINE
REV5:
    // asm: 	B	PLYR_ENGINE	;GO DO IT...
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETREV", 0, 0);
    UNIMPL();
}

void GETRPM(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GETRPM	FOR DRONE CAR
    // *PARAMETERS
    // *	AR4	CAR OBJECT
    // *	AR5	CAR STRUCTURE
    // asm: 	LDI	@GEARTABI,AR0
    // asm: 	ADDI	*+AR5(CARGEAR),AR0
    // asm: 	LDF	*+AR5(CARSPEED),R0	;GET SPEED
    // asm: 	MPYF	*AR0,R0			;MULTIPLY BY GEAR RATIO
    // asm: 	CMPF	NUM_RPM,R0		;LIMIT TO RANGE
    // asm: 	LDFGT	NUM_RPM,R0
    // asm: 	CMPI	0,R0
    // asm: 	LDILT	0,R0
    // asm: 	STF	R0,*+AR5(CARRPM)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETRPM", 0, 0);
    UNIMPL();
}

void GETSKID(void)
{
    // *----------------------------------------------------------------------------
    // *GET SKID FACTOR
    // *PARAMETERS
    // *	AR4	CAR
    // *	AR5	CAR BLOCK
    // *RETURNS
    // *	R0	SKID FACTOR
    // *	0=NOSKID, 1.0=FULL SKID
    // *CHECK SPIN OUT
    // asm: 	LDI	*+AR5(CAR_SPIN),R0  	;FULL SKID ON SPIN-OUT
    // asm: 	BEQ	GETSK00
    // asm:        	LDF	1.00,R0
    // asm: 	B	GETSKXX
    // *CHECK OVERREV
GETSK00:
    // asm: 	LDI	*+AR5(CARGEAR),R0	;HI GEAR?
    // asm: 	CMPI	4,R0
    // asm: 	BZ	GETSK0			;YES, NO OVERREV...
    // asm: 	LDF	*+AR5(CARRPM),R0
    // asm: 	CMPF	OVERREV,R0 		;OVERREV:
    // asm: 	BLT	GETSK0
    // asm:        	LDF	0.80,R0			;YES DO A SKID
    // asm: 	B	GETSKXX
    // *GET STEERING-SPEED SKID
GETSK0:
    // asm: 	LDF	*+AR5(CARYROT),R2
    // asm: 	SUBF	*+AR5(CARVROT),R2
    // asm: 	CALL	NORMITS
    // asm: 	CMPF	1.2,R2			;KEEP IN RANGE
    // asm: 	LDFGT	1.2,R2
    // asm: 	CMPF	-1.2,R2
    // asm: 	LDFLT	-1.2,R2
    // asm: 	LDF	R2,R0
    // asm: 	MPYF	0.2,R0		  	;ADJUST IT DUDE
    // asm: 	ADDF	*+AR5(CARTURN),R0
    // asm: 	ABSF	R0
    // asm: 	CMPF	0.3,R0
    // asm: 	LDFGT	0.3,R0			;MAX IT OUT
    // asm: 	MPYF	*+AR5(CARSPEED),R0
    // *GET LOW SPEED RIPOUT
    // asm: 	LDF	*+AR5(CARTHROTTLE),R1	;FULL THROTTLE?
    // asm: 	CMPF	0.90,R1
    // asm: 	BLT	GETSK1			;NO
    // asm: 	LDI	*+AR5(CARGEAR),R1	;FULL THROTTLE?
    // asm: 	BZ	GETSK1
    // asm: 	CMPI	2,R1
    // asm: 	BGT	GETSK1			;ONLY FIRST GEAR...
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // ;	SUBF	65,R1
    // asm: 	SUBF	100,R1
    // asm: 	LDFGT	0,R1
    // asm: 	ABSF	R1
    // asm: 	MPYF	1.1,R1
    // asm: 	ADDF	R1,R0
GETSK1:
    // *GET BRAKE FACTOR
    // asm: 	LDF	*+AR5(CARBRAKE),R1
    // asm: 	MPYF	1.0,R1
    // asm: 	ADDF	1.0,R1
    // asm: 	MPYF	R1,R0			;DOUBLE IT FOR BRAKE DUDE!!!
    // *GET THROTTLE FACTOR
    // asm: 	LDF	*+AR5(CARTHROTTLE),R1
    // asm: 	MPYF	0.25,R1
    // asm: 	ADDF	1.0,R1
    // asm: 	MPYF	R1,R0			;25% BOOST FOR THROTTLE ON
    // asm: 	SUBF	22,R0
    // ;	SUBF	15,R0
    // asm: 	LDFLT	0,R0   			;NO SKID
    // ;	MPYF	0.05,R0			;DIVIDE BY 25
    // asm: 	MPYF	0.045,R0 		;DIVIDE BY 25
    // asm: GETSKX
    // asm: 	CMPF	1.0,R0
    // asm: 	LDFGT	1.0,R0
    // asm: 	MPYF	*+AR5(CARTRACTION),R0	;GET TRACTION COEFF.
GETSKXX:
    // asm: 	STF	R0,*+AR5(CARSKID) 	;NEW SKID FACTOR
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSKID", 0, 0);
    UNIMPL();
}

void CKOFRD(void)
{
    // *----------------------------------------------------------------------------
    // *CHECK FOR OFFROAD TIMEOUT
    // *PARAMETERS
    // *	AR5	PLAYER CAR STRUCTURE
    // *RETURNS
    // *	R0	RETURNED WITH OFFROAD TIMER
    // asm: 	LDI	AR5,AR3		       	;CHECK FOR ALL WHEELS OFF
    // asm: 	ADDI	CARPCOL,AR3
    // asm: 	LDI	0,R3
    // asm: 	LDI	@OFFROAD_TMR,R0
    // asm: 	LDI	4,RC
    // asm: 	RPTB	OFFRLP
    // asm: 	LDI	*AR3++(CARVSIZ),AR0 	;GET ROAD OBJECT INTERSECTING
    // asm: 	LDI	*+AR0(OID),R1		;CHECK OID
    // asm: 	AND	CLASS_M+TYPE_M,R1
    // asm: 	CMPI	ROAD_C,R1
    // asm: 	BZ	NOTOFF
    // asm: 	AND	CLASS_M,R1
    // asm: 	CMPI	ROAD_C,R1
    // asm: 	LDIZ	1,R3
    // asm: 	CMPI	GROUND_C,R1
OFFRLP:
    // asm: LDIZ	1,R3
    // asm: 	LDI	R3,R3 			;ANY GROUND?
    // asm: 	BNE	NOTOFFX			;YES
    // asm: 	CMPI	8,R0
    // asm: 	LDILE	0,R0			;RESET HIM ON 8 IF NO GROUND
    // asm: 	B	NOTOFFX
NOTOFF:
    // asm: 	LDI	20,R0
    // asm: 	STI	R0,@OFFROAD_TMR		;ALL WHEELS OFFROAD FLAG
NOTOFFX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKOFRD", 0, 0);
    UNIMPL();
}

void GETDIR(void)
{
    // *----------------------------------------------------------------------------
    // *GETDIR-->GET CAR DIRECTION
    // *PARAMETERS
    // *	R0	NEW STEERING DELTA
    // *	AR4	CAR OBJECT
    // *       AR5	CAR BLOCK POINTER
    // *RETURNS
    // *	R0	NET STEERING CHANGE
    // *	CARYROT,CARVROT,CARDROT SET
    // *CLOBBERED
    // *	R1,R2,R3,R4,R5
    // asm: 	LDI	*+AR5(CAR_AIRF),R1
    // asm: 	BZ	GETDIR1
    // asm: 	LDF	0,R0			;NO STEERING IN AIR
    // asm: 	LDF	*+AR5(CARDROT),R1
    // asm: 	ADDF	*+AR5(CARYROT),R1
    // asm: 	B	GETDIR2
GETDIR1:
    // asm: 	LDI	*+AR5(CAR_SPIN),R1	;CHECK SPINOUT...
    // asm: 	BNZ	CARSPIN
    // *GET SKID FACTOR
    // asm: 	LDF	*+AR5(CARSKID),R4	;R4=SKID FACTOR
    // asm: 	LDF	1.0,R5
    // asm: 	SUBF	R4,R5			;R5=1-SKID FACTOR
    // asm: 	MPYF	0.75,R5
    // asm: 	ADDF	0.25,R5			;ADJUST STEER BY .5-1.0 (.5=MAX SKID)
    // asm: 	MPYF	R5,R0			;SKID ADJUSTED STEERING
    // *GET SLIDE DELTA ANGLE
    // asm: 	LDF	*+AR5(CARVROT),R3  	;VELOCITY ROTATION
    // asm: 	LDF	*+AR5(CARYROT),R1	;BODY ROTATION
    // asm: 	SUBF	R3,R1,R2		;ROTATIONAL DIFFERENCE
    // asm: 	CALL	NORMITS			;NORMALIZE DIFFERENCE
    // *GET RECOVERY FACTOR
    // asm: 	MPYF	0.095,R2
    // asm: 	NEGF	R2,R5
    // asm: 	MPYF	0.5,R5
    // asm: 	ADDF	R5,R1
    // *GET NEW CARVROT
    // asm: 	ADDF	R2,R3			;ADD IN RECOVERY FACTOR
    // asm: 	ADDF	R0,R3			;ADD IN STEERING FACTOR
    // asm: 	STF	R3,*+AR5(CARVROT)  	;VELOCITY DIRECTION
    // *GET DELTA BODY MOMENTUM
    // asm: 	MPYF	*+AR5(CARDROT),R4	;BODY DELTA MOMENTUM ON SKID
    // asm: 	MPYF	0.7,R4
    // asm: 	LDF	*+AR5(CARTHROTTLE),R5
    // asm: 	MPYF	0.5,R5
    // asm: 	ADDF	0.5,R5
    // asm: 	MPYF	1.2,R5
    // asm: 	MPYF	R5,R4			;PUMP UP BODY SLIDE...
    // asm: 	ADDF	R4,R0
    // asm: 	STF	R0,*+AR5(CARDROT)	;BODY DELTA
    // asm: 	ADDF	R0,R1			;ADD CARDROT TO CARYROT
GETDIR2:
    // asm: 	STF	R1,*+AR5(CARYROT)	;BODY DIRECTION
    // asm: 	RETS
    // *
    // *CAR SPINOUT
    // *
CARSPIN:
    // asm: 	LDI	@NFRAMES,AR3
    // asm: 	SUBI	1,AR3			;CUT DOWN COUNT
    // asm: 	CMPI	2,R1 			;TIMED SPIN?
    // asm: 	BLT	SPINL0			;NO...
    // *TIMED SPIN
    // asm: 	SUBI	1,R1			;DECREMENT TIME
    // asm: 	SUBI	AR3,R1
    // asm: 	CMPI	2,R1
    // asm: 	BLE	SPINREC			;TIMES UP RECOVER
    // asm: 	STI	R1,*+AR5(CAR_SPIN)
    // asm: 	LDF	*+AR5(CARDROT),R1	;ROTATE THE DUDE
    // asm: 	ADDF	*+AR5(CARYROT),R1
    // asm: 	STF	R1,*+AR5(CARYROT)	;BODY DIRECTION
    // asm:       	B	SPINLX
    // *ANGULAR SPIN
SPINL0:
    // asm: 	LDF	*+AR5(CARDROT),R1	;ROTATE THE DUDE
    // asm: 	LDF	*+AR5(CARYROT),R2  	;VELOCITY ROTATION
    // asm: 	ADDF	R1,R2,R3
    // asm: 	STF	R3,*+AR5(CARYROT)	;BODY DIRECTION
    // asm: 	ABSF	R1,R3
    // asm: 	SUBRF	*+AR5(CARSPRAD),R3	;READY FOR RECOVERY?
    // asm: 	STF	R3,*+AR5(CARSPRAD)
    // asm: 	BNN	SPINL			;SPIN NOT OVER YET...
    // asm: 	CALL	ROADIR			;GET NEAREST TRACK PIECE ANGLE IN R0
    // *CHECK FOR DRONE
    // asm: 	LDI	*+AR4(OID),R1
    // asm: 	AND	CLASS_M+SUBTYPE_M,R1
    // asm: 	CMPI	DRONE_C+DRNE_RHO,R1
    // asm: 	BNE	REGSPIN
    // asm: 	LDI	*+AR4(OID),R1
    // asm: 	AND	TYPE_M,R1
    // asm: 	CMPI	DEAD_VEH_T,R1		;CHECK FOR DEAD DRONE
    // asm: 	BNE	DRONESPIN      		;NOT DEAD
    // asm: 	LDF	*+AR5(CARDROT),R0	;DECAY SPIN FOR DEAD GUY...
    // asm: 	MPYF	0.98,R0
    // asm: 	ABSF	R0,R1
    // asm: 	CMPF	0.04,R1
    // asm: 	LDFLT	0,R0
    // asm: 	STF	R0,*+AR5(CARDROT)
    // asm: 	B	SPINL
DRONESPIN:
    // asm: 	ADDF	3.14,R0			;ADJUST FOR TRAFFIC GOING AGAINST ROAD
REGSPIN:
    // asm: 	LDF	R0,R2
    // asm: 	SUBF	*+AR5(CARYROT),R2  	;VELOCITY ROTATION
    // asm: 	CALL 	NORMITS			;NORMALIZE DIFFERENCE
    // asm: 	ABSF	R2
    // asm: 	CMPF	0.2,R2			;ROTATIONS CLOSE?
    // asm: 	BLT	SPINREC0		;DO THE RECOVERY...
SPINL:
    // asm: 	DB	AR3,SPINL0
SPINLX:
    // asm: 	LDF	0,R0			;NO STEERING SPIN
    // asm: 	RETS
    // *RECOVER FROM SPIN DUDE
SPINREC0:
    // asm: 	STF	R0,*+AR5(CARYROT)	;LOAD ROAD DIRECTION
SPINREC:
    // asm: 	LDF	*+AR5(CARYROT),R2
    // asm: 	CALL 	NORMIT
    // asm: 	STF	R2,*+AR5(CARYROT)	;BODY DIRECTION
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDI	0,R2
    // asm: 	STI	R2,*+AR5(CAR_SPIN)  	;CLEAR SPIN FLAG
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARSPRAD)     	;CLEAR SPIN RADIANS
    // asm: 	STF	R0,*+AR5(CARDROT)	;BODY DELTA
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETDIR", 0, 0);
    UNIMPL();
}

void GETCARROT(void)
{
    // *----------------------------------------------------------------------------
    // *GETCARROT- GET OVERROTATION
    // asm: 	PUSH	R3
    // asm: 	LDF	*+AR5(CARROT),R0      	;GET PREVIOUS VALUE
    // asm: 	LDF	*+AR5(CARTURN),R1
    // asm: 	MPYF	-0.5,R1			;ADJUST VALUE
    // asm: 	LDI	*+AR5(CAR_AIRF),R2 	;NO CHANGE IF IN THE AIR
    // asm: 	LDFNZ	R0,R1
    // asm: 	LDF	*+AR5(CARDIST),R2	;LIMIT ROTATION SLEW RATE BY SPEED
    // asm: 	CMPF	10,R2
    // asm: 	LDFGT	10,R2
    // asm: 	MPYF	0.05,R2
    // asm: 	MPYF	0.1,R2		 	;SLEW RATE
    // asm: 	SUBF	R1,R0,R3
    // asm: 	ABSF	R3,R3
    // asm: 	CMPF	R2,R3			;DONT CHANGE FOR SMALL VALUE
    // asm: 	BLE	GETC1
    // asm: 	SUBF	R1,R0,R3     		;LIMIT TO +-0.05
    // asm: 	BN	GETC0
    // asm: 	NEGF	R2
GETC0:
    // asm: 	ADDF	R2,R0,R1
GETC1:
    // asm: 	LDI	@WRECKFLG,R0
    // asm: 	LDFNZ	0,R1			;CLEAR CARROT ON WRECK
    // *EXPERIMENTAL CODE
    // ;	LDF	0,R1
    // ******************
    // asm: 	STF	R1,*+AR5(CARROT)
    // asm: 	POP	R3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCARROT", 0, 0);
    UNIMPL();
}

void GETSPD(void)
{
    // *----------------------------------------------------------------------------
    // *GET NEW CAR SPEED
    // *
    // *PARAMETERS
    // *	AR4	CAR OBJECT
    // *	AR5	CAR BLOCK
    // *
    // asm: 	LDI	*+AR5(CAR_AIRB),R0
    // asm: 	BZ	GETSPD1	      		;NO AIR DUDE...
    // *AIRBORNE CASE
    // asm: 	FLOATP	@NFRAMES,R0
    // asm: 	MPYF	4,R0 			;GET GRAVITATIONAL ACCEL
    // asm: 	ADDF	*+AR4(OVELY),R0
    // asm: 	STF	R0,*+AR4(OVELY)
    // asm: 	LDF	0,R0			;SET ACCEL, FRICT TO ZERO
    // asm: 	LDF	0,R3
    // asm: 	BR	GETSPD2
    // *CHECK SPIN OUT
GETSPD1:
    // asm: 	LDI	*+AR5(CAR_SPIN),R0
    // asm: 	BZ	GETSPD10       		;NO SPINOUT...
    // asm: 	LDF	0,R0			;SET ACCEL TO ZERO
    // asm: 	LDF	@SPINFRICI,R3
    // asm: 	BR	GETSPD2
    // *GET ENGINE ACCEL
GETSPD10:
    // asm: 	LDF	*+AR5(CARTHROTTLE),R0
    // asm: 	MPYF	*+AR5(CARMAXACCEL),R0
    // asm: 	CMPI	@PLYCAR,AR4	    	;CHEAT ACCEL
    // asm: 	LDFNZ	1.0,R1
    // asm: 	LDFZ	@CHEATACC,R1
    // asm: 	MPYF	R1,R0
    // asm: 	LDF	*+AR5(CARRPM),R1
    // asm: 	MPYF	0.333,R1
    // asm: 	FIX	R1,IR0		   	;GET TABLE INDEX
    // asm: 	CMPI	18,IR0
    // asm: 	LDIGT	18,IR0			;KEEP INDEX IN RANGE
    // asm: 	LDI	@ENGACTABI,AR0
    // asm: 	LDF	*+AR0(IR0),R2		;GET LO POWER FACTOR FOR GEAR
    // asm: 	ADDI	1,IR0
    // asm: 	LDF	*+AR0(IR0),R3		;GET HI POWER FACTOR FOR GEAR
    // asm: 	FLOAT	IR0,R4
    // asm: 	SUBF	R1,R4,R1
    // asm: 	LDFLT	0,R1  			;KEEP FACTOR IN BOUNDS
    // asm: 	CMPF	1.0,R1
    // asm: 	LDFGT	1.0,R1
    // asm: 	MPYF	R1,R2			;INTERPOLATE !!!
    // asm: 	SUBRF	1.0,R1
    // asm: 	MPYF	R1,R3
    // asm: 	ADDF	R2,R3,R1
    // asm: 	LDI	*+AR5(CARGEAR),IR0	;GEAR MULTIPLIER
    // asm: 	LDI	@GEARACTABI,AR0
    // asm: 	MPYF	*+AR0(IR0),R1
    // asm: 	LDF	1.0,R2
    // asm: 	LDI	*+AR5(CARTRANS),R3
    // asm: 	LDFZ	0.96,R2			;4% POWER LOSS AUTOMATIC
    // asm: 	MPYF	R2,R1
    // *CUT ACCEL ON SKID
    // asm: 	MPYF	R1,R0
    // asm: 	LDF	*+AR5(CARSKID),R1    	;CUT DOWN ACCEL ON SKID
    // asm: 	MPYF	0.25,R1			;ONLY 25% CUT
    // asm: 	SUBRF	1.0,R1
    // asm: 	MPYF	R1,R0			;R0=ENGINE ACCEL
    // *GET GRAVITY ACCEL
    // asm: 	LDF	*+AR4(OMAT21),R1	;ADD IN YOUR GRAVITY ACTION
    // asm: 	LDF	2,R3			;DEFAULT CONSTANT
    // asm: 	LDI	@_countdown,R2		;TIMEOUT?
    // asm: 	LDFZ	0,R3			;YES, NO GRAVITY
    // asm: 	LDI	@_MODE,R2		;ON START LINE?
    // asm: 	TSTB	MGO,R2
    // asm: 	LDFZ	0,R3			;YIP, NO GRAVITY
    // asm: 	LDF	*+AR5(CARBRAKE),R2	;BRAKE ON?
    // asm: 	CMPF	0.5,R2
    // asm: 	LDFGT	0,R3			;YES, NO GRAV
    // asm: 	MPYF	R3,R1			;MULTIPLY BY CONSTANT
    // asm: 	ADDF	R1,R0
    // *GET TOTAL FRICTION
    // *GET ROAD FRICTION
    // asm: 	LDI	AR5,AR3
    // asm: 	ADDI	CARPCOL,AR3
    // asm: 	LDF	0,R3
    // asm: 	LDI	4,RC
    // asm: 	RPTB	FRICLP
    // asm: 	LDI	*AR3++(CARVSIZ),AR0 	;GET ROAD OBJECT INTERSECTING
    // asm: 	LDI	*+AR0(OID),R1		;CHECK OID
    // asm: 	AND	CLASS_M+TYPE_M,R1
    // asm: 	CMPI	ROAD_C,R1
    // asm: 	LDFZ	*+AR5(CARRDFR),R2    	;GET ROAD FRICTION
    // asm: 	LDFNZ	*+AR5(CAROFRDFR),R2	;GET OFF ROAD FRICTION
    // ;	MPYF	@DRAFTVAL,R2		;ADJUST FOR DRAFT
FRICLP:
    // asm: ADDF	R2,R3
    // asm: 	MPYF	0.20,R3			;TAKE AVERAGE BASED ON WHEELS OFF
    // *JARV CHANGE
    // asm: 	CMPI	@PLYCAR,AR4
    // asm: 	BNE	FRL1
    // asm: 	LDF	@CHEAT,R1
    // asm: 	CMPF	1.09,R1
    // asm: 	BLE	FRL1
    // asm: 	ADDF	*+AR5(CARRDFR),R3 	;CUT DOWN OFF ROAD FRIC IF BEHIND
    // asm: 	MPYF	0.5,R3
FRL1:
    // *JARV ENDCHANGE
    // *GET SKID FRICTION
    // asm: 	LDF	*+AR5(CARSKID),R4    	;ADD IN SKID FACTOR
    // asm: 	LDF	@SKIDFRICI,R1
    // asm: 	MPYF	R1,R4,R5
    // asm: 	LDF	*+AR5(CARSPEED),R2	;CUT SKID FRICTION FOR LOW SPEED BURNOUT
    // asm: 	CMPF	100,R2
    // asm: 	BGT	FRIC0
    // asm: 	LDF	*+AR5(CARTHROTTLE),R2	;FULL THROTTLE?
    // asm: 	CMPF	0.90,R2
    // asm: 	BLT	FRIC0
    // asm: 	LDI	*+AR5(CARGEAR),R2
    // asm: 	CMPI	2,R2
    // asm: 	BLE	FRIC1			;LOW SPEED BURNOUT, NO FRICTION111
    // ;	BZ	FRIC1			;LOW SPEED BURNOUT, NO FRICTION111
FRIC0:
    // asm: 	ADDF	R5,R3
    // *GET BRAKE FRICTION
FRIC1:
    // asm: 	LDF	*+AR5(CARBRAKE),R5   	;ADD IN BRAKE FRICTION
    // asm: 	MPYF	@BRAKFRICI,R5
    // asm: 	NEGF	R4
    // asm: 	ADDF	1.0,R4			;BRAKE LOSES EFFECT IN SKID
    // asm: 	MPYF	R4,R5
    // asm: 	LDF	1.0,R4			;INCREASE BRAKE EFFECTIVENESS AT LO SPD
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm: 	CMPF	40,R1
    // asm: 	LDFLT	1.5,R4
    // asm: 	CMPF	20,R1
    // asm: 	LDFLT	2.0,R4
    // asm: 	MPYF	R4,R5
    // asm: 	ADDF	R5,R3		     	;TOTAL FRICTION
    // *GET ENGINE FRICTION
    // asm: 	LDI	*+AR5(CARGEAR),IR0
    // asm: 	LDI	@ENGFRI,AR0
    // asm: 	LDF	*+AR0(IR0),R4
    // asm: 	LDF	*+AR5(CARRPM),R5	;MORE REVS MORE FRICTION
    // asm: 	SUBF	42,R5			;HEAVY FRICTION ABOVE 3900
    // asm: 	LDFLT	0,R5
    // asm: 	MPYF	0.04,R5
    // asm: 	ADDF	1.0,R5			;APPROX RANGE 1-2
    // asm: 	MPYF	R4,R5
    // asm: GETSP22
    // asm: 	ADDF	R5,R3
    // *CALC NEW SPEED
GETSPD2:
    // asm: 	LDF	0,R5
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm: 	LDI	@NFRAMES,RC
    // asm: 	CMPI	6,RC	 		;MAX IT OUT TO BE SAFE
    // asm: 	LDIGT	6,RC
    // asm: 	SUBI	1,RC
    // *R0=ACCEL
    // *R1=SPEED
    // *R3=TOTAL FRICTION
    // *R5=DISTANCE
    // *R7=ENGINE FRICTION
    // asm: 	RPTB	GSL0
    // asm: 	MPYF 	R1,R3,R4	     	;BRAKE/ROAD/ENGINE FRICTION
    // asm: 	SUBF	R4,R1			;SUBTRACT FRICTION
    // asm: 	ADDF	R0,R1			;ADD ACCEL
GSL0:
    // asm: ADDF	R1,R5
    // asm: 	CMPF	0.5,R5	      		;MINIMUM SPEED
    // asm: 	LDFLT	0,R5
    // asm: 	CMPF	0,R1			;NO NEGATIVE SPEED
    // asm: 	LDFLT	0,R1
    // asm: 	MPYF	1.5,R5			;SPEEDFUDGE
    // asm: 	STF	R5,*+AR5(CARDIST)	;SAVE YOUR DISTANCE
    // asm: 	STF	R1,*+AR5(CARSPEED)	;NEW SPEED
    // asm: 	RETS
    // *
    // *GET BRAKE PEDAL
    // *RETURNS R0=BRAKE VALUE 0-1 (FLOAT)
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSPD", 0, 0);
    UNIMPL();
}

void GETBRAKE(void)
{
    // asm: 	FLOATP	@_pot2,R0
    // asm: 	NEGF	@BRAKEMN,R1
    // asm: 	ADDF	R1,R0
    // asm: 	CMPF	20,R0
    // asm: 	LDFLT	0,R0
    // asm: 	BLT	BRAKEOFF
    // asm: 	ADDF	@BRAKEMX,R1	;ALL DP RAM IN SAME PAGE
    // asm: 	CALL	DIV_F
    // asm: 	CMPF	1.0,R0 		;KEEP IT IN RANGE
    // asm: 	LDFGT	1.0,R0
    // asm: 	CMPF	0,R0 		;KEEP IT IN RANGE
    // asm: 	LDFLT	0,R0
    // asm: 	MPYF	R0,R0		;SQUARE IT FOR NON-LINEAR FEEL
    // asm: 	PUSHF	R0		;SAVE THE VALUE
    // asm: 	LDI	@BRAKEON,R0
    // asm: 	BNZ	GETBX		;BRAKE ALREADY ON
    // asm: 	CALL	_on_brake	;TURN ON FIRST TIME ONLY
    // asm: 	LDI	1,R0
    // asm: 	B	GETBXX
BRAKEOFF:
    // asm: 	PUSHF	R0		;SAVE THE VALUE
    // asm: 	LDI	@BRAKEON,R0
    // asm: 	BZ	GETBX	   	;ALREADY OFF DUDES
    // asm: 	CALL	_off_brake
    // asm: 	LDI	0,R0
GETBXX:
    // asm: STI	R0,@BRAKEON
GETBX:
    // asm: 	POPF	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETBRAKE", 0, 0);
    UNIMPL();
}

void _off_brake(void)
{
    // *
    // *TURN ON/OFF BRAKE LIGHTS
    // *
    // *PALSET- SETUP PALETTE TRANSFER
    // *AR2=SOURCE DATA ADDRESSS
    // *R2 =DEST PALETTE(B8-15), DEST COLOR(B0-7)
    // *R3 =COUNT
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	LDI	@BUTTON_STATUS,R2
    // asm: 	ANDN	BUT_TAILS,R2
    // asm: 	STI	R2,@BUTTON_STATUS
    // asm: 	LDI	@OFFREDI,AR2
    // asm: 	PUSH	AR2
    // asm: 	BU	L888
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_off_brake", 0, 0);
    UNIMPL();
}

void _on_brake(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	LDI	BUT_TAILS,R2
    // asm: 	OR	@BUTTON_STATUS,R2
    // asm: 	STI	R2,@BUTTON_STATUS
    // asm: 	LDI	@HIREDI,AR2
    // asm: 	PUSH	AR2
L888:
    // asm: LDI	@CHOOSENCAR,AR0
    // asm: 	MPYI	VEHTAB_SIZE,AR0
    // asm: 	ADDI	@VEHICLE_TABLEI,AR0
    // asm: 	LDI	*+AR0(VEHTAB_TAILCNT),R3
    // asm: 	BZ	NO_COLORS
    // asm: 	LDI	*+AR0(VEHTAB_PAL),AR2
#if DEBUG
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	$	;table entry not filled
#endif
    // asm: 	CALL	PAL_FIND
    // asm: 	ADDI	256,R0
    // asm: 	SUBI	R3,R0
    // asm: 	LDI	R0,R2
    // asm: 	POP	AR2
    // asm: 	CALL	PAL_SET
    // asm: BRAK_X	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
NO_COLORS:
    // asm: 	POP	AR2
    // asm: 	BU	BRAK_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_on_brake", 0, 0);
    UNIMPL();
}

void GETPEDAL(void)
{
    // *----------------------------------------------------------------------------
    // *GET GAS PEDAL
    // *RET R0=GAS PEDAL 0-1 FRACTION
    // asm: 	FLOATP	@_pot1,R0
    // asm: 	NEGF	@PEDALMN,R1
    // asm: 	ADDF	R1,R0
    // asm: 	CMPF	5,R0
    // asm: 	LDFLT	0,R0		;GET RID OF CREEP AND NEGATIVES
    // asm: 	ADDF	@PEDALMX,R1
    // asm: 	CALL	DIV_F
    // asm: 	CMPF	1.0,R0 		;KEEP IT IN RANGE
    // asm: 	LDFGT	1.0,R0
    // asm: 	RETS
    // *GET GEAR SHIFT
    // *AR4=CAR OBJECT
    // *AR5=CAR STRUCTURE
    // *RETURNS:
    // *R0=GEAR SHIFT VALUE 0=neutral,1,2,3,4  (INT)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETPEDAL", 0, 0);
    UNIMPL();
}

void GETGEAR(void)
{
    // asm: 	LDI	*+AR5(CARTRANS),R0
    // asm: 	BNZ	GETMAN
    // asm: 	LDI	@SUSPEND_MODE,R0	;SUSPEND MODE?
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BNZ	GETAUTO			;NO
    // asm: 	LDI	*+AR5(CARGEAR),R0	;YES, DONT SWITCH AUTO TRANS
    // asm: 	RETS
    // *MANUAL
GETMAN:
    // asm: 	LDI	0,R0
    // asm: 	LDI	@SWITCHBUTS,R1
    // asm: 	TSTB	SW_4TH,R1
    // asm: 	LDINZ	4,R0
    // asm: 	TSTB	SW_3RD,R1
    // asm: 	LDINZ	3,R0
    // asm: 	TSTB	SW_2ND,R1
    // asm: 	LDINZ	2,R0
    // asm: 	TSTB	SW_1ST,R1
    // asm: 	LDINZ	1,R0
    // asm: 	RETS
    // *AUTOMATIC
    // *AR4=CAR OBJECT
    // *AR5=CAR STRUCTURE
    // *RETURNS:
    // *R0=GEAR SHIFT VALUE 0=neutral,1,2,3,4  (INT)
GETAUTO:
    // asm: 	LDI	0,R2
    // asm: 	LDF	*+AR5(CARTHROTTLE),R3	;TORQUE SENSOR FOR DOWNSHIFT
    // asm: 	CMPF	1.0,R3
    // asm: 	LDFGT	1.0,R3
    // asm: 	MPYF	11,R3
    // asm: 	FIX	R3
    // asm: 	ADDI	12,R3
    // asm: 	LDI	*+AR5(CARGEAR),R0
    // asm: 	FIX	*+AR5(CARRPM),R1
    // asm: 	CMPI	R3,R1
    // asm: 	LDILT	-1,R2	 		;DOWNSHIFT
    // asm: 	ADDI	18,R3
    // asm: 	CMPI	R3,R1
    // asm: 	LDIGT	1,R2			;UPSHIFT
    // asm: 	ADDI	R2,R0
    // asm: 	CMPI	1,R0
    // asm: 	LDILT	1,R0
    // asm: 	CMPI	4,R0
    // asm: 	LDIGT	4,R0			;MAX OUT AT 4TH
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETGEAR", 0, 0);
    UNIMPL();
}

void GETSTEER(void)
{
    // *GET STEERING ANGLE
    // *AR5=CAR STRUCTURE
    // *RET R0= STEERING ANGLE
    // *	 SETS CARTURN VALUE
    // asm: 	LDI	ADJ_STEERING_SENSITIVITY,AR2		;GET DIFFICULTY	ADJUST
    // asm: 	CALL	ADJUSTMENT_READ		;R0  = 0-5 (int)
    // asm: 	FLOAT	R0,R4
    // asm: 	FLOAT	R0,R6
    // asm: 	SUBRF	5,R4
    // asm: 	MPYF	0.06,R4
    // asm: 	ADDF	0.5,R4
    // asm: 	LDF	R4,R5
    // asm: 	SUBRF	1.0,R5
    // asm: 	FLOAT	@_pot0,R0 		;GET POT VALUE
    // asm: 	SUBF	@STEERCT,R0		;SUBTRACT CENTER VALUE
    // asm: 	LDF	@STEERFR,R1
    // asm: 	MPYF	0.5,R1
    // asm: 	CALL	DIV_F
    // asm: 	CMPF	-1,R0 			;LIMIT CHECK
    // asm: 	LDFLT	-1,R0
    // asm: 	CMPF	1,R0 			;LIMIT CHECK
    // asm: 	LDFGT	1,R0
    // asm: 	MPYF	0.02,R6	    		;DESENSITIZE
    // asm: 	ADDF	0.90,R6
    // asm: 	MPYF	R6,R0
    // asm: 	ABSF	R0,R1			;SQUARE IT KEEPING SIGN
    // asm: 	MPYF	R0,R1
    // asm: 	MPYF	R4,R1			;RATIO OF STEERING VALUE SQUARED
    // asm: 	MPYF	R5,R0			;RATIO OF REGULAR
    // ;	MPYF	0.5,R1			;AVERAGE WITH REGULAR
    // ;	MPYF	0.5,R0
    // asm: 	ADDF	R1,R0
    // asm: 	LDF	R0,R1
    // asm: 	MPYF	-0.3,R1			;FUDGE FACTOR FOR WHEEL TURN ANGLE
    // asm: 	STF	R1,*+AR5(CARTURN)	;STORE WHEEL TURN VALUE
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm: 	LDF 	R1,R2
    // asm: 	MPYF	R1,R1
    // asm: 	MPYF	0.0125,R1
    // asm: 	CMPF	5,R1			;LIMIT LO SPEED KICK
    // asm: 	LDFGT	5,R1
    // asm: 	MPYF	0.0625,R2   	      	;HIGH SPEED KICK
    // asm: 	CMPF	15,R2
    // asm: 	LDFGT	15,R2
    // asm: 	ADDF	R2,R1
    // asm: 	MPYF	R1,R0
    // asm: 	FLOATP	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	MPYF	@STEERI,R0
    // asm: 	RETS
    // *
    // *VIEW SWITCHES
    // *ZOOM TO FIRST PERSON
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSTEER", 0, 0);
    UNIMPL();
}

void _VIEW0(void)
{
    // asm: 	FLOAT	PLYPOS1Z,R4	;Z DIST
    // asm: 	FLOAT	PLYPOS1Y,R5	;Y DIST
    // asm: 	LDI	0,R2		;CAMVIEW STATUS
    // asm: 	LDI	BUT_VIEW1,R3
    // asm:  	B	ZOOM
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_VIEW0", 0, 0);
    UNIMPL();
}

void _VIEW1(void)
{
    // asm: 	FLOAT	PLYPOS2Z,R4	;Z DIST
    // asm: 	FLOAT	PLYPOS2Y,R5	;Y DIST
    // asm: 	LDI	1,R2		;CAMVIEW STATUS
    // asm: 	LDI	BUT_VIEW2,R3
    // asm:  	B	ZOOM
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_VIEW1", 0, 0);
    UNIMPL();
}

void _VIEW2(void)
{
    // asm: 	FLOAT	PLYPOS3Z,R4	;Z DIST
    // asm: 	FLOAT	PLYPOS3Y,R5	;Y DIST
    // asm: 	LDI	2,R2		;CAMVIEW STATUS
    // asm: 	LDI	BUT_VIEW3,R3
    // *R2=VIEW (0-2)INT
    // *R3=BUTTON LITE MASK
    // *R4=PLYPOSZ FL
    // *R5=PLYPOSY FL
ZOOM:
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	SUICIDE	       		;DIE IF NOT IN GAME MODE
    // asm: 	CMPI	@CAMVIEW,R2
    // asm: 	BEQ	SUICIDE			;DIE IF SAME MODE
    // asm: 	LDILT	VIEW2IN,AR2
    // asm: 	LDIGE	VIEW2OUT,AR2
    // asm: 	CALL	ONESNDFX
    // asm: 	LDI	@BUTTON_STATUS,R0	;LITE YOUR LITE
    // asm: 	ANDN	BUT_VIEWS,R0
    // asm: 	OR	R3,R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	STF	R4,@ZOOMDG		;STORE GOAL X
    // asm: 	STF	R5,@ZOOMHG	 	;STORE GOAL Y
    // asm: 	SUBF	@ZOOMD,R4
    // asm: 	MPYF	ZOOMRATIO,R4
    // asm: 	SUBF	@ZOOMH,R5
    // asm: 	MPYF	ZOOMRATIO,R5
    // asm: 	LDF	1,R0
    // asm: 	ABSF	R4,R6
    // asm: 	CMPF	25,R6			;PUMP UP FOR LOW ZOOM RATE
    // asm: 	LDFLT	4,R0
    // asm: 	MPYF	R0,R4
    // asm: 	MPYF	R0,R5
    // asm: 	STF	R4,@ZOOMDD
    // asm: 	STF	R5,@ZOOMHD
    // asm: 	LDI	@CAMVIEW,R3		;CHANGE FROM FIRST PERSON ?
    // asm: 	STI	R2,@CAMVIEW		;SAVE NEW CAMERA VIEW
    // asm: 	BNZ	ZOOMX			;NO...
    // asm: 	LDI	@PLYCAR,AR2
    // asm: 	LDI	*+AR2(OFLAGS),R0	;CHECK IF ALREADY ON LIST
    // asm: 	TSTB	O_LIST_M,R0
    // asm: 	CALLZ	OBJ_INSERT		;INSERT PLAYER OBJECT
ZOOMX:
    // asm: 	CALL	ZOOMUP
    // asm: 	BR	SUICIDE	      		;CAN IT DUDES
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_VIEW2", 0, 0);
    UNIMPL();
}

void ZOOMUP(void)
{
    // *----------------------------------------------------------------------------
    // *UPDATE YOUR ZOOM JIVE
    // asm: 	LDF	@ZOOMDD,R2
    // asm: 	BZ	ZOOMUPX		;NO ACTIVE ZOOM, SKIP IT
    // asm: 	BND	ZOOMUP1
    // asm: 	LDF	@ZOOMH,R1
    // asm: 	ADDF	@ZOOMHD,R1
    // asm: 	ADDF	@ZOOMD,R2
    // 	;------->BND	ZOOMUP1
    // asm: 	CMPF	@ZOOMDG,R2
    // asm: 	BGE	ZOOMDN		;DONE WITH ZOOM
    // asm: 	B	ZOOMUP3
ZOOMUP1:
    // asm: 	CMPF	@ZOOMDG,R2
    // asm: 	BGT	ZOOMUP3		;NOT DONE WITH ZOOM
ZOOMDN:
    // asm: LDI	@CAMVIEW,R0	;FIRST PERSON DONE?
    // asm: 	BNZ	ZOOMDN1		;NO..
    // asm: 	LDI	@PLYCAR,AR2
    // asm: 	CALL	OBJ_PULL	    	;CAN THE CAR
ZOOMDN1:
    // asm: LDF	0,R0
    // asm: 	STF	R0,@ZOOMDD	;CLEAR OUT VELOCITIES
    // asm: 	STF	R0,@ZOOMHD
    // asm: 	LDF	@ZOOMDG,R2
    // asm: 	LDF	@ZOOMHG,R1
ZOOMUP3:
    // asm: STF	R1,@ZOOMH	;UPDATE POSITION
    // asm: 	STF	R2,@ZOOMD
ZOOMUPX:
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOMUP", 0, 0);
    UNIMPL();
}

void GETTRAK(void)
{
    // *----------------------------------------------------------------------------
    // *GETTRAK	GET NEAREST TRACK SECTION
    // *PARAMETERS
    // *	AR4	OBJECT
    // *	AR5	STRUCTURE
    // *RETURNS
    // *	AR0	TRACK SEGMENT STORED IN AR5(CARTRAK)
    // *CLOBBERS
    // *	AR2,R0,R3,R4
    // asm: 	LDI	0,AR0			;CLOSEST ROAD SEGMENT INDEX
    // asm: 	LDI	@DYNALIST_BEGIN,R0
    // asm: 	BZ	GETRKX  			;NULL LIST DUDES
    // asm: 	PUSH	IR0
    // asm: 	LDI	OPOSZ,IR0
    // asm: 	FLOAT	7FFFH,R2		;INITIAL CLOSEST DISTANCE
    // asm: 	MPYF	R2,R2
    // asm: 	LDF	*+AR4(OPOSX),R3
    // asm: 	LDF	*+AR4(OPOSZ),R4
    // asm: 	LDI	R0,AR2
GETRK:
    // asm: 	SUBF	*+AR2(OPOSX),R3,R0
    // asm: 	MPYF	R0,R0
    // asm: 	SUBF	*+AR2(IR0),R4,R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	R1,R2
    // asm: 	LDIGT	AR2,AR0
    // asm: 	LDFGT	R1,R2
    // asm: GETRKL
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	BNZD	GETRK
    // asm: 	LDI	R0,AR2
    // asm: 	NOP
    // asm: 	NOP
    // 	;---->	BNZ	GETRK
    // asm: 	STI	AR0,*+AR5(CARTRAK)	;SAVE TRACK SECTION
    // asm: 	POP	IR0
GETRKX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTRAK", 0, 0);
    UNIMPL();
}

void CKBND(void)
{
    // *----------------------------------------------------------------------------
    // *CHECK OUT OF BOUNDS
    // *PARAMETERS
    // *	AR4	CAR
    // *	AR5	CAR DATA BLOCK
    // *RETURNS
    // *	CS IF ANY WHEEL OUT OF BOUNDS
    // *	NC IN BOUNDS
    // *R0	0 = ONROAD
    // *	NE= OFFROAD
    // *TRASHES R0,R1,RC
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR0
    // asm: 	LDI	CARVNUM-2,RC	  	;CHECK ALL WHEELS
    // asm: 	LDI	AR5,AR3
    // asm: 	ADDI	CARPCOL+CARVSIZ,AR3
    // asm: 	LDI	0,R1
    // asm: 	RPTB	CURBCKL
    // asm: 	LDI	*AR3++(CARVSIZ),R0 	;GET ROAD OBJECT INTERSECTING
    // asm: 	BZ	CURBCKX			;WE GOT NOTHING, COLLIDE 'EM
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OID),R0		;CHECK OID
    // asm: 	AND	CLASS_M,R0
    // asm: 	CMPI	ROAD_C,R0		;ROAD/SHOULDER TYPE?
CURBCKL:
    // asm: LDINZ	1,R1
    // asm: 	CLRC
    // asm: 	POP	AR0
    // asm: 	POP	AR3
    // asm: 	LDI	R1,R1
    // asm: 	RETS
CURBCKX:
    // asm: 	SETC
    // asm: 	POP	AR0
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKBND", 0, 0);
    UNIMPL();
}

void TUNCHK(void)
{
    // *----------------------------------------------------------------------------
    // *CHECK CRAWL UP TUNNEL WALLS
    // *PARAMETERS
    // *	AR4	CAR OBJECT
    // *	AR5	CAR BLOCK
    // asm: 	LDI	@_MODE,R0     		;IN TUNNEL?
    // asm: 	TSTB	MINTUNNEL,R0
    // asm: 	RETSZ				;NO, EXIT
    // asm: 	LDI	@WRECKFLG,R0     	;WRECK ON?
    // asm: 	RETSNZ				;NO, EXIT
    // asm: 	LDF	*+AR4(OMAT11),R0	;PLAYER TOO STEEP?
    // asm: 	CMPF	0.50,R0
    // asm: 	RETSGT				;NO...
    // asm: 	CALL	GETNXTRDIR		;YES, SPIN HIM TO CENTER
    // asm: 	LDF	R0,R2
    // asm: 	CALL	ROADIR
    // asm: 	ADDF	R0,R2
    // asm: 	MPYF	0.5,R2
    // asm: 	CALL	NORMITS
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	CALL	ROADIR
    // asm: 	SUBF	*+AR5(CARYROT),R0
    // asm: 	LDF	R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R2,R0
    // asm: 	LDF	15,R1
    // asm: 	CALL	DIV_F
    // asm: 	STF	R0,*+AR5(CARDROT)    	;ROTATE TOWARD ROAD SLOWLY
    // asm: 	LDI	15,R0
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
    // asm:  	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARSPRAD)
    // asm: 	LDF	*+AR5(CARSPEED),R0
    // asm: 	CMPF	40,R0
    // asm: 	LDFLT	40,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // asm: TUNCHKX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TUNCHK", 0, 0);
    UNIMPL();
}

void INBOUNDZ(void)
{
    // *----------------------------------------------------------------------------
    // *CHECK CURB COLLISION
    // *KEEP ON ROAD
    // *PARAMETERS
    // *	AR4	CAR
    // *	AR5	CAR DATA BLOCK
    // asm: 	LDI	@_MODE,R1		;WAITING FOR START?
    // asm: 	TSTB	MGO,R1
    // asm: 	BZ	CURBCLX			;YES, EXIT
    // asm: 	CALL	TUNCHK			;KEEP DUDE OFF TUNNEL WALLS
    // asm: 	CALL	CKBND
    // asm: 	BC	CURBCOL0		;OUT OF BOUNDS
    // asm: 	BNE	SOFTCURB		;SOFT REPELL OFFROAD
    // asm: CURBCKLX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INBOUNDZ", 0, 0);
    UNIMPL();
}

void DRONINBZ(void)
{
    // *----------------------------------------------------------------------------
    // *KEEP DRONE INBOUNDS
    // *PARAMETERS
    // *	AR4	CAR
    // *	AR5	CAR DATA BLOCK
    // asm:  	LDI	*+AR4(ODIST),R0
    // asm: 	ASH	-1,R0
    // asm: 	CMPI	32000,R0		;IS DUDE CLOSE?
    // asm: 	BGT	DRONINBX		;NO, FORGET IT
    // asm: 	LDI	@_MODE,R1		;WAITING FOR START?
    // asm: 	TSTB	MGO,R1
    // asm: 	BZ	DRONINBX		;YES, EXIT
    // asm: 	CALL	CKBND
    // asm: 	BC	CURBCOL0		;OUT OF BOUNDS
DRONINBX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONINBZ", 0, 0);
    UNIMPL();
}

void CURBCOL0(void)
{
    // *----------------------------------------------------------------------------
    // *WE HIT A CURB DUDES
    // *HARD WALL REFLECT SPIN-OUT
    // asm: 	LDI	*+AR4(OID),R0		;PLAYER?
    // asm: 	AND	CLASS_M,R0
    // asm: 	CMPI	PLYR_C,R0
    // asm: 	BZ	CURBCOLP		;YES
    // *DRONE HIT SOUND
    // asm: 	LDI	@WALLHITABI,AR2		;MAKE A DRONE SOUND
    // asm: 	LDI	3,R0
    // asm: 	CALL	DRONESND
    // asm: 	LDF	*+AR5(CARSPEED),R0	;SLOW DOWN DRONE
    // asm: 	MPYF	0.80,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // asm: 	B	CURBCOL
    // *PLAYER HIT SOUND
CURBCOLP:
    // asm: 	LDI	@WALLHITABI,AR2		;MAKE A SOUND
    // asm: 	LDI	3,R0
    // asm: 	CALL	RANDSND
CURBCOL:
    // asm: 	CALL	GETNXTRDIR    		;DIRECTION CAR TO NEXT ROAD SEG.
    // asm: 	LDF	R0,R4
    // asm: 	CALL	ROADIR			;REFLECT VELOCITY
    // asm: 	SUBF	R4,R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R2,R3
    // asm: 	LDF	*+AR5(CARVROT),R1
    // asm: 	SUBF	R1,R0,R2
    // asm: 	CALL	NORMITS
    // *KICK DUDE BACK	INSTANTLY
    // *R0=ROADIR
    // *R2=ROADIR-CARVROT
    // asm: 	PUSHF	R2
    // asm: 	PUSHF	R0
    // asm: 	LDF	*+AR5(CARSPEED),R2
    // asm: 	CMPF	15,R2
    // asm: 	LDFLT	15,R2
    // asm: 	STF	R2,*+AR5(CARSPEED)	;MIN SPEED
    // asm: 	FLOATP	@NFRAMES,R5
    // asm: 	MPYF 	R2,R5			;TOTAL DISTANCE TRAVELED
    // asm: 	LDF	-1.57,R4
    // asm: 	LDF	R3,R3
    // asm: 	LDFN	1.57,R4
    // asm: 	ADDF	R4,R0,R2		;REPELL DIRECTION
    // asm: 	PUSHF	R2
    // asm: 	SUBF	R1,R2			;FIND DIFFERENCE
    // asm: 	CALL	_SINE
    // asm: 	ABSF	R0
    // asm: 	MPYF	R0,R5			;ADJUST VELOCITY FOR PENETRATION ANGLE
    // asm: 	POPF	R2
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R0,R5,R4
    // asm: 	CALL	_SINE
    // asm: 	NEGF	R0			;-SIN
    // asm: 	MPYF	R0,R5,R5
    // asm: 	ADDF	*+AR4(OPOSX),R5		;ADJUST X
    // asm: 	STF	R5,*+AR4(OPOSX)
    // asm: 	ADDF	*+AR4(OPOSZ),R4		;ADJUST Z
    // asm: 	STF	R4,*+AR4(OPOSZ)
    // asm: 	CALL	WALL_SPARK
    // asm: 	POPF	R0
    // asm: 	POPF	R2
    // *REFLECT VELOCITY
    // asm: 	XOR	R2,R3			;CHECK IF ALREADY GOING THE RIGHT WAY
    // asm: 	BN	CURBCOL1		;MOVING IN RIGHT DIRECTION
    // asm: 	NEGF	R2			;NEED TO REFLECT VELOCITY
CURBCOL1:
    // asm: 	LDI	*+AR5(CAR_SPIN),R1  	;ALREADY SPINNING?
    // asm: 	BNZ	CURBSPIN		;YES, SPIN SOME MORE...
    // asm: 	ABSF	R2,R3
CURBCOL1A:
    // asm: 	CMPF	1.2,R3
    // asm: 	BGT	CURBSPIN		;YES, SPIN THE DUDE
    // asm: 	CMPF	0.4,R3			;HARD BOUNCE?
    // asm: 	BGT	CURBSPN			;YES, SPIN THE DUDE
    // asm: 	MPYF	0.3,R2			;CUT DOWN BOUNCE
    // asm: 	MPYF	0.3,R3			;CUT DOWN BOUNCE
    // asm: 	CMPF	0.03,R3			;MINIMUM KICKOUT
    // asm: 	BGE	CURBCOL2A
    // asm: 	LDF	R2,R2
    // asm: 	LDFLT	-0.03,R2
    // asm: 	LDFGT	0.03,R2
CURBCOL2A:
    // asm: 	CMPF	0.06,R3			;MAXIMUM KICKOUT
    // asm: 	BLE	CURBCOL2
    // asm: 	LDF	R2,R2
    // asm: 	LDFLT	-0.06,R2
    // asm: 	LDFGT	0.06,R2
CURBCOL2:
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	STF	R2,*+AR5(CARYROT)	;ADJUST YROT <--- CARVROT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CURBCOL0", 0, 0);
    UNIMPL();
}

void CURBSPIN(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDF	R0,R2
    // asm: 	SUBF	*+AR5(CARYROT),R2	;CHECK YROT-ROAD DIRECTION
    // asm: 	CALL	NORMITS
    // asm: 	LDF	0.025,R0 		;RANDOM ROTATION VALUE
    // asm: 	CALL	FRAND
    // asm: 	ADDF	0.075,R0
    // asm: 	LDF	R2,R2			;CHECK SIGN
    // asm: 	BN	CURBSPIN1
    // asm: 	NEGF	R0
CURBSPIN1:
    // asm: 	STF	R0,*+AR5(CARDROT)	;BODY DELTA
    // asm: 	ABSF	R2			;CORRECTION FACTOR
    // asm: 	ADDF	0.1,R2
    // asm: 	LDF	R2,R1
    // asm: 	CMPF	1.0,R2			;HARD HIT?
    // asm: 	LDFGT	3.14,R1			;YES...
    // asm: 	LDF	*+AR5(CARSPEED),R0
    // asm: 	CMPF	30,R0
    // asm: 	LDFLT	R2,R1
    // asm: 	STF	R1,*+AR5(CARSPRAD) 	;GO AROUND AT LEAST HALFWAY
    // asm: 	LDI	1,R1
    // asm: 	STI	R1,*+AR5(CAR_SPIN)
CURBCLX:
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CURBSPIN", 0, 0);
    UNIMPL();
}

void CURBSPN(void)
{
    // *----------------------------------------------------------------------------
    // *SHORT CORRECTION SPIN
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDF	R0,R2
    // asm: 	SUBF	*+AR5(CARYROT),R2	;CHECK YROT-ROAD DIRECTION
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2,R3
    // asm: 	STF	R3,*+AR5(CARSPRAD) 	;SPIN THIS MUCH DUDES
    // asm: 	MPYF	0.10,R2
    // asm: 	STF	R2,*+AR5(CARDROT)	;BODY DELTA
    // asm: 	LDI	1,R1
    // asm: 	STI	R1,*+AR5(CAR_SPIN)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CURBSPN", 0, 0);
    UNIMPL();
}

void SOFTCURB(void)
{
    // *----------------------------------------------------------------------------
    // *SOFT CURB HIT
    // *
    // asm: 	CALL	ROADIR
    // asm: 	LDF	R0,R1
    // asm: 	CALL	GETNXTRDIR    		;DIRECTION CAR TO NEXT ROAD SEG.
    // asm: 	SUBF	R1,R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R2,R4			;CHECK DIRECTION
    // asm: 	LDFN	-0.1,R5			;GET RELATIVE ANGLE
    // asm: 	LDFNN	0.1,R5
    // asm: 	LDI	*+AR5(CAR_SPIN),R0  	;ALREADY SPINNING?
    // asm: 	BZ	SOFTCURB0		;NO
    // asm: 	CMPI	2,R0			;TIMED SPIN?
    // asm: 	BGE	SOFTCRB00		;YES
    // asm: 	LDF	*+AR5(CARVROT),R0	;REFLECT THE MOTHER OUT
    // asm: 	SUBF	R1,R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2
    // asm: 	CMPF	0.1,R2	      		;MINIMUM REFLECT
    // asm: 	LDFLT	0.1,R2
    // asm: 	LDF	R4,R4
    // asm: 	LDFN	-1,R5			;GET RELATIVE ANGLE
    // asm: 	LDFNN	1,R5
    // asm: 	MPYF	R5,R2
    // asm: 	ADDF	R1,R2
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	RETS
SOFTCRB00:
    // asm: 	LDF	*+AR5(CARSPEED),R0	;TIMED SPIN
    // asm: 	CMPF	80,R0
    // asm: 	BLT	SOFTVELX		;SLOW TREECOL FIX
    // asm: 	B	SOFTVEL			;FAST, NEEDS CORRECTION
    // *CHECK BODY ROTATION
SOFTCURB0:
    // asm: 	LDF	*+AR5(CARYROT),R0
    // asm: 	SUBF	R1,R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R4,R4
    // asm: 	BN	SOFT10	     		;DELTA IS NEGATIVE
    // asm: 	SUBF	R2,R5,R6
    // asm: 	BLT	SOFTVEL			;ROTATION IS O.K.
    // asm: 	CMPF	0.1,R6			;MAX DELTA
    // asm: 	LDFGT	0.1,R6
    // asm: 	B 	SOFT11
SOFT10:
    // asm: 	SUBF	R2,R5,R6
    // asm: 	BGT	SOFTVEL			;ROTATION IS O.K.
    // asm: 	CMPF	-0.1,R6			;MAX DELTA
    // asm: 	LDFLT	-0.1,R6
SOFT11:
    // asm: 	ADDF	*+AR5(CARYROT),R6  	;ADD IN ROTATION
    // asm: 	STF	R6,*+AR5(CARYROT)
    // *CHECK VELOCITY ROTATION
    // asm: SOFTVEL	LDF	*+AR5(CARSPEED),R0	;MINIMUM SPEED
    // asm: 	CMPF	20,R0
    // asm: 	LDFLT	20,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // asm: 	LDF	*+AR5(CARVROT),R0
    // asm: 	SUBF	R1,R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2,R3
    // asm: 	CMPF	0.6,R3
    // asm: 	BLT	SOFTV1
    // asm: 	LDF	R1,R0			;ROADIR->R0
    // asm: 	LDF	*+AR5(CARVROT),R1
    // asm: 	SUBF	R1,R0,R2
    // asm: 	B	CURBCOL1A
    // asm: SOFTV1	LDF	R4,R4
    // asm: 	BN	SOFT20	     		;DELTA IS NEGATIVE
    // asm: 	SUBF	R2,R5
    // asm: 	BLT	SOFTVELX		;ROTATION IS O.K.
    // asm: 	CMPF	0.1,R5			;MAX DELTA
    // asm: 	LDFGT	0.1,R5
    // asm: 	B 	SOFT21
    // asm: SOFT20	SUBF	R2,R5
    // asm: 	BGT	SOFTVELX		;ROTATION IS O.K.
    // asm: 	CMPF	-0.1,R5			;MAX DELTA
    // asm: 	LDFLT	-0.1,R5
    // asm: SOFT21	ADDF	*+AR5(CARVROT),R5  	;ADD IN ROTATION
    // asm: 	STF	R5,*+AR5(CARVROT)
SOFTVELX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SOFTCURB", 0, 0);
    UNIMPL();
}

void GETNXTRDIR(void)
{
    // *----------------------------------------------------------------------------
    // *GET DIRECTION TO NEXT ROAD SEGMENT FROM CAR
    // *
    // asm: 	LDI	*+AR5(CARTRAK),AR2	;GET CLOSEST TRACK PIECE
    // asm: 	LDI	*+AR2(OLINK4),AR0	;GET NEXT ONE
    // asm: 	LDI	AR4,AR2
    // asm: 	B 	GETRD1
    // *GET ROAD-CAR ANGLE
    // *AR2=ROAD SEGMENT OBJECT
    // *AR4=CAR OBJECT
    // *RET R0 = RADIANS (FLOAT)
    // *CLOBBERS AR0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETNXTRDIR", 0, 0);
    UNIMPL();
}

void GETRDCAR(void)
{
    // asm: 	LDI	AR4,AR0
    // asm: 	B 	GETRD1
    // *GET ROAD DIRECTION
    // *AR5=CARBLOCK
    // *RET R0 = RADIANS (FLOAT)
    // *CLOBBERS AR0,AR2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETRDCAR", 0, 0);
    UNIMPL();
}

void ROADIR(void)
{
    // asm: 	LDI	*+AR5(CARTRAK),R0	;GET CLOSEST TRACK PIECE
    // asm: 	BZ	ROADIRX			;NO TRACK PIECE, EXIT
    // asm: 	LDI	R0,AR2
    // *GET ROAD DIRECTION
    // *AR2=ROAD SEGMENT OBJECT
    // *RET R0 = RADIANS (FLOAT)
    // *CLOBBERS AR0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADIR", 0, 0);
    UNIMPL();
}

void GETRDIR(void)
{
    // asm: 	LDI	*+AR2(OLINK4),AR0
GETRD1:
    // asm: PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSHF	R2
    // asm: 	PUSHF	R3
    // asm: 	LDF	*+AR0(OPOSX),R2
    // asm: 	SUBF	*+AR2(OPOSX),R2
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	SUBF	*+AR2(OPOSZ),R3
    // asm: 	CALL	ARCTANF
    // asm: 	POPF	R3
    // asm: 	POPF	R2
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	SUBF	HALFPI,R0
ROADIRX:
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETRDIR", 0, 0);
    UNIMPL();
}

void PLYRWHL(void)
{
    // *PLAYER WHEEL ROUTINE
    // *PARAMETERS
    // *	AR4	OBJECT
    // *	AR5	CAR STRUCTURE
    // asm: 	LDF	@STEERCT,R4		;STEERING CENTER
    // asm: 	LDI	@WHLTIM,R3
    // asm: 	LDI	@WHLOLD,R5
    // asm: 	LDI	0,R2
    // asm: 	LDI	0,R1
    // asm: 	LDI	*+AR5(RF_PCOL),R0	;Right front collide with curb?
    // asm: 	BZ	PWHL0
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	AND	CLASS_M+TYPE_M,R0
    // asm: 	CMPI	ROAD_C+SHLDR_T,R0
    // asm: 	LDIZ	1,R1
PWHL0:
    // asm: LDI	*+AR5(LF_PCOL),R0	;Left front collide with curb?
    // asm: 	BZ	PWHL1
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	AND	CLASS_M+TYPE_M,R0
    // asm: 	CMPI	ROAD_C+SHLDR_T,R0
    // asm: 	LDIZ	2,R2
PWHL1:
    // asm: ADDI	R1,R2
    // asm: 	STI	R2,@WHLOLD
    // asm: 	BZ	PWHLX			;NO SHOULDER ACTIVE
    // asm: 	ANDN	R5,R2			;NEW SHOULDER HIT?
    // asm: 	BZ	PWHLX			;NOT NEW...
    // *COLLIDED WITH A CURB FIRST TIME
    // asm: 	CALL	ROADIR			;FIND WHICH SIDE OF ROAD HIT
    // asm: 	LDF	R0,R2
    // asm: 	CALL	GETNXTRDIR
    // asm: 	SUBF	R0,R2
    // asm: 	CALL 	NORMITS
    // asm: 	LDF	R2,R2
    // asm: 	LDFNN	15,R0
    // asm: 	LDFN	-15,R0
    // asm: 	ADDF	R0,R4			;NEW STERRING CENTER FOR FEEDBACK
    // asm: 	LDI	4,R3			;NEW TIMER VALUE
    // asm: 	B	PWHLX0
PWHLX:
    // asm: SUBI	1,R3
    // asm: 	LDILT	0,R3
    // asm: 	BGT	PWHLXX			;When timer reaches -1, Set WHEELPOS = STEERCT
    // *THIS POINT IS ONLY REACHED WHEN NOT COLLIDING WITH A CURB
    // *PLACE SKID CORRECT HERE
PWHLX0:
    // asm: STF	R4,@WHEELPOS
PWHLXX:
    // asm: STI	R3,@WHLTIM
    // *CHECK OFF ROAD STUFF
    // asm: 	LDI	@WHLOLD,R1  		;OFFROAD?
    // asm: 	BZ	WHLOFFX			;NO
    // asm: 	LDI	@WHLTIM,R3		;TIMER ON?
    // asm: 	BNZ	WHLOFFX			;NO
    // asm: 	LDF	@STEERCT,R4		;STEERING CENTER
    // asm: 	LDF	R4,R5
    // asm: 	ADDF	15,R4
    // asm: 	SUBF	15,R5
    // asm: 	LDF	2,R0
    // asm: 	CMPI	3,R1
    // asm: 	LDFZ	4,R0
    // asm: 	CALL	SFRAND			;If off road offset center by random number
    // asm: 	ADDF	@WHEELPOS,R0
    // asm: 	CMPF	R4,R0		 	;CHECK YOUR LIMITS
    // asm: 	LDFGT	R4,R0
    // asm: 	CMPF	R5,R0
    // asm: 	LDFLT	R5,R0
    // asm: 	STF	R0,@WHEELPOS
WHLOFFX:
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRWHL", 0, 0);
    UNIMPL();
}

void PLYR_SNDS(void)
{
    // ;SHIFTSNDTABI	.WORD	SHIFTSNDTAB
    // ;SHIFTSNDTAB	.WORD	FIRSTSND,SECONDSND,THIRDSND,FOURTHSND
    // *----------------------------------------------------------------------------
    // *PLAYER SOUND EFFECTS TRACKS
    // *TRACK 0 = MUSIC
    // *TRACK 1 = DRONE+PLAYER EFFECTS
    // *TRACK 2 = DRONE+PLAYER EFFECTS
    // *TRACK 3 = ENGINE
    // *PARAMETERS
    // *	AR4	PLYR CAR OBJECT
    // *	AR5	CAR BLOCK
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	RETSNE
    // asm: 	CALL	GETPEDAL
    // asm: 	CMPF	0.75,R0
    // asm: 	BLT	PSND1
    // asm: 	LDI	@_MODE,R2
    // asm: 	AND	MSLINE,R2
    // asm: 	BZ	PSND0
    // asm: 	CREATEC	FLAME_PRC,UTIL_C	;make child flames
PSND0:
    // asm: 	LDI	@REVFLG,R0
    // asm: 	BNE	BACKREV
    // asm: 	CREATEC	FLAME_PRC,UTIL_C	;make child flames
    // *MAKE YOUR REV SOUND
    // *KILL OLDIES
    // asm: 	LDI	2,R0
    // asm: 	LDI	@REVSNDTABI,AR2 	;REVSND ON TRACK 12
    // asm: 	CALL	RANDSND
    // asm: 	LDI	1,R0
    // asm: 	B	PSND2
PSND1:
    // asm: 	CMPF	0.50,R0
    // asm: 	LDIGE	@REVFLG,R0
    // asm: 	LDILT	0,R0
PSND2:
    // asm: 	STI	R0,@REVFLG
    // *CHECK BACKGROUND STARTING LINE REV SOUNDS
BACKREV:
    // asm: 	LDI	@_MODE,R2 		;STARTING LINE MODE?
    // asm: 	AND	MSLINE,R2
    // asm: 	BZ	PLAIR			;NOPE
    // asm: 	LDI	@SNDSTR+SND_SIZ+SND_PRI,R2	;CHECK TRACK1
    // asm: 	BZ	BACKREV1
    // asm: 	LDI	@SNDSTR+(2*SND_SIZ)+SND_PRI,R2	;CHECK TRACK2
    // asm: 	BNZ	PLAIR
BACKREV1:
    // asm: 	SONDFX	STARTLINEREVS2		     	;START REV WHEN NOTHING
    // *CHECK IN AIR SPEECH CALL
PLAIR:
    // *CHECK IN AIR SPEECH CALL
    // asm: 	LDI	@WRECKFLG,R0			;IGNORE AIR SOUND ON WRECK
    // asm: 	BNZ	PLYRSND1B
    // asm: 	LDI	@PLAIRTIM,R0
    // asm: 	BNZ	PLYRSND1A
    // asm: 	LDF	*+AR5(CT_PRDYD),R0
    // asm: 	CMPF	200,R0
    // asm: 	BLT	PLYRSND1B
    // asm: 	LDI	@PLAIRSNDI,AR2
    // asm: 	LDI	3,R0
    // asm: 	CALL	RANDSND
    // asm: 	LDI	100,R0
PLYRSND1A:
    // asm: 	SUBI	1,R0
    // asm: 	STI	R0,@PLAIRTIM
PLYRSND1B:
    // *CHECK BOTTOM OUT SOUND
    // asm: 	LDI	*+AR5(CAR_AIRB),R0
    // asm: 	BNZ	NOBOTTOM
    // asm: 	LDI	@OLDPLYAIR,R0
    // asm: 	BZ	BOTX
    // asm: 	CALL	INIT_SPARK
    // asm: 	SONDFX	BOTTOMOUT
    // asm: 	LDI	0,R0
NOBOTTOM:
    // asm: 	STI	R0,@OLDPLYAIR
BOTX:
    // *CHECK SKID SOUND
    // asm: 	LDF	*+AR5(CARSKID),R0
    // asm: 	CMPF	0.5,R0
    // asm: 	BLT	NO_SMOKE
    // asm: 	CREATEC	SMOKE_PROC,UTIL_C	;make child smoke
NO_SMOKE:
    // asm: 	LDF	*+AR5(CARSKID),R0
    // asm: 	CMPF	0.25,R0
    // asm: 	BLT	NOSKID			;NO SKID ACTIVE
    // asm: 	FLOAT	115,R1
    // asm: 	MPYF	R0,R1			;GET SKID SOUND AMPLITUDE
    // asm: 	FIX	R1			;CONVERT TO INT
    // asm: 	ADDI	140,R1
    // *CHECK ALREADY ACTIVE
    // asm: 	LDI	@SNDSTR+SND_SIZ+SND_IDX,R2	;CHECK TRACK1
    // asm: 	CMPI	SKIDB,R2
    // asm: 	BEQ	SKIDAMP				;ALREADY SKIDDING
    // asm: 	CMPI	SKIDC,R2
    // asm: 	BEQ	SKIDAMP				;ALREADY SKIDDING
    // asm: 	LDI	@SNDSTR+(2*SND_SIZ)+SND_IDX,R2	;CHECK TRACK2
    // asm: 	CMPI	SKIDB,R2
    // asm: 	BEQ	SKIDAMP1		  	;ALREADY SKIDDING
    // asm: 	CMPI	SKIDC,R2
    // asm: 	BEQ	SKIDAMP1			;ALREADY SKIDDING
    // *MAKE NEW SKID SOUND
    // asm: 	LDF	*+AR5(CARSPEED),R0
    // asm: 	CMPF	20,R0
    // asm: 	BGT	NO_FLAME
    // asm: 	CREATEC	FLAME_PRC,UTIL_C	;make child flames
NO_FLAME:
    // asm: 	LDI	@SKIDTABI,AR2
    // asm: 	LDI	2,R0
    // asm: 	CALL	RANDVSND   	       		;START A SKID AND EXIT
    // asm: 	B	SKIDX
SKIDAMP:
    // asm: 	LDI	@SNDSTR+(SND_SIZ)+SND_VOL,R2	;VOLUME CHANGE?
    // asm: 	LDI	1,R0
    // asm: 	B	SKIDAMP10
SKIDAMP1:
    // asm: 	LDI	@SNDSTR+(2*SND_SIZ)+SND_VOL,R2	;VOLUME CHANGE?
    // asm: 	LDI	2,R0
SKIDAMP10:
    // asm: 	SUBI	R1,R2
    // asm: 	ABSI	R2
    // asm: 	CMPI	10,R2				;NOT ENOUGH CHANGE FOR CALL
    // asm: 	BLT	SKIDX
    // asm: 	CALL	SET_TRACK_VOL 			;ADJUST YOUR VOLUME, DUDES
    // asm: 	B	SKIDX
NOSKID:
    // asm: 	LDI	SKIDB,AR2
    // asm: 	CALL	KILLSNDFX
    // asm: 	LDI	SKIDC,AR2
    // asm: 	CALL	KILLSNDFX
SKIDX:
    // *CHECK HARD BRAKE SOUND
    // asm: 	LDF	*+AR5(CARBRAKE),R0
    // asm: 	CMPF	0.8,R0
    // asm: 	BLE	NOBRAKSND
    // asm: 	LDF	*+AR5(CARSPEED),R0     	;SPEED LOW  ?
    // asm: 	CMPF	1.5,R0
    // asm: 	BLT	NOBRAKSND	       	;YES KILL SOUND
    // asm: 	CREATEC	SMOKE_PROC,UTIL_C	;make child smoke
    // asm: 	LDI	BRAKSND,AR2
    // asm: 	CALL	MKFXSND
    // asm: 	B	BRAKSNDX
NOBRAKSND:
    // asm: 	LDI	BRAKSND,AR2
    // asm: 	CALL	KILLSNDFX
BRAKSNDX:
    // *ENGINE SPUTTER SOUND
    // asm: 	LDF	*+AR5(CARTHROTTLE),R0
    // asm: 	CMPF	0.15,R0
    // asm: 	BGE	NOSPUTSND
    // asm: 	LDF	*+AR5(CARRPM),R0
    // asm: 	CMPF	30,R0
    // asm: 	BLE	NOSPUTSND
    // asm: 	LDI	@_MODE,R2
    // asm: 	AND	MSLINE,R2
    // asm: 	BNZ	NOSPUTSND
    // asm: 	LDI	SPUTSND,AR2
    // asm: 	CMPI	@SNDSTR+2*(SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm: 	BZ	SPUTSNDX
    // asm: 	CALL	ONESND
    // asm: 	B	SPUTSNDX
NOSPUTSND:
    // asm: 	LDI	SPUTSND,AR2
    // asm: 	CALL	KILLSNDFX
SPUTSNDX:
    // *ROAD EFFECTS: TUNNEL SOUND
    // asm: 	LDI	@_MODE,R4
    // asm: 	TSTB	MINTUNNEL,R4
    // asm: 	BZ	TUNOFF
    // asm: 	FIX	*+AR5(CARSPEED),R0
    // asm: 	CMPI	100,R0		 		;CLAMP TO LIMITS
    // asm: 	LDILT	100,R0
    // asm: 	CMPI	255,R0
    // asm: 	LDIGT	255,R0
    // asm: 	LDI	TUNSND,AR2
    // asm: 	CALL	MKVFXSND
    // asm: 	B	TUNSNDX
TUNOFF:
    // asm: LDI	TUNSND,AR2
    // asm: 	CALL	KILLSNDFX
TUNSNDX:
    // *ROAD EFFECTS: GRAVEL SOUND
    // asm: 	LDI	*+AR5(LR_PCOL),AR2
    // asm: 	LDI	0,R1
    // asm: 	LDI	0,R2
    // asm: 	LDI	*+AR2(OID),R0
    // asm: 	CMPI	300h,R0
    // asm: 	LDINZ	127,R1
    // asm: 	LDI	*+AR5(RR_PCOL),AR2
    // asm: 	LDI	*+AR2(OID),R0
    // asm: 	CMPI	300h,R0
    // asm: 	LDINZ	127,R2
    // asm: 	ADDI	R1,R2,R0
    // asm: 	BZ	NOGRAV
    // asm: GRAVEL
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm: 	CMPF	1.0,R1
    // asm: 	BLT	NOGRAV
    // asm: 	LDI	GRAVELA,AR2
    // asm: 	B	MKVFXSND
NOGRAV:
    // asm: 	LDI	GRAVELA,AR2
    // asm: 	CALL	KILLSNDFX
    // asm: 	BNC	GRAVX
    // asm: 	SONDFX  BOTTOMOUT
GRAVX:
    // asm: PLSNDX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_SNDS", 0, 0);
    UNIMPL();
}

void MKFXSND(void)
{
    // *----------------------------------------------------------------------------
    // *MAKE PLAYER EFFECTS SOUND
    // *PARAMETERS
    // *	AR2	SOUND
    // asm: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm: 	RETSZ
    // asm: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm: 	BNZ	ONESNDFX
    // asm: MKFXSNDX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MKFXSND", 0, 0);
    UNIMPL();
}

void MKVFXSND(void)
{
    // *----------------------------------------------------------------------------
    // *MAKE PLAYER VOLUME EFFECTS BACKGROUND SOUND
    // *PARAMETERS
    // *	AR2	SOUND
    // *	 R0	VOLUME
    // asm: 	LDI	R0,R1				;SAVE VOLUME
    // asm: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm: 	BNZ	MKVFX1
    // asm: 	SUBPI	@SNDSTR+SND_SIZ+SND_VOL,R0	;CHECK TRACK1 VOLUME
    // asm: 	ABSI	R0
    // asm: 	CMPI	8,R0
    // asm: 	RETSLE
    // asm: 	LDI	1,R0	  	;TRACK #
    // asm: 	B	SET_TRACK_VOL	;R0=TRACK#,R1=VOL
MKVFX1:
    // asm: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm: 	BNZ	VOLSNDFX		     	;DO NEW SOUND DUDES
    // asm: 	SUBI	@SNDSTR+(2*SND_SIZ)+SND_VOL,R0	;CHECK TRACK2 VOLUME
    // asm: 	ABSI	R0
    // asm: 	CMPI	8,R0
    // asm: 	RETSLE
    // asm: 	LDI	2,R0	  	;TRACK #
    // asm: 	B	SET_TRACK_VOL	;R0=TRACK#,R1=VOL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MKVFXSND", 0, 0);
    UNIMPL();
}

void RANDSND(void)
{
    // *----------------------------------------------------------------------------
    // *RANDOM SOUND ROUTINE
    // *PARAMETERS
    // *	R0	RANDOM RANGE (0->R0-1)
    // *	AR2	TABLE ADDR	OF SOUNDS
    // *TRASHED R0
    // asm: 	PUSH	AR2
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	RANDU0
    // asm: 	POP	AR2
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	B	ONESNDFX
    // *RANDOM VOLUME SOUND ROUTINE
    // *PARAMETERS
    // *	R0	RANDOM RANGE (0->R0-1)
    // *	R1	VOLUME
    // *	AR2	TABLE ADDR	OF SOUNDS
    // *TRASHED R0,AR2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDSND", 0, 0);
    UNIMPL();
}

void RANDVSND(void)
{
    // asm: 	PUSH	AR2
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	RANDU0
    // asm: 	POP	AR2
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	LDI	R1,R0
    // asm: 	B	VOLSNDFX     		;R0=VOLUME, AR2=SOUND
    // *DRONE SOUND ROUTINE
    // *PARAMETERS
    // *	R0	RANDOM RANGE (0->R0-1)
    // *	AR2	TABLE ADDR	OF SOUNDS
    // *	AR4	DRONE CAR OBJECT
    // *COMPUTES VOLUME BASED ON DISTANCE
    // *TRASHED R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDVSND", 0, 0);
    UNIMPL();
}

void DRONESND(void)
{
    // asm: 	PUSH	AR2
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	RANDU0
    // asm: 	POP	AR2
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: DRONESND1	       			;MAKE ONE DRONE SOUND ONLY...
    // asm: 	FLOAT	*+AR4(ODIST),R0
    // asm: 	RETSN				;BEHIND PLAYER NO SOUND
    // asm: 	FLOAT	10000,R1
    // asm: 	MPYF	5,R1
    // asm: 	CALL	DIV_F
    // asm: 	SUBRF	1,R0
    // asm: 	RETSN
    // asm: 	FLOAT	220,R1
    // asm: 	MPYF	R1,R0
    // asm: 	FIX	R0
    // asm: 	CMPI	220,R0
    // asm: 	LDIGT	220,R0
    // asm: 	B	VOLSNDFX     		;R0=VOLUME, AR2=SOUND
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONESND", 0, 0);
    UNIMPL();
}

void GETCMOS_VALUES(void)
{
    // asm: 	PUSH	AR3
    // asm: 	LDI	@PEDALMNI,AR3
    // asm: 	LDI	ADJ_GASMIN,AR2
    // asm: 	LDI	6,RC
    // asm: 	RPTB	CMOSALP
    // asm: 	PUSH	AR2
    // asm: 	CALL	ADJUSTMENT_READ
    // asm: 	POP	AR2
    // asm: 	FLOAT	R0
    // asm: 	STF	R0,*AR3++
CMOSALP:
    // asm: ADDI	1,AR2
    // asm: 	LDF	@STEERMX,R0
    // asm: 	SUBF	@STEERMN,R0
    // asm: 	STF	R0,@STEERFR
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCMOS_VALUES", 0, 0);
    UNIMPL();
}

void CAMMATSAV(void)
{
    // *----------------------------------------------------------------------------
    // *PUSH CAMERA MATRIX
    // asm: 	POP	BK
    // asm:      	LDI	SP,AR0
    // asm: 	ADDI	9,SP
    // asm: 	LDI	@CAMERAMATRIXI,AR1
    // asm: 	LDF	*AR1++,R0
    // asm: 	RPTS	7
    // asm: 	LDF	*AR1++,R0
    // asm: 	STF	R0,*++AR0
    // asm: 	B	BK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMMATSAV", 0, 0);
    UNIMPL();
}

void CAMMATAVG(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CAMERA MATRIX AVERAGE
    // asm: 	POP	BK
    // asm: 	LDI	*+AR5(CAR_SPIN),R0	;DONT AVG IN SPIN DUDES
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	CAMMATX
    // asm:      	LDI	SP,AR1
    // asm: 	SUBI	8,AR1
    // asm: 	LDI	@CAMERAMATRIXI,AR0
    // asm: 	LDI	8,RC
    // asm: 	RPTB	CAMAVG
    // asm: 	LDF	*AR1++,R0
    // asm: 	MPYF	0.80,R0
    // asm: 	LDF	*AR0,R1
    // asm: 	MPYF	0.20,R1
    // asm: 	ADDF	R0,R1
CAMAVG:
    // asm: STF	R1,*AR0++
CAMMATX:
    // asm: SUBI	9,SP
    // asm: 	B	BK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMMATAVG", 0, 0);
    UNIMPL();
}

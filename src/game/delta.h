#ifndef DELTA_H
#define DELTA_H

#include "../core/port.h"

/* Generated from asm/DELTA.EQU. */

// DELTA.EQU

// asm: DELTA_SAFETYWIDTH	.set	850
#define DELTA_SAFETYWIDTH 850

// asm: DELTA_WIDTH		.set	500
#define DELTA_WIDTH 500

// asm: DELTA_HALF_WIDTH	.set	300
#define DELTA_HALF_WIDTH 300

// asm: DELTAX_MIN		.set	0
#define DELTAX_MIN 0

// DELTAX_MAX		.set	1920
// DELTAX_MAX_2LN		.set	1050
// asm: DELTAX_MAX		.set	2020
#define DELTAX_MAX 2020

// asm: DELTAX_MAX_2LN		.set	1050
#define DELTAX_MAX_2LN 1050

// asm: MAX_DRONES	.set	11	;MAXIMUM DRONES IN UNIVERSE
#define MAX_DRONES 11 // MAXIMUM DRONES IN UNIVERSE

// asm: REGRAC_SIZE	.set	4	;SIZ
#define REGRAC_SIZE 4 // SIZ

// asm: NUM_RACERS	.set	8
#define NUM_RACERS 8

// SUSPEND_MODE
//
// asm: SM_GO		.set	5577h	;REGULAR DRONE PROCESSING
#define SM_GO 0x5577 // REGULAR DRONE PROCESSING

// asm: SM_HALT		.set	33BBh	;FREEZE DRONES ... DONT MOVE (COIN IN, ETC.)
#define SM_HALT 0x33BB // FREEZE DRONES ... DONT MOVE (COIN IN, ETC.)

// asm: THROTTLE_INIT	.set	0.8
#define THROTTLE_INIT 0.8

// asm: MIN_THROTTLE	.set	0.35
#define MIN_THROTTLE 0.35

// asm: MIN_FBTHROTTLE	.set	0.65
#define MIN_FBTHROTTLE 0.65

// asm: MAX_THROTTLE	.set	1.0
#define MAX_THROTTLE 1.0

// asm: FREE_THROTTLE	.set	0.65	;MAX_THROTTLE-MIN_THROTTLE
#define FREE_THROTTLE 0.65 // MAX_THROTTLE-MIN_THROTTLE

// (TI ASM WONT ASSEMBLY FP CONSTANT MATH)

// asm: MAX_SIGMA_THROTTLE	.set	0.50
#define MAX_SIGMA_THROTTLE 0.50

// asm: MAX_COPCAR_THROTTLE	.set	0.50
#define MAX_COPCAR_THROTTLE 0.50

// asm: RHO_MAX_ACCEL_INIT	.set	0.40	;(0.82 == PLYR)
#define RHO_MAX_ACCEL_INIT 0.40 //(0.82 == PLYR)

// asm: MAX_ACCEL_INIT		.set	0.70	;(0.82 == PLYR)
#define MAX_ACCEL_INIT 0.70 //(0.82 == PLYR)

// asm: DELTA_MAX_ACCEL_INIT	.set	0.90	;(0.82 == PLYR)
#define DELTA_MAX_ACCEL_INIT 0.90 //(0.82 == PLYR)

// ----------------------------------------------------------------------------

// asm: SUBDRNSTRT		.set	PDATA+20
#define SUBDRNSTRT (PDATA + 20)

// asm: RHO_NOISE		.set	SUBDRNSTRT+0	;UH	FLG WHETHER IT HAS RAZZED THE PLYR
#define RHO_NOISE (SUBDRNSTRT + 0) // UH	FLG WHETHER IT HAS RAZZED THE PLYR

// asm: RHO_YELL		.set	SUBDRNSTRT+1	;UH	FLG WHETHER IT HAS YELLED THE PLYR
#define RHO_YELL (SUBDRNSTRT + 1) // UH	FLG WHETHER IT HAS YELLED THE PLYR

// asm: RHO_INIT		.set	SUBDRNSTRT+2	;UH	IDX INITIAL RHO TYPE
#define RHO_INIT (SUBDRNSTRT + 2) // UH	IDX INITIAL RHO TYPE

// asm: RHO_THETA		.set	SUBDRNSTRT+3	;FL
#define RHO_THETA (SUBDRNSTRT + 3) // FL

// asm: RHO_THETA_DELTA		.set	SUBDRNSTRT+4	;FL
#define RHO_THETA_DELTA (SUBDRNSTRT + 4) // FL

// asm: RHO_AMP			.set	SUBDRNSTRT+5	;FL
#define RHO_AMP (SUBDRNSTRT + 5) // FL

// asm: RHO_XHEAD		.set	SUBDRNSTRT+6	;FL
#define RHO_XHEAD (SUBDRNSTRT + 6) // FL

// asm: SIGMA_ONCE		.set	SUBDRNSTRT+0	;UH	FLG THE DRONE HAS BEEN ON AT LEAST ONCE (BREAKDOWN FLAG)
#define SIGMA_ONCE (SUBDRNSTRT + 0) // UH	FLG THE DRONE HAS BEEN ON AT LEAST ONCE (BREAKDOWN FLAG)

// asm: SIGMA_YELL		.set	SUBDRNSTRT+1	;UH	FLG WHETHER PLYR HAS RAZZED THE SIGMA
#define SIGMA_YELL (SUBDRNSTRT + 1) // UH	FLG WHETHER PLYR HAS RAZZED THE SIGMA

// asm: SIGMA_MODEL		.set	SUBDRNSTRT+2
#define SIGMA_MODEL (SUBDRNSTRT + 2)

// asm: SIGMA_FLAG		.set	SUBDRNSTRT+3
#define SIGMA_FLAG (SUBDRNSTRT + 3)

// Change to make copcar compatible with the racers object avoidence routines
// ROADOFFSET		.set	SUBDRNSTRT    	;TRACKING OFFSET TO CENTER OF ROAD
// asm: COPCAR_ONCE		.set	SUBDRNSTRT+1	;UH	FLG THE DRONE HAS BEEN ON AT LEAST ONCE (BREAKDOWN FLAG)
#define COPCAR_ONCE (SUBDRNSTRT + 1) // UH	FLG THE DRONE HAS BEEN ON AT LEAST ONCE (BREAKDOWN FLAG)

// asm: COPCAR_CHASE		.set	SUBDRNSTRT+2
#define COPCAR_CHASE (SUBDRNSTRT + 2)

// asm: COPCAR_REACTION		.set	SUBDRNSTRT+3	;FL	0.1-1.0  DELTA_XLANE ratio tracking
#define COPCAR_REACTION (SUBDRNSTRT + 3) // FL	0.1-1.0  DELTA_XLANE ratio tracking

// asm: COPCAR_SPEED_MOD	.set	SUBDRNSTRT+4	;FL	overtake speed varience
#define COPCAR_SPEED_MOD (SUBDRNSTRT + 4) // FL	overtake speed varience

// asm: COPCAR_SIREN		.set	SUBDRNSTRT+5	;UH	sound call to use
#define COPCAR_SIREN (SUBDRNSTRT + 5) // UH	sound call to use

// asm: ROADOFFSET		.set	SUBDRNSTRT    	;TRACKING OFFSET TO CENTER OF ROAD
#define ROADOFFSET SUBDRNSTRT // TRACKING OFFSET TO CENTER OF ROAD

// asm: POWERSURGE		.set	SUBDRNSTRT+1   	;POWER SURGE VALUE
#define POWERSURGE (SUBDRNSTRT + 1) // POWER SURGE VALUE

// asm: POWERCATCH		.set	SUBDRNSTRT+2   	;POWER CATCHUP VALUE
#define POWERCATCH (SUBDRNSTRT + 2) // POWER CATCHUP VALUE

// asm: SURGETIME		.set	SUBDRNSTRT+3   	;TIMER FOR NEW SURGE
#define SURGETIME (SUBDRNSTRT + 3) // TIMER FOR NEW SURGE

// asm: CATCHTIME		.set	SUBDRNSTRT+4   	;TIMER FOR NEW CATCHUP
#define CATCHTIME (SUBDRNSTRT + 4) // TIMER FOR NEW CATCHUP

// asm: STEALTHMODE		.set	SUBDRNSTRT+5   	;STEALTH MODE FLAG 0=NO,1=AHEAD,-1=BEHND
#define STEALTHMODE (SUBDRNSTRT + 5) // STEALTH MODE FLAG 0=NO,1=AHEAD,-1=BEHND

// ----------------------------------------------------------------------------

// * = MUST BE INITIALIZED ON STARTUP
//

// asm: DM_MMODE	.set	0FFh		;MODE MASK
#define DM_MMODE 0x0FF // MODE MASK

// asm: DM_NORMAL	.set	1		;REGULAR DYNAfLEX TRACKING MODE
#define DM_NORMAL 1 // REGULAR DYNAfLEX TRACKING MODE

// asm: DM_STEALTH	.set	2		;STEALTH (MAP) TRACKING MODE (LINEAR)
#define DM_STEALTH 2 // STEALTH (MAP) TRACKING MODE (LINEAR)

// asm: DM_ST_BEYOND	.set	100h		;ENTERED BECAUSE TO FAR AHEAD
#define DM_ST_BEYOND 0x100 // ENTERED BECAUSE TO FAR AHEAD

// asm: DM_ST_BELOW	.set	200h		;ENTERED BECAUSE TO FAR BEHIND
#define DM_ST_BELOW 0x200 // ENTERED BECAUSE TO FAR BEHIND

// asm: DELTA_STATUS_LANE	.set	03h
#define DELTA_STATUS_LANE 0x03

// DELTA_SSTAT
// 0	Regular Player-Windowed Behavior
// 1	Ignore players movement
//

// MODELS
// asm: VETTE_MOD	.set	0
// asm: HOTROD_MOD	.set	1
// asm: BULLET_MOD	.set	2
// asm: TESTOR_MOD	.set	3
// asm: GTRUCK_MOD	.set	4
// asm: FTRUCK_MOD	.set	5
// asm: CBUS_MOD	.set	6
// asm: COPCAR_MOD	.set	7
// asm: MUSCLE_MOD	.set	8
// asm: CARAVAN_MOD	.set	9
// asm: SBUS_MOD	.set	10
// asm: PTRUCKG_MOD	.set	11
// asm: MUSTANG_MOD	.set	12
// **reserved	.set	13
// asm: JEEP_MOD	.set	14
// asm: PLYR_COPCAR_MOD	.set	15
// asm: PLYR_GTRUCK_MOD	.set	16
// asm: PLYR_SBUS_MOD	.set	17
// see eDELTA_MODEL in types.h

// asm: 	.globl	SUSPEND_MODE
extern int SUSPEND_MODE;

// ***	DELTA.ASM

// asm: 	.globl	 LANEPI,LANEP,LANESI,LANES,LANES4
#define LANEPI LANEP

extern float* LANEP[];

extern float LANES[];

extern float LANES4[];

// asm: 	.globl	 SUB_FUNCTION_XLANE
#define SUB_FUNCTION_XLANE DELTA_SUB_FUNCTION

// asm: 	.globl	 RACER_GRID_START	;Starting Index
extern int RACER_GRID_START;

// asm: 	.globl	 DELTA_OINIT
CARBLK* DELTA_OINIT(PROC* p /*AR7*/, OBJ* obj /*AR4*/);

// asm: 	.globl	 GET_TRACK_POS
void GET_TRACK_POS(void);

// asm: 	.globl	 GET_TRACK_POS_RVS
void GET_TRACK_POS_RVS(void);

// asm: 	.globl	 GET_TRACK_POS_RVS_XLANE
void GET_TRACK_POS_RVS_XLANE(void);

// asm: 	.globl	 DELTA_GET_TRACK_POS
void DELTA_GET_TRACK_POS(void);

// asm: 	.globl	 DELTA_SUB_FUNCTION
void DELTA_SUB_FUNCTION(void);

// ***	SIGMA.ASM
// asm: 	.globl	 SIGMA_DRONE
void SIGMA_DRONE(void);

// ***	RHO.ASM
// asm: 	.globl	 RHO_DRONE
void RHO_DRONE(void);

// asm: 	.globl	 RHO_DIE		;generalized drone die routine
void RHO_DIE(void);

// ***	CHOPPER.ASM
// asm: 	.globl	 CHOPPER
void CHOPPER(void);

// ***	COPCAR.ASM
// asm: 	.globl	 AHEAD_OF_PLAYER_P
void AHEAD_OF_PLAYER_P(void);

// asm: 	.globl	 SET_DRONE_PAL
void SET_DRONE_PAL(void);

// asm: 	.globl	 TRAFFIC,GET_LANES,DIST_TO_PLYR
void GET_LANES(void);

void DIST_TO_PLYR(void);

// asm: 	.globl	 INIT_TRACKING_PIECE,GET_TRACK_POS_RVS,GET_TRACK_POS,GET_TRACK_POS_RVS_XLANE
void INIT_TRACKING_PIECE(void);

// .globl	 GET_STEALTH_POS
// asm: 	.globl	 SUB_FUNCTION_RVS,SUB_FUNCTION,SUB_FUNCTION_RVS_XLANE
void SUB_FUNCTION_RVS(void);

void SUB_FUNCTION(void);

void SUB_FUNCTION_RVS_XLANE(void);

// .globl	 FIND_MAPENTRY
// asm: 	.globl	 INIT_DRONES,ADD_DRONE,FREE_DRONE
void INIT_DRONES(void);

void ADD_DRONE(void);

void FREE_DRONE(void);

// asm: 	.globl	 PRECOLLIDE_PLYR
void PRECOLLIDE_PLYR(void);

// asm: 	.globl	 DRONE_RIDE_RIGHT
float DRONE_RIDE_RIGHT(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/);

#endif /* DELTA_H */

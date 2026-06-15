#ifndef SYS_H
#define SYS_H

/* Generated from asm/SYS.EQU. */

// SYS.EQU
//
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED
//
// RACE_MODE tells us what type of race we are having
// are we doing the A to Z continuous (RM_USA)
// or are we in a single race so that we must check
// what the next leg must be
//
// FIRST_RACE you may only select a car (&transmission)
// before the first race, then you are stuck with
// it as long as you continue the game.

// asm: RM_USA		.set	1
#define RM_USA 1

// asm: RM_SINGLE	.set	2
#define RM_SINGLE 2

// I put this here because this ASSEMBLER DOES NOT RESOLVE
// THE SCOPE OF GLOBALS, it simply ASSUMES that all globals are not constants
// asm: COIN_ENTRY_SIZE	.set	11
#define COIN_ENTRY_SIZE 11

// ----------------------------------------------------------------------------
// A LEVEL OF INDIRECTION IS PLACED IN THE SYSTEM
// SO THAT DIFFERENT LEVELS OF MENUS MAY BE REACHED
//
// asm: RACE_GG		.set	0
#define RACE_GG 0

// asm: RACE_SF		.set	1
#define RACE_SF 1

// asm: RACE_US101	.set	2
#define RACE_US101 2

// asm: RACE_RW		.set	3
#define RACE_RW 3

// asm: RACE_BH		.set	4
#define RACE_BH 4

// asm: RACE_LA		.set	5
#define RACE_LA 5

// asm: RACE_DV		.set	6
#define RACE_DV 6

// asm: RACE_AZ		.set	7
#define RACE_AZ 7

// asm: RACE_GC		.set	8
#define RACE_GC 8

// asm: RACE_IOWA	.set	9
#define RACE_IOWA 9

// asm: RACE_CH		.set	10
#define RACE_CH 10

// asm: RACE_IN		.set	11
#define RACE_IN 11

// asm: RACE_APPL	.set	12
#define RACE_APPL 12

// asm: RACE_DC		.set	13
#define RACE_DC 13

// asm: RACE_USA	.set	14
#define RACE_USA 14

// ----------------------------------------------------------------------------

// STRUCT TRACKSEL
// asm: TS_INDEX	.set	0
#define TS_INDEX 0

// ENDSTRUCT
// asm: TS_SIZE		.set	1
#define TS_SIZE 1

// ----------------------------------------------------------------------------
// SYSTEM MODE FLAGS/VALUES
//
// asm: MBRIDGE		.set	00800h		;ON THE GG BRIDGE
#define MBRIDGE 0x00800 // ON THE GG BRIDGE

// asm: MINTUNNEL	.set	00400h		;INSIDE THE TUNNEL
#define MINTUNNEL 0x00400 // INSIDE THE TUNNEL

// asm: MGO		.set	00200h		;GO! RACE is ACTIVE
#define MGO 0x00200 // GO! RACE is ACTIVE

// asm: MHUD		.set	00100h		;HUD on
#define MHUD 0x00100 // HUD on

// asm: MWATER		.set	00080h		;INFINITY WATER on
#define MWATER 0x00080 // INFINITY WATER on

// asm: MINFIN		.set	00040h		;INFINITY SKY on
#define MINFIN 0x00040 // INFINITY SKY on

// asm: MHS		.set	00020h		;HS High score entry display
#define MHS 0x00020 // HS High score entry display

// asm: MSLINE		.set	00010h		;STARTING LINE
#define MSLINE 0x00010 // STARTING LINE

// asm: MMODE		.set	0000Fh		;MASK main mode types
#define MMODE 0x0000F // MASK main mode types

// asm: MINACTIVE	.set	00000h		;SY network inactive node
#define MINACTIVE 0x00000 // SY network inactive node

// asm: MDIAG		.set	00001h		;SY DIAGNOSTICS
#define MDIAG 0x00001 // SY DIAGNOSTICS

// asm: MATTR		.set	00002h		;SY ATTRACT
#define MATTR 0x00002 // SY ATTRACT

// asm: MINTRO		.set	00003h		;GM the INTRO (GAME)
#define MINTRO 0x00003 // GM the INTRO (GAME)

// asm: MGAME		.set	00004h		;GM the GAME
#define MGAME 0x00004 // GM the GAME

// asm: MBONUS		.set	00005h		;GM the BONUS screen
#define MBONUS 0x00005 // GM the BONUS screen

// asm: MINIT		.set	00006h		;GM entering initials
#define MINIT 0x00006 // GM entering initials

// asm: MINSERT_COINS	.set	00007h		;GM asking the questions more coins...
#define MINSERT_COINS 0x00007 // GM asking the questions more coins...

// asm: MCT		.set	00008h		;GM the choose transmission
#define MCT 0x00008 // GM the choose transmission

// ----------------------------------------------------------------------------

// asm: STARTING_TIME	.set	90		;(INTRO.ASM,BONUS.ASM)
#define STARTING_TIME 90 //(INTRO.ASM,BONUS.ASM)

// asm: MPH_CONVERSION	.set	0.49
#define MPH_CONVERSION 0.49

// asm: IMPH_CONVERSION	.set	2.04	;1/0.49
#define IMPH_CONVERSION 2.04 // 1/0.49

// CAR BLOCK STRUCTURE
//
// asm: CARVNUM		.set	5	;	#OF CAR ROAD CONTACT POINTS
#define CARVNUM 5 // #OF CAR ROAD CONTACT POINTS

// asm: CARVSIZ		.set	6	;	CAR POINT STRUCT SIZE
#define CARVSIZ 6 // CAR POINT STRUCT SIZE

// asm: CARPX		.set	0	;FL	CAR SUSPENSION PT X
#define CARPX 0 // FL	CAR SUSPENSION PT X

// asm: CARPY		.set	1	;FL	CAR SUSPENSION PT Y
#define CARPY 1 // FL	CAR SUSPENSION PT Y

// asm: CARPZ		.set	2	;FL	CAR SUSPENSION PT Z
#define CARPZ 2 // FL	CAR SUSPENSION PT Z

// asm: CARPRDYD	.set	3	;FL	DELTA Y TO ROAD
#define CARPRDYD 3 // FL	DELTA Y TO ROAD

// asm: CARPYV		.set	4	;FL	Y VELOCITY
#define CARPYV 4 // FL	Y VELOCITY

// asm: CARPCOL		.set	5	;UH	ROAD OBJECT COLLIDED WITH
#define CARPCOL 5 // UH	ROAD OBJECT COLLIDED WITH

// STRUCT	CARBLK
// asm: CT_PX		.set	0	;FL	CAR SUSPENSION PT X (CENTERPOINT)
#define CT_PX 0 // FL	CAR SUSPENSION PT X (CENTERPOINT)

// asm: CT_PY		.set	1	;FL	CAR SUSPENSION PT Y
#define CT_PY 1 // FL	CAR SUSPENSION PT Y

// asm: CT_PZ		.set	2	;FL	CAR SUSPENSION PT Z
#define CT_PZ 2 // FL	CAR SUSPENSION PT Z

// asm: CT_PRDYD	.set	3	;FL	DELTA Y TO ROAD
#define CT_PRDYD 3 // FL	DELTA Y TO ROAD

// asm: CT_PYV		.set	4	;FL	Y VELOCITY
#define CT_PYV 4 // FL	Y VELOCITY

// asm: CT_PCOL		.set	5	;UH	ROAD OBJECT COLLIDED WITH
#define CT_PCOL 5 // UH	ROAD OBJECT COLLIDED WITH

// asm: RF_PX		.set	6	;FL	CAR SUSPENSION PT X	(RIGHT FRONT)
#define RF_PX 6 // FL	CAR SUSPENSION PT X	(RIGHT FRONT)

// asm: RF_PY		.set	7	;FL	CAR SUSPENSION PT Y
#define RF_PY 7 // FL	CAR SUSPENSION PT Y

// asm: RF_PZ		.set	8	;FL	CAR SUSPENSION PT Z
#define RF_PZ 8 // FL	CAR SUSPENSION PT Z

// asm: RF_PRDYD	.set	9	;FL	DELTA Y TO ROAD
#define RF_PRDYD 9 // FL	DELTA Y TO ROAD

// asm: RF_PYV		.set	10	;FL	Y VELOCITY
#define RF_PYV 10 // FL	Y VELOCITY

// asm: RF_PCOL		.set	11	;UH	ROAD OBJECT COLLIDED WITH
#define RF_PCOL 11 // UH	ROAD OBJECT COLLIDED WITH

// asm: LF_PX		.set	12	;FL	CAR SUSPENSION PT X	(LEFT FRONT)
#define LF_PX 12 // FL	CAR SUSPENSION PT X	(LEFT FRONT)

// asm: LF_PY		.set	13	;FL	CAR SUSPENSION PT Y
#define LF_PY 13 // FL	CAR SUSPENSION PT Y

// asm: LF_PZ		.set	14	;FL	CAR SUSPENSION PT Z
#define LF_PZ 14 // FL	CAR SUSPENSION PT Z

// asm: LF_PRDYD	.set	15	;FL	DELTA Y TO ROAD
#define LF_PRDYD 15 // FL	DELTA Y TO ROAD

// asm: LF_PYV		.set	16	;FL	Y VELOCITY
#define LF_PYV 16 // FL	Y VELOCITY

// asm: LF_PCOL		.set	17	;UH	ROAD OBJECT COLLIDED WITH
#define LF_PCOL 17 // UH	ROAD OBJECT COLLIDED WITH

// asm: LR_PX		.set	18	;FL	CAR SUSPENSION PT X	(LEFT REAR)
#define LR_PX 18 // FL	CAR SUSPENSION PT X	(LEFT REAR)

// asm: LR_PY		.set	19	;FL	CAR SUSPENSION PT Y
#define LR_PY 19 // FL	CAR SUSPENSION PT Y

// asm: LR_PZ		.set	20	;FL	CAR SUSPENSION PT Z
#define LR_PZ 20 // FL	CAR SUSPENSION PT Z

// asm: LR_PRDYD	.set	21	;FL	DELTA Y TO ROAD
#define LR_PRDYD 21 // FL	DELTA Y TO ROAD

// asm: LR_PYV		.set	22	;FL	Y VELOCITY
#define LR_PYV 22 // FL	Y VELOCITY

// asm: LR_PCOL		.set	23	;UH	ROAD OBJECT COLLIDED WITH
#define LR_PCOL 23 // UH	ROAD OBJECT COLLIDED WITH

// asm: RR_PX		.set	24	;FL	CAR SUSPENSION PT X	(RIGHT REAR)
#define RR_PX 24 // FL	CAR SUSPENSION PT X	(RIGHT REAR)

// asm: RR_PY		.set	25	;FL	CAR SUSPENSION PT Y
#define RR_PY 25 // FL	CAR SUSPENSION PT Y

// asm: RR_PZ		.set	26	;FL	CAR SUSPENSION PT Z
#define RR_PZ 26 // FL	CAR SUSPENSION PT Z

// asm: RR_PRDYD	.set	27	;FL	DELTA Y TO ROAD
#define RR_PRDYD 27 // FL	DELTA Y TO ROAD

// asm: RR_PYV		.set	28	;FL	Y VELOCITY
#define RR_PYV 28 // FL	Y VELOCITY

// asm: RR_PCOL		.set	29	;UH	ROAD OBJECT COLLIDED WITH
#define RR_PCOL 29 // UH	ROAD OBJECT COLLIDED WITH

// asm: CAR_ONROAD	.set	30	;UH	O=NOTHING,300=ROAD,310=SHOULDER,XXX=OTHER
#define CAR_ONROAD 30 // UH	O=NOTHING,300=ROAD,310=SHOULDER,XXX=OTHER

// asm: CAR_AIRF	.set	31	;UH	1 IF FRONT AIRBORNE, 0 IF NOT
#define CAR_AIRF 31 // UH	1 IF FRONT AIRBORNE, 0 IF NOT

// asm: CAR_AIRB	.set	32	;UH	1 IF REAR AIRBORNE, 0 IF NOT
#define CAR_AIRB 32 // UH	1 IF REAR AIRBORNE, 0 IF NOT

// asm: CARTURN		.set	33	;FL	ANGLE OF FRONT WHEELS
#define CARTURN 33 // FL	ANGLE OF FRONT WHEELS

// asm: CARTRACTION	.set	34	;FL	TRACTION COEFF. (0=MAX TRACTION)
#define CARTRACTION 34 // FL	TRACTION COEFF. (0=MAX TRACTION)

// asm: CARMAXACCEL	.set	35	;FL	MAXIMUM ACCEL (PIX/16 MSEC)
#define CARMAXACCEL 35 // FL	MAXIMUM ACCEL (PIX/16 MSEC)

// asm: CARTHROTTLE	.set	36	;FL	THROTTLE VALUE 0-1.0 (1.0=FULL THROTTLE)
#define CARTHROTTLE 36 // FL	THROTTLE VALUE 0-1.0 (1.0=FULL THROTTLE)

// asm: CARSKID		.set	37	;FL	SKID VALUE 0-1.0 (1.0=FULL SKID)
#define CARSKID 37 // FL	SKID VALUE 0-1.0 (1.0=FULL SKID)

// asm: CARSPEED	.set	38	;FL	CURRENT SPEED (PIX/16 MSEC.)
#define CARSPEED 38 // FL	CURRENT SPEED (PIX/16 MSEC.)

// asm: CARDIST		.set	39	;FL	CURRENT DIST TRAVELLED (PIX/16 MSEC.)
#define CARDIST 39 // FL	CURRENT DIST TRAVELLED (PIX/16 MSEC.)

// asm: CARXMOM		.set	40	;FL	ANGULAR MOMENTUM (X RADIANS/F)
#define CARXMOM 40 // FL	ANGULAR MOMENTUM (X RADIANS/F)

// asm: CARYMOM		.set	41	;FL	ANGULAR MOMENTUM (Y RADIANS/F)
#define CARYMOM 41 // FL	ANGULAR MOMENTUM (Y RADIANS/F)

// asm: CARZMOM		.set	42	;FL	ANGULAR MOMENTUM (Z RADIANS/F)
#define CARZMOM 42 // FL	ANGULAR MOMENTUM (Z RADIANS/F)

// asm: CARBRAKE	.set	43	;FL	BRAKING FRICTION (0-1.0)
#define CARBRAKE 43 // FL	BRAKING FRICTION (0-1.0)

// asm: CARYROT		.set	44	;FL	Y ROTATION CAR BODY RADIANS
#define CARYROT 44 // FL	Y ROTATION CAR BODY RADIANS

// asm: CARVROT		.set	45	;FL	Y ROTATION CAR VELOCITY
#define CARVROT 45 // FL	Y ROTATION CAR VELOCITY

// asm: CARDROT		.set	46	;FL	LAST FRAME Y BODY ROTATION
#define CARDROT 46 // FL	LAST FRAME Y BODY ROTATION

// asm: CARROT		.set	47	;FL	OVER-ROTATION FOR TURNING
#define CARROT 47 // FL	OVER-ROTATION FOR TURNING

// asm: CARMASS		.set	48	;FL     MASS OF CAR (tons)
#define CARMASS 48 // FL     MASS OF CAR (tons)

// asm: CAR_SPIN	.set	49	;UH	SPINOUT FLAG
#define CAR_SPIN 49 // UH	SPINOUT FLAG

// asm: CARSPRAD	.set	50	;FL	RADIANS TO SPIN
#define CARSPRAD 50 // FL	RADIANS TO SPIN

// asm: CAR_BUMP	.set	51	;UH	BUMP FLAG 0=NO BUMP, 1-F=INTENSITY
#define CAR_BUMP 51 // UH	BUMP FLAG 0=NO BUMP, 1-F=INTENSITY

// asm: CARPTSTR	.set	52	;SD	TRACK PIECE	} POSITION
#define CARPTSTR 52 // SD	TRACK PIECE	} POSITION

// asm: CARPTSDIST	.set	53	;FL	DISTANCE	}  TRACKING
#define CARPTSDIST 53 // FL	DISTANCE	}  TRACKING

// asm: CARPTSRANK	.set	54	;UD	RANK		}   SYSTEM
#define CARPTSRANK 54 // UD	RANK		}   SYSTEM

// asm: CARTRAK		.set	55	;UH	CLOSEST TRACK PIECE
#define CARTRAK 55 // UH	CLOSEST TRACK PIECE

// asm: CARGEAR		.set	56	;UH	GEAR 0=NEUTRAL,1,2,3,4
#define CARGEAR 56 // UH	GEAR 0=NEUTRAL,1,2,3,4

// asm: CARRPM		.set	57	;FL     RPMX100 (ABOUT 0.0 to 50.0)
#define CARRPM 57 // FL     RPMX100 (ABOUT 0.0 to 50.0)

// asm: CARXPLUS       	.set	58	;FL	CAR BOX X EXTREMES
#define CARXPLUS 58 // FL	CAR BOX X EXTREMES

// asm: CARXMINUS      	.set	59	;FL	CAR BOX X EXTREMES
#define CARXMINUS 59 // FL	CAR BOX X EXTREMES

// asm: CARYPLUS       	.set	60	;FL	CAR BOX Y EXTREMES
#define CARYPLUS 60 // FL	CAR BOX Y EXTREMES

// asm: CARYMINUS      	.set	61	;FL	CAR BOX Y EXTREMES
#define CARYMINUS 61 // FL	CAR BOX Y EXTREMES

// asm: CARZPLUS       	.set	62	;FL	CAR BOX Z EXTREMES
#define CARZPLUS 62 // FL	CAR BOX Z EXTREMES

// asm: CARZMINUS      	.set	63	;FL	CAR BOX Z EXTREMES
#define CARZMINUS 63 // FL	CAR BOX Z EXTREMES

// asm: CAR_ID		.set	64	;UH	DEBUGGING CAR ID
#define CAR_ID 64 // UH	DEBUGGING CAR ID

// asm: CARSHAD		.set	65	;UH	CAR SHADOW FLAG	ON=1, 0=OFF
#define CARSHAD 65 // UH	CAR SHADOW FLAG	ON=1, 0=OFF

// asm: CARXLEAN	.set	66	;FL	X BODY LEAN
#define CARXLEAN 66 // FL	X BODY LEAN

// asm: CARZLEAN	.set	67	;FL	Z BODY LEAN
#define CARZLEAN 67 // FL	Z BODY LEAN

// asm: CARRDFR		.set	68	;FL	ROAD FRICTION
#define CARRDFR 68 // FL	ROAD FRICTION

// asm: CAROFRDFR	.set	69	;FL	OFFROAD FRICTION
#define CAROFRDFR 69 // FL	OFFROAD FRICTION

// asm: CARDIST2CNTR	.set	70	;FL	DISTANCE TO CENTER OF ROAD
#define CARDIST2CNTR 70 // FL	DISTANCE TO CENTER OF ROAD

// asm: CARWHLTAB	.set	71	;FL	WHEEL ROAD SCAN OFFSETS
#define CARWHLTAB 71 // FL	WHEEL ROAD SCAN OFFSETS

// asm: CARTRANS	.set	86	;UH	0=AUTO, 1=MANUAL TRANSMISSION
#define CARTRANS 86 // UH	0=AUTO, 1=MANUAL TRANSMISSION

// asm: CARTRACK_ID	.set	87	;UH	ID OF CURRENT TRACK SECTION
#define CARTRACK_ID 87 // UH	ID OF CURRENT TRACK SECTION

// asm: CAR_OM		.set	88	;UH	OTHER MACHINE CONTROLS CAR (LINKING)
#define CAR_OM 88 // UH	OTHER MACHINE CONTROLS CAR (LINKING)

// asm: CARNUM		.set	89	;UH	CAR DISPATCH # 0-127, B7=MASTER
#define CARNUM 89 // UH	CAR DISPATCH # 0-127, B7=MASTER

// asm: CARUPD		.set	90	;UH     CAR HAS BEEN COMM UPDATED THIS FRAME
#define CARUPD 90 // UH     CAR HAS BEEN COMM UPDATED THIS FRAME

// asm: CARSIZ		.set	91	;SZ	TOTAL SIZE OF CAR STRUCTURE
#define CARSIZ 91 // SZ	TOTAL SIZE OF CAR STRUCTURE

// ENDSTRUCT

// asm: NUM_CARS	.set	25		;NUMBER OF CARS ACTIVE
#define NUM_CARS 25 // NUMBER OF CARS ACTIVE

// asm: FEET		.set	48
#define FEET 48

// STRUCT VEHTAB
// asm: VEHTAB_MODEL	.set	0	;UH	ROM PTR TO MODEL
#define VEHTAB_MODEL 0 // UH	ROM PTR TO MODEL

// asm: VEHTAB_PAL	.set	1	;UH	MODEL PTR
#define VEHTAB_PAL 1 // UH	MODEL PTR

// asm: VEHTAB_ANI	.set	2	;UH	ANIMATION TABLE (0 IF NONE)
#define VEHTAB_ANI 2 // UH	ANIMATION TABLE (0 IF NONE)

// asm: VEHTAB_DMODEL	.set	3	;UH	DEGRADED MODEL (0 IF NONE)
#define VEHTAB_DMODEL 3 // UH	DEGRADED MODEL (0 IF NONE)

// asm: VEHTAB_DMODEL2	.set	4	;UH	DEGRADED MODEL (0 IF NONE) LEVEL 2
#define VEHTAB_DMODEL2 4 // UH	DEGRADED MODEL (0 IF NONE) LEVEL 2

// asm: VEHTAB_TAILCNT	.set	5	;UH
#define VEHTAB_TAILCNT 5 // UH

// asm: VEHTAB_MASS	.set	6	;FL	MASS OF MODEL
#define VEHTAB_MASS 6 // FL	MASS OF MODEL

// asm: VEHTAB_PASSBY	.set	7	;UH	SND PTR TO PASSBY CALL
#define VEHTAB_PASSBY 7 // UH	SND PTR TO PASSBY CALL

// asm: VEHTAB_BNOISE	.set	8	;UH	SND PTR TO BACKGRND CALL
#define VEHTAB_BNOISE 8 // UH	SND PTR TO BACKGRND CALL

// asm: VEHTAB_PALTAB	.set	9	;UH	PALETTE TABLE PTR
#define VEHTAB_PALTAB 9 // UH	PALETTE TABLE PTR

// asm: VEHTAB_DDYNATAB	.set	10	;UH	DRONE DYNAMIC TABLE PTR
#define VEHTAB_DDYNATAB 10 // UH	DRONE DYNAMIC TABLE PTR

// asm: VEHTAB_SIZE	.set	11	;SIZ
#define VEHTAB_SIZE 11 // SIZ

// ENDSTRUCT

// THIS IS THE LEG MAP
//
//
// STRUCT LEG
// asm: LEG_POSX	.set	0	;UH
#define LEG_POSX 0 // UH

// asm: LEG_POSY	.set	1	;UH ALSO HAS RIGHTMOST BIT 0 = 4 LANE 1 = 2 LANE
#define LEG_POSY 1 // UH ALSO HAS RIGHTMOST BIT 0 = 4 LANE 1 = 2 LANE

// asm: LEG_POSZ	.set	2	;UH
#define LEG_POSZ 2 // UH

// asm: LEG_ID		.set	3	;UH
#define LEG_ID 3 // UH

// ENDSTRUCT
// asm: LEG_SIZE	.set	4	;SI
#define LEG_SIZE 4 // SI

// asm: MAX_LEG_ELEMENTS	.set	1600
#define MAX_LEG_ELEMENTS 1600

// STATISTICAL STRUCTURE FOR THE GAME LEGS <NERD DATA>
//
//
//
// STRUCT	tagGAMETRAK
// asm: GT_ETIME	.set	0	;TC	ELAPSED TIME (TIME CODED)
#define GT_ETIME 0 // TC	ELAPSED TIME (TIME CODED)

// asm: GT_POS		.set	1	;UD	POSITION
#define GT_POS 1 // UD	POSITION

// asm: GT_MAXMPH	.set	2	;UD	MAX MPH
#define GT_MAXMPH 2 // UD	MAX MPH

// asm: GT_COLLS	.set	3	;UD	NUMBER OF COLLISIONS
#define GT_COLLS 3 // UD	NUMBER OF COLLISIONS

// asm: GT_SIZE		.set	4	;SIZ
#define GT_SIZE 4 // SIZ

// ENDSTRUCT

//
// this structure has hard coded routines which depend on the
// size being a known constant. see backgrnd.asm.  do not alter
// the size of this structure.
//
// STRUCT	STRDGROUP
// asm: DGRP_HEAD	.set	0	;UH	PTR TO HEAD OBJECT OF THE GROUP
#define DGRP_HEAD 0 // UH	PTR TO HEAD OBJECT OF THE GROUP

// asm: DGRP_BIN	.set	1	;UH	PTR TO BINARY ROM STRUCT
#define DGRP_BIN 1 // UH	PTR TO BINARY ROM STRUCT

// asm: DGRP_FSTART	.set	2	;UH	fLEX START
#define DGRP_FSTART 2 // UH	fLEX START

// asm: DGRP_FLAG	.set	3	;UH	FLAGS (see below)
#define DGRP_FLAG 3 // UH	FLAGS (see below)

// asm: DGRP_IDX	.set	4	;UH	INDEX NUMBER
#define DGRP_IDX 4 // UH	INDEX NUMBER

// ENDSTRUCT
// asm: DGRP_SIZE	.set	5	;SI	sizeof(STRUCT DGROUP_S)
#define DGRP_SIZE 5 // SI	sizeof(STRUCT DGROUP_S)

// asm: MAX_DGROUPS	.set	20	;MAXIMUM DGROUPS ACTIVE
#define MAX_DGROUPS 20 // MAXIMUM DGROUPS ACTIVE

// STRUCT	TYCOB
// asm: TB_FLAG		.set	0	;UH
#define TB_FLAG 0 // UH

// asm: TB_POSX		.set	1	;FL
#define TB_POSX 1 // FL

// asm: TB_POSY		.set	2	;FL
#define TB_POSY 2 // FL

// asm: TB_POSZ		.set	3	;FL
#define TB_POSZ 3 // FL

// asm: TB_RADY		.set	4	;FL
#define TB_RADY 4 // FL

// asm: TB_GROUP	.set	5	;UH
#define TB_GROUP 5 // UH

// asm: TB_GROUPOVERLAY	.set	6	;UH
#define TB_GROUPOVERLAY 6 // UH

// asm: TB_REGSIZE	.set	7	;SIZ
#define TB_REGSIZE 7 // SIZ

// asm: TB_RVS_POSX	.set	7	;FL
#define TB_RVS_POSX 7 // FL

// asm: TB_RVS_POSY	.set	8	;FL
#define TB_RVS_POSY 8 // FL

// asm: TB_RVS_POSZ	.set	9	;FL
#define TB_RVS_POSZ 9 // FL

// asm: TB_RVS_RADY	.set	10	;FL
#define TB_RVS_RADY 10 // FL

// asm: TB_RVSSIZE	.set	11	;SIZ
#define TB_RVSSIZE 11 // SIZ

// asm: TB_OVER2	.set	12	; or 6 or 7
#define TB_OVER2 12 // or 6 or 7

// ENDSTRUCT

// OVERLAY SECTION 'LOOK'
// TYCO TRACK FLAGS
//
// asm: SC_OVERLAY	.set	00001h
#define SC_OVERLAY 0x00001

// asm: SC_BRANCH	.set	00002h
#define SC_BRANCH 0x00002

// asm: SC_2NDSTART	.set	00004h
#define SC_2NDSTART 0x00004

// asm: SC_REVERSE	.set	00008h
#define SC_REVERSE 0x00008

// asm: SC_ROADMODEL	.set	00070h
#define SC_ROADMODEL 0x00070

// asm: SC_OVERDSC	.set	08000h		;USE CURRENT OVERLAY 1 or 2
#define SC_OVERDSC 0x08000 // USE CURRENT OVERLAY 1 or 2

// asm: SC_OVER2	.set	01000h
#define SC_OVER2 0x01000

// asm: SC_LDSECT	.set	01000h		;LOAD A NEW SECTION INTO THE SYSTEM
#define SC_LDSECT 0x01000 // LOAD A NEW SECTION INTO THE SYSTEM

// asm: SC_LDDEST	.set	06000h		;WHICH OVERLAY TO LOAD INTO (1 or 2)
#define SC_LDDEST 0x06000 // WHICH OVERLAY TO LOAD INTO (1 or 2)

// asm: SC_LDSECT_IDX	.set	00F00h		;WHICH LOOK TO LOAD INTO OVERLAY SPACE
#define SC_LDSECT_IDX 0x00F00 // WHICH LOOK TO LOAD INTO OVERLAY SPACE

// asm: SC_ROUTINE	.set	000FF0000h	;IF NON-ZERO ROUTINE SHOULD BE CALLED
#define SC_ROUTINE 0x000FF0000 // IF NON-ZERO ROUTINE SHOULD BE CALLED

// asm: AUTO_TRANSMISSION	.set	0
#define AUTO_TRANSMISSION 0

// asm: MANUAL_TRANSMISSION	.set	1
#define MANUAL_TRANSMISSION 1

// ----------------------------------------------------------------------------
// asm: PI		.set	3.141592654
#define PI 3.141592654

// asm: TWOPI		.set	6.283185307
#define TWOPI 6.283185307

// asm: HALFPI		.set	1.570796327
#define HALFPI 1.570796327

// asm: HALFPI3		.set	4.71238898
#define HALFPI3 4.71238898

// ----------------------------------------------------------------------------
// PLAYER STRUCT
// asm: PLY_STATUS	.set	00h
#define PLY_STATUS 0x00

// asm: PLY_CAR		.set	01h
#define PLY_CAR 0x01

// asm: PLY_PROC	.set	02h
#define PLY_PROC 0x02

// asm: PLY_SIZ		.set	03h
#define PLY_SIZ 0x03

// asm: PLY_CBLK	.set	04h
#define PLY_CBLK 0x04

// ----------------------------------------------------------------------------
// GLOW BUTTONS
//
// asm: BUT_START	.set	1
#define BUT_START 1

// asm: BUT_VIEW1	.set	2
#define BUT_VIEW1 2

// asm: BUT_VIEW2	.set	4
#define BUT_VIEW2 4

// asm: BUT_VIEW3	.set	8
#define BUT_VIEW3 8

// asm: BUT_VIEWS	.set	0Eh
#define BUT_VIEWS 0x0E

// asm: BUT_TAILS	.set	030h
#define BUT_TAILS 0x030

// asm: BUT_FRONT	.set	0C0h
#define BUT_FRONT 0x0C0

// ----------------------------------------------------------------------------
// GENV IDS
//
// !!!DO NOT CHANGE THESE EQUATES!!!  MORE THAN 3 SYSTEMS RELY ON THESE!!!
// !!!NUMBERS BEING EXACTLY AS SPECIFIED HERE.  DO NOT THINK OF CHANGING THEM!!!
//
// asm: BGD_BIGOBJ	.set	01000h
#define BGD_BIGOBJ 0x01000

// asm: BGD_ROUTINE	.set	02000h
#define BGD_ROUTINE 0x02000

// asm: BGD_PALETTE	.set	04000h
#define BGD_PALETTE 0x04000

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// asm: MAX_VERTICES	.set	256
#define MAX_VERTICES 256

// asm: MAX_POLYGONS	.set	300		;ACTUALLY UNLIMITED AS LONG AS CONVEX
#define MAX_POLYGONS 300 // ACTUALLY UNLIMITED AS LONG AS CONVEX

// asm: FP		.set	AR3
#define FP AR3

// asm: SCRNHX		.set	256.0		;HALF OF SCREEN X SIZE FLOAT
#define SCRNHX 256.0 // HALF OF SCREEN X SIZE FLOAT

// asm: SCRNHY		.set	200.0		;HALF OF SCREEN Y SIZE FLOAT
#define SCRNHY 200.0 // HALF OF SCREEN Y SIZE FLOAT

// asm: SCREENHX	.set	256		;HALF OF SCREEN X SIZE INTEGER
#define SCREENHX 256 // HALF OF SCREEN X SIZE INTEGER

// asm: SCREENHY	.set	200		;HALF OF SCREEN Y SIZE INTEGER
#define SCREENHY 200 // HALF OF SCREEN Y SIZE INTEGER

// STRUCT	VECTOR
// asm: X		.set	0
#define X 0

// asm: Y		.set	1
#define Y 1

// asm: Z		.set	2
#define Z 2

// asm: W		.set	3
#define W 3

// ENDSTRUCT

// // STRUCT	EQUATION
// // asm: A		.set	0
// #define A 0

// // asm: B		.set	1
// #define B 1

// // asm: C		.set	2
// #define C 2

// // asm: D		.set	3
// #define D 3

// ENDSTRUCT

// STRUCT MATRIX
// asm: A00	 	.set	0
#define A00 0

// asm: A01	 	.set	1
#define A01 1

// asm: A02	 	.set	2
#define A02 2

// asm: A10	 	.set	3
#define A10 3

// asm: A11	 	.set	4
#define A11 4

// asm: A12	 	.set	5
#define A12 5

// asm: A20	 	.set	6
#define A20 6

// asm: A21	 	.set	7
#define A21 7

// asm: A22	 	.set	8
#define A22 8

// ENDSTRUCT

// ----------------------------------------------------------------------------
// DYNAMIC OBJECT STRUCTURE
//
// If the object has the dynamic object flag set then the vectors are translated
// as follows:
// 1.	assume a regular vertex list
// 2.	get the first dynamic object
// 3.	for each vertex belonging to the dynamic object:
// -subtract off the DYNACENTER
// -rotate by the DYNAMATRIX
// -translate by the DYNATRANS
// -further transform as normal:
// -general rotational matrix
// -general translation
// -division by depth
// -store into blowlist
// 4.	continue getting dynamic objects until finished
//
//
//
// STRUCT DYNAOBJ
// asm: DYNALINK	.set	00		;link to next DYNAOBJ
#define DYNALINK 00 // link to next DYNAOBJ

// asm: DYNACENTERX	.set	01		;the center of the vertices
#define DYNACENTERX 01 // the center of the vertices

// asm: DYNACENTERY	.set	02
#define DYNACENTERY 02

// asm: DYNACENTERZ	.set	03
#define DYNACENTERZ 03

// asm: DYNATRANSX	.set	04		;the translation of the vertices
#define DYNATRANSX 04 // the translation of the vertices

// asm: DYNATRANSY	.set	05
#define DYNATRANSY 05

// asm: DYNATRANSZ	.set	06
#define DYNATRANSZ 06

// asm: DYNAMATRIX 	.set	07		;the matrix of the vertices
#define DYNAMATRIX 07 // the matrix of the vertices

// asm: DYNAMAT00 	.set	07
#define DYNAMAT00 07

// asm: DYNAMAT10 	.set	08
#define DYNAMAT10 08

// asm: DYNAMAT20 	.set	09
#define DYNAMAT20 09

// asm: DYNAMAT01 	.set	0Ah
#define DYNAMAT01 0x0A

// asm: DYNAMAT11 	.set	0Bh
#define DYNAMAT11 0x0B

// asm: DYNAMAT21 	.set	0Ch
#define DYNAMAT21 0x0C

// asm: DYNAMAT02 	.set	0Dh
#define DYNAMAT02 0x0D

// asm: DYNAMAT12 	.set	0Eh
#define DYNAMAT12 0x0E

// asm: DYNAMAT22 	.set	0Fh
#define DYNAMAT22 0x0F

// asm: DYNANVERTS	.set	10h
#define DYNANVERTS 0x10

// asm: DYNAPARENT	.set	11h
#define DYNAPARENT 0x11

// asm: DYNAFLAG	.set	12h
#define DYNAFLAG 0x12

// ENDSTRUCT
// asm: DYNASIZE	.set	13h
#define DYNASIZE 0x13

// asm: NUM_DYNAS	.set	NUM_CARS*7
#define NUM_DYNAS (NUM_CARS * 7)

// SOUND SYSTEM EQUATES
// asm: PRIORITY	.set	00FFh
#define PRIORITY 0x00FF

// asm: CHAN0		.set	0000h		;
#define CHAN0 0x0000

// asm: CHAN1		.set	0100h		;
#define CHAN1 0x0100

// asm: CHAN2		.set	0200h		;
#define CHAN2 0x0200

// asm: CHAN3		.set	0300h		;
#define CHAN3 0x0300

// asm: NINT		.set	1000h		;NON INTERRUPTABLE
#define NINT 0x1000 // NON INTERRUPTABLE

// asm: ENINT		.set	2000h		;NON INTERRUPTABLE BY EQUAL
#define ENINT 0x2000 // NON INTERRUPTABLE BY EQUAL

// asm: NCHAN		.set	4
#define NCHAN 4

// STRUCT SOUNDSTR
// asm: SND_PRI		.set	0		;UH	priority (00=lowest, FF= highest)
#define SND_PRI 0 // UH	priority (00=lowest, FF= highest)

// asm: SND_TIM		.set	1		;UH	TIMER
#define SND_TIM 1 // UH	TIMER

// asm: SND_TMR		.set	2		;UH	TIMER COUNTDOWN
#define SND_TMR 2 // UH	TIMER COUNTDOWN

// asm: SND_ST		.set	3		;UH	ADDRESS OF SOUND TABLE (START)
#define SND_ST 3 // UH	ADDRESS OF SOUND TABLE (START)

// asm: SND_ADDR	.set	4		;UH	ADDRESS OF SOUND TABLE (CURRENT POINTER)
#define SND_ADDR 4 // UH	ADDRESS OF SOUND TABLE (CURRENT POINTER)

// asm: SND_VOL		.set	5		;SD	VOLUME OF TRACK
#define SND_VOL 5 // SD	VOLUME OF TRACK

// asm: SND_IDX		.set	6		;UH	INDEX OF SOUND
#define SND_IDX 6 // UH	INDEX OF SOUND

// asm: SND_REP		.set	7		;UH	REPEAT COUNT OF SOUND
#define SND_REP 7 // UH	REPEAT COUNT OF SOUND

// asm: SND_STATUS	.set	8		;UH	FLAG FX SND increment ?
#define SND_STATUS 8 // UH	FLAG FX SND increment ?

// ENDSTRUCT
// asm: SND_SIZ		.set	9		;SIZ
#define SND_SIZ 9 // SIZ

// asm: NUM_STATIONS	.set	4
#define NUM_STATIONS 4

// asm: SURFIN_STAT	.set	0
#define SURFIN_STAT 0

// asm: HARDROCK_STAT	.set	1
#define HARDROCK_STAT 1

// asm: COUNTRY_STAT	.set	2
#define COUNTRY_STAT 2

// asm: RAP_STAT	.set	3
#define RAP_STAT 3

// asm: 	.globl	RACE_MODE
extern int RACE_MODE;

// asm: 	.globl	FIRST_RACE
extern int FIRST_RACE;

// asm: 	.globl	LEG_MAP	;RAM SPACE
extern int LEG_MAP[MAX_LEG_ELEMENTS * LEG_SIZE];

#endif /* SYS_H */

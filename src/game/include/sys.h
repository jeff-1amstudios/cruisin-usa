#ifndef SYS_H
#define SYS_H

/* Generated from asm/SYS.EQU. */
/*
 * SYS.EQU
 * COPYRIGHT (C) 1994  BY TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * RACE_MODE tells us what type of race we are having
 * are we doing the A to Z continuous (RM_USA)
 * or are we in a single race so that we must check
 * what the next leg must be
 * FIRST_RACE you may only select a car (&transmission)
 * ...
 */

/* Original .globl symbols in this module:
 *   RACE_MODE
 *   FIRST_RACE
 *   LEG_MAP
 */

#define RM_USA 1
#define RM_SINGLE 2
#define COIN_ENTRY_SIZE 11
#define RACE_GG 0
#define RACE_SF 1
#define RACE_US101 2
#define RACE_RW 3
#define RACE_BH 4
#define RACE_LA 5
#define RACE_DV 6
#define RACE_AZ 7
#define RACE_GC 8
#define RACE_IOWA 9
#define RACE_CH 10
#define RACE_IN 11
#define RACE_APPL 12
#define RACE_DC 13
#define RACE_USA 14
#define TS_INDEX 0
#define TS_SIZE 1
#define MBRIDGE 0x00800 /* ON THE GG BRIDGE */
#define MINTUNNEL 0x00400 /* INSIDE THE TUNNEL */
#define MGO 0x00200 /* GO! RACE is ACTIVE */
#define MHUD 0x00100 /* HUD on */
#define MWATER 0x00080 /* INFINITY WATER on */
#define MINFIN 0x00040 /* INFINITY SKY on */
#define MHS 0x00020 /* HS High score entry display */
#define MSLINE 0x00010 /* STARTING LINE */
#define MMODE 0x0000F /* MASK main mode types */
#define MINACTIVE 0x00000 /* SY network inactive node */
#define MDIAG 0x00001 /* SY DIAGNOSTICS */
#define MATTR 0x00002 /* SY ATTRACT */
#define MINTRO 0x00003 /* GM the INTRO (GAME) */
#define MGAME 0x00004 /* GM the GAME */
#define MBONUS 0x00005 /* GM the BONUS screen */
#define MINIT 0x00006 /* GM entering initials */
#define MINSERT_COINS 0x00007 /* GM asking the questions more coins... */
#define MCT 0x00008 /* GM the choose transmission */
#define STARTING_TIME 90 /* (INTRO.ASM,BONUS.ASM) */
#define MPH_CONVERSION 0.49
#define IMPH_CONVERSION 2.04 /* 1/0.49 */
#define CARVNUM 5 /* #OF CAR ROAD CONTACT POINTS */
#define CARVSIZ 6 /* CAR POINT STRUCT SIZE */
#define CARPX 0 /* FL	CAR SUSPENSION PT X */
#define CARPY 1 /* FL	CAR SUSPENSION PT Y */
#define CARPZ 2 /* FL	CAR SUSPENSION PT Z */
#define CARPRDYD 3 /* FL	DELTA Y TO ROAD */
#define CARPYV 4 /* FL	Y VELOCITY */
#define CARPCOL 5 /* UH	ROAD OBJECT COLLIDED WITH */
#define CT_PX 0 /* FL	CAR SUSPENSION PT X (CENTERPOINT) */
#define CT_PY 1 /* FL	CAR SUSPENSION PT Y */
#define CT_PZ 2 /* FL	CAR SUSPENSION PT Z */
#define CT_PRDYD 3 /* FL	DELTA Y TO ROAD */
#define CT_PYV 4 /* FL	Y VELOCITY */
#define CT_PCOL 5 /* UH	ROAD OBJECT COLLIDED WITH */
#define RF_PX 6 /* FL	CAR SUSPENSION PT X	(RIGHT FRONT) */
#define RF_PY 7 /* FL	CAR SUSPENSION PT Y */
#define RF_PZ 8 /* FL	CAR SUSPENSION PT Z */
#define RF_PRDYD 9 /* FL	DELTA Y TO ROAD */
#define RF_PYV 10 /* FL	Y VELOCITY */
#define RF_PCOL 11 /* UH	ROAD OBJECT COLLIDED WITH */
#define LF_PX 12 /* FL	CAR SUSPENSION PT X	(LEFT FRONT) */
#define LF_PY 13 /* FL	CAR SUSPENSION PT Y */
#define LF_PZ 14 /* FL	CAR SUSPENSION PT Z */
#define LF_PRDYD 15 /* FL	DELTA Y TO ROAD */
#define LF_PYV 16 /* FL	Y VELOCITY */
#define LF_PCOL 17 /* UH	ROAD OBJECT COLLIDED WITH */
#define LR_PX 18 /* FL	CAR SUSPENSION PT X	(LEFT REAR) */
#define LR_PY 19 /* FL	CAR SUSPENSION PT Y */
#define LR_PZ 20 /* FL	CAR SUSPENSION PT Z */
#define LR_PRDYD 21 /* FL	DELTA Y TO ROAD */
#define LR_PYV 22 /* FL	Y VELOCITY */
#define LR_PCOL 23 /* UH	ROAD OBJECT COLLIDED WITH */
#define RR_PX 24 /* FL	CAR SUSPENSION PT X	(RIGHT REAR) */
#define RR_PY 25 /* FL	CAR SUSPENSION PT Y */
#define RR_PZ 26 /* FL	CAR SUSPENSION PT Z */
#define RR_PRDYD 27 /* FL	DELTA Y TO ROAD */
#define RR_PYV 28 /* FL	Y VELOCITY */
#define RR_PCOL 29 /* UH	ROAD OBJECT COLLIDED WITH */
#define CAR_ONROAD 30 /* UH	O=NOTHING,300=ROAD,310=SHOULDER,XXX=OTHER */
#define CAR_AIRF 31 /* UH	1 IF FRONT AIRBORNE, 0 IF NOT */
#define CAR_AIRB 32 /* UH	1 IF REAR AIRBORNE, 0 IF NOT */
#define CARTURN 33 /* FL	ANGLE OF FRONT WHEELS */
#define CARTRACTION 34 /* FL	TRACTION COEFF. (0=MAX TRACTION) */
#define CARMAXACCEL 35 /* FL	MAXIMUM ACCEL (PIX/16 MSEC) */
#define CARTHROTTLE 36 /* FL	THROTTLE VALUE 0-1.0 (1.0=FULL THROTTLE) */
#define CARSKID 37 /* FL	SKID VALUE 0-1.0 (1.0=FULL SKID) */
#define CARSPEED 38 /* FL	CURRENT SPEED (PIX/16 MSEC.) */
#define CARDIST 39 /* FL	CURRENT DIST TRAVELLED (PIX/16 MSEC.) */
#define CARXMOM 40 /* FL	ANGULAR MOMENTUM (X RADIANS/F) */
#define CARYMOM 41 /* FL	ANGULAR MOMENTUM (Y RADIANS/F) */
#define CARZMOM 42 /* FL	ANGULAR MOMENTUM (Z RADIANS/F) */
#define CARBRAKE 43 /* FL	BRAKING FRICTION (0-1.0) */
#define CARYROT 44 /* FL	Y ROTATION CAR BODY RADIANS */
#define CARVROT 45 /* FL	Y ROTATION CAR VELOCITY */
#define CARDROT 46 /* FL	LAST FRAME Y BODY ROTATION */
#define CARROT 47 /* FL	OVER-ROTATION FOR TURNING */
#define CARMASS 48 /* FL     MASS OF CAR (tons) */
#define CAR_SPIN 49 /* UH	SPINOUT FLAG */
#define CARSPRAD 50 /* FL	RADIANS TO SPIN */
#define CAR_BUMP 51 /* UH	BUMP FLAG 0=NO BUMP, 1-F=INTENSITY */
#define CARPTSTR 52 /* SD	TRACK PIECE	} POSITION */
#define CARPTSDIST 53 /* FL	DISTANCE	}  TRACKING */
#define CARPTSRANK 54 /* UD	RANK		}   SYSTEM */
#define CARTRAK 55 /* UH	CLOSEST TRACK PIECE */
#define CARGEAR 56 /* UH	GEAR 0=NEUTRAL,1,2,3,4 */
#define CARRPM 57 /* FL     RPMX100 (ABOUT 0.0 to 50.0) */
#define CARXPLUS 58 /* FL	CAR BOX X EXTREMES */
#define CARXMINUS 59 /* FL	CAR BOX X EXTREMES */
#define CARYPLUS 60 /* FL	CAR BOX Y EXTREMES */
#define CARYMINUS 61 /* FL	CAR BOX Y EXTREMES */
#define CARZPLUS 62 /* FL	CAR BOX Z EXTREMES */
#define CARZMINUS 63 /* FL	CAR BOX Z EXTREMES */
#define CAR_ID 64 /* UH	DEBUGGING CAR ID */
#define CARSHAD 65 /* UH	CAR SHADOW FLAG	ON=1, 0=OFF */
#define CARXLEAN 66 /* FL	X BODY LEAN */
#define CARZLEAN 67 /* FL	Z BODY LEAN */
#define CARRDFR 68 /* FL	ROAD FRICTION */
#define CAROFRDFR 69 /* FL	OFFROAD FRICTION */
#define CARDIST2CNTR 70 /* FL	DISTANCE TO CENTER OF ROAD */
#define CARWHLTAB 71 /* FL	WHEEL ROAD SCAN OFFSETS */
#define CARTRANS 86 /* UH	0=AUTO, 1=MANUAL TRANSMISSION */
#define CARTRACK_ID 87 /* UH	ID OF CURRENT TRACK SECTION */
#define CAR_OM 88 /* UH	OTHER MACHINE CONTROLS CAR (LINKING) */
#define CARNUM 89 /* UH	CAR DISPATCH # 0-127, B7=MASTER */
#define CARUPD 90 /* UH     CAR HAS BEEN COMM UPDATED THIS FRAME */
#define CARSIZ 91 /* SZ	TOTAL SIZE OF CAR STRUCTURE */
#define NUM_CARS 25 /* NUMBER OF CARS ACTIVE */
#define FEET 48
#define VEHTAB_MODEL 0 /* UH	ROM PTR TO MODEL */
#define VEHTAB_PAL 1 /* UH	MODEL PTR */
#define VEHTAB_ANI 2 /* UH	ANIMATION TABLE (0 IF NONE) */
#define VEHTAB_DMODEL 3 /* UH	DEGRADED MODEL (0 IF NONE) */
#define VEHTAB_DMODEL2 4 /* UH	DEGRADED MODEL (0 IF NONE) LEVEL 2 */
#define VEHTAB_TAILCNT 5 /* UH */
#define VEHTAB_MASS 6 /* FL	MASS OF MODEL */
#define VEHTAB_PASSBY 7 /* UH	SND PTR TO PASSBY CALL */
#define VEHTAB_BNOISE 8 /* UH	SND PTR TO BACKGRND CALL */
#define VEHTAB_PALTAB 9 /* UH	PALETTE TABLE PTR */
#define VEHTAB_DDYNATAB 10 /* UH	DRONE DYNAMIC TABLE PTR */
#define VEHTAB_SIZE 11 /* SIZ */
#define LEG_POSX 0 /* UH */
#define LEG_POSY 1 /* UH ALSO HAS RIGHTMOST BIT 0 = 4 LANE 1 = 2 LANE */
#define LEG_POSZ 2 /* UH */
#define LEG_ID 3 /* UH */
#define LEG_SIZE 4 /* SI */
#define MAX_LEG_ELEMENTS 1600
#define GT_ETIME 0 /* TC	ELAPSED TIME (TIME CODED) */
#define GT_POS 1 /* UD	POSITION */
#define GT_MAXMPH 2 /* UD	MAX MPH */
#define GT_COLLS 3 /* UD	NUMBER OF COLLISIONS */
#define GT_SIZE 4 /* SIZ */
#define DGRP_HEAD 0 /* UH	PTR TO HEAD OBJECT OF THE GROUP */
#define DGRP_BIN 1 /* UH	PTR TO BINARY ROM STRUCT */
#define DGRP_FSTART 2 /* UH	fLEX START */
#define DGRP_FLAG 3 /* UH	FLAGS (see below) */
#define DGRP_IDX 4 /* UH	INDEX NUMBER */
#define DGRP_SIZE 5 /* SI	sizeof(STRUCT DGROUP_S) */
#define MAX_DGROUPS 20 /* MAXIMUM DGROUPS ACTIVE */
#define TB_FLAG 0 /* UH */
#define TB_POSX 1 /* FL */
#define TB_POSY 2 /* FL */
#define TB_POSZ 3 /* FL */
#define TB_RADY 4 /* FL */
#define TB_GROUP 5 /* UH */
#define TB_GROUPOVERLAY 6 /* UH */
#define TB_REGSIZE 7 /* SIZ */
#define TB_RVS_POSX 7 /* FL */
#define TB_RVS_POSY 8 /* FL */
#define TB_RVS_POSZ 9 /* FL */
#define TB_RVS_RADY 10 /* FL */
#define TB_RVSSIZE 11 /* SIZ */
#define TB_OVER2 12 /* or 6 or 7 */
#define SC_OVERLAY 0x00001
#define SC_BRANCH 0x00002
#define SC_2NDSTART 0x00004
#define SC_REVERSE 0x00008
#define SC_ROADMODEL 0x00070
#define SC_OVERDSC 0x08000 /* USE CURRENT OVERLAY 1 or 2 */
#define SC_OVER2 0x01000
#define SC_LDSECT 0x01000 /* LOAD A NEW SECTION INTO THE SYSTEM */
#define SC_LDDEST 0x06000 /* WHICH OVERLAY TO LOAD INTO (1 or 2) */
#define SC_LDSECT_IDX 0x00F00 /* WHICH LOOK TO LOAD INTO OVERLAY SPACE */
#define SC_ROUTINE 0x000FF0000 /* IF NON-ZERO ROUTINE SHOULD BE CALLED */
#define AUTO_TRANSMISSION 0
#define MANUAL_TRANSMISSION 1
#define PI 3.141592654
#define TWOPI 6.283185307
#define HALFPI 1.570796327
#define HALFPI3 4.71238898
#define PLY_STATUS 0x00
#define PLY_CAR 0x01
#define PLY_PROC 0x02
#define PLY_SIZ 0x03
#define PLY_CBLK 0x04
#define BUT_START 1
#define BUT_VIEW1 2
#define BUT_VIEW2 4
#define BUT_VIEW3 8
#define BUT_VIEWS 0x0E
#define BUT_TAILS 0x030
#define BUT_FRONT 0x0C0
#define BGD_BIGOBJ 0x01000
#define BGD_ROUTINE 0x02000
#define BGD_PALETTE 0x04000
#define MAX_VERTICES 256
#define MAX_POLYGONS 300 /* ACTUALLY UNLIMITED AS LONG AS CONVEX */
#define FP AR3
#define SCRNHX 256.0 /* HALF OF SCREEN X SIZE FLOAT */
#define SCRNHY 200.0 /* HALF OF SCREEN Y SIZE FLOAT */
#define SCREENHX 256 /* HALF OF SCREEN X SIZE INTEGER */
#define SCREENHY 200 /* HALF OF SCREEN Y SIZE INTEGER */
#define X 0
#define Y 1
#define Z 2
#define W 3
#define A 0
#define B 1
#define C 2
#define D 3
#define A00 0
#define A01 1
#define A02 2
#define A10 3
#define A11 4
#define A12 5
#define A20 6
#define A21 7
#define A22 8
#define DYNALINK 00 /* link to next DYNAOBJ */
#define DYNACENTERX 01 /* the center of the vertices */
#define DYNACENTERY 02
#define DYNACENTERZ 03
#define DYNATRANSX 04 /* the translation of the vertices */
#define DYNATRANSY 05
#define DYNATRANSZ 06
#define DYNAMATRIX 07 /* the matrix of the vertices */
#define DYNAMAT00 07
#define DYNAMAT10 08
#define DYNAMAT20 09
#define DYNAMAT01 0x0A
#define DYNAMAT11 0x0B
#define DYNAMAT21 0x0C
#define DYNAMAT02 0x0D
#define DYNAMAT12 0x0E
#define DYNAMAT22 0x0F
#define DYNANVERTS 0x10
#define DYNAPARENT 0x11
#define DYNAFLAG 0x12
#define DYNASIZE 0x13
#define NUM_DYNAS (NUM_CARS*7)
#define PRIORITY 0x00FF
#define CHAN0 0x0000
#define CHAN1 0x0100
#define CHAN2 0x0200
#define CHAN3 0x0300
#define NINT 0x1000 /* NON INTERRUPTABLE */
#define ENINT 0x2000 /* NON INTERRUPTABLE BY EQUAL */
#define NCHAN 4
#define SND_PRI 0 /* UH	priority (00=lowest, FF= highest) */
#define SND_TIM 1 /* UH	TIMER */
#define SND_TMR 2 /* UH	TIMER COUNTDOWN */
#define SND_ST 3 /* UH	ADDRESS OF SOUND TABLE (START) */
#define SND_ADDR 4 /* UH	ADDRESS OF SOUND TABLE (CURRENT POINTER) */
#define SND_VOL 5 /* SD	VOLUME OF TRACK */
#define SND_IDX 6 /* UH	INDEX OF SOUND */
#define SND_REP 7 /* UH	REPEAT COUNT OF SOUND */
#define SND_STATUS 8 /* UH	FLAG FX SND increment ? */
#define SND_SIZ 9 /* SIZ */
#define NUM_STATIONS 4
#define SURFIN_STAT 0
#define HARDROCK_STAT 1
#define COUNTRY_STAT 2
#define RAP_STAT 3

#endif /* SYS_H */

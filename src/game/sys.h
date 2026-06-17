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

// asm: TS_SIZE		.set	1
#define TS_SIZE 1

// ----------------------------------------------------------------------------
// SYSTEM MODE FLAGS/VALUES
// 
// asm: MBRIDGE		.set	00800h		;ON THE GG BRIDGE
#define MBRIDGE 0x00800 //ON THE GG BRIDGE

// asm: MINTUNNEL	.set	00400h		;INSIDE THE TUNNEL
#define MINTUNNEL 0x00400 //INSIDE THE TUNNEL

// asm: MGO		.set	00200h		;GO! RACE is ACTIVE
#define MGO 0x00200 //GO! RACE is ACTIVE

// asm: MHUD		.set	00100h		;HUD on
#define MHUD 0x00100 //HUD on

// asm: MWATER		.set	00080h		;INFINITY WATER on
#define MWATER 0x00080 //INFINITY WATER on

// asm: MINFIN		.set	00040h		;INFINITY SKY on
#define MINFIN 0x00040 //INFINITY SKY on

// asm: MHS		.set	00020h		;HS High score entry display
#define MHS 0x00020 //HS High score entry display

// asm: MSLINE		.set	00010h		;STARTING LINE
#define MSLINE 0x00010 //STARTING LINE

// asm: MMODE		.set	0000Fh		;MASK main mode types
#define MMODE 0x0000F //MASK main mode types

// asm: MINACTIVE	.set	00000h		;SY network inactive node
#define MINACTIVE 0x00000 //SY network inactive node

// asm: MDIAG		.set	00001h		;SY DIAGNOSTICS
#define MDIAG 0x00001 //SY DIAGNOSTICS

// asm: MATTR		.set	00002h		;SY ATTRACT
#define MATTR 0x00002 //SY ATTRACT

// asm: MINTRO		.set	00003h		;GM the INTRO (GAME)
#define MINTRO 0x00003 //GM the INTRO (GAME)

// asm: MGAME		.set	00004h		;GM the GAME
#define MGAME 0x00004 //GM the GAME

// asm: MBONUS		.set	00005h		;GM the BONUS screen
#define MBONUS 0x00005 //GM the BONUS screen

// asm: MINIT		.set	00006h		;GM entering initials
#define MINIT 0x00006 //GM entering initials

// asm: MINSERT_COINS	.set	00007h		;GM asking the questions more coins...
#define MINSERT_COINS 0x00007 //GM asking the questions more coins...

// asm: MCT		.set	00008h		;GM the choose transmission
#define MCT 0x00008 //GM the choose transmission

// ----------------------------------------------------------------------------

// asm: STARTING_TIME	.set	90		;(INTRO.ASM,BONUS.ASM)
#define STARTING_TIME 90 //(INTRO.ASM,BONUS.ASM)

// asm: MPH_CONVERSION	.set	0.49
#define MPH_CONVERSION 0.49

// asm: IMPH_CONVERSION	.set	2.04	;1/0.49
#define IMPH_CONVERSION 2.04 //1/0.49

// CAR BLOCK STRUCTURE
// 
// asm: CARVNUM		.set	5	;	#OF CAR ROAD CONTACT POINTS
#define CARVNUM 5 //#OF CAR ROAD CONTACT POINTS

// asm: CARVSIZ		.set	6	;	CAR POINT STRUCT SIZE
#define CARVSIZ 6 //CAR POINT STRUCT SIZE

// asm: CARPX		.set	0	;FL	CAR SUSPENSION PT X
#define CARPX 0 //FL	CAR SUSPENSION PT X

// asm: CARPY		.set	1	;FL	CAR SUSPENSION PT Y
#define CARPY 1 //FL	CAR SUSPENSION PT Y

// asm: CARPZ		.set	2	;FL	CAR SUSPENSION PT Z
#define CARPZ 2 //FL	CAR SUSPENSION PT Z

// asm: CARPRDYD	.set	3	;FL	DELTA Y TO ROAD
#define CARPRDYD 3 //FL	DELTA Y TO ROAD

// asm: CARPYV		.set	4	;FL	Y VELOCITY
#define CARPYV 4 //FL	Y VELOCITY

// asm: CARPCOL		.set	5	;UH	ROAD OBJECT COLLIDED WITH
#define CARPCOL 5 //UH	ROAD OBJECT COLLIDED WITH

// asm: NUM_CARS	.set	25		;NUMBER OF CARS ACTIVE
#define NUM_CARS 25 //NUMBER OF CARS ACTIVE

// asm: FEET		.set	48
#define FEET 48

// THIS IS THE LEG MAP
// 
// 
// asm: LEG_SIZE	.set	4	;SI
#define LEG_SIZE 4 //SI

// asm: MAX_LEG_ELEMENTS	.set	1600
#define MAX_LEG_ELEMENTS 1600

// STATISTICAL STRUCTURE FOR THE GAME LEGS <NERD DATA>
// 
// 
// 

// 
// this structure has hard coded routines which depend on the
// size being a known constant. see backgrnd.asm.  do not alter
// the size of this structure.
// 
// asm: DGRP_SIZE	.set	5	;SI	sizeof(STRUCT DGROUP_S)
#define DGRP_SIZE 5 //SI	sizeof(STRUCT DGROUP_S)

// asm: MAX_DGROUPS	.set	20	;MAXIMUM DGROUPS ACTIVE
#define MAX_DGROUPS 20 //MAXIMUM DGROUPS ACTIVE

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
#define SC_OVERDSC 0x08000 //USE CURRENT OVERLAY 1 or 2

// asm: SC_OVER2	.set	01000h
#define SC_OVER2 0x01000

// asm: SC_LDSECT	.set	01000h		;LOAD A NEW SECTION INTO THE SYSTEM
#define SC_LDSECT 0x01000 //LOAD A NEW SECTION INTO THE SYSTEM

// asm: SC_LDDEST	.set	06000h		;WHICH OVERLAY TO LOAD INTO (1 or 2)
#define SC_LDDEST 0x06000 //WHICH OVERLAY TO LOAD INTO (1 or 2)

// asm: SC_LDSECT_IDX	.set	00F00h		;WHICH LOOK TO LOAD INTO OVERLAY SPACE
#define SC_LDSECT_IDX 0x00F00 //WHICH LOOK TO LOAD INTO OVERLAY SPACE

// asm: SC_ROUTINE	.set	000FF0000h	;IF NON-ZERO ROUTINE SHOULD BE CALLED
#define SC_ROUTINE 0x000FF0000 //IF NON-ZERO ROUTINE SHOULD BE CALLED

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
#define MAX_POLYGONS 300 //ACTUALLY UNLIMITED AS LONG AS CONVEX

// asm: FP		.set	AR3
#define FP AR3

// asm: SCRNHX		.set	256.0		;HALF OF SCREEN X SIZE FLOAT
#define SCRNHX 256.0 //HALF OF SCREEN X SIZE FLOAT

// asm: SCRNHY		.set	200.0		;HALF OF SCREEN Y SIZE FLOAT
#define SCRNHY 200.0 //HALF OF SCREEN Y SIZE FLOAT

// asm: SCREENHX	.set	256		;HALF OF SCREEN X SIZE INTEGER
#define SCREENHX 256 //HALF OF SCREEN X SIZE INTEGER

// asm: SCREENHY	.set	200		;HALF OF SCREEN Y SIZE INTEGER
#define SCREENHY 200 //HALF OF SCREEN Y SIZE INTEGER

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
// asm: DYNASIZE	.set	13h
#define DYNASIZE 0x13

// asm: NUM_DYNAS	.set	NUM_CARS*7
#define NUM_DYNAS (NUM_CARS*7)

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
#define NINT 0x1000 //NON INTERRUPTABLE

// asm: ENINT		.set	2000h		;NON INTERRUPTABLE BY EQUAL
#define ENINT 0x2000 //NON INTERRUPTABLE BY EQUAL

// asm: NCHAN		.set	4
#define NCHAN 4

// asm: SND_SIZ		.set	9		;SIZ
#define SND_SIZ 9 //SIZ

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
extern int LEG_MAP[];

#endif /* SYS_H */

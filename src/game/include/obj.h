#ifndef OBJ_H
#define OBJ_H

/* Generated from asm/OBJ.EQU. */

// OBJ.EQU
// 
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED
// 
// ROUTINES

// asm: 	.globl	ZSORTWT,RESCAN
void ZSORTWT(void);
void RESCAN(void);
// asm: 	.globl	OSCAN,ISCAN
void OSCAN(void);
void ISCAN(void);
// asm: 	.globl	QMAKEOBJ
// asm: 	.globl	ZSORTPRIOR
void ZSORTPRIOR(void);
// asm: 	.globl	OBJ_FREE_PROC
void OBJ_FREE_PROC(void);
// asm: 	.globl	OBJ_FREE_DRIVE
void OBJ_FREE_DRIVE(void);
// asm: 	.globl	OBJ_FREE_GROUND
void OBJ_FREE_GROUND(void);
// asm: 	.globl	OBJ_FREE_SIGN
void OBJ_FREE_SIGN(void);
// asm: 	.globl	OBJ_INIT
void OBJ_INIT(void);
// asm: 	.globl	OBJ_GET
void OBJ_GET(void);
// asm: 	.globl	OBJ_GETE
void OBJ_GETE(void);
// asm: 	.globl	OBJ_INSERT
void OBJ_INSERT(void);
// asm: 	.globl	OBJ_INSERTP
void OBJ_INSERTP(void);
// asm: 	.globl	OBJ_INSERTHP
void OBJ_INSERTHP(void);
// asm: 	.globl	OBJ_INSERTLP
void OBJ_INSERTLP(void);
// asm: 	.globl	OBJ_FIND_FIRST
void OBJ_FIND_FIRST(void);
// asm: 	.globl	OBJ_FIND_FIRST_PRIORITY
void OBJ_FIND_FIRST_PRIORITY(void);
// asm: 	.globl	OBJ_DELETE
void OBJ_DELETE(void);
// asm: 	.globl	OBJ_DELETE_CLASS
void OBJ_DELETE_CLASS(void);
// asm: 	.globl	OBJ_PULL
void OBJ_PULL(void);
// asm: 	.globl	OBJ_FREE
void OBJ_FREE(void);
// asm: 	.globl	OBJ_MAKE
void OBJ_MAKE(void);
// asm: 	.globl	OBJ_QMAKE
void OBJ_QMAKE(void);

// DATA STRUCTURES
// asm: 	.globl	OACTIVE,OFREE,IDLE_LIST,OACTIVE_PRIORITY,OLOW_PRIORITY
extern int OACTIVE;
extern int OFREE;
extern int IDLE_LIST;
extern int OACTIVE_PRIORITY;
extern int OLOW_PRIORITY;
// asm: 	.globl	DRIVE_LIST,_CAR_LIST,BUILD_LIST,SIGN_LIST
extern int DRIVE_LIST;
extern int SIGN_LIST;
// asm: 	.globl	OACTIVECNT,OFREECNT,OMAX_OBJECTS
extern int OACTIVECNT;
extern int OFREECNT;
extern int OMAX_OBJECTS;
// asm: 	.globl	OLOW_PRIORITY,OHIGH_PRIORITY
extern int OHIGH_PRIORITY;
// asm: 	.globl	OFREEI
// asm: 	.globl	OBJSTR,OBJSTRI
extern int OBJSTR[OBJSIZ*NUM_OBJECTS];

// asm: NUM_OBJECTS	.set	1100
#define NUM_OBJECTS 1100

// asm: DEGRADE_DIST	.set	8000
#define DEGRADE_DIST 8000
// asm: DEGRADE_DIST_LEVEL2	.set	15000
#define DEGRADE_DIST_LEVEL2 15000

// ----------------------------------------------------------------------------
// EXTENDED OBJECT FLAGS
// STORED AT:	*(ROMDATA-1)
// 
// 
// 31   24     16	      8		      0 bits
// --------------------------------------
// |    |      | flags | palette/illum |
// |    |      |	      |	              |
// 
// 
// asm: EOBJ_ILLUM	.set	1000h	;ILLUMINATED MODEL
#define EOBJ_ILLUM 0x1000 //ILLUMINATED MODEL
// asm: EOBJ_1PAL	.set	2000h	;SINGLE PALETTE OBJECT
#define EOBJ_1PAL 0x2000 //SINGLE PALETTE OBJECT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// OBJECT FLAGS
// 
// KEEP FAST FLAGS IN LOWER 15 BITS FOR SINGLE INSTRUCTION MASKS
// 
// 
// asm: O_NOUROT	.set	00001h		;object has absolute coors from UNIVPOS
#define O_NOUROT 0x00001 //object has absolute coors from UNIVPOS
// asm: O_IROT		.set	00002h		;object has identity rotation (always facing front)
#define O_IROT 0x00002 //object has identity rotation (always facing front)
// this must be set w/O_NOUROT
// asm: O_DEGRADE2	.set	00004h		;degradable Level 2 object
#define O_DEGRADE2 0x00004 //degradable Level 2 object
// asm: O_POSTER	.set	00008h		;poster object
#define O_POSTER 0x00008 //poster object
// asm: O_NOCOLL	.set	00010h		;object NOT collidable
#define O_NOCOLL 0x00010 //object NOT collidable
// asm: O_NOUNIV	.set	00020h		;object has absolute distance from UNIVPOS
#define O_NOUNIV 0x00020 //object has absolute distance from UNIVPOS
// asm: O_ILLUM		.set	00040h		;object is facet shaded
#define O_ILLUM 0x00040 //object is facet shaded
// asm: O_NOROT		.set	00080h		;object is NOT self rotating
#define O_NOROT 0x00080 //object is NOT self rotating
// asm: O_NOROTS	.set	O_NOUROT|O_IROT|O_NOROT
#define O_NOROTS (O_NOUROT|O_IROT|O_NOROT)

// asm: O_ILLUMCOL_M	.set  0FF0000h		;mask for illumination color (O_ILLUM must be ON)
#define O_ILLUMCOL_M 0x0FF0000 //mask for illumination color (O_ILLUM must be ON)
// asm: O_ANI		.set	00100h		;object is on animation list
#define O_ANI 0x00100 //object is on animation list
// asm: O_DEGRADE	.set	00200h		;degradable object
#define O_DEGRADE 0x00200 //degradable object
// asm: O_1PAL		.set	00400h		;single paletted object
#define O_1PAL 0x00400 //single paletted object
// asm: O_DYNAMIC	.set	00800h		;dynamic animation object
#define O_DYNAMIC 0x00800 //dynamic animation object

// asm: O_GENVSPEC	.set	O_NOUROT|O_IROT|O_POSTER|O_NOCOLL|O_NOUNIV
#define O_GENVSPEC (O_NOUROT|O_IROT|O_POSTER|O_NOCOLL|O_NOUNIV)
// flags specifiable by GENV system

// LIST FLAGS
// *************************************************
// ***THESE FLAGS SHOULD !NOT! BE SET BY THE USER***
// ***THEY MUST ONLY BE SET BY THE SYSTEM	       ***
// ***(INSOBJ,DELOBJ)			       ***
// *************************************************
// asm: O_LIST_M	.set	03000h		;
#define O_LIST_M 0x03000
// asm: O_LIST0		.set	00000h		;list 0 OFREE list
#define O_LIST0 0x00000 //list 0 OFREE list
// asm: O_LIST1		.set	01000h		;list 1 OACTIVE list
#define O_LIST1 0x01000 //list 1 OACTIVE list
// asm: O_LIST2		.set	02000h		;list 2	OIDLE_LIST list
#define O_LIST2 0x02000 //list 2	OIDLE_LIST list
// asm: O_LIST3		.set	03000h		;list 3	OACTIVE_PRIORITY list
#define O_LIST3 0x03000 //list 3	OACTIVE_PRIORITY list
// *************************************************

// asm: O_GROUND	.set	000200000h	;is on the supplimental ground list
#define O_GROUND 0x000200000 //is on the supplimental ground list
// asm: O_GROUND_B	.set	21
#define O_GROUND_B 21
// asm: O_DEBRIS	.set	000400000h	;is on the supplimental road debris list
#define O_DEBRIS 0x000400000 //is on the supplimental road debris list
// asm: O_DEBRIS_B	.set	22
#define O_DEBRIS_B 22
// asm: O_PROC		.set	000800000h	;has a process associated
#define O_PROC 0x000800000 //has a process associated
// asm: O_PROC_B	.set	23
#define O_PROC_B 23
// O_OFFSET	.set	002000000h	;PHOENIX SYSTEM OBJECT
// O_OFFSET_B	.set	25
// asm: O_BIGOBJECT	.set	004000000h	;dont clip on Y to great
#define O_BIGOBJECT 0x004000000 //dont clip on Y to great
// asm: O_BIGOBJECT_B	.set	26
#define O_BIGOBJECT_B 26
// asm: O_3DROT		.set	008000000h	;object is 3d rotated
#define O_3DROT 0x008000000 //object is 3d rotated
// asm: O_3DROT_B	.set	27
#define O_3DROT_B 27
// asm: O_DRIVE_SUPP	.set	010000000h	;is on the supplimental road list
#define O_DRIVE_SUPP 0x010000000 //is on the supplimental road list
// asm: O_DRIVE_SUPP_B	.set	28
#define O_DRIVE_SUPP_B 28
// asm: O_CAR_SUPP	.set	020000000h	;is on the supplimental vehicle list
#define O_CAR_SUPP 0x020000000 //is on the supplimental vehicle list
// asm: O_CAR_SUPP_B	.set	29
#define O_CAR_SUPP_B 29
// asm: O_BUILD_SUPP	.set	040000000h	;is on the supplimental building list
#define O_BUILD_SUPP 0x040000000 //is on the supplimental building list
// asm: O_BUILD_SUPP_B	.set	30
#define O_BUILD_SUPP_B 30
// asm: O_SIGN_SUPP	.set	080000000h	;in on the supplimental sign list
#define O_SIGN_SUPP 0x080000000 //in on the supplimental sign list
// asm: O_SIGN_SUPP_B	.set	31
#define O_SIGN_SUPP_B 31
// ----------------------------------------------------------------------------

// STRUCT	OBJ
// asm: OLINK	 	.set	00h	;UH
#define OLINK 0x00 //UH
// asm: OPOSX	 	.set	01h	;FL
#define OPOSX 0x01 //FL
// asm: OPOSY	 	.set	02h	;FL
#define OPOSY 0x02 //FL
// asm: OPOSZ	 	.set	03h	;FL
#define OPOSZ 0x03 //FL
// asm: OMATRIX	 	.set	04h	;
#define OMATRIX 0x04
// asm: OMAT00	 	.set	04h	;
#define OMAT00 0x04
// asm: OMAT10	 	.set	05h	;
#define OMAT10 0x05
// asm: OMAT20	 	.set	06h	;
#define OMAT20 0x06
// asm: OMAT01	 	.set	07h	;
#define OMAT01 0x07
// asm: OMAT11	 	.set	08h	;
#define OMAT11 0x08
// asm: OMAT21	 	.set	09h	;
#define OMAT21 0x09
// asm: OMAT02	 	.set	0Ah	;
#define OMAT02 0x0A
// asm: OMAT12	 	.set	0Bh	;
#define OMAT12 0x0B
// asm: OMAT22	 	.set	0Ch	;
#define OMAT22 0x0C
// asm: OROMDATA 	.set	0Dh	;UH
#define OROMDATA 0x0D //UH
// asm: OFLAGS	 	.set	0Eh	;UH
#define OFLAGS 0x0E //UH
// asm: OID		.set	0Fh	;UH
#define OID 0x0F //UH
// asm: OPAL		.set	10h	;UH	for single paletted model
#define OPAL 0x10 //UH	for single paletted model
// 
// asm: OVELX	 	.set	11h	;FL
#define OVELX 0x11 //FL
// asm: OVELY	 	.set	12h	;FL
#define OVELY 0x12 //FL
// asm: OVELZ	 	.set	13h	;FL
#define OVELZ 0x13 //FL
// 
// asm: ORADX	 	.set	14h	;FL
#define ORADX 0x14 //FL
// asm: ORADY	 	.set	15h	;FL
#define ORADY 0x15 //FL
// asm: ORADZ	 	.set	16h	;FL
#define ORADZ 0x16 //FL
// asm: OPLINK	 	.set	17h	;UH
#define OPLINK 0x17 //UH
// asm: OBLINK4		 .set	 17h	;	used in BACKGRND.ASM
#define OBLINK4 0x17 //used in BACKGRND.ASM
// asm: ODEGRADE_ROM	.set	18h	;UH
#define ODEGRADE_ROM 0x18 //UH
// asm: ODEGRADE_ROM2	.set	19h	;UH
#define ODEGRADE_ROM2 0x19 //UH
// asm: OROMDATA2	.set	1Ah	;UH	for models which interpolate or are dynamic objects
#define OROMDATA2 0x1A //UH	for models which interpolate or are dynamic objects
// asm: ODYNALIST	 .set	 1Ah	;	just another name for OROMDATA2
#define ODYNALIST 0x1A //just another name for OROMDATA2
// asm: OCARBLK		.set	1Bh	;UH	VEHICLE BLOCK POINTER (for DRONES/PLAYERS/VEHICLES)
#define OCARBLK 0x1B //UH	VEHICLE BLOCK POINTER (for DRONES/PLAYERS/VEHICLES)
// asm: OANIBLK		 .set	 1Bh	;	for animating objects
#define OANIBLK 0x1B //for animating objects
// asm: ODIST	 	.set	1Ch	;SD	DISTANCE FROM CAMERA POSITION IN Z
#define ODIST 0x1C //SD	DISTANCE FROM CAMERA POSITION IN Z
// asm: ORAD	 	.set	1Dh	;SD	RADIUS of OBJECT
#define ORAD 0x1D //SD	RADIUS of OBJECT
// asm: OUSR1		.set	1Eh	;UH	USER VARIABLE 1 (FORMATION LEAD)
#define OUSR1 0x1E //UH	USER VARIABLE 1 (FORMATION LEAD)
// asm: OLINK2		.set	1Fh	;UH	reserved for BACKGRND GROUP (DE)/ALLOCATION
#define OLINK2 0x1F //UH	reserved for BACKGRND GROUP (DE)/ALLOCATION
// asm: OLINK3		.set	20h	;UH	reserved for type (ie road/drone etc)
#define OLINK3 0x20 //UH	reserved for type (ie road/drone etc)
// asm: OLINK4		.set	21h	;UH	reserved for fLEX MAIN LIST
#define OLINK4 0x21 //UH	reserved for fLEX MAIN LIST
// asm: OBJSIZ		.set   	22h	;SIZ	OBJECT BLOCK SIZE
#define OBJSIZ 0x22 //SIZ	OBJECT BLOCK SIZE
// ENDSTRUCT

// asm: OUSR2		.set	OLINK2
#define OUSR2 OLINK2
// asm: OUSR3		.set	OLINK3
#define OUSR3 OLINK3

#endif /* OBJ_H */

#ifndef OBJ_H
#define OBJ_H

/* Generated from asm/OBJ.EQU. */
/*
 * OBJ.EQU
 * COPYRIGHT (C) 1994  BY TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * ROUTINES
 * DATA STRUCTURES
 * ----------------------------------------------------------------------------
 * EXTENDED OBJECT FLAGS
 * STORED AT:	*(ROMDATA-1)
 * ...
 */

/* Original .globl symbols in this module:
 *   ZSORTWT
 *   RESCAN
 *   OSCAN
 *   ISCAN
 *   QMAKEOBJ
 *   ZSORTPRIOR
 *   OBJ_FREE_PROC
 *   OBJ_FREE_DRIVE
 *   OBJ_FREE_GROUND
 *   OBJ_FREE_SIGN
 *   OBJ_INIT
 *   OBJ_GET
 *   OBJ_GETE
 *   OBJ_INSERT
 *   OBJ_INSERTP
 *   OBJ_INSERTHP
 *   OBJ_INSERTLP
 *   OBJ_FIND_FIRST
 *   OBJ_FIND_FIRST_PRIORITY
 *   OBJ_DELETE
 *   OBJ_DELETE_CLASS
 *   OBJ_PULL
 *   OBJ_FREE
 *   OBJ_MAKE
 *   OBJ_QMAKE
 *   OACTIVE
 *   OFREE
 *   IDLE_LIST
 *   OACTIVE_PRIORITY
 *   OLOW_PRIORITY
 *   DRIVE_LIST
 *   _CAR_LIST
 *   BUILD_LIST
 *   SIGN_LIST
 *   OACTIVECNT
 *   OFREECNT
 *   OMAX_OBJECTS
 *   OLOW_PRIORITY
 *   OHIGH_PRIORITY
 *   OFREEI
 *   OBJSTR
 *   OBJSTRI
 */

#define NUM_OBJECTS 1100
#define DEGRADE_DIST 8000
#define DEGRADE_DIST_LEVEL2 15000
#define EOBJ_ILLUM 0x1000 /* ILLUMINATED MODEL */
#define EOBJ_1PAL 0x2000 /* SINGLE PALETTE OBJECT */
#define O_NOUROT 0x00001 /* object has absolute coors from UNIVPOS */
#define O_IROT 0x00002 /* object has identity rotation (always facing front) */
#define O_DEGRADE2 0x00004 /* degradable Level 2 object */
#define O_POSTER 0x00008 /* poster object */
#define O_NOCOLL 0x00010 /* object NOT collidable */
#define O_NOUNIV 0x00020 /* object has absolute distance from UNIVPOS */
#define O_ILLUM 0x00040 /* object is facet shaded */
#define O_NOROT 0x00080 /* object is NOT self rotating */
#define O_NOROTS (O_NOUROT|O_IROT|O_NOROT)
#define O_ILLUMCOL_M 0x0FF0000 /* mask for illumination color (O_ILLUM must be ON) */
#define O_ANI 0x00100 /* object is on animation list */
#define O_DEGRADE 0x00200 /* degradable object */
#define O_1PAL 0x00400 /* single paletted object */
#define O_DYNAMIC 0x00800 /* dynamic animation object */
#define O_GENVSPEC (O_NOUROT|O_IROT|O_POSTER|O_NOCOLL|O_NOUNIV)
#define O_LIST_M 0x03000
#define O_LIST0 0x00000 /* list 0 OFREE list */
#define O_LIST1 0x01000 /* list 1 OACTIVE list */
#define O_LIST2 0x02000 /* list 2	OIDLE_LIST list */
#define O_LIST3 0x03000 /* list 3	OACTIVE_PRIORITY list */
#define O_GROUND 0x000200000 /* is on the supplimental ground list */
#define O_GROUND_B 21
#define O_DEBRIS 0x000400000 /* is on the supplimental road debris list */
#define O_DEBRIS_B 22
#define O_PROC 0x000800000 /* has a process associated */
#define O_PROC_B 23
#define O_BIGOBJECT 0x004000000 /* dont clip on Y to great */
#define O_BIGOBJECT_B 26
#define O_3DROT 0x008000000 /* object is 3d rotated */
#define O_3DROT_B 27
#define O_DRIVE_SUPP 0x010000000 /* is on the supplimental road list */
#define O_DRIVE_SUPP_B 28
#define O_CAR_SUPP 0x020000000 /* is on the supplimental vehicle list */
#define O_CAR_SUPP_B 29
#define O_BUILD_SUPP 0x040000000 /* is on the supplimental building list */
#define O_BUILD_SUPP_B 30
#define O_SIGN_SUPP 0x080000000 /* in on the supplimental sign list */
#define O_SIGN_SUPP_B 31
#define OLINK 0x00 /* UH */
#define OPOSX 0x01 /* FL */
#define OPOSY 0x02 /* FL */
#define OPOSZ 0x03 /* FL */
#define OMATRIX 0x04
#define OMAT00 0x04
#define OMAT10 0x05
#define OMAT20 0x06
#define OMAT01 0x07
#define OMAT11 0x08
#define OMAT21 0x09
#define OMAT02 0x0A
#define OMAT12 0x0B
#define OMAT22 0x0C
#define OROMDATA 0x0D /* UH */
#define OFLAGS 0x0E /* UH */
#define OID 0x0F /* UH */
#define OPAL 0x10 /* UH	for single paletted model */
#define OVELX 0x11 /* FL */
#define OVELY 0x12 /* FL */
#define OVELZ 0x13 /* FL */
#define ORADX 0x14 /* FL */
#define ORADY 0x15 /* FL */
#define ORADZ 0x16 /* FL */
#define OPLINK 0x17 /* UH */
#define OBLINK4 0x17 /* used in BACKGRND.ASM */
#define ODEGRADE_ROM 0x18 /* UH */
#define ODEGRADE_ROM2 0x19 /* UH */
#define OROMDATA2 0x1A /* UH	for models which interpolate or are dynamic objects */
#define ODYNALIST 0x1A /* just another name for OROMDATA2 */
#define OCARBLK 0x1B /* UH	VEHICLE BLOCK POINTER (for DRONES/PLAYERS/VEHICLES) */
#define OANIBLK 0x1B /* for animating objects */
#define ODIST 0x1C /* SD	DISTANCE FROM CAMERA POSITION IN Z */
#define ORAD 0x1D /* SD	RADIUS of OBJECT */
#define OUSR1 0x1E /* UH	USER VARIABLE 1 (FORMATION LEAD) */
#define OLINK2 0x1F /* UH	reserved for BACKGRND GROUP (DE)/ALLOCATION */
#define OLINK3 0x20 /* UH	reserved for type (ie road/drone etc) */
#define OLINK4 0x21 /* UH	reserved for fLEX MAIN LIST */
#define OBJSIZ 0x22 /* SIZ	OBJECT BLOCK SIZE */
#define OUSR2 OLINK2
#define OUSR3 OLINK3

#endif /* OBJ_H */

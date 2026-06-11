#ifndef BACKGRND_H
#define BACKGRND_H

/* Generated from asm/BACKGRND.ASM. */

// asm: 	.bss	STARTSECTION,1		;Section index to start game at
// addr: 0x0000E42E
extern int STARTSECTION;
// asm: 	.bss	START_POS,3   		;Section X Y Z start (if DEBUGSECTION != 0)
// addr: 0x0000E42F
extern int START_POS[3];
// asm: 	.bss	START_RADY,1  		;Section Y Radian (if DEBUG SECTION != 0)
// addr: 0x0080E432
extern int START_RADY;
// asm: 	.bss	DRIVE_LIST,1		;OBJECTs which can be driven over (road/shoulder)
// addr: 0x0000E433
extern int DRIVE_LIST;
// asm: 	.bss	CAR_LIST,1		;OBJECTs which are vehicles (drones/player car)
// addr: 0x0000E434
extern int CAR_LIST;
// asm: 	.bss	SIGN_LIST,1		;OBJECTs which are signs (posts, road signs, trees etc)
// addr: 0x0000E435
extern int SIGN_LIST;
// asm: 	.bss	GROUND_LIST,1		;OBJECTs which define the the ground but are not ROAD_C or SHLDR_C
// addr: 0x0000E436
extern int GROUND_LIST;
// asm: 	.bss	NEW_GROUP,1	;TEMP PTR TO START OF NEW DGROUP LIST
// addr: 0x0000E437
extern int NEW_GROUP;
// asm: 	.bss	DGROUPS,DGRP_SIZE*MAX_DGROUPS
// addr: 0x0000E438
extern int DGROUPS[DGRP_SIZE*MAX_DGROUPS];
// asm: 	.bss	DGROUP_COUNT,1		;NUMBER OF ACTIVE BACKGRNDS (on DYNA LIST)
// addr: 0x0000E49C
extern int DGROUP_COUNT;
// asm: 	.bss	DGROUP_AW,1		;activate watch pointer (->TYCO.ASM)
// addr: 0x0000E49D
extern int DGROUP_AW;
// asm: 	.bss	DYNALIST_TRUEBEGIN,1	;FIRST ELEMENT OF DYNAFLEX LIST
// addr: 0x0000E49E
extern int DYNALIST_TRUEBEGIN;
// asm: 	.bss	DYNALIST_BEGIN,1	;FIRST ELEMENT OF DYNAFLEX LIST (SECOND SET)
// addr: 0x0000E49F
extern int DYNALIST_BEGIN;
// asm: 	.bss	DYNALIST_END,1		;LAST ELEMENT OF DYNAFLEX LIST
// addr: 0x0000E4A0
extern int DYNALIST_END;
// asm: 	.bss	SUBLIST_BEGIN,1		;FIRST ELEMENT OF SUBLIST (OLINK 3)
// addr: 0x0000E4A1
extern int SUBLIST_BEGIN;
// asm: 	.bss	SUBLIST_END,1		;LAST ELEMENT OF SUBLIST (OLINK 4)
// addr: 0x0000E4A2
extern int SUBLIST_END;
// asm: 	.bss	STARTS,1
// addr: 0x0000E4A3
extern int STARTS;
// asm: 	.bss	SECTIONIDX,1		;section index (of next to activate)
// addr: 0x0000E4A4
extern int SECTIONIDX;
// asm: 	.bss	TYCO_TRACK,1
// addr: 0x0000E4A5
extern int TYCO_TRACK;
// asm: 	.bss	TYCO_TRACK_NTL,1
// addr: 0x0000E4A6
extern int TYCO_TRACK_NTL;
// asm: 	.bss	TYCO_NTL_IDX,1
// addr: 0x0000E4A7
extern int TYCO_NTL_IDX;
// asm: 	.bss	NEWSUBLIST_TOPB,1	;POINTER TO TOP OF SUBLIST (OF GROUP)
// addr: 0x0000406A
extern int NEWSUBLIST_TOPB;
// asm: 	.bss	TYCOFLAG,1	;CURRENT SUBLIST TYCOFLAG (FROM ROM)
// addr: 0x0000E4AA
extern int TYCOFLAG;
// asm: 	.bss	PASS1,1		;CURRENT PASS INDEX (0,1) (USED WHEN ACTIVATING AN OVERLAY)
// addr: 0x0000E4AB
extern int PASS1;
// asm: 	.bss	SECRADY,1	;SECTION RADIAN Y
// addr: 0x0000E4AC
extern int SECRADY;
// asm: 	.bss	SINGLE_SECTION_TEMPPTR,1
// addr: 0x0000E4AD
extern int SINGLE_SECTION_TEMPPTR;

#endif /* BACKGRND_H */

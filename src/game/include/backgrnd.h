#ifndef BACKGRND_H
#define BACKGRND_H

/* Generated from asm/BACKGRND.ASM. */

// asm: 	.bss	STARTSECTION,1		;Section index to start game at
#define STARTSECTION 0x0000E42E
// asm: 	.bss	START_POS,3   		;Section X Y Z start (if DEBUGSECTION != 0)
#define START_POS 0x0000E42F
// asm: 	.bss	START_RADY,1  		;Section Y Radian (if DEBUG SECTION != 0)
#define START_RADY 0x0080E432
// asm: 	.bss	DRIVE_LIST,1		;OBJECTs which can be driven over (road/shoulder)
#define DRIVE_LIST 0x0000E433
// asm: 	.bss	CAR_LIST,1		;OBJECTs which are vehicles (drones/player car)
#define CAR_LIST 0x0000E434
// asm: 	.bss	SIGN_LIST,1		;OBJECTs which are signs (posts, road signs, trees etc)
#define SIGN_LIST 0x0000E435
// asm: 	.bss	GROUND_LIST,1		;OBJECTs which define the the ground but are not ROAD_C or SHLDR_C
#define GROUND_LIST 0x0000E436
// asm: 	.bss	NEW_GROUP,1	;TEMP PTR TO START OF NEW DGROUP LIST
#define NEW_GROUP 0x0000E437
// asm: 	.bss	DGROUPS,DGRP_SIZE*MAX_DGROUPS
#define DGROUPS 0x0000E438
// asm: 	.bss	DGROUP_COUNT,1		;NUMBER OF ACTIVE BACKGRNDS (on DYNA LIST)
#define DGROUP_COUNT 0x0000E49C
// asm: 	.bss	DGROUP_AW,1		;activate watch pointer (->TYCO.ASM)
#define DGROUP_AW 0x0000E49D
// asm: 	.bss	DYNALIST_TRUEBEGIN,1	;FIRST ELEMENT OF DYNAFLEX LIST
#define DYNALIST_TRUEBEGIN 0x0000E49E
// asm: 	.bss	DYNALIST_BEGIN,1	;FIRST ELEMENT OF DYNAFLEX LIST (SECOND SET)
#define DYNALIST_BEGIN 0x0000E49F
// asm: 	.bss	DYNALIST_END,1		;LAST ELEMENT OF DYNAFLEX LIST
#define DYNALIST_END 0x0000E4A0
// asm: 	.bss	SUBLIST_BEGIN,1		;FIRST ELEMENT OF SUBLIST (OLINK 3)
#define SUBLIST_BEGIN 0x0000E4A1
// asm: 	.bss	SUBLIST_END,1		;LAST ELEMENT OF SUBLIST (OLINK 4)
#define SUBLIST_END 0x0000E4A2
// asm: 	.bss	STARTS,1
#define STARTS 0x0000E4A3
// asm: 	.bss	SECTIONIDX,1		;section index (of next to activate)
#define SECTIONIDX 0x0000E4A4
// asm: 	.bss	TYCO_TRACK,1
#define TYCO_TRACK 0x0000E4A5
// asm: 	.bss	TYCO_TRACK_NTL,1
#define TYCO_TRACK_NTL 0x0000E4A6
// asm: 	.bss	TYCO_NTL_IDX,1
#define TYCO_NTL_IDX 0x0000E4A7
// asm: 	.bss	NEWSUBLIST_TOPB,1	;POINTER TO TOP OF SUBLIST (OF GROUP)
#define NEWSUBLIST_TOPB 0x0000406A
// asm: 	.bss	TYCOFLAG,1	;CURRENT SUBLIST TYCOFLAG (FROM ROM)
#define TYCOFLAG 0x0000E4AA
// asm: 	.bss	PASS1,1		;CURRENT PASS INDEX (0,1) (USED WHEN ACTIVATING AN OVERLAY)
#define PASS1 0x0000E4AB
// asm: 	.bss	SECRADY,1	;SECTION RADIAN Y
#define SECRADY 0x0000E4AC
// asm: 	.bss	SINGLE_SECTION_TEMPPTR,1
#define SINGLE_SECTION_TEMPPTR 0x0000E4AD

#endif /* BACKGRND_H */

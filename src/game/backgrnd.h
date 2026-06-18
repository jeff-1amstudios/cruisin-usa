#ifndef BACKGRND_H
#define BACKGRND_H

#include "port.h"

/* Generated from asm/BACKGRND.ASM. */

// BACKGRND.ASM
// asm: 	.bss	STARTSECTION,1		;Section index to start game at
extern int STARTSECTION;
// asm: 	.bss	START_POS,3   		;Section X Y Z start (if DEBUGSECTION != 0)
extern int START_POS[];
// asm: 	.bss	START_RADY,1  		;Section Y Radian (if DEBUG SECTION != 0)
extern int START_RADY;
// asm: 	.bss	DRIVE_LIST,1		;OBJECTs which can be driven over (road/shoulder)
extern int DRIVE_LIST;
// asm: 	.bss	CAR_LIST,1		;OBJECTs which are vehicles (drones/player car)
extern int CAR_LIST;
// asm: 	.bss	SIGN_LIST,1		;OBJECTs which are signs (posts, road signs, trees etc)
extern int SIGN_LIST;
// asm: 	.bss	GROUND_LIST,1		;OBJECTs which define the the ground but are not ROAD_C or SHLDR_C
extern int GROUND_LIST;
// asm: 	.bss	NEW_GROUP,1	;TEMP PTR TO START OF NEW DGROUP LIST
extern int NEW_GROUP;
// asm: 	.bss	DGROUPS,DGRP_SIZE*MAX_DGROUPS
extern int DGROUPS[];
// asm: 	.bss	DGROUP_COUNT,1		;NUMBER OF ACTIVE BACKGRNDS (on DYNA LIST)
extern int DGROUP_COUNT;
// asm: 	.bss	DGROUP_AW,1		;activate watch pointer (->TYCO.ASM)
extern int DGROUP_AW;
// asm: 	.bss	DYNALIST_TRUEBEGIN,1	;FIRST ELEMENT OF DYNAFLEX LIST
extern int DYNALIST_TRUEBEGIN;
// asm: 	.bss	DYNALIST_BEGIN,1	;FIRST ELEMENT OF DYNAFLEX LIST (SECOND SET)
extern int DYNALIST_BEGIN;
// asm: 	.bss	DYNALIST_END,1		;LAST ELEMENT OF DYNAFLEX LIST
extern int DYNALIST_END;
// asm: 	.bss	SUBLIST_BEGIN,1		;FIRST ELEMENT OF SUBLIST (OLINK 3)
extern int SUBLIST_BEGIN;
// asm: 	.bss	SUBLIST_END,1		;LAST ELEMENT OF SUBLIST (OLINK 4)
extern int SUBLIST_END;
// asm: 	.bss	STARTS,1
extern int STARTS;
// asm: 	.bss	SECTIONIDX,1		;section index (of next to activate)
extern int SECTIONIDX;
// asm: 	.bss	TYCO_TRACK,1
extern int TYCO_TRACK;
// asm: 	.bss	TYCO_TRACK_NTL,1
extern int TYCO_TRACK_NTL;
// asm: 	.bss	TYCO_NTL_IDX,1
extern int TYCO_NTL_IDX;
// asm: 	.bss	NEWSUBLIST_TOPB,1	;POINTER TO TOP OF SUBLIST (OF GROUP)
extern int NEWSUBLIST_TOPB;
// asm: 	.bss	TYCOFLAG,1	;CURRENT SUBLIST TYCOFLAG (FROM ROM)
extern int TYCOFLAG;
// asm: 	.bss	PASS1,1		;CURRENT PASS INDEX (0,1) (USED WHEN ACTIVATING AN OVERLAY)
extern int PASS1;
// asm: 	.bss	SECRADY,1	;SECTION RADIAN Y
extern int SECRADY;
// asm: 	.bss	SINGLE_SECTION_TEMPPTR,1
extern int SINGLE_SECTION_TEMPPTR;

#endif /* BACKGRND_H */

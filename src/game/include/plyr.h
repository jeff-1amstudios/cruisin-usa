#ifndef PLYR_H
#define PLYR_H

/* Generated from asm/PLYR.ASM. */

// asm: 	.bss	OFFROAD_TMR,1
extern int OFFROAD_TMR;
// asm: 	.bss	ZOOMD,1		;CURRENT ZOOM DISTANCE
extern int ZOOMD;
// asm: 	.bss	ZOOMDD,1
extern int ZOOMDD;
// asm: 	.bss	ZOOMDG,1	;ZOOM DIST GOAL
extern int ZOOMDG;
// asm: 	.bss	ZOOMH,1		;CURRENT ZOOM HEIGHT
extern int ZOOMH;
// asm: 	.bss	ZOOMHD,1
extern int ZOOMHD;
// asm: 	.bss	ZOOMHG,1	;ZOOM HEIGHT GOAL
extern int ZOOMHG;
// asm: 	.bss	CAMVIEW,1	;CAMERA VIEW 1=THIRD PERSON
extern int CAMVIEW;
// asm: 	.bss	BRAKEON,1	;1=BRAKE PEDAL ON
extern int BRAKEON;
// asm: 	.bss	WRECKFLG,1	;1=PLAYER CAR WRECKED, 0=NORMAL
extern int WRECKFLG;
// asm: 	.bss	REVFLG,1
extern int REVFLG;
// asm: 	.bss	PLYCAR,1
extern int PLYCAR;
// asm: 	.bss	PLYPROC,1
extern int PLYPROC;
// asm: 	.bss	PLYCBLK,1	;End Plyr Struct
extern int PLYCBLK;
// asm: 	.bss	OLDPLYSPD,1	;OLD PLAYER SPEED
extern int OLDPLYSPD;
// asm: 	.bss	OLDPLYAIR,1	;OLD PLAYER AIRBORNE
extern int OLDPLYAIR;
// asm: 	.bss	PLYRFIRST,1	;TICKS PLAYER IN 1ST PLACE
extern int PLYRFIRST;
// asm: 	.bss	PLAIRTIM,1	;TIMER FOR AIR SOUND DISABLE
extern int PLAIRTIM;
// asm: 	.bss	CHEATACC,1
extern int CHEATACC;
// asm: 	.bss	CHEAT,1
extern int CHEAT;
// asm: 	.BSS	ENGVOL,1
extern int ENGVOL;
// asm: 	.bss	WHLTIM,1
extern int WHLTIM;
// asm: 	.bss	WHLOLD,1
extern int WHLOLD;
// asm: 	.bss	PEDALMN,1		;GAS PEDAL MIN	 CMOS 0
extern int PEDALMN;
// asm: 	.bss	PEDALMX,1		;GAS PEDAL MAX	      1
extern int PEDALMX;
// asm: 	.bss	STEERMN,1		;STEERING MIN	      2
extern int STEERMN;
// asm: 	.bss	STEERMX,1		;STEERING MAX	      3
extern int STEERMX;
// asm: 	.bss	STEERCT,1		;STEERING CENTER      4
extern int STEERCT;
// asm: 	.bss	BRAKEMN,1		;BRAKE PEDAL MIN      5
extern int BRAKEMN;
// asm: 	.bss	BRAKEMX,1		;BRAKE PEDAL MAX      6
extern int BRAKEMX;
// asm: 	.bss	STEERFR,1		;STEERING FREE
extern int STEERFR;

#endif /* PLYR_H */

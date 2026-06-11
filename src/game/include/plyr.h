#ifndef PLYR_H
#define PLYR_H

/* Generated from asm/PLYR.ASM. */

// asm: 	.bss	OFFROAD_TMR,1
// addr: 0x0000E89A
extern int OFFROAD_TMR;
// asm: 	.bss	ZOOMD,1		;CURRENT ZOOM DISTANCE
// addr: 0x0000E89B
extern int ZOOMD;
// asm: 	.bss	ZOOMDD,1
// addr: 0x0000E89C
extern int ZOOMDD;
// asm: 	.bss	ZOOMDG,1	;ZOOM DIST GOAL
// addr: 0x0000E89D
extern int ZOOMDG;
// asm: 	.bss	ZOOMH,1		;CURRENT ZOOM HEIGHT
// addr: 0x0000E89E
extern int ZOOMH;
// asm: 	.bss	ZOOMHD,1
// addr: 0x0000E89F
extern int ZOOMHD;
// asm: 	.bss	ZOOMHG,1	;ZOOM HEIGHT GOAL
// addr: 0x0000E8A0
extern int ZOOMHG;
// asm: 	.bss	CAMVIEW,1	;CAMERA VIEW 1=THIRD PERSON
// addr: 0x0000E8A1
extern int CAMVIEW;
// asm: 	.bss	BRAKEON,1	;1=BRAKE PEDAL ON
// addr: 0x0000E8A2
extern int BRAKEON;
// asm: 	.bss	WRECKFLG,1	;1=PLAYER CAR WRECKED, 0=NORMAL
// addr: 0x0000E8A3
extern int WRECKFLG;
// asm: 	.bss	REVFLG,1
// addr: 0x0000E8A4
extern int REVFLG;
// asm: 	.bss	PLYCAR,1
// addr: 0x0000E8A6
extern int PLYCAR;
// asm: 	.bss	PLYPROC,1
// addr: 0x0000E8A7
extern int PLYPROC;
// asm: 	.bss	PLYCBLK,1	;End Plyr Struct
// addr: 0x0000E8A8
extern int PLYCBLK;
// asm: 	.bss	OLDPLYSPD,1	;OLD PLAYER SPEED
// addr: 0x0000E8A9
extern int OLDPLYSPD;
// asm: 	.bss	OLDPLYAIR,1	;OLD PLAYER AIRBORNE
// addr: 0x0000E8AA
extern int OLDPLYAIR;
// asm: 	.bss	PLYRFIRST,1	;TICKS PLAYER IN 1ST PLACE
// addr: 0x0000E8AB
extern int PLYRFIRST;
// asm: 	.bss	PLAIRTIM,1	;TIMER FOR AIR SOUND DISABLE
// addr: 0x0000E8AC
extern int PLAIRTIM;
// asm: 	.bss	CHEATACC,1
// addr: 0x0000E8AD
extern int CHEATACC;
// asm: 	.bss	CHEAT,1
// addr: 0x0000E8AE
extern int CHEAT;
// asm: 	.BSS	ENGVOL,1
// addr: 0x0000E8C7
extern int ENGVOL;
// asm: 	.bss	WHLTIM,1
// addr: 0x0000E8C8
extern int WHLTIM;
// asm: 	.bss	WHLOLD,1
// addr: 0x0000E8C9
extern int WHLOLD;
// asm: 	.bss	PEDALMN,1		;GAS PEDAL MIN	 CMOS 0
// addr: 0x0000E8CA
extern int PEDALMN;
// asm: 	.bss	PEDALMX,1		;GAS PEDAL MAX	      1
// addr: 0x0000E8CB
extern int PEDALMX;
// asm: 	.bss	STEERMN,1		;STEERING MIN	      2
// addr: 0x0000E8CC
extern int STEERMN;
// asm: 	.bss	STEERMX,1		;STEERING MAX	      3
// addr: 0x0000E8CD
extern int STEERMX;
// asm: 	.bss	STEERCT,1		;STEERING CENTER      4
// addr: 0x0000E8CE
extern int STEERCT;
// asm: 	.bss	BRAKEMN,1		;BRAKE PEDAL MIN      5
// addr: 0x0000E8CF
extern int BRAKEMN;
// asm: 	.bss	BRAKEMX,1		;BRAKE PEDAL MAX      6
// addr: 0x0000E8D0
extern int BRAKEMX;
// asm: 	.bss	STEERFR,1		;STEERING FREE
// addr: 0x0000E8D1
extern int STEERFR;

#endif /* PLYR_H */

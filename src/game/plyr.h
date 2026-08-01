#ifndef PLYR_H
#define PLYR_H

#include "../core/port.h"

/* Generated from asm/PLYR.ASM. */

// PLYR.ASM
// asm: 	.bss	OFFROAD_TMR,1
extern int OFFROAD_TMR;
// asm: 	.bss	ZOOMD,1		;CURRENT ZOOM DISTANCE
extern c3x_reg_t ZOOMD;
// asm: 	.bss	ZOOMDD,1
extern c3x_reg_t ZOOMDD;
// asm: 	.bss	ZOOMDG,1	;ZOOM DIST GOAL
extern c3x_reg_t ZOOMDG;
// asm: 	.bss	ZOOMH,1		;CURRENT ZOOM HEIGHT
extern int ZOOMH;
// asm: 	.bss	ZOOMHD,1
extern c3x_reg_t ZOOMHD;
// asm: 	.bss	ZOOMHG,1	;ZOOM HEIGHT GOAL
extern c3x_reg_t ZOOMHG;
// asm: 	.bss	CAMVIEW,1	;CAMERA VIEW 1=THIRD PERSON
extern int CAMVIEW;
// asm: 	.bss	BRAKEON,1	;1=BRAKE PEDAL ON
extern int BRAKEON;
// asm: 	.bss	WRECKFLG,1	;1=PLAYER CAR WRECKED, 0=NORMAL
extern int WRECKFLG;
// asm: 	.bss	REVFLG,1
extern int REVFLG;
// asm: 	.bss	PLYCAR,1
extern OBJ* PLYCAR;
// asm: 	.bss	PLYPROC,1
extern PROC* PLYPROC;
// asm: 	.bss	PLYCBLK,1	;End Plyr Struct
extern CARBLK* PLYCBLK;
// asm: 	.bss	OLDPLYSPD,1	;OLD PLAYER SPEED
extern c3x_reg_t OLDPLYSPD;
// asm: 	.bss	OLDPLYAIR,1	;OLD PLAYER AIRBORNE
extern int OLDPLYAIR;
// asm: 	.bss	PLYRFIRST,1	;TICKS PLAYER IN 1ST PLACE
extern int PLYRFIRST;
// asm: 	.bss	PLAIRTIM,1	;TIMER FOR AIR SOUND DISABLE
extern int PLAIRTIM;
// asm: 	.bss	CHEATACC,1
extern c3x_reg_t CHEATACC;
// asm: 	.bss	CHEAT,1
extern c3x_reg_t CHEAT;
// asm: 	.BSS	ENGVOL,1
extern int ENGVOL;
// asm: 	.bss	WHLTIM,1
extern int WHLTIM;
// asm: 	.bss	WHLOLD,1
extern int WHLOLD;
// asm: 	.bss	PEDALMN,1		;GAS PEDAL MIN	 CMOS 0
extern c3x_reg_t PEDALMN;
// asm: 	.bss	PEDALMX,1		;GAS PEDAL MAX	      1
extern c3x_reg_t PEDALMX;
// asm: 	.bss	STEERMN,1		;STEERING MIN	      2
extern c3x_reg_t STEERMN;
// asm: 	.bss	STEERMX,1		;STEERING MAX	      3
extern c3x_reg_t STEERMX;
// asm: 	.bss	STEERCT,1		;STEERING CENTER      4
extern c3x_reg_t STEERCT;
// asm: 	.bss	BRAKEMN,1		;BRAKE PEDAL MIN      5
extern c3x_reg_t BRAKEMN;
// asm: 	.bss	BRAKEMX,1		;BRAKE PEDAL MAX      6
extern c3x_reg_t BRAKEMX;
// asm: 	.bss	STEERFR,1		;STEERING FREE
extern c3x_reg_t STEERFR;
void RANDSND(const int* sounds /*AR2*/, int range /*R0*/);
void RANDVSND(const int* sounds /*AR2*/, int range /*R0*/, int volume /*R1*/);
c3x_reg_t ROADIR(OBJ* track_obj /*AR0*/);
void GETNXTRDIR(void);
void DRONINBZ(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/);
void INBOUNDZ(void);
void GETRPM(CARBLK* carblk /*AR5*/);
int GETAUTO(CARBLK* carblk /*AR5*/);
void GETTRAK(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/);
void DRONESTOP(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/);
void BONUS_WAIT_LOOP(void);
void PLYR_CAR_INIT(void);
void DRONESND(OBJ* obj /*AR4*/, const int* sounds /*AR2*/, int range /*R0*/);
void DRONESND1(OBJ* obj /*AR4*/, int sound_index /*AR2*/);

#endif /* PLYR_H */

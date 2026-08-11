#ifndef INTRO_H
#define INTRO_H

#include "../core/port.h"

/* Generated from asm/INTRO.ASM. */

// INTRO.ASM
// asm: 	.bss	START_HIT,1
extern int START_HIT;
// asm: 	.bss	CHOSEN_VEHICLE,1	;WHEEL POSITION W/HYSTERISUS
extern int CHOSEN_VEHICLE;
// asm: 	.bss	CHOSEN_TRANSMISSION,1
extern int CHOSEN_TRANSMISSION;
// asm: 	.bss	CHOSEN_RACE,1
extern int CHOSEN_RACE;
// asm: 	.bss	END_OF_GAMEP,1
extern int END_OF_GAMEP;
// asm: 	.bss	RACE_MODE,1		;USA (A-Z) || SINGLE
extern int RACE_MODE;
// asm: 	.bss	FIRST_RACE,1		;T or F
extern int FIRST_RACE;
// asm: 	.bss	POSES,1			;for the WHEEL
extern int POSES;
// asm: 	.bss	GAMEDIFF,1
extern c3x_f32_t GAMEDIFF;
// asm: 	.bss	CHECKPOINT_NUM,1
extern int CHECKPOINT_NUM;
// asm: 	.bss	H2H_FLAGSTATE,1		;WAVEFLAG
extern int H2H_FLAGSTATE;
// asm: 	.bss	FRAMELAG,1
extern int FRAMELAG;
// asm: 	pbss	BONUS_WAITFLAG,1
extern int BONUS_WAITFLAG;
// asm: 	pbss	OM_BONUS_WAITFLAG,1
extern int OM_BONUS_WAITFLAG;
// asm: 	.bss	NOASK_LINK,1
extern int NOASK_LINK;
// asm: 	.bss	DCALL,1		;IS CHOOSE TRANSMISSION ACTIVE?
extern int DCALL;
// asm: 	.bss	START_NOW_P,1
extern int START_NOW_P;
// asm: 	.bss	CHOOSENCAR,1
extern int CHOOSENCAR;
// asm: 	.bss	CAR_CHOICE_GOTTEN,1
extern int CAR_CHOICE_GOTTEN;
// asm: 	.bss	CAR_ARRAY,4
extern int CAR_ARRAY[];
// asm: 	.bss	IS_HIDDEN,1
extern int IS_HIDDEN;
// asm: 	.bss	SPINCURR,1
extern int SPINCURR;
// asm: 	.bss	BABE_CONTROL,1
extern int BABE_CONTROL;
// asm: 	.bss	CURR_FLAGSTATE,1
extern int CURR_FLAGSTATE;
// asm: 	.bss	LASTCHOICE,1
extern int LASTCHOICE;
// asm: 	.bss	_timer,1
extern int _timer;
// asm: 	.bss	SAVEDMODE,1
extern int SAVEDMODE;
// asm: 	.bss	miniidle,1
extern int miniidle;
// asm: 	.bss	DIRTY_SHARED,1
extern int DIRTY_SHARED;

#endif /* INTRO_H */

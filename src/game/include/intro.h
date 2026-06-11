#ifndef INTRO_H
#define INTRO_H

/* Generated from asm/INTRO.ASM. */

// asm: 	.bss	START_HIT,1
// addr: 0x0000E661
extern int START_HIT;
// asm: 	.bss	CHOSEN_VEHICLE,1	;WHEEL POSITION W/HYSTERISUS
// addr: 0x0000E662
extern int CHOSEN_VEHICLE;
// asm: 	.bss	CHOSEN_TRANSMISSION,1
// addr: 0x0000E663
extern int CHOSEN_TRANSMISSION;
// asm: 	.bss	CHOSEN_RACE,1
// addr: 0x0000E664
extern int CHOSEN_RACE;
// asm: 	.bss	END_OF_GAMEP,1
// addr: 0x0000E665
extern int END_OF_GAMEP;
// asm: 	.bss	RACE_MODE,1		;USA (A-Z) || SINGLE
// addr: 0x0000E666
extern int RACE_MODE;
// asm: 	.bss	FIRST_RACE,1		;T or F
// addr: 0x0000E667
extern int FIRST_RACE;
// asm: 	.bss	POSES,1			;for the WHEEL
// addr: 0x0000E668
extern int POSES;
// asm: 	.bss	GAMEDIFF,1
// addr: 0x0000E669
extern int GAMEDIFF;
// asm: 	.bss	CHECKPOINT_NUM,1
// addr: 0x0000E66A
extern int CHECKPOINT_NUM;
// asm: 	.bss	H2H_FLAGSTATE,1		;WAVEFLAG
// addr: 0x0000E66B
extern int H2H_FLAGSTATE;
// asm: 	.bss	FRAMELAG,1
// addr: 0x0000E66C
extern int FRAMELAG;
// asm: 	pbss	BONUS_WAITFLAG,1
// addr: 0x0000C914
extern int BONUS_WAITFLAG;
// asm: 	pbss	OM_BONUS_WAITFLAG,1
// addr: 0x0000C915
extern int OM_BONUS_WAITFLAG;
// asm: 	.bss	NOASK_LINK,1
// addr: 0x0000E66D
extern int NOASK_LINK;
// asm: 	.bss	DCALL,1		;IS CHOOSE TRANSMISSION ACTIVE?
// addr: 0x0000E66E
extern int DCALL;
// asm: 	.bss	START_NOW_P,1
// addr: 0x0000E66F
extern int START_NOW_P;
// asm: 	.bss	CHOOSENCAR,1
// addr: 0x0000E670
extern int CHOOSENCAR;
// asm: 	.bss	CAR_CHOICE_GOTTEN,1
// addr: 0x0000E671
extern int CAR_CHOICE_GOTTEN;
// asm: 	.bss	CAR_ARRAY,4
// addr: 0x0000E672
extern int CAR_ARRAY[4];
// asm: 	.bss	CAR1PAL,129
// addr: 0x0000E676
extern int CAR1PAL[129];
// asm: 	.bss	CAR2PAL,129
// addr: 0x0000E6F7
extern int CAR2PAL[129];
// asm: 	.bss	CAR3PAL,129
// addr: 0x0000E778
extern int CAR3PAL[129];
// asm: 	.bss	CAR4PAL,129
// addr: 0x0000E7F9
extern int CAR4PAL[129];
// asm: 	.bss	IS_HIDDEN,1
// addr: 0x0000E87A
extern int IS_HIDDEN;
// asm: 	.bss	SPINCURR,1
// addr: 0x0000E87B
extern int SPINCURR;
// asm: 	.bss	BABE_CONTROL,1
// addr: 0x0000E87C
extern int BABE_CONTROL;
// asm: 	.bss	CURR_FLAGSTATE,1
// addr: 0x0000E87D
extern int CURR_FLAGSTATE;
// asm: 	.bss	LASTCHOICE,1
// addr: 0x0080E87E
extern int LASTCHOICE;
// asm: 	.bss	_timer,1
// addr: 0x0000E87F
extern int _timer;
// asm: 	.bss	SAVEDMODE,1
// addr: 0x0000E880
extern int SAVEDMODE;
// asm: 	.bss	miniidle,1
// addr: 0x0000E881
extern int miniidle;
// asm: 	.bss	DIRTY_SHARED,1
// addr: 0x0000E882
extern int DIRTY_SHARED;

#endif /* INTRO_H */

#ifndef INTRO_H
#define INTRO_H

/* Generated from asm/INTRO.ASM. */

// asm: 	.bss	START_HIT,1
#define START_HIT 0x0000E661
// asm: 	.bss	CHOSEN_VEHICLE,1	;WHEEL POSITION W/HYSTERISUS
#define CHOSEN_VEHICLE 0x0000E662
// asm: 	.bss	CHOSEN_TRANSMISSION,1
#define CHOSEN_TRANSMISSION 0x0000E663
// asm: 	.bss	CHOSEN_RACE,1
#define CHOSEN_RACE 0x0000E664
// asm: 	.bss	END_OF_GAMEP,1
#define END_OF_GAMEP 0x0000E665
// asm: 	.bss	RACE_MODE,1		;USA (A-Z) || SINGLE
#define RACE_MODE 0x0000E666
// asm: 	.bss	FIRST_RACE,1		;T or F
#define FIRST_RACE 0x0000E667
// asm: 	.bss	POSES,1			;for the WHEEL
#define POSES 0x0000E668
// asm: 	.bss	GAMEDIFF,1
#define GAMEDIFF 0x0000E669
// asm: 	.bss	CHECKPOINT_NUM,1
#define CHECKPOINT_NUM 0x0000E66A
// asm: 	.bss	H2H_FLAGSTATE,1		;WAVEFLAG
#define H2H_FLAGSTATE 0x0000E66B
// asm: 	.bss	FRAMELAG,1
#define FRAMELAG 0x0000E66C
// asm: 	pbss	BONUS_WAITFLAG,1
#define BONUS_WAITFLAG 0x0000C914
// asm: 	pbss	OM_BONUS_WAITFLAG,1
#define OM_BONUS_WAITFLAG 0x0000C915
// asm: 	.bss	NOASK_LINK,1
#define NOASK_LINK 0x0000E66D
// asm: 	.bss	DCALL,1		;IS CHOOSE TRANSMISSION ACTIVE?
#define DCALL 0x0000E66E
// asm: 	.bss	START_NOW_P,1
#define START_NOW_P 0x0000E66F
// asm: 	.bss	CHOOSENCAR,1
#define CHOOSENCAR 0x0000E670
// asm: 	.bss	CAR_CHOICE_GOTTEN,1
#define CAR_CHOICE_GOTTEN 0x0000E671
// asm: 	.bss	CAR_ARRAY,4
#define CAR_ARRAY 0x0000E672
// asm: 	.bss	CAR1PAL,129
#define CAR1PAL 0x0000E676
// asm: 	.bss	CAR2PAL,129
#define CAR2PAL 0x0000E6F7
// asm: 	.bss	CAR3PAL,129
#define CAR3PAL 0x0000E778
// asm: 	.bss	CAR4PAL,129
#define CAR4PAL 0x0000E7F9
// asm: 	.bss	IS_HIDDEN,1
#define IS_HIDDEN 0x0000E87A
// asm: 	.bss	SPINCURR,1
#define SPINCURR 0x0000E87B
// asm: 	.bss	BABE_CONTROL,1
#define BABE_CONTROL 0x0000E87C
// asm: 	.bss	CURR_FLAGSTATE,1
#define CURR_FLAGSTATE 0x0000E87D
// asm: 	.bss	LASTCHOICE,1
#define LASTCHOICE 0x0080E87E
// asm: 	.bss	_timer,1
#define _timer 0x0000E87F
// asm: 	.bss	SAVEDMODE,1
#define SAVEDMODE 0x0000E880
// asm: 	.bss	miniidle,1
#define miniidle 0x0000E881
// asm: 	.bss	DIRTY_SHARED,1
#define DIRTY_SHARED 0x0000E882

#endif /* INTRO_H */

#ifndef BONUS_H
#define BONUS_H

/* Generated from asm/BONUS.ASM. */

// asm: 	.bss	MAXMPH,1		;FL
// addr: 0x0000E4AE
extern int MAXMPH;
// asm: 	.bss	CHALLENGE_RACE,1	;FL 1= TRUE,0= FALSE
// addr: 0x0080E4AF
extern int CHALLENGE_RACE;
// asm: 	.bss	NEXT_STARTUP,1		;IDX
// addr: 0x0000E4B0
extern int NEXT_STARTUP;
// asm: 	.bss	BONUS_WAVE,1		;IDX
// addr: 0x0098E4B1
extern int BONUS_WAVE;
// asm: 	.bss	FINISH_LINE,1		;UH	RoadCode for the finish line of this leg
// addr: 0x0098E4B2
extern int FINISH_LINE;
// asm: 	.bss	DO_FOLDFLAG,1		;UH	==1 when folding flag
// addr: 0x0000E4B3
extern int DO_FOLDFLAG;
// asm: 		.bss	GAMETRAK,NUM_LEGS*GT_SIZE
// addr: 0x0000E4B4
extern int GAMETRAK[NUM_LEGS*GT_SIZE];
// asm: 		.bss	ETIME,1
// addr: 0x0098E4EC
extern int ETIME;
// asm: 	.bss	UNFOLDFLAG,1
// addr: 0x0000E56B
extern int UNFOLDFLAG;
// asm: 	.bss	SAVED_COUNTDOWN,1
// addr: 0x0000E56C
extern int SAVED_COUNTDOWN;
// asm: 	.bss	WAS_HEAD2HEAD_ON,1
// addr: 0x0000E56E
extern int WAS_HEAD2HEAD_ON;
// asm: 	.bss	DID_TIMED_OUT,1
// addr: 0x0000E56F
extern int DID_TIMED_OUT;

#endif /* BONUS_H */

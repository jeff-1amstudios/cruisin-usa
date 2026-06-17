#ifndef BONUS_H
#define BONUS_H

/* Generated from asm/BONUS.ASM. */

// BONUS.ASM
// asm: 	.bss	MAXMPH,1		;FL
extern int MAXMPH;
// asm: 	.bss	CHALLENGE_RACE,1	;FL 1= TRUE,0= FALSE
extern int CHALLENGE_RACE;
// asm: 	.bss	NEXT_STARTUP,1		;IDX
extern int NEXT_STARTUP;
// asm: 	.bss	BONUS_WAVE,1		;IDX
extern int BONUS_WAVE;
// asm: 	.bss	FINISH_LINE,1		;UH	RoadCode for the finish line of this leg
extern int FINISH_LINE;
// asm: 	.bss	DO_FOLDFLAG,1		;UH	==1 when folding flag
extern int DO_FOLDFLAG;
// asm: 		.bss	GAMETRAK,NUM_LEGS*GT_SIZE
extern int GAMETRAK[];
// asm: 		.bss	ETIME,1
extern int ETIME;
// asm: 	.bss	UNFOLDFLAG,1
extern int UNFOLDFLAG;
// asm: 	.bss	SAVED_COUNTDOWN,1
extern int SAVED_COUNTDOWN;
// asm: 	.bss	WAS_HEAD2HEAD_ON,1
extern int WAS_HEAD2HEAD_ON;
// asm: 	.bss	DID_TIMED_OUT,1
extern int DID_TIMED_OUT;

#endif /* BONUS_H */

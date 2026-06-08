#ifndef BONUS_H
#define BONUS_H

/* Generated from asm/BONUS.ASM. */

// asm: 	.bss	MAXMPH,1		;FL
#define MAXMPH 0x0000E4AE
// asm: 	.bss	CHALLENGE_RACE,1	;FL 1= TRUE,0= FALSE
#define CHALLENGE_RACE 0x0080E4AF
// asm: 	.bss	NEXT_STARTUP,1		;IDX
#define NEXT_STARTUP 0x0000E4B0
// asm: 	.bss	BONUS_WAVE,1		;IDX
#define BONUS_WAVE 0x0098E4B1
// asm: 	.bss	FINISH_LINE,1		;UH	RoadCode for the finish line of this leg
#define FINISH_LINE 0x0098E4B2
// asm: 	.bss	DO_FOLDFLAG,1		;UH	==1 when folding flag
#define DO_FOLDFLAG 0x0000E4B3
// asm: 		.bss	GAMETRAK,NUM_LEGS*GT_SIZE
#define GAMETRAK 0x0000E4B4
// asm: 		.bss	ETIME,1
#define ETIME 0x0098E4EC
// asm: 	.bss	UNFOLDFLAG,1
#define UNFOLDFLAG 0x0000E56B
// asm: 	.bss	SAVED_COUNTDOWN,1
#define SAVED_COUNTDOWN 0x0000E56C
// asm: 	.bss	WAS_HEAD2HEAD_ON,1
#define WAS_HEAD2HEAD_ON 0x0000E56E
// asm: 	.bss	DID_TIMED_OUT,1
#define DID_TIMED_OUT 0x0000E56F

#endif /* BONUS_H */

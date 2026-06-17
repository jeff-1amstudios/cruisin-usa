#ifndef UTIL_H
#define UTIL_H

/* Generated from asm/UTIL.ASM. */

// asm: 	pbss	RAND,1		;RANDOM SEED
extern int RAND;
// asm: 	.bss	ACTIVE_SCREEN,1	;start of active screen (not visual screen)
extern int ACTIVE_SCREEN;
// asm: 	.bss	DYNALIST,NUM_DYNAS*DYNASIZE	;(~3420 WORDS)
extern int DYNALIST[NUM_DYNAS*DYNASIZE];
// asm: 	.bss	DYNAFREE,1
extern int DYNAFREE;
// asm: 	.bss	NULL,1
extern int NULL;
// asm: 	.bss	CARLIST,NUM_CARS*CARSIZ 	;(~1050 WORDS)
extern int CARLIST[NUM_CARS*CARSIZ];
// asm: 	.bss	CARFREE,1
extern int CARFREE;
// asm: 	.bss	CAR_COUNT,1
extern int CAR_COUNT;








#endif /* UTIL_H */

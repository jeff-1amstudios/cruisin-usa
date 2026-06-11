#ifndef UTIL_H
#define UTIL_H

/* Generated from asm/UTIL.ASM. */

// asm: 	pbss	RAND,1		;RANDOM SEED
// addr: 0x0000C8FE
extern int RAND;
// asm: 	.bss	ACTIVE_SCREEN,1	;start of active screen (not visual screen)
// addr: 0x0000CE43
extern int ACTIVE_SCREEN;
// asm: 	.bss	DYNALIST,NUM_DYNAS*DYNASIZE	;(~3420 WORDS)
// addr: 0x0000CE44
extern int DYNALIST[NUM_DYNAS*DYNASIZE];
// asm: 	.bss	DYNAFREE,1
// addr: 0x0000DB41
extern int DYNAFREE;
// asm: 	.bss	NULL,1
// addr: 0x0000DB42
extern int NULL;
// asm: 	.bss	CARLIST,NUM_CARS*CARSIZ 	;(~1050 WORDS)
// addr: 0x0000DB43
extern int CARLIST[NUM_CARS*CARSIZ];
// asm: 	.bss	CARFREE,1
// addr: 0x0000E426
extern int CARFREE;
// asm: 	.bss	CAR_COUNT,1
// addr: 0x0000E427
extern int CAR_COUNT;

#endif /* UTIL_H */

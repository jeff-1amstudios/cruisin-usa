#ifndef UTIL_H
#define UTIL_H

/* Generated from asm/UTIL.ASM. */

// asm: 	pbss	RAND,1		;RANDOM SEED
#define RAND 0x0000C8FE
// asm: 	.bss	ACTIVE_SCREEN,1	;start of active screen (not visual screen)
#define ACTIVE_SCREEN 0x0000CE43
// asm: 	.bss	DYNALIST,NUM_DYNAS*DYNASIZE	;(~3420 WORDS)
#define DYNALIST 0x0000CE44
// asm: 	.bss	DYNAFREE,1
#define DYNAFREE 0x0000DB41
// asm: 	.bss	NULL,1
#define NULL 0x0000DB42
// asm: 	.bss	CARLIST,NUM_CARS*CARSIZ 	;(~1050 WORDS)
#define CARLIST 0x0000DB43
// asm: 	.bss	CARFREE,1
#define CARFREE 0x0000E426
// asm: 	.bss	CAR_COUNT,1
#define CAR_COUNT 0x0000E427

#endif /* UTIL_H */

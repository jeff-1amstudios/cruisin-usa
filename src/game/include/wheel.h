#ifndef WHEEL_H
#define WHEEL_H

/* Generated from asm/WHEEL.ASM. */

// asm: 	.bss	POSE,1
// addr: 0x0000EA77
extern int POSE;
// asm: 	pbss	lpot0,1			;LAST POT
// addr: 0x0098C91A
extern int lpot0;
// asm: 	pbss	WHEELPWR,1	;FL
// addr: 0x0000C91B
extern int WHEELPWR;
// asm: 	.bss	WHEELPOS,1
// addr: 0x0000EA78
extern int WHEELPOS;
// asm: 	.bss	WHEELOUT,1
// addr: 0x0098EA79
extern int WHEELOUT;
// asm: 	.bss	WHEELMAX,1
// addr: 0x0098EA7A
extern int WHEELMAX;
// asm: 	pbss	WHEELVEL,1
// addr: 0x0000C91C
extern int WHEELVEL;
// asm: 	.bss	DAMPPWR,1
// addr: 0x0098EA7B
extern int DAMPPWR;

#endif /* WHEEL_H */

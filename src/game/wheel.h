#ifndef WHEEL_H
#define WHEEL_H

#include "../core/port.h"

/* Generated from asm/WHEEL.ASM. */

// WHEEL.ASM
// asm: 	.bss	POSE,1
extern int POSE;
// asm: 	pbss	lpot0,1			;LAST POT
extern int lpot0;
// asm: 	pbss	WHEELPWR,1	;FL
extern c3x_f32_t WHEELPWR;
// asm: 	.bss	WHEELPOS,1
extern c3x_f32_t WHEELPOS;
// asm: 	.bss	WHEELOUT,1
extern c3x_f32_t WHEELOUT;
// asm: 	.bss	WHEELMAX,1
extern c3x_f32_t WHEELMAX;
// asm: 	pbss	WHEELVEL,1
extern int WHEELVEL;
// asm: 	.bss	DAMPPWR,1
extern c3x_f32_t DAMPPWR;

#endif /* WHEEL_H */

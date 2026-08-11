#ifndef COIN_H
#define COIN_H

#include "../core/port.h"

/* Generated from asm/COIN.ASM. */

// COIN.ASM
// asm: 	.bss	COINOFF,1		;T -> DONT SHOW "INSERT COINS" MESSAGE
extern int COINOFF;

// asm: 	.bss	buffer,2
extern int buffer[];
// asm: 	.bss	FCB,1
extern int FCB;
// asm: 	.bss	PCB,1
extern int PCB;
// asm: 	.bss	ICF,1	;INSERT COIN FLASH
extern int ICF;
// asm: 	.bss	VOLUME_ACTIVE,1
extern int VOLUME_ACTIVE;
// asm: 	.bss	VOLUME_COUNT,4
extern int VOLUME_COUNT[];
// asm: 	.bss	CMOS_WP_WORD_SHADOW,1
extern int CMOS_WP_WORD_SHADOW;
// asm: 	.bss	COIN_COUNTER1,1
extern int COIN_COUNTER1;
// asm: 	.bss	COIN_COUNTER2,1
extern int COIN_COUNTER2;
// asm: 	.bss	COUNTER_IDX,1
extern int COUNTER_IDX;
// asm: 	.bss	COUNTER_MODE,1
extern int COUNTER_MODE;

// asm: 	.def	GET_CREDITS_TO_START
int GET_CREDITS_TO_START(void);

#endif /* COIN_H */

#ifndef COIN_H
#define COIN_H

/* Generated from asm/COIN.ASM. */

// asm: 	.bss	COINOFF,1		;T -> DONT SHOW "INSERT COINS" MESSAGE
extern int COINOFF;
// asm: 	.bss	CUSTOM_COINTAB,COIN_ENTRY_SIZE
extern int CUSTOM_COINTAB[COIN_ENTRY_SIZE];
// asm: 	.bss	CUSTOM_COINSTR,10
extern int CUSTOM_COINSTR[10];
// asm: 	.bss	buffer,2
extern int buffer[2];
// asm: 	.bss	FCB,1
extern int FCB;
// asm: 	.bss	PCB,1
extern int PCB;
// asm: 	.bss	CREDITBUFFER,8
extern int CREDITBUFFER[8];
// asm: 	.bss	TOSTARTBUFFER,8
extern int TOSTARTBUFFER[8];
// asm: 	.bss	ICF,1	;INSERT COIN FLASH
extern int ICF;
// asm: 	.bss	VOLUME_ACTIVE,1
extern int VOLUME_ACTIVE;
// asm: 	.bss	VOLUME_COUNT,4
extern int VOLUME_COUNT[4];
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

#endif /* COIN_H */

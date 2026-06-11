#ifndef COIN_H
#define COIN_H

/* Generated from asm/COIN.ASM. */

// asm: 	.bss	COINOFF,1		;T -> DONT SHOW "INSERT COINS" MESSAGE
// addr: 0x0000E571
extern int COINOFF;
// asm: 	.bss	CUSTOM_COINTAB,COIN_ENTRY_SIZE
// addr: 0x0000E572
extern int CUSTOM_COINTAB[COIN_ENTRY_SIZE];
// asm: 	.bss	CUSTOM_COINSTR,10
// addr: 0x0000E57D
extern int CUSTOM_COINSTR[10];
// asm: 	.bss	buffer,2
// addr: 0x0000E587
extern int buffer[2];
// asm: 	.bss	FCB,1
// addr: 0x0000E589
extern int FCB;
// asm: 	.bss	PCB,1
// addr: 0x0000E58A
extern int PCB;
// asm: 	.bss	CREDITBUFFER,8
// addr: 0x0000E58B
extern int CREDITBUFFER[8];
// asm: 	.bss	TOSTARTBUFFER,8
// addr: 0x0000E593
extern int TOSTARTBUFFER[8];
// asm: 	.bss	ICF,1	;INSERT COIN FLASH
// addr: 0x0000E59C
extern int ICF;
// asm: 	.bss	VOLUME_ACTIVE,1
// addr: 0x0000E59D
extern int VOLUME_ACTIVE;
// asm: 	.bss	VOLUME_COUNT,4
// addr: 0x0000E59E
extern int VOLUME_COUNT[4];
// asm: 	.bss	CMOS_WP_WORD_SHADOW,1
// addr: 0x0000E5A2
extern int CMOS_WP_WORD_SHADOW;
// asm: 	.bss	COIN_COUNTER1,1
// addr: 0x0000E5A3
extern int COIN_COUNTER1;
// asm: 	.bss	COIN_COUNTER2,1
// addr: 0x0000E5A4
extern int COIN_COUNTER2;
// asm: 	.bss	COUNTER_IDX,1
// addr: 0x0000E5A5
extern int COUNTER_IDX;
// asm: 	.bss	COUNTER_MODE,1
// addr: 0x0000E5A6
extern int COUNTER_MODE;

#endif /* COIN_H */

#ifndef COIN_H
#define COIN_H

/* Generated from asm/COIN.ASM. */

// asm: 	.bss	COINOFF,1		;T -> DONT SHOW "INSERT COINS" MESSAGE
#define COINOFF 0x0000E571
// asm: 	.bss	CUSTOM_COINTAB,COIN_ENTRY_SIZE
#define CUSTOM_COINTAB 0x0000E572
// asm: 	.bss	CUSTOM_COINSTR,10
#define CUSTOM_COINSTR 0x0000E57D
// asm: 	.bss	buffer,2
#define buffer 0x0000E587
// asm: 	.bss	FCB,1
#define FCB 0x0000E589
// asm: 	.bss	PCB,1
#define PCB 0x0000E58A
// asm: 	.bss	CREDITBUFFER,8
#define CREDITBUFFER 0x0000E58B
// asm: 	.bss	TOSTARTBUFFER,8
#define TOSTARTBUFFER 0x0000E593
// asm: 	.bss	ICF,1	;INSERT COIN FLASH
#define ICF 0x0000E59C
// asm: 	.bss	VOLUME_ACTIVE,1
#define VOLUME_ACTIVE 0x0000E59D
// asm: 	.bss	VOLUME_COUNT,4
#define VOLUME_COUNT 0x0000E59E
// asm: 	.bss	CMOS_WP_WORD_SHADOW,1
#define CMOS_WP_WORD_SHADOW 0x0000E5A2
// asm: 	.bss	COIN_COUNTER1,1
#define COIN_COUNTER1 0x0000E5A3
// asm: 	.bss	COIN_COUNTER2,1
#define COIN_COUNTER2 0x0000E5A4
// asm: 	.bss	COUNTER_IDX,1
#define COUNTER_IDX 0x0000E5A5
// asm: 	.bss	COUNTER_MODE,1
#define COUNTER_MODE 0x0000E5A6

#endif /* COIN_H */

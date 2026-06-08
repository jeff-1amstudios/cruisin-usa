#ifndef HUD_H
#define HUD_H

/* Generated from asm/HUD.ASM. */

// asm: 		.bss	COUNTDOWN_BUF,2
#define COUNTDOWN_BUF 0xFFFFFFFF
// asm: 		.bss	MPH_BUFFER,2
#define MPH_BUFFER 0x0000E632
// asm: 		.bss	_countdown,1	;SD
#define _countdown 0x0000E634
// asm: 		.bss	_MPH,1		;UD
#define _MPH 0x0000E635
// asm: 		.bss	SCORE,1		;UD
#define SCORE 0x0000E636
// asm: 		.bss	POSITION,1	;UD
#define POSITION 0x0000E637
// asm: 		.bss	MAXMPH_COUNT,1	;FL
#define MAXMPH_COUNT 0x0000E638
// asm: 		.bss	OFFROADBUFF,2
#define OFFROADBUFF 0x0000E63D
// asm: 	.bss	MOVEIN_OFFSET,1
#define MOVEIN_OFFSET 0x0000E63F
// asm: 	.bss	LASTSEC,1
#define LASTSEC 0x0000E640
// asm: 	.bss	TACHOMETER_PAL,32
#define TACHOMETER_PAL 0x0000E641

#endif /* HUD_H */

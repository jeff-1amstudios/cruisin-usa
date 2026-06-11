#ifndef HUD_H
#define HUD_H

/* Generated from asm/HUD.ASM. */

// asm: 		.bss	COUNTDOWN_BUF,2
// addr: 0xFFFFFFFF
extern int COUNTDOWN_BUF[2];
// asm: 		.bss	MPH_BUFFER,2
// addr: 0x0000E632
extern int MPH_BUFFER[2];
// asm: 		.bss	_countdown,1	;SD
// addr: 0x0000E634
extern int _countdown;
// asm: 		.bss	_MPH,1		;UD
// addr: 0x0000E635
extern int _MPH;
// asm: 		.bss	SCORE,1		;UD
// addr: 0x0000E636
extern int SCORE;
// asm: 		.bss	POSITION,1	;UD
// addr: 0x0000E637
extern int POSITION;
// asm: 		.bss	MAXMPH_COUNT,1	;FL
// addr: 0x0000E638
extern int MAXMPH_COUNT;
// asm: 		.bss	OFFROADBUFF,2
// addr: 0x0000E63D
extern int OFFROADBUFF[2];
// asm: 	.bss	MOVEIN_OFFSET,1
// addr: 0x0000E63F
extern int MOVEIN_OFFSET;
// asm: 	.bss	LASTSEC,1
// addr: 0x0000E640
extern int LASTSEC;
// asm: 	.bss	TACHOMETER_PAL,32
// addr: 0x0000E641
extern int TACHOMETER_PAL[32];

#endif /* HUD_H */

#ifndef HUD_H
#define HUD_H

/* Generated from asm/HUD.ASM. */

// asm: 		.bss	COUNTDOWN_BUF,2
extern int COUNTDOWN_BUF[2];
// asm: 		.bss	MPH_BUFFER,2
extern int MPH_BUFFER[2];
// asm: 		.bss	_countdown,1	;SD
extern int _countdown;
// asm: 		.bss	_MPH,1		;UD
extern int _MPH;
// asm: 		.bss	SCORE,1		;UD
extern int SCORE;
// asm: 		.bss	POSITION,1	;UD
extern int POSITION;
// asm: 		.bss	MAXMPH_COUNT,1	;FL
extern int MAXMPH_COUNT;
// asm: 		.bss	OFFROADBUFF,2
extern int OFFROADBUFF[2];
// asm: 	.bss	MOVEIN_OFFSET,1
extern int MOVEIN_OFFSET;
// asm: 	.bss	LASTSEC,1
extern int LASTSEC;
// asm: 	.bss	TACHOMETER_PAL,32
extern int TACHOMETER_PAL[32];

#endif /* HUD_H */

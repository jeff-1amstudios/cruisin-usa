#ifndef SND_H
#define SND_H

/* Generated from asm/SND.ASM. */

// asm: 	fbss	IN_RESET_MODE,1
#define IN_RESET_MODE 0x00809A79
// asm: 	fbss	RESET_TIMER,R0
#define RESET_TIMER 0x00809A7A
// asm: 	.bss	SNDSTR,NCHAN*SND_SIZ
#define SNDSTR 0x0000E920
// asm: 	.bss	STATION_TIMEOUT,1
#define STATION_TIMEOUT 0x0000E944
// asm: 	.bss	RS_X,1
#define RS_X 0x0000E945
// asm: 	.bss	TUNE_IDX,1	;ACTUALLY A STATION #
#define TUNE_IDX 0x0000E946
// asm: 	.bss	DO_NOT_REENABLE_INT,1
#define DO_NOT_REENABLE_INT 0x0000E947

#endif /* SND_H */

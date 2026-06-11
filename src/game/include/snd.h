#ifndef SND_H
#define SND_H

/* Generated from asm/SND.ASM. */

// asm: 	fbss	IN_RESET_MODE,1
// addr: 0x00809A79
extern int IN_RESET_MODE;
// asm: 	fbss	RESET_TIMER,R0
// addr: 0x00809A7A
extern int RESET_TIMER[R0];
// asm: 	.bss	SNDSTR,NCHAN*SND_SIZ
// addr: 0x0000E920
extern int SNDSTR[NCHAN*SND_SIZ];
// asm: 	.bss	STATION_TIMEOUT,1
// addr: 0x0000E944
extern int STATION_TIMEOUT;
// asm: 	.bss	RS_X,1
// addr: 0x0000E945
extern int RS_X;
// asm: 	.bss	TUNE_IDX,1	;ACTUALLY A STATION #
// addr: 0x0000E946
extern int TUNE_IDX;
// asm: 	.bss	DO_NOT_REENABLE_INT,1
// addr: 0x0000E947
extern int DO_NOT_REENABLE_INT;

#endif /* SND_H */

#ifndef SND_H
#define SND_H

/* Generated from asm/SND.ASM. */

// asm: 	fbss	IN_RESET_MODE,1
extern int IN_RESET_MODE;
// asm: 	fbss	RESET_TIMER,R0
extern int RESET_TIMER[R0];
// asm: 	.bss	SNDSTR,NCHAN*SND_SIZ
extern int SNDSTR[NCHAN*SND_SIZ];
// asm: 	.bss	STATION_TIMEOUT,1
extern int STATION_TIMEOUT;
// asm: 	.bss	RS_X,1
extern int RS_X;
// asm: 	.bss	TUNE_IDX,1	;ACTUALLY A STATION #
extern int TUNE_IDX;
// asm: 	.bss	DO_NOT_REENABLE_INT,1
extern int DO_NOT_REENABLE_INT;

#endif /* SND_H */

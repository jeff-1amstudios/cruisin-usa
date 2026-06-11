#ifndef WAVE_H
#define WAVE_H

/* Generated from asm/WAVE.ASM. */

// asm: 	pbss	TEASE_COUNT,1	;WHEN SPUTTER SOUND CAN BE CALLED IN ATTR
// addr: 0x0000C916
extern int TEASE_COUNT;
// asm: 	pbss	_ATTR_MODE,1	;CURRENT ATTRACT MODE
// addr: 0x0099C918
extern int _ATTR_MODE;
// asm: 	pbss	LOADED,1	;IS THE SYSTEM READY TO SLAVE OVER TO A NEW SCREENP
// addr: 0x0000C919
extern int LOADED;

#endif /* WAVE_H */

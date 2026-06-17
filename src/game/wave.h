#ifndef WAVE_H
#define WAVE_H

/* Generated from asm/WAVE.ASM. */

// WAVE.ASM
// asm: 	pbss	TEASE_COUNT,1	;WHEN SPUTTER SOUND CAN BE CALLED IN ATTR
extern int TEASE_COUNT;
// asm: 	pbss	_ATTR_MODE,1	;CURRENT ATTRACT MODE
extern int _ATTR_MODE;
// asm: 	pbss	LOADED,1	;IS THE SYSTEM READY TO SLAVE OVER TO A NEW SCREENP
extern int LOADED;

#endif /* WAVE_H */

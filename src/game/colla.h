#ifndef COLLA_H
#define COLLA_H

#include "../core/port.h"

/* Generated from asm/COLLA.ASM. */

// COLLA.ASM
// asm: 	.bss	VL,4
extern int VL[];
// asm: 	.bss	TNORM,3
extern int TNORM[];
// asm: 	.bss	TVECT1,3
extern int TVECT1[];
// asm: 	.bss	TVECT2,3
extern int TVECT2[];
// asm: 	.bss	TMATRIX,9
extern int TMATRIX[];
// asm: 	.bss	COLVEL,1	;RELATIVE COLLISION VELOCITY
extern int COLVEL;
// asm: 	.bss	PMULT,1  	;PLAYER SLOW COLLISION VELOCITY MULTIPLIER
extern int PMULT;
// asm: 	.bss	SPINTEMP,1
extern int SPINTEMP;
// asm: 	FBSS	BOXSCRAM,50
extern int BOXSCRAM[];
// asm: 	.BSS	PLYRBEHIND,1
extern int PLYRBEHIND;
void COLSCC(void);
void GETFLYMAT(void);
void SEND_FLY_KILL(void);
void DECODE_FLY_KILL(void);
void DECODE_FLY_XSFER(void);

#endif /* COLLA_H */

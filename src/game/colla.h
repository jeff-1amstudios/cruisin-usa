#ifndef COLLA_H
#define COLLA_H

#include "../core/port.h"

/* Generated from asm/COLLA.ASM. */

// COLLA.ASM
// asm: 	.bss	VL,4
extern struct VECTOR* VL[];
// asm: 	.bss	TNORM,3
extern c3x_reg_t TNORM[];
// asm: 	.bss	TVECT1,3
extern c3x_reg_t TVECT1[];
// asm: 	.bss	TVECT2,3
extern c3x_reg_t TVECT2[];
// asm: 	.bss	TMATRIX,9
extern c3x_reg_t TMATRIX[];
// asm: 	.bss	COLVEL,1	;RELATIVE COLLISION VELOCITY
extern c3x_f32_t COLVEL;
// asm: 	.bss	PMULT,1  	;PLAYER SLOW COLLISION VELOCITY MULTIPLIER
extern c3x_reg_t PMULT;
// asm: 	.bss	SPINTEMP,1
extern c3x_f32_t SPINTEMP;
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

#ifndef INFIN_H
#define INFIN_H

#include "../core/port.h"

/* Generated from asm/INFIN.ASM. */

// INFIN.ASM
// asm: 	.bss	CAMRADY,1  	;NORMALIZED 0-2PI CAMERA RAD
extern c3x_reg_t CAMRADY;
// asm: 	.bss	HIGHEST_ROADY,1		;FL	CLIP LEVEL IN Y
extern c3x_reg_t HIGHEST_ROADY;
// asm: 	.bss	HIGHEST_ROADY_X,1	;FL	CLIP LEVEL IN Y
extern c3x_reg_t HIGHEST_ROADY_X;
// asm: 	.bss	VAR_ROAD_KFACTOR,1	;FL	CLIPPING OVERSHOOT FACTOR
extern c3x_reg_t VAR_ROAD_KFACTOR;
// asm: 		.bss	AMOUNT_CLIPPED,1
extern c3x_reg_t AMOUNT_CLIPPED;
// asm: 	.bss	INFIN_CORRECT,1
extern c3x_reg_t INFIN_CORRECT;

#endif /* INFIN_H */

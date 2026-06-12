#ifndef INFIN_H
#define INFIN_H

/* Generated from asm/INFIN.ASM. */

// asm: 	.bss	CAMRADY,1  	;NORMALIZED 0-2PI CAMERA RAD
extern int CAMRADY;
// asm: 	.bss	HIGHEST_ROADY,1		;FL	CLIP LEVEL IN Y
extern int HIGHEST_ROADY;
// asm: 	.bss	HIGHEST_ROADY_X,1	;FL	CLIP LEVEL IN Y
extern int HIGHEST_ROADY_X;
// asm: 	.bss	VAR_ROAD_KFACTOR,1	;FL	CLIPPING OVERSHOOT FACTOR
extern int VAR_ROAD_KFACTOR;
// asm: 		.bss	AMOUNT_CLIPPED,1
extern int AMOUNT_CLIPPED;
// asm: 	.bss	INFIN_CORRECT,1
extern int INFIN_CORRECT;

#endif /* INFIN_H */

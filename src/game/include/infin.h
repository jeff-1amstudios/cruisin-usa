#ifndef INFIN_H
#define INFIN_H

/* Generated from asm/INFIN.ASM. */

// asm: 	.bss	CAMRADY,1  	;NORMALIZED 0-2PI CAMERA RAD
// addr: 0x0000E428
extern int CAMRADY;
// asm: 	.bss	HIGHEST_ROADY,1		;FL	CLIP LEVEL IN Y
// addr: 0x0000E429
extern int HIGHEST_ROADY;
// asm: 	.bss	HIGHEST_ROADY_X,1	;FL	CLIP LEVEL IN Y
// addr: 0x0000E42A
extern int HIGHEST_ROADY_X;
// asm: 	.bss	VAR_ROAD_KFACTOR,1	;FL	CLIPPING OVERSHOOT FACTOR
// addr: 0x0000E42B
extern int VAR_ROAD_KFACTOR;
// asm: 		.bss	AMOUNT_CLIPPED,1
// addr: 0x0000E42C
extern int AMOUNT_CLIPPED;
// asm: 	.bss	INFIN_CORRECT,1
// addr: 0x0000E42D
extern int INFIN_CORRECT;

#endif /* INFIN_H */

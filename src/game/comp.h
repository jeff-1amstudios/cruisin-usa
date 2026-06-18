#ifndef COMP_H
#define COMP_H

#include "port.h"

/* Generated from asm/COMP.ASM. */

// COMP.ASM
// asm: 	.bss	DECOMP_ACTIVE,1
extern int DECOMP_ACTIVE;
// asm: 	.bss	HARD_SECTION_LOAD,1
extern int HARD_SECTION_LOAD;
// asm: 	.bss	FLUSH_COUNT,1
extern int FLUSH_COUNT;
// asm: 	.bss	PACIFY_COUNT,1
extern int PACIFY_COUNT;
// asm: 		hibss	DICT,TABLE_SIZE*DICT_SIZ
extern tDICT DICT;
// asm: 		hibss	DECODE_STACK,TABLE_SIZE
extern int DECODE_STACK[];
// asm: 		.bss	NEXT_BUMP_CODE,1
extern int NEXT_BUMP_CODE;
// asm: 	lobss	LINEBUFFER,64
extern int LINEBUFFER[];
// asm: 	.bss	SAVESPC,25
extern int SAVESPC[];
// asm: 	.bss	PREVX,1
extern int PREVX;
// asm: 	.bss	DELTA,1
extern int DELTA;

#endif /* COMP_H */

#ifndef LEG_H
#define LEG_H

#include "types.h"

/* Generated from asm/LEG.ASM. */

// LEG.ASM
// asm: 	.bss	FINISH_ID,1
extern int FINISH_ID;
// asm: 	.bss	LEG_ELEMENTS,1
extern int LEG_ELEMENTS;
// asm: 	hibss	LEG_MAP,MAX_LEG_ELEMENTS*LEG_SIZE
extern LEG_PAYLOAD LEG_MAP[];
// asm: 	.bss	LAST_END_CACHE,1
extern tyco_stream_t LAST_END_CACHE;
// asm: 	.bss	LAST_END_INDEX,1
extern int LAST_END_INDEX;
// asm: 	hibss	LEG_SSLL,MINILL_SIZE*(LEG_SIZE+1)
extern LEG_SSLL_ENTRY LEG_SSLL[];
// asm: 	.bss	LEGFREE,1
extern LEG_SSLL_ENTRY* LEGFREE;
// asm: 	.bss	LEGLL,1
extern LEG_SSLL_ENTRY* LEGLL;
// asm: 	.bss	LEG_RADY,1	;FL
extern c3x_reg_t LEG_RADY;
// asm: 	.bss	LEG_SECTIONIDX,1	;UH
extern int LEG_SECTIONIDX;

#endif /* LEG_H */

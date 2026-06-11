#ifndef LEG_H
#define LEG_H

/* Generated from asm/LEG.ASM. */

// asm: 	.bss	FINISH_ID,1
// addr: 0x0000E883
extern int FINISH_ID;
// asm: 	.bss	LEG_ELEMENTS,1
// addr: 0x0000E884
extern int LEG_ELEMENTS;
// asm: 	hibss	LEG_MAP,MAX_LEG_ELEMENTS*LEG_SIZE
// addr: 0x0001CB6C
extern int LEG_MAP[MAX_LEG_ELEMENTS*LEG_SIZE];
// asm: 	.bss	LAST_END_CACHE,1
// addr: 0x0000E885
extern int LAST_END_CACHE;
// asm: 	.bss	LAST_END_INDEX,1
// addr: 0x0000E886
extern int LAST_END_INDEX;
// asm: 	hibss	LEG_SSLL,MINILL_SIZE*(LEG_SIZE+1)
// addr: 0x0001E46C
extern int LEG_SSLL[MINILL_SIZE*(LEG_SIZE+1)];
// asm: 	.bss	LEGFREE,1
// addr: 0x0000E888
extern int LEGFREE;
// asm: 	.bss	LEGLL,1
// addr: 0x0000E889
extern int LEGLL;
// asm: 	.bss	LEG_RADY,1	;FL
// addr: 0x0000E88B
extern int LEG_RADY;
// asm: 	.bss	LEG_SECTIONIDX,1	;UH
// addr: 0x0000E88C
extern int LEG_SECTIONIDX;

#endif /* LEG_H */

#ifndef COMP_H
#define COMP_H

/* Generated from asm/COMP.ASM. */

// asm: 	.bss	DECOMP_ACTIVE,1
extern int DECOMP_ACTIVE;
// asm: 	.bss	HARD_SECTION_LOAD,1
extern int HARD_SECTION_LOAD;
// asm: 	.bss	FLUSH_COUNT,1
extern int FLUSH_COUNT;
// asm: 	.bss	PACIFY_COUNT,1
extern int PACIFY_COUNT;
// asm: 		hibss	DICT,TABLE_SIZE*DICT_SIZ
extern int DICT[TABLE_SIZE*DICT_SIZ];
// asm: 		hibss	DECODE_STACK,TABLE_SIZE
extern int DECODE_STACK[TABLE_SIZE];
// asm: 		.bss	NEXT_BUMP_CODE,1
extern int NEXT_BUMP_CODE;
// asm: 	lobss	LINEBUFFER,64
extern int LINEBUFFER[64];
// asm: 	.bss	SAVESPC,25
extern int SAVESPC[25];
// asm: 	.bss	PREVX,1
extern int PREVX;
// asm: 	.bss	DELTA,1
extern int DELTA;








#endif /* COMP_H */

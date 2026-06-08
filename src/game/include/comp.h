#ifndef COMP_H
#define COMP_H

/* Generated from asm/COMP.ASM. */

// asm: 	.bss	DECOMP_ACTIVE,1
#define DECOMP_ACTIVE 0x0000E5FA
// asm: 	.bss	HARD_SECTION_LOAD,1
#define HARD_SECTION_LOAD 0x0000E5FB
// asm: 	.bss	FLUSH_COUNT,1
#define FLUSH_COUNT 0x0000E5FC
// asm: 	.bss	PACIFY_COUNT,1
#define PACIFY_COUNT 0x0000E5FD
// asm: 		hibss	DICT,TABLE_SIZE*DICT_SIZ
#define DICT 0x0001979D
// asm: 		hibss	DECODE_STACK,TABLE_SIZE
#define DECODE_STACK 0x0001BA27
// asm: 		.bss	NEXT_BUMP_CODE,1
#define NEXT_BUMP_CODE 0x0000E5FE
// asm: 	lobss	LINEBUFFER,64
#define LINEBUFFER 0x0000C91E
// asm: 	.bss	SAVESPC,25
#define SAVESPC 0x0000E5FF
// asm: 	.bss	PREVX,1
#define PREVX 0x0000E618
// asm: 	.bss	DELTA,1
#define DELTA 0x0000E619

#endif /* COMP_H */

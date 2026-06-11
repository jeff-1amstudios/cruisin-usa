#ifndef COMP_H
#define COMP_H

/* Generated from asm/COMP.ASM. */

// asm: 	.bss	DECOMP_ACTIVE,1
// addr: 0x0000E5FA
extern int DECOMP_ACTIVE;
// asm: 	.bss	HARD_SECTION_LOAD,1
// addr: 0x0000E5FB
extern int HARD_SECTION_LOAD;
// asm: 	.bss	FLUSH_COUNT,1
// addr: 0x0000E5FC
extern int FLUSH_COUNT;
// asm: 	.bss	PACIFY_COUNT,1
// addr: 0x0000E5FD
extern int PACIFY_COUNT;
// asm: 		hibss	DICT,TABLE_SIZE*DICT_SIZ
// addr: 0x0001979D
extern int DICT[TABLE_SIZE*DICT_SIZ];
// asm: 		hibss	DECODE_STACK,TABLE_SIZE
// addr: 0x0001BA27
extern int DECODE_STACK[TABLE_SIZE];
// asm: 		.bss	NEXT_BUMP_CODE,1
// addr: 0x0000E5FE
extern int NEXT_BUMP_CODE;
// asm: 	lobss	LINEBUFFER,64
// addr: 0x0000C91E
extern int LINEBUFFER[64];
// asm: 	.bss	SAVESPC,25
// addr: 0x0000E5FF
extern int SAVESPC[25];
// asm: 	.bss	PREVX,1
// addr: 0x0000E618
extern int PREVX;
// asm: 	.bss	DELTA,1
// addr: 0x0000E619
extern int DELTA;

#endif /* COMP_H */

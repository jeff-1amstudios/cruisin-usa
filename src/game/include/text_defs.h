#ifndef TEXT_H
#define TEXT_H

/* Generated from asm/TEXT.ASM. */

// asm: 	hibss	TEXT_LIST,NUM_TEXTS*TEXT_SIZ
extern int TEXT_LIST[NUM_TEXTS*TEXT_SIZ];
// asm: 	.bss	TEXT_FREE,1
extern int TEXT_FREE;
// asm: 	.bss	TEXT_ACTIVE,1
extern int TEXT_ACTIVE;
// asm: 	.bss	TEXT_FREE_COUNT,1
extern int TEXT_FREE_COUNT;
// asm: 	.bss	TEXT_FREEZE,1
extern int TEXT_FREEZE;

#endif /* TEXT_H */

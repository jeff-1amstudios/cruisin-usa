#ifndef HEADS_H
#define HEADS_H

#include "../core/port.h"

/* Generated from asm/HEADS.ASM. */

// HEADS.ASM
// asm: 	.bss	YINCREMENT,1
extern int YINCREMENT;
// asm: 	.bss	FONTUSED,1
extern int FONTUSED;
void VANITY(void);
void VANITY_SUB(void);

#endif /* HEADS_H */

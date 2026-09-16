#ifndef HEADS_H
#define HEADS_H

#include "../core/port.h"

/* Generated from asm/HEADS.ASM. */

// HEADS.ASM
// asm: 	.bss	YINCREMENT,1
extern c3x_f32_t YINCREMENT;
// asm: 	.bss	FONTUSED,1
typedef void (*HEADS_FONT_FUNC)(tSHADOW_TEXT* text);
extern HEADS_FONT_FUNC FONTUSED;
void VANITY(PROC* p);
void VANITY_SUB(PROC* p);

#endif /* HEADS_H */

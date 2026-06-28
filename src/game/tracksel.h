#ifndef TRACKSEL_H
#define TRACKSEL_H

#include "../core/port.h"

/* Generated from asm/TRACKSEL.ASM. */

// TRACKSEL.ASM
// asm: 	.bss	RAW_CHOOSENTRK,1	;BEFORE CONVERTING
extern int RAW_CHOOSENTRK;
// asm: 	.bss	HIDDEN_ON,1
extern int HIDDEN_ON;
// asm: 	.bss	TSEL_HEAD,1
extern int TSEL_HEAD;
// asm: 	.bss	TSEL_THEONE,1
extern int TSEL_THEONE;
// asm: 	.bss	BOILEROBJ,1
extern int BOILEROBJ;
void MOTION_SCREWED(void);
void BOILERPLATE_INIT(void);

#endif /* TRACKSEL_H */

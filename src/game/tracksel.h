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
extern OBJ* TSEL_HEAD;
// asm: 	.bss	TSEL_THEONE,1
extern OBJ* TSEL_THEONE;
// asm: 	.bss	BOILEROBJ,1
extern OBJ* BOILEROBJ;
void TRACK_SELECTION(PROC* p);
void MOTION_SCREWED(PROC* p);
void BOILERPLATE_INIT(void);

#endif /* TRACKSEL_H */

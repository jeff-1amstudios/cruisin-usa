#ifndef HOTTUB_H
#define HOTTUB_H

#include "../core/port.h"

/* Generated from asm/HOTTUB.ASM. */

// HOTTUB.ASM
// asm: 	.bss	PRES,1
extern int PRES;
// asm: 	.bss	WATCHOBJ,1
extern int WATCHOBJ;
void HT_HOTTUB(void);
void HT_RADAR(void);
void HT_STAGENT(void);

#endif /* HOTTUB_H */

#ifndef MEMTEST_H
#define MEMTEST_H

#include "../core/port.h"

/* Generated from asm/MEMTEST.ASM. */

// MEMTEST.ASM
// asm: 	fbss	BASICS_RAM,10
extern int BASICS_RAM[];
void TEST_STATIC_CHIPS(void);

#endif /* MEMTEST_H */

#ifndef MAP_H
#define MAP_H

#include "../core/port.h"

/* Generated from asm/MAP.ASM. */

// MAP.ASM
// asm: 	.bss	MAPPAL13,1	;FL multiplier
extern int MAPPAL13;
// asm: 	.bss	MAPPAL24,1	;FL multiplier
extern int MAPPAL24;
// asm: 	.bss	STOPWATCH,1		;the counter
extern int STOPWATCH;
// asm: 	.bss	STOPWATCH_CNTL,1	;the controller	0 = stop, 1 = go
extern int STOPWATCH_CNTL;
// asm: 		.bss	lap_buffer,4
extern int lap_buffer[];
// asm: 		.bss	tmp_buffer,2
extern int tmp_buffer[];
// asm: 	.bss	THIS_MACHINE_AHEAD,1
extern int THIS_MACHINE_AHEAD;
void CLEAR_MAP_PALS(void);

#endif /* MAP_H */

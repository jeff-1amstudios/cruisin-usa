#ifndef MAP_H
#define MAP_H

/* Generated from asm/MAP.ASM. */

// asm: 	.bss	MAPPAL13,1	;FL multiplier
// addr: 0x0000E88D
extern int MAPPAL13;
// asm: 	.bss	MAPPAL24,1	;FL multiplier
// addr: 0x0000E88E
extern int MAPPAL24;
// asm: 	.bss	STOPWATCH,1		;the counter
// addr: 0x0000E88F
extern int STOPWATCH;
// asm: 	.bss	STOPWATCH_CNTL,1	;the controller	0 = stop, 1 = go
// addr: 0x0000E890
extern int STOPWATCH_CNTL;
// asm: 		.bss	lap_buffer,4
// addr: 0x0000E891
extern int lap_buffer[4];
// asm: 		.bss	tmp_buffer,2
// addr: 0x0000E895
extern int tmp_buffer[2];
// asm: 	.bss	THIS_MACHINE_AHEAD,1
// addr: 0x0000E897
extern int THIS_MACHINE_AHEAD;

#endif /* MAP_H */

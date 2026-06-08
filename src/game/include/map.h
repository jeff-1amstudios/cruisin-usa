#ifndef MAP_H
#define MAP_H

/* Generated from asm/MAP.ASM. */

// asm: 	.bss	MAPPAL13,1	;FL multiplier
#define MAPPAL13 0x0000E88D
// asm: 	.bss	MAPPAL24,1	;FL multiplier
#define MAPPAL24 0x0000E88E
// asm: 	.bss	STOPWATCH,1		;the counter
#define STOPWATCH 0x0000E88F
// asm: 	.bss	STOPWATCH_CNTL,1	;the controller	0 = stop, 1 = go
#define STOPWATCH_CNTL 0x0000E890
// asm: 		.bss	lap_buffer,4
#define lap_buffer 0x0000E891
// asm: 		.bss	tmp_buffer,2
#define tmp_buffer 0x0000E895
// asm: 	.bss	THIS_MACHINE_AHEAD,1
#define THIS_MACHINE_AHEAD 0x0000E897

#endif /* MAP_H */

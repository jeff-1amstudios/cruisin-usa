#ifndef C30_H
#define C30_H

#include "port.h"

/* Generated from asm/C30.EQU. */

// C30.EQU
// 
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED
// 
// ----------------------------------------------------------------------------
// STATUS WORD FLAGS

// asm: INT0_M		.set	1		;DISPLAY INT
#define INT0_M 1 //DISPLAY INT

// asm: INT1_M		.set	2		;TV30 INT
#define INT1_M 2 //TV30 INT

// asm: INT2_M		.set	4		;COMM INT
#define INT2_M 4 //COMM INT

// asm: INT3_M		.set	8		;ATOD INT
#define INT3_M 8 //ATOD INT

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ON CHIP RAM BLOCKS
// asm: RAM0		.set	0809800h
#define RAM0 0x0809800

// asm: RAM1		.set	0809C00h
#define RAM1 0x0809C00

// ----------------------------------------------------------------------------
// WAIT STATE GENERATION
// asm: CPU_WS	  	.set	0808064h	;WAIT STATE CPU CONTROL REGISTER
#define CPU_WS 0x0808064 //WAIT STATE CPU CONTROL REGISTER

// asm: SOFT_WS		.set	00470h
#define SOFT_WS 0x00470

// asm: HARD_WS		.set	01000h		;WAVE RAM & SCREEN RAM
#define HARD_WS 0x01000 //WAVE RAM & SCREEN RAM

// asm: CMOS_WS		.set	00090h		;CMOS ACCESS
#define CMOS_WS 0x00090 //CMOS ACCESS

// ----------------------------------------------------------------------------
// TIMER DEFINES
// 
// asm: TIMER_CNTL1	.set	808020h
#define TIMER_CNTL1 0x808020

// asm: TIMER_CNTL2	.set	808030h
#define TIMER_CNTL2 0x808030

// asm: TIMER_CNTR1	.set	808024h
#define TIMER_CNTR1 0x808024

// asm: TIMER_CNTR2	.set	808034h
#define TIMER_CNTR2 0x808034

// asm: TIMER_PERIOD1	.set	808028h
#define TIMER_PERIOD1 0x808028

// asm: TIMER_PERIOD2	.set	808038h
#define TIMER_PERIOD2 0x808038

// asm: TMR_GO		.set	40h	;reset and start timer counter
#define TMR_GO 0x40 //reset and start timer counter

// asm: TMR_HOLD	.set	80h	;=0 -> suspend
#define TMR_HOLD 0x80 //=0 -> suspend

// asm: TMR_RESET_GO	.set	TMR_GO|TMR_HOLD
#define TMR_RESET_GO (TMR_GO|TMR_HOLD)

// asm: TMR_SUSPEND	.set	0
#define TMR_SUSPEND 0

// asm: 	.globl	TIMER_RESET,TIMER_READ
void TIMER_RESET(void);

void TIMER_READ(void);

#endif /* C30_H */

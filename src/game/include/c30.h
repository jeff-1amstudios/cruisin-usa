#ifndef C30_H
#define C30_H

/* Generated from asm/C30.EQU. */
/*
 * C30.EQU
 * COPYRIGHT (C) 1994  BY TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * ----------------------------------------------------------------------------
 * STATUS WORD FLAGS
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * ON CHIP RAM BLOCKS
 * ...
 */

/* Original .globl symbols in this module:
 *   TIMER_RESET
 *   TIMER_READ
 */

#define INT0_M 1 /* DISPLAY INT */
#define INT1_M 2 /* TV30 INT */
#define INT2_M 4 /* COMM INT */
#define INT3_M 8 /* ATOD INT */
#define RAM0 0x0809800
#define RAM1 0x0809C00
#define CPU_WS 0x0808064 /* WAIT STATE CPU CONTROL REGISTER */
#define SOFT_WS 0x00470
#define HARD_WS 0x01000 /* WAVE RAM & SCREEN RAM */
#define CMOS_WS 0x00090 /* CMOS ACCESS */
#define TIMER_CNTL1 0x808020
#define TIMER_CNTL2 0x808030
#define TIMER_CNTR1 0x808024
#define TIMER_CNTR2 0x808034
#define TIMER_PERIOD1 0x808028
#define TIMER_PERIOD2 0x808038
#define TMR_GO 0x40 /* reset and start timer counter */
#define TMR_HOLD 0x80 /* =0 -> suspend */
#define TMR_RESET_GO (TMR_GO|TMR_HOLD)
#define TMR_SUSPEND 0

#endif /* C30_H */

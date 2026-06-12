#ifndef DIAG_H
#define DIAG_H

/* Generated from asm/DIAG.EQU. */

// DIAG.EQU
// 
// COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// ALL RIGHTS RESERVED

// asm: COL_WHITE	.set	1
#define COL_WHITE 1

// asm: COL_GREY	.set	2
#define COL_GREY 2

// asm: COL_RED		.set	3
#define COL_RED 3

// asm: COL_GREEN	.set	4
#define COL_GREEN 4

// asm: COL_BLUE	.set	5	;paled for sky
#define COL_BLUE 5 //paled for sky

// asm: COL_YELLOW	.set	6
#define COL_YELLOW 6

// asm: COL_CYAN	.set	7
#define COL_CYAN 7

// asm: COL_BLACK	.set	8
#define COL_BLACK 8

// asm: COL_TBLUE	.set	9
#define COL_TBLUE 9

// asm: COL_AMBER	.set	10
#define COL_AMBER 10

// asm: COL_DGREY	.set	11
#define COL_DGREY 11

// asm: COL_LGREY	.set	12
#define COL_LGREY 12

// asm: COL_LGREY2	.set	13
#define COL_LGREY2 13

// asm: COL_LGREY3	.set	14
#define COL_LGREY3 14

// asm: COL_VDGREY	.set	15
#define COL_VDGREY 15

// COL_VDGREY	.set	2
// asm: DEFAULT_COLOR	.set	COL_VDGREY
#define DEFAULT_COLOR COL_VDGREY

// DEFAULT_COLOR	.set	COL_GREY

// WT_START	.MACRO
// wt67?
// ;	CALL	DIAG_ACTIVEP
// LDI	@_newbut,R0
// TSTB	SW_DIAG,R0
// BNZ	wt67?
// .ENDM
// 
// WT_NSTART	.MACRO
// wt67?
// ;	CALL	DIAG_ACTIVEP
// LDI	@_newbut,R0
// TSTB	SW_DIAG,R0
// BZ	wt67?
// .ENDM

// ----------------------------------------------------------------------------
// BEEP	.MACRO
// LDI	980,AR2
// CALL	SENDSND
// .ENDM

// KEYPAUSE	.MACRO
// WL1?	CALL	DIAG_ACTIVEP
// ;	LDP	@_newbut
// NOT	@_newbut,R0
// AND	SW_DIAG,R0
// BZ	WL1?
// WL2?
// ;	LDP	@_newbut
// ;	NOT	@_newbut,R0
// ;	AND	SW_DIAG,R0
// ;;	BNZ	WL2?
// .ENDM

// asm: 	.globl	MAKE_WINDOW
void MAKE_WINDOW(void);

// LDI	COL_LGREY,RS
// CALL	_fill
// 
// LDI	:X1:,AR2
// LDI	:Y1:,R2
// LDI	:X2:,R3
// LDI	:Y1:,RC
// LDI	COL_LGREY2,RS
// CALL	_line
// 
// LDI	:X1:,AR2
// LDI	:Y1:,R2
// LDI	:X1:,R3
// LDI	:Y2:,RC
// LDI	COL_LGREY2,RS
// CALL	_line
// 
// LDI	:X2:,AR2
// LDI	:Y1:,R2
// LDI	:X2:,R3
// LDI	:Y2:,RC
// LDI	COL_DGREY,RS
// CALL	_line
// 
// LDI	:X1:,AR2
// LDI	:Y2:,R2
// LDI	:X2:,R3
// LDI	:Y2:,RC
// LDI	COL_DGREY,RS
// CALL	_line

// asm: 	.globl	MAKE_IWINDOW
void MAKE_IWINDOW(void);

#endif /* DIAG_H */

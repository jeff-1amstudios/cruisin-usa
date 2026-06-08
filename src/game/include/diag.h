#ifndef DIAG_H
#define DIAG_H

/* Generated from asm/DIAG.EQU. */
/*
 * DIAG.EQU
 * COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * COL_VDGREY	.set	2
 * DEFAULT_COLOR	.set	COL_GREY
 * WT_START	.MACRO
 * wt67?
 * ;	CALL	DIAG_ACTIVEP
 * ...
 */

/* Original .globl symbols in this module:
 *   MAKE_WINDOW
 *   MAKE_WINDOW
 *   MAKE_IWINDOW
 */

#define COL_WHITE 1
#define COL_GREY 2
#define COL_RED 3
#define COL_GREEN 4
#define COL_BLUE 5 /* paled for sky */
#define COL_YELLOW 6
#define COL_CYAN 7
#define COL_BLACK 8
#define COL_TBLUE 9
#define COL_AMBER 10
#define COL_DGREY 11
#define COL_LGREY 12
#define COL_LGREY2 13
#define COL_LGREY3 14
#define COL_VDGREY 15
#define DEFAULT_COLOR COL_VDGREY

#endif /* DIAG_H */

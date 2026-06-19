#ifndef MACS_H
#define MACS_H

#include "port.h"

/* Generated from asm/MACS.EQU. */

// MACS.EQU
// 
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED

// asm: DP_MACROS	.set	0	;1 = ACTIVATE
#define DP_MACROS 0 //1 = ACTIVATE

/*
INCAUD	.MACRO	NUMBER
	LDI	:NUMBER:,AR2
	CALL	AUDIT_INC
	.ENDM
*/
#define INCAUD(NUMBER)        \
    do {                      \
        AR2 = (u32)(NUMBER);  \
        AUDIT_INC();          \
    } while (0)

/*
READADJ	.MACRO	NUMBER
	LDI	:NUMBER:,AR2
	CALL	ADJUSTMENT_READ
	.ENDM
*/
#define READADJ(NUMBER)           \
    do {                          \
        AR2 = (u32)(NUMBER);      \
        ADJUSTMENT_READ();        \
    } while (0)

/*
READAUD	.MACRO	NUMBER
	LDI	:NUMBER:,AR2
	CALL	AUDIT_READ
	.ENDM
*/
#define READAUD(NUMBER)       \
    do {                      \
        AR2 = (u32)(NUMBER);  \
        AUDIT_READ();         \
    } while (0)

/*
SETADJ	.MACRO	NUMBER
	LDI	:NUMBER:,AR2
	CALL	ADJUSTMENT_WRITE
	.ENDM
*/
#define SETADJ(NUMBER)            \
    do {                          \
        AR2 = (u32)(NUMBER);      \
        ADJUSTMENT_WRITE();       \
    } while (0)

/*
SETAUD	.MACRO	NUMBER
	LDI	:NUMBER:,AR2
	CALL	AUDIT_WRITE
	.ENDM
*/
#define SETAUD(NUMBER)        \
    do {                      \
        AR2 = (u32)(NUMBER);  \
        AUDIT_WRITE();        \
    } while (0)

#endif /* MACS_H */

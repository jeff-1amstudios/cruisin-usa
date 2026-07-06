#ifndef MACS_H
#define MACS_H

#include "../core/machine.h"
#include "../core/port.h"
#include <math.h>

/* Generated from asm/MACS.EQU. */

// MACS.EQU
//
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED

// asm: DP_MACROS	.set	0	;1 = ACTIVATE
#define DP_MACROS 0 // 1 = ACTIVATE

/*
INCAUD	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	AUDIT_INC
        .ENDM
*/
#define INCAUD AUDIT_INC

/*
READADJ	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	ADJUSTMENT_READ
        .ENDM
*/
#define READADJ ADJUSTMENT_READ

/*
READAUD	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	AUDIT_READ
        .ENDM
*/
#define READAUD AUDIT_READ

/*
SETADJ	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	ADJUSTMENT_WRITE
        .ENDM
*/
#define SETADJ ADJUSTMENT_WRITE

/*
SETAUD	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	AUDIT_WRITE
        .ENDM
*/
#define SETAUD AUDIT_WRITE

/*
RGB	.MACRO	R,G,B
        .word	(((:R:>>3)&01Fh)<<10)|(((:G:>>3)&01Fh)<<5)|(((:B:>>3)&01Fh))
        .ENDM
*/
#define RGB(R, G, B) \
    (((((u32)(R)) >> 3) & 0x1Fu) << 10) | (((((u32)(G)) >> 3) & 0x1Fu) << 5) | ((((u32)(B)) >> 3) & 0x1Fu)

/*
FONTENT		.MACRO	PRECEDING,XSTART,XEND,YSTART,TRAIL
    .WORD	(:PRECEDING:&0ffffh)|(:TRAIL:<<16),:XSTART:,:XEND:,:YSTART:
    .ENDM
*/
#define FONTENT(PRECEDING, XSTART, XEND, YSTART, TRAIL) \
    { ((((u32)(PRECEDING)) & 0xFFFFu) | (((u32)(TRAIL)) << 16)), (u32)(XSTART), (u32)(XEND), (u32)(YSTART) }

#define ERRON(x) abort()
#define SLOCKON(cond, msg)                      \
    if (cond) {                                 \
        crusn_slockon(__FILE__, __LINE__, msg); \
    }

#define DINT()
#define EINT()

#define TEXTIT _outtextxyc

#define CREATE PRC_CREATE

#define SOND1 ONESND

static inline float TMS320_C3X_SINGLE_TO_FLOAT(u32 raw) {
    int exponent;
    u32 fraction;
    float mantissa;

    exponent = (int8_t)(raw >> 24);
    if (exponent == -128) {
        return 0.0f;
    }

    fraction = raw & 0x007fffffu;
    if ((raw & 0x00800000u) == 0) {
        mantissa = 1.0f + ((float)fraction / 8388608.0f);
    } else {
        mantissa = -2.0f + ((float)fraction / 8388608.0f);
    }

    return ldexpf(mantissa, exponent);
}

#endif /* MACS_H */

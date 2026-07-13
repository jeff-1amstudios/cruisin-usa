#ifndef MACS_H
#define MACS_H

#include "../core/machine.h"
#include "../core/port.h"
#include <math.h>

/* Generated from asm/MACS.EQU. */

// MACS.EQU

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

static inline u32 TMS320_C3X_DOUBLE_TO_SINGLE_RAW_TRUNC(double value) {
    int exponent;
    double mantissa;
    double fraction;
    u32 mantissa_bits;
    uint64_t extended_fraction;

    if (value == 0.0) {
        return 0x80000000u;
    }

    mantissa = frexp(value, &exponent);
    mantissa = ldexp(mantissa, 1);
    exponent -= 1;

    if (exponent < -127) {
        return 0x80000000u;
    }
    if (exponent > 127) {
        return ((u32)(uint8_t)127 << 24) | (value < 0.0 ? 0x00FFFFFFu : 0x007FFFFFu);
    }

    if (mantissa >= 0.0) {
        fraction = (mantissa - 1.0) * 2147483648.0;
        extended_fraction = (uint64_t)floor(fraction);
        mantissa_bits = (u32)(extended_fraction >> 8);
    } else {
        fraction = (mantissa + 2.0) * 2147483648.0;
        extended_fraction = (uint64_t)floor(fraction);
        mantissa_bits = 0x00800000u | ((u32)(extended_fraction >> 8) & 0x007FFFFFu);
    }

    return ((u32)(uint8_t)(int8_t)exponent << 24) | mantissa_bits;
}

static inline float TMS320_C3X_STF_TO_SINGLE(double value) {
    return TMS320_C3X_SINGLE_TO_FLOAT(TMS320_C3X_DOUBLE_TO_SINGLE_RAW_TRUNC(value));
}

static inline int TMS320_C3X_FIX_TO_INT(float value) {
    return (int)floorf(value);
}

#define FIX(value) TMS320_C3X_FIX_TO_INT(value)

#endif /* MACS_H */

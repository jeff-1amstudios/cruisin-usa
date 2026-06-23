#ifndef MACS_H
#define MACS_H

#include "../core/machine.h"
#include "port.h"

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
#define INCAUD(NUMBER)       \
    do {                     \
        AR2 = (u32)(NUMBER); \
        AUDIT_INC();         \
    } while (0)

/*
READADJ	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	ADJUSTMENT_READ
        .ENDM
*/
#define READADJ(NUMBER)      \
    do {                     \
        AR2 = (u32)(NUMBER); \
        ADJUSTMENT_READ();   \
    } while (0)

/*
READAUD	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	AUDIT_READ
        .ENDM
*/
#define READAUD(NUMBER)      \
    do {                     \
        AR2 = (u32)(NUMBER); \
        AUDIT_READ();        \
    } while (0)

/*
SETADJ	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	ADJUSTMENT_WRITE
        .ENDM
*/
#define SETADJ(NUMBER)       \
    do {                     \
        AR2 = (u32)(NUMBER); \
        ADJUSTMENT_WRITE();  \
    } while (0)

/*
SETAUD	.MACRO	NUMBER
        LDI	:NUMBER:,AR2
        CALL	AUDIT_WRITE
        .ENDM
*/
#define SETAUD(NUMBER)       \
    do {                     \
        AR2 = (u32)(NUMBER); \
        AUDIT_WRITE();       \
    } while (0)

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

#define CRUSN_PUSH_REG32(REG) crusn_machine_push_reg32((REG))
#define CRUSN_PUSH_U32(REG) crusn_machine_push_u32((u32)(REG))
#define CRUSN_PUSH_ONE(REG) \
    _Generic((REG), crusn_reg32: CRUSN_PUSH_REG32, default: CRUSN_PUSH_U32)(REG)

#define CRUSN_POP_ONE(REG) \
    _Generic(&(REG), crusn_reg32*: ((REG) = crusn_machine_pop_reg32()), default: ((REG) = crusn_machine_pop_u32()))

#define CRUSN_FOR_EACH_1(M, A1) M(A1)
#define CRUSN_FOR_EACH_2(M, A1, A2) \
    M(A1);                          \
    M(A2)
#define CRUSN_FOR_EACH_3(M, A1, A2, A3) \
    M(A1);                              \
    M(A2);                              \
    M(A3)
#define CRUSN_FOR_EACH_4(M, A1, A2, A3, A4) \
    M(A1);                                  \
    M(A2);                                  \
    M(A3);                                  \
    M(A4)
#define CRUSN_FOR_EACH_5(M, A1, A2, A3, A4, A5) \
    M(A1);                                      \
    M(A2);                                      \
    M(A3);                                      \
    M(A4);                                      \
    M(A5)
#define CRUSN_FOR_EACH_6(M, A1, A2, A3, A4, A5, A6) \
    M(A1);                                          \
    M(A2);                                          \
    M(A3);                                          \
    M(A4);                                          \
    M(A5);                                          \
    M(A6)
#define CRUSN_FOR_EACH_7(M, A1, A2, A3, A4, A5, A6, A7) \
    M(A1);                                              \
    M(A2);                                              \
    M(A3);                                              \
    M(A4);                                              \
    M(A5);                                              \
    M(A6);                                              \
    M(A7)
#define CRUSN_FOR_EACH_8(M, A1, A2, A3, A4, A5, A6, A7, A8) \
    M(A1);                                                  \
    M(A2);                                                  \
    M(A3);                                                  \
    M(A4);                                                  \
    M(A5);                                                  \
    M(A6);                                                  \
    M(A7);                                                  \
    M(A8)

#define CRUSN_FOR_EACH_CHOOSER(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define CRUSN_FOR_EACH(M, ...) \
    CRUSN_FOR_EACH_CHOOSER(__VA_ARGS__, CRUSN_FOR_EACH_8, CRUSN_FOR_EACH_7, CRUSN_FOR_EACH_6, CRUSN_FOR_EACH_5, CRUSN_FOR_EACH_4, CRUSN_FOR_EACH_3, CRUSN_FOR_EACH_2, CRUSN_FOR_EACH_1)(M, __VA_ARGS__)

#define PUSHM(...)                                   \
    do {                                             \
        CRUSN_FOR_EACH(CRUSN_PUSH_ONE, __VA_ARGS__); \
    } while (0)

#define POPM(...)                                   \
    do {                                            \
        CRUSN_FOR_EACH(CRUSN_POP_ONE, __VA_ARGS__); \
    } while (0)

#endif /* MACS_H */

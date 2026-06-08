#ifndef ERROR_H
#define ERROR_H

/* Generated from asm/ERROR.EQU. */
/*
 * ERROR.EQU
 * COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * ERROR CODE IDS
 * ID = 	xxxxCCTS
 * x unused
 * C class
 * T type
 * ...
 */

/* Original .globl symbols in this module:
 *   ERROR_LOG
 *   ERROR_LOG_DISPLAY
 */

#define EC_MASK 0x0FF00
#define ET_MASK 0x000F0
#define ES_MASK 0x0000F
#define EC_OBJ 0x00100
#define ET_ALLOC 0x00010
#define ET_DELETE 0x00020
#define EC_PROC 0x00200
#define EC_COLL 0x00300
#define EC_PALL 0x00400
#define EC_BGND 0x00500
#define EC_WATCHDOG 0x00600
#define EC_UNFINISHED 0x00700
#define EC_WATCHDOG2 0x00800 /* reset triggered by > 100 frames (CUSA.ASM) */

#endif /* ERROR_H */

#ifndef ERROR_H
#define ERROR_H

/* Generated from asm/ERROR.EQU. */

// ERROR.EQU
// 
// COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// ALL RIGHTS RESERVED

// asm: 	.globl	ERROR_LOG
void ERROR_LOG(void);
// asm: 	.globl	ERROR_LOG_DISPLAY
void ERROR_LOG_DISPLAY(void);

// ERROR CODE IDS
// 
// ID = 	xxxxCCTS
// 
// x unused
// C class
// T type
// S subtype
// 
// 
// 
// 

// asm: EC_MASK		.set	0FF00h
#define EC_MASK 0x0FF00
// asm: ET_MASK		.set	000F0h
#define ET_MASK 0x000F0
// asm: ES_MASK		.set	0000Fh
#define ES_MASK 0x0000F

// asm: EC_OBJ		.set	00100h
#define EC_OBJ 0x00100
// asm: ET_ALLOC	.set	00010h
#define ET_ALLOC 0x00010
// asm: ET_DELETE	.set	00020h
#define ET_DELETE 0x00020

// asm: EC_PROC		.set	00200h
#define EC_PROC 0x00200
// asm: EC_COLL		.set	00300h
#define EC_COLL 0x00300
// asm: EC_PALL		.set	00400h
#define EC_PALL 0x00400
// asm: EC_BGND		.set	00500h
#define EC_BGND 0x00500
// asm: EC_WATCHDOG	.set	00600h
#define EC_WATCHDOG 0x00600
// asm: EC_UNFINISHED	.set	00700h
#define EC_UNFINISHED 0x00700
// asm: EC_WATCHDOG2	.set	00800h	;reset triggered by > 100 frames (CUSA.ASM)
#define EC_WATCHDOG2 0x00800 //reset triggered by > 100 frames (CUSA.ASM)

#endif /* ERROR_H */

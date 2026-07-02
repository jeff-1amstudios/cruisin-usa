#ifndef MPROC_H
#define MPROC_H

#include "../core/port.h"

/* Generated from asm/MPROC.EQU. */

// MPROC.EQU
//
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED
//
// ROUTINES

// PSADDR		.set	5	;UH	STARTUP ADDRESS
// PDBGTIM	.set	13	;UD

// asm: NUMPROC	.set	105	;NUMBER OF PROCESSES
#define NUMPROC 105 // NUMBER OF PROCESSES

// asm: 	.globl	CURRENT_PROC,OLDSP
extern PROC* CURRENT_PROC;

extern int OLDSP;

// asm: 	.globl	PRC_INIT
void PRC_INIT(void);

// asm: 	.globl	PRC_CREATE
PROC* PRC_CREATE(PROC_FUNC func /*AR2*/, int pid /*R2*/, PROC_CONTEXT* ctx);

// asm: 	.globl	PRC_CREATE_CHILD
void PRC_CREATE_CHILD(void);

// asm: 	.globl	PRC_DISPATCH
void PRC_DISPATCH(void);

// asm: 	.globl	PRC_SUICIDE
void PRC_SUICIDE(PROC* p);

// asm: 	.globl	PRC_KILL
void PRC_KILL(void);

// asm: 	.globl	PRC_KILLALL
void PRC_KILLALL(void);

// asm: 	.globl	PRC_EXISTP
void PRC_EXISTP(void);

// asm: 	.globl	PRC_XFER
void PRC_XFER(void);

// asm: 	.globl	PRC_FIND
void PRC_FIND(void);

// asm: 	.globl	PRC_FINDNEXT
void PRC_FINDNEXT(void);

// asm: 	.globl	PRC_FOLLOW
void PRC_FOLLOW(void);

// asm: 	.globl	SLEEP
void PRC_SLEEP(PROC* p, int ticks);

#define SLEEP(t)         \
    {                    \
        PRC_SLEEP(p, t); \
        return;          \
    }

#define DIE()           \
    {                   \
        PRC_SUICIDE(p); \
        return;         \
    }

#define REENTER(fn)   \
    {                 \
        p->state = 0; \
        fn(p);        \
    }

#endif /* MPROC_H */

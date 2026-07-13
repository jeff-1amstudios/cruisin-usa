#ifndef MPROC_H
#define MPROC_H

#include "../core/port.h"

/* Generated from asm/MPROC.EQU. */

// MPROC.EQU
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
void PRC_KILL(PROC* proc /*AR2*/);

// asm: 	.globl	PRC_KILLALL
void PRC_KILLALL(int pid, int mask);

// asm: 	.globl	PRC_EXISTP
void PRC_EXISTP(void);

// asm: 	.globl	PRC_XFER
void PRC_XFER(void);

// asm: 	.globl	PRC_FIND
PROC* PRC_FIND(int pid, int mask);

// asm: 	.globl	PRC_FINDNEXT
void PRC_FINDNEXT(void);

// asm: 	.globl	PRC_FOLLOW
void PRC_FOLLOW(void);

// asm: 	.globl	SLEEP
void PRC_SLEEP(PROC* p, int ticks);

#define SLEEP(ticks, resume_number)  \
    p->resume_state = resume_number; \
    PRC_SLEEP(p, ticks);             \
    return;                          \
    PROC_RESUME_##resume_number:

#define DIE()           \
    {                   \
        PRC_SUICIDE(p); \
        return;         \
    }

#define REENTER(fn)          \
    {                        \
        p->resume_state = 0; \
        fn(p);               \
        return;              \
    }

#endif /* MPROC_H */

#ifndef MPROC_H
#define MPROC_H

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
#define NUMPROC 105 //NUMBER OF PROCESSES

// asm: 	.globl	PRC_SLEEP
#define PRC_SLEEP SLEEP

// DATA STRUCTURES
// asm: 	.globl	PACTIVE,PFREE
extern int PACTIVE;

extern int PFREE;

// asm: 	.globl	PACTIVEI
#define PACTIVEI PACTIVE

// asm: 	.globl	CURRENT_PROC,OLDSP
extern int CURRENT_PROC;

extern int OLDSP;

// asm: 	.globl	PRC_INIT
void PRC_INIT(void);

// asm: 	.globl	PRC_CREATE
void PRC_CREATE(void);

// asm: 	.globl	PRC_CREATE_CHILD
void PRC_CREATE_CHILD(void);

// asm: 	.globl	PRC_DISPATCH
void PRC_DISPATCH(void);

// asm: 	.globl	PRC_SUICIDE
void PRC_SUICIDE(void);

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
void SLEEP(void);

// MPROC.ASM
// asm: 	hibss	PRCSTR,PRCSIZ*NUMPROC	;PROCESS STORE AREA
extern int PRCSTR[];

#endif /* MPROC_H */

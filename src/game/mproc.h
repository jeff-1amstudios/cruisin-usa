#ifndef MPROC_H
#define MPROC_H

/* Generated from asm/MPROC.EQU. */

// MPROC.EQU
// 
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED
// 
// ROUTINES

// STRUCT	PROC
// asm: PLINK	.set	0	;UH	LINK TO NEXT 32 BITS
#define PLINK 0 //UH	LINK TO NEXT 32 BITS

// asm: PSPTR	.set    1	;UH	PROCESS STACK POINTER 32 BITS
#define PSPTR 1 //UH	PROCESS STACK POINTER 32 BITS

// asm: PWAKE	.set	2	;UH
#define PWAKE 2 //UH

// asm: PID	.set	3	;UH	PROCESS ID
#define PID 3 //UH	PROCESS ID

// asm: PTIME	.set	4	;UH	SLEEP TIME X 16MSEC
#define PTIME 4 //UH	SLEEP TIME X 16MSEC

// asm: PR4	.set	5	;UH
#define PR4 5 //UH

// asm: PR5	.set	6	;UH
#define PR5 6 //UH

// asm: PR6	.set	7	;FL
#define PR6 7 //FL

// asm: PR7	.set	8	;FL
#define PR7 8 //FL

// asm: PAR4	.set	9	;UH
#define PAR4 9 //UH

// asm: PAR5	.set	10	;UH
#define PAR5 10 //UH

// asm: PAR6	.set	11	;UH
#define PAR6 11 //UH

// asm: PDATA	.set    12	;UH	PROCESS DATA STORE
#define PDATA 12 //UH	PROCESS DATA STORE

// asm: PSDATA	.set    47      ;UH	PROCESS STACK DATA
#define PSDATA 47 //UH	PROCESS STACK DATA

// asm: PRCSIZ	.set	62	;SIZ	END OF DATA STRUCTURE
#define PRCSIZ 62 //SIZ	END OF DATA STRUCTURE

// ENDSTRUCT
// PSADDR		.set	5	;UH	STARTUP ADDRESS
// PDBGTIM	.set	13	;UD

// asm: NUMPROC	.set	105	;NUMBER OF PROCESSES
#define NUMPROC 105 //NUMBER OF PROCESSES

// DATA STRUCTURES
// asm: 	.globl	PACTIVE,PFREE
extern int PACTIVE;

extern int PFREE;

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

// asm: 	.globl	PRC_SLEEP
void PRC_SLEEP(void);

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

#endif /* MPROC_H */

#ifndef MPROC_H
#define MPROC_H

/* Generated from asm/MPROC.ASM. */

// MPROC.ASM
// asm: 	.bss	CURRENT_PROC,1		;CURRENT PROCESS POINTER (DEBUG ONLY!)
extern int CURRENT_PROC;
// asm: 	.bss	OLDSP,1	    		;SAVE OLD STACK POINTER
extern int OLDSP;
// asm: 	.bss	PACTIVE,1		;ACTIVE PROCESS LIST POINTER
extern int PACTIVE;
// asm: 	.bss	PFREE,1			;FREE PROCESS LIST POINTER
extern int PFREE;
// asm: 	hibss	PRCSTR,PRCSIZ*NUMPROC	;PROCESS STORE AREA
extern int PRCSTR[];

#endif /* MPROC_H */

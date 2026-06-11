#ifndef MPROC_H
#define MPROC_H

/* Generated from asm/MPROC.ASM. */

// asm: 	.bss	CURRENT_PROC,1		;CURRENT PROCESS POINTER (DEBUG ONLY!)
// addr: 0x0000C9AE
extern int CURRENT_PROC;
// asm: 	.bss	OLDSP,1	    		;SAVE OLD STACK POINTER
// addr: 0x0000C9AF
extern int OLDSP;
// asm: 	.bss	PACTIVE,1		;ACTIVE PROCESS LIST POINTER
// addr: 0x0000C9B0
extern int PACTIVE;
// asm: 	.bss	PFREE,1			;FREE PROCESS LIST POINTER
// addr: 0x0000C9B1
extern int PFREE;
// asm: 	hibss	PRCSTR,PRCSIZ*NUMPROC	;PROCESS STORE AREA
// addr: 0x0000EC17
extern int PRCSTR[PRCSIZ*NUMPROC];

#endif /* MPROC_H */

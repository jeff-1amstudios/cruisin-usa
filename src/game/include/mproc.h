#ifndef MPROC_H
#define MPROC_H

/* Generated from asm/MPROC.EQU. */
/*
 * MPROC.EQU
 * COPYRIGHT (C) 1994  BY TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * ROUTINES
 * DATA STRUCTURES
 * STRUCT	PROC
 * ENDSTRUCT
 * PSADDR		.set	5	;UH	STARTUP ADDRESS
 * ...
 */

/* Original .globl symbols in this module:
 *   PRC_INIT
 *   PRC_CREATE
 *   PRC_CREATE_CHILD
 *   PRC_DISPATCH
 *   PRC_SLEEP
 *   PRC_SUICIDE
 *   PRC_KILL
 *   PRC_KILLALL
 *   PRC_EXISTP
 *   PRC_XFER
 *   PRC_FIND
 *   PRC_FINDNEXT
 *   PRC_FOLLOW
 *   SLEEP
 *   SUICIDE
 *   PACTIVE
 *   PFREE
 *   PACTIVEI
 *   CURRENT_PROC
 *   OLDSP
 */

#define PLINK 0 /* UH	LINK TO NEXT 32 BITS */
#define PSPTR 1 /* UH	PROCESS STACK POINTER 32 BITS */
#define PWAKE 2 /* UH */
#define PID 3 /* UH	PROCESS ID */
#define PTIME 4 /* UH	SLEEP TIME X 16MSEC */
#define PR4 5 /* UH */
#define PR5 6 /* UH */
#define PR6 7 /* FL */
#define PR7 8 /* FL */
#define PAR4 9 /* UH */
#define PAR5 10 /* UH */
#define PAR6 11 /* UH */
#define PDATA 12 /* UH	PROCESS DATA STORE */
#define PSDATA 47 /* UH	PROCESS STACK DATA */
#define PRCSIZ 62 /* SIZ	END OF DATA STRUCTURE */
#define NUMPROC 105 /* NUMBER OF PROCESSES */

#endif /* MPROC_H */

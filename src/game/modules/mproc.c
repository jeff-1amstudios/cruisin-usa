#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/mproc.h"
#include "../include/macs.h"
#include "../include/c30.h"
#include "../include/vunit.h"
#include "../include/error.h"
#include "../include/mproc_defs.h"

/*
 * Source module: asm/MPROC.ASM
 */

// *----------------------------------------------------------------------------
// *MULTIPROCESS SYSTEM
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *
#if DEBUG
/* asm: NUM_PROCS_ACTIVE	.bss	NUM_PROCS_ACTIVE,1 */
int NUM_PROCS_ACTIVE;
/* asm: NUM_PROCS_IDLE	.bss	NUM_PROCS_IDLE,1 */
int NUM_PROCS_IDLE;
#endif
/* asm: CURRENT_PROC	.bss	CURRENT_PROC,1 */
int CURRENT_PROC;
/* asm: OLDSP	.bss	OLDSP,1 */
int OLDSP;
/* asm: PACTIVE	.bss	PACTIVE,1 */
int PACTIVE;
/* asm: PFREE	.bss	PFREE,1 */
int PFREE;
/* asm: PRCSTR	hibss	PRCSTR,PRCSIZ*NUMPROC */
int PRCSTR[PRCSIZ*NUMPROC];
// *----------------------------------------------------------------------------
#endif
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *GET A PROCESS
// *
// *	START UP A PROCESS AT FUNCTION_NAME.
// *	-FIND AN AVAILABLE PROCESS SPACE
// *	-SETUP STORED VARIABLES (AR7-3,R4-7) AND START ADDRESS
// *	-SETUP LSP POSITION (LOCAL STACK POINTER)
// *	-SET SLEEP TIKS TO ZERO
// *
// *PARAMETERS
// *	R2	PID
// *	AR2	START ADDRESS
// *RETURNS
// *	(IF PROCESS IS AVAILABLE)
// *		CARRY CLEAR
// *		AR0	POINTER TO PROCESS BLOCK
// *	(IF NO PROCESSES AVAILABLE)
// *		CARRY SET
// *
// *----------------------------------------------------------------------------

void PRC_CREATE(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@PFREE,R0		;TAKE OFF THE TOP OF PROCESS FREE LIST
    // asm: 	BNZ	GETPROC0
    // asm: 	ERRON	U,EC_PROC|ET_ALLOC	;OUT OF PROCESSES ERROR
    // asm: 	SETC
    // asm: 	B	GETPROCX
GETPROC0:
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	STI	R0,@PFREE		;AND UPDATE FREE LIST
    // asm: 	LDI	@PACTIVE,R0		;INSERT TO HEAD OF PROCESS ACTIVE LIST
    // asm: 	STI	R0,*AR0
    // asm: 	STI	AR0,@PACTIVE
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR0(PTIME)		;PLACE SLEEP TIME
    // asm: 	STI	AR2,*+AR0(PWAKE)	;START ADDRESS OF PROCESS
    // ;	STI	AR2,*+AR0(PSADDR)	;START ADDRESS OF PROCESS (SAVE FOR DEBUG & ID)
    // asm: 	STI	R2,*+AR0(PID)		;SET PROCESS TYPE
    // asm: 	LDI	AR0,R0
    // asm: 	ADDI	PSDATA,R0	  	;WHERE LOCAL STACK POINTER ACTUALLY IS
    // asm: 	STI	R0,*+AR0(PSPTR)		;STORE LOCAL STACK POINTER (LSP)
    // asm: 	STI	AR4,*+AR0(PAR4)
    // asm: 	STI	AR5,*+AR0(PAR5)
    // asm: 	STI	AR6,*+AR0(PAR6)
    // asm: 	STI	R4,*+AR0(PR4)
    // asm: 	STI	R5,*+AR0(PR5)
    // asm: 	STF	R6,*+AR0(PR6)
    // asm: 	STF	R7,*+AR0(PR7)
#if DEBUG
    // asm: 	LDI	@NUM_PROCS_ACTIVE,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@NUM_PROCS_ACTIVE
    // asm: 	LDI	@NUM_PROCS_IDLE,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@NUM_PROCS_IDLE
#endif
    // asm: 	CLRC
GETPROCX:
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_CREATE", 0, 0);
    UNIMPL();
}

void PRC_CREATE_CHILD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET A CHILD PROCESS
    // *LINKED AFTER AR7 (CALLING PROC) ON LIST
    // *RETURNS
    // *	AR0	POINTER TO PROCESS
    // *
    // asm:     	CALL 	PRC_CREATE
    // asm: 	RETSC
    // asm: 	PUSH	R0
    // asm: 	LDI	*AR0,R0			;PULL HIM FROM FRONT OF LIST
    // asm: 	STI	R0,@PACTIVE
    // asm: 	LDI	*AR7,R0			;PUT HIM AFTER CREATING PROCESS
    // asm: 	STI	R0,*AR0
    // asm: 	STI	AR0,*AR7
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_CREATE_CHILD", 0, 0);
    UNIMPL();
}

void PRC_DISPATCH(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DISPPROC
    // *
    // *	Dispatch current process list.
    // *
    // *
    // *REGISTER ALLOCATION (WITHIN PROCESSES) AS FOLLOWS:
    // *
    // *ALL REGISTERS TRASHED EXCEPT THE FOLLOWING:
    // *
    // *AR4	SAVED (OBJECT POINTER)
    // *AR5	SAVED (CAR BLOCK POINTER W/DRONES/PLAYER)
    // *AR6	SAVED
    // *AR7	PROCESS POINTER
    // *
    // *R4	SAVED AS INT
    // *R5	SAVED AS INT
    // *R6	SAVED AS FLOAT
    // *R7	SAVED AS FLOAT
    // *
    // *
    // asm: DISPPROC
    // asm: 	LDI	@PACTIVEI,AR7
    // asm: 	B	NEXTPRC
    // *
    // *SLEEP
    // *	SEND CURRENT PROCESS TO SLEEP.
    // *	SAVE RETURN ADDRESS
    // *	GO GET NEXT PROCESS
    // *PASSED
    // *	AR2	SLEEP TIME x 16MSEC.
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_DISPATCH", 0, 0);
    UNIMPL();
}

void PRC_SLEEP(void)
{
    // asm: SLEEP
    // asm: 	POP	R0
    // asm: 	STI	R0,*+AR7(PWAKE)		;SAVE WAKEUP ADDRESS
#if DEBUG
    // asm: 	CALL	PRC_DEBUG_CHECK
#endif
#if DEBUG
    // asm: 	CMPI	0,DP
    // asm: 	BNE	$
    // asm: 	CMPI	@OLDSP,SP		;PROC IN AR7
    // asm: 	SLOCKON	NE,"_sleep   OLDSP != SP  *FATAL*"
    // asm: 	LDI	@CURRENT_PROC,R0
    // asm: 	CMPI	R0,AR7			;AR7 HAS BEEN TRASHED!
    // asm: 	SLOCKON	NE,"_sleep   CURRENT_PROC != AR7  *FATAL*"
    // ;	LDP	@TIMER_CNTR1
    // ;	LDI	@TIMER_CNTR1,R0
    // ;	STI	R0,*+AR7(PDBGTIM)
    // ;	SETDP
#endif
    // asm: 	SETDP
    // asm: 	LDI	@OLDSP,SP		;PROC IN AR7
    // asm: 	LDI	@CURRENT_PROC,AR7
    // asm: 	STI	R4,*+AR7(PR4)
    // asm: 	STI	R5,*+AR7(PR5)
    // asm: 	STF	R6,*+AR7(PR6)
    // asm: 	STF	R7,*+AR7(PR7)
    // asm: 	STI	AR2,*+AR7(PTIME)	;SAVE SLEEP TIME
    // asm: 	STI	AR4,*+AR7(PAR4)
    // asm: 	STI	AR5,*+AR7(PAR5)
    // asm: 	STI	AR6,*+AR7(PAR6)
NEXTPRC:
    // asm: LDI	*AR7,R0			;GET NEXT PROC, SET Z FLAG
NP1:
    // asm: BZD	DISPPRCX
    // asm: 	LDI	R0,AR7			;PUT IT IN AR7
    // asm: 	LDI	*+AR7(PTIME),R0		;IS SLEEP TIME ZERO?
    // asm: 	SUBI	1,R0
    // 	;---->BZ DISPPRCX
    // asm: 	BGTD	NP1
    // asm: 	STI	R0,*+AR7(PTIME)
    // asm: 	LDI	*AR7,R0			;GET NEXT PROC
    // asm: 	NOP				;FOR DELAYED BRANCH
    // 	;---->BGT NP1
    // asm: EXEC
    // asm: 	STI	SP,@OLDSP
    // asm: 	STI	AR7,@CURRENT_PROC	;SAVE CURRENT PROCESS POINTER
    // asm: 	LDI	*+AR7(PAR6),AR6
    // asm: 	LDI	*+AR7(PAR5),AR5
    // asm: 	LDI	*+AR7(PAR4),AR4
    // asm: 	LDF	*+AR7(PR7),R7
    // asm: 	LDI	*+AR7(PWAKE),R0
    // asm: 	BUD	R0
    // asm: 	LDF	*+AR7(PR6),R6
    // asm: 	LDI	*+AR7(PR5),R5
    // asm: 	LDI	*+AR7(PR4),R4
    // 	;---->BU R0 DELAYED BRANCH HERE
DISPPRCX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_SLEEP", 0, 0);
    UNIMPL();
}

void PRC_SUICIDE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SUICIDE
    // *
    // *REMOVE CURRENT PROCESS FROM ACTIVE LIST
    // *
    // *PROCESSES MUST BRANCH TO SUICIDE
    // *
SUICIDE:
#if DEBUG
    // asm: 	PUSH	R0
    // asm: 	LDI	@NUM_PROCS_ACTIVE,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@NUM_PROCS_ACTIVE
    // asm: 	LDI	@NUM_PROCS_IDLE,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@NUM_PROCS_IDLE
    // asm: 	POP	R0
    // asm: 	CALL	PRC_DEBUG_CHECK
#endif
#if DEBUG
    // asm: 	CMPI	0,DP
    // asm: 	BNE	$
    // asm: 	CMPI	@OLDSP,SP
    // asm: 	SLOCKON	NE,"SUICIDE   OLD != SP  *FATAL*"
    // asm: 	BNE	$			;PROC IN AR7
    // asm: 	LDI	@CURRENT_PROC,R0
    // asm: 	CMPI	R0,AR7
    // asm: 	SLOCKON	NE,"SUICIDE   CURRENT_PROC != AR7  *FATAL*"
#endif
    // asm: 	LDI	*AR7,R0			;LINK TO NEXT PROC
    // asm: 	LDI	@PFREE,AR1		;LINK TO START OF FREE
    // asm: 	STI	AR1,*AR7
    // asm: 	STI	AR7,@PFREE
    // asm: 	LDI	@PACTIVEI,R1		;WE MUST FIND DEAD PROCESS TO LINK AROUND
DIELP:
    // asm: LDI	R1,AR1
    // asm: 	LDI	*AR1,R1
    // asm: 	ERRON	Z,EC_PROC|ET_DELETE	;SUICIDE   PROCESS NOT FOUND??? *FATAL*"
    // asm: 	BZ	NEXTPRC
    // asm: 	CMPI	R1,AR7
    // asm: 	BNE	DIELP
    // asm: 	BUD	NEXTPRC
    // asm: 	STI	R0,*AR1			;LINK AROUND DEAD PROCESS
    // asm: 	LDI	AR1,AR7			;SO SOMETHING IS POINTING TO NEXT PROC
    // asm: 	NOP
    // 	;--->BR NEXTPRC
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_SUICIDE", 0, 0);
    UNIMPL();
}

void PRC_KILL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *KILL A PROCESS
    // *
    // *FIND AND REMOVE PROCESS FROM ACTIVE LIST
    // *
    // *PARAMETERS
    // *	AR2	POINTER TO PROCESS TO KILL
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR1
    // asm: 	CMPI	AR2,AR7
    // asm: 	ERRON	Z,EC_PROC|ET_DELETE|3	;KILL ATTEMPTED ON SELF"
    // asm: 	BEQ	SUICIDE
    // asm: 	LDI	@PACTIVEI,R1		;WE MUST FIND DEAD PROCESS TO LINK AROUND
KILLP:
    // asm: 	LDI	R1,AR1
    // asm: 	LDI	*AR1,R1
    // asm: 	ERRON	Z,EC_PROC|ET_DELETE|4	;LOCKUP ON END OF LIST FOUND"
    // asm: 	BZ	KILL_X
    // asm: 	CMPI	R1,AR2
    // asm: 	BNE	KILLP
    // asm: 	LDI	*AR2,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	@PFREEI,AR1
    // asm: 	LDI	*AR1,R1
    // asm: 	STI	R1,*AR2
    // asm: 	STI	AR2,*AR1
KILL_X:
#if DEBUG
    // asm: 	PUSH	R0
    // asm: 	LDI	@NUM_PROCS_ACTIVE,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@NUM_PROCS_ACTIVE
    // asm: 	LDI	@NUM_PROCS_IDLE,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@NUM_PROCS_IDLE
    // asm: 	POP	R0
#endif
    // asm: 	POP	AR1
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_KILL", 0, 0);
    UNIMPL();
}

void PRC_KILLALL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *KILLALL
    // *KILL CLASS OF PROCESSES
    // *
    // *PARAMETERS
    // *	R0	PID
    // *	R1	MASK
    // *
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	AND	R1,R0
    // asm: 	LDI	@PACTIVEI,AR2		;WE MUST FIND DEAD PROCESS TO LINK AROUND
KILLALLP:
    // asm: 	LDI	AR2,AR1
KLP0:
    // asm: 	LDI	*AR1,R2
    // asm: 	BZ	KADONE			;WE'RE DONE
    // asm: 	LDI	R2,AR2
    // asm: 	LDI	*+AR2(PID),R2
    // asm: 	AND	R1,R2
    // asm: 	CMPI	R2,R0
    // asm: 	BNZ	KILLALLP
    // asm: 	CMPI	AR7,AR2
    // asm: 	BZ	KILLALLP		;DONT KILL YOURSELF
    // asm: 	LDI	*AR2,R2
    // asm: 	STI	R2,*AR1			;LINK AROUND
    // asm: 	LDI	@PFREE,R2
    // asm: 	STI	R2,*AR2
    // asm: 	STI	AR2,@PFREE
#if DEBUG
    // asm: 	PUSH	R0
    // asm: 	LDI	@NUM_PROCS_ACTIVE,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@NUM_PROCS_ACTIVE
    // asm: 	LDI	@NUM_PROCS_IDLE,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@NUM_PROCS_IDLE
    // asm: 	POP	R0
#endif
    // asm: 	BR	KLP0
KADONE:
#if DEBUG
    // asm: 	CALL	PRC_DEBUG_CHECK
#endif
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_KILLALL", 0, 0);
    UNIMPL();
}

void PRC_EXISTP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PRC_EXISTP	DOES A PROCESS EXIST
    // *
    // *PARAMETERS
    // *	AR2	PID
    // *	R2	MASK
    // *RETURNS
    // *	(IF FOUND)
    // *		C=1 IF FOUND
    // *		R0 POINTING TO PROCESS
    // *	(IF NOT FOUND)
    // *		C=0
    // *		R0=0 IF PROCESS NOT FOUND
    // *
    // asm: 	CLRC				;CLEAR CARRY
    // asm: 	PUSH	AR2
    // asm: 	LDI	AR2,RC
    // asm: 	AND	R2,RC
    // asm: 	LDI	@PACTIVEI,AR2
EXPL:
    // asm: 	LDI	*AR2,R0
    // asm: 	BZ	EXDONE			;WE'RE DONE, ITS NOT HERE
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	*+AR2(PID),RS
    // asm: 	AND	R2,RS
    // asm: 	CMPI	RC,RS
    // asm: 	BNZ	EXPL
    // asm: 	SETC				;WE FOUND IT DUDES!!!
EXDONE:
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_EXISTP", 0, 0);
    UNIMPL();
}

void PRC_INIT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *INITIALIZE PROCESS DATA STRUCTURES
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDI	@PACTIVEI,AR0		;ZERO ACTIVE POINTER
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
    // asm: 	LDI	@PFREEI,AR0		;GET FREE POINTER
    // asm: 	LDI	@PRCSTRI,AR1
    // asm: 	LDI	NUMPROC-1,RC
    // asm: 	RPTB	PINITL
    // asm: 	STI	AR1,*AR0
    // asm: 	LDI	AR1,AR0
PINITL:
    // asm: ADDI	PRCSIZ,AR1
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
#if DEBUG
    // asm: 	STI	R0,@NUM_PROCS_ACTIVE
    // asm: 	LDI	NUMPROC,R0
    // asm: 	STI	R0,@NUM_PROCS_IDLE
#endif
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_INIT", 0, 0);
    UNIMPL();
}

void PRC_XFER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *XFERPROC
    // *
    // *TRANSFER A PROCESS TO A DIFFERENT WAKEUP ADDRESS. WAKEUP WILL BE NEXT
    // *POSSIBLE
    // *
    // *PARAMETERS
    // *	AR0	POINTER TO PROCESS
    // *	AR1	POINTER TO ADDRESS TO WAKE UP
    // *
    // asm: 	PUSH	AR1
#if DEBUG
    // asm: 	CMPI	AR0,AR7				;ARE WE ATTEMPTING TO XFER OURSELVES?
    // asm: 	SLOCKON	Z,"XFERPROC  ATTEMPT TO XFER OURSELF"
#endif
    // asm: 	STI	AR1,*+AR0(PWAKE)		;SAVE WAKEUP ADDRESS
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR0(PTIME)			;WAKEUP ASAP
    // 	;someday it might be nessesary to re-initialize the
    // 	;processes stack.  for now this is ignored.
    // asm: 	POP	AR1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_XFER", 0, 0);
    UNIMPL();
}

void PRC_FINDNEXT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PRC_FIND
    // *	FIND PROCESS FROM ACTIVE LIST
    // *
    // *PARAMETERS
    // *	R0	PID
    // *	R1	PID MASK
    // *RETURNS
    // *	AR0	POINTER TO PROCESS IF FOUND OR ZERO IF NONE FOUND
    // *
    // *
    // *PRC_FINDNEXT
    // *
    // *
    // *PARAMETERS
    // *	AR0	POINTER TO LIST
    // *
    // asm: 	BUD	FINDE
    // asm: 	PUSH	R2
    // asm: 	AND	R1,R0
    // asm: 	NOP
    // 	;---->	BUD	FINDE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_FINDNEXT", 0, 0);
    UNIMPL();
}

void PRC_FIND(void)
{
    // asm: 	BUD	FINDE
    // asm: 	PUSH	R2
    // asm: 	AND	R1,R0
    // asm: 	LDI	@PACTIVEI,AR0		;WE MUST FIND DEAD PROCESS TO LINK AROUND
    // 	;---->	BUD	FINDE
FINDLP:
    // asm: 	CMPI	R2,R0
    // asm: 	BEQ	FINDPROCX
FINDE:
    // asm: LDI	*AR0,R2			;SET CONDITION CODE
    // asm: 	BNZD	FINDLP
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	*+AR0(PID),R2
    // asm: 	AND	R1,R2
    // 	;---->	BNZD	FINDLP
FINDPROCX:
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_FIND", 0, 0);
    UNIMPL();
}

void PRC_FOLLOW(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SET THIS PROCESS TO FOLLOW SPECIFIED PROCESS
    // *	(ASSERT ME AS A CHILD OF THE SPECIFIED PROCESS)
    // *
    // *PARAMETERS
    // *	AR2	PROCESS TO FOLLOW
    // *	AR7	!THIS PROCESS!
    // *
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR7
    // asm: 	LDI	@PACTIVEI,R1		;WE MUST FIND DEAD PROCESS TO LINK AROUND
PFOLLP:
    // asm: 	LDI	R1,AR1
    // asm: 	LDI	*AR1,R1
    // asm: 	ERRON	Z,EC_PROC|6		;LOCKUP ON END OF LIST FOUND
    // asm: 	BZ	PROC_FOLLOW_X
    // asm: 	CMPI	R1,AR7
    // asm: 	BNE	PFOLLP
    // asm: 	LDI	*AR7,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	*AR2,R1
    // asm: 	STI	R1,*AR7
    // asm: 	STI	AR7,*AR2
PROC_FOLLOW_X:
    // asm: 	POP	AR7
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_FOLLOW", 0, 0);
    UNIMPL();
}

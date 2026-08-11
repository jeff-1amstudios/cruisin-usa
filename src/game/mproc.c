#include "mproc.h"

#include "../core/machine.h"
#include "../core/validator.h"
#include "c30.h"
#include "error.h"
#include "macs.h"
#include "vunit.h"

/*
 * Source module: asm/MPROC.ASM
 */

static void PRC_DEBUG_CHECK(void);
static void NEXTPRC(PROC* proc);
PROC* PRC_CREATE_CHILD(PROC_FUNC func /*AR2*/, int pid /*R2*/, PROC_CONTEXT* ctx);
void PRC_DISPATCH(void);
void PRC_KILL(PROC* proc /*AR2*/);
void PRC_KILLALL(int pid, int mask);
void PRC_EXISTP(void);
void PRC_INIT(void);
void PRC_XFER(void);
void PRC_FINDNEXT(void);
PROC* PRC_FIND(int pid, int mask);
void PRC_FOLLOW(void);

#define PACTIVEI PACTIVE
#define PFREEI PFREE
#define PRCSTRI PRCSTR

/*
 *----------------------------------------------------------------------------
 *MULTIPROCESS SYSTEM
 *
 */

#if DEBUG
/* asm: NUM_PROCS_ACTIVE	.bss	NUM_PROCS_ACTIVE,1 */
static int NUM_PROCS_ACTIVE;
/* asm: NUM_PROCS_IDLE	.bss	NUM_PROCS_IDLE,1 */
static int NUM_PROCS_IDLE;
#endif
/* asm: CURRENT_PROC	.bss	CURRENT_PROC,1 */
PROC* CURRENT_PROC;
/* asm: OLDSP	.bss	OLDSP,1 */
int OLDSP;
/* asm: PACTIVE	.bss	PACTIVE,1 */
PROC* PACTIVE;
/* asm: PFREE	.bss	PFREE,1 */
PROC* PFREE;
PROC PRCSTR[NUMPROC];

// *----------------------------------------------------------------------------
static void NEXTPRC(PROC* proc) {
    PROC* next_proc;

    // asm 0000A8AC: NEXTPRC	LDI	*AR7,R0			;GET NEXT PROC, SET Z FLAG
    // asm 0000A8AD: NP1	BZD	DISPPRCX
    // asm 0000A8AE: 	LDI	R0,AR7			;PUT IT IN AR7
    // asm 0000A8AF: 	LDI	*+AR7(PTIME),R0		;IS SLEEP TIME ZERO?
    // asm 0000A8B0: 	SUBI	1,R0
    // asm 0000A8B1: 	BGTD	NP1
    // asm 0000A8B2: 	STI	R0,*+AR7(PTIME)
    // asm 0000A8B3: 	LDI	*AR7,R0			;GET NEXT PROC
    // asm 0000A8B4: 	NOP				;FOR DELAYED BRANCH
    // asm 0000A8B5: EXEC
    // asm 0000A8B5: 	STI	SP,@OLDSP
    // asm 0000A8B6: 	STI	AR7,@CURRENT_PROC	;SAVE CURRENT PROCESS POINTER
    // asm 0000A8B7: 	LDI	*+AR7(PAR6),AR6
    // asm 0000A8B8: 	LDI	*+AR7(PAR5),AR5
    // asm 0000A8B9: 	LDI	*+AR7(PAR4),AR4
    // asm 0000A8BA: 	LDF	*+AR7(PR7),R7
    // asm 0000A8BB: 	LDI	*+AR7(PWAKE),R0
    // asm 0000A8BC: 	BUD	R0
    // asm 0000A8BD: 	LDF	*+AR7(PR6),R6
    // asm 0000A8BE: 	LDI	*+AR7(PR5),R5
    // asm 0000A8BF: 	LDI	*+AR7(PR4),R4
    // asm 0000A8C0: DISPPRCX	RETS
    while (proc != NULL) {
        if (proc->sleep_ticks > 0) {
            proc->sleep_ticks -= 1;
        }

        if (proc->sleep_ticks == 0) {
            next_proc = proc->link;
            CURRENT_PROC = proc;
            proc->current_resume_depth = 0;
            proc->yielded = 0;
            proc->func(proc);

            if (CURRENT_PROC != proc) {
                return;
            }

            proc = next_proc;
            continue;
        }

        proc = proc->link;
    }
}

// *----------------------------------------------------------------------------
static void PRC_DEBUG_CHECK(void) {
#if DEBUG
    // asm: 	PUSH	R0
    // asm: 	LDI	@NUM_PROCS_ACTIVE,R0
    // asm: 	ADDI	@NUM_PROCS_IDLE,R0
    // asm: 	CMPI	NUMPROC,R0
    // asm: 	BNE	$
    // asm: 	POP	R0
#endif
    // asm 0000A86E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_DEBUG_CHECK", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GET A PROCESS
 *
 *	START UP A PROCESS AT FUNCTION_NAME.
 *	-FIND AN AVAILABLE PROCESS SPACE
 *	-SETUP STORED VARIABLES (AR7-3,R4-7) AND START ADDRESS
 *	-SETUP LSP POSITION (LOCAL STACK POINTER)
 *	-SET SLEEP TIKS TO ZERO
 *
 *PARAMETERS
 *	R2	PID
 *	AR2	START ADDRESS
 *RETURNS
 *	(IF PROCESS IS AVAILABLE)
 *		CARRY CLEAR
 *		AR0	POINTER TO PROCESS BLOCK
 *	(IF NO PROCESSES AVAILABLE)
 *		CARRY SET
 *
 */
PROC* PRC_CREATE(PROC_FUNC func /*AR2*/, int pid /*R2*/, PROC_CONTEXT* ctx) {
    PROC* proc;

    // asm 0000A86F: 	PUSH	R0
    // asm 0000A870: 	LDI	@PFREE,R0		;TAKE OFF THE TOP OF PROCESS FREE LIST
    // asm 0000A871: 	BNZ	GETPROC0
    // asm 0000A872: 	ERRON	U,EC_PROC|ET_ALLOC	;OUT OF PROCESSES ERROR
    // asm 0000A87A: 	SETC
    // asm 0000A87B: 	B	GETPROCX
    proc = PFREE;
    if (proc == NULL) {
        ERRON(EC_PROC | ET_ALLOC);
        return NULL;
    }

GETPROC0:
    // asm 0000A87C: 	LDI	R0,AR0
    // asm 0000A87D: 	LDI	*AR0,R0
    // asm 0000A87E: 	STI	R0,@PFREE		;AND UPDATE FREE LIST
    PFREE = proc->link;

    // asm 0000A87F: 	LDI	@PACTIVE,R0		;INSERT TO HEAD OF PROCESS ACTIVE LIST
    // asm 0000A880: 	STI	R0,*AR0
    // asm 0000A881: 	STI	AR0,@PACTIVE
    proc->link = PACTIVE;
    PACTIVE = proc;

    // asm 0000A882: 	LDI	0,R0
    // asm 0000A883: 	STI	R0,*+AR0(PTIME)		;PLACE SLEEP TIME
    for (int resume_depth = 0; resume_depth < PROC_RESUME_STACK_SIZE; resume_depth++) {
        proc->resume_states[resume_depth] = 0;
    }
    proc->current_resume_depth = 0;
    proc->yielded = 0;
    proc->sleep_ticks = 0;

    // asm 0000A884: 	STI	AR2,*+AR0(PWAKE)	;START ADDRESS OF PROCESS
    // ;	STI	AR2,*+AR0(PSADDR)	;START ADDRESS OF PROCESS (SAVE FOR DEBUG & ID)
    proc->func = func;

    // asm 0000A885: 	STI	R2,*+AR0(PID)		;SET PROCESS TYPE
    proc->id = pid;

    proc->ctx = ctx;

    // asm 0000A886: 	LDI	AR0,R0
    // asm 0000A887: 	ADDI	PSDATA,R0	  	;WHERE LOCAL STACK POINTER ACTUALLY IS
    // asm 0000A888: 	STI	R0,*+AR0(PSPTR)		;STORE LOCAL STACK POINTER (LSP)

    // asm 0000A889: 	STI	AR4,*+AR0(PAR4)
    // asm 0000A88A: 	STI	AR5,*+AR0(PAR5)
    // asm 0000A88B: 	STI	AR6,*+AR0(PAR6)
    // asm 0000A88C: 	STI	R4,*+AR0(PR4)
    // asm 0000A88D: 	STI	R5,*+AR0(PR5)
    // asm 0000A88E: 	STF	R6,*+AR0(PR6)
    // asm 0000A88F: 	STF	R7,*+AR0(PR7)

#if DEBUG
    // asm: 	LDI	@NUM_PROCS_ACTIVE,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@NUM_PROCS_ACTIVE
    NUM_PROCS_ACTIVE += 1;

    // asm: 	LDI	@NUM_PROCS_IDLE,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@NUM_PROCS_IDLE
    NUM_PROCS_IDLE -= 1;
#endif
    // asm 0000A890: 	CLRC
GETPROCX:
    // asm 0000A891: 	POP	R0
    // asm 0000A892: 	RETS
    return proc;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GET A CHILD PROCESS
 *LINKED AFTER AR7 (CALLING PROC) ON LIST
 *RETURNS
 *	AR0	POINTER TO PROCESS
 *
 */
PROC* PRC_CREATE_CHILD(PROC_FUNC func /*AR2*/, int pid /*R2*/, PROC_CONTEXT* ctx) {
    PROC* proc;

    // asm 0000A893:     	CALL 	PRC_CREATE
    proc = PRC_CREATE(func, pid, ctx);
    // asm 0000A894: 	RETSC
    if (proc == NULL) {
        return NULL;
    }
    // asm 0000A895: 	PUSH	R0
    // asm 0000A896: 	LDI	*AR0,R0			;PULL HIM FROM FRONT OF LIST
    // asm 0000A897: 	STI	R0,@PACTIVE
    // asm 0000A898: 	LDI	*AR7,R0			;PUT HIM AFTER CREATING PROCESS
    // asm 0000A899: 	STI	R0,*AR0
    // asm 0000A89A: 	STI	AR0,*AR7
    if (CURRENT_PROC != NULL) {
        PACTIVE = proc->link;
        proc->link = CURRENT_PROC->link;
        CURRENT_PROC->link = proc;
    }
    // With the root sentinel (CURRENT_PROC == NULL), PRC_CREATE already placed
    // the child at PACTIVE, which is exactly where the assembly reinserts it.
    // asm 0000A89B: 	POP	R0
    // asm 0000A89C: 	RETS
    return proc;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DISPPROC
 *
 *	Dispatch current process list.
 *
 *
 *REGISTER ALLOCATION (WITHIN PROCESSES) AS FOLLOWS:
 *
 *ALL REGISTERS TRASHED EXCEPT THE FOLLOWING:
 *
 *AR4	SAVED (OBJECT POINTER)
 *AR5	SAVED (CAR BLOCK POINTER W/DRONES/PLAYER)
 *AR6	SAVED
 *AR7	PROCESS POINTER
 *
 *R4	SAVED AS INT
 *R5	SAVED AS INT
 *R6	SAVED AS FLOAT
 *R7	SAVED AS FLOAT
 *
 *
 */
void PRC_DISPATCH(void) {
    // asm 0000A89D: DISPPROC
    // asm 0000A89D: 	LDI	@PACTIVEI,AR7
    // asm 0000A89E: 	B	NEXTPRC
    // *
    // *SLEEP
    // *	SEND CURRENT PROCESS TO SLEEP.
    // *	SAVE RETURN ADDRESS
    // *	GO GET NEXT PROCESS
    // *PASSED
    // *	AR2	SLEEP TIME x 16MSEC.
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    MAME_ASSERT_FUNCTION_ENTRY();
    NEXTPRC(PACTIVE);
}

void PRC_SLEEP(PROC* p, int ticks) {
    // asm 0000A89F: 	POP	R0
    // asm 0000A8A0: 	STI	R0,*+AR7(PWAKE)		;SAVE WAKEUP ADDRESS
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
    // asm 0000A8A1: 	SETDP
    // asm 0000A8A2: 	LDI	@OLDSP,SP		;PROC IN AR7
    // asm 0000A8A3: 	LDI	@CURRENT_PROC,AR7
    // asm 0000A8A4: 	STI	R4,*+AR7(PR4)
    // asm 0000A8A5: 	STI	R5,*+AR7(PR5)
    // asm 0000A8A6: 	STF	R6,*+AR7(PR6)
    // asm 0000A8A7: 	STF	R7,*+AR7(PR7)
    // asm 0000A8A8: 	STI	AR2,*+AR7(PTIME)	;SAVE SLEEP TIME
    // asm 0000A8A9: 	STI	AR4,*+AR7(PAR4)
    // asm 0000A8AA: 	STI	AR5,*+AR7(PAR5)
    // asm 0000A8AB: 	STI	AR6,*+AR7(PAR6)
    p->sleep_ticks = ticks;
    NEXTPRC(p->link);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SUICIDE
 *
 *REMOVE CURRENT PROCESS FROM ACTIVE LIST
 *
 *PROCESSES MUST BRANCH TO SUICIDE
 *
 */
void PRC_SUICIDE(PROC* p) {
    PROC* next_proc;
    PROC** linkp;

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
    // asm 0000A8C1: 	LDI	*AR7,R0			;LINK TO NEXT PROC
    // asm 0000A8C2: 	LDI	@PFREE,AR1		;LINK TO START OF FREE
    // asm 0000A8C3: 	STI	AR1,*AR7
    // asm 0000A8C4: 	STI	AR7,@PFREE
    // asm 0000A8C5: 	LDI	@PACTIVEI,R1		;WE MUST FIND DEAD PROCESS TO LINK AROUND
DIELP:
    // asm 0000A8C6: LDI	R1,AR1
    // asm 0000A8C7: 	LDI	*AR1,R1
    // asm 0000A8C8: 	ERRON	Z,EC_PROC|ET_DELETE	;SUICIDE   PROCESS NOT FOUND??? *FATAL*"
    // asm 0000A8D0: 	BZ	NEXTPRC
    // asm 0000A8D1: 	CMPI	R1,AR7
    // asm 0000A8D2: 	BNE	DIELP
    // asm 0000A8D3: 	BUD	NEXTPRC
    // asm 0000A8D4: 	STI	R0,*AR1			;LINK AROUND DEAD PROCESS
    // asm 0000A8D5: 	LDI	AR1,AR7			;SO SOMETHING IS POINTING TO NEXT PROC
    // asm 0000A8D6: 	NOP
    // 	;--->BR NEXTPRC
    next_proc = p->link;
    p->link = PFREE;
    PFREE = p;

    linkp = &PACTIVE;
    while (*linkp != NULL) {
        if (*linkp == p) {
            *linkp = next_proc;
            if (p->ctx != NULL) {
                free(p->ctx);
                p->ctx = NULL;
            }
            NEXTPRC(next_proc);
            return;
        }
        linkp = &(*linkp)->link;
    }

    ERRON(EC_PROC | ET_DELETE);
    NEXTPRC(next_proc);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *KILL A PROCESS
 *
 *FIND AND REMOVE PROCESS FROM ACTIVE LIST
 *
 *PARAMETERS
 *	AR2	POINTER TO PROCESS TO KILL
 *
 */
void PRC_KILL(PROC* proc /*AR2*/) {
    PROC** linkp;

    // asm 0000A8D7: 	PUSH	R1
    // asm 0000A8D8: 	PUSH	AR1
    // asm 0000A8D9: 	CMPI	AR2,AR7
    // asm 0000A8DA: 	ERRON	Z,EC_PROC|ET_DELETE|3	;KILL ATTEMPTED ON SELF"
    // asm 0000A8E2: 	BEQ	SUICIDE
    if (proc == CURRENT_PROC) {
        ERRON(EC_PROC | ET_DELETE | 3);
        PRC_SUICIDE(CURRENT_PROC);
        return;
    }
    // asm 0000A8E3: 	LDI	@PACTIVEI,R1		;WE MUST FIND DEAD PROCESS TO LINK AROUND
    linkp = &PACTIVE;
KILLP:
    // asm 0000A8E4: 	LDI	R1,AR1
    // asm 0000A8E5: 	LDI	*AR1,R1
    // asm 0000A8E6: 	ERRON	Z,EC_PROC|ET_DELETE|4	;LOCKUP ON END OF LIST FOUND"
    // asm 0000A8EE: 	BZ	KILL_X
    if (*linkp == NULL) {
        ERRON(EC_PROC | ET_DELETE | 4);
        goto KILL_X;
    }
    // asm 0000A8EF: 	CMPI	R1,AR2
    // asm 0000A8F0: 	BNE	KILLP
    if (*linkp != proc) {
        linkp = &(*linkp)->link;
        goto KILLP;
    }
    // asm 0000A8F1: 	LDI	*AR2,R1
    // asm 0000A8F2: 	STI	R1,*AR1			;LINK AROUND
    *linkp = proc->link;
    // asm 0000A8F3: 	LDI	@PFREEI,AR1
    // asm 0000A8F4: 	LDI	*AR1,R1
    // asm 0000A8F5: 	STI	R1,*AR2
    proc->link = PFREE;
    // asm 0000A8F6: 	STI	AR2,*AR1
    PFREE = proc;
    if (proc->ctx != NULL) {
        free(proc->ctx);
        proc->ctx = NULL;
    }
KILL_X:
#if DEBUG
    // asm: 	PUSH	R0
    // asm: 	LDI	@NUM_PROCS_ACTIVE,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@NUM_PROCS_ACTIVE
    NUM_PROCS_ACTIVE -= 1;
    // asm: 	LDI	@NUM_PROCS_IDLE,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@NUM_PROCS_IDLE
    NUM_PROCS_IDLE += 1;
    // asm: 	POP	R0
#endif
    // asm 0000A8F7: 	POP	AR1
    // asm 0000A8F8: 	POP	R1
    // asm 0000A8F9: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *KILLALL
 *KILL CLASS OF PROCESSES
 *
 *PARAMETERS
 *	R0	PID
 *	R1	MASK
 *
 */
void PRC_KILLALL(int pid, int mask) {
    PROC** linkp;
    PROC* proc;
    int masked_pid;

    // asm 0000A8FA: 	PUSH	AR1
    // asm 0000A8FB: 	PUSH	AR2
    // asm 0000A8FC: 	AND	R1,R0
    masked_pid = pid & mask;
    // asm 0000A8FD: 	LDI	@PACTIVEI,AR2		;WE MUST FIND DEAD PROCESS TO LINK AROUND
    linkp = &PACTIVE;
KILLALLP:
    // asm 0000A8FE: 	LDI	AR2,AR1
KLP0:
    // asm 0000A8FF: 	LDI	*AR1,R2
    // asm 0000A900: 	BZ	KADONE			;WE'RE DONE
    proc = *linkp;
    if (proc == NULL) {
        goto KADONE;
    }
    // asm 0000A901: 	LDI	R2,AR2
    // asm 0000A902: 	LDI	*+AR2(PID),R2
    // asm 0000A903: 	AND	R1,R2
    // asm 0000A904: 	CMPI	R2,R0
    // asm 0000A905: 	BNZ	KILLALLP
    if ((proc->id & mask) != masked_pid) {
        linkp = &proc->link;
        goto KILLALLP;
    }
    // asm 0000A906: 	CMPI	AR7,AR2
    // asm 0000A907: 	BZ	KILLALLP		;DONT KILL YOURSELF
    if (proc == CURRENT_PROC) {
        linkp = &proc->link;
        goto KILLALLP;
    }
    // asm 0000A908: 	LDI	*AR2,R2
    // asm 0000A909: 	STI	R2,*AR1			;LINK AROUND
    *linkp = proc->link;
    // asm 0000A90A: 	LDI	@PFREE,R2
    // asm 0000A90B: 	STI	R2,*AR2
    proc->link = PFREE;
    // asm 0000A90C: 	STI	AR2,@PFREE
    PFREE = proc;
#if DEBUG
    // asm: 	PUSH	R0
    // asm: 	LDI	@NUM_PROCS_ACTIVE,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@NUM_PROCS_ACTIVE
    NUM_PROCS_ACTIVE -= 1;
    // asm: 	LDI	@NUM_PROCS_IDLE,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@NUM_PROCS_IDLE
    NUM_PROCS_IDLE += 1;
    // asm: 	POP	R0
#endif
    // asm 0000A90D: 	BR	KLP0
    goto KLP0;
KADONE:
#if DEBUG
    // asm: 	CALL	PRC_DEBUG_CHECK
    PRC_DEBUG_CHECK();
#endif
    // asm 0000A90E: 	POP	AR2
    // asm 0000A90F: 	POP	AR1
    // asm 0000A910: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PRC_EXISTP	DOES A PROCESS EXIST
 *
 *PARAMETERS
 *	AR2	PID
 *	R2	MASK
 *RETURNS
 *	(IF FOUND)
 *		C=1 IF FOUND
 *		R0 POINTING TO PROCESS
 *	(IF NOT FOUND)
 *		C=0
 *		R0=0 IF PROCESS NOT FOUND
 *
 */
void PRC_EXISTP(void) {
    // asm 0000A911: 	CLRC				;CLEAR CARRY
    // asm 0000A912: 	PUSH	AR2
    // asm 0000A913: 	LDI	AR2,RC
    // asm 0000A914: 	AND	R2,RC
    // asm 0000A915: 	LDI	@PACTIVEI,AR2
EXPL:
    // asm 0000A916: 	LDI	*AR2,R0
    // asm 0000A917: 	BZ	EXDONE			;WE'RE DONE, ITS NOT HERE
    // asm 0000A918: 	LDI	R0,AR2
    // asm 0000A919: 	LDI	*+AR2(PID),RS
    // asm 0000A91A: 	AND	R2,RS
    // asm 0000A91B: 	CMPI	RC,RS
    // asm 0000A91C: 	BNZ	EXPL
    // asm 0000A91D: 	SETC				;WE FOUND IT DUDES!!!
EXDONE:
    // asm 0000A91E: 	POP	AR2
    // asm 0000A91F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_EXISTP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INITIALIZE PROCESS DATA STRUCTURES
 *
 */
void PRC_INIT(void) {
    PROC* proc;
    int i;

    // asm:
    // *INITIALIZE PROCESS DATA STRUCTURES

    // ZERO ACTIVE POINTER
    PACTIVE = NULL;

    // GET FREE POINTER
    proc = PRCSTR;
    PFREE = proc;

    for (i = 0; i < NUMPROC - 1; ++i) {
        proc->link = &PRCSTR[i + 1];
        proc++;
    }
    proc->link = NULL;

#if DEBUG
    NUM_PROCS_ACTIVE = 0;
    NUM_PROCS_IDLE = NUMPROC;
#endif
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *XFERPROC
 *
 *TRANSFER A PROCESS TO A DIFFERENT WAKEUP ADDRESS. WAKEUP WILL BE NEXT
 *POSSIBLE
 *
 *PARAMETERS
 *	AR0	POINTER TO PROCESS
 *	AR1	POINTER TO ADDRESS TO WAKE UP
 *
 */
void PRC_XFER(void) {
    // asm 0000A933: 	PUSH	AR1
#if DEBUG
    // asm: 	CMPI	AR0,AR7				;ARE WE ATTEMPTING TO XFER OURSELVES?
    // asm: 	SLOCKON	Z,"XFERPROC  ATTEMPT TO XFER OURSELF"
#endif
    // asm 0000A934: 	STI	AR1,*+AR0(PWAKE)		;SAVE WAKEUP ADDRESS
    // asm 0000A935: 	LDI	1,R0
    // asm 0000A936: 	STI	R0,*+AR0(PTIME)			;WAKEUP ASAP
    // 	;someday it might be nessesary to re-initialize the
    // 	;processes stack.  for now this is ignored.
    // asm 0000A937: 	POP	AR1
    // asm 0000A938: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_XFER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PRC_FIND
 *	FIND PROCESS FROM ACTIVE LIST
 *
 *PARAMETERS
 *	R0	PID
 *	R1	PID MASK
 *RETURNS
 *	AR0	POINTER TO PROCESS IF FOUND OR ZERO IF NONE FOUND
 *
 *
 *PRC_FINDNEXT
 *
 *
 *PARAMETERS
 *	AR0	POINTER TO LIST
 *
 */
void PRC_FINDNEXT(void) {
    // asm 0000A939: 	BUD	FINDE
    // asm 0000A93A: 	PUSH	R2
    // asm 0000A93B: 	AND	R1,R0
    // asm 0000A93C: 	NOP
    // 	;---->	BUD	FINDE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_FINDNEXT", 0, 0);
    UNIMPL();
}

PROC* PRC_FIND(int pid, int mask) {
    PROC* proc;
    int masked_pid;

    // asm 0000A93D: 	BUD	FINDE
    // asm 0000A93E: 	PUSH	R2
    // asm 0000A93F: 	AND	R1,R0
    masked_pid = pid & mask;
    // asm 0000A940: 	LDI	@PACTIVEI,AR0		;WE MUST FIND DEAD PROCESS TO LINK AROUND
    // 	;---->	BUD	FINDE
    proc = PACTIVE;
FINDLP:
    // asm 0000A941: 	CMPI	R2,R0
    // asm 0000A942: 	BEQ	FINDPROCX
FINDE:
    // asm 0000A943: LDI	*AR0,R2			;SET CONDITION CODE
    // asm 0000A944: 	BNZD	FINDLP
    // asm 0000A945: 	LDI	R2,AR0
    // asm 0000A946: 	LDI	*+AR0(PID),R2
    // asm 0000A947: 	AND	R1,R2
    // 	;---->	BNZD	FINDLP
    while (proc != NULL) {
        if ((proc->id & mask) == masked_pid) {
            break;
        }
        proc = proc->link;
    }
FINDPROCX:
    // asm 0000A948: 	POP	R2
    // asm 0000A949: 	RETS
    return proc;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SET THIS PROCESS TO FOLLOW SPECIFIED PROCESS
 *	(ASSERT ME AS A CHILD OF THE SPECIFIED PROCESS)
 *
 *PARAMETERS
 *	AR2	PROCESS TO FOLLOW
 *	AR7	!THIS PROCESS!
 *
 *
 */
void PRC_FOLLOW(void) {
    // asm 0000A94A: 	PUSH	R1
    // asm 0000A94B: 	PUSH	AR1
    // asm 0000A94C: 	PUSH	AR2
    // asm 0000A94D: 	PUSH	AR7
    // asm 0000A94E: 	LDI	@PACTIVEI,R1		;WE MUST FIND DEAD PROCESS TO LINK AROUND
PFOLLP:
    // asm 0000A94F: 	LDI	R1,AR1
    // asm 0000A950: 	LDI	*AR1,R1
    // asm 0000A951: 	ERRON	Z,EC_PROC|6		;LOCKUP ON END OF LIST FOUND
    // asm 0000A959: 	BZ	PROC_FOLLOW_X
    // asm 0000A95A: 	CMPI	R1,AR7
    // asm 0000A95B: 	BNE	PFOLLP
    // asm 0000A95C: 	LDI	*AR7,R1
    // asm 0000A95D: 	STI	R1,*AR1			;LINK AROUND
    // asm 0000A95E: 	LDI	*AR2,R1
    // asm 0000A95F: 	STI	R1,*AR7
    // asm 0000A960: 	STI	AR7,*AR2
PROC_FOLLOW_X:
    // asm 0000A961: 	POP	AR7
    // asm 0000A962: 	POP	AR2
    // asm 0000A963: 	POP	AR1
    // asm 0000A964: 	POP	R1
    // asm 0000A965: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRC_FOLLOW", 0, 0);
    UNIMPL();
}

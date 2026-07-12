#include "commq.h"

#include "../core/machine.h"
#include "cmos.h"
#include "comm.h"
#include "delta.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "racer.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/COMMQ.ASM
 */

void CLEAR_LINK(void);
void DECODE_BUFFER(void);
void COMMQ_PACKET_INIT(void);
void COMMQ_READY_TO_SEND(void);
void MESSAGE_ADD(void);
void MESSAGE_ADD_SB(int message /*AR2*/);
static void DECODE_NULL(void);
void SEND_WAVEFL_READY(void);
void SEND_WAVEFL_SET(void);
void SEND_WAVEFL_GO(void);
static void DECODE_WAVEFL_READY(void);
static void DECODE_WAVEFL_SET(void);
static void DECODE_WAVEFL_GO(void);
static void SEND_LINKEDT(void);
static void SEND_LINKEDF(void);
static void DECODE_LINKEDT(void);
static void DECODE_LINKEDF(void);
static void DECODE_MODE(void);
void SEND_CHECKPOINT(void);
static void SEND_FINISH(void);
void SEND_MODE(void);
static void DECODE_LINKCANCELLED(void);
void SEND_LINKCANCELLED(void);
void SEND_START_GAME(void);
static void DECODE_START_GAME(void);
void SEND_END_GAME(void);
static void DECODE_END_GAME(void);
void SEND_TIMECODE(void);
void SEND_RACENUM(void);
static void DECODE_RACENUM(void);
static void DECODE_VEHICLE(void);
void SEND_VEHICLE(void);
static void DECODE_TIMECODE(void);
void SEND_RHO_POS(void);
void SEND_RACER_POS(void);
void SEND_PLAYERS_POS(void);
void SEND_RHO_CREATE(void);
void FIND_DRONE(void);
#define DECODE_FLY_UPDATE DECODE_RHO_UPDATE
static void DECODE_RHO_UPDATE(void);
static void DECODE_RACER_UPDATE(void);
static void DECODE_CAR_UPDATE(void);
static void DECODE_RHO_CREATE(void);
static void DECODE_RACER_KILL(void);
static void DECODE_OM_TRACK(void);
void SEND_OM_TRACK(void);
#define SEND_BSYNC0 SEND_BSYNC3
#define SEND_BSYNC1 SEND_BSYNC3
#define SEND_BSYNC2 SEND_BSYNC3
void SEND_BSYNC3(void);
#define DECODE_BSYNC0 DECODE_BSYNC3
#define DECODE_BSYNC1 DECODE_BSYNC3
#define DECODE_BSYNC2 DECODE_BSYNC3
static void DECODE_BSYNC3(void);
static void DECODE_CHANGE_MUSIC(void);
void SEND_CHANGE_MUSIC(void);
static void DECODE_COINDROP(void);
void SEND_COINDROP(void);
static void DECODE_DIAGNOSTIC(void);
static void SEND_DIAGNOSTIC(void);
static void DECODE_ATTRSND(void);
void SEND_ATTRSND(void);

#define DECODE_FLY_UPDATE DECODE_RHO_UPDATE
#define SEND_BSYNC0 SEND_BSYNC3
#define SEND_BSYNC1 SEND_BSYNC3
#define SEND_BSYNC2 SEND_BSYNC3
#define DECODE_BSYNC0 DECODE_BSYNC3
#define DECODE_BSYNC1 DECODE_BSYNC3
#define DECODE_BSYNC2 DECODE_BSYNC3
#define SEND_BUFFER_AI SEND_BUFFER_A
#define COMMQ_TMP_BUFFI COMMQ_TMP_BUFF
#define DECODE_BLOCKI DECODE_BLOCK

void DECODE_RACER_XSFER(void);
void DECODE_RHO_KILL(void);
void DECODE_RHO_XSFER(void);
void DECODE_FLY_KILL(void);
void DECODE_FLY_XSFER(void);
void SEND_FLY_KILL(void);
extern int OM_TRACK_LO;
extern int OM_TRACK_HI;

static uintptr_t DECODE_BLOCK[];

/*
 *----------------------------------------------------------------------------
 *
 *
 *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

/* asm: IGNORE_UPDATES	.bss	IGNORE_UPDATES,1 */
int IGNORE_UPDATES;
/* asm: COMMQ_TMP_BUFF	fbss	COMMQ_TMP_BUFF,80 */
int COMMQ_TMP_BUFF[80];
/* asm: SEND_BUFFER_A	phibss	SEND_BUFFER_A,COMM_BUFFER_SIZE */
int SEND_BUFFER_A[COMM_BUFFER_SIZE];
/* asm: SEND_BUFFER_A_LEN	pbss	SEND_BUFFER_A_LEN,1 */
int SEND_BUFFER_A_LEN;
/*
        ;RECEIVE BUFFER
        ;
*/
/* asm: RBUFF_LEN	pbss	RBUFF_LEN,1 */
int RBUFF_LEN;
/* asm: RECEIVE_BUFFER	phibss	RECEIVE_BUFFER,COMM_BUFFER_SIZE */
int RECEIVE_BUFFER[COMM_BUFFER_SIZE];
/*
        ;OTHER MACHINE
        ;(OM_*)
        ;
*/
/* asm: OM_DIFF	pbss	OM_DIFF,1 */
float OM_DIFF;
/* asm: OM_MODE	pbss	OM_MODE,1 */
int OM_MODE;
/* asm: OM_CHOSEN_RACE	pbss	OM_CHOSEN_RACE,1 */
int OM_CHOSEN_RACE;
/* asm: OM_VEHICLE	pbss	OM_VEHICLE,1 */
int OM_VEHICLE;
/* asm: OM_STATE	pbss	OM_STATE,1 */
int OM_STATE;
/* asm: MY_STATE	pbss	MY_STATE,1 */
int MY_STATE;
/* asm: OM_ATTR_MODE	pbss	OM_ATTR_MODE,1 */
int OM_ATTR_MODE;
/* asm: OLD_OM_ATTR_MODE	pbss	OLD_OM_ATTR_MODE,1 */
int OLD_OM_ATTR_MODE;
/* asm: OM_LINKWAIT	pbss	OM_LINKWAIT,1 */
int OM_LINKWAIT;
/* asm: MY_LINKWAIT	.bss	MY_LINKWAIT,1 */
int MY_LINKWAIT;
/* asm: LINKEDP	pbss	LINKEDP,1 */
int LINKEDP;
/* asm: OM_HIDDEN_ON	pbss	OM_HIDDEN_ON,1 */
int OM_HIDDEN_ON;
/* asm: OM_RACE_MODE	pbss	OM_RACE_MODE,1 */
int OM_RACE_MODE;
/* asm: BSYNC	.bss	BSYNC,1 */
int BSYNC;
/* asm: OM_BSYNC	pbss	OM_BSYNC,1 */
int OM_BSYNC;
/* asm: OM_POSITION	pbss	OM_POSITION,1 */
int OM_POSITION;
/* asm: HEAD2HEAD_ON	pbss	HEAD2HEAD_ON,1 */
int HEAD2HEAD_ON;
/* asm: SAVED_PLY2CAR	.bss	SAVED_PLY2CAR,1 */
OBJ* SAVED_PLY2CAR;

void CLEAR_LINK(void) {
    int i;

    // asm:
    SAVED_PLY2CAR = PLY2CAR;

    MY_STATE = 0;
    OM_STATE = 0;
    OM_MODE = 0;
    HEAD2HEAD_ON = 0;
    MY_LINKWAIT = 0;
    OM_LINKWAIT = 0;
    PLY2CAR = 0;
    CAR_LIST = NULL;

    for (i = 0; i < 10; ++i) {
        RACER_PTR[i] = 0;
    }

    OM_CHOSEN_RACE = -1;
    OM_VEHICLE = -1;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 */
void DECODE_BUFFER(void) {
    // asm 0000767D: 	LDI	@DIPRAM,R0
    // asm 0000767E: 	TSTB	DIP_COMMP,R0 	       ;LINKED ?
    // asm 0000767F: 	RETSNZ			       ;NOPE...
    // asm 00007680: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 00007681: 	RETSZ
    // asm 00007682: 	LDI	@RBUFF_LEN,R0	       ;ANYTHING THERE ?
    // asm 00007683: 	RETSZ			       ;NOPE
    // asm 00007684: 	LDI	@RECEIVE_BUFFERI,AR2
    // asm 00007685: 	LDI	@RBUFF_LEN,AR6
    // asm 00007686: 	INC	AR6
    // asm 00007687: 	LS	1,AR6
    // asm 00007688: 	ADDI	AR2,AR6
    // asm 00007689: 	B	DECLPX
DECODE_LP:
    // asm 0000768A: 	LDI	*AR2++,AR0		;GET THE BLOCK ID
    // asm 0000768B: 	LS	8,AR0
    // asm 0000768C: 	RS	24,AR0			;SHIFT OFF THE CRAP
    // asm 0000768D: 	CMPI	CB_LASTMSG,AR0		;CHECK BOGUS MESSAGE
#if CDEBUG
    // asm: 	BGE	$			;TRAP ON BUGUS FOR DEBUG
#endif
    // asm 0000768E: 	BGE	ISDONE			;EXIT ON BOGUS
    // asm 0000768F: 	ADDI	@DECODE_BLOCKI,AR0
    // asm 00007690: 	LDI	*AR0,R0
    // asm 00007691: 	CALLU	R0
DECLPX:
    // asm 00007692: 	CMPI	AR6,AR2
    // asm 00007693: 	BLT	DECODE_LP
ISDONE:
    // asm 00007694: 	CLRI	R0
    // asm 00007695: 	STI	R0,@RBUFF_LEN
    // asm 00007696: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_BUFFER", 0, 0);
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *	.word	ID,ROUTINE TO DECODE
 *
 *
 *Each decode routine can assume:
 *	  AR2 is the pointer to the data block
 *	  AR6,DP registers must be saved/restored
 *	  AR2 should be incremented to the next block
 *
 *
 */
/* asm: DECODE_BLOCKI	.word	DECODE_BLOCK */
#define DECODE_BLOCKI DECODE_BLOCK
/* asm: DECODE_BLOCK */
/* asm: 	.word	DECODE_NULL */
/* asm: 	.word	DECODE_MODE		;CB_MODE */
/* asm: 	.word	DECODE_TIMECODE		;CB_TIMECODE */
/* asm: 	.word	DECODE_RACENUM		;CB_RACENUM */
/* asm: 	.word	DECODE_VEHICLE		;CHOSEN CAR IDENTITY CODE */
/* asm: 	.word	DECODE_START_GAME */
/* asm: 	.word	DECODE_END_GAME */
/* asm: 	.word	DECODE_CAR_UPDATE	;UPDATE DATA BLOCK FOR CAR */
/* asm: 	.word	DECODE_END_GAME */
/* asm: 	.word	DECODE_LINKEDT */
/* asm: 	.word	DECODE_LINKEDF */
/* asm: 	.word	DECODE_LINKCANCELLED */
/* asm: 	 */
/* asm: 	.word	DECODE_RACER_UPDATE */
/* asm: 	.word	DECODE_RACER_XSFER */
/* asm: 	.word	DECODE_RACER_KILL */
/* asm: 	 */
/* asm: 	.word	DECODE_RHO_CREATE */
/* asm: 	.word	DECODE_RHO_KILL */
/* asm: 	.word	DECODE_RHO_XSFER */
/* asm: 	.word	DECODE_RHO_UPDATE */
/* asm: 	 */
/* asm: 	.word	DECODE_FLY_KILL */
/* asm: 	.word	DECODE_FLY_XSFER */
/* asm: 	.word	DECODE_FLY_UPDATE */
/* asm: 	 */
/* asm: 	.word	DECODE_WAVEFL_READY */
/* asm: 	.word	DECODE_WAVEFL_SET */
/* asm: 	.word	DECODE_WAVEFL_GO */
/* asm: 	 */
/* asm: 	.word	DECODE_OM_TRACK */
/* asm: 	 */
/* asm: 	.word	DECODE_BSYNC0 */
/* asm: 	.word	DECODE_BSYNC1 */
/* asm: 	.word	DECODE_BSYNC2 */
/* asm: 	.word	DECODE_BSYNC3 */
/* asm: 	 */
/* asm: 	.word	DECODE_CHANGE_MUSIC */
/* asm: 	.word	DECODE_COINDROP */
/* asm: 	.word	DECODE_DIAGNOSTIC */
/* asm: 	 */
/* asm: 	.word	DECODE_ATTRSND */
/* asm: 	 */
/* asm: 	.word	-1 */
/* asm: 	 */
static uintptr_t DECODE_BLOCK[] = {
    (uintptr_t)DECODE_NULL,
    (uintptr_t)DECODE_MODE,     // CB_MODE
    (uintptr_t)DECODE_TIMECODE, // CB_TIMECODE
    (uintptr_t)DECODE_RACENUM,  // CB_RACENUM
    (uintptr_t)DECODE_VEHICLE,  // CHOSEN CAR IDENTITY CODE
    (uintptr_t)DECODE_START_GAME,
    (uintptr_t)DECODE_END_GAME,
    (uintptr_t)DECODE_CAR_UPDATE, // UPDATE DATA BLOCK FOR CAR
    (uintptr_t)DECODE_END_GAME,
    (uintptr_t)DECODE_LINKEDT,
    (uintptr_t)DECODE_LINKEDF,
    (uintptr_t)DECODE_LINKCANCELLED,
    (uintptr_t)DECODE_RACER_UPDATE,
    (uintptr_t)DECODE_RACER_XSFER,
    (uintptr_t)DECODE_RACER_KILL,
    (uintptr_t)DECODE_RHO_CREATE,
    (uintptr_t)DECODE_RHO_KILL,
    (uintptr_t)DECODE_RHO_XSFER,
    (uintptr_t)DECODE_RHO_UPDATE,
    (uintptr_t)DECODE_FLY_KILL,
    (uintptr_t)DECODE_FLY_XSFER,
    (uintptr_t)DECODE_FLY_UPDATE,
    (uintptr_t)DECODE_WAVEFL_READY,
    (uintptr_t)DECODE_WAVEFL_SET,
    (uintptr_t)DECODE_WAVEFL_GO,
    (uintptr_t)DECODE_OM_TRACK,
    (uintptr_t)DECODE_BSYNC0,
    (uintptr_t)DECODE_BSYNC1,
    (uintptr_t)DECODE_BSYNC2,
    (uintptr_t)DECODE_BSYNC3,
    (uintptr_t)DECODE_CHANGE_MUSIC,
    (uintptr_t)DECODE_COINDROP,
    (uintptr_t)DECODE_DIAGNOSTIC,
    (uintptr_t)DECODE_ATTRSND,
    -1,
};

/*
 *----------------------------------------------------------------------------
 *
 *SETUP SEND BUFFER POINTERS
 *
 */
void COMMQ_PACKET_INIT(void) {
    // asm 000076BB: 	PUSH	R0
    // ;	LDI	@SEND_BUFFER_AI,R0
    // ;	STI	R0,@FILLBUFF_PTR
    // ;	LDL	SEND_BUFFER_A_LEN,R0
    // ;	STI	R0,@FILLBUFF_LEN_PTR
    // asm 000076BC: 	CLRI	R0
    // asm 000076BD: 	STI	R0,@SEND_BUFFER_A_LEN
    // asm 000076BE: CMQIX
    // asm 000076BE: 	CALL	SEND_MODE
    // asm 000076BF: 	POP	R0
    // asm 000076C0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMMQ_PACKET_INIT", 0, 0);
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *FOR MASTER, HE WILL SEND IMMEDIATELY FOLLOWING,
 *FOR SLAVE, HE WILL SWITCH BUFFERS, AND WHEN REQUESTED SEND THIS BUFFER
 *IF THE NEXT BUFFER IF FULL, SLAVE WILL OVERWRITE THIS BUFFER, AND SO ON
 *
 *
 */
void COMMQ_READY_TO_SEND(void) {
    // asm 000076C1: 	PUSH	R0
    // asm 000076C2: 	PUSH	AR0
    // asm 000076C3: 	PUSH	AR2
    // 	;PAD THE END OF THE MESSAGES
    // asm 000076C4: 	LDI	CB_NULL,AR2
    // asm 000076C5: 	CALL	MESSAGE_ADD_SB
    // asm 000076C6: 	POP	AR2
    // asm 000076C7: 	POP	AR0
    // asm 000076C8: 	POP	R0
    // asm 000076C9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMMQ_READY_TO_SEND", 0, 0);
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *REMEMBER ALSO THAT THESE ARE BYTE STRINGS (LOWER 8 BITS ONLY)
 *
 *
 *
 *PARAMETERS
 *	RC	(LENGTH OF BUFFER)-1
 *	AR2	PTR TO MESSAGE STRING (BUFFER)
 *
 *	(REMEMBER THAT THE FIRST WORD OF THE MESSAGE SHOULD BE THE
 *	BLOCK HEADER)
 *
 */
void MESSAGE_ADD(void) {
    // asm 000076CA: 	PUSH	AR0
    // asm 000076CB: 	PUSH	AR2
    // asm 000076CC: 	PUSH	AR3
    // asm 000076CD: 	PUSH	R0
    // asm 000076CE: 	LDI	@DIPRAM,R0
    // asm 000076CF: 	TSTB	DIP_COMMP,R0
    // asm 000076D0: 	BNZ	MESSADDX
    // asm 000076D1: 	LDI	RC,R0
    // asm 000076D2: 	LDI	@SEND_BUFFER_AI,AR3
    // asm 000076D3: 	ADDI	@SEND_BUFFER_A_LEN,AR3
    // asm 000076D4: 	ADDI	@SEND_BUFFER_A_LEN,R0
    // ;	LDI	@FILLBUFF_LEN_PTR,AR0
    // ;	LDI	@FILLBUFF_PTR,AR3
    // ;	ADDI	*AR0,AR3		;start of new message
    // ;	ADDI	*AR0,R0
    // asm 000076D5: 	INC	R0			;->(Length - 1)
    // asm 000076D6: 	CMPI	COMM_BUFFER_SIZE,R0
#if CDEBUG
    // asm: 	BGT	$
#endif
    // asm 000076D7: 	BGT	MESSADDX
    // ;	STI	R0,*AR0			;length
    // asm 000076D8: 	STI	R0,@SEND_BUFFER_A_LEN
    // asm 000076D9: 	RPTB	JJAG
    // asm 000076DA: 	LDI	*AR2++,R0
JJAG:
    // asm 000076DB: STI	R0,*AR3++
MESSADDX:
    // asm 000076DC: 	POP	R0
    // asm 000076DD: 	POP	AR3
    // asm 000076DE: 	POP	AR2
    // asm 000076DF: 	POP	AR0
    // asm 000076E0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MESSAGE_ADD", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *MESSAGE ADD, SINGLE BYTE
 *
 *PARAMETERS
 *	AR2	MESSAGE
 *
 */
void MESSAGE_ADD_SB(int message /*AR2*/) {
    // asm 000076E1: 	PUSH	R0
    // asm 000076E2: 	PUSH	AR0
    // asm 000076E3: 	LDI	@DIPRAM,R0
    // asm 000076E4: 	TSTB	DIP_COMMP,R0
    // asm 000076E5: 	BNZ	MASBX
    // asm 000076E6: 	LDI	@SEND_BUFFER_A_LEN,R0
    // ;	LDI	@FILLBUFF_LEN_PTR,AR0
    // ;	LDI	*AR0,R0
    // asm 000076E7: 	INC	R0
    // asm 000076E8: 	CMPI	COMM_BUFFER_SIZE,R0
#if CDEBUG
    // asm: 	BGT	$
#endif
    // asm 000076E9: 	BGT	MASBX
    // ;	STI	R0,*AR0
    // asm 000076EA: 	STI	R0,@SEND_BUFFER_A_LEN
    // ;	LDI	*AR0,AR0
    // ;	ADDI	@FILLBUFF_PTR,AR0
    // asm 000076EB: 	LDI	@SEND_BUFFER_AI,AR0
    // asm 000076EC: 	ADDI	R0,AR0
    // asm 000076ED: 	STI	AR2,*-AR0(1)
MASBX:
    // asm 000076EE: 	POP	AR0
    // asm 000076EF: 	POP	R0
    // asm 000076F0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MESSAGE_ADD_SB", 0, 0);

    if ((DIPRAM & DIP_COMMP) == 0) {
        int length = SEND_BUFFER_A_LEN + 1;

        if (length <= COMM_BUFFER_SIZE) {
            SEND_BUFFER_A_LEN = length;
            SEND_BUFFER_A[length - 1] = message;
        }
    }
}

// *----------------------------------------------------------------------------

static void DECODE_NULL(void) {
    // asm 000076F1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_NULL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_WAVEFL_READY(void) {
    // asm 000076F2: 	LDI	CB_WAVEFL_READY,AR2
    // asm 000076F3: 	BR	MESSAGE_ADD_SB
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_WAVEFL_READY", 0, 0);
    MESSAGE_ADD_SB(CB_WAVEFL_READY);
}

void SEND_WAVEFL_SET(void) {
    // asm 000076F4: 	LDI	CB_WAVEFL_SET,AR2
    // asm 000076F5: 	BR	MESSAGE_ADD_SB
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_WAVEFL_SET", 0, 0);
    MESSAGE_ADD_SB(CB_WAVEFL_SET);
}

void SEND_WAVEFL_GO(void) {
    // asm 000076F6: 	CLRI	R0
    // asm 000076F7: 	STI	R0,@_sectime
    // asm 000076F8: 	LDI	CB_WAVEFL_GO,AR2
    // asm 000076F9: 	BR	MESSAGE_ADD_SB
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_WAVEFL_GO", 0, 0);
    _sectime = 0;
    MESSAGE_ADD_SB(CB_WAVEFL_GO);
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_WAVEFL_READY(void) {
    // asm 000076FA: 	LDI	1,R0
    // asm 000076FB: 	STI	R0,@H2H_FLAGSTATE
    // asm 000076FC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_WAVEFL_READY", 0, 0);
    UNIMPL();
}

static void DECODE_WAVEFL_SET(void) {
    // asm 000076FD: 	LDI	2,R0
    // asm 000076FE: 	STI	R0,@H2H_FLAGSTATE
    // asm 000076FF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_WAVEFL_SET", 0, 0);
    UNIMPL();
}

static void DECODE_WAVEFL_GO(void) {
    // asm 00007700: 	LDI	3,R0
    // asm 00007701: 	STI	R0,@H2H_FLAGSTATE
    // asm 00007702: 	CLRI	R0
    // asm 00007703: 	STI	R0,@_sectime
    // asm 00007704: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_WAVEFL_GO", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void SEND_LINKEDT(void) {
    // asm 00007705: 	PUSH	AR2
    // asm 00007706: 	LDI	CB_HEAD2HEADT,AR2
    // asm 00007707: 	CALL	MESSAGE_ADD_SB
    // asm 00007708: 	POP	AR2
    // asm 00007709: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_LINKEDT", 0, 0);
    UNIMPL();
}

static void SEND_LINKEDF(void) {
    // asm 0000770A: 	PUSH	AR2
    // asm 0000770B: 	LDI	CB_HEAD2HEADF,AR2
    // asm 0000770C: 	CALL	MESSAGE_ADD_SB
    // asm 0000770D: 	POP	AR2
    // asm 0000770E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_LINKEDF", 0, 0);
    UNIMPL();
}

static void DECODE_LINKEDT(void) {
    // asm 0000770F: 	PUSH	R0
    // asm 00007710: 	LDI	1,R0
    // asm 00007711: 	STI	R0,@LINKEDP
    // asm 00007712: 	POP	R0
    // asm 00007713: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_LINKEDT", 0, 0);
    UNIMPL();
}

static void DECODE_LINKEDF(void) {
    // asm 00007714: 	PUSH	R0
    // asm 00007715: 	LDI	2,R0
    // asm 00007716: 	STI	R0,@LINKEDP
    // asm 00007717: 	POP	R0
    // asm 00007718: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_LINKEDF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_MODE(void) {
    // asm 00007719: 	PUSH	R0
    // asm 0000771A: 	PUSH	R1
    // asm 0000771B: 	PUSH	R2
    // asm 0000771C: 	LDI	0FFh,R2
    // asm 0000771D: 	LS	16,R2
    // asm 0000771E: 	AND	*AR2++,R2,R0
    // asm 0000771F: 	RS	16,R0
    // asm 00007720: 	AND	*AR2++,R2,R1
    // asm 00007721: 	RS	8,R1
    // asm 00007722: 	OR	R1,R0
    // asm 00007723: 	AND	*AR2++,R2,R1
    // asm 00007724: 	OR	R1,R0
    // asm 00007725: 	AND	*AR2++,R2,R1
    // asm 00007726: 	LS	8,R1
    // asm 00007727: 	OR	R1,R0
    // asm 00007728: 	STI	R0,@OM_MODE
    // 	;the state
    // asm 00007729: 	AND	*AR2++,R2,R0
    // asm 0000772A: 	RS	16,R0
    // asm 0000772B: 	AND	*AR2++,R2,R1
    // asm 0000772C: 	LS	8,R1
    // asm 0000772D: 	RS	16,R1
    // asm 0000772E: 	OR	R1,R0
    // asm 0000772F: 	STI	R0,@OM_STATE
    // asm 00007730: 	AND	*AR2++,R2,R0
    // asm 00007731: 	RS	16,R0
    // asm 00007732: 	STI	R0,@OM_BONUS_WAITFLAG
    // 	;POSITION
    // asm 00007733: 	AND	*AR2++,R2,R0
    // asm 00007734: 	RS	16,R0
    // asm 00007735: 	STI	R0,@OM_POSITION
    // 	;the attract mode index
    // asm 00007736: 	AND	*AR2++,R2,R0
    // asm 00007737: 	LS	8,R0
    // asm 00007738: 	ASH	-24,R0
    // asm 00007739: 	LDI	@OM_ATTR_MODE,R1
    // asm 0000773A: 	STI	R1,@OLD_OM_ATTR_MODE
    // asm 0000773B: 	STI	R0,@OM_ATTR_MODE
    // 	;DECODE  OM_DIFF
    // asm 0000773C: 	AND	*AR2++,R2,R0
    // asm 0000773D: 	LSH	-16,R0
    // asm 0000773E: 	FLOAT	R0
    // asm 0000773F: 	MPYF	0.01,R0
    // asm 00007740: 	STF	R0,@OM_DIFF
    // asm 00007741: 	LDI	@OM_MODE,R0
    // asm 00007742: 	AND	MMODE,R0
    // asm 00007743: 	CMPI	MATTR,R0
    // asm 00007744: 	BNE	DC_M_NOP
    // asm 00007745: 	LDI	0,R0
    // asm 00007746: 	STI	R0,@HEAD2HEAD_ON
    // asm 00007747: 	LDI	-1,R0
    // asm 00007748: 	STI	R0,@OM_CHOSEN_RACE
    // asm 00007749: 	STI	R0,@OM_VEHICLE
    // asm 0000774A: 	BU	DCMX
DC_M_NOP:
    // asm 0000774B: 	LDI	@OM_STATE,R0
    // asm 0000774C: 	TSTB	OMS_FINISHLINE,R0
    // asm 0000774D: 	BZ	DWAI
    // asm 0000774E: 	LDI	@MY_STATE,R1
    // asm 0000774F: 	TSTB	OMS_FINISHLINE,R1
    // asm 00007750: 	BNZ	DWAI
    // 	;else: Hes finished, Im not
    // 	;therefore:  0 -> countdown
    // ;	LDI	1,R1
    // ;	STI	R1,@I_TIMED_OUT
    // asm 00007751: 	CLRI	R1
    // asm 00007752: 	STI	R1,@_countdown
    // asm 00007753: 	BU	DCMX
DWAI:
    // asm 00007754: 	LDI	@OM_MODE,R0
    // asm 00007755: 	AND	MMODE,R0
    // asm 00007756: 	CMPI	MGAME,R0	;OM in game?
    // asm 00007757: 	BNE	DCMX
    // asm 00007758: 	LDI	@_MODE,R0	;ME in game?
    // asm 00007759: 	AND	MMODE,R0
    // asm 0000775A: 	CMPI	MGAME,R0
    // asm 0000775B: 	BNE	NONEWCP
    // asm 0000775C: 	LDI	@OM_STATE,R0
    // asm 0000775D: 	AND	OMS_CPOINT_M,R0
    // asm 0000775E: 	LDI	@MY_STATE,R1
    // asm 0000775F: 	AND	OMS_CPOINT_M,R1
    // asm 00007760: 	CMPI	R1,R0
    // asm 00007761: 	BEQ	NONEWCP
    // asm 00007762: 	CMPI	R0,R1		;is my CP cnt > his CP cnt?
    // asm 00007763: 	CALLLT	CHECKPOINT_HIT_R
NONEWCP:
DCMX:
    // asm 00007764: 	POP	R2
    // asm 00007765: 	POP	R1
    // asm 00007766: 	POP	R0
    // asm 00007767: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_MODE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *turn on the checkpoint bit within the MY_STATE
 *does not actually send data over the comm lines.
 *
 *
 */
void SEND_CHECKPOINT(void) {
    // asm 00007768: 	LDI	@CHECKPOINT_NUM,R0
    // asm 00007769: 	LDI	1,R1
    // asm 0000776A: 	LS	R0,R1
    // asm 0000776B: 	LS	8,R1
    // asm 0000776C: 	OR	@MY_STATE,R1
    // asm 0000776D: 	STI	R1,@MY_STATE
    // asm 0000776E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_CHECKPOINT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void SEND_FINISH(void) {
    // asm 0000776F: 	LDI	OMS_FINISHLINE,R0
    // asm 00007770: 	OR	@MY_STATE,R0
    // asm 00007771: 	STI	R0,@MY_STATE
    // asm 00007772: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_FINISH", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_MODE(void) {
    // asm 00007773: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00007774: 	LDI	CB_MODE,R0
    // asm 00007775: 	STI	R0,*AR2++
    // asm 00007776: 	LDI	@_MODE,R0
    // asm 00007777: 	STI	R0,*AR2++
    // asm 00007778: 	RS	8,R0
    // asm 00007779: 	STI	R0,*AR2++
    // asm 0000777A: 	RS	8,R0
    // asm 0000777B: 	STI	R0,*AR2++
    // asm 0000777C: 	RS	8,R0
    // asm 0000777D: 	STI	R0,*AR2++
    // asm 0000777E: 	LDI	@MY_STATE,R0
    // asm 0000777F: 	STI	R0,*AR2++
    // asm 00007780: 	RS	8,R0
    // asm 00007781: 	STI	R0,*AR2++
    // asm 00007782: 	LDI	@BONUS_WAITFLAG,R0
    // asm 00007783: 	STI	R0,*AR2++
    // asm 00007784: 	LDI	@POSITION,R0
    // asm 00007785: 	STI	R0,*AR2++
    // asm 00007786: 	LDI	@_ATTR_MODE,R0
    // asm 00007787: 	STI	R0,*AR2++
    // asm 00007788: 	LDF	@GAMEDIFF,R0 		;SEND DIFFICULTY
    // asm 00007789: 	MPYF	100,R0
    // asm 0000778A: 	FIX	R0
    // asm 0000778B: 	STI	R0,*AR2--(10)
    // asm 0000778C: 	LDI	11-1,RC
    // asm 0000778D: 	BR	MESSAGE_ADD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_MODE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_LINKCANCELLED(void) {
    // asm 0000778E: 	CLRI	R0
    // asm 0000778F: 	STI	R0,@OM_LINKWAIT
    // asm 00007790: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_LINKCANCELLED", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_LINKCANCELLED(void) {
    // asm 00007791: 	LDI	CB_LINKCANCELLED,AR2
    // asm 00007792: 	BR	MESSAGE_ADD_SB
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_LINKCANCELLED", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 * T 	I BOUGHT IN
 *
 *
 *
 */
void SEND_START_GAME(void) {
    // asm 00007793: 	CLRI	R0
    // asm 00007794: 	STI	R0,@IGNORE_UPDATES
    // asm 00007795: 	LDI	CB_START_GAME,AR2
    // asm 00007796: 	CALL	MESSAGE_ADD_SB
    // asm 00007797: 	LDI	0,R0
    // asm 00007798: 	STI	R0,@MY_STATE
    // asm 00007799: 	STI	R0,@HEAD2HEAD_ON
    // asm 0000779A: 	LDI	1,R0
    // asm 0000779B: 	STI	R0,@MY_LINKWAIT
    // asm 0000779C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_START_GAME", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_START_GAME(void) {
    // asm 0000779D: 	LDI	1,R0
    // asm 0000779E: 	STI	R0,@OM_LINKWAIT
    // asm 0000779F: 	LDI	@MY_LINKWAIT,R0
    // asm 000077A0: 	BZ	NOPE
    // asm 000077A1: 	LDI	1,R0
    // asm 000077A2: 	STI	R0,@HEAD2HEAD_ON
    // asm 000077A3: 	INCAUD	AUD_H2HGAMES
    // asm 000077A5: 	CLRI	R0
    // asm 000077A6: 	STI	R0,@OM_LINKWAIT
    // asm 000077A7: 	STI	R0,@MY_LINKWAIT
    // asm 000077A8: 	CALL	SEND_LINKEDT
    // asm 000077A9: 	LDI	-1,R0
    // asm 000077AA: 	STI	R0,@OM_CHOSEN_RACE
    // asm 000077AB: 	STI	R0,@OM_VEHICLE
    // asm 000077AC: 	PUSH	AR2
    // asm 000077AD: 	SOND1	START_THEME
    // asm 000077AF: 	POP	AR2
    // asm 000077B0: 	LDI	@RACE_MODE,R0
    // asm 000077B1: 	CMPI	RM_USA,R0
    // asm 000077B2: 	RETSNE
    // asm 000077B3: 	CALL	SEND_RACENUM
    // asm 000077B4: 	RETS
NOPE:
    // asm 000077B5: 	CALL	SEND_LINKEDF
    // asm 000077B6: 	LDI	-1,R0
    // asm 000077B7: 	STI	R0,@OM_CHOSEN_RACE
    // asm 000077B8: 	STI	R0,@OM_VEHICLE
    // asm 000077B9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_START_GAME", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_END_GAME(void) {
    // asm 000077BA: 	LDI	CB_END_GAME,AR2
    // asm 000077BB: 	BR	MESSAGE_ADD_SB
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_END_GAME", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *clear the variables associated with the game...
 */
static void DECODE_END_GAME(void) {
    // asm 000077BC: 	LDI	0,R0
    // asm 000077BD: 	STI	R0,@HEAD2HEAD_ON
    // asm 000077BE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_END_GAME", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_TIMECODE(void) {
    // asm 000077BF: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 000077C0: 	LDI	CB_TIMECODE,R0
    // asm 000077C1: 	STI	R0,*AR2++
    // asm 000077C2: 	LDI	@_countdown,R0
    // asm 000077C3: 	STI	R0,*AR2++
    // asm 000077C4: 	RS	8,R0
    // asm 000077C5: 	STI	R0,*AR2++
    // asm 000077C6: 	LDI	@_sectime,R0
    // asm 000077C7: 	STI	R0,*AR2--(3)
    // asm 000077C8: 	LDI	4-1,RC
    // asm 000077C9: 	BR	MESSAGE_ADD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_TIMECODE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_RACENUM(void) {
    // asm 000077CA: 	LDL	COMMQ_TMP_BUFF,AR2
    // asm 000077CB: 	LDI	CB_RACENUM,R0
    // asm 000077CC: 	STI	R0,*AR2++
    // asm 000077CD: 	LDI	@HIDDEN_ON,R0
    // asm 000077CE: 	STI	R0,*AR2++
    // asm 000077CF: 	LDI	@RACE_MODE,R0
    // asm 000077D0: 	STI	R0,*AR2++
    // asm 000077D1: 	LDI	@CHOSEN_RACE,R0
    // asm 000077D2: 	STI	R0,*AR2--(3)
    // asm 000077D3: 	LDI	4-1,RC
    // asm 000077D4: 	BR	MESSAGE_ADD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RACENUM", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_RACENUM(void) {
    // asm 000077D5: 	PUSH	R1
    // asm 000077D6: 	LDI	*AR2++,R1
    // asm 000077D7: 	LS	8,R1
    // asm 000077D8: 	ASH	-24,R1
    // asm 000077D9: 	STI	R1,@OM_HIDDEN_ON
    // asm 000077DA: 	LDI	*AR2++,R0
    // asm 000077DB: 	LS	8,R0
    // asm 000077DC: 	RS	24,R0
    // asm 000077DD: 	STI	R0,@OM_RACE_MODE
    // asm 000077DE: 	LDI	*AR2++,R0
    // asm 000077DF: 	LS	8,R0
    // asm 000077E0: 	RS	24,R0
    // asm 000077E1: 	STI	R0,@OM_CHOSEN_RACE
    // asm 000077E2: NOTHIDDEN
    // asm 000077E2: 	POP	R1
    // asm 000077E3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RACENUM", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_VEHICLE(void) {
    // asm 000077E4: 	LDI	*AR2++,R0
    // asm 000077E5: 	LS	8,R0
    // asm 000077E6: 	RS	24,R0
    // asm 000077E7: 	STI	R0,@OM_VEHICLE
    // asm 000077E8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_VEHICLE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_VEHICLE(void) {
    // asm 000077E9: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 000077EA: 	LDI	CB_VEHICLE,R0
    // asm 000077EB: 	STI	R0,*AR2++
    // asm 000077EC: 	LDI	@CHOOSENCAR,R0	;contains extended vehicles
    // asm 000077ED: 	STI	R0,*AR2--
    // asm 000077EE: 	LDI	2-1,RC
    // asm 000077EF: 	BR	MESSAGE_ADD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_VEHICLE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_TIMECODE(void) {
    // asm 000077F0: 	PUSH	R0
    // asm 000077F1: 	PUSH	R1
    // asm 000077F2: 	PUSH	R2
    // asm 000077F3: 	LDI	0FFh,R2
    // asm 000077F4: 	LS	16,R2
    // asm 000077F5: 	AND	*AR2++,R2,R0
    // asm 000077F6: 	RS	16,R0
    // asm 000077F7: 	AND	*AR2++,R2,R1
    // asm 000077F8: 	RS	8,R1
    // asm 000077F9: 	OR	R1,R0
    // asm 000077FA: 	STI	R0,@_countdown
    // asm 000077FB: 	AND	*AR2++,R2,R0
    // asm 000077FC: 	STI	R0,@_sectime
    // asm 000077FD: 	POP	R2
    // asm 000077FE: 	POP	R1
    // asm 000077FF: 	POP	R0
    // asm 00007800: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_TIMECODE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void SEND_RHO_POS(void) {
    // asm 00007801: 	LDI	*+AR7(DELTA_LAST_OID),R0
    // asm 00007802: 	STI	R0,*+AR5(CARTRACK_ID)  	;SAVE TRACK ID
    // asm 00007803: 	LDI	CB_RHO_UPDATE,R0	;GET MESSAGE HEADER
    // asm 00007804: 	LDI	AR4,AR0	    		;GET OBJECT IN AR0
    // asm 00007805: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00007806: 	STI	R0,*AR2++
    // *SEND CAR ID #
    // asm 00007807: 	LDI	*+AR5(CARNUM),R0
    // asm 00007808: 	B	SEND_CP0
    // *
    // *AR4=CAR OBJECT
    // *AR5=CAR BLOCK
    // *AR7=RACER PROCESS
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RHO_POS", 0, 0);
    UNIMPL();
}

void SEND_RACER_POS(void) {
    // asm 00007809: 	LDI	*+AR7(DELTA_LAST_OID),R0
    // asm 0000780A: 	STI	R0,*+AR5(CARTRACK_ID)  	;SAVE TRACK ID
    // asm 0000780B: 	LDI	CB_RACER_UPDATE,R0	;GET MESSAGE HEADER
    // asm 0000780C: 	B	SEND_CAR_POS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RACER_POS", 0, 0);
    UNIMPL();
}

void SEND_PLAYERS_POS(void) {
    // *
    // *AR4=PLAYER CAR OBJECT
    // *AR7=PLAYER PROCESS
    // *
    // asm 0000780D: 	LDI	CB_CAR_UPDATE,R0	;GET MESSAGE HEADER
SEND_CAR_POS:
    // *SEND MESSAGE HEADER
    // asm 0000780E: 	LDI	AR4,AR0	    		;GET OBJECT IN AR0
    // asm 0000780F: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00007810: 	STI	R0,*AR2++
    // *SEND CAR ID #
    // asm 00007811: 	LDI	*+AR7(DELTA_INIT),R0
SEND_CP0:
    // asm 00007812: 	STI	R0,*AR2++
    // *SEND XYZ
    // asm 00007813: 	ADDI	OPOSX,AR0
    // asm 00007814: 	LDI	2,RC
    // asm 00007815: 	RPTB	SENDXYZ			;GET XYZ COORD
    // asm 00007816: 	FIX	*AR0++,R0		;CONVERT TO 32 BIT FIXED
    // asm 00007817: 	STI	R0,*AR2++
    // asm 00007818: 	LSH	-8,R0
    // asm 00007819: 	STI	R0,*AR2++
    // asm 0000781A: 	LSH	-8,R0
    // asm 0000781B: 	STI	R0,*AR2++
    // asm 0000781C: 	LSH	-8,R0
SENDXYZ:
    // asm 0000781D: STI	R0,*AR2++
    // *SEND MATRIX
    // asm 0000781E: 	LDI	AR4,AR0	    		;GET OBJECT IN AR0
    // asm 0000781F: 	ADDI	OMATRIX,AR0
    // asm 00007820: 	FLOAT	32000,R1
    // asm 00007821: 	LDI	8,RC			;DO 9 MATRIX ENTRIES
    // asm 00007822: 	RPTB	MATLP
    // asm 00007823: 	MPYF	*AR0++,R1,R0
    // asm 00007824: 	FIX	R0
    // asm 00007825: 	STI	R0,*AR2++
    // asm 00007826: 	LSH	-8,R0
MATLP:
    // asm 00007827: STI	R0,*AR2++
    // *SEND CARYROT
    // asm 00007828: 	LDI	AR4,AR0	    		;GET OBJECT IN AR0
    // asm 00007829: 	LDI	*+AR0(OCARBLK),AR1
    // asm 0000782A: 	LDF	*+AR1(CARYROT),R2
    // asm 0000782B: 	CALL	NORMITS
    // asm 0000782C: 	FLOAT	1000,R1
    // asm 0000782D: 	MPYF	R1,R2
    // asm 0000782E: 	FIX	R2
    // asm 0000782F: 	STI	R2,*AR2++
    // asm 00007830: 	LSH	-8,R2
    // asm 00007831: 	STI	R2,*AR2++
    // *SEND CAR STRUCTURE STUFF
    // *SEND CARTURN
    // asm 00007832: 	LDF	*+AR1(CARTURN),R2
    // asm 00007833: 	CALL	NORMITS
    // asm 00007834: 	FLOAT	1000,R1
    // asm 00007835: 	MPYF	R1,R2
    // asm 00007836: 	FIX	R2
    // asm 00007837: 	STI	R2,*AR2++
    // asm 00007838: 	LSH	-8,R2
    // asm 00007839: 	STI	R2,*AR2++
    // *SEND CARVROT
    // asm 0000783A: 	LDF	*+AR1(CARVROT),R2
    // asm 0000783B: 	CALL	NORMITS
    // asm 0000783C: 	FLOAT	1000,R1
    // asm 0000783D: 	MPYF	R1,R2
    // asm 0000783E: 	FIX	R2
    // asm 0000783F: 	STI	R2,*AR2++
    // asm 00007840: 	LSH	-8,R2
    // asm 00007841: 	STI	R2,*AR2++
    // *SEND CARSPEED
    // asm 00007842: 	FIX	*+AR1(CARSPEED),R2
    // asm 00007843: 	STI	R2,*AR2++
    // asm 00007844: 	LSH	-8,R2
    // asm 00007845: 	STI	R2,*AR2++
    // *SEND AIRF, AIRB
    // asm 00007846: 	LDI	*+AR1(CAR_AIRF),R2
    // asm 00007847: 	STI	R2,*AR2++
    // asm 00007848: 	LDI	*+AR1(CAR_AIRB),R2
    // asm 00007849: 	STI	R2,*AR2++
    // *SEND BRAKE
    // asm 0000784A: 	LDF	*+AR1(CARBRAKE),R2
    // asm 0000784B: 	MPYF	100,R2
    // asm 0000784C: 	FIX	R2
    // asm 0000784D: 	STI	R2,*AR2++
    // *SEND TRACK ID
    // asm 0000784E: 	LDI	*+AR1(CARTRACK_ID),R2
    // asm 0000784F: 	STI	R2,*AR2++
    // asm 00007850: 	LSH	-8,R2
    // asm 00007851: 	STI	R2,*AR2++
    // asm 00007852: 	LSH	-8,R2
    // asm 00007853: 	STI	R2,*AR2++ 		;24 BITS ID
    // ;	CLRI	R0			;padding
    // ;	STI	R0,*AR2++
    // asm 00007854: 	LDI	@COMMQ_TMP_BUFFI,AR2	;GET START OF MESSAGE
    // asm 00007855: 	LDI	45,RC
    // ;	LDI	46,RC
    // asm 00007856: 	BR	MESSAGE_ADD
    // *
    // *SEND RHO CREATION
    // *AR4=OBJECT
    // *R5=VEHICLE DESCRIPTOR INDEX
    // *R0=VEHICLE ID #
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_PLAYERS_POS", 0, 0);
    UNIMPL();
}

void SEND_RHO_CREATE(void) {
    // asm 00007857: 	LDI	@HEAD2HEAD_ON,R1
    // asm 00007858: 	RETSZ
    // asm 00007859: 	LDI	CB_RHO_CREATE,R1	;GET MESSAGE HEADER
    // asm 0000785A: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 0000785B: 	STI	R1,*AR2++		;MESSAGE HEADER CODE
    // asm 0000785C: 	STI	R0,*AR2++		;save car id #
    // asm 0000785D: 	STI	R5,*AR2++	    	;SAVE CAR INDEX
    // asm 0000785E: 	LDI	*+AR4(OPAL),R0
    // asm 0000785F: 	LSH	-8,R0
    // asm 00007860: 	STI	R0,*AR2++
    // asm 00007861: 	LDI	4-1,RC
    // asm 00007862: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00007863: 	BR	MESSAGE_ADD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RHO_CREATE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *MATRIX DECODE CONSTANT
/* asm: MATCON	.FLOAT	0.00003125		;1/32000 */
static float MATCON = 0.00003125f;
/* asm: RADCON	.FLOAT	0.001			;1/1000 */
/* asm: 	 */
static float RADCON = 0.001f;
/*
 *
 *FIND DRONE CAR
 *RET Z=FOUND, AR0=CAR OBJECT
 *R2=-16, R3=-8
 */

void FIND_DRONE(void) {
    // asm 00007866: 	LDI	-16,R2
    // asm 00007867: 	LDI	-8,R3
    // *GET CAR ID #
    // asm 00007868: 	LSH	R2,*AR2++,R1   		;GET CAR #
    // asm 00007869: 	LDI	@CAR_LIST,R0	 	;GET LIST
    // asm 0000786A: 	B	FD1
FDL:
    // asm 0000786B: 	LDI	*+AR0(OCARBLK),AR1	;GET CAR BLOCK
    // asm 0000786C: 	CMPI	*+AR1(CARNUM),R1   	;IS THIS THE CAR
    // asm 0000786D: 	RETSZ				;WE FOUND IT
    // asm 0000786E: 	LDI	*+AR0(OLINK3),R0
FD1:
    // asm 0000786F: 	BNZD	FDL
    // asm 00007870: 	LDI	R0,AR0
    // asm 00007871: 	NOP
    // asm 00007872: 	NOP
    // 	;---->BNZD	FDL
    // asm 00007873: 	LDI	1,R0			;INDICATE DRONE NOT FOUND
    // asm 00007874: 	RETS
    // *
    // *DECODE RHO CAR UPDATE
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_DRONE", 0, 0);
    UNIMPL();
}

static void DECODE_RHO_UPDATE(void) {
    // asm 00007875: 	LDI	@IGNORE_UPDATES,R0
    // asm 00007876: 	BNZ	DECCARX
    // asm 00007877: 	CALL	FIND_DRONE
    // asm 00007878: 	BZ	DECODE_CAR_UP0
    // asm 00007879: 	SUBI	1,AR2
    // asm 0000787A: 	B	DECCARX			;CAR NOT FOUND
    // *
    // *DECODE DRONE CAR UPDATE
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RHO_UPDATE", 0, 0);
    UNIMPL();
}

static void DECODE_RACER_UPDATE(void) {
    // asm 0000787B: 	LDI	@IGNORE_UPDATES,R0
    // asm 0000787C: 	BNZ	DECCARX
    // asm 0000787D: 	LDI	-16,R2
    // asm 0000787E: 	LDI	-8,R3
    // *GET CAR ID #
    // asm 0000787F: 	LSH	R2,*AR2,R0   		;GET CAR #
    // asm 00007880: 	BLT	DECCARX
    // asm 00007881: 	CMPI	9,R0
    // asm 00007882: 	BGT	DECCARX			;RANGE CHECK
    // asm 00007883: 	ADDI	@RACER_PTRI,R0
    // asm 00007884: 	LDI	R0,AR0
    // asm 00007885: 	LDI	*AR0,R0		 	;GET POINTER
    // asm 00007886: 	BZ	DECCARX
    // asm 00007887: 	ADDI	1,AR2
    // asm 00007888: 	LDI	R0,AR0
    // asm 00007889: 	B	DECODE_CAR_UP0
    // *
    // *DECODE THE LINKED PLAYERS POSITION
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RACER_UPDATE", 0, 0);
    UNIMPL();
}

static void DECODE_CAR_UPDATE(void) {
    // asm 0000788A: 	LDI	@IGNORE_UPDATES,R0
    // asm 0000788B: 	BNZ	DECCARX
    // asm 0000788C: 	LDI	@PLY2CAR,R0
    // asm 0000788D: 	BZ	DECCARX			;NO CAR EXISTS
    // asm 0000788E: 	LDI	R0,AR0
    // asm 0000788F: 	LDI	-16,R2
    // asm 00007890: 	LDI	-8,R3
    // *GET CAR ID #
    // asm 00007891: 	LSH	R2,*AR2++,R1   		;GET CAR #
    // *
    // *AR0=POINTER TO CAR
    // *
DECODE_CAR_UP0:
    // asm 00007892: 	LDI	AR0,AR4			;SAVE CAR INDEX
    // *GET CAR XYZ
    // asm 00007893: 	ADDI	OPOSX,AR0
    // asm 00007894: 	LDI	2,RC
    // asm 00007895: 	RPTB	GETXYZ
    // asm 00007896: 	LSH	R2,*AR2++,R0		;GET X, Y, Z
    // asm 00007897: 	LSH	R2,*AR2++,R1
    // asm 00007898: 	LS	8,R1
    // asm 00007899: 	OR	R1,R0
    // asm 0000789A: 	LSH	R2,*AR2++,R1
    // asm 0000789B: 	LS	16,R1
    // asm 0000789C: 	OR	R1,R0
    // asm 0000789D: 	LSH	R2,*AR2++,R1
    // asm 0000789E: 	LS	24,R1
    // asm 0000789F: 	OR	R1,R0
    // asm 000078A0: 	FLOAT	R0
GETXYZ:
    // asm 000078A1: STF	R0,*AR0++
    // ;	ADDI	27,AR2	 		;BLOW IT OFF
    // ;	RETS
    // *GET MATRIX
    // asm 000078A2: 	LDI	AR4,AR0
    // asm 000078A3: 	ADDI	OMATRIX,AR0
    // asm 000078A4: 	LDF	@MATCON,R1
    // asm 000078A5: 	LDI	8,RC			;DO 9 MATRIX ENTRIES
    // asm 000078A6: 	RPTB	GETMAT
    // asm 000078A7: 	LSH	R2,*AR2++,R0
    // asm 000078A8: 	LSH	R3,*AR2++,R1
    // asm 000078A9: 	ADDI	R1,R0
    // asm 000078AA: 	LSH	16,R0
    // asm 000078AB: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm 000078AC: 	FLOAT	R0
    // asm 000078AD: 	MPYF	@MATCON,R0
GETMAT:
    // asm 000078AE: STF	R0,*AR0++
    // *GET CARYROT
    // asm 000078AF: 	LDI	AR4,AR0
    // asm 000078B0: 	LDI	*+AR0(OCARBLK),AR1
    // asm 000078B1: 	LDI	1,R0	 		;SET UPDATE FLAG
    // asm 000078B2: 	STI	R0,*+AR1(CARUPD)
    // asm 000078B3: 	LSH	R2,*AR2++,R0
    // asm 000078B4: 	LSH	R3,*AR2++,R1
    // asm 000078B5: 	ADDI	R1,R0
    // asm 000078B6: 	LSH	16,R0
    // asm 000078B7: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm 000078B8: 	FLOAT	R0
    // asm 000078B9: 	MPYF	@RADCON,R0
    // asm 000078BA: 	STF	R0,*+AR1(CARYROT)
    // *GET CARTURN
    // asm 000078BB: 	LSH	R2,*AR2++,R0
    // asm 000078BC: 	LSH	R3,*AR2++,R1
    // asm 000078BD: 	ADDI	R1,R0
    // asm 000078BE: 	LSH	16,R0
    // asm 000078BF: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm 000078C0: 	FLOAT	R0
    // asm 000078C1: 	MPYF	@RADCON,R0
    // asm 000078C2: 	STF	R0,*+AR1(CARTURN)
    // *GET CARVROT
    // asm 000078C3: 	LSH	R2,*AR2++,R0
    // asm 000078C4: 	LSH	R3,*AR2++,R1
    // asm 000078C5: 	ADDI	R1,R0
    // asm 000078C6: 	LSH	16,R0
    // asm 000078C7: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm 000078C8: 	FLOAT	R0
    // asm 000078C9: 	MPYF	@RADCON,R0
    // asm 000078CA: 	STF	R0,*+AR1(CARVROT)
    // *GET CARSPEED
    // asm 000078CB: 	LSH	R2,*AR2++,R0
    // asm 000078CC: 	LSH	R3,*AR2++,R1
    // asm 000078CD: 	ADDI	R1,R0
    // asm 000078CE: 	LSH	16,R0
    // asm 000078CF: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm 000078D0: 	FLOAT	R0
    // asm 000078D1: 	STF	R0,*+AR1(CARSPEED)
    // *GET AIRF, AIRB
    // asm 000078D2: 	LSH	R2,*AR2++,R1
    // asm 000078D3: 	STI	R1,*+AR1(CAR_AIRF)
    // asm 000078D4: 	LSH	R2,*AR2++,R1
    // asm 000078D5: 	STI	R1,*+AR1(CAR_AIRB)
    // *GET BRAKE
    // asm 000078D6: 	LSH	R2,*AR2++,R1
    // asm 000078D7: 	FLOAT	R1
    // asm 000078D8: 	MPYF	0.01,R1
    // asm 000078D9: 	STF	R1,*+AR1(CARBRAKE)	;BRAKE = FLOAT 0-1
    // *GET TRACK ID
    // asm 000078DA: 	LSH	R2,*AR2++,R0		;GET ROAD SECTION ID
    // asm 000078DB: 	LSH	R3,*AR2++,R1
    // asm 000078DC: 	ADDI	R1,R0
    // asm 000078DD: 	ADDI	*AR2++,R0
    // ;	LDI	*+AR0(OPLINK),AR3
    // ;	STI	R0,*+AR3(DELTA_LAST_OID)
    // asm 000078DE: 	STI	R0,*+AR1(CARTRACK_ID)
    // ;	ADDI	1,AR2	 		;PADDING
    // asm 000078DF: 	RETS
DECCARX:
    // ;	ADDI	46,AR2	 		;BLOW IT OFF
    // asm 000078E0: 	ADDI	45,AR2	 		;BLOW IT OFF
    // asm 000078E1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_CAR_UPDATE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static void DECODE_RHO_CREATE(void) {
    // asm 000078E2: 	LDI	@HEAD2HEAD_ON,R0
    // asm 000078E3: 	BZ	DCRHOX
    // asm 000078E4: 	LDI	@IGNORE_UPDATES,R0
    // asm 000078E5: 	BNZ	DCRHOX
    // asm 000078E6: 	PUSH	AR6
    // asm 000078E7: 	LDI	-16,R2
    // asm 000078E8: 	LDI	-8,R3
    // asm 000078E9: 	LSH	R2,*AR2++,R4   		;GET CAR ID #
    // asm 000078EA: 	LSH	R2,*AR2++,R5   		;GET INDEX  #
    // asm 000078EB: 	LSH	R3,*AR2++,AR6  		;GET OPAL IN AR6
    // asm 000078EC: 	PUSH	AR2
    // asm 000078ED: 	CREATE	RHO_START,DRONE_C|VEHICLE_T|DRNE_RHO
    // asm 000078F0: 	POP	AR2
    // asm 000078F1: 	POP	AR6
    // asm 000078F2: 	RETS
DCRHOX:
    // asm 000078F3: 	ADDI	3,AR2
    // asm 000078F4: 	RETS
    // *
    // *KILL OFF RACER
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RHO_CREATE", 0, 0);
    UNIMPL();
}

static void DECODE_RACER_KILL(void) {
    // asm 000078F5: 	LDI	-16,R2
    // asm 000078F6: 	LSH	R2,*AR2++,R0   		;GET CAR ID #
    // asm 000078F7: 	LDI	@IGNORE_UPDATES,R1
    // asm 000078F8: 	BNZ	DCRKX
    // asm 000078F9: 	BLT	DECRKX
    // asm 000078FA: 	CMPI	9,R0
    // asm 000078FB: 	BGT	DECRKX			;RANGE CHECK
    // asm 000078FC: 	ADDI	@RACER_PTRI,R0
    // asm 000078FD: 	LDI	R0,AR0
    // asm 000078FE: 	LDI	*AR0,R0		 	;GET POINTER
    // asm 000078FF: 	BZ	DECRKX
    // *KILL OFF OBJECT, PROCESS, ANIMATION PROCESS
DECRKX:
    // asm 00007900: 	RETS
DCRKX:
    // asm 00007901: 	INC	AR2
    // asm 00007902: 	RETS
    // *DECODE TRACK RANGE OTHER MACHINE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RACER_KILL", 0, 0);
    UNIMPL();
}

static void DECODE_OM_TRACK(void) {
    // asm 00007903: 	LDI	-16,R2
    // asm 00007904: 	LDI	-8,R3
    // asm 00007905: 	LSH	R2,*AR2++,R0		;GET ROAD SECTION ID
    // asm 00007906: 	LSH	R3,*AR2++,R1
    // asm 00007907: 	ADDI	R1,R0
    // asm 00007908: 	ADDI	*AR2++,R0
    // asm 00007909: 	STI	R0,@OM_TRACK_LO
    // asm 0000790A: 	LDI	-16,R2
    // asm 0000790B: 	LDI	-8,R3
    // asm 0000790C: 	LSH	R2,*AR2++,R0		;GET ROAD SECTION ID
    // asm 0000790D: 	LSH	R3,*AR2++,R1
    // asm 0000790E: 	ADDI	R1,R0
    // asm 0000790F: 	ADDI	*AR2++,R0
    // asm 00007910: 	STI	R0,@OM_TRACK_HI
    // asm 00007911: 	RETS
    // *SEND YOUR TRACK RANGE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_OM_TRACK", 0, 0);
    UNIMPL();
}

void SEND_OM_TRACK(void) {
    // asm 00007912: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00007913: 	LDI	CB_OM_TRACK,R2			;GET MESSAGE HEADER
    // asm 00007914: 	STI	R2,*AR2++
    // asm 00007915: 	LDI	@DYNALIST_TRUEBEGIN,AR0		;GET CLOSEST ROAD ID
    // asm 00007916: 	LDI	*+AR0(OLINK4),AR0		;LINK A COUPLE AHEAD
    // asm 00007917: 	LDI	*+AR0(OLINK4),AR0
    // asm 00007918: 	LDI	*+AR0(OUSR1),R2
    // asm 00007919: 	STI	R2,*AR2++
    // asm 0000791A: 	LSH	-8,R2
    // asm 0000791B: 	STI	R2,*AR2++
    // asm 0000791C: 	LSH	-8,R2
    // asm 0000791D: 	STI	R2,*AR2++ 			;24 BITS ID
    // asm 0000791E: 	LDI	@DYNALIST_END,AR0		;GET FURTHEST ROAD ID
    // asm 0000791F: 	LDI	*+AR0(OUSR1),R2
    // asm 00007920: 	STI	R2,*AR2++
    // asm 00007921: 	LSH	-8,R2
    // asm 00007922: 	STI	R2,*AR2++
    // asm 00007923: 	LSH	-8,R2
    // asm 00007924: 	STI	R2,*AR2++ 			;24 BITS ID
    // asm 00007925: 	LDI	7-1,RC
    // asm 00007926: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00007927: 	BR	MESSAGE_ADD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_OM_TRACK", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_BSYNC3(void) {
    // ;	LDI	0,R0
    // ;	STI	R0,@BSYNC
    // ;	LDI	CB_BONUS_SYNC0,AR2
    // ;	BU	SBLS
    // ;	LDI	1,R0
    // ;	STI	R0,@BSYNC
    // ;	LDI	CB_BONUS_SYNC1,AR2
    // ;	BU	SBLS
    // ;	LDI	2,R0
    // ;	STI	R0,@BSYNC
    // ;	LDI	CB_BONUS_SYNC2,AR2
    // ;	BU	SBLS
    // asm 00007928: 	LDI	3,R0
    // asm 00007929: 	STI	R0,@BSYNC
    // asm 0000792A: 	LDI	CB_BONUS_SYNC3,AR2
SBLS:
    // asm 0000792B: BR	MESSAGE_ADD_SB
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_BSYNC3", 0, 0);
    UNIMPL();
}

static void DECODE_BSYNC3(void) {
    // ;	LDI	0,R0
    // ;	STI	R0,@OM_BSYNC
    // ;	RETS
    // ;	LDI	1,R0
    // ;	STI	R0,@OM_BSYNC
    // ;	RETS
    // ;	LDI	2,R0
    // ;	STI	R0,@OM_BSYNC
    // ;	RETS
    // asm 0000792C: 	LDI	3,R0
    // asm 0000792D: 	STI	R0,@OM_BSYNC
    // asm 0000792E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_BSYNC3", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_CHANGE_MUSIC(void) {
    // asm 0000792F: 	LDI	*AR2++,R0
    // asm 00007930: 	LDI	@HEAD2HEAD_ON,R1
    // asm 00007931: 	RETSZ
    // asm 00007932: 	PUSH	AR2
    // asm 00007933: 	LS	8,R0
    // asm 00007934: 	RS	24,R0
    // asm 00007935: 	LDI	R0,AR2
    // asm 00007936: 	CALL	SET_TUNE_LINKED
    // asm 00007937: 	POP	AR2
    // asm 00007938: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_CHANGE_MUSIC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_CHANGE_MUSIC(void) {
    // asm 00007939: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 0000793A: 	LDI	CB_CHANGE_MUSIC,R0
    // asm 0000793B: 	STI	R0,*AR2++
    // asm 0000793C: 	LDI	@TUNE_IDX,R0
    // asm 0000793D: 	STI	R0,*AR2--
    // asm 0000793E: 	LDI	2-1,RC
    // asm 0000793F: 	CALL	MESSAGE_ADD
    // asm 00007940: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_CHANGE_MUSIC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: COINDROP	.bss	COINDROP,1 */
int COINDROP;

// *----------------------------------------------------------------------------
static void DECODE_COINDROP(void) {
    // asm 00007941: 	LDI	1,R0
    // asm 00007942: 	STI	R0,@COINDROP
    // asm 00007943: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_COINDROP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_COINDROP(void) {
    // asm 00007944: 	LDI	CB_COINDROP,AR2
    // asm 00007945: 	BR	MESSAGE_ADD_SB
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_COINDROP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: OM_DIAGVALUE	fbss	OM_DIAGVALUE,1 */
int OM_DIAGVALUE;
/* asm: DIAGVALUE	fbss	DIAGVALUE,1 */
int DIAGVALUE;

/*
 *----------------------------------------------------------------------------
 *For diagnostics on the Link we send a 'ping'
 *the ping is echoed back with the upper nybble
 *changed to reflect the machine type (MASTER
 *or SLAVE... 'A0' or 'B0' respectively.
 *
 *
 */
static void DECODE_DIAGNOSTIC(void) {
    // asm 00007946: 	LDI	*AR2++,R0
    // asm 00007947: 	LS	8,R0
    // asm 00007948: 	ASH	-24,R0
    // asm 00007949: 	STI	R0,@OM_DIAGVALUE
    // asm 0000794A: 	AND	0Fh,R0
    // asm 0000794B: 	STI	R0,@DIAGVALUE
    // asm 0000794C: 	CALL	SEND_DIAGNOSTIC
    // asm 0000794D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_DIAGNOSTIC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void SEND_DIAGNOSTIC(void) {
    // asm 0000794E: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 0000794F: 	LDI	CB_DIAGNOSTIC,R0
    // asm 00007950: 	STI	R0,*AR2++
    // asm 00007951: 	LDI	@DIAGVALUE,R0
    // asm 00007952: 	INC	R0
    // asm 00007953: 	STI	R0,@DIAGVALUE
    // asm 00007954: 	LDI	@DIPRAM,R0
    // asm 00007955: 	TSTB	CMDP_MASTER,R0
    // asm 00007956: 	LDINZ	0A0h,R1		;MASTER
    // asm 00007957: 	LDIZ	0B0h,R1		;SLAVE
    // asm 00007958: 	OR	R1,R0
    // asm 00007959: 	STI	R0,*AR2--
    // asm 0000795A: 	LDI	2-1,RC
    // asm 0000795B: 	BR	MESSAGE_ADD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_DIAGNOSTIC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DECODE_ATTRSND(void) {
    // asm 0000795C: 	PUSH	R0
    // asm 0000795D: 	PUSH	AR2
    // asm 0000795E: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm 00007960: 	CMPI	0,R0
    // asm 00007961: 	BEQ	NO_MUSIC
    // asm 00007962: 	SOND1	ATTR_THEME
NO_MUSIC:
    // asm 00007964: 	POP	AR2
    // asm 00007965: 	POP	R0
    // asm 00007966: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_ATTRSND", 0, 0);

    if (READADJ(ADJ_ATTRACT_MODE_SOUND) != 0) {
        SOND1(ATTR_THEME);
    }
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SEND_ATTRSND(void) {
    // asm 00007967: 	LDI	CB_ATTRSND,AR2
    // asm 00007968: 	BU	MESSAGE_ADD_SB
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_ATTRSND", 0, 0);
    MESSAGE_ADD_SB(CB_ATTRSND);
}

// *----------------------------------------------------------------------------

#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/obj.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/comm.h"
#include "../include/delta.h"
#include "../include/commq.h"

/*
 * Source module: asm/COMMQ.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: SEND_BUFFER_AI	.word	SEND_BUFFER_A */
int SEND_BUFFER_AI = (int)(SEND_BUFFER_A);
// 	;RECEIVE BUFFER
// 	;
// 	;OTHER MACHINE
// 	;(OM_*)
// 	;
/* asm: COMMQ_TMP_BUFFI	.word	COMMQ_TMP_BUFF */
int COMMQ_TMP_BUFFI = (int)(COMMQ_TMP_BUFF);
// *----------------------------------------------------------------------------
// *Clear all of the linking elements
// *
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *	.word	ID,ROUTINE TO DECODE
// *
// *
// *Each decode routine can assume:
// *	  AR2 is the pointer to the data block
// *	  AR6,DP registers must be saved/restored
// *	  AR2 should be incremented to the next block
// *
// *
/* asm: DECODE_BLOCKI	.word	DECODE_BLOCK */
int DECODE_BLOCKI = (int)(DECODE_BLOCK);
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *AR4=CAR OBJECT
// *AR5=CAR BLOCK
// *AR7=RHO PROCESS
// *
// *----------------------------------------------------------------------------
// *MATRIX DECODE CONSTANT
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *DECODE RHO CREATION
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------

void CLEAR_LINK(void)
{
    // asm: 	LDI	@PLY2CAR,R0
    // asm: 	STI	R0,@SAVED_PLY2CAR
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@MY_STATE
    // asm: 	STI	R0,@OM_STATE
    // asm: 	STI	R0,@OM_MODE
    // asm: 	STI	R0,@HEAD2HEAD_ON
    // asm: 	STI	R0,@MY_LINKWAIT
    // asm: 	STI	R0,@OM_LINKWAIT
    // asm: 	STI	R0,@PLY2CAR
    // asm: 	STI	R0,@CAR_LIST
    // asm: 	LDI	@RACER_PTRI,AR0
    // asm: 	RPTS	10-1
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@OM_CHOSEN_RACE
    // asm: 	STI	R0,@OM_VEHICLE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_LINK", 0, 0);
    UNIMPL();
}

void DECODE_BUFFER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_COMMP,R0 	       ;LINKED ?
    // asm: 	RETSNZ			       ;NOPE...
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm: 	RETSZ
    // asm: 	LDI	@RBUFF_LEN,R0	       ;ANYTHING THERE ?
    // asm: 	RETSZ			       ;NOPE
    // asm: 	LDI	@RECEIVE_BUFFERI,AR2
    // asm: 	LDI	@RBUFF_LEN,AR6
    // asm: 	INC	AR6
    // asm: 	LS	1,AR6
    // asm: 	ADDI	AR2,AR6
    // asm: 	B	DECLPX
DECODE_LP:
    // asm: 	LDI	*AR2++,AR0		;GET THE BLOCK ID
    // asm: 	LS	8,AR0
    // asm: 	RS	24,AR0			;SHIFT OFF THE CRAP
    // asm: 	CMPI	CB_LASTMSG,AR0		;CHECK BOGUS MESSAGE
#if CDEBUG
    // asm: 	BGE	$			;TRAP ON BUGUS FOR DEBUG
#endif
    // asm: 	BGE	ISDONE			;EXIT ON BOGUS
    // asm: 	ADDI	@DECODE_BLOCKI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
DECLPX:
    // asm: 	CMPI	AR6,AR2
    // asm: 	BLT	DECODE_LP
ISDONE:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@RBUFF_LEN
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_BUFFER", 0, 0);
    UNIMPL();
}

void COMMQ_PACKET_INIT(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *SETUP SEND BUFFER POINTERS
    // *
    // asm: 	PUSH	R0
    // ;	LDI	@SEND_BUFFER_AI,R0
    // ;	STI	R0,@FILLBUFF_PTR
    // ;	LDL	SEND_BUFFER_A_LEN,R0
    // ;	STI	R0,@FILLBUFF_LEN_PTR
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@SEND_BUFFER_A_LEN
    // asm: CMQIX
    // asm: 	CALL	SEND_MODE
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMMQ_PACKET_INIT", 0, 0);
    UNIMPL();
}

void COMMQ_READY_TO_SEND(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *FOR MASTER, HE WILL SEND IMMEDIATELY FOLLOWING,
    // *FOR SLAVE, HE WILL SWITCH BUFFERS, AND WHEN REQUESTED SEND THIS BUFFER
    // *IF THE NEXT BUFFER IF FULL, SLAVE WILL OVERWRITE THIS BUFFER, AND SO ON
    // *
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // 	;PAD THE END OF THE MESSAGES
    // asm: 	LDI	CB_NULL,AR2
    // asm: 	CALL	MESSAGE_ADD_SB
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMMQ_READY_TO_SEND", 0, 0);
    UNIMPL();
}

void MESSAGE_ADD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *REMEMBER ALSO THAT THESE ARE BYTE STRINGS (LOWER 8 BITS ONLY)
    // *
    // *
    // *
    // *PARAMETERS
    // *	RC	(LENGTH OF BUFFER)-1
    // *	AR2	PTR TO MESSAGE STRING (BUFFER)
    // *
    // *	(REMEMBER THAT THE FIRST WORD OF THE MESSAGE SHOULD BE THE
    // *	BLOCK HEADER)
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	PUSH	R0
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_COMMP,R0
    // asm: 	BNZ	MESSADDX
    // asm: 	LDI	RC,R0
    // asm: 	LDI	@SEND_BUFFER_AI,AR3
    // asm: 	ADDI	@SEND_BUFFER_A_LEN,AR3
    // asm: 	ADDI	@SEND_BUFFER_A_LEN,R0
    // ;	LDI	@FILLBUFF_LEN_PTR,AR0
    // ;	LDI	@FILLBUFF_PTR,AR3
    // ;	ADDI	*AR0,AR3		;start of new message
    // ;	ADDI	*AR0,R0
    // asm: 	INC	R0			;->(Length - 1)
    // asm: 	CMPI	COMM_BUFFER_SIZE,R0
#if CDEBUG
    // asm: 	BGT	$
#endif
    // asm: 	BGT	MESSADDX
    // ;	STI	R0,*AR0			;length
    // asm: 	STI	R0,@SEND_BUFFER_A_LEN
    // asm: 	RPTB	JJAG
    // asm: 	LDI	*AR2++,R0
JJAG:
    // asm: STI	R0,*AR3++
MESSADDX:
    // asm: 	POP	R0
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MESSAGE_ADD", 0, 0);
    UNIMPL();
}

void MESSAGE_ADD_SB(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *MESSAGE ADD, SINGLE BYTE
    // *
    // *PARAMETERS
    // *	AR2	MESSAGE
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_COMMP,R0
    // asm: 	BNZ	MASBX
    // asm: 	LDI	@SEND_BUFFER_A_LEN,R0
    // ;	LDI	@FILLBUFF_LEN_PTR,AR0
    // ;	LDI	*AR0,R0
    // asm: 	INC	R0
    // asm: 	CMPI	COMM_BUFFER_SIZE,R0
#if CDEBUG
    // asm: 	BGT	$
#endif
    // asm: 	BGT	MASBX
    // ;	STI	R0,*AR0
    // asm: 	STI	R0,@SEND_BUFFER_A_LEN
    // ;	LDI	*AR0,AR0
    // ;	ADDI	@FILLBUFF_PTR,AR0
    // asm: 	LDI	@SEND_BUFFER_AI,AR0
    // asm: 	ADDI	R0,AR0
    // asm: 	STI	AR2,*-AR0(1)
MASBX:
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MESSAGE_ADD_SB", 0, 0);
    UNIMPL();
}

void SEND_WAVEFL_READY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	CB_WAVEFL_READY,AR2
    // asm: 	BR	MESSAGE_ADD_SB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_WAVEFL_READY", 0, 0);
    UNIMPL();
}

void SEND_WAVEFL_SET(void)
{
    // asm: 	LDI	CB_WAVEFL_SET,AR2
    // asm: 	BR	MESSAGE_ADD_SB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_WAVEFL_SET", 0, 0);
    UNIMPL();
}

void SEND_WAVEFL_GO(void)
{
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_sectime
    // asm: 	LDI	CB_WAVEFL_GO,AR2
    // asm: 	BR	MESSAGE_ADD_SB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_WAVEFL_GO", 0, 0);
    UNIMPL();
}

void DECODE_WAVEFL_READY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@H2H_FLAGSTATE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_WAVEFL_READY", 0, 0);
    UNIMPL();
}

void DECODE_WAVEFL_SET(void)
{
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@H2H_FLAGSTATE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_WAVEFL_SET", 0, 0);
    UNIMPL();
}

void DECODE_WAVEFL_GO(void)
{
    // asm: 	LDI	3,R0
    // asm: 	STI	R0,@H2H_FLAGSTATE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_sectime
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_WAVEFL_GO", 0, 0);
    UNIMPL();
}

void SEND_LINKEDT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	AR2
    // asm: 	LDI	CB_HEAD2HEADT,AR2
    // asm: 	CALL	MESSAGE_ADD_SB
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_LINKEDT", 0, 0);
    UNIMPL();
}

void SEND_LINKEDF(void)
{
    // asm: 	PUSH	AR2
    // asm: 	LDI	CB_HEAD2HEADF,AR2
    // asm: 	CALL	MESSAGE_ADD_SB
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_LINKEDF", 0, 0);
    UNIMPL();
}

void DECODE_LINKEDT(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@LINKEDP
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_LINKEDT", 0, 0);
    UNIMPL();
}

void DECODE_LINKEDF(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@LINKEDP
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_LINKEDF", 0, 0);
    UNIMPL();
}

void DECODE_MODE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	LDI	0FFh,R2
    // asm: 	LS	16,R2
    // asm: 	AND	*AR2++,R2,R0
    // asm: 	RS	16,R0
    // asm: 	AND	*AR2++,R2,R1
    // asm: 	RS	8,R1
    // asm: 	OR	R1,R0
    // asm: 	AND	*AR2++,R2,R1
    // asm: 	OR	R1,R0
    // asm: 	AND	*AR2++,R2,R1
    // asm: 	LS	8,R1
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,@OM_MODE
    // 	;the state
    // asm: 	AND	*AR2++,R2,R0
    // asm: 	RS	16,R0
    // asm: 	AND	*AR2++,R2,R1
    // asm: 	LS	8,R1
    // asm: 	RS	16,R1
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,@OM_STATE
    // asm: 	AND	*AR2++,R2,R0
    // asm: 	RS	16,R0
    // asm: 	STI	R0,@OM_BONUS_WAITFLAG
    // 	;POSITION
    // asm: 	AND	*AR2++,R2,R0
    // asm: 	RS	16,R0
    // asm: 	STI	R0,@OM_POSITION
    // 	;the attract mode index
    // asm: 	AND	*AR2++,R2,R0
    // asm: 	LS	8,R0
    // asm: 	ASH	-24,R0
    // asm: 	LDI	@OM_ATTR_MODE,R1
    // asm: 	STI	R1,@OLD_OM_ATTR_MODE
    // asm: 	STI	R0,@OM_ATTR_MODE
    // 	;DECODE  OM_DIFF
    // asm: 	AND	*AR2++,R2,R0
    // asm: 	LSH	-16,R0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	0.01,R0
    // asm: 	STF	R0,@OM_DIFF
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BNE	DC_M_NOP
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@HEAD2HEAD_ON
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@OM_CHOSEN_RACE
    // asm: 	STI	R0,@OM_VEHICLE
    // asm: 	BU	DCMX
DC_M_NOP:
    // asm: 	LDI	@OM_STATE,R0
    // asm: 	TSTB	OMS_FINISHLINE,R0
    // asm: 	BZ	DWAI
    // asm: 	LDI	@MY_STATE,R1
    // asm: 	TSTB	OMS_FINISHLINE,R1
    // asm: 	BNZ	DWAI
    // 	;else: Hes finished, Im not
    // 	;therefore:  0 -> countdown
    // ;	LDI	1,R1
    // ;	STI	R1,@I_TIMED_OUT
    // asm: 	CLRI	R1
    // asm: 	STI	R1,@_countdown
    // asm: 	BU	DCMX
DWAI:
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0	;OM in game?
    // asm: 	BNE	DCMX
    // asm: 	LDI	@_MODE,R0	;ME in game?
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	NONEWCP
    // asm: 	LDI	@OM_STATE,R0
    // asm: 	AND	OMS_CPOINT_M,R0
    // asm: 	LDI	@MY_STATE,R1
    // asm: 	AND	OMS_CPOINT_M,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BEQ	NONEWCP
    // asm: 	CMPI	R0,R1		;is my CP cnt > his CP cnt?
    // asm: 	CALLLT	CHECKPOINT_HIT_R
NONEWCP:
DCMX:
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_MODE", 0, 0);
    UNIMPL();
}

void SEND_CHECKPOINT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *turn on the checkpoint bit within the MY_STATE
    // *does not actually send data over the comm lines.
    // *
    // *
    // asm: 	LDI	@CHECKPOINT_NUM,R0
    // asm: 	LDI	1,R1
    // asm: 	LS	R0,R1
    // asm: 	LS	8,R1
    // asm: 	OR	@MY_STATE,R1
    // asm: 	STI	R1,@MY_STATE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_CHECKPOINT", 0, 0);
    UNIMPL();
}

void SEND_FINISH(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	OMS_FINISHLINE,R0
    // asm: 	OR	@MY_STATE,R0
    // asm: 	STI	R0,@MY_STATE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_FINISH", 0, 0);
    UNIMPL();
}

void SEND_MODE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_MODE,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@_MODE,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	RS	8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	RS	8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	RS	8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@MY_STATE,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	RS	8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@BONUS_WAITFLAG,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@POSITION,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@_ATTR_MODE,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDF	@GAMEDIFF,R0 		;SEND DIFFICULTY
    // asm: 	MPYF	100,R0
    // asm: 	FIX	R0
    // asm: 	STI	R0,*AR2--(10)
    // asm: 	LDI	11-1,RC
    // asm: 	BR	MESSAGE_ADD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_MODE", 0, 0);
    UNIMPL();
}

void DECODE_LINKCANCELLED(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@OM_LINKWAIT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_LINKCANCELLED", 0, 0);
    UNIMPL();
}

void SEND_LINKCANCELLED(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	CB_LINKCANCELLED,AR2
    // asm: 	BR	MESSAGE_ADD_SB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_LINKCANCELLED", 0, 0);
    UNIMPL();
}

void SEND_START_GAME(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // * T 	I BOUGHT IN
    // *
    // *
    // *
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@IGNORE_UPDATES
    // asm: 	LDI	CB_START_GAME,AR2
    // asm: 	CALL	MESSAGE_ADD_SB
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@MY_STATE
    // asm: 	STI	R0,@HEAD2HEAD_ON
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@MY_LINKWAIT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_START_GAME", 0, 0);
    UNIMPL();
}

void DECODE_START_GAME(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@OM_LINKWAIT
    // asm: 	LDI	@MY_LINKWAIT,R0
    // asm: 	BZ	NOPE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HEAD2HEAD_ON
    // asm: 	INCAUD	AUD_H2HGAMES
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@OM_LINKWAIT
    // asm: 	STI	R0,@MY_LINKWAIT
    // asm: 	CALL	SEND_LINKEDT
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@OM_CHOSEN_RACE
    // asm: 	STI	R0,@OM_VEHICLE
    // asm: 	PUSH	AR2
    // asm: 	SOND1	START_THEME
    // asm: 	POP	AR2
    // asm: 	LDI	@RACE_MODE,R0
    // asm: 	CMPI	RM_USA,R0
    // asm: 	RETSNE
    // asm: 	CALL	SEND_RACENUM
    // asm: 	RETS
NOPE:
    // asm: 	CALL	SEND_LINKEDF
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@OM_CHOSEN_RACE
    // asm: 	STI	R0,@OM_VEHICLE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_START_GAME", 0, 0);
    UNIMPL();
}

void SEND_END_GAME(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	CB_END_GAME,AR2
    // asm: 	BR	MESSAGE_ADD_SB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_END_GAME", 0, 0);
    UNIMPL();
}

void DECODE_END_GAME(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *clear the variables associated with the game...
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@HEAD2HEAD_ON
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_END_GAME", 0, 0);
    UNIMPL();
}

void SEND_TIMECODE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_TIMECODE,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@_countdown,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	RS	8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@_sectime,R0
    // asm: 	STI	R0,*AR2--(3)
    // asm: 	LDI	4-1,RC
    // asm: 	BR	MESSAGE_ADD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_TIMECODE", 0, 0);
    UNIMPL();
}

void SEND_RACENUM(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	COMMQ_TMP_BUFF,AR2
    // asm: 	LDI	CB_RACENUM,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@HIDDEN_ON,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@RACE_MODE,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@CHOSEN_RACE,R0
    // asm: 	STI	R0,*AR2--(3)
    // asm: 	LDI	4-1,RC
    // asm: 	BR	MESSAGE_ADD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RACENUM", 0, 0);
    UNIMPL();
}

void DECODE_RACENUM(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	R1
    // asm: 	LDI	*AR2++,R1
    // asm: 	LS	8,R1
    // asm: 	ASH	-24,R1
    // asm: 	STI	R1,@OM_HIDDEN_ON
    // asm: 	LDI	*AR2++,R0
    // asm: 	LS	8,R0
    // asm: 	RS	24,R0
    // asm: 	STI	R0,@OM_RACE_MODE
    // asm: 	LDI	*AR2++,R0
    // asm: 	LS	8,R0
    // asm: 	RS	24,R0
    // asm: 	STI	R0,@OM_CHOSEN_RACE
    // asm: NOTHIDDEN
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RACENUM", 0, 0);
    UNIMPL();
}

void DECODE_VEHICLE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*AR2++,R0
    // asm: 	LS	8,R0
    // asm: 	RS	24,R0
    // asm: 	STI	R0,@OM_VEHICLE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_VEHICLE", 0, 0);
    UNIMPL();
}

void SEND_VEHICLE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_VEHICLE,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@CHOOSENCAR,R0	;contains extended vehicles
    // asm: 	STI	R0,*AR2--
    // asm: 	LDI	2-1,RC
    // asm: 	BR	MESSAGE_ADD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_VEHICLE", 0, 0);
    UNIMPL();
}

void DECODE_TIMECODE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	LDI	0FFh,R2
    // asm: 	LS	16,R2
    // asm: 	AND	*AR2++,R2,R0
    // asm: 	RS	16,R0
    // asm: 	AND	*AR2++,R2,R1
    // asm: 	RS	8,R1
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	AND	*AR2++,R2,R0
    // asm: 	STI	R0,@_sectime
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_TIMECODE", 0, 0);
    UNIMPL();
}

void SEND_RHO_POS(void)
{
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0
    // asm: 	STI	R0,*+AR5(CARTRACK_ID)  	;SAVE TRACK ID
    // asm: 	LDI	CB_RHO_UPDATE,R0	;GET MESSAGE HEADER
    // asm: 	LDI	AR4,AR0	    		;GET OBJECT IN AR0
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	STI	R0,*AR2++
    // *SEND CAR ID #
    // asm: 	LDI	*+AR5(CARNUM),R0
    // asm: 	B	SEND_CP0
    // *
    // *AR4=CAR OBJECT
    // *AR5=CAR BLOCK
    // *AR7=RACER PROCESS
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RHO_POS", 0, 0);
    UNIMPL();
}

void SEND_RACER_POS(void)
{
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0
    // asm: 	STI	R0,*+AR5(CARTRACK_ID)  	;SAVE TRACK ID
    // asm: 	LDI	CB_RACER_UPDATE,R0	;GET MESSAGE HEADER
    // asm: 	B	SEND_CAR_POS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RACER_POS", 0, 0);
    UNIMPL();
}

void SEND_PLAYERS_POS(void)
{
    // *
    // *AR4=PLAYER CAR OBJECT
    // *AR7=PLAYER PROCESS
    // *
    // asm: 	LDI	CB_CAR_UPDATE,R0	;GET MESSAGE HEADER
SEND_CAR_POS:
    // *SEND MESSAGE HEADER
    // asm: 	LDI	AR4,AR0	    		;GET OBJECT IN AR0
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	STI	R0,*AR2++
    // *SEND CAR ID #
    // asm: 	LDI	*+AR7(DELTA_INIT),R0
SEND_CP0:
    // asm: 	STI	R0,*AR2++
    // *SEND XYZ
    // asm: 	ADDI	OPOSX,AR0
    // asm: 	LDI	2,RC
    // asm: 	RPTB	SENDXYZ			;GET XYZ COORD
    // asm: 	FIX	*AR0++,R0		;CONVERT TO 32 BIT FIXED
    // asm: 	STI	R0,*AR2++
    // asm: 	LSH	-8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LSH	-8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LSH	-8,R0
SENDXYZ:
    // asm: STI	R0,*AR2++
    // *SEND MATRIX
    // asm: 	LDI	AR4,AR0	    		;GET OBJECT IN AR0
    // asm: 	ADDI	OMATRIX,AR0
    // asm: 	FLOAT	32000,R1
    // asm: 	LDI	8,RC			;DO 9 MATRIX ENTRIES
    // asm: 	RPTB	MATLP
    // asm: 	MPYF	*AR0++,R1,R0
    // asm: 	FIX	R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LSH	-8,R0
MATLP:
    // asm: STI	R0,*AR2++
    // *SEND CARYROT
    // asm: 	LDI	AR4,AR0	    		;GET OBJECT IN AR0
    // asm: 	LDI	*+AR0(OCARBLK),AR1
    // asm: 	LDF	*+AR1(CARYROT),R2
    // asm: 	CALL	NORMITS
    // asm: 	FLOAT	1000,R1
    // asm: 	MPYF	R1,R2
    // asm: 	FIX	R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++
    // *SEND CAR STRUCTURE STUFF
    // *SEND CARTURN
    // asm: 	LDF	*+AR1(CARTURN),R2
    // asm: 	CALL	NORMITS
    // asm: 	FLOAT	1000,R1
    // asm: 	MPYF	R1,R2
    // asm: 	FIX	R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++
    // *SEND CARVROT
    // asm: 	LDF	*+AR1(CARVROT),R2
    // asm: 	CALL	NORMITS
    // asm: 	FLOAT	1000,R1
    // asm: 	MPYF	R1,R2
    // asm: 	FIX	R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++
    // *SEND CARSPEED
    // asm: 	FIX	*+AR1(CARSPEED),R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++
    // *SEND AIRF, AIRB
    // asm: 	LDI	*+AR1(CAR_AIRF),R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LDI	*+AR1(CAR_AIRB),R2
    // asm: 	STI	R2,*AR2++
    // *SEND BRAKE
    // asm: 	LDF	*+AR1(CARBRAKE),R2
    // asm: 	MPYF	100,R2
    // asm: 	FIX	R2
    // asm: 	STI	R2,*AR2++
    // *SEND TRACK ID
    // asm: 	LDI	*+AR1(CARTRACK_ID),R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++ 		;24 BITS ID
    // ;	CLRI	R0			;padding
    // ;	STI	R0,*AR2++
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2	;GET START OF MESSAGE
    // asm: 	LDI	45,RC
    // ;	LDI	46,RC
    // asm: 	BR	MESSAGE_ADD
    // *
    // *SEND RHO CREATION
    // *AR4=OBJECT
    // *R5=VEHICLE DESCRIPTOR INDEX
    // *R0=VEHICLE ID #
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_PLAYERS_POS", 0, 0);
    UNIMPL();
}

void SEND_RHO_CREATE(void)
{
    // asm: 	LDI	@HEAD2HEAD_ON,R1
    // asm: 	RETSZ
    // asm: 	LDI	CB_RHO_CREATE,R1	;GET MESSAGE HEADER
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	STI	R1,*AR2++		;MESSAGE HEADER CODE
    // asm: 	STI	R0,*AR2++		;save car id #
    // asm: 	STI	R5,*AR2++	    	;SAVE CAR INDEX
    // asm: 	LDI	*+AR4(OPAL),R0
    // asm: 	LSH	-8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	4-1,RC
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	BR	MESSAGE_ADD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_RHO_CREATE", 0, 0);
    UNIMPL();
}

void FIND_DRONE(void)
{
    // *
    // *FIND DRONE CAR
    // *RET Z=FOUND, AR0=CAR OBJECT
    // *R2=-16, R3=-8
    // asm: 	LDI	-16,R2
    // asm: 	LDI	-8,R3
    // *GET CAR ID #
    // asm: 	LSH	R2,*AR2++,R1   		;GET CAR #
    // asm: 	LDI	@CAR_LIST,R0	 	;GET LIST
    // asm: 	B	FD1
FDL:
    // asm: 	LDI	*+AR0(OCARBLK),AR1	;GET CAR BLOCK
    // asm: 	CMPI	*+AR1(CARNUM),R1   	;IS THIS THE CAR
    // asm: 	RETSZ				;WE FOUND IT
    // asm: 	LDI	*+AR0(OLINK3),R0
FD1:
    // asm: 	BNZD	FDL
    // asm: 	LDI	R0,AR0
    // asm: 	NOP
    // asm: 	NOP
    // 	;---->BNZD	FDL
    // asm: 	LDI	1,R0			;INDICATE DRONE NOT FOUND
    // asm: 	RETS
    // *
    // *DECODE RHO CAR UPDATE
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_DRONE", 0, 0);
    UNIMPL();
}

void DECODE_FLY_UPDATE(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void DECODE_RHO_UPDATE(void)
{
    // asm: 	LDI	@IGNORE_UPDATES,R0
    // asm: 	BNZ	DECCARX
    // asm: 	CALL	FIND_DRONE
    // asm: 	BZ	DECODE_CAR_UP0
    // asm: 	SUBI	1,AR2
    // asm: 	B	DECCARX			;CAR NOT FOUND
    // *
    // *DECODE DRONE CAR UPDATE
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RHO_UPDATE", 0, 0);
    UNIMPL();
}

void DECODE_RACER_UPDATE(void)
{
    // asm: 	LDI	@IGNORE_UPDATES,R0
    // asm: 	BNZ	DECCARX
    // asm: 	LDI	-16,R2
    // asm: 	LDI	-8,R3
    // *GET CAR ID #
    // asm: 	LSH	R2,*AR2,R0   		;GET CAR #
    // asm: 	BLT	DECCARX
    // asm: 	CMPI	9,R0
    // asm: 	BGT	DECCARX			;RANGE CHECK
    // asm: 	ADDI	@RACER_PTRI,R0
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,R0		 	;GET POINTER
    // asm: 	BZ	DECCARX
    // asm: 	ADDI	1,AR2
    // asm: 	LDI	R0,AR0
    // asm: 	B	DECODE_CAR_UP0
    // *
    // *DECODE THE LINKED PLAYERS POSITION
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RACER_UPDATE", 0, 0);
    UNIMPL();
}

void DECODE_CAR_UPDATE(void)
{
    // asm: 	LDI	@IGNORE_UPDATES,R0
    // asm: 	BNZ	DECCARX
    // asm: 	LDI	@PLY2CAR,R0
    // asm: 	BZ	DECCARX			;NO CAR EXISTS
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	-16,R2
    // asm: 	LDI	-8,R3
    // *GET CAR ID #
    // asm: 	LSH	R2,*AR2++,R1   		;GET CAR #
    // *
    // *AR0=POINTER TO CAR
    // *
DECODE_CAR_UP0:
    // asm: 	LDI	AR0,AR4			;SAVE CAR INDEX
    // *GET CAR XYZ
    // asm: 	ADDI	OPOSX,AR0
    // asm: 	LDI	2,RC
    // asm: 	RPTB	GETXYZ
    // asm: 	LSH	R2,*AR2++,R0		;GET X, Y, Z
    // asm: 	LSH	R2,*AR2++,R1
    // asm: 	LS	8,R1
    // asm: 	OR	R1,R0
    // asm: 	LSH	R2,*AR2++,R1
    // asm: 	LS	16,R1
    // asm: 	OR	R1,R0
    // asm: 	LSH	R2,*AR2++,R1
    // asm: 	LS	24,R1
    // asm: 	OR	R1,R0
    // asm: 	FLOAT	R0
GETXYZ:
    // asm: STF	R0,*AR0++
    // ;	ADDI	27,AR2	 		;BLOW IT OFF
    // ;	RETS
    // *GET MATRIX
    // asm: 	LDI	AR4,AR0
    // asm: 	ADDI	OMATRIX,AR0
    // asm: 	LDF	@MATCON,R1
    // asm: 	LDI	8,RC			;DO 9 MATRIX ENTRIES
    // asm: 	RPTB	GETMAT
    // asm: 	LSH	R2,*AR2++,R0
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	LSH	16,R0
    // asm: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm: 	FLOAT	R0
    // asm: 	MPYF	@MATCON,R0
GETMAT:
    // asm: STF	R0,*AR0++
    // *GET CARYROT
    // asm: 	LDI	AR4,AR0
    // asm: 	LDI	*+AR0(OCARBLK),AR1
    // asm: 	LDI	1,R0	 		;SET UPDATE FLAG
    // asm: 	STI	R0,*+AR1(CARUPD)
    // asm: 	LSH	R2,*AR2++,R0
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	LSH	16,R0
    // asm: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm: 	FLOAT	R0
    // asm: 	MPYF	@RADCON,R0
    // asm: 	STF	R0,*+AR1(CARYROT)
    // *GET CARTURN
    // asm: 	LSH	R2,*AR2++,R0
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	LSH	16,R0
    // asm: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm: 	FLOAT	R0
    // asm: 	MPYF	@RADCON,R0
    // asm: 	STF	R0,*+AR1(CARTURN)
    // *GET CARVROT
    // asm: 	LSH	R2,*AR2++,R0
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	LSH	16,R0
    // asm: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm: 	FLOAT	R0
    // asm: 	MPYF	@RADCON,R0
    // asm: 	STF	R0,*+AR1(CARVROT)
    // *GET CARSPEED
    // asm: 	LSH	R2,*AR2++,R0
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	LSH	16,R0
    // asm: 	ASH	-16,R0			;DO SIGN EXTEND
    // asm: 	FLOAT	R0
    // asm: 	STF	R0,*+AR1(CARSPEED)
    // *GET AIRF, AIRB
    // asm: 	LSH	R2,*AR2++,R1
    // asm: 	STI	R1,*+AR1(CAR_AIRF)
    // asm: 	LSH	R2,*AR2++,R1
    // asm: 	STI	R1,*+AR1(CAR_AIRB)
    // *GET BRAKE
    // asm: 	LSH	R2,*AR2++,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	0.01,R1
    // asm: 	STF	R1,*+AR1(CARBRAKE)	;BRAKE = FLOAT 0-1
    // *GET TRACK ID
    // asm: 	LSH	R2,*AR2++,R0		;GET ROAD SECTION ID
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	ADDI	*AR2++,R0
    // ;	LDI	*+AR0(OPLINK),AR3
    // ;	STI	R0,*+AR3(DELTA_LAST_OID)
    // asm: 	STI	R0,*+AR1(CARTRACK_ID)
    // ;	ADDI	1,AR2	 		;PADDING
    // asm: 	RETS
DECCARX:
    // ;	ADDI	46,AR2	 		;BLOW IT OFF
    // asm: 	ADDI	45,AR2	 		;BLOW IT OFF
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_CAR_UPDATE", 0, 0);
    UNIMPL();
}

void DECODE_RHO_CREATE(void)
{
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	DCRHOX
    // asm: 	LDI	@IGNORE_UPDATES,R0
    // asm: 	BNZ	DCRHOX
    // asm: 	PUSH	AR6
    // asm: 	LDI	-16,R2
    // asm: 	LDI	-8,R3
    // asm: 	LSH	R2,*AR2++,R4   		;GET CAR ID #
    // asm: 	LSH	R2,*AR2++,R5   		;GET INDEX  #
    // asm: 	LSH	R3,*AR2++,AR6  		;GET OPAL IN AR6
    // asm: 	PUSH	AR2
    // asm: 	CREATE	RHO_START,DRONE_C|VEHICLE_T|DRNE_RHO
    // asm: 	POP	AR2
    // asm: 	POP	AR6
    // asm: 	RETS
DCRHOX:
    // asm: 	ADDI	3,AR2
    // asm: 	RETS
    // *
    // *KILL OFF RACER
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RHO_CREATE", 0, 0);
    UNIMPL();
}

void DECODE_RACER_KILL(void)
{
    // asm: 	LDI	-16,R2
    // asm: 	LSH	R2,*AR2++,R0   		;GET CAR ID #
    // asm: 	LDI	@IGNORE_UPDATES,R1
    // asm: 	BNZ	DCRKX
    // asm: 	BLT	DECRKX
    // asm: 	CMPI	9,R0
    // asm: 	BGT	DECRKX			;RANGE CHECK
    // asm: 	ADDI	@RACER_PTRI,R0
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,R0		 	;GET POINTER
    // asm: 	BZ	DECRKX
    // *KILL OFF OBJECT, PROCESS, ANIMATION PROCESS
DECRKX:
    // asm: 	RETS
DCRKX:
    // asm: 	INC	AR2
    // asm: 	RETS
    // *DECODE TRACK RANGE OTHER MACHINE
    // asm: DECODE_OM_TRACK
    // asm: 	LDI	-16,R2
    // asm: 	LDI	-8,R3
    // asm: 	LSH	R2,*AR2++,R0		;GET ROAD SECTION ID
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	ADDI	*AR2++,R0
    // asm: 	STI	R0,@OM_TRACK_LO
    // asm: 	LDI	-16,R2
    // asm: 	LDI	-8,R3
    // asm: 	LSH	R2,*AR2++,R0		;GET ROAD SECTION ID
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	ADDI	*AR2++,R0
    // asm: 	STI	R0,@OM_TRACK_HI
    // asm: 	RETS
    // *SEND YOUR TRACK RANGE
    // asm: SEND_OM_TRACK
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_OM_TRACK,R2			;GET MESSAGE HEADER
    // asm: 	STI	R2,*AR2++
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR0		;GET CLOSEST ROAD ID
    // asm: 	LDI	*+AR0(OLINK4),AR0		;LINK A COUPLE AHEAD
    // asm: 	LDI	*+AR0(OLINK4),AR0
    // asm: 	LDI	*+AR0(OUSR1),R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++ 			;24 BITS ID
    // asm: 	LDI	@DYNALIST_END,AR0		;GET FURTHEST ROAD ID
    // asm: 	LDI	*+AR0(OUSR1),R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LSH	-8,R2
    // asm: 	STI	R2,*AR2++ 			;24 BITS ID
    // asm: 	LDI	7-1,RC
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	BR	MESSAGE_ADD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RACER_KILL", 0, 0);
    UNIMPL();
}

void SEND_BSYNC0(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // ;	LDI	0,R0
    // ;	STI	R0,@BSYNC
    // ;	LDI	CB_BONUS_SYNC0,AR2
    // ;	BU	SBLS
    /* no executable asm lines detected */
    UNIMPL();
}

void SEND_BSYNC1(void)
{
    // ;	LDI	1,R0
    // ;	STI	R0,@BSYNC
    // ;	LDI	CB_BONUS_SYNC1,AR2
    // ;	BU	SBLS
    /* no executable asm lines detected */
    UNIMPL();
}

void SEND_BSYNC2(void)
{
    // ;	LDI	2,R0
    // ;	STI	R0,@BSYNC
    // ;	LDI	CB_BONUS_SYNC2,AR2
    // ;	BU	SBLS
    /* no executable asm lines detected */
    UNIMPL();
}

void SEND_BSYNC3(void)
{
    // asm: 	LDI	3,R0
    // asm: 	STI	R0,@BSYNC
    // asm: 	LDI	CB_BONUS_SYNC3,AR2
SBLS:
    // asm: BR	MESSAGE_ADD_SB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_BSYNC3", 0, 0);
    UNIMPL();
}

void DECODE_BSYNC0(void)
{
    // ;	LDI	0,R0
    // ;	STI	R0,@OM_BSYNC
    // ;	RETS
    /* no executable asm lines detected */
    UNIMPL();
}

void DECODE_BSYNC1(void)
{
    // ;	LDI	1,R0
    // ;	STI	R0,@OM_BSYNC
    // ;	RETS
    /* no executable asm lines detected */
    UNIMPL();
}

void DECODE_BSYNC2(void)
{
    // ;	LDI	2,R0
    // ;	STI	R0,@OM_BSYNC
    // ;	RETS
    /* no executable asm lines detected */
    UNIMPL();
}

void DECODE_BSYNC3(void)
{
    // asm: 	LDI	3,R0
    // asm: 	STI	R0,@OM_BSYNC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_BSYNC3", 0, 0);
    UNIMPL();
}

void DECODE_CHANGE_MUSIC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*AR2++,R0
    // asm: 	LDI	@HEAD2HEAD_ON,R1
    // asm: 	RETSZ
    // asm: 	PUSH	AR2
    // asm: 	LS	8,R0
    // asm: 	RS	24,R0
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	SET_TUNE_LINKED
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_CHANGE_MUSIC", 0, 0);
    UNIMPL();
}

void SEND_CHANGE_MUSIC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_CHANGE_MUSIC,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@TUNE_IDX,R0
    // asm: 	STI	R0,*AR2--
    // asm: 	LDI	2-1,RC
    // asm: 	CALL	MESSAGE_ADD
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_CHANGE_MUSIC", 0, 0);
    UNIMPL();
}

void DECODE_COINDROP(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@COINDROP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_COINDROP", 0, 0);
    UNIMPL();
}

void SEND_COINDROP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	CB_COINDROP,AR2
    // asm: 	BR	MESSAGE_ADD_SB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_COINDROP", 0, 0);
    UNIMPL();
}

void DECODE_DIAGNOSTIC(void)
{
    // *----------------------------------------------------------------------------
    // *For diagnostics on the Link we send a 'ping'
    // *the ping is echoed back with the upper nybble
    // *changed to reflect the machine type (MASTER
    // *or SLAVE... 'A0' or 'B0' respectively.
    // *
    // *
    // asm: 	LDI	*AR2++,R0
    // asm: 	LS	8,R0
    // asm: 	ASH	-24,R0
    // asm: 	STI	R0,@OM_DIAGVALUE
    // asm: 	AND	0Fh,R0
    // asm: 	STI	R0,@DIAGVALUE
    // asm: 	CALL	SEND_DIAGNOSTIC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_DIAGNOSTIC", 0, 0);
    UNIMPL();
}

void SEND_DIAGNOSTIC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_DIAGNOSTIC,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LDI	@DIAGVALUE,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@DIAGVALUE
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	LDINZ	0A0h,R1		;MASTER
    // asm: 	LDIZ	0B0h,R1		;SLAVE
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,*AR2--
    // asm: 	LDI	2-1,RC
    // asm: 	BR	MESSAGE_ADD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_DIAGNOSTIC", 0, 0);
    UNIMPL();
}

void DECODE_ATTRSND(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR2
    // asm: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NO_MUSIC
    // asm: 	SOND1	ATTR_THEME
NO_MUSIC:
    // asm: 	POP	AR2
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_ATTRSND", 0, 0);
    UNIMPL();
}

void SEND_ATTRSND(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	CB_ATTRSND,AR2
    // asm: 	BU	MESSAGE_ADD_SB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_ATTRSND", 0, 0);
    UNIMPL();
}

#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "text.h"
#include "objects.h"
#include "comm.h"
#include "snd.h"
#include "discovered_defines.h"

/*
 * Source module: asm/SND.ASM
 */

/* asm: IN_RESET_MODE	fbss	IN_RESET_MODE,1 */
int IN_RESET_MODE;
/* asm: RESET_TIMER	fbss	RESET_TIMER,R0 */
int RESET_TIMER[R0];
/* asm: SNDSTR	.bss	SNDSTR,NCHAN*SND_SIZ */
int SNDSTR[NCHAN*SND_SIZ];
/* asm: SNDEND	.bss	SNDEND,0 */
int SNDEND;
/* *----------------------------------------------------------------------------
*STATION TABLE
*
 */
/* asm: STATION_LIST	.word	MUNSTER_SURF	;0 */
/* asm: 	.word	SHUFFLE_DRIV	;1 */
/* asm: 	.word	DISCODUCK	;2 */
/* asm: 	.word	SPAGETTI	;3 */
/* asm: 	.word	VENTURE		;4 */
/* asm: 	.word	STRAIGHT_2_4	;5 */
/* asm: 	.word	DISCODUCK	;6 */
/* asm: 	.word	BOOGIE		;7 */
int STATION_LIST[8] = {
    MUNSTER_SURF, // 0
    SHUFFLE_DRIV, // 1
    DISCODUCK, // 2
    SPAGETTI, // 3
    VENTURE, // 4
    STRAIGHT_2_4, // 5
    DISCODUCK, // 6
    BOOGIE, // 7
};
/* asm: STATION_TEXTI	.word	STATION_TEXT */
#define STATION_TEXTI STATION_TEXT
/* *----------------------------------------------------------------------------
 */
/* asm: STATION_TIMEOUT	.bss	STATION_TIMEOUT,1 */
int STATION_TIMEOUT;
/* asm: RS_X	.bss	RS_X,1 */
int RS_X;
/* asm: TUNE_IDX	.bss	TUNE_IDX,1 */
int TUNE_IDX;
/* *----------------------------------------------------------------------------
*CLEAR THE SOUND DATA BASE
*
 */
/* *----------------------------------------------------------------------------
*SEND SOUND TO SOUND BOARD
*
*PARAMETER
*	AR2	SOUND CODE (16 BITS)
*
*RETURNS
*	NOTHING
*
*NOTE	On the actual writes to the sound board we are blowing the
*	specified timings as per Loffs suggestion to save time. He
*	says that they are over speced and that it shouldn't be a
*	problem. Initial tests indicate no change in operation.
*	Alter .set SPACER to a larger number if there is a problem.
*
*
 */
#define SPACER 30 //# OF NOPs BETWEEN WRITES

void RADIO_SHOW(void)
{
    // asm: 	LDI	@TUNE_IDX,AR2
    // asm: 	ADDI	@STATION_TEXTI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	FLOAT	210,R2
    // asm: 	FLOAT	370,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	LDL	cdd,AR2
    // asm: 	LDI	160,R2
    // asm: 	LDI	360,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	SLEEP	1
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	KILLME
    // asm: 	LDI	@STATION_TIMEOUT,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@STATION_TIMEOUT
    // asm: 	BLE	SUICIDE
    // asm: 	BU	RADIO_SHOW
KILLME:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@STATION_TIMEOUT
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RADIO_SHOW", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RADIO_BUT(void)
{
    // asm: 	LDPI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // ;	BNE	SUICIDE
    // asm: 	BEQ	RBMGAME
    // asm: 	BR	RBMATTR_CHECK	;This code is in hstdp.asm
RBMGAME:
    // asm: 	CALL	CHANGE_STATION
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RADIO_BUT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PARAMETERS
*	AR2	STATION #
*
 */
void SET_STATION(void)
{
#if DEBUG
    // asm: 	CMPI	0,AR2
    // asm: 	BLT	$
    // asm: 	CMPI	NUM_STATIONS,AR2
    // asm: 	BGE	$
#endif
    // asm: 	STI	AR2,@TUNE_IDX
    // asm: 	CALL	RESUME_TUNE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_STATION", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PARAMETERS
*	NONE
*
 */
void CHANGE_STATION(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@TUNE_IDX,R0
    // asm: 	INC	R0
    // asm: 	CMPI	8,R0
    // asm: 	LDIGE	0,R0
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	CALL	RESUME_TUNE
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHANGE_STATION", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PARAMETERS (SET_TUNE)
*	AR2	CHANNEL
*
 */
void SET_TUNE_LINKED(void)
{
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR6
    // asm: 	LDI	1,AR6
    // asm: 	STI	AR2,@TUNE_IDX
    // asm: 	BU	L987
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_TUNE_LINKED", 0, 0);
    UNIMPL();
}

void SET_TUNE(void)
{
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR6
    // asm: 	CLRI	AR6
    // asm: 	STI	AR2,@TUNE_IDX
    // asm: 	BU	L987
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_TUNE", 0, 0);
    UNIMPL();
}

void RESUME_TUNE(void)
{
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR6
    // asm: 	CLRI	AR6
L987:
    // asm: 	CALL	RESUME_TUNE_NT
    // asm: 	LDI	UTIL_C|RADIOPROC_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_FIND
    // asm: 	CMPI	0,AR0
    // asm: 	BNE	ISTHERE
    // asm: 	CREATE	RADIO_SHOW,UTIL_C|RADIOPROC_T
ISTHERE:
    // asm: 	LDI	40,R0
    // asm: 	STI	R0,@STATION_TIMEOUT
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@RS_X
    // ;	LDI	0,R0
    // ;	LDI	150,R1
    // ;	CALL	SET_TRACK_VOL
    // asm: 	CMPI	1,AR6
    // asm: 	BEQ	NOOMUS
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	NOOMUS
    // asm: 	CALL	SEND_CHANGE_MUSIC
NOOMUS:
    // asm: 	POP	AR6
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESUME_TUNE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RESUME_TUNE_NT(void)
{
    // asm: 	LDI	@TUNE_IDX,AR2
    // asm: 	CMPI	8,AR2
    // asm: 	LDIGT	0,AR2
    // asm: 	ADDI	@STATION_LISTI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	ONESND
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESUME_TUNE_NT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PARAMETERS
*	R0	TRACK #(0-3)	(EXCEPT IN MASTER VOL)
*	R1	VOL (0-255)
*
 */
void SET_MASTER_VOL(void)
{
    // asm: 	PUSH	AR2
    // asm: 	BUD	JI1
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	LDI	055AAh,R0
    // 	;---->	BUD	JI1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MASTER_VOL", 0, 0);
    UNIMPL();
}

void SET_TRACK_VOL(void)
{
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	LDI	1,AR2
    // asm: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm: 	PUSH	AR0
    // asm: 	LDI	R0,AR0
    // asm: 	MPYI	SND_SIZ,AR0
    // asm: 	ADDI	@SNDSTRI,AR0		;WE HAVE POINTER TO CHANNEL IN AR0
    // asm: 	STI	R1,*+AR0(SND_VOL)
    // asm: 	POP	AR0
    // asm: 	ADDI	055ABh,R0
JI1:
    // asm: 	LDI	1,AR2
    // asm: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	SENDSND
    // asm: 	AND	0FFh,R1
    // asm: 	CLRI	AR2
    // asm: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm: 	NOT	R1,R0
    // asm: 	AND	0FFh,R0
    // asm: 	LS	8,R1
    // asm: 	OR	R0,R1
    // asm: 	LDI	R1,AR2
    // asm: 	CALL	SENDSND
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_TRACK_VOL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RESET_SNDBRD(void)
{
    // 	;new sound system reset
    // 	;
    // 	;
    // 	;
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	LDI	R0,R1
    // asm: 	ANDN	SND2_RESET,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	STI	R1,@SYSCNTLR
    // asm: 	SETDP
    // asm: 	STI	R1,@SYSCNTL
    // 	;
    // asm: 	PUSH	DP
    // asm: 	LDP	@SOUND
    // asm: 	LDI	0FF00h,R0
    // asm: 	LDI	0F700h,R1
    // asm: 	STPI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	LDI	0FF3Ch,R0
    // asm: 	LDI	0FB3Ch,R1
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	LDI	500,RC
    // asm: 	MPYI	1000,RC
    // asm: 	RPTB	WAITIT
WAITIT:
    // asm: NOP
    // asm: 	LDI	0FF1Ch,R0
    // asm: 	LDI	0FB1Ch,R1
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	POP	DP
    // asm: 	BU	CLRSNDDB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_SNDBRD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SILENT(void)
{
    // asm: 	PUSH	AR2
    // asm: 	CALL	CLRSNDDB
    // asm: 	CLRI	AR2
    // asm: 	CALL	SENDSND
    // asm: 	LDI	1256,AR2
    // asm: 	CALL	SENDSND
    // asm: 	LDI	1,AR2
    // asm: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm: 	LDI	055CCh,AR2
    // asm: 	CALL	SENDSND
    // asm: 	LDI	0,AR2
    // asm: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm: 	CALL	SENDSND
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SILENT", 0, 0);
    UNIMPL();
}

void CLRSNDDB(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	R0
    // asm: 	LDI	@SNDSTRI,AR0
    // asm: 	CLRI	R0
    // asm: 	RPTS	(SND_SIZ*NCHAN)-1
    // asm: 	STI	R0,*AR0++
    // asm: 	POP	R0
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRSNDDB", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GET SOUND PRIORITY
*
*PARAMETERS
*	AR0	CHANNEL #0-3
*
 */
void GETPRI(void)
{
    // asm: 	RS	8,AR0
    // asm: 	AND	0Fh,AR0			;isolate channel
    // asm: 	MPYI	SND_SIZ,AR0
    // asm: 	ADDI	@SNDSTRI,AR0		;we have pointer to channel in AR0
    // asm: 	LDI	*+AR0(SND_PRI),R0
    // asm: 	AND	0FFh,R0			;extract priority of current sound
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETPRI", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ACTIVATE SOUND WITH VARIABLE VOLUME
*
*PARAMETERS
*	AR2	SOUND INDEX
*	R0	VOLUME 0-255
*
 */
void VOLSNDFX(void)
{
    // asm:  	AND	0FFH,R0		;KEEP IN RANGE
    // asm: 	B	SNDFX
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VOLSNDFX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ACTIVATE ONE SOUND ON ONE OF THE FX TRACKS (CHAN1 or CHAN2)
*
*1.  IF EITHER TRACK IS OPEN THEN SEND TO THAT TRACK
*2.  ELSE SEE IF IT HAS A HIGHER PRIORITY THAN EITHER TRACK
*3.  ELSE RETURN NOT AVAILABLE
*
*PARAMETERS
*	AR2	SOUND ENTRY INDEX
*RETURNS
*	C	SOUND SUCCESSFUL
*	NC	SONUD UNSUCCESSFUL
*
*
 */
void ONESNDFX(void)
{
    // asm: 	LDI	255,R0
    // *
    // *PARAMETERS
    // *	R0	SOUND VOLUME
    // *
SNDFX:
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR0
    // asm: 	PUSH 	R0			;SAVE VOLUME
    // asm: 	LDI	@_MODE,R1
    // asm: 	AND	MMODE,R1
    // asm: 	CMPI	MATTR,R1
    // asm: 	BEQ	SNDLDXFX		;IF attr DONT CALL SOUND FX
    // asm: 	LDI	1,AR0
    // asm: 	LDI	@SNDSTR+SND_SIZ+SND_PRI,R2
    // asm: 	LDI	R2,R0
    // asm: 	AND	0FFh,R2
    // asm: 	LDI	@SNDSTR+SND_SIZ+SND_SIZ+SND_PRI,R3
    // asm: 	LDI	R3,R1
    // asm: 	AND	0FFh,R3
    // asm: 	CMPI	R3,R2
    // asm: 	LDIGT	R3,R2
    // asm: 	LDIGT	R1,R0
    // asm: 	LDIGT	2,AR0	  		;CHANNEL #, LOWEST PRIORITY
    // asm: 	LDI	AR2,R3			;SAVE THE INDEX (IDENTITY)
    // asm: 	ADDI	@SNDTABI,AR2		;INDEX OUT TO GET ACTUAL SCRIPT
    // asm: 	LDI	*AR2,R1			;GET PRIORITY WORD
    // asm: 	AND	0FFh,R1
    // asm: 	CMPI	R2,R1
    // asm: 	BLT	NOSOUND_FX
    // asm: 	BGT	ONESNDFX2
    // asm: 	TSTB	ENINT,R0
    // asm: 	BNZ	NOSOUND_FX
ONESNDFX2:
    // asm: 	TSTB	NINT,R0
    // asm: 	BNZ	NOSOUND_FX
    // asm: 	LDI	AR0,R0
    // asm: 	MPYI	SND_SIZ,AR0
    // asm: 	ADDI	@SNDSTRI,AR0		;WE HAVE POINTER TO CHANNEL IN AR0
    // asm: 	POP	R1			;GET VOLUME VALUE
    // asm: 	CMPI	*+AR0(SND_VOL),R1
    // asm: 	BEQ	NOVOL
    // asm: 	CALL	SET_TRACK_VOL
NOVOL:
    // asm: 	LDI	1,R2			;R2  - # OF REPEATS
    // asm: 	STI	R2,*+AR0(SND_REP)
    // asm: 	STI	AR2,*+AR0(SND_ST)
    // asm: 	STI	AR2,*+AR0(SND_ADDR)
    // asm: 	STI	R3,*+AR0(SND_IDX)
    // asm: 	SUBI	1,R0
    // asm: 	STI	R0,*+AR0(SND_STATUS)	;INCREMENT SOUND BIT
    // asm: 	CALL	SNDUPD			;GET THE SOUND GOING
    // asm: 	PUSH	R0			;DUMMY PUSH
SNDLDXFX:
    // asm: 	SETC
L88:
    // asm: POP	R0
    // asm: 	POP	AR0
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
NOSOUND_FX:
    // asm: 	CLRC
    // asm: 	BU	L88
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ONESNDFX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*KILL A SOUND ON ONE OF THE FX TRACKS (CHAN1 or CHAN2)
*
*1.  IF TRACK HAS SOUND, KILL THAT TRACK
*2.  IF SOUND NOT ACTIVE DO NOTHING
*
*PARAMETERS
*
*	AR2	SOUND ENTRY INDEX
*
*RETURNS CS IF SOUND ACTUALLY KILLED, OTHERWISE CC
*
 */
void KILLSNDFX(void)
{
    // asm: 	PUSH	AR2
    // asm: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2
    // asm: 	BNE	KILSFX1
    // asm: 	SOND1	KILLCHAN1
    // asm: 	B	KILSFX3
KILSFX1:
    // asm: 	CMPI	@SNDSTR+2*(SND_SIZ)+SND_IDX,AR2
    // asm: 	BNE	KILSFX2
    // asm: 	SOND1	KILLCHAN2
    // asm: 	SETC
    // asm: 	B	KILSFX3
KILSFX2:
    // asm: 	CLRC
KILSFX3:
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILLSNDFX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	R0	SPEED  (0-255)
*	R1	VOLUME (0-255)
*
*
 */
void PLYR_ENGINE(void)
{
    // asm: 	LS	8,R0
    // asm: 	OR	R0,R1
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@DO_NOT_REENABLE_INT
    // asm: 	LDI	055CCh,AR2
    // asm: 	CALL	SENDSND
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@DO_NOT_REENABLE_INT
    // asm: 	LDI	R1,AR2
    // asm: 	B	SENDSND
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_ENGINE", 0, 0);
    UNIMPL();
}

void HARDSND(void)
{
    // asm: 	ADDI	@SNDTABI,AR2		;INDEX OUT TO GET ACTUAL SCRIPT
    // asm: 	LDI	*+AR2(1),AR2
    // asm: 	LS	16,AR2
    // asm: 	RS	16,AR2
    // asm: 	CALL	SENDSND
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HARDSND", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ACTIVATE ONE SOUND
*
*
*PARAMETERS
*	AR2	SOUND ENTRY INDEX
*
*
 */
void ONESND(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	AR0
#if DEBUG
    // asm: 	CMPI	0,AR2
    // asm: 	BLT	$
    // asm: 	CMPI	KILLALLS,AR2
    // asm: 	BGT	$
#endif
    // asm: 	LDI	AR2,R4			;SAVE THE INDEX (IDENTITY)
    // asm: 	ADDI	@SNDTABI,AR2		;INDEX OUT TO GET ACTUAL SCRIPT
    // asm: 	LDI	*AR2,R0			;GET PRIORITY WORD
    // asm: 	LDI	R0,AR0
    // asm: 	RS	8,AR0
    // asm: 	AND	0FH,AR0			;ISOLATE CHANNEL
#if DEBUG
    // asm: 	CMPI	3,AR0
    // asm: 	BGT	$
#endif
    // asm: 	MPYI	SND_SIZ,AR0
    // asm: 	ADDI	@SNDSTRI,AR0		;WE HAVE POINTER TO CHANNEL IN AR0
    // asm: 	LDI	*+AR0(SND_PRI),R1
    // asm: 	TSTB	NINT,R1			;IS IT NON-INTERRUPTABLE?
    // asm: 	BNZ	SNDLDX
    // asm: 	LDI	R1,R2
    // asm: 	AND	0FFH,R1			;EXTRACT PRIORITY OF CURRENT SOUND
    // asm: 	LDI	R0,R3
    // asm: 	AND	0FFH,R3			;EXTRACT PRIORITY OF OUR TEST
    // asm: 	CMPI	R3,R1
    // asm: 	BLT	SNDLD1			;BR->CURRENT SOUND IS MORE IMPORTANT
    // asm: 	BNE	SNDLDX
    // asm: 	TSTB	ENINT,R2		;CHECK INTERRUPTABLE BY EQUAL
    // asm: 	BNE	SNDLDX			;NOPE
SNDLD1:
    // asm: 	LDI	*+AR0(SND_VOL),R0
    // asm: 	CMPI	255,R0
    // asm: 	BEQ	NOTRESETVOL
    // asm: 	LDI	*AR2,R0			;GET PRIORITY WORD
    // asm: 	RS	8,R0
    // asm: 	AND	0FH,R0			;ISOLATE CHANNEL
    // asm: 	LDI	255,R1
    // asm: 	CALL	SET_TRACK_VOL
NOTRESETVOL:
    // asm: 	LDI	1,R2			;R2  - # OF REPEATS
    // asm: 	STI	R2,*+AR0(SND_REP)
    // asm: 	STI	AR2,*+AR0(SND_ST)
    // asm: 	STI	AR2,*+AR0(SND_ADDR)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR0(SND_STATUS)
    // asm: 	STI	R4,*+AR0(SND_IDX)
    // asm: 	CALL	SNDUPD			;GET THE SOUND GOING
SNDLDX:
    // asm: 	POP	AR0
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ONESND", 0, 0);
    UNIMPL();
}

void SENDSND(void)
{
    // asm: 	LDP	@IN_RESET_MODE
    // asm: 	LDI	@IN_RESET_MODE,R0
    // asm: 	SETDP
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NIRM
    // asm: 	RETS
NIRM:
    // asm: 	PUSH	AR3
    // asm: 	LDL	9A0000h,AR3	;SND2
    // asm: 	PUSHM	R0,R1
    // asm: 	DINT
    // asm: 	LDP	@SOUND		;byte 1
    // asm: 	LDI	AR2,R0
    // asm: 	RS	8,R0
    // asm: 	LDI	R0,R1
    // asm: 	OR	0FD00h,R1
    // asm: 	OR	0FF00h,R0
    // asm: 	STI	R0,@SOUND
    // asm: 	STI	R0,*AR3		;<-NEW SND2
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	AND	0FFh,AR2
    // asm: 	LDI	AR2,R0
    // asm: 	LDI	AR2,R1
    // asm: 	OR	0FF00h,R0	;byte 0
    // asm: 	OR	0FD00h,R1
    // asm: 	STI	R0,@SOUND
    // asm: 	STI	R0,*AR3		;<-NEW SND2
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	SETDP
    // asm: 	LDI	@DO_NOT_REENABLE_INT,R0
    // asm: 	CALLZ	ENABLEGIE
    // asm: 	POPM	R1,R0
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SENDSND", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*Frame Number	Action
*  1		Initialize Reset
*  2		Lift Reset
*  4		Send Single Byte
*  6		Turn off Reset
*
*
 */
void RESETMUNGE(void)
{
    // asm: 	DINT
    // asm: 	LDP	@RESET_TIMER
    // asm: 	LDI	@RESET_TIMER,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@RESET_TIMER
    // asm: 	SETDP
    // asm: 	CMPI	2,R0
    // asm: 	BNE	NOT_F1
    // 	;first pass through
    // 	;new reset mumbo jumbo
    // 	;
    // 	;
    // asm: 	LDI	@SYSCNTL,R0		;reset LOW for new board
    // asm: 	LDI	R0,R1
    // asm: 	ANDN	SND2_RESET,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	PUSH	R1
    // 	;continued below
    // asm: 	LDP	@SOUND
    // asm: 	LDI	0FF00h,R0		;set wheel board addr
    // asm: 	LDI	0F700h,R1
    // asm: 	STPI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	LDI	0FF3Ch,R0		;set reset HI (old board)
    // asm: 	LDI	0FB3Ch,R1
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	RPTS	2500			;wait for 100 milliseconds
    // asm: 	NOP
    // 	;new board from above
    // 	;
    // asm: 	POP	R1			;set reset HIGH (new board)
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R1,@SYSCNTLR
    // asm: 	SETDP
    // asm: 	STI	R1,@SYSCNTL
    // 	;
    // asm: 	LDP	@SOUND
    // asm: 	LDI	0FF00h,R0		;set addr
    // asm: 	LDI	0F700h,R1
    // asm: 	STPI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	LDI	0FF1Ch,R0		;set reset LOW (old board)
    // asm: 	LDI	0FB1Ch,R1
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	RPTS	50
    // asm: 	NOP
    // asm: 	BU	RESETMUNGE_X
NOT_F1:
    // asm: NOT_F2
    // asm: 	CMPI	4,R0
    // asm: 	BNE	NOT_F4
    // asm: 	LDL	9A0000h,AR2	;SND2
    // asm: 	LDP	@SOUND		;byte 1
    // asm: 	LDI	0FD00h,R1	;send dummy byte to abort BONGs
    // asm: 	LDI	0FF00h,R0
    // asm: 	STI	R0,@SOUND
    // asm: 	STI	R0,*AR2		;<-NEW SND2
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R1,@SOUND
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R0,@SOUND
    // asm: 	BU	RESETMUNGE_X
NOT_F4:
    // asm: 	CMPI	6,R0
    // asm: 	BNE	NOT_F6
    // asm: 	CLRI	R0
    // asm: 	LDP	@IN_RESET_MODE
    // asm: 	STI	R0,@IN_RESET_MODE
    // asm: 	SETDP
    // asm: 	READADJ	ADJ_VOLUME
    // asm: 	LDI	R0,R1
    // asm: 	CALL	SET_MASTER_VOL
    // 	;BU	RESETMUNGE_X
NOT_F6:
RESETMUNGE_X:
    // asm: 	SETDP
    // asm: 	CALL	ENABLEGIE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESETMUNGE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SND_RESET_QUIET(void)
{
    // asm: 	LDP	@RESET_TIMER
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@RESET_TIMER
    // asm: 	STI	R0,@IN_RESET_MODE
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SND_RESET_QUIET", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CALLED IN DISPLAY INTERRUPT (16ms)
*
 */
void SNDPROC(void)
{
    // asm: 	LDP	@IN_RESET_MODE
    // asm: 	LDI	@IN_RESET_MODE,R0
    // asm: 	SETDP
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	RESETMUNGE
    // asm: 	PUSH	R0
    // asm: 	PUSH	R4
    // asm: 	LDI	@SNDSTRI,AR0
    // asm: 	LDI	NCHAN-1,R4
SNDLP:
    // asm: 	LDI	*+AR0(SND_TMR),R0
    // asm: 	BEQ	SNDPLP
    // asm: 	DEC	R0
    // asm: 	STI	R0,*+AR0(SND_TMR)
    // asm: 	CALLZ	SNDUPD			;on ZERO CALL UPDATE
SNDPLP:
    // asm: 	ADDI	SND_SIZ,AR0
    // asm: 	DEC	R4
    // asm: 	BGE	SNDLP
    // asm: 	POP	R4
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SNDPROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PARAMETERS
*	AR0	PTR TO SOUND CHANNEL RAM
*
 */
void SNDUPD(void)
{
    // ;this code allows for scripted sound calls.
    // ;as of now they are not used, and I don't
    // ;believe they will be before CUSA is finished.
    // ;therefore this operation is removed.
    // ;
    // ;on next implimentation, set a BIT in the
    // ;control flag signifying a scripted sound
    // ;call - a WORD is a terrible thing to waste.
    // ;	ELP July 1,1994
    // ;
    // asm: 	PUSH	AR1
    // asm: 	LDI	*+AR0(SND_ADDR),AR1
    // asm: 	LDI	*AR1++,R0
    // asm: 	BZ	SNDCLRE			;NULL entry let us quit
    // asm: 	STI	R0,*+AR0(SND_PRI)
    // asm: 	LDI	*AR1,R0
    // asm: 	LDI	R0,AR2
    // asm: 	AND	07FFFh,AR2
    // asm: 	LDI	*+AR0(SND_STATUS),R0
    // asm: 	BZ	JUSTGO
    // asm: 	INC	AR2
JUSTGO:
    // asm: 	CALL	SENDSND			;do the actually sending
    // asm: 	LDI	*AR1++,R0		;EXTRACT TIMING DATA
    // asm: 	RS	16,R0
    // asm: 	STI	R0,*+AR0(SND_TMR)	;start countdown
    // asm: 	STI	AR1,*+AR0(SND_ADDR)
    // asm: 	CMPI	0,R0
    // asm: 	BGT	SNDUPX
SNDCLRE:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR0(SND_PRI)
    // asm: 	STI	R0,*+AR0(SND_ST)
    // asm: 	STI	R0,*+AR0(SND_STATUS)
SNDUPX:
    // asm: 	POP	AR1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SNDUPD", 0, 0);
    UNIMPL();
}

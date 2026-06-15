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
#include "discovered_labels.h"

/*
 * Source module: asm/SND.ASM
 */

void RADIO_SHOW(void);
void KILLME(void);
void RADIO_BUT(void);
void RBMGAME(void);
void SET_STATION(void);
void CHANGE_STATION(void);
void SET_TUNE_LINKED(void);
void SET_TUNE(void);
void RESUME_TUNE(void);
void RESUME_TUNE_NT(void);
void SET_MASTER_VOL(void);
void SET_TRACK_VOL(void);
void RESET_SNDBRD(void);
void SILENT(void);
void CLRSNDDB(void);
void GETPRI(void);
void VOLSNDFX(void);
void ONESNDFX(void);
void NOSOUND_FX(void);
void KILLSNDFX(void);
void KILSFX1(void);
void KILSFX2(void);
void PLYR_ENGINE(void);
void HARDSND(void);
void ONESND(void);
void SENDSND(void);
void NIRM(void);
void RESETMUNGE(void);
void NOT_F1(void);
void NOT_F4(void);
void SND_RESET_QUIET(void);
void SNDPROC(void);
void SNDUPD(void);

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
int STATION_LIST[] = {
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
/* asm: DO_NOT_REENABLE_INT	.bss	DO_NOT_REENABLE_INT,1 */
int DO_NOT_REENABLE_INT;
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
    // asm 000090E1: 	LDI	@TUNE_IDX,AR2
    // asm 000090E2: 	ADDI	@STATION_TEXTI,AR2
    // asm 000090E3: 	LDI	*AR2,AR2
    // asm 000090E4: 	FLOAT	210,R2
    // asm 000090E5: 	FLOAT	370,R3
    // asm 000090E6: 	LDI	1,RC
    // asm 000090E7: 	CALL	TEXT_ADDDS
    // asm 000090E8: 	LDL	cdd,AR2
    // asm 000090E9: 	LDI	160,R2
    // asm 000090EA: 	LDI	360,R3
    // asm 000090EB: 	LDI	TM|ZS,R4
    // asm 000090EC: 	CALL	BLTMOD2D_DS
    // asm 000090ED: 	SLEEP	1
    // asm 000090EF: 	LDI	@_MODE,R0
    // asm 000090F0: 	AND	MMODE,R0
    // asm 000090F1: 	CMPI	MGAME,R0
    // asm 000090F2: 	BNE	KILLME
    // asm 000090F3: 	LDI	@STATION_TIMEOUT,R0
    // asm 000090F4: 	DEC	R0
    // asm 000090F5: 	STI	R0,@STATION_TIMEOUT
    // asm 000090F6: 	BLE	SUICIDE
    // asm 000090F7: 	BU	RADIO_SHOW
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RADIO_SHOW", 0, 0);
    UNIMPL();
}

void KILLME(void)
{
    // asm 000090F8: 	CLRI	R0
    // asm 000090F9: 	STI	R0,@STATION_TIMEOUT
    // asm 000090FA: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILLME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RADIO_BUT(void)
{
    // asm 000090FB: 	LDPI	@_MODE,R0
    // asm 000090FC: 	AND	MMODE,R0
    // asm 000090FD: 	CMPI	MGAME,R0
    // ;	BNE	SUICIDE
    // asm 000090FE: 	BEQ	RBMGAME
    // asm 000090FF: 	BR	RBMATTR_CHECK	;This code is in hstdp.asm
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RADIO_BUT", 0, 0);
    UNIMPL();
}

void RBMGAME(void)
{
    // asm 00009100: 	CALL	CHANGE_STATION
    // asm 00009101: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RBMGAME", 0, 0);
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
    // asm 00009102: 	STI	AR2,@TUNE_IDX
    // asm 00009103: 	CALL	RESUME_TUNE
    // asm 00009104: 	RETS
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
    // asm 00009105: 	PUSH	R0
    // asm 00009106: 	LDI	@TUNE_IDX,R0
    // asm 00009107: 	INC	R0
    // asm 00009108: 	CMPI	8,R0
    // asm 00009109: 	LDIGE	0,R0
    // asm 0000910A: 	STI	R0,@TUNE_IDX
    // asm 0000910B: 	CALL	RESUME_TUNE
    // asm 0000910C: 	POP	R0
    // asm 0000910D: 	RETS
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
    // asm 0000910E: 	PUSH	AR2
    // asm 0000910F: 	PUSH	AR6
    // asm 00009110: 	LDI	1,AR6
    // asm 00009111: 	STI	AR2,@TUNE_IDX
    // asm 00009112: 	BU	L987
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_TUNE_LINKED", 0, 0);
    UNIMPL();
}

void SET_TUNE(void)
{
    // asm 00009113: 	PUSH	AR2
    // asm 00009114: 	PUSH	AR6
    // asm 00009115: 	CLRI	AR6
    // asm 00009116: 	STI	AR2,@TUNE_IDX
    // asm 00009117: 	BU	L987
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_TUNE", 0, 0);
    UNIMPL();
}

void RESUME_TUNE(void)
{
    // asm 00009118: 	PUSH	AR2
    // asm 00009119: 	PUSH	AR6
    // asm 0000911A: 	CLRI	AR6
L987:
    // asm 0000911B: 	CALL	RESUME_TUNE_NT
    // asm 0000911C: 	LDI	UTIL_C|RADIOPROC_T,R0
    // asm 0000911D: 	LDI	-1,R1
    // asm 0000911E: 	CALL	PRC_FIND
    // asm 0000911F: 	CMPI	0,AR0
    // asm 00009120: 	BNE	ISTHERE
    // asm 00009121: 	CREATE	RADIO_SHOW,UTIL_C|RADIOPROC_T
ISTHERE:
    // asm 00009124: 	LDI	40,R0
    // asm 00009125: 	STI	R0,@STATION_TIMEOUT
    // asm 00009126: 	LDI	0,R0
    // asm 00009127: 	STI	R0,@RS_X
    // ;	LDI	0,R0
    // ;	LDI	150,R1
    // ;	CALL	SET_TRACK_VOL
    // asm 00009128: 	CMPI	1,AR6
    // asm 00009129: 	BEQ	NOOMUS
    // asm 0000912A: 	LDI	@HEAD2HEAD_ON,R0
    // asm 0000912B: 	BZ	NOOMUS
    // asm 0000912C: 	CALL	SEND_CHANGE_MUSIC
NOOMUS:
    // asm 0000912D: 	POP	AR6
    // asm 0000912E: 	POP	AR2
    // asm 0000912F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESUME_TUNE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RESUME_TUNE_NT(void)
{
    // asm 00009130: 	LDI	@TUNE_IDX,AR2
    // asm 00009131: 	CMPI	8,AR2
    // asm 00009132: 	LDIGT	0,AR2
    // asm 00009133: 	ADDI	@STATION_LISTI,AR2
    // asm 00009134: 	LDI	*AR2,AR2
    // asm 00009135: 	CALL	ONESND
    // asm 00009136: 	RETS
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
    // asm 00009137: 	PUSH	AR2
    // asm 00009138: 	BUD	JI1
    // asm 00009139: 	PUSH	R0
    // asm 0000913A: 	PUSH	R1
    // asm 0000913B: 	LDI	055AAh,R0
    // 	;---->	BUD	JI1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MASTER_VOL", 0, 0);
    UNIMPL();
}

void SET_TRACK_VOL(void)
{
    // asm 0000913C: 	PUSH	AR2
    // asm 0000913D: 	PUSH	R0
    // asm 0000913E: 	PUSH	R1
    // asm 0000913F: 	LDI	1,AR2
    // asm 00009140: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm 00009141: 	PUSH	AR0
    // asm 00009142: 	LDI	R0,AR0
    // asm 00009143: 	MPYI	SND_SIZ,AR0
    // asm 00009144: 	ADDI	@SNDSTRI,AR0		;WE HAVE POINTER TO CHANNEL IN AR0
    // asm 00009145: 	STI	R1,*+AR0(SND_VOL)
    // asm 00009146: 	POP	AR0
    // asm 00009147: 	ADDI	055ABh,R0
JI1:
    // asm 00009148: 	LDI	1,AR2
    // asm 00009149: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm 0000914A: 	LDI	R0,AR2
    // asm 0000914B: 	CALL	SENDSND
    // asm 0000914C: 	AND	0FFh,R1
    // asm 0000914D: 	CLRI	AR2
    // asm 0000914E: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm 0000914F: 	NOT	R1,R0
    // asm 00009150: 	AND	0FFh,R0
    // asm 00009151: 	LS	8,R1
    // asm 00009152: 	OR	R0,R1
    // asm 00009153: 	LDI	R1,AR2
    // asm 00009154: 	CALL	SENDSND
    // asm 00009155: 	POP	R1
    // asm 00009156: 	POP	R0
    // asm 00009157: 	POP	AR2
    // asm 00009158: 	RETS
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
    // asm 00009159: 	LDI	@SYSCNTL,R0
    // asm 0000915A: 	LDI	R0,R1
    // asm 0000915B: 	ANDN	SND2_RESET,R0
    // asm 0000915C: 	STI	R0,@SYSCNTL
    // asm 0000915D: 	LDP	@SYSCNTLR
    // asm 0000915E: 	STI	R0,@SYSCNTLR
    // asm 0000915F: 	NOP
    // asm 00009160: 	NOP
    // asm 00009161: 	NOP
    // asm 00009162: 	STI	R1,@SYSCNTLR
    // asm 00009163: 	SETDP
    // asm 00009164: 	STI	R1,@SYSCNTL
    // 	;
    // asm 00009165: 	PUSH	DP
    // asm 00009166: 	LDP	@SOUND
    // asm 00009167: 	LDI	0FF00h,R0
    // asm 00009168: 	LDI	0F700h,R1
    // asm 00009169: 	STPI	R0,@SOUND
    // asm 0000916A: 	RPTS	50
    // asm 0000916B: 	NOP
    // asm 0000916C: 	STI	R1,@SOUND
    // asm 0000916D: 	RPTS	50
    // asm 0000916E: 	NOP
    // asm 0000916F: 	STI	R0,@SOUND
    // asm 00009170: 	RPTS	50
    // asm 00009171: 	NOP
    // asm 00009172: 	LDI	0FF3Ch,R0
    // asm 00009173: 	LDI	0FB3Ch,R1
    // asm 00009174: 	STI	R0,@SOUND
    // asm 00009175: 	RPTS	50
    // asm 00009176: 	NOP
    // asm 00009177: 	STI	R1,@SOUND
    // asm 00009178: 	RPTS	50
    // asm 00009179: 	NOP
    // asm 0000917A: 	STI	R0,@SOUND
    // asm 0000917B: 	RPTS	50
    // asm 0000917C: 	NOP
    // asm 0000917D: 	LDI	500,RC
    // asm 0000917E: 	MPYI	1000,RC
    // asm 0000917F: 	RPTB	WAITIT
WAITIT:
    // asm 00009180: NOP
    // asm 00009181: 	LDI	0FF1Ch,R0
    // asm 00009182: 	LDI	0FB1Ch,R1
    // asm 00009183: 	STI	R0,@SOUND
    // asm 00009184: 	RPTS	50
    // asm 00009185: 	NOP
    // asm 00009186: 	STI	R1,@SOUND
    // asm 00009187: 	RPTS	50
    // asm 00009188: 	NOP
    // asm 00009189: 	STI	R0,@SOUND
    // asm 0000918A: 	RPTS	50
    // asm 0000918B: 	NOP
    // asm 0000918C: 	POP	DP
    // asm 0000918D: 	BU	CLRSNDDB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_SNDBRD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SILENT(void)
{
    // asm 0000918E: 	PUSH	AR2
    // asm 0000918F: 	CALL	CLRSNDDB
    // asm 00009190: 	CLRI	AR2
    // asm 00009191: 	CALL	SENDSND
    // asm 00009192: 	LDI	1256,AR2
    // asm 00009193: 	CALL	SENDSND
    // asm 00009194: 	LDI	1,AR2
    // asm 00009195: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm 00009196: 	LDI	055CCh,AR2
    // asm 00009197: 	CALL	SENDSND
    // asm 00009198: 	LDI	0,AR2
    // asm 00009199: 	STI	AR2,@DO_NOT_REENABLE_INT
    // asm 0000919A: 	CALL	SENDSND
    // asm 0000919B: 	POP	AR2
    // asm 0000919C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SILENT", 0, 0);
    UNIMPL();
}

void CLRSNDDB(void)
{
    // asm 0000919E: 	PUSH	AR0
    // asm 0000919F: 	PUSH	R0
    // asm 000091A0: 	LDI	@SNDSTRI,AR0
    // asm 000091A1: 	CLRI	R0
    // asm 000091A2: 	RPTS	(SND_SIZ*NCHAN)-1
    // asm 000091A3: 	STI	R0,*AR0++
    // asm 000091A4: 	POP	R0
    // asm 000091A5: 	POP	AR0
    // asm 000091A6: 	RETS
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
    // asm 000091A7: 	RS	8,AR0
    // asm 000091A8: 	AND	0Fh,AR0			;isolate channel
    // asm 000091A9: 	MPYI	SND_SIZ,AR0
    // asm 000091AA: 	ADDI	@SNDSTRI,AR0		;we have pointer to channel in AR0
    // asm 000091AB: 	LDI	*+AR0(SND_PRI),R0
    // asm 000091AC: 	AND	0FFh,R0			;extract priority of current sound
    // asm 000091AD: 	RETS
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
    // asm 000091AE:  	AND	0FFH,R0		;KEEP IN RANGE
    // asm 000091AF: 	B	SNDFX
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
    // asm 000091B0: 	LDI	255,R0
    // *
    // *PARAMETERS
    // *	R0	SOUND VOLUME
    // *
SNDFX:
    // asm 000091B1: 	PUSH	R1
    // asm 000091B2: 	PUSH	R2
    // asm 000091B3: 	PUSH	R3
    // asm 000091B4: 	PUSH	AR0
    // asm 000091B5: 	PUSH 	R0			;SAVE VOLUME
    // asm 000091B6: 	LDI	@_MODE,R1
    // asm 000091B7: 	AND	MMODE,R1
    // asm 000091B8: 	CMPI	MATTR,R1
    // asm 000091B9: 	BEQ	SNDLDXFX		;IF attr DONT CALL SOUND FX
    // asm 000091BA: 	LDI	1,AR0
    // asm 000091BB: 	LDI	@SNDSTR+SND_SIZ+SND_PRI,R2
    // asm 000091BC: 	LDI	R2,R0
    // asm 000091BD: 	AND	0FFh,R2
    // asm 000091BE: 	LDI	@SNDSTR+SND_SIZ+SND_SIZ+SND_PRI,R3
    // asm 000091BF: 	LDI	R3,R1
    // asm 000091C0: 	AND	0FFh,R3
    // asm 000091C1: 	CMPI	R3,R2
    // asm 000091C2: 	LDIGT	R3,R2
    // asm 000091C3: 	LDIGT	R1,R0
    // asm 000091C4: 	LDIGT	2,AR0	  		;CHANNEL #, LOWEST PRIORITY
    // asm 000091C5: 	LDI	AR2,R3			;SAVE THE INDEX (IDENTITY)
    // asm 000091C6: 	ADDI	@SNDTABI,AR2		;INDEX OUT TO GET ACTUAL SCRIPT
    // asm 000091C7: 	LDI	*AR2,R1			;GET PRIORITY WORD
    // asm 000091C8: 	AND	0FFh,R1
    // asm 000091C9: 	CMPI	R2,R1
    // asm 000091CA: 	BLT	NOSOUND_FX
    // asm 000091CB: 	BGT	ONESNDFX2
    // asm 000091CC: 	TSTB	ENINT,R0
    // asm 000091CD: 	BNZ	NOSOUND_FX
ONESNDFX2:
    // asm 000091CE: 	TSTB	NINT,R0
    // asm 000091CF: 	BNZ	NOSOUND_FX
    // asm 000091D0: 	LDI	AR0,R0
    // asm 000091D1: 	MPYI	SND_SIZ,AR0
    // asm 000091D2: 	ADDI	@SNDSTRI,AR0		;WE HAVE POINTER TO CHANNEL IN AR0
    // asm 000091D3: 	POP	R1			;GET VOLUME VALUE
    // asm 000091D4: 	CMPI	*+AR0(SND_VOL),R1
    // asm 000091D5: 	BEQ	NOVOL
    // asm 000091D6: 	CALL	SET_TRACK_VOL
NOVOL:
    // asm 000091D7: 	LDI	1,R2			;R2  - # OF REPEATS
    // asm 000091D8: 	STI	R2,*+AR0(SND_REP)
    // asm 000091D9: 	STI	AR2,*+AR0(SND_ST)
    // asm 000091DA: 	STI	AR2,*+AR0(SND_ADDR)
    // asm 000091DB: 	STI	R3,*+AR0(SND_IDX)
    // asm 000091DC: 	SUBI	1,R0
    // asm 000091DD: 	STI	R0,*+AR0(SND_STATUS)	;INCREMENT SOUND BIT
    // asm 000091DE: 	CALL	SNDUPD			;GET THE SOUND GOING
    // asm 000091DF: 	PUSH	R0			;DUMMY PUSH
SNDLDXFX:
    // asm 000091E0: 	SETC
L88:
    // asm 000091E1: POP	R0
    // asm 000091E2: 	POP	AR0
    // asm 000091E3: 	POP	R3
    // asm 000091E4: 	POP	R2
    // asm 000091E5: 	POP	R1
    // asm 000091E6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ONESNDFX", 0, 0);
    UNIMPL();
}

void NOSOUND_FX(void)
{
    // asm 000091E7: 	CLRC
    // asm 000091E8: 	BU	L88
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOSOUND_FX", 0, 0);
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
    // asm 000091E9: 	PUSH	AR2
    // asm 000091EA: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2
    // asm 000091EB: 	BNE	KILSFX1
    // asm 000091EC: 	SOND1	KILLCHAN1
    // asm 000091EE: 	B	KILSFX3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILLSNDFX", 0, 0);
    UNIMPL();
}

void KILSFX1(void)
{
    // asm 000091EF: 	CMPI	@SNDSTR+2*(SND_SIZ)+SND_IDX,AR2
    // asm 000091F0: 	BNE	KILSFX2
    // asm 000091F1: 	SOND1	KILLCHAN2
    // asm 000091F3: 	SETC
    // asm 000091F4: 	B	KILSFX3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILSFX1", 0, 0);
    UNIMPL();
}

void KILSFX2(void)
{
    // asm 000091F5: 	CLRC
KILSFX3:
    // asm 000091F6: 	POP	AR2
    // asm 000091F7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILSFX2", 0, 0);
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
    // asm 000091F8: 	LS	8,R0
    // asm 000091F9: 	OR	R0,R1
    // asm 000091FA: 	LDI	1,R0
    // asm 000091FB: 	STI	R0,@DO_NOT_REENABLE_INT
    // asm 000091FC: 	LDI	055CCh,AR2
    // asm 000091FD: 	CALL	SENDSND
    // asm 000091FE: 	CLRI	R0
    // asm 000091FF: 	STI	R0,@DO_NOT_REENABLE_INT
    // asm 00009200: 	LDI	R1,AR2
    // asm 00009201: 	B	SENDSND
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_ENGINE", 0, 0);
    UNIMPL();
}

void HARDSND(void)
{
    // asm 00009202: 	ADDI	@SNDTABI,AR2		;INDEX OUT TO GET ACTUAL SCRIPT
    // asm 00009203: 	LDI	*+AR2(1),AR2
    // asm 00009204: 	LS	16,AR2
    // asm 00009205: 	RS	16,AR2
    // asm 00009206: 	CALL	SENDSND
    // asm 00009207: 	RETS
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
    // asm 00009208: 	PUSH	R0
    // asm 00009209: 	PUSH	R1
    // asm 0000920A: 	PUSH	R2
    // asm 0000920B: 	PUSH	R3
    // asm 0000920C: 	PUSH	R4
    // asm 0000920D: 	PUSH	AR0
#if DEBUG
    // asm: 	CMPI	0,AR2
    // asm: 	BLT	$
    // asm: 	CMPI	KILLALLS,AR2
    // asm: 	BGT	$
#endif
    // asm 0000920E: 	LDI	AR2,R4			;SAVE THE INDEX (IDENTITY)
    // asm 0000920F: 	ADDI	@SNDTABI,AR2		;INDEX OUT TO GET ACTUAL SCRIPT
    // asm 00009210: 	LDI	*AR2,R0			;GET PRIORITY WORD
    // asm 00009211: 	LDI	R0,AR0
    // asm 00009212: 	RS	8,AR0
    // asm 00009213: 	AND	0FH,AR0			;ISOLATE CHANNEL
#if DEBUG
    // asm: 	CMPI	3,AR0
    // asm: 	BGT	$
#endif
    // asm 00009214: 	MPYI	SND_SIZ,AR0
    // asm 00009215: 	ADDI	@SNDSTRI,AR0		;WE HAVE POINTER TO CHANNEL IN AR0
    // asm 00009216: 	LDI	*+AR0(SND_PRI),R1
    // asm 00009217: 	TSTB	NINT,R1			;IS IT NON-INTERRUPTABLE?
    // asm 00009218: 	BNZ	SNDLDX
    // asm 00009219: 	LDI	R1,R2
    // asm 0000921A: 	AND	0FFH,R1			;EXTRACT PRIORITY OF CURRENT SOUND
    // asm 0000921B: 	LDI	R0,R3
    // asm 0000921C: 	AND	0FFH,R3			;EXTRACT PRIORITY OF OUR TEST
    // asm 0000921D: 	CMPI	R3,R1
    // asm 0000921E: 	BLT	SNDLD1			;BR->CURRENT SOUND IS MORE IMPORTANT
    // asm 0000921F: 	BNE	SNDLDX
    // asm 00009220: 	TSTB	ENINT,R2		;CHECK INTERRUPTABLE BY EQUAL
    // asm 00009221: 	BNE	SNDLDX			;NOPE
SNDLD1:
    // asm 00009222: 	LDI	*+AR0(SND_VOL),R0
    // asm 00009223: 	CMPI	255,R0
    // asm 00009224: 	BEQ	NOTRESETVOL
    // asm 00009225: 	LDI	*AR2,R0			;GET PRIORITY WORD
    // asm 00009226: 	RS	8,R0
    // asm 00009227: 	AND	0FH,R0			;ISOLATE CHANNEL
    // asm 00009228: 	LDI	255,R1
    // asm 00009229: 	CALL	SET_TRACK_VOL
NOTRESETVOL:
    // asm 0000922A: 	LDI	1,R2			;R2  - # OF REPEATS
    // asm 0000922B: 	STI	R2,*+AR0(SND_REP)
    // asm 0000922C: 	STI	AR2,*+AR0(SND_ST)
    // asm 0000922D: 	STI	AR2,*+AR0(SND_ADDR)
    // asm 0000922E: 	CLRI	R0
    // asm 0000922F: 	STI	R0,*+AR0(SND_STATUS)
    // asm 00009230: 	STI	R4,*+AR0(SND_IDX)
    // asm 00009231: 	CALL	SNDUPD			;GET THE SOUND GOING
SNDLDX:
    // asm 00009232: 	POP	AR0
    // asm 00009233: 	POP	R4
    // asm 00009234: 	POP	R3
    // asm 00009235: 	POP	R2
    // asm 00009236: 	POP	R1
    // asm 00009237: 	POP	R0
    // asm 00009238: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ONESND", 0, 0);
    UNIMPL();
}

void SENDSND(void)
{
    // asm 00009239: 	LDP	@IN_RESET_MODE
    // asm 0000923A: 	LDI	@IN_RESET_MODE,R0
    // asm 0000923B: 	SETDP
    // asm 0000923C: 	CMPI	0,R0
    // asm 0000923D: 	BEQ	NIRM
    // asm 0000923E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SENDSND", 0, 0);
    UNIMPL();
}

void NIRM(void)
{
    // asm 0000923F: 	PUSH	AR3
    // asm 00009240: 	LDL	9A0000h,AR3	;SND2
    // asm 00009241: 	PUSHM	R0,R1
    // asm 00009243: 	DINT
    // asm 00009249: 	LDP	@SOUND		;byte 1
    // asm 0000924A: 	LDI	AR2,R0
    // asm 0000924B: 	RS	8,R0
    // asm 0000924C: 	LDI	R0,R1
    // asm 0000924D: 	OR	0FD00h,R1
    // asm 0000924E: 	OR	0FF00h,R0
    // asm 0000924F: 	STI	R0,@SOUND
    // asm 00009250: 	STI	R0,*AR3		;<-NEW SND2
    // asm 00009251: 	RPTS	SPACER
    // asm 00009252: 	NOP
    // asm 00009253: 	STI	R1,@SOUND
    // asm 00009254: 	RPTS	SPACER
    // asm 00009255: 	NOP
    // asm 00009256: 	STI	R0,@SOUND
    // asm 00009257: 	RPTS	SPACER
    // asm 00009258: 	NOP
    // asm 00009259: 	AND	0FFh,AR2
    // asm 0000925A: 	LDI	AR2,R0
    // asm 0000925B: 	LDI	AR2,R1
    // asm 0000925C: 	OR	0FF00h,R0	;byte 0
    // asm 0000925D: 	OR	0FD00h,R1
    // asm 0000925E: 	STI	R0,@SOUND
    // asm 0000925F: 	STI	R0,*AR3		;<-NEW SND2
    // asm 00009260: 	RPTS	SPACER
    // asm 00009261: 	NOP
    // asm 00009262: 	STI	R1,@SOUND
    // asm 00009263: 	RPTS	SPACER
    // asm 00009264: 	NOP
    // asm 00009265: 	STI	R0,@SOUND
    // asm 00009266: 	RPTS	SPACER
    // asm 00009267: 	NOP
    // asm 00009268: 	SETDP
    // asm 00009269: 	LDI	@DO_NOT_REENABLE_INT,R0
    // asm 0000926A: 	CALLZ	ENABLEGIE
    // asm 0000926B: 	POPM	R1,R0
    // asm 0000926D: 	POP	AR3
    // asm 0000926E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NIRM", 0, 0);
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
    // asm 0000926F: 	DINT
    // asm 00009275: 	LDP	@RESET_TIMER
    // asm 00009276: 	LDI	@RESET_TIMER,R0
    // asm 00009277: 	INC	R0
    // asm 00009278: 	STI	R0,@RESET_TIMER
    // asm 00009279: 	SETDP
    // asm 0000927A: 	CMPI	2,R0
    // asm 0000927B: 	BNE	NOT_F1
    // 	;first pass through
    // 	;new reset mumbo jumbo
    // 	;
    // 	;
    // asm 0000927C: 	LDI	@SYSCNTL,R0		;reset LOW for new board
    // asm 0000927D: 	LDI	R0,R1
    // asm 0000927E: 	ANDN	SND2_RESET,R0
    // asm 0000927F: 	STI	R0,@SYSCNTL
    // asm 00009280: 	LDP	@SYSCNTLR
    // asm 00009281: 	STI	R0,@SYSCNTLR
    // asm 00009282: 	PUSH	R1
    // 	;continued below
    // asm 00009283: 	LDP	@SOUND
    // asm 00009284: 	LDI	0FF00h,R0		;set wheel board addr
    // asm 00009285: 	LDI	0F700h,R1
    // asm 00009286: 	STPI	R0,@SOUND
    // asm 00009287: 	RPTS	50
    // asm 00009288: 	NOP
    // asm 00009289: 	STI	R1,@SOUND
    // asm 0000928A: 	RPTS	50
    // asm 0000928B: 	NOP
    // asm 0000928C: 	STI	R0,@SOUND
    // asm 0000928D: 	RPTS	50
    // asm 0000928E: 	NOP
    // asm 0000928F: 	LDI	0FF3Ch,R0		;set reset HI (old board)
    // asm 00009290: 	LDI	0FB3Ch,R1
    // asm 00009291: 	STI	R0,@SOUND
    // asm 00009292: 	RPTS	50
    // asm 00009293: 	NOP
    // asm 00009294: 	STI	R1,@SOUND
    // asm 00009295: 	RPTS	50
    // asm 00009296: 	NOP
    // asm 00009297: 	STI	R0,@SOUND
    // asm 00009298: 	RPTS	50
    // asm 00009299: 	NOP
    // asm 0000929A: 	RPTS	2500			;wait for 100 milliseconds
    // asm 0000929B: 	NOP
    // 	;new board from above
    // 	;
    // asm 0000929C: 	POP	R1			;set reset HIGH (new board)
    // asm 0000929D: 	LDP	@SYSCNTLR
    // asm 0000929E: 	STI	R1,@SYSCNTLR
    // asm 0000929F: 	SETDP
    // asm 000092A0: 	STI	R1,@SYSCNTL
    // 	;
    // asm 000092A1: 	LDP	@SOUND
    // asm 000092A2: 	LDI	0FF00h,R0		;set addr
    // asm 000092A3: 	LDI	0F700h,R1
    // asm 000092A4: 	STPI	R0,@SOUND
    // asm 000092A5: 	RPTS	50
    // asm 000092A6: 	NOP
    // asm 000092A7: 	STI	R1,@SOUND
    // asm 000092A8: 	RPTS	50
    // asm 000092A9: 	NOP
    // asm 000092AA: 	STI	R0,@SOUND
    // asm 000092AB: 	RPTS	50
    // asm 000092AC: 	NOP
    // asm 000092AD: 	LDI	0FF1Ch,R0		;set reset LOW (old board)
    // asm 000092AE: 	LDI	0FB1Ch,R1
    // asm 000092AF: 	STI	R0,@SOUND
    // asm 000092B0: 	RPTS	50
    // asm 000092B1: 	NOP
    // asm 000092B2: 	STI	R1,@SOUND
    // asm 000092B3: 	RPTS	50
    // asm 000092B4: 	NOP
    // asm 000092B5: 	STI	R0,@SOUND
    // asm 000092B6: 	RPTS	50
    // asm 000092B7: 	NOP
    // asm 000092B8: 	BU	RESETMUNGE_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESETMUNGE", 0, 0);
    UNIMPL();
}

void NOT_F1(void)
{
    // asm 000092B9: NOT_F2
    // asm 000092B9: 	CMPI	4,R0
    // asm 000092BA: 	BNE	NOT_F4
    // asm 000092BB: 	LDL	9A0000h,AR2	;SND2
    // asm 000092BC: 	LDP	@SOUND		;byte 1
    // asm 000092BD: 	LDI	0FD00h,R1	;send dummy byte to abort BONGs
    // asm 000092BE: 	LDI	0FF00h,R0
    // asm 000092BF: 	STI	R0,@SOUND
    // asm 000092C0: 	STI	R0,*AR2		;<-NEW SND2
    // asm 000092C1: 	RPTS	SPACER
    // asm 000092C2: 	NOP
    // asm 000092C3: 	STI	R1,@SOUND
    // asm 000092C4: 	RPTS	SPACER
    // asm 000092C5: 	NOP
    // asm 000092C6: 	STI	R0,@SOUND
    // asm 000092C7: 	BU	RESETMUNGE_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOT_F1", 0, 0);
    UNIMPL();
}

void NOT_F4(void)
{
    // asm 000092C8: 	CMPI	6,R0
    // asm 000092C9: 	BNE	NOT_F6
    // asm 000092CA: 	CLRI	R0
    // asm 000092CB: 	LDP	@IN_RESET_MODE
    // asm 000092CC: 	STI	R0,@IN_RESET_MODE
    // asm 000092CD: 	SETDP
    // asm 000092CE: 	READADJ	ADJ_VOLUME
    // asm 000092D0: 	LDI	R0,R1
    // asm 000092D1: 	CALL	SET_MASTER_VOL
    // 	;BU	RESETMUNGE_X
NOT_F6:
RESETMUNGE_X:
    // asm 000092D2: 	SETDP
    // asm 000092D3: 	CALL	ENABLEGIE
    // asm 000092D4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOT_F4", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SND_RESET_QUIET(void)
{
    // asm 000092D5: 	LDP	@RESET_TIMER
    // asm 000092D6: 	LDI	1,R0
    // asm 000092D7: 	STI	R0,@RESET_TIMER
    // asm 000092D8: 	STI	R0,@IN_RESET_MODE
    // asm 000092D9: 	SETDP
    // asm 000092DA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SND_RESET_QUIET", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CALLED IN DISPLAY INTERRUPT (16ms)
*
 */
void SNDPROC(void)
{
    // asm 000092DB: 	LDP	@IN_RESET_MODE
    // asm 000092DC: 	LDI	@IN_RESET_MODE,R0
    // asm 000092DD: 	SETDP
    // asm 000092DE: 	CMPI	1,R0
    // asm 000092DF: 	BEQ	RESETMUNGE
    // asm 000092E0: 	PUSH	R0
    // asm 000092E1: 	PUSH	R4
    // asm 000092E2: 	LDI	@SNDSTRI,AR0
    // asm 000092E3: 	LDI	NCHAN-1,R4
SNDLP:
    // asm 000092E4: 	LDI	*+AR0(SND_TMR),R0
    // asm 000092E5: 	BEQ	SNDPLP
    // asm 000092E6: 	DEC	R0
    // asm 000092E7: 	STI	R0,*+AR0(SND_TMR)
    // asm 000092E8: 	CALLZ	SNDUPD			;on ZERO CALL UPDATE
SNDPLP:
    // asm 000092E9: 	ADDI	SND_SIZ,AR0
    // asm 000092EA: 	DEC	R4
    // asm 000092EB: 	BGE	SNDLP
    // asm 000092EC: 	POP	R4
    // asm 000092ED: 	POP	R0
    // asm 000092EE: 	RETS
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
    // asm 000092EF: 	PUSH	AR1
    // asm 000092F0: 	LDI	*+AR0(SND_ADDR),AR1
    // asm 000092F1: 	LDI	*AR1++,R0
    // asm 000092F2: 	BZ	SNDCLRE			;NULL entry let us quit
    // asm 000092F3: 	STI	R0,*+AR0(SND_PRI)
    // asm 000092F4: 	LDI	*AR1,R0
    // asm 000092F5: 	LDI	R0,AR2
    // asm 000092F6: 	AND	07FFFh,AR2
    // asm 000092F7: 	LDI	*+AR0(SND_STATUS),R0
    // asm 000092F8: 	BZ	JUSTGO
    // asm 000092F9: 	INC	AR2
JUSTGO:
    // asm 000092FA: 	CALL	SENDSND			;do the actually sending
    // asm 000092FB: 	LDI	*AR1++,R0		;EXTRACT TIMING DATA
    // asm 000092FC: 	RS	16,R0
    // asm 000092FD: 	STI	R0,*+AR0(SND_TMR)	;start countdown
    // asm 000092FE: 	STI	AR1,*+AR0(SND_ADDR)
    // asm 000092FF: 	CMPI	0,R0
    // asm 00009300: 	BGT	SNDUPX
SNDCLRE:
    // asm 00009301: 	CLRI	R0
    // asm 00009302: 	STI	R0,*+AR0(SND_PRI)
    // asm 00009303: 	STI	R0,*+AR0(SND_ST)
    // asm 00009304: 	STI	R0,*+AR0(SND_STATUS)
SNDUPX:
    // asm 00009305: 	POP	AR1
    // asm 00009306: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SNDUPD", 0, 0);
    UNIMPL();
}

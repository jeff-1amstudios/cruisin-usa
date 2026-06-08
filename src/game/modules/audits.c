#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/macs.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/text.h"
#include "../include/dirq.h"
#include "../include/diag.h"
#include "../include/error.h"
#include "../include/audits.h"

/*
 * Source module: asm/AUDITS.ASM
 */

// ;	.FILE	"AUDITS.ASM"
// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *----------------------------------------------------------------------------
// *DISPLAYABLE AUDITS
// *
// *	.word	AUDIT_#,string
// *
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *AUDITS DISPLAY
// *
// *----------------------------------------------------------------------------

void _word(void)
{
    // asm: AUDITI:,l?
AUDITI:
    // asm: ,l?
    TRACE_EVENT(&g_crusn_machine->trace, "function", ".word", 0, 0);
    UNIMPL();
}

void COMPUTE_FREEGAMES_PRCNT(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *
    // *RETURN
    // *	R0	VALUE
    // *
    // asm: 	READAUD	AUD_GAMES_CONTINUES
    // asm: 	LDI	R0,R1
    // asm: 	READAUD	AUD_GAMES_START
    // asm: 	ADDI	R0,R1
    // asm: 	LDIZ	R1,R0
    // asm: 	RETSZ
    // asm: 	READAUD	AUD_TOTAL_FREEGAMES
    // ;	ADDI	R0,R1
    // asm: 	MPYI	100,R0
    // asm: 	CALL	DIV_I30
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMPUTE_FREEGAMES_PRCNT", 0, 0);
    UNIMPL();
}

void COMPUTE_GAMETIME(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *RETURN
    // *	R0	VALUE
    // *
    // asm: 	READAUD	AUD_GAMES_CONTINUES
    // asm: 	LDI	R0,R1
    // asm: 	READAUD	AUD_GAMES_START
    // asm: 	ADDI	R0,R1
    // asm: 	READAUD	AUD_GAMEON_TIME
    // asm: 	CALL	DIV_I30
    // asm: 	RETS
    // asm: AUDIT_LIST
    // asm: 	AUDENT	AUD_COIN1,"LEFT COIN"
    // asm: 	AUDENT	AUD_COIN2,"RIGHT COIN"
    // asm: 	AUDENT	AUD_COIN3,"THIRD COIN"
    // asm: 	AUDENT	AUD_COIN4,"FOURTH COIN"
    // asm: 	AUDENT	AUD_SERVICE_CREDITS,"SERVICE CREDITS"
    // asm: 	AUDENT_SPACER
    // asm: 	AUDENT	AUD_GAMES_START,"TOTAL STARTS"
    // asm: 	AUDENT	AUD_GAMES_CONTINUES,"TOTAL CONTINUES"
    // asm: 	AUDENT	AUD_H2HGAMES,"HEAD 2 HEAD GAMES"
    // asm: 	AUDENT	AUD_TOTAL_FREEGAMES,"FREE RACES"
    // asm: 	AUDENT	AUD_GAMES_EXPIRED,"EXPIRED RACES"
    // asm: 	AUDENTR	AUD_GAMEON_TIME,"FREE RACE PERCENTAGE",COMPUTE_FREEGAMES_PRCNT
    // asm: 	AUDENT_SPACER
    // ;	AUDENT	ADJ_DIFFICULTY,"DIFFICULTY"
    // ;	AUDENT	ADJ_FIRST_PLACE_PERCENT,"1ST PLACE AUTO PERCENT"
    // asm: 	AUDENT	AUD_POWERON_TIME,"POWER ON TIME SECS"
    // asm: 	AUDENT	AUD_GAMEON_TIME,"GAME TIME SECS"
    // asm: 	AUDENTR	AUD_GAMEON_TIME,"AVERAGE GAME TIME SECS",COMPUTE_GAMETIME
    // asm: 	AUDENT_PAGE
    // 	;-------
    // asm: 	AUDENT	AUD_AUTOMATIC_TRANS_SELECTED,"AUTOMATIC SELECTED"
    // asm: 	AUDENT	AUD_MANUAL_TRANS_SELECTED,"MANUAL SELECTED"
    // asm: 	AUDENT_SPACER
    // asm: 	AUDENT	AUD_VETTE_SELECTED,"63 MUSCLE SELECTED"
    // asm: 	AUDENT	AUD_LABOMBA_SELECTED,"LABOMBA SELECTED"
    // asm: 	AUDENT	AUD_DEVASTATOR_SELECTED,"DEVASTATOR SELECTED"
    // asm: 	AUDENT	AUD_ITALIA_SELECTED,"ITALIA SELECTED"
    // asm: 	AUDENT_SPACER
    // asm: 	AUDENT	AUD_HID_JEEP_SELECTED,"JEEP SELECTED"
    // asm: 	AUDENT	AUD_HID_COPCAR_SELECTED,"COPCAR SELECTED"
    // asm: 	AUDENT_SPACER
    // asm: 	AUDENT	AUD_VIEW1_TIME,"VIEW1 TIME SECS"
    // asm: 	AUDENT	AUD_VIEW2_TIME,"VIEW2 TIME SECS"
    // asm: 	AUDENT	AUD_VIEW3_TIME,"VIEW3 TIME SECS"
    // asm: 	AUDENT_PAGE
    // 	;-------
    // asm: 	AUDENT	AUD_LOG_COUNT,"EVENT"
    // asm: 	AUDENT	AUD_NUM_UNFINISHED,"U-GAMES"
    // asm: 	AUDENT	AUD_NUM_WATCHDOGS,"WATCHDOGS"
    // asm: 	AUDENT	AUD_SUCCESSFULL_BURNINS,"BURNIN LOOPS"
    // asm: 	AUDENT_SPACER
    // asm: 	AUDENT	ADJ_DIFFICULTY,"GAME DIFFICULTY"
    // ;	AUDENT	ADJ_DIFF_LOCAL,"LOCAL DIFFICULTY"
    // ;	AUDENT	ADJ_FIRST_PLACE_PERCENT,"1ST PLACE AUTOPERCENTAGE"
    // ;	AUDENT	ADJ_RAMP_PERCENTAGE,"RAMP PERCENTAGE"
    // ;	AUDENT	ADJ_RAMPGAMES,"AUTO RAMP GAMES"
    // ;	AUDENT	ADJ_RAMP_COUNT,"RAMP COUNT"
    // asm: 	AUDENT_PAGE
    // 	;-------
    // asm: 	AUDENT	AUD_FS_AZ,"SELECT ARIZONA"
    // asm: 	AUDENT	AUD_FS_US101,"SELECT US101"
    // asm: 	AUDENT	AUD_FS_LA,"SELECT LA"
    // asm: 	AUDENT	AUD_FS_IOWA,"SELECT IOWA"
    // asm: 	AUDENT	AUD_FS_DV,"SELECT DEATH VALLEY"
    // asm: 	AUDENT	AUD_FS_USA,"SELECT USA"
    // asm: 	AUDENT	AUD_FS_BH,"SELECT BEVHILLS"
    // asm: 	AUDENT	AUD_FS_APPL,"SELECT APPALACHIA"
    // asm: 	AUDENT	AUD_FS_RW,"SELECT REDWOOD"
    // asm: 	AUDENT	AUD_FS_RM,"SELECT RUSHMORE"
    // asm: 	AUDENT	AUD_FS_CH,"SELECT CHICAGO"
    // asm: 	AUDENT_SPACER
    // asm: 	AUDENT	AUD_FS_HID_SF,"SELECT SANFRAN"
    // asm: 	AUDENT	AUD_FS_HID_IN,"SELECT INDIANA"
    // asm: 	AUDENT	AUD_FS_HID_GG,"SELECT GGATE"
    // asm: 	AUDENT_PAGE
    // 	;-------
    // asm: 	AUDENT	AUD_START_GGATE,"START GGATE"
    // asm: 	AUDENT	AUD_START_SF,"START SF"
    // asm: 	AUDENT	AUD_START_US101,"START US101"
    // asm: 	AUDENT	AUD_START_REDWOOD,"START REDWOOD"
    // asm: 	AUDENT	AUD_START_BEVHILLS,"START BEVHILLS"
    // asm: 	AUDENT	AUD_START_LA,"START LA"
    // asm: 	AUDENT	AUD_START_DV,"START DV"
    // asm: 	AUDENT	AUD_START_ARIZONA,"START ARIZONA"
    // asm: 	AUDENT	AUD_START_GC,"START GC"
    // asm: 	AUDENT	AUD_START_IOWA,"START IOWA"
    // asm: 	AUDENT	AUD_START_CHICAGO,"START CHICAGO"
    // asm: 	AUDENT	AUD_START_INDIANA,"START INDIANA"
    // asm: 	AUDENT	AUD_START_APPALACHIA,"START APPALACHIA"
    // asm: 	AUDENT	AUD_START_DC,"START DC"
    // asm: 	AUDENT_PAGE
    // 	;-------
    // asm: 	AUDENT	AUD_WIN_GGATE,"GGATE WINS"
    // asm: 	AUDENT	AUD_WIN_SF,"SANFRAN WINS"
    // asm: 	AUDENT	AUD_WIN_US101,"US101 WINS"
    // asm: 	AUDENT	AUD_WIN_REDWOOD,"REDWOOD WINS"
    // asm: 	AUDENT	AUD_WIN_BEVHILLS,"BEVHILLS WINS"
    // asm: 	AUDENT	AUD_WIN_LA,"LA WINS"
    // asm: 	AUDENT	AUD_WIN_DV,"DEATHVALLEY WINS"
    // asm: 	AUDENT	AUD_WIN_ARIZONA,"ARIZONA WINS"
    // asm: 	AUDENT	AUD_WIN_GC,"GRAND CANYON WINS"
    // asm: 	AUDENT	AUD_WIN_IOWA,"IOWA WINS"
    // asm: 	AUDENT	AUD_WIN_CHICAGO,"CHICAGO WINS"
    // asm: 	AUDENT	AUD_WIN_INDIANA,"INDIANA WINS"
    // asm: 	AUDENT	AUD_WIN_APPALACHIA,"APPALACHIA WINS"
    // asm: 	AUDENT	AUD_WIN_DC,"DC WINS"
    // asm: 	AUDENT_PAGE
    // 	;-------
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMPUTE_GAMETIME", 0, 0);
    UNIMPL();
}

void AUDIT_DISPLAY(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	CLRI	AR4			;page index
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTITT	"HIT VOLUME UP FOR NEXT PAGE",50,330
    // asm: 	LDI	@SOFT_AUDIT,R0
    // asm: 	BNZ	KKLL
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTITT	"DIP SWITCH BANK 2 SELECTS COIN MODE",50,340
    // asm: 	CALL	TOEXIT
KKLL:
    // asm: IWINFR	10,40,400,220
    // asm: 	LDL	AUDIT_LIST,AR5
LYTL:
    // asm: 	FILL	12,42,398,218,COL_LGREY
    // asm: 	LDI	50,R5
    // asm: 	LDI	DEFAULT_COLOR,RC
LLTT:
    // asm: LDI	*AR5++,AR2
    // asm: 	CMPI	-1,AR2
    // asm: 	BEQ	LLRR
    // asm: 	CMPI	-2,AR2
    // asm: 	BEQ	KKLL
    // asm: 	CMPI	-3,AR2
    // asm: 	BEQ	JJTTA
    // asm: 	LDI	AR2,R0
    // asm: 	RS	16,R0
    // asm: 	CMPI	AUD_ROUTINE,R0
    // asm: 	BNE	NOCALL
    // asm: 	LDI	*AR5++,R0
    // asm: 	CALLU	R0
    // asm: 	BU	J22AB
NOCALL:
    // asm: CALL	AUDIT_READ
J22AB:
    // asm: LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: J22A
    // asm: 	LDI	R5,R3
    // asm: 	LDI	250,R2
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*AR5++,AR2
    // asm: 	LDI	R5,R3
    // asm: 	LDI	20,R2
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
JJTTA:
    // asm: ADDI	10,R5
    // asm: 	BU	LLTT
LLRR:
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@OLDDIP
WL2A:
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	LDI	@OLDDIP,R1
    // asm: 	CMPI	R0,R1
    // asm: 	BEQ	NODIPPLOT
    // asm: 	STI	R0,@OLDDIP
    // asm: 	LDI	50,AR2
    // asm: 	LDI	240,R2
    // asm: 	LDI	300,R3
    // asm: 	LDI	249,RC
    // asm: 	LDI	0,RS
    // asm: 	CALL	_fill
    // ;	LDI	@SOFT_AUDIT,R0
    // ;	BZ	KKL7
    // ;
    // ;	CALL	GETCOINTXT
    // ;	LDI	AR0,AR2
    // ;	LDI	DEFAULT_COLOR,RC
    // ;	LDI	50,R2
    // ;	LDI	240,R3
    // ;	LDI	R0,AR0
    // ;	CALL	_outtextxyc
    // ;
    // ;KKL7
NODIPPLOT:
    // asm: 	LDI	@_newbut,R0	;old system method
    // asm: 	LDI	@_newbut,R1		;debounce
    // asm: 	TSTB	SW_DIAG,R1		;start to return to main
    // asm: 	BZ	BOOKX
    // asm: 	RS	16,R1
    // asm: 	TSTB	SW_VIEW0_H,R1
    // asm: 	BNZ	NOLOG
    // asm: 	LDI	@SOFT_AUDIT,R0		;OPERATOR MAY NOT VIEW THESE
    // asm: 	BZ	NOLOG
    // asm: 	CALL	ERROR_LOG_DISPLAY
    // asm: 	BU	AUDIT_DISPLAY
NOLOG:
    // asm: 	TSTB	SW_VOLPLUS,R0
    // asm: 	BZ	DBNCE
    // ;	BZ	LYTL			;NEXT PAGE
    // asm: 	TSTB	SW_VOLMINUS,R0
    // asm: 	BNZ	FFB2
    // ;	BZ	LYTL			;NEXT PAGE
DBNCE:
    // asm: NOT	@_newbut,R1		;debounce
    // asm: 	AND	SW_VOLPLUS|SW_VOLMINUS,R1
    // asm: 	BNZ	DBNCE
    // asm: 	BU	LYTL
FFB2:
    // asm: TSTB	SW_VIEW2_H,R1		;view2 to display next audit page
    // asm: 	BNZ	KKLL5
RB44:
    // asm: LDI	@_newbut,R1		;debounce
    // asm: 	RS	16,R1
    // asm: 	TSTB	SW_VIEW2_H,R1		;view2 to display next audit page
    // asm: 	BZ	RB44
    // asm: 	BU	LYTL			;NEXT PAGE
KKLL5:
    // asm: 	LDI	@SOFT_AUDIT,R0
    // asm: 	BZ	KKL
    // asm: 	TSTB	SW_VIEW1_H,R1		;VIEW1 TO EXIT TO GAME (IN SECRET MODE)
    // asm: 	RETSZ
KKL:
    // asm: 	BU	WL2A
BOOKX:
WL266:
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_DIAG,R0
    // asm: 	BNZ	WL266
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_DISPLAY", 0, 0);
    UNIMPL();
}

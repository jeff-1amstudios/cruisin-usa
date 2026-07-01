
#include "audits.h"
#include "../core/machine.h"
#include "c30.h"
#include "cmos.h"
#include "diag.h"
#include "dirq.h"
#include "error.h"
#include "globals.h"
#include "macs.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/AUDITS.ASM
 */

static void COMPUTE_FREEGAMES_PRCNT(void);
static void COMPUTE_GAMETIME(void);
void AUDIT_DISPLAY(void);

extern const char EXPIRED[];

/*
;	.FILE	"AUDITS.ASM"
*----------------------------------------------------------------------------
*
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

/*
 *----------------------------------------------------------------------------
 *DISPLAYABLE AUDITS
 *
 *	.word	AUDIT_#,string
 *
 *
 */

#define AUD_ROUTINE 0x5A
#define AUD_ROUTINEH 0x5A0000

/*
 *----------------------------------------------------------------------------
 *
 *
 *RETURN
 *	R0	VALUE
 *
 */
static void COMPUTE_FREEGAMES_PRCNT(void) {
    // asm 00001419: 	READAUD	AUD_GAMES_CONTINUES
    // asm 0000141B: 	LDI	R0,R1
    // asm 0000141C: 	READAUD	AUD_GAMES_START
    // asm 0000141E: 	ADDI	R0,R1
    // asm 0000141F: 	LDIZ	R1,R0
    // asm 00001420: 	RETSZ
    // asm 00001421: 	READAUD	AUD_TOTAL_FREEGAMES
    // ;	ADDI	R0,R1
    // asm 00001423: 	MPYI	100,R0
    // asm 00001424: 	CALL	DIV_I30
    // asm 00001425: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMPUTE_FREEGAMES_PRCNT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *RETURN
 *	R0	VALUE
 *
 */
static void COMPUTE_GAMETIME(void) {
    // asm 00001426: 	READAUD	AUD_GAMES_CONTINUES
    // asm 00001428: 	LDI	R0,R1
    // asm 00001429: 	READAUD	AUD_GAMES_START
    // asm 0000142B: 	ADDI	R0,R1
    // asm 0000142C: 	READAUD	AUD_GAMEON_TIME
    // asm 0000142E: 	CALL	DIV_I30
    // asm 0000142F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMPUTE_GAMETIME", 0, 0);
    UNIMPL();
}

/* asm: AUDIT_LIST */
/* asm: AUDENT	AUD_COIN1,"LEFT COIN" */
/* asm: AUDENT	AUD_COIN2,"RIGHT COIN" */
/* asm: AUDENT	AUD_COIN3,"THIRD COIN" */
/* asm: AUDENT	AUD_COIN4,"FOURTH COIN" */
/* asm: AUDENT	AUD_SERVICE_CREDITS,"SERVICE CREDITS" */
/* asm: AUDENT_SPACER */
/* asm: AUDENT	AUD_GAMES_START,"TOTAL STARTS" */
/* asm: AUDENT	AUD_GAMES_CONTINUES,"TOTAL CONTINUES" */
/* asm: AUDENT	AUD_H2HGAMES,"HEAD 2 HEAD GAMES" */
/* asm: AUDENT	AUD_TOTAL_FREEGAMES,"FREE RACES" */
/* asm: AUDENT	AUD_GAMES_EXPIRED,"EXPIRED RACES" */
/* asm: AUDENTR	AUD_GAMEON_TIME,"FREE RACE PERCENTAGE",COMPUTE_FREEGAMES_PRCNT */
/* asm: AUDENT_SPACER */
/* asm: AUDENT	AUD_POWERON_TIME,"POWER ON TIME SECS" */
/* asm: AUDENT	AUD_GAMEON_TIME,"GAME TIME SECS" */
/* asm: AUDENTR	AUD_GAMEON_TIME,"AVERAGE GAME TIME SECS",COMPUTE_GAMETIME */
/* asm: AUDENT_PAGE */
/* asm: AUDENT	AUD_AUTOMATIC_TRANS_SELECTED,"AUTOMATIC SELECTED" */
/* asm: AUDENT	AUD_MANUAL_TRANS_SELECTED,"MANUAL SELECTED" */
/* asm: AUDENT_SPACER */
/* asm: AUDENT	AUD_VETTE_SELECTED,"63 MUSCLE SELECTED" */
/* asm: AUDENT	AUD_LABOMBA_SELECTED,"LABOMBA SELECTED" */
/* asm: AUDENT	AUD_DEVASTATOR_SELECTED,"DEVASTATOR SELECTED" */
/* asm: AUDENT	AUD_ITALIA_SELECTED,"ITALIA SELECTED" */
/* asm: AUDENT_SPACER */
/* asm: AUDENT	AUD_HID_JEEP_SELECTED,"JEEP SELECTED" */
/* asm: AUDENT	AUD_HID_COPCAR_SELECTED,"COPCAR SELECTED" */
/* asm: AUDENT_SPACER */
/* asm: AUDENT	AUD_VIEW1_TIME,"VIEW1 TIME SECS" */
/* asm: AUDENT	AUD_VIEW2_TIME,"VIEW2 TIME SECS" */
/* asm: AUDENT	AUD_VIEW3_TIME,"VIEW3 TIME SECS" */
/* asm: AUDENT_PAGE */
/* asm: AUDENT	AUD_LOG_COUNT,"EVENT" */
/* asm: AUDENT	AUD_NUM_UNFINISHED,"U-GAMES" */
/* asm: AUDENT	AUD_NUM_WATCHDOGS,"WATCHDOGS" */
/* asm: AUDENT	AUD_SUCCESSFULL_BURNINS,"BURNIN LOOPS" */
/* asm: AUDENT_SPACER */
/* asm: AUDENT	ADJ_DIFFICULTY,"GAME DIFFICULTY" */
/* asm: AUDENT_PAGE */
/* asm: AUDENT	AUD_FS_AZ,"SELECT ARIZONA" */
/* asm: AUDENT	AUD_FS_US101,"SELECT US101" */
/* asm: AUDENT	AUD_FS_LA,"SELECT LA" */
/* asm: AUDENT	AUD_FS_IOWA,"SELECT IOWA" */
/* asm: AUDENT	AUD_FS_DV,"SELECT DEATH VALLEY" */
/* asm: AUDENT	AUD_FS_USA,"SELECT USA" */
/* asm: AUDENT	AUD_FS_BH,"SELECT BEVHILLS" */
/* asm: AUDENT	AUD_FS_APPL,"SELECT APPALACHIA" */
/* asm: AUDENT	AUD_FS_RW,"SELECT REDWOOD" */
/* asm: AUDENT	AUD_FS_RM,"SELECT RUSHMORE" */
/* asm: AUDENT	AUD_FS_CH,"SELECT CHICAGO" */
/* asm: AUDENT_SPACER */
/* asm: AUDENT	AUD_FS_HID_SF,"SELECT SANFRAN" */
/* asm: AUDENT	AUD_FS_HID_IN,"SELECT INDIANA" */
/* asm: AUDENT	AUD_FS_HID_GG,"SELECT GGATE" */
/* asm: AUDENT_PAGE */
/* asm: AUDENT	AUD_START_GGATE,"START GGATE" */
/* asm: AUDENT	AUD_START_SF,"START SF" */
/* asm: AUDENT	AUD_START_US101,"START US101" */
/* asm: AUDENT	AUD_START_REDWOOD,"START REDWOOD" */
/* asm: AUDENT	AUD_START_BEVHILLS,"START BEVHILLS" */
/* asm: AUDENT	AUD_START_LA,"START LA" */
/* asm: AUDENT	AUD_START_DV,"START DV" */
/* asm: AUDENT	AUD_START_ARIZONA,"START ARIZONA" */
/* asm: AUDENT	AUD_START_GC,"START GC" */
/* asm: AUDENT	AUD_START_IOWA,"START IOWA" */
/* asm: AUDENT	AUD_START_CHICAGO,"START CHICAGO" */
/* asm: AUDENT	AUD_START_INDIANA,"START INDIANA" */
/* asm: AUDENT	AUD_START_APPALACHIA,"START APPALACHIA" */
/* asm: AUDENT	AUD_START_DC,"START DC" */
/* asm: AUDENT_PAGE */
/* asm: AUDENT	AUD_WIN_GGATE,"GGATE WINS" */
/* asm: AUDENT	AUD_WIN_SF,"SANFRAN WINS" */
/* asm: AUDENT	AUD_WIN_US101,"US101 WINS" */
/* asm: AUDENT	AUD_WIN_REDWOOD,"REDWOOD WINS" */
/* asm: AUDENT	AUD_WIN_BEVHILLS,"BEVHILLS WINS" */
/* asm: AUDENT	AUD_WIN_LA,"LA WINS" */
/* asm: AUDENT	AUD_WIN_DV,"DEATHVALLEY WINS" */
/* asm: AUDENT	AUD_WIN_ARIZONA,"ARIZONA WINS" */
/* asm: AUDENT	AUD_WIN_GC,"GRAND CANYON WINS" */
/* asm: AUDENT	AUD_WIN_IOWA,"IOWA WINS" */
/* asm: AUDENT	AUD_WIN_CHICAGO,"CHICAGO WINS" */
/* asm: AUDENT	AUD_WIN_INDIANA,"INDIANA WINS" */
/* asm: AUDENT	AUD_WIN_APPALACHIA,"APPALACHIA WINS" */
/* asm: AUDENT	AUD_WIN_DC,"DC WINS" */
/* asm: AUDENT_PAGE */
static int AUDIT_LIST;
// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *AUDITS DISPLAY
 *
 */
/* asm: OLDDIP	.bss	OLDDIP,1 */
int OLDDIP;

void AUDIT_DISPLAY(void) {
    // asm 000014D0: 	CALL	SETUP_DIAG_SCREEN
    // asm 000014D1: 	CLRI	AR4			;page index
    // asm 000014D2: 	LDI	DEFAULT_COLOR,RC
    // asm 000014D3: 	TEXTITT	"HIT VOLUME UP FOR NEXT PAGE",50,330
    // asm 000014D7: 	LDI	@SOFT_AUDIT,R0
    // asm 000014D8: 	BNZ	KKLL
    // asm 000014D9: 	LDI	DEFAULT_COLOR,RC
    // asm 000014DA: 	TEXTITT	"DIP SWITCH BANK 2 SELECTS COIN MODE",50,340
    // asm 000014DE: 	CALL	TOEXIT
KKLL:
    // asm 000014DF: IWINFR	10,40,400,220
    // asm 000014F7: 	LDL	AUDIT_LIST,AR5
LYTL:
    // asm 000014F8: 	FILL	12,42,398,218,COL_LGREY
    // asm 000014FE: 	LDI	50,R5
    // asm 000014FF: 	LDI	DEFAULT_COLOR,RC
LLTT:
    // asm 00001500: LDI	*AR5++,AR2
    // asm 00001501: 	CMPI	-1,AR2
    // asm 00001502: 	BEQ	LLRR
    // asm 00001503: 	CMPI	-2,AR2
    // asm 00001504: 	BEQ	KKLL
    // asm 00001505: 	CMPI	-3,AR2
    // asm 00001506: 	BEQ	JJTTA
    // asm 00001507: 	LDI	AR2,R0
    // asm 00001508: 	RS	16,R0
    // asm 00001509: 	CMPI	AUD_ROUTINE,R0
    // asm 0000150A: 	BNE	NOCALL
    // asm 0000150B: 	LDI	*AR5++,R0
    // asm 0000150C: 	CALLU	R0
    // asm 0000150D: 	BU	J22AB
NOCALL:
    // asm 0000150E: CALL	AUDIT_READ
J22AB:
    // asm 0000150F: LDI	R0,R2
    // asm 00001510: 	LDI	@bufferI,AR2
    // asm 00001511: 	CALL	_itoa
    // asm 00001512: J22A
    // asm 00001512: 	LDI	R5,R3
    // asm 00001513: 	LDI	250,R2
    // asm 00001514: 	LDI	DEFAULT_COLOR,RC
    // asm 00001515: 	CALL	_outtextxyc
    // asm 00001516: 	LDI	*AR5++,AR2
    // asm 00001517: 	LDI	R5,R3
    // asm 00001518: 	LDI	20,R2
    // asm 00001519: 	LDI	DEFAULT_COLOR,RC
    // asm 0000151A: 	CALL	_outtextxyc
JJTTA:
    // asm 0000151B: ADDI	10,R5
    // asm 0000151C: 	BU	LLTT
LLRR:
    // asm 0000151D: 	LDI	0,R0
    // asm 0000151E: 	STI	R0,@OLDDIP
WL2A:
    // asm 0000151F: 	LDI	@DIPRAM,R0
    // asm 00001520: 	LDI	@OLDDIP,R1
    // asm 00001521: 	CMPI	R0,R1
    // asm 00001522: 	BEQ	NODIPPLOT
    // asm 00001523: 	STI	R0,@OLDDIP
    // asm 00001524: 	LDI	50,AR2
    // asm 00001525: 	LDI	240,R2
    // asm 00001526: 	LDI	300,R3
    // asm 00001527: 	LDI	249,RC
    // asm 00001528: 	LDI	0,RS
    // asm 00001529: 	CALL	_fill
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
    // asm 0000152A: 	LDI	@_newbut,R0	;old system method
    // asm 0000152B: 	LDI	@_newbut,R1		;debounce
    // asm 0000152C: 	TSTB	SW_DIAG,R1		;start to return to main
    // asm 0000152D: 	BZ	BOOKX
    // asm 0000152E: 	RS	16,R1
    // asm 0000152F: 	TSTB	SW_VIEW0_H,R1
    // asm 00001530: 	BNZ	NOLOG
    // asm 00001531: 	LDI	@SOFT_AUDIT,R0		;OPERATOR MAY NOT VIEW THESE
    // asm 00001532: 	BZ	NOLOG
    // asm 00001533: 	CALL	ERROR_LOG_DISPLAY
    // asm 00001534: 	BU	AUDIT_DISPLAY
NOLOG:
    // asm 00001535: 	TSTB	SW_VOLPLUS,R0
    // asm 00001536: 	BZ	DBNCE
    // ;	BZ	LYTL			;NEXT PAGE
    // asm 00001537: 	TSTB	SW_VOLMINUS,R0
    // asm 00001538: 	BNZ	FFB2
    // ;	BZ	LYTL			;NEXT PAGE
DBNCE:
    // asm 00001539: NOT	@_newbut,R1		;debounce
    // asm 0000153A: 	AND	SW_VOLPLUS|SW_VOLMINUS,R1
    // asm 0000153B: 	BNZ	DBNCE
    // asm 0000153C: 	BU	LYTL
FFB2:
    // asm 0000153D: TSTB	SW_VIEW2_H,R1		;view2 to display next audit page
    // asm 0000153E: 	BNZ	KKLL5
RB44:
    // asm 0000153F: LDI	@_newbut,R1		;debounce
    // asm 00001540: 	RS	16,R1
    // asm 00001541: 	TSTB	SW_VIEW2_H,R1		;view2 to display next audit page
    // asm 00001542: 	BZ	RB44
    // asm 00001543: 	BU	LYTL			;NEXT PAGE
KKLL5:
    // asm 00001544: 	LDI	@SOFT_AUDIT,R0
    // asm 00001545: 	BZ	KKL
    // asm 00001546: 	TSTB	SW_VIEW1_H,R1		;VIEW1 TO EXIT TO GAME (IN SECRET MODE)
    // asm 00001547: 	RETSZ
KKL:
    // asm 00001548: 	BU	WL2A
BOOKX:
WL266:
    // asm 00001549: 	NOT	@_newbut,R0
    // asm 0000154A: 	AND	SW_DIAG,R0
    // asm 0000154B: 	BNZ	WL266
    // asm 0000154C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_DISPLAY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

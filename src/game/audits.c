#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "macs.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "text.h"
#include "dirq.h"
#include "diag.h"
#include "error.h"
#include "port.h"
#include "audits.h"

/*
 * Source module: asm/AUDITS.ASM
 */

void COMPUTE_FREEGAMES_PRCNT(void);
void COMPUTE_GAMETIME(void);
void AUDIT_DISPLAY(void);

#define AUD_ROUTINE 0x5A
#define AUD_ROUTINEH 0x5A0000

/* *----------------------------------------------------------------------------
*
*
*RETURN
*	R0	VALUE
*
 */
void COMPUTE_FREEGAMES_PRCNT(void)
{
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

/* *----------------------------------------------------------------------------
*
*
*RETURN
*	R0	VALUE
*
 */
void COMPUTE_GAMETIME(void)
{
    // asm: 	READAUD	AUD_GAMES_CONTINUES
    // asm 00001421: 	LDI	R0,R1
    // asm: 	READAUD	AUD_GAMES_START
    // asm 00001423: 	ADDI	R0,R1
    // asm: 	READAUD	AUD_GAMEON_TIME
    // asm 00001425: 	CALL	DIV_I30
    // asm: 	RETS
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
int AUDIT_LIST;
/* *----------------------------------------------------------------------------
*AUDITS DISPLAY
*
 */
/* asm: OLDDIP	.bss	OLDDIP,1 */
int OLDDIP;

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
    // asm 000014D0: IWINFR	10,40,400,220
    // asm: 	LDL	AUDIT_LIST,AR5
LYTL:
    // asm: 	FILL	12,42,398,218,COL_LGREY
    // asm 000014D1: 	LDI	50,R5
    // asm 000014D2: 	LDI	DEFAULT_COLOR,RC
LLTT:
    // asm 000014D3: LDI	*AR5++,AR2
    // asm: 	CMPI	-1,AR2
    // asm 000014D7: 	BEQ	LLRR
    // asm 000014D8: 	CMPI	-2,AR2
    // asm: 	BEQ	KKLL
    // asm 000014D9: 	CMPI	-3,AR2
    // asm 000014DA: 	BEQ	JJTTA
    // asm 000014DF: 	LDI	AR2,R0
    // asm: 	RS	16,R0
    // asm: 	CMPI	AUD_ROUTINE,R0
    // asm 000014F7: 	BNE	NOCALL
    // asm: 	LDI	*AR5++,R0
    // asm 000014F8: 	CALLU	R0
    // asm 000014FE: 	BU	J22AB
NOCALL:
    // asm 000014FF: CALL	AUDIT_READ
J22AB:
    // asm 00001501: LDI	R0,R2
    // asm 00001502: 	LDI	@bufferI,AR2
    // asm 00001503: 	CALL	_itoa
    // asm 00001504: J22A
    // asm 00001506: 	LDI	R5,R3
    // asm: 	LDI	250,R2
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm 00001507: 	CALL	_outtextxyc
    // asm 00001509: 	LDI	*AR5++,AR2
    // asm 0000150A: 	LDI	R5,R3
    // asm 0000150B: 	LDI	20,R2
    // asm 0000150C: 	LDI	DEFAULT_COLOR,RC
    // asm 0000150D: 	CALL	_outtextxyc
JJTTA:
    // asm 0000150E: ADDI	10,R5
    // asm 0000150E: 	BU	LLTT
LLRR:
    // asm 00001511: 	LDI	0,R0
    // asm: 	STI	R0,@OLDDIP
WL2A:
    // asm 00001513: 	LDI	@DIPRAM,R0
    // asm 00001514: 	LDI	@OLDDIP,R1
    // asm 00001515: 	CMPI	R0,R1
    // asm: 	BEQ	NODIPPLOT
    // asm 00001517: 	STI	R0,@OLDDIP
    // asm 00001519: 	LDI	50,AR2
    // asm 0000151A: 	LDI	240,R2
    // asm 0000151B: 	LDI	300,R3
    // asm 0000151C: 	LDI	249,RC
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
    // asm 00001527: 	LDI	@_newbut,R0	;old system method
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
    // asm 0000152A: 	BZ	DBNCE
    // ;	BZ	LYTL			;NEXT PAGE
    // asm: 	TSTB	SW_VOLMINUS,R0
    // asm 0000152B: 	BNZ	FFB2
    // ;	BZ	LYTL			;NEXT PAGE
DBNCE:
    // asm 0000152E: NOT	@_newbut,R1		;debounce
    // asm 0000152E: 	AND	SW_VOLPLUS|SW_VOLMINUS,R1
    // asm 0000152F: 	BNZ	DBNCE
    // asm 00001530: 	BU	LYTL
FFB2:
    // asm 00001531: TSTB	SW_VIEW2_H,R1		;view2 to display next audit page
    // asm 00001532: 	BNZ	KKLL5
RB44:
    // asm 00001534: LDI	@_newbut,R1		;debounce
    // asm: 	RS	16,R1
    // asm: 	TSTB	SW_VIEW2_H,R1		;view2 to display next audit page
    // asm 00001535: 	BZ	RB44
    // asm 00001536: 	BU	LYTL			;NEXT PAGE
KKLL5:
    // asm 00001538: 	LDI	@SOFT_AUDIT,R0
    // asm: 	BZ	KKL
    // asm: 	TSTB	SW_VIEW1_H,R1		;VIEW1 TO EXIT TO GAME (IN SECRET MODE)
    // asm 00001539: 	RETSZ
KKL:
    // asm 0000153B: 	BU	WL2A
    // asm 0000153D: BOOKX
WL266:
    // asm: 	NOT	@_newbut,R0
    // asm 0000153F: 	AND	SW_DIAG,R0
    // asm 00001540: 	BNZ	WL266
    // asm 00001541: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_DISPLAY", 0, 0);
    UNIMPL();
}

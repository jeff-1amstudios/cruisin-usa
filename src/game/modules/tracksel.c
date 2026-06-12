#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/obj.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/delta.h"
#include "../include/error.h"
#include "../include/comm.h"
#include "../include/tracksel.h"

/*
 * Source module: asm/TRACKSEL.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: TURNOBJ	.bss	TURNOBJ,1 */
int TURNOBJ;
// *----------------------------------------------------------------------------
// *
// *
// *
#define SMALL_CURSOR (PDATA+0)
#define BIG_CURSOR (PDATA+1)
#define OLD_INDEX (PDATA+2)
#define ROM_SMALL_CURSOR (PDATA+3)
#define ROM_BIG_CURSOR (PDATA+4)
#define LAST_HIDDEN_TRACK (PDATA+5)
/* asm: RAW_CHOOSENTRK	.bss	RAW_CHOOSENTRK,1 */
int RAW_CHOOSENTRK;
/* asm: CRUISE	SPTR	"CRUISE THE USA" */
const char *CRUISE = "CRUISE THE USA";
/* asm: CRUISE2	SPTR	"JAM WITH THE PRESIDENT" */
const char *CRUISE2 = "JAM WITH THE PRESIDENT";
/* asm: CRUISE3	SPTR	"IN THE WHITE HOUSE HOT TUB" */
const char *CRUISE3 = "IN THE WHITE HOUSE HOT TUB";
// *----------------------------------------------------------------------------
// *
// *
// *
// *
/* asm: LLG1	.word	L4A */
const char * *LLG1 = L4A;
/* asm: LLG2	.word	L5A */
const char * *LLG2 = L5A;
/* asm: LLG3	.word	L5B */
const char * *LLG3 = L5B;
/* asm: L88	.word	IBOIBOJ */
int *L88 = IBOIBOJ;
const char *L4A = "MOTION RECALIBRATION";
const char *L5A = "REMAIN SEATED";
const char *L5B = "DO NOT TOUCH MATS";
/* asm: IBOIBOJ	.bss	IBOIBOJ,2 */
int IBOIBOJ[2];
// 	;******
// 	;******
// 	;******  NORMAL GAME MODE...
// 	;******
// 	;******
// 	;Has the player made Pole Position?  (1)
// 	;T-> Branch over this Coin Grab
// 	;F-> Get more coin to continue
// 	;
// 	;*************************************************************
// 	;*************************************************************
// 	;SELECT TRACK MAIN LOOP
// 	;*************************************************************
// 	;*************************************************************
// 	;are we linked?
// 	;has the other player chosen a game
// 	;
// 	;-------time remaining
// 	;
// 	;HEAD2HEAD
// 	;
// 	;are we in linked game?
// 	;	F -> continue
// 	;has the other machine chosen the game?
// 	;	T->  accept the game
// 	;
// 	;HEAD2HEAD
// 	;
// 	;NOW TELL OTHER MACHINE:
// 	;
// 	;	that we are finished with selecting the race
// 	;	if we are subordinate dont???
// 	;
// 	;
// 	;IF we are a slave, then we must wait
// 	;for master to tell us what we chose
#if CDEBUG
#endif
// 	;THIS IS NOW MEANINGLESS
// 	;CLEAR THIS FOR FUTURE USE
// 	;
// 	;\\\  \\\  \\\  \\\  \\\  \\\
// 	;    \\\  \\\  \\\  \\\  \\\  \\\
// ;	;HEAD2HEAD
// ;	;
// ;	;NOW TELL OTHER MACHINE:
// ;	;
// ;	;	that we are finished with selecting the race
// ;	;	if we are subordinate dont???
// ;	;
// ;	;
// ;	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
// ;	STI	R0,@HIDDEN_ON
// ;	CALL	SEND_RACENUM
// 	;delete cursor
// 	;
#if DEBUG
#endif
// 	;********************************************************************
// 	;WAIT FOR DUAL PLAYER GAME HERE
// 	;except with the first game
// 	;
// 	;********************************************************************
// 	;
// 	;
// 	;********************************************************************
// 	;********************************************************************
// 	;OM race mode == RM_SINGLE <=>
// 	;	not racing across the USA already
// 	;else
// 	;	we have bought into an ongoing
// 	;	race game
// ;	BNE	NBIRAU
// ;	BU	NOHID1
// ;NBIRAU
/* asm: NOTUSA_RACE	NOTUSA_RACE
	;Translate race index into an actual
	;useable value.
	;
	;
	.bss	HIDDEN_ON,1 */
int NOTUSA_RACE[HIDDEN_ON];
// 	;this must happen before translation
// 	;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: TSEL_HEAD	.bss	TSEL_HEAD,1 */
int TSEL_HEAD;
/* asm: TSEL_THEONE	.bss	TSEL_THEONE,1 */
int TSEL_THEONE;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *
/* asm: ANIMATE_MAP_TAB	.word	mhop1,mhop2,mhop3,mhop4,-1 */
int ANIMATE_MAP_TAB[5] = {
    mhop1, mhop2, mhop3, mhop4, -1,
};
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: TURNANI	.word	whel1,whel2,whel3,whel4,whel3,whel2,-1 */
int TURNANI[7] = {
    whel1, whel2, whel3, whel4, whel3, whel2, -1,
};
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
#define TRACK_SELECTION_LIST_LEN 11
/* asm: TRACK_SELECTION_LISTI	.word	TRACK_SELECTION_LIST */
#define TRACK_SELECTION_LISTI TRACK_SELECTION_LIST
/* asm: TRACK_SELECTION_LIST */
/* asm: 	.word	RACE_AZ */
/* asm: 	.word	RACE_US101 */
/* asm: 	.word	RACE_LA */
/* asm: 	.word	RACE_IOWA */
/* asm: 	.word	RACE_DV */
/* asm: 	.word	RACE_USA */
/* asm: 	;	.word	RACE_DC */
/* asm: 	.word	RACE_BH */
/* asm: 	.word	RACE_APPL */
/* asm: 	.word	RACE_RW */
/* asm: 	.word	RACE_GC */
/* asm: 	.word	RACE_CH */
/* asm: 	;hidden races */
/* asm: 	.word	RACE_GG */
/* asm: 	.word	RACE_IN */
/* asm: 	.word	RACE_SF */
/* asm: 	*---------------------------------------------------------------------------- */
int TRACK_SELECTION_LIST[14] = {
    RACE_AZ,
    RACE_US101,
    RACE_LA,
    RACE_IOWA,
    RACE_DV,
    RACE_USA,
    RACE_BH,
    RACE_APPL,
    RACE_RW,
    RACE_GC,
    RACE_CH,
    RACE_GG,
    RACE_IN,
    RACE_SF,
};
// *----------------------------------------------------------------------------
/* asm: BOILEROBJ	.bss	BOILEROBJ,1 */
int BOILEROBJ;
// *----------------------------------------------------------------------------

void TRACK_SELECTION(void)
{
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	STI	R0,@START_HIT
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CLEARRDY	  	;READY FOR INTERRUPT
KK5:
    // asm: LDI	@CLEARRDY,R0
    // asm: 	BNZ	KK5
    // ;	CALL	PAL_INIT
    // asm: 	CALL	LOAD_FIXED_PALETTES
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,*+AR7(OLD_INDEX)
    // asm: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    // ;	CALL	CLEANUP_PALS
    // asm: 	LDL	crace_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	SLEEP	1
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // asm: 	LDL	_SECcrace,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
HOLD4LOAD:
    // asm: 	SLEEP	1
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm: 	BNZ	HOLD4LOAD
    // asm: 	LDL	shared_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	shared_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECshared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	OBJ_INIT
    // asm: 	CALL	BOILERPLATE_INIT
    // asm: 	LDI	@FIRST_RACE,R0
    // asm: 	BZ	NOT_RACE1
    // asm: 	LDL	_SECctrans,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	ctrans_PALETTES,AR2
    // asm: 	CALL	alloc_section
NOT_RACE1:
    // asm: 	LDP	@_CAMERAPOS
    // asm: 	CLRF	R0
    // asm: 	STF	R0,@_CAMERAPOS+X
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	FLOAT	-368,R0
    // asm: 	STF	R0,@_CAMERAPOS+Z
    // asm: 	SETDP
    // asm: 	LDI	TRACK_SELECTION_LIST_LEN+1,R0
    // asm: 	STI	R0,@POSES
    // asm: 	LDI	RM_SINGLE,R0
    // asm: 	STI	R0,@RACE_MODE
    // asm: 	LDL	racemap_grp,AR2
    // asm: 	CALL	LOAD_SINGLE_SECTION
    // asm: 	CALL	FIX_TRACK_SCREEN
    // asm: 	SONDFX	VIEW2IN
    // asm: 	LDI	6,R0
    // asm: 	STI	R0,@POSE
    // asm: 	JSRP	ZOOMINP
    // asm: 	READAUD	AUD_RESET_TOTALLY
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	JAJAD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRACK_SELECTION", 0, 0);
    UNIMPL();
}

void CLEANUP_TRACKSEL_PALS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	crace_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEANUP_TRACKSEL_PALS", 0, 0);
    UNIMPL();
}

void RESTORE_HIDDEN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *V1+V2 = GOLDEN GATE PARK  not
    // *V1+V3 = SAN FRANCISCO
    // *V2+V3 = INDIANA
    // *
    // *
    // *
    // *
    // *
    // *	R0	IDX {1,2,3}
    // *
    // asm: 	CMPI	-1,R0
    // asm: 	RETSEQ
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	LDI	401h,AR2
    // asm: 	CMPI	2,R0
    // asm: 	LDIEQ	406h,AR2
    // asm: 	CMPI	3,R0
    // asm: 	LDIEQ	409h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OCARBLK),R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESTORE_HIDDEN", 0, 0);
    UNIMPL();
}

void HIDDEN_TRACKS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm: 	RS	16,R0
    // asm: 	AND	SW_VIEW0_H|SW_VIEW1_H|SW_VIEW2_H,R0
    // asm: 	CMPI	SW_VIEW0_H|SW_VIEW1_H,R0
    // asm: 	BNE	NHT1
    // asm: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	HIDDTX
    // asm: 	CALL	RESTORE_HIDDEN
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    // 	;replace
    // asm: 	LDI	401h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	*+AR0(OROMDATA),R0
    // asm: 	STI	R0,*+AR0(OCARBLK)
    // asm: 	LDL	mpcesy,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	BU	HIDDTX
NHT1:
    // asm: CMPI	SW_VIEW0_H|SW_VIEW2_H,R0
    // asm: 	BNE	NHT2
    // asm: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm: 	CMPI	2,R0
    // asm: 	BEQ	HIDDTX
    // asm: 	CALL	RESTORE_HIDDEN
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    // 	;replace
    // asm: 	LDI	406h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	*+AR0(OROMDATA),R0
    // asm: 	STI	R0,*+AR0(OCARBLK)
    // asm: 	LDL	mpcmed,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	BU	HIDDTX
NHT2:
    // asm: CMPI	SW_VIEW1_H|SW_VIEW2_H,R0
    // asm: 	BNE	NHT3
    // asm: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm: 	CMPI	3,R0
    // asm: 	BEQ	HIDDTX
    // asm: 	CALL	RESTORE_HIDDEN
    // asm: 	LDI	3,R0
    // asm: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    // 	;replace
    // asm: 	LDI	409h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	409h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OROMDATA),R0
    // asm: 	STI	R0,*+AR0(OCARBLK)
    // asm: 	LDL	mpcexp,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	BU	HIDDTX
NHT3:
    // asm: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	HIDDTX
    // asm: 	CALL	RESTORE_HIDDEN
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
HIDDTX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIDDEN_TRACKS", 0, 0);
    UNIMPL();
}

void GET_RACE_IDX(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET THE RACE INDEX (POINTED TO BY WHEEL)
    // *
    // *
    // asm: 	CALL	GETCHOICE
    // asm: 	LDI	@POSE,R0
    // asm: 	CMPI	5,R0
    // asm: 	BLE	NDDR
    // asm: 	DEC	R0
NDDR:
    // asm: CMPI	@CHOSEN_RACE,R0
    // asm: 	BEQ	NSND
    // asm: 	PUSH	R0
    // asm: 	SONDFX	TRANS
    // asm: 	POP	R0
NSND:
    // asm: STI	R0,@CHOSEN_RACE
    // asm: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R1
    // asm: 	CMPI	-1,R1
    // asm: 	BEQ	NH
    // asm: 	CMPI	1,R0
    // asm: 	BNE	R55
    // asm: 	CMPI	1,R1		;Hidden golden gate
    // asm: 	LDIEQ	11,AR3
    // asm: 	BNE	NH
    // asm: 	BU	R66
R55:
    // asm: 	CMPI	6,R0
    // asm: 	BNE	R56
    // asm: 	CMPI	2,R1
    // asm: 	LDIEQ	12,AR3
    // asm: 	BNE	NH
    // asm: 	BU	R66
R56:
    // asm: 	CMPI	9,R0
    // asm: 	BNE	NH
    // asm: 	CMPI	3,R1
    // asm: 	LDIEQ	13,AR3
    // asm: 	BNE	NH
R66:
    // asm: 	LDI	@CHOSEN_RACE,R0
    // asm: 	LDI	R0,AR2
    // asm: 	BU	KK88
NH:
    // asm: 	LDI	@CHOSEN_RACE,AR2
    // asm: 	LDI	AR2,AR3
KK88:
    // asm: ADDI	400h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDF	*+AR0(OPOSX),R2
    // asm: 	FLOAT	256,R0
    // asm: 	ADDF	R0,R2
    // asm: 	LDF	*+AR0(OPOSY),R3
    // asm: 	FLOAT	180,R0
    // asm: 	ADDF	R0,R3
    // asm: 	LDI	AR3,R0
    // asm: 	MPYI	TS_SIZE,R0
    // asm: 	LDI	R0,AR0
    // asm: 	ADDI	@TRACK_SELECTION_LISTI,AR0
    // asm: 	LDI	*AR0,AR2
    // asm: 	ADDI	@LEG_NAMESI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET12FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	CMPI	5,AR3
    // asm: 	BNE	NOTTHEWH
    // asm: 	READAUD	ADJ_CLINTON
    // asm: 	CMPI	1,R0
    // asm: 	BNE	NOTTHEWH
    // asm: 	LDI	@CRUISE2,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	120,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET12FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDI	@CRUISE3,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	135,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET12FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOTTHEWH:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_RACE_IDX", 0, 0);
    UNIMPL();
}

void ZOOMINP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	20,AR4
ZITSP:
    // asm: CALL	ZOOMIN_TSEL
    // asm: 	LDI	@OM_CHOSEN_RACE,R0
    // asm: 	CMPI	-1,R0
    // asm: 	BNE	NOWAY543
    // asm: 	CALL	GET_RACE_IDX
    // asm: 	CALL	TRACK_SEL_CURSOR
NOWAY543:
    // asm: 	SLEEP	1
    // asm: 	DBU	AR4,ZITSP
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOMINP", 0, 0);
    UNIMPL();
}

void ZOOMIN_TSEL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@OACTIVE,AR5
ZISL:
    // asm: LDI	AR5,R0
    // asm: 	BZ	ZIX
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR5
    // asm: 	LDF	*+AR0(OVELZ),R0
    // asm: 	SUBF	*+AR0(OPOSZ),R0
    // asm: 	MPYF	0.42,R0	;.15
    // asm: 	CMPF	-5,R0
    // asm: 	BGT	JJDD
    // asm: 	ADDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	BU	HGF
JJDD:
    // asm: 	LDF	*+AR0(OVELZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	CLRF	R2
    // asm: 	STF	R2,*+AR0(ORADZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	BU	HHFFD
HGF:
    // asm: 	LDF	*+AR0(ORADZ),R2
    // asm: 	MPYF	0.65,R2
    // asm: 	STF	R2,*+AR0(ORADZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
HHFFD:
    // asm: 	BU	ZISL
ZIX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOMIN_TSEL", 0, 0);
    UNIMPL();
}

void FIX_TRACK_SCREEN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *DO THE NESSESARY MUMBO JUMBO, *AND* LINKEM _ALL_
    // *TOGETHER VIA OLINK2
    // *
    // *
    // *
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@TSEL_HEAD
    // asm: 	LDI	-1,R6
    // asm: 	LDI	@OACTIVE,AR5
FTSL:
    // asm: LDI	AR5,R0
    // asm: 	BZ	FTSLX
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR5		;NEXT OBJECT
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OVELX)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OVELY)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OVELZ)
    // asm: 	RANDN	28000
    // asm: 	FLOAT	R0
    // asm: 	ADDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDF	6,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR0(ORADZ)
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	CMPI	040Ch,R0	;SMALL CURSOR
    // asm: 	BNE	NSC
    // asm: 	STI	AR0,*+AR7(SMALL_CURSOR)
    // asm: 	LDI	*+AR0(OROMDATA),R0
    // asm: 	STI	R0,*+AR7(ROM_SMALL_CURSOR)
    // asm: 	CMPI	-1,R6
    // asm: 	LDIEQ	AR0,R6
    // asm: 	STI	AR5,*+AR0(OLINK2)
    // asm: 	BU	FTSL
NSC:
    // asm: 	CMPI	8A1h,R0
    // asm: 	BNE	NTRM1
    // asm: 	LDI	AR0,AR4
    // asm: 	CREATE	CYCLE_PUSH,UTIL_C|CHOOSERACE_T
    // asm: 	LDI	AR4,AR0
    // asm: 	CMPI	-1,R6
    // asm: 	LDIEQ	AR0,R6
    // asm: 	STI	AR5,*+AR0(OLINK2)
    // asm: 	BU	FTSL
NTRM1:
    // asm: 	CMPI	8A2h,R0
    // asm: 	BNE	NWHEL1
    // asm: 	LDI	AR0,AR4
    // asm: 	CREATE	TURNTO_SELECT,UTIL_C|CHOOSERACE_T
    // asm: 	LDI	AR4,AR0
    // asm: 	CMPI	-1,R6
    // asm: 	LDIEQ	AR0,R6
    // asm: 	STI	AR5,*+AR0(OLINK2)
    // asm: 	BU	FTSL
NWHEL1:
    // asm: 	CMPI	0410h,R0
    // asm: 	BNE	NBC
    // asm: 	STI	AR0,*+AR7(BIG_CURSOR)
    // asm: 	LDI	*+AR0(OROMDATA),R0
    // asm: 	STI	R0,*+AR7(ROM_BIG_CURSOR)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	CMPI	-1,R6
    // asm: 	LDIEQ	AR0,R6
    // asm: 	STI	AR5,*+AR0(OLINK2)
    // asm: 	BU	FTSL
NBC:
    // asm: 	CMPI	0405h,R0
    // asm: 	BNE	NAS
    // asm: 	PUSH	AR0
    // asm: 	LDI	AR0,AR4
    // asm: 	CREATE	ANIMATE_MAP,UTIL_C|CHOOSERACE_T
    // asm: 	POP	AR0
    // asm: 	CMPI	-1,R6
    // asm: 	LDIEQ	AR0,R6
    // asm: 	STI	AR5,*+AR0(OLINK2)
    // asm: 	BU	FTSL
NAS:
    // asm: 	CMPI	40Eh,R0
    // asm: 	BNE	NN77
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	BU	FTSL
NN77:
    // asm: 	CMPI	40Fh,R0
    // asm: 	BNE	NN88
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	BU	FTSL
NN88:
    // asm: 	CMPI	-1,R6
    // asm: 	LDIEQ	AR0,R6
    // asm: 	STI	AR5,*+AR0(OLINK2)
    // asm: 	BU	FTSL
FTSLX:
    // asm: 	STI	R6,@TSEL_HEAD
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIX_TRACK_SCREEN", 0, 0);
    UNIMPL();
}

void TSEL_ZOOMOUTP(void)
{
    // asm: 	SONDFX	VIEW2IN
    // asm: 	LDI	@RACE_MODE,R0
    // asm: 	CMPI	RM_USA,R0
    // asm: 	BNE	JJKKD
    // asm: 	LDI	5,AR2
    // asm: 	BU	LLGF
JJKKD:
    // asm: LDI	@RAW_CHOOSENTRK,AR2
LLGF:
    // asm: LDI	AR2,AR3
    // asm: 	ADDI	400h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	STI	AR0,@TSEL_THEONE
    // asm: 	CREATEC	CENTER_THEONE,UTIL_C
    // asm: 	LDI	30,AR5
TSLLP:
    // asm: CALL	TSEL_ZOOMOUT
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,TSLLP
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TSEL_ZOOMOUTP", 0, 0);
    UNIMPL();
}

void CENTER_THEONE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@TSEL_THEONE,AR4
    // asm: 	LDI	15,AR5
CTOLP:
    // asm: LDF	*+AR4(OPOSX),R0
    // asm: 	MPYF	0.8,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	MPYF	0.8,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CALL	CENTER_SUB1
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,CTOLP
    // asm: 	LDI	15,AR5
CTOLPB:
    // asm: LDF	*+AR4(OPOSZ),R0
    // asm: 	MPYF	0.08,R0
    // asm: 	CMPF	50,R0
    // asm: 	LDFLT	50,R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	CALL	CENTER_SUB1
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,CTOLPB
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CENTER_THEONE", 0, 0);
    UNIMPL();
}

void CENTER_SUB1(void)
{
    // asm: 	LDF	*+AR4(OPOSX),R2
    // asm: 	FLOAT	256,R0
    // asm: 	ADDF	R0,R2
    // asm: 	LDF	*+AR4(OPOSY),R3
    // asm: 	FLOAT	180,R0
    // asm: 	ADDF	R0,R3
    // asm: 	LDI	@RACE_MODE,R0
    // asm: 	CMPI	RM_USA,R0
    // asm: 	BNE	K89
    // asm: 	LDI	14,AR2
    // asm: 	BU	K99
K89:
    // asm: LDI	@CHOSEN_RACE,AR2
K99:
    // asm: ADDI	@LEG_NAMESI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET12FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CENTER_SUB1", 0, 0);
    UNIMPL();
}

void TSEL_ZOOMOUT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@TSEL_HEAD,R0
    // asm: 	BU	LKGG
JJKK:
    // asm: LDI	*+AR0(OLINK2),R0
    // asm: 	BZ	ALLDN
LKGG:
    // asm: LDI	R0,AR0
    // asm: 	CMPI	@TSEL_THEONE,R0
    // asm: 	BEQ	JJKK			;skip our choice
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	MPYF	0.1,R0
    // asm: 	ADDF	*+AR0(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	MPYF	0.1,R0
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	MPYF	0.08,R0
    // asm: 	CMPF	50,R0
    // asm: 	LDFLT	50,R0
    // asm: 	ADDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	BU	JJKK
ALLDN:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TSEL_ZOOMOUT", 0, 0);
    UNIMPL();
}

void ANIMATE_MAP(void)
{
    // *
    // asm: 	LDI	@ANIMATE_MAP_TABI,AR5
AMLP:
    // asm: 	LDI	*AR5++,R0
    // asm: 	BN	ANIMATE_MAP
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	SLEEP	4
    // asm: 	BU	AMLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ANIMATE_MAP", 0, 0);
    UNIMPL();
}

void TRACK_SEL_CURSOR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // asm: 	LDI	*+AR7(OLD_INDEX),R0
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	CMPI	R0,AR0
    // asm: 	RETSEQ
    // asm: 	STI	AR0,*+AR7(OLD_INDEX)
    // asm: 	LDI	AR0,AR3
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	400h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	CMPI	5,AR3
    // asm: 	LDIEQ	*+AR7(ROM_BIG_CURSOR),R0
    // asm: 	LDINE	*+AR7(ROM_SMALL_CURSOR),R0
    // asm: 	LDI	*+AR7(SMALL_CURSOR),AR1
    // asm: 	STI	R0,*+AR1(OROMDATA)
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	STF	R0,*+AR1(OPOSX)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR1(OPOSY)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	SUBF	8,R0
    // asm: 	STF	R0,*+AR1(OPOSZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRACK_SEL_CURSOR", 0, 0);
    UNIMPL();
}

void GET_TURNOBJ(void)
{
    // *	R6	X
    // *	R7	Y
    // *	AR5	Z
    // *
    // asm: 	FLOAT	AR5,R5
    // asm: 	LDL	TURNANI,AR5
    // asm: 	CALL	OBJ_GET
    // asm: 	LDI	AR0,AR4
    // asm: 	STF	R6,*+AR4(OPOSX)
    // asm: 	STF	R7,*+AR4(OPOSY)
    // asm: 	STF	R5,*+AR4(OPOSZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
GETTOLP:
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0	;<-this mode
    // asm: 	CMPI	MINTRO,R0
    // asm: 	BNE	SUICIDE
    // asm: 	LDI	*AR5++,R0
    // asm: 	BNN	JIJI
    // asm: 	LDL	TURNANI,AR5
    // asm: 	LDI	*AR5++,R0
JIJI:
    // asm: STI	R0,*+AR4(OROMDATA)
    // asm: 	SLEEP	7
    // asm: 	BU	GETTOLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TURNOBJ", 0, 0);
    UNIMPL();
}

void BOILERPLATE_INIT(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDL	next,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	0,R3
    // asm: 	LDI	0,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	STI	AR0,@BOILEROBJ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOILERPLATE_INIT", 0, 0);
    UNIMPL();
}

void BOILERPLATE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*+AR7(SMALL_CURSOR),AR2
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	STF	R0,*+AR2(OCARBLK)
    // asm: 	FLOAT	-6000,R0
    // asm: 	STF	R0,*+AR2(OPOSX)
    // asm: 	LDI	@BOILEROBJ,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOILERPLATE", 0, 0);
    UNIMPL();
}

void BOILERPLATE_DELETE(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@BOILEROBJ,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	*+AR7(SMALL_CURSOR),AR2
    // asm: 	LDF	*+AR2(OCARBLK),R0
    // asm: 	STF	R0,*+AR2(OPOSX)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOILERPLATE_DELETE", 0, 0);
    UNIMPL();
}

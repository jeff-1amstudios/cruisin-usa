#include "../core/cpu.h"
#include "../core/machine.h"
#include "obj.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "delta.h"
#include "error.h"
#include "comm.h"
#include "tracksel.h"
#include "discovered_labels.h"

/*
 * Source module: asm/TRACKSEL.ASM
 */

void TRACK_SELECTION(void);
void JJKKA(void);
void JJ2(void);
void CLEANUP_TRACKSEL_PALS(void);
void RESTORE_HIDDEN(void);
void HIDDEN_TRACKS(void);
void NHT1(void);
void NHT2(void);
void GET_RACE_IDX(void);
void ZOOMINP(void);
void ZOOMIN_TSEL(void);
void FIX_TRACK_SCREEN(void);
void TSEL_ZOOMOUTP(void);
void JJKKD(void);
void CENTER_THEONE(void);
void CENTER_SUB1(void);
void K89(void);
void TSEL_ZOOMOUT(void);
void JJKK(void);
void ANIMATE_MAP(void);
void TRACK_SEL_CURSOR(void);
void GET_TURNOBJ(void);
void BOILERPLATE_INIT(void);
void BOILERPLATE(void);
void BOILERPLATE_DELETE(void);

/* asm: TURNOBJ	.bss	TURNOBJ,1 */
int TURNOBJ;
/* *----------------------------------------------------------------------------
*
*
*
 */
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
/* asm: LLG1	.word	L4A */
const char *LLG1 = L4A;
/* asm: LLG2	.word	L5A */
const char *LLG2 = L5A;
/* asm: LLG3	.word	L5B */
const char *LLG3 = L5B;
/* asm: L88	.word	IBOIBOJ */
int *L88 = IBOIBOJ;
/* asm: IBOIBOJ	.bss	IBOIBOJ,2 */
int IBOIBOJ[2];
#if CDEBUG
#endif
/* asm: HIDDEN_ON	.bss	HIDDEN_ON,1 */
int HIDDEN_ON;
/* *----------------------------------------------------------------------------
 */
/* asm: TSEL_HEAD	.bss	TSEL_HEAD,1 */
int TSEL_HEAD;
/* asm: TSEL_THEONE	.bss	TSEL_THEONE,1 */
int TSEL_THEONE;
/* *----------------------------------------------------------------------------
*
*
 */
/* asm: ANIMATE_MAP_TAB	.word	mhop1,mhop2,mhop3,mhop4,-1 */
int ANIMATE_MAP_TAB[] = {
    mhop1, mhop2, mhop3, mhop4, -1,
};
/* *----------------------------------------------------------------------------
 */
/* asm: TURNANI	.word	whel1,whel2,whel3,whel4,whel3,whel2,-1 */
int TURNANI[] = {
    whel1, whel2, whel3, whel4, whel3, whel2, -1,
};
/* *----------------------------------------------------------------------------
 */
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
/* asm: 	.word	RACE_BH */
/* asm: 	.word	RACE_APPL */
/* asm: 	.word	RACE_RW */
/* asm: 	.word	RACE_GC */
/* asm: 	.word	RACE_CH */
/* asm: 	.word	RACE_GG */
/* asm: 	.word	RACE_IN */
/* asm: 	.word	RACE_SF */
int TRACK_SELECTION_LIST[] = {
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
/* *----------------------------------------------------------------------------
 */
/* asm: BOILEROBJ	.bss	BOILEROBJ,1 */
int BOILEROBJ;

/* *----------------------------------------------------------------------------
*
*
*
*
 */
void TRACK_SELECTION(void)
{
    // asm 00006CBA: 	CLRI	R0
    // asm 00006CBB: 	STI	R0,@NOAERASE
    // asm 00006CBC: 	STI	R0,@START_HIT
    // asm 00006CBD: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 00006CC2: 	DMA_WT		R0
    // asm 00006CC7: 	LDI	1,R0
    // asm 00006CC8: 	STI	R0,@CLEARRDY	  	;READY FOR INTERRUPT
KK5:
    // asm 00006CC9: LDI	@CLEARRDY,R0
    // asm 00006CCA: 	BNZ	KK5
    // ;	CALL	PAL_INIT
    // asm 00006CCB: 	CALL	LOAD_FIXED_PALETTES
    // asm 00006CCC: 	LDI	-1,R0
    // asm 00006CCD: 	STI	R0,*+AR7(OLD_INDEX)
    // asm 00006CCE: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    // ;	CALL	CLEANUP_PALS
    // asm 00006CCF: 	LDL	crace_PALETTES,AR2
    // asm 00006CD0: 	CALL	alloc_section
    // asm 00006CD1: 	SLEEP	1
    // asm 00006CD3: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 00006CD8: 	DMA_WT		R0
    // asm 00006CDD: 	CALL	FIFO_RESET
    // asm 00006CDE: 	LDL	_SECcrace,AR2
    // asm 00006CDF: 	CALL	LOAD_SECTION_REQ
HOLD4LOAD:
    // asm 00006CE0: 	SLEEP	1
    // asm 00006CE2: 	LDI	@DECOMP_ACTIVE,R0
    // asm 00006CE3: 	BNZ	HOLD4LOAD
    // asm 00006CE4: 	LDL	shared_PALETTES,AR2
    // asm 00006CE5: 	CALL	dealloc_section
    // asm 00006CE6: 	LDL	shared_PALETTES,AR2
    // asm 00006CE7: 	CALL	alloc_section
    // asm 00006CE8: 	LDL	_SECshared,AR2
    // asm 00006CE9: 	CALL	LOAD_SECTION_REQ
    // asm 00006CEA: 	CALL	OBJ_INIT
    // asm 00006CEB: 	CALL	BOILERPLATE_INIT
    // asm 00006CEC: 	LDI	@FIRST_RACE,R0
    // asm 00006CED: 	BZ	NOT_RACE1
    // asm 00006CEE: 	LDL	_SECctrans,AR2
    // asm 00006CEF: 	CALL	LOAD_SECTION_REQ
    // asm 00006CF0: 	LDL	ctrans_PALETTES,AR2
    // asm 00006CF1: 	CALL	alloc_section
NOT_RACE1:
    // asm 00006CF2: 	LDP	@_CAMERAPOS
    // asm 00006CF3: 	CLRF	R0
    // asm 00006CF4: 	STF	R0,@_CAMERAPOS+X
    // asm 00006CF5: 	STF	R0,@_CAMERAPOS+Y
    // asm 00006CF6: 	FLOAT	-368,R0
    // asm 00006CF7: 	STF	R0,@_CAMERAPOS+Z
    // asm 00006CF8: 	SETDP
    // asm 00006CF9: 	LDI	TRACK_SELECTION_LIST_LEN+1,R0
    // asm 00006CFA: 	STI	R0,@POSES
    // asm 00006CFB: 	LDI	RM_SINGLE,R0
    // asm 00006CFC: 	STI	R0,@RACE_MODE
    // asm 00006CFD: 	LDL	racemap_grp,AR2
    // asm 00006CFE: 	CALL	LOAD_SINGLE_SECTION
    // asm 00006CFF: 	CALL	FIX_TRACK_SCREEN
    // asm 00006D00: 	SONDFX	VIEW2IN
    // asm 00006D02: 	LDI	6,R0
    // asm 00006D03: 	STI	R0,@POSE
    // asm 00006D04: 	JSRP	ZOOMINP
    // asm 00006D0A: 	READAUD	AUD_RESET_TOTALLY
    // asm 00006D0C: 	CMPI	0,R0
    // asm 00006D0D: 	BEQ	JAJAD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRACK_SELECTION", 0, 0);
    UNIMPL();
}

void JJKKA(void)
{
    // asm 00006DCF: LDI	*+AR0(OLINK2),R0
#if DEBUG
    // asm: 	BZ	$
#endif
    // asm 00006DD0: 	BZ	ALLDNA
LKGGA:
    // asm 00006DD1: LDI	R0,AR0
    // asm 00006DD2: 	CMPI	*+AR0(OLINK2),AR4
    // asm 00006DD3: 	BNE	JJKKA
    // asm 00006DD4: 	STI	AR5,*+AR0(OLINK2)
ALLDNA:
    // asm 00006DD5: 	LDI	*+AR7(SMALL_CURSOR),AR2
    // asm 00006DD6: 	CALL	OBJ_DELETE
    // 	;********************************************************************
    // 	;WAIT FOR DUAL PLAYER GAME HERE
    // 	;except with the first game
    // 	;
    // 	;********************************************************************
    // 	;
    // asm 00006DD7: 	LDI	@FIRST_RACE,R0
    // asm 00006DD8: 	BNZ	KAKAF
    // asm 00006DD9: 	JSRP	WAIT_FOR_CHALLENGER
KAKAF:
    // 	;
    // 	;********************************************************************
    // 	;********************************************************************
    // 	;OM race mode == RM_SINGLE <=>
    // 	;	not racing across the USA already
    // 	;else
    // 	;	we have bought into an ongoing
    // 	;	race game
    // asm 00006DDF: 	LDI	@RACE_MODE,R0
    // asm 00006DE0: 	CMPI	RM_USA,R0
    // asm 00006DE1: 	BEQ	BOUGHRAU
    // ;	BNE	NBIRAU
    // ;	BU	NOHID1
    // ;NBIRAU
    // asm 00006DE2: 	LDI	@CHOSEN_RACE,R0
    // asm 00006DE3: 	CMPI	5,R0
    // asm 00006DE4: 	LDIEQ	RM_USA,R1
    // asm 00006DE5: 	LDINE	RM_SINGLE,R1
    // asm 00006DE6: 	STI	R1,@RACE_MODE
    // asm 00006DE7: 	CMPI	RM_USA,R1
    // asm 00006DE8: 	BNE	NOTUSA_RACE
    // asm 00006DE9: 	INCAUD	AUD_START_GGATE
    // asm 00006DEB: 	INCAUD	AUD_FS_USA
    // asm 00006DED: 	LDI	0,AR2
    // asm 00006DEE: 	BU	JJ2
NOTUSA_RACE:
    // 	;Translate race index into an actual
    // 	;useable value.
    // 	;
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JJKKA", 0, 0);
    UNIMPL();
}

void JJ2(void)
{
    // asm 00006E0A: STI	AR2,@CHOSEN_RACE
    // asm 00006E0B: BOUGHRAU
    // asm 00006E0B: 	JSRP	TSEL_ZOOMOUTP
    // asm 00006E11: 	CLRI	R0
    // asm 00006E12: 	STI	R0,@NOAERASE
    // asm 00006E13: 	CALL	OBJ_INIT
    // asm 00006E14: 	SLEEP	2
    // asm 00006E16: 	CALL	CLEANUP_TRACKSEL_PALS
    // asm 00006E17: 	CALL	CLEANUP_PALS
    // asm 00006E18: 	LDI	@CHOSEN_RACE,AR0
    // asm 00006E19: 	ADDI	@RACE_STARTING_POINTSI,AR0
    // asm 00006E1A: 	LDI	*AR0,R0
    // asm 00006E1B: 	STI	R0,@STARTSECTION
    // asm 00006E1C: 	CALL	FIND_STARTING_VALUES
    // asm 00006E1D: 	LDI	UTIL_C|CHOOSERACE_T,R0
    // asm 00006E1E: 	LDI	-1,R1
    // asm 00006E1F: 	CALL	PRC_KILLALL
    // asm 00006E20: 	LDI	@CHOSEN_RACE,R0
    // asm 00006E21: 	STI	R0,@BONUS_WAVE		;Set bonus wave 'cause noby else does until you finish a wave
    // asm 00006E22: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JJ2", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void CLEANUP_TRACKSEL_PALS(void)
{
    // asm 00006E26: 	LDL	crace_PALETTES,AR2
    // asm 00006E27: 	CALL	dealloc_section
    // asm 00006E28: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEANUP_TRACKSEL_PALS", 0, 0);
    UNIMPL();
}

/* *
*
*	R0	IDX {1,2,3}
*
 */
void RESTORE_HIDDEN(void)
{
    // asm 00006E29: 	CMPI	-1,R0
    // asm 00006E2A: 	RETSEQ
    // asm 00006E2B: 	PUSH	AR0
    // asm 00006E2C: 	PUSH	AR2
    // asm 00006E2D: 	LDI	401h,AR2
    // asm 00006E2E: 	CMPI	2,R0
    // asm 00006E2F: 	LDIEQ	406h,AR2
    // asm 00006E30: 	CMPI	3,R0
    // asm 00006E31: 	LDIEQ	409h,AR2
    // asm 00006E32: 	CALL	OBJ_FIND_FIRST
    // asm 00006E33: 	LDI	*+AR0(OCARBLK),R0
    // asm 00006E34: 	STI	R0,*+AR0(OROMDATA)
    // asm 00006E35: 	POP	AR2
    // asm 00006E36: 	POP	AR0
    // asm 00006E37: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESTORE_HIDDEN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void HIDDEN_TRACKS(void)
{
    // asm 00006E38: 	LDI	@SWITCHBUTS,R0
    // asm 00006E39: 	RS	16,R0
    // asm 00006E3A: 	AND	SW_VIEW0_H|SW_VIEW1_H|SW_VIEW2_H,R0
    // asm 00006E3B: 	CMPI	SW_VIEW0_H|SW_VIEW1_H,R0
    // asm 00006E3C: 	BNE	NHT1
    // asm 00006E3D: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006E3E: 	CMPI	1,R0
    // asm 00006E3F: 	BEQ	HIDDTX
    // asm 00006E40: 	CALL	RESTORE_HIDDEN
    // asm 00006E41: 	LDI	1,R0
    // asm 00006E42: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    // 	;replace
    // asm 00006E43: 	LDI	401h,AR2
    // asm 00006E44: 	CALL	OBJ_FIND_FIRST
    // asm 00006E45: 	LDI	*+AR0(OFLAGS),R0
    // asm 00006E46: 	ANDN	O_1PAL,R0
    // asm 00006E47: 	STI	R0,*+AR0(OFLAGS)
    // asm 00006E48: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006E49: 	STI	R0,*+AR0(OCARBLK)
    // asm 00006E4A: 	LDL	mpcesy,R0
    // asm 00006E4B: 	STI	R0,*+AR0(OROMDATA)
    // asm 00006E4C: 	BU	HIDDTX
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIDDEN_TRACKS", 0, 0);
    UNIMPL();
}

void NHT1(void)
{
    // asm 00006E4D: CMPI	SW_VIEW0_H|SW_VIEW2_H,R0
    // asm 00006E4E: 	BNE	NHT2
    // asm 00006E4F: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006E50: 	CMPI	2,R0
    // asm 00006E51: 	BEQ	HIDDTX
    // asm 00006E52: 	CALL	RESTORE_HIDDEN
    // asm 00006E53: 	LDI	2,R0
    // asm 00006E54: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    // 	;replace
    // asm 00006E55: 	LDI	406h,AR2
    // asm 00006E56: 	CALL	OBJ_FIND_FIRST
    // asm 00006E57: 	LDI	*+AR0(OFLAGS),R0
    // asm 00006E58: 	ANDN	O_1PAL,R0
    // asm 00006E59: 	STI	R0,*+AR0(OFLAGS)
    // asm 00006E5A: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006E5B: 	STI	R0,*+AR0(OCARBLK)
    // asm 00006E5C: 	LDL	mpcmed,R0
    // asm 00006E5D: 	STI	R0,*+AR0(OROMDATA)
    // asm 00006E5E: 	BU	HIDDTX
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NHT1", 0, 0);
    UNIMPL();
}

void NHT2(void)
{
    // asm 00006E5F: CMPI	SW_VIEW1_H|SW_VIEW2_H,R0
    // asm 00006E60: 	BNE	NHT3
    // asm 00006E61: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006E62: 	CMPI	3,R0
    // asm 00006E63: 	BEQ	HIDDTX
    // asm 00006E64: 	CALL	RESTORE_HIDDEN
    // asm 00006E65: 	LDI	3,R0
    // asm 00006E66: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    // 	;replace
    // asm 00006E67: 	LDI	409h,AR2
    // asm 00006E68: 	CALL	OBJ_FIND_FIRST
    // asm 00006E69: 	LDI	*+AR0(OFLAGS),R0
    // asm 00006E6A: 	ANDN	O_1PAL,R0
    // asm 00006E6B: 	STI	R0,*+AR0(OFLAGS)
    // asm 00006E6C: 	LDI	409h,AR2
    // asm 00006E6D: 	CALL	OBJ_FIND_FIRST
    // asm 00006E6E: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006E6F: 	STI	R0,*+AR0(OCARBLK)
    // asm 00006E70: 	LDL	mpcexp,R0
    // asm 00006E71: 	STI	R0,*+AR0(OROMDATA)
    // asm 00006E72: 	BU	HIDDTX
NHT3:
    // asm 00006E73: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006E74: 	CMPI	-1,R0
    // asm 00006E75: 	BEQ	HIDDTX
    // asm 00006E76: 	CALL	RESTORE_HIDDEN
    // asm 00006E77: 	LDI	-1,R0
    // asm 00006E78: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
HIDDTX:
    // asm 00006E79: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NHT2", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GET THE RACE INDEX (POINTED TO BY WHEEL)
*
*
 */
void GET_RACE_IDX(void)
{
    // asm 00006E7A: 	CALL	GETCHOICE
    // asm 00006E7B: 	LDI	@POSE,R0
    // asm 00006E7C: 	CMPI	5,R0
    // asm 00006E7D: 	BLE	NDDR
    // asm 00006E7E: 	DEC	R0
NDDR:
    // asm 00006E7F: CMPI	@CHOSEN_RACE,R0
    // asm 00006E80: 	BEQ	NSND
    // asm 00006E81: 	PUSH	R0
    // asm 00006E82: 	SONDFX	TRANS
    // asm 00006E84: 	POP	R0
NSND:
    // asm 00006E85: STI	R0,@CHOSEN_RACE
    // asm 00006E86: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R1
    // asm 00006E87: 	CMPI	-1,R1
    // asm 00006E88: 	BEQ	NH
    // asm 00006E89: 	CMPI	1,R0
    // asm 00006E8A: 	BNE	R55
    // asm 00006E8B: 	CMPI	1,R1		;Hidden golden gate
    // asm 00006E8C: 	LDIEQ	11,AR3
    // asm 00006E8D: 	BNE	NH
    // asm 00006E8E: 	BU	R66
R55:
    // asm 00006E8F: 	CMPI	6,R0
    // asm 00006E90: 	BNE	R56
    // asm 00006E91: 	CMPI	2,R1
    // asm 00006E92: 	LDIEQ	12,AR3
    // asm 00006E93: 	BNE	NH
    // asm 00006E94: 	BU	R66
R56:
    // asm 00006E95: 	CMPI	9,R0
    // asm 00006E96: 	BNE	NH
    // asm 00006E97: 	CMPI	3,R1
    // asm 00006E98: 	LDIEQ	13,AR3
    // asm 00006E99: 	BNE	NH
R66:
    // asm 00006E9A: 	LDI	@CHOSEN_RACE,R0
    // asm 00006E9B: 	LDI	R0,AR2
    // asm 00006E9C: 	BU	KK88
NH:
    // asm 00006E9D: 	LDI	@CHOSEN_RACE,AR2
    // asm 00006E9E: 	LDI	AR2,AR3
KK88:
    // asm 00006E9F: ADDI	400h,AR2
    // asm 00006EA0: 	CALL	OBJ_FIND_FIRST
    // asm 00006EA1: 	LDF	*+AR0(OPOSX),R2
    // asm 00006EA2: 	FLOAT	256,R0
    // asm 00006EA3: 	ADDF	R0,R2
    // asm 00006EA4: 	LDF	*+AR0(OPOSY),R3
    // asm 00006EA5: 	FLOAT	180,R0
    // asm 00006EA6: 	ADDF	R0,R3
    // asm 00006EA7: 	LDI	AR3,R0
    // asm 00006EA8: 	MPYI	TS_SIZE,R0
    // asm 00006EA9: 	LDI	R0,AR0
    // asm 00006EAA: 	ADDI	@TRACK_SELECTION_LISTI,AR0
    // asm 00006EAB: 	LDI	*AR0,AR2
    // asm 00006EAC: 	ADDI	@LEG_NAMESI,AR2
    // asm 00006EAD: 	LDI	*AR2,AR2
    // asm 00006EAE: 	LDI	1,RC
    // asm 00006EAF: 	CALL	TEXT_ADD
    // asm 00006EB0: 	CALL	SET12FONT
    // asm 00006EB1: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00006EB4: 	CMPI	5,AR3
    // asm 00006EB5: 	BNE	NOTTHEWH
    // asm 00006EB6: 	READAUD	ADJ_CLINTON
    // asm 00006EB8: 	CMPI	1,R0
    // asm 00006EB9: 	BNE	NOTTHEWH
    // asm 00006EBA: 	LDI	@CRUISE2,AR2
    // asm 00006EBB: 	FLOAT	256,R2
    // asm 00006EBC: 	FLOAT	120,R3
    // asm 00006EBD: 	LDI	1,RC
    // asm 00006EBE: 	CALL	TEXT_ADD
    // asm 00006EBF: 	CALL	SET12FONT
    // asm 00006EC0: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00006EC3: 	LDI	@CRUISE3,AR2
    // asm 00006EC4: 	FLOAT	256,R2
    // asm 00006EC5: 	FLOAT	135,R3
    // asm 00006EC6: 	LDI	1,RC
    // asm 00006EC7: 	CALL	TEXT_ADD
    // asm 00006EC8: 	CALL	SET12FONT
    // asm 00006EC9: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOTTHEWH:
    // asm 00006ECC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_RACE_IDX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void ZOOMINP(void)
{
    // asm 00006ECD: 	LDI	20,AR4
ZITSP:
    // asm 00006ECE: CALL	ZOOMIN_TSEL
    // asm 00006ECF: 	LDI	@OM_CHOSEN_RACE,R0
    // asm 00006ED0: 	CMPI	-1,R0
    // asm 00006ED1: 	BNE	NOWAY543
    // asm 00006ED2: 	CALL	GET_RACE_IDX
    // asm 00006ED3: 	CALL	TRACK_SEL_CURSOR
NOWAY543:
    // asm 00006ED4: 	SLEEP	1
    // asm 00006ED6: 	DBU	AR4,ZITSP
    // asm 00006ED7: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOMINP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void ZOOMIN_TSEL(void)
{
    // asm 00006EDB: 	LDI	@OACTIVE,AR5
ZISL:
    // asm 00006EDC: LDI	AR5,R0
    // asm 00006EDD: 	BZ	ZIX
    // asm 00006EDE: 	LDI	R0,AR0
    // asm 00006EDF: 	LDI	*AR0,AR5
    // asm 00006EE0: 	LDF	*+AR0(OVELZ),R0
    // asm 00006EE1: 	SUBF	*+AR0(OPOSZ),R0
    // asm 00006EE2: 	MPYF	0.42,R0	;.15
    // asm 00006EE3: 	CMPF	-5,R0
    // asm 00006EE4: 	BGT	JJDD
    // asm 00006EE5: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00006EE6: 	STF	R0,*+AR0(OPOSZ)
    // asm 00006EE7: 	BU	HGF
JJDD:
    // asm 00006EE8: 	LDF	*+AR0(OVELZ),R0
    // asm 00006EE9: 	STF	R0,*+AR0(OPOSZ)
    // asm 00006EEA: 	CLRF	R2
    // asm 00006EEB: 	STF	R2,*+AR0(ORADZ)
    // asm 00006EEC: 	LDI	AR0,AR2
    // asm 00006EED: 	ADDI	OMATRIX,AR2
    // asm 00006EEE: 	CALL	FIND_ZMATRIX
    // asm 00006EEF: 	BU	HHFFD
HGF:
    // asm 00006EF0: 	LDF	*+AR0(ORADZ),R2
    // asm 00006EF1: 	MPYF	0.65,R2
    // asm 00006EF2: 	STF	R2,*+AR0(ORADZ)
    // asm 00006EF3: 	LDI	AR0,AR2
    // asm 00006EF4: 	ADDI	OMATRIX,AR2
    // asm 00006EF5: 	CALL	FIND_ZMATRIX
HHFFD:
    // asm 00006EF6: 	BU	ZISL
ZIX:
    // asm 00006EF7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOMIN_TSEL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*DO THE NESSESARY MUMBO JUMBO, *AND* LINKEM _ALL_
*TOGETHER VIA OLINK2
*
*
*
 */
void FIX_TRACK_SCREEN(void)
{
    // asm 00006EF8: 	CLRI	R0
    // asm 00006EF9: 	STI	R0,@TSEL_HEAD
    // asm 00006EFA: 	LDI	-1,R6
    // asm 00006EFB: 	LDI	@OACTIVE,AR5
FTSL:
    // asm 00006EFC: LDI	AR5,R0
    // asm 00006EFD: 	BZ	FTSLX
    // asm 00006EFE: 	LDI	R0,AR0
    // asm 00006EFF: 	LDI	*AR0,AR5		;NEXT OBJECT
    // asm 00006F00: 	LDF	*+AR0(OPOSX),R0
    // asm 00006F01: 	STF	R0,*+AR0(OVELX)
    // asm 00006F02: 	LDF	*+AR0(OPOSY),R0
    // asm 00006F03: 	STF	R0,*+AR0(OVELY)
    // asm 00006F04: 	LDF	*+AR0(OPOSZ),R0
    // asm 00006F05: 	STF	R0,*+AR0(OVELZ)
    // asm 00006F06: 	RANDN	28000
    // asm 00006F08: 	FLOAT	R0
    // asm 00006F09: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00006F0A: 	STF	R0,*+AR0(OPOSZ)
    // asm 00006F0B: 	LDF	6,R0
    // asm 00006F0C: 	CALL	SFRAND
    // asm 00006F0D: 	STF	R0,*+AR0(ORADZ)
    // asm 00006F0E: 	LDI	*+AR0(OID),R0
    // asm 00006F0F: 	CMPI	040Ch,R0	;SMALL CURSOR
    // asm 00006F10: 	BNE	NSC
    // asm 00006F11: 	STI	AR0,*+AR7(SMALL_CURSOR)
    // asm 00006F12: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006F13: 	STI	R0,*+AR7(ROM_SMALL_CURSOR)
    // asm 00006F14: 	CMPI	-1,R6
    // asm 00006F15: 	LDIEQ	AR0,R6
    // asm 00006F16: 	STI	AR5,*+AR0(OLINK2)
    // asm 00006F17: 	BU	FTSL
NSC:
    // asm 00006F18: 	CMPI	8A1h,R0
    // asm 00006F19: 	BNE	NTRM1
    // asm 00006F1A: 	LDI	AR0,AR4
    // asm 00006F1B: 	CREATE	CYCLE_PUSH,UTIL_C|CHOOSERACE_T
    // asm 00006F1E: 	LDI	AR4,AR0
    // asm 00006F1F: 	CMPI	-1,R6
    // asm 00006F20: 	LDIEQ	AR0,R6
    // asm 00006F21: 	STI	AR5,*+AR0(OLINK2)
    // asm 00006F22: 	BU	FTSL
NTRM1:
    // asm 00006F23: 	CMPI	8A2h,R0
    // asm 00006F24: 	BNE	NWHEL1
    // asm 00006F25: 	LDI	AR0,AR4
    // asm 00006F26: 	CREATE	TURNTO_SELECT,UTIL_C|CHOOSERACE_T
    // asm 00006F29: 	LDI	AR4,AR0
    // asm 00006F2A: 	CMPI	-1,R6
    // asm 00006F2B: 	LDIEQ	AR0,R6
    // asm 00006F2C: 	STI	AR5,*+AR0(OLINK2)
    // asm 00006F2D: 	BU	FTSL
NWHEL1:
    // asm 00006F2E: 	CMPI	0410h,R0
    // asm 00006F2F: 	BNE	NBC
    // asm 00006F30: 	STI	AR0,*+AR7(BIG_CURSOR)
    // asm 00006F31: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006F32: 	STI	R0,*+AR7(ROM_BIG_CURSOR)
    // asm 00006F33: 	LDI	AR0,AR2
    // asm 00006F34: 	CALL	OBJ_DELETE
    // asm 00006F35: 	CMPI	-1,R6
    // asm 00006F36: 	LDIEQ	AR0,R6
    // asm 00006F37: 	STI	AR5,*+AR0(OLINK2)
    // asm 00006F38: 	BU	FTSL
NBC:
    // asm 00006F39: 	CMPI	0405h,R0
    // asm 00006F3A: 	BNE	NAS
    // asm 00006F3B: 	PUSH	AR0
    // asm 00006F3C: 	LDI	AR0,AR4
    // asm 00006F3D: 	CREATE	ANIMATE_MAP,UTIL_C|CHOOSERACE_T
    // asm 00006F40: 	POP	AR0
    // asm 00006F41: 	CMPI	-1,R6
    // asm 00006F42: 	LDIEQ	AR0,R6
    // asm 00006F43: 	STI	AR5,*+AR0(OLINK2)
    // asm 00006F44: 	BU	FTSL
NAS:
    // asm 00006F45: 	CMPI	40Eh,R0
    // asm 00006F46: 	BNE	NN77
    // asm 00006F47: 	LDI	AR0,AR2
    // asm 00006F48: 	CALL	OBJ_DELETE
    // asm 00006F49: 	BU	FTSL
NN77:
    // asm 00006F4A: 	CMPI	40Fh,R0
    // asm 00006F4B: 	BNE	NN88
    // asm 00006F4C: 	LDI	AR0,AR2
    // asm 00006F4D: 	CALL	OBJ_DELETE
    // asm 00006F4E: 	BU	FTSL
NN88:
    // asm 00006F4F: 	CMPI	-1,R6
    // asm 00006F50: 	LDIEQ	AR0,R6
    // asm 00006F51: 	STI	AR5,*+AR0(OLINK2)
    // asm 00006F52: 	BU	FTSL
FTSLX:
    // asm 00006F53: 	STI	R6,@TSEL_HEAD
    // asm 00006F54: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIX_TRACK_SCREEN", 0, 0);
    UNIMPL();
}

void TSEL_ZOOMOUTP(void)
{
    // asm 00006F55: 	SONDFX	VIEW2IN
    // asm 00006F57: 	LDI	@RACE_MODE,R0
    // asm 00006F58: 	CMPI	RM_USA,R0
    // asm 00006F59: 	BNE	JJKKD
    // asm 00006F5A: 	LDI	5,AR2
    // asm 00006F5B: 	BU	LLGF
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TSEL_ZOOMOUTP", 0, 0);
    UNIMPL();
}

void JJKKD(void)
{
    // asm 00006F5C: LDI	@RAW_CHOOSENTRK,AR2
LLGF:
    // asm 00006F5D: LDI	AR2,AR3
    // asm 00006F5E: 	ADDI	400h,AR2
    // asm 00006F5F: 	CALL	OBJ_FIND_FIRST
    // asm 00006F60: 	STI	AR0,@TSEL_THEONE
    // asm 00006F61: 	CREATEC	CENTER_THEONE,UTIL_C
    // asm 00006F64: 	LDI	30,AR5
TSLLP:
    // asm 00006F65: CALL	TSEL_ZOOMOUT
    // asm 00006F66: 	SLEEP	1
    // asm 00006F68: 	DBU	AR5,TSLLP
    // asm 00006F69: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JJKKD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void CENTER_THEONE(void)
{
    // asm 00006F6D: 	LDI	@TSEL_THEONE,AR4
    // asm 00006F6E: 	LDI	15,AR5
CTOLP:
    // asm 00006F6F: LDF	*+AR4(OPOSX),R0
    // asm 00006F70: 	MPYF	0.8,R0
    // asm 00006F71: 	STF	R0,*+AR4(OPOSX)
    // asm 00006F72: 	LDF	*+AR4(OPOSY),R0
    // asm 00006F73: 	MPYF	0.8,R0
    // asm 00006F74: 	STF	R0,*+AR4(OPOSY)
    // asm 00006F75: 	CALL	CENTER_SUB1
    // asm 00006F76: 	SLEEP	1
    // asm 00006F78: 	DBU	AR5,CTOLP
    // asm 00006F79: 	LDI	15,AR5
CTOLPB:
    // asm 00006F7A: LDF	*+AR4(OPOSZ),R0
    // asm 00006F7B: 	MPYF	0.08,R0
    // asm 00006F7C: 	CMPF	50,R0
    // asm 00006F7D: 	LDFLT	50,R0
    // asm 00006F7E: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00006F7F: 	STF	R0,*+AR4(OPOSZ)
    // asm 00006F80: 	CALL	CENTER_SUB1
    // asm 00006F81: 	SLEEP	1
    // asm 00006F83: 	DBU	AR5,CTOLPB
    // asm 00006F84: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CENTER_THEONE", 0, 0);
    UNIMPL();
}

void CENTER_SUB1(void)
{
    // asm 00006F85: 	LDF	*+AR4(OPOSX),R2
    // asm 00006F86: 	FLOAT	256,R0
    // asm 00006F87: 	ADDF	R0,R2
    // asm 00006F88: 	LDF	*+AR4(OPOSY),R3
    // asm 00006F89: 	FLOAT	180,R0
    // asm 00006F8A: 	ADDF	R0,R3
    // asm 00006F8B: 	LDI	@RACE_MODE,R0
    // asm 00006F8C: 	CMPI	RM_USA,R0
    // asm 00006F8D: 	BNE	K89
    // asm 00006F8E: 	LDI	14,AR2
    // asm 00006F8F: 	BU	K99
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CENTER_SUB1", 0, 0);
    UNIMPL();
}

void K89(void)
{
    // asm 00006F90: LDI	@CHOSEN_RACE,AR2
K99:
    // asm 00006F91: ADDI	@LEG_NAMESI,AR2
    // asm 00006F92: 	LDI	*AR2,AR2
    // asm 00006F93: 	LDI	1,RC
    // asm 00006F94: 	CALL	TEXT_ADD
    // asm 00006F95: 	CALL	SET12FONT
    // asm 00006F96: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00006F99: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "K89", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TSEL_ZOOMOUT(void)
{
    // asm 00006F9A: 	LDI	@TSEL_HEAD,R0
    // asm 00006F9B: 	BU	LKGG
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TSEL_ZOOMOUT", 0, 0);
    UNIMPL();
}

void JJKK(void)
{
    // asm 00006F9C: LDI	*+AR0(OLINK2),R0
    // asm 00006F9D: 	BZ	ALLDN
LKGG:
    // asm 00006F9E: LDI	R0,AR0
    // asm 00006F9F: 	CMPI	@TSEL_THEONE,R0
    // asm 00006FA0: 	BEQ	JJKK			;skip our choice
    // asm 00006FA1: 	LDF	*+AR0(OPOSX),R0
    // asm 00006FA2: 	MPYF	0.1,R0
    // asm 00006FA3: 	ADDF	*+AR0(OPOSX),R0
    // asm 00006FA4: 	STF	R0,*+AR0(OPOSX)
    // asm 00006FA5: 	LDF	*+AR0(OPOSY),R0
    // asm 00006FA6: 	MPYF	0.1,R0
    // asm 00006FA7: 	ADDF	*+AR0(OPOSY),R0
    // asm 00006FA8: 	STF	R0,*+AR0(OPOSY)
    // asm 00006FA9: 	LDF	*+AR0(OPOSZ),R0
    // asm 00006FAA: 	MPYF	0.08,R0
    // asm 00006FAB: 	CMPF	50,R0
    // asm 00006FAC: 	LDFLT	50,R0
    // asm 00006FAD: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00006FAE: 	STF	R0,*+AR0(OPOSZ)
    // asm 00006FAF: 	BU	JJKK
ALLDN:
    // asm 00006FB0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JJKK", 0, 0);
    UNIMPL();
}

/* *
 */
void ANIMATE_MAP(void)
{
    // asm 00006FB7: 	LDI	@ANIMATE_MAP_TABI,AR5
AMLP:
    // asm 00006FB8: 	LDI	*AR5++,R0
    // asm 00006FB9: 	BN	ANIMATE_MAP
    // asm 00006FBA: 	STI	R0,*+AR4(OROMDATA)
    // asm 00006FBB: 	SLEEP	4
    // asm 00006FBD: 	BU	AMLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ANIMATE_MAP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
 */
void TRACK_SEL_CURSOR(void)
{
    // asm 00006FBE: 	LDI	*+AR7(OLD_INDEX),R0
    // asm 00006FBF: 	LDI	@CHOSEN_RACE,AR0
    // asm 00006FC0: 	CMPI	R0,AR0
    // asm 00006FC1: 	RETSEQ
    // asm 00006FC2: 	STI	AR0,*+AR7(OLD_INDEX)
    // asm 00006FC3: 	LDI	AR0,AR3
    // asm 00006FC4: 	LDI	AR0,AR2
    // asm 00006FC5: 	ADDI	400h,AR2
    // asm 00006FC6: 	CALL	OBJ_FIND_FIRST
    // asm 00006FC7: 	CMPI	5,AR3
    // asm 00006FC8: 	LDIEQ	*+AR7(ROM_BIG_CURSOR),R0
    // asm 00006FC9: 	LDINE	*+AR7(ROM_SMALL_CURSOR),R0
    // asm 00006FCA: 	LDI	*+AR7(SMALL_CURSOR),AR1
    // asm 00006FCB: 	STI	R0,*+AR1(OROMDATA)
    // asm 00006FCC: 	LDF	*+AR0(OPOSX),R0
    // asm 00006FCD: 	STF	R0,*+AR1(OPOSX)
    // asm 00006FCE: 	LDF	*+AR0(OPOSY),R0
    // asm 00006FCF: 	STF	R0,*+AR1(OPOSY)
    // asm 00006FD0: 	LDF	*+AR0(OPOSZ),R0
    // asm 00006FD1: 	SUBF	8,R0
    // asm 00006FD2: 	STF	R0,*+AR1(OPOSZ)
    // asm 00006FD3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRACK_SEL_CURSOR", 0, 0);
    UNIMPL();
}

/* *	R6	X
*	R7	Y
*	AR5	Z
*
 */
void GET_TURNOBJ(void)
{
    // asm 00006FDB: 	FLOAT	AR5,R5
    // asm 00006FDC: 	LDL	TURNANI,AR5
    // asm 00006FDD: 	CALL	OBJ_GET
    // asm 00006FDE: 	LDI	AR0,AR4
    // asm 00006FDF: 	STF	R6,*+AR4(OPOSX)
    // asm 00006FE0: 	STF	R7,*+AR4(OPOSY)
    // asm 00006FE1: 	STF	R5,*+AR4(OPOSZ)
    // asm 00006FE2: 	LDI	AR0,AR2
    // asm 00006FE3: 	CALL	OBJ_INSERTP
GETTOLP:
    // asm 00006FE4: 	LDI	@_MODE,R0
    // asm 00006FE5: 	AND	MMODE,R0	;<-this mode
    // asm 00006FE6: 	CMPI	MINTRO,R0
    // asm 00006FE7: 	BNE	SUICIDE
    // asm 00006FE8: 	LDI	*AR5++,R0
    // asm 00006FE9: 	BNN	JIJI
    // asm 00006FEA: 	LDL	TURNANI,AR5
    // asm 00006FEB: 	LDI	*AR5++,R0
JIJI:
    // asm 00006FEC: STI	R0,*+AR4(OROMDATA)
    // asm 00006FED: 	SLEEP	7
    // asm 00006FEF: 	BU	GETTOLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TURNOBJ", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void BOILERPLATE_INIT(void)
{
    // asm 00006FFF: 	LDL	next,AR2
    // asm 00007000: 	LDI	0,R2
    // asm 00007001: 	LDI	0,R3
    // asm 00007002: 	LDI	0,RC
    // asm 00007003: 	CALL	OBJ_QMAKE
    // asm 00007004: 	STI	AR0,@BOILEROBJ
    // asm 00007005: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOILERPLATE_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void BOILERPLATE(void)
{
    // asm 00007006: 	LDI	*+AR7(SMALL_CURSOR),AR2
    // asm 00007007: 	LDF	*+AR2(OPOSX),R0
    // asm 00007008: 	STF	R0,*+AR2(OCARBLK)
    // asm 00007009: 	FLOAT	-6000,R0
    // asm 0000700A: 	STF	R0,*+AR2(OPOSX)
    // asm 0000700B: 	LDI	@BOILEROBJ,AR2
    // asm 0000700C: 	CALL	OBJ_INSERTP
    // asm 0000700D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOILERPLATE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void BOILERPLATE_DELETE(void)
{
    // asm 0000700E: 	LDI	@BOILEROBJ,AR2
    // asm 0000700F: 	CALL	OBJ_DELETE
    // asm 00007010: 	LDI	*+AR7(SMALL_CURSOR),AR2
    // asm 00007011: 	LDF	*+AR2(OCARBLK),R0
    // asm 00007012: 	STF	R0,*+AR2(OPOSX)
    // asm 00007013: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOILERPLATE_DELETE", 0, 0);
    UNIMPL();
}

#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "heads.h"
#include "discovered_labels.h"

/*
 * Source module: asm/HEADS.ASM
 */

void TEXTDELER(void);
void VANITY_HEADS(void);
void SET18FONTDS_WHITE(void);
void VANITY(void);
void VANITY_SUB(void);
void GETT(void);
void CENTEREM(void);
void RIGHTEM(void);

#define HEADTYPE 0x88D0
/* asm: HEADS	.word	jeno */
/* asm: 	.word	marc */
/* asm: 	.word	pet */
/* asm: 	.word	xion */
/* asm: 	.word	ted */
/* asm: 	.word	matt */
/* asm: 	.word	vince */
/* asm: 	.word	carl */
/* asm: 	.word	glen */
/* asm: 	.word	ken */
/* asm: 	.word	eric */
/* asm: 	.word	-1 */
int HEADS[] = {
    jeno,
    marc,
    pet,
    xion,
    ted,
    matt,
    vince,
    carl,
    glen,
    ken,
    eric,
    -1,
};
/* asm: YINCREMENT	.bss	YINCREMENT,1 */
int YINCREMENT;
/* asm: FONTUSED	.bss	FONTUSED,1 */
int FONTUSED;

/* *----------------------------------------------------------------------------
 */
void TEXTDELER(void)
{
    // asm 0000A1A4: 	LDI	@TEXT_ACTIVEI,AR4
TEXTDELERLP:
    // asm 0000A1A5: 	LDI	*AR4,R0
    // asm 0000A1A6: 	BZ	TXTXX
TTEST:
    // asm 0000A1A7: LDI	R0,AR4
    // asm 0000A1A8: 	LDF	*+AR4(TEXT_POSY),R0
    // asm 0000A1A9: 	CMPF	-25,R0
    // asm 0000A1AA: 	BGT	TEXTDELERLP
    // asm 0000A1AB: 	LDI	*AR4,R7
    // asm 0000A1AC: 	LDI	@TEXT_ACTIVEI,R1	;get free list pointer
DELLP:
    // asm 0000A1AD: LDI	R1,AR1
    // asm 0000A1AE: 	LDI	*AR1,R1
#if DEBUG
    // asm: 	BZ	$			;lockup on end of list found
#endif
    // asm 0000A1AF: 	CMPI	R1,AR4
    // asm 0000A1B0: 	BNE	DELLP
    // asm 0000A1B1: 	LDI	*AR4,R1
    // asm 0000A1B2: 	STI	R1,*AR1			;LINK AROUND
    // asm 0000A1B3: 	LDI	@TEXT_FREE_COUNT,R1
    // asm 0000A1B4: 	INC	R1
    // asm 0000A1B5: 	STI	R1,@TEXT_FREE_COUNT
    // asm 0000A1B6: 	LDI	@TEXT_FREEI,AR1		;get free list pointer
    // asm 0000A1B7: 	LDI	*AR1,R1
    // asm 0000A1B8: 	STI	R1,*AR4
    // asm 0000A1B9: 	STI	AR4,*AR1
    // asm 0000A1BA: 	LDI	R7,R0
    // asm 0000A1BB: 	BZ	TXTXX
    // asm 0000A1BC: 	BU	TTEST
TXTXX:
    // asm 0000A1BD: 	SLEEP	1
    // asm 0000A1BF: 	BU	TEXTDELER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXTDELER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
 */
void VANITY_HEADS(void)
{
    // asm 0000A1CC: 	SETDP
    // asm 0000A1CD: 	LDI	@CAMERAPOSI,AR6
    // asm 0000A1CE: 	CLRF	R0
    // asm 0000A1CF: 	STF	R0,*+AR6(X)
    // asm 0000A1D0: 	STF	R0,*+AR6(Y)
    // asm 0000A1D1: 	STF	R0,*+AR6(Z)
    // asm 0000A1D2: 	LDL	theheads_PALETTES,AR2
    // asm 0000A1D3: 	CALL	alloc_section
    // asm 0000A1D4: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 0000A1D9: 	DMA_WT		R0
    // asm 0000A1DE: 	CALL	FIFO_RESET
    // asm 0000A1DF: 	LDI	1,R0
    // asm 0000A1E0: 	STI	R0,@HARD_SECTION_LOAD
    // asm 0000A1E1: 	LDL	_SECtheheads,AR2
    // asm 0000A1E2: 	CALL	LOAD_SECTION_REQ
    // asm 0000A1E3: 	LDL	HEADS,AR5
    // ;	CLRF	R6
    // asm 0000A1E4: 	LDF	PI,R6
VANHDLP:
    // asm 0000A1E5: 	LDI	*AR5++,R0
    // asm 0000A1E6: 	BN	VHIX
    // asm 0000A1E7: 	LDI	R0,AR2
    // asm 0000A1E8: 	CALL	OBJ_GETE
    // asm 0000A1E9: 	LDF	R6,R2
    // ;	CALL	_SINE
    // asm 0000A1EA: 	PUSH	AR0
    // asm 0000A1EB: 	CALL	_HPsin
    // asm 0000A1EC: 	POP	AR0
    // asm 0000A1ED: 	MPYF	130,R0
    // asm 0000A1EE: 	STF	R0,*+AR0(OPOSX)
    // asm 0000A1EF: 	LDF	R6,R2
    // ;	CALL	_COSI
    // asm 0000A1F0: 	PUSH	AR0
    // asm 0000A1F1: 	CALL	_HPcos
    // asm 0000A1F2: 	POP	AR0
    // asm 0000A1F3: 	MPYF	130,R0
    // asm 0000A1F4: 	STF	R0,*+AR0(OPOSY)
    // asm 0000A1F5: 	FLOAT	368,R0
    // asm 0000A1F6: 	STF	R0,*+AR0(OPOSZ)
    // asm 0000A1F7: 	STF	R6,*+AR0(ORADY)
    // asm 0000A1F8: 	LDI	AR0,AR2
    // asm 0000A1F9: 	CALL	OBJ_INSERT
    // asm 0000A1FA: 	SUBF	0.571,R6
    // asm 0000A1FB: 	B	VANHDLP
VHIX:
    // ;	DIE
VANITY_HEADS_LP:
    // asm 0000A1FC: 	LDI	@OACTIVEI,AR0
VOLP:
    // asm 0000A1FD: 	LDI	*AR0,R0
    // asm 0000A1FE: 	BZ	VOSLP
    // asm 0000A1FF: 	LDI	R0,AR0
    // asm 0000A200: 	LDF	*+AR0(ORADY),R2
    // asm 0000A201: 	ADDF	0.01,R2
    // asm 0000A202: 	STF	R2,*+AR0(ORADY)
    // asm 0000A203: 	LDF	R2,R6
    // asm 0000A204: 	CALL	_SINE
    // asm 0000A205: 	MPYF	130,R0
    // asm 0000A206: 	STF	R0,*+AR0(OPOSX)
    // asm 0000A207: 	LDF	R6,R2
    // asm 0000A208: 	CALL	_COSI
    // asm 0000A209: 	MPYF	130,R0
    // asm 0000A20A: 	STF	R0,*+AR0(OPOSY)
    // asm 0000A20B: 	BU	VOLP
VOSLP:
    // asm 0000A20C: SLEEP	1
    // asm 0000A20E: 	BU	VANITY_HEADS_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VANITY_HEADS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SET18FONTDS_WHITE(void)
{
    // asm 0000A20F: 	CALL	SET18FONTDS
    // asm 0000A210: 	LDL	font18_white,AR2
    // asm 0000A211: 	CALL	PAL_FIND_RAW
    // asm 0000A212: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 0000A213: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 0000A214: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET18FONTDS_WHITE", 0, 0);
    UNIMPL();
}

void VANITY(void)
{
    // asm 0000A215: 	JSRP	VANITY_SUB
    // asm 0000A21B: 	BR	CYCLE_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VANITY", 0, 0);
    UNIMPL();
}

void VANITY_SUB(void)
{
    // asm 0000A21C: 	LDI	MATTR,R0
    // asm 0000A21D: 	STI	R0,@_MODE
    // asm 0000A21E: 	LDI	1,R0
    // asm 0000A21F: 	STI	R0,@COINOFF
    // asm 0000A220: 	CLRI	R0
    // asm 0000A221: 	STI	R0,@BGNDCOLA
    // asm 0000A222: 	STI	R0,@NOAERASE
    // asm 0000A223: 	LDI	@CAMERAPOSI,AR6
    // asm 0000A224: 	CLRF	R2
    // asm 0000A225: 	STF	R2,*+AR6(X)
    // asm 0000A226: 	STF	R2,*+AR6(Y)
    // asm 0000A227: 	STF	R2,*+AR6(Z)
    // asm 0000A228: 	LDI	@CAMERARADI,AR6
    // asm 0000A229: 	STF	R2,*+AR6(X)
    // asm 0000A22A: 	STF	R2,*+AR6(Y)
    // asm 0000A22B: 	STF	R2,*+AR6(Z)
    // asm 0000A22C: 	LDI	@CAMERAMATRIXI,AR2
    // asm 0000A22D: 	CALL	FIND_YMATRIX
    // asm 0000A22E: 	SLEEP	1
    // asm 0000A230: 	CREATE	VANITY_HEADS,HEADTYPE
    // asm 0000A233: 	CREATE	TEXTDELER,HEADTYPE
    // ;	CREATE	LOGO_WATCH,UTIL_C
    // asm 0000A236: 	LDL	SET18FONTDS_WHITE,R0
    // asm 0000A237: 	STI	R0,@FONTUSED
    // asm 0000A238: 	LDF	24,R0
    // asm 0000A239: 	STF	R0,@YINCREMENT
    // asm 0000A23A: 	FLOAT	420,R6
    // asm 0000A23B: 	LDI	2,AR5
    // asm 0000A23C: 	LDI	@THANKS_TABLEI,AR4
DTLP2:
    // asm 0000A23D: 	ADDF	@YINCREMENT,R6		;ADDF	24,R6
KKPD:
    // asm 0000A23E: FLOAT	402,R0
    // asm 0000A23F: 	CMPF	R0,R6
    // asm 0000A240: 	BLT	JJDDA
    // asm 0000A241: 	SLEEP	1
    // asm 0000A243: 	SUBF	1,R6
    // asm 0000A244: 	BU	KKPD
JJDDA:
    // asm 0000A245: 	LDI	*AR4++,R7	;number of entries
    // asm 0000A246: 	CMPI	10,R7
    // asm 0000A247: 	BNE	NOT10
    // asm 0000A248: 	LDI	*AR4++,R0
    // asm 0000A249: 	STI	R0,@FONTUSED
    // asm 0000A24A: 	LDF	10,R0
    // asm 0000A24B: 	STF	R0,@YINCREMENT
    // asm 0000A24C: 	BU	KKPD
NOT10:
    // asm 0000A24D: 	CMPI	8,R7
    // asm 0000A24E: 	BNE	NOTSPEC8
    // asm 0000A24F: 	FLOAT	*AR4++,R2
    // asm 0000A250: 	LDF	R6,R3
    // asm 0000A251: 	LDI	*AR4++,AR2
    // asm 0000A252: 	CALL	GETT
    // asm 0000A253: 	BU	DTLP2
NOTSPEC8:
    // asm 0000A254: 	CMPI	7,R7
    // asm 0000A255: 	BNE	NOTSPEC7
    // asm 0000A256: 	FLOAT	*AR4++,R2
    // asm 0000A257: 	LDF	R6,R3
    // asm 0000A258: 	LDI	*AR4++,AR2
    // asm 0000A259: 	CALL	GETT
    // asm 0000A25A: 	CALL	RIGHTEM
    // asm 0000A25B: 	BU	DTLP2
NOTSPEC7:
    // asm 0000A25C: 	CMPI	4,R7
    // asm 0000A25D: 	BNE	NOTSPEC4
    // asm 0000A25E: 	LDI	1,R0
    // asm 0000A25F: 	STI	R0,@TEXT_FREEZE
    // asm 0000A260: 	SLEEP	60
    // asm 0000A262: 	CLRI	R0
    // asm 0000A263: 	STI	R0,@TEXT_FREEZE
    // asm 0000A264: 	BU	KKPD
NOTSPEC4:
    // asm 0000A265: 	CMPI	99,R7
    // asm 0000A266: 	BNE	NOTSPEC99
    // asm 0000A267: 	FLOAT	*AR4++,R2
    // asm 0000A268: 	FLOAT	(256-600),R2
    // asm 0000A269: 	LDF	R6,R3
    // asm 0000A26A: 	LDI	*AR4++,AR2
    // asm 0000A26B: 	CALL	GETT
    // asm 0000A26C: 	CALL	CENTEREM
    // asm 0000A26D: 	PUSH	AR4
    // asm 0000A26E: 	PUSH	AR5
    // asm 0000A26F: 	LDI	AR0,AR4
    // asm 0000A270: 	LDI	AR1,AR5
    // asm 0000A271: 	CREATE	KRIS,HEADTYPE
    // asm 0000A274: 	POP	AR5
    // asm 0000A275: 	POP	AR4
    // asm 0000A276: 	BU	DTLP2
KRIS:
    // asm 0000A277: SLEEP	1
    // asm 0000A279: 	LDF	*+AR4(TEXT_POSX),R0
    // asm 0000A27A: 	FLOAT	259,R1
    // asm 0000A27B: 	SUBF	R0,R1
    // asm 0000A27C: 	MPYF	0.05,R1
    // asm 0000A27D: 	ADDF	R1,R0
    // asm 0000A27E: 	STF	R0,*+AR4(TEXT_POSX)
    // asm 0000A27F: 	LDF	*+AR5(TEXT_POSX),R0
    // asm 0000A280: 	FLOAT	256,R1
    // asm 0000A281: 	SUBF	R0,R1
    // asm 0000A282: 	MPYF	0.05,R1
    // asm 0000A283: 	ADDF	R1,R0
    // asm 0000A284: 	STF	R0,*+AR5(TEXT_POSX)
    // asm 0000A285: 	BU	KRIS
NOTSPEC99:
    // asm 0000A286: 	CMPI	3,R7
    // asm 0000A287: 	BNE	NOTSPEC3
    // asm 0000A288: 	FLOAT	*AR4++,R2
    // asm 0000A289: 	LDF	R6,R3
    // asm 0000A28A: 	LDI	*AR4++,AR2
    // asm 0000A28B: 	CALL	GETT
    // asm 0000A28C: 	CALL	CENTEREM
    // asm 0000A28D: 	PUSH	AR4
    // asm 0000A28E: 	PUSH	AR5
    // asm 0000A28F: 	LDI	AR0,AR4
    // asm 0000A290: 	LDI	AR1,AR5
    // asm 0000A291: 	CREATE	ADAMSTOPRIBYL,HEADTYPE
    // asm 0000A294: 	POP	AR5
    // asm 0000A295: 	POP	AR4
    // asm 0000A296: 	BU	DTLP2
CHISPRIB:
    // asm 0000A298: SPTR	"CHRISTINE PRIBYL"
ADAMSTOPRIBYL:
    // asm 0000A298: 	SLEEP	1
    // asm 0000A29A: 	LDF	*+AR4(TEXT_POSY),R0
    // asm 0000A29B: 	FLOAT	250,R1
    // asm 0000A29C: 	CMPF	R1,R0
    // asm 0000A29D: 	BGT	ADAMSTOPRIBYL
    // asm 0000A29E: 	LDI	@CHISPRIB,R0
    // asm 0000A29F: 	STI	R0,*+AR4(TEXT_PTR)
    // asm 0000A2A0: 	STI	R0,*+AR5(TEXT_PTR)
    // asm 0000A2A1: 	DIE
NOTSPEC3:
    // asm 0000A2A2: 	CMPI	-1,R7
    // asm 0000A2A3: 	BEQ	WTD55
    // asm 0000A2A4: 	CMPI	0,R7
    // asm 0000A2A5: 	BNE	NOT0
    // asm 0000A2A6: 	BU	DTLP2
NOT0:
    // asm 0000A2A7: 	CMPI	2,R7
    // asm 0000A2A8: 	BEQ	TWO
    // 	;ONE
    // asm 0000A2A9: 	FLOAT	*AR4++,R2
    // asm 0000A2AA: 	LDF	R6,R3
    // asm 0000A2AB: 	LDI	*AR4++,AR2
    // asm 0000A2AC: 	CALL	GETT
    // asm 0000A2AD: 	CALL	CENTEREM
    // asm 0000A2AE: 	BU	DTLP2
TWO:
    // asm 0000A2AF: 	FLOAT	*AR4++,R2
    // asm 0000A2B0: 	LDF	R6,R3
    // asm 0000A2B1: 	LDI	*AR4++,AR2
    // asm 0000A2B2: 	CALL	GETT
    // asm 0000A2B3: 	FLOAT	*AR4++,R2
    // asm 0000A2B4: 	LDF	R6,R3
    // asm 0000A2B5: 	LDI	*AR4++,AR2
    // asm 0000A2B6: 	CALL	GETT
    // asm 0000A2B7: 	CALL	RIGHTEM
    // asm 0000A2B8: 	BU	DTLP2
WTD55:
    // asm 0000A2B9: 	LDI	HEADTYPE,R0
    // asm 0000A2BA: 	LDI	-1,R1
    // asm 0000A2BB: 	CALL	PRC_KILLALL
    // asm 0000A2BC: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VANITY_SUB", 0, 0);
    UNIMPL();
}

/* ;	LDL	SET18FONTDS
;	LDL	SETFIXEDFONTDS
 */
void GETT(void)
{
    // asm 0000A2C0: 	LDI	9999,RC
    // asm 0000A2C1: 	CALL	TEXT_ADDDS
    // asm 0000A2C2: 	LDF	-1,R0
    // asm 0000A2C3: 	STF	R0,*+AR0(TEXT_VELY)
    // asm 0000A2C4: 	STF	R0,*+AR1(TEXT_VELY)
    // asm 0000A2C5: 	LDI	@FONTUSED,R0
    // asm 0000A2C6: 	CALLU	R0
    // asm 0000A2C7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETT", 0, 0);
    UNIMPL();
}

void CENTEREM(void)
{
    // asm 0000A2C8: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 0000A2C9: 	OR	TXT_CENTER,R0
    // asm 0000A2CA: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 0000A2CB: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 0000A2CC: 	OR	TXT_CENTER,R0
    // asm 0000A2CD: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm 0000A2CE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CENTEREM", 0, 0);
    UNIMPL();
}

void RIGHTEM(void)
{
    // asm 0000A2CF: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 0000A2D0: 	OR	TXT_RIGHT,R0
    // asm 0000A2D1: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 0000A2D2: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 0000A2D3: 	OR	TXT_RIGHT,R0
    // asm 0000A2D4: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm 0000A2D5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RIGHTEM", 0, 0);
    UNIMPL();
}

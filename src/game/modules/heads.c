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
#include "../include/heads.h"

/*
 * Source module: asm/HEADS.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
#define HEADTYPE 0x88D0
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
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
int HEADS[] = { jeno, marc, pet, xion, ted, matt, vince, carl, glen, ken, eric, -1 };
// *----------------------------------------------------------------------------
/* asm: YINCREMENT	.bss	YINCREMENT,1 */
int YINCREMENT;
// *----------------------------------------------------------------------------
/* asm: CHISPRIB	SPTR	"CHRISTINE PRIBYL" */
char *CHISPRIB = "CHRISTINE PRIBYL";
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *SUBROUTINE FOR THE ABOVE
// *
// *
// *PARAMETERS
// *	AR2	STRING
// *	R2	X
// *	R3	Y
// *	R4	FONT
// *RETURNS
// *	AR0	S1
// *	AR1	S2
// *
/* asm: FONTUSED	.bss	FONTUSED,1 */
int FONTUSED;
// *----------------------------------------------------------------------------

void TEXTDELER(void)
{
    // asm: 	LDI	@TEXT_ACTIVEI,AR4
TEXTDELERLP:
    // asm: 	LDI	*AR4,R0
    // asm: 	BZ	TXTXX
TTEST:
    // asm: LDI	R0,AR4
    // asm: 	LDF	*+AR4(TEXT_POSY),R0
    // asm: 	CMPF	-25,R0
    // asm: 	BGT	TEXTDELERLP
    // asm: 	LDI	*AR4,R7
    // asm: 	LDI	@TEXT_ACTIVEI,R1	;get free list pointer
DELLP:
    // asm: LDI	R1,AR1
    // asm: 	LDI	*AR1,R1
#if DEBUG
    // asm: 	BZ	$			;lockup on end of list found
#endif
    // asm: 	CMPI	R1,AR4
    // asm: 	BNE	DELLP
    // asm: 	LDI	*AR4,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	@TEXT_FREE_COUNT,R1
    // asm: 	INC	R1
    // asm: 	STI	R1,@TEXT_FREE_COUNT
    // asm: 	LDI	@TEXT_FREEI,AR1		;get free list pointer
    // asm: 	LDI	*AR1,R1
    // asm: 	STI	R1,*AR4
    // asm: 	STI	AR4,*AR1
    // asm: 	LDI	R7,R0
    // asm: 	BZ	TXTXX
    // asm: 	BU	TTEST
TXTXX:
    // asm: 	SLEEP	1
    // asm: 	BU	TEXTDELER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXTDELER", 0, 0);
    UNIMPL();
}

void VANITY_HEADS(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAPOSI,AR6
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR6(X)
    // asm: 	STF	R0,*+AR6(Y)
    // asm: 	STF	R0,*+AR6(Z)
    // asm: 	LDL	theheads_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	LDL	_SECtheheads,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	HEADS,AR5
    // ;	CLRF	R6
    // asm: 	LDF	PI,R6
VANHDLP:
    // asm: 	LDI	*AR5++,R0
    // asm: 	BN	VHIX
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	LDF	R6,R2
    // ;	CALL	_SINE
    // asm: 	PUSH	AR0
    // asm: 	CALL	_HPsin
    // asm: 	POP	AR0
    // asm: 	MPYF	130,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	R6,R2
    // ;	CALL	_COSI
    // asm: 	PUSH	AR0
    // asm: 	CALL	_HPcos
    // asm: 	POP	AR0
    // asm: 	MPYF	130,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	STF	R6,*+AR0(ORADY)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	SUBF	0.571,R6
    // asm: 	B	VANHDLP
VHIX:
    // ;	DIE
VANITY_HEADS_LP:
    // asm: 	LDI	@OACTIVEI,AR0
VOLP:
    // asm: 	LDI	*AR0,R0
    // asm: 	BZ	VOSLP
    // asm: 	LDI	R0,AR0
    // asm: 	LDF	*+AR0(ORADY),R2
    // asm: 	ADDF	0.01,R2
    // asm: 	STF	R2,*+AR0(ORADY)
    // asm: 	LDF	R2,R6
    // asm: 	CALL	_SINE
    // asm: 	MPYF	130,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	R6,R2
    // asm: 	CALL	_COSI
    // asm: 	MPYF	130,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	BU	VOLP
VOSLP:
    // asm: SLEEP	1
    // asm: 	BU	VANITY_HEADS_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VANITY_HEADS", 0, 0);
    UNIMPL();
}

void SET18FONTDS_WHITE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	SET18FONTDS
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET18FONTDS_WHITE", 0, 0);
    UNIMPL();
}

void VANITY(void)
{
    // asm: 	JSRP	VANITY_SUB
    // asm: 	BR	CYCLE_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VANITY", 0, 0);
    UNIMPL();
}

void VANITY_SUB(void)
{
    // asm: 	LDI	MATTR,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@COINOFF
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	STI	R0,@NOAERASE
    // asm: 	LDI	@CAMERAPOSI,AR6
    // asm: 	CLRF	R2
    // asm: 	STF	R2,*+AR6(X)
    // asm: 	STF	R2,*+AR6(Y)
    // asm: 	STF	R2,*+AR6(Z)
    // asm: 	LDI	@CAMERARADI,AR6
    // asm: 	STF	R2,*+AR6(X)
    // asm: 	STF	R2,*+AR6(Y)
    // asm: 	STF	R2,*+AR6(Z)
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	SLEEP	1
    // asm: 	CREATE	VANITY_HEADS,HEADTYPE
    // asm: 	CREATE	TEXTDELER,HEADTYPE
    // ;	CREATE	LOGO_WATCH,UTIL_C
    // asm: 	LDL	SET18FONTDS_WHITE,R0
    // asm: 	STI	R0,@FONTUSED
    // asm: 	LDF	24,R0
    // asm: 	STF	R0,@YINCREMENT
    // asm: 	FLOAT	420,R6
    // asm: 	LDI	2,AR5
    // asm: 	LDI	@THANKS_TABLEI,AR4
DTLP2:
    // asm: 	ADDF	@YINCREMENT,R6		;ADDF	24,R6
KKPD:
    // asm: FLOAT	402,R0
    // asm: 	CMPF	R0,R6
    // asm: 	BLT	JJDDA
    // asm: 	SLEEP	1
    // asm: 	SUBF	1,R6
    // asm: 	BU	KKPD
JJDDA:
    // asm: 	LDI	*AR4++,R7	;number of entries
    // asm: 	CMPI	10,R7
    // asm: 	BNE	NOT10
    // asm: 	LDI	*AR4++,R0
    // asm: 	STI	R0,@FONTUSED
    // asm: 	LDF	10,R0
    // asm: 	STF	R0,@YINCREMENT
    // asm: 	BU	KKPD
NOT10:
    // asm: 	CMPI	8,R7
    // asm: 	BNE	NOTSPEC8
    // asm: 	FLOAT	*AR4++,R2
    // asm: 	LDF	R6,R3
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CALL	GETT
    // asm: 	BU	DTLP2
NOTSPEC8:
    // asm: 	CMPI	7,R7
    // asm: 	BNE	NOTSPEC7
    // asm: 	FLOAT	*AR4++,R2
    // asm: 	LDF	R6,R3
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CALL	GETT
    // asm: 	CALL	RIGHTEM
    // asm: 	BU	DTLP2
NOTSPEC7:
    // asm: 	CMPI	4,R7
    // asm: 	BNE	NOTSPEC4
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@TEXT_FREEZE
    // asm: 	SLEEP	60
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@TEXT_FREEZE
    // asm: 	BU	KKPD
NOTSPEC4:
    // asm: 	CMPI	99,R7
    // asm: 	BNE	NOTSPEC99
    // asm: 	FLOAT	*AR4++,R2
    // asm: 	FLOAT	(256-600),R2
    // asm: 	LDF	R6,R3
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CALL	GETT
    // asm: 	CALL	CENTEREM
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	AR1,AR5
    // asm: 	CREATE	KRIS,HEADTYPE
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	BU	DTLP2
KRIS:
    // asm: SLEEP	1
    // asm: 	LDF	*+AR4(TEXT_POSX),R0
    // asm: 	FLOAT	259,R1
    // asm: 	SUBF	R0,R1
    // asm: 	MPYF	0.05,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR4(TEXT_POSX)
    // asm: 	LDF	*+AR5(TEXT_POSX),R0
    // asm: 	FLOAT	256,R1
    // asm: 	SUBF	R0,R1
    // asm: 	MPYF	0.05,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR5(TEXT_POSX)
    // asm: 	BU	KRIS
NOTSPEC99:
    // asm: 	CMPI	3,R7
    // asm: 	BNE	NOTSPEC3
    // asm: 	FLOAT	*AR4++,R2
    // asm: 	LDF	R6,R3
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CALL	GETT
    // asm: 	CALL	CENTEREM
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	AR1,AR5
    // asm: 	CREATE	ADAMSTOPRIBYL,HEADTYPE
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	BU	DTLP2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VANITY_SUB", 0, 0);
    UNIMPL();
}

void ADAMSTOPRIBYL(void)
{
    // asm: 	SLEEP	1
    // asm: 	LDF	*+AR4(TEXT_POSY),R0
    // asm: 	FLOAT	250,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	ADAMSTOPRIBYL
    // asm: 	LDI	@CHISPRIB,R0
    // asm: 	STI	R0,*+AR4(TEXT_PTR)
    // asm: 	STI	R0,*+AR5(TEXT_PTR)
    // asm: 	DIE
NOTSPEC3:
    // asm: 	CMPI	-1,R7
    // asm: 	BEQ	WTD55
    // asm: 	CMPI	0,R7
    // asm: 	BNE	NOT0
    // asm: 	BU	DTLP2
NOT0:
    // asm: 	CMPI	2,R7
    // asm: 	BEQ	TWO
    // 	;ONE
    // asm: 	FLOAT	*AR4++,R2
    // asm: 	LDF	R6,R3
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CALL	GETT
    // asm: 	CALL	CENTEREM
    // asm: 	BU	DTLP2
TWO:
    // asm: 	FLOAT	*AR4++,R2
    // asm: 	LDF	R6,R3
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CALL	GETT
    // asm: 	FLOAT	*AR4++,R2
    // asm: 	LDF	R6,R3
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CALL	GETT
    // asm: 	CALL	RIGHTEM
    // asm: 	BU	DTLP2
WTD55:
    // asm: 	LDI	HEADTYPE,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADAMSTOPRIBYL", 0, 0);
    UNIMPL();
}

void GETT(void)
{
    // ;	LDL	SET18FONTDS
    // ;	LDL	SETFIXEDFONTDS
    // asm: 	LDI	9999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	LDF	-1,R0
    // asm: 	STF	R0,*+AR0(TEXT_VELY)
    // asm: 	STF	R0,*+AR1(TEXT_VELY)
    // asm: 	LDI	@FONTUSED,R0
    // asm: 	CALLU	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETT", 0, 0);
    UNIMPL();
}

void CENTEREM(void)
{
    // asm: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm: 	OR	TXT_CENTER,R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm: 	OR	TXT_CENTER,R0
    // asm: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CENTEREM", 0, 0);
    UNIMPL();
}

void RIGHTEM(void)
{
    // asm: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm: 	OR	TXT_RIGHT,R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm: 	OR	TXT_RIGHT,R0
    // asm: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RIGHTEM", 0, 0);
    UNIMPL();
}

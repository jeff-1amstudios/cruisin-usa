#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/vunit.h"
#include "../include/globals.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/texttab.h"
#include "../include/text_defs.h"

/*
 * Source module: asm/TEXT.ASM
 */

// *----------------------------------------------------------------------------
// *TEXT ROUTINES
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: TEXT_FREEI	.word	TEXT_FREE */
int TEXT_FREEI = (int)(TEXT_FREE);
/* asm: TEXT_ACTIVEI	.word	TEXT_ACTIVE */
int TEXT_ACTIVEI = (int)(TEXT_ACTIVE);
/* asm: TEXT_LISTI	.word	TEXT_LIST */
int TEXT_LISTI = (int)(TEXT_LIST);
/* asm: FIXEDFONT_A	.word	fixedfnt */
int FIXEDFONT_A = (int)(fixedfnt);
/* asm: TEXTTABLEFIXEDI	.word	FIXEDFONT */
int TEXTTABLEFIXEDI = (int)(FIXEDFONT);
/* asm: FONTDIGITSM_A	.word	dnums_I */
int FONTDIGITSM_A = (int)(dnums_I);
/* asm: FONT18_A	.word	font18_I */
int FONT18_A = (int)(font18_I);
/* asm: TEXTTABLEFONT18	.word	FONT18_TAB */
int TEXTTABLEFONT18 = (int)(FONT18_TAB);
/* asm: FONT40_A	.word	ommdfont_I */
int FONT40_A = (int)(ommdfont_I);
/* asm: FONT10_A	.word	osg10fnt_I */
int FONT10_A = (int)(osg10fnt_I);
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: FONTN43_A	.word	lgnum43_I */
int FONTN43_A = (int)(lgnum43_I);
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: OGSMFONT_TABI	.word	OGSMFONT_TAB */
int OGSMFONT_TABI = (int)(OGSMFONT_TAB);
// *----------------------------------------------------------------------------

void TEXT_INIT(void)
{
    // asm: 	LDI	@TEXT_LISTI,AR2
    // asm: 	LDI	@TEXT_FREEI,R2
    // asm: 	LDI	@TEXT_ACTIVEI,R3
    // asm: 	LDI	NUM_TEXTS-1,RC
    // asm: 	LDI	TEXT_SIZ,RS
    // asm: 	CALL	INIT_LINKED_LIST
    // asm: 	LDI	NUM_TEXTS,R2
    // asm: 	STI	R2,@TEXT_FREE_COUNT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXT_INIT", 0, 0);
    UNIMPL();
}

void TEXT_ADDDS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *TEXT_ADD(int *string_pointer, float posx, float posy, int tiks)
    // *
    // *PARAMETERS
    // *	AR2	PTR TO PACKED TEXT STRING
    // *	R2	(FL) POS X
    // *	R3	(FL) POS Y
    // *	RC	TIKS TO DISPLAY
    // *RETURNS
    // *	AR0	PTR TO TEXT STRUCTURE
    // *
    // *
    // asm: 	PUSH	AR2
    // asm: 	CALL	TEXT_ADD
    // asm: 	POP	AR2
    // asm: 	LDI	AR0,AR1
    // asm: 	CALL	TEXT_ADD
    // asm: 	LDI	TXT_NRZ,R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm: 	LDF	R2,R0
    // asm: 	ADDF	2,R0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	LDF	R3,R0
    // asm: 	ADDF	2,R0
    // asm: 	STF	R0,*+AR0(TEXT_POSY)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXT_ADDDS", 0, 0);
    UNIMPL();
}

void TEXT_ADD1(void)
{
    // asm: 	LDI	1,RC
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXT_ADD1", 0, 0);
    UNIMPL();
}

void TEXT_ADD(void)
{
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R2
    // asm: 	LDI	@TEXT_FREEI,AR2
    // asm: 	LDI	@TEXT_ACTIVEI,R2
    // asm: 	CALL	GET_LLIST
#if DEBUG
    // asm: 	CMPI	0,AR0
    // asm: 	BZ	$
#endif
    // asm: 	LDI	@TEXT_FREE_COUNT,R2
    // asm: 	DEC	R2
    // asm: 	STI	R2,@TEXT_FREE_COUNT
    // asm: 	POPF	R2
    // asm: 	POP	R2
    // asm: 	POP	AR2
    // asm: 	STI	AR2,*+AR0(TEXT_PTR)
    // asm: 	STF	R2,*+AR0(TEXT_POSX)
    // asm: 	STF	R3,*+AR0(TEXT_POSY)
    // asm: 	STI	RC,*+AR0(TEXT_TIKS)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)	;clear the flags
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR0(TEXT_VELX)
    // asm: 	STF	R0,*+AR0(TEXT_VELY)
    // 	;set default font
    // asm: 	BU	SET18FONT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXT_ADD", 0, 0);
    UNIMPL();
}

void SETSMDIGITFONT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	12,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	LDI	@FONTDIGITSM_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	LDL	dnums_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	LDI	@FONTDIGSMI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETSMDIGITFONT", 0, 0);
    UNIMPL();
}

void SETSMDIGITFONTDS(void)
{
    // asm: 	LDI	12,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm: 	LDI	@FONTDIGITSM_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	STI	R0,*+AR1(TEXT_IMG)
    // asm: 	LDL	dnums_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	LDI	@FONTDIGSMI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETSMDIGITFONTDS", 0, 0);
    UNIMPL();
}

void SETLGDIGITFONT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	22,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	LDI	@FONTDIGITLG_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	LDL	dnums_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	LDI	@FONTDIGLGI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETLGDIGITFONT", 0, 0);
    UNIMPL();
}

void SETLGDIGITFONTDS(void)
{
    // asm: 	LDI	22,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm: 	LDI	@FONTDIGITLG_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	STI	R0,*+AR1(TEXT_IMG)
    // asm: 	LDL	dnums_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	LDI	@FONTDIGLGI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETLGDIGITFONTDS", 0, 0);
    UNIMPL();
}

void SETN43FONT(void)
{
    // asm: 	LDI	40,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	LDI	@FONTN43_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	LDL	lgnum43_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	LDI	@FONTN43TABI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETN43FONT", 0, 0);
    UNIMPL();
}

void SETN43FONTDS(void)
{
    // asm: 	LDI	40,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm: 	LDI	@FONTN43_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	STI	R0,*+AR1(TEXT_IMG)
    // asm: 	LDL	lgnum43_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	LDI	@FONTN43TABI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETN43FONTDS", 0, 0);
    UNIMPL();
}

void SET40FONT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	42,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	LDI	@FONT40_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	LDL	ommdfont_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	LDI	@FONT40TABI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET40FONT", 0, 0);
    UNIMPL();
}

void SET40FONTDS(void)
{
    // asm: 	LDI	42,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm: 	LDI	@FONT40_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	STI	R0,*+AR1(TEXT_IMG)
    // asm: 	LDL	ommdfont_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	LDI	@FONT40TABI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET40FONTDS", 0, 0);
    UNIMPL();
}

void SET12FONT(void)
{
    // asm: 	LDI	12,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	LDI	@FONT10_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	LDI	osg10fnt_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	LDI	@OGSMFONT_TABI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET12FONT", 0, 0);
    UNIMPL();
}

void SET12FONTDS(void)
{
    // asm: 	LDI	12,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm: 	LDI	@FONT10_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	STI	R0,*+AR1(TEXT_IMG)
    // asm: 	LDI	osg10fnt_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	LDI	@OGSMFONT_TABI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET12FONTDS", 0, 0);
    UNIMPL();
}

void SET18FONT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	17,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	LDI	@FONT18_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	LDL	font18_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	LDI	@TEXTTABLEFONT18,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET18FONT", 0, 0);
    UNIMPL();
}

void SET18FONTDS(void)
{
    // asm: 	LDI	17,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm: 	LDI	@FONT18_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	STI	R0,*+AR1(TEXT_IMG)
    // asm: 	LDL	font18_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	LDI	@TEXTTABLEFONT18,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET18FONTDS", 0, 0);
    UNIMPL();
}

void SETFIXEDFONT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	6,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	LDI	@FIXEDFONT_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	LDL	fixedfnt_tPAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	LDI	@TEXTTABLEFIXEDI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETFIXEDFONT", 0, 0);
    UNIMPL();
}

void SETFIXEDFONTDS(void)
{
    // asm: 	LDI	6,R0
    // asm: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm: 	LDI	@FIXEDFONT_A,R0
    // asm: 	STI	R0,*+AR0(TEXT_IMG)
    // asm: 	STI	R0,*+AR1(TEXT_IMG)
    // asm: 	LDL	fixedfnt_tPAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	LDI	@TEXTTABLEFIXEDI,R0
    // asm: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm: 	STI	R0,*+AR1(TEXT_ADDR)
    // ;	LDF	*+AR1(TEXT_POSX),R0
    // ;	SUBF	-1,R0
    // ;	STF	R0,*+AR1(TEXT_POSX)
    // ;	LDF	*+AR1(TEXT_POSY),R0
    // ;	SUBF	-1,R0
    // ;	STF	R0,*+AR1(TEXT_POSY)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETFIXEDFONTDS", 0, 0);
    UNIMPL();
}

void STRLEN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *STRING LENGTH (IN PIXELS)
    // *
    // *
    // *PARAMETERS
    // *	AR2	PTR TO STRING
    // *RETURNS
    // *	R0	LENGTH (IN PIXEL) OF STRING
    // *
    // asm: 	PUSH	RS
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	CLRI	R0			;length of string
    // asm: 	CLRI	RS
    // asm: STRLP	CMPI	-32,RS
    // asm: 	BNE	STLP2
    // asm: 	CLRI	RS
    // asm: 	NOP	*AR2++
STLP2:
    // asm: 	LDI	*AR2,AR0
    // asm: 	LSH	RS,AR0
    // asm: 	SUBI	8,RS
    // asm: 	AND	0FFh,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BZ	STRLENX
    // asm: 	CMPI	' ',AR0
    // asm: 	BEQ	STRLENNCHAR
    // asm: 	SUBI	'0',AR0			;THE START OF THE FONT
    // asm: 	LDI	AR0,AR1
    // asm: 	MPYI	FONTENT_SIZE,AR1
    // asm: 	ADDI	*+AR4(TEXT_ADDR),AR1
    // asm: 	LDI	*+AR1(FONTENT_XEND),R1
    // asm: 	SUBI	*+AR1(FONTENT_XSTART),R1
    // asm: 	LDI	*+AR1(FONTENT_PRE),R2
    // asm: 	LS	16,R2
    // asm: 	ASH	-16,R2			;MUST SIGN EXTEND THIS DUDE
    // asm: 	ADDI	R2,R1
    // asm: 	LDI	*+AR1(FONTENT_TRAIL),R2
    // asm: 	RS	16,R2
    // asm: 	ADDI	R2,R1
STRLENNCHAR:
    // asm: 	ADDI	R1,R0			;INCREASE STRING LENGTH
    // asm: 	BU	STRLP
STRLENX:
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	RS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "STRLEN", 0, 0);
    UNIMPL();
}

void TEXT_OUTPUT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	R4
    // asm: 	PUSH	R7
    // asm: 	BUD	NXTGRP
    // asm: 	PUSHF	R7
    // asm: 	NOP
    // asm: 	LDI	@TEXT_ACTIVEI,AR4
    // 	;---->	BUD	NXTGRP
TEXTLP:
    // asm: 	LDI	R0,AR4
    // asm: 	LDI	*+AR4(TEXT_PTR),AR2
    // asm: 	FIX	*+AR4(TEXT_POSX),R2
    // asm: 	FIX	*+AR4(TEXT_POSY),R3
    // asm:  	LDI	*+AR4(TEXT_COLOR),R0
    // asm: 	TSTB	TXT_CENTER,R0
    // asm: 	BZ	NO_CENTER
    // asm: 	CALL	STRLEN
    // asm: 	RS	1,R0
    // asm: 	SUBI	R0,R2
    // asm: 	B	TEXT_RET
NO_CENTER:
    // asm: 	TSTB	TXT_RIGHT,R0
    // asm: 	BZ	NO_RIGHT
    // asm: 	CALL	STRLEN
    // asm: 	SUBI	R0,R2
    // 	;B	TEXT_RET
NO_RIGHT:
TEXT_RET:
    // asm: 	CLRI	RS
OLP:
    // asm: CMPI	-32,RS
    // asm: 	BNE	REGLP
    // asm: 	CLRI	RS
    // asm: 	NOP	*AR2++
REGLP:
    // asm: 	LDI	*AR2,AR0
    // asm: 	LSH	RS,AR0
    // asm: 	SUBI	8,RS
    // asm: 	AND	0FFh,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BZ	OUCX
    // asm: 	CMPI	'/',AR0
    // asm: 	LDIEQ	'@',AR0
    // asm: 	CMPI	' ',AR0
    // asm: 	BEQ	NXTCHAR
    // asm: 	SUBI	'0',AR0			;the start of the font
    // 	;NOW PLOT OUT THE CHARACTER
    // asm:  	LDI	*+AR4(TEXT_COLOR),R0
    // asm: 	TSTB	TXT_NRZ,R0
    // asm: 	BZ	IBO1
    // asm: 	AND	0FFh,R0
    // asm: 	OR	NZR|ZS|TM,R0
    // asm: 	BU	IBO2
IBO1:
    // asm: LDI	TM|ZS,R0
IBO2:
    // asm: 	STI	R0,@_ACNTL
    // asm: 	LDI	*+AR4(TEXT_PAL),R1
    // asm: 	STI	R1,@_ACMAP
    // asm: 	LDI	AR0,AR1
    // asm: 	MPYI	FONTENT_SIZE,AR1
    // asm: 	ADDI	*+AR4(TEXT_ADDR),AR1
    // asm: 	LDI	*+AR1(FONTENT_PRE),R0
    // asm: 	LS	16,R0
    // asm: 	ASH	-16,R0			;MUST SIGN EXTEND THIS DUDE
    // asm: 	ADDI	R0,R2
    // asm: 	LDI	*+AR1(FONTENT_XSTART),R0
    // asm: 	LDI	*+AR1(FONTENT_YSTART),R4
    // asm: 	LS	8,R4
    // asm: 	OR	R4,R0
    // asm: 	STI	R0,@_AIVI+0
    // asm: 	LDI	*+AR4(TEXT_HEIGHT),R1
    // asm: 	LS	8,R1
    // asm: 	ADDI	R0,R1
    // asm: 	STI	R1,@_AIVI+3
    // asm: 	LDI	*+AR1(FONTENT_XEND),R0
    // asm: 	OR	R4,R0
    // asm: 	STI	R0,@_AIVI+1
    // asm: 	LDI	*+AR4(TEXT_HEIGHT),R1
    // asm: 	LS	8,R1
    // asm: 	ADDI	R0,R1
    // asm: 	STI	R1,@_AIVI+2
    // asm: 	LDI	*+AR4(TEXT_IMG),R0
    // asm: 	STI	R0,@_ADDRL
    // asm: 	LDI	*+AR1(FONTENT_XEND),R7
    // asm: 	SUBI	*+AR1(FONTENT_XSTART),R7
    // asm: 	STI	R2,@_ARPS+0
    // asm: 	STI	R2,@_ARPS+9	;FP3X
    // asm: 	ADDI	R7,R2
    // asm: 	STI	R2,@_ARPS+3	;FP1X
    // asm: 	STI	R2,@_ARPS+6	;FP2X
    // asm: 	SUBI	R7,R2
    // asm: 	STI	R3,@_ARPS+1	;FP0Y
    // asm: 	STI	R3,@_ARPS+4	;FP1Y
    // asm: 	ADDI	*+AR4(TEXT_HEIGHT),R3
    // asm: 	STI	R3,@_ARPS+7	;FP2Y
    // asm: 	STI	R3,@_ARPS+10	;FP3Y
    // asm: 	SUBI	*+AR4(TEXT_HEIGHT),R3
    // asm: 	CALL	_stuff_fpga
    // asm: 	LDI	*+AR1(FONTENT_TRAIL),R0
    // asm: 	RS	16,R0
    // asm: 	ADDI	R0,R7
NXTCHAR:
    // asm: 	ADDI	R7,R2			;to next X position
    // asm: 	BU	OLP
OUCX:
    // asm: 	LDI	@TEXT_FREEZE,R0
    // asm: 	BNZ	ISFROZEN
    // asm: 	LDF	*+AR4(TEXT_POSX),R0
    // asm: 	ADDF	*+AR4(TEXT_VELX),R0
    // asm: 	STF	R0,*+AR4(TEXT_POSX)
    // asm: 	LDF	*+AR4(TEXT_POSY),R0
    // asm: 	ADDF	*+AR4(TEXT_VELY),R0
    // asm: 	STF	R0,*+AR4(TEXT_POSY)
ISFROZEN:
    // asm: 	LDI	*+AR4(TEXT_TIKS),R0
    // asm: 	LDI	*+AR4(TEXT_COLOR),R1
    // asm: 	TSTB	TXT_NOPULL,R1		;NOPULL = never decrement tik count
    // asm: 	BNZ	NODELETE
    // asm: 	SUBI	1,R0
    // asm: 	BGT	NODELETE
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
    // asm: 	BNZ	TEXTLP
    // asm: 	B	TXTOUT
NODELETE:
    // asm: 	STI	R0,*+AR4(TEXT_TIKS)
NXTGRP:
    // asm: 	LDI	*AR4,R0
    // asm: 	BNZ	TEXTLP
TXTOUT:
    // asm: 	POPF	R7
    // asm: 	POP	R7
    // asm: 	POP	R4
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXT_OUTPUT", 0, 0);
    UNIMPL();
}

void STRCPY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *STRING COPY
    // *
    // *PARAMETERS
    // *	AR0	SOURCE STRING
    // *	AR1	DESTINATION STRING
    // *RETURNS
    // *	AR1	SOURCE STRING
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // 	;FIND WHERE AR0 ENDS
    // asm: 	CLRI	R0
SCPLP0:
    // asm: CMPI	-32,R0
    // asm: 	BNE	REGPLP0
    // asm: 	CLRI	R0
    // asm: 	NOP	*AR0++
    // asm: 	NOP	*AR1++
REGPLP0:
    // asm: 	LDI	*AR0,AR2
    // asm: 	STI	AR2,*AR1
    // asm: 	LSH	R0,AR2
    // asm: 	SUBI	8,R0
    // asm: 	AND	0FFh,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BNZ	SCPLP0
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "STRCPY", 0, 0);
    UNIMPL();
}

void STRCAT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *STRING CONCATENATION
    // *
    // *APPEND THE CONTENTS OF AR1 TO AR0
    // *
    // *PARAMETERS
    // *	AR0	ORIGINAL STRING (W/SPACE FOR ADDITION)
    // *	AR1	APPEND STRING
    // *RETURNS
    // *	AR0	ORIGINAL STRING + APPEND STRING
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // 	;FIND WHERE AR0 ENDS
    // asm: 	CLRI	R0
SCLP0:
    // asm: CMPI	-32,R0
    // asm: 	BNE	REGLP0
    // asm: 	CLRI	R0
    // asm: 	NOP	*AR0++
REGLP0:
    // asm: 	LDI	*AR0,AR2
    // asm: 	LSH	R0,AR2
    // asm: 	SUBI	8,R0
    // asm: 	AND	0FFh,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BNZ	SCLP0
    // asm: 	ADDI	8,R0
    // 	;NOW APPEND THE DATA
    // asm: 	CLRI	R1
SCLP1:
    // asm: CMPI	-32,R1
    // asm: 	BNE	REGLP1
    // asm: 	CLRI	R1
    // asm: 	NOP	*AR1++
REGLP1:
    // asm: 	LDI	*AR1,AR3
    // asm: 	LSH	R1,AR3
    // asm: 	SUBI	8,R1
    // asm: 	AND	0FFh,AR3
    // asm: 	LDI	AR3,AR4
    // asm: 	NEGI	R0,R3
    // asm: 	LSH	R3,AR3
    // asm: 	LDI	*AR0,R4
    // asm: 	OR	AR3,R4
    // asm: 	STI	R4,*AR0
    // asm: 	SUBI	8,R0
    // asm: 	CMPI	-32,R0
    // asm: 	BNE	REGLP2
    // asm: 	CLRI	R0
    // asm: 	NOP	*AR0++
    // asm: 	STI	R0,*AR0			;IN THE CASE OF NULL ALIGNED
REGLP2:
    // asm: 	CMPI	0,AR4
    // asm: 	BNZ	SCLP1
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "STRCAT", 0, 0);
    UNIMPL();
}

void FIXEDFONT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *FONTENT		.macro	PRECEDING,XSTART,XEND,YSTART,TRAIL
    // *
    // *
    // asm: 	FONTENT	0,64,71,0,1	;0
    // asm: 	FONTENT	0,72,79,0,1	;1
    // asm: 	FONTENT	0,80,87,0,1	;2
    // asm: 	FONTENT	0,88,95,0,1	;3
    // asm: 	FONTENT	0,96,103,0,1	;4
    // asm: 	FONTENT	0,104,111,0,1	;5
    // asm: 	FONTENT	0,112,119,0,1	;6
    // asm: 	FONTENT	0,120,127,0,1	;7
    // asm: 	FONTENT	0,128,135,0,1	;8
    // asm: 	FONTENT	0,136,143,0,1	;9
    // 				;special characters:
    // 				;ASCII		actual
    // asm: 	FONTENT	0,144,151,0,1	; :
    // asm: 	FONTENT	0,152,159,0,1	; ;
    // asm: 	FONTENT	0,160,167,0,1	; <
    // asm: 	FONTENT	0,48,55,0,1	; =	.
    // asm: 	FONTENT	0,32,39,0,1	; >	,
    // asm: 	FONTENT	0,40,47,0,1	; ?	-
    // asm: 	FONTENT	0,56,63,0,1	; @	/
    // asm: 	FONTENT	0,200,207,0,1	;A
    // asm: 	FONTENT	0,208,215,0,1
    // asm: 	FONTENT	0,216,223,0,1
    // asm: 	FONTENT	0,224,231,0,1	;D
    // asm: 	FONTENT	0,232,239,0,1	;E
    // asm: 	FONTENT	0,240,247,0,1	;F
    // asm: 	FONTENT	0,248,255,0,1	;G
    // asm: 	FONTENT	0,0,7,7,1	;H
    // asm: 	FONTENT	0,8,15,7,1
    // asm: 	FONTENT	0,16,23,7,1
    // asm: 	FONTENT	0,24,31,7,1
    // asm: 	FONTENT	0,32,39,7,1	;L
    // asm: 	FONTENT	0,40,47,7,1	;M
    // asm: 	FONTENT	0,48,55,7,1	;N
    // asm: 	FONTENT	0,56,63,7,1	;O
    // asm: 	FONTENT	0,64,71,7,1	;P
    // asm: 	FONTENT	0,72,79,7,1	;Q
    // asm: 	FONTENT	0,80,87,7,1	;R
    // asm: 	FONTENT	0,88,95,7,1
    // asm: 	FONTENT	0,96,103,7,1
    // asm: 	FONTENT	0,104,111,7,1	;U
    // asm: 	FONTENT	0,112,119,7,1
    // asm: 	FONTENT	0,120,127,7,1
    // asm: 	FONTENT	0,128,135,7,1
    // asm: 	FONTENT	0,136,143,7,1
    // asm: 	FONTENT	0,144,151,7,1	;Z
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIXEDFONT", 0, 0);
    UNIMPL();
}

void HIGHLIGHTN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *PARAMETERS
    // *	AR2	PTR TO TEXT ENTRY
    // *	R2	CHARACTER TO HIGHLIGHT
    // *	R3	PALETTE
    // *
    // asm: 	INC	R2
    // asm: 	CALL	PUSHALL
    // asm: 	LDI	R2,IR0
    // asm: 	LDI	R3,IR1
    // asm: 	LDI	AR2,AR4
    // asm: 	LDI	*+AR4(TEXT_PTR),AR2
    // asm: 	FIX	*+AR4(TEXT_POSX),R2
    // asm: 	FIX	*+AR4(TEXT_POSY),R3
    // asm:  	LDI	*+AR4(TEXT_COLOR),R0
    // asm: 	TSTB	TXT_CENTER,R0
    // asm: 	BZ	NO_CENTERa
    // asm: 	CALL	STRLEN
    // asm: 	RS	1,R0
    // asm: 	SUBI	R0,R2
    // asm: 	B	text_reta
NO_CENTERa:
    // asm: 	TSTB	TXT_RIGHT,R0
    // asm: 	BZ	NO_RIGHTa
    // asm: 	CALL	STRLEN
    // asm: 	SUBI	R0,R2
    // 	;---->B	text_reta
NO_RIGHTa:
text_reta:
    // asm: 	CLRI	BK
    // asm: 	CLRI	RS
OLPa:
    // asm: INC	BK
    // asm: 	CMPI	-32,RS
    // asm: 	BNE	REGLPa
    // asm: 	CLRI	RS
    // asm: 	NOP	*AR2++
REGLPa:
    // asm: 	LDI	*AR2,AR0
    // asm: 	LSH	RS,AR0
    // asm: 	SUBI	8,RS
    // asm: 	AND	0FFh,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BZ	oucXa
    // asm: 	CMPI	' ',AR0
    // asm: 	BEQ	NXTCHARa
    // asm: 	SUBI	'0',AR0			;the start of the font
    // asm: 	CMPI	IR0,BK
    // asm: 	BEQ	ALLREG
    // asm: 	BGT	oucXa
    // asm: 	LDI	AR0,AR1
    // asm: 	MPYI	FONTENT_SIZE,AR1
    // asm: 	ADDI	*+AR4(TEXT_ADDR),AR1
    // asm: 	LDI	*+AR1(FONTENT_XEND),R7
    // asm: 	SUBI	*+AR1(FONTENT_XSTART),R7
    // asm: 	LDI	*+AR1(FONTENT_PRE),R0
    // asm: 	LS	16,R0
    // asm: 	ASH	-16,R0			;MUST SIGN EXTEND THIS DUDE
    // asm: 	ADDI	R0,R7
    // asm: 	LDI	*+AR1(FONTENT_TRAIL),R0
    // asm: 	ASH	-16,R0
    // asm: 	ADDI	R0,R7
    // asm: 	BU	NXTCHARa
ALLREG:
    // 	;NOW PLOT OUT THE CHARACTER
    // asm:  	LDI	*+AR4(TEXT_COLOR),R0
    // asm: 	AND	0FFh,R0
    // asm: 	BZ	IBO1a
    // asm: 	OR	NZR,R0
IBO1a:
    // asm: OR	TM|ZS,R0
    // asm: 	LDI	TM|ZS|NZR|6,R0
    // asm: 	STI	R0,@_ACNTL
    // asm: 	CLRI	IR1			;palette 0 ignore given
    // asm: 	STI	IR1,@_ACMAP		;SPECIAL PALETTE
    // asm: 	LDI	AR0,AR1
    // asm: 	MPYI	FONTENT_SIZE,AR1
    // asm: 	ADDI	*+AR4(TEXT_ADDR),AR1
    // asm: 	LDI	*+AR1(FONTENT_PRE),R0
    // asm: 	LS	16,R0
    // asm: 	ASH	-16,R0			;MUST SIGN EXTEND THIS DUDE
    // asm: 	ADDI	R0,R2
    // asm: 	LDI	*+AR1(FONTENT_XSTART),R0
    // asm: 	LDI	*+AR1(FONTENT_YSTART),R4
    // asm: 	LS	8,R4
    // asm: 	OR	R4,R0
    // asm: 	STI	R0,@_AIVI+0
    // asm: 	LDI	*+AR4(TEXT_HEIGHT),R1
    // asm: 	LS	8,R1
    // asm: 	ADDI	R0,R1
    // asm: 	STI	R1,@_AIVI+3
    // asm: 	LDI	*+AR1(FONTENT_XEND),R0
    // asm: 	OR	R4,R0
    // asm: 	STI	R0,@_AIVI+1
    // asm: 	LDI	*+AR4(TEXT_HEIGHT),R1
    // asm: 	LS	8,R1
    // asm: 	ADDI	R0,R1
    // asm: 	STI	R1,@_AIVI+2
    // asm: 	LDI	*+AR4(TEXT_IMG),R0
    // asm: 	STI	R0,@_ADDRL
    // asm: 	LDI	*+AR1(FONTENT_XEND),R7
    // asm: 	SUBI	*+AR1(FONTENT_XSTART),R7
    // asm: 	STI	R2,@_ARPS+0
    // asm: 	STI	R2,@_ARPS+9	;FP3X
    // asm: 	ADDI	R7,R2
    // asm: 	STI	R2,@_ARPS+3	;FP1X
    // asm: 	STI	R2,@_ARPS+6	;FP2X
    // asm: 	SUBI	R7,R2
    // asm: 	STI	R3,@_ARPS+1	;FP0Y
    // asm: 	STI	R3,@_ARPS+4	;FP1Y
    // asm: 	ADDI	*+AR4(TEXT_HEIGHT),R3
    // asm: 	DEC	R3
    // asm: 	STI	R3,@_ARPS+7	;FP2Y
    // asm: 	STI	R3,@_ARPS+10	;FP3Y
    // asm: 	SUBI	*+AR4(TEXT_HEIGHT),R3
    // asm: 	INC	R3
    // asm: 	CALL	_stuff_fpga
    // asm: 	LDI	*+AR1(FONTENT_TRAIL),R0
    // asm: 	ASH	-16,R0
    // asm: 	ADDI	R0,R7
NXTCHARa:
    // asm: 	ADDI	R7,R2			;to next X position
    // asm: 	BU	OLPa
oucXa:
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIGHLIGHTN", 0, 0);
    UNIMPL();
}

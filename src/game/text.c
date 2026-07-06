#include "text.h"

#include "../core/machine.h"
#include "globals.h"
#include "macs.h"
#include "pall.h"
#include "texttab.h"
#include "validator.h"
#include "vunit.h"

/*
 * Source module: asm/TEXT.ASM
 */

void TEXT_INIT(void);
tSHADOW_TEXT TEXT_ADDDS(const char* text, float x, float y, int ticks);
void TEXT_ADD1(void);
tTEXT* TEXT_ADD(const char* text, float x, float y, int ticks);
void SETSMDIGITFONT(tTEXT* t /*AR0*/);
void SETSMDIGITFONTDS(tSHADOW_TEXT* t);
void SETLGDIGITFONT(tTEXT* t /*AR0*/);
void SETLGDIGITFONTDS(tSHADOW_TEXT* t);
void SETN43FONT(tTEXT* t /*AR0*/);
void SETN43FONTDS(tSHADOW_TEXT* t);
void SET40FONT(tTEXT* t /*AR0*/);
void SET40FONTDS(tSHADOW_TEXT* t);
void SET12FONT(tTEXT* t /*AR0*/);
void SET12FONTDS(tSHADOW_TEXT* t);
void SET18FONT(tTEXT* t /*AR0*/);
void SET18FONTDS(tSHADOW_TEXT* t);
void SETFIXEDFONT(tTEXT* t /*AR0*/);
void SETFIXEDFONTDS(tSHADOW_TEXT* t);
static int STRLEN(const tTEXT* text /*AR2*/);
#define TEXT_FONT_PRE(font) ((int16_t)((font)->pre_and_trail & 0xFFFF))
#define TEXT_FONT_TRAIL(font) ((u16)((font)->pre_and_trail >> 16))
void TEXT_OUTPUT(void);
void STRCPY(void);
void STRCAT(char* dest, const char* src);
void HIGHLIGHTN(void);

#define TEXT_FREEI TEXT_FREE
#define TEXT_ACTIVEI TEXT_ACTIVE
#define TEXT_LISTI TEXT_LIST
#define TEXTTABLEFIXEDI FIXEDFONT
#define OGSMFONT_TABI OGSMFONT_TAB

static int FIXEDFONT;

/*
 *----------------------------------------------------------------------------
 *TEXT ROUTINES
 *
 *COPYRIGHT (C) 1994  BY TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

tTEXT TEXT_LIST[NUM_TEXTS];
/* asm: TEXT_FREE	.bss	TEXT_FREE,1 */
tTEXT* TEXT_FREE;
/* asm: TEXT_ACTIVE	.bss	TEXT_ACTIVE,1 */
tTEXT* TEXT_ACTIVE;
/* asm: TEXT_FREE_COUNT	.bss	TEXT_FREE_COUNT,1 */
int TEXT_FREE_COUNT;
/* asm: TEXT_FREEZE	.bss	TEXT_FREEZE,1 */
int TEXT_FREEZE;
/* asm: FIXEDFONT_A	.word	fixedfnt */
static int FIXEDFONT_A = fixedfnt;
/* asm: TEXTTABLEFIXEDI	.word	FIXEDFONT */
/* asm: 	 */
#define TEXTTABLEFIXEDI FIXEDFONT
/* asm: FONTDIGITSM_A	.word	dnums_I */
/* asm: 	 */
static int FONTDIGITSM_A = dnums_I;
/* asm: FONT18_A	.word	font18_I */
static int FONT18_A = font18_I;
/* asm: TEXTTABLEFONT18	.word	FONT18_TAB */
static FONTENTRY* TEXTTABLEFONT18 = FONT18_TAB;
/* asm: FONT40_A	.word	ommdfont_I */
static int FONT40_A = ommdfont_I;
/* asm: FONT10_A	.word	osg10fnt_I */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
static int FONT10_A = osg10fnt_I;

// *----------------------------------------------------------------------------
void TEXT_INIT(void) {
    // asm:
    INIT_LINKED_LIST(
        TEXT_LIST,            /* AR2 */
        (void**)&TEXT_FREE,   /* R2 */
        (void**)&TEXT_ACTIVE, /* R3 */
        NUM_TEXTS - 1,        /* RC */
        sizeof(tTEXT)         /* RS */
    );

    TEXT_FREE_COUNT = NUM_TEXTS;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *TEXT_ADD(int *string_pointer, float posx, float posy, int tiks)
 *
 *PARAMETERS
 *	AR2	PTR TO PACKED TEXT STRING
 *	R2	(FL) POS X
 *	R3	(FL) POS Y
 *	RC	TIKS TO DISPLAY
 *RETURNS
 *	AR0	PTR TO TEXT STRUCTURE
 *
 *
 */
tSHADOW_TEXT TEXT_ADDDS(const char* text, float x, float y, int ticks) {
    tTEXT* front;
    tTEXT* shadow;

    // MAME_VALIDATE_ARG_FLOAT("R2", &x);
    // MAME_VALIDATE_ARG_FLOAT("R3", &y);

    front = TEXT_ADD(text, x, y, ticks);
    shadow = TEXT_ADD(text, x, y, ticks);
    shadow->color = TXT_NRZ;
    shadow->posx = x + 2.0f;
    shadow->posy = y + 2.0f;

    tSHADOW_TEXT ret;
    ret.front = front;
    ret.shadow = shadow;
    return ret;
}

void TEXT_ADD1(void) {
    // asm 0000798A: 	LDI	1,RC
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXT_ADD1", 0, 0);
    UNIMPL();
}

tTEXT* TEXT_ADD(const char* text, float x, float y, int ticks) {
    tTEXT* t;

    t = GET_LLIST((void**)&TEXT_FREE, (void**)&TEXT_ACTIVE);

#if DEBUG
    if (t == NULL) {
        for (;;) {
            /* debug lockup */
        }
    }
#endif

    TEXT_FREE_COUNT--;

    t->ptr = text;
    t->posx = x;
    t->posy = y;

    t->tiks = ticks;

    t->color = 0; /* clear the flags */

    t->velx = 0.0f;
    t->vely = 0.0f;

    /* set default font */
    SET18FONT(t);
    return t;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SETSMDIGITFONT(tTEXT* t /*AR0*/) {
    // asm 000079A1: 	LDI	12,R0
    // asm 000079A2: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 000079A3: 	LDI	@FONTDIGITSM_A,R0
    // asm 000079A4: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 000079A5: 	LDL	dnums_p,AR2
    // asm 000079A6: 	CALL	PAL_FIND
    // asm 000079A7: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000079A8: 	LDI	@FONTDIGSMI,R0
    // asm 000079A9: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 000079AA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETSMDIGITFONT", 0, 0);
    UNIMPL();
}

void SETSMDIGITFONTDS(tSHADOW_TEXT* t) {
    // asm 000079AB: 	LDI	12,R0
    // asm 000079AC: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 000079AD: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm 000079AE: 	LDI	@FONTDIGITSM_A,R0
    // asm 000079AF: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 000079B0: 	STI	R0,*+AR1(TEXT_IMG)
    // asm 000079B1: 	LDL	dnums_p,AR2
    // asm 000079B2: 	CALL	PAL_FIND
    // asm 000079B3: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000079B4: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 000079B5: 	LDI	@FONTDIGSMI,R0
    // asm 000079B6: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 000079B7: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm 000079B8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETSMDIGITFONTDS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SETLGDIGITFONT(tTEXT* t /*AR0*/) {
    // asm 000079B9: 	LDI	22,R0
    // asm 000079BA: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 000079BB: 	LDI	@FONTDIGITLG_A,R0
    // asm 000079BC: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 000079BD: 	LDL	dnums_p,AR2
    // asm 000079BE: 	CALL	PAL_FIND
    // asm 000079BF: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000079C0: 	LDI	@FONTDIGLGI,R0
    // asm 000079C1: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 000079C2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETLGDIGITFONT", 0, 0);
    UNIMPL();
}

void SETLGDIGITFONTDS(tSHADOW_TEXT* t) {
    // asm 000079C3: 	LDI	22,R0
    // asm 000079C4: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 000079C5: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm 000079C6: 	LDI	@FONTDIGITLG_A,R0
    // asm 000079C7: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 000079C8: 	STI	R0,*+AR1(TEXT_IMG)
    // asm 000079C9: 	LDL	dnums_p,AR2
    // asm 000079CA: 	CALL	PAL_FIND
    // asm 000079CB: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000079CC: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 000079CD: 	LDI	@FONTDIGLGI,R0
    // asm 000079CE: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 000079CF: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm 000079D0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETLGDIGITFONTDS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: FONTN43_A	.word	lgnum43_I */
static int FONTN43_A = lgnum43_I;

void SETN43FONT(tTEXT* t /*AR0*/) {
    // asm 000079D2: 	LDI	40,R0
    // asm 000079D3: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 000079D4: 	LDI	@FONTN43_A,R0
    // asm 000079D5: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 000079D6: 	LDL	lgnum43_p,AR2
    // asm 000079D7: 	CALL	PAL_FIND
    // asm 000079D8: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000079D9: 	LDI	@FONTN43TABI,R0
    // asm 000079DA: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 000079DB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETN43FONT", 0, 0);
    UNIMPL();
}

void SETN43FONTDS(tSHADOW_TEXT* t) {
    // asm 000079DC: 	LDI	40,R0
    // asm 000079DD: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 000079DE: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm 000079DF: 	LDI	@FONTN43_A,R0
    // asm 000079E0: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 000079E1: 	STI	R0,*+AR1(TEXT_IMG)
    // asm 000079E2: 	LDL	lgnum43_p,AR2
    // asm 000079E3: 	CALL	PAL_FIND
    // asm 000079E4: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000079E5: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 000079E6: 	LDI	@FONTN43TABI,R0
    // asm 000079E7: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 000079E8: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm 000079E9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETN43FONTDS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SET40FONT(tTEXT* t /*AR0*/) {
    // asm 000079EA: 	LDI	42,R0
    // asm 000079EB: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 000079EC: 	LDI	@FONT40_A,R0
    // asm 000079ED: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 000079EE: 	LDL	ommdfont_p,AR2
    // asm 000079EF: 	CALL	PAL_FIND
    // asm 000079F0: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000079F1: 	LDI	@FONT40TABI,R0
    // asm 000079F2: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 000079F3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET40FONT", 0, 0);
    UNIMPL();
}

void SET40FONTDS(tSHADOW_TEXT* t) {
    // asm 000079F4: 	LDI	42,R0
    // asm 000079F5: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 000079F6: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm 000079F7: 	LDI	@FONT40_A,R0
    // asm 000079F8: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 000079F9: 	STI	R0,*+AR1(TEXT_IMG)
    // asm 000079FA: 	LDL	ommdfont_p,AR2
    // asm 000079FB: 	CALL	PAL_FIND
    // asm 000079FC: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000079FD: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 000079FE: 	LDI	@FONT40TABI,R0
    // asm 000079FF: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 00007A00: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm 00007A01: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET40FONTDS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

void SET12FONT(tTEXT* t /*AR0*/) {
    // asm 00007A03: 	LDI	12,R0
    // asm 00007A04: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 00007A05: 	LDI	@FONT10_A,R0
    // asm 00007A06: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 00007A07: 	LDI	osg10fnt_p,AR2
    // asm 00007A08: 	CALL	PAL_FIND
    // asm 00007A09: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00007A0A: 	LDI	@OGSMFONT_TABI,R0
    // asm 00007A0B: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 00007A0C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET12FONT", 0, 0);
    UNIMPL();
}

void SET12FONTDS(tSHADOW_TEXT* t) {
    int palette;

    // asm 00007A0D: 	LDI	12,R0
    // asm 00007A0E: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 00007A0F: 	STI	R0,*+AR1(TEXT_HEIGHT)
    t->front->height = 12;
    t->shadow->height = 12;
    // asm 00007A10: 	LDI	@FONT10_A,R0
    // asm 00007A11: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 00007A12: 	STI	R0,*+AR1(TEXT_IMG)
    t->front->image_addr = FONT10_A;
    t->shadow->image_addr = FONT10_A;
    // asm 00007A13: 	LDI	osg10fnt_p,AR2
    // asm 00007A14: 	CALL	PAL_FIND
    palette = PAL_FIND(osg10fnt_p);
    // asm 00007A15: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00007A16: 	STI	R0,*+AR1(TEXT_PAL)
    t->front->palette = palette;
    t->shadow->palette = palette;
    // asm 00007A17: 	LDI	@OGSMFONT_TABI,R0
    // asm 00007A18: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 00007A19: 	STI	R0,*+AR1(TEXT_ADDR)
    t->front->text_addr = OGSMFONT_TAB;
    t->shadow->text_addr = OGSMFONT_TAB;
    // asm 00007A1A: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SET18FONT(tTEXT* t) {
    // asm 00007A1B: 	LDI	17,R0
    // asm 00007A1C: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 00007A1D: 	LDI	@FONT18_A,R0
    // asm 00007A1E: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 00007A1F: 	LDL	font18_p,AR2
    // asm 00007A20: 	CALL	PAL_FIND
    // asm 00007A21: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00007A22: 	LDI	@TEXTTABLEFONT18,R0
    // asm 00007A23: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 00007A24: 	RETS
    t->height = 17;
    t->image_addr = FONT18_A;

    t->palette = PAL_FIND(font18_p_ROM);

    t->text_addr = TEXTTABLEFONT18;
}

void SET18FONTDS(tSHADOW_TEXT* t) {
    // asm 00007A25: 	LDI	17,R0
    // asm 00007A26: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 00007A27: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm 00007A28: 	LDI	@FONT18_A,R0
    // asm 00007A29: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 00007A2A: 	STI	R0,*+AR1(TEXT_IMG)
    // asm 00007A2B: 	LDL	font18_p,AR2
    // asm 00007A2C: 	CALL	PAL_FIND
    // asm 00007A2D: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00007A2E: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 00007A2F: 	LDI	@TEXTTABLEFONT18,R0
    // asm 00007A30: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 00007A31: 	STI	R0,*+AR1(TEXT_ADDR)
    // asm 00007A32: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET18FONTDS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SETFIXEDFONT(tTEXT* t /*AR0*/) {
    // asm 00007A33: 	LDI	6,R0
    // asm 00007A34: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 00007A35: 	LDI	@FIXEDFONT_A,R0
    // asm 00007A36: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 00007A37: 	LDL	fixedfnt_tPAL,AR2
    // asm 00007A38: 	CALL	PAL_FIND_RAW
    // asm 00007A39: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00007A3A: 	LDI	@TEXTTABLEFIXEDI,R0
    // asm 00007A3B: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 00007A3C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETFIXEDFONT", 0, 0);
    UNIMPL();
}

void SETFIXEDFONTDS(tSHADOW_TEXT* t) {
    // asm 00007A3D: 	LDI	6,R0
    // asm 00007A3E: 	STI	R0,*+AR0(TEXT_HEIGHT)
    // asm 00007A3F: 	STI	R0,*+AR1(TEXT_HEIGHT)
    // asm 00007A40: 	LDI	@FIXEDFONT_A,R0
    // asm 00007A41: 	STI	R0,*+AR0(TEXT_IMG)
    // asm 00007A42: 	STI	R0,*+AR1(TEXT_IMG)
    // asm 00007A43: 	LDL	fixedfnt_tPAL,AR2
    // asm 00007A44: 	CALL	PAL_FIND_RAW
    // asm 00007A45: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00007A46: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 00007A47: 	LDI	@TEXTTABLEFIXEDI,R0
    // asm 00007A48: 	STI	R0,*+AR0(TEXT_ADDR)
    // asm 00007A49: 	STI	R0,*+AR1(TEXT_ADDR)
    // ;	LDF	*+AR1(TEXT_POSX),R0
    // ;	SUBF	-1,R0
    // ;	STF	R0,*+AR1(TEXT_POSX)
    // ;	LDF	*+AR1(TEXT_POSY),R0
    // ;	SUBF	-1,R0
    // ;	STF	R0,*+AR1(TEXT_POSY)
    // asm 00007A4A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETFIXEDFONTDS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *STRING LENGTH (IN PIXELS)
 *
 *
 *PARAMETERS
 *	AR2	PTR TO STRING
 *RETURNS
 *	R0	LENGTH (IN PIXEL) OF STRING
 *
 */
static int STRLEN(const tTEXT* text /*AR2*/) {
    FONTENTRY* font;
    const char* str;
    int length;
    int ch;
    int char_width;

    // asm 00007A4B: 	PUSH	RS
    // asm 00007A4C: 	PUSH	R1
    // asm 00007A4D: 	PUSH	R2
    // asm 00007A4E: 	PUSH	AR0
    // asm 00007A4F: 	PUSH	AR1
    // asm 00007A50: 	PUSH	AR2
    // asm 00007A51: 	CLRI	R0			;length of string
    // asm 00007A52: 	CLRI	RS
    str = text->ptr;
    length = 0; // ;length of string
STRLP:
    ch = (unsigned char)*str++;

    // asm 00007A5B: 	CMPI	0,AR0
    // asm 00007A5C: 	BZ	STRLENX
    if (ch == 0) {
        goto STRLENX;
    }

    // asm 00007A5D: 	CMPI	' ',AR0
    // asm 00007A5E: 	BEQ	STRLENNCHAR
    if (ch == ' ') {
        goto STRLENNCHAR;
    }

    // TEXT_OUTPUT remaps '/' to '@' before indexing the font table.
    if (ch == '/') {
        ch = '@';
    }

    // asm 00007A5F: 	SUBI	'0',AR0     ;THE START OF THE FONT
    ch -= '0'; // ;THE START OF THE FONT

    // asm 00007A60: 	LDI	AR0,AR1
    // asm 00007A61: 	MPYI	FONTENT_SIZE,AR1
    // asm 00007A62: 	ADDI	*+AR4(TEXT_ADDR),AR1
    font = &text->text_addr[ch];

    // asm 00007A63: 	LDI	*+AR1(FONTENT_XEND),R1
    // asm 00007A64: 	SUBI	*+AR1(FONTENT_XSTART),R1
    char_width = (int)font->x_end - (int)font->x_start;

    // asm 00007A65: 	LDI	*+AR1(FONTENT_PRE),R2
    // asm 00007A66: 	LS	16,R2
    // asm 00007A67: 	ASH	-16,R2			;MUST SIGN EXTEND THIS DUDE
    // asm 00007A68: 	ADDI	R2,R1
    char_width += TEXT_FONT_PRE(font);

    // asm 00007A69: 	LDI	*+AR1(FONTENT_TRAIL),R2
    // asm 00007A6A: 	RS	16,R2
    // asm 00007A6B: 	ADDI	R2,R1
    char_width += TEXT_FONT_TRAIL(font);

STRLENNCHAR:
    // asm 00007A6C: 	ADDI	R1,R0			;INCREASE STRING LENGTH
    length += char_width; // ;INCREASE STRING LENGTH
    // asm 00007A6D: 	BU	STRLP
    goto STRLP;
STRLENX:
    // asm 00007A6E: 	POP	AR2
    // asm 00007A6F: 	POP	AR1
    // asm 00007A70: 	POP	AR0
    // asm 00007A71: 	POP	R2
    // asm 00007A72: 	POP	R1
    // asm 00007A73: 	POP	RS
    // asm 00007A74: 	RETS
    return length;
}

// *----------------------------------------------------------------------------

void TEXT_OUTPUT(void) {
    tTEXT* text;
    tTEXT** prev_link;
    tTEXT* next_text;
    const char* str;
    FONTENTRY* font;
    int x;
    int y;
    int color;
    int ch;
    int width;
    int pre;
    int trail;
    int top_left;
    int y_start;

    MAME_VALIDATE_FUNCTION_ENTRY();

    // asm 00007A75: 	PUSH	AR4
    // asm 00007A76: 	PUSH	AR5
    // asm 00007A77: 	PUSH	R4
    // asm 00007A78: 	PUSH	R7
    // asm 00007A79: 	BUD	NXTGRP
    // asm 00007A7A: 	PUSHF	R7
    // asm 00007A7B: 	NOP
    // asm 00007A7C: 	LDI	@TEXT_ACTIVEI,AR4
    // 	;---->	BUD	NXTGRP
    prev_link = &TEXT_ACTIVE;
    text = TEXT_ACTIVE;

TEXTLP:
    // asm 00007A7D: 	LDI	R0,AR4
    if (text == NULL) {
        goto TXTOUT;
    }

    // asm 00007A7E: 	LDI	*+AR4(TEXT_PTR),AR2
    // asm 00007A7F: 	FIX	*+AR4(TEXT_POSX),R2
    // asm 00007A80: 	FIX	*+AR4(TEXT_POSY),R3
    str = text->ptr;
    x = (int)text->posx;
    y = (int)text->posy;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007A81, "R2", &x);
    // MAME_VALIDATE_REG_AT_ADDR(0x00007A81, "R3", &y);

    // asm 00007A81:  	LDI	*+AR4(TEXT_COLOR),R0
    // asm 00007A82: 	TSTB	TXT_CENTER,R0
    // asm 00007A83: 	BZ	NO_CENTER
    color = text->color;
    if (color & TXT_CENTER) {
        width = STRLEN(text);
        // asm 00007A84: 	CALL	STRLEN
        // asm 00007A85: 	RS	1,R0
        // MAME_VALIDATE_REG_AT_ADDR(0x00007A85, "R0", &width);
        // asm 00007A86: 	SUBI	R0,R2
        x -= width >> 1;
        // MAME_VALIDATE_REG_AT_ADDR(0x00007A87, "R2", &x);
        // asm 00007A87: 	B	TEXT_RET
        goto TEXT_RET;
    }

NO_CENTER:
    // asm 00007A88: 	TSTB	TXT_RIGHT,R0
    // asm 00007A89: 	BZ	NO_RIGHT
    if (color & TXT_RIGHT) {
        width = STRLEN(text);
        // asm 00007A8A: 	CALL	STRLEN
        // MAME_VALIDATE_REG_AT_ADDR(0x00007A8B, "R0", &width);
        // asm 00007A8B: 	SUBI	R0,R2
        x -= width;
    }
NO_RIGHT:

TEXT_RET:
    // asm 00007A8C: 	CLRI	RS
OLP:
    ch = (unsigned char)*str++;

    // asm 00007A95: 	CMPI	0,AR0
    // asm 00007A96: 	BZ	OUCX
    if (ch == 0) {
        goto OUCX;
    }

    // asm 00007A97: 	CMPI	'/',AR0
    // asm 00007A98: 	LDIEQ	'@',AR0
    if (ch == '/') {
        ch = '@';
    }

    // asm 00007A99: 	CMPI	' ',AR0
    // asm 00007A9A: 	BEQ	NXTCHAR
    if (ch == ' ') {
        goto NXTCHAR;
    }

    // asm 00007A9B: 	SUBI	'0',AR0			;the start of the font
    ch -= '0';

    // 	;NOW PLOT OUT THE CHARACTER
    // asm 00007A9C:  	LDI	*+AR4(TEXT_COLOR),R0
    // asm 00007A9D: 	TSTB	TXT_NRZ,R0
    // asm 00007A9E: 	BZ	IBO1
    color = text->color;
    if (color & TXT_NRZ) {
        // asm 00007A9F: 	AND	0FFh,R0
        // asm 00007AA0: 	OR	NZR|ZS|TM,R0
        // asm 00007AA1: 	BU	IBO2
        _ACNTL = (color & 0xFF) | NZR | ZS | TM;
    } else {
    IBO1:
        // asm 00007AA2: LDI	TM|ZS,R0
        _ACNTL = TM | ZS;
    }
IBO2:

    // asm 00007AA3: 	STI	R0,@_ACNTL
    // asm 00007AA4: 	LDI	*+AR4(TEXT_PAL),R1
    // asm 00007AA5: 	STI	R1,@_ACMAP
    _ACMAP = text->palette;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AA6, "R1", &_ACMAP);

    // asm 00007AA6: 	LDI	AR0,AR1
    // asm 00007AA7: 	MPYI	FONTENT_SIZE,AR1
    // asm 00007AA8: 	ADDI	*+AR4(TEXT_ADDR),AR1
    font = &text->text_addr[ch];

    // asm 00007AA9: 	LDI	*+AR1(FONTENT_PRE),R0
    // asm 00007AAA: 	LS	16,R0
    // asm 00007AAB: 	ASH	-16,R0			;MUST SIGN EXTEND THIS DUDE
    // asm 00007AAC: 	ADDI	R0,R2
    pre = TEXT_FONT_PRE(font);
    x += pre;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AAD, "R2", &x);
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AAD, "R0", &pre);

    // asm 00007AAD: 	LDI	*+AR1(FONTENT_XSTART),R0
    // asm 00007AAE: 	LDI	*+AR1(FONTENT_YSTART),R4
    // asm 00007AAF: 	LS	8,R4
    // asm 00007AB0: 	OR	R4,R0
    y_start = ((int)font->y_start) << 8;
    top_left = ((int)font->x_start) | y_start;

    // asm 00007AB1: 	STI	R0,@_AIVI+0
    _AIVI[0] = top_left;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AB2, "R0", &top_left);

    // asm 00007AB2: 	LDI	*+AR4(TEXT_HEIGHT),R1
    // asm 00007AB3: 	LS	8,R1
    // asm 00007AB4: 	ADDI	R0,R1
    // asm 00007AB5: 	STI	R1,@_AIVI+3
    _AIVI[3] = top_left + (((int)text->height) << 8);
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AB6, "R1", &_AIVI[3]);

    // asm 00007AB6: 	LDI	*+AR1(FONTENT_XEND),R0
    // asm 00007AB7: 	OR	R4,R0
    // asm 00007AB8: 	STI	R0,@_AIVI+1
    _AIVI[1] = ((int)font->x_end) | y_start;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AB9, "R0", &_AIVI[1]);

    // asm 00007AB9: 	LDI	*+AR4(TEXT_HEIGHT),R1
    // asm 00007ABA: 	LS	8,R1
    // asm 00007ABB: 	ADDI	R0,R1
    // asm 00007ABC: 	STI	R1,@_AIVI+2
    _AIVI[2] = _AIVI[1] + (((int)text->height) << 8);
    // MAME_VALIDATE_REG_AT_ADDR(0x00007ABD, "R1", &_AIVI[2]);

    // asm 00007ABD: 	LDI	*+AR4(TEXT_IMG),R0
    // asm 00007ABE: 	STI	R0,@_ADDRL
    _ADDRL = text->image_addr;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007ABF, "R0", &_ADDRL);

    // asm 00007ABF: 	LDI	*+AR1(FONTENT_XEND),R7
    // asm 00007AC0: 	SUBI	*+AR1(FONTENT_XSTART),R7
    width = (int)font->x_end - (int)font->x_start;

    // asm 00007AC1: 	STI	R2,@_ARPS+0
    // asm 00007AC2: 	STI	R2,@_ARPS+9	;FP3X
    _ARPS[0] = x;
    _ARPS[9] = x;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AC3, "R2", &x);

    // asm 00007AC3: 	ADDI	R7,R2
    x += width;

    // asm 00007AC4: 	STI	R2,@_ARPS+3	;FP1X
    // asm 00007AC5: 	STI	R2,@_ARPS+6	;FP2X
    _ARPS[3] = x;
    _ARPS[6] = x;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AC6, "R2", &x);

    // asm 00007AC6: 	SUBI	R7,R2
    x -= width;

    // asm 00007AC7: 	STI	R3,@_ARPS+1	;FP0Y
    // asm 00007AC8: 	STI	R3,@_ARPS+4	;FP1Y
    _ARPS[1] = y;
    _ARPS[4] = y;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007AC9, "R3", &y);

    // asm 00007AC9: 	ADDI	*+AR4(TEXT_HEIGHT),R3
    y += (int)text->height;

    // asm 00007ACA: 	STI	R3,@_ARPS+7	;FP2Y
    // asm 00007ACB: 	STI	R3,@_ARPS+10	;FP3Y
    _ARPS[7] = y;
    _ARPS[10] = y;
    // MAME_VALIDATE_REG_AT_ADDR(0x00007ACC, "R3", &y);

    // asm 00007ACC: 	SUBI	*+AR4(TEXT_HEIGHT),R3
    y -= (int)text->height;

    // asm 00007ACD: 	CALL	_stuff_fpga
    _stuff_fpga();

    // asm 00007ACE: 	LDI	*+AR1(FONTENT_TRAIL),R0
    // asm 00007ACF: 	RS	16,R0
    // asm 00007AD0: 	ADDI	R0,R7
    trail = TEXT_FONT_TRAIL(font);
    width += trail;

NXTCHAR:
    // asm 00007AD1: 	ADDI	R7,R2			;to next X position
    x += width;
    // asm 00007AD2: 	BU	OLP
    goto OLP;

OUCX:

    // asm 00007AD3: 	LDI	@TEXT_FREEZE,R0
    // asm 00007AD4: 	BNZ	ISFROZEN
    if (TEXT_FREEZE == 0) {
        // asm 00007AD5: 	LDF	*+AR4(TEXT_POSX),R0
        // asm 00007AD6: 	ADDF	*+AR4(TEXT_VELX),R0
        // asm 00007AD7: 	STF	R0,*+AR4(TEXT_POSX)
        text->posx += text->velx;
        // asm 00007AD8: 	LDF	*+AR4(TEXT_POSY),R0
        // asm 00007AD9: 	ADDF	*+AR4(TEXT_VELY),R0
        // asm 00007ADA: 	STF	R0,*+AR4(TEXT_POSY)
        text->posy += text->vely;
    }
ISFROZEN:

    // asm 00007ADB: 	LDI	*+AR4(TEXT_TIKS),R0
    // asm 00007ADC: 	LDI	*+AR4(TEXT_COLOR),R1
    // asm 00007ADD: 	TSTB	TXT_NOPULL,R1		;NOPULL = never decrement tik count
    // asm 00007ADE: 	BNZ	NODELETE
    color = text->color;
    if (color & TXT_NOPULL) {
        goto NODELETE;
    }

    // asm 00007ADF: 	SUBI	1,R0
    // asm 00007AE0: 	BGT	NODELETE
    text->tiks -= 1;
    if ((int)text->tiks > 0) {
        goto NODELETE;
    }

    // asm 00007AE1: 	LDI	*AR4,R7
    next_text = text->link;

    // asm 00007AE2: 	LDI	@TEXT_ACTIVEI,R1	;get free list pointer
    // asm 00007AE3: LDI	R1,AR1
    // asm 00007AE4: 	LDI	*AR1,R1
    // asm 00007AE5: 	CMPI	R1,AR4
    // asm 00007AE6: 	BNE	DELLP
    // asm 00007AE7: 	LDI	*AR4,R1
    // asm 00007AE8: 	STI	R1,*AR1			;LINK AROUND
    *prev_link = next_text;

    // asm 00007AE9: 	LDI	@TEXT_FREE_COUNT,R1
    // asm 00007AEA: 	INC	R1
    // asm 00007AEB: 	STI	R1,@TEXT_FREE_COUNT
    TEXT_FREE_COUNT += 1;

    // asm 00007AEC: 	LDI	@TEXT_FREEI,AR1		;get free list pointer
    // asm 00007AED: 	LDI	*AR1,R1
    // asm 00007AEE: 	STI	R1,*AR4
    // asm 00007AEF: 	STI	AR4,*AR1
    text->link = TEXT_FREE;
    TEXT_FREE = text;

    // asm 00007AF0: 	LDI	R7,R0
    text = next_text;

    // asm 00007AF1: 	BNZ	TEXTLP
    if (text != NULL) {
        goto TEXTLP;
    }

    // asm 00007AF2: 	B	TXTOUT
    goto TXTOUT;

NODELETE:
    // asm 00007AF3: 	STI	R0,*+AR4(TEXT_TIKS)

NXTGRP:
    // asm 00007AF4: 	LDI	*AR4,R0
    prev_link = &text->link;
    text = text->link;

    // asm 00007AF5: 	BNZ	TEXTLP
    if (text != NULL) {
        goto TEXTLP;
    }

TXTOUT:
    // asm 00007AF6: 	POPF	R7
    // asm 00007AF7: 	POP	R7
    // asm 00007AF8: 	POP	R4
    // asm 00007AF9: 	POP	AR5
    // asm 00007AFA: 	POP	AR4
    // asm 00007AFB: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *STRING COPY
 *
 *PARAMETERS
 *	AR0	SOURCE STRING
 *	AR1	DESTINATION STRING
 *RETURNS
 *	AR1	SOURCE STRING
 *
 */
void STRCPY(void) {
    // asm 00007AFC: 	PUSH	R0
    // asm 00007AFD: 	PUSH	AR0
    // asm 00007AFE: 	PUSH	AR1
    // asm 00007AFF: 	PUSH	AR2
    // 	;FIND WHERE AR0 ENDS
    // asm 00007B00: 	CLRI	R0
SCPLP0:
    // asm 00007B01: CMPI	-32,R0
    // asm 00007B02: 	BNE	REGPLP0
    // asm 00007B03: 	CLRI	R0
    // asm 00007B04: 	NOP	*AR0++
    // asm 00007B05: 	NOP	*AR1++
REGPLP0:
    // asm 00007B06: 	LDI	*AR0,AR2
    // asm 00007B07: 	STI	AR2,*AR1
    // asm 00007B08: 	LSH	R0,AR2
    // asm 00007B09: 	SUBI	8,R0
    // asm 00007B0A: 	AND	0FFh,AR2
    // asm 00007B0B: 	CMPI	0,AR2
    // asm 00007B0C: 	BNZ	SCPLP0
    // asm 00007B0D: 	POP	AR2
    // asm 00007B0E: 	POP	AR1
    // asm 00007B0F: 	POP	AR0
    // asm 00007B10: 	POP	R0
    // asm 00007B11: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "STRCPY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *STRING CONCATENATION
 *
 *APPEND THE CONTENTS OF AR1 TO AR0
 *
 *PARAMETERS
 *	AR0	ORIGINAL STRING (W/SPACE FOR ADDITION)
 *	AR1	APPEND STRING
 *RETURNS
 *	AR0	ORIGINAL STRING + APPEND STRING
 *
 */
void STRCAT(char* dest, const char* src) {
    while (*dest != '\0') {
        dest++;
    }

    do {
        *dest++ = *src;
    } while (*src++ != '\0');

    return;

    // asm 00007B12: 	PUSH	R0
    // asm 00007B13: 	PUSH	R1
    // asm 00007B14: 	PUSH	R2
    // asm 00007B15: 	PUSH	R3
    // asm 00007B16: 	PUSH	R4
    // asm 00007B17: 	PUSH	AR0
    // asm 00007B18: 	PUSH	AR1
    // asm 00007B19: 	PUSH	AR2
    // asm 00007B1A: 	PUSH	AR3
    // asm 00007B1B: 	PUSH	AR4
    // 	;FIND WHERE AR0 ENDS
    // asm 00007B1C: 	CLRI	R0
SCLP0:
    // asm 00007B1D: CMPI	-32,R0
    // asm 00007B1E: 	BNE	REGLP0
    // asm 00007B1F: 	CLRI	R0
    // asm 00007B20: 	NOP	*AR0++
REGLP0:
    // asm 00007B21: 	LDI	*AR0,AR2
    // asm 00007B22: 	LSH	R0,AR2
    // asm 00007B23: 	SUBI	8,R0
    // asm 00007B24: 	AND	0FFh,AR2
    // asm 00007B25: 	CMPI	0,AR2
    // asm 00007B26: 	BNZ	SCLP0
    // asm 00007B27: 	ADDI	8,R0
    // 	;NOW APPEND THE DATA
    // asm 00007B28: 	CLRI	R1
SCLP1:
    // asm 00007B29: CMPI	-32,R1
    // asm 00007B2A: 	BNE	REGLP1
    // asm 00007B2B: 	CLRI	R1
    // asm 00007B2C: 	NOP	*AR1++
REGLP1:
    // asm 00007B2D: 	LDI	*AR1,AR3
    // asm 00007B2E: 	LSH	R1,AR3
    // asm 00007B2F: 	SUBI	8,R1
    // asm 00007B30: 	AND	0FFh,AR3
    // asm 00007B31: 	LDI	AR3,AR4
    // asm 00007B32: 	NEGI	R0,R3
    // asm 00007B33: 	LSH	R3,AR3
    // asm 00007B34: 	LDI	*AR0,R4
    // asm 00007B35: 	OR	AR3,R4
    // asm 00007B36: 	STI	R4,*AR0
    // asm 00007B37: 	SUBI	8,R0
    // asm 00007B38: 	CMPI	-32,R0
    // asm 00007B39: 	BNE	REGLP2
    // asm 00007B3A: 	CLRI	R0
    // asm 00007B3B: 	NOP	*AR0++
    // asm 00007B3C: 	STI	R0,*AR0			;IN THE CASE OF NULL ALIGNED
REGLP2:
    // asm 00007B3D: 	CMPI	0,AR4
    // asm 00007B3E: 	BNZ	SCLP1
    // asm 00007B3F: 	POP	AR4
    // asm 00007B40: 	POP	AR3
    // asm 00007B41: 	POP	AR2
    // asm 00007B42: 	POP	AR1
    // asm 00007B43: 	POP	AR0
    // asm 00007B44: 	POP	R4
    // asm 00007B45: 	POP	R3
    // asm 00007B46: 	POP	R2
    // asm 00007B47: 	POP	R1
    // asm 00007B48: 	POP	R0
    // asm 00007B49: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *FONTENT		.macro	PRECEDING,XSTART,XEND,YSTART,TRAIL
 *
 *
 */
/* asm: FIXEDFONT: */
/* asm: FONTENT	0,64,71,0,1	;0 */
/* asm: FONTENT	0,72,79,0,1	;1 */
/* asm: FONTENT	0,80,87,0,1	;2 */
/* asm: FONTENT	0,88,95,0,1	;3 */
/* asm: FONTENT	0,96,103,0,1	;4 */
/* asm: FONTENT	0,104,111,0,1	;5 */
/* asm: FONTENT	0,112,119,0,1	;6 */
/* asm: FONTENT	0,120,127,0,1	;7 */
/* asm: FONTENT	0,128,135,0,1	;8 */
/* asm: FONTENT	0,136,143,0,1	;9 */
/* asm: FONTENT	0,144,151,0,1	; : */
/* asm: FONTENT	0,152,159,0,1	; ; */
/* asm: FONTENT	0,160,167,0,1	; < */
/* asm: FONTENT	0,48,55,0,1	; =	. */
/* asm: FONTENT	0,32,39,0,1	; >	, */
/* asm: FONTENT	0,40,47,0,1	; ?	- */
/* asm: FONTENT	0,56,63,0,1	; @	/ */
/* asm: FONTENT	0,200,207,0,1	;A */
/* asm: FONTENT	0,208,215,0,1 */
/* asm: FONTENT	0,216,223,0,1 */
/* asm: FONTENT	0,224,231,0,1	;D */
/* asm: FONTENT	0,232,239,0,1	;E */
/* asm: FONTENT	0,240,247,0,1	;F */
/* asm: FONTENT	0,248,255,0,1	;G */
/* asm: FONTENT	0,0,7,7,1	;H */
/* asm: FONTENT	0,8,15,7,1 */
/* asm: FONTENT	0,16,23,7,1 */
/* asm: FONTENT	0,24,31,7,1 */
/* asm: FONTENT	0,32,39,7,1	;L */
/* asm: FONTENT	0,40,47,7,1	;M */
/* asm: FONTENT	0,48,55,7,1	;N */
/* asm: FONTENT	0,56,63,7,1	;O */
/* asm: FONTENT	0,64,71,7,1	;P */
/* asm: FONTENT	0,72,79,7,1	;Q */
/* asm: FONTENT	0,80,87,7,1	;R */
/* asm: FONTENT	0,88,95,7,1 */
/* asm: FONTENT	0,96,103,7,1 */
/* asm: FONTENT	0,104,111,7,1	;U */
/* asm: FONTENT	0,112,119,7,1 */
/* asm: FONTENT	0,120,127,7,1 */
/* asm: FONTENT	0,128,135,7,1 */
/* asm: FONTENT	0,136,143,7,1 */
/* asm: FONTENT	0,144,151,7,1	;Z */
static int FIXEDFONT;

/*
 *----------------------------------------------------------------------------
 *
 *
 *PARAMETERS
 *	AR2	PTR TO TEXT ENTRY
 *	R2	CHARACTER TO HIGHLIGHT
 *	R3	PALETTE
 *
 */
void HIGHLIGHTN(void) {
    // asm 00007BF6: 	INC	R2
    // asm 00007BF7: 	CALL	PUSHALL
    // asm 00007BF8: 	LDI	R2,IR0
    // asm 00007BF9: 	LDI	R3,IR1
    // asm 00007BFA: 	LDI	AR2,AR4
    // asm 00007BFB: 	LDI	*+AR4(TEXT_PTR),AR2
    // asm 00007BFC: 	FIX	*+AR4(TEXT_POSX),R2
    // asm 00007BFD: 	FIX	*+AR4(TEXT_POSY),R3
    // asm 00007BFE:  	LDI	*+AR4(TEXT_COLOR),R0
    // asm 00007BFF: 	TSTB	TXT_CENTER,R0
    // asm 00007C00: 	BZ	NO_CENTERa
    // asm 00007C01: 	CALL	STRLEN
    // asm 00007C02: 	RS	1,R0
    // asm 00007C03: 	SUBI	R0,R2
    // asm 00007C04: 	B	text_reta
NO_CENTERa:
    // asm 00007C05: 	TSTB	TXT_RIGHT,R0
    // asm 00007C06: 	BZ	NO_RIGHTa
    // asm 00007C07: 	CALL	STRLEN
    // asm 00007C08: 	SUBI	R0,R2
    // 	;---->B	text_reta
NO_RIGHTa:
text_reta:
    // asm 00007C09: 	CLRI	BK
    // asm 00007C0A: 	CLRI	RS
OLPa:
    // asm 00007C0B: INC	BK
    // asm 00007C0C: 	CMPI	-32,RS
    // asm 00007C0D: 	BNE	REGLPa
    // asm 00007C0E: 	CLRI	RS
    // asm 00007C0F: 	NOP	*AR2++
REGLPa:
    // asm 00007C10: 	LDI	*AR2,AR0
    // asm 00007C11: 	LSH	RS,AR0
    // asm 00007C12: 	SUBI	8,RS
    // asm 00007C13: 	AND	0FFh,AR0
    // asm 00007C14: 	CMPI	0,AR0
    // asm 00007C15: 	BZ	oucXa
    // asm 00007C16: 	CMPI	' ',AR0
    // asm 00007C17: 	BEQ	NXTCHARa
    // asm 00007C18: 	SUBI	'0',AR0			;the start of the font
    // asm 00007C19: 	CMPI	IR0,BK
    // asm 00007C1A: 	BEQ	ALLREG
    // asm 00007C1B: 	BGT	oucXa
    // asm 00007C1C: 	LDI	AR0,AR1
    // asm 00007C1D: 	MPYI	FONTENT_SIZE,AR1
    // asm 00007C1E: 	ADDI	*+AR4(TEXT_ADDR),AR1
    // asm 00007C1F: 	LDI	*+AR1(FONTENT_XEND),R7
    // asm 00007C20: 	SUBI	*+AR1(FONTENT_XSTART),R7
    // asm 00007C21: 	LDI	*+AR1(FONTENT_PRE),R0
    // asm 00007C22: 	LS	16,R0
    // asm 00007C23: 	ASH	-16,R0			;MUST SIGN EXTEND THIS DUDE
    // asm 00007C24: 	ADDI	R0,R7
    // asm 00007C25: 	LDI	*+AR1(FONTENT_TRAIL),R0
    // asm 00007C26: 	ASH	-16,R0
    // asm 00007C27: 	ADDI	R0,R7
    // asm 00007C28: 	BU	NXTCHARa
ALLREG:
    // 	;NOW PLOT OUT THE CHARACTER
    // asm 00007C29:  	LDI	*+AR4(TEXT_COLOR),R0
    // asm 00007C2A: 	AND	0FFh,R0
    // asm 00007C2B: 	BZ	IBO1a
    // asm 00007C2C: 	OR	NZR,R0
IBO1a:
    // asm 00007C2D: OR	TM|ZS,R0
    // asm 00007C2E: 	LDI	TM|ZS|NZR|6,R0
    // asm 00007C2F: 	STI	R0,@_ACNTL
    // asm 00007C30: 	CLRI	IR1			;palette 0 ignore given
    // asm 00007C31: 	STI	IR1,@_ACMAP		;SPECIAL PALETTE
    // asm 00007C32: 	LDI	AR0,AR1
    // asm 00007C33: 	MPYI	FONTENT_SIZE,AR1
    // asm 00007C34: 	ADDI	*+AR4(TEXT_ADDR),AR1
    // asm 00007C35: 	LDI	*+AR1(FONTENT_PRE),R0
    // asm 00007C36: 	LS	16,R0
    // asm 00007C37: 	ASH	-16,R0			;MUST SIGN EXTEND THIS DUDE
    // asm 00007C38: 	ADDI	R0,R2
    // asm 00007C39: 	LDI	*+AR1(FONTENT_XSTART),R0
    // asm 00007C3A: 	LDI	*+AR1(FONTENT_YSTART),R4
    // asm 00007C3B: 	LS	8,R4
    // asm 00007C3C: 	OR	R4,R0
    // asm 00007C3D: 	STI	R0,@_AIVI+0
    // asm 00007C3E: 	LDI	*+AR4(TEXT_HEIGHT),R1
    // asm 00007C3F: 	LS	8,R1
    // asm 00007C40: 	ADDI	R0,R1
    // asm 00007C41: 	STI	R1,@_AIVI+3
    // asm 00007C42: 	LDI	*+AR1(FONTENT_XEND),R0
    // asm 00007C43: 	OR	R4,R0
    // asm 00007C44: 	STI	R0,@_AIVI+1
    // asm 00007C45: 	LDI	*+AR4(TEXT_HEIGHT),R1
    // asm 00007C46: 	LS	8,R1
    // asm 00007C47: 	ADDI	R0,R1
    // asm 00007C48: 	STI	R1,@_AIVI+2
    // asm 00007C49: 	LDI	*+AR4(TEXT_IMG),R0
    // asm 00007C4A: 	STI	R0,@_ADDRL
    // asm 00007C4B: 	LDI	*+AR1(FONTENT_XEND),R7
    // asm 00007C4C: 	SUBI	*+AR1(FONTENT_XSTART),R7
    // asm 00007C4D: 	STI	R2,@_ARPS+0
    // asm 00007C4E: 	STI	R2,@_ARPS+9	;FP3X
    // asm 00007C4F: 	ADDI	R7,R2
    // asm 00007C50: 	STI	R2,@_ARPS+3	;FP1X
    // asm 00007C51: 	STI	R2,@_ARPS+6	;FP2X
    // asm 00007C52: 	SUBI	R7,R2
    // asm 00007C53: 	STI	R3,@_ARPS+1	;FP0Y
    // asm 00007C54: 	STI	R3,@_ARPS+4	;FP1Y
    // asm 00007C55: 	ADDI	*+AR4(TEXT_HEIGHT),R3
    // asm 00007C56: 	DEC	R3
    // asm 00007C57: 	STI	R3,@_ARPS+7	;FP2Y
    // asm 00007C58: 	STI	R3,@_ARPS+10	;FP3Y
    // asm 00007C59: 	SUBI	*+AR4(TEXT_HEIGHT),R3
    // asm 00007C5A: 	INC	R3
    // asm 00007C5B: 	CALL	_stuff_fpga
    // asm 00007C5C: 	LDI	*+AR1(FONTENT_TRAIL),R0
    // asm 00007C5D: 	ASH	-16,R0
    // asm 00007C5E: 	ADDI	R0,R7
NXTCHARa:
    // asm 00007C5F: 	ADDI	R7,R2			;to next X position
    // asm 00007C60: 	BU	OLPa
oucXa:
    // asm 00007C61: 	CALL	POPALL
    // asm 00007C62: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIGHLIGHTN", 0, 0);
    UNIMPL();
}

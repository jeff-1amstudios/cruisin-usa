#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "vunit.h"
#include "macs.h"
#include "text.h"
#include "font1a.h"

/*
 * Source module: asm/FONT1A.ASM
 */

static void ENABLEGIE_font(void);
void _ftoa(void);
void _itoaLZ(void);
void _itoa(void);
void HEX2ASC(void);
void _fill(void);
void _outtextxyc(void);
void _pixel(void);

#define _font1I _font1

void DIV_I30(void);
void MOD_I30(void);
extern int ACTIVE_SCREEN;
extern int _font1[];
extern int COMMINTM;

// *----------------------------------------------------------------------------
static void ENABLEGIE_font(void)
{
    // asm 0000A75C: RETI
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static const char POINT[] = ".";
/* asm: ftoa_tmp	.bss	ftoa_tmp,2 */
int ftoa_tmp[2];

void _ftoa(void)
{
    // asm 0000A75E: 	PUSH	R0
    // asm 0000A75F: 	PUSH	R2
    // asm 0000A760: 	PUSH	AR2
    // asm 0000A761: 	PUSH	AR2
    // asm 0000A762: 	PUSHFL	R2
    // asm 0000A764: 	FIX	R2
    // asm 0000A765: 	CALL	_itoa
    // asm 0000A766: 	LDL	POINT,AR1
    // asm 0000A767: 	LDI	AR2,AR0
    // asm 0000A768: 	CALL	STRCAT
    // asm 0000A769: 	POPFL	R2
    // asm 0000A76B: 	LDL	ftoa_tmp,AR2
    // asm 0000A76C: 	FIX	R2,R0
    // asm 0000A76D: 	FLOAT	R0
    // asm 0000A76E: 	SUBF	R0,R2
    // asm 0000A76F: 	FIX	R2
    // asm 0000A770: 	MPYI	100,R2
    // asm 0000A771: 	CALL	_itoa
    // asm 0000A772: 	LDI	AR2,AR1
    // asm 0000A773: 	POP	AR0
    // asm 0000A774: 	CALL	STRCAT
    // asm 0000A775: 	POP	AR2
    // asm 0000A776: 	POP	R2
    // asm 0000A777: 	POP	R0
    // asm 0000A778: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_ftoa", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*INTEGER TO ASCIZ (PACKED)
*
*PARAMETERS
*	AR2	STRING SPACE
*	R2	NUMBER
*RETS
*	AR2	POINTING TO SPACE
*	R0	HAVING THE SHIFT COUNT
*
*
*INTEGER TO ASCIZ WITH LEADING ZERO
*/
void _itoaLZ(void)
{
    // asm 0000A779: 	PUSH	R0			;this entry includes a leading zero
    // asm 0000A77A: 	PUSH	R1			;if the value is 9 or less
    // asm 0000A77B: 	PUSH	R2			;
    // asm 0000A77C: 	PUSH	R3			;
    // asm 0000A77D: 	PUSH	AR0
    // asm 0000A77E: 	PUSH	AR1
    // asm 0000A77F: 	PUSH	AR2
    // asm 0000A780: 	PUSH	AR7
    // asm 0000A781: 	PUSH	R6
    // asm 0000A782: 	PUSH	R7
    // asm 0000A783: 	CLRI	R7			;flag if negative
    // asm 0000A784: 	CMPI	0,R2
    // asm 0000A785: 	BZD	ISZERO2
    // asm 0000A786: 	LDILT	1,R7
    // asm 0000A787: 	ABSI	R2
    // asm 0000A788: 	CLRI	R3
    // asm 0000A789: 	CMPI	9,R2
    // asm 0000A78A: 	BGT	itoa1
    // asm 0000A78B: 	LDI	1,AR7
    // asm 0000A78C: 	BU	itoa1
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_itoaLZ", 0, 0);
    UNIMPL();
}

void _itoa(void)
{
    // asm 0000A78D: 	PUSH	R0
    // asm 0000A78E: 	PUSH	R1
    // asm 0000A78F: 	PUSH	R2
    // asm 0000A790: 	PUSH	R3
    // asm 0000A791: 	PUSH	AR0
    // asm 0000A792: 	PUSH	AR1
    // asm 0000A793: 	PUSH	AR2
    // asm 0000A794: 	PUSH	AR7
    // asm 0000A795: 	PUSH	R6
    // asm 0000A796: 	PUSH	R7
    // asm 0000A797: 	CLRI	R7			;flag if negative
    // asm 0000A798: 	CMPI	0,R2
    // asm 0000A799: 	BZD	ISZERO
    // asm 0000A79A: 	LDILT	1,R7
    // asm 0000A79B: 	CLRI	AR7
    // asm 0000A79C: 	ABSI	R2
    // asm 0000A79D: 	CLRI	R3
itoa1:
    // asm 0000A79E: LDI	10,R1			;this loop generates the ASCII
    // asm 0000A79F: 	LDI	R2,R0			;pieces and pushes them on the stack
    // asm 0000A7A0: 	CALL	MOD_I30
    // asm 0000A7A1: 	OR	30h,R0
    // asm 0000A7A2: 	PUSH	R0
    // asm 0000A7A3: 	INC	R3
    // asm 0000A7A4: 	LDI	10,R1
    // asm 0000A7A5: 	LDI	R2,R0
    // asm 0000A7A6: 	CALL	DIV_I30
    // asm 0000A7A7: 	LDI	R0,R2
    // asm 0000A7A8: 	BGT	itoa1
    // asm 0000A7A9: 	CMPI	1,AR7
    // asm 0000A7AA: 	BNE	NOLEADINGZERO
    // asm 0000A7AB: 	LDI	30h,R0
    // asm 0000A7AC: 	PUSH	R0
    // asm 0000A7AD: 	INC	R3
NOLEADINGZERO:
    // asm 0000A7AE: 	CLRI	R6			;this loop pops the ASCII pieces off
    // asm 0000A7AF: 	CLRI	R1			;the stack and packs them into the
    // asm 0000A7B0: 	CMPI	0,R7			;destination space
    // asm 0000A7B1: 	BEQ	NOTNEG			;check sign flag
    // asm 0000A7B2: 	LDI	MINUS_CHAR,R1
    // asm 0000A7B3: 	ADDI	8,R6
NOTNEG:
LOOP2:
    // asm 0000A7B4: POP	R0
    // asm 0000A7B5: 	LSH	R6,R0
    // asm 0000A7B6: 	OR	R0,R1
    // asm 0000A7B7: 	ADDI	8,R6
    // asm 0000A7B8: 	CMPI	32,R6
    // asm 0000A7B9: 	BLT	DALOP
    // asm 0000A7BA: 	STI	R1,*AR2++
    // asm 0000A7BB: 	CLRI	R1
    // asm 0000A7BC: 	CLRI	R6
DALOP:
    // asm 0000A7BD: DEC	R3
    // asm 0000A7BE: 	BGT	LOOP2
    // asm 0000A7BF: 	LDI	0,R0			;NULL terminator
    // asm 0000A7C0: 	LSH	R6,R0
    // asm 0000A7C1: 	OR	R0,R1
    // asm 0000A7C2: 	STI	R1,*AR2
    // asm 0000A7C3: 	LDI	R6,R0
itoaX:
    // asm 0000A7C4: 	POP	R7
    // asm 0000A7C5: 	POP	R6
    // asm 0000A7C6: 	POP	AR7
    // asm 0000A7C7: 	POP	AR2
    // asm 0000A7C8: 	POP	AR1
    // asm 0000A7C9: 	POP	AR0
    // asm 0000A7CA: 	POP	R3
    // asm 0000A7CB: 	POP	R2
    // asm 0000A7CC: 	POP	R1
    // asm 0000A7CD: 	POP	R0
    // asm 0000A7CE: 	RETS
ISZERO2:
    // asm 0000A7CF: 	BUD	itoaX
    // asm 0000A7D0: 	LDI	03030h,R0			;case when number is zero
    // asm 0000A7D1: 	STI	R0,*AR2
    // asm 0000A7D2: 	LDI	16,R0
    // 	;---->	BUD	itoaX
ISZERO:
    // asm 0000A7D3: 	BUD	itoaX
    // asm 0000A7D4: 	LDI	030h,R0			;case when number is zero
    // asm 0000A7D5: 	STI	R0,*AR2
    // asm 0000A7D6: 	LDI	8,R0
    // 	;---->	BUD	itoaX
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_itoa", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*HEX2ASC	CONVERT HEX TO ASCII
*
*PARAMETERS
*	R2	NUMBER
*	AR2	BUFFER
*
*
*/
void HEX2ASC(void)
{
    // asm 0000A7D7: 	PUSH	R0
    // asm 0000A7D8: 	PUSH	R1
    // asm 0000A7D9: 	PUSH	R2
    // asm 0000A7DA: 	PUSH	R3
    // asm 0000A7DB: 	PUSH	AR0
    // asm 0000A7DC: 	PUSH	AR1
    // asm 0000A7DD: 	PUSH	AR2
    // asm 0000A7DE: 	PUSH	AR7
    // asm 0000A7DF: 	PUSH	R6
    // asm 0000A7E0: 	PUSH	R7
    // asm 0000A7E1: 	CMPI	0,R2
    // asm 0000A7E2: 	BZD	ISZEROH
    // ;	LS	16,R2
    // ;	RS	16,R2
    // asm 0000A7E3: 	CLRI	R3
htoa1:
    // asm 0000A7E4: 	LDI	R2,R0
    // asm 0000A7E5: 	AND	0Fh,R0
    // asm 0000A7E6: 	CMPI	10,R0
    // asm 0000A7E7: 	BLT	DDJ
    // asm 0000A7E8: 	ADDI	'A'-10,R0
    // asm 0000A7E9: 	BU	JJK
DDJ:
    // asm 0000A7EA: ADDI	30h,R0
JJK:
    // asm 0000A7EB: PUSH	R0
    // asm 0000A7EC: 	INC	R3
    // asm 0000A7ED: 	RS	4,R2
    // asm 0000A7EE: 	BGT	htoa1
    // asm 0000A7EF: 	CLRI	R6			;this loop pops the ASCII pieces off
    // asm 0000A7F0: 	CLRI	R1			;the stack and packs them into the
LOOP2H:
    // asm 0000A7F1: POP	R0
    // asm 0000A7F2: 	LSH	R6,R0
    // asm 0000A7F3: 	OR	R0,R1
    // asm 0000A7F4: 	ADDI	8,R6
    // asm 0000A7F5: 	CMPI	32,R6
    // asm 0000A7F6: 	BLT	HDALOP
    // asm 0000A7F7: 	STI	R1,*AR2++
    // asm 0000A7F8: 	CLRI	R1
    // asm 0000A7F9: 	CLRI	R6
HDALOP:
    // asm 0000A7FA: DEC	R3
    // asm 0000A7FB: 	BGT	LOOP2H
    // asm 0000A7FC: 	LDI	0,R0			;NULL terminator
    // asm 0000A7FD: 	LSH	R6,R0
    // asm 0000A7FE: 	OR	R0,R1
    // asm 0000A7FF: 	STI	R1,*AR2
    // asm 0000A800: 	LDI	R6,R0
htoaX:
    // asm 0000A801: 	POP	R7
    // asm 0000A802: 	POP	R6
    // asm 0000A803: 	POP	AR7
    // asm 0000A804: 	POP	AR2
    // asm 0000A805: 	POP	AR1
    // asm 0000A806: 	POP	AR0
    // asm 0000A807: 	POP	R3
    // asm 0000A808: 	POP	R2
    // asm 0000A809: 	POP	R1
    // asm 0000A80A: 	POP	R0
    // asm 0000A80B: 	RETS
ISZEROH:
    // asm 0000A80C: 	BUD	htoaX
    // asm 0000A80D: 	LDI	030h,R0			;case when number is zero
    // asm 0000A80E: 	STI	R0,*AR2
    // asm 0000A80F: 	LDI	8,R0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEX2ASC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*fill(int x1,int y1,int x2,int y2,int color)
*
*PARAMETERS
*	AR2	X1
*	R2	Y1
*	R3	X2
*	RC	Y2
*	RS	COLOR
*
*
*/
void _fill(void)
{
    uint32_t x0;
    int x;
    int y;

    // asm 0000A810: 	PUSH	AR2
    x0 = AR2;
FILLLP1:
    // asm 0000A811: 	PUSH	R3
    // asm 0000A812: 	LDI	RS,R3
    // asm 0000A813: 	CALL	_pixel
    // asm 0000A814: 	POP	R3
    // asm 0000A815: 	INC	AR2
    // asm 0000A816: 	CMPI	AR2,R3
    // asm 0000A817: 	BGE	FILLLP1
    // asm 0000A818: 	POP	AR2
    // asm 0000A819: 	PUSH	AR2
    // asm 0000A81A: 	INC	R2
    // asm 0000A81B: 	CMPI	R2,RC
    // asm 0000A81C: 	BGE	FILLLP1
    // asm 0000A81D: 	POP	AR2
    // asm 0000A81E: 	RETS
    for (y = R2.s; y <= (int)RC; ++y) {
        for (x = (int)x0; x <= R3.s; ++x) {
            AR2 = (uintptr_t)(uint32_t)x;
            R2.s = y;
            R3.s = (int)RS;
            _pixel();
        }
    }

    AR2 = x0;
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*OUTPUT TEXT STRING AT X,Y PACKED CHARACTERS
*void	outtextxyc(Pchar *,int x,int y,int color_pal);
*
*PARAMETERS
*	AR2	STRING
*	R2	X LOC
*	R3	Y LOC
*	RC	COLOR
*
*	print a text string with the .string (packed)
*
*CLOBBERS	RS,RE,RC
*
*/
void _outtextxyc(void)
{
    const unsigned char *string_ptr;
    unsigned int glyph_index;
    unsigned int row_bits;
    int ch;
    int row;
    int col;

    // asm 0000A81F: 	PUSH	R4
    // asm 0000A820: 	PUSH	R5
    // asm 0000A821: 	LDI	3,RS
    // asm 0000A822: 	CLRI	RS
    string_ptr = (const unsigned char *)(uintptr_t)AR2;
    RS = 0;
OLP:
    // asm 0000A823: CMPI	-32,RS
    // asm 0000A824: 	BNE	REGLP
    if ((int32_t)RS == -32) {
        // asm 0000A825: 	CLRI	RS
        RS = 0;
        // asm 0000A826: 	NOP	*AR2++
        string_ptr += 4;
    }
REGLP:
    // asm 0000A827: 	LDI	*AR2,AR0
    ch = string_ptr[((unsigned int)(-(int32_t)RS)) >> 3];
    // asm 0000A828: 	LSH	RS,AR0
    // asm 0000A829: 	SUBI	8,RS
    RS = (uint32_t)((int32_t)RS - 8);
    // asm 0000A82A: 	AND	0FFh,AR0
    // asm 0000A82B: 	CMPI	0,AR0
    // asm 0000A82C: 	BZ	oucX
    if (ch == 0) {
        goto oucX;
    }
    // asm 0000A82D: 	CMPI	' ',AR0
    // asm 0000A82E: 	BEQ	NXTCHAR
    if (ch == ' ') {
        goto NXTCHAR;
    }
    // asm 0000A82F: 	SUBI	'(',AR0			;the start of the font
    glyph_index = (unsigned int)(ch - '(');
    // 	;NOW PLOT OT THE CHARACTER
    // asm 0000A830: 	MPYI	7,AR0			;index to character
    glyph_index *= 7u;
    // asm 0000A831: 	ADDI	@_font1I,AR0		;pointing to font data
    // asm 0000A832: 	LDI	7,R5			;Y count
    row = 0;
OUTRLP:
    // asm 0000A833: 	LDI	*AR0++,R1
    row_bits = (unsigned int)_font1[glyph_index + (unsigned int)row];
    // asm 0000A834: 	LS	24,R1
    // asm 0000A835: 	LDI	8,R4
    col = 0;
LOOP1:
    // asm 0000A836: ASH	1,R1
    // asm 0000A837: 	BNC	NPLOT
    if (col < 7 && (row_bits & (1u << (6 - col))) != 0) {
        // asm 0000A838: 	PUSH	AR2
        crusn_machine_push_u32((u32)AR2);
        // asm 0000A839: 	PUSH	R2
        crusn_machine_push_reg32(R2);
        // asm 0000A83A: 	PUSH	R3
        crusn_machine_push_reg32(R3);
        // asm 0000A83B: 	LDI	R2,AR2
        AR2 = (uintptr_t)(uint32_t)R2.s;
        // asm 0000A83C: 	LDI	R3,R2
        R2.s = R3.s;
        // asm 0000A83D: 	LDI	RC,R3
        R3.s = (int)RC;
        // asm 0000A83E: 	CALL	_pixel
        _pixel();
        // asm 0000A83F: 	POP	R3
        R3 = crusn_machine_pop_reg32();
        // asm 0000A840: 	POP	R2
        R2 = crusn_machine_pop_reg32();
        // asm 0000A841: 	POP	AR2
        AR2 = crusn_machine_pop_u32();
    }
NPLOT:
    // asm 0000A842: INC	R2			;advance to next pixel
    ++R2.s;
    // asm 0000A843: 	DEC	R4
    // asm 0000A844: 	BGT	LOOP1
    ++col;
    if (col < 8) {
        goto LOOP1;
    }
    // asm 0000A845: 	SUBI	8,R2			;reset X position
    R2.s -= 8;
    // asm 0000A846: 	INC	R3			;increment Y position
    ++R3.s;
    // asm 0000A847: 	DEC	R5
    // asm 0000A848: 	BGT	OUTRLP
    ++row;
    if (row < 7) {
        goto OUTRLP;
    }
    // asm 0000A849: 	SUBI	7,R3			;reset Y position
    R3.s -= 7;
NXTCHAR:
    // asm 0000A84A: ADDI	8,R2			;to next X position
    R2.s += 8;
    // asm 0000A84B: 	BU	OLP
    goto OLP;
oucX:
    // asm 0000A84C: 	POP	R5
    // asm 0000A84D: 	POP	R4
    // asm 0000A84E: 	RETS
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*PIXEL ROUTINE
*
*NOTE :	other routines rely on the spec being exactly as they are specified
*here. DO NOT TRASH OTHER REGISTERS!!!
*
*PARAMETERS
*	AR2	X COORD
*	R2	Y COORD
*	R3	COLOR
*
*R3	: [ xxxxPPCC ]
*	PP   PALETTE CODE
*	CC   COLOR CODE
*
*CLOBBERS	R0,AR1
*
*/
void _pixel(void)
{
    uint32_t x;
    int y;

    // asm 0000A84F: 	PUSH	AR1
    // asm 0000A850: 	PUSH	AR2
    // asm 0000A851: 	PUSH	R2
    // asm 0000A852: 	PUSH	IE
    // asm 0000A853: 	LDP	@COMMINTM
    // asm 0000A854: 	LDI	@COMMINTM,IE
    // asm 0000A855: 	SETDP
    // asm 0000A856: 	LDP	@CPU_WS
    // asm 0000A857: 	LDI	HARD_WS,AR1
    // asm 0000A858: 	AND	0DFFFh,ST		;turn off GIE.
    // asm 0000A859: 	POP	IE
    // asm 0000A85A: 	STI	AR1,@CPU_WS		;SET WAITSTATE MODE
    // asm 0000A85B: 	SETDP
    // asm 0000A85C: 	LDI	0,AR1			;DUMMY READ ADDR
    // asm 0000A85D: 	LSH	9,R2
    // asm 0000A85E: 	ADDI	@ACTIVE_SCREEN,AR2	;GET CURRENT SCREEN PAGE
    // asm 0000A85F: 	ADDI	R2,AR2
    // asm 0000A860: 	LDP	@CPU_WS
    // asm 0000A861: 	STI	R3,*AR2			;store to screen
    // asm 0000A862: 	LDI	*AR1,R2			;DUMMY READ
    // asm 0000A863: 	LDI	SOFT_WS,R2
    // asm 0000A864: 	STI	R2,@CPU_WS
    // asm 0000A865: 	SETDP
    // asm 0000A866: 	CALL	ENABLEGIE_font
    // asm 0000A867: 	POP	R2
    // asm 0000A868: 	POP	AR2
    // asm 0000A869: 	POP	AR1
    // asm 0000A86A: 	RETS
    x = AR2;
    y = R2.s;
    if (x >= CRUSN_SCREEN_WIDTH || y < 0 || y >= CRUSN_SCREEN_HEIGHT) {
        return;
    }

    crusn_mem_wr32((word_addr_t)ACTIVE_SCREEN + ((word_addr_t)y << 9) + x, R3.u);
}

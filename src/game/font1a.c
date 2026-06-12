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

/* *----------------------------------------------------------------------------
 */
const char *POINT = ".";
/* asm: ftoa_tmp	.bss	ftoa_tmp,2 */
int ftoa_tmp[2];

void _ftoa(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR2
    // asm: 	PUSHFL	R2
    // asm: 	FIX	R2
    // asm: 	CALL	_itoa
    // asm: 	LDL	POINT,AR1
    // asm: 	LDI	AR2,AR0
    // asm: 	CALL	STRCAT
    // asm: 	POPFL	R2
    // asm: 	LDL	ftoa_tmp,AR2
    // asm: 	FIX	R2,R0
    // asm: 	FLOAT	R0
    // asm: 	SUBF	R0,R2
    // asm: 	FIX	R2
    // asm: 	MPYI	100,R2
    // asm: 	CALL	_itoa
    // asm: 	LDI	AR2,AR1
    // asm: 	POP	AR0
    // asm: 	CALL	STRCAT
    // asm: 	POP	AR2
    // asm: 	POP	R2
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_ftoa", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
    // asm: 	PUSH	R0			;this entry includes a leading zero
    // asm: 	PUSH	R1			;if the value is 9 or less
    // asm: 	PUSH	R2			;
    // asm: 	PUSH	R3			;
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR7
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	CLRI	R7			;flag if negative
    // asm: 	CMPI	0,R2
    // asm: 	BZD	ISZERO2
    // asm: 	LDILT	1,R7
    // asm: 	ABSI	R2
    // asm: 	CLRI	R3
    // asm: 	CMPI	9,R2
    // asm: 	BGT	itoa1
    // asm: 	LDI	1,AR7
    // asm: 	BU	itoa1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_itoaLZ", 0, 0);
    UNIMPL();
}

void _itoa(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR7
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	CLRI	R7			;flag if negative
    // asm: 	CMPI	0,R2
    // asm: 	BZD	ISZERO
    // asm: 	LDILT	1,R7
    // asm: 	CLRI	AR7
    // asm: 	ABSI	R2
    // asm: 	CLRI	R3
itoa1:
    // asm: LDI	10,R1			;this loop generates the ASCII
    // asm: 	LDI	R2,R0			;pieces and pushes them on the stack
    // asm: 	CALL	MOD_I30
    // asm: 	OR	30h,R0
    // asm: 	PUSH	R0
    // asm: 	INC	R3
    // asm: 	LDI	10,R1
    // asm: 	LDI	R2,R0
    // asm: 	CALL	DIV_I30
    // asm: 	LDI	R0,R2
    // asm: 	BGT	itoa1
    // asm: 	CMPI	1,AR7
    // asm: 	BNE	NOLEADINGZERO
    // asm: 	LDI	30h,R0
    // asm: 	PUSH	R0
    // asm: 	INC	R3
NOLEADINGZERO:
    // asm: 	CLRI	R6			;this loop pops the ASCII pieces off
    // asm: 	CLRI	R1			;the stack and packs them into the
    // asm: 	CMPI	0,R7			;destination space
    // asm: 	BEQ	NOTNEG			;check sign flag
    // asm: 	LDI	MINUS_CHAR,R1
    // asm: 	ADDI	8,R6
NOTNEG:
LOOP2:
    // asm: POP	R0
    // asm: 	LSH	R6,R0
    // asm: 	OR	R0,R1
    // asm: 	ADDI	8,R6
    // asm: 	CMPI	32,R6
    // asm: 	BLT	DALOP
    // asm: 	STI	R1,*AR2++
    // asm: 	CLRI	R1
    // asm: 	CLRI	R6
DALOP:
    // asm: DEC	R3
    // asm: 	BGT	LOOP2
    // asm: 	LDI	0,R0			;NULL terminator
    // asm: 	LSH	R6,R0
    // asm: 	OR	R0,R1
    // asm: 	STI	R1,*AR2
    // asm: 	LDI	R6,R0
itoaX:
    // asm: 	POP	R7
    // asm: 	POP	R6
    // asm: 	POP	AR7
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
ISZERO2:
    // asm: 	BUD	itoaX
    // asm: 	LDI	03030h,R0			;case when number is zero
    // asm: 	STI	R0,*AR2
    // asm: 	LDI	16,R0
    // 	;---->	BUD	itoaX
ISZERO:
    // asm: 	BUD	itoaX
    // asm: 	LDI	030h,R0			;case when number is zero
    // asm: 	STI	R0,*AR2
    // asm: 	LDI	8,R0
    // 	;---->	BUD	itoaX
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_itoa", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR7
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	CMPI	0,R2
    // asm: 	BZD	ISZEROH
    // ;	LS	16,R2
    // ;	RS	16,R2
    // asm: 	CLRI	R3
htoa1:
    // asm: 	LDI	R2,R0
    // asm: 	AND	0Fh,R0
    // asm: 	CMPI	10,R0
    // asm: 	BLT	DDJ
    // asm: 	ADDI	'A'-10,R0
    // asm: 	BU	JJK
DDJ:
    // asm: ADDI	30h,R0
JJK:
    // asm: PUSH	R0
    // asm: 	INC	R3
    // asm: 	RS	4,R2
    // asm: 	BGT	htoa1
    // asm: 	CLRI	R6			;this loop pops the ASCII pieces off
    // asm: 	CLRI	R1			;the stack and packs them into the
LOOP2H:
    // asm: POP	R0
    // asm: 	LSH	R6,R0
    // asm: 	OR	R0,R1
    // asm: 	ADDI	8,R6
    // asm: 	CMPI	32,R6
    // asm: 	BLT	HDALOP
    // asm: 	STI	R1,*AR2++
    // asm: 	CLRI	R1
    // asm: 	CLRI	R6
HDALOP:
    // asm: DEC	R3
    // asm: 	BGT	LOOP2H
    // asm: 	LDI	0,R0			;NULL terminator
    // asm: 	LSH	R6,R0
    // asm: 	OR	R0,R1
    // asm: 	STI	R1,*AR2
    // asm: 	LDI	R6,R0
htoaX:
    // asm: 	POP	R7
    // asm: 	POP	R6
    // asm: 	POP	AR7
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
ISZEROH:
    // asm: 	BUD	htoaX
    // asm: 	LDI	030h,R0			;case when number is zero
    // asm: 	STI	R0,*AR2
    // asm: 	LDI	8,R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEX2ASC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
    // asm: 	PUSH	AR2
FILLLP1:
    // asm: 	PUSH	R3
    // asm: 	LDI	RS,R3
    // asm: 	CALL	_pixel
    // asm: 	POP	R3
    // asm: 	INC	AR2
    // asm: 	CMPI	AR2,R3
    // asm: 	BGE	FILLLP1
    // asm: 	POP	AR2
    // asm: 	PUSH	AR2
    // asm: 	INC	R2
    // asm: 	CMPI	R2,RC
    // asm: 	BGE	FILLLP1
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_fill", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	LDI	3,RS
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
    // asm: 	BZ	oucX
    // asm: 	CMPI	' ',AR0
    // asm: 	BEQ	NXTCHAR
    // asm: 	SUBI	'(',AR0			;the start of the font
    // 	;NOW PLOT OT THE CHARACTER
    // asm: 	MPYI	7,AR0			;index to character
    // asm: 	ADDI	@_font1I,AR0		;pointing to font data
    // asm: 	LDI	7,R5			;Y count
OUTRLP:
    // asm: 	LDI	*AR0++,R1
    // asm: 	LS	24,R1
    // asm: 	LDI	8,R4
LOOP1:
    // asm: ASH	1,R1
    // asm: 	BNC	NPLOT
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	LDI	R2,AR2
    // asm: 	LDI	R3,R2
    // asm: 	LDI	RC,R3
    // asm: 	CALL	_pixel
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	AR2
NPLOT:
    // asm: INC	R2			;advance to next pixel
    // asm: 	DEC	R4
    // asm: 	BGT	LOOP1
    // asm: 	SUBI	8,R2			;reset X position
    // asm: 	INC	R3			;increment Y position
    // asm: 	DEC	R5
    // asm: 	BGT	OUTRLP
    // asm: 	SUBI	7,R3			;reset Y position
NXTCHAR:
    // asm: ADDI	8,R2			;to next X position
    // asm: 	BU	OLP
oucX:
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_outtextxyc", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	PUSH	IE
    // asm: 	LDP	@COMMINTM
    // asm: 	LDI	@COMMINTM,IE
    // asm: 	SETDP
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	HARD_WS,AR1
    // asm: 	AND	0DFFFh,ST		;turn off GIE.
    // asm: 	POP	IE
    // asm: 	STI	AR1,@CPU_WS		;SET WAITSTATE MODE
    // asm: 	SETDP
    // asm: 	LDI	0,AR1			;DUMMY READ ADDR
    // asm: 	LSH	9,R2
    // asm: 	ADDI	@ACTIVE_SCREEN,AR2	;GET CURRENT SCREEN PAGE
    // asm: 	ADDI	R2,AR2
    // asm: 	LDP	@CPU_WS
    // asm: 	STI	R3,*AR2			;store to screen
    // asm: 	LDI	*AR1,R2			;DUMMY READ
    // asm: 	LDI	SOFT_WS,R2
    // asm: 	STI	R2,@CPU_WS
    // asm: 	SETDP
    // asm: 	CALL	ENABLEGIE
    // asm: 	POP	R2
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_pixel", 0, 0);
    UNIMPL();
}

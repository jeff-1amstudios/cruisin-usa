#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/comp.h"

/*
 * Source module: asm/COMP.ASM
 */

// *----------------------------------------------------------------------------
// *DECOMPRESSION SYSTEM
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: PADDING	.bss	PADDING,50 */
int PADDING[50];
/* asm: DECOMP_ACTIVE	.bss	DECOMP_ACTIVE,1 */
int DECOMP_ACTIVE;
/* asm: HARD_SECTION_LOAD	.bss	HARD_SECTION_LOAD,1 */
int HARD_SECTION_LOAD;
/* asm: FLUSH_COUNT	.bss	FLUSH_COUNT,1 */
int FLUSH_COUNT;
/* asm: PACIFY_COUNT	.bss	PACIFY_COUNT,1 */
int PACIFY_COUNT;
#define PACIFY_MOMENT 2048
// ;PACIFY_MOMENT	.set	512
// *----------------------------------------------------------------------------
// *REGISTER ALLOCATION
// *
// *AR0	SADDR	(source addr or bitstream)
// *AR1	DADDR	(dest addr)
// *AR2	scratch
// *AR3	DICTI
// *AR4	scratch
// *AR5	scratch
// *AR6	CURRENT_CODE_BITS
// *AR7	BIT_ADDR
// *
// *R0	scratch
// *R1	scratch
// *R2	scratch
// *R3	PUTC_SH
// *R4	new_code
// *R5	old_code
// *R6	character
// *R7	BUFCNT
// *
// *
// *IR0	CHARACTER (offset)
// *IR1	PUTC_BUF
// *BK	NEXT_CODE
// *
// *RC	count
// *RS
// *RE
// *
// *
// *
// *
#define BITS 12
#define MAX_CODE ((( 1 << BITS) -1 ))
#define TABLE_SIZE 4421
#define END_OF_STREAM 256
#define BUMP_CODE 257
#define FLUSH_CODE 258
#define FIRST_CODE 259
#define UNUSED (-1)
// *
// *
// *STRUCT	dictionary
#define PARENT_CODE 0
#define CODE_VALUE 0
#define CHARACTER TABLE_SIZE
#define DICT_SIZ 2
// *ENDSTRUCT
/* asm: DICT	hibss	DICT,TABLE_SIZE*DICT_SIZ */
int DICT[TABLE_SIZE*DICT_SIZ];
/* asm: DECODE_STACK	hibss	DECODE_STACK,TABLE_SIZE */
int DECODE_STACK[TABLE_SIZE];
/* asm: NEXT_BUMP_CODE	.bss	NEXT_BUMP_CODE,1 */
int NEXT_BUMP_CODE;
/* asm: LINEBUFFER	lobss	LINEBUFFER,64 */
int LINEBUFFER[64];
// *----------------------------------------------------------------------------
// *
// *BIT_ADDR
// *SADDR
// *CURRENT_CODE_BITS
// *
// *
// *
// *----------------------------------------------------------------------------
/* asm: SAVESPCI	.word	SAVESPC+1 */
int SAVESPCI = (int)(SAVESPC+1);
/* asm: SAVESPC	.bss	SAVESPC,25 */
int SAVESPC[25];
// *----------------------------------------------------------------------------
#define MIN_X 240
#define MAX_X 300
/* asm: BOOT_PACIFY_SCREEN_P	.word	1 */
int BOOT_PACIFY_SCREEN_P = (int)(1);
/* asm: PREVX	.bss	PREVX,1 */
int PREVX;
/* asm: DELTA	.bss	DELTA,1 */
int DELTA;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *SECTION LOAD REQUEST
// *	IF ACTIVE THEN REQUEST IS QUEUED BY CREATING A PROCESS
// *
// *
// *PARAMETERS
// *	AR2	POINTER TO SECTION CONTROL
// *
/* asm: LASTLOAD	.bss	LASTLOAD,1 */
int LASTLOAD;
// *----------------------------------------------------------------------------

void INPUT_BITS(void)
{
    // asm: 	ADDI	CURRENT_CODE_BITS,BIT_ADDR,R0
    // asm: 	IFI	R0,GT,31,MULTIWORD
    // asm: 	LDI	*AR0,R0				;get data
    // asm: 	LSH	BIT_ADDR,R0
    // asm: 	LDI	CURRENT_CODE_BITS,R1
    // asm: 	SUBI	32,R1
    // asm: 	LSH	R1,R0
    // asm: 	ADDI	CURRENT_CODE_BITS,BIT_ADDR
    // asm: 	RETS
    // asm: MULTIWORD
    // asm: 	LDI	*AR0++,R1
    // asm: 	LSH	BIT_ADDR,R1		;left justify
    // asm: 	LDI	32,R0
    // asm: 	SUBI	BIT_ADDR,R0		;how many bits in 1st word
    // asm: 	LDI	CURRENT_CODE_BITS,R2
    // asm: 	SUBI	R0,R2			;bits remaining in second word
    // asm: 	LDI	R2,BIT_ADDR
    // asm: 	LDI	*AR0,R0
    // asm: 	SUBI	32,R2
    // asm: 	LSH	R2,R0			;second word now is right justified w/proper bits
    // asm: 	LDI	CURRENT_CODE_BITS,R2
    // asm: 	SUBI	32,R2
    // asm: 	LSH	R2,R1
    // asm: 	OR	R1,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INPUT_BITS", 0, 0);
    UNIMPL();
}

void PUTC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PARAMETERS
    // *	R0	CHARACTER (BYTE) TO OUTPUT
    // *	AR1	DADDR
    // *
    // *
    // asm: 	LDI	@LINEBUFFERI,AR2
    // asm: 	ADDI	bufcnt,AR2
    // asm: 	INC	bufcnt
    // asm: 	STI	PUTC_BUF,*AR2		;for AR timing
    // asm: 	CLRI	PUTC_BUF
    // asm: 	CLRI	PUTC_SH
    // asm: 	CMPI	64,bufcnt
    // asm: 	RETSLT
    // ;	LDP	@_newbut
    // ;	NOT	@_newbut,R0
    // ;	TSTB	SW_DIAG,R0
    // ;	BNZ	ENTER_DIAG
    // ;	LDP	@DIPRAM
    // ;	NOT	@DIPRAM,R0
    // ;	TSTB	DIP_DIAG,R0
    // ;	BNZ	ENTER_DIAG
#if DEBUG
    // asm: 	LDI	0A0h,AR2
    // asm: 	LS	16,AR2
    // asm: 	CMPI	AR2,AR1
    // asm: 	SLOCKON	LT,"COMP\PUTC   ATTEMPT UNDER WRITE OF WAVERAM"
    // asm: 	LDI	0BFh,AR2
    // asm: 	LS	16,AR2
    // asm: 	CMPI	AR2,AR1
    // asm: 	SLOCKON	GT,"COMP\PUTC  ATTEMPT OVER WRITE OF WAVERAM"
#endif
    // 	;PACIFY
    // asm: 	LDI	@PACIFY_COUNT,R0
    // asm: 	ADDI	64,R0
    // asm: 	STI	R0,@PACIFY_COUNT
    // 	;
    // asm: 	CLRI	bufcnt
    // asm: 	PUSH	AR4
    // asm: 	LDI	@LINEBUFFERI,AR4
    // asm: 	CLRI	AR2			;for dummy read
    // asm: 	PUSH	ST			;this push must be here
    // asm: 	PUSH	RC
    // asm: 	PUSH	RE
    // asm: 	PUSH	RS
    // asm: 	PUSH	R7
    // asm: 	PUSH	IE
    // asm: 	LDP	@COMMINTM
    // asm: 	LDI	@COMMINTM,IE
    // asm: 	SETDP
    // ;	PUSH	IE			;disable interrupts
    // ;	LDI	0,IE			;watch for pipeline conflicts
    // asm: 	LDI	HARD_WS,R0
    // asm: 	LDI	SOFT_WS,R1
    // ;	AND	0DFFFh,ST		;turn off GIE.
    // ;	POP	IE
    // asm: 	LDP	@CPU_WS
    // asm: 	STI	R0,@CPU_WS		;set hard wait states
    // asm: 	LDI	63,RC
    // asm: 	LDI	-16,R7
    // asm: 	RPTB	WVWRLP2
    // asm: 	LDI	*AR4,R2			;read from the buffer
    // asm:     	STI	R2,*AR1++		;write to wave ram
    // asm:  	LSH	R7,*AR4++,R2		;read/shift right
WVWRLP2:
    // asm: STI	R2,*AR1++
    // asm: 	LDI	*AR4,R2			;DUMMY READ TO CLEAR THE LINE
    // asm: 	STI	R1,@CPU_WS		;set soft wait states
    // asm: 	SETDP
    // asm: 	POP	IE
    // asm: 	POP	R7			;this pop sequence must be here
    // asm: 	POP	RS
    // asm: 	POP	RE
    // asm: 	POP	RC
    // asm: 	POP	ST
    // asm: 	BUD	ENABLEGIE
    // asm: 	POP	AR4
    // asm: 	CLRI	PUTC_BUF
    // asm: 	CLRI	PUTC_SH
    // 	;---->	BUD	ENABLEGIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PUTC", 0, 0);
    UNIMPL();
}

void DECOMPRESS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	AR4	SADDR
    // *	AR5	DADDR
    // *
    // *
    // asm: 	CALL	PUSHALL
    // 	;
    // 	;NEW ADDITION.  DONT F*CK THE WAVERAM
    // 	;
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // 	;
    // 	;
    // asm: 	LDI	1,R0
    // asm: 	STPI	R0,@DECOMP_ACTIVE
    // asm: 	CLRI	R0
    // asm: 	STPI	R0,@FLUSH_COUNT
    // asm: 	LDI	AR4,AR0				;SADDR
    // asm: 	LDI	AR5,AR1				;DADDR
    // asm: 	CLRI	bufcnt
    // asm: 	LDI	@DICTI,AR3
    // asm: 	LDI	CHARACTER,IR0
    // asm: 	CLRI	BIT_ADDR
    // asm: 	CLRI	PUTC_BUF
    // asm: 	CLRI	PUTC_SH
    // asm: 	CALL	SAVE_DECOMP_REGS
    // asm: 	LDI	@HARD_SECTION_LOAD,R0
    // asm: 	BZ	NOHARDLOAD
    // asm: 	CALL	DECOMPRESS_PROC
NOHARDLOAD:
    // asm: 	CALL	POPALL
    // asm: 	RETS
DECOMPRESS_TOPLP:
    // ;	LDI	@FLUSH_COUNT,R0
    // ;	INC	R0
    // ;	STPI	R0,@FLUSH_COUNT
    // ;	IFI	R0,LT,3,DECOMPRESS_TOPLP3
    // ;	CLRI	R0
    // ;	STPI	R0,@FLUSH_COUNT
    // asm: 	CALL	FEED_WATCHDOG
    // 	;
    // asm: 	LDI	@PACIFY_COUNT,R0
    // asm: 	CMPI	PACIFY_MOMENT,R0
    // asm: 	BLT	DECOMPRESS_TOPLP3
    // 	;
    // asm: 	LDI	@HARD_SECTION_LOAD,R0
    // asm: 	BZ	CONT
    // asm: 	LDI	@BOOT_PACIFY_SCREEN_P,R0
    // asm: 	CALLNZ	BOOT_PACIFY_SCREEN
    // asm: 	BU	DECOMPRESS_TOPLP3
CONT:
    // asm: 	CALL	SAVE_DECOMP_REGS
    // asm: 	CALL	POPALL
    // ;	CALL	ENABLEGIE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECOMPRESS", 0, 0);
    UNIMPL();
}

void DECOMPRESS_PROC(void)
{
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm: 	RETSZ
    // 	;
    // 	;NEW ADDITION.  DONT F*CK THE WAVERAM
    // 	;
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // 	;
    // 	;
    // 	;PACIFIER
    // asm: 	CLRI	R0
    // asm: 	STPI	R0,@PACIFY_COUNT
    // 	;
    // ;	PUSH	IE			;disable interrupts
    // ;	LDI	0,IE			;watch for pipeline conflicts
    // ;	LDI	HARD_WS,R0
    // ;	LDI	SOFT_WS,R1
    // ;	AND	0DFFFh,ST		;turn off GIE.
    // ;	POP	IE
    // asm: DECOMPRESS_TOPLP2
    // asm: 	CALL	PUSHALL
    // asm: 	CALL	RESTORE_DECOMP_REGS
    // ;	CALL	FEED_WATCHDOG
DECOMPRESS_TOPLP3:
    // asm: 	LDI	FIRST_CODE,NEXT_CODE
    // asm: 	LDI	9,CURRENT_CODE_BITS
    // asm: 	LDI	511,R0
    // asm: 	STPI	R0,@NEXT_BUMP_CODE
    // asm: 	CALL	INPUT_BITS		;returns in R0
    // asm: 	LDI	R0,old_code		;old_code = INPUTBITS()
    // asm: 	IFI	old_code,EQ,END_OF_STREAM,DECOMPRESSX
    // asm: 	LDI	old_code,character	;character = old_code
    // 	;redun	LDI	old_code,R0
    // asm: 	LSH	PUTC_SH,R0		;this is inlined!
    // asm: 	OR	R0,PUTC_BUF		;
    // asm: 	ADDI	8,PUTC_SH		;
    // asm: 	CMPI	32,PUTC_SH		;
    // asm: 	CALLGE	PUTC
    // ;	CALL	PUTC			;
DECOMPRESSLP:
    // asm: 	CALL	INPUT_BITS
    // asm: 	LDI	retval,new_code
    // asm: 	CMPI	END_OF_STREAM,new_code
    // asm: 	BEQ	DECOMPRESSX
    // asm: 	CMPI	FLUSH_CODE,new_code
    // asm: 	BEQ	DECOMPRESS_TOPLP
    // asm: 	CMPI	BUMP_CODE,new_code
    // asm: 	BNE	NOBUMP
    // asm: 	INC	CURRENT_CODE_BITS
    // asm: 	BU	DECOMPRESSLP
NOBUMP:
    // asm: 	CMPI	NEXT_CODE,new_code
    // asm: 	BLTD	NODS
    // asm: 	LDP	@DECODE_STACKI
    // asm: 	LDI	@DECODE_STACKI,AR4
    // asm: 	CLRI	count
    // ;	NOP
    // 	;---->	BLTD	NODS
    // asm: 	BUD	NODSRT
    // asm: 	STI	character,*AR4++		;decode_stack[0] = character
    // asm: 	LDI	1,count				;count
    // asm: 	LDI	old_code,AR5			;code
    // 	;---->	BUD	NODSRT
NODS:
    // ;	CLRI	count
    // asm: 	LDI	new_code,AR5
NODSRT:
    // asm: 	IFI	AR5,LE,255,DECODEL1i		;while( code > 255)  {
DECODEL3i:
    // asm: 	ADDI	AR3,AR5
    // asm: 	LDI	*+AR5(IR0),R0			;CHARACTER
    // asm: 	STI	R0,*AR4++			;decode_stack[ count++] = DICT[ code].character
    // asm: 	INC	count				;must keep counter correct
    // asm: 	LDI	*+AR5(PARENT_CODE),AR5		;code = DICT[code].parent_code
    // asm: 	CMPI	255,AR5
    // asm: 	BGT	DECODEL3i
    // asm: DECODEL1i
    // asm: 	LDI	AR5,character
    // asm: 	LDI	AR5,R0				;setup the first write
    // asm: 	RPTB	BLOOPER
    // asm: 	LSH	PUTC_SH,R0		;this is inlined!
    // asm: 	OR	R0,PUTC_BUF		;
    // asm: 	ADDI	8,PUTC_SH		;
    // asm: 	CMPI	32,PUTC_SH		;
    // asm: 	CALLGE	PUTC
    // 	;CALL	PUTC
    // asm: BLOOPER	LDI	*--AR4,R0			;and this becomes a pre-decrement
    // asm: 	ADDI	NEXT_CODE,AR3,AR2
    // asm: 	STI	old_code,*+AR2(PARENT_CODE)	;DICT[next_code].parent = old_code
    // asm: 	BUD	DECOMPRESSLP
    // asm: 	STI	character,*+AR2(IR0)		;DICT[next_code].character = character
    // asm: 	INC	NEXT_CODE
    // asm: 	LDI	new_code,old_code
    // 	;---->	BUD	DECOMPRESSLP
DECOMPRESSX:
    // asm: 	CLRI	R0
    // asm: 	STPI	R0,@DECOMP_ACTIVE
    // asm: 	STPI	R0,@HARD_SECTION_LOAD
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECOMPRESS_PROC", 0, 0);
    UNIMPL();
}

void SAVE_DECOMP_REGS(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDP	@SAVESPC
    // asm: 	STI	AR0,@SAVESPC
    // asm: 	LDI	@SAVESPCI,AR0
    // asm: 	STI	AR1,*AR0++
    // asm: 	STI	AR2,*AR0++
    // asm: 	STI	AR3,*AR0++
    // asm: 	STI	AR4,*AR0++
    // asm: 	STI	AR5,*AR0++
    // asm: 	STI	AR6,*AR0++
    // asm: 	STI	AR7,*AR0++
    // asm: 	STI	R1,*AR0++
    // asm: 	STI	R2,*AR0++
    // asm: 	STI	R3,*AR0++
    // asm: 	STI	R4,*AR0++
    // asm: 	STI	R5,*AR0++
    // asm: 	STI	R6,*AR0++
    // asm: 	STI	R7,*AR0++
    // asm: 	STI	RC,*AR0++
    // asm: 	STI	RE,*AR0++
    // asm: 	STI	RS,*AR0++
    // asm: 	STI	IR0,*AR0++
    // asm: 	STI	IR1,*AR0++
    // asm: 	STI	BK,*AR0++
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SAVE_DECOMP_REGS", 0, 0);
    UNIMPL();
}

void RESTORE_DECOMP_REGS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@SAVESPCI,AR0
    // asm: 	LDI	*AR0++,AR1
    // asm: 	LDI	*AR0++,AR2
    // asm: 	LDI	*AR0++,AR3
    // asm: 	LDI	*AR0++,AR4
    // asm: 	LDI	*AR0++,AR5
    // asm: 	LDI	*AR0++,AR6
    // asm: 	LDI	*AR0++,AR7
    // asm: 	LDI	*AR0++,R1
    // asm: 	LDI	*AR0++,R2
    // asm: 	LDI	*AR0++,R3
    // asm: 	LDI	*AR0++,R4
    // asm: 	LDI	*AR0++,R5
    // asm: 	LDI	*AR0++,R6
    // asm: 	LDI	*AR0++,R7
    // asm: 	LDI	*AR0++,RC
    // asm: 	LDI	*AR0++,RE
    // asm: 	LDI	*AR0++,RS
    // asm: 	LDI	*AR0++,IR0
    // asm: 	LDI	*AR0++,IR1
    // asm: 	LDI	*AR0++,BK
    // asm: 	LDP	@SAVESPC
    // asm: 	LDI	@SAVESPC,AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESTORE_DECOMP_REGS", 0, 0);
    UNIMPL();
}

void BOOT_PACIFY_SCREEN(void)
{
    // ;PREVX		.word	MIN_X
    // ;DELTA		.word	1
    // *----------------------------------------------------------------------------
    // asm: 	CALL	SAVE_DECOMP_REGS
    // asm: 	LDI	@PREVX,R6
    // asm: 	LDI	R6,AR2
    // asm: 	LDI	R6,R3
    // asm: 	LDI	111,R2
    // asm: 	LDI	116,RC
    // ;	LDI	@DELTA,R0
    // ;	LDILT	0,RS
    // ;	LDIGT	11,RS
    // asm: 	LDI	0,RS
    // asm: 	CALL	_line
    // asm: 	LDI	@PREVX,R6
    // asm: 	LDI	@DELTA,R7
    // asm: 	ADDI	R7,R6
    // asm: 	STI	R6,@PREVX
    // asm: 	LDI	R6,AR2
    // asm: 	LDI	R6,R3
    // asm: 	LDI	111,R2
    // asm: 	LDI	116,RC
    // asm: 	LDI	3,RS
    // asm: 	CALL	_line
    // asm: 	LDI	@PREVX,R6
    // asm: 	CMPI	MIN_X,R6
    // asm: 	BGT	LL
    // asm: 	LDI	1,R7
    // asm: 	STPI	R7,@DELTA
LL:
    // asm: 	CMPI	MAX_X,R6
    // asm: 	BLT	LLL
    // asm: 	LDI	-1,R7
    // asm: 	STPI	R7,@DELTA
LLL:
    // asm: 	CALL	RESTORE_DECOMP_REGS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOOT_PACIFY_SCREEN", 0, 0);
    UNIMPL();
}

void LOAD_SECTION_REQ(void)
{
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm: 	BZ	NOWTLD
    // 	;a decompression is executing, queue the request
    // 	;
    // 	;
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR4
    // asm: 	LDI	AR2,AR4
    // asm: 	CREATE	REQWAIT,SPAWNER_C|LOAD_REQ_T
    // asm: 	POP	AR4
    // asm: 	POP	AR0
    // asm: 	POP	R2
    // asm: 	BU	NOLOAD
NOWTLD:
    // ;	LDI	@LASTLOAD,AR0
    // ;	CMPI	AR0,AR2
    // ;	BEQ	NOLOAD
    // ;	STI	AR2,@LASTLOAD
    // asm: 	LDI	*AR2++,AR4
    // asm: 	LDI	*AR2++,AR5
    // asm: 	CALL	DECOMPRESS
NOLOAD:
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SECTION_REQ", 0, 0);
    UNIMPL();
}

void REQWAIT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	SLEEP	1
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm: 	BNZ	REQWAIT
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REQWAIT", 0, 0);
    UNIMPL();
}

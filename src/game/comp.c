#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "objects.h"
#include "text.h"
#include "port.h"
#include "comp.h"

/*
 * Source module: asm/COMP.ASM
 */

void INPUT_BITS(void);
void PUTC(void);
void DECOMPRESS(void);
void DECOMPRESS_PROC(void);
void SAVE_DECOMP_REGS(void);
void RESTORE_DECOMP_REGS(void);
void BOOT_PACIFY_SCREEN(void);
void LOAD_SECTION_REQ(void);
void REQWAIT(void);

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
/* *
*
 */
#define BITS 12
#define MAX_CODE ((( 1 << BITS) -1 ))
#define TABLE_SIZE 4421
#define END_OF_STREAM 256
#define BUMP_CODE 257
#define FLUSH_CODE 258
#define FIRST_CODE 259
#define UNUSED (-1)
/* asm: DICT	hibss	DICT,TABLE_SIZE*DICT_SIZ */
int DICT[TABLE_SIZE*DICT_SIZ];
/* asm: DECODE_STACK	hibss	DECODE_STACK,TABLE_SIZE */
int DECODE_STACK[TABLE_SIZE];
/* asm: NEXT_BUMP_CODE	.bss	NEXT_BUMP_CODE,1 */
int NEXT_BUMP_CODE;
/* asm: LINEBUFFER	lobss	LINEBUFFER,64 */
int LINEBUFFER[64];

/* *----------------------------------------------------------------------------
*
*BIT_ADDR
*SADDR
*CURRENT_CODE_BITS
*
*
*
 */
void INPUT_BITS(void)
{
    // asm: 	ADDI	CURRENT_CODE_BITS,BIT_ADDR,R0
    // asm: 	IFI	R0,GT,31,MULTIWORD
    // asm: 	LDI	*AR0,R0				;get data
    // asm: 	LSH	BIT_ADDR,R0
    // asm: 	LDI	CURRENT_CODE_BITS,R1
    // asm 0000A2DB: 	SUBI	32,R1
    // asm 0000A2DC: 	LSH	R1,R0
    // asm: 	ADDI	CURRENT_CODE_BITS,BIT_ADDR
    // asm 0000A2DE: 	RETS
    // asm 0000A2E0: MULTIWORD
    // asm 0000A2E2: 	LDI	*AR0++,R1
    // asm 0000A2E4: 	LSH	BIT_ADDR,R1		;left justify
    // asm: 	LDI	32,R0
    // asm: 	SUBI	BIT_ADDR,R0		;how many bits in 1st word
    // asm 0000A2E5: 	LDI	CURRENT_CODE_BITS,R2
    // asm: 	SUBI	R0,R2			;bits remaining in second word
    // asm 0000A2E6: 	LDI	R2,BIT_ADDR
    // asm 0000A2E8: 	LDI	*AR0,R0
    // asm: 	SUBI	32,R2
    // asm 0000A2E9: 	LSH	R2,R0			;second word now is right justified w/proper bits
    // asm 0000A2EB: 	LDI	CURRENT_CODE_BITS,R2
    // asm: 	SUBI	32,R2
    // asm 0000A2EC: 	LSH	R2,R1
    // asm 0000A2ED: 	OR	R1,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INPUT_BITS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PARAMETERS
*	R0	CHARACTER (BYTE) TO OUTPUT
*	AR1	DADDR
*
*
 */
void PUTC(void)
{
    // asm: 	LDI	@LINEBUFFERI,AR2
    // asm: 	ADDI	bufcnt,AR2
    // asm: 	INC	bufcnt
    // asm: 	STI	PUTC_BUF,*AR2		;for AR timing
    // asm: 	CLRI	PUTC_BUF
    // asm 0000A2F4: 	CLRI	PUTC_SH
    // asm 0000A2F6: 	CMPI	64,bufcnt
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
    // asm 0000A2FC: 	CLRI	bufcnt
    // asm 0000A2FD: 	PUSH	AR4
    // asm 0000A2FE: 	LDI	@LINEBUFFERI,AR4
    // asm: 	CLRI	AR2			;for dummy read
    // asm: 	PUSH	ST			;this push must be here
    // asm 0000A2FF: 	PUSH	RC
    // asm 0000A300: 	PUSH	RE
    // asm 0000A301: 	PUSH	RS
    // asm 0000A302: 	PUSH	R7
    // asm 0000A303: 	PUSH	IE
    // asm 0000A304: 	LDP	@COMMINTM
    // asm 0000A305: 	LDI	@COMMINTM,IE
    // asm 0000A306: 	SETDP
    // ;	PUSH	IE			;disable interrupts
    // ;	LDI	0,IE			;watch for pipeline conflicts
    // asm 0000A309: 	LDI	HARD_WS,R0
    // asm 0000A30A: 	LDI	SOFT_WS,R1
    // ;	AND	0DFFFh,ST		;turn off GIE.
    // ;	POP	IE
    // asm 0000A30C: 	LDP	@CPU_WS
    // asm 0000A30D: 	STI	R0,@CPU_WS		;set hard wait states
    // asm: 	LDI	63,RC
    // asm: 	LDI	-16,R7
    // asm: 	RPTB	WVWRLP2
    // asm 0000A30E: 	LDI	*AR4,R2			;read from the buffer
    // asm 0000A30F:     	STI	R2,*AR1++		;write to wave ram
    // asm:  	LSH	R7,*AR4++,R2		;read/shift right
WVWRLP2:
    // asm 0000A310: STI	R2,*AR1++
    // asm 0000A313: 	LDI	*AR4,R2			;DUMMY READ TO CLEAR THE LINE
    // asm 0000A314: 	STI	R1,@CPU_WS		;set soft wait states
    // asm 0000A315: 	SETDP
    // asm: 	POP	IE
    // asm 0000A317: 	POP	R7			;this pop sequence must be here
    // asm 0000A318: 	POP	RS
    // asm 0000A319: 	POP	RE
    // asm: 	POP	RC
    // asm 0000A31A: 	POP	ST
    // asm 0000A31B: 	BUD	ENABLEGIE
    // asm 0000A31C: 	POP	AR4
    // asm 0000A31D: 	CLRI	PUTC_BUF
    // asm 0000A31E: 	CLRI	PUTC_SH
    // 	;---->	BUD	ENABLEGIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PUTC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	AR4	SADDR
*	AR5	DADDR
*
*
 */
void DECOMPRESS(void)
{
    // asm: 	CALL	PUSHALL
    // 	;
    // 	;NEW ADDITION.  DONT F*CK THE WAVERAM
    // 	;
    // asm 0000A324: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // 	;
    // 	;
    // asm 0000A32A: 	LDI	1,R0
    // asm 0000A32F: 	STPI	R0,@DECOMP_ACTIVE
    // asm: 	CLRI	R0
    // asm: 	STPI	R0,@FLUSH_COUNT
    // asm 0000A332: 	LDI	AR4,AR0				;SADDR
    // asm 0000A333: 	LDI	AR5,AR1				;DADDR
    // asm: 	CLRI	bufcnt
    // asm: 	LDI	@DICTI,AR3
    // asm 0000A334: 	LDI	CHARACTER,IR0
    // asm 0000A335: 	CLRI	BIT_ADDR
    // asm 0000A336: 	CLRI	PUTC_BUF
    // asm: 	CLRI	PUTC_SH
    // asm 0000A338: 	CALL	SAVE_DECOMP_REGS
    // asm: 	LDI	@HARD_SECTION_LOAD,R0
    // asm 0000A33A: 	BZ	NOHARDLOAD
    // asm 0000A33B: 	CALL	DECOMPRESS_PROC
NOHARDLOAD:
    // asm 0000A33D: 	CALL	POPALL
    // asm 0000A33E: 	RETS
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
    // asm 0000A343: 	LDI	@HARD_SECTION_LOAD,R0
    // asm 0000A344: 	BZ	CONT
    // asm: 	LDI	@BOOT_PACIFY_SCREEN_P,R0
    // asm: 	CALLNZ	BOOT_PACIFY_SCREEN
    // asm 0000A346: 	BU	DECOMPRESS_TOPLP3
CONT:
    // asm 0000A349: 	CALL	SAVE_DECOMP_REGS
    // asm: 	CALL	POPALL
    // ;	CALL	ENABLEGIE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECOMPRESS", 0, 0);
    UNIMPL();
}

void DECOMPRESS_PROC(void)
{
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm 0000A34D: 	RETSZ
    // 	;
    // 	;NEW ADDITION.  DONT F*CK THE WAVERAM
    // 	;
    // asm 0000A34F: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
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
    // asm 0000A35D: DECOMPRESS_TOPLP2
    // asm: 	CALL	PUSHALL
    // asm: 	CALL	RESTORE_DECOMP_REGS
    // ;	CALL	FEED_WATCHDOG
DECOMPRESS_TOPLP3:
    // asm: 	LDI	FIRST_CODE,NEXT_CODE
    // asm 0000A35D: 	LDI	9,CURRENT_CODE_BITS
    // asm 0000A35E: 	LDI	511,R0
    // asm: 	STPI	R0,@NEXT_BUMP_CODE
    // asm 0000A35F: 	CALL	INPUT_BITS		;returns in R0
    // asm 0000A360: 	LDI	R0,old_code		;old_code = INPUTBITS()
    // asm 0000A361: 	IFI	old_code,EQ,END_OF_STREAM,DECOMPRESSX
    // asm 0000A362: 	LDI	old_code,character	;character = old_code
    // 	;redun	LDI	old_code,R0
    // asm 0000A364: 	LSH	PUTC_SH,R0		;this is inlined!
    // asm 0000A365: 	OR	R0,PUTC_BUF		;
    // asm 0000A367: 	ADDI	8,PUTC_SH		;
    // asm: 	CMPI	32,PUTC_SH		;
    // asm: 	CALLGE	PUTC
    // ;	CALL	PUTC			;
DECOMPRESSLP:
    // asm: 	CALL	INPUT_BITS
    // asm: 	LDI	retval,new_code
    // asm: 	CMPI	END_OF_STREAM,new_code
    // asm: 	BEQ	DECOMPRESSX
    // asm: 	CMPI	FLUSH_CODE,new_code
    // asm 0000A36D: 	BEQ	DECOMPRESS_TOPLP
    // asm 0000A36F: 	CMPI	BUMP_CODE,new_code
    // asm 0000A370: 	BNE	NOBUMP
    // asm 0000A371: 	INC	CURRENT_CODE_BITS
    // asm 0000A372: 	BU	DECOMPRESSLP
NOBUMP:
    // asm 0000A374: 	CMPI	NEXT_CODE,new_code
    // asm: 	BLTD	NODS
    // asm 0000A376: 	LDP	@DECODE_STACKI
    // asm: 	LDI	@DECODE_STACKI,AR4
    // asm: 	CLRI	count
    // ;	NOP
    // 	;---->	BLTD	NODS
    // asm 0000A379: 	BUD	NODSRT
    // asm 0000A37A: 	STI	character,*AR4++		;decode_stack[0] = character
    // asm 0000A37B: 	LDI	1,count				;count
    // asm: 	LDI	old_code,AR5			;code
    // 	;---->	BUD	NODSRT
NODS:
    // ;	CLRI	count
    // asm 0000A37D: 	LDI	new_code,AR5
NODSRT:
    // asm: 	IFI	AR5,LE,255,DECODEL1i		;while( code > 255)  {
DECODEL3i:
    // asm: 	ADDI	AR3,AR5
    // asm: 	LDI	*+AR5(IR0),R0			;CHARACTER
    // asm: 	STI	R0,*AR4++			;decode_stack[ count++] = DICT[ code].character
    // asm: 	INC	count				;must keep counter correct
    // asm 0000A381: 	LDI	*+AR5(PARENT_CODE),AR5		;code = DICT[code].parent_code
    // asm: 	CMPI	255,AR5
    // asm 0000A383: 	BGT	DECODEL3i
    // asm 0000A385: DECODEL1i
    // asm 0000A387: 	LDI	AR5,character
    // asm 0000A388: 	LDI	AR5,R0				;setup the first write
    // asm: 	RPTB	BLOOPER
    // asm: 	LSH	PUTC_SH,R0		;this is inlined!
    // asm: 	OR	R0,PUTC_BUF		;
    // asm 0000A38A: 	ADDI	8,PUTC_SH		;
    // asm 0000A38B: 	CMPI	32,PUTC_SH		;
    // asm: 	CALLGE	PUTC
    // 	;CALL	PUTC
    // asm 0000A38E: BLOOPER	LDI	*--AR4,R0			;and this becomes a pre-decrement
    // asm 0000A390: 	ADDI	NEXT_CODE,AR3,AR2
    // asm 0000A391: 	STI	old_code,*+AR2(PARENT_CODE)	;DICT[next_code].parent = old_code
    // asm: 	BUD	DECOMPRESSLP
    // asm 0000A392: 	STI	character,*+AR2(IR0)		;DICT[next_code].character = character
    // asm: 	INC	NEXT_CODE
    // asm 0000A393: 	LDI	new_code,old_code
    // 	;---->	BUD	DECOMPRESSLP
DECOMPRESSX:
    // asm 0000A397: 	CLRI	R0
    // asm 0000A398: 	STPI	R0,@DECOMP_ACTIVE
    // asm: 	STPI	R0,@HARD_SECTION_LOAD
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECOMPRESS_PROC", 0, 0);
    UNIMPL();
}

/* asm: SAVESPCI	.word	SAVESPC+1 */
int SAVESPCI = SAVESPC+1;
/* asm: SAVESPC	.bss	SAVESPC,25 */
int SAVESPC[25];

/* *----------------------------------------------------------------------------
 */
void SAVE_DECOMP_REGS(void)
{
    // asm: 	LDP	@SAVESPC
    // asm: 	STI	AR0,@SAVESPC
    // asm: 	LDI	@SAVESPCI,AR0
    // asm: 	STI	AR1,*AR0++
    // asm: 	STI	AR2,*AR0++
    // asm 0000A39F: 	STI	AR3,*AR0++
    // asm 0000A3A0: 	STI	AR4,*AR0++
    // asm: 	STI	AR5,*AR0++
    // asm 0000A3A1: 	STI	AR6,*AR0++
    // asm 0000A3A2: 	STI	AR7,*AR0++
    // asm 0000A3A4: 	STI	R1,*AR0++
    // asm 0000A3A5: 	STI	R2,*AR0++
    // asm 0000A3A6: 	STI	R3,*AR0++
    // asm 0000A3A7: 	STI	R4,*AR0++
    // asm 0000A3A8: 	STI	R5,*AR0++
    // asm: 	STI	R6,*AR0++
    // asm 0000A3A9: 	STI	R7,*AR0++
    // asm 0000A3AB: 	STI	RC,*AR0++
    // asm 0000A3AC: 	STI	RE,*AR0++
    // asm 0000A3AD: 	STI	RS,*AR0++
    // asm 0000A3AE: 	STI	IR0,*AR0++
    // asm 0000A3AF: 	STI	IR1,*AR0++
    // asm: 	STI	BK,*AR0++
    // asm 0000A3B1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SAVE_DECOMP_REGS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RESTORE_DECOMP_REGS(void)
{
    // asm: 	LDI	@SAVESPCI,AR0
    // asm: 	LDI	*AR0++,AR1
    // asm: 	LDI	*AR0++,AR2
    // asm: 	LDI	*AR0++,AR3
    // asm: 	LDI	*AR0++,AR4
    // asm: 	LDI	*AR0++,AR5
    // asm 0000A3B7: 	LDI	*AR0++,AR6
    // asm 0000A3B8: 	LDI	*AR0++,AR7
    // asm 0000A3BA: 	LDI	*AR0++,R1
    // asm 0000A3BB: 	LDI	*AR0++,R2
    // asm 0000A3BC: 	LDI	*AR0++,R3
    // asm 0000A3BD: 	LDI	*AR0++,R4
    // asm 0000A3BE: 	LDI	*AR0++,R5
    // asm: 	LDI	*AR0++,R6
    // asm 0000A3BF: 	LDI	*AR0++,R7
    // asm 0000A3C1: 	LDI	*AR0++,RC
    // asm 0000A3C2: 	LDI	*AR0++,RE
    // asm 0000A3C3: 	LDI	*AR0++,RS
    // asm 0000A3C4: 	LDI	*AR0++,IR0
    // asm 0000A3C5: 	LDI	*AR0++,IR1
    // asm: 	LDI	*AR0++,BK
    // asm 0000A3C7: 	LDP	@SAVESPC
    // asm 0000A3C8: 	LDI	@SAVESPC,AR0
    // asm 0000A3CA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESTORE_DECOMP_REGS", 0, 0);
    UNIMPL();
}

#define MIN_X 240 //if this changes modify CUSA.ASM
#define MAX_X 300
/* asm: BOOT_PACIFY_SCREEN_P	.word	1 */
int BOOT_PACIFY_SCREEN_P = 1;
/* asm: PREVX	.bss	PREVX,1 */
int PREVX;
/* asm: DELTA	.bss	DELTA,1 */
int DELTA;

/* *----------------------------------------------------------------------------
 */
void BOOT_PACIFY_SCREEN(void)
{
    // asm: 	CALL	SAVE_DECOMP_REGS
    // asm: 	LDI	@PREVX,R6
    // asm: 	LDI	R6,AR2
    // asm: 	LDI	R6,R3
    // asm 0000A3D0: 	LDI	111,R2
    // asm: 	LDI	116,RC
    // ;	LDI	@DELTA,R0
    // ;	LDILT	0,RS
    // ;	LDIGT	11,RS
    // asm 0000A3D4: 	LDI	0,RS
    // asm 0000A3D5: 	CALL	_line
    // asm: 	LDI	@PREVX,R6
    // asm: 	LDI	@DELTA,R7
    // asm: 	ADDI	R7,R6
    // asm 0000A3D6: 	STI	R6,@PREVX
    // asm: 	LDI	R6,AR2
    // asm 0000A3D8: 	LDI	R6,R3
    // asm 0000A3D9: 	LDI	111,R2
    // asm 0000A3DA: 	LDI	116,RC
    // asm 0000A3DB: 	LDI	3,RS
    // asm: 	CALL	_line
    // asm 0000A3DE: 	LDI	@PREVX,R6
    // asm 0000A3DF: 	CMPI	MIN_X,R6
    // asm 0000A3E0: 	BGT	LL
    // asm 0000A3E1: 	LDI	1,R7
    // asm: 	STPI	R7,@DELTA
LL:
    // asm 0000A3E2: 	CMPI	MAX_X,R6
    // asm 0000A3E3: 	BLT	LLL
    // asm 0000A3E4: 	LDI	-1,R7
    // asm 0000A3E5: 	STPI	R7,@DELTA
LLL:
    // asm 0000A3E7: 	CALL	RESTORE_DECOMP_REGS
    // asm 0000A3E8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOOT_PACIFY_SCREEN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SECTION LOAD REQUEST
*	IF ACTIVE THEN REQUEST IS QUEUED BY CREATING A PROCESS
*
*
*PARAMETERS
*	AR2	POINTER TO SECTION CONTROL
*
 */
/* asm: LASTLOAD	.bss	LASTLOAD,1 */
int LASTLOAD;

void LOAD_SECTION_REQ(void)
{
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm: 	BZ	NOWTLD
    // 	;a decompression is executing, queue the request
    // 	;
    // 	;
    // asm 0000A3EF: 	PUSH	R2
    // asm 0000A3F0: 	PUSH	AR0
    // asm: 	PUSH	AR4
    // asm: 	LDI	AR2,AR4
    // asm: 	CREATE	REQWAIT,SPAWNER_C|LOAD_REQ_T
    // asm: 	POP	AR4
    // asm 0000A3F1: 	POP	AR0
    // asm 0000A3F2: 	POP	R2
    // asm 0000A3F3: 	BU	NOLOAD
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
    // asm 0000A3FC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SECTION_REQ", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void REQWAIT(void)
{
    // asm 0000A401: 	SLEEP	1
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm: 	BNZ	REQWAIT
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm 0000A402: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REQWAIT", 0, 0);
    UNIMPL();
}

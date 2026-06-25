#include "comp.h"
#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/COMP.ASM
 */

static void INPUT_BITS(void);
static void PUTC(void);
void DECOMPRESS(void);
void DECOMPRESS_PROC(void);
static void SAVE_DECOMP_REGS(void);
static void RESTORE_DECOMP_REGS(void);
static void BOOT_PACIFY_SCREEN(void);
void LOAD_SECTION_REQ(void);
static void REQWAIT(void);

#define CPU_WSI CPU_WS
#define DICTI DICT
#define DECODE_STACKI DECODE_STACK
#define LINEBUFFERI LINEBUFFER

/*
 *----------------------------------------------------------------------------
 *DECOMPRESSION SYSTEM
 *
 *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

/* asm: PADDING	.bss	PADDING,50 */
static int PADDING[50];
/* asm: DECOMP_ACTIVE	.bss	DECOMP_ACTIVE,1 */
int DECOMP_ACTIVE;
/* asm: HARD_SECTION_LOAD	.bss	HARD_SECTION_LOAD,1 */
int HARD_SECTION_LOAD;
/* asm: FLUSH_COUNT	.bss	FLUSH_COUNT,1 */
int FLUSH_COUNT;
/* asm: PACIFY_COUNT	.bss	PACIFY_COUNT,1 */
int PACIFY_COUNT;
#define PACIFY_MOMENT 2048
/*
;PACIFY_MOMENT	.set	512
*----------------------------------------------------------------------------
*REGISTER ALLOCATION
*
*AR0	SADDR	(source addr or bitstream)
*AR1	DADDR	(dest addr)
*AR2	scratch
*AR3	DICTI
*AR4	scratch
*AR5	scratch
*AR6	CURRENT_CODE_BITS
*AR7	BIT_ADDR
*
*R0	scratch
*R1	scratch
*R2	scratch
*R3	PUTC_SH
*R4	new_code
*R5	old_code
*R6	character
*R7	BUFCNT
*
*
*IR0	CHARACTER (offset)
*IR1	PUTC_BUF
*BK	NEXT_CODE
*
*RC	count
*RS
*RE
*
*
*/

/*
 *
 *
 */
#define BITS 12
#define MAX_CODE (((1 << BITS) - 1))
#define TABLE_SIZE 4421
#define END_OF_STREAM 256
#define BUMP_CODE 257
#define FLUSH_CODE 258
#define FIRST_CODE 259
#define UNUSED (-1)
/*
 *
 *
 */

tDICT DICT;
/* asm: DECODE_STACK	hibss	DECODE_STACK,TABLE_SIZE */
int DECODE_STACK[TABLE_SIZE];
/* asm: NEXT_BUMP_CODE	.bss	NEXT_BUMP_CODE,1 */
int NEXT_BUMP_CODE;
/* asm: LINEBUFFER	lobss	LINEBUFFER,64 */
int LINEBUFFER[64];

/*
 *----------------------------------------------------------------------------
 *
 *BIT_ADDR
 *SADDR
 *CURRENT_CODE_BITS
 *
 *
 *
 */
static void INPUT_BITS(void) {
    // asm 0000A2DB: 	ADDI	CURRENT_CODE_BITS,BIT_ADDR,R0
    // asm 0000A2DC: 	IFI	R0,GT,31,MULTIWORD
    // asm 0000A2DE: 	LDI	*AR0,R0				;get data
    // asm 0000A2DF: 	LSH	BIT_ADDR,R0
    // asm 0000A2E0: 	LDI	CURRENT_CODE_BITS,R1
    // asm 0000A2E1: 	SUBI	32,R1
    // asm 0000A2E2: 	LSH	R1,R0
    // asm 0000A2E3: 	ADDI	CURRENT_CODE_BITS,BIT_ADDR
    // asm 0000A2E4: 	RETS
    // asm 0000A2E5: MULTIWORD
    // asm 0000A2E5: 	LDI	*AR0++,R1
    // asm 0000A2E6: 	LSH	BIT_ADDR,R1		;left justify
    // asm 0000A2E7: 	LDI	32,R0
    // asm 0000A2E8: 	SUBI	BIT_ADDR,R0		;how many bits in 1st word
    // asm 0000A2E9: 	LDI	CURRENT_CODE_BITS,R2
    // asm 0000A2EA: 	SUBI	R0,R2			;bits remaining in second word
    // asm 0000A2EB: 	LDI	R2,BIT_ADDR
    // asm 0000A2EC: 	LDI	*AR0,R0
    // asm 0000A2ED: 	SUBI	32,R2
    // asm 0000A2EE: 	LSH	R2,R0			;second word now is right justified w/proper bits
    // asm 0000A2EF: 	LDI	CURRENT_CODE_BITS,R2
    // asm 0000A2F0: 	SUBI	32,R2
    // asm 0000A2F1: 	LSH	R2,R1
    // asm 0000A2F2: 	OR	R1,R0
    // asm 0000A2F3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INPUT_BITS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PARAMETERS
 *	R0	CHARACTER (BYTE) TO OUTPUT
 *	AR1	DADDR
 *
 *
 */
static void PUTC(void) {
    // asm 0000A2F4: 	LDI	@LINEBUFFERI,AR2
    // asm 0000A2F5: 	ADDI	bufcnt,AR2
    // asm 0000A2F6: 	INC	bufcnt
    // asm 0000A2F7: 	STI	PUTC_BUF,*AR2		;for AR timing
    // asm 0000A2F8: 	CLRI	PUTC_BUF
    // asm 0000A2F9: 	CLRI	PUTC_SH
    // asm 0000A2FA: 	CMPI	64,bufcnt
    // asm 0000A2FB: 	RETSLT
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
    // asm 0000A2FC: 	LDI	@PACIFY_COUNT,R0
    // asm 0000A2FD: 	ADDI	64,R0
    // asm 0000A2FE: 	STI	R0,@PACIFY_COUNT
    // 	;
    // asm 0000A2FF: 	CLRI	bufcnt
    // asm 0000A300: 	PUSH	AR4
    // asm 0000A301: 	LDI	@LINEBUFFERI,AR4
    // asm 0000A302: 	CLRI	AR2			;for dummy read
    // asm 0000A303: 	PUSH	ST			;this push must be here
    // asm 0000A304: 	PUSH	RC
    // asm 0000A305: 	PUSH	RE
    // asm 0000A306: 	PUSH	RS
    // asm 0000A307: 	PUSH	R7
    // asm 0000A308: 	PUSH	IE
    // asm 0000A309: 	LDP	@COMMINTM
    // asm 0000A30A: 	LDI	@COMMINTM,IE
    // asm 0000A30B: 	SETDP
    // ;	PUSH	IE			;disable interrupts
    // ;	LDI	0,IE			;watch for pipeline conflicts
    // asm 0000A30C: 	LDI	HARD_WS,R0
    // asm 0000A30D: 	LDI	SOFT_WS,R1
    // ;	AND	0DFFFh,ST		;turn off GIE.
    // ;	POP	IE
    // asm 0000A30E: 	LDP	@CPU_WS
    // asm 0000A30F: 	STI	R0,@CPU_WS		;set hard wait states
    // asm 0000A310: 	LDI	63,RC
    // asm 0000A311: 	LDI	-16,R7
    // asm 0000A312: 	RPTB	WVWRLP2
    // asm 0000A313: 	LDI	*AR4,R2			;read from the buffer
    // asm 0000A314:     	STI	R2,*AR1++		;write to wave ram
    // asm 0000A315:  	LSH	R7,*AR4++,R2		;read/shift right
WVWRLP2:
    // asm 0000A316: STI	R2,*AR1++		;write to wave ram
    // asm 0000A317: 	LDI	*AR4,R2			;DUMMY READ TO CLEAR THE LINE
    // asm 0000A318: 	STI	R1,@CPU_WS		;set soft wait states
    // asm 0000A319: 	SETDP
    // asm 0000A31A: 	POP	IE
    // asm 0000A31B: 	POP	R7			;this pop sequence must be here
    // asm 0000A31C: 	POP	RS
    // asm 0000A31D: 	POP	RE
    // asm 0000A31E: 	POP	RC
    // asm 0000A31F: 	POP	ST
    // asm 0000A320: 	BUD	ENABLEGIE
    // asm 0000A321: 	POP	AR4
    // asm 0000A322: 	CLRI	PUTC_BUF
    // asm 0000A323: 	CLRI	PUTC_SH
    // 	;---->	BUD	ENABLEGIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PUTC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR4	SADDR
 *	AR5	DADDR
 *
 *
 */
void DECOMPRESS(void) {
    // asm 0000A324: 	CALL	PUSHALL
    // 	;
    // 	;NEW ADDITION.  DONT F*CK THE WAVERAM
    // 	;
    // asm 0000A325: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 0000A32A: 	DMA_WT		R0
    // asm 0000A32F: 	CALL	FIFO_RESET
    // 	;
    // 	;
    // asm 0000A330: 	LDI	1,R0
    // asm 0000A331: 	STPI	R0,@DECOMP_ACTIVE
    // asm 0000A332: 	CLRI	R0
    // asm 0000A333: 	STPI	R0,@FLUSH_COUNT
    // asm 0000A334: 	LDI	AR4,AR0				;SADDR
    // asm 0000A335: 	LDI	AR5,AR1				;DADDR
    // asm 0000A336: 	CLRI	bufcnt
    // asm 0000A337: 	LDI	@DICTI,AR3
    // asm 0000A338: 	LDI	CHARACTER,IR0
    // asm 0000A339: 	CLRI	BIT_ADDR
    // asm 0000A33A: 	CLRI	PUTC_BUF
    // asm 0000A33B: 	CLRI	PUTC_SH
    // asm 0000A33C: 	CALL	SAVE_DECOMP_REGS
    // asm 0000A33D: 	LDI	@HARD_SECTION_LOAD,R0
    // asm 0000A33E: 	BZ	NOHARDLOAD
    // asm 0000A33F: 	CALL	DECOMPRESS_PROC
NOHARDLOAD:
    // asm 0000A340: 	CALL	POPALL
    // asm 0000A341: 	RETS
DECOMPRESS_TOPLP:
    // ;	LDI	@FLUSH_COUNT,R0
    // ;	INC	R0
    // ;	STPI	R0,@FLUSH_COUNT
    // ;	IFI	R0,LT,3,DECOMPRESS_TOPLP3
    // ;	CLRI	R0
    // ;	STPI	R0,@FLUSH_COUNT
    // asm 0000A342: 	CALL	FEED_WATCHDOG
    // 	;
    // asm 0000A343: 	LDI	@PACIFY_COUNT,R0
    // asm 0000A344: 	CMPI	PACIFY_MOMENT,R0
    // asm 0000A345: 	BLT	DECOMPRESS_TOPLP3
    // 	;
    // asm 0000A346: 	LDI	@HARD_SECTION_LOAD,R0
    // asm 0000A347: 	BZ	CONT
    // asm 0000A348: 	LDI	@BOOT_PACIFY_SCREEN_P,R0
    // asm 0000A349: 	CALLNZ	BOOT_PACIFY_SCREEN
    // asm 0000A34A: 	BU	DECOMPRESS_TOPLP3
CONT:
    // asm 0000A34B: 	CALL	SAVE_DECOMP_REGS
    // asm 0000A34C: 	CALL	POPALL
    // ;	CALL	ENABLEGIE
    // asm 0000A34D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECOMPRESS", 0, 0);
    UNIMPL();
}

void DECOMPRESS_PROC(void) {
    // asm 0000A34E: 	LDI	@DECOMP_ACTIVE,R0
    // asm 0000A34F: 	RETSZ
    // 	;
    // 	;NEW ADDITION.  DONT F*CK THE WAVERAM
    // 	;
    // asm 0000A350: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 0000A355: 	DMA_WT		R0
    // asm 0000A35A: 	CALL	FIFO_RESET
    // 	;
    // 	;
    // 	;PACIFIER
    // asm 0000A35B: 	CLRI	R0
    // asm 0000A35C: 	STPI	R0,@PACIFY_COUNT
    // 	;
    // ;	PUSH	IE			;disable interrupts
    // ;	LDI	0,IE			;watch for pipeline conflicts
    // ;	LDI	HARD_WS,R0
    // ;	LDI	SOFT_WS,R1
    // ;	AND	0DFFFh,ST		;turn off GIE.
    // ;	POP	IE
    // asm 0000A35D: DECOMPRESS_TOPLP2
    // asm 0000A35D: 	CALL	PUSHALL
    // asm 0000A35E: 	CALL	RESTORE_DECOMP_REGS
    // ;	CALL	FEED_WATCHDOG
DECOMPRESS_TOPLP3:
    // asm 0000A35F: 	LDI	FIRST_CODE,NEXT_CODE
    // asm 0000A360: 	LDI	9,CURRENT_CODE_BITS
    // asm 0000A361: 	LDI	511,R0
    // asm 0000A362: 	STPI	R0,@NEXT_BUMP_CODE
    // asm 0000A363: 	CALL	INPUT_BITS		;returns in R0
    // asm 0000A364: 	LDI	R0,old_code		;old_code = INPUTBITS()
    // asm 0000A365: 	IFI	old_code,EQ,END_OF_STREAM,DECOMPRESSX
    // asm 0000A367: 	LDI	old_code,character	;character = old_code
    // 	;redun	LDI	old_code,R0
    // asm 0000A368: 	LSH	PUTC_SH,R0		;this is inlined!
    // asm 0000A369: 	OR	R0,PUTC_BUF		;
    // asm 0000A36A: 	ADDI	8,PUTC_SH		;
    // asm 0000A36B: 	CMPI	32,PUTC_SH		;
    // asm 0000A36C: 	CALLGE	PUTC
    // ;	CALL	PUTC			;
DECOMPRESSLP:
    // asm 0000A36D: 	CALL	INPUT_BITS
    // asm 0000A36E: 	LDI	retval,new_code
    // asm 0000A36F: 	CMPI	END_OF_STREAM,new_code
    // asm 0000A370: 	BEQ	DECOMPRESSX
    // asm 0000A371: 	CMPI	FLUSH_CODE,new_code
    // asm 0000A372: 	BEQ	DECOMPRESS_TOPLP
    // asm 0000A373: 	CMPI	BUMP_CODE,new_code
    // asm 0000A374: 	BNE	NOBUMP
    // asm 0000A375: 	INC	CURRENT_CODE_BITS
    // asm 0000A376: 	BU	DECOMPRESSLP
NOBUMP:
    // asm 0000A377: 	CMPI	NEXT_CODE,new_code
    // asm 0000A378: 	BLTD	NODS
    // asm 0000A379: 	LDP	@DECODE_STACKI
    // asm 0000A37A: 	LDI	@DECODE_STACKI,AR4
    // asm 0000A37B: 	CLRI	count
    // ;	NOP
    // 	;---->	BLTD	NODS
    // asm 0000A37C: 	BUD	NODSRT
    // asm 0000A37D: 	STI	character,*AR4++		;decode_stack[0] = character
    // asm 0000A37E: 	LDI	1,count				;count
    // asm 0000A37F: 	LDI	old_code,AR5			;code
    // 	;---->	BUD	NODSRT
NODS:
    // ;	CLRI	count
    // asm 0000A380: 	LDI	new_code,AR5
NODSRT:
    // asm 0000A381: 	IFI	AR5,LE,255,DECODEL1i		;while( code > 255)  {
DECODEL3i:
    // asm 0000A383: 	ADDI	AR3,AR5
    // asm 0000A384: 	LDI	*+AR5(IR0),R0			;CHARACTER
    // asm 0000A385: 	STI	R0,*AR4++			;decode_stack[ count++] = DICT[ code].character
    // asm 0000A386: 	INC	count				;must keep counter correct
    // asm 0000A387: 	LDI	*+AR5(PARENT_CODE),AR5		;code = DICT[code].parent_code
    // asm 0000A388: 	CMPI	255,AR5
    // asm 0000A389: 	BGT	DECODEL3i
    // asm 0000A38A: DECODEL1i
    // asm 0000A38A: 	LDI	AR5,character
    // asm 0000A38B: 	LDI	AR5,R0				;setup the first write
    // asm 0000A38C: 	RPTB	BLOOPER
    // asm 0000A38D: 	LSH	PUTC_SH,R0		;this is inlined!
    // asm 0000A38E: 	OR	R0,PUTC_BUF		;
    // asm 0000A38F: 	ADDI	8,PUTC_SH		;
    // asm 0000A390: 	CMPI	32,PUTC_SH		;
    // asm 0000A391: 	CALLGE	PUTC
    // 	;CALL	PUTC
BLOOPER:
    // asm 0000A392: LDI	*--AR4,R0			;and this becomes a pre-decrement
    // asm 0000A393: 	ADDI	NEXT_CODE,AR3,AR2
    // asm 0000A394: 	STI	old_code,*+AR2(PARENT_CODE)	;DICT[next_code].parent = old_code
    // asm 0000A395: 	BUD	DECOMPRESSLP
    // asm 0000A396: 	STI	character,*+AR2(IR0)		;DICT[next_code].character = character
    // asm 0000A397: 	INC	NEXT_CODE
    // asm 0000A398: 	LDI	new_code,old_code
    // 	;---->	BUD	DECOMPRESSLP
DECOMPRESSX:
    // asm 0000A399: 	CLRI	R0
    // asm 0000A39A: 	STPI	R0,@DECOMP_ACTIVE
    // asm 0000A39B: 	STPI	R0,@HARD_SECTION_LOAD
    // asm 0000A39C: 	CALL	POPALL
    // asm 0000A39D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECOMPRESS_PROC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: SAVESPCI	.word	SAVESPC+1 */
static uintptr_t SAVESPCI = (uintptr_t)(SAVESPC + 1);
/* asm: SAVESPC	.bss	SAVESPC,25 */
int SAVESPC[25];

// *----------------------------------------------------------------------------
static void SAVE_DECOMP_REGS(void) {
    // asm 0000A39F: 	LDP	@SAVESPC
    // asm 0000A3A0: 	STI	AR0,@SAVESPC
    // asm 0000A3A1: 	LDI	@SAVESPCI,AR0
    // asm 0000A3A2: 	STI	AR1,*AR0++
    // asm 0000A3A3: 	STI	AR2,*AR0++
    // asm 0000A3A4: 	STI	AR3,*AR0++
    // asm 0000A3A5: 	STI	AR4,*AR0++
    // asm 0000A3A6: 	STI	AR5,*AR0++
    // asm 0000A3A7: 	STI	AR6,*AR0++
    // asm 0000A3A8: 	STI	AR7,*AR0++
    // asm 0000A3A9: 	STI	R1,*AR0++
    // asm 0000A3AA: 	STI	R2,*AR0++
    // asm 0000A3AB: 	STI	R3,*AR0++
    // asm 0000A3AC: 	STI	R4,*AR0++
    // asm 0000A3AD: 	STI	R5,*AR0++
    // asm 0000A3AE: 	STI	R6,*AR0++
    // asm 0000A3AF: 	STI	R7,*AR0++
    // asm 0000A3B0: 	STI	RC,*AR0++
    // asm 0000A3B1: 	STI	RE,*AR0++
    // asm 0000A3B2: 	STI	RS,*AR0++
    // asm 0000A3B3: 	STI	IR0,*AR0++
    // asm 0000A3B4: 	STI	IR1,*AR0++
    // asm 0000A3B5: 	STI	BK,*AR0++
    // asm 0000A3B6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SAVE_DECOMP_REGS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void RESTORE_DECOMP_REGS(void) {
    // asm 0000A3B7: 	LDI	@SAVESPCI,AR0
    // asm 0000A3B8: 	LDI	*AR0++,AR1
    // asm 0000A3B9: 	LDI	*AR0++,AR2
    // asm 0000A3BA: 	LDI	*AR0++,AR3
    // asm 0000A3BB: 	LDI	*AR0++,AR4
    // asm 0000A3BC: 	LDI	*AR0++,AR5
    // asm 0000A3BD: 	LDI	*AR0++,AR6
    // asm 0000A3BE: 	LDI	*AR0++,AR7
    // asm 0000A3BF: 	LDI	*AR0++,R1
    // asm 0000A3C0: 	LDI	*AR0++,R2
    // asm 0000A3C1: 	LDI	*AR0++,R3
    // asm 0000A3C2: 	LDI	*AR0++,R4
    // asm 0000A3C3: 	LDI	*AR0++,R5
    // asm 0000A3C4: 	LDI	*AR0++,R6
    // asm 0000A3C5: 	LDI	*AR0++,R7
    // asm 0000A3C6: 	LDI	*AR0++,RC
    // asm 0000A3C7: 	LDI	*AR0++,RE
    // asm 0000A3C8: 	LDI	*AR0++,RS
    // asm 0000A3C9: 	LDI	*AR0++,IR0
    // asm 0000A3CA: 	LDI	*AR0++,IR1
    // asm 0000A3CB: 	LDI	*AR0++,BK
    // asm 0000A3CC: 	LDP	@SAVESPC
    // asm 0000A3CD: 	LDI	@SAVESPC,AR0
    // asm 0000A3CE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESTORE_DECOMP_REGS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

#define MIN_X 240 // if this changes modify CUSA.ASM
#define MAX_X 300
/* asm: BOOT_PACIFY_SCREEN_P	.word	1 */
/* asm: 	 */
int BOOT_PACIFY_SCREEN_P = 1;
/* asm: PREVX	.bss	PREVX,1 */
int PREVX;
/* asm: DELTA	.bss	DELTA,1 */
int DELTA;
/*
;PREVX		.word	MIN_X
;DELTA		.word	1
*/

// *----------------------------------------------------------------------------
static void BOOT_PACIFY_SCREEN(void) {
    // asm 0000A3D0: 	CALL	SAVE_DECOMP_REGS
    // asm 0000A3D1: 	LDI	@PREVX,R6
    // asm 0000A3D2: 	LDI	R6,AR2
    // asm 0000A3D3: 	LDI	R6,R3
    // asm 0000A3D4: 	LDI	111,R2
    // asm 0000A3D5: 	LDI	116,RC
    // ;	LDI	@DELTA,R0
    // ;	LDILT	0,RS
    // ;	LDIGT	11,RS
    // asm 0000A3D6: 	LDI	0,RS
    // asm 0000A3D7: 	CALL	_line
    // asm 0000A3D8: 	LDI	@PREVX,R6
    // asm 0000A3D9: 	LDI	@DELTA,R7
    // asm 0000A3DA: 	ADDI	R7,R6
    // asm 0000A3DB: 	STI	R6,@PREVX
    // asm 0000A3DC: 	LDI	R6,AR2
    // asm 0000A3DD: 	LDI	R6,R3
    // asm 0000A3DE: 	LDI	111,R2
    // asm 0000A3DF: 	LDI	116,RC
    // asm 0000A3E0: 	LDI	3,RS
    // asm 0000A3E1: 	CALL	_line
    // asm 0000A3E2: 	LDI	@PREVX,R6
    // asm 0000A3E3: 	CMPI	MIN_X,R6
    // asm 0000A3E4: 	BGT	LL
    // asm 0000A3E5: 	LDI	1,R7
    // asm 0000A3E6: 	STPI	R7,@DELTA
LL:
    // asm 0000A3E7: 	CMPI	MAX_X,R6
    // asm 0000A3E8: 	BLT	LLL
    // asm 0000A3E9: 	LDI	-1,R7
    // asm 0000A3EA: 	STPI	R7,@DELTA
LLL:
    // asm 0000A3EB: 	CALL	RESTORE_DECOMP_REGS
    // asm 0000A3EC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOOT_PACIFY_SCREEN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SECTION LOAD REQUEST
 *	IF ACTIVE THEN REQUEST IS QUEUED BY CREATING A PROCESS
 *
 *
 *PARAMETERS
 *	AR2	POINTER TO SECTION CONTROL
 *
 */
/* asm: LASTLOAD	.bss	LASTLOAD,1 */
static int LASTLOAD;

void LOAD_SECTION_REQ(void) {
    // asm 0000A3ED: 	PUSH	AR4
    // asm 0000A3EE: 	PUSH	AR5
    // asm 0000A3EF: 	LDI	@DECOMP_ACTIVE,R0
    // asm 0000A3F0: 	BZ	NOWTLD
    // 	;a decompression is executing, queue the request
    // 	;
    // 	;
    // asm 0000A3F1: 	PUSH	R2
    // asm 0000A3F2: 	PUSH	AR0
    // asm 0000A3F3: 	PUSH	AR4
    // asm 0000A3F4: 	LDI	AR2,AR4
    // asm 0000A3F5: 	CREATE	REQWAIT,SPAWNER_C|LOAD_REQ_T
    // asm 0000A3F8: 	POP	AR4
    // asm 0000A3F9: 	POP	AR0
    // asm 0000A3FA: 	POP	R2
    // asm 0000A3FB: 	BU	NOLOAD
NOWTLD:
    // ;	LDI	@LASTLOAD,AR0
    // ;	CMPI	AR0,AR2
    // ;	BEQ	NOLOAD
    // ;	STI	AR2,@LASTLOAD
    // asm 0000A3FC: 	LDI	*AR2++,AR4
    // asm 0000A3FD: 	LDI	*AR2++,AR5
    // asm 0000A3FE: 	CALL	DECOMPRESS
NOLOAD:
    // asm 0000A3FF: 	POP	AR5
    // asm 0000A400: 	POP	AR4
    // asm 0000A401: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SECTION_REQ", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void REQWAIT(void) {
    // asm 0000A402: 	SLEEP	1
    // asm 0000A404: 	LDI	@DECOMP_ACTIVE,R0
    // asm 0000A405: 	BNZ	REQWAIT
    // asm 0000A406: 	LDI	AR4,AR2
    // asm 0000A407: 	CALL	LOAD_SECTION_REQ
    // asm 0000A408: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REQWAIT", 0, 0);
    UNIMPL();
}

#include "../core/cpu.h"
#include "../core/machine.h"
#include "mproc.h"
#include "c30.h"
#include "macs.h"
#include "obj.h"
#include "objects.h"
#include "globals.h"
#include "sys.h"
#include "text.h"
#include "vunit.h"
#include "cmos.h"
#include "sndtab.h"
#include "pall.h"
#include "sysid.h"
#include "dirq.h"
#include "delta.h"
#include "port.h"
#include "util.h"

/*
 * Source module: asm/UTIL.ASM
 */

void TVBP(void);
void TVBPX(void);
void SETPAGE0(void);
void SETPAGE1(void);
#define FASTCLR0 FASTCLR1
void FASTCLR1(void);
void CLRSCRN(void);
void CLRSCRN1(void);
void CLRSCRN0(void);
void CLR255(void);
void CLR511(void);
void SCRNFIL(void);
void SCREEN_FILL(void);
void CLRCRAM(void);
void RANDOM(void);
void FRAND(void);
void SFRAND(void);
void RANDU0(void);
void RANDU(void);
void SRAND(void);
void RANDPER(void);
void INIT_LINKED_LIST(void);
void GET_LLIST(void);
void ALLOC_LLIST(void);
void FREE_LLIST(void);
void DEL_LLIST(void);
void VEHICLE_ANI_INIT(void);
void CARPROC(void);
void LEAN(void);
void DYNAOBJ_INIT(void);
void GETDYNA(void);
void DELDYNA(void);
void CARB_INIT(void);
void GETCAR(void);
void DELCAR(void);
void SCAN_OBJECTS(void);
void PUSHALL(void);
void POPALL(void);
void DISTANCE_2D(void);
void OVELADD(void);
void OVELNADD(void);
void FORWARD(void);

extern int RAND;
extern int CRTCTLRAM;
extern int ACTIVE_SCREEN;
extern int LINE255I;
extern int LINE511I;
extern int SCRSIZI;
extern float NTWOPII;
extern int DYNALIST[];
extern int DYNAFREE;
extern int NULL;
extern int CARLIST[];
extern int CARFREE;
extern int CAR_COUNT;

/* asm: RAND	pbss	RAND,1 */
int RAND;
/* asm: CRTCTLRAM	.bss	CRTCTLRAM,1 */
int CRTCTLRAM;
/* asm: ACTIVE_SCREEN	.bss	ACTIVE_SCREEN,1 */
int ACTIVE_SCREEN;
/* *----------------------------------------------------------------------------
*ROM DEFINITIONS
*
 */
/* asm: LINE255I	.word	SCREEN0+3F000H 	;LAST LINE PAGE 0 */
int LINE255I = SCREEN0+0x3F000;
/* asm: LINE511I	.word	SCREEN0+7FC00H	;LAST LINE PAGE 1 */
int LINE511I = SCREEN0+0x7FC00;
/* asm: SCRSIZI	.word	3FFFFH */
int SCRSIZI = 0x3FFFF;
#if DEBUG

void TVBP(void)
{
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TVBP", 0, 0);
    UNIMPL();
}

void TVBPX(void)
{
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TVBPX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
#endif

/* *----------------------------------------------------------------------------
*SET SCREEN DISPLAY TO PAGE 0  (AND WRITE PAGE TO 1)
*
 */
void SETPAGE0(void)
{
    // ;	.if	DEBUG
    // asm 00008E76: 	LDI	@PAGEWORD,R0
    // asm 00008E77: 	BNE	P1
    // ;	.endif
    // asm 00008E78: 	LDI	@SCREEN1I,R0		;set active screen to 1 (writeable)
    // asm 00008E79: 	STI	R0,@ACTIVE_SCREEN
    // asm 00008E7A: 	LDP	@DMA_SETUP
    // asm 00008E7B: 	LDI	@DMA_SETUP,R0
    // asm 00008E7C: 	ANDN	DMA_VIDEO_PAG_DISPLAYED,R0
    // asm 00008E7D: 	OR	DMA_DMA_WRITE_PAGE,R0
    // asm 00008E7E: 	STI	R0,@DMA_SETUP
    // asm 00008E7F: 	SETDP
    // asm 00008E80: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETPAGE0", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SET SCREEN DISPLAY TO PAGE 1	(AND WRITE PAGE TO 0)
*
 */
void SETPAGE1(void)
{
    // ;	.if	DEBUG
    // asm 00008E81: 	LDI	@PAGEWORD,R0
    // asm 00008E82: 	BNE	P1
    // ;	.endif
    // asm 00008E83: 	LDI	@SCREEN0I,R0		;set active screen to 1 (writeable)
    // asm 00008E84: 	STI	R0,@ACTIVE_SCREEN
    // asm 00008E85: 	LDP	@DMA_SETUP
    // asm 00008E86: 	LDI	@DMA_SETUP,R0
    // asm 00008E87: 	OR	DMA_VIDEO_PAG_DISPLAYED,R0
    // asm 00008E88: 	ANDN	DMA_DMA_WRITE_PAGE,R0
    // asm 00008E89: 	STI	R0,@DMA_SETUP
    // asm 00008E8A: 	SETDP
    // asm 00008E8B: 	RETS
    // ;	.if	DEBUG
P1:
    // asm 00008E8D: 	LDI	@SCREEN1I,R0		;set active screen to 1 (writeable)
    // asm 00008E8E: 	STI	R0,@ACTIVE_SCREEN
    // asm 00008E8F: 	LDP	@DMA_SETUP
    // asm 00008E90: 	LDI	@DMA_SETUP,R0
    // asm 00008E91: 	OR	DMA_VIDEO_PAG_DISPLAYED+DMA_DMA_WRITE_PAGE,R0
    // asm 00008E92: 	STI	R0,@DMA_SETUP
    // asm 00008E93: 	SETDP
    // asm 00008E94: 	RETS
    // ;	.endif
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETPAGE1", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SRT PAGE ONE
*
*CLEAR SCREEN FAST SHIFT REGISTER TRANSFER
*CALL ONLY DURING VBLANK
*
 */
void FASTCLR1(void)
{
    // asm 00008E95: 	LDI	@NOAERASE,R0
    // asm 00008E96: 	RETSNZ
    // asm 00008E97: 	LDI	0,AR2		;X
    // asm 00008E98: 	STI	AR2,@_ACNTL
    // asm 00008E99: 	LDI	0,R2		;Y
    // asm 00008E9A: 	LDI	511,R3		;X2
    // asm 00008E9B: 	LDI	399,RC		;Y2
    // asm 00008E9C: 	LDI	0,RS		;PAL
    // asm 00008E9D: 	LDI	0A0h,RE		;ADDR
    // asm 00008E9E: 	RS	16,RE
    // asm 00008E9F: 	CALL	_rdma
    // asm 00008EA0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FASTCLR1", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CLEAR SCREEN BITMAP
*
 */
void CLRSCRN(void)
{
    // asm 00008EA1: 	CALL	CLRSCRN0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRSCRN", 0, 0);
    UNIMPL();
}

void CLRSCRN1(void)
{
    // asm 00008EA2: 	PUSH	AR2
    // asm 00008EA3: 	LDI	@SCREEN1I,AR2
    // asm 00008EA4: 	B	CLRSC00
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRSCRN1", 0, 0);
    UNIMPL();
}

void CLRSCRN0(void)
{
    // asm 00008EA5: 	PUSH	AR2
    // asm 00008EA6: 	LDI	@SCREEN0I,AR2
    // asm 00008EA7: CLRSC00	PUSH	R3
    // asm 00008EA8: 	LDI	@SCRSIZI,R3
CLRSC01:
    // asm 00008EA9: 	PUSH	R2
    // asm 00008EAA: 	LDI	0,R2
    // asm 00008EAB: 	CALL	SCREEN_FILL
    // asm 00008EAC: 	POP	R2
    // asm 00008EAD: 	POP	R3
    // asm 00008EAE: 	POP	AR2
    // asm 00008EAF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRSCRN0", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CLEAR LINE 255,255 OF BITMAP
*
 */
void CLR255(void)
{
    // asm 00008EB0: 	PUSH	AR2
    // asm 00008EB1: 	PUSH	R3
    // asm 00008EB2: 	LDI	@LINE255I,AR2
    // asm 00008EB3: 	LDI	1023,R3		;ONE ROW ONLY
    // asm 00008EB4: 	B	CLRSC01
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR255", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CLEAR LINE 511,511 OF BITMAP
*
 */
void CLR511(void)
{
    // asm 00008EB5: 	PUSH	AR2
    // asm 00008EB6: 	PUSH	R3
    // asm 00008EB7: 	LDI	@LINE511I,AR2
    // asm 00008EB8: 	LDI	1023,R3		;ONE ROW ONLY
    // asm 00008EB9: 	B	CLRSC01
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR511", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*FILL SCREEN
 */
void SCRNFIL(void)
{
    // asm 00008EBA: 	LDI	@SCREEN0I,AR2
    // asm 00008EBB: 	LDI	@FILSIZI,R3
    // asm 00008EBC: 	LDI	@FILWORD,R2	;fill it with some crud
    // asm 00008EBD: 	B	SCREEN_FILL
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCRNFIL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SCREEN WRITER
*
*PARAMETERS
*	AR2	START ADDRESS
*	R2	COLOR
*	R3	COUNT-1
*
 */
void SCREEN_FILL(void)
{
    // asm 00008EC0: 	PUSH	R0
    // asm 00008EC1: 	PUSH	AR1
    // asm 00008EC2: 	PUSH	AR2
    // asm 00008EC3: 	PUSH	IE
    // asm 00008EC4: 	LDP	@COMMINTM
    // asm 00008EC5: 	LDI	@COMMINTM,IE
    // asm 00008EC6: 	SETDP
    // asm 00008EC7: 	PUSH	DP
    // asm 00008EC8: 	LDP	@CPU_WS
    // asm 00008EC9: 	LDI	0,AR1
    // asm 00008ECA: 	LDI	R3,RC
    // asm 00008ECB: 	LDI	HARD_WS,R0
    // asm 00008ECC: 	STI	R0,@CPU_WS
    // asm 00008ECD: 	RPTB	CLRSCL
    // asm 00008ECE: 	STI	R2,*AR2++
    // asm 00008ECF: CLRSCL	LDI	*AR1,R0		;DUMMY READ FOR WAIT STATE SHIT
    // asm 00008ED0: 	LDP	@CPU_WS
    // asm 00008ED1: 	LDI	SOFT_WS,R0
    // asm 00008ED2: 	STI	R0,@CPU_WS
    // asm 00008ED3: 	POP	DP
    // asm 00008ED4: 	POP	IE
    // asm 00008ED5: 	POP	AR2
    // asm 00008ED6: 	POP	AR1
    // asm 00008ED7: 	POP	R0
    // asm 00008ED8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREEN_FILL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CLEAR COLOR RAM
*
 */
void CLRCRAM(void)
{
    // asm 00008ED9: 	LDI	COLORAM>>16,AR0
    // asm 00008EDA: 	LSH	16,AR0
    // asm 00008EDB: 	LDI	0,R1
    // asm 00008EDC: 	RPTS	07FFFH
    // asm 00008EDD: 	STI	R1,*AR0++
    // asm 00008EDE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRCRAM", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*RANDOM NUMBER UTILITIES
*----------------------------------------------------------------------------
*RANDOM - GENERATE A RANDOM NUMBER
*RETURNS
*	R0	32 BIT RANDOM #
*
 */
void RANDOM(void)
{
    // asm 00008EDF: 	PUSH	R1
    // asm 00008EE0: 	LDI	@RAND,R0
    // asm 00008EE1: 	LDI	R0,R1
    // asm 00008EE2: 	LSH	1,R0
    // asm 00008EE3: 	XOR	R0,R1
    // asm 00008EE4: 	BNN	RND2
    // asm 00008EE5: 	OR	1,R0
RND2:
    // asm 00008EE6: POP	R1
    // asm 00008EE7: 	MPYI	794Fh,R0
    // asm 00008EE8: 	STI	R0,@RAND
    // asm 00008EE9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDOM", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*FLOATING POINT RANDOM
*
*PARAMETERS
*	R0	NUMBER
*RETURNS
*	R0	RANDOM NUMBER 0->N FLOATING POINT
*
 */
void FRAND(void)
{
    // asm 00008EEA: 	PUSH	AR2
    // asm 00008EEB: 	PUSHFL	R1
    // asm 00008EED: 	PUSHF	R0
    // asm 00008EEE: 	LDI	10000,AR2
    // asm 00008EEF: 	CALL	RANDU0
    // asm 00008EF0: 	FLOAT	R0
    // asm 00008EF1: 	MPYF	0.01,R0
    // asm 00008EF2: 	MPYF	0.01,R0
    // asm 00008EF3: 	POPF	R1
    // asm 00008EF4: 	MPYF	R1,R0
    // asm 00008EF5: 	POPFL	R1
    // asm 00008EF7: 	POP	AR2
    // asm 00008EF8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FRAND", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SIGNED FLOATING POINT RANDOM
*
*PARAMETERS
*	R0	FL NUMBER
*RETURNS
*	R0	FL RANDOM NUMBER -N->+N FLOATING POINT
*
 */
void SFRAND(void)
{
    // asm 00008EF9: 	PUSH	R1
    // asm 00008EFA: 	PUSHF	R1
    // asm 00008EFB: 	PUSHF	R0
    // asm 00008EFC: 	MPYF	2,R0
    // asm 00008EFD: 	CALL	FRAND
    // asm 00008EFE: 	POPF	R1
    // asm 00008EFF: 	SUBF	R1,R0
    // asm 00008F00: 	POPF	R1
    // asm 00008F01: 	POP	R1
    // asm 00008F02: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SFRAND", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*RANDOM UNSIGNED 0 TO N-1
*
*PARAMETERS
*	AR2	N  RANGE INPUT (0-FFFF)
*RETURNS
*	R0	RANDOM # BETWEEN 0 AND [AR2]
*
 */
void RANDU0(void)
{
    // asm 00008F03: 	CALL	RANDOM
    // asm 00008F04: 	LSH	-16,R0
    // asm 00008F05: 	MPYI	AR2,R0
    // asm 00008F06: 	LSH	-16,R0
    // asm 00008F07: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDU0", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*RANDOM UNSIGNED
*
*PARAMETERS
*	AR2	N
*RETURNS
*	R0	RANDOM # BETWEEN 1 AND N
 */
void RANDU(void)
{
    // asm 00008F08: 	CALL	RANDU0
    // asm 00008F09: 	ADDI	1,R0
    // asm 00008F0A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDU", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SIGNED RANDOM
*
*PARAMETERS
*	AR2	N
*RETURNS
*	R0	RANDOM # IN RANDGE +/- N
*
 */
void SRAND(void)
{
    // asm 00008F0B: 	LSH	1,AR2
    // asm 00008F0C: 	ADDI	1,AR2
    // asm 00008F0D: 	CALL	RANDU0
    // asm 00008F0E: 	LSH	-1,AR2
    // asm 00008F0F: 	SUBI	AR2,R0
    // asm 00008F10: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SRAND", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*RANDOM % ROUTINE
*
*PARAMETERS
*	AR2	PROBABILITY OF EVENT (0-1000) P(A0=1000) = 1; P(A0=1) = 1/1000.
*RETURNS
*	C=1	IF PROBABILITY IS TRUE,
*		R0	ACTUAL RANDOM # 0-999
*	C=0	FOR FALSE
*		R0	ZERO
 */
void RANDPER(void)
{
    // asm 00008F11: 	CALL	RANDOM
    // asm 00008F12: 	LSH	-16,R0
    // asm 00008F13: 	MPYI	1000,R0
    // asm 00008F14: 	LSH	-16,R0
    // asm 00008F15: 	CMPI	AR2,R0
    // asm 00008F16: 	BC	RANDPX
    // asm 00008F17: 	LDI	0,R0
RANDPX:
    // asm 00008F18: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDPER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GENERIC LINKED LIST ROUTINES
*These routines are for use with any linked list situation that do not
*require special attention (such as the object list).
*
*----------------------------------------------------------------------------
*void	INIT_LINKED_LIST(start_addr, free_list, active_list, length-1, size)
*
*PARAMETERS
*	AR2	START ADDRESS
*	R2	FREE LIST
*	R3	ACTIVE LIST
*	RC	LENGTH-1
*	RS	ENTRY SIZE
*
*
 */
void INIT_LINKED_LIST(void)
{
    // asm 00008F19: 	PUSH	R0
    // asm 00008F1A: 	PUSH	AR0
    // asm 00008F1B: 	LDI	R3,AR0		;ZERO ACTIVE POINTER
    // asm 00008F1C: 	LDI	0,R0
    // asm 00008F1D: 	STI	R0,*AR0
    // asm 00008F1E: 	LDI	R2,AR0	 	;GET FREE POINTER
    // asm 00008F1F: 	LDI	RS,R0		;SAVE THE SIZE
    // asm 00008F20: 	RPTB	INIT_LL
    // asm 00008F21: 	STI	AR2,*AR0
    // asm 00008F22: 	LDI	AR2,AR0
INIT_LL:
    // asm 00008F23: ADDI	R0,AR2
    // asm 00008F24: 	LDI	0,R0
    // asm 00008F25: 	STI	R0,*AR0
    // asm 00008F26: 	POP	AR0
    // asm 00008F27: 	POP	R0
    // asm 00008F28: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LINKED_LIST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GET_LLIST(free_list,active_list)
*GET A LINKED LIST ELEMENT AND INSERT ON LIST
*
*
*PARAMETERS
*	AR2	FREELIST PTR
*	R2	ACTIVE LIST PTR
*
*RETURNS
*	(SUCCESSFUL)
*		AR0	LIST ELEMENT (INSERTED INTO ACTIVE LIST)
*		R0	LIST ELEMENT (INSERTED INTO ACTIVE LIST)
*		CARRY SET
*	(UNSUCCESSFUL)
*		CARRY CLEAR
*
 */
void GET_LLIST(void)
{
    // asm 00008F29: 	PUSH	R1
    // asm 00008F2A: 	PUSH	AR1
    // asm 00008F2B: 	LDI	*AR2,R0
    // asm: 	SLOCKON	Z,"GET_LLIST  out of elements"
    // asm 00008F2C: 	BZ	GETLL_ERR
    // asm 00008F2D: 	LDI	R0,AR0
    // asm 00008F2E: 	LDI	*AR0,AR0
    // asm 00008F2F: 	STI	AR0,*AR2		;and update free list
    // 	;insert into the active list
    // asm 00008F30: 	LDI	R2,AR1			;get ptr to active
    // asm 00008F31: 	LDI	R0,AR0			;get ptr to element
    // asm 00008F32: 	LDI	*AR1,R1			;get 1st element in active
    // asm 00008F33: 	STI	R1,*AR0			;link element into element
    // asm 00008F34: 	STI	AR0,*AR1		;store element into active
    // asm 00008F35: 	SETC
GETLL_X:
    // asm 00008F36: 	POP	AR1
    // asm 00008F37: 	POP	R1
    // asm 00008F38: 	RETS
GETLL_ERR:
    // asm 00008F39: 	CLRC
    // asm 00008F3A: 	BU	GETLL_X
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_LLIST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ALLOC_LLIST(free_list)
*GET A LINKED LIST ELEMENT (DO NOT INSERT TO ACTIVE)
*
*PARAMETERS
*	AR2	FREE LIST
*RETURNS
*	AR0	LIST ELEMENT (NOT INSERTED INTO ACTIVE LIST)
*
 */
void ALLOC_LLIST(void)
{
    // asm 00008F3B: 	PUSH	R0
    // asm 00008F3C: 	LDI	*AR2,R0
    // asm: 	SLOCKON	Z,"_allocllist  out of elements"
    // asm 00008F3D: 	BZ	ALLOCLIST_ISERROR
    // asm 00008F3E: 	LDI	R0,AR0
    // asm 00008F3F: 	LDI	*AR0,AR0
    // asm 00008F40: 	STI	AR0,*AR2		;and update free list
    // asm 00008F41: 	LDI	R0,AR0
    // asm 00008F42: 	SETC
ALLOCLIST_X:
    // asm 00008F43: 	POP	R0
    // asm 00008F44: 	RETS
ALLOCLIST_ISERROR:
    // asm 00008F45: 	CLRC
    // asm 00008F46: 	BU	ALLOCLIST_X
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ALLOC_LLIST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*FREE_LLIST(element, active, free)
*FREE AN ELEMENT OF A LINKED LIST
*
*PARAMETERS
*	AR2	OBJECT
*	R2	free list
*
*
 */
void FREE_LLIST(void)
{
    // asm 00008F47: 	PUSH	R2
    // asm 00008F48: 	PUSH	AR0
    // asm 00008F49: 	LDI	R2,AR0		;free
    // asm 00008F4A: 	LDI	*AR0,R2
    // asm 00008F4B: 	STI	R2,*AR2
    // asm 00008F4C: 	STI	AR2,*AR0
    // asm 00008F4D: 	LDI	AR0,R2
    // asm 00008F4E: 	POP	AR0
    // asm 00008F4F: 	POP	R2
    // asm 00008F50: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREE_LLIST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*DEL_LLIST(element, active, free)
*DELETE AN ELEMENT OF A LINKED LIST
*
*PARAMETERS
*	AR2	OBJECT
*	R2	ACTIVE LIST
*	R3	FREE LIST
*
 */
void DEL_LLIST(void)
{
    // asm 00008F51: 	PUSH	R0
    // asm 00008F52: 	PUSH	R1
    // asm 00008F53: 	PUSH	AR0
    // asm 00008F54: 	PUSH	AR1
    // asm 00008F55: 	LDI	R2,R1
DELLP:
    // asm 00008F56: LDI	R1,AR1
    // asm 00008F57: 	LDI	*AR1,R1
    // asm: 	SLOCKON	Z,"DEL_LLIST  end of list found"
    // asm 00008F58: 	BZ	DEL_LLX
    // asm 00008F59: 	CMPI	R1,AR2
    // asm 00008F5A: 	BNE	DELLP
    // asm 00008F5B: 	LDI	*AR2,R1
    // asm 00008F5C: 	STI	R1,*AR1			;LINK AROUND
    // asm 00008F5D: 	LDI	R3,AR1			;get free list pointer
    // asm 00008F5E: 	LDI	*AR1,R1
    // asm 00008F5F: 	STI	R1,*AR2
    // asm 00008F60: 	STI	AR2,*AR1
DEL_LLX:
    // asm 00008F61: 	POP	AR1
    // asm 00008F62: 	POP	AR0
    // asm 00008F63: 	POP	R1
    // asm 00008F64: 	POP	R0
    // asm 00008F65: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEL_LLIST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*VEHICLE ANIMATION UTILITIES
*
*----------------------------------------------------------------------------
*
*GET DYNAMIC OBJECTS FOR A CAR
*
*PARAMETERS
*	AR2	INDEX OF VEHICLE
*	AR4	CAR OBJECT
*
 */
void VEHICLE_ANI_INIT(void)
{
    // asm 00008F66: 	PUSH	AR0
    // asm 00008F67: 	PUSH	AR3
    // asm 00008F68: 	MPYI	VEHTAB_SIZE,AR2
    // asm 00008F69: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 00008F6A: 	LDI	*+AR2(VEHTAB_ANI),AR2
    // asm 00008F6B: 	CMPI	0,AR2			;COULD BE A NULL ENTRY (NO ANIMATION)
    // asm 00008F6C: 	BEQ	VANIX
    // asm 00008F6D: 	LDI	O_DYNAMIC,R0	 	;MAKE PARENT OBJECT DYNAMIC
    // asm 00008F6E: 	OR	*+AR4(OFLAGS),R0
    // asm 00008F6F: 	STI	R0,*+AR4(OFLAGS)
    // *INITIALIZE CENTERXYZ,TRANSXYZ,VERTS
    // asm 00008F70: 	LDI	AR4,AR3
    // asm 00008F71: 	ADDI	ODYNALIST,AR3
    // asm 00008F72: 	LDI	*AR2++,RC		;GET DYNAMIC OBJECT COUNT
    // asm 00008F73: 	RPTB	WHEELLP
    // asm 00008F74:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
    // asm 00008F75: 	STI	AR0,*AR3
    // asm 00008F76: 	LDF	*AR2++,R0
    // asm 00008F77: 	STF	R0,*+AR0(DYNACENTERX)
    // asm 00008F78: 	STF	R0,*+AR0(DYNATRANSX)
    // asm 00008F79: 	LDF	*AR2++,R0
    // asm 00008F7A: 	STF	R0,*+AR0(DYNACENTERY)
    // asm 00008F7B: 	STF	R0,*+AR0(DYNATRANSY)
    // asm 00008F7C: 	LDF	*AR2++,R0
    // asm 00008F7D: 	STF	R0,*+AR0(DYNACENTERZ)
    // asm 00008F7E: 	STF	R0,*+AR0(DYNATRANSZ)
    // asm 00008F7F: 	LDI	*AR2++,R0
    // asm 00008F80: 	STI	R0,*+AR0(DYNANVERTS)
    // asm 00008F81: 	LDI	*AR2++,R0
    // asm 00008F82: 	STI	R0,*+AR0(DYNAFLAG)
    // asm 00008F83: 	STI	AR4,*+AR0(DYNAPARENT)
WHEELLP:
    // asm 00008F84: LDI	AR0,AR3
    // asm 00008F85: 	LDI	0,R0
    // asm 00008F86: 	STI	R0,*AR3			;LAST LINK IS ZERO, DUDES
    // *GET A CAR PROCESS
    // asm 00008F87: 	LDI	*AR2++,R0		;GET PROCESS POINTER
    // asm 00008F88: 	LDI	@CARPROCI,AR2
    // asm 00008F89: 	LDI	DRONE_C|ANI_T,R2	;PID
    // asm 00008F8A: 	CALL	PRC_CREATE_CHILD
    // asm 00008F8B: 	STI	AR0,*+AR4(ORADZ)	;DOUBLING AS A PROC PTR
VANIX:
    // asm 00008F8C: 	POP	AR3
    // asm 00008F8D: 	POP	AR0
    // asm 00008F8E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VEHICLE_ANI_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CAR PROCESS
*
*	SPINS WHEELS
*	TURNS FRONT WHEELS
*	LEANS BODY
*
*PARAMETERS
*	AR4	CAR OBJECT
*	AR5	CAR BLOCK
*	R6	X RADIANS	;FOR WHEEL SPIN
*USES
*	AR3	DYNA OBJECT
*	AR4	MASTER OBJECT
*	AR5	CAR BLOCK
*	AR6	DYNAMATRIX
*	R4	OLD ORADY
*	R5	Z RADIANS FOR BODY LEAN
*	R6	X RADIANS FOR BODY LEAN
*	R7	OLD CAR SPEED
*	PDATA	OLD CAR ORADY
*	PDATA+1 BODY LEAN Z RADIANS
*	PDATA+2 X RADIANS FOR WHEEL SPIN
 */

void CARPROC(void)
{
    // asm 00008F90: 	LDI	*+AR4(OCARBLK),AR5
    // asm 00008F91: 	LDF	0,R6	 		;INIT SPIN RADIANS
    // asm 00008F92: 	LDF	*+AR5(CARSPEED),R0	;INIT SPEED
    // asm 00008F93: 	LDF	R0,R7
    // asm 00008F94: 	LDF	*+AR4(ORADY),R0
    // asm 00008F95: 	STF	R0,*+AR7(PDATA)		;INIT OLD ORADY
    // asm 00008F96: 	CLRF	R5			;INITIALIZE BODY Z RADIANS
    // asm 00008F97: 	STF	R5,*+AR7(PDATA+1)	;SAVE Z RADIANS
    // asm 00008F98: 	LDF	0,R0			;INITIALIZE WHEEL X RADIANS
    // asm 00008F99: 	STF	R0,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
CARPROCL:
    // asm 00008F9A: 	LDI	3,AR2	  		;SLEEP TIME
    // asm 00008F9B: 	LDI	@_MODE,R0
    // asm 00008F9C: 	AND	MMODE,R0
    // asm 00008F9D: 	CMPI	MINTRO,R0
    // asm 00008F9E: 	BEQ	NCS
    // asm 00008F9F: 	LDI	@_MODE,R2		;ARE WE AT STARTING LINE?
    // asm 00008FA0: 	TSTB	MSLINE,R2
    // asm 00008FA1: 	BNZ	NCS
    // asm 00008FA2: 	LDI	@SUSPEND_MODE,R0
    // asm 00008FA3: 	CMPI	SM_HALT,R0
    // asm 00008FA4: 	BNE	NCS
    // asm 00008FA5: 	LDF	*+AR5(CARSPEED),R7	;UPDATE OLD SPEED TO AVOID JERK
    // asm 00008FA6: 	B	CARSLP
NCS:
    // asm 00008FA7: 	LDI	*+AR4(ODIST),R0
    // asm 00008FA8: 	CMPI	20000,R0		;FAR OFF JUST SLEEP
    // asm 00008FA9: 	BGT	CARSLP
    // *GET FRONT WHEEL STEER MATRIX
    // asm 00008FAA: 	LDF	*+AR5(CARTURN),R2
    // asm 00008FAB: 	MPYF	1.5,R2			;BOOST TURN A LITTLE
    // asm 00008FAC: 	LDI	@MATRIXAI,AR2
    // asm 00008FAD: 	CALL	FIND_YMATRIX
    // asm 00008FAE: 	LDI	AR2,AR0
    // *GET WHEEL SPIN MATRIX
    // asm 00008FAF: 	LDF	*+AR5(CARSPEED),R2
    // asm 00008FB0: 	MPYF	0.02,R2   		;FUDGE FACTOR
    // asm 00008FB1: 	ADDF	*+AR7(PDATA+2),R2
    // asm 00008FB2: 	STF	R2,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
    // asm 00008FB3: 	LDI	@MATRIXBI,AR2		;GET X SPIN IN MATRIXB
    // asm 00008FB4: 	CALL	FIND_XMATRIX
    // *CONCAT FOR FRONT WHEELS
    // asm 00008FB5: 	LDI	@MATRIXCI,AR1		;A X B = C
    // asm 00008FB6: 	LDI	AR1,AR6			;SAVE FRONT WHEEL MATRIX PTR
    // asm 00008FB7: 	LDI	AR2,AR3			;SAVE REAR WHEEL MATRIX PTR
    // asm 00008FB8: 	CALL	CONCAT201    		;CONCAT YOUR MATRICES INTO DYNOBJ
    // *STUFF YOUR DYNAMIC MATRICES
    // asm 00008FB9: 	LDI	*+AR4(ODYNALIST),R0
    // asm: 	SLOCKON	Z,"UTIL\CARPROC   dynamic objects not found"
CDTOP:
    // asm 00008FBA: 	LDI	R0,AR0
    // asm 00008FBB: 	LDI	*+AR0(DYNAFLAG),R0
    // asm 00008FBC: 	BN	CDLP			;SHADOW...CONTINUE
    // asm 00008FBD: 	BZ	CARBODY			;HANDLE BODY
    // asm 00008FBE: 	LDI	AR0,AR2
    // asm 00008FBF: 	ADDI	DYNAMATRIX,AR2
    // asm 00008FC0: 	CMPI	1,R0
    // asm 00008FC1: 	BZ	CARRWHL			;REAR WHEEL
    // *STUFF FRONT WHEEL
    // asm 00008FC2: 	LDF	*AR6++,R0
    // asm 00008FC3: 	RPTS	7
    // asm 00008FC4: 	LDF	*AR6++,R0
    // asm 00008FC5:  	STF	R0,*AR2++
    // asm 00008FC6: 	NOP	*AR6--(9)
    // asm 00008FC7: 	B	CDLP
    // *STUFF REAR WHEEL
CARRWHL:
    // asm 00008FC8: 	LDF	*AR3++,R0
    // asm 00008FC9: 	RPTS	7
    // asm 00008FCA: 	LDF	*AR3++,R0
    // asm 00008FCB:  	STF	R0,*AR2++
    // asm 00008FCC: 	NOP	*AR3--(9)
CDLP:
    // asm 00008FCD: 	LDI	*AR0,R0
    // asm 00008FCE: 	BNZ	CDTOP
    // asm 00008FCF: 	LDI	3,AR2	  		;SLEEP TIME
    // asm 00008FD0: 	B	CARSLP
    // *HANDLE BODY
    // *BODY MUST BE LAST
CARBODY:
    // asm 00008FD1: 	CALL	LEAN
    // asm 00008FD2: 	LDI	1,AR2
CARSLP:
    // asm 00008FD3: 	CALL	SLEEP
    // asm 00008FD4: 	B 	CARPROCL
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARPROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: NTWOPII	.float	-TWOPI */
float NTWOPII = -TWOPI;

/* *
*LEAN BODY IN Z
*
*PARAMETERS
*	AR0	DYNA OBJECT
*	AR4	MASTER OBJECT
*	AR5	CAR BLOCK
*	AR6	DYNAMATRIX
*	R4	OLD ORADY
*	R5	Z RADIANS
*	R6	X RADIANS
*	R7	OLD SPEED
*	PDATA	OLD CAR ORADY
*	PDATA+1 BODY LEAN Z RADIANS
*
 */
void LEAN(void)
{
    // asm 00008FD6: 	LDI	AR0,AR1
    // asm 00008FD7: 	ADDI	DYNAMATRIX,AR1
    // 	;GET X LEAN (BRAKE/ACCEL)
    // 	;
    // asm 00008FD8: 	LDF	R7,R0
    // asm 00008FD9: 	LDF	*+AR5(CARSPEED),R7	;GET NEW SPEED
    // asm 00008FDA: 	SUBF	R0,R7,R0
    // asm 00008FDB: 	MPYF	0.06,R0			;CONVERT TO RADIANS
    // asm 00008FDC: 	ADDF	R0,R6
    // asm 00008FDD: 	MPYF	0.25,R6
    // asm 00008FDE: 	NEGF	R6,R2
    // asm 00008FDF: 	LDI	*+AR5(CAR_AIRF),R0
    // asm 00008FE0: 	OR	*+AR5(CAR_AIRB),R0
    // asm 00008FE1: 	LDFNZ	0,R2 			;ZERO OUT WHEN IN AIR FOLKS
    // asm 00008FE2: 	LDF	R2,R2			;AMPLIFY ACCELERATION ONLY
    // asm 00008FE3: 	LDFGT	1,R1
    // asm 00008FE4: 	LDFLT	2,R1
    // asm 00008FE5: 	MPYF	R1,R2
    // asm 00008FE6: 	LDF	*+AR5(CARRPM),R0      	;REV FACTOR
    // asm 00008FE7: 	MPYF	0.01,R0
    // asm 00008FE8: 	MPYF	-0.05,R0
    // asm 00008FE9: 	ADDF	R0,R2
    // asm 00008FEA: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm 00008FEB: 	LDFGT	0.1,R2
    // asm 00008FEC: 	CMPF	-0.1,R2
    // asm 00008FED: 	LDFLT	-0.1,R2
    // asm 00008FEE: 	STF	R2,*+AR5(CARXLEAN)
    // asm 00008FEF: 	LDI	@MATRIXBI,AR2
    // asm 00008FF0: 	CALL	FIND_XMATRIX
    // asm 00008FF1: 	LDI	AR2,AR0			;SAVE MATRIX PTR
    // 	;GET YOUR Z LEAN (CORNERING)
    // 	;
    // asm 00008FF2: 	LDF	*+AR7(PDATA),R4		;OLD ORADY
    // asm 00008FF3: 	LDF	*+AR7(PDATA+1),R5	;Z RADIANS
    // asm 00008FF4: 	LDF	*+AR4(ORADY),R0
    // asm 00008FF5: 	STF	R0,*+AR7(PDATA)		;SAVE NEW OLD ORADY
    // asm 00008FF6: 	SUBF	R4,R0			;DELTA ORADY
    // asm 00008FF7: 	LDF	0,R1
    // asm 00008FF8: 	CMPF	3.14,R0
    // asm 00008FF9: 	LDFGT	@NTWOPII,R1
    // asm 00008FFA: 	CMPF	-3.14,R0
    // asm 00008FFB: 	LDFLT	@TWOPII,R1
    // asm 00008FFC: 	ADDI	R1,R0			;HANDLE RADIAN WRAPAROUND
    // asm 00008FFD: 	MPYF	R7,R0			;MULTIPLY BY SPEED FACTOR
    // asm 00008FFE: 	MPYF	0.06,R0			;CONVERT TO RADIANS
    // asm 00008FFF: 	MPYF	0.1,R0			;CONVERT TO RADIANS
    // asm 00009000: 	ADDF	R0,R5
    // asm 00009001: 	MPYF	0.5,R5
    // asm 00009002: 	STF	R5,*+AR7(PDATA+1)	;SAVE NEW Z RADIANS
    // asm 00009003: 	NEGF	R5,R2
    // asm 00009004: 	LDI	*+AR5(CAR_AIRF),R0
    // asm 00009005: 	OR	*+AR5(CAR_AIRB),R0
    // asm 00009006: 	LDFNZ	0,R2 			;ZERO OUT WHEN IN AIR FOLKS
    // asm 00009007: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm 00009008: 	LDFGT	0.1,R2
    // asm 00009009: 	CMPF	-0.1,R2
    // asm 0000900A: 	LDFLT	-0.1,R2
    // asm 0000900B: 	STF	R2,*+AR5(CARZLEAN)    	;SAVE IT
    // ;	MPYF	3,R2			;PUMP IT UP
    // asm 0000900C: 	MPYF	2.2,R2			;PUMP IT UP
    // asm 0000900D: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm 0000900E: 	LDFGT	0.1,R2
    // asm 0000900F: 	CMPF	-0.1,R2
    // asm 00009010: 	LDFLT	-0.1,R2
    // asm 00009011: 	LDI	@MATRIXAI,AR2		;GET Z IN TEMP THING
    // asm 00009012: 	CALL	FIND_ZMATRIX
    // asm 00009013: 	CALL	CONCAT201    		;CONCAT YOUR MATRICES INTO DYNOBJ
    // asm 00009014: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEAN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*DYNAOBJECT DYNAMIC ALLOCATION SUBSYSTEM
*----------------------------------------------------------------------------
*
 */
/* asm: DYNALIST	.bss	DYNALIST,NUM_DYNAS*DYNASIZE */
int DYNALIST[NUM_DYNAS*DYNASIZE];
/* asm: DYNAFREE	.bss	DYNAFREE,1 */
int DYNAFREE;
/* asm: NULL	.bss	NULL,1 */
int NULL;

/* *----------------------------------------------------------------------------
 */
void DYNAOBJ_INIT(void)
{
    // asm 00009018: 	PUSH	AR2
    // asm 00009019: 	PUSH	R2
    // asm 0000901A: 	PUSH	R3
    // asm 0000901B: 	PUSH	RC
    // asm 0000901C: 	PUSH	RS
    // asm 0000901D: 	LDI	@DYNALISTI,AR2
    // asm 0000901E: 	LDI	@DYNAFREEI,R2
    // asm 0000901F: 	LDI	@NULLI,R3
    // asm 00009020: 	LDI	NUM_DYNAS-1,RC
    // asm 00009021: 	LDI	DYNASIZE,RS
    // asm 00009022: 	CALL	INIT_LINKED_LIST
    // asm 00009023: 	POP	RS
    // asm 00009024: 	POP	RC
    // asm 00009025: 	POP	R3
    // asm 00009026: 	POP	R2
    // asm 00009027: 	POP	AR2
    // asm 00009028: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DYNAOBJ_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GET A DYNAMIC OBJECT
*
*RETURNS
*	(SUCCESSFUL)
*		CARRY SET
*		AR0	POINTER TO DYNA OBJ
*	(UNSUCCESSFUL)
*		CARRY CLEAR
*
*
 */
void GETDYNA(void)
{
    // asm 00009029: 	PUSH	R0
    // ;	LDP	@DYNAFREE
    // asm 0000902A: 	LDI	@DYNAFREE,R0
    // asm 0000902B: 	LDI	R0,AR0
    // asm: 	SLOCKON	Z,"UTIL\GETDYNA   out of dynamic objects"
    // asm 0000902C: 	BZ	GETDYNA_ERR
    // asm 0000902D: 	LDI	*AR0,R0
    // asm 0000902E: 	STI	R0,@DYNAFREE
    // asm 0000902F: 	ADDI	DYNAMATRIX,AR0		;INIT YOUR MATRIX FOLKS
    // asm 00009030: 	CALL	INITMAT
    // asm 00009031: 	SUBI	DYNAMATRIX,AR0
    // asm 00009032: 	CLRI	R0
    // asm 00009033: 	STI	R0,*+AR0(DYNAFLAG)
    // asm 00009034: 	SETC
GETDYNA_X:
    // asm 00009035: 	POP	R0
    // asm 00009036: 	RETS
GETDYNA_ERR:
    // asm 00009037: 	CLRC
    // asm 00009038: 	B	GETDYNA_X
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETDYNA", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*DELETE A DYNA OBJECT
*
*PARAMETERS
*	AR2	POINTER TO DYNA OBJ
*
*
 */
void DELDYNA(void)
{
    // asm 00009039: 	PUSH	R0
    // ;	LDP	@DYNAFREE
    // asm 0000903A: 	LDI	@DYNAFREE,R0
    // asm 0000903B: 	STI	R0,*AR2
    // asm 0000903C: 	STI	AR2,@DYNAFREE
    // asm 0000903D: 	POP	R0
    // asm 0000903E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELDYNA", 0, 0);
    UNIMPL();
}

/* asm: CARLIST	.bss	CARLIST,NUM_CARS*CARSIZ */
int CARLIST[NUM_CARS*CARSIZ];
/* asm: CARFREE	.bss	CARFREE,1 */
int CARFREE;
/* asm: CAR_COUNT	.bss	CAR_COUNT,1 */
int CAR_COUNT;

/* *----------------------------------------------------------------------------
 */
void CARB_INIT(void)
{
    // asm 00009041: 	LDI	@CARFREEI,AR0
    // asm 00009042: 	LDI	@CARLISTI,AR1
    // asm 00009043: 	LDI	NUM_CARS-1,RC
    // asm 00009044: 	RPTB	CARINTL
    // asm 00009045: 	STI	AR1,*AR0
    // asm 00009046: 	LDI	AR1,AR0
CARINTL:
    // asm 00009047: ADDI	CARSIZ,AR1
    // asm 00009048: 	LDI	0,R0
    // asm 00009049: 	STI	R0,*AR0
    // asm 0000904A: 	STPI	R0,@CAR_COUNT
    // asm 0000904B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARB_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GET A CAR OBJECT
*
*RETURNS
*	(SUCCESSFUL)
*		AR0	POINTER TO CAR OBJ
*		CARRY SET
*	(UNSUCCESSFUL)
*		CARRY CLEAR
*
*
 */
void GETCAR(void)
{
    // asm 0000904C: 	PUSH	R0
    // ;	LDP	@CARFREE
    // asm 0000904D: 	LDI	@CARFREE,AR0
    // asm 0000904E: 	CMPI	0,AR0
    // asm: 	SLOCKON	Z,"UTIL\GETCAR   out of cars"
    // asm 0000904F: 	BZ	GETCAR_ERR
    // asm 00009050: 	LDI	*AR0,R0
    // asm 00009051: 	STI	R0,@CARFREE
    // asm 00009052: 	INCM	@CAR_COUNT
    // asm 00009055: 	SETC
GETCAR_X:
    // asm 00009056: 	POP	R0
    // asm 00009057: 	RETS
GETCAR_ERR:
    // asm 00009058: 	CLRC
    // asm 00009059: 	B	GETCAR_X
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCAR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*DELETE A CAR OBJECT
*
*PARAMETERS
*	AR2	POINTER TO CAR OBJ
*
 */
void DELCAR(void)
{
    // asm 0000905A: 	PUSH	R0
    // ;	LDP	@CARFREE
    // asm 0000905B: 	LDI	@CARFREE,R0
    // asm 0000905C: 	STI	R0,*AR2
    // asm 0000905D: 	STI	AR2,@CARFREE
    // ;	LDP	@CAR_COUNT
    // asm 0000905E: 	LDI	@CAR_COUNT,R0
    // asm 0000905F: 	DEC	R0
    // asm: 	SLOCKON	LT,"UTIL\DELCAR   erroneous CAR_COUNT"
    // asm 00009060: 	STI	R0,@CAR_COUNT
    // asm 00009061: 	POP	R0
    // asm 00009062: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELCAR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SCAN_OBJECTS(void)
{
    // asm 00009063: 	CALL	ISCAN
    // asm 00009064: 	SLEEP	1
    // asm 00009066: 	CALL	OSCAN
    // asm 00009067: 	SLEEP	1
    // asm 00009069: 	B	SCAN_OBJECTS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCAN_OBJECTS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void PUSHALL(void)
{
    // asm 0000906A: 	POP	BK
    // asm 0000906B: 	PUSH	AR0
    // asm 0000906C: 	PUSH	AR1
    // asm 0000906D: 	PUSH	AR2
    // asm 0000906E: 	PUSH	AR3
    // asm 0000906F: 	PUSH	AR4
    // asm 00009070: 	PUSH	AR5
    // asm 00009071: 	PUSH	AR6
    // asm 00009072: 	PUSH	AR7
    // asm 00009073: 	PUSH	R0
    // asm 00009074: 	PUSH	R1
    // asm 00009075: 	PUSH	R2
    // asm 00009076: 	PUSH	R3
    // asm 00009077: 	PUSH	R4
    // asm 00009078: 	PUSH	R5
    // asm 00009079: 	PUSH	R6
    // asm 0000907A: 	PUSH	R7
    // asm 0000907B: 	PUSHF	R0
    // asm 0000907C: 	PUSHF	R1
    // asm 0000907D: 	PUSHF	R2
    // asm 0000907E: 	PUSHF	R3
    // asm 0000907F: 	PUSHF	R4
    // asm 00009080: 	PUSHF	R5
    // asm 00009081: 	PUSHF	R6
    // asm 00009082: 	PUSHF	R7
    // asm 00009083: 	BU	BK
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PUSHALL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void POPALL(void)
{
    // asm 00009084: 	POP	BK
    // asm 00009085: 	POPF	R7
    // asm 00009086: 	POPF	R6
    // asm 00009087: 	POPF	R5
    // asm 00009088: 	POPF	R4
    // asm 00009089: 	POPF	R3
    // asm 0000908A: 	POPF	R2
    // asm 0000908B: 	POPF	R1
    // asm 0000908C: 	POPF	R0
    // asm 0000908D: 	POP	R7
    // asm 0000908E: 	POP	R6
    // asm 0000908F: 	POP	R5
    // asm 00009090: 	POP	R4
    // asm 00009091: 	POP	R3
    // asm 00009092: 	POP	R2
    // asm 00009093: 	POP	R1
    // asm 00009094: 	POP	R0
    // asm 00009095: 	POP	AR7
    // asm 00009096: 	POP	AR6
    // asm 00009097: 	POP	AR5
    // asm 00009098: 	POP	AR4
    // asm 00009099: 	POP	AR3
    // asm 0000909A: 	POP	AR2
    // asm 0000909B: 	POP	AR1
    // asm 0000909C: 	POP	AR0
    // asm 0000909D: 	BU	BK
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POPALL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	X = sin(T)
*	Y = cos(T)
*	R2	(FL) THETA
*	R0	(FL) DISTANCE
*RETURNS
*	R0	(FL) X DISTANCE
*	R1	(FL) Z DISTANCE
*
*
 */
void DISTANCE_2D(void)
{
    // asm 0000909E: 	PUSHFL	R3
    // asm 000090A0: 	LDF	R0,R1
    // asm 000090A1: 	NEGF	R0,R3
    // asm 000090A2: 	CALL	_COSI
    // asm 000090A3: 	MPYF	R0,R1
    // asm 000090A4: 	CALL	_SINE
    // asm 000090A5: 	MPYF	R3,R0
    // asm 000090A6: 	POPFL	R3
    // asm 000090A8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISTANCE_2D", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ADD OBJECT VELOCITIES
*
*PARAMETERS
*	AR4	OBJECT
*
 */
void OVELADD(void)
{
    // asm 000090A9: 	LDF	*+AR4(OVELX),R0
    // asm 000090AA: 	ADDF	*+AR4(OPOSX),R0
    // asm 000090AB: 	STF	R0,*+AR4(OPOSX)
    // asm 000090AC: 	LDF	*+AR4(OVELY),R0
    // asm 000090AD: 	ADDF	*+AR4(OPOSY),R0
    // asm 000090AE: 	STF	R0,*+AR4(OPOSY)
    // asm 000090AF: 	LDF	*+AR4(OVELZ),R0
    // asm 000090B0: 	ADDF	*+AR4(OPOSZ),R0
    // asm 000090B1: 	STF	R0,*+AR4(OPOSZ)
    // asm 000090B2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OVELADD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ADD N FRAMES X VELOCITY
 */
void OVELNADD(void)
{
    // asm 000090B3: 	FLOATP	@NFRAMES,R1
    // asm 000090B4: 	LDF	*+AR4(OVELX),R0
    // asm 000090B5: 	MPYF	R1,R0
    // asm 000090B6: 	ADDF	*+AR4(OPOSX),R0
    // asm 000090B7: 	STF	R0,*+AR4(OPOSX)
    // asm 000090B8: 	LDF	*+AR4(OVELY),R0
    // asm 000090B9: 	MPYF	R1,R0
    // asm 000090BA: 	ADDF	*+AR4(OPOSY),R0
    // asm 000090BB: 	STF	R0,*+AR4(OPOSY)
    // asm 000090BC: 	LDF	*+AR4(OVELZ),R0
    // asm 000090BD: 	MPYF	R1,R0
    // asm 000090BE: 	ADDF	*+AR4(OPOSZ),R0
    // asm 000090BF: 	STF	R0,*+AR4(OPOSZ)
    // asm 000090C0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OVELNADD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*MOVE FORWARD
*PARAMETERS
*	AR2	MATRIX
*	R2	DIST
*	R3	DESTINATION VECTOR
 */
void FORWARD(void)
{
    // asm 000090C1: 	PUSH	AR2
    // asm 000090C2: 	LDF	0,R0
    // asm 000090C3: 	PUSHF	R0
    // asm 000090C4: 	PUSHF	R0
    // asm 000090C5: 	PUSHF	R2
    // asm 000090C6: 	LDI	AR2,R2
    // asm 000090C7: 	LDI	SP,AR2
    // asm 000090C8: 	SUBI	2,AR2
    // asm 000090C9: 	CALL	MATRIX_MUL
    // asm 000090CA: 	POPF	R2
    // asm 000090CB: 	SUBI	2,SP
    // asm 000090CC: 	POP	AR2
    // asm 000090CD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FORWARD", 0, 0);
    UNIMPL();
}

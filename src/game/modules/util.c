#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/mproc.h"
#include "../include/c30.h"
#include "../include/macs.h"
#include "../include/obj.h"
#include "../include/objects.h"
#include "../include/globals.h"
#include "../include/sys.h"
#include "../include/text.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/sysid.h"
#include "../include/dirq.h"
#include "../include/delta.h"
#include "../include/util.h"

/*
 * Source module: asm/UTIL.ASM
 */

// *----------------------------------------------------------------------------
// *UTILITY ROUTINES
// *
// *COPYRIGHT (C) 1994 BY TV GAMES,INC.
// *ALL RIGHTS RESERVED
// *
// *
/* asm: RAND	pbss	RAND,1 */
int RAND;
/* asm: CRTCTLRAM	.bss	CRTCTLRAM,1 */
int CRTCTLRAM;
/* asm: ACTIVE_SCREEN	.bss	ACTIVE_SCREEN,1 */
int ACTIVE_SCREEN;
// *----------------------------------------------------------------------------
// *ROM DEFINITIONS
// *
/* asm: LINE255I	.word	SCREEN0+3F000H 	;LAST LINE PAGE 0 */
int LINE255I = (int)(SCREEN0+0x3F000);
/* asm: LINE511I	.word	SCREEN0+7FC00H	;LAST LINE PAGE 1 */
int LINE511I = (int)(SCREEN0+0x7FC00);
/* asm: SCRSIZI	.word	3FFFFH */
int SCRSIZI = (int)(0x3FFFF);
#if DEBUG
// *----------------------------------------------------------------------------
// *TV30 DEBUGGING ROUTINES
// *
// *The two rountines TVBP, and TVBPX, if present, are called upon entering and
// *exiting a breakpoint (respectively).
// *
// *----------------------------------------------------------------------------
#endif
/* asm: PAGEWORD	.word	0 */
int PAGEWORD = (int)(0);
/* asm: FILSIZI	.word	3FFFFH */
int FILSIZI = (int)(0x3FFFF);
/* asm: FILWORD	.word	93093H */
int FILWORD = (int)(0x93093);
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *CAR PROCESS
// *
// *	SPINS WHEELS
// *	TURNS FRONT WHEELS
// *	LEANS BODY
// *
// *PARAMETERS
// *	AR4	CAR OBJECT
// *	AR5	CAR BLOCK
// *	R6	X RADIANS	;FOR WHEEL SPIN
// *USES
// *	AR3	DYNA OBJECT
// *	AR4	MASTER OBJECT
// *	AR5	CAR BLOCK
// *	AR6	DYNAMATRIX
// *	R4	OLD ORADY
// *	R5	Z RADIANS FOR BODY LEAN
// *	R6	X RADIANS FOR BODY LEAN
// *	R7	OLD CAR SPEED
// *	PDATA	OLD CAR ORADY
// *	PDATA+1 BODY LEAN Z RADIANS
// *	PDATA+2 X RADIANS FOR WHEEL SPIN
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *DYNAOBJECT DYNAMIC ALLOCATION SUBSYSTEM
// *----------------------------------------------------------------------------
// *
/* asm: DYNALIST	.bss	DYNALIST,NUM_DYNAS*DYNASIZE */
int DYNALIST[NUM_DYNAS*DYNASIZE];
/* asm: DYNAFREE	.bss	DYNAFREE,1 */
int DYNAFREE;
/* asm: NULL	.bss	NULL,1 */
int NULL;
// *----------------------------------------------------------------------------
/* asm: CARLIST	.bss	CARLIST,NUM_CARS*CARSIZ */
int CARLIST[NUM_CARS*CARSIZ];
/* asm: CARFREE	.bss	CARFREE,1 */
int CARFREE;
/* asm: CAR_COUNT	.bss	CAR_COUNT,1 */
int CAR_COUNT;
// *----------------------------------------------------------------------------

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

void SETPAGE0(void)
{
    // *----------------------------------------------------------------------------
    // *SET SCREEN DISPLAY TO PAGE 0  (AND WRITE PAGE TO 1)
    // *
    // ;	.if	DEBUG
    // asm: 	LDI	@PAGEWORD,R0
    // asm: 	BNE	P1
    // ;	.endif
    // asm: 	LDI	@SCREEN1I,R0		;set active screen to 1 (writeable)
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDP	@DMA_SETUP
    // asm: 	LDI	@DMA_SETUP,R0
    // asm: 	ANDN	DMA_VIDEO_PAG_DISPLAYED,R0
    // asm: 	OR	DMA_DMA_WRITE_PAGE,R0
    // asm: 	STI	R0,@DMA_SETUP
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETPAGE0", 0, 0);
    UNIMPL();
}

void SETPAGE1(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SET SCREEN DISPLAY TO PAGE 1	(AND WRITE PAGE TO 0)
    // *
    // ;	.if	DEBUG
    // asm: 	LDI	@PAGEWORD,R0
    // asm: 	BNE	P1
    // ;	.endif
    // asm: 	LDI	@SCREEN0I,R0		;set active screen to 1 (writeable)
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDP	@DMA_SETUP
    // asm: 	LDI	@DMA_SETUP,R0
    // asm: 	OR	DMA_VIDEO_PAG_DISPLAYED,R0
    // asm: 	ANDN	DMA_DMA_WRITE_PAGE,R0
    // asm: 	STI	R0,@DMA_SETUP
    // asm: 	SETDP
    // asm: 	RETS
    // ;	.if	DEBUG
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETPAGE1", 0, 0);
    UNIMPL();
}

void FASTCLR0(void)
{
    // ;	.endif
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SRT PAGE ONE
    // *
    // *CLEAR SCREEN FAST SHIFT REGISTER TRANSFER
    // *CALL ONLY DURING VBLANK
    // *
    /* no executable asm lines detected */
    UNIMPL();
}

void FASTCLR1(void)
{
    // asm: 	LDI	@NOAERASE,R0
    // asm: 	RETSNZ
    // asm: 	LDI	0,AR2		;X
    // asm: 	STI	AR2,@_ACNTL
    // asm: 	LDI	0,R2		;Y
    // asm: 	LDI	511,R3		;X2
    // asm: 	LDI	399,RC		;Y2
    // asm: 	LDI	0,RS		;PAL
    // asm: 	LDI	0A0h,RE		;ADDR
    // asm: 	RS	16,RE
    // asm: 	CALL	_rdma
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FASTCLR1", 0, 0);
    UNIMPL();
}

void CLRSCRN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CLEAR SCREEN BITMAP
    // *
    // asm: 	CALL	CLRSCRN0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRSCRN", 0, 0);
    UNIMPL();
}

void CLRSCRN1(void)
{
    // asm: 	PUSH	AR2
    // asm: 	LDI	@SCREEN1I,AR2
    // asm: 	B	CLRSC00
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRSCRN1", 0, 0);
    UNIMPL();
}

void CLRSCRN0(void)
{
    // asm: 	PUSH	AR2
    // asm: 	LDI	@SCREEN0I,AR2
    // asm: CLRSC00	PUSH	R3
    // asm: 	LDI	@SCRSIZI,R3
CLRSC01:
    // asm: 	PUSH	R2
    // asm: 	LDI	0,R2
    // asm: 	CALL	SCREEN_FILL
    // asm: 	POP	R2
    // asm: 	POP	R3
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRSCRN0", 0, 0);
    UNIMPL();
}

void CLR255(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CLEAR LINE 255,255 OF BITMAP
    // *
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R3
    // asm: 	LDI	@LINE255I,AR2
    // asm: 	LDI	1023,R3		;ONE ROW ONLY
    // asm: 	B	CLRSC01
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR255", 0, 0);
    UNIMPL();
}

void CLR511(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CLEAR LINE 511,511 OF BITMAP
    // *
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R3
    // asm: 	LDI	@LINE511I,AR2
    // asm: 	LDI	1023,R3		;ONE ROW ONLY
    // asm: 	B	CLRSC01
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR511", 0, 0);
    UNIMPL();
}

void SCRNFIL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *FILL SCREEN
    // asm: 	LDI	@SCREEN0I,AR2
    // asm: 	LDI	@FILSIZI,R3
    // asm: 	LDI	@FILWORD,R2	;fill it with some crud
    // asm: 	B	SCREEN_FILL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCRNFIL", 0, 0);
    UNIMPL();
}

void SCREEN_FILL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SCREEN WRITER
    // *
    // *PARAMETERS
    // *	AR2	START ADDRESS
    // *	R2	COLOR
    // *	R3	COUNT-1
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	IE
    // asm: 	LDP	@COMMINTM
    // asm: 	LDI	@COMMINTM,IE
    // asm: 	SETDP
    // asm: 	PUSH	DP
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	0,AR1
    // asm: 	LDI	R3,RC
    // asm: 	LDI	HARD_WS,R0
    // asm: 	STI	R0,@CPU_WS
    // asm: 	RPTB	CLRSCL
    // asm: 	STI	R2,*AR2++
    // asm: CLRSCL	LDI	*AR1,R0		;DUMMY READ FOR WAIT STATE SHIT
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	SOFT_WS,R0
    // asm: 	STI	R0,@CPU_WS
    // asm: 	POP	DP
    // asm: 	POP	IE
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCREEN_FILL", 0, 0);
    UNIMPL();
}

void CLRCRAM(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CLEAR COLOR RAM
    // *
    // asm: 	LDI	COLORAM>>16,AR0
    // asm: 	LSH	16,AR0
    // asm: 	LDI	0,R1
    // asm: 	RPTS	07FFFH
    // asm: 	STI	R1,*AR0++
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRCRAM", 0, 0);
    UNIMPL();
}

void RANDOM(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *RANDOM NUMBER UTILITIES
    // *----------------------------------------------------------------------------
    // *RANDOM - GENERATE A RANDOM NUMBER
    // *RETURNS
    // *	R0	32 BIT RANDOM #
    // *
    // asm: 	PUSH	R1
    // asm: 	LDI	@RAND,R0
    // asm: 	LDI	R0,R1
    // asm: 	LSH	1,R0
    // asm: 	XOR	R0,R1
    // asm: 	BNN	RND2
    // asm: 	OR	1,R0
RND2:
    // asm: POP	R1
    // asm: 	MPYI	794Fh,R0
    // asm: 	STI	R0,@RAND
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDOM", 0, 0);
    UNIMPL();
}

void FRAND(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *FLOATING POINT RANDOM
    // *
    // *PARAMETERS
    // *	R0	NUMBER
    // *RETURNS
    // *	R0	RANDOM NUMBER 0->N FLOATING POINT
    // *
    // asm: 	PUSH	AR2
    // asm: 	PUSHFL	R1
    // asm: 	PUSHF	R0
    // asm: 	LDI	10000,AR2
    // asm: 	CALL	RANDU0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	0.01,R0
    // asm: 	MPYF	0.01,R0
    // asm: 	POPF	R1
    // asm: 	MPYF	R1,R0
    // asm: 	POPFL	R1
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FRAND", 0, 0);
    UNIMPL();
}

void SFRAND(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SIGNED FLOATING POINT RANDOM
    // *
    // *PARAMETERS
    // *	R0	FL NUMBER
    // *RETURNS
    // *	R0	FL RANDOM NUMBER -N->+N FLOATING POINT
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSHF	R1
    // asm: 	PUSHF	R0
    // asm: 	MPYF	2,R0
    // asm: 	CALL	FRAND
    // asm: 	POPF	R1
    // asm: 	SUBF	R1,R0
    // asm: 	POPF	R1
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SFRAND", 0, 0);
    UNIMPL();
}

void RANDU0(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *RANDOM UNSIGNED 0 TO N-1
    // *
    // *PARAMETERS
    // *	AR2	N  RANGE INPUT (0-FFFF)
    // *RETURNS
    // *	R0	RANDOM # BETWEEN 0 AND [AR2]
    // *
    // asm: 	CALL	RANDOM
    // asm: 	LSH	-16,R0
    // asm: 	MPYI	AR2,R0
    // asm: 	LSH	-16,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDU0", 0, 0);
    UNIMPL();
}

void RANDU(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *RANDOM UNSIGNED
    // *
    // *PARAMETERS
    // *	AR2	N
    // *RETURNS
    // *	R0	RANDOM # BETWEEN 1 AND N
    // asm: 	CALL	RANDU0
    // asm: 	ADDI	1,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDU", 0, 0);
    UNIMPL();
}

void SRAND(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SIGNED RANDOM
    // *
    // *PARAMETERS
    // *	AR2	N
    // *RETURNS
    // *	R0	RANDOM # IN RANDGE +/- N
    // *
    // asm: 	LSH	1,AR2
    // asm: 	ADDI	1,AR2
    // asm: 	CALL	RANDU0
    // asm: 	LSH	-1,AR2
    // asm: 	SUBI	AR2,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SRAND", 0, 0);
    UNIMPL();
}

void RANDPER(void)
{
    // *----------------------------------------------------------------------------
    // ;*----------------------------------------------------------------------------
    // ;*RANGE RANDOM
    // ;*PARAMETERS
    // ;*	AR2	LOWER BOUND
    // ;*	R2	UPPER BOUND
    // ;*RETURNS
    // ;*	R0	RANDOM # FROM AR2 TO R2
    // ;*
    // ;RANGRAND:
    // ;	SUBI	AR2,R2,R0
    // ;	ADDI	1,R0
    // ;	CALL	RANDU0
    // ;	ADDI	AR2,R0
    // ;	RETS
    // ;*----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *RANDOM % ROUTINE
    // *
    // *PARAMETERS
    // *	AR2	PROBABILITY OF EVENT (0-1000) P(A0=1000) = 1; P(A0=1) = 1/1000.
    // *RETURNS
    // *	C=1	IF PROBABILITY IS TRUE,
    // *		R0	ACTUAL RANDOM # 0-999
    // *	C=0	FOR FALSE
    // *		R0	ZERO
    // asm: 	CALL	RANDOM
    // asm: 	LSH	-16,R0
    // asm: 	MPYI	1000,R0
    // asm: 	LSH	-16,R0
    // asm: 	CMPI	AR2,R0
    // asm: 	BC	RANDPX
    // asm: 	LDI	0,R0
RANDPX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDPER", 0, 0);
    UNIMPL();
}

void INIT_LINKED_LIST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GENERIC LINKED LIST ROUTINES
    // *These routines are for use with any linked list situation that do not
    // *require special attention (such as the object list).
    // *
    // *----------------------------------------------------------------------------
    // *void	INIT_LINKED_LIST(start_addr, free_list, active_list, length-1, size)
    // *
    // *PARAMETERS
    // *	AR2	START ADDRESS
    // *	R2	FREE LIST
    // *	R3	ACTIVE LIST
    // *	RC	LENGTH-1
    // *	RS	ENTRY SIZE
    // *
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	LDI	R3,AR0		;ZERO ACTIVE POINTER
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
    // asm: 	LDI	R2,AR0	 	;GET FREE POINTER
    // asm: 	LDI	RS,R0		;SAVE THE SIZE
    // asm: 	RPTB	INIT_LL
    // asm: 	STI	AR2,*AR0
    // asm: 	LDI	AR2,AR0
INIT_LL:
    // asm: ADDI	R0,AR2
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LINKED_LIST", 0, 0);
    UNIMPL();
}

void GET_LLIST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET_LLIST(free_list,active_list)
    // *GET A LINKED LIST ELEMENT AND INSERT ON LIST
    // *
    // *
    // *PARAMETERS
    // *	AR2	FREELIST PTR
    // *	R2	ACTIVE LIST PTR
    // *
    // *RETURNS
    // *	(SUCCESSFUL)
    // *		AR0	LIST ELEMENT (INSERTED INTO ACTIVE LIST)
    // *		R0	LIST ELEMENT (INSERTED INTO ACTIVE LIST)
    // *		CARRY SET
    // *	(UNSUCCESSFUL)
    // *		CARRY CLEAR
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR1
    // asm: 	LDI	*AR2,R0
    // asm: 	SLOCKON	Z,"GET_LLIST  out of elements"
    // asm: 	BZ	GETLL_ERR
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR0
    // asm: 	STI	AR0,*AR2		;and update free list
    // 	;insert into the active list
    // asm: 	LDI	R2,AR1			;get ptr to active
    // asm: 	LDI	R0,AR0			;get ptr to element
    // asm: 	LDI	*AR1,R1			;get 1st element in active
    // asm: 	STI	R1,*AR0			;link element into element
    // asm: 	STI	AR0,*AR1		;store element into active
    // asm: 	SETC
GETLL_X:
    // asm: 	POP	AR1
    // asm: 	POP	R1
    // asm: 	RETS
GETLL_ERR:
    // asm: 	CLRC
    // asm: 	BU	GETLL_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_LLIST", 0, 0);
    UNIMPL();
}

void ALLOC_LLIST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *ALLOC_LLIST(free_list)
    // *GET A LINKED LIST ELEMENT (DO NOT INSERT TO ACTIVE)
    // *
    // *PARAMETERS
    // *	AR2	FREE LIST
    // *RETURNS
    // *	AR0	LIST ELEMENT (NOT INSERTED INTO ACTIVE LIST)
    // *
    // asm: 	PUSH	R0
    // asm: 	LDI	*AR2,R0
    // asm: 	SLOCKON	Z,"_allocllist  out of elements"
    // asm: 	BZ	ALLOCLIST_ISERROR
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR0
    // asm: 	STI	AR0,*AR2		;and update free list
    // asm: 	LDI	R0,AR0
    // asm: 	SETC
ALLOCLIST_X:
    // asm: 	POP	R0
    // asm: 	RETS
ALLOCLIST_ISERROR:
    // asm: 	CLRC
    // asm: 	BU	ALLOCLIST_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ALLOC_LLIST", 0, 0);
    UNIMPL();
}

void FREE_LLIST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *FREE_LLIST(element, active, free)
    // *FREE AN ELEMENT OF A LINKED LIST
    // *
    // *PARAMETERS
    // *	AR2	OBJECT
    // *	R2	free list
    // *
    // *
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR0
    // asm: 	LDI	R2,AR0		;free
    // asm: 	LDI	*AR0,R2
    // asm: 	STI	R2,*AR2
    // asm: 	STI	AR2,*AR0
    // asm: 	LDI	AR0,R2
    // asm: 	POP	AR0
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREE_LLIST", 0, 0);
    UNIMPL();
}

void DEL_LLIST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DEL_LLIST(element, active, free)
    // *DELETE AN ELEMENT OF A LINKED LIST
    // *
    // *PARAMETERS
    // *	AR2	OBJECT
    // *	R2	ACTIVE LIST
    // *	R3	FREE LIST
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDI	R2,R1
DELLP:
    // asm: LDI	R1,AR1
    // asm: 	LDI	*AR1,R1
    // asm: 	SLOCKON	Z,"DEL_LLIST  end of list found"
    // asm: 	BZ	DEL_LLX
    // asm: 	CMPI	R1,AR2
    // asm: 	BNE	DELLP
    // asm: 	LDI	*AR2,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	R3,AR1			;get free list pointer
    // asm: 	LDI	*AR1,R1
    // asm: 	STI	R1,*AR2
    // asm: 	STI	AR2,*AR1
DEL_LLX:
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEL_LLIST", 0, 0);
    UNIMPL();
}

void VEHICLE_ANI_INIT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *VEHICLE ANIMATION UTILITIES
    // *
    // *----------------------------------------------------------------------------
    // *
    // *GET DYNAMIC OBJECTS FOR A CAR
    // *
    // *PARAMETERS
    // *	AR2	INDEX OF VEHICLE
    // *	AR4	CAR OBJECT
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR3
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDI	*+AR2(VEHTAB_ANI),AR2
    // asm: 	CMPI	0,AR2			;COULD BE A NULL ENTRY (NO ANIMATION)
    // asm: 	BEQ	VANIX
    // asm: 	LDI	O_DYNAMIC,R0	 	;MAKE PARENT OBJECT DYNAMIC
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // *INITIALIZE CENTERXYZ,TRANSXYZ,VERTS
    // asm: 	LDI	AR4,AR3
    // asm: 	ADDI	ODYNALIST,AR3
    // asm: 	LDI	*AR2++,RC		;GET DYNAMIC OBJECT COUNT
    // asm: 	RPTB	WHEELLP
    // asm:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
    // asm: 	STI	AR0,*AR3
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERX)
    // asm: 	STF	R0,*+AR0(DYNATRANSX)
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERY)
    // asm: 	STF	R0,*+AR0(DYNATRANSY)
    // asm: 	LDF	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERZ)
    // asm: 	STF	R0,*+AR0(DYNATRANSZ)
    // asm: 	LDI	*AR2++,R0
    // asm: 	STI	R0,*+AR0(DYNANVERTS)
    // asm: 	LDI	*AR2++,R0
    // asm: 	STI	R0,*+AR0(DYNAFLAG)
    // asm: 	STI	AR4,*+AR0(DYNAPARENT)
WHEELLP:
    // asm: LDI	AR0,AR3
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR3			;LAST LINK IS ZERO, DUDES
    // *GET A CAR PROCESS
    // asm: 	LDI	*AR2++,R0		;GET PROCESS POINTER
    // asm: 	LDI	@CARPROCI,AR2
    // asm: 	LDI	DRONE_C|ANI_T,R2	;PID
    // asm: 	CALL	PRC_CREATE_CHILD
    // asm: 	STI	AR0,*+AR4(ORADZ)	;DOUBLING AS A PROC PTR
VANIX:
    // asm: 	POP	AR3
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VEHICLE_ANI_INIT", 0, 0);
    UNIMPL();
}

void CARPROC(void)
{
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	LDF	0,R6	 		;INIT SPIN RADIANS
    // asm: 	LDF	*+AR5(CARSPEED),R0	;INIT SPEED
    // asm: 	LDF	R0,R7
    // asm: 	LDF	*+AR4(ORADY),R0
    // asm: 	STF	R0,*+AR7(PDATA)		;INIT OLD ORADY
    // asm: 	CLRF	R5			;INITIALIZE BODY Z RADIANS
    // asm: 	STF	R5,*+AR7(PDATA+1)	;SAVE Z RADIANS
    // asm: 	LDF	0,R0			;INITIALIZE WHEEL X RADIANS
    // asm: 	STF	R0,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
CARPROCL:
    // asm: 	LDI	3,AR2	  		;SLEEP TIME
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MINTRO,R0
    // asm: 	BEQ	NCS
    // asm: 	LDI	@_MODE,R2		;ARE WE AT STARTING LINE?
    // asm: 	TSTB	MSLINE,R2
    // asm: 	BNZ	NCS
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BNE	NCS
    // asm: 	LDF	*+AR5(CARSPEED),R7	;UPDATE OLD SPEED TO AVOID JERK
    // asm: 	B	CARSLP
NCS:
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	CMPI	20000,R0		;FAR OFF JUST SLEEP
    // asm: 	BGT	CARSLP
    // *GET FRONT WHEEL STEER MATRIX
    // asm: 	LDF	*+AR5(CARTURN),R2
    // asm: 	MPYF	1.5,R2			;BOOST TURN A LITTLE
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR2,AR0
    // *GET WHEEL SPIN MATRIX
    // asm: 	LDF	*+AR5(CARSPEED),R2
    // asm: 	MPYF	0.02,R2   		;FUDGE FACTOR
    // asm: 	ADDF	*+AR7(PDATA+2),R2
    // asm: 	STF	R2,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
    // asm: 	LDI	@MATRIXBI,AR2		;GET X SPIN IN MATRIXB
    // asm: 	CALL	FIND_XMATRIX
    // *CONCAT FOR FRONT WHEELS
    // asm: 	LDI	@MATRIXCI,AR1		;A X B = C
    // asm: 	LDI	AR1,AR6			;SAVE FRONT WHEEL MATRIX PTR
    // asm: 	LDI	AR2,AR3			;SAVE REAR WHEEL MATRIX PTR
    // asm: 	CALL	CONCAT201    		;CONCAT YOUR MATRICES INTO DYNOBJ
    // *STUFF YOUR DYNAMIC MATRICES
    // asm: 	LDI	*+AR4(ODYNALIST),R0
    // asm: 	SLOCKON	Z,"UTIL\CARPROC   dynamic objects not found"
CDTOP:
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(DYNAFLAG),R0
    // asm: 	BN	CDLP			;SHADOW...CONTINUE
    // asm: 	BZ	CARBODY			;HANDLE BODY
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	DYNAMATRIX,AR2
    // asm: 	CMPI	1,R0
    // asm: 	BZ	CARRWHL			;REAR WHEEL
    // *STUFF FRONT WHEEL
    // asm: 	LDF	*AR6++,R0
    // asm: 	RPTS	7
    // asm: 	LDF	*AR6++,R0
    // asm:  	STF	R0,*AR2++
    // asm: 	NOP	*AR6--(9)
    // asm: 	B	CDLP
    // *STUFF REAR WHEEL
CARRWHL:
    // asm: 	LDF	*AR3++,R0
    // asm: 	RPTS	7
    // asm: 	LDF	*AR3++,R0
    // asm:  	STF	R0,*AR2++
    // asm: 	NOP	*AR3--(9)
CDLP:
    // asm: 	LDI	*AR0,R0
    // asm: 	BNZ	CDTOP
    // asm: 	LDI	3,AR2	  		;SLEEP TIME
    // asm: 	B	CARSLP
    // *HANDLE BODY
    // *BODY MUST BE LAST
CARBODY:
    // asm: 	CALL	LEAN
    // asm: 	LDI	1,AR2
CARSLP:
    // asm: 	CALL	SLEEP
    // asm: 	B 	CARPROCL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARPROC", 0, 0);
    UNIMPL();
}

void LEAN(void)
{
    // *
    // *LEAN BODY IN Z
    // *
    // *PARAMETERS
    // *	AR0	DYNA OBJECT
    // *	AR4	MASTER OBJECT
    // *	AR5	CAR BLOCK
    // *	AR6	DYNAMATRIX
    // *	R4	OLD ORADY
    // *	R5	Z RADIANS
    // *	R6	X RADIANS
    // *	R7	OLD SPEED
    // *	PDATA	OLD CAR ORADY
    // *	PDATA+1 BODY LEAN Z RADIANS
    // *
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	DYNAMATRIX,AR1
    // 	;GET X LEAN (BRAKE/ACCEL)
    // 	;
    // asm: 	LDF	R7,R0
    // asm: 	LDF	*+AR5(CARSPEED),R7	;GET NEW SPEED
    // asm: 	SUBF	R0,R7,R0
    // asm: 	MPYF	0.06,R0			;CONVERT TO RADIANS
    // asm: 	ADDF	R0,R6
    // asm: 	MPYF	0.25,R6
    // asm: 	NEGF	R6,R2
    // asm: 	LDI	*+AR5(CAR_AIRF),R0
    // asm: 	OR	*+AR5(CAR_AIRB),R0
    // asm: 	LDFNZ	0,R2 			;ZERO OUT WHEN IN AIR FOLKS
    // asm: 	LDF	R2,R2			;AMPLIFY ACCELERATION ONLY
    // asm: 	LDFGT	1,R1
    // asm: 	LDFLT	2,R1
    // asm: 	MPYF	R1,R2
    // asm: 	LDF	*+AR5(CARRPM),R0      	;REV FACTOR
    // asm: 	MPYF	0.01,R0
    // asm: 	MPYF	-0.05,R0
    // asm: 	ADDF	R0,R2
    // asm: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm: 	LDFGT	0.1,R2
    // asm: 	CMPF	-0.1,R2
    // asm: 	LDFLT	-0.1,R2
    // asm: 	STF	R2,*+AR5(CARXLEAN)
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	AR2,AR0			;SAVE MATRIX PTR
    // 	;GET YOUR Z LEAN (CORNERING)
    // 	;
    // asm: 	LDF	*+AR7(PDATA),R4		;OLD ORADY
    // asm: 	LDF	*+AR7(PDATA+1),R5	;Z RADIANS
    // asm: 	LDF	*+AR4(ORADY),R0
    // asm: 	STF	R0,*+AR7(PDATA)		;SAVE NEW OLD ORADY
    // asm: 	SUBF	R4,R0			;DELTA ORADY
    // asm: 	LDF	0,R1
    // asm: 	CMPF	3.14,R0
    // asm: 	LDFGT	@NTWOPII,R1
    // asm: 	CMPF	-3.14,R0
    // asm: 	LDFLT	@TWOPII,R1
    // asm: 	ADDI	R1,R0			;HANDLE RADIAN WRAPAROUND
    // asm: 	MPYF	R7,R0			;MULTIPLY BY SPEED FACTOR
    // asm: 	MPYF	0.06,R0			;CONVERT TO RADIANS
    // asm: 	MPYF	0.1,R0			;CONVERT TO RADIANS
    // asm: 	ADDF	R0,R5
    // asm: 	MPYF	0.5,R5
    // asm: 	STF	R5,*+AR7(PDATA+1)	;SAVE NEW Z RADIANS
    // asm: 	NEGF	R5,R2
    // asm: 	LDI	*+AR5(CAR_AIRF),R0
    // asm: 	OR	*+AR5(CAR_AIRB),R0
    // asm: 	LDFNZ	0,R2 			;ZERO OUT WHEN IN AIR FOLKS
    // asm: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm: 	LDFGT	0.1,R2
    // asm: 	CMPF	-0.1,R2
    // asm: 	LDFLT	-0.1,R2
    // asm: 	STF	R2,*+AR5(CARZLEAN)    	;SAVE IT
    // ;	MPYF	3,R2			;PUMP IT UP
    // asm: 	MPYF	2.2,R2			;PUMP IT UP
    // asm: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm: 	LDFGT	0.1,R2
    // asm: 	CMPF	-0.1,R2
    // asm: 	LDFLT	-0.1,R2
    // asm: 	LDI	@MATRIXAI,AR2		;GET Z IN TEMP THING
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	CALL	CONCAT201    		;CONCAT YOUR MATRICES INTO DYNOBJ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEAN", 0, 0);
    UNIMPL();
}

void DYNAOBJ_INIT(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	RC
    // asm: 	PUSH	RS
    // asm: 	LDI	@DYNALISTI,AR2
    // asm: 	LDI	@DYNAFREEI,R2
    // asm: 	LDI	@NULLI,R3
    // asm: 	LDI	NUM_DYNAS-1,RC
    // asm: 	LDI	DYNASIZE,RS
    // asm: 	CALL	INIT_LINKED_LIST
    // asm: 	POP	RS
    // asm: 	POP	RC
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DYNAOBJ_INIT", 0, 0);
    UNIMPL();
}

void GETDYNA(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET A DYNAMIC OBJECT
    // *
    // *RETURNS
    // *	(SUCCESSFUL)
    // *		CARRY SET
    // *		AR0	POINTER TO DYNA OBJ
    // *	(UNSUCCESSFUL)
    // *		CARRY CLEAR
    // *
    // *
    // asm: 	PUSH	R0
    // ;	LDP	@DYNAFREE
    // asm: 	LDI	@DYNAFREE,R0
    // asm: 	LDI	R0,AR0
    // asm: 	SLOCKON	Z,"UTIL\GETDYNA   out of dynamic objects"
    // asm: 	BZ	GETDYNA_ERR
    // asm: 	LDI	*AR0,R0
    // asm: 	STI	R0,@DYNAFREE
    // asm: 	ADDI	DYNAMATRIX,AR0		;INIT YOUR MATRIX FOLKS
    // asm: 	CALL	INITMAT
    // asm: 	SUBI	DYNAMATRIX,AR0
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR0(DYNAFLAG)
    // asm: 	SETC
GETDYNA_X:
    // asm: 	POP	R0
    // asm: 	RETS
GETDYNA_ERR:
    // asm: 	CLRC
    // asm: 	B	GETDYNA_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETDYNA", 0, 0);
    UNIMPL();
}

void DELDYNA(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DELETE A DYNA OBJECT
    // *
    // *PARAMETERS
    // *	AR2	POINTER TO DYNA OBJ
    // *
    // *
    // asm: 	PUSH	R0
    // ;	LDP	@DYNAFREE
    // asm: 	LDI	@DYNAFREE,R0
    // asm: 	STI	R0,*AR2
    // asm: 	STI	AR2,@DYNAFREE
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELDYNA", 0, 0);
    UNIMPL();
}

void CARB_INIT(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@CARFREEI,AR0
    // asm: 	LDI	@CARLISTI,AR1
    // asm: 	LDI	NUM_CARS-1,RC
    // asm: 	RPTB	CARINTL
    // asm: 	STI	AR1,*AR0
    // asm: 	LDI	AR1,AR0
CARINTL:
    // asm: ADDI	CARSIZ,AR1
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
    // asm: 	STPI	R0,@CAR_COUNT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARB_INIT", 0, 0);
    UNIMPL();
}

void GETCAR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET A CAR OBJECT
    // *
    // *RETURNS
    // *	(SUCCESSFUL)
    // *		AR0	POINTER TO CAR OBJ
    // *		CARRY SET
    // *	(UNSUCCESSFUL)
    // *		CARRY CLEAR
    // *
    // *
    // asm: 	PUSH	R0
    // ;	LDP	@CARFREE
    // asm: 	LDI	@CARFREE,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	SLOCKON	Z,"UTIL\GETCAR   out of cars"
    // asm: 	BZ	GETCAR_ERR
    // asm: 	LDI	*AR0,R0
    // asm: 	STI	R0,@CARFREE
    // asm: 	INCM	@CAR_COUNT
    // asm: 	SETC
GETCAR_X:
    // asm: 	POP	R0
    // asm: 	RETS
GETCAR_ERR:
    // asm: 	CLRC
    // asm: 	B	GETCAR_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCAR", 0, 0);
    UNIMPL();
}

void DELCAR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DELETE A CAR OBJECT
    // *
    // *PARAMETERS
    // *	AR2	POINTER TO CAR OBJ
    // *
    // asm: 	PUSH	R0
    // ;	LDP	@CARFREE
    // asm: 	LDI	@CARFREE,R0
    // asm: 	STI	R0,*AR2
    // asm: 	STI	AR2,@CARFREE
    // ;	LDP	@CAR_COUNT
    // asm: 	LDI	@CAR_COUNT,R0
    // asm: 	DEC	R0
    // asm: 	SLOCKON	LT,"UTIL\DELCAR   erroneous CAR_COUNT"
    // asm: 	STI	R0,@CAR_COUNT
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELCAR", 0, 0);
    UNIMPL();
}

void SCAN_OBJECTS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	ISCAN
    // asm: 	SLEEP	1
    // asm: 	CALL	OSCAN
    // asm: 	SLEEP	1
    // asm: 	B	SCAN_OBJECTS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCAN_OBJECTS", 0, 0);
    UNIMPL();
}

void PUSHALL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	POP	BK
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R1
    // asm: 	PUSHF	R2
    // asm: 	PUSHF	R3
    // asm: 	PUSHF	R4
    // asm: 	PUSHF	R5
    // asm: 	PUSHF	R6
    // asm: 	PUSHF	R7
    // asm: 	BU	BK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PUSHALL", 0, 0);
    UNIMPL();
}

void POPALL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	POP	BK
    // asm: 	POPF	R7
    // asm: 	POPF	R6
    // asm: 	POPF	R5
    // asm: 	POPF	R4
    // asm: 	POPF	R3
    // asm: 	POPF	R2
    // asm: 	POPF	R1
    // asm: 	POPF	R0
    // asm: 	POP	R7
    // asm: 	POP	R6
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR7
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	BU	BK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POPALL", 0, 0);
    UNIMPL();
}

void DISTANCE_2D(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	X = sin(T)
    // *	Y = cos(T)
    // *	R2	(FL) THETA
    // *	R0	(FL) DISTANCE
    // *RETURNS
    // *	R0	(FL) X DISTANCE
    // *	R1	(FL) Z DISTANCE
    // *
    // *
    // asm: 	PUSHFL	R3
    // asm: 	LDF	R0,R1
    // asm: 	NEGF	R0,R3
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R0,R1
    // asm: 	CALL	_SINE
    // asm: 	MPYF	R3,R0
    // asm: 	POPFL	R3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISTANCE_2D", 0, 0);
    UNIMPL();
}

void OVELADD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *ADD OBJECT VELOCITIES
    // *
    // *PARAMETERS
    // *	AR4	OBJECT
    // *
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OVELY),R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OVELZ),R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OVELADD", 0, 0);
    UNIMPL();
}

void OVELNADD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *ADD N FRAMES X VELOCITY
    // asm: 	FLOATP	@NFRAMES,R1
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OVELY),R0
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OVELZ),R0
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OVELNADD", 0, 0);
    UNIMPL();
}

void FORWARD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *MOVE FORWARD
    // *PARAMETERS
    // *	AR2	MATRIX
    // *	R2	DIST
    // *	R3	DESTINATION VECTOR
    // asm: 	PUSH	AR2
    // asm: 	LDF	0,R0
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R2
    // asm: 	LDI	AR2,R2
    // asm: 	LDI	SP,AR2
    // asm: 	SUBI	2,AR2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	POPF	R2
    // asm: 	SUBI	2,SP
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FORWARD", 0, 0);
    UNIMPL();
}

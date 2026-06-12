#include "../core/cpu.h"
#include "../core/machine.h"
#include "vunit.h"
#include "mproc.h"
#include "pall.h"
#include "objects.h"
#include "sys.h"
#include "macs.h"
#include "error.h"
#include "globals.h"
#include "pall_defs.h"
#include "discovered_defines.h"

/*
 * Source module: asm/PALL.ASM
 */

void PAL_INIT(void);
void PAL_XFER(void);
void NOTCLR(void);
void PALTR0(void);
void PACBLK(void);
void NOT_PACKED_PAL(void);
void REGDOIT(void);
void PALTRX(void);
void PAL_OVERWRITE(void);
void PAL_FIND(void);
void FPLXEX(void);
void PAL_FIND_RAW(void);
void FOUNDRAW(void);
void PAL_DELETE_RAW(void);
void PAL_ALLOC(void);
void GPL0(void);
void GETPL(void);
void PAL_ALLOC_RAW(void);
void RAWPL(void);
void PAL_SET(void);
void PALXFER_INIT(void);
void PALXFER_GET(void);
void PALXFER_DEL(void);
void PAL_DELETE(void);
void PAL_DIMMER(void);

/* asm: PALRAM	.bss	PALRAM,PALNUM */
int PALRAM[PALNUM];
/* asm: RAWLOCS	.bss	RAWLOCS,PALNUM */
int RAWLOCS[PALNUM];
/* asm: PTTRAM	.bss	PTTRAM,PALNUM*3 */
int PTTRAM[PALNUM*3];
/* asm: NUM_FIXED	.bss	NUM_FIXED,1 */
int NUM_FIXED;
/* *----------------------------------------------------------------------------
*INDEX STORAGE
 */
/* asm: PALROMI	.word	_PALROM		;INDEXED PALETTES SOURCE ADDR LIST */
#define PALROMI _PALROM
/* asm: PALLISTI	.word	_PALLIST	;CROSS-REFERENCE LIST */
#define PALLISTI _PALLIST
/* *----------------------------------------------------------------------------
*PALTRANS- TRANSFER PALETTE DATA TO COLORRAM
*	CALL FROM DISPLAY IRQ AT BEGIN OF VBLNK
*
*CLOBBERS
*	R0,R1,R7,AR0,AR1,AR2
*
 */
/* asm: PALSXFER	.bss	PALSXFER,1 */
int PALSXFER;
/* asm: COLRAML	.word	COLORAM */
int COLRAML = COLORAM;
/* asm: COLRAMH	.word	COLORAM+7FFFh */
int COLRAMH = COLORAM+0x7FFF;
#endif
/* *STRUCT PALXFER
 */
#define PALX_LINK 0 //UH
#define PALX_SADDR 1 //UH
#define PALX_DADDR 2 //UH
#define PALX_COUNT 3 //UH
#define PALX_SIZE 4 //SIZ
#define NXFER_PALS 128
/* asm: PALXFER_ACTIVE	.bss	PALXFER_ACTIVE,1 */
int PALXFER_ACTIVE;
/* asm: PALXFER_FREE	.bss	PALXFER_FREE,1 */
int PALXFER_FREE;
/* asm: PALXFER_AVAILABLE_P	.bss	PALXFER_AVAILABLE_P,1 */
int PALXFER_AVAILABLE_P;
/* asm: PALXFER_STR	.bss	PALXFER_STR,PALX_SIZE*NXFER_PALS */
int PALXFER_STR[PALX_SIZE*NXFER_PALS];

/* *----------------------------------------------------------------------------
*CLEAR OUT PALETTE RAM
*
*
 */
void PAL_INIT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // 	;CLEAR OUT PALETTE TRANSFER RAM
    // asm: 	LDP	@PTTRAMI
    // asm: 	LDI	@PTTRAMI,AR0
    // asm: 	LDI	0,R0
    // asm: 	RPTS	(PALNUM*3)-1
    // asm: 	STI	R0,*AR0++
    // 	;CLEAR OUT PALETTE ALLOCATION RAM
    // asm: 	LDP	@PALRAMI
    // asm: 	LDI	@PALRAMI,AR0
    // asm: 	RPTS	PALNUM-1
    // asm: 	STI	R0,*AR0++
    // 	;CLEAR OUT RAW PALETTE LOOKUP LIST
    // asm: 	LDP	@RAWLOCSI
    // asm: 	LDI	@RAWLOCSI,AR0
    // asm: 	RPTS	PALNUM-1
    // asm: 	STI	R0,*AR0++
    // 	;CLEAR OUT PALETTE CROSS-REF LIST
    // asm: 	LDP	@PALLISTI
    // asm: 	LDI	@PALLISTI,AR0
    // asm: 	RPTS	num_palettes-1
    // asm: 	STI	R0,*AR0++
    // asm: 	CALL	PALXFER_INIT
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_INIT", 0, 0);
    UNIMPL();
}

void PAL_XFER(void)
{
    // asm: 	CLRI	R7
    // asm: 	LDP	@PALXFER_ACTIVE
    // asm: 	LDI	@PALXFER_ACTIVE,R0
    // asm: 	BNE	NOTCLR
    // 	;NONE LEFT TO TRANSFER
    // asm: 	STI	R0,@PALXFER_AVAILABLE_P
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_XFER", 0, 0);
    UNIMPL();
}

void NOTCLR(void)
{
    // asm: 	LDI	@PALXFER_ACTIVE,R0
    // asm: 	B	I889
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTCLR", 0, 0);
    UNIMPL();
}

void PALTR0(void)
{
    // asm: 	INC	R7
    // asm: 	CMPI	12,R7
    // asm: 	BGT	PALTRX
    // asm: 	LDI	*AR0,R0		;NEXT XFER BLOCK
    // asm: 	CALL	PALXFER_DEL
I889:
    // asm: LDI	R0,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BZ	PALTRX
    // asm: 	LDI	*+AR0(PALX_COUNT),R0
    // ;	BZ	PALTRX		;COUNT=0,GAME OVER DONE
    // asm: 	BNN	NOT_PACKED_PAL
    // 	;THIS IS A PACKED PALETTE STORED AS:  16BITS|(16BITS<<16)
    // 	;UNPACK ON THE FLY
    // asm: 	LS	1,R0		;remove high bit
    // asm: 	RS	2,R0		;divide by 2
#if DEBUG
    // asm: 	CMPI	256,R0
    // asm: 	SLOCKON	GT,"PALL\PALTRANS  more than 256 entries?"
#endif
    // ;	STI	R1,*AR0++		;CLEAR OUT COUNT
    // asm: 	LDI	*+AR0(PALX_SADDR),AR1	;GET SOURCE
    // asm: 	LDI	*+AR0(PALX_DADDR),AR2	;GET DESTINATION
    // ;	LDI	*AR0++,AR1		;GET SOURCE
    // ;	LDI	*AR0++,AR2		;GET DESTINATION
#if DEBUG
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PALTR0", 0, 0);
    UNIMPL();
}

void PACBLK(void)
{
    // asm: STI	R2,*AR2++	;SECOND COLOR
    // asm: 	B     	PALTR0		;LOOK FOR NEXT TRANSFER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PACBLK", 0, 0);
    UNIMPL();
}

void NOT_PACKED_PAL(void)
{
    // ;	STI	R1,*AR0++	;CLEAR OUT COUNT
    // asm: 	LDI	*+AR0(PALX_SADDR),AR1	;GET SOURCE
    // asm: 	LDI	*+AR0(PALX_DADDR),AR2	;GET DESTINATION
    // ;	LDI	*AR0++,AR1	;GET SOURCE
    // ;	LDI	*AR0++,AR2	;GET DESTINATION
#if DEBUG
    // asm: 	CMPI	@COLRAML,AR2
    // asm: 	SLOCKON	LT,"PALL\PALTRANS SETUP XFER OUT OF CRAM LT 2"
    // asm: 	CMPI	@COLRAMH,AR2
    // asm: 	SLOCKON	GT,"PALL\PALTRANS SETUP XFER OUT OF CRAM GT 2"
#endif
    // asm: 	SUBI	2,R0		;DEC COUNT BY 1
    // asm: 	BNN	REGDOIT
    // asm: 	LDI	*AR1++,R2	;single case
    // asm: 	STI	R2,*AR2++
    // asm: 	B	PALTR0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOT_PACKED_PAL", 0, 0);
    UNIMPL();
}

void REGDOIT(void)
{
    // asm: 	LDI	*AR1++,R2
    // asm: 	RPTS	R0
    // asm: 	LDI	*AR1++,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	B     	PALTR0		;LOOK FOR NEXT TRANSFER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REGDOIT", 0, 0);
    UNIMPL();
}

void PALTRX(void)
{
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PALTRX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	R0	PAL TO OVERWRITE
*	R1	SOURCE PAL
*
*
 */
void PAL_OVERWRITE(void)
{
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	SLOCKON	C,"PALL\PAL_OVERWRITE  FINDPAL FAILURE"
    // asm: 	RETSC
    // asm: 	LDP	@PALROMI
    // asm: 	LDI	R1,AR2
    // asm: 	ADDI	@PALROMI,AR2
    // asm: 	LDI	*AR2,AR2
    // 	;SETUP TRANSFER
    // asm: 	LDI	*AR2++,R3	;GET COUNT
    // asm: 	LDI	R0,R2		;GET DESTINATION
    // asm: 	CALL	PAL_SET
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_OVERWRITE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*FIND A PALETTE
*
*PARAMETERS
*	AR2	PALETTE INDEX
*RETURNS
*	R0	COLORAM CODE
*	CARRY SET IF PALETTE NOT FOUND
*
 */
void PAL_FIND(void)
{
    // asm: 	PUSH	AR2
    // asm: 	SETC		 	;ASSUME PALETTE NOT FOUND
    // asm: 	LDP	@PALLISTI
    // asm: 	ADDI	@PALLISTI,AR2
    // asm: 	LDI	*AR2,R0
    // asm: 	BZ	FPLXEX
    // asm: 	LSH	-16,R0
    // asm: 	LSH	8,R0
    // asm: 	CLRC	    		;WE FOUND IT DUDES...
    // asm: FPLX
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_FIND", 0, 0);
    UNIMPL();
}

void FPLXEX(void)
{
    // ;edbg
    // ;	BU	$
    // asm: 	SETC
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FPLXEX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*FIND RAW PALETTE
*
*Raw palettes are not allocated the same as regular polygon palettes
*(they are not part of the indexing system)  Therefore a different
*method has been used for their allocation.  This simply reverses the
*process and finds out where we put the dadeo.
*PARAMETERS
*	AR2	RAW PALETTE ADDRESS
*RETURNS
*	NOT FOUND
*		CARRY CLEAR
*	FOUND
*		CARRY SET
*		R0	PALLETTE CODE
*
 */
void PAL_FIND_RAW(void)
{
    // asm: 	PUSH	AR0
    // asm: 	LDI	-1,R0
    // asm: 	LDP	@RAWLOCSI
    // asm: 	LDI	@RAWLOCSI,AR0
FINDRLP:
    // asm: ADDI	1,R0
    // asm: 	CMPI	AR2,*AR0++
    // asm: 	BEQ	FOUNDRAW
    // asm: 	CMPI	PALNUM,R0
    // asm: 	BLE	FINDRLP
    // asm: 	LDI	-1,R0
    // ;	.if	DEBUG
    // ;edbg	;DBG DBG
    // ;	BU	$
    // ;	;DBG
    // ;	.endif
    // asm: 	CLRC
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_FIND_RAW", 0, 0);
    UNIMPL();
}

void FOUNDRAW(void)
{
    // asm: 	LSH	8,R0
    // asm: 	SETC
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FOUNDRAW", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
*PARAMETERS
*	AR2	ACTUAL PALETTE INDEX
*		(16 BIT)
*
 */
void PAL_DELETE_RAW(void)
{
    // asm: 	PUSH	AR0
    // asm: 	RS	8,AR2
    // asm: 	LDP	@RAWLOCSI
    // asm: 	LDI	@RAWLOCSI,AR0
    // asm: 	ADDI	AR2,AR0
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*AR0
    // ;edbg
    // ;	CMPI	0,AR2
    // ;	BEQ	$
    // ;edbg
    // asm: 	LDP	@PALRAMI
    // asm: 	LDI	@PALRAMI,AR0
    // asm: 	ADDI	AR2,AR0
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*AR0
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_DELETE_RAW", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GET A PALETTE
*
*PARAMETERS
*	AR2	PALETTE SOURCE INDEX
*RETURNS
*	R0	PALETTE CODE
*	LOCKUP ON ERROR
*
 */
void PAL_ALLOC(void)
{
    // asm: 	PUSHM	R2,R3,AR1,AR0,AR2
    // ;	.if	DEBUG
    // ;	CMPI	256,AR2
    // ;	SLOCKON	GT,"PALL\GETPAL	next RAM loc??? find out"
    // ;	.endif
    // 	;LOOK IF ALREADY ALLOCATED
    // asm: 	LDI	@PALLISTI,AR1
    // asm: 	ADDI	AR2,AR1
    // asm: 	LDI	*AR1,R0	     	;ALREADY ALLOCATED?
    // asm: 	BZ	GPL0		;NO...
    // asm: 	ADDI	1,R0		;YES, INCREMENT AND RETURN
    // asm: 	STI	R0,*AR1
    // asm: 	B	GPLX		;RETURN...
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_ALLOC", 0, 0);
    UNIMPL();
}

void GPL0(void)
{
    // 	;FIND A FREE ONE
    // asm: 	LDP	@PALRAMI	 	;LOOK FOR FREE CELL
    // asm: 	LDI	@PALRAMI,AR0
    // asm: 	LDI	PALNUM-1,RC
    // asm: 	LDI	*AR0++,R0 	;GET FIRST ONE
    // asm: 	RPTB	GPLP
    // asm: 	BZ	GETPL		;GOT A EMPTY
GPLP:
    // asm: LDI	*AR0++,R0
    // asm: GPERR
    // ;	SLOCKON	U,"PALL\GETPAL   ERROR NONE LEFT"
    // asm: 	ERRON	U,77h
    // ;edbg	.if	DEBUG
    // ;	BU	$
    // ;	.endif
    // asm: 	B	GPLX
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GPL0", 0, 0);
    UNIMPL();
}

void GETPL(void)
{
    // asm: 	LDI	AR2,R2
    // asm: 	LDP	PALROMI
    // asm: 	ADDI	@PALROMI,AR2
    // asm: 	LDI	*AR2,AR2	;NOW HOLDS RAM LOCATION
    // asm: 	OR	8000H,R2	;MAKE SURE A BIT IS SET
    // asm: 	STI	R2,*-AR0(1)	;MARK PALETTE AS TAKEN
    // asm: 	SUBI	PALNUM-1,RC	;GET PALETTE CODE
    // asm: 	NEGI	RC,R0
    // asm: 	LSH	16,R0
    // asm: 	ADDI	1,R0		;INC COUNT
    // asm: 	STI	R0,*AR1
    // 	;SETUP TRANSFER
    // asm: 	LDI	*AR2++,R3	;GET COUNT
    // asm: 	LDI	R0,R2		;GET DESTINATION
    // asm: 	LSH	-16,R2
    // asm: 	LSH	8,R2
    // asm: 	CALL	PAL_SET
    // asm: 	SUBI	1,AR2		;RESTORE AR2
GPLX:
    // asm: 	LSH	-16,R0		;SHIFT DOWN CODE
    // asm: 	LSH	8,R0
    // asm: 	POPM	AR2,AR0,AR1,R3,R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETPL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PAL_ALLOC_RAW
*THIS ROUTINE IS FOR USE WHEN YOU HAVE A SELF-CREATED PALETTE THAT
*YOU NEED TO ALLOCATE INTO MEMORY, BUT NOT USE THE PCOMP INDEXING SYSTEM
*
*PARAMETERS
*	AR2	PALETTE SOURCE ADDRESS
*RETURNS
*	R0	PALETTE CODE
*	LOCKUP ON ERROR
*
 */
void PAL_ALLOC_RAW(void)
{
    // asm: 	PUSHM	R2,R3,AR1,AR0,AR2
    // 	;FIND A FREE ONE
    // asm: 	LDP	@PALRAMI	 	;LOOK FOR FREE CELL
    // asm: 	LDI	@PALRAMI,AR0
    // asm: 	LDI	PALNUM-1,RC
    // asm: 	LDI	*AR0++,R0 	;GET FIRST ONE
    // asm: 	RPTB	RPLP
    // asm: 	BZ	RAWPL		;GOT A EMPTY
RPLP:
    // asm: LDI	*AR0++,R0
    // asm: 	ERRON	U,78h
    // ;	SLOCKON	U,"PALL\RAWPAL  ERROR... NONE LEFT"
    // ;edbg	.if	DEBUG
    // ;	BU	$
    // ;	.endif
    // asm: 	B	RAWPEX		;UNTIL WE COME UP WITH A BETTER IDEA
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_ALLOC_RAW", 0, 0);
    UNIMPL();
}

void RAWPL(void)
{
    // asm: 	LDI	-1,R2
    // asm: 	STI	R2,*-AR0(1)	;MARK PALETTE AS TAKEN
    // asm: 	SUBI	PALNUM-1,RC	;GET PALETTE CODE
    // asm: 	NEGI	RC,R0
    // asm: 	LSH	16,R0
    // asm: 	ADDI	1,R0		;INC COUNT
    // 	;SETUP TRANSFER
    // asm: 	LDI	*AR2++,R3	;GET COUNT
    // asm: 	LDI	R0,R2		;GET DESTINATION
    // asm: 	LSH	-16,R2
    // asm: 	LSH	8,R2
    // asm: 	CALL	PAL_SET
    // asm: 	SUBI	1,AR2		;RESTORE AR2
RPLX:
    // asm: LSH	-16,R0
    // asm: 	LDI	R0,AR0
    // asm: 	LDP	@RAWLOCSI
    // asm: 	ADDI	@RAWLOCSI,AR0
    // asm: 	STI	AR2,*AR0
RAWPEX:
    // asm: 	LSH	8,R0
    // asm: 	POPM	AR2,AR0,AR1,R3,R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAWPL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SETUP PALETTE TRANSFER
*
*PARAMETERS
*	AR2	SOURCE DATA ADDRESSS
*	R2	DEST PALETTE(B8-15), DEST COLOR(B0-7)
*	R3	COUNT
*
 */
void PAL_SET(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // ;	LDI	2000h,R0
    // ;	CMPI	R2,R0
    // ;	BEQ	$	;... found it
    // ;edbg
    // ;	LDI	R2,R0
    // ;	RS	8,R0
    // ;	CMPI	0,R0
    // ;	BEQ	$
    // ;edbg
    // asm: 	DINT
    // asm: 	CALL	PALXFER_GET
    // asm: 	STI	AR2,*+AR0(PALX_SADDR)	;SAVE SOURCE ADDR
    // asm: 	LDI	COLORAM>>16,RC		;ADD IN COLORAM ADDRESS
    // asm: 	LSH	16,RC			;SHIFT IT IN PLACE
    // asm: 	ADDI	R2,RC			;CONVERTED TO ADDRESS
    // asm: 	STI	RC,*+AR0(PALX_DADDR)	;SAVE DEST ADDR
    // asm: 	STI	R3,*+AR0(PALX_COUNT)	;SAVE WORD COUNT
    // asm: 	CALL	ENABLEGIE
#if DEBUG
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	CMPI	@COLRAML,RC
    // asm: 	ERRON	LT,88
    // ;	SLOCKON	LT,"PALL\PALTRANS SETUP XFER OUT OF CRAM LT"
    // asm: 	CMPI	@COLRAMH,RC
    // asm: 	ERRON	GT,88
    // ;	SLOCKON	GT,"PALL\PALTRANS SETUP XFER OUT OF CRAM GT"
#endif
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_SET", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void PALXFER_INIT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@PALXFER_ACTIVE
    // asm: 	LDI	@PALXFER_FREEI,AR0	 	;GET FREE POINTER
    // asm: 	LDI	@PALXFER_STRI,AR1
    // asm: 	LDI	NXFER_PALS-1,RC
    // asm: 	RPTB	PXIL
    // asm: 	STI	AR1,*AR0
    // asm: 	LDI	AR1,AR0
PXIL:
    // asm: ADDI	PALX_SIZE,AR1
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PALXFER_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ALLOCATE A PALETTE XFER BLOCK
*
*RETURNS
*	AR0	PTR TO AVAILABLE XFER BLOCK
*
 */
void PALXFER_GET(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@PALXFER_AVAILABLE_P
    // asm: 	LDI	@PALXFER_FREE,AR0
#if DEBUG
    // asm: 	CMPI	0,AR0
    // asm: 	BNE	CNT
    // asm: 	EINT
    // asm: 	BU	$
CNT:
#endif
    // asm: 	LDI	*+AR0(PALX_LINK),R0
    // asm: 	STI	R0,@PALXFER_FREE
    // asm: 	LDI	@PALXFER_ACTIVE,R0
    // asm: 	STI	R0,*+AR0(PALX_LINK)
    // asm: 	STI	AR0,@PALXFER_ACTIVE
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PALXFER_GET", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*FREE A PALETTE XFER BLOCK
*
*PARAMETERS
*	AR0	PTR TO XFER BLOCK TO FREE
*
 */
void PALXFER_DEL(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR1
    // asm: 	LDI	@PALXFER_ACTIVEI,R1
PXFLP:
    // asm: LDI	R1,AR1
    // asm: 	LDI	*AR1,R1
#if DEBUG
    // asm: 	BZ	$
#endif
    // asm: 	BZ	PXXX
    // asm: 	CMPI	R1,AR0
    // asm: 	BNE	PXFLP
    // asm: 	LDI	*AR0,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	@PALXFER_FREE,R1
    // asm: 	STI	R1,*AR0
    // asm: 	STI	AR0,@PALXFER_FREE
PXXX:
    // asm: 	POP	AR1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PALXFER_DEL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*DELETE A PALETTE
*
*PARAMETERS
*	AR2	PALETTE INDEX
*	LOCKUP ON ERROR
*
 */
void PAL_DELETE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // ;edbg
    // ;	CMPI	0,AR2
    // ;	BEQ	$
    // ;edbg
    // 	;DELETE FROM PALETTE LIST AREA
    // asm: 	LDP	@PALLISTI
    // asm: 	LDI	@PALLISTI,AR0
    // asm: 	ADDI	AR2,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	BZ	DELP2
    // asm: 	RS	16,R0
    // asm: 	LS	16,R0
    // asm:      	STI	R0,*AR0
    // asm: 	LDI	R0,RC
    // asm: 	LSH	16,R0
    // asm: 	BNZ	DELP2	;palette not found ignore...
    // 	;CLEAR OUT PALRAM, LIST ENTRY IF COUNT IS ZERO
DELP1:
    // asm: LDI	0,R0
    // asm: 	STI	R0,*AR0		;CLEAR OUT LIST ENTRY
    // asm: 	LDP	@PALRAMI
    // asm: 	LDI	@PALRAMI,AR0
    // asm: 	LSH	-16,RC
    // asm: 	ADDI	RC,AR0
    // asm: 	STI	R0,*AR0		;ZERO OUT PALRAM AREA
DELP2:
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_DELETE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PALETTE DIMMER
*
*PARAMETERS
*	AR0	SOURCE PALETTE
*	AR1	RAM BUFFER
*	R0	(FL) UNIT NUMBER
*OPERATION
*	READS DATA IN FROM SOURCE PALETTE
*	MULTIPLIES BY UNIT NUMBER
*	STORES DATA IN RAM BUFFER
*	CALLS PAL_SET APPROPRIATELY
*	(ASSUMES RAM BUFFER IS ALREADY ALLOCATED
*	 AS A PALETTE)
*
*
 */
void PAL_DIMMER(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSHFL	R6
    // asm: 	PUSHFL	R7
    // asm: 	PUSH	AR1
    // asm: 	LDF	R0,R7
    // asm: 	LDI	*AR0++,R0
    // asm: 	STI	R0,*AR1++
    // asm: 	LDI	127,AR5
    // asm: PDMLP
    // asm: 	LDI	*AR0++,R4	;get src2
    // asm: 	LDI	R4,R1
    // asm: 	AND	01Fh,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R7,R1
    // asm: 	FIX	R1
    // asm: 	LDI	R4,R2		;--
    // asm: 	RS	5,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	MPYF	R7,R2
    // asm: 	FIX	R2
    // asm: 	LS	5,R2
    // asm: 	OR	R2,R1
    // asm: 	LDI	R4,R2		;--
    // asm: 	RS	10,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	MPYF	R7,R2
    // asm: 	FIX	R2
    // asm: 	LS	10,R2
    // asm: 	OR	R2,R1
    // asm: 	PUSH	R1
    // 	;----second word----------
    // asm: 	RS	16,R4		;get src1(B)
    // asm: 	LDI	R4,R1
    // asm: 	AND	01Fh,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R7,R1
    // asm: 	FIX	R1
    // asm: 	LDI	R4,R2
    // asm: 	RS	5,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	MPYF	R7,R2
    // asm: 	FIX	R2
    // asm: 	LS	5,R2
    // asm: 	OR	R2,R1
    // asm: 	LDI	R4,R2
    // asm: 	RS	10,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	MPYF	R7,R2
    // asm: 	FIX	R2
    // asm: 	LS	10,R2
    // asm: 	OR	R2,R1
    // asm: 	POP	R2
    // asm: 	LS	16,R1
    // asm: 	OR	R2,R1
    // asm: 	STI	R1,*AR1++
    // asm: 	DBU	AR5,PDMLP
    // asm: 	POP	AR2			;RECOVER RAM BUFFER ADDR
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	*AR2++,R3
    // asm: 	LDI	R0,R2
    // asm: 	CALL	PAL_SET
    // asm: 	POPFL	R7
    // asm: 	POPFL	R6
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_DIMMER", 0, 0);
    UNIMPL();
}

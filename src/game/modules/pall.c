#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/vunit.h"
#include "../include/mproc.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/sys.h"
#include "../include/macs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/pall_defs.h"

/*
 * Source module: asm/PALL.ASM
 */

// *----------------------------------------------------------------------------
// *PALETTE ALLOCATION SYSTEM
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *
// *PALETTE MANAGEMENT ROUTINES
// *PALETTES ARE ALLOCATED IN 128 BLOCKS OF 256 COLORS
// *
// *----------------------------------------------------------------------------
// *INDEX STORAGE
/* asm: PALROMI	.word	_PALROM		;INDEXED PALETTES SOURCE ADDR LIST */
int PALROMI = (int)(_PALROM);
/* asm: PTTRAMI	.word	PTTRAM		;PALETTE TRANSFER RAM */
int PTTRAMI = (int)(PTTRAM);
/* asm: PALRAMI	.word	PALRAM		;INDEXED PALETTE ACTIVE LOCATION LIST */
int PALRAMI = (int)(PALRAM);
/* asm: PALLISTI	.word	_PALLIST	;CROSS-REFERENCE LIST */
int PALLISTI = (int)(_PALLIST);
/* asm: RAWLOCSI	.word	RAWLOCS		;RAW LOCATION REFERENCE SPACE */
int RAWLOCSI = (int)(RAWLOCS);
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *CLEAR OUT PALETTE RAM
// *
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *PALTRANS- TRANSFER PALETTE DATA TO COLORRAM
// *	CALL FROM DISPLAY IRQ AT BEGIN OF VBLNK
// *
// *CLOBBERS
// *	R0,R1,R7,AR0,AR1,AR2
// *
// *----------------------------------------------------------------------------
// *STRUCT PALXFER
#define PALX_LINK 0
#define PALX_SADDR 1
#define PALX_DADDR 2
#define PALX_COUNT 3
#define PALX_SIZE 4
// *ENDSTRUCT
/* asm: PALXFER_ACTIVEI	.word	PALXFER_ACTIVE */
int PALXFER_ACTIVEI = (int)(PALXFER_ACTIVE);
/* asm: PALXFER_FREEI	.word	PALXFER_FREE */
int PALXFER_FREEI = (int)(PALXFER_FREE);
/* asm: PALXFER_STRI	.word	PALXFER_STR */
int PALXFER_STRI = (int)(PALXFER_STR);
#define NXFER_PALS 128
// *----------------------------------------------------------------------------

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
NOTCLR:
    // asm: 	LDI	@PALXFER_ACTIVE,R0
    // asm: 	B	I889
PALTR0:
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
    // asm: 	CMPI	@COLRAML,AR2
    // asm: 	SLOCKON	LT,"PALL\PALTRANS SETUP XFER OUT OF CRAM LT"
    // asm: 	CMPI	@COLRAMH,AR2
    // asm: 	SLOCKON	GT,"PALL\PALTRANS SETUP XFER OUT OF CRAM GT"
#endif
    // asm: 	SUBI	1,R0		;DEC COUNT BY 1
    // asm: 	LDI	R0,RC
    // asm: 	RPTB	PACBLK
    // asm: 	LDI	*AR1++,R2
    // asm: 	STI	R2,*AR2++	;FIRST COLOR
    // asm: 	RS	16,R2
PACBLK:
    // asm: STI	R2,*AR2++
    // asm: 	B     	PALTR0		;LOOK FOR NEXT TRANSFER
NOT_PACKED_PAL:
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
REGDOIT:
    // asm: 	LDI	*AR1++,R2
    // asm: 	RPTS	R0
    // asm: 	LDI	*AR1++,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	B     	PALTR0		;LOOK FOR NEXT TRANSFER
PALTRX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_XFER", 0, 0);
    UNIMPL();
}

void PAL_OVERWRITE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *PARAMETERS
    // *	R0	PAL TO OVERWRITE
    // *	R1	SOURCE PAL
    // *
    // *
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

void PAL_FIND(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *FIND A PALETTE
    // *
    // *PARAMETERS
    // *	AR2	PALETTE INDEX
    // *RETURNS
    // *	R0	COLORAM CODE
    // *	CARRY SET IF PALETTE NOT FOUND
    // *
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
FPLXEX:
    // ;edbg
    // ;	BU	$
    // asm: 	SETC
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_FIND", 0, 0);
    UNIMPL();
}

void PAL_FIND_RAW(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *FIND RAW PALETTE
    // *
    // *Raw palettes are not allocated the same as regular polygon palettes
    // *(they are not part of the indexing system)  Therefore a different
    // *method has been used for their allocation.  This simply reverses the
    // *process and finds out where we put the dadeo.
    // *PARAMETERS
    // *	AR2	RAW PALETTE ADDRESS
    // *RETURNS
    // *	NOT FOUND
    // *		CARRY CLEAR
    // *	FOUND
    // *		CARRY SET
    // *		R0	PALLETTE CODE
    // *
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
FOUNDRAW:
    // asm: 	LSH	8,R0
    // asm: 	SETC
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_FIND_RAW", 0, 0);
    UNIMPL();
}

void PAL_DELETE_RAW(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // *PARAMETERS
    // *	AR2	ACTUAL PALETTE INDEX
    // *		(16 BIT)
    // *
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

void PAL_ALLOC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET A PALETTE
    // *
    // *PARAMETERS
    // *	AR2	PALETTE SOURCE INDEX
    // *RETURNS
    // *	R0	PALETTE CODE
    // *	LOCKUP ON ERROR
    // *
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
GPL0:
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
GETPL:
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_ALLOC", 0, 0);
    UNIMPL();
}

void PAL_ALLOC_RAW(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PAL_ALLOC_RAW
    // *THIS ROUTINE IS FOR USE WHEN YOU HAVE A SELF-CREATED PALETTE THAT
    // *YOU NEED TO ALLOCATE INTO MEMORY, BUT NOT USE THE PCOMP INDEXING SYSTEM
    // *
    // *PARAMETERS
    // *	AR2	PALETTE SOURCE ADDRESS
    // *RETURNS
    // *	R0	PALETTE CODE
    // *	LOCKUP ON ERROR
    // *
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
RAWPL:
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_ALLOC_RAW", 0, 0);
    UNIMPL();
}

void PAL_SET(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SETUP PALETTE TRANSFER
    // *
    // *PARAMETERS
    // *	AR2	SOURCE DATA ADDRESSS
    // *	R2	DEST PALETTE(B8-15), DEST COLOR(B0-7)
    // *	R3	COUNT
    // *
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

void PALXFER_INIT(void)
{
    // *----------------------------------------------------------------------------
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

void PALXFER_GET(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *ALLOCATE A PALETTE XFER BLOCK
    // *
    // *RETURNS
    // *	AR0	PTR TO AVAILABLE XFER BLOCK
    // *
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

void PALXFER_DEL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *FREE A PALETTE XFER BLOCK
    // *
    // *PARAMETERS
    // *	AR0	PTR TO XFER BLOCK TO FREE
    // *
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

void PAL_DELETE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DELETE A PALETTE
    // *
    // *PARAMETERS
    // *	AR2	PALETTE INDEX
    // *	LOCKUP ON ERROR
    // *
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

void PAL_DIMMER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PALETTE DIMMER
    // *
    // *PARAMETERS
    // *	AR0	SOURCE PALETTE
    // *	AR1	RAM BUFFER
    // *	R0	(FL) UNIT NUMBER
    // *OPERATION
    // *	READS DATA IN FROM SOURCE PALETTE
    // *	MULTIPLIES BY UNIT NUMBER
    // *	STORES DATA IN RAM BUFFER
    // *	CALLS PAL_SET APPROPRIATELY
    // *	(ASSUMES RAM BUFFER IS ALREADY ALLOCATED
    // *	 AS A PALETTE)
    // *
    // *
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

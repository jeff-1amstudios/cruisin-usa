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
void PAL_OVERWRITE(void);
void PAL_FIND(void);
void PAL_FIND_RAW(void);
void PAL_DELETE_RAW(void);
void PAL_ALLOC(void);
void PAL_ALLOC_RAW(void);
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
    // asm 00009EAB: 	PUSH	R0
    // asm 00009EAC: 	PUSH	AR0
    // 	;CLEAR OUT PALETTE TRANSFER RAM
    // asm 00009EAD: 	LDP	@PTTRAMI
    // asm 00009EAE: 	LDI	@PTTRAMI,AR0
    // asm 00009EAF: 	LDI	0,R0
    // asm 00009EB0: 	RPTS	(PALNUM*3)-1
    // asm 00009EB1: 	STI	R0,*AR0++
    // 	;CLEAR OUT PALETTE ALLOCATION RAM
    // asm 00009EB2: 	LDP	@PALRAMI
    // asm 00009EB3: 	LDI	@PALRAMI,AR0
    // asm 00009EB4: 	RPTS	PALNUM-1
    // asm 00009EB5: 	STI	R0,*AR0++
    // 	;CLEAR OUT RAW PALETTE LOOKUP LIST
    // asm 00009EB6: 	LDP	@RAWLOCSI
    // asm 00009EB7: 	LDI	@RAWLOCSI,AR0
    // asm 00009EB8: 	RPTS	PALNUM-1
    // asm 00009EB9: 	STI	R0,*AR0++
    // 	;CLEAR OUT PALETTE CROSS-REF LIST
    // asm 00009EBA: 	LDP	@PALLISTI
    // asm 00009EBB: 	LDI	@PALLISTI,AR0
    // asm 00009EBC: 	RPTS	num_palettes-1
    // asm 00009EBD: 	STI	R0,*AR0++
    // asm 00009EBE: 	CALL	PALXFER_INIT
    // asm 00009EBF: 	POP	AR0
    // asm 00009EC0: 	POP	R0
    // asm 00009EC1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_INIT", 0, 0);
    UNIMPL();
}

void PAL_XFER(void)
{
    // asm 00009EC2: 	CLRI	R7
    // asm 00009EC3: 	LDP	@PALXFER_ACTIVE
    // asm 00009EC4: 	LDI	@PALXFER_ACTIVE,R0
    // asm 00009EC5: 	BNE	NOTCLR
    // 	;NONE LEFT TO TRANSFER
    // asm 00009EC6: 	STI	R0,@PALXFER_AVAILABLE_P
    // asm 00009EC7: 	RETS
NOTCLR:
    // asm 00009EC8: 	LDI	@PALXFER_ACTIVE,R0
    // asm 00009EC9: 	B	I889
PALTR0:
    // asm 00009ECA: 	INC	R7
    // asm 00009ECB: 	CMPI	12,R7
    // asm 00009ECC: 	BGT	PALTRX
    // asm 00009ECD: 	LDI	*AR0,R0		;NEXT XFER BLOCK
    // asm 00009ECE: 	CALL	PALXFER_DEL
I889:
    // asm 00009ECF: LDI	R0,AR0
    // asm 00009ED0: 	CMPI	0,AR0
    // asm 00009ED1: 	BZ	PALTRX
    // asm 00009ED2: 	LDI	*+AR0(PALX_COUNT),R0
    // ;	BZ	PALTRX		;COUNT=0,GAME OVER DONE
    // asm 00009ED3: 	BNN	NOT_PACKED_PAL
    // 	;THIS IS A PACKED PALETTE STORED AS:  16BITS|(16BITS<<16)
    // 	;UNPACK ON THE FLY
    // asm 00009ED4: 	LS	1,R0		;remove high bit
    // asm 00009ED5: 	RS	2,R0		;divide by 2
#if DEBUG
    // asm: 	CMPI	256,R0
    // asm: 	SLOCKON	GT,"PALL\PALTRANS  more than 256 entries?"
#endif
    // ;	STI	R1,*AR0++		;CLEAR OUT COUNT
    // asm 00009ED6: 	LDI	*+AR0(PALX_SADDR),AR1	;GET SOURCE
    // asm 00009ED7: 	LDI	*+AR0(PALX_DADDR),AR2	;GET DESTINATION
    // ;	LDI	*AR0++,AR1		;GET SOURCE
    // ;	LDI	*AR0++,AR2		;GET DESTINATION
#if DEBUG
    // asm: 	CMPI	@COLRAML,AR2
    // asm: 	SLOCKON	LT,"PALL\PALTRANS SETUP XFER OUT OF CRAM LT"
    // asm: 	CMPI	@COLRAMH,AR2
    // asm: 	SLOCKON	GT,"PALL\PALTRANS SETUP XFER OUT OF CRAM GT"
#endif
    // asm 00009ED8: 	SUBI	1,R0		;DEC COUNT BY 1
    // asm 00009ED9: 	LDI	R0,RC
    // asm 00009EDA: 	RPTB	PACBLK
    // asm 00009EDB: 	LDI	*AR1++,R2
    // asm 00009EDC: 	STI	R2,*AR2++	;FIRST COLOR
    // asm 00009EDD: 	RS	16,R2
PACBLK:
    // asm 00009EDE: STI	R2,*AR2++
    // asm 00009EDF: 	B     	PALTR0		;LOOK FOR NEXT TRANSFER
NOT_PACKED_PAL:
    // ;	STI	R1,*AR0++	;CLEAR OUT COUNT
    // asm 00009EE0: 	LDI	*+AR0(PALX_SADDR),AR1	;GET SOURCE
    // asm 00009EE1: 	LDI	*+AR0(PALX_DADDR),AR2	;GET DESTINATION
    // ;	LDI	*AR0++,AR1	;GET SOURCE
    // ;	LDI	*AR0++,AR2	;GET DESTINATION
#if DEBUG
    // asm: 	CMPI	@COLRAML,AR2
    // asm: 	SLOCKON	LT,"PALL\PALTRANS SETUP XFER OUT OF CRAM LT 2"
    // asm: 	CMPI	@COLRAMH,AR2
    // asm: 	SLOCKON	GT,"PALL\PALTRANS SETUP XFER OUT OF CRAM GT 2"
#endif
    // asm 00009EE2: 	SUBI	2,R0		;DEC COUNT BY 1
    // asm 00009EE3: 	BNN	REGDOIT
    // asm 00009EE4: 	LDI	*AR1++,R2	;single case
    // asm 00009EE5: 	STI	R2,*AR2++
    // asm 00009EE6: 	B	PALTR0
REGDOIT:
    // asm 00009EE7: 	LDI	*AR1++,R2
    // asm 00009EE8: 	RPTS	R0
    // asm 00009EE9: 	LDI	*AR1++,R2
    // asm 00009EEA: 	STI	R2,*AR2++
    // asm 00009EEB: 	B     	PALTR0		;LOOK FOR NEXT TRANSFER
PALTRX:
    // asm 00009EEC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_XFER", 0, 0);
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
    // asm 00009EED: 	LDI	R0,AR2
    // asm 00009EEE: 	CALL	PAL_FIND
    // asm: 	SLOCKON	C,"PALL\PAL_OVERWRITE  FINDPAL FAILURE"
    // asm 00009EEF: 	RETSC
    // asm 00009EF0: 	LDP	@PALROMI
    // asm 00009EF1: 	LDI	R1,AR2
    // asm 00009EF2: 	ADDI	@PALROMI,AR2
    // asm 00009EF3: 	LDI	*AR2,AR2
    // 	;SETUP TRANSFER
    // asm 00009EF4: 	LDI	*AR2++,R3	;GET COUNT
    // asm 00009EF5: 	LDI	R0,R2		;GET DESTINATION
    // asm 00009EF6: 	CALL	PAL_SET
    // asm 00009EF7: 	RETS
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
    // asm 00009EF8: 	PUSH	AR2
    // asm 00009EF9: 	SETC		 	;ASSUME PALETTE NOT FOUND
    // asm 00009EFA: 	LDP	@PALLISTI
    // asm 00009EFB: 	ADDI	@PALLISTI,AR2
    // asm 00009EFC: 	LDI	*AR2,R0
    // asm 00009EFD: 	BZ	FPLXEX
    // asm 00009EFE: 	LSH	-16,R0
    // asm 00009EFF: 	LSH	8,R0
    // asm 00009F00: 	CLRC	    		;WE FOUND IT DUDES...
    // asm 00009F01: FPLX
    // asm 00009F01: 	POP	AR2
    // asm 00009F02: 	RETS
FPLXEX:
    // ;edbg
    // ;	BU	$
    // asm 00009F03: 	SETC
    // asm 00009F04: 	POP	AR2
    // asm 00009F05: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_FIND", 0, 0);
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
    // asm 00009F06: 	PUSH	AR0
    // asm 00009F07: 	LDI	-1,R0
    // asm 00009F08: 	LDP	@RAWLOCSI
    // asm 00009F09: 	LDI	@RAWLOCSI,AR0
FINDRLP:
    // asm 00009F0A: ADDI	1,R0
    // asm 00009F0B: 	CMPI	AR2,*AR0++
    // asm 00009F0C: 	BEQ	FOUNDRAW
    // asm 00009F0D: 	CMPI	PALNUM,R0
    // asm 00009F0E: 	BLE	FINDRLP
    // asm 00009F0F: 	LDI	-1,R0
    // ;	.if	DEBUG
    // ;edbg	;DBG DBG
    // ;	BU	$
    // ;	;DBG
    // ;	.endif
    // asm 00009F10: 	CLRC
    // asm 00009F11: 	POP	AR0
    // asm 00009F12: 	RETS
FOUNDRAW:
    // asm 00009F13: 	LSH	8,R0
    // asm 00009F14: 	SETC
    // asm 00009F15: 	POP	AR0
    // asm 00009F16: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_FIND_RAW", 0, 0);
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
    // asm 00009F17: 	PUSH	AR0
    // asm 00009F18: 	RS	8,AR2
    // asm 00009F19: 	LDP	@RAWLOCSI
    // asm 00009F1A: 	LDI	@RAWLOCSI,AR0
    // asm 00009F1B: 	ADDI	AR2,AR0
    // asm 00009F1C: 	CLRI	R0
    // asm 00009F1D: 	STI	R0,*AR0
    // ;edbg
    // ;	CMPI	0,AR2
    // ;	BEQ	$
    // ;edbg
    // asm 00009F1E: 	LDP	@PALRAMI
    // asm 00009F1F: 	LDI	@PALRAMI,AR0
    // asm 00009F20: 	ADDI	AR2,AR0
    // asm 00009F21: 	CLRI	R0
    // asm 00009F22: 	STI	R0,*AR0
    // asm 00009F23: 	POP	AR0
    // asm 00009F24: 	RETS
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
    // asm 00009F25: 	PUSHM	R2,R3,AR1,AR0,AR2
    // ;	.if	DEBUG
    // ;	CMPI	256,AR2
    // ;	SLOCKON	GT,"PALL\GETPAL	next RAM loc??? find out"
    // ;	.endif
    // 	;LOOK IF ALREADY ALLOCATED
    // asm 00009F2A: 	LDI	@PALLISTI,AR1
    // asm 00009F2B: 	ADDI	AR2,AR1
    // asm 00009F2C: 	LDI	*AR1,R0	     	;ALREADY ALLOCATED?
    // asm 00009F2D: 	BZ	GPL0		;NO...
    // asm 00009F2E: 	ADDI	1,R0		;YES, INCREMENT AND RETURN
    // asm 00009F2F: 	STI	R0,*AR1
    // asm 00009F30: 	B	GPLX		;RETURN...
GPL0:
    // 	;FIND A FREE ONE
    // asm 00009F31: 	LDP	@PALRAMI	 	;LOOK FOR FREE CELL
    // asm 00009F32: 	LDI	@PALRAMI,AR0
    // asm 00009F33: 	LDI	PALNUM-1,RC
    // asm 00009F34: 	LDI	*AR0++,R0 	;GET FIRST ONE
    // asm 00009F35: 	RPTB	GPLP
    // asm 00009F36: 	BZ	GETPL		;GOT A EMPTY
GPLP:
    // asm 00009F37: LDI	*AR0++,R0
    // asm 00009F38: GPERR
    // ;	SLOCKON	U,"PALL\GETPAL   ERROR NONE LEFT"
    // asm 00009F38: 	ERRON	U,77h
    // ;edbg	.if	DEBUG
    // ;	BU	$
    // ;	.endif
    // asm 00009F40: 	B	GPLX
GETPL:
    // asm 00009F41: 	LDI	AR2,R2
    // asm 00009F42: 	LDP	PALROMI
    // asm 00009F43: 	ADDI	@PALROMI,AR2
    // asm 00009F44: 	LDI	*AR2,AR2	;NOW HOLDS RAM LOCATION
    // asm 00009F45: 	OR	8000H,R2	;MAKE SURE A BIT IS SET
    // asm 00009F46: 	STI	R2,*-AR0(1)	;MARK PALETTE AS TAKEN
    // asm 00009F47: 	SUBI	PALNUM-1,RC	;GET PALETTE CODE
    // asm 00009F48: 	NEGI	RC,R0
    // asm 00009F49: 	LSH	16,R0
    // asm 00009F4A: 	ADDI	1,R0		;INC COUNT
    // asm 00009F4B: 	STI	R0,*AR1
    // 	;SETUP TRANSFER
    // asm 00009F4C: 	LDI	*AR2++,R3	;GET COUNT
    // asm 00009F4D: 	LDI	R0,R2		;GET DESTINATION
    // asm 00009F4E: 	LSH	-16,R2
    // asm 00009F4F: 	LSH	8,R2
    // asm 00009F50: 	CALL	PAL_SET
    // asm 00009F51: 	SUBI	1,AR2		;RESTORE AR2
GPLX:
    // asm 00009F52: 	LSH	-16,R0		;SHIFT DOWN CODE
    // asm 00009F53: 	LSH	8,R0
    // asm 00009F54: 	POPM	AR2,AR0,AR1,R3,R2
    // asm 00009F59: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_ALLOC", 0, 0);
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
    // asm 00009F5A: 	PUSHM	R2,R3,AR1,AR0,AR2
    // 	;FIND A FREE ONE
    // asm 00009F5F: 	LDP	@PALRAMI	 	;LOOK FOR FREE CELL
    // asm 00009F60: 	LDI	@PALRAMI,AR0
    // asm 00009F61: 	LDI	PALNUM-1,RC
    // asm 00009F62: 	LDI	*AR0++,R0 	;GET FIRST ONE
    // asm 00009F63: 	RPTB	RPLP
    // asm 00009F64: 	BZ	RAWPL		;GOT A EMPTY
RPLP:
    // asm 00009F65: LDI	*AR0++,R0
    // asm 00009F66: 	ERRON	U,78h
    // ;	SLOCKON	U,"PALL\RAWPAL  ERROR... NONE LEFT"
    // ;edbg	.if	DEBUG
    // ;	BU	$
    // ;	.endif
    // asm 00009F6E: 	B	RAWPEX		;UNTIL WE COME UP WITH A BETTER IDEA
RAWPL:
    // asm 00009F6F: 	LDI	-1,R2
    // asm 00009F70: 	STI	R2,*-AR0(1)	;MARK PALETTE AS TAKEN
    // asm 00009F71: 	SUBI	PALNUM-1,RC	;GET PALETTE CODE
    // asm 00009F72: 	NEGI	RC,R0
    // asm 00009F73: 	LSH	16,R0
    // asm 00009F74: 	ADDI	1,R0		;INC COUNT
    // 	;SETUP TRANSFER
    // asm 00009F75: 	LDI	*AR2++,R3	;GET COUNT
    // asm 00009F76: 	LDI	R0,R2		;GET DESTINATION
    // asm 00009F77: 	LSH	-16,R2
    // asm 00009F78: 	LSH	8,R2
    // asm 00009F79: 	CALL	PAL_SET
    // asm 00009F7A: 	SUBI	1,AR2		;RESTORE AR2
RPLX:
    // asm 00009F7B: LSH	-16,R0
    // asm 00009F7C: 	LDI	R0,AR0
    // asm 00009F7D: 	LDP	@RAWLOCSI
    // asm 00009F7E: 	ADDI	@RAWLOCSI,AR0
    // asm 00009F7F: 	STI	AR2,*AR0
RAWPEX:
    // asm 00009F80: 	LSH	8,R0
    // asm 00009F81: 	POPM	AR2,AR0,AR1,R3,R2
    // asm 00009F86: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_ALLOC_RAW", 0, 0);
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
    // asm 00009F87: 	PUSH	R0
    // asm 00009F88: 	PUSH	AR0
    // ;	LDI	2000h,R0
    // ;	CMPI	R2,R0
    // ;	BEQ	$	;... found it
    // ;edbg
    // ;	LDI	R2,R0
    // ;	RS	8,R0
    // ;	CMPI	0,R0
    // ;	BEQ	$
    // ;edbg
    // asm 00009F89: 	DINT
    // asm 00009F8F: 	CALL	PALXFER_GET
    // asm 00009F90: 	STI	AR2,*+AR0(PALX_SADDR)	;SAVE SOURCE ADDR
    // asm 00009F91: 	LDI	COLORAM>>16,RC		;ADD IN COLORAM ADDRESS
    // asm 00009F92: 	LSH	16,RC			;SHIFT IT IN PLACE
    // asm 00009F93: 	ADDI	R2,RC			;CONVERTED TO ADDRESS
    // asm 00009F94: 	STI	RC,*+AR0(PALX_DADDR)	;SAVE DEST ADDR
    // asm 00009F95: 	STI	R3,*+AR0(PALX_COUNT)	;SAVE WORD COUNT
    // asm 00009F96: 	CALL	ENABLEGIE
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
    // asm 00009F97: 	POP	AR0
    // asm 00009F98: 	POP	R0
    // asm 00009F99: 	RETS
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
    // asm 00009F9D: 	LDI	@PALXFER_FREEI,AR0	 	;GET FREE POINTER
    // asm 00009F9E: 	LDI	@PALXFER_STRI,AR1
    // asm 00009F9F: 	LDI	NXFER_PALS-1,RC
    // asm 00009FA0: 	RPTB	PXIL
    // asm 00009FA1: 	STI	AR1,*AR0
    // asm: 	LDI	AR1,AR0
PXIL:
    // asm 00009FA2: ADDI	PALX_SIZE,AR1
    // asm 00009FA4: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
    // asm 00009FA6: 	POP	AR1
    // asm 00009FA7: 	POP	AR0
    // asm 00009FA8: 	POP	R0
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
    // asm 00009FAF: 	BNE	CNT
    // asm 00009FB0: 	EINT
    // asm 00009FB1: 	BU	$
CNT:
#endif
    // asm: 	LDI	*+AR0(PALX_LINK),R0
    // asm: 	STI	R0,@PALXFER_FREE
    // asm: 	LDI	@PALXFER_ACTIVE,R0
    // asm: 	STI	R0,*+AR0(PALX_LINK)
    // asm: 	STI	AR0,@PALXFER_ACTIVE
    // asm: 	POP	R0
    // asm 00009FB3: 	RETS
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
    // asm 00009FBA: LDI	R1,AR1
    // asm: 	LDI	*AR1,R1
#if DEBUG
    // asm 00009FBA: 	BZ	$
#endif
    // asm: 	BZ	PXXX
    // asm 00009FBD: 	CMPI	R1,AR0
    // asm 00009FBE: 	BNE	PXFLP
    // asm: 	LDI	*AR0,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	@PALXFER_FREE,R1
    // asm 00009FC0: 	STI	R1,*AR0
    // asm 00009FC1: 	STI	AR0,@PALXFER_FREE
PXXX:
    // asm 00009FC2: 	POP	AR1
    // asm 00009FC3: 	POP	R0
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
    // asm 00009FCC: 	LS	16,R0
    // asm 00009FCE:      	STI	R0,*AR0
    // asm: 	LDI	R0,RC
    // asm 00009FCF: 	LSH	16,R0
    // asm 00009FD0: 	BNZ	DELP2	;palette not found ignore...
    // 	;CLEAR OUT PALRAM, LIST ENTRY IF COUNT IS ZERO
DELP1:
    // asm 00009FD3: LDI	0,R0
    // asm 00009FD3: 	STI	R0,*AR0		;CLEAR OUT LIST ENTRY
    // asm 00009FD4: 	LDP	@PALRAMI
    // asm 00009FD5: 	LDI	@PALRAMI,AR0
    // asm 00009FD6: 	LSH	-16,RC
    // asm: 	ADDI	RC,AR0
    // asm: 	STI	R0,*AR0		;ZERO OUT PALRAM AREA
DELP2:
    // asm 00009FD8: 	POP	AR0
    // asm 00009FD9: 	POP	R0
    // asm 00009FDA: 	RETS
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
    // asm 00009FE2: 	PUSH	AR1
    // asm 00009FE3: 	LDF	R0,R7
    // asm 00009FE5: 	LDI	*AR0++,R0
    // asm 00009FE6: 	STI	R0,*AR1++
    // asm 00009FE8: 	LDI	127,AR5
    // asm 00009FEA: PDMLP
    // asm 00009FEA: 	LDI	*AR0++,R4	;get src2
    // asm 00009FEB: 	LDI	R4,R1
    // asm: 	AND	01Fh,R1
    // asm 00009FEC: 	FLOAT	R1
    // asm 00009FED: 	MPYF	R7,R1
    // asm 00009FEE: 	FIX	R1
    // asm 00009FEF: 	LDI	R4,R2		;--
    // asm 00009FF0: 	RS	5,R2
    // asm 00009FF1: 	AND	01Fh,R2
    // asm 00009FF2: 	FLOAT	R2
    // asm 00009FF3: 	MPYF	R7,R2
    // asm 00009FF4: 	FIX	R2
    // asm: 	LS	5,R2
    // asm 00009FF5: 	OR	R2,R1
    // asm 00009FF7: 	LDI	R4,R2		;--
    // asm 00009FF8: 	RS	10,R2
    // asm 00009FF9: 	AND	01Fh,R2
    // asm 00009FFA: 	FLOAT	R2
    // asm 00009FFB: 	MPYF	R7,R2
    // asm 00009FFC: 	FIX	R2
    // asm: 	LS	10,R2
    // asm 00009FFD: 	OR	R2,R1
    // asm 00009FFE: 	PUSH	R1
    // 	;----second word----------
    // asm 0000A001: 	RS	16,R4		;get src1(B)
    // asm 0000A002: 	LDI	R4,R1
    // asm 0000A003: 	AND	01Fh,R1
    // asm 0000A004: 	FLOAT	R1
    // asm 0000A005: 	MPYF	R7,R1
    // asm: 	FIX	R1
    // asm 0000A006: 	LDI	R4,R2
    // asm 0000A007: 	RS	5,R2
    // asm 0000A008: 	AND	01Fh,R2
    // asm 0000A009: 	FLOAT	R2
    // asm 0000A00A: 	MPYF	R7,R2
    // asm 0000A00B: 	FIX	R2
    // asm: 	LS	5,R2
    // asm 0000A00C: 	OR	R2,R1
    // asm 0000A00E: 	LDI	R4,R2
    // asm 0000A00F: 	RS	10,R2
    // asm 0000A010: 	AND	01Fh,R2
    // asm 0000A011: 	FLOAT	R2
    // asm 0000A012: 	MPYF	R7,R2
    // asm 0000A013: 	FIX	R2
    // asm: 	LS	10,R2
    // asm 0000A014: 	OR	R2,R1
    // asm 0000A016: 	POP	R2
    // asm 0000A017: 	LS	16,R1
    // asm 0000A018: 	OR	R2,R1
    // asm 0000A019: 	STI	R1,*AR1++
    // asm 0000A01A: 	DBU	AR5,PDMLP
    // asm: 	POP	AR2			;RECOVER RAM BUFFER ADDR
    // asm 0000A01C: 	CALL	PAL_FIND_RAW
    // asm 0000A01D: 	LDI	*AR2++,R3
    // asm 0000A01E: 	LDI	R0,R2
    // asm 0000A01F: 	CALL	PAL_SET
    // asm: 	POPFL	R7
    // asm 0000A021: 	POPFL	R6
    // asm 0000A022: 	POP	R1
    // asm 0000A023: 	POP	R0
    // asm 0000A024: 	POP	AR2
    // asm 0000A025: 	POP	AR1
    // asm: 	POP	AR0
    // asm 0000A026: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_DIMMER", 0, 0);
    UNIMPL();
}

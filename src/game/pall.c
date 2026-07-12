#include "pall.h"

#include "../core/machine.h"
#include "../core/validator.h"
#include "error.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "objects.h"
#include "pall.h"
#include "sys.h"
#include "vunit.h"

/*
 * Source module: asm/PALL.ASM
 */

void PAL_INIT(void);
void PAL_XFER(void);
void PAL_OVERWRITE(void);
int PAL_FIND(u32 pal_index);
int PAL_FIND_RAW(const tPAL* palette_source);
void PAL_DELETE_RAW(int actual_palette_index /*AR2*/);
static void PALXFER_INIT(void);
static PALXFER* PALXFER_GET(void);
static void PALXFER_DEL(PALXFER* target);
void PAL_DELETE(int palette_code /*AR2*/);
void PAL_DIMMER(void);

#define PALROMI _PALROM
#define PTTRAMI PTTRAM
#define PALRAMI PALRAM
#define PALLISTI _PALLIST
#define RAWLOCSI RAWLOCS
#define PALXFER_ACTIVEI PALXFER_ACTIVE
#define PALXFER_FREEI PALXFER_FREE
#define PALXFER_STRI PALXFER_STR

/*
 *----------------------------------------------------------------------------
 *PALETTE ALLOCATION SYSTEM
 *
 *COPYRIGHT (C) 1994  BY TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 *
 *PALETTE MANAGEMENT ROUTINES
 *PALETTES ARE ALLOCATED IN 128 BLOCKS OF 256 COLORS
 *
 */

/* asm: PALRAM	.bss	PALRAM,PALNUM */
int PALRAM[PALNUM];
/* asm: RAWLOCS	.bss	RAWLOCS,PALNUM */
tPAL* RAWLOCS[PALNUM];
/* asm: PTTRAM	.bss	PTTRAM,PALNUM*3 */
int PTTRAM[PALNUM * 3];
/* asm: NUM_FIXED	.bss	NUM_FIXED,1 */
static int NUM_FIXED;

static int num_palettes = 253;

/*
 *----------------------------------------------------------------------------
 *INDEX STORAGE
 */
/* asm: PALROMI		.word	_PALROM		;INDEXED PALETTES SOURCE ADDR LIST */
#define PALROMI _PALROM
/* asm: PALLISTI	.word	_PALLIST	;CROSS-REFERENCE LIST */
#define PALLISTI _PALLIST

/*
 *----------------------------------------------------------------------------
 *CLEAR OUT PALETTE RAM
 *
 *
 */
void PAL_INIT(void) {
    // Clear palette transfer RAM.
    // Note: this is PALNUM * 3 words, not PALNUM * sizeof(PALXFER).
    // So PTTRAMI may not include the link field, or this may be a separate table.
    for (int i = 0; i < PALNUM * 3; i++) {
        PTTRAMI[i] = 0;
    }

    // Clear allocation state for palette RAM slots.
    for (int i = 0; i < PALNUM; i++) {
        PALRAMI[i] = 0;
    }

    // Clear raw palette source lookup by slot.
    for (int i = 0; i < PALNUM; i++) {
        RAWLOCSI[i] = NULL;
    }

    // Clear PCOMP/source-index palette refcount table.
    for (int i = 0; i < num_palettes; i++) {
        PALLISTI[i].ref_count_and_pal_code = 0;
    }

    PALXFER_INIT();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PALTRANS- TRANSFER PALETTE DATA TO COLORRAM
 *	CALL FROM DISPLAY IRQ AT BEGIN OF VBLNK
 *
 *CLOBBERS
 *	R0,R1,R7,AR0,AR1,AR2
 *
 */
/* asm: PALSXFER	.bss	PALSXFER,1 */
static int PALSXFER;

void PAL_XFER(void)
{
    int r7;
    PALXFER* ar0;
    intptr_t r0;
    u32* ar1;
    u32* ar2;
    int rc;

    // asm 00009EC2: 	CLRI	R7
    r7 = 0;
    // asm 00009EC3: 	LDP	@PALXFER_ACTIVE
    // asm 00009EC4: 	LDI	@PALXFER_ACTIVE,R0
    r0 = (intptr_t)PALXFER_ACTIVE;
    // asm 00009EC5: 	BNE	NOTCLR
    if (r0 != 0) {
        goto NOTCLR;
    }
    // 	;NONE LEFT TO TRANSFER
    // asm 00009EC6: 	STI	R0,@PALXFER_AVAILABLE_P
    PALXFER_AVAILABLE_P = r0;
    // asm 00009EC7: 	RETS
    return;
NOTCLR:
    // asm 00009EC8: 	LDI	@PALXFER_ACTIVE,R0
    r0 = (intptr_t)PALXFER_ACTIVE;
    // asm 00009EC9: 	B	I889
    goto I889;
PALTR0:
    // asm 00009ECA: 	INC	R7
    r7++;
    // asm 00009ECB: 	CMPI	12,R7
    if (r7 > 12) {
        goto PALTRX;
    }
    // asm 00009ECC: 	BGT	PALTRX
    // asm 00009ECD: 	LDI	*AR0,R0		;NEXT XFER BLOCK
    r0 = (intptr_t)ar0->link;
    // asm 00009ECE: 	CALL	PALXFER_DEL
    PALXFER_DEL(ar0);
I889:
    // asm 00009ECF: LDI	R0,AR0
    ar0 = (PALXFER*)r0;
    // asm 00009ED0: 	CMPI	0,AR0
    if (ar0 == NULL) {
        goto PALTRX;
    }
    // asm 00009ED1: 	BZ	PALTRX
    // asm 00009ED2: 	LDI	*+AR0(PALX_COUNT),R0
    r0 = (int)ar0->count;
    // ;	BZ	PALTRX		;COUNT=0,GAME OVER DONE
    // asm 00009ED3: 	BNN	NOT_PACKED_PAL
    if (r0 >= 0) {
        goto NOT_PACKED_PAL;
    }
    // 	;THIS IS A PACKED PALETTE STORED AS:  16BITS|(16BITS<<16)
    // 	;UNPACK ON THE FLY
    // asm 00009ED4: 	LS	1,R0		;remove high bit
    r0 <<= 1;
    // asm 00009ED5: 	RS	2,R0		;divide by 2
    r0 = (int)((u32)r0 >> 2);
#if DEBUG
    // asm: 	CMPI	256,R0
    // asm: 	SLOCKON	GT,"PALL\PALTRANS  more than 256 entries?"
#endif
    // ;	STI	R1,*AR0++		;CLEAR OUT COUNT
    // asm 00009ED6: 	LDI	*+AR0(PALX_SADDR),AR1	;GET SOURCE
    ar1 = ar0->source_addr;
    // asm 00009ED7: 	LDI	*+AR0(PALX_DADDR),AR2	;GET DESTINATION
    ar2 = ar0->dest_addr;
    // ;	LDI	*AR0++,AR1		;GET SOURCE
    // ;	LDI	*AR0++,AR2		;GET DESTINATION
#if DEBUG
COLRAML:
    // asm 00009ED8: .word	COLORAM
    // asm: 	CMPI	@COLRAML,AR2
    // asm: 	SLOCKON	LT,"PALL\PALTRANS SETUP XFER OUT OF CRAM LT"
    // asm: 	CMPI	@COLRAMH,AR2
    // asm: 	SLOCKON	GT,"PALL\PALTRANS SETUP XFER OUT OF CRAM GT"
#endif
    // asm 00009ED8: 	SUBI	1,R0		;DEC COUNT BY 1
    r0 -= 1;
    // asm 00009ED9: 	LDI	R0,RC
    rc = r0;
    // asm 00009EDA: 	RPTB	PACBLK
    // asm 00009EDB: 	LDI	*AR1++,R2
    // asm 00009EDC: 	STI	R2,*AR2++	;FIRST COLOR
    // asm 00009EDD: 	RS	16,R2
    while (1) {
        u32 r2 = *ar1++;

        *ar2++ = r2 & 0xffffu;
        r2 >>= 16;
PACBLK:
        // asm 00009EDE: STI	R2,*AR2++	;SECOND COLOR
        *ar2++ = r2;
        if (rc-- <= 0) {
            break;
        }
    }
    // asm 00009EDF: 	B     	PALTR0		;LOOK FOR NEXT TRANSFER
    goto PALTR0;
NOT_PACKED_PAL:
    // ;	STI	R1,*AR0++	;CLEAR OUT COUNT
    // asm 00009EE0: 	LDI	*+AR0(PALX_SADDR),AR1	;GET SOURCE
    ar1 = ar0->source_addr;
    // asm 00009EE1: 	LDI	*+AR0(PALX_DADDR),AR2	;GET DESTINATION
    ar2 = ar0->dest_addr;
    // ;	LDI	*AR0++,AR1	;GET SOURCE
    // ;	LDI	*AR0++,AR2	;GET DESTINATION
#if DEBUG
    // asm: 	CMPI	@COLRAML,AR2
    // asm: 	SLOCKON	LT,"PALL\PALTRANS SETUP XFER OUT OF CRAM LT 2"
    // asm: 	CMPI	@COLRAMH,AR2
    // asm: 	SLOCKON	GT,"PALL\PALTRANS SETUP XFER OUT OF CRAM GT 2"
#endif
    // asm 00009EE2: 	SUBI	2,R0		;DEC COUNT BY 1
    r0 -= 2;
    // asm 00009EE3: 	BNN	REGDOIT
    if (r0 >= 0) {
        goto REGDOIT;
    }
    // asm 00009EE4: 	LDI	*AR1++,R2	;single case
    r0 = (int)*ar1++;
    // asm 00009EE5: 	STI	R2,*AR2++
    *ar2++ = (u32)r0;
    // asm 00009EE6: 	B	PALTR0
    goto PALTR0;
REGDOIT:
    // asm 00009EE7: 	LDI	*AR1++,R2
    {
        u32 r2 = *ar1++;

    // asm 00009EE8: 	RPTS	R0
    // asm 00009EE9: 	LDI	*AR1++,R2
    // asm 00009EE9:  ||	STI	R2,*AR2++
    // asm 00009EEA: 	STI	R2,*AR2++
        while (r0-- >= 0) {
            *ar2++ = r2;
            r2 = *ar1++;
        }
        *ar2++ = r2;
    }
    // asm 00009EEB: 	B     	PALTR0		;LOOK FOR NEXT TRANSFER
    goto PALTR0;
PALTRX:
    // asm 00009EEC: 	RETS
    if (PALXFER_ACTIVE == NULL) {
        PALXFER_AVAILABLE_P = 0;
    }
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *FIND A PALETTE
 *
 *PARAMETERS
 *	AR2	PALETTE INDEX
 *RETURNS
 *	R0	COLORAM CODE
 *	CARRY SET IF PALETTE NOT FOUND
 *
 */
int PAL_FIND(u32 pal_index) {
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
    // ;edbg
    // ;	BU	$
    // asm 00009F03: 	SETC
    // asm 00009F04: 	POP	AR2
    // asm 00009F05: 	RETS
    int ref_count_and_pal_code;

    ref_count_and_pal_code = PALLISTI[pal_index].ref_count_and_pal_code;
    if (ref_count_and_pal_code == 0) {
        return -1;
    }

    return (ref_count_and_pal_code >> 16) << 8;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
int PAL_FIND_RAW(const tPAL* palette_source) {
    // asm 00009F06: 	PUSH	AR0
    // asm 00009F07: 	LDI	-1,R0
    // asm 00009F08: 	LDP	@RAWLOCSI
    // asm 00009F09: 	LDI	@RAWLOCSI,AR0
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
    // asm 00009F13: 	LSH	8,R0
    // asm 00009F14: 	SETC
    // asm 00009F15: 	POP	AR0
    // asm 00009F16: 	RETS
    for (int slot = 0; slot < PALNUM; slot++) {
        if (RAWLOCSI[slot] == palette_source) {
            return slot << 8;
        }
    }

    return -1;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *PARAMETERS
 *	AR2	ACTUAL PALETTE INDEX
 *		(16 BIT)
 *
 */
void PAL_DELETE_RAW(int actual_palette_index /*AR2*/)
{
    int slot;

    // asm 00009F17: 	PUSH	AR0
    // asm 00009F18: 	RS	8,AR2
    slot = actual_palette_index >> 8;
    // asm 00009F19: 	LDP	@RAWLOCSI
    // asm 00009F1A: 	LDI	@RAWLOCSI,AR0
    // asm 00009F1B: 	ADDI	AR2,AR0
    // asm 00009F1C: 	CLRI	R0
    // asm 00009F1D: 	STI	R0,*AR0
    RAWLOCSI[slot] = NULL;
    // ;edbg
    // ;	CMPI	0,AR2
    // ;	BEQ	$
    // ;edbg
    // asm 00009F1E: 	LDP	@PALRAMI
    // asm 00009F1F: 	LDI	@PALRAMI,AR0
    // asm 00009F20: 	ADDI	AR2,AR0
    // asm 00009F21: 	CLRI	R0
    // asm 00009F22: 	STI	R0,*AR0
    PALRAMI[slot] = 0;
    // asm 00009F23: 	POP	AR0
    // asm 00009F24: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GET A PALETTE
 *
 *PARAMETERS
 *	AR2	PALETTE SOURCE INDEX
 *RETURNS
 *	R0	PALETTE CODE
 *	LOCKUP ON ERROR
 *
 */
tPALETTE_CODE PAL_ALLOC(u32 pal_index) {
    tPALLIST_ENTRY* entry = &_PALLIST[pal_index];

    /* LOOK IF ALREADY ALLOCATED */
    if (entry->ref_count_and_pal_code != 0) {
        /* YES, INCREMENT AND RETURN */
        entry->ref_count_and_pal_code++;

        // Upper 16 bits store the palette slot.
        return ((entry->ref_count_and_pal_code >> 16) << 8);
    }

    /* FIND A FREE ONE */
    /* LOOK FOR FREE CELL */
    int slot = -1;

    for (int i = 0; i < PALNUM; ++i) {
        if (PALRAM[i] == 0) {
            slot = i;
            break;
        }
    }

    if (slot < 0) {
        /* ERROR NONE LEFT */
        ERRON(0x77);
        return 0;
    }

    tPAL* pal_src = PALROMI[pal_index];

    /* MAKE SURE A BIT IS SET */
    PALRAMI[slot] = pal_index | 0x8000;

    /* GET PALETTE CODE */
    entry->ref_count_and_pal_code = ((u32)slot << 16) | 1;

    /* SETUP TRANSFER */
    /* GET COUNT */
    u32 count = pal_src->flags_and_count;

    /* GET DESTINATION */
    PAL_SET(pal_src->data, (u32)slot << 8, count);

    /* SHIFT DOWN CODE */
    return (u32)slot << 8;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
tPALETTE_CODE PAL_ALLOC_RAW(tPAL* palette_source) {
    int slot = -1;

    // mame_validate_arg("AR2", palette_source);

    for (int i = 0; i < PALNUM; i++) {
        if (PALRAMI[i] == 0) {
            slot = i;
            break;
        }
    }

    if (slot < 0) {
        ERRON(0x78);
        return 0; // original likely never returns normally
    }

    // Raw palettes are marked as taken, but not associated with a PCOMP index.
    PALRAMI[slot] = UINT32_MAX;

    // First word of source is the transfer count.
    uint32_t count = palette_source->flags_and_count;

    // mame_validate_reg_at_addr(0x00009F76, "R3", &count);

    // Hardware palette destination is slot in bits 8-15, color index 0 in bits 0-7.
    uint32_t palette_code = (uint32_t)slot << 8;

    PAL_SET(palette_source->data, palette_code, count);

    // Remember the original raw palette record pointer, including count header.
    RAWLOCSI[slot] = palette_source;

    // mame_validate_reg_at_addr(0x00009F86, "R0", &palette_code);

    return palette_code;
}

// *----------------------------------------------------------------------------

#define PALX_LINK 0
#define PALX_SADDR 1
#define PALX_DADDR 2
#define PALX_COUNT 3
#define PALX_SIZE 4

/*
 *----------------------------------------------------------------------------
 *SETUP PALETTE TRANSFER
 *
 *PARAMETERS
 *	AR2	SOURCE DATA ADDRESSS
 *	R2	DEST PALETTE(B8-15), DEST COLOR(B0-7)
 *	R3	COUNT
 *
 */
void PAL_SET(uint32_t* src, uint32_t destPaletteColor, uint32_t count) {
    PALXFER* xfer;

    xfer = PALXFER_GET(); // get/free/allocate a transfer slot

    xfer->source_addr = src;

    // Build full Color RAM destination address.
    // COLORAM high address bits are combined with palette/color offset.
    xfer->dest_addr = COLOROM_ADDR((COLORAM & 0xFFFF0000) + destPaletteColor);

    xfer->count = count;

#ifdef DEBUG
    // TODO
    // if (xfer->dest_addr < COLRAML || xfer->dest_addr > COLRAMH) {
    //     ERRON(0x88);
    // }
#endif
}

// *----------------------------------------------------------------------------

#define NXFER_PALS 128
/* asm: PALXFER_ACTIVE	.bss	PALXFER_ACTIVE,1 */
PALXFER* PALXFER_ACTIVE;
/* asm: PALXFER_FREE	.bss	PALXFER_FREE,1 */
PALXFER* PALXFER_FREE;
/* asm: PALXFER_AVAILABLE_P	.bss	PALXFER_AVAILABLE_P,1 */
int PALXFER_AVAILABLE_P;
PALXFER PALXFER_STR[NXFER_PALS];

// *----------------------------------------------------------------------------
void PALXFER_INIT(void) {
    PALXFER_ACTIVE = NULL;
    PALXFER_FREEI = PALXFER_STRI;

    for (u32 i = 0; i < NXFER_PALS - 1; i++) {
        PALXFER_STRI[i].link = &PALXFER_STRI[i + 1];
    }

    PALXFER_STRI[NXFER_PALS - 1].link = NULL;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ALLOCATE A PALETTE XFER BLOCK
 *
 *RETURNS
 *	AR0	PTR TO AVAILABLE XFER BLOCK
 *
 */
static PALXFER* PALXFER_GET(void) {
    PALXFER_AVAILABLE_P = 1;

    PALXFER* xfer = PALXFER_FREE;

#ifdef DEBUG
    if (xfer == NULL) {
        abort();
    }
#endif

    // unlink from free list
    PALXFER_FREE = (PALXFER*)xfer->link;

    // push onto active list
    xfer->link = PALXFER_ACTIVE;
    PALXFER_ACTIVE = xfer;

    return xfer;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *FREE A PALETTE XFER BLOCK
 *
 *PARAMETERS
 *	AR0	PTR TO XFER BLOCK TO FREE
 *
 */
static void PALXFER_DEL(PALXFER* target)
{
    PALXFER** r1p;
    PALXFER* r1;
    PALXFER** ar1;

    // asm 00009FBA: 	PUSH	R0
    // asm 00009FBB: 	PUSH	AR1
    // asm 00009FBC: 	LDI	@PALXFER_ACTIVEI,R1
    r1p = &PALXFER_ACTIVEI;
PXFLP:
    // asm 00009FBD: LDI	R1,AR1
    ar1 = r1p;
    // asm 00009FBE: 	LDI	*AR1,R1
    r1 = *ar1;
#if DEBUG
    // asm: 	BZ	$
#endif
    // asm 00009FBF: 	BZ	PXXX
    if (r1 == NULL) {
        goto PXXX;
    }
    // asm 00009FC0: 	CMPI	R1,AR0
    if (r1 != target) {
        r1p = &r1->link;
        goto PXFLP;
    }
    // asm 00009FC1: 	BNE	PXFLP
    // asm 00009FC2: 	LDI	*AR0,R1
    r1 = target->link;
    // asm 00009FC3: 	STI	R1,*AR1			;LINK AROUND
    *ar1 = r1;
    // asm 00009FC4: 	LDI	@PALXFER_FREE,R1
    r1 = PALXFER_FREE;
    // asm 00009FC5: 	STI	R1,*AR0
    target->link = r1;
    // asm 00009FC6: 	STI	AR0,@PALXFER_FREE
    PALXFER_FREE = target;
PXXX:
    // asm 00009FC7: 	POP	AR1
    // asm 00009FC8: 	POP	R0
    // asm 00009FC9: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DELETE A PALETTE
 *
 *PARAMETERS
 *	AR2	PALETTE INDEX
 *	LOCKUP ON ERROR
 *
 */
void PAL_DELETE(int palette_code /*AR2*/)
{
    int entry;
    int slot;

    // asm 00009FCA: 	PUSH	R0
    // asm 00009FCB: 	PUSH	AR0
    // ;edbg
    // ;	CMPI	0,AR2
    // ;	BEQ	$
    // ;edbg
    // 	;DELETE FROM PALETTE LIST AREA
    // asm 00009FCC: 	LDP	@PALLISTI
    // asm 00009FCD: 	LDI	@PALLISTI,AR0
    // asm 00009FCE: 	ADDI	AR2,AR0
    // asm 00009FCF: 	LDI	*AR0,R0
    entry = PALLISTI[palette_code].ref_count_and_pal_code;
    // asm 00009FD0: 	BZ	DELP2
    if (entry == 0) {
        return;
    }
    // asm 00009FD1: 	RS	16,R0
    // asm 00009FD2: 	LS	16,R0
    // asm 00009FD3:      	STI	R0,*AR0
    entry &= 0xffff0000u;
    PALLISTI[palette_code].ref_count_and_pal_code = entry;
    // asm 00009FD4: 	LDI	R0,RC
    // asm 00009FD5: 	LSH	16,R0
    // asm 00009FD6: 	BNZ	DELP2	;palette not found ignore...
    if ((entry << 16) != 0) {
        return;
    }
    // 	;CLEAR OUT PALRAM, LIST ENTRY IF COUNT IS ZERO
DELP1:
    // asm 00009FD7: LDI	0,R0
    // asm 00009FD8: 	STI	R0,*AR0		;CLEAR OUT LIST ENTRY
    PALLISTI[palette_code].ref_count_and_pal_code = 0;
    // asm 00009FD9: 	LDP	@PALRAMI
    // asm 00009FDA: 	LDI	@PALRAMI,AR0
    // asm 00009FDB: 	LSH	-16,RC
    // asm 00009FDC: 	ADDI	RC,AR0
    // asm 00009FDD: 	STI	R0,*AR0		;ZERO OUT PALRAM AREA
    slot = entry >> 16;
    PALRAMI[slot] = 0;
DELP2:
    // asm 00009FDE: 	POP	AR0
    // asm 00009FDF: 	POP	R0
    // asm 00009FE0: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
    // asm 00009FE1: 	PUSH	AR0
    // asm 00009FE2: 	PUSH	AR1
    // asm 00009FE3: 	PUSH	AR2
    // asm 00009FE4: 	PUSH	R0
    // asm 00009FE5: 	PUSH	R1
    // asm 00009FE6: 	PUSHFL	R6
    // asm 00009FE8: 	PUSHFL	R7
    // asm 00009FEA: 	PUSH	AR1
    // asm 00009FEB: 	LDF	R0,R7
    // asm 00009FEC: 	LDI	*AR0++,R0
    // asm 00009FED: 	STI	R0,*AR1++
    // asm 00009FEE: 	LDI	127,AR5
    // asm 00009FEF: PDMLP
    // asm 00009FEF: 	LDI	*AR0++,R4	;get src2
    // asm 00009FF0: 	LDI	R4,R1
    // asm 00009FF1: 	AND	01Fh,R1
    // asm 00009FF2: 	FLOAT	R1
    // asm 00009FF3: 	MPYF	R7,R1
    // asm 00009FF4: 	FIX	R1
    // asm 00009FF5: 	LDI	R4,R2		;--
    // asm 00009FF6: 	RS	5,R2
    // asm 00009FF7: 	AND	01Fh,R2
    // asm 00009FF8: 	FLOAT	R2
    // asm 00009FF9: 	MPYF	R7,R2
    // asm 00009FFA: 	FIX	R2
    // asm 00009FFB: 	LS	5,R2
    // asm 00009FFC: 	OR	R2,R1
    // asm 00009FFD: 	LDI	R4,R2		;--
    // asm 00009FFE: 	RS	10,R2
    // asm 00009FFF: 	AND	01Fh,R2
    // asm 0000A000: 	FLOAT	R2
    // asm 0000A001: 	MPYF	R7,R2
    // asm 0000A002: 	FIX	R2
    // asm 0000A003: 	LS	10,R2
    // asm 0000A004: 	OR	R2,R1
    // asm 0000A005: 	PUSH	R1
    // 	;----second word----------
    // asm 0000A006: 	RS	16,R4		;get src1(B)
    // asm 0000A007: 	LDI	R4,R1
    // asm 0000A008: 	AND	01Fh,R1
    // asm 0000A009: 	FLOAT	R1
    // asm 0000A00A: 	MPYF	R7,R1
    // asm 0000A00B: 	FIX	R1
    // asm 0000A00C: 	LDI	R4,R2
    // asm 0000A00D: 	RS	5,R2
    // asm 0000A00E: 	AND	01Fh,R2
    // asm 0000A00F: 	FLOAT	R2
    // asm 0000A010: 	MPYF	R7,R2
    // asm 0000A011: 	FIX	R2
    // asm 0000A012: 	LS	5,R2
    // asm 0000A013: 	OR	R2,R1
    // asm 0000A014: 	LDI	R4,R2
    // asm 0000A015: 	RS	10,R2
    // asm 0000A016: 	AND	01Fh,R2
    // asm 0000A017: 	FLOAT	R2
    // asm 0000A018: 	MPYF	R7,R2
    // asm 0000A019: 	FIX	R2
    // asm 0000A01A: 	LS	10,R2
    // asm 0000A01B: 	OR	R2,R1
    // asm 0000A01C: 	POP	R2
    // asm 0000A01D: 	LS	16,R1
    // asm 0000A01E: 	OR	R2,R1
    // asm 0000A01F: 	STI	R1,*AR1++
    // asm 0000A020: 	DBU	AR5,PDMLP
    // asm 0000A021: 	POP	AR2			;RECOVER RAM BUFFER ADDR
    // asm 0000A022: 	CALL	PAL_FIND_RAW
    // asm 0000A023: 	LDI	*AR2++,R3
    // asm 0000A024: 	LDI	R0,R2
    // asm 0000A025: 	CALL	PAL_SET
    // asm 0000A026: 	POPFL	R7
    // asm 0000A028: 	POPFL	R6
    // asm 0000A02A: 	POP	R1
    // asm 0000A02B: 	POP	R0
    // asm 0000A02C: 	POP	AR2
    // asm 0000A02D: 	POP	AR1
    // asm 0000A02E: 	POP	AR0
    // asm 0000A02F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAL_DIMMER", 0, 0);
    UNIMPL();
}

#include "util.h"

#include "../core/machine.h"
#include "c30.h"
#include "cmos.h"
#include "delta.h"
#include "dirq.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "validator.h"
#include "vunit.h"

/*
 * Source module: asm/UTIL.ASM
 */

static void TVBP(void);
static void TVBPX(void);
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
word_addr_t SCREEN_FILL(word_addr_t start_addr, u32 color, u32 count_minus_one);
void CLRCRAM(void);
uint32_t RANDOM(void);
float FRAND(float limit /*R0*/);
float SFRAND(float limit /*R0*/);
int RANDU0(int range /*AR2*/);
int RANDU(int range /*AR2*/);
int SRAND(int range /*AR2*/);
int RANDPER(int probability /*AR2*/);
void INIT_LINKED_LIST(void* start_addr /*AR2*/, void** free_list /*R2*/, void** active_list /*R3*/, int length_minus_1 /*RC*/, int size /*RS*/);
void* GET_LLIST(void** free_list, void** active_list);
void ALLOC_LLIST(void);
void FREE_LLIST(void);
void DEL_LLIST(void);
void VEHICLE_ANI_INIT(int vehicle_index /*AR2*/, OBJ* obj /*AR4*/);
void CARPROC(PROC* p);
void LEAN(PROC* p, DYNAOBJ* dyna, OBJ* obj, CARBLK* carblk);
void DYNAOBJ_INIT(void);
DYNAOBJ* GETDYNA(void);
void DELDYNA(void);
void CARB_INIT(void);
CARBLK* GETCAR(void);
void DELCAR(void);
void SCAN_OBJECTS(PROC* p);
void PUSHALL(void);
void POPALL(void);
void DISTANCE_2D(void);
void OVELADD(void);
void OVELNADD(void);
void FORWARD(void);

#define FASTCLR0 FASTCLR1
#define SCREEN0I SCREEN0
#define SCREEN1I SCREEN1
#define CARPROCI CARPROC
#define DYNALISTI DYNALIST
#define DYNAFREEI DYNAFREE
#define CARLISTI CARLIST
#define CARFREEI CARFREE

extern int FILSIZI;
extern MATRIX _MATRIXA;
extern MATRIX _MATRIXB;
extern MATRIX _MATRIXC;

/* asm: RAND	pbss	RAND,1 */
int RAND;
/* asm: CRTCTLRAM	.bss	CRTCTLRAM,1 */
int CRTCTLRAM;
/* asm: ACTIVE_SCREEN	.bss	ACTIVE_SCREEN,1 */
int ACTIVE_SCREEN;
/* asm: NULL	.bss	NULL,1 */
int NULL_;
/*
 *----------------------------------------------------------------------------
 *ROM DEFINITIONS
 *
 */
/* asm: LINE255I	.word	SCREEN0+3F000H 	;LAST LINE PAGE 0 */
static int LINE255I = SCREEN0 + 0x3F000;
/* asm: LINE511I	.word	SCREEN0+7FC00H	;LAST LINE PAGE 1 */
/* asm: 	 */
static int LINE511I = SCREEN0 + 0x7FC00;
/* asm: SCRSIZI		.word	3FFFFH */
/* asm: 	 */
/* asm: 	 */
static int SCRSIZI = 0x3FFFF;
/* asm: FILWORD	.word	93093H */
static int FILWORD = 0x93093;
static u32 DMA_SETUP_SHADOW = DMA_SETUP_INIT;
#if DEBUG

static void TVBP(void) {
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TVBP", 0, 0);
    UNIMPL();
}

static void TVBPX(void) {
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TVBPX", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
#endif

/*
 *----------------------------------------------------------------------------
 *SET SCREEN DISPLAY TO PAGE 0  (AND WRITE PAGE TO 1)
 *
 */
void SETPAGE0(void) {
    u32 dma_setup;

    // ;	.if	DEBUG
    // asm 00008E76: 	LDI	@PAGEWORD,R0
    // asm 00008E77: 	BNE	P1
    // ;	.endif
    // asm 00008E78: 	LDI	@SCREEN1I,R0		;set active screen to 1 (writeable)
    // asm 00008E79: 	STI	R0,@ACTIVE_SCREEN
    ACTIVE_SCREEN = SCREEN1I;
    // asm 00008E7A: 	LDP	@DMA_SETUP
    // asm 00008E7B: 	LDI	@DMA_SETUP,R0
    dma_setup = DMA_SETUP_SHADOW;
    // asm 00008E7C: 	ANDN	DMA_VIDEO_PAG_DISPLAYED,R0
    dma_setup &= ~DMA_VIDEO_PAG_DISPLAYED;
    // asm 00008E7D: 	OR	DMA_DMA_WRITE_PAGE,R0
    dma_setup |= DMA_DMA_WRITE_PAGE;
    // asm 00008E7E: 	STI	R0,@DMA_SETUP
    DMA_SETUP_SHADOW = dma_setup;
    crusn_machine_set_screen_pages(g_crusn_machine, 0, 1);
    // asm 00008E7F: 	SETDP
    // asm 00008E80: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SET SCREEN DISPLAY TO PAGE 1	(AND WRITE PAGE TO 0)
 *
 */
void SETPAGE1(void) {
    u32 dma_setup;

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
    ACTIVE_SCREEN = SCREEN0I;
    // asm 00008E8F: 	LDP	@DMA_SETUP
    // asm 00008E90: 	LDI	@DMA_SETUP,R0
    dma_setup = DMA_SETUP_SHADOW;
    // asm 00008E91: 	OR	DMA_VIDEO_PAG_DISPLAYED+DMA_DMA_WRITE_PAGE,R0
    dma_setup |= DMA_VIDEO_PAG_DISPLAYED;
    dma_setup &= ~DMA_DMA_WRITE_PAGE;
    // asm 00008E92: 	STI	R0,@DMA_SETUP
    DMA_SETUP_SHADOW = dma_setup;
    crusn_machine_set_screen_pages(g_crusn_machine, 1, 0);
    // asm 00008E93: 	SETDP
    // asm 00008E94: 	RETS
    // ;	.endif
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SRT PAGE ONE
 *
 *CLEAR SCREEN FAST SHIFT REGISTER TRANSFER
 *CALL ONLY DURING VBLANK
 *
 */
void FASTCLR1(void) {
    word_addr_t start_addr;

    // asm 00008E95: 	LDI	@NOAERASE,R0
    // asm 00008E96: 	RETSNZ
    if (NOAERASE != 0) {
        return;
    }
    // asm 00008E97: 	LDI	0,AR2		;X
    // asm 00008E98: 	STI	AR2,@_ACNTL
    _ACNTL = 0;
    // asm 00008E99: 	LDI	0,R2		;Y
    // asm 00008E9A: 	LDI	511,R3		;X2
    // asm 00008E9B: 	LDI	399,RC		;Y2
    // asm 00008E9C: 	LDI	0,RS		;PAL
    // asm 00008E9D: 	LDI	0A0h,RE		;ADDR
    // asm 00008E9E: 	RS	16,RE
    if (ACTIVE_SCREEN == SCREEN1I) {
        start_addr = (word_addr_t)SCREEN1I;
    } else {
        start_addr = (word_addr_t)SCREEN0I;
    }
    // asm 00008E9F: 	CALL	_rdma
    SCREEN_FILL(start_addr, 0, (u32)SCRSIZI);
    // asm 00008EA0: 	RETS
}

// *----------------------------------------------------------------------------

static void port_clear_screen(word_addr_t start_addr) {
    SCREEN_FILL(start_addr, 0, (u32)SCRSIZI);
}

/*
 *----------------------------------------------------------------------------
 *CLEAR SCREEN BITMAP
 *
 */
void CLRSCRN(void) {
    CLRSCRN0();
}

void CLRSCRN1(void) {
    port_clear_screen((word_addr_t)SCREEN1I);
}

void CLRSCRN0(void) {
    port_clear_screen((word_addr_t)SCREEN0I);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *CLEAR LINE 255,255 OF BITMAP
 *
 */
void CLR255(void) {
    SCREEN_FILL((word_addr_t)LINE255I, 0, 1023);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *CLEAR LINE 511,511 OF BITMAP
 *
 */
void CLR511(void) {
    SCREEN_FILL((word_addr_t)LINE511I, 0, 1023);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *FILL SCREEN
 */
void SCRNFIL(void) {
    SCREEN_FILL((word_addr_t)SCREEN0I, (u32)FILWORD, (u32)FILSIZI);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SCREEN WRITER
 *
 *PARAMETERS
 *	start_addr	START ADDRESS
 *	color		FILL VALUE
 *	count_minus_one	COUNT-1
 *
 */
word_addr_t SCREEN_FILL(word_addr_t start_addr, u32 color, u32 count_minus_one) {
    word_addr_t addr = start_addr;

    for (u32 i = 0; i <= count_minus_one; ++i) {
        crusn_mem_wr32(addr++, color);
    }

    return addr;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *CLEAR COLOR RAM
 *
 */
void CLRCRAM(void) {
    // asm 00008ED9: 	LDI	COLORAM>>16,AR0
    // asm 00008EDA: 	LSH	16,AR0
    // asm 00008EDB: 	LDI	0,R1
    // asm 00008EDC: 	RPTS	07FFFH
    // asm 00008EDD: 	STI	R1,*AR0++
    // asm 00008EDE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRCRAM", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *RANDOM NUMBER UTILITIES
 *----------------------------------------------------------------------------
 *RANDOM - GENERATE A RANDOM NUMBER
 *RETURNS
 *	R0	32 BIT RANDOM #
 *
 */
uint32_t RANDOM(void) {
    uint32_t rand_seed;
    uint32_t r1;

    // asm 00008EDF: 	PUSH	R1
    // asm 00008EE0: 	LDI	@RAND,R0
    rand_seed = (uint32_t)RAND;
    // asm 00008EE1: 	LDI	R0,R1
    r1 = rand_seed;
    // asm 00008EE2: 	LSH	1,R0
    rand_seed <<= 1;
    // asm 00008EE3: 	XOR	R0,R1
    rand_seed ^= r1;
    // asm 00008EE4: 	BNN	RND2
    if ((int32_t)rand_seed < 0) {
        // asm 00008EE5: 	OR	1,R0
        rand_seed |= 1u;
    }
RND2:
    // asm 00008EE6: POP	R1
    // asm 00008EE7: 	MPYI	794Fh,R0
    rand_seed *= 0x794Fu;
    // asm 00008EE8: 	STI	R0,@RAND
    RAND = (int)rand_seed;
    // asm 00008EE9: 	RETS
    return rand_seed;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *FLOATING POINT RANDOM
 *
 *PARAMETERS
 *	R0	NUMBER
 *RETURNS
 *	R0	RANDOM NUMBER 0->N FLOATING POINT
 *
 */
float FRAND(float limit /*R0*/) {
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
    return limit * ((float)((RANDOM() >> 16) & 0xFFFFu) / 65536.0f);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SIGNED FLOATING POINT RANDOM
 *
 *PARAMETERS
 *	R0	FL NUMBER
 *RETURNS
 *	R0	FL RANDOM NUMBER -N->+N FLOATING POINT
 *
 */
float SFRAND(float limit /*R0*/) {
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
    return FRAND(limit * 2.0f) - limit;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *RANDOM UNSIGNED 0 TO N-1
 *
 *PARAMETERS
 *	AR2	N  RANGE INPUT (0-FFFF)
 *RETURNS
 *	R0	RANDOM # BETWEEN 0 AND [AR2]
 *
 */
int RANDU0(int range /*AR2*/) {
    int value;

    // asm 00008F03: 	CALL	RANDOM
    value = (int)(RANDOM() >> 16);
    // asm 00008F04: 	LSH	-16,R0
    // asm 00008F05: 	MPYI	AR2,R0
    value *= range;
    // asm 00008F06: 	LSH	-16,R0
    value >>= 16;
    // asm 00008F07: 	RETS
    return value;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *RANDOM UNSIGNED
 *
 *PARAMETERS
 *	AR2	N
 *RETURNS
 *	R0	RANDOM # BETWEEN 1 AND N
 */
int RANDU(int range /*AR2*/) {
    // asm 00008F08: 	CALL	RANDU0
    int value = RANDU0(range);
    // asm 00008F09: 	ADDI	1,R0
    value += 1;
    // asm 00008F0A: 	RETS
    return value;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SIGNED RANDOM
 *
 *PARAMETERS
 *	AR2	N
 *RETURNS
 *	R0	RANDOM # IN RANDGE +/- N
 *
 */
int SRAND(int range /*AR2*/) {
    int doubled_range;
    int value;

    // asm 00008F0B: 	LSH	1,AR2
    doubled_range = range << 1;
    // asm 00008F0C: 	ADDI	1,AR2
    doubled_range += 1;
    // asm 00008F0D: 	CALL	RANDU0
    value = RANDU0(doubled_range);
    // asm 00008F0E: 	LSH	-1,AR2
    doubled_range >>= 1;
    // asm 00008F0F: 	SUBI	AR2,R0
    value -= doubled_range;
    // asm 00008F10: 	RETS
    return value;
}

// *----------------------------------------------------------------------------

/*
;*----------------------------------------------------------------------------
;*RANGE RANDOM
;*PARAMETERS
;*	AR2	LOWER BOUND
;*	R2	UPPER BOUND
;*RETURNS
;*	R0	RANDOM # FROM AR2 TO R2
;*
;RANGRAND:
;	SUBI	AR2,R2,R0
;	ADDI	1,R0
;	CALL	RANDU0
;	ADDI	AR2,R0
;	RETS
;*----------------------------------------------------------------------------
*/

/*
 *----------------------------------------------------------------------------
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
int RANDPER(int probability /*AR2*/) {
    int value;

    // asm 00008F11: 	CALL	RANDOM
    value = (int)(RANDOM() >> 16);
    // asm 00008F12: 	LSH	-16,R0
    // asm 00008F13: 	MPYI	1000,R0
    value *= 1000;
    // asm 00008F14: 	LSH	-16,R0
    value >>= 16;
    // asm 00008F15: 	CMPI	AR2,R0
    if (value < probability) {
        return value;
    }
    // asm 00008F16: 	BC	RANDPX
    // asm 00008F17: 	LDI	0,R0
    return 0;
RANDPX:
    // asm 00008F18: 	RETS
    return value;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
void INIT_LINKED_LIST(void* start_addr /*AR2*/, void** free_list /*R2*/, void** active_list /*R3*/, int length_minus_1 /*RC*/, int size /*RS*/) {
    char* entry;
    void** linkp;
    int i;

    // ZERO ACTIVE POINTER
    *active_list = NULL;

    // GET FREE POINTER
    linkp = free_list;
    entry = (char*)start_addr;

    for (i = 0; i < length_minus_1; ++i) {
        *linkp = entry;
        linkp = (void**)entry;
        entry += size;
    }

    *linkp = NULL;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
void* GET_LLIST(void** free_list, void** active_list) {
    void* elem;
    void* next;

    elem = *free_list;
    SLOCKON(elem == NULL, "GET_LLIST  out of elements");

    next = *(void**)elem;
    *free_list = next; /* and update free list */

    /* insert into the active list */
    *(void**)elem = *active_list;
    *active_list = elem;

    return elem;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ALLOC_LLIST(free_list)
 *GET A LINKED LIST ELEMENT (DO NOT INSERT TO ACTIVE)
 *
 *PARAMETERS
 *	AR2	FREE LIST
 *RETURNS
 *	AR0	LIST ELEMENT (NOT INSERTED INTO ACTIVE LIST)
 *
 */
void ALLOC_LLIST(void) {
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

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *FREE_LLIST(element, active, free)
 *FREE AN ELEMENT OF A LINKED LIST
 *
 *PARAMETERS
 *	AR2	OBJECT
 *	R2	free list
 *
 *
 */
void FREE_LLIST(void) {
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

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DEL_LLIST(element, active, free)
 *DELETE AN ELEMENT OF A LINKED LIST
 *
 *PARAMETERS
 *	AR2	OBJECT
 *	R2	ACTIVE LIST
 *	R3	FREE LIST
 *
 */
void DEL_LLIST(void) {
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

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
void VEHICLE_ANI_INIT(int vehicle_index /*AR2*/, OBJ* obj /*AR4*/) {
    DYNATAB* table;
    DYNAOBJ** link;
    DYNAOBJ* dyna;
    PROC* proc;
    PROC_CONTEXT* ctx;
    int i;

    MAME_ASSERT_MEM(0x00008F66, "d@(AR4+1d)", &obj->radius);

    // asm 00008F66: 	PUSH	AR0
    // asm 00008F67: 	PUSH	AR3
    // asm 00008F68: 	MPYI	VEHTAB_SIZE,AR2
    // asm 00008F69: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 00008F6A: 	LDI	*+AR2(VEHTAB_ANI),AR2
    table = VEHICLE_TABLE[vehicle_index].animation;
    // asm 00008F6B: 	CMPI	0,AR2			;COULD BE A NULL ENTRY (NO ANIMATION)
    // asm 00008F6C: 	BEQ	VANIX
    if (table == NULL) {
        return;
    }
    // asm 00008F6D: 	LDI	O_DYNAMIC,R0	 	;MAKE PARENT OBJECT DYNAMIC
    // asm 00008F6E: 	OR	*+AR4(OFLAGS),R0
    // asm 00008F6F: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= O_DYNAMIC;
    // *INITIALIZE CENTERXYZ,TRANSXYZ,VERTS
    // asm 00008F70: 	LDI	AR4,AR3
    // asm 00008F71: 	ADDI	ODYNALIST,AR3
    link = (DYNAOBJ**)&obj->dynalist;
    // asm 00008F72: 	LDI	*AR2++,RC		;GET DYNAMIC OBJECT COUNT
    // asm 00008F73: 	RPTB	WHEELLP
    for (i = 0; i <= table->count_minus_1; ++i) {
        // asm 00008F74:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
        dyna = GETDYNA();
        // asm 00008F75: 	STI	AR0,*AR3
        *link = dyna;
        // asm 00008F76: 	LDF	*AR2++,R0
        // asm 00008F77: 	STF	R0,*+AR0(DYNACENTERX)
        // asm 00008F78: 	STF	R0,*+AR0(DYNATRANSX)
        dyna->center_x = table->entries[i].center.X;
        dyna->trans_x = table->entries[i].center.X;
        // asm 00008F79: 	LDF	*AR2++,R0
        // asm 00008F7A: 	STF	R0,*+AR0(DYNACENTERY)
        // asm 00008F7B: 	STF	R0,*+AR0(DYNATRANSY)
        dyna->center_y = table->entries[i].center.Y;
        dyna->trans_y = table->entries[i].center.Y;
        // asm 00008F7C: 	LDF	*AR2++,R0
        // asm 00008F7D: 	STF	R0,*+AR0(DYNACENTERZ)
        // asm 00008F7E: 	STF	R0,*+AR0(DYNATRANSZ)
        dyna->center_z = table->entries[i].center.Z;
        dyna->trans_z = table->entries[i].center.Z;
        // asm 00008F7F: 	LDI	*AR2++,R0
        // asm 00008F80: 	STI	R0,*+AR0(DYNANVERTS)
        dyna->nverts = table->entries[i].verts_minus_1;
        // asm 00008F81: 	LDI	*AR2++,R0
        // asm 00008F82: 	STI	R0,*+AR0(DYNAFLAG)
        dyna->flag = table->entries[i].flag;
        MAME_ASSERT_REG(0x00008F82, "R0", &dyna->flag);
        // asm 00008F83: 	STI	AR4,*+AR0(DYNAPARENT)
        dyna->parent = obj;
    WHEELLP:
        // asm 00008F84: LDI	AR0,AR3
        link = &dyna->link;
    }
    // asm 00008F85: 	LDI	0,R0
    // asm 00008F86: 	STI	R0,*AR3			;LAST LINK IS ZERO, DUDES
    *link = NULL;
    // *GET A CAR PROCESS
    // asm 00008F87: 	LDI	*AR2++,R0		;GET PROCESS POINTER
    // asm 00008F88: 	LDI	@CARPROCI,AR2
    // asm 00008F89: 	LDI	DRONE_C|ANI_T,R2	;PID
    // asm 00008F8A: 	CALL	PRC_CREATE_CHILD
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->CARPROC.obj = obj;
    proc = PRC_CREATE_CHILD(CARPROC, DRONE_C | ANI_T, ctx);
    // asm 00008F8B: 	STI	AR0,*+AR4(ORADZ)	;DOUBLING AS A PROC PTR
    obj->radz_ptr = (uintptr_t)proc;
VANIX:
    // asm 00008F8C: 	POP	AR3
    // asm 00008F8D: 	POP	AR0
    // asm 00008F8E: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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

void CARPROC(PROC* p) {
    OBJ* obj;
    CARBLK* carblk;
    MATRIX* front_wheel_matrix;
    MATRIX* rear_wheel_matrix;
    DYNAOBJ* dyna;
    int sleep_ticks;

    switch (p->resume_state) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 00008F90: 	LDI	*+AR4(OCARBLK),AR5
    obj = p->ctx->CARPROC.obj;
    carblk = obj->carblk;
    p->ctx->CARPROC.carblk = carblk;
    // asm 00008F91: 	LDF	0,R6	 		;INIT SPIN RADIANS
    p->ctx->CARPROC.body_x_radians = 0.0f;
    // asm 00008F92: 	LDF	*+AR5(CARSPEED),R0	;INIT SPEED
    // asm 00008F93: 	LDF	R0,R7
    p->ctx->CARPROC.old_car_speed = carblk->speed;
    // asm 00008F94: 	LDF	*+AR4(ORADY),R0
    // asm 00008F95: 	STF	R0,*+AR7(PDATA)		;INIT OLD ORADY
    p->ctx->CARPROC.old_orady = obj->rady;
    // asm 00008F96: 	CLRF	R5			;INITIALIZE BODY Z RADIANS
    // asm 00008F97: 	STF	R5,*+AR7(PDATA+1)	;SAVE Z RADIANS
    p->ctx->CARPROC.body_z_radians = 0.0f;
    // asm 00008F98: 	LDF	0,R0			;INITIALIZE WHEEL X RADIANS
    // asm 00008F99: 	STF	R0,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
    p->ctx->CARPROC.wheel_x_radians = 0.0f;
CARPROCL:
    obj = p->ctx->CARPROC.obj;
    carblk = p->ctx->CARPROC.carblk;
    // asm 00008F9A: 	LDI	3,AR2	  		;SLEEP TIME
    sleep_ticks = 3;
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
    if (((_MODE & MMODE) != MINTRO) && ((_MODE & MSLINE) == 0) && SUSPEND_MODE == SM_HALT) {
        p->ctx->CARPROC.old_car_speed = carblk->speed;
        goto CARSLP;
    }
NCS:
    // asm 00008FA7: 	LDI	*+AR4(ODIST),R0
    // asm 00008FA8: 	CMPI	20000,R0		;FAR OFF JUST SLEEP
    // asm 00008FA9: 	BGT	CARSLP
    if (obj->dist > 20000) {
        goto CARSLP;
    }
    // *GET FRONT WHEEL STEER MATRIX
    // asm 00008FAA: 	LDF	*+AR5(CARTURN),R2
    // asm 00008FAB: 	MPYF	1.5,R2			;BOOST TURN A LITTLE
    // asm 00008FAC: 	LDI	@MATRIXAI,AR2
    // asm 00008FAD: 	CALL	FIND_YMATRIX
    // asm 00008FAE: 	LDI	AR2,AR0
    FIND_YMATRIX(&MATRIXAI, carblk->turn * 1.5f);
    // *GET WHEEL SPIN MATRIX
    // asm 00008FAF: 	LDF	*+AR5(CARSPEED),R2
    // asm 00008FB0: 	MPYF	0.02,R2   		;FUDGE FACTOR
    // asm 00008FB1: 	ADDF	*+AR7(PDATA+2),R2
    // asm 00008FB2: 	STF	R2,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
    p->ctx->CARPROC.wheel_x_radians += carblk->speed * 0.02f;
    // asm 00008FB3: 	LDI	@MATRIXBI,AR2		;GET X SPIN IN MATRIXB
    // asm 00008FB4: 	CALL	FIND_XMATRIX
    FIND_XMATRIX(&MATRIXBI, p->ctx->CARPROC.wheel_x_radians);
    // *CONCAT FOR FRONT WHEELS
    // asm 00008FB5: 	LDI	@MATRIXCI,AR1		;A X B = C
    // asm 00008FB6: 	LDI	AR1,AR6			;SAVE FRONT WHEEL MATRIX PTR
    // asm 00008FB7: 	LDI	AR2,AR3			;SAVE REAR WHEEL MATRIX PTR
    // asm 00008FB8: 	CALL	CONCAT201    		;CONCAT YOUR MATRICES INTO DYNOBJ
    CONCAT201(&MATRIXAI, &MATRIXBI, &MATRIXCI);
    front_wheel_matrix = &MATRIXCI;
    rear_wheel_matrix = &MATRIXBI;
    // *STUFF YOUR DYNAMIC MATRICES
    // asm 00008FB9: 	LDI	*+AR4(ODYNALIST),R0
    dyna = obj->dynalist;
    // asm: 	SLOCKON	Z,"UTIL\CARPROC   dynamic objects not found"
    SLOCKON(dyna == NULL, "UTIL\\CARPROC   dynamic objects not found");

CDTOP:
    // asm 00008FBA: 	LDI	R0,AR0
    // asm 00008FBB: 	LDI	*+AR0(DYNAFLAG),R0
    // asm 00008FBC: 	BN	CDLP			;SHADOW...CONTINUE
    // asm 00008FBD: 	BZ	CARBODY			;HANDLE BODY
    if (dyna->flag < 0) {
        goto CDLP;
    }
    if (dyna->flag == 0) {
        goto CARBODY;
    }
    // asm 00008FBE: 	LDI	AR0,AR2
    // asm 00008FBF: 	ADDI	DYNAMATRIX,AR2
    // asm 00008FC0: 	CMPI	1,R0
    // asm 00008FC1: 	BZ	CARRWHL			;REAR WHEEL
    if (dyna->flag == 1) {
        goto CARRWHL;
    }
    // *STUFF FRONT WHEEL
    // asm 00008FC2: 	LDF	*AR6++,R0
    // asm 00008FC3: 	RPTS	7
    // asm 00008FC4: 	LDF	*AR6++,R0
    // asm 00008FC4:  ||	STF	R0,*AR2++
    // asm 00008FC5:  	STF	R0,*AR2++
    // asm 00008FC6: 	NOP	*AR6--(9)
    // asm 00008FC7: 	B	CDLP
    dyna->omatrix = *(OBJ_MATRIX*)front_wheel_matrix;
    goto CDLP;
    // *STUFF REAR WHEEL
CARRWHL:
    // asm 00008FC8: 	LDF	*AR3++,R0
    // asm 00008FC9: 	RPTS	7
    // asm 00008FCA: 	LDF	*AR3++,R0
    // asm 00008FCA:  ||	STF	R0,*AR2++
    // asm 00008FCB:  	STF	R0,*AR2++
    // asm 00008FCC: 	NOP	*AR3--(9)
    dyna->omatrix = *(OBJ_MATRIX*)rear_wheel_matrix;
CDLP:
    // asm 00008FCD: 	LDI	*AR0,R0
    dyna = dyna->link;
    // asm 00008FCE: 	BNZ	CDTOP
    if (dyna != NULL) {
        goto CDTOP;
    }
    // asm 00008FCF: 	LDI	3,AR2	  		;SLEEP TIME
    sleep_ticks = 3;
    // asm 00008FD0: 	B	CARSLP
    goto CARSLP;

    // *HANDLE BODY
    // *BODY MUST BE LAST
CARBODY:
    // asm 00008FD1: 	CALL	LEAN
    LEAN(p, dyna, obj, carblk);
    // asm 00008FD2: 	LDI	1,AR2
    sleep_ticks = 1;
CARSLP:
    // asm 00008FD3: 	CALL	SLEEP
    SLEEP(sleep_ticks, 1);

    // asm 00008FD4: 	B 	CARPROCL
    goto CARPROCL;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: NTWOPII	.float	-TWOPI */
static float NTWOPII = -TWOPI;

/*
 *
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
void LEAN(PROC* p, DYNAOBJ* dyna, OBJ* obj, CARBLK* carblk) {
    MATRIX* body_x_matrix;
    float delta_speed;
    float x_lean;
    float new_speed;
    float delta_rady;
    float wrap_adjust;
    float z_lean;

    // asm 00008FD6: 	LDI	AR0,AR1
    // asm 00008FD7: 	ADDI	DYNAMATRIX,AR1
    // 	;GET X LEAN (BRAKE/ACCEL)
    // 	;
    // asm 00008FD8: 	LDF	R7,R0
    // asm 00008FD9: 	LDF	*+AR5(CARSPEED),R7	;GET NEW SPEED
    // asm 00008FDA: 	SUBF	R0,R7,R0
    new_speed = carblk->speed;
    delta_speed = new_speed - p->ctx->CARPROC.old_car_speed;
    // asm 00008FDB: 	MPYF	0.06,R0			;CONVERT TO RADIANS
    // asm 00008FDC: 	ADDF	R0,R6
    p->ctx->CARPROC.body_x_radians += delta_speed * 0.06f;
    // asm 00008FDD: 	MPYF	0.25,R6
    // asm 00008FDE: 	NEGF	R6,R2
    x_lean = -(p->ctx->CARPROC.body_x_radians * 0.25f);
    // asm 00008FDF: 	LDI	*+AR5(CAR_AIRF),R0
    // asm 00008FE0: 	OR	*+AR5(CAR_AIRB),R0
    // asm 00008FE1: 	LDFNZ	0,R2 			;ZERO OUT WHEN IN AIR FOLKS
    if (carblk->front_airborne != 0 || carblk->rear_airborne != 0) {
        x_lean = 0.0f;
    }
    // asm 00008FE2: 	LDF	R2,R2			;AMPLIFY ACCELERATION ONLY
    // asm 00008FE3: 	LDFGT	1,R1
    // asm 00008FE4: 	LDFLT	2,R1
    // asm 00008FE5: 	MPYF	R1,R2
    if (x_lean > 0.0f) {
        x_lean *= 1.0f;
    } else if (x_lean < 0.0f) {
        x_lean *= 2.0f;
    }
    // asm 00008FE6: 	LDF	*+AR5(CARRPM),R0      	;REV FACTOR
    // asm 00008FE7: 	MPYF	0.01,R0
    // asm 00008FE8: 	MPYF	-0.05,R0
    // asm 00008FE9: 	ADDF	R0,R2
    x_lean += (carblk->rpm_x100 * 0.01f) * -0.05f;
    // asm 00008FEA: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm 00008FEB: 	LDFGT	0.1,R2
    // asm 00008FEC: 	CMPF	-0.1,R2
    // asm 00008FED: 	LDFLT	-0.1,R2
    if (x_lean > 0.1f) {
        x_lean = 0.1f;
    }
    if (x_lean < -0.1f) {
        x_lean = -0.1f;
    }
    // asm 00008FEE: 	STF	R2,*+AR5(CARXLEAN)
    carblk->x_lean = x_lean;
    // asm 00008FEF: 	LDI	@MATRIXBI,AR2
    // asm 00008FF0: 	CALL	FIND_XMATRIX
    // asm 00008FF1: 	LDI	AR2,AR0			;SAVE MATRIX PTR
    FIND_XMATRIX(&MATRIXBI, x_lean);
    body_x_matrix = &MATRIXBI;
    p->ctx->CARPROC.old_car_speed = new_speed;
    // 	;GET YOUR Z LEAN (CORNERING)
    // 	;
    // asm 00008FF2: 	LDF	*+AR7(PDATA),R4		;OLD ORADY
    // asm 00008FF3: 	LDF	*+AR7(PDATA+1),R5	;Z RADIANS
    // asm 00008FF4: 	LDF	*+AR4(ORADY),R0
    delta_rady = obj->rady - p->ctx->CARPROC.old_orady;
    // asm 00008FF5: 	STF	R0,*+AR7(PDATA)		;SAVE NEW OLD ORADY
    p->ctx->CARPROC.old_orady = obj->rady;
    // asm 00008FF6: 	SUBF	R4,R0			;DELTA ORADY
    // asm 00008FF7: 	LDF	0,R1
    wrap_adjust = 0.0f;
    // asm 00008FF8: 	CMPF	3.14,R0
    // asm 00008FF9: 	LDFGT	@NTWOPII,R1
    if (delta_rady > 3.14f) {
        wrap_adjust = NTWOPII;
    }
    // asm 00008FFA: 	CMPF	-3.14,R0
    // asm 00008FFB: 	LDFLT	@TWOPII,R1
    if (delta_rady < -3.14f) {
        wrap_adjust = TWOPII;
    }
    // asm 00008FFC: 	ADDI	R1,R0			;HANDLE RADIAN WRAPAROUND
    delta_rady += wrap_adjust;
    // asm 00008FFD: 	MPYF	R7,R0			;MULTIPLY BY SPEED FACTOR
    delta_rady *= p->ctx->CARPROC.old_car_speed;
    // asm 00008FFE: 	MPYF	0.06,R0			;CONVERT TO RADIANS
    // asm 00008FFF: 	MPYF	0.1,R0			;CONVERT TO RADIANS
    delta_rady *= 0.06f;
    delta_rady *= 0.1f;
    // asm 00009000: 	ADDF	R0,R5
    p->ctx->CARPROC.body_z_radians += delta_rady;
    // asm 00009001: 	MPYF	0.5,R5
    p->ctx->CARPROC.body_z_radians *= 0.5f;
    // asm 00009002: 	STF	R5,*+AR7(PDATA+1)	;SAVE NEW Z RADIANS
    // asm 00009003: 	NEGF	R5,R2
    z_lean = -p->ctx->CARPROC.body_z_radians;
    // asm 00009004: 	LDI	*+AR5(CAR_AIRF),R0
    // asm 00009005: 	OR	*+AR5(CAR_AIRB),R0
    // asm 00009006: 	LDFNZ	0,R2 			;ZERO OUT WHEN IN AIR FOLKS
    if (carblk->front_airborne != 0 || carblk->rear_airborne != 0) {
        z_lean = 0.0f;
    }
    // asm 00009007: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm 00009008: 	LDFGT	0.1,R2
    // asm 00009009: 	CMPF	-0.1,R2
    // asm 0000900A: 	LDFLT	-0.1,R2
    if (z_lean > 0.1f) {
        z_lean = 0.1f;
    }
    if (z_lean < -0.1f) {
        z_lean = -0.1f;
    }
    // asm 0000900B: 	STF	R2,*+AR5(CARZLEAN)    	;SAVE IT
    carblk->z_lean = z_lean;
    // ;	MPYF	3,R2			;PUMP IT UP
    // asm 0000900C: 	MPYF	2.2,R2			;PUMP IT UP
    z_lean *= 2.2f;
    // asm 0000900D: 	CMPF	0.1,R2			;LIMIT CHECK
    // asm 0000900E: 	LDFGT	0.1,R2
    // asm 0000900F: 	CMPF	-0.1,R2
    // asm 00009010: 	LDFLT	-0.1,R2
    if (z_lean > 0.1f) {
        z_lean = 0.1f;
    }
    if (z_lean < -0.1f) {
        z_lean = -0.1f;
    }
    // asm 00009011: 	LDI	@MATRIXAI,AR2		;GET Z IN TEMP THING
    // asm 00009012: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&MATRIXAI, z_lean);
    // asm 00009013: 	CALL	CONCAT201    		;CONCAT YOUR MATRICES INTO DYNOBJ
    CONCAT201(body_x_matrix, &MATRIXAI, (MATRIX*)&dyna->omatrix);
    // asm 00009014: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DYNAOBJECT DYNAMIC ALLOCATION SUBSYSTEM
 *----------------------------------------------------------------------------
 *
 */
/* asm: DYNALIST	.bss	DYNALIST,NUM_DYNAS*DYNASIZE */
DYNAOBJ DYNALIST[NUM_DYNAS];
/* asm: DYNAFREE	.bss	DYNAFREE,1 */
DYNAOBJ* DYNAFREE;

DYNAOBJ* DYNAACTIVE; // port added

// *----------------------------------------------------------------------------
void DYNAOBJ_INIT(void) {
    INIT_LINKED_LIST(
        DYNALIST,            /* AR2 */
        (void**)&DYNAFREE,   /* R2 */
        (void**)&DYNAACTIVE, /* R3 */
        NUM_DYNAS - 1,       /* RC */
        sizeof(DYNAOBJ)      /* RS */
    );
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
DYNAOBJ* GETDYNA(void) {
    DYNAOBJ* dyna;

    // asm 00009029: 	PUSH	R0
    // ;	LDP	@DYNAFREE
    // asm 0000902A: 	LDI	@DYNAFREE,R0
    // asm 0000902B: 	LDI	R0,AR0
    // asm: 	SLOCKON	Z,"UTIL\GETDYNA   out of dynamic objects"
    // asm 0000902C: 	BZ	GETDYNA_ERR
    dyna = DYNAFREE;
    if (dyna == NULL) {
        return NULL;
    }
    // asm 0000902D: 	LDI	*AR0,R0
    // asm 0000902E: 	STI	R0,@DYNAFREE
    DYNAFREE = dyna->link;
    // asm 0000902F: 	ADDI	DYNAMATRIX,AR0		;INIT YOUR MATRIX FOLKS
    // asm 00009030: 	CALL	INITMAT
    // asm 00009031: 	SUBI	DYNAMATRIX,AR0
    INITMAT((MATRIX*)&dyna->omatrix);
    // asm 00009032: 	CLRI	R0
    // asm 00009033: 	STI	R0,*+AR0(DYNAFLAG)
    dyna->flag = 0;
    // asm 00009034: 	SETC
GETDYNA_X:
    // asm 00009035: 	POP	R0
    // asm 00009036: 	RETS
    return dyna;
GETDYNA_ERR:
    // asm 00009037: 	CLRC
    // asm 00009038: 	B	GETDYNA_X
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    return NULL;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DELETE A DYNA OBJECT
 *
 *PARAMETERS
 *	AR2	POINTER TO DYNA OBJ
 *
 *
 */
void DELDYNA(void) {
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

// *----------------------------------------------------------------------------

CARBLK CARLIST[NUM_CARS];
/* asm: CARFREE	.bss	CARFREE,1 */
CARBLK* CARFREE;
/* asm: CAR_COUNT	.bss	CAR_COUNT,1 */
int CAR_COUNT;

// *----------------------------------------------------------------------------
void CARB_INIT(void) {
    CARBLK* car;
    CARBLK** freep;
    int i;

    // asm:
    freep = &CARFREE;

    for (i = 0; i < NUM_CARS - 1; i++) {
        *freep = &CARLIST[i];
        freep = &CARLIST[i].link;
    }

    *freep = NULL;
    CAR_COUNT = 0;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
CARBLK* GETCAR(void) {
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
    if (CARFREE != NULL) {
        CARBLK* car = CARFREE;

        CARFREE = car->link;
        CAR_COUNT += 1;
        return car;
    }

GETCAR_X:
    // asm 00009056: 	POP	R0
    // asm 00009057: 	RETS
GETCAR_ERR:
    // asm 00009058: 	CLRC
    // asm 00009059: 	B	GETCAR_X
    return NULL;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DELETE A CAR OBJECT
 *
 *PARAMETERS
 *	AR2	POINTER TO CAR OBJ
 *
 */
void DELCAR(void) {
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

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SCAN_OBJECTS(PROC* p) {
    switch (p->resume_state) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }
    // asm 00009063: 	CALL	ISCAN
    ISCAN();
    // asm 00009064: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00009066: 	CALL	OSCAN
    OSCAN();
    // asm 00009067: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00009069: 	B	SCAN_OBJECTS
    REENTER(SCAN_OBJECTS);
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void PUSHALL(void) {
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void POPALL(void) {
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
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
void DISTANCE_2D(void) {
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

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ADD OBJECT VELOCITIES
 *
 *PARAMETERS
 *	AR4	OBJECT
 *
 */
void OVELADD(void) {
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

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ADD N FRAMES X VELOCITY
 */
void OVELNADD(void) {
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

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *MOVE FORWARD
 *PARAMETERS
 *	AR2	MATRIX
 *	R2	DIST
 *	R3	DESTINATION VECTOR
 */
void FORWARD(void) {
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

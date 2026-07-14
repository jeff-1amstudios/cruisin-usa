
#include "leg.h"
#include "../core/machine.h"
#include "discovered_labels.h"
#include "globals.h"
#include "macs.h"
#include "sys.h"
#include "validator.h"
#include "vunit.h"

#include <string.h>

/*
 * Source module: asm/LEG.ASM
 */

void LEG_INIT(void);
static void ELEMENT_INIT(void);
static LEG_SSLL_ENTRY* ELEMENT_GET(void);
static void ELEMENT_ADD(LEG_SSLL_ENTRY* element_entry);
static LEG_PAYLOAD* ELEMENT_DUMP_INTO_LEGMAP(LEG_PAYLOAD* map_entry);
void LEG_GENERATE_MAP(int start_index /*AR0*/, int end_index /*AR1*/);
static LEG_PAYLOAD* LEG_ADD_GROUP(u32* tyco, LEG_PAYLOAD* map_entry, int section_index);
static void GENERATE_LINEAR_DISTANCE(void);

extern MATRIX _MATRIXA;
extern VECTOR _VECTORA;

enum {
    LEG_LINK_WORD = 0,
    LEG_ENTRY_WORD = 1,
};

#define USEFIXED 1
#define USEEXTENDED 0
/* asm: FINISH_ID	.bss	FINISH_ID,1 */
int FINISH_ID;
/*
 *----------------------------------------------------------------------------
 * USEAGE:
 *
 *
 *FOR EACH LEG IN THE SYSTEM THE LEG_GENERATE_MAP ROUTINE IS
 *CALLED TO GENERATE THE LEG MAP.
 *THIS MAP IS IDENTICAL TO HOW THE OBJECTS WILL BE ALLOCATED ON
 *THE DYNAMIC LIST.
 *
 *THE LIST STARTS AT LEGMAP (located in hibss space)
 *AND HAS THE FOLLOWING STRUCTURE:
 *
 *
 *LEG_SIZE	.set	4	;SI
 *	.globl	LEG_MAP	;RAM SPACE
 *MAX_LEG_ELEMENTS	.set	1200
 *
 *THE LAST ENTRY IN THE MAP HAS ALL FIELDS AS
 *0FFFF FFFFh
 *
 *DRONES ON STARTING GRID MUST USE:
 *	.bss	RACER_GRID_START
 *AS THE STARTING INDEX TO ASSUME THE PLAYER
 *WILL BE LOCATED AT.
 *
 *
 *
 */

/* asm: LEG_ELEMENTS	.bss	LEG_ELEMENTS,1 */
int LEG_ELEMENTS;
/* asm: LEG_MAP	hibss	LEG_MAP,MAX_LEG_ELEMENTS*LEG_SIZE */
LEG_PAYLOAD LEG_MAP[MAX_LEG_ELEMENTS];
/* asm: LAST_END_CACHE	.bss	LAST_END_CACHE,1 */
u32* LAST_END_CACHE;
/* asm: LAST_END_INDEX	.bss	LAST_END_INDEX,1 */
int LAST_END_INDEX;
/* asm: LAST_ORIENTATION	.bss	LAST_ORIENTATION,1 */
static int LAST_ORIENTATION;

/*
 *----------------------------------------------------------------------------
 *
 *
 *THIS INITIALIZATION IS FOR EACH GAME, NOT
 *EACH LEG
 *
 *
 */
void LEG_INIT(void) {
    // asm 0000AA60: 	PUSH	R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEG_INIT", 0, 0);
    // asm 0000AA61: 	CLRI	R0
    // asm 0000AA62: 	STI	R0,@LEG_ELEMENTS
    LEG_ELEMENTS = 0;
    // asm 0000AA63: 	LDI	1,R0
    // asm 0000AA64: 	STI	R0,@LAST_END_INDEX
    LAST_END_INDEX = 1;
    // asm 0000AA65: 	LDI	@TYCO_TRKI,R0
    // asm 0000AA66: 	STI	R0,@LAST_END_CACHE
    LAST_END_CACHE = ROM_PTR(TYCO_TRK_ROM);
    // asm 0000AA67: 	POP	R0
    // asm 0000AA68: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
#define MINILL_SIZE 60
/* asm: LEG_SSLL	hibss	LEG_SSLL,MINILL_SIZE*(LEG_SIZE+1) */
LEG_SSLL_ENTRY LEG_SSLL[MINILL_SIZE];
/* asm: LEGFREE	.bss	LEGFREE,1 */
LEG_SSLL_ENTRY* LEGFREE;
/* asm: LEGLL	.bss	LEGLL,1 */
LEG_SSLL_ENTRY* LEGLL;

/*
 *
 *NOT A TRUE LINKED LIST, BECAUSE THERE IS NO DYNAMIC
 *DEALLOCATION (TO ALLOW REALLOCATION)  THE SYSTEM IS
 *RESET WHEN PIECES ARE REALLOCATED.
 *
 *THIS IS FOR THE SAKE OF SPEED
 *
 */
static void ELEMENT_INIT(void) {
    // asm 0000AA69: 	PUSH	R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ELEMENT_INIT", 0, 0);
    // asm 0000AA6A: 	LDL	LEG_SSLL,R0
    // asm 0000AA6B: 	STI	R0,@LEGFREE
    LEGFREE = LEG_SSLL;
    // asm 0000AA6C: 	CLRI	R0
    // asm 0000AA6D: 	STI	R0,@LEGLL
    LEGLL = NULL;
#if DEBUG
    // asm: 	STI	R0,@DBG_LEGCNT
#endif
    // asm 0000AA6E: 	POP	R0
    // asm 0000AA6F: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *RETURNS
 *	AR3	PTR TO ELEMENT
 *
 */
static LEG_SSLL_ENTRY* ELEMENT_GET(void) {
    LEG_SSLL_ENTRY* element_entry;
    // asm 0000AA70: 	PUSH	AR0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ELEMENT_GET", 0, 0);
#if DEBUG
    // asm: 	LDI	@DBG_LEGCNT,AR0		;CHECK TO SEE IF WE ARE ALLOCATING TO MANY
    // asm: 	INC	AR0			;PIECES
    // asm: 	CMPI	MINILL_SIZE,AR0
    // asm: 	BGE	$
    // asm: 	STI	AR0,@DBG_LEGCNT
#endif
    // asm 0000AA71: 	LDI	@LEGFREE,AR3
    element_entry = LEGFREE;
    // asm 0000AA72: 	LDI	AR3,AR0
    LEGFREE = element_entry + 1;
    // asm 0000AA73: 	ADDI	LEG_SIZE+1,AR0
    // asm 0000AA74: 	STI	AR0,@LEGFREE
    // asm 0000AA75: 	POP	AR0
    // asm 0000AA76: 	RETS
    return element_entry;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR3	LIST ELEMENT
 *
 *INSERT LOWEST TO HIGHEST ONTO LEGLL
 *
 */

static void ELEMENT_ADD(LEG_SSLL_ENTRY* element_entry) {
    int element_id;
    LEG_SSLL_ENTRY** link_ptr;
    LEG_SSLL_ENTRY* next_entry;

    // asm 0000AA78: 	LDI	*+AR3(1+LEG_ID),R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ELEMENT_ADD", 0, 0);
    element_id = element_entry->leg.as_fixed.id;
    // asm 0000AA79: 	LDI	@LEGLLI,AR0
    link_ptr = &LEGLL;
R65:
    // asm 0000AA7A: LDI	*AR0,AR1
    next_entry = *link_ptr;
    // asm 0000AA7B: 	CMPI	0,AR1
    if (next_entry != NULL) {
        // asm 0000AA7C: 	BEQ	QT
        // asm 0000AA7D: 	CMPI	*+AR1(1+LEG_ID),R0
        if ((int)next_entry->leg.as_fixed.id <= element_id) {
            // asm 0000AA7E: 	BLT	QT
            // asm 0000AA7F: 	LDI	AR1,AR0
            link_ptr = &next_entry->leg_ssll_next_offset;
            // asm 0000AA80: 	BU	R65
            goto R65;
        }
    }
QT:
    // asm 0000AA81: 	STI	AR1,*AR3
    element_entry->leg_ssll_next_offset = next_entry;
    // asm 0000AA82: 	STI	AR3,*AR0
    *link_ptr = element_entry;
    // asm 0000AA83: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR3	PTR TO LEG MAP ENTRY
 *
 */
static LEG_PAYLOAD* ELEMENT_DUMP_INTO_LEGMAP(LEG_PAYLOAD* map_entry) {
    LEG_SSLL_ENTRY* element_entry;
    LEG_SSLL_ENTRY* current_entry;

    // asm 0000AA84: 	PUSH	R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ELEMENT_DUMP_INTO_LEGMAP", 0, 0);
    // asm 0000AA85: 	PUSH	AR0
    // asm 0000AA86: 	PUSH	AR1
    // asm 0000AA87: 	LDI	@LEGLL,AR0
    element_entry = LEGLL;
EDILL:
    // asm 0000AA88: CMPI	0,AR0
    if (element_entry == NULL) {
        goto ENDIT;
    }
    // asm 0000AA89: 	BEQ	ENDIT
    // asm 0000AA8A: 	LDI	AR0,AR1
    current_entry = element_entry;
    // asm 0000AA8B: 	LDI	*AR0,AR0
    element_entry = element_entry->leg_ssll_next_offset;
    // asm 0000AA8C: 	INC	AR1
    // asm 0000AA8D: 	LDI	LEG_SIZE-1,RC
    // asm 0000AA8E: 	RPTB	K99
    // asm 0000AA8F: 	LDI	*AR1++,R0
K99:
    // asm 0000AA90: STI	R0,*AR3++
    *map_entry++ = current_entry->leg;
    // asm 0000AA91: 	BU	EDILL
    goto EDILL;
ENDIT:
    // asm 0000AA92: 	POP	AR1
    // asm 0000AA93: 	POP	AR0
    // asm 0000AA94: 	POP	R0
    // asm 0000AA95: 	RETS
    return map_entry;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR0	START INDEX
 *	AR1	END INDEX
 *
 *
 */
void LEG_GENERATE_MAP(int start_index /*AR0*/, int end_index /*AR1*/) {
    u32* tyco_ptr;
    int section_index;
    LEG_PAYLOAD* map_entry;
    u32 flag;

    // asm 0000AA96: 	LDI	AR1,R1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEG_GENERATE_MAP", 0, 0);
    MAME_ASSERT_REG(0x0000AA96, "AR0", &start_index);
    MAME_ASSERT_REG(0x0000AA96, "AR1", &end_index);
    // asm 0000AA97: 	ADDI	2,R1
    // asm 0000AA98: 	STI	R1,@FINISH_ID
    FINISH_ID = end_index + 2;
    // asm 0000AA99: 	LDI	@LAST_END_CACHE,AR2
    tyco_ptr = LAST_END_CACHE;
    // asm 0000AA9A: 	LDI	@LAST_END_INDEX,AR4
    section_index = LAST_END_INDEX;
    // 	;GROUP LOOP
GENMAPLP:
    // *
    // *FIND THE TYCOMAP ENTRY APPROPRIATE TO THIS LEG
    // *
    // *
    // *
LPP:
    // asm 0000AA9B: CMPI	AR0,AR4
    if (section_index == start_index) {
        goto DNFND;
    }
    // asm 0000AA9C: 	BEQ	DNFND
    // asm 0000AA9D: 	LDI	*AR2++(6),R1
    flag = *tyco_ptr;
    tyco_ptr += 6;
    MAME_ASSERT_REG(0x0000AA9E, "R1", &flag);
    // asm 0000AA9E: 	TSTB	1,R1
    if ((flag & SC_OVERLAY) != 0) {
        tyco_ptr += 1;
    }
    // asm 0000AA9F: 	BZ	NOOVERA
    // asm 0000AAA0: 	ADDI	1,AR2
NOOVERA:
    // asm 0000AAA1: TSTB	SC_REVERSE,R1
    if ((flag & SC_REVERSE) != 0) {
        tyco_ptr += 4;
    }
    // asm 0000AAA2: 	BZ	CNTA
    // asm 0000AAA3: 	NOP	*AR2++(4)
CNTA:
    // asm 0000AAA4: TSTB	SC_OVER2,R1
    if ((flag & SC_OVER2) != 0) {
        tyco_ptr += 1;
    }
    // asm 0000AAA5: 	BZ	CNTNA
    // asm 0000AAA6: 	NOP	*AR2++			;OVERLAY 2
CNTNA:
    // asm 0000AAA7: 	INC	AR4			;INCREMENT INDEX
    section_index += 1;
    // asm 0000AAA8: 	BU	LPP
    goto LPP;
DNFND:
    // asm 0000AAA9: 	LDI	@LEG_MAPI,AR3
    map_entry = (LEG_PAYLOAD*)LEG_MAPI;
    // *
    // *LOAD TYCO ENTRIES UNTIL LAST LEG IS FOUND
    // *
    // *
LOAD_LP:
    // 	;AR2	TYCO ENTRY
    // 	;AR3	MAP ENTRY
    // asm 0000AAAA: 	CALL	LEG_ADD_GROUP
    MAME_ASSERT_REG(0x0000AAAA, "AR4", &section_index);
    map_entry = LEG_ADD_GROUP(tyco_ptr, map_entry, section_index);
    // asm 0000AAAB: 	LDI	*AR2++(6),R1
    flag = *tyco_ptr;
    tyco_ptr += 6;
    // asm 0000AAAC: 	TSTB	1,R1
    if ((flag & SC_OVERLAY) != 0) {
        tyco_ptr += 1;
    }
    // asm 0000AAAD: 	BZ	NOOVERB
    // asm 0000AAAE: 	ADDI	1,AR2
NOOVERB:
    // asm 0000AAAF: TSTB	SC_REVERSE,R1
    if ((flag & SC_REVERSE) != 0) {
        tyco_ptr += 4;
    }
    // asm 0000AAB0: 	BZ	CNTB
    // asm 0000AAB1: 	NOP	*AR2++(4)
CNTB:
    // asm 0000AAB2: TSTB	SC_OVER2,R1
    if ((flag & SC_OVER2) != 0) {
        tyco_ptr += 1;
    }
    // asm 0000AAB3: 	BZ	CNTNB
    // asm 0000AAB4: 	NOP	*AR2++			;OVERLAY 2
CNTNB:
    // asm 0000AAB5: 	INC	AR4			;INCREMENT INDEX
    section_index += 1;
    // asm 0000AAB6: 	CMPI	AR1,AR4
    if (section_index <= end_index) {
        goto LOAD_LP;
    }
    // asm 0000AAB7: 	BLE	LOAD_LP
    // asm 0000AAB8: 	STI	AR2,@LAST_END_CACHE
    LAST_END_CACHE = tyco_ptr;
    // asm 0000AAB9: 	STI	AR4,@LAST_END_INDEX
    LAST_END_INDEX = section_index;
    // *
    // *LOAD THREE EXTRA LEGS ONTO THE END
    // *
    // asm 0000AABA: 	ADDI	3,AR1
    end_index += 3;
LOAD_LP2:
    // 	;AR2	TYCO ENTRY
    // 	;AR3	MAP ENTRY
    // asm 0000AABB: 	CALL	LEG_ADD_GROUP
    MAME_ASSERT_REG(0x0000AABB, "AR4", &section_index);
    map_entry = LEG_ADD_GROUP(tyco_ptr, map_entry, section_index);
    // asm 0000AABC: 	LDI	*AR2++(6),R1
    flag = *tyco_ptr;
    tyco_ptr += 6;
    // asm 0000AABD: 	TSTB	1,R1
    if ((flag & SC_OVERLAY) != 0) {
        tyco_ptr += 1;
    }
    // asm 0000AABE: 	BZ	NOOVERB2
    // asm 0000AABF: 	ADDI	1,AR2
NOOVERB2:
    // asm 0000AAC0: 	TSTB	SC_REVERSE,R1
    if ((flag & SC_REVERSE) != 0) {
        tyco_ptr += 4;
    }
    // asm 0000AAC1: 	BZ	CNTB2
    // asm 0000AAC2: 	NOP	*AR2++(4)
CNTB2:
    // asm 0000AAC3: TSTB	SC_OVER2,R1
    if ((flag & SC_OVER2) != 0) {
        tyco_ptr += 1;
    }
    // asm 0000AAC4: 	BZ	CNTNB2
    // asm 0000AAC5: 	NOP	*AR2++			;OVERLAY 2
CNTNB2:
    // asm 0000AAC6: 	INC	AR4			;INCREMENT INDEX
    section_index += 1;
    // asm 0000AAC7: 	CMPI	AR1,AR4
    if (section_index <= end_index) {
        goto LOAD_LP2;
    }
    // asm 0000AAC8: 	BLE	LOAD_LP2
    // *
    // *FLAG THE END OF THE LIST
    // *
    // asm 0000AAC9: 	LDI	-1,R0
    map_entry->as_fixed.pos_x = -1;
    // asm 0000AACA: 	RPTS	LEG_SIZE-1
    map_entry->as_fixed.pos_y_with_lane_flag = -1;
    map_entry->as_fixed.pos_z = -1;
    map_entry->as_fixed.id = -1;
    // asm 0000AACB: 	STI	R0,*AR3++
    // *
    // *AND RETURN TO GAME...
    // *
    // asm 0000AACC: 	RETS
}

// *----------------------------------------------------------------------------

/* asm: LEG_FLAG	.bss	LEG_FLAG,1 */
static int LEG_FLAG;
/* asm: LEG_RADY	.bss	LEG_RADY,1 */
float LEG_RADY = 1.0f;
/* asm: LEG_SECTIONIDX	.bss	LEG_SECTIONIDX,1 */
int LEG_SECTIONIDX;

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *PARAMETERS
 *	AR2	PTR TO TYCO ROM ENTRY
 *	AR3	PTR TO LEG MAP ENTRY
 *	AR4	INDEX
 *
 *
 */
static LEG_PAYLOAD* LEG_ADD_GROUP(u32* tyco_ptr /*AR2*/, LEG_PAYLOAD* map_entry /*AR3*/, int section_index /*AR4*/) {
    u32 flag;
    int lane_count;
    word_addr_t group_addr;
    s32* group_ptr;
    int object_count;
    LEG_SSLL_ENTRY* ssll_entry;
    LEG_PAYLOAD* leg;
    float object_radians;
    u32 object_id;
    u32 object_type;

    // asm 0000AACD: 	PUSH	R4
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEG_ADD_GROUP", 0, 0);
    // asm 0000AACE: 	PUSH	R5
    // asm 0000AACF: 	PUSH	R6
    // asm 0000AAD0: 	PUSH	AR0
    // asm 0000AAD1: 	PUSH	AR1
    // asm 0000AAD2: 	PUSH	AR2
    // asm 0000AAD3: 	PUSH	AR3
    // asm 0000AAD4: 	PUSH	AR4
    // asm 0000AAD5: 	PUSH	AR5
    // asm 0000AAD6: 	PUSH	AR7
    // asm 0000AAD7: 	STI	AR4,@LEG_SECTIONIDX
    LEG_SECTIONIDX = section_index;
    // asm 0000AAD8: 	CALL	ELEMENT_INIT
    ELEMENT_INIT();
    // asm 0000AAD9: 	CALL	ELEMENT_GET
    ssll_entry = ELEMENT_GET();
    // asm 0000AADA: 	INC	AR3
    // asm 0000AADB: 	LDI	AR2,AR7			;DEDICATED POINTER
    // asm 0000AADC: 	LDI	*AR7,R5			;LOAD FLAG
    flag = *tyco_ptr;
    MAME_ASSERT_REG(0x0000AADD, "R5", &flag);
    // asm 0000AADD: 	LDI	R5,R6			;LANE COUNT
    // asm 0000AADE: 	RS	7,R6
    lane_count = (flag >> 7) & 1;
    // asm 0000AADF: 	AND	1,R6			;THIS IS THE LANES
    // asm 0000AAE0: 	LDF	*+AR7(TB_RADY),R2
    // asm 0000AAE1: 	STF	R2,@LEG_RADY
    LEG_RADY = TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[4]);
    MAME_ASSERT_REG_FLOAT(0x0000AAE1, "R2", &LEG_RADY);
    // asm 0000AAE2: 	LDI	@MATRIXAI,AR2		;Group rotation matrix
    // asm 0000AAE3: 	CALL	HPFIND_YMATRIX		;require High Precision
    HPFIND_YMATRIX(&MATRIXAI, LEG_RADY);
    // asm 0000AAE4: 	LDI	*+AR7(TB_GROUP),AR5	;Group pointer
    group_ptr = ROM_PTR(tyco_ptr[5]);

    // asm 0000AAE5: 	ADDI	1,AR5			;skip radius
    group_ptr += 1;
    // asm 0000AAE6: 	LDI	*AR5++,R4		;get number of objects to load
    object_count = *group_ptr++;
    MAME_ASSERT_REG(0x0000AAE7, "R4", &object_count);
    // asm 0000AAE7: 	SUBI	1,R4
    object_count -= 1;
L12:
    // asm 0000AAE8: LDI	*AR5++,AR2		;GET MODEL PTR
    group_ptr++;
    // asm 0000AAE9: 	FLOAT	*AR5++,R1		;GET X POSITION

    leg = &ssll_entry->leg;
    // asm 0000AAEA: 	STF	R1,*+AR3(LEG_POSX)
    leg->as_float.pos.X = (float)*group_ptr++;
    // asm 0000AAEB: 	FLOAT	*AR5++,R1		;GET Y POSITION
    leg->as_float.pos.Y = (float)*group_ptr++;
    // asm 0000AAEC: 	TSTB	SC_REVERSE,R5
    LEG_FLAG = flag & SC_REVERSE;
    MAME_ASSERT_REG(0x0000AAED, "R5", &flag);
    // asm 0000AAED: 	BZD	NOTREVERSED
    // asm 0000AAEE: 	STF	R1,*+AR3(LEG_POSY)
    // asm 0000AAEF: 	FLOAT	*AR5++,R1		;GET Z POSITION
    leg->as_float.pos.Z = (float)*group_ptr++;
    // asm 0000AAF0: 	STF	R1,*+AR3(LEG_POSZ)
    if ((flag & SC_REVERSE) == 0) {
        goto NOTREVERSED;
    }
    // 	;---->BZD	NOTREVERSED
    // 	;SPECIAL REVERSED CASE
    // 	;
    // asm 0000AAF1: 	PUSH	AR7
    // asm 0000AAF2: 	TSTB	SC_OVERLAY,R5
    if ((flag & SC_OVERLAY) == 0) {
        tyco_ptr -= 1;
    }
    // asm 0000AAF3: 	BNZ	ISOVER
    // asm 0000AAF4: 	DEC	AR7
ISOVER:
    // asm 0000AAF5: 	LDF	*+AR7(TB_RVS_POSX),R0	;TRANSLATE BY THE NEGATIVE OFFSET
    // asm 0000AAF6: 	ADDF	*+AR3(LEG_POSX),R0	;POSITION (THIS BLOCKS ENDING POSITION)
    // asm 0000AAF7: 	STF	R0,*+AR3(LEG_POSX)
    leg->as_float.pos.X = leg->as_float.pos.X + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[7]);
    MAME_ASSERT_REG_FLOAT(0x0000AAF7, "R0", &leg->as_float.pos.X);
    // asm 0000AAF8: 	LDF	*+AR7(TB_RVS_POSY),R0
    // asm 0000AAF9: 	ADDF	*+AR3(LEG_POSY),R0
    // asm 0000AAFA: 	STF	R0,*+AR3(LEG_POSY)
    leg->as_float.pos.Y = leg->as_float.pos.Y + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[8]);
    // asm 0000AAFB: 	LDF	*+AR7(TB_RVS_POSZ),R0
    // asm 0000AAFC: 	ADDF	*+AR3(LEG_POSZ),R0
    // asm 0000AAFD: 	STF	R0,*+AR3(LEG_POSZ)
    leg->as_float.pos.Z = leg->as_float.pos.Z + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[9]);
    if ((flag & SC_OVERLAY) == 0) {
        tyco_ptr += 1;
    }
    // asm 0000AAFE: 	POP	AR7
ISOVERA:
    // asm 0000AAFF: 	LDI	@MATRIXAI,R2
    // asm 0000AB00: 	LDI	AR3,AR2
    // asm 0000AB01: 	ADDI	LEG_POSX,AR2
    // asm 0000AB02: 	LDI	@VECTORAI,R3
    // asm 0000AB03: 	CALL	MATRIX_MUL		;rotation by occurance matrix
    MATRIX_MUL(&leg->as_float.pos, &MATRIXAI, &VECTORAI);
    // asm 0000AB04: 	LDI	@VECTORAI,AR0
    // asm 0000AB05: 	LDF	*AR0++,R1
    // asm 0000AB06: 	ADDF	*+AR7(TB_POSX),R1
    // asm 0000AB07: 	STF	R1,*+AR3(LEG_POSX)
    leg->as_float.pos.X = VECTORAI.X + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[1]);
    MAME_ASSERT_REG_FLOAT(0x0000AB08, "R1", &leg->as_float.pos.X);
    // asm 0000AB08: 	LDF	*AR0++,R1
    // asm 0000AB09: 	ADDF	*+AR7(TB_POSY),R1
    // asm 0000AB0A: 	STF	R1,*+AR3(LEG_POSY)
    leg->as_float.pos.Y = VECTORAI.Y + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[2]);
    // asm 0000AB0B: 	LDF	*AR0++,R1
    // asm 0000AB0C: 	ADDF	*+AR7(TB_POSZ),R1
    // asm 0000AB0D: 	STF	R1,*+AR3(LEG_POSZ)
    leg->as_float.pos.Z = VECTORAI.Z + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[3]);
    // asm 0000AB0E: 	BU	JOIN_UP
    goto JOIN_UP;
NOTREVERSED:
    // asm 0000AB0F: 	LDI	@MATRIXAI,R2
    // asm 0000AB10: 	LDI	AR3,AR2
    // asm 0000AB11: 	ADDI	LEG_POSX,AR2
    // asm 0000AB12: 	LDI	R2,R3
    // asm 0000AB13: 	LDI	@VECTORAI,R3
    // asm 0000AB14: 	CALL	MATRIX_MUL		;ROTATION BY OCCURANCE MATRIX
    MATRIX_MUL(&leg->as_float.pos, &MATRIXAI, &VECTORAI);
    // asm 0000AB15: 	LDI	@VECTORAI,AR0
    // asm 0000AB16: 	LDF	*AR0++,R1
    // asm 0000AB17: 	ADDF	*+AR7(TB_POSX),R1
    // asm 0000AB18: 	STF	R1,*+AR3(LEG_POSX)
    leg->as_float.pos.X = VECTORAI.X + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[1]);
    MAME_ASSERT_REG_FLOAT(0x0000AB19, "R1", &leg->as_float.pos.X);
    //  asm 0000AB19: 	LDF	*AR0++,R1
    //  asm 0000AB1A: 	ADDF	*+AR7(TB_POSY),R1
    //  asm 0000AB1B: 	STF	R1,*+AR3(LEG_POSY)
    leg->as_float.pos.Y = VECTORAI.Y + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[2]);
    // asm 0000AB1C: 	LDF	*AR0++,R1
    // asm 0000AB1D: 	ADDF	*+AR7(TB_POSZ),R1
    // asm 0000AB1E: 	STF	R1,*+AR3(LEG_POSZ)
    leg->as_float.pos.Z = VECTORAI.Z + TMS320_C3X_SINGLE_TO_FLOAT(tyco_ptr[3]);
JOIN_UP:
    // asm 0000AB1F: LDF	*AR5++,R2		;LOAD OBJECTS RADIANS
    object_radians = TMS320_C3X_SINGLE_TO_FLOAT(*group_ptr++);
    MAME_ASSERT_REG_FLOAT(0x0000AB20, "R2", &object_radians);
    // asm 0000AB20: 	LDI	*AR5++,R1		;LOAD OBJECT ID (GENV STYLE)
    object_id = *group_ptr++;
    MAME_ASSERT_REG(0x0000AB21, "R1", &object_id);
    // asm 0000AB21: 	LDI	R1,R0
    // asm 0000AB22: 	AND	00F00h,R0
    object_type = object_id & 0x0F00;
    MAME_ASSERT_REG(0x0000AB23, "R0", &object_type);
    // asm 0000AB23: 	CMPI	0B00h,R0
    // asm 0000AB24: 	BNE	NOTDYNAROAD
    if (object_type != 0x0B00) {
        goto NOTDYNAROAD;
    }
    // 	;*******************
    // 	;*******************
    // 	;
    // 	;THIS IS THE CASE WE ARE LOOKING FOR
    // 	;
    // 	;*******************
    // 	;*******************
    // 	;
    // 	;add this element to dynamic fLEX list
    // 	;
    // asm 0000AB25: 	LDI	R1,R3
    object_id &= 0xFF;
    // asm 0000AB26: 	LDI	@LEG_SECTIONIDX,R1
    // asm 0000AB27: 	LS	8,R1
    // asm 0000AB28: 	LDI	R3,R0
    // asm 0000AB29: 	AND	0FFh,R0
    // asm 0000AB2A: 	TSTB	SC_REVERSE,R5			;in the case of reversed track
    // asm 0000AB2B: 	BZ	NOTRVSTRK			;we say the index value is
    // asm 0000AB2C: 	SUBRI	255,R0				;255 - index
    if ((flag & SC_REVERSE) != 0) {
        object_id = 255 - object_id;
    }
NOTRVSTRK:
    // asm 0000AB2D: 	OR	R1,R0
    object_id |= LEG_SECTIONIDX << 8;
    // asm 0000AB2E: 	STI	R0,*+AR3(LEG_ID)
    leg->as_fixed.id = object_id;
#if USEFIXED
    // asm 0000AB2F: 	FIX	*+AR3(LEG_POSX),R0
    // asm 0000AB30: 	STI	R0,*+AR3(LEG_POSX)
    leg->as_fixed.pos_x = FIX(leg->as_float.pos.X);

    // asm 0000AB31: 	FIX	*+AR3(LEG_POSY),R0
    // asm 0000AB32: 	ANDN	1,R0
    // asm 0000AB33: 	OR	R6,R0				;# of LANES
    // asm 0000AB34: 	STI	R0,*+AR3(LEG_POSY)
    leg->as_fixed.pos_y_with_lane_flag = ((FIX(leg->as_float.pos.Y) & ~1) | lane_count);
    // asm 0000AB35: 	FIX	*+AR3(LEG_POSZ),R0
    // asm 0000AB36: 	STI	R0,*+AR3(LEG_POSZ)
    object_radians = leg->as_float.pos.Z;
    leg->as_fixed.pos_z = FIX(object_radians);
#endif
    // asm 0000AB37: 	DEC	AR3
    // asm 0000AB38: 	CALL	ELEMENT_ADD
    ELEMENT_ADD(ssll_entry);
    // asm 0000AB39: 	CALL	ELEMENT_GET
    ssll_entry = ELEMENT_GET();
    // asm 0000AB3A: 	INC	AR3
    // ;	NOP	*AR3++(LEG_SIZE)
NOTDYNAROAD:
    // asm 0000AB3B: 	SUBI	1,R4
    object_count -= 1;
    // asm 0000AB3C: 	BGE	L12
    if (object_count >= 0) {
        goto L12;
    }
ACTIVATE_X:
    // asm 0000AB3D: 	POP	AR7
    // asm 0000AB3E: 	POP	AR5
    // asm 0000AB3F: 	POP	AR4
    // asm 0000AB40: 	POP	AR3
    // asm 0000AB41: 	POP	AR2
    // asm 0000AB42: 	POP	AR1
    // asm 0000AB43: 	POP	AR0
    // asm 0000AB44: 	POP	R6
    // asm 0000AB45: 	POP	R5
    // asm 0000AB46: 	POP	R4
    // asm 0000AB47: 	CALL	ELEMENT_DUMP_INTO_LEGMAP
    map_entry = ELEMENT_DUMP_INTO_LEGMAP(map_entry);
    // asm 0000AB48: 	RETS
    return map_entry;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *POST PROCESS LEG MAP
 *
 *
 *
 *
 */
static void GENERATE_LINEAR_DISTANCE(void) {
#if USEEXTENDED
    // asm: 	LDL	LEG_MAP,AR2
    // asm: 	FLOAT	*+AR2(LEG_POSX),R6	;STARTING POSITION
    // asm: 	FLOAT	*+AR2(LEG_POSZ),R7
    // asm: 	CLRF	R4			;ACCUMULATED DISTANCE
GLDLP:
    // asm 0000AB49: LDI	*AR2,R0
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	GLDX
    // asm: 	FLOAT	*+AR2(LEG_POSX),R2	;STARTING POSITION
    // asm: 	FLOAT	*+AR2(LEG_POSZ),R1
    // asm: 	SUBF	R5,R2
    // asm: 	SUBF	R6,R1
    // asm: 	MPYF	R2,R2
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	CALL	SQRT
    // asm: 	ADDF	R0,R4
    // asm: 	FIX	R4,R0
    // asm: 	LS	16,R0
    // asm: 	LDI	*+AR2(LEG_POSY),R6
    // asm: 	LS	16,R6
    // asm: 	RS	16,R6
    // asm: 	OR	R0,R6
    // asm: 	STI	R6,*+AR2(LEG_POSY)
    // asm: 	FLOAT	*+AR2(LEG_POSX),R6	;STARTING POSITION
    // asm: 	FLOAT	*+AR2(LEG_POSZ),R7
    // asm: 	NOP	*AR3++(LEG_SIZE)
    // asm: 	BU	GLDLP
GLDX:
#endif
    // asm 0000AB49: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GENERATE_LINEAR_DISTANCE", 0, 0);
}

// *----------------------------------------------------------------------------

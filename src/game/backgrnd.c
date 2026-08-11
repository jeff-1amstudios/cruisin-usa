#include "backgrnd.h"

#include <math.h>
#include <string.h>

#include "../core/machine.h"
#include "../core/romreader.h"
#include "cmos.h"
#include "cornobj.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "validator.h"
#include "vunit.h"

/*
 * Source module: asm/BACKGRND.ASM
 */

void FIND_STARTING_VALUES(void);
void BGD_INIT(void);
static void BGD_WATCHER(PROC* p);
static u32 BGD_ACTIVATE_TYCOGROUP(tyco_stream_t tyco_ptr /*AR2*/);
static void ADD_TO_NEWLIST(OBJ* obj /*AR4*/);
static void FIND_SUBLIST_START_END(void);
static void APPEND_NEWLIST(void);
static void SHINY_NEWLIST(void);
static void GROUP_DELETE(u32 group_id /*AR2*/);
c3x_reg_t GET_XZ_DISTANCE(VECTOR* v1 /*AR2*/, VECTOR* v2 /*R2*/);
static void BGD_OROUTINE(OBJ* obj /*AR4*/);
static void OVERCAR(OBJ* obj /*AR4*/);
static void CARFORWARD(PROC* p);
static void ROAD_DEBRIS_CREATE_55GAL(OBJ* obj /*AR4*/);
static void ROAD_DEBRIS_CREATE(OBJ* obj /*AR4*/);
static void SMOKE_STACK(OBJ* obj /*AR4*/);
static void CAR_FIRE(OBJ* obj /*AR4*/);
static void DC_MINIFOUNTAIN(OBJ* obj /*AR4*/);
static void DC_FOUNTAIN(OBJ* obj /*AR4*/);
static void WATERFALL(OBJ* obj /*AR4*/);
static void WATERANI_PROC(PROC* p);
static void WATERFALL_SND(PROC* p);
void AMBIENCE_SOUND(void);
void HUNGH_ANI(OBJ* obj /*AR4*/);
static void HUNGH_ANI_REENTER(void);
static void PLACE_ON_ROAD(void);
void RUT_ANI(OBJ* obj /*AR4*/);
static void PLAINANI_PROC_SLOW(PROC* p);
static void FLAGWAVE_TALL(OBJ* obj /*AR4*/);
static void FLAGWAVE(OBJ* obj /*AR4*/);
static void PLAINANI_PROC(PROC* p);
static void OHARE_PLANE(OBJ* obj /*AR4*/);
static void PLANE_FWRD(PROC* p);
static void TRAIN_FWRD_MAKEB(void);
static void TRAIN_FWRD_MAKE(OBJ* obj /*AR4*/);
static void TRAIN_FWRDB(void);
static void TRAIN_FWRD(PROC* p);
void LOAD_SINGLE_SECTION_OFFSET(void);

#define NEW_GROUPI NEW_GROUP
#define DGROUPSI DGROUPS
#define OVERCARLISTI OVERCARLIST
#define SMOKE_ANII SMOKE_ANI
#define CAR_FIRE_ANII CAR_FIRE_ANI
#define DC_MINIFOUNTAIN_ANII DC_MINIFOUNTAIN_ANI
#define DC_FOUNTAIN_ANII DC_FOUNTAIN_ANI
#define WATERFALL_ANII WATERFALL_ANI
#define FLAGANII FLAGANI
#define FLAGANITALLI FLAGANITALL
#define RUT_ANISI RUT_ANIS
#define HUNGH_ANISI HUNGH_ANIS
#define BABE_PALISTI BABE_PALIST
#define ROUTINE_TABI ROUTINE_TAB

typedef struct BGD_OROUTINE_ENTRY {
    u32 object_id;
    void (*func)(OBJ* obj);
} BGD_OROUTINE_ENTRY;

static int SMOKE_ANI[7];
static int CAR_FIRE_ANI[13];
static int DC_MINIFOUNTAIN_ANI[7];
static int DC_FOUNTAIN_ANI[6];
static int BABE_PALIST[10];
static const BGD_OROUTINE_ENTRY ROUTINE_TAB[];

extern MATRIX _MATRIXA;
extern VECTOR _VECTORA;

enum {
    TB_POSX = 1,
    TB_POSY = 2,
    TB_POSZ = 3,
    TB_RADY = 4,
    TB_GROUP = 5,
    TB_GROUPOVERLAY = 6,
    TB_REGSIZE = 7,
    TB_RVS_POSX = 7,
    TB_RVS_POSY = 8,
    TB_RVS_POSZ = 9,
    TB_RVS_RADY = 10,
    TB_RVSSIZE = 11,
};

/* asm: STARTSECTION	.bss	STARTSECTION,1 */
int STARTSECTION;
/* asm: START_POS	.bss	START_POS,3 */
c3x_reg_t START_POS[3];
/* asm: START_RADY	.bss	START_RADY,1 */
c3x_reg_t START_RADY = C3X_INIT(1.0f, 0x0000000000ull);
/* asm: DRIVE_LIST	.bss	DRIVE_LIST,1 */
OBJ* DRIVE_LIST;
/* asm: CAR_LIST	.bss	CAR_LIST,1 */
OBJ* CAR_LIST;
/* asm: SIGN_LIST	.bss	SIGN_LIST,1 */
OBJ* SIGN_LIST;
/* asm: GROUND_LIST	.bss	GROUND_LIST,1 */
OBJ* GROUND_LIST;
/*
 *----------------------------------------------------------------------------
 *DYNAMIC fLEX OBJECTS (THE ACTUAL ROAD OBJECTS (LIST))
 *
 *	OLINK2	GROUP SUBLINK (standard)
 *	OLINK4	DYNATRACK fLEX LINK
 *	OBLINK4	DYNATRACK fLEX BACK LINK  (== OPLINK)
 *	OUSR1	fLEX code
 *
 *
 */
/* asm: NEW_GROUP	.bss	NEW_GROUP,1 */
OBJ* NEW_GROUP;
/* asm: DGROUPS	.bss	DGROUPS,DGRP_SIZE*MAX_DGROUPS */
DGROUP_ENTRY DGROUPS[MAX_DGROUPS];
/* asm: DGROUP_COUNT	.bss	DGROUP_COUNT,1 */
int DGROUP_COUNT;
/* asm: DGROUP_AW	.bss	DGROUP_AW,1 */
tyco_stream_t DGROUP_AW;
/* asm: DYNALIST_TRUEBEGIN	.bss	DYNALIST_TRUEBEGIN,1 */
OBJ* DYNALIST_TRUEBEGIN;
/* asm: DYNALIST_BEGIN	.bss	DYNALIST_BEGIN,1 */
OBJ* DYNALIST_BEGIN;
/* asm: DYNALIST_END	.bss	DYNALIST_END,1 */
OBJ* DYNALIST_END;
/* asm: SUBLIST_BEGIN	.bss	SUBLIST_BEGIN,1 */
OBJ* SUBLIST_BEGIN;
/* asm: SUBLIST_END	.bss	SUBLIST_END,1 */
OBJ* SUBLIST_END;
/* asm: STARTS	.bss	STARTS,1 */
OBJ* STARTS;
// *----------------------------------------------------------------------------
/* asm: SECTIONIDX	.bss	SECTIONIDX,1 */
int SECTIONIDX;
/* asm: TYCO_TRACK	.bss	TYCO_TRACK,1 */
tyco_stream_t TYCO_TRACK;
/* asm: TYCO_TRACK_NTL	.bss	TYCO_TRACK_NTL,1 */
tyco_stream_t TYCO_TRACK_NTL;
/* asm: TYCO_NTL_IDX	.bss	TYCO_NTL_IDX,1 */
int TYCO_NTL_IDX;
/* asm: ATTRACT_ACTIVATE_DIST	.float	15000 */
static const c3x_f32_t ATTRACT_ACTIVATE_DIST = C3X_F32_INIT(15000.0f);
/* asm: ACTIVATE_DIST		.float	5000	;to activate */
/* asm: 	 */
static const c3x_f32_t ACTIVATE_DIST = C3X_F32_INIT(5000.0f);
/* asm: DACT_DIST		.float	80000	;dynamic activate distance */
static const c3x_f32_t DACT_DIST = C3X_F32_INIT(80000.0f);
/* asm: DDACT_DIST		.float	15000	;dynamic activate distance (+ radius) */
static const c3x_f32_t DDACT_DIST = C3X_F32_INIT(15000.0f);
// ;ATTR_DDACT_DIST		.float	35000	;dynamic deactivate distance (+ radius)
/* asm: ATTR_DDACT_DIST		.float	45000	;dynamic deactivate distance (+ radius) */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
static const c3x_f32_t ATTR_DDACT_DIST = C3X_F32_INIT(45000.0f);

// *----------------------------------------------------------------------------
void FIND_STARTING_VALUES(void) {
    tyco_stream_t tyco_ptr;
    int start_section;
    u32 flag;
    int remaining_sections;

    // asm 00003F80: 	LDI	@TYCO_TRKI,AR1
    MAME_ASSERT_FUNCTION_ENTRY();
    tyco_ptr = (tyco_stream_t)ROM_PTR(TYCO_TRK_ROM);
    // asm 00003F81: 	LDI	@STARTSECTION,R0	;first time dont do the crime
    start_section = STARTSECTION; // ;first time dont do the crime
    // asm 00003F82: 	CMPI	0,R0
    if (start_section != 0) {
        // asm 00003F83: 	BEQ	NOWARP
        // asm 00003F84: 	LDI	R0,AR0
        // asm 00003F85: 	DEC	AR0
        remaining_sections = start_section - 1;
    LPP:
        // asm 00003F86: LDI	*AR1++(6),R1
        flag = *tyco_ptr;
        tyco_ptr += 6;
        // asm 00003F87: 	TSTB	1,R1
        // asm 00003F88: 	BZ	NOOVER
        if ((flag & 1) != 0) {
            // asm 00003F89: 	ADDI	1,AR1
            tyco_ptr += 1;
        }
    NOOVER:
        // asm 00003F8A: TSTB	SC_REVERSE,R1
        // asm 00003F8B: 	BZ	CNT
        if ((flag & SC_REVERSE) != 0) {
            // asm 00003F8C: 	NOP	*AR1++(4)
            tyco_ptr += 4;
        }
    CNT:
        // asm 00003F8D: TSTB	SC_OVER2,R1
        // asm 00003F8E: 	BZ	CNTN
        if ((flag & SC_OVER2) != 0) {
            // asm 00003F8F: 	NOP	*AR1++			;OVERLAY 2
            tyco_ptr += 1;
        }
    CNTN:
        // asm 00003F90: DBU	AR0,LPP
        if (remaining_sections-- > 0) {
            goto LPP;
        }
    }
NOWARP:
    // asm 00003F91: 	STI	AR1,@TYCO_TRACK
    TYCO_TRACK = tyco_ptr;
    // asm 00003F92: 	STI	AR1,@DGROUP_AW
    DGROUP_AW = tyco_ptr;
    // asm 00003F93: 	STI	R0,@SECTIONIDX
    SECTIONIDX = start_section;
    // asm 00003F94: 	STI	R0,@TYCO_NTL_IDX
    TYCO_NTL_IDX = start_section;
    // asm 00003F95: 	LDF	*++AR1,R0
    // asm 00003F96: 	STF	R0,@START_POS
    START_POS[0] = ROM_ParseFloat(*++tyco_ptr);
    // asm 00003F97: 	LDF	*++AR1,R0
    // asm 00003F98: 	STF	R0,@START_POS+Y
    START_POS[1] = ROM_ParseFloat(*++tyco_ptr);
    // asm 00003F99: 	LDF	*++AR1,R0
    // asm 00003F9A: 	STF	R0,@START_POS+Z
    START_POS[2] = ROM_ParseFloat(*++tyco_ptr);
    // asm 00003F9B: 	LDF	*++AR1,R2
    // asm 00003F9C: 	STF	R2,@START_RADY
    START_RADY = ROM_ParseFloat(*++tyco_ptr);

    MAME_ASSERT_REG_FLOAT(0x00003F9D, "R2", &START_RADY);
    // asm 00003F9D: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INITIALIZE the background system for the game
 *
 *
 */
void BGD_INIT(void) {
    DGROUP_ENTRY* dgroup_ptr;
    u32 dgroup_head;
    tyco_stream_t tyco_ptr;
    u32 flag;
    int remaining_groups;

    // asm 00003F9E: 	CLRI	R0
    MAME_ASSERT_FUNCTION_ENTRY();
    // asm 00003F9F: 	STI	R0,@DGROUP_COUNT
    DGROUP_COUNT = 0;
    // asm 00003FA0: 	STI	R0,@DYNALIST_END
    DYNALIST_END = NULL;
    // asm 00003FA1: 	STI	R0,@DYNALIST_BEGIN
    DYNALIST_BEGIN = NULL;
    // asm 00003FA2: 	STI	R0,@SIGN_LIST
    SIGN_LIST = NULL;
    // asm 00003FA3: 	STI	R0,@TYCO_NTL_IDX
    TYCO_NTL_IDX = 0;
    // ;	LDI	@_MODE,R1
    // ;	AND	MMODE,R1
    // ;	CMPI	MATTR,R1
    // ;	BEQ	REGULAR_START		;START AT WORLD BEGINING
    // ;	.if	DEBUG
    // ;	.globl	DEBUG_STARTSECTION
    // ;	.data
    // ;DEBUG_STARTSECTION	.word	0
    // ;	.text
    // ;	LDI	@DEBUG_STARTSECTION,R0
    // ;	BZ	L17
    // ;	STI	R0,@STARTSECTION
    // ;	.globl	SECTION_SETUP
    // ;	CALLNZ	SECTION_SETUP
    // ;L17
    // ;	.endif
    // asm 00003FA4: 	CALL	FIND_STARTING_VALUES
    FIND_STARTING_VALUES();
    // asm 00003FA5: 	BU	KKTIL
REGULAR_START: //;kept for attract mode only

    // asm 00003FA6: 	CLRI	R0
    // asm 00003FA7: 	STI	R0,@SECTIONIDX
    // asm 00003FA8: 	STI	R0,@TYCO_NTL_IDX
    // asm 00003FA9: 	LDI	@TYCO_TRKI,R0
    // asm 00003FAA: 	STI	R0,@TYCO_TRACK
    // asm 00003FAB: 	STI	R0,@DGROUP_AW
    // asm 00003FAC: 	CLRF	R0
    // asm 00003FAD: 	STF	R0,@START_POS+X
    // asm 00003FAE: 	STF	R0,@START_POS+Y
    // asm 00003FAF: 	STF	R0,@START_POS+Z
    // asm 00003FB0: 	STF	R0,@START_RADY
KKTIL:
    // asm 00003FB1: 	LDI	@DGROUP_AW,AR2
    tyco_ptr = DGROUP_AW;
    // asm 00003FB2: 	LDI	AR2,AR0
    // asm 00003FB3: 	CALL	BGD_ACTIVATE_TYCOGROUP	;returns top pointer in R0
    dgroup_head = BGD_ACTIVATE_TYCOGROUP(tyco_ptr); // ;returns top pointer in R0
    MAME_ASSERT_REG(0x00003FB4, "R0", &dgroup_head);
    // asm 00003FB4: 	LDI	@DGROUPSI,AR1
    dgroup_ptr = DGROUPS;
    // asm 00003FB5: 	STI	R0,*+AR1(DGRP_HEAD)	;lead object (link by OLINK3)
    dgroup_ptr[0].head = dgroup_head; // ;lead object (link by OLINK3)
    // asm 00003FB6: 	STI	AR0,*+AR1(DGRP_BIN)	;rom struct ptr
    dgroup_ptr[0].bin = tyco_ptr; // ;rom struct ptr
    // asm 00003FB7: 	LDI	@STARTS,R0
    // asm 00003FB8: 	STI	R0,*+AR1(DGRP_FSTART)
    dgroup_ptr[0].fstart = STARTS;
    // asm 00003FB9: 	LDI	@SECTIONIDX,R0
    // asm 00003FBA: 	STI	R0,*+AR1(DGRP_IDX)	;SAVE THE INDEX #
    dgroup_ptr[0].idx = SECTIONIDX; // ;SAVE THE INDEX #
    // asm 00003FBB: 	STI	R0,@TYCO_NTL_IDX
    TYCO_NTL_IDX = SECTIONIDX;
    // asm 00003FBC: 	LDI	@TYCOFLAG,R0
    // asm 00003FBD: 	STI	R0,*+AR1(DGRP_FLAG)	;SAVE THE TYCOFLAG
    dgroup_ptr[0].flag = TYCOFLAG; // ;SAVE THE TYCOFLAG
    // asm 00003FBE: 	INCM	@DGROUP_COUNT
    DGROUP_COUNT += 1;
    // asm 00003FC1: 	LDI	@DGROUP_AW,AR0
    tyco_ptr = DGROUP_AW;
    // asm 00003FC2: 	STI	AR0,@TYCO_TRACK_NTL
    TYCO_TRACK_NTL = tyco_ptr;
    // asm 00003FC3: 	LDI	*AR0++(6),R1
    flag = *tyco_ptr;
    tyco_ptr += 6;
    // asm 00003FC4: 	TSTB	1,R1
    // asm 00003FC5: 	BZ	NOOVERD
    if ((flag & 1) != 0) {
        // asm 00003FC6: 	ADDI	1,AR0
        tyco_ptr += 1;
    }
NOOVERD:
    // asm 00003FC7: 	TSTB	SC_REVERSE,R1
    // asm 00003FC8: 	BZ	CNT55a
    if ((flag & SC_REVERSE) != 0) {
        // asm 00003FC9: 	NOP	*AR0++(4)
        tyco_ptr += 4;
    }
CNT55a:
    // asm 00003FCA: TSTB	SC_OVER2,R1
    // asm 00003FCB: 	BZ	NOO2a
    if ((flag & SC_OVER2) != 0) {
        // asm 00003FCC: 	ADDI	1,AR0
        tyco_ptr += 1;
    }
NOO2a:
    // asm 00003FCD: STI	AR0,@DGROUP_AW
    DGROUP_AW = tyco_ptr;
#if DEBUG
    // asm: 	LDI	@DGROUP_COUNT,R0
    // asm: 	CMPI	1,R0
    // asm: 	SLOCKON	NE,"DGROUP\BACKGRND_INIT ERROR"
#endif
    // asm 00003FCE: 	PUSH	AR6
    // asm 00003FCF: 	LDI	@DGROUPSI,AR1
    // asm 00003FD0: 	LDI	3,AR6
    remaining_groups = 3;
LPP5:
    // asm 00003FD1: 	LDI	@DGROUP_AW,AR2
    tyco_ptr = DGROUP_AW;
    // asm 00003FD2: 	LDI	AR2,AR0
    // asm 00003FD3: 	CALL	BGD_ACTIVATE_TYCOGROUP	;returns top pointer in R0
    dgroup_head = BGD_ACTIVATE_TYCOGROUP(tyco_ptr); // ;returns top pointer in R0
    MAME_ASSERT_REG(0x00003FD4, "R0", &dgroup_head);
    // asm 00003FD4: 	STI	R0,*+AR1(DGRP_HEAD+DGRP_SIZE)	;lead object (link by OLINK3)
    dgroup_ptr[1].head = dgroup_head; // ;lead object (link by OLINK3)
    // asm 00003FD5: 	STI	AR0,*+AR1(DGRP_BIN+DGRP_SIZE)	;rom struct ptr
    dgroup_ptr[1].bin = tyco_ptr; // ;rom struct ptr
    // asm 00003FD6: 	LDI	@STARTS,R0
    // asm 00003FD7: 	STI	R0,*+AR1(DGRP_FSTART+DGRP_SIZE)
    dgroup_ptr[1].fstart = STARTS;
    // asm 00003FD8: 	LDI	@SECTIONIDX,R0
    // asm 00003FD9: 	STI	R0,*+AR1(DGRP_IDX+DGRP_SIZE)	;SAVE THE INDEX #
    dgroup_ptr[1].idx = SECTIONIDX; // ;SAVE THE INDEX #
    // asm 00003FDA: 	LDI	@TYCOFLAG,R0
    // asm 00003FDB: 	STI	R0,*+AR1(DGRP_FLAG+DGRP_SIZE)	;SAVE THE TYCOFLAG
    dgroup_ptr[1].flag = TYCOFLAG; // ;SAVE THE TYCOFLAG
    // asm 00003FDC: 	INCM	@DGROUP_COUNT
    DGROUP_COUNT += 1;
    // asm 00003FDF: 	LDI	@DGROUP_AW,AR0
    tyco_ptr = DGROUP_AW;
    // asm 00003FE0: 	LDI	*AR0++(6),R1
    flag = *tyco_ptr;
    tyco_ptr += 6;
    // asm 00003FE1: 	TSTB	1,R1
    // asm 00003FE2: 	BZ	NOOVERE
    if ((flag & 1) != 0) {
        // asm 00003FE3: 	ADDI	1,AR0
        tyco_ptr += 1;
    }
NOOVERE:
    // asm 00003FE4: TSTB	SC_REVERSE,R1
    // asm 00003FE5: 	BZ	CNT55d
    if ((flag & SC_REVERSE) != 0) {
        // asm 00003FE6: 	NOP	*AR0++(4)
        tyco_ptr += 4;
    }
CNT55d:
    // asm 00003FE7: TSTB	SC_OVER2,R1
    // asm 00003FE8: 	BZ	NOO2d
    if ((flag & SC_OVER2) != 0) {
        // asm 00003FE9: 	ADDI	1,AR0
        tyco_ptr += 1;
    }
NOO2d:
    // asm 00003FEA: STI	AR0,@DGROUP_AW
    DGROUP_AW = tyco_ptr;
    // asm 00003FEB: 	ADDI	DGRP_SIZE,AR1
    dgroup_ptr += 1;
    // asm 00003FEC: 	DBU	AR6,LPP5
    if (remaining_groups-- > 0) {
        goto LPP5;
    }
    // asm 00003FED: 	POP	AR6
    // asm 00003FEE: 	CREATE	BGD_WATCHER,UTIL_C|BACKGRND_T
    PRC_CREATE(BGD_WATCHER, UTIL_C | BACKGRND_T, NULL);
    // asm 00003FF1: 	RETS
}

// *----------------------------------------------------------------------------

static void BGD_WATCHER(PROC* p) {
    TYCOHEADER section_header;
    DGROUP_ENTRY* dgroup;
    tyco_stream_t tyco_ptr;
    const s32* group_ptr;
    OBJ* closest_track_piece;
    u32 dgroup_head;
    u32 flag;
    int mode;
    int routine_index;
    c3x_reg_t distance;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 00003FF4: 	LDI	@_MODE,R0
    // asm 00003FF5: 	AND	MMODE,R0
    mode = _MODE & MMODE;
    // asm 00003FF6: 	CMPI	MGAME,R0
    // asm 00003FF7: 	BEQ	LLKK
    // asm 00003FF8: 	CMPI	MATTR,R0
    // asm 00003FF9: 	BNE	BGD_SLP
    if (mode != MGAME && mode != MATTR) {
        goto BGD_SLP;
    }
LLKK:
    // 	;--------------------------------------------------------------------
    // 	;CHECK #1:	HAVE WE CROSSED A THRESHOLD?
    // 	;
    // 	;if (distance_to(THRESHOLD_POS) < ACTIVATE_DIST)
    // 	;	CALL	routine
    // 	;	increment THRESHOLD CHECK PTR
    // 	;
    // 	;New Method::
    // 	;	if Closest Road Piece ROAD_ID (OUSR1) > Checkpoint ROAD_ID
    // 	;		CALL	routine
    // 	;		increment THRESHOLD CHECK PTR
    // 	;
    // asm 00003FFA: 	LDI	@_MODE,R0
    // asm 00003FFB: 	AND	MMODE,R0
    // asm 00003FFC: 	CMPI	MGAME,R0
    // asm 00003FFD: 	BNE	NO_ACTIVATION
    if ((_MODE & MMODE) != MGAME) {
        goto NO_ACTIVATION;
    }
    // asm 00003FFE: 	LDI	@PLYCBLK,AR0
    // asm 00003FFF: 	LDI	*+AR0(CARTRAK),AR0
    closest_track_piece = OBJREF_TO_PTR(PLYCBLK->closest_track_piece);
    // asm 00004000: 	LDI	*+AR0(OUSR1),R0
    // asm 00004001: 	LDI	@TYCO_NTL_IDX,R1
    // asm 00004002: 	LS	8,R1
    // asm 00004003: 	CMPI	R1,R0
    // asm 00004004: 	BLT	NO_ACTIVATION
    if ((u32)closest_track_piece->usr1 < ((u32)TYCO_NTL_IDX << 8)) {
        goto NO_ACTIVATION;
    }
    // 	;blah.asm
    // asm 00004005: 	LDI	@TYCO_TRACK_NTL,AR2
    tyco_ptr = TYCO_TRACK_NTL;
    // asm 00004006: 	LDI	*AR2,AR0
    // asm 00004007: 	RS	16,AR0
    // asm 00004008: 	AND	0FFh,AR0
    // asm 00004009: 	LDI	AR0,R0
    flag = (u32)*tyco_ptr;
    routine_index = (int)((flag >> 16) & 0xFFu);
    // asm 0000400A: 	CALLNZ	SECTION_ROUTINE
    if (routine_index != 0) {
        SECTION_ROUTINE(routine_index);
    }
    // asm 0000400B: 	LDI	@TYCO_NTL_IDX,R0
    // asm 0000400C:  	INC	R0
    // asm 0000400D: 	STI	R0,@TYCO_NTL_IDX
    TYCO_NTL_IDX += 1;
    // asm 0000400E: 	LDI	@TYCO_TRACK_NTL,AR1
    // asm 0000400F: 	LDI	*AR1++(6),R1
    tyco_ptr = TYCO_TRACK_NTL;
    flag = (u32)*tyco_ptr;
    tyco_ptr += 6;
    // asm 00004010: 	TSTB	1,R1
    // asm 00004011: 	BZ	NOOVERA
    // asm 00004012: 	ADDI	1,AR1
    if ((flag & SC_OVERLAY) != 0) {
        tyco_ptr += 1;
    }
NOOVERA:
    // asm 00004013: TSTB	SC_REVERSE,R1
    // asm 00004014: 	BZ	CNTFF
    // asm 00004015: 	NOP	*AR1++(4)
    if ((flag & SC_REVERSE) != 0) {
        tyco_ptr += 4;
    }
CNTFF:
    // asm 00004016: TSTB	SC_OVER2,R1
    // asm 00004017: 	BZ	NOO2FF
    // asm 00004018: 	ADDI	1,AR1
    if ((flag & SC_OVER2) != 0) {
        tyco_ptr += 1;
    }
NOO2FF:
    // asm 00004019: STI	AR1,@TYCO_TRACK_NTL
    TYCO_TRACK_NTL = tyco_ptr;
    // 	;in the end we probably want this check to insure we dont
    // 	;continue trying to allocated beyond the end of the defined game
    // 	;although this can also be done by a routine...
    // 	;
    // ;	LDI	@SECTIONIDX,R1
    // ;	LDI	@TYCO_LENGTH,R0
    // ;	CMPI	R0,R1
    // ;	BGE	SUICIDE
NO_ACTIVATION:
    // 	;--------------------------------------------------------------------
    // 	;CHECK #2:	SHOULD THE NEXT SECTION BE ACTIVATED?
    // 	;
    // 	;if (distance_to(DGROUP_AW) < DACT_DIST)
    // 	;	CALL	BGD_ACTIVATE_TYCOGROUP(AR2 <- GROUP_POINTER)
    // 	;	SETUP DGROUP STRUCTURE  (APPEND TO END OF LIST)
    // 	;	INCM	@DGROUP_COUNT
    // 	;	increment DGROUP_AW	//DGROUP ACTIVATE WATCH
    // 	;
    // 	;
    // asm 0000401A: 	LDI	@DGROUP_AW,AR0
    tyco_ptr = DGROUP_AW;
    // asm 0000401B: 	LDI	AR0,AR2
    // asm 0000401C: 	ADDI	1,AR2
    ROM_ReadTYCOHEADER(tyco_ptr, &section_header);
    // asm 0000401D: 	LDI	@CAMERAPOSI,R2
    // asm 0000401E: 	CALL	GET_XZ_DISTANCE
    distance = GET_XZ_DISTANCE(&section_header.pos, &CAMERAPOSI);
    MAME_ASSERT_REG_FLOAT(0x0000401F, "R0", &distance);
    // asm 0000401F: 	CMPF	@DACT_DIST,R0
    // asm 00004020: 	BGT	NOACT
    if (C3X_GT(distance, C3X_LDF(DACT_DIST))) {
        goto NOACT;
    }
    // asm 00004021: 	LDI	AR0,AR2
    // asm 00004022: 	CALL	BGD_ACTIVATE_TYCOGROUP	;returns top pointer in R0
    dgroup_head = BGD_ACTIVATE_TYCOGROUP(tyco_ptr); // ;returns top pointer in R0
    MAME_ASSERT_REG(0x00004023, "R0", &dgroup_head);
    // asm 00004023: 	LDI	@DGROUP_COUNT,AR1
    // asm 00004024: 	MPYI	DGRP_SIZE,AR1
    // asm 00004025: 	ADDI	@DGROUPSI,AR1
    dgroup = &DGROUPSI[DGROUP_COUNT];
    // asm 00004026: 	STI	R0,*+AR1(DGRP_HEAD)	;lead object (link by OLINK3)
    dgroup->head = dgroup_head; // ;lead object (link by OLINK3)
    // asm 00004027: 	STI	AR0,*+AR1(DGRP_BIN)	;rom struct ptr
    dgroup->bin = tyco_ptr; // ;rom struct ptr
    // asm 00004028: 	LDI	@SECTIONIDX,R0
    // asm 00004029: 	STI	R0,*+AR1(DGRP_IDX)	;SAVE THE INDEX #
    dgroup->idx = SECTIONIDX; // ;SAVE THE INDEX #
    // asm 0000402A: 	LDI	@TYCOFLAG,R0
    // asm 0000402B: 	STI	R0,*+AR1(DGRP_FLAG)	;SAVE THE TYCOFLAG
    dgroup->flag = TYCOFLAG; // ;SAVE THE TYCOFLAG
    // asm 0000402C: 	LDI	@STARTS,R0
    // asm 0000402D: 	STI	R0,*+AR1(DGRP_FSTART)
    dgroup->fstart = STARTS;
    // asm 0000402E: 	INCM	@DGROUP_COUNT
    DGROUP_COUNT += 1;
    // asm 00004031: 	LDI	*AR0++(6),R1
    flag = (u32)*tyco_ptr;
    tyco_ptr += 6;
    // asm 00004032: 	TSTB	1,R1
    // asm 00004033: 	BZ	NOOVERB
    // asm 00004034: 	ADDI	1,AR0
    if ((flag & SC_OVERLAY) != 0) {
        tyco_ptr += 1;
    }
NOOVERB:
    // asm 00004035: TSTB	SC_REVERSE,R1
    // asm 00004036: 	BZ	CNT55
    // asm 00004037: 	NOP	*AR0++(4)
    if ((flag & SC_REVERSE) != 0) {
        tyco_ptr += 4;
    }
CNT55:
    // asm 00004038: TSTB	SC_OVER2,R1
    // asm 00004039: 	BZ	NOO2
    // asm 0000403A: 	ADDI	1,AR0
    if ((flag & SC_OVER2) != 0) {
        tyco_ptr += 1;
    }
NOO2:
    // asm 0000403B: STPI	AR0,@DGROUP_AW
    DGROUP_AW = tyco_ptr;
    // asm 0000403C: NOACT	;end of activation routine
NOACT:
    // 	;--------------------------------------------------------------------
    // 	;CHECK #3:	SHOULD THE OLDEST SECTION BE DEACTIVATED?
    // 	;
    // 	;if (distance_to(THRESHOLD_POS) < DACT_DIST)
    // 	;	CALL  GROUP_DELETE
    // 	;	PACK DGROUP STRUCTURES
    // 	;	DECM	@DGROUP_COUNT
    // 	;	increment DGROUP_AW	//DGROUP ACTIVATE WATCH
    // 	;
    // 	;
    // asm 0000403C: 	LDI	@(DGROUPS+DGRP_BIN),AR2
    tyco_ptr = DGROUPS[0].bin;
    // asm 0000403D: 	INC	AR2
    ROM_ReadTYCOHEADER(tyco_ptr, &section_header);
    // asm 0000403E: 	LDI	@CAMERAPOSI,R2
    // asm 0000403F: 	CALL	GET_XZ_DISTANCE
    distance = GET_XZ_DISTANCE(&section_header.pos, &CAMERAPOSI);
    MAME_ASSERT_REG_FLOAT(0x00004040, "R0", &distance);
    // asm 00004040: 	LDI	@(DGROUPS+DGRP_BIN),AR0
    // asm 00004041: 	LDI	*+AR0(TB_GROUP),AR0
    group_ptr = ROM_PTR(tyco_ptr[TB_GROUP]);
    // asm 00004042: 	SUBF	*AR0,R0
    distance = C3X_SUB(distance, ROM_ParseFloat(*group_ptr));
    MAME_ASSERT_REG_FLOAT(0x00004043, "R0", &distance);
    // asm 00004043: 	LDI	@_MODE,R1
    // asm 00004044: 	AND	MMODE,R1
    // asm 00004045: 	CMPI	MATTR,R1
    // asm 00004046: 	BNE	GAMECHK
    mode = _MODE & MMODE;
    if (mode != MATTR) {
        goto GAMECHK;
    }
    // asm 00004047: 	CMPF	@ATTR_DDACT_DIST,R0
    // asm 00004048: 	BLT	NODEACT
    if (C3X_LT(distance, C3X_LDF(ATTR_DDACT_DIST))) {
        goto NODEACT;
    }
    // asm 00004049: 	BU	DO_DEL
    goto DO_DEL;
GAMECHK:
    // asm 0000404A: CMPF	@DDACT_DIST,R0
    // asm 0000404B: 	BLT	NODEACT
    if (C3X_LT(distance, C3X_LDF(DDACT_DIST))) {
        goto NODEACT;
    }
DO_DEL:
    // asm 0000404C: 	LDI	@(DGROUPS+DGRP_HEAD),AR2
    // asm 0000404D: 	CALL	GROUP_DELETE
    GROUP_DELETE(DGROUPS[0].head);
    // asm 0000404E: 	LDI	@(DGROUPS+DGRP_SIZE+DGRP_FSTART),AR2
    // asm 0000404F: 	STI	AR2,@DYNALIST_BEGIN
    DYNALIST_BEGIN = DGROUPS[1].fstart;
    // asm 00004050: 	STI	AR2,@DYNALIST_TRUEBEGIN
    DYNALIST_TRUEBEGIN = DYNALIST_BEGIN;
    // asm 00004051: 	CLRI	R0
    // asm 00004052: 	STI	R0,*+AR2(OBLINK4)	;NULL TERMINATE BACK LINK ON LIST
    DYNALIST_BEGIN->blink4 = 0; // ;NULL TERMINATE BACK LINK ON LIST
    // 	;REPACK DGROUP SECTIONS
    // 	;
    // 	;for i = 1 to dgroup_count
    // 	;	memcpy(dgroup[i-1], dgroup[i], sizeof(dgroup))
    // 	;
    // asm 00004053: 	LDI	@DGROUP_COUNT,RC
    // asm 00004054: 	SUBI	2,RC
#if DEBUG
    // asm: 	CMPI	0,RC
    // asm: 	SLOCKON	LT,"BACKGRND\LBACK_WATCH ERROR"
    SLOCKON(DGROUP_COUNT - 2 < 0, "BACKGRND\\BACK_WATCH ERROR");
#endif
    // asm 00004055: 	BLT	SHIFT1
    // asm 00004056: 	LDI	@DGROUPSI,AR0
    // asm 00004057: 	LDI	AR0,AR1
    // asm 00004058: 	ADDI	DGRP_SIZE,AR1
    // asm 00004059: 	RPTB	LL45
SHIFT1:
    // asm 0000405A: LDI	*AR1++,R0
    // asm 0000405B: 	STI	R0,*AR0++
    // asm 0000405C: 	LDI	*AR1++,R0
    // asm 0000405D: 	STI	R0,*AR0++
    // asm 0000405E: 	LDI	*AR1++,R0
    // asm 0000405F: 	STI	R0,*AR0++
    // asm 00004060: 	LDI	*AR1++,R0
    // asm 00004061: 	STI	R0,*AR0++
    // asm 00004062: 	LDI	*AR1++,R0
LL45:
    // asm 00004063: STI	R0,*AR0++
    memmove(DGROUPSI, DGROUPSI + 1, (size_t)(DGROUP_COUNT - 1) * sizeof(DGROUPSI[0]));
    // asm 00004064: 	DECM	@DGROUP_COUNT
    DGROUP_COUNT -= 1;
NODEACT:
#if DEBUG
    // 	;DEBUGGING CHECK TO VERIFY THAT WE NEVER EXCEED
    // 	;THE AMOUNT OF DGROUPs WE CAN HANDLE
    // 	;
    // asm: 	LDI	@DGROUP_COUNT,R0
    // asm: 	SLOCKON	LE,"BACKGRND\LBACK_WATCH ERRONEOUS DGROUP_COUNT LE"
    SLOCKON(DGROUP_COUNT <= 0, "BACKGRND\\BACK_WATCH ERRONEOUS DGROUP_COUNT LE");
    // asm: 	CMPI	MAX_DGROUPS,R0
    // asm: 	SLOCKON	GE,"BACKGRND\LBACK_WATCH ERRONEOUS DGROUP_COUNT GE"
    SLOCKON(DGROUP_COUNT >= MAX_DGROUPS, "BACKGRND\\BACK_WATCH ERRONEOUS DGROUP_COUNT GE");
#endif
BGD_SLP:
    // asm 00004067: 	SLEEP	3
    SLEEP(3, 1);
    // asm 00004069: 	B	BGD_WATCHER
    REENTER(BGD_WATCHER);
}

// *----------------------------------------------------------------------------

/* asm: NEWSUBLIST_TOPB	.bss	NEWSUBLIST_TOPB,1 */
u32 NEWSUBLIST_TOPB;
/* asm: GROUP_RADY	.bss	GROUP_RADY,1 */
static int GROUP_RADY;
/* asm: TYCOFLAG	.bss	TYCOFLAG,1 */
int TYCOFLAG;
/* asm: PASS1	.bss	PASS1,1 */
int PASS1;
/* asm: SECRADY	.bss	SECRADY,1 */
c3x_reg_t SECRADY = C3X_INIT(1.0f, 0x0000000000ull);

static u32 BGD_ACTIVATE_TYCOGROUP(tyco_stream_t tyco_ptr /*AR2*/) {
    tyco_stream_t section_ptr;
    const s32* group_ptr;
    void* romdata;
    OBJ* obj;
    OBJ* corn_obj;
    u32 flag;
    u32 flags;
    uintptr_t usr1;
    int object_count;
    u32 raw_id;
    u32 object_class;

    // asm 0000406B: 	PUSH	R4
    // asm 0000406C: 	PUSH	R5
    // asm 0000406D: 	PUSH	AR0
    // asm 0000406E: 	PUSH	AR1
    // asm 0000406F: 	PUSH	AR2
    // asm 00004070: 	PUSH	AR4
    // asm 00004071: 	PUSH	AR5
    // asm 00004072: 	PUSH	AR7
    // asm 00004073: 	LDI	@SECTIONIDX,R0
    // asm 00004074: 	INC	R0
    // asm 00004075: 	STI	R0,@SECTIONIDX
    SECTIONIDX += 1;
    // *CORNFLAKE CHECK
    // asm 00004076: 	CMPI	4,R0
    // asm 00004077: 	BNE	NOTCORNFLAKE
    if (SECTIONIDX == 4) {
        // asm 00004078: 	PUSH	AR4
        // asm 00004079: 	LDL	cornpops,AR2
        romdata = ROM_PTR(cornpops);
        // asm 0000407A: 	LDI	6700,R2
        // asm 0000407B: 	LDI	-4250,R3
        // asm 0000407C: 	LDI	32600,RC
        // asm 0000407D: 	ADDI	@LVAL,RC
        // asm 0000407E: 	CALL	OBJ_QMAKE
        corn_obj = OBJ_QMAKE(romdata, 6700, -4250, 32600 + 151720);
        SLOCKON(corn_obj == NULL, "BACKGRND\\ACTIVATE TYCOGROUP CORNPOPS");
        // asm 0000407F: 	LDI	AR0,AR4
        if (corn_obj != NULL) {
            // asm 00004080: 	LDF	-0.2,R2
            corn_obj->rad.Y = C3X_STF(C3X_IMM_F32(-0.2f));
            // asm 00004081: 	LDI	AR4,AR2
            // asm 00004082: 	ADDI	OMATRIX,AR2
            // asm 00004083: 	CALL	FIND_YMATRIX
            FIND_YMATRIX(&corn_obj->omatrix, C3X_LDF(corn_obj->rad.Y));
            // asm 00004084: 	LDI	*+AR4(OFLAGS),R0
            // asm 00004085: 	OR	O_1PAL,R0
            corn_obj->flags |= O_1PAL;
            // asm 00004086: 	STI	R0,*+AR4(OFLAGS)
            // asm 00004087: 	LDL	CORNPAL,AR2
            // asm 00004088: 	CALL	PAL_FIND_RAW
            corn_obj->palette = PAL_FIND_RAW((tPAL*)ROM_PTR(CORNPAL));
            // asm 00004089: 	STI	R0,*+AR4(OPAL)
            // asm 0000408A: 	LDI	AR4,AR2
            // asm 0000408B: 	CALL	OBJ_INSERT
            OBJ_INSERT(corn_obj);
            // asm 0000408C: 	LDI	@SECTIONIDX,R0
            // asm 0000408D: 	LS	8,R0
            // asm 0000408E: 	OR	0AAh,R0
            corn_obj->link2 = ((uintptr_t)SECTIONIDX << 8) | 0xAAu;
            // asm 0000408F: 	STI	R0,*+AR4(OLINK2)
            // asm 00004090: 	POP	AR4
        }
    }
NOTCORNFLAKE:
    // *CORNFLAKE END CHECK
    // asm 00004091: 	LDI	-1,R0
    // asm 00004092: 	STI	R0,@SUBLIST_BEGIN
    SUBLIST_BEGIN = NULL;
    // asm 00004093: 	CLRI	R0
    // asm 00004094: 	STI	R0,@PASS1
    PASS1 = 0;
    // asm 00004095: 	STI	R0,@NEW_GROUP
    NEW_GROUP = NULL;
    // asm 00004096: 	LDI	@TYCO_TRACK,AR2
    TYCO_TRACK = tyco_ptr;
    // asm 00004097: 	LDI	AR2,AR7				;DEDICATED POINTER
    section_ptr = tyco_ptr;
    // asm 00004098: 	LDI	*AR7,R0				;LOAD FLAG
    flag = *section_ptr;
    // asm 00004099: 	STI	R0,@TYCOFLAG
    TYCOFLAG = flag;
    // asm 0000409A: 	TSTB	SC_OVERLAY,R0
    // asm 0000409B: 	BZ	NOVRDD
    if ((flag & SC_OVERLAY) != 0) {
        // asm 0000409C: 	ADDI	1,AR2
        tyco_ptr += 1;
    }
NOVRDD:
    // asm 0000409D: TSTB	SC_REVERSE,R0
    // asm 0000409E: 	BZD	REG_LD
    // asm 0000409F: 	NOP
    // asm 000040A0: 	ADDI	TB_REGSIZE-1,AR2
    // asm 000040A1: 	STI	AR2,@TYCO_TRACK
    TYCO_TRACK = tyco_ptr + (TB_REGSIZE - 1);
    // 	;---->	BZD	REG_LD
    // asm 000040A2: 	ADDI	TB_RVSSIZE-TB_REGSIZE,AR2	;REVERSED SECTION HAS LARGER STRUCTURE
    // asm 000040A3: 	STI	AR2,@TYCO_TRACK
    if ((flag & SC_REVERSE) != 0) {
        TYCO_TRACK += TB_RVSSIZE - TB_REGSIZE; // ;REVERSED SECTION HAS LARGER STRUCTURE
    }
REG_LD:
    // asm 000040A4: TSTB	SC_OVER2,R0			;hopefully...
    // asm 000040A5: 	BZ	NOEXTRA
    if ((flag & SC_OVER2) != 0) {
        // asm 000040A6: 	LDI	@TYCO_TRACK,R0
        // asm 000040A7: 	INC	R0
        // asm 000040A8: 	STI	R0,@TYCO_TRACK
        TYCO_TRACK += 1;
    }
NOEXTRA:
    // asm 000040A9: 	LDF	*+AR7(TB_RADY),R0
    // asm 000040AA: 	STF	R0,@SECRADY
    SECRADY = ROM_ParseFloat(section_ptr[TB_RADY]);
    // asm 000040AB: 	LDI	@MATRIXAI,AR2		;Group rotation matrix
    // asm 000040AC: 	LDF	@SECRADY,R2
    MAME_ASSERT_REG_FLOAT(0x000040AD, "R2", &SECRADY);
    // asm 000040AD: 	CALL	HPFIND_YMATRIX		;require High Precision
    HPFIND_YMATRIX(&MATRIXAI, SECRADY);
    // asm 000040AE: 	LDI	*+AR7(TB_GROUP),AR5	;Group pointer
    MAME_ASSERT_REG(0x000040AF, "AR5", &section_ptr[TB_GROUP]);
    group_ptr = ROM_PTR(section_ptr[TB_GROUP]);
    // asm 000040AF: 	ADDI	1,AR5			;skip radius
    group_ptr += 1;
    // asm 000040B0: 	LDI	@SECTIONIDX,R0
    // asm 000040B1: 	LS	8,R0
    // asm 000040B2: 	OR	0AAh,R0
    // asm 000040B3: 	STI	R0,@NEWSUBLIST_TOPB
    NEWSUBLIST_TOPB = ((u32)SECTIONIDX << 8) | 0xAAu;
    // asm 000040B4: 	PUSH	R0
    // asm 000040B5: 	LDI	*AR5++,R4		;get number of objects to load
    object_count = *group_ptr++;
    MAME_ASSERT_REG(0x000040B6, "R4", &object_count);
    // asm: 	SLOCKON	LE,"BACKGRND 1"
    SLOCKON(object_count <= 0, "BACKGRND 1");
    // asm 000040B6: 	SUBI	1,R4
    object_count -= 1;
    // asm 000040B7: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\ACTIVATE TYCOGROUP OUT OF OBJECTS"
    SLOCKON(object_count > OFREECNT, "BACKGRND\\ACTIVATE TYCOGROUP OUT OF OBJECTS");
L12:
    // asm 000040B8: 	LDI	*AR5++,AR2		;GET MODEL PTR
    obj = OBJ_GETE(ROM_PTR(*group_ptr++));
    // asm 000040B9: 	CALL	OBJ_GETE
    // asm: 	SLOCKON	C,"BACKGRND\ACTIVATE   DANGER ERROR *FATAL*"
    SLOCKON(obj == NULL, "BACKGRND\\ACTIVATE   DANGER ERROR *FATAL*");
    // asm 000040BA: 	BC	ACTIVATE_X
    if (obj == NULL) {
        goto ACTIVATE_X;
    }
    // 					;INTERNAL LINK (FOR DEALLOCATION)
    // asm 000040BB: 	LDI	AR0,AR4			;SET OBJECT ROM PTR
    // asm 000040BC: 	POP	R0			;GET LAST BACK LINK
    // asm 000040BD: 	STI	R0,*+AR4(OLINK2)
    obj->link2 = NEWSUBLIST_TOPB;
    // asm 000040BE: 	PUSH	R0
    // asm 000040BF: 	FLOAT	*AR5++,R1		;GET X POSITION
    // asm 000040C0: 	STF	R1,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_FROM_INT(*group_ptr++));
    MAME_ASSERT_REG_FLOAT(0x000040C0, "R1", &obj->pos.X);
    // asm 000040C1: 	FLOAT	*AR5++,R1		;GET Y POSITION
    obj->pos.Y = C3X_STF(C3X_FROM_INT(*group_ptr++));
    MAME_ASSERT_REG_FLOAT(0x000040C5, "R1", &obj->pos.Y);
    // asm 000040C2: 	LDI	@TYCOFLAG,R0
    // asm 000040C3: 	TSTB	SC_REVERSE,R0
    // asm 000040C4: 	BZD	NOTREVERSED
    // asm 000040C5: 	STF	R1,*+AR4(OPOSY)
    // asm 000040C6: 	FLOAT	*AR5++,R1		;GET Z POSITION
    // asm 000040C7: 	STF	R1,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_FROM_INT(*group_ptr++));
    MAME_ASSERT_REG_FLOAT(0x000040C7, "R1", &obj->pos.Z);
    if ((TYCOFLAG & SC_REVERSE) == 0) {
        goto NOTREVERSED;
    }
    // 	;---->BZD	NOTREVERSED
    // 	;
    // 	;	SPECIAL REVERSED CASE
    // 	;
    // asm 000040C8: 	PUSH	AR7
    // asm 000040C9: 	TSTB	SC_OVERLAY,R0
    // asm 000040CA: 	BNZ	ISOVER
    if ((TYCOFLAG & SC_OVERLAY) == 0) {
        // asm 000040CB: 	DEC	AR7
        section_ptr -= 1;
    }
ISOVER:
    // asm 000040CC: 	LDF	*+AR7(TB_RVS_POSX),R0	;TRANSLATE BY THE NEGATIVE OFFSET
    // asm 000040CD: 	ADDF	*+AR4(OPOSX),R0		;POSITION (THIS BLOCKS ENDING POSITION)
    // asm 000040CE: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(obj->pos.X, ROM_ParseFloat(section_ptr[TB_RVS_POSX])));
    // asm 000040CF: 	LDF	*+AR7(TB_RVS_POSY),R0
    // asm 000040D0: 	ADDF	*+AR4(OPOSY),R0
    // asm 000040D1: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_ADD(obj->pos.Y, ROM_ParseFloat(section_ptr[TB_RVS_POSY])));
    // asm 000040D2: 	LDF	*+AR7(TB_RVS_POSZ),R0
    // asm 000040D3: 	ADDF	*+AR4(OPOSZ),R0
    // asm 000040D4: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(obj->pos.Z, ROM_ParseFloat(section_ptr[TB_RVS_POSZ])));
    if ((TYCOFLAG & SC_OVERLAY) == 0) {
        section_ptr += 1;
    }
    // asm 000040D5: 	POP	AR7
    // asm 000040D6: 	LDI	@MATRIXAI,R2
    // asm 000040D7: 	LDI	AR4,AR2
    // asm 000040D8: 	ADDI	OPOSX,AR2
    // asm 000040D9: 	LDI	@VECTORAI,R3
    // asm 000040DA: 	CALL	MATRIX_MUL		;rotation by occurance matrix
    MATRIX_MUL(&obj->pos, &MATRIXAI, &VECTORAI); // ;rotation by occurance matrix
    // asm 000040DB: 	LDI	@VECTORAI,AR0
    // asm 000040DC: 	LDF	*AR0++,R1
    // asm 000040DD: 	ADDF	*+AR7(TB_POSX),R1
    // asm 000040DE: 	STF	R1,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(VECTORAI.X, ROM_ParseFloat(section_ptr[TB_POSX])));
    // asm 000040DF: 	LDF	*AR0++,R1
    // asm 000040E0: 	ADDF	*+AR7(TB_POSY),R1
    // asm 000040E1: 	STF	R1,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_ADD(VECTORAI.Y, ROM_ParseFloat(section_ptr[TB_POSY])));
    MAME_ASSERT_REG_FLOAT(0x000040E1, "R1", &obj->pos.Y);
    // asm 000040E2: 	LDF	*AR0++,R1
    // asm 000040E3: 	ADDF	*+AR7(TB_POSZ),R1
    // asm 000040E4: 	STF	R1,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(VECTORAI.Z, ROM_ParseFloat(section_ptr[TB_POSZ])));
    // asm 000040E5: 	LDF	*AR5++,R2		;GET Y ROT
    // asm 000040E6: 	ADDF	@SECRADY,R2
    // asm 000040E7: 	STF	R2,*+AR4(ORADY)
    obj->rad.Y = C3X_STF(C3X_ADD(ROM_ParseFloat(*group_ptr++), SECRADY));
    MAME_ASSERT_REG_FLOAT(0x000040E8, "R2", &obj->rad.Y);
    // asm 000040E8: 	LDI	AR4,AR2
    // asm 000040E9: 	ADDI	OMATRIX,AR2
    // asm 000040EA: 	CALL	HPFIND_YMATRIX
    HPFIND_YMATRIX(&obj->omatrix, C3X_LDF(obj->rad.Y));
    // asm 000040EB: 	BU	JOIN_UP
    goto JOIN_UP;
NOTREVERSED:
    // asm 000040EC: 	LDI	@MATRIXAI,R2
    // asm 000040ED: 	LDI	AR4,AR2
    // asm 000040EE: 	ADDI	OPOSX,AR2
    // asm 000040EF: 	LDI	R2,R3
    // asm 000040F0: 	LDI	@VECTORAI,R3
    // asm 000040F1: 	CALL	MATRIX_MUL		;ROTATION BY OCCURANCE MATRIX
    MATRIX_MUL(&obj->pos, &MATRIXAI, &VECTORAI); // ;ROTATION BY OCCURANCE MATRIX
    // asm 000040F2: 	LDI	@VECTORAI,AR0
    // asm 000040F3: 	LDF	*AR0++,R1
    // asm 000040F4: 	ADDF	*+AR7(TB_POSX),R1
    // asm 000040F5: 	STF	R1,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(VECTORAI.X, ROM_ParseFloat(section_ptr[TB_POSX])));
    // asm 000040F6: 	LDF	*AR0++,R1
    // asm 000040F7: 	ADDF	*+AR7(TB_POSY),R1
    // asm 000040F8: 	STF	R1,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_ADD(VECTORAI.Y, ROM_ParseFloat(section_ptr[TB_POSY])));
    MAME_ASSERT_REG_FLOAT(0x000040F8, "R1", &obj->pos.Y);
    // asm 000040F9: 	LDF	*AR0++,R1
    // asm 000040FA: 	ADDF	*+AR7(TB_POSZ),R1
    // asm 000040FB: 	STF	R1,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(VECTORAI.Z, ROM_ParseFloat(section_ptr[TB_POSZ])));
    // asm 000040FC: 	LDF	*AR5++,R2		;SET THE RADIANS FOR THE OBJECT
    // asm 000040FD: 	ADDF	@SECRADY,R2
    // asm 000040FE: 	STF	R2,*+AR4(ORADY)
    obj->rad.Y = C3X_STF(C3X_ADD(ROM_ParseFloat(*group_ptr++), SECRADY));
    MAME_ASSERT_REG_FLOAT(0x000040FF, "R2", &obj->rad.Y);
    // asm 000040FF: 	LDI	AR4,AR2
    // asm 00004100: 	ADDI	OMATRIX,AR2
    // asm 00004101: 	CALL	HPFIND_YMATRIX
    HPFIND_YMATRIX(&obj->omatrix, C3X_LDF(obj->rad.Y));
JOIN_UP:
    // asm 00004102: 	LDI	*AR5++,R1		;LOAD OBJECT ID (GENV STYLE)
    raw_id = *group_ptr++;
    // asm 00004103: 	LDI	R1,R2
    // asm 00004104: 	AND	CLASS_M|TYPE_M|SUBTYPE_M,R1
    // asm 00004105: 	STI	R1,*+AR4(OID)
    obj->id = raw_id & (CLASS_M | TYPE_M | SUBTYPE_M);
    // 	;TEST TO SEE IF WE SHOULD CALL A SPECIAL ROUTINE
    // 	;OR PALETTE SHIFT
    // 	;
    // asm 00004106: 	TSTB	BGD_ROUTINE,R2
    // asm 00004107: 	CALLNZ	BGD_OROUTINE
    if ((raw_id & BGD_ROUTINE) != 0) {
        BGD_OROUTINE(obj);
    }
    // asm 00004108: 	LDI	R2,R0
    // asm 00004109: 	RS	16,R2
    // asm 0000410A: 	AND	O_GENVSPEC,R2		;make sure list data is not ORed in
    flags = ((raw_id >> 16) & O_GENVSPEC); // ;make sure list data is not ORed in
    // asm 0000410B: 	TSTB	BGD_BIGOBJ,R0		;BIG OBJECT TEST
    // asm 0000410C: 	BZ	NOTBIGOBJ
    if ((raw_id & BGD_BIGOBJ) != 0) {
        // asm 0000410D: 	LDI	1,R0
        // asm 0000410E: 	LS	O_BIGOBJECT_B,R0
        // asm 0000410F: 	OR	R0,R2
        flags |= (1u << O_BIGOBJECT_B);
    }
NOTBIGOBJ:
    // asm 00004110: 	OR	*+AR4(OFLAGS),R2	;or in the flags
    // asm 00004111: 	STI	R2,*+AR4(OFLAGS)
    obj->flags |= flags; // ;or in the flags
    // asm 00004112: 	LDI	AR4,AR2
    // asm 00004113: 	CALL	OBJ_INSERT			;INSERT THE BABE
    OBJ_INSERT(obj);
    // 	;NOW FIND IF THE OBJECT BELONGS IN A SUPPLIMENTAL
    // 	;LIST.
    // 	;OID =	x3xx	-> DRIVE_SUPP
    // 	;	x4xx	-> BUILD_SUPP
    // 	;
    // asm 00004114: 	LDI	*+AR4(OID),R0
    // asm 00004115: 	AND	CLASS_M,R0
    object_class = obj->id & CLASS_M;
    // asm 00004116: 	CMPI	ROAD_C,R0
    // asm 00004117: 	BNE	NOTDRIVE
    if (object_class == ROAD_C) {
        // asm 00004118: 	LDI	@DRIVE_LIST,AR0
        // asm 00004119: 	STI	AR0,*+AR4(OLINK3)
        // asm 0000411A: 	STI	AR4,@DRIVE_LIST
        obj->link3 = (uintptr_t)DRIVE_LIST;
        DRIVE_LIST = obj;
        // asm 0000411B: 	LDI	1,R1
        // asm 0000411C: 	BUD	DONELISTS
        // asm 0000411D: 	LS	28,R1			;O_ROAD_SUPP
        // asm 0000411E: 	OR	*+AR4(OFLAGS),R1
        // asm 0000411F: 	STI	R1,*+AR4(OFLAGS)
        obj->flags |= (1u << O_DRIVE_SUPP_B); // ;O_ROAD_SUPP
        goto DONELISTS;
    }
NOTDRIVE:
    // asm 00004120: 	CMPI	GROUND_C,R0
    // asm 00004121: 	BNE	NOTGROUND
    if (object_class == GROUND_C) {
        // asm 00004122: 	LDI	@GROUND_LIST,AR0
        // asm 00004123: 	STI	AR0,*+AR4(OLINK3)
        // asm 00004124: 	STI	AR4,@GROUND_LIST
        obj->link3 = (uintptr_t)GROUND_LIST;
        GROUND_LIST = obj;
        // asm 00004125: 	LDI	1,R1
        // asm 00004126: 	LS	O_GROUND_B,R1
        // asm 00004127: 	OR	*+AR4(OFLAGS),R1
        // asm 00004128: 	STI	R1,*+AR4(OFLAGS)
        obj->flags |= (1u << O_GROUND_B);
        // asm 00004129: 	B	DONELISTS
        goto DONELISTS;
    }
NOTGROUND:
    // asm 0000412A: 	CMPI	TSIGN_C,R0
    // asm 0000412B: 	BNE	NOTSIGN
    if (object_class == TSIGN_C) {
        // asm 0000412C: 	LDI	@SIGN_LIST,AR0
        // asm 0000412D: 	STI	AR0,*+AR4(OLINK3)
        // asm 0000412E: 	STI	AR4,@SIGN_LIST
        obj->link3 = (uintptr_t)SIGN_LIST;
        SIGN_LIST = obj;
        // asm 0000412F: 	LDI	1,R1
        // asm 00004130: 	LS	O_SIGN_SUPP_B,R1
        // asm 00004131: 	OR	*+AR4(OFLAGS),R1
        // asm 00004132: 	STI	R1,*+AR4(OFLAGS)
        obj->flags |= (1u << O_SIGN_SUPP_B);
        // asm 00004133: 	B	DONELISTS
        goto DONELISTS;
    }
NOTSIGN:
    // asm 00004134: 	CMPI	0B00h,R0
    // asm 00004135: 	BNE	NOTDYNAROAD
    if (object_class == 0x0B00) {
        // 	;
        // 	;add this element to dynamic fLEX list
        // 	;
        // asm 00004136: 	LDI	@SECTIONIDX,R1
        // asm 00004137: 	LS	8,R1
        // asm 00004138: 	LDI	*+AR4(OID),R0
        // asm 00004139: 	AND	0FFh,R0
        // asm 0000413A: 	LDI	@TYCOFLAG,R2			;in the case of reversed track
        // asm 0000413B: 	TSTB	SC_REVERSE,R2			;we say the index value is
        // asm 0000413C: 	BZ	NOTRVSTRK			;255 - index
        // asm 0000413D: 	SUBRI	255,R0				;
        usr1 = obj->id & 0xFF;
        if ((TYCOFLAG & SC_REVERSE) != 0) {
            usr1 = 255 - usr1;
        }
    NOTRVSTRK:
        // asm 0000413E: 	OR	R1,R0
        // asm 0000413F: 	STI	R0,*+AR4(OUSR1)
        usr1 |= (uintptr_t)(SECTIONIDX << 8);
        obj->usr1 = usr1;
        // asm 00004140: 	LDI	0300h,R0
        // asm 00004141: 	STI	R0,*+AR4(OID)
        obj->id = ROAD_C;
        // asm 00004142: 	LDI	@DRIVE_LIST,AR0
        // asm 00004143: 	STI	AR0,*+AR4(OLINK3)
        // asm 00004144: 	STI	AR4,@DRIVE_LIST
        obj->link3 = (uintptr_t)DRIVE_LIST;
        DRIVE_LIST = obj;
        // asm 00004145: 	LDI	1,R1
        // asm 00004146: 	LS	O_DRIVE_SUPP_B,R1
        // asm 00004147: 	OR	*+AR4(OFLAGS),R1
        // asm 00004148: 	STI	R1,*+AR4(OFLAGS)
        obj->flags |= (1u << O_DRIVE_SUPP_B);
        // asm 00004149: 	CALL	ADD_TO_NEWLIST
        ADD_TO_NEWLIST(obj);
        // asm 0000414A: 	BU	DONELISTS
        goto DONELISTS;
    }
NOTDYNAROAD:
DONELISTS:
    // asm 0000414B: 	SUBI	1,R4
    // asm 0000414C: 	BGE	L12
    object_count -= 1;
    if (object_count >= 0) {
        goto L12;
    }
ACTIVATE_X:
    // asm 0000414D: 	POP	AR0			;clear stack of last item
    // asm 0000414E: 	LDI	@PASS1,R0
    // asm 0000414F: 	BNZ	CHECK2
    if (PASS1 == 0) {
        // asm 00004150: 	LDI	1,R0
        // asm 00004151: 	STI	R0,@PASS1
        PASS1 = 1;
        // asm 00004152: 	LDI	*AR7,R0			;load flag
        flag = *section_ptr;
        // asm 00004153: 	TSTB	SC_OVERLAY,R0
        // asm 00004154: 	BZ	CHECK2II
        if ((flag & SC_OVERLAY) != 0) {
            // asm 00004155: 	LDI	*+AR7(TB_GROUPOVERLAY),AR5	;Group pointer
            group_ptr = ROM_PTR(section_ptr[TB_GROUPOVERLAY]);
            // asm 00004156: 	ADDI	1,AR5			;skip radius
            group_ptr += 1;
            // asm 00004157: 	LDI	@NEWSUBLIST_TOPB,R0
            // asm 00004158: 	PUSH	R0
            // asm 00004159: 	LDI	*AR5++,R4		;get number of objects to load
            object_count = (int)*group_ptr++;
            // asm: 	SLOCKON	LE,"BACKGRND  ERRONEOUS GROUP LOADED"
            SLOCKON(object_count <= 0, "BACKGRND  ERRONEOUS GROUP LOADED");
            // asm 0000415A: 	SUBI	1,R4
            object_count -= 1;
            // asm 0000415B: 	CMPI	@OFREECNT,R4
            // asm: 	SLOCKON	GT,"BACKGRND\ACTIVATE TYCOGROUP OUT OF OBJECTS 2"
            SLOCKON(object_count > OFREECNT, "BACKGRND\\ACTIVATE TYCOGROUP OUT OF OBJECTS 2");
            // asm 0000415C: 	BU	L12
            goto L12;
        }
        goto CHECK2II;
    }
CHECK2:
    // asm 0000415D: 	CMPI	2,R0
    // asm 0000415E: 	BEQ	NOOVERLAYGROUP
    if (PASS1 != 2) {
        // asm 0000415F: 	LDI	2,R0
        // asm 00004160: 	STI	R0,@PASS1
        PASS1 = 2;
        // asm 00004161: 	LDI	*AR7,R0			;load flag
        flag = *section_ptr;
        // asm 00004162: 	TSTB	SC_OVER2,R0
        // asm 00004163: 	BZ	NOOVERLAYGROUP
        if ((flag & SC_OVER2) != 0) {
            // asm 00004164: 	PUSH	IR0
            // asm 00004165: 	TSTB	SC_REVERSE,R0
            // asm 00004166: 	LDIZ	TB_GROUPOVERLAY+1,IR0
            // asm 00004167: 	LDINZ	TB_GROUPOVERLAY+5,IR0
            // asm 00004168: 	LDI	*+AR7(IR0),AR5	;Group pointer
            group_ptr = ROM_PTR(section_ptr[(flag & SC_REVERSE) != 0 ? (TB_GROUPOVERLAY + 5) : (TB_GROUPOVERLAY + 1)]);
            // asm 00004169: 	ADDI	1,AR5			;skip radius
            group_ptr += 1;
            // asm 0000416A: 	TSTB	SC_REVERSE,R0
            // asm 0000416B: 	BZ	UHNO2
            if ((flag & SC_REVERSE) != 0) {
                // asm 0000416C: 	TSTB	SC_OVERLAY,R0
                // asm 0000416D: 	LDIZ	TB_RVS_RADY-1,IR0
                // asm 0000416E: 	LDINZ	TB_RVS_RADY,IR0
                // asm 0000416F: 	LDF	*+AR7(IR0),R0
                // asm 00004170: 	STPF	R0,@SECRADY
                SECRADY = C3X_LOAD(section_ptr[(flag & SC_OVERLAY) != 0 ? TB_RVS_RADY : (TB_RVS_RADY - 1)]);
            }
        UHNO2:
            // asm 00004171: 	POP	IR0
            // asm 00004172: 	LDI	@TYCOFLAG,R0		;overlay 2 is not reversed - EVER!
            // asm 00004173: 	ANDN	SC_REVERSE,R0
            // asm 00004174: 	STI	R0,@TYCOFLAG
            TYCOFLAG &= ~SC_REVERSE;
            // asm 00004175: 	LDI	@NEWSUBLIST_TOPB,R0
            // asm 00004176: 	PUSH	R0
            // asm 00004177: 	LDI	*AR5++,R4		;get number of objects to load
            object_count = (int)*group_ptr++;
            // asm: 	SLOCKON	LE,"BACKGRND  ERRONEOUS GROUP LOADED OVER2"
            SLOCKON(object_count <= 0, "BACKGRND  ERRONEOUS GROUP LOADED OVER2");
            // asm 00004178: 	SUBI	1,R4
            object_count -= 1;
            // asm 00004179: 	CMPI	@OFREECNT,R4
            // asm: 	SLOCKON	GT,"BACKGRND\ACTIVATE TYCOGROUP OUT OF OBJECTS 2"
            SLOCKON(object_count > OFREECNT, "BACKGRND\\ACTIVATE TYCOGROUP OUT OF OBJECTS 2");
            // asm 0000417A: 	BU	L12
            goto L12;
        }
    }
CHECK2II:
    // asm 0000417B: 	CMPI	2,R0
    // asm 0000417C: 	BEQ	NOOVERLAYGROUP
    if (PASS1 != 2) {
        // asm 0000417D: 	LDI	2,R0
        // asm 0000417E: 	STI	R0,@PASS1
        PASS1 = 2;
        // asm 0000417F: 	LDI	*AR7,R0			;load flag
        flag = *section_ptr;
        // asm 00004180: 	TSTB	SC_OVER2,R0
        // asm 00004181: 	BZ	NOOVERLAYGROUP
        if ((flag & SC_OVER2) != 0) {
            // asm 00004182: 	PUSH	IR0
            // asm 00004183: 	TSTB	SC_REVERSE,R0
            // asm 00004184: 	LDIZ	TB_GROUPOVERLAY,IR0
            // asm 00004185: 	LDINZ	TB_GROUPOVERLAY+4,IR0
            // asm 00004186: 	LDI	*+AR7(IR0),AR5		;Group pointer
            group_ptr = ROM_PTR(section_ptr[(flag & SC_REVERSE) != 0 ? (TB_GROUPOVERLAY + 4) : TB_GROUPOVERLAY]);
            // asm 00004187: 	TSTB	SC_REVERSE,R0
            // asm 00004188: 	BZ	UHNO
            if ((flag & SC_REVERSE) != 0) {
                // asm 00004189: 	TSTB	SC_OVERLAY,R0
                // asm 0000418A: 	LDIZ	TB_RVS_RADY-1,IR0
                // asm 0000418B: 	LDINZ	TB_RVS_RADY,IR0
                // asm 0000418C: 	LDF	*+AR7(IR0),R0
                // asm 0000418D: 	STPF	R0,@SECRADY
                SECRADY = C3X_LOAD(section_ptr[(flag & SC_OVERLAY) != 0 ? TB_RVS_RADY : (TB_RVS_RADY - 1)]);
            }
        UHNO:
            // asm 0000418E: 	POP	IR0
            // asm 0000418F: 	ADDI	1,AR5			;skip radius
            group_ptr += 1;
            // asm 00004190: 	LDI	@TYCOFLAG,R0		;overlay 2 is not reversed - EVER!
            // asm 00004191: 	ANDN	SC_REVERSE,R0
            // asm 00004192: 	STI	R0,@TYCOFLAG
            TYCOFLAG &= ~SC_REVERSE;
            // asm 00004193: 	LDI	@NEWSUBLIST_TOPB,R0
            // asm 00004194: 	PUSH	R0
            // asm 00004195: 	LDI	*AR5++,R4		;get number of objects to load
            object_count = (int)*group_ptr++;
            // asm: 	SLOCKON	LE,"BACKGRND  ERRONEOUS GROUP LOADED OVER2"
            SLOCKON(object_count <= 0, "BACKGRND  ERRONEOUS GROUP LOADED OVER2");
            // asm 00004196: 	SUBI	1,R4
            object_count -= 1;
            // asm 00004197: 	CMPI	@OFREECNT,R4
            // asm: 	SLOCKON	GT,"BACKGRND\ACTIVATE TYCOGROUP OUT OF OBJECTS 2"
            SLOCKON(object_count > OFREECNT, "BACKGRND\\ACTIVATE TYCOGROUP OUT OF OBJECTS 2");
            // asm 00004198: 	BU	L12
            goto L12;
        }
    }
NOOVERLAYGROUP:
    // 	;*NOW DO THE FANCY MESS OF:
    // 	;*
    // 	;*	1)	DO WE LOAD A NEW OVERLAY INTO A SECTION?
    // 	;*	2)	WHICH SECTION DO WE OVERLAY IT INTO (1 OR 2)?
    // 	;*	3)	WHICH LOOK DO WE PLACE INTO THE SECTION (DESERT..)?
    // 	;*
    // 	;*
    // asm 00004199: 	LDI	@TYCOFLAG,R0
    // asm 0000419A: 	TSTB	SC_LDSECT,R0
    // asm 0000419B: 	BZ	NO_NEWLOAD
NO_NEWLOAD:
    // asm 0000419C: 	CALL	FIND_SUBLIST_START_END
    FIND_SUBLIST_START_END();
    // asm 0000419D: 	CALL	APPEND_NEWLIST
    APPEND_NEWLIST();
    // asm 0000419E: 	LDI	@NEWSUBLIST_TOPB,R0		;return pointer to 1st object
    // asm 0000419F: 	POP	AR7
    // asm 000041A0: 	POP	AR5
    // asm 000041A1: 	POP	AR4
    // asm 000041A2: 	POP	AR2
    // asm 000041A3: 	POP	AR1
    // asm 000041A4: 	POP	AR0
    // asm 000041A5: 	POP	R5
    // asm 000041A6: 	POP	R4
    // asm 000041A7: 	RETS
    return NEWSUBLIST_TOPB;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ADD AN ELEMENT (OBJECT) TO THE NEW LIST BEING CREATED
 *SEARCH ON ITS OID SO THAT IT IS IN ORDER
 *
 *
 *PARAMETERS
 *	AR4		POINTER TO OBJECT
 *	*AR4(OUSR1)	INDEX (lower 2 nybbles of OID)
 *
 *
 *PLACE IN ASCENDING ORDER IN THE LIST
 *
 *
 */
static void ADD_TO_NEWLIST(OBJ* obj /*AR4*/) {
    OBJ** link_ptr;
    OBJ* prev_obj;
    OBJ* next_obj;
    uintptr_t index_value;

    // asm 000041A8: 	LDI	@NEW_GROUPI,AR1
    // asm 000041A9: 	LDI	*AR1,R0
    if (NEW_GROUP == NULL) {
        // asm 000041AA: 	BNZ	REGADD
        // asm 000041AB: 	STI	AR4,*AR1
        NEW_GROUP = obj;
        // asm 000041AC: 	CLRI	R0
        // asm 000041AD: 	STI	R0,*+AR4(OLINK4)
        obj->link4 = 0;
        // asm 000041AE: 	STI	R0,*+AR4(OBLINK4)
        obj->plink = NULL;
        // asm 000041AF: 	RETS
        return;
    }
REGADD:
    // asm 000041B0: 	SUBI	OLINK4,AR1
    link_ptr = &NEW_GROUP;
    // asm 000041B1: 	LDI	AR1,AR2
    prev_obj = NULL;
    // asm 000041B2: 	LDI	*+AR4(OUSR1),R0		;GET fLEX index number
    index_value = obj->usr1; // ;GET fLEX index number
ADDNOBJL:
    // asm 000041B3: 	LDI	AR1,AR0			;AR0 IS PREVIOUS LINK
    // asm 000041B4: 	LDI	*+AR1(OLINK4),R1	;CHECK END OF LIST
    next_obj = *link_ptr;
    // asm 000041B5: 	BZ	ADDNOBJ_AT_END
    if (next_obj != NULL && next_obj->usr1 < index_value) {
        // asm 000041B6: 	LDI	R1,AR1
        prev_obj = next_obj;
        // asm 000041B7: 	CMPI	*+AR1(OUSR1),R0
        // asm 000041B8: 	BGT	ADDNOBJL
        link_ptr = (OBJ**)&next_obj->link4;
        goto ADDNOBJL;
    }
ADDNOBJ_AT_END:
    // asm 000041B9: 	STI	R1,*+AR4(OLINK4)	;LINK TO NEXT (SUBLIST LINK)
    obj->link4 = (uintptr_t)next_obj; // ;LINK TO NEXT (SUBLIST LINK)
    // asm 000041BA: 	STI	AR4,*+AR0(OLINK4)	;LINK FROM PREVIOUS (SUBLIST LINK)
    *link_ptr = obj; // ;LINK FROM PREVIOUS (SUBLIST LINK)
    // asm 000041BB: 	CMPI	AR2,AR0			;IS IT THE HEAD OF LIST WE LOAD AT
    // asm 000041BC: 	LDIEQ	0,AR0
    // asm 000041BD: 	STI	AR0,*+AR4(OBLINK4)	;BACK LINK
    obj->blink4 = (uintptr_t)prev_obj; // ;BACK LINK
    // asm 000041BE: 	CMPI	0,R1			;IS THE NEXT ELEMENT NULL
    // asm 000041BF: 	BEQ	ISZERO
    if (next_obj != NULL) {
        // asm 000041C0: 	LDI	R1,AR0			;IF NOT THE NEXT ELEMENTS BACK LINK
        // asm 000041C1: 	STI	AR4,*+AR0(OBLINK4)	;  MUST POINT TO NEW ELEMENT
        next_obj->blink4 = (uintptr_t)obj; // ;  MUST POINT TO NEW ELEMENT
    }
ISZERO:;
    // asm 000041C2: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FIND_SUBLIST_START_END(void) {
    OBJ* obj;

    // asm 000041C3: 	PUSH	R1
    // asm 000041C4: 	PUSH	AR0
    // asm 000041C5: 	LDI	@NEW_GROUP,AR0
#if DEBUG
    // asm: 	CMPI	0,AR0
    // asm: 	SLOCKON	EQ,"BACKGRND\FIND_SUBLIST_START_END"
#endif
    // asm 000041C6: 	STPI	AR0,@SUBLIST_BEGIN
    obj = NEW_GROUP;
    SUBLIST_BEGIN = obj;
FSLP:
    // asm 000041C7: 	LDI	*+AR0(OLINK4),R1	;CHECK END OF LIST
    // asm 000041C8: 	BZ	FSDN
    if ((OBJ*)obj->link4 != NULL) {
        // asm 000041C9: 	LDI	R1,AR0
        obj = (OBJ*)obj->link4;
        // asm 000041CA: 	BU	FSLP
        goto FSLP;
    }
FSDN:
    // asm 000041CB: 	STPI	AR0,@SUBLIST_END	;ALWAYS NAME IT AS THE END
    SUBLIST_END = obj; // ;ALWAYS NAME IT AS THE END
    // asm 000041CC: 	POP	AR0
    // asm 000041CD: 	POP	R1
    // asm 000041CE: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *APPEND THE NEW LIST TO THE OLD LIST
 *
 *LINKED VIA OLINK4
 *
 *	DONT FORGET THE BACK LINK
 *
 *	if (DYNALIST empty)  {
 *		DYNALIST_END	<- SUBLIST_END
 *		DYNALIST_TRUE_BEGIN <- SUBLIST_BEGIN
 *		DYNALIST_BEGIN	<- SUBLIST_BEGIN
 *		STARTS  <- SUBLIST_BEGIN
 *	}
 *	else  {
 *		*(DYNALIST_END+OLINK4) <- SUBLIST_BEGIN
 *		DYNALIST_END <- SUBLIST_END	new end of list pointer
 *		STARTS <- SUBLIST_BEGIN
 *	}
 *
 *
 */
static void APPEND_NEWLIST(void) {
    OBJ* old_end;

    // asm 000041CF: 	LDI	@DYNALIST_END,R0
    if (DYNALIST_END == NULL) {
        // asm 000041D0: 	BZ	SHINY_NEWLIST
        SHINY_NEWLIST();
        return;
    }
    // asm 000041D1: 	LDI	R0,AR0
    old_end = DYNALIST_END;
    // asm 000041D2: 	LDI	@SUBLIST_BEGIN,R0
    // asm 000041D3: 	STI	R0,*+AR0(OLINK4)
    old_end->link4 = (uintptr_t)SUBLIST_BEGIN;
    // asm 000041D4: 	LDI	@SUBLIST_END,AR1
    // asm 000041D5: 	STPI	AR1,@DYNALIST_END
    DYNALIST_END = SUBLIST_END;
    // asm 000041D6: 	LDI	R0,AR1
    // asm 000041D7: 	STI	AR0,*+AR1(OBLINK4)
    SUBLIST_BEGIN->blink4 = (uintptr_t)old_end;
    // asm 000041D8: 	STPI	R0,@STARTS
    STARTS = SUBLIST_BEGIN;
    // asm 000041D9: 	RETS
}

static void SHINY_NEWLIST(void) {
    // asm 000041DA: 	LDI	@SUBLIST_END,R0
    // asm 000041DB: 	STPI	R0,@DYNALIST_END
    DYNALIST_END = SUBLIST_END;
    // asm 000041DC: 	LDI	@SUBLIST_BEGIN,R0	;new line, check it out...
    // asm 000041DD: 	STPI	R0,@DYNALIST_TRUEBEGIN
    DYNALIST_TRUEBEGIN = SUBLIST_BEGIN;
    // asm 000041DE: 	LDI	@SUBLIST_BEGIN,R0
    // asm: 	SLOCKON	Z,"BACKGRND\SHINY_NEWLIST"
    // asm 000041DF: 	STPI	R0,@DYNALIST_BEGIN
    DYNALIST_BEGIN = SUBLIST_BEGIN;
    // asm 000041E0: 	STPI	R0,@STARTS
    STARTS = SUBLIST_BEGIN;
    // asm 000041E1: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DELETE GROUP LIST FROM OBJECT LIST
 *
 *	CYCLE OUT THE LRU IN ACTIVE GROUPS
 *
 *PARAMETERS
 *	AR2	ID OF ELEMENTS WITHIN THE LIST
 *
 *search for items in active
 *search for items in idle
 *
 *
 */
static void GROUP_DELETE(u32 group_id /*AR2*/) {
    OBJ** list_link;
    OBJ* obj;
    int list_index;

    // asm 000041E2: 	PUSH	R0
    // asm 000041E3: 	PUSH	R1
    // asm 000041E4: 	PUSH	AR0
    // asm 000041E5: 	PUSH	AR1
    MAME_ASSERT_ARG("AR2", &group_id);
    // asm 000041E6: 	LDI	@OACTIVEI,R1
    // asm 000041E7: 	CALL	DELLP
    list_link = &OACTIVEI;
    list_index = 0;
    goto DELLP;
AFTER_ACTIVE_DELETE:
    // asm 000041E8: 	LDI	@IDLE_LISTI,R1
    // asm 000041E9: 	CALL	DELLP
    list_link = &IDLE_LISTI;
    list_index = 1;
    goto DELLP;
AFTER_IDLE_DELETE:
    // asm 000041EA: 	POP	AR1
    // asm 000041EB: 	POP	AR0
    // asm 000041EC: 	POP	R1
    // asm 000041ED: 	POP	R0
    // asm 000041EE: 	RETS
    goto GROUP_DELETE_RETURN;
DELLP:
    // asm 000041EF: LDI	R1,AR1			;WE MUST FIND DEAD OBJECT TO LINK AROUND
    // asm 000041F0: 	LDI	*AR1,R1
    obj = *list_link;
    // asm 000041F1: 	BZ	NXTCHK
    if (obj == NULL) {
        goto NXTCHK;
    }
    // asm 000041F2: 	LDI	R1,AR0
    // asm 000041F3: 	CMPI	*+AR0(OLINK2),AR2
    // asm 000041F4: 	BNE	DELLP
    if (obj->link2 != group_id) {
        list_link = &obj->link;
        goto DELLP;
    }
    // asm 000041F5: 	LDI	*AR0,R0			;find link from delete-e
    // asm 000041F6: 	STI	R0,*AR1			;LINK AROUND
    *list_link = obj->link;
    // asm 000041F7: 	PUSH	AR2
    // asm 000041F8: 	LDI	AR0,AR2
    // 	;Mirror these checks in OBJ.ASM
    // 	;	'OBJ_DELETE'
    // 	;
    // asm 000041F9: 	LDI	*+AR2(OFLAGS),R0	;KILL PROC ASSOCIATED WITH OBJ
    // asm 000041FA: 	RS	(O_DEBRIS_B+1),R0
    // asm 000041FB: 	CALLC	FREE_RDDEBRIS
    if ((obj->flags & O_DEBRIS) != 0) {
        FREE_RDDEBRIS(obj);
    }
    // asm 000041FC: 	LDI	*+AR2(OFLAGS),R0	;KILL PROC ASSOCIATED WITH OBJ
    // asm 000041FD: 	RS	(O_PROC_B+1),R0
    // asm 000041FE: 	CALLC	OBJ_FREE_PROC
    if ((obj->flags & O_PROC) != 0) {
        OBJ_FREE_PROC(obj);
    }
    // asm 000041FF: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF DRIVE LIST
    // asm 00004200: 	RS	(O_DRIVE_SUPP_B+1),R0
    // asm 00004201: 	CALLC	OBJ_FREE_DRIVE
    if ((obj->flags & O_DRIVE_SUPP) != 0) {
        OBJ_FREE_DRIVE(obj);
    }
    // asm 00004202: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF GROUND LIST
    // asm 00004203: 	RS	(O_GROUND_B+1),R0
    // asm 00004204: 	CALLC	OBJ_FREE_GROUND
    if ((obj->flags & O_GROUND) != 0) {
        OBJ_FREE_GROUND(obj);
    }
    // asm 00004205: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF SIGN LIST
    // asm 00004206: 	RS	(O_SIGN_SUPP_B+1),R0
    // asm 00004207: 	CALLC	OBJ_FREE_SIGN
    if ((obj->flags & O_SIGN_SUPP) != 0) {
        OBJ_FREE_SIGN(obj);
    }
    // asm 00004208: 	POP	AR2
    // asm 00004209: 	CLRI	R0
    // asm 0000420A: 	STI	R0,*+AR0(OLINK2)	;CLEAR SEARCH ID
    obj->link2 = 0;
    // asm 0000420B: 	STI	R0,*+AR0(OFLAGS)
    obj->flags = 0;
    // asm 0000420C: 	LDI	@OFREE,R0
    // asm 0000420D: 	STI	R0,*AR0			;place delete-e on free
    obj->link = OFREE;
    // asm 0000420E: 	STI	AR0,@OFREE
    OFREE = obj;
    // asm 0000420F: 	LDI	@OFREECNT,R0		;INCREMENT FREE OBJECT COUNT
    // asm 00004210: 	ADDI	1,R0
    // asm 00004211: 	STI	R0,@OFREECNT
    OFREECNT += 1;
    // asm 00004212: 	LDI	AR1,R1
    // asm 00004213: 	BU	DELLP
    goto DELLP;
NXTCHK:
    // asm 00004214: 	RETS
    if (list_index == 0) {
        goto AFTER_ACTIVE_DELETE;
    }
    goto AFTER_IDLE_DELETE;
GROUP_DELETE_RETURN:;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR2	VECTOR V1
 *	R2	VECTOR V2
 *RETURNS
 *	R0	(FLOAT) DISTANCE FROM V1 TO V2
 *	(FROM X/Z ONLY)
 *
 *
 */
c3x_reg_t GET_XZ_DISTANCE(VECTOR* v1 /*AR2*/, VECTOR* v2 /*R2*/) {
    c3x_reg_t delta_x;
    c3x_reg_t delta_z;

    // asm 00004215: 	PUSH	AR0
    // asm 00004216: 	PUSH	R3
    // asm 00004217: 	LDI	R2,AR0
    // asm 00004218: 	LDF	*AR0++(2),R3
    delta_x = C3X_LDF(v2->X);
    // asm 00004219: 	SUBF	*AR2++(2),R3
    delta_x = C3X_SUB(delta_x, v1->X);
    MAME_ASSERT_REG_FLOAT(0x0000421A, "R3", &delta_x);
    // asm 0000421A: 	LDF	*AR0++,R2
    delta_z = C3X_LDF(v2->Z);
    // asm 0000421B: 	SUBF	*AR2++,R2
    delta_z = C3X_SUB(delta_z, v1->Z);
    MAME_ASSERT_REG_FLOAT(0x0000421C, "R2", &delta_z);
    // asm 0000421C: 	MPYF	R3,R3
    delta_x = C3X_MUL(delta_x, delta_x);
    // asm 0000421D: 	MPYF	R2,R2
    delta_z = C3X_MUL(delta_z, delta_z);
    // asm 0000421E: 	ADDF	R3,R2
    delta_z = C3X_ADD(delta_z, delta_x);
    // asm 0000421F: 	CALL	SQRT
    // asm 00004220: 	POP	R3
    // asm 00004221: 	POP	AR0
    // asm 00004222: 	RETS
    return SQRT(delta_z);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *BGD_OROUTINE
 *	The object has (or will have) a routine associated with it, search the
 *	ID of the object vs the list and call any routine nessesary.
 *
 *
 *
 *PARAMETERS
 *	AR4	OBJECT (NOT INSERTED)
 *
 *
 *NOTE
 *	ALL REGISTERS MUST BE SAVED
 *
 */
static void BGD_OROUTINE(OBJ* obj /*AR4*/) {
    // asm 00004223: 	PUSH	R0
    // asm 00004224: 	PUSH	R1
    // asm 00004225: 	PUSH	AR0
    // asm 00004226: 	LDI	*+AR4(OID),R0
    // asm 00004227: 	LDI	@ROUTINE_TABI,AR0
    for (const BGD_OROUTINE_ENTRY* entry = ROUTINE_TABI; entry->object_id != 0; entry++) {
        // asm 00004228: BGDORLP	CMPI	*AR0,R0
        if (entry->object_id == obj->id) {
            // asm 0000422D: BGD_RFND	LDI	*+AR0,R0
            // asm 0000422E: 	CALLU	R0
            entry->func(obj);
            break;
        }

        // asm 0000422A: 	LDI	*AR0++(2),R1
        // asm 0000422B: 	BNZ	BGDORLP
    }
    // asm 00004235: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void OVERCAR(OBJ* obj /*AR4*/) {
    // asm 00004236: 	PUSH	R0
    // asm 00004237: 	PUSH	R2
    // asm 00004238: 	PUSH	AR2
    // asm 00004239: 	CREATE	CARFORWARD,22
    // asm 0000423C: 	STI	AR0,*+AR4(OPLINK)
    // asm 0000423D: 	LDI	SPAWNER_C,R0
    // asm 0000423E: 	STI	R0,*+AR0(PID)
    // asm 0000423F: 	LDI	1,R0
    // asm 00004240: 	LS	O_PROC_B,R0
    // asm 00004241: 	OR	*+AR4(OFLAGS),R0
    // asm 00004242: 	ANDN	O_1PAL,R0
    // asm 00004243: 	STI	R0,*+AR4(OFLAGS)
    // asm 00004244: 	RANDN	4
    // asm 00004246: 	ADDI	@OVERCARLISTI,R0
    // asm 00004247: 	LDI	R0,AR2
    // asm 00004248: 	LDI	*AR2,AR2
    // asm 00004249: 	STI	AR2,*+AR4(OROMDATA)
    // asm 0000424A: 	POP	AR2
    // asm 0000424B: 	POP	R2
    // asm 0000424C: 	POP	R0
    // asm 0000424D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OVERCAR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: OVERCARLIST	.word	dcbus,dgtruck,dsbus,dcbus */
/* asm: 	 */
static int OVERCARLIST[] = {
    dcbus_ROM,
    dgtruck_ROM,
    dsbus_ROM,
    dcbus_ROM,
};

static void CARFORWARD(PROC* p) {
    // asm 00004253: 	RANDN	50
    // asm 00004255: 	CMPI	25,R0
    // asm 00004256: 	BLT	CARSUP
    // asm 00004257: 	CALL	CLR_VECTORA
    // asm 00004258: 	FLOAT	16000,R0
    // asm 00004259: 	STF	R0,*+AR2(Z)
    // asm 0000425A: 	LDI	AR2,R3
    // asm 0000425B: 	LDI	AR4,R2
    // asm 0000425C: 	ADDI	OMATRIX,R2
    // asm 0000425D: 	CALL	MATRIX_MUL
    // asm 0000425E: 	LDF	*+AR4(OPOSX),R0
    // asm 0000425F: 	ADDF	*+AR2(X),R0
    // asm 00004260: 	STF	R0,*+AR4(OPOSX)
    // asm 00004261: 	LDF	*+AR4(OPOSY),R0
    // asm 00004262: 	ADDF	*+AR2(Y),R0
    // asm 00004263: 	STF	R0,*+AR4(OPOSY)
    // asm 00004264: 	LDF	*+AR4(OPOSZ),R0
    // asm 00004265: 	ADDF	*+AR2(Z),R0
    // asm 00004266: 	STF	R0,*+AR4(OPOSZ)
    // asm 00004267: 	LDF	*+AR4(ORADY),R2
    // asm 00004268: 	ADDF	PI,R2
    // asm 00004269: 	STF	R2,*+AR4(ORADY)
    // asm 0000426A: 	LDI	AR4,AR2
    // asm 0000426B: 	ADDI	OMATRIX,AR2
    // asm 0000426C: 	CALL	FIND_YMATRIX
CARSUP:
    // asm 0000426D: 	RANDN	500
    // asm 0000426F: 	FLOAT	R0
    // asm 00004270: 	CALL	CLR_VECTORA
    // asm 00004271: 	STF	R0,*+AR2(Z)
    // asm 00004272: 	LDI	AR2,R3
    // asm 00004273: 	LDI	AR4,R2
    // asm 00004274: 	ADDI	OMATRIX,R2
    // asm 00004275: 	CALL	MATRIX_MUL
    // asm 00004276: 	LDF	*+AR4(OPOSX),R0
    // asm 00004277: 	ADDF	*+AR2(X),R0
    // asm 00004278: 	STF	R0,*+AR4(OPOSX)
    // asm 00004279: 	LDF	*+AR4(OPOSY),R0
    // asm 0000427A: 	ADDF	*+AR2(Y),R0
    // asm 0000427B: 	STF	R0,*+AR4(OPOSY)
    // asm 0000427C: 	LDF	*+AR4(OPOSZ),R0
    // asm 0000427D: 	ADDF	*+AR2(Z),R0
    // asm 0000427E: 	STF	R0,*+AR4(OPOSZ)
    // asm 0000427F: 	RANDN	30
    // asm 00004281: 	FLOAT	R0
    // asm 00004282: 	LDF	R0,R6
    // asm 00004283: 	ADDF	50,R6
    // asm 00004284: 	RANDN	3
    // asm 00004286: 	ADDI	1,R0
    // asm 00004287: 	FLOAT	R0
    // asm 00004288: 	MPYF	R0,R6
CARFORWARDLP:
    // asm 00004289: 	CALL	CLR_VECTORA
    // asm 0000428A: 	LDF	R6,R0
    // asm 0000428B: 	MPYF	@NFRAMES,R0
    // asm 0000428C: 	STF	R0,*+AR2(Z)
    // asm 0000428D: 	LDI	AR2,R3
    // asm 0000428E: 	LDI	AR4,R2
    // asm 0000428F: 	ADDI	OMATRIX,R2
    // asm 00004290: 	CALL	MATRIX_MUL
    // asm 00004291: 	LDF	*+AR4(OPOSX),R0
    // asm 00004292: 	ADDF	*+AR2(X),R0
    // asm 00004293: 	STF	R0,*+AR4(OPOSX)
    // asm 00004294: 	LDF	*+AR4(OPOSY),R0
    // asm 00004295: 	ADDF	*+AR2(Y),R0
    // asm 00004296: 	STF	R0,*+AR4(OPOSY)
    // asm 00004297: 	LDF	*+AR4(OPOSZ),R0
    // asm 00004298: 	ADDF	*+AR2(Z),R0
    // asm 00004299: 	STF	R0,*+AR4(OPOSZ)
    // asm 0000429A: 	SLEEP	1
    // asm 0000429C: 	BU	CARFORWARDLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARFORWARD", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void ROAD_DEBRIS_CREATE_55GAL(OBJ* obj /*AR4*/) {
    // asm 0000429D: 	PUSH	R0
    // asm 0000429E: 	CALL	ADD_RDDEBRIS
    // asm 0000429F: 	LDI	0731h,R0
    // asm 000042A0: 	STI	R0,*+AR4(OID)
    // asm 000042A1: 	POP	R0
    // asm 000042A2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROAD_DEBRIS_CREATE_55GAL", 0, 0);
    UNIMPL_TODO();
}

static void ROAD_DEBRIS_CREATE(OBJ* obj /*AR4*/) {
    // asm 000042A3: 	BU	ADD_RDDEBRIS
    ADD_RDDEBRIS(obj);
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void SMOKE_STACK(OBJ* obj /*AR4*/) {
    PROC_CONTEXT* ctx;
    PROC* proc;

    MAME_ASSERT_FUNCTION_ENTRY();

    // asm 000042A4: 	PUSH	R0
    // asm 000042A5: 	PUSH	AR0
    // asm 000042A6: 	PUSH	AR2
    // asm 000042A7: 	PUSH	AR5
    // asm 000042A8: 	PUSH	AR6
    // asm 000042A9: 	BUD	MAKEPPP
    // asm 000042AA: 	PUSH	R2
    // asm 000042AB: 	NOP
    // asm 000042AC: 	LDI	@SMOKE_ANII,AR6
    // 	;---->	BUD	MAKEPPP
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->BACKGRND_PLAINANI_PROC.obj = obj;
    ctx->BACKGRND_PLAINANI_PROC.script = SMOKE_ANII;
    ctx->BACKGRND_PLAINANI_PROC.script_index = 0;
    proc = CREATE(PLAINANI_PROC, SPAWNER_C | ANIMATION_T, ctx);
    if (proc != NULL) {
        obj->plink = proc;
        obj->flags |= 1u << O_PROC_B;
    }
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOKE_STACK", 0, 0);
}

static void CAR_FIRE(OBJ* obj /*AR4*/) {
    (void)obj;
    // asm 000042AD: 	PUSH	R0
    // asm 000042AE: 	PUSH	AR0
    // asm 000042AF: 	PUSH	AR2
    // asm 000042B0: 	PUSH	AR5
    // asm 000042B1: 	PUSH	AR6
    // asm 000042B2: 	BUD	MAKEPPP
    // asm 000042B3: 	PUSH	R2
    // asm 000042B4: 	NOP
    // asm 000042B5: 	LDI	@CAR_FIRE_ANII,AR6
    // 	;---->	BUD	MAKEPPP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAR_FIRE", 0, 0);
    UNIMPL();
}

/* asm: SMOKE_ANI	.word	smoa,smob,smoc,smod,smoe,smof */
/* asm: 	.word	-1 */
/* asm: 	 */
static int SMOKE_ANI[] = {
    smoa_ROM,
    smob_ROM,
    smoc_ROM,
    smod_ROM,
    smoe_ROM,
    smof_ROM,
    -1,
};
// ;eug1,eug2,eug3,eug4,eug5,eug6
/* asm: CAR_FIRE_ANII	.word	CAR_FIRE_ANI */
#define CAR_FIRE_ANII CAR_FIRE_ANI
/* asm: CAR_FIRE_ANI */
/* asm: 	.word	rdflm1,rdflm2,rdflm3,rdflm4,rdflm5,rdflm6 */
/* asm: 	.word	rdflm7,rdflm8,rdflm9,rdflm10,rdflm11,rdflm12 */
/* asm: 	.word	-1 */
/* asm: 	 */
static int CAR_FIRE_ANI[] = {
    rdflm1_ROM,
    rdflm2_ROM,
    rdflm3_ROM,
    rdflm4_ROM,
    rdflm5_ROM,
    rdflm6_ROM,
    rdflm7_ROM,
    rdflm8_ROM,
    rdflm9_ROM,
    rdflm10_ROM,
    rdflm11_ROM,
    rdflm12_ROM,
    -1,
};
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: DC_MINIFOUNTAIN_ANII	.word	DC_MINIFOUNTAIN_ANI */
#define DC_MINIFOUNTAIN_ANII DC_MINIFOUNTAIN_ANI
/* asm: DC_MINIFOUNTAIN_ANI */
/* asm: 	.word	aft1,aft2,aft3,aft4,aft5,aft6,-1 */
static int DC_MINIFOUNTAIN_ANI[] = {
    aft1_ROM,
    aft2_ROM,
    aft3_ROM,
    aft4_ROM,
    aft5_ROM,
    aft6_ROM,
    -1,
};

static void DC_MINIFOUNTAIN(OBJ* obj /*AR4*/) {
    (void)obj;
    // asm 000042D4: 	PUSH	R0
    // asm 000042D5: 	PUSH	AR0
    // asm 000042D6: 	PUSH	AR2
    // asm 000042D7: 	PUSH	AR5
    // asm 000042D8: 	PUSH	AR6
    // asm 000042D9: 	PUSH	R2
    // asm 000042DA: 	LDI	@DC_MINIFOUNTAIN_ANII,AR6
    // asm 000042DB: 	CREATE	WATERANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 000042DE: 	BC	FWL1A
    // asm 000042DF: 	BU	J262
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DC_MINIFOUNTAIN", 0, 0);
    UNIMPL();
}

/* asm: DC_FOUNTAIN_ANII	.word	DC_FOUNTAIN_ANI */
#define DC_FOUNTAIN_ANII DC_FOUNTAIN_ANI
/* asm: DC_FOUNTAIN_ANI */
/* asm: 	.word	ft2,ft3,ft4,ft5,ft6,-1 */
static int DC_FOUNTAIN_ANI[] = {
    ft2_ROM,
    ft3_ROM,
    ft4_ROM,
    ft5_ROM,
    ft6_ROM,
    -1,
};

static void DC_FOUNTAIN(OBJ* obj /*AR4*/) {
    (void)obj;
    // asm 000042E7: 	PUSH	R0
    // asm 000042E8: 	PUSH	AR0
    // asm 000042E9: 	PUSH	AR2
    // asm 000042EA: 	PUSH	AR5
    // asm 000042EB: 	PUSH	AR6
    // asm 000042EC: 	PUSH	R2
    // asm 000042ED: 	LDI	@DC_FOUNTAIN_ANII,AR6
    // asm 000042EE: 	CREATE	WATERANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 000042F1: 	BC	FWL1A
    // asm 000042F2: 	BU	J262
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DC_FOUNTAIN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
/* asm: WATERFALL_ANI	.word	w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,-1 */
/* asm: 	 */
static int WATERFALL_ANI[] = {
    w1_ROM,
    w2_ROM,
    w3_ROM,
    w4_ROM,
    w5_ROM,
    w6_ROM,
    w7_ROM,
    w8_ROM,
    w9_ROM,
    w10_ROM,
    -1,
};

static void WATERFALL(OBJ* obj /*AR4*/) {
    (void)obj;
    // asm 000042FF: 	PUSH	R0
    // asm 00004300: 	PUSH	AR0
    // asm 00004301: 	PUSH	AR2
    // asm 00004302: 	PUSH	AR5
    // asm 00004303: 	PUSH	AR6
    // asm 00004304: 	PUSH	R2
    // asm 00004305: 	CREATE	WATERFALL_SND,SPAWNER_C|ANIMATION_T
    // asm 00004308: 	LDI	@WATERFALL_ANII,AR6
    // asm 00004309: 	CREATE	WATERANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 0000430C: 	BC	FWL1A
J262:
    // asm 0000430D: STI	AR0,*+AR4(OPLINK)
    // asm 0000430E: 	LDI	1,R0
    // asm 0000430F: 	LS	O_PROC_B,R0
    // asm 00004310: 	OR	*+AR4(OFLAGS),R0
    // asm 00004311: 	STI	R0,*+AR4(OFLAGS)
FWL1A:
    // asm 00004312: POP	R2
    // asm 00004313: 	POP	AR6
    // asm 00004314: 	POP	AR5
    // asm 00004315: 	POP	AR2
    // asm 00004316: 	POP	AR0
    // asm 00004317: 	POP	R0
    // asm 00004318: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERFALL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *AR6	START OF SCRIPT
 *
 *the big difference is that this *MUST* handle the sound animation also...
 *
 *
 */
static void WATERANI_PROC(PROC* p) {
    // asm 00004319: 	LDI	AR6,AR5
WATERANI_LP:
    // asm 0000431A: 	LDI	*AR5++,R0
    // asm 0000431B: 	BLT	WATERANI_PROC
    // asm 0000431C: 	STI	R0,*+AR4(OROMDATA)
    // asm 0000431D: 	RANDN	4
    // asm 0000431F: 	LDI	R0,R0
    // asm 00004320: 	LDIZ	1,AR2
    // asm 00004321: 	LDINZ	2,AR2
    // asm 00004322: 	CALL	PRC_SLEEP
    // asm 00004323: 	BU	WATERANI_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERANI_PROC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *

/*
 *----------------------------------------------------------------------------
 *
 *AR4=WATERFALL OBJECT
 *
 */
static void WATERFALL_SND(PROC* p) {
    // asm 00004324: 	SLEEP	1
    // asm 00004326: 	LDI	*+AR4(ODIST),R0
    // asm 00004327: 	LDFN	4.0,R1
    // asm 00004328: 	LDFNN	1.0,R1
    // asm 00004329: 	CMPI	-20000,R0
    // asm 0000432A: 	BGT	WF1
    // asm 0000432B: 	LDI	@CAMVIEW,R2
    // asm 0000432C: 	BNE	WF0
    // asm 0000432D: 	LDI	@PLYCBLK,AR0
    // asm 0000432E: 	LDI   	*+AR0(CAR_SPIN),R2
    // asm 0000432F: 	BNE	WF1
WF0:
    // asm 00004330: 	LDI	WATERFALLSND,AR2 	;KILL OFF SOUND
    // asm 00004331: 	CALL	KILLSNDFX
    // asm 00004332: 	DIE
WF1:
    // asm 00004333: 	FLOAT	R0,R2
    // asm 00004334: 	MPYF	R1,R2
    // asm 00004335: 	ABSF	R2
    // asm 00004336: 	MPYF	0.01,R2
    // asm 00004337: 	MPYF	0.1,R2 			;0-50
    // asm 00004338: 	SUBRF	75,R2
    // asm 00004339: 	BN	WATERFALL_SND
    // asm 0000433A: 	MPYF	7,R2
    // asm 0000433B: 	FIX	R2
    // asm 0000433C: 	CMPI	255,R2
    // asm 0000433D: 	LDIGT	255,R2
    // asm 0000433E: 	LDI	WATERFALLSND,AR2
    // asm 0000433F: 	CALL	AMBIENCE_SOUND
    // asm 00004340: 	BU	WATERFALL_SND
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERFALL_SND", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR2	SOUND
 *	R2	VOLUME (0-255)
 *
 */
void AMBIENCE_SOUND(void) {
    // asm 00004341: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm 00004342: 	BEQ	IS_T1
    // asm 00004343: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm 00004344: 	BEQ	IS_T2
    // asm 00004345: 	CALL	ONESNDFX
    // asm 00004346: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm 00004347: 	BNE	NOT_T1
IS_T1:
    // asm 00004348: 	LDI	R2,R1				;volume
    // asm 00004349: 	LDI	1,R0
    // asm 0000434A: 	CALL	SET_TRACK_VOL
    // asm 0000434B: 	BU	HEND
NOT_T1:
    // asm 0000434C: CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm 0000434D: 	BNE	NOT_T2
IS_T2:
    // asm 0000434E: 	LDI	R2,R1
    // asm 0000434F: 	LDI	2,R0
    // asm 00004350: 	CALL	SET_TRACK_VOL
NOT_T2:
HEND:
    // asm 00004351: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AMBIENCE_SOUND", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: FLAGANI		.word	bflag1,bflag2,bflag3,bflag4,bflag5 */
/* asm: 	.word	bflag6,bflag7,bflag9,bflag10,-1 */
/* asm: 	 */
static int FLAGANI[] = {
    bflag1_ROM,
    bflag2_ROM,
    bflag3_ROM,
    bflag4_ROM,
    bflag5_ROM,
    bflag6_ROM,
    bflag7_ROM,
    bflag9_ROM,
    bflag10_ROM,
    -1,
};
/* asm: FLAGANITALL	.word	aflag1,aflag2,aflag3,aflag4,aflag5 */
/* asm: 	.word	aflag6,aflag7,aflag9,aflag10,-1 */
/* asm: 	 */
static int FLAGANITALL[] = {
    aflag1_ROM,
    aflag2_ROM,
    aflag3_ROM,
    aflag4_ROM,
    aflag5_ROM,
    aflag6_ROM,
    aflag7_ROM,
    aflag9_ROM,
    aflag10_ROM,
    -1,
};
/* asm: RUT_ANIS	.word	rut,rut2,rut3,-1 */
static int RUT_ANIS[] = {
    rut_ROM,
    rut2_ROM,
    rut3_ROM,
    -1,
};
/* asm: HUNGH_ANIS	.word	hungh1,hungh2,hungh3,hungh4,hungh5,hungh6,hungh7,-1 */
static int HUNGH_ANIS[] = {
    hungh1_ROM,
    hungh2_ROM,
    hungh3_ROM,
    hungh4_ROM,
    hungh5_ROM,
    hungh6_ROM,
    hungh7_ROM,
    -1,
    // ----------------------------------------------------------------------------
};
/* asm: BABE_PALIST */
/* asm: 	.word	ungh1_blue,logo_p,ungh1_green,nintendo_p,ungh1_silver,map1_p */
/* asm: 	.word	ungh1_yellow,lift_p,ungh1_skin,bvwall_p */
static int BABE_PALIST[] = {
    ungh1_blue_ROM,
    logo_p,
    ungh1_green_ROM,
    nintendo_p,
    ungh1_silver_ROM,
    map1_p,
    ungh1_yellow_ROM,
    lift_p,
    ungh1_skin_ROM,
    bvwall_p,
};

void HUNGH_ANI(OBJ* obj /*AR4*/) {
    (void)obj;
    // asm 00004377: 	PUSH	R0
    // asm 00004378: 	PUSH	AR0
    // asm 00004379: 	PUSH	AR2
    // asm 0000437A: 	PUSH	AR5
    // asm 0000437B: 	PUSH	AR6
    // asm 0000437C: 	PUSH	R2
    // asm 0000437D: 	PUSH	AR2
    // asm 0000437E: 	READAUD	ADJ_GIRLS
    // asm 00004380: 	POP	AR2
    // asm 00004381: 	CMPI	0,R0
    // asm 00004382: 	BEQ	DORUT_ANI
    // asm 00004383: 	LDI	@HUNGH_ANISI,AR6
    // asm 00004384: 	LDI	*+AR4(OFLAGS),R0
    // ;	ANDN	O_1PAL,R0
    // asm 00004385: 	OR	O_POSTER,R0
    // asm 00004386: 	STI	R0,*+AR4(OFLAGS)
    // asm 00004387: 	LDI	5,AR2
    // asm 00004388: 	CALL	RANDU0
    // asm 00004389: 	CMPI	4,R0
    // asm 0000438A: 	BEQ	HUNGH_NOPAL
    // asm 0000438B: 	MPYI	2,R0
    // asm 0000438C: 	ADDI	@BABE_PALISTI,R0
    // asm 0000438D: 	LDI	R0,AR5
    // asm 0000438E: 	LDI	*+AR5,AR2
    // asm 0000438F: 	CALL	PAL_FIND
    // asm 00004390: 	BC	HUNGH_NOPAL
    // asm 00004391: 	LDI	R0,R2
    // asm 00004392: 	LDI	*AR5,AR2
    // asm 00004393: 	LDI	*AR2++,R3
    // asm 00004394: 	CALL	PAL_SET
    // asm 00004395: 	STI	R0,*+AR4(OPAL)
HUNGH_NOPAL:
    // asm 00004396: 	CALL	PLACE_ON_ROAD
    // asm 00004397: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_BABE_S,R0
    // asm 00004398: 	STI	R0,*+AR4(OID)
    // asm 00004399: 	LDI	AR6,AR5
    // asm 0000439A: 	CREATE	PLAINANI_LP_SLOW,SPAWNER_C|ANIMATION_T|7
    // asm 0000439D: 	BC	FWL1
    // asm 0000439E: 	BU	J2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HUNGH_ANI", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static void HUNGH_ANI_REENTER(void) {
    // asm 0000439F: 	PUSH	R0
    // asm 000043A0: 	PUSH	AR0
    // asm 000043A1: 	PUSH	AR2
    // asm 000043A2: 	PUSH	AR5
    // asm 000043A3: 	PUSH	AR6
    // asm 000043A4: 	PUSH	R2
    // asm 000043A5: 	LDI	@HUNGH_ANISI,AR6
    // asm 000043A6: 	LDI	*+AR4(OFLAGS),R0
    // ;	ANDN	O_1PAL,R0
    // asm 000043A7: 	OR	O_POSTER,R0
    // asm 000043A8: 	STI	R0,*+AR4(OFLAGS)
    // asm 000043A9: 	LDI	AR6,AR5
    // asm 000043AA: 	CREATE	PLAINANI_LP_SLOW,SPAWNER_C|ANIMATION_T|7
    // asm 000043AD: 	BC	FWL1
    // asm 000043AE: 	BU	J2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HUNGH_ANI_REENTER", 0, 0);
    UNIMPL();
}

static void PLACE_ON_ROAD(void) {
    // asm 000043AF: 	CALL	ADD_RDDEBRIS
    // asm 000043B0: 	PUSH	AR4
    // asm 000043B1: 	ADDI	OPOSX,AR4
    // asm 000043B2: 	CALL	CAMSCAN
    // asm 000043B3: 	POP	AR4
    // asm 000043B4: 	FIX	R0,R1		;Check for dude in the sign
    // asm 000043B5: 	CMPI	1000,R1
    // asm 000043B6: 	BGT	PORX
    // asm 000043B7: 	LDF	*+AR4(OPOSY),R1
    // asm 000043B8: 	ADDF	R1,R0
    // asm 000043B9: 	SUBF   	45,R0
    // asm 000043BA: 	STF	R0,*+AR4(OPOSY)
PORX:
    // asm 000043BB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLACE_ON_ROAD", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void RUT_ANI(OBJ* obj /*AR4*/) {
    (void)obj;
    // asm 000043BC: 	PUSH	R0
    // asm 000043BD: 	PUSH	AR0
    // asm 000043BE: 	PUSH	AR2
    // asm 000043BF: 	PUSH	AR5
    // asm 000043C0: 	PUSH	AR6
    // asm 000043C1: 	PUSH	R2
DORUT_ANI:
    // asm 000043C2: 	LDI	@RUT_ANISI,AR6
    // asm 000043C3: 	LDI	*+AR4(OFLAGS),R0
    // asm 000043C4: 	ANDN	O_1PAL,R0
    // asm 000043C5: 	OR	O_POSTER,R0
    // asm 000043C6: 	STI	R0,*+AR4(OFLAGS)
    // asm 000043C7: 	CALL	PLACE_ON_ROAD
    // asm 000043C8: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_DUDE_S,R0
    // asm 000043C9: 	STI	R0,*+AR4(OID)
    // asm 000043CA: 	LDI	AR6,AR5
    // asm 000043CB: 	CREATE	PLAINANI_LP_SLOW,SPAWNER_C|ANIMATION_T|7
    // asm 000043CE: 	BC	FWL1
    // asm 000043CF: 	BU	J2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUT_ANI", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
static void PLAINANI_PROC_SLOW(PROC* p) {
    // asm 000043D0: 	LDI	AR6,AR5
PLAINANI_LP_SLOW:
    // asm 000043D1: 	LDI	*AR5++,R0
    // asm 000043D2: 	BLT	PLAINANI_PROC_SLOW
    // asm 000043D3: 	STI	R0,*+AR4(OROMDATA)
    // asm 000043D4: 	RANDN	4
    // asm 000043D6: 	LDI	R0,R0
    // asm 000043D7: 	LDIZ	1,AR2
    // asm 000043D8: 	LDINZ	2,AR2
    // asm 000043D9: 	ADDI	1,AR2
    // asm 000043DA: 	CALL	PRC_SLEEP
    // asm 000043DB: 	BU	PLAINANI_LP_SLOW
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLAINANI_PROC_SLOW", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FLAGWAVE_TALL(OBJ* obj /*AR4*/) {
    (void)obj;
    // asm 000043DC: 	PUSH	R0
    // asm 000043DD: 	PUSH	AR0
    // asm 000043DE: 	PUSH	AR2
    // asm 000043DF: 	PUSH	AR5
    // asm 000043E0: 	PUSH	AR6
    // asm 000043E1: 	PUSH	R2
    // asm 000043E2: 	LDI	@FLAGANITALLI,AR6
    // asm 000043E3: 	LDI	*+AR4(OFLAGS),R0
    // asm 000043E4: 	ANDN	O_1PAL,R0
    // asm 000043E5: 	STI	R0,*+AR4(OFLAGS)
    // asm 000043E6: 	RANDN	7
    // asm 000043E8: 	LDI	AR6,AR5
    // asm 000043E9: 	ADDI	R0,AR5
    // asm 000043EA: 	CREATE	PLAINANI_LP,SPAWNER_C|ANIMATION_T
    // asm 000043ED: 	BC	FWL1
    // asm 000043EE: 	BU	J2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLAGWAVE_TALL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FLAGWAVE(OBJ* obj /*AR4*/) {
    PROC_CONTEXT* ctx;
    PROC* proc;
    int script_index;

    // asm 000043EF: 	PUSH	R0
    // asm 000043F0: 	PUSH	AR0
    // asm 000043F1: 	PUSH	AR2
    // asm 000043F2: 	PUSH	AR5
    // asm 000043F3: 	PUSH	AR6
    // asm 000043F4: 	PUSH	R2
    // asm 000043F5: 	LDI	@FLAGANII,AR6
    // asm 000043F6: 	LDI	*+AR4(OFLAGS),R0
    // asm 000043F7: 	ANDN	O_1PAL,R0
    // asm 000043F8: 	STI	R0,*+AR4(OFLAGS)
    obj->flags &= ~O_1PAL;
    // asm 000043F9: 	RANDN	7
    script_index = RANDU0(7);
    // asm 000043FB: 	LDI	AR6,AR5
    // asm 000043FC: 	ADDI	R0,AR5
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->BACKGRND_PLAINANI_PROC.obj = obj;
    ctx->BACKGRND_PLAINANI_PROC.script = FLAGANII;
    ctx->BACKGRND_PLAINANI_PROC.script_index = script_index;
    // asm 000043FD: 	CREATE	PLAINANI_LP,SPAWNER_C|ANIMATION_T
    proc = CREATE(PLAINANI_PROC, SPAWNER_C | ANIMATION_T, ctx);
    // asm 00004400: 	BC	FWL1
    if (proc == NULL) {
        goto FWL1;
    }
    // asm 00004401: 	BU	J2
    goto J2;
MAKEPPP:
    // asm 00004402: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->BACKGRND_PLAINANI_PROC.obj = obj;
    ctx->BACKGRND_PLAINANI_PROC.script = FLAGANII;
    ctx->BACKGRND_PLAINANI_PROC.script_index = 0;
    proc = CREATE(PLAINANI_PROC, SPAWNER_C | ANIMATION_T, ctx);
    // asm 00004405: 	BC	FWL1
    if (proc == NULL) {
        goto FWL1;
    }
J2:
    // asm 00004406: STI	AR0,*+AR4(OPLINK)
    obj->plink = proc;
    // asm 00004407: 	LDI	1,R0
    // asm 00004408: 	LS	O_PROC_B,R0
    // asm 00004409: 	OR	*+AR4(OFLAGS),R0
    // asm 0000440A: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= 1u << O_PROC_B;
FWL1:
    // asm 0000440B: POP	R2
    // asm 0000440C: 	POP	AR6
    // asm 0000440D: 	POP	AR5
    // asm 0000440E: 	POP	AR2
    // asm 0000440F: 	POP	AR0
    // asm 00004410: 	POP	R0
    // asm 00004411: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *AR6	START OF SCRIPT
 *
 */
static void PLAINANI_PROC(PROC* p) {
    PROC_CONTEXT* ctx = p->ctx;
    int frame;
    int sleep_ticks;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 00004412: 	LDI	AR6,AR5
PLAINANI_LP:
    // asm 00004413: 	LDI	*AR5++,R0
    frame = ctx->BACKGRND_PLAINANI_PROC.script[ctx->BACKGRND_PLAINANI_PROC.script_index++];
    // asm 00004414: 	BLT	PLAINANI_PROC
    if (frame < 0) {
        ctx->BACKGRND_PLAINANI_PROC.script_index = 0;
        goto PLAINANI_LP;
    }
    // asm 00004415: 	STI	R0,*+AR4(OROMDATA)
    ctx->BACKGRND_PLAINANI_PROC.obj->romdata = ROM_PTR((word_addr_t)frame);
    // asm 00004416: 	RANDN	4
    sleep_ticks = RANDU0(4);
    // asm 00004418: 	LDI	R0,R0
    // asm 00004419: 	LDIZ	1,AR2
    // asm 0000441A: 	LDINZ	2,AR2
    sleep_ticks = sleep_ticks == 0 ? 1 : 2;
    // asm 0000441B: 	CALL	PRC_SLEEP
    SLEEP(sleep_ticks, 1);
    // asm 0000441C: 	BU	PLAINANI_LP
    goto PLAINANI_LP;
}

// *----------------------------------------------------------------------------

/* asm: SINGLE_SECTION_TEMPPTR	.bss	SINGLE_SECTION_TEMPPTR,1 */
OBJ* SINGLE_SECTION_TEMPPTR;

/*
 *----------------------------------------------------------------------------
 *ABSOLUTE LOAD INTO OBJECT LIST FOR A SINGLE SECTION
 *
 *
 *PARAMETERS
 *	AR2	POINTER TO ROM GROUP (OBJECTS.EQU)
 *
 *RETURNS
 *	R0	PTR TO THE FIRST OBJECT IN GROUP
 *
 */

OBJ* LOAD_SINGLE_SECTION(LOAD_SINGLE_SECTION_GROUP* rom_group /*AR2*/) {
    const u32* rom_cursor;
    OBJ* obj;
    OBJ** next_link;
    int count;
    u32 model_rom_addr, id;

    // asm 0000441D: 	PUSH	R4
    // asm 0000441E: 	PUSH	R5
    // asm 0000441F: 	PUSH	AR0
    // asm 00004420: 	PUSH	AR2
    // asm 00004421: 	PUSH	AR4
    // asm 00004422: 	PUSH	AR5
    // asm 00004423: 	PUSH	AR6
    // asm 00004424: 	PUSH	AR7
    // asm 00004425: 	LDL	SINGLE_SECTION_TEMPPTR,AR6
    // asm 00004426: 	SUBI	OLINK2,AR6
    next_link = &SINGLE_SECTION_TEMPPTR;

    // asm 00004427: 	LDI	AR2,AR5
    // asm 00004428: 	INC	AR5
    // asm 00004429: 	LDI	*AR5++,R4		;get number of objects to load
    rom_cursor = (const u32*)rom_group + 1;
    count = (int)crusn_read_u32(&rom_cursor); // ;get number of objects to load
    MAME_ASSERT_REG(0x0000442A, "R4", &count);

    // asm: 	SLOCKON	LE, BACKGRND\\LOAD_SINGLE_SECTION GROUP ERROR
    SLOCKON(count <= 0, "BACKGRND\\LOAD_SINGLE_SECTION GROUP ERROR");

    // asm 0000442A: 	DEC	R4
    // asm 0000442B: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\\LOAD_SINGLE_SECTION OUT OF OBJECTS"
    count--;
    SLOCKON(count > OFREECNT, "BACKGRND\\LOAD_SINGLE_SECTION OUT OF OBJECTS");

LS_L12:
    // asm 0000442C: 	LDI	*AR5++,AR2		;GET MODEL PTR
    // asm 0000442D: 	CALL	OBJ_GETE
    model_rom_addr = crusn_read_u32(&rom_cursor);
    obj = OBJ_GETE(ROM_PTR(model_rom_addr)); // ;GET MODEL PTR

    // asm: 	SLOCKON	C,"BACKGRND\\LOAD_SINGLE_SECTION OUT OF OBJECTS *FATAL*"
    SLOCKON(obj == NULL, "BACKGRND\\LOAD_SINGLE_SECTION OUT OF OBJECTS *FATAL*");
    // asm 0000442E: 	BC	LS_ACTIVATE_X
    if (obj == NULL) {
        goto LS_ACTIVATE_X;
    }

    // asm 0000442F: 	LDI	AR0,AR4
    // asm 00004430: 	STI	AR4,*+AR6(OLINK2)
    *next_link = obj;

    // asm 00004431: 	LDI	AR4,AR6
    next_link = (OBJ**)&obj->link2;

    // asm 00004432: 	FLOAT	*AR5++,R1		;GET X POSITION
    // asm 00004433: 	STF	R1,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_FROM_INT(crusn_read_s32(&rom_cursor)));
    MAME_ASSERT_REG_FLOAT(0x00004433, "R1", &obj->pos.X);

    // asm 00004434: 	FLOAT	*AR5++,R1		;GET Y POSITION
    // asm 00004435: 	STF	R1,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_FROM_INT(crusn_read_s32(&rom_cursor)));
    MAME_ASSERT_REG_FLOAT(0x00004435, "R1", &obj->pos.Y);

    // asm 00004436: 	FLOAT	*AR5++,R1		;GET Z POSITION
    // asm 00004437: 	STF	R1,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_FROM_INT(crusn_read_s32(&rom_cursor)));
    MAME_ASSERT_REG_FLOAT(0x00004437, "R1", &obj->pos.Z);

    // asm 00004438: 	LDF	*AR5++,R2		;SET THE RADIANS FOR THE OBJECT
    // asm 00004439: 	STF	R2,*+AR4(ORADY)
    obj->rad.Y = C3X_STF(crusn__read_f32(&rom_cursor));

    // asm 0000443A: 	LDI	AR4,AR2
    // asm 0000443B: 	ADDI	OMATRIX,AR2
    // asm 0000443C: 	CALL	HPFIND_YMATRIX
    HPFIND_YMATRIX(&obj->omatrix, C3X_LDF(obj->rad.Y));

    // asm 0000443D: 	LDI	*AR5++,R1		;LOAD OBJECT ID (GENV STYLE)
    // asm 0000443E: 	LDI	R1,R2
    // asm 0000443F: 	AND	CLASS_M|TYPE_M|SUBTYPE_M,R1
    // asm 00004440: 	STI	R1,*+AR4(OID)
    id = crusn_read_u32(&rom_cursor);
    obj->id = id & (CLASS_M | TYPE_M | SUBTYPE_M);
    MAME_ASSERT_REG(0x00004440, "R1", &obj->id);

    // asm 00004441: 	LDI	AR4,AR2
    // asm 00004442: 	CALL	OBJ_INSERT			;INSERT THE BABE
    OBJ_INSERT(obj);

    // asm 00004443: 	SUBI	1,R4
    // asm 00004444: 	BGE	LS_L12
    count--;
    if (count >= 0) {
        goto LS_L12;
    }

LS_ACTIVATE_X:
    // asm 00004445: 	CLRI	R0
    // asm 00004446: 	STI	R0,*+AR6(OLINK2)
    *next_link = NULL;

    // asm 00004447: 	LDI	@SINGLE_SECTION_TEMPPTR,R0
    // asm 00004448: 	POP	AR7
    // asm 00004449: 	POP	AR6
    // asm 0000444A: 	POP	AR5
    // asm 0000444B: 	POP	AR4
    // asm 0000444C: 	POP	AR2
    // asm 0000444D: 	POP	AR0
    // asm 0000444E: 	POP	R5
    // asm 0000444F: 	POP	R4
    // asm 00004450: 	RETS
    return SINGLE_SECTION_TEMPPTR;
}

// *----------------------------------------------------------------------------

/*
;*----------------------------------------------------------------------------
;OIL_PUMP:
;	PUSH	AR0
;	PUSH	AR2
;	PUSH	R2
;	CREATE	OILWELL,UTIL_C
;	POP	R2
;	POP	AR2
;	POP	AR0
;	RETS
;
;OILWELL_DYNA
;	.word	2		;#OF DYNAS-1
;
;;	.float	-1197,0,0	;WHEEL
;	.float	0,-538,0	;WHEEL
;	.word	41		;VERTS-1
;	.word	2		;DYNAFLAG
;
;	.float	-1276,-2325,0	;HAMMER
;	.word	38		;VERTS-1
;	.word	3		;DYNAFLAG
;
;	.float	0,0,0		;main body
;	.word	56		;VERTS-1
;	.word	1		;DYNAFLAG
;
;*
;*PARAMETERS
;*	AR4	OILWELL OBJECT
;*
;*
;OILWELL:
;	STI	AR7,*+AR4(OPLINK)
;
;	LDI	1,R0
;	LS	O_PROC_B,R0
;	OR	*+AR4(OFLAGS),R0
;	OR	O_DYNAMIC,R0	 	;MAKE PARENT OBJECT DYNAMIC
;	STI	R0,*+AR4(OFLAGS)
;
;	LDL	OILWELL_DYNA,AR2
;
;	LDI	AR4,AR3
;	ADDI	ODYNALIST,AR3
;
;*INIT DYNA OBJECTS
;	LDI	*AR2++,RC		;GET DYNAMIC OBJECT COUNT (-1)
;	RPTB	OWINLP
;
; 	CALL	GETDYNA	     		;LINK HIM INTO LIST
;	STI	AR0,*AR3
;
;
;	LDF	*AR2++,R0
;	STF	R0,*+AR0(DYNACENTERX)
;	STF	R0,*+AR0(DYNATRANSX)
;	LDF	*AR2++,R0
;	STF	R0,*+AR0(DYNACENTERY)
;	STF	R0,*+AR0(DYNATRANSY)
;	LDF	*AR2++,R0
;	STF	R0,*+AR0(DYNACENTERZ)
;	STF	R0,*+AR0(DYNATRANSZ)
;	LDI	*AR2++,R0
;	STI	R0,*+AR0(DYNANVERTS)
;	LDI	*AR2++,R0
;	STI	R0,*+AR0(DYNAFLAG)
;
;	CMPI	2,R0	;
;	BNE	H75
;	STI	AR0,*+AR7(PDATA)		;WHEEL
;H75
;
;	CMPI	3,R0	;
;	BNE	H76
;	STI	AR0,*+AR7(PDATA+1)	;HAMMER
;H76
;
;	STI	AR4,*+AR0(DYNAPARENT)
;OWINLP	LDI	AR0,AR3
;
;	LDI	0,R0
;	STI	R0,*AR3			;LAST LINK IS ZERO, DUDES
;
;	LDI	AR3,AR0
;	ADDI	DYNAMATRIX,AR0
;	CALL	INITMAT
;
;	CLRF	R6
;	CLRF	R7
;
;	LDF	0.1,R0
;	STF	R0,*+AR7(PDATA+2)
;
;*
;*
;*	R6	THETA FOR THE WHEELS
;*	R7	THETA FOR THE HAMMER
;*
;OILWELLP
;
;	ADDF	0.1,R6
;	LDF	R6,R2
;	CALL	NORMITS
;	LDF	R2,R6
;	LDI	*+AR7(PDATA),AR2
;	ADDI	DYNAMATRIX,AR2
;	CALL	FIND_ZMATRIX
;
;
;	LDF	*+AR7(PDATA+2),R0
;	NEGF	R0,R1
;
;	CMPF	PI,R7
;	BLT	NC1
;	LDF	R1,R0
;	BU	IRV
;NC1
;	CMPF	-PI,R7
;	BGT	IRV
;	LDF	R1,R0
;IRV
;	ADDF	R0,R7
;	STF	R0,*+AR7(PDATA+2)
;
;	LDF	R0,R2
;	LDI	*+AR7(PDATA+1),AR2
;	ADDI	DYNAMATRIX,AR2
;	CALL	FIND_ZMATRIX
;
;	SLEEP	1
;	BU	OILWELLP
;*----------------------------------------------------------------------------
*/

// *----------------------------------------------------------------------------
static void OHARE_PLANE(OBJ* obj /*AR4*/) {
    PROC_CONTEXT* ctx;
    PROC* proc;

    MAME_ASSERT_FUNCTION_ENTRY();

    // asm 00004451: 	PUSH	AR0
    // asm 00004452: 	PUSH	AR2
    // asm 00004453: 	PUSH	R2
    // asm 00004454: 	SONDFX	JETFLYBY
    ONESNDFX(JETFLYBY);
    // asm 00004456: 	CREATE	PLANE_FWRD,UTIL_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->BACKGRND_PLANE_FWRD.obj = obj;
    proc = CREATE(PLANE_FWRD, UTIL_C, ctx);
    // asm 00004459: 	STI	AR0,*+AR4(OPLINK)
    obj->plink = proc;
    // asm 0000445A: 	LDI	1,R0
    // asm 0000445B: 	LS	O_PROC_B,R0
    // asm 0000445C: 	OR	*+AR4(OFLAGS),R0
    // asm 0000445D: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= 1u << O_PROC_B;
    // asm 0000445E: 	POP	R2
    // asm 0000445F: 	POP	AR2
    // asm 00004460: 	POP	AR0
    // asm 00004461: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OHARE_PLANE", 0, 0);
}

static void PLANE_FWRD(PROC* p) {
    OBJ* obj;
    c3x_reg_t angle;
    c3x_reg_t frame_distance;
    c3x_reg_t remaining_distance;
    c3x_reg_t speed;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    obj = p->ctx->BACKGRND_PLANE_FWRD.obj;
    // asm 00004462: 	FLOAT	250,R7
    speed = C3X_FROM_INT(250);
    // asm 00004463: 	FLOAT	32000,R6
    remaining_distance = C3X_FROM_INT(32000);
    // asm 00004464: 	MPYF	4,R6
    remaining_distance = C3X_MUL(remaining_distance, C3X_IMM_F32(4));
    // *
    // *
PLANE_FWL:
    // asm 00004465: 	LDF	*+AR4(ORADY),R2
    angle = C3X_LDF(obj->rad.Y);
    // asm 00004466: 	ADDF	HALFPI,R2
    angle = C3X_ADD(angle, C3X_IMM_F32(HALFPI));
    // asm 00004467: 	LDI	@MATRIXAI,AR2
    // asm 00004468: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXAI, angle);
    // asm 00004469: 	CALL	CLR_VECTORA
    CLR_VECTORA();
    // asm 0000446A: 	LDF	R7,R0
    // asm 0000446B: 	FLOAT	@NFRAMES,R1
    // asm 0000446C: 	MPYF	R1,R0
    frame_distance = C3X_MUL(speed, C3X_FROM_INT(NFRAMES));
    // asm 0000446D: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF(frame_distance);
    // asm 0000446E: 	LDI	@MATRIXAI,R2
    // asm 0000446F: 	LDI	AR2,R3
    // asm 00004470: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &MATRIXAI, &VECTORAI);
    // asm 00004471: 	LDI	@VECTORAI,AR2
    // asm 00004472: 	LDF	*+AR4(OPOSX),R0
    // asm 00004473: 	ADDF	*+AR2(X),R0
    // asm 00004474: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), C3X_LDF(VECTORAI.X)));
    // asm 00004475: 	LDF	*+AR4(OPOSY),R0
    // asm 00004476: 	ADDF	*+AR2(Y),R0
    // asm 00004477: 	ADDF	50,R0
    // asm 00004478: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_ADD(
        C3X_ADD(C3X_LDF(obj->pos.Y), C3X_LDF(VECTORAI.Y)),
        C3X_IMM_F32(50)));
    // asm 00004479: 	LDF	*+AR4(OPOSZ),R0
    // asm 0000447A: 	ADDF	*+AR2(Z),R0
    // asm 0000447B: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Z), C3X_LDF(VECTORAI.Z)));
    // asm 0000447C: 	ABSF	R7,R0
    // asm 0000447D: 	FLOAT	@NFRAMES,R1
    // asm 0000447E: 	MPYF	R1,R0
    frame_distance = C3X_MUL(C3X_ABS(speed), C3X_FROM_INT(NFRAMES));
    // asm 0000447F: 	SUBF	R0,R6
    remaining_distance = C3X_SUB(remaining_distance, frame_distance);
    // asm 00004480: 	BLT	TRAINX
    if (C3X_LT(remaining_distance, C3X_FROM_INT(0))) {
        obj->plink = NULL;
        obj->flags &= ~(1u << O_PROC_B);
        DIE();
    }
    // asm 00004481: 	SLEEP	1
    p->ctx->BACKGRND_PLANE_FWRD.speed = C3X_STF(speed);
    p->ctx->BACKGRND_PLANE_FWRD.remaining_distance = C3X_STF(remaining_distance);
    SLEEP(1, 1);
    obj = p->ctx->BACKGRND_PLANE_FWRD.obj;
    speed = C3X_LDF(p->ctx->BACKGRND_PLANE_FWRD.speed);
    remaining_distance = C3X_LDF(p->ctx->BACKGRND_PLANE_FWRD.remaining_distance);
    // asm 00004483: 	BU	PLANE_FWL
    goto PLANE_FWL;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLANE_FWRD", 0, 0);
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void TRAIN_FWRD_MAKEB(void) {
    // asm 00004484: 	PUSH	AR0
    // asm 00004485: 	PUSH	AR2
    // asm 00004486: 	PUSH	R2
    // asm 00004487: 	CREATE	TRAIN_FWRDB,UTIL_C
    // asm 0000448A: 	STI	AR0,*+AR4(OPLINK)
    // asm 0000448B: 	LDI	1,R0
    // asm 0000448C: 	LS	O_PROC_B,R0
    // asm 0000448D: 	OR	*+AR4(OFLAGS),R0
    // asm 0000448E: 	STI	R0,*+AR4(OFLAGS)
    // asm 0000448F: 	BU	J87
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAIN_FWRD_MAKEB", 0, 0);
    UNIMPL();
}

static void TRAIN_FWRD_MAKE(OBJ* obj /*AR4*/) {
    PROC_CONTEXT* ctx;
    PROC* proc;

    MAME_ASSERT_FUNCTION_ENTRY();

    // asm 00004490: 	PUSH	AR0
    // asm 00004491: 	PUSH	AR2
    // asm 00004492: 	PUSH	R2
    // asm 00004493: 	CREATE	TRAIN_FWRD,UTIL_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->BACKGRND_TRAIN_FWRD.obj = obj;
    proc = CREATE(TRAIN_FWRD, UTIL_C, ctx);
    // asm 00004496: 	STI	AR0,*+AR4(OPLINK)
    obj->plink = proc;
    // asm 00004497: 	LDI	1,R0
    // asm 00004498: 	LS	O_PROC_B,R0
    // asm 00004499: 	OR	*+AR4(OFLAGS),R0
    // asm 0000449A: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= 1u << O_PROC_B;
J87:
    // asm 0000449B: POP	R2
    // asm 0000449C: 	POP	AR2
    // asm 0000449D: 	POP	AR0
    // asm 0000449E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAIN_FWRD_MAKE", 0, 0);
}

static void TRAIN_FWRDB(void) {
    // asm 0000449F: 	LDI	*+AR4(OUSR1),R0
    // asm 000044A0: 	ADDI	5,R0			;kludge to delete later than loaded
    // asm 000044A1: 	STI	R0,*+AR4(OUSR1)
    // asm 000044A2: 	FLOAT	-150,R7
    // asm 000044A3: 	FLOAT	32000,R6
    // asm 000044A4: 	MPYF	8,R6
    // asm 000044A5: 	BU	J765
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAIN_FWRDB", 0, 0);
    UNIMPL();
}

static void TRAIN_FWRD(PROC* p) {
    OBJ* obj;
    c3x_reg_t frame_distance;
    c3x_reg_t remaining_distance;
    c3x_reg_t speed;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    obj = p->ctx->BACKGRND_TRAIN_FWRD.obj;
    // asm 000044A6: 	FLOAT	150,R7
    speed = C3X_FROM_INT(150);
    // asm 000044A7: 	FLOAT	32000,R6
    remaining_distance = C3X_FROM_INT(32000);
    // asm 000044A8: 	MPYF	3,R6
    remaining_distance = C3X_MUL(remaining_distance, C3X_IMM_F32(3));
J765:
    // *
    // *
TRAIN_FWL:
    // asm 000044A9: 	CALL	CLR_VECTORA
    CLR_VECTORA();
    // asm 000044AA: 	LDF	R7,R0
    // asm 000044AB: 	FLOAT	@NFRAMES,R1
    // asm 000044AC: 	MPYF	R1,R0
    frame_distance = C3X_MUL(speed, C3X_FROM_INT(NFRAMES));
    // asm 000044AD: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF(frame_distance);
    // asm 000044AE: 	LDI	AR4,R2
    // asm 000044AF: 	ADDI	OMATRIX,R2
    // asm 000044B0: 	LDI	AR2,R3
    // asm 000044B1: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, (MATRIX*)&obj->omatrix, &VECTORAI);
    // asm 000044B2: 	LDI	@VECTORAI,AR2
    // asm 000044B3: 	LDF	*+AR4(OPOSX),R0
    // asm 000044B4: 	ADDF	*+AR2(X),R0
    // asm 000044B5: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), C3X_LDF(VECTORAI.X)));
    // asm 000044B6: 	LDF	*+AR4(OPOSY),R0
    // asm 000044B7: 	ADDF	*+AR2(Y),R0
    // asm 000044B8: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Y), C3X_LDF(VECTORAI.Y)));
    // asm 000044B9: 	LDF	*+AR4(OPOSZ),R0
    // asm 000044BA: 	ADDF	*+AR2(Z),R0
    // asm 000044BB: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Z), C3X_LDF(VECTORAI.Z)));
    // asm 000044BC: 	ABSF	R7,R0
    // asm 000044BD: 	FLOAT	@NFRAMES,R1
    // asm 000044BE: 	MPYF	R1,R0
    frame_distance = C3X_MUL(C3X_ABS(speed), C3X_FROM_INT(NFRAMES));
    // asm 000044BF: 	SUBF	R0,R6
    remaining_distance = C3X_SUB(remaining_distance, frame_distance);
    // asm 000044C0: 	BLT	TRAINX
    if (C3X_LT(remaining_distance, C3X_FROM_INT(0))) {
        goto TRAINX;
    }
    // asm 000044C1: 	SLEEP	1
    p->ctx->BACKGRND_TRAIN_FWRD.speed = C3X_STF(speed);
    p->ctx->BACKGRND_TRAIN_FWRD.remaining_distance = C3X_STF(remaining_distance);
    SLEEP(1, 1);
    obj = p->ctx->BACKGRND_TRAIN_FWRD.obj;
    speed = C3X_LDF(p->ctx->BACKGRND_TRAIN_FWRD.speed);
    remaining_distance = C3X_LDF(p->ctx->BACKGRND_TRAIN_FWRD.remaining_distance);
    // asm 000044C3: 	BU	TRAIN_FWL
    goto TRAIN_FWL;
TRAINX:
    // asm 000044C4: 	CLRI	R0
    // asm 000044C5: 	STI	R0,*+AR4(OPLINK)
    obj->plink = NULL;
    // asm 000044C6: 	LDI	1,R0
    // asm 000044C7: 	LS	O_PROC_B,R0
    // asm 000044C8: 	LDI	*+AR4(OFLAGS),R1
    // asm 000044C9: 	ANDN	R0,R1
    // asm 000044CA: 	STI	R1,*+AR4(OFLAGS)
    obj->flags &= ~(1u << O_PROC_B);
    // asm 000044CB: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAIN_FWRD", 0, 0);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *PARAMETERS
 *	AR2	SECTION POINTER
 *
 *	OFFSET BY START_POS
 *
 *
 */
void LOAD_SINGLE_SECTION_OFFSET(void) {
    // asm 000044CC: 	PUSH	R4
    // asm 000044CD: 	PUSH	R5
    // asm 000044CE: 	PUSH	AR0
    // asm 000044CF: 	PUSH	AR2
    // asm 000044D0: 	PUSH	AR4
    // asm 000044D1: 	PUSH	AR5
    // asm 000044D2: 	PUSH	AR6
    // asm 000044D3: 	PUSH	AR7
    // asm 000044D4: 	LDL	SINGLE_SECTION_TEMPPTR,AR6
    // asm 000044D5: 	SUBI	OLINK2,AR6
    // asm 000044D6: 	LDI	AR2,AR5
    // asm 000044D7: 	INC	AR5
    // asm 000044D8: 	LDI	*AR5++,R4		;get number of objects to load
    // asm: 	SLOCKON	LE,"BACKGRND\LOAD_SINGLE_SECTION GROUP ERROR"
    // asm 000044D9: 	DEC	R4
    // asm 000044DA: 	LDF	@START_RADY,R2
    // asm 000044DB: 	LDI	@MATRIXAI,AR2
    // asm 000044DC: 	CALL	HPFIND_YMATRIX
    // asm 000044DD: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\LOAD_SINGLE_SECTION OUT OF OBJECTS"
LS_L12O:
    // asm 000044DE: 	LDI	*AR5++,AR2		;GET MODEL PTR
    // asm 000044DF: 	CALL	OBJ_GETE
    // asm: 	SLOCKON	C,"BACKGRND\LOAD_SINGLE_SECTION OUT OF OBJECTS *FATAL*"
    // asm 000044E0: 	BC	LS_ACTIVATE_XO
    // asm 000044E1: 	LDI	AR0,AR4
    // asm 000044E2: 	STI	AR4,*+AR6(OLINK2)
    // asm 000044E3: 	LDI	AR4,AR6
    // asm 000044E4: 	LDI	@VECTORAI,AR2
    // asm 000044E5: 	FLOAT	*AR5++,R1		;GET X POSITION
    // asm 000044E6: 	STF	R1,*+AR2(X)
    // asm 000044E7: 	FLOAT	*AR5++,R1		;GET Y POSITION
    // asm 000044E8: 	STF	R1,*+AR2(Y)
    // asm 000044E9: 	FLOAT	*AR5++,R1		;GET Z POSITION
    // asm 000044EA: 	STF	R1,*+AR2(Z)
    // asm 000044EB: 	LDI	@MATRIXAI,R2
    // asm 000044EC: 	LDI	AR2,R3
    // asm 000044ED: 	CALL	MATRIX_MUL
    // asm 000044EE: 	LDF	*+AR2(X),R1
    // asm 000044EF: 	ADDF	@START_POS+X,R1
    // asm 000044F0: 	STF	R1,*+AR4(OPOSX)
    // asm 000044F1: 	LDF	*+AR2(Y),R1
    // asm 000044F2: 	ADDF	@START_POS+Y,R1
    // asm 000044F3: 	STF	R1,*+AR4(OPOSY)
    // asm 000044F4: 	LDF	*+AR2(Z),R1
    // asm 000044F5: 	ADDF	@START_POS+Z,R1
    // asm 000044F6: 	STF	R1,*+AR4(OPOSZ)
    // asm 000044F7: 	LDF	*AR5++,R2		;GET THE RADIANS FOR THE OBJECT
    // asm 000044F8: 	ADDF	@START_RADY,R2
    // asm 000044F9: 	STF	R2,*+AR4(ORADY)
    // asm 000044FA: 	LDI	AR4,AR2
    // asm 000044FB: 	ADDI	OMATRIX,AR2
    // asm 000044FC: 	CALL	HPFIND_YMATRIX
    // asm 000044FD: 	LDI	*AR5++,R1		;GET OBJECT ID (GENV STYLE)
    // asm 000044FE: 	LDI	R1,R2
    // asm 000044FF: 	AND	CLASS_M|TYPE_M|SUBTYPE_M,R1
    // asm 00004500: 	STI	R1,*+AR4(OID)
    // asm 00004501: 	LDI	AR4,AR2
    // asm 00004502: 	CALL	OBJ_INSERT			;INSERT THE BABE
    // asm 00004503: 	SUBI	1,R4
    // asm 00004504: 	BGE	LS_L12O
LS_ACTIVATE_XO:
    // asm 00004505: 	CLRI	R0
    // asm 00004506: 	STI	R0,*+AR6(OLINK2)
    // asm 00004507: 	LDI	@SINGLE_SECTION_TEMPPTR,R0
    // asm 00004508: 	POP	AR7
    // asm 00004509: 	POP	AR6
    // asm 0000450A: 	POP	AR5
    // asm 0000450B: 	POP	AR4
    // asm 0000450C: 	POP	AR2
    // asm 0000450D: 	POP	AR0
    // asm 0000450E: 	POP	R5
    // asm 0000450F: 	POP	R4
    // asm 00004510: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SINGLE_SECTION_OFFSET", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: ROUTINE_TAB: */
/* asm: 	.word	40Ah,FLAGWAVE */
/* asm: 	.word	460h,ROAD_DEBRIS_CREATE */
/* asm: 	.word	461h,ROAD_DEBRIS_CREATE_55GAL */
/* asm: 	.word	462h,ROAD_DEBRIS_CREATE_55GAL	;actually TOXIC */
/* asm: 	.word	463h,ROAD_DEBRIS_CREATE_55GAL	;actually CONE */
/* asm: 	.word	465h,FLAGWAVE			;short flag */
/* asm: 	.word	466h,FLAGWAVE_TALL		;tall flag */
/* asm: 	.word	467h,WATERFALL */
/* asm: 	.word	469h,OVERCAR		;LA & CHICAGO, FREEWAY OVERPASS CAR */
/* asm: 	.word	470h,RRSTART_ENGINE */
/* asm: 	.word	471h,RRSTART_BOXCAR */
/* asm: 	.word	472h,RRSTART_BOXCAR */
/* asm: 	.word	473h,RRSTART_BOXCAR */
/* asm: 	.word	474h,RRSTART_BOXCAR */
/* asm: 	.word	475h,RRSTART_BOXCAR */
/* asm: 	.word	476h,RRSTART_BOXCAR */
/* asm: 	.word	481h,SMOKE_STACK */
/* asm: 	.word	482h,CAR_FIRE */
/* asm: 	.word	498h,OHARE_PLANE	;CHICAGO AIRPLANE */
/* asm: 	.word	741h,RUT_ANI */
/* asm: 	.word	742h,HUNGH_ANI */
/* asm: 	 */
/* asm: 	.word	4A1h,DC_FOUNTAIN */
/* asm: 	.word	4A2h,DC_MINIFOUNTAIN */
/* asm: 	.word	0	;END OF TABLE ID */
static const BGD_OROUTINE_ENTRY ROUTINE_TAB[] = {
    { 0x40A, FLAGWAVE },
    { 0x460, ROAD_DEBRIS_CREATE },
    { 0x461, ROAD_DEBRIS_CREATE_55GAL },
    { 0x462, ROAD_DEBRIS_CREATE_55GAL }, // actually TOXIC
    { 0x463, ROAD_DEBRIS_CREATE_55GAL }, // actually CONE
    { 0x465, FLAGWAVE },                 // short flag
    { 0x466, FLAGWAVE_TALL },            // tall flag
    //
    { 0x467, WATERFALL },
    { 0x469, OVERCAR }, // LA & CHICAGO, FREEWAY OVERPASS CAR
    { 0x470, RRSTART_ENGINE },
    { 0x471, RRSTART_BOXCAR },
    { 0x472, RRSTART_BOXCAR },
    { 0x473, RRSTART_BOXCAR },
    { 0x474, RRSTART_BOXCAR },
    { 0x475, RRSTART_BOXCAR },
    { 0x476, RRSTART_BOXCAR },
    //
    { 0x481, SMOKE_STACK },
    { 0x482, CAR_FIRE },
    //
    // 	.word	485h,TRAIN_FWRD_MAKEB	;CHICAGO TRAIN FORWARD SPECIFIED DISTANCE BOXCAR (BRIDGE)
    // 	.word	486h,TRAIN_FWRD_MAKEB	;CHICAGO TRAIN FORWARD SPECIFIED DISTANCE ENGINE (BRIDGE)
    // 	.word	487h,OIL_PUMP		;RUSHMORE OIL PUMP
    // 	.word	495h,TRAIN_FWRD_MAKE	;CHICAGO TRAIN FORWARD SPECIFIED DISTANCE BOXCAR
    // 	.word	496h,TRAIN_FWRD_MAKE	;CHICAGO TRAIN FORWARD SPECIFIED DISTANCE ENGINE
    { 0x498, OHARE_PLANE }, // CHICAGO AIRPLANE
    //
    { 0x741, RUT_ANI },
    { 0x742, HUNGH_ANI },
    { 0x4A1, DC_FOUNTAIN },
    { 0x4A2, DC_MINIFOUNTAIN },
    { 0, 0 }, // END OF TABLE ID
    // ----------------------------------------------------------------------------
};

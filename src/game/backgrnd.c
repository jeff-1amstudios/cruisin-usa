#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "cornobj.h"
#include "backgrnd.h"
#include "discovered_defines.h"
#include "discovered_labels.h"

/*
 * Source module: asm/BACKGRND.ASM
 */

void FIND_STARTING_VALUES(void);
void BGD_INIT(void);
void BGD_WATCHER(void);
void GAMECHK(void);
void BGD_ACTIVATE_TYCOGROUP(void);
void NOTCORNFLAKE(void);
void NOTREVERSED(void);
void NOTGROUND(void);
void NOTSIGN(void);
void NOTDYNAROAD(void);
void CHECK2(void);
void CHECK2II(void);
void NOOVERLAYGROUP(void);
void ADD_TO_NEWLIST(void);
void REGADD(void);
void FIND_SUBLIST_START_END(void);
void FSDN(void);
void APPEND_NEWLIST(void);
void GROUP_DELETE(void);
void DELLP(void);
void NXTCHK(void);
void GET_XZ_DISTANCE(void);
void BGD_OROUTINE(void);
void OVERCAR(void);
void CARFORWARD(void);
void ROAD_DEBRIS_CREATE_55GAL(void);
void ROAD_DEBRIS_CREATE(void);
void SMOKE_STACK(void);
void CAR_FIRE(void);
void DC_MINIFOUNTAIN(void);
void DC_FOUNTAIN(void);
void WATERFALL(void);
void WATERANI_PROC(void);
void WATERFALL_SND(void);
void AMBIENCE_SOUND(void);
void NOT_T1(void);
void HUNGH_ANI(void);
void HUNGH_ANI_REENTER(void);
void PLACE_ON_ROAD(void);
void RUT_ANI(void);
void PLAINANI_PROC_SLOW(void);
void FLAGWAVE_TALL(void);
void FLAGWAVE(void);
void MAKEPPP(void);
void PLAINANI_PROC(void);
void LOAD_SINGLE_SECTION(void);
void OHARE_PLANE(void);
void PLANE_FWRD(void);
void TRAIN_FWRD_MAKEB(void);
void TRAIN_FWRD_MAKE(void);
void TRAIN_FWRDB(void);
void TRAIN_FWRD(void);
void TRAINX(void);
void LOAD_SINGLE_SECTION_OFFSET(void);

/* asm: STARTSECTION	.bss	STARTSECTION,1 */
int STARTSECTION;
/* asm: START_POS	.bss	START_POS,3 */
int START_POS[3];
/* asm: START_RADY	.bss	START_RADY,1 */
int START_RADY;
/* asm: DRIVE_LIST	.bss	DRIVE_LIST,1 */
int DRIVE_LIST;
/* asm: CAR_LIST	.bss	CAR_LIST,1 */
int CAR_LIST;
/* asm: SIGN_LIST	.bss	SIGN_LIST,1 */
int SIGN_LIST;
/* asm: GROUND_LIST	.bss	GROUND_LIST,1 */
int GROUND_LIST;
/* *----------------------------------------------------------------------------
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
int NEW_GROUP;
/* asm: DGROUPS	.bss	DGROUPS,DGRP_SIZE*MAX_DGROUPS */
int DGROUPS[DGRP_SIZE*MAX_DGROUPS];
/* asm: DGROUP_COUNT	.bss	DGROUP_COUNT,1 */
int DGROUP_COUNT;
/* asm: DGROUP_AW	.bss	DGROUP_AW,1 */
int DGROUP_AW;
/* asm: DYNALIST_TRUEBEGIN	.bss	DYNALIST_TRUEBEGIN,1 */
int DYNALIST_TRUEBEGIN;
/* asm: DYNALIST_BEGIN	.bss	DYNALIST_BEGIN,1 */
int DYNALIST_BEGIN;
/* asm: DYNALIST_END	.bss	DYNALIST_END,1 */
int DYNALIST_END;
/* asm: SUBLIST_BEGIN	.bss	SUBLIST_BEGIN,1 */
int SUBLIST_BEGIN;
/* asm: SUBLIST_END	.bss	SUBLIST_END,1 */
int SUBLIST_END;
/* asm: STARTS	.bss	STARTS,1 */
int STARTS;
/* *----------------------------------------------------------------------------
 */
/* asm: SECTIONIDX	.bss	SECTIONIDX,1 */
int SECTIONIDX;
/* asm: TYCO_TRACK	.bss	TYCO_TRACK,1 */
int TYCO_TRACK;
/* asm: TYCO_TRACK_NTL	.bss	TYCO_TRACK_NTL,1 */
int TYCO_TRACK_NTL;
/* asm: TYCO_NTL_IDX	.bss	TYCO_NTL_IDX,1 */
int TYCO_NTL_IDX;
/* asm: ATTRACT_ACTIVATE_DIST	.float	15000 */
float ATTRACT_ACTIVATE_DIST = 15000.0f;
/* asm: ACTIVATE_DIST	.float	5000	;to activate */
float ACTIVATE_DIST = 5000.0f;
/* asm: DACT_DIST	.float	80000	;dynamic activate distance */
float DACT_DIST = 80000.0f;
/* asm: DDACT_DIST	.float	15000	;dynamic activate distance (+ radius) */
float DDACT_DIST = 15000.0f;
/* ;ATTR_DDACT_DIST		.float	35000	;dynamic deactivate distance (+ radius)
 */
/* asm: ATTR_DDACT_DIST	.float	45000	;dynamic deactivate distance (+ radius) */
float ATTR_DDACT_DIST = 45000.0f;
/* *----------------------------------------------------------------------------
*ACTIVATE A TYCO GROUP (ROAD PIECES ARE LINKED, ETC.)
*
*PARAMETERS
*	AR2	PTR TO TYCO BLOCK
*
*
*
 */
/* asm: NEWSUBLIST_TOP	.word	NEWSUBLIST_TOPB */
int *NEWSUBLIST_TOP = NEWSUBLIST_TOPB;
/* asm: NEWSUBLIST_TOPB	.bss	NEWSUBLIST_TOPB,1 */
int NEWSUBLIST_TOPB;
/* asm: GROUP_RADY	.bss	GROUP_RADY,1 */
int GROUP_RADY;
/* asm: TYCOFLAG	.bss	TYCOFLAG,1 */
int TYCOFLAG;
/* asm: PASS1	.bss	PASS1,1 */
int PASS1;
/* asm: SECRADY	.bss	SECRADY,1 */
int SECRADY;
/* asm: LVAL	.word	151720 */
int LVAL = 151720;
/* *----------------------------------------------------------------------------
 */
/* asm: OVERCARLIST	.word	dcbus,dgtruck,dsbus,dcbus */
int OVERCARLIST[] = {
    dcbus, dgtruck, dsbus, dcbus,
};
/* asm: SMOKE_ANI	.word	smoa,smob,smoc,smod,smoe,smof */
/* asm: 	.word	-1 */
int SMOKE_ANI[] = {
    smoa, smob, smoc, smod, smoe, smof,
    -1,
};
/* ;eug1,eug2,eug3,eug4,eug5,eug6
 */
/* asm: CAR_FIRE_ANII	.word	CAR_FIRE_ANI */
#define CAR_FIRE_ANII CAR_FIRE_ANI
/* asm: CAR_FIRE_ANI */
/* asm: 	.word	rdflm1,rdflm2,rdflm3,rdflm4,rdflm5,rdflm6 */
/* asm: 	.word	rdflm7,rdflm8,rdflm9,rdflm10,rdflm11,rdflm12 */
/* asm: 	.word	-1 */
int CAR_FIRE_ANI[] = {
    rdflm1, rdflm2, rdflm3, rdflm4, rdflm5, rdflm6,
    rdflm7, rdflm8, rdflm9, rdflm10, rdflm11, rdflm12,
    -1,
};
/* asm: DC_MINIFOUNTAIN_ANII	.word	DC_MINIFOUNTAIN_ANI */
#define DC_MINIFOUNTAIN_ANII DC_MINIFOUNTAIN_ANI
/* asm: DC_MINIFOUNTAIN_ANI */
/* asm: 	.word	aft1,aft2,aft3,aft4,aft5,aft6,-1 */
int DC_MINIFOUNTAIN_ANI[] = {
    aft1, aft2, aft3, aft4, aft5, aft6, -1,
};
/* asm: DC_FOUNTAIN_ANII	.word	DC_FOUNTAIN_ANI */
#define DC_FOUNTAIN_ANII DC_FOUNTAIN_ANI
/* asm: DC_FOUNTAIN_ANI */
/* asm: 	.word	ft2,ft3,ft4,ft5,ft6,-1 */
int DC_FOUNTAIN_ANI[] = {
    ft2, ft3, ft4, ft5, ft6, -1,
};
/* *----------------------------------------------------------------------------
 */
/* asm: WATERFALL_ANI	.word	w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,-1 */
int WATERFALL_ANI[] = {
    w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, -1,
};
/* *----------------------------------------------------------------------------
 */
/* asm: FLAGANI	.word	bflag1,bflag2,bflag3,bflag4,bflag5 */
/* asm: 	.word	bflag6,bflag7,bflag9,bflag10,-1 */
int FLAGANI[] = {
    bflag1, bflag2, bflag3, bflag4, bflag5,
    bflag6, bflag7, bflag9, bflag10, -1,
};
/* asm: FLAGANITALL	.word	aflag1,aflag2,aflag3,aflag4,aflag5 */
/* asm: 	.word	aflag6,aflag7,aflag9,aflag10,-1 */
int FLAGANITALL[] = {
    aflag1, aflag2, aflag3, aflag4, aflag5,
    aflag6, aflag7, aflag9, aflag10, -1,
};
/* asm: RUT_ANIS	.word	rut,rut2,rut3,-1 */
int RUT_ANIS[] = {
    rut, rut2, rut3, -1,
};
/* asm: HUNGH_ANIS	.word	hungh1,hungh2,hungh3,hungh4,hungh5,hungh6,hungh7,-1 */
int HUNGH_ANIS[] = {
    hungh1, hungh2, hungh3, hungh4, hungh5, hungh6, hungh7, -1,
};
/* asm: BABE_PALIST */
/* asm: 	.word	ungh1_blue,logo_p,ungh1_green,nintendo_p,ungh1_silver,map1_p */
/* asm: 	.word	ungh1_yellow,lift_p,ungh1_skin,bvwall_p */
int BABE_PALIST[] = {
    ungh1_blue, logo_p, ungh1_green, nintendo_p, ungh1_silver, map1_p,
    ungh1_yellow, lift_p, ungh1_skin, bvwall_p,
};
/* *----------------------------------------------------------------------------
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
/* asm: SINGLE_SECTION_TEMPPTR	.bss	SINGLE_SECTION_TEMPPTR,1 */
int SINGLE_SECTION_TEMPPTR;
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
/* asm: 	.word	4A1h,DC_FOUNTAIN */
/* asm: 	.word	4A2h,DC_MINIFOUNTAIN */
/* asm: 	.word	0	;END OF TABLE ID */
int ROUTINE_TAB[] = {
    0x40A, FLAGWAVE,
    0x460, ROAD_DEBRIS_CREATE,
    0x461, ROAD_DEBRIS_CREATE_55GAL,
    0x462, ROAD_DEBRIS_CREATE_55GAL, // actually TOXIC
    0x463, ROAD_DEBRIS_CREATE_55GAL, // actually CONE
    0x465, FLAGWAVE, // short flag
    0x466, FLAGWAVE_TALL, // tall flag
    0x467, WATERFALL,
    0x469, OVERCAR, // LA & CHICAGO, FREEWAY OVERPASS CAR
    0x470, RRSTART_ENGINE,
    0x471, RRSTART_BOXCAR,
    0x472, RRSTART_BOXCAR,
    0x473, RRSTART_BOXCAR,
    0x474, RRSTART_BOXCAR,
    0x475, RRSTART_BOXCAR,
    0x476, RRSTART_BOXCAR,
    0x481, SMOKE_STACK,
    0x482, CAR_FIRE,
    0x498, OHARE_PLANE, // CHICAGO AIRPLANE
    0x741, RUT_ANI,
    0x742, HUNGH_ANI,
    0x4A1, DC_FOUNTAIN,
    0x4A2, DC_MINIFOUNTAIN,
    0, // END OF TABLE ID
};

/* *----------------------------------------------------------------------------
 */
void FIND_STARTING_VALUES(void)
{
    // asm: 	LDI	@TYCO_TRKI,AR1
    // asm: 	LDI	@STARTSECTION,R0	;first time dont do the crime
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NOWARP
    // asm: 	LDI	R0,AR0
    // asm: 	DEC	AR0
LPP:
    // asm: LDI	*AR1++(6),R1
    // asm: 	TSTB	1,R1
    // asm: 	BZ	NOOVER
    // asm: 	ADDI	1,AR1
NOOVER:
    // asm: TSTB	SC_REVERSE,R1
    // asm: 	BZ	CNT
    // asm: 	NOP	*AR1++(4)
CNT:
    // asm: TSTB	SC_OVER2,R1
    // asm: 	BZ	CNTN
    // asm: 	NOP	*AR1++			;OVERLAY 2
CNTN:
    // asm: DBU	AR0,LPP
NOWARP:
    // asm: 	STI	AR1,@TYCO_TRACK
    // asm: 	STI	AR1,@DGROUP_AW
    // asm: 	STI	R0,@SECTIONIDX
    // asm: 	STI	R0,@TYCO_NTL_IDX
    // asm: 	LDF	*++AR1,R0
    // asm: 	STF	R0,@START_POS
    // asm: 	LDF	*++AR1,R0
    // asm: 	STF	R0,@START_POS+Y
    // asm: 	LDF	*++AR1,R0
    // asm: 	STF	R0,@START_POS+Z
    // asm: 	LDF	*++AR1,R2
    // asm: 	STF	R2,@START_RADY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_STARTING_VALUES", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*INITIALIZE the background system for the game
*
*
 */
void BGD_INIT(void)
{
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@DGROUP_COUNT
    // asm: 	STI	R0,@DYNALIST_END
    // asm: 	STI	R0,@DYNALIST_BEGIN
    // asm: 	STI	R0,@SIGN_LIST
    // asm: 	STI	R0,@TYCO_NTL_IDX
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
    // asm: 	CALL	FIND_STARTING_VALUES
    // asm: 	BU	KKTIL
    // asm: REGULAR_START				;kept for attract mode only
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@SECTIONIDX
    // asm: 	STI	R0,@TYCO_NTL_IDX
    // asm: 	LDI	@TYCO_TRKI,R0
    // asm: 	STI	R0,@TYCO_TRACK
    // asm: 	STI	R0,@DGROUP_AW
    // asm: 	CLRF	R0
    // asm: 	STF	R0,@START_POS+X
    // asm: 	STF	R0,@START_POS+Y
    // asm: 	STF	R0,@START_POS+Z
    // asm: 	STF	R0,@START_RADY
KKTIL:
    // asm: 	LDI	@DGROUP_AW,AR2
    // asm: 	LDI	AR2,AR0
    // asm: 	CALL	BGD_ACTIVATE_TYCOGROUP	;returns top pointer in R0
    // asm: 	LDI	@DGROUPSI,AR1
    // asm: 	STI	R0,*+AR1(DGRP_HEAD)	;lead object (link by OLINK3)
    // asm: 	STI	AR0,*+AR1(DGRP_BIN)	;rom struct ptr
    // asm: 	LDI	@STARTS,R0
    // asm: 	STI	R0,*+AR1(DGRP_FSTART)
    // asm: 	LDI	@SECTIONIDX,R0
    // asm: 	STI	R0,*+AR1(DGRP_IDX)	;SAVE THE INDEX #
    // asm: 	STI	R0,@TYCO_NTL_IDX
    // asm: 	LDI	@TYCOFLAG,R0
    // asm: 	STI	R0,*+AR1(DGRP_FLAG)	;SAVE THE TYCOFLAG
    // asm: 	INCM	@DGROUP_COUNT
    // asm: 	LDI	@DGROUP_AW,AR0
    // asm: 	STI	AR0,@TYCO_TRACK_NTL
    // asm: 	LDI	*AR0++(6),R1
    // asm: 	TSTB	1,R1
    // asm: 	BZ	NOOVERD
    // asm: 	ADDI	1,AR0
NOOVERD:
    // asm: 	TSTB	SC_REVERSE,R1
    // asm: 	BZ	CNT55a
    // asm: 	NOP	*AR0++(4)
CNT55a:
    // asm: TSTB	SC_OVER2,R1
    // asm: 	BZ	NOO2a
    // asm: 	ADDI	1,AR0
NOO2a:
    // asm: STI	AR0,@DGROUP_AW
#if DEBUG
    // asm: 	LDI	@DGROUP_COUNT,R0
    // asm: 	CMPI	1,R0
    // asm: 	SLOCKON	NE,"DGROUP\BACKGRND_INIT ERROR"
#endif
    // asm: 	PUSH	AR6
    // asm: 	LDI	@DGROUPSI,AR1
    // asm: 	LDI	3,AR6
    // asm: LPP5
    // asm: 	LDI	@DGROUP_AW,AR2
    // asm: 	LDI	AR2,AR0
    // asm: 	CALL	BGD_ACTIVATE_TYCOGROUP	;returns top pointer in R0
    // asm: 	STI	R0,*+AR1(DGRP_HEAD+DGRP_SIZE)	;lead object (link by OLINK3)
    // asm: 	STI	AR0,*+AR1(DGRP_BIN+DGRP_SIZE)	;rom struct ptr
    // asm: 	LDI	@STARTS,R0
    // asm: 	STI	R0,*+AR1(DGRP_FSTART+DGRP_SIZE)
    // asm: 	LDI	@SECTIONIDX,R0
    // asm: 	STI	R0,*+AR1(DGRP_IDX+DGRP_SIZE)	;SAVE THE INDEX #
    // asm: 	LDI	@TYCOFLAG,R0
    // asm: 	STI	R0,*+AR1(DGRP_FLAG+DGRP_SIZE)	;SAVE THE TYCOFLAG
    // asm: 	INCM	@DGROUP_COUNT
    // asm: 	LDI	@DGROUP_AW,AR0
    // asm: 	LDI	*AR0++(6),R1
    // asm: 	TSTB	1,R1
    // asm: 	BZ	NOOVERE
    // asm: 	ADDI	1,AR0
NOOVERE:
    // asm: TSTB	SC_REVERSE,R1
    // asm: 	BZ	CNT55d
    // asm: 	NOP	*AR0++(4)
CNT55d:
    // asm: TSTB	SC_OVER2,R1
    // asm: 	BZ	NOO2d
    // asm: 	ADDI	1,AR0
NOO2d:
    // asm: STI	AR0,@DGROUP_AW
    // asm: 	ADDI	DGRP_SIZE,AR1
    // asm: 	DBU	AR6,LPP5
    // asm: 	POP	AR6
    // asm: 	CREATE	BGD_WATCHER,UTIL_C|BACKGRND_T
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BGD_INIT", 0, 0);
    UNIMPL();
}

void BGD_WATCHER(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BEQ	LLKK
    // asm: 	CMPI	MATTR,R0
    // asm: 	BNE	BGD_SLP
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
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	NO_ACTIVATION
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDI	*+AR0(CARTRAK),AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	LDI	@TYCO_NTL_IDX,R1
    // asm: 	LS	8,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BLT	NO_ACTIVATION
    // 	;blah.asm
    // asm: 	LDI	@TYCO_TRACK_NTL,AR2
    // asm: 	LDI	*AR2,AR0
    // asm: 	RS	16,AR0
    // asm: 	AND	0FFh,AR0
    // asm: 	LDI	AR0,R0
    // asm: 	CALLNZ	SECTION_ROUTINE
    // asm: 	LDI	@TYCO_NTL_IDX,R0
    // asm:  	INC	R0
    // asm: 	STI	R0,@TYCO_NTL_IDX
    // asm: 	LDI	@TYCO_TRACK_NTL,AR1
    // asm: 	LDI	*AR1++(6),R1
    // asm: 	TSTB	1,R1
    // asm: 	BZ	NOOVERA
    // asm: 	ADDI	1,AR1
NOOVERA:
    // asm: TSTB	SC_REVERSE,R1
    // asm: 	BZ	CNTFF
    // asm: 	NOP	*AR1++(4)
CNTFF:
    // asm: TSTB	SC_OVER2,R1
    // asm: 	BZ	NOO2FF
    // asm: 	ADDI	1,AR1
NOO2FF:
    // asm: STI	AR1,@TYCO_TRACK_NTL
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
    // asm: 	LDI	@DGROUP_AW,AR0
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	1,AR2
    // asm: 	LDI	@CAMERAPOSI,R2
    // asm: 	CALL	GET_XZ_DISTANCE
    // asm: 	CMPF	@DACT_DIST,R0
    // asm: 	BGT	NOACT
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	BGD_ACTIVATE_TYCOGROUP	;returns top pointer in R0
    // asm: 	LDI	@DGROUP_COUNT,AR1
    // asm: 	MPYI	DGRP_SIZE,AR1
    // asm: 	ADDI	@DGROUPSI,AR1
    // asm: 	STI	R0,*+AR1(DGRP_HEAD)	;lead object (link by OLINK3)
    // asm: 	STI	AR0,*+AR1(DGRP_BIN)	;rom struct ptr
    // asm: 	LDI	@SECTIONIDX,R0
    // asm: 	STI	R0,*+AR1(DGRP_IDX)	;SAVE THE INDEX #
    // asm: 	LDI	@TYCOFLAG,R0
    // asm: 	STI	R0,*+AR1(DGRP_FLAG)	;SAVE THE TYCOFLAG
    // asm: 	LDI	@STARTS,R0
    // asm: 	STI	R0,*+AR1(DGRP_FSTART)
    // asm: 	INCM	@DGROUP_COUNT
    // asm: 	LDI	*AR0++(6),R1
    // asm: 	TSTB	1,R1
    // asm: 	BZ	NOOVERB
    // asm: 	ADDI	1,AR0
NOOVERB:
    // asm: TSTB	SC_REVERSE,R1
    // asm: 	BZ	CNT55
    // asm: 	NOP	*AR0++(4)
CNT55:
    // asm: TSTB	SC_OVER2,R1
    // asm: 	BZ	NOO2
    // asm: 	ADDI	1,AR0
NOO2:
    // asm: STPI	AR0,@DGROUP_AW
    // asm: NOACT	;end of activation routine
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
    // asm: 	LDI	@(DGROUPS+DGRP_BIN),AR2
    // asm: 	INC	AR2
    // asm: 	LDI	@CAMERAPOSI,R2
    // asm: 	CALL	GET_XZ_DISTANCE
    // asm: 	LDI	@(DGROUPS+DGRP_BIN),AR0
    // asm: 	LDI	*+AR0(TB_GROUP),AR0
    // asm: 	SUBF	*AR0,R0
    // asm: 	LDI	@_MODE,R1
    // asm: 	AND	MMODE,R1
    // asm: 	CMPI	MATTR,R1
    // asm: 	BNE	GAMECHK
    // asm: 	CMPF	@ATTR_DDACT_DIST,R0
    // asm: 	BLT	NODEACT
    // asm: 	BU	DO_DEL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BGD_WATCHER", 0, 0);
    UNIMPL();
}

void GAMECHK(void)
{
    // asm: CMPF	@DDACT_DIST,R0
    // asm: 	BLT	NODEACT
DO_DEL:
    // asm: 	LDI	@(DGROUPS+DGRP_HEAD),AR2
    // asm: 	CALL	GROUP_DELETE
    // asm: 	LDI	@(DGROUPS+DGRP_SIZE+DGRP_FSTART),AR2
    // asm: 	STI	AR2,@DYNALIST_BEGIN
    // asm: 	STI	AR2,@DYNALIST_TRUEBEGIN
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR2(OBLINK4)	;NULL TERMINATE BACK LINK ON LIST
    // 	;REPACK DGROUP SECTIONS
    // 	;
    // 	;for i = 1 to dgroup_count
    // 	;	memcpy(dgroup[i-1], dgroup[i], sizeof(dgroup))
    // 	;
    // asm: 	LDI	@DGROUP_COUNT,RC
    // asm: 	SUBI	2,RC
#if DEBUG
    // asm: 	CMPI	0,RC
    // asm: 	SLOCKON	LT,"BACKGRND\LBACK_WATCH ERROR"
#endif
    // asm: 	BLT	SHIFT1
    // asm: 	LDI	@DGROUPSI,AR0
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	DGRP_SIZE,AR1
    // asm: 	RPTB	LL45
SHIFT1:
    // asm: LDI	*AR1++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	*AR1++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	*AR1++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	*AR1++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	*AR1++,R0
LL45:
    // asm: STI	R0,*AR0++
    // asm: 	DECM	@DGROUP_COUNT
NODEACT:
#if DEBUG
    // 	;DEBUGGING CHECK TO VERIFY THAT WE NEVER EXCEED
    // 	;THE AMOUNT OF DGROUPs WE CAN HANDLE
    // 	;
    // asm: 	LDI	@DGROUP_COUNT,R0
    // asm: 	SLOCKON	LE,"BACKGRND\LBACK_WATCH ERRONEOUS DGROUP_COUNT LE"
    // asm: 	CMPI	MAX_DGROUPS,R0
    // asm: 	SLOCKON	GE,"BACKGRND\LBACK_WATCH ERRONEOUS DGROUP_COUNT GE"
#endif
    // asm: 	SLEEP	3
    // asm: 	B	BGD_WATCHER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GAMECHK", 0, 0);
    UNIMPL();
}

void BGD_ACTIVATE_TYCOGROUP(void)
{
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR7
    // asm: 	LDI	@SECTIONIDX,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@SECTIONIDX
    // *CORNFLAKE CHECK
    // asm: 	CMPI	4,R0
    // asm: 	BNE	NOTCORNFLAKE
    // asm: 	PUSH	AR4
    // asm: 	LDL	cornpops,AR2
    // asm: 	LDI	6700,R2
    // asm: 	LDI	-4250,R3
    // asm: 	LDI	32600,RC
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BGD_ACTIVATE_TYCOGROUP", 0, 0);
    UNIMPL();
}

void NOTCORNFLAKE(void)
{
    // *CORNFLAKE END CHECK
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@SUBLIST_BEGIN
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@PASS1
    // asm: 	STI	R0,@NEW_GROUP
    // asm: 	LDI	@TYCO_TRACK,AR2
    // asm: 	LDI	AR2,AR7				;DEDICATED POINTER
    // asm: 	LDI	*AR7,R0				;LOAD FLAG
    // asm: 	STI	R0,@TYCOFLAG
    // asm: 	TSTB	SC_OVERLAY,R0
    // asm: 	BZ	NOVRDD
    // asm: 	ADDI	1,AR2
NOVRDD:
    // asm: TSTB	SC_REVERSE,R0
    // asm: 	BZD	REG_LD
    // asm: 	NOP
    // asm: 	ADDI	TB_REGSIZE-1,AR2
    // asm: 	STI	AR2,@TYCO_TRACK
    // 	;---->	BZD	REG_LD
    // asm: 	ADDI	TB_RVSSIZE-TB_REGSIZE,AR2	;REVERSED SECTION HAS LARGER STRUCTURE
    // asm: 	STI	AR2,@TYCO_TRACK
REG_LD:
    // asm: TSTB	SC_OVER2,R0			;hopefully...
    // asm: 	BZ	NOEXTRA
    // asm: 	LDI	@TYCO_TRACK,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@TYCO_TRACK
NOEXTRA:
    // asm: 	LDF	*+AR7(TB_RADY),R0
    // asm: 	STF	R0,@SECRADY
    // asm: 	LDI	@MATRIXAI,AR2		;Group rotation matrix
    // asm: 	LDF	@SECRADY,R2
    // asm: 	CALL	HPFIND_YMATRIX		;require High Precision
    // asm: 	LDI	*+AR7(TB_GROUP),AR5	;Group pointer
    // asm: 	ADDI	1,AR5			;skip radius
    // asm: 	LDI	@SECTIONIDX,R0
    // asm: 	LS	8,R0
    // asm: 	OR	0AAh,R0
    // asm: 	STI	R0,@NEWSUBLIST_TOPB
    // asm: 	PUSH	R0
    // asm: 	LDI	*AR5++,R4		;get number of objects to load
    // asm: 	SLOCKON	LE,"BACKGRND 1"
    // asm: 	SUBI	1,R4
    // asm: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\ACTIVATE TYCOGROUP OUT OF OBJECTS"
L12:
    // asm: 	LDI	*AR5++,AR2		;GET MODEL PTR
    // asm: 	CALL	OBJ_GETE
    // asm: 	SLOCKON	C,"BACKGRND\ACTIVATE   DANGER ERROR *FATAL*"
    // asm: 	BC	ACTIVATE_X
    // 					;INTERNAL LINK (FOR DEALLOCATION)
    // asm: 	LDI	AR0,AR4			;SET OBJECT ROM PTR
    // asm: 	POP	R0			;GET LAST BACK LINK
    // asm: 	STI	R0,*+AR4(OLINK2)
    // asm: 	PUSH	R0
    // asm: 	FLOAT	*AR5++,R1		;GET X POSITION
    // asm: 	STF	R1,*+AR4(OPOSX)
    // asm: 	FLOAT	*AR5++,R1		;GET Y POSITION
    // asm: 	LDI	@TYCOFLAG,R0
    // asm: 	TSTB	SC_REVERSE,R0
    // asm: 	BZD	NOTREVERSED
    // asm: 	STF	R1,*+AR4(OPOSY)
    // asm: 	FLOAT	*AR5++,R1		;GET Z POSITION
    // asm: 	STF	R1,*+AR4(OPOSZ)
    // 	;---->BZD	NOTREVERSED
    // 	;
    // 	;	SPECIAL REVERSED CASE
    // 	;
    // asm: 	PUSH	AR7
    // asm: 	TSTB	SC_OVERLAY,R0
    // asm: 	BNZ	ISOVER
    // asm: 	DEC	AR7
ISOVER:
    // asm: 	LDF	*+AR7(TB_RVS_POSX),R0	;TRANSLATE BY THE NEGATIVE OFFSET
    // asm: 	ADDF	*+AR4(OPOSX),R0		;POSITION (THIS BLOCKS ENDING POSITION)
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR7(TB_RVS_POSY),R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR7(TB_RVS_POSZ),R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	POP	AR7
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OPOSX,AR2
    // asm: 	LDI	@VECTORAI,R3
    // asm: 	CALL	MATRIX_MUL		;rotation by occurance matrix
    // asm: 	LDI	@VECTORAI,AR0
    // asm: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR7(TB_POSX),R1
    // asm: 	STF	R1,*+AR4(OPOSX)
    // asm: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR7(TB_POSY),R1
    // asm: 	STF	R1,*+AR4(OPOSY)
    // asm: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR7(TB_POSZ),R1
    // asm: 	STF	R1,*+AR4(OPOSZ)
    // asm: 	LDF	*AR5++,R2		;GET Y ROT
    // asm: 	ADDF	@SECRADY,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	HPFIND_YMATRIX
    // asm: 	BU	JOIN_UP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTCORNFLAKE", 0, 0);
    UNIMPL();
}

void NOTREVERSED(void)
{
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OPOSX,AR2
    // asm: 	LDI	R2,R3
    // asm: 	LDI	@VECTORAI,R3
    // asm: 	CALL	MATRIX_MUL		;ROTATION BY OCCURANCE MATRIX
    // asm: 	LDI	@VECTORAI,AR0
    // asm: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR7(TB_POSX),R1
    // asm: 	STF	R1,*+AR4(OPOSX)
    // asm: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR7(TB_POSY),R1
    // asm: 	STF	R1,*+AR4(OPOSY)
    // asm: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR7(TB_POSZ),R1
    // asm: 	STF	R1,*+AR4(OPOSZ)
    // asm: 	LDF	*AR5++,R2		;SET THE RADIANS FOR THE OBJECT
    // asm: 	ADDF	@SECRADY,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	HPFIND_YMATRIX
JOIN_UP:
    // asm: 	LDI	*AR5++,R1		;LOAD OBJECT ID (GENV STYLE)
    // asm: 	LDI	R1,R2
    // asm: 	AND	CLASS_M|TYPE_M|SUBTYPE_M,R1
    // asm: 	STI	R1,*+AR4(OID)
    // 	;TEST TO SEE IF WE SHOULD CALL A SPECIAL ROUTINE
    // 	;OR PALETTE SHIFT
    // 	;
    // asm: 	TSTB	BGD_ROUTINE,R2
    // asm: 	CALLNZ	BGD_OROUTINE
    // asm: 	LDI	R2,R0
    // asm: 	RS	16,R2
    // asm: 	AND	O_GENVSPEC,R2		;make sure list data is not ORed in
    // asm: 	TSTB	BGD_BIGOBJ,R0		;BIG OBJECT TEST
    // asm: 	BZ	NOTBIGOBJ
    // asm: 	LDI	1,R0
    // asm: 	LS	O_BIGOBJECT_B,R0
    // asm: 	OR	R0,R2
NOTBIGOBJ:
    // asm: 	OR	*+AR4(OFLAGS),R2	;or in the flags
    // asm: 	STI	R2,*+AR4(OFLAGS)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT			;INSERT THE BABE
    // 	;NOW FIND IF THE OBJECT BELONGS IN A SUPPLIMENTAL
    // 	;LIST.
    // 	;OID =	x3xx	-> DRIVE_SUPP
    // 	;	x4xx	-> BUILD_SUPP
    // 	;
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	AND	CLASS_M,R0
    // asm: 	CMPI	ROAD_C,R0
    // asm: 	BNE	NOTDRIVE
    // asm: 	LDI	@DRIVE_LIST,AR0
    // asm: 	STI	AR0,*+AR4(OLINK3)
    // asm: 	STI	AR4,@DRIVE_LIST
    // asm: 	LDI	1,R1
    // asm: 	BUD	DONELISTS
    // asm: 	LS	28,R1			;O_ROAD_SUPP
    // asm: 	OR	*+AR4(OFLAGS),R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
NOTDRIVE:
    // asm: 	CMPI	GROUND_C,R0
    // asm: 	BNE	NOTGROUND
    // asm: 	LDI	@GROUND_LIST,AR0
    // asm: 	STI	AR0,*+AR4(OLINK3)
    // asm: 	STI	AR4,@GROUND_LIST
    // asm: 	LDI	1,R1
    // asm: 	LS	O_GROUND_B,R1
    // asm: 	OR	*+AR4(OFLAGS),R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // asm: 	B	DONELISTS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTREVERSED", 0, 0);
    UNIMPL();
}

void NOTGROUND(void)
{
    // asm: 	CMPI	TSIGN_C,R0
    // asm: 	BNE	NOTSIGN
    // asm: 	LDI	@SIGN_LIST,AR0
    // asm: 	STI	AR0,*+AR4(OLINK3)
    // asm: 	STI	AR4,@SIGN_LIST
    // asm: 	LDI	1,R1
    // asm: 	LS	O_SIGN_SUPP_B,R1
    // asm: 	OR	*+AR4(OFLAGS),R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // asm: 	B	DONELISTS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTGROUND", 0, 0);
    UNIMPL();
}

void NOTSIGN(void)
{
    // asm: 	CMPI	0B00h,R0
    // asm: 	BNE	NOTDYNAROAD
    // 	;
    // 	;add this element to dynamic fLEX list
    // 	;
    // asm: 	LDI	@SECTIONIDX,R1
    // asm: 	LS	8,R1
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	AND	0FFh,R0
    // asm: 	LDI	@TYCOFLAG,R2			;in the case of reversed track
    // asm: 	TSTB	SC_REVERSE,R2			;we say the index value is
    // asm: 	BZ	NOTRVSTRK			;255 - index
    // asm: 	SUBRI	255,R0				;
    // asm: NOTRVSTRK					;
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,*+AR4(OUSR1)
    // asm: 	LDI	0300h,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	@DRIVE_LIST,AR0
    // asm: 	STI	AR0,*+AR4(OLINK3)
    // asm: 	STI	AR4,@DRIVE_LIST
    // asm: 	LDI	1,R1
    // asm: 	LS	O_DRIVE_SUPP_B,R1
    // asm: 	OR	*+AR4(OFLAGS),R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // asm: 	CALL	ADD_TO_NEWLIST
    // asm: 	BU	DONELISTS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTSIGN", 0, 0);
    UNIMPL();
}

void NOTDYNAROAD(void)
{
DONELISTS:
    // asm: 	SUBI	1,R4
    // asm: 	BGE	L12
ACTIVATE_X:
    // asm: 	POP	AR0			;clear stack of last item
    // asm: 	LDI	@PASS1,R0
    // asm: 	BNZ	CHECK2
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@PASS1
    // asm: 	LDI	*AR7,R0			;load flag
    // asm: 	TSTB	SC_OVERLAY,R0
    // asm: 	BZ	CHECK2II
    // asm: 	LDI	*+AR7(TB_GROUPOVERLAY),AR5	;Group pointer
    // asm: 	ADDI	1,AR5			;skip radius
    // asm: 	LDI	@NEWSUBLIST_TOPB,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	*AR5++,R4		;get number of objects to load
    // asm: 	SLOCKON	LE,"BACKGRND  ERRONEOUS GROUP LOADED"
    // asm: 	SUBI	1,R4
    // asm: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\ACTIVATE TYCOGROUP OUT OF OBJECTS 2"
    // asm: 	BU	L12
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTDYNAROAD", 0, 0);
    UNIMPL();
}

void CHECK2(void)
{
    // asm: 	CMPI	2,R0
    // asm: 	BEQ	NOOVERLAYGROUP
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@PASS1
    // asm: 	LDI	*AR7,R0			;load flag
    // asm: 	TSTB	SC_OVER2,R0
    // asm: 	BZ	NOOVERLAYGROUP
    // asm: 	PUSH	IR0
    // asm: 	TSTB	SC_REVERSE,R0
    // asm: 	LDIZ	TB_GROUPOVERLAY+1,IR0
    // asm: 	LDINZ	TB_GROUPOVERLAY+5,IR0
    // asm: 	LDI	*+AR7(IR0),AR5	;Group pointer
    // asm: 	ADDI	1,AR5			;skip radius
    // asm: 	TSTB	SC_REVERSE,R0
    // asm: 	BZ	UHNO2
    // asm: 	TSTB	SC_OVERLAY,R0
    // asm: 	LDIZ	TB_RVS_RADY-1,IR0
    // asm: 	LDINZ	TB_RVS_RADY,IR0
    // asm: 	LDF	*+AR7(IR0),R0
    // asm: 	STPF	R0,@SECRADY
UHNO2:
    // asm: 	POP	IR0
    // asm: 	LDI	@TYCOFLAG,R0		;overlay 2 is not reversed - EVER!
    // asm: 	ANDN	SC_REVERSE,R0
    // asm: 	STI	R0,@TYCOFLAG
    // asm: 	LDI	@NEWSUBLIST_TOPB,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	*AR5++,R4		;get number of objects to load
    // asm: 	SLOCKON	LE,"BACKGRND  ERRONEOUS GROUP LOADED OVER2"
    // asm: 	SUBI	1,R4
    // asm: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\ACTIVATE TYCOGROUP OUT OF OBJECTS 2"
    // asm: 	BU	L12
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK2", 0, 0);
    UNIMPL();
}

void CHECK2II(void)
{
    // asm: 	CMPI	2,R0
    // asm: 	BEQ	NOOVERLAYGROUP
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@PASS1
    // asm: 	LDI	*AR7,R0			;load flag
    // asm: 	TSTB	SC_OVER2,R0
    // asm: 	BZ	NOOVERLAYGROUP
    // asm: 	PUSH	IR0
    // asm: 	TSTB	SC_REVERSE,R0
    // asm: 	LDIZ	TB_GROUPOVERLAY,IR0
    // asm: 	LDINZ	TB_GROUPOVERLAY+4,IR0
    // asm: 	LDI	*+AR7(IR0),AR5		;Group pointer
    // asm: 	TSTB	SC_REVERSE,R0
    // asm: 	BZ	UHNO
    // asm: 	TSTB	SC_OVERLAY,R0
    // asm: 	LDIZ	TB_RVS_RADY-1,IR0
    // asm: 	LDINZ	TB_RVS_RADY,IR0
    // asm: 	LDF	*+AR7(IR0),R0
    // asm: 	STPF	R0,@SECRADY
UHNO:
    // asm: 	POP	IR0
    // asm: 	ADDI	1,AR5			;skip radius
    // asm: 	LDI	@TYCOFLAG,R0		;overlay 2 is not reversed - EVER!
    // asm: 	ANDN	SC_REVERSE,R0
    // asm: 	STI	R0,@TYCOFLAG
    // asm: 	LDI	@NEWSUBLIST_TOPB,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	*AR5++,R4		;get number of objects to load
    // asm: 	SLOCKON	LE,"BACKGRND  ERRONEOUS GROUP LOADED OVER2"
    // asm: 	SUBI	1,R4
    // asm: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\ACTIVATE TYCOGROUP OUT OF OBJECTS 2"
    // asm: 	BU	L12
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK2II", 0, 0);
    UNIMPL();
}

void NOOVERLAYGROUP(void)
{
    // 	;*NOW DO THE FANCY MESS OF:
    // 	;*
    // 	;*	1)	DO WE LOAD A NEW OVERLAY INTO A SECTION?
    // 	;*	2)	WHICH SECTION DO WE OVERLAY IT INTO (1 OR 2)?
    // 	;*	3)	WHICH LOOK DO WE PLACE INTO THE SECTION (DESERT..)?
    // 	;*
    // 	;*
    // asm: 	LDI	@TYCOFLAG,R0
    // asm: 	TSTB	SC_LDSECT,R0
    // asm: 	BZ	NO_NEWLOAD
NO_NEWLOAD:
    // asm: 	CALL	FIND_SUBLIST_START_END
    // asm: 	CALL	APPEND_NEWLIST
    // asm: 	LDI	@NEWSUBLIST_TOPB,R0		;return pointer to 1st object
    // asm: 	POP	AR7
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOOVERLAYGROUP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void ADD_TO_NEWLIST(void)
{
    // asm: 	LDI	@NEW_GROUPI,AR1
    // asm: 	LDI	*AR1,R0
    // asm: 	BNZ	REGADD
    // asm: 	STI	AR4,*AR1
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR4(OLINK4)
    // asm: 	STI	R0,*+AR4(OBLINK4)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADD_TO_NEWLIST", 0, 0);
    UNIMPL();
}

void REGADD(void)
{
    // asm: 	SUBI	OLINK4,AR1
    // asm: 	LDI	AR1,AR2
    // asm: 	LDI	*+AR4(OUSR1),R0		;GET fLEX index number
ADDNOBJL:
    // asm: 	LDI	AR1,AR0			;AR0 IS PREVIOUS LINK
    // asm: 	LDI	*+AR1(OLINK4),R1	;CHECK END OF LIST
    // asm: 	BZ	ADDNOBJ_AT_END
    // asm: 	LDI	R1,AR1
    // asm: 	CMPI	*+AR1(OUSR1),R0
    // asm: 	BGT	ADDNOBJL
ADDNOBJ_AT_END:
    // asm: 	STI	R1,*+AR4(OLINK4)	;LINK TO NEXT (SUBLIST LINK)
    // asm: 	STI	AR4,*+AR0(OLINK4)	;LINK FROM PREVIOUS (SUBLIST LINK)
    // asm: 	CMPI	AR2,AR0			;IS IT THE HEAD OF LIST WE LOAD AT
    // asm: 	LDIEQ	0,AR0
    // asm: 	STI	AR0,*+AR4(OBLINK4)	;BACK LINK
    // asm: 	CMPI	0,R1			;IS THE NEXT ELEMENT NULL
    // asm: 	BEQ	ISZERO
    // asm: 	LDI	R1,AR0			;IF NOT THE NEXT ELEMENTS BACK LINK
    // asm: 	STI	AR4,*+AR0(OBLINK4)	;  MUST POINT TO NEW ELEMENT
ISZERO:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REGADD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FIND_SUBLIST_START_END(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR0
    // asm: 	LDI	@NEW_GROUP,AR0
#if DEBUG
    // asm: 	CMPI	0,AR0
    // asm: 	SLOCKON	EQ,"BACKGRND\FIND_SUBLIST_START_END"
#endif
    // asm: 	STPI	AR0,@SUBLIST_BEGIN
FSLP:
    // asm: 	LDI	*+AR0(OLINK4),R1	;CHECK END OF LIST
    // asm: 	BZ	FSDN
    // asm: 	LDI	R1,AR0
    // asm: 	BU	FSLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_SUBLIST_START_END", 0, 0);
    UNIMPL();
}

void FSDN(void)
{
    // asm: 	STPI	AR0,@SUBLIST_END	;ALWAYS NAME IT AS THE END
    // asm: 	POP	AR0
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FSDN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void APPEND_NEWLIST(void)
{
    // asm: 	LDI	@DYNALIST_END,R0
    // asm: 	BZ	SHINY_NEWLIST
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	@SUBLIST_BEGIN,R0
    // asm: 	STI	R0,*+AR0(OLINK4)
    // asm: 	LDI	@SUBLIST_END,AR1
    // asm: 	STPI	AR1,@DYNALIST_END
    // asm: 	LDI	R0,AR1
    // asm: 	STI	AR0,*+AR1(OBLINK4)
    // asm: 	STPI	R0,@STARTS
    // asm: 	RETS
SHINY_NEWLIST:
    // asm: 	LDI	@SUBLIST_END,R0
    // asm: 	STPI	R0,@DYNALIST_END
    // asm: 	LDI	@SUBLIST_BEGIN,R0	;new line, check it out...
    // asm: 	STPI	R0,@DYNALIST_TRUEBEGIN
    // asm: 	LDI	@SUBLIST_BEGIN,R0
    // asm: 	SLOCKON	Z,"BACKGRND\SHINY_NEWLIST"
    // asm: 	STPI	R0,@DYNALIST_BEGIN
    // asm: 	STPI	R0,@STARTS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "APPEND_NEWLIST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void GROUP_DELETE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDI	@OACTIVEI,R1
    // asm: 	CALL	DELLP
    // asm: 	LDI	@IDLE_LISTI,R1
    // asm: 	CALL	DELLP
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GROUP_DELETE", 0, 0);
    UNIMPL();
}

void DELLP(void)
{
    // asm: LDI	R1,AR1			;WE MUST FIND DEAD OBJECT TO LINK AROUND
    // asm: 	LDI	*AR1,R1
    // asm: 	BZ	NXTCHK
    // asm: 	LDI	R1,AR0
    // asm: 	CMPI	*+AR0(OLINK2),AR2
    // asm: 	BNE	DELLP
    // asm: 	LDI	*AR0,R0			;find link from delete-e
    // asm: 	STI	R0,*AR1			;LINK AROUND
    // asm: 	PUSH	AR2
    // asm: 	LDI	AR0,AR2
    // 	;Mirror these checks in OBJ.ASM
    // 	;	'OBJ_DELETE'
    // 	;
    // asm: 	LDI	*+AR2(OFLAGS),R0	;KILL PROC ASSOCIATED WITH OBJ
    // asm: 	RS	(O_DEBRIS_B+1),R0
    // asm: 	CALLC	FREE_RDDEBRIS
    // asm: 	LDI	*+AR2(OFLAGS),R0	;KILL PROC ASSOCIATED WITH OBJ
    // asm: 	RS	(O_PROC_B+1),R0
    // asm: 	CALLC	OBJ_FREE_PROC
    // asm: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF DRIVE LIST
    // asm: 	RS	(O_DRIVE_SUPP_B+1),R0
    // asm: 	CALLC	OBJ_FREE_DRIVE
    // asm: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF GROUND LIST
    // asm: 	RS	(O_GROUND_B+1),R0
    // asm: 	CALLC	OBJ_FREE_GROUND
    // asm: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF SIGN LIST
    // asm: 	RS	(O_SIGN_SUPP_B+1),R0
    // asm: 	CALLC	OBJ_FREE_SIGN
    // asm: 	POP	AR2
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR0(OLINK2)	;CLEAR SEARCH ID
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	@OFREE,R0
    // asm: 	STI	R0,*AR0			;place delete-e on free
    // asm: 	STI	AR0,@OFREE
    // asm: 	LDI	@OFREECNT,R0		;INCREMENT FREE OBJECT COUNT
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,@OFREECNT
    // asm: 	LDI	AR1,R1
    // asm: 	BU	DELLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELLP", 0, 0);
    UNIMPL();
}

void NXTCHK(void)
{
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NXTCHK", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void GET_XZ_DISTANCE(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	R3
    // asm: 	LDI	R2,AR0
    // asm: 	LDF	*AR0++(2),R3
    // asm: 	SUBF	*AR2++(2),R3
    // asm: 	LDF	*AR0++,R2
    // asm: 	SUBF	*AR2++,R2
    // asm: 	MPYF	R3,R3
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R3,R2
    // asm: 	CALL	SQRT
    // asm: 	POP	R3
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_XZ_DISTANCE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void BGD_OROUTINE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR0
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	LDI	@ROUTINE_TABI,AR0
    // asm: BGDORLP	CMPI	*AR0,R0
    // asm: 	BEQ	BGD_RFND
    // asm: 	LDI	*AR0++(2),R1
    // asm: 	BNZ	BGDORLP
    // asm: 	POP	AR0
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
BGD_RFND:
    // asm: 	LDI	*+AR0,R0
    // asm: 	CALLU	R0
    // asm: 	POP	AR0
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BGD_OROUTINE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void OVERCAR(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR2
    // asm: 	CREATE	CARFORWARD,22
    // asm: 	STI	AR0,*+AR4(OPLINK)
    // asm: 	LDI	SPAWNER_C,R0
    // asm: 	STI	R0,*+AR0(PID)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	RANDN	4
    // asm: 	ADDI	@OVERCARLISTI,R0
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	STI	AR2,*+AR4(OROMDATA)
    // asm: 	POP	AR2
    // asm: 	POP	R2
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OVERCAR", 0, 0);
    UNIMPL();
}

void CARFORWARD(void)
{
    // asm: 	RANDN	50
    // asm: 	CMPI	25,R0
    // asm: 	BLT	CARSUP
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	16000,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR2(Y),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR2(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR4(ORADY),R2
    // asm: 	ADDF	PI,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
CARSUP:
    // asm: 	RANDN	500
    // asm: 	FLOAT	R0
    // asm: 	CALL	CLR_VECTORA
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR2(Y),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR2(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	RANDN	30
    // asm: 	FLOAT	R0
    // asm: 	LDF	R0,R6
    // asm: 	ADDF	50,R6
    // asm: 	RANDN	3
    // asm: 	ADDI	1,R0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	R0,R6
CARFORWARDLP:
    // asm: 	CALL	CLR_VECTORA
    // asm: 	LDF	R6,R0
    // asm: 	MPYF	@NFRAMES,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR2(Y),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR2(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	SLEEP	1
    // asm: 	BU	CARFORWARDLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARFORWARD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void ROAD_DEBRIS_CREATE_55GAL(void)
{
    // asm: 	PUSH	R0
    // asm: 	CALL	ADD_RDDEBRIS
    // asm: 	LDI	0731h,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROAD_DEBRIS_CREATE_55GAL", 0, 0);
    UNIMPL();
}

void ROAD_DEBRIS_CREATE(void)
{
    // asm: 	BU	ADD_RDDEBRIS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROAD_DEBRIS_CREATE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SMOKE_STACK(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	BUD	MAKEPPP
    // asm: 	PUSH	R2
    // asm: 	NOP
    // asm: 	LDI	@SMOKE_ANII,AR6
    // 	;---->	BUD	MAKEPPP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOKE_STACK", 0, 0);
    UNIMPL();
}

void CAR_FIRE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	BUD	MAKEPPP
    // asm: 	PUSH	R2
    // asm: 	NOP
    // asm: 	LDI	@CAR_FIRE_ANII,AR6
    // 	;---->	BUD	MAKEPPP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAR_FIRE", 0, 0);
    UNIMPL();
}

void DC_MINIFOUNTAIN(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	R2
    // asm: 	LDI	@DC_MINIFOUNTAIN_ANII,AR6
    // asm: 	CREATE	WATERANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	BC	FWL1A
    // asm: 	BU	J262
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DC_MINIFOUNTAIN", 0, 0);
    UNIMPL();
}

void DC_FOUNTAIN(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	R2
    // asm: 	LDI	@DC_FOUNTAIN_ANII,AR6
    // asm: 	CREATE	WATERANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	BC	FWL1A
    // asm: 	BU	J262
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DC_FOUNTAIN", 0, 0);
    UNIMPL();
}

void WATERFALL(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	R2
    // asm: 	CREATE	WATERFALL_SND,SPAWNER_C|ANIMATION_T
    // asm: 	LDI	@WATERFALL_ANII,AR6
    // asm: 	CREATE	WATERANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	BC	FWL1A
J262:
    // asm: STI	AR0,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
FWL1A:
    // asm: POP	R2
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERFALL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*AR6	START OF SCRIPT
*
*the big difference is that this *MUST* handle the sound animation also...
*
*
 */
void WATERANI_PROC(void)
{
    // asm: 	LDI	AR6,AR5
WATERANI_LP:
    // asm: 	LDI	*AR5++,R0
    // asm: 	BLT	WATERANI_PROC
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	RANDN	4
    // asm: 	LDI	R0,R0
    // asm: 	LDIZ	1,AR2
    // asm: 	LDINZ	2,AR2
    // asm: 	CALL	PRC_SLEEP
    // asm: 	BU	WATERANI_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERANI_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*AR4=WATERFALL OBJECT
*
 */
void WATERFALL_SND(void)
{
    // asm: 	SLEEP	1
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	LDFN	4.0,R1
    // asm: 	LDFNN	1.0,R1
    // asm: 	CMPI	-20000,R0
    // asm: 	BGT	WF1
    // asm: 	LDI	@CAMVIEW,R2
    // asm: 	BNE	WF0
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDI   	*+AR0(CAR_SPIN),R2
    // asm: 	BNE	WF1
WF0:
    // asm: 	LDI	WATERFALLSND,AR2 	;KILL OFF SOUND
    // asm: 	CALL	KILLSNDFX
    // asm: 	DIE
WF1:
    // asm: 	FLOAT	R0,R2
    // asm: 	MPYF	R1,R2
    // asm: 	ABSF	R2
    // asm: 	MPYF	0.01,R2
    // asm: 	MPYF	0.1,R2 			;0-50
    // asm: 	SUBRF	75,R2
    // asm: 	BN	WATERFALL_SND
    // asm: 	MPYF	7,R2
    // asm: 	FIX	R2
    // asm: 	CMPI	255,R2
    // asm: 	LDIGT	255,R2
    // asm: 	LDI	WATERFALLSND,AR2
    // asm: 	CALL	AMBIENCE_SOUND
    // asm: 	BU	WATERFALL_SND
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERFALL_SND", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	AR2	SOUND
*	R2	VOLUME (0-255)
*
 */
void AMBIENCE_SOUND(void)
{
    // asm: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm: 	BEQ	IS_T1
    // asm: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm: 	BEQ	IS_T2
    // asm: 	CALL	ONESNDFX
    // asm: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm: 	BNE	NOT_T1
IS_T1:
    // asm: 	LDI	R2,R1				;volume
    // asm: 	LDI	1,R0
    // asm: 	CALL	SET_TRACK_VOL
    // asm: 	BU	HEND
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AMBIENCE_SOUND", 0, 0);
    UNIMPL();
}

void NOT_T1(void)
{
    // asm: CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm: 	BNE	NOT_T2
IS_T2:
    // asm: 	LDI	R2,R1
    // asm: 	LDI	2,R0
    // asm: 	CALL	SET_TRACK_VOL
NOT_T2:
HEND:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOT_T1", 0, 0);
    UNIMPL();
}

void HUNGH_ANI(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR2
    // asm: 	READAUD	ADJ_GIRLS
    // asm: 	POP	AR2
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	DORUT_ANI
    // asm: 	LDI	@HUNGH_ANISI,AR6
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // ;	ANDN	O_1PAL,R0
    // asm: 	OR	O_POSTER,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	5,AR2
    // asm: 	CALL	RANDU0
    // asm: 	CMPI	4,R0
    // asm: 	BEQ	HUNGH_NOPAL
    // asm: 	MPYI	2,R0
    // asm: 	ADDI	@BABE_PALISTI,R0
    // asm: 	LDI	R0,AR5
    // asm: 	LDI	*+AR5,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	BC	HUNGH_NOPAL
    // asm: 	LDI	R0,R2
    // asm: 	LDI	*AR5,AR2
    // asm: 	LDI	*AR2++,R3
    // asm: 	CALL	PAL_SET
    // asm: 	STI	R0,*+AR4(OPAL)
HUNGH_NOPAL:
    // asm: 	CALL	PLACE_ON_ROAD
    // asm: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_BABE_S,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	AR6,AR5
    // asm: 	CREATE	PLAINANI_LP_SLOW,SPAWNER_C|ANIMATION_T|7
    // asm: 	BC	FWL1
    // asm: 	BU	J2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HUNGH_ANI", 0, 0);
    UNIMPL();
}

void HUNGH_ANI_REENTER(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	R2
    // asm: 	LDI	@HUNGH_ANISI,AR6
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // ;	ANDN	O_1PAL,R0
    // asm: 	OR	O_POSTER,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	AR6,AR5
    // asm: 	CREATE	PLAINANI_LP_SLOW,SPAWNER_C|ANIMATION_T|7
    // asm: 	BC	FWL1
    // asm: 	BU	J2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HUNGH_ANI_REENTER", 0, 0);
    UNIMPL();
}

void PLACE_ON_ROAD(void)
{
    // asm: 	CALL	ADD_RDDEBRIS
    // asm: 	PUSH	AR4
    // asm: 	ADDI	OPOSX,AR4
    // asm: 	CALL	CAMSCAN
    // asm: 	POP	AR4
    // asm: 	FIX	R0,R1		;Check for dude in the sign
    // asm: 	CMPI	1000,R1
    // asm: 	BGT	PORX
    // asm: 	LDF	*+AR4(OPOSY),R1
    // asm: 	ADDF	R1,R0
    // asm: 	SUBF   	45,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
PORX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLACE_ON_ROAD", 0, 0);
    UNIMPL();
}

void RUT_ANI(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	R2
DORUT_ANI:
    // asm: 	LDI	@RUT_ANISI,AR6
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	OR	O_POSTER,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	CALL	PLACE_ON_ROAD
    // asm: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_DUDE_S,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	AR6,AR5
    // asm: 	CREATE	PLAINANI_LP_SLOW,SPAWNER_C|ANIMATION_T|7
    // asm: 	BC	FWL1
    // asm: 	BU	J2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUT_ANI", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void PLAINANI_PROC_SLOW(void)
{
    // asm: 	LDI	AR6,AR5
PLAINANI_LP_SLOW:
    // asm: 	LDI	*AR5++,R0
    // asm: 	BLT	PLAINANI_PROC_SLOW
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	RANDN	4
    // asm: 	LDI	R0,R0
    // asm: 	LDIZ	1,AR2
    // asm: 	LDINZ	2,AR2
    // asm: 	ADDI	1,AR2
    // asm: 	CALL	PRC_SLEEP
    // asm: 	BU	PLAINANI_LP_SLOW
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLAINANI_PROC_SLOW", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FLAGWAVE_TALL(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	R2
    // asm: 	LDI	@FLAGANITALLI,AR6
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	RANDN	7
    // asm: 	LDI	AR6,AR5
    // asm: 	ADDI	R0,AR5
    // asm: 	CREATE	PLAINANI_LP,SPAWNER_C|ANIMATION_T
    // asm: 	BC	FWL1
    // asm: 	BU	J2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLAGWAVE_TALL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FLAGWAVE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	R2
    // asm: 	LDI	@FLAGANII,AR6
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	RANDN	7
    // asm: 	LDI	AR6,AR5
    // asm: 	ADDI	R0,AR5
    // asm: 	CREATE	PLAINANI_LP,SPAWNER_C|ANIMATION_T
    // asm: 	BC	FWL1
    // asm: 	BU	J2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLAGWAVE", 0, 0);
    UNIMPL();
}

void MAKEPPP(void)
{
    // asm: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	BC	FWL1
J2:
    // asm: STI	AR0,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
FWL1:
    // asm: POP	R2
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKEPPP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*AR6	START OF SCRIPT
*
 */
void PLAINANI_PROC(void)
{
    // asm: 	LDI	AR6,AR5
PLAINANI_LP:
    // asm: 	LDI	*AR5++,R0
    // asm: 	BLT	PLAINANI_PROC
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	RANDN	4
    // asm: 	LDI	R0,R0
    // asm: 	LDIZ	1,AR2
    // asm: 	LDINZ	2,AR2
    // asm: 	CALL	PRC_SLEEP
    // asm: 	BU	PLAINANI_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLAINANI_PROC", 0, 0);
    UNIMPL();
}

void LOAD_SINGLE_SECTION(void)
{
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	LDL	SINGLE_SECTION_TEMPPTR,AR6
    // asm: 	SUBI	OLINK2,AR6
    // asm: 	LDI	AR2,AR5
    // asm: 	INC	AR5
    // asm: 	LDI	*AR5++,R4		;get number of objects to load
    // asm: 	SLOCKON	LE,"BACKGRND\LOAD_SINGLE_SECTION GROUP ERROR"
    // asm: 	DEC	R4
    // asm: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\LOAD_SINGLE_SECTION OUT OF OBJECTS"
LS_L12:
    // asm: 	LDI	*AR5++,AR2		;GET MODEL PTR
    // asm: 	CALL	OBJ_GETE
    // asm: 	SLOCKON	C,"BACKGRND\LOAD_SINGLE_SECTION OUT OF OBJECTS *FATAL*"
    // asm: 	BC	LS_ACTIVATE_X
    // asm: 	LDI	AR0,AR4
    // asm: 	STI	AR4,*+AR6(OLINK2)
    // asm: 	LDI	AR4,AR6
    // asm: 	FLOAT	*AR5++,R1		;GET X POSITION
    // asm: 	STF	R1,*+AR4(OPOSX)
    // asm: 	FLOAT	*AR5++,R1		;GET Y POSITION
    // asm: 	STF	R1,*+AR4(OPOSY)
    // asm: 	FLOAT	*AR5++,R1		;GET Z POSITION
    // asm: 	STF	R1,*+AR4(OPOSZ)
    // asm: 	LDF	*AR5++,R2		;SET THE RADIANS FOR THE OBJECT
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	HPFIND_YMATRIX
    // asm: 	LDI	*AR5++,R1		;LOAD OBJECT ID (GENV STYLE)
    // asm: 	LDI	R1,R2
    // asm: 	AND	CLASS_M|TYPE_M|SUBTYPE_M,R1
    // asm: 	STI	R1,*+AR4(OID)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT			;INSERT THE BABE
    // asm: 	SUBI	1,R4
    // asm: 	BGE	LS_L12
LS_ACTIVATE_X:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR6(OLINK2)
    // asm: 	LDI	@SINGLE_SECTION_TEMPPTR,R0
    // asm: 	POP	AR7
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SINGLE_SECTION", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void OHARE_PLANE(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	SONDFX	JETFLYBY
    // asm: 	CREATE	PLANE_FWRD,UTIL_C
    // asm: 	STI	AR0,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	POP	R2
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OHARE_PLANE", 0, 0);
    UNIMPL();
}

void PLANE_FWRD(void)
{
    // asm: 	FLOAT	250,R7
    // asm: 	FLOAT	32000,R6
    // asm: 	MPYF	4,R6
    // *
    // *
PLANE_FWL:
    // asm: 	LDF	*+AR4(ORADY),R2
    // asm: 	ADDF	HALFPI,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CALL	CLR_VECTORA
    // asm: 	LDF	R7,R0
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR2(Y),R0
    // asm: 	ADDF	50,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR2(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	ABSF	R7,R0
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	SUBF	R0,R6
    // asm: 	BLT	TRAINX
    // asm: 	SLEEP	1
    // asm: 	BU	PLANE_FWL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLANE_FWRD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TRAIN_FWRD_MAKEB(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	CREATE	TRAIN_FWRDB,UTIL_C
    // asm: 	STI	AR0,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	BU	J87
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAIN_FWRD_MAKEB", 0, 0);
    UNIMPL();
}

void TRAIN_FWRD_MAKE(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	CREATE	TRAIN_FWRD,UTIL_C
    // asm: 	STI	AR0,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
J87:
    // asm: POP	R2
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAIN_FWRD_MAKE", 0, 0);
    UNIMPL();
}

void TRAIN_FWRDB(void)
{
    // asm: 	LDI	*+AR4(OUSR1),R0
    // asm: 	ADDI	5,R0			;kludge to delete later than loaded
    // asm: 	STI	R0,*+AR4(OUSR1)
    // asm: 	FLOAT	-150,R7
    // asm: 	FLOAT	32000,R6
    // asm: 	MPYF	8,R6
    // asm: 	BU	J765
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAIN_FWRDB", 0, 0);
    UNIMPL();
}

void TRAIN_FWRD(void)
{
    // asm: 	FLOAT	150,R7
    // asm: 	FLOAT	32000,R6
    // asm: 	MPYF	3,R6
J765:
    // *
    // *
TRAIN_FWL:
    // asm: 	CALL	CLR_VECTORA
    // asm: 	LDF	R7,R0
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR2(Y),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR2(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	ABSF	R7,R0
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	SUBF	R0,R6
    // asm: 	BLT	TRAINX
    // asm: 	SLEEP	1
    // asm: 	BU	TRAIN_FWL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAIN_FWRD", 0, 0);
    UNIMPL();
}

void TRAINX(void)
{
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	LDI	*+AR4(OFLAGS),R1
    // asm: 	ANDN	R0,R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAINX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	AR2	SECTION POINTER
*
*	OFFSET BY START_POS
*
*
 */
void LOAD_SINGLE_SECTION_OFFSET(void)
{
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	LDL	SINGLE_SECTION_TEMPPTR,AR6
    // asm: 	SUBI	OLINK2,AR6
    // asm: 	LDI	AR2,AR5
    // asm: 	INC	AR5
    // asm: 	LDI	*AR5++,R4		;get number of objects to load
    // asm: 	SLOCKON	LE,"BACKGRND\LOAD_SINGLE_SECTION GROUP ERROR"
    // asm: 	DEC	R4
    // asm: 	LDF	@START_RADY,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	HPFIND_YMATRIX
    // asm: 	CMPI	@OFREECNT,R4
    // asm: 	SLOCKON	GT,"BACKGRND\LOAD_SINGLE_SECTION OUT OF OBJECTS"
LS_L12O:
    // asm: 	LDI	*AR5++,AR2		;GET MODEL PTR
    // asm: 	CALL	OBJ_GETE
    // asm: 	SLOCKON	C,"BACKGRND\LOAD_SINGLE_SECTION OUT OF OBJECTS *FATAL*"
    // asm: 	BC	LS_ACTIVATE_XO
    // asm: 	LDI	AR0,AR4
    // asm: 	STI	AR4,*+AR6(OLINK2)
    // asm: 	LDI	AR4,AR6
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	FLOAT	*AR5++,R1		;GET X POSITION
    // asm: 	STF	R1,*+AR2(X)
    // asm: 	FLOAT	*AR5++,R1		;GET Y POSITION
    // asm: 	STF	R1,*+AR2(Y)
    // asm: 	FLOAT	*AR5++,R1		;GET Z POSITION
    // asm: 	STF	R1,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R1
    // asm: 	ADDF	@START_POS+X,R1
    // asm: 	STF	R1,*+AR4(OPOSX)
    // asm: 	LDF	*+AR2(Y),R1
    // asm: 	ADDF	@START_POS+Y,R1
    // asm: 	STF	R1,*+AR4(OPOSY)
    // asm: 	LDF	*+AR2(Z),R1
    // asm: 	ADDF	@START_POS+Z,R1
    // asm: 	STF	R1,*+AR4(OPOSZ)
    // asm: 	LDF	*AR5++,R2		;GET THE RADIANS FOR THE OBJECT
    // asm: 	ADDF	@START_RADY,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	HPFIND_YMATRIX
    // asm: 	LDI	*AR5++,R1		;GET OBJECT ID (GENV STYLE)
    // asm: 	LDI	R1,R2
    // asm: 	AND	CLASS_M|TYPE_M|SUBTYPE_M,R1
    // asm: 	STI	R1,*+AR4(OID)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT			;INSERT THE BABE
    // asm: 	SUBI	1,R4
    // asm: 	BGE	LS_L12O
LS_ACTIVATE_XO:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR6(OLINK2)
    // asm: 	LDI	@SINGLE_SECTION_TEMPPTR,R0
    // asm: 	POP	AR7
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SINGLE_SECTION_OFFSET", 0, 0);
    UNIMPL();
}

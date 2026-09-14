
#include "bonus.h"
#include "../core/machine.h"
#include "cmos.h"
#include "comm.h"
#include "delta.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "objects.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/BONUS.ASM
 */

static void BONUS_SANFRAN(void);
#define BONUS_GGATE BONUS_BEVHILLS
static void BONUS_BEVHILLS(void);
static void BONUSNULL(void);
void BONUS14(void);
void BONUS13(void);
void BONUS12(void);
void BONUS11(void);
void BONUS10(void);
void BONUS9(void);
void BONUS8(void);
void BONUS7(void);
void BONUS6(void);
void BONUS5(void);
void BONUS4(void);
void BONUS3(void);
void BONUS2(void);
void BONUS1(void);
static void JSUB(void);
static void DISPLAY_H2H_WINNER(PROC* p);
static void OBJ_DELETE_HIGH_PRIORITY(OBJ* obj);
static void BONUS_SCREEN(PROC* p);
static void BONSCRN2(PROC* p);
static void CLINTON_SHOW(PROC* p);
void BLINK_FREEBE(PROC* p);
static void BACKUP_CAMERA(PROC* p);
void TIMED_OUT(void);
static void KILL_PLYR_SOUNDS(void);
static void FREE_RACE_ANNOUNCE(PROC* p);
static void SHOWLEG_PROC(PROC* p);
static void SHOWNEXTLEG_PROC(PROC* p);
static void PLACE_FLAG(int index);
static void PLACE_FLAG_PROC(PROC* p);
static void BONS_MAXMPH(PROC* p);
static void BONS_RECORDTIME(PROC* p);
static void BONS_TEXT_ANIMATE(PROC* p);
static void BONS_HOTTIME_REC(PROC* p);
static void BONS_HOTTIME(PROC* p);
static void BONS_HOTTIME_RUN(PROC* p);
static void TEXTTOG(int* toggle, tTEXT* front, tTEXT* shadow, const char* string);
static void BONS_ETIME(PROC* p);
static void BONS_POSITION(PROC* p);
static void DRAW_BONS_POSITION(int position_index, int ypos);
void KILL_THEM(void);
static void FIND_AND_REACTIVATE(void);
static void KILL_THE_REANIMATORS(void);

#define BONUS_GGATE BONUS_BEVHILLS
#define LEG_NAMESI LEG_NAMES
#define GAMETRAKI GAMETRAK
#define BUFFERSI BIGBUFFER
#define BONUS_TABLEI BONUS_TABLE
#define CONGRAT_SPEECHI CONGRAT_SPEECH
#define LCTSI LCTS
#define LNLSI LNLS
#define FLAG_POS_TABLEI FLAG_POS_TABLE

#define OHIGH_PRIORITYI OHIGH_PRIORITY
#define BABE_PALISTI BABE_PALIST
void MOTION_SCREWED(PROC* p);
void CLEAR_MAP_PALS(void);
void VANITY_SUB(void);
extern uintptr_t ISOFF;
void RUT_ANI(OBJ* obj /*AR4*/);
void HUNGH_ANI_REENTER(OBJ* obj /*AR4*/);

// static uintptr_t BONUS_POSTLAUNCH[];
static int CONGRAT_SPEECH[4];
static int FLAG_POS_TABLE[28];
static const char NULLSTR5[];
static void BONUS_LK(int next_startup, int bonus_wave);

/* asm: MAXMPH	.bss	MAXMPH,1 */
c3x_f32_t MAXMPH;
/* asm: CHALLENGE_RACE	.bss	CHALLENGE_RACE,1 */
int CHALLENGE_RACE;
/* asm: NEXT_STARTUP	.bss	NEXT_STARTUP,1 */
int NEXT_STARTUP;
/* asm: BONUS_WAVE	.bss	BONUS_WAVE,1 */
int BONUS_WAVE;
/* asm: FINISH_LINE	.bss	FINISH_LINE,1 */
int FINISH_LINE;
/* asm: DO_FOLDFLAG	.bss	DO_FOLDFLAG,1 */
int DO_FOLDFLAG;
/* asm: UNFOLDFLAG	.bss	UNFOLDFLAG,1 */
int UNFOLDFLAG;
/* asm: WAS_HEAD2HEAD_ON	.bss	WAS_HEAD2HEAD_ON,1 */
int WAS_HEAD2HEAD_ON;
#define NUM_LEGS 14

static const char LEG1[] = "GOLDEN GATE PARK";
static const char LEG2[] = "SAN FRANCISCO";
static const char LEG3[] = "US 101";
static const char LEG4[] = "REDWOOD FOREST";
static const char LEG5[] = "BEVERLY HILLS";
static const char LEG6[] = "LA FREEWAY";
static const char LEG7[] = "DEATH VALLEY";
static const char LEG8[] = "ARIZONA";
static const char LEG9[] = "GRAND CANYON";
static const char LEG10[] = "IOWA";
static const char LEG11[] = "CHICAGO";
static const char LEG12[] = "INDIANA";
static const char LEG13[] = "APPALACHIA";
static const char LEG14[] = "WASHINGTON DC";
static const char LEG_USA[] = "SF TO WASHINGTON DC";
static const char WINT1[] = "FREE GAME FOR";
static const char WINT2[] = "1ST PLACE";
static const char EXPIRED[] = "EXPIRED";
static const char BHDDFAS[] = "WINNER";

/* asm: LEG_NAMES	.word	LEG1,LEG2,LEG3,LEG4,LEG5,LEG6,LEG7,LEG8 */
/* asm: 	.word	LEG9,LEG10,LEG11,LEG12,LEG13,LEG14,LEG_USA */
/* asm: 	 */
/* asm: 	 */
const char* LEG_NAMES[] = {
    LEG1,
    LEG2,
    LEG3,
    LEG4,
    LEG5,
    LEG6,
    LEG7,
    LEG8,
    LEG9,
    LEG10,
    LEG11,
    LEG12,
    LEG13,
    LEG14,
    LEG_USA,
};
/*
 *STATISTICAL STRUCTURE FOR THE GAME LEGS <NERD DATA>
 *
 *
 *
 */

int GAMETRAK[NUM_LEGS * sizeof(tagGAMETRAK)];
/* asm: ETIME	.bss	ETIME,1 */
int ETIME;
// ;etime,#,maxmph
/* asm: BUFFERSI	.word	BIGBUFFER */
#define BUFFERSI BIGBUFFER
/* asm: BIGBUFFER	.bss	BIGBUFFER,(4+1+2+2)*NUM_LEGS */
static int BIGBUFFER[(4 + 1 + 2 + 2) * NUM_LEGS];
/* asm: BONUS_POSTLAUNCH */
/* asm: 	.word	BONUS_GGATE,BONUS_SANFRAN,BONUSNULL,BONUSNULL */
/* asm: 	.word	BONUS_BEVHILLS,BONUSNULL,BONUSNULL */
/* asm: 	.word	BONUSNULL,BONUSNULL,BONUSNULL */
/* asm: 	.word	BONUSNULL,BONUSNULL,BONUSNULL */
/* asm: 	.word	BONUSNULL,BONUSNULL,BONUSNULL */
void_func_ptr BONUS_POSTLAUNCH[] = {
    BONUS_GGATE,
    BONUS_SANFRAN,
    BONUSNULL,
    BONUSNULL,
    BONUS_BEVHILLS,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
    BONUSNULL,
};

// *----------------------------------------------------------------------------
static void BONUS_SANFRAN(void) {
    // asm 00003966: 	CREATE	TRAFFIC_LIGHT,SPAWNER_C|COLORCYC_T
    PROC_CONTEXT* ctx = NEW_PROC_CONTEXT();
    CREATE(TRAFFIC_LIGHT, SPAWNER_C | COLORCYC_T, ctx);
    // asm 00003969: 	FLOAT	-35,R0
    // asm 0000396A: 	STF	R0,@INFIN_CORRECT
    INFIN_CORRECT = C3X_FROM_INT(-35);

    // fallthrough into BONUS_BEVHILLS
    BONUS_BEVHILLS();
}

static void BONUS_BEVHILLS(void) {
    // asm 0000396B: 	LDI	@_MODE,R0
    // asm 0000396C: 	OR	MWATER,R0
    // asm 0000396D: 	STI	R0,@_MODE
    _MODE |= MWATER;
}

static void BONUSNULL(void) {
}

// *----------------------------------------------------------------------------

/* asm: BONUS_TABLE	.word	BONUS1,BONUS2,BONUS3,BONUS4,BONUS5 */
/* asm: 	.word	BONUS6,BONUS7,BONUS8,BONUS9,BONUS10 */
/* asm: 	.word	BONUS11,BONUS12,BONUS13,BONUS14 */
static void (*BONUS_TABLE[])(void) = {
    BONUS1,
    BONUS2,
    BONUS3,
    BONUS4,
    BONUS5,
    BONUS6,
    BONUS7,
    BONUS8,
    BONUS9,
    BONUS10,
    BONUS11,
    BONUS12,
    BONUS13,
    BONUS14,
};

/*
 *----------------------------------------------------------------------------
 *ROUTINES FOR OVERLAY.ASM
 *
 *
 */
void BONUS14(void) {
    // asm 00003970: 	LDI	14,R1
    // asm 00003971: 	BU	LK
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS14", 0, 0);
    UNIMPL();
}

void BONUS13(void) {
    // asm 00003972: LDI	L_LEG14_BEGIN+1,R0
    // asm 00003973: 	LDI	13,R1
    // asm 00003974: 	BU	LK
    BONUS_LK(L_LEG14_BEGIN + 1, 13);
}

void BONUS12(void) {
    // asm 00003975: LDI	L_LEG13_BEGIN+1,R0
    // asm 00003976: 	LDI	12,R1
    // asm 00003977: 	BU	LK
    BONUS_LK(L_LEG13_BEGIN + 1, 12);
}

void BONUS11(void) {
    // asm 00003978: LDI	L_LEG12_BEGIN+1,R0
    // asm 00003979: 	LDI	11,R1
    // asm 0000397A: 	BU	LK
    BONUS_LK(L_LEG12_BEGIN + 1, 11);
}

void BONUS10(void) {
    // asm 0000397B: LDI	L_LEG11_BEGIN+1,R0
    // asm 0000397C: 	LDI	10,R1
    // asm 0000397D: 	BU	LK
    BONUS_LK(L_LEG11_BEGIN + 1, 10);
}

void BONUS9(void) {
    // asm 0000397E: LDI	L_LEG10_BEGIN+1,R0
    // asm 0000397F: 	LDI	9,R1
    // asm 00003980: 	BU	LK
    BONUS_LK(L_LEG10_BEGIN + 1, 9);
}

void BONUS8(void) {
    // asm 00003981: LDI	L_LEG9_BEGIN+1,R0
    // asm 00003982: 	LDI	8,R1
    // asm 00003983: 	BU	LK
    BONUS_LK(L_LEG9_BEGIN + 1, 8);
}

void BONUS7(void) {
    // asm 00003984: LDI	L_LEG8_BEGIN+1,R0
    // asm 00003985: 	LDI	7,R1
    // asm 00003986: 	BU	LK
    BONUS_LK(L_LEG8_BEGIN + 1, 7);
}

void BONUS6(void) {
    // asm 00003987: LDI	L_LEG7_BEGIN+1,R0
    // asm 00003988: 	LDI	6,R1
    // asm 00003989: 	BU	LK
    BONUS_LK(L_LEG7_BEGIN + 1, 6);
}

void BONUS5(void) {
    // asm 0000398A: LDI	L_LEG6_BEGIN+1,R0
    // asm 0000398B: 	LDI	5,R1
    // asm 0000398C: 	BU	LK
    BONUS_LK(L_LEG6_BEGIN + 1, 5);
}

void BONUS4(void) {
    // asm 0000398D: LDI	L_LEG5_BEGIN+1,R0
    // asm 0000398E: 	LDI	4,R1
    // asm 0000398F: 	BU	LK
    BONUS_LK(L_LEG5_BEGIN + 1, 4);
}

void BONUS3(void) {
    // asm 00003990: LDI	L_LEG4_BEGIN+1,R0
    // asm 00003991: 	LDI	3,R1
    // asm 00003992: 	BU	LK
    BONUS_LK(L_LEG4_BEGIN + 1, 3);
}

void BONUS2(void) {
    // asm 00003993: LDI	L_LEG3_BEGIN+1,R0
    // asm 00003994: 	LDI	2,R1
    // asm 00003995: 	BU	LK
    BONUS_LK(L_LEG3_BEGIN + 1, 2);
}

void BONUS1(void) {
    // asm 00003996: LDI	L_LEG2_BEGIN+1,R0
    // asm 00003997: 	LDI	1,R1
    BONUS_LK(L_LEG2_BEGIN + 1, 1);
}

static void BONUS_LK(int next_startup, int bonus_wave) {
LK:
    // asm 00003998: STI	R0,@NEXT_STARTUP
    NEXT_STARTUP = next_startup;
    // asm 00003999: 	STI	R1,@BONUS_WAVE
    BONUS_WAVE = bonus_wave;
    // asm 0000399A: 	LDI	@_MODE,R0
    // asm 0000399B: 	LDI	R0,R1
    // asm 0000399C: 	AND	MMODE,R1
    // asm 0000399D: 	CMPI	MGAME,R1
    // asm 0000399E: 	RETSNE
    if ((_MODE & MMODE) != MGAME) {
        return;
    }
    // asm 0000399F: 	ANDN	MMODE,R0
    // asm 000039A0: 	OR	MBONUS,R0
    // asm 000039A1: 	ANDN	MINTUNNEL,R0
    // asm 000039A2: 	STI	R0,@_MODE
    _MODE = ((_MODE & ~MMODE) | MBONUS) & ~MINTUNNEL;
    // 	;;;	COMMUNICATIONS ALCHEMY
    // 	;;;
    // asm 000039A3: 	LDI	@MY_STATE,R0
    // asm 000039A4: 	OR	OMS_FINISHLINE,R0
    // asm 000039A5: 	STI	R0,@MY_STATE
    MY_STATE |= OMS_FINISHLINE;
    // 	;DIFFICULTY ADJUSTMENTS
    // 	;
    // asm 000039A6: 	LDI	@POSITION,R0
    // asm 000039A7: 	CALL	DIFF_CHANGE
    DIFF_CHANGE(POSITION);
    // 	;AUDIT MUMBO JUMBO
    // 	;
    // asm 000039A8: 	LDI	@BONUS_WAVE,R2
    // asm 000039A9: 	SETAUD	AUD_LAST_LEG
    SETAUD(AUD_LAST_LEG, BONUS_WAVE);
    // asm 000039AB: 	LDI	@BONUS_WAVE,AR2
    // asm 000039AC: 	DEC	AR2
    // asm 000039AD: 	MPYI	2,AR2
    // asm 000039AE: 	ADDI	AUD_FINISH_GGATE,AR2
    // asm 000039AF: 	CALL	AUDIT_INC
    AUDIT_INC(AUD_FINISH_GGATE + (BONUS_WAVE - 1) * 2);
    // asm 000039B0: 	CLRI	R0
    // asm 000039B1: 	STI	R0,@FIRST_RACE
    FIRST_RACE = 0;
    // asm 000039B2: 	CALL	KILL_PLYR_SOUNDS
    KILL_PLYR_SOUNDS();
    // asm 000039B3: 	LDI	MAX_DRONES,R0
    // asm 000039B4: 	STI	R0,@DD_MAX_DRONES
    DD_MAX_DRONES = MAX_DRONES;
    // asm 000039B5: 	READAUD	ADJ_CHECKPOINT_BONUS
    // asm 000039B7: 	STI	R0,@CHECKPOINT_TIME_BONUS
    CHECKPOINT_TIME_BONUS = READAUD(ADJ_CHECKPOINT_BONUS);
    // asm 000039B8: 	LDI	@PLYCAR,AR4
    // asm 000039B9: 	LDI	@PLYCBLK,AR5
    // asm 000039BA: 	CALL	FIND_PLAYERS_POSITION
    FIND_PLAYERS_POSITION(PLYCAR, PLYCBLK);
    // asm 000039BB: 	CREATEC	BONUS_SCREEN,22
    CREATEC(CURRENT_PROC, BONUS_SCREEN, 22, NULL);
    // asm 000039BE: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

static void JSUB(void) {
    tSHADOW_TEXT text;
    int palette;
    // asm 000039BF: 	LDL	BHDDFAS,AR2
    // asm 000039C0: 	FLOAT	256,R2
    // asm 000039C1: 	FLOAT	278,R3
    // asm 000039C2: 	LDI	7,RC
    // asm 000039C3: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(BHDDFAS, C3X_FROM_INT(256), C3X_FROM_INT(278), 7);
    // asm 000039C4: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 000039C5: 	OR	TXT_CENTER,R0
    // asm 000039C6: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 000039C7: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 000039C8: 	OR	TXT_CENTER,R0
    // asm 000039C9: 	STI	R0,*+AR1(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    text.shadow->color |= TXT_CENTER;
    // asm 000039CA: 	LDL	font18_white,AR2
    // asm 000039CB: 	CALL	PAL_FIND_RAW
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(font18_white_ROM));
    // asm 000039CC: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000039CD: 	STI	R0,*+AR1(TEXT_PAL)
    text.front->palette = (u32)palette;
    text.shadow->palette = (u32)palette;
    // asm 000039CE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JSUB", 0, 0);
}

static void DISPLAY_H2H_WINNER(PROC* p) {
    OBJ* obj;
    int flags;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    }
    // asm 000039CF: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm 000039D0: 	BZ	DODIE
    if (WAS_HEAD2HEAD_ON == 0)
        goto DODIE;
    // asm 000039D1: 	LDI	@OM_POSITION,R0
    // asm 000039D2: 	CMPI	@POSITION,R0
    // asm 000039D3: 	BLT	DODIE
    if (OM_POSITION < POSITION)
        goto DODIE;
    // asm 000039D4: 	CLRI	R0
    // asm 000039D5: 	STI	R0,@UNFOLDFLAG
    UNFOLDFLAG = 0;
    // asm 000039D6: 	LDL	big2,AR2
    // asm 000039D7: 	LDI	0,R2
    // asm 000039D8: 	LDI	140,R3
    // asm 000039D9: 	LDI	856,RC
    // asm 000039DA: 	CALL	OBJ_QMAKE
    flags = O_IROT | O_NOROT | O_NOUROT | O_NOUNIV | O_1PAL;
    obj = OBJ_QMAKE(ROM_PTR(big2_ROM), 0, 140, 856);
    // asm 000039DB: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 000039DC: 	OR	*+AR0(OFLAGS),R0
    // asm 000039DD: 	STI	R0,*+AR0(OFLAGS)
    obj->flags |= (u32)flags;
    // asm 000039DE: 	LDI	AR0,AR2
    // asm 000039DF: 	CALL	OBJ_INSERTHP
    OBJ_INSERTHP(obj);
    // asm 000039E0: 	LDL	H2HPAL3,AR2
    // asm 000039E1: 	CALL	PAL_FIND_RAW
    // asm 000039E2: 	STI	R0,*+AR0(OPAL)
    obj->palette = (u32)PAL_FIND_RAW((tPAL*)ROM_PTR(H2HPAL3_ROM));
    // asm 000039E3: 	STI	AR0,*+AR7(PDATA+2)	;big2
    p->ctx.DISPLAY_H2H_WINNER.objects[2] = obj; // ;big2
    // asm 000039E4: 	LDL	redhd1,AR2
    // asm 000039E5: 	LDI	0,R2
    // asm 000039E6: 	LDI	80,R3
    // asm 000039E7: 	LDI	868,RC
    // asm 000039E8: 	CALL	OBJ_QMAKE
    obj = OBJ_QMAKE(ROM_PTR(redhd1_ROM), 0, 80, 868);
    // asm 000039E9: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 000039EA: 	OR	*+AR0(OFLAGS),R0
    // asm 000039EB: 	STI	R0,*+AR0(OFLAGS)
    obj->flags |= (u32)flags;
    // asm 000039EC: 	LDI	AR0,AR2
    // asm 000039ED: 	CALL	OBJ_INSERTHP
    OBJ_INSERTHP(obj);
    // asm 000039EE: 	LDL	H2HPAL2,AR2
    // asm 000039EF: 	CALL	PAL_FIND_RAW
    // asm 000039F0: 	STI	R0,*+AR0(OPAL)
    obj->palette = (u32)PAL_FIND_RAW((tPAL*)ROM_PTR(H2HPAL2_ROM));
    // asm 000039F1: 	STI	AR0,*+AR7(PDATA)	;red (bottom)
    p->ctx.DISPLAY_H2H_WINNER.objects[0] = obj; // ;red (bottom)
    // asm 000039F2: 	LDL	yelhd1,AR2
    // asm 000039F3: 	LDI	0,R2
    // asm 000039F4: 	LDI	200,R3
    // asm 000039F5: 	LDI	868,RC
    // asm 000039F6: 	CALL	OBJ_QMAKE
    obj = OBJ_QMAKE(ROM_PTR(yelhd1_ROM), 0, 200, 868);
    // asm 000039F7: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 000039F8: 	OR	*+AR0(OFLAGS),R0
    // asm 000039F9: 	STI	R0,*+AR0(OFLAGS)
    obj->flags |= (u32)flags;
    // asm 000039FA: 	LDI	AR0,AR2
    // asm 000039FB: 	CALL	OBJ_INSERTHP
    OBJ_INSERTHP(obj);
    // asm 000039FC: 	LDL	H2HPAL2,AR2
    // asm 000039FD: 	CALL	PAL_FIND_RAW
    // asm 000039FE: 	STI	R0,*+AR0(OPAL)
    obj->palette = (u32)PAL_FIND_RAW((tPAL*)ROM_PTR(H2HPAL2_ROM));
    // asm 000039FF: 	STI	AR0,*+AR7(PDATA+1)	;yellow (bottom)
    p->ctx.DISPLAY_H2H_WINNER.objects[1] = obj; // ;yellow (bottom)
    // asm 00003A00: DHHW_LP
DHHW_LP:
    // asm 00003A00: 	LDI	@UNFOLDFLAG,R0
    // asm 00003A01: 	BNZ	DODIEXXX
    if (UNFOLDFLAG != 0)
        goto DODIEXXX;
    // asm 00003A02: 	CALL	JSUB
    JSUB();
    // asm 00003A03: 	SLEEP	10
    SLEEP(10, 1);
    // asm 00003A05: 	CALL	JSUB
    JSUB();
    // asm 00003A06: 	SLEEP	5
    SLEEP(5, 2);
    // asm 00003A08: 	FLOAT	-5000,R6
    // asm 00003A09: 	LDI	*+AR7(PDATA),AR2
    // asm 00003A0A: 	LDF	*+AR2(OPOSZ),R0
    // asm 00003A0B: 	ADDF	R6,R0
    // asm 00003A0C: 	STF	R0,*+AR2(OPOSZ)
    // asm 00003A0D: 	LDI	*+AR7(PDATA+1),AR2
    // asm 00003A0E: 	LDF	*+AR2(OPOSZ),R0
    // asm 00003A0F: 	ADDF	R6,R0
    // asm 00003A10: 	STF	R0,*+AR2(OPOSZ)
    // asm 00003A11: 	LDI	*+AR7(PDATA+2),AR2
    // asm 00003A12: 	LDF	*+AR2(OPOSZ),R0
    // asm 00003A13: 	ADDF	R6,R0
    // asm 00003A14: 	STF	R0,*+AR2(OPOSZ)
    for (int index = 0; index < 3; index++) {
        p->ctx.DISPLAY_H2H_WINNER.objects[index]->pos.Z = C3X_STF(C3X_ADD(
            C3X_LDF(p->ctx.DISPLAY_H2H_WINNER.objects[index]->pos.Z), C3X_FROM_INT(-5000)));
    }
    // asm 00003A15: 	SLEEP	5
    SLEEP(5, 3);
    // asm 00003A17: 	FLOAT	5000,R6
    // asm 00003A18: 	LDI	*+AR7(PDATA),AR2
    // asm 00003A19: 	LDF	*+AR2(OPOSZ),R0
    // asm 00003A1A: 	ADDF	R6,R0
    // asm 00003A1B: 	STF	R0,*+AR2(OPOSZ)
    // asm 00003A1C: 	LDI	*+AR7(PDATA+1),AR2
    // asm 00003A1D: 	LDF	*+AR2(OPOSZ),R0
    // asm 00003A1E: 	ADDF	R6,R0
    // asm 00003A1F: 	STF	R0,*+AR2(OPOSZ)
    // asm 00003A20: 	LDI	*+AR7(PDATA+2),AR2
    // asm 00003A21: 	LDF	*+AR2(OPOSZ),R0
    // asm 00003A22: 	ADDF	R6,R0
    // asm 00003A23: 	STF	R0,*+AR2(OPOSZ)
    for (int index = 0; index < 3; index++) {
        p->ctx.DISPLAY_H2H_WINNER.objects[index]->pos.Z = C3X_STF(C3X_ADD(
            C3X_LDF(p->ctx.DISPLAY_H2H_WINNER.objects[index]->pos.Z), C3X_FROM_INT(5000)));
    }
    // asm 00003A24: 	DBU	AR5,DHHW_LP
    goto DHHW_LP;
DODIEXXX:
    // asm 00003A25: 	LDI	*+AR7(PDATA),AR2
    // asm 00003A26: 	CALL	OBJ_DELETE_HIGH_PRIORITY
    OBJ_DELETE_HIGH_PRIORITY(p->ctx.DISPLAY_H2H_WINNER.objects[0]);
    // asm 00003A27: 	LDI	*+AR7(PDATA+1),AR2
    // asm 00003A28: 	CALL	OBJ_DELETE_HIGH_PRIORITY
    OBJ_DELETE_HIGH_PRIORITY(p->ctx.DISPLAY_H2H_WINNER.objects[1]);
    // asm 00003A29: 	LDI	*+AR7(PDATA+2),AR2
    // asm 00003A2A: 	CALL	OBJ_DELETE_HIGH_PRIORITY
    OBJ_DELETE_HIGH_PRIORITY(p->ctx.DISPLAY_H2H_WINNER.objects[2]);
DODIE:
    // asm 00003A2B: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

static void OBJ_DELETE_HIGH_PRIORITY(OBJ* obj) {
    OBJ** link;
    // asm 00003A2C: 	PUSH	R0
    // asm 00003A2D: 	PUSH	R1
    // asm 00003A2E: 	PUSH	AR1
    // asm 00003A2F: 	PUSH	AR2
    // asm 00003A30: 	LDI	@OHIGH_PRIORITYI,R1
    link = &OHIGH_PRIORITY;
DELLP:
    // asm 00003A31: LDI	R1,AR1		;WE MUST FIND DEAD OBJECT TO LINK AROUND
    // asm 00003A32: 	LDI	*AR1,R1
    // asm 00003A33: 	BZ	DELOBJX
    if (*link == NULL)
        goto DELOBJX;
    // asm 00003A34: 	CMPI	R1,AR2
    // asm 00003A35: 	BNE	DELLP
    if (*link != obj) {
        link = &(*link)->link;
        goto DELLP;
    }
    // asm 00003A36: 	LDI	*AR2,R1
    // asm 00003A37: 	STI	R1,*AR1			;LINK AROUND
    *link = obj->link; // ;LINK AROUND
    // asm 00003A38: 	LDI	@OFREE,R1
    // asm 00003A39: 	STI	R1,*AR2
    // asm 00003A3A: 	STI	AR2,@OFREE
    obj->link = OFREE;
    OFREE = obj;
    // asm 00003A3B: 	LDI	@OFREECNT,R0		;INCREMENT FREE OBJECT COUNT
    // asm 00003A3C: 	ADDI	1,R0
    // asm 00003A3D: 	STI	R0,@OFREECNT
    OFREECNT += 1;
    // asm 00003A3E: 	CLRI	R0
    // asm 00003A3F: 	STI	R0,*+AR2(OLINK2)	;CLEAR SEARCH ID
    // asm 00003A40: 	STI	R0,*+AR2(OFLAGS)
    obj->link2 = 0; // ;CLEAR SEARCH ID
    obj->flags = 0;
DELOBJX:
    // asm 00003A41: 	POP	AR2
    // asm 00003A42: 	POP	AR1
    // asm 00003A43: 	POP	R1
    // asm 00003A44: 	POP	R0
    // asm 00003A45: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_DELETE_HIGH_PRIORITY", 0, 0);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 */
/* asm: SAVED_COUNTDOWN	.bss	SAVED_COUNTDOWN,1 */
int SAVED_COUNTDOWN;

static void BONUS_SCREEN(PROC* p) {
    int saved_background_color;
    PROC_CONTEXT* bonus_ctx;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    }

    // asm 00003A46: 	LDI	@BGNDCOLA,R0
    // asm 00003A47: 	STI	R0,*+AR7(PDATA)
    saved_background_color = BGNDCOLA;
    // asm 00003A48: 	LDI	@_countdown,R0
    // asm 00003A49: 	STI	R0,@SAVED_COUNTDOWN
    SAVED_COUNTDOWN = _countdown;
    // asm 00003A4A: 	CLRI	R0
    // asm 00003A4B: 	STI	R0,@BGNDCOLA
    BGNDCOLA = 0;
    // asm 00003A4C: 	CALL	SILENT
    SILENT();
    // asm 00003A4D: 	CALL	SND_RESET_QUIET
    SND_RESET_QUIET();
    // asm 00003A4E: 	CLRI	R0
    // asm 00003A4F: 	STI	R0,@STOPWATCH_CNTL
    STOPWATCH_CNTL = 0;
    // asm 00003A50: 	STI	R0,@DO_FOLDFLAG
    DO_FOLDFLAG = 0;
    // asm 00003A51: 	CALL	KILL_THEM
    KILL_THEM();
    // asm 00003A52: 	LDI	0,R0			;SMOKE MAY BE KILLED, SO I HAVE TO RESET THIS
    // asm 00003A53: 	STI	R0,@TIRE_SMOKE_COUNT
    TIRE_SMOKE_COUNT = 0; // ;SMOKE MAY BE KILLED, SO I HAVE TO RESET THIS
    // asm 00003A54: 	LDI	1,R0
    // asm 00003A55: 	STI	R0,@IGNORE_UPDATES
    IGNORE_UPDATES = 1;
    // asm 00003A56: 	CALL	PRC_INIT
    PRC_INIT();
    // asm 00003A57: 	CREATE	BONSCRN2,UTIL_C
    bonus_ctx = NEW_PROC_CONTEXT();
    bonus_ctx->BONSCRN2.background_color = saved_background_color;
    CREATE(BONSCRN2, UTIL_C, bonus_ctx);
    // asm 00003A5A: 	CREATE	BONUS_WAIT_LOOP,PLYR_C|PLYR1_T
    CREATE(BONUS_WAIT_LOOP, PLYR_C | PLYR1_T, NULL);
    // asm 00003A5D: 	CREATE	SCAN_OBJECTS,UTIL_C
    CREATE(SCAN_OBJECTS, UTIL_C, NULL);
    // asm 00003A60: 	CALL	FIND_AND_REACTIVATE	;REACTIVATE CHEERING ANIMATIONS
    FIND_AND_REACTIVATE(); // ;REACTIVATE CHEERING ANIMATIONS
#if DEBUG
    // asm: 	CALL	VERIFY_CODE_INTEGRITY
#endif
    // asm 00003A61:         LDP     @FASTSTKI		;GET PAGE OF STORED ADDRESS
    // asm 00003A62:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm 00003A63: 	BR	COLD_ENTER
    COLD_ENTER();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

/* asm: SPEEDHIT	.bss	SPEEDHIT,1 */
static int SPEEDHIT;

/*
 *
 *
 *
 */
static void BONSCRN2(PROC* p) {
    PROC_CONTEXT* child_ctx;
    PROC* proc;
    tagGAMETRAK* game_track;
    const tPALETTE_OVERWRITE_ENTRY* palette_entry;
    tPAL* source_palette;
    int switch_mask;
    int switch_code;
    int steer_delta;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    case 4:
        goto PROC_RESUME_4;
    case 5:
        goto PROC_RESUME_5;
    case 6:
        goto PROC_RESUME_6;
    case 7:
        goto PROC_RESUME_7;
    case 8:
        goto PROC_RESUME_8;
    case 9:
        goto PROC_RESUME_9;
    case 10:
        goto PROC_RESUME_10;
    case 11:
        goto PROC_RESUME_11;
    case 12:
        goto PROC_RESUME_12;
    case 13:
        goto PROC_RESUME_13;
    case 14:
        goto PROC_RESUME_14;
    case 15:
        goto PROC_RESUME_15;
    case 16:
        goto PROC_RESUME_16;
    case 17:
        goto PROC_RESUME_17;
    }
    // asm 00003A64: 	LDI	@DID_TIMED_OUT,R0
    // asm 00003A65: 	BZ	DOREG3A
    // asm 00003A66: 	LDI	0,R0		;ELAPSED TIME OF 0 = DID NOT FINISH
    // asm 00003A67: 	STI	R0,@ETIME
    if (DID_TIMED_OUT != 0) {
        ETIME = 0; // ;ELAPSED TIME OF 0 = DID NOT FINISH
    }
DOREG3A:
    // asm 00003A68: LDI	@STOPWATCH,R0
    // asm 00003A69: 	STI	R0,@ETIME
    ETIME = STOPWATCH;
    // *ELP CHANGE
    // 	;TURN ON LINK HERE... ALL LISTEN
    // 	;
    // 	;
    // asm 00003A6A: 	CALL	CLRONE		;CAN NOW BE DUAL PLAYER
    CLRONE(); // ;CAN NOW BE DUAL PLAYER
    // *ELP END CHANGE
    // asm 00003A6B: 	CLRI	R0
    // asm 00003A6C: 	STI	R0,@START_HIT	;----------------------------------
    START_HIT = 0;
    // asm 00003A6D: 	CREATE	MOVEOUT_HUD_EQUIP,UTIL_C
    CREATE(MOVEOUT_HUD_EQUIP, UTIL_C, &(PROC_CONTEXT){ 0 });
    // 	;Wait for sound board to reset
    // 	;
CNR_ENTER:
    // asm 00003A70: 	LDP	@IN_RESET_MODE
    // asm 00003A71: 	LDI	@IN_RESET_MODE,R0
    // asm 00003A72: 	SETDP
    // asm 00003A73: 	BZ	CONTINUE
    if (IN_RESET_MODE == 0) {
        goto CONTINUE;
    }
    // asm 00003A74: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00003A76: 	BU	CNR_ENTER
    goto CNR_ENTER;
CONTINUE:
    // asm 00003A77: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00003A78: 	STI	R0,@WAS_HEAD2HEAD_ON
    WAS_HEAD2HEAD_ON = HEAD2HEAD_ON;
    // asm 00003A79: 	CALL	CLEAR_LINK	;WE ARE NOW *NOT* LINKED
    CLEAR_LINK(); // ;WE ARE NOW *NOT* LINKED
    // asm 00003A7A: 	SOND1	MAPTUNE
    SOND1(MAPTUNE);
    // asm 00003A7C: 	LDI	@DID_TIMED_OUT,R0
    // asm 00003A7D: 	BZ	PAPAPA44
    if (DID_TIMED_OUT == 0) {
        goto PAPAPA44;
    }
    // asm 00003A7E: 	LDI	@POSITION,R0
    // asm 00003A7F: 	CMPI	1,R0
    // asm 00003A80: 	BNE	COOLRET
    if (POSITION != 1) {
        goto COOLRET;
    }
    // asm 00003A81: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm 00003A82: 	BZ	COOLRET
    if (WAS_HEAD2HEAD_ON == 0) {
        goto COOLRET;
    }
    // asm 00003A83: 	LDI	2,R0
    // asm 00003A84: 	STI	R0,@POSITION
    POSITION = 2;
    // asm 00003A85: 	BU	COOLRET
    goto COOLRET;
PAPAPA44:
    // asm 00003A86: 	CREATE	BACKUP_CAMERA,UTIL_C|BONUS_SCREEN_T
    CREATE(BACKUP_CAMERA, UTIL_C | BONUS_SCREEN_T, &(PROC_CONTEXT){ 0 });
    // asm 00003A89: 	SONDFX	CROWD1
    SONDFX(CROWD1);
    // ;	LDI	@DID_TIMED_OUT,R0
    // ;	B
    // asm 00003A8B: 	SONDFX	CROWDROAR
    SONDFX(CROWDROAR);
    // asm 00003A8D: 	SOND1	CHICKCHEER		;CHAN3
    SOND1(CHICKCHEER); // ;CHAN3
    // asm 00003A8F: JJDDHH
    // asm 00003A8F: 	LDI	@POSITION,R0
    // asm 00003A90: 	CMPI	1,R0
    // asm 00003A91: 	BNE	COOLRET
    if (POSITION != 1) {
        goto COOLRET;
    }
    // asm 00003A92: 	CREATEC	FREE_RACE_ANNOUNCE,UTIL_C
    CREATEC(CURRENT_PROC, FREE_RACE_ANNOUNCE, UTIL_C, &(PROC_CONTEXT){ 0 });
    // asm 00003A95: 	JSRP	BABE_TROPHY
    JSRP(BABE_TROPHY, 15);
COOLRET:
    // 	;LAME TEXT EFFECTS
    // 	;
    // 	;
    // asm 00003A9B: 	LDI	AR7,AR5
    // asm 00003A9C: 	CREATE	BONS_MAXMPH,UTIL_C|TEXTP_T
    CREATE(BONS_MAXMPH, UTIL_C | TEXTP_T, &(PROC_CONTEXT){ 0 });
    // asm 00003A9F: 	CREATE	BONS_ETIME,UTIL_C|TEXTP_T
    CREATE(BONS_ETIME, UTIL_C | TEXTP_T, &(PROC_CONTEXT){ 0 });
    // asm 00003AA2: 	CREATE	BONS_POSITION,UTIL_C|TEXTP_T
    CREATE(BONS_POSITION, UTIL_C | TEXTP_T, &(PROC_CONTEXT){ 0 });
    // asm 00003AA5: 	CREATE	BONS_RECORDTIME,UTIL_C|TEXTP_T
    CREATE(BONS_RECORDTIME, UTIL_C | TEXTP_T, &(PROC_CONTEXT){ 0 });
    // asm 00003AA8: 	CREATE	DISPLAY_H2H_WINNER,UTIL_C|TEXTP_T|3
    CREATE(DISPLAY_H2H_WINNER, UTIL_C | TEXTP_T | 3, &(PROC_CONTEXT){ 0 });
    // asm 00003AAB: 	LDI	@DID_TIMED_OUT,R0
    // asm 00003AAC: 	BNZ	NODOHOTTIME
    if (DID_TIMED_OUT != 0) {
        goto NODOHOTTIME;
    }
    // asm 00003AAD: 	CALL	INTO_TABLE_P
    switch_code = INTO_TABLE_P();
    // asm 00003AAE: 	BNC	NODOHOTTIME
    if (switch_code < 0) {
        goto NODOHOTTIME;
    }
    // asm 00003AAF: 	CMPI	0,R0
    // asm 00003AB0: 	BNE	ISREC
    if (switch_code != 0) {
        goto ISREC;
    }
    // asm 00003AB1: 	CREATE	BONS_HOTTIME_REC,UTIL_C|TEXTP_T
    CREATE(BONS_HOTTIME_REC, UTIL_C | TEXTP_T, &(PROC_CONTEXT){ 0 });
    // asm 00003AB4: 	BU	NODOHOTTIME
    goto NODOHOTTIME;
ISREC:
    // asm 00003AB5: CREATE	BONS_HOTTIME,UTIL_C|TEXTP_T
    CREATE(BONS_HOTTIME, UTIL_C | TEXTP_T, &(PROC_CONTEXT){ 0 });
NODOHOTTIME:
    // 	;insert show best times here!
    // 	;
    // 	;
    // asm 00003AB8: 	SLEEP	5
    SLEEP(5, 2);
    // asm 00003ABA: 	LDI	@POSITION,R0
    // asm 00003ABB: 	CMPI	1,R0
    // asm 00003ABC: 	BEQ	JJAG
    if (POSITION == 1) {
        goto JJAG;
    }
    // asm 00003ABD: 	LDI	0,R0
    // asm 00003ABE: 	LDI	100,R1
    // asm 00003ABF: 	CALL	SET_TRACK_VOL
    SET_TRACK_VOL(0, 100);
    // asm 00003AC0: 	SOND1	MAPTUNE
    SOND1(MAPTUNE);
JJAG:
    // asm 00003AC2: 	SONDFX	CROWD1
    SONDFX(CROWD1);
    // asm 00003AC4: 	LDI	120-1,AR5
    p->ctx.BONSCRN2.wait_count = 120 - 1;
    // asm 00003AC5: PAPA45
PAPA45:
    // asm 00003AC5: 	LDI	@START_HIT,R0
    // asm 00003AC6: 	BNZ	PAPA45X
    if (START_HIT != 0) {
        goto PAPA45X;
    }
    // asm 00003AC7: 	SUBI	@NFRAMES,AR5
    // asm 00003AC8: 	CMPI	0,AR5
    // asm 00003AC9: 	LDILT	0,AR5
    p->ctx.BONSCRN2.wait_count -= NFRAMES;
    if (p->ctx.BONSCRN2.wait_count < 0) {
        p->ctx.BONSCRN2.wait_count = 0;
    }
    // asm 00003ACA: 	SLEEP	1
    SLEEP(1, 3);
    // asm 00003ACC: 	DBU	AR5,PAPA45
    p->ctx.BONSCRN2.wait_count -= 1;
    if (p->ctx.BONSCRN2.wait_count >= 0) {
        goto PAPA45;
    }
PAPA45X:
    // asm 00003ACD: 	CREATE	UNFOLDMAP,UTIL_C
    CREATE(UNFOLDMAP, UTIL_C, &(PROC_CONTEXT){ 0 });
    // asm 00003AD0: 	SLEEP	10
    SLEEP(10, 4);
    // asm 00003AD2: 	LDI	1,R0
    // asm 00003AD3: 	STI	R0,@UNFOLDFLAG
    UNFOLDFLAG = 1;
    // asm 00003AD4: 	SLEEP	30
    SLEEP(30, 5);
    // asm 00003AD6: 	CALL	LOAD_FIXED_PALETTES
    LOAD_FIXED_PALETTES();
    // ;THIS RELOADS THE PALETTES THAT WERE USED FOR THE BABES
    // asm 00003AD7: 	PUSH	AR5
    // asm 00003AD8: 	PUSH	AR6
    // asm 00003AD9: 	LDI	@BABE_PALISTI,AR5
    // asm 00003ADA: 	LDI	4,AR6
    p->ctx.BONSCRN2.palette_index = 0;
    // asm 00003ADB: FIXPALLP
FIXPALLP:
    palette_entry = &BABE_PALISTI[p->ctx.BONSCRN2.palette_index];
    // asm 00003ADB: 	LDI	*+AR5,AR2
    // asm 00003ADC: 	CALL	PAL_FIND
    switch_code = PAL_FIND(palette_entry->destination_palette_index);
    // asm 00003ADD: 	BC	FIXPAL1
    if (switch_code < 0) {
        goto FIXPAL1;
    }
    // asm 00003ADE: 	LDI	AR2,R0
    // asm 00003ADF: 	LDI	R0,R1
    // asm 00003AE0: 	CALL	PAL_OVERWRITE
    source_palette = (tPAL*)ROM_PTR(palette_entry->source_palette_rom);
    PAL_SET(source_palette->data, (u32)switch_code, (u32)source_palette->flags_and_count);
FIXPAL1:
    // asm 00003AE1: 	NOP	*++AR5(2)
    // asm 00003AE2: 	DBU	AR6,FIXPALLP
    p->ctx.BONSCRN2.palette_index += 1;
    if (p->ctx.BONSCRN2.palette_index <= 4) {
        goto FIXPALLP;
    }
    // asm 00003AE3: 	POP	AR6
    // asm 00003AE4: 	POP	AR5
    // asm 00003AE5: 	LDL	SW_VIEW0|SW_VIEW1|SW_VIEW2|SW_RADIO|SW_4TH|SW_3RD|SW_2ND|SW_1ST,R1
    switch_mask = SW_VIEW0 | SW_VIEW1 | SW_VIEW2 | SW_RADIO | SW_4TH | SW_3RD | SW_2ND | SW_1ST;
    // asm 00003AE6: 	LDL	SW_VIEW1|SW_RADIO|SW_2ND,R2
    switch_code = SW_VIEW1 | SW_RADIO | SW_2ND;
    // asm 00003AE7: 	LDI	@SWITCHBUTS,R0
    // asm 00003AE8: 	AND	R1,R0
    // asm 00003AE9: 	CMPI	R2,R0
    // asm 00003AEA: 	BNE	NOWAY
    if ((SWITCHBUTS & switch_mask) != switch_code) {
        goto NOWAY;
    }
    // asm 00003AEB: 	READADJ	ADJ_STEERMIN
    steer_delta = READADJ(ADJ_STEERMIN);
    // asm 00003AED: 	SUBI	@_pot0,R0
    // asm 00003AEE: 	ABSI	R0
    steer_delta = abs(steer_delta - _pot0);
    // asm 00003AEF: 	CMPI	20,R0
    // asm 00003AF0: 	BGT	NOWAY
    if (steer_delta > 20) {
        goto NOWAY;
    }
    // asm 00003AF1: 	LDI	RM_USA,R0
    // asm 00003AF2: 	STI	R0,@RACE_MODE
    RACE_MODE = RM_USA;
NOWAY:
    // asm 00003AF3: 	CALL	KILL_THE_REANIMATORS		;cheering crowd
    KILL_THE_REANIMATORS(); // ;cheering crowd
    // asm 00003AF4: 	LDL	shared_PALETTES,AR2
    // asm 00003AF5: 	CALL	dealloc_section
    dealloc_section(shared_PALETTES);
    // asm 00003AF6: 	LDL	shared_PALETTES,AR2
    // asm 00003AF7: 	CALL	alloc_section
    alloc_section(shared_PALETTES);
    // asm 00003AF8: 	LDL	_SECshared,AR2
    // asm 00003AF9: 	CALL	LOAD_SECTION_REQ
    LOAD_SECTION_REQ(&SECshared);
    // asm 00003AFA: 	CALL	INIT_DRONES			;init DRONE tracker system
    INIT_DRONES(); // ;init DRONE tracker system
    // asm 00003AFB: 	LDI	@RACE_MODE,R0
    // asm 00003AFC: 	CMPI	RM_USA,R0
    // asm 00003AFD: 	BNE	DDF1
    if (RACE_MODE != RM_USA) {
        goto DDF1;
    }
    // asm 00003AFE: 	CREATE	SHOWLEG_PROC,UTIL_C|55h
    CREATE(SHOWLEG_PROC, UTIL_C | 0x55, &(PROC_CONTEXT){ 0 });
DDF1:
    // asm 00003B01: 	SONDFX	CC_DIGIT
    SONDFX(CC_DIGIT);
    // 	;SHOW STATISTICS TO DATE
    // 	;
    // 	;
    // asm 00003B03: 	LDI	@BONUS_WAVE,R5
    // asm 00003B04: 	SUBI	1,R5
    game_track = &((tagGAMETRAK*)GAMETRAK)[BONUS_WAVE - 1];
    // asm 00003B05: 	LDI	R5,AR0
    // asm 00003B06: 	MPYI	GT_SIZE,AR0
    // asm 00003B07: 	ADDI	@GAMETRAKI,AR0
    // ;	LDI	@MAXMPH,R0
    // ;	STI	R0,*+AR0(GT_MAXMPH)
    // asm 00003B08: 	LDI	1,R0
    // asm 00003B09: 	STI	R0,*+AR0(GT_POS)
    game_track->position = 1;
    // asm 00003B0A: 	STI	R0,*+AR0(GT_COLLS)
    game_track->collisions = 1;
    // asm 00003B0B: 	LDI	@ETIME,R0
    // asm 00003B0C: 	STI	R0,*+AR0(GT_ETIME)
    game_track->elapsed_time = (u32)ETIME;
    // asm 00003B0D: 	LDI	@RACE_MODE,R0
    // asm 00003B0E: 	CMPI	RM_USA,R0
    // asm 00003B0F: 	BNE	NOTNIN2
    if (RACE_MODE != RM_USA) {
        goto NOTNIN2;
    }
    // asm 00003B10: 	LDI	@GAMETRAKI,AR3
    // asm 00003B11: 	LDI	@BUFFERSI,AR4
    // asm 00003B12: 	CLRI	R4
    p->ctx.BONSCRN2.flag_index = 0;
    // asm 00003B13: 	FLOAT	180,R7		;Y
    p->ctx.BONSCRN2.flag_y = C3X_FROM_INT(180); // ;Y
NXTNAME:
    // asm 00003B14: 	LDI	@START_HIT,R0
    // asm 00003B15: 	BNZ	NOTNIN2
    if (START_HIT != 0) {
        goto NOTNIN2;
    }
    // asm 00003B16: 	CALL	PLACE_FLAG
    PLACE_FLAG(p->ctx.BONSCRN2.flag_index);
    // asm 00003B17: 	SLEEP	2
    SLEEP(2, 6);
    // asm 00003B19: 	ADDF	10,R7
    p->ctx.BONSCRN2.flag_y = C3X_ADD(p->ctx.BONSCRN2.flag_y, C3X_IMM_F32(10));
    // asm 00003B1A: 	ADDI	GT_SIZE,AR3
    // asm 00003B1B: 	INC	R4
    p->ctx.BONSCRN2.flag_index += 1;
    // asm 00003B1C: 	CMPI	R5,R4
    // asm 00003B1D: 	BLE	NXTNAME
    if (p->ctx.BONSCRN2.flag_index <= BONUS_WAVE - 1) {
        goto NXTNAME;
    }
NOTNIN2:
    // 	;CHECK INITS
    // 	;
    // 	;SLEEP	30			;Make sure last flag is done
    // asm 00003B1E: 	LDI	20-1,AR5
    p->ctx.BONSCRN2.wait_count = 20 - 1;
    // asm 00003B1F: PAPA35
PAPA35:
    // asm 00003B1F: 	LDI	@START_HIT,R0
    // asm 00003B20: 	BNZ	PAPA35X
    if (START_HIT != 0) {
        goto PAPA35X;
    }
    // asm 00003B21: 	SLEEP	1
    SLEEP(1, 7);
    // asm 00003B23: 	DBU	AR5,PAPA35
    p->ctx.BONSCRN2.wait_count -= 1;
    if (p->ctx.BONSCRN2.wait_count >= 0) {
        goto PAPA35;
    }
PAPA35X:
    // 	;DANGEROUS AND MESSY CODE, BUT
    // 	;WE NEVER TOUCH THE ACTIVE LIST
    // 	;ANYWAY, THEREFORE WE DONT NEED
    // 	;THESE ELEMENTS
    // 	;(OBJ_INIT CALLED IMMEDIATELY NEXT)
    // 	;
    // asm 00003B24: 	CLRI	R0
    // asm 00003B25: 	STI	R0,@OACTIVE
    OACTIVE = NULL;
    // asm 00003B26: 	READAUD	ADJ_HIGH_SCORE_ENTRY
    // asm 00003B28: 	CMPI	0,R0
    // asm 00003B29: 	BEQ	NOENTER
    if (READAUD(ADJ_HIGH_SCORE_ENTRY) == 0) {
        goto NOENTER;
    }
    // asm 00003B2A: 	JSRP	ENTER_INITIALS
    JSRP(ENTER_INITIALS, 16);
    // asm 00003B30: 	LDF	@STEERCT,R0		;STEERING CENTER
    // asm 00003B31: 	STF	R0,@WHEELPOS
    WHEELPOS = C3X_STF(C3X_LDF(STEERCT)); // ;STEERING CENTER
NOENTER:
    // asm 00003B32: 	CLRI	R0
    // asm 00003B33: 	STI	R0,@NOSWAP		;NO PAGE SWAPPING OFF
    NOSWAP = 0; // ;NO PAGE SWAPPING OFF
    // asm 00003B34: 	LDI	@RACE_MODE,R0
    // asm 00003B35: 	CMPI	RM_USA,R0
    // asm 00003B36: 	BNE	NOTNIN
    if (RACE_MODE != RM_USA) {
        goto NOTNIN;
    }
    // asm 00003B37: 	LDI	@BONUS_WAVE,R0
    // asm 00003B38: 	CMPI	14,R0
    // asm 00003B39: 	BEQ	CLINTON_SHOW
    if (BONUS_WAVE == 14) {
        PROC_CONTINUE(CLINTON_SHOW, 8);
        return;
    }
NOTNIN:
    // asm 00003B3A: 	CLRI	R0
    // asm 00003B3B: 	STI	R0,@NOSWAP
    NOSWAP = 0;
    // asm 00003B3C: 	STI	R0,@NOLONG_VEHICLES
    NOLONG_VEHICLES = 0;
    // asm 00003B3D: 	CLRF	R0
    // asm 00003B3E: 	LDP	@_CAMERAPOS+X
    // asm 00003B3F: 	STF	R0,@_CAMERAPOS+X
    _CAMERAPOS.X = C3X_STF(C3X_IMM_F32(0));
    // asm 00003B40: 	FLOAT	-1000,R0
    // asm 00003B41: 	STF	R0,@_CAMERAPOS+Y
    _CAMERAPOS.Y = C3X_STF(C3X_FROM_INT(-1000));
    // asm 00003B42: 	FLOAT	-2500,R0
    // asm 00003B43: 	STF	R0,@_CAMERAPOS+Z
    _CAMERAPOS.Z = C3X_STF(C3X_FROM_INT(-2500));
    // asm 00003B44: 	CLRF	R2
    // asm 00003B45: 	STF	R2,@_CAMERARAD+Y
    _CAMERARAD.Y = C3X_STF(C3X_IMM_F32(0));
    // asm 00003B46: 	SETDP
    // asm 00003B47: 	LDI	@CAMERAMATRIXI,AR2
    // asm 00003B48: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&_CAMERAMATRIX, C3X_LDF(_CAMERARAD.Y));
    // 	;\\++\\	    //--//
    // 	;  \\++\\ //--//
    // 	;    \\++\\ //
    // asm 00003B49: 	LDI	@RACE_MODE,R0
    // asm 00003B4A: 	CMPI	RM_USA,R0
    // asm 00003B4B: 	BNE	NOT_ATOZ
    if (RACE_MODE != RM_USA) {
        goto NOT_ATOZ;
    }
    // ;	CALL	CLEANUP_PALS
    // asm 00003B4C: 	LDI	@BONUS_WAVE,AR0
    // asm 00003B4D: 	ADDI	@FULLSETUP_TABLEI,AR0
ISWRP:
    // asm 00003B4E: LDI	*AR0,R0
    // asm 00003B4F: 	CALLU	R0
    FULLSETUP_TABLE[BONUS_WAVE]();
NOT_ATOZ:
    // asm 00003B50: 	SLEEP	10
    SLEEP(10, 9);
    // asm 00003B52: 	LDI	1,R0
    // asm 00003B53: 	STI	R0,@DO_FOLDFLAG
    DO_FOLDFLAG = 1;
    // asm 00003B54: 	SLEEP	10
    SLEEP(10, 10);
    // asm 00003B56: 	LDI	@RACE_MODE,R0
    // asm 00003B57: 	CMPI	RM_USA,R0
    // asm 00003B58: 	BNE	CHOOSE_NEXT_RACE
    if (RACE_MODE != RM_USA) {
        PROC_CONTINUE(CHOOSE_NEXT_RACE, 14);
        return;
    }
    // asm 00003B59: 	READAUD	AUD_RESET_TOTALLY
    // asm 00003B5B: 	CMPI	0,R0
    // asm 00003B5C: 	BNE	MOTION_SCREWED
    if (READAUD(AUD_RESET_TOTALLY) != 0) {
        PROC_CONTINUE(MOTION_SCREWED, 17);
        return;
    }
    // asm 00003B5D: 	CREATE	SHOWNEXTLEG_PROC,UTIL_C|TEXTP_T
    CREATE(SHOWNEXTLEG_PROC, UTIL_C | TEXTP_T, &(PROC_CONTEXT){ 0 });
    // asm 00003B60: 	LDI	@DID_TIMED_OUT,R0		;IF TIMED OUT THEN YOU MAY NOT GET A FREE RACE
    // asm 00003B61: 	BNZ	NOT1ST
    if (DID_TIMED_OUT != 0) {
        goto NOT1ST;
    }
    // asm 00003B62: 	LDI	@POSITION,R0
    // asm 00003B63: 	CMPI	1,R0
    // asm 00003B64: 	BNE	NOT1ST
    if (POSITION != 1) {
        goto NOT1ST;
    }
    // asm 00003B65: 	READAUD	ADJ_FREEGAME
    // asm 00003B67: 	CMPI	0,R0
    // asm 00003B68: 	BEQ	NOT1ST
    if (READAUD(ADJ_FREEGAME) == 0) {
        goto NOT1ST;
    }
    // asm 00003B69: 	LDI	@BONUS_WAVE,AR2
    // asm 00003B6A: 	DEC	AR2
    // asm 00003B6B: 	ADDI	AUD_WIN_GGATE,AR2
    // asm 00003B6C: 	CALL	AUDIT_INC
    AUDIT_INC(AUD_WIN_GGATE + BONUS_WAVE - 1);
    // asm 00003B6D: 	INCAUD	AUD_TOTAL_FREEGAMES
    INCAUD(AUD_TOTAL_FREEGAMES);
    // asm 00003B6F: 	LDI	@_MODE,R0
    // asm 00003B70: 	ANDN	MMODE,R0
    // asm 00003B71: 	OR	MINTRO,R0
    // asm 00003B72: 	STI	R0,@_MODE
    _MODE = (_MODE & ~MMODE) | MINTRO;
    // asm 00003B73: 	LDI	@BONUS_WAVE,R0
    // asm 00003B74: 	STI	R0,@CHOSEN_RACE
    CHOSEN_RACE = BONUS_WAVE;
    // asm 00003B75: 	CALL	SEND_RACENUM
    SEND_RACENUM();
    // asm 00003B76: 	BU	FINFIN
    goto FINFIN;
    // 	;Has the player made Pole Position?  (1)
    // 	;T-> Branch over this Coin Grab
    // 	;F-> Get more coin to continue
    // 	;
NOT1ST:
    // 	;;;	SYNCED INSMORE...
    // 	;;;	the difference is that the two machine communicate the time
    // 	;;;
    // 	;;;
    // asm 00003B77: 	JSRP	INSMORE
    JSRP(INSMORE, 11);
    // asm 00003B7D: 	LDI	@_MODE,R0
    // asm 00003B7E: 	ANDN	MMODE,R0
    // asm 00003B7F: 	OR	MINTRO,R0
    // asm 00003B80: 	STI	R0,@_MODE
    _MODE = (_MODE & ~MMODE) | MINTRO;
    // *ELP CHANGE
    // ;	CALL	CLRONE		;CAN NOW BE DUAL PLAYER
    // *ELP END CHANGE
    // asm 00003B81: 	CMPI	1,AR6
    // asm 00003B82: 	BNE	ENDPLAYER
    if (p->ctx.INSMORE_FRAME.result != 1) {
        ENDPLAYER();
        return;
    }
    // asm 00003B83: 	INCAUD	AUD_GAMES_CONTINUES
    INCAUD(AUD_GAMES_CONTINUES);
    // asm 00003B85: 	CALL	HSTDEC
    HSTDEC();
    // asm 00003B86: 	LDI	@BONUS_WAVE,R0
    // asm 00003B87: 	STI	R0,@CHOSEN_RACE
    CHOSEN_RACE = BONUS_WAVE;
    // asm 00003B88: 	CALL	SEND_RACENUM
    SEND_RACENUM();
FINFIN:
    // *ELP CHANGE
    // ;	LDI	@_MODE,R0
    // ;	ANDN	MINFIN|MWATER,R0
    // ;	STI	R0,@_MODE
    // *ELP END CHANGE
    // asm 00003B89: 	JSRP	ISSUE_STARTGAME
    JSRP(ISSUE_STARTGAME, 12);
    // asm 00003B8F: 	JSRP	WAIT_FOR_CHALLENGER
    JSRP(WAIT_FOR_CHALLENGER, 13);
    // 	;  /-----RACE MODE ONLY-------
    // 	;  |
    // 	;  V
    // asm 00003B95: 	LDI	@BONUS_WAVE,AR2
    // asm 00003B96: 	MPYI	2,AR2
    // asm 00003B97: 	ADDI	AUD_START_GGATE,AR2
    // asm 00003B98: 	CALL	AUDIT_INC
    AUDIT_INC(AUD_START_GGATE + BONUS_WAVE * 2);
    // asm 00003B99: 	CALL	TEXT_INIT
    TEXT_INIT();
    // asm 00003B9A: 	LDI	UTIL_C|TEXTP_T,R0
    // asm 00003B9B: 	LDI	-1,R1
    // asm 00003B9C: 	CALL	PRC_KILLALL
    PRC_KILLALL(UTIL_C | TEXTP_T, -1);
    // *ELP CHANGE
    // asm 00003B9D: 	LDI	@_MODE,R0
    // asm 00003B9E: 	ANDN	MINFIN|MWATER,R0
    // asm 00003B9F: 	STI	R0,@_MODE
    _MODE &= ~(MINFIN | MWATER);
    // *ELP END CHANGE
    // asm 00003BA0: 	CALL	OBJ_INIT
    OBJ_INIT();
    // asm 00003BA1: 	CALL	DYNAOBJ_INIT		;init DYNAMIC OBJECTS
    DYNAOBJ_INIT(); // ;init DYNAMIC OBJECTS
    // asm 00003BA2: 	CALL	CARB_INIT		;init CAR BLOCKS
    CARB_INIT(); // ;init CAR BLOCKS
    // asm 00003BA3: 	CALL	INIT_RDDEBRIS		;initialize ROAD DEBRIS list(s)
    INIT_RDDEBRIS(); // ;initialize ROAD DEBRIS list(s)
    // 	;;;	RE INITIALIZE TRACK...
    // 	;;;
    // 	;;;
    // asm 00003BA4: 	CALL	TEXT_INIT
    TEXT_INIT();
    // asm 00003BA5: 	LDI	UTIL_C|TEXTP_T,R0
    // asm 00003BA6: 	LDI	-1,R1
    // asm 00003BA7: 	CALL	PRC_KILLALL
    PRC_KILLALL(UTIL_C | TEXTP_T, -1);
    // asm 00003BA8: 	LDI	0,R0
    // asm 00003BA9: 	LDI	255,R1
    // asm 00003BAA: 	CALL	SET_TRACK_VOL
    SET_TRACK_VOL(0, 255);
    // asm 00003BAB: 	CALL	RESUME_TUNE_NT
    RESUME_TUNE_NT();
    // asm 00003BAC: 	LDI	BUT_VIEW2,R0			;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm 00003BAD: 	STI	R0,@BUTTON_STATUS
    BUTTON_STATUS = BUT_VIEW2; // ;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm 00003BAE: 	READAUD	ADJ_TIME_TO_START
    // asm 00003BB0: 	MPYI	5,R0
    // asm 00003BB1: 	ADDI	60,R0
    // asm 00003BB2: 	STI	R0,@_countdown
    _countdown = READAUD(ADJ_TIME_TO_START) * 5 + 60;
    // asm 00003BB3: 	LDI	@_MODE,R0
    // asm 00003BB4: 	ANDN	MGO|MMODE,R0
    // asm 00003BB5: 	OR	MGAME|MINFIN,R0
    // asm 00003BB6: 	STI	R0,@_MODE
    _MODE = (_MODE & ~(MGO | MMODE)) | MGAME | MINFIN;
    // asm 00003BB7: 	CREATEC	FOLDMAP,UTIL_C
    CREATEC(CURRENT_PROC, FOLDMAP, UTIL_C, &(PROC_CONTEXT){ 0 });
    // asm 00003BBA: 	LDI	@BONUS_WAVE,AR0
    // asm 00003BBB: 	ADDI	@BONUS_POSTLAUNCHI,AR0
    // asm 00003BBC: 	LDI	*AR0,R0
    // asm 00003BBD: 	CALLU	R0
    BONUS_POSTLAUNCH[BONUS_WAVE]();
    // asm 00003BBE: 	LDI	@NEXT_STARTUP,R0
    // asm 00003BBF: 	STI	R0,@STARTSECTION
    STARTSECTION = NEXT_STARTUP;
    // asm 00003BC0: 	CALL	BGD_INIT
    BGD_INIT();
    // asm 00003BC1: 	PUSH	AR7
    // asm 00003BC2: 	LDI	UTIL_C|BACKGRND_T,R0
    // asm 00003BC3: 	LDI	-1,R1
    // asm 00003BC4: 	CALL	PRC_FIND
    proc = PRC_FIND(UTIL_C | BACKGRND_T, -1);
    // asm 00003BC5: 	LDI	AR0,AR7
    // asm 00003BC6: 	LDI	3,R0
    // asm 00003BC7: 	STI	R0,*+AR7(PTIME)
    if (proc != NULL) {
        proc->sleep_ticks = 3;
    }
    // asm 00003BC8: 	LDI	PLYR_C|PLYR1_T,R0
    // asm 00003BC9: 	LDI	-1,R1
    // asm 00003BCA: 	CALL	PRC_FIND
    proc = PRC_FIND(PLYR_C | PLYR1_T, -1);
    // asm 00003BCB: 	LDI	AR0,AR2
    // asm 00003BCC: 	CALL	PRC_FOLLOW
    if (proc != NULL) {
        PRC_FOLLOW(proc, p);
    }
    // asm 00003BCD: 	POP	AR7
    // asm 00003BCE: 	LDI	*+AR7(PDATA),R0
    // asm 00003BCF: 	STI	R0,@BGNDCOLA
    BGNDCOLA = p->ctx.BONSCRN2.background_color;
    // asm 00003BD0: 	LDI	1,R0
    // asm 00003BD1: 	STI	R0,@NOAERASE
    NOAERASE = 1;
    // asm 00003BD2: 	CLRI	R0
    // asm 00003BD3: 	STI	R0,@OHIGH_PRIORITY
    OHIGH_PRIORITY = NULL;
    // asm 00003BD4: 	CREATE	WAVEFLAG,UTIL_C|MONKEY_T
    child_ctx = NEW_PROC_CONTEXT();
    CREATE(WAVEFLAG, UTIL_C | MONKEY_T, child_ctx);
    // asm 00003BD7: 	CLRI	R0
    // asm 00003BD8: 	STI	R0,@DID_TIMED_OUT
    DID_TIMED_OUT = 0;
    // asm 00003BD9: 	CALL	INIT_GAMELEG
    INIT_GAMELEG();
    // Original game bugfix: keep subsequent USA tour legs at the same 28.5 Hz
    // gameplay frame rate used by the first leg and by standalone races.
    FRAMRATE = 1;
    // asm 00003BDA: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *entered into from above (process)
 *
 *
 *
 *
 */
static void CLINTON_SHOW(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        MAME_ASSERT_ORDERING("CLINTON_SHOW");
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }
    // asm 00003BDB: 	LDI	RM_SINGLE,R0
    // asm 00003BDC: 	STI	R0,@RACE_MODE
    RACE_MODE = RM_SINGLE;
    // asm 00003BDD: 	LDI	1,R0
    // asm 00003BDE: 	STI	R0,@DO_FOLDFLAG
    DO_FOLDFLAG = 1;
    // asm 00003BDF: 	LDI	UTIL_C,R0
    // asm 00003BE0: 	LDI	-1,R1
    // asm 00003BE1: 	CALL	PRC_KILLALL
    PRC_KILLALL(UTIL_C, -1);
    // asm 00003BE2: 	LDI	UTIL_C|TEXTP_T,R0
    // asm 00003BE3: 	LDI	-1,R1
    // asm 00003BE4: 	CALL	PRC_KILLALL
    PRC_KILLALL(UTIL_C | TEXTP_T, -1);
    // asm 00003BE5: 	LDI	PLYR_C|PLYR1_T,R0
    // asm 00003BE6: 	LDI	-1,R1
    // asm 00003BE7: 	CALL	PRC_KILLALL
    PRC_KILLALL(PLYR_C | PLYR1_T, -1);
    // asm 00003BE8: 	CALL	TEXT_INIT
    TEXT_INIT();
    // asm 00003BE9: 	READAUD	ADJ_CLINTON
    // asm 00003BEB: 	CMPI	1,R0
    // asm 00003BEC: 	BNE	NOCLINTON
    if (READAUD(ADJ_CLINTON) != 1) {
        goto NOCLINTON;
    }
    // asm 00003BED: 	JSRP	HOTTUB_SCENE
    HOTTUB_SCENE();
NOCLINTON:
    // 	;patched from hottub.asm
    // 	;to cure potential lockup when HOTTUB_SCENE
    // 	;is not called.
    // 	;Wed Mar 8 09:52:18 1995
    // 	;
    // 	;
    // asm 00003BF3: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00003BF5: 	CALL	OBJ_INIT
    OBJ_INIT();
    // asm 00003BF6: 	CALL	INIT_DRONES	;initialize DRONE tracker system
    INIT_DRONES(); // ;initialize DRONE tracker system
    // asm 00003BF7: 	CALL	DYNAOBJ_INIT	;initialize DYNAMIC OBJECTS
    DYNAOBJ_INIT(); // ;initialize DYNAMIC OBJECTS
    // asm 00003BF8: 	CALL	CARB_INIT	;initialize CAR BLOCKS
    CARB_INIT(); // ;initialize CAR BLOCKS
    // asm 00003BF9: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    INIT_RDDEBRIS(); // ;initialize ROAD DEBRIS list(s)
    // asm 00003BFA: 	LDI	1234h,R0
    // asm 00003BFB: 	LDI	-1,R1
    // asm 00003BFC: 	CALL	PRC_KILLALL
    PRC_KILLALL(0x1234, -1);
    // asm 00003BFD: 	LDI	SPAWNER_C|ANIMATION_T,R0
    // asm 00003BFE: 	LDI	-1,R1
    // asm 00003BFF: 	CALL	PRC_KILLALL
    PRC_KILLALL(SPAWNER_C | ANIMATION_T, -1);
    // asm 00003C00: 	LDL	dc_shared_PALETTES,AR2
    // asm 00003C01: 	CALL	dealloc_section
    dealloc_section(dc_shared_PALETTES);
    // asm 00003C02: 	LDL	finale_PALETTES,AR2
    // asm 00003C03: 	CALL	dealloc_section
    dealloc_section(finale_PALETTES);
    // 	;
    // 	;
    // 	;>>>insert MAP with time for entire race
    // 	;
    // asm 00003C04: 	LDI	15,R0
    // asm 00003C05: 	STI	R0,@BONUS_WAVE
    BONUS_WAVE = 15;
    // asm 00003C06: 	READAUD	ADJ_HIGH_SCORE_ENTRY
    // asm 00003C08: 	CMPI	0,R0
    // asm 00003C09: 	BEQ	NOENTR2
    if (READAUD(ADJ_HIGH_SCORE_ENTRY) == 0) {
        goto NOENTR2;
    }
    // asm 00003C0A: 	JSRP	ENTER_INITIALS
    JSRP(ENTER_INITIALS, 2);
    // asm 00003C10: 	LDF	@STEERCT,R0		;STEERING CENTER
    // asm 00003C11: 	STF	R0,@WHEELPOS
    WHEELPOS = C3X_STF(C3X_LDF(STEERCT)); // ;STEERING CENTER
NOENTR2:
    // asm 00003C12: CLRI	R0
    // asm 00003C13: 	STI	R0,@NOSWAP
    NOSWAP = 0;
    // asm 00003C14: 	CALL	OBJ_INIT
    OBJ_INIT();
    // asm 00003C15: 	CALL	CLEAR_MAP_PALS
    CLEAR_MAP_PALS();
    // asm 00003C16: 	LDI	MATTR,R0
    // asm 00003C17: 	STI	R0,@_MODE
    _MODE = MATTR;
    // asm 00003C18: 	LDI	@POSITION,R0
    // asm 00003C19: 	CMPI	1,R0
    // asm 00003C1A: 	BNE	JAJD
    if (POSITION != 1) {
        goto JAJD;
    }
    // asm 00003C1B: 	INCAUD	AUD_CREDITS
    INCAUD(AUD_CREDITS);
    // asm 00003C1D: 	INCAUD	AUD_WIN_DC
    INCAUD(AUD_WIN_DC);
    // asm 00003C1F: 	INCAUD	AUD_TOTAL_FREEGAMES
    INCAUD(AUD_TOTAL_FREEGAMES);
JAJD:
    // asm 00003C21: 	JSRP	VANITY_SUB
    VANITY_SUB();
    // asm 00003C27: 	LDI	-2,R0
    // asm 00003C28: 	STI	R0,@_ATTR_MODE
    _ATTR_MODE = -2;
    // asm 00003C29: 	BU	SET_ATTR
    SET_ATTR();
    return;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

void BLINK_FREEBE(PROC* p) {
    int mode;
    int posx;
    int posy;
    int palette;
    tSHADOW_TEXT text;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

    // asm 00003C2A: 	SLEEP	5
    SLEEP(5, 1);
    // asm 00003C2C: 	LDI	5*18,AR5
    p->ctx.BLINK_FREEBE.loop_count = 5 * 18;
    // asm 00003C2D: BFLP1
BFLP1:
    // asm 00003C2D: 	LDI	@_MODE,R4
    // asm 00003C2E: 	AND	MMODE,R4
    mode = _MODE & MMODE;
    // asm 00003C2F: 	CMPI	MATTR,R4
    // asm 00003C30: 	LDINE	70,R3
    // asm 00003C31: 	LDIEQ	125,R3
    posy = mode != MATTR ? 70 : 125;
    // asm 00003C32: 	LDINE	256,R2
    // asm 00003C33: 	LDIEQ	365,R2
    posx = mode != MATTR ? 256 : 365;
    // asm 00003C34: 	FLOAT	R2
    // asm 00003C35: 	FLOAT	R3
    // asm 00003C36: 	LDI	1,RC
    // asm 00003C37: 	LDL	WINT1,AR2
    // asm 00003C38: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(WINT1, C3X_FROM_INT(posx), C3X_FROM_INT(posy), 1);
    // asm 00003C39: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    // asm 00003C3C: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.shadow->color |= TXT_CENTER;
    // asm 00003C3F: 	LDL	font18_white,AR2
    // asm 00003C40: 	CALL	PAL_FIND_RAW
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(font18_white_ROM));
    // asm 00003C41: 	STI	R0,*+AR0(TEXT_PAL)
    text.front->palette = (u32)palette;
    // asm 00003C42: 	STI	R0,*+AR1(TEXT_PAL)
    text.shadow->palette = (u32)palette;
    // asm 00003C43: 	CMPI	MATTR,R4
    // asm 00003C44: 	LDINE	90,R3
    // asm 00003C45: 	LDIEQ	150,R3
    posy = mode != MATTR ? 90 : 150;
    // asm 00003C46: 	LDINE	256,R2
    // asm 00003C47: 	LDIEQ	365,R2
    posx = mode != MATTR ? 256 : 365;
    // asm 00003C48: 	FLOAT	R2
    // asm 00003C49: 	FLOAT	R3
    // asm 00003C4A: 	LDI	1,RC
    // asm 00003C4B: 	LDL	WINT2,AR2
    // asm 00003C4C: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(WINT2, C3X_FROM_INT(posx), C3X_FROM_INT(posy), 1);
    // asm 00003C4D: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    // asm 00003C50: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.shadow->color |= TXT_CENTER;
    // asm 00003C53: 	LDL	font18_white,AR2
    // asm 00003C54: 	CALL	PAL_FIND_RAW
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(font18_white_ROM));
    // asm 00003C55: 	STI	R0,*+AR0(TEXT_PAL)
    text.front->palette = (u32)palette;
    // asm 00003C56: 	STI	R0,*+AR1(TEXT_PAL)
    text.shadow->palette = (u32)palette;
    // ;insert frame rate equalizer
    // ;
    // asm 00003C57: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00003C59: 	DBU	AR5,BFLP1
    p->ctx.BLINK_FREEBE.loop_count -= 1;
    if (p->ctx.BLINK_FREEBE.loop_count >= 0) {
        goto BFLP1;
    }
    // asm 00003C5A: 	DIE
    DIE();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 */
static void BACKUP_CAMERA(PROC* p) {
    OBJ* track_obj;
    OBJ* next_track_obj;
    c3x_reg_t angle;
    c3x_reg_t distance;
    c3x_reg_t camera_x;
    c3x_reg_t camera_z;
    c3x_reg_t delta;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00003C5B: 	LDI	@PLYCBLK,AR0
    // asm 00003C5C: 	LDI	*+AR0(CARTRAK),AR2
    track_obj = OBJREF_TO_PTR(PLYCBLK->closest_track_piece);
    // asm 00003C5D: 	LDI	*+AR2(OLINK4),AR0
    next_track_obj = (OBJ*)track_obj->link4;
    // asm 00003C5E: 	LDF	*+AR0(OPOSX),R2
    // asm 00003C5F: 	SUBF	*+AR2(OPOSX),R2
    // asm 00003C60: 	LDF	*+AR0(OPOSZ),R3
    // asm 00003C61: 	SUBF	*+AR2(OPOSZ),R3
    // asm 00003C62: 	CALL	ARCTANF
    angle = ARCTANF(C3X_SUB(C3X_LDF(next_track_obj->pos.X), C3X_LDF(track_obj->pos.X)),
        C3X_SUB(C3X_LDF(next_track_obj->pos.Z), C3X_LDF(track_obj->pos.Z)));
    // asm 00003C63: 	SUBF	HALFPI,R0
    angle = C3X_SUB(angle, C3X_IMM_F32(HALFPI));
    // asm 00003C64: 	LDF	R0,R2
    // asm 00003C65: 	FLOAT	-7500,R0
    // asm 00003C66: 	ADDF	@ZOOMD,R0
    distance = C3X_ADD(C3X_FROM_INT(-7500), C3X_LDF(ZOOMD));
    // asm 00003C67: 	CALL	DISTANCE_2D
    // asm 00003C68: 	LDF	R0,R6
    // asm 00003C69: 	LDF	R1,R7
    p->ctx.BACKUP_CAMERA.target_x = C3X_MUL(distance, _COSI(angle));
    p->ctx.BACKUP_CAMERA.target_z = C3X_MUL(C3X_NEG(distance), _SINE(angle));
    // asm 00003C6A: 	LDP	@_CAMERAPOS+X
    // asm 00003C6B: 	ADDF	@_CAMERAPOS+X,R6
    // asm 00003C6C: 	ADDF	@_CAMERAPOS+Z,R7
    p->ctx.BACKUP_CAMERA.target_x = C3X_ADD(p->ctx.BACKUP_CAMERA.target_x, C3X_LDF(_CAMERAPOS.X));
    p->ctx.BACKUP_CAMERA.target_z = C3X_ADD(p->ctx.BACKUP_CAMERA.target_z, C3X_LDF(_CAMERAPOS.Z));
    // asm 00003C6D: 	SETDP
    // asm 00003C6E: 	LDI	30,AR5
    p->ctx.BACKUP_CAMERA.loop_count = 30;
    // asm 00003C6F: BACKLP
BACKLP:
    // asm 00003C6F: 	LDP	@_CAMERAPOS+X
    // asm 00003C70: 	LDF	@_CAMERAPOS+X,R0
    // asm 00003C71: 	LDF	@_CAMERAPOS+Z,R1
    // asm 00003C72: 	SUBF	R0,R6,R2
    // asm 00003C73: 	SUBF	R1,R7,R3
    camera_x = C3X_LDF(_CAMERAPOS.X);
    camera_z = C3X_LDF(_CAMERAPOS.Z);
    // asm 00003C74: 	MPYF	0.02,R2
    delta = C3X_MUL(C3X_SUB(p->ctx.BACKUP_CAMERA.target_x, camera_x), C3X_IMM_F32(0.02));
    // asm 00003C75: 	MPYF	0.02,R3
    // asm 00003C76: 	ADDF	R0,R2,R0
    // asm 00003C77: 	ADDF	R1,R3,R1
    // asm 00003C78: 	STF	R0,@_CAMERAPOS+X
    _CAMERAPOS.X = C3X_STF(C3X_ADD(camera_x, delta));
    // asm 00003C79: 	STF	R1,@_CAMERAPOS+Z
    delta = C3X_MUL(C3X_SUB(p->ctx.BACKUP_CAMERA.target_z, camera_z), C3X_IMM_F32(0.02));
    _CAMERAPOS.Z = C3X_STF(C3X_ADD(camera_z, delta));
    // asm 00003C7A: 	LDF	@_CAMERAPOS+Y,R0
    // asm 00003C7B: 	SUBF	10,R0
    // asm 00003C7C: 	STF	R0,@_CAMERAPOS+Y
    _CAMERAPOS.Y = C3X_STF(C3X_SUB(C3X_LDF(_CAMERAPOS.Y), C3X_IMM_F32(10)));
    // asm 00003C7D: 	SETDP
    // asm 00003C7E: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00003C80: 	DBU	AR5,BACKLP
    p->ctx.BACKUP_CAMERA.loop_count -= 1;
    if (p->ctx.BACKUP_CAMERA.loop_count >= 0) {
        goto BACKLP;
    }
    // asm 00003C81: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

/* asm: DID_TIMED_OUT	.bss	DID_TIMED_OUT,1 */
int DID_TIMED_OUT;

// *----------------------------------------------------------------------------
void TIMED_OUT(void) {
    // asm 00003C82: 	LDI	1,R0
    // asm 00003C83: 	STI	R0,@DID_TIMED_OUT
    DID_TIMED_OUT = 1;
    // asm 00003C84: 	INCAUD	AUD_GAMES_EXPIRED
    INCAUD(AUD_GAMES_EXPIRED);
    // asm 00003C86: 	LDI	@BONUS_WAVE,AR2
    // asm 00003C87: 	ADDI	@BONUS_TABLEI,AR2
    // asm 00003C88: 	LDI	*AR2,R0
    // asm 00003C89: 	CALLU	R0
    BONUS_TABLEI[BONUS_WAVE]();
    // asm 00003C8A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMED_OUT", 0, 0);
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void KILL_PLYR_SOUNDS(void) {
    // asm 00003C8B: 	LDI	1000,AR2		;ENGINE RPM IDLE
    // asm 00003C8C: 	CALL	SENDSND
    SENDSND(1000); // ;ENGINE RPM IDLE
    // asm 00003C8D: 	LDI	SKIDB,AR2 		;KILL LOOPERS WHILE SUSPENDED
    // asm 00003C8E: 	CALL	KILLSNDFX
    KILLSNDFX(SKIDB); // ;KILL LOOPERS WHILE SUSPENDED
    // asm 00003C8F: 	LDI	SKIDC,AR2 		;KILL LOOPERS WHILE SUSPENDED
    // asm 00003C90: 	CALL	KILLSNDFX
    KILLSNDFX(SKIDC); // ;KILL LOOPERS WHILE SUSPENDED
    // asm 00003C91: 	LDI	BRAKSND,AR2
    // asm 00003C92: 	CALL	KILLSNDFX
    KILLSNDFX(BRAKSND);
    // asm 00003C93: 	LDI	TUNSND,AR2
    // asm 00003C94: 	CALL	KILLSNDFX
    KILLSNDFX(TUNSND);
    // asm 00003C95: 	LDI	GRAVELA,AR2
    // asm 00003C96: 	CALL	KILLSNDFX
    KILLSNDFX(GRAVELA);
    // asm 00003C97: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILL_PLYR_SOUNDS", 0, 0);
}

// *----------------------------------------------------------------------------

static const char FRA1[] = "FIRST PLACE";
static const char FRA2[] = "FREE RACE";
#define CONGRATS 4
/* asm: CONGRAT_SPEECH:	.word	GL_WOOLAUGH,GL_YEAH,GL_YES,GL_YOUDIDIT */
/* asm: 	 */
/* asm: 	 */
static int CONGRAT_SPEECH[] = {
    GL_WOOLAUGH,
    GL_YEAH,
    GL_YES,
    GL_YOUDIDIT,
};

static void FREE_RACE_ANNOUNCE(PROC* p) {
    tTEXT* text;
    const char* second_line;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }
    // asm 00003CC4: 	SOND1	GETREADYTUNE
    SOND1(GETREADYTUNE);
    // asm 00003CC6: 	LDI	CONGRATS,AR2
    // asm 00003CC7: 	CALL	RANDU0
    // asm 00003CC8: 	LDI	R0,AR2
    // asm 00003CC9: 	ADDI	@CONGRAT_SPEECHI,AR2
    // asm 00003CCA: 	LDI	*AR2,AR2
    // asm 00003CCB: 	CALL	ONESNDFX
    ONESNDFX(CONGRAT_SPEECH[RANDU0(CONGRATS)]);
    // asm 00003CCC: 	LDI	7,AR5
    p->ctx.FREE_RACE_ANNOUNCE.loop_count = 7;
    // asm 00003CCD: FRAL1
FRAL1:
    // asm 00003CCD: 	LDI	@START_HIT,R0
    // asm 00003CCE: 	BNZ	PRC_SUICIDE
    if (START_HIT != 0) {
        DIE();
    }
    // asm 00003CCF: 	LDL	FRA1,AR2
    // asm 00003CD0: 	FLOAT	256,R2
    // asm 00003CD1: 	FLOAT	100,R3
    // asm 00003CD2: 	LDI	15,RC
    // asm 00003CD3: 	CALL	TEXT_ADD
    text = TEXT_ADD(FRA1, C3X_FROM_INT(256), C3X_FROM_INT(100), 15);
    // asm 00003CD4: 	CALL	SET40FONT
    SET40FONT(text);
    // asm 00003CD5: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text->color |= TXT_CENTER;
    // asm 00003CD8: 	SLEEP	9
    SLEEP(9, 1);
    // asm 00003CDA: 	LDL	FRA2,AR2
    // asm 00003CDB: 	PUSH	AR2
    // asm 00003CDC: 	READAUD	ADJ_FREEGAME
    // asm 00003CDE: 	POP	AR2
    // asm 00003CDF: 	CMPI	0,R0
    // asm 00003CE0: 	BNE	ISFREE
    second_line = FRA2;
    if (READAUD(ADJ_FREEGAME) != 0) {
        goto ISFREE;
    }
    // asm 00003CE1: 	LDL	FRA1,AR2
    second_line = FRA1;
ISFREE:
    // asm 00003CE2: 	FLOAT	256,R2
    // asm 00003CE3: 	FLOAT	150,R3
    // asm 00003CE4: 	LDI	15,RC
    // asm 00003CE5: 	CALL	TEXT_ADD
    text = TEXT_ADD(second_line, C3X_FROM_INT(256), C3X_FROM_INT(150), 15);
    // asm 00003CE6: 	CALL	SET40FONT
    SET40FONT(text);
    // asm 00003CE7: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text->color |= TXT_CENTER;
    // asm 00003CEA: 	SLEEP	10
    SLEEP(10, 2);
    // asm 00003CEC: 	DBU	AR5,FRAL1
    p->ctx.FREE_RACE_ANNOUNCE.loop_count -= 1;
    if (p->ctx.FREE_RACE_ANNOUNCE.loop_count >= 0) {
        goto FRAL1;
    }
    // asm 00003CED: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 */
static const char LCTS[] = "RACE COMPLETED:";
static const char LNLS[] = "NEXT RACE:";
/*
 *
 *
 */
#define SHOWLEG_TIME (20 + 20 + 50)

/*
 *
 *
 */
static void SHOWLEG_PROC(PROC* p) {
    tSHADOW_TEXT text;
    c3x_reg_t target;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    }
    // asm 00003CF0: 	LDI	@BONUS_WAVE,AR2
    // asm 00003CF1: 	DEC	AR2
    // asm 00003CF2: 	ADDI	@LEG_NAMESI,AR2
    // asm 00003CF3: 	LDI	*AR2,AR2
    // asm 00003CF4: 	FLOAT	256,R2
    // asm 00003CF5: 	FLOAT	300,R3
    // asm 00003CF6: 	LDI	SHOWLEG_TIME,RC
    // asm 00003CF7: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(LEG_NAMES[BONUS_WAVE - 1], C3X_FROM_INT(256), C3X_FROM_INT(300), SHOWLEG_TIME);
    // asm 00003CF8: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00003CFB: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    text.shadow->color |= TXT_CENTER;
    // asm 00003CFE: 	STI	AR0,*+AR7(PDATA)
    // asm 00003CFF: 	STI	AR1,*+AR7(PDATA+1)
    p->ctx.SHOWLEG_PROC.front_text[0] = text.front;
    p->ctx.SHOWLEG_PROC.shadow_text[0] = text.shadow;
    // asm 00003D00: 	CALL	SET18FONTDS
    SET18FONTDS(&text);
    // asm 00003D01: 	LDI	@LCTSI,AR2
    // asm 00003D02: 	FLOAT	256,R2
    // asm 00003D03: 	FLOAT	275,R3
    // asm 00003D04: 	LDI	SHOWLEG_TIME,RC
    // asm 00003D05: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(LCTS, C3X_FROM_INT(256), C3X_FROM_INT(275), SHOWLEG_TIME);
    // asm 00003D06: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00003D09: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    text.shadow->color |= TXT_CENTER;
    // asm 00003D0C: 	STI	AR0,*+AR7(PDATA+2)
    // asm 00003D0D: 	STI	AR1,*+AR7(PDATA+3)
    p->ctx.SHOWLEG_PROC.front_text[1] = text.front;
    p->ctx.SHOWLEG_PROC.shadow_text[1] = text.shadow;
    // asm 00003D0E: 	CALL	SET18FONTDS
    SET18FONTDS(&text);
    // asm 00003D0F: 	FLOAT	-100,R6
    p->ctx.SHOWLEG_PROC.posx = C3X_FROM_INT(-100);
    // asm 00003D10: 	LDI	20,AR5	;20
    p->ctx.SHOWLEG_PROC.loop_count = 20; // ;20
SLLP1:
    // asm 00003D11: FLOAT	256,R0
    // asm 00003D12: 	SUBF	R6,R0
    // asm 00003D13: 	MPYF	0.2,R0
    // asm 00003D14: 	ADDF	R0,R6
    target = C3X_FROM_INT(256);
    p->ctx.SHOWLEG_PROC.posx = C3X_ADD(
        p->ctx.SHOWLEG_PROC.posx,
        C3X_MUL(C3X_SUB(target, p->ctx.SHOWLEG_PROC.posx), C3X_IMM_F32(0.2)));
    // asm 00003D15: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D16: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D17: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D18: 	STF	R6,*+AR0(TEXT_POSX)
    p->ctx.SHOWLEG_PROC.shadow_text[0]->posx = C3X_STF(p->ctx.SHOWLEG_PROC.posx);
    p->ctx.SHOWLEG_PROC.shadow_text[1]->posx = C3X_STF(p->ctx.SHOWLEG_PROC.posx);
    // asm 00003D19: 	LDF	R6,R0
    // asm 00003D1A: 	ADDF	3,R0
    // asm 00003D1B: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D1C: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D1D: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D1E: 	STF	R0,*+AR0(TEXT_POSX)
    target = C3X_ADD(p->ctx.SHOWLEG_PROC.posx, C3X_IMM_F32(3));
    p->ctx.SHOWLEG_PROC.front_text[0]->posx = C3X_STF(target);
    p->ctx.SHOWLEG_PROC.front_text[1]->posx = C3X_STF(target);
    // asm 00003D1F: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00003D21: 	DBU	AR5,SLLP1
    p->ctx.SHOWLEG_PROC.loop_count -= 1;
    if (p->ctx.SHOWLEG_PROC.loop_count >= 0) {
        goto SLLP1;
    }
    // 	;CENTER IT
    // 	;
    // asm 00003D22: 	FLOAT	256,R6
    p->ctx.SHOWLEG_PROC.posx = C3X_FROM_INT(256);
    // asm 00003D23: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D24: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D25: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D26: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D27: 	LDF	R6,R0
    // asm 00003D28: 	ADDF	3,R0
    // asm 00003D29: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D2A: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D2B: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D2C: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.SHOWLEG_PROC.shadow_text[0]->posx = C3X_STF(p->ctx.SHOWLEG_PROC.posx);
    p->ctx.SHOWLEG_PROC.shadow_text[1]->posx = C3X_STF(p->ctx.SHOWLEG_PROC.posx);
    target = C3X_ADD(p->ctx.SHOWLEG_PROC.posx, C3X_IMM_F32(3));
    p->ctx.SHOWLEG_PROC.front_text[0]->posx = C3X_STF(target);
    p->ctx.SHOWLEG_PROC.front_text[1]->posx = C3X_STF(target);
    // asm 00003D2D: 	SLEEP	35
    SLEEP(35, 2);
    // asm 00003D2F: 	LDI	20,AR5
    p->ctx.SHOWLEG_PROC.loop_count = 20;
SLLP1A:
    // asm 00003D30: FLOAT	-100,R0
    // asm 00003D31: 	SUBF	R6,R0
    // asm 00003D32: 	MPYF	0.2,R0
    // asm 00003D33: 	ADDF	R0,R6
    target = C3X_FROM_INT(-100);
    p->ctx.SHOWLEG_PROC.posx = C3X_ADD(
        p->ctx.SHOWLEG_PROC.posx,
        C3X_MUL(C3X_SUB(target, p->ctx.SHOWLEG_PROC.posx), C3X_IMM_F32(0.2)));
    // asm 00003D34: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D35: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D36: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D37: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D38: 	LDF	R6,R0
    // asm 00003D39: 	ADDF	3,R0
    // asm 00003D3A: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D3B: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D3C: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D3D: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.SHOWLEG_PROC.shadow_text[0]->posx = C3X_STF(p->ctx.SHOWLEG_PROC.posx);
    p->ctx.SHOWLEG_PROC.shadow_text[1]->posx = C3X_STF(p->ctx.SHOWLEG_PROC.posx);
    target = C3X_ADD(p->ctx.SHOWLEG_PROC.posx, C3X_IMM_F32(3));
    p->ctx.SHOWLEG_PROC.front_text[0]->posx = C3X_STF(target);
    p->ctx.SHOWLEG_PROC.front_text[1]->posx = C3X_STF(target);
    // asm 00003D3E: 	SLEEP	1
    SLEEP(1, 3);
    // asm 00003D40: 	DBU	AR5,SLLP1A
    p->ctx.SHOWLEG_PROC.loop_count -= 1;
    if (p->ctx.SHOWLEG_PROC.loop_count >= 0) {
        goto SLLP1A;
    }
    // asm 00003D41: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void SHOWNEXTLEG_PROC(PROC* p) {
    tSHADOW_TEXT text;
    c3x_reg_t target;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    case 4:
        goto PROC_RESUME_4;
    }
    // asm 00003D42: 	SLEEP	20
    SLEEP(20, 1);
    // asm 00003D44: 	LDI	@BONUS_WAVE,AR2
    // asm 00003D45: 	ADDI	@LEG_NAMESI,AR2
    // asm 00003D46: 	LDI	*AR2,AR2
    // asm 00003D47: 	FLOAT	256,R2
    // asm 00003D48: 	FLOAT	50,R3
    // asm 00003D49: 	LDI	SHOWLEG_TIME,RC
    // asm 00003D4A: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(LEG_NAMES[BONUS_WAVE], C3X_FROM_INT(256), C3X_FROM_INT(50), SHOWLEG_TIME);
    // asm 00003D4B: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00003D4E: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    text.shadow->color |= TXT_CENTER;
    // asm 00003D51: 	STI	AR0,*+AR7(PDATA)
    // asm 00003D52: 	STI	AR1,*+AR7(PDATA+1)
    p->ctx.SHOWNEXTLEG_PROC.front_text[0] = text.front;
    p->ctx.SHOWNEXTLEG_PROC.shadow_text[0] = text.shadow;
    // asm 00003D53: 	CALL	SET18FONTDS
    SET18FONTDS(&text);
    // asm 00003D54: 	LDI	@LNLSI,AR2
    // asm 00003D55: 	FLOAT	256,R2
    // ;	FLOAT	275,R3
    // asm 00003D56: 	FLOAT	25,R3
    // asm 00003D57: 	LDI	SHOWLEG_TIME,RC
    // asm 00003D58: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(LNLS, C3X_FROM_INT(256), C3X_FROM_INT(25), SHOWLEG_TIME);
    // asm 00003D59: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00003D5C: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    text.shadow->color |= TXT_CENTER;
    // asm 00003D5F: 	STI	AR0,*+AR7(PDATA+2)
    // asm 00003D60: 	STI	AR1,*+AR7(PDATA+3)
    p->ctx.SHOWNEXTLEG_PROC.front_text[1] = text.front;
    p->ctx.SHOWNEXTLEG_PROC.shadow_text[1] = text.shadow;
    // asm 00003D61: 	CALL	SET18FONTDS
    SET18FONTDS(&text);
    // asm 00003D62: 	FLOAT	600,R6
    p->ctx.SHOWNEXTLEG_PROC.posx = C3X_FROM_INT(600);
    // asm 00003D63: 	LDI	16,AR5
    p->ctx.SHOWNEXTLEG_PROC.loop_count = 16;
SLLP2:
    // asm 00003D64: FLOAT	256,R0
    // asm 00003D65: 	SUBF	R6,R0
    // asm 00003D66: 	MPYF	0.25,R0
    // asm 00003D67: 	ADDF	R0,R6
    target = C3X_FROM_INT(256);
    p->ctx.SHOWNEXTLEG_PROC.posx = C3X_ADD(p->ctx.SHOWNEXTLEG_PROC.posx,
        C3X_MUL(C3X_SUB(target, p->ctx.SHOWNEXTLEG_PROC.posx), C3X_IMM_F32(0.25)));
    // asm 00003D68: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D69: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D6A: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D6B: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D6C: 	LDF	R6,R0
    // asm 00003D6D: 	ADDF	3,R0
    // asm 00003D6E: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D6F: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D70: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D71: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.SHOWNEXTLEG_PROC.shadow_text[0]->posx = C3X_STF(p->ctx.SHOWNEXTLEG_PROC.posx);
    p->ctx.SHOWNEXTLEG_PROC.shadow_text[1]->posx = C3X_STF(p->ctx.SHOWNEXTLEG_PROC.posx);
    target = C3X_ADD(p->ctx.SHOWNEXTLEG_PROC.posx, C3X_IMM_F32(3));
    p->ctx.SHOWNEXTLEG_PROC.front_text[0]->posx = C3X_STF(target);
    p->ctx.SHOWNEXTLEG_PROC.front_text[1]->posx = C3X_STF(target);
    // asm 00003D72: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00003D74: 	DBU	AR5,SLLP2
    p->ctx.SHOWNEXTLEG_PROC.loop_count -= 1;
    if (p->ctx.SHOWNEXTLEG_PROC.loop_count >= 0) {
        goto SLLP2;
    }
    // 	;CENTER IT
    // 	;
    // asm 00003D75: 	FLOAT	256,R6
    p->ctx.SHOWNEXTLEG_PROC.posx = C3X_FROM_INT(256);
    // asm 00003D76: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D77: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D78: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D79: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D7A: 	LDF	R6,R0
    // asm 00003D7B: 	ADDF	3,R0
    // asm 00003D7C: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D7D: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D7E: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D7F: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.SHOWNEXTLEG_PROC.shadow_text[0]->posx = C3X_STF(p->ctx.SHOWNEXTLEG_PROC.posx);
    p->ctx.SHOWNEXTLEG_PROC.shadow_text[1]->posx = C3X_STF(p->ctx.SHOWNEXTLEG_PROC.posx);
    target = C3X_ADD(p->ctx.SHOWNEXTLEG_PROC.posx, C3X_IMM_F32(3));
    p->ctx.SHOWNEXTLEG_PROC.front_text[0]->posx = C3X_STF(target);
    p->ctx.SHOWNEXTLEG_PROC.front_text[1]->posx = C3X_STF(target);
    // asm 00003D80: 	SLEEP	50
    SLEEP(50, 3);
    // asm 00003D82: 	LDI	215,AR5
    p->ctx.SHOWNEXTLEG_PROC.loop_count = 215;
SLLP2A:
    // asm 00003D83: FLOAT	600,R0
    // asm 00003D84: 	SUBF	R6,R0
    // asm 00003D85: 	MPYF	0.25,R0
    // asm 00003D86: 	ADDF	R0,R6
    target = C3X_FROM_INT(600);
    p->ctx.SHOWNEXTLEG_PROC.posx = C3X_ADD(p->ctx.SHOWNEXTLEG_PROC.posx,
        C3X_MUL(C3X_SUB(target, p->ctx.SHOWNEXTLEG_PROC.posx), C3X_IMM_F32(0.25)));
    // asm 00003D87: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D88: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D89: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D8A: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D8B: 	LDF	R6,R0
    // asm 00003D8C: 	ADDF	3,R0
    // asm 00003D8D: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D8E: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D8F: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D90: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.SHOWNEXTLEG_PROC.shadow_text[0]->posx = C3X_STF(p->ctx.SHOWNEXTLEG_PROC.posx);
    p->ctx.SHOWNEXTLEG_PROC.shadow_text[1]->posx = C3X_STF(p->ctx.SHOWNEXTLEG_PROC.posx);
    target = C3X_ADD(p->ctx.SHOWNEXTLEG_PROC.posx, C3X_IMM_F32(3));
    p->ctx.SHOWNEXTLEG_PROC.front_text[0]->posx = C3X_STF(target);
    p->ctx.SHOWNEXTLEG_PROC.front_text[1]->posx = C3X_STF(target);
    // asm 00003D91: 	SLEEP	1
    SLEEP(1, 4);
    // asm 00003D93: 	DBU	AR5,SLLP2A
    p->ctx.SHOWNEXTLEG_PROC.loop_count -= 1;
    if (p->ctx.SHOWNEXTLEG_PROC.loop_count >= 0) {
        goto SLLP2A;
    }
    // asm 00003D94: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	R4	INDEX
 *
 *
 */
/* asm: FLAG_POS_TABLE */
/* asm: 	.word	 36,133	;GGpark */
/* asm: 	.word	 39,147	;Sanfran */
/* asm: 	.word	 40,156	;I101 */
/* asm: 	.word	 43,166	;Redwds */
/* asm: 	.word	 63,192	;BeverlyHils */
/* asm: 	.word	 76,200	;LA freeway */
/* asm: 	.word	 88,190	;Deathvalley */
/* asm: 	.word	128,198	;Arizona */
/* asm: 	.word	257,106	;Mount Rush */
/* asm: 	.word	302,125	;Iowa */
/* asm: 	.word	334,125	;Chicago */
/* asm: 	.word	358,134	;Indiana */
/* asm: 	.word	412,129	;Appalachia */
/* asm: 	.word	434,140	;Washington */
static int FLAG_POS_TABLE[] = {
    36,
    133, // GGpark
    39,
    147, // Sanfran
    40,
    156, // I101
    43,
    166, // Redwds
    63,
    192, // BeverlyHils
    76,
    200, // LA freeway
    88,
    190, // Deathvalley
    128,
    198, // Arizona
    257,
    106, // Mount Rush
    302,
    125, // Iowa
    334,
    125, // Chicago
    358,
    134, // Indiana
    412,
    129, // Appalachia
    434,
    140, // Washington
};

/*
 *----------------------------------------------------------------------------
 *
 *
 *PARAMETERS
 *	R4	INDEX
 *
 */
static void PLACE_FLAG(int index) {
    OBJ* obj;
    PROC_CONTEXT* ctx;
    int table_index;
    // asm 00003D96: 	PUSH	AR0
    // asm 00003D97: 	PUSH	AR4
    // asm 00003D98: 	PUSH	R2
    // asm 00003D99: 	PUSH	R3
    // asm 00003D9A: 	PUSH	RC
    // asm 00003D9B: 	LDI	R4,AR0
    // asm 00003D9C: 	MPYI	2,AR0
    // asm 00003D9D: 	ADDI	@FLAG_POS_TABLEI,AR0
    table_index = index * 2;
    // asm 00003D9E: 	LDI	*AR0,R2
    // asm 00003D9F: 	SUBI	256,R2
    // asm 00003DA0: 	LDI	*+AR0,R3
    // asm 00003DA1: 	SUBI	200-20,R3
    // asm 00003DA2: 	LDI	367,RC
    // asm 00003DA3: 	LDL	star,AR2
    // asm 00003DA4: 	CALL	OBJ_QMAKE
    obj = OBJ_QMAKE(ROM_PTR(star_ROM), FLAG_POS_TABLE[table_index] - 256,
        FLAG_POS_TABLE[table_index + 1] - (200 - 20), 367);
    // asm 00003DA5: 	LDIL	O_3DROT,R0
    // asm 00003DA8: 	OR	O_NOUROT|O_NOUNIV,R0
    // asm 00003DA9: 	OR	*+AR0(OFLAGS),R0
    // asm 00003DAA: 	STI	R0,*+AR0(OFLAGS)
    obj->flags |= O_3DROT | O_NOUROT | O_NOUNIV;
    // asm 00003DAB: 	LDI	AR0,AR2
    // asm 00003DAC: 	CALL	OBJ_INSERTHP
    OBJ_INSERTHP(obj);
    // asm 00003DAD: 	LDI	AR0,AR4
    // asm 00003DAE: 	LDF	*+AR4(OPOSX),R0
    // asm 00003DAF: 	STF	R0,*+AR4(OVELX)
    obj->vel_x = C3X_STF(C3X_LDF(obj->pos.X));
    // asm 00003DB0: 	LDF	*+AR4(OPOSY),R0
    // asm 00003DB1: 	STF	R0,*+AR4(OVELY)
    obj->vel_y = C3X_STF(C3X_LDF(obj->pos.Y));
    // asm 00003DB2: 	LDF	100,R0
    // asm 00003DB3: 	CALL	SFRAND
    // asm 00003DB4: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(SFRAND(C3X_IMM_F32(100)));
    // asm 00003DB5: 	FLOAT	-230,R0
    // asm 00003DB6: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_FROM_INT(-230));
    // asm 00003DB7: 	CREATEC	PLACE_FLAG_PROC,UTIL_C
    ctx = NEW_PROC_CONTEXT();
    ctx->PLACE_FLAG_PROC.obj = obj;
    CREATEC(CURRENT_PROC, PLACE_FLAG_PROC, UTIL_C, ctx);
    // asm 00003DBA: 	SONDFX	MAPSTAR2
    SONDFX(MAPSTAR2);
    // asm 00003DBC: 	POP	RC
    // asm 00003DBD: 	POP	RC
    // asm 00003DBE: 	POP	R2
    // asm 00003DBF: 	POP	AR4
    // asm 00003DC0: 	POP	AR0
    // asm 00003DC1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLACE_FLAG", 0, 0);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *PARAMETERS
 *	AR4	OBJECT
 *	AR4	*+(OVEL) POSITION TO ACHIEVE
 *
 *
 */
static void PLACE_FLAG_PROC(PROC* p) {
    c3x_reg_t x;
    c3x_reg_t y;
    c3x_reg_t distance_squared;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00003DC2: 	LDI	12,AR5
    p->ctx.PLACE_FLAG_PROC.loop_count = 12;
    // asm 00003DC3: PFPLP
PFPLP:
    // asm 00003DC3: 	LDF	*+AR4(OVELX),R0
    // asm 00003DC4: 	SUBF	*+AR4(OPOSX),R0
    // asm 00003DC5: 	MPYF	0.25,R0
    // asm 00003DC6: 	ADDF	*+AR4(OPOSX),R0
    // asm 00003DC7: 	STF	R0,*+AR4(OPOSX)
    x = C3X_ADD(C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->pos.X),
        C3X_MUL(C3X_SUB(C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->vel_x),
                    C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->pos.X)),
            C3X_IMM_F32(0.25)));
    p->ctx.PLACE_FLAG_PROC.obj->pos.X = C3X_STF(x);
    // asm 00003DC8: 	LDF	*+AR4(OVELY),R1
    // asm 00003DC9: 	SUBF	*+AR4(OPOSY),R1
    // asm 00003DCA: 	MPYF	0.25,R1
    // asm 00003DCB: 	ADDF	*+AR4(OPOSY),R1
    // asm 00003DCC: 	STF	R1,*+AR4(OPOSY)
    y = C3X_ADD(C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->pos.Y),
        C3X_MUL(C3X_SUB(C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->vel_y),
                    C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->pos.Y)),
            C3X_IMM_F32(0.25)));
    p->ctx.PLACE_FLAG_PROC.obj->pos.Y = C3X_STF(y);
    // asm 00003DCD: 	MPYF	R1,R1
    // asm 00003DCE: 	MPYF	R0,R0
    // asm 00003DCF: 	ADDF	R1,R0
    distance_squared = C3X_ADD(C3X_MUL(y, y), C3X_MUL(x, x));
    // asm 00003DD0: 	CMPF	25,R0
    // asm 00003DD1: 	BLT	ALLDN7
    if (C3X_LT(distance_squared, C3X_IMM_F32(25))) {
        goto ALLDN7;
    }
    // asm 00003DD2: 	LDF	*+AR4(ORADZ),R2
    // asm 00003DD3: 	ADDF	0.1,R2
    // asm 00003DD4: 	STF	R2,*+AR4(ORADZ)
    p->ctx.PLACE_FLAG_PROC.obj->rad.Z = C3X_STF(C3X_ADD(
        C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->rad.Z), C3X_IMM_F32(0.1)));
    // asm 00003DD5: 	LDI	AR4,AR2
    // asm 00003DD6: 	ADDI	OMATRIX,AR2
    // asm 00003DD7: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&p->ctx.PLACE_FLAG_PROC.obj->omatrix,
        C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->rad.Z));
    // asm 00003DD8: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00003DDA: 	DBU	AR5,PFPLP
    p->ctx.PLACE_FLAG_PROC.loop_count -= 1;
    if (p->ctx.PLACE_FLAG_PROC.loop_count >= 0) {
        goto PFPLP;
    }
ALLDN7:
    // asm 00003DDB: 	LDF	*+AR4(OVELX),R0
    // asm 00003DDC: 	STF	R0,*+AR4(OPOSX)
    p->ctx.PLACE_FLAG_PROC.obj->pos.X = C3X_STF(C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->vel_x));
    // asm 00003DDD: 	LDF	*+AR4(OVELY),R0
    // asm 00003DDE: 	STF	R0,*+AR4(OPOSY)
    p->ctx.PLACE_FLAG_PROC.obj->pos.Y = C3X_STF(C3X_LDF(p->ctx.PLACE_FLAG_PROC.obj->vel_y));
    // asm 00003DDF: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

static const char BT1[] = "AVERAGE MPH:";
static const char BT1A[] = "AVERAGE KPH:";
// Original PDATA remains readable after DIE; static storage avoids retaining a freed PROC_CONTEXT buffer.
static char bonus_maxmph[32];

static void BONS_MAXMPH(PROC* p) {
    tSHADOW_TEXT text;
    c3x_reg_t value;
    c3x_reg_t delta;
    int palette;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    }
    // asm 00003DE0: 	LDL	BT1,R6
    // asm 00003DE1: 	LDL	BT1A,R7
    // asm 00003DE2: 	READAUD	ADJ_MPHORKPM
    // asm 00003DE4: 	CMPI	0,R0
    // asm 00003DE5: 	LDIEQ	R6,AR2
    // asm 00003DE6: 	LDINE	R7,AR2
    // asm 00003DE7: 	FLOAT	0,R2
    // asm 00003DE8: 	FLOAT	20,R3
    // asm 00003DE9: 	LDI	999,RC
    // asm 00003DEA: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(READAUD(ADJ_MPHORKPM) == 0 ? BT1 : BT1A, C3X_FROM_INT(0), C3X_FROM_INT(20), 999);
    // asm 00003DEB: 	STI	AR0,*+AR7(PDATA+10)	;SHADOW
    // asm 00003DEC: 	STI	AR1,*+AR7(PDATA+11)	;ACTUAL
    p->ctx.BONS_TEXT.label_front = text.front;
    p->ctx.BONS_TEXT.label_shadow = text.shadow;
    // asm 00003DED: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 00003DEE: 	OR	TXT_RIGHT,R0
    // asm 00003DEF: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 00003DF0: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 00003DF1: 	OR	TXT_RIGHT,R0
    // asm 00003DF2: 	STI	R0,*+AR1(TEXT_COLOR)
    text.front->color |= TXT_RIGHT;
    text.shadow->color |= TXT_RIGHT;
    // asm 00003DF3: 	READAUD	ADJ_MPHORKPM
    // asm 00003DF5: 	CMPI	0,R0
    // asm 00003DF6: 	BEQ	ISMPHT
    // asm 00003DF7: 	LDF	@MAXMPH,R0
    // asm 00003DF8: 	MPYF	1.6666,R0
    value = C3X_LDF(MAXMPH);
    if (READAUD(ADJ_MPHORKPM) != 0) {
        value = C3X_MUL(value, C3X_IMM_F32(1.6666));
    }
    // asm 00003DF9: 	BU	KJL
ISMPHT:
    // asm 00003DFA: LDF	@MAXMPH,R0
KJL:
    // asm 00003DFB: LDF	@MAXMPH_COUNT,R1
    // asm 00003DFC: 	CALL	DIV_F
    value = C3X_DIV(value, C3X_LDF(MAXMPH_COUNT));
    // asm 00003DFD: 	FIX	R0,R2
    // asm 00003DFE: 	LDI	AR7,AR2
    // asm 00003DFF: 	ADDI	PDATA,AR2
    // asm 00003E00: 	CALL	_itoa
    _itoa(bonus_maxmph, C3X_FIX(value));
    // asm 00003E01: 	FLOAT	512,R2
    // asm 00003E02: 	FLOAT	20,R3
    // asm 00003E03: 	LDI	999,RC
    // asm 00003E04: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(bonus_maxmph, C3X_FROM_INT(512), C3X_FROM_INT(20), 999);
    // asm 00003E05: 	CALL	SETLGDIGITFONTDS
    SETLGDIGITFONTDS(&text);
    // asm 00003E06: 	STI	AR0,*+AR7(PDATA+12)
    // asm 00003E07: 	STI	AR1,*+AR7(PDATA+13)
    p->ctx.BONS_TEXT.value_front = text.front;
    p->ctx.BONS_TEXT.value_shadow = text.shadow;
    // asm 00003E08: 	LDL	dnums_amber,AR2
    // asm 00003E09: 	CALL	PAL_FIND_RAW
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(dnums_amber_ROM));
    // asm 00003E0A: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00003E0B: 	STI	R0,*+AR1(TEXT_PAL)
    text.front->palette = (u32)palette;
    text.shadow->palette = (u32)palette;
    // asm 00003E0C: 	CLRF	R0
    // asm 00003E0D: 	STF	R0,@MAXMPH
    // asm 00003E0E: 	STF	R0,@MAXMPH_COUNT
    MAXMPH = C3X_STF(C3X_IMM_F32(0));
    MAXMPH_COUNT = C3X_STF(C3X_IMM_F32(0));
ENTER_HERE:
    // asm 00003E0F: 	LDI	31,AR5
    p->ctx.BONS_TEXT.loop_count = 31;
    // asm 00003E10: 	CLRF	R6		;FIXED STF
    p->ctx.BONS_TEXT.label_posx = C3X_IMM_F32(0); // ;FIXED STF
    // asm 00003E11: 	FLOAT	512,R7		;FLT STF
    p->ctx.BONS_TEXT.value_posx = C3X_FROM_INT(512); // ;FLT STF
    // asm 00003E12: BML
BML:
    // asm 00003E12: 	FLOAT	250,R0
    // asm 00003E13: 	SUBF	R6,R0
    // asm 00003E14: 	MPYF	0.1,R0
    // asm 00003E15: 	CMPF	8,R0
    // asm 00003E16: 	LDFLT	8,R0
    // asm 00003E17: 	ADDF	R0,R6
    delta = C3X_MUL(C3X_SUB(C3X_FROM_INT(250), p->ctx.BONS_TEXT.label_posx), C3X_IMM_F32(0.1));
    if (C3X_LT(delta, C3X_IMM_F32(8)))
        delta = C3X_IMM_F32(8);
    p->ctx.BONS_TEXT.label_posx = C3X_ADD(p->ctx.BONS_TEXT.label_posx, delta);
    if (C3X_GT(p->ctx.BONS_TEXT.label_posx, C3X_FROM_INT(250)))
        p->ctx.BONS_TEXT.label_posx = C3X_FROM_INT(250);
    // asm 00003E18: 	FLOAT	250,R0
    // asm 00003E19: 	CMPF	R0,R6
    // asm 00003E1A: 	LDFGT	R0,R6
    // asm 00003E1B: 	LDI	*+AR7(PDATA+10),AR0
    // asm 00003E1C: 	LDI	*+AR7(PDATA+11),AR1
    // asm 00003E1D: 	STF	R6,*+AR1(TEXT_POSX)
    // asm 00003E1E: 	LDF	R6,R0
    // asm 00003E1F: 	ADDF	3,R0
    // asm 00003E20: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.BONS_TEXT.label_shadow->posx = C3X_STF(p->ctx.BONS_TEXT.label_posx);
    p->ctx.BONS_TEXT.label_front->posx = C3X_STF(C3X_ADD(p->ctx.BONS_TEXT.label_posx, C3X_IMM_F32(3)));
    // asm 00003E21: 	FLOAT	260,R0
    // asm 00003E22: 	SUBF	R7,R0
    // asm 00003E23: 	MPYF	0.20,R0
    // asm 00003E24: 	CMPF	-12,R0
    // asm 00003E25: 	LDFLT	-12,R0
    // asm 00003E26: 	ADDF	R0,R7
    delta = C3X_MUL(C3X_SUB(C3X_FROM_INT(260), p->ctx.BONS_TEXT.value_posx), C3X_IMM_F32(0.20));
    if (C3X_LT(delta, C3X_IMM_F32(-12)))
        delta = C3X_IMM_F32(-12);
    p->ctx.BONS_TEXT.value_posx = C3X_ADD(p->ctx.BONS_TEXT.value_posx, delta);
    if (C3X_LT(p->ctx.BONS_TEXT.value_posx, C3X_FROM_INT(260)))
        p->ctx.BONS_TEXT.value_posx = C3X_FROM_INT(260);
    // asm 00003E27: 	FLOAT	260,R0
    // asm 00003E28: 	CMPF	R0,R7
    // asm 00003E29: 	LDFLT	R0,R7
    // asm 00003E2A: 	LDI	*+AR7(PDATA+12),AR0
    // asm 00003E2B: 	LDI	*+AR7(PDATA+13),AR1
    // asm 00003E2C: 	STF	R7,*+AR1(TEXT_POSX)
    // asm 00003E2D: 	LDF	R7,R0
    // asm 00003E2E: 	ADDF	3,R0
    // asm 00003E2F: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.BONS_TEXT.value_shadow->posx = C3X_STF(p->ctx.BONS_TEXT.value_posx);
    p->ctx.BONS_TEXT.value_front->posx = C3X_STF(C3X_ADD(p->ctx.BONS_TEXT.value_posx, C3X_IMM_F32(3)));
    // asm 00003E30: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00003E32: 	DBU	AR5,BML
    p->ctx.BONS_TEXT.loop_count -= 1;
    if (p->ctx.BONS_TEXT.loop_count >= 0)
        goto BML;
    // asm 00003E33: 	FLOAT	250,R0
    // asm 00003E34: 	FLOAT	253,R1
    // asm 00003E35: 	FLOAT	260,R2
    // asm 00003E36: 	FLOAT	263,R3
    // asm 00003E37: 	LDI	*+AR7(PDATA+10),AR0
    // asm 00003E38: 	STF	R1,*+AR0(TEXT_POSX)
    // asm 00003E39: 	LDI	*+AR7(PDATA+11),AR0
    // asm 00003E3A: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003E3B: 	LDI	*+AR7(PDATA+12),AR0
    // asm 00003E3C: 	STF	R3,*+AR0(TEXT_POSX)
    // asm 00003E3D: 	LDI	*+AR7(PDATA+13),AR0
    // asm 00003E3E: 	STF	R2,*+AR0(TEXT_POSX)
    p->ctx.BONS_TEXT.label_front->posx = C3X_STF(C3X_FROM_INT(253));
    p->ctx.BONS_TEXT.label_shadow->posx = C3X_STF(C3X_FROM_INT(250));
    p->ctx.BONS_TEXT.value_front->posx = C3X_STF(C3X_FROM_INT(263));
    p->ctx.BONS_TEXT.value_shadow->posx = C3X_STF(C3X_FROM_INT(260));
KKL:
    // asm 00003E3F: SLEEP	1
    SLEEP(1, 2);
    // asm 00003E41: 	LDI	@DO_FOLDFLAG,R0
    // asm 00003E42: 	BZ	KKL
    if (DO_FOLDFLAG == 0)
        goto KKL;
    // asm 00003E43: 	SONDFX	WIPE2
    SONDFX(WIPE2);
    // asm 00003E45: 	LDI	31,AR5
    p->ctx.BONS_TEXT.loop_count = 31;
    // asm 00003E46: 	FLOAT	250,R6		;FIXED STF
    // asm 00003E47: 	FLOAT	260,R7		;FLT STF
    p->ctx.BONS_TEXT.label_posx = C3X_FROM_INT(250);
    p->ctx.BONS_TEXT.value_posx = C3X_FROM_INT(260);
    // asm 00003E48: BML2
BML2:
    // asm 00003E48: 	FLOAT	-20,R0
    // asm 00003E49: 	SUBF	R6,R0
    // asm 00003E4A: 	MPYF	0.1,R0
    // asm 00003E4B: 	ADDF	R0,R6
    p->ctx.BONS_TEXT.label_posx = C3X_ADD(p->ctx.BONS_TEXT.label_posx,
        C3X_MUL(C3X_SUB(C3X_FROM_INT(-20), p->ctx.BONS_TEXT.label_posx), C3X_IMM_F32(0.1)));
    // asm 00003E4C: 	LDI	*+AR7(PDATA+10),AR0
    // asm 00003E4D: 	LDI	*+AR7(PDATA+11),AR1
    // asm 00003E4E: 	STF	R6,*+AR1(TEXT_POSX)
    // asm 00003E4F: 	LDF	R6,R0
    // asm 00003E50: 	ADDF	3,R0
    // asm 00003E51: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.BONS_TEXT.label_shadow->posx = C3X_STF(p->ctx.BONS_TEXT.label_posx);
    p->ctx.BONS_TEXT.label_front->posx = C3X_STF(C3X_ADD(p->ctx.BONS_TEXT.label_posx, C3X_IMM_F32(3)));
    // asm 00003E52: 	FLOAT	532,R0
    // asm 00003E53: 	SUBF	R7,R0
    // asm 00003E54: 	MPYF	0.1,R0
    // asm 00003E55: 	ADDF	R0,R7
    p->ctx.BONS_TEXT.value_posx = C3X_ADD(p->ctx.BONS_TEXT.value_posx,
        C3X_MUL(C3X_SUB(C3X_FROM_INT(532), p->ctx.BONS_TEXT.value_posx), C3X_IMM_F32(0.1)));
    // asm 00003E56: 	LDI	*+AR7(PDATA+12),AR0
    // asm 00003E57: 	LDI	*+AR7(PDATA+13),AR1
    // asm 00003E58: 	STF	R7,*+AR1(TEXT_POSX)
    // asm 00003E59: 	LDF	R7,R0
    // asm 00003E5A: 	ADDF	3,R0
    // asm 00003E5B: 	STF	R0,*+AR0(TEXT_POSX)
    p->ctx.BONS_TEXT.value_shadow->posx = C3X_STF(p->ctx.BONS_TEXT.value_posx);
    p->ctx.BONS_TEXT.value_front->posx = C3X_STF(C3X_ADD(p->ctx.BONS_TEXT.value_posx, C3X_IMM_F32(3)));
    // asm 00003E5C: 	SLEEP	1
    SLEEP(1, 3);
    // asm 00003E5E: 	DBU	AR5,BML2
    p->ctx.BONS_TEXT.loop_count -= 1;
    if (p->ctx.BONS_TEXT.loop_count >= 0)
        goto BML2;
    // asm 00003E5F: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

static const char RT2[] = "RECORD TIME:";
// Original PDATA remains readable after DIE; static storage avoids retaining a freed PROC_CONTEXT buffer.
static char bonus_recordtime[32];

static void BONS_TEXT_ANIMATE(PROC* p) {
    c3x_reg_t delta;

    switch (PROC_RESUME_STATE) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    }

    p->ctx.BONS_TEXT.loop_count = 31;
    p->ctx.BONS_TEXT.label_posx = C3X_IMM_F32(0);
    p->ctx.BONS_TEXT.value_posx = C3X_FROM_INT(512);
BONS_TEXT_IN:
    delta = C3X_MUL(C3X_SUB(C3X_FROM_INT(250), p->ctx.BONS_TEXT.label_posx), C3X_IMM_F32(0.1));
    if (C3X_LT(delta, C3X_IMM_F32(8))) {
        delta = C3X_IMM_F32(8);
    }
    p->ctx.BONS_TEXT.label_posx = C3X_ADD(p->ctx.BONS_TEXT.label_posx, delta);
    if (C3X_GT(p->ctx.BONS_TEXT.label_posx, C3X_FROM_INT(250))) {
        p->ctx.BONS_TEXT.label_posx = C3X_FROM_INT(250);
    }
    p->ctx.BONS_TEXT.label_shadow->posx = C3X_STF(p->ctx.BONS_TEXT.label_posx);
    p->ctx.BONS_TEXT.label_front->posx = C3X_STF(C3X_ADD(p->ctx.BONS_TEXT.label_posx, C3X_IMM_F32(3)));

    delta = C3X_MUL(C3X_SUB(C3X_FROM_INT(260), p->ctx.BONS_TEXT.value_posx), C3X_IMM_F32(0.20));
    if (C3X_LT(delta, C3X_IMM_F32(-12))) {
        delta = C3X_IMM_F32(-12);
    }
    p->ctx.BONS_TEXT.value_posx = C3X_ADD(p->ctx.BONS_TEXT.value_posx, delta);
    if (C3X_LT(p->ctx.BONS_TEXT.value_posx, C3X_FROM_INT(260))) {
        p->ctx.BONS_TEXT.value_posx = C3X_FROM_INT(260);
    }
    p->ctx.BONS_TEXT.value_shadow->posx = C3X_STF(p->ctx.BONS_TEXT.value_posx);
    p->ctx.BONS_TEXT.value_front->posx = C3X_STF(C3X_ADD(p->ctx.BONS_TEXT.value_posx, C3X_IMM_F32(3)));
    SLEEP(1, 1);
    p->ctx.BONS_TEXT.loop_count -= 1;
    if (p->ctx.BONS_TEXT.loop_count >= 0) {
        goto BONS_TEXT_IN;
    }

    p->ctx.BONS_TEXT.label_front->posx = C3X_STF(C3X_FROM_INT(253));
    p->ctx.BONS_TEXT.label_shadow->posx = C3X_STF(C3X_FROM_INT(250));
    p->ctx.BONS_TEXT.value_front->posx = C3X_STF(C3X_FROM_INT(263));
    p->ctx.BONS_TEXT.value_shadow->posx = C3X_STF(C3X_FROM_INT(260));
BONS_TEXT_WAIT:
    SLEEP(1, 2);
    if (DO_FOLDFLAG == 0) {
        goto BONS_TEXT_WAIT;
    }
    SONDFX(WIPE2);
    p->ctx.BONS_TEXT.loop_count = 31;
    p->ctx.BONS_TEXT.label_posx = C3X_FROM_INT(250);
    p->ctx.BONS_TEXT.value_posx = C3X_FROM_INT(260);
BONS_TEXT_OUT:
    p->ctx.BONS_TEXT.label_posx = C3X_ADD(p->ctx.BONS_TEXT.label_posx,
        C3X_MUL(C3X_SUB(C3X_FROM_INT(-20), p->ctx.BONS_TEXT.label_posx), C3X_IMM_F32(0.1)));
    p->ctx.BONS_TEXT.value_posx = C3X_ADD(p->ctx.BONS_TEXT.value_posx,
        C3X_MUL(C3X_SUB(C3X_FROM_INT(532), p->ctx.BONS_TEXT.value_posx), C3X_IMM_F32(0.1)));
    p->ctx.BONS_TEXT.label_shadow->posx = C3X_STF(p->ctx.BONS_TEXT.label_posx);
    p->ctx.BONS_TEXT.label_front->posx = C3X_STF(C3X_ADD(p->ctx.BONS_TEXT.label_posx, C3X_IMM_F32(3)));
    p->ctx.BONS_TEXT.value_shadow->posx = C3X_STF(p->ctx.BONS_TEXT.value_posx);
    p->ctx.BONS_TEXT.value_front->posx = C3X_STF(C3X_ADD(p->ctx.BONS_TEXT.value_posx, C3X_IMM_F32(3)));
    SLEEP(1, 3);
    p->ctx.BONS_TEXT.loop_count -= 1;
    if (p->ctx.BONS_TEXT.loop_count >= 0) {
        goto BONS_TEXT_OUT;
    }
    DIE();
}

static void BONS_RECORDTIME(PROC* p) {
    tSHADOW_TEXT text;
    RACEENTRY entry;
    word_addr_t address;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }
    // asm 00003E60: 	SLEEP	5
    SLEEP(5, 1);
    // asm 00003E62: 	LDL	RT2,AR2
    // asm 00003E63: 	FLOAT	0,R2
    // asm 00003E64: 	FLOAT	120,R3
    // asm 00003E65: 	LDI	999,RC
    // asm 00003E66: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(RT2, C3X_FROM_INT(0), C3X_FROM_INT(120), 999);
    // asm 00003E67: 	CALL	SET18FONTDS
    SET18FONTDS(&text);
    // asm 00003E68: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 00003E69: 	OR	TXT_RIGHT,R0
    // asm 00003E6A: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 00003E6B: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 00003E6C: 	OR	TXT_RIGHT,R0
    // asm 00003E6D: 	STI	R0,*+AR1(TEXT_COLOR)
    text.front->color |= TXT_RIGHT;
    text.shadow->color |= TXT_RIGHT;
    // asm 00003E6E: 	STI	AR0,*+AR7(PDATA+10)
    // asm 00003E6F: 	STI	AR1,*+AR7(PDATA+11)
    p->ctx.BONS_TEXT.label_front = text.front;
    p->ctx.BONS_TEXT.label_shadow = text.shadow;
    // asm 00003E70: 	LDI	@BONUS_WAVE,R6
    // asm 00003E71: 	DEC	R6
    // asm 00003E72: 	LDI	0,R7
    // asm 00003E73: 	CALL	GET_TABLE_ADDR
    address = GET_TABLE_ADDR(BONUS_WAVE - 1, 0);
    // asm 00003E74: 	CALL	TABLE_ENTRY_READ
    entry = TABLE_ENTRY_READ(&address);
    // 	;
    // 	;R0	time code
    // 	;R1	init 1
    // 	;R2	init 2
    // 	;R3	init 3
    // 	;
    // asm 00003E75: 	LDI	AR7,AR1
    // asm 00003E76: 	ADDI	PDATA+14,AR1
    // asm 00003E77: 	AND	0FFh,R1
    // asm 00003E78: 	AND	0FFh,R2
    // asm 00003E79: 	AND	0FFh,R3
    // asm 00003E7A: 	LS	8,R2
    // asm 00003E7B: 	OR	R2,R1
    // asm 00003E7C: 	LS	16,R3
    // asm 00003E7D: 	OR	R3,R1
    // asm 00003E7E: 	LDI	' ',R2
    // asm 00003E7F: 	LS	24,R2
    // asm 00003E80: 	OR	R2,R1
    // asm 00003E81: 	STI	R1,*AR1
    // asm 00003E82: 	LDI	' ',R2
    // asm 00003E83: 	STI	R2,*+AR1
    bonus_recordtime[0] = (char)(entry.init1 & 0xff);
    bonus_recordtime[1] = (char)(entry.init2 & 0xff);
    bonus_recordtime[2] = (char)(entry.init3 & 0xff);
    bonus_recordtime[3] = ' ';
    bonus_recordtime[4] = ' ';
    // asm 00003E84: 	LDI	AR7,AR2
    // asm 00003E85: 	ADDI	PDATA,AR2
    // asm 00003E86: 	CALL	TIME2STR
    TIME2STR(&bonus_recordtime[5], (int)entry.time);
    // asm 00003E87: 	LDI	AR1,AR0
    // asm 00003E88: 	LDI	AR2,AR1
    // asm 00003E89: 	LDI	AR0,AR2
    // asm 00003E8A: 	CALL	STRCAT		;AR1 from above
    // asm 00003E8B: 	FLOAT	512,R2
    // asm 00003E8C: 	FLOAT	120,R3
    // asm 00003E8D: 	LDI	999,RC
    // asm 00003E8E: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(bonus_recordtime, C3X_FROM_INT(512), C3X_FROM_INT(120), 999);
    // asm 00003E8F: 	CALL	SET18FONTDS
    SET18FONTDS(&text);
    // asm 00003E90: 	STI	AR0,*+AR7(PDATA+12)
    // asm 00003E91: 	STI	AR1,*+AR7(PDATA+13)
    p->ctx.BONS_TEXT.value_front = text.front;
    p->ctx.BONS_TEXT.value_shadow = text.shadow;
    // asm 00003E92: 	BU	ENTER_HERE
    JSRP(BONS_TEXT_ANIMATE, 2);
    return;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

static const char HH1[] = "NEW RECORD TIME";
static const char HH2[] = "NEW HOT TIME";

static void BONS_HOTTIME_REC(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }
    // asm 00003E93: 	CLRI	R4			;ON OFF TOGGLE
    p->ctx.BONS_HOTTIME.toggle = 0; // ;ON OFF TOGGLE
    // asm 00003E94: 	SLEEP	5
    SLEEP(5, 1);
    // asm 00003E96: 	LDL	HH1,AR2
    p->ctx.BONS_HOTTIME.string = HH1;
    // asm 00003E97: 	BU	BLAHB
    PROC_CONTINUE(BONS_HOTTIME_RUN, 2);
    return;
}

static void BONS_HOTTIME(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }
    // asm 00003E98: 	CLRI	R4			;ON OFF TOGGLE
    p->ctx.BONS_HOTTIME.toggle = 0; // ;ON OFF TOGGLE
    // asm 00003E99: 	SLEEP	5
    SLEEP(5, 1);
    // asm 00003E9B: 	LDL	HH2,AR2
    p->ctx.BONS_HOTTIME.string = HH2;
BLAHB:
    // asm 00003E9C: STI	AR2,*+AR7(PDATA)
    // asm 00003E9D: 	FLOAT	256,R2
    // asm 00003E9E: 	FLOAT	(150+300),R3
    // asm 00003E9F: 	LDI	999,RC
    // asm 00003EA0: 	CALL	TEXT_ADDDS
    PROC_CONTINUE(BONS_HOTTIME_RUN, 2);
    return;
    // asm 00003EA1: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 00003EA2: 	OR	TXT_CENTER,R0
    // asm 00003EA3: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 00003EA4: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 00003EA5: 	OR	TXT_CENTER,R0
    // asm 00003EA6: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm 00003EA7: 	LDI	AR0,AR4
    // asm 00003EA8: 	LDI	AR1,AR5
    // asm 00003EA9: 	LDL	font18_white,AR2
    // asm 00003EAA: 	CALL	PAL_FIND_RAW
    // asm 00003EAB: 	STI	R0,*+AR4(TEXT_PAL)
    // asm 00003EAC: 	STI	R0,*+AR5(TEXT_PAL)
BONHTLP:
    // asm 00003EAD: 	LDF	*+AR4(TEXT_POSY),R0
    // asm 00003EAE: 	FLOAT	152,R1
    // asm 00003EAF: 	LDF	R1,R3
    // asm 00003EB0: 	SUBF	R0,R1,R1
    // asm 00003EB1: 	MPYF	0.1,R1
    // asm 00003EB2: 	ADDF	R0,R1
    // asm 00003EB3: 	CMPF	R3,R1
    // asm 00003EB4: 	LDFLT	R3,R1
    // asm 00003EB5: 	STF	R1,*+AR4(TEXT_POSY)
    // asm 00003EB6: 	LDF	*+AR5(TEXT_POSY),R0
    // asm 00003EB7: 	FLOAT	150,R1
    // asm 00003EB8: 	LDF	R1,R3
    // asm 00003EB9: 	SUBF	R0,R1,R1
    // asm 00003EBA: 	MPYF	0.1,R1
    // asm 00003EBB: 	ADDF	R0,R1
    // asm 00003EBC: 	CMPF	R3,R1
    // asm 00003EBD: 	LDFLT	R3,R1
    // asm 00003EBE: 	STF	R1,*+AR5(TEXT_POSY)
    // asm 00003EBF: 	CALL	TEXTTOG
    // asm 00003EC0: 	SLEEP	1
    // asm 00003EC2: 	LDI	@DO_FOLDFLAG,R0
    // asm 00003EC3: 	BZ	BONHTLP
BONHTLP2:
    // asm 00003EC4: 	LDF	*+AR4(TEXT_POSY),R0
    // asm 00003EC5: 	FLOAT	152+300,R1
    // asm 00003EC6: 	LDF	R1,R3
    // asm 00003EC7: 	SUBF	R0,R1,R1
    // asm 00003EC8: 	MPYF	0.1,R1
    // asm 00003EC9: 	ADDF	R0,R1
    // asm 00003ECA: 	CMPF	R3,R1
    // asm 00003ECB: 	LDFGT	R3,R1
    // asm 00003ECC: 	STF	R1,*+AR4(TEXT_POSY)
    // asm 00003ECD: 	LDF	*+AR5(TEXT_POSY),R0
    // asm 00003ECE: 	FLOAT	150+300,R1
    // asm 00003ECF: 	LDF	R1,R3
    // asm 00003ED0: 	SUBF	R0,R1,R1
    // asm 00003ED1: 	MPYF	0.1,R1
    // asm 00003ED2: 	ADDF	R0,R1
    // asm 00003ED3: 	CMPF	R3,R1
    // asm 00003ED4: 	LDFGT	R3,R1
    // asm 00003ED5: 	STF	R1,*+AR5(TEXT_POSY)
    // asm 00003ED6: 	SLEEP	1
    // asm 00003ED8: 	BU	BONHTLP2
BADA:;
    // asm 00003ED9: LDI	1,R0
    // asm 00003EDA: 	STI	R0,*+AR4(TEXT_TIKS)
    // asm 00003EDB: 	STI	R0,*+AR5(TEXT_TIKS)
    // asm 00003EDC: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

static void BONS_HOTTIME_RUN(PROC* p) {
    tSHADOW_TEXT text;
    c3x_reg_t position;
    int palette;

    switch (PROC_RESUME_STATE) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

    text = TEXT_ADDDS(p->ctx.BONS_HOTTIME.string, C3X_FROM_INT(256), C3X_FROM_INT(450), 999);
    text.front->color |= TXT_CENTER;
    text.shadow->color |= TXT_CENTER;
    p->ctx.BONS_HOTTIME.front = text.front;
    p->ctx.BONS_HOTTIME.shadow = text.shadow;
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(font18_white_ROM));
    text.front->palette = (u32)palette;
    text.shadow->palette = (u32)palette;
BONS_HOTTIME_IN:
    position = C3X_ADD(C3X_LDF(p->ctx.BONS_HOTTIME.front->posy),
        C3X_MUL(C3X_SUB(C3X_FROM_INT(152), C3X_LDF(p->ctx.BONS_HOTTIME.front->posy)), C3X_IMM_F32(0.1)));
    if (C3X_LT(position, C3X_FROM_INT(152)))
        position = C3X_FROM_INT(152);
    p->ctx.BONS_HOTTIME.front->posy = C3X_STF(position);
    position = C3X_ADD(C3X_LDF(p->ctx.BONS_HOTTIME.shadow->posy),
        C3X_MUL(C3X_SUB(C3X_FROM_INT(150), C3X_LDF(p->ctx.BONS_HOTTIME.shadow->posy)), C3X_IMM_F32(0.1)));
    if (C3X_LT(position, C3X_FROM_INT(150)))
        position = C3X_FROM_INT(150);
    p->ctx.BONS_HOTTIME.shadow->posy = C3X_STF(position);
    TEXTTOG(&p->ctx.BONS_HOTTIME.toggle, p->ctx.BONS_HOTTIME.front,
        p->ctx.BONS_HOTTIME.shadow, p->ctx.BONS_HOTTIME.string);
    SLEEP(1, 1);
    if (DO_FOLDFLAG == 0)
        goto BONS_HOTTIME_IN;
BONS_HOTTIME_OUT:
    p->ctx.BONS_HOTTIME.front->posy = C3X_STF(C3X_ADD(C3X_LDF(p->ctx.BONS_HOTTIME.front->posy),
        C3X_MUL(C3X_SUB(C3X_FROM_INT(452), C3X_LDF(p->ctx.BONS_HOTTIME.front->posy)), C3X_IMM_F32(0.1))));
    p->ctx.BONS_HOTTIME.shadow->posy = C3X_STF(C3X_ADD(C3X_LDF(p->ctx.BONS_HOTTIME.shadow->posy),
        C3X_MUL(C3X_SUB(C3X_FROM_INT(450), C3X_LDF(p->ctx.BONS_HOTTIME.shadow->posy)), C3X_IMM_F32(0.1))));
    SLEEP(1, 2);
    goto BONS_HOTTIME_OUT;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void TEXTTOG(int* toggle, tTEXT* front, tTEXT* shadow, const char* string) {
    // asm 00003EDD: 	ADDI	@NFRAMES,R4
    *toggle += NFRAMES;
    // asm 00003EDE: 	CMPI	30,R4
    // asm 00003EDF: 	BLT	ISOFF
    if (*toggle < 30)
        goto ISOFF;
    // asm 00003EE0: 	CMPI	35,R4
    // asm 00003EE1: 	BLT	GAGA
    if (*toggle < 35)
        goto GAGA;
    // asm 00003EE2: 	CLRI	R4
    *toggle = 0;
GAGA:
    // asm 00003EE3: 	LDL	NULLSTR5,R0
    // asm 00003EE4: 	BU	IBOIBO
    string = NULLSTR5;
    goto IBOIBO;
ISOFF:
    // asm 00003EE5: LDI	*+AR7(PDATA),R0
IBOIBO:
    // asm 00003EE6: STI	R0,*+AR4(TEXT_PTR)
    // asm 00003EE7: 	STI	R0,*+AR5(TEXT_PTR)
    front->ptr = (char*)string;
    shadow->ptr = (char*)string;
    // asm 00003EE8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXTTOG", 0, 0);
}

static const char NULLSTR5[] = " ";
// *----------------------------------------------------------------------------

static const char BT2[] = "ELAPSED TIME:";

static void BONS_ETIME(PROC* p) {
    tSHADOW_TEXT text;
    int palette;
    const char* value_string;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }
    // asm 00003EE9: 	SLEEP	5
    SLEEP(5, 1);
    // asm 00003EEB: 	LDL	BT2,AR2
    // asm 00003EEC: 	FLOAT	0,R2
    // asm 00003EED: 	FLOAT	50,R3
    // asm 00003EEE: 	LDI	999,RC
    // asm 00003EEF: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(BT2, C3X_FROM_INT(0), C3X_FROM_INT(50), 999);
    // asm 00003EF0: 	CALL	SET18FONTDS
    SET18FONTDS(&text);
    // asm 00003EF1: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 00003EF2: 	OR	TXT_RIGHT,R0
    // asm 00003EF3: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 00003EF4: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 00003EF5: 	OR	TXT_RIGHT,R0
    // asm 00003EF6: 	STI	R0,*+AR1(TEXT_COLOR)
    text.front->color |= TXT_RIGHT;
    text.shadow->color |= TXT_RIGHT;
    // asm 00003EF7: 	STI	AR0,*+AR7(PDATA+10)
    // asm 00003EF8: 	STI	AR1,*+AR7(PDATA+11)
    p->ctx.BONS_TEXT.label_front = text.front;
    p->ctx.BONS_TEXT.label_shadow = text.shadow;
    // asm 00003EF9: 	LDI	@DID_TIMED_OUT,R0
    // asm 00003EFA: 	BZ	DOREG3
    if (DID_TIMED_OUT == 0) {
        goto DOREG3;
    }
    // asm 00003EFB: 	LDI	0,R0		;ELAPSED TIME OF 0 = DID NOT FINISH
    // asm 00003EFC: 	STI	R0,@ETIME
    ETIME = 0; // ;ELAPSED TIME OF 0 = DID NOT FINISH
    // asm 00003EFD: 	LDL	EXPIRED,AR2
    value_string = EXPIRED;
    // asm 00003EFE: 	BU	LREG3
    goto LREG3;
DOREG3:
    // asm 00003EFF: LDI	@STOPWATCH,R0
    // asm 00003F00: 	STI	R0,@ETIME
    ETIME = STOPWATCH;
    // asm 00003F01: 	LDI	AR7,AR2
    // asm 00003F02: 	ADDI	PDATA,AR2
    // asm 00003F03: 	CALL	TIME2STR
    TIME2STR(p->ctx.BONS_TEXT.buffer, ETIME);
    value_string = p->ctx.BONS_TEXT.buffer;
LREG3:
    // asm 00003F04: 	FLOAT	512,R2
    // asm 00003F05: 	FLOAT	50,R3
    // asm 00003F06: 	LDI	999,RC
    // asm 00003F07: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(value_string, C3X_FROM_INT(512), C3X_FROM_INT(50), 999);
    // asm 00003F08: 	CALL	SET18FONTDS
    SET18FONTDS(&text);
    // asm 00003F09: 	STI	AR0,*+AR7(PDATA+12)
    // asm 00003F0A: 	STI	AR1,*+AR7(PDATA+13)
    p->ctx.BONS_TEXT.value_front = text.front;
    p->ctx.BONS_TEXT.value_shadow = text.shadow;
    // asm 00003F0B: 	LDL	font18_white,AR2
    // asm 00003F0C: 	CALL	PAL_FIND_RAW
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(font18_white_ROM));
    // asm 00003F0D: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00003F0E: 	STI	R0,*+AR1(TEXT_PAL)
    text.front->palette = (u32)palette;
    text.shadow->palette = (u32)palette;
    // asm 00003F0F: 	CLRI	R0
    // asm 00003F10: 	STI	R0,@STOPWATCH
    STOPWATCH = 0;
    // asm 00003F11: 	BU	ENTER_HERE
    JSRP(BONS_TEXT_ANIMATE, 2);
    return;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

static const char BT3[] = "POSITION:";

static void DRAW_BONS_POSITION(int position_index, int ypos) {
    u32 score_model;
    u32 suffix_model;
    int score_x_offset;

    switch (position_index) {
    case 0:
        score_model = scred1_ROM;
        suffix_model = dst_ROM;
        score_x_offset = 7;
        break;
    case 1:
        score_model = scred2_ROM;
        suffix_model = dnd_ROM;
        score_x_offset = 0;
        break;
    case 2:
        score_model = scred3_ROM;
        suffix_model = drd_ROM;
        score_x_offset = 0;
        break;
    case 3:
        score_model = scred4_ROM;
        suffix_model = dth_ROM;
        score_x_offset = 0;
        break;
    case 4:
        score_model = scred5_ROM;
        suffix_model = dth_ROM;
        score_x_offset = 0;
        break;
    case 5:
        score_model = scred6_ROM;
        suffix_model = dth_ROM;
        score_x_offset = 0;
        break;
    case 6:
        score_model = scred7_ROM;
        suffix_model = dth_ROM;
        score_x_offset = 0;
        break;
    case 7:
        score_model = scred8_ROM;
        suffix_model = dth_ROM;
        score_x_offset = 0;
        break;
    case 8:
        score_model = scred9_ROM;
        suffix_model = dth_ROM;
        score_x_offset = 0;
        break;
    default:
        score_model = scred10_ROM;
        suffix_model = dth_ROM;
        score_x_offset = -20;
        break;
    }
    BLTMOD2D_DS((const BLTMOD2D_MODEL*)ROM_PTR(score_model), 220 + score_x_offset, ypos, TM | ZS);
    BLTMOD2D_DS((const BLTMOD2D_MODEL*)ROM_PTR(suffix_model), 220 + 35, ypos, TM | ZS);
}

static void BONS_POSITION(PROC* p) {
    c3x_reg_t ypos;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    }
    // asm 00003F12: 	LDI	@POSITION,AR4
    // asm 00003F13: 	DEC	AR4
    // asm 00003F14: 	MPYI	3,AR4
    // asm 00003F15: 	ADDI	@POS_TABLEI,AR4
    p->ctx.BONS_POSITION.position_index = POSITION - 1;
    // asm 00003F16: 	LDI	-60,R5		;YPOS
    p->ctx.BONS_POSITION.ypos = -60; // ;YPOS
    // asm 00003F17: 	LDI	40,AR6
    p->ctx.BONS_POSITION.loop_count = 40;
    // asm 00003F18: BPL1
BPL1:
    // asm 00003F18: 	FLOAT	R5,R0
    // asm 00003F19: 	LDF	R0,R1
    // asm 00003F1A: 	FLOAT	138-60,R2
    // asm 00003F1B: 	SUBRF	R2,R0
    // asm 00003F1C: 	MPYF	0.2,R0
    // asm 00003F1D: 	ADDF	R1,R0
    // asm 00003F1E: 	FIX	R0,R5
    ypos = C3X_ADD(C3X_FROM_INT(p->ctx.BONS_POSITION.ypos),
        C3X_MUL(C3X_SUB(C3X_FROM_INT(78), C3X_FROM_INT(p->ctx.BONS_POSITION.ypos)), C3X_IMM_F32(0.2)));
    p->ctx.BONS_POSITION.ypos = C3X_FIX(ypos);
    // asm 00003F1F: 	LDI	*AR4,AR2
    // asm 00003F20: 	LDI	220,R2
    // asm 00003F21: 	ADDI	*+AR4(2),R2
    // asm 00003F22: 	LDI	R5,R3
    // asm 00003F23: 	LDI	TM|ZS,R4
    // asm 00003F24: 	CALL	BLTMOD2D_DS
    // asm 00003F25: 	LDI	*+AR4(1),AR2
    // asm 00003F26: 	LDI	220+35,R2
    // asm 00003F27: 	LDI	R5,R3
    // asm 00003F28: 	LDI	TM|ZS,R4
    // asm 00003F29: 	CALL	BLTMOD2D_DS
    DRAW_BONS_POSITION(p->ctx.BONS_POSITION.position_index, p->ctx.BONS_POSITION.ypos);
    // asm 00003F2A: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00003F2C: 	DBU	AR6,BPL1
    p->ctx.BONS_POSITION.loop_count -= 1;
    if (p->ctx.BONS_POSITION.loop_count >= 0)
        goto BPL1;
    // asm 00003F2D: 	LDI	158,AR6
BPL2:
    // asm 00003F2E: 	LDI	*AR4,AR2
    // asm 00003F2F: 	LDI	220,R2
    // asm 00003F30: 	ADDI	*+AR4(2),R2
    // asm 00003F31: 	LDI	R5,R3
    // asm 00003F32: 	LDI	TM|ZS,R4
    // asm 00003F33: 	CALL	BLTMOD2D_DS
    // asm 00003F34: 	LDI	*+AR4(1),AR2
    // asm 00003F35: 	LDI	220+35,R2
    // asm 00003F36: 	LDI	R5,R3
    // asm 00003F37: 	LDI	TM|ZS,R4
    // asm 00003F38: 	CALL	BLTMOD2D_DS
    DRAW_BONS_POSITION(p->ctx.BONS_POSITION.position_index, p->ctx.BONS_POSITION.ypos);
    // asm 00003F39: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00003F3B: 	LDI	@DO_FOLDFLAG,R0
    // asm 00003F3C: 	BZ	BPL2
    if (DO_FOLDFLAG == 0)
        goto BPL2;
    // asm 00003F3D: 	LDI	40,AR6
    p->ctx.BONS_POSITION.loop_count = 40;
    // asm 00003F3E: BPL3
BPL3:
    // asm 00003F3E: 	FLOAT	R5,R0
    // asm 00003F3F: 	LDF	R0,R1
    // asm 00003F40: 	SUBRF	-60,R0
    // asm 00003F41: 	MPYF	0.2,R0
    // asm 00003F42: 	ADDF	R1,R0
    // asm 00003F43: 	FIX	R0,R5
    ypos = C3X_ADD(C3X_FROM_INT(p->ctx.BONS_POSITION.ypos),
        C3X_MUL(C3X_SUB(C3X_FROM_INT(-60), C3X_FROM_INT(p->ctx.BONS_POSITION.ypos)), C3X_IMM_F32(0.2)));
    p->ctx.BONS_POSITION.ypos = C3X_FIX(ypos);
    // asm 00003F44: 	LDI	*AR4,AR2
    // asm 00003F45: 	LDI	220,R2
    // asm 00003F46: 	ADDI	*+AR4(2),R2
    // asm 00003F47: 	LDI	R5,R3
    // asm 00003F48: 	LDI	TM|ZS,R4
    // asm 00003F49: 	CALL	BLTMOD2D_DS
    // asm 00003F4A: 	LDI	*+AR4(1),AR2
    // asm 00003F4B: 	LDI	220+35,R2
    // asm 00003F4C: 	LDI	R5,R3
    // asm 00003F4D: 	LDI	TM|ZS,R4
    // asm 00003F4E: 	CALL	BLTMOD2D_DS
    DRAW_BONS_POSITION(p->ctx.BONS_POSITION.position_index, p->ctx.BONS_POSITION.ypos);
    // asm 00003F4F: 	SLEEP	1
    SLEEP(1, 3);
    // asm 00003F51: 	DBU	AR6,BPL3
    p->ctx.BONS_POSITION.loop_count -= 1;
    if (p->ctx.BONS_POSITION.loop_count >= 0)
        goto BPL3;
    // asm 00003F52: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void KILL_THEM(void) {
    // 	;KILL ALL DRONES
    // asm 00003F53: 	LDI	DRONE_C,R0
    // asm 00003F54: 	LDI	CLASS_M,R1
    // asm 00003F55: 	CALL	PRC_KILLALL
    PRC_KILLALL(DRONE_C, CLASS_M);
    // 	;TRAFFIC
    // asm 00003F56: 	LDI	SPAWNER_C,R0
    // asm 00003F57: 	LDI	CLASS_M,R1
    // asm 00003F58: 	CALL	PRC_KILLALL
    PRC_KILLALL(SPAWNER_C, CLASS_M);
    // 	;WAVEFLAG, MONKEYs
    // asm 00003F59: 	LDI	UTIL_C|MONKEY_T,R0
    // asm 00003F5A: 	LDI	CLASS_M|TYPE_M,R1
    // asm 00003F5B: 	CALL	PRC_KILLALL
    PRC_KILLALL(UTIL_C | MONKEY_T, CLASS_M | TYPE_M);
    // 	;LBACK_WATCH
    // asm 00003F5C: 	LDI	UTIL_C|BACKGRND_T,R0
    // asm 00003F5D: 	LDI	-1,R1
    // asm 00003F5E: 	CALL	PRC_KILLALL
    PRC_KILLALL(UTIL_C | BACKGRND_T, -1);
    // asm 00003F5F: 	CALL	DELETE_SPLAT
    DELETE_SPLAT();
    // asm 00003F60: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILL_THEM", 0, 0);
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FIND_AND_REACTIVATE(void) {
    OBJ* obj;

    // asm 00003F61: 	PUSH	AR0
    // asm 00003F62: 	PUSH	AR4
    // asm 00003F63: 	LDI	@OACTIVE,AR0
    obj = OACTIVE;
FARLP:
    // asm 00003F64: LDI	*AR0,R0
    // asm 00003F65: 	BZ	FARX
    if (obj == NULL) {
        goto FARX;
    }
    // asm 00003F66: 	LDI	R0,AR0
    // asm 00003F67: 	LDI	*+AR0(OID),R0
    // asm 00003F68: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_DUDE_S,R0
    // asm 00003F69: 	BNE	NOTRUT
    if (obj->id != (RDDEBRIS_C | TSC_IGNORE | TSC_DUDE_S)) {
        goto NOTRUT;
    }
    // asm 00003F6A: 	LDI	AR0,AR4
    // asm 00003F6B: 	CALL	RUT_ANI
    RUT_ANI(obj);
    // asm 00003F6C: 	BU	FARLP
    obj = obj->link;
    goto FARLP;
NOTRUT:
    // asm 00003F6D: CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_BABE_S,R0
    // asm 00003F6E: 	BNE	FARLP
    if (obj->id != (RDDEBRIS_C | TSC_IGNORE | TSC_BABE_S)) {
        obj = obj->link;
        goto FARLP;
    }
    // asm 00003F6F: 	LDI	AR0,AR4
    // asm 00003F70: 	CALL	HUNGH_ANI_REENTER
    HUNGH_ANI_REENTER(obj);
    // asm 00003F71: 	BU	FARLP
    obj = obj->link;
    goto FARLP;
FARX:
    // asm 00003F72: 	POP	AR4
    // asm 00003F73: 	POP	AR0
    // asm 00003F74: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_AND_REACTIVATE", 0, 0);
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void KILL_THE_REANIMATORS(void) {
    // asm 00003F75: 	LDI	SPAWNER_C|ANIMATION_T|7,R0
    // asm 00003F76: 	LDI	-1,R1
    // asm 00003F77: 	CALL	PRC_KILLALL
    PRC_KILLALL(SPAWNER_C | ANIMATION_T | 7, -1);
    // asm 00003F78: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILL_THE_REANIMATORS", 0, 0);
}

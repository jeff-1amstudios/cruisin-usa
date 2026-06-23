#include "wave.h"
#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "cmos.h"
#include "comm.h"
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
#include "vunit.h"

/*
 * Source module: asm/WAVE.ASM
 */

void WAVE(void);
static void HEAD2HEAD_WAIT(void);
static void HIGH_SCORE(void);
static void MIDSPIN(void);
static void MIDSPINHS(void);
static void RACELEG(void);
#define HIGH_SCORE_INI THANKS
static void THANKS(void);
static void LOAD_HIGH_SCORE(void);
static void BEGIN_GAME(void);
void INIT_SYSTEM(void);
void LOAD_FIXED_PALETTES(void);
void LOAD_STARTUP_PALS(void);
static void LOAD_VARIOUS_PALETTES(void);

#define HIGH_SCORE_INI THANKS
#define VEHICLE_TABLEI VEHICLE_TABLE

extern int OLD_BUTTON_STATUS;
void SPIN_CAR(void);

static VEHTAB VEHICLE_TABLE[18];
static int CVETTEPAL[4];
static int HOTRODPAL[3];
static int BULLETPAL[4];
static int TESTORPAL[3];
static int GTRUCKPAL[2];
static int DDYNA_GTRUCK[14];
static int NOPAL;
static int DDYNA_FTRUCK[10];
static int DDYNA_CBUS[14];
static int DDYNA_COPCAR[10];
static int MUSCLEPAL[3];
static int DDYNA_MUSCLE[10];
static int CARAVANPAL[3];
static int DDYNA_CARAVAN[10];
static int DDYNA_SBUS[10];
static int PTRUCKGPAL[3];
static int DDYNA_PTRUCKG[10];
static int DDYNA_MUSTANG[10];
static int JEEPPAL[3];

/*
 *----------------------------------------------------------------------------
 *
 *
 *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

/* asm: TEASE_COUNT	pbss	TEASE_COUNT,1 */
int TEASE_COUNT;
/* asm: ATTR_MODEL	pbss	ATTR_MODEL,1 */
int ATTR_MODEL;
/* asm: _ATTR_MODE	pbss	_ATTR_MODE,1 */
int _ATTR_MODE;
/* asm: LOADED	pbss	LOADED,1 */
int LOADED;

/*
 *----------------------------------------------------------------------------
 *PARAMETERS
 *	AR2	WAVE INDEX
 *		<  0 -> ATTRACT MODE N
 *		>= 0 -> REGULAR GAME
 *
 */
void WAVE(void) {
    // asm 00009307: 	POP	AR7	;return addr
    // asm 00009308: 	CALL	SILENT
    // 	;CLEAR ALL RAM AND RELOAD CODE
    // 	;
    // asm 00009309: 	CLRI	R0			;CLEAR INTERNAL RAM
    // asm 0000930A: 	LDIL	RAM0,AR0
    // asm 0000930D: 	RPTS	2047
    // asm 0000930E: 	STI	R0,*AR0++
    // asm 0000930F: 	CALL	COMM_ENABLE_INT2
    // asm 00009310: 	LDI	@COUNTER_IDX,R0
    // asm 00009311: 	PUSH	R0
    // asm 00009312: 	LDI	@COUNTER_MODE,R0
    // asm 00009313: 	PUSH	R0
    // asm 00009314: 	LDI	@COIN_COUNTER1,R0
    // asm 00009315: 	PUSH	R0
    // asm 00009316: 	LDI	@COIN_COUNTER2,R0
    // asm 00009317: 	PUSH	R0
    // asm 00009318: 	CALL	CLR_RAM			;CLEAR BSS SPACE
    // asm 00009319: 	POP	R0
    // asm 0000931A: 	STI	R0,@COIN_COUNTER2
    // asm 0000931B: 	POP	R0
    // asm 0000931C: 	STI	R0,@COIN_COUNTER1
    // asm 0000931D: 	POP	R0
    // asm 0000931E: 	STI	R0,@COUNTER_MODE
    // asm 0000931F: 	POP	R0
    // asm 00009320: 	STI	R0,@COUNTER_IDX
    // 	;RELOAD GAME CODE
    // asm 00009321: 	LDI	0,AR1			;SOURCE ADDRESS
    // asm 00009322: 	LDI	4000h,AR3		;DESINATION ADDRESS
    // asm 00009323: 	LS	8,AR3
    // asm 00009324: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm 00009325: 	LS	4,RC			;FAST RAM
    // asm 00009326: 	RPTB	LD_RAM
    // asm 00009327: 	LDI	*AR1++,R0
LD_RAM:
    // asm 00009328: STI	R0,*AR3++
    // asm 00009329: 	PUSH	AR7
    // asm 0000932A: 	PUSH	AR2
#if DEBUG
    // asm: 	CALL	VERIFY_CODE_INTEGRITY
#endif
    // asm 0000932B: 	CALL	CLRONE	;CAN NOW BE DUAL PLAYER
    // asm 0000932C: 	CALL	INIT_SYSTEM
    // asm 0000932D: 	LDP	@_CAMERARAD
    // asm 0000932E: 	CLRF	R0
    // asm 0000932F: 	STF	R0,@_CAMERARAD+X
    // asm 00009330: 	STF	R0,@_CAMERARAD+Y
    // asm 00009331: 	STF	R0,@_CAMERARAD+Z
    // asm 00009332: 	STF	R0,@_CAMERAPOS+X
    // asm 00009333: 	STF	R0,@_CAMERAPOS+Y
    // asm 00009334: 	STF	R0,@_CAMERAPOS+Z
    // asm 00009335: 	STF	R0,@_LIGHT+Z
    // asm 00009336: 	LDF	0.707,R0
    // asm 00009337: 	STF	R0,@_LIGHT+X
    // asm 00009338: 	STF	R0,@_LIGHT+Y
    // asm 00009339: 	SETDP
    // asm 0000933A: 	CALL	INIT_CUSTOM_COIN	;Set the CUSTOM SETUP in RAM
    // asm 0000933B: 	LDI	@CAMERAMATRIXI,AR0
    // asm 0000933C: 	CALL	INITMAT
    // asm 0000933D: 	FLOAT	35,R0
    // asm 0000933E: 	STF	R0,@INFIN_CORRECT
    // asm 0000933F: 	LDI	1,R0
    // asm 00009340: 	STI	R0,@CLEARRDY
    // asm 00009341: 	READADJ	ADJ_STEERCENTER
    // asm 00009343: 	FLOAT	R0
    // asm 00009344: 	STF	R0,@WHEELPOS
    // asm 00009345: 	CLRF	R0
    // asm 00009346: 	STF	R0,@WHEELPWR
    // asm 00009347: 	STF	R0,@WHEELVEL
    // asm 00009348: 	CLRI	R0
    // asm 00009349: 	STI	R0,@COINOFF
    // asm 0000934A: 	STI	R0,@NOLONG_VEHICLES
    // asm 0000934B: 	CALL	LOAD_FIXED_PALETTES
    // asm 0000934C: 	CREATE	SCAN_OBJECTS,UTIL_C
    // asm 0000934F: 	POP	AR2
    // asm 00009350: 	CMPI	1,AR2
    // asm 00009351: 	BEQ	BEGIN_GAME
    // asm 00009352: 	CLRI	AR0
    // asm 00009353: 	LDP	@SWITCH3
    // asm 00009354: 	NOT	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // asm 00009355: 	LDI	*AR0,R2			;Loff
    // asm 00009356: 	SETDP
    // asm 00009357: 	AND	SW_VIEW0_H|SW_VIEW1_H|SW_VIEW2_H,R0
    // asm 00009358: 	CMPI	SW_VIEW1_H|SW_VIEW2_H,R0
    // asm 00009359: 	BEQ	CREDITS
    // asm 0000935A: 	LDI	AR2,AR0
    // asm 0000935B: 	ADDI	@_ATTR_WAVETABI,AR0
    // asm 0000935C: 	LDI	*AR0,R0
    // asm 0000935D: 	CALLU	R0
    // asm 0000935E: 	RETS
    // asm 00009367: _ATTR_WAVETAB
HEAD2HEADWATCH:
    // asm 00009367: 	SLEEP	1
    // asm 00009369: 	LDI	@OM_MODE,R0
    // asm 0000936A: 	AND	MMODE,R0
    // asm 0000936B: 	CMPI	MBONUS,R0
    // asm 0000936C: 	BEQ	ISTRUE
    // asm 0000936D: 	CMPI	MINIT,R0
    // asm 0000936E: 	BEQ	ISTRUE
    // asm 0000936F: 	CMPI	MINSERT_COINS,R0
    // asm 00009370: 	BEQ	ISTRUE
    // asm 00009371: 	LDI	@OM_LINKWAIT,R0
    // asm 00009372: 	BZ	HEAD2HEADWATCH
ISTRUE:
    // asm 00009373: LDI	-7,R0
    // asm 00009374: 	STI	R0,@_ATTR_MODE
    // asm 00009375: 	BR	SET_ATTR
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAVE", 0, 0);
    UNIMPL();
}

static void HEAD2HEAD_WAIT(void) {
    // asm 00009376: 	LDI	@BUTTON_STATUS,R0
    // asm 00009377: 	ANDN	BUT_VIEWS,R0
    // asm 00009378: 	STI	R0,@BUTTON_STATUS
    // asm 00009379: 	CREATE	HEAD2HEAD_LOGO_WAIT,UTIL_C
    // asm 0000937C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEAD_WAIT", 0, 0);
    UNIMPL();
}

static void HIGH_SCORE(void) {
    // asm 0000937D: 	LDI	@BUTTON_STATUS,R0
    // asm 0000937E: 	ANDN	BUT_VIEWS,R0
    // asm 0000937F: 	STI	R0,@BUTTON_STATUS
    // asm 00009380: 	LDI	-1,R0	;Kick start the light routine
    // asm 00009381: 	STI	R0,@OLD_BUTTON_STATUS
    // asm 00009382: 	LDI	MATTR|MHS,R0
    // asm 00009383: 	STI	R0,@_MODE
    // asm 00009384: 	CREATE	DISPLAY_HIGH_SCORES,UTIL_C
    // asm 00009387: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm 0000938A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIGH_SCORE", 0, 0);
    UNIMPL();
}

static void MIDSPIN(void) {
    // asm 0000938B: 	LDI	@BUTTON_STATUS,R0
    // asm 0000938C: 	ANDN	BUT_VIEWS,R0
    // asm 0000938D: 	STI	R0,@BUTTON_STATUS
    // asm 0000938E: 	CLRI	R0
    // asm 0000938F: 	LDI	MATTR,R0
    // asm 00009390: 	STI	R0,@_MODE
    // asm 00009391: 	CALL	LOAD_ATTR_LEG
    // asm 00009392: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm 00009395: 	LDI	@ATTRWAVE,AR6		;DCS LOGOSOUND will play only if 0
    // ;	LDI	3,AR2
    // ;	CALL	RANDU0
    // ;	CMPI	0,R0
    // ;	BEQ	BBDD
    // ;
    // ;	CREATE	HEAD2HEAD_LOGO,UTIL_C
    // ;	RETS
    // ;BBDD
    // asm 00009396: 	CREATE	_MIDWAYSPIN,UTIL_C
    // asm 00009399: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MIDSPIN", 0, 0);
    UNIMPL();
}

static void MIDSPINHS(void) {
    // asm 0000939A: 	LDI	@BUTTON_STATUS,R0
    // asm 0000939B: 	ANDN	BUT_VIEWS,R0
    // asm 0000939C: 	STI	R0,@BUTTON_STATUS
    // asm 0000939D: 	LDL	_SECpress,AR2
    // asm 0000939E: 	CALL	LOAD_SECTION_REQ
    // asm 0000939F: 	LDI	MATTR,R0
    // asm 000093A0: 	STI	R0,@_MODE
    // asm 000093A1: 	CREATE	SPIN_CAR,UTIL_C
    // asm 000093A4: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm 000093A7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MIDSPINHS", 0, 0);
    UNIMPL();
}

static void RACELEG(void) {
    // asm 000093A8: 	CLRI	R0
    // asm 000093A9: 	STI	R0,@TEASE_COUNT
    // asm 000093AA: 	LDI	MATTR,R0
    // asm 000093AB: 	STI	R0,@_MODE
    // asm 000093AC: 	CREATE	ATTRACT_DELTA,DRONE_C|DELTA_ATTR_S
    // asm 000093AF: 	LDI	@ATTRWAVE,R0
    // asm 000093B0: 	CMPI	4,R0
    // asm 000093B1: 	LDIEQ	25*30,R0
    // asm 000093B2: 	LDINE	30*30,R0
    // asm 000093B3: 	STI	R0,@_timer
    // asm 000093B4: 	CREATE	_timeout,UTIL_C
    // asm 000093B7: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm 000093BA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RACELEG", 0, 0);
    UNIMPL();
}

static void THANKS(void) {
    // asm 000093BB: 	CALL	LOAD_HIGH_SCORE
    // asm 000093BC: 	BU	HIGH_SCORE
    // 	;these are not cycled, they are special routines
CREDITS:
    // asm 000093BD: 	CREATE	VANITY,UTIL_C
    // asm 000093C0: 	LDI	-1,R0
    // asm 000093C1: 	STI	R0,@_ATTR_MODE
    // asm 000093C2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THANKS", 0, 0);
    UNIMPL();
}

static void LOAD_HIGH_SCORE(void) {
    // asm 000093C3: 	CALL	FIFO_RESET
    // asm 000093C4: 	LDI	1,R0
    // asm 000093C5: 	STI	R0,@HARD_SECTION_LOAD
    // asm 000093C6: 	LDL	_SECpress,AR2
    // asm 000093C7: 	CALL	LOAD_SECTION_REQ
    // asm 000093C8: 	LDI	-1,R0
    // asm 000093C9: 	STI	R0,@_ATTR_MODE
    // asm 000093CA: 	RETS
    // *
    // *NOTE	ANY GAME STARTUP CODE & PROC INITS ARE CALLED/CREATED IN INTRO.ASM
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_HIGH_SCORE", 0, 0);
    UNIMPL();
}

static void BEGIN_GAME(void) {
    // asm 000093CB: 	CALL	SND_RESET_QUIET
    // asm 000093CC: 	LDI	1,R0
    // asm 000093CD: 	STI	R0,@LOADED
    // asm 000093CE: 	CREATE	PLYR_INTRO,PLYR_C|PLYR1_T	;PLAYERS PROCESS
    // asm 000093D1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BEGIN_GAME", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void INIT_SYSTEM(void) {
    // asm 000093D2: 	CALL	PRC_INIT	;initialize process system
    PRC_INIT();
    // asm 000093D3: 	CALL	OBJ_INIT	;initialize object system
    OBJ_INIT();
    // asm 000093D4: 	CALL	TEXT_INIT	;initialize text system
    TEXT_INIT();
    // asm 000093D5: 	CALL	INIT_DRONES	;initialize DRONE tracker system
    INIT_DRONES();
    // asm 000093D6: 	CALL	DYNAOBJ_INIT	;initialize DYNAMIC OBJECTS
    DYNAOBJ_INIT();
    // asm 000093D7: 	CALL	CARB_INIT	;initialize CAR BLOCKS
    CARB_INIT();
    // asm 000093D8: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    INIT_RDDEBRIS();
    // asm 000093D9: 	CLRI	R0
    R0.s = 0;
    // asm 000093DA: 	STI	R0,@_sectime
    _sectime = R0.s;
    // asm 000093DB: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: FIXEDPAL: */
/* asm: .word	14 */
/* asm: RGB	0,0,0 */
/* asm: RGB	255,255,255		;1  WHITE */
/* asm: RGB	070h,070h,070h		;2  GREY */
/* asm: RGB	255,0,0			;3  RED */
/* asm: RGB	0,255,0			;4  GREEN */
/* asm: RGB	055h,055h,99h		;5  BLUE  (for the sky paled out) */
/* asm: RGB	255,255,0		;6  YELLOW */
/* asm: RGB	0,255,255		;7  CYAN */
/* asm: RGB	0,0,0			;8  BLACK */
/* asm: RGB	0,0,255			;9  TRUE BLUE */
/* asm: RGB	0C0h,0A0h,0		;A  Amber color */
/* asm: RGB	050h,050h,050h		;B	DARK GREY */
/* asm: RGB	0A0h,0A0h,0A0h		;C	LIGHT GREY */
/* asm: RGB	030h,030h,030h		;D	VERY DARK GREY */
tPAL FIXEDPAL = {
    14,
    {
        RGB(0, 0, 0),
        RGB(255, 255, 255),       // 1  WHITE
        RGB(0x070, 0x070, 0x070), // 2  GREY
        RGB(255, 0, 0),           // 3  RED
        RGB(0, 255, 0),           // 4  GREEN
        RGB(0x055, 0x055, 0x099), // 5  BLUE  (for the sky paled out)
        RGB(255, 255, 0),         // 6  YELLOW
        RGB(0, 255, 255),         // 7  CYAN
        RGB(0, 0, 0),             // 8  BLACK
        RGB(0, 0, 255),           // 9  TRUE BLUE
        RGB(0x0C0, 0x0A0, 0),     // A  Amber color
        RGB(0x050, 0x050, 0x050), // B	DARK GREY
        RGB(0x0A0, 0x0A0, 0x0A0), // C	LIGHT GREY
        RGB(0x030, 0x030, 0x030), // D	VERY DARK GREY
    }
};
/* asm: ILLUM_PAL: */
/* asm: .word	16 */
/* asm: RGB	06Ah,000h,000h */
/* asm: RGB	06Fh,000h,000h */
/* asm: RGB	074h,000h,000h */
/* asm: RGB	079h,000h,000h */
/* asm: RGB	07Eh,000h,000h */
/* asm: RGB	083h,000h,000h */
/* asm: RGB	088h,000h,000h */
/* asm: RGB	08Dh,000h,000h */
/* asm: RGB	092h,000h,000h */
/* asm: RGB	097h,000h,000h */
/* asm: RGB	09Ch,000h,000h */
/* asm: RGB	0A1h,002h,005h */
/* asm: RGB	0A6h,007h,00Ah */
/* asm: RGB	0ABh,00Ch,00Fh */
/* asm: RGB	0B0h,011h,014h */
/* asm: RGB	0B5h,016h,019h */
/* asm: romdata */
tPAL ILLUM_PAL = {
    16,
    { RGB(0x06A, 0x000, 0x000),
        RGB(0x06F, 0x000, 0x000),
        RGB(0x074, 0x000, 0x000),
        RGB(0x079, 0x000, 0x000),
        RGB(0x07E, 0x000, 0x000),
        RGB(0x083, 0x000, 0x000),
        RGB(0x088, 0x000, 0x000),
        RGB(0x08D, 0x000, 0x000),
        RGB(0x092, 0x000, 0x000),
        RGB(0x097, 0x000, 0x000),
        RGB(0x09C, 0x000, 0x000),
        RGB(0x0A1, 0x002, 0x005),
        RGB(0x0A6, 0x007, 0x00A),
        RGB(0x0AB, 0x00C, 0x00F),
        RGB(0x0B0, 0x011, 0x014),
        RGB(0x0B5, 0x016, 0x019) }
};

/*
 *----------------------------------------------------------------------------
 *THESE PALETTES ONLY HAVE TO BE LOADED UPON STARTUP
 *
 */
void LOAD_FIXED_PALETTES(void) {
    PAL_INIT();

    PAL_ALLOC_RAW(&FIXEDPAL);

    crusn_mem_wr32(COLORAM + 0xFF, R0.u);

    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(fixedfnt_tPAL_ROM));
    PAL_ALLOC_RAW(&ILLUM_PAL);
    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(lgnum43_coolyelo_ROM));
    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(font18_white_ROM));
    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(dnums_amber_ROM));
    LOAD_VARIOUS_PALETTES();
    AR2 = (uintptr_t)ROM_ADDR(shared_PALETTES_ROM);
    HARDalloc_section();
    AR2 = (uintptr_t)sky1_p;
    PAL_ALLOC();
    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(osg10fnt_white_ROM));
    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(bnout1_smoke_ROM));
    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(H2HPAL1_ROM));
    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(H2HPAL2_ROM));
    PAL_ALLOC_RAW((tPAL*)ROM_ADDR(H2HPAL3_ROM));

    // asm 0000941C:  LDL     general_PALETTES,AR2
    AR2 = (uintptr_t)ROM_ADDR(general_PALETTES_ROM);
    HARDalloc_section();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void LOAD_STARTUP_PALS(void) {
    // asm 0000941F: 	LDL	ggate_PALETTES,AR2
    // asm 00009420: 	CALL	HARDalloc_section
    // asm 00009421: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_STARTUP_PALS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: COPCARTAB	;PLAYERS VERSION!!! */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,101,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	192,101,-301	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	208,101,371	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-192,101,-301	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-208,101,371	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	0,0,0	 	;BODY XYZ CENTER OFFSET */
/* asm: 	.word	72-1		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
static int COPCARTAB[] = {
    5, // #OF DYNAS-1
    0,
    101,
    0,  // SHADOW
    3,  // VERTS-1
    -1, // DYNAFLAG
    192,
    101,
    -301, // RT REAR WHEEL XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    208,
    101,
    371, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -192,
    101,
    -301, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    -208,
    101,
    371, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    0,
    0,
    0,      // BODY XYZ CENTER OFFSET
    72 - 1, // VERTS-1
    0,      // DYNAFLAG
};
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: HOTRODTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,117,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	211,100,-329	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	221,100,414	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-211,100,-329	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-221,100,414	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	0,0,0	 	;BODY XYZ CENTER OFFSET */
/* asm: 	.word	131		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
/* asm: 	 */
/* asm: 	 */
static int HOTRODTAB[] = {
    5, // #OF DYNAS-1
    0,
    117,
    0,  // SHADOW
    3,  // VERTS-1
    -1, // DYNAFLAG
    211,
    100,
    -329, // RT REAR WHEEL XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    221,
    100,
    414, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -211,
    100,
    -329, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    -221,
    100,
    414, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    0,
    0,
    0,   // BODY XYZ CENTER OFFSET
    131, // VERTS-1
    0,   // DYNAFLAG
};
/* asm: TESTORTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,62,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	251,62,-333	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	251,62,285	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-251,62,285	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-251,62,-333	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	0,0,0	 	;BODY XYZ CENTER OFFSET */
/* asm: 	.word	59		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
/* asm: 	 */
static int TESTORTAB[] = {
    5, // #OF DYNAS-1
    0,
    62,
    0,  // SHADOW
    3,  // VERTS-1
    -1, // DYNAFLAG
    251,
    62,
    -333, // RT REAR WHEEL XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    251,
    62,
    285, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -251,
    62,
    285, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -251,
    62,
    -333, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    0,
    0,
    0,  // BODY XYZ CENTER OFFSET
    59, // VERTS-1
    0,  // DYNAFLAG
};
/*
 *
 *JEEP DYNAMIC OBJECT SETUP
 *
 */
/* asm: JEEPTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,119,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	250,131,-285	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	250,131,355	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-250,131,-285	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-250,131,357	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	62		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
/* asm: 	 */
static int JEEPTAB[] = {
    5, // #OF DYNAS-1
    0,
    119,
    0,  // SHADOW
    3,  // VERTS-1
    -1, // DYNAFLAG
    250,
    131,
    -285, // RT REAR WHEEL XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    250,
    131,
    355, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -250,
    131,
    -285, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    -250,
    131,
    357, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    0,
    0,
    0,  // BODY XYZ CENTER OFFSET
    62, // VERTS-1
    0,  // DYNAFLAG
};
/*
 *
 *CVETT DYNAMIC OBJECT SETUP
 *
 */
/* asm: VETTTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,-164,0	;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	254,67,-296	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	228,81,376	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-254,67,-296	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-228,81,376	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	114		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
/* asm: 	 */
/* asm: 	 */
static int VETTTAB[] = {
    5, // #OF DYNAS-1
    0,
    -164,
    0,  // SHADOW
    3,  // VERTS-1
    -1, // DYNAFLAG
    254,
    67,
    -296, // RT REAR WHEEL XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    228,
    81,
    376, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -254,
    67,
    -296, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    -228,
    81,
    376, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    0,
    0,
    0,   // BODY XYZ CENTER OFFSET
    114, // VERTS-1
    0,   // DYNAFLAG
};
/*
 *
 *GTRUCKP DYNAMIC OBJECT SETUP
 *
 */
/* asm: GTRUCKTABP */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,-117,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	259,86,-358	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	248,90,388	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-259,86,-358	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-248,90,388	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	131		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
/* asm: 	 */
/* asm: 	 */
static int GTRUCKTABP[] = {
    5, // #OF DYNAS-1
    0,
    -117,
    0,  // SHADOW
    3,  // VERTS-1
    -1, // DYNAFLAG
    259,
    86,
    -358, // RT REAR WHEEL XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    248,
    90,
    388, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -259,
    86,
    -358, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    -248,
    90,
    388, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    0,
    0,
    0,   // BODY XYZ CENTER OFFSET
    131, // VERTS-1
    0,   // DYNAFLAG
};
/*
 *
 *MISSILE DYNAMIC OBJECT SETUP
 *
 */
/* asm: MISSILE_TAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,-117,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	259,86,-358	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	248,90,388	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-259,86,-358	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.float	-248,90,388	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	131		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
/* asm: 	 */
/* asm: 	 */
static int MISSILE_TAB[] = {
    5, // #OF DYNAS-1
    0,
    -117,
    0,  // SHADOW
    3,  // VERTS-1
    -1, // DYNAFLAG
    259,
    86,
    -358, // RT REAR WHEEL XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    248,
    90,
    388, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -259,
    86,
    -358, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    -248,
    90,
    388, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    0,
    0,
    0,   // BODY XYZ CENTER OFFSET
    131, // VERTS-1
    0,   // DYNAFLAG
};
/*
 *
 *PLAYERS SCHOOL BUS
 *
 */
/* asm: PSBUSTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	 */
/* asm: 	.float	0,-127,0	;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	246,127,-380	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	247,127,573	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-247,127,-380	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	 */
/* asm: 	.float	-247,127,573	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	91		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
/* asm: 	 */
static int PSBUSTAB[] = {
    5, // #OF DYNAS-1
    0,
    -127,
    0,  // SHADOW
    3,  // VERTS-1
    -1, // DYNAFLAG
    246,
    127,
    -380, // RT REAR WHEEL XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    247,
    127,
    573, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    -247,
    127,
    -380, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23,   // VERTS-1
    1,    // DYNAFLAG
    -247,
    127,
    573, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23,  // VERTS-1
    2,   // DYNAFLAG
    0,
    0,
    0,  // BODY XYZ CENTER OFFSET
    91, // VERTS-1
    0,  // DYNAFLAG
    // ----------------------------------------------------------------------------
};

/* asm: VEHICLE_TABLE: */
/* asm: 	 */
/* asm: 	.word	cvette,cvette_p,VETTTAB,0,0 */
/* asm: 	.word	1 */
/* asm: 	.float	1.0 */
/* asm: 	.word	SPCARPASS,0 */
/* asm: 	.word	CVETTEPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	.word	hotrod,hotrod_p,HOTRODTAB,dhotrod,d2hotrod */
/* asm: 	.word	0 */
/* asm: 	.float	1.0 */
/* asm: 	.word	SPCARPASS,0 */
/* asm: 	.word	HOTRODPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	.word	missle,missle_p,MISSILE_TAB,0,0 */
/* asm: 	.word	1 */
/* asm: 	.float	1.0 */
/* asm: 	.word	FCARPASS,0 */
/* asm: 	.word	BULLETPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	.word	testor,testor_p,TESTORTAB,dtestor,d2testor */
/* asm: 	.word	3 */
/* asm: 	.float	1.0 */
/* asm: 	.word	SPCARPASS,0 */
/* asm: 	.word	TESTORPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	.word	gtruck,gtruck_p,0,dgtruck,d2gtruck */
/* asm: 	.word	0 */
/* asm: 	.float	1.5 */
/* asm: 	.word	TRUCKPASS,0 */
/* asm: 	.word	GTRUCKPAL */
/* asm: 	.word	DDYNA_GTRUCK */
/* asm: 	 */
/* asm: 	.word	ftruck,ftruck_p,0,0,0 */
/* asm: 	.word	0 */
/* asm: 	.float	2 */
/* asm: 	.word	TRUCKPASS,0 */
/* asm: 	.word	NOPAL */
/* asm: 	.word	DDYNA_FTRUCK */
/* asm: 	 */
/* asm: 	.word	cbus,cbus_p,0,dcbus,d2cbus */
/* asm: 	.word	0 */
/* asm: 	.float	2.0 */
/* asm: 	.word	TRUCKPASS,0 */
/* asm: 	.word	NOPAL */
/* asm: 	.word	DDYNA_CBUS */
/* asm: 	 */
/* asm: 	.word	copcar,copcar_p,0,dcopcar,d2copcar */
/* asm: 	.word	2 */
/* asm: 	.float	1.0 */
/* asm: 	.word	SPCARPASS,0 */
/* asm: 	.word	NOPAL */
/* asm: 	.word	DDYNA_COPCAR */
/* asm: 	 */
/* asm: 	.word	muscle,muscle_p,0,dmuscle,d2muscle */
/* asm: 	.word	0 */
/* asm: 	.float	1.0 */
/* asm: 	.word	SPCARPASS,0 */
/* asm: 	.word	MUSCLEPAL */
/* asm: 	.word	DDYNA_MUSCLE */
/* asm: 	 */
/* asm: 	.word	caravan,caravan_p,0,dcaravan,d2carava */
/* asm: 	.word	0 */
/* asm: 	.float	1.2 */
/* asm: 	.word	TRUCKPASS,0 */
/* asm: 	.word	CARAVANPAL */
/* asm: 	.word	DDYNA_CARAVAN */
/* asm: 	 */
/* asm: 	.word	sbus,sbus_p,0,dsbus,d2sbus */
/* asm: 	.word	0 */
/* asm: 	.float	2.0 */
/* asm: 	.word	TRUCKPASS,0 */
/* asm: 	.word	NOPAL */
/* asm: 	.word	DDYNA_SBUS */
/* asm: 	 */
/* asm: 	.word	ptruckg,ptruckg_p,0,dptruckg,d2ptruck */
/* asm: 	.word	0 */
/* asm: 	.float	1.0 */
/* asm: 	.word	TRUCKPASS,0 */
/* asm: 	.word	PTRUCKGPAL */
/* asm: 	.word	DDYNA_PTRUCKG */
/* asm: 	 */
/* asm: 	.word	mustang,mustang,0,dmustang,d2mustang */
/* asm: 	.word	0 */
/* asm: 	.float	1.0 */
/* asm: 	.word	SPCARPASS,0 */
/* asm: 	.word	NOPAL */
/* asm: 	.word	DDYNA_MUSTANG */
/* asm: 	 */
/* asm: 	.word	toxic,0,0,0,0		;toxic in DV */
/* asm: 	.word	0 */
/* asm: 	.float	2.0 */
/* asm: 	.word	0,0 */
/* asm: 	.word	NOPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	.word	jeep,jeep_p,JEEPTAB,djeep,0 */
/* asm: 	.word	0 */
/* asm: 	.float	1.0 */
/* asm: 	.word	SMCARPASS,0 */
/* asm: 	.word	JEEPPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.word	copcarp,copcar_p,COPCARTAB,dcopcar,d2copcar */
/* asm: 	.word	2 */
/* asm: 	.float	1.0 */
/* asm: 	.word	SPCARPASS,0 */
/* asm: 	.word	NOPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	.word	gtruckp,gtruck_p,GTRUCKTABP,dgtruck,d2gtruck */
/* asm: 	.word	0 */
/* asm: 	.float	1.5 */
/* asm: 	.word	TRUCKPASS,0 */
/* asm: 	.word	GTRUCKPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	.word	sbusp,sbus_p,PSBUSTAB,0,0 */
/* asm: 	.word	0 */
/* asm: 	.float	5.5 */
/* asm: 	.word	TRUCKPASS,0 */
/* asm: 	.word	NOPAL */
/* asm: 	.word	0 */
/* asm: 	 */
/* asm: 	 */
static VEHTAB VEHICLE_TABLE[] = {
    { cvette_ROM, cvette_p, (uintptr_t)VETTTAB, 0, 0, 1, 1.0f, SPCARPASS, 0, (uintptr_t)CVETTEPAL, 0 },
    { hotrod_ROM, hotrod_p, (uintptr_t)HOTRODTAB, dhotrod_ROM, d2hotrod_ROM, 0, 1.0f, SPCARPASS, 0, (uintptr_t)HOTRODPAL, 0 },
    { missle_ROM, missle_p, (uintptr_t)MISSILE_TAB, 0, 0, 1, 1.0f, FCARPASS, 0, (uintptr_t)BULLETPAL, 0 },
    { testor_ROM, testor_p, (uintptr_t)TESTORTAB, dtestor_ROM, d2testor_ROM, 3, 1.0f, SPCARPASS, 0, (uintptr_t)TESTORPAL, 0 },
    { gtruck_ROM, gtruck_p, 0, dgtruck_ROM, d2gtruck_ROM, 0, 1.5f, TRUCKPASS, 0, (uintptr_t)GTRUCKPAL, (uintptr_t)DDYNA_GTRUCK },
    { ftruck_ROM, ftruck_p, 0, 0, 0, 0, 2.0f, TRUCKPASS, 0, (uintptr_t)&NOPAL, (uintptr_t)DDYNA_FTRUCK },
    { cbus_ROM, cbus_p, 0, dcbus_ROM, d2cbus_ROM, 0, 2.0f, TRUCKPASS, 0, (uintptr_t)&NOPAL, (uintptr_t)DDYNA_CBUS },
    { copcar_ROM, copcar_p, 0, dcopcar_ROM, d2copcar_ROM, 2, 1.0f, SPCARPASS, 0, (uintptr_t)&NOPAL, (uintptr_t)DDYNA_COPCAR },
    { muscle_ROM, muscle_p, 0, dmuscle_ROM, d2muscle_ROM, 0, 1.0f, SPCARPASS, 0, (uintptr_t)MUSCLEPAL, (uintptr_t)DDYNA_MUSCLE },
    { caravan_ROM, caravan_p, 0, dcaravan_ROM, d2carava_ROM, 0, 1.2f, TRUCKPASS, 0, (uintptr_t)CARAVANPAL, (uintptr_t)DDYNA_CARAVAN },
    { sbus_ROM, sbus_p, 0, dsbus_ROM, d2sbus_ROM, 0, 2.0f, TRUCKPASS, 0, (uintptr_t)&NOPAL, (uintptr_t)DDYNA_SBUS },
    { ptruckg_ROM, ptruckg_p, 0, dptruckg_ROM, d2ptruck_ROM, 0, 1.0f, TRUCKPASS, 0, (uintptr_t)PTRUCKGPAL, (uintptr_t)DDYNA_PTRUCKG },
    { mustang_ROM, mustang_ROM, 0, dmustang_ROM, d2mustang_ROM, 0, 1.0f, SPCARPASS, 0, (uintptr_t)&NOPAL, (uintptr_t)DDYNA_MUSTANG },
    { toxic_ROM, 0, 0, 0, 0, 0, 2.0f, 0, 0, (uintptr_t)&NOPAL, 0 },
    { jeep_ROM, jeep_p, (uintptr_t)JEEPTAB, djeep_ROM, 0, 0, 1.0f, SMCARPASS, 0, (uintptr_t)JEEPPAL, 0 },
    { copcarp_ROM, copcar_p, (uintptr_t)COPCARTAB, dcopcar_ROM, d2copcar_ROM, 2, 1.0f, SPCARPASS, 0, (uintptr_t)&NOPAL, 0 },
    { gtruckp_ROM, gtruck_p, (uintptr_t)GTRUCKTABP, dgtruck_ROM, d2gtruck_ROM, 0, 1.5f, TRUCKPASS, 0, (uintptr_t)GTRUCKPAL, 0 },
    { sbusp_ROM, sbus_p, (uintptr_t)PSBUSTAB, 0, 0, 0, 5.5f, TRUCKPASS, 0, (uintptr_t)&NOPAL, 0 },
};

/*
 *	number of axels,number of vertices
 *	{
 *	vertices, xcenter,ycenter,zcenter
 *	} * number of axels
 *
 */
/* asm: DDYNA_GTRUCK */
/* asm: 	.word	3,154 */
/* asm: 	.word	8,-252,211,671 */
/* asm: 	.word	8,-254,211,-422 */
/* asm: 	.word	8,-254,211,-704 */
static int DDYNA_GTRUCK[] = {
    3,
    154,
    8,
    -252,
    211,
    671,
    8,
    -254,
    211,
    -422,
    8,
    -254,
    211,
    -704,
};
/* asm: DDYNA_FTRUCK */
/* asm: 	.word	2,92 */
/* asm: 	.word	8,-291,210,624 */
/* asm: 	.word	8,-293,210,-605 */
static int DDYNA_FTRUCK[] = {
    2,
    92,
    8,
    -291,
    210,
    624,
    8,
    -293,
    210,
    -605,
};
/* asm: DDYNA_CBUS */
/* asm: 	.word	3,144 */
/* asm: 	.word	8,-291,261,-843 */
/* asm: 	.word	8,-294,261,-582 */
/* asm: 	.word	8,-294,261,914 */
static int DDYNA_CBUS[] = {
    // 	.word	3,120
    3,
    144,
    8,
    -291,
    261,
    -843,
    8,
    -294,
    261,
    -582,
    8,
    -294,
    261,
    914,
};
/* asm: DDYNA_COPCAR */
/* asm: 	.word	2,135 */
/* asm: 	.word	8,0,97,369 */
/* asm: 	.word	8,0,97,-296 */
static int DDYNA_COPCAR[] = {
    2,
    135,
    8,
    0,
    97,
    369,
    8,
    0,
    97,
    -296,
};
/* asm: DDYNA_MUSCLE */
/* asm: 	.word	2,148 */
/* asm: 	.word	8,244,108,441 */
/* asm: 	.word	8,237,108,-277 */
static int DDYNA_MUSCLE[] = {
    2,
    148,
    8,
    244,
    108,
    441,
    8,
    237,
    108,
    -277,
};
/* asm: DDYNA_CARAVAN */
/* asm: 	.word	2,105 */
/* asm: 	.word	8,-263,156,466 */
/* asm: 	.word	8,263,156,-435 */
static int DDYNA_CARAVAN[] = {
    2,
    105,
    8,
    -263,
    156,
    466,
    8,
    263,
    156,
    -435,
};
/* asm: DDYNA_SBUS */
/* asm: 	.word	2,152 */
/* asm: 	.word	8,316,242,830 */
/* asm: 	.word	8,315,242,-378 */
static int DDYNA_SBUS[] = {
    2,
    152,
    8,
    316,
    242,
    830,
    8,
    315,
    242,
    -378,
};
/* asm: DDYNA_PTRUCKG */
/* asm: 	.word	2,169 */
/* asm: 	.word	8,197,107,323 */
/* asm: 	.word	8,196,107,-270 */
static int DDYNA_PTRUCKG[] = {
    2,
    169,
    8,
    197,
    107,
    323,
    8,
    196,
    107,
    -270,
};
/* asm: DDYNA_MUSTANG */
/* asm: 	.word	2,173 */
/* asm: 	.word	8,234,93,407 */
/* asm: 	.word	8,233,93,-348 */
/* asm: 	 */
static int DDYNA_MUSTANG[] = {
    2,
    173,
    8,
    234,
    93,
    407,
    8,
    233,
    93,
    -348,
};
/* asm: NOPAL */
/* asm: 	.word	0 */
/* asm: 	 */
static int NOPAL = 0;
// ;TESTORPAL	.word	3,testor_blue,testor_grape,testor_gold
/* asm: TESTORPAL	.word	2,testor_blue,testor_grape */
static int TESTORPAL[] = {
    2,
    testor_blue_ROM,
    testor_grape_ROM,
};
/* asm: BULLETPAL	.word	3,missle_yellow,missle_red,missle_blue */
static int BULLETPAL[] = {
    3,
    missle_yellow_ROM,
    missle_red_ROM,
    missle_blue_ROM,
};
// ;HOTRODPAL	.word	3,hotrod_yellow,hotrod_brtblue,hotrod_maroon
/* asm: HOTRODPAL	.word	2,hotrod_yellow,hotrod_brtblue */
static int HOTRODPAL[] = {
    2,
    hotrod_yellow_ROM,
    hotrod_brtblue_ROM,
};
/* asm: CVETTEPAL	.word	3,cvette_blue,cvette_red,cvette_purple */
/* asm: 	 */
static int CVETTEPAL[] = {
    3,
    cvette_blue_ROM,
    cvette_red_ROM,
    cvette_purple_ROM,
};
/* asm: JEEPPAL		.word	2,jeep_red,jeep_yellow */
static int JEEPPAL[] = {
    2,
    jeep_red_ROM,
    jeep_yellow_ROM,
};
// ;GTRUCKPAL	.word	2,gtruck_yellow,gtruck_bluep
/* asm: GTRUCKPAL	.word	1,gtruck_bluep */
static int GTRUCKPAL[] = {
    1,
    gtruck_bluep_ROM,
};
/* asm: CARAVANPAL	.word	2,caravan_redyelo,caravan_yelogrey */
static int CARAVANPAL[] = {
    2,
    caravan_redyelo_ROM,
    caravan_yelogrey_ROM,
};
/* asm: MUSCLEPAL	.word	2,muscle_yellow,muscle_green */
static int MUSCLEPAL[] = {
    2,
    muscle_yellow_ROM,
    muscle_green_ROM,
};
/* asm: PTRUCKGPAL	.word	2,ptruckg_purple,ptruckg_yellr */
/* asm: 	 */
/* asm: 	 */
static int PTRUCKGPAL[] = {
    2,
    ptruckg_purple_ROM,
    ptruckg_yellr_ROM,
};

static void LOAD_VARIOUS_PALETTES(void) {
    // asm 00009507: 	LDL	caravan_redyelo,AR2
    // asm 00009508: 	CALL	PAL_ALLOC_RAW
    // asm 00009509: 	LDL	caravan_yelogrey,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 0000950B: 	LDL	hotrod_yellow,AR2
    // asm 0000950C: 	CALL	PAL_ALLOC_RAW
    // asm 0000950D: 	LDL	hotrod_brtblue,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 0000950F: 	LDL	missle_yellow,AR2
    // asm 00009510: 	CALL	PAL_ALLOC_RAW
    // asm 00009511: 	LDL	missle_red,AR2
    // asm 00009512: 	CALL	PAL_ALLOC_RAW
    // asm 00009513: 	LDL	missle_blue,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 00009515: 	LDL	jeep_red,AR2
    // asm 00009516: 	CALL	PAL_ALLOC_RAW
    // asm 00009517: 	LDL	jeep_yellow,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 00009519: 	LDL	cvette_blue,AR2
    // asm 0000951A: 	CALL	PAL_ALLOC_RAW
    // asm 0000951B: 	LDL	cvette_red,AR2
    // asm 0000951C: 	CALL	PAL_ALLOC_RAW
    // asm 0000951D: 	LDL	cvette_purple,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 0000951F: 	LDL	testor_blue,AR2
    // asm 00009520: 	CALL	PAL_ALLOC_RAW
    // asm 00009521: 	LDL	testor_grape,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 00009523: 	LDL	gtruck_bluep,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 00009525: 	LDL	muscle_yellow,AR2
    // asm 00009526: 	CALL	PAL_ALLOC_RAW
    // asm 00009527: 	LDL	muscle_green,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 00009529: 	LDL	ptruckg_purple,AR2
    // asm 0000952A: 	CALL	PAL_ALLOC_RAW
    // asm 0000952B: 	LDL	ptruckg_yellr,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_VARIOUS_PALETTES", 0, 0);
    UNIMPL();
}

static int tower_bgrey[] = {
    0x080000000 | 256,
    (0x00 << 16) | 0x021,
    (0x042 << 16) | 0x0463,
    (0x0463 << 16) | 0x0884,
    (0x0884 << 16) | 0x0884,
    (0x0884 << 16) | 0x0CA5,
    (0x0CA5 << 16) | 0x0CA5,
    (0x0CA5 << 16) | 0x0CA5,
    (0x0CA5 << 16) | 0x0CA5,
    (0x0CA5 << 16) | 0x0CA5,
    (0x0CA5 << 16) | 0x0CA5,
    (0x0CA5 << 16) | 0x010C6,
    (0x010C6 << 16) | 0x0CA5,
    (0x0CA5 << 16) | 0x010C6,
    (0x0CA5 << 16) | 0x010C6,
    (0x010C6 << 16) | 0x010C6,
    (0x010C6 << 16) | 0x010C6,
    (0x010C6 << 16) | 0x010C6,
    (0x010C6 << 16) | 0x010C6,
    (0x014E7 << 16) | 0x010C6,
    (0x010C6 << 16) | 0x010C6,
    (0x010C6 << 16) | 0x014E7,
    (0x010C6 << 16) | 0x010C6,
    (0x014E7 << 16) | 0x010C6,
    (0x014E7 << 16) | 0x014E7,
    (0x014E7 << 16) | 0x014E7,
    (0x014E7 << 16) | 0x014E7,
    (0x014E7 << 16) | 0x014E7,
    (0x014E7 << 16) | 0x014E7,
    (0x014E7 << 16) | 0x014E7,
    (0x014E7 << 16) | 0x01908,
    (0x01908 << 16) | 0x01D29,
    (0x014E7 << 16) | 0x01908,
    (0x014E7 << 16) | 0x01908,
    (0x01908 << 16) | 0x01908,
    (0x01908 << 16) | 0x01908,
    (0x01908 << 16) | 0x01908,
    (0x01908 << 16) | 0x014E7,
    (0x01908 << 16) | 0x01908,
    (0x01908 << 16) | 0x01D29,
    (0x01908 << 16) | 0x01908,
    (0x01908 << 16) | 0x010C6,
    (0x01D29 << 16) | 0x01908,
    (0x01908 << 16) | 0x01D29,
    (0x01908 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x01908,
    (0x01908 << 16) | 0x01D29,
    (0x01908 << 16) | 0x01D29,
    (0x01908 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x01908,
    (0x01D29 << 16) | 0x01908,
    (0x01D29 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x0214A,
    (0x01D29 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x0214A,
    (0x01D29 << 16) | 0x0214A,
    (0x014E7 << 16) | 0x01D29,
    (0x01D29 << 16) | 0x01D29,
    (0x0214A << 16) | 0x0214A,
    (0x0214A << 16) | 0x01D29,
    (0x0214A << 16) | 0x01D29,
    (0x0214A << 16) | 0x0214A,
    (0x0214A << 16) | 0x01D29,
    (0x0214A << 16) | 0x0214A,
    (0x0256B << 16) | 0x01D29,
    (0x0214A << 16) | 0x0214A,
    (0x0214A << 16) | 0x0214A,
    (0x01D29 << 16) | 0x0214A,
    (0x0256B << 16) | 0x014E7,
    (0x01D29 << 16) | 0x0214A,
    (0x0256B << 16) | 0x0214A,
    (0x0214A << 16) | 0x0214A,
    (0x0256B << 16) | 0x0214A,
    (0x0214A << 16) | 0x0256B,
    (0x0256B << 16) | 0x0214A,
    (0x0214A << 16) | 0x0214A,
    (0x0256B << 16) | 0x0214A,
    (0x0256B << 16) | 0x0256B,
    (0x0214A << 16) | 0x0256B,
    (0x0256B << 16) | 0x0214A,
    (0x0256B << 16) | 0x0256B,
    (0x0214A << 16) | 0x0256B,
    (0x0298C << 16) | 0x0256B,
    (0x0256B << 16) | 0x0214A,
    (0x0256B << 16) | 0x0256B,
    (0x0256B << 16) | 0x0256B,
    (0x0256B << 16) | 0x01D29,
    (0x0298C << 16) | 0x0256B,
    (0x0298C << 16) | 0x0256B,
    (0x0256B << 16) | 0x0256B,
    (0x0298C << 16) | 0x0298C,
    (0x0298C << 16) | 0x0256B,
    (0x0298C << 16) | 0x0298C,
    (0x0298C << 16) | 0x01D29,
    (0x0298C << 16) | 0x0298C,
    (0x0298C << 16) | 0x0298C,
    (0x0298C << 16) | 0x0256B,
    (0x0298C << 16) | 0x0298C,
    (0x0256B << 16) | 0x0298C,
    (0x0298C << 16) | 0x0298C,
    (0x0298C << 16) | 0x0298C,
    (0x02DAD << 16) | 0x02DAD,
    (0x0298C << 16) | 0x02DAD,
    (0x02DAD << 16) | 0x0298C,
    (0x0298C << 16) | 0x02DAD,
    (0x02DAD << 16) | 0x02DAD,
    (0x02DAD << 16) | 0x02DAD,
    (0x0298C << 16) | 0x02DAD,
    (0x02DAD << 16) | 0x02DAD,
    (0x031CE << 16) | 0x02DAD,
    (0x02DAD << 16) | 0x031CE,
    (0x031CE << 16) | 0x031CE,
    (0x031CE << 16) | 0x02DAD,
    (0x031CE << 16) | 0x031CE,
    (0x02DAD << 16) | 0x031CE,
    (0x031CE << 16) | 0x031CE,
    (0x031CE << 16) | 0x035EF,
    (0x035EF << 16) | 0x031CE,
    (0x031CE << 16) | 0x035EF,
    (0x03A10 << 16) | 0x035EF,
    (0x02DAD << 16) | 0x03E31,
    (0x035EF << 16) | 0x03E31,
    (0x03E31 << 16) | 0x04252,
    (0x04673 << 16) | 0x04A94,
};

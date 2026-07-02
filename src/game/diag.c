#include "diag.h"

#include "../core/machine.h"
#include "c30.h"
#include "cmos.h"
#include "dirq.h"
#include "error.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/DIAG.ASM
 */

static void WT_START(void);
static void WT_NSTART(void);
static void KEYPAUSE(void);
static void BEEP(void);
void MAKE_WINDOW(void);
void MAKE_IWINDOW(void);
void HARD_LOAD_DIAGPAL(void);
void HIDDEN_DIAG(void);
static void ENTER_DIAG_INIT(void);
void ENTER_DIAG(void);
static void RETURN_TO_MAINMENU(void);
void TOEXIT(void);
static void MONITOR_DIAGNOSTICS(void);
static void COLORBARS(void);
static void SHOWSWITCH(void);
static void ASCII_TO_BINARY(void);
static void DISPLAY_SWITCH_STATUS(void);
static void DISPLAY_SHIFTER(void);
static void DISPPOTS(void);
static void SHOW_DIPSWITCH(void);
static void SHOW_DIPSWITCH_STATUS(void);
static void DISPLAY_DIPSWITCH_STATUS(void);
static void RUN_MOTION_MENU(void);
static void RUN_ADJUSTMENT_MENU(void);
static void PRINT_ADJUSTMENT(void);
static void RUN_UTILITIES_MENU(void);
static void RUN_DIAG_MENU(void);
static void HANDLE_MENU(void);
static void XIT_MENU_TO_MAIN(void);
void SETUP_DIAG_SCREEN(void);
static void SOUND_BOARD_TEST(void);
static void STEERING_WHEEL_TEST(void);
static void AMODESND(void);
static void BURNIN_TEST(void);
static void RUN_COUNTRY(void);
static void MAKE_STANDARW(void);
static void PRINT_STANDARD(void);
static void PRINT_SPYN(void);
void SET_CONTROLS(void);
static void SET_VOLUME(void);
static void SHOW_VOLUME(void);
static void FULL_FACTORY_RESTORE(void);
void FFRSUB(void);
static void DEFAULT_ADJUSTMENTS(void);
static void RESET_HIGH_SCORES(void);
static void CLEAR_GAME_AUDITS(void);
static void FREEPLAYONOFF(void);
static void FREEGAMEONOFF(void);
static void HIGHSCORE_ENTRY(void);
static void GIRLS_ONOFF(void);
static void CLINTON_ONOFF(void);
static void ROADKILL_ONOFF(void);
static void MILES_OR_KILO(void);
static void CLEAR_CREDITS(void);
static void MESSAGE_BOX(void);
static void CLEAR_YES_NO(void);
static void ASK_YES_NO(void);
static void DUAL_ON_OFF(void);
static void SET_STEERING_SENSITIVITY(void);
static void SET_HIGHSCORE_RESET(void);
static void SET_GAME_DIFFICULTY(void);
static void GAMEDIFF_CVT(void);
static void SET_TIME_TO_START(void);
static void TIME_CVT(void);
static void SET_CHECKPOINT_BONUS(void);
static void CHECKPOINT_BONUS_CVT(void);
static void SET_MAX_CREDITS(void);
static void SET_MIN_VOL_LEVEL(void);
static void SET_MIN_VOL_LEVEL_CVT(void);
static void MULTI_VALUE(void);
static void MULTI_VALUE_STEP(void);
static void RUN_STANDARD_PRICING(void);
static void PRICE_YES_NO(void);
static void RUN_CUSTOM_PRICING(void);
static void FRACTION_YES_NO(void);
static void CUSTOM_YES_NO(void);
static void SET_SLOT(void);
static void PRINT_SLOT(void);
static void PRINT_VALUE(void);
static void PRINT_VOL(void);
static void GETSTR_LENGTH(void);
static void PRINT_COINMODE(void);
static void PRINT_CUSTOM_SELECTED(void);
static void PRINT_TIME(void);
static void PRINT_NUMBER(void);
static void PRINT_YN_INV(void);
static void PRINT_YN(void);
static void PRINT_ONOFF_INV(void);
static void PRINT_ONOFF(void);
static void PRINT_MPH_INV(void);
static void PRINT_MPH(void);
static void RUN_LAMP_TEST(void);
static void LAMPSOFF(void);
static void LAMPSALL(void);
static void LAMPS(void);
static void LAMPV1(void);
static void LAMPV2(void);
static void LAMPV3(void);
static void LAMPLT(void);
static void LAMPRT(void);
static void LAMPLF(void);
static void LAMPRF(void);
static void MOTION_CHECK_SAFETY_CODES(void);
static void MSC_BOARD_NOTPLUGGED(void);
static void MSC_MAT_STEPPED(void);
static void MSC_MAT_NOTPLUGGED(void);
static void MSC_OPTO_PATH(void);
static void MSC_OPTO_LED(void);
static void MSC_OPTO_DETECTOR(void);
static void MSC_FAILSAFE_OK(void);
static void MSC_FAILSAFE_NOTCONNECTED(void);
static void DRAW_MOTION_SAFETY_DIAG(void);
static void CLEAR_MATS(void);
void MOTION_SAFETY_SWITCHES_DIAG(void);

#define bufferI buffer
#define SWITCHES_DIAGI SWITCHES_DIAG
#define DMOTI DMOT
#define DNMOTI DNMOT
#define DLINKDI DLINKD
#define DNLINKDI DNLINKD
#define ADJTABI ADJTAB
#define COUNTRY_MENUTABI COUNTRY_MENUTAB
#define GAMEDIFF_CVTI GAMEDIFF_CVT
#define TIME_CVTI TIME_CVT
#define CHECKPOINT_BONUS_CVTI CHECKPOINT_BONUS_CVT
#define SET_MIN_VOL_LEVEL_CVTI SET_MIN_VOL_LEVEL_CVT
#define PSTABI PSTAB

void GETCOIN_DEFAULT(void);
void GET_THIS_COINTXT(void);
#define COINMSGC_TABI COINMSGC_TAB
void HARDSND(void);
#define COUNTRY_TABI COUNTRY_TAB
extern const char MSGC_USA1[];
extern const char MSGC_USA2[];
extern const char MSGC_USA3[];
extern const char MSGC_USA4[];
extern const char MSGC_USA5[];
extern const char MSGC_USA6[];
extern const char MSGC_USA7[];
extern const char MSGC_USA8[];
extern const char MSGC_USA9[];
extern const char MSGC_USA10[];
extern const char MSGC_USA11[];
extern const char MSGC_USA12[];
extern const char MSGC_GERMAN1[];
extern const char MSGC_GERMAN2[];
extern const char MSGC_GERMAN3[];
extern const char MSGC_GERMAN4[];
extern const char MSGC_GERMAN5[];
extern const char MSGC_GERMAN6[];
extern const char MSGC_GERMAN7[];
extern const char MSGC_GERMAN8[];
extern const char MSGC_FRENCH1[];
extern const char MSGC_FRENCH2[];
extern const char MSGC_FRENCH3[];
extern const char MSGC_FRENCH4[];
extern const char MSGC_FRENCH5[];
extern const char MSGC_FRENCH6[];
extern const char MSGC_FRENCH7[];
extern const char MSGC_FRENCH8[];
extern const char MSGC_FRENCH9[];
extern const char MSGC_FRENCH10[];
extern const char MSGC_FRENCH11[];
extern const char MSGC_CANADA1[];
extern const char MSGC_CANADA2[];
extern const char MSGC_CANADA3[];
extern const char MSGC_SWISS1[];
extern const char MSGC_SWISS2[];
extern const char MSGC_SWISS3[];
extern const char MSGC_ITALY1[];
extern const char MSGC_ITALY2[];
extern const char MSGC_ITALY3[];
extern const char MSGC_UK1[];
extern const char MSGC_UK2[];
extern const char MSGC_UK3[];
extern const char MSGC_SPAIN1[];
extern const char MSGC_SPAIN2[];
extern const char MSGC_SPAIN3[];
extern const char MSGC_SPAIN4[];
extern const char MSGC_SPAIN5[];
extern const char MSGC_SPAIN6[];
extern const char MSGC_AUSTRALIA1[];
extern const char MSGC_AUSTRALIA2[];
extern const char MSGC_AUSTRALIA3[];
extern const char MSGC_AUSTRALIA4[];
extern const char MSGC_JAPAN1[];
extern const char MSGC_JAPAN2[];
extern const char MSGC_JAPAN3[];
extern const char MSGC_TAIWAN1[];
extern const char MSGC_TAIWAN2[];
extern const char MSGC_TAIWAN3[];
extern const char MSGC_AUSTRIA1[];
extern const char MSGC_AUSTRIA2[];
extern const char MSGC_AUSTRIA3[];
extern const char MSGC_AUSTRIA4[];
extern const char MSGC_BELGIUM1[];
extern const char MSGC_BELGIUM2[];
extern const char MSGC_BELGIUM3[];
extern const char MSGC_SWEDEN1[];
extern const char MSGC_FINLAND1[];
extern const char MSGC_NETHERLAND1[];
extern const char MSGC_NETHERLAND2[];
extern const char MSGC_NORWAY1[];
extern const char MSGC_DENMARK1[];
extern const char MSGC_HUNGARY1[];
extern const char MSGC_GENERAL1[];
extern const char MSGC_GENERAL2[];
extern const char MSGC_GENERAL3[];
extern const char MSGC_GENERAL4[];
extern const char MSGC_GENERAL5[];
extern const char MSGC_GENERAL6[];
extern const char MSGC_GENERAL7[];
extern const char MSGC_GENERAL8[];
void SET_COINAGE_ADJ(void);
void DIAG_MOTION_TESTX(void);
void DIAG_MOTION_TESTY(void);
void DIAG_MOTION_TESTZ(void);
void DIAG_MOTION_BURNIN(void);
void SEND_MOTOR_OFF(void);

static int MAIN_MENU;
static const char MMT[];
static const char DMOD1[];
static const char DMOD2[];
static int MOTION_MENU;
static int ADJUSTMENT_MENU;
static uintptr_t ADJTAB[];
static int UTILITIES_MENU;
static int DIAGNOSTIC_MENU;
static const char ETMAIN[];
static uintptr_t COUNTRY_MENUTAB[];
static const char SPM[];
static const char YES[];
static const char NO[];
static const char FFRST[];
static const char DAS[];
static const char HSRS[];
static const char BOOKR[];
static const char CCD2A[];
static const char STP[];
static const char CPS[];
static const char CPCS[];
static const char CUPC[];
static const char CTSS[];
static const char CTCS[];
static const char CCTS[];
static const char CCTC[];
static const char CTPC[];
static uintptr_t PSTAB[];
static int MOTION_SAFETY_CODES;

/*
 *----------------------------------------------------------------------------
 *DIAGNOSTIC AND BOOK KEEPING ROUTINES
 *
 *COPYRIGHT (C) 1994  BY TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

// *WAIT FOR ENTER BUTTON ON
static void WT_START(void) {
WT_S1:
    // asm 000005F6: 	LDI	@_newbut,R0
    // asm 000005F7: 	TSTB	SW_DIAG,R0
    // asm 000005F8: 	BNZ	WT_S1
    // asm 000005F9: 	RETS
    // *WAIT FOR ENTER BUTTON OFF
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WT_START", 0, 0);
    UNIMPL();
}

static void WT_NSTART(void) {
WT_NS1:
    // asm 000005FA: 	LDI	@_newbut,R0
    // asm 000005FB: 	TSTB	SW_DIAG,R0
    // asm 000005FC: 	BZ	WT_NS1
    // asm 000005FD: 	RETS
    // *WAIT FOR ENTER ON THEN OFF
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WT_NSTART", 0, 0);
    UNIMPL();
}

static void KEYPAUSE(void) {
    // asm 000005FE: 	CALL	WT_START
    // asm 000005FF: 	B	WT_NSTART
    // *MAKE A BEEP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KEYPAUSE", 0, 0);
    UNIMPL();
}

static void BEEP(void) {
    // asm 00000600: 	LDI	980,AR2
    // asm 00000601: 	BR	SENDSND
    // ;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BEEP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
#define MW_Y2 0
#define MW_X2 1
#define MW_Y1 2
#define MW_X1 3

void MAKE_WINDOW(void) {
    // asm 00000602: 	PUSH	AR3
    // asm 00000603: 	PUSH	AR2
    // asm 00000604: 	PUSH	R2
    // asm 00000605: 	PUSH	R3
    // asm 00000606: 	PUSH	RC
    // asm 00000607: 	LDI	SP,AR3
    // asm 00000608: 	LDI	COL_LGREY,RS
    // asm 00000609: 	CALL	_fill
    // asm 0000060A: 	LDI	*-AR3(MW_X1),AR2
    // asm 0000060B: 	LDI	*-AR3(MW_Y1),R2
    // asm 0000060C: 	LDI	*-AR3(MW_X2),R3
    // asm 0000060D: 	LDI	*-AR3(MW_Y1),RC
    // asm 0000060E: 	LDI	COL_LGREY2,RS
    // asm 0000060F: 	CALL	_line
    // asm 00000610: 	LDI	*-AR3(MW_X1),AR2
    // asm 00000611: 	LDI	*-AR3(MW_Y1),R2
    // asm 00000612: 	LDI	*-AR3(MW_X1),R3
    // asm 00000613: 	LDI	*-AR3(MW_Y2),RC
    // asm 00000614: 	LDI	COL_LGREY2,RS
    // asm 00000615: 	CALL	_line
    // asm 00000616: 	LDI	*-AR3(MW_X2),AR2
    // asm 00000617: 	LDI	*-AR3(MW_Y1),R2
    // asm 00000618: 	LDI	*-AR3(MW_X2),R3
    // asm 00000619: 	LDI	*-AR3(MW_Y2),RC
    // asm 0000061A: 	LDI	COL_DGREY,RS
    // asm 0000061B: 	CALL	_line
    // asm 0000061C: 	LDI	*-AR3(MW_X1),AR2
    // asm 0000061D: 	LDI	*-AR3(MW_Y2),R2
    // asm 0000061E: 	LDI	*-AR3(MW_X2),R3
    // asm 0000061F: 	LDI	*-AR3(MW_Y2),RC
    // asm 00000620: 	LDI	COL_DGREY,RS
    // asm 00000621: 	CALL	_line
    // asm 00000622: 	POP	RC
    // asm 00000623: 	POP	R3
    // asm 00000624: 	POP	R2
    // asm 00000625: 	POP	AR2
    // asm 00000626: 	POP	AR3
    // asm 00000627: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_WINDOW", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void MAKE_IWINDOW(void) {
    // asm 00000628: 	PUSH	AR3
    // asm 00000629: 	PUSH	AR2
    // asm 0000062A: 	PUSH	R2
    // asm 0000062B: 	PUSH	R3
    // asm 0000062C: 	PUSH	RC
    // asm 0000062D: 	LDI	SP,AR3
    // asm 0000062E: 	LDI	COL_LGREY,RS
    // asm 0000062F: 	CALL	_fill
    // asm 00000630: 	LDI	*-AR3(MW_X1),AR2
    // asm 00000631: 	LDI	*-AR3(MW_Y1),R2
    // asm 00000632: 	LDI	*-AR3(MW_X2),R3
    // asm 00000633: 	LDI	*-AR3(MW_Y1),RC
    // asm 00000634: 	LDI	COL_DGREY,RS
    // asm 00000635: 	CALL	_line
    // asm 00000636: 	LDI	*-AR3(MW_X1),AR2
    // asm 00000637: 	LDI	*-AR3(MW_Y1),R2
    // asm 00000638: 	LDI	*-AR3(MW_X1),R3
    // asm 00000639: 	LDI	*-AR3(MW_Y2),RC
    // asm 0000063A: 	LDI	COL_DGREY,RS
    // asm 0000063B: 	CALL	_line
    // asm 0000063C: 	LDI	*-AR3(MW_X2),AR2
    // asm 0000063D: 	LDI	*-AR3(MW_Y1),R2
    // asm 0000063E: 	LDI	*-AR3(MW_X2),R3
    // asm 0000063F: 	LDI	*-AR3(MW_Y2),RC
    // asm 00000640: 	LDI	COL_LGREY2,RS
    // asm 00000641: 	CALL	_line
    // asm 00000642: 	LDI	*-AR3(MW_X1),AR2
    // asm 00000643: 	LDI	*-AR3(MW_Y2),R2
    // asm 00000644: 	LDI	*-AR3(MW_X2),R3
    // asm 00000645: 	LDI	*-AR3(MW_Y2),RC
    // asm 00000646: 	LDI	COL_LGREY2,RS
    // asm 00000647: 	CALL	_line
    // asm 00000648: 	POP	RC
    // asm 00000649: 	POP	R3
    // asm 0000064A: 	POP	R2
    // asm 0000064B: 	POP	AR2
    // asm 0000064C: 	POP	AR3
    // asm 0000064D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_IWINDOW", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static const char inst1[] = "USE UP/DOWN TO SELECT FUNCTION";
static const char inst2[] = "ENTER TO ACTIVATE";
static const char title[] = "CRUISIN USA   DIAGNOSTICS";
static const char toexit[] = "HIT ENTER TO EXIT";
/* asm: DIAGPAL: */
/* asm: .word	16			;LENGTH OF PALETTE */
/* asm: RGB	060h,060h,060h		;0 LIGHT GREY */
/* asm: RGB	0ffh,0ffh,0ffh		;1  WHITE */
/* asm: RGB	070h,070h,070h		;2  GREY */
/* asm: RGB	0ffh,000h,000h		;3  RED */
/* asm: RGB	000h,0ffh,000h		;4  GREEN */
/* asm: RGB	055h,055h,099h		;5  BLUE  (for the sky paled out) */
/* asm: RGB	0ffh,0ffh,000h		;6  YELLOW */
/* asm: RGB	000h,0ffh,0ffh		;7  CYAN */
/* asm: RGB	000h,000h,000h		;8  BLACK */
/* asm: RGB	000h,000h,077h		;9  TRUE BLUE */
/* asm: RGB	0C0h,0A0h,000h		;A  Amber color */
/* asm: RGB	050h,050h,050h		;B	DARK GREY */
/* asm: RGB	060h,060h,060h		;	LIGHT GREY */
/* asm: RGB	090h,090h,090h		;	LIGHT GREY2 */
/* asm: RGB	0C0h,0C0h,0C0h		;	LIGHT GREY3 */
/* asm: RGB	020h,020h,020h		;	LIGHT GREY3 */
static tPAL DIAGPAL[] = {
    16, // LENGTH OF PALETTE
    {
        RGB(0x060, 0x060, 0x060), // 0 LIGHT GREY
        RGB(0x0FF, 0x0FF, 0x0FF), // 1  WHITE
        RGB(0x070, 0x070, 0x070), // 2  GREY
        RGB(0x0FF, 0x000, 0x000), // 3  RED
        RGB(0x000, 0x0FF, 0x000), // 4  GREEN
        RGB(0x055, 0x055, 0x099), // 5  BLUE  (for the sky paled out)
        RGB(0x0FF, 0x0FF, 0x000), // 6  YELLOW
        RGB(0x000, 0x0FF, 0x0FF), // 7  CYAN
        RGB(0x000, 0x000, 0x000), // 8  BLACK
        RGB(0x000, 0x000, 0x077), // 9  TRUE BLUE
        RGB(0x0C0, 0x0A0, 0x000), // A  Amber color
        RGB(0x050, 0x050, 0x050), // B	DARK GREY
        RGB(0x060, 0x060, 0x060), // LIGHT GREY
        RGB(0x090, 0x090, 0x090), // LIGHT GREY2
        RGB(0x0C0, 0x0C0, 0x0C0), // LIGHT GREY3
        RGB(0x020, 0x020, 0x020), // LIGHT GREY3
    }
};

// *----------------------------------------------------------------------------
void HARD_LOAD_DIAGPAL(void) {
    // asm 0000065F: 	LDL	DIAGPAL,AR2
    // asm 00000660: 	LDIL	COLORAM,AR1
    // asm 00000663: 	LDI	*AR2++,AR0
    // asm 00000664: 	DEC	AR0
    // asm 00000665: LODLP
    // asm 00000665: 	LDI	*AR2++,R0
    // asm 00000666: 	STI	R0,*AR1++
    // asm 00000667: 	DBU	AR0,LODLP
    // asm 00000668: 	RETS					;F  -cycling color
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HARD_LOAD_DIAGPAL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: SOFT_AUDIT	.bss	SOFT_AUDIT,1 */
int SOFT_AUDIT;

void HIDDEN_DIAG(void) {
    // asm 00000669: 	CALL	ENTER_DIAG_INIT
    // asm 0000066A: 	LDI	@DIAGPAL+1,R0
    // asm 0000066B: 	STI	R0,@BGNDCOLA
    // asm 0000066C: 	LDI	MDIAG,R0		;set mode appropriate
    // asm 0000066D: 	STI	R0,@_MODE
    // asm 0000066E: 	CALL	CLRSCRN			;we are NOT page flipping
    // asm 0000066F: 	CALL	SETPAGE0
    // asm 00000670: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 00000673: 	STI	R0,@ACTIVE_SCREEN
    // asm 00000674: 	LDI	1,R0
    // asm 00000675: 	STI	R0,@SOFT_AUDIT
    // asm 00000676: 	CALL	AUDIT_DISPLAY
    // asm 00000677: 	BR	DIAG_RETURN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIDDEN_DIAG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 */
static void ENTER_DIAG_INIT(void) {
    // asm 00000678: 	POP	R0
    // asm 00000679: 	LDP	@FASTSTKI
    // asm 0000067A:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm 0000067B: 	PUSH	R0
    // asm 0000067C: 	FIFO_CLRP	R0		;is the fifo clear
    // asm 00000681: 	DMA_WT		R0
    // asm 00000686: 	CALL	FIFO_RESET
    // asm 00000687: 	CALL	SILENT
    // asm 00000688: 	LDI	1,R2
    // asm 00000689: 	SETAUD	ADJ_OUTOFDIAG
    // asm 0000068B: 	LDI	MDIAG,R0		;MELLOW THE WHEEL
    // asm 0000068C: 	STI	R0,@_MODE
    // asm 0000068D: 	LDI	INT0_M|INT3_M,IE	;ENABLE INTERRUPTS
    // asm 0000068E: 	AND     0,IF
    // asm 0000068F: 	CALL	ENABLEGIE		;MUST DO AT START FOR I/O READING
    // asm 00000690: 	CALL	INIT_SYSTEM
    // asm 00000691: 	CALL	PAL_INIT
    // asm 00000692: 	LDL	DIAGPAL,AR2
    // asm 00000693: 	CALL	PAL_ALLOC_RAW
    // asm 00000694: 	CLRI	AR2
    // asm 00000695: 	CALL	SENDSND
    // asm 00000696: 	CLRI	R4
    // asm 00000697: 	STI	R4,@SOFT_AUDIT
    // asm 00000698: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTER_DIAG_INIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void ENTER_DIAG(void) {
    // asm 00000699: 	CALL	ENTER_DIAG_INIT
    // asm 0000069A: ENTER_DIAG2
    // asm 0000069A: 	CLRI	R0
    // asm 0000069B: 	STI	R0,@BUTTON_STATUS
    // asm 0000069C: 	LDI	MDIAG,R0		;set mode appropriate
    // asm 0000069D: 	STI	R0,@_MODE
    // asm 0000069E: 	LDL	MAIN_MENU,AR5
    // asm 0000069F: 	LDI	MAIN_MENU_LEN,AR6
    // asm 000006A0: 	LDI	@MMT,AR7
    // asm 000006A1: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTER_DIAG", 0, 0);
    UNIMPL();
}

static void RETURN_TO_MAINMENU(void) {
    // asm 000006A2: 	LDI	MDIAG,R0		;set mode appropriate
    // asm 000006A3: 	STI	R0,@_MODE
    // asm 000006A4: 	LDL	MAIN_MENU,AR5
    // asm 000006A5: 	LDI	MAIN_MENU_LEN,AR6
    // asm 000006A6: 	LDI	@MMT,AR7
    // asm 000006A7: 	BU	HMER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RETURN_TO_MAINMENU", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void TOEXIT(void) {
    // asm 000006A8: 	LDI	DEFAULT_COLOR,RC
    // asm 000006A9: 	TEXTIT	toexit,180,371
    // asm 000006AD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOEXIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
;*ARE DIAGs STILL ACTIVE?
;*T-> RETURN
;*F-> RESET
;*
;DIAG_ACTIVEP:
;	LDI	@_newbut,R0	;old system method
;	RETS
*----------------------------------------------------------------------------
*/

/*
 *----------------------------------------------------------------------------
 *
 */
/* asm: FILSIZI	.word	3FFFFH */
int FILSIZI = 0x3FFFF;
/* asm: REDV	.word	30003h		;these values point into fixed palette */
static int REDV = 0x30003;
/* asm: GREENV	.word	40004h */
static int GREENV = 0x40004;
/* asm: BLUEV	.word	90009h */
static int BLUEV = 0x90009;
/* asm: BLACKV	.word	80008h */
/* asm: 	 */
static int BLACKV = 0x80008;

static void MONITOR_DIAGNOSTICS(void) {
    // asm 000006B3: 	CALL	COLORBARS
    // asm 000006B4: 	CALL	KEYPAUSE
    // asm 000006B5: 	CALL	PAL_INIT
    // asm 000006B6: 	LDL	DIAGPAL,AR2
    // asm 000006B7: 	CALL	PAL_ALLOC_RAW
    // asm 000006B8: 	LDIL	SCREEN0,AR2
    // asm 000006BB: 	LDI	@FILSIZI,R3
    // asm 000006BC: 	LDI	@REDV,R2
    // asm 000006BD: 	CALL	SCREEN_FILL
    // asm 000006BE: 	CALL	KEYPAUSE
    // ;	CALL	DIAG_ACTIVEP
    // asm 000006BF: 	LDIL	SCREEN0,AR2
    // asm 000006C2: 	LDI	@FILSIZI,R3
    // asm 000006C3: 	LDI	@GREENV,R2
    // asm 000006C4: 	CALL	SCREEN_FILL
    // asm 000006C5: 	CALL	KEYPAUSE
    // ;	CALL	DIAG_ACTIVEP
    // asm 000006C6: 	LDIL	SCREEN0,AR2
    // asm 000006C9: 	LDI	@FILSIZI,R3
    // asm 000006CA: 	LDI	@BLUEV,R2
    // asm 000006CB: 	CALL	SCREEN_FILL
    // asm 000006CC: 	CALL	KEYPAUSE
    // ;	CALL	DIAG_ACTIVEP
    // asm 000006CD: 	LDIL	SCREEN0,AR2
    // asm 000006D0: 	LDI	@FILSIZI,R3
    // asm 000006D1: 	LDI	@BLACKV,R2
    // asm 000006D2: 	CALL	SCREEN_FILL
    // 	;12 across
    // 	;HLINES
    // 	;20 down
    // asm 000006D3: 	LDI	20,AR7
    // asm 000006D4: 	LDI	0,AR6
    // asm 000006D5: LINLOP
    // asm 000006D5: 	CMPI	20,AR6
    // asm 000006D6: 	LDIEQ	19,AR6
    // asm 000006D7: 	CLRI	AR2
    // asm 000006D8: 	LDI	511,R3
    // asm 000006D9: 	LDI	AR6,R2
    // asm 000006DA: 	LDI	AR6,RC
    // asm 000006DB: 	LDI	1,RS
    // asm 000006DC: 	CALL	_line
    // asm 000006DD: 	ADDI	20,AR6
    // asm 000006DE: 	DBU	AR7,LINLOP
    // 	;VLINES
    // asm 000006DF: 	LDI	32,AR7
    // asm 000006E0: 	LDI	0,AR6
    // asm 000006E1: VLINLOP
    // asm 000006E1: 	CMPI	16,AR6
    // asm 000006E2: 	LDIEQ	15,AR6
    // asm 000006E3: 	CLRI	R2
    // asm 000006E4: 	LDI	399,RC
    // asm 000006E5: 	LDI	AR6,AR2
    // asm 000006E6: 	LDI	AR6,R3
    // asm 000006E7: 	LDI	1,RS
    // asm 000006E8: 	CALL	_line
    // asm 000006E9: 	ADDI	16,AR6
    // asm 000006EA: 	DBU	AR7,VLINLOP
    // asm 000006EB: 	CALL	KEYPAUSE
    // asm 000006EC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MONITOR_DIAGNOSTICS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void COLORBARS(void) {
    // asm 000006ED: 	CALL	CLRSCRN0
    // asm 000006EE: 	CLRI	R0
    // asm 000006EF: 	STI	R0,@BGNDCOLA
    // asm 000006F0: 	LDI	09Eh,AR0
    // asm 000006F1: 	LS	16,AR0
    // asm 000006F2: 	LDI	15,RC
    // asm 000006F3: 	RPTB	GREYLP
    // asm 000006F4: 	LDI	R0,R1
    // asm 000006F5: 	LDI	R0,R2
    // asm 000006F6: 	LS	5,R1
    // asm 000006F7: 	LS	10,R2
    // asm 000006F8: 	LDI	R0,R3
    // asm 000006F9: 	OR	R1,R3
    // asm 000006FA: 	OR	R2,R3
    // asm 000006FB: 	STI	R3,*AR0++
GREYLP:
    // asm 000006FC: ADDI	2,R0
    // asm 000006FD: 	CLRI	R0
    // asm 000006FE: 	LDI	15,RC
    // asm 000006FF: 	RPTB	REDLP
    // asm 00000700: 	LDI	R0,R1
    // asm 00000701: 	LS	10,R1
    // asm 00000702: 	STI	R1,*AR0++
REDLP:
    // asm 00000703: ADDI	2,R0
    // asm 00000704: 	CLRI	R0
    // asm 00000705: 	LDI	15,RC
    // asm 00000706: 	RPTB	GREENLP
    // asm 00000707: 	LDI	R0,R1
    // asm 00000708: 	LSH	5,R1
    // asm 00000709: 	STI	R1,*AR0++
GREENLP:
    // asm 0000070A: ADDI	2,R0
    // asm 0000070B: 	CLRI	R0
    // asm 0000070C: 	LDI	15,RC
    // asm 0000070D: 	RPTB	BLUELP
    // asm 0000070E: 	STI	R0,*AR0++
BLUELP:
    // asm 0000070F: ADDI	2,R0
    // asm 00000710: 	CLRI	R3		;color
    // asm 00000711: 	LDI	50,R2		;starting Y coordinate
    // asm 00000712: 	LDI	3,AR4
    // asm 00000713: WHOLELP
    // asm 00000713: 	LDI	R3,R4
    // asm 00000714: 	LDI	50,AR5		;lines per mess
GROUPLP:
    // asm 00000715: LDI	R4,R3
    // asm 00000716: 	LDI	128,AR2		;starting X pixel
    // asm 00000717: 	LDI	15,AR7		;blocks per lines
LINELP:
    // asm 00000718: LDI	15,AR6		;pixels per block
BLOCKLP:
    // asm 00000719: CALL	_pixel
    // asm 0000071A: 	INC	AR2		;next pixel
    // asm 0000071B: 	DBU	AR6,BLOCKLP
    // asm 0000071C: 	INC	R3
    // asm 0000071D: 	DBU	AR7,LINELP
    // asm 0000071E: 	INC	R2
    // asm 0000071F: 	DBU	AR5,GROUPLP
    // asm 00000720: 	ADDI	20,R2
    // asm 00000721: 	DBU	AR4,WHOLELP
    // asm 00000722: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLORBARS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SHOW SWITCHES (AND POTS)
 *
 */
/* asm: buffer	.bss	buffer,20 */
int buffer[20];
/* asm: _opot0	.bss	_opot0,1 */
int _opot0;
/* asm: _opot1	.bss	_opot1,1 */
int _opot1;
/* asm: _opot2	.bss	_opot2,1 */
int _opot2;

static void SHOWSWITCH(void) {
    // asm 00000724: 	CALL	SETUP_DIAG_SCREEN
    // asm 00000725: 	LDI	-1,R0
    // asm 00000726: 	STI	R0,@_opot0
    // asm 00000727: 	STI	R0,@_opot1
    // asm 00000728: 	STI	R0,@_opot2
    // asm 00000729: 	STI	R0,@_onebut
    // asm 0000072A: 	STI	R0,@LASTDIPPLOT
    // asm 0000072B: 	STI	R0,@LASTSHIFT
    // asm 0000072C: 	IWINDOW	125,170,145,355
    // asm 00000731: 	IWINDOW	135,95,220,135
    // asm 00000736: 	IWINDOW	245,95,420,135
    // asm 0000073B: 	GROUP	10,60,465,150	;pots etc
    // asm 0000076B: 	GROUP	10,160,170,360	;switches
    // asm 0000079B: 	GROUP	190,160,360,250	;shifter
    // asm 000007CB: 	LDI	COL_VDGREY,RC
    // asm 000007CC: 	TEXTITT	"SWITCH TEST",205,26
    // asm 000007D0: 	TEXTITT	"STEERING WHEEL",20,100
    // asm 000007D4: 	TEXTITT	"GAS PEDAL",20,110
    // asm 000007D8: 	TEXTITT	"BRAKE PEDAL",20,120
    // asm 000007DC: 	TEXTITT	"VIEW 1",20,180
    // asm 000007E0: 	TEXTITT	"VIEW 2",20,200
    // asm 000007E4: 	TEXTITT	"VIEW 3",20,220
    // asm 000007E8: 	TEXTITT	"START",20,240
    // asm 000007EC: 	TEXTITT	"COIN 1",20,260
    // asm 000007F0: 	TEXTITT	"COIN 2",20,280
    // asm 000007F4: 	TEXTITT	"COIN 3",20,300
    // asm 000007F8: 	TEXTITT	"COIN 4",20,320
    // asm 000007FC: 	TEXTITT	"RADIO",20,340
    // asm 00000800: 	TEXTITT	"HIT ENTER TO EXIT",40,40
    // asm 00000804: 	TEXTITT	"ENTER",215,280
    // asm 00000808: 	TEXTITT	"VOLUME UP",215,300
    // asm 0000080C: 	TEXTITT	"VOLUME DOWN",215,320
    // asm 00000810: 	TEXTITT	"SERVICE",215,340
    // asm 00000814: 	TEXTITT	"CREDIT",225,350
    // asm 00000818: 	IWINDOW	325,270,355,360
SSLP:
    // ;	CALL	DIAG_ACTIVEP
    // asm 0000081D: 	CALL	DISPPOTS
    // asm 0000081E: 	CALL	DISPLAY_SWITCH_STATUS
    // asm 0000081F: 	LDI	COL_VDGREY,RC
    // asm 00000820: 	TEXTITT	"CMOS VALUES",250,70
    // asm 00000824: 	TEXTITT	"MIN  CENTER MAX",250,80
    // asm 00000828: 	TEXTITT	"POT READING",134,80
    // asm 0000082C: 	READADJ	ADJ_GASMIN
    // asm 0000082E: 	LDI	R0,R2
    // asm 0000082F: 	LDI	@bufferI,AR2
    // asm 00000830: 	CALL	_itoa
    // asm 00000831: 	LDI	250,R2
    // asm 00000832: 	LDI	110,R3
    // asm 00000833: 	LDI	DEFAULT_COLOR,RC
    // asm 00000834: 	CALL	_outtextxyc
    // asm 00000835: 	READADJ	ADJ_GASMAX
    // asm 00000837: 	LDI	R0,R2
    // asm 00000838: 	LDI	@bufferI,AR2
    // asm 00000839: 	CALL	_itoa
    // asm 0000083A: 	LDI	350,R2
    // asm 0000083B: 	LDI	110,R3
    // asm 0000083C: 	LDI	DEFAULT_COLOR,RC
    // asm 0000083D: 	CALL	_outtextxyc
    // asm 0000083E: 	READADJ	ADJ_STEERMIN
    // asm 00000840: 	LDI	R0,R2
    // asm 00000841: 	LDI	@bufferI,AR2
    // asm 00000842: 	CALL	_itoa
    // asm 00000843: 	LDI	250,R2
    // asm 00000844: 	LDI	100,R3
    // asm 00000845: 	LDI	DEFAULT_COLOR,RC
    // asm 00000846: 	CALL	_outtextxyc
    // asm 00000847: 	READADJ	ADJ_STEERCENTER
    // asm 00000849: 	LDI	R0,R2
    // asm 0000084A: 	LDI	@bufferI,AR2
    // asm 0000084B: 	CALL	_itoa
    // asm 0000084C: 	LDI	300,R2
    // asm 0000084D: 	LDI	100,R3
    // asm 0000084E: 	LDI	DEFAULT_COLOR,RC
    // asm 0000084F: 	CALL	_outtextxyc
    // asm 00000850: 	READADJ	ADJ_STEERMAX
    // asm 00000852: 	LDI	R0,R2
    // asm 00000853: 	LDI	@bufferI,AR2
    // asm 00000854: 	CALL	_itoa
    // asm 00000855: 	LDI	350,R2
    // asm 00000856: 	LDI	100,R3
    // asm 00000857: 	LDI	DEFAULT_COLOR,RC
    // asm 00000858: 	CALL	_outtextxyc
    // asm 00000859: 	READADJ	ADJ_BRAKEMIN
    // asm 0000085B: 	LDI	R0,R2
    // asm 0000085C: 	LDI	@bufferI,AR2
    // asm 0000085D: 	CALL	_itoa
    // asm 0000085E: 	LDI	250,R2
    // asm 0000085F: 	LDI	120,R3
    // asm 00000860: 	LDI	DEFAULT_COLOR,RC
    // asm 00000861: 	CALL	_outtextxyc
    // asm 00000862: 	READADJ	ADJ_BRAKEMAX
    // asm 00000864: 	LDI	R0,R2
    // asm 00000865: 	LDI	@bufferI,AR2
    // asm 00000866: 	CALL	_itoa
    // asm 00000867: 	LDI	350,R2
    // asm 00000868: 	LDI	120,R3
    // asm 00000869: 	LDI	DEFAULT_COLOR,RC
    // asm 0000086A: 	CALL	_outtextxyc
    // asm 0000086B: 	LDI	7FFFh,R0		;waste a little time
    // asm 0000086C: 	MPYI	10,R0
    // asm 0000086D: 	RPTS	R0
    // asm 0000086E: 	NOP
    // asm 0000086F: 	NOT	@_newbut,R0
    // asm 00000870: 	AND	SW_DIAG,R0
    // asm 00000871: 	BNZ	NOWTOEXIT
    // ;	RETSNZ
    // asm 00000872: 	CALL	WAIT_FOR_VBLANK		;wait for vblk
    // asm 00000873: 	B	SSLP
NOWTOEXIT:
    // asm 00000874: 	LDI	330,AR2
    // asm 00000875: 	LDI	AR2,R3
    // asm 00000876: 	ADDI	10,R3
    // asm 00000877: 	LDI	275,R2
    // asm 00000878: 	SUBI	2,R2
    // asm 00000879: 	LDI	R2,RC
    // asm 0000087A: 	ADDI	12,RC
    // asm 0000087B: 	LDI	COL_RED,RS
    // asm 0000087C: 	CALL	_fill
    // asm 0000087D: 	LDI	10,AR5
BBD5:
    // asm 0000087E: CALL	WAIT_FOR_VBLANK		;wait for vblk
    // asm 0000087F: 	DBU	AR5,BBD5
    // asm 00000880: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOWSWITCH", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *BOTTOM EIGHT BITS ONLY
 *
 *
 *PARAMETER
 *	AR2	STRING
 *	R2	VALUE
 *
 *
 */
static void ASCII_TO_BINARY(void) {
    // asm 00000881: 	LDI	0,RS
    // asm 00000882: 	LDI	0,R0	;output
    // asm 00000883: 	LDI	80h,R1	;mask
    // asm 00000884: 	LDI	7,AR3
    // asm 00000885: ATBLP
    // asm 00000885: 	TSTB	R1,R2
    // asm 00000886: 	LDIZ	'0',R3
    // asm 00000887: 	LDINZ	'1',R3
    // asm 00000888: 	LSH	RS,R3
    // asm 00000889: 	OR	R3,R0
    // asm 0000088A: 	RS	1,R1
    // asm 0000088B: 	ADDI	8,RS
    // asm 0000088C: 	CMPI	32,RS
    // asm 0000088D: 	BLT	JJ
    // asm 0000088E: 	LDI	0,RS
    // asm 0000088F: 	STI	R0,*AR2++
    // asm 00000890: 	CLRI	R0
JJ:
    // asm 00000891: 	DBU	AR3,ATBLP
    // asm 00000892: 	CLRI	R0
    // asm 00000893: 	STI	R0,*AR2++
    // asm 00000894: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASCII_TO_BINARY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: _onebut	.bss	_onebut,1 */
int _onebut;

static void DISPLAY_SWITCH_STATUS(void) {
    // asm 00000895: 	CALL	DISPLAY_SHIFTER
    // asm 00000896: 	LDI	@_newbut,R0
    // asm 00000897: 	LDI	@_onebut,R1
    // asm 00000898: 	CMPI	R1,R0
    // asm 00000899: 	RETSEQ
    // asm 0000089A: 	STI	R0,@_onebut
    // asm 0000089B: 	FILL	130,175,140,350,0	;MAIN
    // asm 000008A1: 	FILL	327,272,353,358,0	;TEST SWITCHES
    // asm 000008A7: 	LDI	@SWITCHES_DIAGI,AR6
DSS:
    // asm 000008A8: 	LDI	@_newbut,R0
    // asm 000008A9: 	LDI	*AR6++(3),R1
    // asm 000008AA: 	CMPI	-1,R1
    // asm 000008AB: 	BZ	DONEDSS
    // asm 000008AC: 	AND	R1,R0
    // asm 000008AD: 	BNZ	DSS
    // asm 000008AE: 	LDI	*-AR6(1),AR2
    // asm 000008AF: 	LDI	AR2,R3
    // asm 000008B0: 	ADDI	10,R3
    // asm 000008B1: 	LDI	*-AR6(2),R2
    // asm 000008B2: 	SUBI	2,R2
    // asm 000008B3: 	LDI	R2,RC
    // asm 000008B4: 	ADDI	12,RC
    // asm 000008B5: 	LDI	COL_RED,RS
    // asm 000008B6: 	CALL	_fill
    // asm 000008B7: NOPLOT
    // asm 000008B7: 	B	DSS
DONEDSS:
    // asm 000008B8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_SWITCH_STATUS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: SWITCHES_DIAG	.word	SW_VIEW0,180,130 */
/* asm: 	.word	SW_VIEW1,200,130 */
/* asm: 	.word	SW_VIEW2,220,130 */
/* asm: 	.word	SW_START,240,130 */
/* asm: 	.word	SW_COIN1,260,130 */
/* asm: 	.word	SW_COIN2,280,130 */
/* asm: 	.word	SW_COIN3,300,130 */
/* asm: 	.word	SW_COIN4,320,130 */
/* asm: 	.word	SW_RADIO,340,130 */
/* asm: 	.word	SW_DIAG,275,330 */
/* asm: 	.word	SW_VOLPLUS,295,330 */
/* asm: 	.word	SW_VOLMINUS,315,330 */
/* asm: 	.word	SW_COINSRV,335,330 */
/* asm: 	.word	-1 */
static int SWITCHES_DIAG[] = {
    SW_VIEW0,
    180,
    130,
    SW_VIEW1,
    200,
    130,
    SW_VIEW2,
    220,
    130,
    SW_START,
    240,
    130,
    SW_COIN1,
    260,
    130,
    SW_COIN2,
    280,
    130,
    SW_COIN3,
    300,
    130,
    SW_COIN4,
    320,
    130,
    SW_RADIO,
    340,
    130,
    SW_DIAG,
    275,
    330,
    SW_VOLPLUS,
    295,
    330,
    SW_VOLMINUS,
    315,
    330,
    SW_COINSRV,
    335,
    330,
    -1,
};
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: LASTSHIFT	.bss	LASTSHIFT,1 */
int LASTSHIFT;

static void DISPLAY_SHIFTER(void) {
    // asm 000008BA: 	NOT	@_newbut,R0
    // asm 000008BB: 	AND	3C00h,R0
    // asm 000008BC: 	CMPI	@LASTSHIFT,R0
    // asm 000008BD: 	RETSEQ
    // asm 000008BE: 	STI	R0,@LASTSHIFT
    // asm 000008BF: 	LDI	COL_VDGREY,RC
    // asm 000008C0: 	TEXTITT	"SHIFT 1",215,170
    // asm 000008C4: 	TEXTITT	"SHIFT 2",215,240
    // asm 000008C8: 	TEXTITT	"SHIFT 3",290,170
    // asm 000008CC: 	TEXTITT	"SHIFT 4",290,240
    // asm 000008D0: 	PUSH	R0
    // asm 000008D1: 	PUSH	AR2
    // asm 000008D2: 	FILL	230,180,325,230,0
    // asm 000008D8: 	POP	AR2
    // asm 000008D9: 	POP	R0
    // ; 400h = 4
    // ; 800h = 3
    // ;1000h = 2
    // ;2000h = 1
    // asm 000008DA: 	LDI	@_newbut,R0
    // asm 000008DB: 	TSTB	2000h,R0
    // asm 000008DC: 	BNZ	NOT1ST
    // asm 000008DD: 	FILL	230,180,250,200,COL_RED
NOT1ST:
    // asm 000008E3: 	LDI	@_newbut,R0
    // asm 000008E4: 	TSTB	1000h,R0
    // asm 000008E5: 	BNZ	NOT2ND
    // asm 000008E6: 	FILL	230,210,250,230,COL_RED
NOT2ND:
    // asm 000008EC: 	LDI	@_newbut,R0
    // asm 000008ED: 	TSTB	800h,R0
    // asm 000008EE: 	BNZ	NOT3RD
    // asm 000008EF: 	FILL	305,180,325,200,COL_RED
NOT3RD:
    // asm 000008F5: 	LDI	@_newbut,R0
    // asm 000008F6: 	TSTB	400h,R0
    // asm 000008F7: 	BNZ	NOT4TH
    // asm 000008F8: 	FILL	305,210,325,230,COL_RED
NOT4TH:
    // asm 000008FE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_SHIFTER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DISPPOTS(void) {
    // asm 000008FF: 	LDI	@_pot0,R2
    // asm 00000900: 	CMPI	@_opot0,R2
    // asm 00000901: 	BEQ	NOPLOTpot0
    // asm 00000902: 	STI	R2,@_opot0
    // asm 00000903: 	FILL	150,100,215,109,0
    // asm 00000909: 	LDI	@_pot0,R2
    // asm 0000090A: 	LDI	@bufferI,AR2
    // asm 0000090B: 	CALL	_itoa
    // asm 0000090C: 	LDI	150,R2
    // asm 0000090D: 	LDI	100,R3
    // asm 0000090E: 	LDI	COL_VDGREY,RC
    // asm 0000090F: 	CALL	_outtextxyc
NOPLOTpot0:
    // asm 00000910: 	LDI	@_pot1,R2
    // asm 00000911: 	CMPI	@_opot1,R2
    // asm 00000912: 	BEQ	NOPLOTpot1
    // asm 00000913: 	STI	R2,@_opot1
    // asm 00000914: 	FILL	150,110,215,119,0
    // asm 0000091A: 	LDI	@_pot1,R2
    // asm 0000091B: 	LDI	@bufferI,AR2
    // asm 0000091C: 	CALL	_itoa
    // asm 0000091D: 	LDI	150,R2
    // asm 0000091E: 	LDI	110,R3
    // asm 0000091F: 	LDI	COL_VDGREY,RC
    // asm 00000920: 	CALL	_outtextxyc
NOPLOTpot1:
    // asm 00000921: 	LDI	@_pot2,R2
    // asm 00000922: 	CMPI	@_opot2,R2
    // asm 00000923: 	BEQ	NOPLOTpot2
    // asm 00000924: 	STI	R2,@_opot2
    // asm 00000925: 	FILL	150,120,215,129,0
    // asm 0000092B: 	LDPI	@_pot2,R2
    // asm 0000092C: 	LDPI	@bufferI,AR2
    // asm 0000092D: 	CALL	_itoa
    // asm 0000092E: 	LDI	150,R2
    // asm 0000092F: 	LDI	120,R3
    // asm 00000930: 	LDI	COL_VDGREY,RC
    // asm 00000931: 	CALL	_outtextxyc
NOPLOTpot2:
    // asm 00000932: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPPOTS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static const char SW1T[] = "DIP SW2";
static const char SW2T[] = "DIP SW3";
/* asm: SW1BANK	.bss	SW1BANK,3 */
static int SW1BANK[3];
/* asm: SW2BANK	.bss	SW2BANK,3 */
static int SW2BANK[3];
/* asm: LASTDIPPLOT	.bss	LASTDIPPLOT,1 */
int LASTDIPPLOT;

static void SHOW_DIPSWITCH(void) {
    // asm 00000933: 	CALL	SETUP_DIAG_SCREEN
    // asm 00000934: 	LDI	COL_VDGREY,RC
    // asm 00000935: 	TEXTITT	"DIP SWITCH TEST",20,15
    // asm 00000939: 	LDI	-1,R0
    // asm 0000093A: 	STI	R0,@LASTDIPPLOT
    // asm 0000093B: 	CALL	TOEXIT
DIPLP:
    // asm 0000093C: 	CALL	DISPLAY_DIPSWITCH_STATUS
    // asm 0000093D: 	NOT	@_newbut,R0
    // asm 0000093E: 	AND	SW_DIAG,R0
    // asm 0000093F: 	RETSNZ
    // asm 00000940: 	CALL	WAIT_FOR_VBLANK		;wait for vblk
    // asm 00000941: 	B	DIPLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_DIPSWITCH", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	R5	X
 *	R6	Y
 *	R7	THE DIP
 *
 *vertical plotting of the dipswitches as 'on'|'off'
 *
 *
 */
/* asm: DIPTMP	.bss	DIPTMP,2 */
int DIPTMP[2];
static const char ONTEXT[] = "ON";
static const char OFFTEXT[] = "OFF";
/* asm: ISON	.word	ONTEXT */
static uintptr_t ISON = (uintptr_t)&ONTEXT;
/* asm: ISOFF	.word	OFFTEXT */
static uintptr_t ISOFF = (uintptr_t)&OFFTEXT;

// *
static void SHOW_DIPSWITCH_STATUS(void) {
    // asm 00000942: 	LDI	7,AR5
    // asm 00000943: 	LDI	1,R4		;Index Number
    // asm 00000944: SDS_LP
    // asm 00000944: 	TSTB	80h,R7
    // asm 00000945: 	LDIZ	@ISON,AR2
    // asm 00000946: 	LDINZ	@ISOFF,AR2
    // asm 00000947: 	PUSH	R7
    // asm 00000948: 	LDI	R5,R2
    // asm 00000949: 	ADDI	20,R2
    // asm 0000094A: 	LDI	R6,R3
    // asm 0000094B: 	LDI	COL_VDGREY,RC
    // asm 0000094C: 	CALL	_outtextxyc
    // asm 0000094D: 	LDI	R4,R2
    // asm 0000094E: 	LDL	DIPTMP,AR2
    // asm 0000094F: 	CALL	_itoa
    // asm 00000950: 	LDI	R5,R2
    // asm 00000951: 	LDI	R6,R3
    // asm 00000952: 	LDI	COL_VDGREY,RC
    // asm 00000953: 	CALL	_outtextxyc
    // asm 00000954: 	POP	R7
    // asm 00000955: 	LS	1,R7
    // asm 00000956: 	INC	R4
    // asm 00000957: 	ADDI	10,R6
    // asm 00000958: 	DBU	AR5,SDS_LP
    // asm 00000959: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_DIPSWITCH_STATUS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *
 */
static void DISPLAY_DIPSWITCH_STATUS(void) {
    // asm 0000095A: 	LDI	@DIPRAM,R0
    // asm 0000095B: 	CMPI	@LASTDIPPLOT,R0
    // asm 0000095C: 	RETSEQ
    // asm 0000095D: 	STI	R0,@LASTDIPPLOT
    // 	;BANK 1
    // asm 0000095E: 	LDI	COL_VDGREY,RC
    // asm 0000095F: 	TEXTITT	"DIP SW2 U97",30,40
    // asm 00000963: 	GROUP	20,50,400,170
    // asm 00000993: 	FILL	22,52,398,168,0
    // asm 00000999: 	LDI	30,R5
    // asm 0000099A: 	LDI	60,R6
    // asm 0000099B: 	CLRI	AR2
    // asm 0000099C: 	LDP	@DIPSW
    // asm 0000099D: 	LDI	@DIPSW,R7
    // asm 0000099E: 	LDI	*AR2,AR2
    // asm 0000099F: 	SETDP
    // asm 000009A0: 	RS	16,R7
    // asm 000009A1: 	CALL	SHOW_DIPSWITCH_STATUS
    // 	;BANK ONE (SW2) DECODED...
    // ;	.data
    // ;TONI	.word	TON
    // ;TOFFI	.word	TOFF
    // ;TON	.string	"TEST ON",0
    // ;TOFF	.string	"TEST OFF",0
    // ;	.text
    // ;
    // ;	LDI	90,R2			;DIAGNOSTICS
    // ;	LDI	60+(0*10),R3
    // ;	LDI	@DIPRAM,R0
    // ;	TSTB	DIP_DIAG,R0
    // ;	LDINZ	@TOFFI,AR2
    // ;	LDIZ	@TONI,AR2
    // ;	CALL	_outtextxyc
    // asm 000009A2: 	LDI	90,R2			;MOTION
    // asm 000009A3: 	LDI	60+(1*10),R3
    // asm 000009A4: 	LDI	@DIPRAM,R0
    // asm 000009A5: 	TSTB	DIP_MOTION,R0
    // asm 000009A6: 	LDINZ	@DNMOTI,AR2
    // asm 000009A7: 	LDIZ	@DMOTI,AR2
    // asm 000009A8: 	CALL	_outtextxyc
DNSUPI:
    // asm 000009A9: .word	DNSUP
DSUPI:
    // asm 000009A9: .word	DSUP
    // asm 000009A9: 	LDI	90,R2
    // asm 000009AA: 	LDI	60+(2*10),R3
    // asm 000009AB: 	LDI	@DIPRAM,R0
    // asm 000009AC: 	TSTB	DIP_STANDUP,R0
    // asm 000009AD: 	LDINZ	@DNSUPI,AR2
    // asm 000009AE: 	LDIZ	@DSUPI,AR2
    // asm 000009AF: 	CALL	_outtextxyc
    // ;	.data
    // ;RF1I	.word	RF1
    // ;RF2I	.word	RF2
    // ;RF1	.string	"FREEZE ON",0
    // ;RF2	.string	"FREEZE OFF",0
    // ;	.text
    // ;
    // ;	LDI	90,R2
    // ;	LDI	60+(3*10),R3
    // ;	LDI	@DIPRAM,R0
    // ;	TSTB	DIP_FREEZE,R0
    // ;	LDIZ	@RF1I,AR2
    // ;	LDINZ	@RF2I,AR2
    // ;	CALL	_outtextxyc
    // asm 000009B0: 	LDI	90,R2			;COMMUNICATION
    // asm 000009B1: 	LDI	60+(5*10),R3
    // asm 000009B2: 	LDI	@DIPRAM,R0
    // asm 000009B3: 	TSTB	DIP_COMMP,R0
    // asm 000009B4: 	LDINZ	@DNLINKDI,AR2
    // asm 000009B5: 	LDIZ	@DLINKDI,AR2
    // asm 000009B6: 	CALL	_outtextxyc
    // asm 000009B7: 	LDI	90,R2			;COMMUNICATION
    // asm 000009B8: 	LDI	60+(7*10),R3
    // asm 000009B9: 	LDI	@DIPRAM,R0
    // asm 000009BA: 	TSTB	CMDP_MASTER,R0
    // asm 000009BB: 	LDIZ	@DMOD1,AR2
    // asm 000009BC: 	LDINZ	@DMOD2,AR2
    // asm 000009BD: 	CALL	_outtextxyc
    // 	;BANK 2
    // asm 000009BE: 	LDI	COL_VDGREY,RC
    // asm 000009BF: 	TEXTITT	"DIP SW3",30,180
    // asm 000009C3: 	GROUP	20,190,400,310
    // asm 000009F3: 	FILL	22,192,398,308,0
    // asm 000009F9: 	LDI	30,R5
    // asm 000009FA: 	LDI	200,R6
    // asm 000009FB: 	CLRI	AR2
    // asm 000009FC: 	LDP	@DIPSW
    // asm 000009FD: 	LDI	@DIPSW,R7
    // asm 000009FE: 	LDI	*AR2,AR2
    // asm 000009FF: 	SETDP
    // asm 00000A00: 	RS	24,R7
    // asm 00000A01: 	CALL	SHOW_DIPSWITCH_STATUS
    // 	;BANK TWO (SW3) DECODED...
    // asm 00000A02: 	LINE	85,200,85,200+(6*10)+9,COL_VDGREY
    // asm 00000A08: 	CALL	GETCOIN_DEFAULT
    // asm 00000A09: 	PUSH	R2
    // asm 00000A0A: 	LDI	R2,AR0
    // asm 00000A0B: 	CALL	GET_THIS_COINTXT
    // asm 00000A0C: 	LDI	AR0,AR4
    // asm 00000A0D: 	LDI	*AR4++,AR2
    // asm 00000A0E: 	LDI	90,R2
    // asm 00000A0F: 	LDI	200+(3*10),R3
    // asm 00000A10: 	LDI	COL_VDGREY,RC
    // asm 00000A11: 	CALL	_outtextxyc
    // asm 00000A12: 	LDI	*AR4++,AR2
    // asm 00000A13: 	CMPI	0,AR2
    // asm 00000A14: 	BEQ	DONE_MODE_MSG
    // asm 00000A15: 	LDI	90,R2
    // asm 00000A16: 	LDI	200+(4*10),R3
    // asm 00000A17: 	LDI	COL_VDGREY,RC
    // asm 00000A18: 	CALL	_outtextxyc
    // asm 00000A19: 	LDI	*AR4++,AR2
    // asm 00000A1A: 	CMPI	0,AR2
    // asm 00000A1B: 	BEQ	DONE_MODE_MSG
    // asm 00000A1C: 	LDI	90,R2
    // asm 00000A1D: 	LDI	200+(5*10),R3
    // asm 00000A1E: 	LDI	COL_VDGREY,RC
    // asm 00000A1F: 	CALL	_outtextxyc
DONE_MODE_MSG:
    // asm 00000A20: 	POP	AR2
    // asm 00000A21: 	ADDI	@COINMSGC_TABI,AR2
    // asm 00000A22: 	LDI	*AR2,AR2
    // asm 00000A23: 	LDI	90,R2
    // asm 00000A24: 	LDI	200+(2*10),R3
    // asm 00000A25: 	LDI	COL_VDGREY,RC
    // asm 00000A26: 	CALL	_outtextxyc
    // ;	PUSH	AR2
    // ;	LDI	R2,AR0
    // ;	CALL	GET_THIS_COINTXT
    // ;	LDI	AR0,AR2
    // ;	LDI	90,R2
    // ;	LDI	200+(3*10),R3
    // ;	LDI	COL_VDGREY,RC
    // ;	CALL	_outtextxyc
    // ;
    // ;	POP	AR2
    // ;	LDI	*+AR2,AR2
    // ;	LDI	90,R2
    // ;	LDI	200+(2*10),R3
    // ;	LDI	COL_VDGREY,RC
    // ;	CALL	_outtextxyc
COINCNTR1I:
    // asm 00000A27: .word	COINCNTR1
COINCNTR2I:
    // asm 00000A27: .word	COINCNTR2
    // asm 00000A27: 	LDI	90,R2			;COMMUNICATION
    // asm 00000A28: 	LDI	200+(7*10),R3
    // asm 00000A29: 	LDI	@DIPRAM,R0
    // asm 00000A2A: 	TSTB	DIP_COINCNTR,R0
    // asm 00000A2B: 	LDINZ	@COINCNTR1I,AR2
    // asm 00000A2C: 	LDIZ	@COINCNTR2I,AR2
    // asm 00000A2D: 	CALL	_outtextxyc
    // asm 00000A2E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_DIPSWITCH_STATUS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static const char DMOT[] = "MOTION ON";
static const char DNMOT[] = "MOTION OFF";
static const char DLINKD[] = "LINK ON";
static const char DNLINKD[] = "LINK OFF";
/* asm: DMOD1	SPTR	"MASTER" */
static const char DMOD1[] = "MASTER";
/* asm: DMOD2	SPTR	"SLAVE" */
static const char DMOD2[] = "SLAVE";
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: MOMT	SPTR	"MOTION MENU" */
static const char MOMT[] = "MOTION MENU";

static void RUN_MOTION_MENU(void) {
    // asm 00000A36: 	LDL	MOTION_MENU,AR5
    // asm 00000A37: 	LDI	MOTION_MENU_LEN,AR6
    // asm 00000A38: 	LDI	@MOMT,AR7
    // asm 00000A39: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_MOTION_MENU", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
/* asm: AMT	SPTR	"ADJUSTMENT MENU" */
static const char AMT[] = "ADJUSTMENT MENU";

static void RUN_ADJUSTMENT_MENU(void) {
    // asm 00000A3B: 	LDL	ADJUSTMENT_MENU,AR5
    // asm 00000A3C: 	LDI	ADJUSTMENT_MENU_LEN,AR6
    // asm 00000A3D: 	LDI	@AMT,AR7
    // asm 00000A3E: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_ADJUSTMENT_MENU", 0, 0);
    UNIMPL();
}

/* asm: ADJTAB */
/* asm: 	 */
/* asm: 	.word	ADJ_COINMODE,PRINT_COINMODE */
/* asm: 	.word	ADJ_STANDARD_PRICING,PRINT_CUSTOM_SELECTED */
/* asm: 	 */
/* asm: 	.word	ADJ_FREE_PLAY,PRINT_ONOFF */
/* asm: 	.word	ADJ_FREEGAME,PRINT_ONOFF */
/* asm: 	.word	ADJ_TIME_TO_START,PRINT_TIME */
/* asm: 	.word	ADJ_CHECKPOINT_BONUS,PRINT_NUMBER */
/* asm: 	.word	ADJ_ATTRACT_MODE_SOUND,PRINT_ONOFF */
/* asm: 	.word	ADJ_HIGH_SCORE_ENTRY,PRINT_ONOFF */
/* asm: 	.word	ADJ_MIN_VOL_LEVEL,PRINT_VOL */
/* asm: 	.word	ADJ_MPHORKPM,PRINT_MPH */
/* asm: 	.word	ADJ_ROADKILL,PRINT_ONOFF */
/* asm: 	.word	ADJ_CLINTON,PRINT_ONOFF */
/* asm: 	.word	ADJ_GIRLS,PRINT_ONOFF */
/* asm: 	.word	ADJ_HIGHSCORE_RESET,PRINT_NUMBER */
/* asm: 	.word	ADJ_DIFFICULTY,PRINT_NUMBER */
/* asm: 	.word	ADJ_MAX_CREDITS,PRINT_NUMBER */
/* asm: 	.word	ADJ_STEERING_SENSITIVITY,PRINT_NUMBER */
/* asm: 	.word	0,0 */
static uintptr_t ADJTAB[] = {
    (uintptr_t)(ADJ_COINMODE),
    (uintptr_t)PRINT_COINMODE,
    (uintptr_t)(ADJ_STANDARD_PRICING),
    (uintptr_t)PRINT_CUSTOM_SELECTED,
    // 	.word	0,0
    // 	.word	0,0
    (uintptr_t)(ADJ_FREE_PLAY),
    (uintptr_t)PRINT_ONOFF,
    (uintptr_t)(ADJ_FREEGAME),
    (uintptr_t)PRINT_ONOFF,
    (uintptr_t)(ADJ_TIME_TO_START),
    (uintptr_t)PRINT_TIME,
    (uintptr_t)(ADJ_CHECKPOINT_BONUS),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_ATTRACT_MODE_SOUND),
    (uintptr_t)PRINT_ONOFF,
    (uintptr_t)(ADJ_HIGH_SCORE_ENTRY),
    (uintptr_t)PRINT_ONOFF,
    (uintptr_t)(ADJ_MIN_VOL_LEVEL),
    (uintptr_t)PRINT_VOL,
    (uintptr_t)(ADJ_MPHORKPM),
    (uintptr_t)PRINT_MPH,
    (uintptr_t)(ADJ_ROADKILL),
    (uintptr_t)PRINT_ONOFF,
    (uintptr_t)(ADJ_CLINTON),
    (uintptr_t)PRINT_ONOFF,
    (uintptr_t)(ADJ_GIRLS),
    (uintptr_t)PRINT_ONOFF,
    (uintptr_t)(ADJ_HIGHSCORE_RESET),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_DIFFICULTY),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_MAX_CREDITS),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_STEERING_SENSITIVITY),
    (uintptr_t)PRINT_NUMBER,
    0,
    0,
};

static void PRINT_ADJUSTMENT(void) {
    // asm 00000A40: 	PUSH	AR4
    // asm 00000A41: 	LDI	@ADJTABI,AR4
    // asm 00000A42: 	CALL	PRINT_VALUE
    // asm 00000A43: 	POP	AR4
    // asm 00000A44: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_ADJUSTMENT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
/* asm: UMT	SPTR	"UTILITIES MENU" */
static const char UMT[] = "UTILITIES MENU";

static void RUN_UTILITIES_MENU(void) {
    // asm 00000A46: 	LDL	UTILITIES_MENU,AR5
    // asm 00000A47: 	LDI	UTILITIES_MENU_LEN,AR6
    // asm 00000A48: 	LDI	@UMT,AR7
    // asm 00000A49: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_UTILITIES_MENU", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
/* asm: MMT	SPTR	"MAIN MENU" */
static const char MMT[] = "MAIN MENU";
/* asm: DMT	SPTR	"DIAGNOSTIC MENU" */
static const char DMT[] = "DIAGNOSTIC MENU";

static void RUN_DIAG_MENU(void) {
    // asm 00000A4C: 	LDL	DIAGNOSTIC_MENU,AR5
    // asm 00000A4D: 	LDI	DIAGNOSTIC_MENU_LEN,AR6
    // asm 00000A4E: 	LDI	@DMT,AR7
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_DIAG_MENU", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR5	POINTER TO MENU TABLE
 *	AR6	LENGTH OF MENU
 *	AR7	SPTR MENU NAME
 *
 */

/* asm: MENU_SCROLL	.bss	MENU_SCROLL,1 */
int MENU_SCROLL;

static void HANDLE_MENU(void) {
    // asm 00000A4F: 	LDI	0,R0
    // asm 00000A50: 	STI	R0,@MENU_SCROLL
    // asm 00000A51: 	CLRI	R6			;MENU INDEX
HMER:
    // asm 00000A52: 	PUSH	R6
    // asm 00000A53: 	LDI	@DIAGPAL+1,R0
    // asm 00000A54: 	STI	R0,@BGNDCOLA
    // asm 00000A55: 	PUSH	AR7
    // asm 00000A56: 	CALL	SETUP_DIAG_SCREEN
    // asm 00000A57: 	LDI	COL_VDGREY,RC
    // asm 00000A58: 	TEXTIT	title,30,20
    // asm 00000A5C: 	TEXTIT	inst1,20,260
    // asm 00000A60: 	TEXTIT	inst2,20,270
    // asm 00000A64: 	LDI	*-AR5,R2		;Routine that prints the value
    // asm 00000A65: 	BEQ	HMSTAND
    // asm 00000A66: 	LDI	*-AR5(2),R2
    // asm 00000A67: 	BEQ	HMSTAND
    // asm 00000A68: 	CALLU	R2
    // asm 00000A69: 	BR	HMNOTSTAND
HMSTAND:
    // asm 00000A6A: 	IWINDOW	20,45,280,250
HMNOTSTAND:
    // asm 00000A6F: 	LDI	30,R2
    // asm 00000A70: 	LDI	35,R3
    // asm 00000A71: 	LDI	COL_VDGREY,RC
    // asm 00000A72: 	POP	AR7
    // asm 00000A73: 	LDI	AR7,AR2
    // asm 00000A74: 	CALL	_outtextxyc
    // asm 00000A75: 	LDI	10000,R7		;wait_mac counter
    // asm 00000A76: 	MPYI	55,R7
    // asm 00000A77: 	POP	R6
HANMENL:
    // asm 00000A78: 	LDI	@MENU_SCROLL,R0
    // ;	CLRI	R0
    // ;	LDI	AR5,AR4
    // asm 00000A79: 	LDI	2,AR4			;OFFSET AR4 to MENU_SCROLL
    // asm 00000A7A: 	MPYI	R0,AR4
    // asm 00000A7B: 	ADDI	AR5,AR4
    // asm 00000A7C: 	LDI	50,R3			;Y POSITION OF TEXT
    // asm 00000A7D: 	PUSH	AR7
    // asm 00000A7E: 	LDI	AR6,AR7
    // asm 00000A7F: 	SUBI	@MENU_SCROLL,AR7
    // asm 00000A80: 	CMPI	20,AR7
    // asm 00000A81: 	LDIGT	20,AR7
    // asm 00000A82: 	DEC	AR7
    // asm 00000A83: HANMENL_PLOT
    // asm 00000A83: 	LDI	COL_VDGREY,RC
    // asm 00000A84: 	CMPI	R0,R6
    // asm 00000A85: 	LDIEQ	COL_RED,RC
    // asm 00000A86: 	LDI	*AR4++(2),AR2
    // asm 00000A87: 	LDI	30,R2
    // asm 00000A88: 	CALL	_outtextxyc
    // asm 00000A89: 	LDI	*-AR5,R5		;Routine that prints the value
    // asm 00000A8A: 	CALLNE	R5
    // asm 00000A8B: 	ADDI	10,R3
    // asm 00000A8C: 	INC	R0
    // asm 00000A8D: 	DBU	AR7,HANMENL_PLOT
    // asm 00000A8E: 	POP	AR7
    // asm 00000A8F: 	LDI	@_newbut,R0		;old system method
    // asm 00000A90: 	LDI	R0,R1
    // asm 00000A91: 	TSTB    SW_COINSRV,R1
    // asm 00000A92: 	BNZ	JAJA5
    // ;this locks on the main menu.
    // ;fix later
    // asm 00000A93: 	LDL	MAIN_MENU,AR0
    // asm 00000A94: 	CMPI	AR0,AR5
    // asm 00000A95: 	BEQ	JAJA5
    // asm 00000A96: 	LDI	982,AR2
    // asm 00000A97: 	CALL	SENDSND
    // asm 00000A98: 	PUSH	AR5
    // asm 00000A99: 	PUSH	AR6
    // asm 00000A9A: 	PUSH	AR7
    // asm 00000A9B: 	PUSH	R6
    // asm 00000A9C: 	CALL	XIT_MENU_TO_MAIN
    // ;
    // ;	CALLZ	ESCAPE_TO_MAIN
JAJA5:
    // asm 00000A9D: 	TSTB    SW_DIAG,R1
    // asm 00000A9E: 	BZ	HM_EXECUTE
    // asm 00000A9F: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm 00000AA0: 	BNZ	HM_NOTUP
    // asm 00000AA1: 	LDI	980,AR2
    // asm 00000AA2: 	CALL	SENDSND
    // asm 00000AA3: 	ADDI	1,R6
    // asm 00000AA4: 	CMPI	AR6,R6
    // asm 00000AA5: 	BLT	HM_WT
    // asm 00000AA6: 	LDI	AR6,R6
    // asm 00000AA7: 	DEC	R6
    // asm 00000AA8: 	B	HM_WT
HM_NOTUP:
    // asm 00000AA9: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm 00000AAA: 	BNZ	HM_NOTDOWN
    // asm 00000AAB: 	LDI	981,AR2
    // asm 00000AAC: 	CALL	SENDSND
    // asm 00000AAD: 	SUBI	1,R6
    // asm 00000AAE: 	LDILT	0,R6
HM_WT:
    // ;	WAIT_MAC
    // asm 00000AAF: 	LDI	R7,R1
wta:
    // asm 00000AB0: 	DEC	R1
    // asm 00000AB1: 	BLT	wtta
    // asm 00000AB2: 	LDI	@_newbut,R0
    // asm 00000AB3: 	AND	SW_VOLPLUS|SW_VOLMINUS,R0
    // asm 00000AB4: 	CMPI	SW_VOLPLUS|SW_VOLMINUS,R0
    // asm 00000AB5: 	BNE	wta
    // asm 00000AB6: 	LDI	10000,R7
    // asm 00000AB7: 	MPYI	55,R7
    // asm 00000AB8: 	BU	J55
wtta:
    // asm 00000AB9: 	LDI	5000,R7
    // asm 00000ABA: 	MPYI	55,R7
J55:
HM_NOTDOWN:
    // asm 00000ABB: 	CMPI	0,R6
    // asm 00000ABC: 	BEQ	NO_SCROLL
    // asm 00000ABD: 	LDI	0,R2
    // asm 00000ABE: 	LDI	@MENU_SCROLL,R0
    // asm 00000ABF: 	LDI	R0,R1
    // asm 00000AC0: 	ADDI	20,R1
    // asm 00000AC1: 	CMPI	R0,R6
    // asm 00000AC2: 	BLT	GO_SCROLL
    // asm 00000AC3: 	CMPI	R1,AR6
    // asm 00000AC4: 	BLE	NO_SCROLL
GO_SCROLL:
    // asm 00000AC5: 	CMPI	R0,R6
    // asm 00000AC6: 	LDILT	-10,R2
    // asm 00000AC7: 	CMPI	R1,R6
    // asm 00000AC8: 	LDIGE	10,R2
    // asm 00000AC9: 	ADDI	R2,R0
    // asm 00000ACA: 	LDIN	0,R0
    // asm 00000ACB: 	STI	R0,@MENU_SCROLL
    // asm 00000ACC: 	CMPI	0,R2
    // asm 00000ACD: 	BNE	HMER			;REDRAW entir screen if scrolled
NO_SCROLL:
    // asm 00000ACE: 	B	HANMENL
HM_EXECUTE:
    // asm 00000ACF: 	LDI	982,AR2
    // asm 00000AD0: 	CALL	SENDSND
    // asm 00000AD1: 	LDI	R6,AR0
    // asm 00000AD2: 	MPYI	2,AR0
    // asm 00000AD3: 	ADDI	AR5,AR0
    // asm 00000AD4: 	LDI	*+AR0(1),R0
    // asm 00000AD5: 	BZ	HANMENL
    // asm 00000AD6: 	PUSH	AR5
    // asm 00000AD7: 	PUSH	AR6
    // asm 00000AD8: 	PUSH	AR7
    // asm 00000AD9: 	PUSH	R6
    // asm 00000ADA: 	CALLU	R0
    // asm 00000ADB: 	POP	R6
    // asm 00000ADC: 	POP	AR7
    // asm 00000ADD: 	POP	AR6
    // asm 00000ADE: 	POP	AR5
    // asm 00000ADF: 	B	HMER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HANDLE_MENU", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
;ESCAPE_TO_MAIN:
;	LDI	982,AR2
;	CALL	SENDSND
;	POP	R0
;	RETS
*/
static void XIT_MENU_TO_MAIN(void) {
    // asm 00000AE0: 	LDI	0,R0
    // asm 00000AE1: 	STI	R0,@MENU_SCROLL
    // asm 00000AE2: 	CALL	LAMPSOFF
    // asm 00000AE3: 	POP	R0	;return address
    // asm 00000AE4: 	POP	R6
    // asm 00000AE5: 	POP	AR7
    // asm 00000AE6: 	POP	AR6
    // asm 00000AE7: 	POP	AR5
    // asm 00000AE8: 	RETS
    // ;	BR	RETURN_TO_MAINMENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "XIT_MENU_TO_MAIN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SETUP_DIAG_SCREEN(void) {
    // asm 00000AE9: 	CALL	CLRSCRN
    // asm 00000AEA: 	CALL	SETPAGE0
    // asm 00000AEB: 	LDIL	SCREEN0,R0
    // asm 00000AEE: 	STI	R0,@ACTIVE_SCREEN
    // asm 00000AEF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETUP_DIAG_SCREEN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

/* asm: MUSIC_TAB */
/* asm: SNDMAC	"REDLINE SHUFFLE",SHUFFLE_DRIV */
/* asm: SNDMAC	"HOUSE SPECIAL",DISCODUCK */
/* asm: SNDMAC	"DEADWOOD RIDE",SPAGETTI */
static int MUSIC_TAB;
/* asm: EFFECTS_TAB */
/* asm: SNDMAC	"GIRL SPEECH",CHOOSEUCAR */
/* asm: SNDMAC	"TRUCK HORN",TRUCKHORN_BLAST */
/* asm: SNDMAC	"EXPLOSION 1",EXP1 */
/* asm: SNDMAC	"COLLISION",SCOLLF */
/* asm: SNDMAC	"CAR PASS",CARPASS */
static int EFFECTS_TAB;

static void SOUND_BOARD_TEST(void) {
    // asm 00000B02: 	CALL	SETUP_DIAG_SCREEN
    // asm 00000B03: 	CLRI	R5	;TUNE INDEX
    // asm 00000B04: 	CLRI	R6	;SNDFX INDEX
    // asm 00000B05: 	LDI	COL_VDGREY,RC
    // asm 00000B06: 	TEXTITT	"SOUND BOARD TEST",50,20
    // asm 00000B0A: 	TEXTITT	"ENTER  TO EXIT",20,100
    // asm 00000B0E: 	TEXTITT	"UP     TO CHANGE MUSIC",20,110
    // asm 00000B12: 	TEXTITT	"DOWN   TO TRIGGER SOUND EFFECT",20,120
    // asm 00000B16: 	LDL	MUSIC_TAB,AR5
    // asm 00000B17: 	LDL	EFFECTS_TAB,AR6
    // asm 00000B18: 	LDI	-1,R5
    // asm 00000B19: 	LDI	-1,R6
SBT_UPDT:
    // asm 00000B1A: 	CMPI	AR5,R5
    // asm 00000B1B: 	BEQ	NUPDT
    // asm 00000B1C: 	LDI	AR5,R5
    // asm 00000B1D: 	FILL	20,200,200,220,0
    // asm 00000B23: 	LDI	COL_VDGREY,RC
    // asm 00000B24: 	TEXTITT	"TUNE",20,200
    // asm 00000B28: 	LDI	*AR5,AR2
    // asm 00000B29: 	LDI	30,R2
    // asm 00000B2A: 	LDI	210,R3
    // asm 00000B2B: 	CALL	_outtextxyc
    // asm 00000B2C: 	LDI	*+AR5,AR2
    // asm 00000B2D: 	CALL	HARDSND
NUPDT:
    // asm 00000B2E: 	CMPI	AR6,R6
    // asm 00000B2F: 	BEQ	NUPDE
    // asm 00000B30: 	LDI	AR6,R6
    // asm 00000B31: 	FILL	20,250,200,270,0
    // asm 00000B37: 	LDI	COL_VDGREY,RC
    // asm 00000B38: 	TEXTITT	"SOUND EFFECT",20,250
    // asm 00000B3C: 	LDI	*AR6,AR2
    // asm 00000B3D: 	LDI	30,R2
    // asm 00000B3E: 	LDI	260,R3
    // asm 00000B3F: 	CALL	_outtextxyc
    // asm 00000B40: 	LDI	*+AR6,AR2
    // asm 00000B41: 	CALL	HARDSND
NUPDE:
SBT_LP:
    // asm 00000B42: 	NOT	@_newbut,R0
    // asm 00000B43: 	AND	SW_DIAG,R0
    // asm 00000B44: 	BZ	NOXIT
    // asm 00000B45: 	CALL	SILENT
    // asm 00000B46: 	RETS
NOXIT:
    // asm 00000B47: 	NOT	@_newbut,R0
    // asm 00000B48: 	AND	SW_VOLPLUS,R0
    // asm 00000B49: 	BZ	NONEXTT
SBTLP2:
    // asm 00000B4A: CALL	WAIT_FOR_VBLANK
    // asm 00000B4B: 	NOT	@_newbut,R0
    // asm 00000B4C: 	AND	SW_VOLPLUS,R0
    // asm 00000B4D: 	BNZ	SBTLP2
    // asm 00000B4E: 	ADDI	2,AR5
    // asm 00000B4F: 	LDI	*AR5,R0
    // asm 00000B50: 	BNN	SBT_UPDT
    // asm 00000B51: 	LDL	MUSIC_TAB,AR5
    // asm 00000B52: 	BU	SBT_UPDT
NONEXTT:
    // asm 00000B53: 	NOT	@_newbut,R0
    // asm 00000B54: 	AND	SW_VOLMINUS,R0
    // asm 00000B55: 	BZ	NONEXTE
SBTLP1:
    // asm 00000B56: CALL	WAIT_FOR_VBLANK
    // asm 00000B57: 	NOT	@_newbut,R0
    // asm 00000B58: 	AND	SW_VOLMINUS,R0
    // asm 00000B59: 	BNZ	SBTLP1
    // asm 00000B5A: 	ADDI	2,AR6
    // asm 00000B5B: 	LDI	*AR6,R0
    // asm 00000B5C: 	BNN	SBT_UPDT
    // asm 00000B5D: 	LDL	EFFECTS_TAB,AR6
    // asm 00000B5E: 	BU	SBT_UPDT
NONEXTE:
    // asm 00000B5F: 	LDI	7FFFh,R0
    // asm 00000B60: 	MPYI	10,R0
    // asm 00000B61: 	RPTS	R0
    // asm 00000B62: 	NOP
    // asm 00000B63: 	CALL	WAIT_FOR_VBLANK
    // asm 00000B64: 	BU	SBT_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SOUND_BOARD_TEST", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void STEERING_WHEEL_TEST(void) {
    // asm 00000B65: 	CALL	SETUP_DIAG_SCREEN
    // asm 00000B66: 	LDI	COL_VDGREY,RC
    // asm 00000B67: 	TEXTITT	"STEERING WHEEL TEST",20,50
    // asm 00000B6B: 	TEXTITT	"MOVING WHEEL TO LEFT",20,100
    // asm 00000B6F: 	CLRI	R6
    // *	R0	BYTE TO SEND TO WHEEL
    // asm 00000B70: 	LDI	127,AR5
    // asm 00000B71: SWTLP1
    // asm 00000B71: 	INC	R6
    // asm 00000B72: 	CMPI	127,R6
    // asm 00000B73: 	LDIGT	127,R6
    // asm 00000B74: 	LDI	R6,R0
    // asm 00000B75: 	OR	080h,R0
    // asm 00000B76: 	CALL	TOWHEEL
    // asm 00000B77: 	CALL	WAIT_FOR_VBLANK
    // asm 00000B78: 	DBU	AR5,SWTLP1
    // asm 00000B79: 	LDI	COL_VDGREY,RC
    // asm 00000B7A: 	TEXTITT	"HIT ENTER TO CONTINUE",20,120
    // asm 00000B7E: 	CALL	WT_NSTART
    // asm 00000B7F: 	CALL	WT_START
    // asm 00000B80: 	CALL	BEEP
    // asm 00000B81: 	LDI	COL_VDGREY,RC
    // asm 00000B82: 	TEXTITT	"MOVING WHEEL TO RIGHT",20,140
    // asm 00000B86: 	CLRI	R6
    // asm 00000B87: 	LDI	127,AR5
    // asm 00000B88: SWTLP3
    // asm 00000B88: 	INC	R6
    // asm 00000B89: 	CMPI	127,R6
    // asm 00000B8A: 	LDIGT	127,R6
    // asm 00000B8B: 	LDI	R6,R0
    // asm 00000B8C: 	CALL	TOWHEEL
    // asm 00000B8D: 	CALL	WAIT_FOR_VBLANK
    // asm 00000B8E: 	DBU	AR5,SWTLP3
    // asm 00000B8F: 	LDI	COL_VDGREY,RC
    // asm 00000B90: 	TEXTITT	"HIT ENTER TO CONTINUE",20,160
    // asm 00000B94: 	CALL	WT_NSTART
    // asm 00000B95: 	CALL	WT_START
    // asm 00000B96: 	CALL	BEEP
    // asm 00000B97: 	CLRI	R0
    // asm 00000B98: 	CALL	TOWHEEL
    // asm 00000B99: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "STEERING_WHEEL_TEST", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: AMODES	SPTR	"ATTRACT MODE SOUNDS" */
static const char AMODES[] = "ATTRACT MODE SOUNDS";

static void AMODESND(void) {
    // asm 00000B9B: 	READAUD	ADJ_ATTRACT_MODE_SOUND
    // asm 00000B9D: 	LDI	R0,R5
    // asm 00000B9E: 	LDI	@AMODES,AR2
    // asm 00000B9F: 	LDI	@NULLSTR,AR3
    // asm 00000BA0: 	CALL	DUAL_ON_OFF
    // asm 00000BA1: 	LDI	R5,R2
    // asm 00000BA2: 	SETADJ	ADJ_ATTRACT_MODE_SOUND
    // asm 00000BA4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AMODESND", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void BURNIN_TEST(void) {
    // asm 00000BA5: 	CALL	TEST_CHIPS
    // asm 00000BA6: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm 00000BAA: 	DINT
    // asm 00000BB0:         LDP	@FASTSTKI
    // asm 00000BB1:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm 00000BB2: 	LDI	AUD_SUCCESSFULL_BURNINS,AR2
    // asm 00000BB3: 	CALL	AUDIT_INC
    // 	;IGNORE GLITCH FIX
    // asm 00000BB4: 	LDP	@SWITCH1
    // asm 00000BB5: 	LDI	@SWITCH1,R0
    // asm 00000BB6: 	SETDP
    // asm 00000BB7: 	TSTB	SW_DIAG,R0
    // asm 00000BB8: 	BNZ	BURNIN_TEST
    // asm 00000BB9: 	BU	ENTER_DIAG
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BURNIN_TEST", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

// ;	.ref	COINMSG_TABI	;????

#define STANDARD_PRICING_MENU_LEN 16
/* asm: STANDARD_PRICING_MENU */
/* asm: MENUENTRY	"USA",RUN_COUNTRY */
/* asm: MENUENTRY	"GERMAN",RUN_COUNTRY */
/* asm: MENUENTRY	"FRENCH",RUN_COUNTRY */
/* asm: MENUENTRY	"CANADA",RUN_COUNTRY */
/* asm: MENUENTRY	"SWISS",RUN_COUNTRY */
/* asm: MENUENTRY	"ITALY",RUN_COUNTRY */
/* asm: MENUENTRY	"UK",RUN_COUNTRY */
/* asm: MENUENTRY	"SPAIN",RUN_COUNTRY */
/* asm: MENUENTRY	"AUSTRALIA",RUN_COUNTRY */
/* asm: MENUENTRY	"JAPAN",RUN_COUNTRY */
/* asm: MENUENTRY	"TAIWAN",RUN_COUNTRY */
/* asm: MENUENTRY	"AUSTRIA",RUN_COUNTRY */
/* asm: MENUENTRY	"BELGIUM",RUN_COUNTRY */
/* asm: MENUENTRY	"OTHER",RUN_COUNTRY */
/* asm: MENUENTRY	"GENERAL",RUN_COUNTRY */
/* asm: MENUENTRY	"EXIT TO MAIN",XIT_MENU_TO_MAIN */
/* asm: romdata */
static int STANDARD_PRICING_MENU;
#define USA_MENU_LEN 13
/* asm: USA_MENU */
/* asm: 	.word	MSGC_USA1,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA2,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA3,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA4,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA5,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA6,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA7,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA8,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA9,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA10,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA11,PRICE_YES_NO */
/* asm: 	.word	MSGC_USA12,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t USA_MENU[] = {
    (uintptr_t)&MSGC_USA1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA4,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA5,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA6,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA7,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA8,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA9,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA10,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA11,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_USA12,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define GERMAN_MENU_LEN 9
/* asm: GERMAN_MENU */
/* asm: 	.word	MSGC_GERMAN1,PRICE_YES_NO */
/* asm: 	.word	MSGC_GERMAN2,PRICE_YES_NO */
/* asm: 	.word	MSGC_GERMAN3,PRICE_YES_NO */
/* asm: 	.word	MSGC_GERMAN4,PRICE_YES_NO */
/* asm: 	.word	MSGC_GERMAN5,PRICE_YES_NO */
/* asm: 	.word	MSGC_GERMAN6,PRICE_YES_NO */
/* asm: 	.word	MSGC_GERMAN7,PRICE_YES_NO */
/* asm: 	.word	MSGC_GERMAN8,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t GERMAN_MENU[] = {
    (uintptr_t)&MSGC_GERMAN1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GERMAN2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GERMAN3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GERMAN4,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GERMAN5,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GERMAN6,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GERMAN7,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GERMAN8,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define FRENCH_MENU_LEN 12
/* asm: FRENCH_MENU */
/* asm: 	.word	MSGC_FRENCH1,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH2,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH3,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH4,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH5,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH6,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH7,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH8,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH9,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH10,PRICE_YES_NO */
/* asm: 	.word	MSGC_FRENCH11,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t FRENCH_MENU[] = {
    (uintptr_t)&MSGC_FRENCH1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH4,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH5,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH6,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH7,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH8,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH9,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH10,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FRENCH11,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define CANADA_MENU_LEN 4
/* asm: CANADA_MENU */
/* asm: 	.word	MSGC_CANADA1,PRICE_YES_NO */
/* asm: 	.word	MSGC_CANADA2,PRICE_YES_NO */
/* asm: 	.word	MSGC_CANADA3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t CANADA_MENU[] = {
    (uintptr_t)&MSGC_CANADA1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_CANADA2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_CANADA3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define SWISS_MENU_LEN 4
/* asm: SWISS_MENU */
/* asm: 	.word	MSGC_SWISS1,PRICE_YES_NO */
/* asm: 	.word	MSGC_SWISS2,PRICE_YES_NO */
/* asm: 	.word	MSGC_SWISS3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t SWISS_MENU[] = {
    (uintptr_t)&MSGC_SWISS1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_SWISS2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_SWISS3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define ITALY_MENU_LEN 4
/* asm: ITALY_MENU */
/* asm: 	.word	MSGC_ITALY1,PRICE_YES_NO */
/* asm: 	.word	MSGC_ITALY2,PRICE_YES_NO */
/* asm: 	.word	MSGC_ITALY3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t ITALY_MENU[] = {
    (uintptr_t)&MSGC_ITALY1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_ITALY2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_ITALY3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define UK_MENU_LEN 4
/* asm: UK_MENU */
/* asm: 	.word	MSGC_UK1,PRICE_YES_NO */
/* asm: 	.word	MSGC_UK2,PRICE_YES_NO */
/* asm: 	.word	MSGC_UK3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t UK_MENU[] = {
    (uintptr_t)&MSGC_UK1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_UK2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_UK3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define SPAIN_MENU_LEN 7
/* asm: SPAIN_MENU */
/* asm: 	.word	MSGC_SPAIN1,PRICE_YES_NO */
/* asm: 	.word	MSGC_SPAIN2,PRICE_YES_NO */
/* asm: 	.word	MSGC_SPAIN3,PRICE_YES_NO */
/* asm: 	.word	MSGC_SPAIN4,PRICE_YES_NO */
/* asm: 	.word	MSGC_SPAIN5,PRICE_YES_NO */
/* asm: 	.word	MSGC_SPAIN6,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t SPAIN_MENU[] = {
    (uintptr_t)&MSGC_SPAIN1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_SPAIN2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_SPAIN3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_SPAIN4,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_SPAIN5,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_SPAIN6,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define AUSTRALIA_MENU_LEN 5
/* asm: AUSTRALIA_MENU */
/* asm: 	.word	MSGC_AUSTRALIA1,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRALIA2,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRALIA3,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRALIA4,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t AUSTRALIA_MENU[] = {
    (uintptr_t)&MSGC_AUSTRALIA1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_AUSTRALIA2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_AUSTRALIA3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_AUSTRALIA4,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define JAPAN_MENU_LEN 4
/* asm: JAPAN_MENU */
/* asm: 	.word	MSGC_JAPAN1,PRICE_YES_NO */
/* asm: 	.word	MSGC_JAPAN2,PRICE_YES_NO */
/* asm: 	.word	MSGC_JAPAN3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t JAPAN_MENU[] = {
    (uintptr_t)&MSGC_JAPAN1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_JAPAN2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_JAPAN3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define TAIWAN_MENU_LEN 4
/* asm: TAIWAN_MENU */
/* asm: 	.word	MSGC_TAIWAN1,PRICE_YES_NO */
/* asm: 	.word	MSGC_TAIWAN2,PRICE_YES_NO */
/* asm: 	.word	MSGC_TAIWAN3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t TAIWAN_MENU[] = {
    (uintptr_t)&MSGC_TAIWAN1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_TAIWAN2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_TAIWAN3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define AUSTRIA_MENU_LEN 5
/* asm: AUSTRIA_MENU */
/* asm: 	.word	MSGC_AUSTRIA1,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRIA2,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRIA3,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRIA4,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t AUSTRIA_MENU[] = {
    (uintptr_t)&MSGC_AUSTRIA1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_AUSTRIA2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_AUSTRIA3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_AUSTRIA4,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define BELGIUM_MENU_LEN 4
/* asm: BELGIUM_MENU */
/* asm: 	.word	MSGC_BELGIUM1,PRICE_YES_NO */
/* asm: 	.word	MSGC_BELGIUM2,PRICE_YES_NO */
/* asm: 	.word	MSGC_BELGIUM3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
/* asm: 	 */
static uintptr_t BELGIUM_MENU[] = {
    (uintptr_t)&MSGC_BELGIUM1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_BELGIUM2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_BELGIUM3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define OTHER_MENU_LEN 8
/* asm: OTHER_MENU */
/* asm: 	.word	MSGC_SWEDEN1,PRICE_YES_NO */
/* asm: 	.word	MSGC_FINLAND1,PRICE_YES_NO */
/* asm: 	.word	MSGC_NETHERLAND1,PRICE_YES_NO */
/* asm: 	.word	MSGC_NETHERLAND2,PRICE_YES_NO */
/* asm: 	.word	MSGC_NORWAY1,PRICE_YES_NO */
/* asm: 	.word	MSGC_DENMARK1,PRICE_YES_NO */
/* asm: 	.word	MSGC_HUNGARY1,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t OTHER_MENU[] = {
    (uintptr_t)&MSGC_SWEDEN1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_FINLAND1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_NETHERLAND1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_NETHERLAND2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_NORWAY1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_DENMARK1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_HUNGARY1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
#define GENERAL_MENU_LEN 9
/* asm: GENERAL_MENU */
/* asm: 	.word	MSGC_GENERAL1,PRICE_YES_NO */
/* asm: 	.word	MSGC_GENERAL2,PRICE_YES_NO */
/* asm: 	.word	MSGC_GENERAL3,PRICE_YES_NO */
/* asm: 	.word	MSGC_GENERAL4,PRICE_YES_NO */
/* asm: 	.word	MSGC_GENERAL5,PRICE_YES_NO */
/* asm: 	.word	MSGC_GENERAL6,PRICE_YES_NO */
/* asm: 	.word	MSGC_GENERAL7,PRICE_YES_NO */
/* asm: 	.word	MSGC_GENERAL8,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
/* asm: 	 */
static uintptr_t GENERAL_MENU[] = {
    (uintptr_t)&MSGC_GENERAL1,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GENERAL2,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GENERAL3,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GENERAL4,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GENERAL5,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GENERAL6,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GENERAL7,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&MSGC_GENERAL8,
    (uintptr_t)PRICE_YES_NO,
    (uintptr_t)&ETMAIN,
    (uintptr_t)XIT_MENU_TO_MAIN,
};
static const char ETMAIN[] = "EXIT TO MAIN";
/* asm: COUNTRY_MENUTAB */
/* asm: 	.word	USA_MENU,USA_MENU_LEN */
/* asm: 	.word	GERMAN_MENU,GERMAN_MENU_LEN */
/* asm: 	.word	FRENCH_MENU,FRENCH_MENU_LEN */
/* asm: 	.word	CANADA_MENU,CANADA_MENU_LEN */
/* asm: 	.word	SWISS_MENU,SWISS_MENU_LEN */
/* asm: 	.word	ITALY_MENU,ITALY_MENU_LEN */
/* asm: 	.word	UK_MENU,UK_MENU_LEN */
/* asm: 	.word	SPAIN_MENU,SPAIN_MENU_LEN */
/* asm: 	.word	AUSTRALIA_MENU,AUSTRALIA_MENU_LEN */
/* asm: 	.word	JAPAN_MENU,JAPAN_MENU_LEN */
/* asm: 	.word	TAIWAN_MENU,TAIWAN_MENU_LEN */
/* asm: 	.word	AUSTRIA_MENU,AUSTRIA_MENU_LEN */
/* asm: 	.word	BELGIUM_MENU,BELGIUM_MENU_LEN */
/* asm: 	.word	OTHER_MENU,OTHER_MENU_LEN */
/* asm: 	.word	GENERAL_MENU,GENERAL_MENU_LEN */
static uintptr_t COUNTRY_MENUTAB[] = {
    (uintptr_t)&USA_MENU,
    (uintptr_t)(USA_MENU_LEN),
    (uintptr_t)&GERMAN_MENU,
    (uintptr_t)(GERMAN_MENU_LEN),
    (uintptr_t)&FRENCH_MENU,
    (uintptr_t)(FRENCH_MENU_LEN),
    (uintptr_t)&CANADA_MENU,
    (uintptr_t)(CANADA_MENU_LEN),
    (uintptr_t)&SWISS_MENU,
    (uintptr_t)(SWISS_MENU_LEN),
    (uintptr_t)&ITALY_MENU,
    (uintptr_t)(ITALY_MENU_LEN),
    (uintptr_t)&UK_MENU,
    (uintptr_t)(UK_MENU_LEN),
    (uintptr_t)&SPAIN_MENU,
    (uintptr_t)(SPAIN_MENU_LEN),
    (uintptr_t)&AUSTRALIA_MENU,
    (uintptr_t)(AUSTRALIA_MENU_LEN),
    (uintptr_t)&JAPAN_MENU,
    (uintptr_t)(JAPAN_MENU_LEN),
    (uintptr_t)&TAIWAN_MENU,
    (uintptr_t)(TAIWAN_MENU_LEN),
    (uintptr_t)&AUSTRIA_MENU,
    (uintptr_t)(AUSTRIA_MENU_LEN),
    (uintptr_t)&BELGIUM_MENU,
    (uintptr_t)(BELGIUM_MENU_LEN),
    (uintptr_t)&OTHER_MENU,
    (uintptr_t)(OTHER_MENU_LEN),
    (uintptr_t)&GENERAL_MENU,
    (uintptr_t)(GENERAL_MENU_LEN),
};
/* asm: COUNTRY	.bss	COUNTRY,1 */
int COUNTRY;

static void RUN_COUNTRY(void) {
    // asm 00000BDC: 	LDI	-1,R0
    // asm 00000BDD: 	STI	R0,@PSLAST		;JUMP START PRINT STANDARD
    // asm 00000BDE: 	LDI	R6,AR5
    // asm 00000BDF: 	ADDI	@COUNTRY_TABI,AR5
    // asm 00000BE0: 	LDI	*AR5,R0
    // asm 00000BE1: 	STI	R0,@COUNTRY
    // asm 00000BE2: 	LDI	R6,R0
    // asm 00000BE3: 	MPYI	2,R0
    // asm 00000BE4: 	LDI	@COUNTRY_MENUTABI,AR5
    // asm 00000BE5: 	ADDI	R0,AR5
    // asm 00000BE6: 	LDI	*+AR5,AR6			;MENU LENGTH
    // asm 00000BE7: 	LDI	*AR5,AR5			;MENU DATA
    // asm 00000BE8: 	LDI	@SPM,AR7
    // asm 00000BE9: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_COUNTRY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static void MAKE_STANDARW(void) {
    // asm 00000BEA: 	IWINDOW	20,45,250,250
    // asm 00000BEF: 	IWINDOW	262,45,500,250
    // asm 00000BF4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_STANDARW", 0, 0);
    UNIMPL();
}

#define PSSTARTY 100
/* asm: PSLAST	.bss	PSLAST,1 */
int PSLAST;

static void PRINT_STANDARD(void) {
    // asm 00000BF5: 	CMPI	R0,R6
    // asm 00000BF6: 	RETSNE
    // asm 00000BF7: 	CMPI	@PSLAST,R0
    // asm 00000BF8: 	RETSEQ
    // asm 00000BF9: 	STI	R0,@PSLAST
    // asm 00000BFA: 	CALL	PUSHALL
    // asm 00000BFB: 	PUSH	RC
    // asm 00000BFC: 	FILL	270,PSSTARTY+120,498,PSSTARTY+129,0
    // asm 00000C02: 	FILL	490-24,PSSTARTY,498,PSSTARTY+149,0
    // asm 00000C08: 	FILL	270,PSSTARTY-40,498,PSSTARTY-10,0
    // asm 00000C0E: 	CMPI	0,AR7
    // asm 00000C0F: 	BLE	PS_SKIPMENU
    // asm 00000C10: 	LDI	COL_RED,RC
    // asm 00000C11: 	LDI	R0,AR4
    // asm 00000C12: 	ADDI	@COUNTRY,AR4
    // asm 00000C13: 	MPYI	COIN_ENTRY_SIZE,AR4
    // asm 00000C14: 	ADDI	@COIN_TABLEI,AR4
    // asm 00000C15: 	ADDI	3,AR4
    // asm 00000C16: 	LDI	*AR4++,AR2
    // asm 00000C17: 	LDI	270,R2
    // asm 00000C18: 	LDI	PSSTARTY-40,R3
    // asm 00000C19: 	CALL	_outtextxyc
    // asm 00000C1A: 	LDI	*AR4++,AR2
    // asm 00000C1B: 	CMPI	0,AR2
    // asm 00000C1C: 	BEQ	DONE_MSG
    // asm 00000C1D: 	LDI	270,R2
    // asm 00000C1E: 	LDI	PSSTARTY-30,R3
    // asm 00000C1F: 	CALL	_outtextxyc
    // asm 00000C20: 	LDI	*AR4++,AR2
    // asm 00000C21: 	CMPI	0,AR2
    // asm 00000C22: 	BEQ	DONE_MSG
    // asm 00000C23: 	LDI	270,R2
    // asm 00000C24: 	LDI	PSSTARTY-20,R3
    // asm 00000C25: 	CALL	_outtextxyc
DONE_MSG:
    // asm 00000C26: 	LDI	DEFAULT_COLOR,RC
    // asm 00000C27: 	TEXTITT	"DETAILED PRICING INFORMATION",270,PSSTARTY-50
    // asm 00000C2B: 	TEXTITT	"LEFT CHUTE UNITS",270,PSSTARTY+0
    // asm 00000C2F: 	TEXTITT	"RIGHT CHUTE UNITS",270,PSSTARTY+10
    // asm 00000C33: 	TEXTITT	"CENTER CHUTE UNITS",270,PSSTARTY+20
    // asm 00000C37: 	TEXTITT	"FOURTH CHUTE UNITS",270,PSSTARTY+30
    // asm 00000C3B: 	TEXTITT	"UNITS PER CREDIT",270,PSSTARTY+40
    // asm 00000C3F: 	TEXTITT	"UNITS FOR BONUS",270,PSSTARTY+50
    // asm 00000C43: 	TEXTITT	"MINIMUM UNITS",270,PSSTARTY+60
    // asm 00000C47: 	TEXTITT	"CREDITS TO START",270,PSSTARTY+70
    // asm 00000C4B: 	TEXTITT	"CREDITS TO CONTINUE",270,PSSTARTY+80
    // asm 00000C4F: 	TEXTITT	"LEFT  CENTER  RIGHT  FOURTH",270,PSSTARTY+100
    // asm 00000C53: 	TEXTITT	"PRICING SELECTED",270,PSSTARTY+140
PS_SKIPMENU:
    // asm 00000C57: 	POP	RC
    // asm 00000C58: 	CMPI	0,AR7
    // asm 00000C59: 	BLE	PSLPX
    // asm 00000C5A: 	LDI	R0,AR4
    // asm 00000C5B: 	ADDI	@COUNTRY,AR4
    // asm 00000C5C: 	MPYI	COIN_ENTRY_SIZE,AR4
    // asm 00000C5D: 	ADDI	@COIN_TABLEI,AR4
    // asm 00000C5E: 	LDI	*AR4++,R4
    // asm 00000C5F: 	LDI	490,R2	;X POS
    // asm 00000C60: 	LDI	PSSTARTY,R3	;Y POS START OF TEXT
    // ;	PUSH	AR2
    // ;First print the SLOT units
PSLP:
    // asm 00000C61: 	LDI	R4,R0
    // asm 00000C62: 	AND	0FFh,R0
    // asm 00000C63: 	PUSH	R2
    // asm 00000C64: 	PUSH	RC
    // asm 00000C65: 	LDI	R0,R2
    // asm 00000C66: 	LDI	@bufferI,AR2
    // asm 00000C67: 	CALL	_itoa
    // asm 00000C68: 	POP	RC
    // asm 00000C69: 	POP	R2
    // asm 00000C6A: 	PUSH	R2		;PRESERVE R2 for LATER USE
    // asm 00000C6B: 	LDI	0,R1
    // asm 00000C6C: 	CMPI	10,R0
    // asm 00000C6D: 	LDIGE	-8,R1		;Right justify
    // asm 00000C6E: 	CMPI	100,R0
    // asm 00000C6F: 	LDIGE	-16,R1		;Right justify
    // asm 00000C70: 	ADDI	R1,R2
    // asm 00000C71: 	CALL	_outtextxyc
    // asm 00000C72: 	POP	R2
    // asm 00000C73: 	LSH	-8,R4
    // asm 00000C74: 	ADDI	10,R3
    // asm 00000C75: 	CMPI	PSSTARTY+40,R3
    // asm 00000C76: 	BLT	PSLP
    // asm 00000C77: 	CMPI	PSSTARTY+80,R3
    // asm 00000C78: 	BEQ	PSLP1
    // asm 00000C79: 	CMPI	PSSTARTY+40,R3
    // asm 00000C7A: 	BGT	PSLP1_CHECK
PSLP1:
    // ;	POP	AR2
    // asm 00000C7B: 	LDI	*AR4++,R4
    // ;	PUSH	AR2
PSLP1_CHECK:
    // asm 00000C7C: 	CMPI	PSSTARTY+90,R3
    // asm 00000C7D: 	BLT	PSLP
    // ;	POP	AR4
    // asm 00000C7E: 	ADDI	3,AR4
    // ;Print LEFT slot denomination
    // asm 00000C7F: 	LDI	*AR4++,AR2
    // asm 00000C80: 	LDI	PSSTARTY+120,R3
    // asm 00000C81: 	LDI	270,R2
    // asm 00000C82: 	CALL	_outtextxyc
    // ;Print RIGHT slot denomination
    // asm 00000C83: 	LDI	*AR4++,AR2
    // asm 00000C84: 	LDI	270+112,R2
    // asm 00000C85: 	CALL	_outtextxyc
    // ;Print CENTER slot denomination
    // asm 00000C86: 	LDI	*AR4++,AR2
    // asm 00000C87: 	LDI	270+56,R2
    // asm 00000C88: 	CALL	_outtextxyc
    // ;Print FOURTH slot denomination
    // asm 00000C89: 	LDI	*AR4++,AR2
    // asm 00000C8A: 	LDI	270+176,R2
    // asm 00000C8B: 	CALL	_outtextxyc
    // asm 00000C8C: 	LDI	R6,R1
    // asm 00000C8D: 	ADDI	@COUNTRY,R1
    // asm 00000C8E: 	READADJ	ADJ_COINMODE
    // asm 00000C90: 	CMPI	R0,R1
    // asm 00000C91: 	LDIEQ	1,R0
    // asm 00000C92: 	LDINE	0,R0
    // asm 00000C93: 	LDI	PSSTARTY+140,R3
    // asm 00000C94: 	CALL	PRINT_SPYN
PSLPX:
    // asm 00000C95: 	CALL	POPALL
    // asm 00000C96: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_STANDARD", 0, 0);
    UNIMPL();
}

static void PRINT_SPYN(void) {
    // asm 00000C97: 	LDI	@YES,AR2
    // asm 00000C98: 	LDI	474,R2
    // asm 00000C99: 	CMPI	0,R0
    // asm 00000C9A: 	LDIEQ	@NO,AR2
    // asm 00000C9B: 	LDIEQ	482,R2
    // asm 00000C9C: 	CALL	_outtextxyc
    // asm 00000C9D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_SPYN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
#define CUSTOM_PRICING_MENU_LEN 16
/* asm: CUSTOM_PRICING_MENU */
/* asm: MENUENTRY	"LEFT CHUTE UNITS",SET_SLOT */
/* asm: MENUENTRY	"RIGHT CHUTE UNITS",SET_SLOT */
/* asm: MENUENTRY	"CENTER CHUTE UNITS",SET_SLOT */
/* asm: MENUENTRY	"FOURTH CHUTE UNITS",SET_SLOT */
/* asm: MENUENTRY	"UNITS PER CREDIT",SET_SLOT */
/* asm: MENUENTRY	"UNITS FOR BONUS",SET_SLOT */
/* asm: MENUENTRY	"MINIMUM UNITS",SET_SLOT */
/* asm: MENUENTRY	"CREDITS TO START",SET_SLOT */
/* asm: MENUENTRY	"CREDITS TO CONTINUE",SET_SLOT */
/* asm: MENUENTRY	"LEFT CHUTE COUNTER",SET_SLOT */
/* asm: MENUENTRY	"RIGHT CHUTE COUNTER",SET_SLOT */
/* asm: MENUENTRY	"CENTER CHUTE COUNTER",SET_SLOT */
/* asm: MENUENTRY	"FOURTH CHUTE COUNTER",SET_SLOT */
/* asm: MENUENTRY	"SHOW FRACTION",FRACTION_YES_NO */
/* asm: MENUENTRY	"SELECT CUSTOM PRICING",CUSTOM_YES_NO */
/* asm: MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN */
/* asm: romdata */
static int CUSTOM_PRICING_MENU;
static const char sct[] = "CALIBRATE CONTROLS";

void SET_CONTROLS(void) {
    // asm 00000CC0: 	CALL	SETUP_DIAG_SCREEN
    // asm 00000CC1: 	LDI	DEFAULT_COLOR,RC
    // asm 00000CC2: 	TEXTIT	sct,150,10
    // asm 00000CC6: 	TEXTITT	"TAKE HANDS AND FEET OFF CONTROLS, AND SET STEERING",50,40
    // asm 00000CCA: 	TEXTITT	"WHEEL TO CENTER THEN HIT ENTER",50,50
    // asm 00000CCE: 	CALL	WT_NSTART
    // asm 00000CCF: 	CALL	WT_START
    // asm 00000CD0: 	CALL	BEEP
    // 	;get gas pedal min
    // asm 00000CD1: 	LDI	@_pot1,R2
    // asm 00000CD2: 	SETADJ	ADJ_GASMIN
    // asm 00000CD4: 	LDI	@_pot2,R2
    // asm 00000CD5: 	SETADJ	ADJ_BRAKEMIN
    // 	;get wheel center
    // asm 00000CD7: 	LDI	@_pot0,R2
    // asm 00000CD8: 	SETADJ	ADJ_STEERCENTER
    // asm 00000CDA: 	CALL	CLRSCRN
    // 	;turn wheel to left and hit sw1
    // asm 00000CDB: 	LDI	DEFAULT_COLOR,RC
    // asm 00000CDC: 	TEXTIT	sct,150,10
    // asm 00000CE0: 	TEXTITT	"TURN WHEEL ALL THE WAY TO THE LEFT AND HIT ENTER",50,40
    // asm 00000CE4: 	CALL	WT_NSTART
    // asm 00000CE5: 	CALL	WT_START
    // asm 00000CE6: 	CALL	BEEP
    // asm 00000CE7: 	LDI	@_pot0,R2
    // asm 00000CE8: 	SETADJ	ADJ_STEERMIN
    // asm 00000CEA: 	CALL	CLRSCRN
    // asm 00000CEB: 	LDI	DEFAULT_COLOR,RC
    // asm 00000CEC: 	TEXTIT	sct,150,10
    // asm 00000CF0: 	TEXTITT	"TURN WHEEL ALL THE WAY TO THE RIGHT AND HIT ENTER",50,40		;turn wheel to right
    // asm 00000CF4: 	CALL	WT_NSTART
    // asm 00000CF5: 	CALL	WT_START
    // asm 00000CF6: 	CALL	BEEP
    // asm 00000CF7: 	LDI	@_pot0,R2
    // asm 00000CF8: 	SETADJ	ADJ_STEERMAX
    // asm 00000CFA: 	CALL	CLRSCRN
    // asm 00000CFB: 	LDI	DEFAULT_COLOR,RC
    // asm 00000CFC: 	TEXTIT	sct,150,10
    // 	;"push gas pedal all the way and hit sw1"
    // asm 00000D00: 	TEXTITT	"PUSH GAS PEDAL TO MAX AND HIT ENTER",50,40
    // asm 00000D04: 	CALL	WT_NSTART
    // asm 00000D05: 	CALL	WT_START
    // asm 00000D06: 	CALL	BEEP
    // asm 00000D07: 	LDI	@_pot1,R2
    // asm 00000D08: 	SETADJ	ADJ_GASMAX
    // asm 00000D0A: 	LDI	@DIPRAM,R0
    // asm 00000D0B: 	TSTB	DIP_STANDUP,R0
    // asm 00000D0C: 	BNZ	DADA5
    // asm 00000D0D: 	CALL	CLRSCRN
    // asm 00000D0E: 	LDI	DEFAULT_COLOR,RC
    // asm 00000D0F: 	TEXTIT	sct,150,10
    // asm 00000D13: 	TEXTITT	"PUSH BRAKE PEDAL TO MAX AND HIT ENTER",50,40
    // asm 00000D17: 	CALL	WT_NSTART
    // asm 00000D18: 	CALL	WT_START
    // asm 00000D19: 	CALL	BEEP
    // asm 00000D1A: 	LDI	@_pot2,R2
    // asm 00000D1B: 	SETADJ	ADJ_BRAKEMAX
    // asm 00000D1D: 	CALL	WT_NSTART
DADA5:
    // asm 00000D1E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_CONTROLS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SET_VOLUME	SYSTEM ADJUSTMENT
 *
 *
 *START	RETURN
 *VIEW1	VOLUME UP
 *VIEW2	VOLUME DOWN
 *
 *
 */
static void SET_VOLUME(void) {
    // asm 00000D1F: 	CALL	SETUP_DIAG_SCREEN
    // asm 00000D20: 	GROUP	90,60,410,240
    // asm 00000D50: 	IWINDOW	120,72,240,90
    // asm 00000D55: 	LDI	COL_VDGREY,RC
    // asm 00000D56: 	TEXTITT	"ADJUST VOLUME",125,76
    // asm 00000D5A: 	IWINDOW	119,149,379,181
    // asm 00000D5F: 	FILL	120,150,378,180,COL_BLACK
    // asm 00000D65: 	LDI	COL_VDGREY,RC
    // asm 00000D66: 	TEXTITT	"MIN",118,135
    // asm 00000D6A: 	TEXTITT	"MAX",360,135
    // asm 00000D6E: 	TEXTITT	"ENTER  EXIT",130,200
    // asm 00000D72: 	TEXTITT	"UP     INCREASE VOLUME",130,210
    // asm 00000D76: 	TEXTITT	"DOWN   DECREASE VOLUME",130,220
    // asm 00000D7A: 	READADJ	ADJ_VOLUME
    // asm 00000D7C: 	LDI	R0,R4
    // asm 00000D7D: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm 00000D7F: 	CMPI	R0,R4
    // asm 00000D80: 	LDILT	R0,R4
    // asm 00000D81: 	CALL	SHOW_VOLUME
    // asm 00000D82: 	SOND1	MUNSTER_SURF
    // asm 00000D84: 	SOND1	ENGINESTART
SETVOL_LP:
    // asm 00000D86: 	LDI	700,AR2
    // asm 00000D87: 	CALL	RANDU0
    // asm 00000D88: 	LDI	R0,R0
    // asm 00000D89: 	BNZ	SETNN
    // asm 00000D8A: 	LDI	6,AR2
    // asm 00000D8B: 	CALL	RANDU0
    // asm 00000D8C: 	LDPI	@INCIDENTI,AR2
    // asm 00000D8D: 	ADDI	R0,AR2
    // asm 00000D8E: 	LDI	*AR2,AR2
    // asm 00000D8F: 	CALL	ONESND
INCIDENTI:
    // asm 00000D90: .word	INCIDENT
INCIDENT:
    // asm 00000D90: .word	CARPASS,TRUCKPASS
SETNN:
    // asm 00000D90: 	LDI	@_newbut,R0
    // asm 00000D91: 	LDI	R0,R1
    // asm 00000D92: 	TSTB	SW_DIAG,R1		;move up
    // asm 00000D93: 	BNZ	NOTRET
    // asm 00000D94: 	SOND1	KILLALLS
    // asm 00000D96: 	RETS
NOTRET:
    // asm 00000D97: 	TSTB	SW_VOLPLUS,R0		;move up
    // asm 00000D98: 	BNZ	NOTUPA
    // asm 00000D99: 	ADDI	1,R4
    // asm 00000D9A: 	CMPI	255,R4
    // asm 00000D9B: 	LDIGT	255,R4
    // asm 00000D9C: 	PUSH	R0
    // asm 00000D9D: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm 00000D9F: 	CMPI	R0,R4
    // asm 00000DA0: 	LDILT	R0,R4
    // asm 00000DA1: 	POP	R0
    // asm 00000DA2: 	LDI	R4,R1
    // asm 00000DA3: 	CALL	SET_MASTER_VOL
    // asm 00000DA4: 	LDI	R4,R2
    // asm 00000DA5: 	SETADJ	ADJ_VOLUME
    // asm 00000DA7: 	CALL	SHOW_VOLUME
    // asm 00000DA8: 	B	WTA
NOTUPA:
    // asm 00000DA9: 	TSTB	SW_VOLMINUS,R0		;move down
    // asm 00000DAA: 	BNZ	NOTDOWNA
    // asm 00000DAB: 	SUBI	1,R4
    // asm 00000DAC: 	LDILT	1,R4
    // asm 00000DAD: 	PUSH	R0
    // asm 00000DAE: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm 00000DB0: 	CMPI	R0,R4
    // asm 00000DB1: 	LDILT	R0,R4
    // asm 00000DB2: 	POP	R0
    // asm 00000DB3: 	LDI	R4,R1
    // asm 00000DB4: 	CALL	SET_MASTER_VOL
    // asm 00000DB5: 	LDI	R4,R2
    // asm 00000DB6: 	SETADJ	ADJ_VOLUME
    // asm 00000DB8: 	CALL	SHOW_VOLUME
WTA:
NOTDOWNA:
    // asm 00000DB9: 	B	SETVOL_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_VOLUME", 0, 0);
    UNIMPL();
}

static void SHOW_VOLUME(void) {
    // asm 00000DBA: 	LDI	120,AR2
    // asm 00000DBB: 	LDI	AR2,R3
    // asm 00000DBC: 	ADDI	R4,R3
    // asm 00000DBD: 	LDI	150,R2
    // asm 00000DBE: 	LDI	180,RC
    // asm 00000DBF: 	LDI	COL_GREEN,RS
    // asm 00000DC0: 	CALL	_fill
    // asm 00000DC1: 	LDI	120,AR2
    // asm 00000DC2: 	ADDI	R4,AR2
    // asm 00000DC3: 	LDI	355,R3
    // asm 00000DC4: 	LDI	150,R2
    // asm 00000DC5: 	LDI	180,RC
    // asm 00000DC6: 	LDI	COL_BLACK,RS
    // asm 00000DC7: 	CALL	_fill
    // asm 00000DC8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_VOLUME", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULL_FACTORY_RESTORE(void) {
    // asm 00000DC9: 	LDL	FFR,AR2
    // asm 00000DCA: 	LDI	@NULLSTR,AR3
    // asm 00000DCB: 	CALL	CLEAR_YES_NO
    // asm 00000DCC: 	RETSC
    // asm 00000DCD: 	SETDP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULL_FACTORY_RESTORE", 0, 0);
    UNIMPL();
}

void FFRSUB(void) {
    // asm 00000DCE: 	CALL	RESET_BOOKKEEPING
    // asm 00000DCF: 	CALL	INIT_HSTD_TABLES
    // asm 00000DD0: 	CALL	RESET_ADJUSTMENTS
    // asm 00000DD1: 	CLRI	R2
    // asm 00000DD2: 	SETAUD	AUD_PCREDITS
    // asm 00000DD4: 	SETAUD	AUD_CREDITS
    // asm 00000DD6: 	LDI	@FFRST,AR2
    // asm 00000DD7: 	CALL	MESSAGE_BOX
    // asm 00000DD8: 	CALL	SET_CONTROLS
    // asm 00000DD9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FFRSUB", 0, 0);
    UNIMPL();
}

/* asm: FFRST	SPTR	"FULL FACTORY RESTORE" */
static const char FFRST[] = "FULL FACTORY RESTORE";
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DEFAULT_ADJUSTMENTS(void) {
    // asm 00000DDB: 	LDL	RDA,AR2
    // asm 00000DDC: 	LDI	@NULLSTR,AR3
    // asm 00000DDD: 	CALL	CLEAR_YES_NO
    // asm 00000DDE: 	RETSC
    // asm 00000DDF: 	SETDP
    // asm 00000DE0: 	CALL	RESET_ADJUSTMENTS
    // asm 00000DE1: 	LDI	@DAS,AR2
    // asm 00000DE2: 	CALL	MESSAGE_BOX
    // asm 00000DE3: 	CALL	SET_CONTROLS
    // asm 00000DE4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEFAULT_ADJUSTMENTS", 0, 0);
    UNIMPL();
}

/* asm: DAS	SPTR	"DEFAULT ADJUSTMENTS SET" */
static const char DAS[] = "DEFAULT ADJUSTMENTS SET";
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void RESET_HIGH_SCORES(void) {
    // asm 00000DE6: 	LDL	RHS,AR2
    // asm 00000DE7: 	LDI	@NULLSTR,AR3
    // asm 00000DE8: 	CALL	CLEAR_YES_NO
    // asm 00000DE9: 	RETSC
    // asm 00000DEA: 	SETDP
    // asm 00000DEB: 	CALL	INIT_HSTD_TABLES
    // asm 00000DEC: 	LDI	@HSRS,AR2
    // asm 00000DED: 	CALL	MESSAGE_BOX
    // asm 00000DEE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_HIGH_SCORES", 0, 0);
    UNIMPL();
}

/* asm: HSRS	SPTR	"HIGH SCORES RESET" */
static const char HSRS[] = "HIGH SCORES RESET";
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CLEAR_GAME_AUDITS(void) {
    // asm 00000DF0: 	LDL	CGA,AR2
    // asm 00000DF1: 	LDI	@NULLSTR,AR3
    // asm 00000DF2: 	CALL	CLEAR_YES_NO
    // asm 00000DF3: 	RETSC
    // asm 00000DF4: 	SETDP
    // asm 00000DF5: 	CALL	RESET_BOOKKEEPING
    // asm 00000DF6: 	LDI	@BOOKR,AR2
    // asm 00000DF7: 	CALL	MESSAGE_BOX
    // asm 00000DF8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_GAME_AUDITS", 0, 0);
    UNIMPL();
}

/* asm: BOOKR	SPTR	"BOOKKEEPING RESET" */
static const char BOOKR[] = "BOOKKEEPING RESET";
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: FREEPLAYT	SPTR	"FREE PLAY" */
static const char FREEPLAYT[] = "FREE PLAY";

static void FREEPLAYONOFF(void) {
    // asm 00000DFB: 	READAUD	ADJ_FREE_PLAY
    // asm 00000DFD: 	LDI	R0,R5
    // asm 00000DFE: 	LDI	@FREEPLAYT,AR2
    // asm 00000DFF: 	LDI	@NULLSTR,AR3
    // asm 00000E00: 	CALL	DUAL_ON_OFF
    // asm 00000E01: 	LDI	R5,R2
    // asm 00000E02: 	SETADJ	ADJ_FREE_PLAY
    // asm 00000E04: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREEPLAYONOFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: FGOO1	SPTR	"GIVE FREE GAME" */
static const char FGOO1[] = "GIVE FREE GAME";
/* asm: FGOO2	SPTR	"ON 1ST POSITION" */
static const char FGOO2[] = "ON 1ST POSITION";

static void FREEGAMEONOFF(void) {
    // asm 00000E07: 	READAUD	ADJ_FREEGAME
    // asm 00000E09: 	LDI	R0,R5
    // asm 00000E0A: 	LDI	@FGOO1,AR2
    // asm 00000E0B: 	LDI	@FGOO2,AR3
    // asm 00000E0C: 	CALL	DUAL_ON_OFF
    // asm 00000E0D: 	LDI	R5,R2
    // asm 00000E0E: 	SETADJ	ADJ_FREEGAME
    // asm 00000E10: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREEGAMEONOFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: APHSTD	SPTR	"ALLOW PLAYER TO" */
static const char APHSTD[] = "ALLOW PLAYER TO";
/* asm: APHSTD2	SPTR	"ENTER INITIALS" */
static const char APHSTD2[] = "ENTER INITIALS";

static void HIGHSCORE_ENTRY(void) {
    // asm 00000E13: 	READAUD	ADJ_HIGH_SCORE_ENTRY
    // asm 00000E15: 	LDI	R0,R5
    // asm 00000E16: 	LDI	@APHSTD,AR2
    // asm 00000E17: 	LDI	@APHSTD2,AR3
    // asm 00000E18: 	CALL	DUAL_ON_OFF
    // asm 00000E19: 	LDI	R5,R2
    // asm 00000E1A: 	SETADJ	ADJ_HIGH_SCORE_ENTRY
    // asm 00000E1C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIGHSCORE_ENTRY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: GI1	SPTR	"SHOW GIRLS" */
static const char GI1[] = "SHOW GIRLS";

static void GIRLS_ONOFF(void) {
    // asm 00000E1E: 	READAUD	ADJ_GIRLS
    // asm 00000E20: 	LDI	R0,R5
    // asm 00000E21: 	LDI	@GI1,AR2
    // asm 00000E22: 	LDI	@NULLSTR,AR3
    // asm 00000E23: 	CALL	DUAL_ON_OFF
    // asm 00000E24: 	LDI	R5,R2
    // asm 00000E25: 	SETADJ	ADJ_GIRLS
    // asm 00000E27: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GIRLS_ONOFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CL1	SPTR	"SHOW PRESIDENT" */
static const char CL1[] = "SHOW PRESIDENT";

static void CLINTON_ONOFF(void) {
    // asm 00000E29: 	READAUD	ADJ_CLINTON
    // asm 00000E2B: 	LDI	R0,R5
    // asm 00000E2C: 	LDI	@CL1,AR2
    // asm 00000E2D: 	LDI	@NULLSTR,AR3
    // asm 00000E2E: 	CALL	DUAL_ON_OFF
    // asm 00000E2F: 	LDI	R5,R2
    // asm 00000E30: 	SETADJ	ADJ_CLINTON
    // asm 00000E32: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLINTON_ONOFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: RK1	SPTR	"SHOW ROADKILL" */
static const char RK1[] = "SHOW ROADKILL";

static void ROADKILL_ONOFF(void) {
    // asm 00000E34: 	READAUD	ADJ_ROADKILL
    // asm 00000E36: 	LDI	R0,R5
    // asm 00000E37: 	LDI	@RK1,AR2
    // asm 00000E38: 	LDI	@NULLSTR,AR3
    // asm 00000E39: 	CALL	DUAL_ON_OFF
    // asm 00000E3A: 	LDI	R5,R2
    // asm 00000E3B: 	SETADJ	ADJ_ROADKILL
    // asm 00000E3D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADKILL_ONOFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *	0 = MPH (default)
 *	1 = KPH
 *
 */
/* asm: MP1	SPTR	"SPEED IN MPH OR KPH" */
static const char MP1[] = "SPEED IN MPH OR KPH";
/* asm: MP2	SPTR	"KPH" */
static const char MP2[] = "KPH";
/* asm: MP3	SPTR	"MPH" */
static const char MP3[] = "MPH";

static void MILES_OR_KILO(void) {
    // asm 00000E41: 	READAUD	ADJ_MPHORKPM
    // asm 00000E43: 	LDI	R0,R5
    // asm 00000E44: 	LDI	@MP1,AR2
    // asm 00000E45: 	LDI	@NULLSTR,AR3
    // asm 00000E46: 	LDI	@MP2,AR4
    // asm 00000E47: 	LDI	@MP3,AR5
    // asm 00000E48: 	CALL	DUAL_VALUE
    // asm 00000E49: 	LDI	R5,R2
    // asm 00000E4A: 	SETADJ	ADJ_MPHORKPM
    // asm 00000E4C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MILES_OR_KILO", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CLR_CRED	SPTR	"CLEAR CREDITS" */
static const char CLR_CRED[] = "CLEAR CREDITS";
/* asm: YES		SPTR	"YES" */
static const char YES[] = "YES";
/* asm: NO		SPTR	"NO" */
static const char NO[] = "NO";
/* asm: ONS		SPTR	"ON" */
static const char ONS[] = "ON";
/* asm: OFFS		SPTR	"OFF" */
static const char OFFS[] = "OFF";
/* asm: NULLSTR		SPTR	"" */
const char NULLSTR[] = "";
/* asm: AREUSURE	SPTR	"ARE YOU SURE?" */
static const char AREUSURE[] = "ARE YOU SURE?";

// *----------------------------------------------------------------------------
static void CLEAR_CREDITS(void) {
    // asm 00000E54: 	LDI	@CLR_CRED,AR2
    // asm 00000E55: 	LDI	@AREUSURE,AR3
    // asm 00000E56: 	CALL	CLEAR_YES_NO
    // asm 00000E57: 	RETSC
    // asm 00000E58: 	SETDP
    // asm 00000E59: 	CLRI	R2
    // asm 00000E5A: 	SETAUD	AUD_PCREDITS
    // asm 00000E5C: 	SETAUD	AUD_CREDITS
    // asm 00000E5E: 	LDI	@CCD2A,AR2
    // asm 00000E5F: 	CALL	MESSAGE_BOX
    // asm 00000E60: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_CREDITS", 0, 0);
    UNIMPL();
}

/* asm: CCD2A	SPTR	"CREDITS CLEARED" */
static const char CCD2A[] = "CREDITS CLEARED";
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MESSAGE_BOX(void) {
    // asm 00000E62: 	PUSH	AR2
    // asm 00000E63: 	WINDOW	110,115,370,285
    // asm 00000E68: 	IWINDOW	120,125,360,275
    // asm 00000E6D: 	LDI	COL_VDGREY,RC
    // asm 00000E6E: 	POP	AR2
    // asm 00000E6F: 	LDI	130,R2
    // asm 00000E70: 	LDI	170,R3
    // asm 00000E71: 	CALL	_outtextxyc
    // asm 00000E72: 	LDI	30,AR6
ABLP:
    // asm 00000E73: CALL	WAIT_FOR_VBLANK
    // asm 00000E74: 	DBU	AR6,ABLP
    // asm 00000E75: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MESSAGE_BOX", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DUAL VALUE
 *
 *
 *PARAMETERS
 *	AR2	TEXT STRING 1
 *	AR3	TEXT STRING 2
 *	AR4	RESPONSE 1
 *	AR5	RESPONSE 2
 *	R5	VALUE {0,1}
 *
 *RETURNS
 *	C  ON YES  (RESPONSE 1)
 *	NC ON NO   (RESPONSE 2)
 *
 */
static void CLEAR_YES_NO(void) {
    // asm 00000E76: 	LDI	0,R5
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_YES_NO", 0, 0);
    UNIMPL();
}

static void ASK_YES_NO(void) {
    // asm 00000E77: 	LDI	@YES,AR4
    // asm 00000E78: 	LDI	@NO,AR5
    // asm 00000E79: 	BU	DUAL_VALUE
    // *	R5	DEFAULT
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASK_YES_NO", 0, 0);
    UNIMPL();
}

static void DUAL_ON_OFF(void) {
    // asm 00000E7A: 	LDI	@ONS,AR4
    // asm 00000E7B: 	LDI	@OFFS,AR5
    // asm 00000E7C: 	BU	DUAL_VALUE
DUAL_VALUE:
    // asm 00000E7D: 	AND	1,R5				;KEEP IT WITHIN RANGE
    // asm 00000E7E: 	PUSH	AR4
    // asm 00000E7F: 	PUSH	AR5
    // asm 00000E80: 	PUSH	AR3
    // asm 00000E81: 	PUSH	AR2
    // asm 00000E82: 	WAIT_MAC_NOREPEAT
    // asm 00000E86: 	WINDOW	110,115,370,285
    // asm 00000E8B: 	IWINDOW	120,125,360,275
    // asm 00000E90: 	LDI	COL_VDGREY,RC
    // asm 00000E91: 	POP	AR2
    // asm 00000E92: 	LDI	130,R2
    // asm 00000E93: 	LDI	140,R3
    // asm 00000E94: 	CALL	_outtextxyc
    // asm 00000E95: 	POP	AR2
    // asm 00000E96: 	LDI	130,R2
    // asm 00000E97: 	LDI	150,R3
    // asm 00000E98: 	CALL	_outtextxyc
    // asm 00000E99: 	TEXTITT	"UP TO CHANGE",130,235
    // asm 00000E9D: 	TEXTITT	"ENTER TO ACCEPT",130,245
    // asm 00000EA1: 	CALL	WAIT_FOR_VBLANK
    // asm 00000EA2: 	CALL	WAIT_FOR_VBLANK
    // asm 00000EA3: 	POP	AR7
    // asm 00000EA4: 	POP	AR6
CYNLOOP:
    // asm 00000EA5: 	LDI	COL_VDGREY,RC
    // asm 00000EA6: 	CMPI	1,R5
    // asm 00000EA7: 	LDIEQ	COL_RED,RC
    // asm 00000EA8: 	LDI	AR6,AR2
    // asm 00000EA9: 	LDI	170,R2
    // asm 00000EAA: 	LDI	200,R3
    // asm 00000EAB: 	CALL	_outtextxyc
    // asm 00000EAC: 	LDI	COL_VDGREY,RC
    // asm 00000EAD: 	CMPI	0,R5
    // asm 00000EAE: 	LDIEQ	COL_RED,RC
    // asm 00000EAF: 	LDI	AR7,AR2
    // asm 00000EB0: 	LDI	220,R2
    // asm 00000EB1: 	LDI	200,R3
    // asm 00000EB2: 	CALL	_outtextxyc
    // asm 00000EB3: 	LDI	@_newbut,R0		;old system method
    // asm 00000EB4: 	TSTB    SW_DIAG,R0
    // asm 00000EB5: 	BZ	DOEXIT
    // asm 00000EB6: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm 00000EB7: 	BZ	CHNG
    // asm 00000EB8: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm 00000EB9: 	BZ	NCHNG
    // asm 00000EBA: 	BU	NCHNG
CHNG:
    // asm 00000EBB: 	XOR	1,R5
    // asm 00000EBC: 	SOND1	WPC_MISC2
NCHNG:
    // asm 00000EBE: 	WAIT_MAC_NOREPEAT
    // asm 00000EC2: 	B	CYNLOOP
DOEXIT:
    // asm 00000EC3: 	CMPI	0,R5
    // asm 00000EC4: 	BEQ	CYN_YES
    // asm 00000EC5: CYN_NO
    // asm 00000EC5: 	CLRC
    // asm 00000EC6: 	RETS
CYN_YES:
    // asm 00000EC7: 	SETC
    // asm 00000EC8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DUAL_ON_OFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: SSGD	SPTR	"STEERING SENSITIVITY" */
static const char SSGD[] = "STEERING SENSITIVITY";
/* asm: SSGD2	SPTR	"0 (ADOLT) TO 5 (GAMEPLAYER)" */
static const char SSGD2[] = "0 (ADOLT) TO 5 (GAMEPLAYER)";

static void SET_STEERING_SENSITIVITY(void) {
    // asm 00000ECB: 	READAUD	ADJ_STEERING_SENSITIVITY
    // asm 00000ECD: 	LDI	R0,R5
    // asm 00000ECE: 	LDI	@SSGD,AR2
    // asm 00000ECF: 	LDI	@SSGD2,AR3
    // asm 00000ED0: 	LDI	0,R6
    // asm 00000ED1: 	LDI	5,R7
    // asm 00000ED2: 	LDI	@GAMEDIFF_CVTI,AR4
    // asm 00000ED3: 	LDI	1,R4
    // asm 00000ED4: 	CALL	MULTI_VALUE_STEP
    // asm 00000ED5: 	LDI	R5,R2
    // asm 00000ED6: 	SETADJ	ADJ_STEERING_SENSITIVITY
    // asm 00000ED8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_STEERING_SENSITIVITY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: HSGD	SPTR	"PLAYS TO HIGHSCORE" */
static const char HSGD[] = "PLAYS TO HIGHSCORE";
/* asm: HSGD2	SPTR	"RESET 1000 TO 25000" */
static const char HSGD2[] = "RESET 1000 TO 25000";

static void SET_HIGHSCORE_RESET(void) {
    // asm 00000EDB: 	READAUD	ADJ_HIGHSCORE_RESET
    // asm 00000EDD: 	LDI	R0,R5
    // asm 00000EDE: 	LDI	@HSGD,AR2
    // asm 00000EDF: 	LDI	@HSGD2,AR3
    // asm 00000EE0: 	LDI	1000,R6
    // asm 00000EE1: 	LDI	25000,R7
    // asm 00000EE2: 	LDI	@GAMEDIFF_CVTI,AR4
    // asm 00000EE3: 	LDI	1000,R4
    // asm 00000EE4: 	CALL	MULTI_VALUE_STEP
    // asm 00000EE5: 	LDI	R5,R2
    // asm 00000EE6: 	SETADJ	ADJ_HIGHSCORE_RESET
    // asm 00000EE8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_HIGHSCORE_RESET", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
;*----------------------------------------------------------------------------
;FSGD	SPTR	"FIRST PLACE PERCENT"
;FSGD2	SPTR	"10 TO 40"
;SET_FIRST_PLACE_PERCENT:
;
;	READAUD	ADJ_FIRST_PLACE_PERCENT
;	LDI	R0,R5
;
;	LDI	@FSGD,AR2
;	LDI	@FSGD2,AR3
;	LDI	10,R6
;	LDI	40,R7
;
;	LDI	@GAMEDIFF_CVTI,AR4
;	CALL	MULTI_VALUE
;
;	LDI	R5,R2
;	SETADJ	ADJ_FIRST_PLACE_PERCENT
;	RETS
;*----------------------------------------------------------------------------
*/

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 */
/* asm: SGD	SPTR	"GAME DIFFICULTY" */
static const char SGD[] = "GAME DIFFICULTY";
/* asm: SGD2	SPTR	"0 (EASY) TO 9 (HARD)" */
static const char SGD2[] = "0 (EASY) TO 9 (HARD)";

static void SET_GAME_DIFFICULTY(void) {
    // asm 00000EEB: 	READAUD	ADJ_DIFFICULTY
    // asm 00000EED: 	LDI	R0,R5
    // asm 00000EEE: 	LDI	@SGD,AR2
    // asm 00000EEF: 	LDI	@SGD2,AR3
    // asm 00000EF0: 	CLRI	R6
    // asm 00000EF1: 	LDI	9,R7
    // asm 00000EF2: 	LDI	@GAMEDIFF_CVTI,AR4
    // asm 00000EF3: 	LDI	1,R4
    // asm 00000EF4: 	CALL	MULTI_VALUE_STEP
    // asm 00000EF5: 	LDI	R5,R2
    // asm 00000EF6: 	SETADJ	ADJ_DIFFICULTY
    // asm 00000EF8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_GAME_DIFFICULTY", 0, 0);
    UNIMPL();
}

/*
 *
 *
 */
static void GAMEDIFF_CVT(void) {
    // asm 00000EFA: 	PUSH	R2
    // asm 00000EFB: 	LDI	R5,R2
    // asm 00000EFC: 	LDI	@bufferI,AR2
    // asm 00000EFD: 	CALL	_itoa
    // asm 00000EFE: 	LDI	@bufferI,AR2
    // asm 00000EFF: 	POP	R2
    // asm 00000F00: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GAMEDIFF_CVT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 */
/* asm: ITD	SPTR	"INITIAL TIME" */
static const char ITD[] = "INITIAL TIME";
/* asm: ITD2	SPTR	"60-90 SECONDS" */
static const char ITD2[] = "60-90 SECONDS";

static void SET_TIME_TO_START(void) {
    // asm 00000F03: 	READAUD	ADJ_TIME_TO_START
    // asm 00000F05: 	LDI	R0,R5
    // asm 00000F06: 	LDI	@ITD,AR2
    // asm 00000F07: 	LDI	@ITD2,AR3
    // asm 00000F08: 	CLRI	R6
    // asm 00000F09: 	LDI	6,R7
    // asm 00000F0A: 	LDI	@TIME_CVTI,AR4
    // asm 00000F0B: 	CALL	MULTI_VALUE
    // asm 00000F0C: 	LDI	R5,R2
    // asm 00000F0D: 	SETADJ	ADJ_TIME_TO_START
    // asm 00000F0F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_TIME_TO_START", 0, 0);
    UNIMPL();
}

/*
 *
 *
 */
static void TIME_CVT(void) {
    // asm 00000F11: 	PUSH	R2
    // asm 00000F12: 	LDI	R5,R2
    // asm 00000F13: 	MPYI	5,R2
    // asm 00000F14: 	ADDI	60,R2
    // asm 00000F15: 	LDI	@bufferI,AR2
    // asm 00000F16: 	CALL	_itoa
    // asm 00000F17: 	LDI	@bufferI,AR2
    // asm 00000F18: 	POP	R2
    // asm 00000F19: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIME_CVT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CPBTT	SPTR	"CHECKPOINT TIME BONUS" */
static const char CPBTT[] = "CHECKPOINT TIME BONUS";
/* asm: CPBTT2	SPTR	"10-25 SECONDS" */
static const char CPBTT2[] = "10-25 SECONDS";

static void SET_CHECKPOINT_BONUS(void) {
    // asm 00000F1C: 	READAUD	ADJ_CHECKPOINT_BONUS
    // asm 00000F1E: 	LDI	R0,R5
    // asm 00000F1F: 	LDI	@CPBTT,AR2
    // asm 00000F20: 	LDI	@CPBTT2,AR3
    // asm 00000F21: 	LDI	10,R6
    // asm 00000F22: 	LDI	25,R7
    // asm 00000F23: 	LDI	@CHECKPOINT_BONUS_CVTI,AR4
    // asm 00000F24: 	CALL	MULTI_VALUE
    // asm 00000F25: 	LDI	R5,R2
    // asm 00000F26: 	SETADJ	ADJ_CHECKPOINT_BONUS
    // asm 00000F28: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_CHECKPOINT_BONUS", 0, 0);
    UNIMPL();
}

static void CHECKPOINT_BONUS_CVT(void) {
    // asm 00000F2A: 	PUSH	R2
    // asm 00000F2B: 	LDI	R5,R2
    // asm 00000F2C: 	LDI	@bufferI,AR2
    // asm 00000F2D: 	CALL	_itoa
    // asm 00000F2E: 	LDI	@bufferI,AR2
    // asm 00000F2F: 	POP	R2
    // asm 00000F30: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKPOINT_BONUS_CVT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: MXT1	SPTR	"MAX CREDITS" */
static const char MXT1[] = "MAX CREDITS";
/* asm: MXT2	SPTR	"10-50" */
static const char MXT2[] = "10-50";

static void SET_MAX_CREDITS(void) {
    // asm 00000F33: 	READAUD	ADJ_MAX_CREDITS
    // asm 00000F35: 	LDI	R0,R5
    // asm 00000F36: 	LDI	@MXT1,AR2
    // asm 00000F37: 	LDI	@MXT2,AR3
    // asm 00000F38: 	LDI	10,R6
    // asm 00000F39: 	LDI	50,R7
    // asm 00000F3A: 	LDI	@CHECKPOINT_BONUS_CVTI,AR4
    // asm 00000F3B: 	CALL	MULTI_VALUE
    // asm 00000F3C: 	LDI	R5,R2
    // asm 00000F3D: 	SETADJ	ADJ_MAX_CREDITS
    // asm 00000F3F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MAX_CREDITS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 */
/* asm: MVL	SPTR	"MINIMUM VOLUME LEVEL" */
static const char MVL[] = "MINIMUM VOLUME LEVEL";

static void SET_MIN_VOL_LEVEL(void) {
    // asm 00000F41: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm 00000F43: 	FLOAT	R0,R5
    // asm 00000F44: 	MPYF	0.11764,R5
    // asm 00000F45: 	FIX	R5
    // asm 00000F46: 	LDI	@MVL,AR2
    // asm 00000F47: 	LDI	@NULLSTR,AR3
    // asm 00000F48: 	CLRI	R6
    // asm 00000F49: 	LDI	30,R7
    // asm 00000F4A: 	LDI	@SET_MIN_VOL_LEVEL_CVTI,AR4
    // asm 00000F4B: 	CALL	MULTI_VALUE
    // asm 00000F4C: 	FLOAT	R5
    // asm 00000F4D: 	MPYF	8.5,R5
    // asm 00000F4E: 	FIX	R5,R2
    // asm 00000F4F: 	SETADJ	ADJ_MIN_VOL_LEVEL
    // asm 00000F51: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MIN_VOL_LEVEL", 0, 0);
    UNIMPL();
}

/*
 *
 *
 */
static void SET_MIN_VOL_LEVEL_CVT(void) {
    // asm 00000F53: 	PUSH	R2
    // asm 00000F54: 	LDI	R5,R2
    // asm 00000F55: 	LDI	@bufferI,AR2
    // asm 00000F56: 	CALL	_itoa
    // asm 00000F57: 	LDI	@bufferI,AR2
    // asm 00000F58: 	POP	R2
    // asm 00000F59: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MIN_VOL_LEVEL_CVT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *MULTI VALUE
 *
 *PARAMETERS
 *	AR2	STRING 1
 *	AR3	STRING 2
 *	AR4	ROUTINE TO RETURN VALUE STRING
 *	R5	INITIAL VALUE
 *	R6	MIN VALUE
 *	R7	MAX VALUE
 *	(NO WRAP AROUND)
 *RETURNS
 *	R5	VALUE (INDEX)
 *
 */

/* asm: REPEAT_SW	.bss	REPEAT_SW,1 */
int REPEAT_SW;
/* asm: REPEAT_TIME	.bss	REPEAT_TIME,1 */
int REPEAT_TIME;
/*
 *
 *
 *PARAMETERS (ADDITIONAL FOR STEP)
 *	R4	STEP
 *
 *
 */

static void MULTI_VALUE(void) {
    // asm 00000F5A: 	LDI	1,R4
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MULTI_VALUE", 0, 0);
    UNIMPL();
}

static void MULTI_VALUE_STEP(void) {
    // asm 00000F5B: 	PUSH	R5
    // asm 00000F5C: 	PUSH	R6
    // asm 00000F5D: 	PUSH	R7
    // asm 00000F5E: 	PUSH	AR4
    // asm 00000F5F: 	PUSH	AR5
    // asm 00000F60: 	PUSH	AR3
    // asm 00000F61: 	PUSH	AR2
    // asm 00000F62: 	WAIT_MAC_NOREPEAT
    // asm 00000F66: 	LDI	10000,R0
    // asm 00000F67: 	MPYI	55,R0
    // asm 00000F68: 	STI	R0,@REPEAT_TIME
    // asm 00000F69: 	LDI	0,R0
    // asm 00000F6A: 	STI	R0,@REPEAT_SW
    // asm 00000F6B: 	WINDOW	110,115,320,285
    // asm 00000F70: 	IWINDOW	120,125,310,275
    // asm 00000F75: 	LDI	COL_VDGREY,RC
    // asm 00000F76: 	POP	AR2
    // asm 00000F77: 	LDI	130,R2
    // asm 00000F78: 	LDI	140,R3
    // asm 00000F79: 	CALL	_outtextxyc
    // asm 00000F7A: 	POP	AR2
    // asm 00000F7B: 	LDI	130,R2
    // asm 00000F7C: 	LDI	150,R3
    // asm 00000F7D: 	CALL	_outtextxyc
    // asm 00000F7E: 	TEXTITT	"ENTER TO ACCEPT",130,225
    // asm 00000F82: 	TEXTITT	"UP   TO INCREASE",130,235
    // asm 00000F86: 	TEXTITT	"DOWN   TO DECREASE",130,245
    // asm 00000F8A: 	CALL	WAIT_FOR_VBLANK
    // asm 00000F8B: 	CALL	WAIT_FOR_VBLANK
    // asm 00000F8C: 	POP	AR7
    // asm 00000F8D: 	POP	AR6
    // asm 00000F8E: 	POP	R7
    // asm 00000F8F: 	POP	R6
    // asm 00000F90: 	POP	R5
DVLOOP:
    // asm 00000F91: 	CMPI	R6,R5
    // asm 00000F92: 	LDILT	R6,R5
    // asm 00000F93: 	CMPI	R7,R5
    // asm 00000F94: 	LDIGT	R7,R5
    // asm 00000F95: 	FILL	170,200,309,209,0
    // asm 00000F9B: 	LDI	AR4,R0
    // asm 00000F9C: 	CALLU	R0		;VALUE PASSED IN R5 (ALL REGS PRESERVED)
    // asm 00000F9D: 	LDI	COL_VDGREY,RC
    // asm 00000F9E: 	LDI	170,R2
    // asm 00000F9F: 	LDI	200,R3
    // asm 00000FA0: 	CALL	_outtextxyc
    // asm 00000FA1: 	CALL	WAIT_FOR_VBLANK
    // asm 00000FA2: 	LDI	@_newbut,R0	;old system method
    // asm 00000FA3: 	TSTB    SW_DIAG,R0
    // asm 00000FA4: 	BZ	DOEXITDV
    // asm 00000FA5: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm 00000FA6: 	BZ	CHNGDV
    // asm 00000FA7: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm 00000FA8: 	BNZ	NCHNGDV
    // asm 00000FA9: 	SUBI	R4,R5
    // asm 00000FAA: 	LDI	980,AR2
    // asm 00000FAB: 	CALL	SENDSND
    // asm 00000FAC: 	BU	NCHNGDV
CHNGDV:
    // asm 00000FAD: 	ADDI	R4,R5
    // asm 00000FAE: 	LDI	981,AR2
    // asm 00000FAF: 	CALL	SENDSND
NCHNGDV:
    // asm 00000FB0: 	LDI	@REPEAT_TIME,R1
WT5:
    // asm 00000FB1: 	DEC	R1
    // asm 00000FB2: 	BLT	WTT5
    // asm 00000FB3: 	LDI	@_newbut,R0
    // asm 00000FB4: 	AND	SW_VOLPLUS|SW_VOLMINUS|SW_DIAG,R0
    // asm 00000FB5: 	CMPI	@REPEAT_SW,R0
    // asm 00000FB6: 	BEQ	WT5
WTT5:
    // asm 00000FB7: 	CMPI	@REPEAT_SW,R0
    // asm 00000FB8: 	BNE	WTTT5
    // asm 00000FB9: 	LDI	5000,R1
    // asm 00000FBA: 	MPYI	55,R1
    // asm 00000FBB: 	STI	R1,@REPEAT_TIME
    // asm 00000FBC: 	B	DVLOOP
WTTT5:
    // asm 00000FBD: 	LDI	10000,R1
    // asm 00000FBE: 	MPYI	55,R1
    // asm 00000FBF: 	STI	R1,@REPEAT_TIME
    // asm 00000FC0: 	STI	R0,@REPEAT_SW
    // asm 00000FC1: 	B	DVLOOP
DOEXITDV:
    // asm 00000FC2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MULTI_VALUE_STEP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: SPM	SPTR	"STANDARD PRICING MENU" */
static const char SPM[] = "STANDARD PRICING MENU";

static void RUN_STANDARD_PRICING(void) {
    // asm 00000FC4: 	LDL	STANDARD_PRICING_MENU,AR5
    // asm 00000FC5: 	LDI	STANDARD_PRICING_MENU_LEN,AR6
    // asm 00000FC6: 	LDI	@SPM,AR7
    // asm 00000FC7: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_STANDARD_PRICING", 0, 0);
    UNIMPL();
}

static const char SELTP[] = "SELECT THIS PRICING";

static void PRICE_YES_NO(void) {
    // asm 00000FC8: 	LDI	-1,R0
    // asm 00000FC9: 	STI	R0,@PSLAST		;JUMP START PRINT STANDARD
    // asm 00000FCA: 	LDL	SELTP,AR2
    // asm 00000FCB: 	LDI	@AREUSURE,AR3
    // asm 00000FCC: 	PUSH	R6
    // asm 00000FCD: 	CALL	CLEAR_YES_NO
    // asm 00000FCE: 	POP	R6
    // asm 00000FCF: 	RETSC
    // asm 00000FD0: 	LDI	R6,R2
    // asm 00000FD1: 	ADDI	@COUNTRY,R2
    // asm 00000FD2: 	LDI	ADJ_COINMODE,AR2
    // asm 00000FD3: 	CALL	ADJUSTMENT_WRITE
    // asm 00000FD4: 	LDI	1,R2
    // asm 00000FD5: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm 00000FD6: 	CALL	ADJUSTMENT_WRITE
    // 	;IF YOU CHANGE MODES YOU RESET THE CREDITS
    // 	;
    // asm 00000FD7: 	LDI	0,R2
    // asm 00000FD8: 	LDI	AUD_PCREDITS,AR2
    // asm 00000FD9: 	CALL	ADJUSTMENT_WRITE
    // asm 00000FDA: 	LDI	0,R2
    // asm 00000FDB: 	LDI	AUD_CREDITS,AR2
    // asm 00000FDC: 	CALL	ADJUSTMENT_WRITE
    // 	;NOTE: AUD_BCREDITS RESET DURING GAME INIT
    // asm 00000FDD: 	CALL	SET_COINAGE_ADJ		;Set the custom coinage to the standard just selected
    // asm 00000FDE: 	LDI	@STP,AR2
    // asm 00000FDF: 	CALL	MESSAGE_BOX
    // asm 00000FE0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRICE_YES_NO", 0, 0);
    UNIMPL();
}

/* asm: STP	SPTR	"PRICING SELECTED" */
static const char STP[] = "PRICING SELECTED";
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

/* asm: RCMT	SPTR	"CUSTOM PRICING" */
static const char RCMT[] = "CUSTOM PRICING";

static void RUN_CUSTOM_PRICING(void) {
    // asm 00000FE3: 	LDL	CUSTOM_PRICING_MENU,AR5
    // asm 00000FE4: 	LDI	CUSTOM_PRICING_MENU_LEN,AR6
    // asm 00000FE5: 	LDI	@RCMT,AR7
    // asm 00000FE6: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_CUSTOM_PRICING", 0, 0);
    UNIMPL();
}

static const char FRACYN[] = "SHOW FRACTION";

static void FRACTION_YES_NO(void) {
    // asm 00000FE7: 	READAUD	ADJ_SHOW_FRAC
    // asm 00000FE9: 	LDI	R0,R5
    // asm 00000FEA: 	LDL	FRACYN,AR2
    // asm 00000FEB: 	LDI	@NULLSTR,AR3
    // asm 00000FEC: 	CALL	ASK_YES_NO
    // asm 00000FED: 	LDI	1,R2
    // asm 00000FEE: 	LDIC	0,R2
    // asm 00000FEF: 	LDI	ADJ_SHOW_FRAC,AR2
    // asm 00000FF0: 	CALL	ADJUSTMENT_WRITE
    // asm 00000FF1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FRACTION_YES_NO", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static const char USECP[] = "USE CUSTOM PRICING";

static void CUSTOM_YES_NO(void) {
    // asm 00000FF2: 	READAUD	ADJ_STANDARD_PRICING
    // asm 00000FF4: 	LDI	R0,R5
    // asm 00000FF5: 	XOR	1,R5
    // asm 00000FF6: 	LDL	USECP,AR2
    // asm 00000FF7: 	LDI	@NULLSTR,AR3
    // asm 00000FF8: 	CALL	ASK_YES_NO
    // asm 00000FF9: 	LDI	0,R2
    // asm 00000FFA: 	LDIC	1,R2
    // asm 00000FFB: 	PUSH	R2
    // asm 00000FFC: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm 00000FFD: 	CALL	ADJUSTMENT_WRITE
    // asm 00000FFE: 	POP	R2
    // asm 00000FFF: 	CMPI	1,R2
    // asm 00001000: 	BEQ	CUSTOM_NO
    // asm 00001001:  	LDI	@CPS,AR2
    // asm 00001002: 	CALL	MESSAGE_BOX
CUSTOM_NO:
    // asm 00001003: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CUSTOM_YES_NO", 0, 0);
    UNIMPL();
}

/* asm: CPS	SPTR	"CUSTOM PRICING SELECTED" */
static const char CPS[] = "CUSTOM PRICING SELECTED";
// *----------------------------------------------------------------------------

/* asm: CMESSAGE_TAB */
/* asm: 	.word	CPCS,CPCS,CPCS,CPCS,CUPC,CTSS,CTCS,CCTS,CCTC,CTPC,CTPC,CTPC,CTPC */
/* asm: 	 */
static uintptr_t CMESSAGE_TAB[] = {
    (uintptr_t)&CPCS,
    (uintptr_t)&CPCS,
    (uintptr_t)&CPCS,
    (uintptr_t)&CPCS,
    (uintptr_t)&CUPC,
    (uintptr_t)&CTSS,
    (uintptr_t)&CTCS,
    (uintptr_t)&CCTS,
    (uintptr_t)&CCTC,
    (uintptr_t)&CTPC,
    (uintptr_t)&CTPC,
    (uintptr_t)&CTPC,
    (uintptr_t)&CTPC,
};
static const char CPCS[] = "UNITS PER COIN";
static const char CUPC[] = "UNITS PER CREDIT";
static const char CTSS[] = "BONUS UNITS";
static const char CTCS[] = "MINIMUM UNITS";
static const char CCTS[] = "CREDITS TO START";
static const char CCTC[] = "CREDITS TO CONTINUE";
static const char CTPC[] = "COUNTER TICKS PER COIN";

static void SET_SLOT(void) {
    // asm 00001032: 	LDI	R6,AR2			;R6 = menu selection index
    // asm 00001033: 	LDI	ADJ_COIN1_UNITS,R0
    // asm 00001034: 	CMPI	9,R6
    // asm 00001035: 	BLT	SET_SLOT1
    // asm 00001036: 	LDI	ADJ_COIN1_COUNTER,R0
    // asm 00001037: 	SUBI	9,R0			;STRIP THE MENU OFFSET
SET_SLOT1:
    // asm 00001038: 	ADDI	R0,AR2
    // asm 00001039: 	CALL	ADJUSTMENT_READ
    // asm 0000103A: 	LDI	R0,R5
    // asm 0000103B: 	LDL	CMESSAGE_TAB,AR2
    // asm 0000103C: 	ADDI	R6,AR2
    // asm 0000103D: 	LDI	*AR2,AR2
    // asm 0000103E: 	LDI	@NULLSTR,AR3
    // asm 0000103F: 	PUSH	R6
    // asm 00001040: 	LDI	0,R6			;RANGE 0 to 100
    // asm 00001041: 	LDI	100,R7
    // asm 00001042: 	LDI	@GAMEDIFF_CVTI,AR4	;This will print the values as numbers
    // asm 00001043: 	CALL	MULTI_VALUE
    // asm 00001044: 	POP	R6
    // asm 00001045: 	LDI	R5,R2
    // asm 00001046: 	LDI	R6,AR2
    // asm 00001047: 	LDI	ADJ_COIN1_UNITS,R0
    // asm 00001048: 	CMPI	9,R6
    // asm 00001049: 	BLT	SET_SLOT2
    // asm 0000104A: 	LDI	ADJ_COIN1_COUNTER,R0
    // asm 0000104B: 	SUBI	9,R0			;STRIP THE MENU OFFSET
SET_SLOT2:
    // asm 0000104C: 	ADDI	R0,AR2
    // asm 0000104D: 	CALL	ADJUSTMENT_WRITE
    // ;IF YOU CHANGE MODES YOU RESET THE CREDITS
    // asm 0000104E: 	LDI	0,R2
    // asm 0000104F: 	LDI	AUD_PCREDITS,AR2
    // asm 00001050: 	CALL	ADJUSTMENT_WRITE
    // asm 00001051: 	LDI	0,R2
    // asm 00001052: 	LDI	AUD_CREDITS,AR2
    // asm 00001053: 	CALL	ADJUSTMENT_WRITE
    // asm 00001054: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_SLOT", 0, 0);
    UNIMPL();
}

/* asm: PSTAB */
/* asm: 	.word	ADJ_COIN1_UNITS,PRINT_NUMBER */
/* asm: 	.word	ADJ_COIN2_UNITS,PRINT_NUMBER */
/* asm: 	.word	ADJ_COIN3_UNITS,PRINT_NUMBER */
/* asm: 	.word	ADJ_COIN4_UNITS,PRINT_NUMBER */
/* asm: 	.word	ADJ_UNITS_PER_CREDIT,PRINT_NUMBER */
/* asm: 	.word	ADJ_BONUS_UNITS,PRINT_NUMBER */
/* asm: 	.word	ADJ_UNITS_MIN,PRINT_NUMBER */
/* asm: 	.word	ADJ_CREDITS_TO_START,PRINT_NUMBER */
/* asm: 	.word	ADJ_CREDITS_TO_CONTINUE,PRINT_NUMBER */
/* asm: 	.word	ADJ_COIN1_COUNTER,PRINT_NUMBER */
/* asm: 	.word	ADJ_COIN2_COUNTER,PRINT_NUMBER */
/* asm: 	.word	ADJ_COIN3_COUNTER,PRINT_NUMBER */
/* asm: 	.word	ADJ_COIN4_COUNTER,PRINT_NUMBER */
/* asm: 	.word	ADJ_SHOW_FRAC,PRINT_YN */
/* asm: 	.word	ADJ_STANDARD_PRICING,PRINT_YN_INV */
/* asm: 	.word	0,0 */
/* asm: 	 */
static uintptr_t PSTAB[] = {
    (uintptr_t)(ADJ_COIN1_UNITS),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_COIN2_UNITS),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_COIN3_UNITS),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_COIN4_UNITS),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_UNITS_PER_CREDIT),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_BONUS_UNITS),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_UNITS_MIN),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_CREDITS_TO_START),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_CREDITS_TO_CONTINUE),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_COIN1_COUNTER),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_COIN2_COUNTER),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_COIN3_COUNTER),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_COIN4_COUNTER),
    (uintptr_t)PRINT_NUMBER,
    (uintptr_t)(ADJ_SHOW_FRAC),
    (uintptr_t)PRINT_YN,
    (uintptr_t)(ADJ_STANDARD_PRICING),
    (uintptr_t)PRINT_YN_INV,
    0,
    0,
};

static void PRINT_SLOT(void) {
    // asm 00001056: 	PUSH	AR4
    // asm 00001057: 	LDI	@PSTABI,AR4
    // asm 00001058: 	CALL	PRINT_VALUE
    // asm 00001059: 	POP	AR4
    // asm 0000105A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_SLOT", 0, 0);
    UNIMPL();
}

static void PRINT_VALUE(void) {
    // asm 0000105B: 	PUSH	R0
    // asm 0000105C: 	PUSH	R1
    // asm 0000105D: 	PUSH	R2
    // asm 0000105E: 	PUSH	AR2
    // asm 0000105F: 	MPYI	2,R0
    // asm 00001060: 	ADDI	R0,AR4
    // asm 00001061: 	LDI	*AR4,AR2
    // asm 00001062: 	CALL	ADJUSTMENT_READ
    // ;	LDI	*+AR4,AR2
    // ;	ADDI	@PVTABI,AR2
    // ;	LDI	*AR2,R1
    // ;	BEQ	PRINT_VALUEX
    // asm 00001063: 	LDI	*+AR4,R1
    // asm 00001064: 	BEQ	PRINT_VALUEX
    // asm 00001065: 	CALLU	R1
PRINT_VALUEX:
    // asm 00001066: 	POP	AR2
    // asm 00001067: 	POP	R2
    // asm 00001068: 	POP	R1
    // asm 00001069: 	POP	R0
    // asm 0000106A: 	RETS
    // ;PVTABI	.word	PVTAB
    // ;
    // ;	.word	0
    // ;	.word	PRINT_MPH_INV	;-4
    // ;	.word	PRINT_ONOFF_INV	;-3
    // ;	.word	PRINT_YN_INV	;-2
    // ;	.word	0		;-1
    // ;PVTAB	.word	0		;0
    // ;	.word	PRINT_NUMBER	;1
    // ;	.word	PRINT_YN	;2
    // ;	.word	PRINT_ONOFF	;3
    // ;	.word	PRINT_MPH	;4
    // ;	.word	PRINT_TIME	;5
    // ;	.word	PRINT_VOL	;6
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_VALUE", 0, 0);
    UNIMPL();
}

static void PRINT_VOL(void) {
    // asm 0000106B: 	PUSH	RC
    // asm 0000106C: 	FLOAT	R0
    // asm 0000106D: 	MPYF	0.11764,R0
    // asm 0000106E: 	FIX	R0
    // asm 0000106F: 	LDI	R0,R2
    // asm 00001070: 	LDI	@bufferI,AR2
    // asm 00001071: 	CALL	_itoa
    // asm 00001072: 	LDI	@bufferI,AR2
    // asm 00001073: 	LDI	270,R2
    // asm 00001074: 	CMPI	10,R0
    // asm 00001075: 	LDIGE	262,R2
    // asm 00001076: 	CMPI	100,R0
    // asm 00001077: 	LDIGE	254,R2
    // asm 00001078: 	POP	RC
    // asm 00001079: 	CALL	_outtextxyc
    // asm 0000107A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_VOL", 0, 0);
    UNIMPL();
}

static void GETSTR_LENGTH(void) {
    // asm 0000107B: 	PUSH	AR2
    // asm 0000107C: 	PUSH	R0
    // asm 0000107D: 	PUSH	R1
    // asm 0000107E: 	PUSH	R3
    // asm 0000107F: 	CLRI	R2
GSLP:
    // asm 00001080: 	CLRI	R1
    // asm 00001081: 	LDI	*AR2++,R3
GSLP1:
    // asm 00001082: 	LDI	R3,R0
    // asm 00001083: 	LSH	R1,R0
    // asm 00001084: 	AND	0FFH,R0
    // asm 00001085: 	BEQ	GSLDONE
    // asm 00001086: 	ADDI	1,R2
    // asm 00001087: 	SUBI	8,R1
    // asm 00001088: 	CMPI	-32,R1
    // asm 00001089: 	BGT	GSLP1
    // asm 0000108A: 	BR	GSLP
GSLDONE:
    // asm 0000108B: 	POP	R3
    // asm 0000108C: 	POP	R1
    // asm 0000108D: 	POP	R0
    // asm 0000108E: 	POP	AR2
    // asm 0000108F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSTR_LENGTH", 0, 0);
    UNIMPL();
}

static void PRINT_COINMODE(void) {
    // asm 00001090: 	PUSH	R0
    // asm 00001091: 	READADJ	ADJ_STANDARD_PRICING
    // asm 00001093: 	POP	R2
    // asm 00001094: 	CMPI	0,R0
    // asm 00001095: 	RETSEQ
    // asm 00001096: 	ADDI	@COINMSGC_TABI,R2
    // asm 00001097: 	LDI	R2,AR2
    // asm 00001098: 	LDI	*AR2,AR2
    // asm 00001099: 	CALL	GETSTR_LENGTH
    // asm 0000109A: 	MPYI	-8,R2
    // asm 0000109B: 	ADDI	278,R2
    // asm 0000109C: 	CALL	_outtextxyc
    // asm 0000109D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_COINMODE", 0, 0);
    UNIMPL();
}

static void PRINT_CUSTOM_SELECTED(void) {
    // asm 0000109E: 	CMPI	1,R0
    // asm 0000109F: 	RETSEQ
    // asm 000010A0: 	LDI	@ISON,AR2
    // asm 000010A1: 	LDI	262,R2
    // asm 000010A2: 	CALL	_outtextxyc
    // asm 000010A3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_CUSTOM_SELECTED", 0, 0);
    UNIMPL();
}

static void PRINT_TIME(void) {
    // asm 000010A4: 	MPYI	5,R0
    // asm 000010A5: 	ADDI	60,R0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_TIME", 0, 0);
    UNIMPL();
}

static void PRINT_NUMBER(void) {
    // asm 000010A6: 	PUSH	RC
    // asm 000010A7: 	LDI	R0,R2
    // asm 000010A8: 	LDI	@bufferI,AR2
    // asm 000010A9: 	CALL	_itoa
    // asm 000010AA: 	LDI	@bufferI,AR2
    // asm 000010AB: 	LDI	270,R2
    // asm 000010AC: 	CMPI	10,R0
    // asm 000010AD: 	LDIGE	262,R2
    // asm 000010AE: 	CMPI	100,R0
    // asm 000010AF: 	LDIGE	254,R2
    // asm 000010B0: 	CMPI	1000,R0
    // asm 000010B1: 	LDIGE	246,R2
    // asm 000010B2: 	CMPI	10000,R0
    // asm 000010B3: 	LDIGE	238,R2
    // asm 000010B4: 	PUSH	R1
    // asm 000010B5: 	LDI	10000,R1
    // asm 000010B6: 	MPYI	10,R1
    // asm 000010B7: 	CMPI	R1,R0
    // asm 000010B8: 	LDIGE	230,R2
    // asm 000010B9: 	POP	R1
    // asm 000010BA: 	POP	RC
    // asm 000010BB: 	CALL	_outtextxyc
    // asm 000010BC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_NUMBER", 0, 0);
    UNIMPL();
}

static void PRINT_YN_INV(void) {
    // asm 000010BD: 	NEGI	R0
    // asm 000010BE: 	ADDI	1,R0	;Drops through to PRINT_YN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_YN_INV", 0, 0);
    UNIMPL();
}

static void PRINT_YN(void) {
    // asm 000010BF: 	LDI	@YES,AR2
    // asm 000010C0: 	LDI	254,R2
    // asm 000010C1: 	CMPI	0,R0
    // asm 000010C2: 	LDIEQ	@NO,AR2
    // asm 000010C3: 	LDIEQ	262,R2
    // asm 000010C4: 	CALL	_outtextxyc
    // asm 000010C5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_YN", 0, 0);
    UNIMPL();
}

static void PRINT_ONOFF_INV(void) {
    // asm 000010C6: 	NEGI	R0
    // asm 000010C7: 	ADDI	1,R0	;Drops through to PRINT_ONOFF
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_ONOFF_INV", 0, 0);
    UNIMPL();
}

static void PRINT_ONOFF(void) {
    // asm 000010C8: 	LDI	@ISON,AR2
    // asm 000010C9: 	LDI	262,R2
    // asm 000010CA: 	CMPI	0,R0
    // asm 000010CB: 	LDIEQ	@ISOFF,AR2
    // asm 000010CC: 	LDIEQ	254,R2
    // asm 000010CD: 	CALL	_outtextxyc
    // asm 000010CE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_ONOFF", 0, 0);
    UNIMPL();
}

static void PRINT_MPH_INV(void) {
    // asm 000010CF: 	NEGI	R0
    // asm 000010D0: 	ADDI	1,R0	;Drops through to PRINT_MPH
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_MPH_INV", 0, 0);
    UNIMPL();
}

static void PRINT_MPH(void) {
    // asm 000010D1: 	LDI	@MP3,AR2
    // asm 000010D2: 	LDI	254,R2
    // asm 000010D3: 	CMPI	1,R0
    // asm 000010D4: 	LDIEQ	@MP2,AR2
    // asm 000010D5: 	CALL	_outtextxyc
    // asm 000010D6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_MPH", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *MENU DEFINITIONS
 *
 *
 */
#define MAIN_MENU_LEN 7
/* asm: MAIN_MENU: */
/* asm: MENUENTRY	"DIAGNOSTIC MENU",RUN_DIAG_MENU */
/* asm: MENUENTRY	"GAME AUDITS",AUDIT_DISPLAY */
/* asm: MENUENTRY	"ADJUSTMENTS MENU",RUN_ADJUSTMENT_MENU */
/* asm: MENUENTRY	"UTILITIES",RUN_UTILITIES_MENU */
/* asm: MENUENTRY	"CALIBRATE CONTROLS",SET_CONTROLS */
/* asm: MENUENTRY	"ADJUST VOLUME",SET_VOLUME */
/* asm: MENUENTRY	"EXIT TO GAME OVER",DIAG_RETURN */
static int MAIN_MENU;
#define DIAGNOSTIC_MENU_LEN 10
/* asm: DIAGNOSTIC_MENU: */
/* asm: MENUENTRY	"SWITCH TEST",SHOWSWITCH */
/* asm: MENUENTRY	"DIP SWITCH TEST",SHOW_DIPSWITCH */
/* asm: MENUENTRY	"CPU BOARD TEST",TEST_CHIPS */
/* asm: MENUENTRY	"SOUND BOARD TEST",SOUND_BOARD_TEST */
/* asm: MENUENTRY	"STEERING WHEEL TEST",STEERING_WHEEL_TEST */
/* asm: MENUENTRY	"MONITOR TEST",MONITOR_DIAGNOSTICS */
/* asm: MENUENTRY	"BURNIN TEST",BURNIN_TEST */
/* asm: MENUENTRY	"MOTION MENU",RUN_MOTION_MENU		;MOTION_BURNIN */
/* asm: MENUENTRY	"LAMP TEST",RUN_LAMP_TEST */
/* asm: MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN */
static int DIAGNOSTIC_MENU;
#define UTILITIES_MENU_LEN 6
/* asm: UTILITIES_MENU: */
/* asm: MENUENTRY	"CLEAR CREDITS",CLEAR_CREDITS */
/* asm: MENUENTRY	"CLEAR GAME AUDITS",CLEAR_GAME_AUDITS */
/* asm: MENUENTRY	"RESET HIGH SCORES",RESET_HIGH_SCORES */
/* asm: MENUENTRY	"DEFAULT ADJUSTMENTS",DEFAULT_ADJUSTMENTS */
/* asm: MENUENTRY	"FULL FACTORY RESTORE",FULL_FACTORY_RESTORE */
/* asm: MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN */
static int UTILITIES_MENU;
#define ADJUSTMENT_MENU_LEN 18
/* asm: ADJUSTMENT_MENU */
/* asm: MENUENTRY	"STANDARD PRICING",RUN_STANDARD_PRICING */
/* asm: MENUENTRY	"CUSTOM PRICING",RUN_CUSTOM_PRICING */
/* asm: MENUENTRY	"FREE PLAY",FREEPLAYONOFF */
/* asm: MENUENTRY	"FIRST PLACE AWARDS FREE GAME",FREEGAMEONOFF */
/* asm: MENUENTRY	"START TIME BONUS SECS",SET_TIME_TO_START */
/* asm: MENUENTRY	"CHECKPOINT BONUS TIME SECS",SET_CHECKPOINT_BONUS */
/* asm: MENUENTRY	"ATTRACT MODE SOUND",AMODESND */
/* asm: MENUENTRY	"HIGH SCORE ENTRY",HIGHSCORE_ENTRY */
/* asm: MENUENTRY	"MIN VOLUME LEVEL",SET_MIN_VOL_LEVEL */
/* asm: MENUENTRY	"SPEED IN MPH OR KPH",MILES_OR_KILO */
/* asm: MENUENTRY	"SHOW ROADKILL",ROADKILL_ONOFF */
/* asm: MENUENTRY	"SHOW PRESIDENT",CLINTON_ONOFF */
/* asm: MENUENTRY	"SHOW GIRLS",GIRLS_ONOFF */
/* asm: MENUENTRY	"HIGHSCORE RESET",SET_HIGHSCORE_RESET */
/* asm: MENUENTRY	"GAME DIFFICULTY",SET_GAME_DIFFICULTY */
/* asm: MENUENTRY	"MAX CREDITS",SET_MAX_CREDITS */
/* asm: MENUENTRY	"STEERING SENSITIVITY",SET_STEERING_SENSITIVITY */
/* asm: MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN */
static int ADJUSTMENT_MENU;
#define MOTION_MENU_LEN 7
/* asm: MOTION_MENU */
/* asm: MENUENTRY	"FRONT AXIS UP/DOWN",DIAG_MOTION_TESTZ */
/* asm: MENUENTRY	"LEFT REAR AXIS UP/DOWN",DIAG_MOTION_TESTX */
/* asm: MENUENTRY	"RIGHT REAR AXIS UP/DOWN",DIAG_MOTION_TESTY */
/* asm: MENUENTRY	"COMBINED MOTION",DIAG_MOTION_BURNIN */
/* asm: MENUENTRY	"MOTION SAFETY SWITCHES",MOTION_SAFETY_SWITCHES_DIAG */
static int MOTION_MENU;
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: LAMPT	SPTR	"LAMP TEST" */
static const char LAMPT[] = "LAMP TEST";
#define LAMP_MENU_LEN 11
/* asm: LAMP_MENU: */
/* asm: MENUENTRY	"TURN ON ALL LAMPS",LAMPSALL */
/* asm: MENUENTRY	"TURN OFF ALL LAMPS",LAMPSOFF */
/* asm: MENUENTRY	"TURN ON START",LAMPS */
/* asm: MENUENTRY	"TURN ON VIEW 1",LAMPV1 */
/* asm: MENUENTRY	"TURN ON VIEW 2",LAMPV2 */
/* asm: MENUENTRY	"TURN ON VIEW 3",LAMPV3 */
/* asm: MENUENTRY	"TURN ON LEFT TAIL",LAMPLT */
/* asm: MENUENTRY	"TURN ON RIGHT TAIL",LAMPRT */
/* asm: MENUENTRY	"TURN ON LEFT FRONT",LAMPLF */
/* asm: MENUENTRY	"TURN ON RIGHT FRONT",LAMPRF */
/* asm: MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN */
static int LAMP_MENU;

static void RUN_LAMP_TEST(void) {
    // asm 00001155: 	LDL	LAMP_MENU,AR5
    // asm 00001156: 	LDI	LAMP_MENU_LEN,AR6
    // asm 00001157: 	LDI	@LAMPT,AR7
    // asm 00001158: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_LAMP_TEST", 0, 0);
    UNIMPL();
}

static void LAMPSOFF(void) {
    // asm 00001159: 	LDI	0,R0
    // asm 0000115A: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPSOFF", 0, 0);
    UNIMPL();
}

static void LAMPSALL(void) {
    // asm 0000115B: 	LDI	0FFh,R0
    // asm 0000115C: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPSALL", 0, 0);
    UNIMPL();
}

static void LAMPS(void) {
    // asm 0000115D: LDI	1,R0
    // asm 0000115E: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPS", 0, 0);
    UNIMPL();
}

static void LAMPV1(void) {
    // asm 0000115F: LDI	2,R0
    // asm 00001160: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV1", 0, 0);
    UNIMPL();
}

static void LAMPV2(void) {
    // asm 00001161: LDI	4,R0
    // asm 00001162: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV2", 0, 0);
    UNIMPL();
}

static void LAMPV3(void) {
    // asm 00001163: LDI	8,R0
    // asm 00001164: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV3", 0, 0);
    UNIMPL();
}

static void LAMPLT(void) {
    // asm 00001165: LDI	10h,R0
    // asm 00001166: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPLT", 0, 0);
    UNIMPL();
}

static void LAMPRT(void) {
    // asm 00001167: LDI	20h,R0
    // asm 00001168: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPRT", 0, 0);
    UNIMPL();
}

static void LAMPLF(void) {
    // asm 00001169: LDI	40h,R0
    // asm 0000116A: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPLF", 0, 0);
    UNIMPL();
}

static void LAMPRF(void) {
    // asm 0000116B: LDI	80h,R0
LJIN:
    // asm 0000116C: LDP	@BUTTON_STATUS
    // asm 0000116D: 	STI	R0,@BUTTON_STATUS
    // asm 0000116E: 	SETDP
    // asm 0000116F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPRF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MOTION_CHECK_SAFETY_CODES(void) {
    // asm 00001170: 	PUSH	R0
    // asm 00001171: 	PUSH	R1
    // asm 00001172: 	PUSH	R2
    // asm 00001173: 	PUSH	R3
    // asm 00001174: 	CALL	CLEAR_MATS
    // asm 00001175: 	CLRI	AR2
    // asm 00001176: 	LDP	@991030h
    // asm 00001177: 	LDI	@991030h,R0
    // asm 00001178: 	LDI	*AR2,AR2
    // asm 00001179: 	SETDP
    // asm 0000117A: 	RS	8,R0		;right justify
    // asm 0000117B: 	LDL	MOTION_SAFETY_CODES,AR2
MCSC_LP:
    // asm 0000117C: LDI	*AR2,R1
    // asm 0000117D: 	CMPI	-1,R1
    // asm 0000117E: 	BEQ	NO_ERRORS
    // asm 0000117F: 	AND	*+AR2(1),R1
    // asm 00001180: 	LDI	R0,R3
    // asm 00001181: 	AND	*+AR2(1),R3
    // asm 00001182: 	CMPI	R3,R1
    // asm 00001183: 	BNE	JJDDA
    // asm 00001184: 	LDI	*+AR2(2),R0
    // asm 00001185: 	CALLU	R0
    // asm 00001186: 	BU	ALL_DONE
JJDDA:
    // asm 00001187: NOP	*++AR2(3)
    // asm 00001188: 	BU	MCSC_LP
NO_ERRORS:
    // asm 00001189: 	IWINDOW	50,320,450,360
    // asm 0000118E: 	LDI	COL_BLACK,RC
    // asm 0000118F: 	TEXTITT	"ALL SYSTEMS OK",70,330
ALL_DONE:
    // asm 00001193: 	POP	R3
    // asm 00001194: 	POP	R2
    // asm 00001195: 	POP	R1
    // asm 00001196: 	POP	R0
    // asm 00001197: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_CHECK_SAFETY_CODES", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

/* asm: MOTION_SAFETY_CODES: */
/* asm: MSCM	080h,080h,MSC_BOARD_NOTPLUGGED */
/* asm: MSCM	070h,0F0h,MSC_FAILSAFE_NOTCONNECTED */
/* asm: MSCM	060h,0F0h,MSC_FAILSAFE_OK */
/* asm: MSCM	050h,0F0h,MSC_OPTO_LED */
/* asm: MSCM	040h,0F0h,MSC_OPTO_DETECTOR */
/* asm: MSCM	030h,0F0h,MSC_OPTO_PATH */
/* asm: MSCM	020h,0F0h,MSC_MAT_STEPPED */
/* asm: MSCM	010h,0F0h,MSC_MAT_NOTPLUGGED */
static int MOTION_SAFETY_CODES;
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MSC_BOARD_NOTPLUGGED(void) {
    // asm 000011B1: 	IWINDOW	50,320,450,360
    // asm 000011B6: 	LDI	COL_BLACK,RC
    // asm 000011B7: 	TEXTITT	"BOARD NOT PLUGGED IN OR PAL NOT INSTALLED",70,330
    // asm 000011BB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_BOARD_NOTPLUGGED", 0, 0);
    UNIMPL();
}

static void MSC_MAT_STEPPED(void) {
    // asm 000011BC: 	IWINDOW	50,320,450,360
    // asm 000011C1: 	LDI	COL_BLACK,RC
    // asm 000011C2: 	TEXTITT	"MAT STEPPED ON",70,330
    // asm 000011C6: 	CLRI	AR2
    // asm 000011C7: 	LDP	@991030h
    // asm 000011C8: 	LDI	@991030h,R4
    // asm 000011C9: 	LDI	*AR2,AR2
    // asm 000011CA: 	SETDP
    // asm 000011CB: 	RS	8,R4		;right justify
    // asm 000011CC: 	TSTB	1,R4
    // asm 000011CD: 	BZ	NM1
    // asm 000011CE: 	FILL	102,142,128,218,COL_RED
NM1:
    // asm 000011D4: TSTB	2,R4
    // asm 000011D5: 	BZ	NM2
    // asm 000011D6: 	FILL	32,232,128,278,COL_RED
NM2:
    // asm 000011DC: TSTB	4,R4
    // asm 000011DD: 	BZ	NM3
    // asm 000011DE: 	FILL	32,142,58,218,COL_RED
NM3:
    // asm 000011E4: TSTB	8,R4
    // asm 000011E5: 	BZ	NM4
    // asm 000011E6: 	FILL	32,82,128,118,COL_RED
NM4:
    // asm 000011EC: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_MAT_STEPPED", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MSC_MAT_NOTPLUGGED(void) {
    // asm 000011ED: 	IWINDOW	50,320,450,360
    // asm 000011F2: 	LDI	COL_BLACK,RC
    // asm 000011F3: 	TEXTITT	"MAT NOT PLUGGED IN",70,330
    // asm 000011F7: 	CLRI	AR2
    // asm 000011F8: 	LDP	@991030h
    // asm 000011F9: 	LDI	@991030h,R4
    // asm 000011FA: 	LDI	*AR2,AR2
    // asm 000011FB: 	SETDP
    // asm 000011FC: 	RS	8,R4		;right justify
    // asm 000011FD: 	TSTB	1,R4
    // asm 000011FE: 	BZ	NM1A
    // asm 000011FF: 	FILL	102,142,128,218,COL_RED
NM1A:
    // asm 00001205: TSTB	2,R4
    // asm 00001206: 	BZ	NM2A
    // asm 00001207: 	FILL	32,232,128,278,COL_RED
NM2A:
    // asm 0000120D: TSTB	4,R4
    // asm 0000120E: 	BZ	NM3A
    // asm 0000120F: 	FILL	32,142,58,218,COL_RED
NM3A:
    // asm 00001215: TSTB	8,R4
    // asm 00001216: 	BZ	NM4A
    // asm 00001217: 	FILL	32,82,128,118,COL_RED
NM4A:
    // asm 0000121D: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_MAT_NOTPLUGGED", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MSC_OPTO_PATH(void) {
    // asm 0000121E: 	IWINDOW	50,320,450,360
    // asm 00001223: 	LDI	COL_BLACK,RC
    // asm 00001224: 	TEXTITT	"OPTO PATH BROKEN",70,330
    // asm 00001228: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_PATH", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MSC_OPTO_LED(void) {
    // asm 00001229: 	IWINDOW	50,320,450,360
    // asm 0000122E: 	LDI	COL_BLACK,RC
    // asm 0000122F: 	TEXTITT	"OPTO LED NOT EMITTING",70,330
    // asm 00001233: 	CLRI	AR2
    // asm 00001234: 	LDP	@991030h
    // asm 00001235: 	LDI	@991030h,R4
    // asm 00001236: 	LDI	*AR2,AR2
    // asm 00001237: 	SETDP
    // asm 00001238: 	RS	8,R4		;right justify
    // asm 00001239: 	TSTB	1,R4
    // asm 0000123A: 	BZ	NJ1
    // asm 0000123B: 	FILLW	150+32,212,16,16,COL_RED	;L1
NJ1:
    // asm 00001241: TSTB	2,R4
    // asm 00001242: 	BZ	NJ2
    // asm 00001243: 	FILLW	150+57,232,16,16,COL_RED	;L2
NJ2:
    // asm 00001249: TSTB	4,R4
    // asm 0000124A: 	BZ	NJ3
    // asm 0000124B: 	FILLW	150+87,232,16,16,COL_RED	;L3
NJ3:
    // asm 00001251: TSTB	8,R4
    // asm 00001252: 	BZ	NJ4
    // asm 00001253: 	FILLW	150+112,212,16,16,COL_RED 	;L4
NJ4:
    // asm 00001259: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_LED", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MSC_OPTO_DETECTOR(void) {
    // asm 0000125A: 	IWINDOW	50,320,450,360
    // asm 0000125F: 	LDI	COL_BLACK,RC
    // asm 00001260: 	TEXTITT	"OPTO DETECTOR NOT RECEIVING",70,330
    // asm 00001264: 	CLRI	AR2
    // asm 00001265: 	LDP	@991030h
    // asm 00001266: 	LDI	@991030h,R4
    // asm 00001267: 	LDI	*AR2,AR2
    // asm 00001268: 	SETDP
    // asm 00001269: 	RS	8,R4		;right justify
    // asm 0000126A: 	TSTB	1,R4
    // asm 0000126B: 	BZ	NJ1A
    // asm 0000126C: 	FILLW	150+32,112,16,16,COL_RED	;D1
NJ1A:
    // asm 00001272: TSTB	2,R4
    // asm 00001273: 	BZ	NJ2A
    // asm 00001274: 	FILLW	150+57,82,16,16,COL_RED		;D2
NJ2A:
    // asm 0000127A: TSTB	4,R4
    // asm 0000127B: 	BZ	NJ3A
    // asm 0000127C: 	FILLW	150+87,82,16,16,COL_RED		;D3
NJ3A:
    // asm 00001282: TSTB	8,R4
    // asm 00001283: 	BZ	NJ4A
    // asm 00001284: 	FILLW	150+112,112,16,16,COL_RED 	;D4
NJ4A:
    // asm 0000128A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_DETECTOR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MSC_FAILSAFE_OK(void) {
    // asm 0000128B: 	IWINDOW	50,320,450,360
    // asm 00001290: 	LDI	COL_BLACK,RC
    // asm 00001291: 	TEXTITT	"FAIL SAFE SWITCH ENGAGED",70,330
    // asm 00001295: 	CLRI	AR2
    // asm 00001296: 	LDP	@991030h
    // asm 00001297: 	LDI	@991030h,R4
    // asm 00001298: 	LDI	*AR2,AR2
    // asm 00001299: 	SETDP
    // asm 0000129A: 	RS	8,R4		;right justify
    // asm 0000129B: 	TSTB	4,R4
    // asm 0000129C: 	BZ	NM4B
    // asm 0000129D: 	FILL	150+150+62,82,150+150+98,118,COL_RED
NM4B:
    // asm 000012A3: TSTB	1,R4
    // asm 000012A4: 	BZ	NM1B
    // asm 000012A5: 	FILL	150+150+32,232,150+150+58,278,COL_RED
NM1B:
    // asm 000012AB: TSTB	2,R4
    // asm 000012AC: 	BZ	NM2B
    // asm 000012AD: 	FILL	150+150+102,232,150+150+128,278,COL_RED
NM2B:
    // asm 000012B3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_FAILSAFE_OK", 0, 0);
    UNIMPL();
}

static void MSC_FAILSAFE_NOTCONNECTED(void) {
    // asm 000012B4: 	IWINDOW	50,320,450,360
    // asm 000012B9: 	LDI	COL_BLACK,RC
    // asm 000012BA: 	TEXTITT	"FAIL SAFE SWITCH NOT CONNECTED CORRECTLY",70,330
    // asm 000012BE: 	CLRI	AR2
    // asm 000012BF: 	LDP	@991030h
    // asm 000012C0: 	LDI	@991030h,R4
    // asm 000012C1: 	LDI	*AR2,AR2
    // asm 000012C2: 	SETDP
    // asm 000012C3: 	RS	8,R4		;right justify
    // asm 000012C4: 	TSTB	4,R4
    // asm 000012C5: 	BZ	NM4BA
    // asm 000012C6: 	FILL	150+150+62,82,150+150+98,118,COL_RED
NM4BA:
    // asm 000012CC: TSTB	1,R4
    // asm 000012CD: 	BZ	NM1BA
    // asm 000012CE: 	FILL	150+150+32,232,150+150+58,278,COL_RED
NM1BA:
    // asm 000012D4: TSTB	2,R4
    // asm 000012D5: 	BZ	NM2BA
    // asm 000012D6: 	FILL	150+150+102,232,150+150+128,278,COL_RED
NM2BA:
    // asm 000012DC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_FAILSAFE_NOTCONNECTED", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void DRAW_MOTION_SAFETY_DIAG(void) {
    // asm 000012DD: 	LDI	COL_BLACK,RC
    // asm 000012DE: 	TEXTITT	"MATS:",50,50
    // asm 000012E2: 	TEXTITT	"FRONT",55,72
    // asm 000012E6: 	TEXTITT	"REAR",55,285
    // 	;MATS
    // asm 000012EA: 	RECT	30,80,130,120,COL_BLACK
    // asm 000012F0: 	RECT	30,140,60,220,COL_BLACK
    // asm 000012F6: 	RECT	100,140,130,220,COL_BLACK
    // asm 000012FC: 	RECT	30,230,130,280,COL_BLACK
    // asm 00001302: 	LDI	COL_BLACK,RC
    // asm 00001303: 	TEXTITT	"OPTOS:",150+50,50
    // asm 00001307: 	TEXTITT	"FRONT",150+55,72
    // asm 0000130B: 	TEXTITT	"REAR",150+55,285
    // 	;LEDS
    // asm 0000130F: 	RECTW	150+30,110,20,20,COL_BLACK	;D1
    // asm 00001315: 	RECTW	150+55,80,20,20,COL_BLACK	;D2
    // asm 0000131B: 	RECTW	150+85,80,20,20,COL_BLACK	;D3
    // asm 00001321: 	RECTW	150+110,110,20,20,COL_BLACK 	;D4
    // asm 00001327: 	RECTW	150+30,210,20,20,COL_BLACK	;L1
    // asm 0000132D: 	RECTW	150+55,230,20,20,COL_BLACK	;L2
    // asm 00001333: 	RECTW	150+85,230,20,20,COL_BLACK	;L3
    // asm 00001339: 	RECTW	150+110,210,20,20,COL_BLACK 	;L4
    // ;	RECT	150+30,80,150+130,120,COL_BLACK
    // ;	RECT	150+30,140,150+60,220,COL_BLACK
    // ;	RECT	150+100,140,150+130,220,COL_BLACK
    // ;	RECT	150+30,230,150+130,280,COL_BLACK
    // asm 0000133F: 	LDI	COL_BLACK,RC
    // asm 00001340: 	TEXTITT	"FAILSAFE SWITCHES:",150+150,50
    // asm 00001344: 	TEXTITT	"FRONT",150+150+61,72
    // asm 00001348: 	TEXTITT	"REAR",150+150+65,285
    // 	;FAIL SAFE
    // asm 0000134C: 	RECT	150+150+60,80,150+150+100,120,COL_BLACK
    // asm 00001352: 	RECT	150+150+30,230,150+150+60,280,COL_BLACK
    // asm 00001358: 	RECT	150+150+100,230,150+150+130,280,COL_BLACK
    // asm 0000135E: 	LDI	COL_BLACK,RC
    // asm 0000135F: 	TEXTITT	"HIT ENTER TO ESCAPE",20,380
    // asm 00001363: 	TEXTITT	"MOTION SAFETY DIAGNOSTICS",30,20
    // asm 00001367: 	TEXTITT	"ERROR MESSAGE:",50,305
    // asm 0000136B: 	TEXTITT	"MOTION STOP:",300,15
    // asm 0000136F: 	IWINDOW	50,320,450,360	;error window
    // asm 00001374: 	IWINDOW	390,5,430,35	;motion
    // asm 00001379: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRAW_MOTION_SAFETY_DIAG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static void CLEAR_MATS(void) {
    // 	;MATS
    // asm 0000137A: 	FILL	32,82,128,118,0
    // asm 00001380: 	FILL	32,142,58,218,0
    // asm 00001386: 	FILL	102,142,128,218,0
    // asm 0000138C: 	FILL	32,232,128,278,0
    // 	;FAIL SAFE
    // asm 00001392: 	FILL	150+150+62,82,150+150+98,118,0
    // asm 00001398: 	FILL	150+150+32,232,150+150+58,278,0
    // asm 0000139E: 	FILL	150+150+102,232,150+150+128,278,0
    // 	;LED (LED)
    // asm 000013A4: 	FILLW	150+32,212,16,16,0	;L1
    // asm 000013AA: 	FILLW	150+57,232,16,16,0	;L2
    // asm 000013B0: 	FILLW	150+87,232,16,16,0	;L3
    // asm 000013B6: 	FILLW	150+112,212,16,16,0 	;L4
    // 	;LED (DETECTOR)
    // asm 000013BC: 	FILLW	150+32,112,16,16,0	;D1
    // asm 000013C2: 	FILLW	150+57,82,16,16,0	;D2
    // asm 000013C8: 	FILLW	150+87,82,16,16,0	;D3
    // asm 000013CE: 	FILLW	150+112,112,16,16,0 	;D4
    // asm 000013D4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_MATS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *diagnostic routine to show motion safety condition
 *
 *
 *
 */

/* asm: OLD_MOTION_CODE	.bss	OLD_MOTION_CODE,1 */
int OLD_MOTION_CODE;

void MOTION_SAFETY_SWITCHES_DIAG(void) {
    // asm 000013D5: 	CALL	SETUP_DIAG_SCREEN
    // asm 000013D6: 	CALL	DRAW_MOTION_SAFETY_DIAG
    // asm 000013D7: 	CALL	MOTION_CHECK_SAFETY_CODES
    // asm 000013D8: 	CLRI	AR2
    // asm 000013D9: 	LDP	@991030h
    // asm 000013DA: 	LDI	@991030h,R0
    // asm 000013DB: 	LDI	*AR2,AR2
    // asm 000013DC: 	SETDP
    // asm 000013DD: 	RS	7,R0
    // asm 000013DE: 	LS	16+7,R0
    // asm 000013DF: 	RS	16,R0
    // asm 000013E0: 	STI	R0,@OLD_MOTION_CODE
MSSD_LP:
    // asm 000013E1: 	NOT	@_newbut,R0
    // asm 000013E2: 	AND	SW_DIAG,R0
    // asm 000013E3: 	RETSNZ
    // asm 000013E4: 	CLRI	AR2
    // asm 000013E5: 	LDP	@991030h
    // asm 000013E6: 	LDI	@991030h,R0
    // asm 000013E7: 	LDI	*AR2,AR2
    // asm 000013E8: 	SETDP
    // asm 000013E9: 	TSTB	080h,R0
    // asm 000013EA: 	BZ	MSOFF
    // asm 000013EB: 	FILL	400,10,420,30,COL_RED
    // asm 000013F1: 	BU	MSFX
MSOFF:
    // asm 000013F2: FILL	400,10,420,30,0
MSFX:
    // asm 000013F8: 	CLRI	AR2
    // asm 000013F9: 	LDP	@991030h
    // asm 000013FA: 	LDI	@991030h,R0
    // asm 000013FB: 	LDI	*AR2,AR2
    // asm 000013FC: 	SETDP
    // asm 000013FD: 	RS	7,R0
    // asm 000013FE: 	LS	16+7,R0
    // asm 000013FF: 	RS	16,R0
    // asm 00001400: 	CMPI	@OLD_MOTION_CODE,R0
    // asm 00001401: 	BEQ	MSSD_LP
    // asm 00001402: 	STI	R0,@OLD_MOTION_CODE
    // ;
    // ;binary debugging aid
    // ;
    // asm 00001403: 	FILL	300,305,500,315,0
    // asm 00001409: 	CLRI	AR2
    // asm 0000140A: 	LDP	@991030h
    // asm 0000140B: 	LDI	@991030h,R2
    // asm 0000140C: 	LDI	*AR2,AR2
    // asm 0000140D: 	SETDP
    // asm 0000140E: 	RS	8,R2
    // asm 0000140F: 	AND	0FFh,R2
    // asm 00001410: 	LDL	IBOIBOT,AR2
    // asm 00001411: 	CALL	HEX2ASC
    // asm 00001412: 	LDI	COL_BLACK,RC
    // asm 00001413: 	LDI	300,R2
    // asm 00001414: 	LDI	305,R3
    // asm 00001415: 	CALL	_outtextxyc
    // ;end dbg
    // ;
    // asm 00001416: 	CALL	MOTION_CHECK_SAFETY_CODES
    // asm 00001417: 	BU	MSSD_LP
    // asm 00001418: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_SAFETY_SWITCHES_DIAG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

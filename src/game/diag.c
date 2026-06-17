#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "dirq.h"
#include "diag.h"
#include "error.h"
#include "port.h"

/*
 * Source module: asm/DIAG.ASM
 */

void WT_START(void);
void WT_NSTART(void);
void KEYPAUSE(void);
void BEEP(void);
void MAKE_WINDOW(void);
void MAKE_IWINDOW(void);
void HARD_LOAD_DIAGPAL(void);
void HIDDEN_DIAG(void);
void ENTER_DIAG_INIT(void);
void ENTER_DIAG(void);
void RETURN_TO_MAINMENU(void);
void TOEXIT(void);
void MONITOR_DIAGNOSTICS(void);
void COLORBARS(void);
void SHOWSWITCH(void);
void ASCII_TO_BINARY(void);
void DISPLAY_SWITCH_STATUS(void);
void DISPLAY_SHIFTER(void);
void DISPPOTS(void);
void SHOW_DIPSWITCH(void);
void SHOW_DIPSWITCH_STATUS(void);
void DISPLAY_DIPSWITCH_STATUS(void);
void RUN_MOTION_MENU(void);
void RUN_ADJUSTMENT_MENU(void);
void PRINT_ADJUSTMENT(void);
void RUN_UTILITIES_MENU(void);
void RUN_DIAG_MENU(void);
void HANDLE_MENU(void);
void XIT_MENU_TO_MAIN(void);
void SETUP_DIAG_SCREEN(void);
void SOUND_BOARD_TEST(void);
void STEERING_WHEEL_TEST(void);
void AMODESND(void);
void BURNIN_TEST(void);
void RUN_COUNTRY(void);
void MAKE_STANDARW(void);
void PRINT_STANDARD(void);
void PRINT_SPYN(void);
void SET_CONTROLS(void);
void SET_VOLUME(void);
void SHOW_VOLUME(void);
void FULL_FACTORY_RESTORE(void);
void FFRSUB(void);
void DEFAULT_ADJUSTMENTS(void);
void RESET_HIGH_SCORES(void);
void CLEAR_GAME_AUDITS(void);
void FREEPLAYONOFF(void);
void FREEGAMEONOFF(void);
void HIGHSCORE_ENTRY(void);
void GIRLS_ONOFF(void);
void CLINTON_ONOFF(void);
void ROADKILL_ONOFF(void);
void MILES_OR_KILO(void);
void CLEAR_CREDITS(void);
void MESSAGE_BOX(void);
void CLEAR_YES_NO(void);
void ASK_YES_NO(void);
void DUAL_ON_OFF(void);
void SET_STEERING_SENSITIVITY(void);
void SET_HIGHSCORE_RESET(void);
void SET_GAME_DIFFICULTY(void);
void GAMEDIFF_CVT(void);
void SET_TIME_TO_START(void);
void TIME_CVT(void);
void SET_CHECKPOINT_BONUS(void);
void CHECKPOINT_BONUS_CVT(void);
void SET_MAX_CREDITS(void);
void SET_MIN_VOL_LEVEL(void);
void SET_MIN_VOL_LEVEL_CVT(void);
void MULTI_VALUE(void);
void MULTI_VALUE_STEP(void);
void RUN_STANDARD_PRICING(void);
void PRICE_YES_NO(void);
void RUN_CUSTOM_PRICING(void);
void FRACTION_YES_NO(void);
void CUSTOM_YES_NO(void);
void SET_SLOT(void);
void PRINT_SLOT(void);
void PRINT_VALUE(void);
void PRINT_VOL(void);
void GETSTR_LENGTH(void);
void PRINT_COINMODE(void);
void PRINT_CUSTOM_SELECTED(void);
void PRINT_TIME(void);
void PRINT_NUMBER(void);
void PRINT_YN_INV(void);
void PRINT_YN(void);
void PRINT_ONOFF_INV(void);
void PRINT_ONOFF(void);
void PRINT_MPH_INV(void);
void PRINT_MPH(void);
void RUN_LAMP_TEST(void);
void LAMPSOFF(void);
void LAMPSALL(void);
void LAMPS(void);
void LAMPV1(void);
void LAMPV2(void);
void LAMPV3(void);
void LAMPLT(void);
void LAMPRT(void);
void LAMPLF(void);
void LAMPRF(void);
void MOTION_CHECK_SAFETY_CODES(void);
void MSC_BOARD_NOTPLUGGED(void);
void MSC_MAT_STEPPED(void);
void MSC_MAT_NOTPLUGGED(void);
void MSC_OPTO_PATH(void);
void MSC_OPTO_LED(void);
void MSC_OPTO_DETECTOR(void);
void MSC_FAILSAFE_OK(void);
void MSC_FAILSAFE_NOTCONNECTED(void);
void DRAW_MOTION_SAFETY_DIAG(void);
void CLEAR_MATS(void);
void MOTION_SAFETY_SWITCHES_DIAG(void);

/* *WAIT FOR ENTER BUTTON ON
 */
void WT_START(void)
{
WT_S1:
    // asm 000005F6: 	LDI	@_newbut,R0
    // asm 000005F7: 	TSTB	SW_DIAG,R0
    // asm 000005F8: 	BNZ	WT_S1
    // asm 000005F9: 	RETS
    // *WAIT FOR ENTER BUTTON OFF
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WT_START", 0, 0);
    UNIMPL();
}

void WT_NSTART(void)
{
WT_NS1:
    // asm 000005FA: 	LDI	@_newbut,R0
    // asm 000005FB: 	TSTB	SW_DIAG,R0
    // asm 000005FC: 	BZ	WT_NS1
    // asm 000005FD: 	RETS
    // *WAIT FOR ENTER ON THEN OFF
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WT_NSTART", 0, 0);
    UNIMPL();
}

void KEYPAUSE(void)
{
    // asm 000005FE: 	CALL	WT_START
    // asm 000005FF: 	B	WT_NSTART
    // *MAKE A BEEP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KEYPAUSE", 0, 0);
    UNIMPL();
}

void BEEP(void)
{
    // asm 00000600: 	LDI	980,AR2
    // asm 00000601: 	BR	SENDSND
    // ;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BEEP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
#define MW_Y2 0
#define MW_X2 1
#define MW_Y1 2
#define MW_X1 3

void MAKE_WINDOW(void)
{
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

void MAKE_IWINDOW(void)
{
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

const char *inst1 = "USE UP/DOWN TO SELECT FUNCTION";
const char *inst2 = "ENTER TO ACTIVATE";
const char *title = "CRUISIN USA   DIAGNOSTICS";
const char *toexit = "HIT ENTER TO EXIT";
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
int DIAGPAL;

/* *----------------------------------------------------------------------------
 */
void HARD_LOAD_DIAGPAL(void)
{
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

/* asm: SOFT_AUDIT	.bss	SOFT_AUDIT,1 */
int SOFT_AUDIT;

void HIDDEN_DIAG(void)
{
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

/* *----------------------------------------------------------------------------
*
*
 */
void ENTER_DIAG_INIT(void)
{
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

/* *----------------------------------------------------------------------------
 */
void ENTER_DIAG(void)
{
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

void RETURN_TO_MAINMENU(void)
{
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

/* *----------------------------------------------------------------------------
 */
void TOEXIT(void)
{
    // asm 000006A8: 	LDI	DEFAULT_COLOR,RC
    // asm 000006A9: 	TEXTIT	toexit,180,371
    // asm 000006AD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOEXIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
 */
/* asm: FILSIZI	.word	3FFFFH */
int FILSIZI = 0x3FFFF;
/* asm: REDV	.word	30003h		;these values point into fixed palette */
int REDV = 0x30003;
/* asm: GREENV	.word	40004h */
int GREENV = 0x40004;
/* asm: BLUEV	.word	90009h */
int BLUEV = 0x90009;
/* asm: BLACKV	.word	80008h */
int BLACKV = 0x80008;

void MONITOR_DIAGNOSTICS(void)
{
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

/* *----------------------------------------------------------------------------
 */
void COLORBARS(void)
{
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
    // asm 0000070F: BLUELP	ADDI	2,R0
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
    // asm 00000718: LDI	15,AR6
    // asm 00000719: BLOCKLP	CALL	_pixel
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

/* *----------------------------------------------------------------------------
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

void SHOWSWITCH(void)
{
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
    // asm 0000087E: BBD5	CALL	WAIT_FOR_VBLANK		;wait for vblk
    // asm 0000087F: 	DBU	AR5,BBD5
    // asm 00000880: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOWSWITCH", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*BOTTOM EIGHT BITS ONLY
*
*
*PARAMETER
*	AR2	STRING
*	R2	VALUE
*
*
 */
void ASCII_TO_BINARY(void)
{
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

/* *----------------------------------------------------------------------------
 */
/* asm: _onebut	.bss	_onebut,1 */
int _onebut;

void DISPLAY_SWITCH_STATUS(void)
{
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

/* *----------------------------------------------------------------------------
 */
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
int SWITCHES_DIAG[] = {
    SW_VIEW0, 180, 130,
    SW_VIEW1, 200, 130,
    SW_VIEW2, 220, 130,
    SW_START, 240, 130,
    SW_COIN1, 260, 130,
    SW_COIN2, 280, 130,
    SW_COIN3, 300, 130,
    SW_COIN4, 320, 130,
    SW_RADIO, 340, 130,
    SW_DIAG, 275, 330,
    SW_VOLPLUS, 295, 330,
    SW_VOLMINUS, 315, 330,
    SW_COINSRV, 335, 330,
    -1,
};
/* *----------------------------------------------------------------------------
 */
/* asm: LASTSHIFT	.bss	LASTSHIFT,1 */
int LASTSHIFT;

void DISPLAY_SHIFTER(void)
{
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

/* *----------------------------------------------------------------------------
 */
void DISPPOTS(void)
{
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
    // asm 00000910: NOPLOTpot0
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
    // asm 00000921: NOPLOTpot1
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
    // asm 00000932: NOPLOTpot2
    // asm 00000932: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPPOTS", 0, 0);
    UNIMPL();
}

const char *SW1T = "DIP SW2";
const char *SW2T = "DIP SW3";
/* asm: SW1BANK	.bss	SW1BANK,3 */
int SW1BANK[3];
/* asm: SW2BANK	.bss	SW2BANK,3 */
int SW2BANK[3];
/* asm: LASTDIPPLOT	.bss	LASTDIPPLOT,1 */
int LASTDIPPLOT;

void SHOW_DIPSWITCH(void)
{
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

/* *----------------------------------------------------------------------------
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
/* asm: ISON	.word	ONTEXT */
const char *ISON = ONTEXT;
/* asm: ISOFF	.word	OFFTEXT */
const char *ISOFF = OFFTEXT;

/* *
 */
void SHOW_DIPSWITCH_STATUS(void)
{
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

/* *----------------------------------------------------------------------------
*
*
*
*
 */
void DISPLAY_DIPSWITCH_STATUS(void)
{
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

/* asm: DMOD1	SPTR	"MASTER" */
const char *DMOD1 = "MASTER";
/* asm: DMOD2	SPTR	"SLAVE" */
const char *DMOD2 = "SLAVE";
/* *----------------------------------------------------------------------------
 */
/* asm: MOMT	SPTR	"MOTION MENU" */
const char *MOMT = "MOTION MENU";

void RUN_MOTION_MENU(void)
{
    // asm: 	LDL	MOTION_MENU,AR5
    // asm: 	LDI	MOTION_MENU_LEN,AR6
    // asm: 	LDI	@MOMT,AR7
    // asm: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_MOTION_MENU", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: AMT	SPTR	"ADJUSTMENT MENU" */
const char *AMT = "ADJUSTMENT MENU";

void RUN_ADJUSTMENT_MENU(void)
{
    // asm 00000A38: 	LDL	ADJUSTMENT_MENU,AR5
    // asm 00000A39: 	LDI	ADJUSTMENT_MENU_LEN,AR6
    // asm: 	LDI	@AMT,AR7
    // asm: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_ADJUSTMENT_MENU", 0, 0);
    UNIMPL();
}

/* asm: ADJTAB */
/* asm: 	.word	ADJ_COINMODE,PRINT_COINMODE */
/* asm: 	.word	ADJ_STANDARD_PRICING,PRINT_CUSTOM_SELECTED */
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
int ADJTAB[] = {
    ADJ_COINMODE, PRINT_COINMODE,
    ADJ_STANDARD_PRICING, PRINT_CUSTOM_SELECTED,
    ADJ_FREE_PLAY, PRINT_ONOFF,
    ADJ_FREEGAME, PRINT_ONOFF,
    ADJ_TIME_TO_START, PRINT_TIME,
    ADJ_CHECKPOINT_BONUS, PRINT_NUMBER,
    ADJ_ATTRACT_MODE_SOUND, PRINT_ONOFF,
    ADJ_HIGH_SCORE_ENTRY, PRINT_ONOFF,
    ADJ_MIN_VOL_LEVEL, PRINT_VOL,
    ADJ_MPHORKPM, PRINT_MPH,
    ADJ_ROADKILL, PRINT_ONOFF,
    ADJ_CLINTON, PRINT_ONOFF,
    ADJ_GIRLS, PRINT_ONOFF,
    ADJ_HIGHSCORE_RESET, PRINT_NUMBER,
    ADJ_DIFFICULTY, PRINT_NUMBER,
    ADJ_MAX_CREDITS, PRINT_NUMBER,
    ADJ_STEERING_SENSITIVITY, PRINT_NUMBER,
    0, 0,
};

void PRINT_ADJUSTMENT(void)
{
    // asm: 	PUSH	AR4
    // asm: 	LDI	@ADJTABI,AR4
    // asm: 	CALL	PRINT_VALUE
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_ADJUSTMENT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: UMT	SPTR	"UTILITIES MENU" */
const char *UMT = "UTILITIES MENU";

void RUN_UTILITIES_MENU(void)
{
    // asm 00000A44: 	LDL	UTILITIES_MENU,AR5
    // asm: 	LDI	UTILITIES_MENU_LEN,AR6
    // asm: 	LDI	@UMT,AR7
    // asm: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_UTILITIES_MENU", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: MMT	SPTR	"MAIN MENU" */
const char *MMT = "MAIN MENU";
/* asm: DMT	SPTR	"DIAGNOSTIC MENU" */
const char *DMT = "DIAGNOSTIC MENU";

void RUN_DIAG_MENU(void)
{
    // asm 00000A49: 	LDL	DIAGNOSTIC_MENU,AR5
    // asm: 	LDI	DIAGNOSTIC_MENU_LEN,AR6
    // asm: 	LDI	@DMT,AR7
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_DIAG_MENU", 0, 0);
    UNIMPL();
}

/* asm: MENU_SCROLL	.bss	MENU_SCROLL,1 */
int MENU_SCROLL;

void HANDLE_MENU(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@MENU_SCROLL
    // asm: 	CLRI	R6			;MENU INDEX
HMER:
    // asm 00000A4F: 	PUSH	R6
    // asm 00000A50: 	LDI	@DIAGPAL+1,R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	PUSH	AR7
    // asm 00000A52: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	LDI	COL_VDGREY,RC
    // asm 00000A55: 	TEXTIT	title,30,20
    // asm: 	TEXTIT	inst1,20,260
    // asm 00000A56: 	TEXTIT	inst2,20,270
    // asm: 	LDI	*-AR5,R2		;Routine that prints the value
    // asm 00000A57: 	BEQ	HMSTAND
    // asm 00000A58: 	LDI	*-AR5(2),R2
    // asm 00000A5C: 	BEQ	HMSTAND
    // asm 00000A60: 	CALLU	R2
    // asm: 	BR	HMNOTSTAND
HMSTAND:
    // asm 00000A65: 	IWINDOW	20,45,280,250
HMNOTSTAND:
    // asm 00000A67: 	LDI	30,R2
    // asm 00000A68: 	LDI	35,R3
    // asm 00000A69: 	LDI	COL_VDGREY,RC
    // asm: 	POP	AR7
    // asm 00000A6A: 	LDI	AR7,AR2
    // asm: 	CALL	_outtextxyc
    // asm 00000A70: 	LDI	10000,R7		;wait_mac counter
    // asm 00000A71: 	MPYI	55,R7
    // asm 00000A72: 	POP	R6
HANMENL:
    // asm 00000A75: 	LDI	@MENU_SCROLL,R0
    // ;	CLRI	R0
    // ;	LDI	AR5,AR4
    // asm: 	LDI	2,AR4			;OFFSET AR4 to MENU_SCROLL
    // asm: 	MPYI	R0,AR4
    // asm: 	ADDI	AR5,AR4
    // asm: 	LDI	50,R3			;Y POSITION OF TEXT
    // asm 00000A7A: 	PUSH	AR7
    // asm: 	LDI	AR6,AR7
    // asm 00000A7C: 	SUBI	@MENU_SCROLL,AR7
    // asm: 	CMPI	20,AR7
    // asm: 	LDIGT	20,AR7
    // asm: 	DEC	AR7
    // asm 00000A7E: HANMENL_PLOT
    // asm 00000A7F: 	LDI	COL_VDGREY,RC
    // asm 00000A80: 	CMPI	R0,R6
    // asm 00000A81: 	LDIEQ	COL_RED,RC
    // asm 00000A82: 	LDI	*AR4++(2),AR2
    // asm: 	LDI	30,R2
    // asm 00000A83: 	CALL	_outtextxyc
    // asm 00000A85: 	LDI	*-AR5,R5		;Routine that prints the value
    // asm: 	CALLNE	R5
    // asm 00000A87: 	ADDI	10,R3
    // asm 00000A88: 	INC	R0
    // asm: 	DBU	AR7,HANMENL_PLOT
    // asm 00000A89: 	POP	AR7
    // asm 00000A8C: 	LDI	@_newbut,R0		;old system method
    // asm 00000A8D: 	LDI	R0,R1
    // asm 00000A8E: 	TSTB    SW_COINSRV,R1
    // asm: 	BNZ	JAJA5
    // ;this locks on the main menu.
    // ;fix later
    // asm 00000A91: 	LDL	MAIN_MENU,AR0
    // asm 00000A92: 	CMPI	AR0,AR5
    // asm: 	BEQ	JAJA5
    // asm: 	LDI	982,AR2
    // asm: 	CALL	SENDSND
    // asm 00000A93: 	PUSH	AR5
    // asm 00000A94: 	PUSH	AR6
    // asm 00000A95: 	PUSH	AR7
    // asm: 	PUSH	R6
    // asm 00000A96: 	CALL	XIT_MENU_TO_MAIN
    // ;
    // ;	CALLZ	ESCAPE_TO_MAIN
JAJA5:
    // asm 00000A9C: 	TSTB    SW_DIAG,R1
    // asm: 	BZ	HM_EXECUTE
    // asm: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm: 	BNZ	HM_NOTUP
    // asm 00000A9D: 	LDI	980,AR2
    // asm 00000A9E: 	CALL	SENDSND
    // asm 00000A9F: 	ADDI	1,R6
    // asm 00000AA0: 	CMPI	AR6,R6
    // asm: 	BLT	HM_WT
    // asm 00000AA2: 	LDI	AR6,R6
    // asm: 	DEC	R6
    // asm 00000AA3: 	B	HM_WT
HM_NOTUP:
    // asm: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm 00000AA6: 	BNZ	HM_NOTDOWN
    // asm 00000AA8: 	LDI	981,AR2
    // asm: 	CALL	SENDSND
    // asm 00000AA9: 	SUBI	1,R6
    // asm 00000AAA: 	LDILT	0,R6
HM_WT:
    // ;	WAIT_MAC
    // asm 00000AAC: 	LDI	R7,R1
wta:
    // asm 00000AAD: 	DEC	R1
    // asm 00000AAE: 	BLT	wtta
    // asm: 	LDI	@_newbut,R0
    // asm 00000AAF: 	AND	SW_VOLPLUS|SW_VOLMINUS,R0
    // asm: 	CMPI	SW_VOLPLUS|SW_VOLMINUS,R0
    // asm 00000AB0: 	BNE	wta
    // asm: 	LDI	10000,R7
    // asm 00000AB2: 	MPYI	55,R7
    // asm 00000AB3: 	BU	J55
wtta:
    // asm 00000AB5: 	LDI	5000,R7
    // asm: 	MPYI	55,R7
J55:
HM_NOTDOWN:
    // asm 00000ABA: 	CMPI	0,R6
    // asm: 	BEQ	NO_SCROLL
    // asm: 	LDI	0,R2
    // asm: 	LDI	@MENU_SCROLL,R0
    // asm: 	LDI	R0,R1
    // asm: 	ADDI	20,R1
    // asm 00000ABB: 	CMPI	R0,R6
    // asm 00000ABC: 	BLT	GO_SCROLL
    // asm 00000ABD: 	CMPI	R1,AR6
    // asm 00000ABE: 	BLE	NO_SCROLL
GO_SCROLL:
    // asm 00000AC0: 	CMPI	R0,R6
    // asm 00000AC1: 	LDILT	-10,R2
    // asm 00000AC2: 	CMPI	R1,R6
    // asm 00000AC3: 	LDIGE	10,R2
    // asm 00000AC4: 	ADDI	R2,R0
    // asm: 	LDIN	0,R0
    // asm 00000AC5: 	STI	R0,@MENU_SCROLL
    // asm 00000AC6: 	CMPI	0,R2
    // asm 00000AC7: 	BNE	HMER			;REDRAW entir screen if scrolled
NO_SCROLL:
    // asm 00000ACA: 	B	HANMENL
HM_EXECUTE:
    // asm 00000ACD: 	LDI	982,AR2
    // asm: 	CALL	SENDSND
    // asm 00000ACE: 	LDI	R6,AR0
    // asm: 	MPYI	2,AR0
    // asm: 	ADDI	AR5,AR0
    // asm 00000ACF: 	LDI	*+AR0(1),R0
    // asm 00000AD0: 	BZ	HANMENL
    // asm: 	PUSH	AR5
    // asm 00000AD1: 	PUSH	AR6
    // asm 00000AD2: 	PUSH	AR7
    // asm 00000AD3: 	PUSH	R6
    // asm 00000AD4: 	CALLU	R0
    // asm 00000AD5: 	POP	R6
    // asm 00000AD6: 	POP	AR7
    // asm 00000AD7: 	POP	AR6
    // asm 00000AD8: 	POP	AR5
    // asm 00000AD9: 	B	HMER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HANDLE_MENU", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
;ESCAPE_TO_MAIN:
;	LDI	982,AR2
;	CALL	SENDSND
;	POP	R0
;	RETS
 */
void XIT_MENU_TO_MAIN(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@MENU_SCROLL
    // asm: 	CALL	LAMPSOFF
    // asm: 	POP	R0	;return address
    // asm 00000AE0: 	POP	R6
    // asm 00000AE1: 	POP	AR7
    // asm: 	POP	AR6
    // asm 00000AE2: 	POP	AR5
    // asm: 	RETS
    // ;	BR	RETURN_TO_MAINMENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "XIT_MENU_TO_MAIN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SETUP_DIAG_SCREEN(void)
{
    // asm: 	CALL	CLRSCRN
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETUP_DIAG_SCREEN", 0, 0);
    UNIMPL();
}

/* asm: MUSIC_TAB */
/* asm: SNDMAC	"REDLINE SHUFFLE",SHUFFLE_DRIV */
/* asm: SNDMAC	"HOUSE SPECIAL",DISCODUCK */
/* asm: SNDMAC	"DEADWOOD RIDE",SPAGETTI */
int MUSIC_TAB;
/* asm: EFFECTS_TAB */
/* asm: SNDMAC	"GIRL SPEECH",CHOOSEUCAR */
/* asm: SNDMAC	"TRUCK HORN",TRUCKHORN_BLAST */
/* asm: SNDMAC	"EXPLOSION 1",EXP1 */
/* asm: SNDMAC	"COLLISION",SCOLLF */
/* asm: SNDMAC	"CAR PASS",CARPASS */
int EFFECTS_TAB;

void SOUND_BOARD_TEST(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	CLRI	R5	;TUNE INDEX
    // asm: 	CLRI	R6	;SNDFX INDEX
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"SOUND BOARD TEST",50,20
    // asm: 	TEXTITT	"ENTER  TO EXIT",20,100
    // asm: 	TEXTITT	"UP     TO CHANGE MUSIC",20,110
    // asm: 	TEXTITT	"DOWN   TO TRIGGER SOUND EFFECT",20,120
    // asm: 	LDL	MUSIC_TAB,AR5
    // asm 00000B02: 	LDL	EFFECTS_TAB,AR6
    // asm 00000B03: 	LDI	-1,R5
    // asm 00000B04: 	LDI	-1,R6
SBT_UPDT:
    // asm 00000B06: 	CMPI	AR5,R5
    // asm 00000B0A: 	BEQ	NUPDT
    // asm 00000B12: 	LDI	AR5,R5
    // asm: 	FILL	20,200,200,220,0
    // asm 00000B16: 	LDI	COL_VDGREY,RC
    // asm 00000B17: 	TEXTITT	"TUNE",20,200
    // asm: 	LDI	*AR5,AR2
    // asm 00000B18: 	LDI	30,R2
    // asm 00000B19: 	LDI	210,R3
    // asm: 	CALL	_outtextxyc
    // asm 00000B1A: 	LDI	*+AR5,AR2
    // asm: 	CALL	HARDSND
NUPDT:
    // asm 00000B23: 	CMPI	AR6,R6
    // asm 00000B24: 	BEQ	NUPDE
    // asm 00000B29: 	LDI	AR6,R6
    // asm 00000B2A: 	FILL	20,250,200,270,0
    // asm 00000B2B: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"SOUND EFFECT",20,250
    // asm 00000B2C: 	LDI	*AR6,AR2
    // asm: 	LDI	30,R2
    // asm 00000B2D: 	LDI	260,R3
    // asm: 	CALL	_outtextxyc
    // asm 00000B2E: 	LDI	*+AR6,AR2
    // asm: 	CALL	HARDSND
NUPDE:
SBT_LP:
    // asm 00000B3D: 	NOT	@_newbut,R0
    // asm 00000B3E: 	AND	SW_DIAG,R0
    // asm 00000B3F: 	BZ	NOXIT
    // asm 00000B40: 	CALL	SILENT
    // asm: 	RETS
NOXIT:
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_VOLPLUS,R0
    // asm: 	BZ	NONEXTT
SBTLP2:
    // asm 00000B42: CALL	WAIT_FOR_VBLANK
    // asm 00000B43: 	NOT	@_newbut,R0
    // asm 00000B44: 	AND	SW_VOLPLUS,R0
    // asm: 	BNZ	SBTLP2
    // asm 00000B46: 	ADDI	2,AR5
    // asm: 	LDI	*AR5,R0
    // asm: 	BNN	SBT_UPDT
    // asm 00000B47: 	LDL	MUSIC_TAB,AR5
    // asm 00000B48: 	BU	SBT_UPDT
NONEXTT:
    // asm 00000B4B: 	NOT	@_newbut,R0
    // asm 00000B4C: 	AND	SW_VOLMINUS,R0
    // asm 00000B4D: 	BZ	NONEXTE
SBTLP1:
    // asm 00000B4E: CALL	WAIT_FOR_VBLANK
    // asm 00000B4F: 	NOT	@_newbut,R0
    // asm 00000B50: 	AND	SW_VOLMINUS,R0
    // asm 00000B51: 	BNZ	SBTLP1
    // asm: 	ADDI	2,AR6
    // asm: 	LDI	*AR6,R0
    // asm 00000B53: 	BNN	SBT_UPDT
    // asm 00000B54: 	LDL	EFFECTS_TAB,AR6
    // asm 00000B55: 	BU	SBT_UPDT
NONEXTE:
    // asm 00000B58: 	LDI	7FFFh,R0
    // asm 00000B59: 	MPYI	10,R0
    // asm: 	RPTS	R0
    // asm: 	NOP
    // asm 00000B5A: 	CALL	WAIT_FOR_VBLANK
    // asm 00000B5B: 	BU	SBT_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SOUND_BOARD_TEST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void STEERING_WHEEL_TEST(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm 00000B5F: 	LDI	COL_VDGREY,RC
    // asm 00000B60: 	TEXTITT	"STEERING WHEEL TEST",20,50
    // asm 00000B61: 	TEXTITT	"MOVING WHEEL TO LEFT",20,100
    // asm 00000B63: 	CLRI	R6
    // *	R0	BYTE TO SEND TO WHEEL
    // asm: 	LDI	127,AR5
    // asm 00000B65: SWTLP1
    // asm: 	INC	R6
    // asm 00000B65: 	CMPI	127,R6
    // asm 00000B66: 	LDIGT	127,R6
    // asm 00000B6B: 	LDI	R6,R0
    // asm: 	OR	080h,R0
    // asm 00000B6F: 	CALL	TOWHEEL
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	DBU	AR5,SWTLP1
    // asm: 	LDI	COL_VDGREY,RC
    // asm 00000B71: 	TEXTITT	"HIT ENTER TO CONTINUE",20,120
    // asm 00000B72: 	CALL	WT_NSTART
    // asm 00000B73: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm 00000B76: 	LDI	COL_VDGREY,RC
    // asm 00000B77: 	TEXTITT	"MOVING WHEEL TO RIGHT",20,140
    // asm: 	CLRI	R6
    // asm: 	LDI	127,AR5
    // asm 00000B79: SWTLP3
    // asm 00000B7A: 	INC	R6
    // asm 00000B7E: 	CMPI	127,R6
    // asm 00000B7F: 	LDIGT	127,R6
    // asm: 	LDI	R6,R0
    // asm: 	CALL	TOWHEEL
    // asm 00000B81: 	CALL	WAIT_FOR_VBLANK
    // asm 00000B82: 	DBU	AR5,SWTLP3
    // asm 00000B87: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"HIT ENTER TO CONTINUE",20,160
    // asm 00000B88: 	CALL	WT_NSTART
    // asm 00000B89: 	CALL	WT_START
    // asm 00000B8A: 	CALL	BEEP
    // asm 00000B8B: 	CLRI	R0
    // asm 00000B8C: 	CALL	TOWHEEL
    // asm 00000B8E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "STEERING_WHEEL_TEST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: AMODES	SPTR	"ATTRACT MODE SOUNDS" */
const char *AMODES = "ATTRACT MODE SOUNDS";

void AMODESND(void)
{
    // asm: 	READAUD	ADJ_ATTRACT_MODE_SOUND
    // asm 00000B97: 	LDI	R0,R5
    // asm: 	LDI	@AMODES,AR2
    // asm 00000B99: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_ATTRACT_MODE_SOUND
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AMODESND", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void BURNIN_TEST(void)
{
    // asm 00000B9E: 	CALL	TEST_CHIPS
    // asm 00000BA0: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm: 	DINT
    // asm 00000BA1:         LDP	@FASTSTKI
    // asm 00000BA2:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm: 	LDI	AUD_SUCCESSFULL_BURNINS,AR2
    // asm: 	CALL	AUDIT_INC
    // 	;IGNORE GLITCH FIX
    // asm: 	LDP	@SWITCH1
    // asm 00000BA5: 	LDI	@SWITCH1,R0
    // asm: 	SETDP
    // asm 00000BAA: 	TSTB	SW_DIAG,R0
    // asm 00000BB0: 	BNZ	BURNIN_TEST
    // asm: 	BU	ENTER_DIAG
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BURNIN_TEST", 0, 0);
    UNIMPL();
}

#define STANDARD_PRICING_MENU_LEN 16
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
int USA_MENU[] = {
    MSGC_USA1, PRICE_YES_NO,
    MSGC_USA2, PRICE_YES_NO,
    MSGC_USA3, PRICE_YES_NO,
    MSGC_USA4, PRICE_YES_NO,
    MSGC_USA5, PRICE_YES_NO,
    MSGC_USA6, PRICE_YES_NO,
    MSGC_USA7, PRICE_YES_NO,
    MSGC_USA8, PRICE_YES_NO,
    MSGC_USA9, PRICE_YES_NO,
    MSGC_USA10, PRICE_YES_NO,
    MSGC_USA11, PRICE_YES_NO,
    MSGC_USA12, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
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
int GERMAN_MENU[] = {
    MSGC_GERMAN1, PRICE_YES_NO,
    MSGC_GERMAN2, PRICE_YES_NO,
    MSGC_GERMAN3, PRICE_YES_NO,
    MSGC_GERMAN4, PRICE_YES_NO,
    MSGC_GERMAN5, PRICE_YES_NO,
    MSGC_GERMAN6, PRICE_YES_NO,
    MSGC_GERMAN7, PRICE_YES_NO,
    MSGC_GERMAN8, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
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
int FRENCH_MENU[] = {
    MSGC_FRENCH1, PRICE_YES_NO,
    MSGC_FRENCH2, PRICE_YES_NO,
    MSGC_FRENCH3, PRICE_YES_NO,
    MSGC_FRENCH4, PRICE_YES_NO,
    MSGC_FRENCH5, PRICE_YES_NO,
    MSGC_FRENCH6, PRICE_YES_NO,
    MSGC_FRENCH7, PRICE_YES_NO,
    MSGC_FRENCH8, PRICE_YES_NO,
    MSGC_FRENCH9, PRICE_YES_NO,
    MSGC_FRENCH10, PRICE_YES_NO,
    MSGC_FRENCH11, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define CANADA_MENU_LEN 4
/* asm: CANADA_MENU */
/* asm: 	.word	MSGC_CANADA1,PRICE_YES_NO */
/* asm: 	.word	MSGC_CANADA2,PRICE_YES_NO */
/* asm: 	.word	MSGC_CANADA3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int CANADA_MENU[] = {
    MSGC_CANADA1, PRICE_YES_NO,
    MSGC_CANADA2, PRICE_YES_NO,
    MSGC_CANADA3, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define SWISS_MENU_LEN 4
/* asm: SWISS_MENU */
/* asm: 	.word	MSGC_SWISS1,PRICE_YES_NO */
/* asm: 	.word	MSGC_SWISS2,PRICE_YES_NO */
/* asm: 	.word	MSGC_SWISS3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int SWISS_MENU[] = {
    MSGC_SWISS1, PRICE_YES_NO,
    MSGC_SWISS2, PRICE_YES_NO,
    MSGC_SWISS3, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define ITALY_MENU_LEN 4
/* asm: ITALY_MENU */
/* asm: 	.word	MSGC_ITALY1,PRICE_YES_NO */
/* asm: 	.word	MSGC_ITALY2,PRICE_YES_NO */
/* asm: 	.word	MSGC_ITALY3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int ITALY_MENU[] = {
    MSGC_ITALY1, PRICE_YES_NO,
    MSGC_ITALY2, PRICE_YES_NO,
    MSGC_ITALY3, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define UK_MENU_LEN 4
/* asm: UK_MENU */
/* asm: 	.word	MSGC_UK1,PRICE_YES_NO */
/* asm: 	.word	MSGC_UK2,PRICE_YES_NO */
/* asm: 	.word	MSGC_UK3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int UK_MENU[] = {
    MSGC_UK1, PRICE_YES_NO,
    MSGC_UK2, PRICE_YES_NO,
    MSGC_UK3, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
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
int SPAIN_MENU[] = {
    MSGC_SPAIN1, PRICE_YES_NO,
    MSGC_SPAIN2, PRICE_YES_NO,
    MSGC_SPAIN3, PRICE_YES_NO,
    MSGC_SPAIN4, PRICE_YES_NO,
    MSGC_SPAIN5, PRICE_YES_NO,
    MSGC_SPAIN6, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define AUSTRALIA_MENU_LEN 5
/* asm: AUSTRALIA_MENU */
/* asm: 	.word	MSGC_AUSTRALIA1,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRALIA2,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRALIA3,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRALIA4,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int AUSTRALIA_MENU[] = {
    MSGC_AUSTRALIA1, PRICE_YES_NO,
    MSGC_AUSTRALIA2, PRICE_YES_NO,
    MSGC_AUSTRALIA3, PRICE_YES_NO,
    MSGC_AUSTRALIA4, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define JAPAN_MENU_LEN 4
/* asm: JAPAN_MENU */
/* asm: 	.word	MSGC_JAPAN1,PRICE_YES_NO */
/* asm: 	.word	MSGC_JAPAN2,PRICE_YES_NO */
/* asm: 	.word	MSGC_JAPAN3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int JAPAN_MENU[] = {
    MSGC_JAPAN1, PRICE_YES_NO,
    MSGC_JAPAN2, PRICE_YES_NO,
    MSGC_JAPAN3, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define TAIWAN_MENU_LEN 4
/* asm: TAIWAN_MENU */
/* asm: 	.word	MSGC_TAIWAN1,PRICE_YES_NO */
/* asm: 	.word	MSGC_TAIWAN2,PRICE_YES_NO */
/* asm: 	.word	MSGC_TAIWAN3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int TAIWAN_MENU[] = {
    MSGC_TAIWAN1, PRICE_YES_NO,
    MSGC_TAIWAN2, PRICE_YES_NO,
    MSGC_TAIWAN3, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define AUSTRIA_MENU_LEN 5
/* asm: AUSTRIA_MENU */
/* asm: 	.word	MSGC_AUSTRIA1,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRIA2,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRIA3,PRICE_YES_NO */
/* asm: 	.word	MSGC_AUSTRIA4,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int AUSTRIA_MENU[] = {
    MSGC_AUSTRIA1, PRICE_YES_NO,
    MSGC_AUSTRIA2, PRICE_YES_NO,
    MSGC_AUSTRIA3, PRICE_YES_NO,
    MSGC_AUSTRIA4, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
#define BELGIUM_MENU_LEN 4
/* asm: BELGIUM_MENU */
/* asm: 	.word	MSGC_BELGIUM1,PRICE_YES_NO */
/* asm: 	.word	MSGC_BELGIUM2,PRICE_YES_NO */
/* asm: 	.word	MSGC_BELGIUM3,PRICE_YES_NO */
/* asm: 	.word	ETMAIN,XIT_MENU_TO_MAIN */
int BELGIUM_MENU[] = {
    MSGC_BELGIUM1, PRICE_YES_NO,
    MSGC_BELGIUM2, PRICE_YES_NO,
    MSGC_BELGIUM3, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
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
int OTHER_MENU[] = {
    MSGC_SWEDEN1, PRICE_YES_NO,
    MSGC_FINLAND1, PRICE_YES_NO,
    MSGC_NETHERLAND1, PRICE_YES_NO,
    MSGC_NETHERLAND2, PRICE_YES_NO,
    MSGC_NORWAY1, PRICE_YES_NO,
    MSGC_DENMARK1, PRICE_YES_NO,
    MSGC_HUNGARY1, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
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
int GENERAL_MENU[] = {
    MSGC_GENERAL1, PRICE_YES_NO,
    MSGC_GENERAL2, PRICE_YES_NO,
    MSGC_GENERAL3, PRICE_YES_NO,
    MSGC_GENERAL4, PRICE_YES_NO,
    MSGC_GENERAL5, PRICE_YES_NO,
    MSGC_GENERAL6, PRICE_YES_NO,
    MSGC_GENERAL7, PRICE_YES_NO,
    MSGC_GENERAL8, PRICE_YES_NO,
    ETMAIN, XIT_MENU_TO_MAIN,
};
const char *ETMAIN = "EXIT TO MAIN";
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
int COUNTRY_MENUTAB[] = {
    USA_MENU, USA_MENU_LEN,
    GERMAN_MENU, GERMAN_MENU_LEN,
    FRENCH_MENU, FRENCH_MENU_LEN,
    CANADA_MENU, CANADA_MENU_LEN,
    SWISS_MENU, SWISS_MENU_LEN,
    ITALY_MENU, ITALY_MENU_LEN,
    UK_MENU, UK_MENU_LEN,
    SPAIN_MENU, SPAIN_MENU_LEN,
    AUSTRALIA_MENU, AUSTRALIA_MENU_LEN,
    JAPAN_MENU, JAPAN_MENU_LEN,
    TAIWAN_MENU, TAIWAN_MENU_LEN,
    AUSTRIA_MENU, AUSTRIA_MENU_LEN,
    BELGIUM_MENU, BELGIUM_MENU_LEN,
    OTHER_MENU, OTHER_MENU_LEN,
    GENERAL_MENU, GENERAL_MENU_LEN,
};
/* asm: COUNTRY	.bss	COUNTRY,1 */
int COUNTRY;

void RUN_COUNTRY(void)
{
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@PSLAST		;JUMP START PRINT STANDARD
    // asm: 	LDI	R6,AR5
    // asm: 	ADDI	@COUNTRY_TABI,AR5
    // asm: 	LDI	*AR5,R0
    // asm: 	STI	R0,@COUNTRY
    // asm: 	LDI	R6,R0
    // asm: 	MPYI	2,R0
    // asm: 	LDI	@COUNTRY_MENUTABI,AR5
    // asm: 	ADDI	R0,AR5
    // asm: 	LDI	*+AR5,AR6			;MENU LENGTH
    // asm 00000BDC: 	LDI	*AR5,AR5			;MENU DATA
    // asm 00000BDD: 	LDI	@SPM,AR7
    // asm: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_COUNTRY", 0, 0);
    UNIMPL();
}

void MAKE_STANDARW(void)
{
    // asm 00000BE4: 	IWINDOW	20,45,250,250
    // asm 00000BE5: 	IWINDOW	262,45,500,250
    // asm 00000BE6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_STANDARW", 0, 0);
    UNIMPL();
}

#define PSSTARTY 100
/* asm: PSLAST	.bss	PSLAST,1 */
int PSLAST;

void PRINT_STANDARD(void)
{
    // asm: 	CMPI	R0,R6
    // asm: 	RETSNE
    // asm: 	CMPI	@PSLAST,R0
    // asm: 	RETSEQ
    // asm: 	STI	R0,@PSLAST
    // asm 00000BEF: 	CALL	PUSHALL
    // asm: 	PUSH	RC
    // asm: 	FILL	270,PSSTARTY+120,498,PSSTARTY+129,0
    // asm: 	FILL	490-24,PSSTARTY,498,PSSTARTY+149,0
    // asm: 	FILL	270,PSSTARTY-40,498,PSSTARTY-10,0
    // asm 00000BF5: 	CMPI	0,AR7
    // asm 00000BF6: 	BLE	PS_SKIPMENU
    // asm 00000BF8: 	LDI	COL_RED,RC
    // asm: 	LDI	R0,AR4
    // asm 00000BFA: 	ADDI	@COUNTRY,AR4
    // asm: 	MPYI	COIN_ENTRY_SIZE,AR4
    // asm 00000BFB: 	ADDI	@COIN_TABLEI,AR4
    // asm 00000BFC: 	ADDI	3,AR4
    // asm 00000C08: 	LDI	*AR4++,AR2
    // asm 00000C0E: 	LDI	270,R2
    // asm 00000C0F: 	LDI	PSSTARTY-40,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*AR4++,AR2
    // asm 00000C11: 	CMPI	0,AR2
    // asm 00000C12: 	BEQ	DONE_MSG
    // asm 00000C13: 	LDI	270,R2
    // asm 00000C14: 	LDI	PSSTARTY-30,R3
    // asm 00000C15: 	CALL	_outtextxyc
    // asm 00000C16: 	LDI	*AR4++,AR2
    // asm 00000C17: 	CMPI	0,AR2
    // asm 00000C18: 	BEQ	DONE_MSG
    // asm 00000C19: 	LDI	270,R2
    // asm: 	LDI	PSSTARTY-20,R3
    // asm 00000C1A: 	CALL	_outtextxyc
DONE_MSG:
    // asm 00000C1C: 	LDI	DEFAULT_COLOR,RC
    // asm 00000C1D: 	TEXTITT	"DETAILED PRICING INFORMATION",270,PSSTARTY-50
    // asm 00000C1E: 	TEXTITT	"LEFT CHUTE UNITS",270,PSSTARTY+0
    // asm 00000C1F: 	TEXTITT	"RIGHT CHUTE UNITS",270,PSSTARTY+10
    // asm: 	TEXTITT	"CENTER CHUTE UNITS",270,PSSTARTY+20
    // asm 00000C20: 	TEXTITT	"FOURTH CHUTE UNITS",270,PSSTARTY+30
    // asm 00000C21: 	TEXTITT	"UNITS PER CREDIT",270,PSSTARTY+40
    // asm 00000C22: 	TEXTITT	"UNITS FOR BONUS",270,PSSTARTY+50
    // asm 00000C23: 	TEXTITT	"MINIMUM UNITS",270,PSSTARTY+60
    // asm 00000C24: 	TEXTITT	"CREDITS TO START",270,PSSTARTY+70
    // asm 00000C25: 	TEXTITT	"CREDITS TO CONTINUE",270,PSSTARTY+80
    // asm: 	TEXTITT	"LEFT  CENTER  RIGHT  FOURTH",270,PSSTARTY+100
    // asm 00000C26: 	TEXTITT	"PRICING SELECTED",270,PSSTARTY+140
PS_SKIPMENU:
    // asm 00000C2B: 	POP	RC
    // asm 00000C2F: 	CMPI	0,AR7
    // asm 00000C33: 	BLE	PSLPX
    // asm 00000C3B: 	LDI	R0,AR4
    // asm 00000C3F: 	ADDI	@COUNTRY,AR4
    // asm 00000C43: 	MPYI	COIN_ENTRY_SIZE,AR4
    // asm 00000C47: 	ADDI	@COIN_TABLEI,AR4
    // asm 00000C4B: 	LDI	*AR4++,R4
    // asm 00000C4F: 	LDI	490,R2	;X POS
    // asm 00000C53: 	LDI	PSSTARTY,R3	;Y POS START OF TEXT
    // ;	PUSH	AR2
    // ;First print the SLOT units
PSLP:
    // asm: 	LDI	R4,R0
    // asm 00000C5A: 	AND	0FFh,R0
    // asm 00000C5B: 	PUSH	R2
    // asm 00000C5C: 	PUSH	RC
    // asm 00000C5D: 	LDI	R0,R2
    // asm 00000C5E: 	LDI	@bufferI,AR2
    // asm 00000C5F: 	CALL	_itoa
    // asm 00000C60: 	POP	RC
    // asm: 	POP	R2
    // asm: 	PUSH	R2		;PRESERVE R2 for LATER USE
    // asm: 	LDI	0,R1
    // asm: 	CMPI	10,R0
    // asm 00000C61: 	LDIGE	-8,R1		;Right justify
    // asm 00000C62: 	CMPI	100,R0
    // asm 00000C63: 	LDIGE	-16,R1		;Right justify
    // asm 00000C64: 	ADDI	R1,R2
    // asm 00000C65: 	CALL	_outtextxyc
    // asm 00000C66: 	POP	R2
    // asm 00000C67: 	LSH	-8,R4
    // asm 00000C68: 	ADDI	10,R3
    // asm 00000C69: 	CMPI	PSSTARTY+40,R3
    // asm 00000C6A: 	BLT	PSLP
    // asm 00000C6C: 	CMPI	PSSTARTY+80,R3
    // asm 00000C6D: 	BEQ	PSLP1
    // asm 00000C6F: 	CMPI	PSSTARTY+40,R3
    // asm 00000C70: 	BGT	PSLP1_CHECK
PSLP1:
    // ;	POP	AR2
    // asm 00000C73: 	LDI	*AR4++,R4
    // ;	PUSH	AR2
PSLP1_CHECK:
    // asm 00000C76: 	CMPI	PSSTARTY+90,R3
    // asm: 	BLT	PSLP
    // ;	POP	AR4
    // asm 00000C79: 	ADDI	3,AR4
    // ;Print LEFT slot denomination
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	PSSTARTY+120,R3
    // asm 00000C7B: 	LDI	270,R2
    // asm: 	CALL	_outtextxyc
    // ;Print RIGHT slot denomination
    // asm 00000C7D: 	LDI	*AR4++,AR2
    // asm: 	LDI	270+112,R2
    // asm: 	CALL	_outtextxyc
    // ;Print CENTER slot denomination
    // asm: 	LDI	*AR4++,AR2
    // asm 00000C7F: 	LDI	270+56,R2
    // asm 00000C80: 	CALL	_outtextxyc
    // ;Print FOURTH slot denomination
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	270+176,R2
    // asm 00000C83: 	CALL	_outtextxyc
    // asm: 	LDI	R6,R1
    // asm: 	ADDI	@COUNTRY,R1
    // asm 00000C86: 	READADJ	ADJ_COINMODE
    // asm 00000C87: 	CMPI	R0,R1
    // asm 00000C88: 	LDIEQ	1,R0
    // asm: 	LDINE	0,R0
    // asm: 	LDI	PSSTARTY+140,R3
    // asm 00000C89: 	CALL	PRINT_SPYN
PSLPX:
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_STANDARD", 0, 0);
    UNIMPL();
}

void PRINT_SPYN(void)
{
    // asm 00000C90: 	LDI	@YES,AR2
    // asm 00000C91: 	LDI	474,R2
    // asm 00000C92: 	CMPI	0,R0
    // asm 00000C93: 	LDIEQ	@NO,AR2
    // asm 00000C94: 	LDIEQ	482,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_SPYN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
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
int CUSTOM_PRICING_MENU;
const char *sct = "CALIBRATE CONTROLS";

void SET_CONTROLS(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTIT	sct,150,10
    // asm: 	TEXTITT	"TAKE HANDS AND FEET OFF CONTROLS, AND SET STEERING",50,40
    // asm: 	TEXTITT	"WHEEL TO CENTER THEN HIT ENTER",50,50
    // asm: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // 	;get gas pedal min
    // asm: 	LDI	@_pot1,R2
    // asm 00000CC0: 	SETADJ	ADJ_GASMIN
    // asm 00000CC1: 	LDI	@_pot2,R2
    // asm 00000CC2: 	SETADJ	ADJ_BRAKEMIN
    // 	;get wheel center
    // asm 00000CCE: 	LDI	@_pot0,R2
    // asm 00000CCF: 	SETADJ	ADJ_STEERCENTER
    // asm 00000CD1: 	CALL	CLRSCRN
    // 	;turn wheel to left and hit sw1
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm 00000CD4: 	TEXTIT	sct,150,10
    // asm 00000CD5: 	TEXTITT	"TURN WHEEL ALL THE WAY TO THE LEFT AND HIT ENTER",50,40
    // asm: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm 00000CD7: 	CALL	BEEP
    // asm: 	LDI	@_pot0,R2
    // asm: 	SETADJ	ADJ_STEERMIN
    // asm: 	CALL	CLRSCRN
    // asm 00000CDB: 	LDI	DEFAULT_COLOR,RC
    // asm 00000CDC: 	TEXTIT	sct,150,10
    // asm 00000CE0: 	TEXTITT	"TURN WHEEL ALL THE WAY TO THE RIGHT AND HIT ENTER",50,40		;turn wheel to right
    // asm 00000CE4: 	CALL	WT_NSTART
    // asm 00000CE5: 	CALL	WT_START
    // asm 00000CE6: 	CALL	BEEP
    // asm: 	LDI	@_pot0,R2
    // asm 00000CE7: 	SETADJ	ADJ_STEERMAX
    // asm 00000CF4: 	CALL	CLRSCRN
    // asm 00000CF5: 	LDI	DEFAULT_COLOR,RC
    // asm 00000CF6: 	TEXTIT	sct,150,10
    // 	;"push gas pedal all the way and hit sw1"
    // asm 00000CF7: 	TEXTITT	"PUSH GAS PEDAL TO MAX AND HIT ENTER",50,40
    // asm 00000CF8: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm: 	LDI	@_pot1,R2
    // asm: 	SETADJ	ADJ_GASMAX
    // asm 00000CFC: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_STANDUP,R0
    // asm 00000D00: 	BNZ	DADA5
    // asm 00000D05: 	CALL	CLRSCRN
    // asm 00000D06: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTIT	sct,150,10
    // asm: 	TEXTITT	"PUSH BRAKE PEDAL TO MAX AND HIT ENTER",50,40
    // asm 00000D07: 	CALL	WT_NSTART
    // asm 00000D08: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm: 	LDI	@_pot2,R2
    // asm 00000D0A: 	SETADJ	ADJ_BRAKEMAX
    // asm 00000D0C: 	CALL	WT_NSTART
DADA5:
    // asm 00000D0E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_CONTROLS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SET_VOLUME	SYSTEM ADJUSTMENT
*
*
*START	RETURN
*VIEW1	VOLUME UP
*VIEW2	VOLUME DOWN
*
*
 */
void SET_VOLUME(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	GROUP	90,60,410,240
    // asm: 	IWINDOW	120,72,240,90
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"ADJUST VOLUME",125,76
    // asm: 	IWINDOW	119,149,379,181
    // asm: 	FILL	120,150,378,180,COL_BLACK
    // asm: 	LDI	COL_VDGREY,RC
    // asm 00000D1F: 	TEXTITT	"MIN",118,135
    // asm: 	TEXTITT	"MAX",360,135
    // asm 00000D50: 	TEXTITT	"ENTER  EXIT",130,200
    // asm 00000D55: 	TEXTITT	"UP     INCREASE VOLUME",130,210
    // asm 00000D56: 	TEXTITT	"DOWN   DECREASE VOLUME",130,220
    // asm 00000D5A: 	READADJ	ADJ_VOLUME
    // asm 00000D5F: 	LDI	R0,R4
    // asm 00000D65: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm 00000D66: 	CMPI	R0,R4
    // asm 00000D6A: 	LDILT	R0,R4
    // asm 00000D6E: 	CALL	SHOW_VOLUME
    // asm 00000D76: 	SOND1	MUNSTER_SURF
    // asm: 	SOND1	ENGINESTART
    // asm 00000D7C: SETVOL_LP
    // asm: 	LDI	700,AR2
    // asm 00000D7D: 	CALL	RANDU0
    // asm 00000D7F: 	LDI	R0,R0
    // asm 00000D80: 	BNZ	SETNN
    // asm 00000D81: 	LDI	6,AR2
    // asm: 	CALL	RANDU0
    // asm 00000D84: 	LDPI	@INCIDENTI,AR2
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	ONESND
    // asm 00000D8C: SETNN
    // asm 00000D8F: 	LDI	@_newbut,R0
    // asm: 	LDI	R0,R1
    // asm: 	TSTB	SW_DIAG,R1		;move up
    // asm: 	BNZ	NOTRET
    // asm: 	SOND1	KILLALLS
    // asm: 	RETS
NOTRET:
    // asm: 	TSTB	SW_VOLPLUS,R0		;move up
    // asm: 	BNZ	NOTUPA
    // asm 00000D91: 	ADDI	1,R4
    // asm 00000D92: 	CMPI	255,R4
    // asm 00000D93: 	LDIGT	255,R4
    // asm: 	PUSH	R0
    // asm: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm: 	CMPI	R0,R4
    // asm 00000D97: 	LDILT	R0,R4
    // asm 00000D98: 	POP	R0
    // asm 00000D9A: 	LDI	R4,R1
    // asm 00000D9B: 	CALL	SET_MASTER_VOL
    // asm: 	LDI	R4,R2
    // asm: 	SETADJ	ADJ_VOLUME
    // asm 00000D9D: 	CALL	SHOW_VOLUME
    // asm 00000D9F: 	B	WTA
NOTUPA:
    // asm: 	TSTB	SW_VOLMINUS,R0		;move down
    // asm: 	BNZ	NOTDOWNA
    // asm 00000DA3: 	SUBI	1,R4
    // asm: 	LDILT	1,R4
    // asm 00000DA7: 	PUSH	R0
    // asm 00000DA8: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm: 	CMPI	R0,R4
    // asm: 	LDILT	R0,R4
    // asm 00000DA9: 	POP	R0
    // asm 00000DAB: 	LDI	R4,R1
    // asm 00000DAC: 	CALL	SET_MASTER_VOL
    // asm: 	LDI	R4,R2
    // asm: 	SETADJ	ADJ_VOLUME
    // asm 00000DAE: 	CALL	SHOW_VOLUME
WTA:
NOTDOWNA:
    // asm 00000DB2: 	B	SETVOL_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_VOLUME", 0, 0);
    UNIMPL();
}

void SHOW_VOLUME(void)
{
    // asm 00000DB3: 	LDI	120,AR2
    // asm 00000DB4: 	LDI	AR2,R3
    // asm: 	ADDI	R4,R3
    // asm 00000DB5: 	LDI	150,R2
    // asm 00000DB6: 	LDI	180,RC
    // asm: 	LDI	COL_GREEN,RS
    // asm 00000DB8: 	CALL	_fill
    // asm: 	LDI	120,AR2
    // asm 00000DB9: 	ADDI	R4,AR2
    // asm: 	LDI	355,R3
    // asm: 	LDI	150,R2
    // asm 00000DBA: 	LDI	180,RC
    // asm 00000DBB: 	LDI	COL_BLACK,RS
    // asm 00000DBC: 	CALL	_fill
    // asm 00000DBD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_VOLUME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FULL_FACTORY_RESTORE(void)
{
    // asm 00000DC5: 	LDL	FFR,AR2
    // asm 00000DC6: 	LDI	@NULLSTR,AR3
    // asm 00000DC7: 	CALL	CLEAR_YES_NO
    // asm 00000DC8: 	RETSC
    // asm: 	SETDP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULL_FACTORY_RESTORE", 0, 0);
    UNIMPL();
}

void FFRSUB(void)
{
    // asm: 	CALL	RESET_BOOKKEEPING
    // asm: 	CALL	INIT_HSTD_TABLES
    // asm: 	CALL	RESET_ADJUSTMENTS
    // asm: 	CLRI	R2
    // asm 00000DC9: 	SETAUD	AUD_PCREDITS
    // asm 00000DCA: 	SETAUD	AUD_CREDITS
    // asm 00000DCC: 	LDI	@FFRST,AR2
    // asm 00000DCD: 	CALL	MESSAGE_BOX
    // asm: 	CALL	SET_CONTROLS
    // asm 00000DCF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FFRSUB", 0, 0);
    UNIMPL();
}

/* asm: FFRST	SPTR	"FULL FACTORY RESTORE" */
const char *FFRST = "FULL FACTORY RESTORE";

/* *----------------------------------------------------------------------------
 */
void DEFAULT_ADJUSTMENTS(void)
{
    // asm: 	LDL	RDA,AR2
    // asm 00000DD9: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	CLEAR_YES_NO
    // asm: 	RETSC
    // asm: 	SETDP
    // asm: 	CALL	RESET_ADJUSTMENTS
    // asm: 	LDI	@DAS,AR2
    // asm: 	CALL	MESSAGE_BOX
    // asm: 	CALL	SET_CONTROLS
    // asm 00000DDC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEFAULT_ADJUSTMENTS", 0, 0);
    UNIMPL();
}

/* asm: DAS	SPTR	"DEFAULT ADJUSTMENTS SET" */
const char *DAS = "DEFAULT ADJUSTMENTS SET";

/* *----------------------------------------------------------------------------
 */
void RESET_HIGH_SCORES(void)
{
    // asm 00000DE3: 	LDL	RHS,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm 00000DE4: 	CALL	CLEAR_YES_NO
    // asm: 	RETSC
    // asm: 	SETDP
    // asm: 	CALL	INIT_HSTD_TABLES
    // asm: 	LDI	@HSRS,AR2
    // asm: 	CALL	MESSAGE_BOX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_HIGH_SCORES", 0, 0);
    UNIMPL();
}

/* asm: HSRS	SPTR	"HIGH SCORES RESET" */
const char *HSRS = "HIGH SCORES RESET";

/* *----------------------------------------------------------------------------
 */
void CLEAR_GAME_AUDITS(void)
{
    // asm 00000DED: 	LDL	CGA,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm 00000DEE: 	CALL	CLEAR_YES_NO
    // asm: 	RETSC
    // asm: 	SETDP
    // asm: 	CALL	RESET_BOOKKEEPING
    // asm: 	LDI	@BOOKR,AR2
    // asm: 	CALL	MESSAGE_BOX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_GAME_AUDITS", 0, 0);
    UNIMPL();
}

/* asm: BOOKR	SPTR	"BOOKKEEPING RESET" */
const char *BOOKR = "BOOKKEEPING RESET";
/* *----------------------------------------------------------------------------
 */
/* asm: FREEPLAYT	SPTR	"FREE PLAY" */
const char *FREEPLAYT = "FREE PLAY";

void FREEPLAYONOFF(void)
{
    // asm: 	READAUD	ADJ_FREE_PLAY
    // asm 00000DF6: 	LDI	R0,R5
    // asm: 	LDI	@FREEPLAYT,AR2
    // asm 00000DF8: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_FREE_PLAY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREEPLAYONOFF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: FGOO1	SPTR	"GIVE FREE GAME" */
const char *FGOO1 = "GIVE FREE GAME";
/* asm: FGOO2	SPTR	"ON 1ST POSITION" */
const char *FGOO2 = "ON 1ST POSITION";

void FREEGAMEONOFF(void)
{
    // asm: 	READAUD	ADJ_FREEGAME
    // asm 00000E01: 	LDI	R0,R5
    // asm 00000E04: 	LDI	@FGOO1,AR2
    // asm: 	LDI	@FGOO2,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_FREEGAME
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREEGAMEONOFF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: APHSTD	SPTR	"ALLOW PLAYER TO" */
const char *APHSTD = "ALLOW PLAYER TO";
/* asm: APHSTD2	SPTR	"ENTER INITIALS" */
const char *APHSTD2 = "ENTER INITIALS";

void HIGHSCORE_ENTRY(void)
{
    // asm: 	READAUD	ADJ_HIGH_SCORE_ENTRY
    // asm 00000E0D: 	LDI	R0,R5
    // asm 00000E10: 	LDI	@APHSTD,AR2
    // asm: 	LDI	@APHSTD2,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_HIGH_SCORE_ENTRY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIGHSCORE_ENTRY", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: GI1	SPTR	"SHOW GIRLS" */
const char *GI1 = "SHOW GIRLS";

void GIRLS_ONOFF(void)
{
    // asm 00000E17: 	READAUD	ADJ_GIRLS
    // asm 00000E18: 	LDI	R0,R5
    // asm 00000E19: 	LDI	@GI1,AR2
    // asm 00000E1A: 	LDI	@NULLSTR,AR3
    // asm 00000E1C: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_GIRLS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GIRLS_ONOFF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: CL1	SPTR	"SHOW PRESIDENT" */
const char *CL1 = "SHOW PRESIDENT";

void CLINTON_ONOFF(void)
{
    // asm 00000E22: 	READAUD	ADJ_CLINTON
    // asm 00000E23: 	LDI	R0,R5
    // asm 00000E24: 	LDI	@CL1,AR2
    // asm 00000E25: 	LDI	@NULLSTR,AR3
    // asm 00000E27: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_CLINTON
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLINTON_ONOFF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: RK1	SPTR	"SHOW ROADKILL" */
const char *RK1 = "SHOW ROADKILL";

void ROADKILL_ONOFF(void)
{
    // asm 00000E2C: 	READAUD	ADJ_ROADKILL
    // asm 00000E2D: 	LDI	R0,R5
    // asm: 	LDI	@RK1,AR2
    // asm 00000E2F: 	LDI	@NULLSTR,AR3
    // asm 00000E30: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_ROADKILL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADKILL_ONOFF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*	0 = MPH (default)
*	1 = KPH
*
 */
/* asm: MP1	SPTR	"SPEED IN MPH OR KPH" */
const char *MP1 = "SPEED IN MPH OR KPH";
/* asm: MP2	SPTR	"KPH" */
const char *MP2 = "KPH";
/* asm: MP3	SPTR	"MPH" */
const char *MP3 = "MPH";

void MILES_OR_KILO(void)
{
    // asm: 	READAUD	ADJ_MPHORKPM
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@MP1,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	LDI	@MP2,AR4
    // asm: 	LDI	@MP3,AR5
    // asm: 	CALL	DUAL_VALUE
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_MPHORKPM
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MILES_OR_KILO", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: CLR_CRED	SPTR	"CLEAR CREDITS" */
const char *CLR_CRED = "CLEAR CREDITS";
/* asm: YES		SPTR	"YES" */
const char *YES = "YES";
/* asm: NO		SPTR	"NO" */
const char *NO = "NO";
/* asm: ONS		SPTR	"ON" */
const char *ONS = "ON";
/* asm: OFFS		SPTR	"OFF" */
const char *OFFS = "OFF";
/* asm: NULLSTR		SPTR	"" */
const char *NULLSTR = "";
/* asm: AREUSURE	SPTR	"ARE YOU SURE?" */
const char *AREUSURE = "ARE YOU SURE?";

/* *----------------------------------------------------------------------------
 */
void CLEAR_CREDITS(void)
{
    // asm: 	LDI	@CLR_CRED,AR2
    // asm: 	LDI	@AREUSURE,AR3
    // asm: 	CALL	CLEAR_YES_NO
    // asm: 	RETSC
    // asm: 	SETDP
    // asm: 	CLRI	R2
    // asm: 	SETAUD	AUD_PCREDITS
    // asm: 	SETAUD	AUD_CREDITS
    // asm: 	LDI	@CCD2A,AR2
    // asm: 	CALL	MESSAGE_BOX
    // asm 00000E55: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_CREDITS", 0, 0);
    UNIMPL();
}

/* asm: CCD2A	SPTR	"CREDITS CLEARED" */
const char *CCD2A = "CREDITS CLEARED";

/* *----------------------------------------------------------------------------
 */
void MESSAGE_BOX(void)
{
    // asm 00000E5C: 	PUSH	AR2
    // asm: 	WINDOW	110,115,370,285
    // asm 00000E5E: 	IWINDOW	120,125,360,275
    // asm: 	LDI	COL_VDGREY,RC
    // asm 00000E60: 	POP	AR2
    // asm: 	LDI	130,R2
    // asm: 	LDI	170,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	30,AR6
ABLP:
    // asm 00000E62: CALL	WAIT_FOR_VBLANK
    // asm: 	DBU	AR6,ABLP
    // asm 00000E62: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MESSAGE_BOX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void CLEAR_YES_NO(void)
{
    // asm: 	LDI	0,R5
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_YES_NO", 0, 0);
    UNIMPL();
}

void ASK_YES_NO(void)
{
    // asm: 	LDI	@YES,AR4
    // asm: 	LDI	@NO,AR5
    // asm: 	BU	DUAL_VALUE
    // *	R5	DEFAULT
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASK_YES_NO", 0, 0);
    UNIMPL();
}

void DUAL_ON_OFF(void)
{
    // asm: 	LDI	@ONS,AR4
    // asm: 	LDI	@OFFS,AR5
    // asm: 	BU	DUAL_VALUE
DUAL_VALUE:
    // asm: 	AND	1,R5				;KEEP IT WITHIN RANGE
    // asm 00000E77: 	PUSH	AR4
    // asm 00000E78: 	PUSH	AR5
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR2
    // asm: 	WAIT_MAC_NOREPEAT
    // asm 00000E7B: 	WINDOW	110,115,370,285
    // asm 00000E7C: 	IWINDOW	120,125,360,275
    // asm: 	LDI	COL_VDGREY,RC
    // asm 00000E7D: 	POP	AR2
    // asm 00000E7E: 	LDI	130,R2
    // asm 00000E7F: 	LDI	140,R3
    // asm: 	CALL	_outtextxyc
    // asm 00000E81: 	POP	AR2
    // asm 00000E82: 	LDI	130,R2
    // asm: 	LDI	150,R3
    // asm 00000E86: 	CALL	_outtextxyc
    // asm 00000E90: 	TEXTITT	"UP TO CHANGE",130,235
    // asm: 	TEXTITT	"ENTER TO ACCEPT",130,245
    // asm 00000E91: 	CALL	WAIT_FOR_VBLANK
    // asm 00000E92: 	CALL	WAIT_FOR_VBLANK
    // asm: 	POP	AR7
    // asm 00000E95: 	POP	AR6
CYNLOOP:
    // asm 00000E98: 	LDI	COL_VDGREY,RC
    // asm: 	CMPI	1,R5
    // asm 00000E99: 	LDIEQ	COL_RED,RC
    // asm 00000E9D: 	LDI	AR6,AR2
    // asm 00000EA1: 	LDI	170,R2
    // asm 00000EA2: 	LDI	200,R3
    // asm: 	CALL	_outtextxyc
    // asm 00000EA4: 	LDI	COL_VDGREY,RC
    // asm: 	CMPI	0,R5
    // asm: 	LDIEQ	COL_RED,RC
    // asm 00000EA5: 	LDI	AR7,AR2
    // asm 00000EA6: 	LDI	220,R2
    // asm 00000EA7: 	LDI	200,R3
    // asm 00000EA8: 	CALL	_outtextxyc
    // asm 00000EAA: 	LDI	@_newbut,R0		;old system method
    // asm 00000EAB: 	TSTB    SW_DIAG,R0
    // asm: 	BZ	DOEXIT
    // asm 00000EAC: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm 00000EAD: 	BZ	CHNG
    // asm 00000EAE: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm 00000EAF: 	BZ	NCHNG
    // asm 00000EB0: 	BU	NCHNG
CHNG:
    // asm 00000EB2: 	XOR	1,R5
    // asm: 	SOND1	WPC_MISC2
NCHNG:
    // asm 00000EB4: 	WAIT_MAC_NOREPEAT
    // asm 00000EB5: 	B	CYNLOOP
DOEXIT:
    // asm 00000EB8: 	CMPI	0,R5
    // asm 00000EB9: 	BEQ	CYN_YES
    // asm 00000EBA: CYN_NO
    // asm: 	CLRC
    // asm 00000EBB: 	RETS
CYN_YES:
    // asm 00000EBE: 	SETC
    // asm 00000EC2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DUAL_ON_OFF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: SSGD	SPTR	"STEERING SENSITIVITY" */
const char *SSGD = "STEERING SENSITIVITY";
/* asm: SSGD2	SPTR	"0 (ADOLT) TO 5 (GAMEPLAYER)" */
const char *SSGD2 = "0 (ADOLT) TO 5 (GAMEPLAYER)";

void SET_STEERING_SENSITIVITY(void)
{
    // asm: 	READAUD	ADJ_STEERING_SENSITIVITY
    // asm: 	LDI	R0,R5
    // asm 00000EC8: 	LDI	@SSGD,AR2
    // asm: 	LDI	@SSGD2,AR3
    // asm: 	LDI	0,R6
    // asm: 	LDI	5,R7
    // asm: 	LDI	@GAMEDIFF_CVTI,AR4
    // asm: 	LDI	1,R4
    // asm: 	CALL	MULTI_VALUE_STEP
    // asm 00000ECB: 	LDI	R5,R2
    // asm 00000ECD: 	SETADJ	ADJ_STEERING_SENSITIVITY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_STEERING_SENSITIVITY", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: HSGD	SPTR	"PLAYS TO HIGHSCORE" */
const char *HSGD = "PLAYS TO HIGHSCORE";
/* asm: HSGD2	SPTR	"RESET 1000 TO 25000" */
const char *HSGD2 = "RESET 1000 TO 25000";

void SET_HIGHSCORE_RESET(void)
{
    // asm: 	READAUD	ADJ_HIGHSCORE_RESET
    // asm 00000ED5: 	LDI	R0,R5
    // asm 00000ED8: 	LDI	@HSGD,AR2
    // asm: 	LDI	@HSGD2,AR3
    // asm: 	LDI	1000,R6
    // asm: 	LDI	25000,R7
    // asm: 	LDI	@GAMEDIFF_CVTI,AR4
    // asm: 	LDI	1000,R4
    // asm: 	CALL	MULTI_VALUE_STEP
    // asm 00000EDB: 	LDI	R5,R2
    // asm 00000EDD: 	SETADJ	ADJ_HIGHSCORE_RESET
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_HIGHSCORE_RESET", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
 */
/* asm: SGD	SPTR	"GAME DIFFICULTY" */
const char *SGD = "GAME DIFFICULTY";
/* asm: SGD2	SPTR	"0 (EASY) TO 9 (HARD)" */
const char *SGD2 = "0 (EASY) TO 9 (HARD)";

void SET_GAME_DIFFICULTY(void)
{
    // asm: 	READAUD	ADJ_DIFFICULTY
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@SGD,AR2
    // asm: 	LDI	@SGD2,AR3
    // asm: 	CLRI	R6
    // asm: 	LDI	9,R7
    // asm: 	LDI	@GAMEDIFF_CVTI,AR4
    // asm: 	LDI	1,R4
    // asm: 	CALL	MULTI_VALUE_STEP
    // asm 00000EEB: 	LDI	R5,R2
    // asm 00000EED: 	SETADJ	ADJ_DIFFICULTY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_GAME_DIFFICULTY", 0, 0);
    UNIMPL();
}

/* *
*
 */
void GAMEDIFF_CVT(void)
{
    // asm: 	PUSH	R2
    // asm 00000EF2: 	LDI	R5,R2
    // asm 00000EF3: 	LDI	@bufferI,AR2
    // asm 00000EF4: 	CALL	_itoa
    // asm: 	LDI	@bufferI,AR2
    // asm 00000EF5: 	POP	R2
    // asm 00000EF6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GAMEDIFF_CVT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
 */
/* asm: ITD	SPTR	"INITIAL TIME" */
const char *ITD = "INITIAL TIME";
/* asm: ITD2	SPTR	"60-90 SECONDS" */
const char *ITD2 = "60-90 SECONDS";

void SET_TIME_TO_START(void)
{
    // asm 00000F00: 	READAUD	ADJ_TIME_TO_START
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@ITD,AR2
    // asm: 	LDI	@ITD2,AR3
    // asm: 	CLRI	R6
    // asm: 	LDI	6,R7
    // asm: 	LDI	@TIME_CVTI,AR4
    // asm: 	CALL	MULTI_VALUE
    // asm: 	LDI	R5,R2
    // asm 00000F03: 	SETADJ	ADJ_TIME_TO_START
    // asm 00000F05: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_TIME_TO_START", 0, 0);
    UNIMPL();
}

/* *
*
 */
void TIME_CVT(void)
{
    // asm 00000F09: 	PUSH	R2
    // asm: 	LDI	R5,R2
    // asm 00000F0A: 	MPYI	5,R2
    // asm 00000F0B: 	ADDI	60,R2
    // asm 00000F0C: 	LDI	@bufferI,AR2
    // asm 00000F0D: 	CALL	_itoa
    // asm 00000F0F: 	LDI	@bufferI,AR2
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIME_CVT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: CPBTT	SPTR	"CHECKPOINT TIME BONUS" */
const char *CPBTT = "CHECKPOINT TIME BONUS";
/* asm: CPBTT2	SPTR	"10-25 SECONDS" */
const char *CPBTT2 = "10-25 SECONDS";

void SET_CHECKPOINT_BONUS(void)
{
    // asm 00000F15: 	READAUD	ADJ_CHECKPOINT_BONUS
    // asm 00000F16: 	LDI	R0,R5
    // asm 00000F18: 	LDI	@CPBTT,AR2
    // asm 00000F19: 	LDI	@CPBTT2,AR3
    // asm: 	LDI	10,R6
    // asm: 	LDI	25,R7
    // asm: 	LDI	@CHECKPOINT_BONUS_CVTI,AR4
    // asm: 	CALL	MULTI_VALUE
    // asm: 	LDI	R5,R2
    // asm 00000F1C: 	SETADJ	ADJ_CHECKPOINT_BONUS
    // asm 00000F1E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_CHECKPOINT_BONUS", 0, 0);
    UNIMPL();
}

void CHECKPOINT_BONUS_CVT(void)
{
    // asm 00000F20: 	PUSH	R2
    // asm 00000F21: 	LDI	R5,R2
    // asm 00000F22: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm 00000F23: 	LDI	@bufferI,AR2
    // asm 00000F24: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKPOINT_BONUS_CVT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: MXT1	SPTR	"MAX CREDITS" */
const char *MXT1 = "MAX CREDITS";
/* asm: MXT2	SPTR	"10-50" */
const char *MXT2 = "10-50";

void SET_MAX_CREDITS(void)
{
    // asm 00000F2C: 	READAUD	ADJ_MAX_CREDITS
    // asm 00000F2D: 	LDI	R0,R5
    // asm 00000F2F: 	LDI	@MXT1,AR2
    // asm 00000F30: 	LDI	@MXT2,AR3
    // asm: 	LDI	10,R6
    // asm: 	LDI	50,R7
    // asm: 	LDI	@CHECKPOINT_BONUS_CVTI,AR4
    // asm: 	CALL	MULTI_VALUE
    // asm: 	LDI	R5,R2
    // asm 00000F33: 	SETADJ	ADJ_MAX_CREDITS
    // asm 00000F35: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MAX_CREDITS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
 */
/* asm: MVL	SPTR	"MINIMUM VOLUME LEVEL" */
const char *MVL = "MINIMUM VOLUME LEVEL";

void SET_MIN_VOL_LEVEL(void)
{
    // asm 00000F3F: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm: 	FLOAT	R0,R5
    // asm: 	MPYF	0.11764,R5
    // asm: 	FIX	R5
    // asm: 	LDI	@MVL,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CLRI	R6
    // asm: 	LDI	30,R7
    // asm: 	LDI	@SET_MIN_VOL_LEVEL_CVTI,AR4
    // asm: 	CALL	MULTI_VALUE
    // asm 00000F44: 	FLOAT	R5
    // asm 00000F45: 	MPYF	8.5,R5
    // asm: 	FIX	R5,R2
    // asm 00000F46: 	SETADJ	ADJ_MIN_VOL_LEVEL
    // asm 00000F47: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MIN_VOL_LEVEL", 0, 0);
    UNIMPL();
}

/* *
*
 */
void SET_MIN_VOL_LEVEL_CVT(void)
{
    // asm 00000F4B: 	PUSH	R2
    // asm: 	LDI	R5,R2
    // asm: 	LDI	@bufferI,AR2
    // asm 00000F4C: 	CALL	_itoa
    // asm 00000F4D: 	LDI	@bufferI,AR2
    // asm 00000F4E: 	POP	R2
    // asm 00000F4F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MIN_VOL_LEVEL_CVT", 0, 0);
    UNIMPL();
}

/* asm: REPEAT_SW	.bss	REPEAT_SW,1 */
int REPEAT_SW;
/* asm: REPEAT_TIME	.bss	REPEAT_TIME,1 */
int REPEAT_TIME;

void MULTI_VALUE(void)
{
    // asm: 	LDI	1,R4
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MULTI_VALUE", 0, 0);
    UNIMPL();
}

void MULTI_VALUE_STEP(void)
{
    // asm: 	PUSH	R5
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR2
    // asm: 	WAIT_MAC_NOREPEAT
    // asm: 	LDI	10000,R0
    // asm 00000F5B: 	MPYI	55,R0
    // asm 00000F5C: 	STI	R0,@REPEAT_TIME
    // asm 00000F5D: 	LDI	0,R0
    // asm 00000F5E: 	STI	R0,@REPEAT_SW
    // asm 00000F60: 	WINDOW	110,115,320,285
    // asm 00000F61: 	IWINDOW	120,125,310,275
    // asm 00000F62: 	LDI	COL_VDGREY,RC
    // asm: 	POP	AR2
    // asm 00000F66: 	LDI	130,R2
    // asm 00000F67: 	LDI	140,R3
    // asm 00000F68: 	CALL	_outtextxyc
    // asm 00000F6A: 	POP	AR2
    // asm: 	LDI	130,R2
    // asm: 	LDI	150,R3
    // asm 00000F6B: 	CALL	_outtextxyc
    // asm 00000F75: 	TEXTITT	"ENTER TO ACCEPT",130,225
    // asm: 	TEXTITT	"UP   TO INCREASE",130,235
    // asm 00000F76: 	TEXTITT	"DOWN   TO DECREASE",130,245
    // asm 00000F77: 	CALL	WAIT_FOR_VBLANK
    // asm 00000F78: 	CALL	WAIT_FOR_VBLANK
    // asm 00000F7A: 	POP	AR7
    // asm 00000F7B: 	POP	AR6
    // asm 00000F7C: 	POP	R7
    // asm 00000F7D: 	POP	R6
    // asm: 	POP	R5
DVLOOP:
    // asm 00000F86: 	CMPI	R6,R5
    // asm 00000F8A: 	LDILT	R6,R5
    // asm 00000F8B: 	CMPI	R7,R5
    // asm: 	LDIGT	R7,R5
    // asm 00000F8C: 	FILL	170,200,309,209,0
    // asm 00000F8E: 	LDI	AR4,R0
    // asm 00000F8F: 	CALLU	R0		;VALUE PASSED IN R5 (ALL REGS PRESERVED)
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	LDI	170,R2
    // asm 00000F91: 	LDI	200,R3
    // asm 00000F92: 	CALL	_outtextxyc
    // asm 00000F94: 	CALL	WAIT_FOR_VBLANK
    // asm: 	LDI	@_newbut,R0	;old system method
    // asm 00000F9B: 	TSTB    SW_DIAG,R0
    // asm 00000F9C: 	BZ	DOEXITDV
    // asm 00000F9D: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm 00000F9E: 	BZ	CHNGDV
    // asm 00000F9F: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm 00000FA0: 	BNZ	NCHNGDV
    // asm 00000FA1: 	SUBI	R4,R5
    // asm: 	LDI	980,AR2
    // asm 00000FA2: 	CALL	SENDSND
    // asm 00000FA3: 	BU	NCHNGDV
CHNGDV:
    // asm: 	ADDI	R4,R5
    // asm 00000FA5: 	LDI	981,AR2
    // asm 00000FA6: 	CALL	SENDSND
NCHNGDV:
    // asm 00000FA8: 	LDI	@REPEAT_TIME,R1
WT5:
    // asm: 	DEC	R1
    // asm 00000FAA: 	BLT	WTT5
    // asm 00000FAC: 	LDI	@_newbut,R0
    // asm: 	AND	SW_VOLPLUS|SW_VOLMINUS|SW_DIAG,R0
    // asm 00000FAD: 	CMPI	@REPEAT_SW,R0
    // asm 00000FAE: 	BEQ	WT5
WTT5:
    // asm: 	CMPI	@REPEAT_SW,R0
    // asm 00000FB0: 	BNE	WTTT5
    // asm: 	LDI	5000,R1
    // asm: 	MPYI	55,R1
    // asm 00000FB1: 	STI	R1,@REPEAT_TIME
    // asm 00000FB2: 	B	DVLOOP
WTTT5:
    // asm 00000FB3: 	LDI	10000,R1
    // asm 00000FB4: 	MPYI	55,R1
    // asm 00000FB5: 	STI	R1,@REPEAT_TIME
    // asm: 	STI	R0,@REPEAT_SW
    // asm 00000FB7: 	B	DVLOOP
DOEXITDV:
    // asm 00000FBA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MULTI_VALUE_STEP", 0, 0);
    UNIMPL();
}

/* asm: SPM	SPTR	"STANDARD PRICING MENU" */
const char *SPM = "STANDARD PRICING MENU";

void RUN_STANDARD_PRICING(void)
{
    // asm 00000FBF: 	LDL	STANDARD_PRICING_MENU,AR5
    // asm: 	LDI	STANDARD_PRICING_MENU_LEN,AR6
    // asm 00000FC0: 	LDI	@SPM,AR7
    // asm 00000FC1: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_STANDARD_PRICING", 0, 0);
    UNIMPL();
}

const char *SELTP = "SELECT THIS PRICING";

void PRICE_YES_NO(void)
{
    // asm: 	LDI	-1,R0
    // asm 00000FC4: 	STI	R0,@PSLAST		;JUMP START PRINT STANDARD
    // asm 00000FC6: 	LDL	SELTP,AR2
    // asm 00000FC7: 	LDI	@AREUSURE,AR3
    // asm: 	PUSH	R6
    // asm: 	CALL	CLEAR_YES_NO
    // asm: 	POP	R6
    // asm: 	RETSC
    // asm: 	LDI	R6,R2
    // asm: 	ADDI	@COUNTRY,R2
    // asm 00000FC8: 	LDI	ADJ_COINMODE,AR2
    // asm 00000FC9: 	CALL	ADJUSTMENT_WRITE
    // asm 00000FCA: 	LDI	1,R2
    // asm 00000FCB: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm 00000FCC: 	CALL	ADJUSTMENT_WRITE
    // 	;IF YOU CHANGE MODES YOU RESET THE CREDITS
    // 	;
    // asm: 	LDI	0,R2
    // asm 00000FD0: 	LDI	AUD_PCREDITS,AR2
    // asm 00000FD1: 	CALL	ADJUSTMENT_WRITE
    // asm 00000FD3: 	LDI	0,R2
    // asm: 	LDI	AUD_CREDITS,AR2
    // asm 00000FD4: 	CALL	ADJUSTMENT_WRITE
    // 	;NOTE: AUD_BCREDITS RESET DURING GAME INIT
    // asm: 	CALL	SET_COINAGE_ADJ		;Set the custom coinage to the standard just selected
    // asm 00000FD8: 	LDI	@STP,AR2
    // asm 00000FD9: 	CALL	MESSAGE_BOX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRICE_YES_NO", 0, 0);
    UNIMPL();
}

/* asm: STP	SPTR	"PRICING SELECTED" */
const char *STP = "PRICING SELECTED";
/* asm: RCMT	SPTR	"CUSTOM PRICING" */
const char *RCMT = "CUSTOM PRICING";

void RUN_CUSTOM_PRICING(void)
{
    // asm: 	LDL	CUSTOM_PRICING_MENU,AR5
    // asm 00000FDE: 	LDI	CUSTOM_PRICING_MENU_LEN,AR6
    // asm 00000FDF: 	LDI	@RCMT,AR7
    // asm 00000FE0: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_CUSTOM_PRICING", 0, 0);
    UNIMPL();
}

const char *FRACYN = "SHOW FRACTION";

void FRACTION_YES_NO(void)
{
    // asm 00000FE3: 	READAUD	ADJ_SHOW_FRAC
    // asm 00000FE4: 	LDI	R0,R5
    // asm 00000FE6: 	LDL	FRACYN,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	ASK_YES_NO
    // asm: 	LDI	1,R2
    // asm: 	LDIC	0,R2
    // asm: 	LDI	ADJ_SHOW_FRAC,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FRACTION_YES_NO", 0, 0);
    UNIMPL();
}

const char *USECP = "USE CUSTOM PRICING";

void CUSTOM_YES_NO(void)
{
    // asm 00000FEF: 	READAUD	ADJ_STANDARD_PRICING
    // asm 00000FF0: 	LDI	R0,R5
    // asm: 	XOR	1,R5
    // asm: 	LDL	USECP,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	ASK_YES_NO
    // asm: 	LDI	0,R2
    // asm: 	LDIC	1,R2
    // asm: 	PUSH	R2
    // asm: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm 00000FF2: 	CALL	ADJUSTMENT_WRITE
    // asm 00000FF4: 	POP	R2
    // asm 00000FF5: 	CMPI	1,R2
    // asm: 	BEQ	CUSTOM_NO
    // asm 00000FF6:  	LDI	@CPS,AR2
    // asm 00000FF7: 	CALL	MESSAGE_BOX
CUSTOM_NO:
    // asm 00000FFA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CUSTOM_YES_NO", 0, 0);
    UNIMPL();
}

/* asm: CPS	SPTR	"CUSTOM PRICING SELECTED" */
const char *CPS = "CUSTOM PRICING SELECTED";
/* asm: CMESSAGE_TAB */
/* asm: 	.word	CPCS,CPCS,CPCS,CPCS,CUPC,CTSS,CTCS,CCTS,CCTC,CTPC,CTPC,CTPC,CTPC */
const char *CMESSAGE_TAB[] = {
    CPCS, CPCS, CPCS, CPCS, CUPC, CTSS, CTCS, CCTS, CCTC, CTPC, CTPC, CTPC, CTPC,
};
const char *CPCS = "UNITS PER COIN";
const char *CUPC = "UNITS PER CREDIT";
const char *CTSS = "BONUS UNITS";
const char *CTCS = "MINIMUM UNITS";
const char *CCTS = "CREDITS TO START";
const char *CCTC = "CREDITS TO CONTINUE";
const char *CTPC = "COUNTER TICKS PER COIN";

void SET_SLOT(void)
{
    // asm: 	LDI	R6,AR2			;R6 = menu selection index
    // asm: 	LDI	ADJ_COIN1_UNITS,R0
    // asm: 	CMPI	9,R6
    // asm: 	BLT	SET_SLOT1
    // asm: 	LDI	ADJ_COIN1_COUNTER,R0
    // asm: 	SUBI	9,R0			;STRIP THE MENU OFFSET
    // asm 00001032: SET_SLOT1
    // asm: 	ADDI	R0,AR2
    // asm: 	CALL	ADJUSTMENT_READ
    // asm: 	LDI	R0,R5
    // asm: 	LDL	CMESSAGE_TAB,AR2
    // asm 00001032: 	ADDI	R6,AR2
    // asm 00001033: 	LDI	*AR2,AR2
    // asm 00001035: 	LDI	@NULLSTR,AR3
    // asm 00001036: 	PUSH	R6
    // asm 00001037: 	LDI	0,R6			;RANGE 0 to 100
    // asm: 	LDI	100,R7
    // asm 00001039: 	LDI	@GAMEDIFF_CVTI,AR4	;This will print the values as numbers
    // asm 0000103A: 	CALL	MULTI_VALUE
    // asm: 	POP	R6
    // asm 0000103C: 	LDI	R5,R2
    // asm 0000103D: 	LDI	R6,AR2
    // asm 0000103E: 	LDI	ADJ_COIN1_UNITS,R0
    // asm 0000103F: 	CMPI	9,R6
    // asm 00001040: 	BLT	SET_SLOT2
    // asm 00001041: 	LDI	ADJ_COIN1_COUNTER,R0
    // asm: 	SUBI	9,R0			;STRIP THE MENU OFFSET
    // asm 00001042: SET_SLOT2
    // asm 00001043: 	ADDI	R0,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // ;IF YOU CHANGE MODES YOU RESET THE CREDITS
    // asm 00001047: 	LDI	0,R2
    // asm 00001048: 	LDI	AUD_PCREDITS,AR2
    // asm 00001049: 	CALL	ADJUSTMENT_WRITE
    // asm 0000104B: 	LDI	0,R2
    // asm: 	LDI	AUD_CREDITS,AR2
    // asm 0000104C: 	CALL	ADJUSTMENT_WRITE
    // asm 0000104D: 	RETS
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
int PSTAB[] = {
    ADJ_COIN1_UNITS, PRINT_NUMBER,
    ADJ_COIN2_UNITS, PRINT_NUMBER,
    ADJ_COIN3_UNITS, PRINT_NUMBER,
    ADJ_COIN4_UNITS, PRINT_NUMBER,
    ADJ_UNITS_PER_CREDIT, PRINT_NUMBER,
    ADJ_BONUS_UNITS, PRINT_NUMBER,
    ADJ_UNITS_MIN, PRINT_NUMBER,
    ADJ_CREDITS_TO_START, PRINT_NUMBER,
    ADJ_CREDITS_TO_CONTINUE, PRINT_NUMBER,
    ADJ_COIN1_COUNTER, PRINT_NUMBER,
    ADJ_COIN2_COUNTER, PRINT_NUMBER,
    ADJ_COIN3_COUNTER, PRINT_NUMBER,
    ADJ_COIN4_COUNTER, PRINT_NUMBER,
    ADJ_SHOW_FRAC, PRINT_YN,
    ADJ_STANDARD_PRICING, PRINT_YN_INV,
    0, 0,
};

void PRINT_SLOT(void)
{
    // asm: 	PUSH	AR4
    // asm: 	LDI	@PSTABI,AR4
    // asm: 	CALL	PRINT_VALUE
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_SLOT", 0, 0);
    UNIMPL();
}

void PRINT_VALUE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR2
    // asm 00001056: 	MPYI	2,R0
    // asm 00001057: 	ADDI	R0,AR4
    // asm 00001059: 	LDI	*AR4,AR2
    // asm 0000105A: 	CALL	ADJUSTMENT_READ
    // ;	LDI	*+AR4,AR2
    // ;	ADDI	@PVTABI,AR2
    // ;	LDI	*AR2,R1
    // ;	BEQ	PRINT_VALUEX
    // asm: 	LDI	*+AR4,R1
    // asm 0000105F: 	BEQ	PRINT_VALUEX
    // asm 00001060: 	CALLU	R1
PRINT_VALUEX:
    // asm 00001061: 	POP	AR2
    // asm 00001062: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
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

void PRINT_VOL(void)
{
    // asm: 	PUSH	RC
    // asm: 	FLOAT	R0
    // asm: 	MPYF	0.11764,R0
    // asm: 	FIX	R0
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@bufferI,AR2
    // asm: 	LDI	270,R2
    // asm: 	CMPI	10,R0
    // asm 0000106B: 	LDIGE	262,R2
    // asm: 	CMPI	100,R0
    // asm 0000106C: 	LDIGE	254,R2
    // asm 0000106D: 	POP	RC
    // asm 0000106E: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_VOL", 0, 0);
    UNIMPL();
}

void GETSTR_LENGTH(void)
{
    // asm 00001072: 	PUSH	AR2
    // asm 00001073: 	PUSH	R0
    // asm 00001074: 	PUSH	R1
    // asm 00001075: 	PUSH	R3
    // asm 00001077: 	CLRI	R2
GSLP:
    // asm 00001079: 	CLRI	R1
    // asm 0000107A: 	LDI	*AR2++,R3
GSLP1:
    // asm: 	LDI	R3,R0
    // asm: 	LSH	R1,R0
    // asm 0000107B: 	AND	0FFH,R0
    // asm 0000107C: 	BEQ	GSLDONE
    // asm 0000107D: 	ADDI	1,R2
    // asm 0000107E: 	SUBI	8,R1
    // asm: 	CMPI	-32,R1
    // asm 0000107F: 	BGT	GSLP1
    // asm: 	BR	GSLP
GSLDONE:
    // asm 00001081: 	POP	R3
    // asm: 	POP	R1
    // asm 00001082: 	POP	R0
    // asm 00001083: 	POP	AR2
    // asm 00001084: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSTR_LENGTH", 0, 0);
    UNIMPL();
}

void PRINT_COINMODE(void)
{
    // asm 00001087: 	PUSH	R0
    // asm 00001088: 	READADJ	ADJ_STANDARD_PRICING
    // asm 00001089: 	POP	R2
    // asm 0000108A: 	CMPI	0,R0
    // asm: 	RETSEQ
    // asm 0000108B: 	ADDI	@COINMSGC_TABI,R2
    // asm 0000108C: 	LDI	R2,AR2
    // asm 0000108D: 	LDI	*AR2,AR2
    // asm 0000108E: 	CALL	GETSTR_LENGTH
    // asm 0000108F: 	MPYI	-8,R2
    // asm: 	ADDI	278,R2
    // asm: 	CALL	_outtextxyc
    // asm 00001090: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_COINMODE", 0, 0);
    UNIMPL();
}

void PRINT_CUSTOM_SELECTED(void)
{
    // asm 00001094: 	CMPI	1,R0
    // asm 00001095: 	RETSEQ
    // asm 00001097: 	LDI	@ISON,AR2
    // asm 00001098: 	LDI	262,R2
    // asm 00001099: 	CALL	_outtextxyc
    // asm 0000109A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_CUSTOM_SELECTED", 0, 0);
    UNIMPL();
}

void PRINT_TIME(void)
{
    // asm: 	MPYI	5,R0
    // asm: 	ADDI	60,R0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_TIME", 0, 0);
    UNIMPL();
}

void PRINT_NUMBER(void)
{
    // asm: 	PUSH	RC
    // asm 000010A0: 	LDI	R0,R2
    // asm 000010A1: 	LDI	@bufferI,AR2
    // asm 000010A2: 	CALL	_itoa
    // asm 000010A3: 	LDI	@bufferI,AR2
    // asm: 	LDI	270,R2
    // asm: 	CMPI	10,R0
    // asm: 	LDIGE	262,R2
    // asm 000010A4: 	CMPI	100,R0
    // asm 000010A5: 	LDIGE	254,R2
    // asm: 	CMPI	1000,R0
    // asm: 	LDIGE	246,R2
    // asm 000010A6: 	CMPI	10000,R0
    // asm 000010A7: 	LDIGE	238,R2
    // asm 000010A9: 	PUSH	R1
    // asm 000010AA: 	LDI	10000,R1
    // asm 000010AB: 	MPYI	10,R1
    // asm 000010AC: 	CMPI	R1,R0
    // asm 000010AD: 	LDIGE	230,R2
    // asm 000010AE: 	POP	R1
    // asm 000010B1: 	POP	RC
    // asm 000010B2: 	CALL	_outtextxyc
    // asm 000010B3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_NUMBER", 0, 0);
    UNIMPL();
}

void PRINT_YN_INV(void)
{
    // asm 000010B5: 	NEGI	R0
    // asm 000010B6: 	ADDI	1,R0	;Drops through to PRINT_YN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_YN_INV", 0, 0);
    UNIMPL();
}

void PRINT_YN(void)
{
    // asm 000010B8: 	LDI	@YES,AR2
    // asm 000010B9: 	LDI	254,R2
    // asm: 	CMPI	0,R0
    // asm: 	LDIEQ	@NO,AR2
    // asm 000010BA: 	LDIEQ	262,R2
    // asm 000010BB: 	CALL	_outtextxyc
    // asm 000010BC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_YN", 0, 0);
    UNIMPL();
}

void PRINT_ONOFF_INV(void)
{
    // asm 000010BD: 	NEGI	R0
    // asm 000010BE: 	ADDI	1,R0	;Drops through to PRINT_ONOFF
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_ONOFF_INV", 0, 0);
    UNIMPL();
}

void PRINT_ONOFF(void)
{
    // asm 000010BF: 	LDI	@ISON,AR2
    // asm 000010C0: 	LDI	262,R2
    // asm 000010C1: 	CMPI	0,R0
    // asm 000010C2: 	LDIEQ	@ISOFF,AR2
    // asm 000010C3: 	LDIEQ	254,R2
    // asm 000010C4: 	CALL	_outtextxyc
    // asm 000010C5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_ONOFF", 0, 0);
    UNIMPL();
}

void PRINT_MPH_INV(void)
{
    // asm 000010C6: 	NEGI	R0
    // asm 000010C7: 	ADDI	1,R0	;Drops through to PRINT_MPH
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_MPH_INV", 0, 0);
    UNIMPL();
}

void PRINT_MPH(void)
{
    // asm 000010C8: 	LDI	@MP3,AR2
    // asm 000010C9: 	LDI	254,R2
    // asm 000010CA: 	CMPI	1,R0
    // asm 000010CB: 	LDIEQ	@MP2,AR2
    // asm 000010CC: 	CALL	_outtextxyc
    // asm 000010CD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_MPH", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
int MAIN_MENU;
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
int DIAGNOSTIC_MENU;
#define UTILITIES_MENU_LEN 6
/* asm: UTILITIES_MENU: */
/* asm: MENUENTRY	"CLEAR CREDITS",CLEAR_CREDITS */
/* asm: MENUENTRY	"CLEAR GAME AUDITS",CLEAR_GAME_AUDITS */
/* asm: MENUENTRY	"RESET HIGH SCORES",RESET_HIGH_SCORES */
/* asm: MENUENTRY	"DEFAULT ADJUSTMENTS",DEFAULT_ADJUSTMENTS */
/* asm: MENUENTRY	"FULL FACTORY RESTORE",FULL_FACTORY_RESTORE */
/* asm: MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN */
int UTILITIES_MENU;
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
int ADJUSTMENT_MENU;
#define MOTION_MENU_LEN 7
/* asm: MOTION_MENU */
/* asm: MENUENTRY	"FRONT AXIS UP/DOWN",DIAG_MOTION_TESTZ */
/* asm: MENUENTRY	"LEFT REAR AXIS UP/DOWN",DIAG_MOTION_TESTX */
/* asm: MENUENTRY	"RIGHT REAR AXIS UP/DOWN",DIAG_MOTION_TESTY */
/* asm: MENUENTRY	"COMBINED MOTION",DIAG_MOTION_BURNIN */
/* asm: MENUENTRY	"MOTION SAFETY SWITCHES",MOTION_SAFETY_SWITCHES_DIAG */
int MOTION_MENU;
/* *----------------------------------------------------------------------------
 */
/* asm: LAMPT	SPTR	"LAMP TEST" */
const char *LAMPT = "LAMP TEST";
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
int LAMP_MENU;

void RUN_LAMP_TEST(void)
{
    // asm: 	LDL	LAMP_MENU,AR5
    // asm: 	LDI	LAMP_MENU_LEN,AR6
    // asm: 	LDI	@LAMPT,AR7
    // asm: 	BU	HANDLE_MENU
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_LAMP_TEST", 0, 0);
    UNIMPL();
}

void LAMPSOFF(void)
{
    // asm: 	LDI	0,R0
    // asm: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPSOFF", 0, 0);
    UNIMPL();
}

void LAMPSALL(void)
{
    // asm: 	LDI	0FFh,R0
    // asm: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPSALL", 0, 0);
    UNIMPL();
}

void LAMPS(void)
{
    // asm: LDI	1,R0
    // asm: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPS", 0, 0);
    UNIMPL();
}

void LAMPV1(void)
{
    // asm 00001155: LDI	2,R0
    // asm 00001156: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV1", 0, 0);
    UNIMPL();
}

void LAMPV2(void)
{
    // asm 00001157: LDI	4,R0
    // asm 00001158: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV2", 0, 0);
    UNIMPL();
}

void LAMPV3(void)
{
    // asm 00001159: LDI	8,R0
    // asm 00001159: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV3", 0, 0);
    UNIMPL();
}

void LAMPLT(void)
{
    // asm 0000115A: LDI	10h,R0
    // asm: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPLT", 0, 0);
    UNIMPL();
}

void LAMPRT(void)
{
    // asm 0000115B: LDI	20h,R0
    // asm 0000115C: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPRT", 0, 0);
    UNIMPL();
}

void LAMPLF(void)
{
    // asm 0000115D: LDI	40h,R0
    // asm 0000115E: 	BU	LJIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPLF", 0, 0);
    UNIMPL();
}

void LAMPRF(void)
{
    // asm 0000115F: LDI	80h,R0
LJIN:
    // asm 00001161: LDP	@BUTTON_STATUS
    // asm 00001162: 	STI	R0,@BUTTON_STATUS
    // asm 00001163: 	SETDP
    // asm 00001164: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPRF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MOTION_CHECK_SAFETY_CODES(void)
{
    // asm 0000116A: 	PUSH	R0
    // asm 0000116B: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm 0000116C: 	PUSH	R3
    // asm 0000116E: 	CALL	CLEAR_MATS
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R0
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm 00001170: 	RS	8,R0		;right justify
    // asm 00001172: 	LDL	MOTION_SAFETY_CODES,AR2
MCSC_LP:
    // asm 00001173: LDI	*AR2,R1
    // asm: 	CMPI	-1,R1
    // asm 00001174: 	BEQ	NO_ERRORS
    // asm: 	AND	*+AR2(1),R1
    // asm 00001176: 	LDI	R0,R3
    // asm 00001177: 	AND	*+AR2(1),R3
    // asm 00001178: 	CMPI	R3,R1
    // asm 00001179: 	BNE	JJDDA
    // asm: 	LDI	*+AR2(2),R0
    // asm 0000117B: 	CALLU	R0
    // asm 0000117C: 	BU	ALL_DONE
JJDDA:
    // asm 0000117E: NOP	*++AR2(3)
    // asm 0000117F: 	BU	MCSC_LP
NO_ERRORS:
    // asm 00001182: 	IWINDOW	50,320,450,360
    // asm 00001183: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"ALL SYSTEMS OK",70,330
ALL_DONE:
    // asm 00001186: 	POP	R3
    // asm: 	POP	R2
    // asm 00001187: 	POP	R1
    // asm 00001188: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_CHECK_SAFETY_CODES", 0, 0);
    UNIMPL();
}

/* asm: MOTION_SAFETY_CODES: */
/* asm: MSCM	080h,080h,MSC_BOARD_NOTPLUGGED */
/* asm: MSCM	070h,0F0h,MSC_FAILSAFE_NOTCONNECTED */
/* asm: MSCM	060h,0F0h,MSC_FAILSAFE_OK */
/* asm: MSCM	050h,0F0h,MSC_OPTO_LED */
/* asm: MSCM	040h,0F0h,MSC_OPTO_DETECTOR */
/* asm: MSCM	030h,0F0h,MSC_OPTO_PATH */
/* asm: MSCM	020h,0F0h,MSC_MAT_STEPPED */
/* asm: MSCM	010h,0F0h,MSC_MAT_NOTPLUGGED */
int MOTION_SAFETY_CODES;

/* *----------------------------------------------------------------------------
 */
void MSC_BOARD_NOTPLUGGED(void)
{
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"BOARD NOT PLUGGED IN OR PAL NOT INSTALLED",70,330
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_BOARD_NOTPLUGGED", 0, 0);
    UNIMPL();
}

void MSC_MAT_STEPPED(void)
{
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"MAT STEPPED ON",70,330
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R4
    // asm: 	LDI	*AR2,AR2
    // asm 000011B1: 	SETDP
    // asm 000011B6: 	RS	8,R4		;right justify
    // asm: 	TSTB	1,R4
    // asm 000011BB: 	BZ	NM1
    // asm: 	FILL	102,142,128,218,COL_RED
NM1:
    // asm 000011BC: TSTB	2,R4
    // asm 000011C1: 	BZ	NM2
    // asm 000011C2: 	FILL	32,232,128,278,COL_RED
NM2:
    // asm 000011C6: TSTB	4,R4
    // asm: 	BZ	NM3
    // asm 000011C6: 	FILL	32,142,58,218,COL_RED
NM3:
    // asm 000011C7: TSTB	8,R4
    // asm 000011C8: 	BZ	NM4
    // asm 000011C9: 	FILL	32,82,128,118,COL_RED
NM4:
    // asm 000011CA: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_MAT_STEPPED", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_MAT_NOTPLUGGED(void)
{
    // asm 000011D4: 	IWINDOW	50,320,450,360
    // asm 000011D5: 	LDI	COL_BLACK,RC
    // asm 000011D6: 	TEXTITT	"MAT NOT PLUGGED IN",70,330
    // asm 000011DE: 	CLRI	AR2
    // asm 000011E4: 	LDP	@991030h
    // asm 000011E5: 	LDI	@991030h,R4
    // asm 000011E6: 	LDI	*AR2,AR2
    // asm 000011EC: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	1,R4
    // asm: 	BZ	NM1A
    // asm: 	FILL	102,142,128,218,COL_RED
NM1A:
    // asm 000011ED: TSTB	2,R4
    // asm 000011F2: 	BZ	NM2A
    // asm 000011F3: 	FILL	32,232,128,278,COL_RED
NM2A:
    // asm 000011F7: TSTB	4,R4
    // asm: 	BZ	NM3A
    // asm 000011F7: 	FILL	32,142,58,218,COL_RED
NM3A:
    // asm 000011F8: TSTB	8,R4
    // asm 000011F9: 	BZ	NM4A
    // asm 000011FA: 	FILL	32,82,128,118,COL_RED
NM4A:
    // asm 000011FB: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_MAT_NOTPLUGGED", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_OPTO_PATH(void)
{
    // asm 00001205: 	IWINDOW	50,320,450,360
    // asm 00001206: 	LDI	COL_BLACK,RC
    // asm 00001207: 	TEXTITT	"OPTO PATH BROKEN",70,330
    // asm 0000120D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_PATH", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_OPTO_LED(void)
{
    // asm 0000121D: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"OPTO LED NOT EMITTING",70,330
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm 0000121E: 	LDI	@991030h,R4
    // asm 00001223: 	LDI	*AR2,AR2
    // asm 00001224: 	SETDP
    // asm 00001228: 	RS	8,R4		;right justify
    // asm: 	TSTB	1,R4
    // asm: 	BZ	NJ1
    // asm: 	FILLW	150+32,212,16,16,COL_RED	;L1
NJ1:
    // asm 00001229: TSTB	2,R4
    // asm 0000122E: 	BZ	NJ2
    // asm 0000122F: 	FILLW	150+57,232,16,16,COL_RED	;L2
NJ2:
    // asm 00001233: TSTB	4,R4
    // asm 00001234: 	BZ	NJ3
    // asm 00001235: 	FILLW	150+87,232,16,16,COL_RED	;L3
NJ3:
    // asm 00001237: TSTB	8,R4
    // asm 00001238: 	BZ	NJ4
    // asm: 	FILLW	150+112,212,16,16,COL_RED 	;L4
NJ4:
    // asm 0000123A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_LED", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_OPTO_DETECTOR(void)
{
    // asm 0000124A: 	IWINDOW	50,320,450,360
    // asm 0000124B: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"OPTO DETECTOR NOT RECEIVING",70,330
    // asm 00001253: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm 00001259: 	LDI	@991030h,R4
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	1,R4
    // asm: 	BZ	NJ1A
    // asm: 	FILLW	150+32,112,16,16,COL_RED	;D1
NJ1A:
    // asm 0000125F: TSTB	2,R4
    // asm 00001260: 	BZ	NJ2A
    // asm: 	FILLW	150+57,82,16,16,COL_RED		;D2
NJ2A:
    // asm 00001264: TSTB	4,R4
    // asm 00001265: 	BZ	NJ3A
    // asm 00001266: 	FILLW	150+87,82,16,16,COL_RED		;D3
NJ3A:
    // asm 00001268: TSTB	8,R4
    // asm 00001269: 	BZ	NJ4A
    // asm: 	FILLW	150+112,112,16,16,COL_RED 	;D4
NJ4A:
    // asm 0000126B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_DETECTOR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_FAILSAFE_OK(void)
{
    // asm 0000127A: 	IWINDOW	50,320,450,360
    // asm 0000127B: 	LDI	COL_BLACK,RC
    // asm 0000127C: 	TEXTITT	"FAIL SAFE SWITCH ENGAGED",70,330
    // asm 00001282: 	CLRI	AR2
    // asm 00001283: 	LDP	@991030h
    // asm 00001284: 	LDI	@991030h,R4
    // asm: 	LDI	*AR2,AR2
    // asm 0000128A: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	4,R4
    // asm: 	BZ	NM4B
    // asm: 	FILL	150+150+62,82,150+150+98,118,COL_RED
NM4B:
    // asm 0000128B: TSTB	1,R4
    // asm 00001290: 	BZ	NM1B
    // asm 00001291: 	FILL	150+150+32,232,150+150+58,278,COL_RED
NM1B:
    // asm 00001295: TSTB	2,R4
    // asm 00001295: 	BZ	NM2B
    // asm 00001296: 	FILL	150+150+102,232,150+150+128,278,COL_RED
NM2B:
    // asm 00001298: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_FAILSAFE_OK", 0, 0);
    UNIMPL();
}

void MSC_FAILSAFE_NOTCONNECTED(void)
{
    // asm 0000129A: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"FAIL SAFE SWITCH NOT CONNECTED CORRECTLY",70,330
    // asm 0000129C: 	CLRI	AR2
    // asm 0000129D: 	LDP	@991030h
    // asm 000012A3: 	LDI	@991030h,R4
    // asm 000012A4: 	LDI	*AR2,AR2
    // asm 000012A5: 	SETDP
    // asm 000012AB: 	RS	8,R4		;right justify
    // asm 000012AD: 	TSTB	4,R4
    // asm: 	BZ	NM4BA
    // asm 000012B3: 	FILL	150+150+62,82,150+150+98,118,COL_RED
NM4BA:
    // asm 000012B4: TSTB	1,R4
    // asm 000012B4: 	BZ	NM1BA
    // asm 000012B9: 	FILL	150+150+32,232,150+150+58,278,COL_RED
NM1BA:
    // asm 000012BA: TSTB	2,R4
    // asm: 	BZ	NM2BA
    // asm 000012BE: 	FILL	150+150+102,232,150+150+128,278,COL_RED
NM2BA:
    // asm 000012C2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_FAILSAFE_NOTCONNECTED", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void DRAW_MOTION_SAFETY_DIAG(void)
{
    // asm 000012CE: 	LDI	COL_BLACK,RC
    // asm 000012D4: 	TEXTITT	"MATS:",50,50
    // asm 000012D5: 	TEXTITT	"FRONT",55,72
    // asm 000012D6: 	TEXTITT	"REAR",55,285
    // 	;MATS
    // asm: 	RECT	30,80,130,120,COL_BLACK
    // asm: 	RECT	30,140,60,220,COL_BLACK
    // asm 000012DC: 	RECT	100,140,130,220,COL_BLACK
    // asm: 	RECT	30,230,130,280,COL_BLACK
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"OPTOS:",150+50,50
    // asm: 	TEXTITT	"FRONT",150+55,72
    // asm 000012DD: 	TEXTITT	"REAR",150+55,285
    // 	;LEDS
    // asm 000012E6: 	RECTW	150+30,110,20,20,COL_BLACK	;D1
    // asm: 	RECTW	150+55,80,20,20,COL_BLACK	;D2
    // asm 000012EA: 	RECTW	150+85,80,20,20,COL_BLACK	;D3
    // asm 000012F0: 	RECTW	150+110,110,20,20,COL_BLACK 	;D4
    // asm 000012FC: 	RECTW	150+30,210,20,20,COL_BLACK	;L1
    // asm: 	RECTW	150+55,230,20,20,COL_BLACK	;L2
    // asm: 	RECTW	150+85,230,20,20,COL_BLACK	;L3
    // asm: 	RECTW	150+110,210,20,20,COL_BLACK 	;L4
    // ;	RECT	150+30,80,150+130,120,COL_BLACK
    // ;	RECT	150+30,140,150+60,220,COL_BLACK
    // ;	RECT	150+100,140,150+130,220,COL_BLACK
    // ;	RECT	150+30,230,150+130,280,COL_BLACK
    // asm 00001327: 	LDI	COL_BLACK,RC
    // asm 0000132D: 	TEXTITT	"FAILSAFE SWITCHES:",150+150,50
    // asm 00001333: 	TEXTITT	"FRONT",150+150+61,72
    // asm 00001339: 	TEXTITT	"REAR",150+150+65,285
    // 	;FAIL SAFE
    // asm: 	RECT	150+150+60,80,150+150+100,120,COL_BLACK
    // asm: 	RECT	150+150+30,230,150+150+60,280,COL_BLACK
    // asm: 	RECT	150+150+100,230,150+150+130,280,COL_BLACK
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"HIT ENTER TO ESCAPE",20,380
    // asm: 	TEXTITT	"MOTION SAFETY DIAGNOSTICS",30,20
    // asm: 	TEXTITT	"ERROR MESSAGE:",50,305
    // asm 0000133F: 	TEXTITT	"MOTION STOP:",300,15
    // asm 00001344: 	IWINDOW	50,320,450,360	;error window
    // asm 00001348: 	IWINDOW	390,5,430,35	;motion
    // asm 0000134C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRAW_MOTION_SAFETY_DIAG", 0, 0);
    UNIMPL();
}

void CLEAR_MATS(void)
{
    // 	;MATS
    // asm 0000135E: 	FILL	32,82,128,118,0
    // asm 0000135F: 	FILL	32,142,58,218,0
    // asm 00001363: 	FILL	102,142,128,218,0
    // asm 00001367: 	FILL	32,232,128,278,0
    // 	;FAIL SAFE
    // asm: 	FILL	150+150+62,82,150+150+98,118,0
    // asm 0000136F: 	FILL	150+150+32,232,150+150+58,278,0
    // asm 00001374: 	FILL	150+150+102,232,150+150+128,278,0
    // 	;LED (LED)
    // asm: 	FILLW	150+32,212,16,16,0	;L1
    // asm: 	FILLW	150+57,232,16,16,0	;L2
    // asm 0000137A: 	FILLW	150+87,232,16,16,0	;L3
    // asm 00001380: 	FILLW	150+112,212,16,16,0 	;L4
    // 	;LED (DETECTOR)
    // asm: 	FILLW	150+32,112,16,16,0	;D1
    // asm: 	FILLW	150+57,82,16,16,0	;D2
    // asm 00001392: 	FILLW	150+87,82,16,16,0	;D3
    // asm 00001398: 	FILLW	150+112,112,16,16,0 	;D4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_MATS", 0, 0);
    UNIMPL();
}

/* asm: OLD_MOTION_CODE	.bss	OLD_MOTION_CODE,1 */
int OLD_MOTION_CODE;

void MOTION_SAFETY_SWITCHES_DIAG(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	CALL	DRAW_MOTION_SAFETY_DIAG
    // asm: 	CALL	MOTION_CHECK_SAFETY_CODES
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R0
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	7,R0
    // asm: 	LS	16+7,R0
    // asm: 	RS	16,R0
    // asm: 	STI	R0,@OLD_MOTION_CODE
MSSD_LP:
    // asm: 	NOT	@_newbut,R0
    // asm 000013D7: 	AND	SW_DIAG,R0
    // asm 000013D8: 	RETSNZ
    // asm 000013DA: 	CLRI	AR2
    // asm 000013DB: 	LDP	@991030h
    // asm 000013DC: 	LDI	@991030h,R0
    // asm 000013DD: 	LDI	*AR2,AR2
    // asm 000013DE: 	SETDP
    // asm 000013E0: 	TSTB	080h,R0
    // asm: 	BZ	MSOFF
    // asm: 	FILL	400,10,420,30,COL_RED
    // asm: 	BU	MSFX
MSOFF:
    // asm 000013E2: FILL	400,10,420,30,0
MSFX:
    // asm 000013E6: 	CLRI	AR2
    // asm 000013E7: 	LDP	@991030h
    // asm 000013E8: 	LDI	@991030h,R0
    // asm: 	LDI	*AR2,AR2
    // asm 000013E9: 	SETDP
    // asm 000013EA: 	RS	7,R0
    // asm: 	LS	16+7,R0
    // asm 000013EB: 	RS	16,R0
    // asm 000013F1: 	CMPI	@OLD_MOTION_CODE,R0
    // asm: 	BEQ	MSSD_LP
    // asm 000013F2: 	STI	R0,@OLD_MOTION_CODE
    // ;
    // ;binary debugging aid
    // ;
    // asm 000013F9: 	FILL	300,305,500,315,0
    // asm 000013FD: 	CLRI	AR2
    // asm 000013FE: 	LDP	@991030h
    // asm 000013FF: 	LDI	@991030h,R2
    // asm 00001400: 	LDI	*AR2,AR2
    // asm 00001401: 	SETDP
    // asm 00001402: 	RS	8,R2
    // asm: 	AND	0FFh,R2
    // asm: 	LDL	IBOIBOT,AR2
    // asm: 	CALL	HEX2ASC
    // asm: 	LDI	COL_BLACK,RC
    // asm 00001403: 	LDI	300,R2
    // asm: 	LDI	305,R3
    // asm: 	CALL	_outtextxyc
    // ;end dbg
    // ;
    // asm 0000140D: 	CALL	MOTION_CHECK_SAFETY_CODES
    // asm 0000140E: 	BU	MSSD_LP
    // asm 00001410: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_SAFETY_SWITCHES_DIAG", 0, 0);
    UNIMPL();
}

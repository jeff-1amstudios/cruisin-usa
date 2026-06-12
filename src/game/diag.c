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
#include "diag_defs.h"

/*
 * Source module: asm/DIAG.ASM
 */

/* ;
*----------------------------------------------------------------------------
 */
#define MW_Y2 0
#define MW_X2 1
#define MW_Y1 2
#define MW_X1 3
const char *inst1 = "USE UP/DOWN TO SELECT FUNCTION";
const char *inst2 = "ENTER TO ACTIVATE";
const char *title = "CRUISIN USA   DIAGNOSTICS";
const char *toexit = "HIT ENTER TO EXIT";
/* asm: DIAGPAL: */
/* asm: 	.word	16			;LENGTH OF PALETTE */
int DIAGPAL = 16;
/* asm: SOFT_AUDIT	.bss	SOFT_AUDIT,1 */
int SOFT_AUDIT;
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
/* *----------------------------------------------------------------------------
 */
/* asm: _onebut	.bss	_onebut,1 */
int _onebut;
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
int SWITCHES_DIAG[40] = {
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
const char *SW1T = "DIP SW2";
const char *SW2T = "DIP SW3";
/* asm: SW1BANK	.bss	SW1BANK,3 */
int SW1BANK[3];
/* asm: SW2BANK	.bss	SW2BANK,3 */
int SW2BANK[3];
/* asm: LASTDIPPLOT	.bss	LASTDIPPLOT,1 */
int LASTDIPPLOT;
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
const char * *ISON = ONTEXT;
/* asm: ISOFF	.word	OFFTEXT */
const char * *ISOFF = OFFTEXT;
const char *ONTEXT = "ON";
const char *OFFTEXT = "OFF";
const char *DNSUP = "UPRIGHT";
const char *DSUP = "SITDOWN";
const char *COINCNTR1 = "ONE COIN COUNTER";
const char *COINCNTR2 = "TWO COIN COUNTERS";
const char *DMOT = "MOTION ON";
const char *DNMOT = "MOTION OFF";
const char *DLINKD = "LINK ON";
const char *DNLINKD = "LINK OFF";
/* asm: DMOD1	SPTR	"MASTER" */
const char *DMOD1 = "MASTER";
/* asm: DMOD2	SPTR	"SLAVE" */
const char *DMOD2 = "SLAVE";
/* *----------------------------------------------------------------------------
 */
/* asm: MOMT	SPTR	"MOTION MENU" */
const char *MOMT = "MOTION MENU";
/* *----------------------------------------------------------------------------
 */
/* asm: AMT	SPTR	"ADJUSTMENT MENU" */
const char *AMT = "ADJUSTMENT MENU";
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
int ADJTAB[36] = {
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
/* *----------------------------------------------------------------------------
 */
/* asm: UMT	SPTR	"UTILITIES MENU" */
const char *UMT = "UTILITIES MENU";
/* *----------------------------------------------------------------------------
 */
/* asm: MMT	SPTR	"MAIN MENU" */
const char *MMT = "MAIN MENU";
/* asm: DMT	SPTR	"DIAGNOSTIC MENU" */
const char *DMT = "DIAGNOSTIC MENU";
/* asm: MENU_SCROLL	.bss	MENU_SCROLL,1 */
int MENU_SCROLL;
/* *----------------------------------------------------------------------------
 */
/* asm: AMODES	SPTR	"ATTRACT MODE SOUNDS" */
const char *AMODES = "ATTRACT MODE SOUNDS";
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
int USA_MENU[26] = {
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
int GERMAN_MENU[18] = {
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
int FRENCH_MENU[24] = {
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
int CANADA_MENU[8] = {
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
int SWISS_MENU[8] = {
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
int ITALY_MENU[8] = {
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
int UK_MENU[8] = {
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
int SPAIN_MENU[14] = {
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
int AUSTRALIA_MENU[10] = {
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
int JAPAN_MENU[8] = {
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
int TAIWAN_MENU[8] = {
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
int AUSTRIA_MENU[10] = {
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
int BELGIUM_MENU[8] = {
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
int OTHER_MENU[16] = {
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
int GENERAL_MENU[18] = {
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
int COUNTRY_MENUTAB[30] = {
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
#define PSSTARTY 100
/* asm: PSLAST	.bss	PSLAST,1 */
int PSLAST;
/* *----------------------------------------------------------------------------
 */
#define CUSTOM_PRICING_MENU_LEN 16
const char *sct = "CALIBRATE CONTROLS";
/* asm: INCIDENT	.word	CARPASS,TRUCKPASS */
/* asm: 	.word	SCOLLA,TKEY */
/* asm: 	.word	BOTTOMOUT,VIEW2IN */
int INCIDENT[6] = {
    CARPASS, TRUCKPASS,
    SCOLLA, TKEY,
    BOTTOMOUT, VIEW2IN,
};
const char *FFR = "FULL FACTORY RESTORE";
/* asm: FFRST	SPTR	"FULL FACTORY RESTORE" */
const char *FFRST = "FULL FACTORY RESTORE";
const char *RDA = "RESET DEFAULT ADJUSTMENTS";
/* asm: DAS	SPTR	"DEFAULT ADJUSTMENTS SET" */
const char *DAS = "DEFAULT ADJUSTMENTS SET";
const char *RHS = "RESET HIGH SCORES";
/* asm: HSRS	SPTR	"HIGH SCORES RESET" */
const char *HSRS = "HIGH SCORES RESET";
const char *CGA = "CLEAR GAME AUDITS";
/* asm: BOOKR	SPTR	"BOOKKEEPING RESET" */
const char *BOOKR = "BOOKKEEPING RESET";
/* *----------------------------------------------------------------------------
 */
/* asm: FREEPLAYT	SPTR	"FREE PLAY" */
const char *FREEPLAYT = "FREE PLAY";
/* *----------------------------------------------------------------------------
 */
/* asm: FGOO1	SPTR	"GIVE FREE GAME" */
const char *FGOO1 = "GIVE FREE GAME";
/* asm: FGOO2	SPTR	"ON 1ST POSITION" */
const char *FGOO2 = "ON 1ST POSITION";
/* *----------------------------------------------------------------------------
 */
/* asm: APHSTD	SPTR	"ALLOW PLAYER TO" */
const char *APHSTD = "ALLOW PLAYER TO";
/* asm: APHSTD2	SPTR	"ENTER INITIALS" */
const char *APHSTD2 = "ENTER INITIALS";
/* *----------------------------------------------------------------------------
 */
/* asm: GI1	SPTR	"SHOW GIRLS" */
const char *GI1 = "SHOW GIRLS";
/* *----------------------------------------------------------------------------
 */
/* asm: CL1	SPTR	"SHOW PRESIDENT" */
const char *CL1 = "SHOW PRESIDENT";
/* *----------------------------------------------------------------------------
 */
/* asm: RK1	SPTR	"SHOW ROADKILL" */
const char *RK1 = "SHOW ROADKILL";
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
/* asm: CCD2A	SPTR	"CREDITS CLEARED" */
const char *CCD2A = "CREDITS CLEARED";
/* *----------------------------------------------------------------------------
 */
/* asm: SSGD	SPTR	"STEERING SENSITIVITY" */
const char *SSGD = "STEERING SENSITIVITY";
/* asm: SSGD2	SPTR	"0 (ADOLT) TO 5 (GAMEPLAYER)" */
const char *SSGD2 = "0 (ADOLT) TO 5 (GAMEPLAYER)";
/* *----------------------------------------------------------------------------
 */
/* asm: HSGD	SPTR	"PLAYS TO HIGHSCORE" */
const char *HSGD = "PLAYS TO HIGHSCORE";
/* asm: HSGD2	SPTR	"RESET 1000 TO 25000" */
const char *HSGD2 = "RESET 1000 TO 25000";
/* *----------------------------------------------------------------------------
*
*
*
 */
/* asm: SGD	SPTR	"GAME DIFFICULTY" */
const char *SGD = "GAME DIFFICULTY";
/* asm: SGD2	SPTR	"0 (EASY) TO 9 (HARD)" */
const char *SGD2 = "0 (EASY) TO 9 (HARD)";
/* *----------------------------------------------------------------------------
*
*
*
 */
/* asm: ITD	SPTR	"INITIAL TIME" */
const char *ITD = "INITIAL TIME";
/* asm: ITD2	SPTR	"60-90 SECONDS" */
const char *ITD2 = "60-90 SECONDS";
/* *----------------------------------------------------------------------------
 */
/* asm: CPBTT	SPTR	"CHECKPOINT TIME BONUS" */
const char *CPBTT = "CHECKPOINT TIME BONUS";
/* asm: CPBTT2	SPTR	"10-25 SECONDS" */
const char *CPBTT2 = "10-25 SECONDS";
/* *----------------------------------------------------------------------------
 */
/* asm: MXT1	SPTR	"MAX CREDITS" */
const char *MXT1 = "MAX CREDITS";
/* asm: MXT2	SPTR	"10-50" */
const char *MXT2 = "10-50";
/* *----------------------------------------------------------------------------
*
*
*
 */
/* asm: MVL	SPTR	"MINIMUM VOLUME LEVEL" */
const char *MVL = "MINIMUM VOLUME LEVEL";
/* asm: REPEAT_SW	.bss	REPEAT_SW,1 */
int REPEAT_SW;
/* asm: REPEAT_TIME	.bss	REPEAT_TIME,1 */
int REPEAT_TIME;
/* asm: SPM	SPTR	"STANDARD PRICING MENU" */
const char *SPM = "STANDARD PRICING MENU";
const char *SELTP = "SELECT THIS PRICING";
/* asm: STP	SPTR	"PRICING SELECTED" */
const char *STP = "PRICING SELECTED";
/* asm: RCMT	SPTR	"CUSTOM PRICING" */
const char *RCMT = "CUSTOM PRICING";
const char *FRACYN = "SHOW FRACTION";
const char *USECP = "USE CUSTOM PRICING";
/* asm: CPS	SPTR	"CUSTOM PRICING SELECTED" */
const char *CPS = "CUSTOM PRICING SELECTED";
/* asm: CMESSAGE_TAB */
/* asm: 	.word	CPCS,CPCS,CPCS,CPCS,CUPC,CTSS,CTCS,CCTS,CCTC,CTPC,CTPC,CTPC,CTPC */
const char * *CMESSAGE_TAB[13] = {
    CPCS, CPCS, CPCS, CPCS, CUPC, CTSS, CTCS, CCTS, CCTC, CTPC, CTPC, CTPC, CTPC,
};
const char *CPCS = "UNITS PER COIN";
const char *CUPC = "UNITS PER CREDIT";
const char *CTSS = "BONUS UNITS";
const char *CTCS = "MINIMUM UNITS";
const char *CCTS = "CREDITS TO START";
const char *CCTC = "CREDITS TO CONTINUE";
const char *CTPC = "COUNTER TICKS PER COIN";
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
int PSTAB[32] = {
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
/* *----------------------------------------------------------------------------
*MENU DEFINITIONS
*
*
 */
#define MAIN_MENU_LEN 7
#define DIAGNOSTIC_MENU_LEN 10
#define UTILITIES_MENU_LEN 6
#define ADJUSTMENT_MENU_LEN 18
#define MOTION_MENU_LEN 7
/* *----------------------------------------------------------------------------
 */
/* asm: LAMPT	SPTR	"LAMP TEST" */
const char *LAMPT = "LAMP TEST";
#define LAMP_MENU_LEN 11
/* asm: OLD_MOTION_CODE	.bss	OLD_MOTION_CODE,1 */
int OLD_MOTION_CODE;

void MAKE_WINDOW(void)
{
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	RC
    // asm: 	LDI	SP,AR3
    // asm: 	LDI	COL_LGREY,RS
    // asm: 	CALL	_fill
    // asm: 	LDI	*-AR3(MW_X1),AR2
    // asm: 	LDI	*-AR3(MW_Y1),R2
    // asm: 	LDI	*-AR3(MW_X2),R3
    // asm: 	LDI	*-AR3(MW_Y1),RC
    // asm: 	LDI	COL_LGREY2,RS
    // asm: 	CALL	_line
    // asm: 	LDI	*-AR3(MW_X1),AR2
    // asm: 	LDI	*-AR3(MW_Y1),R2
    // asm: 	LDI	*-AR3(MW_X1),R3
    // asm: 	LDI	*-AR3(MW_Y2),RC
    // asm: 	LDI	COL_LGREY2,RS
    // asm: 	CALL	_line
    // asm: 	LDI	*-AR3(MW_X2),AR2
    // asm: 	LDI	*-AR3(MW_Y1),R2
    // asm: 	LDI	*-AR3(MW_X2),R3
    // asm: 	LDI	*-AR3(MW_Y2),RC
    // asm: 	LDI	COL_DGREY,RS
    // asm: 	CALL	_line
    // asm: 	LDI	*-AR3(MW_X1),AR2
    // asm: 	LDI	*-AR3(MW_Y2),R2
    // asm: 	LDI	*-AR3(MW_X2),R3
    // asm: 	LDI	*-AR3(MW_Y2),RC
    // asm: 	LDI	COL_DGREY,RS
    // asm: 	CALL	_line
    // asm: 	POP	RC
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	AR2
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_WINDOW", 0, 0);
    UNIMPL();
}

void MAKE_IWINDOW(void)
{
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	RC
    // asm: 	LDI	SP,AR3
    // asm: 	LDI	COL_LGREY,RS
    // asm: 	CALL	_fill
    // asm: 	LDI	*-AR3(MW_X1),AR2
    // asm: 	LDI	*-AR3(MW_Y1),R2
    // asm: 	LDI	*-AR3(MW_X2),R3
    // asm: 	LDI	*-AR3(MW_Y1),RC
    // asm: 	LDI	COL_DGREY,RS
    // asm: 	CALL	_line
    // asm: 	LDI	*-AR3(MW_X1),AR2
    // asm: 	LDI	*-AR3(MW_Y1),R2
    // asm: 	LDI	*-AR3(MW_X1),R3
    // asm: 	LDI	*-AR3(MW_Y2),RC
    // asm: 	LDI	COL_DGREY,RS
    // asm: 	CALL	_line
    // asm: 	LDI	*-AR3(MW_X2),AR2
    // asm: 	LDI	*-AR3(MW_Y1),R2
    // asm: 	LDI	*-AR3(MW_X2),R3
    // asm: 	LDI	*-AR3(MW_Y2),RC
    // asm: 	LDI	COL_LGREY2,RS
    // asm: 	CALL	_line
    // asm: 	LDI	*-AR3(MW_X1),AR2
    // asm: 	LDI	*-AR3(MW_Y2),R2
    // asm: 	LDI	*-AR3(MW_X2),R3
    // asm: 	LDI	*-AR3(MW_Y2),RC
    // asm: 	LDI	COL_LGREY2,RS
    // asm: 	CALL	_line
    // asm: 	POP	RC
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	AR2
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_IWINDOW", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void HARD_LOAD_DIAGPAL(void)
{
    // asm: 	LDL	DIAGPAL,AR2
    // asm: 	LDIL	COLORAM,AR1
    // asm: 	LDI	*AR2++,AR0
    // asm: 	DEC	AR0
    // asm: LODLP
    // asm: 	LDI	*AR2++,R0
    // asm: 	STI	R0,*AR1++
    // asm: 	DBU	AR0,LODLP
    // asm: 	RETS					;F  -cycling color
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HARD_LOAD_DIAGPAL", 0, 0);
    UNIMPL();
}

void HIDDEN_DIAG(void)
{
    // asm: 	CALL	ENTER_DIAG_INIT
    // asm: 	LDI	@DIAGPAL+1,R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	LDI	MDIAG,R0		;set mode appropriate
    // asm: 	STI	R0,@_MODE
    // asm: 	CALL	CLRSCRN			;we are NOT page flipping
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@SOFT_AUDIT
    // asm: 	CALL	AUDIT_DISPLAY
    // asm: 	BR	DIAG_RETURN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIDDEN_DIAG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
 */
void ENTER_DIAG_INIT(void)
{
    // asm: 	POP	R0
    // asm: 	LDP	@FASTSTKI
    // asm:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm: 	PUSH	R0
    // asm: 	FIFO_CLRP	R0		;is the fifo clear
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // asm: 	CALL	SILENT
    // asm: 	LDI	1,R2
    // asm: 	SETAUD	ADJ_OUTOFDIAG
    // asm: 	LDI	MDIAG,R0		;MELLOW THE WHEEL
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	INT0_M|INT3_M,IE	;ENABLE INTERRUPTS
    // asm: 	AND     0,IF
    // asm: 	CALL	ENABLEGIE		;MUST DO AT START FOR I/O READING
    // asm: 	CALL	INIT_SYSTEM
    // asm: 	CALL	PAL_INIT
    // asm: 	LDL	DIAGPAL,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	CLRI	AR2
    // asm: 	CALL	SENDSND
    // asm: 	CLRI	R4
    // asm: 	STI	R4,@SOFT_AUDIT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTER_DIAG_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void ENTER_DIAG(void)
{
    // asm: 	CALL	ENTER_DIAG_INIT
    // asm: ENTER_DIAG2
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	LDI	MDIAG,R0		;set mode appropriate
    // asm: 	STI	R0,@_MODE
    // asm: 	LDL	MAIN_MENU,AR5
    // asm: 	LDI	MAIN_MENU_LEN,AR6
    // asm: 	LDI	@MMT,AR7
    // asm: 	BU	HANDLE_MENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTER_DIAG", 0, 0);
    UNIMPL();
}

void RETURN_TO_MAINMENU(void)
{
    // asm: 	LDI	MDIAG,R0		;set mode appropriate
    // asm: 	STI	R0,@_MODE
    // asm: 	LDL	MAIN_MENU,AR5
    // asm: 	LDI	MAIN_MENU_LEN,AR6
    // asm: 	LDI	@MMT,AR7
    // asm: 	BU	HMER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RETURN_TO_MAINMENU", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TOEXIT(void)
{
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTIT	toexit,180,371
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOEXIT", 0, 0);
    UNIMPL();
}

void MONITOR_DIAGNOSTICS(void)
{
    // asm: 	CALL	COLORBARS
    // asm: 	CALL	KEYPAUSE
    // asm: 	CALL	PAL_INIT
    // asm: 	LDL	DIAGPAL,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDIL	SCREEN0,AR2
    // asm: 	LDI	@FILSIZI,R3
    // asm: 	LDI	@REDV,R2
    // asm: 	CALL	SCREEN_FILL
    // asm: 	CALL	KEYPAUSE
    // ;	CALL	DIAG_ACTIVEP
    // asm: 	LDIL	SCREEN0,AR2
    // asm: 	LDI	@FILSIZI,R3
    // asm: 	LDI	@GREENV,R2
    // asm: 	CALL	SCREEN_FILL
    // asm: 	CALL	KEYPAUSE
    // ;	CALL	DIAG_ACTIVEP
    // asm: 	LDIL	SCREEN0,AR2
    // asm: 	LDI	@FILSIZI,R3
    // asm: 	LDI	@BLUEV,R2
    // asm: 	CALL	SCREEN_FILL
    // asm: 	CALL	KEYPAUSE
    // ;	CALL	DIAG_ACTIVEP
    // asm: 	LDIL	SCREEN0,AR2
    // asm: 	LDI	@FILSIZI,R3
    // asm: 	LDI	@BLACKV,R2
    // asm: 	CALL	SCREEN_FILL
    // 	;12 across
    // 	;HLINES
    // 	;20 down
    // asm: 	LDI	20,AR7
    // asm: 	LDI	0,AR6
    // asm: LINLOP
    // asm: 	CMPI	20,AR6
    // asm: 	LDIEQ	19,AR6
    // asm: 	CLRI	AR2
    // asm: 	LDI	511,R3
    // asm: 	LDI	AR6,R2
    // asm: 	LDI	AR6,RC
    // asm: 	LDI	1,RS
    // asm: 	CALL	_line
    // asm: 	ADDI	20,AR6
    // asm: 	DBU	AR7,LINLOP
    // 	;VLINES
    // asm: 	LDI	32,AR7
    // asm: 	LDI	0,AR6
    // asm: VLINLOP
    // asm: 	CMPI	16,AR6
    // asm: 	LDIEQ	15,AR6
    // asm: 	CLRI	R2
    // asm: 	LDI	399,RC
    // asm: 	LDI	AR6,AR2
    // asm: 	LDI	AR6,R3
    // asm: 	LDI	1,RS
    // asm: 	CALL	_line
    // asm: 	ADDI	16,AR6
    // asm: 	DBU	AR7,VLINLOP
    // asm: 	CALL	KEYPAUSE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MONITOR_DIAGNOSTICS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void COLORBARS(void)
{
    // asm: 	CALL	CLRSCRN0
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	LDI	09Eh,AR0
    // asm: 	LS	16,AR0
    // asm: 	LDI	15,RC
    // asm: 	RPTB	GREYLP
    // asm: 	LDI	R0,R1
    // asm: 	LDI	R0,R2
    // asm: 	LS	5,R1
    // asm: 	LS	10,R2
    // asm: 	LDI	R0,R3
    // asm: 	OR	R1,R3
    // asm: 	OR	R2,R3
    // asm: 	STI	R3,*AR0++
GREYLP:
    // asm: ADDI	2,R0
    // asm: 	CLRI	R0
    // asm: 	LDI	15,RC
    // asm: 	RPTB	REDLP
    // asm: 	LDI	R0,R1
    // asm: 	LS	10,R1
    // asm: 	STI	R1,*AR0++
REDLP:
    // asm: ADDI	2,R0
    // asm: 	CLRI	R0
    // asm: 	LDI	15,RC
    // asm: 	RPTB	GREENLP
    // asm: 	LDI	R0,R1
    // asm: 	LSH	5,R1
    // asm: 	STI	R1,*AR0++
GREENLP:
    // asm: ADDI	2,R0
    // asm: 	CLRI	R0
    // asm: 	LDI	15,RC
    // asm: 	RPTB	BLUELP
    // asm: 	STI	R0,*AR0++
    // asm: BLUELP	ADDI	2,R0
    // asm: 	CLRI	R3		;color
    // asm: 	LDI	50,R2		;starting Y coordinate
    // asm: 	LDI	3,AR4
    // asm: WHOLELP
    // asm: 	LDI	R3,R4
    // asm: 	LDI	50,AR5		;lines per mess
GROUPLP:
    // asm: LDI	R4,R3
    // asm: 	LDI	128,AR2		;starting X pixel
    // asm: 	LDI	15,AR7		;blocks per lines
LINELP:
    // asm: LDI	15,AR6
    // asm: BLOCKLP	CALL	_pixel
    // asm: 	INC	AR2		;next pixel
    // asm: 	DBU	AR6,BLOCKLP
    // asm: 	INC	R3
    // asm: 	DBU	AR7,LINELP
    // asm: 	INC	R2
    // asm: 	DBU	AR5,GROUPLP
    // asm: 	ADDI	20,R2
    // asm: 	DBU	AR4,WHOLELP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLORBARS", 0, 0);
    UNIMPL();
}

void SHOWSWITCH(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@_opot0
    // asm: 	STI	R0,@_opot1
    // asm: 	STI	R0,@_opot2
    // asm: 	STI	R0,@_onebut
    // asm: 	STI	R0,@LASTDIPPLOT
    // asm: 	STI	R0,@LASTSHIFT
    // asm: 	IWINDOW	125,170,145,355
    // asm: 	IWINDOW	135,95,220,135
    // asm: 	IWINDOW	245,95,420,135
    // asm: 	GROUP	10,60,465,150	;pots etc
    // asm: 	GROUP	10,160,170,360	;switches
    // asm: 	GROUP	190,160,360,250	;shifter
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"SWITCH TEST",205,26
    // asm: 	TEXTITT	"STEERING WHEEL",20,100
    // asm: 	TEXTITT	"GAS PEDAL",20,110
    // asm: 	TEXTITT	"BRAKE PEDAL",20,120
    // asm: 	TEXTITT	"VIEW 1",20,180
    // asm: 	TEXTITT	"VIEW 2",20,200
    // asm: 	TEXTITT	"VIEW 3",20,220
    // asm: 	TEXTITT	"START",20,240
    // asm: 	TEXTITT	"COIN 1",20,260
    // asm: 	TEXTITT	"COIN 2",20,280
    // asm: 	TEXTITT	"COIN 3",20,300
    // asm: 	TEXTITT	"COIN 4",20,320
    // asm: 	TEXTITT	"RADIO",20,340
    // asm: 	TEXTITT	"HIT ENTER TO EXIT",40,40
    // asm: 	TEXTITT	"ENTER",215,280
    // asm: 	TEXTITT	"VOLUME UP",215,300
    // asm: 	TEXTITT	"VOLUME DOWN",215,320
    // asm: 	TEXTITT	"SERVICE",215,340
    // asm: 	TEXTITT	"CREDIT",225,350
    // asm: 	IWINDOW	325,270,355,360
SSLP:
    // ;	CALL	DIAG_ACTIVEP
    // asm: 	CALL	DISPPOTS
    // asm: 	CALL	DISPLAY_SWITCH_STATUS
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"CMOS VALUES",250,70
    // asm: 	TEXTITT	"MIN  CENTER MAX",250,80
    // asm: 	TEXTITT	"POT READING",134,80
    // asm: 	READADJ	ADJ_GASMIN
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	250,R2
    // asm: 	LDI	110,R3
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	READADJ	ADJ_GASMAX
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	350,R2
    // asm: 	LDI	110,R3
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	READADJ	ADJ_STEERMIN
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	250,R2
    // asm: 	LDI	100,R3
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	READADJ	ADJ_STEERCENTER
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	300,R2
    // asm: 	LDI	100,R3
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	READADJ	ADJ_STEERMAX
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	350,R2
    // asm: 	LDI	100,R3
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	READADJ	ADJ_BRAKEMIN
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	250,R2
    // asm: 	LDI	120,R3
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	READADJ	ADJ_BRAKEMAX
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	350,R2
    // asm: 	LDI	120,R3
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	7FFFh,R0		;waste a little time
    // asm: 	MPYI	10,R0
    // asm: 	RPTS	R0
    // asm: 	NOP
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_DIAG,R0
    // asm: 	BNZ	NOWTOEXIT
    // ;	RETSNZ
    // asm: 	CALL	WAIT_FOR_VBLANK		;wait for vblk
    // asm: 	B	SSLP
NOWTOEXIT:
    // asm: 	LDI	330,AR2
    // asm: 	LDI	AR2,R3
    // asm: 	ADDI	10,R3
    // asm: 	LDI	275,R2
    // asm: 	SUBI	2,R2
    // asm: 	LDI	R2,RC
    // asm: 	ADDI	12,RC
    // asm: 	LDI	COL_RED,RS
    // asm: 	CALL	_fill
    // asm: 	LDI	10,AR5
    // asm: BBD5	CALL	WAIT_FOR_VBLANK		;wait for vblk
    // asm: 	DBU	AR5,BBD5
    // asm: 	RETS
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
    // asm: 	LDI	0,RS
    // asm: 	LDI	0,R0	;output
    // asm: 	LDI	80h,R1	;mask
    // asm: 	LDI	7,AR3
    // asm: ATBLP
    // asm: 	TSTB	R1,R2
    // asm: 	LDIZ	'0',R3
    // asm: 	LDINZ	'1',R3
    // asm: 	LSH	RS,R3
    // asm: 	OR	R3,R0
    // asm: 	RS	1,R1
    // asm: 	ADDI	8,RS
    // asm: 	CMPI	32,RS
    // asm: 	BLT	JJ
    // asm: 	LDI	0,RS
    // asm: 	STI	R0,*AR2++
    // asm: 	CLRI	R0
JJ:
    // asm: 	DBU	AR3,ATBLP
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*AR2++
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASCII_TO_BINARY", 0, 0);
    UNIMPL();
}

void DISPLAY_SWITCH_STATUS(void)
{
    // asm: 	CALL	DISPLAY_SHIFTER
    // asm: 	LDI	@_newbut,R0
    // asm: 	LDI	@_onebut,R1
    // asm: 	CMPI	R1,R0
    // asm: 	RETSEQ
    // asm: 	STI	R0,@_onebut
    // asm: 	FILL	130,175,140,350,0	;MAIN
    // asm: 	FILL	327,272,353,358,0	;TEST SWITCHES
    // asm: 	LDI	@SWITCHES_DIAGI,AR6
DSS:
    // asm: 	LDI	@_newbut,R0
    // asm: 	LDI	*AR6++(3),R1
    // asm: 	CMPI	-1,R1
    // asm: 	BZ	DONEDSS
    // asm: 	AND	R1,R0
    // asm: 	BNZ	DSS
    // asm: 	LDI	*-AR6(1),AR2
    // asm: 	LDI	AR2,R3
    // asm: 	ADDI	10,R3
    // asm: 	LDI	*-AR6(2),R2
    // asm: 	SUBI	2,R2
    // asm: 	LDI	R2,RC
    // asm: 	ADDI	12,RC
    // asm: 	LDI	COL_RED,RS
    // asm: 	CALL	_fill
    // asm: NOPLOT
    // asm: 	B	DSS
DONEDSS:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_SWITCH_STATUS", 0, 0);
    UNIMPL();
}

void DISPLAY_SHIFTER(void)
{
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	3C00h,R0
    // asm: 	CMPI	@LASTSHIFT,R0
    // asm: 	RETSEQ
    // asm: 	STI	R0,@LASTSHIFT
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"SHIFT 1",215,170
    // asm: 	TEXTITT	"SHIFT 2",215,240
    // asm: 	TEXTITT	"SHIFT 3",290,170
    // asm: 	TEXTITT	"SHIFT 4",290,240
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR2
    // asm: 	FILL	230,180,325,230,0
    // asm: 	POP	AR2
    // asm: 	POP	R0
    // ; 400h = 4
    // ; 800h = 3
    // ;1000h = 2
    // ;2000h = 1
    // asm: 	LDI	@_newbut,R0
    // asm: 	TSTB	2000h,R0
    // asm: 	BNZ	NOT1ST
    // asm: 	FILL	230,180,250,200,COL_RED
NOT1ST:
    // asm: 	LDI	@_newbut,R0
    // asm: 	TSTB	1000h,R0
    // asm: 	BNZ	NOT2ND
    // asm: 	FILL	230,210,250,230,COL_RED
NOT2ND:
    // asm: 	LDI	@_newbut,R0
    // asm: 	TSTB	800h,R0
    // asm: 	BNZ	NOT3RD
    // asm: 	FILL	305,180,325,200,COL_RED
NOT3RD:
    // asm: 	LDI	@_newbut,R0
    // asm: 	TSTB	400h,R0
    // asm: 	BNZ	NOT4TH
    // asm: 	FILL	305,210,325,230,COL_RED
NOT4TH:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_SHIFTER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void DISPPOTS(void)
{
    // asm: 	LDI	@_pot0,R2
    // asm: 	CMPI	@_opot0,R2
    // asm: 	BEQ	NOPLOTpot0
    // asm: 	STI	R2,@_opot0
    // asm: 	FILL	150,100,215,109,0
    // asm: 	LDI	@_pot0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	150,R2
    // asm: 	LDI	100,R3
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	CALL	_outtextxyc
NOPLOTpot0:
    // asm: 	LDI	@_pot1,R2
    // asm: 	CMPI	@_opot1,R2
    // asm: 	BEQ	NOPLOTpot1
    // asm: 	STI	R2,@_opot1
    // asm: 	FILL	150,110,215,119,0
    // asm: 	LDI	@_pot1,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	150,R2
    // asm: 	LDI	110,R3
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	CALL	_outtextxyc
NOPLOTpot1:
    // asm: 	LDI	@_pot2,R2
    // asm: 	CMPI	@_opot2,R2
    // asm: 	BEQ	NOPLOTpot2
    // asm: 	STI	R2,@_opot2
    // asm: 	FILL	150,120,215,129,0
    // asm: 	LDPI	@_pot2,R2
    // asm: 	LDPI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	150,R2
    // asm: 	LDI	120,R3
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	CALL	_outtextxyc
NOPLOTpot2:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPPOTS", 0, 0);
    UNIMPL();
}

void SHOW_DIPSWITCH(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"DIP SWITCH TEST",20,15
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@LASTDIPPLOT
    // asm: 	CALL	TOEXIT
DIPLP:
    // asm: 	CALL	DISPLAY_DIPSWITCH_STATUS
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_DIAG,R0
    // asm: 	RETSNZ
    // asm: 	CALL	WAIT_FOR_VBLANK		;wait for vblk
    // asm: 	B	DIPLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_DIPSWITCH", 0, 0);
    UNIMPL();
}

/* *
 */
void SHOW_DIPSWITCH_STATUS(void)
{
    // asm: 	LDI	7,AR5
    // asm: 	LDI	1,R4		;Index Number
    // asm: SDS_LP
    // asm: 	TSTB	80h,R7
    // asm: 	LDIZ	@ISON,AR2
    // asm: 	LDINZ	@ISOFF,AR2
    // asm: 	PUSH	R7
    // asm: 	LDI	R5,R2
    // asm: 	ADDI	20,R2
    // asm: 	LDI	R6,R3
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	R4,R2
    // asm: 	LDL	DIPTMP,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	R5,R2
    // asm: 	LDI	R6,R3
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	POP	R7
    // asm: 	LS	1,R7
    // asm: 	INC	R4
    // asm: 	ADDI	10,R6
    // asm: 	DBU	AR5,SDS_LP
    // asm: 	RETS
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
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	CMPI	@LASTDIPPLOT,R0
    // asm: 	RETSEQ
    // asm: 	STI	R0,@LASTDIPPLOT
    // 	;BANK 1
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"DIP SW2 U97",30,40
    // asm: 	GROUP	20,50,400,170
    // asm: 	FILL	22,52,398,168,0
    // asm: 	LDI	30,R5
    // asm: 	LDI	60,R6
    // asm: 	CLRI	AR2
    // asm: 	LDP	@DIPSW
    // asm: 	LDI	@DIPSW,R7
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	16,R7
    // asm: 	CALL	SHOW_DIPSWITCH_STATUS
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
    // asm: 	LDI	90,R2			;MOTION
    // asm: 	LDI	60+(1*10),R3
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_MOTION,R0
    // asm: 	LDINZ	@DNMOTI,AR2
    // asm: 	LDIZ	@DMOTI,AR2
    // asm: 	CALL	_outtextxyc
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_DIPSWITCH_STATUS", 0, 0);
    UNIMPL();
}

void RUN_MOTION_MENU(void)
{
    // asm: 	LDL	MOTION_MENU,AR5
    // asm: 	LDI	MOTION_MENU_LEN,AR6
    // asm: 	LDI	@MOMT,AR7
    // asm: 	BU	HANDLE_MENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_MOTION_MENU", 0, 0);
    UNIMPL();
}

void RUN_ADJUSTMENT_MENU(void)
{
    // asm: 	LDL	ADJUSTMENT_MENU,AR5
    // asm: 	LDI	ADJUSTMENT_MENU_LEN,AR6
    // asm: 	LDI	@AMT,AR7
    // asm: 	BU	HANDLE_MENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_ADJUSTMENT_MENU", 0, 0);
    UNIMPL();
}

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

void RUN_UTILITIES_MENU(void)
{
    // asm: 	LDL	UTILITIES_MENU,AR5
    // asm: 	LDI	UTILITIES_MENU_LEN,AR6
    // asm: 	LDI	@UMT,AR7
    // asm: 	BU	HANDLE_MENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_UTILITIES_MENU", 0, 0);
    UNIMPL();
}

void RUN_DIAG_MENU(void)
{
    // asm: 	LDL	DIAGNOSTIC_MENU,AR5
    // asm: 	LDI	DIAGNOSTIC_MENU_LEN,AR6
    // asm: 	LDI	@DMT,AR7
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_DIAG_MENU", 0, 0);
    UNIMPL();
}

void HANDLE_MENU(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@MENU_SCROLL
    // asm: 	CLRI	R6			;MENU INDEX
HMER:
    // asm: 	PUSH	R6
    // asm: 	LDI	@DIAGPAL+1,R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	PUSH	AR7
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTIT	title,30,20
    // asm: 	TEXTIT	inst1,20,260
    // asm: 	TEXTIT	inst2,20,270
    // asm: 	LDI	*-AR5,R2		;Routine that prints the value
    // asm: 	BEQ	HMSTAND
    // asm: 	LDI	*-AR5(2),R2
    // asm: 	BEQ	HMSTAND
    // asm: 	CALLU	R2
    // asm: 	BR	HMNOTSTAND
HMSTAND:
    // asm: 	IWINDOW	20,45,280,250
HMNOTSTAND:
    // asm: 	LDI	30,R2
    // asm: 	LDI	35,R3
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	POP	AR7
    // asm: 	LDI	AR7,AR2
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	10000,R7		;wait_mac counter
    // asm: 	MPYI	55,R7
    // asm: 	POP	R6
HANMENL:
    // asm: 	LDI	@MENU_SCROLL,R0
    // ;	CLRI	R0
    // ;	LDI	AR5,AR4
    // asm: 	LDI	2,AR4			;OFFSET AR4 to MENU_SCROLL
    // asm: 	MPYI	R0,AR4
    // asm: 	ADDI	AR5,AR4
    // asm: 	LDI	50,R3			;Y POSITION OF TEXT
    // asm: 	PUSH	AR7
    // asm: 	LDI	AR6,AR7
    // asm: 	SUBI	@MENU_SCROLL,AR7
    // asm: 	CMPI	20,AR7
    // asm: 	LDIGT	20,AR7
    // asm: 	DEC	AR7
    // asm: HANMENL_PLOT
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	CMPI	R0,R6
    // asm: 	LDIEQ	COL_RED,RC
    // asm: 	LDI	*AR4++(2),AR2
    // asm: 	LDI	30,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*-AR5,R5		;Routine that prints the value
    // asm: 	CALLNE	R5
    // asm: 	ADDI	10,R3
    // asm: 	INC	R0
    // asm: 	DBU	AR7,HANMENL_PLOT
    // asm: 	POP	AR7
    // asm: 	LDI	@_newbut,R0		;old system method
    // asm: 	LDI	R0,R1
    // asm: 	TSTB    SW_COINSRV,R1
    // asm: 	BNZ	JAJA5
    // ;this locks on the main menu.
    // ;fix later
    // asm: 	LDL	MAIN_MENU,AR0
    // asm: 	CMPI	AR0,AR5
    // asm: 	BEQ	JAJA5
    // asm: 	LDI	982,AR2
    // asm: 	CALL	SENDSND
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	PUSH	R6
    // asm: 	CALL	XIT_MENU_TO_MAIN
    // ;
    // ;	CALLZ	ESCAPE_TO_MAIN
JAJA5:
    // asm: 	TSTB    SW_DIAG,R1
    // asm: 	BZ	HM_EXECUTE
    // asm: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm: 	BNZ	HM_NOTUP
    // asm: 	LDI	980,AR2
    // asm: 	CALL	SENDSND
    // asm: 	ADDI	1,R6
    // asm: 	CMPI	AR6,R6
    // asm: 	BLT	HM_WT
    // asm: 	LDI	AR6,R6
    // asm: 	DEC	R6
    // asm: 	B	HM_WT
HM_NOTUP:
    // asm: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm: 	BNZ	HM_NOTDOWN
    // asm: 	LDI	981,AR2
    // asm: 	CALL	SENDSND
    // asm: 	SUBI	1,R6
    // asm: 	LDILT	0,R6
HM_WT:
    // ;	WAIT_MAC
    // asm: 	LDI	R7,R1
wta:
    // asm: 	DEC	R1
    // asm: 	BLT	wtta
    // asm: 	LDI	@_newbut,R0
    // asm: 	AND	SW_VOLPLUS|SW_VOLMINUS,R0
    // asm: 	CMPI	SW_VOLPLUS|SW_VOLMINUS,R0
    // asm: 	BNE	wta
    // asm: 	LDI	10000,R7
    // asm: 	MPYI	55,R7
    // asm: 	BU	J55
wtta:
    // asm: 	LDI	5000,R7
    // asm: 	MPYI	55,R7
J55:
HM_NOTDOWN:
    // asm: 	CMPI	0,R6
    // asm: 	BEQ	NO_SCROLL
    // asm: 	LDI	0,R2
    // asm: 	LDI	@MENU_SCROLL,R0
    // asm: 	LDI	R0,R1
    // asm: 	ADDI	20,R1
    // asm: 	CMPI	R0,R6
    // asm: 	BLT	GO_SCROLL
    // asm: 	CMPI	R1,AR6
    // asm: 	BLE	NO_SCROLL
GO_SCROLL:
    // asm: 	CMPI	R0,R6
    // asm: 	LDILT	-10,R2
    // asm: 	CMPI	R1,R6
    // asm: 	LDIGE	10,R2
    // asm: 	ADDI	R2,R0
    // asm: 	LDIN	0,R0
    // asm: 	STI	R0,@MENU_SCROLL
    // asm: 	CMPI	0,R2
    // asm: 	BNE	HMER			;REDRAW entir screen if scrolled
NO_SCROLL:
    // asm: 	B	HANMENL
HM_EXECUTE:
    // asm: 	LDI	982,AR2
    // asm: 	CALL	SENDSND
    // asm: 	LDI	R6,AR0
    // asm: 	MPYI	2,AR0
    // asm: 	ADDI	AR5,AR0
    // asm: 	LDI	*+AR0(1),R0
    // asm: 	BZ	HANMENL
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	PUSH	R6
    // asm: 	CALLU	R0
    // asm: 	POP	R6
    // asm: 	POP	AR7
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	B	HMER
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
    // asm: 	POP	R6
    // asm: 	POP	AR7
    // asm: 	POP	AR6
    // asm: 	POP	AR5
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
    // asm: 	LDL	EFFECTS_TAB,AR6
    // asm: 	LDI	-1,R5
    // asm: 	LDI	-1,R6
SBT_UPDT:
    // asm: 	CMPI	AR5,R5
    // asm: 	BEQ	NUPDT
    // asm: 	LDI	AR5,R5
    // asm: 	FILL	20,200,200,220,0
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"TUNE",20,200
    // asm: 	LDI	*AR5,AR2
    // asm: 	LDI	30,R2
    // asm: 	LDI	210,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*+AR5,AR2
    // asm: 	CALL	HARDSND
NUPDT:
    // asm: 	CMPI	AR6,R6
    // asm: 	BEQ	NUPDE
    // asm: 	LDI	AR6,R6
    // asm: 	FILL	20,250,200,270,0
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"SOUND EFFECT",20,250
    // asm: 	LDI	*AR6,AR2
    // asm: 	LDI	30,R2
    // asm: 	LDI	260,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*+AR6,AR2
    // asm: 	CALL	HARDSND
NUPDE:
SBT_LP:
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_DIAG,R0
    // asm: 	BZ	NOXIT
    // asm: 	CALL	SILENT
    // asm: 	RETS
NOXIT:
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_VOLPLUS,R0
    // asm: 	BZ	NONEXTT
SBTLP2:
    // asm: CALL	WAIT_FOR_VBLANK
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_VOLPLUS,R0
    // asm: 	BNZ	SBTLP2
    // asm: 	ADDI	2,AR5
    // asm: 	LDI	*AR5,R0
    // asm: 	BNN	SBT_UPDT
    // asm: 	LDL	MUSIC_TAB,AR5
    // asm: 	BU	SBT_UPDT
NONEXTT:
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_VOLMINUS,R0
    // asm: 	BZ	NONEXTE
SBTLP1:
    // asm: CALL	WAIT_FOR_VBLANK
    // asm: 	NOT	@_newbut,R0
    // asm: 	AND	SW_VOLMINUS,R0
    // asm: 	BNZ	SBTLP1
    // asm: 	ADDI	2,AR6
    // asm: 	LDI	*AR6,R0
    // asm: 	BNN	SBT_UPDT
    // asm: 	LDL	EFFECTS_TAB,AR6
    // asm: 	BU	SBT_UPDT
NONEXTE:
    // asm: 	LDI	7FFFh,R0
    // asm: 	MPYI	10,R0
    // asm: 	RPTS	R0
    // asm: 	NOP
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	BU	SBT_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SOUND_BOARD_TEST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void STEERING_WHEEL_TEST(void)
{
    // asm: 	CALL	SETUP_DIAG_SCREEN
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"STEERING WHEEL TEST",20,50
    // asm: 	TEXTITT	"MOVING WHEEL TO LEFT",20,100
    // asm: 	CLRI	R6
    // *	R0	BYTE TO SEND TO WHEEL
    // asm: 	LDI	127,AR5
    // asm: SWTLP1
    // asm: 	INC	R6
    // asm: 	CMPI	127,R6
    // asm: 	LDIGT	127,R6
    // asm: 	LDI	R6,R0
    // asm: 	OR	080h,R0
    // asm: 	CALL	TOWHEEL
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	DBU	AR5,SWTLP1
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"HIT ENTER TO CONTINUE",20,120
    // asm: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"MOVING WHEEL TO RIGHT",20,140
    // asm: 	CLRI	R6
    // asm: 	LDI	127,AR5
    // asm: SWTLP3
    // asm: 	INC	R6
    // asm: 	CMPI	127,R6
    // asm: 	LDIGT	127,R6
    // asm: 	LDI	R6,R0
    // asm: 	CALL	TOWHEEL
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	DBU	AR5,SWTLP3
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"HIT ENTER TO CONTINUE",20,160
    // asm: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm: 	CLRI	R0
    // asm: 	CALL	TOWHEEL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "STEERING_WHEEL_TEST", 0, 0);
    UNIMPL();
}

void AMODESND(void)
{
    // asm: 	READAUD	ADJ_ATTRACT_MODE_SOUND
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@AMODES,AR2
    // asm: 	LDI	@NULLSTR,AR3
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
    // asm: 	CALL	TEST_CHIPS
    // asm: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm: 	DINT
    // asm:         LDP	@FASTSTKI
    // asm:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm: 	LDI	AUD_SUCCESSFULL_BURNINS,AR2
    // asm: 	CALL	AUDIT_INC
    // 	;IGNORE GLITCH FIX
    // asm: 	LDP	@SWITCH1
    // asm: 	LDI	@SWITCH1,R0
    // asm: 	SETDP
    // asm: 	TSTB	SW_DIAG,R0
    // asm: 	BNZ	BURNIN_TEST
    // asm: 	BU	ENTER_DIAG
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BURNIN_TEST", 0, 0);
    UNIMPL();
}

void MAKE_STANDARW(void)
{
    // asm: 	IWINDOW	20,45,250,250
    // asm: 	IWINDOW	262,45,500,250
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_STANDARW", 0, 0);
    UNIMPL();
}

void PRINT_STANDARD(void)
{
    // asm: 	CMPI	R0,R6
    // asm: 	RETSNE
    // asm: 	CMPI	@PSLAST,R0
    // asm: 	RETSEQ
    // asm: 	STI	R0,@PSLAST
    // asm: 	CALL	PUSHALL
    // asm: 	PUSH	RC
    // asm: 	FILL	270,PSSTARTY+120,498,PSSTARTY+129,0
    // asm: 	FILL	490-24,PSSTARTY,498,PSSTARTY+149,0
    // asm: 	FILL	270,PSSTARTY-40,498,PSSTARTY-10,0
    // asm: 	CMPI	0,AR7
    // asm: 	BLE	PS_SKIPMENU
    // asm: 	LDI	COL_RED,RC
    // asm: 	LDI	R0,AR4
    // asm: 	ADDI	@COUNTRY,AR4
    // asm: 	MPYI	COIN_ENTRY_SIZE,AR4
    // asm: 	ADDI	@COIN_TABLEI,AR4
    // asm: 	ADDI	3,AR4
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	270,R2
    // asm: 	LDI	PSSTARTY-40,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	DONE_MSG
    // asm: 	LDI	270,R2
    // asm: 	LDI	PSSTARTY-30,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	DONE_MSG
    // asm: 	LDI	270,R2
    // asm: 	LDI	PSSTARTY-20,R3
    // asm: 	CALL	_outtextxyc
DONE_MSG:
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTITT	"DETAILED PRICING INFORMATION",270,PSSTARTY-50
    // asm: 	TEXTITT	"LEFT CHUTE UNITS",270,PSSTARTY+0
    // asm: 	TEXTITT	"RIGHT CHUTE UNITS",270,PSSTARTY+10
    // asm: 	TEXTITT	"CENTER CHUTE UNITS",270,PSSTARTY+20
    // asm: 	TEXTITT	"FOURTH CHUTE UNITS",270,PSSTARTY+30
    // asm: 	TEXTITT	"UNITS PER CREDIT",270,PSSTARTY+40
    // asm: 	TEXTITT	"UNITS FOR BONUS",270,PSSTARTY+50
    // asm: 	TEXTITT	"MINIMUM UNITS",270,PSSTARTY+60
    // asm: 	TEXTITT	"CREDITS TO START",270,PSSTARTY+70
    // asm: 	TEXTITT	"CREDITS TO CONTINUE",270,PSSTARTY+80
    // asm: 	TEXTITT	"LEFT  CENTER  RIGHT  FOURTH",270,PSSTARTY+100
    // asm: 	TEXTITT	"PRICING SELECTED",270,PSSTARTY+140
PS_SKIPMENU:
    // asm: 	POP	RC
    // asm: 	CMPI	0,AR7
    // asm: 	BLE	PSLPX
    // asm: 	LDI	R0,AR4
    // asm: 	ADDI	@COUNTRY,AR4
    // asm: 	MPYI	COIN_ENTRY_SIZE,AR4
    // asm: 	ADDI	@COIN_TABLEI,AR4
    // asm: 	LDI	*AR4++,R4
    // asm: 	LDI	490,R2	;X POS
    // asm: 	LDI	PSSTARTY,R3	;Y POS START OF TEXT
    // ;	PUSH	AR2
    // ;First print the SLOT units
PSLP:
    // asm: 	LDI	R4,R0
    // asm: 	AND	0FFh,R0
    // asm: 	PUSH	R2
    // asm: 	PUSH	RC
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	POP	RC
    // asm: 	POP	R2
    // asm: 	PUSH	R2		;PRESERVE R2 for LATER USE
    // asm: 	LDI	0,R1
    // asm: 	CMPI	10,R0
    // asm: 	LDIGE	-8,R1		;Right justify
    // asm: 	CMPI	100,R0
    // asm: 	LDIGE	-16,R1		;Right justify
    // asm: 	ADDI	R1,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	POP	R2
    // asm: 	LSH	-8,R4
    // asm: 	ADDI	10,R3
    // asm: 	CMPI	PSSTARTY+40,R3
    // asm: 	BLT	PSLP
    // asm: 	CMPI	PSSTARTY+80,R3
    // asm: 	BEQ	PSLP1
    // asm: 	CMPI	PSSTARTY+40,R3
    // asm: 	BGT	PSLP1_CHECK
PSLP1:
    // ;	POP	AR2
    // asm: 	LDI	*AR4++,R4
    // ;	PUSH	AR2
PSLP1_CHECK:
    // asm: 	CMPI	PSSTARTY+90,R3
    // asm: 	BLT	PSLP
    // ;	POP	AR4
    // asm: 	ADDI	3,AR4
    // ;Print LEFT slot denomination
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	PSSTARTY+120,R3
    // asm: 	LDI	270,R2
    // asm: 	CALL	_outtextxyc
    // ;Print RIGHT slot denomination
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	270+112,R2
    // asm: 	CALL	_outtextxyc
    // ;Print CENTER slot denomination
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	270+56,R2
    // asm: 	CALL	_outtextxyc
    // ;Print FOURTH slot denomination
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	270+176,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	R6,R1
    // asm: 	ADDI	@COUNTRY,R1
    // asm: 	READADJ	ADJ_COINMODE
    // asm: 	CMPI	R0,R1
    // asm: 	LDIEQ	1,R0
    // asm: 	LDINE	0,R0
    // asm: 	LDI	PSSTARTY+140,R3
    // asm: 	CALL	PRINT_SPYN
PSLPX:
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_STANDARD", 0, 0);
    UNIMPL();
}

void PRINT_SPYN(void)
{
    // asm: 	LDI	@YES,AR2
    // asm: 	LDI	474,R2
    // asm: 	CMPI	0,R0
    // asm: 	LDIEQ	@NO,AR2
    // asm: 	LDIEQ	482,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_SPYN", 0, 0);
    UNIMPL();
}

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
    // asm: 	SETADJ	ADJ_GASMIN
    // asm: 	LDI	@_pot2,R2
    // asm: 	SETADJ	ADJ_BRAKEMIN
    // 	;get wheel center
    // asm: 	LDI	@_pot0,R2
    // asm: 	SETADJ	ADJ_STEERCENTER
    // asm: 	CALL	CLRSCRN
    // 	;turn wheel to left and hit sw1
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTIT	sct,150,10
    // asm: 	TEXTITT	"TURN WHEEL ALL THE WAY TO THE LEFT AND HIT ENTER",50,40
    // asm: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm: 	LDI	@_pot0,R2
    // asm: 	SETADJ	ADJ_STEERMIN
    // asm: 	CALL	CLRSCRN
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTIT	sct,150,10
    // asm: 	TEXTITT	"TURN WHEEL ALL THE WAY TO THE RIGHT AND HIT ENTER",50,40		;turn wheel to right
    // asm: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm: 	LDI	@_pot0,R2
    // asm: 	SETADJ	ADJ_STEERMAX
    // asm: 	CALL	CLRSCRN
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTIT	sct,150,10
    // 	;"push gas pedal all the way and hit sw1"
    // asm: 	TEXTITT	"PUSH GAS PEDAL TO MAX AND HIT ENTER",50,40
    // asm: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm: 	LDI	@_pot1,R2
    // asm: 	SETADJ	ADJ_GASMAX
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_STANDUP,R0
    // asm: 	BNZ	DADA5
    // asm: 	CALL	CLRSCRN
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTIT	sct,150,10
    // asm: 	TEXTITT	"PUSH BRAKE PEDAL TO MAX AND HIT ENTER",50,40
    // asm: 	CALL	WT_NSTART
    // asm: 	CALL	WT_START
    // asm: 	CALL	BEEP
    // asm: 	LDI	@_pot2,R2
    // asm: 	SETADJ	ADJ_BRAKEMAX
    // asm: 	CALL	WT_NSTART
DADA5:
    // asm: 	RETS
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
    // asm: 	TEXTITT	"MIN",118,135
    // asm: 	TEXTITT	"MAX",360,135
    // asm: 	TEXTITT	"ENTER  EXIT",130,200
    // asm: 	TEXTITT	"UP     INCREASE VOLUME",130,210
    // asm: 	TEXTITT	"DOWN   DECREASE VOLUME",130,220
    // asm: 	READADJ	ADJ_VOLUME
    // asm: 	LDI	R0,R4
    // asm: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm: 	CMPI	R0,R4
    // asm: 	LDILT	R0,R4
    // asm: 	CALL	SHOW_VOLUME
    // asm: 	SOND1	MUNSTER_SURF
    // asm: 	SOND1	ENGINESTART
SETVOL_LP:
    // asm: 	LDI	700,AR2
    // asm: 	CALL	RANDU0
    // asm: 	LDI	R0,R0
    // asm: 	BNZ	SETNN
    // asm: 	LDI	6,AR2
    // asm: 	CALL	RANDU0
    // asm: 	LDPI	@INCIDENTI,AR2
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	ONESND
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_VOLUME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FULL_FACTORY_RESTORE(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void DEFAULT_ADJUSTMENTS(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RESET_HIGH_SCORES(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void CLEAR_GAME_AUDITS(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void FREEPLAYONOFF(void)
{
    // asm: 	READAUD	ADJ_FREE_PLAY
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@FREEPLAYT,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_FREE_PLAY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREEPLAYONOFF", 0, 0);
    UNIMPL();
}

void FREEGAMEONOFF(void)
{
    // asm: 	READAUD	ADJ_FREEGAME
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@FGOO1,AR2
    // asm: 	LDI	@FGOO2,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_FREEGAME
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREEGAMEONOFF", 0, 0);
    UNIMPL();
}

void HIGHSCORE_ENTRY(void)
{
    // asm: 	READAUD	ADJ_HIGH_SCORE_ENTRY
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@APHSTD,AR2
    // asm: 	LDI	@APHSTD2,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_HIGH_SCORE_ENTRY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIGHSCORE_ENTRY", 0, 0);
    UNIMPL();
}

void GIRLS_ONOFF(void)
{
    // asm: 	READAUD	ADJ_GIRLS
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@GI1,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_GIRLS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GIRLS_ONOFF", 0, 0);
    UNIMPL();
}

void CLINTON_ONOFF(void)
{
    // asm: 	READAUD	ADJ_CLINTON
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@CL1,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_CLINTON
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLINTON_ONOFF", 0, 0);
    UNIMPL();
}

void ROADKILL_ONOFF(void)
{
    // asm: 	READAUD	ADJ_ROADKILL
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@RK1,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	DUAL_ON_OFF
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_ROADKILL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADKILL_ONOFF", 0, 0);
    UNIMPL();
}

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
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_CREDITS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MESSAGE_BOX(void)
{
    // asm: 	PUSH	AR2
    // asm: 	WINDOW	110,115,370,285
    // asm: 	IWINDOW	120,125,360,275
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	POP	AR2
    // asm: 	LDI	130,R2
    // asm: 	LDI	170,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	30,AR6
ABLP:
    // asm: CALL	WAIT_FOR_VBLANK
    // asm: 	DBU	AR6,ABLP
    // asm: 	RETS
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_YES_NO", 0, 0);
    UNIMPL();
}

void ASK_YES_NO(void)
{
    // asm: 	LDI	@YES,AR4
    // asm: 	LDI	@NO,AR5
    // asm: 	BU	DUAL_VALUE
    // *	R5	DEFAULT
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
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR2
    // asm: 	WAIT_MAC_NOREPEAT
    // asm: 	WINDOW	110,115,370,285
    // asm: 	IWINDOW	120,125,360,275
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	POP	AR2
    // asm: 	LDI	130,R2
    // asm: 	LDI	140,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	POP	AR2
    // asm: 	LDI	130,R2
    // asm: 	LDI	150,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	TEXTITT	"UP TO CHANGE",130,235
    // asm: 	TEXTITT	"ENTER TO ACCEPT",130,245
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	POP	AR7
    // asm: 	POP	AR6
CYNLOOP:
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	CMPI	1,R5
    // asm: 	LDIEQ	COL_RED,RC
    // asm: 	LDI	AR6,AR2
    // asm: 	LDI	170,R2
    // asm: 	LDI	200,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	CMPI	0,R5
    // asm: 	LDIEQ	COL_RED,RC
    // asm: 	LDI	AR7,AR2
    // asm: 	LDI	220,R2
    // asm: 	LDI	200,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	@_newbut,R0		;old system method
    // asm: 	TSTB    SW_DIAG,R0
    // asm: 	BZ	DOEXIT
    // asm: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm: 	BZ	CHNG
    // asm: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm: 	BZ	NCHNG
    // asm: 	BU	NCHNG
CHNG:
    // asm: 	XOR	1,R5
    // asm: 	SOND1	WPC_MISC2
NCHNG:
    // asm: 	WAIT_MAC_NOREPEAT
    // asm: 	B	CYNLOOP
DOEXIT:
    // asm: 	CMPI	0,R5
    // asm: 	BEQ	CYN_YES
    // asm: CYN_NO
    // asm: 	CLRC
    // asm: 	RETS
CYN_YES:
    // asm: 	SETC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DUAL_ON_OFF", 0, 0);
    UNIMPL();
}

void SET_STEERING_SENSITIVITY(void)
{
    // asm: 	READAUD	ADJ_STEERING_SENSITIVITY
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@SSGD,AR2
    // asm: 	LDI	@SSGD2,AR3
    // asm: 	LDI	0,R6
    // asm: 	LDI	5,R7
    // asm: 	LDI	@GAMEDIFF_CVTI,AR4
    // asm: 	LDI	1,R4
    // asm: 	CALL	MULTI_VALUE_STEP
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_STEERING_SENSITIVITY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_STEERING_SENSITIVITY", 0, 0);
    UNIMPL();
}

void SET_HIGHSCORE_RESET(void)
{
    // asm: 	READAUD	ADJ_HIGHSCORE_RESET
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@HSGD,AR2
    // asm: 	LDI	@HSGD2,AR3
    // asm: 	LDI	1000,R6
    // asm: 	LDI	25000,R7
    // asm: 	LDI	@GAMEDIFF_CVTI,AR4
    // asm: 	LDI	1000,R4
    // asm: 	CALL	MULTI_VALUE_STEP
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_HIGHSCORE_RESET
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_HIGHSCORE_RESET", 0, 0);
    UNIMPL();
}

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
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_DIFFICULTY
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
    // asm: 	LDI	R5,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@bufferI,AR2
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GAMEDIFF_CVT", 0, 0);
    UNIMPL();
}

void SET_TIME_TO_START(void)
{
    // asm: 	READAUD	ADJ_TIME_TO_START
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@ITD,AR2
    // asm: 	LDI	@ITD2,AR3
    // asm: 	CLRI	R6
    // asm: 	LDI	6,R7
    // asm: 	LDI	@TIME_CVTI,AR4
    // asm: 	CALL	MULTI_VALUE
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_TIME_TO_START
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_TIME_TO_START", 0, 0);
    UNIMPL();
}

/* *
*
 */
void TIME_CVT(void)
{
    // asm: 	PUSH	R2
    // asm: 	LDI	R5,R2
    // asm: 	MPYI	5,R2
    // asm: 	ADDI	60,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@bufferI,AR2
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIME_CVT", 0, 0);
    UNIMPL();
}

void SET_CHECKPOINT_BONUS(void)
{
    // asm: 	READAUD	ADJ_CHECKPOINT_BONUS
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@CPBTT,AR2
    // asm: 	LDI	@CPBTT2,AR3
    // asm: 	LDI	10,R6
    // asm: 	LDI	25,R7
    // asm: 	LDI	@CHECKPOINT_BONUS_CVTI,AR4
    // asm: 	CALL	MULTI_VALUE
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_CHECKPOINT_BONUS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_CHECKPOINT_BONUS", 0, 0);
    UNIMPL();
}

void CHECKPOINT_BONUS_CVT(void)
{
    // asm: 	PUSH	R2
    // asm: 	LDI	R5,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@bufferI,AR2
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKPOINT_BONUS_CVT", 0, 0);
    UNIMPL();
}

void SET_MAX_CREDITS(void)
{
    // asm: 	READAUD	ADJ_MAX_CREDITS
    // asm: 	LDI	R0,R5
    // asm: 	LDI	@MXT1,AR2
    // asm: 	LDI	@MXT2,AR3
    // asm: 	LDI	10,R6
    // asm: 	LDI	50,R7
    // asm: 	LDI	@CHECKPOINT_BONUS_CVTI,AR4
    // asm: 	CALL	MULTI_VALUE
    // asm: 	LDI	R5,R2
    // asm: 	SETADJ	ADJ_MAX_CREDITS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MAX_CREDITS", 0, 0);
    UNIMPL();
}

void SET_MIN_VOL_LEVEL(void)
{
    // asm: 	READAUD	ADJ_MIN_VOL_LEVEL
    // asm: 	FLOAT	R0,R5
    // asm: 	MPYF	0.11764,R5
    // asm: 	FIX	R5
    // asm: 	LDI	@MVL,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CLRI	R6
    // asm: 	LDI	30,R7
    // asm: 	LDI	@SET_MIN_VOL_LEVEL_CVTI,AR4
    // asm: 	CALL	MULTI_VALUE
    // asm: 	FLOAT	R5
    // asm: 	MPYF	8.5,R5
    // asm: 	FIX	R5,R2
    // asm: 	SETADJ	ADJ_MIN_VOL_LEVEL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MIN_VOL_LEVEL", 0, 0);
    UNIMPL();
}

/* *
*
 */
void SET_MIN_VOL_LEVEL_CVT(void)
{
    // asm: 	PUSH	R2
    // asm: 	LDI	R5,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@bufferI,AR2
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_MIN_VOL_LEVEL_CVT", 0, 0);
    UNIMPL();
}

void MULTI_VALUE(void)
{
    // asm: 	LDI	1,R4
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
    // asm: 	MPYI	55,R0
    // asm: 	STI	R0,@REPEAT_TIME
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@REPEAT_SW
    // asm: 	WINDOW	110,115,320,285
    // asm: 	IWINDOW	120,125,310,275
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	POP	AR2
    // asm: 	LDI	130,R2
    // asm: 	LDI	140,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	POP	AR2
    // asm: 	LDI	130,R2
    // asm: 	LDI	150,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	TEXTITT	"ENTER TO ACCEPT",130,225
    // asm: 	TEXTITT	"UP   TO INCREASE",130,235
    // asm: 	TEXTITT	"DOWN   TO DECREASE",130,245
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	POP	AR7
    // asm: 	POP	AR6
    // asm: 	POP	R7
    // asm: 	POP	R6
    // asm: 	POP	R5
DVLOOP:
    // asm: 	CMPI	R6,R5
    // asm: 	LDILT	R6,R5
    // asm: 	CMPI	R7,R5
    // asm: 	LDIGT	R7,R5
    // asm: 	FILL	170,200,309,209,0
    // asm: 	LDI	AR4,R0
    // asm: 	CALLU	R0		;VALUE PASSED IN R5 (ALL REGS PRESERVED)
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	LDI	170,R2
    // asm: 	LDI	200,R3
    // asm: 	CALL	_outtextxyc
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	LDI	@_newbut,R0	;old system method
    // asm: 	TSTB    SW_DIAG,R0
    // asm: 	BZ	DOEXITDV
    // asm: 	TSTB	SW_VOLPLUS,R0		;move down
    // asm: 	BZ	CHNGDV
    // asm: 	TSTB	SW_VOLMINUS,R0		;move up
    // asm: 	BNZ	NCHNGDV
    // asm: 	SUBI	R4,R5
    // asm: 	LDI	980,AR2
    // asm: 	CALL	SENDSND
    // asm: 	BU	NCHNGDV
CHNGDV:
    // asm: 	ADDI	R4,R5
    // asm: 	LDI	981,AR2
    // asm: 	CALL	SENDSND
NCHNGDV:
    // asm: 	LDI	@REPEAT_TIME,R1
WT5:
    // asm: 	DEC	R1
    // asm: 	BLT	WTT5
    // asm: 	LDI	@_newbut,R0
    // asm: 	AND	SW_VOLPLUS|SW_VOLMINUS|SW_DIAG,R0
    // asm: 	CMPI	@REPEAT_SW,R0
    // asm: 	BEQ	WT5
WTT5:
    // asm: 	CMPI	@REPEAT_SW,R0
    // asm: 	BNE	WTTT5
    // asm: 	LDI	5000,R1
    // asm: 	MPYI	55,R1
    // asm: 	STI	R1,@REPEAT_TIME
    // asm: 	B	DVLOOP
WTTT5:
    // asm: 	LDI	10000,R1
    // asm: 	MPYI	55,R1
    // asm: 	STI	R1,@REPEAT_TIME
    // asm: 	STI	R0,@REPEAT_SW
    // asm: 	B	DVLOOP
DOEXITDV:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MULTI_VALUE_STEP", 0, 0);
    UNIMPL();
}

void RUN_STANDARD_PRICING(void)
{
    // asm: 	LDL	STANDARD_PRICING_MENU,AR5
    // asm: 	LDI	STANDARD_PRICING_MENU_LEN,AR6
    // asm: 	LDI	@SPM,AR7
    // asm: 	BU	HANDLE_MENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_STANDARD_PRICING", 0, 0);
    UNIMPL();
}

void PRICE_YES_NO(void)
{
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@PSLAST		;JUMP START PRINT STANDARD
    // asm: 	LDL	SELTP,AR2
    // asm: 	LDI	@AREUSURE,AR3
    // asm: 	PUSH	R6
    // asm: 	CALL	CLEAR_YES_NO
    // asm: 	POP	R6
    // asm: 	RETSC
    // asm: 	LDI	R6,R2
    // asm: 	ADDI	@COUNTRY,R2
    // asm: 	LDI	ADJ_COINMODE,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // asm: 	LDI	1,R2
    // asm: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // 	;IF YOU CHANGE MODES YOU RESET THE CREDITS
    // 	;
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_PCREDITS,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_CREDITS,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // 	;NOTE: AUD_BCREDITS RESET DURING GAME INIT
    // asm: 	CALL	SET_COINAGE_ADJ		;Set the custom coinage to the standard just selected
    // asm: 	LDI	@STP,AR2
    // asm: 	CALL	MESSAGE_BOX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRICE_YES_NO", 0, 0);
    UNIMPL();
}

void RUN_CUSTOM_PRICING(void)
{
    // asm: 	LDL	CUSTOM_PRICING_MENU,AR5
    // asm: 	LDI	CUSTOM_PRICING_MENU_LEN,AR6
    // asm: 	LDI	@RCMT,AR7
    // asm: 	BU	HANDLE_MENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_CUSTOM_PRICING", 0, 0);
    UNIMPL();
}

void FRACTION_YES_NO(void)
{
    // asm: 	READAUD	ADJ_SHOW_FRAC
    // asm: 	LDI	R0,R5
    // asm: 	LDL	FRACYN,AR2
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

void CUSTOM_YES_NO(void)
{
    // asm: 	READAUD	ADJ_STANDARD_PRICING
    // asm: 	LDI	R0,R5
    // asm: 	XOR	1,R5
    // asm: 	LDL	USECP,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	CALL	ASK_YES_NO
    // asm: 	LDI	0,R2
    // asm: 	LDIC	1,R2
    // asm: 	PUSH	R2
    // asm: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // asm: 	POP	R2
    // asm: 	CMPI	1,R2
    // asm: 	BEQ	CUSTOM_NO
    // asm:  	LDI	@CPS,AR2
    // asm: 	CALL	MESSAGE_BOX
CUSTOM_NO:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CUSTOM_YES_NO", 0, 0);
    UNIMPL();
}

void SET_SLOT(void)
{
    // asm: 	LDI	R6,AR2			;R6 = menu selection index
    // asm: 	LDI	ADJ_COIN1_UNITS,R0
    // asm: 	CMPI	9,R6
    // asm: 	BLT	SET_SLOT1
    // asm: 	LDI	ADJ_COIN1_COUNTER,R0
    // asm: 	SUBI	9,R0			;STRIP THE MENU OFFSET
SET_SLOT1:
    // asm: 	ADDI	R0,AR2
    // asm: 	CALL	ADJUSTMENT_READ
    // asm: 	LDI	R0,R5
    // asm: 	LDL	CMESSAGE_TAB,AR2
    // asm: 	ADDI	R6,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	LDI	@NULLSTR,AR3
    // asm: 	PUSH	R6
    // asm: 	LDI	0,R6			;RANGE 0 to 100
    // asm: 	LDI	100,R7
    // asm: 	LDI	@GAMEDIFF_CVTI,AR4	;This will print the values as numbers
    // asm: 	CALL	MULTI_VALUE
    // asm: 	POP	R6
    // asm: 	LDI	R5,R2
    // asm: 	LDI	R6,AR2
    // asm: 	LDI	ADJ_COIN1_UNITS,R0
    // asm: 	CMPI	9,R6
    // asm: 	BLT	SET_SLOT2
    // asm: 	LDI	ADJ_COIN1_COUNTER,R0
    // asm: 	SUBI	9,R0			;STRIP THE MENU OFFSET
SET_SLOT2:
    // asm: 	ADDI	R0,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // ;IF YOU CHANGE MODES YOU RESET THE CREDITS
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_PCREDITS,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_CREDITS,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_SLOT", 0, 0);
    UNIMPL();
}

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
    // asm: 	MPYI	2,R0
    // asm: 	ADDI	R0,AR4
    // asm: 	LDI	*AR4,AR2
    // asm: 	CALL	ADJUSTMENT_READ
    // ;	LDI	*+AR4,AR2
    // ;	ADDI	@PVTABI,AR2
    // ;	LDI	*AR2,R1
    // ;	BEQ	PRINT_VALUEX
    // asm: 	LDI	*+AR4,R1
    // asm: 	BEQ	PRINT_VALUEX
    // asm: 	CALLU	R1
PRINT_VALUEX:
    // asm: 	POP	AR2
    // asm: 	POP	R2
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
    // asm: 	LDIGE	262,R2
    // asm: 	CMPI	100,R0
    // asm: 	LDIGE	254,R2
    // asm: 	POP	RC
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_VOL", 0, 0);
    UNIMPL();
}

void GETSTR_LENGTH(void)
{
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R3
    // asm: 	CLRI	R2
GSLP:
    // asm: 	CLRI	R1
    // asm: 	LDI	*AR2++,R3
GSLP1:
    // asm: 	LDI	R3,R0
    // asm: 	LSH	R1,R0
    // asm: 	AND	0FFH,R0
    // asm: 	BEQ	GSLDONE
    // asm: 	ADDI	1,R2
    // asm: 	SUBI	8,R1
    // asm: 	CMPI	-32,R1
    // asm: 	BGT	GSLP1
    // asm: 	BR	GSLP
GSLDONE:
    // asm: 	POP	R3
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSTR_LENGTH", 0, 0);
    UNIMPL();
}

void PRINT_COINMODE(void)
{
    // asm: 	PUSH	R0
    // asm: 	READADJ	ADJ_STANDARD_PRICING
    // asm: 	POP	R2
    // asm: 	CMPI	0,R0
    // asm: 	RETSEQ
    // asm: 	ADDI	@COINMSGC_TABI,R2
    // asm: 	LDI	R2,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	GETSTR_LENGTH
    // asm: 	MPYI	-8,R2
    // asm: 	ADDI	278,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_COINMODE", 0, 0);
    UNIMPL();
}

void PRINT_CUSTOM_SELECTED(void)
{
    // asm: 	CMPI	1,R0
    // asm: 	RETSEQ
    // asm: 	LDI	@ISON,AR2
    // asm: 	LDI	262,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_CUSTOM_SELECTED", 0, 0);
    UNIMPL();
}

void PRINT_TIME(void)
{
    // asm: 	MPYI	5,R0
    // asm: 	ADDI	60,R0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_TIME", 0, 0);
    UNIMPL();
}

void PRINT_NUMBER(void)
{
    // asm: 	PUSH	RC
    // asm: 	LDI	R0,R2
    // asm: 	LDI	@bufferI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@bufferI,AR2
    // asm: 	LDI	270,R2
    // asm: 	CMPI	10,R0
    // asm: 	LDIGE	262,R2
    // asm: 	CMPI	100,R0
    // asm: 	LDIGE	254,R2
    // asm: 	CMPI	1000,R0
    // asm: 	LDIGE	246,R2
    // asm: 	CMPI	10000,R0
    // asm: 	LDIGE	238,R2
    // asm: 	PUSH	R1
    // asm: 	LDI	10000,R1
    // asm: 	MPYI	10,R1
    // asm: 	CMPI	R1,R0
    // asm: 	LDIGE	230,R2
    // asm: 	POP	R1
    // asm: 	POP	RC
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_NUMBER", 0, 0);
    UNIMPL();
}

void PRINT_YN_INV(void)
{
    // asm: 	NEGI	R0
    // asm: 	ADDI	1,R0	;Drops through to PRINT_YN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_YN_INV", 0, 0);
    UNIMPL();
}

void PRINT_YN(void)
{
    // asm: 	LDI	@YES,AR2
    // asm: 	LDI	254,R2
    // asm: 	CMPI	0,R0
    // asm: 	LDIEQ	@NO,AR2
    // asm: 	LDIEQ	262,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_YN", 0, 0);
    UNIMPL();
}

void PRINT_ONOFF_INV(void)
{
    // asm: 	NEGI	R0
    // asm: 	ADDI	1,R0	;Drops through to PRINT_ONOFF
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_ONOFF_INV", 0, 0);
    UNIMPL();
}

void PRINT_ONOFF(void)
{
    // asm: 	LDI	@ISON,AR2
    // asm: 	LDI	262,R2
    // asm: 	CMPI	0,R0
    // asm: 	LDIEQ	@ISOFF,AR2
    // asm: 	LDIEQ	254,R2
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_ONOFF", 0, 0);
    UNIMPL();
}

void PRINT_MPH_INV(void)
{
    // asm: 	NEGI	R0
    // asm: 	ADDI	1,R0	;Drops through to PRINT_MPH
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_MPH_INV", 0, 0);
    UNIMPL();
}

void PRINT_MPH(void)
{
    // asm: 	LDI	@MP3,AR2
    // asm: 	LDI	254,R2
    // asm: 	CMPI	1,R0
    // asm: 	LDIEQ	@MP2,AR2
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_MPH", 0, 0);
    UNIMPL();
}

void MAIN_MENU(void)
{
    // asm: 	MENUENTRY	"DIAGNOSTIC MENU",RUN_DIAG_MENU
    // asm: 	MENUENTRY	"GAME AUDITS",AUDIT_DISPLAY
    // asm: 	MENUENTRY	"ADJUSTMENTS MENU",RUN_ADJUSTMENT_MENU
    // asm: 	MENUENTRY	"UTILITIES",RUN_UTILITIES_MENU
    // asm: 	MENUENTRY	"CALIBRATE CONTROLS",SET_CONTROLS
    // asm: 	MENUENTRY	"ADJUST VOLUME",SET_VOLUME
    // asm: 	MENUENTRY	"EXIT TO GAME OVER",DIAG_RETURN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAIN_MENU", 0, 0);
    UNIMPL();
}

void DIAGNOSTIC_MENU(void)
{
    // asm: 	MENUENTRY	"SWITCH TEST",SHOWSWITCH
    // asm: 	MENUENTRY	"DIP SWITCH TEST",SHOW_DIPSWITCH
    // asm: 	MENUENTRY	"CPU BOARD TEST",TEST_CHIPS
    // asm: 	MENUENTRY	"SOUND BOARD TEST",SOUND_BOARD_TEST
    // asm: 	MENUENTRY	"STEERING WHEEL TEST",STEERING_WHEEL_TEST
    // asm: 	MENUENTRY	"MONITOR TEST",MONITOR_DIAGNOSTICS
    // asm: 	MENUENTRY	"BURNIN TEST",BURNIN_TEST
    // asm: 	MENUENTRY	"MOTION MENU",RUN_MOTION_MENU		;MOTION_BURNIN
    // asm: 	MENUENTRY	"LAMP TEST",RUN_LAMP_TEST
    // asm: 	MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAGNOSTIC_MENU", 0, 0);
    UNIMPL();
}

void UTILITIES_MENU(void)
{
    // asm: 	MENUENTRY	"CLEAR CREDITS",CLEAR_CREDITS
    // asm: 	MENUENTRY	"CLEAR GAME AUDITS",CLEAR_GAME_AUDITS
    // asm: 	MENUENTRY	"RESET HIGH SCORES",RESET_HIGH_SCORES
    // asm: 	MENUENTRY	"DEFAULT ADJUSTMENTS",DEFAULT_ADJUSTMENTS
    // asm: 	MENUENTRY	"FULL FACTORY RESTORE",FULL_FACTORY_RESTORE
    // asm: 	MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UTILITIES_MENU", 0, 0);
    UNIMPL();
}

void LAMP_MENU(void)
{
    // asm: 	MENUENTRY	"TURN ON ALL LAMPS",LAMPSALL
    // asm: 	MENUENTRY	"TURN OFF ALL LAMPS",LAMPSOFF
    // asm: 	MENUENTRY	"TURN ON START",LAMPS
    // asm: 	MENUENTRY	"TURN ON VIEW 1",LAMPV1
    // asm: 	MENUENTRY	"TURN ON VIEW 2",LAMPV2
    // asm: 	MENUENTRY	"TURN ON VIEW 3",LAMPV3
    // asm: 	MENUENTRY	"TURN ON LEFT TAIL",LAMPLT
    // asm: 	MENUENTRY	"TURN ON RIGHT TAIL",LAMPRT
    // asm: 	MENUENTRY	"TURN ON LEFT FRONT",LAMPLF
    // asm: 	MENUENTRY	"TURN ON RIGHT FRONT",LAMPRF
    // asm: 	MENUENTRY	"EXIT TO MAIN MENU",XIT_MENU_TO_MAIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMP_MENU", 0, 0);
    UNIMPL();
}

void RUN_LAMP_TEST(void)
{
    // asm: 	LDL	LAMP_MENU,AR5
    // asm: 	LDI	LAMP_MENU_LEN,AR6
    // asm: 	LDI	@LAMPT,AR7
    // asm: 	BU	HANDLE_MENU
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUN_LAMP_TEST", 0, 0);
    UNIMPL();
}

void LAMPSOFF(void)
{
    // asm: 	LDI	0,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPSOFF", 0, 0);
    UNIMPL();
}

void LAMPSALL(void)
{
    // asm: 	LDI	0FFh,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPSALL", 0, 0);
    UNIMPL();
}

void LAMPS(void)
{
    // asm: LDI	1,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPS", 0, 0);
    UNIMPL();
}

void LAMPV1(void)
{
    // asm: LDI	2,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV1", 0, 0);
    UNIMPL();
}

void LAMPV2(void)
{
    // asm: LDI	4,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV2", 0, 0);
    UNIMPL();
}

void LAMPV3(void)
{
    // asm: LDI	8,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPV3", 0, 0);
    UNIMPL();
}

void LAMPLT(void)
{
    // asm: LDI	10h,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPLT", 0, 0);
    UNIMPL();
}

void LAMPRT(void)
{
    // asm: LDI	20h,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPRT", 0, 0);
    UNIMPL();
}

void LAMPLF(void)
{
    // asm: LDI	40h,R0
    // asm: 	BU	LJIN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPLF", 0, 0);
    UNIMPL();
}

void LAMPRF(void)
{
    // asm: LDI	80h,R0
LJIN:
    // asm: LDP	@BUTTON_STATUS
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LAMPRF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MOTION_CHECK_SAFETY_CODES(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	CALL	CLEAR_MATS
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R0
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R0		;right justify
    // asm: 	LDL	MOTION_SAFETY_CODES,AR2
MCSC_LP:
    // asm: LDI	*AR2,R1
    // asm: 	CMPI	-1,R1
    // asm: 	BEQ	NO_ERRORS
    // asm: 	AND	*+AR2(1),R1
    // asm: 	LDI	R0,R3
    // asm: 	AND	*+AR2(1),R3
    // asm: 	CMPI	R3,R1
    // asm: 	BNE	JJDDA
    // asm: 	LDI	*+AR2(2),R0
    // asm: 	CALLU	R0
    // asm: 	BU	ALL_DONE
JJDDA:
    // asm: NOP	*++AR2(3)
    // asm: 	BU	MCSC_LP
NO_ERRORS:
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"ALL SYSTEMS OK",70,330
ALL_DONE:
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_CHECK_SAFETY_CODES", 0, 0);
    UNIMPL();
}

void MOTION_SAFETY_CODES(void)
{
    // asm: 	MSCM	080h,080h,MSC_BOARD_NOTPLUGGED
    // asm: 	MSCM	070h,0F0h,MSC_FAILSAFE_NOTCONNECTED
    // asm: 	MSCM	060h,0F0h,MSC_FAILSAFE_OK
    // asm: 	MSCM	050h,0F0h,MSC_OPTO_LED
    // asm: 	MSCM	040h,0F0h,MSC_OPTO_DETECTOR
    // asm: 	MSCM	030h,0F0h,MSC_OPTO_PATH
    // asm: 	MSCM	020h,0F0h,MSC_MAT_STEPPED
    // asm: 	MSCM	010h,0F0h,MSC_MAT_NOTPLUGGED
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_SAFETY_CODES", 0, 0);
    UNIMPL();
}

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
    // asm: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	1,R4
    // asm: 	BZ	NM1
    // asm: 	FILL	102,142,128,218,COL_RED
NM1:
    // asm: TSTB	2,R4
    // asm: 	BZ	NM2
    // asm: 	FILL	32,232,128,278,COL_RED
NM2:
    // asm: TSTB	4,R4
    // asm: 	BZ	NM3
    // asm: 	FILL	32,142,58,218,COL_RED
NM3:
    // asm: TSTB	8,R4
    // asm: 	BZ	NM4
    // asm: 	FILL	32,82,128,118,COL_RED
NM4:
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_MAT_STEPPED", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_MAT_NOTPLUGGED(void)
{
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"MAT NOT PLUGGED IN",70,330
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R4
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	1,R4
    // asm: 	BZ	NM1A
    // asm: 	FILL	102,142,128,218,COL_RED
NM1A:
    // asm: TSTB	2,R4
    // asm: 	BZ	NM2A
    // asm: 	FILL	32,232,128,278,COL_RED
NM2A:
    // asm: TSTB	4,R4
    // asm: 	BZ	NM3A
    // asm: 	FILL	32,142,58,218,COL_RED
NM3A:
    // asm: TSTB	8,R4
    // asm: 	BZ	NM4A
    // asm: 	FILL	32,82,128,118,COL_RED
NM4A:
    // asm: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_MAT_NOTPLUGGED", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_OPTO_PATH(void)
{
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"OPTO PATH BROKEN",70,330
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_PATH", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_OPTO_LED(void)
{
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"OPTO LED NOT EMITTING",70,330
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R4
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	1,R4
    // asm: 	BZ	NJ1
    // asm: 	FILLW	150+32,212,16,16,COL_RED	;L1
NJ1:
    // asm: TSTB	2,R4
    // asm: 	BZ	NJ2
    // asm: 	FILLW	150+57,232,16,16,COL_RED	;L2
NJ2:
    // asm: TSTB	4,R4
    // asm: 	BZ	NJ3
    // asm: 	FILLW	150+87,232,16,16,COL_RED	;L3
NJ3:
    // asm: TSTB	8,R4
    // asm: 	BZ	NJ4
    // asm: 	FILLW	150+112,212,16,16,COL_RED 	;L4
NJ4:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_LED", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_OPTO_DETECTOR(void)
{
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"OPTO DETECTOR NOT RECEIVING",70,330
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R4
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	1,R4
    // asm: 	BZ	NJ1A
    // asm: 	FILLW	150+32,112,16,16,COL_RED	;D1
NJ1A:
    // asm: TSTB	2,R4
    // asm: 	BZ	NJ2A
    // asm: 	FILLW	150+57,82,16,16,COL_RED		;D2
NJ2A:
    // asm: TSTB	4,R4
    // asm: 	BZ	NJ3A
    // asm: 	FILLW	150+87,82,16,16,COL_RED		;D3
NJ3A:
    // asm: TSTB	8,R4
    // asm: 	BZ	NJ4A
    // asm: 	FILLW	150+112,112,16,16,COL_RED 	;D4
NJ4A:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_OPTO_DETECTOR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MSC_FAILSAFE_OK(void)
{
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"FAIL SAFE SWITCH ENGAGED",70,330
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R4
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	4,R4
    // asm: 	BZ	NM4B
    // asm: 	FILL	150+150+62,82,150+150+98,118,COL_RED
NM4B:
    // asm: TSTB	1,R4
    // asm: 	BZ	NM1B
    // asm: 	FILL	150+150+32,232,150+150+58,278,COL_RED
NM1B:
    // asm: TSTB	2,R4
    // asm: 	BZ	NM2B
    // asm: 	FILL	150+150+102,232,150+150+128,278,COL_RED
NM2B:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_FAILSAFE_OK", 0, 0);
    UNIMPL();
}

void MSC_FAILSAFE_NOTCONNECTED(void)
{
    // asm: 	IWINDOW	50,320,450,360
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"FAIL SAFE SWITCH NOT CONNECTED CORRECTLY",70,330
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R4
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R4		;right justify
    // asm: 	TSTB	4,R4
    // asm: 	BZ	NM4BA
    // asm: 	FILL	150+150+62,82,150+150+98,118,COL_RED
NM4BA:
    // asm: TSTB	1,R4
    // asm: 	BZ	NM1BA
    // asm: 	FILL	150+150+32,232,150+150+58,278,COL_RED
NM1BA:
    // asm: TSTB	2,R4
    // asm: 	BZ	NM2BA
    // asm: 	FILL	150+150+102,232,150+150+128,278,COL_RED
NM2BA:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSC_FAILSAFE_NOTCONNECTED", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void DRAW_MOTION_SAFETY_DIAG(void)
{
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"MATS:",50,50
    // asm: 	TEXTITT	"FRONT",55,72
    // asm: 	TEXTITT	"REAR",55,285
    // 	;MATS
    // asm: 	RECT	30,80,130,120,COL_BLACK
    // asm: 	RECT	30,140,60,220,COL_BLACK
    // asm: 	RECT	100,140,130,220,COL_BLACK
    // asm: 	RECT	30,230,130,280,COL_BLACK
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"OPTOS:",150+50,50
    // asm: 	TEXTITT	"FRONT",150+55,72
    // asm: 	TEXTITT	"REAR",150+55,285
    // 	;LEDS
    // asm: 	RECTW	150+30,110,20,20,COL_BLACK	;D1
    // asm: 	RECTW	150+55,80,20,20,COL_BLACK	;D2
    // asm: 	RECTW	150+85,80,20,20,COL_BLACK	;D3
    // asm: 	RECTW	150+110,110,20,20,COL_BLACK 	;D4
    // asm: 	RECTW	150+30,210,20,20,COL_BLACK	;L1
    // asm: 	RECTW	150+55,230,20,20,COL_BLACK	;L2
    // asm: 	RECTW	150+85,230,20,20,COL_BLACK	;L3
    // asm: 	RECTW	150+110,210,20,20,COL_BLACK 	;L4
    // ;	RECT	150+30,80,150+130,120,COL_BLACK
    // ;	RECT	150+30,140,150+60,220,COL_BLACK
    // ;	RECT	150+100,140,150+130,220,COL_BLACK
    // ;	RECT	150+30,230,150+130,280,COL_BLACK
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"FAILSAFE SWITCHES:",150+150,50
    // asm: 	TEXTITT	"FRONT",150+150+61,72
    // asm: 	TEXTITT	"REAR",150+150+65,285
    // 	;FAIL SAFE
    // asm: 	RECT	150+150+60,80,150+150+100,120,COL_BLACK
    // asm: 	RECT	150+150+30,230,150+150+60,280,COL_BLACK
    // asm: 	RECT	150+150+100,230,150+150+130,280,COL_BLACK
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	TEXTITT	"HIT ENTER TO ESCAPE",20,380
    // asm: 	TEXTITT	"MOTION SAFETY DIAGNOSTICS",30,20
    // asm: 	TEXTITT	"ERROR MESSAGE:",50,305
    // asm: 	TEXTITT	"MOTION STOP:",300,15
    // asm: 	IWINDOW	50,320,450,360	;error window
    // asm: 	IWINDOW	390,5,430,35	;motion
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRAW_MOTION_SAFETY_DIAG", 0, 0);
    UNIMPL();
}

void CLEAR_MATS(void)
{
    // 	;MATS
    // asm: 	FILL	32,82,128,118,0
    // asm: 	FILL	32,142,58,218,0
    // asm: 	FILL	102,142,128,218,0
    // asm: 	FILL	32,232,128,278,0
    // 	;FAIL SAFE
    // asm: 	FILL	150+150+62,82,150+150+98,118,0
    // asm: 	FILL	150+150+32,232,150+150+58,278,0
    // asm: 	FILL	150+150+102,232,150+150+128,278,0
    // 	;LED (LED)
    // asm: 	FILLW	150+32,212,16,16,0	;L1
    // asm: 	FILLW	150+57,232,16,16,0	;L2
    // asm: 	FILLW	150+87,232,16,16,0	;L3
    // asm: 	FILLW	150+112,212,16,16,0 	;L4
    // 	;LED (DETECTOR)
    // asm: 	FILLW	150+32,112,16,16,0	;D1
    // asm: 	FILLW	150+57,82,16,16,0	;D2
    // asm: 	FILLW	150+87,82,16,16,0	;D3
    // asm: 	FILLW	150+112,112,16,16,0 	;D4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_MATS", 0, 0);
    UNIMPL();
}

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
    // asm: 	AND	SW_DIAG,R0
    // asm: 	RETSNZ
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R0
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	TSTB	080h,R0
    // asm: 	BZ	MSOFF
    // asm: 	FILL	400,10,420,30,COL_RED
    // asm: 	BU	MSFX
MSOFF:
    // asm: FILL	400,10,420,30,0
MSFX:
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R0
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	7,R0
    // asm: 	LS	16+7,R0
    // asm: 	RS	16,R0
    // asm: 	CMPI	@OLD_MOTION_CODE,R0
    // asm: 	BEQ	MSSD_LP
    // asm: 	STI	R0,@OLD_MOTION_CODE
    // ;
    // ;binary debugging aid
    // ;
    // asm: 	FILL	300,305,500,315,0
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R2
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R2
    // asm: 	AND	0FFh,R2
    // asm: 	LDL	IBOIBOT,AR2
    // asm: 	CALL	HEX2ASC
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	LDI	300,R2
    // asm: 	LDI	305,R3
    // asm: 	CALL	_outtextxyc
    // ;end dbg
    // ;
    // asm: 	CALL	MOTION_CHECK_SAFETY_CODES
    // asm: 	BU	MSSD_LP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_SAFETY_SWITCHES_DIAG", 0, 0);
    UNIMPL();
}

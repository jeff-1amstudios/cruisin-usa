#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/vunit.h"
#include "../include/c30.h"
#include "../include/cmos.h"
#include "../include/macs.h"
#include "../include/sys.h"
#include "../include/globals.h"

/*
 * Source module: asm/COINTAB.ASM
 */

// *----------------------------------------------------------------------------
// *COINTAB.ASM
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *
// ;	.def	MSGC_HONGKONG1,MSGC_HONGKONG2,MSGC_HONGKONG3
/* asm: COINMSGC_TAB */
/* asm: 	.word	MSGC_USA1,MSGC_USA2,MSGC_USA3,MSGC_USA4,MSGC_USA5 */
/* asm: 	.word	MSGC_USA6,MSGC_USA7,MSGC_USA8,MSGC_USA9,MSGC_USA10,MSGC_USA11,MSGC_USA12 */
/* asm: 	.word	MSGC_GERMAN1,MSGC_GERMAN2,MSGC_GERMAN3,MSGC_GERMAN4,MSGC_GERMAN5 */
/* asm: 	.word	MSGC_GERMAN6,MSGC_GERMAN7,MSGC_GERMAN8 */
/* asm: 	.word	MSGC_FRENCH1,MSGC_FRENCH2,MSGC_FRENCH3,MSGC_FRENCH4,MSGC_FRENCH5,MSGC_FRENCH6 */
/* asm: 	.word	MSGC_FRENCH7,MSGC_FRENCH8,MSGC_FRENCH9,MSGC_FRENCH10,MSGC_FRENCH11 */
/* asm: 	.word	MSGC_CANADA1,MSGC_CANADA2,MSGC_CANADA3 */
/* asm: 	.word	MSGC_SWISS1,MSGC_SWISS2,MSGC_SWISS3 */
/* asm: 	.word	MSGC_ITALY1,MSGC_ITALY2,MSGC_ITALY3 */
/* asm: 	.word	MSGC_UK1,MSGC_UK2,MSGC_UK3 */
/* asm: 	.word	MSGC_SPAIN1,MSGC_SPAIN2,MSGC_SPAIN3,MSGC_SPAIN4,MSGC_SPAIN5,MSGC_SPAIN6 */
/* asm: 	.word	MSGC_AUSTRALIA1,MSGC_AUSTRALIA2,MSGC_AUSTRALIA3,MSGC_AUSTRALIA4 */
/* asm: 	.word	MSGC_JAPAN1,MSGC_JAPAN2,MSGC_JAPAN3 */
/* asm: 	.word	MSGC_TAIWAN1,MSGC_TAIWAN2,MSGC_TAIWAN3 */
/* asm: 	.word	MSGC_AUSTRIA1,MSGC_AUSTRIA2,MSGC_AUSTRIA3,MSGC_AUSTRIA4 */
/* asm: 	.word	MSGC_BELGIUM1,MSGC_BELGIUM2,MSGC_BELGIUM3 */
/* asm: 	.word	MSGC_SWEDEN1,MSGC_FINLAND1,MSGC_NETHERLAND1,MSGC_NETHERLAND2,MSGC_NORWAY1,MSGC_DENMARK1 */
/* asm: 	.word	MSGC_HUNGARY1 */
/* asm: 	.word	MSGC_GENERAL1,MSGC_GENERAL2,MSGC_GENERAL3,MSGC_GENERAL4,MSGC_GENERAL5 */
/* asm: 	.word	MSGC_GENERAL6,MSGC_GENERAL7,MSGC_GENERAL8 */
/* asm: 	;	.word	MSGC_HONGKONG1,MSGC_HONGKONG2,MSGC_HONGKONG3 */
const char * *COINMSGC_TAB[81] = {
    MSGC_USA1, MSGC_USA2, MSGC_USA3, MSGC_USA4, MSGC_USA5,
    MSGC_USA6, MSGC_USA7, MSGC_USA8, MSGC_USA9, MSGC_USA10, MSGC_USA11, MSGC_USA12,
    MSGC_GERMAN1, MSGC_GERMAN2, MSGC_GERMAN3, MSGC_GERMAN4, MSGC_GERMAN5,
    MSGC_GERMAN6, MSGC_GERMAN7, MSGC_GERMAN8,
    MSGC_FRENCH1, MSGC_FRENCH2, MSGC_FRENCH3, MSGC_FRENCH4, MSGC_FRENCH5, MSGC_FRENCH6,
    MSGC_FRENCH7, MSGC_FRENCH8, MSGC_FRENCH9, MSGC_FRENCH10, MSGC_FRENCH11,
    MSGC_CANADA1, MSGC_CANADA2, MSGC_CANADA3,
    MSGC_SWISS1, MSGC_SWISS2, MSGC_SWISS3,
    MSGC_ITALY1, MSGC_ITALY2, MSGC_ITALY3,
    MSGC_UK1, MSGC_UK2, MSGC_UK3,
    MSGC_SPAIN1, MSGC_SPAIN2, MSGC_SPAIN3, MSGC_SPAIN4, MSGC_SPAIN5, MSGC_SPAIN6,
    MSGC_AUSTRALIA1, MSGC_AUSTRALIA2, MSGC_AUSTRALIA3, MSGC_AUSTRALIA4,
    MSGC_JAPAN1, MSGC_JAPAN2, MSGC_JAPAN3,
    MSGC_TAIWAN1, MSGC_TAIWAN2, MSGC_TAIWAN3,
    MSGC_AUSTRIA1, MSGC_AUSTRIA2, MSGC_AUSTRIA3, MSGC_AUSTRIA4,
    MSGC_BELGIUM1, MSGC_BELGIUM2, MSGC_BELGIUM3,
    MSGC_SWEDEN1, MSGC_FINLAND1, MSGC_NETHERLAND1, MSGC_NETHERLAND2, MSGC_NORWAY1, MSGC_DENMARK1,
    MSGC_HUNGARY1,
    MSGC_GENERAL1, MSGC_GENERAL2, MSGC_GENERAL3, MSGC_GENERAL4, MSGC_GENERAL5,
    MSGC_GENERAL6, MSGC_GENERAL7, MSGC_GENERAL8,
};
const char *MSG_NULL = "";
const char *MSG_41 = "4 CREDITS / 1 COIN";
const char *MSG_32D = "3 CREDITS / 2 DOLLARS";
const char *MSG_31 = "3 CREDITS / 1 COIN";
const char *MSG_21D = "2 CREDITS / 1 DOLLAR";
const char *MSG_21 = "2 CREDITS / 1 COIN";
const char *MSG_11 = "1 COIN / 1 CREDIT";
const char *MSG_11D = "1 DOLLAR / 1 CREDIT";
const char *MSG_11DM = "1DM / 1 CREDIT";
const char *MSG_1150D = "1 50 DOLLAR / 1 CREDIT";
const char *MSG_12 = "2 COINS / 1 CREDIT";
const char *MSG_12D = "2 DOLLARS / 1 CREDIT";
const char *MSG_12DM = "2DM / 1 CREDIT";
const char *MSG_13 = "3 COINS / 1 CREDIT";
const char *MSG_13D = "3 DOLLARS / 1 CREDIT";
const char *MSG_13DM = "3DM / 1 CREDIT";
const char *MSG_14 = "4 COINS / 1 CREDIT";
const char *MSG_15 = "5 COINS / 1 CREDIT";
const char *MSG_15X20C = "5 X 2O CENTS / 1 CREDIT";
const char *MSG_110X20C = "1O X 2O CENTS / 1 CREDIT";
const char *MSG_16 = "6 COINS / 1 CREDIT";
const char *MSG_18 = "8 COINS / 1 CREDIT";
const char *MSG_25DM = "5DM / 2 CREDITS";
const char *MSG_35D = "5 DOLLARS / 3 CREDITS";
const char *MSG_35DM = "5DM / 3 CREDITS";
const char *MSG_45D = "5 DOLLARS / 4 CREDITS";
const char *MSG_65D = "5 DOLLARS / 6 CREDITS";
const char *MSG_65DM = "5DM / 6 CREDITS";
const char *MSG_11F = "1F / 1 CREDIT";
const char *MSG_12F = "2F / 1 CREDIT";
const char *MSG_13F = "3F / 1 CREDIT";
const char *MSG_15F = "5F / 1 CREDIT";
const char *MSG_13X5F = "3 X 5F / 1 CREDIT";
const char *MSG_110F = "1OF / 1 CREDIT";
const char *MSG_120F = "2OF / 1 CREDIT";
const char *MSG_12X20F = "2 X 2OF / 1 CREDIT";
const char *MSG_25F = "5F / 2 CREDITS";
const char *MSG_210F = "1OF / 2 CREDITS";
const char *MSG_220F = "2OF / 2 CREDITS";
const char *MSG_35F = "5F / 3 CREDITS";
const char *MSG_310F = "1OF / 3 CREDITS";
const char *MSG_320F = "2OF / 3 CREDITS";
const char *MSG_350F = "5OF / 3 CREDITS";
const char *MSG_430F = "3OF / 4 CREDITS";
const char *MSG_450F = "5OF / 4 CREDITS";
const char *MSG_55F = "5F / 5 CREDITS";
const char *MSG_520F = "2OF / 5 CREDITS";
const char *MSG_540F = "4OF / 5 CREDITS";
const char *MSG_720F = "2OF / 7 CREDITS";
const char *MSG_740F = "4OF / 7 CREDITS";
const char *MSG_1500L = "5OO LIRE / 1 CREDIT";
const char *MSG_12X500L = "2 X 5OO LIRE / 1 CREDIT";
const char *MSG_14X500L = "4 X 5OO LIRE / 1 CREDIT";
const char *MSG_150P = "5OP / 1 CREDIT";
const char *MSG_13X20P = "3 X 2OP / 1 CREDIT";
const char *MSG_1100P = "1OOP / 1 CREDIT";
const char *MSG_1100PESETA = "1OOPESETA / 1 CREDIT";
const char *MSG_12X100PESETA = "2 X 1OOPESETA / 1 CREDIT";
const char *MSG_2100PESETA = "1OOPESETA / 2 CREDITS";
const char *MSG_32X100PESETA = "2 X 1OOPESETA / 3 CREDITS";
const char *MSG_3500PESETA = "5OOPESETA / 3 CREDITS";
const char *MSG_5500PESETA = "5OOPESETA / 5 CREDITS";
const char *MSG_6500PESETA = "5OOPESETA / 6 CREDITS";
const char *MSG_7500PESETA = "5OOPESETA / 7 CREDITS";
const char *MSG_10500PESETA = "5OOPESETA / 1O CREDITS";
const char *MSG_1100YEN = "1OO YEN / 1 CREDIT";
const char *MSG_3500YEN = "5OO YEN / 3 CREDITS";
const char *MSG_1NT10 = "NT1O / 1 CREDIT";
const char *MSG_15SCHIL = "5 SCHILLING / 1 CREDIT";
const char *MSG_12X5SCHIL = "2 X 5 SCHILLING / 1 CREDIT";
const char *MSG_12X10SCHIL = "2 X 10 SCHILLING / 1 CREDIT";
const char *MSG_110SCHIL = "1O SCHILLING / 1 CREDIT";
const char *MSG_210SCHIL = "1O SCHILLING / 2 CREDITS";
const char *MSG_230SCHIL = "3O SCHILLING / 2 CREDITS";
const char *MSG_320SCHIL = "2O SCHILLING / 3 CREDITS";
const char *MSG_15KRONA = "5 KRONA / 1 CREDIT";
const char *MSG_15MARKKA = "5 MARKKA / 1 CREDIT";
const char *MSG_15KRONE = "5 KRONE / 1 CREDIT";
const char *MSG_11HFI = "1 HFI / 1 CREDIT";
const char *MSG_1250HFI = "2 5O HFI / 1 CREDIT";
const char *MSG_3250HFI = "2 5O HFI / 3 CREDITS";
const char *MSG_14X20FORINT = "4 X 2O FORINT / 1 CREDIT";
const char *MSG_2140FORINT = "14O FORINT / 2 CREDITS";
const char *MSG_25C = ".25";
const char *MSG_20C = ".2O";
const char *MSG_100D = "1.OO";
const char *MSG_200D = "2.OO";
const char *MSG_1DM = "1 DM";
const char *MSG_2DM = "2 DM";
const char *MSG_5DM = "5 DM";
const char *MSG_1F = "1 F";
const char *MSG_5F = "5 F";
const char *MSG_10F = "1O F";
const char *MSG_20F = "2O F";
const char *MSG_50F = "5O F";
const char *MSG_500L = "5OO LIRE";
const char *MSG_10P = "1OP";
const char *MSG_20P = "2OP";
const char *MSG_25P = "25P";
const char *MSG_50P = "5OP";
const char *MSG_100P = "1OOP";
const char *MSG_200P = "2OOP";
const char *MSG_500P = "5OOP";
const char *MSG_100YEN = "1OO YEN";
const char *MSG_500YEN = "5OO YEN";
const char *MSG_NT10 = "NT1O";
const char *MSG_1COIN = "1 COIN";
const char *MSG_4COINS = "4 COINS";
const char *MSG_5SCHIL = "5 SCHIL";
const char *MSG_10SCHIL = "1O SCHIL";
const char *MSG_20SCHIL = "2O SCHIL";
const char *MSG_1KRONA = "1 KRONA";
const char *MSG_5KRONA = " 5KRONA";
const char *MSG_10KRONA = "1OKRONA";
const char *MSG_1MARKKA = "1MARKKA";
const char *MSG_5MARKKA = "  5MARKKA";
const char *MSG_10MARKKA = "1OMARKKA";
const char *MSG_1HFI = "1HFI";
const char *MSG_250HFI = "2.5O HFI";
const char *MSG_1KRONE = "1KRON";
const char *MSG_5KRONE = "5KRON";
const char *MSG_10KRONE = "1OKRON";
const char *MSG_20KRONE = "2OKRON";
const char *MSG_20FORINT = "2OFORINT";
const char *MSG_100FORINT = "1OOFORINT";
const char *MSGC_USA1 = "USA1";
const char *MSGC_USA2 = "USA2";
const char *MSGC_USA3 = "USA3";
const char *MSGC_USA4 = "USA4";
const char *MSGC_USA5 = "USA5";
const char *MSGC_USA6 = "USA6";
const char *MSGC_USA7 = "USA7";
const char *MSGC_USA8 = "USA8";
const char *MSGC_USA9 = "USA9";
const char *MSGC_USA10 = "USA10";
const char *MSGC_USA11 = "USA11";
const char *MSGC_USA12 = "USA12";
const char *MSGC_GERMAN1 = "GERMAN1";
const char *MSGC_GERMAN2 = "GERMAN2";
const char *MSGC_GERMAN3 = "GERMAN3";
const char *MSGC_GERMAN4 = "GERMAN4";
const char *MSGC_GERMAN5 = "GERMAN5";
const char *MSGC_GERMAN6 = "GERMAN6";
const char *MSGC_GERMAN7 = "GERMAN7";
const char *MSGC_GERMAN8 = "GERMAN8";
const char *MSGC_FRENCH1 = "FRENCH1";
const char *MSGC_FRENCH2 = "FRENCH2";
const char *MSGC_FRENCH3 = "FRENCH3";
const char *MSGC_FRENCH4 = "FRENCH4";
const char *MSGC_FRENCH5 = "FRENCH5";
const char *MSGC_FRENCH6 = "FRENCH6";
const char *MSGC_FRENCH7 = "FRENCH7";
const char *MSGC_FRENCH8 = "FRENCH8";
const char *MSGC_FRENCH9 = "FRENCH9";
const char *MSGC_FRENCH10 = "FRENCH10";
const char *MSGC_FRENCH11 = "FRENCH11";
const char *MSGC_CANADA1 = "CANADA1";
const char *MSGC_CANADA2 = "CANADA2";
const char *MSGC_CANADA3 = "CANADA3";
const char *MSGC_SWISS1 = "SWISS1";
const char *MSGC_SWISS2 = "SWISS2";
const char *MSGC_SWISS3 = "SWISS3";
const char *MSGC_ITALY1 = "ITALY1";
const char *MSGC_ITALY2 = "ITALY2";
const char *MSGC_ITALY3 = "ITALY3";
const char *MSGC_UK1 = "UK1";
const char *MSGC_UK2 = "UK2";
const char *MSGC_UK3 = "UK3";
const char *MSGC_SPAIN1 = "SPAIN1";
const char *MSGC_SPAIN2 = "SPAIN2";
const char *MSGC_SPAIN3 = "SPAIN3";
const char *MSGC_SPAIN4 = "SPAIN4";
const char *MSGC_SPAIN5 = "SPAIN5";
const char *MSGC_SPAIN6 = "SPAIN6";
const char *MSGC_AUSTRALIA1 = "AUSTRALIA1";
const char *MSGC_AUSTRALIA2 = "AUSTRALIA2";
const char *MSGC_AUSTRALIA3 = "AUSTRALIA3";
const char *MSGC_AUSTRALIA4 = "AUSTRALIA4";
const char *MSGC_JAPAN1 = "JAPAN1";
const char *MSGC_JAPAN2 = "JAPAN2";
const char *MSGC_JAPAN3 = "JAPAN3";
const char *MSGC_TAIWAN1 = "TAIWAN1";
const char *MSGC_TAIWAN2 = "TAIWAN2";
const char *MSGC_TAIWAN3 = "TAIWAN3";
const char *MSGC_AUSTRIA1 = "AUSTRIA1";
const char *MSGC_AUSTRIA2 = "AUSTRIA2";
const char *MSGC_AUSTRIA3 = "AUSTRIA3";
const char *MSGC_AUSTRIA4 = "AUSTRIA4";
const char *MSGC_BELGIUM1 = "BELGIUM1";
const char *MSGC_BELGIUM2 = "BELGIUM2";
const char *MSGC_BELGIUM3 = "BELGIUM3";
const char *MSGC_SWEDEN1 = "SWEDEN1";
const char *MSGC_FINLAND1 = "FINLAND1";
const char *MSGC_NETHERLAND1 = "NETHERLAND1";
const char *MSGC_NETHERLAND2 = "NETHERLAND2";
const char *MSGC_NORWAY1 = "NORWAY1";
const char *MSGC_DENMARK1 = "DENMARK1";
const char *MSGC_HUNGARY1 = "HUNGARY1";
const char *MSGC_GENERAL1 = "GENERAL1";
const char *MSGC_GENERAL2 = "GENERAL2";
const char *MSGC_GENERAL3 = "GENERAL3";
const char *MSGC_GENERAL4 = "GENERAL4";
const char *MSGC_GENERAL5 = "GENERAL5";
const char *MSGC_GENERAL6 = "GENERAL6";
const char *MSGC_GENERAL7 = "GENERAL7";
const char *MSGC_GENERAL8 = "GENERAL8";
const char *MSGC_HONGKONG1 = "HONGKONG1";
const char *MSGC_HONGKONG2 = "HONGKONG2";
const char *MSGC_HONGKONG3 = "HONGKONG3";
// *----------------------------------------------------------------------------
// *COIN TABLE ENTRY 11 words total
// *0	byte	coin1,coin2,coin3,coin4
// *1	byte	units per credit,units for bonus,min units,credits to start
// *2	byte	credits to continue,show partial credits,NOT USED,NOT USED
// *3	word	message_string line 1;Must have at lease one message
// *4	word	message_string line 2;	0 = no message on line 2 or 3
// *5	word	message_string line 3;	0 = no message on line 3
// *6	word	message_string coin 1 denaomination	NOTE: NULL$ = NOT USED
// *7	word	message_string coin 2 denaomination	NOTE: NULL$ = NOT USED
// *8	word	message_string coin 3 denaomination	NOTE: NULL$ = NOT USED
// *9	word	message_string coin 4 denaomination	NOTE: NULL$ = NOT USED
// *10	byte	coin1_denom,coin2_denom,coin3_denom,coin4_denom
// *NOTE ALL UNUSED BYTES MUST BE SET TO 0
// ;1/3X25
// ;1/2X25
// ;1/4X25
// ;1/5X25
// ;1/6X25
// ;1/8X25
// ;1/8X25, 1/2X100, 3/1X500
// ;1/25, 3X25/START, 2X25 CONTINUE
// ;1/25, 2X25/START, 1X25 CONTINUE
// ;1/2X100 START, 1/150 CONTINUE
// ;1/6X25, 4/5X100
// ;1/100 START, 1/3X25 CONTINUE
// ;1/3X1DM, 2/5DM
// ;1/1DM, 5/5DM
// ;1/2X1DM, 3/5DM
// ;2/5DM, 1/3X1DM ECA
// ;3/5DM, 1/2DM, 1/2X1DM ECA
// ;1/3X1DM, 2/5DM NO BONUS
// ;1/2X1DM, 3/5DM NO BONUS
// ;1/1DM, 6/5DM NO BONUS 2 start 1 continue
// ;1/10F, 2/20F, 4/30
// ;1/10F, 2/20F, 5/40F
// ;1/10F, 3/20F, 7/40F
// ;1/5F, 2/10F, 5/20F
// ;1/5F, 3/10F, 7/20F
// ;1F, 5F, 10F, 20F ECA
// ;1/10F 2/20F, 4/30F
// ;1F, 5F, 10F, 20F ECA
// ;1/10F 2/20F, 5/40F
// ;1F, 5F, 10F, 20F ECA
// ;1/10F 3/20F, 7/40F
// ;1F, 5F, 10F, 20F ECA
// ;1/5F 2/10F, 5/20F
// ;1F, 5F, 10F, 20F ECA
// ;1/5F 3/10F, 7/20F
// ;1/3X5F, 4/50F
// ;1/2X25, 2/100,
// ;1/4X25, 1/100,
// ;1/4X25, 1/100, 3/2X100
// ;1/3X1F, 2/5F
// ;1/1F, 5/5F
// ;1/2X1F, 3/5F
// ;1/500LIRE
// ;1/2X500LIRE
// ;1/4X500LIRE
// ;2/100P, 1/50P, ECA
// ;1/100P, 1/2X50P, ECA
// ;1/3X20P, 1/50P
// ;1/100PESETA, 6/500PESETA
// ;3/2X100PESETA, 7/500PESETA
// ;100PESETA, 5/500PESETA
// ;1/2X100PESETA, 3/500PESETA
// ;2/100PESETA, 10/500PESETA
// ;1/100PESETA ECA
// ;1/5X20, 1/100
// ;1/10X20, 1/2X100
// ;1/2X100
// ;1/3X100
// ;1/100
// ;1/2X100
// ;1/100, 3/500
// ;1/NT10
// ;1/2XUS25
// ;2/1 1 COIN = 2 PLAYS
// ;1/5SCHIL, 2/10SCHIL
// ;1/10SCHIL, 3/20SCHIL
// ;1/2X5SCHIL, 1/10SCHIL
// ;1/2X10SCHIL, 2/30SCHIL
// ;1/20F
// ;1/2X20F
// ;1/20FB 3/50FB ECA
// ;1/5KRONA ECA
// ;1/5MARKA
// ;1/2.50HFI
// ;1/1HFI, 3/1X2.50HFI
// ;1/5KRONE ECA
// ;1/5DKR ECA
// ;1/4X20 FORINT, 2/140 FORINT
// ;GENERAL COIN MODES
// ;1 CREDIT/1 COIN
// ;1 CREDIT/2 COINS
// ;1 CREDIT/3 COINS
// ;1 CREDIT/4 COINS
// ;1 CREDIT/5 COINS
// ;1 CREDIT/6 COINS
// ;2 CREDITS/1 COIN
// ;3 CREDITS/1 COIN
// ;;5 COINS / 1 CREDIT, 4 COINS / 1 CREDIT
// ;HONGKONG1
// ;	.string	3,3,3,12
// ;	.string	1,0,0,1
// ;	.string	1,1,0,0
// ;	.word	MSG_31
// ;	.word	0
// ;	.word	0
// ;	.word	MSG_1COIN
// ;	.word	MSG_1COIN
// ;	.word	MSG_1COIN
// ;	.word	MSG_4COINS
#define USA_OFF ((USA1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define GERMAN_OFF ((GERMAN1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define FRENCH_OFF ((FRENCH1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define CANADA_OFF ((CANADA1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define SWISS_OFF ((SWISS1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define ITALY_OFF ((ITALY1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define UK_OFF ((UK1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define SPAIN_OFF ((SPAIN1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define AUSTRALIA_OFF ((AUSTRALIA1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define JAPAN_OFF ((JAPAN1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define TAIWAN_OFF ((TAIWAN1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define AUSTRIA_OFF ((AUSTRIA1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define BELGIUM_OFF ((BELGIUM1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define SWEDEN_OFF ((SWEDEN1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define FINLAND_OFF ((FINLAND1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define NETHERLAND_OFF ((NETHERLAND1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define NORWAY_OFF ((NORWAY1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define DENMARK_OFF ((DENMARK1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define HUNAGRY_OFF ((HUNGARY1-COIN_TABLE)/COIN_ENTRY_SIZE)
#define GENERAL_OFF ((GENERAL1-COIN_TABLE)/COIN_ENTRY_SIZE)
// ;HONGKONG_OFF	.set (HONGKONG1-COIN_TABLE)/COIN_ENTRY_SIZE
/* asm: COUNTRY_TAB */
/* asm: 	.word	USA_OFF		;1 */
/* asm: 	.word	GERMAN_OFF	;2 */
/* asm: 	.word	FRENCH_OFF	;3 */
/* asm: 	.word	CANADA_OFF	;4 */
/* asm: 	.word	SWISS_OFF	;5 */
/* asm: 	.word	ITALY_OFF	;6 */
/* asm: 	.word	UK_OFF		;7 */
/* asm: 	.word	SPAIN_OFF	;8 */
/* asm: 	.word	AUSTRALIA_OFF	;9 */
/* asm: 	.word	JAPAN_OFF	;10 */
/* asm: 	.word	TAIWAN_OFF	;11 */
/* asm: 	.word	AUSTRIA_OFF	;12 */
/* asm: 	.word	BELGIUM_OFF	;13 */
/* asm: 	.word	SWEDEN_OFF	;14	OTHER COUNTRYS */
/* asm: 	.word	GENERAL_OFF	;15 */
/* asm: 	;	.word	HONGKONG_OFF	;16 */
/* asm: 	;The COUNTRY_DEFAULTS TABLE is used for the dip switch settings */
/* asm: 	;The COUNTRY_DEFAULTS table is needed because the COUNTRY_TAB is used for the menu system */
/* asm: 	;in diag and differs from the dipswitch settings. It also alows us to insert countrys without */
/* asm: 	;altering the dufault dip switch settings */
/* asm: 	;format OF TABLE ENTRY PER COUNTRY */
/* asm: 	;	.word	mode_offset0,mode_offset1,mode_offset2,mode_offset3 */
int COUNTRY_TAB[15] = {
    USA_OFF, // 1
    GERMAN_OFF, // 2
    FRENCH_OFF, // 3
    CANADA_OFF, // 4
    SWISS_OFF, // 5
    ITALY_OFF, // 6
    UK_OFF, // 7
    SPAIN_OFF, // 8
    AUSTRALIA_OFF, // 9
    JAPAN_OFF, // 10
    TAIWAN_OFF, // 11
    AUSTRIA_OFF, // 12
    BELGIUM_OFF, // 13
    SWEDEN_OFF, // 14	OTHER COUNTRYS
    GENERAL_OFF, // 15
};
/* asm: COUNTRY_DEFAULTS */
/* asm: 	.word	USA1,USA3,USA7,USA8				;0 */
/* asm: 	.word	GERMAN1,GERMAN2,GERMAN3,GERMAN4			;1 */
/* asm: 	.word	FRENCH1,FRENCH2,FRENCH3,FRENCH4			;2 */
/* asm: 	.word	CANADA1,CANADA2,CANADA3,CANADA1			;3 */
/* asm: 	.word	SWISS1,SWISS2,SWISS3,SWISS1			;4 */
/* asm: 	.word	ITALY1,ITALY2,ITALY3,ITALY1			;5 */
/* asm: 	.word	UK1,UK2,UK3,UK1					;6 */
/* asm: 	.word	SPAIN1,SPAIN2,SPAIN3,SPAIN4			;7 */
/* asm: 	.word	AUSTRALIA1,AUSTRALIA2,AUSTRALIA3,AUSTRALIA4	;8 */
/* asm: 	.word	JAPAN1,JAPAN2,JAPAN3,JAPAN1			;9 */
/* asm: 	.word	TAIWAN1,TAIWAN2,TAIWAN3,TAIWAN1			;10 */
/* asm: 	.word	AUSTRIA1,AUSTRIA2,AUSTRIA3,AUSTRIA4		;11 */
/* asm: 	.word	BELGIUM1,BELGIUM2,BELGIUM3,BELGIUM1		;12 */
/* asm: 	.word	SWEDEN1,SWEDEN1,SWEDEN1,SWEDEN1			;13 */
/* asm: 	.word	FINLAND1,FINLAND1,FINLAND1,FINLAND1		;14 */
/* asm: 	.word	NETHERLAND1,NETHERLAND2,NETHERLAND1,NETHERLAND2	;15 */
/* asm: 	.word	NORWAY1,NORWAY1,NORWAY1,NORWAY1 		;16 */
/* asm: 	.word	DENMARK1,DENMARK1,DENMARK1,DENMARK1		;17 */
/* asm: 	.word	HUNGARY1,HUNGARY1,HUNGARY1,HUNGARY1		;18 */
/* asm: 	.word	GENERAL1,GENERAL3,GENERAL5,GENERAL7		;19 */
/* asm: 	;	.word	HONGKONG1,HONGKONG2,HONGKONG3,HONGKONG1		;20 */
int COUNTRY_DEFAULTS[80] = {
    USA1, USA3, USA7, USA8, // 0
    GERMAN1, GERMAN2, GERMAN3, GERMAN4, // 1
    FRENCH1, FRENCH2, FRENCH3, FRENCH4, // 2
    CANADA1, CANADA2, CANADA3, CANADA1, // 3
    SWISS1, SWISS2, SWISS3, SWISS1, // 4
    ITALY1, ITALY2, ITALY3, ITALY1, // 5
    UK1, UK2, UK3, UK1, // 6
    SPAIN1, SPAIN2, SPAIN3, SPAIN4, // 7
    AUSTRALIA1, AUSTRALIA2, AUSTRALIA3, AUSTRALIA4, // 8
    JAPAN1, JAPAN2, JAPAN3, JAPAN1, // 9
    TAIWAN1, TAIWAN2, TAIWAN3, TAIWAN1, // 10
    AUSTRIA1, AUSTRIA2, AUSTRIA3, AUSTRIA4, // 11
    BELGIUM1, BELGIUM2, BELGIUM3, BELGIUM1, // 12
    SWEDEN1, SWEDEN1, SWEDEN1, SWEDEN1, // 13
    FINLAND1, FINLAND1, FINLAND1, FINLAND1, // 14
    NETHERLAND1, NETHERLAND2, NETHERLAND1, NETHERLAND2, // 15
    NORWAY1, NORWAY1, NORWAY1, NORWAY1, // 16
    DENMARK1, DENMARK1, DENMARK1, DENMARK1, // 17
    HUNGARY1, HUNGARY1, HUNGARY1, HUNGARY1, // 18
    GENERAL1, GENERAL3, GENERAL5, GENERAL7, // 19
};

void cointab(void)
{
    UNIMPL();
}

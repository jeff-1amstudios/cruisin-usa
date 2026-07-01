// NOEDIT

#include "../core/machine.h"
#include "../core/port.h"
#include "c30.h"
#include "cmos.h"
#include "globals.h"
#include "macs.h"
#include "sys.h"
#include "vunit.h"

/*
 * Source module: asm/COINTAB.ASM
 */

/* ;	.def	MSGC_HONGKONG1,MSGC_HONGKONG2,MSGC_HONGKONG3
 */
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

char MSG_NULL[] = "";
char MSG_41[] = "4 CREDITS / 1 COIN";
char MSG_32D[] = "3 CREDITS / 2 DOLLARS";
char MSG_31[] = "3 CREDITS / 1 COIN";
char MSG_21D[] = "2 CREDITS / 1 DOLLAR";
char MSG_21[] = "2 CREDITS / 1 COIN";
char MSG_11[] = "1 COIN / 1 CREDIT";
char MSG_11D[] = "1 DOLLAR / 1 CREDIT";
char MSG_11DM[] = "1DM / 1 CREDIT";
char MSG_1150D[] = "1 50 DOLLAR / 1 CREDIT";
char MSG_12[] = "2 COINS / 1 CREDIT";
char MSG_12D[] = "2 DOLLARS / 1 CREDIT";
char MSG_12DM[] = "2DM / 1 CREDIT";
char MSG_13[] = "3 COINS / 1 CREDIT";
char MSG_13D[] = "3 DOLLARS / 1 CREDIT";
char MSG_13DM[] = "3DM / 1 CREDIT";
char MSG_14[] = "4 COINS / 1 CREDIT";
char MSG_15[] = "5 COINS / 1 CREDIT";
char MSG_15X20C[] = "5 X 2O CENTS / 1 CREDIT";
char MSG_110X20C[] = "1O X 2O CENTS / 1 CREDIT";
char MSG_16[] = "6 COINS / 1 CREDIT";
char MSG_18[] = "8 COINS / 1 CREDIT";
char MSG_25DM[] = "5DM / 2 CREDITS";
char MSG_35D[] = "5 DOLLARS / 3 CREDITS";
char MSG_35DM[] = "5DM / 3 CREDITS";
char MSG_45D[] = "5 DOLLARS / 4 CREDITS";
char MSG_65D[] = "5 DOLLARS / 6 CREDITS";
char MSG_65DM[] = "5DM / 6 CREDITS";
char MSG_11F[] = "1F / 1 CREDIT";
char MSG_12F[] = "2F / 1 CREDIT";
char MSG_13F[] = "3F / 1 CREDIT";
char MSG_15F[] = "5F / 1 CREDIT";
char MSG_13X5F[] = "3 X 5F / 1 CREDIT";
char MSG_110F[] = "1OF / 1 CREDIT";
char MSG_120F[] = "2OF / 1 CREDIT";
char MSG_12X20F[] = "2 X 2OF / 1 CREDIT";
char MSG_25F[] = "5F / 2 CREDITS";
char MSG_210F[] = "1OF / 2 CREDITS";
char MSG_220F[] = "2OF / 2 CREDITS";
char MSG_35F[] = "5F / 3 CREDITS";
char MSG_310F[] = "1OF / 3 CREDITS";
char MSG_320F[] = "2OF / 3 CREDITS";
char MSG_350F[] = "5OF / 3 CREDITS";
char MSG_430F[] = "3OF / 4 CREDITS";
char MSG_450F[] = "5OF / 4 CREDITS";
char MSG_55F[] = "5F / 5 CREDITS";
char MSG_520F[] = "2OF / 5 CREDITS";
char MSG_540F[] = "4OF / 5 CREDITS";
char MSG_720F[] = "2OF / 7 CREDITS";
char MSG_740F[] = "4OF / 7 CREDITS";
char MSG_1500L[] = "5OO LIRE / 1 CREDIT";
char MSG_12X500L[] = "2 X 5OO LIRE / 1 CREDIT";
char MSG_14X500L[] = "4 X 5OO LIRE / 1 CREDIT";
char MSG_150P[] = "5OP / 1 CREDIT";
char MSG_13X20P[] = "3 X 2OP / 1 CREDIT";
char MSG_1100P[] = "1OOP / 1 CREDIT";
char MSG_1100PESETA[] = "1OOPESETA / 1 CREDIT";
char MSG_12X100PESETA[] = "2 X 1OOPESETA / 1 CREDIT";
char MSG_2100PESETA[] = "1OOPESETA / 2 CREDITS";
char MSG_32X100PESETA[] = "2 X 1OOPESETA / 3 CREDITS";
char MSG_3500PESETA[] = "5OOPESETA / 3 CREDITS";
char MSG_5500PESETA[] = "5OOPESETA / 5 CREDITS";
char MSG_6500PESETA[] = "5OOPESETA / 6 CREDITS";
char MSG_7500PESETA[] = "5OOPESETA / 7 CREDITS";
char MSG_10500PESETA[] = "5OOPESETA / 1O CREDITS";
char MSG_1100YEN[] = "1OO YEN / 1 CREDIT";
char MSG_3500YEN[] = "5OO YEN / 3 CREDITS";
char MSG_1NT10[] = "NT1O / 1 CREDIT";
char MSG_15SCHIL[] = "5 SCHILLING / 1 CREDIT";
char MSG_12X5SCHIL[] = "2 X 5 SCHILLING / 1 CREDIT";
char MSG_12X10SCHIL[] = "2 X 10 SCHILLING / 1 CREDIT";
char MSG_110SCHIL[] = "1O SCHILLING / 1 CREDIT";
char MSG_210SCHIL[] = "1O SCHILLING / 2 CREDITS";
char MSG_230SCHIL[] = "3O SCHILLING / 2 CREDITS";
char MSG_320SCHIL[] = "2O SCHILLING / 3 CREDITS";
char MSG_15KRONA[] = "5 KRONA / 1 CREDIT";
char MSG_15MARKKA[] = "5 MARKKA / 1 CREDIT";
char MSG_15KRONE[] = "5 KRONE / 1 CREDIT";
char MSG_11HFI[] = "1 HFI / 1 CREDIT";
char MSG_1250HFI[] = "2 5O HFI / 1 CREDIT";
char MSG_3250HFI[] = "2 5O HFI / 3 CREDITS";
char MSG_14X20FORINT[] = "4 X 2O FORINT / 1 CREDIT";
char MSG_2140FORINT[] = "14O FORINT / 2 CREDITS";
char MSG_25C[] = ".25";
char MSG_20C[] = ".2O";
char MSG_100D[] = "1.OO";
char MSG_200D[] = "2.OO";
char MSG_1DM[] = "1 DM";
char MSG_2DM[] = "2 DM";
char MSG_5DM[] = "5 DM";
char MSG_1F[] = "1 F";
char MSG_5F[] = "5 F";
char MSG_10F[] = "1O F";
char MSG_20F[] = "2O F";
char MSG_50F[] = "5O F";
char MSG_500L[] = "5OO LIRE";
char MSG_10P[] = "1OP";
char MSG_20P[] = "2OP";
char MSG_25P[] = "25P";
char MSG_50P[] = "5OP";
char MSG_100P[] = "1OOP";
char MSG_200P[] = "2OOP";
char MSG_500P[] = "5OOP";
char MSG_100YEN[] = "1OO YEN";
char MSG_500YEN[] = "5OO YEN";
char MSG_NT10[] = "NT1O";
char MSG_1COIN[] = "1 COIN";
char MSG_4COINS[] = "4 COINS";
char MSG_5SCHIL[] = "5 SCHIL";
char MSG_10SCHIL[] = "1O SCHIL";
char MSG_20SCHIL[] = "2O SCHIL";
char MSG_1KRONA[] = "1 KRONA";
char MSG_5KRONA[] = " 5KRONA";
char MSG_10KRONA[] = "1OKRONA";
char MSG_1MARKKA[] = "1MARKKA";
char MSG_5MARKKA[] = "  5MARKKA";
char MSG_10MARKKA[] = "1OMARKKA";
char MSG_1HFI[] = "1HFI";
char MSG_250HFI[] = "2.5O HFI";
char MSG_1KRONE[] = "1KRON";
char MSG_5KRONE[] = "5KRON";
char MSG_10KRONE[] = "1OKRON";
char MSG_20KRONE[] = "2OKRON";
char MSG_20FORINT[] = "2OFORINT";
char MSG_100FORINT[] = "1OOFORINT";
char MSGC_USA1[] = "USA1";
char MSGC_USA2[] = "USA2";
char MSGC_USA3[] = "USA3";
char MSGC_USA4[] = "USA4";
char MSGC_USA5[] = "USA5";
char MSGC_USA6[] = "USA6";
char MSGC_USA7[] = "USA7";
char MSGC_USA8[] = "USA8";
char MSGC_USA9[] = "USA9";
char MSGC_USA10[] = "USA10";
char MSGC_USA11[] = "USA11";
char MSGC_USA12[] = "USA12";
char MSGC_GERMAN1[] = "GERMAN1";
char MSGC_GERMAN2[] = "GERMAN2";
char MSGC_GERMAN3[] = "GERMAN3";
char MSGC_GERMAN4[] = "GERMAN4";
char MSGC_GERMAN5[] = "GERMAN5";
char MSGC_GERMAN6[] = "GERMAN6";
char MSGC_GERMAN7[] = "GERMAN7";
char MSGC_GERMAN8[] = "GERMAN8";
char MSGC_FRENCH1[] = "FRENCH1";
char MSGC_FRENCH2[] = "FRENCH2";
char MSGC_FRENCH3[] = "FRENCH3";
char MSGC_FRENCH4[] = "FRENCH4";
char MSGC_FRENCH5[] = "FRENCH5";
char MSGC_FRENCH6[] = "FRENCH6";
char MSGC_FRENCH7[] = "FRENCH7";
char MSGC_FRENCH8[] = "FRENCH8";
char MSGC_FRENCH9[] = "FRENCH9";
char MSGC_FRENCH10[] = "FRENCH10";
char MSGC_FRENCH11[] = "FRENCH11";
char MSGC_CANADA1[] = "CANADA1";
char MSGC_CANADA2[] = "CANADA2";
char MSGC_CANADA3[] = "CANADA3";
char MSGC_SWISS1[] = "SWISS1";
char MSGC_SWISS2[] = "SWISS2";
char MSGC_SWISS3[] = "SWISS3";
char MSGC_ITALY1[] = "ITALY1";
char MSGC_ITALY2[] = "ITALY2";
char MSGC_ITALY3[] = "ITALY3";
char MSGC_UK1[] = "UK1";
char MSGC_UK2[] = "UK2";
char MSGC_UK3[] = "UK3";
char MSGC_SPAIN1[] = "SPAIN1";
char MSGC_SPAIN2[] = "SPAIN2";
char MSGC_SPAIN3[] = "SPAIN3";
char MSGC_SPAIN4[] = "SPAIN4";
char MSGC_SPAIN5[] = "SPAIN5";
char MSGC_SPAIN6[] = "SPAIN6";
char MSGC_AUSTRALIA1[] = "AUSTRALIA1";
char MSGC_AUSTRALIA2[] = "AUSTRALIA2";
char MSGC_AUSTRALIA3[] = "AUSTRALIA3";
char MSGC_AUSTRALIA4[] = "AUSTRALIA4";
char MSGC_JAPAN1[] = "JAPAN1";
char MSGC_JAPAN2[] = "JAPAN2";
char MSGC_JAPAN3[] = "JAPAN3";
char MSGC_TAIWAN1[] = "TAIWAN1";
char MSGC_TAIWAN2[] = "TAIWAN2";
char MSGC_TAIWAN3[] = "TAIWAN3";
char MSGC_AUSTRIA1[] = "AUSTRIA1";
char MSGC_AUSTRIA2[] = "AUSTRIA2";
char MSGC_AUSTRIA3[] = "AUSTRIA3";
char MSGC_AUSTRIA4[] = "AUSTRIA4";
char MSGC_BELGIUM1[] = "BELGIUM1";
char MSGC_BELGIUM2[] = "BELGIUM2";
char MSGC_BELGIUM3[] = "BELGIUM3";
char MSGC_SWEDEN1[] = "SWEDEN1";
char MSGC_FINLAND1[] = "FINLAND1";
char MSGC_NETHERLAND1[] = "NETHERLAND1";
char MSGC_NETHERLAND2[] = "NETHERLAND2";
char MSGC_NORWAY1[] = "NORWAY1";
char MSGC_DENMARK1[] = "DENMARK1";
char MSGC_HUNGARY1[] = "HUNGARY1";
char MSGC_GENERAL1[] = "GENERAL1";
char MSGC_GENERAL2[] = "GENERAL2";
char MSGC_GENERAL3[] = "GENERAL3";
char MSGC_GENERAL4[] = "GENERAL4";
char MSGC_GENERAL5[] = "GENERAL5";
char MSGC_GENERAL6[] = "GENERAL6";
char MSGC_GENERAL7[] = "GENERAL7";
char MSGC_GENERAL8[] = "GENERAL8";
char MSGC_HONGKONG1[] = "HONGKONG1";
char MSGC_HONGKONG2[] = "HONGKONG2";
char MSGC_HONGKONG3[] = "HONGKONG3";

char* COINMSGC_TAB[] = {
    MSGC_USA1,
    MSGC_USA2,
    MSGC_USA3,
    MSGC_USA4,
    MSGC_USA5,
    MSGC_USA6,
    MSGC_USA7,
    MSGC_USA8,
    MSGC_USA9,
    MSGC_USA10,
    MSGC_USA11,
    MSGC_USA12,
    MSGC_GERMAN1,
    MSGC_GERMAN2,
    MSGC_GERMAN3,
    MSGC_GERMAN4,
    MSGC_GERMAN5,
    MSGC_GERMAN6,
    MSGC_GERMAN7,
    MSGC_GERMAN8,
    MSGC_FRENCH1,
    MSGC_FRENCH2,
    MSGC_FRENCH3,
    MSGC_FRENCH4,
    MSGC_FRENCH5,
    MSGC_FRENCH6,
    MSGC_FRENCH7,
    MSGC_FRENCH8,
    MSGC_FRENCH9,
    MSGC_FRENCH10,
    MSGC_FRENCH11,
    MSGC_CANADA1,
    MSGC_CANADA2,
    MSGC_CANADA3,
    MSGC_SWISS1,
    MSGC_SWISS2,
    MSGC_SWISS3,
    MSGC_ITALY1,
    MSGC_ITALY2,
    MSGC_ITALY3,
    MSGC_UK1,
    MSGC_UK2,
    MSGC_UK3,
    MSGC_SPAIN1,
    MSGC_SPAIN2,
    MSGC_SPAIN3,
    MSGC_SPAIN4,
    MSGC_SPAIN5,
    MSGC_SPAIN6,
    MSGC_AUSTRALIA1,
    MSGC_AUSTRALIA2,
    MSGC_AUSTRALIA3,
    MSGC_AUSTRALIA4,
    MSGC_JAPAN1,
    MSGC_JAPAN2,
    MSGC_JAPAN3,
    MSGC_TAIWAN1,
    MSGC_TAIWAN2,
    MSGC_TAIWAN3,
    MSGC_AUSTRIA1,
    MSGC_AUSTRIA2,
    MSGC_AUSTRIA3,
    MSGC_AUSTRIA4,
    MSGC_BELGIUM1,
    MSGC_BELGIUM2,
    MSGC_BELGIUM3,
    MSGC_SWEDEN1,
    MSGC_FINLAND1,
    MSGC_NETHERLAND1,
    MSGC_NETHERLAND2,
    MSGC_NORWAY1,
    MSGC_DENMARK1,
    MSGC_HUNGARY1,
    MSGC_GENERAL1,
    MSGC_GENERAL2,
    MSGC_GENERAL3,
    MSGC_GENERAL4,
    MSGC_GENERAL5,
    MSGC_GENERAL6,
    MSGC_GENERAL7,
    MSGC_GENERAL8,
};
/* ;	.word	MSGC_HONGKONG1,MSGC_HONGKONG2,MSGC_HONGKONG3
 */

static COINTAB_ENTRY USA1 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 3,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_13, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA2 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA3 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 4,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_14, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA4 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 5,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA5 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 6,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_16, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA6 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 8,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_18, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA7 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 8,
    .units_for_bonus = 20,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12D, MSG_35D, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA8 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 3,
    .credits_to_continue = 2,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA9 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 2,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA10 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 4,
    .credits_to_continue = 3,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA11 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 6,
    .units_for_bonus = 20,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1150D, MSG_45D, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY USA12 = {
    .coin = { 1, 1, 4, 4 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 4,
    .credits_to_continue = 3,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_100D,
    .coin4_denom_string = MSG_100D,
    .coin_denom = { 1, 1, 4, 4 },
};

static COINTAB_ENTRY GERMAN1 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 5,
    .min_units = 0,
    .credits_to_start = 3,
    .credits_to_continue = 3,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11DM, MSG_65DM, NULL },
    .coin1_denom_string = MSG_1DM,
    .coin2_denom_string = MSG_5DM,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY GERMAN2 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11DM, NULL, NULL },
    .coin1_denom_string = MSG_1DM,
    .coin2_denom_string = MSG_5DM,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY GERMAN3 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 5,
    .min_units = 0,
    .credits_to_start = 2,
    .credits_to_continue = 2,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11DM, MSG_65DM, NULL },
    .coin1_denom_string = MSG_1DM,
    .coin2_denom_string = MSG_5DM,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY GERMAN4 = {
    .coin = { 5, 2, 1, 0 },
    .units_per_credit = 3,
    .units_for_bonus = 5,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_13DM, MSG_25DM, NULL },
    .coin1_denom_string = MSG_5DM,
    .coin2_denom_string = MSG_2DM,
    .coin3_denom_string = MSG_1DM,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 5, 2, 1, 0 },
};

static COINTAB_ENTRY GERMAN5 = {
    .coin = { 5, 2, 1, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 5,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12DM, MSG_35DM, NULL },
    .coin1_denom_string = MSG_5DM,
    .coin2_denom_string = MSG_2DM,
    .coin3_denom_string = MSG_1DM,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 5, 2, 1, 0 },
};

static COINTAB_ENTRY GERMAN6 = {
    .coin = { 1, 6, 0, 0 },
    .units_per_credit = 3,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_13DM, MSG_25DM, NULL },
    .coin1_denom_string = MSG_1DM,
    .coin2_denom_string = MSG_5DM,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY GERMAN7 = {
    .coin = { 1, 6, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12DM, MSG_35DM, NULL },
    .coin1_denom_string = MSG_1DM,
    .coin2_denom_string = MSG_5DM,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY GERMAN8 = {
    .coin = { 1, 6, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 2,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11DM, MSG_65DM, NULL },
    .coin1_denom_string = MSG_1DM,
    .coin2_denom_string = MSG_5DM,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY FRENCH1 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 3,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_110F, MSG_220F, MSG_430F },
    .coin1_denom_string = MSG_10F,
    .coin2_denom_string = MSG_20F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY FRENCH2 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 4,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_110F, MSG_220F, MSG_540F },
    .coin1_denom_string = MSG_10F,
    .coin2_denom_string = MSG_20F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY FRENCH3 = {
    .coin = { 3, 6, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 12,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_110F, MSG_320F, MSG_740F },
    .coin1_denom_string = MSG_10F,
    .coin2_denom_string = MSG_20F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY FRENCH4 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 4,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15F, MSG_210F, MSG_520F },
    .coin1_denom_string = MSG_5F,
    .coin2_denom_string = MSG_10F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY FRENCH5 = {
    .coin = { 3, 6, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 12,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15F, MSG_310F, MSG_720F },
    .coin1_denom_string = MSG_5F,
    .coin2_denom_string = MSG_10F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY FRENCH6 = {
    .coin = { 1, 5, 10, 20 },
    .units_per_credit = 10,
    .units_for_bonus = 30,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_110F, MSG_220F, MSG_430F },
    .coin1_denom_string = MSG_1F,
    .coin2_denom_string = MSG_5F,
    .coin3_denom_string = MSG_10F,
    .coin4_denom_string = MSG_20F,
    .coin_denom = { 1, 5, 10, 20 },
};

static COINTAB_ENTRY FRENCH7 = {
    .coin = { 1, 5, 10, 20 },
    .units_per_credit = 10,
    .units_for_bonus = 40,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_110F, MSG_220F, MSG_540F },
    .coin1_denom_string = MSG_1F,
    .coin2_denom_string = MSG_5F,
    .coin3_denom_string = MSG_10F,
    .coin4_denom_string = MSG_20F,
    .coin_denom = { 1, 5, 10, 20 },
};

static COINTAB_ENTRY FRENCH8 = {
    .coin = { 1, 5, 10, 30 },
    .units_per_credit = 10,
    .units_for_bonus = 40,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_110F, MSG_320F, MSG_740F },
    .coin1_denom_string = MSG_1F,
    .coin2_denom_string = MSG_5F,
    .coin3_denom_string = MSG_10F,
    .coin4_denom_string = MSG_20F,
    .coin_denom = { 1, 5, 10, 20 },
};

static COINTAB_ENTRY FRENCH9 = {
    .coin = { 1, 5, 10, 20 },
    .units_per_credit = 5,
    .units_for_bonus = 20,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15F, MSG_210F, MSG_520F },
    .coin1_denom_string = MSG_1F,
    .coin2_denom_string = MSG_5F,
    .coin3_denom_string = MSG_10F,
    .coin4_denom_string = MSG_20F,
    .coin_denom = { 1, 5, 10, 20 },
};

static COINTAB_ENTRY FRENCH10 = {
    .coin = { 1, 5, 15, 30 },
    .units_per_credit = 5,
    .units_for_bonus = 30,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15F, MSG_310F, MSG_720F },
    .coin1_denom_string = MSG_1F,
    .coin2_denom_string = MSG_5F,
    .coin3_denom_string = MSG_10F,
    .coin4_denom_string = MSG_20F,
    .coin_denom = { 1, 5, 10, 20 },
};

static COINTAB_ENTRY FRENCH11 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 3,
    .units_for_bonus = 10,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_13X5F, MSG_450F, NULL },
    .coin1_denom_string = MSG_5F,
    .coin2_denom_string = MSG_10F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY CANADA1 = {
    .coin = { 1, 4, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12, MSG_21D, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_100D,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 4, 0, 0 },
};

static COINTAB_ENTRY CANADA2 = {
    .coin = { 1, 4, 0, 0 },
    .units_per_credit = 4,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11D, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_100D,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 4, 0, 0 },
};

static COINTAB_ENTRY CANADA3 = {
    .coin = { 1, 4, 0, 0 },
    .units_per_credit = 4,
    .units_for_bonus = 8,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11D, MSG_32D, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_100D,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 4, 0, 0 },
};

static COINTAB_ENTRY SWISS1 = {
    .coin = { 1, 6, 0, 0 },
    .units_per_credit = 3,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_13F, MSG_25F, NULL },
    .coin1_denom_string = MSG_1F,
    .coin2_denom_string = MSG_5F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY SWISS2 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11F, MSG_55F, NULL },
    .coin1_denom_string = MSG_1F,
    .coin2_denom_string = MSG_5F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY SWISS3 = {
    .coin = { 1, 6, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12F, MSG_35F, NULL },
    .coin1_denom_string = MSG_1F,
    .coin2_denom_string = MSG_5F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY ITALY1 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1500L, NULL, NULL },
    .coin1_denom_string = MSG_500L,
    .coin2_denom_string = MSG_500L,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY ITALY2 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12X500L, NULL, NULL },
    .coin1_denom_string = MSG_500L,
    .coin2_denom_string = MSG_500L,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY ITALY3 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 4,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_14X500L, NULL, NULL },
    .coin1_denom_string = MSG_500L,
    .coin2_denom_string = MSG_500L,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY UK1 = {
    .coin = { 10, 5, 2, 1 },
    .units_per_credit = 5,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_150P, NULL, NULL },
    .coin1_denom_string = MSG_100P,
    .coin2_denom_string = MSG_50P,
    .coin3_denom_string = MSG_20P,
    .coin4_denom_string = MSG_10P,
    .coin_denom = { 10, 5, 2, 1 },
};

static COINTAB_ENTRY UK2 = {
    .coin = { 10, 5, 2, 1 },
    .units_per_credit = 10,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1100P, NULL, NULL },
    .coin1_denom_string = MSG_100P,
    .coin2_denom_string = MSG_50P,
    .coin3_denom_string = MSG_20P,
    .coin4_denom_string = MSG_10P,
    .coin_denom = { 10, 5, 2, 1 },
};

static COINTAB_ENTRY UK3 = {
    .coin = { 1, 3, 0, 0 },
    .units_per_credit = 3,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_13X20P, MSG_150P, NULL },
    .coin1_denom_string = MSG_20P,
    .coin2_denom_string = MSG_50P,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 2, 5, 0, 0 },
};

static COINTAB_ENTRY SPAIN1 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 5,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1100PESETA, MSG_6500PESETA, NULL },
    .coin1_denom_string = MSG_100P,
    .coin2_denom_string = MSG_500P,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY SPAIN2 = {
    .coin = { 3, 14, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_32X100PESETA, MSG_7500PESETA, NULL },
    .coin1_denom_string = MSG_100P,
    .coin2_denom_string = MSG_500P,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY SPAIN3 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1100PESETA, MSG_5500PESETA, NULL },
    .coin1_denom_string = MSG_100P,
    .coin2_denom_string = MSG_500P,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY SPAIN4 = {
    .coin = { 1, 6, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12X100PESETA, MSG_3500PESETA, NULL },
    .coin1_denom_string = MSG_100P,
    .coin2_denom_string = MSG_500P,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY SPAIN5 = {
    .coin = { 2, 10, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_2100PESETA, MSG_10500PESETA, NULL },
    .coin1_denom_string = MSG_100P,
    .coin2_denom_string = MSG_500P,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY SPAIN6 = {
    .coin = { 1, 4, 8, 20 },
    .units_per_credit = 4,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1100PESETA, NULL, NULL },
    .coin1_denom_string = MSG_25P,
    .coin2_denom_string = MSG_100P,
    .coin3_denom_string = MSG_200P,
    .coin4_denom_string = MSG_500P,
    .coin_denom = { 1, 4, 8, 20 },
};

static COINTAB_ENTRY AUSTRALIA1 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 5,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15X20C, MSG_11D, NULL },
    .coin1_denom_string = MSG_20C,
    .coin2_denom_string = MSG_100D,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY AUSTRALIA2 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 10,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_110X20C, MSG_12D, NULL },
    .coin1_denom_string = MSG_20C,
    .coin2_denom_string = MSG_100D,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY AUSTRALIA3 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12D, NULL, NULL },
    .coin1_denom_string = MSG_100D,
    .coin2_denom_string = MSG_200D,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY AUSTRALIA4 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 3,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_13D, NULL, NULL },
    .coin1_denom_string = MSG_100D,
    .coin2_denom_string = MSG_200D,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY JAPAN1 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11, NULL, NULL },
    .coin1_denom_string = MSG_100YEN,
    .coin2_denom_string = MSG_100YEN,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY JAPAN2 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12, NULL, NULL },
    .coin1_denom_string = MSG_100YEN,
    .coin2_denom_string = MSG_100YEN,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY JAPAN3 = {
    .coin = { 1, 3, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1100YEN, MSG_3500YEN, NULL },
    .coin1_denom_string = MSG_100YEN,
    .coin2_denom_string = MSG_500YEN,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY TAIWAN1 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1NT10, NULL, NULL },
    .coin1_denom_string = MSG_NT10,
    .coin2_denom_string = MSG_NT10,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY TAIWAN2 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12, NULL, NULL },
    .coin1_denom_string = MSG_25C,
    .coin2_denom_string = MSG_25C,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY TAIWAN3 = {
    .coin = { 2, 2, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_21, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY AUSTRIA1 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15SCHIL, MSG_210SCHIL, NULL },
    .coin1_denom_string = MSG_5SCHIL,
    .coin2_denom_string = MSG_10SCHIL,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY AUSTRIA2 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 2,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_110SCHIL, MSG_320SCHIL, NULL },
    .coin1_denom_string = MSG_10SCHIL,
    .coin2_denom_string = MSG_20SCHIL,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY AUSTRIA3 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12X5SCHIL, MSG_110SCHIL, NULL },
    .coin1_denom_string = MSG_5SCHIL,
    .coin2_denom_string = MSG_10SCHIL,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY AUSTRIA4 = {
    .coin = { 1, 2, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 3,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12X10SCHIL, MSG_230SCHIL, NULL },
    .coin1_denom_string = MSG_10SCHIL,
    .coin2_denom_string = MSG_20SCHIL,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 0, 0 },
};

static COINTAB_ENTRY BELGIUM1 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_120F, NULL, NULL },
    .coin1_denom_string = MSG_20F,
    .coin2_denom_string = MSG_20F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY BELGIUM2 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12X20F, NULL, NULL },
    .coin1_denom_string = MSG_20F,
    .coin2_denom_string = MSG_20F,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY BELGIUM3 = {
    .coin = { 1, 4, 10, 0 },
    .units_per_credit = 4,
    .units_for_bonus = 10,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_120F, MSG_350F, NULL },
    .coin1_denom_string = MSG_5F,
    .coin2_denom_string = MSG_20F,
    .coin3_denom_string = MSG_50F,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 4, 10, 0 },
};

static COINTAB_ENTRY SWEDEN1 = {
    .coin = { 1, 5, 10, 0 },
    .units_per_credit = 5,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15KRONA, NULL, NULL },
    .coin1_denom_string = MSG_1KRONA,
    .coin2_denom_string = MSG_5KRONA,
    .coin3_denom_string = MSG_10KRONA,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 10, 0 },
};

static COINTAB_ENTRY FINLAND1 = {
    .coin = { 1, 5, 10, 0 },
    .units_per_credit = 5,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15MARKKA, NULL, NULL },
    .coin1_denom_string = MSG_1MARKKA,
    .coin2_denom_string = MSG_5MARKKA,
    .coin3_denom_string = MSG_10MARKKA,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 10, 0 },
};

static COINTAB_ENTRY NETHERLAND1 = {
    .coin = { 1, 1, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_1250HFI, NULL, NULL },
    .coin1_denom_string = MSG_250HFI,
    .coin2_denom_string = MSG_250HFI,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 1, 0, 0 },
};

static COINTAB_ENTRY NETHERLAND2 = {
    .coin = { 1, 3, 0, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11HFI, MSG_3250HFI, NULL },
    .coin1_denom_string = MSG_1HFI,
    .coin2_denom_string = MSG_250HFI,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 3, 0, 0 },
};

static COINTAB_ENTRY NORWAY1 = {
    .coin = { 1, 2, 4, 0 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 0,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15KRONE, NULL, NULL },
    .coin1_denom_string = MSG_5KRONE,
    .coin2_denom_string = MSG_10KRONE,
    .coin3_denom_string = MSG_20KRONE,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 2, 4, 0 },
};

static COINTAB_ENTRY DENMARK1 = {
    .coin = { 1, 5, 10, 20 },
    .units_per_credit = 5,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15KRONE, NULL, NULL },
    .coin1_denom_string = MSG_1KRONE,
    .coin2_denom_string = MSG_5KRONE,
    .coin3_denom_string = MSG_10KRONE,
    .coin4_denom_string = MSG_20KRONE,
    .coin_denom = { 1, 5, 10, 20 },
};

static COINTAB_ENTRY HUNGARY1 = {
    .coin = { 1, 5, 0, 0 },
    .units_per_credit = 4,
    .units_for_bonus = 7,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_14X20FORINT, MSG_2140FORINT, NULL },
    .coin1_denom_string = MSG_20FORINT,
    .coin2_denom_string = MSG_100FORINT,
    .coin3_denom_string = MSG_NULL,
    .coin4_denom_string = MSG_NULL,
    .coin_denom = { 1, 5, 0, 0 },
};

static COINTAB_ENTRY GENERAL1 = {
    .coin = { 1, 1, 1, 4 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_11, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_1COIN,
    .coin4_denom_string = MSG_4COINS,
    .coin_denom = { 1, 1, 1, 4 },
};

static COINTAB_ENTRY GENERAL2 = {
    .coin = { 1, 1, 1, 4 },
    .units_per_credit = 2,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_12, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_1COIN,
    .coin4_denom_string = MSG_4COINS,
    .coin_denom = { 1, 1, 1, 4 },
};

static COINTAB_ENTRY GENERAL3 = {
    .coin = { 1, 1, 1, 4 },
    .units_per_credit = 3,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_13, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_1COIN,
    .coin4_denom_string = MSG_4COINS,
    .coin_denom = { 1, 1, 1, 4 },
};

static COINTAB_ENTRY GENERAL4 = {
    .coin = { 1, 1, 1, 4 },
    .units_per_credit = 4,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_14, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_1COIN,
    .coin4_denom_string = MSG_4COINS,
    .coin_denom = { 1, 1, 1, 4 },
};

static COINTAB_ENTRY GENERAL5 = {
    .coin = { 1, 1, 1, 4 },
    .units_per_credit = 5,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_15, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_1COIN,
    .coin4_denom_string = MSG_4COINS,
    .coin_denom = { 1, 1, 1, 4 },
};

static COINTAB_ENTRY GENERAL6 = {
    .coin = { 1, 1, 1, 4 },
    .units_per_credit = 6,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_16, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_1COIN,
    .coin4_denom_string = MSG_4COINS,
    .coin_denom = { 1, 1, 1, 4 },
};

static COINTAB_ENTRY GENERAL7 = {
    .coin = { 2, 2, 2, 8 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_21, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_1COIN,
    .coin4_denom_string = MSG_4COINS,
    .coin_denom = { 1, 1, 1, 4 },
};

static COINTAB_ENTRY GENERAL8 = {
    .coin = { 3, 3, 3, 12 },
    .units_per_credit = 1,
    .units_for_bonus = 0,
    .min_units = 0,
    .credits_to_start = 1,
    .credits_to_continue = 1,
    .show_partial_credits = 1,
    .unused0 = 0,
    .unused1 = 0,
    .message_lines = { MSG_31, NULL, NULL },
    .coin1_denom_string = MSG_1COIN,
    .coin2_denom_string = MSG_1COIN,
    .coin3_denom_string = MSG_1COIN,
    .coin4_denom_string = MSG_4COINS,
    .coin_denom = { 1, 1, 1, 4 },
};

COINTAB_ENTRY* COIN_TABLE[] = {
    &USA1,
    &USA2,
    &USA3,
    &USA4,
    &USA5,
    &USA6,
    &USA7,
    &USA8,
    &USA9,
    &USA10,
    &USA11,
    &USA12,
    &GERMAN1,
    &GERMAN2,
    &GERMAN3,
    &GERMAN4,
    &GERMAN5,
    &GERMAN6,
    &GERMAN7,
    &GERMAN8,
    &FRENCH1,
    &FRENCH2,
    &FRENCH3,
    &FRENCH4,
    &FRENCH5,
    &FRENCH6,
    &FRENCH7,
    &FRENCH8,
    &FRENCH9,
    &FRENCH10,
    &FRENCH11,
    &CANADA1,
    &CANADA2,
    &CANADA3,
    &SWISS1,
    &SWISS2,
    &SWISS3,
    &ITALY1,
    &ITALY2,
    &ITALY3,
    &UK1,
    &UK2,
    &UK3,
    &SPAIN1,
    &SPAIN2,
    &SPAIN3,
    &SPAIN4,
    &SPAIN5,
    &SPAIN6,
    &AUSTRALIA1,
    &AUSTRALIA2,
    &AUSTRALIA3,
    &AUSTRALIA4,
    &JAPAN1,
    &JAPAN2,
    &JAPAN3,
    &TAIWAN1,
    &TAIWAN2,
    &TAIWAN3,
    &AUSTRIA1,
    &AUSTRIA2,
    &AUSTRIA3,
    &AUSTRIA4,
    &BELGIUM1,
    &BELGIUM2,
    &BELGIUM3,
    &SWEDEN1,
    &FINLAND1,
    &NETHERLAND1,
    &NETHERLAND2,
    &NORWAY1,
    &DENMARK1,
    &HUNGARY1,
    &GENERAL1,
    &GENERAL2,
    &GENERAL3,
    &GENERAL4,
    &GENERAL5,
    &GENERAL6,
    &GENERAL7,
    &GENERAL8,
};

// unsigned COIN_TABLE_LEN = sizeof(COIN_TAB) / sizeof(coin_table[0]);

// #define USA_OFF ((USA1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define GERMAN_OFF ((GERMAN1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define FRENCH_OFF ((FRENCH1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define CANADA_OFF ((CANADA1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define SWISS_OFF ((SWISS1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define ITALY_OFF ((ITALY1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define UK_OFF ((UK1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define SPAIN_OFF ((SPAIN1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define AUSTRALIA_OFF ((AUSTRALIA1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define JAPAN_OFF ((JAPAN1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define TAIWAN_OFF ((TAIWAN1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define AUSTRIA_OFF ((AUSTRIA1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define BELGIUM_OFF ((BELGIUM1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define SWEDEN_OFF ((SWEDEN1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define FINLAND_OFF ((FINLAND1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define NETHERLAND_OFF ((NETHERLAND1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define NORWAY_OFF ((NORWAY1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define DENMARK_OFF ((DENMARK1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define HUNAGRY_OFF ((HUNGARY1 - COIN_TABLE) / COIN_ENTRY_SIZE)
// #define GENERAL_OFF ((GENERAL1 - COIN_TABLE) / COIN_ENTRY_SIZE)
/* ;HONGKONG_OFF	.set (HONGKONG1-COIN_TABLE)/COIN_ENTRY_SIZE */

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
COINTAB_ENTRY* COUNTRY_TAB[] = {
    &USA1,       // 1
    &GERMAN1,    // 2
    &FRENCH1,    // 3
    &CANADA1,    // 4
    &SWISS1,     // 5
    &ITALY1,     // 6
    &UK1,        // 7
    &SPAIN1,     // 8
    &AUSTRALIA1, // 9
    &JAPAN1,     // 10
    &TAIWAN1,    // 11
    &AUSTRIA1,   // 12
    &BELGIUM1,   // 13
    &SWEDEN1,    // 14	OTHER COUNTRYS
    &GENERAL1,   // 15
};
/* ;	.word	HONGKONG_OFF	;16
;The COUNTRY_DEFAULTS TABLE is used for the dip switch settings
;The COUNTRY_DEFAULTS table is needed because the COUNTRY_TAB is used for the menu system
;in diag and differs from the dipswitch settings. It also alows us to insert countrys without
;altering the dufault dip switch settings
;format OF TABLE ENTRY PER COUNTRY
;	.word	mode_offset0,mode_offset1,mode_offset2,mode_offset3
 */
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
// clang-format off
COINTAB_ENTRY* COUNTRY_DEFAULTS[] = {
    &USA1, &USA3, &USA7, &USA8, // 0
    &GERMAN1, &GERMAN2, &GERMAN3, &GERMAN4, // 1
    &FRENCH1, &FRENCH2, &FRENCH3, &FRENCH4, // 2
    &CANADA1, &CANADA2, &CANADA3, &CANADA1, // 3
    &SWISS1, &SWISS2, &SWISS3, &SWISS1, // 4
    &ITALY1, &ITALY2, &ITALY3, &ITALY1, // 5
    &UK1, &UK2, &UK3, &UK1, // 6
    &SPAIN1, &SPAIN2, &SPAIN3, &SPAIN4, // 7
    &AUSTRALIA1, &AUSTRALIA2, &AUSTRALIA3, &AUSTRALIA4, // 8
    &JAPAN1, &JAPAN2, &JAPAN3, &JAPAN1, // 9
    &TAIWAN1, &TAIWAN2, &TAIWAN3, &TAIWAN1, // 10
    &AUSTRIA1, &AUSTRIA2, &AUSTRIA3, &AUSTRIA4, // 11
    &BELGIUM1, &BELGIUM2, &BELGIUM3, &BELGIUM1, // 12
    &SWEDEN1, &SWEDEN1, &SWEDEN1, &SWEDEN1, // 13
    &FINLAND1, &FINLAND1, &FINLAND1, &FINLAND1, // 14
    &NETHERLAND1, &NETHERLAND2, &NETHERLAND1, &NETHERLAND2, // 15
    &NORWAY1, &NORWAY1, &NORWAY1, &NORWAY1, // 16
    &DENMARK1, &DENMARK1, &DENMARK1, &DENMARK1, // 17
    &HUNGARY1, &HUNGARY1, &HUNGARY1, &HUNGARY1, // 18
    &GENERAL1, &GENERAL3, &GENERAL5, &GENERAL7, // 19
};
// clang-format on

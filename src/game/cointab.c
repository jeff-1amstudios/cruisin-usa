// NOEDIT
#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "cmos.h"
#include "globals.h"
#include "macs.h"
#include "port.h"
#include "sys.h"
#include "vunit.h"

/*
 * Source module: asm/COINTAB.ASM
 */

#define COIN_TABLEI COIN_TABLE
#define COIN_TABLEENDI COIN_TABLEEND
#define COUNTRY_TABI COUNTRY_TAB
#define COUNTRY_DEFAULTSI COUNTRY_DEFAULTS
#define COINMSGC_TABI COINMSGC_TAB
#define COUNTRY_DEFAULTS_ENDI COUNTRY_DEFAULTS_END

extern uintptr_t COINMSGC_TAB[];
extern const char MSG_NULL[];
extern const char MSG_41[];
extern const char MSG_32D[];
extern const char MSG_31[];
extern const char MSG_21D[];
extern const char MSG_21[];
extern const char MSG_11[];
extern const char MSG_11D[];
extern const char MSG_11DM[];
extern const char MSG_1150D[];
extern const char MSG_12[];
extern const char MSG_12D[];
extern const char MSG_12DM[];
extern const char MSG_13[];
extern const char MSG_13D[];
extern const char MSG_13DM[];
extern const char MSG_14[];
extern const char MSG_15[];
extern const char MSG_15X20C[];
extern const char MSG_110X20C[];
extern const char MSG_16[];
extern const char MSG_18[];
extern const char MSG_25DM[];
extern const char MSG_35D[];
extern const char MSG_35DM[];
extern const char MSG_45D[];
extern const char MSG_65D[];
extern const char MSG_65DM[];
extern const char MSG_11F[];
extern const char MSG_12F[];
extern const char MSG_13F[];
extern const char MSG_15F[];
extern const char MSG_13X5F[];
extern const char MSG_110F[];
extern const char MSG_120F[];
extern const char MSG_12X20F[];
extern const char MSG_25F[];
extern const char MSG_210F[];
extern const char MSG_220F[];
extern const char MSG_35F[];
extern const char MSG_310F[];
extern const char MSG_320F[];
extern const char MSG_350F[];
extern const char MSG_430F[];
extern const char MSG_450F[];
extern const char MSG_55F[];
extern const char MSG_520F[];
extern const char MSG_540F[];
extern const char MSG_720F[];
extern const char MSG_740F[];
extern const char MSG_1500L[];
extern const char MSG_12X500L[];
extern const char MSG_14X500L[];
extern const char MSG_150P[];
extern const char MSG_13X20P[];
extern const char MSG_1100P[];
extern const char MSG_1100PESETA[];
extern const char MSG_12X100PESETA[];
extern const char MSG_2100PESETA[];
extern const char MSG_32X100PESETA[];
extern const char MSG_3500PESETA[];
extern const char MSG_5500PESETA[];
extern const char MSG_6500PESETA[];
extern const char MSG_7500PESETA[];
extern const char MSG_10500PESETA[];
extern const char MSG_1100YEN[];
extern const char MSG_3500YEN[];
extern const char MSG_1NT10[];
extern const char MSG_15SCHIL[];
extern const char MSG_12X5SCHIL[];
extern const char MSG_12X10SCHIL[];
extern const char MSG_110SCHIL[];
extern const char MSG_210SCHIL[];
extern const char MSG_230SCHIL[];
extern const char MSG_320SCHIL[];
extern const char MSG_15KRONA[];
extern const char MSG_15MARKKA[];
extern const char MSG_15KRONE[];
extern const char MSG_11HFI[];
extern const char MSG_1250HFI[];
extern const char MSG_3250HFI[];
extern const char MSG_14X20FORINT[];
extern const char MSG_2140FORINT[];
extern const char MSG_25C[];
extern const char MSG_20C[];
extern const char MSG_100D[];
extern const char MSG_200D[];
extern const char MSG_1DM[];
extern const char MSG_2DM[];
extern const char MSG_5DM[];
extern const char MSG_1F[];
extern const char MSG_5F[];
extern const char MSG_10F[];
extern const char MSG_20F[];
extern const char MSG_50F[];
extern const char MSG_500L[];
extern const char MSG_10P[];
extern const char MSG_20P[];
extern const char MSG_25P[];
extern const char MSG_50P[];
extern const char MSG_100P[];
extern const char MSG_200P[];
extern const char MSG_500P[];
extern const char MSG_100YEN[];
extern const char MSG_500YEN[];
extern const char MSG_NT10[];
extern const char MSG_1COIN[];
extern const char MSG_4COINS[];
extern const char MSG_5SCHIL[];
extern const char MSG_10SCHIL[];
extern const char MSG_20SCHIL[];
extern const char MSG_1KRONA[];
extern const char MSG_5KRONA[];
extern const char MSG_10KRONA[];
extern const char MSG_1MARKKA[];
extern const char MSG_5MARKKA[];
extern const char MSG_10MARKKA[];
extern const char MSG_1HFI[];
extern const char MSG_250HFI[];
extern const char MSG_1KRONE[];
extern const char MSG_5KRONE[];
extern const char MSG_10KRONE[];
extern const char MSG_20KRONE[];
extern const char MSG_20FORINT[];
extern const char MSG_100FORINT[];
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
extern const char MSGC_HONGKONG1[];
extern const char MSGC_HONGKONG2[];
extern const char MSGC_HONGKONG3[];
extern uintptr_t USA1[];
extern uintptr_t USA2[];
extern uintptr_t USA3[];
extern uintptr_t USA4[];
extern uintptr_t USA5[];
extern uintptr_t USA6[];
extern uintptr_t USA7[];
extern uintptr_t USA8[];
extern uintptr_t USA9[];
extern uintptr_t USA10[];
extern uintptr_t USA11[];
extern uintptr_t USA12[];
extern uintptr_t GERMAN1[];
extern uintptr_t GERMAN2[];
extern uintptr_t GERMAN3[];
extern uintptr_t GERMAN4[];
extern uintptr_t GERMAN5[];
extern uintptr_t GERMAN6[];
extern uintptr_t GERMAN7[];
extern uintptr_t GERMAN8[];
extern uintptr_t FRENCH1[];
extern uintptr_t FRENCH2[];
extern uintptr_t FRENCH3[];
extern uintptr_t FRENCH4[];
extern uintptr_t FRENCH5[];
extern uintptr_t FRENCH6[];
extern uintptr_t FRENCH7[];
extern uintptr_t FRENCH8[];
extern uintptr_t FRENCH9[];
extern uintptr_t FRENCH10[];
extern uintptr_t FRENCH11[];
extern uintptr_t CANADA1[];
extern uintptr_t CANADA2[];
extern uintptr_t CANADA3[];
extern uintptr_t SWISS1[];
extern uintptr_t SWISS2[];
extern uintptr_t SWISS3[];
extern uintptr_t ITALY1[];
extern uintptr_t ITALY2[];
extern uintptr_t ITALY3[];
extern uintptr_t UK1[];
extern uintptr_t UK2[];
extern uintptr_t UK3[];
extern uintptr_t SPAIN1[];
extern uintptr_t SPAIN2[];
extern uintptr_t SPAIN3[];
extern uintptr_t SPAIN4[];
extern uintptr_t SPAIN5[];
extern uintptr_t SPAIN6[];
extern uintptr_t AUSTRALIA1[];
extern uintptr_t AUSTRALIA2[];
extern uintptr_t AUSTRALIA3[];
extern uintptr_t AUSTRALIA4[];
extern uintptr_t JAPAN1[];
extern uintptr_t JAPAN2[];
extern uintptr_t JAPAN3[];
extern uintptr_t TAIWAN1[];
extern uintptr_t TAIWAN2[];
extern uintptr_t TAIWAN3[];
extern uintptr_t AUSTRIA1[];
extern uintptr_t AUSTRIA2[];
extern uintptr_t AUSTRIA3[];
extern uintptr_t AUSTRIA4[];
extern uintptr_t BELGIUM1[];
extern uintptr_t BELGIUM2[];
extern uintptr_t BELGIUM3[];
extern uintptr_t SWEDEN1[];
extern uintptr_t FINLAND1[];
extern uintptr_t NETHERLAND1[];
extern uintptr_t NETHERLAND2[];
extern uintptr_t NORWAY1[];
extern uintptr_t DENMARK1[];
extern uintptr_t HUNGARY1[];
extern uintptr_t GENERAL1[];
extern uintptr_t GENERAL2[];
extern uintptr_t GENERAL3[];
extern uintptr_t GENERAL4[];
extern uintptr_t GENERAL5[];
extern uintptr_t GENERAL6[];
extern uintptr_t GENERAL7[];
extern uintptr_t GENERAL8[];
extern int COUNTRY_TAB[];
extern uintptr_t* COUNTRY_DEFAULTS[];

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
uintptr_t COINMSGC_TAB[] = {
    (uintptr_t)&MSGC_USA1,
    (uintptr_t)&MSGC_USA2,
    (uintptr_t)&MSGC_USA3,
    (uintptr_t)&MSGC_USA4,
    (uintptr_t)&MSGC_USA5,
    (uintptr_t)&MSGC_USA6,
    (uintptr_t)&MSGC_USA7,
    (uintptr_t)&MSGC_USA8,
    (uintptr_t)&MSGC_USA9,
    (uintptr_t)&MSGC_USA10,
    (uintptr_t)&MSGC_USA11,
    (uintptr_t)&MSGC_USA12,
    (uintptr_t)&MSGC_GERMAN1,
    (uintptr_t)&MSGC_GERMAN2,
    (uintptr_t)&MSGC_GERMAN3,
    (uintptr_t)&MSGC_GERMAN4,
    (uintptr_t)&MSGC_GERMAN5,
    (uintptr_t)&MSGC_GERMAN6,
    (uintptr_t)&MSGC_GERMAN7,
    (uintptr_t)&MSGC_GERMAN8,
    (uintptr_t)&MSGC_FRENCH1,
    (uintptr_t)&MSGC_FRENCH2,
    (uintptr_t)&MSGC_FRENCH3,
    (uintptr_t)&MSGC_FRENCH4,
    (uintptr_t)&MSGC_FRENCH5,
    (uintptr_t)&MSGC_FRENCH6,
    (uintptr_t)&MSGC_FRENCH7,
    (uintptr_t)&MSGC_FRENCH8,
    (uintptr_t)&MSGC_FRENCH9,
    (uintptr_t)&MSGC_FRENCH10,
    (uintptr_t)&MSGC_FRENCH11,
    (uintptr_t)&MSGC_CANADA1,
    (uintptr_t)&MSGC_CANADA2,
    (uintptr_t)&MSGC_CANADA3,
    (uintptr_t)&MSGC_SWISS1,
    (uintptr_t)&MSGC_SWISS2,
    (uintptr_t)&MSGC_SWISS3,
    (uintptr_t)&MSGC_ITALY1,
    (uintptr_t)&MSGC_ITALY2,
    (uintptr_t)&MSGC_ITALY3,
    (uintptr_t)&MSGC_UK1,
    (uintptr_t)&MSGC_UK2,
    (uintptr_t)&MSGC_UK3,
    (uintptr_t)&MSGC_SPAIN1,
    (uintptr_t)&MSGC_SPAIN2,
    (uintptr_t)&MSGC_SPAIN3,
    (uintptr_t)&MSGC_SPAIN4,
    (uintptr_t)&MSGC_SPAIN5,
    (uintptr_t)&MSGC_SPAIN6,
    (uintptr_t)&MSGC_AUSTRALIA1,
    (uintptr_t)&MSGC_AUSTRALIA2,
    (uintptr_t)&MSGC_AUSTRALIA3,
    (uintptr_t)&MSGC_AUSTRALIA4,
    (uintptr_t)&MSGC_JAPAN1,
    (uintptr_t)&MSGC_JAPAN2,
    (uintptr_t)&MSGC_JAPAN3,
    (uintptr_t)&MSGC_TAIWAN1,
    (uintptr_t)&MSGC_TAIWAN2,
    (uintptr_t)&MSGC_TAIWAN3,
    (uintptr_t)&MSGC_AUSTRIA1,
    (uintptr_t)&MSGC_AUSTRIA2,
    (uintptr_t)&MSGC_AUSTRIA3,
    (uintptr_t)&MSGC_AUSTRIA4,
    (uintptr_t)&MSGC_BELGIUM1,
    (uintptr_t)&MSGC_BELGIUM2,
    (uintptr_t)&MSGC_BELGIUM3,
    (uintptr_t)&MSGC_SWEDEN1,
    (uintptr_t)&MSGC_FINLAND1,
    (uintptr_t)&MSGC_NETHERLAND1,
    (uintptr_t)&MSGC_NETHERLAND2,
    (uintptr_t)&MSGC_NORWAY1,
    (uintptr_t)&MSGC_DENMARK1,
    (uintptr_t)&MSGC_HUNGARY1,
    (uintptr_t)&MSGC_GENERAL1,
    (uintptr_t)&MSGC_GENERAL2,
    (uintptr_t)&MSGC_GENERAL3,
    (uintptr_t)&MSGC_GENERAL4,
    (uintptr_t)&MSGC_GENERAL5,
    (uintptr_t)&MSGC_GENERAL6,
    (uintptr_t)&MSGC_GENERAL7,
    (uintptr_t)&MSGC_GENERAL8,
};
/* ;	.word	MSGC_HONGKONG1,MSGC_HONGKONG2,MSGC_HONGKONG3
 */
const char MSG_NULL[] = "";
const char MSG_41[] = "4 CREDITS / 1 COIN";
const char MSG_32D[] = "3 CREDITS / 2 DOLLARS";
const char MSG_31[] = "3 CREDITS / 1 COIN";
const char MSG_21D[] = "2 CREDITS / 1 DOLLAR";
const char MSG_21[] = "2 CREDITS / 1 COIN";
const char MSG_11[] = "1 COIN / 1 CREDIT";
const char MSG_11D[] = "1 DOLLAR / 1 CREDIT";
const char MSG_11DM[] = "1DM / 1 CREDIT";
const char MSG_1150D[] = "1 50 DOLLAR / 1 CREDIT";
const char MSG_12[] = "2 COINS / 1 CREDIT";
const char MSG_12D[] = "2 DOLLARS / 1 CREDIT";
const char MSG_12DM[] = "2DM / 1 CREDIT";
const char MSG_13[] = "3 COINS / 1 CREDIT";
const char MSG_13D[] = "3 DOLLARS / 1 CREDIT";
const char MSG_13DM[] = "3DM / 1 CREDIT";
const char MSG_14[] = "4 COINS / 1 CREDIT";
const char MSG_15[] = "5 COINS / 1 CREDIT";
const char MSG_15X20C[] = "5 X 2O CENTS / 1 CREDIT";
const char MSG_110X20C[] = "1O X 2O CENTS / 1 CREDIT";
const char MSG_16[] = "6 COINS / 1 CREDIT";
const char MSG_18[] = "8 COINS / 1 CREDIT";
const char MSG_25DM[] = "5DM / 2 CREDITS";
const char MSG_35D[] = "5 DOLLARS / 3 CREDITS";
const char MSG_35DM[] = "5DM / 3 CREDITS";
const char MSG_45D[] = "5 DOLLARS / 4 CREDITS";
const char MSG_65D[] = "5 DOLLARS / 6 CREDITS";
const char MSG_65DM[] = "5DM / 6 CREDITS";
const char MSG_11F[] = "1F / 1 CREDIT";
const char MSG_12F[] = "2F / 1 CREDIT";
const char MSG_13F[] = "3F / 1 CREDIT";
const char MSG_15F[] = "5F / 1 CREDIT";
const char MSG_13X5F[] = "3 X 5F / 1 CREDIT";
const char MSG_110F[] = "1OF / 1 CREDIT";
const char MSG_120F[] = "2OF / 1 CREDIT";
const char MSG_12X20F[] = "2 X 2OF / 1 CREDIT";
const char MSG_25F[] = "5F / 2 CREDITS";
const char MSG_210F[] = "1OF / 2 CREDITS";
const char MSG_220F[] = "2OF / 2 CREDITS";
const char MSG_35F[] = "5F / 3 CREDITS";
const char MSG_310F[] = "1OF / 3 CREDITS";
const char MSG_320F[] = "2OF / 3 CREDITS";
const char MSG_350F[] = "5OF / 3 CREDITS";
const char MSG_430F[] = "3OF / 4 CREDITS";
const char MSG_450F[] = "5OF / 4 CREDITS";
const char MSG_55F[] = "5F / 5 CREDITS";
const char MSG_520F[] = "2OF / 5 CREDITS";
const char MSG_540F[] = "4OF / 5 CREDITS";
const char MSG_720F[] = "2OF / 7 CREDITS";
const char MSG_740F[] = "4OF / 7 CREDITS";
const char MSG_1500L[] = "5OO LIRE / 1 CREDIT";
const char MSG_12X500L[] = "2 X 5OO LIRE / 1 CREDIT";
const char MSG_14X500L[] = "4 X 5OO LIRE / 1 CREDIT";
const char MSG_150P[] = "5OP / 1 CREDIT";
const char MSG_13X20P[] = "3 X 2OP / 1 CREDIT";
const char MSG_1100P[] = "1OOP / 1 CREDIT";
const char MSG_1100PESETA[] = "1OOPESETA / 1 CREDIT";
const char MSG_12X100PESETA[] = "2 X 1OOPESETA / 1 CREDIT";
const char MSG_2100PESETA[] = "1OOPESETA / 2 CREDITS";
const char MSG_32X100PESETA[] = "2 X 1OOPESETA / 3 CREDITS";
const char MSG_3500PESETA[] = "5OOPESETA / 3 CREDITS";
const char MSG_5500PESETA[] = "5OOPESETA / 5 CREDITS";
const char MSG_6500PESETA[] = "5OOPESETA / 6 CREDITS";
const char MSG_7500PESETA[] = "5OOPESETA / 7 CREDITS";
const char MSG_10500PESETA[] = "5OOPESETA / 1O CREDITS";
const char MSG_1100YEN[] = "1OO YEN / 1 CREDIT";
const char MSG_3500YEN[] = "5OO YEN / 3 CREDITS";
const char MSG_1NT10[] = "NT1O / 1 CREDIT";
const char MSG_15SCHIL[] = "5 SCHILLING / 1 CREDIT";
const char MSG_12X5SCHIL[] = "2 X 5 SCHILLING / 1 CREDIT";
const char MSG_12X10SCHIL[] = "2 X 10 SCHILLING / 1 CREDIT";
const char MSG_110SCHIL[] = "1O SCHILLING / 1 CREDIT";
const char MSG_210SCHIL[] = "1O SCHILLING / 2 CREDITS";
const char MSG_230SCHIL[] = "3O SCHILLING / 2 CREDITS";
const char MSG_320SCHIL[] = "2O SCHILLING / 3 CREDITS";
const char MSG_15KRONA[] = "5 KRONA / 1 CREDIT";
const char MSG_15MARKKA[] = "5 MARKKA / 1 CREDIT";
const char MSG_15KRONE[] = "5 KRONE / 1 CREDIT";
const char MSG_11HFI[] = "1 HFI / 1 CREDIT";
const char MSG_1250HFI[] = "2 5O HFI / 1 CREDIT";
const char MSG_3250HFI[] = "2 5O HFI / 3 CREDITS";
const char MSG_14X20FORINT[] = "4 X 2O FORINT / 1 CREDIT";
const char MSG_2140FORINT[] = "14O FORINT / 2 CREDITS";
const char MSG_25C[] = ".25";
const char MSG_20C[] = ".2O";
const char MSG_100D[] = "1.OO";
const char MSG_200D[] = "2.OO";
const char MSG_1DM[] = "1 DM";
const char MSG_2DM[] = "2 DM";
const char MSG_5DM[] = "5 DM";
const char MSG_1F[] = "1 F";
const char MSG_5F[] = "5 F";
const char MSG_10F[] = "1O F";
const char MSG_20F[] = "2O F";
const char MSG_50F[] = "5O F";
const char MSG_500L[] = "5OO LIRE";
const char MSG_10P[] = "1OP";
const char MSG_20P[] = "2OP";
const char MSG_25P[] = "25P";
const char MSG_50P[] = "5OP";
const char MSG_100P[] = "1OOP";
const char MSG_200P[] = "2OOP";
const char MSG_500P[] = "5OOP";
const char MSG_100YEN[] = "1OO YEN";
const char MSG_500YEN[] = "5OO YEN";
const char MSG_NT10[] = "NT1O";
const char MSG_1COIN[] = "1 COIN";
const char MSG_4COINS[] = "4 COINS";
const char MSG_5SCHIL[] = "5 SCHIL";
const char MSG_10SCHIL[] = "1O SCHIL";
const char MSG_20SCHIL[] = "2O SCHIL";
const char MSG_1KRONA[] = "1 KRONA";
const char MSG_5KRONA[] = " 5KRONA";
const char MSG_10KRONA[] = "1OKRONA";
const char MSG_1MARKKA[] = "1MARKKA";
const char MSG_5MARKKA[] = "  5MARKKA";
const char MSG_10MARKKA[] = "1OMARKKA";
const char MSG_1HFI[] = "1HFI";
const char MSG_250HFI[] = "2.5O HFI";
const char MSG_1KRONE[] = "1KRON";
const char MSG_5KRONE[] = "5KRON";
const char MSG_10KRONE[] = "1OKRON";
const char MSG_20KRONE[] = "2OKRON";
const char MSG_20FORINT[] = "2OFORINT";
const char MSG_100FORINT[] = "1OOFORINT";
const char MSGC_USA1[] = "USA1";
const char MSGC_USA2[] = "USA2";
const char MSGC_USA3[] = "USA3";
const char MSGC_USA4[] = "USA4";
const char MSGC_USA5[] = "USA5";
const char MSGC_USA6[] = "USA6";
const char MSGC_USA7[] = "USA7";
const char MSGC_USA8[] = "USA8";
const char MSGC_USA9[] = "USA9";
const char MSGC_USA10[] = "USA10";
const char MSGC_USA11[] = "USA11";
const char MSGC_USA12[] = "USA12";
const char MSGC_GERMAN1[] = "GERMAN1";
const char MSGC_GERMAN2[] = "GERMAN2";
const char MSGC_GERMAN3[] = "GERMAN3";
const char MSGC_GERMAN4[] = "GERMAN4";
const char MSGC_GERMAN5[] = "GERMAN5";
const char MSGC_GERMAN6[] = "GERMAN6";
const char MSGC_GERMAN7[] = "GERMAN7";
const char MSGC_GERMAN8[] = "GERMAN8";
const char MSGC_FRENCH1[] = "FRENCH1";
const char MSGC_FRENCH2[] = "FRENCH2";
const char MSGC_FRENCH3[] = "FRENCH3";
const char MSGC_FRENCH4[] = "FRENCH4";
const char MSGC_FRENCH5[] = "FRENCH5";
const char MSGC_FRENCH6[] = "FRENCH6";
const char MSGC_FRENCH7[] = "FRENCH7";
const char MSGC_FRENCH8[] = "FRENCH8";
const char MSGC_FRENCH9[] = "FRENCH9";
const char MSGC_FRENCH10[] = "FRENCH10";
const char MSGC_FRENCH11[] = "FRENCH11";
const char MSGC_CANADA1[] = "CANADA1";
const char MSGC_CANADA2[] = "CANADA2";
const char MSGC_CANADA3[] = "CANADA3";
const char MSGC_SWISS1[] = "SWISS1";
const char MSGC_SWISS2[] = "SWISS2";
const char MSGC_SWISS3[] = "SWISS3";
const char MSGC_ITALY1[] = "ITALY1";
const char MSGC_ITALY2[] = "ITALY2";
const char MSGC_ITALY3[] = "ITALY3";
const char MSGC_UK1[] = "UK1";
const char MSGC_UK2[] = "UK2";
const char MSGC_UK3[] = "UK3";
const char MSGC_SPAIN1[] = "SPAIN1";
const char MSGC_SPAIN2[] = "SPAIN2";
const char MSGC_SPAIN3[] = "SPAIN3";
const char MSGC_SPAIN4[] = "SPAIN4";
const char MSGC_SPAIN5[] = "SPAIN5";
const char MSGC_SPAIN6[] = "SPAIN6";
const char MSGC_AUSTRALIA1[] = "AUSTRALIA1";
const char MSGC_AUSTRALIA2[] = "AUSTRALIA2";
const char MSGC_AUSTRALIA3[] = "AUSTRALIA3";
const char MSGC_AUSTRALIA4[] = "AUSTRALIA4";
const char MSGC_JAPAN1[] = "JAPAN1";
const char MSGC_JAPAN2[] = "JAPAN2";
const char MSGC_JAPAN3[] = "JAPAN3";
const char MSGC_TAIWAN1[] = "TAIWAN1";
const char MSGC_TAIWAN2[] = "TAIWAN2";
const char MSGC_TAIWAN3[] = "TAIWAN3";
const char MSGC_AUSTRIA1[] = "AUSTRIA1";
const char MSGC_AUSTRIA2[] = "AUSTRIA2";
const char MSGC_AUSTRIA3[] = "AUSTRIA3";
const char MSGC_AUSTRIA4[] = "AUSTRIA4";
const char MSGC_BELGIUM1[] = "BELGIUM1";
const char MSGC_BELGIUM2[] = "BELGIUM2";
const char MSGC_BELGIUM3[] = "BELGIUM3";
const char MSGC_SWEDEN1[] = "SWEDEN1";
const char MSGC_FINLAND1[] = "FINLAND1";
const char MSGC_NETHERLAND1[] = "NETHERLAND1";
const char MSGC_NETHERLAND2[] = "NETHERLAND2";
const char MSGC_NORWAY1[] = "NORWAY1";
const char MSGC_DENMARK1[] = "DENMARK1";
const char MSGC_HUNGARY1[] = "HUNGARY1";
const char MSGC_GENERAL1[] = "GENERAL1";
const char MSGC_GENERAL2[] = "GENERAL2";
const char MSGC_GENERAL3[] = "GENERAL3";
const char MSGC_GENERAL4[] = "GENERAL4";
const char MSGC_GENERAL5[] = "GENERAL5";
const char MSGC_GENERAL6[] = "GENERAL6";
const char MSGC_GENERAL7[] = "GENERAL7";
const char MSGC_GENERAL8[] = "GENERAL8";
const char MSGC_HONGKONG1[] = "HONGKONG1";
const char MSGC_HONGKONG2[] = "HONGKONG2";
const char MSGC_HONGKONG3[] = "HONGKONG3";
/* ;1/3X25
 */
/* asm: USA1	.string	1,1,4,4		;coin1,coin2,coin3,coin4 */
/* asm: 	.string	3,0,0,1		;units per credit,units for bonus,min units,credits to start */
/* asm: 	.string	1,1,0,0		;credits to continue,show partial credits,NOT USED,NOT USED */
/* asm: 	.word	MSG_13		;message_string line 1;Must have at lease one message */
/* asm: 	.word	0		;message_string line 2;	0 = no message on line 2 or 3 */
/* asm: 	.word	0		;message_string line 3;	0 = no message on line 3 */
/* asm: 	.word	MSG_25C		;message_string coin 1 denaomination	NOTE: NULL$ = NOT USED */
/* asm: 	.word	MSG_25C		;message_string coin 2 denaomination	NOTE: NULL$ = NOT USED */
/* asm: 	.word	MSG_100D	;message_string coin 3 denaomination	NOTE: NULL$ = NOT USED */
/* asm: 	.word	MSG_100D	;message_string coin 4 denaomination	NOTE: NULL$ = NOT USED */
/* asm: 	.string	1,1,4,4		;coin1,coin2,coin3,coin4 denominations */
uintptr_t USA1[] = {
    0x04040101,           // coin1,coin2,coin3,coin4
    0x01000003,           // units per credit,units for bonus,min units,credits to start
    0x00000101,           // credits to continue,show partial credits,NOT USED,NOT USED
    (uintptr_t)&MSG_13,   // message_string line 1;Must have at lease one message
    0,                    // message_string line 2;	0 = no message on line 2 or 3
    0,                    // message_string line 3;	0 = no message on line 3
    (uintptr_t)&MSG_25C,  // message_string coin 1 denaomination	NOTE: NULL$ = NOT USED
    (uintptr_t)&MSG_25C,  // message_string coin 2 denaomination	NOTE: NULL$ = NOT USED
    (uintptr_t)&MSG_100D, // message_string coin 3 denaomination	NOTE: NULL$ = NOT USED
    (uintptr_t)&MSG_100D, // message_string coin 4 denaomination	NOTE: NULL$ = NOT USED
    0x04040101,           // coin1,coin2,coin3,coin4 denominations
};
/* ;1/2X25
 */
/* asm: USA2	.string	1,1,4,4 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA2[] = {
    0x04040101,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/4X25
 */
/* asm: USA3 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	4,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_14 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA3[] = {
    0x04040101,
    0x01000004,
    0x00000101,
    (uintptr_t)&MSG_14,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/5X25
 */
/* asm: USA4 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	5,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_15 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA4[] = {
    0x04040101,
    0x01000005,
    0x00000101,
    (uintptr_t)&MSG_15,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/6X25
 */
/* asm: USA5 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	6,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_16 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA5[] = {
    0x04040101,
    0x01000006,
    0x00000101,
    (uintptr_t)&MSG_16,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/8X25
 */
/* asm: USA6 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	8,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_18 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA6[] = {
    0x04040101,
    0x01000008,
    0x00000101,
    (uintptr_t)&MSG_18,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/8X25, 1/2X100, 3/1X500
 */
/* asm: USA7 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	8,20,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12D */
/* asm: 	.word	MSG_35D */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA7[] = {
    0x04040101,
    0x01001408,
    0x00000101,
    (uintptr_t)&MSG_12D,
    (uintptr_t)&MSG_35D,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/25, 3X25/START, 2X25 CONTINUE
 */
/* asm: USA8 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	1,0,0,3 */
/* asm: 	.string	2,0,0,0 */
/* asm: 	.word	MSG_11 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA8[] = {
    0x04040101,
    0x03000001,
    0x00000002,
    (uintptr_t)&MSG_11,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/25, 2X25/START, 1X25 CONTINUE
 */
/* asm: USA9 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	1,0,0,2 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_11 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA9[] = {
    0x04040101,
    0x02000001,
    0x00000001,
    (uintptr_t)&MSG_11,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/2X100 START, 1/150 CONTINUE
 */
/* asm: USA10 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	2,0,0,4 */
/* asm: 	.string	3,1,0,0 */
/* asm: 	.word	MSG_12 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA10[] = {
    0x04040101,
    0x04000002,
    0x00000103,
    (uintptr_t)&MSG_12,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/6X25, 4/5X100
 */
/* asm: USA11 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	6,20,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_1150D */
/* asm: 	.word	MSG_45D */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA11[] = {
    0x04040101,
    0x01001406,
    0x00000101,
    (uintptr_t)&MSG_1150D,
    (uintptr_t)&MSG_45D,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/100 START, 1/3X25 CONTINUE
 */
/* asm: USA12 */
/* asm: 	.string	1,1,4,4 */
/* asm: 	.string	1,0,0,4 */
/* asm: 	.string	3,1,0,0 */
/* asm: 	.word	MSG_11 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_100D */
/* asm: 	.string	1,1,4,4 */
uintptr_t USA12[] = {
    0x04040101,
    0x04000001,
    0x00000103,
    (uintptr_t)&MSG_11,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_100D,
    0x04040101,
};
/* ;1/3X1DM, 2/5DM
 */
/* asm: GERMAN1 */
/* asm: 	.string	1,5,0,0 */
/* asm: 	.string	1,5,0,3 */
/* asm: 	.string	3,1,0,0 */
/* asm: 	.word	MSG_11DM */
/* asm: 	.word	MSG_65DM */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1DM */
/* asm: 	.word	MSG_5DM */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t GERMAN1[] = {
    0x00000501,
    0x03000501,
    0x00000103,
    (uintptr_t)&MSG_11DM,
    (uintptr_t)&MSG_65DM,
    0,
    (uintptr_t)&MSG_1DM,
    (uintptr_t)&MSG_5DM,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/1DM, 5/5DM
 */
/* asm: GERMAN2 */
/* asm: 	.string	1,5,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_11DM */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1DM */
/* asm: 	.word	MSG_5DM */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t GERMAN2[] = {
    0x00000501,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_11DM,
    0,
    0,
    (uintptr_t)&MSG_1DM,
    (uintptr_t)&MSG_5DM,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/2X1DM, 3/5DM
 */
/* asm: GERMAN3 */
/* asm: 	.string	1,5,0,0 */
/* asm: 	.string	1,5,0,2 */
/* asm: 	.string	2,1,0,0 */
/* asm: 	.word	MSG_11DM */
/* asm: 	.word	MSG_65DM */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1DM */
/* asm: 	.word	MSG_5DM */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t GERMAN3[] = {
    0x00000501,
    0x02000501,
    0x00000102,
    (uintptr_t)&MSG_11DM,
    (uintptr_t)&MSG_65DM,
    0,
    (uintptr_t)&MSG_1DM,
    (uintptr_t)&MSG_5DM,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;2/5DM, 1/3X1DM ECA
 */
/* asm: GERMAN4 */
/* asm: 	.string	5,2,1,0 */
/* asm: 	.string	3,5,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_13DM */
/* asm: 	.word	MSG_25DM */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_5DM */
/* asm: 	.word	MSG_2DM */
/* asm: 	.word	MSG_1DM */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	5,2,1,0 */
uintptr_t GERMAN4[] = {
    0x00010205,
    0x01000503,
    0x00000101,
    (uintptr_t)&MSG_13DM,
    (uintptr_t)&MSG_25DM,
    0,
    (uintptr_t)&MSG_5DM,
    (uintptr_t)&MSG_2DM,
    (uintptr_t)&MSG_1DM,
    (uintptr_t)&MSG_NULL,
    0x00010205,
};
/* ;3/5DM, 1/2DM, 1/2X1DM ECA
 */
/* asm: GERMAN5 */
/* asm: 	.string	5,2,1,0 */
/* asm: 	.string	2,5,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12DM */
/* asm: 	.word	MSG_35DM */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_5DM */
/* asm: 	.word	MSG_2DM */
/* asm: 	.word	MSG_1DM */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	5,2,1,0 */
uintptr_t GERMAN5[] = {
    0x00010205,
    0x01000502,
    0x00000101,
    (uintptr_t)&MSG_12DM,
    (uintptr_t)&MSG_35DM,
    0,
    (uintptr_t)&MSG_5DM,
    (uintptr_t)&MSG_2DM,
    (uintptr_t)&MSG_1DM,
    (uintptr_t)&MSG_NULL,
    0x00010205,
};
/* ;1/3X1DM, 2/5DM NO BONUS
 */
/* asm: GERMAN6 */
/* asm: 	.string	1,6,0,0 */
/* asm: 	.string	3,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_13DM */
/* asm: 	.word	MSG_25DM */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1DM */
/* asm: 	.word	MSG_5DM */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t GERMAN6[] = {
    0x00000601,
    0x01000003,
    0x00000101,
    (uintptr_t)&MSG_13DM,
    (uintptr_t)&MSG_25DM,
    0,
    (uintptr_t)&MSG_1DM,
    (uintptr_t)&MSG_5DM,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/2X1DM, 3/5DM NO BONUS
 */
/* asm: GERMAN7 */
/* asm: 	.string	1,6,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12DM */
/* asm: 	.word	MSG_35DM */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1DM */
/* asm: 	.word	MSG_5DM */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t GERMAN7[] = {
    0x00000601,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12DM,
    (uintptr_t)&MSG_35DM,
    0,
    (uintptr_t)&MSG_1DM,
    (uintptr_t)&MSG_5DM,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/1DM, 6/5DM NO BONUS 2 start 1 continue
 */
/* asm: GERMAN8 */
/* asm: 	.string	1,6,0,0 */
/* asm: 	.string	1,0,0,2 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_11DM */
/* asm: 	.word	MSG_65DM */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1DM */
/* asm: 	.word	MSG_5DM */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t GERMAN8[] = {
    0x00000601,
    0x02000001,
    0x00000101,
    (uintptr_t)&MSG_11DM,
    (uintptr_t)&MSG_65DM,
    0,
    (uintptr_t)&MSG_1DM,
    (uintptr_t)&MSG_5DM,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/10F, 2/20F, 4/30
 */
/* asm: FRENCH1 */
/* asm: 	.string	1,2,0,0 */
/* asm: 	.string	1,3,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_110F */
/* asm: 	.word	MSG_220F */
/* asm: 	.word	MSG_430F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_20F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,2,0,0 */
uintptr_t FRENCH1[] = {
    0x00000201,
    0x01000301,
    0x00000001,
    (uintptr_t)&MSG_110F,
    (uintptr_t)&MSG_220F,
    (uintptr_t)&MSG_430F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_20F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/10F, 2/20F, 5/40F
 */
/* asm: FRENCH2 */
/* asm: 	.string	1,2,0,0 */
/* asm: 	.string	1,4,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_110F */
/* asm: 	.word	MSG_220F */
/* asm: 	.word	MSG_540F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_20F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,2,0,0 */
uintptr_t FRENCH2[] = {
    0x00000201,
    0x01000401,
    0x00000001,
    (uintptr_t)&MSG_110F,
    (uintptr_t)&MSG_220F,
    (uintptr_t)&MSG_540F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_20F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/10F, 3/20F, 7/40F
 */
/* asm: FRENCH3 */
/* asm: 	.string	3,6,0,0 */
/* asm: 	.string	2,12,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_110F */
/* asm: 	.word	MSG_320F */
/* asm: 	.word	MSG_740F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_20F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,2,0,0 */
uintptr_t FRENCH3[] = {
    0x00000603,
    0x01000C02,
    0x00000001,
    (uintptr_t)&MSG_110F,
    (uintptr_t)&MSG_320F,
    (uintptr_t)&MSG_740F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_20F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/5F, 2/10F, 5/20F
 */
/* asm: FRENCH4 */
/* asm: 	.string	1,2,0,0 */
/* asm: 	.string	1,4,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_15F */
/* asm: 	.word	MSG_210F */
/* asm: 	.word	MSG_520F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,2,0,0 */
uintptr_t FRENCH4[] = {
    0x00000201,
    0x01000401,
    0x00000001,
    (uintptr_t)&MSG_15F,
    (uintptr_t)&MSG_210F,
    (uintptr_t)&MSG_520F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/5F, 3/10F, 7/20F
 */
/* asm: FRENCH5 */
/* asm: 	.string	3,6,0,0 */
/* asm: 	.string	2,12,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_15F */
/* asm: 	.word	MSG_310F */
/* asm: 	.word	MSG_720F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,2,0,0 */
uintptr_t FRENCH5[] = {
    0x00000603,
    0x01000C02,
    0x00000001,
    (uintptr_t)&MSG_15F,
    (uintptr_t)&MSG_310F,
    (uintptr_t)&MSG_720F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1F, 5F, 10F, 20F ECA
;1/10F 2/20F, 4/30F
 */
/* asm: FRENCH6 */
/* asm: 	.string	1,5,10,20 */
/* asm: 	.string	10,30,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_110F */
/* asm: 	.word	MSG_220F */
/* asm: 	.word	MSG_430F */
/* asm: 	.word	MSG_1F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_20F */
/* asm: 	.string	1,5,10,20 */
uintptr_t FRENCH6[] = {
    0x140A0501,
    0x01001E0A,
    0x00000101,
    (uintptr_t)&MSG_110F,
    (uintptr_t)&MSG_220F,
    (uintptr_t)&MSG_430F,
    (uintptr_t)&MSG_1F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_20F,
    0x140A0501,
};
/* ;1F, 5F, 10F, 20F ECA
;1/10F 2/20F, 5/40F
 */
/* asm: FRENCH7 */
/* asm: 	.string	1,5,10,20 */
/* asm: 	.string	10,40,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_110F */
/* asm: 	.word	MSG_220F */
/* asm: 	.word	MSG_540F */
/* asm: 	.word	MSG_1F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_20F */
/* asm: 	.string	1,5,10,20 */
uintptr_t FRENCH7[] = {
    0x140A0501,
    0x0100280A,
    0x00000101,
    (uintptr_t)&MSG_110F,
    (uintptr_t)&MSG_220F,
    (uintptr_t)&MSG_540F,
    (uintptr_t)&MSG_1F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_20F,
    0x140A0501,
};
/* ;1F, 5F, 10F, 20F ECA
;1/10F 3/20F, 7/40F
 */
/* asm: FRENCH8 */
/* asm: 	.string	1,5,10,30 */
/* asm: 	.string	10,40,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_110F */
/* asm: 	.word	MSG_320F */
/* asm: 	.word	MSG_740F */
/* asm: 	.word	MSG_1F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_20F */
/* asm: 	.string	1,5,10,20 */
uintptr_t FRENCH8[] = {
    0x1E0A0501,
    0x0100280A,
    0x00000101,
    (uintptr_t)&MSG_110F,
    (uintptr_t)&MSG_320F,
    (uintptr_t)&MSG_740F,
    (uintptr_t)&MSG_1F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_20F,
    0x140A0501,
};
/* ;1F, 5F, 10F, 20F ECA
;1/5F 2/10F, 5/20F
 */
/* asm: FRENCH9 */
/* asm: 	.string	1,5,10,20 */
/* asm: 	.string	5,20,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_15F */
/* asm: 	.word	MSG_210F */
/* asm: 	.word	MSG_520F */
/* asm: 	.word	MSG_1F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_20F */
/* asm: 	.string	1,5,10,20 */
uintptr_t FRENCH9[] = {
    0x140A0501,
    0x01001405,
    0x00000101,
    (uintptr_t)&MSG_15F,
    (uintptr_t)&MSG_210F,
    (uintptr_t)&MSG_520F,
    (uintptr_t)&MSG_1F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_20F,
    0x140A0501,
};
/* ;1F, 5F, 10F, 20F ECA
;1/5F 3/10F, 7/20F
 */
/* asm: FRENCH10 */
/* asm: 	.string	1,5,15,30 */
/* asm: 	.string	5,30,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_15F */
/* asm: 	.word	MSG_310F */
/* asm: 	.word	MSG_720F */
/* asm: 	.word	MSG_1F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_20F */
/* asm: 	.string	1,5,10,20 */
uintptr_t FRENCH10[] = {
    0x1E0F0501,
    0x01001E05,
    0x00000101,
    (uintptr_t)&MSG_15F,
    (uintptr_t)&MSG_310F,
    (uintptr_t)&MSG_720F,
    (uintptr_t)&MSG_1F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_20F,
    0x140A0501,
};
/* ;1/3X5F, 4/50F
 */
/* asm: FRENCH11 */
/* asm: 	.string	1,2,0,0 */
/* asm: 	.string	3,10,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_13X5F */
/* asm: 	.word	MSG_450F */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_10F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,2,0,0 */
uintptr_t FRENCH11[] = {
    0x00000201,
    0x01000A03,
    0x00000101,
    (uintptr_t)&MSG_13X5F,
    (uintptr_t)&MSG_450F,
    0,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_10F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/2X25, 2/100,
 */
/* asm: CANADA1 */
/* asm: 	.string	1,4,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12 */
/* asm: 	.word	MSG_21D */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,4,0,0 */
uintptr_t CANADA1[] = {
    0x00000401,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12,
    (uintptr_t)&MSG_21D,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000401,
};
/* ;1/4X25, 1/100,
 */
/* asm: CANADA2 */
/* asm: 	.string	1,4,0,0 */
/* asm: 	.string	4,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_11D */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,4,0,0 */
uintptr_t CANADA2[] = {
    0x00000401,
    0x01000004,
    0x00000101,
    (uintptr_t)&MSG_11D,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000401,
};
/* ;1/4X25, 1/100, 3/2X100
 */
/* asm: CANADA3 */
/* asm: 	.string	1,4,0,0 */
/* asm: 	.string	4,8,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_11D */
/* asm: 	.word	MSG_32D */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,4,0,0 */
uintptr_t CANADA3[] = {
    0x00000401,
    0x01000804,
    0x00000101,
    (uintptr_t)&MSG_11D,
    (uintptr_t)&MSG_32D,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000401,
};
/* ;1/3X1F, 2/5F
 */
/* asm: SWISS1 */
/* asm: 	.string	1,6,0,0 */
/* asm: 	.string	3,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_13F */
/* asm: 	.word	MSG_25F */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t SWISS1[] = {
    0x00000601,
    0x01000003,
    0x00000101,
    (uintptr_t)&MSG_13F,
    (uintptr_t)&MSG_25F,
    0,
    (uintptr_t)&MSG_1F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/1F, 5/5F
 */
/* asm: SWISS2 */
/* asm: 	.string	1,5,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_11F */
/* asm: 	.word	MSG_55F */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t SWISS2[] = {
    0x00000501,
    0x01000001,
    0x00000101,
    (uintptr_t)&MSG_11F,
    (uintptr_t)&MSG_55F,
    0,
    (uintptr_t)&MSG_1F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/2X1F, 3/5F
 */
/* asm: SWISS3 */
/* asm: 	.string	1,6,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12F */
/* asm: 	.word	MSG_35F */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1F */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t SWISS3[] = {
    0x00000601,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12F,
    (uintptr_t)&MSG_35F,
    0,
    (uintptr_t)&MSG_1F,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/500LIRE
 */
/* asm: ITALY1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_1500L */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_500L */
/* asm: 	.word	MSG_500L */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,1,0,0 */
uintptr_t ITALY1[] = {
    0x00000101,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_1500L,
    0,
    0,
    (uintptr_t)&MSG_500L,
    (uintptr_t)&MSG_500L,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/2X500LIRE
 */
/* asm: ITALY2 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12X500L */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_500L */
/* asm: 	.word	MSG_500L */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,1,0,0 */
uintptr_t ITALY2[] = {
    0x00000101,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12X500L,
    0,
    0,
    (uintptr_t)&MSG_500L,
    (uintptr_t)&MSG_500L,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/4X500LIRE
 */
/* asm: ITALY3 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.string	4,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_14X500L */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_500L */
/* asm: 	.word	MSG_500L */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,1,0,0 */
uintptr_t ITALY3[] = {
    0x00000101,
    0x01000004,
    0x00000101,
    (uintptr_t)&MSG_14X500L,
    0,
    0,
    (uintptr_t)&MSG_500L,
    (uintptr_t)&MSG_500L,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;2/100P, 1/50P, ECA
 */
/* asm: UK1 */
/* asm: 	.string	10,5,2,1 */
/* asm: 	.string	5,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_150P */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100P */
/* asm: 	.word	MSG_50P */
/* asm: 	.word	MSG_20P */
/* asm: 	.word	MSG_10P */
/* asm: 	.string	10,5,2,1 */
uintptr_t UK1[] = {
    0x0102050A,
    0x01000005,
    0x00000101,
    (uintptr_t)&MSG_150P,
    0,
    0,
    (uintptr_t)&MSG_100P,
    (uintptr_t)&MSG_50P,
    (uintptr_t)&MSG_20P,
    (uintptr_t)&MSG_10P,
    0x0102050A,
};
/* ;1/100P, 1/2X50P, ECA
 */
/* asm: UK2 */
/* asm: 	.string	10,5,2,1 */
/* asm: 	.string	10,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_1100P */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100P */
/* asm: 	.word	MSG_50P */
/* asm: 	.word	MSG_20P */
/* asm: 	.word	MSG_10P */
/* asm: 	.string	10,5,2,1 */
uintptr_t UK2[] = {
    0x0102050A,
    0x0100000A,
    0x00000101,
    (uintptr_t)&MSG_1100P,
    0,
    0,
    (uintptr_t)&MSG_100P,
    (uintptr_t)&MSG_50P,
    (uintptr_t)&MSG_20P,
    (uintptr_t)&MSG_10P,
    0x0102050A,
};
/* ;1/3X20P, 1/50P
 */
/* asm: UK3 */
/* asm: 	.string	1,3,0,0 */
/* asm: 	.string	3,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_13X20P */
/* asm: 	.word	MSG_150P */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_20P */
/* asm: 	.word	MSG_50P */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	2,5,0,0 */
uintptr_t UK3[] = {
    0x00000301,
    0x01000003,
    0x00000101,
    (uintptr_t)&MSG_13X20P,
    (uintptr_t)&MSG_150P,
    0,
    (uintptr_t)&MSG_20P,
    (uintptr_t)&MSG_50P,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000502,
};
/* ;1/100PESETA, 6/500PESETA
 */
/* asm: SPAIN1 */
/* asm: 	.string	1,5,0,0 */
/* asm: 	.string	1,5,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_1100PESETA */
/* asm: 	.word	MSG_6500PESETA */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100P */
/* asm: 	.word	MSG_500P */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t SPAIN1[] = {
    0x00000501,
    0x01000501,
    0x00000001,
    (uintptr_t)&MSG_1100PESETA,
    (uintptr_t)&MSG_6500PESETA,
    0,
    (uintptr_t)&MSG_100P,
    (uintptr_t)&MSG_500P,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;3/2X100PESETA, 7/500PESETA
 */
/* asm: SPAIN2 */
/* asm: 	.string	3,14,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_32X100PESETA */
/* asm: 	.word	MSG_7500PESETA */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100P */
/* asm: 	.word	MSG_500P */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t SPAIN2[] = {
    0x00000E03,
    0x01000002,
    0x00000001,
    (uintptr_t)&MSG_32X100PESETA,
    (uintptr_t)&MSG_7500PESETA,
    0,
    (uintptr_t)&MSG_100P,
    (uintptr_t)&MSG_500P,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;100PESETA, 5/500PESETA
 */
/* asm: SPAIN3 */
/* asm: 	.string	1,5,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_1100PESETA */
/* asm: 	.word	MSG_5500PESETA */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100P */
/* asm: 	.word	MSG_500P */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t SPAIN3[] = {
    0x00000501,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_1100PESETA,
    (uintptr_t)&MSG_5500PESETA,
    0,
    (uintptr_t)&MSG_100P,
    (uintptr_t)&MSG_500P,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/2X100PESETA, 3/500PESETA
 */
/* asm: SPAIN4 */
/* asm: 	.string	1,6,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12X100PESETA */
/* asm: 	.word	MSG_3500PESETA */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100P */
/* asm: 	.word	MSG_500P */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t SPAIN4[] = {
    0x00000601,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12X100PESETA,
    (uintptr_t)&MSG_3500PESETA,
    0,
    (uintptr_t)&MSG_100P,
    (uintptr_t)&MSG_500P,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;2/100PESETA, 10/500PESETA
 */
/* asm: SPAIN5 */
/* asm: 	.string	2,10,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_2100PESETA */
/* asm: 	.word	MSG_10500PESETA */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100P */
/* asm: 	.word	MSG_500P */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,5,0,0 */
uintptr_t SPAIN5[] = {
    0x00000A02,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_2100PESETA,
    (uintptr_t)&MSG_10500PESETA,
    0,
    (uintptr_t)&MSG_100P,
    (uintptr_t)&MSG_500P,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/100PESETA ECA
 */
/* asm: SPAIN6 */
/* asm: 	.string	1,4,8,20 */
/* asm: 	.string	4,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_1100PESETA */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25P */
/* asm: 	.word	MSG_100P */
/* asm: 	.word	MSG_200P */
/* asm: 	.word	MSG_500P */
/* asm: 	.string	1,4,8,20 */
uintptr_t SPAIN6[] = {
    0x14080401,
    0x01000004,
    0x00000101,
    (uintptr_t)&MSG_1100PESETA,
    0,
    0,
    (uintptr_t)&MSG_25P,
    (uintptr_t)&MSG_100P,
    (uintptr_t)&MSG_200P,
    (uintptr_t)&MSG_500P,
    0x14080401,
};
/* ;1/5X20, 1/100
 */
/* asm: AUSTRALIA1 */
/* asm: 	.string 1,5,0,0 */
/* asm: 	.string	5,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_15X20C */
/* asm: 	.word	MSG_11D */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_20C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,5,0,0 */
uintptr_t AUSTRALIA1[] = {
    0x00000501,
    0x01000005,
    0x00000101,
    (uintptr_t)&MSG_15X20C,
    (uintptr_t)&MSG_11D,
    0,
    (uintptr_t)&MSG_20C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/10X20, 1/2X100
 */
/* asm: AUSTRALIA2 */
/* asm: 	.string 1,5,0,0 */
/* asm: 	.string	10,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_110X20C */
/* asm: 	.word	MSG_12D */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_20C */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,5,0,0 */
uintptr_t AUSTRALIA2[] = {
    0x00000501,
    0x0100000A,
    0x00000101,
    (uintptr_t)&MSG_110X20C,
    (uintptr_t)&MSG_12D,
    0,
    (uintptr_t)&MSG_20C,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/2X100
 */
/* asm: AUSTRALIA3 */
/* asm: 	.string 1,2,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12D */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_200D */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,2,0,0 */
uintptr_t AUSTRALIA3[] = {
    0x00000201,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12D,
    0,
    0,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_200D,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/3X100
 */
/* asm: AUSTRALIA4 */
/* asm: 	.string 1,2,0,0 */
/* asm: 	.string	3,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_13D */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100D */
/* asm: 	.word	MSG_200D */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,2,0,0 */
uintptr_t AUSTRALIA4[] = {
    0x00000201,
    0x01000003,
    0x00000101,
    (uintptr_t)&MSG_13D,
    0,
    0,
    (uintptr_t)&MSG_100D,
    (uintptr_t)&MSG_200D,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/100
 */
/* asm: JAPAN1 */
/* asm: 	.string 1,1,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_11 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100YEN */
/* asm: 	.word	MSG_100YEN */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,1,0,0 */
uintptr_t JAPAN1[] = {
    0x00000101,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_11,
    0,
    0,
    (uintptr_t)&MSG_100YEN,
    (uintptr_t)&MSG_100YEN,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/2X100
 */
/* asm: JAPAN2 */
/* asm: 	.string 1,1,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100YEN */
/* asm: 	.word	MSG_100YEN */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,1,0,0 */
uintptr_t JAPAN2[] = {
    0x00000101,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12,
    0,
    0,
    (uintptr_t)&MSG_100YEN,
    (uintptr_t)&MSG_100YEN,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/100, 3/500
 */
/* asm: JAPAN3 */
/* asm: 	.string 1,3,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_1100YEN */
/* asm: 	.word	MSG_3500YEN */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_100YEN */
/* asm: 	.word	MSG_500YEN */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,5,0,0 */
uintptr_t JAPAN3[] = {
    0x00000301,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_1100YEN,
    (uintptr_t)&MSG_3500YEN,
    0,
    (uintptr_t)&MSG_100YEN,
    (uintptr_t)&MSG_500YEN,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;1/NT10
 */
/* asm: TAIWAN1 */
/* asm: 	.string 1,1,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_1NT10 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_NT10 */
/* asm: 	.word	MSG_NT10 */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,1,0,0 */
uintptr_t TAIWAN1[] = {
    0x00000101,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_1NT10,
    0,
    0,
    (uintptr_t)&MSG_NT10,
    (uintptr_t)&MSG_NT10,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/2XUS25
 */
/* asm: TAIWAN2 */
/* asm: 	.string 1,1,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_25C */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,1,0,0 */
uintptr_t TAIWAN2[] = {
    0x00000101,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12,
    0,
    0,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_25C,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;2/1 1 COIN = 2 PLAYS
 */
/* asm: TAIWAN3 */
/* asm: 	.string 2,2,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_21 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,1,0,0 */
uintptr_t TAIWAN3[] = {
    0x00000202,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_21,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/5SCHIL, 2/10SCHIL
 */
/* asm: AUSTRIA1 */
/* asm: 	.string 1,2,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_15SCHIL */
/* asm: 	.word	MSG_210SCHIL */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_5SCHIL */
/* asm: 	.word	MSG_10SCHIL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,2,0,0 */
uintptr_t AUSTRIA1[] = {
    0x00000201,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_15SCHIL,
    (uintptr_t)&MSG_210SCHIL,
    0,
    (uintptr_t)&MSG_5SCHIL,
    (uintptr_t)&MSG_10SCHIL,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/10SCHIL, 3/20SCHIL
 */
/* asm: AUSTRIA2 */
/* asm: 	.string 1,2,0,0 */
/* asm: 	.string	1,2,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_110SCHIL */
/* asm: 	.word	MSG_320SCHIL */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_10SCHIL */
/* asm: 	.word	MSG_20SCHIL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,2,0,0 */
uintptr_t AUSTRIA2[] = {
    0x00000201,
    0x01000201,
    0x00000001,
    (uintptr_t)&MSG_110SCHIL,
    (uintptr_t)&MSG_320SCHIL,
    0,
    (uintptr_t)&MSG_10SCHIL,
    (uintptr_t)&MSG_20SCHIL,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/2X5SCHIL, 1/10SCHIL
 */
/* asm: AUSTRIA3 */
/* asm: 	.string 1,2,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12X5SCHIL */
/* asm: 	.word	MSG_110SCHIL */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_5SCHIL */
/* asm: 	.word	MSG_10SCHIL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,2,0,0 */
uintptr_t AUSTRIA3[] = {
    0x00000201,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12X5SCHIL,
    (uintptr_t)&MSG_110SCHIL,
    0,
    (uintptr_t)&MSG_5SCHIL,
    (uintptr_t)&MSG_10SCHIL,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/2X10SCHIL, 2/30SCHIL
 */
/* asm: AUSTRIA4 */
/* asm: 	.string 1,2,0,0 */
/* asm: 	.string	2,3,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12X10SCHIL */
/* asm: 	.word	MSG_230SCHIL */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_10SCHIL */
/* asm: 	.word	MSG_20SCHIL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,2,0,0 */
uintptr_t AUSTRIA4[] = {
    0x00000201,
    0x01000302,
    0x00000101,
    (uintptr_t)&MSG_12X10SCHIL,
    (uintptr_t)&MSG_230SCHIL,
    0,
    (uintptr_t)&MSG_10SCHIL,
    (uintptr_t)&MSG_20SCHIL,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000201,
};
/* ;1/20F
 */
/* asm: BELGIUM1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_120F */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_20F */
/* asm: 	.word	MSG_20F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,1,0,0 */
uintptr_t BELGIUM1[] = {
    0x00000101,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_120F,
    0,
    0,
    (uintptr_t)&MSG_20F,
    (uintptr_t)&MSG_20F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/2X20F
 */
/* asm: BELGIUM2 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12X20F */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_20F */
/* asm: 	.word	MSG_20F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,1,0,0 */
uintptr_t BELGIUM2[] = {
    0x00000101,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12X20F,
    0,
    0,
    (uintptr_t)&MSG_20F,
    (uintptr_t)&MSG_20F,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/20FB 3/50FB ECA
 */
/* asm: BELGIUM3 */
/* asm: 	.string	1,4,10,0 */
/* asm: 	.string	4,10,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_120F */
/* asm: 	.word	MSG_350F */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_5F */
/* asm: 	.word	MSG_20F */
/* asm: 	.word	MSG_50F */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string	1,4,10,0 */
uintptr_t BELGIUM3[] = {
    0x000A0401,
    0x01000A04,
    0x00000101,
    (uintptr_t)&MSG_120F,
    (uintptr_t)&MSG_350F,
    0,
    (uintptr_t)&MSG_5F,
    (uintptr_t)&MSG_20F,
    (uintptr_t)&MSG_50F,
    (uintptr_t)&MSG_NULL,
    0x000A0401,
};
/* ;1/5KRONA ECA
 */
/* asm: SWEDEN1 */
/* asm: 	.string 1,5,10,0 */
/* asm: 	.string	5,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_15KRONA */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1KRONA */
/* asm: 	.word	MSG_5KRONA */
/* asm: 	.word	MSG_10KRONA */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,5,10,0 */
uintptr_t SWEDEN1[] = {
    0x000A0501,
    0x01000005,
    0x00000101,
    (uintptr_t)&MSG_15KRONA,
    0,
    0,
    (uintptr_t)&MSG_1KRONA,
    (uintptr_t)&MSG_5KRONA,
    (uintptr_t)&MSG_10KRONA,
    (uintptr_t)&MSG_NULL,
    0x000A0501,
};
/* ;1/5MARKA
 */
/* asm: FINLAND1 */
/* asm: 	.string 1,5,10,0 */
/* asm: 	.string	5,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_15MARKKA */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1MARKKA */
/* asm: 	.word	MSG_5MARKKA */
/* asm: 	.word	MSG_10MARKKA */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,5,10,0 */
uintptr_t FINLAND1[] = {
    0x000A0501,
    0x01000005,
    0x00000101,
    (uintptr_t)&MSG_15MARKKA,
    0,
    0,
    (uintptr_t)&MSG_1MARKKA,
    (uintptr_t)&MSG_5MARKKA,
    (uintptr_t)&MSG_10MARKKA,
    (uintptr_t)&MSG_NULL,
    0x000A0501,
};
/* ;1/2.50HFI
 */
/* asm: NETHERLAND1 */
/* asm: 	.string 1,1,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_1250HFI */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_250HFI */
/* asm: 	.word	MSG_250HFI */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,1,0,0 */
uintptr_t NETHERLAND1[] = {
    0x00000101,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_1250HFI,
    0,
    0,
    (uintptr_t)&MSG_250HFI,
    (uintptr_t)&MSG_250HFI,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000101,
};
/* ;1/1HFI, 3/1X2.50HFI
 */
/* asm: NETHERLAND2 */
/* asm: 	.string 1,3,0,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_11HFI */
/* asm: 	.word	MSG_3250HFI */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1HFI */
/* asm: 	.word	MSG_250HFI */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,3,0,0 */
uintptr_t NETHERLAND2[] = {
    0x00000301,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_11HFI,
    (uintptr_t)&MSG_3250HFI,
    0,
    (uintptr_t)&MSG_1HFI,
    (uintptr_t)&MSG_250HFI,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000301,
};
/* ;1/5KRONE ECA
 */
/* asm: NORWAY1 */
/* asm: 	.string 1,2,4,0 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,0,0,0 */
/* asm: 	.word	MSG_15KRONE */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_5KRONE */
/* asm: 	.word	MSG_10KRONE */
/* asm: 	.word	MSG_20KRONE */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,2,4,0 */
uintptr_t NORWAY1[] = {
    0x00040201,
    0x01000001,
    0x00000001,
    (uintptr_t)&MSG_15KRONE,
    0,
    0,
    (uintptr_t)&MSG_5KRONE,
    (uintptr_t)&MSG_10KRONE,
    (uintptr_t)&MSG_20KRONE,
    (uintptr_t)&MSG_NULL,
    0x00040201,
};
/* ;1/5DKR ECA
 */
/* asm: DENMARK1 */
/* asm: 	.string 1,5,10,20 */
/* asm: 	.string	5,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_15KRONE */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1KRONE */
/* asm: 	.word	MSG_5KRONE */
/* asm: 	.word	MSG_10KRONE */
/* asm: 	.word	MSG_20KRONE */
/* asm: 	.string 1,5,10,20 */
uintptr_t DENMARK1[] = {
    0x140A0501,
    0x01000005,
    0x00000101,
    (uintptr_t)&MSG_15KRONE,
    0,
    0,
    (uintptr_t)&MSG_1KRONE,
    (uintptr_t)&MSG_5KRONE,
    (uintptr_t)&MSG_10KRONE,
    (uintptr_t)&MSG_20KRONE,
    0x140A0501,
};
/* ;1/4X20 FORINT, 2/140 FORINT
 */
/* asm: HUNGARY1 */
/* asm: 	.string 1,5,0,0 */
/* asm: 	.string	4,7,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_14X20FORINT */
/* asm: 	.word	MSG_2140FORINT */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_20FORINT */
/* asm: 	.word	MSG_100FORINT */
/* asm: 	.word	MSG_NULL */
/* asm: 	.word	MSG_NULL */
/* asm: 	.string 1,5,0,0 */
uintptr_t HUNGARY1[] = {
    0x00000501,
    0x01000704,
    0x00000101,
    (uintptr_t)&MSG_14X20FORINT,
    (uintptr_t)&MSG_2140FORINT,
    0,
    (uintptr_t)&MSG_20FORINT,
    (uintptr_t)&MSG_100FORINT,
    (uintptr_t)&MSG_NULL,
    (uintptr_t)&MSG_NULL,
    0x00000501,
};
/* ;GENERAL COIN MODES
;1 CREDIT/1 COIN
 */
/* asm: GENERAL1 */
/* asm: 	.string	1,1,1,4 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_11 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_4COINS */
/* asm: 	.string	1,1,1,4 */
uintptr_t GENERAL1[] = {
    0x04010101,
    0x01000001,
    0x00000101,
    (uintptr_t)&MSG_11,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_4COINS,
    0x04010101,
};
/* ;1 CREDIT/2 COINS
 */
/* asm: GENERAL2 */
/* asm: 	.string	1,1,1,4 */
/* asm: 	.string	2,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_12 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_4COINS */
/* asm: 	.string	1,1,1,4 */
uintptr_t GENERAL2[] = {
    0x04010101,
    0x01000002,
    0x00000101,
    (uintptr_t)&MSG_12,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_4COINS,
    0x04010101,
};
/* ;1 CREDIT/3 COINS
 */
/* asm: GENERAL3 */
/* asm: 	.string	1,1,1,4 */
/* asm: 	.string	3,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_13 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_4COINS */
/* asm: 	.string	1,1,1,4 */
uintptr_t GENERAL3[] = {
    0x04010101,
    0x01000003,
    0x00000101,
    (uintptr_t)&MSG_13,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_4COINS,
    0x04010101,
};
/* ;1 CREDIT/4 COINS
 */
/* asm: GENERAL4 */
/* asm: 	.string	1,1,1,4 */
/* asm: 	.string	4,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_14 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_4COINS */
/* asm: 	.string	1,1,1,4 */
uintptr_t GENERAL4[] = {
    0x04010101,
    0x01000004,
    0x00000101,
    (uintptr_t)&MSG_14,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_4COINS,
    0x04010101,
};
/* ;1 CREDIT/5 COINS
 */
/* asm: GENERAL5 */
/* asm: 	.string	1,1,1,4 */
/* asm: 	.string	5,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_15 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_4COINS */
/* asm: 	.string	1,1,1,4 */
uintptr_t GENERAL5[] = {
    0x04010101,
    0x01000005,
    0x00000101,
    (uintptr_t)&MSG_15,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_4COINS,
    0x04010101,
};
/* ;1 CREDIT/6 COINS
 */
/* asm: GENERAL6 */
/* asm: 	.string	1,1,1,4 */
/* asm: 	.string	6,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_16 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_4COINS */
/* asm: 	.string	1,1,1,4 */
uintptr_t GENERAL6[] = {
    0x04010101,
    0x01000006,
    0x00000101,
    (uintptr_t)&MSG_16,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_4COINS,
    0x04010101,
};
/* ;2 CREDITS/1 COIN
 */
/* asm: GENERAL7 */
/* asm: 	.string	2,2,2,8 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_21 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_4COINS */
/* asm: 	.string	1,1,1,4 */
uintptr_t GENERAL7[] = {
    0x08020202,
    0x01000001,
    0x00000101,
    (uintptr_t)&MSG_21,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_4COINS,
    0x04010101,
};
/* ;3 CREDITS/1 COIN
 */
/* asm: GENERAL8 */
/* asm: 	.string	3,3,3,12 */
/* asm: 	.string	1,0,0,1 */
/* asm: 	.string	1,1,0,0 */
/* asm: 	.word	MSG_31 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_1COIN */
/* asm: 	.word	MSG_4COINS */
/* asm: 	.string	1,1,1,4 */
uintptr_t GENERAL8[] = {
    0x0C030303,
    0x01000001,
    0x00000101,
    (uintptr_t)&MSG_31,
    0,
    0,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_1COIN,
    (uintptr_t)&MSG_4COINS,
    0x04010101,
};
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
// TODO: fix this properly
#define USA_OFF 0
#define GERMAN_OFF 0
#define FRENCH_OFF 0
#define CANADA_OFF 0
#define SWISS_OFF 0
#define ITALY_OFF 0
#define UK_OFF 0
#define SPAIN_OFF 0
#define AUSTRALIA_OFF 0
#define JAPAN_OFF 0
#define TAIWAN_OFF 0
#define AUSTRIA_OFF 0
#define BELGIUM_OFF 0
#define SWEDEN_OFF 0
#define FINLAND_OFF 0
#define NETHERLAND_OFF 0
#define NORWAY_OFF 0
#define DENMARK_OFF 0
#define HUNAGRY_OFF 0
#define GENERAL_OFF 0


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
int COUNTRY_TAB[] = {
    USA_OFF,       // 1
    GERMAN_OFF,    // 2
    FRENCH_OFF,    // 3
    CANADA_OFF,    // 4
    SWISS_OFF,     // 5
    ITALY_OFF,     // 6
    UK_OFF,        // 7
    SPAIN_OFF,     // 8
    AUSTRALIA_OFF, // 9
    JAPAN_OFF,     // 10
    TAIWAN_OFF,    // 11
    AUSTRIA_OFF,   // 12
    BELGIUM_OFF,   // 13
    SWEDEN_OFF,    // 14	OTHER COUNTRYS
    GENERAL_OFF,   // 15
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
uintptr_t* COUNTRY_DEFAULTS[] = {
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
// clang-format on

#include "../core/cpu.h"
#include "../core/machine.h"
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
#include "port.h"
#include "coin.h"
#include "diag.h"

/*
 * Source module: asm/COIN.ASM
 */

void COIN1(void);
void COIN2(void);
void COIN3(void);
void COIN4(void);
void SERV_COIN(void);
void GETCOIN(void);
void GET_THIS_COIN(void);
void GET_THIS_COINTXT(void);
void GETCOINTXT(void);
void GET_COIN1(void);
void GET_COIN2(void);
void GET_COIN3(void);
void GET_COIN4(void);
void GET_UNITS_PER_CREDIT(void);
void GET_UNITS_FOR_BONUS(void);
void GET_MIN_UNITS(void);
void GET_CREDITS_TO_START(void);
void GET_CREDITS_TO_CONTINUE(void);
void GET_SHOW_PARTIAL(void);
void GET_COIN1_COUNTER(void);
void GET_COIN2_COUNTER(void);
void GET_COIN3_COUNTER(void);
void GET_COIN4_COUNTER(void);
void GETCOIN_DEFAULT(void);
void SET_COINAGE_ADJ(void);
void INIT_CUSTOM_COIN(void);
void INICC(void);
void FONT18RED(void);
void FONT18REDDS(void);
void INSERT_COINS(void);
void FLASH_INSERTCOINS(void);
void SHOW_INSERTCOINS(void);
void FLASH_START(void);
void PRINT_COINAGE(void);
void GET_COINAGE_HIGHT(void);
void WHITE10FNT(void);
void PRINT_CREDITS(void);
void FLASH_TO_START(void);
void TOSTART_STRING(void);
void TOCONT_STRING(void);
void VOLUME_DISPLAY(void);
void INSERT_MORE_COINS(void);
void FLASH_INSERTCOINSM(void);
void PRINT_TOCONT(void);
void COIN_COUNTER(void);
void MOTION_VALID(void);

#define CUSTOM_COINTABI CUSTOM_COINTAB
#define CUSTOM_COINSTRI CUSTOM_COINSTR
#define bufferi buffer
#define CTS_STRI CTS_STR
#define CTC_STRI CTC_STR
#define FCBI FCB
#define PCBI PCB
#define CREDITBUFFI CREDITBUFFER
#define TOSTARTBUFFI TOSTARTBUFFER
#define NCBI NCB
#define SPCI SPC
#define DBLSPCI DBLSPC
#define CWI CW
#define CWSI CWS
#define SCI SCS
#define MSG_NULLI MSG_NULL
#define INSERTCOINSI INSERTCOINS
#define HITSTARTI HITSTART

#define COIN_TABLEENDI COIN_TABLEEND
#define COUNTRY_DEFAULTS_ENDI COUNTRY_DEFAULTS_END
#define COUNTRY_DEFAULTSI COUNTRY_DEFAULTS
void CHECK_MOTION_DIP(void);
void CHECK_MOTION_PRESENT(void);

extern int COINOFF;
extern int CUSTOM_COINTAB[];
extern int CUSTOM_COINSTR[];
extern int buffer[];
extern const char CTS_STR[];
extern const char CTC_STR[];
extern int FCB;
extern int PCB;
extern int CREDITBUFFER[];
extern int TOSTARTBUFFER[];
extern const char NCB[];
extern const char SPC[];
extern const char DBLSPC[];
extern const char CW[];
extern const char CWS[];
extern int SCS;
extern const char MSG_NULL[];
extern const char INSERTCOINS[];
extern const char HITSTART[];
extern int ICF;
extern int VOLUME_ACTIVE;
extern int VOLUME_COUNT[];
extern const char VOLUME_TXT[];
extern int CMOS_WP_WORD_SHADOW;
extern int COIN_COUNTER1;
extern int COIN_COUNTER2;
extern int COUNTER_IDX;
extern int COUNTER_MODE;
extern const char MOTIONDIS[];

/* asm: COINOFF	.bss	COINOFF,1 */
int COINOFF;

/* *----------------------------------------------------------------------------
*THESE ROUTINES MUST PRESERVE R3
 */
void COIN1(void)
{
    // asm 0000733F: 	SOND1	COININ
    // asm 00007341: 	INCAUD	AUD_COIN1
    // asm 00007343: 	CALL	SEND_COINDROP
    // asm 00007344: 	CALL	GET_COIN1_COUNTER
    // asm 00007345: 	LDP	@COIN_COUNTER1
    // asm 00007346: 	LDI	@COIN_COUNTER1,R1
    // asm 00007347: 	ADDI	R0,R1
    // ;	INC	R1
    // asm 00007348: 	STI	R1,@COIN_COUNTER1
    // asm 00007349: 	SETDP
    // asm 0000734A: 	CALL	GET_COIN1
    // asm 0000734B: 	BU	CHECK_CREDITS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN1", 0, 0);
    UNIMPL();
}

void COIN2(void)
{
    // asm 0000734C: 	SOND1	COININ
    // asm 0000734E: 	INCAUD	AUD_COIN2
    // asm 00007350: 	CALL	SEND_COINDROP
    // ;this is a possibly permenant kludge of only having
    // ;1 coin counter
    // ;
    // asm 00007351: 	LDI	@DIPRAM,R0
    // asm 00007352: 	TSTB	DIP_COINCNTR,R0
    // asm 00007353: 	BZ	JAJA3
    // asm 00007354: 	CALL	GET_COIN2_COUNTER
    // asm 00007355: 	LDP	@COIN_COUNTER1
    // asm 00007356: 	LDI	@COIN_COUNTER1,R1
    // asm 00007357: 	ADDI	R0,R1
    // ;	INC	R1
    // asm 00007358: 	STI	R1,@COIN_COUNTER1
    // asm 00007359: 	SETDP
    // asm 0000735A: 	BU	JAJA5
JAJA3:
    // asm 0000735B: 	CALL	GET_COIN2_COUNTER
    // asm 0000735C: 	LDP	@COIN_COUNTER2
    // asm 0000735D: 	LDI	@COIN_COUNTER2,R1
    // asm 0000735E: 	ADDI	R0,R1
    // asm 0000735F: 	STI	R1,@COIN_COUNTER2
    // asm 00007360: 	SETDP
JAJA5:
    // asm 00007361: 	CALL	GET_COIN2
    // asm 00007362: 	BU	CHECK_CREDITS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN2", 0, 0);
    UNIMPL();
}

void COIN3(void)
{
    // asm 00007363: 	SOND1	COININ
    // asm 00007365: 	INCAUD	AUD_COIN2
    // asm 00007367: 	CALL	GET_COIN3_COUNTER
    // asm 00007368: 	LDP	@COIN_COUNTER1
    // asm 00007369: 	LDI	@COIN_COUNTER1,R1
    // asm 0000736A: 	ADDI	R0,R1
    // asm 0000736B: 	STI	R1,@COIN_COUNTER1
    // asm 0000736C: 	SETDP
    // asm 0000736D: 	CALL	GET_COIN3
    // asm 0000736E: 	BU	CHECK_CREDITS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN3", 0, 0);
    UNIMPL();
}

void COIN4(void)
{
    // asm 0000736F: 	SOND1	COININ
    // asm 00007371: 	INCAUD	AUD_COIN4
    // asm 00007373: 	CALL	GET_COIN4_COUNTER
    // asm 00007374: 	LDP	@COIN_COUNTER1
    // asm 00007375: 	LDI	@COIN_COUNTER1,R1
    // asm 00007376: 	ADDI	R0,R1
    // asm 00007377: 	STI	R1,@COIN_COUNTER1
    // asm 00007378: 	SETDP
    // asm 00007379: 	CALL	GET_COIN4
    // asm 0000737A: 	BU	CHECK_CREDITS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN4", 0, 0);
    UNIMPL();
}

void SERV_COIN(void)
{
    // asm 0000737B: 	SOND1	COININ
    // asm 0000737D: 	INCAUD	AUD_SERVICE_CREDITS
    // asm 0000737F: 	READAUD	AUD_CREDITS
    // asm 00007381: 	LDI	R0,R1
    // asm 00007382: 	INC	R1
    // asm 00007383: 	READAUD	ADJ_MAX_CREDITS
    // asm 00007385: 	CMPI	R0,R1
    // asm 00007386: 	LDIGE	R0,R1
    // asm 00007387: 	LDI	R1,R2
    // asm 00007388: 	SETAUD	AUD_CREDITS
    // asm 0000738A: 	DIE
CHECK_CREDITS:
    // 	;R2 HAS PCREDITS TO INCREMENT
    // 	;
    // asm 0000738B: 	CMPI	0,R2			;ZERO COIN SLOT? THIS will mees up the bonus system
    // asm 0000738C: 	BEQ	CHECK_CREDITSX
    // asm 0000738D: 	LDI	R2,R1			;ADD TO PCREDITS
    // asm 0000738E: 	ADDAUD	AUD_PCREDITS		;R2 HAS NEW VALUE
    // asm 00007390: 	LDI	R1,R2			;ADD TO BCREDITS
    // asm 00007391: 	ADDAUD	AUD_BCREDITS		;BONUS COUNTER
    // asm 00007393: 	READAUD	AUD_PCREDITS		;READ BACK THE PCREDITS
    // asm 00007395: 	LDI	R0,R2
CHECK_CREDITSLP:
    // ;FIRST CHECK TO SEE IF MORE THAN MIN PCREDITS FOR A CREDIT
    // asm 00007396: 	CALL	GET_MIN_UNITS
    // asm 00007397: 	CMPI	R1,R2
    // asm 00007398: 	BLT	NOINC
    // asm 00007399: SKIP_MIN
    // asm 00007399: 	CALL	GET_UNITS_PER_CREDIT
    // asm 0000739A: 	CMPI	R1,R2
    // asm 0000739B: 	BLT	NOINC
    // asm 0000739C: 	SUBI	R1,R2
    // asm 0000739D: 	SETAUD	AUD_PCREDITS
    // asm 0000739F: 	PUSH	R2
    // asm 000073A0: 	READAUD	AUD_CREDITS
    // asm 000073A2: 	PUSH	R2
    // asm 000073A3: 	LDI	R0,R1
    // asm 000073A4: 	INC	R1
    // asm 000073A5: 	READAUD	ADJ_MAX_CREDITS
    // asm 000073A7: 	CMPI	R0,R1
    // asm 000073A8: 	LDIGE	R0,R1
    // asm 000073A9: 	LDI	R1,R2
    // asm 000073AA: 	SETAUD	AUD_CREDITS
    // asm 000073AC: 	POP	R2
    // asm 000073AD: 	INCAUD	AUD_PAID_CREDITS
    // asm 000073AF: NOINCCRD
    // asm 000073AF: 	POP	R2
    // asm 000073B0: 	BU	CHECK_CREDITSLP
NOINC:
    // asm 000073B1: 	READAUD	AUD_BCREDITS
    // asm 000073B3: 	LDI	R0,R2
    // asm 000073B4: 	CALL	GET_UNITS_FOR_BONUS
    // asm 000073B5: 	CMPI	0,R1
    // asm 000073B6: 	BEQ	NOBONUS
    // asm 000073B7: 	CMPI	R1,R2
    // asm 000073B8: 	BLT	NOBONUS
    // asm 000073B9: 	LDI	0,R2
    // asm 000073BA: 	LDI	AUD_BCREDITS,AR2
    // asm 000073BB: 	CALL	AUDIT_WRITE
    // asm 000073BC: 	READAUD	AUD_CREDITS		;BONUS CREDIT
    // asm 000073BE: 	LDI	R0,R1
    // asm 000073BF: 	ADDI	1,R1			;Actually give the bonus
    // asm 000073C0: 	READAUD	ADJ_MAX_CREDITS
    // asm 000073C2: 	CMPI	R0,R1
    // asm 000073C3: 	LDIGE	R0,R1
    // asm 000073C4: 	LDI	R1,R2
    // asm 000073C5: 	SETAUD	AUD_CREDITS
    // asm 000073C7: 	LDI	0,R2
    // asm 000073C8: 	SETAUD	AUD_PCREDITS
NOBONUS:
CHECK_CREDITSX:
    // asm 000073CA: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SERV_COIN", 0, 0);
    UNIMPL();
}

void GETCOIN(void)
{
    // asm 000073CB: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm 000073CC: 	CALL	ADJUSTMENT_READ
    // asm 000073CD: 	CMPI	1,R0
    // asm 000073CE: 	BEQ	USE_STANDARD
    // asm 000073CF: 	LDI	@CUSTOM_COINTABI,AR0
    // asm 000073D0: 	RETS
USE_STANDARD:
    // asm 000073D1: 	LDI	ADJ_COINMODE,AR2
    // asm 000073D2: 	CALL	ADJUSTMENT_READ
    // asm 000073D3: 	LDI	R0,AR0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCOIN", 0, 0);
    UNIMPL();
}

void GET_THIS_COIN(void)
{
    // asm 000073D4: 	AND	07Fh,AR0
    // asm 000073D5: 	MPYI	COIN_ENTRY_SIZE,AR0
    // asm 000073D6: 	ADDI	@COIN_TABLEI,AR0
    // asm 000073D7: 	CMPI	@COIN_TABLEENDI,AR0
    // asm 000073D8: 	BLT	GETCOINX
    // asm 000073D9: 	LDI	@COIN_TABLEI,AR0
GETCOINX:
    // asm 000073DA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_THIS_COIN", 0, 0);
    UNIMPL();
}

void GET_THIS_COINTXT(void)
{
    // asm 000073DB: 	CALL	GET_THIS_COIN
    // asm 000073DC: 	ADDI	3,AR0		;NOTE AR0 now points to the COIN TABLE ENTRY WHERE THE MESSAGE RESIDES
    // asm 000073DD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_THIS_COINTXT", 0, 0);
    UNIMPL();
}

void GETCOINTXT(void)
{
    // asm 000073DE: 	CALL	GETCOIN
    // asm 000073DF: 	ADDI	3,AR0		;NOTE AR0 now points to the COIN TABLE ENTRY WHERE THE MESSAGE RESIDES
    // asm 000073E0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCOINTXT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_COIN1(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*AR0,R2			;GET UNITS FOR COIN 1
    // asm: 	AND	0FFh,R2			; byte	coin1,coin2,coin3,coin4
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN1", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_COIN2(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*AR0,R2			;GET UNITS FOR COIN 2
    // asm: 	LSH	-8,R2
    // asm: 	AND	0FFh,R2			; byte	coin1,coin2,coin3,coin4
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN2", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_COIN3(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*AR0,R2			;GET UNITS FOR COIN 3
    // asm: 	LSH	-16,R2
    // asm: 	AND	0FFh,R2			; byte	coin1,coin2,coin3,coin4
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN3", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_COIN4(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*AR0,R2			;GET UNITS FOR COIN 4
    // asm: 	LSH	-24,R2
    // asm: 	AND	0FFh,R2			; byte	coin1,coin2,coin3,coin4
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN4", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_UNITS_PER_CREDIT(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(1),R1		;GET #MIN UNITS FOR 1 CREDIT
    // asm: 	AND	0FFh,R1			; byte	units per credit,units for bonus,min units,credits to start
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_UNITS_PER_CREDIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_UNITS_FOR_BONUS(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(1),R1		;GET #UNITS FOR 1 CREDIT
    // asm: 	LSH	-8,R1
    // asm: 	AND	0FFh,R1			; byte	units per credit,units for bonus,min units,credits to start
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_UNITS_FOR_BONUS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_MIN_UNITS(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(1),R1		;GET #MIN UNITS FOR 1 CREDIT
    // asm: 	LSH	-16,R1
    // asm: 	AND	0FFh,R1			; byte	units per credit,units for bonus,min units,credits to start
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_MIN_UNITS", 0, 0);
    UNIMPL();
}

void GET_CREDITS_TO_START(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(1),R1		;GET #UNITS FOR 1 CREDIT
    // asm: 	LSH	-24,R1
    // asm: 	AND	0FFh,R1			; byte	units per credit,units for bonus,min units,credits to start
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_CREDITS_TO_START", 0, 0);
    UNIMPL();
}

void GET_CREDITS_TO_CONTINUE(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(2),R1		;GET #UNITS FOR 1 CREDIT
    // asm: 	AND	0FFh,R1			; byte	credits to continue,show partial credits,NOT USED,NOT USED
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_CREDITS_TO_CONTINUE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_SHOW_PARTIAL(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(2),R1		;GET #UNITS FOR 1 CREDIT
    // asm: 	LSH	-8,R1
    // asm: 	AND	0FFh,R1			; byte	credits to continue,show partial credits,NOT USED,NOT USED
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_SHOW_PARTIAL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_COIN1_COUNTER(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(10),R0		;GET UNITS FOR COIN 1 DENOMINATION
    // asm: 	AND	0FFh,R0			; byte	coin1,coin2,coin3,coin4
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN1_COUNTER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_COIN2_COUNTER(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(10),R0		;GET UNITS FOR COIN 2 DENOMINATION
    // asm: 	LSH	-8,R0
    // asm: 	AND	0FFh,R0			; byte	coin1,coin2,coin3,coin4
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN2_COUNTER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_COIN3_COUNTER(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(10),R0		;GET UNITS FOR COIN 3 DENOMINATION
    // asm: 	LSH	-16,R0
    // asm: 	AND	0FFh,R0			; byte	coin1,coin2,coin3,coin4
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN3_COUNTER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_COIN4_COUNTER(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	CALL	GETCOIN
    // asm: 	LDI	*+AR0(10),R0		;GET UNITS FOR COIN 4 DENOMINATION
    // asm: 	LSH	-24,R0
    // asm: 	AND	0FFh,R0			; byte	coin1,coin2,coin3,coin4
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN4_COUNTER", 0, 0);
    UNIMPL();
}

void GETCOIN_DEFAULT(void)
{
    // asm 000073E2: 	PUSH	AR2
    // asm 000073E3: 	CLRI	AR2
    // asm 000073E4: 	LDP	@DIPSW
    // asm 000073E5: 	LDI	@DIPSW,R2
    // asm 000073E6: 	LDI	*AR2,AR2
    // asm 000073E7: 	POP	AR2
    // asm 000073E8: 	SETDP
    // asm 000073E9: 	RS	24,R2
    // asm 000073EA: 	NOT	R2
    // asm 000073EB: 	LDI	0,R0
    // asm 000073EC: 	LDI	7,RC
    // asm 000073ED: 	RPTB	INVBLP
    // asm 000073EE: 	LSH	-1,R2
INVBLP:
    // asm 000073EF: ROLC	R0
    // asm 000073F0: 	LDI	R0,R2
    // asm 000073F1: 	AND	060h,R2
    // asm 000073F2: 	LSH	-5,R2
    // asm 000073F3: 	LDI	@COUNTRY_DEFAULTSI,AR2
    // asm 000073F4: 	AND	01Fh,R0
    // asm 000073F5: 	MPYI	4,R0
    // asm 000073F6: 	ADDI	R0,AR2
    // asm 000073F7: 	ADDI	R2,AR2
    // asm 000073F8: 	CMPI	@COUNTRY_DEFAULTS_ENDI,AR2
    // asm 000073F9: 	LDIGT	@COUNTRY_DEFAULTSI,AR2
    // asm 000073FA: 	LDI	*AR2,R0
    // asm 000073FB: 	SUBI	@COIN_TABLEI,R0
    // asm 000073FC: 	LDI	COIN_ENTRY_SIZE,R1
    // asm 000073FD: 	CALL	DIV_I
    // asm 000073FE: 	LDI	R0,R2
    // asm 000073FF: 	LDI	*AR2,AR2
    // asm 00007400: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCOIN_DEFAULT", 0, 0);
    UNIMPL();
}

void SET_COINAGE_ADJ(void)
{
    // asm 00007401: 	CALL	PUSHALL		;Take no chances
    // asm 00007402: 	CALL	GET_COIN1
    // asm 00007403: 	SETADJ	ADJ_COIN1_UNITS
    // asm 00007405: 	CALL	GET_COIN2
    // asm 00007406: 	SETADJ	ADJ_COIN2_UNITS
    // asm 00007408: 	CALL	GET_COIN3
    // asm 00007409: 	SETADJ	ADJ_COIN3_UNITS
    // asm 0000740B: 	CALL	GET_COIN4
    // asm 0000740C: 	SETADJ	ADJ_COIN4_UNITS
    // asm 0000740E: 	CALL	GET_UNITS_PER_CREDIT
    // asm 0000740F: 	LDI	R1,R2
    // asm 00007410: 	SETADJ	ADJ_UNITS_PER_CREDIT
    // asm 00007412: 	CALL	GET_UNITS_FOR_BONUS
    // asm 00007413: 	LDI	R1,R2
    // asm 00007414: 	SETADJ	ADJ_BONUS_UNITS
    // asm 00007416: 	CALL	GET_MIN_UNITS
    // asm 00007417: 	LDI	R1,R2
    // asm 00007418: 	SETADJ	ADJ_UNITS_MIN
    // asm 0000741A: 	CALL	GET_CREDITS_TO_START
    // asm 0000741B: 	LDI	R1,R2
    // asm 0000741C: 	SETADJ	ADJ_CREDITS_TO_START
    // asm 0000741E: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm 0000741F: 	LDI	R1,R2
    // asm 00007420: 	SETADJ	ADJ_CREDITS_TO_CONTINUE
    // asm 00007422: 	CALL	GET_SHOW_PARTIAL
    // asm 00007423: 	LDI	R1,R2
    // asm 00007424: 	SETADJ	ADJ_SHOW_FRAC
    // asm 00007426: 	CALL	GET_COIN1_COUNTER
    // asm 00007427: 	SETADJ	ADJ_COIN1_COUNTER
    // asm 00007429: 	CALL	GET_COIN2_COUNTER
    // asm 0000742A: 	SETADJ	ADJ_COIN2_COUNTER
    // asm 0000742C: 	CALL	GET_COIN3_COUNTER
    // asm 0000742D: 	SETADJ	ADJ_COIN3_COUNTER
    // asm 0000742F: 	CALL	GET_COIN4_COUNTER
    // asm 00007430: 	SETADJ	ADJ_COIN4_COUNTER
    // asm 00007432: 	CALL	POPALL
    // asm 00007433: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_COINAGE_ADJ", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void INIT_CUSTOM_COIN(void)
{
    // asm 00007434: 	LDI	3*8,R5
    // asm 00007435: 	LDI	@CUSTOM_COINTABI,AR3
    // asm 00007436: 	LDI	ADJ_COIN4_UNITS,AR2
    // asm 00007437: 	CALL	INICC		;Set SLOT SETTINGS coin1,coin2,coin3,coin4
    // asm 00007438: 	LDI	ADJ_CREDITS_TO_START,AR2
    // asm 00007439: 	LDI	3*8,R5			;Set units per credit,units for bonus,min units,credits to start
    // asm 0000743A: 	CALL	INICC
    // asm 0000743B: 	LDI	ADJ_SHOW_FRAC,AR2
    // asm 0000743C: 	LDI	1*8,R5			;Set credits to continue,show partial credits,NOT USED,NOT USED
    // asm 0000743D: 	CALL	INICC
    // asm 0000743E: 	LDI	@MSG_NULLI,R0		;Set pointer to the string
    // asm 0000743F: 	STI	R0,*AR3++
    // asm 00007440: 	LDI	0,R0
    // asm 00007441: 	STI	R0,*AR3++
    // asm 00007442: 	STI	R0,*AR3++
    // asm 00007443: 	LDI	@MSG_NULLI,R0		;Point denomination messages to NULL string
    // asm 00007444: 	STI	R0,*AR3++
    // asm 00007445: 	STI	R0,*AR3++
    // asm 00007446: 	STI	R0,*AR3++
    // asm 00007447: 	STI	R0,*AR3++
    // asm 00007448: 	LDI	3*8,R5
    // asm 00007449: 	LDI	ADJ_COIN4_COUNTER,AR2
    // asm 0000744A: 	CALL	INICC		;Set SLOT SETTINGS coin1,coin2,coin3,coin4
    // asm 0000744B: ICCX
    // asm 0000744B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_CUSTOM_COIN", 0, 0);
    UNIMPL();
}

void INICC(void)
{
    // asm 0000744C: 	LDI	0,R6
INICC_LP:
    // asm 0000744D: 	PUSH	AR2
    // asm 0000744E: 	CALL	ADJUSTMENT_READ
    // asm 0000744F: 	POP	AR2
    // asm 00007450: 	NOP	*AR2--
    // asm 00007451: 	AND	0FFh,R0
    // asm 00007452: 	LSH	R5,R0
    // asm 00007453: 	OR	R0,R6
    // asm 00007454: 	SUBI	8,R5
    // asm 00007455: 	BGE	INICC_LP
    // asm 00007456: 	STI	R6,*AR3++
    // asm 00007457: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INICC", 0, 0);
    UNIMPL();
}

/* asm: CUSTOM_COINTAB	.bss	CUSTOM_COINTAB,COIN_ENTRY_SIZE */
int CUSTOM_COINTAB[COIN_ENTRY_SIZE];
/* asm: CUSTOM_COINSTR	.bss	CUSTOM_COINSTR,10 */
int CUSTOM_COINSTR[10];
/* asm: bufferi	.word	buffer */
#define bufferi buffer
/* asm: buffer	.bss	buffer,2 */
int buffer[2];
const char CTS_STR[] = " TO START";
const char CTC_STR[] = " TO CONTINUE";
/* asm: FCB	.bss	FCB,1 */
int FCB;
/* asm: PCB	.bss	PCB,1 */
int PCB;
/* asm: CREDITBUFFI	.word	CREDITBUFFER */
#define CREDITBUFFI CREDITBUFFER
/* asm: CREDITBUFFER	.bss	CREDITBUFFER,8 */
int CREDITBUFFER[8];
/* asm: TOSTARTBUFFI	.word	TOSTARTBUFFER */
#define TOSTARTBUFFI TOSTARTBUFFER
/* asm: TOSTARTBUFFER	.bss	TOSTARTBUFFER,8 */
int TOSTARTBUFFER[8];
const char NCB[] = "@";
const char SPC[] = " ";
const char DBLSPC[] = "  ";
const char CW[] = "CREDITS";
const char CWS[] = "CREDIT";
/* asm: SCI	.word	SCS	;CREDITS TO START (START CREDIT) */
#define SCI SCS
/* asm: SCS	.bss	SCS,1 */
int SCS;
const char MSG_NULL[] = "";

void FONT18RED(void)
{
    // asm 00007477: 	LDL	font18_white,AR2
    // asm 00007478: 	CALL	PAL_FIND_RAW
    // asm 00007479: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 0000747A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FONT18RED", 0, 0);
    UNIMPL();
}

void FONT18REDDS(void)
{
    // asm 0000747B: 	LDL	font18_white,AR2
    // asm 0000747C: 	CALL	PAL_FIND_RAW
    // asm 0000747D: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 0000747E: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 0000747F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FONT18REDDS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*	F  P/N  CREDITS
*
 */
const char INSERTCOINS[] = "INSERT COINS";
const char HITSTART[] = "PRESS START";
/* asm: ICF	.bss	ICF,1 */
int ICF;

void INSERT_COINS(void)
{
    // asm 00007489: 	CALL	VOLUME_DISPLAY
    // asm 0000748A: 	CALL	MOTION_VALID
    // asm 0000748B: 	LDI	@COINOFF,R0
    // asm 0000748C: 	RETSNZ
    // asm 0000748D: 	READADJ	ADJ_FREE_PLAY
    // asm 0000748F: 	FLOAT	360,R3
    // asm 00007490: 	CMPI	1,R0
    // asm 00007491: 	BEQ	FLASH_START
    // asm 00007492: 	LDI	@_ATTR_MODE,R0
    // asm 00007493: 	AND	1,R0
    // asm 00007494: 	BZ	SHOW_COINAGE
    // asm 00007495: 	READAUD	AUD_PCREDITS
    // asm 00007497: 	PUSH	R0
    // asm 00007498: 	READAUD	AUD_CREDITS
    // asm 0000749A: 	POP	R2
    // asm 0000749B: 	ADDI	R0,R2
    // asm 0000749C: 	BZ	FLASH_INSERTCOINS
    // 	;
    // asm 0000749D: 	LDI	@_MODE,R0
    // asm 0000749E: 	AND	MMODE,R0
    // asm 0000749F: 	CMPI	MINSERT_COINS,R0
    // asm 000074A0: 	BEQ	FLASH_INSERTCOINS
SHOW_COINAGE:
    // asm 000074A1: 	CALL	GET_COINAGE_HIGHT	;Sets R3
    // asm 000074A2: 	ADDF	44,R3
    // asm 000074A3: 	MPYF	-0.5,R3
    // asm 000074A4: 	FLOAT	345,R0			;Center of text hight
    // asm 000074A5: 	ADDF	R0,R3
    // asm 000074A6: 	READAUD	AUD_CREDITS
    // asm 000074A8: 	CALL	GET_CREDITS_TO_START
    // asm 000074A9: 	CMPI	R1,R0
    // asm 000074AA: 	BLT	NO_START
    // asm 000074AB: 	CALLGE	FLASH_START
    // asm 000074AC: 	ADDF	22,R3
    // asm 000074AD: 	BU	SHOW_CREDITS
NO_START:
    // asm 000074AE: 	CALL	FLASH_TO_START		;FLASH THE TO START AND TO CONTINUE MESSAGES
    // asm 000074AF: 	ADDF	22,R3
SHOW_CREDITS:
    // asm 000074B0: 	READAUD	AUD_PCREDITS
    // asm 000074B2: 	PUSH	R0
    // asm 000074B3: 	READAUD	AUD_CREDITS
    // asm 000074B5: 	POP	R2
    // asm 000074B6: 	ADDI	R0,R2
    // asm 000074B7: 	BNZ	GODO_CREDITS
    // asm 000074B8: 	CALL	SHOW_INSERTCOINS
    // asm 000074B9: 	BU	DO_COINAGE
GODO_CREDITS:
    // asm 000074BA: 	CALL	PRINT_CREDITS		;Print the credits message
    // asm 000074BB: 	LDI	@CREDITBUFFI,AR2
    // asm 000074BC: 	FLOAT	256,R2
    // asm 000074BD: 	LDI	1,RC
    // asm 000074BE: 	CALL	TEXT_ADDDS
    // asm 000074BF: 	CALL	FONT18REDDS
    // asm 000074C0: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000074C3: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
DO_COINAGE:
    // asm 000074C6: 	ADDF	22,R3
    // asm 000074C7: 	FLOAT	256,R2
    // ;	FLOAT	345,R3
    // asm 000074C8: 	CALL	PRINT_COINAGE
    // asm 000074C9: INSERT_COINSX
    // asm 000074C9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_COINS", 0, 0);
    UNIMPL();
}

void FLASH_INSERTCOINS(void)
{
    // asm 000074CA: 	LDI	@ICF,R0
    // asm 000074CB: 	BGT	NO_INSERTCOINS
    // asm 000074CC: 	SUBI	@NFRAMES,R0
    // asm 000074CD: 	CMPI	-30,R0
    // asm 000074CE: 	LDILT	1,R0
    // asm 000074CF: 	STI	R0,@ICF
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_INSERTCOINS", 0, 0);
    UNIMPL();
}

void SHOW_INSERTCOINS(void)
{
    // asm 000074D0: 	LDI	@INSERTCOINSI,AR2
    // asm 000074D1: 	FLOAT	256,R2
    // asm 000074D2: 	LDI	1,RC
    // asm 000074D3: 	CALL	TEXT_ADDDS
    // asm 000074D4: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000074D7: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 000074DA: 	CALL	FONT18REDDS
    // asm 000074DB: 	BU	FLASH_INSERTCOINSX
NO_INSERTCOINS:
    // asm 000074DC: 	LDI	@ICF,R0
    // asm 000074DD: 	ADDI	@NFRAMES,R0
    // asm 000074DE: 	CMPI	30,R0
    // asm 000074DF: 	LDIGT	-1,R0
    // asm 000074E0: 	STI	R0,@ICF
FLASH_INSERTCOINSX:
    // asm 000074E1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_INSERTCOINS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
* This routine FLASHES THE PRESS START MESSAGE
*
 */
void FLASH_START(void)
{
    // asm 000074E2: 	LDI	BUT_START,R0
    // asm 000074E3: 	STI	R0,@BUTTON_STATUS
    // asm 000074E4: 	LDI	@ICF,R0
    // asm 000074E5: 	BGT	START_SKIP
    // asm 000074E6: 	SUBI	@NFRAMES,R0
    // asm 000074E7: 	CMPI	-20,R0
    // asm 000074E8: 	LDILT	1,R0
    // asm 000074E9: 	STI	R0,@ICF
    // asm 000074EA: 	LDI	@HITSTARTI,AR2
    // asm 000074EB: 	FLOAT	256,R2
    // asm 000074EC: 	LDI	1,RC
    // asm 000074ED: 	CALL	TEXT_ADDDS
    // asm 000074EE: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000074F1: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 000074F4: 	CALL	FONT18REDDS
    // asm 000074F5: 	BU	FLASH_STARTX
    // asm 000074F6: START_SKIP
    // asm 000074F6: 	ADDI	@NFRAMES,R0
    // asm 000074F7: 	CMPI	20,R0
    // asm 000074F8: 	LDIGT	-1,R0
    // asm 000074F9: 	STI	R0,@ICF
    // asm 000074FA: 	BU	FLASH_STARTX
FLASH_STARTX:
    // asm 000074FB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_START", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PRINT_COINAGE
*
*Prints up to 3 lines of text that describes credits per coin
*
*INPUT
*	R2 (FLOAT)	= X screen position in pixels
*	R3 (FLOAT)	= Y screen position in pixels
 */
#define FONT10_HIGHT 17.0

void PRINT_COINAGE(void)
{
    // asm 000074FC: 	CALL	GETCOINTXT
    // asm 000074FD: 	LDI	*AR0,AR2
    // asm 000074FE: 	LDI	1,RC
    // asm 000074FF: 	CALL	TEXT_ADDDS
    // asm 00007500: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00007503: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00007506: 	CALL	SET12FONTDS
    // asm 00007507: 	CALL	WHITE10FNT
    // asm 00007508: 	CALL	GETCOINTXT
    // asm 00007509: 	LDI	*+AR0(1),AR2
    // asm 0000750A: 	CMPI	0,AR2
    // asm 0000750B: 	BEQ	PRINT_COINAGEX
    // asm 0000750C: 	ADDF	FONT10_HIGHT,R3
    // asm 0000750D: 	LDI	1,RC
    // asm 0000750E: 	CALL	TEXT_ADDDS
    // asm 0000750F: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00007512: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00007515: 	CALL	SET12FONTDS
    // asm 00007516: 	CALL	WHITE10FNT
    // asm 00007517: 	CALL	GETCOINTXT
    // asm 00007518: 	LDI	*+AR0(2),AR2
    // asm 00007519: 	CMPI	0,AR2
    // asm 0000751A: 	BEQ	PRINT_COINAGEX
    // asm 0000751B: 	ADDF	FONT10_HIGHT,R3
    // asm 0000751C: 	LDI	1,RC
    // asm 0000751D: 	CALL	TEXT_ADDDS
    // asm 0000751E: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00007521: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00007524: 	CALL	SET12FONTDS
    // asm 00007525: 	CALL	WHITE10FNT
PRINT_COINAGEX:
    // asm 00007526: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_COINAGE", 0, 0);
    UNIMPL();
}

void GET_COINAGE_HIGHT(void)
{
    // asm 00007527: 	PUSHF	R0
    // asm 00007528: 	PUSH	R0
    // asm 00007529: 	PUSH	AR0
    // asm 0000752A: 	CALL	GETCOINTXT
    // asm 0000752B: 	LDF	FONT10_HIGHT,R3
    // asm 0000752C: 	LDI	*+AR0(1),R0
    // asm 0000752D: 	BEQ	GCHX
    // asm 0000752E: 	ADDF	FONT10_HIGHT,R3
    // asm 0000752F: 	LDI	*+AR0(2),R0
    // asm 00007530: 	BEQ	GCHX
    // asm 00007531: 	ADDF	FONT10_HIGHT,R3
GCHX:
    // asm 00007532: 	POP	AR0
    // asm 00007533: 	POP	R0
    // asm 00007534: 	POPF	R0
    // asm 00007535: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COINAGE_HIGHT", 0, 0);
    UNIMPL();
}

void WHITE10FNT(void)
{
    // asm 00007536: 	LDL	osg10fnt_white,AR2
    // asm 00007537: 	CALL	PAL_FIND_RAW
    // asm 00007538: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00007539: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 0000753A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WHITE10FNT", 0, 0);
    UNIMPL();
}

void PRINT_CREDITS(void)
{
    // asm 0000753B: 	CLRI	R7
    // asm 0000753C: 	STI	R7,@CREDITBUFFER	;STRING BUFFER
    // asm 0000753D: 	CALL	GET_SHOW_PARTIAL
    // asm 0000753E: 	LDI	R1,R4
    // asm 0000753F: 	READAUD	AUD_CREDITS		;DO WE HAVE ANY CREDITS?
    // asm 00007541: 	LDI	R0,R2
    // asm 00007542: 	BNZ	DO_CREDITS		;Yes, then go print them
    // asm 00007543: 	CMPI	1,R4			;No, then check partial credits on?
    // asm 00007544: 	BEQ	CHKPCREDITS		;Yes, then go print the partial credits
DO_CREDITS:
    // asm 00007545: 	LDI	1,R7
    // asm 00007546: 	LDI	@FCBI,AR2
    // asm 00007547: 	CALL	_itoa
    // asm 00007548: 	LDI	@CREDITBUFFI,AR0
    // asm 00007549: 	LDI	@FCBI,AR1
    // asm 0000754A: 	CALL	STRCAT
    // asm 0000754B: 	LDI	@CREDITBUFFI,AR0
    // asm 0000754C: 	LDI	@DBLSPCI,AR1
    // asm 0000754D: 	CALL	STRCAT
CHKPCREDITS:
    // asm 0000754E: 	READAUD	AUD_PCREDITS		;Are there any patial credits?
    // asm 00007550: 	LDI	R0,R2
    // asm 00007551: 	BZ	NO_PCREDITS		;No, then skip this
    // asm 00007552: 	CMPI	0,R4			;Is partial credits on?
    // asm 00007553: 	BEQ	NO_PCREDITS		;No, then skip this
    // asm 00007554: 	LDI	1,R7
    // asm 00007555: 	LDI	@PCBI,AR2
    // asm 00007556: 	CALL	_itoa
    // asm 00007557: 	LDI	@CREDITBUFFI,AR0
    // asm 00007558: 	LDI	@PCBI,AR1
    // asm 00007559: 	CALL	STRCAT
    // asm 0000755A:  	LDI	@CREDITBUFFI,AR0
    // asm 0000755B: 	LDI	@NCBI,AR1
    // asm 0000755C: 	CALL	STRCAT
    // asm 0000755D: 	CALL	GET_UNITS_PER_CREDIT
    // asm 0000755E: 	LDI	R1,R2
    // asm 0000755F: 	LDI	@SCI,AR2
    // asm 00007560: 	CALL	_itoa
    // asm 00007561: 	LDI	@CREDITBUFFI,AR0
    // asm 00007562: 	LDI	@SCI,AR1
    // asm 00007563: 	CALL	STRCAT
    // asm 00007564: 	LDI	@CREDITBUFFI,AR0
    // asm 00007565: 	LDI	@SPCI,AR1
    // asm 00007566: 	CALL	STRCAT
NO_PCREDITS:
    // asm 00007567: 	LDI	@CREDITBUFFI,AR0
    // asm 00007568: 	LDI	@CWI,AR1
    // asm 00007569: 	READAUD	AUD_CREDITS		;DO WE HAVE ONLY 1 CREDIT?
    // asm 0000756B: 	CMPI	1,R0
    // asm 0000756C: 	LDILE	@CWSI,AR1		;ONLY HAVE ON CREDIT USE SINGULAR "CREDIT"
    // asm 0000756D: 	CALL	STRCAT
    // 	;
    // asm 0000756E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_CREDITS", 0, 0);
    UNIMPL();
}

void FLASH_TO_START(void)
{
    // asm 0000756F: 	LDI	@ICF,R0
    // asm 00007570: 	BGT	NO_TOSTART
    // asm 00007571: 	SUBI	@NFRAMES,R0
    // asm 00007572: 	CMPI	-45,R0
    // asm 00007573: 	LDILT	1,R0
    // asm 00007574: 	STI	R0,@ICF
    // asm 00007575: 	CALL	TOSTART_STRING
    // asm 00007576: 	BU	PRINT_TOSTART
NO_TOSTART:
    // asm 00007577: 	LDI	@ICF,R0
    // asm 00007578: 	ADDI	@NFRAMES,R0
    // asm 00007579: 	CMPI	45,R0
    // asm 0000757A: 	LDIGT	-1,R0
    // asm 0000757B: 	STI	R0,@ICF
    // asm 0000757C: 	CALL	GET_CREDITS_TO_START
    // asm 0000757D: 	LDI	R1,R0
    // asm 0000757E: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm 0000757F: 	LDI	R1,R2
    // asm 00007580: 	CMPI	R0,R2
    // asm 00007581: 	BEQ	FLASH_TOSTARTX
    // asm 00007582: 	CALL	TOCONT_STRING
PRINT_TOSTART:
    // asm 00007583: 	LDI	@TOSTARTBUFFI,AR2
    // asm 00007584: 	FLOAT	256,R2
    // asm 00007585: 	LDI	1,RC
    // asm 00007586: 	CALL	TEXT_ADDDS
    // asm 00007587: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 0000758A: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 0000758D: 	CALL	FONT18REDDS
FLASH_TOSTARTX:
    // asm 0000758E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_TO_START", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TOSTART_STRING(void)
{
    // asm 0000758F: 	CLRI	R7
    // asm 00007590: 	STI	R7,@TOSTARTBUFFER	;STRING BUFFER
    // asm 00007591: 	CALL	GET_CREDITS_TO_START
    // asm 00007592: 	LDI	R1,R2
    // asm 00007593: 	LDI	1,R7
    // asm 00007594: 	LDI	@FCBI,AR2
    // asm 00007595: 	CALL	_itoa
    // asm 00007596: 	LDI	@TOSTARTBUFFI,AR0
    // asm 00007597: 	LDI	@FCBI,AR1
    // asm 00007598: 	CALL	STRCAT
    // asm 00007599: 	LDI	@TOSTARTBUFFI,AR0
    // asm 0000759A: 	LDI	@SPCI,AR1
    // asm 0000759B: 	CALL	STRCAT
    // asm 0000759C: 	LDI	@TOSTARTBUFFI,AR0
    // asm 0000759D: 	LDI	@CWI,AR1
    // asm 0000759E: 	CALL	GET_CREDITS_TO_START
    // asm 0000759F: 	CMPI	1,R1
    // asm 000075A0: 	LDIEQ	@CWSI,AR1		;ONLY HAVE ON CREDIT USE SINGULAR "CREDIT"
    // asm 000075A1: 	CALL	STRCAT
    // asm 000075A2: 	LDI	@TOSTARTBUFFI,AR0
    // asm 000075A3: 	LDI	@CTS_STRI,AR1
    // asm 000075A4: 	CALL	STRCAT
    // asm 000075A5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOSTART_STRING", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TOCONT_STRING(void)
{
    // asm 000075A6: 	CLRI	R7
    // asm 000075A7: 	STI	R7,@TOSTARTBUFFER	;STRING BUFFER
    // asm 000075A8: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm 000075A9: 	LDI	R1,R2
    // asm 000075AA: 	LDI	1,R7
    // asm 000075AB: 	LDI	@FCBI,AR2
    // asm 000075AC: 	CALL	_itoa
    // asm 000075AD: 	LDI	@TOSTARTBUFFI,AR0
    // asm 000075AE: 	LDI	@FCBI,AR1
    // asm 000075AF: 	CALL	STRCAT
    // asm 000075B0: 	LDI	@TOSTARTBUFFI,AR0
    // asm 000075B1: 	LDI	@SPCI,AR1
    // asm 000075B2: 	CALL	STRCAT
    // asm 000075B3: 	LDI	@TOSTARTBUFFI,AR0
    // asm 000075B4: 	LDI	@CWI,AR1
    // asm 000075B5: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm 000075B6: 	CMPI	1,R1
    // asm 000075B7: 	LDIEQ	@CWSI,AR1		;ONLY HAVE ON CREDIT USE SINGULAR "CREDIT"
    // asm 000075B8: 	CALL	STRCAT
    // asm 000075B9: 	LDI	@TOSTARTBUFFI,AR0
    // asm 000075BA: 	LDI	@CTC_STRI,AR1
    // asm 000075BB: 	CALL	STRCAT
    // asm 000075BC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOCONT_STRING", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*VOLUME DISPLAY
*
*
 */
/* asm: VOLUME_ACTIVE	.bss	VOLUME_ACTIVE,1 */
int VOLUME_ACTIVE;
/* asm: VOLUME_COUNT	.bss	VOLUME_COUNT,4 */
int VOLUME_COUNT[4];
const char VOLUME_TXT[] = "VOLUME";

void VOLUME_DISPLAY(void)
{
    // asm 000075BF: 	LDI	@VOLUME_ACTIVE,R0
    // asm 000075C0: 	RETSZ
    // asm 000075C1: 	DEC	R0
    // asm 000075C2: 	STI	R0,@VOLUME_ACTIVE
    // asm 000075C3: 	LDI	@_MODE,R0
    // asm 000075C4: 	AND	MMODE,R0
    // asm 000075C5: 	CMPI	MDIAG,R0
    // asm 000075C6: 	RETSEQ
    // asm 000075C7: 	READADJ	ADJ_VOLUME
    // asm 000075C9: 	FLOAT	R0,R2
    // asm 000075CA: 	MPYF	0.11764,R2		;30/255
    // asm 000075CB: 	FIX	R2
    // asm 000075CC: 	LDL	VOLUME_COUNT,AR2
    // asm 000075CD: 	CALL	_itoa
    // asm 000075CE: 	FLOAT	180,R2
    // asm 000075CF: 	FLOAT	310,R3
    // asm 000075D0: 	LDI	1,RC
    // asm 000075D1: 	CALL	TEXT_ADDDS
    // asm 000075D2: 	CALL	FONT18REDDS
    // asm 000075D3: 	LDL	VOLUME_TXT,AR2
    // asm 000075D4: 	FLOAT	60,R2
    // asm 000075D5: 	FLOAT	310,R3
    // asm 000075D6: 	LDI	1,RC
    // asm 000075D7: 	CALL	TEXT_ADDDS
    // asm 000075D8: 	CALL	FONT18REDDS
    // asm 000075D9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VOLUME_DISPLAY", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
*
*
 */
void INSERT_MORE_COINS(void)
{
    // asm 000075DA: 	PUSH	R4
    // asm 000075DB: 	READADJ	ADJ_FREE_PLAY
    // asm 000075DD: 	CMPI	1,R0
    // asm 000075DE: 	BEQ	INSERT_COINSXM
    // asm 000075DF: 	CALL	PRINT_TOCONT
    // asm 000075E0: 	READAUD	AUD_CREDITS
    // asm 000075E2: 	LDI	R0,R1
    // asm 000075E3: 	READAUD	AUD_PCREDITS
    // asm 000075E5: 	ADDI	R0,R1
    // asm 000075E6: 	BNZ	ISMCC
    // asm 000075E7: 	CALL	FLASH_INSERTCOINSM
    // asm 000075E8: 	BU	INSERT_COINSXM
ISMCC:
    // asm 000075E9: 	CALL	PRINT_CREDITS
    // asm 000075EA: 	LDI	@CREDITBUFFI,AR2
    // asm 000075EB: 	FLOAT	256,R2
    // asm 000075EC: 	FLOAT	237,R3
    // asm 000075ED: 	LDI	1,RC
    // asm 000075EE: 	CALL	TEXT_ADDDS
    // asm 000075EF: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000075F2: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
INSERT_COINSXM:
    // asm 000075F5: 	POP	R4
    // asm 000075F6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_MORE_COINS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FLASH_INSERTCOINSM(void)
{
    // asm 000075F7: 	LDI	@ICF,R0
    // asm 000075F8: 	BGT	NO_INSERTCOINSM
    // asm 000075F9: 	SUBI	@NFRAMES,R0
    // asm 000075FA: 	CMPI	-30,R0
    // asm 000075FB: 	LDILT	1,R0
    // asm 000075FC: 	STI	R0,@ICF
    // asm 000075FD: 	LDI	@INSERTCOINSI,AR2
    // asm 000075FE: 	FLOAT	256,R2
    // asm 000075FF: 	FLOAT	237,R3
    // asm 00007600: 	LDI	1,RC
    // asm 00007601: 	CALL	TEXT_ADDDS
    // asm 00007602: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00007605: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00007608: 	BU	FLASH_INSERTCOINSXM
NO_INSERTCOINSM:
    // asm 00007609: 	LDI	@ICF,R0
    // asm 0000760A: 	ADDI	@NFRAMES,R0
    // asm 0000760B: 	CMPI	30,R0
    // asm 0000760C: 	LDIGT	-1,R0
    // asm 0000760D: 	STI	R0,@ICF
FLASH_INSERTCOINSXM:
    // asm 0000760E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_INSERTCOINSM", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void PRINT_TOCONT(void)
{
    // asm 0000760F: 	CALL	TOCONT_STRING
    // asm 00007610: 	LDI	@TOSTARTBUFFI,AR2
    // asm 00007611: 	FLOAT	256,R2
    // asm 00007612: 	FLOAT	200,R3
    // asm 00007613: 	LDI	1,RC
    // asm 00007614: 	CALL	TEXT_ADDDS
    // asm 00007615: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00007618: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 0000761B: TOCONTX
    // asm 0000761B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_TOCONT", 0, 0);
    UNIMPL();
}

/* asm: CMOS_WP_WORD_SHADOW	.bss	CMOS_WP_WORD_SHADOW,1 */
int CMOS_WP_WORD_SHADOW;
/* asm: COIN_COUNTER1	.bss	COIN_COUNTER1,1 */
int COIN_COUNTER1;
/* asm: COIN_COUNTER2	.bss	COIN_COUNTER2,1 */
int COIN_COUNTER2;
/* asm: COUNTER_IDX	.bss	COUNTER_IDX,1 */
int COUNTER_IDX;
/* asm: COUNTER_MODE	.bss	COUNTER_MODE,1 */
int COUNTER_MODE;

/* *----------------------------------------------------------------------------
*
*IT TAKES 75ms - 100ms FOR STOBING THE COIN COUNTERS
*
*IF (COUNTER_IDX >= 6)  {
*	IF (COUNTER_MODE)
*
*
*
*}
*ELSE
*	COUNTER_IDX++
*
*
*
*
 */
void COIN_COUNTER(void)
{
    // asm 0000761C: 	LDP	@COUNTER_IDX
    // asm 0000761D: 	LDI	@COUNTER_IDX,R0
    // asm 0000761E: 	INC	R0
    // asm 0000761F: 	STI	R0,@COUNTER_IDX
    // asm 00007620: 	SETDP
    // asm 00007621: 	CMPI	8,R0
    // asm 00007622: 	RETSLT
    // asm 00007623: 	LDP	@COUNTER_IDX
    // asm 00007624: 	CLRI	R0
    // asm 00007625: 	STI	R0,@COUNTER_IDX
    // asm 00007626: 	LDP	@COUNTER_MODE
    // asm 00007627: 	LDI	@COUNTER_MODE,R0
    // asm 00007628: 	BZ	CLEARIT
    // asm 00007629: 	CLRI	R0
    // asm 0000762A: 	STI	R0,@COUNTER_MODE
    // asm 0000762B: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm 0000762C: 	LDI	@CMOS_WP_WORD_SHADOW,R0
    // asm 0000762D: 	ANDN	0F00h,R0
    // asm 0000762E: 	LDP	@COIN_COUNTER1
    // asm 0000762F: 	LDI	@COIN_COUNTER1,R1
    // asm 00007630: 	DEC	R1
    // asm 00007631: 	BLT	NOINC1
    // asm 00007632: 	STI	R1,@COIN_COUNTER1
    // asm 00007633: 	OR	100h,R0
NOINC1:
    // asm 00007634: 	LDP	@COIN_COUNTER2
    // asm 00007635: 	LDI	@COIN_COUNTER2,R1
    // asm 00007636: 	DEC	R1
    // asm 00007637: 	BLT	NOINC2
    // asm 00007638: 	STI	R1,@COIN_COUNTER2
    // asm 00007639: 	OR	200h,R0
NOINC2:
    // asm 0000763A: 	DINT
    // asm 00007640: 	LDP	@CMOS_WP_WORD
    // asm 00007641: 	STI	R0,@CMOS_WP_WORD
    // asm 00007642: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm 00007643: 	STI	R0,@CMOS_WP_WORD_SHADOW
    // asm 00007644: 	EINT
    // asm 00007645: 	SETDP
    // asm 00007646: 	RETS
CLEARIT:
    // asm 00007647: 	LDI	1,R0
    // asm 00007648: 	STI	R0,@COUNTER_MODE
    // asm 00007649: 	DINT
    // asm 0000764F: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm 00007650: 	LDI	@CMOS_WP_WORD_SHADOW,R0
    // asm 00007651: 	ANDN	0F00h,R0
    // asm 00007652: 	STI	R0,@CMOS_WP_WORD_SHADOW
    // asm 00007653: 	LDP	@CMOS_WP_WORD
    // asm 00007654: 	STI	R0,@CMOS_WP_WORD
    // asm 00007655: 	EINT
    // asm 00007656: 	SETDP
    // asm 00007657: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN_COUNTER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: MOTIONDIS	SPTR	"MOTION OFF" */
const char MOTIONDIS[] = "MOTION OFF";

void MOTION_VALID(void)
{
    // asm 00007659: 	CALL	CHECK_MOTION_DIP
    // asm 0000765A: 	RETSNZ			;RETURN IF NON MOVING
    // asm 0000765B: 	CALL	CHECK_MOTION_PRESENT
    // asm 0000765C: 	RETSEQ			;RETURN IF ALL OK
    // asm 0000765D: 	LDI	@MOTIONDIS,AR2
    // asm 0000765E: 	FLOAT	256,R2
    // asm 0000765F: 	FLOAT	305,R3
    // asm 00007660: 	LDI	1,RC
    // asm 00007661: 	CALL	TEXT_ADDDS
    // asm 00007662: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00007665: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00007668: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_VALID", 0, 0);
    UNIMPL();
}

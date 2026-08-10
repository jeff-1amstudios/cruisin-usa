#include "coin.h"

#include "../core/machine.h"
#include "../core/validator.h"
#include "cmos.h"
#include "cointab.h"
#include "diag.h"
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
 * Source module: asm/COIN.ASM
 */

void COIN1(PROC* p);
void COIN2(PROC* p);
void COIN3(PROC* p);
void COIN4(PROC* p);
void SERV_COIN(PROC* p);
static COINTAB_ENTRY* GETCOIN(void);
static void GET_THIS_COIN(void);
static void GET_THIS_COINTXT(void);
char** GETCOINTXT(void);
static void GET_COIN1(void);
static void GET_COIN2(void);
static void GET_COIN3(void);
static void GET_COIN4(void);
static void GET_UNITS_PER_CREDIT(void);
static void GET_UNITS_FOR_BONUS(void);
static void GET_MIN_UNITS(void);
static int GET_CREDITS_TO_START(void);
static void GET_CREDITS_TO_CONTINUE(void);
static void GET_SHOW_PARTIAL(void);
static void GET_COIN1_COUNTER(void);
static void GET_COIN2_COUNTER(void);
static void GET_COIN3_COUNTER(void);
static void GET_COIN4_COUNTER(void);
void GETCOIN_DEFAULT(void);
static void SET_COINAGE_ADJ(void);
void INIT_CUSTOM_COIN(void);
static u32 INICC(int adjustment_index, int shift_start);
static void FONT18RED(tTEXT* t);
static void FONT18REDDS(tSHADOW_TEXT* t);
void INSERT_COINS(void);
static void FLASH_INSERTCOINS(c3x_reg_t posy);
static void SHOW_INSERTCOINS(c3x_reg_t posy);
static void FLASH_START(c3x_reg_t posy /*R3*/);
static void PRINT_COINAGE(c3x_reg_t x, c3x_reg_t y);
static c3x_reg_t GET_COINAGE_HIGHT(void);
static void WHITE10FNT(tSHADOW_TEXT* t);
static void PRINT_CREDITS(void);
static void FLASH_TO_START(c3x_reg_t posy);
static void TOSTART_STRING(void);
static void TOCONT_STRING(void);
void VOLUME_DISPLAY(void);
void INSERT_MORE_COINS(void);
static void FLASH_INSERTCOINSM(void);
static void NO_INSERTCOINS();
static void PRINT_TOCONT(void);
void COIN_COUNTER(void);
static void MOTION_VALID(void);

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
#define INSERTCOINSI INSERTCOINS
#define HITSTARTI HITSTART

#define COIN_TABLEENDI COIN_TABLEEND
#define COUNTRY_DEFAULTS_ENDI COUNTRY_DEFAULTS_END
#define COUNTRY_DEFAULTSI COUNTRY_DEFAULTS
int CHECK_MOTION_DIP(void);
int CHECK_MOTION_PRESENT(void);

static const char NCB[];
static char MSG_NULL[];
static int SCS;

/* asm: COINOFF	.bss	COINOFF,1 */
int COINOFF;

/* asm: CUSTOM_COINTAB	.bss	CUSTOM_COINTAB,COIN_ENTRY_SIZE */
COINTAB_ENTRY CUSTOM_COINTAB;
/* asm: CUSTOM_COINSTR	.bss	CUSTOM_COINSTR,10 */
char CUSTOM_COINSTR[10 * 4];

/*
 *----------------------------------------------------------------------------
 *THESE ROUTINES MUST PRESERVE R3
 */
void COIN1(PROC* p) {
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

void COIN2(PROC* p) {
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

void COIN3(PROC* p) {
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

void COIN4(PROC* p) {
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

void SERV_COIN(PROC* p) {
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

// *----------------------------------------------------------------------------
/*
 *RETURNS
 *	AR0	PTR TO COIN TABLE ENTRY
 */
static COINTAB_ENTRY* GETCOIN(void) {
    // asm 000073CB: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm 000073CC: 	CALL	ADJUSTMENT_READ
    // asm 000073CD: 	CMPI	1,R0
    // asm 000073CE: 	BEQ	USE_STANDARD
    // asm 000073CF: 	LDI	@CUSTOM_COINTABI,AR0
    // asm 000073D0: 	RETS
    if (ADJUSTMENT_READ(ADJ_STANDARD_PRICING) != 1) {
        return &CUSTOM_COINTAB;
    }

USE_STANDARD:
    // asm 000073D1: 	LDI	ADJ_COINMODE,AR2
    // asm 000073D2: 	CALL	ADJUSTMENT_READ
    // asm 000073D3: 	LDI	R0,AR0
    return COIN_TABLE[ADJUSTMENT_READ(ADJ_COINMODE) & 0x7F];
}

static void GET_THIS_COIN(void) {
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

static void GET_THIS_COINTXT(void) {
    // asm 000073DB: 	CALL	GET_THIS_COIN
    // asm 000073DC: 	ADDI	3,AR0		;NOTE AR0 now points to the COIN TABLE ENTRY WHERE THE MESSAGE RESIDES
    // asm 000073DD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_THIS_COINTXT", 0, 0);
    UNIMPL();
}

char** GETCOINTXT(void) {
    // asm 000073DE: 	CALL	GETCOIN
    // asm 000073DF: 	ADDI	3,AR0		;NOTE AR0 now points to the COIN TABLE ENTRY WHERE THE MESSAGE RESIDES
    // asm 000073E0: 	RETS
    return GETCOIN()->message_lines;
}

// *----------------------------------------------------------------------------

/*
 *COIN TABLE ENTRY 11 words total
 *0	byte	coin1,coin2,coin3,coin4
 *1	byte	units per credit,units for bonus,min units,credits to start
 *2	byte	credits to continue,show partial credits,NOT USED,NOT USED
 *3	word	message_string line 1;Must have at lease one message
 *4	word	message_string line 2;	0 = no message on line 2 or 3
 *5	word	message_string line 3;	0 = no message on line 3
 *6	word	message_string coin 1 denaomination	NOTE: NULL$ = NOT USED
 *7	word	message_string coin 2 denaomination	NOTE: NULL$ = NOT USED
 *8	word	message_string coin 3 denaomination	NOTE: NULL$ = NOT USED
 *9	word	message_string coin 4 denaomination	NOTE: NULL$ = NOT USED
 *10	byte	coin1_denom,coin2_denom,coin3_denom,coin4_denom
 *NOTE ALL UNUSED BYTES MUST BE SET TO 0
 */

// *----------------------------------------------------------------------------
static void GET_COIN1(void) {
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

// *----------------------------------------------------------------------------
static void GET_COIN2(void) {
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

// *----------------------------------------------------------------------------
static void GET_COIN3(void) {
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

// *----------------------------------------------------------------------------
static void GET_COIN4(void) {
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

// *----------------------------------------------------------------------------
static void GET_UNITS_PER_CREDIT(void) {
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
    SCS = GETCOIN()->units_per_credit;
}

// *----------------------------------------------------------------------------
static void GET_UNITS_FOR_BONUS(void) {
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

// *----------------------------------------------------------------------------
static void GET_MIN_UNITS(void) {
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

static int GET_CREDITS_TO_START(void) {
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
    return GETCOIN()->credits_to_start;
}

static void GET_CREDITS_TO_CONTINUE(void) {
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
    SCS = GETCOIN()->credits_to_continue;
}

// *----------------------------------------------------------------------------
static void GET_SHOW_PARTIAL(void) {
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
    SCS = GETCOIN()->show_partial_credits;
}

// *----------------------------------------------------------------------------
static void GET_COIN1_COUNTER(void) {
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

// *----------------------------------------------------------------------------
static void GET_COIN2_COUNTER(void) {
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

// *----------------------------------------------------------------------------
static void GET_COIN3_COUNTER(void) {
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

// *----------------------------------------------------------------------------
static void GET_COIN4_COUNTER(void) {
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

/*
 *----------------------------------------------------------------------------
 *Reads DIPSW3 to determine the default value of the coin mode
 *RETURNS
 *	AR2= POINTER TO COIN MODE DATA
 *	R2= COINMODE NUMBER
 */

void GETCOIN_DEFAULT(void) {
    unsigned int dip_value;
    unsigned int reversed_bits;
    int switch_index;
    int country_index;
    COINTAB_ENTRY* default_coin;
    int coin_mode;

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
    dip_value = crusn_mem_rd32(DIPSW);
    dip_value = (~(dip_value >> 24)) & 0xFF;

    reversed_bits = 0;
    for (switch_index = 0; switch_index < 8; ++switch_index) {
        reversed_bits = (reversed_bits << 1) | (dip_value & 1);
        dip_value >>= 1;
    }

    country_index = (reversed_bits & 0x1F) * 4;
    switch_index = (reversed_bits >> 5) & 0x03;

    if (country_index >= 20 * 4) {
        country_index = 0;
    }

    default_coin = COUNTRY_DEFAULTS[country_index + switch_index];
    coin_mode = 0;
    while (COIN_TABLE[coin_mode] != default_coin) {
        coin_mode++;
    }

    ADJUSTMENT_WRITE(ADJ_COINMODE, coin_mode);
}

static void SET_COINAGE_ADJ(void) {
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

// *----------------------------------------------------------------------------
void INIT_CUSTOM_COIN(void) {
    u32 packed_fields;

    // asm 00007434: 	LDI	3*8,R5
    // asm 00007435: 	LDI	@CUSTOM_COINTABI,AR3
    // asm 00007436: 	LDI	ADJ_COIN4_UNITS,AR2
    // asm 00007437: 	CALL	INICC		;Set SLOT SETTINGS coin1,coin2,coin3,coin4
    packed_fields = INICC(ADJ_COIN4_UNITS, 3 * 8);
    CUSTOM_COINTAB.coin[0] = packed_fields & 0xFF;
    CUSTOM_COINTAB.coin[1] = (packed_fields >> 8) & 0xFF;
    CUSTOM_COINTAB.coin[2] = (packed_fields >> 16) & 0xFF;
    CUSTOM_COINTAB.coin[3] = (packed_fields >> 24) & 0xFF;
    // asm 00007438: 	LDI	ADJ_CREDITS_TO_START,AR2
    // asm 00007439: 	LDI	3*8,R5			;Set units per credit,units for bonus,min units,credits to start
    // asm 0000743A: 	CALL	INICC
    packed_fields = INICC(ADJ_CREDITS_TO_START, 3 * 8);
    CUSTOM_COINTAB.units_per_credit = packed_fields & 0xFF;
    CUSTOM_COINTAB.units_for_bonus = (packed_fields >> 8) & 0xFF;
    CUSTOM_COINTAB.min_units = (packed_fields >> 16) & 0xFF;
    CUSTOM_COINTAB.credits_to_start = (packed_fields >> 24) & 0xFF;
    // asm 0000743B: 	LDI	ADJ_SHOW_FRAC,AR2
    // asm 0000743C: 	LDI	1*8,R5			;Set credits to continue,show partial credits,NOT USED,NOT USED
    // asm 0000743D: 	CALL	INICC
    packed_fields = INICC(ADJ_SHOW_FRAC, 1 * 8);
    CUSTOM_COINTAB.credits_to_continue = packed_fields & 0xFF;
    CUSTOM_COINTAB.show_partial_credits = (packed_fields >> 8) & 0xFF;
    CUSTOM_COINTAB.unused0 = 0;
    CUSTOM_COINTAB.unused1 = 0;
    // asm 0000743E: 	LDI	@MSG_NULLI,R0		;Set pointer to the string
    // asm 0000743F: 	STI	R0,*AR3++
    CUSTOM_COINTAB.message_lines[0] = MSG_NULL;
    // asm 00007440: 	LDI	0,R0
    // asm 00007441: 	STI	R0,*AR3++
    // asm 00007442: 	STI	R0,*AR3++
    CUSTOM_COINTAB.message_lines[1] = NULL;
    CUSTOM_COINTAB.message_lines[2] = NULL;
    // asm 00007443: 	LDI	@MSG_NULLI,R0		;Point denomination messages to NULL string
    // asm 00007444: 	STI	R0,*AR3++
    // asm 00007445: 	STI	R0,*AR3++
    // asm 00007446: 	STI	R0,*AR3++
    // asm 00007447: 	STI	R0,*AR3++
    CUSTOM_COINTAB.coin1_denom_string = MSG_NULL;
    CUSTOM_COINTAB.coin2_denom_string = MSG_NULL;
    CUSTOM_COINTAB.coin3_denom_string = MSG_NULL;
    CUSTOM_COINTAB.coin4_denom_string = MSG_NULL;
    // asm 00007448: 	LDI	3*8,R5
    // asm 00007449: 	LDI	ADJ_COIN4_COUNTER,AR2
    // asm 0000744A: 	CALL	INICC		;Set SLOT SETTINGS coin1,coin2,coin3,coin4
    packed_fields = INICC(ADJ_COIN4_COUNTER, 3 * 8);
    CUSTOM_COINTAB.coin_denom[0] = packed_fields & 0xFF;
    CUSTOM_COINTAB.coin_denom[1] = (packed_fields >> 8) & 0xFF;
    CUSTOM_COINTAB.coin_denom[2] = (packed_fields >> 16) & 0xFF;
    CUSTOM_COINTAB.coin_denom[3] = (packed_fields >> 24) & 0xFF;
    // asm 0000744B: ICCX
    // asm 0000744B: 	RETS
}

static u32 INICC(int adjustment_index, int shift_start) {
    u32 packed_fields;
    u32 adjustment_value;

    // asm 0000744C: 	LDI	0,R6
    packed_fields = 0;
INICC_LP:
    // asm 0000744D: 	PUSH	AR2
    // asm 0000744E: 	CALL	ADJUSTMENT_READ
    // asm 0000744F: 	POP	AR2
    // asm 00007450: 	NOP	*AR2--
    adjustment_value = ADJUSTMENT_READ(adjustment_index);
    adjustment_index -= 1;
    // asm 00007451: 	AND	0FFh,R0
    // asm 00007452: 	LSH	R5,R0
    // asm 00007453: 	OR	R0,R6
    packed_fields |= (adjustment_value & 0xFF) << shift_start;
    // asm 00007454: 	SUBI	8,R5
    shift_start -= 8;
    // asm 00007455: 	BGE	INICC_LP
    if (shift_start >= 0) {
        goto INICC_LP;
    }
    // asm 00007456: 	STI	R6,*AR3++
    // asm 00007457: 	RETS
    return packed_fields;
}

/* asm: buffer	.bss	buffer,2 */
int buffer[2];
static const char CTS_STR[] = " TO START";
static const char CTC_STR[] = " TO CONTINUE";
/* asm: FCB	.bss	FCB,1 */
int FCB;
/* asm: PCB	.bss	PCB,1 */
int PCB;
/* asm: CREDITBUFFER	.bss	CREDITBUFFER,8 */
char CREDITBUFFER[8 * 4];
/* asm: TOSTARTBUFFER	.bss	TOSTARTBUFFER,8 */
char TOSTARTBUFFER[8 * 4];
static const char NCB[] = "@";
static const char SPC[] = " ";
static const char DBLSPC[] = "  ";
static const char CW[] = "CREDITS";
static const char CWS[] = "CREDIT";
/* asm: SCI	.word	SCS	;CREDITS TO START (START CREDIT) */
#define SCI SCS
/* asm: SCS	.bss	SCS,1 */
static int SCS;
static char MSG_NULL[] = "";
// *----------------------------------------------------------------------------

static void FONT18RED(tTEXT* t) {
    // asm 00007477: 	LDL	font18_white,AR2
    // asm 00007478: 	CALL	PAL_FIND_RAW
    // asm 00007479: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 0000747A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FONT18RED", 0, 0);
    UNIMPL();
}

static void FONT18REDDS(tSHADOW_TEXT* t) {
    // asm 0000747B: 	LDL	font18_white,AR2
    // asm 0000747C: 	CALL	PAL_FIND_RAW
    // asm 0000747D: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 0000747E: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 0000747F: 	RETS
    int palette;

    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(font18_white_ROM));
    t->front->palette = palette;
    t->shadow->palette = palette;
}

/*
 *----------------------------------------------------------------------------
 *
 *	F  P/N  CREDITS
 *
 */
static const char INSERTCOINS[] = "INSERT COINS";
static const char HITSTART[] = "PRESS START";
/* asm: ICF	.bss	ICF,1 */
int ICF;

void INSERT_COINS(void) {
    int total_credits;
    int credits_to_start;
    c3x_reg_t posy;
    tSHADOW_TEXT t;
    MAME_ASSERT_FUNCTION_ENTRY();

    // asm 00007489: 	CALL	VOLUME_DISPLAY
    VOLUME_DISPLAY();

    // asm 0000748A: 	CALL	MOTION_VALID
    MOTION_VALID();

    // asm 0000748B: 	LDI	@COINOFF,R0
    // asm 0000748C: 	RETSNZ
    if (COINOFF != 0) {
        return;
    }

    // asm 0000748D: 	READADJ	ADJ_FREE_PLAY
    // asm 0000748F: 	FLOAT	360,R3
    posy = C3X_FROM_INT(360);

    // asm 00007490: 	CMPI	1,R0
    // asm 00007491: 	BEQ	FLASH_START
    if (READADJ(ADJ_FREE_PLAY) == 1) {
        FLASH_START(posy);
        return;
    }

    // asm 00007492: 	LDI	@_ATTR_MODE,R0
    // asm 00007493: 	AND	1,R0
    // asm 00007494: 	BZ	SHOW_COINAGE
    if ((_ATTR_MODE & 1) != 0) {
        // asm 00007495: 	READAUD	AUD_PCREDITS
        // asm 00007497: 	PUSH	R0
        // asm 00007498: 	READAUD	AUD_CREDITS
        // asm 0000749A: 	POP	R2
        // asm 0000749B: 	ADDI	R0,R2
        total_credits = READAUD(AUD_PCREDITS) + READAUD(AUD_CREDITS);

        // asm 0000749C: 	BZ	FLASH_INSERTCOINS
        if (total_credits == 0) {
            FLASH_INSERTCOINS(posy);
            return;
        }

        // 	;
        // asm 0000749D: 	LDI	@_MODE,R0
        // asm 0000749E: 	AND	MMODE,R0
        // asm 0000749F: 	CMPI	MINSERT_COINS,R0
        // asm 000074A0: 	BEQ	FLASH_INSERTCOINS
        if ((_MODE & MMODE) == MINSERT_COINS) {
            FLASH_INSERTCOINS(posy);
            return;
        }
    }
SHOW_COINAGE:
    // asm 000074A1: 	CALL	GET_COINAGE_HIGHT	;Sets R3
    posy = GET_COINAGE_HIGHT();

    // asm 000074A2: 	ADDF	44,R3
    posy = C3X_ADD(posy, C3X_FROM_INT(44));

    // asm 000074A3: 	MPYF	-0.5,R3
    posy = C3X_MUL(posy, C3X_IMM_F32(-0.5));

    // asm 000074A4: 	FLOAT	345,R0			;Center of text hight
    // asm 000074A5: 	ADDF	R0,R3
    posy = C3X_ADD(posy, C3X_FROM_INT(345)); /* Center of text hight */

    // asm 000074A6: 	READAUD	AUD_CREDITS
    // asm 000074A8: 	CALL	GET_CREDITS_TO_START
    // asm 000074A9: 	CMPI	R1,R0
    // asm 000074AA: 	BLT	NO_START
    // asm 000074AB: 	CALLGE	FLASH_START
    credits_to_start = GET_CREDITS_TO_START();
    if (READAUD(AUD_CREDITS) >= credits_to_start) {
        FLASH_START(posy);
    }

    // asm 000074AC: 	ADDF	22,R3
    // asm 000074AD: 	BU	SHOW_CREDITS
    if (READAUD(AUD_CREDITS) < credits_to_start) {
    NO_START:
        // asm 000074AE: 	CALL	FLASH_TO_START		;FLASH THE TO START AND TO CONTINUE MESSAGES
        FLASH_TO_START(posy); /* FLASH THE TO START AND TO CONTINUE MESSAGES */
        // asm 000074AF: 	ADDF	22,R3
        posy = C3X_ADD(posy, C3X_FROM_INT(22));
    } else {
        posy = C3X_ADD(posy, C3X_FROM_INT(22));
    }
SHOW_CREDITS:
    // asm 000074B0: 	READAUD	AUD_PCREDITS
    // asm 000074B2: 	PUSH	R0
    // asm 000074B3: 	READAUD	AUD_CREDITS
    // asm 000074B5: 	POP	R2
    // asm 000074B6: 	ADDI	R0,R2
    total_credits = READAUD(AUD_PCREDITS) + READAUD(AUD_CREDITS);

    // asm 000074B7: 	BNZ	GODO_CREDITS
    if (total_credits == 0) {
        // asm 000074B8: 	CALL	SHOW_INSERTCOINS
        SHOW_INSERTCOINS(posy);
        // asm 000074B9: 	BU	DO_COINAGE
        goto DO_COINAGE;
    }
GODO_CREDITS:
    // asm 000074BA: 	CALL	PRINT_CREDITS		;Print the credits message
    PRINT_CREDITS(); /* Print the credits message */

    // asm 000074BB: 	LDI	@CREDITBUFFI,AR2
    // asm 000074BC: 	FLOAT	256,R2
    // asm 000074BD: 	LDI	1,RC
    // asm 000074BE: 	CALL	TEXT_ADDDS
    t = TEXT_ADDDS(CREDITBUFFER, C3X_FROM_INT(256), posy, 1);

    // asm 000074BF: 	CALL	FONT18REDDS
    FONT18REDDS(&t);

    // asm 000074C0: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000074C3: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    t.front->color |= TXT_CENTER;
    t.shadow->color |= TXT_CENTER;
DO_COINAGE:
    // asm 000074C6: 	ADDF	22,R3
    posy = C3X_ADD(posy, C3X_FROM_INT(22));

    // asm 000074C8: 	CALL	PRINT_COINAGE
    PRINT_COINAGE(C3X_FROM_INT(256), posy);

    // asm 000074C9: INSERT_COINSX
    // asm 000074C9: 	RETS
}

/*
 *----------------------------------------------------------------------------
 * This routine FLASHES THE INSERT COINS MESSAGE
 *
 */

static void FLASH_INSERTCOINS(c3x_reg_t posy) {
    // asm 000074CA: 	LDI	@ICF,R0
    // asm 000074CB: 	BGT	NO_INSERTCOINS
    if (ICF > 0) {
        NO_INSERTCOINS();
        return;
    }

    // asm 000074CC: 	SUBI	@NFRAMES,R0
    // asm 000074CD: 	CMPI	-30,R0
    // asm 000074CE: 	LDILT	1,R0
    // asm 000074CF: 	STI	R0,@ICF
    ICF -= NFRAMES;
    if (ICF < -30) {
        ICF = 1;
    }

    SHOW_INSERTCOINS(posy);
}

static void SHOW_INSERTCOINS(c3x_reg_t posy /*R3*/) {
    // asm 000074D3: 	CALL	TEXT_ADDDS
    tSHADOW_TEXT t = TEXT_ADDDS(INSERTCOINS, C3X_FROM_INT(256), posy, 1);

    // asm 000074D4: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000074D7: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    t.front->color |= TXT_CENTER;
    t.shadow->color |= TXT_CENTER;

    // asm 000074DA: 	CALL	FONT18REDDS
    FONT18REDDS(&t);

    // asm 000074DB: 	BU	FLASH_INSERTCOINSX
}

static void NO_INSERTCOINS() {
    // asm 000074DC: 	LDI	@ICF,R0
    // asm 000074DD: 	ADDI	@NFRAMES,R0
    // asm 000074DE: 	CMPI	30,R0
    // asm 000074DF: 	LDIGT	-1,R0
    // asm 000074E0: 	STI	R0,@ICF
    ICF += NFRAMES;
    if (ICF > 30) {
        ICF = -1;
    }
FLASH_INSERTCOINSX:
    // asm 000074E1: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 * This routine FLASHES THE PRESS START MESSAGE
 *
 */
static void FLASH_START(c3x_reg_t posy /*R3*/) {
    tSHADOW_TEXT t;
    int flash_state;

    // asm 000074E2: 	LDI	BUT_START,R0
    // asm 000074E3: 	STI	R0,@BUTTON_STATUS
    BUTTON_STATUS = BUT_START;
    // asm 000074E4: 	LDI	@ICF,R0
    flash_state = ICF;
    // asm 000074E5: 	BGT	START_SKIP
    if (flash_state > 0) {
        goto START_SKIP;
    }
    // asm 000074E6: 	SUBI	@NFRAMES,R0
    flash_state -= NFRAMES;
    // asm 000074E7: 	CMPI	-20,R0
    // asm 000074E8: 	LDILT	1,R0
    if (flash_state < -20) {
        flash_state = 1;
    }
    // asm 000074E9: 	STI	R0,@ICF
    ICF = flash_state;
    // asm 000074EA: 	LDI	@HITSTARTI,AR2
    // asm 000074EB: 	FLOAT	256,R2
    // asm 000074EC: 	LDI	1,RC
    // asm 000074ED: 	CALL	TEXT_ADDDS
    t = TEXT_ADDDS(HITSTART, C3X_FROM_INT(256), posy, 1);
    // asm 000074EE: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000074F1: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    t.front->color |= TXT_CENTER;
    t.shadow->color |= TXT_CENTER;
    // asm 000074F4: 	CALL	FONT18REDDS
    FONT18REDDS(&t);
    // asm 000074F5: 	BU	FLASH_STARTX
    goto FLASH_STARTX;
START_SKIP:
    // asm 000074F6: 	ADDI	@NFRAMES,R0
    flash_state += NFRAMES;
    // asm 000074F7: 	CMPI	20,R0
    // asm 000074F8: 	LDIGT	-1,R0
    if (flash_state > 20) {
        flash_state = -1;
    }
    // asm 000074F9: 	STI	R0,@ICF
    ICF = flash_state;
    // asm 000074FA: 	BU	FLASH_STARTX
FLASH_STARTX:
    // asm 000074FB: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *PRINT_COINAGE
 *
 *Prints up to 3 lines of text that describes credits per coin
 *
 *INPUT
 *	R2 (FLOAT)	= X screen position in pixels
 *	R3 (FLOAT)	= Y screen position in pixels
 */
#define FONT10_HIGHT 17.0

static void PRINT_COINAGE(c3x_reg_t x, c3x_reg_t y) {
    COINTAB_ENTRY* coin_text;
    tSHADOW_TEXT t;

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
    coin_text = GETCOIN();

    t = TEXT_ADDDS(coin_text->message_lines[0], x, y, 1);
    t.front->color |= TXT_CENTER;
    t.shadow->color |= TXT_CENTER;
    SET12FONTDS(&t);
    WHITE10FNT(&t);

    if (coin_text->message_lines[1] == 0) {
        goto PRINT_COINAGEX;
    }

    y = C3X_ADD(y, C3X_IMM_F32(FONT10_HIGHT));
    t = TEXT_ADDDS(coin_text->message_lines[1], x, y, 1);
    t.front->color |= TXT_CENTER;
    t.shadow->color |= TXT_CENTER;
    SET12FONTDS(&t);
    WHITE10FNT(&t);

    if (coin_text->message_lines[2] == 0) {
        goto PRINT_COINAGEX;
    }

    y = C3X_ADD(y, C3X_IMM_F32(FONT10_HIGHT));
    t = TEXT_ADDDS(coin_text->message_lines[2], x, y, 1);
    t.front->color |= TXT_CENTER;
    t.shadow->color |= TXT_CENTER;
    SET12FONTDS(&t);
    WHITE10FNT(&t);
PRINT_COINAGEX:
    // asm 00007526: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *GET_COINAGE_HIGHT
 *
 *Calculates the hight in pixels of the coinage text for centering
 *
 *RETURNS
 *	R3 (FLOAT)	= Y screen hight in pixels
 */

static c3x_reg_t GET_COINAGE_HIGHT(void) {
    char** coin_text;
    c3x_reg_t height;

    // asm 00007527: 	PUSHF	R0
    // asm 00007528: 	PUSH	R0
    // asm 00007529: 	PUSH	AR0
    // asm 0000752A: 	CALL	GETCOINTXT
    // asm 0000752B: 	LDF	FONT10_HIGHT,R3
    coin_text = GETCOINTXT();
    height = C3X_IMM_F32(FONT10_HIGHT);

    // asm 0000752C: 	LDI	*+AR0(1),R0
    // asm 0000752D: 	BEQ	GCHX
    if (coin_text[1] != 0) {
        // asm 0000752E: 	ADDF	FONT10_HIGHT,R3
        height = C3X_ADD(height, C3X_IMM_F32(FONT10_HIGHT));
        // asm 0000752F: 	LDI	*+AR0(2),R0
        // asm 00007530: 	BEQ	GCHX
        if (coin_text[2] != 0) {
            // asm 00007531: 	ADDF	FONT10_HIGHT,R3
            height = C3X_ADD(height, C3X_IMM_F32(FONT10_HIGHT));
        }
    }
GCHX:
    // asm 00007532: 	POP	AR0
    // asm 00007533: 	POP	R0
    // asm 00007534: 	POPF	R0
    // asm 00007535: 	RETS
    return height;
}

static void WHITE10FNT(tSHADOW_TEXT* t) {
    int palette;

    // asm 00007536: 	LDL	osg10fnt_white,AR2
    // asm 00007537: 	CALL	PAL_FIND_RAW
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(osg10fnt_white_ROM));
    // asm 00007538: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00007539: 	STI	R0,*+AR1(TEXT_PAL)
    t->front->palette = palette;
    t->shadow->palette = palette;
    // asm 0000753A: 	RETS
}

/*
 *----------------------------------------------------------------------------
 *PRINT_CREDITS
 *
 *Prints the CREDITS message including partial credits if the mode is on.
 *
 */

static void PRINT_CREDITS(void) {
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

/*
 *----------------------------------------------------------------------------
 *FLASH_TO_START
 *
 *Flash the messages N CREDTIS TO START/N CREDITS TO CONTINUE MESSAGE
 */

static void FLASH_TO_START(c3x_reg_t posy) {
    int flash_state;
    int credits_to_start;
    int credits_to_continue;
    tSHADOW_TEXT t;

    // asm 0000756F: 	LDI	@ICF,R0
    // asm 00007570: 	BGT	NO_TOSTART
    // asm 00007571: 	SUBI	@NFRAMES,R0
    // asm 00007572: 	CMPI	-45,R0
    // asm 00007573: 	LDILT	1,R0
    // asm 00007574: 	STI	R0,@ICF
    // asm 00007575: 	CALL	TOSTART_STRING
    // asm 00007576: 	BU	PRINT_TOSTART
    flash_state = ICF;
    if (flash_state <= 0) {
        flash_state -= NFRAMES;
        if (flash_state < -45) {
            flash_state = 1;
        }
        ICF = flash_state;
        TOSTART_STRING();
        goto PRINT_TOSTART;
    }
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
    flash_state += NFRAMES;
    if (flash_state > 45) {
        flash_state = -1;
    }
    ICF = flash_state;
    credits_to_start = GET_CREDITS_TO_START();
    GET_CREDITS_TO_CONTINUE();
    credits_to_continue = SCS;
    if (credits_to_continue == credits_to_start) {
        return;
    }
    TOCONT_STRING();
PRINT_TOSTART:
    // asm 00007583: 	LDI	@TOSTARTBUFFI,AR2
    // asm 00007584: 	FLOAT	256,R2
    // asm 00007585: 	LDI	1,RC
    // asm 00007586: 	CALL	TEXT_ADDDS
    // asm 00007587: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 0000758A: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 0000758D: 	CALL	FONT18REDDS
    t = TEXT_ADDDS(TOSTARTBUFFER, C3X_FROM_INT(256), posy, 1);
    t.front->color |= TXT_CENTER;
    t.shadow->color |= TXT_CENTER;
    FONT18REDDS(&t);
FLASH_TOSTARTX:
    // asm 0000758E: 	RETS
}

// *----------------------------------------------------------------------------
static void TOSTART_STRING(void) {
    // asm 0000758F: 	CLRI	R7
    // asm 00007590: 	STI	R7,@TOSTARTBUFFER	;STRING BUFFER
    TOSTARTBUFFER[0] = '\0'; // ;STRING BUFFER

    // asm 00007591: 	CALL	GET_CREDITS_TO_START
    // asm 00007592: 	LDI	R1,R2
    // asm 00007593: 	LDI	1,R7
    // asm 00007594: 	LDI	@FCBI,AR2
    // asm 00007595: 	CALL	_itoa
    SCS = GET_CREDITS_TO_START();
    _itoa((char*)&FCB, SCS);

    // asm 00007596: 	LDI	@TOSTARTBUFFI,AR0
    // asm 00007597: 	LDI	@FCBI,AR1
    // asm 00007598: 	CALL	STRCAT
    STRCAT(TOSTARTBUFFER, (char*)&FCB);

    // asm 00007599: 	LDI	@TOSTARTBUFFI,AR0
    // asm 0000759A: 	LDI	@SPCI,AR1
    // asm 0000759B: 	CALL	STRCAT
    STRCAT(TOSTARTBUFFER, SPCI);

    // asm 0000759C: 	LDI	@TOSTARTBUFFI,AR0
    // asm 0000759D: 	LDI	@CWI,AR1
    // asm 0000759E: 	CALL	GET_CREDITS_TO_START
    // asm 0000759F: 	CMPI	1,R1
    // asm 000075A0: 	LDIEQ	@CWSI,AR1		;ONLY HAVE ON CREDIT USE SINGULAR "CREDIT"
    // asm 000075A1: 	CALL	STRCAT
    if (GET_CREDITS_TO_START() == 1) {
        STRCAT(TOSTARTBUFFER, CWSI); // ;ONLY HAVE ON CREDIT USE SINGULAR "CREDIT"
    } else {
        STRCAT(TOSTARTBUFFER, CWI);
    }

    // asm 000075A2: 	LDI	@TOSTARTBUFFI,AR0
    // asm 000075A3: 	LDI	@CTS_STRI,AR1
    // asm 000075A4: 	CALL	STRCAT
    STRCAT(TOSTARTBUFFER, CTS_STRI);

    // asm 000075A5: 	RETS
}

// *----------------------------------------------------------------------------
static void TOCONT_STRING(void) {
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
    TOSTARTBUFFER[0] = '\0';
    GET_CREDITS_TO_CONTINUE();
    _itoa((char*)&FCB, SCS);
    STRCAT(TOSTARTBUFFER, (char*)&FCB);
    STRCAT(TOSTARTBUFFER, SPCI);
    GET_CREDITS_TO_CONTINUE();
    if (SCS == 1) {
        STRCAT(TOSTARTBUFFER, CWSI);
    } else {
        STRCAT(TOSTARTBUFFER, CWI);
    }
    STRCAT(TOSTARTBUFFER, CTC_STRI);
}

/*
 *----------------------------------------------------------------------------
 *VOLUME DISPLAY
 *
 *
 */
/* asm: VOLUME_ACTIVE	.bss	VOLUME_ACTIVE,1 */
int VOLUME_ACTIVE;
/* asm: VOLUME_COUNT	.bss	VOLUME_COUNT,4 */
int VOLUME_COUNT[4];
static const char VOLUME_TXT[] = "VOLUME";

void VOLUME_DISPLAY(void) {
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
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *
 *
 */
void INSERT_MORE_COINS(void) {
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

// *----------------------------------------------------------------------------
static void FLASH_INSERTCOINSM(void) {
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

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void PRINT_TOCONT(void) {
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

// *----------------------------------------------------------------------------

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

/*
 *----------------------------------------------------------------------------
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
void COIN_COUNTER(void) {
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
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: MOTIONDIS	SPTR	"MOTION OFF" */
static const char MOTIONDIS[] = "MOTION OFF";

static void MOTION_VALID(void) {
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
    UNIMPL_TODO();
}

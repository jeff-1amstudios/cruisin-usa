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
#include "coin.h"
#include "diag_defs.h"

/*
 * Source module: asm/COIN.ASM
 */

/* asm: COINOFF	.bss	COINOFF,1 */
int COINOFF;
/* asm: CUSTOM_COINTAB	.bss	CUSTOM_COINTAB,COIN_ENTRY_SIZE */
int CUSTOM_COINTAB[COIN_ENTRY_SIZE];
/* asm: CUSTOM_COINSTR	.bss	CUSTOM_COINSTR,10 */
int CUSTOM_COINSTR[10];
/* asm: bufferi	.word	buffer */
#define bufferi buffer
/* asm: buffer	.bss	buffer,2 */
int buffer[2];
const char *CTS_STR = " TO START";
const char *CTC_STR = " TO CONTINUE";
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
const char *NCB = "@";
const char *SPC = " ";
const char *DBLSPC = "  ";
const char *CW = "CREDITS";
const char *CWS = "CREDIT";
/* asm: SCI	.word	SCS	;CREDITS TO START (START CREDIT) */
#define SCI SCS
/* asm: SCS	.bss	SCS,1 */
int SCS;
const char *MSG_NULL = "";
/* *----------------------------------------------------------------------------
*
*	F  P/N  CREDITS
*
 */
const char *INSERTCOINS = "INSERT COINS";
const char *HITSTART = "PRESS START";
/* asm: ICF	.bss	ICF,1 */
int ICF;
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
/* *----------------------------------------------------------------------------
*VOLUME DISPLAY
*
*
 */
/* asm: VOLUME_ACTIVE	.bss	VOLUME_ACTIVE,1 */
int VOLUME_ACTIVE;
/* asm: VOLUME_COUNT	.bss	VOLUME_COUNT,4 */
int VOLUME_COUNT[4];
const char *VOLUME_TXT = "VOLUME";
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
 */
/* asm: MOTIONDIS	SPTR	"MOTION OFF" */
const char *MOTIONDIS = "MOTION OFF";

/* *----------------------------------------------------------------------------
*THESE ROUTINES MUST PRESERVE R3
 */
void COIN1(void)
{
    // asm: 	SOND1	COININ
    // asm: 	INCAUD	AUD_COIN1
    // asm: 	CALL	SEND_COINDROP
    // asm: 	CALL	GET_COIN1_COUNTER
    // asm: 	LDP	@COIN_COUNTER1
    // asm: 	LDI	@COIN_COUNTER1,R1
    // asm: 	ADDI	R0,R1
    // ;	INC	R1
    // asm: 	STI	R1,@COIN_COUNTER1
    // asm: 	SETDP
    // asm: 	CALL	GET_COIN1
    // asm: 	BU	CHECK_CREDITS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN1", 0, 0);
    UNIMPL();
}

void COIN2(void)
{
    // asm: 	SOND1	COININ
    // asm: 	INCAUD	AUD_COIN2
    // asm: 	CALL	SEND_COINDROP
    // ;this is a possibly permenant kludge of only having
    // ;1 coin counter
    // ;
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_COINCNTR,R0
    // asm: 	BZ	JAJA3
    // asm: 	CALL	GET_COIN2_COUNTER
    // asm: 	LDP	@COIN_COUNTER1
    // asm: 	LDI	@COIN_COUNTER1,R1
    // asm: 	ADDI	R0,R1
    // ;	INC	R1
    // asm: 	STI	R1,@COIN_COUNTER1
    // asm: 	SETDP
    // asm: 	BU	JAJA5
JAJA3:
    // asm: 	CALL	GET_COIN2_COUNTER
    // asm: 	LDP	@COIN_COUNTER2
    // asm: 	LDI	@COIN_COUNTER2,R1
    // asm: 	ADDI	R0,R1
    // asm: 	STI	R1,@COIN_COUNTER2
    // asm: 	SETDP
JAJA5:
    // asm: 	CALL	GET_COIN2
    // asm: 	BU	CHECK_CREDITS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN2", 0, 0);
    UNIMPL();
}

void COIN3(void)
{
    // asm: 	SOND1	COININ
    // asm: 	INCAUD	AUD_COIN2
    // asm: 	CALL	GET_COIN3_COUNTER
    // asm: 	LDP	@COIN_COUNTER1
    // asm: 	LDI	@COIN_COUNTER1,R1
    // asm: 	ADDI	R0,R1
    // asm: 	STI	R1,@COIN_COUNTER1
    // asm: 	SETDP
    // asm: 	CALL	GET_COIN3
    // asm: 	BU	CHECK_CREDITS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN3", 0, 0);
    UNIMPL();
}

void COIN4(void)
{
    // asm: 	SOND1	COININ
    // asm: 	INCAUD	AUD_COIN4
    // asm: 	CALL	GET_COIN4_COUNTER
    // asm: 	LDP	@COIN_COUNTER1
    // asm: 	LDI	@COIN_COUNTER1,R1
    // asm: 	ADDI	R0,R1
    // asm: 	STI	R1,@COIN_COUNTER1
    // asm: 	SETDP
    // asm: 	CALL	GET_COIN4
    // asm: 	BU	CHECK_CREDITS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN4", 0, 0);
    UNIMPL();
}

void SERV_COIN(void)
{
    // asm: 	SOND1	COININ
    // asm: 	INCAUD	AUD_SERVICE_CREDITS
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	LDI	R0,R1
    // asm: 	INC	R1
    // asm: 	READAUD	ADJ_MAX_CREDITS
    // asm: 	CMPI	R0,R1
    // asm: 	LDIGE	R0,R1
    // asm: 	LDI	R1,R2
    // asm: 	SETAUD	AUD_CREDITS
    // asm: 	DIE
CHECK_CREDITS:
    // 	;R2 HAS PCREDITS TO INCREMENT
    // 	;
    // asm: 	CMPI	0,R2			;ZERO COIN SLOT? THIS will mees up the bonus system
    // asm: 	BEQ	CHECK_CREDITSX
    // asm: 	LDI	R2,R1			;ADD TO PCREDITS
    // asm: 	ADDAUD	AUD_PCREDITS		;R2 HAS NEW VALUE
    // asm: 	LDI	R1,R2			;ADD TO BCREDITS
    // asm: 	ADDAUD	AUD_BCREDITS		;BONUS COUNTER
    // asm: 	READAUD	AUD_PCREDITS		;READ BACK THE PCREDITS
    // asm: 	LDI	R0,R2
CHECK_CREDITSLP:
    // ;FIRST CHECK TO SEE IF MORE THAN MIN PCREDITS FOR A CREDIT
    // asm: 	CALL	GET_MIN_UNITS
    // asm: 	CMPI	R1,R2
    // asm: 	BLT	NOINC
    // asm: SKIP_MIN
    // asm: 	CALL	GET_UNITS_PER_CREDIT
    // asm: 	CMPI	R1,R2
    // asm: 	BLT	NOINC
    // asm: 	SUBI	R1,R2
    // asm: 	SETAUD	AUD_PCREDITS
    // asm: 	PUSH	R2
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	PUSH	R2
    // asm: 	LDI	R0,R1
    // asm: 	INC	R1
    // asm: 	READAUD	ADJ_MAX_CREDITS
    // asm: 	CMPI	R0,R1
    // asm: 	LDIGE	R0,R1
    // asm: 	LDI	R1,R2
    // asm: 	SETAUD	AUD_CREDITS
    // asm: 	POP	R2
    // asm: 	INCAUD	AUD_PAID_CREDITS
    // asm: NOINCCRD
    // asm: 	POP	R2
    // asm: 	BU	CHECK_CREDITSLP
NOINC:
    // asm: 	READAUD	AUD_BCREDITS
    // asm: 	LDI	R0,R2
    // asm: 	CALL	GET_UNITS_FOR_BONUS
    // asm: 	CMPI	0,R1
    // asm: 	BEQ	NOBONUS
    // asm: 	CMPI	R1,R2
    // asm: 	BLT	NOBONUS
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_BCREDITS,AR2
    // asm: 	CALL	AUDIT_WRITE
    // asm: 	READAUD	AUD_CREDITS		;BONUS CREDIT
    // asm: 	LDI	R0,R1
    // asm: 	ADDI	1,R1			;Actually give the bonus
    // asm: 	READAUD	ADJ_MAX_CREDITS
    // asm: 	CMPI	R0,R1
    // asm: 	LDIGE	R0,R1
    // asm: 	LDI	R1,R2
    // asm: 	SETAUD	AUD_CREDITS
    // asm: 	LDI	0,R2
    // asm: 	SETAUD	AUD_PCREDITS
NOBONUS:
CHECK_CREDITSX:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SERV_COIN", 0, 0);
    UNIMPL();
}

void GETCOIN(void)
{
    // asm: 	LDI	ADJ_STANDARD_PRICING,AR2
    // asm: 	CALL	ADJUSTMENT_READ
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	USE_STANDARD
    // asm: 	LDI	@CUSTOM_COINTABI,AR0
    // asm: 	RETS
USE_STANDARD:
    // asm: 	LDI	ADJ_COINMODE,AR2
    // asm: 	CALL	ADJUSTMENT_READ
    // asm: 	LDI	R0,AR0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCOIN", 0, 0);
    UNIMPL();
}

void GET_THIS_COIN(void)
{
    // asm: 	AND	07Fh,AR0
    // asm: 	MPYI	COIN_ENTRY_SIZE,AR0
    // asm: 	ADDI	@COIN_TABLEI,AR0
    // asm: 	CMPI	@COIN_TABLEENDI,AR0
    // asm: 	BLT	GETCOINX
    // asm: 	LDI	@COIN_TABLEI,AR0
GETCOINX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_THIS_COIN", 0, 0);
    UNIMPL();
}

void GET_THIS_COINTXT(void)
{
    // asm: 	CALL	GET_THIS_COIN
    // asm: 	ADDI	3,AR0		;NOTE AR0 now points to the COIN TABLE ENTRY WHERE THE MESSAGE RESIDES
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_THIS_COINTXT", 0, 0);
    UNIMPL();
}

void GETCOINTXT(void)
{
    // asm: 	CALL	GETCOIN
    // asm: 	ADDI	3,AR0		;NOTE AR0 now points to the COIN TABLE ENTRY WHERE THE MESSAGE RESIDES
    // asm: 	RETS
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COIN4_COUNTER", 0, 0);
    UNIMPL();
}

void GETCOIN_DEFAULT(void)
{
    // asm: 	PUSH	AR2
    // asm: 	CLRI	AR2
    // asm: 	LDP	@DIPSW
    // asm: 	LDI	@DIPSW,R2
    // asm: 	LDI	*AR2,AR2
    // asm: 	POP	AR2
    // asm: 	SETDP
    // asm: 	RS	24,R2
    // asm: 	NOT	R2
    // asm: 	LDI	0,R0
    // asm: 	LDI	7,RC
    // asm: 	RPTB	INVBLP
    // asm: 	LSH	-1,R2
INVBLP:
    // asm: ROLC	R0
    // asm: 	LDI	R0,R2
    // asm: 	AND	060h,R2
    // asm: 	LSH	-5,R2
    // asm: 	LDI	@COUNTRY_DEFAULTSI,AR2
    // asm: 	AND	01Fh,R0
    // asm: 	MPYI	4,R0
    // asm: 	ADDI	R0,AR2
    // asm: 	ADDI	R2,AR2
    // asm: 	CMPI	@COUNTRY_DEFAULTS_ENDI,AR2
    // asm: 	LDIGT	@COUNTRY_DEFAULTSI,AR2
    // asm: 	LDI	*AR2,R0
    // asm: 	SUBI	@COIN_TABLEI,R0
    // asm: 	LDI	COIN_ENTRY_SIZE,R1
    // asm: 	CALL	DIV_I
    // asm: 	LDI	R0,R2
    // asm: 	LDI	*AR2,AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCOIN_DEFAULT", 0, 0);
    UNIMPL();
}

void SET_COINAGE_ADJ(void)
{
    // asm: 	CALL	PUSHALL		;Take no chances
    // asm: 	CALL	GET_COIN1
    // asm: 	SETADJ	ADJ_COIN1_UNITS
    // asm: 	CALL	GET_COIN2
    // asm: 	SETADJ	ADJ_COIN2_UNITS
    // asm: 	CALL	GET_COIN3
    // asm: 	SETADJ	ADJ_COIN3_UNITS
    // asm: 	CALL	GET_COIN4
    // asm: 	SETADJ	ADJ_COIN4_UNITS
    // asm: 	CALL	GET_UNITS_PER_CREDIT
    // asm: 	LDI	R1,R2
    // asm: 	SETADJ	ADJ_UNITS_PER_CREDIT
    // asm: 	CALL	GET_UNITS_FOR_BONUS
    // asm: 	LDI	R1,R2
    // asm: 	SETADJ	ADJ_BONUS_UNITS
    // asm: 	CALL	GET_MIN_UNITS
    // asm: 	LDI	R1,R2
    // asm: 	SETADJ	ADJ_UNITS_MIN
    // asm: 	CALL	GET_CREDITS_TO_START
    // asm: 	LDI	R1,R2
    // asm: 	SETADJ	ADJ_CREDITS_TO_START
    // asm: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm: 	LDI	R1,R2
    // asm: 	SETADJ	ADJ_CREDITS_TO_CONTINUE
    // asm: 	CALL	GET_SHOW_PARTIAL
    // asm: 	LDI	R1,R2
    // asm: 	SETADJ	ADJ_SHOW_FRAC
    // asm: 	CALL	GET_COIN1_COUNTER
    // asm: 	SETADJ	ADJ_COIN1_COUNTER
    // asm: 	CALL	GET_COIN2_COUNTER
    // asm: 	SETADJ	ADJ_COIN2_COUNTER
    // asm: 	CALL	GET_COIN3_COUNTER
    // asm: 	SETADJ	ADJ_COIN3_COUNTER
    // asm: 	CALL	GET_COIN4_COUNTER
    // asm: 	SETADJ	ADJ_COIN4_COUNTER
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_COINAGE_ADJ", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void INIT_CUSTOM_COIN(void)
{
    // asm: 	LDI	3*8,R5
    // asm: 	LDI	@CUSTOM_COINTABI,AR3
    // asm: 	LDI	ADJ_COIN4_UNITS,AR2
    // asm: 	CALL	INICC		;Set SLOT SETTINGS coin1,coin2,coin3,coin4
    // asm: 	LDI	ADJ_CREDITS_TO_START,AR2
    // asm: 	LDI	3*8,R5			;Set units per credit,units for bonus,min units,credits to start
    // asm: 	CALL	INICC
    // asm: 	LDI	ADJ_SHOW_FRAC,AR2
    // asm: 	LDI	1*8,R5			;Set credits to continue,show partial credits,NOT USED,NOT USED
    // asm: 	CALL	INICC
    // asm: 	LDI	@MSG_NULLI,R0		;Set pointer to the string
    // asm: 	STI	R0,*AR3++
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR3++
    // asm: 	STI	R0,*AR3++
    // asm: 	LDI	@MSG_NULLI,R0		;Point denomination messages to NULL string
    // asm: 	STI	R0,*AR3++
    // asm: 	STI	R0,*AR3++
    // asm: 	STI	R0,*AR3++
    // asm: 	STI	R0,*AR3++
    // asm: 	LDI	3*8,R5
    // asm: 	LDI	ADJ_COIN4_COUNTER,AR2
    // asm: 	CALL	INICC		;Set SLOT SETTINGS coin1,coin2,coin3,coin4
    // asm: ICCX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_CUSTOM_COIN", 0, 0);
    UNIMPL();
}

void INICC(void)
{
    // asm: 	LDI	0,R6
INICC_LP:
    // asm: 	PUSH	AR2
    // asm: 	CALL	ADJUSTMENT_READ
    // asm: 	POP	AR2
    // asm: 	NOP	*AR2--
    // asm: 	AND	0FFh,R0
    // asm: 	LSH	R5,R0
    // asm: 	OR	R0,R6
    // asm: 	SUBI	8,R5
    // asm: 	BGE	INICC_LP
    // asm: 	STI	R6,*AR3++
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INICC", 0, 0);
    UNIMPL();
}

void FONT18RED(void)
{
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FONT18RED", 0, 0);
    UNIMPL();
}

void FONT18REDDS(void)
{
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FONT18REDDS", 0, 0);
    UNIMPL();
}

void INSERT_COINS(void)
{
    // asm: 	CALL	VOLUME_DISPLAY
    // asm: 	CALL	MOTION_VALID
    // asm: 	LDI	@COINOFF,R0
    // asm: 	RETSNZ
    // asm: 	READADJ	ADJ_FREE_PLAY
    // asm: 	FLOAT	360,R3
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	FLASH_START
    // asm: 	LDI	@_ATTR_MODE,R0
    // asm: 	AND	1,R0
    // asm: 	BZ	SHOW_COINAGE
    // asm: 	READAUD	AUD_PCREDITS
    // asm: 	PUSH	R0
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	POP	R2
    // asm: 	ADDI	R0,R2
    // asm: 	BZ	FLASH_INSERTCOINS
    // 	;
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MINSERT_COINS,R0
    // asm: 	BEQ	FLASH_INSERTCOINS
SHOW_COINAGE:
    // asm: 	CALL	GET_COINAGE_HIGHT	;Sets R3
    // asm: 	ADDF	44,R3
    // asm: 	MPYF	-0.5,R3
    // asm: 	FLOAT	345,R0			;Center of text hight
    // asm: 	ADDF	R0,R3
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	CALL	GET_CREDITS_TO_START
    // asm: 	CMPI	R1,R0
    // asm: 	BLT	NO_START
    // asm: 	CALLGE	FLASH_START
    // asm: 	ADDF	22,R3
    // asm: 	BU	SHOW_CREDITS
NO_START:
    // asm: 	CALL	FLASH_TO_START		;FLASH THE TO START AND TO CONTINUE MESSAGES
    // asm: 	ADDF	22,R3
SHOW_CREDITS:
    // asm: 	READAUD	AUD_PCREDITS
    // asm: 	PUSH	R0
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	POP	R2
    // asm: 	ADDI	R0,R2
    // asm: 	BNZ	GODO_CREDITS
    // asm: 	CALL	SHOW_INSERTCOINS
    // asm: 	BU	DO_COINAGE
GODO_CREDITS:
    // asm: 	CALL	PRINT_CREDITS		;Print the credits message
    // asm: 	LDI	@CREDITBUFFI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	FONT18REDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
DO_COINAGE:
    // asm: 	ADDF	22,R3
    // asm: 	FLOAT	256,R2
    // ;	FLOAT	345,R3
    // asm: 	CALL	PRINT_COINAGE
    // asm: INSERT_COINSX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_COINS", 0, 0);
    UNIMPL();
}

void FLASH_INSERTCOINS(void)
{
    // asm: 	LDI	@ICF,R0
    // asm: 	BGT	NO_INSERTCOINS
    // asm: 	SUBI	@NFRAMES,R0
    // asm: 	CMPI	-30,R0
    // asm: 	LDILT	1,R0
    // asm: 	STI	R0,@ICF
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_INSERTCOINS", 0, 0);
    UNIMPL();
}

void SHOW_INSERTCOINS(void)
{
    // asm: 	LDI	@INSERTCOINSI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	CALL	FONT18REDDS
    // asm: 	BU	FLASH_INSERTCOINSX
NO_INSERTCOINS:
    // asm: 	LDI	@ICF,R0
    // asm: 	ADDI	@NFRAMES,R0
    // asm: 	CMPI	30,R0
    // asm: 	LDIGT	-1,R0
    // asm: 	STI	R0,@ICF
FLASH_INSERTCOINSX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_INSERTCOINS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
* This routine FLASHES THE PRESS START MESSAGE
*
 */
void FLASH_START(void)
{
    // asm: 	LDI	BUT_START,R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	LDI	@ICF,R0
    // asm: 	BGT	START_SKIP
    // asm: 	SUBI	@NFRAMES,R0
    // asm: 	CMPI	-20,R0
    // asm: 	LDILT	1,R0
    // asm: 	STI	R0,@ICF
    // asm: 	LDI	@HITSTARTI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	CALL	FONT18REDDS
    // asm: 	BU	FLASH_STARTX
START_SKIP:
    // asm: 	ADDI	@NFRAMES,R0
    // asm: 	CMPI	20,R0
    // asm: 	LDIGT	-1,R0
    // asm: 	STI	R0,@ICF
    // asm: 	BU	FLASH_STARTX
FLASH_STARTX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_START", 0, 0);
    UNIMPL();
}

void PRINT_COINAGE(void)
{
    // asm: 	CALL	GETCOINTXT
    // asm: 	LDI	*AR0,AR2
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	CALL	SET12FONTDS
    // asm: 	CALL	WHITE10FNT
    // asm: 	CALL	GETCOINTXT
    // asm: 	LDI	*+AR0(1),AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	PRINT_COINAGEX
    // asm: 	ADDF	FONT10_HIGHT,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	CALL	SET12FONTDS
    // asm: 	CALL	WHITE10FNT
    // asm: 	CALL	GETCOINTXT
    // asm: 	LDI	*+AR0(2),AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	PRINT_COINAGEX
    // asm: 	ADDF	FONT10_HIGHT,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	CALL	SET12FONTDS
    // asm: 	CALL	WHITE10FNT
PRINT_COINAGEX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_COINAGE", 0, 0);
    UNIMPL();
}

void GET_COINAGE_HIGHT(void)
{
    // asm: 	PUSHF	R0
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	CALL	GETCOINTXT
    // asm: 	LDF	FONT10_HIGHT,R3
    // asm: 	LDI	*+AR0(1),R0
    // asm: 	BEQ	GCHX
    // asm: 	ADDF	FONT10_HIGHT,R3
    // asm: 	LDI	*+AR0(2),R0
    // asm: 	BEQ	GCHX
    // asm: 	ADDF	FONT10_HIGHT,R3
GCHX:
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	POPF	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_COINAGE_HIGHT", 0, 0);
    UNIMPL();
}

void WHITE10FNT(void)
{
    // asm: 	LDL	osg10fnt_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WHITE10FNT", 0, 0);
    UNIMPL();
}

void PRINT_CREDITS(void)
{
    // asm: 	CLRI	R7
    // asm: 	STI	R7,@CREDITBUFFER	;STRING BUFFER
    // asm: 	CALL	GET_SHOW_PARTIAL
    // asm: 	LDI	R1,R4
    // asm: 	READAUD	AUD_CREDITS		;DO WE HAVE ANY CREDITS?
    // asm: 	LDI	R0,R2
    // asm: 	BNZ	DO_CREDITS		;Yes, then go print them
    // asm: 	CMPI	1,R4			;No, then check partial credits on?
    // asm: 	BEQ	CHKPCREDITS		;Yes, then go print the partial credits
DO_CREDITS:
    // asm: 	LDI	1,R7
    // asm: 	LDI	@FCBI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@CREDITBUFFI,AR0
    // asm: 	LDI	@FCBI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@CREDITBUFFI,AR0
    // asm: 	LDI	@DBLSPCI,AR1
    // asm: 	CALL	STRCAT
CHKPCREDITS:
    // asm: 	READAUD	AUD_PCREDITS		;Are there any patial credits?
    // asm: 	LDI	R0,R2
    // asm: 	BZ	NO_PCREDITS		;No, then skip this
    // asm: 	CMPI	0,R4			;Is partial credits on?
    // asm: 	BEQ	NO_PCREDITS		;No, then skip this
    // asm: 	LDI	1,R7
    // asm: 	LDI	@PCBI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@CREDITBUFFI,AR0
    // asm: 	LDI	@PCBI,AR1
    // asm: 	CALL	STRCAT
    // asm:  	LDI	@CREDITBUFFI,AR0
    // asm: 	LDI	@NCBI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	CALL	GET_UNITS_PER_CREDIT
    // asm: 	LDI	R1,R2
    // asm: 	LDI	@SCI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@CREDITBUFFI,AR0
    // asm: 	LDI	@SCI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@CREDITBUFFI,AR0
    // asm: 	LDI	@SPCI,AR1
    // asm: 	CALL	STRCAT
NO_PCREDITS:
    // asm: 	LDI	@CREDITBUFFI,AR0
    // asm: 	LDI	@CWI,AR1
    // asm: 	READAUD	AUD_CREDITS		;DO WE HAVE ONLY 1 CREDIT?
    // asm: 	CMPI	1,R0
    // asm: 	LDILE	@CWSI,AR1		;ONLY HAVE ON CREDIT USE SINGULAR "CREDIT"
    // asm: 	CALL	STRCAT
    // 	;
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_CREDITS", 0, 0);
    UNIMPL();
}

void FLASH_TO_START(void)
{
    // asm: 	LDI	@ICF,R0
    // asm: 	BGT	NO_TOSTART
    // asm: 	SUBI	@NFRAMES,R0
    // asm: 	CMPI	-45,R0
    // asm: 	LDILT	1,R0
    // asm: 	STI	R0,@ICF
    // asm: 	CALL	TOSTART_STRING
    // asm: 	BU	PRINT_TOSTART
NO_TOSTART:
    // asm: 	LDI	@ICF,R0
    // asm: 	ADDI	@NFRAMES,R0
    // asm: 	CMPI	45,R0
    // asm: 	LDIGT	-1,R0
    // asm: 	STI	R0,@ICF
    // asm: 	CALL	GET_CREDITS_TO_START
    // asm: 	LDI	R1,R0
    // asm: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm: 	LDI	R1,R2
    // asm: 	CMPI	R0,R2
    // asm: 	BEQ	FLASH_TOSTARTX
    // asm: 	CALL	TOCONT_STRING
PRINT_TOSTART:
    // asm: 	LDI	@TOSTARTBUFFI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	CALL	FONT18REDDS
FLASH_TOSTARTX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_TO_START", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TOSTART_STRING(void)
{
    // asm: 	CLRI	R7
    // asm: 	STI	R7,@TOSTARTBUFFER	;STRING BUFFER
    // asm: 	CALL	GET_CREDITS_TO_START
    // asm: 	LDI	R1,R2
    // asm: 	LDI	1,R7
    // asm: 	LDI	@FCBI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@TOSTARTBUFFI,AR0
    // asm: 	LDI	@FCBI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@TOSTARTBUFFI,AR0
    // asm: 	LDI	@SPCI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@TOSTARTBUFFI,AR0
    // asm: 	LDI	@CWI,AR1
    // asm: 	CALL	GET_CREDITS_TO_START
    // asm: 	CMPI	1,R1
    // asm: 	LDIEQ	@CWSI,AR1		;ONLY HAVE ON CREDIT USE SINGULAR "CREDIT"
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@TOSTARTBUFFI,AR0
    // asm: 	LDI	@CTS_STRI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOSTART_STRING", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TOCONT_STRING(void)
{
    // asm: 	CLRI	R7
    // asm: 	STI	R7,@TOSTARTBUFFER	;STRING BUFFER
    // asm: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm: 	LDI	R1,R2
    // asm: 	LDI	1,R7
    // asm: 	LDI	@FCBI,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDI	@TOSTARTBUFFI,AR0
    // asm: 	LDI	@FCBI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@TOSTARTBUFFI,AR0
    // asm: 	LDI	@SPCI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@TOSTARTBUFFI,AR0
    // asm: 	LDI	@CWI,AR1
    // asm: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm: 	CMPI	1,R1
    // asm: 	LDIEQ	@CWSI,AR1		;ONLY HAVE ON CREDIT USE SINGULAR "CREDIT"
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@TOSTARTBUFFI,AR0
    // asm: 	LDI	@CTC_STRI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOCONT_STRING", 0, 0);
    UNIMPL();
}

void VOLUME_DISPLAY(void)
{
    // asm: 	LDI	@VOLUME_ACTIVE,R0
    // asm: 	RETSZ
    // asm: 	DEC	R0
    // asm: 	STI	R0,@VOLUME_ACTIVE
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MDIAG,R0
    // asm: 	RETSEQ
    // asm: 	READADJ	ADJ_VOLUME
    // asm: 	FLOAT	R0,R2
    // asm: 	MPYF	0.11764,R2		;30/255
    // asm: 	FIX	R2
    // asm: 	LDL	VOLUME_COUNT,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	180,R2
    // asm: 	FLOAT	310,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	FONT18REDDS
    // asm: 	LDL	VOLUME_TXT,AR2
    // asm: 	FLOAT	60,R2
    // asm: 	FLOAT	310,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	FONT18REDDS
    // asm: 	RETS
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
    // asm: 	PUSH	R4
    // asm: 	READADJ	ADJ_FREE_PLAY
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	INSERT_COINSXM
    // asm: 	CALL	PRINT_TOCONT
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	LDI	R0,R1
    // asm: 	READAUD	AUD_PCREDITS
    // asm: 	ADDI	R0,R1
    // asm: 	BNZ	ISMCC
    // asm: 	CALL	FLASH_INSERTCOINSM
    // asm: 	BU	INSERT_COINSXM
ISMCC:
    // asm: 	CALL	PRINT_CREDITS
    // asm: 	LDI	@CREDITBUFFI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	237,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
INSERT_COINSXM:
    // asm: 	POP	R4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_MORE_COINS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FLASH_INSERTCOINSM(void)
{
    // asm: 	LDI	@ICF,R0
    // asm: 	BGT	NO_INSERTCOINSM
    // asm: 	SUBI	@NFRAMES,R0
    // asm: 	CMPI	-30,R0
    // asm: 	LDILT	1,R0
    // asm: 	STI	R0,@ICF
    // asm: 	LDI	@INSERTCOINSI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	237,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	BU	FLASH_INSERTCOINSXM
NO_INSERTCOINSM:
    // asm: 	LDI	@ICF,R0
    // asm: 	ADDI	@NFRAMES,R0
    // asm: 	CMPI	30,R0
    // asm: 	LDIGT	-1,R0
    // asm: 	STI	R0,@ICF
FLASH_INSERTCOINSXM:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_INSERTCOINSM", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void PRINT_TOCONT(void)
{
    // asm: 	CALL	TOCONT_STRING
    // asm: 	LDI	@TOSTARTBUFFI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	200,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: TOCONTX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT_TOCONT", 0, 0);
    UNIMPL();
}

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
    // asm: 	LDP	@COUNTER_IDX
    // asm: 	LDI	@COUNTER_IDX,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@COUNTER_IDX
    // asm: 	SETDP
    // asm: 	CMPI	8,R0
    // asm: 	RETSLT
    // asm: 	LDP	@COUNTER_IDX
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@COUNTER_IDX
    // asm: 	LDP	@COUNTER_MODE
    // asm: 	LDI	@COUNTER_MODE,R0
    // asm: 	BZ	CLEARIT
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@COUNTER_MODE
    // asm: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm: 	LDI	@CMOS_WP_WORD_SHADOW,R0
    // asm: 	ANDN	0F00h,R0
    // asm: 	LDP	@COIN_COUNTER1
    // asm: 	LDI	@COIN_COUNTER1,R1
    // asm: 	DEC	R1
    // asm: 	BLT	NOINC1
    // asm: 	STI	R1,@COIN_COUNTER1
    // asm: 	OR	100h,R0
NOINC1:
    // asm: 	LDP	@COIN_COUNTER2
    // asm: 	LDI	@COIN_COUNTER2,R1
    // asm: 	DEC	R1
    // asm: 	BLT	NOINC2
    // asm: 	STI	R1,@COIN_COUNTER2
    // asm: 	OR	200h,R0
NOINC2:
    // asm: 	DINT
    // asm: 	LDP	@CMOS_WP_WORD
    // asm: 	STI	R0,@CMOS_WP_WORD
    // asm: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm: 	STI	R0,@CMOS_WP_WORD_SHADOW
    // asm: 	EINT
    // asm: 	SETDP
    // asm: 	RETS
CLEARIT:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@COUNTER_MODE
    // asm: 	DINT
    // asm: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm: 	LDI	@CMOS_WP_WORD_SHADOW,R0
    // asm: 	ANDN	0F00h,R0
    // asm: 	STI	R0,@CMOS_WP_WORD_SHADOW
    // asm: 	LDP	@CMOS_WP_WORD
    // asm: 	STI	R0,@CMOS_WP_WORD
    // asm: 	EINT
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN_COUNTER", 0, 0);
    UNIMPL();
}

void MOTION_VALID(void)
{
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	RETSNZ			;RETURN IF NON MOVING
    // asm: 	CALL	CHECK_MOTION_PRESENT
    // asm: 	RETSEQ			;RETURN IF ALL OK
    // asm: 	LDI	@MOTIONDIS,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	305,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_VALID", 0, 0);
    UNIMPL();
}

#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/sys.h"
#include "../include/vunit.h"
#include "../include/c30.h"
#include "../include/cmos.h"
#include "../include/macs.h"
#include "../include/globals.h"

/*
 * Source module: asm/CMOS.ASM
 */

// *----------------------------------------------------------------------------
// *CMOS ROUTINES
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *DECREMENT HIGH SCORE RESET COUNTER
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *
// *
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *AUDIT_WRITE
// *
// *PARAMETERS
// *	AR2	INDEX
// *	R2	VALUE
// *RETURNS
// *	R0	AUDIT VALUE
// *
#endif
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
#define DEFAULT_TABLE_SIZE 2
#define NUM_TABLES 14
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *Initialize the CMOS tables
// *
// *ALL TABLES DEFAULT TO THE ABOVE TABLE
// *
// *Related routines for management of CMOS stuff
// *
// *	INIT_HSTD_TABLES
// *	GET_TABLE_ADDR
// *	TABLE_ENTRY_READ
// *	TABLE_ENTRY_WRITE0
// *	TABLE_ENTRY_WRITE
// *	CHECK_RACE_TABLE
// *	INSERT_TABLE_ENTRY
// *
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------

void CMOS_ON_C(void)
{
    // asm: 	PUSH	DP
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	CMOS_WS,R1
    // asm: 	STI	R1,@CPU_WS
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_ON_C", 0, 0);
    UNIMPL();
}

void CMOS_OFF_C(void)
{
    // asm: 	PUSH	DP
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	SOFT_WS,R1
    // asm: 	STI	R1,@CPU_WS
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_OFF_C", 0, 0);
    UNIMPL();
}

void CMOS_WPON_C(void)
{
    // asm: 	PUSH	DP
    // asm: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm: 	LDI	@CMOS_WP_WORD_SHADOW,R1
    // asm: 	AND	0F00h,R1
    // asm: 	LDP	@CMOS_WP_WORD
    // asm: 	STI	R1,@CMOS_WP_WORD
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_WPON_C", 0, 0);
    UNIMPL();
}

void CMOS_WPOFF_C(void)
{
    // asm: 	PUSH	DP
    // asm: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm: 	LDI	@CMOS_WP_WORD_SHADOW,R1
    // asm: 	AND	0F00h,R1
    // asm: 	LDP	@CMOS_WP_WORD
    // asm: 	OR	CMOS_WP,R1
    // asm: 	STI	R1,@CMOS_WP_WORD
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_WPOFF_C", 0, 0);
    UNIMPL();
}

void VALIDATE_CMOS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *VALIDATE_CMOS
    // *
    // *THIS ROUTINE VERIFIES THAT THE CMOS WE ARE USING
    // *IS CORRECT.  IF NOT, THE VALUES ARE RESET TO THE DEFAULTS.
    // *CALL ON STARTUP
    // *
    // *
    // asm: 	READAUD	AUD_PCREDITS
    // asm: 	LDI	R0,R2
    // asm: 	CMPI	10,R0
    // asm: 	LDIGT	10,R2
    // asm: 	CMPI	0,R0
    // asm: 	LDILT	0,R2
    // asm: 	SETAUD	AUD_PCREDITS
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	LDI	R0,R2
    // asm: 	CMPI	30,R0
    // asm: 	LDIGT	30,R2
    // asm: 	CMPI	0,R0
    // asm: 	LDILT	0,R2
    // asm: 	SETAUD	AUD_CREDITS
    // asm: 	CALL	VALIDATE_HSTD_TABLES
    // asm: 	CALL	VERIFY_ADJUSTMENTS_ACCURACY
    // asm: 	CLRC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VALIDATE_CMOS", 0, 0);
    UNIMPL();
}

void RESET_BOOKKEEPING(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *RESET_BOOKKEEPING
    // *
    // *	RESET THE BOOKKEEPING CMOS PAGE (GENERATE THE NEW CHECKSUM)
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR4
    // 	;erase AUDITS
    // asm: 	LDI	(NUM_AUDITS-MAX_ADJUSTMENTS)-1,AR4
    // asm: 	LDI	@CMOSI,AR2
    // asm: 	ADDI	(MAX_ADJUSTMENTS+1)<<2,AR2
    // asm: 	CLRI	R2
RBLP:
    // asm: CALL	_wr_cw
    // asm: 	DBU	AR4,RBLP
    // asm: 	POP	AR4
    // asm: 	POP	AR2
    // asm: 	POP	R2
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_BOOKKEEPING", 0, 0);
    UNIMPL();
}

void _word(void)
{
    // asm: LOW:
_word:
    // asm: HIGH:
_word:
    // asm: DEFAULT:
    // ;	.word	:HIGH:<<8|:LOW:|(:DEFAULT:<<16)
    // asm: VERIFY_ADJUSTMENTS_ACCURACYTAB
    // asm: 	VADJTAB	0,81,0		;ADJ_COINMODE
    // asm: 	VADJTAB	0,255,0		;ADJ_GASMIN
    // asm: 	VADJTAB	0,255,255	;ADJ_GASMAX
    // asm: 	VADJTAB	0,255,0		;ADJ_STEERMIN
    // asm: 	VADJTAB	0,255,255	;ADJ_STEERMAX
    // asm: 	VADJTAB	0,255,128	;ADJ_STEERCENTER
    // asm: 	VADJTAB	0,255,0		;ADJ_BRAKEMIN
    // asm: 	VADJTAB	0,255,255	;ADJ_BRAKEMAX
    // asm: 	VADJTAB	0,255,200	;ADJ_VOLUME
    // asm: 	VADJTAB	0,100,1		;ADJ_COIN1_UNITS
    // asm: 	VADJTAB	0,100,1		;ADJ_COIN2_UNITS
    // asm: 	VADJTAB	0,100,1		;ADJ_COIN3_UNITS
    // asm: 	VADJTAB	0,100,1		;ADJ_COIN4_UNITS
    // asm: 	VADJTAB 0,100,1		;ADJ_UNITS_PER_CREDIT
    // asm: 	VADJTAB 0,100,0		;ADJ_BONUS_UNITS
    // asm: 	VADJTAB 0,100,0		;ADJ_UNITS_MIN
    // asm: 	VADJTAB 0,100,3		;ADJ_CREDITS_TO_START
    // asm: 	VADJTAB 0,100,3		;ADJ_CREDITS_TO_CONTINUE
    // asm: 	VADJTAB 0,1,1		;ADJ_SHOW_FRAC
    // asm: 	VADJTAB 0,127,1		;ADJ_COIN1_COUNTER
    // asm: 	VADJTAB 0,127,1		;ADJ_COIN2_COUNTER
    // asm: 	VADJTAB 0,127,1		;ADJ_COIN3_COUNTER
    // asm: 	VADJTAB 0,127,4		;ADJ_COIN4_COUNTER
    // asm: 	VADJTAB	0,1,1		;ADJ_STANDARD_PRICING
    // asm: 	VADJTAB	0,1,0		;ADJ_CUSTOM_PRICING
    // asm: 	VADJTAB	0,1,0		;ADJ_FREE_PLAY
    // asm: 	VADJTAB	0,1,1		;ADJ_FREEGAME
    // asm: 	VADJTAB	0,9,5		;ADJ_DIFFICULTY
    // asm: 	VADJTAB	0,6,3		;ADJ_TIME_TO_START
    // asm: 	VADJTAB	10,25,20	;ADJ_CHECKPOINT_BONUS
    // asm: 	VADJTAB	0,1,0		;ADJ_ATTRACT_MODE_SOUND
    // asm: 	VADJTAB	0,1,1		;ADJ_HIGH_SCORE_ENTRY
    // asm: 	VADJTAB	0,255,100	;ADJ_MIN_VOL_LEVEL
    // asm: 	VADJTAB	0,25000,4000	;ADJ_ACTUALHSTDRESET
    // asm: 	VADJTAB	0,1,0		;ADJ_MPHORKPM
    // asm: 	VADJTAB	0,1,1		;ADJ_ROADKILL
    // asm: 	VADJTAB	0,1,1		;ADJ_CLINTON
    // asm: 	VADJTAB	0,1,1		;ADJ_GIRLS
    // asm: 	VADJTAB	0,5,5		;ADJ_STEERING_SENSE
    // asm: 	VADJTAB	1000,25000,5000	;ADJ_HIGHSCORE_RESET (step 1000)
    // asm: 	VADJTAB	0,100,50	;ADJ_DIFF_LOCAL
    // asm: 	VADJTAB	0,1,0		;UNKNOWN
    // asm: 	VADJTAB	0,30,20		;ADJ_RAMP_PERCENTAGE
    // asm: 	VADJTAB	0,5000,0	;ADJ_RAMP_COUNT
    // asm: 	VADJTAB	0,1,1		;ADJ_ENTER_INITS
    // asm: 	VADJTAB	10,50,30	;ADJ_MAX_CREDITS
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", ".word", 0, 0);
    UNIMPL();
}

void VERIFY_ADJUSTMENTS_ACCURACY(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	CLRI	AR5
    // asm: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR6
VAALP:
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	AUDIT_READ	;R0 = ADJUSTMENT VALUE
    // asm: 	LDI	*AR6++,R1
    // asm: 	LDI	*AR6++,R2
    // asm: 	LDI	*AR6++,R3
    // asm: 	CMPI	R1,R0
    // asm: 	BLT	DORST
    // asm: 	CMPI	R2,R0
    // asm: 	BLE	NORST
DORST:
    // asm: LDI	R3,R2
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
NORST:
    // asm: INC	AR5
    // asm: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm: 	BLT	VAALP
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR2
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VERIFY_ADJUSTMENTS_ACCURACY", 0, 0);
    UNIMPL();
}

void RESET_ADJUSTMENTS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	CLRI	AR5
    // asm: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR6
VAALP2:
    // asm: 	LDI	*AR6++,R2
    // asm: 	LDI	*AR6++,R2
    // asm: 	LDI	*AR6++,R2		;RESET VALUE
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	AUDIT_WRITE
    // asm: 	INC	AR5
    // asm: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm: 	BLT	VAALP2
    // asm: 	CALL	GETCOIN_DEFAULT
    // ;	LDP	@DIPSW
    // ;	LDI	@DIPSW,R2
    // ;	SETDP
    // ;	RS	24,R2
    // ;	NOT	R2
    // ;	LDI	0,R0
    // ;	LDI	7,RC
    // ;	RPTB	INVBLP
    // ;	LSH	-1,R2
    // ;INVBLP	ROLC	R0
    // ;	AND	07Fh,R0
    // ;	LDI	R0,R2
    // ;Now check to see if the dipswitch setting is a valid coinmode
    // asm: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR2
    // asm: 	LDI	ADJ_COINMODE,R0
    // asm: 	MPYI	3,R0
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*+AR2,R0
    // asm: 	CMPI	R0,R2
    // asm: 	LDIGT	*+AR2(2),R2
    // asm: 	LDI	ADJ_COINMODE,AR2
    // asm: 	CALL	ADJUSTMENT_WRITE
    // asm: 	CALL	CHECKSUMGEN_ADJ
    // asm: 	LDI	R0,R2
    // asm: 	SETAUD	ADJ_CHECKSUM
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR2
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_ADJUSTMENTS", 0, 0);
    UNIMPL();
}

void CHECKSUMGEN_ADJ(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GENERATE CHECKSUM FOR THE ADJUSTMENTS
    // *
    // *RETURNS
    // *	R0	ADJUSTMENT CHECKSUM
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	CLRI	AR5
    // asm: 	CLRI	R1
VAALP3:
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	AUDIT_READ	;R0 = ADJUSTMENT VALUE
    // asm: 	ADDI	R0,R1
    // asm: 	INC	AR5
    // asm: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm: 	BLT	VAALP3
    // asm: 	LDI	R1,R0
    // asm: 	POP	AR5
    // asm: 	POP	AR2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKSUMGEN_ADJ", 0, 0);
    UNIMPL();
}

void AUDIT_INC(void)
{
    // *----------------------------------------------------------------------------
    // ;*----------------------------------------------------------------------------
    // ;*GENERATE CHECKSUM FOR THE AUDITS
    // ;*
    // ;*RETURNS
    // ;*	R0	ADJUSTMENT CHECKSUM
    // ;*
    // ;CHECKSUMGEN_AUD:
    // ;	PUSH	R0
    // ;	PUSH	R1
    // ;	PUSH	AR2
    // ;	PUSH	AR5
    // ;
    // ;	LDI	AUD_COIN1,AR5
    // ;	CLRI	R1
    // ;VAALP4	LDI	AR5,AR2
    // ;	CALL	AUDIT_READ	;R0 = ADJUSTMENT VALUE
    // ;	ADDI	R0,R1
    // ;
    // ;	INC	AR5
    // ;	CMPI	NUM_AUDITS-MAX_ADJUSTMENTS,AR5
    // ;	BLT	VAALP4
    // ;
    // ;	LDI	R1,R0
    // ;	POP	AR5
    // ;	POP	AR2
    // ;	POP	R1
    // ;	POP	R0
    // ;	RETS
    // ;*----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	audit_inc(int index)
    // *
    // *PARAMETERS
    // *	AR2	INDEX
    // *RETURNS
    // *	R0	AUDIT VALUE
    // *
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CALL	_rd_cw
    // asm: 	NOP	*AR2--(4)
    // asm: 	ADDI	1,R0
    // asm: 	LDI	R0,R2
    // asm: 	CALL	_wr_cw
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_INC", 0, 0);
    UNIMPL();
}

void AUDIT_ADD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	audit_add(int index)
    // *
    // *PARAMETERS
    // *	AR2	INDEX
    // *	R2	VALUE
    // *RETURNS
    // *	R0	AUDIT VALUE
    // *
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CALL	_rd_cw
    // asm: 	NOP	*AR2--(4)
    // asm: 	ADDI	R0,R2
    // asm: 	CALL	_wr_cw
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_ADD", 0, 0);
    UNIMPL();
}

void ADJUSTMENT_READ(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *AUDIT_READ
    // *
    // *PARAMETERS
    // *	AR2	INDEX
    // *RETURNS
    // *	R0	AUDIT VALUE
    // *
    /* no executable asm lines detected */
    UNIMPL();
}

void AUDIT_READ(void)
{
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CALL	_rd_cw
    // asm: 	NOP	*AR2--(4)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_READ", 0, 0);
    UNIMPL();
}

void ADJUSTMENT_WRITE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // *PARAMETERS
    // *	AR2	INDEX
    // *	R2	VALUE
    // *RETURNS
    // *	R0	AUDIT VALUE
    // asm: 	CALL	AUDIT_WRITE_ADJ
    // asm: 	CALL	CHECKSUMGEN_ADJ
    // asm: 	LDI	R0,R2
    // asm: 	LDI	ADJ_CHECKSUM,AR2
    // asm: 	CALL	AUDIT_WRITE_ADJ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADJUSTMENT_WRITE", 0, 0);
    UNIMPL();
}

void AUDIT_WRITE_ADJ(void)
{
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CALL	_wr_cw
    // asm: 	NOP	*AR2--(4)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_WRITE_ADJ", 0, 0);
    UNIMPL();
}

void _rd_cw(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *NOTE!!!:	MODIFICATIONS TO _rd_cw,_wr_cw,_rd_cwR,_wr_cwR
    // *NOTE!!!:	MODIFICATIONS TO _rd_cw,_wr_cw,_rd_cwR,_wr_cwR
    // *NOTE!!!:	MODIFICATIONS TO _rd_cw,_wr_cw,_rd_cwR,_wr_cwR
    // *
    // *
    // *THE REGISTER RULES ARE:
    // *
    // *	AR2 is incremented to next entry in CMOS
    // *	R0  holds RETURN VALUE
    // *
    // *	ALL OTHER REGS ARE PRESERVED!!!
    // *	ALL OTHER REGS ARE PRESERVED!!!
    // *	ALL OTHER REGS ARE PRESERVED!!!
    // *
    // *
    // *	These are the basic pieces for CMOS I/O.  No other routines
    // *	should directly access CMOS.
    // *
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *int	rd_cw(int addr);
    // *READ CMOS WORD
    // *
    // *PARAMETERS
    // *	AR2	ADDR
    // *RETURNS
    // *	R0	WORD
    // *	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
    // asm: 	PUSH	R1
    // asm: 	CMOS_ON
    // asm: 	NOP			;DELAY FOR TIMING...
    // asm: 	LDI	*AR2++,R0
    // asm: 	RS	24,R0
    // asm: 	LS	8,R0
    // asm: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm: 	OR	R1,R0
    // asm: 	LS	8,R0
    // asm: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm: 	OR	R1,R0
    // asm: 	LS	8,R0
    // asm: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm: 	OR	R1,R0
    // asm: 	CMOS_OFF
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rd_cw", 0, 0);
    UNIMPL();
}

void _wr_cw(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	wr_cw(int addr, int value);
    // *WRITE CMOS WORD
    // *
    // *PARAMETERS
    // *	AR2	LOC
    // *	R2	WORD
    // *RETURNS
    // *	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	CMOS_ON
    // asm: 	CMOS_WP_OFF
    // asm: 	NOP			;DELAY FOR TIMING...
    // asm: 	STI	R2,*AR2++
    // asm: 	LS	8,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LS	8,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LS	8,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	CMOS_WP_ON
    // asm: 	CMOS_OFF
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_wr_cw", 0, 0);
    UNIMPL();
}

void _rd_cwR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *int	rd_cmR(int addr);
    // *READ CMOS WORD REVERSED  (MOTOROLA STYLE)
    // *
    // *PARAMETERS
    // *	AR2	LOC
    // *RETURNS
    // *	R0	WORD
    // *	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
    // *
    // asm: 	PUSH	R1
    // asm: 	CMOS_ON
    // asm: 	NOP			;DELAY FOR TIMING...
    // asm: 	LDI	*AR2++,R0
    // asm: 	RS	24,R0
    // asm: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm: 	LS	8,R1
    // asm: 	OR	R1,R0
    // asm: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm: 	LS	16,R1
    // asm: 	OR	R1,R0
    // asm: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm: 	LS	24,R1
    // asm: 	OR	R1,R0
    // asm: 	CMOS_OFF
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rd_cwR", 0, 0);
    UNIMPL();
}

void _wr_cwR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	wr_cmR(int addr, int value);
    // *WRITE CMOS WORD REVERSED
    // *
    // *PARAMETERS
    // *	AR2	LOC
    // *	R2	WORD
    // *RETURNS
    // *	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	LDI	R2,R3
    // asm: 	LS	24,R3
    // asm: 	CMOS_ON
    // asm: 	CMOS_WP_OFF
    // asm: 	NOP			;DELAY FOR TIMING...
    // asm: 	STI	R3,*AR2++
    // asm: 	LDI	R2,R3
    // asm: 	RS	8,R3
    // asm: 	LS	24,R3
    // asm: 	STI	R3,*AR2++
    // asm: 	LDI	R2,R3
    // asm: 	RS	16,R3
    // asm: 	LS	24,R3
    // asm: 	STI	R3,*AR2++
    // asm: 	RS	24,R2
    // asm: 	LS	24,R3
    // asm: 	STI	R2,*AR2++
    // asm: 	CMOS_WP_ON
    // asm: 	CMOS_OFF
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_wr_cwR", 0, 0);
    UNIMPL();
}

void _string(void)
{
    // asm: I1:,:I2:,:I3:,:POSTN:
    TRACE_EVENT(&g_crusn_machine->trace, "function", ".string", 0, 0);
    UNIMPL();
}

void DEFAULT_TABLE(void)
{
    // asm: 	TABLEENT	'T','V','G',2,01,3
    // asm: 	TABLEENT	'E','L','P',2,02,3
    // asm: 	TABLEENT	'E','P','J',2,03,3
    // asm: 	TABLEENT	'M','L',' ',2,04,3
    // asm: 	TABLEENT	'G','W','S',2,06,3
    // asm: 	TABLEENT	'S','X','C',2,08,3
    // asm: 	TABLEENT	'T','E','D',2,10,3
    // asm: 	TABLEENT	'M','E','Y',2,12,3
    // asm: 	TABLEENT	'M','M','V',2,14,3
    // asm: 	TABLEENT	'B','D','P',2,16,3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEFAULT_TABLE", 0, 0);
    UNIMPL();
}

void DEFAULT_TABLE_TOTAL(void)
{
    // asm: 	TABLEENT	'T','V','G',28,00,3
    // asm: 	TABLEENT	'E','L','P',28,10,3
    // asm: 	TABLEENT	'E','P','J',28,20,3
    // asm: 	TABLEENT	'M','L',' ',28,30,3
    // asm: 	TABLEENT	'G','W','S',28,40,3
    // asm: 	TABLEENT	'S','X','C',28,50,3
    // asm: 	TABLEENT	'T','E','D',30,00,3
    // asm: 	TABLEENT	'M','E','Y',31,10,3
    // asm: 	TABLEENT	'M','M','V',32,20,3
    // asm: 	TABLEENT	'B','D','P',34,30,3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEFAULT_TABLE_TOTAL", 0, 0);
    UNIMPL();
}

void INIT_LASTHS_TABLE(void)
{
    // *INIT_LASTHS_TABLE
    // *CHECK_LASTHS
    // *----------------------------------------------------------------------------
    // *Initialize the LAST HS SET CMOS table
    // *
    // *This table is to keep track of the High Score entries set (if any at all)
    // *by the last player.
    // *There is one byte per Race location.
    // * -1 = not set by last player. 0-9 = Position in this table for that race
    // asm: 	LDI	NUM_TABLES+1,R6
    // asm: 	LDI	0,R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	LDI	-1,R2
    // asm: 	LDI	14,AR5
    // asm: ILT_LP
    // asm: 	CALL	_wr_cw
    // asm: 	DBU	AR5,ILT_LP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LASTHS_TABLE", 0, 0);
    UNIMPL();
}

void UPDATE_LASTHS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *	R6 = RACE NUMBER
    // *	R7 = ENTRY NUMBER
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSH	R2
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	LDI	R6,R0
    // asm: 	LDI	NUM_TABLES+1,R6
    // asm: 	LDI	0,R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	MPYI	4,R0		;4 bytes per word
    // asm: 	ADDI	R0,AR2
    // asm: 	POP	R7
    // asm: 	LDI	R7,R2
    // asm: 	CALL	_wr_cw
    // asm: 	POP	R6
    // asm: 	POP	R2
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UPDATE_LASTHS", 0, 0);
    UNIMPL();
}

void CHECK_LASTHS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *	R1 = RACE NUMBER
    // *
    // *Returns R0 = position in table of the last player that played. -1 = not in table
    // asm: 	LDI	NUM_TABLES+1,R6
    // asm: 	LDI	0,R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	MPYI	4,R1		;4 bytes per word
    // asm: 	ADDI	R1,AR2
    // asm: 	CALL	_rd_cw
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_LASTHS", 0, 0);
    UNIMPL();
}

void INIT_HSTD_TABLES(void)
{
    // asm: 	CLRI	R6
    // asm: 	LDI	15-1,AR5
    // asm: ITL1
    // asm: 	CLRI	R7
    // asm: 	LDI	10-1,AR4
    // asm: 	LDL	DEFAULT_TABLE,AR1
    // asm: 	CMPI	14,R6			;the 15th entry is the totals for the entire country
    // asm: 	LDIEQ	@DEFAULT_TABLE_TOTALI,AR1
    // asm: ITL2
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	CALL	TABLE_ENTRY_WRITE
    // asm: 	ADDI	DEFAULT_TABLE_SIZE,AR1
    // asm: 	INC	R7
    // asm: 	DBU	AR4,ITL2
    // asm: 	INC	R6
    // asm: 	DBU	AR5,ITL1
    // asm: 	CALL	INIT_LASTHS_TABLE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_HSTD_TABLES", 0, 0);
    UNIMPL();
}

void VALIDATE_HSTD_TABLES(void)
{
    // asm: 	CLRI	R6
    // asm: 	LDI	10-1,AR5
    // asm: ITL1A
    // asm: 	CLRI	R7
    // asm: 	LDI	10-1,AR4
    // asm: 	LDL	DEFAULT_TABLE,AR1
    // asm: ITL2A
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	CALL	TABLE_ENTRY_READ
    // asm: 	CMPI	' ',R1
    // asm: 	BEQ	J1
    // asm: 	CMPI	'0',R1
    // asm: 	BLT	RESETALL
    // asm: 	CMPI	'Z',R1
    // asm: 	BGT	RESETALL
J1:
    // asm: CMPI	' ',R2
    // asm: 	BEQ	J2
    // asm: 	CMPI	'0',R2
    // asm: 	BLT	RESETALL
    // asm: 	CMPI	'Z',R2
    // asm: 	BGT	RESETALL
J2:
    // asm: CMPI	' ',R3
    // asm: 	BEQ	J3
    // asm: 	CMPI	'0',R3
    // asm: 	BLT	RESETALL
    // asm: 	CMPI	'Z',R3
    // asm: 	BGT	RESETALL
J3:
    // asm: CMPI	0,R4
    // asm: 	BLT	RESETALL
    // asm: 	CMPI	10,R4
    // asm: 	BGT	RESETALL
    // asm: 	CMPI	4*60*60,R0
    // asm: 	BGT	RESETALL
    // asm: 	CMPI	0,R0
    // asm: 	BLT	RESETALL
    // asm: 	ADDI	DEFAULT_TABLE_SIZE,AR1
    // asm: 	INC	R7
    // asm: 	DBU	AR4,ITL2A
    // asm: 	INC	R6
    // asm: 	DBU	AR5,ITL1A
    // asm: 	RETS
RESETALL:
    // asm: 	CALL	INIT_HSTD_TABLES
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VALIDATE_HSTD_TABLES", 0, 0);
    UNIMPL();
}

void GET_TABLE_ADDR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	R6	RACE NUMBER
    // *	R7	ENTRY NUMBER
    // *
    // *RETURNS
    // *	AR2	ADDRESS
    // *
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	MPYI	RACE_TABLE_SIZE,R6
    // asm: 	MPYI	TE_SIZE,R7
    // asm: 	ADDI	R6,R7,AR2
    // asm: 	ADDI	MAX_AUDITS*4,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	POP	R7
    // asm: 	POP	R6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TABLE_ADDR", 0, 0);
    UNIMPL();
}

void TABLE_ENTRY_WRITE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *WRITE ENTRY IN TABLE
    // *
    // *PARAMETER
    // *	AR2	ADDRESS OF ENTRY
    // *	AR1	TABLE ENTRY
    // *RETURNS
    // *	(AR2 INCREMENTED TO THE NEXT ENTRY)
    // *
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR1
    // asm: 	LDI	*AR1++,R0
    // asm: 	LDI	*AR1++,R1
    // asm: 	LDI	R1,R2
    // asm: 	RS	8,R2
    // asm: 	LDI	R1,R3
    // asm: 	RS	16,R3
    // asm: 	LDI	R1,R4
    // asm: 	RS	24,R4
    // asm: 	POP	AR1
    // asm: 	BU	TEWL1
    // *
    // *PARAMETERS
    // *	R0	TIME CODE
    // *	R1	Initial 1
    // *	R2	Initial 2
    // *	R3	Initial 3
    // *	R4	POSITION
    // *	AR2	ADDR
    // *
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_WRITE", 0, 0);
    UNIMPL();
}

void TABLE_ENTRY_WRITE0(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
TEWL1:
    // asm: 	PUSH	R2
    // asm: 	LDI	R0,R2
    // asm: 	CALL	_wr_cw
    // asm: 	POP	R2
    // asm: 	PUSH	R1
    // asm: 	CMOS_ON
    // asm: 	CMOS_WP_OFF
    // ;	NOP			;DELAY FOR TIMING...
    // asm: 	POP	R1
    // asm: 	LS	24,R1
    // asm: 	STI	R1,*AR2++
    // asm: 	LS	24,R2
    // asm: 	STI	R2,*AR2++
    // asm: 	LS	24,R3
    // asm: 	STI	R3,*AR2++
    // asm: 	LS	24,R4
    // asm: 	STI	R4,*AR2++
    // asm: 	CMOS_WP_ON
    // asm: 	CMOS_OFF
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_WRITE0", 0, 0);
    UNIMPL();
}

void TABLE_ENTRY_READ(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *READ A TABLE ENTRY
    // *
    // *
    // *PARAMETERS
    // *	AR2	ADDR
    // *RETURNS
    // *	R0	TIME CODE
    // *	R1	Initial 1
    // *	R2	Initial 2
    // *	R3	Initial 3
    // *	R4	POSITION
    // *	(AR2 INCREMENTED TO THE NEXT ENTRY)
    // *
    // asm: 	CALL	_rd_cw
    // asm: 	PUSH	R0
    // asm: 	CMOS_ON
    // asm: 	CMOS_WP_OFF
    // asm: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm: 	LDI	*AR2++,R2
    // asm: 	RS	24,R2
    // asm: 	LDI	*AR2++,R3
    // asm: 	RS	24,R3
    // asm: 	LDI	*AR2++,R4
    // asm: 	RS	24,R4
    // asm: 	PUSH	R1
    // asm: 	CMOS_WP_ON	;This trashes register R1
    // asm: 	CMOS_OFF
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_READ", 0, 0);
    UNIMPL();
}

void CHECK_RACE_TABLE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CHECK_TABLE
    // *
    // *THIS ROUTINE CHECKS THE SPECIFIED TABLE TO SEE WHETHER OR NOT
    // *THE PLAYER CAN ENTER INTO THE TABLE
    // *
    // *PARAMETERS
    // *	R0	TIME CODE (TO CHECK)
    // *	R1	RACE INDEX
    // *RETURNS
    // *	R0	{-1}     = NO, MAY NOT ENTER
    // *	R0	{0...9}  = INDEX TO ENTER TABLE
    // *
    // asm: 	PUSH	R0
    // asm: 	LDI	R1,R6
    // asm: 	CLRI	R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	POP	R6
    // asm: 	CLRI	R5		;RETURN INDEX
NXTENT:
    // asm: CALL	TABLE_ENTRY_READ
    // asm: 	CMPI	R0,R6
    // asm: 	BLT	INSERT_HERE
    // asm: 	INC	R5
    // asm: 	CMPI	10,R5
    // asm: 	BLT	NXTENT
    // asm: 	LDI	-1,R0
    // asm: 	RETS
INSERT_HERE:
    // asm: 	LDI	R5,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_RACE_TABLE", 0, 0);
    UNIMPL();
}

void INSERT_TABLE_ENTRY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *PARAMETERS
    // *	R0	TIME CODE
    // *	R1	Initial 1
    // *	R2	Initial 2
    // *	R3	Initial 3
    // *	R4	POSITION
    // *	R5	INDEX #
    // *	R6	RACE #
    // *
    // *
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	LDI	NUM_ENTRIES_PER_RACE-2,R7	;Start at position 8
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	CMPI	9,R5
    // asm: 	BGE	ITLX
ITEL:
    // asm: CALL	TABLE_ENTRY_READ
    // asm: 	CALL	TABLE_ENTRY_WRITE0	;This uses the registers as input instead of *AR1
    // asm: 	NOP	*AR2--(TE_SIZE*3)
    // asm: 	DEC	R7
    // asm: 	CMPI	R5,R7
    // asm: 	BGE	ITEL				;Copy down position including where inserting
ITLX:
    // asm: POP	R7
    // asm: 	CALL	GET_TABLE_ADDR			;Get the ADDR of the insert point
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	CALL	TABLE_ENTRY_WRITE0	;This uses the registers as input instead of *AR1
    // asm: 	CALL	UPDATE_LASTHS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_TABLE_ENTRY", 0, 0);
    UNIMPL();
}

#include "../core/cpu.h"
#include "../core/machine.h"
#include "sys.h"
#include "vunit.h"
#include "c30.h"
#include "cmos.h"
#include "macs.h"
#include "globals.h"

/*
 * Source module: asm/CMOS.ASM
 */

void HSTDEC(void);
void CMOS_ON_C(void);
void CMOS_OFF_C(void);
void CMOS_WPON_C(void);
void CMOS_WPOFF_C(void);
void VALIDATE_CMOS(void);
void RESET_BOOKKEEPING(void);
static void VERIFY_ADJUSTMENTS_ACCURACY(void);
void RESET_ADJUSTMENTS(void);
static void CHECKSUMGEN_ADJ(void);
void AUDIT_INC(void);
void AUDIT_ADD(void);
#define ADJUSTMENT_READ AUDIT_READ
void AUDIT_READ(void);
void ADJUSTMENT_WRITE(void);
void AUDIT_WRITE(void);
static void AUDIT_WRITE_ADJ(void);
void _rd_cw(void);
void _wr_cw(void);
void _rd_cwR(void);
void _wr_cwR(void);
void INIT_LASTHS_TABLE(void);
static void UPDATE_LASTHS(void);
void CHECK_LASTHS(void);
void INIT_HSTD_TABLES(void);
void VALIDATE_HSTD_TABLES(void);
static void RESETALL(void);
void GET_TABLE_ADDR(void);
static void TABLE_ENTRY_WRITE(void);
static void TABLE_ENTRY_WRITE0(void);
void TABLE_ENTRY_READ(void);
void CHECK_RACE_TABLE(void);
void INSERT_TABLE_ENTRY(void);

#define ADJUSTMENT_READ AUDIT_READ
#define CMOSI CMOS
#define DEFAULT_TABLE_TOTALI DEFAULT_TABLE_TOTAL

void GETCOIN_DEFAULT(void);

static int DEFAULT_TABLE_TOTAL;

/*
*----------------------------------------------------------------------------
*CMOS ROUTINES
*
*COPYRIGHT (C) 1994  BY TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*DECREMENT HIGH SCORE RESET COUNTER
*
*/
void HSTDEC(void)
{
    // asm 00009962: 	READAUD	ADJ_ACTUALHSTDRESET
    // asm 00009964: 	SUBI	1,R0
    // asm 00009965: 	LDILT	0,R0
    // asm 00009966: 	LDI	R0,R2
    // asm 00009967: 	SETADJ	ADJ_ACTUALHSTDRESET
    // asm 00009969: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HSTDEC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void CMOS_ON_C(void)
{
    // asm 0000996A: 	PUSH	DP
    // asm 0000996B: 	LDP	@CPU_WS
    // asm 0000996C: 	LDI	CMOS_WS,R1
    // asm 0000996D: 	STI	R1,@CPU_WS
    // asm 0000996E: 	POP	DP
    // asm 0000996F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_ON_C", 0, 0);
    UNIMPL();
}

void CMOS_OFF_C(void)
{
    // asm 00009970: 	PUSH	DP
    // asm 00009971: 	LDP	@CPU_WS
    // asm 00009972: 	LDI	SOFT_WS,R1
    // asm 00009973: 	STI	R1,@CPU_WS
    // asm 00009974: 	POP	DP
    // asm 00009975: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_OFF_C", 0, 0);
    UNIMPL();
}

void CMOS_WPON_C(void)
{
    // asm 00009976: 	PUSH	DP
    // asm 00009977: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm 00009978: 	LDI	@CMOS_WP_WORD_SHADOW,R1
    // asm 00009979: 	AND	0F00h,R1
    // asm 0000997A: 	LDP	@CMOS_WP_WORD
    // asm 0000997B: 	STI	R1,@CMOS_WP_WORD
    // asm 0000997C: 	POP	DP
    // asm 0000997D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_WPON_C", 0, 0);
    UNIMPL();
}

void CMOS_WPOFF_C(void)
{
    // asm 0000997E: 	PUSH	DP
    // asm 0000997F: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm 00009980: 	LDI	@CMOS_WP_WORD_SHADOW,R1
    // asm 00009981: 	AND	0F00h,R1
    // asm 00009982: 	LDP	@CMOS_WP_WORD
    // asm 00009983: 	OR	CMOS_WP,R1
    // asm 00009984: 	STI	R1,@CMOS_WP_WORD
    // asm 00009985: 	POP	DP
    // asm 00009986: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_WPOFF_C", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*VALIDATE_CMOS
*
*THIS ROUTINE VERIFIES THAT THE CMOS WE ARE USING
*IS CORRECT.  IF NOT, THE VALUES ARE RESET TO THE DEFAULTS.
*CALL ON STARTUP
*
*
*/
void VALIDATE_CMOS(void)
{
    // asm 00009987: 	READAUD	AUD_PCREDITS
    // asm 00009989: 	LDI	R0,R2
    // asm 0000998A: 	CMPI	10,R0
    // asm 0000998B: 	LDIGT	10,R2
    // asm 0000998C: 	CMPI	0,R0
    // asm 0000998D: 	LDILT	0,R2
    // asm 0000998E: 	SETAUD	AUD_PCREDITS
    // asm 00009990: 	READAUD	AUD_CREDITS
    // asm 00009992: 	LDI	R0,R2
    // asm 00009993: 	CMPI	30,R0
    // asm 00009994: 	LDIGT	30,R2
    // asm 00009995: 	CMPI	0,R0
    // asm 00009996: 	LDILT	0,R2
    // asm 00009997: 	SETAUD	AUD_CREDITS
    // asm 00009999: 	CALL	VALIDATE_HSTD_TABLES
    // asm 0000999A: 	CALL	VERIFY_ADJUSTMENTS_ACCURACY
    // asm 0000999B: 	CLRC
    // asm 0000999C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VALIDATE_CMOS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*RESET_BOOKKEEPING
*
*	RESET THE BOOKKEEPING CMOS PAGE (GENERATE THE NEW CHECKSUM)
*
*/
void RESET_BOOKKEEPING(void)
{
    // asm 0000999D: 	PUSH	R0
    // asm 0000999E: 	PUSH	R2
    // asm 0000999F: 	PUSH	AR2
    // asm 000099A0: 	PUSH	AR4
    // 	;erase AUDITS
    // asm 000099A1: 	LDI	(NUM_AUDITS-MAX_ADJUSTMENTS)-1,AR4
    // asm 000099A2: 	LDI	@CMOSI,AR2
    // asm 000099A3: 	ADDI	(MAX_ADJUSTMENTS+1)<<2,AR2
    // asm 000099A4: 	CLRI	R2
RBLP:
    // asm 000099A5: CALL	_wr_cw
    // asm 000099A6: 	DBU	AR4,RBLP
    // asm 000099A7: 	POP	AR4
    // asm 000099A8: 	POP	AR2
    // asm 000099A9: 	POP	R2
    // asm 000099AA: 	POP	R0
    // asm 000099AB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_BOOKKEEPING", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: VERIFY_ADJUSTMENTS_ACCURACYTAB */
/* asm: VADJTAB	0,81,0		;ADJ_COINMODE */
/* asm: VADJTAB	0,255,0		;ADJ_GASMIN */
/* asm: VADJTAB	0,255,255	;ADJ_GASMAX */
/* asm: VADJTAB	0,255,0		;ADJ_STEERMIN */
/* asm: VADJTAB	0,255,255	;ADJ_STEERMAX */
/* asm: VADJTAB	0,255,128	;ADJ_STEERCENTER */
/* asm: VADJTAB	0,255,0		;ADJ_BRAKEMIN */
/* asm: VADJTAB	0,255,255	;ADJ_BRAKEMAX */
/* asm: VADJTAB	0,255,200	;ADJ_VOLUME */
/* asm: VADJTAB	0,100,1		;ADJ_COIN1_UNITS */
/* asm: VADJTAB	0,100,1		;ADJ_COIN2_UNITS */
/* asm: VADJTAB	0,100,1		;ADJ_COIN3_UNITS */
/* asm: VADJTAB	0,100,1		;ADJ_COIN4_UNITS */
/* asm: VADJTAB 0,100,1		;ADJ_UNITS_PER_CREDIT */
/* asm: VADJTAB 0,100,0		;ADJ_BONUS_UNITS */
/* asm: VADJTAB 0,100,0		;ADJ_UNITS_MIN */
/* asm: VADJTAB 0,100,3		;ADJ_CREDITS_TO_START */
/* asm: VADJTAB 0,100,3		;ADJ_CREDITS_TO_CONTINUE */
/* asm: VADJTAB 0,1,1		;ADJ_SHOW_FRAC */
/* asm: VADJTAB 0,127,1		;ADJ_COIN1_COUNTER */
/* asm: VADJTAB 0,127,1		;ADJ_COIN2_COUNTER */
/* asm: VADJTAB 0,127,1		;ADJ_COIN3_COUNTER */
/* asm: VADJTAB 0,127,4		;ADJ_COIN4_COUNTER */
/* asm: VADJTAB	0,1,1		;ADJ_STANDARD_PRICING */
/* asm: VADJTAB	0,1,0		;ADJ_CUSTOM_PRICING */
/* asm: VADJTAB	0,1,0		;ADJ_FREE_PLAY */
/* asm: VADJTAB	0,1,1		;ADJ_FREEGAME */
/* asm: VADJTAB	0,9,5		;ADJ_DIFFICULTY */
/* asm: VADJTAB	0,6,3		;ADJ_TIME_TO_START */
/* asm: VADJTAB	10,25,20	;ADJ_CHECKPOINT_BONUS */
/* asm: VADJTAB	0,1,0		;ADJ_ATTRACT_MODE_SOUND */
/* asm: VADJTAB	0,1,1		;ADJ_HIGH_SCORE_ENTRY */
/* asm: VADJTAB	0,255,100	;ADJ_MIN_VOL_LEVEL */
/* asm: VADJTAB	0,25000,4000	;ADJ_ACTUALHSTDRESET */
/* asm: VADJTAB	0,1,0		;ADJ_MPHORKPM */
/* asm: VADJTAB	0,1,1		;ADJ_ROADKILL */
/* asm: VADJTAB	0,1,1		;ADJ_CLINTON */
/* asm: VADJTAB	0,1,1		;ADJ_GIRLS */
/* asm: VADJTAB	0,5,5		;ADJ_STEERING_SENSE */
/* asm: VADJTAB	1000,25000,5000	;ADJ_HIGHSCORE_RESET (step 1000) */
/* asm: VADJTAB	0,100,50	;ADJ_DIFF_LOCAL */
/* asm: VADJTAB	0,1,0		;UNKNOWN */
/* asm: VADJTAB	0,30,20		;ADJ_RAMP_PERCENTAGE */
/* asm: VADJTAB	0,5000,0	;ADJ_RAMP_COUNT */
/* asm: VADJTAB	0,1,1		;ADJ_ENTER_INITS */
/* asm: VADJTAB	10,50,30	;ADJ_MAX_CREDITS */
static int VERIFY_ADJUSTMENTS_ACCURACYTAB;

// *
static void VERIFY_ADJUSTMENTS_ACCURACY(void)
{
    // asm 000099AC: 	PUSH	R0
    // asm 000099AD: 	PUSH	R1
    // asm 000099AE: 	PUSH	R2
    // asm 000099AF: 	PUSH	R3
    // asm 000099B0: 	PUSH	R4
    // asm 000099B1: 	PUSH	AR2
    // asm 000099B2: 	PUSH	AR5
    // asm 000099B3: 	PUSH	AR6
    // asm 000099B4: 	CLRI	AR5
    // asm 000099B5: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR6
VAALP:
    // asm 000099B6: 	LDI	AR5,AR2
    // asm 000099B7: 	CALL	AUDIT_READ	;R0 = ADJUSTMENT VALUE
    // asm 000099B8: 	LDI	*AR6++,R1
    // asm 000099B9: 	LDI	*AR6++,R2
    // asm 000099BA: 	LDI	*AR6++,R3
    // asm 000099BB: 	CMPI	R1,R0
    // asm 000099BC: 	BLT	DORST
    // asm 000099BD: 	CMPI	R2,R0
    // asm 000099BE: 	BLE	NORST
DORST:
    // asm 000099BF: LDI	R3,R2
    // asm 000099C0: 	LDI	AR5,AR2
    // asm 000099C1: 	CALL	ADJUSTMENT_WRITE
NORST:
    // asm 000099C2: INC	AR5
    // asm 000099C3: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm 000099C4: 	BLT	VAALP
    // asm 000099C5: 	POP	AR6
    // asm 000099C6: 	POP	AR5
    // asm 000099C7: 	POP	AR2
    // asm 000099C8: 	POP	R4
    // asm 000099C9: 	POP	R3
    // asm 000099CA: 	POP	R2
    // asm 000099CB: 	POP	R1
    // asm 000099CC: 	POP	R0
    // asm 000099CD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VERIFY_ADJUSTMENTS_ACCURACY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void RESET_ADJUSTMENTS(void)
{
    // asm 000099CE: 	PUSH	R0
    // asm 000099CF: 	PUSH	R1
    // asm 000099D0: 	PUSH	R2
    // asm 000099D1: 	PUSH	R3
    // asm 000099D2: 	PUSH	AR2
    // asm 000099D3: 	PUSH	AR5
    // asm 000099D4: 	PUSH	AR6
    // asm 000099D5: 	CLRI	AR5
    // asm 000099D6: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR6
VAALP2:
    // asm 000099D7: 	LDI	*AR6++,R2
    // asm 000099D8: 	LDI	*AR6++,R2
    // asm 000099D9: 	LDI	*AR6++,R2		;RESET VALUE
    // asm 000099DA: 	LDI	AR5,AR2
    // asm 000099DB: 	CALL	AUDIT_WRITE
    // asm 000099DC: 	INC	AR5
    // asm 000099DD: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm 000099DE: 	BLT	VAALP2
    // asm 000099DF: 	CALL	GETCOIN_DEFAULT
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
    // asm 000099E0: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR2
    // asm 000099E1: 	LDI	ADJ_COINMODE,R0
    // asm 000099E2: 	MPYI	3,R0
    // asm 000099E3: 	ADDI	R0,AR2
    // asm 000099E4: 	LDI	*+AR2,R0
    // asm 000099E5: 	CMPI	R0,R2
    // asm 000099E6: 	LDIGT	*+AR2(2),R2
    // asm 000099E7: 	LDI	ADJ_COINMODE,AR2
    // asm 000099E8: 	CALL	ADJUSTMENT_WRITE
    // asm 000099E9: 	CALL	CHECKSUMGEN_ADJ
    // asm 000099EA: 	LDI	R0,R2
    // asm 000099EB: 	SETAUD	ADJ_CHECKSUM
    // asm 000099ED: 	POP	AR6
    // asm 000099EE: 	POP	AR5
    // asm 000099EF: 	POP	AR2
    // asm 000099F0: 	POP	R3
    // asm 000099F1: 	POP	R2
    // asm 000099F2: 	POP	R1
    // asm 000099F3: 	POP	R0
    // asm 000099F4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_ADJUSTMENTS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*GENERATE CHECKSUM FOR THE ADJUSTMENTS
*
*RETURNS
*	R0	ADJUSTMENT CHECKSUM
*
*/
static void CHECKSUMGEN_ADJ(void)
{
    // asm 000099F5: 	PUSH	R1
    crusn_machine_push_reg32(R1);
    // asm 000099F6: 	PUSH	AR2
    crusn_machine_push_u32((u32)AR2);
    // asm 000099F7: 	PUSH	AR5
    crusn_machine_push_u32((u32)AR5);
    // asm 000099F8: 	CLRI	AR5
    AR5 = 0;
    // asm 000099F9: 	CLRI	R1
    R1.s = 0;
VAALP3:
    // asm 000099FA: 	LDI	AR5,AR2
    AR2 = AR5;
    // asm 000099FB: 	CALL	AUDIT_READ	;R0 = ADJUSTMENT VALUE
    AUDIT_READ();
    // asm 000099FC: 	ADDI	R0,R1
    R1.s += R0.s;
    // asm 000099FD: 	INC	AR5
    ++AR5;
    // asm 000099FE: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm 000099FF: 	BLT	VAALP3
    if ((int32_t)AR5 < NUM_ADJUSTMENTS) {
        goto VAALP3;
    }
    // asm 00009A00: 	LDI	R1,R0
    R0 = R1;
    // asm 00009A01: 	POP	AR5
    AR5 = crusn_machine_pop_u32();
    // asm 00009A02: 	POP	AR2
    AR2 = crusn_machine_pop_u32();
    // asm 00009A03: 	POP	R1
    R1 = crusn_machine_pop_reg32();
    // asm 00009A04: 	RETS
}

// *----------------------------------------------------------------------------

/*
;*----------------------------------------------------------------------------
;*GENERATE CHECKSUM FOR THE AUDITS
;*
;*RETURNS
;*	R0	ADJUSTMENT CHECKSUM
;*
;CHECKSUMGEN_AUD:
;	PUSH	R0
;	PUSH	R1
;	PUSH	AR2
;	PUSH	AR5
;
;	LDI	AUD_COIN1,AR5
;	CLRI	R1
;VAALP4	LDI	AR5,AR2
;	CALL	AUDIT_READ	;R0 = ADJUSTMENT VALUE
;	ADDI	R0,R1
;
;	INC	AR5
;	CMPI	NUM_AUDITS-MAX_ADJUSTMENTS,AR5
;	BLT	VAALP4
;
;	LDI	R1,R0
;	POP	AR5
;	POP	AR2
;	POP	R1
;	POP	R0
;	RETS
;*----------------------------------------------------------------------------
*/

/*
*----------------------------------------------------------------------------
*void	audit_inc(int index)
*
*PARAMETERS
*	AR2	INDEX
*RETURNS
*	R0	AUDIT VALUE
*
*/
void AUDIT_INC(void)
{
    // asm 00009A05: 	LS	2,AR2
    // asm 00009A06: 	ADDI	@CMOSI,AR2
    // asm 00009A07: 	CALL	_rd_cw
    // asm 00009A08: 	NOP	*AR2--(4)
    // asm 00009A09: 	ADDI	1,R0
    // asm 00009A0A: 	LDI	R0,R2
    // asm 00009A0B: 	CALL	_wr_cw
    // asm 00009A0C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_INC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*void	audit_add(int index)
*
*PARAMETERS
*	AR2	INDEX
*	R2	VALUE
*RETURNS
*	R0	AUDIT VALUE
*
*/
void AUDIT_ADD(void)
{
    // asm 00009A0D: 	LS	2,AR2
    // asm 00009A0E: 	ADDI	@CMOSI,AR2
    // asm 00009A0F: 	CALL	_rd_cw
    // asm 00009A10: 	NOP	*AR2--(4)
    // asm 00009A11: 	ADDI	R0,R2
    // asm 00009A12: 	CALL	_wr_cw
    // asm 00009A13: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_ADD", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*AUDIT_READ
*
*PARAMETERS
*	AR2	INDEX
*RETURNS
*	R0	AUDIT VALUE
*
*/
void AUDIT_READ(void)
{
    // asm 00009A14: 	LS	2,AR2
    AR2 <<= 2;
    // asm 00009A15: 	ADDI	@CMOSI,AR2
    AR2 += CMOSI;
    // asm 00009A16: 	CALL	_rd_cw
    _rd_cw();
    // asm 00009A17: 	NOP	*AR2--(4)
    AR2 -= 4;
    // asm 00009A18: 	RETS
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*
*
*
*PARAMETERS
*	AR2	INDEX
*	R2	VALUE
*RETURNS
*	R0	AUDIT VALUE
*/
void ADJUSTMENT_WRITE(void)
{
    // asm 00009A19: 	CALL	AUDIT_WRITE_ADJ
    AUDIT_WRITE_ADJ();
    // asm 00009A1A: 	CALL	CHECKSUMGEN_ADJ
    CHECKSUMGEN_ADJ();
    // asm 00009A1B: 	LDI	R0,R2
    R2 = R0;
    // asm 00009A1C: 	LDI	ADJ_CHECKSUM,AR2
    AR2 = ADJ_CHECKSUM;
    // asm 00009A1D: 	CALL	AUDIT_WRITE_ADJ
    AUDIT_WRITE_ADJ();
    // asm 00009A1E: 	RETS
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*AUDIT_WRITE
*
*PARAMETERS
*	AR2	INDEX
*	R2	VALUE
*RETURNS
*	R0	AUDIT VALUE
*
*/
void AUDIT_WRITE(void)
{
#if DEBUG
    // asm: 	CMPI	NUM_ADJUSTMENTS,AR2
    // asm: 	BLT	$	;AUDIT BEING USED TO WRITE ADJUSTMENT
#endif
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_WRITE", 0, 0);
    UNIMPL();
}

static void AUDIT_WRITE_ADJ(void)
{
    // asm 00009A1F: 	LS	2,AR2
    AR2 <<= 2;
    // asm 00009A20: 	ADDI	@CMOSI,AR2
    AR2 += CMOSI;
    // asm 00009A21: 	CALL	_wr_cw
    _wr_cw();
    // asm 00009A22: 	NOP	*AR2--(4)
    AR2 -= 4;
    // asm 00009A23: 	RETS
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*
*NOTE!!!:	MODIFICATIONS TO _rd_cw,_wr_cw,_rd_cwR,_wr_cwR
*NOTE!!!:	MODIFICATIONS TO _rd_cw,_wr_cw,_rd_cwR,_wr_cwR
*NOTE!!!:	MODIFICATIONS TO _rd_cw,_wr_cw,_rd_cwR,_wr_cwR
*
*
*THE REGISTER RULES ARE:
*
*	AR2 is incremented to next entry in CMOS
*	R0  holds RETURN VALUE
*
*	ALL OTHER REGS ARE PRESERVED!!!
*	ALL OTHER REGS ARE PRESERVED!!!
*	ALL OTHER REGS ARE PRESERVED!!!
*
*
*	These are the basic pieces for CMOS I/O.  No other routines
*	should directly access CMOS.
*
*----------------------------------------------------------------------------
*/

/*
*----------------------------------------------------------------------------
*int	rd_cw(int addr);
*READ CMOS WORD
*
*PARAMETERS
*	AR2	ADDR
*RETURNS
*	R0	WORD
*	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
*/
void _rd_cw(void)
{
    // asm 00009A24: 	PUSH	R1
    crusn_machine_push_reg32(R1);
    // asm 00009A25: 	CMOS_ON
    // asm 00009A26: 	NOP			;DELAY FOR TIMING...
    // asm 00009A27: 	LDI	*AR2++,R0
    R0.u = crusn_mem_rd32(AR2++);
    // asm 00009A28: 	RS	24,R0
    R0.s >>= 24;
    // asm 00009A29: 	LS	8,R0
    R0.u <<= 8;
    // asm 00009A2A: 	LDI	*AR2++,R1
    R1.u = crusn_mem_rd32(AR2++);
    // asm 00009A2B: 	RS	24,R1
    R1.s >>= 24;
    // asm 00009A2C: 	OR	R1,R0
    R0.u |= R1.u;
    // asm 00009A2D: 	LS	8,R0
    R0.u <<= 8;
    // asm 00009A2E: 	LDI	*AR2++,R1
    R1.u = crusn_mem_rd32(AR2++);
    // asm 00009A2F: 	RS	24,R1
    R1.s >>= 24;
    // asm 00009A30: 	OR	R1,R0
    R0.u |= R1.u;
    // asm 00009A31: 	LS	8,R0
    R0.u <<= 8;
    // asm 00009A32: 	LDI	*AR2++,R1
    R1.u = crusn_mem_rd32(AR2++);
    // asm 00009A33: 	RS	24,R1
    R1.s >>= 24;
    // asm 00009A34: 	OR	R1,R0
    R0.u |= R1.u;
    // asm 00009A35: 	CMOS_OFF
    // asm 00009A36: 	POP	R1
    R1 = crusn_machine_pop_reg32();
    // asm 00009A37: 	RETS
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*void	wr_cw(int addr, int value);
*WRITE CMOS WORD
*
*PARAMETERS
*	AR2	LOC
*	R2	WORD
*RETURNS
*	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
*
*/
void _wr_cw(void)
{
    // asm 00009A38: 	PUSH	R1
    // asm 00009A39: 	PUSH	R2
    // asm 00009A3A: 	PUSH	R3
    crusn_machine_push_reg32(R2);
    // asm 00009A3B: 	CMOS_ON
    // asm 00009A3C: 	CMOS_WP_OFF
    // asm 00009A3D: 	NOP			;DELAY FOR TIMING...
    // asm 00009A3E: 	STI	R2,*AR2++
    crusn_mem_wr32(AR2++, R2.u);
    // asm 00009A3F: 	LS	8,R2
    R2.u <<= 8;
    // asm 00009A40: 	STI	R2,*AR2++
    crusn_mem_wr32(AR2++, R2.u);
    // asm 00009A41: 	LS	8,R2
    R2.u <<= 8;
    // asm 00009A42: 	STI	R2,*AR2++
    crusn_mem_wr32(AR2++, R2.u);
    // asm 00009A43: 	LS	8,R2
    R2.u <<= 8;
    // asm 00009A44: 	STI	R2,*AR2++
    crusn_mem_wr32(AR2++, R2.u);
    // asm 00009A45: 	CMOS_WP_ON
    // asm 00009A46: 	CMOS_OFF
    // asm 00009A47: 	POP	R3
    // asm 00009A48: 	POP	R2
    R2 = crusn_machine_pop_reg32();
    // asm 00009A49: 	POP	R1
    // asm 00009A4A: 	RETS
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*int	rd_cmR(int addr);
*READ CMOS WORD REVERSED  (MOTOROLA STYLE)
*
*PARAMETERS
*	AR2	LOC
*RETURNS
*	R0	WORD
*	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
*
*/
void _rd_cwR(void)
{
    // asm 00009A4B: 	PUSH	R1
    // asm 00009A4C: 	CMOS_ON
    // asm 00009A4D: 	NOP			;DELAY FOR TIMING...
    // asm 00009A4E: 	LDI	*AR2++,R0
    // asm 00009A4F: 	RS	24,R0
    // asm 00009A50: 	LDI	*AR2++,R1
    // asm 00009A51: 	RS	24,R1
    // asm 00009A52: 	LS	8,R1
    // asm 00009A53: 	OR	R1,R0
    // asm 00009A54: 	LDI	*AR2++,R1
    // asm 00009A55: 	RS	24,R1
    // asm 00009A56: 	LS	16,R1
    // asm 00009A57: 	OR	R1,R0
    // asm 00009A58: 	LDI	*AR2++,R1
    // asm 00009A59: 	RS	24,R1
    // asm 00009A5A: 	LS	24,R1
    // asm 00009A5B: 	OR	R1,R0
    // asm 00009A5C: 	CMOS_OFF
    // asm 00009A5D: 	POP	R1
    // asm 00009A5E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rd_cwR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*void	wr_cmR(int addr, int value);
*WRITE CMOS WORD REVERSED
*
*PARAMETERS
*	AR2	LOC
*	R2	WORD
*RETURNS
*	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
*
*/
void _wr_cwR(void)
{
    // asm 00009A5F: 	PUSH	R1
    // asm 00009A60: 	PUSH	R2
    // asm 00009A61: 	PUSH	R3
    // asm 00009A62: 	LDI	R2,R3
    // asm 00009A63: 	LS	24,R3
    // asm 00009A64: 	CMOS_ON
    // asm 00009A65: 	CMOS_WP_OFF
    // asm 00009A66: 	NOP			;DELAY FOR TIMING...
    // asm 00009A67: 	STI	R3,*AR2++
    // asm 00009A68: 	LDI	R2,R3
    // asm 00009A69: 	RS	8,R3
    // asm 00009A6A: 	LS	24,R3
    // asm 00009A6B: 	STI	R3,*AR2++
    // asm 00009A6C: 	LDI	R2,R3
    // asm 00009A6D: 	RS	16,R3
    // asm 00009A6E: 	LS	24,R3
    // asm 00009A6F: 	STI	R3,*AR2++
    // asm 00009A70: 	RS	24,R2
    // asm 00009A71: 	LS	24,R3
    // asm 00009A72: 	STI	R2,*AR2++
    // asm 00009A73: 	CMOS_WP_ON
    // asm 00009A74: 	CMOS_OFF
    // asm 00009A75: 	POP	R3
    // asm 00009A76: 	POP	R2
    // asm 00009A77: 	POP	R1
    // asm 00009A78: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_wr_cwR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
#define DEFAULT_TABLE_SIZE 2
/* asm: DEFAULT_TABLE: */
/* asm: TABLEENT	'T','V','G',2,01,3 */
/* asm: TABLEENT	'E','L','P',2,02,3 */
/* asm: TABLEENT	'E','P','J',2,03,3 */
/* asm: TABLEENT	'M','L',' ',2,04,3 */
/* asm: TABLEENT	'G','W','S',2,06,3 */
/* asm: TABLEENT	'S','X','C',2,08,3 */
/* asm: TABLEENT	'T','E','D',2,10,3 */
/* asm: TABLEENT	'M','E','Y',2,12,3 */
/* asm: TABLEENT	'M','M','V',2,14,3 */
/* asm: TABLEENT	'B','D','P',2,16,3 */
static int DEFAULT_TABLE;
/* asm: DEFAULT_TABLE_TOTAL: */
/* asm: TABLEENT	'T','V','G',28,00,3 */
/* asm: TABLEENT	'E','L','P',28,10,3 */
/* asm: TABLEENT	'E','P','J',28,20,3 */
/* asm: TABLEENT	'M','L',' ',28,30,3 */
/* asm: TABLEENT	'G','W','S',28,40,3 */
/* asm: TABLEENT	'S','X','C',28,50,3 */
/* asm: TABLEENT	'T','E','D',30,00,3 */
/* asm: TABLEENT	'M','E','Y',31,10,3 */
/* asm: TABLEENT	'M','M','V',32,20,3 */
/* asm: TABLEENT	'B','D','P',34,30,3 */
static int DEFAULT_TABLE_TOTAL;
#define NUM_TABLES 14

/*
*INIT_LASTHS_TABLE
*CHECK_LASTHS
*----------------------------------------------------------------------------
*Initialize the LAST HS SET CMOS table
*
*This table is to keep track of the High Score entries set (if any at all)
*by the last player.
*There is one byte per Race location.
* -1 = not set by last player. 0-9 = Position in this table for that race
*/
void INIT_LASTHS_TABLE(void)
{
    // asm 00009AA2: 	LDI	NUM_TABLES+1,R6
    // asm 00009AA3: 	LDI	0,R7
    // asm 00009AA4: 	CALL	GET_TABLE_ADDR
    // asm 00009AA5: 	LDI	-1,R2
    // asm 00009AA6: 	LDI	14,AR5
    // asm 00009AA7: ILT_LP
    // asm 00009AA7: 	CALL	_wr_cw
    // asm 00009AA8: 	DBU	AR5,ILT_LP
    // asm 00009AA9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LASTHS_TABLE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*	R6 = RACE NUMBER
*	R7 = ENTRY NUMBER
*/
static void UPDATE_LASTHS(void)
{
    // asm 00009AAA: 	PUSH	AR2
    // asm 00009AAB: 	PUSH	R0
    // asm 00009AAC: 	PUSH	R2
    // asm 00009AAD: 	PUSH	R6
    // asm 00009AAE: 	PUSH	R7
    // asm 00009AAF: 	LDI	R6,R0
    // asm 00009AB0: 	LDI	NUM_TABLES+1,R6
    // asm 00009AB1: 	LDI	0,R7
    // asm 00009AB2: 	CALL	GET_TABLE_ADDR
    // asm 00009AB3: 	MPYI	4,R0		;4 bytes per word
    // asm 00009AB4: 	ADDI	R0,AR2
    // asm 00009AB5: 	POP	R7
    // asm 00009AB6: 	LDI	R7,R2
    // asm 00009AB7: 	CALL	_wr_cw
    // asm 00009AB8: 	POP	R6
    // asm 00009AB9: 	POP	R2
    // asm 00009ABA: 	POP	R0
    // asm 00009ABB: 	POP	AR2
    // asm 00009ABC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UPDATE_LASTHS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*	R1 = RACE NUMBER
*
*Returns R0 = position in table of the last player that played. -1 = not in table
*/

void CHECK_LASTHS(void)
{
    // asm 00009ABD: 	LDI	NUM_TABLES+1,R6
    // asm 00009ABE: 	LDI	0,R7
    // asm 00009ABF: 	CALL	GET_TABLE_ADDR
    // asm 00009AC0: 	MPYI	4,R1		;4 bytes per word
    // asm 00009AC1: 	ADDI	R1,AR2
    // asm 00009AC2: 	CALL	_rd_cw
    // asm 00009AC3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_LASTHS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void INIT_HSTD_TABLES(void)
{
    // asm 00009AC4: 	CLRI	R6
    // asm 00009AC5: 	LDI	15-1,AR5
    // asm 00009AC6: ITL1
    // asm 00009AC6: 	CLRI	R7
    // asm 00009AC7: 	LDI	10-1,AR4
    // asm 00009AC8: 	LDL	DEFAULT_TABLE,AR1
    // asm 00009AC9: 	CMPI	14,R6			;the 15th entry is the totals for the entire country
    // asm 00009ACA: 	LDIEQ	@DEFAULT_TABLE_TOTALI,AR1
    // asm 00009ACB: ITL2
    // asm 00009ACB: 	CALL	GET_TABLE_ADDR
    // asm 00009ACC: 	CALL	TABLE_ENTRY_WRITE
    // asm 00009ACD: 	ADDI	DEFAULT_TABLE_SIZE,AR1
    // asm 00009ACE: 	INC	R7
    // asm 00009ACF: 	DBU	AR4,ITL2
    // asm 00009AD0: 	INC	R6
    // asm 00009AD1: 	DBU	AR5,ITL1
    // asm 00009AD2: 	CALL	INIT_LASTHS_TABLE
    // asm 00009AD3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_HSTD_TABLES", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void VALIDATE_HSTD_TABLES(void)
{
    // asm 00009AD4: 	CLRI	R6
    // asm 00009AD5: 	LDI	10-1,AR5
    // asm 00009AD6: ITL1A
    // asm 00009AD6: 	CLRI	R7
    // asm 00009AD7: 	LDI	10-1,AR4
    // asm 00009AD8: 	LDL	DEFAULT_TABLE,AR1
    // asm 00009AD9: ITL2A
    // asm 00009AD9: 	CALL	GET_TABLE_ADDR
    // asm 00009ADA: 	CALL	TABLE_ENTRY_READ
    // asm 00009ADB: 	CMPI	' ',R1
    // asm 00009ADC: 	BEQ	J1
    // asm 00009ADD: 	CMPI	'0',R1
    // asm 00009ADE: 	BLT	RESETALL
    // asm 00009ADF: 	CMPI	'Z',R1
    // asm 00009AE0: 	BGT	RESETALL
J1:
    // asm 00009AE1: CMPI	' ',R2
    // asm 00009AE2: 	BEQ	J2
    // asm 00009AE3: 	CMPI	'0',R2
    // asm 00009AE4: 	BLT	RESETALL
    // asm 00009AE5: 	CMPI	'Z',R2
    // asm 00009AE6: 	BGT	RESETALL
J2:
    // asm 00009AE7: CMPI	' ',R3
    // asm 00009AE8: 	BEQ	J3
    // asm 00009AE9: 	CMPI	'0',R3
    // asm 00009AEA: 	BLT	RESETALL
    // asm 00009AEB: 	CMPI	'Z',R3
    // asm 00009AEC: 	BGT	RESETALL
J3:
    // asm 00009AED: CMPI	0,R4
    // asm 00009AEE: 	BLT	RESETALL
    // asm 00009AEF: 	CMPI	10,R4
    // asm 00009AF0: 	BGT	RESETALL
    // asm 00009AF1: 	CMPI	4*60*60,R0
    // asm 00009AF2: 	BGT	RESETALL
    // asm 00009AF3: 	CMPI	0,R0
    // asm 00009AF4: 	BLT	RESETALL
    // asm 00009AF5: 	ADDI	DEFAULT_TABLE_SIZE,AR1
    // asm 00009AF6: 	INC	R7
    // asm 00009AF7: 	DBU	AR4,ITL2A
    // asm 00009AF8: 	INC	R6
    // asm 00009AF9: 	DBU	AR5,ITL1A
    // asm 00009AFA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VALIDATE_HSTD_TABLES", 0, 0);
    UNIMPL();
}

static void RESETALL(void)
{
    // asm 00009AFB: 	CALL	INIT_HSTD_TABLES
    // asm 00009AFC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESETALL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*
*PARAMETERS
*	R6	RACE NUMBER
*	R7	ENTRY NUMBER
*
*RETURNS
*	AR2	ADDRESS
*
*/
void GET_TABLE_ADDR(void)
{
    // asm 00009AFD: 	PUSH	R6
    // asm 00009AFE: 	PUSH	R7
    // asm 00009AFF: 	MPYI	RACE_TABLE_SIZE,R6
    // asm 00009B00: 	MPYI	TE_SIZE,R7
    // asm 00009B01: 	ADDI	R6,R7,AR2
    // asm 00009B02: 	ADDI	MAX_AUDITS*4,AR2
    // asm 00009B03: 	ADDI	@CMOSI,AR2
    // asm 00009B04: 	POP	R7
    // asm 00009B05: 	POP	R6
    // asm 00009B06: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TABLE_ADDR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*WRITE ENTRY IN TABLE
*
*PARAMETER
*	AR2	ADDRESS OF ENTRY
*	AR1	TABLE ENTRY
*RETURNS
*	(AR2 INCREMENTED TO THE NEXT ENTRY)
*
*
*/
static void TABLE_ENTRY_WRITE(void)
{
    // asm 00009B07: 	PUSH	R0
    // asm 00009B08: 	PUSH	R1
    // asm 00009B09: 	PUSH	R2
    // asm 00009B0A: 	PUSH	R3
    // asm 00009B0B: 	PUSH	AR1
    // asm 00009B0C: 	LDI	*AR1++,R0
    // asm 00009B0D: 	LDI	*AR1++,R1
    // asm 00009B0E: 	LDI	R1,R2
    // asm 00009B0F: 	RS	8,R2
    // asm 00009B10: 	LDI	R1,R3
    // asm 00009B11: 	RS	16,R3
    // asm 00009B12: 	LDI	R1,R4
    // asm 00009B13: 	RS	24,R4
    // asm 00009B14: 	POP	AR1
    // asm 00009B15: 	BU	TEWL1
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
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_WRITE", 0, 0);
    UNIMPL();
}

static void TABLE_ENTRY_WRITE0(void)
{
    // asm 00009B16: 	PUSH	R0
    // asm 00009B17: 	PUSH	R1
    // asm 00009B18: 	PUSH	R2
    // asm 00009B19: 	PUSH	R3
TEWL1:
    // asm 00009B1A: 	PUSH	R2
    // asm 00009B1B: 	LDI	R0,R2
    // asm 00009B1C: 	CALL	_wr_cw
    // asm 00009B1D: 	POP	R2
    // asm 00009B1E: 	PUSH	R1
    // asm 00009B1F: 	CMOS_ON
    // asm 00009B20: 	CMOS_WP_OFF
    // ;	NOP			;DELAY FOR TIMING...
    // asm 00009B21: 	POP	R1
    // asm 00009B22: 	LS	24,R1
    // asm 00009B23: 	STI	R1,*AR2++
    // asm 00009B24: 	LS	24,R2
    // asm 00009B25: 	STI	R2,*AR2++
    // asm 00009B26: 	LS	24,R3
    // asm 00009B27: 	STI	R3,*AR2++
    // asm 00009B28: 	LS	24,R4
    // asm 00009B29: 	STI	R4,*AR2++
    // asm 00009B2A: 	CMOS_WP_ON
    // asm 00009B2B: 	CMOS_OFF
    // asm 00009B2C: 	POP	R3
    // asm 00009B2D: 	POP	R2
    // asm 00009B2E: 	POP	R1
    // asm 00009B2F: 	POP	R0
    // asm 00009B30: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_WRITE0", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*READ A TABLE ENTRY
*
*
*PARAMETERS
*	AR2	ADDR
*RETURNS
*	R0	TIME CODE
*	R1	Initial 1
*	R2	Initial 2
*	R3	Initial 3
*	R4	POSITION
*	(AR2 INCREMENTED TO THE NEXT ENTRY)
*
*/
void TABLE_ENTRY_READ(void)
{
    // asm 00009B31: 	CALL	_rd_cw
    // asm 00009B32: 	PUSH	R0
    // asm 00009B33: 	CMOS_ON
    // asm 00009B34: 	CMOS_WP_OFF
    // asm 00009B35: 	LDI	*AR2++,R1
    // asm 00009B36: 	RS	24,R1
    // asm 00009B37: 	LDI	*AR2++,R2
    // asm 00009B38: 	RS	24,R2
    // asm 00009B39: 	LDI	*AR2++,R3
    // asm 00009B3A: 	RS	24,R3
    // asm 00009B3B: 	LDI	*AR2++,R4
    // asm 00009B3C: 	RS	24,R4
    // asm 00009B3D: 	PUSH	R1
    // asm 00009B3E: 	CMOS_WP_ON	;This trashes register R1
    // asm 00009B3F: 	CMOS_OFF
    // asm 00009B40: 	POP	R1
    // asm 00009B41: 	POP	R0
    // asm 00009B42: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_READ", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*CHECK_TABLE
*
*THIS ROUTINE CHECKS THE SPECIFIED TABLE TO SEE WHETHER OR NOT
*THE PLAYER CAN ENTER INTO THE TABLE
*
*PARAMETERS
*	R0	TIME CODE (TO CHECK)
*	R1	RACE INDEX
*RETURNS
*	R0	{-1}     = NO, MAY NOT ENTER
*	R0	{0...9}  = INDEX TO ENTER TABLE
*
*/
void CHECK_RACE_TABLE(void)
{
    // asm 00009B43: 	PUSH	R0
    // asm 00009B44: 	LDI	R1,R6
    // asm 00009B45: 	CLRI	R7
    // asm 00009B46: 	CALL	GET_TABLE_ADDR
    // asm 00009B47: 	POP	R6
    // asm 00009B48: 	CLRI	R5		;RETURN INDEX
NXTENT:
    // asm 00009B49: CALL	TABLE_ENTRY_READ
    // asm 00009B4A: 	CMPI	R0,R6
    // asm 00009B4B: 	BLT	INSERT_HERE
    // asm 00009B4C: 	INC	R5
    // asm 00009B4D: 	CMPI	10,R5
    // asm 00009B4E: 	BLT	NXTENT
    // asm 00009B4F: 	LDI	-1,R0
    // asm 00009B50: 	RETS
INSERT_HERE:
    // asm 00009B51: 	LDI	R5,R0
    // asm 00009B52: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_RACE_TABLE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*
*
*PARAMETERS
*	R0	TIME CODE
*	R1	Initial 1
*	R2	Initial 2
*	R3	Initial 3
*	R4	POSITION
*	R5	INDEX #
*	R6	RACE #
*
*
*
*/
void INSERT_TABLE_ENTRY(void)
{
    // asm 00009B53: 	PUSH	R0
    // asm 00009B54: 	PUSH	R1
    // asm 00009B55: 	PUSH	R2
    // asm 00009B56: 	PUSH	R3
    // asm 00009B57: 	PUSH	R4
    // asm 00009B58: 	PUSH	R5
    // asm 00009B59: 	LDI	NUM_ENTRIES_PER_RACE-2,R7	;Start at position 8
    // asm 00009B5A: 	CALL	GET_TABLE_ADDR
    // asm 00009B5B: 	CMPI	9,R5
    // asm 00009B5C: 	BGE	ITLX
ITEL:
    // asm 00009B5D: CALL	TABLE_ENTRY_READ
    // asm 00009B5E: 	CALL	TABLE_ENTRY_WRITE0	;This uses the registers as input instead of *AR1
    // asm 00009B5F: 	NOP	*AR2--(TE_SIZE*3)
    // asm 00009B60: 	DEC	R7
    // asm 00009B61: 	CMPI	R5,R7
    // asm 00009B62: 	BGE	ITEL				;Copy down position including where inserting
ITLX:
    // asm 00009B63: POP	R7
    // asm 00009B64: 	CALL	GET_TABLE_ADDR			;Get the ADDR of the insert point
    // asm 00009B65: 	POP	R4
    // asm 00009B66: 	POP	R3
    // asm 00009B67: 	POP	R2
    // asm 00009B68: 	POP	R1
    // asm 00009B69: 	POP	R0
    // asm 00009B6A: 	CALL	TABLE_ENTRY_WRITE0	;This uses the registers as input instead of *AR1
    // asm 00009B6B: 	CALL	UPDATE_LASTHS
    // asm 00009B6C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_TABLE_ENTRY", 0, 0);
    UNIMPL();
}

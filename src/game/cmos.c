#include "cmos.h"

#include "../core/machine.h"
#include "c30.h"
#include "globals.h"
#include "macs.h"
#include "sys.h"
#include "validator.h"
#include "vunit.h"

/*
 * Source module: asm/CMOS.ASM
 */

void HSTDEC(void);
void CMOS_ON_C(void);
void CMOS_OFF_C(void);
void CMOS_WPON_C(void);
void CMOS_WPOFF_C(void);
int VALIDATE_CMOS(void);
void RESET_BOOKKEEPING(void);
static void VERIFY_ADJUSTMENTS_ACCURACY(void);
void RESET_ADJUSTMENTS(void);
static int CHECKSUMGEN_ADJ(void);
int AUDIT_INC(int index);
int AUDIT_ADD(int index, int value);
#define ADJUSTMENT_READ AUDIT_READ
int AUDIT_READ(int index);
int ADJUSTMENT_WRITE(int index, int value);
int AUDIT_WRITE(int index, int value);
static int AUDIT_WRITE_ADJ(int index, int value);
int _rd_cw(word_addr_t addr);
void _wr_cw(word_addr_t addr, int value);
int _rd_cwR(word_addr_t addr);
void _wr_cwR(word_addr_t addr, int value);
void INIT_LASTHS_TABLE(void);
static void UPDATE_LASTHS(void);
void CHECK_LASTHS(void);
void INIT_HSTD_TABLES(void);
int VALIDATE_HSTD_TABLES(void);
word_addr_t GET_TABLE_ADDR(int race_index /*R6*/, int entry_index /*R7*/);
static void TABLE_ENTRY_WRITE(void);
static void TABLE_ENTRY_WRITE0(void);
RACEENTRY TABLE_ENTRY_READ(word_addr_t* addr /*AR2*/);
void CHECK_RACE_TABLE(void);
void INSERT_TABLE_ENTRY(void);

#define ADJUSTMENT_READ AUDIT_READ
#define CMOSI CMOS
#define DEFAULT_TABLE_TOTALI DEFAULT_TABLE_TOTAL

void GETCOIN_DEFAULT(void);

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
void HSTDEC(void) {
    int value = AUDIT_READ(ADJ_ACTUALHSTDRESET) - 1;

    if (value < 0) {
        value = 0;
    }

    ADJUSTMENT_WRITE(ADJ_ACTUALHSTDRESET, value);
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void CMOS_ON_C(void) {
    // crusn_mem_wr32(CPU_WS, CMOS_WS);
}

void CMOS_OFF_C(void) {
    // crusn_mem_wr32(CPU_WS, SOFT_WS);
}

void CMOS_WPON_C(void) {
    // crusn_mem_wr32(CMOS_WP_WORD, (u32)(CMOS_WP_WORD_SHADOW & 0x0F00));
}

void CMOS_WPOFF_C(void) {
    // crusn_mem_wr32(CMOS_WP_WORD, (u32)((CMOS_WP_WORD_SHADOW & 0x0F00) | CMOS_WP));
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
int VALIDATE_CMOS(void) {
    int partial_credits = AUDIT_READ(AUD_PCREDITS);
    int credits = AUDIT_READ(AUD_CREDITS);
    int hstd_ok;

    if (partial_credits > 10) {
        partial_credits = 10;
    } else if (partial_credits < 0) {
        partial_credits = 0;
    }
    AUDIT_WRITE(AUD_PCREDITS, partial_credits);

    if (credits > 30) {
        credits = 30;
    } else if (credits < 0) {
        credits = 0;
    }
    AUDIT_WRITE(AUD_CREDITS, credits);

    hstd_ok = VALIDATE_HSTD_TABLES();
    VERIFY_ADJUSTMENTS_ACCURACY();

    // CLRC
    return 0;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *RESET_BOOKKEEPING
 *
 *	RESET THE BOOKKEEPING CMOS PAGE (GENERATE THE NEW CHECKSUM)
 *
 */
void RESET_BOOKKEEPING(void) {
    int index;

    for (index = MAX_ADJUSTMENTS + 1; index < NUM_AUDITS; ++index) {
        _wr_cw((word_addr_t)CMOSI + (word_addr_t)(index * 4), 0);
    }
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
#define VADJTAB(LOW, HIGH, DEFAULT) { LOW, HIGH, DEFAULT }
static const ADJUSTMENT_RANGE VERIFY_ADJUSTMENTS_ACCURACYTAB[] = {
    VADJTAB(0, 81, 0),          /* ADJ_COINMODE */
    VADJTAB(0, 255, 0),         /* ADJ_GASMIN */
    VADJTAB(0, 255, 255),       /* ADJ_GASMAX */
    VADJTAB(0, 255, 0),         /* ADJ_STEERMIN */
    VADJTAB(0, 255, 255),       /* ADJ_STEERMAX */
    VADJTAB(0, 255, 128),       /* ADJ_STEERCENTER */
    VADJTAB(0, 255, 0),         /* ADJ_BRAKEMIN */
    VADJTAB(0, 255, 255),       /* ADJ_BRAKEMAX */
    VADJTAB(0, 255, 200),       /* ADJ_VOLUME */
    VADJTAB(0, 100, 1),         /* ADJ_COIN1_UNITS */
    VADJTAB(0, 100, 1),         /* ADJ_COIN2_UNITS */
    VADJTAB(0, 100, 1),         /* ADJ_COIN3_UNITS */
    VADJTAB(0, 100, 1),         /* ADJ_COIN4_UNITS */
    VADJTAB(0, 100, 1),         /* ADJ_UNITS_PER_CREDIT */
    VADJTAB(0, 100, 0),         /* ADJ_BONUS_UNITS */
    VADJTAB(0, 100, 0),         /* ADJ_UNITS_MIN */
    VADJTAB(0, 100, 3),         /* ADJ_CREDITS_TO_START */
    VADJTAB(0, 100, 3),         /* ADJ_CREDITS_TO_CONTINUE */
    VADJTAB(0, 1, 1),           /* ADJ_SHOW_FRAC */
    VADJTAB(0, 127, 1),         /* ADJ_COIN1_COUNTER */
    VADJTAB(0, 127, 1),         /* ADJ_COIN2_COUNTER */
    VADJTAB(0, 127, 1),         /* ADJ_COIN3_COUNTER */
    VADJTAB(0, 127, 4),         /* ADJ_COIN4_COUNTER */
    VADJTAB(0, 1, 1),           /* ADJ_STANDARD_PRICING */
    VADJTAB(0, 1, 0),           /* ADJ_CUSTOM_PRICING */
    VADJTAB(0, 1, 0),           /* ADJ_FREE_PLAY */
    VADJTAB(0, 1, 1),           /* ADJ_FREEGAME */
    VADJTAB(0, 9, 5),           /* ADJ_DIFFICULTY */
    VADJTAB(0, 6, 3),           /* ADJ_TIME_TO_START */
    VADJTAB(10, 25, 20),        /* ADJ_CHECKPOINT_BONUS */
    VADJTAB(0, 1, 0),           /* ADJ_ATTRACT_MODE_SOUND */
    VADJTAB(0, 1, 1),           /* ADJ_HIGH_SCORE_ENTRY */
    VADJTAB(0, 255, 100),       /* ADJ_MIN_VOL_LEVEL */
    VADJTAB(0, 25000, 4000),    /* ADJ_ACTUALHSTDRESET */
    VADJTAB(0, 1, 0),           /* ADJ_MPHORKPM */
    VADJTAB(0, 1, 1),           /* ADJ_ROADKILL */
    VADJTAB(0, 1, 1),           /* ADJ_CLINTON */
    VADJTAB(0, 1, 1),           /* ADJ_GIRLS */
    VADJTAB(0, 5, 5),           /* ADJ_STEERING_SENSE */
    VADJTAB(1000, 25000, 5000), /* ADJ_HIGHSCORE_RESET */
    VADJTAB(0, 100, 50),        /* ADJ_DIFF_LOCAL */
    VADJTAB(0, 1, 0),           /* UNKNOWN */
    VADJTAB(0, 30, 20),         /* ADJ_RAMP_PERCENTAGE */
    VADJTAB(0, 5000, 0),        /* ADJ_RAMP_COUNT */
    VADJTAB(0, 1, 1),           /* ADJ_ENTER_INITS */
    VADJTAB(10, 50, 30),        /* ADJ_MAX_CREDITS */
};

// *
static void VERIFY_ADJUSTMENTS_ACCURACY(void) {
    int index;

    for (index = 0; index < NUM_ADJUSTMENTS; ++index) {
        int value = AUDIT_READ(index);
        const ADJUSTMENT_RANGE* range = &VERIFY_ADJUSTMENTS_ACCURACYTAB[index];

        if (value < range->low || value > range->high) {
            ADJUSTMENT_WRITE(index, range->default_value);
        }
    }
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void RESET_ADJUSTMENTS(void) {
    int index;
    int checksum;

    for (index = 0; index < NUM_ADJUSTMENTS; ++index) {
        AUDIT_WRITE(index, VERIFY_ADJUSTMENTS_ACCURACYTAB[index].default_value);
    }

    GETCOIN_DEFAULT();

    if (AUDIT_READ(ADJ_COINMODE) > VERIFY_ADJUSTMENTS_ACCURACYTAB[ADJ_COINMODE].high) {
        ADJUSTMENT_WRITE(ADJ_COINMODE, VERIFY_ADJUSTMENTS_ACCURACYTAB[ADJ_COINMODE].default_value);
    }

    checksum = CHECKSUMGEN_ADJ();
    AUDIT_WRITE(ADJ_CHECKSUM, checksum);
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
static int CHECKSUMGEN_ADJ(void) {
    int checksum = 0;
    int index;

    for (index = 0; index < NUM_ADJUSTMENTS; ++index) {
        checksum += AUDIT_READ(index);
    }

    return checksum;
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
int AUDIT_INC(int index) {
    return AUDIT_ADD(index, 1);
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
int AUDIT_ADD(int index, int value) {
    int new_value = AUDIT_READ(index) + value;
    AUDIT_WRITE(index, new_value);
    return new_value;
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
int AUDIT_READ(int index) {
    return _rd_cw((u32)CMOSI + (u32)(index * 4));
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
int ADJUSTMENT_WRITE(int index, int value) {
    int checksum;

    AUDIT_WRITE_ADJ(index, value);
    checksum = CHECKSUMGEN_ADJ();
    AUDIT_WRITE_ADJ(ADJ_CHECKSUM, checksum);
    return value;
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
int AUDIT_WRITE(int index, int value) {
    AUDIT_WRITE_ADJ(index, value);
    return value;
}

static int AUDIT_WRITE_ADJ(int index, int value) {
    _wr_cw((u32)CMOSI + (u32)(index * 4), value);
    return value;
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
int _rd_cw(word_addr_t addr) {
    u32 value = 0;
    int i;

    CMOS_ON_C();
    for (i = 0; i < 4; ++i) {
        value = (value << 8) | ((crusn_mem_rd32(addr + (word_addr_t)i) >> 24) & 0xffu);
    }
    CMOS_OFF_C();

    return (int)value;
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
void _wr_cw(word_addr_t addr, int value) {
    u32 raw = (u32)value;
    int i;

    CMOS_ON_C();
    CMOS_WPOFF_C();
    for (i = 0; i < 4; ++i) {
        crusn_mem_wr32(addr + (word_addr_t)i, raw << (i * 8));
    }
    CMOS_WPON_C();
    CMOS_OFF_C();
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
int _rd_cwR(word_addr_t addr) {
    u32 value = 0;
    int i;

    CMOS_ON_C();
    for (i = 0; i < 4; ++i) {
        value |= ((crusn_mem_rd32(addr + (word_addr_t)i) >> 24) & 0xffu) << (i * 8);
    }
    CMOS_OFF_C();

    return (int)value;
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
void _wr_cwR(word_addr_t addr, int value) {
    u32 raw = (u32)value;
    int i;

    CMOS_ON_C();
    CMOS_WPOFF_C();
    for (i = 0; i < 4; ++i) {
        crusn_mem_wr32(addr + (word_addr_t)i, ((raw >> (i * 8)) & 0xffu) << 24);
    }
    CMOS_WPON_C();
    CMOS_OFF_C();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
#define DEFAULT_TABLE_SIZE 2
#define TABLEENT(I1, I2, I3, MIN, SEC, POSTN) \
    { (((MIN) * 60u * 56u) + ((SEC) * 56u)), { 0, 0, 0 }, (I1), (I2), (I3), (POSTN) }

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
static const RACEENTRY DEFAULT_TABLE[] = {
    TABLEENT('T', 'V', 'G', 2, 1, 3),
    TABLEENT('E', 'L', 'P', 2, 2, 3),
    TABLEENT('E', 'P', 'J', 2, 3, 3),
    TABLEENT('M', 'L', ' ', 2, 4, 3),
    TABLEENT('G', 'W', 'S', 2, 6, 3),
    TABLEENT('S', 'X', 'C', 2, 8, 3),
    TABLEENT('T', 'E', 'D', 2, 10, 3),
    TABLEENT('M', 'E', 'Y', 2, 12, 3),
    TABLEENT('M', 'M', 'V', 2, 14, 3),
    TABLEENT('B', 'D', 'P', 2, 16, 3),
};
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
static const RACEENTRY DEFAULT_TABLE_TOTAL[] = {
    TABLEENT('T', 'V', 'G', 28, 0, 3),
    TABLEENT('E', 'L', 'P', 28, 10, 3),
    TABLEENT('E', 'P', 'J', 28, 20, 3),
    TABLEENT('M', 'L', ' ', 28, 30, 3),
    TABLEENT('G', 'W', 'S', 28, 40, 3),
    TABLEENT('S', 'X', 'C', 28, 50, 3),
    TABLEENT('T', 'E', 'D', 30, 0, 3),
    TABLEENT('M', 'E', 'Y', 31, 10, 3),
    TABLEENT('M', 'M', 'V', 32, 20, 3),
    TABLEENT('B', 'D', 'P', 34, 30, 3),
};
#undef TABLEENT

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
void INIT_LASTHS_TABLE(void) {
    word_addr_t addr = GET_TABLE_ADDR(NUM_TABLES + 1, 0);
    int count = 14;

    // asm 00009AA2: 	LDI	NUM_TABLES+1,R6
    // asm 00009AA3: 	LDI	0,R7
    // asm 00009AA4: 	CALL	GET_TABLE_ADDR
    // asm 00009AA5: 	LDI	-1,R2
    // asm 00009AA6: 	LDI	14,AR5
    // asm 00009AA7: ILT_LP
    // asm 00009AA7: 	CALL	_wr_cw
ILT_LP:
    _wr_cw(addr, -1);
    addr += 4;
    // asm 00009AA8: 	DBU	AR5,ILT_LP
    count -= 1;
    if (count >= 0) {
        goto ILT_LP;
    }
    // asm 00009AA9: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *	R6 = RACE NUMBER
 *	R7 = ENTRY NUMBER
 */
static void UPDATE_LASTHS(void) {
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

void CHECK_LASTHS(void) {
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

void INIT_HSTD_TABLES(void) {
    int race_index = 0;

    // asm 00009AC4: 	CLRI	R6
    // asm 00009AC5: 	LDI	15-1,AR5
    // asm 00009AC6: ITL1
ITL1:
    // asm 00009AC6: 	CLRI	R7
    {
        int entry_index = 0;
        const RACEENTRY* table = race_index == 14 ? DEFAULT_TABLE_TOTAL : DEFAULT_TABLE;

        // asm 00009AC7: 	LDI	10-1,AR4
        // asm 00009AC8: 	LDL	DEFAULT_TABLE,AR1
        // asm 00009AC9: 	CMPI	14,R6			;the 15th entry is the totals for the entire country
        // asm 00009ACA: 	LDIEQ	@DEFAULT_TABLE_TOTALI,AR1
        // asm 00009ACB: ITL2
    ITL2:
        // asm 00009ACB: 	CALL	GET_TABLE_ADDR
        // asm 00009ACC: 	CALL	TABLE_ENTRY_WRITE
        {
            word_addr_t addr = GET_TABLE_ADDR(race_index, entry_index);

            _wr_cw(addr, (s32)table[entry_index].time);
            crusn_mem_wr32(addr + 4u, table[entry_index].init1 << 24);
            crusn_mem_wr32(addr + 5u, table[entry_index].init2 << 24);
            crusn_mem_wr32(addr + 6u, table[entry_index].init3 << 24);
            crusn_mem_wr32(addr + 7u, table[entry_index].rank << 24);
        }
        // asm 00009ACD: 	ADDI	DEFAULT_TABLE_SIZE,AR1
        // asm 00009ACE: 	INC	R7
        entry_index += 1;
        // asm 00009ACF: 	DBU	AR4,ITL2
        if (entry_index <= 9) {
            goto ITL2;
        }
    }
    // asm 00009AD0: 	INC	R6
    race_index += 1;
    // asm 00009AD1: 	DBU	AR5,ITL1
    if (race_index <= 14) {
        goto ITL1;
    }
    // asm 00009AD2: 	CALL	INIT_LASTHS_TABLE
    INIT_LASTHS_TABLE();
    // asm 00009AD3: 	RETS
}

// *----------------------------------------------------------------------------

int VALIDATE_HSTD_TABLES(void) {
    int race_index;
    int entry_index;
    word_addr_t addr;
    RACEENTRY entry;

    // asm 00009AD4: 	CLRI	R6
    // asm 00009AD5: 	LDI	10-1,AR5
    race_index = 0;
ITL1A:
    // asm 00009AD6: 	CLRI	R7
    // asm 00009AD7: 	LDI	10-1,AR4
    // asm 00009AD8: 	LDL	DEFAULT_TABLE,AR1
    entry_index = 0;
ITL2A:
    // asm 00009AD9: 	CALL	GET_TABLE_ADDR
    addr = GET_TABLE_ADDR(race_index, entry_index);
    // asm 00009ADA: 	CALL	TABLE_ENTRY_READ
    entry = TABLE_ENTRY_READ(&addr);

    // asm 00009ADB: 	CMPI	' ',R1
    // asm 00009ADC: 	BEQ	J1
    if (entry.init1 == ' ') {
        goto J1;
    }
    // asm 00009ADD: 	CMPI	'0',R1
    // asm 00009ADE: 	BLT	RESETALL
    if (entry.init1 < '0') {
        goto RESETALL;
    }
    // asm 00009ADF: 	CMPI	'Z',R1
    // asm 00009AE0: 	BGT	RESETALL
    if (entry.init1 > 'Z') {
        goto RESETALL;
    }
J1:
    // asm 00009AE1: 	CMPI	' ',R2
    // asm 00009AE2: 	BEQ	J2
    if (entry.init2 == ' ') {
        goto J2;
    }
    // asm 00009AE3: 	CMPI	'0',R2
    // asm 00009AE4: 	BLT	RESETALL
    if (entry.init2 < '0') {
        goto RESETALL;
    }
    // asm 00009AE5: 	CMPI	'Z',R2
    // asm 00009AE6: 	BGT	RESETALL
    if (entry.init2 > 'Z') {
        goto RESETALL;
    }
J2:
    // asm 00009AE7: 	CMPI	' ',R3
    // asm 00009AE8: 	BEQ	J3
    if (entry.init3 == ' ') {
        goto J3;
    }
    // asm 00009AE9: 	CMPI	'0',R3
    // asm 00009AEA: 	BLT	RESETALL
    if (entry.init3 < '0') {
        goto RESETALL;
    }
    // asm 00009AEB: 	CMPI	'Z',R3
    // asm 00009AEC: 	BGT	RESETALL
    if (entry.init3 > 'Z') {
        goto RESETALL;
    }

J3:
    // asm 00009AED: 	CMPI	0,R4
    // asm 00009AEE: 	BLT	RESETALL
    if (entry.rank < 0) {
        goto RESETALL;
    }
    // asm 00009AEF: 	CMPI	10,R4
    // asm 00009AF0: 	BGT	RESETALL
    if (entry.rank > 10) {
        goto RESETALL;
    }

    // asm 00009AF1: 	CMPI	4*60*60,R0
    // asm 00009AF2: 	BGT	RESETALL
    if (entry.time > (4 * 60 * 60)) {
        goto RESETALL;
    }
    // asm 00009AF3: 	CMPI	0,R0
    // asm 00009AF4: 	BLT	RESETALL
    if (entry.time < 0) {
        goto RESETALL;
    }

    // asm 00009AF5: 	ADDI	DEFAULT_TABLE_SIZE,AR1
    // asm 00009AF6: 	INC	R7
    entry_index += 1;
    // asm 00009AF7: 	DBU	AR4,ITL2A
    if (entry_index <= 9) {
        goto ITL2A;
    }

    // asm 00009AF8: 	INC	R6
    race_index += 1;
    // asm 00009AF9: 	DBU	AR5,ITL1A
    if (race_index <= 9) {
        goto ITL1A;
    }
    // asm 00009AFA: 	RETS
    return 1;

RESETALL:
    // asm 00009AFB: 	CALL	INIT_HSTD_TABLES
    INIT_HSTD_TABLES();
    // asm 00009AFC: 	RETS
    return 0;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PARAMETERS
 *	R6	RACE NUMBER
 *	R7	ENTRY NUMBER
 *
 *RETURNS
 *	AR2	ADDRESS
 *
 */
word_addr_t GET_TABLE_ADDR(int race_index /*R6*/, int entry_index /*R7*/) {
    word_addr_t addr;

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
    addr = (word_addr_t)CMOSI + (word_addr_t)(MAX_AUDITS * 4) + (word_addr_t)(race_index * RACE_TABLE_SIZE)
        + (word_addr_t)(entry_index * sizeof(RACEENTRY));
    // MAME_VALIDATE_REG_AT_ADDR(0x00009B04, "AR2", &addr);
    return addr;
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
static void TABLE_ENTRY_WRITE(void) {
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

static void TABLE_ENTRY_WRITE0(void) {
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
RACEENTRY TABLE_ENTRY_READ(word_addr_t* addr /*AR2*/) {
    RACEENTRY entry;

    // asm 00009B31: 	CALL	_rd_cw
    entry.time = (u32)_rd_cw(*addr);
    *addr += 4;
    // asm 00009B32: 	PUSH	R0
    MAME_VALIDATE_REG_AT_ADDR(0x00009B32, "R0", &entry.time);
    // asm 00009B33: 	CMOS_ON
    // asm 00009B34: 	CMOS_WP_OFF
    // asm 00009B35: 	LDI	*AR2++,R1
    // asm 00009B36: 	RS	24,R1
    CMOS_ON_C();
    CMOS_WPOFF_C();
    entry.init1 = (crusn_mem_rd32(*addr) >> 24) & 0xffu;
    *addr += 1;
    MAME_VALIDATE_REG_AT_ADDR(0x00009B37, "R1", &entry.init1);
    // asm 00009B37: 	LDI	*AR2++,R2
    // asm 00009B38: 	RS	24,R2
    entry.init2 = (crusn_mem_rd32(*addr) >> 24) & 0xffu;
    *addr += 1;
    MAME_VALIDATE_REG_AT_ADDR(0x00009B39, "R2", &entry.init2);
    // asm 00009B39: 	LDI	*AR2++,R3
    // asm 00009B3A: 	RS	24,R3
    entry.init3 = (crusn_mem_rd32(*addr) >> 24) & 0xffu;
    *addr += 1;
    MAME_VALIDATE_REG_AT_ADDR(0x00009B3B, "R3", &entry.init3);
    // asm 00009B3B: 	LDI	*AR2++,R4
    // asm 00009B3C: 	RS	24,R4
    entry.rank = (crusn_mem_rd32(*addr) >> 24) & 0xffu;
    *addr += 1;
    MAME_VALIDATE_REG_AT_ADDR(0x00009B3D, "R4", &entry.rank);
    // asm 00009B3D: 	PUSH	R1
    // asm 00009B3E: 	CMOS_WP_ON	;This trashes register R1
    // asm 00009B3F: 	CMOS_OFF
    CMOS_WPON_C(); // ;This trashes register R1
    CMOS_OFF_C();
    // asm 00009B40: 	POP	R1
    // asm 00009B41: 	POP	R0
    // asm 00009B42: 	RETS
    return entry;
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
void CHECK_RACE_TABLE(void) {
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
void INSERT_TABLE_ENTRY(void) {
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

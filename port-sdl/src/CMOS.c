#include "NOT_IMPLEMENTED.h"
#include "TRACE.h"
#include "STUB.h"
#include "CMOS.h"

/* Scaffold generated from CMOS.ASM */

/* LASTHS table model: one entry per race, -1 means not set. */
static int g_lasths_table[14];
static int g_lasths_race;
static int g_lasths_entry;
static int g_lasths_result = -1;

void CMOS_ON_C(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CMOS_OFF_C(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CMOS_WPON_C(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CMOS_WPOFF_C(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void VALIDATE_CMOS(void) {
    TRACE();
    STUB();
}

void RESET_BOOKKEEPING(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void VERIFY_ADJUSTMENTS_ACCURACY(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RESET_ADJUSTMENTS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CHECKSUMGEN_ADJ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void AUDIT_INC(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void AUDIT_ADD(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

int ADJUSTMENT_READ(int adj_index) {
    TRACE();
    /* Default adjustment values for host build until CMOS tables are translated. */
    static const int defaults[] = {
        0,   /* ADJ_COINMODE */
        0,   /* ADJ_GASMIN */
        255, /* ADJ_GASMAX */
        0,   /* ADJ_STEERMIN */
        255, /* ADJ_STEERMAX */
        128, /* ADJ_STEERCENTER */
        0,   /* ADJ_BRAKEMIN */
        255, /* ADJ_BRAKEMAX */
    };
    if (adj_index >= 0 && adj_index < (int)(sizeof(defaults) / sizeof(defaults[0]))) {
        return defaults[adj_index];
    }
    return 0;
}

void AUDIT_READ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ADJUSTMENT_WRITE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void AUDIT_WRITE(void) {
    TRACE();
    STUB();
}

void AUDIT_WRITE_ADJ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _rd_cw(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _wr_cw(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _rd_cwR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _wr_cwR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void DEFAULT_TABLE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void DEFAULT_TABLE_TOTAL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void INIT_LASTHS_TABLE(void) {
    TRACE();
    for (int i = 0; i < 14; i++) {
        g_lasths_table[i] = -1;
    }
}

void UPDATE_LASTHS(void) {
    TRACE();
    if (g_lasths_race >= 0 && g_lasths_race < 14) {
        g_lasths_table[g_lasths_race] = g_lasths_entry;
    }
}

void CHECK_LASTHS(void) {
    TRACE();
    if (g_lasths_race >= 0 && g_lasths_race < 14) {
        g_lasths_result = g_lasths_table[g_lasths_race];
    } else {
        g_lasths_result = -1;
    }
}

void INIT_HSTD_TABLES(void) {
    TRACE();
    STUB();
}

void VALIDATE_HSTD_TABLES(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RESETALL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GET_TABLE_ADDR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TABLE_ENTRY_WRITE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TABLE_ENTRY_WRITE0(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TABLE_ENTRY_READ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CHECK_RACE_TABLE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void INSERT_TABLE_ENTRY(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

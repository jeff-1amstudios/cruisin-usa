#include "WAVE.h"

#include "GAME_STATE.h"
#include "TRACE.h"
#include "PROCESS.h"

#include "NOT_IMPLEMENTED.h"
static void WAVE_ATTRACT_ENTRY(int attr_mode) {
    TRACE();
    /* Platform shim for attract process creation in WAVE.ASM. */
    switch (attr_mode) {
        case -1: /* HIGH_SCORE */
            _MODE = MATTR | MHS;
            break;
        case -2: /* MIDSPIN */
        case -3: /* RACELEG */
        case -4: /* MIDSPINHS */
        case -5: /* THANKS */
        case -6: /* CREDITS */
        case -7: /* HEAD2HEAD_WAIT */
        default:
            _MODE = MATTR;
            break;
    }
    PLATFORM_RENDER_MODE_SCREEN();
}

void INIT_SYSTEM(void) {
    TRACE();
    /* Pending exact translation from WAVE.ASM/related init modules. */
}

void LOAD_FIXED_PALETTES(void) {
    TRACE();
    /* Pending exact palette allocator translation from WAVE.ASM. */
}

void WAVE(void) {
    TRACE();
    /* Partial exact translation of WAVE.ASM startup semantics. */

    CLR_RAM();
    COMM_ENABLE_INT2();
    INIT_SYSTEM();
    LOAD_FIXED_PALETTES();

    if (_ATTR_MODE == 1) {
        /* BEGIN_GAME in WAVE.ASM creates PLYR_INTRO process. */
        PRC_CREATE(PLYR_INTRO);
        return;
    }

    WAVE_ATTRACT_ENTRY(_ATTR_MODE);
}

void HEAD2HEADWATCH(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HEAD2HEAD_WAIT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HIGH_SCORE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void MIDSPIN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void MIDSPINHS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RACELEG(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HIGH_SCORE_INI(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void THANKS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CREDITS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void BEGIN_GAME(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void FIXEDPAL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ILLUM_PAL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void VEHICLE_TABLE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void tower_bgrey(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

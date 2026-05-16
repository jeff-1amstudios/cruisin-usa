#include "NOT_IMPLEMENTED.h"
#include "TRACE.h"
#include "STUB.h"
#include "GAME_STATE.h"
#include "WAVE.h"
#include "COMP.h"
#include "HUD.h"
#include "PROCESS.h"

void OBJ_INIT(void);
void OBJ_QMAKE(void);
void OBJ_INSERTP(void);
void OBJ_DELETE(void);
void BOILERPLATE_INIT(void);
void FIX_TRACK_SCREEN(void);
void ZOOMINP(void);
void ZOOMIN_TSEL(void);
void TRACK_SEL_CURSOR(void);
void TURNTO_SELECT(void);
void CYCLE_PUSH(void);
void ANIMATE_MAP(void);
void GETCHOICE(void);

/* Scaffold generated from TRACKSEL.ASM */
static const unsigned int crace_PALETTES[2] = {0u, 0u};
static const unsigned int shared_PALETTES[2] = {0u, 0u};
static const unsigned int ctrans_PALETTES[2] = {0u, 0u};

static uint32_t NOAERASE;
static uint32_t CLEARRDY;
static int OLD_INDEX;
static int LAST_HIDDEN_TRACK;
static uint32_t POSES;
static uint32_t RACE_MODE;
static uint32_t POSE;
static unsigned int BOILEROBJ;
static float SMALL_CURSOR_SAVED_X;
static int OM_CHOSEN_RACE = -1;
static int TSEL_HEAD;
static int SMALL_CURSOR;
static int BIG_CURSOR;
static int ROM_SMALL_CURSOR;
static int ROM_BIG_CURSOR;
static int CHOSEN_RACE;

enum {
    RM_SINGLE = 0,
};

void LOAD_SINGLE_SECTION(void) {
    TRACE();
    STUB();
}

void TRACK_SELECTION(void) {
    TRACE();
    NOAERASE = 0;
    START_HIT = 0;

    /* FIFO/DMA clear handshake is hardware-specific; keep clear-ready flow only. */
    CLEARRDY = 1;
    CLEARRDY = 0;

    LOAD_FIXED_PALETTES();

    OLD_INDEX = -1;
    LAST_HIDDEN_TRACK = -1;

    HUD_SET_SECTION_HEADER(crace_PALETTES);
    alloc_section();

    LOAD_SECTION_REQ(NULL); /* _SECcrace in ASM */
    /* HOLD4LOAD waits for DECOMP_ACTIVE; synchronous host load path returns complete. */

    HUD_SET_SECTION_HEADER(shared_PALETTES);
    dealloc_section(); /* shared_PALETTES */
    HUD_SET_SECTION_HEADER(shared_PALETTES);
    alloc_section();   /* shared_PALETTES */
    LOAD_SECTION_REQ(NULL); /* _SECshared in ASM */

    OBJ_INIT();
    BOILERPLATE_INIT();

    LOAD_SECTION_REQ(NULL); /* _SECctrans on first race path (kept unconditional for now) */
    HUD_SET_SECTION_HEADER(ctrans_PALETTES);
    alloc_section();        /* ctrans_PALETTES */

    POSES = 0; /* TRACK_SELECTION_LIST_LEN+1 in ASM */
    RACE_MODE = RM_SINGLE;

    LOAD_SINGLE_SECTION(); /* racemap_grp */
    FIX_TRACK_SCREEN();

    POSE = 6;
    ZOOMINP();
}

void MOTION_SCREWED(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CLEANUP_TRACKSEL_PALS(void) {
    TRACE();
    HUD_SET_SECTION_HEADER(crace_PALETTES);
    dealloc_section();
}

void RESTORE_HIDDEN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HIDDEN_TRACKS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GET_RACE_IDX(void) {
    TRACE();
    int r0;
    int ar3;

    GETCHOICE();

    r0 = (int)POSE;
    if (r0 > 5) {
        r0 -= 1;
    }

    /* Sondfx TRANS in ASM when changed. */
    CHOSEN_RACE = r0;

    if (LAST_HIDDEN_TRACK != -1) {
        if (r0 == 1 && LAST_HIDDEN_TRACK == 1) {
            ar3 = 11;
        } else if (r0 == 6 && LAST_HIDDEN_TRACK == 2) {
            ar3 = 12;
        } else if (r0 == 9 && LAST_HIDDEN_TRACK == 3) {
            ar3 = 13;
        } else {
            ar3 = CHOSEN_RACE;
        }
    } else {
        ar3 = CHOSEN_RACE;
    }

    /* OBJ_FIND_FIRST + text placement/draw path depends on object/text systems
       not fully translated yet; keep state updates faithful and continue flow. */
    (void)ar3;
}

void ZOOMINP(void) {
    TRACE();
    int ar4 = 20;
    do {
        ZOOMIN_TSEL();

        if (OM_CHOSEN_RACE == -1) {
            GET_RACE_IDX();
            TRACK_SEL_CURSOR();
        }

        /* SLEEP 1: platform/frame pacing lives in main loop on host port. */
        ar4--;
    } while (ar4 >= 0);
}

void ZOOMIN_TSEL(void) {
    TRACE();
    /* TRACKSEL.ASM walks OACTIVE list and eases OPOSZ toward OVELZ,
       rotating via ORADZ/FIND_ZMATRIX while objects settle.
       Exact object list/field offsets are pending OBJ layout translation. */
    int ar5 = 0; /* OACTIVE head (pending shared object-list state wiring) */

    while (ar5 != 0) {
        int ar0 = ar5;
        (void)ar0;
        ar5 = 0; /* *AR0 next */

        /* Per-object numeric update path is deferred until OBJ field mapping lands. */
    }
}

void FIX_TRACK_SCREEN(void) {
    TRACE();
    int r6 = -1;
    int ar5 = 0; /* OACTIVE head; pending shared OBJ active-list wiring. */

    TSEL_HEAD = 0;

    while (ar5 != 0) {
        const int ar0 = ar5;
        const int next = 0; /* *AR0 next; pending OBJ field map */
        int oid = 0;        /* *+AR0(OID); pending OBJ field map */

        ar5 = next;

        /* Branches follow TRACKSEL.ASM object-id dispatch. */
        if (oid == 0x040c) {
            SMALL_CURSOR = ar0;
            ROM_SMALL_CURSOR = 0; /* *+AR0(OROMDATA) */
            if (r6 == -1) {
                r6 = ar0;
            }
            continue;
        }

        if (oid == 0x08a1) {
            PRC_CREATE(CYCLE_PUSH);
            if (r6 == -1) {
                r6 = ar0;
            }
            continue;
        }

        if (oid == 0x08a2) {
            PRC_CREATE(TURNTO_SELECT);
            if (r6 == -1) {
                r6 = ar0;
            }
            continue;
        }

        if (oid == 0x0410) {
            BIG_CURSOR = ar0;
            ROM_BIG_CURSOR = 0; /* *+AR0(OROMDATA) */
            OBJ_DELETE();
            if (r6 == -1) {
                r6 = ar0;
            }
            continue;
        }

        if (oid == 0x0405) {
            PRC_CREATE(ANIMATE_MAP);
            if (r6 == -1) {
                r6 = ar0;
            }
            continue;
        }

        if (oid == 0x040e || oid == 0x040f) {
            OBJ_DELETE();
            continue;
        }

        if (r6 == -1) {
            r6 = ar0;
        }
    }

    TSEL_HEAD = r6;
}

void TSEL_ZOOMOUTP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CENTER_THEONE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CENTER_SUB1(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TSEL_ZOOMOUT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ANIMATE_MAP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TRACK_SEL_CURSOR(void) {
    TRACE();
    int ar0 = CHOSEN_RACE;
    int ar3;

    if (OLD_INDEX == ar0) {
        return;
    }
    OLD_INDEX = ar0;

    ar3 = ar0;
    if (ar3 == 5) {
        /* big cursor art path in ASM */
        (void)ROM_BIG_CURSOR;
    } else {
        /* small cursor art path in ASM */
        (void)ROM_SMALL_CURSOR;
    }

    /* ASM updates SMALL_CURSOR object romdata and position from chosen race object.
       Exact object field mapping is pending OBJ layout translation. */
}

void GET_TURNOBJ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void BOILERPLATE_INIT(void) {
    TRACE();
    /* TRACKSEL.ASM: OBJ_QMAKE(next, 0, 0, 0) then store BOILEROBJ. */
    OBJ_QMAKE();
    BOILEROBJ = 0;
}

void BOILERPLATE(void) {
    TRACE();
    /* Cursor object field copies are pending exact object-layout translation. */
    STUB();
    SMALL_CURSOR_SAVED_X = 0.0f;
    OBJ_INSERTP();
}

void BOILERPLATE_DELETE(void) {
    TRACE();
    OBJ_DELETE();
    /* Restore cursor X from saved slot once object layout is translated. */
    (void)SMALL_CURSOR_SAVED_X;
    STUB();
}

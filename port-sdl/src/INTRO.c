#include "INTRO.h"

#include "GAME_STATE.h"
#include "PROCESS.h"
#include "WAVE.h"
#include "TRACE.h"
#include "COIN.h"
#include "CMOS.h"
#include "CUSA.h"
#include "STUB.h"
#include "NOT_IMPLEMENTED.h"

static int TEASE_COUNT;
static uint32_t RACE_MODE;
static uint32_t WAS_HEAD2HEAD_ON;
static uint32_t FIRST_RACE;
static uint32_t BONUS_WAVE;
static float GAMEDIFF;
static uint32_t TUNE_IDX;
static uint32_t IN_RESET_MODE;
static uint32_t BUTTON_STATUS;
static uint32_t SCORE;
static uint32_t END_OF_GAMEP;
static uint32_t STARTSECTION;
static uint32_t CAR_CHOICE_GOTTEN;
static uint32_t CHOSEN_RACE;
static int32_t IS_HIDDEN;
static uint32_t DCALL;
static uint32_t START_NOW_P;
static uint32_t _countdown;
static float GAME_TIMER;
static uint32_t BGNDCOLA;
static uint32_t FRAMRATE;
static uint32_t TIMECLR;
static uint32_t DRONE_DISPATCH_P;
static uint32_t SCREENWIPE_DONE;
static uint32_t NEXT_STARTUP;
static uint32_t CHALLENGE_RACE;
static uint32_t POSITION;
static uint32_t CHECKPOINT_TIME_BONUS;
static uint32_t NOAERASE;
static float WHEELPWR;

enum {
    RM_SINGLE = 0,
    BUT_START = 1u << 0,
    BUT_VIEW2 = 1u << 2,
};

static void ISSUE_STARTGAME(void) {
    TRACE();
    STUB();
}
static void INIT_PEDALCHK(void) { TRACE(); STUB(); }
void GETCMOS_VALUES(void);
void PRC_KILLALL(void);
void BGD_INIT(void);
void SCREENWIPE_OPEN(void);
void OBJ_INIT(void);
void TEXT_INIT(void);
void TRACK_SELECTION(void);
void CHOOSE_TRANSMISSION(void);
void CLEANUP_TRACKSEL_PALS(void);
void CHOOSECAR(void);
void WAVEFLAG(void);
void INIT_GAMELEG(void);

static int GAME_AVAILABLEP(void) {
    TRACE();
    /* Non-platform game-state check is not translated yet; keep flow moving in host build. */
    STUB();
    return 1;
}

void PLYR_INTRO(void) {
    TRACE();
    /* INTRO.ASM PLYR_INTRO */
    RACE_MODE = RM_SINGLE;
    _MODE = MINTRO | MGO;
    WAS_HEAD2HEAD_ON = 0;
    FIRST_RACE = 1;

    ISSUE_STARTGAME(); /* JSRP ISSUE_STARTGAME */

    INIT_LASTHS_TABLE();
    /* SETADJ(ADJ_INITIALS,-1), INCAUD(...), HSTDEC are not translated yet. */
    STUB();

    BONUS_WAVE = 0;
    GAMEDIFF = 1.0f;

    {
        uint32_t unfinished = 0;       /* READAUD(AUD_UNFINISHED_GAMES) */
        uint32_t unfinished_found = 0; /* READAUD(AUD_UNFINISHED_GAMES_FOUND) */
        STUB();
        if (unfinished_found != unfinished) {
            /* SETAUD(AUD_UNFINISHED_GAMES_FOUND, ...) */
            STUB();
        }
    }

    /* INCAUD(AUD_NUM_UNFINISHED) */
    STUB();
    TUNE_IDX = 1;

CNR_ENTER:
    if (IN_RESET_MODE != 0) {
        PRC_CREATE(PLYR_INTRO);
        return;
    }

    /* SOND1(START_THEME) */
    STUB();

    BUTTON_STATUS = BUT_START;

    SCORE = 0;
    END_OF_GAMEP = 0;
    STARTSECTION = 0;
    CAR_CHOICE_GOTTEN = 0;
    CHOSEN_RACE = 0;
    IS_HIDDEN = -1;

    GETCMOS_VALUES();
    INIT_PEDALCHK();
    OBJ_INIT();
    TEXT_INIT();

    TRACK_SELECTION(); /* JSRP */
    if (FIRST_RACE == 0) {
        goto LOAD_NEW_SELECTION;
    }

    INIT_PEDALCHK();
    OBJ_INIT();
    TEXT_INIT();
    DCALL = 0;

    CHOOSE_TRANSMISSION();
    DCALL = 1;
    CHOOSECAR();

    BUTTON_STATUS = BUT_VIEW2;

    if (START_NOW_P == 0) {
        PRC_CREATE(PLYR_INTRO);
        return;
    }

ALL_JOINUP:
    _MODE |= MINFIN;

    STUB(); /* READAUD(ADJ_TIME_TO_START) */
    _countdown = 60u;

    WAVEFLAG();
    CLEANUP_TRACKSEL_PALS();
    PRC_KILLALL();

    GAME_TIMER = 0.0f;
    /* INCAUD(AUD_NUM_BUYINS) */
    STUB();

    BGNDCOLA = 0xAAu;
    FRAMRATE = 1;
    TIMECLR = 1;
    DRONE_DISPATCH_P = 1;

    SCREENWIPE_DONE = 0;
    NEXT_STARTUP = 0;
    CHALLENGE_RACE = 0;
    POSITION = 9;
    STUB(); /* READAUD(ADJ_CHECKPOINT_BONUS) */
    CHECKPOINT_TIME_BONUS = 0;
    NOAERASE = 1;

    if (FIRST_RACE == 0) {
        /* branch to _PLYR */
        return;
    }

    /* SETAUD(AUD_LAST_LEG,0) */
    STUB();
    WHEELPWR = 1.0f;
    return; /* DIE */

LOAD_NEW_SELECTION:
    _MODE = MINTRO | MINFIN;
    STARTSECTION = 0;
    BGD_INIT();
    INIT_GAMELEG();
    _MODE = MGAME;
    SCREENWIPE_OPEN();
    _MODE = MGAME | MHUD | MINFIN;
    /* Original does CALLU via BONUS_POSTLAUNCH table entry for selected race. */
    NOT_IMPLEMENTED();
    BUTTON_STATUS = BUT_VIEW2;
    goto ALL_JOINUP;
}

void _start(void) {
    TRACE();
    const uint32_t mode = (_MODE & MMODE);

    /* INTRO.ASM: in these modes, START only latches START_HIT and exits. */
    if (mode == MCT || mode == MBONUS || mode == MINSERT_COINS || mode == MINIT || mode == MINTRO) {
        START_HIT = 1;
        return;
    }

    /* INTRO.ASM _startX: ignore START while already in gameplay. */
    if (mode == MGAME) {
        return;
    }

    if (!GAME_AVAILABLEP()) {
        if (TEASE_COUNT <= 0) {
            TEASE_COUNT++;
            /* Platform boundary: teaser sound call not yet wired. */
        }
        return;
    }

    (void)GET_CREDITS_TO_START();
    AUDIT_WRITE();

    _MODE = (_MODE & ~MMODE) | MINTRO;
    _ATTR_MODE = 1;
    WAVE();
    COLD_ENTER();
}

void WAVEFLAG(void) {
    TRACE();
    /* Transition marker in original flow; detailed behavior pending. */
}

void INIT_GAMELEG(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CHOOSECAR(void) {
    TRACE();
    /* In original flow this is set after chooser/zoom sequence completes. */
    START_NOW_P = 1;
}

void HEAD2HEAD_LOGO_WAIT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HEAD2HEAD_LOGO(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void KABOSHP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void JINMSG(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void THROBIT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void WAIT_FOR_CHALLENGER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CHECK_ENDBONUS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void WAIT_FOR_ENDBONUS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ISSUE_STARTGAME_TSEL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CHOOSE_NEXT_RACE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void WATCH_PLYRS_CAR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void THE_CAR_CHOICE_PROC(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RAISE_DOOR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ZOOMTOCAR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETTHECARS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SHOW_CAR_STATISTICS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CLEANUP_DIMCAR_PALS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETTHECAR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ROUNDER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void AFFECT_THE_CARS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HIDDEN_VEHICLES(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RESET_ORIGINAL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void AFFECTED_CAR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CAR_DIMMER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETCHOICE(void) {
    TRACE();
    /* INTRO.ASM GETCHOICE:
       wheel-position hysteresis over POSE/POSES using steer calibration.
       Shared steering/pose globals are not fully centralized yet, so keep
       non-aborting behavior until those globals are wired. */
}

void PEDALCHK(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RACESEL_TIMER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void WAITINTROTIMER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void INTROTIMER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void DIAL_ROUT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ULTRA_PROC(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ULTRA_LOGO(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SET_ATTR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CYCLE_ATTR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _debug(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _timeout(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void INSMORE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void COIN_CNTDOWN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TRAFFIC_LIGHT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CPOINT_LIGHT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SHOW_RACE_NAME(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

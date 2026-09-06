
#include "tracksel.h"
#include "../core/machine.h"
#include "cmos.h"
#include "comm.h"
#include "comp.h"
#include "delta.h"
#include "error.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "objects.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/TRACKSEL.ASM
 */

void TRACK_SELECTION(PROC* p);
void MOTION_SCREWED(PROC* p);
static void JAJAD__tail(PROC* p);
void CLEANUP_TRACKSEL_PALS(void);
static void RESTORE_HIDDEN(int hidden_index);
static void HIDDEN_TRACKS(PROC* p);
static void GET_RACE_IDX(PROC* p);
static void ZOOMINP(PROC* p);
static void ZOOMIN_TSEL(void);
static void FIX_TRACK_SCREEN(PROC* p);
static void TSEL_ZOOMOUTP(PROC* p);
static void CENTER_THEONE(PROC* p);
static void CENTER_SUB1(OBJ* obj);
static void TSEL_ZOOMOUT(void);
static void ANIMATE_MAP(PROC* p);
static void TRACK_SEL_CURSOR(PROC* p);
static void GET_TURNOBJ(PROC* p);
void BOILERPLATE_INIT(void);
static void BOILERPLATE(void);
static void BOILERPLATE_DELETE(void);

#define ANIMATE_MAP_TABI ANIMATE_MAP_TAB
#define TRACK_SELECTION_LISTI TRACK_SELECTION_LIST
#define TRACK_SELECTION_LIST_LEN 11

static int TRACK_SELECTION_LIST[14];

/* asm: TURNOBJ	.bss	TURNOBJ,1 */
static int TURNOBJ;
/*
 *----------------------------------------------------------------------------
 *
 *
 *
 */
#define SMALL_CURSOR (PDATA + 0)
#define BIG_CURSOR (PDATA + 1)
#define OLD_INDEX (PDATA + 2)
#define ROM_SMALL_CURSOR (PDATA + 3)
#define ROM_BIG_CURSOR (PDATA + 4)
#define LAST_HIDDEN_TRACK (PDATA + 5)
/* asm: RAW_CHOOSENTRK	.bss	RAW_CHOOSENTRK,1 */
int RAW_CHOOSENTRK;
/* asm: HIDDEN_ON	.bss	HIDDEN_ON,1 */
int HIDDEN_ON;
/* asm: CRUISE	SPTR	"CRUISE THE USA" */
static const char CRUISE[] = "CRUISE THE USA";
/* asm: CRUISE2	SPTR	"JAM WITH THE PRESIDENT" */
static const char CRUISE2[] = "JAM WITH THE PRESIDENT";
/* asm: CRUISE3	SPTR	"IN THE WHITE HOUSE HOT TUB" */
static const char CRUISE3[] = "IN THE WHITE HOUSE HOT TUB";
static const char MOTION_RECALIBRATION[] = "MOTION RECALIBRATION";
static const char REMAIN_SEATED[] = "REMAIN SEATED";
static const char DO_NOT_TOUCH_MATS[] = "DO NOT TOUCH MATS";

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *
 */
void TRACK_SELECTION(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    case 4:
        goto PROC_RESUME_4;
    case 5:
        goto PROC_RESUME_5;
    }

    // asm 00006CBA: 	CLRI	R0
    NOAERASE = 0;
    // asm 00006CBB: 	STI	R0,@NOAERASE
    // asm 00006CBC: 	STI	R0,@START_HIT
    START_HIT = 0;
    // asm 00006CBD: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 00006CC2: 	DMA_WT		R0
    // asm 00006CC7: 	LDI	1,R0
    // asm 00006CC8: 	STI	R0,@CLEARRDY	  	;READY FOR INTERRUPT
    CLEARRDY = 1; // READY FOR INTERRUPT
KK5:
    // asm 00006CC9: LDI	@CLEARRDY,R0
    // asm 00006CCA: 	BNZ	KK5
    /* The original interrupt runs concurrently and clears CLEARRDY. The
       portable main loop delivers that interrupt after this process yields;
       spinning here would prevent it from ever running. */
    // ;	CALL	PAL_INIT
    // asm 00006CCB: 	CALL	LOAD_FIXED_PALETTES
    LOAD_FIXED_PALETTES();
    // asm 00006CCC: 	LDI	-1,R0
    // asm 00006CCD: 	STI	R0,*+AR7(OLD_INDEX)
    p->ctx.TRACK_SELECTION.old_index = -1;
    // asm 00006CCE: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    p->ctx.TRACK_SELECTION.last_hidden_track = -1;
    // ;	CALL	CLEANUP_PALS
    // asm 00006CCF: 	LDL	crace_PALETTES,AR2
    // asm 00006CD0: 	CALL	alloc_section
    alloc_section(crace_PALETTES);
    // asm 00006CD1: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00006CD3: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 00006CD8: 	DMA_WT		R0
    // asm 00006CDD: 	CALL	FIFO_RESET
    FIFO_RESET();
    // asm 00006CDE: 	LDL	_SECcrace,AR2
    // asm 00006CDF: 	CALL	LOAD_SECTION_REQ
    LOAD_SECTION_REQ(&SECcrace_TRACKSEL);
HOLD4LOAD:
    // asm 00006CE0: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00006CE2: 	LDI	@DECOMP_ACTIVE,R0
    // asm 00006CE3: 	BNZ	HOLD4LOAD
    if (DECOMP_ACTIVE != 0) {
        goto HOLD4LOAD;
    }
    // asm 00006CE4: 	LDL	shared_PALETTES,AR2
    // asm 00006CE5: 	CALL	dealloc_section
    dealloc_section(shared_PALETTES);
    // asm 00006CE6: 	LDL	shared_PALETTES,AR2
    // asm 00006CE7: 	CALL	alloc_section
    alloc_section(shared_PALETTES);
    // asm 00006CE8: 	LDL	_SECshared,AR2
    // asm 00006CE9: 	CALL	LOAD_SECTION_REQ
    LOAD_SECTION_REQ(&SECshared);
    // asm 00006CEA: 	CALL	OBJ_INIT
    OBJ_INIT();
    // asm 00006CEB: 	CALL	BOILERPLATE_INIT
    BOILERPLATE_INIT();
    // asm 00006CEC: 	LDI	@FIRST_RACE,R0
    // asm 00006CED: 	BZ	NOT_RACE1
    if (FIRST_RACE == 0) {
        goto NOT_RACE1;
    }
    // asm 00006CEE: 	LDL	_SECctrans,AR2
    // asm 00006CEF: 	CALL	LOAD_SECTION_REQ
    LOAD_SECTION_REQ(&SECctrans_TRACKSEL);
    // asm 00006CF0: 	LDL	ctrans_PALETTES,AR2
    // asm 00006CF1: 	CALL	alloc_section
    alloc_section(ctrans_PALETTES);
NOT_RACE1:
    // asm 00006CF2: 	LDP	@_CAMERAPOS
    // asm 00006CF3: 	CLRF	R0
    // asm 00006CF4: 	STF	R0,@_CAMERAPOS+X
    _CAMERAPOS.X = C3X_STF(C3X_FROM_INT(0));
    // asm 00006CF5: 	STF	R0,@_CAMERAPOS+Y
    _CAMERAPOS.Y = C3X_STF(C3X_FROM_INT(0));
    // asm 00006CF6: 	FLOAT	-368,R0
    // asm 00006CF7: 	STF	R0,@_CAMERAPOS+Z
    _CAMERAPOS.Z = C3X_STF(C3X_FROM_INT(-368));
    // asm 00006CF8: 	SETDP
    // asm 00006CF9: 	LDI	TRACK_SELECTION_LIST_LEN+1,R0
    // asm 00006CFA: 	STI	R0,@POSES
    POSES = TRACK_SELECTION_LIST_LEN + 1;
    // asm 00006CFB: 	LDI	RM_SINGLE,R0
    // asm 00006CFC: 	STI	R0,@RACE_MODE
    RACE_MODE = RM_SINGLE;
    // asm 00006CFD: 	LDL	racemap_grp,AR2
    // asm 00006CFE: 	CALL	LOAD_SINGLE_SECTION
    LOAD_SINGLE_SECTION((LOAD_SINGLE_SECTION_GROUP*)ROM_PTR(racemap_grp_ROM));
    // asm 00006CFF: 	CALL	FIX_TRACK_SCREEN
    FIX_TRACK_SCREEN(p);
    // asm 00006D00: 	SONDFX	VIEW2IN
    ONESNDFX(VIEW2IN);
    // asm 00006D02: 	LDI	6,R0
    // asm 00006D03: 	STI	R0,@POSE
    POSE = 6;
    // asm 00006D04: 	JSRP	ZOOMINP
    JSRP(ZOOMINP, 3);
    // asm 00006D0A: 	READAUD	AUD_RESET_TOTALLY
    // asm 00006D0C: 	CMPI	0,R0
    // asm 00006D0D: 	BEQ	JAJAD
    if (READAUD(AUD_RESET_TOTALLY) == 0) {
        PROC_CONTINUE(JAJAD__tail, 4);
        return;
    }
    PROC_CONTINUE(MOTION_SCREWED, 5);
    return;
}

void MOTION_SCREWED(PROC* p) {
    tSHADOW_TEXT text;

    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

LLG1:
    // asm 00006D0E: .word	L4A
LLG2:
    // asm 00006D0E: .word	L5A
LLG3:
    // asm 00006D0E: .word	L5B
L88:
    // asm 00006D0E: .word	IBOIBOJ
    // asm 00006D0E: 	CALL	OBJ_INIT
    OBJ_INIT();
    // asm 00006D0F: 	LDI	10,AR5
    p->ctx.TRACK_SELECTION.motion_recalibration = 10;
LLKJ:
    // asm 00006D10: LLKJ
    // asm 00006D10: 	LDI	@LLG1,AR2
    // asm 00006D11: 	FLOAT	256,R2
    // asm 00006D12: 	FLOAT	50,R3
    // asm 00006D13: 	LDI	60,RC
    // asm 00006D14: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(MOTION_RECALIBRATION, C3X_FROM_INT(256), C3X_FROM_INT(50), 60);
    // asm 00006D15: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    // asm 00006D18: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.shadow->color |= TXT_CENTER;
    // asm 00006D1B: 	CALL	SET40FONTDS
    SET40FONTDS(&text);
    // asm 00006D1C: 	LDI	@LLG2,AR2
    // asm 00006D1D: 	FLOAT	256,R2
    // asm 00006D1E: 	FLOAT	120,R3
    // asm 00006D1F: 	LDI	60,RC
    // asm 00006D20: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(REMAIN_SEATED, C3X_FROM_INT(256), C3X_FROM_INT(120), 60);
    // asm 00006D21: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    // asm 00006D24: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.shadow->color |= TXT_CENTER;
    // asm 00006D27: 	CALL	SET40FONTDS
    SET40FONTDS(&text);
    // asm 00006D28: 	LDI	@LLG3,AR2
    // asm 00006D29: 	FLOAT	256,R2
    // asm 00006D2A: 	FLOAT	190,R3
    // asm 00006D2B: 	LDI	60,RC
    // asm 00006D2C: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(DO_NOT_TOUCH_MATS, C3X_FROM_INT(256), C3X_FROM_INT(190), 60);
    // asm 00006D2D: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    // asm 00006D30: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.shadow->color |= TXT_CENTER;
    // asm 00006D33: 	CALL	SET40FONTDS
    SET40FONTDS(&text);
    // asm 00006D34: 	LDI	AR5,R2
    // asm 00006D35: 	LDI	@L88,AR2
    // asm 00006D36: 	CALL	_itoa
    _itoa(p->ctx.TRACK_SELECTION.motion_countdown,
        p->ctx.TRACK_SELECTION.motion_recalibration);
    // asm 00006D37: 	LDI	@L88,AR2
    // asm 00006D38: 	FLOAT	256,R2
    // asm 00006D39: 	FLOAT	260,R3
    // asm 00006D3A: 	LDI	60,RC
    // asm 00006D3B: 	CALL	TEXT_ADDDS
    text = TEXT_ADDDS(p->ctx.TRACK_SELECTION.motion_countdown,
        C3X_FROM_INT(256), C3X_FROM_INT(260), 60);
    // asm 00006D3C: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text.front->color |= TXT_CENTER;
    // asm 00006D3F: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    text.shadow->color |= TXT_CENTER;
    // asm 00006D42: 	CALL	SETN43FONTDS
    SETN43FONTDS(&text);
    // asm 00006D43: 	SONDFX	BASICBEEPH
    ONESNDFX(BASICBEEPH);
    // asm 00006D45: 	SLEEP	60
    SLEEP(60, 1);
    // asm 00006D47: 	DBU	AR5,LLKJ
    if (--p->ctx.TRACK_SELECTION.motion_recalibration >= 0)
        goto LLKJ;
    // asm 00006D48: 	SLEEP	70
    SLEEP(70, 2);
    // asm 00006D4A: 	BU	DIAG_RETURN
    _c_int00();
    return;
}

static void JAJAD__tail(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_ORDERING("JAJAD");
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    case 4:
        goto PROC_RESUME_4;
    case 5:
        goto PROC_RESUME_5;
    case 6:
        goto PROC_RESUME_6;
    case 7:
        goto PROC_RESUME_7;
    case 8:
        goto PROC_RESUME_8;
    }

    // 	;******
    // 	;******
    // 	;******  NORMAL GAME MODE...
    // 	;******
    // 	;******
    // asm 00006D4B: 	LDI	@FIRST_RACE,R0
    // asm 00006D4C: 	BNZ	NOBUYIN77
    if (FIRST_RACE != 0)
        goto NOBUYIN77;
    // asm 00006D4D: 	LDI	@DID_TIMED_OUT,R0		;IF TIMED OUT THEN YOU MAY NOT GET A FREE RACE
    // asm 00006D4E: 	BNZ	NOT1ST
    if (DID_TIMED_OUT != 0)
        goto NOT1ST;
    // asm 00006D4F: 	LDI	@POSITION,R0
    // asm 00006D50: 	CMPI	1,R0
    // asm 00006D51: 	BNE	NOT1ST
    if (POSITION != 1)
        goto NOT1ST;
    // asm 00006D52: 	READAUD	ADJ_FREEGAME
    // asm 00006D54: 	CMPI	0,R0
    // asm 00006D55: 	BEQ	NOT1ST
    if (READAUD(ADJ_FREEGAME) == 0)
        goto NOT1ST;
    // asm 00006D56: 	LDI	@BONUS_WAVE,AR2
    // asm 00006D57: 	DEC	AR2
    // asm 00006D58: 	ADDI	AUD_WIN_GGATE,AR2
    // asm 00006D59: 	CALL	AUDIT_INC
    AUDIT_INC(AUD_WIN_GGATE + BONUS_WAVE - 1);
    // asm 00006D5A: 	INCAUD	AUD_GAMES_CONTINUES
    INCAUD(AUD_GAMES_CONTINUES);
    // asm 00006D5C: 	INCAUD	AUD_TOTAL_FREEGAMES
    INCAUD(AUD_TOTAL_FREEGAMES);
    // asm 00006D5E: 	CALL	HSTDEC
    HSTDEC();
    // asm 00006D5F: 	BU	TOJOIN
    goto TOJOIN;
    // 	;Has the player made Pole Position?  (1)
    // 	;T-> Branch over this Coin Grab
    // 	;F-> Get more coin to continue
    // 	;
NOT1ST:
    // asm 00006D60: 	CALL	BOILERPLATE
    BOILERPLATE();
    // asm 00006D61: 	JSRP	INSMORE
    JSRP(INSMORE, 1);
    // asm 00006D67: 	CALL	BOILERPLATE_DELETE
    BOILERPLATE_DELETE();
    // asm 00006D68: 	CMPI	1,AR6
    // asm 00006D69: 	BNE	ENDPLAYER
    if (p->ctx.INSMORE_FRAME.result != 1) {
        ENDPLAYER();
        return;
    }
    // asm 00006D6A: 	INCAUD	AUD_GAMES_CONTINUES
    INCAUD(AUD_GAMES_CONTINUES);
    // asm 00006D6C: 	CALL	HSTDEC
    HSTDEC();
TOJOIN:
    // asm 00006D6D: 	JSRP	ISSUE_STARTGAME_TSEL
    JSRP(ISSUE_STARTGAME_TSEL, 2);
NOBUYIN77:
    // asm 00006D73: 	LDI	@_MODE,R0
    // asm 00006D74: 	ANDN	MBRIDGE|MINTUNNEL|MHUD|MWATER|MINFIN|MHS|MSLINE|MMODE,R0
    // asm 00006D75: 	OR	MINTRO|MGO,R0
    // asm 00006D76: 	STI	R0,@_MODE
    _MODE = (_MODE & ~(MBRIDGE | MINTUNNEL | MHUD | MWATER | MINFIN | MHS | MSLINE | MMODE)) | MINTRO | MGO;
    // asm 00006D77: 	LDI	12,R0
    // asm 00006D78: 	STI	R0,@_countdown
    _countdown = 12;
    // asm 00006D79: 	CALL	INIT_PEDALCHK
    INIT_PEDALCHK(&p->ctx.pedal_released);
    // asm 00006D7A: 	CLRI	R0
    // asm 00006D7B: 	STI	R0,@START_HIT
    START_HIT = 0;
    // asm 00006D7C: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00006D7D: 	BZ	PRAPA
    if (HEAD2HEAD_ON == 0)
        goto PRAPA;
    // asm 00006D7E: 	LDI	@OM_CHOSEN_RACE,R0
    // asm 00006D7F: 	CMPI	-1,R0
    // asm 00006D80: 	BNE	PRAJHFS
    if (OM_CHOSEN_RACE != -1)
        goto PRAJHFS;
PRAPA:
    // asm 00006D81: 	LDI	8,AR5
    p->ctx.TRACK_SELECTION.loop_count = 8;
BABO:
    // asm 00006D82: BABO
    // asm 00006D82: 	CALL	GET_RACE_IDX
    GET_RACE_IDX(p);
    // asm 00006D83: 	CALL	TRACK_SEL_CURSOR
    TRACK_SEL_CURSOR(p);
    // asm 00006D84: 	SLEEP	1
    SLEEP(1, 3);
    // asm 00006D86: 	DBU	AR5,BABO
    if (--p->ctx.TRACK_SELECTION.loop_count >= 0)
        goto BABO;
PRAJHFS:
    // asm 00006D87: 	CLRI	R0
    // asm 00006D88: 	STI	R0,@START_HIT
    START_HIT = 0;
    // 	;*************************************************************
    // 	;*************************************************************
    // 	;SELECT TRACK MAIN LOOP
    // 	;*************************************************************
    // 	;*************************************************************
TRACK_SEL_LP:
    // 	;are we linked?
    // asm 00006D89: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00006D8A: 	BZ	NOWAY
    if (HEAD2HEAD_ON == 0)
        goto NOWAY;
    // 	;has the other player chosen a game
    // 	;
    // asm 00006D8B: 	LDI	@OM_CHOSEN_RACE,R0
    // asm 00006D8C: 	CMPI	-1,R0
    // asm 00006D8D: 	BEQ	NOWAY
    if (OM_CHOSEN_RACE == -1)
        goto NOWAY;
    // asm 00006D8E: 	LDI	@OM_HIDDEN_ON,R1
    // asm 00006D8F: 	STI	R1,*+AR7(LAST_HIDDEN_TRACK)
    p->ctx.TRACK_SELECTION.last_hidden_track = OM_HIDDEN_ON;
    // asm 00006D90: 	STI	R0,@CHOSEN_RACE
    CHOSEN_RACE = OM_CHOSEN_RACE;
    // asm 00006D91: 	LDI	@OM_RACE_MODE,R0
    // asm 00006D92: 	STI	R0,@RACE_MODE
    RACE_MODE = OM_RACE_MODE;
    // asm 00006D93: 	BU	TSLPX
    goto TSLPX;
NOWAY:
    // asm 00006D94: 	LDI	@START_HIT,R0
    // asm 00006D95: 	BNZ	TSLPX
    if (START_HIT != 0)
        goto TSLPX;
    // asm 00006D96: 	CALL	HIDDEN_TRACKS
    HIDDEN_TRACKS(p);
    // asm 00006D97: 	CALL	GET_RACE_IDX
    GET_RACE_IDX(p);
    // asm 00006D98: 	CALL	TRACK_SEL_CURSOR
    TRACK_SEL_CURSOR(p);
    // 	;-------time remaining
    // 	;
    // asm 00006D99: 	LDL	time,AR2
    // asm 00006D9A: 	LDI	242,R2			;R2	POS X
    // asm 00006D9B: 	LDI	240,R3			;R3	POS Y
    // asm 00006D9C: 	LDI	TM|ZS,R4
    // asm 00006D9D: 	CALL	BLTMOD2D
    // asm 00006D9E: 	LDI	@CRUISE,AR2
    // asm 00006D9F: 	FLOAT	256,R2
    // asm 00006DA0: 	FLOAT	30,R3
    // asm 00006DA1: 	LDI	1,RC
    // asm 00006DA2: 	CALL	TEXT_ADDDS
    tSHADOW_TEXT title = TEXT_ADDDS(CRUISE, C3X_FROM_INT(256), C3X_FROM_INT(30), 1);
    // asm 00006DA3: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    title.front->color |= TXT_CENTER;
    // asm 00006DA6: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    title.shadow->color |= TXT_CENTER;
    // 	;HEAD2HEAD
    // 	;
    // 	;are we in linked game?
    // 	;	F -> continue
    // 	;has the other machine chosen the game?
    // 	;	T->  accept the game
    // 	;
    // asm 00006DA9: 	CALL	PEDALCHK
    if (PEDALCHK(&p->ctx.pedal_released))
        goto TSLPX;
    // asm 00006DAA: 	BC	TSLPX
    // asm 00006DAB: 	CALL	RACESEL_TIMER
    RACESEL_TIMER();
    // asm 00006DAC: 	SLEEP	1
    SLEEP(1, 4);
    // asm 00006DAE: 	LDI	@_countdown,R0
    // asm 00006DAF: 	BGT	TRACK_SEL_LP
    if (_countdown > 0)
        goto TRACK_SEL_LP;
TSLPX:
    // 	;HEAD2HEAD
    // 	;
    // 	;NOW TELL OTHER MACHINE:
    // 	;
    // 	;	that we are finished with selecting the race
    // 	;	if we are subordinate dont???
    // 	;
    // 	;
    // asm 00006DB0: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006DB1: 	STI	R0,@HIDDEN_ON
    HIDDEN_ON = p->ctx.TRACK_SELECTION.last_hidden_track;
    // asm 00006DB2: 	CALL	SEND_RACENUM
    SEND_RACENUM();
    // 	;IF we are a slave, then we must wait
    // 	;for master to tell us what we chose
    // asm 00006DB3: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00006DB4: 	BZ	NOTYETLNKD
    if (HEAD2HEAD_ON == 0)
        goto NOTYETLNKD;
    // asm 00006DB5: 	LDI	@DIPRAM,R0
    // asm 00006DB6: 	TSTB	CMDP_MASTER,R0
    // asm 00006DB7: 	BZ	NOTYETLNKD	;actually: is master therefore IGNORE
    if ((DIPRAM & CMDP_MASTER) == 0)
        goto NOTYETLNKD;
    // asm 00006DB8: 	LDI	20,AR5
    p->ctx.TRACK_SELECTION.feedback_count = 20;
FEEDBACKLP:
    // asm 00006DB9: 	SLEEP	1
    SLEEP(1, 5);
    // asm 00006DBB: 	DEC	AR5
    p->ctx.TRACK_SELECTION.feedback_count--;
    // asm 00006DBC: 	CMPI	0,AR5
#if CDEBUG
    // asm: 	BEQ	$
#endif
    // asm 00006DBD: 	BEQ	NOTYETLNKD
    if (p->ctx.TRACK_SELECTION.feedback_count == 0)
        goto NOTYETLNKD;
    // asm 00006DBE: 	LDI	@OM_CHOSEN_RACE,R0
    // asm 00006DBF: 	CMPI	-1,R0
    // asm 00006DC0: 	BEQ	FEEDBACKLP
    if (OM_CHOSEN_RACE == -1)
        goto FEEDBACKLP;
    // asm 00006DC1: 	LDI	@OM_HIDDEN_ON,R1
    // asm 00006DC2: 	STI	R1,*+AR7(LAST_HIDDEN_TRACK)
    p->ctx.TRACK_SELECTION.last_hidden_track = OM_HIDDEN_ON;
    // asm 00006DC3: 	LDI	@OM_CHOSEN_RACE,R0
    // asm 00006DC4: 	STI	R0,@CHOSEN_RACE
    CHOSEN_RACE = OM_CHOSEN_RACE;
    // asm 00006DC5: 	LDI	@OM_RACE_MODE,R0
    // asm 00006DC6: 	STI	R0,@RACE_MODE
    RACE_MODE = OM_RACE_MODE;
NOTYETLNKD:
    // 	;THIS IS NOW MEANINGLESS
    // 	;CLEAR THIS FOR FUTURE USE
    // 	;
    // asm 00006DC7: 	CLRI	R0
    // asm 00006DC8: 	STI	R0,@OM_HIDDEN_ON
    OM_HIDDEN_ON = 0;
    // 	;\\\  \\\  \\\  \\\  \\\  \\\
    // 	;    \\\  \\\  \\\  \\\  \\\  \\\
    // asm 00006DC9: 	SONDFX	TRANS
    ONESNDFX(TRANS);
    // ;	;HEAD2HEAD
    // ;	;
    // ;	;NOW TELL OTHER MACHINE:
    // ;	;
    // ;	;	that we are finished with selecting the race
    // ;	;	if we are subordinate dont???
    // ;	;
    // ;	;
    // ;	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // ;	STI	R0,@HIDDEN_ON
    // ;	CALL	SEND_RACENUM
    // 	;delete cursor
    // 	;
    // asm 00006DCB: 	LDI	*+AR7(SMALL_CURSOR),AR4
    // asm 00006DCC: 	LDI	*+AR4(OLINK2),AR5
    // asm 00006DCD: 	LDI	@TSEL_HEAD,R0
    // asm 00006DCE: 	BU	LKGGA
JJKKA:
    // asm 00006DCF: LDI	*+AR0(OLINK2),R0
#if DEBUG
    // asm: 	BZ	$
#endif
    // asm 00006DD0: 	BZ	ALLDNA
LKGGA:
    // asm 00006DD1: LDI	R0,AR0
    // asm 00006DD2: 	CMPI	*+AR0(OLINK2),AR4
    // asm 00006DD3: 	BNE	JJKKA
    // asm 00006DD4: 	STI	AR5,*+AR0(OLINK2)
    if (p->ctx.TRACK_SELECTION.small_cursor != NULL) {
        OBJ* link_obj = TSEL_HEAD;
        while (link_obj != NULL && (OBJ*)link_obj->link2 != p->ctx.TRACK_SELECTION.small_cursor) {
            link_obj = (OBJ*)link_obj->link2;
        }
        if (link_obj != NULL)
            link_obj->link2 = p->ctx.TRACK_SELECTION.small_cursor->link2;
    }
ALLDNA:
    // asm 00006DD5: 	LDI	*+AR7(SMALL_CURSOR),AR2
    // asm 00006DD6: 	CALL	OBJ_DELETE
    if (p->ctx.TRACK_SELECTION.small_cursor != NULL) {
        OBJ_DELETE(p->ctx.TRACK_SELECTION.small_cursor);
        p->ctx.TRACK_SELECTION.small_cursor = NULL;
    }
    // 	;********************************************************************
    // 	;WAIT FOR DUAL PLAYER GAME HERE
    // 	;except with the first game
    // 	;
    // 	;********************************************************************
    // 	;
    // asm 00006DD7: 	LDI	@FIRST_RACE,R0
    // asm 00006DD8: 	BNZ	KAKAF
    if (FIRST_RACE != 0)
        goto KAKAF;
    // asm 00006DD9: 	JSRP	WAIT_FOR_CHALLENGER
    JSRP(WAIT_FOR_CHALLENGER, 6);
KAKAF:
    // 	;
    // 	;********************************************************************
    // 	;********************************************************************
    // 	;OM race mode == RM_SINGLE <=>
    // 	;	not racing across the USA already
    // 	;else
    // 	;	we have bought into an ongoing
    // 	;	race game
    // asm 00006DDF: 	LDI	@RACE_MODE,R0
    // asm 00006DE0: 	CMPI	RM_USA,R0
    // asm 00006DE1: 	BEQ	BOUGHRAU
    if (RACE_MODE == RM_USA)
        goto BOUGHRAU;
    // ;	BNE	NBIRAU
    // ;	BU	NOHID1
    // ;NBIRAU
    // asm 00006DE2: 	LDI	@CHOSEN_RACE,R0
    // asm 00006DE3: 	CMPI	5,R0
    // asm 00006DE4: 	LDIEQ	RM_USA,R1
    // asm 00006DE5: 	LDINE	RM_SINGLE,R1
    // asm 00006DE6: 	STI	R1,@RACE_MODE
    RACE_MODE = CHOSEN_RACE == 5 ? RM_USA : RM_SINGLE;
    // asm 00006DE7: 	CMPI	RM_USA,R1
    // asm 00006DE8: 	BNE	NOTUSA_RACE
    if (RACE_MODE != RM_USA)
        goto NOTUSA_RACE;
    // asm 00006DE9: 	INCAUD	AUD_START_GGATE
    INCAUD(AUD_START_GGATE);
    // asm 00006DEB: 	INCAUD	AUD_FS_USA
    INCAUD(AUD_FS_USA);
    // asm 00006DED: 	LDI	0,AR2
    // asm 00006DEE: 	BU	JJ2
    int translated_race = 0;
    goto JJ2;
NOTUSA_RACE:
    // 	;Translate race index into an actual
    // 	;useable value.
    // 	;
    // 	;
    // asm 00006DEF: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006DF0: 	STI	R0,@HIDDEN_ON
    HIDDEN_ON = p->ctx.TRACK_SELECTION.last_hidden_track;
    // asm 00006DF1: 	CMPI	-1,R0
    // asm 00006DF2: 	BEQ	NO_HID1
    if (HIDDEN_ON == -1)
        goto NO_HID1;
    // asm 00006DF3: 	LDI	@CHOSEN_RACE,AR2
    // asm 00006DF4: 	STI	AR2,@RAW_CHOOSENTRK
    RAW_CHOOSENTRK = CHOSEN_RACE;
    // asm 00006DF5: 	LDI	RACE_GG,AR2
    // asm 00006DF6: 	LDI	AUD_FS_HID_GG,AR3
    // asm 00006DF7: 	CMPI	2,R0
    // asm 00006DF8: 	LDIEQ	RACE_IN,AR2
    // asm 00006DF9: 	LDIEQ	AUD_FS_HID_IN,AR3
    // asm 00006DFA: 	CMPI	3,R0
    // asm 00006DFB: 	LDIEQ	RACE_SF,AR2
    translated_race = HIDDEN_ON == 2 ? RACE_IN : HIDDEN_ON == 3 ? RACE_SF
                                                                : RACE_GG;
    // asm 00006DFC: 	LDIEQ	AUD_FS_HID_SF,AR3
    int audit_index = HIDDEN_ON == 2 ? AUD_FS_HID_IN : HIDDEN_ON == 3 ? AUD_FS_HID_SF
                                                                      : AUD_FS_HID_GG;
    // asm 00006DFD: 	PUSH	AR2
    // asm 00006DFE: 	LDI	AR3,AR2
    // asm 00006DFF: 	CALL	AUDIT_INC
    AUDIT_INC(audit_index);
    // asm 00006E00: 	POP	AR2
    // asm 00006E01: 	BU	JJ2
    goto JJ2;
NO_HID1:
    // 	;this must happen before translation
    // 	;
    // asm 00006E02: 	LDI	@CHOSEN_RACE,AR2
    // asm 00006E03: 	STI	AR2,@RAW_CHOOSENTRK
    RAW_CHOOSENTRK = CHOSEN_RACE;
    // asm 00006E04: 	ADDI	AUD_FS_AZ,AR2
    // asm 00006E05: 	CALL	AUDIT_INC
    AUDIT_INC(AUD_FS_AZ + CHOSEN_RACE);
    // asm 00006E06: 	LDI	@CHOSEN_RACE,AR2
    // asm 00006E07: 	MPYI	TS_SIZE,AR2
    // asm 00006E08: 	ADDI	@TRACK_SELECTION_LISTI,AR2
    // asm 00006E09: 	LDI	*AR2,AR2
    translated_race = TRACK_SELECTION_LIST[CHOSEN_RACE];
JJ2:
    // asm 00006E0A: STI	AR2,@CHOSEN_RACE
    CHOSEN_RACE = translated_race;
BOUGHRAU:
    // asm 00006E0B: 	JSRP	TSEL_ZOOMOUTP
    JSRP(TSEL_ZOOMOUTP, 7);
    // asm 00006E11: 	CLRI	R0
    // asm 00006E12: 	STI	R0,@NOAERASE
    NOAERASE = 0;
    // asm 00006E13: 	CALL	OBJ_INIT
    OBJ_INIT();
    // asm 00006E14: 	SLEEP	2
    SLEEP(2, 8);
    // asm 00006E16: 	CALL	CLEANUP_TRACKSEL_PALS
    CLEANUP_TRACKSEL_PALS();
    // asm 00006E17: 	CALL	CLEANUP_PALS
    CLEANUP_PALS();
    // asm 00006E18: 	LDI	@CHOSEN_RACE,AR0
    // asm 00006E19: 	ADDI	@RACE_STARTING_POINTSI,AR0
    // asm 00006E1A: 	LDI	*AR0,R0
    // asm 00006E1B: 	STI	R0,@STARTSECTION
    STARTSECTION = RACE_STARTING_POINTS[CHOSEN_RACE];
    // asm 00006E1C: 	CALL	FIND_STARTING_VALUES
    FIND_STARTING_VALUES();
    // asm 00006E1D: 	LDI	UTIL_C|CHOOSERACE_T,R0
    // asm 00006E1E: 	LDI	-1,R1
    // asm 00006E1F: 	CALL	PRC_KILLALL
    PRC_KILLALL(UTIL_C | CHOOSERACE_T, -1);
    // asm 00006E20: 	LDI	@CHOSEN_RACE,R0
    // asm 00006E21: 	STI	R0,@BONUS_WAVE		;Set bonus wave 'cause noby else does until you finish a wave
    BONUS_WAVE = CHOSEN_RACE;
    // asm 00006E22: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void CLEANUP_TRACKSEL_PALS(void) {
    // asm 00006E26: 	LDL	crace_PALETTES,AR2
    // asm 00006E27: 	CALL	dealloc_section
    dealloc_section(crace_PALETTES);
    // asm 00006E28: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *V1+V2 = GOLDEN GATE PARK  not
 *V1+V3 = SAN FRANCISCO
 *V2+V3 = INDIANA
 *
 *
 *
 */

/*
 *
 *
 *	R0	IDX {1,2,3}
 *
 */
static void RESTORE_HIDDEN(int hidden_index) {
    OBJ* obj;
    // asm 00006E29: 	CMPI	-1,R0
    if (hidden_index == -1) {
        return;
    }
    // asm 00006E2A: 	RETSEQ
    // asm 00006E2B: 	PUSH	AR0
    // asm 00006E2C: 	PUSH	AR2
    // asm 00006E2D: 	LDI	401h,AR2
    int object_id = 0x401;
    // asm 00006E2E: 	CMPI	2,R0
    // asm 00006E2F: 	LDIEQ	406h,AR2
    if (hidden_index == 2)
        object_id = 0x406;
    // asm 00006E30: 	CMPI	3,R0
    // asm 00006E31: 	LDIEQ	409h,AR2
    if (hidden_index == 3)
        object_id = 0x409;
    // asm 00006E32: 	CALL	OBJ_FIND_FIRST
    obj = OBJ_FIND_FIRST(object_id);
    // asm 00006E33: 	LDI	*+AR0(OCARBLK),R0
    // asm 00006E34: 	STI	R0,*+AR0(OROMDATA)
    if (obj != NULL)
        obj->romdata = obj->aniblock;
    // asm 00006E35: 	POP	AR2
    // asm 00006E36: 	POP	AR0
    // asm 00006E37: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void HIDDEN_TRACKS(PROC* p) {
    OBJ* obj;
    int view_buttons = (SWITCHBUTS >> 16) & (SW_VIEW0_H | SW_VIEW1_H | SW_VIEW2_H);
    // asm 00006E38: 	LDI	@SWITCHBUTS,R0
    // asm 00006E39: 	RS	16,R0
    // asm 00006E3A: 	AND	SW_VIEW0_H|SW_VIEW1_H|SW_VIEW2_H,R0
    // asm 00006E3B: 	CMPI	SW_VIEW0_H|SW_VIEW1_H,R0
    // asm 00006E3C: 	BNE	NHT1
    if (view_buttons != (SW_VIEW0_H | SW_VIEW1_H))
        goto NHT1;
    // asm 00006E3D: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006E3E: 	CMPI	1,R0
    // asm 00006E3F: 	BEQ	HIDDTX
    if (p->ctx.TRACK_SELECTION.last_hidden_track == 1)
        goto HIDDTX;
    // asm 00006E40: 	CALL	RESTORE_HIDDEN
    RESTORE_HIDDEN(p->ctx.TRACK_SELECTION.last_hidden_track);
    // asm 00006E41: 	LDI	1,R0
    // asm 00006E42: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    p->ctx.TRACK_SELECTION.last_hidden_track = 1;
    // 	;replace
    // asm 00006E43: 	LDI	401h,AR2
    // asm 00006E44: 	CALL	OBJ_FIND_FIRST
    obj = OBJ_FIND_FIRST(0x401);
    // asm 00006E45: 	LDI	*+AR0(OFLAGS),R0
    // asm 00006E46: 	ANDN	O_1PAL,R0
    // asm 00006E47: 	STI	R0,*+AR0(OFLAGS)
    if (obj == NULL)
        goto HIDDTX;
    obj->flags &= ~O_1PAL;
    // asm 00006E48: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006E49: 	STI	R0,*+AR0(OCARBLK)
    obj->aniblock = obj->romdata;
    // asm 00006E4A: 	LDL	mpcesy,R0
    // asm 00006E4B: 	STI	R0,*+AR0(OROMDATA)
    obj->romdata = ROM_PTR(mpcesy_ROM);
    // asm 00006E4C: 	BU	HIDDTX
NHT1:
    // asm 00006E4D: CMPI	SW_VIEW0_H|SW_VIEW2_H,R0
    // asm 00006E4E: 	BNE	NHT2
    if (view_buttons != (SW_VIEW0_H | SW_VIEW2_H))
        goto NHT2;
    // asm 00006E4F: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006E50: 	CMPI	2,R0
    // asm 00006E51: 	BEQ	HIDDTX
    if (p->ctx.TRACK_SELECTION.last_hidden_track == 2)
        goto HIDDTX;
    // asm 00006E52: 	CALL	RESTORE_HIDDEN
    RESTORE_HIDDEN(p->ctx.TRACK_SELECTION.last_hidden_track);
    // asm 00006E53: 	LDI	2,R0
    // asm 00006E54: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    p->ctx.TRACK_SELECTION.last_hidden_track = 2;
    // 	;replace
    // asm 00006E55: 	LDI	406h,AR2
    // asm 00006E56: 	CALL	OBJ_FIND_FIRST
    obj = OBJ_FIND_FIRST(0x406);
    // asm 00006E57: 	LDI	*+AR0(OFLAGS),R0
    // asm 00006E58: 	ANDN	O_1PAL,R0
    // asm 00006E59: 	STI	R0,*+AR0(OFLAGS)
    if (obj == NULL)
        goto HIDDTX;
    obj->flags &= ~O_1PAL;
    // asm 00006E5A: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006E5B: 	STI	R0,*+AR0(OCARBLK)
    obj->aniblock = obj->romdata;
    // asm 00006E5C: 	LDL	mpcmed,R0
    // asm 00006E5D: 	STI	R0,*+AR0(OROMDATA)
    obj->romdata = ROM_PTR(mpcmed_ROM);
    // asm 00006E5E: 	BU	HIDDTX
NHT2:
    // asm 00006E5F: CMPI	SW_VIEW1_H|SW_VIEW2_H,R0
    // asm 00006E60: 	BNE	NHT3
    if (view_buttons != (SW_VIEW1_H | SW_VIEW2_H))
        goto NHT3;
    // asm 00006E61: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006E62: 	CMPI	3,R0
    // asm 00006E63: 	BEQ	HIDDTX
    if (p->ctx.TRACK_SELECTION.last_hidden_track == 3)
        goto HIDDTX;
    // asm 00006E64: 	CALL	RESTORE_HIDDEN
    RESTORE_HIDDEN(p->ctx.TRACK_SELECTION.last_hidden_track);
    // asm 00006E65: 	LDI	3,R0
    // asm 00006E66: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    p->ctx.TRACK_SELECTION.last_hidden_track = 3;
    // 	;replace
    // asm 00006E67: 	LDI	409h,AR2
    // asm 00006E68: 	CALL	OBJ_FIND_FIRST
    obj = OBJ_FIND_FIRST(0x409);
    // asm 00006E69: 	LDI	*+AR0(OFLAGS),R0
    // asm 00006E6A: 	ANDN	O_1PAL,R0
    // asm 00006E6B: 	STI	R0,*+AR0(OFLAGS)
    if (obj == NULL)
        goto HIDDTX;
    obj->flags &= ~O_1PAL;
    // asm 00006E6C: 	LDI	409h,AR2
    // asm 00006E6D: 	CALL	OBJ_FIND_FIRST
    obj = OBJ_FIND_FIRST(0x409);
    // asm 00006E6E: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006E6F: 	STI	R0,*+AR0(OCARBLK)
    if (obj == NULL)
        goto HIDDTX;
    obj->aniblock = obj->romdata;
    // asm 00006E70: 	LDL	mpcexp,R0
    // asm 00006E71: 	STI	R0,*+AR0(OROMDATA)
    obj->romdata = ROM_PTR(mpcexp_ROM);
    // asm 00006E72: 	BU	HIDDTX
NHT3:
    // asm 00006E73: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R0
    // asm 00006E74: 	CMPI	-1,R0
    // asm 00006E75: 	BEQ	HIDDTX
    if (p->ctx.TRACK_SELECTION.last_hidden_track == -1)
        goto HIDDTX;
    // asm 00006E76: 	CALL	RESTORE_HIDDEN
    RESTORE_HIDDEN(p->ctx.TRACK_SELECTION.last_hidden_track);
    // asm 00006E77: 	LDI	-1,R0
    // asm 00006E78: 	STI	R0,*+AR7(LAST_HIDDEN_TRACK)
    p->ctx.TRACK_SELECTION.last_hidden_track = -1;
HIDDTX:
    // asm 00006E79: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GET THE RACE INDEX (POINTED TO BY WHEEL)
 *
 *
 */
static void GET_RACE_IDX(PROC* p) {
    OBJ* race_obj;
    tTEXT* text;
    int race_index;
    int display_index;
    // asm 00006E7A: 	CALL	GETCHOICE
    GETCHOICE();
    // asm 00006E7B: 	LDI	@POSE,R0
    // asm 00006E7C: 	CMPI	5,R0
    // asm 00006E7D: 	BLE	NDDR
    race_index = POSE;
    if (race_index <= 5)
        goto NDDR;
    // asm 00006E7E: 	DEC	R0
    race_index--;
NDDR:
    // asm 00006E7F: CMPI	@CHOSEN_RACE,R0
    // asm 00006E80: 	BEQ	NSND
    if (race_index == CHOSEN_RACE)
        goto NSND;
    // asm 00006E81: 	PUSH	R0
    // asm 00006E82: 	SONDFX	TRANS
    ONESNDFX(TRANS);
    // asm 00006E84: 	POP	R0
NSND:
    // asm 00006E85: STI	R0,@CHOSEN_RACE
    CHOSEN_RACE = race_index;
    display_index = race_index;
    // asm 00006E86: 	LDI	*+AR7(LAST_HIDDEN_TRACK),R1
    // asm 00006E87: 	CMPI	-1,R1
    // asm 00006E88: 	BEQ	NH
    if (p->ctx.TRACK_SELECTION.last_hidden_track == -1)
        goto NH;
    // asm 00006E89: 	CMPI	1,R0
    // asm 00006E8A: 	BNE	R55
    if (race_index != 1)
        goto R55;
    // asm 00006E8B: 	CMPI	1,R1		;Hidden golden gate
    // asm 00006E8C: 	LDIEQ	11,AR3
    // asm 00006E8D: 	BNE	NH
    if (p->ctx.TRACK_SELECTION.last_hidden_track != 1)
        goto NH;
    display_index = 11;
    // asm 00006E8E: 	BU	R66
R55:
    // asm 00006E8F: 	CMPI	6,R0
    // asm 00006E90: 	BNE	R56
    if (race_index != 6)
        goto R56;
    // asm 00006E91: 	CMPI	2,R1
    // asm 00006E92: 	LDIEQ	12,AR3
    // asm 00006E93: 	BNE	NH
    if (p->ctx.TRACK_SELECTION.last_hidden_track != 2)
        goto NH;
    display_index = 12;
    // asm 00006E94: 	BU	R66
R56:
    // asm 00006E95: 	CMPI	9,R0
    // asm 00006E96: 	BNE	NH
    if (race_index != 9)
        goto NH;
    // asm 00006E97: 	CMPI	3,R1
    // asm 00006E98: 	LDIEQ	13,AR3
    // asm 00006E99: 	BNE	NH
    if (p->ctx.TRACK_SELECTION.last_hidden_track != 3)
        goto NH;
    display_index = 13;
R66:
    // asm 00006E9A: 	LDI	@CHOSEN_RACE,R0
    // asm 00006E9B: 	LDI	R0,AR2
    // asm 00006E9C: 	BU	KK88
NH:
    // asm 00006E9D: 	LDI	@CHOSEN_RACE,AR2
    // asm 00006E9E: 	LDI	AR2,AR3
    display_index = CHOSEN_RACE;
KK88:
    // asm 00006E9F: ADDI	400h,AR2
    // asm 00006EA0: 	CALL	OBJ_FIND_FIRST
    race_obj = OBJ_FIND_FIRST(CHOSEN_RACE + 0x400);
    if (race_obj == NULL)
        return;
    // asm 00006EA1: 	LDF	*+AR0(OPOSX),R2
    // asm 00006EA2: 	FLOAT	256,R0
    // asm 00006EA3: 	ADDF	R0,R2
    // asm 00006EA4: 	LDF	*+AR0(OPOSY),R3
    // asm 00006EA5: 	FLOAT	180,R0
    // asm 00006EA6: 	ADDF	R0,R3
    // asm 00006EA7: 	LDI	AR3,R0
    // asm 00006EA8: 	MPYI	TS_SIZE,R0
    // asm 00006EA9: 	LDI	R0,AR0
    // asm 00006EAA: 	ADDI	@TRACK_SELECTION_LISTI,AR0
    // asm 00006EAB: 	LDI	*AR0,AR2
    // asm 00006EAC: 	ADDI	@LEG_NAMESI,AR2
    // asm 00006EAD: 	LDI	*AR2,AR2
    // asm 00006EAE: 	LDI	1,RC
    // asm 00006EAF: 	CALL	TEXT_ADD
    text = TEXT_ADD(
        LEG_NAMES[TRACK_SELECTION_LIST[display_index]],
        C3X_ADD(C3X_LDF(race_obj->pos.X), C3X_FROM_INT(256)),
        C3X_ADD(C3X_LDF(race_obj->pos.Y), C3X_FROM_INT(180)), 1);
    // asm 00006EB0: 	CALL	SET12FONT
    SET12FONT(text);
    // asm 00006EB1: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text->color |= TXT_CENTER;
    // asm 00006EB4: 	CMPI	5,AR3
    // asm 00006EB5: 	BNE	NOTTHEWH
    if (display_index != 5)
        goto NOTTHEWH;
    // asm 00006EB6: 	READAUD	ADJ_CLINTON
    // asm 00006EB8: 	CMPI	1,R0
    // asm 00006EB9: 	BNE	NOTTHEWH
    if (READAUD(ADJ_CLINTON) != 1)
        goto NOTTHEWH;
    // asm 00006EBA: 	LDI	@CRUISE2,AR2
    // asm 00006EBB: 	FLOAT	256,R2
    // asm 00006EBC: 	FLOAT	120,R3
    // asm 00006EBD: 	LDI	1,RC
    // asm 00006EBE: 	CALL	TEXT_ADD
    text = TEXT_ADD(CRUISE2, C3X_FROM_INT(256), C3X_FROM_INT(120), 1);
    // asm 00006EBF: 	CALL	SET12FONT
    SET12FONT(text);
    // asm 00006EC0: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text->color |= TXT_CENTER;
    // asm 00006EC3: 	LDI	@CRUISE3,AR2
    // asm 00006EC4: 	FLOAT	256,R2
    // asm 00006EC5: 	FLOAT	135,R3
    // asm 00006EC6: 	LDI	1,RC
    // asm 00006EC7: 	CALL	TEXT_ADD
    text = TEXT_ADD(CRUISE3, C3X_FROM_INT(256), C3X_FROM_INT(135), 1);
    // asm 00006EC8: 	CALL	SET12FONT
    SET12FONT(text);
    // asm 00006EC9: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text->color |= TXT_CENTER;
NOTTHEWH:
    // asm 00006ECC: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void ZOOMINP(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        // MAME_ASSERT_ORDERING("ZOOMINP");
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 00006ECD: 	LDI	20,AR4
    p->ctx.TRACK_SELECTION.loop_count = 20;
ZITSP:
    // asm 00006ECE: CALL	ZOOMIN_TSEL
    ZOOMIN_TSEL();
    // asm 00006ECF: 	LDI	@OM_CHOSEN_RACE,R0
    // asm 00006ED0: 	CMPI	-1,R0
    // asm 00006ED1: 	BNE	NOWAY543
    if (OM_CHOSEN_RACE != -1) {
        goto NOWAY543;
    }
    // asm 00006ED2: 	CALL	GET_RACE_IDX
    GET_RACE_IDX(p);
    // asm 00006ED3: 	CALL	TRACK_SEL_CURSOR
    TRACK_SEL_CURSOR(p);
NOWAY543:
    // asm 00006ED4: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00006ED6: 	DBU	AR4,ZITSP
    p->ctx.TRACK_SELECTION.loop_count -= 1;
    if (p->ctx.TRACK_SELECTION.loop_count >= 0) {
        goto ZITSP;
    }
    // asm 00006ED7: 	RETP
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void ZOOMIN_TSEL(void) {
    OBJ* obj;
    OBJ* next_obj;
    c3x_reg_t delta;

    // MAME_ASSERT_FUNCTION_ENTRY();

    // asm 00006EDB: 	LDI	@OACTIVE,AR5
    next_obj = OACTIVE;
ZISL:
    // asm 00006EDC: LDI	AR5,R0
    // asm 00006EDD: 	BZ	ZIX
    if (next_obj == NULL) {
        goto ZIX;
    }
    // asm 00006EDE: 	LDI	R0,AR0
    obj = next_obj;
    // asm 00006EDF: 	LDI	*AR0,AR5
    next_obj = obj->link;
    // asm 00006EE0: 	LDF	*+AR0(OVELZ),R0
    // asm 00006EE1: 	SUBF	*+AR0(OPOSZ),R0
    // asm 00006EE2: 	MPYF	0.42,R0	;.15
    delta = C3X_MUL(
        C3X_SUB(C3X_LDF(obj->vel_z), C3X_LDF(obj->pos.Z)),
        C3X_IMM_F32(0.42));
    // asm 00006EE3: 	CMPF	-5,R0
    // asm 00006EE4: 	BGT	JJDD
    if (C3X_GT(delta, C3X_IMM_F32(-5))) {
        goto JJDD;
    }
    // asm 00006EE5: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00006EE6: 	STF	R0,*+AR0(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(delta, C3X_LDF(obj->pos.Z)));
    // asm 00006EE7: 	BU	HGF
    goto HGF;
JJDD:
    // asm 00006EE8: 	LDF	*+AR0(OVELZ),R0
    // asm 00006EE9: 	STF	R0,*+AR0(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_LDF(obj->vel_z));
    // asm 00006EEA: 	CLRF	R2
    // asm 00006EEB: 	STF	R2,*+AR0(ORADZ)
    obj->rad.Z = C3X_STF(C3X_FROM_INT(0));
    // asm 00006EEC: 	LDI	AR0,AR2
    // asm 00006EED: 	ADDI	OMATRIX,AR2
    // asm 00006EEE: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&obj->omatrix, C3X_LDF(obj->rad.Z));
    // asm 00006EEF: 	BU	HHFFD
    goto HHFFD;
HGF:
    // asm 00006EF0: 	LDF	*+AR0(ORADZ),R2
    // asm 00006EF1: 	MPYF	0.65,R2
    // asm 00006EF2: 	STF	R2,*+AR0(ORADZ)
    obj->rad.Z = C3X_STF(C3X_MUL(C3X_LDF(obj->rad.Z), C3X_IMM_F32(0.65)));
    // asm 00006EF3: 	LDI	AR0,AR2
    // asm 00006EF4: 	ADDI	OMATRIX,AR2
    // asm 00006EF5: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&obj->omatrix, C3X_LDF(obj->rad.Z));
HHFFD:
    // asm 00006EF6: 	BU	ZISL
    goto ZISL;
ZIX:
    // asm 00006EF7: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *DO THE NESSESARY MUMBO JUMBO, *AND* LINKEM _ALL_
 *TOGETHER VIA OLINK2
 *
 *
 *
 */
static void FIX_TRACK_SCREEN(PROC* p) {
    OBJ* obj;
    OBJ* next_obj;
    OBJ* first_obj = NULL;
    PROC_CONTEXT* ctx;
    // asm 00006EF8: 	CLRI	R0
    TSEL_HEAD = NULL;
    // asm 00006EF9: 	STI	R0,@TSEL_HEAD
    // asm 00006EFA: 	LDI	-1,R6
    // asm 00006EFB: 	LDI	@OACTIVE,AR5
    next_obj = OACTIVE;
FTSL:
    // asm 00006EFC: LDI	AR5,R0
    // asm 00006EFD: 	BZ	FTSLX
    if (next_obj == NULL)
        goto FTSLX;
    // asm 00006EFE: 	LDI	R0,AR0
    obj = next_obj;
    // asm 00006EFF: 	LDI	*AR0,AR5		;NEXT OBJECT
    next_obj = obj->link;
    // asm 00006F00: 	LDF	*+AR0(OPOSX),R0
    // asm 00006F01: 	STF	R0,*+AR0(OVELX)
    obj->vel_x = obj->pos.X;
    // asm 00006F02: 	LDF	*+AR0(OPOSY),R0
    // asm 00006F03: 	STF	R0,*+AR0(OVELY)
    obj->vel_y = obj->pos.Y;
    // asm 00006F04: 	LDF	*+AR0(OPOSZ),R0
    // asm 00006F05: 	STF	R0,*+AR0(OVELZ)
    obj->vel_z = obj->pos.Z;
    // asm 00006F06: 	RANDN	28000
    // asm 00006F08: 	FLOAT	R0
    // asm 00006F09: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00006F0A: 	STF	R0,*+AR0(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Z), C3X_FROM_INT(RANDU0(28000))));
    // asm 00006F0B: 	LDF	6,R0
    // asm 00006F0C: 	CALL	SFRAND
    // asm 00006F0D: 	STF	R0,*+AR0(ORADZ)
    obj->rad.Z = C3X_STF(SFRAND(C3X_FROM_INT(6)));
    // asm 00006F0E: 	LDI	*+AR0(OID),R0
    // asm 00006F0F: 	CMPI	040Ch,R0	;SMALL CURSOR
    // asm 00006F10: 	BNE	NSC
    if (obj->id != 0x40c)
        goto NSC;
    // asm 00006F11: 	STI	AR0,*+AR7(SMALL_CURSOR)
    p->ctx.TRACK_SELECTION.small_cursor = obj;
    // asm 00006F12: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006F13: 	STI	R0,*+AR7(ROM_SMALL_CURSOR)
    p->ctx.TRACK_SELECTION.rom_small_cursor = obj->romdata;
    // asm 00006F14: 	CMPI	-1,R6
    // asm 00006F15: 	LDIEQ	AR0,R6
    if (first_obj == NULL)
        first_obj = obj;
    // asm 00006F16: 	STI	AR5,*+AR0(OLINK2)
    obj->link2 = (uintptr_t)next_obj;
    // asm 00006F17: 	BU	FTSL
    goto FTSL;
NSC:
    // asm 00006F18: 	CMPI	8A1h,R0
    // asm 00006F19: 	BNE	NTRM1
    if (obj->id != 0x8a1)
        goto NTRM1;
    // asm 00006F1A: 	LDI	AR0,AR4
    // asm 00006F1B: 	CREATE	CYCLE_PUSH,UTIL_C|CHOOSERACE_T
    ctx = NEW_PROC_CONTEXT();
    ctx->TRACKSEL_ANIMATION.obj = obj;
    CREATE(CYCLE_PUSH, UTIL_C | CHOOSERACE_T, ctx);
    // asm 00006F1E: 	LDI	AR4,AR0
    // asm 00006F1F: 	CMPI	-1,R6
    // asm 00006F20: 	LDIEQ	AR0,R6
    if (first_obj == NULL)
        first_obj = obj;
    // asm 00006F21: 	STI	AR5,*+AR0(OLINK2)
    obj->link2 = (uintptr_t)next_obj;
    // asm 00006F22: 	BU	FTSL
    goto FTSL;
NTRM1:
    // asm 00006F23: 	CMPI	8A2h,R0
    // asm 00006F24: 	BNE	NWHEL1
    if (obj->id != 0x8a2)
        goto NWHEL1;
    // asm 00006F25: 	LDI	AR0,AR4
    // asm 00006F26: 	CREATE	TURNTO_SELECT,UTIL_C|CHOOSERACE_T
    ctx = NEW_PROC_CONTEXT();
    ctx->TRACKSEL_ANIMATION.obj = obj;
    CREATE(TURNTO_SELECT, UTIL_C | CHOOSERACE_T, ctx);
    // asm 00006F29: 	LDI	AR4,AR0
    // asm 00006F2A: 	CMPI	-1,R6
    // asm 00006F2B: 	LDIEQ	AR0,R6
    if (first_obj == NULL)
        first_obj = obj;
    // asm 00006F2C: 	STI	AR5,*+AR0(OLINK2)
    obj->link2 = (uintptr_t)next_obj;
    // asm 00006F2D: 	BU	FTSL
    goto FTSL;
NWHEL1:
    // asm 00006F2E: 	CMPI	0410h,R0
    // asm 00006F2F: 	BNE	NBC
    if (obj->id != 0x410)
        goto NBC;
    // asm 00006F30: 	STI	AR0,*+AR7(BIG_CURSOR)
    p->ctx.TRACK_SELECTION.big_cursor = obj;
    // asm 00006F31: 	LDI	*+AR0(OROMDATA),R0
    // asm 00006F32: 	STI	R0,*+AR7(ROM_BIG_CURSOR)
    p->ctx.TRACK_SELECTION.rom_big_cursor = obj->romdata;
    // asm 00006F33: 	LDI	AR0,AR2
    // asm 00006F34: 	CALL	OBJ_DELETE
    OBJ_DELETE(obj);
    // asm 00006F35: 	CMPI	-1,R6
    // asm 00006F36: 	LDIEQ	AR0,R6
    // asm 00006F37: 	STI	AR5,*+AR0(OLINK2)
    // asm 00006F38: 	BU	FTSL
    goto FTSL;
NBC:
    // asm 00006F39: 	CMPI	0405h,R0
    // asm 00006F3A: 	BNE	NAS
    if (obj->id != 0x405)
        goto NAS;
    // asm 00006F3B: 	PUSH	AR0
    // asm 00006F3C: 	LDI	AR0,AR4
    // asm 00006F3D: 	CREATE	ANIMATE_MAP,UTIL_C|CHOOSERACE_T
    ctx = NEW_PROC_CONTEXT();
    ctx->TRACKSEL_ANIMATION.obj = obj;
    CREATE(ANIMATE_MAP, UTIL_C | CHOOSERACE_T, ctx);
    // asm 00006F40: 	POP	AR0
    // asm 00006F41: 	CMPI	-1,R6
    // asm 00006F42: 	LDIEQ	AR0,R6
    if (first_obj == NULL)
        first_obj = obj;
    // asm 00006F43: 	STI	AR5,*+AR0(OLINK2)
    obj->link2 = (uintptr_t)next_obj;
    // asm 00006F44: 	BU	FTSL
    goto FTSL;
NAS:
    // asm 00006F45: 	CMPI	40Eh,R0
    // asm 00006F46: 	BNE	NN77
    if (obj->id != 0x40e)
        goto NN77;
    // asm 00006F47: 	LDI	AR0,AR2
    // asm 00006F48: 	CALL	OBJ_DELETE
    OBJ_DELETE(obj);
    // asm 00006F49: 	BU	FTSL
    goto FTSL;
NN77:
    // asm 00006F4A: 	CMPI	40Fh,R0
    // asm 00006F4B: 	BNE	NN88
    if (obj->id != 0x40f)
        goto NN88;
    // asm 00006F4C: 	LDI	AR0,AR2
    // asm 00006F4D: 	CALL	OBJ_DELETE
    OBJ_DELETE(obj);
    // asm 00006F4E: 	BU	FTSL
    goto FTSL;
NN88:
    // asm 00006F4F: 	CMPI	-1,R6
    // asm 00006F50: 	LDIEQ	AR0,R6
    if (first_obj == NULL)
        first_obj = obj;
    // asm 00006F51: 	STI	AR5,*+AR0(OLINK2)
    obj->link2 = (uintptr_t)next_obj;
    // asm 00006F52: 	BU	FTSL
    goto FTSL;
FTSLX:
    // asm 00006F53: 	STI	R6,@TSEL_HEAD
    TSEL_HEAD = first_obj;
    // asm 00006F54: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: TSEL_HEAD	.bss	TSEL_HEAD,1 */
OBJ* TSEL_HEAD;
/* asm: TSEL_THEONE	.bss	TSEL_THEONE,1 */
OBJ* TSEL_THEONE;

static void TSEL_ZOOMOUTP(PROC* p) {
    PROC_CONTEXT* ctx;
    int race_index;

    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00006F55: 	SONDFX	VIEW2IN
    ONESNDFX(VIEW2IN);
    // asm 00006F57: 	LDI	@RACE_MODE,R0
    // asm 00006F58: 	CMPI	RM_USA,R0
    // asm 00006F59: 	BNE	JJKKD
    if (RACE_MODE != RM_USA)
        goto JJKKD;
    // asm 00006F5A: 	LDI	5,AR2
    race_index = 5;
    // asm 00006F5B: 	BU	LLGF
    goto LLGF;
JJKKD:
    // asm 00006F5C: LDI	@RAW_CHOOSENTRK,AR2
    race_index = RAW_CHOOSENTRK;
LLGF:
    // asm 00006F5D: LDI	AR2,AR3
    // asm 00006F5E: 	ADDI	400h,AR2
    // asm 00006F5F: 	CALL	OBJ_FIND_FIRST
    TSEL_THEONE = OBJ_FIND_FIRST(race_index + 0x400);
    // asm 00006F60: 	STI	AR0,@TSEL_THEONE
    // asm 00006F61: 	CREATEC	CENTER_THEONE,UTIL_C
    ctx = NEW_PROC_CONTEXT();
    ctx->CENTER_THEONE.obj = TSEL_THEONE;
    CREATEC(p, CENTER_THEONE, UTIL_C, ctx);
    // asm 00006F64: 	LDI	30,AR5
    p->ctx.TRACK_SELECTION.loop_count = 30;
TSLLP:
    // asm 00006F65: CALL	TSEL_ZOOMOUT
    TSEL_ZOOMOUT();
    // asm 00006F66: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00006F68: 	DBU	AR5,TSLLP
    if (--p->ctx.TRACK_SELECTION.loop_count >= 0)
        goto TSLLP;
    // asm 00006F69: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CENTER_THEONE(PROC* p) {
    OBJ* obj;
    c3x_reg_t delta;

    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }
    // asm 00006F6D: 	LDI	@TSEL_THEONE,AR4
    obj = p->ctx.CENTER_THEONE.obj;
    if (obj == NULL)
        DIE();
    // asm 00006F6E: 	LDI	15,AR5
    p->ctx.CENTER_THEONE.loop_count = 15;
CTOLP:
    // asm 00006F6F: LDF	*+AR4(OPOSX),R0
    // asm 00006F70: 	MPYF	0.8,R0
    // asm 00006F71: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_MUL(C3X_LDF(obj->pos.X), C3X_IMM_F32(0.8)));
    // asm 00006F72: 	LDF	*+AR4(OPOSY),R0
    // asm 00006F73: 	MPYF	0.8,R0
    // asm 00006F74: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_MUL(C3X_LDF(obj->pos.Y), C3X_IMM_F32(0.8)));
    // asm 00006F75: 	CALL	CENTER_SUB1
    CENTER_SUB1(obj);
    // asm 00006F76: 	SLEEP	1
    SLEEP(1, 1);
    obj = p->ctx.CENTER_THEONE.obj;
    // asm 00006F78: 	DBU	AR5,CTOLP
    if (--p->ctx.CENTER_THEONE.loop_count >= 0)
        goto CTOLP;
    // asm 00006F79: 	LDI	15,AR5
    p->ctx.CENTER_THEONE.loop_count = 15;
CTOLPB:
    // asm 00006F7A: LDF	*+AR4(OPOSZ),R0
    // asm 00006F7B: 	MPYF	0.08,R0
    // asm 00006F7C: 	CMPF	50,R0
    // asm 00006F7D: 	LDFLT	50,R0
    // asm 00006F7E: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00006F7F: 	STF	R0,*+AR4(OPOSZ)
    delta = C3X_MUL(C3X_LDF(obj->pos.Z), C3X_IMM_F32(0.08));
    if (C3X_LT(delta, C3X_FROM_INT(50)))
        delta = C3X_FROM_INT(50);
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Z), delta));
    // asm 00006F80: 	CALL	CENTER_SUB1
    CENTER_SUB1(obj);
    // asm 00006F81: 	SLEEP	1
    SLEEP(1, 2);
    obj = p->ctx.CENTER_THEONE.obj;
    // asm 00006F83: 	DBU	AR5,CTOLPB
    if (--p->ctx.CENTER_THEONE.loop_count >= 0)
        goto CTOLPB;
    // asm 00006F84: 	DIE
    DIE();
}

static void CENTER_SUB1(OBJ* obj) {
    tTEXT* text;
    int name_index;
    // asm 00006F85: 	LDF	*+AR4(OPOSX),R2
    // asm 00006F86: 	FLOAT	256,R0
    // asm 00006F87: 	ADDF	R0,R2
    // asm 00006F88: 	LDF	*+AR4(OPOSY),R3
    // asm 00006F89: 	FLOAT	180,R0
    // asm 00006F8A: 	ADDF	R0,R3
    // asm 00006F8B: 	LDI	@RACE_MODE,R0
    // asm 00006F8C: 	CMPI	RM_USA,R0
    // asm 00006F8D: 	BNE	K89
    // asm 00006F8E: 	LDI	14,AR2
    // asm 00006F8F: 	BU	K99
K89:
    // asm 00006F90: LDI	@CHOSEN_RACE,AR2
K99:
    // asm 00006F91: ADDI	@LEG_NAMESI,AR2
    // asm 00006F92: 	LDI	*AR2,AR2
    // asm 00006F93: 	LDI	1,RC
    // asm 00006F94: 	CALL	TEXT_ADD
    name_index = RACE_MODE == RM_USA ? 14 : CHOSEN_RACE;
    text = TEXT_ADD(LEG_NAMES[name_index],
        C3X_ADD(C3X_LDF(obj->pos.X), C3X_FROM_INT(256)),
        C3X_ADD(C3X_LDF(obj->pos.Y), C3X_FROM_INT(180)), 1);
    // asm 00006F95: 	CALL	SET12FONT
    SET12FONT(text);
    // asm 00006F96: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text->color |= TXT_CENTER;
    // asm 00006F99: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void TSEL_ZOOMOUT(void) {
    OBJ* obj = TSEL_HEAD;
    // asm 00006F9A: 	LDI	@TSEL_HEAD,R0
    // asm 00006F9B: 	BU	LKGG
    goto LKGG;
JJKK:
    // asm 00006F9C: LDI	*+AR0(OLINK2),R0
    obj = (OBJ*)obj->link2;
    // asm 00006F9D: 	BZ	ALLDN
    if (obj == NULL)
        goto ALLDN;
LKGG:
    // asm 00006F9E: LDI	R0,AR0
    // asm 00006F9F: 	CMPI	@TSEL_THEONE,R0
    // asm 00006FA0: 	BEQ	JJKK			;skip our choice
    if (obj == TSEL_THEONE)
        goto JJKK;
    // asm 00006FA1: 	LDF	*+AR0(OPOSX),R0
    // asm 00006FA2: 	MPYF	0.1,R0
    // asm 00006FA3: 	ADDF	*+AR0(OPOSX),R0
    // asm 00006FA4: 	STF	R0,*+AR0(OPOSX)
    obj->pos.X = C3X_STF(C3X_MUL(C3X_LDF(obj->pos.X), C3X_IMM_F32(1.1)));
    // asm 00006FA5: 	LDF	*+AR0(OPOSY),R0
    // asm 00006FA6: 	MPYF	0.1,R0
    // asm 00006FA7: 	ADDF	*+AR0(OPOSY),R0
    // asm 00006FA8: 	STF	R0,*+AR0(OPOSY)
    obj->pos.Y = C3X_STF(C3X_MUL(C3X_LDF(obj->pos.Y), C3X_IMM_F32(1.1)));
    // asm 00006FA9: 	LDF	*+AR0(OPOSZ),R0
    // asm 00006FAA: 	MPYF	0.08,R0
    // asm 00006FAB: 	CMPF	50,R0
    // asm 00006FAC: 	LDFLT	50,R0
    // asm 00006FAD: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00006FAE: 	STF	R0,*+AR0(OPOSZ)
    c3x_reg_t delta = C3X_MUL(C3X_LDF(obj->pos.Z), C3X_IMM_F32(0.08));
    if (C3X_LT(delta, C3X_FROM_INT(50)))
        delta = C3X_FROM_INT(50);
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Z), delta));
    // asm 00006FAF: 	BU	JJKK
ALLDN:
    // asm 00006FB0: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 */
/* asm: ANIMATE_MAP_TAB		.word	mhop1,mhop2,mhop3,mhop4,-1 */
static int ANIMATE_MAP_TAB[] = {
    mhop1_ROM,
    mhop2_ROM,
    mhop3_ROM,
    mhop4_ROM,
    -1,
};

// *
static void ANIMATE_MAP(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00006FB7: 	LDI	@ANIMATE_MAP_TABI,AR5
    p->ctx.TRACKSEL_ANIMATION.script_index = 0;
AMLP:
    // asm 00006FB8: 	LDI	*AR5++,R0
    // asm 00006FB9: 	BN	ANIMATE_MAP
    if (ANIMATE_MAP_TAB[p->ctx.TRACKSEL_ANIMATION.script_index] < 0) {
        p->ctx.TRACKSEL_ANIMATION.script_index = 0;
    }
    // asm 00006FBA: 	STI	R0,*+AR4(OROMDATA)
    p->ctx.TRACKSEL_ANIMATION.obj->romdata = ROM_PTR(
        ANIMATE_MAP_TAB[p->ctx.TRACKSEL_ANIMATION.script_index++]);
    // asm 00006FBB: 	SLEEP	4
    SLEEP(4, 1);
    // asm 00006FBD: 	BU	AMLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    goto AMLP;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 */
static void TRACK_SEL_CURSOR(PROC* p) {
    OBJ* cursor;
    OBJ* race_obj;
    // asm 00006FBE: 	LDI	*+AR7(OLD_INDEX),R0
    // asm 00006FBF: 	LDI	@CHOSEN_RACE,AR0
    // asm 00006FC0: 	CMPI	R0,AR0
    // asm 00006FC1: 	RETSEQ
    if (p->ctx.TRACK_SELECTION.old_index == CHOSEN_RACE)
        return;
    // asm 00006FC2: 	STI	AR0,*+AR7(OLD_INDEX)
    p->ctx.TRACK_SELECTION.old_index = CHOSEN_RACE;
    // asm 00006FC3: 	LDI	AR0,AR3
    // asm 00006FC4: 	LDI	AR0,AR2
    // asm 00006FC5: 	ADDI	400h,AR2
    // asm 00006FC6: 	CALL	OBJ_FIND_FIRST
    race_obj = OBJ_FIND_FIRST(CHOSEN_RACE + 0x400);
    cursor = p->ctx.TRACK_SELECTION.small_cursor;
    if (race_obj == NULL || cursor == NULL)
        return;
    // asm 00006FC7: 	CMPI	5,AR3
    // asm 00006FC8: 	LDIEQ	*+AR7(ROM_BIG_CURSOR),R0
    // asm 00006FC9: 	LDINE	*+AR7(ROM_SMALL_CURSOR),R0
    // asm 00006FCA: 	LDI	*+AR7(SMALL_CURSOR),AR1
    // asm 00006FCB: 	STI	R0,*+AR1(OROMDATA)
    cursor->romdata = CHOSEN_RACE == 5
        ? p->ctx.TRACK_SELECTION.rom_big_cursor
        : p->ctx.TRACK_SELECTION.rom_small_cursor;
    // asm 00006FCC: 	LDF	*+AR0(OPOSX),R0
    // asm 00006FCD: 	STF	R0,*+AR1(OPOSX)
    cursor->pos.X = race_obj->pos.X;
    // asm 00006FCE: 	LDF	*+AR0(OPOSY),R0
    // asm 00006FCF: 	STF	R0,*+AR1(OPOSY)
    cursor->pos.Y = race_obj->pos.Y;
    // asm 00006FD0: 	LDF	*+AR0(OPOSZ),R0
    // asm 00006FD1: 	SUBF	8,R0
    // asm 00006FD2: 	STF	R0,*+AR1(OPOSZ)
    cursor->pos.Z = C3X_STF(C3X_SUB(C3X_LDF(race_obj->pos.Z), C3X_FROM_INT(8)));
    // asm 00006FD3: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: TURNANI	.word	whel1,whel2,whel3,whel4,whel3,whel2,-1 */
/* asm: 	 */
static int TURNANI[] = {
    whel1_ROM,
    whel2_ROM,
    whel3_ROM,
    whel4_ROM,
    whel3_ROM,
    whel2_ROM,
    -1,
};

/*
 *	R6	X
 *	R7	Y
 *	AR5	Z
 *
 */
static void GET_TURNOBJ(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    OBJ* obj = p->ctx.TRACKSEL_ANIMATION.obj;
    if (obj == NULL)
        DIE();
    // asm 00006FDB: 	FLOAT	AR5,R5
    // asm 00006FDC: 	LDL	TURNANI,AR5
    p->ctx.TRACKSEL_ANIMATION.script_index = 0;
    // asm 00006FDD: 	CALL	OBJ_GET
    // asm 00006FDE: 	LDI	AR0,AR4
    // asm 00006FDF: 	STF	R6,*+AR4(OPOSX)
    // asm 00006FE0: 	STF	R7,*+AR4(OPOSY)
    // asm 00006FE1: 	STF	R5,*+AR4(OPOSZ)
    // asm 00006FE2: 	LDI	AR0,AR2
    // asm 00006FE3: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);
GETTOLP:
    // asm 00006FE4: 	LDI	@_MODE,R0
    // asm 00006FE5: 	AND	MMODE,R0	;<-this mode
    // asm 00006FE6: 	CMPI	MINTRO,R0
    // asm 00006FE7: 	BNE	SUICIDE
    if ((_MODE & MMODE) != MINTRO)
        DIE();
    // asm 00006FE8: 	LDI	*AR5++,R0
    // asm 00006FE9: 	BNN	JIJI
    if (TURNANI[p->ctx.TRACKSEL_ANIMATION.script_index] < 0) {
        p->ctx.TRACKSEL_ANIMATION.script_index = 0;
    }
    // asm 00006FEA: 	LDL	TURNANI,AR5
    // asm 00006FEB: 	LDI	*AR5++,R0
JIJI:
    // asm 00006FEC: STI	R0,*+AR4(OROMDATA)
    obj->romdata = ROM_PTR(TURNANI[p->ctx.TRACKSEL_ANIMATION.script_index++]);
    // asm 00006FED: 	SLEEP	7
    SLEEP(7, 1);
    // asm 00006FEF: 	BU	GETTOLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    goto GETTOLP;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: TRACK_SELECTION_LISTI	.word	TRACK_SELECTION_LIST */
#define TRACK_SELECTION_LISTI TRACK_SELECTION_LIST
/* asm: TRACK_SELECTION_LIST */
/* asm: 	.word	RACE_AZ */
/* asm: 	.word	RACE_US101 */
/* asm: 	.word	RACE_LA */
/* asm: 	.word	RACE_IOWA */
/* asm: 	.word	RACE_DV */
/* asm: 	 */
/* asm: 	.word	RACE_USA */
/* asm: 	 */
/* asm: 	.word	RACE_BH */
/* asm: 	 */
/* asm: 	.word	RACE_APPL */
/* asm: 	.word	RACE_RW */
/* asm: 	.word	RACE_GC */
/* asm: 	.word	RACE_CH */
/* asm: 	 */
/* asm: 	.word	RACE_GG */
/* asm: 	.word	RACE_IN */
/* asm: 	.word	RACE_SF */
/* asm: 	 */
/* asm: 	 */
static int TRACK_SELECTION_LIST[] = {
    RACE_AZ,
    RACE_US101,
    RACE_LA,
    RACE_IOWA,
    RACE_DV,
    RACE_USA,
    // 	.word	RACE_DC
    RACE_BH,
    RACE_APPL,
    RACE_RW,
    RACE_GC,
    RACE_CH,
    // hidden races
    RACE_GG,
    RACE_IN,
    RACE_SF,
    // ----------------------------------------------------------------------------
};

// *----------------------------------------------------------------------------
void BOILERPLATE_INIT(void) {
    // asm 00006FFF: 	LDL	next,AR2
    // asm 00007000: 	LDI	0,R2
    // asm 00007001: 	LDI	0,R3
    // asm 00007002: 	LDI	0,RC
    // asm 00007003: 	CALL	OBJ_QMAKE
    BOILEROBJ = OBJ_QMAKE(ROM_PTR(next_ROM), 0, 0, 0);
    // asm 00007004: 	STI	AR0,@BOILEROBJ
    // asm 00007005: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void BOILERPLATE(void) {
    OBJ* cursor = CURRENT_PROC->ctx.TRACK_SELECTION.small_cursor;
    // asm 00007006: 	LDI	*+AR7(SMALL_CURSOR),AR2
    // asm 00007007: 	LDF	*+AR2(OPOSX),R0
    // asm 00007008: 	STF	R0,*+AR2(OCARBLK)
    if (cursor == NULL || BOILEROBJ == NULL)
        return;
    cursor->usr1_as_float = cursor->pos.X;
    // asm 00007009: 	FLOAT	-6000,R0
    // asm 0000700A: 	STF	R0,*+AR2(OPOSX)
    cursor->pos.X = C3X_STF(C3X_FROM_INT(-6000));
    // asm 0000700B: 	LDI	@BOILEROBJ,AR2
    // asm 0000700C: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(BOILEROBJ);
    // asm 0000700D: 	RETS
    return;
}

// *----------------------------------------------------------------------------
/* asm: BOILEROBJ	.bss	BOILEROBJ,1 */
OBJ* BOILEROBJ;

// *----------------------------------------------------------------------------
static void BOILERPLATE_DELETE(void) {
    OBJ* cursor = CURRENT_PROC->ctx.TRACK_SELECTION.small_cursor;
    // asm 0000700E: 	LDI	@BOILEROBJ,AR2
    // asm 0000700F: 	CALL	OBJ_DELETE
    if (BOILEROBJ != NULL)
        OBJ_DELETE(BOILEROBJ);
    // asm 00007010: 	LDI	*+AR7(SMALL_CURSOR),AR2
    // asm 00007011: 	LDF	*+AR2(OCARBLK),R0
    // asm 00007012: 	STF	R0,*+AR2(OPOSX)
    if (cursor != NULL)
        cursor->pos.X = cursor->usr1_as_float;
    // asm 00007013: 	RETS
    return;
}

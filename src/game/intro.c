
#include "intro.h"
#include "../core/machine.h"
#include "cmos.h"
#include "comm.h"
#include "delta.h"
#include "error.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "racer.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/INTRO.ASM
 */

void HEAD2HEAD_LOGO_WAIT(void);
void HEAD2HEAD_LOGO(void);
static void KABOSHP(void);
static void JINMSG(void);
static void THROBIT(void);
void WAIT_FOR_CHALLENGER(void);
static void CHECK_ENDBONUS(void);
static void WAIT_FOR_ENDBONUS(void);
void ISSUE_STARTGAME_TSEL(void);
void ISSUE_STARTGAME(void);
void PLYR_INTRO(void);
void CHOOSE_NEXT_RACE(void);
static void LOAD_NEW_SELECTION(void);
static void WATCH_PLYRS_CAR(void);
void INIT_GAMELEG(void);
static void CHOOSECAR(void);
void THE_CAR_CHOICE_PROC(void);
static void RAISE_DOOR(void);
void ZOOMTOCAR(void);
void GETTHECARS(void);
void SHOW_CAR_STATISTICS(void);
static void CLEANUP_DIMCAR_PALS(void);
static void GETTHECAR(void);
void ROUNDER(void);
static void AFFECT_THE_CARS(void);
static void HIDDEN_VEHICLES(void);
static void RESET_ORIGINAL(void);
static void AFFECTED_CAR(void);
static void CAR_DIMMER(void);
static void LIGHT_INIT(void);
static void LIGHT_OFF(void);
static void LIGHT_ON(void);
void INIT_PEDALCHK(void);
void GETCHOICE(void);
void PEDALCHK(void);
void RACESEL_TIMER(void);
static void WAITINTROTIMER(void);
void INTROTIMER(void);
void DIAL_ROUT(void);
void ENDPLAYER(void);
static void GAME_AVAILABLEP(void);
void _start(void);
static void ULTRA_PROC(PROC* p);
void ULTRA_LOGO(void);
void LOGO_SMALL(void);
void SET_ATTR(void);
void CYCLE_ATTR(void);
void INSMORE(void);
static void COIN_CNTDOWN(void);
void LOAD_SHARED(void);
static void SHOW_RACE_NAME(void);

#define CCTI CCT
#define CCTABI CCTAB
#define RACE_STARTING_POINTSI RACE_STARTING_POINTS
#define SCS_TABI SCS_TAB
#define CARPAL_TABLEI CARPAL_TABLE
#define CARSRCPAL_TABI CARSRCPAL_TAB
#define HIDDEN_TABLEI HIDDEN_TABLE
#define RGBTAB_CPI RGBTAB_CP

extern int BOILEROBJ;
void BOILERPLATE_INIT(void);
void CHECK_MOTION_DIP(void);
void CHECK_MOTION_PRESENT(void);
void ABORT_RESET_GALIL(void);
extern const char XQ[];
void SEND_CMD(void);
void WAIT_ACK(void);
void GET_CREDITS_TO_START(void);
void GET_CREDITS_TO_CONTINUE(void);

static tCHOOSE_CAR_ENTRY CCTAB[];
static int TRAFFIC_LL[7];

/* asm: START_HIT	.bss	START_HIT,1 */
int START_HIT;
/* asm: CHOSEN_VEHICLE	.bss	CHOSEN_VEHICLE,1 */
int CHOSEN_VEHICLE;
/* asm: CHOSEN_TRANSMISSION	.bss	CHOSEN_TRANSMISSION,1 */
int CHOSEN_TRANSMISSION;
/* asm: CHOSEN_RACE	.bss	CHOSEN_RACE,1 */
int CHOSEN_RACE;
/* asm: END_OF_GAMEP	.bss	END_OF_GAMEP,1 */
int END_OF_GAMEP;
/* asm: RACE_MODE	.bss	RACE_MODE,1 */
int RACE_MODE;
/* asm: FIRST_RACE	.bss	FIRST_RACE,1 */
int FIRST_RACE;
/* asm: POSES	.bss	POSES,1 */
int POSES;
/* asm: GAMEDIFF	.bss	GAMEDIFF,1 */
float GAMEDIFF;
/* asm: CHECKPOINT_NUM	.bss	CHECKPOINT_NUM,1 */
int CHECKPOINT_NUM;
/* asm: H2H_FLAGSTATE	.bss	H2H_FLAGSTATE,1 */
int H2H_FLAGSTATE;
static const char JINOW[] = "JOIN IN NOW";

void HEAD2HEAD_LOGO_WAIT(void) {
    // asm 0000154D: 	LDI	1,AR6
    // asm 0000154E: 	BU	H2HLE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEAD_LOGO_WAIT", 0, 0);
    UNIMPL();
}

void HEAD2HEAD_LOGO(void) {
    // asm 0000154F: 	CLRI	AR6
H2HLE:
    // asm 00001550: 	LDI	2,R0
    // asm 00001551: 	STI	R0,@FRAMRATE	;frame governor
    // asm 00001552: 	LDI	@_MODE,R0	;shut off infinity
    // asm 00001553: 	AND	MMODE,R0
    // asm 00001554: 	STI	R0,@_MODE
    // asm 00001555: 	LDL	redhd1,AR2
    // asm 00001556: 	LDI	0,R2
    // asm 00001557: 	LDI	0,R3
    // asm 00001558: 	LDI	368,RC
    // asm 00001559: 	CALL	OBJ_QMAKE
    // asm 0000155A: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 0000155B: 	OR	*+AR0(OFLAGS),R0
    // asm 0000155C: 	STI	R0,*+AR0(OFLAGS)
    // asm 0000155D: 	LDI	AR0,AR2
    // asm 0000155E: 	CALL	OBJ_INSERTP
    // asm 0000155F: 	LDL	H2HPAL2,AR2
    // asm 00001560: 	CALL	PAL_FIND_RAW
    // asm 00001561: 	STI	R0,*+AR0(OPAL)
    // asm 00001562: 	STI	AR0,*+AR7(PDATA)	;red (bottom)
    // asm 00001563: 	LDL	yelhd1,AR2
    // asm 00001564: 	LDI	0,R2
    // asm 00001565: 	LDI	0,R3
    // asm 00001566: 	LDI	368,RC
    // asm 00001567: 	CALL	OBJ_QMAKE
    // asm 00001568: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 00001569: 	OR	*+AR0(OFLAGS),R0
    // asm 0000156A: 	STI	R0,*+AR0(OFLAGS)
    // asm 0000156B: 	LDI	AR0,AR2
    // asm 0000156C: 	CALL	OBJ_INSERTP
    // asm 0000156D: 	LDL	H2HPAL2,AR2
    // asm 0000156E: 	CALL	PAL_FIND_RAW
    // asm 0000156F: 	STI	R0,*+AR0(OPAL)
    // asm 00001570: 	STI	AR0,*+AR7(PDATA+1)	;yellow (bottom)
    // asm 00001571: 	LDL	big2,AR2
    // asm 00001572: 	LDI	0,R2
    // asm 00001573: 	LDI	0,R3
    // asm 00001574: 	LDI	368,RC
    // asm 00001575: 	CALL	OBJ_QMAKE
    // asm 00001576: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 00001577: 	OR	*+AR0(OFLAGS),R0
    // asm 00001578: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001579: 	LDI	AR0,AR2
    // asm 0000157A: 	CALL	OBJ_INSERTP
    // asm 0000157B: 	LDL	H2HPAL3,AR2
    // asm 0000157C: 	CALL	PAL_FIND_RAW
    // asm 0000157D: 	STI	R0,*+AR0(OPAL)
    // asm 0000157E: 	STI	AR0,*+AR7(PDATA+2)	;big2
    // asm 0000157F: 	FLOAT	160,R6	;60+4*25
    // asm 00001580: 	FLOAT	4000,R7			;big2
    // asm 00001581: 	LDI	25,AR5
    // asm 00001582: H2HLLP
    // asm 00001582: 	LDI	*+AR7(PDATA+1),AR0	;red (bottom)
    // asm 00001583: 	STF	R6,*+AR0(OPOSY)
    // asm 00001584: 	LDI	*+AR7(PDATA),AR0	;yellow
    // asm 00001585: 	NEGF	R6,R0
    // asm 00001586: 	STF	R0,*+AR0(OPOSY)
    // asm 00001587: 	LDI	*+AR7(PDATA+2),AR0	;big2
    // asm 00001588: 	STF	R7,*+AR0(OPOSZ)
    // asm 00001589: 	SUBF	4,R6
    // asm 0000158A: 	LDLF	360,R0
    // asm 0000158B: 	SUBF	R0,R7,R1
    // asm 0000158C: 	MPYF	0.1,R1
    // asm 0000158D: 	SUBF	R1,R7
    // asm 0000158E: 	CALL	JINMSG
    // asm 0000158F: 	CALL	KABOSHP
    // asm 00001590: 	BC	KABOSH
    // asm 00001591: 	SLEEP	1
    // asm 00001593: 	DBU	AR5,H2HLLP
    // asm 00001594: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm 00001596: 	CMPI	1,R0
    // asm 00001597: 	BNE	NOSND1
    // asm 00001598: 	SOND1	STAMP
NOSND1:
    // 	;cycle HEAD   2    HEAD punch
    // 	;
    // asm 0000159A: 	LDI	*+AR7(PDATA),AR4	;yellow
    // asm 0000159B: 	FLOAT	120,R6
    // asm 0000159C: 	JSRP	THROBIT
    // asm 000015A2: 	CALL	KABOSHP
    // asm 000015A3: 	BC	KABOSH
    // asm 000015A4: 	LDI	*+AR7(PDATA+2),AR4	;big2
    // asm 000015A5: 	FLOAT	300,R6
    // asm 000015A6: 	JSRP	THROBIT
    // asm 000015AC: 	CALL	KABOSHP
    // asm 000015AD: 	BC	KABOSH
    // asm 000015AE: 	LDI	*+AR7(PDATA+1),AR4	;red (bottom)
    // asm 000015AF: 	FLOAT	120,R6
    // asm 000015B0: 	JSRP	THROBIT
    // asm 000015B6: 	CALL	KABOSHP
    // asm 000015B7: 	BC	KABOSH
    // 	;
    // 	;
    // asm 000015B8: 	CMPI	1,AR6		;are we waiting for a joiner??
    // asm 000015B9: 	BEQ	WFC
    // asm 000015BA: 	CALL	KABOSHP
    // asm 000015BB: 	BC	KABOSH
    // asm 000015BC: 	SLEEP	30
    // asm 000015BE: 	BR	CYCLE_ATTR
WFC:
    // asm 000015BF: 	LDI	1,R0
    // asm 000015C0: 	STI	R0,@FRAMRATE	;frame governor
WFCLP342:
    // asm 000015C1: 	LDI	@ICF,R0
    // asm 000015C2: 	BGT	NOMSGG
    // asm 000015C3: 	LDL	JINOW,AR2
    // asm 000015C4: 	FLOAT	256,R2
    // asm 000015C5: 	FLOAT	100,R3
    // asm 000015C6: 	LDI	1,RC
    // asm 000015C7: 	CALL	TEXT_ADD
    // asm 000015C8: 	CALL	SET40FONT
    // asm 000015C9: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOMSGG:
    // asm 000015CC: 	SLEEP	1
    // asm 000015CE: 	CALL	KABOSHP
    // asm 000015CF: 	BNC	WFCLP342
    // ;	LDI	@TRANSMISSION_ACTIVE,R0
    // ;	BZ	KABOSH
    // ;
    // ;	LDI	@OM_MODE,R0
    // ;	AND	MMODE,R0
    // ;	CMPI	MBONUS,R0
    // ;	BEQ	WFCLP342
    // ;	CMPI	MINIT,R0
    // ;	BEQ	WFCLP342
    // ;	CMPI	MINSERT_COINS,R0
    // ;	BEQ	WFCLP342
    // ;
    // ;	LDI	@OM_LINKWAIT,R0
    // ;	BNZ	WFCLP342
KABOSH:
    // asm 000015D0: 	CALL	CLEAR_LINK
    // asm 000015D1: 	LDI	-2,R0
    // asm 000015D2: 	STI	R0,@_ATTR_MODE
    // asm 000015D3: 	BU	SET_ATTR
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEAD_LOGO", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *RETURNS CARRY SET ON KABOSH THIS MESS
 *
 *
 */
static void KABOSHP(void) {
    // asm 000015D4: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 000015D5: 	BZ	TKABOSH
    // asm 000015D6: 	LDI	@OM_MODE,R0
    // asm 000015D7: 	AND	MMODE,R0
    // asm 000015D8: 	CMPI	MBONUS,R0
    // asm 000015D9: 	BEQ	FKABOSH
    // asm 000015DA: 	CMPI	MINIT,R0
    // asm 000015DB: 	BEQ	FKABOSH
    // asm 000015DC: 	CMPI	MINSERT_COINS,R0
    // asm 000015DD: 	BEQ	FKABOSH
    // asm 000015DE: 	CMPI	MATTR,R0
    // asm 000015DF: 	BEQ	TKABOSH
    // asm 000015E0: 	LDI	@OM_LINKWAIT,R0
    // asm 000015E1: 	BZ	TKABOSH
FKABOSH:
    // asm 000015E2: 	CLRC
    // asm 000015E3: 	RETS
TKABOSH:
    // asm 000015E4: 	SETC
    // asm 000015E5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KABOSHP", 0, 0);
    UNIMPL();
}

static void JINMSG(void) {
    // asm 000015E6: 	CMPI	1,AR6
    // asm 000015E7: 	BNE	NOJINMSG
    // asm 000015E8: 	LDL	JINOW,AR2
    // asm 000015E9: 	FLOAT	256,R2
    // asm 000015EA: 	FLOAT	100,R3
    // asm 000015EB: 	LDI	1,RC
    // asm 000015EC: 	CALL	TEXT_ADD
    // asm 000015ED: 	CALL	SET40FONT
    // asm 000015EE: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOJINMSG:
    // asm 000015F1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JINMSG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
static void THROBIT(void) {
    // asm 000015F2: 	LDI	1,AR5
    // asm 000015F3: THROBLP1
    // asm 000015F3: 	LDF	*+AR4(OPOSZ),R0
    // asm 000015F4: 	SUBF	R6,R0
    // asm 000015F5: 	STF	R0,*+AR4(OPOSZ)
    // asm 000015F6: 	CALL	JINMSG
    // asm 000015F7: 	CALL	KABOSHP
    // asm 000015F8: 	BC	ENDTHROB
    // asm 000015F9: 	SLEEP	1
    // asm 000015FB: 	DBU	AR5,THROBLP1
    // asm 000015FC: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm 000015FE: 	CMPI	1,R0
    // asm 000015FF: 	BNE	NOSND2
    // asm 00001600: 	SOND1	STAMP
NOSND2:
    // asm 00001602: 	LDI	4,AR5
KKGKG:
    // asm 00001603: CALL	JINMSG
    // asm 00001604: 	CALL	KABOSHP
    // asm 00001605: 	BC	ENDTHROB
    // asm 00001606: 	SLEEP	1
    // asm 00001608: 	DBU	AR5,KKGKG
    // asm 00001609: 	LDI	1,AR5
    // asm 0000160A: THROBLP2
    // asm 0000160A: 	LDF	*+AR4(OPOSZ),R0
    // asm 0000160B: 	ADDF	R6,R0
    // asm 0000160C: 	STF	R0,*+AR4(OPOSZ)
    // asm 0000160D: 	CALL	JINMSG
    // asm 0000160E: 	CALL	KABOSHP
    // asm 0000160F: 	BC	ENDTHROB
    // asm 00001610: 	SLEEP	1
    // asm 00001612: 	DBU	AR5,THROBLP2
ENDTHROB:
    // asm 00001613: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THROBIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static const char WFCHAL1[] = "WAITING";
static const char WFCHAL2[] = "FOR CHALLENGER";
static const char WFCHAL3[] = "HOLD RADIO OR VIEW3 TO CANCEL";
/* asm: FRAMELAG	.bss	FRAMELAG,1 */
int FRAMELAG;

/*
 *
 *
 */
void WAIT_FOR_CHALLENGER(void) {
    // asm 00001617: 	LDI	5,R0
    // asm 00001618: 	STI	R0,@FRAMELAG
    // asm 00001619: 	CLRI	R0
    // asm 0000161A: 	STI	R0,@BOILEROBJ
    // 	;IF NO TRANSMISSIONS ARE HAPPENING
    // 	;(OTHER == SINGLE PLAYER) THEN IGNORE THIS MESS AND ASSUME
    // 	;SINGLE PLAYER
    // 	;
    // asm 0000161B: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 0000161C: 	BZ	WFCLPXXX
    // asm 0000161D: 	LDI	@NOASK_LINK,R0
    // asm 0000161E: 	BNZ	WFCLPXXX
    // asm 0000161F: 	LDI	@FIRST_RACE,R0
    // asm 00001620: 	BNZ	CHAHC
    // asm 00001621: 	CALL	BOILERPLATE_INIT
    // asm 00001622: 	LDI	AR0,AR2
    // asm 00001623: 	CALL	OBJ_INSERTP
CHAHC:
    // asm 00001624: 	CLRI	R0
    // asm 00001625: 	STI	R0,@START_HIT
    // asm 00001626: 	STI	R0,@miniidle
    // asm 00001627: 	LDI	@_MODE,R0
    // asm 00001628: 	OR	MGO,R0
    // asm 00001629: 	STI	R0,@_MODE
    // 	;if no challenger
    // asm 0000162A: 	LDI	20,R0
    // asm 0000162B: 	STI	R0,@_countdown
    // asm 0000162C: 	CALL	SEND_VEHICLE
WFCLP:
    // asm 0000162D: 	CALL	SEND_VEHICLE
    // asm 0000162E: 	LDI	@COINDROP,R0
    // asm 0000162F: 	BZ	NOWORRY
    // asm 00001630: 	CLRI	R0
    // asm 00001631: 	STI	R0,@COINDROP
    // asm 00001632: 	LDI	20,R1
    // asm 00001633: 	BU	JJFHF
NOWORRY:
    // asm 00001634: 	LDI	@_countdown,R1
    // asm 00001635: 	LDI	@OM_MODE,R0
    // asm 00001636: 	AND	MMODE,R0
    // asm 00001637: 	CMPI	MINIT,R0
    // asm 00001638: 	LDIEQ	20,R1
    // asm 00001639: 	CMPI	MBONUS,R0
    // asm 0000163A: 	LDIEQ	20,R1
JJFHF:
    // asm 0000163B: STI	R1,@_countdown
    // 	;IF challenger found THEN wait until ready then exit
    // 	;
    // asm 0000163C: 	LDI	@HEAD2HEAD_ON,R0
    // asm 0000163D: 	BZ	BABAB
    // asm 0000163E: 	LDI	@OM_VEHICLE,R0
    // asm 0000163F: 	BGE	WAITX		;WE'RE THERE DUDE!
BABAB:
    // asm 00001640: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001641: 	BNZ	HHFBF
    // asm 00001642: 	LDI	@_countdown,R0
    // asm 00001643: 	BLE	WFCLPXXX
HHFBF:
    // asm 00001644: 	LDI	@FRAMELAG,R0
    // asm 00001645: 	DEC	R0
    // asm 00001646: 	LDILT	0,R0
    // asm 00001647: 	STI	R0,@FRAMELAG
    // asm 00001648: 	BGT	NOMSG768
    // asm 00001649: 	LDL	WFCHAL1,AR2
    // asm 0000164A: 	FLOAT	256,R2
    // asm 0000164B: 	FLOAT	100,R3
    // asm 0000164C: 	LDI	1,RC
    // asm 0000164D: 	CALL	TEXT_ADD
    // asm 0000164E: 	CALL	SET40FONT
    // asm 0000164F: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001652: 	LDL	WFCHAL2,AR2
    // asm 00001653: 	FLOAT	256,R2
    // asm 00001654: 	FLOAT	146,R3
    // asm 00001655: 	LDI	1,RC
    // asm 00001656: 	CALL	TEXT_ADD
    // asm 00001657: 	CALL	SET40FONT
    // asm 00001658: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOMSG768:
    // 	;if it is a linked game AND he hasn't chosen his vehicle
    // 	;then wait for him to choose his vehicle
    // 	;
    // asm 0000165B: 	LDI	@HEAD2HEAD_ON,R0
    // asm 0000165C: 	BZ	BABAHH
    // asm 0000165D: 	SLEEP	1
    // asm 0000165F: 	BU	WFCLP
BABAHH:
    // *ELP CHANGE
    // ;	LDI	@miniidle,R0
    // ;	INC	R0
    // ;	CMPI	2,R0
    // ;	LDIGE	0,R0
    // ;	STI	R0,@miniidle
    // ;	CMPI	0,R0
    // ;	BNE	JJFJFJ
    // asm 00001660: 	LDI	@SWITCHBUTS,R0
    // asm 00001661: 	RS	16,R0
    // ;	AND	SW_VIEW0_H|SW_VIEW1_H|SW_VIEW2_H|SW_RADIO_H,R0
    // asm 00001662: 	AND	SW_VIEW2_H|SW_RADIO_H,R0
    // asm 00001663: 	BZ	JJFJFJ
    // asm 00001664: 	LDI	@_countdown,R0
    // asm 00001665: 	DEC	R0
    // asm 00001666: 	LDILT	0,R0
    // asm 00001667: 	STI	R0,@_countdown
JJFJFJ:
    // *ELP END CHANGE
    // asm 00001668: 	LDI	@FRAMELAG,R0
    // asm 00001669: 	BGT	NOMSG123
    // asm 0000166A: 	CALL	WAITINTROTIMER
    // asm 0000166B: 	LDL	WFCHAL3,AR2
    // asm 0000166C: 	FLOAT	256,R2
    // asm 0000166D: 	FLOAT	192,R3
    // asm 0000166E: 	LDI	1,RC
    // asm 0000166F: 	CALL	TEXT_ADDDS
    // asm 00001670: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001673: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
NOMSG123:
    // asm 00001676: 	SLEEP	1
    // asm 00001678: 	BU	WFCLP
WFCLPXXX:
    // asm 00001679: 	CLRI	R0
    // asm 0000167A: 	STI	R0,@MY_LINKWAIT
    // asm 0000167B: 	STI	R0,@HEAD2HEAD_ON
    // asm 0000167C: 	CALL	SEND_LINKCANCELLED
    // asm 0000167D: 	SLEEP	1
    // asm 0000167F: 	CALL	SETONE		;1 PLAYER GAME
WAITX:
    // 	;*****
    // 	;*****  WE CAN NO LONGER ACCEPT A DUDE
    // 	;*****
    // 	;*****
    // asm 00001680: 	LDI	@BOILEROBJ,AR2
    // asm 00001681: 	CMPI	0,AR2
    // asm 00001682: 	CALLNE	OBJ_DELETE
    // asm 00001683: 	LDI	@_MODE,R0
    // asm 00001684: 	ANDN	MGO,R0
    // asm 00001685: 	STI	R0,@_MODE
    // asm 00001686: 	LDI	5,R0			;so player doesn't puke (see near call)
    // asm 00001687: 	STI	R0,@_countdown
    // asm 00001688: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_FOR_CHALLENGER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *an issue start game has been called and
 *it has been determined that the other machine
 *is either in MBONUS or MINIT.  at that point
 *we will wait for the other machine to enter
 *MATTR, or MINTRO
 *
 *
 *
 */
static void CHECK_ENDBONUS(void) {
    // asm 0000168C: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 0000168D: 	BZ	CEBT
    // asm 0000168E: 	LDI	@OM_BONUS_WAITFLAG,R0
    // asm 0000168F: 	BNZ	CEBT
    // asm 00001690: 	LDI	@OM_LINKWAIT,R0
    // asm 00001691: 	BNZ	CEBT
    // asm 00001692: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001693: 	BNZ	CEBT
    // asm 00001694: 	LDI	@OM_MODE,R0
    // asm 00001695: 	AND	MMODE,R0
    // asm 00001696: 	CMPI	MATTR,R0
    // asm 00001697: 	BEQ	CEBT
    // asm 00001698: 	CLRC
    // asm 00001699: 	RETS
CEBT:
    // asm 0000169A: 	SETC
    // asm 0000169B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_ENDBONUS", 0, 0);
    UNIMPL();
}

/* asm: BONUS_WAITFLAG	pbss	BONUS_WAITFLAG,1 */
int BONUS_WAITFLAG;
/* asm: OM_BONUS_WAITFLAG	pbss	OM_BONUS_WAITFLAG,1 */
int OM_BONUS_WAITFLAG;

static void WAIT_FOR_ENDBONUS(void) {
    // asm 0000169C: 	PUSHP	R6
    // asm 0000169F: 	SLEEP	2
    // asm 000016A1: 	CALL	CHECK_ENDBONUS
    // asm 000016A2: 	BC	ISGAME5
    // asm 000016A3: 	POPP	R6
    // asm 000016A6: 	CMPI	1,R6
    // asm 000016A7: 	BEQ	NOOBJSSS
    // asm 000016A8: 	LDL	H2HPAL2,AR2
    // asm 000016A9: 	CALL	PAL_ALLOC_RAW
    // asm 000016AA: 	LDL	H2HPAL3,AR2
    // asm 000016AB: 	CALL	PAL_ALLOC_RAW
    // asm 000016AC: 	LDL	redhd1,AR2
    // asm 000016AD: 	LDI	0,R2
    // asm 000016AE: 	LDI	-60,R3
    // asm 000016AF: 	LDI	368,RC
    // asm 000016B0: 	CALL	OBJ_QMAKE
    // asm 000016B1: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 000016B2: 	OR	*+AR0(OFLAGS),R0
    // asm 000016B3: 	STI	R0,*+AR0(OFLAGS)
    // asm 000016B4: 	LDI	AR0,AR2
    // asm 000016B5: 	PUSHP	AR2
    // asm 000016B8: 	CALL	OBJ_INSERTP
    // asm 000016B9: 	LDI	AR2,AR0
    // asm 000016BA: 	LDL	H2HPAL2,AR2
    // asm 000016BB: 	CALL	PAL_FIND_RAW
    // asm 000016BC: 	STI	R0,*+AR0(OPAL)
    // asm 000016BD: 	LDL	yelhd1,AR2
    // asm 000016BE: 	LDI	0,R2
    // asm 000016BF: 	LDI	60,R3
    // asm 000016C0: 	LDI	368,RC
    // asm 000016C1: 	CALL	OBJ_QMAKE
    // asm 000016C2: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 000016C3: 	OR	*+AR0(OFLAGS),R0
    // asm 000016C4: 	STI	R0,*+AR0(OFLAGS)
    // asm 000016C5: 	LDI	AR0,AR2
    // asm 000016C6: 	PUSHP	AR2
    // asm 000016C9: 	CALL	OBJ_INSERTP
    // asm 000016CA: 	LDI	AR2,AR0
    // asm 000016CB: 	LDL	H2HPAL2,AR2
    // asm 000016CC: 	CALL	PAL_FIND_RAW
    // asm 000016CD: 	STI	R0,*+AR0(OPAL)
    // asm 000016CE: 	LDL	big2,AR2
    // asm 000016CF: 	LDI	0,R2
    // asm 000016D0: 	LDI	0,R3
    // asm 000016D1: 	LDI	368,RC
    // asm 000016D2: 	CALL	OBJ_QMAKE
    // asm 000016D3: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 000016D4: 	OR	*+AR0(OFLAGS),R0
    // asm 000016D5: 	STI	R0,*+AR0(OFLAGS)
    // asm 000016D6: 	LDI	AR0,AR2
    // asm 000016D7: 	PUSHP	AR2
    // asm 000016DA: 	CALL	OBJ_INSERTP
    // asm 000016DB: 	LDI	AR2,AR0
    // asm 000016DC: 	LDL	H2HPAL3,AR2
    // asm 000016DD: 	CALL	PAL_FIND_RAW
    // asm 000016DE: 	STI	R0,*+AR0(OPAL)
    // asm 000016DF: 	BU	LKJASDFGD
NOOBJSSS:
    // asm 000016E0: 	CALL	BOILERPLATE_INIT
    // asm 000016E1: 	LDI	AR0,AR2
    // asm 000016E2: 	CALL	OBJ_INSERTP
LKJASDFGD:
    // asm 000016E3: 	PUSHP	R6
WAIT_FOR_ENDBONUS_LP:
    // asm 000016E6: 	LDI	1,R0
    // asm 000016E7: 	STI	R0,@BONUS_WAITFLAG
    // asm 000016E8: 	LDL	WFCHAL1,AR2
    // asm 000016E9: 	FLOAT	256,R2
    // asm 000016EA: 	FLOAT	100,R3
    // asm 000016EB: 	LDI	1,RC
    // asm 000016EC: 	CALL	TEXT_ADD
    // asm 000016ED: 	CALL	SET40FONT
    // asm 000016EE: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000016F1: 	LDL	WFCHAL2,AR2
    // asm 000016F2: 	FLOAT	256,R2
    // asm 000016F3: 	FLOAT	160,R3
    // asm 000016F4: 	LDI	1,RC
    // asm 000016F5: 	CALL	TEXT_ADD
    // asm 000016F6: 	CALL	SET40FONT
    // asm 000016F7: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000016FA: 	CALL	CHECK_ENDBONUS
    // asm 000016FB: 	BC	ISGAME5
    // asm 000016FC: 	SLEEP	1
    // asm 000016FE: 	BU	WAIT_FOR_ENDBONUS_LP
ISGAME5:
    // asm 000016FF: 	CLRI	R0
    // asm 00001700: 	STI	R0,@BONUS_WAITFLAG
    // asm 00001701: 	SLEEP	2
    // asm 00001703: 	POPP	R6
    // asm 00001706: 	CMPI	1,R6
    // asm 00001707: 	BEQ	NOJHASD
    // asm 00001708: 	POPP	AR2
    // asm 0000170B: 	CALL	OBJ_DELETE
    // asm 0000170C: 	POPP	AR2
    // asm 0000170F: 	CALL	OBJ_DELETE
    // asm 00001710: 	POPP	AR2
    // asm 00001713: 	CALL	OBJ_DELETE
    // asm 00001714: 	LDL	H2HPAL2,AR2
    // asm 00001715: 	CALL	PAL_DELETE_RAW
    // asm 00001716: 	LDL	H2HPAL3,AR2
    // asm 00001717: 	CALL	PAL_DELETE_RAW
    // asm 00001718: 	BU	IURENDFL
NOJHASD:
    // asm 00001719: 	LDI	@BOILEROBJ,AR2
    // asm 0000171A: 	CMPI	0,AR2
    // asm 0000171B: 	CALLNE	OBJ_DELETE
IURENDFL:
    // asm 0000171C: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_FOR_ENDBONUS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void ISSUE_STARTGAME_TSEL(void) {
    // asm 00001720: 	LDI	1,R6
    // asm 00001721: 	BU	LKAS534
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISSUE_STARTGAME_TSEL", 0, 0);
    UNIMPL();
}

void ISSUE_STARTGAME(void) {
    // asm 00001722: 	CLRI	R6
LKAS534:
    // asm 00001723: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 00001724: 	BZ	NOGAME
    // ;	LDI	@OM_MODE,R0
    // ;	AND	MMODE,R0
    // ;	CMPI	MGAME,R0
    // ;	BEQ	NOGAME
    // *ELP CHANGE
    // asm 00001725: 	LDI	@FIRST_RACE,R0
    // asm 00001726: 	BNZ	NOTNND
    // asm 00001727: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm 00001728: 	BNZ	NOTNND
    // asm 00001729: 	LDI	@OM_MODE,R0
    // asm 0000172A: 	AND	MMODE,R0
    // asm 0000172B: 	CMPI	MBONUS,R0
    // asm 0000172C: 	BEQ	NOGAME
    // asm 0000172D: 	CMPI	MINIT,R0
    // asm 0000172E: 	BEQ	NOGAME
    // asm 0000172F: 	CMPI	MINSERT_COINS,R0
    // asm 00001730: 	BEQ	NOGAME
NOTNND:
    // 	;IF THE OTHER GAME IS:
    // 	;	 MBONUS
    // 	;	 MINIT
    // 	;THEN
    // 	;	PLACE WAIT FOR CHALLENGER ON SCREEN
    // 	;	IF PLAYER WAITS THEN WAIT FOR OTHER ISSUE START GAME
    // 	;	ELSE IF PLAYER SINGLE THEN SINGLE PLAYER
    // 	;
    // asm 00001731: 	LDI	@OM_MODE,R0
    // asm 00001732: 	AND	MMODE,R0
    // asm 00001733: 	CMPI	MBONUS,R0
    // asm 00001734: 	BEQ	DOIT6
    // asm 00001735: 	CMPI	MINSERT_COINS,R0
    // asm 00001736: 	BEQ	DOIT6
    // asm 00001737: 	CMPI	MINIT,R0
    // asm 00001738: 	BNE	NODOIT6
DOIT6:
    // asm 00001739: 	JSRP	WAIT_FOR_ENDBONUS
NODOIT6:
    // asm 0000173F: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm 00001740: 	BNZ	DOITANY4
    // asm 00001741: 	LDI	@OM_BONUS_WAITFLAG,R0
    // asm 00001742: 	BNZ	DOITANY4
    // asm 00001743: 	LDI	@_ATTR_MODE,R0
    // asm 00001744: 	CMPI	-7,R0
    // asm 00001745: 	BEQ	DOITANY4
    // asm 00001746: 	LDI	@OM_MODE,R0
    // asm 00001747: 	AND	MMODE,R0
    // asm 00001748: 	CMPI	MATTR,R0
    // asm 00001749: 	BEQ	DOITANY4
    // *ELP CHANGE
    // ;	BNE	PLAPA55
    // ;	LDI	@FIRST_RACE,R0
    // ;	BNZ	DOITANY4
    // ;PLAPA55
    // *ELP END CHANGE
    // 	;we were not playing a linked game,
    // 	;AND we were both playing race across
    // 	;the usa
    // 	;THEREFORE
    // 	;to continue is to play a single player game
    // 	;
    // *ELP CHANGE February 8,1995
    // 	;when the player hits start there is a slight pause
    // 	;when the player goes into INTRO (1) but before
    // 	;he goes to issue start game (2)
    // 	;(1) MINTRO on start button decode then calls COLD_ENTER
    // 	;(2) creates the PLYR_INTRO process, and 1st sequence
    // 	;	then sends the LINK predicate to the other machine
    // 	;(note:)
    // 	;  a.	prior to FEB 8th the game would go to MGAME
    // 	;  b.	the game needs to switch modes to note drain the coins
    // 	;
    // 	;
    // asm 0000174A: 	CMPI	MINTRO,R0
    // asm 0000174B: 	BNE	NOGAME
    // ;	BU	NOGAME
    // *ELP END CHANGE
DOITANY4:
    // asm 0000174C: 	CLRI	R0
    // asm 0000174D: 	STI	R0,@BONUS_WAITFLAG
    // *ELP END CHANGE
    // asm 0000174E: 	CLRI	R0
    // asm 0000174F: 	STI	R0,@LINKEDP
    // asm 00001750: 	CALL	SEND_START_GAME
    // asm 00001751: 	LDI	30,AR5
WTFORRETVAL:
    // asm 00001752: 	DEC	AR5
    // asm 00001753: 	CMPI	0,AR5
    // asm 00001754: 	BLE	NTINLK
    // asm 00001755: 	SLEEP	1
    // *ELP CHANGE
    // asm 00001757: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 00001758: 	BZ	NOGAME
    // *ELP END CHANGE
    // asm 00001759: 	LDI	@LINKEDP,R0	;0 = invalid
    // 				;1 = linked
    // 				;2 = not linked
    // asm 0000175A: 	BZ	WTFORRETVAL
    // asm 0000175B: 	CMPI	1,R0
    // asm 0000175C: 	BNE	NTINLK
    // asm 0000175D: 	LDI	1,R0
    // asm 0000175E: 	STI	R0,@HEAD2HEAD_ON
    // asm 0000175F: 	INCAUD	AUD_H2HGAMES
    // asm 00001761: 	CLRI	R0
    // asm 00001762: 	STI	R0,@OM_LINKWAIT
    // asm 00001763: 	STI	R0,@MY_LINKWAIT
NTINLK:
    // asm 00001764: 	CLRI	R0
    // asm 00001765: 	STI	R0,@NOASK_LINK
    // asm 00001766: 	RETP
NOGAME:
    // *ELP CHANGE
    // asm 0000176A: 	CALL	SEND_LINKCANCELLED
    // asm 0000176B: 	SLEEP	1
    // *ELP END CHANGE
    // asm 0000176D: 	CALL	CLEAR_LINK
    // asm 0000176E: 	LDI	1,R0
    // asm 0000176F: 	STI	R0,@NOASK_LINK
    // asm 00001770: 	CALL	SETONE
    // asm 00001771: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISSUE_STARTGAME", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void PLYR_INTRO(void) {
    // asm 00001775: 	LDI	RM_SINGLE,R0
    // asm 00001776: 	STI	R0,@RACE_MODE
    // asm 00001777: 	LDI	MINTRO|MGO,R0
    // asm 00001778: 	STI	R0,@_MODE
    // asm 00001779: 	CLRI	R0
    // asm 0000177A: 	STI	R0,@WAS_HEAD2HEAD_ON
    // asm 0000177B: 	LDI	1,R0
    // asm 0000177C: 	STI	R0,@FIRST_RACE
    // asm 0000177D: 	JSRP	ISSUE_STARTGAME
    // asm 00001783: 	CALL	INIT_LASTHS_TABLE		;Initialize the table for players hs entries
    // asm 00001784: 	LDI	-1,R2
    // asm 00001785: 	SETADJ	ADJ_INITIALS
    // asm 00001787: 	INCAUD	AUD_GAMENUMBER
    // asm 00001789: 	INCAUD	AUD_GAMES_START
    // asm 0000178B: 	CALL	HSTDEC
    // asm 0000178C: 	CLRI	R0
    // asm 0000178D: 	STI	R0,@BONUS_WAVE
    // asm 0000178E: 	LDF	1.0,R0
    // asm 0000178F: 	STF	R0,@GAMEDIFF
    // asm 00001790: 	READAUD	AUD_UNFINISHED_GAMES
    // asm 00001792: 	PUSH	R0
    // asm 00001793: 	READAUD	AUD_UNFINISHED_GAMES_FOUND
    // asm 00001795: 	POP	R1
    // asm 00001796: 	CMPI	R1,R0
    // asm 00001797: 	BEQ	NOULOG
    // asm 00001798: 	ERRON	U,EC_UNFINISHED
    // asm 000017A0: 	LDI	R1,R2
    // asm 000017A1: 	SETAUD	AUD_UNFINISHED_GAMES_FOUND
NOULOG:
    // asm 000017A3: 	INCAUD	AUD_NUM_UNFINISHED
    // asm 000017A5: 	LDI	1,R0				;SHUFFLE DRIVIN
    // asm 000017A6: 	STI	R0,@TUNE_IDX
CNR_ENTER:
    // asm 000017A7: 	LDP	@IN_RESET_MODE
    // asm 000017A8: 	LDI	@IN_RESET_MODE,R0
    // asm 000017A9: 	SETDP
    // asm 000017AA: 	BZ	CONTINUE
    // asm 000017AB: 	SLEEP	1
    // asm 000017AD: 	BU	CNR_ENTER
CONTINUE:
    // asm 000017AE: 	SOND1	START_THEME
    // asm 000017B0: 	LDI	BUT_START,R0		;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm 000017B1: 	STI	R0,@BUTTON_STATUS
    // asm 000017B2: 	CLRI	R0			;INITIALIZE SCORE
    // asm 000017B3: 	STI	R0,@SCORE
    // asm 000017B4: 	STI	R0,@END_OF_GAMEP	;END OF GAME FLAG
    // asm 000017B5: 	STI	R0,@_MPH
    // asm 000017B6: 	STI	R0,@STARTSECTION	;BEGIN LEG OF JOURNEY
    // asm 000017B7: 	STI	R0,@CAR_CHOICE_GOTTEN
    // asm 000017B8: 	STI	R0,@CHOSEN_RACE
    // asm 000017B9: 	LDI	-1,R0
    // asm 000017BA: 	STI	R0,@IS_HIDDEN
    // asm 000017BB: 	CALL	GETCMOS_VALUES
    // asm 000017BC: 	CALL	INIT_PEDALCHK
    // asm 000017BD: 	CALL	OBJ_INIT
    // asm 000017BE: 	CALL	TEXT_INIT
    // asm 000017BF: 	JSRP	TRACK_SELECTION
    // asm 000017C5: 	LDI	@FIRST_RACE,R0
    // asm 000017C6: 	BZ	LOAD_NEW_SELECTION
    // asm 000017C7: 	CALL	INIT_PEDALCHK
    // asm 000017C8: 	CALL	OBJ_INIT
    // asm 000017C9: 	CALL	TEXT_INIT
    // asm 000017CA: 	CLRI	R0
    // asm 000017CB: 	STI	R0,@DCALL
    // asm 000017CC: 	JSRP	CHOOSE_TRANSMISSION
    // asm 000017D2: 	LDI	1,R0
    // asm 000017D3: 	STI	R0,@DCALL
    // asm 000017D4: 	JSRP	CHOOSECAR
    // asm 000017DA: 	LDI	BUT_VIEW2,R0			;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm 000017DB: 	STI	R0,@BUTTON_STATUS
WFSNP:
    // asm 000017DC: SLEEP	1
    // asm 000017DE: 	LDI	@START_NOW_P,R0
    // asm 000017DF: 	BZ	WFSNP
ALL_JOINUP:
    // asm 000017E0: 	LDI	@_MODE,R0
    // asm 000017E1: 	OR	MINFIN,R0
    // asm 000017E2: 	STI	R0,@_MODE
    // asm 000017E3: 	READAUD	ADJ_TIME_TO_START
    // asm 000017E5: 	MPYI	5,R0
    // asm 000017E6: 	ADDI	60,R0
    // asm 000017E7: 	STI	R0,@_countdown
    // asm 000017E8: 	CREATEC	WAVEFLAG,UTIL_C|MONKEY_T
    // asm 000017EB: 	CALL	CLEANUP_TRACKSEL_PALS
    // asm 000017EC: 	LDI	UTIL_C|CHOOSECAR_T,R0
    // asm 000017ED: 	LDI	-1,R1
    // asm 000017EE: 	CALL	PRC_KILLALL
    // 	;
    // 	;SETUP THE GAME
    // 	;
    // asm 000017EF: 	CLRF	R0
    // asm 000017F0: 	STF	R0,@GAME_TIMER
    // asm 000017F1: 	INCAUD	AUD_NUM_BUYINS
    // asm 000017F3: 	LDI	0AAh,R0
    // asm 000017F4: 	STI	R0,@BGNDCOLA
    // asm 000017F5: 	LDI	1,R0	    			;SET GAME FRAME RATE
    // asm 000017F6: 	STI	R0,@FRAMRATE
    // asm 000017F7: 	STI	R0,@TIMECLR
    // asm 000017F8: 	STI	R0,@DRONE_DISPATCH_P
    // asm 000017F9: 	CLRI	R0
    // asm 000017FA: 	STI	R0,@SCREENWIPE_DONE
    // asm 000017FB: 	STI	R0,@NEXT_STARTUP
    // asm 000017FC: 	STI	R0,@CHALLENGE_RACE
    // asm 000017FD: 	LDI	9,R0
    // asm 000017FE: 	STI	R0,@POSITION
    // asm 000017FF: 	READAUD	ADJ_CHECKPOINT_BONUS
    // asm 00001801: 	STI	R0,@CHECKPOINT_TIME_BONUS
    // asm 00001802: 	LDI	1,R0
    // asm 00001803: 	STI	R0,@NOAERASE
    // 	;///////////////////////////////
    // asm 00001804: 	LDI	@FIRST_RACE,R0
    // asm 00001805: 	BZ	_PLYR
    // asm 00001806: 	CLRI	R2				;FIRST WAVE
    // asm 00001807: 	SETAUD	AUD_LAST_LEG
    // asm 00001809: 	LDF	1.0,R0
    // asm 0000180A: 	STF	R0,@WHEELPWR
    // asm 0000180B: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_INTRO", 0, 0);
    UNIMPL();
}

/* asm: START_NOW_P	.bss	START_NOW_P,1 */
int START_NOW_P;
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void CHOOSE_NEXT_RACE(void) {
    // asm 0000180C: 	CALL	TEXT_INIT
    // asm 0000180D: 	LDI	UTIL_C|TEXTP_T,R0
    // asm 0000180E: 	LDI	-1,R1
    // asm 0000180F: 	CALL	PRC_KILLALL
    // asm 00001810: 	LDI	PLYR_C|PLYR1_T,R0
    // asm 00001811: 	LDI	-1,R1
    // asm 00001812: 	CALL	PRC_KILLALL
    // asm 00001813: 	LDI	@_MODE,R0
    // asm 00001814: 	ANDN	MINFIN|MWATER,R0
    // asm 00001815: 	STI	R0,@_MODE
    // asm 00001816: 	CALL	OBJ_INIT
    // asm 00001817: 	CALL	DYNAOBJ_INIT	;init DYNAMIC OBJECTS
    // asm 00001818: 	CALL	CARB_INIT	;init CAR BLOCKS
    // asm 00001819: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    // asm 0000181A: 	CLRI	R0
    // asm 0000181B: 	STI	R0,@FIRST_RACE
    // 	;
    // asm 0000181C: 	SLEEP	4
    // 	;
    // asm 0000181E: 	BU	CNR_ENTER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOOSE_NEXT_RACE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void LOAD_NEW_SELECTION(void) {
    // asm 0000181F: 	LDI	@CHOSEN_RACE,AR0
    // asm 00001820: 	ADDI	@FULLSETUP_TABLEI,AR0
    // asm 00001821: 	LDI	*AR0,R0
    // asm 00001822: 	CALLU	R0
    // asm 00001823: 	LDI	MINTRO|MINFIN,R0
    // asm 00001824: 	STI	R0,@_MODE
    // asm 00001825: 	LDF	@START_POS+X,R0
    // asm 00001826: 	LDF	@START_POS+Y,R1
    // asm 00001827: 	LDF	@START_POS+Z,R2
    // asm 00001828: 	LDP	@_CAMERAPOS+X
    // asm 00001829: 	STF	R0,@_CAMERAPOS+X
    // asm 0000182A: 	STF	R1,@_CAMERAPOS+Y
    // asm 0000182B: 	STF	R2,@_CAMERAPOS+Z
    // asm 0000182C: 	SETDP
    // asm 0000182D: 	LDI	@CHOSEN_RACE,AR0
    // asm 0000182E: 	ADDI	@RACE_STARTING_POINTSI,AR0
    // asm 0000182F: 	LDI	*AR0,R0
    // asm 00001830: 	STI	R0,@STARTSECTION
    // asm 00001831: 	CALL	BGD_INIT
    // asm 00001832: 	CALL	INIT_GAMELEG
    // asm 00001833: 	LDI	MGAME,R0
    // asm 00001834: 	STI	R0,@_MODE
    // asm 00001835: 	CALL	SCREENWIPE_OPEN
    // asm 00001836: 	SLEEP	6
    // asm 00001838: 	LDI	MGAME|MHUD|MINFIN,R0
    // asm 00001839: 	STI	R0,@_MODE
    // asm 0000183A: 	LDI	@CHOSEN_RACE,AR0
    // asm 0000183B: 	ADDI	@BONUS_POSTLAUNCHI,AR0
    // asm 0000183C: 	LDI	*AR0,R0
    // asm 0000183D: 	CALLU	R0
    // asm 0000183E: 	LDI	BUT_VIEW2,R0			;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm 0000183F: 	STI	R0,@BUTTON_STATUS
    // asm 00001840: 	BU	ALL_JOINUP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_NEW_SELECTION", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

#define PLYPOS2YL (-400)
#define PLYPOS2ZL 2200

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR4	PLYRS CAR
 *
 */
static void WATCH_PLYRS_CAR(void) {
    // asm 00001841: 	LDF	@START_RADY,R2
    // asm 00001842: 	LDI	@MATRIXAI,AR2
    // asm 00001843: 	CALL	FIND_YMATRIX
    // asm 00001844: 	LDI	AR2,R2
    // asm 00001845: 	CALL	CLR_VECTORA
    // asm 00001846: 	FLOAT	-PLYPOS2ZL,R0
    // asm 00001847: 	STF	R0,*+AR2(Z)
    // asm 00001848: 	FLOAT	PLYPOS2YL,R0
    // asm 00001849: 	STF	R0,*+AR2(Y)
    // asm 0000184A: 	LDI	@MATRIXAI,R2
    // asm 0000184B: 	LDI	AR2,R3
    // asm 0000184C: 	CALL	MATRIX_MUL
    // asm 0000184D: 	LDF	*+AR4(OPOSX),R0
    // asm 0000184E: 	LDF	*+AR4(OPOSY),R1
    // asm 0000184F: 	LDF	*+AR4(OPOSZ),R2
    // asm 00001850: 	ADDF	*+AR2(X),R0
    // asm 00001851: 	ADDF	*+AR2(Y),R1
    // asm 00001852: 	ADDF	*+AR2(Z),R2
    // asm 00001853: 	LDP	@_CAMERAPOS
    // asm 00001854: 	STF	R0,@_CAMERAPOS+X
    // asm 00001855: 	STF	R1,@_CAMERAPOS+Y
    // asm 00001856: 	STF	R2,@_CAMERAPOS+Z
    // asm 00001857: 	SETDP
    // asm 00001858: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATCH_PLYRS_CAR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void INIT_GAMELEG(void) {
    // asm 00001859: 	CLRI	R0
    // asm 0000185A: 	STI	R0,@SMOKE_COUNT
    // asm 0000185B: 	STI	R0,@DID_TIMED_OUT
    // asm 0000185C: 	STI	R0,@CHECKPOINT_NUM
    // asm 0000185D: 	STI	R0,@REAL_CHECKPOINTS
    // asm 0000185E: 	STI	R0,@MOTION_STOP_HIT
    // asm 0000185F: 	STI	R0,@H2H_FLAGSTATE
    // asm 00001860: 	LDI	2,R0
    // asm 00001861: 	STI	R0,@FRAMRATE
    // asm 00001862: 	LDF	0,R0
    // asm 00001863: 	STF	R0,@ROADKILL_SOUND_TIMER
    // asm 00001864: 	CREATE	RHO_DISPATCHER,SPAWNER_C|TRAFFIC_T
    // asm 00001867: 	CREATE	SIGMA_DISPATCHER,SPAWNER_C|TRAFFIC_T
    // asm 0000186A: 	CREATE	CPOINT_LIGHT,SPAWNER_C|COLORCYC_T
    // asm 0000186D: 	CREATEC	POSITION_FINDER,SPAWNER_C|TRAFFIC_T
    // asm 00001870: 	CREATE	MOVEIN_HUD_EQUIP,UTIL_C
    // asm 00001873: 	LDI	SM_HALT,R0
    // asm 00001874: 	STI	R0,@SUSPEND_MODE
    // asm 00001875: 	LDI	0,R4
    // asm 00001876: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001879: 	LDI	1,R4
    // asm 0000187A: 	CREATE	RACER_DRONE,DRONE_C
    // asm 0000187D: 	LDI	2,R4
    // asm 0000187E: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001881: 	LDI	3,R4
    // asm 00001882: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001885: 	LDI	4,R4
    // asm 00001886: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001889: 	LDI	5,R4
    // asm 0000188A: 	CREATE	RACER_DRONE,DRONE_C
    // asm 0000188D: 	LDI	6,R4
    // asm 0000188E: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001891: 	LDI	7,R4
    // asm 00001892: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001895: 	LDI	9,R4
    // asm 00001896: 	LDI	@DIPRAM,R0
    // asm 00001897: 	TSTB	DIP_COMMP,R0
    // asm 00001898: 	BNZ	BABA66
    // asm 00001899: 	TSTB	CMDP_MASTER,R0
    // asm 0000189A: 	BZ	BABA66
    // asm 0000189B: 	LDI	8,R4
BABA66:
    // asm 0000189C: 	CREATE	RACER_DRONE,DRONE_C
    // asm 0000189F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_GAMELEG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *CHOOSECAR
 *
 */
static const char CCT[] = "CHOOSE CAR";
/*
        ;all Y's were 0
        ;
*/
static tCHOOSE_CAR_ENTRY CCTAB[] = {
    { -1384, -164, -4708, cvettem_ROM, PI, 0x0481 },
    { -448, -200, -4708, hotrodm_ROM, PI, 0x0482 },
    { 464, -177, -4708, misslem_ROM, PI, 0x0483 },
    { 1424, -147, -4708, testorm_ROM, PI, 0x0484 },
};
/* asm: CHOOSENCAR	.bss	CHOOSENCAR,1 */
int CHOOSENCAR;
/* asm: RACE_STARTING_POINTS */
/* asm: 	.word	0 */
/* asm: 	.word	L_LEG2_BEGIN+1 */
/* asm: 	.word	L_LEG3_BEGIN+1 */
/* asm: 	.word	L_LEG4_BEGIN+1 */
/* asm: 	.word	L_LEG5_BEGIN+1 */
/* asm: 	.word	L_LEG6_BEGIN+1 */
/* asm: 	.word	L_LEG7_BEGIN+1 */
/* asm: 	.word	L_LEG8_BEGIN+1 */
/* asm: 	.word	L_LEG9_BEGIN+1 */
/* asm: 	.word	L_LEG10_BEGIN+1 */
/* asm: 	.word	L_LEG11_BEGIN+1 */
/* asm: 	.word	L_LEG12_BEGIN+1 */
/* asm: 	.word	L_LEG13_BEGIN+1 */
/* asm: 	.word	L_LEG14_BEGIN+1 */
int RACE_STARTING_POINTS[] = {
    0,
    L_LEG2_BEGIN + 1,
    L_LEG3_BEGIN + 1,
    L_LEG4_BEGIN + 1,
    L_LEG5_BEGIN + 1,
    L_LEG6_BEGIN + 1,
    L_LEG7_BEGIN + 1,
    L_LEG8_BEGIN + 1,
    L_LEG9_BEGIN + 1,
    L_LEG10_BEGIN + 1,
    L_LEG11_BEGIN + 1,
    L_LEG12_BEGIN + 1,
    L_LEG13_BEGIN + 1,
    L_LEG14_BEGIN + 1,
};

/*
 *
 *CAMERA POSITION IS ASSUMED TO BE SET BY THE TIME
 *THIS ROUTINE IS REACHED.
 *
 *
 */
static void CHOOSECAR(void) {
    // asm 000018A6: 	LDI	@CHOSEN_RACE,AR0
    // asm 000018A7: 	ADDI	@FULLSETUP_TABLEI,AR0
    // asm 000018A8: 	LDI	*AR0,R0
    // asm 000018A9: 	CALLU	R0
    // asm 000018AA: 	LDI	@CHOSEN_RACE,AR0
    // asm 000018AB: 	ADDI	@RACE_STARTING_POINTSI,AR0
    // asm 000018AC: 	LDI	*AR0,R0
    // asm 000018AD: 	STI	R0,@STARTSECTION
    // asm 000018AE: 	CALL	BGD_INIT
    // asm 000018AF: 	LDI	MINTRO|MINFIN|MWATER|MGO,R0
    // asm 000018B0: 	STI	R0,@_MODE
    // asm 000018B1: 	CLRI	R0
    // asm 000018B2: 	STI	R0,@BGNDCOLA
    // asm 000018B3: 	STI	R0,@START_HIT
    // asm 000018B4: 	CALL	TEXT_INIT
    // asm 000018B5: 	SOND1	CHOOSEUCAR
    // asm 000018B7: 	CREATEC	HIDDEN_VEHICLES,UTIL_C|CHOOSECAR_T
    // asm 000018BA: 	LDI	@CCTI,AR2
    // asm 000018BB: 	FLOAT	256,R2
    // asm 000018BC: 	FLOAT	50,R3
    // asm 000018BD: 	LDI	600,RC
    // asm 000018BE: 	CALL	TEXT_ADD
    // asm 000018BF: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000018C2: 	CALL	SET40FONT
JT75:
    // asm 000018C3: LDI	@CAR_CHOICE_GOTTEN,R0
    // asm 000018C4: 	BNZ	KIBO
    // asm 000018C5: 	SLEEP	1
    // asm 000018C7: 	BU	JT75
KIBO:
    // asm 000018C8: 	LDL	crace_PALETTES,AR2
    // asm 000018C9: 	CALL	dealloc_section
XOFFSETI:
    // asm 000018CA: .word	XOFFSET
    // asm 000018CA: 	LDI	@CHOSEN_VEHICLE,AR0
    // asm 000018CB: 	ADDI	@XOFFSETI,AR0
    // asm 000018CC: 	LDF	*AR0,R6
    // asm 000018CD: 	FLOAT	576,R1
    // asm 000018CE: 	SUBRF	R1,R6
    // 	;if a slave then ALWAYS appear on right side
    // 	;
    // 	;
    // asm 000018CF: 	LDI	@DIPRAM,R0
    // asm 000018D0: 	TSTB	DIP_COMMP,R0
    // asm 000018D1: 	BNZ	BABA
    // asm 000018D2: 	TSTB	CMDP_MASTER,R0
    // asm 000018D3: 	BZ	BABA
    // asm 000018D4: 	FLOAT	LANESIZE,R1
    // asm 000018D5: 	ADDF	R1,R6
BABA:
    // asm 000018D6: 	LDF	@START_RADY,R2
    // asm 000018D7: 	LDI	@MATRIXAI,AR2
    // asm 000018D8: 	CALL	HPFIND_YMATRIX
    // asm 000018D9: 	LDI	AR2,R2
    // asm 000018DA: 	CALL	CLR_VECTORA
    // asm 000018DB: 	STF	R6,*+AR2(X)
    // asm 000018DC: 	LDI	AR2,R3
    // asm 000018DD: 	CALL	MATRIX_MUL
    // asm 000018DE: 	LDF	*+AR2(X),R6
    // asm 000018DF: 	LDF	*+AR2(Z),R7
    // asm 000018E0: 	LDI	@SINGLE_SECTION_TEMPPTR,R0
NLD:
    // asm 000018E1: LDI	R0,AR0
    // asm 000018E2: 	LDF	*+AR0(OPOSX),R0
    // asm 000018E3: 	ADDF	R6,R0
    // asm 000018E4: 	STF	R0,*+AR0(OPOSX)
    // asm 000018E5: 	LDF	*+AR0(OPOSZ),R0
    // asm 000018E6: 	ADDF	R7,R0
    // asm 000018E7: 	STF	R0,*+AR0(OPOSZ)
    // asm 000018E8: 	LDI	*+AR0(OLINK2),R0
    // asm 000018E9: 	BNZ	NLD
    // asm 000018EA: 	LDI	@CAMERAPOSI,AR2
    // asm 000018EB: 	LDF	*+AR2(X),R0
    // asm 000018EC: 	ADDF	R6,R0
    // asm 000018ED: 	STF	R0,*+AR2(X)
    // asm 000018EE: 	LDF	*+AR2(Z),R0
    // asm 000018EF: 	ADDF	R7,R0
    // asm 000018F0: 	STF	R0,*+AR2(Z)
    // asm 000018F1: 	LDL	CAR_ARRAY,AR3
    // asm 000018F2: 	LDI	3,AR5
LNNN:
    // asm 000018F3: LDI	*AR3++,AR0
    // asm 000018F4: 	LDF	*+AR0(OPOSX),R0
    // asm 000018F5: 	ADDF	R6,R0
    // asm 000018F6: 	STF	R0,*+AR0(OPOSX)
    // asm 000018F7: 	LDF	*+AR0(OPOSZ),R0
    // asm 000018F8: 	ADDF	R7,R0
    // asm 000018F9: 	STF	R0,*+AR0(OPOSZ)
    // asm 000018FA: 	DBU	AR5,LNNN
    // asm 000018FB: 	LDI	@_MODE,R0
    // asm 000018FC: 	ANDN	MGO,R0
    // asm 000018FD: 	STI	R0,@_MODE
    // asm 000018FE: 	LDI	5,R0		;WAVEFLAG routine properly sets this
    // asm 000018FF: 	STI	R0,@_countdown	;we just need to set so PLYR doesnt puke
    // asm 00001900: 	LDI	100h,R0		;FULL START INDEX
    // asm 00001901: 	STI	R0,@RACER_GRID_START
    // asm 00001902: 	CALL	TEXT_INIT			;ELIMINATE THE 'CHOOSE CAR' TEXT
    // asm 00001903: 	LDI	@CHOSEN_VEHICLE,R0
    // asm 00001904: 	LDI	@IS_HIDDEN,R1
    // asm 00001905: 	CMPI	-1,R1
    // asm 00001906: 	BEQ	NOHIDE
    // asm 00001907: 	PUSH	R0
    // asm 00001908: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 00001909: 	CMPI	5,AR2
    // asm 0000190A: 	LDIEQ	1,AR2
    // asm 0000190B: 	CMPI	6,AR2
    // asm 0000190C: 	LDIEQ	2,AR2
    // asm 0000190D: 	CMPI	7,AR2
    // asm 0000190E: 	LDIEQ	3,AR2
    // asm 0000190F: 	ADDI	AUD_CAR_SELECTION+4,AR2
    // asm 00001910: 	CALL	AUDIT_INC
    // asm 00001911: 	POP	R0
    // asm 00001912: 	ADDI	4,R0
    // asm 00001913: 	BU	TMP886
NOHIDE:
    // asm 00001914: 	PUSH	R0
    // asm 00001915: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 00001916: 	ADDI	AUD_CAR_SELECTION,AR2
    // asm 00001917: 	CALL	AUDIT_INC
    // asm 00001918: 	POP	R0
TMP886:
    // asm 00001919: STI	R0,@CHOOSENCAR
    // asm 0000191A: 	LDI	UTIL_C|CHOOSECAR_T,R0
    // asm 0000191B: 	LDI	-1,R1
    // asm 0000191C: 	CALL	PRC_KILLALL
    // asm 0000191D: 	LDI	@DIPRAM,R0
    // asm 0000191E: 	TSTB	DIP_COMMP,R0
    // asm 0000191F: 	BNZ	JAJAKKA
    // asm 00001920: 	JSRP	WAIT_FOR_CHALLENGER
JAJAKKA:
    // asm 00001926: 	CALL	INIT_GAMELEG
    // asm 00001927: 	CREATE	DROPTHEWHEEL,UTIL_C|CHOOSECAR_T
    // asm 0000192A: 	CREATE	DROPTHECYCLE,UTIL_C|CHOOSECAR_T
    // asm 0000192D: 	CREATE	DROPTHETURN,UTIL_C|CHOOSECAR_T
    // asm 00001930: 	CREATE	RAISE_DOOR,UTIL_C|CHOOSECAR_T	;in 50
    // 		;(BECOMES PLYR PROC)
    // asm 00001933: 	CREATE	ZOOMTOCAR,UTIL_C|CHOOSECAR_T	;in 25 (TRANS.ASM)
    // asm 00001936: 	SLEEP	70
    // asm 00001938: 	SLEEP	1
    // asm 0000193A: 	LDI	UTIL_C|CHOOSECAR_T,R0
    // asm 0000193B: 	LDI	-1,R1
    // asm 0000193C: 	CALL	PRC_KILLALL
    // asm 0000193D: 	CALL	CLEANUP_DIMCAR_PALS
    // asm 0000193E: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOOSECAR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CAR_CHOICE_GOTTEN	.bss	CAR_CHOICE_GOTTEN,1 */
int CAR_CHOICE_GOTTEN;

void THE_CAR_CHOICE_PROC(void) {
    // ;	SLEEP	15
    // asm 00001942: 	CLRI	R0
    // asm 00001943: 	STI	R0,@CAR_CHOICE_GOTTEN
    // asm 00001944: 	STI	R0,@START_HIT
    // *
    // *CHOOSE CAR LOOP
    // *
    // asm 00001945: 	LDI	12,R0
    // asm 00001946: 	STI	R0,@_countdown
    // asm 00001947: 	CALL	INIT_PEDALCHK
CCLP:
    // asm 00001948: 	LDI	@START_HIT,R0
    // asm 00001949: 	BNZ	CCLPX
    // asm 0000194A: 	CALL	GETCHOICE
    // asm 0000194B: 	CALL	TRANSCHOICE
    // asm 0000194C: 	CALL	DIAL_ROUT
    // asm 0000194D: 	CALL	SHOW_CAR_STATISTICS
    // 	;-------time remaining
    // 	;
    // asm 0000194E: 	LDL	time,AR2
    // asm 0000194F: 	LDI	242,R2			;R2	POS X
    // asm 00001950: 	LDI	337,R3			;R3	POS Y
    // asm 00001951: 	LDI	TM|ZS,R4
    // asm 00001952: 	CALL	BLTMOD2D
    // asm 00001953: 	CALL	PEDALCHK
    // asm 00001954: 	BC	CCLPX
    // asm 00001955: 	CALL	INTROTIMER
    // asm 00001956: 	SLEEP	1
    // asm 00001958: 	LDI	@_countdown,R0
    // asm 00001959: 	BGT	CCLP
CCLPX:
    // *
    // *END CHOOSE CAR LOOP
    // *
    // asm 0000195A: 	LDI	1,R0
    // asm 0000195B: 	STI	R0,@CAR_CHOICE_GOTTEN
    // asm 0000195C: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THE_CAR_CHOICE_PROC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *CAR IS CHOOSEN, THEN RAISE THE GARAGE DOOR
 *
 *
 *
 */
static void RAISE_DOOR(void) {
    // asm 0000195D: 	SOND1	GOPEN
    // asm 0000195F: 	LDI	49,AR5
    // asm 00001960: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 00001961: 	ADDI	501h,AR2
    // asm 00001962: 	CALL	OBJ_FIND_FIRST
    // asm 00001963: 	LDI	AR0,AR4
    // asm 00001964: RDLP
    // asm 00001964: 	LDF	*+AR4(OPOSY),R0
    // asm 00001965: 	SUBF	14,R0
    // asm 00001966: 	STF	R0,*+AR4(OPOSY)
    // asm 00001967: 	SLEEP	1
    // asm 00001969: 	DBU	AR5,RDLP
    // asm 0000196A: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAISE_DOOR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *THIS PROCESS:
 *	1)	MOVES THE CAMERA TO THE VEHICLE
 *	2)	LOWERS VEHICLE TO GROUND
 *	3)	TURNS VEHICLE TO FACING OUT
 *	4)	DRIVES VEHICLE FORWARD (WHILE TRACKING
 *		WITH CAMERA)
 *	5)	INITIALIZES AS PLAYERS CAR
 *	6)	TRANSFERS CONTROL TO THE PLYRS
 *		MAIN ROUTINE
 *
 *
 *
 */
void ZOOMTOCAR(void) {
    // asm 0000196B: 	CLRI	R0
    // asm 0000196C: 	STI	R0,@START_NOW_P
    // asm 0000196D: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 0000196E: 	ADDI	481h,AR2
    // asm 0000196F: 	CALL	OBJ_FIND_FIRST
    // asm 00001970: 	LDI	AR0,AR4
    // asm 00001971: 	LDF	@START_RADY,R2
    // asm 00001972: 	LDI	@MATRIXAI,AR2
    // asm 00001973: 	CALL	FIND_YMATRIX
    // asm 00001974: 	LDI	25,AR5
    // asm 00001975: ZTCLP
    // 	;NOW MOVE TO IN FRONT OF OBJECT
    // asm 00001975: 	LDF	@START_RADY,R2
    // asm 00001976: 	LDI	@MATRIXAI,AR2
    // asm 00001977: 	CALL	FIND_YMATRIX
    // asm 00001978: 	LDI	AR2,R2
    // asm 00001979: 	CALL	CLR_VECTORA
    // asm 0000197A: 	FLOAT	-PLYPOS2ZL,R0
    // asm 0000197B: 	STF	R0,*+AR2(Z)
    // asm 0000197C: 	FLOAT	PLYPOS2YL,R0
    // asm 0000197D: 	STF	R0,*+AR2(Y)
    // asm 0000197E: 	LDI	@MATRIXAI,R2
    // asm 0000197F: 	LDI	AR2,R3
    // asm 00001980: 	CALL	MATRIX_MUL
    // asm 00001981: 	LDF	*+AR4(OPOSX),R0
    // asm 00001982: 	LDF	*+AR4(OPOSY),R1
    // asm 00001983: 	LDF	*+AR4(OPOSZ),R2
    // asm 00001984: 	ADDF	*+AR2(X),R0
    // asm 00001985: 	ADDF	*+AR2(Y),R1
    // asm 00001986: 	ADDF	*+AR2(Z),R2
    // asm 00001987: 	LDP	@_CAMERAPOS
    // asm 00001988: 	SUBF	@_CAMERAPOS+X,R0
    // asm 00001989: 	SUBF	@_CAMERAPOS+Y,R1
    // asm 0000198A: 	SUBF	@_CAMERAPOS+Z,R2
    // asm 0000198B: 	MPYF	0.2,R0
    // asm 0000198C: 	MPYF	0.2,R1
    // asm 0000198D: 	MPYF	0.2,R2
    // asm 0000198E: 	ADDF	@_CAMERAPOS+X,R0
    // asm 0000198F: 	ADDF	@_CAMERAPOS+Y,R1
    // asm 00001990: 	ADDF	@_CAMERAPOS+Z,R2
    // asm 00001991: 	STF	R0,@_CAMERAPOS+X
    // asm 00001992: 	STF	R1,@_CAMERAPOS+Y
    // asm 00001993: 	STF	R2,@_CAMERAPOS+Z
    // asm 00001994: 	SETDP
    // 	;TURN CAR TO FACE AWAY (SMOOTH)
    // asm 00001995: 	LDF	@START_RADY,R2
    // asm 00001996: 	CALL	NORMITS
    // asm 00001997: 	LDF	R2,R0
    // asm 00001998: 	LDF	*+AR4(ORADY),R2
    // asm 00001999: 	CALL	NORMITS
    // asm 0000199A: 	STF	R2,*+AR4(ORADY)
    // asm 0000199B: 	CALL	GETTHETADIFF
    // asm 0000199C: 	ABSF	R0,R1
    // asm 0000199D: 	CMPF	0.04,R1
    // asm 0000199E: 	BLT	DOALL1
    // asm 0000199F: 	MPYF	0.10,R0
    // asm 000019A0: 	ADDF	R0,R2
    // asm 000019A1: 	BU	IBO45A
DOALL1:
    // asm 000019A2: ADDF	R0,R2
IBO45A:
    // asm 000019A3: STF	R2,*+AR4(ORADY)
    // asm 000019A4: 	LDI	AR4,AR2
    // asm 000019A5: 	ADDI	OMATRIX,AR2
    // asm 000019A6: 	CALL	FIND_YMATRIX
    // 	;LOWER CAR TO GROUND
    // asm 000019A7: 	LDF	*+AR4(OPOSY),R0
    // asm 000019A8: 	LDF	*+AR4(OUSR1),R1
    // asm 000019A9: 	SUBF	R0,R1,R0
    // asm 000019AA: 	MPYF	0.15,R0
    // asm 000019AB: 	LDF	R0,R1
    // asm 000019AC: 	ADDF	*+AR4(OPOSY),R0
    // asm 000019AD: 	STF	R0,*+AR4(OPOSY)
    // asm 000019AE: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 000019AF: 	ADDI	401h,AR2
    // asm 000019B0: 	CALL	OBJ_FIND_FIRST
    // asm 000019B1: 	LDF	*+AR0(OPOSY),R0
    // asm 000019B2: 	ADDF	R1,R0
    // asm 000019B3: 	STF	R0,*+AR0(OPOSY)
    // asm 000019B4: 	SLEEP	1
    // asm 000019B6: 	DBU	AR5,ZTCLP
    // 	;NOW SET ALL FACING PROPERLY
    // 	;
    // asm 000019B7: 	PUSH	AR4
    // asm 000019B8: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 000019B9: 	ADDI	481h,AR2
    // asm 000019BA: 	CALL	OBJ_FIND_FIRST
    // asm 000019BB: 	LDI	AR0,AR4
    // asm 000019BC: 	POP	AR4
    // asm 000019BD: 	LDI	PLYR_C|PLYR1_T,R0
    // asm 000019BE: 	STI	R0,*+AR7(PID)
    // *
    // *
    // *
    // *
    // 	;NOW MOVE TO IN FRONT OF OBJECT
    // asm 000019BF: 	LDI	@CAMERAPOSI,AR6
    // asm 000019C0: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 000019C1: 	CMPI	4,AR2
    // asm 000019C2: 	BLT	NDO
    // asm 000019C3: 	SUBI	4,AR2
NDO:
    // asm 000019C4: ADDI	401h,AR2
    // asm 000019C5: 	CALL	OBJ_FIND_FIRST
    // asm 000019C6: 	LDI	AR0,AR2
    // asm 000019C7: 	CALL	OBJ_DELETE
    // 	;TURN CAR TO FACE AWAY
    // asm 000019C8: 	LDF	*+AR4(ORADY),R2
    // asm 000019C9: 	LDF	@START_RADY,R0
    // asm 000019CA: 	CALL	GETTHETADIFF
    // asm 000019CB: 	ADDF	R0,R2
    // asm 000019CC: 	STF	R2,*+AR4(ORADY)
    // asm 000019CD: 	LDI	AR4,AR2
    // asm 000019CE: 	ADDI	OMATRIX,AR2
    // asm 000019CF: 	CALL	FIND_YMATRIX
    // 	;LOWER CAR TO GROUND
    // asm 000019D0: 	LDF	*+AR4(OPOSY),R0
    // asm 000019D1: 	LDF	*+AR4(OUSR1),R1
    // asm 000019D2: 	SUBF	R0,R1,R0
    // asm 000019D3: 	ADDF	*+AR4(OPOSY),R0
    // asm 000019D4: 	STF	R0,*+AR4(OPOSY)
    // asm 000019D5: 	SONDFX	ENGINESTART
    // *
    // *
    // *
    // asm 000019D7: 	LDI	24,AR5
    // asm 000019D8: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 000019D9: 	CMPI	4,AR2
    // asm 000019DA: 	BLT	NDO2
    // asm 000019DB: 	SUBI	4,AR2
NDO2:
    // asm 000019DC: 	ADDI	481h,AR2
    // asm 000019DD: 	CALL	OBJ_FIND_FIRST
    // asm 000019DE: 	LDI	AR0,AR4
    // asm 000019DF: 	LDI	AR4,AR2
    // asm 000019E0: 	CALL	OBJ_PULL
    // asm 000019E1: 	CALL	PLYR_CAR_INIT
    // asm 000019E2: 	LDF	1,R6
    // asm 000019E3: 	PUSH	AR5
    // asm 000019E4: 	PUSHFL	R6
    // asm 000019E6: 	LDI	*+AR4(OCARBLK),AR5
    // asm 000019E7: 	STF	R6,*+AR5(CARSPEED)
    // asm 000019E8: 	LDF	@START_RADY,R6
    // asm 000019E9: 	STF	R6,*+AR5(CARYROT)
    // asm 000019EA: 	STF	R6,*+AR5(CARVROT)
    // asm 000019EB: 	STF	R6,*+AR5(CARDROT)
    // asm 000019EC: 	CLRF	R2
    // asm 000019ED: 	CALL	DRONEGO
    // asm 000019EE: 	POPFL	R6
    // asm 000019F0: 	POP	AR5
    // asm 000019F1: 	CALL	WATCH_PLYRS_CAR
    // asm 000019F2: 	SONDFX	STARTLINEREVS2
    // *
    // *NOW WE MOVE FORWARD AND INTO THE POSITION
    // *OF INSIDE LANE OF ROAD PIECE (OUSR1==0102h)
    // *
    // asm 000019F4: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm 000019F5: 	LDI	*+AR0(OUSR1),R1
    // asm 000019F6: 	ANDN	0FFh,R1
    // asm 000019F7: 	LDI	*+AR0(OLINK4),AR0
    // asm 000019F8: 	LDI	AR0,AR5
    // 	;MOVE FORWARD LOOP
    // 	;
MOVELOOP:
    // asm 000019F9: 	LDF	*+AR5(OPOSX),R0
    // asm 000019FA: 	SUBF	*+AR4(OPOSX),R0
    // asm 000019FB: 	MPYF	R0,R0
    // asm 000019FC: 	LDF	*+AR5(OPOSZ),R1
    // asm 000019FD: 	SUBF	*+AR4(OPOSZ),R1
    // asm 000019FE: 	MPYF	R1,R1
    // asm 000019FF: 	ADDF	R1,R0
    // asm 00001A00: 	LDF	R0,R4
    // asm 00001A01: 	FLOAT	700,R1
    // 	;if a slave then ALWAYS appear on right side
    // 	;
    // asm 00001A02: 	LDI	@DIPRAM,R3
    // asm 00001A03: 	TSTB	DIP_COMMP,R3
    // asm 00001A04: 	BNZ	BABADUY
    // asm 00001A05: 	TSTB	CMDP_MASTER,R3
    // asm 00001A06: 	BZ	BABADUY
    // asm 00001A07: 	FLOAT	LANESIZE,R3
    // asm 00001A08: 	ADDF	R3,R1
BABADUY:
    // asm 00001A09: 	MPYF	R1,R1
    // asm 00001A0A: 	CMPF	R1,R0
    // asm 00001A0B: 	BLE	IBODONE
    // asm 00001A0C: 	PUSH	AR5
    // asm 00001A0D: 	LDI	*+AR4(OCARBLK),AR5
    // asm 00001A0E: 	LDF	0.5,R0
    // asm 00001A0F: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm 00001A10: 	CLRF	R2
    // asm 00001A11: 	CALL	DRONEGO
    // asm 00001A12: 	POP	AR5
    // asm 00001A13: 	CALL	WATCH_PLYRS_CAR
    // asm 00001A14: 	SLEEP	1
    // asm 00001A16: 	BU	MOVELOOP
IBODONE:
    // asm 00001A17: 	LDI	@DIPRAM,R3
    // asm 00001A18: 	TSTB	DIP_COMMP,R3
    // asm 00001A19: 	BNZ	BABADUY4
    // asm 00001A1A: 	LDI	AR5,AR2
    // asm 00001A1B: 	TSTB	CMDP_MASTER,R3
    // asm 00001A1C: 	LDIZ	572,AR3
    // asm 00001A1D: 	LDINZ	1724,AR3
    // asm 00001A1E: 	FLOAT	AR3,R0
    // asm 00001A1F: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm 00001A20: 	LDI	0,AR3
    // asm 00001A21: 	CALL	SPOS_INIT		;INIT STARTING POSITION
BABADUY4:
    // asm 00001A22: 	LDI	1,R0
    // asm 00001A23: 	STI	R0,@START_NOW_P
    // asm 00001A24: 	LDI	MGAME|MINFIN|MWATER,R0	;NOT MGO NOT MHUD
    // asm 00001A25: 	STI	R0,@_MODE
    // asm 00001A26: 	BU	PLYR_INTRO_ENTER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOMTOCAR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CAR_ARRAY	.bss	CAR_ARRAY,4 */
int CAR_ARRAY[4];

void GETTHECARS(void) {
    // asm 00001A27: 	LDF	@START_RADY,R2
    // asm 00001A28: 	LDI	@MATRIXAI,AR2
    // asm 00001A29: 	CALL	FIND_YMATRIX
    // asm 00001A2A: 	LDL	CAR_ARRAY,AR3
    // asm 00001A2B: 	LDI	@CCTABI,AR4
    // asm 00001A2C: 	CLRI	R4
LISTLP:
    // asm 00001A2D: CALL	OBJ_GET
    // asm 00001A2E: 	LDI	@VECTORAI,AR2
    // asm 00001A2F: 	FLOAT	*AR4++,R1		;GET X POSITION
    // asm 00001A30: 	STF	R1,*+AR2(X)
    // asm 00001A31: 	FLOAT	*AR4++,R1		;GET Y POSITION
    // asm 00001A32: 	STF	R1,*+AR2(Y)
    // asm 00001A33: 	FLOAT	*AR4++,R1		;GET Z POSITION
    // asm 00001A34: 	STF	R1,*+AR2(Z)
    // asm 00001A35: 	LDI	@MATRIXAI,R2
    // asm 00001A36: 	LDI	AR2,R3
    // asm 00001A37: 	CALL	MATRIX_MUL
    // asm 00001A38: 	LDF	*+AR2(X),R0
    // asm 00001A39: 	ADDF	@START_POS+X,R0
    // asm 00001A3A: 	STF	R0,*+AR0(OPOSX)
    // asm 00001A3B: 	LDF	*+AR2(Y),R0
    // asm 00001A3C: 	ADDF	@START_POS+Y,R0
    // asm 00001A3D: 	STF	R0,*+AR0(OPOSY)
    // asm 00001A3E: 	LDF	*+AR2(Z),R0
    // asm 00001A3F: 	ADDF	@START_POS+Z,R0
    // asm 00001A40: 	STF	R0,*+AR0(OPOSZ)
    // asm 00001A41: 	LDI	*AR4++,R0
    // asm 00001A42: 	STI	R0,*+AR0(OROMDATA)
    // asm 00001A43: 	STI	AR0,*AR3++
    // asm 00001A44: 	LDI	AR0,AR2
    // asm 00001A45: 	CALL	OBJ_INSERT
    // asm 00001A46: 	LDI	AR7,AR1
    // asm 00001A47: 	ADDI	PDATA,AR1
    // asm 00001A48: 	ADDI	R4,AR1
    // asm 00001A49: 	STI	AR0,*AR1
    // asm 00001A4A: 	INC	R4
    // asm 00001A4B: 	LDF	*AR4++,R2
    // asm 00001A4C: 	ADDF	@START_RADY,R2
    // asm 00001A4D: 	LDI	AR0,AR2
    // asm 00001A4E: 	ADDI	OMATRIX,AR2
    // asm 00001A4F: 	STF	R2,*+AR0(ORADY)
    // asm 00001A50: 	CALL	FIND_YMATRIX
    // asm 00001A51: 	LDI	*AR4++,R0
    // asm 00001A52: 	STI	R0,*+AR0(OID)
    // asm 00001A53: 	LDI	*AR4,R0
    // asm 00001A54: 	CMPI	-1,R0
    // asm 00001A55: 	BNE	LISTLP
    // asm 00001A56: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTHECARS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SHOW CAR STATISTICS
 *
 *USES
 *	AR4,AR5,AR6,R4,R5
 *
 */
/* asm: SCS_TAB		.float	10,70,170,230 */
/* asm: 	 */
static float SCS_TAB[] = {
    10.0f,
    70.0f,
    170.0f,
    230.0f,
};

void SHOW_CAR_STATISTICS(void) {
    // asm 00001A5C: 	LDI	@DCALL,R0
    // asm 00001A5D: 	RETSZ
    // asm 00001A5E: 	PUSH	AR3
    // asm 00001A5F: 	PUSH	AR4
    // asm 00001A60: 	PUSH	AR5
    // asm 00001A61: 	PUSH	AR6
    // asm 00001A62: 	PUSH	AR7
    // asm 00001A63: 	LDI	@CHOSEN_VEHICLE,AR7
    // asm 00001A64: 	ADDI	@SCS_TABI,AR7
    // asm 00001A65: 	LDL	TITLES,AR5
    // asm 00001A66: 	LDI	5-1,AR3
GBERLP:
    // asm 00001A67: LDI	*AR5++,AR2
    // asm 00001A68: 	LDF	*AR7,R2
    // asm 00001A69: 	FLOAT	180,R3
    // asm 00001A6A: 	FLOAT	AR3,R0
    // asm 00001A6B: 	MPYF	10,R0
    // asm 00001A6C: 	SUBF	R0,R3
    // asm 00001A6D: 	LDI	1,RC
    // asm 00001A6E: 	CALL	TEXT_ADDDS
    // asm 00001A6F: 	CALL	SETFIXEDFONTDS
    // asm 00001A70: 	DBU	AR3,GBERLP
    // asm 00001A71: 	LDI	@CHOSEN_VEHICLE,AR5
    // asm 00001A72: 	ADDI	@TEXTTABSI,AR5
    // asm 00001A73: 	LDI	*AR5,AR5
    // asm 00001A74: 	LDI	5,AR3
    // asm 00001A75: GNNERLP
    // asm 00001A75: 	LDI	*AR5++,AR2
    // asm 00001A76: 	LDF	*AR7,R2
    // asm 00001A77: 	ADDF	100,R2
    // asm 00001A78: 	CMPI	6,AR3
    // asm 00001A79: 	LDFEQ	*AR7,R2
    // asm 00001A7A: 	FLOAT	180,R3
    // asm 00001A7B: 	FLOAT	AR3,R0
    // asm 00001A7C: 	MPYF	10,R0
    // asm 00001A7D: 	SUBF	R0,R3
    // asm 00001A7E: 	LDI	1,RC
    // asm 00001A7F: 	CALL	TEXT_ADDDS
    // asm 00001A80: 	CALL	SETFIXEDFONTDS
    // asm 00001A81: 	DBU	AR3,GNNERLP
    // asm 00001A82: 	POP	AR7
    // asm 00001A83: 	POP	AR6
    // asm 00001A84: 	POP	AR5
    // asm 00001A85: 	POP	AR4
    // asm 00001A86: 	POP	AR3
    // asm 00001A87: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_CAR_STATISTICS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: CAR1PAL	.bss	CAR1PAL,129 */
tPAL CAR1PAL;
/* asm: CAR2PAL	.bss	CAR2PAL,129 */
tPAL CAR2PAL;
/* asm: CAR3PAL	.bss	CAR3PAL,129 */
tPAL CAR3PAL;
/* asm: CAR4PAL	.bss	CAR4PAL,129 */
tPAL CAR4PAL;
/* asm: CARPAL_TABLE	.word	CAR1PAL,CAR2PAL,CAR3PAL,CAR4PAL */
static tPAL* CARPAL_TABLE[] = {
    &CAR1PAL,
    &CAR2PAL,
    &CAR3PAL,
    &CAR4PAL,
};
/* asm: CARSRCPAL_TAB	.word	cvette_p,hotrod_p,missle_p,testor_p */
static int CARSRCPAL_TAB[] = {
    cvette_p,
    hotrod_p,
    missle_p,
    testor_p,
};

/*
 *----------------------------------------------------------------------------
 *
 *(AND OVERHEAD LIGHT CLEANUP)
 *
 *
 */
static void CLEANUP_DIMCAR_PALS(void) {
    // asm 00001A8A: 	LDL	flour_lghtof,AR2
    // asm 00001A8B: 	CALL	PAL_FIND_RAW
    // asm 00001A8C: 	LDI	R0,AR2
    // asm 00001A8D: 	CALL	PAL_DELETE_RAW
    // asm 00001A8E: 	LDL	CAR1PAL,AR2
    // asm 00001A8F: 	CALL	PAL_FIND_RAW
    // asm 00001A90: 	LDI	R0,AR2
    // asm 00001A91: 	CALL	PAL_DELETE_RAW
    // asm 00001A92: 	LDL	CAR2PAL,AR2
    // asm 00001A93: 	CALL	PAL_FIND_RAW
    // asm 00001A94: 	LDI	R0,AR2
    // asm 00001A95: 	CALL	PAL_DELETE_RAW
    // asm 00001A96: 	LDL	CAR3PAL,AR2
    // asm 00001A97: 	CALL	PAL_FIND_RAW
    // asm 00001A98: 	LDI	R0,AR2
    // asm 00001A99: 	CALL	PAL_DELETE_RAW
    // asm 00001A9A: 	LDL	CAR4PAL,AR2
    // asm 00001A9B: 	CALL	PAL_FIND_RAW
    // asm 00001A9C: 	LDI	R0,AR2
    // asm 00001A9D: 	CALL	PAL_DELETE_RAW
    // asm 00001A9E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEANUP_DIMCAR_PALS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*GENERAL PURPOSE CHOOSE CAR LOOP ROUTINE
*
*
        ;THE CAR
*/
#define RNDR_C1_DYH (PDATA + 1) // desired Y height
#define RNDR_C1_SYH (PDATA + 2) // starting y height
#define RNDR_C2_DYH (PDATA + 3)
#define RNDR_C2_SYH (PDATA + 4)
#define RNDR_C3_DYH (PDATA + 5)
#define RNDR_C3_SYH (PDATA + 6)
#define RNDR_C4_DYH (PDATA + 7)
#define RNDR_C4_SYH (PDATA + 8)
// 	;THE LIFT
#define RNDR_L1_DYH (PDATA + 9)  // desired Y height
#define RNDR_L1_SYH (PDATA + 10) // starting y height
#define RNDR_L2_DYH (PDATA + 11)
#define RNDR_L2_SYH (PDATA + 12)
#define RNDR_L3_DYH (PDATA + 13)
#define RNDR_L3_SYH (PDATA + 14)
#define RNDR_L4_DYH (PDATA + 15)
#define RNDR_L4_SYH (PDATA + 16)
#define CDYH 0
#define CSYH 1
#define C_SIZE 2
#define C_STRT (PDATA + 1)
#define C_OFF2LIFT 8

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *
 *PARAMETERS
 *	AR2	RAW PALETTE	CAR1PAL
 *	AR3	ID OF VEHICLE
 *	IR0	RNDR_C1_DYH
 *	IR1	INDEX OF VEHCILE
 *
 */
static void GETTHECAR(void) {
    // asm 00001A9F: 	LDI	1,R0
    // asm 00001AA0: 	STI	R0,*AR2
    // asm 00001AA1: 	CALL	PAL_ALLOC_RAW
    // asm 00001AA2: 	LDI	R0,R6
    // asm 00001AA3: 	LDI	AR3,AR2
    // asm 00001AA4: 	CALL	OBJ_FIND_FIRST
    // asm 00001AA5: 	BNC	$
    // asm 00001AA6: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001AA7: 	OR	O_1PAL,R0
    // asm 00001AA8: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001AA9: 	STI	R6,*+AR0(OPAL)
    // asm 00001AAA: 	LDF	*+AR0(OPOSY),R0
    // asm 00001AAB: 	STF	R0,*+AR7(IR0)
    // asm 00001AAC: 	INC	IR0
    // asm 00001AAD: 	STF	R0,*+AR7(IR0)
    // asm 00001AAE: 	STF	R0,*+AR0(OUSR1)
    // asm 00001AAF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTHECAR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 */
void ROUNDER(void) {
    // asm 00001AB0: 	LDL	CAR1PAL,AR2
    // asm 00001AB1: 	LDI	481h,AR3
    // asm 00001AB2: 	LDI	RNDR_C1_DYH,IR0
    // asm 00001AB3: 	CALL	GETTHECAR
    // asm 00001AB4: 	LDL	CAR2PAL,AR2
    // asm 00001AB5: 	LDI	482h,AR3
    // asm 00001AB6: 	LDI	RNDR_C2_DYH,IR0
    // asm 00001AB7: 	CALL	GETTHECAR
    // asm 00001AB8: 	LDL	CAR3PAL,AR2
    // asm 00001AB9: 	LDI	483h,AR3
    // asm 00001ABA: 	LDI	RNDR_C3_DYH,IR0
    // asm 00001ABB: 	CALL	GETTHECAR
    // asm 00001ABC: 	LDL	CAR4PAL,AR2
    // asm 00001ABD: 	LDI	484h,AR3
    // asm 00001ABE: 	LDI	RNDR_C4_DYH,IR0
    // asm 00001ABF: 	CALL	GETTHECAR
    // asm 00001AC0: 	LDI	401h,AR2
    // asm 00001AC1: 	CALL	OBJ_FIND_FIRST
    // asm 00001AC2: 	LDF	*+AR0(OPOSY),R0
    // asm 00001AC3: 	STF	R0,*+AR7(RNDR_L1_DYH)
    // asm 00001AC4: 	STF	R0,*+AR7(RNDR_L1_SYH)
    // asm 00001AC5: 	LDI	402h,AR2
    // asm 00001AC6: 	CALL	OBJ_FIND_FIRST
    // asm 00001AC7: 	LDF	*+AR0(OPOSY),R0
    // asm 00001AC8: 	STF	R0,*+AR7(RNDR_L2_DYH)
    // asm 00001AC9: 	STF	R0,*+AR7(RNDR_L2_SYH)
    // asm 00001ACA: 	LDI	403h,AR2
    // asm 00001ACB: 	CALL	OBJ_FIND_FIRST
    // asm 00001ACC: 	LDF	*+AR0(OPOSY),R0
    // asm 00001ACD: 	STF	R0,*+AR7(RNDR_L3_DYH)
    // asm 00001ACE: 	STF	R0,*+AR7(RNDR_L3_SYH)
    // asm 00001ACF: 	LDI	404h,AR2
    // asm 00001AD0: 	CALL	OBJ_FIND_FIRST
    // asm 00001AD1: 	LDF	*+AR0(OPOSY),R0
    // asm 00001AD2: 	STF	R0,*+AR7(RNDR_L4_DYH)
    // asm 00001AD3: 	STF	R0,*+AR7(RNDR_L4_SYH)
    // asm 00001AD4: 	LDI	0,AR0
    // asm 00001AD5: 	LDF	0.5,R0
    // asm 00001AD6: 	CALL	CAR_DIMMER
    // asm 00001AD7: 	LDI	1,AR0
    // asm 00001AD8: 	LDF	0.5,R0
    // asm 00001AD9: 	CALL	CAR_DIMMER
    // asm 00001ADA: 	LDI	2,AR0
    // asm 00001ADB: 	LDF	0.5,R0
    // asm 00001ADC: 	CALL	CAR_DIMMER
    // asm 00001ADD: 	LDI	3,AR0
    // asm 00001ADE: 	LDF	0.5,R0
    // asm 00001ADF: 	CALL	CAR_DIMMER
    // asm 00001AE0: 	CALL	LIGHT_INIT
    // asm 00001AE1: 	LDI	@CHOSEN_VEHICLE,R0
    // asm 00001AE2: 	STI	R0,@SPINCURR
    // asm 00001AE3: 	CLRI	R0
    // asm 00001AE4: 	STI	R0,*+AR7(PDATA)
ROUNDERLP:
    // asm 00001AE5: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 00001AE6: 	LDI	*+AR7(PDATA),R0
    // asm 00001AE7: 	CMPI	R0,AR2
    // asm 00001AE8: 	BEQ	RLL
    // asm 00001AE9: 	STI	AR2,*+AR7(PDATA)
    // asm 00001AEA: 	LDI	@DCALL,R1
    // asm 00001AEB: 	BZ	NODO56
    // asm 00001AEC: 	PUSH	AR2
    // asm 00001AED: 	PUSH	R0
    // asm 00001AEE: 	SONDFX	HYDRO
    // asm 00001AF0: 	POP	R0
    // asm 00001AF1: 	POP	AR2
NODO56:
    // asm 00001AF2: 	LDI	R0,IR0
    // asm 00001AF3: 	MPYI	C_SIZE,IR0
    // asm 00001AF4: 	ADDI	C_STRT,IR0
    // asm 00001AF5: 	INC	IR0
    // asm 00001AF6: 	LDF	*+AR7(IR0),R1
    // asm 00001AF7: 	DEC	IR0
    // asm 00001AF8: 	STF	R1,*+AR7(IR0)
    // asm 00001AF9: 	PUSH	AR2
    // asm 00001AFA: 	LDI	R0,AR2
    // asm 00001AFB: 	CALL	LIGHT_OFF
    // asm 00001AFC: 	POP	AR2
    // asm 00001AFD: 	LDI	R0,AR0
    // asm 00001AFE: 	LDF	0.5,R0
    // asm 00001AFF: 	CALL	CAR_DIMMER
    // asm 00001B00: 	LDI	AR2,IR0
    // asm 00001B01: 	MPYI	C_SIZE,IR0
    // asm 00001B02: 	ADDI	C_STRT,IR0
    // asm 00001B03: 	INC	IR0
    // asm 00001B04: 	LDF	*+AR7(IR0),R0
    // asm 00001B05: 	DEC	IR0
    // asm 00001B06: 	FLOAT	300,R1
    // asm 00001B07: 	SUBF	R1,R0
    // asm 00001B08: 	STF	R0,*+AR7(IR0)
    // asm 00001B09: 	PUSH	AR2
    // asm 00001B0A: 	CALL	LIGHT_ON
    // asm 00001B0B: 	POP	AR2
    // asm 00001B0C: 	LDI	AR2,AR0
    // asm 00001B0D: 	LDF	1.0,R0
    // asm 00001B0E: 	CALL	CAR_DIMMER
RLL:
    // asm 00001B0F: 	CALL	AFFECT_THE_CARS
    // asm 00001B10: 	SLEEP	1
    // asm 00001B12: 	BU	ROUNDERLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROUNDER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *AFFECT CARS
 *
 *
 *PARAMETERS
 *	INDEX OF CAR
 *
 *
 *OPERATION
 *	RAISE VEHICLE AND SPIN
 *
 *
 */
static void AFFECT_THE_CARS(void) {
    // asm 00001B13: 	LDI	481h,AR2
    // asm 00001B14: 	LDI	RNDR_C1_DYH,IR0
    // asm 00001B15: 	LDI	0,IR1
    // asm 00001B16: 	CALL	AFFECTED_CAR
    // asm 00001B17: 	LDI	482h,AR2
    // asm 00001B18: 	LDI	RNDR_C2_DYH,IR0
    // asm 00001B19: 	LDI	1,IR1
    // asm 00001B1A: 	CALL	AFFECTED_CAR
    // asm 00001B1B: 	LDI	483h,AR2
    // asm 00001B1C: 	LDI	RNDR_C3_DYH,IR0
    // asm 00001B1D: 	LDI	2,IR1
    // asm 00001B1E: 	CALL	AFFECTED_CAR
    // asm 00001B1F: 	LDI	484h,AR2
    // asm 00001B20: 	LDI	RNDR_C4_DYH,IR0
    // asm 00001B21: 	LDI	3,IR1
    // asm 00001B22: 	CALL	AFFECTED_CAR
    // asm 00001B23: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AFFECT_THE_CARS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *HIDDEN VEHICLES
 *
 *
 *
 */
/* asm: IS_HIDDEN	.bss	IS_HIDDEN,1 */
int IS_HIDDEN;
/* asm: HIDDEN_TABLE	.word	jeepm,sbuspm,copcar,gtruck */
/* asm: 	 */
/* asm: 	 */
static int HIDDEN_TABLE[] = {
    jeepm_ROM,
    sbuspm_ROM,
    copcar_ROM,
    gtruck_ROM,
};

static void HIDDEN_VEHICLES(void) {
    // asm 00001B29: 	LDI	-1,R0
    // asm 00001B2A: 	STI	R0,@IS_HIDDEN
HIDDEN_VEHICLES_LP:
    // asm 00001B2B: 	SLEEP	1
    // asm 00001B2D: 	LDI	@CAR_CHOICE_GOTTEN,R0
    // asm 00001B2E: 	BZ	NSCD
    // asm 00001B2F: 	DIE
NSCD:
    // asm 00001B30: 	LDI	@SWITCHBUTS,R0
    // asm 00001B31: 	RS	16,R0
    // asm 00001B32: 	AND	SW_VIEW0_H|SW_VIEW1_H,R0
    // asm 00001B33: 	BZ	SETAS_ORIGINALS
    // asm 00001B34: SETAS_HIDDEN
    // asm 00001B34: 	LDI	@IS_HIDDEN,R0
    // asm 00001B35: 	CMPI	-1,R0
    // asm 00001B36: 	CALLNE	RESET_ORIGINAL
    // asm 00001B37: 	LDI	@CHOSEN_VEHICLE,AR2
    // ;	CMPI	1,AR2
    // ;	BEQ	HIDDEN_VEHICLES_LP
    // asm 00001B38: 	CMPI	3,AR2
    // asm 00001B39: 	BEQ	HIDDEN_VEHICLES_LP
    // asm 00001B3A: 	STI	AR2,@IS_HIDDEN
    // asm 00001B3B: 	ADDI	481h,AR2
    // asm 00001B3C: 	CALL	OBJ_FIND_FIRST
    // asm 00001B3D: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 00001B3E: 	ADDI	@HIDDEN_TABLEI,AR2
    // asm 00001B3F: 	LDI	*AR2,R0
    // asm 00001B40: 	STI	R0,*+AR0(OROMDATA)
    // asm 00001B41: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001B42: 	ANDN	O_1PAL,R0
    // asm 00001B43: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001B44: 	BU	HIDDEN_VEHICLES_LP
SETAS_ORIGINALS:
    // asm 00001B45: 	LDI	@IS_HIDDEN,R0
    // asm 00001B46: 	CMPI	-1,R0
    // asm 00001B47: 	BEQ	HIDDEN_VEHICLES_LP	;no one was hidden,
    // 					;just ignore
    // asm 00001B48: 	CALL	RESET_ORIGINAL
    // asm 00001B49: 	LDI	-1,R0
    // asm 00001B4A: 	STI	R0,@IS_HIDDEN
    // asm 00001B4B: 	BU	HIDDEN_VEHICLES_LP
    // *PARAMETERS
    // *	R0	INDEX OF PREVIOUS VEHICLES
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIDDEN_VEHICLES", 0, 0);
    UNIMPL();
}

static void RESET_ORIGINAL(void) {
    // 	;insert code here to set the old vehicle
    // 	;as
    // 	;
    // asm 00001B4C: 	PUSH	R0
    // asm 00001B4D: 	LDI	R0,AR2
    // asm 00001B4E: 	ADDI	481h,AR2
    // asm 00001B4F: 	CALL	OBJ_FIND_FIRST
    // asm 00001B50: 	POP	AR2
    // asm 00001B51: 	MPYI	6,AR2
    // asm 00001B52: 	ADDI	@CCTABI,AR2
    // asm 00001B53: 	LDI	*+AR2(3),R0
    // asm 00001B54: 	STI	R0,*+AR0(OROMDATA)
    // asm 00001B55: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001B56: 	OR	O_1PAL,R0
    // asm 00001B57: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001B58: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_ORIGINAL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: SPINCURR	.bss	SPINCURR,1 */
int SPINCURR;

/*
 *----------------------------------------------------------------------------
 *
 *
 *PARAMETERS
 *	AR2	ID
 *	IR0	DESIRED HEIGHT OFFSET of AR7
 *	IR1	IDX OF CAR
 *
 *
 */
static void AFFECTED_CAR(void) {
    // asm 00001B59: 	CALL	OBJ_FIND_FIRST
    // asm 00001B5A: 	LDF	*+AR0(OPOSY),R0
    // asm 00001B5B: 	LDF	*+AR7(IR0),R1
    // asm 00001B5C: 	SUBF	R0,R1,R0
    // asm 00001B5D: 	MPYF	0.15,R0
    // asm 00001B5E: 	ADDF	*+AR0(OPOSY),R0
    // asm 00001B5F: 	STF	R0,*+AR0(OPOSY)
    // asm 00001B60: 	PUSH	IR0
    // asm 00001B61: 	PUSH	AR0
    // asm 00001B62: 	LDI	AR0,AR1
    // asm 00001B63: 	SUBI	080h,AR2
    // asm 00001B64: 	CALL	OBJ_FIND_FIRST
    // asm 00001B65: 	LDF	*+AR1(OPOSY),R0
    // asm 00001B66: 	INC	IR0
    // asm 00001B67: 	SUBF	*+AR7(IR0),R0
    // asm 00001B68: 	ADDI	C_OFF2LIFT,IR0
    // asm 00001B69: 	ADDF	*+AR7(IR0),R0
    // asm 00001B6A: 	STF	R0,*+AR0(OPOSY)
    // asm 00001B6B: 	POP	AR0
    // asm 00001B6C: 	POP	IR0
    // asm 00001B6D: 	LDI	@SPINCURR,R0
    // asm 00001B6E: 	LDI	@CHOSEN_VEHICLE,R1
    // asm 00001B6F: 	CMPI	R0,R1
    // asm 00001B70: 	BEQ	IBOIBO
    // asm 00001B71: 	CMPI	R0,IR1
    // asm 00001B72: 	BNE	N12
    // 	;track to PI
    // asm 00001B73: 	LDF	*+AR0(ORADY),R2
    // asm 00001B74: 	LDLF	0.392699,R1
    // asm 00001B75: 	ADDF	R1,R2
    // asm 00001B76: 	CALL	NORMITS
    // asm 00001B77: 	LDF	PI,R0
    // asm 00001B78: 	ADDF	@START_RADY,R0
    // asm 00001B79: 	PUSHFL	R2
    // asm 00001B7B: 	LDF	R0,R2
    // asm 00001B7C: 	CALL	NORMITS
    // asm 00001B7D: 	LDF	R2,R0
    // asm 00001B7E: 	POPFL	R2
    // asm 00001B80: 	CALL	GETTHETADIFF
    // asm 00001B81: 	PUSHFL	R2
    // asm 00001B83: 	LDF	R0,R2
    // asm 00001B84: 	CALL	NORMITS
    // asm 00001B85: 	LDF	R2,R0
    // asm 00001B86: 	POPFL	R2
    // asm 00001B88: 	CMPF	0.04,R0
    // asm 00001B89: 	BLT	DOALL
    // asm 00001B8A: 	MPYF	0.10,R0
    // asm 00001B8B: 	ADDF	R0,R2
    // asm 00001B8C: 	BU	IBO45
DOALL:
    // asm 00001B8D: ADDF	R0,R2
    // asm 00001B8E: 	LDI	@CHOSEN_VEHICLE,R1
    // asm 00001B8F: 	STI	R1,@SPINCURR
IBO45:
    // asm 00001B90: STF	R2,*+AR0(ORADY)
    // asm 00001B91: 	LDI	AR0,AR2
    // asm 00001B92: 	ADDI	OMATRIX,AR2
    // asm 00001B93: 	CALL	FIND_YMATRIX
IBOIBO:
    // asm 00001B94: CMPI	R1,IR1
    // asm 00001B95: 	BNE	N12
    // 	;just spin
    // asm 00001B96: 	LDF	*+AR0(ORADY),R2
    // asm 00001B97: 	ADDF	0.1,R2
    // asm 00001B98: 	STF	R2,*+AR0(ORADY)
    // asm 00001B99: 	LDI	AR0,AR2
    // asm 00001B9A: 	ADDI	OMATRIX,AR2
    // asm 00001B9B: 	CALL	FIND_YMATRIX
N12:
    // asm 00001B9C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AFFECTED_CAR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR0	CAR INDEX
 *	R0	(FL) DIMMER VALUE
 *
 */
static void CAR_DIMMER(void) {
    // asm 00001B9D: 	LDI	AR0,AR1
    // asm 00001B9E: 	ADDI	@CARPAL_TABLEI,AR1
    // asm 00001B9F: 	LDI	*AR1,AR1		;NOW HOLDS RAM LOCATION
    // asm 00001BA0: 	ADDI	@CARSRCPAL_TABI,AR0
    // asm 00001BA1: 	LDI	*AR0,AR0
    // asm 00001BA2: 	ADDI	@PALROMI,AR0
    // asm 00001BA3: 	LDI	*AR0,AR0
    // asm 00001BA4: 	CALL	PAL_DIMMER
    // asm 00001BA5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAR_DIMMER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void LIGHT_INIT(void) {
    // asm 00001C27: 	PUSH	AR2
    // asm 00001C28: 	LDL	flour_lghtof,AR2
    // asm 00001C29: 	CALL	PAL_ALLOC_RAW
    // asm 00001C2A: 	LDI	0,AR2
    // asm 00001C2B: 	CALL	LIGHT_OFF
    // asm 00001C2C: 	LDI	1,AR2
    // asm 00001C2D: 	CALL	LIGHT_OFF
    // asm 00001C2E: 	LDI	2,AR2
    // asm 00001C2F: 	CALL	LIGHT_OFF
    // asm 00001C30: 	LDI	3,AR2
    // asm 00001C31: 	CALL	LIGHT_OFF
    // asm 00001C32: 	POP	AR2
    // asm 00001C33: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_INIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *PARAMETERS
 *	AR2	INDEX  (0 to 3)
 *
 */
static void LIGHT_OFF(void) {
    // asm 00001C34: 	PUSH	R0
    // asm 00001C35: 	PUSH	AR0
    // asm 00001C36: 	PUSH	AR2
    // asm 00001C37: 	ADDI	601h,AR2
    // asm 00001C38: 	PUSH	AR2
    // asm 00001C39: 	CALL	OBJ_FIND_FIRST
    // asm 00001C3A: 	LDL	flour_lghtof,AR2
    // asm 00001C3B: 	CALL	PAL_FIND_RAW
    // asm 00001C3C: 	STI	R0,*+AR0(OPAL)
    // asm 00001C3D: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001C3E: 	OR	O_1PAL,R0
    // asm 00001C3F: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001C40: 	POP	AR2
    // asm 00001C41: 	SUBI	200h,AR2
    // asm 00001C42: 	CALL	OBJ_FIND_FIRST
    // asm 00001C43: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001C44: 	ANDN	O_1PAL,R0
    // asm 00001C45: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001C46: 	POP	AR2
    // asm 00001C47: 	POP	AR0
    // asm 00001C48: 	POP	R0
    // asm 00001C49: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_OFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *as above
 *
 */
static void LIGHT_ON(void) {
    // asm 00001C4A: 	PUSH	R0
    // asm 00001C4B: 	PUSH	AR0
    // asm 00001C4C: 	PUSH	AR2
    // asm 00001C4D: 	ADDI	601h,AR2
    // asm 00001C4E: 	CALL	OBJ_FIND_FIRST
    // asm 00001C4F: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001C50: 	ANDN	O_1PAL,R0
    // asm 00001C51: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001C52: 	SUBI	200h,AR2
    // asm 00001C53: 	CALL	OBJ_FIND_FIRST
    // asm 00001C54: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001C55: 	ANDN	O_1PAL,R0
    // asm 00001C56: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001C57: 	POP	AR2
    // asm 00001C58: 	POP	AR0
    // asm 00001C59: 	POP	R0
    // asm 00001C5A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_ON", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *USES
 *	R5	AS A DEDICATED REG
 *
 */
void INIT_PEDALCHK(void) {
    // asm 00001C5B: 	CLRI	R5				;FLAG : HAS THE PEDAL BEEN RELEASEDP
    // asm 00001C5C: 	LDI	@_pot1,R0
    // asm 00001C5D: 	FIX	@PEDALMN,R1
    // asm 00001C5E: 	ADDI	20,R1
    // asm 00001C5F: 	CMPI	R1,R0
    // asm 00001C60: 	LDIGE	1,R5				;GE -> IT HASN'T
    // asm 00001C61: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_PEDALCHK", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void GETCHOICE(void) {
    // asm 00001C62: 	PUSHF	R4
    // asm 00001C63: 	PUSH	R4
    // asm 00001C64: 	LDI	@_MODE,R4
    // asm 00001C65: 	AND	MMODE,R4
    // asm 00001C66: 	FLOAT	@_pot0,R3 		;GET CURRENT WHEEL
    // asm 00001C67: 	FLOAT	@POSES,R1		;GET # POSES
    // asm 00001C68: 	LDF	@STEERFR,R0
    // asm 00001C69: 	CALL	DIV_F
    // asm 00001C6A: 	LDF	R0,R1
    // asm 00001C6B: 	MPYF	0.5,R1
    // asm 00001C6C: 	FLOAT	@POSE,R2		;@CHOSEN_VEHICLE,R2
    // asm 00001C6D: 	MPYF	R0,R2
    // asm 00001C6E: 	ADDF	R1,R2	 		;MIDDLE OF ZONE
    // asm 00001C6F: 	ADDF	@STEERMN,R2		;ADD IN MINIMUM
    // asm 00001C70: 	CMPI	MINIT,R4
    // asm 00001C71: 	BEQ	GETCHA
    // asm 00001C72: 	STF	R2,@WHEELPOS
GETCHA:
    // asm 00001C73: 	SUBF	R3,R2
    // asm 00001C74: 	ABSF	R2,R3			;FIND DIFFERENCE
    // asm 00001C75: 	MPYF	0.6,R0		   	;SLIGHT HYSTERESIS
    // asm 00001C76: 	CMPF	R0,R3
    // asm 00001C77: 	BLE	GETCHX
    // asm 00001C78: 	LDF	R2,R2
    // asm 00001C79: 	LDILT	1,R0
    // asm 00001C7A: 	LDIGE	-1,R0
    // asm 00001C7B: 	ADDI	@POSE,R0		;CHOSEN_VEHICLE,R0
    // asm 00001C7C: 	LDFLT	0,R0
    // asm 00001C7D: 	CMPI	@POSES,R0
    // asm 00001C7E: 	BLT	GETCH1
    // asm 00001C7F: 	LDI	@POSES,R0
    // asm 00001C80: 	SUBI	1,R0
GETCH1:
    // asm 00001C81: STI	R0,@POSE
GETCHX:
    // asm 00001C82: 	POP	R4
    // asm 00001C83: 	POPF	R4
    // asm 00001C84: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCHOICE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *USES
 *	R5	see above
 *RETURNS
 *	NC	FALSE
 *	C	TRUE
 *
 */
void PEDALCHK(void) {
    // asm 00001C85: 	LDI	@_pot1,R0			;set in main IRQ
    // asm 00001C86: 	LDP	@PEDALMN
    // asm 00001C87: 	FIX	@PEDALMN,R1
    // asm 00001C88: 	ADDI	20,R1
    // asm 00001C89: 	CMPI	R1,R0
    // asm 00001C8A: 	LDILT	0,R5
    // asm 00001C8B: 	BLT	SKIPKEY
    // asm 00001C8C: 	LDI	R5,R5				;IF the pedal has not yet been released
    // asm 00001C8D: 	BZ	PEDALTRUE			;up do not accept this as a valid pedal choice
SKIPKEY:
    // asm 00001C8E: PEDALFALSE
    // asm 00001C8E: 	CLRC
    // asm 00001C8F: 	RETS
PEDALTRUE:
    // asm 00001C90: 	SETC
    // asm 00001C91: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PEDALCHK", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static const char T_READY[] = "READY";
static const char T_SET[] = "SET";
static const char T_GO[] = "GO";
static const char T_CHALLENG[] = "CHALLENGE RACE";
/* asm: TLIST	.word	T_READY,CHICK_READY,SEND_WAVEFL_READY */
/* asm: 	.word	T_SET,CHICK_SET,SEND_WAVEFL_SET */
static uintptr_t TLIST[] = {
    (uintptr_t)&T_READY,
    (uintptr_t)(CHICK_READY),
    (uintptr_t)SEND_WAVEFL_READY,
    (uintptr_t)&T_SET,
    (uintptr_t)(CHICK_SET),
    (uintptr_t)SEND_WAVEFL_SET,
};
/* asm: TLGO	.word	T_GO,CHICK_GO,SEND_WAVEFL_GO */
/* asm: 	 */
static uintptr_t TLGO[] = {
    (uintptr_t)&T_GO,
    (uintptr_t)(CHICK_GO),
    (uintptr_t)SEND_WAVEFL_GO,
};
/* asm: BABE_CONTROL	.bss	BABE_CONTROL,1 */
int BABE_CONTROL;
/* asm: CURR_FLAGSTATE	.bss	CURR_FLAGSTATE,1 */
int CURR_FLAGSTATE;

void WAVEFLAG(PROC* p) {
    // asm 00001C9B: 	LDP	@STOPWATCH_CNTL
    // asm 00001C9C: 	CLRI	R0
    // asm 00001C9D: 	STI	R0,@STOPWATCH_CNTL
    // asm 00001C9E: 	STI	R0,@BABE_CONTROL
    // asm 00001C9F: 	LDP	@STOPWATCH
    // asm 00001CA0: 	STI	R0,@STOPWATCH
    // asm 00001CA1: 	LDI	@_MODE,R0
    // asm 00001CA2: 	LDI	R0,R5
    // asm 00001CA3: 	OR	MHUD|MSLINE,R0
    // asm 00001CA4: 	STI	R0,@_MODE
    // asm 00001CA5: 	READADJ	ADJ_FREEGAME
    // asm 00001CA7: 	CMPI	0,R0
    // asm 00001CA8: 	BEQ	FGLL
    // asm 00001CA9: 	CREATE	BLINK_FREEBE,UTIL_C
    // asm 00001CAC: 	LDI	@_MODE,R0
    // asm 00001CAD: 	AND	MMODE,R0
    // asm 00001CAE: 	CMPI	MATTR,R0
    // asm 00001CAF: 	BEQ	FGLL
    // asm 00001CB0: 	CREATE	SHOW_RACE_NAME,UTIL_C
FGLL:
    // ;	LDI	@HEAD2HEAD_ON,R0
    // ;	CALLNZ	SEND_BSYNC0
    // asm 00001CB3: 	SLEEP	5
    // asm 00001CB5: 	LDI	@_MODE,R0
    // asm 00001CB6: 	AND	MMODE,R0
    // asm 00001CB7: 	CMPI	MATTR,R0
    // asm 00001CB8: 	BEQ	NOBABE
    // asm 00001CB9: 	CREATEC	BABE_WAVEFLAG,UTIL_C
NOBABE:
    // asm 00001CBC: 	CLRI	R0
    // asm 00001CBD: 	STI	R0,@CURR_FLAGSTATE
    // asm 00001CBE: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001CBF: 	BZ	NOHEAD2HEAD
    // asm 00001CC0: 	CLRI	R0
    // asm 00001CC1: 	STI	R0,@OM_BSYNC
H2HWTLP:
    // asm 00001CC2: 	SLEEP	1
    // asm 00001CC4: 	CALL	SEND_BSYNC3
    // asm 00001CC5: 	LDI	@OM_BSYNC,R0
    // asm 00001CC6: 	CMPI	3,R0
    // asm 00001CC7: 	BNE	H2HWTLP
NOHEAD2HEAD:
    // 	;-----------------------------------
    // 	;	-----------------------------------
    // 	;		-----------------------------------
    // asm 00001CC8: 	SONDFX	STARTLINEREVS2
    // asm 00001CCA: 	LDI	2,AR5
    // asm 00001CCB: 	LDL	TLIST,AR6
WAVEFLAGLP:
    // asm 00001CCC: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001CCD: 	BZ	JDJFF
    // asm 00001CCE: 	LDI	@DIPRAM,R0
    // asm 00001CCF: 	TSTB	CMDP_MASTER,R0
    // asm 00001CD0: 	BZ	JDJFF
JFF:
    // asm 00001CD1: LDI	@CURR_FLAGSTATE,R0
    // asm 00001CD2: 	CMPI	3,R0
    // asm 00001CD3: 	BEQ	JUMPOUT
    // asm 00001CD4: 	CMPI	@H2H_FLAGSTATE,R0
    // asm 00001CD5: 	BLT	NXTSTAT
    // asm 00001CD6: 	SLEEP	1
    // asm 00001CD8: 	BU	JFF
NXTSTAT:
    // asm 00001CD9: LDI	@H2H_FLAGSTATE,R0
    // asm 00001CDA: 	STI	R0,@CURR_FLAGSTATE
    // asm 00001CDB: 	BU	KKLFF
JDJFF:
    // asm 00001CDC: 	SLEEP	20
KKLFF:
    // asm 00001CDE: 	LDI	*AR6++,AR2
    // asm 00001CDF: 	FLOAT	256,R2
    // asm 00001CE0: 	FLOAT	160,R3
    // asm 00001CE1: 	LDI	20,RC
    // asm 00001CE2: 	CALL	TEXT_ADD
    // asm 00001CE3: 	CALL	SET40FONT
    // asm 00001CE4: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001CE7: 	LDI	*AR6++,AR2
    // asm 00001CE8: 	CALL	ONESNDFX
    // 	;if H2H_ON && MASTER
    // 	;then  SEND_STATE
    // 	;
    // asm 00001CE9: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001CEA: 	BZ	BABAD
    // asm 00001CEB: 	LDI	@DIPRAM,R0
    // asm 00001CEC: 	TSTB	CMDP_MASTER,R0
    // asm 00001CED: 	BNZ	BABAD
    // asm 00001CEE: 	LDI	*AR6,R0
    // asm 00001CEF: 	CALLU	R0
BABAD:
    // asm 00001CF0: ADDI	1,AR6
    // asm 00001CF1: 	DEC	AR5
    // asm 00001CF2: 	CMPI	0,AR5
    // asm 00001CF3: 	BGT	WAVEFLAGLP
JUMPOUT:
    // asm 00001CF4: LDL	TLGO,AR6
    // asm 00001CF5: 	SLEEP	15
    // asm 00001CF7: 	LDI	1,R0
    // asm 00001CF8: 	STI	R0,@BABE_CONTROL
    // asm 00001CF9: 	SLEEP	5
    // asm 00001CFB: 	LDI	*AR6++,AR2
    // asm 00001CFC: 	FLOAT	256,R2
    // asm 00001CFD: 	FLOAT	160,R3
    // asm 00001CFE: 	LDI	20,RC
    // asm 00001CFF: 	CALL	TEXT_ADD
    // asm 00001D00: 	CALL	SET40FONT
    // asm 00001D01: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001D04: 	LDI	*AR6++,AR2
    // asm 00001D05: 	CALL	ONESNDFX
    // asm 00001D06: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001D07: 	BZ	BABAD666
    // asm 00001D08: 	LDI	@DIPRAM,R0
    // asm 00001D09: 	TSTB	CMDP_MASTER,R0
    // asm 00001D0A: 	BNZ	BABAD666
    // asm 00001D0B: 	CALL	SEND_WAVEFL_GO
BABAD666:
    // asm 00001D0C: 	OR	MGO,R5			;SAVED MODE
    // asm 00001D0D: 	ANDN	MSLINE,R5
    // asm 00001D0E: 	STI	R5,@_MODE
    // asm 00001D0F: 	STI	R5,@STOPWATCH_CNTL	;STOPWATCH TIMER
    // asm 00001D10: 	CALL	CHECK_MOTION_DIP
    // asm 00001D11: 	BNZ	NANAD			;RETURN IF NON MOVING
    // asm 00001D12: 	CALL	CHECK_MOTION_PRESENT
    // asm 00001D13: 	BNE	NANAD
    // asm 00001D14: 	CLRI	AR2
    // asm 00001D15: 	LDP	@991030h
    // asm 00001D16: 	LDI	@991030h,R2
    // asm 00001D17: 	LDI	*AR2,AR2
    // asm 00001D18: 	SETDP
    // asm 00001D19: 	LDL	0FF80h,R1
    // asm 00001D1A: 	AND	R1,R2
    // asm 00001D1B: 	BZ	NANAD
    // asm 00001D1C: 	LDI	MOTION_ERROR_TIKS,R1
    // asm 00001D1D: 	STI	R1,@WAITTIK
    // asm 00001D1E: 	BU	NANAD
    // asm 00001D1F: 	CALL	ABORT_RESET_GALIL
    // asm 00001D20: 	LDL	XQ,AR2				;tell galil to continue executing program
    // asm 00001D21: 	CALL	SEND_CMD
    // asm 00001D22: 	CALL	WAIT_ACK
NANAD:
    // asm 00001D23: 	LDI	SM_GO,R0
    // asm 00001D24: 	STI	R0,@SUSPEND_MODE
    // asm 00001D25: 	SONDFX	PEELOUT
    // asm 00001D27: 	READAUD	ADJ_TIME_TO_START
    // asm 00001D29: 	MPYI	5,R0
    // asm 00001D2A: 	ADDI	60,R0
    // asm 00001D2B: 	STI	R0,@_countdown
    // asm 00001D2C: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001D2D: 	BZ	NOTHHHH
    // asm 00001D2E: 	LDI	@OM_RACE_MODE,R0
    // asm 00001D2F: 	CMPI	RM_USA,R0
    // asm 00001D30: 	BNE	NOTHHHH
    // asm 00001D31: 	LDI	RM_USA,R0
    // asm 00001D32: 	STI	R0,@RACE_MODE
NOTHHHH:
    // asm 00001D33: 	LDI	@_MODE,R0	   	;MAKE SURE MODE IS IN GAME
    // asm 00001D34: 	AND	MMODE,R0
    // asm 00001D35: 	CMPI	MATTR,R0
    // asm 00001D36: 	CALLNE	RESUME_TUNE_NT
    // asm 00001D37:  	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAVEFLAG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void RACESEL_TIMER(void) {
    // asm 00001D38: 	LDI	@_countdown,R2
    // asm 00001D39: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm 00001D3A: 	CALL	_itoa
    // asm 00001D3B: 	BUD	IT_E2
    // asm 00001D3C: 	FLOAT	256,R2
    // asm 00001D3D: 	FLOAT	253,R3
    // asm 00001D3E: 	LDI	1,RC
    // 	;---->	BUD	IT_E2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RACESEL_TIMER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
static void WAITINTROTIMER(void) {
    // asm 00001D3F: 	FLOAT	215,R3
    // asm 00001D40: 	BU	LKJAFSD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAITINTROTIMER", 0, 0);
    UNIMPL();
}

void INTROTIMER(void) {
    // asm 00001D41: 	FLOAT	350,R3
LKJAFSD:
    // asm 00001D42: 	PUSHF	R3
    // asm 00001D43: 	LDI	@_countdown,R2
    // asm 00001D44: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm 00001D45: 	CALL	_itoa
    // asm 00001D46: 	POPF	R3
    // asm 00001D47: 	FLOAT	256,R2
    // asm 00001D48: 	LDI	1,RC
IT_E2:
    // asm 00001D49: CALL	TEXT_ADD
    // asm 00001D4A: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001D4D: 	CALL	SETN43FONT
    // asm 00001D4E: 	LDL	lgnum43_coolyelo,AR2
    // asm 00001D4F: 	CALL	PAL_FIND_RAW
    // asm 00001D50: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00001D51: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INTROTIMER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DIAL ROUT
 *
 *SET THE PROPER OBJECT COLOR CYCLING
 *
 */
/* asm: LASTCHOICE	.bss	LASTCHOICE,1 */
int LASTCHOICE;

void DIAL_ROUT(void) {
    // asm 00001D52: 	LDI	@POSE,AR2
    // asm 00001D53: 	CMPI	0,AR2
    // asm 00001D54: 	LDILT	0,AR2
    // asm 00001D55: 	CMPI	3,AR2
    // asm 00001D56: 	LDIGT	3,AR2
    // asm 00001D57: 	STPI	AR2,@CHOSEN_VEHICLE
    // asm 00001D58: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAL_ROUT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *END OF PLAYERS GAME
 *
 *	COMPUTE TIME
 *	SHOW STATISTICS
 *	IDENTIFY GAME OVER
 *	HSTD IF NESSESARY
 *	GOTO ATTRACT MODE
 *
 *
 */
void ENDPLAYER(void) {
    // asm 00001D59: 	CLRI	R2
    // asm 00001D5A: 	SETAUD	AUD_BCREDITS
    // asm 00001D5C: 	LDI	AUD_NUM_UNFINISHED,AR2
    // asm 00001D5D: 	CALL	AUDIT_READ
    // asm 00001D5E: 	DEC	R0
    // asm 00001D5F: 	LDI	R0,R2
    // asm 00001D60: 	SETAUD	AUD_NUM_UNFINISHED
    // asm 00001D62: 	LDI	0,R0
    // asm 00001D63: 	STI	R0,@FRAMRATE   		;RESET FRAME RATE TO ATTRACT MODE
    // ;	LDI	-5,AR2			;HSTD SHOULD BE NEXT SCREEN!
    // asm 00001D64: 	LDI	-2,AR2			;HSTD SHOULD BE NEXT SCREEN!
    // asm 00001D65: 	STI	AR2,@_ATTR_MODE
    // asm 00001D66: 	LDI	DRONE_C,R0
    // asm 00001D67: 	LDI	CLASS_M,R1
    // asm 00001D68: 	CALL	PRC_KILLALL
    // asm 00001D69: 	LDI	SPAWNER_C,R0
    // asm 00001D6A: 	LDI	CLASS_M,R1
    // asm 00001D6B: 	CALL	PRC_KILLALL
    // *ELP CHANGE
    // asm 00001D6C: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 00001D71: 	DMA_WT		R0
    // asm 00001D76: 	LDI	1,R0
    // asm 00001D77: 	STI	R0,@CLEARRDY	  	;READY FOR INTERRUPT
KK5:
    // asm 00001D78: LDI	@CLEARRDY,R0
    // asm 00001D79: 	BNZ	KK5
    // *ELP END CHANGE
    // *
    // *NOW CLEAN UP THE SYSTEM,. REINITIALIZE EVERYTHING AND
    // *GO INTO ATTRACT MODE
    // *
    // asm 00001D7A: 	CALL	TEXT_INIT
    // ;	CLRI	AR2
    // ;	CALL	SENDSND
    // ;	SOND1	GAMEOVR
    // *ELP CHANGE
    // ;	LDL	_SECggate,AR2
    // ;	CALL	LOAD_SECTION_REQ
    // *ELP END CHANGE
    // asm 00001D7B: 	CLRI	R0			;R0	(B0-15) PID
    // asm 00001D7C: 	CLRI	R1			;R1	(B0-15) MASK
    // asm 00001D7D: 	CALL	PRC_KILLALL
    // asm 00001D7E: 	BU	SET_ATTR
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENDPLAYER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GAME AVAILABLE P
 *RETURNS
 *	CARRY SET  CREDIT ENTER - GAME IS AVAILABLE
 *	CARRY CLR  NO CREDIT AVAILABLE
 *	R0	CREDITS AVAILABLE
 *
 */
static void GAME_AVAILABLEP(void) {
    // asm 00001D7F: 	PUSH	R2
    // asm 00001D80: 	PUSH	AR2
    // asm 00001D81: 	READADJ	ADJ_FREE_PLAY
    // asm 00001D83: 	CMPI	1,R0
    // asm 00001D84: 	BEQ	GA_TRUE
    // asm 00001D85: 	CALL	GET_CREDITS_TO_START
    // asm 00001D86: 	READAUD	AUD_CREDITS
    // asm 00001D88: 	CMPI	R1,R0
    // asm 00001D89: 	BLT	GA_FALSE
GA_TRUE:
    // asm 00001D8A: 	SETC
    // asm 00001D8B: 	POP	AR2
    // asm 00001D8C: 	POP	R2
    // asm 00001D8D: 	RETS
GA_FALSE:
    // asm 00001D8E: 	CLRC
    // asm 00001D8F: 	POP	AR2
    // asm 00001D90: 	POP	R2
    // asm 00001D91: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GAME_AVAILABLEP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *START BUTTON
 *
 *
 */
void _start(void) {
    // asm 00001D92: 	LDI	@_MODE,R0
    // asm 00001D93: 	AND	MMODE,R0
    // asm 00001D94: 	CMPI	MCT,R0
    // asm 00001D95: 	BEQ	CN
    // asm 00001D96: 	CMPI	MBONUS,R0
    // asm 00001D97: 	BEQ	CN
    // asm 00001D98: 	CMPI	MINSERT_COINS,R0
    // asm 00001D99: 	BEQ	CN
    // asm 00001D9A: 	CMPI	MINIT,R0
    // asm 00001D9B: 	BEQ	CN
    // asm 00001D9C: 	CMPI	MINTRO,R0
    // asm 00001D9D: 	BNE	NOTINTRO
CN:
    // asm 00001D9E: LDI	1,R0
    // asm 00001D9F: 	STI	R0,@START_HIT
    // asm 00001DA0: 	DIE
NOTINTRO:
    // asm 00001DA1: NOTINSRT
    // asm 00001DA1: 	CMPI	MGAME,R0
    // asm 00001DA2: 	BEQ	_startX
    // asm 00001DA3: 	CALL	GAME_AVAILABLEP
    // asm 00001DA4: 	BC	CANSTART
    // 	;NO CREDITS TO START!
    // asm 00001DA5: 	LDI	@TEASE_COUNT,R0
    // asm 00001DA6: 	CMPI	0,R0
    // asm 00001DA7: 	BGT	_startX
    // asm 00001DA8: 	INC	R0
    // asm 00001DA9: 	STI	R0,@TEASE_COUNT
    // asm 00001DAA: 	SOND1	TEASE_TURNKEY
    // asm 00001DAC: 	DIE
CANSTART:
    // asm 00001DAD: 	CALL	GET_CREDITS_TO_START
    // asm 00001DAE: 	SUBI	R1,R0
    // asm 00001DAF: 	LDI	R0,R2
    // asm 00001DB0: 	CMPI	0,R2
    // asm 00001DB1: 	LDILT	0,R2
    // asm 00001DB2: 	SETAUD	AUD_CREDITS		;DECREMENT CREDIT COUNT
    // asm 00001DB4: 	LDI	0,R2
    // asm 00001DB5: 	LDI	AUD_BCREDITS,AR2
    // asm 00001DB6: 	CALL	AUDIT_WRITE
    // asm 00001DB7: startgame
    // asm 00001DB7: 	LDI	@_MODE,R0
#if DEBUG
    // asm: 	LDI	R0,R1
    // asm: 	ANDN	MMODE,R0
    // asm: 	AND	MMODE,R1
    // asm: 	CMPI	MATTR,R1		;this kludge allows us, in debugging
    // asm: 	BEQ	CYCLEOUT		;more to cycle out of game mode into
    // asm: 	OR	MATTR,R1		;attract mode...
    // asm: 	STI	R1,@_MODE		;
    // 	;
    // 	;SYSTEM SHUTDOWN, STRAIGHTLINE CODE NOW ACTIVE
    // 	;
    // asm: 	BR	CYCLE_ATTR
    // 	;
    // 	;
CYCLEOUT:
#endif
    // asm 00001DB8: 	ANDN	MMODE,R0
    // *ELP CHANGE February 8,1995
    // ;	OR	MGAME,R0
    // 	;we must begin the game in MINTRO
    // 	;
    // 	;
    // asm 00001DB9: 	OR	MINTRO,R0
    // *ELP END CHANGE
    // asm 00001DBA: 	STI	R0,@_MODE
    // asm 00001DBB: 	CLRI	R0
    // asm 00001DBC: 	STI	R0,@(_plyr1+PLY_CAR)
    // asm 00001DBD:         LDP     @FASTSTKI		;GET PAGE OF STORED ADDRESS
    // asm 00001DBE:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm 00001DBF: 	LDI	1,AR2
    // asm 00001DC0: 	CALL	WAVE
    // asm 00001DC1: 	BR	COLD_ENTER		;RESET SYSTEM RUNNING
_startX:
    // asm 00001DC2: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_start", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR4	OBJECT
 *
 *
 */
static void ULTRA_PROC(PROC* p /*AR7*/) {
    OBJ* obj;
    float radians;

    switch (p->resume_state) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 00001DC3: 	CLRF	R6
    p->ctx->ULTRA_PROC.radians = 0.0f;

UPLP:
    // asm 00001DC4: LDF	0.0349078,R0
    // asm 00001DC5: 	FLOAT	@NFRAMES,R1
    // asm 00001DC6: 	MPYF	R1,R0
    // asm 00001DC7: 	SUBF	R0,R6
    p->ctx->ULTRA_PROC.radians -= 0.0349078f * (float)NFRAMES;

    // asm 00001DC8: 	LDF	R6,R2
    // asm 00001DC9: 	LDI	AR4,AR2
    // asm 00001DCA: 	ADDI	OMATRIX,AR2
    // asm 00001DCB: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&p->ctx->ULTRA_PROC.obj->omatrix, p->ctx->ULTRA_PROC.radians);

    // asm 00001DCC: 	SLEEP	1
    SLEEP(1, 1);

    // asm 00001DCE: 	BU	UPLP
    goto UPLP;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void ULTRA_LOGO(void) {
    OBJ* obj;

    // asm 00001DCF: 	CALL	OBJ_GET
    obj = OBJ_GET();

    // asm 00001DD0: 	RETSC
    if (obj == NULL) {
        return;
    }

    // asm 00001DD1: 	LDIL	nintendo,R0
    // asm 00001DD4: 	STI	R0,*+AR0(OROMDATA)
    obj->romdata = ROM_PTR(nintendo_ROM);

    // asm 00001DD5: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm 00001DD6: 	OR	*+AR0(OFLAGS),R0
    // asm 00001DD7: 	STI	R0,*+AR0(OFLAGS)
    obj->flags |= O_IROT | O_NOROT | O_NOUROT | O_NOUNIV;

    // asm 00001DD8: 	CLRF	R0
    // asm 00001DD9: 	STF	R0,*+AR0(OPOSX)
    obj->posx = 0.0f;

    // asm 00001DDA: 	FLOAT	50,R0
    // asm 00001DDB: 	STF	R0,*+AR0(OPOSY)
    obj->posy = 50.0f;

    // asm 00001DDC: 	FLOAT	368,R0
    // asm 00001DDD: 	STF	R0,*+AR0(OPOSZ)
    obj->posz = 368.0f;

    // asm 00001DDE: 	LDI	AR0,AR2
    // asm 00001DDF: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);

    // asm 00001DE0: 	LDI	AR2,AR4
    // asm 00001DE1: 	PUSH	AR0
    // asm 00001DE2: 	PUSH	AR2

    // asm 00001DE3: 	CREATE	ULTRA_PROC,UTIL_C
    PROC_CONTEXT* ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->ULTRA_PROC.obj = obj;
    CREATE(ULTRA_PROC, UTIL_C, ctx);

    // asm 00001DE6: 	POP	AR2
    // asm 00001DE7: 	POP	AR0
    // asm 00001DE8: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void LOGO_SMALL(void) {
    // asm 00001DE9: 	LDI	@DIPRAM,R0
    // asm 00001DEA: 	TSTB 	DIP_COMMP,R0
    // asm 00001DEB: 	RETSNZ
    // asm 00001DEC: 	LDL	redhd1,AR2	;red (bottom)
    // asm 00001DED: 	LDI	230,R2
    // asm 00001DEE: 	LDI	-190-60,R3
    // asm 00001DEF: 	LDI	926,RC	;368*2
    // asm 00001DF0: 	CALL	OBJ_QMAKE
    // asm 00001DF1: 	RETSC
    // asm 00001DF2: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 00001DF3: 	OR	*+AR0(OFLAGS),R0
    // asm 00001DF4: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001DF5: 	LDI	AR0,AR2
    // asm 00001DF6: 	CALL	OBJ_INSERT
    // asm 00001DF7: 	LDL	H2HPAL2,AR2
    // asm 00001DF8: 	CALL	PAL_FIND_RAW
    // asm 00001DF9: 	STI	R0,*+AR0(OPAL)
    // asm 00001DFA: 	LDL	yelhd1,AR2	;top
    // asm 00001DFB: 	LDI	230,R2
    // asm 00001DFC: 	LDI	-190+60,R3
    // asm 00001DFD: 	LDI	926,RC	;368*2
    // asm 00001DFE: 	CALL	OBJ_QMAKE
    // asm 00001DFF: 	RETSC
    // asm 00001E00: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 00001E01: 	OR	*+AR0(OFLAGS),R0
    // asm 00001E02: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001E03: 	LDI	AR0,AR2
    // asm 00001E04: 	CALL	OBJ_INSERT
    // asm 00001E05: 	LDL	H2HPAL2,AR2
    // asm 00001E06: 	CALL	PAL_FIND_RAW
    // asm 00001E07: 	STI	R0,*+AR0(OPAL)
    // asm 00001E08: 	LDL	big2,AR2
    // asm 00001E09: 	LDI	230,R2
    // asm 00001E0A: 	LDI	-190,R3
    // asm 00001E0B: 	LDI	924,RC	;(368*2)-2
    // asm 00001E0C: 	CALL	OBJ_QMAKE
    // asm 00001E0D: 	RETSC
    // asm 00001E0E: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 00001E0F: 	OR	*+AR0(OFLAGS),R0
    // asm 00001E10: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001E11: 	LDI	AR0,AR2
    // asm 00001E12: 	CALL	OBJ_INSERT
    // asm 00001E13: 	LDL	H2HPAL3,AR2
    // asm 00001E14: 	CALL	PAL_FIND_RAW
    // asm 00001E15: 	STI	R0,*+AR0(OPAL)
    // asm 00001E16: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOGO_SMALL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *STRAIGHT LINE CODE, SYSTEM SHUTDOWN
 *THIS ROUTINE IS BRANCHED TO, NOT CALLED!
 *
 *
 */
void SET_ATTR(void) {
    // asm 00001E17: 	CALL	SILENT
    // asm 00001E18:         LDI	@FASTSTKI,SP		;GET PAGE OF STORED ADDRESS
    // asm 00001E19: 	LDI	@_ATTR_MODE,AR2		;AND INTO FP TOO
    // asm 00001E1A: 	CALL	WAVE
    // asm 00001E1B: 	BU	COLD_ENTER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_ATTR", 0, 0);
    UNIMPL();
}

void _debug(void) {
    CYCLE_ATTR();
}

void CYCLE_ATTR(void) {
    // asm 00001E1C: 	CALL	SILENT
    SILENT();

    // asm 00001E1D: 	CALL	LOAD_FIXED_PALETTES
    LOAD_FIXED_PALETTES();

    // asm 00001E1E:         LDI	@FASTSTKI,SP		;GET PAGE OF STORED ADDRESS
    // ignored: stack pointer restore

    // asm 00001E1F: 	LDI	@_ATTR_MODE,AR2		;AND INTO FP TOO
    // asm 00001E20: 	DEC	AR2
    _ATTR_MODE--;

    // asm 00001E21: 	CMPI	-4,AR2
    // asm 00001E22: 	LDILT	-1,AR2
    if (_ATTR_MODE < -4) {
        _ATTR_MODE = -1;
    }

    // asm 00001E23: 	STI	AR2,@_ATTR_MODE

    // asm 00001E24: 	CALL	WAVE
    WAVE(_ATTR_MODE);

    // asm 00001E25: 	BU	COLD_ENTER		;RESET SYSTEM RUNNING
    COLD_ENTER(); // ;RESET SYSTEM RUNNING
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ATTRACT MODE TIMEOUT MECHANISM
 *SLEEP _timer TIKS THEN JUMP TO CYCLE_ATTR
 *
 */
/* asm: _timer	.bss	_timer,1 */
int _timer;

void _timeout(PROC* p) {
    switch (p->resume_state) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00001E26: 	LDI	@_timer,AR2
    // asm 00001E27: 	CALL	SLEEP
    SLEEP(_timer, 1);
    // asm 00001E28: 	BU	CYCLE_ATTR
    CYCLE_ATTR();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INSERT COINS ROUTINES
 *
 *	INSMORE		JSRPed FROM PLYR.ASM
 *	COIN_CNTDOWN	CREATED, KILLED
 *
 */
/* asm: TROI	SPTR	"INSERT COINS" */
static const char TROI[] = "INSERT COINS";
/* asm: ICCI	SPTR	"TO CONTINUE" */
static const char ICCI[] = "TO CONTINUE";
/* asm: PSCI	SPTR	"PRESS START" */
static const char PSCI[] = "PRESS START";
/* asm: SAVEDMODE	.bss	SAVEDMODE,1 */
int SAVEDMODE;

void INSMORE(void) {
    // asm 00001E2C: 	LDI	@_MODE,R0
    // asm 00001E2D: 	STI	R0,@SAVEDMODE
    // asm 00001E2E: 	CALL	CHECK_MOTION_DIP
    // asm 00001E2F: 	BNZ	KFFDA
    // asm 00001E30: 	CALL	ABORT_RESET_GALIL
    // asm 00001E31: 	LDI	0,R0
    // asm 00001E32: 	STI	R0,@MOTION_STOP_HIT
    // asm 00001E33: 	STI	R0,@MOTION_SAFETY_ON
    // asm 00001E34: 	STI	R0,@WAITTIK
KFFDA:
    // 	;
    // asm 00001E35: 	LDF	@GAME_TIMER,R2
    // asm 00001E36: 	MPYF	100,R2
    // asm 00001E37: 	FIX	R2
    // asm 00001E38: 	READAUD	AUD_TOTAL_TIME
    // asm 00001E3A: 	ADDI	R0,R2
    // asm 00001E3B: 	SETAUD	AUD_TOTAL_TIME
    // asm 00001E3D: 	READAUD	AUD_NUM_BUYINS
    // asm 00001E3F: 	LDI	R0,R1
    // asm 00001E40: 	LDI	R2,R0
    // asm 00001E41: 	CALL	DIV_I30
    // asm 00001E42: 	LDI	R0,R2
    // asm 00001E43: 	SETAUD	AUD_AVG_TIME
    // 	;
    // asm 00001E45: 	LDI	20,R0
    // asm 00001E46: 	STI	R0,@_countdown
    // asm 00001E47: 	CALL	SILENT
    // asm 00001E48: 	SOND1	DISCODUCK
    // asm 00001E4A: 	CLRI	R0
    // asm 00001E4B: 	STI	R0,@STOPWATCH_CNTL
    // asm 00001E4C: 	STI	R0,@START_HIT
    // asm 00001E4D: 	LDI	@_MODE,R0
    // asm 00001E4E: 	ANDN	MMODE,R0
    // asm 00001E4F: 	OR	MINSERT_COINS|MGO,R0
    // asm 00001E50: 	STI	R0,@_MODE
    // asm 00001E51: 	LDI	@TROI,AR2
    // asm 00001E52: 	FLOAT	256,R2
    // asm 00001E53: 	FLOAT	100,R3
    // asm 00001E54: 	LDI	9999,RC
    // asm 00001E55: 	CALL	TEXT_ADD
    // asm 00001E56: 	CALL	SET40FONT
    // asm 00001E57: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001E5A: 	LDI	AR0,AR4			;SAVE POINTERS FOR TEXT CHANGE
    // asm 00001E5B: 	LDI	@ICCI,AR2
    // asm 00001E5C: 	FLOAT	256,R2
    // asm 00001E5D: 	FLOAT	150,R3
    // asm 00001E5E: 	LDI	9999,RC
    // asm 00001E5F: 	CALL	TEXT_ADD
    // asm 00001E60: 	CALL	SET40FONT
    // asm 00001E61: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001E64: 	CREATEC	COIN_CNTDOWN,034h
    // asm 00001E67: 	READAUD	AUD_BCREDITS
    // asm 00001E69: 	LDI	R0,R4			;WATCH CREDITS
    // asm 00001E6A: 	CLRI	R5			;TO START TOGGLE
    // asm 00001E6B: 	LDI	15,AR6			;wait at least 15 frames to continue
    // asm 00001E6C: 	LDI	0,R0
    // asm 00001E6D: 	STI	R0,@miniidle
    // 	;wait for a coin to be dropped in
    // 	;if coins are in change text to PRESS START
INSMORE_LP:
    // asm 00001E6E: 	CALL	INSERT_MORE_COINS
    // asm 00001E6F: 	READAUD	AUD_BCREDITS
    // asm 00001E71: 	CMPI	R0,R4
    // asm 00001E72: 	BEQ	NOINCTIM
    // asm 00001E73: 	LDI	R0,R4
    // asm 00001E74: 	LDI	20,R0
    // asm 00001E75: 	STI	R0,@_countdown
NOINCTIM:
    // asm 00001E76: 	LDI	R5,R5
    // asm 00001E77: 	BNZ	CHECKHIT
    // asm 00001E78: 	READADJ	ADJ_FREE_PLAY
    // asm 00001E7A: 	CMPI	1,R0
    // asm 00001E7B: 	BEQ	FREEP
    // asm 00001E7C: 	READAUD	AUD_CREDITS
    // asm 00001E7E: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm 00001E7F: 	CMPI	R1,R0
    // asm 00001E80: 	BLT	JUSTGOON
    // ;	CMPI	0,R0
    // ;	BEQ	JUSTGOON
FREEP:
    // asm 00001E81: 	LDI	@PSCI,R0		;change the text
    // asm 00001E82: 	STI	R0,*+AR4(TEXT_PTR)
    // asm 00001E83: 	LDI	1,R5
    // asm 00001E84: 	LDI	@BUTTON_STATUS,R0
    // asm 00001E85: 	OR	BUT_START,R0
    // asm 00001E86: 	STI	R0,@BUTTON_STATUS
JUSTGOON:
    // 	;check to see if plyr hit start (decrement count)
    // asm 00001E87: 	LDI	R5,R5
    // asm 00001E88: 	BNZ	CANWT
TOSLP:
    // 	;HITTING THE BUTTON W/O CREDITS
    // asm 00001E89: 	LDI	@START_HIT,R0
    // asm 00001E8A: 	BZ	NRST
    // asm 00001E8B: 	CLRI	R0
    // asm 00001E8C: 	STI	R0,@START_HIT
    // asm 00001E8D: 	LDI	20,R0
    // asm 00001E8E: 	STI	R0,@_countdown
NRST:
    // asm 00001E8F: 	LDI	@miniidle,R0	;only every 10th frame
    // asm 00001E90: 	CMPI	0,R0
    // asm 00001E91: 	LDI	@SWITCHBUTS,R0
    // asm 00001E92: 	LDL	SW_RADIO|SW_VIEW0|SW_VIEW1|SW_VIEW2,R1
    // asm 00001E93: 	AND	R1,R0
    // asm 00001E94: 	BZ	TOSLP2
    // asm 00001E95: 	CMPI	R1,R0		;but if ALL are pressed...dont decrement
    // asm 00001E96: 	BEQ	TOSLP2
    // asm 00001E97: 	LDI	@_countdown,R0
    // asm 00001E98: 	DEC	R0
    // asm 00001E99: 	STI	R0,@_countdown
TOSLP2:
    // asm 00001E9A: 	DEC	AR6
    // asm 00001E9B: 	CMPI	0,AR6
    // asm 00001E9C: 	BGT	CANWT
    // asm 00001E9D: 	LDI	@SWITCHBUTS,R0
    // asm 00001E9E: 	TSTB	SW_START,R0
    // asm 00001E9F: 	BZ	CANWT
    // ;	LDI	AR5,R0		;if no decrement -> DONT
    // ;	BNZ	CANWT
    // asm 00001EA0: 	LDI	@_countdown,R0
    // asm 00001EA1: 	DEC	R0
    // asm 00001EA2: 	STI	R0,@_countdown
    // asm 00001EA3: 	LDI	15,AR6			;wait at least 15 frames to continue
CANWT:
    // asm 00001EA4: 	LDI	@miniidle,R0
    // asm 00001EA5: 	INC	R0
    // asm 00001EA6: 	CMPI	25,R0
    // asm 00001EA7: 	LDIGE	0,R0
    // asm 00001EA8: 	STI	R0,@miniidle
    // asm 00001EA9: 	SLEEP	1
    // asm 00001EAB: 	LDI	@_countdown,R0
    // asm 00001EAC: 	BNZ	INSMORE_LP
    // asm 00001EAD: 	CLRI	AR6
    // asm 00001EAE: 	BU	RETURNTOPLYR
CHECKHIT:
    // asm 00001EAF: 	LDI	@START_HIT,R0
    // asm 00001EB0: 	BZ	TOSLP
    // 	;Secret Button Combo!!
    // 	;
    // 	;if the plyr holds all the view buttons and radio as he
    // 	;hits the start button, he gets to CRUISE the USA
    // 	;-ELP August 11,1994
    // 	;
    // asm 00001EB1: 	LDI	@SWITCHBUTS,R0
    // asm 00001EB2: 	LDIL	(SW_RADIO|SW_VIEW0|SW_VIEW1|SW_VIEW2),R1
    // asm 00001EB5: 	AND	R1,R0
    // asm 00001EB6: 	CMPI	R1,R0
    // asm 00001EB7: 	BNE	NOSECRET_CRUISE
    // asm 00001EB8: 	LDI	RM_USA,R0
    // asm 00001EB9: 	STI	R0,@RACE_MODE
NOSECRET_CRUISE:
    // asm 00001EBA: 	LDI	0,R2
    // asm 00001EBB: 	LDI	AUD_BCREDITS,AR2
    // asm 00001EBC: 	CALL	AUDIT_WRITE
    // asm 00001EBD: 	READAUD	AUD_CREDITS
    // asm 00001EBF: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm 00001EC0: 	SUBI	R1,R0
    // ;	DEC	R0
    // asm 00001EC1: 	LDILT	0,R0
    // asm 00001EC2: 	LDI	R0,R2
    // asm 00001EC3: 	SETAUD	AUD_CREDITS
    // asm 00001EC5: 	LDI	@_MODE,R0
    // asm 00001EC6: 	ANDN	MMODE,R0
    // asm 00001EC7: 	OR	MGAME,R0
    // asm 00001EC8: 	STI	R0,@_MODE
    // asm 00001EC9: 	LDI	1,R0
    // asm 00001ECA: 	STI	R0,@STOPWATCH_CNTL
    // asm 00001ECB: 	LDI	60,R0
    // asm 00001ECC: 	STI	R0,@_countdown
    // asm 00001ECD: 	LDI	1,AR6
    // asm 00001ECE: 	LDI	@BUTTON_STATUS,R0
    // asm 00001ECF: 	ANDN	BUT_START,R0
    // asm 00001ED0: 	STI	R0,@BUTTON_STATUS
    // asm 00001ED1: 	INCAUD	AUD_NUM_BUYINS
RETURNTOPLYR:
    // asm 00001ED3: 	LDI	@SAVEDMODE,R0
    // asm 00001ED4: 	STI	R0,@_MODE
    // asm 00001ED5: 	LDI	034h,R0
    // asm 00001ED6: 	LDI	-1,R1
    // asm 00001ED7: 	CALL	PRC_KILLALL
    // asm 00001ED8: 	CALL	TEXT_INIT
    // asm 00001ED9: 	CALL	RESUME_TUNE_NT
    // asm 00001EDA: 	CLRF	R0
    // asm 00001EDB: 	STPF	R0,@GAME_TIMER
    // asm 00001EDC: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSMORE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PLACE THE COUNTDOWN TIMER
 *
 *
 */
static void COIN_CNTDOWN(void) {
    // asm 00001EE0: 	LDI	@_countdown,R4
COIN_CNTDOWN_LP:
    // asm 00001EE1: 	LDI	@_countdown,R2
    // asm 00001EE2: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm 00001EE3: 	CALL	_itoa
    // asm 00001EE4: 	FLOAT	256,R2
    // asm 00001EE5: 	FLOAT	270,R3
    // asm 00001EE6: 	LDI	1,RC
    // asm 00001EE7: 	CALL	TEXT_ADD
    // asm 00001EE8: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001EEB: 	CALL	SETN43FONT
    // asm 00001EEC: 	LDL	lgnum43_coolyelo,AR2
    // asm 00001EED: 	CALL	PAL_FIND_RAW
    // asm 00001EEE: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00001EEF: 	SLEEP	1
    // asm 00001EF1: 	BU	COIN_CNTDOWN_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN_CNTDOWN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: DIRTY_SHARED	.bss	DIRTY_SHARED,1 */
int DIRTY_SHARED;

void LOAD_SHARED(void) {
    // asm 00001EF2: 	CLRI	R0
    // asm 00001EF3: 	STI	R0,@DIRTY_SHARED
    // asm 00001EF4: 	LDL	_SECshared,AR2
    // asm 00001EF5: 	CALL	LOAD_SECTION_REQ
    // asm 00001EF6: 	LDI	bottom_gtmp_p,R0		;ONE TO OVERWRITE
    // asm 00001EF7: 	LDI	R0,R1			;WHAT TO OVERWRITE IT WITH
    // asm 00001EF8: 	CALL	PAL_OVERWRITE
    // asm 00001EF9: 	LDI	bottom2_gtmp_p,R0
    // asm 00001EFA: 	LDI	R0,R1
    // asm 00001EFB: 	CALL	PAL_OVERWRITE
    // asm 00001EFC: 	LDI	shldr2_gtmp_p,R0
    // asm 00001EFD: 	LDI	R0,R1
    // asm 00001EFE: 	CALL	PAL_OVERWRITE
    // asm 00001EFF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SHARED", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void TRAFFIC_LIGHT(PROC* p) {
    // asm 00001F0F: 	SLEEP	20
    // asm 00001F11: 	LDL	TRAFFIC_LL,AR5
TLT_LP:
    // asm 00001F12: 	LDI	light_p,AR2
    // asm 00001F13: 	CALL	PAL_FIND
    // asm 00001F14: 	BNC	NSSD
    // asm 00001F15: 	BR	SUICIDE
NSSD:
    // asm 00001F16: 	LDI	*AR5++,AR2
    // asm 00001F17: 	CMPI	-1,AR2
    // asm 00001F18: 	BNE	CCC
    // asm 00001F19: 	LDL	TRAFFIC_LL,AR5
    // asm 00001F1A: 	LDI	*AR5++,AR2
CCC:
    // asm 00001F1B: 	LDI	R0,R2
    // asm 00001F1C: 	LDIL	8000000Ah,R3	;16
    // asm 00001F1F: 	CALL	PAL_SET
    // asm 00001F20: 	LDI	*AR5++,AR2
    // asm 00001F21: 	CALL	SLEEP
    // asm 00001F22: 	BU	TLT_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAFFIC_LIGHT", 0, 0);
    UNIMPL();
}

/* asm: TRAFFIC_LL	.word	light_yellowon,10,light_redon,32,light_greenon,32,-1 */
/* asm: 	 */
/* asm: 	 */
static int TRAFFIC_LL[] = {
    light_yellowon,
    10,
    light_redon,
    32,
    light_greenon,
    32,
    -1,
    // ----------------------------------------------------------------------------
};

// *----------------------------------------------------------------------------
void CPOINT_LIGHT(PROC* p) {
    // asm 00001F2A: 	LDI	@RGBTAB_CPI,AR4
    // asm 00001F2B: 	INC	AR4
    // asm 00001F2C: 	LDI	checks_p,AR2
    // asm 00001F2D: 	CALL	PAL_FIND
    // asm 00001F2E: 	BNC	SDASDFA
    // asm 00001F2F: 	BR	SUICIDE
SDASDFA:
    // asm 00001F30: 	LDI	R0,AR6
CPL_LP:
    // asm 00001F31: 	LDI	AR4,AR2
    // asm 00001F32: 	LDI	AR6,R2
    // asm 00001F33: 	ADDI	251,R2
    // asm 00001F34: 	LDI	5,R3
    // asm 00001F35: 	CALL	PAL_SET
    // asm 00001F36: 	LDI	*++AR4(5),R0
    // asm 00001F37: 	BNN	CNT
    // asm 00001F38: 	LDI	@RGBTAB_CPI,AR4
CNT:
    // asm 00001F39: 	LDI	*AR4++,AR2
    // asm 00001F3A: 	CALL	SLEEP
    // asm 00001F3B: 	B	CPL_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CPOINT_LIGHT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: RGBTAB_CPI	.word	RGBTAB_CP */
#define RGBTAB_CPI RGBTAB_CP
/* asm: RGBTAB_CP */
/* asm: .word	4 */
/* asm: RGB	212,212,0 */
/* asm: RGB	212,212,0 */
/* asm: RGB	212,212,0 */
static int RGBTAB_CP[] = {
    4,
    RGB(212, 212, 0),
    RGB(212, 212, 0),
    RGB(212, 212, 0),
    0,
    0,
    2,
    RGB(255, 255, 0),
    RGB(255, 255, 0),
    RGB(255, 255, 0),
    0,
    0,
    4,
    0,
    0,
    0,
    RGB(212, 212, 0),
    RGB(212, 212, 0),
    2,
    0,
    0,
    0,
    RGB(255, 255, 0),
    RGB(255, 255, 0),
    -1,
};
// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *
 */
static void SHOW_RACE_NAME(void) {
    // asm 00001F56: 	LDI	@BONUS_WAVE,AR2
    // asm 00001F57: 	ADDI	@LEG_NAMESI,AR2
    // asm 00001F58: 	LDI	*AR2,AR2
    // asm 00001F59: 	FLOAT	256,R2
    // asm 00001F5A: 	FLOAT	360,R3
    // asm 00001F5B: 	LDI	340,RC
    // asm 00001F5C: 	CALL	TEXT_ADDDS
    // asm 00001F5D: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001F60: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00001F63: 	LDI	AR0,AR4
    // asm 00001F64: 	LDI	AR1,AR5
    // asm 00001F65: 	FLOAT	-100,R6
    // asm 00001F66: 	LDI	20,AR6
SLLP1:
    // asm 00001F67: FLOAT	256,R0
    // asm 00001F68: 	SUBF	R6,R0
    // asm 00001F69: 	MPYF	0.2,R0
    // asm 00001F6A: 	ADDF	R0,R6
    // asm 00001F6B: 	STF	R6,*+AR5(TEXT_POSX)
    // asm 00001F6C: 	LDF	R6,R0
    // asm 00001F6D: 	ADDF	3,R0
    // asm 00001F6E: 	STF	R0,*+AR4(TEXT_POSX)
    // asm 00001F6F: 	SLEEP	1
    // asm 00001F71: 	DBU	AR6,SLLP1
    // 	;CENTER IT
    // 	;
    // asm 00001F72: 	FLOAT	256,R6
    // asm 00001F73: 	STF	R6,*+AR5(TEXT_POSX)
    // asm 00001F74: 	LDF	R6,R0
    // asm 00001F75: 	ADDF	3,R0
    // asm 00001F76: 	STF	R0,*+AR4(TEXT_POSX)
    // asm 00001F77: 	SLEEP	50
    // asm 00001F79: 	LDI	20,AR6
SLLP1A:
    // asm 00001F7A: FLOAT	-100,R0
    // asm 00001F7B: 	SUBF	R6,R0
    // asm 00001F7C: 	MPYF	0.2,R0
    // asm 00001F7D: 	ADDF	R0,R6
    // asm 00001F7E: 	STF	R6,*+AR5(TEXT_POSX)
    // asm 00001F7F: 	LDF	R6,R0
    // asm 00001F80: 	ADDF	3,R0
    // asm 00001F81: 	STF	R0,*+AR4(TEXT_POSX)
    // asm 00001F82: 	SLEEP	1
    // asm 00001F84: 	DBU	AR6,SLLP1A
    // asm 00001F85: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_RACE_NAME", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

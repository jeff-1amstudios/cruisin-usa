#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "delta.h"
#include "error.h"
#include "h2hobj.h"
#include "comm.h"
#include "racer.h"
#include "port.h"
#include "intro.h"

/*
 * Source module: asm/INTRO.ASM
 */

void HEAD2HEAD_LOGO_WAIT(void);
void HEAD2HEAD_LOGO(void);
void KABOSHP(void);
void JINMSG(void);
void THROBIT(void);
void WAIT_FOR_CHALLENGER(void);
void CHECK_ENDBONUS(void);
void WAIT_FOR_ENDBONUS(void);
void ISSUE_STARTGAME_TSEL(void);
void ISSUE_STARTGAME(void);
void PLYR_INTRO(void);
void CHOOSE_NEXT_RACE(void);
void LOAD_NEW_SELECTION(void);
void WATCH_PLYRS_CAR(void);
void INIT_GAMELEG(void);
void CHOOSECAR(void);
void THE_CAR_CHOICE_PROC(void);
void RAISE_DOOR(void);
void ZOOMTOCAR(void);
void GETTHECARS(void);
void SHOW_CAR_STATISTICS(void);
void CLEANUP_DIMCAR_PALS(void);
void GETTHECAR(void);
void ROUNDER(void);
void AFFECT_THE_CARS(void);
void HIDDEN_VEHICLES(void);
void RESET_ORIGINAL(void);
void AFFECTED_CAR(void);
void CAR_DIMMER(void);
void LIGHT_INIT(void);
void LIGHT_OFF(void);
void LIGHT_ON(void);
void INIT_PEDALCHK(void);
void GETCHOICE(void);
void PEDALCHK(void);
void WAVEFLAG(void);
void RACESEL_TIMER(void);
void WAITINTROTIMER(void);
void INTROTIMER(void);
void DIAL_ROUT(void);
void ENDPLAYER(void);
void GAME_AVAILABLEP(void);
void _start(void);
void ULTRA_PROC(void);
void ULTRA_LOGO(void);
void LOGO_SMALL(void);
void SET_ATTR(void);
void _debug(void);
void _timeout(void);
void INSMORE(void);
void COIN_CNTDOWN(void);
void LOAD_SHARED(void);
void TRAFFIC_LIGHT(void);
void CPOINT_LIGHT(void);
void SHOW_RACE_NAME(void);

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
int GAMEDIFF;
/* asm: CHECKPOINT_NUM	.bss	CHECKPOINT_NUM,1 */
int CHECKPOINT_NUM;
/* asm: H2H_FLAGSTATE	.bss	H2H_FLAGSTATE,1 */
int H2H_FLAGSTATE;
const char *JINOW = "JOIN IN NOW";

void HEAD2HEAD_LOGO_WAIT(void)
{
    // asm 0000154D: 	LDI	1,AR6
    // asm 0000154E: 	BU	H2HLE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEAD_LOGO_WAIT", 0, 0);
    UNIMPL();
}

void HEAD2HEAD_LOGO(void)
{
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

/* *RETURNS CARRY SET ON KABOSH THIS MESS
*
*
 */
void KABOSHP(void)
{
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

void JINMSG(void)
{
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

/* *----------------------------------------------------------------------------
 */
void THROBIT(void)
{
    // asm: 	LDI	1,AR5
    // asm 000015F2: THROBLP1
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	SUBF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	CALL	JINMSG
    // asm: 	CALL	KABOSHP
    // asm: 	BC	ENDTHROB
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,THROBLP1
    // asm: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm 000015F3: 	CMPI	1,R0
    // asm 000015F4: 	BNE	NOSND2
    // asm 000015F5: 	SOND1	STAMP
NOSND2:
    // asm 000015F7: 	LDI	4,AR5
KKGKG:
    // asm 000015F8: CALL	JINMSG
    // asm 000015F9: 	CALL	KABOSHP
    // asm 000015FB: 	BC	ENDTHROB
    // asm: 	SLEEP	1
    // asm 000015FC: 	DBU	AR5,KKGKG
    // asm 000015FF: 	LDI	1,AR5
    // asm 00001600: THROBLP2
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm 00001602: 	CALL	JINMSG
    // asm 00001603: 	CALL	KABOSHP
    // asm 00001604: 	BC	ENDTHROB
    // asm 00001605: 	SLEEP	1
    // asm 00001606: 	DBU	AR5,THROBLP2
ENDTHROB:
    // asm: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THROBIT", 0, 0);
    UNIMPL();
}

const char *WFCHAL1 = "WAITING";
const char *WFCHAL2 = "FOR CHALLENGER";
const char *WFCHAL3 = "HOLD RADIO OR VIEW3 TO CANCEL";
/* asm: FRAMELAG	.bss	FRAMELAG,1 */
int FRAMELAG;

/* *
*
 */
void WAIT_FOR_CHALLENGER(void)
{
    // asm: 	LDI	5,R0
    // asm: 	STI	R0,@FRAMELAG
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BOILEROBJ
    // 	;IF NO TRANSMISSIONS ARE HAPPENING
    // 	;(OTHER == SINGLE PLAYER) THEN IGNORE THIS MESS AND ASSUME
    // 	;SINGLE PLAYER
    // 	;
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 00001617: 	BZ	WFCLPXXX
    // asm: 	LDI	@NOASK_LINK,R0
    // asm: 	BNZ	WFCLPXXX
    // asm: 	LDI	@FIRST_RACE,R0
    // asm: 	BNZ	CHAHC
    // asm: 	CALL	BOILERPLATE_INIT
    // asm: 	LDI	AR0,AR2
    // asm 0000161B: 	CALL	OBJ_INSERTP
CHAHC:
    // asm 0000161D: 	CLRI	R0
    // asm 0000161E: 	STI	R0,@START_HIT
    // asm: 	STI	R0,@miniidle
    // asm 0000161F: 	LDI	@_MODE,R0
    // asm 00001620: 	OR	MGO,R0
    // asm: 	STI	R0,@_MODE
    // 	;if no challenger
    // asm 00001623: 	LDI	20,R0
    // asm: 	STI	R0,@_countdown
    // asm 00001624: 	CALL	SEND_VEHICLE
WFCLP:
    // asm 00001627: 	CALL	SEND_VEHICLE
    // asm 00001629: 	LDI	@COINDROP,R0
    // asm: 	BZ	NOWORRY
    // asm: 	CLRI	R0
    // asm 0000162A: 	STI	R0,@COINDROP
    // asm 0000162B: 	LDI	20,R1
    // asm: 	BU	JJFHF
NOWORRY:
    // asm: 	LDI	@_countdown,R1
    // asm: 	LDI	@OM_MODE,R0
    // asm 0000162D: 	AND	MMODE,R0
    // asm: 	CMPI	MINIT,R0
    // asm 0000162E: 	LDIEQ	20,R1
    // asm 0000162F: 	CMPI	MBONUS,R0
    // asm 00001630: 	LDIEQ	20,R1
JJFHF:
    // asm 00001631: STI	R1,@_countdown
    // 	;IF challenger found THEN wait until ready then exit
    // 	;
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001634: 	BZ	BABAB
    // asm 00001636: 	LDI	@OM_VEHICLE,R0
    // asm 00001637: 	BGE	WAITX		;WE'RE THERE DUDE!
    // asm 00001638: BABAB
    // asm 0000163A: 	LDI	@HEAD2HEAD_ON,R0
    // asm 0000163B: 	BNZ	HHFBF
    // asm: 	LDI	@_countdown,R0
    // asm: 	BLE	WFCLPXXX
HHFBF:
    // asm 0000163D: 	LDI	@FRAMELAG,R0
    // asm: 	DEC	R0
    // asm 0000163E: 	LDILT	0,R0
    // asm 0000163F: 	STI	R0,@FRAMELAG
    // asm: 	BGT	NOMSG768
    // asm 00001641: 	LDL	WFCHAL1,AR2
    // asm 00001642: 	FLOAT	256,R2
    // asm 00001643: 	FLOAT	100,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm 00001644: 	CALL	SET40FONT
    // asm 00001645: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001647: 	LDL	WFCHAL2,AR2
    // asm: 	FLOAT	256,R2
    // asm 00001648: 	FLOAT	146,R3
    // asm: 	LDI	1,RC
    // asm 00001649: 	CALL	TEXT_ADD
    // asm 0000164A: 	CALL	SET40FONT
    // asm 0000164B: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOMSG768:
    // 	;if it is a linked game AND he hasn't chosen his vehicle
    // 	;then wait for him to choose his vehicle
    // 	;
    // asm 00001652: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001653: 	BZ	BABAHH
    // asm 00001654: 	SLEEP	1
    // asm 00001655: 	BU	WFCLP
    // asm 00001656: BABAHH
    // *ELP CHANGE
    // ;	LDI	@miniidle,R0
    // ;	INC	R0
    // ;	CMPI	2,R0
    // ;	LDIGE	0,R0
    // ;	STI	R0,@miniidle
    // ;	CMPI	0,R0
    // ;	BNE	JJFJFJ
    // asm 0000165F: 	LDI	@SWITCHBUTS,R0
    // asm: 	RS	16,R0
    // ;	AND	SW_VIEW0_H|SW_VIEW1_H|SW_VIEW2_H|SW_RADIO_H,R0
    // asm: 	AND	SW_VIEW2_H|SW_RADIO_H,R0
    // asm: 	BZ	JJFJFJ
    // asm: 	LDI	@_countdown,R0
    // asm: 	DEC	R0
    // asm: 	LDILT	0,R0
    // asm: 	STI	R0,@_countdown
JJFJFJ:
    // *ELP END CHANGE
    // asm 00001661: 	LDI	@FRAMELAG,R0
    // asm: 	BGT	NOMSG123
    // asm 00001663: 	CALL	WAITINTROTIMER
    // asm 00001664: 	LDL	WFCHAL3,AR2
    // asm 00001665: 	FLOAT	256,R2
    // asm 00001666: 	FLOAT	192,R3
    // asm 00001667: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
NOMSG123:
    // asm: 	SLEEP	1
    // asm 0000166A: 	BU	WFCLP
WFCLPXXX:
    // asm 0000166C: 	CLRI	R0
    // asm 0000166D: 	STI	R0,@MY_LINKWAIT
    // asm 0000166E: 	STI	R0,@HEAD2HEAD_ON
    // asm 00001670: 	CALL	SEND_LINKCANCELLED
    // asm 00001673: 	SLEEP	1
    // asm: 	CALL	SETONE		;1 PLAYER GAME
WAITX:
    // 	;*****
    // 	;*****  WE CAN NO LONGER ACCEPT A DUDE
    // 	;*****
    // 	;*****
    // asm 0000167C: 	LDI	@BOILEROBJ,AR2
    // asm 0000167D: 	CMPI	0,AR2
    // asm 0000167F: 	CALLNE	OBJ_DELETE
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MGO,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	5,R0			;so player doesn't puke (see near call)
    // asm: 	STI	R0,@_countdown
    // asm: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_FOR_CHALLENGER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*an issue start game has been called and
*it has been determined that the other machine
*is either in MBONUS or MINIT.  at that point
*we will wait for the other machine to enter
*MATTR, or MINTRO
*
*
*
 */
void CHECK_ENDBONUS(void)
{
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm: 	BZ	CEBT
    // asm: 	LDI	@OM_BONUS_WAITFLAG,R0
    // asm: 	BNZ	CEBT
    // asm: 	LDI	@OM_LINKWAIT,R0
    // asm: 	BNZ	CEBT
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BNZ	CEBT
    // asm 0000168C: 	LDI	@OM_MODE,R0
    // asm 0000168D: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm 0000168E: 	BEQ	CEBT
    // asm: 	CLRC
    // asm 00001690: 	RETS
CEBT:
    // asm: 	SETC
    // asm 00001692: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_ENDBONUS", 0, 0);
    UNIMPL();
}

/* asm: BONUS_WAITFLAG	pbss	BONUS_WAITFLAG,1 */
int BONUS_WAITFLAG;
/* asm: OM_BONUS_WAITFLAG	pbss	OM_BONUS_WAITFLAG,1 */
int OM_BONUS_WAITFLAG;

void WAIT_FOR_ENDBONUS(void)
{
    // asm 00001697: 	PUSHP	R6
    // asm 00001698: 	SLEEP	2
    // asm 00001699: 	CALL	CHECK_ENDBONUS
    // asm: 	BC	ISGAME5
    // asm: 	POPP	R6
    // asm: 	CMPI	1,R6
    // asm 0000169C: 	BEQ	NOOBJSSS
    // asm 0000169F: 	LDL	H2HPAL2,AR2
    // asm 000016A1: 	CALL	PAL_ALLOC_RAW
    // asm 000016A2: 	LDL	H2HPAL3,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm 000016A3: 	LDL	redhd1,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	-60,R3
    // asm: 	LDI	368,RC
    // asm 000016A6: 	CALL	OBJ_QMAKE
    // asm 000016A7: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm 000016A8: 	STI	R0,*+AR0(OFLAGS)
    // asm 000016A9: 	LDI	AR0,AR2
    // asm 000016AA: 	PUSHP	AR2
    // asm 000016AB: 	CALL	OBJ_INSERTP
    // asm: 	LDI	AR2,AR0
    // asm 000016AD: 	LDL	H2HPAL2,AR2
    // asm 000016AE: 	CALL	PAL_FIND_RAW
    // asm 000016AF: 	STI	R0,*+AR0(OPAL)
    // asm 000016B3: 	LDL	yelhd1,AR2
    // asm 000016B4: 	LDI	0,R2
    // asm 000016B5: 	LDI	60,R3
    // asm 000016B8: 	LDI	368,RC
    // asm 000016B9: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 000016BA: 	OR	*+AR0(OFLAGS),R0
    // asm 000016BB: 	STI	R0,*+AR0(OFLAGS)
    // asm 000016BC: 	LDI	AR0,AR2
    // asm: 	PUSHP	AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	AR2,AR0
    // asm 000016BD: 	LDL	H2HPAL2,AR2
    // asm 000016BE: 	CALL	PAL_FIND_RAW
    // asm 000016BF: 	STI	R0,*+AR0(OPAL)
    // asm 000016C3: 	LDL	big2,AR2
    // asm 000016C4: 	LDI	0,R2
    // asm 000016C5: 	LDI	0,R3
    // asm 000016C6: 	LDI	368,RC
    // asm 000016C9: 	CALL	OBJ_QMAKE
    // asm 000016CA: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 000016CB: 	OR	*+AR0(OFLAGS),R0
    // asm 000016CC: 	STI	R0,*+AR0(OFLAGS)
    // asm 000016CD: 	LDI	AR0,AR2
    // asm: 	PUSHP	AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	AR2,AR0
    // asm 000016CF: 	LDL	H2HPAL3,AR2
    // asm 000016D0: 	CALL	PAL_FIND_RAW
    // asm 000016D1: 	STI	R0,*+AR0(OPAL)
    // asm 000016D2: 	BU	LKJASDFGD
NOOBJSSS:
    // asm 000016D6: 	CALL	BOILERPLATE_INIT
    // asm 000016D7: 	LDI	AR0,AR2
    // asm 000016DA: 	CALL	OBJ_INSERTP
LKJASDFGD:
    // asm: 	PUSHP	R6
WAIT_FOR_ENDBONUS_LP:
    // asm 000016DE: 	LDI	1,R0
    // asm 000016DF: 	STI	R0,@BONUS_WAITFLAG
    // asm: 	LDL	WFCHAL1,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	100,R3
    // asm 000016E0: 	LDI	1,RC
    // asm 000016E1: 	CALL	TEXT_ADD
    // asm 000016E2: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDL	WFCHAL2,AR2
    // asm: 	FLOAT	256,R2
    // asm 000016E6: 	FLOAT	160,R3
    // asm 000016E7: 	LDI	1,RC
    // asm 000016E8: 	CALL	TEXT_ADD
    // asm 000016E9: 	CALL	SET40FONT
    // asm 000016EA: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000016ED: 	CALL	CHECK_ENDBONUS
    // asm 000016EE: 	BC	ISGAME5
    // asm 000016F2: 	SLEEP	1
    // asm 000016F3: 	BU	WAIT_FOR_ENDBONUS_LP
ISGAME5:
    // asm 000016F5: 	CLRI	R0
    // asm 000016F6: 	STI	R0,@BONUS_WAITFLAG
    // asm: 	SLEEP	2
    // asm 000016FA: 	POPP	R6
    // asm 000016FB: 	CMPI	1,R6
    // asm: 	BEQ	NOJHASD
    // asm 000016FC: 	POPP	AR2
    // asm 000016FE: 	CALL	OBJ_DELETE
    // asm: 	POPP	AR2
    // asm 000016FF: 	CALL	OBJ_DELETE
    // asm 00001700: 	POPP	AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDL	H2HPAL2,AR2
    // asm 00001703: 	CALL	PAL_DELETE_RAW
    // asm 00001706: 	LDL	H2HPAL3,AR2
    // asm 00001707: 	CALL	PAL_DELETE_RAW
    // asm: 	BU	IURENDFL
NOJHASD:
    // asm 0000170B: 	LDI	@BOILEROBJ,AR2
    // asm 0000170C: 	CMPI	0,AR2
    // asm 0000170F: 	CALLNE	OBJ_DELETE
IURENDFL:
    // asm 00001713: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_FOR_ENDBONUS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void ISSUE_STARTGAME_TSEL(void)
{
    // asm 00001718: 	LDI	1,R6
    // asm: 	BU	LKAS534
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISSUE_STARTGAME_TSEL", 0, 0);
    UNIMPL();
}

void ISSUE_STARTGAME(void)
{
    // asm 0000171A: 	CLRI	R6
LKAS534:
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 0000171C: 	BZ	NOGAME
    // ;	LDI	@OM_MODE,R0
    // ;	AND	MMODE,R0
    // ;	CMPI	MGAME,R0
    // ;	BEQ	NOGAME
    // *ELP CHANGE
    // asm 00001723: 	LDI	@FIRST_RACE,R0
    // asm 00001724: 	BNZ	NOTNND
    // asm: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm: 	BNZ	NOTNND
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BEQ	NOGAME
    // asm: 	CMPI	MINIT,R0
    // asm: 	BEQ	NOGAME
    // asm: 	CMPI	MINSERT_COINS,R0
    // asm: 	BEQ	NOGAME
NOTNND:
    // 	;IF THE OTHER GAME IS:
    // 	;	 MBONUS
    // 	;	 MINIT
    // 	;THEN
    // 	;	PLACE WAIT FOR CHALLENGER ON SCREEN
    // 	;	IF PLAYER WAITS THEN WAIT FOR OTHER ISSUE START GAME
    // 	;	ELSE IF PLAYER SINGLE THEN SINGLE PLAYER
    // 	;
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BEQ	DOIT6
    // asm: 	CMPI	MINSERT_COINS,R0
    // asm: 	BEQ	DOIT6
    // asm: 	CMPI	MINIT,R0
    // asm: 	BNE	NODOIT6
DOIT6:
    // asm: 	JSRP	WAIT_FOR_ENDBONUS
NODOIT6:
    // asm 00001731: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm 00001732: 	BNZ	DOITANY4
    // asm 00001734: 	LDI	@OM_BONUS_WAITFLAG,R0
    // asm 00001735: 	BNZ	DOITANY4
    // asm 00001737: 	LDI	@_ATTR_MODE,R0
    // asm 00001738: 	CMPI	-7,R0
    // asm: 	BEQ	DOITANY4
    // asm: 	LDI	@OM_MODE,R0
    // asm 0000173F: 	AND	MMODE,R0
    // asm 00001740: 	CMPI	MATTR,R0
    // asm: 	BEQ	DOITANY4
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
    // asm: 	CMPI	MINTRO,R0
    // asm: 	BNE	NOGAME
    // ;	BU	NOGAME
    // *ELP END CHANGE
DOITANY4:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BONUS_WAITFLAG
    // *ELP END CHANGE
    // asm 0000174A: 	CLRI	R0
    // asm 0000174B: 	STI	R0,@LINKEDP
    // asm: 	CALL	SEND_START_GAME
    // asm: 	LDI	30,AR5
WTFORRETVAL:
    // asm: 	DEC	AR5
    // asm 0000174C: 	CMPI	0,AR5
    // asm 0000174D: 	BLE	NTINLK
    // asm: 	SLEEP	1
    // *ELP CHANGE
    // asm 0000174F: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm 00001750: 	BZ	NOGAME
    // *ELP END CHANGE
    // asm: 	LDI	@LINKEDP,R0	;0 = invalid
    // 				;1 = linked
    // 				;2 = not linked
    // asm 00001754: 	BZ	WTFORRETVAL
    // asm: 	CMPI	1,R0
    // asm 00001755: 	BNE	NTINLK
    // asm: 	LDI	1,R0
    // asm 00001757: 	STI	R0,@HEAD2HEAD_ON
    // asm: 	INCAUD	AUD_H2HGAMES
    // asm 00001759: 	CLRI	R0
    // asm: 	STI	R0,@OM_LINKWAIT
    // asm: 	STI	R0,@MY_LINKWAIT
NTINLK:
    // asm: 	CLRI	R0
    // asm 0000175D: 	STI	R0,@NOASK_LINK
    // asm 0000175E: 	RETP
NOGAME:
    // *ELP CHANGE
    // asm: 	CALL	SEND_LINKCANCELLED
    // asm 00001761: 	SLEEP	1
    // *ELP END CHANGE
    // asm 00001763: 	CALL	CLEAR_LINK
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOASK_LINK
    // asm: 	CALL	SETONE
    // asm 00001764: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISSUE_STARTGAME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void PLYR_INTRO(void)
{
    // asm: 	LDI	RM_SINGLE,R0
    // asm 0000176D: 	STI	R0,@RACE_MODE
    // asm 0000176E: 	LDI	MINTRO|MGO,R0
    // asm 0000176F: 	STI	R0,@_MODE
    // asm 00001770: 	CLRI	R0
    // asm 00001771: 	STI	R0,@WAS_HEAD2HEAD_ON
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@FIRST_RACE
    // asm: 	JSRP	ISSUE_STARTGAME
    // asm: 	CALL	INIT_LASTHS_TABLE		;Initialize the table for players hs entries
    // asm 00001775: 	LDI	-1,R2
    // asm 00001776: 	SETADJ	ADJ_INITIALS
    // asm 00001778: 	INCAUD	AUD_GAMENUMBER
    // asm 00001779: 	INCAUD	AUD_GAMES_START
    // asm 0000177B: 	CALL	HSTDEC
    // asm 0000177D: 	CLRI	R0
    // asm: 	STI	R0,@BONUS_WAVE
    // asm 00001784: 	LDF	1.0,R0
    // asm 00001785: 	STF	R0,@GAMEDIFF
    // asm 00001787: 	READAUD	AUD_UNFINISHED_GAMES
    // asm 00001789: 	PUSH	R0
    // asm: 	READAUD	AUD_UNFINISHED_GAMES_FOUND
    // asm 0000178B: 	POP	R1
    // asm: 	CMPI	R1,R0
    // asm: 	BEQ	NOULOG
    // asm 0000178C: 	ERRON	U,EC_UNFINISHED
    // asm 0000178D: 	LDI	R1,R2
    // asm: 	SETAUD	AUD_UNFINISHED_GAMES_FOUND
NOULOG:
    // asm: 	INCAUD	AUD_NUM_UNFINISHED
    // asm 00001792: 	LDI	1,R0				;SHUFFLE DRIVIN
    // asm 00001793: 	STI	R0,@TUNE_IDX
CNR_ENTER:
    // asm 00001797: 	LDP	@IN_RESET_MODE
    // asm 00001798: 	LDI	@IN_RESET_MODE,R0
    // asm 000017A0: 	SETDP
    // asm 000017A1: 	BZ	CONTINUE
    // asm: 	SLEEP	1
    // asm: 	BU	CNR_ENTER
CONTINUE:
    // asm: 	SOND1	START_THEME
    // asm: 	LDI	BUT_START,R0		;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm 000017A8: 	CLRI	R0			;INITIALIZE SCORE
    // asm 000017A9: 	STI	R0,@SCORE
    // asm 000017AA: 	STI	R0,@END_OF_GAMEP	;END OF GAME FLAG
    // asm 000017AB: 	STI	R0,@_MPH
    // asm 000017AD: 	STI	R0,@STARTSECTION	;BEGIN LEG OF JOURNEY
    // asm: 	STI	R0,@CAR_CHOICE_GOTTEN
    // asm 000017AE: 	STI	R0,@CHOSEN_RACE
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@IS_HIDDEN
    // asm 000017B2: 	CALL	GETCMOS_VALUES
    // asm 000017B3: 	CALL	INIT_PEDALCHK
    // asm 000017B4: 	CALL	OBJ_INIT
    // asm 000017B5: 	CALL	TEXT_INIT
    // asm 000017B7: 	JSRP	TRACK_SELECTION
    // asm 000017B8: 	LDI	@FIRST_RACE,R0
    // asm 000017B9: 	BZ	LOAD_NEW_SELECTION
    // asm: 	CALL	INIT_PEDALCHK
    // asm: 	CALL	OBJ_INIT
    // asm 000017BB: 	CALL	TEXT_INIT
    // asm 000017BE: 	CLRI	R0
    // asm: 	STI	R0,@DCALL
    // asm 000017C5: 	JSRP	CHOOSE_TRANSMISSION
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@DCALL
    // asm 000017C7: 	JSRP	CHOOSECAR
    // asm: 	LDI	BUT_VIEW2,R0			;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm: 	STI	R0,@BUTTON_STATUS
WFSNP:
    // asm 000017CB: SLEEP	1
    // asm: 	LDI	@START_NOW_P,R0
    // asm 000017CC: 	BZ	WFSNP
ALL_JOINUP:
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MINFIN,R0
    // asm 000017DA: 	STI	R0,@_MODE
    // asm 000017DC: 	READAUD	ADJ_TIME_TO_START
    // asm 000017DE: 	MPYI	5,R0
    // asm 000017DF: 	ADDI	60,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	CREATEC	WAVEFLAG,UTIL_C|MONKEY_T
    // asm: 	CALL	CLEANUP_TRACKSEL_PALS
    // asm 000017E1: 	LDI	UTIL_C|CHOOSECAR_T,R0
    // asm 000017E2: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // 	;
    // 	;SETUP THE GAME
    // 	;
    // asm 000017E7: 	CLRF	R0
    // asm: 	STF	R0,@GAME_TIMER
    // asm 000017E8: 	INCAUD	AUD_NUM_BUYINS
    // asm 000017EC: 	LDI	0AAh,R0
    // asm 000017ED: 	STI	R0,@BGNDCOLA
    // asm: 	LDI	1,R0	    			;SET GAME FRAME RATE
    // asm: 	STI	R0,@FRAMRATE
    // asm: 	STI	R0,@TIMECLR
    // asm: 	STI	R0,@DRONE_DISPATCH_P
    // asm 000017F0: 	CLRI	R0
    // asm 000017F1: 	STI	R0,@SCREENWIPE_DONE
    // asm: 	STI	R0,@NEXT_STARTUP
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	9,R0
    // asm 000017F5: 	STI	R0,@POSITION
    // asm 000017F8: 	READAUD	ADJ_CHECKPOINT_BONUS
    // asm: 	STI	R0,@CHECKPOINT_TIME_BONUS
    // asm 000017FA: 	LDI	1,R0
    // asm 000017FB: 	STI	R0,@NOAERASE
    // 	;///////////////////////////////
    // asm: 	LDI	@FIRST_RACE,R0
    // asm 000017FD: 	BZ	_PLYR
    // asm: 	CLRI	R2				;FIRST WAVE
    // asm 000017FF: 	SETAUD	AUD_LAST_LEG
    // asm: 	LDF	1.0,R0
    // asm 00001802: 	STF	R0,@WHEELPWR
    // asm: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_INTRO", 0, 0);
    UNIMPL();
}

/* asm: START_NOW_P	.bss	START_NOW_P,1 */
int START_NOW_P;

/* *----------------------------------------------------------------------------
 */
void CHOOSE_NEXT_RACE(void)
{
    // asm: 	CALL	TEXT_INIT
    // asm 00001809: 	LDI	UTIL_C|TEXTP_T,R0
    // asm 0000180A: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	PLYR_C|PLYR1_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MINFIN|MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm 0000180D: 	CALL	OBJ_INIT
    // asm 0000180E: 	CALL	DYNAOBJ_INIT	;init DYNAMIC OBJECTS
    // asm 0000180F: 	CALL	CARB_INIT	;init CAR BLOCKS
    // asm: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    // asm 00001812: 	CLRI	R0
    // asm: 	STI	R0,@FIRST_RACE
    // 	;
    // asm 00001815: 	SLEEP	4
    // 	;
    // asm 00001817: 	BU	CNR_ENTER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOOSE_NEXT_RACE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void LOAD_NEW_SELECTION(void)
{
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	ADDI	@FULLSETUP_TABLEI,AR0
    // asm 0000181C: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm 0000181E: 	LDI	MINTRO|MINFIN,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDF	@START_POS+X,R0
    // asm: 	LDF	@START_POS+Y,R1
    // asm: 	LDF	@START_POS+Z,R2
    // asm 0000181F: 	LDP	@_CAMERAPOS+X
    // asm 00001820: 	STF	R0,@_CAMERAPOS+X
    // asm 00001821: 	STF	R1,@_CAMERAPOS+Y
    // asm 00001822: 	STF	R2,@_CAMERAPOS+Z
    // asm: 	SETDP
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm 00001825: 	ADDI	@RACE_STARTING_POINTSI,AR0
    // asm 00001826: 	LDI	*AR0,R0
    // asm 00001827: 	STI	R0,@STARTSECTION
    // asm: 	CALL	BGD_INIT
    // asm 00001828: 	CALL	INIT_GAMELEG
    // asm 0000182A: 	LDI	MGAME,R0
    // asm 0000182B: 	STI	R0,@_MODE
    // asm 0000182C: 	CALL	SCREENWIPE_OPEN
    // asm: 	SLEEP	6
    // asm 0000182E: 	LDI	MGAME|MHUD|MINFIN,R0
    // asm 0000182F: 	STI	R0,@_MODE
    // asm 00001831: 	LDI	@CHOSEN_RACE,AR0
    // asm 00001832: 	ADDI	@BONUS_POSTLAUNCHI,AR0
    // asm: 	LDI	*AR0,R0
    // asm 00001833: 	CALLU	R0
    // asm 00001835: 	LDI	BUT_VIEW2,R0			;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm 00001836: 	BU	ALL_JOINUP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_NEW_SELECTION", 0, 0);
    UNIMPL();
}

#define PLYPOS2YL (-400)
#define PLYPOS2ZL 2200

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	AR4	PLYRS CAR
*
 */
void WATCH_PLYRS_CAR(void)
{
    // asm: 	LDF	@START_RADY,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR2,R2
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	-PLYPOS2ZL,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	FLOAT	PLYPOS2YL,R0
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm 00001841: 	CALL	MATRIX_MUL
    // asm 00001844: 	LDF	*+AR4(OPOSX),R0
    // asm 00001845: 	LDF	*+AR4(OPOSY),R1
    // asm: 	LDF	*+AR4(OPOSZ),R2
    // asm 00001847: 	ADDF	*+AR2(X),R0
    // asm 00001848: 	ADDF	*+AR2(Y),R1
    // asm 00001849: 	ADDF	*+AR2(Z),R2
    // asm 0000184B: 	LDP	@_CAMERAPOS
    // asm 0000184C: 	STF	R0,@_CAMERAPOS+X
    // asm: 	STF	R1,@_CAMERAPOS+Y
    // asm: 	STF	R2,@_CAMERAPOS+Z
    // asm 0000184D: 	SETDP
    // asm 0000184E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATCH_PLYRS_CAR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void INIT_GAMELEG(void)
{
    // asm: 	CLRI	R0
    // asm 00001853: 	STI	R0,@SMOKE_COUNT
    // asm 00001854: 	STI	R0,@DID_TIMED_OUT
    // asm 00001855: 	STI	R0,@CHECKPOINT_NUM
    // asm 00001856: 	STI	R0,@REAL_CHECKPOINTS
    // asm 00001857: 	STI	R0,@MOTION_STOP_HIT
    // asm 00001858: 	STI	R0,@H2H_FLAGSTATE
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@FRAMRATE
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,@ROADKILL_SOUND_TIMER
    // asm 0000185A: 	CREATE	RHO_DISPATCHER,SPAWNER_C|TRAFFIC_T
    // asm 0000185B: 	CREATE	SIGMA_DISPATCHER,SPAWNER_C|TRAFFIC_T
    // asm 0000185C: 	CREATE	CPOINT_LIGHT,SPAWNER_C|COLORCYC_T
    // asm 0000185D: 	CREATEC	POSITION_FINDER,SPAWNER_C|TRAFFIC_T
    // asm 0000185E: 	CREATE	MOVEIN_HUD_EQUIP,UTIL_C
    // asm: 	LDI	SM_HALT,R0
    // asm 00001862: 	STI	R0,@SUSPEND_MODE
    // asm: 	LDI	0,R4
    // asm 00001864: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001867: 	LDI	1,R4
    // asm 0000186A: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001870: 	LDI	2,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	3,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001874: 	LDI	4,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001875: 	LDI	5,R4
    // asm 00001876: 	CREATE	RACER_DRONE,DRONE_C
    // asm 0000187A: 	LDI	6,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm 0000187D: 	LDI	7,R4
    // asm 0000187E: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	9,R4
    // asm 00001885: 	LDI	@DIPRAM,R0
    // asm 00001886: 	TSTB	DIP_COMMP,R0
    // asm 00001889: 	BNZ	BABA66
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm 0000188D: 	BZ	BABA66
    // asm 00001891: 	LDI	8,R4
    // asm 00001892: BABA66
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm 00001896: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_GAMELEG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CHOOSECAR
*
 */
/* 	;all Y's were 0
	;
 */
/* asm: CCTAB */
/* asm: 	.word	-1384,-164,-4708,cvettem */
/* asm: 	.float	PI */
/* asm: 	.word	0481h */
/* asm: 	.word	-448,-200,-4708,hotrodm */
/* asm: 	.float	PI */
/* asm: 	.word	0482h */
/* asm: 	.word	464,-177,-4708,misslem */
/* asm: 	.float	PI */
/* asm: 	.word	0483h */
/* asm: 	.word	1424,-147,-4708,testorm */
/* asm: 	.float	PI */
/* asm: 	.word	0484h */
/* asm: 	.word	-1 */
int CCTAB[] = {
    -1384, -164, -4708, cvettem,
    PI,
    0x0481,
    -448, -200, -4708, hotrodm,
    PI,
    0x0482,
    464, -177, -4708, misslem,
    PI,
    0x0483,
    1424, -147, -4708, testorm,
    PI,
    0x0484,
    -1,
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
    L_LEG2_BEGIN+1,
    L_LEG3_BEGIN+1,
    L_LEG4_BEGIN+1,
    L_LEG5_BEGIN+1,
    L_LEG6_BEGIN+1,
    L_LEG7_BEGIN+1,
    L_LEG8_BEGIN+1,
    L_LEG9_BEGIN+1,
    L_LEG10_BEGIN+1,
    L_LEG11_BEGIN+1,
    L_LEG12_BEGIN+1,
    L_LEG13_BEGIN+1,
    L_LEG14_BEGIN+1,
};

/* *
*CAMERA POSITION IS ASSUMED TO BE SET BY THE TIME
*THIS ROUTINE IS REACHED.
*
*
 */
void CHOOSECAR(void)
{
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	ADDI	@FULLSETUP_TABLEI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	ADDI	@RACE_STARTING_POINTSI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	STI	R0,@STARTSECTION
    // asm: 	CALL	BGD_INIT
    // asm 000018A6: 	LDI	MINTRO|MINFIN|MWATER|MGO,R0
    // asm 000018A7: 	STI	R0,@_MODE
    // asm 000018A9: 	CLRI	R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm 000018AA: 	STI	R0,@START_HIT
    // asm 000018AC: 	CALL	TEXT_INIT
    // asm 000018AD: 	SOND1	CHOOSEUCAR
    // asm: 	CREATEC	HIDDEN_VEHICLES,UTIL_C|CHOOSECAR_T
    // asm 000018B0: 	LDI	@CCTI,AR2
    // asm: 	FLOAT	256,R2
    // asm 000018B1: 	FLOAT	50,R3
    // asm 000018B2: 	LDI	600,RC
    // asm 000018B3: 	CALL	TEXT_ADD
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000018B4: 	CALL	SET40FONT
JT75:
    // asm 000018B7: LDI	@CAR_CHOICE_GOTTEN,R0
    // asm: 	BNZ	KIBO
    // asm: 	SLEEP	1
    // asm 000018BA: 	BU	JT75
KIBO:
    // asm 000018BD: 	LDL	crace_PALETTES,AR2
    // asm 000018BE: 	CALL	dealloc_section
    // asm 000018C8: 	LDI	@CHOSEN_VEHICLE,AR0
    // asm 000018C9: 	ADDI	@XOFFSETI,AR0
    // asm: 	LDF	*AR0,R6
    // asm: 	FLOAT	576,R1
    // asm: 	SUBRF	R1,R6
    // 	;if a slave then ALWAYS appear on right side
    // 	;
    // 	;
    // asm: 	LDI	@DIPRAM,R0
    // asm 000018CA: 	TSTB	DIP_COMMP,R0
    // asm 000018CB: 	BNZ	BABA
    // asm 000018CD: 	TSTB	CMDP_MASTER,R0
    // asm 000018CE: 	BZ	BABA
    // asm: 	FLOAT	LANESIZE,R1
    // asm: 	ADDF	R1,R6
    // asm 000018CF: BABA
    // asm 000018D0: 	LDF	@START_RADY,R2
    // asm 000018D1: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	HPFIND_YMATRIX
    // asm 000018D2: 	LDI	AR2,R2
    // asm 000018D3: 	CALL	CLR_VECTORA
    // asm: 	STF	R6,*+AR2(X)
    // asm 000018D4: 	LDI	AR2,R3
    // asm 000018D5: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R6
    // asm: 	LDF	*+AR2(Z),R7
    // asm 000018D8: 	LDI	@SINGLE_SECTION_TEMPPTR,R0
NLD:
    // asm 000018D9: LDI	R0,AR0
    // asm 000018DA: 	LDF	*+AR0(OPOSX),R0
    // asm 000018DB: 	ADDF	R6,R0
    // asm 000018DC: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm 000018DE: 	ADDF	R7,R0
    // asm 000018DF: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	*+AR0(OLINK2),R0
    // asm: 	BNZ	NLD
    // asm 000018E1: 	LDI	@CAMERAPOSI,AR2
    // asm 000018E2: 	LDF	*+AR2(X),R0
    // asm 000018E3: 	ADDF	R6,R0
    // asm 000018E4: 	STF	R0,*+AR2(X)
    // asm 000018E5: 	LDF	*+AR2(Z),R0
    // asm 000018E6: 	ADDF	R7,R0
    // asm 000018E7: 	STF	R0,*+AR2(Z)
    // asm 000018E9: 	LDL	CAR_ARRAY,AR3
    // asm: 	LDI	3,AR5
LNNN:
    // asm 000018EA: LDI	*AR3++,AR0
    // asm 000018EB: 	LDF	*+AR0(OPOSX),R0
    // asm 000018EC: 	ADDF	R6,R0
    // asm 000018ED: 	STF	R0,*+AR0(OPOSX)
    // asm 000018EE: 	LDF	*+AR0(OPOSZ),R0
    // asm 000018EF: 	ADDF	R7,R0
    // asm 000018F0: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	DBU	AR5,LNNN
    // asm 000018F3: 	LDI	@_MODE,R0
    // asm 000018F4: 	ANDN	MGO,R0
    // asm 000018F5: 	STI	R0,@_MODE
    // asm 000018F6: 	LDI	5,R0		;WAVEFLAG routine properly sets this
    // asm: 	STI	R0,@_countdown	;we just need to set so PLYR doesnt puke
    // asm 000018F8: 	LDI	100h,R0		;FULL START INDEX
    // asm 000018F9: 	STI	R0,@RACER_GRID_START
    // asm: 	CALL	TEXT_INIT			;ELIMINATE THE 'CHOOSE CAR' TEXT
    // asm 000018FB: 	LDI	@CHOSEN_VEHICLE,R0
    // asm 000018FD: 	LDI	@IS_HIDDEN,R1
    // asm 000018FE: 	CMPI	-1,R1
    // asm 000018FF: 	BEQ	NOHIDE
    // asm 00001900: 	PUSH	R0
    // asm 00001901: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm: 	CMPI	5,AR2
    // asm 00001902: 	LDIEQ	1,AR2
    // asm: 	CMPI	6,AR2
    // asm 00001903: 	LDIEQ	2,AR2
    // asm: 	CMPI	7,AR2
    // asm 00001904: 	LDIEQ	3,AR2
    // asm 00001906: 	ADDI	AUD_CAR_SELECTION+4,AR2
    // asm: 	CALL	AUDIT_INC
    // asm 00001907: 	POP	R0
    // asm: 	ADDI	4,R0
    // asm 00001909: 	BU	TMP886
NOHIDE:
    // asm 0000190C: 	PUSH	R0
    // asm 0000190D: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 0000190E: 	ADDI	AUD_CAR_SELECTION,AR2
    // asm: 	CALL	AUDIT_INC
    // asm 0000190F: 	POP	R0
TMP886:
    // asm 00001911: STI	R0,@CHOOSENCAR
    // asm: 	LDI	UTIL_C|CHOOSECAR_T,R0
    // asm 00001914: 	LDI	-1,R1
    // asm 00001915: 	CALL	PRC_KILLALL
    // asm 00001918: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_COMMP,R0
    // asm 00001919: 	BNZ	JAJAKKA
    // asm: 	JSRP	WAIT_FOR_CHALLENGER
JAJAKKA:
    // asm 0000191A: 	CALL	INIT_GAMELEG
    // asm 0000191C: 	CREATE	DROPTHEWHEEL,UTIL_C|CHOOSECAR_T
    // asm: 	CREATE	DROPTHECYCLE,UTIL_C|CHOOSECAR_T
    // asm: 	CREATE	DROPTHETURN,UTIL_C|CHOOSECAR_T
    // asm 0000191D: 	CREATE	RAISE_DOOR,UTIL_C|CHOOSECAR_T	;in 50
    // 		;(BECOMES PLYR PROC)
    // asm: 	CREATE	ZOOMTOCAR,UTIL_C|CHOOSECAR_T	;in 25 (TRANS.ASM)
    // asm: 	SLEEP	70
    // asm 0000192A: 	SLEEP	1
    // asm 00001930: 	LDI	UTIL_C|CHOOSECAR_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	CALL	CLEANUP_DIMCAR_PALS
    // asm 00001933: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOOSECAR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: CAR_CHOICE_GOTTEN	.bss	CAR_CHOICE_GOTTEN,1 */
int CAR_CHOICE_GOTTEN;

void THE_CAR_CHOICE_PROC(void)
{
    // ;	SLEEP	15
    // asm 0000193D: 	CLRI	R0
    // asm 0000193E: 	STI	R0,@CAR_CHOICE_GOTTEN
    // asm: 	STI	R0,@START_HIT
    // *
    // *CHOOSE CAR LOOP
    // *
    // asm: 	LDI	12,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	CALL	INIT_PEDALCHK
CCLP:
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	CCLPX
    // asm 00001943: 	CALL	GETCHOICE
    // asm 00001944: 	CALL	TRANSCHOICE
    // asm: 	CALL	DIAL_ROUT
    // asm: 	CALL	SHOW_CAR_STATISTICS
    // 	;-------time remaining
    // 	;
    // asm 00001947: 	LDL	time,AR2
    // asm: 	LDI	242,R2			;R2	POS X
    // asm 00001948: 	LDI	337,R3			;R3	POS Y
    // asm 00001949: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
    // asm 0000194C: 	CALL	PEDALCHK
    // asm 0000194D: 	BC	CCLPX
    // asm: 	CALL	INTROTIMER
    // asm: 	SLEEP	1
    // asm 0000194E: 	LDI	@_countdown,R0
    // asm 0000194F: 	BGT	CCLP
CCLPX:
    // *
    // *END CHOOSE CAR LOOP
    // *
    // asm: 	LDI	1,R0
    // asm 00001953: 	STI	R0,@CAR_CHOICE_GOTTEN
    // asm 00001954: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THE_CAR_CHOICE_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CAR IS CHOOSEN, THEN RAISE THE GARAGE DOOR
*
*
*
 */
void RAISE_DOOR(void)
{
    // asm: 	SOND1	GOPEN
    // asm: 	LDI	49,AR5
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm: 	ADDI	501h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	AR0,AR4
    // asm 0000195D: RDLP
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	14,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,RDLP
    // asm 00001960: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAISE_DOOR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void ZOOMTOCAR(void)
{
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_NOW_P
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm: 	ADDI	481h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	AR0,AR4
    // asm: 	LDF	@START_RADY,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	25,AR5
    // asm 0000196D: ZTCLP
    // 	;NOW MOVE TO IN FRONT OF OBJECT
    // asm 00001970: 	LDF	@START_RADY,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm 00001971: 	CALL	FIND_YMATRIX
    // asm 00001972: 	LDI	AR2,R2
    // asm 00001973: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	-PLYPOS2ZL,R0
    // asm 00001974: 	STF	R0,*+AR2(Z)
    // asm: 	FLOAT	PLYPOS2YL,R0
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm 00001975: 	CALL	MATRIX_MUL
    // asm 00001978: 	LDF	*+AR4(OPOSX),R0
    // asm 00001979: 	LDF	*+AR4(OPOSY),R1
    // asm: 	LDF	*+AR4(OPOSZ),R2
    // asm 0000197B: 	ADDF	*+AR2(X),R0
    // asm 0000197C: 	ADDF	*+AR2(Y),R1
    // asm 0000197D: 	ADDF	*+AR2(Z),R2
    // asm 0000197F: 	LDP	@_CAMERAPOS
    // asm 00001980: 	SUBF	@_CAMERAPOS+X,R0
    // asm: 	SUBF	@_CAMERAPOS+Y,R1
    // asm: 	SUBF	@_CAMERAPOS+Z,R2
    // asm 00001981: 	MPYF	0.2,R0
    // asm 00001982: 	MPYF	0.2,R1
    // asm 00001983: 	MPYF	0.2,R2
    // asm: 	ADDF	@_CAMERAPOS+X,R0
    // asm 00001984: 	ADDF	@_CAMERAPOS+Y,R1
    // asm 00001985: 	ADDF	@_CAMERAPOS+Z,R2
    // asm 00001987: 	STF	R0,@_CAMERAPOS+X
    // asm 00001988: 	STF	R1,@_CAMERAPOS+Y
    // asm 00001989: 	STF	R2,@_CAMERAPOS+Z
    // asm 0000198A: 	SETDP
    // 	;TURN CAR TO FACE AWAY (SMOOTH)
    // asm 0000198F: 	LDF	@START_RADY,R2
    // asm 00001990: 	CALL	NORMITS
    // asm: 	LDF	R2,R0
    // asm: 	LDF	*+AR4(ORADY),R2
    // asm 00001991: 	CALL	NORMITS
    // asm 00001992: 	STF	R2,*+AR4(ORADY)
    // asm 00001993: 	CALL	GETTHETADIFF
    // asm: 	ABSF	R0,R1
    // asm: 	CMPF	0.04,R1
    // asm: 	BLT	DOALL1
    // asm 00001995: 	MPYF	0.10,R0
    // asm 00001996: 	ADDF	R0,R2
    // asm 00001997: 	BU	IBO45A
DOALL1:
    // asm 00001999: ADDF	R0,R2
IBO45A:
    // asm 0000199B: STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm 0000199C: 	ADDI	OMATRIX,AR2
    // asm 0000199D: 	CALL	FIND_YMATRIX
    // 	;LOWER CAR TO GROUND
    // asm 000019A0: 	LDF	*+AR4(OPOSY),R0
    // asm 000019A1: 	LDF	*+AR4(OUSR1),R1
    // asm: 	SUBF	R0,R1,R0
    // asm 000019A2: 	MPYF	0.15,R0
    // asm: 	LDF	R0,R1
    // asm 000019A3: 	ADDF	*+AR4(OPOSY),R0
    // asm 000019A4: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm: 	ADDI	401h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm 000019A7: 	LDF	*+AR0(OPOSY),R0
    // asm 000019A8: 	ADDF	R1,R0
    // asm 000019A9: 	STF	R0,*+AR0(OPOSY)
    // asm 000019AB: 	SLEEP	1
    // asm 000019AC: 	DBU	AR5,ZTCLP
    // 	;NOW SET ALL FACING PROPERLY
    // 	;
    // asm 000019B1: 	PUSH	AR4
    // asm 000019B2: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 000019B3: 	ADDI	481h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm 000019B4: 	LDI	AR0,AR4
    // asm 000019B6: 	POP	AR4
    // asm: 	LDI	PLYR_C|PLYR1_T,R0
    // asm: 	STI	R0,*+AR7(PID)
    // *
    // *
    // *
    // *
    // 	;NOW MOVE TO IN FRONT OF OBJECT
    // asm 000019BB: 	LDI	@CAMERAPOSI,AR6
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 000019BD: 	CMPI	4,AR2
    // asm 000019BE: 	BLT	NDO
    // asm: 	SUBI	4,AR2
NDO:
    // asm 000019BF: ADDI	401h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // 	;TURN CAR TO FACE AWAY
    // asm: 	LDF	*+AR4(ORADY),R2
    // asm 000019C0: 	LDF	@START_RADY,R0
    // asm 000019C1: 	CALL	GETTHETADIFF
    // asm 000019C2: 	ADDF	R0,R2
    // asm 000019C3: 	STF	R2,*+AR4(ORADY)
    // asm 000019C4: 	LDI	AR4,AR2
    // asm 000019C5: 	ADDI	OMATRIX,AR2
    // asm 000019C6: 	CALL	FIND_YMATRIX
    // 	;LOWER CAR TO GROUND
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm 000019C8: 	LDF	*+AR4(OUSR1),R1
    // asm 000019C9: 	SUBF	R0,R1,R0
    // asm 000019CA: 	ADDF	*+AR4(OPOSY),R0
    // asm 000019CB: 	STF	R0,*+AR4(OPOSY)
    // asm 000019CE: 	SONDFX	ENGINESTART
    // *
    // *
    // *
    // asm: 	LDI	24,AR5
    // asm 000019D1: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 000019D2: 	CMPI	4,AR2
    // asm 000019D3: 	BLT	NDO2
    // asm 000019D4: 	SUBI	4,AR2
NDO2:
    // asm: 	ADDI	481h,AR2
    // asm 000019D5: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	AR0,AR4
    // asm 000019D7: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_PULL
    // asm 000019D8: 	CALL	PLYR_CAR_INIT
    // asm 000019DA: 	LDF	1,R6
    // asm 000019DB: 	PUSH	AR5
    // asm: 	PUSHFL	R6
    // asm 000019DC: 	LDI	*+AR4(OCARBLK),AR5
    // asm 000019DD: 	STF	R6,*+AR5(CARSPEED)
    // asm: 	LDF	@START_RADY,R6
    // asm: 	STF	R6,*+AR5(CARYROT)
    // asm 000019DF: 	STF	R6,*+AR5(CARVROT)
    // asm 000019E0: 	STF	R6,*+AR5(CARDROT)
    // asm: 	CLRF	R2
    // asm 000019E2: 	CALL	DRONEGO
    // asm 000019E6: 	POPFL	R6
    // asm 000019E7: 	POP	AR5
    // asm: 	CALL	WATCH_PLYRS_CAR
    // asm 000019EA: 	SONDFX	STARTLINEREVS2
    // *
    // *NOW WE MOVE FORWARD AND INTO THE POSITION
    // *OF INSIDE LANE OF ROAD PIECE (OUSR1==0102h)
    // *
    // asm 000019EE: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm 000019F0: 	LDI	*+AR0(OUSR1),R1
    // asm 000019F1: 	ANDN	0FFh,R1
    // asm: 	LDI	*+AR0(OLINK4),AR0
    // asm 000019F2: 	LDI	AR0,AR5
    // 	;MOVE FORWARD LOOP
    // 	;
MOVELOOP:
    // asm 000019F5: 	LDF	*+AR5(OPOSX),R0
    // asm 000019F6: 	SUBF	*+AR4(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm 000019F7: 	LDF	*+AR5(OPOSZ),R1
    // asm 000019F8: 	SUBF	*+AR4(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R0
    // asm: 	LDF	R0,R4
    // asm: 	FLOAT	700,R1
    // 	;if a slave then ALWAYS appear on right side
    // 	;
    // asm 000019FB: 	LDI	@DIPRAM,R3
    // asm 000019FC: 	TSTB	DIP_COMMP,R3
    // asm 000019FD: 	BNZ	BABADUY
    // asm 000019FF: 	TSTB	CMDP_MASTER,R3
    // asm: 	BZ	BABADUY
    // asm 00001A01: 	FLOAT	LANESIZE,R3
    // asm: 	ADDF	R3,R1
    // asm 00001A02: BABADUY
    // asm: 	MPYF	R1,R1
    // asm: 	CMPF	R1,R0
    // asm 00001A02: 	BLE	IBODONE
    // asm: 	PUSH	AR5
    // asm 00001A05: 	LDI	*+AR4(OCARBLK),AR5
    // asm 00001A06: 	LDF	0.5,R0
    // asm: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm 00001A07: 	CLRF	R2
    // asm 00001A08: 	CALL	DRONEGO
    // asm: 	POP	AR5
    // asm 00001A09: 	CALL	WATCH_PLYRS_CAR
    // asm 00001A0A: 	SLEEP	1
    // asm 00001A0B: 	BU	MOVELOOP
IBODONE:
    // asm 00001A0C: 	LDI	@DIPRAM,R3
    // asm 00001A0D: 	TSTB	DIP_COMMP,R3
    // asm 00001A0E: 	BNZ	BABADUY4
    // asm 00001A10: 	LDI	AR5,AR2
    // asm 00001A12: 	TSTB	CMDP_MASTER,R3
    // asm 00001A13: 	LDIZ	572,AR3
    // asm 00001A14: 	LDINZ	1724,AR3
    // asm: 	FLOAT	AR3,R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm 00001A19: 	LDI	0,AR3
    // asm: 	CALL	SPOS_INIT		;INIT STARTING POSITION
    // asm 00001A1A: BABADUY4
    // asm 00001A1C: 	LDI	1,R0
    // asm 00001A1D: 	STI	R0,@START_NOW_P
    // asm 00001A1E: 	LDI	MGAME|MINFIN|MWATER,R0	;NOT MGO NOT MHUD
    // asm 00001A1F: 	STI	R0,@_MODE
    // asm: 	BU	PLYR_INTRO_ENTER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOMTOCAR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: CAR_ARRAY	.bss	CAR_ARRAY,4 */
int CAR_ARRAY[4];

void GETTHECARS(void)
{
    // asm 00001A23: 	LDF	@START_RADY,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm 00001A24: 	CALL	FIND_YMATRIX
    // asm 00001A26: 	LDL	CAR_ARRAY,AR3
    // asm: 	LDI	@CCTABI,AR4
    // asm: 	CLRI	R4
LISTLP:
    // asm 00001A27: CALL	OBJ_GET
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	FLOAT	*AR4++,R1		;GET X POSITION
    // asm: 	STF	R1,*+AR2(X)
    // asm 00001A27: 	FLOAT	*AR4++,R1		;GET Y POSITION
    // asm 00001A28: 	STF	R1,*+AR2(Y)
    // asm 00001A29: 	FLOAT	*AR4++,R1		;GET Z POSITION
    // asm: 	STF	R1,*+AR2(Z)
    // asm 00001A2B: 	LDI	@MATRIXAI,R2
    // asm 00001A2C: 	LDI	AR2,R3
    // asm 00001A2D: 	CALL	MATRIX_MUL
    // asm 00001A2F: 	LDF	*+AR2(X),R0
    // asm 00001A30: 	ADDF	@START_POS+X,R0
    // asm 00001A31: 	STF	R0,*+AR0(OPOSX)
    // asm 00001A32: 	LDF	*+AR2(Y),R0
    // asm 00001A33: 	ADDF	@START_POS+Y,R0
    // asm 00001A34: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR2(Z),R0
    // asm 00001A35: 	ADDF	@START_POS+Z,R0
    // asm 00001A36: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	*AR4++,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm 00001A38: 	STI	AR0,*AR3++
    // asm 00001A3A: 	LDI	AR0,AR2
    // asm 00001A3B: 	CALL	OBJ_INSERT
    // asm 00001A3D: 	LDI	AR7,AR1
    // asm 00001A3E: 	ADDI	PDATA,AR1
    // asm 00001A3F: 	ADDI	R4,AR1
    // asm 00001A40: 	STI	AR0,*AR1
    // asm: 	INC	R4
    // asm 00001A42: 	LDF	*AR4++,R2
    // asm 00001A43: 	ADDF	@START_RADY,R2
    // asm: 	LDI	AR0,AR2
    // asm 00001A44: 	ADDI	OMATRIX,AR2
    // asm 00001A45: 	STF	R2,*+AR0(ORADY)
    // asm: 	CALL	FIND_YMATRIX
    // asm 00001A47: 	LDI	*AR4++,R0
    // asm 00001A48: 	STI	R0,*+AR0(OID)
    // asm 00001A4A: 	LDI	*AR4,R0
    // asm: 	CMPI	-1,R0
    // asm 00001A4B: 	BNE	LISTLP
    // asm 00001A4C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTHECARS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SHOW CAR STATISTICS
*
*USES
*	AR4,AR5,AR6,R4,R5
*
 */
/* asm: SCS_TAB	.float	10,70,170,230 */
float SCS_TAB[] = {
    10.0f, 70.0f, 170.0f, 230.0f,
};

void SHOW_CAR_STATISTICS(void)
{
    // asm: 	LDI	@DCALL,R0
    // asm: 	RETSZ
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	LDI	@CHOSEN_VEHICLE,AR7
    // asm: 	ADDI	@SCS_TABI,AR7
    // asm: 	LDL	TITLES,AR5
    // asm 00001A5C: 	LDI	5-1,AR3
GBERLP:
    // asm 00001A5D: LDI	*AR5++,AR2
    // asm 00001A5E: 	LDF	*AR7,R2
    // asm 00001A5F: 	FLOAT	180,R3
    // asm 00001A60: 	FLOAT	AR3,R0
    // asm 00001A61: 	MPYF	10,R0
    // asm 00001A62: 	SUBF	R0,R3
    // asm: 	LDI	1,RC
    // asm 00001A63: 	CALL	TEXT_ADDDS
    // asm 00001A64: 	CALL	SETFIXEDFONTDS
    // asm: 	DBU	AR3,GBERLP
    // asm 00001A67: 	LDI	@CHOSEN_VEHICLE,AR5
    // asm 00001A68: 	ADDI	@TEXTTABSI,AR5
    // asm 00001A69: 	LDI	*AR5,AR5
    // asm 00001A6C: 	LDI	5,AR3
    // asm 00001A6D: GNNERLP
    // asm 00001A6E: 	LDI	*AR5++,AR2
    // asm 00001A6F: 	LDF	*AR7,R2
    // asm 00001A70: 	ADDF	100,R2
    // asm: 	CMPI	6,AR3
    // asm: 	LDFEQ	*AR7,R2
    // asm 00001A72: 	FLOAT	180,R3
    // asm 00001A73: 	FLOAT	AR3,R0
    // asm: 	MPYF	10,R0
    // asm: 	SUBF	R0,R3
    // asm 00001A74: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm 00001A75: 	CALL	SETFIXEDFONTDS
    // asm 00001A76: 	DBU	AR3,GNNERLP
    // asm 00001A78: 	POP	AR7
    // asm 00001A79: 	POP	AR6
    // asm: 	POP	AR5
    // asm 00001A7A: 	POP	AR4
    // asm 00001A7B: 	POP	AR3
    // asm 00001A7C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_CAR_STATISTICS", 0, 0);
    UNIMPL();
}

/* asm: CAR1PAL	.bss	CAR1PAL,129 */
int CAR1PAL[129];
/* asm: CAR2PAL	.bss	CAR2PAL,129 */
int CAR2PAL[129];
/* asm: CAR3PAL	.bss	CAR3PAL,129 */
int CAR3PAL[129];
/* asm: CAR4PAL	.bss	CAR4PAL,129 */
int CAR4PAL[129];
/* asm: CARPAL_TABLE	.word	CAR1PAL,CAR2PAL,CAR3PAL,CAR4PAL */
int *CARPAL_TABLE[] = {
    CAR1PAL, CAR2PAL, CAR3PAL, CAR4PAL,
};
/* asm: CARSRCPAL_TAB	.word	cvette_p,hotrod_p,missle_p,testor_p */
int CARSRCPAL_TAB[] = {
    cvette_p, hotrod_p, missle_p, testor_p,
};

/* *----------------------------------------------------------------------------
*
*(AND OVERHEAD LIGHT CLEANUP)
*
*
 */
void CLEANUP_DIMCAR_PALS(void)
{
    // asm: 	LDL	flour_lghtof,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDL	CAR1PAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDL	CAR2PAL,AR2
    // asm 00001A8A: 	CALL	PAL_FIND_RAW
    // asm 00001A8B: 	LDI	R0,AR2
    // asm 00001A8C: 	CALL	PAL_DELETE_RAW
    // asm: 	LDL	CAR3PAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm 00001A8E: 	LDI	R0,AR2
    // asm 00001A8F: 	CALL	PAL_DELETE_RAW
    // asm 00001A91: 	LDL	CAR4PAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm 00001A92: 	LDI	R0,AR2
    // asm 00001A93: 	CALL	PAL_DELETE_RAW
    // asm 00001A94: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEANUP_DIMCAR_PALS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GENERAL PURPOSE CHOOSE CAR LOOP ROUTINE
*
*
	;THE CAR
 */
#define RNDR_C1_DYH (PDATA+1) //desired Y height
#define RNDR_C1_SYH (PDATA+2) //starting y height
#define RNDR_C2_DYH (PDATA+3)
#define RNDR_C2_SYH (PDATA+4)
#define RNDR_C3_DYH (PDATA+5)
#define RNDR_C3_SYH (PDATA+6)
#define RNDR_C4_DYH (PDATA+7)
#define RNDR_C4_SYH (PDATA+8)
/* 	;THE LIFT
 */
#define RNDR_L1_DYH (PDATA+9) //desired Y height
#define RNDR_L1_SYH (PDATA+10) //starting y height
#define RNDR_L2_DYH (PDATA+11)
#define RNDR_L2_SYH (PDATA+12)
#define RNDR_L3_DYH (PDATA+13)
#define RNDR_L3_SYH (PDATA+14)
#define RNDR_L4_DYH (PDATA+15)
#define RNDR_L4_SYH (PDATA+16)
#define CDYH 0
#define CSYH 1
#define C_SIZE 2
#define C_STRT (PDATA+1)
#define C_OFF2LIFT 8

/* *----------------------------------------------------------------------------
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
void GETTHECAR(void)
{
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDI	R0,R6
    // asm: 	LDI	AR3,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	BNC	$
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	STI	R6,*+AR0(OPAL)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm 00001A9F: 	STF	R0,*+AR7(IR0)
    // asm 00001AA0: 	INC	IR0
    // asm 00001AA1: 	STF	R0,*+AR7(IR0)
    // asm 00001AA2: 	STF	R0,*+AR0(OUSR1)
    // asm 00001AA3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTHECAR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
 */
void ROUNDER(void)
{
    // asm 00001AAC: 	LDL	CAR1PAL,AR2
    // asm 00001AAD: 	LDI	481h,AR3
    // asm 00001AAE: 	LDI	RNDR_C1_DYH,IR0
    // asm 00001AAF: 	CALL	GETTHECAR
    // asm: 	LDL	CAR2PAL,AR2
    // asm: 	LDI	482h,AR3
    // asm: 	LDI	RNDR_C2_DYH,IR0
    // asm: 	CALL	GETTHECAR
    // asm: 	LDL	CAR3PAL,AR2
    // asm: 	LDI	483h,AR3
    // asm 00001AB0: 	LDI	RNDR_C3_DYH,IR0
    // asm 00001AB1: 	CALL	GETTHECAR
    // asm 00001AB3: 	LDL	CAR4PAL,AR2
    // asm: 	LDI	484h,AR3
    // asm 00001AB4: 	LDI	RNDR_C4_DYH,IR0
    // asm 00001AB5: 	CALL	GETTHECAR
    // asm: 	LDI	401h,AR2
    // asm 00001AB8: 	CALL	OBJ_FIND_FIRST
    // asm 00001AB9: 	LDF	*+AR0(OPOSY),R0
    // asm 00001ABA: 	STF	R0,*+AR7(RNDR_L1_DYH)
    // asm 00001ABB: 	STF	R0,*+AR7(RNDR_L1_SYH)
    // asm 00001ABC: 	LDI	402h,AR2
    // asm 00001ABD: 	CALL	OBJ_FIND_FIRST
    // asm 00001ABE: 	LDF	*+AR0(OPOSY),R0
    // asm 00001ABF: 	STF	R0,*+AR7(RNDR_L2_DYH)
    // asm: 	STF	R0,*+AR7(RNDR_L2_SYH)
    // asm 00001AC0: 	LDI	403h,AR2
    // asm 00001AC1: 	CALL	OBJ_FIND_FIRST
    // asm 00001AC2: 	LDF	*+AR0(OPOSY),R0
    // asm 00001AC3: 	STF	R0,*+AR7(RNDR_L3_DYH)
    // asm 00001AC4: 	STF	R0,*+AR7(RNDR_L3_SYH)
    // asm 00001AC5: 	LDI	404h,AR2
    // asm 00001AC6: 	CALL	OBJ_FIND_FIRST
    // asm 00001AC7: 	LDF	*+AR0(OPOSY),R0
    // asm 00001AC8: 	STF	R0,*+AR7(RNDR_L4_DYH)
    // asm 00001AC9: 	STF	R0,*+AR7(RNDR_L4_SYH)
    // asm 00001ACD: 	LDI	0,AR0
    // asm 00001ACE: 	LDF	0.5,R0
    // asm: 	CALL	CAR_DIMMER
    // asm 00001AD0: 	LDI	1,AR0
    // asm 00001AD1: 	LDF	0.5,R0
    // asm 00001AD2: 	CALL	CAR_DIMMER
    // asm: 	LDI	2,AR0
    // asm: 	LDF	0.5,R0
    // asm: 	CALL	CAR_DIMMER
    // asm 00001AD4: 	LDI	3,AR0
    // asm 00001AD5: 	LDF	0.5,R0
    // asm 00001AD6: 	CALL	CAR_DIMMER
    // asm 00001AD7: 	CALL	LIGHT_INIT
    // asm: 	LDI	@CHOSEN_VEHICLE,R0
    // asm 00001ADA: 	STI	R0,@SPINCURR
    // asm 00001ADC: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(PDATA)
ROUNDERLP:
    // asm 00001ADE: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 00001ADF: 	LDI	*+AR7(PDATA),R0
    // asm: 	CMPI	R0,AR2
    // asm 00001AE0: 	BEQ	RLL
    // asm: 	STI	AR2,*+AR7(PDATA)
    // asm: 	LDI	@DCALL,R1
    // asm 00001AE3: 	BZ	NODO56
    // asm: 	PUSH	AR2
    // asm 00001AE5: 	PUSH	R0
    // asm 00001AE6: 	SONDFX	HYDRO
    // asm 00001AE7: 	POP	R0
    // asm 00001AE8: 	POP	AR2
NODO56:
    // asm: 	LDI	R0,IR0
    // asm: 	MPYI	C_SIZE,IR0
    // asm 00001AEA: 	ADDI	C_STRT,IR0
    // asm 00001AEB: 	INC	IR0
    // asm: 	LDF	*+AR7(IR0),R1
    // asm 00001AEC: 	DEC	IR0
    // asm 00001AED: 	STF	R1,*+AR7(IR0)
    // asm 00001AF1: 	PUSH	AR2
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm 00001AF2: 	POP	AR2
    // asm 00001AF3: 	LDI	R0,AR0
    // asm 00001AF4: 	LDF	0.5,R0
    // asm 00001AF5: 	CALL	CAR_DIMMER
    // asm 00001AF8: 	LDI	AR2,IR0
    // asm: 	MPYI	C_SIZE,IR0
    // asm: 	ADDI	C_STRT,IR0
    // asm 00001AF9: 	INC	IR0
    // asm 00001AFA: 	LDF	*+AR7(IR0),R0
    // asm 00001AFB: 	DEC	IR0
    // asm 00001AFC: 	FLOAT	300,R1
    // asm 00001AFD: 	SUBF	R1,R0
    // asm 00001AFE: 	STF	R0,*+AR7(IR0)
    // asm: 	PUSH	AR2
    // asm 00001B00: 	CALL	LIGHT_ON
    // asm 00001B01: 	POP	AR2
    // asm 00001B02: 	LDI	AR2,AR0
    // asm 00001B03: 	LDF	1.0,R0
    // asm 00001B04: 	CALL	CAR_DIMMER
RLL:
    // asm 00001B07: 	CALL	AFFECT_THE_CARS
    // asm: 	SLEEP	1
    // asm: 	BU	ROUNDERLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROUNDER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void AFFECT_THE_CARS(void)
{
    // asm: 	LDI	481h,AR2
    // asm: 	LDI	RNDR_C1_DYH,IR0
    // asm: 	LDI	0,IR1
    // asm: 	CALL	AFFECTED_CAR
    // asm: 	LDI	482h,AR2
    // asm: 	LDI	RNDR_C2_DYH,IR0
    // asm: 	LDI	1,IR1
    // asm: 	CALL	AFFECTED_CAR
    // asm: 	LDI	483h,AR2
    // asm: 	LDI	RNDR_C3_DYH,IR0
    // asm 00001B13: 	LDI	2,IR1
    // asm 00001B14: 	CALL	AFFECTED_CAR
    // asm 00001B16: 	LDI	484h,AR2
    // asm: 	LDI	RNDR_C4_DYH,IR0
    // asm 00001B17: 	LDI	3,IR1
    // asm 00001B18: 	CALL	AFFECTED_CAR
    // asm 00001B19: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AFFECT_THE_CARS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*HIDDEN VEHICLES
*
*
*
 */
/* asm: IS_HIDDEN	.bss	IS_HIDDEN,1 */
int IS_HIDDEN;
/* asm: HIDDEN_TABLE	.word	jeepm,sbuspm,copcar,gtruck */
int HIDDEN_TABLE[] = {
    jeepm, sbuspm, copcar, gtruck,
};

void HIDDEN_VEHICLES(void)
{
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@IS_HIDDEN
HIDDEN_VEHICLES_LP:
    // asm: 	SLEEP	1
    // asm: 	LDI	@CAR_CHOICE_GOTTEN,R0
    // asm: 	BZ	NSCD
    // asm: 	DIE
NSCD:
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm 00001B29: 	RS	16,R0
    // asm 00001B2A: 	AND	SW_VIEW0_H|SW_VIEW1_H,R0
    // asm: 	BZ	SETAS_ORIGINALS
    // asm 00001B2B: SETAS_HIDDEN
    // asm: 	LDI	@IS_HIDDEN,R0
    // asm 00001B2D: 	CMPI	-1,R0
    // asm 00001B2E: 	CALLNE	RESET_ORIGINAL
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // ;	CMPI	1,AR2
    // ;	BEQ	HIDDEN_VEHICLES_LP
    // asm 00001B32: 	CMPI	3,AR2
    // asm 00001B33: 	BEQ	HIDDEN_VEHICLES_LP
    // asm: 	STI	AR2,@IS_HIDDEN
    // asm 00001B35: 	ADDI	481h,AR2
    // asm 00001B36: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm 00001B37: 	ADDI	@HIDDEN_TABLEI,AR2
    // asm: 	LDI	*AR2,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm 00001B38: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001B39: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	BU	HIDDEN_VEHICLES_LP
    // asm 00001B3B: SETAS_ORIGINALS
    // asm: 	LDI	@IS_HIDDEN,R0
    // asm 00001B3D: 	CMPI	-1,R0
    // asm 00001B3E: 	BEQ	HIDDEN_VEHICLES_LP	;no one was hidden,
    // 					;just ignore
    // asm 00001B40: 	CALL	RESET_ORIGINAL
    // asm 00001B41: 	LDI	-1,R0
    // asm 00001B42: 	STI	R0,@IS_HIDDEN
    // asm 00001B43: 	BU	HIDDEN_VEHICLES_LP
    // *PARAMETERS
    // *	R0	INDEX OF PREVIOUS VEHICLES
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIDDEN_VEHICLES", 0, 0);
    UNIMPL();
}

void RESET_ORIGINAL(void)
{
    // 	;insert code here to set the old vehicle
    // 	;as
    // 	;
    // asm 00001B49: 	PUSH	R0
    // asm 00001B4A: 	LDI	R0,AR2
    // asm 00001B4B: 	ADDI	481h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	POP	AR2
    // asm: 	MPYI	6,AR2
    // asm: 	ADDI	@CCTABI,AR2
    // asm: 	LDI	*+AR2(3),R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001B4C: 	OR	O_1PAL,R0
    // asm 00001B4D: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001B4E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_ORIGINAL", 0, 0);
    UNIMPL();
}

/* asm: SPINCURR	.bss	SPINCURR,1 */
int SPINCURR;

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	AR2	ID
*	IR0	DESIRED HEIGHT OFFSET of AR7
*	IR1	IDX OF CAR
*
*
 */
void AFFECTED_CAR(void)
{
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	LDF	*+AR7(IR0),R1
    // asm: 	SUBF	R0,R1,R0
    // asm: 	MPYF	0.15,R0
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	PUSH	IR0
    // asm: 	PUSH	AR0
    // asm: 	LDI	AR0,AR1
    // asm: 	SUBI	080h,AR2
    // asm 00001B59: 	CALL	OBJ_FIND_FIRST
    // asm 00001B5A: 	LDF	*+AR1(OPOSY),R0
    // asm 00001B5B: 	INC	IR0
    // asm 00001B5C: 	SUBF	*+AR7(IR0),R0
    // asm 00001B5D: 	ADDI	C_OFF2LIFT,IR0
    // asm 00001B5E: 	ADDF	*+AR7(IR0),R0
    // asm 00001B5F: 	STF	R0,*+AR0(OPOSY)
    // asm: 	POP	AR0
    // asm 00001B60: 	POP	IR0
    // asm 00001B63: 	LDI	@SPINCURR,R0
    // asm 00001B64: 	LDI	@CHOSEN_VEHICLE,R1
    // asm 00001B65: 	CMPI	R0,R1
    // asm 00001B66: 	BEQ	IBOIBO
    // asm 00001B68: 	CMPI	R0,IR1
    // asm 00001B69: 	BNE	N12
    // 	;track to PI
    // asm 00001B6C: 	LDF	*+AR0(ORADY),R2
    // asm: 	LDLF	0.392699,R1
    // asm: 	ADDF	R1,R2
    // asm 00001B6D: 	CALL	NORMITS
    // asm 00001B6E: 	LDF	PI,R0
    // asm 00001B6F: 	ADDF	@START_RADY,R0
    // asm 00001B70: 	PUSHFL	R2
    // asm: 	LDF	R0,R2
    // asm 00001B71: 	CALL	NORMITS
    // asm 00001B72: 	LDF	R2,R0
    // asm: 	POPFL	R2
    // asm: 	CALL	GETTHETADIFF
    // asm 00001B75: 	PUSHFL	R2
    // asm 00001B76: 	LDF	R0,R2
    // asm 00001B77: 	CALL	NORMITS
    // asm 00001B78: 	LDF	R2,R0
    // asm 00001B79: 	POPFL	R2
    // asm 00001B7C: 	CMPF	0.04,R0
    // asm 00001B7D: 	BLT	DOALL
    // asm 00001B80: 	MPYF	0.10,R0
    // asm: 	ADDF	R0,R2
    // asm: 	BU	IBO45
DOALL:
    // asm 00001B83: ADDF	R0,R2
    // asm 00001B84: 	LDI	@CHOSEN_VEHICLE,R1
    // asm 00001B85: 	STI	R1,@SPINCURR
IBO45:
    // asm 00001B88: STF	R2,*+AR0(ORADY)
    // asm 00001B88: 	LDI	AR0,AR2
    // asm 00001B89: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
IBOIBO:
    // asm 00001B8B: CMPI	R1,IR1
    // asm 00001B8C: 	BNE	N12
    // 	;just spin
    // asm 00001B8E: 	LDF	*+AR0(ORADY),R2
    // asm 00001B8F: 	ADDF	0.1,R2
    // asm: 	STF	R2,*+AR0(ORADY)
    // asm 00001B90: 	LDI	AR0,AR2
    // asm 00001B91: 	ADDI	OMATRIX,AR2
    // asm 00001B92: 	CALL	FIND_YMATRIX
N12:
    // asm 00001B94: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AFFECTED_CAR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	AR0	CAR INDEX
*	R0	(FL) DIMMER VALUE
*
 */
void CAR_DIMMER(void)
{
    // asm 00001B9C: 	LDI	AR0,AR1
    // asm: 	ADDI	@CARPAL_TABLEI,AR1
    // asm: 	LDI	*AR1,AR1		;NOW HOLDS RAM LOCATION
    // asm: 	ADDI	@CARSRCPAL_TABI,AR0
    // asm: 	LDI	*AR0,AR0
    // asm: 	ADDI	@PALROMI,AR0
    // asm: 	LDI	*AR0,AR0
    // asm: 	CALL	PAL_DIMMER
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAR_DIMMER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void LIGHT_INIT(void)
{
    // asm 00001BA0: 	PUSH	AR2
    // asm 00001BA1: 	LDL	flour_lghtof,AR2
    // asm 00001BA2: 	CALL	PAL_ALLOC_RAW
    // asm 00001BA4: 	LDI	0,AR2
    // asm 00001BA5: 	CALL	LIGHT_OFF
    // asm: 	LDI	1,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm: 	LDI	2,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm: 	LDI	3,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm 00001C27: 	POP	AR2
    // asm 00001C28: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	AR2	INDEX  (0 to 3)
*
 */
void LIGHT_OFF(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	ADDI	601h,AR2
    // asm: 	PUSH	AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDL	flour_lghtof,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001C35: 	POP	AR2
    // asm 00001C36: 	SUBI	200h,AR2
    // asm 00001C37: 	CALL	OBJ_FIND_FIRST
    // asm 00001C38: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001C39: 	ANDN	O_1PAL,R0
    // asm 00001C3A: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001C3C: 	POP	AR2
    // asm 00001C3D: 	POP	AR0
    // asm 00001C3E: 	POP	R0
    // asm 00001C3F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_OFF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*as above
*
 */
void LIGHT_ON(void)
{
    // asm 00001C46: 	PUSH	R0
    // asm 00001C47: 	PUSH	AR0
    // asm 00001C48: 	PUSH	AR2
    // asm 00001C49: 	ADDI	601h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	SUBI	200h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm 00001C4A: 	ANDN	O_1PAL,R0
    // asm 00001C4B: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001C4D: 	POP	AR2
    // asm 00001C4E: 	POP	AR0
    // asm 00001C4F: 	POP	R0
    // asm 00001C50: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_ON", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*USES
*	R5	AS A DEDICATED REG
*
 */
void INIT_PEDALCHK(void)
{
    // asm 00001C57: 	CLRI	R5				;FLAG : HAS THE PEDAL BEEN RELEASEDP
    // asm 00001C58: 	LDI	@_pot1,R0
    // asm 00001C59: 	FIX	@PEDALMN,R1
    // asm 00001C5A: 	ADDI	20,R1
    // asm: 	CMPI	R1,R0
    // asm: 	LDIGE	1,R5				;GE -> IT HASN'T
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_PEDALCHK", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GETCHOICE(void)
{
    // asm 00001C5B: 	PUSHF	R4
    // asm 00001C5C: 	PUSH	R4
    // asm 00001C5D: 	LDI	@_MODE,R4
    // asm 00001C5E: 	AND	MMODE,R4
    // asm 00001C5F: 	FLOAT	@_pot0,R3 		;GET CURRENT WHEEL
    // asm 00001C60: 	FLOAT	@POSES,R1		;GET # POSES
    // asm: 	LDF	@STEERFR,R0
    // asm: 	CALL	DIV_F
    // asm: 	LDF	R0,R1
    // asm: 	MPYF	0.5,R1
    // asm 00001C63: 	FLOAT	@POSE,R2		;@CHOSEN_VEHICLE,R2
    // asm 00001C64: 	MPYF	R0,R2
    // asm 00001C65: 	ADDF	R1,R2	 		;MIDDLE OF ZONE
    // asm 00001C66: 	ADDF	@STEERMN,R2		;ADD IN MINIMUM
    // asm 00001C67: 	CMPI	MINIT,R4
    // asm: 	BEQ	GETCHA
    // asm 00001C68: 	STF	R2,@WHEELPOS
GETCHA:
    // asm: 	SUBF	R3,R2
    // asm 00001C6B: 	ABSF	R2,R3			;FIND DIFFERENCE
    // asm: 	MPYF	0.6,R0		   	;SLIGHT HYSTERESIS
    // asm 00001C6C: 	CMPF	R0,R3
    // asm 00001C6D: 	BLE	GETCHX
    // asm 00001C6F: 	LDF	R2,R2
    // asm 00001C70: 	LDILT	1,R0
    // asm 00001C71: 	LDIGE	-1,R0
    // asm: 	ADDI	@POSE,R0		;CHOSEN_VEHICLE,R0
    // asm 00001C73: 	LDFLT	0,R0
    // asm: 	CMPI	@POSES,R0
    // asm 00001C74: 	BLT	GETCH1
    // asm 00001C75: 	LDI	@POSES,R0
    // asm 00001C76: 	SUBI	1,R0
GETCH1:
    // asm 00001C77: STI	R0,@POSE
GETCHX:
    // asm 00001C78: 	POP	R4
    // asm 00001C79: 	POPF	R4
    // asm 00001C7A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCHOICE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*USES
*	R5	see above
*RETURNS
*	NC	FALSE
*	C	TRUE
*
 */
void PEDALCHK(void)
{
    // asm: 	LDI	@_pot1,R0			;set in main IRQ
    // asm: 	LDP	@PEDALMN
    // asm: 	FIX	@PEDALMN,R1
    // asm: 	ADDI	20,R1
    // asm: 	CMPI	R1,R0
    // asm: 	LDILT	0,R5
    // asm: 	BLT	SKIPKEY
    // asm: 	LDI	R5,R5				;IF the pedal has not yet been released
    // asm: 	BZ	PEDALTRUE			;up do not accept this as a valid pedal choice
SKIPKEY:
    // asm 00001C85: PEDALFALSE
    // asm 00001C86: 	CLRC
    // asm 00001C87: 	RETS
PEDALTRUE:
    // asm 00001C8A: 	SETC
    // asm 00001C8B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PEDALCHK", 0, 0);
    UNIMPL();
}

const char *T_READY = "READY";
const char *T_SET = "SET";
const char *T_GO = "GO";
const char *T_CHALLENG = "CHALLENGE RACE";
/* asm: TLIST	.word	T_READY,CHICK_READY,SEND_WAVEFL_READY */
/* asm: 	.word	T_SET,CHICK_SET,SEND_WAVEFL_SET */
int TLIST[] = {
    T_READY, CHICK_READY, SEND_WAVEFL_READY,
    T_SET, CHICK_SET, SEND_WAVEFL_SET,
};
/* asm: TLGO	.word	T_GO,CHICK_GO,SEND_WAVEFL_GO */
int TLGO[] = {
    T_GO, CHICK_GO, SEND_WAVEFL_GO,
};
/* asm: BABE_CONTROL	.bss	BABE_CONTROL,1 */
int BABE_CONTROL;
/* asm: CURR_FLAGSTATE	.bss	CURR_FLAGSTATE,1 */
int CURR_FLAGSTATE;

void WAVEFLAG(void)
{
    // asm: 	LDP	@STOPWATCH_CNTL
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@STOPWATCH_CNTL
    // asm: 	STI	R0,@BABE_CONTROL
    // asm: 	LDP	@STOPWATCH
    // asm: 	STI	R0,@STOPWATCH
    // asm: 	LDI	@_MODE,R0
    // asm: 	LDI	R0,R5
    // asm: 	OR	MHUD|MSLINE,R0
    // asm: 	STI	R0,@_MODE
    // asm 00001C9C: 	READADJ	ADJ_FREEGAME
    // asm 00001C9D: 	CMPI	0,R0
    // asm 00001C9E: 	BEQ	FGLL
    // asm 00001CA0: 	CREATE	BLINK_FREEBE,UTIL_C
    // asm 00001CA1: 	LDI	@_MODE,R0
    // asm 00001CA2: 	AND	MMODE,R0
    // asm 00001CA3: 	CMPI	MATTR,R0
    // asm 00001CA4: 	BEQ	FGLL
    // asm: 	CREATE	SHOW_RACE_NAME,UTIL_C
FGLL:
    // ;	LDI	@HEAD2HEAD_ON,R0
    // ;	CALLNZ	SEND_BSYNC0
    // asm 00001CA9: 	SLEEP	5
    // asm 00001CAD: 	LDI	@_MODE,R0
    // asm 00001CAE: 	AND	MMODE,R0
    // asm 00001CAF: 	CMPI	MATTR,R0
    // asm: 	BEQ	NOBABE
    // asm: 	CREATEC	BABE_WAVEFLAG,UTIL_C
NOBABE:
    // asm: 	CLRI	R0
    // asm 00001CB3: 	STI	R0,@CURR_FLAGSTATE
    // asm 00001CB5: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001CB6: 	BZ	NOHEAD2HEAD
    // asm 00001CB7: 	CLRI	R0
    // asm 00001CB8: 	STI	R0,@OM_BSYNC
H2HWTLP:
    // asm 00001CB9: 	SLEEP	1
    // asm: 	CALL	SEND_BSYNC3
    // asm: 	LDI	@OM_BSYNC,R0
    // asm 00001CBC: 	CMPI	3,R0
    // asm 00001CBD: 	BNE	H2HWTLP
NOHEAD2HEAD:
    // 	;-----------------------------------
    // 	;	-----------------------------------
    // 	;		-----------------------------------
    // asm 00001CC2: 	SONDFX	STARTLINEREVS2
    // asm 00001CC5: 	LDI	2,AR5
    // asm 00001CC6: 	LDL	TLIST,AR6
WAVEFLAGLP:
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	JDJFF
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BZ	JDJFF
JFF:
    // asm 00001CC8: LDI	@CURR_FLAGSTATE,R0
    // asm: 	CMPI	3,R0
    // asm 00001CCA: 	BEQ	JUMPOUT
    // asm 00001CCB: 	CMPI	@H2H_FLAGSTATE,R0
    // asm: 	BLT	NXTSTAT
    // asm: 	SLEEP	1
    // asm 00001CCC: 	BU	JFF
NXTSTAT:
    // asm 00001CCD: LDI	@H2H_FLAGSTATE,R0
    // asm 00001CCE: 	STI	R0,@CURR_FLAGSTATE
    // asm 00001CCF: 	BU	KKLFF
JDJFF:
    // asm: 	SLEEP	20
KKLFF:
    // asm 00001CD3: 	LDI	*AR6++,AR2
    // asm 00001CD4: 	FLOAT	256,R2
    // asm 00001CD5: 	FLOAT	160,R3
    // asm 00001CD6: 	LDI	20,RC
    // asm 00001CD8: 	CALL	TEXT_ADD
    // asm 00001CD9: 	CALL	SET40FONT
    // asm 00001CDA: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*AR6++,AR2
    // asm 00001CDC: 	CALL	ONESNDFX
    // 	;if H2H_ON && MASTER
    // 	;then  SEND_STATE
    // 	;
    // asm 00001CE1: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001CE2: 	BZ	BABAD
    // asm 00001CE3: 	LDI	@DIPRAM,R0
    // asm 00001CE4: 	TSTB	CMDP_MASTER,R0
    // asm: 	BNZ	BABAD
    // asm 00001CE7: 	LDI	*AR6,R0
    // asm 00001CE8: 	CALLU	R0
    // asm 00001CE9: BABAD	ADDI	1,AR6
    // asm: 	DEC	AR5
    // asm: 	CMPI	0,AR5
    // asm 00001CE9: 	BGT	WAVEFLAGLP
JUMPOUT:
    // asm 00001CEC: LDL	TLGO,AR6
    // asm 00001CED: 	SLEEP	15
    // asm 00001CEF: 	LDI	1,R0
    // asm: 	STI	R0,@BABE_CONTROL
    // asm 00001CF1: 	SLEEP	5
    // asm 00001CF3: 	LDI	*AR6++,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	160,R3
    // asm 00001CF4: 	LDI	20,RC
    // asm 00001CF5: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm 00001CF7: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*AR6++,AR2
    // asm: 	CALL	ONESNDFX
    // asm 00001CFB: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00001CFC: 	BZ	BABAD666
    // asm 00001CFD: 	LDI	@DIPRAM,R0
    // asm 00001CFE: 	TSTB	CMDP_MASTER,R0
    // asm 00001CFF: 	BNZ	BABAD666
    // asm 00001D00: 	CALL	SEND_WAVEFL_GO
    // asm 00001D01: BABAD666
    // asm: 	OR	MGO,R5			;SAVED MODE
    // asm: 	ANDN	MSLINE,R5
    // asm 00001D06: 	STI	R5,@_MODE
    // asm 00001D07: 	STI	R5,@STOPWATCH_CNTL	;STOPWATCH TIMER
    // asm 00001D0B: 	CALL	CHECK_MOTION_DIP
    // asm: 	BNZ	NANAD			;RETURN IF NON MOVING
    // asm: 	CALL	CHECK_MOTION_PRESENT
    // asm: 	BNE	NANAD
    // asm 00001D0C: 	CLRI	AR2
    // asm 00001D0D: 	LDP	@991030h
    // asm 00001D0E: 	LDI	@991030h,R2
    // asm 00001D0F: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	LDL	0FF80h,R1
    // asm: 	AND	R1,R2
    // asm 00001D10: 	BZ	NANAD
    // asm 00001D13: 	LDI	MOTION_ERROR_TIKS,R1
    // asm: 	STI	R1,@WAITTIK
    // asm 00001D14: 	BU	NANAD
    // asm 00001D17: 	CALL	ABORT_RESET_GALIL
    // asm 00001D19: 	LDL	XQ,AR2				;tell galil to continue executing program
    // asm 00001D1A: 	CALL	SEND_CMD
    // asm 00001D1B: 	CALL	WAIT_ACK
NANAD:
    // asm 00001D1C: 	LDI	SM_GO,R0
    // asm 00001D1D: 	STI	R0,@SUSPEND_MODE
    // asm: 	SONDFX	PEELOUT
    // asm 00001D1F: 	READAUD	ADJ_TIME_TO_START
    // asm: 	MPYI	5,R0
    // asm 00001D20: 	ADDI	60,R0
    // asm 00001D21: 	STI	R0,@_countdown
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	NOTHHHH
    // asm 00001D23: 	LDI	@OM_RACE_MODE,R0
    // asm 00001D24: 	CMPI	RM_USA,R0
    // asm: 	BNE	NOTHHHH
    // asm 00001D25: 	LDI	RM_USA,R0
    // asm: 	STI	R0,@RACE_MODE
NOTHHHH:
    // asm: 	LDI	@_MODE,R0	   	;MAKE SURE MODE IS IN GAME
    // asm 00001D2C: 	AND	MMODE,R0
    // asm 00001D2D: 	CMPI	MATTR,R0
    // asm 00001D2E: 	CALLNE	RESUME_TUNE_NT
    // asm 00001D2F:  	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAVEFLAG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RACESEL_TIMER(void)
{
    // asm: 	LDI	@_countdown,R2
    // asm 00001D33: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm 00001D34: 	CALL	_itoa
    // asm 00001D35: 	BUD	IT_E2
    // asm 00001D36: 	FLOAT	256,R2
    // asm 00001D37: 	FLOAT	253,R3
    // asm: 	LDI	1,RC
    // 	;---->	BUD	IT_E2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RACESEL_TIMER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void WAITINTROTIMER(void)
{
    // asm: 	FLOAT	215,R3
    // asm: 	BU	LKJAFSD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAITINTROTIMER", 0, 0);
    UNIMPL();
}

void INTROTIMER(void)
{
    // asm 00001D39: 	FLOAT	350,R3
LKJAFSD:
    // asm 00001D3B: 	PUSHF	R3
    // asm 00001D3C: 	LDI	@_countdown,R2
    // asm 00001D3D: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm 00001D3E: 	CALL	_itoa
    // asm: 	POPF	R3
    // asm: 	FLOAT	256,R2
    // asm: 	LDI	1,RC
IT_E2:
    // asm 00001D3F: CALL	TEXT_ADD
    // asm 00001D40: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	CALL	SETN43FONT
    // asm: 	LDL	lgnum43_coolyelo,AR2
    // asm 00001D42: 	CALL	PAL_FIND_RAW
    // asm 00001D43: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00001D44: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INTROTIMER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*DIAL ROUT
*
*SET THE PROPER OBJECT COLOR CYCLING
*
 */
/* asm: LASTCHOICE	.bss	LASTCHOICE,1 */
int LASTCHOICE;

void DIAL_ROUT(void)
{
    // asm 00001D51: 	LDI	@POSE,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	LDILT	0,AR2
    // asm: 	CMPI	3,AR2
    // asm: 	LDIGT	3,AR2
    // asm: 	STPI	AR2,@CHOSEN_VEHICLE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAL_ROUT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void ENDPLAYER(void)
{
    // asm: 	CLRI	R2
    // asm: 	SETAUD	AUD_BCREDITS
    // asm: 	LDI	AUD_NUM_UNFINISHED,AR2
    // asm: 	CALL	AUDIT_READ
    // asm: 	DEC	R0
    // asm: 	LDI	R0,R2
    // asm: 	SETAUD	AUD_NUM_UNFINISHED
    // asm: 	LDI	0,R0
    // asm 00001D59: 	STI	R0,@FRAMRATE   		;RESET FRAME RATE TO ATTRACT MODE
    // ;	LDI	-5,AR2			;HSTD SHOULD BE NEXT SCREEN!
    // asm: 	LDI	-2,AR2			;HSTD SHOULD BE NEXT SCREEN!
    // asm 00001D5C: 	STI	AR2,@_ATTR_MODE
    // asm 00001D5E: 	LDI	DRONE_C,R0
    // asm 00001D5F: 	LDI	CLASS_M,R1
    // asm 00001D60: 	CALL	PRC_KILLALL
    // asm: 	LDI	SPAWNER_C,R0
    // asm 00001D62: 	LDI	CLASS_M,R1
    // asm 00001D63: 	CALL	PRC_KILLALL
    // *ELP CHANGE
    // asm 00001D64: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 00001D65: 	DMA_WT		R0
    // asm: 	LDI	1,R0
    // asm 00001D66: 	STI	R0,@CLEARRDY	  	;READY FOR INTERRUPT
KK5:
    // asm 00001D67: LDI	@CLEARRDY,R0
    // asm 00001D68: 	BNZ	KK5
    // *ELP END CHANGE
    // *
    // *NOW CLEAN UP THE SYSTEM,. REINITIALIZE EVERYTHING AND
    // *GO INTO ATTRACT MODE
    // *
    // asm 00001D71: 	CALL	TEXT_INIT
    // ;	CLRI	AR2
    // ;	CALL	SENDSND
    // ;	SOND1	GAMEOVR
    // *ELP CHANGE
    // ;	LDL	_SECggate,AR2
    // ;	CALL	LOAD_SECTION_REQ
    // *ELP END CHANGE
    // asm: 	CLRI	R0			;R0	(B0-15) PID
    // asm 00001D7A: 	CLRI	R1			;R1	(B0-15) MASK
    // asm: 	CALL	PRC_KILLALL
    // asm: 	BU	SET_ATTR
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENDPLAYER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GAME AVAILABLE P
*RETURNS
*	CARRY SET  CREDIT ENTER - GAME IS AVAILABLE
*	CARRY CLR  NO CREDIT AVAILABLE
*	R0	CREDITS AVAILABLE
*
 */
void GAME_AVAILABLEP(void)
{
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR2
    // asm: 	READADJ	ADJ_FREE_PLAY
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	GA_TRUE
    // asm: 	CALL	GET_CREDITS_TO_START
    // asm: 	READAUD	AUD_CREDITS
    // asm 00001D7F: 	CMPI	R1,R0
    // asm 00001D80: 	BLT	GA_FALSE
GA_TRUE:
    // asm 00001D84: 	SETC
    // asm: 	POP	AR2
    // asm: 	POP	R2
    // asm 00001D85: 	RETS
GA_FALSE:
    // asm 00001D86: 	CLRC
    // asm 00001D88: 	POP	AR2
    // asm 00001D89: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GAME_AVAILABLEP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*START BUTTON
*
*
 */
void _start(void)
{
    // asm 00001D90: 	LDI	@_MODE,R0
    // asm 00001D91: 	AND	MMODE,R0
    // asm: 	CMPI	MCT,R0
    // asm: 	BEQ	CN
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BEQ	CN
    // asm: 	CMPI	MINSERT_COINS,R0
    // asm: 	BEQ	CN
    // asm: 	CMPI	MINIT,R0
    // asm: 	BEQ	CN
    // asm 00001D92: 	CMPI	MINTRO,R0
    // asm 00001D93: 	BNE	NOTINTRO
CN:
    // asm 00001D94: LDI	1,R0
    // asm 00001D95: 	STI	R0,@START_HIT
    // asm 00001D96: 	DIE
NOTINTRO:
    // asm 00001D99: NOTINSRT
    // asm 00001D9B: 	CMPI	MGAME,R0
    // asm: 	BEQ	_startX
    // asm 00001D9D: 	CALL	GAME_AVAILABLEP
    // asm: 	BC	CANSTART
    // 	;NO CREDITS TO START!
    // asm 00001DA0: 	LDI	@TEASE_COUNT,R0
    // asm: 	CMPI	0,R0
    // asm: 	BGT	_startX
    // asm: 	INC	R0
    // asm: 	STI	R0,@TEASE_COUNT
    // asm 00001DA1: 	SOND1	TEASE_TURNKEY
    // asm 00001DA2: 	DIE
CANSTART:
    // asm 00001DA4: 	CALL	GET_CREDITS_TO_START
    // asm: 	SUBI	R1,R0
    // asm 00001DA5: 	LDI	R0,R2
    // asm 00001DA6: 	CMPI	0,R2
    // asm 00001DA7: 	LDILT	0,R2
    // asm 00001DA8: 	SETAUD	AUD_CREDITS		;DECREMENT CREDIT COUNT
    // asm 00001DAC: 	LDI	0,R2
    // asm: 	LDI	AUD_BCREDITS,AR2
    // asm: 	CALL	AUDIT_WRITE
    // asm 00001DAE: startgame
    // asm: 	LDI	@_MODE,R0
#if DEBUG
    // asm 00001DB1: 	LDI	R0,R1
    // asm 00001DB2: 	ANDN	MMODE,R0
    // asm: 	AND	MMODE,R1
    // asm 00001DB4: 	CMPI	MATTR,R1		;this kludge allows us, in debugging
    // asm 00001DB5: 	BEQ	CYCLEOUT		;more to cycle out of game mode into
    // asm 00001DB6: 	OR	MATTR,R1		;attract mode...
    // asm: 	STI	R1,@_MODE		;
    // 	;
    // 	;SYSTEM SHUTDOWN, STRAIGHTLINE CODE NOW ACTIVE
    // 	;
    // asm: 	BR	CYCLE_ATTR
    // 	;
    // 	;
CYCLEOUT:
#endif
    // asm: 	ANDN	MMODE,R0
    // *ELP CHANGE February 8,1995
    // ;	OR	MGAME,R0
    // 	;we must begin the game in MINTRO
    // 	;
    // 	;
    // asm: 	OR	MINTRO,R0
    // *ELP END CHANGE
    // asm: 	STI	R0,@_MODE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@(_plyr1+PLY_CAR)
    // asm:         LDP     @FASTSTKI		;GET PAGE OF STORED ADDRESS
    // asm:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm: 	LDI	1,AR2
    // asm: 	CALL	WAVE
    // asm: 	BR	COLD_ENTER		;RESET SYSTEM RUNNING
_startX:
    // asm 00001DBA: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_start", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	AR4	OBJECT
*
*
 */
void ULTRA_PROC(void)
{
    // asm 00001DC2: 	CLRF	R6
UPLP:
    // asm: LDF	0.0349078,R0
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	SUBF	R0,R6
    // asm: 	LDF	R6,R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	SLEEP	1
    // asm: 	BU	UPLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ULTRA_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void ULTRA_LOGO(void)
{
    // asm 00001DC9: 	CALL	OBJ_GET
    // asm 00001DCA: 	RETSC
    // asm 00001DCB: 	LDIL	nintendo,R0
    // asm 00001DCC: 	STI	R0,*+AR0(OROMDATA)
    // asm 00001DCE: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	50,R0
    // asm 00001DCF: 	STF	R0,*+AR0(OPOSY)
    // asm 00001DD0: 	FLOAT	368,R0
    // asm 00001DD1: 	STF	R0,*+AR0(OPOSZ)
    // asm 00001DD5: 	LDI	AR0,AR2
    // asm 00001DD6: 	CALL	OBJ_INSERTP
    // asm: 	LDI	AR2,AR4
    // asm 00001DD8: 	PUSH	AR0
    // asm 00001DD9: 	PUSH	AR2
    // asm: 	CREATE	ULTRA_PROC,UTIL_C
    // asm 00001DDA: 	POP	AR2
    // asm 00001DDB: 	POP	AR0
    // asm 00001DDC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ULTRA_LOGO", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void LOGO_SMALL(void)
{
    // asm 00001DE1: 	LDI	@DIPRAM,R0
    // asm 00001DE2: 	TSTB 	DIP_COMMP,R0
    // asm 00001DE3: 	RETSNZ
    // asm 00001DE8: 	LDL	redhd1,AR2	;red (bottom)
    // asm: 	LDI	230,R2
    // asm: 	LDI	-190-60,R3
    // asm: 	LDI	926,RC	;368*2
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	RETSC
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 00001DE9: 	OR	*+AR0(OFLAGS),R0
    // asm 00001DEA: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001DEB: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDL	H2HPAL2,AR2
    // asm 00001DEC: 	CALL	PAL_FIND_RAW
    // asm 00001DED: 	STI	R0,*+AR0(OPAL)
    // asm 00001DF0: 	LDL	yelhd1,AR2	;top
    // asm 00001DF1: 	LDI	230,R2
    // asm 00001DF2: 	LDI	-190+60,R3
    // asm 00001DF3: 	LDI	926,RC	;368*2
    // asm 00001DF4: 	CALL	OBJ_QMAKE
    // asm 00001DF5: 	RETSC
    // asm 00001DF6: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 00001DF7: 	OR	*+AR0(OFLAGS),R0
    // asm 00001DF8: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001DF9: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDL	H2HPAL2,AR2
    // asm 00001DFA: 	CALL	PAL_FIND_RAW
    // asm 00001DFB: 	STI	R0,*+AR0(OPAL)
    // asm 00001DFE: 	LDL	big2,AR2
    // asm 00001DFF: 	LDI	230,R2
    // asm 00001E00: 	LDI	-190,R3
    // asm 00001E01: 	LDI	924,RC	;(368*2)-2
    // asm 00001E02: 	CALL	OBJ_QMAKE
    // asm 00001E03: 	RETSC
    // asm 00001E04: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm 00001E05: 	OR	*+AR0(OFLAGS),R0
    // asm 00001E06: 	STI	R0,*+AR0(OFLAGS)
    // asm 00001E07: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDL	H2HPAL3,AR2
    // asm 00001E08: 	CALL	PAL_FIND_RAW
    // asm 00001E09: 	STI	R0,*+AR0(OPAL)
    // asm 00001E0A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOGO_SMALL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*STRAIGHT LINE CODE, SYSTEM SHUTDOWN
*THIS ROUTINE IS BRANCHED TO, NOT CALLED!
*
*
 */
void SET_ATTR(void)
{
    // asm 00001E15: 	CALL	SILENT
    // asm 00001E16:         LDI	@FASTSTKI,SP		;GET PAGE OF STORED ADDRESS
    // asm: 	LDI	@_ATTR_MODE,AR2		;AND INTO FP TOO
    // asm: 	CALL	WAVE
    // asm: 	BU	COLD_ENTER
CYCLE_ATTR:
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_ATTR", 0, 0);
    UNIMPL();
}

void _debug(void)
{
    // asm: 	CALL	SILENT
    // asm: 	CALL	LOAD_FIXED_PALETTES
    // asm 00001E17:         LDI	@FASTSTKI,SP		;GET PAGE OF STORED ADDRESS
    // asm 00001E18: 	LDI	@_ATTR_MODE,AR2		;AND INTO FP TOO
    // asm 00001E19: 	DEC	AR2
    // asm 00001E1A: 	CMPI	-4,AR2
    // asm 00001E1B: 	LDILT	-1,AR2
    // asm: 	STI	AR2,@_ATTR_MODE
    // asm: 	CALL	WAVE
    // asm: 	BU	COLD_ENTER		;RESET SYSTEM RUNNING
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_debug", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ATTRACT MODE TIMEOUT MECHANISM
*SLEEP _timer TIKS THEN JUMP TO CYCLE_ATTR
*
 */
/* asm: _timer	.bss	_timer,1 */
int _timer;

void _timeout(void)
{
    // asm 00001E23: 	LDI	@_timer,AR2
    // asm 00001E24: 	CALL	SLEEP
    // asm 00001E25: 	BU	CYCLE_ATTR
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_timeout", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*INSERT COINS ROUTINES
*
*	INSMORE		JSRPed FROM PLYR.ASM
*	COIN_CNTDOWN	CREATED, KILLED
*
 */
/* asm: TROI	SPTR	"INSERT COINS" */
const char *TROI = "INSERT COINS";
/* asm: ICCI	SPTR	"TO CONTINUE" */
const char *ICCI = "TO CONTINUE";
/* asm: PSCI	SPTR	"PRESS START" */
const char *PSCI = "PRESS START";
/* asm: SAVEDMODE	.bss	SAVEDMODE,1 */
int SAVEDMODE;

void INSMORE(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	STI	R0,@SAVEDMODE
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	BNZ	KFFDA
    // asm: 	CALL	ABORT_RESET_GALIL
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@MOTION_STOP_HIT
    // asm: 	STI	R0,@MOTION_SAFETY_ON
    // asm 00001E2C: 	STI	R0,@WAITTIK
KFFDA:
    // 	;
    // asm: 	LDF	@GAME_TIMER,R2
    // asm 00001E30: 	MPYF	100,R2
    // asm 00001E31: 	FIX	R2
    // asm 00001E32: 	READAUD	AUD_TOTAL_TIME
    // asm 00001E33: 	ADDI	R0,R2
    // asm: 	SETAUD	AUD_TOTAL_TIME
    // asm: 	READAUD	AUD_NUM_BUYINS
    // asm: 	LDI	R0,R1
    // asm: 	LDI	R2,R0
    // asm: 	CALL	DIV_I30
    // asm 00001E36: 	LDI	R0,R2
    // asm 00001E37: 	SETAUD	AUD_AVG_TIME
    // 	;
    // asm 00001E3B: 	LDI	20,R0
    // asm: 	STI	R0,@_countdown
    // asm 00001E40: 	CALL	SILENT
    // asm 00001E41: 	SOND1	DISCODUCK
    // asm 00001E42: 	CLRI	R0
    // asm 00001E43: 	STI	R0,@STOPWATCH_CNTL
    // asm: 	STI	R0,@START_HIT
    // asm: 	LDI	@_MODE,R0
    // asm 00001E45: 	ANDN	MMODE,R0
    // asm 00001E46: 	OR	MINSERT_COINS|MGO,R0
    // asm: 	STI	R0,@_MODE
    // asm 00001E48: 	LDI	@TROI,AR2
    // asm: 	FLOAT	256,R2
    // asm 00001E4A: 	FLOAT	100,R3
    // asm 00001E4B: 	LDI	9999,RC
    // asm 00001E4C: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm 00001E4D: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001E4E: 	LDI	AR0,AR4			;SAVE POINTERS FOR TEXT CHANGE
    // asm: 	LDI	@ICCI,AR2
    // asm: 	FLOAT	256,R2
    // asm 00001E51: 	FLOAT	150,R3
    // asm 00001E52: 	LDI	9999,RC
    // asm 00001E53: 	CALL	TEXT_ADD
    // asm 00001E54: 	CALL	SET40FONT
    // asm 00001E55: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00001E5A: 	CREATEC	COIN_CNTDOWN,034h
    // asm: 	READAUD	AUD_BCREDITS
    // asm: 	LDI	R0,R4			;WATCH CREDITS
    // asm 00001E5C: 	CLRI	R5			;TO START TOGGLE
    // asm 00001E5D: 	LDI	15,AR6			;wait at least 15 frames to continue
    // asm 00001E5E: 	LDI	0,R0
    // asm 00001E5F: 	STI	R0,@miniidle
    // 	;wait for a coin to be dropped in
    // 	;if coins are in change text to PRESS START
INSMORE_LP:
    // asm: 	CALL	INSERT_MORE_COINS
    // asm 00001E6C: 	READAUD	AUD_BCREDITS
    // asm 00001E6D: 	CMPI	R0,R4
    // asm: 	BEQ	NOINCTIM
    // asm: 	LDI	R0,R4
    // asm: 	LDI	20,R0
    // asm: 	STI	R0,@_countdown
NOINCTIM:
    // asm 00001E6E: 	LDI	R5,R5
    // asm: 	BNZ	CHECKHIT
    // asm 00001E6F: 	READADJ	ADJ_FREE_PLAY
    // asm 00001E71: 	CMPI	1,R0
    // asm 00001E72: 	BEQ	FREEP
    // asm 00001E74: 	READAUD	AUD_CREDITS
    // asm: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm 00001E76: 	CMPI	R1,R0
    // asm 00001E77: 	BLT	JUSTGOON
    // ;	CMPI	0,R0
    // ;	BEQ	JUSTGOON
FREEP:
    // asm: 	LDI	@PSCI,R0		;change the text
    // asm: 	STI	R0,*+AR4(TEXT_PTR)
    // asm 00001E7C: 	LDI	1,R5
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm 00001E7E: 	OR	BUT_START,R0
    // asm 00001E7F: 	STI	R0,@BUTTON_STATUS
JUSTGOON:
    // 	;check to see if plyr hit start (decrement count)
    // asm: 	LDI	R5,R5
    // asm: 	BNZ	CANWT
TOSLP:
    // 	;HITTING THE BUTTON W/O CREDITS
    // asm 00001E85: 	LDI	@START_HIT,R0
    // asm 00001E86: 	BZ	NRST
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_HIT
    // asm: 	LDI	20,R0
    // asm 00001E87: 	STI	R0,@_countdown
NRST:
    // asm: 	LDI	@miniidle,R0	;only every 10th frame
    // asm: 	CMPI	0,R0
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm: 	LDL	SW_RADIO|SW_VIEW0|SW_VIEW1|SW_VIEW2,R1
    // asm 00001E89: 	AND	R1,R0
    // asm 00001E8A: 	BZ	TOSLP2
    // asm 00001E8C: 	CMPI	R1,R0		;but if ALL are pressed...dont decrement
    // asm 00001E8D: 	BEQ	TOSLP2
    // asm: 	LDI	@_countdown,R0
    // asm 00001E8F: 	DEC	R0
    // asm 00001E90: 	STI	R0,@_countdown
TOSLP2:
    // asm 00001E93: 	DEC	AR6
    // asm 00001E94: 	CMPI	0,AR6
    // asm: 	BGT	CANWT
    // asm 00001E96: 	LDI	@SWITCHBUTS,R0
    // asm: 	TSTB	SW_START,R0
    // asm 00001E97: 	BZ	CANWT
    // ;	LDI	AR5,R0		;if no decrement -> DONT
    // ;	BNZ	CANWT
    // asm: 	LDI	@_countdown,R0
    // asm 00001E9A: 	DEC	R0
    // asm 00001E9B: 	STI	R0,@_countdown
    // asm 00001E9C: 	LDI	15,AR6			;wait at least 15 frames to continue
CANWT:
    // asm 00001E9F: 	LDI	@miniidle,R0
    // asm: 	INC	R0
    // asm: 	CMPI	25,R0
    // asm: 	LDIGE	0,R0
    // asm: 	STI	R0,@miniidle
    // asm 00001EA0: 	SLEEP	1
    // asm 00001EA1: 	LDI	@_countdown,R0
    // asm 00001EA2: 	BNZ	INSMORE_LP
    // asm 00001EA3: 	CLRI	AR6
    // asm: 	BU	RETURNTOPLYR
CHECKHIT:
    // asm 00001EA4: 	LDI	@START_HIT,R0
    // asm 00001EA5: 	BZ	TOSLP
    // 	;Secret Button Combo!!
    // 	;
    // 	;if the plyr holds all the view buttons and radio as he
    // 	;hits the start button, he gets to CRUISE the USA
    // 	;-ELP August 11,1994
    // 	;
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm: 	LDIL	(SW_RADIO|SW_VIEW0|SW_VIEW1|SW_VIEW2),R1
    // asm 00001EAF: 	AND	R1,R0
    // asm 00001EB0: 	CMPI	R1,R0
    // asm: 	BNE	NOSECRET_CRUISE
    // asm: 	LDI	RM_USA,R0
    // asm: 	STI	R0,@RACE_MODE
NOSECRET_CRUISE:
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_BCREDITS,AR2
    // asm 00001EB1: 	CALL	AUDIT_WRITE
    // asm 00001EB7: 	READAUD	AUD_CREDITS
    // asm: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm 00001EB8: 	SUBI	R1,R0
    // ;	DEC	R0
    // asm 00001EBA: 	LDILT	0,R0
    // asm 00001EBB: 	LDI	R0,R2
    // asm 00001EBC: 	SETAUD	AUD_CREDITS
    // asm: 	LDI	@_MODE,R0
    // asm 00001EBD: 	ANDN	MMODE,R0
    // asm 00001EBF: 	OR	MGAME,R0
    // asm 00001EC0: 	STI	R0,@_MODE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@STOPWATCH_CNTL
    // asm 00001EC2: 	LDI	60,R0
    // asm 00001EC3: 	STI	R0,@_countdown
    // asm: 	LDI	1,AR6
    // asm 00001EC5: 	LDI	@BUTTON_STATUS,R0
    // asm 00001EC6: 	ANDN	BUT_START,R0
    // asm 00001EC7: 	STI	R0,@BUTTON_STATUS
    // asm: 	INCAUD	AUD_NUM_BUYINS
    // asm 00001ECA: RETURNTOPLYR
    // asm: 	LDI	@SAVEDMODE,R0
    // asm 00001ECB: 	STI	R0,@_MODE
    // asm 00001ECD: 	LDI	034h,R0
    // asm: 	LDI	-1,R1
    // asm 00001ECE: 	CALL	PRC_KILLALL
    // asm 00001ECF: 	CALL	TEXT_INIT
    // asm 00001ED0: 	CALL	RESUME_TUNE_NT
    // asm 00001ED1: 	CLRF	R0
    // asm: 	STPF	R0,@GAME_TIMER
    // asm: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSMORE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PLACE THE COUNTDOWN TIMER
*
*
 */
void COIN_CNTDOWN(void)
{
    // asm 00001EDA: 	LDI	@_countdown,R4
COIN_CNTDOWN_LP:
    // asm: 	LDI	@_countdown,R2
    // asm: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	270,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	CALL	SETN43FONT
    // asm: 	LDL	lgnum43_coolyelo,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm 00001EE1: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00001EE2: 	SLEEP	1
    // asm 00001EE3: 	BU	COIN_CNTDOWN_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN_CNTDOWN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: DIRTY_SHARED	.bss	DIRTY_SHARED,1 */
int DIRTY_SHARED;

void LOAD_SHARED(void)
{
    // asm 00001EEC: 	CLRI	R0
    // asm 00001EED: 	STI	R0,@DIRTY_SHARED
    // asm 00001EEF: 	LDL	_SECshared,AR2
    // asm 00001EF1: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	bottom_gtmp_p,R0		;ONE TO OVERWRITE
    // asm: 	LDI	R0,R1			;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	bottom2_gtmp_p,R0
    // asm: 	LDI	R0,R1
    // asm 00001EF2: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	shldr2_gtmp_p,R0
    // asm 00001EF4: 	LDI	R0,R1
    // asm 00001EF5: 	CALL	PAL_OVERWRITE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SHARED", 0, 0);
    UNIMPL();
}

void TRAFFIC_LIGHT(void)
{
    // asm: 	SLEEP	20
    // asm 00001EFC: 	LDL	TRAFFIC_LL,AR5
TLT_LP:
    // asm 00001EFF: 	LDI	light_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	BNC	NSSD
    // asm: 	BR	SUICIDE
NSSD:
    // asm: 	LDI	*AR5++,AR2
    // asm: 	CMPI	-1,AR2
    // asm 00001F0F: 	BNE	CCC
    // asm 00001F11: 	LDL	TRAFFIC_LL,AR5
    // asm: 	LDI	*AR5++,AR2
CCC:
    // asm 00001F12: 	LDI	R0,R2
    // asm 00001F13: 	LDIL	8000000Ah,R3	;16
    // asm 00001F14: 	CALL	PAL_SET
    // asm 00001F15: 	LDI	*AR5++,AR2
    // asm: 	CALL	SLEEP
    // asm: 	BU	TLT_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAFFIC_LIGHT", 0, 0);
    UNIMPL();
}

/* asm: TRAFFIC_LL	.word	light_yellowon,10,light_redon,32,light_greenon,32,-1 */
int TRAFFIC_LL[] = {
    light_yellowon, 10, light_redon, 32, light_greenon, 32, -1,
};

/* *----------------------------------------------------------------------------
 */
void CPOINT_LIGHT(void)
{
    // asm 00001F1C: 	LDI	@RGBTAB_CPI,AR4
    // asm 00001F1F: 	INC	AR4
    // asm 00001F21: 	LDI	checks_p,AR2
    // asm 00001F22: 	CALL	PAL_FIND
    // asm: 	BNC	SDASDFA
    // asm: 	BR	SUICIDE
SDASDFA:
    // asm: 	LDI	R0,AR6
CPL_LP:
    // asm: 	LDI	AR4,AR2
    // asm 00001F2A: 	LDI	AR6,R2
    // asm 00001F2B: 	ADDI	251,R2
    // asm: 	LDI	5,R3
    // asm 00001F2C: 	CALL	PAL_SET
    // asm 00001F2F: 	LDI	*++AR4(5),R0
    // asm: 	BNN	CNT
    // asm 00001F30: 	LDI	@RGBTAB_CPI,AR4
CNT:
    // asm 00001F31: 	LDI	*AR4++,AR2
    // asm 00001F32: 	CALL	SLEEP
    // asm 00001F33: 	B	CPL_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CPOINT_LIGHT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: RGBTAB_CP */
/* asm: .word	4 */
/* asm: RGB	212,212,0 */
/* asm: RGB	212,212,0 */
/* asm: RGB	212,212,0 */
int RGBTAB_CP;

/* *----------------------------------------------------------------------------
*
*
*
*
 */
void SHOW_RACE_NAME(void)
{
    // asm: 	LDI	@BONUS_WAVE,AR2
    // asm: 	ADDI	@LEG_NAMESI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	360,R3
    // asm: 	LDI	340,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	AR1,AR5
    // asm 00001F59: 	FLOAT	-100,R6
    // asm 00001F5A: 	LDI	20,AR6
SLLP1:
    // asm 00001F5B: FLOAT	256,R0
    // asm 00001F5C: 	SUBF	R6,R0
    // asm 00001F5D: 	MPYF	0.2,R0
    // asm 00001F60: 	ADDF	R0,R6
    // asm: 	STF	R6,*+AR5(TEXT_POSX)
    // asm 00001F63: 	LDF	R6,R0
    // asm 00001F64: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR4(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm 00001F65: 	DBU	AR6,SLLP1
    // 	;CENTER IT
    // 	;
    // asm 00001F6A: 	FLOAT	256,R6
    // asm 00001F6B: 	STF	R6,*+AR5(TEXT_POSX)
    // asm 00001F6C: 	LDF	R6,R0
    // asm 00001F6D: 	ADDF	3,R0
    // asm 00001F6E: 	STF	R0,*+AR4(TEXT_POSX)
    // asm 00001F6F: 	SLEEP	50
    // asm: 	LDI	20,AR6
SLLP1A:
    // asm 00001F72: FLOAT	-100,R0
    // asm: 	SUBF	R6,R0
    // asm 00001F72: 	MPYF	0.2,R0
    // asm 00001F73: 	ADDF	R0,R6
    // asm 00001F74: 	STF	R6,*+AR5(TEXT_POSX)
    // asm 00001F75: 	LDF	R6,R0
    // asm 00001F76: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR4(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR6,SLLP1A
    // asm 00001F79: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_RACE_NAME", 0, 0);
    UNIMPL();
}

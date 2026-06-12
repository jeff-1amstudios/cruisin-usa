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
#include "comm.h"
#include "h2hobj.h"
#include "bonus.h"
#include "discovered_defines.h"

/*
 * Source module: asm/BONUS.ASM
 */

/* asm: MAXMPH	.bss	MAXMPH,1 */
int MAXMPH;
/* asm: CHALLENGE_RACE	.bss	CHALLENGE_RACE,1 */
int CHALLENGE_RACE;
/* asm: NEXT_STARTUP	.bss	NEXT_STARTUP,1 */
int NEXT_STARTUP;
/* asm: BONUS_WAVE	.bss	BONUS_WAVE,1 */
int BONUS_WAVE;
/* asm: FINISH_LINE	.bss	FINISH_LINE,1 */
int FINISH_LINE;
/* asm: DO_FOLDFLAG	.bss	DO_FOLDFLAG,1 */
int DO_FOLDFLAG;
#define NUM_LEGS 14
/* asm: LEG_NAMES	.word	LEG1,LEG2,LEG3,LEG4,LEG5,LEG6,LEG7,LEG8 */
/* asm: 	.word	LEG9,LEG10,LEG11,LEG12,LEG13,LEG14,LEG_USA */
const char * *LEG_NAMES[15] = {
    LEG1, LEG2, LEG3, LEG4, LEG5, LEG6, LEG7, LEG8,
    LEG9, LEG10, LEG11, LEG12, LEG13, LEG14, LEG_USA,
};
const char *LEG1 = "GOLDEN GATE PARK";
const char *LEG2 = "SAN FRANCISCO";
const char *LEG3 = "US 101";
const char *LEG4 = "REDWOOD FOREST";
const char *LEG5 = "BEVERLY HILLS";
const char *LEG6 = "LA FREEWAY";
const char *LEG7 = "DEATH VALLEY";
const char *LEG8 = "ARIZONA";
const char *LEG9 = "GRAND CANYON";
const char *LEG10 = "IOWA";
const char *LEG11 = "CHICAGO";
const char *LEG12 = "INDIANA";
const char *LEG13 = "APPALACHIA";
const char *LEG14 = "WASHINGTON DC";
const char *LEG_USA = "SF TO WASHINGTON DC";
const char *WINT1 = "FREE GAME FOR";
const char *WINT2 = "1ST PLACE";
const char *EXPIRED = "EXPIRED";
/* *STATISTICAL STRUCTURE FOR THE GAME LEGS <NERD DATA>
*
*
*
*STRUCT	tagGAMETRAK
 */
#define GT_ETIME 0 //TC	ELAPSED TIME (TIME CODED)
#define GT_POS 1 //UD	POSITION
#define GT_MAXMPH 2 //UD	MAX MPH
#define GT_COLLS 3 //UD	NUMBER OF COLLISIONS
#define GT_SIZE 4 //SIZ
/* asm: GAMETRAK	.bss	GAMETRAK,NUM_LEGS*GT_SIZE */
int GAMETRAK[NUM_LEGS*GT_SIZE];
/* asm: ETIME	.bss	ETIME,1 */
int ETIME;
/* ;etime,#,maxmph
 */
/* asm: BUFFERSI	.word	BIGBUFFER */
#define BUFFERSI BIGBUFFER
/* asm: BIGBUFFER	.bss	BIGBUFFER,(4+1+2+2)*NUM_LEGS */
int BIGBUFFER[(4+1+2+2)*NUM_LEGS];
/* asm: BONUS_POSTLAUNCH */
/* asm: 	.word	BONUS_GGATE,BONUS_SANFRAN,BONUSNULL,BONUSNULL */
/* asm: 	.word	BONUS_BEVHILLS,BONUSNULL,BONUSNULL */
/* asm: 	.word	BONUSNULL,BONUSNULL,BONUSNULL */
/* asm: 	.word	BONUSNULL,BONUSNULL,BONUSNULL */
/* asm: 	.word	BONUSNULL,BONUSNULL,BONUSNULL */
int BONUS_POSTLAUNCH[16] = {
    BONUS_GGATE, BONUS_SANFRAN, BONUSNULL, BONUSNULL,
    BONUS_BEVHILLS, BONUSNULL, BONUSNULL,
    BONUSNULL, BONUSNULL, BONUSNULL,
    BONUSNULL, BONUSNULL, BONUSNULL,
    BONUSNULL, BONUSNULL, BONUSNULL,
};
/* asm: BONUS_TABLE	.word	BONUS1,BONUS2,BONUS3,BONUS4,BONUS5 */
/* asm: 	.word	BONUS6,BONUS7,BONUS8,BONUS9,BONUS10 */
/* asm: 	.word	BONUS11,BONUS12,BONUS13,BONUS14 */
int BONUS_TABLE[14] = {
    BONUS1, BONUS2, BONUS3, BONUS4, BONUS5,
    BONUS6, BONUS7, BONUS8, BONUS9, BONUS10,
    BONUS11, BONUS12, BONUS13, BONUS14,
};
const char *BHDDFAS = "WINNER";
/* asm: UNFOLDFLAG	.bss	UNFOLDFLAG,1 */
int UNFOLDFLAG;
/* *----------------------------------------------------------------------------
*
 */
/* asm: SAVED_COUNTDOWN	.bss	SAVED_COUNTDOWN,1 */
int SAVED_COUNTDOWN;
/* asm: CONTINUE	CONTINUE
	.bss	WAS_HEAD2HEAD_ON,1 */
int CONTINUE[WAS_HEAD2HEAD_ON];
/* asm: DID_TIMED_OUT	.bss	DID_TIMED_OUT,1 */
int DID_TIMED_OUT;
const char *FRA1 = "FIRST PLACE";
const char *FRA2 = "FREE RACE";
#define CONGRATS 4
/* *----------------------------------------------------------------------------
*
*
 */
const char *LCTS = "RACE COMPLETED:";
const char *LNLS = "NEXT RACE:";
/* *
*
 */
#define SHOWLEG_TIME (20+20+50)
/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	R4	INDEX
*
*
 */
/* asm: FLAG_POS_TABLE */
/* asm: 	.word	 36,133	;GGpark */
/* asm: 	.word	 39,147	;Sanfran */
/* asm: 	.word	 40,156	;I101 */
/* asm: 	.word	 43,166	;Redwds */
/* asm: 	.word	 63,192	;BeverlyHils */
/* asm: 	.word	 76,200	;LA freeway */
/* asm: 	.word	 88,190	;Deathvalley */
/* asm: 	.word	128,198	;Arizona */
/* asm: 	.word	257,106	;Mount Rush */
/* asm: 	.word	302,125	;Iowa */
/* asm: 	.word	334,125	;Chicago */
/* asm: 	.word	358,134	;Indiana */
/* asm: 	.word	412,129	;Appalachia */
/* asm: 	.word	434,140	;Washington */
int FLAG_POS_TABLE[28] = {
    36, 133, // GGpark
    39, 147, // Sanfran
    40, 156, // I101
    43, 166, // Redwds
    63, 192, // BeverlyHils
    76, 200, // LA freeway
    88, 190, // Deathvalley
    128, 198, // Arizona
    257, 106, // Mount Rush
    302, 125, // Iowa
    334, 125, // Chicago
    358, 134, // Indiana
    412, 129, // Appalachia
    434, 140, // Washington
};
const char *BT1 = "AVERAGE MPH:";
const char *BT1A = "AVERAGE KPH:";
const char *RT2 = "RECORD TIME:";
const char *HH1 = "NEW RECORD TIME";
const char *HH2 = "NEW HOT TIME";
const char *NULLSTR5 = " ";
const char *BT2 = "ELAPSED TIME:";
const char *BT3 = "POSITION:";

/* *----------------------------------------------------------------------------
 */
void BONUS_SANFRAN(void)
{
    // asm: 	CREATE	TRAFFIC_LIGHT,SPAWNER_C|COLORCYC_T
    // asm: 	FLOAT	-35,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS_SANFRAN", 0, 0);
    UNIMPL();
}

void BONUS_GGATE(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void BONUS_BEVHILLS(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MWATER,R0
    // asm: 	STI	R0,@_MODE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS_BEVHILLS", 0, 0);
    UNIMPL();
}

void BONUSNULL(void)
{
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUSNULL", 0, 0);
    UNIMPL();
}

void JSUB(void)
{
    // asm: 	LDL	BHDDFAS,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	278,R3
    // asm: 	LDI	7,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm: 	OR	TXT_CENTER,R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm: 	OR	TXT_CENTER,R0
    // asm: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JSUB", 0, 0);
    UNIMPL();
}

void DISPLAY_H2H_WINNER(void)
{
    // asm: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm: 	BZ	DODIE
    // asm: 	LDI	@OM_POSITION,R0
    // asm: 	CMPI	@POSITION,R0
    // asm: 	BLT	DODIE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@UNFOLDFLAG
    // asm: 	LDL	big2,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	140,R3
    // asm: 	LDI	856,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTHP
    // asm: 	LDL	H2HPAL3,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	AR0,*+AR7(PDATA+2)	;big2
    // asm: 	LDL	redhd1,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	80,R3
    // asm: 	LDI	868,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTHP
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	AR0,*+AR7(PDATA)	;red (bottom)
    // asm: 	LDL	yelhd1,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	200,R3
    // asm: 	LDI	868,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTHP
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	AR0,*+AR7(PDATA+1)	;yellow (bottom)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_H2H_WINNER", 0, 0);
    UNIMPL();
}

void OBJ_DELETE_HIGH_PRIORITY(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	LDI	@OHIGH_PRIORITYI,R1
DELLP:
    // asm: LDI	R1,AR1		;WE MUST FIND DEAD OBJECT TO LINK AROUND
    // asm: 	LDI	*AR1,R1
    // asm: 	BZ	DELOBJX
    // asm: 	CMPI	R1,AR2
    // asm: 	BNE	DELLP
    // asm: 	LDI	*AR2,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	@OFREE,R1
    // asm: 	STI	R1,*AR2
    // asm: 	STI	AR2,@OFREE
    // asm: 	LDI	@OFREECNT,R0		;INCREMENT FREE OBJECT COUNT
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,@OFREECNT
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR2(OLINK2)	;CLEAR SEARCH ID
    // asm: 	STI	R0,*+AR2(OFLAGS)
DELOBJX:
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_DELETE_HIGH_PRIORITY", 0, 0);
    UNIMPL();
}

void BONUS_SCREEN(void)
{
    // asm: 	LDI	@BGNDCOLA,R0
    // asm: 	STI	R0,*+AR7(PDATA)
    // asm: 	LDI	@_countdown,R0
    // asm: 	STI	R0,@SAVED_COUNTDOWN
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	CALL	SILENT
    // asm: 	CALL	SND_RESET_QUIET
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@STOPWATCH_CNTL
    // asm: 	STI	R0,@DO_FOLDFLAG
    // asm: 	CALL	KILL_THEM
    // asm: 	LDI	0,R0			;SMOKE MAY BE KILLED, SO I HAVE TO RESET THIS
    // asm: 	STI	R0,@TIRE_SMOKE_COUNT
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@IGNORE_UPDATES
    // asm: 	CALL	PRC_INIT
    // asm: 	CREATE	BONSCRN2,UTIL_C
    // asm: 	CREATE	BONUS_WAIT_LOOP,PLYR_C|PLYR1_T
    // asm: 	CREATE	SCAN_OBJECTS,UTIL_C
    // asm: 	CALL	FIND_AND_REACTIVATE	;REACTIVATE CHEERING ANIMATIONS
#if DEBUG
    // asm: 	CALL	VERIFY_CODE_INTEGRITY
#endif
    // asm:         LDP     @FASTSTKI		;GET PAGE OF STORED ADDRESS
    // asm:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm: 	BR	COLD_ENTER
    // *
    // *
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS_SCREEN", 0, 0);
    UNIMPL();
}

void BONSCRN2(void)
{
    // asm: 	LDI	@DID_TIMED_OUT,R0
    // asm: 	BZ	DOREG3A
    // asm: 	LDI	0,R0		;ELAPSED TIME OF 0 = DID NOT FINISH
    // asm: 	STI	R0,@ETIME
DOREG3A:
    // asm: LDI	@STOPWATCH,R0
    // asm: 	STI	R0,@ETIME
    // *ELP CHANGE
    // 	;TURN ON LINK HERE... ALL LISTEN
    // 	;
    // 	;
    // asm: 	CALL	CLRONE		;CAN NOW BE DUAL PLAYER
    // *ELP END CHANGE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_HIT	;----------------------------------
    // asm: 	CREATE	MOVEOUT_HUD_EQUIP,UTIL_C
    // 	;Wait for sound board to reset
    // 	;
CNR_ENTER:
    // asm: 	LDP	@IN_RESET_MODE
    // asm: 	LDI	@IN_RESET_MODE,R0
    // asm: 	SETDP
    // asm: 	BZ	CONTINUE
    // asm: 	SLEEP	1
    // asm: 	BU	CNR_ENTER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONSCRN2", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*entered into from above (process)
*
*
*
*
 */
void CLINTON_SHOW(void)
{
    // asm: 	LDI	RM_SINGLE,R0
    // asm: 	STI	R0,@RACE_MODE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@DO_FOLDFLAG
    // asm: 	LDI	UTIL_C,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	UTIL_C|TEXTP_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	PLYR_C|PLYR1_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	CALL	TEXT_INIT
    // asm: 	READAUD	ADJ_CLINTON
    // asm: 	CMPI	1,R0
    // asm: 	BNE	NOCLINTON
    // asm: 	JSRP	HOTTUB_SCENE
NOCLINTON:
    // 	;patched from hottub.asm
    // 	;to cure potential lockup when HOTTUB_SCENE
    // 	;is not called.
    // 	;Wed Mar 8 09:52:18 1995
    // 	;
    // 	;
    // asm: 	SLEEP	1
    // asm: 	CALL	OBJ_INIT
    // asm: 	CALL	INIT_DRONES	;initialize DRONE tracker system
    // asm: 	CALL	DYNAOBJ_INIT	;initialize DYNAMIC OBJECTS
    // asm: 	CALL	CARB_INIT	;initialize CAR BLOCKS
    // asm: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    // asm: 	LDI	1234h,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	SPAWNER_C|ANIMATION_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDL	dc_shared_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	finale_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // 	;
    // 	;
    // 	;>>>insert MAP with time for entire race
    // 	;
    // asm: 	LDI	15,R0
    // asm: 	STI	R0,@BONUS_WAVE
    // asm: 	READAUD	ADJ_HIGH_SCORE_ENTRY
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NOENTR2
    // asm: 	JSRP	ENTER_INITIALS
    // asm: 	LDF	@STEERCT,R0		;STEERING CENTER
    // asm: 	STF	R0,@WHEELPOS
NOENTR2:
    // asm: CLRI	R0
    // asm: 	STI	R0,@NOSWAP
    // asm: 	CALL	OBJ_INIT
    // asm: 	CALL	CLEAR_MAP_PALS
    // asm: 	LDI	MATTR,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	@POSITION,R0
    // asm: 	CMPI	1,R0
    // asm: 	BNE	JAJD
    // asm: 	INCAUD	AUD_CREDITS
    // asm: 	INCAUD	AUD_WIN_DC
    // asm: 	INCAUD	AUD_TOTAL_FREEGAMES
JAJD:
    // asm: 	JSRP	VANITY_SUB
    // asm: 	LDI	-2,R0
    // asm: 	STI	R0,@_ATTR_MODE
    // asm: 	BU	SET_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLINTON_SHOW", 0, 0);
    UNIMPL();
}

void BLINK_FREEBE(void)
{
    // asm: 	SLEEP	5
    // asm: 	LDI	5*18,AR5
    // asm: BFLP1
    // asm: 	LDI	@_MODE,R4
    // asm: 	AND	MMODE,R4
    // asm: 	CMPI	MATTR,R4
    // asm: 	LDINE	70,R3
    // asm: 	LDIEQ	125,R3
    // asm: 	LDINE	256,R2
    // asm: 	LDIEQ	365,R2
    // asm: 	FLOAT	R2
    // asm: 	FLOAT	R3
    // asm: 	LDI	1,RC
    // asm: 	LDL	WINT1,AR2
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	CMPI	MATTR,R4
    // asm: 	LDINE	90,R3
    // asm: 	LDIEQ	150,R3
    // asm: 	LDINE	256,R2
    // asm: 	LDIEQ	365,R2
    // asm: 	FLOAT	R2
    // asm: 	FLOAT	R3
    // asm: 	LDI	1,RC
    // asm: 	LDL	WINT2,AR2
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // ;insert frame rate equalizer
    // ;
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,BFLP1
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLINK_FREEBE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
 */
void BACKUP_CAMERA(void)
{
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDI	*+AR0(CARTRAK),AR2
    // asm: 	LDI	*+AR2(OLINK4),AR0
    // asm: 	LDF	*+AR0(OPOSX),R2
    // asm: 	SUBF	*+AR2(OPOSX),R2
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	SUBF	*+AR2(OPOSZ),R3
    // asm: 	CALL	ARCTANF
    // asm: 	SUBF	HALFPI,R0
    // asm: 	LDF	R0,R2
    // asm: 	FLOAT	-7500,R0
    // asm: 	ADDF	@ZOOMD,R0
    // asm: 	CALL	DISTANCE_2D
    // asm: 	LDF	R0,R6
    // asm: 	LDF	R1,R7
    // asm: 	LDP	@_CAMERAPOS+X
    // asm: 	ADDF	@_CAMERAPOS+X,R6
    // asm: 	ADDF	@_CAMERAPOS+Z,R7
    // asm: 	SETDP
    // asm: 	LDI	30,AR5
    // asm: BACKLP
    // asm: 	LDP	@_CAMERAPOS+X
    // asm: 	LDF	@_CAMERAPOS+X,R0
    // asm: 	LDF	@_CAMERAPOS+Z,R1
    // asm: 	SUBF	R0,R6,R2
    // asm: 	SUBF	R1,R7,R3
    // asm: 	MPYF	0.02,R2
    // asm: 	MPYF	0.02,R3
    // asm: 	ADDF	R0,R2,R0
    // asm: 	ADDF	R1,R3,R1
    // asm: 	STF	R0,@_CAMERAPOS+X
    // asm: 	STF	R1,@_CAMERAPOS+Z
    // asm: 	LDF	@_CAMERAPOS+Y,R0
    // asm: 	SUBF	10,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	SETDP
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,BACKLP
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BACKUP_CAMERA", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TIMED_OUT(void)
{
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@DID_TIMED_OUT
    // asm: 	INCAUD	AUD_GAMES_EXPIRED
    // asm: 	LDI	@BONUS_WAVE,AR2
    // asm: 	ADDI	@BONUS_TABLEI,AR2
    // asm: 	LDI	*AR2,R0
    // asm: 	CALLU	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMED_OUT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void KILL_PLYR_SOUNDS(void)
{
    // asm: 	LDI	1000,AR2		;ENGINE RPM IDLE
    // asm: 	CALL	SENDSND
    // asm: 	LDI	SKIDB,AR2 		;KILL LOOPERS WHILE SUSPENDED
    // asm: 	CALL	KILLSNDFX
    // asm: 	LDI	SKIDC,AR2 		;KILL LOOPERS WHILE SUSPENDED
    // asm: 	CALL	KILLSNDFX
    // asm: 	LDI	BRAKSND,AR2
    // asm: 	CALL	KILLSNDFX
    // asm: 	LDI	TUNSND,AR2
    // asm: 	CALL	KILLSNDFX
    // asm: 	LDI	GRAVELA,AR2
    // asm: 	CALL	KILLSNDFX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILL_PLYR_SOUNDS", 0, 0);
    UNIMPL();
}

void CONGRAT_SPEECH(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void FREE_RACE_ANNOUNCE(void)
{
    // asm: 	SOND1	GETREADYTUNE
    // asm: 	LDI	CONGRATS,AR2
    // asm: 	CALL	RANDU0
    // asm: 	LDI	R0,AR2
    // asm: 	ADDI	@CONGRAT_SPEECHI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	ONESNDFX
    // asm: 	LDI	7,AR5
    // asm: FRAL1
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PRC_SUICIDE
    // asm: 	LDL	FRA1,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	100,R3
    // asm: 	LDI	15,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	SLEEP	9
    // asm: 	LDL	FRA2,AR2
    // asm: 	PUSH	AR2
    // asm: 	READAUD	ADJ_FREEGAME
    // asm: 	POP	AR2
    // asm: 	CMPI	0,R0
    // asm: 	BNE	ISFREE
    // asm: 	LDL	FRA1,AR2
ISFREE:
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	150,R3
    // asm: 	LDI	15,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	SLEEP	10
    // asm: 	DBU	AR5,FRAL1
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREE_RACE_ANNOUNCE", 0, 0);
    UNIMPL();
}

/* *
*
 */
void SHOWLEG_PROC(void)
{
    // asm: 	LDI	@BONUS_WAVE,AR2
    // asm: 	DEC	AR2
    // asm: 	ADDI	@LEG_NAMESI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	300,R3
    // asm: 	LDI	SHOWLEG_TIME,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	STI	AR0,*+AR7(PDATA)
    // asm: 	STI	AR1,*+AR7(PDATA+1)
    // asm: 	CALL	SET18FONTDS
    // asm: 	LDI	@LCTSI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	275,R3
    // asm: 	LDI	SHOWLEG_TIME,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	STI	AR0,*+AR7(PDATA+2)
    // asm: 	STI	AR1,*+AR7(PDATA+3)
    // asm: 	CALL	SET18FONTDS
    // asm: 	FLOAT	-100,R6
    // asm: 	LDI	20,AR5	;20
SLLP1:
    // asm: FLOAT	256,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	R0,R6
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	LDI	*+AR7(PDATA),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,SLLP1
    // 	;CENTER IT
    // 	;
    // asm: 	FLOAT	256,R6
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	LDI	*+AR7(PDATA),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	35
    // asm: 	LDI	20,AR5
SLLP1A:
    // asm: FLOAT	-100,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	R0,R6
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	LDI	*+AR7(PDATA),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,SLLP1A
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOWLEG_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SHOWNEXTLEG_PROC(void)
{
    // asm: 	SLEEP	20
    // asm: 	LDI	@BONUS_WAVE,AR2
    // asm: 	ADDI	@LEG_NAMESI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	50,R3
    // asm: 	LDI	SHOWLEG_TIME,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	STI	AR0,*+AR7(PDATA)
    // asm: 	STI	AR1,*+AR7(PDATA+1)
    // asm: 	CALL	SET18FONTDS
    // asm: 	LDI	@LNLSI,AR2
    // asm: 	FLOAT	256,R2
    // ;	FLOAT	275,R3
    // asm: 	FLOAT	25,R3
    // asm: 	LDI	SHOWLEG_TIME,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	STI	AR0,*+AR7(PDATA+2)
    // asm: 	STI	AR1,*+AR7(PDATA+3)
    // asm: 	CALL	SET18FONTDS
    // asm: 	FLOAT	600,R6
    // asm: 	LDI	16,AR5
SLLP2:
    // asm: FLOAT	256,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.25,R0
    // asm: 	ADDF	R0,R6
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	LDI	*+AR7(PDATA),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,SLLP2
    // 	;CENTER IT
    // 	;
    // asm: 	FLOAT	256,R6
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	LDI	*+AR7(PDATA),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	50
    // asm: 	LDI	215,AR5
SLLP2A:
    // asm: FLOAT	600,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.25,R0
    // asm: 	ADDF	R0,R6
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	LDI	*+AR7(PDATA),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,SLLP2A
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOWNEXTLEG_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	R4	INDEX
*
 */
void PLACE_FLAG(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR4
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	RC
    // asm: 	LDI	R4,AR0
    // asm: 	MPYI	2,AR0
    // asm: 	ADDI	@FLAG_POS_TABLEI,AR0
    // asm: 	LDI	*AR0,R2
    // asm: 	SUBI	256,R2
    // asm: 	LDI	*+AR0,R3
    // asm: 	SUBI	200-20,R3
    // asm: 	LDI	367,RC
    // asm: 	LDL	star,AR2
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDIL	O_3DROT,R0
    // asm: 	OR	O_NOUROT|O_NOUNIV,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTHP
    // asm: 	LDI	AR0,AR4
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OVELY)
    // asm: 	LDF	100,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	FLOAT	-230,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CREATEC	PLACE_FLAG_PROC,UTIL_C
    // asm: 	SONDFX	MAPSTAR2
    // asm: 	POP	RC
    // asm: 	POP	RC
    // asm: 	POP	R2
    // asm: 	POP	AR4
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLACE_FLAG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	AR4	OBJECT
*	AR4	*+(OVEL) POSITION TO ACHIEVE
*
*
 */
void PLACE_FLAG_PROC(void)
{
    // asm: 	LDI	12,AR5
    // asm: PFPLP
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	SUBF	*+AR4(OPOSX),R0
    // asm: 	MPYF	0.25,R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OVELY),R1
    // asm: 	SUBF	*+AR4(OPOSY),R1
    // asm: 	MPYF	0.25,R1
    // asm: 	ADDF	*+AR4(OPOSY),R1
    // asm: 	STF	R1,*+AR4(OPOSY)
    // asm: 	MPYF	R1,R1
    // asm: 	MPYF	R0,R0
    // asm: 	ADDF	R1,R0
    // asm: 	CMPF	25,R0
    // asm: 	BLT	ALLDN7
    // asm: 	LDF	*+AR4(ORADZ),R2
    // asm: 	ADDF	0.1,R2
    // asm: 	STF	R2,*+AR4(ORADZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,PFPLP
ALLDN7:
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OVELY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLACE_FLAG_PROC", 0, 0);
    UNIMPL();
}

void BONS_MAXMPH(void)
{
    // asm: 	LDL	BT1,R6
    // asm: 	LDL	BT1A,R7
    // asm: 	READAUD	ADJ_MPHORKPM
    // asm: 	CMPI	0,R0
    // asm: 	LDIEQ	R6,AR2
    // asm: 	LDINE	R7,AR2
    // asm: 	FLOAT	0,R2
    // asm: 	FLOAT	20,R3
    // asm: 	LDI	999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	STI	AR0,*+AR7(PDATA+10)	;SHADOW
    // asm: 	STI	AR1,*+AR7(PDATA+11)	;ACTUAL
    // asm: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm: 	OR	TXT_RIGHT,R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm: 	OR	TXT_RIGHT,R0
    // asm: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm: 	READAUD	ADJ_MPHORKPM
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	ISMPHT
    // asm: 	LDF	@MAXMPH,R0
    // asm: 	MPYF	1.6666,R0
    // asm: 	BU	KJL
ISMPHT:
    // asm: LDF	@MAXMPH,R0
KJL:
    // asm: LDF	@MAXMPH_COUNT,R1
    // asm: 	CALL	DIV_F
    // asm: 	FIX	R0,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	PDATA,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	512,R2
    // asm: 	FLOAT	20,R3
    // asm: 	LDI	999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SETLGDIGITFONTDS
    // asm: 	STI	AR0,*+AR7(PDATA+12)
    // asm: 	STI	AR1,*+AR7(PDATA+13)
    // asm: 	LDL	dnums_amber,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,@MAXMPH
    // asm: 	STF	R0,@MAXMPH_COUNT
ENTER_HERE:
    // asm: 	LDI	31,AR5
    // asm: 	CLRF	R6		;FIXED STF
    // asm: 	FLOAT	512,R7		;FLT STF
    // asm: BML
    // asm: 	FLOAT	250,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.1,R0
    // asm: 	CMPF	8,R0
    // asm: 	LDFLT	8,R0
    // asm: 	ADDF	R0,R6
    // asm: 	FLOAT	250,R0
    // asm: 	CMPF	R0,R6
    // asm: 	LDFGT	R0,R6
    // asm: 	LDI	*+AR7(PDATA+10),AR0
    // asm: 	LDI	*+AR7(PDATA+11),AR1
    // asm: 	STF	R6,*+AR1(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	FLOAT	260,R0
    // asm: 	SUBF	R7,R0
    // asm: 	MPYF	0.20,R0
    // asm: 	CMPF	-12,R0
    // asm: 	LDFLT	-12,R0
    // asm: 	ADDF	R0,R7
    // asm: 	FLOAT	260,R0
    // asm: 	CMPF	R0,R7
    // asm: 	LDFLT	R0,R7
    // asm: 	LDI	*+AR7(PDATA+12),AR0
    // asm: 	LDI	*+AR7(PDATA+13),AR1
    // asm: 	STF	R7,*+AR1(TEXT_POSX)
    // asm: 	LDF	R7,R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,BML
    // asm: 	FLOAT	250,R0
    // asm: 	FLOAT	253,R1
    // asm: 	FLOAT	260,R2
    // asm: 	FLOAT	263,R3
    // asm: 	LDI	*+AR7(PDATA+10),AR0
    // asm: 	STF	R1,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+11),AR0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+12),AR0
    // asm: 	STF	R3,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+13),AR0
    // asm: 	STF	R2,*+AR0(TEXT_POSX)
KKL:
    // asm: SLEEP	1
    // asm: 	LDI	@DO_FOLDFLAG,R0
    // asm: 	BZ	KKL
    // asm: 	SONDFX	WIPE2
    // asm: 	LDI	31,AR5
    // asm: 	FLOAT	250,R6		;FIXED STF
    // asm: 	FLOAT	260,R7		;FLT STF
    // asm: BML2
    // asm: 	FLOAT	-20,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.1,R0
    // asm: 	ADDF	R0,R6
    // asm: 	LDI	*+AR7(PDATA+10),AR0
    // asm: 	LDI	*+AR7(PDATA+11),AR1
    // asm: 	STF	R6,*+AR1(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	FLOAT	532,R0
    // asm: 	SUBF	R7,R0
    // asm: 	MPYF	0.1,R0
    // asm: 	ADDF	R0,R7
    // asm: 	LDI	*+AR7(PDATA+12),AR0
    // asm: 	LDI	*+AR7(PDATA+13),AR1
    // asm: 	STF	R7,*+AR1(TEXT_POSX)
    // asm: 	LDF	R7,R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,BML2
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_MAXMPH", 0, 0);
    UNIMPL();
}

void BONS_RECORDTIME(void)
{
    // asm: 	SLEEP	5
    // asm: 	LDL	RT2,AR2
    // asm: 	FLOAT	0,R2
    // asm: 	FLOAT	120,R3
    // asm: 	LDI	999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SET18FONTDS
    // asm: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm: 	OR	TXT_RIGHT,R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm: 	OR	TXT_RIGHT,R0
    // asm: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm: 	STI	AR0,*+AR7(PDATA+10)
    // asm: 	STI	AR1,*+AR7(PDATA+11)
    // asm: 	LDI	@BONUS_WAVE,R6
    // asm: 	DEC	R6
    // asm: 	LDI	0,R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	CALL	TABLE_ENTRY_READ
    // 	;
    // 	;R0	time code
    // 	;R1	init 1
    // 	;R2	init 2
    // 	;R3	init 3
    // 	;
    // asm: 	LDI	AR7,AR1
    // asm: 	ADDI	PDATA+14,AR1
    // asm: 	AND	0FFh,R1
    // asm: 	AND	0FFh,R2
    // asm: 	AND	0FFh,R3
    // asm: 	LS	8,R2
    // asm: 	OR	R2,R1
    // asm: 	LS	16,R3
    // asm: 	OR	R3,R1
    // asm: 	LDI	' ',R2
    // asm: 	LS	24,R2
    // asm: 	OR	R2,R1
    // asm: 	STI	R1,*AR1
    // asm: 	LDI	' ',R2
    // asm: 	STI	R2,*+AR1
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	PDATA,AR2
    // asm: 	CALL	TIME2STR
    // asm: 	LDI	AR1,AR0
    // asm: 	LDI	AR2,AR1
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	STRCAT		;AR1 from above
    // asm: 	FLOAT	512,R2
    // asm: 	FLOAT	120,R3
    // asm: 	LDI	999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SET18FONTDS
    // asm: 	STI	AR0,*+AR7(PDATA+12)
    // asm: 	STI	AR1,*+AR7(PDATA+13)
    // asm: 	BU	ENTER_HERE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_RECORDTIME", 0, 0);
    UNIMPL();
}

void BONS_HOTTIME_REC(void)
{
    // asm: 	CLRI	R4			;ON OFF TOGGLE
    // asm: 	SLEEP	5
    // asm: 	LDL	HH1,AR2
    // asm: 	BU	BLAHB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_HOTTIME_REC", 0, 0);
    UNIMPL();
}

void BONS_HOTTIME(void)
{
    // asm: 	CLRI	R4			;ON OFF TOGGLE
    // asm: 	SLEEP	5
    // asm: 	LDL	HH2,AR2
    // asm: BLAHB	STI	AR2,*+AR7(PDATA)
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	(150+300),R3
    // asm: 	LDI	999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm: 	OR	TXT_CENTER,R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm: 	OR	TXT_CENTER,R0
    // asm: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	AR1,AR5
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR4(TEXT_PAL)
    // asm: 	STI	R0,*+AR5(TEXT_PAL)
BONHTLP:
    // asm: 	LDF	*+AR4(TEXT_POSY),R0
    // asm: 	FLOAT	152,R1
    // asm: 	LDF	R1,R3
    // asm: 	SUBF	R0,R1,R1
    // asm: 	MPYF	0.1,R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	R3,R1
    // asm: 	LDFLT	R3,R1
    // asm: 	STF	R1,*+AR4(TEXT_POSY)
    // asm: 	LDF	*+AR5(TEXT_POSY),R0
    // asm: 	FLOAT	150,R1
    // asm: 	LDF	R1,R3
    // asm: 	SUBF	R0,R1,R1
    // asm: 	MPYF	0.1,R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	R3,R1
    // asm: 	LDFLT	R3,R1
    // asm: 	STF	R1,*+AR5(TEXT_POSY)
    // asm: 	CALL	TEXTTOG
    // asm: 	SLEEP	1
    // asm: 	LDI	@DO_FOLDFLAG,R0
    // asm: 	BZ	BONHTLP
BONHTLP2:
    // asm: 	LDF	*+AR4(TEXT_POSY),R0
    // asm: 	FLOAT	152+300,R1
    // asm: 	LDF	R1,R3
    // asm: 	SUBF	R0,R1,R1
    // asm: 	MPYF	0.1,R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	R3,R1
    // asm: 	LDFGT	R3,R1
    // asm: 	STF	R1,*+AR4(TEXT_POSY)
    // asm: 	LDF	*+AR5(TEXT_POSY),R0
    // asm: 	FLOAT	150+300,R1
    // asm: 	LDF	R1,R3
    // asm: 	SUBF	R0,R1,R1
    // asm: 	MPYF	0.1,R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	R3,R1
    // asm: 	LDFGT	R3,R1
    // asm: 	STF	R1,*+AR5(TEXT_POSY)
    // asm: 	SLEEP	1
    // asm: 	BU	BONHTLP2
    // asm: BADA	LDI	1,R0
    // asm: 	STI	R0,*+AR4(TEXT_TIKS)
    // asm: 	STI	R0,*+AR5(TEXT_TIKS)
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_HOTTIME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TEXTTOG(void)
{
    // asm: 	ADDI	@NFRAMES,R4
    // asm: 	CMPI	30,R4
    // asm: 	BLT	ISOFF
    // asm: 	CMPI	35,R4
    // asm: 	BLT	GAGA
    // asm: 	CLRI	R4
GAGA:
    // asm: 	LDL	NULLSTR5,R0
    // asm: 	BU	IBOIBO
ISOFF:
    // asm: LDI	*+AR7(PDATA),R0
IBOIBO:
    // asm: STI	R0,*+AR4(TEXT_PTR)
    // asm: 	STI	R0,*+AR5(TEXT_PTR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXTTOG", 0, 0);
    UNIMPL();
}

void BONS_ETIME(void)
{
    // asm: 	SLEEP	5
    // asm: 	LDL	BT2,AR2
    // asm: 	FLOAT	0,R2
    // asm: 	FLOAT	50,R3
    // asm: 	LDI	999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SET18FONTDS
    // asm: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm: 	OR	TXT_RIGHT,R0
    // asm: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm: 	OR	TXT_RIGHT,R0
    // asm: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm: 	STI	AR0,*+AR7(PDATA+10)
    // asm: 	STI	AR1,*+AR7(PDATA+11)
    // asm: 	LDI	@DID_TIMED_OUT,R0
    // asm: 	BZ	DOREG3
    // asm: 	LDI	0,R0		;ELAPSED TIME OF 0 = DID NOT FINISH
    // asm: 	STI	R0,@ETIME
    // asm: 	LDL	EXPIRED,AR2
    // asm: 	BU	LREG3
DOREG3:
    // asm: LDI	@STOPWATCH,R0
    // asm: 	STI	R0,@ETIME
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	PDATA,AR2
    // asm: 	CALL	TIME2STR
LREG3:
    // asm: 	FLOAT	512,R2
    // asm: 	FLOAT	50,R3
    // asm: 	LDI	999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SET18FONTDS
    // asm: 	STI	AR0,*+AR7(PDATA+12)
    // asm: 	STI	AR1,*+AR7(PDATA+13)
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@STOPWATCH
    // asm: 	BU	ENTER_HERE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_ETIME", 0, 0);
    UNIMPL();
}

void BONS_POSITION(void)
{
    // asm: 	LDI	@POSITION,AR4
    // asm: 	DEC	AR4
    // asm: 	MPYI	3,AR4
    // asm: 	ADDI	@POS_TABLEI,AR4
    // asm: 	LDI	-60,R5		;YPOS
    // asm: 	LDI	40,AR6
    // asm: BPL1
    // asm: 	FLOAT	R5,R0
    // asm: 	LDF	R0,R1
    // asm: 	FLOAT	138-60,R2
    // asm: 	SUBRF	R2,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	R1,R0
    // asm: 	FIX	R0,R5
    // asm: 	LDI	*AR4,AR2
    // asm: 	LDI	220,R2
    // asm: 	ADDI	*+AR4(2),R2
    // asm: 	LDI	R5,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	LDI	*+AR4(1),AR2
    // asm: 	LDI	220+35,R2
    // asm: 	LDI	R5,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	SLEEP	1
    // asm: 	DBU	AR6,BPL1
    // asm: 	LDI	158,AR6
BPL2:
    // asm: 	LDI	*AR4,AR2
    // asm: 	LDI	220,R2
    // asm: 	ADDI	*+AR4(2),R2
    // asm: 	LDI	R5,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	LDI	*+AR4(1),AR2
    // asm: 	LDI	220+35,R2
    // asm: 	LDI	R5,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	SLEEP	1
    // asm: 	LDI	@DO_FOLDFLAG,R0
    // asm: 	BZ	BPL2
    // asm: 	LDI	40,AR6
    // asm: BPL3
    // asm: 	FLOAT	R5,R0
    // asm: 	LDF	R0,R1
    // asm: 	SUBRF	-60,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	R1,R0
    // asm: 	FIX	R0,R5
    // asm: 	LDI	*AR4,AR2
    // asm: 	LDI	220,R2
    // asm: 	ADDI	*+AR4(2),R2
    // asm: 	LDI	R5,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	LDI	*+AR4(1),AR2
    // asm: 	LDI	220+35,R2
    // asm: 	LDI	R5,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	SLEEP	1
    // asm: 	DBU	AR6,BPL3
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_POSITION", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void KILL_THEM(void)
{
    // 	;KILL ALL DRONES
    // asm: 	LDI	DRONE_C,R0
    // asm: 	LDI	CLASS_M,R1
    // asm: 	CALL	PRC_KILLALL
    // 	;TRAFFIC
    // asm: 	LDI	SPAWNER_C,R0
    // asm: 	LDI	CLASS_M,R1
    // asm: 	CALL	PRC_KILLALL
    // 	;WAVEFLAG, MONKEYs
    // asm: 	LDI	UTIL_C|MONKEY_T,R0
    // asm: 	LDI	CLASS_M|TYPE_M,R1
    // asm: 	CALL	PRC_KILLALL
    // 	;LBACK_WATCH
    // asm: 	LDI	UTIL_C|BACKGRND_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	CALL	DELETE_SPLAT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILL_THEM", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FIND_AND_REACTIVATE(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR4
    // asm: 	LDI	@OACTIVE,AR0
FARLP:
    // asm: LDI	*AR0,R0
    // asm: 	BZ	FARX
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_DUDE_S,R0
    // asm: 	BNE	NOTRUT
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	RUT_ANI
    // asm: 	BU	FARLP
NOTRUT:
    // asm: CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_BABE_S,R0
    // asm: 	BNE	FARLP
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	HUNGH_ANI_REENTER
    // asm: 	BU	FARLP
FARX:
    // asm: 	POP	AR4
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_AND_REACTIVATE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void KILL_THE_REANIMATORS(void)
{
    // asm: 	LDI	SPAWNER_C|ANIMATION_T|7,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILL_THE_REANIMATORS", 0, 0);
    UNIMPL();
}

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
#include "discovered_labels.h"

/*
 * Source module: asm/BONUS.ASM
 */

void BONUS_SANFRAN(void);
void BONUS_GGATE(void);
void BONUS_BEVHILLS(void);
void BONUSNULL(void);
void BONUS14(void);
void BONUS13(void);
void BONUS12(void);
void BONUS11(void);
void BONUS10(void);
void BONUS9(void);
void BONUS8(void);
void BONUS7(void);
void BONUS6(void);
void BONUS5(void);
void BONUS4(void);
void BONUS3(void);
void BONUS2(void);
void BONUS1(void);
void JSUB(void);
void DISPLAY_H2H_WINNER(void);
void DODIEXXX(void);
void OBJ_DELETE_HIGH_PRIORITY(void);
void BONUS_SCREEN(void);
void BONSCRN2(void);
void PAPAPA44(void);
void ISREC(void);
void NOT1ST(void);
void CLINTON_SHOW(void);
void BLINK_FREEBE(void);
void BACKUP_CAMERA(void);
void TIMED_OUT(void);
void KILL_PLYR_SOUNDS(void);
void CONGRAT_SPEECH(void);
void FREE_RACE_ANNOUNCE(void);
void SHOWLEG_PROC(void);
void SHOWNEXTLEG_PROC(void);
void PLACE_FLAG(void);
void PLACE_FLAG_PROC(void);
void BONS_MAXMPH(void);
void ISMPHT(void);
void BONS_RECORDTIME(void);
void BONS_HOTTIME_REC(void);
void BONS_HOTTIME(void);
void TEXTTOG(void);
void ISOFF(void);
void BONS_ETIME(void);
void DOREG3(void);
void BONS_POSITION(void);
void KILL_THEM(void);
void FIND_AND_REACTIVATE(void);
void NOTRUT(void);
void FARX(void);
void KILL_THE_REANIMATORS(void);

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
const char *LEG_NAMES[] = {
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
void (*BONUS_POSTLAUNCH[])(void) = {
    BONUS_GGATE, BONUS_SANFRAN, BONUSNULL, BONUSNULL,
    BONUS_BEVHILLS, BONUSNULL, BONUSNULL,
    BONUSNULL, BONUSNULL, BONUSNULL,
    BONUSNULL, BONUSNULL, BONUSNULL,
    BONUSNULL, BONUSNULL, BONUSNULL,
};
/* asm: BONUS_TABLE	.word	BONUS1,BONUS2,BONUS3,BONUS4,BONUS5 */
/* asm: 	.word	BONUS6,BONUS7,BONUS8,BONUS9,BONUS10 */
/* asm: 	.word	BONUS11,BONUS12,BONUS13,BONUS14 */
void (*BONUS_TABLE[])(void) = {
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
/* asm: SPEEDHIT	.bss	SPEEDHIT,1 */
int SPEEDHIT;
/* asm: WAS_HEAD2HEAD_ON	.bss	WAS_HEAD2HEAD_ON,1 */
int WAS_HEAD2HEAD_ON;
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
int FLAG_POS_TABLE[] = {
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
    // asm 00003966: 	STI	R0,@_MODE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS_BEVHILLS", 0, 0);
    UNIMPL();
}

void BONUSNULL(void)
{
    // asm 0000396A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUSNULL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ROUTINES FOR OVERLAY.ASM
*
*
 */
void BONUS14(void)
{
    // asm: 	LDI	14,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS14", 0, 0);
    UNIMPL();
}

void BONUS13(void)
{
    // asm: LDI	L_LEG14_BEGIN+1,R0
    // asm: 	LDI	13,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS13", 0, 0);
    UNIMPL();
}

void BONUS12(void)
{
    // asm 00003970: LDI	L_LEG13_BEGIN+1,R0
    // asm 00003970: 	LDI	12,R1
    // asm 00003971: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS12", 0, 0);
    UNIMPL();
}

void BONUS11(void)
{
    // asm 00003973: LDI	L_LEG12_BEGIN+1,R0
    // asm 00003974: 	LDI	11,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS11", 0, 0);
    UNIMPL();
}

void BONUS10(void)
{
    // asm 00003976: LDI	L_LEG11_BEGIN+1,R0
    // asm 00003977: 	LDI	10,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS10", 0, 0);
    UNIMPL();
}

void BONUS9(void)
{
    // asm 00003979: LDI	L_LEG10_BEGIN+1,R0
    // asm 0000397A: 	LDI	9,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS9", 0, 0);
    UNIMPL();
}

void BONUS8(void)
{
    // asm 0000397C: LDI	L_LEG9_BEGIN+1,R0
    // asm 0000397D: 	LDI	8,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS8", 0, 0);
    UNIMPL();
}

void BONUS7(void)
{
    // asm 0000397F: LDI	L_LEG8_BEGIN+1,R0
    // asm 00003980: 	LDI	7,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS7", 0, 0);
    UNIMPL();
}

void BONUS6(void)
{
    // asm 00003982: LDI	L_LEG7_BEGIN+1,R0
    // asm 00003983: 	LDI	6,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS6", 0, 0);
    UNIMPL();
}

void BONUS5(void)
{
    // asm 00003985: LDI	L_LEG6_BEGIN+1,R0
    // asm 00003986: 	LDI	5,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS5", 0, 0);
    UNIMPL();
}

void BONUS4(void)
{
    // asm 00003988: LDI	L_LEG5_BEGIN+1,R0
    // asm 00003989: 	LDI	4,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS4", 0, 0);
    UNIMPL();
}

void BONUS3(void)
{
    // asm 0000398B: LDI	L_LEG4_BEGIN+1,R0
    // asm 0000398C: 	LDI	3,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS3", 0, 0);
    UNIMPL();
}

void BONUS2(void)
{
    // asm 0000398E: LDI	L_LEG3_BEGIN+1,R0
    // asm 0000398F: 	LDI	2,R1
    // asm: 	BU	LK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS2", 0, 0);
    UNIMPL();
}

void BONUS1(void)
{
    // asm 00003991: LDI	L_LEG2_BEGIN+1,R0
    // asm 00003992: 	LDI	1,R1
LK:
    // asm 00003993: STI	R0,@NEXT_STARTUP
    // asm 00003993: 	STI	R1,@BONUS_WAVE
    // asm 00003995: 	LDI	@_MODE,R0
    // asm: 	LDI	R0,R1
    // asm 00003996: 	AND	MMODE,R1
    // asm 00003997: 	CMPI	MGAME,R1
    // asm 00003998: 	RETSNE
    // asm: 	ANDN	MMODE,R0
    // asm 0000399A: 	OR	MBONUS,R0
    // asm 0000399B: 	ANDN	MINTUNNEL,R0
    // asm 0000399C: 	STI	R0,@_MODE
    // 	;;;	COMMUNICATIONS ALCHEMY
    // 	;;;
    // asm 000039A1: 	LDI	@MY_STATE,R0
    // asm 000039A2: 	OR	OMS_FINISHLINE,R0
    // asm: 	STI	R0,@MY_STATE
    // 	;DIFFICULTY ADJUSTMENTS
    // 	;
    // asm 000039A3: 	LDI	@POSITION,R0
    // asm 000039A4: 	CALL	DIFF_CHANGE
    // 	;AUDIT MUMBO JUMBO
    // 	;
    // asm: 	LDI	@BONUS_WAVE,R2
    // asm 000039A6: 	SETAUD	AUD_LAST_LEG
    // asm: 	LDI	@BONUS_WAVE,AR2
    // asm: 	DEC	AR2
    // asm: 	MPYI	2,AR2
    // asm: 	ADDI	AUD_FINISH_GGATE,AR2
    // asm 000039A8: 	CALL	AUDIT_INC
    // asm: 	CLRI	R0
    // asm 000039AB: 	STI	R0,@FIRST_RACE
    // asm 000039AE: 	CALL	KILL_PLYR_SOUNDS
    // asm: 	LDI	MAX_DRONES,R0
    // asm 000039B0: 	STI	R0,@DD_MAX_DRONES
    // asm: 	READAUD	ADJ_CHECKPOINT_BONUS
    // asm: 	STI	R0,@CHECKPOINT_TIME_BONUS
    // asm 000039B3: 	LDI	@PLYCAR,AR4
    // asm 000039B4: 	LDI	@PLYCBLK,AR5
    // asm: 	CALL	FIND_PLAYERS_POSITION
    // asm 000039B7: 	CREATEC	BONUS_SCREEN,22
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS1", 0, 0);
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
    // asm 000039BF: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 000039C0: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 000039C1: 	OR	TXT_CENTER,R0
    // asm 000039C2: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm 000039C3: 	LDL	font18_white,AR2
    // asm 000039C4: 	CALL	PAL_FIND_RAW
    // asm 000039C5: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 000039C6: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 000039C7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JSUB", 0, 0);
    UNIMPL();
}

void DISPLAY_H2H_WINNER(void)
{
    // asm 000039CA: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm 000039CB: 	BZ	DODIE
    // asm 000039CD: 	LDI	@OM_POSITION,R0
    // asm 000039CE: 	CMPI	@POSITION,R0
    // asm: 	BLT	DODIE
    // asm 000039CF: 	CLRI	R0
    // asm 000039D0: 	STI	R0,@UNFOLDFLAG
    // asm: 	LDL	big2,AR2
    // asm 000039D4: 	LDI	0,R2
    // asm 000039D5: 	LDI	140,R3
    // asm: 	LDI	856,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm 000039D6: 	STI	R0,*+AR0(OFLAGS)
    // asm 000039D7: 	LDI	AR0,AR2
    // asm 000039D8: 	CALL	OBJ_INSERTHP
    // asm 000039DA: 	LDL	H2HPAL3,AR2
    // asm 000039DB: 	CALL	PAL_FIND_RAW
    // asm 000039DC: 	STI	R0,*+AR0(OPAL)
    // asm 000039DD: 	STI	AR0,*+AR7(PDATA+2)	;big2
    // asm: 	LDL	redhd1,AR2
    // asm 000039E0: 	LDI	0,R2
    // asm 000039E1: 	LDI	80,R3
    // asm 000039E2: 	LDI	868,RC
    // asm 000039E3: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm 000039E4: 	STI	R0,*+AR0(OFLAGS)
    // asm 000039E6: 	LDI	AR0,AR2
    // asm 000039E7: 	CALL	OBJ_INSERTHP
    // asm 000039E9: 	LDL	H2HPAL2,AR2
    // asm 000039EA: 	CALL	PAL_FIND_RAW
    // asm 000039EB: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	AR0,*+AR7(PDATA)	;red (bottom)
    // asm 000039EE: 	LDL	yelhd1,AR2
    // asm 000039EF: 	LDI	0,R2
    // asm 000039F0: 	LDI	200,R3
    // asm 000039F1: 	LDI	868,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm 000039F2: 	STI	R0,*+AR0(OFLAGS)
    // asm 000039F3: 	LDI	AR0,AR2
    // asm 000039F4: 	CALL	OBJ_INSERTHP
    // asm 000039F5: 	LDL	H2HPAL2,AR2
    // asm 000039F6: 	CALL	PAL_FIND_RAW
    // asm 000039F7: 	STI	R0,*+AR0(OPAL)
    // asm 000039F8: 	STI	AR0,*+AR7(PDATA+1)	;yellow (bottom)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_H2H_WINNER", 0, 0);
    UNIMPL();
}

void DODIEXXX(void)
{
    // asm 00003A22: 	LDI	*+AR7(PDATA),AR2
    // asm 00003A23: 	CALL	OBJ_DELETE_HIGH_PRIORITY
    // asm: 	LDI	*+AR7(PDATA+1),AR2
    // asm 00003A24: 	CALL	OBJ_DELETE_HIGH_PRIORITY
    // asm: 	LDI	*+AR7(PDATA+2),AR2
    // asm: 	CALL	OBJ_DELETE_HIGH_PRIORITY
DODIE:
    // asm 00003A26: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DODIEXXX", 0, 0);
    UNIMPL();
}

void OBJ_DELETE_HIGH_PRIORITY(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm 00003A2B: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	LDI	@OHIGH_PRIORITYI,R1
DELLP:
    // asm 00003A2D: LDI	R1,AR1		;WE MUST FIND DEAD OBJECT TO LINK AROUND
    // asm 00003A2E: 	LDI	*AR1,R1
    // asm 00003A2F: 	BZ	DELOBJX
    // asm: 	CMPI	R1,AR2
    // asm 00003A30: 	BNE	DELLP
    // asm 00003A31: 	LDI	*AR2,R1
    // asm 00003A32: 	STI	R1,*AR1			;LINK AROUND
    // asm 00003A33: 	LDI	@OFREE,R1
    // asm: 	STI	R1,*AR2
    // asm 00003A34: 	STI	AR2,@OFREE
    // asm: 	LDI	@OFREECNT,R0		;INCREMENT FREE OBJECT COUNT
    // asm 00003A36: 	ADDI	1,R0
    // asm 00003A37: 	STI	R0,@OFREECNT
    // asm 00003A39: 	CLRI	R0
    // asm 00003A3A: 	STI	R0,*+AR2(OLINK2)	;CLEAR SEARCH ID
    // asm: 	STI	R0,*+AR2(OFLAGS)
DELOBJX:
    // asm 00003A3D: 	POP	AR2
    // asm: 	POP	AR1
    // asm 00003A3E: 	POP	R1
    // asm 00003A3F: 	POP	R0
    // asm 00003A40: 	RETS
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
    // asm 00003A46: 	STI	R0,@BGNDCOLA
    // asm 00003A47: 	CALL	SILENT
    // asm: 	CALL	SND_RESET_QUIET
    // asm 00003A49: 	CLRI	R0
    // asm: 	STI	R0,@STOPWATCH_CNTL
    // asm 00003A4A: 	STI	R0,@DO_FOLDFLAG
    // asm 00003A4C: 	CALL	KILL_THEM
    // asm 00003A4D: 	LDI	0,R0			;SMOKE MAY BE KILLED, SO I HAVE TO RESET THIS
    // asm: 	STI	R0,@TIRE_SMOKE_COUNT
    // asm 00003A4F: 	LDI	1,R0
    // asm 00003A50: 	STI	R0,@IGNORE_UPDATES
    // asm: 	CALL	PRC_INIT
    // asm 00003A51: 	CREATE	BONSCRN2,UTIL_C
    // asm 00003A52: 	CREATE	BONUS_WAIT_LOOP,PLYR_C|PLYR1_T
    // asm 00003A53: 	CREATE	SCAN_OBJECTS,UTIL_C
    // asm 00003A54: 	CALL	FIND_AND_REACTIVATE	;REACTIVATE CHEERING ANIMATIONS
#if DEBUG
    // asm 00003A57: 	CALL	VERIFY_CODE_INTEGRITY
#endif
    // asm:         LDP     @FASTSTKI		;GET PAGE OF STORED ADDRESS
    // asm 00003A60:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm: 	BR	COLD_ENTER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS_SCREEN", 0, 0);
    UNIMPL();
}

/* *
*
*
 */
void BONSCRN2(void)
{
    // asm: 	LDI	@DID_TIMED_OUT,R0
    // asm: 	BZ	DOREG3A
    // asm: 	LDI	0,R0		;ELAPSED TIME OF 0 = DID NOT FINISH
    // asm: 	STI	R0,@ETIME
DOREG3A:
    // asm 00003A64: LDI	@STOPWATCH,R0
    // asm: 	STI	R0,@ETIME
    // *ELP CHANGE
    // 	;TURN ON LINK HERE... ALL LISTEN
    // 	;
    // 	;
    // asm 00003A68: 	CALL	CLRONE		;CAN NOW BE DUAL PLAYER
    // *ELP END CHANGE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_HIT	;----------------------------------
    // asm: 	CREATE	MOVEOUT_HUD_EQUIP,UTIL_C
    // 	;Wait for sound board to reset
    // 	;
CNR_ENTER:
    // asm: 	LDP	@IN_RESET_MODE
    // asm: 	LDI	@IN_RESET_MODE,R0
    // asm 00003A6D: 	SETDP
    // asm: 	BZ	CONTINUE
    // asm: 	SLEEP	1
    // asm: 	BU	CNR_ENTER
CONTINUE:
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONSCRN2", 0, 0);
    UNIMPL();
}

void PAPAPA44(void)
{
    // asm 00003A81: 	CREATE	BACKUP_CAMERA,UTIL_C|BONUS_SCREEN_T
    // asm: 	SONDFX	CROWD1
    // ;	LDI	@DID_TIMED_OUT,R0
    // ;	B
    // asm 00003A85: 	SONDFX	CROWDROAR
    // asm: 	SOND1	CHICKCHEER		;CHAN3
    // asm 00003A86: JJDDHH
    // asm: 	LDI	@POSITION,R0
    // asm: 	CMPI	1,R0
    // asm 00003A8B: 	BNE	COOLRET
    // asm: 	CREATEC	FREE_RACE_ANNOUNCE,UTIL_C
    // asm: 	JSRP	BABE_TROPHY
COOLRET:
    // 	;LAME TEXT EFFECTS
    // 	;
    // 	;
    // asm 00003A95: 	LDI	AR7,AR5
    // asm: 	CREATE	BONS_MAXMPH,UTIL_C|TEXTP_T
    // asm: 	CREATE	BONS_ETIME,UTIL_C|TEXTP_T
    // asm: 	CREATE	BONS_POSITION,UTIL_C|TEXTP_T
    // asm: 	CREATE	BONS_RECORDTIME,UTIL_C|TEXTP_T
    // asm: 	CREATE	DISPLAY_H2H_WINNER,UTIL_C|TEXTP_T|3
    // asm 00003A9B: 	LDI	@DID_TIMED_OUT,R0
    // asm 00003A9C: 	BNZ	NODOHOTTIME
    // asm 00003AA2: 	CALL	INTO_TABLE_P
    // asm 00003AA5: 	BNC	NODOHOTTIME
    // asm 00003AA8: 	CMPI	0,R0
    // asm: 	BNE	ISREC
    // asm 00003AAB: 	CREATE	BONS_HOTTIME_REC,UTIL_C|TEXTP_T
    // asm 00003AAC: 	BU	NODOHOTTIME
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PAPAPA44", 0, 0);
    UNIMPL();
}

void ISREC(void)
{
    // asm 00003AAD: CREATE	BONS_HOTTIME,UTIL_C|TEXTP_T
NODOHOTTIME:
    // 	;insert show best times here!
    // 	;
    // 	;
    // asm 00003AB5: 	SLEEP	5
    // asm: 	LDI	@POSITION,R0
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	JJAG
    // asm: 	LDI	0,R0
    // asm: 	LDI	100,R1
    // asm 00003AB8: 	CALL	SET_TRACK_VOL
    // asm 00003ABA: 	SOND1	MAPTUNE
JJAG:
    // asm: 	SONDFX	CROWD1
    // asm 00003ABE: 	LDI	120-1,AR5
    // asm 00003ABF: PAPA45
    // asm 00003AC0: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PAPA45X
    // asm 00003AC2: 	SUBI	@NFRAMES,AR5
    // asm: 	CMPI	0,AR5
    // asm 00003AC4: 	LDILT	0,AR5
    // asm: 	SLEEP	1
    // asm 00003AC5: 	DBU	AR5,PAPA45
PAPA45X:
    // asm 00003AC7: 	CREATE	UNFOLDMAP,UTIL_C
    // asm 00003AC8: 	SLEEP	10
    // asm 00003AC9: 	LDI	1,R0
    // asm 00003ACA: 	STI	R0,@UNFOLDFLAG
    // asm 00003ACC: 	SLEEP	30
    // asm: 	CALL	LOAD_FIXED_PALETTES
    // ;THIS RELOADS THE PALETTES THAT WERE USED FOR THE BABES
    // asm 00003AD4: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm 00003AD6: 	LDI	@BABE_PALISTI,AR5
    // asm: 	LDI	4,AR6
    // asm 00003AD7: FIXPALLP
    // asm: 	LDI	*+AR5,AR2
    // asm: 	CALL	PAL_FIND
    // asm 00003AD7: 	BC	FIXPAL1
    // asm 00003AD8: 	LDI	AR2,R0
    // asm 00003AD9: 	LDI	R0,R1
    // asm 00003ADA: 	CALL	PAL_OVERWRITE
FIXPAL1:
    // asm 00003ADB: 	NOP	*++AR5(2)
    // asm 00003ADC: 	DBU	AR6,FIXPALLP
    // asm 00003ADD: 	POP	AR6
    // asm 00003ADE: 	POP	AR5
    // asm 00003AE1: 	LDL	SW_VIEW0|SW_VIEW1|SW_VIEW2|SW_RADIO|SW_4TH|SW_3RD|SW_2ND|SW_1ST,R1
    // asm 00003AE2: 	LDL	SW_VIEW1|SW_RADIO|SW_2ND,R2
    // asm 00003AE4: 	LDI	@SWITCHBUTS,R0
    // asm: 	AND	R1,R0
    // asm: 	CMPI	R2,R0
    // asm: 	BNE	NOWAY
    // asm 00003AE6: 	READADJ	ADJ_STEERMIN
    // asm: 	SUBI	@_pot0,R0
    // asm 00003AE7: 	ABSI	R0
    // asm 00003AE8: 	CMPI	20,R0
    // asm 00003AE9: 	BGT	NOWAY
    // asm: 	LDI	RM_USA,R0
    // asm 00003AEB: 	STI	R0,@RACE_MODE
NOWAY:
    // asm 00003AF0: 	CALL	KILL_THE_REANIMATORS		;cheering crowd
    // asm 00003AF2: 	LDL	shared_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	shared_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm 00003AF3: 	LDL	_SECshared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm 00003AF4: 	CALL	INIT_DRONES			;init DRONE tracker system
    // asm 00003AF6: 	LDI	@RACE_MODE,R0
    // asm 00003AF7: 	CMPI	RM_USA,R0
    // asm 00003AF8: 	BNE	DDF1
    // asm 00003AF9: 	CREATE	SHOWLEG_PROC,UTIL_C|55h
DDF1:
    // asm: 	SONDFX	CC_DIGIT
    // 	;SHOW STATISTICS TO DATE
    // 	;
    // 	;
    // asm: 	LDI	@BONUS_WAVE,R5
    // asm 00003B01: 	SUBI	1,R5
    // asm: 	LDI	R5,AR0
    // asm: 	MPYI	GT_SIZE,AR0
    // asm: 	ADDI	@GAMETRAKI,AR0
    // ;	LDI	@MAXMPH,R0
    // ;	STI	R0,*+AR0(GT_MAXMPH)
    // asm 00003B05: 	LDI	1,R0
    // asm 00003B06: 	STI	R0,*+AR0(GT_POS)
    // asm 00003B07: 	STI	R0,*+AR0(GT_COLLS)
    // asm: 	LDI	@ETIME,R0
    // asm: 	STI	R0,*+AR0(GT_ETIME)
    // asm: 	LDI	@RACE_MODE,R0
    // asm 00003B0B: 	CMPI	RM_USA,R0
    // asm 00003B0C: 	BNE	NOTNIN2
    // asm: 	LDI	@GAMETRAKI,AR3
    // asm: 	LDI	@BUFFERSI,AR4
    // asm 00003B0D: 	CLRI	R4
    // asm 00003B0E: 	FLOAT	180,R7		;Y
NXTNAME:
    // asm: 	LDI	@START_HIT,R0
    // asm 00003B10: 	BNZ	NOTNIN2
    // asm 00003B12: 	CALL	PLACE_FLAG
    // asm 00003B13: 	SLEEP	2
    // asm: 	ADDF	10,R7
    // asm 00003B14: 	ADDI	GT_SIZE,AR3
    // asm 00003B15: 	INC	R4
    // asm: 	CMPI	R5,R4
    // asm 00003B16: 	BLE	NXTNAME
NOTNIN2:
    // 	;CHECK INITS
    // 	;
    // 	;SLEEP	30			;Make sure last flag is done
    // asm 00003B1D: 	LDI	20-1,AR5
    // asm 00003B1E: PAPA35
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PAPA35X
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,PAPA35
PAPA35X:
    // 	;DANGEROUS AND MESSY CODE, BUT
    // 	;WE NEVER TOUCH THE ACTIVE LIST
    // 	;ANYWAY, THEREFORE WE DONT NEED
    // 	;THESE ELEMENTS
    // 	;(OBJ_INIT CALLED IMMEDIATELY NEXT)
    // 	;
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@OACTIVE
    // asm: 	READAUD	ADJ_HIGH_SCORE_ENTRY
    // asm: 	CMPI	0,R0
    // asm 00003B24: 	BEQ	NOENTER
    // asm 00003B25: 	JSRP	ENTER_INITIALS
    // asm: 	LDF	@STEERCT,R0		;STEERING CENTER
    // asm: 	STF	R0,@WHEELPOS
NOENTER:
    // asm 00003B29: 	CLRI	R0
    // asm 00003B2A: 	STI	R0,@NOSWAP		;NO PAGE SWAPPING OFF
    // asm: 	LDI	@RACE_MODE,R0
    // asm: 	CMPI	RM_USA,R0
    // asm 00003B32: 	BNE	NOTNIN
    // asm 00003B33: 	LDI	@BONUS_WAVE,R0
    // asm: 	CMPI	14,R0
    // asm: 	BEQ	CLINTON_SHOW
NOTNIN:
    // asm 00003B35: 	CLRI	R0
    // asm 00003B36: 	STI	R0,@NOSWAP
    // asm 00003B37: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	CLRF	R0
    // asm: 	LDP	@_CAMERAPOS+X
    // asm 00003B3A: 	STF	R0,@_CAMERAPOS+X
    // asm 00003B3B: 	FLOAT	-1000,R0
    // asm 00003B3C: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	FLOAT	-2500,R0
    // asm: 	STF	R0,@_CAMERAPOS+Z
    // asm 00003B3E: 	CLRF	R2
    // asm 00003B3F: 	STF	R2,@_CAMERARAD+Y
    // asm 00003B40: 	SETDP
    // asm 00003B41: 	LDI	@CAMERAMATRIXI,AR2
    // asm 00003B42: 	CALL	FIND_YMATRIX
    // 	;\\++\\	    //--//
    // 	;  \\++\\ //--//
    // 	;    \\++\\ //
    // asm: 	LDI	@RACE_MODE,R0
    // asm: 	CMPI	RM_USA,R0
    // asm: 	BNE	NOT_ATOZ
    // ;	CALL	CLEANUP_PALS
    // asm 00003B49: 	LDI	@BONUS_WAVE,AR0
    // asm 00003B4A: 	ADDI	@FULLSETUP_TABLEI,AR0
ISWRP:
    // asm 00003B4B: LDI	*AR0,R0
    // asm: 	CALLU	R0
NOT_ATOZ:
    // asm: 	SLEEP	10
    // asm 00003B4C: 	LDI	1,R0
    // asm 00003B4D: 	STI	R0,@DO_FOLDFLAG
    // asm 00003B4E: 	SLEEP	10
    // asm 00003B50: 	LDI	@RACE_MODE,R0
    // asm: 	CMPI	RM_USA,R0
    // asm 00003B52: 	BNE	CHOOSE_NEXT_RACE
    // asm: 	READAUD	AUD_RESET_TOTALLY
    // asm 00003B56: 	CMPI	0,R0
    // asm 00003B57: 	BNE	MOTION_SCREWED
    // asm: 	CREATE	SHOWNEXTLEG_PROC,UTIL_C|TEXTP_T
    // asm 00003B59: 	LDI	@DID_TIMED_OUT,R0		;IF TIMED OUT THEN YOU MAY NOT GET A FREE RACE
    // asm 00003B5B: 	BNZ	NOT1ST
    // asm: 	LDI	@POSITION,R0
    // asm: 	CMPI	1,R0
    // asm 00003B5D: 	BNE	NOT1ST
    // asm 00003B60: 	READAUD	ADJ_FREEGAME
    // asm 00003B61: 	CMPI	0,R0
    // asm: 	BEQ	NOT1ST
    // asm 00003B64: 	LDI	@BONUS_WAVE,AR2
    // asm: 	DEC	AR2
    // asm 00003B65: 	ADDI	AUD_WIN_GGATE,AR2
    // asm 00003B67: 	CALL	AUDIT_INC
    // asm: 	INCAUD	AUD_TOTAL_FREEGAMES
    // asm 00003B69: 	LDI	@_MODE,R0
    // asm 00003B6A: 	ANDN	MMODE,R0
    // asm 00003B6B: 	OR	MINTRO,R0
    // asm 00003B6C: 	STI	R0,@_MODE
    // asm 00003B6D: 	LDI	@BONUS_WAVE,R0
    // asm: 	STI	R0,@CHOSEN_RACE
    // asm 00003B6F: 	CALL	SEND_RACENUM
    // asm 00003B71: 	BU	FINFIN
    // 	;Has the player made Pole Position?  (1)
    // 	;T-> Branch over this Coin Grab
    // 	;F-> Get more coin to continue
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISREC", 0, 0);
    UNIMPL();
}

void NOT1ST(void)
{
    // 	;;;	SYNCED INSMORE...
    // 	;;;	the difference is that the two machine communicate the time
    // 	;;;
    // 	;;;
    // asm: 	JSRP	INSMORE
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MMODE,R0
    // asm: 	OR	MINTRO,R0
    // asm: 	STI	R0,@_MODE
    // *ELP CHANGE
    // ;	CALL	CLRONE		;CAN NOW BE DUAL PLAYER
    // *ELP END CHANGE
    // asm 00003B7F: 	CMPI	1,AR6
    // asm 00003B80: 	BNE	ENDPLAYER
    // asm: 	INCAUD	AUD_GAMES_CONTINUES
    // asm: 	CALL	HSTDEC
    // asm: 	LDI	@BONUS_WAVE,R0
    // asm: 	STI	R0,@CHOSEN_RACE
    // asm 00003B81: 	CALL	SEND_RACENUM
FINFIN:
    // *ELP CHANGE
    // ;	LDI	@_MODE,R0
    // ;	ANDN	MINFIN|MWATER,R0
    // ;	STI	R0,@_MODE
    // *ELP END CHANGE
    // asm: 	JSRP	ISSUE_STARTGAME
    // asm: 	JSRP	WAIT_FOR_CHALLENGER
    // 	;  /-----RACE MODE ONLY-------
    // 	;  |
    // 	;  V
    // asm: 	LDI	@BONUS_WAVE,AR2
    // asm: 	MPYI	2,AR2
    // asm: 	ADDI	AUD_START_GGATE,AR2
    // asm: 	CALL	AUDIT_INC
    // asm 00003B95: 	CALL	TEXT_INIT
    // asm 00003B96: 	LDI	UTIL_C|TEXTP_T,R0
    // asm 00003B97: 	LDI	-1,R1
    // asm 00003B98: 	CALL	PRC_KILLALL
    // *ELP CHANGE
    // asm: 	LDI	@_MODE,R0
    // asm 00003B99: 	ANDN	MINFIN|MWATER,R0
    // asm 00003B9A: 	STI	R0,@_MODE
    // *ELP END CHANGE
    // asm: 	CALL	OBJ_INIT
    // asm 00003B9D: 	CALL	DYNAOBJ_INIT		;init DYNAMIC OBJECTS
    // asm 00003B9E: 	CALL	CARB_INIT		;init CAR BLOCKS
    // asm 00003B9F: 	CALL	INIT_RDDEBRIS		;initialize ROAD DEBRIS list(s)
    // 	;;;	RE INITIALIZE TRACK...
    // 	;;;
    // 	;;;
    // asm 00003BA3: 	CALL	TEXT_INIT
    // asm: 	LDI	UTIL_C|TEXTP_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	0,R0
    // asm: 	LDI	255,R1
    // asm: 	CALL	SET_TRACK_VOL
    // asm 00003BA4: 	CALL	RESUME_TUNE_NT
    // asm 00003BA9: 	LDI	BUT_VIEW2,R0			;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm 00003BAA: 	STI	R0,@BUTTON_STATUS
    // asm: 	READAUD	ADJ_TIME_TO_START
    // asm: 	MPYI	5,R0
    // asm: 	ADDI	60,R0
    // asm: 	STI	R0,@_countdown
    // asm 00003BAD: 	LDI	@_MODE,R0
    // asm: 	ANDN	MGO|MMODE,R0
    // asm 00003BAE: 	OR	MGAME|MINFIN,R0
    // asm 00003BB0: 	STI	R0,@_MODE
    // asm 00003BB2: 	CREATEC	FOLDMAP,UTIL_C
    // asm 00003BB4: 	LDI	@BONUS_WAVE,AR0
    // asm 00003BB5: 	ADDI	@BONUS_POSTLAUNCHI,AR0
    // asm 00003BB6: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	LDI	@NEXT_STARTUP,R0
    // asm 00003BBA: 	STI	R0,@STARTSECTION
    // asm 00003BBB: 	CALL	BGD_INIT
    // asm 00003BBD: 	PUSH	AR7
    // asm: 	LDI	UTIL_C|BACKGRND_T,R0
    // asm: 	LDI	-1,R1
    // asm 00003BBE: 	CALL	PRC_FIND
    // asm 00003BBF: 	LDI	AR0,AR7
    // asm 00003BC0: 	LDI	3,R0
    // asm: 	STI	R0,*+AR7(PTIME)
    // asm 00003BC2: 	LDI	PLYR_C|PLYR1_T,R0
    // asm 00003BC3: 	LDI	-1,R1
    // asm 00003BC4: 	CALL	PRC_FIND
    // asm 00003BC5: 	LDI	AR0,AR2
    // asm 00003BC6: 	CALL	PRC_FOLLOW
    // asm 00003BC7: 	POP	AR7
    // asm 00003BC8: 	LDI	*+AR7(PDATA),R0
    // asm 00003BC9: 	STI	R0,@BGNDCOLA
    // asm 00003BCB: 	LDI	1,R0
    // asm 00003BCC: 	STI	R0,@NOAERASE
    // asm 00003BCD: 	CLRI	R0
    // asm: 	STI	R0,@OHIGH_PRIORITY
    // asm 00003BCF: 	CREATE	WAVEFLAG,UTIL_C|MONKEY_T
    // asm 00003BD0: 	CLRI	R0
    // asm 00003BD1: 	STI	R0,@DID_TIMED_OUT
    // asm 00003BD3: 	CALL	INIT_GAMELEG
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOT1ST", 0, 0);
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
    // asm 00003BDB: 	CALL	PRC_KILLALL
    // asm: 	LDI	UTIL_C|TEXTP_T,R0
    // asm 00003BDD: 	LDI	-1,R1
    // asm 00003BDE: 	CALL	PRC_KILLALL
    // asm 00003BE0: 	LDI	PLYR_C|PLYR1_T,R0
    // asm 00003BE1: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm 00003BE3: 	CALL	TEXT_INIT
    // asm 00003BE5: 	READAUD	ADJ_CLINTON
    // asm 00003BE6: 	CMPI	1,R0
    // asm 00003BE7: 	BNE	NOCLINTON
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
    // asm 00003BF5: 	LDI	1234h,R0
    // asm 00003BF6: 	LDI	-1,R1
    // asm 00003BF7: 	CALL	PRC_KILLALL
    // asm: 	LDI	SPAWNER_C|ANIMATION_T,R0
    // asm: 	LDI	-1,R1
    // asm 00003BFA: 	CALL	PRC_KILLALL
    // asm: 	LDL	dc_shared_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm 00003BFD: 	LDL	finale_PALETTES,AR2
    // asm 00003BFE: 	CALL	dealloc_section
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
    // asm 00003C04: 	LDF	@STEERCT,R0		;STEERING CENTER
    // asm 00003C05: 	STF	R0,@WHEELPOS
NOENTR2:
    // asm 00003C06: CLRI	R0
    // asm 00003C08: 	STI	R0,@NOSWAP
    // asm 00003C0A: 	CALL	OBJ_INIT
    // asm 00003C12: 	CALL	CLEAR_MAP_PALS
    // asm: 	LDI	MATTR,R0
    // asm 00003C14: 	STI	R0,@_MODE
    // asm: 	LDI	@POSITION,R0
    // asm 00003C15: 	CMPI	1,R0
    // asm: 	BNE	JAJD
    // asm: 	INCAUD	AUD_CREDITS
    // asm 00003C16: 	INCAUD	AUD_WIN_DC
    // asm 00003C17: 	INCAUD	AUD_TOTAL_FREEGAMES
JAJD:
    // asm 00003C19: 	JSRP	VANITY_SUB
    // asm 00003C1A: 	LDI	-2,R0
    // asm 00003C1B: 	STI	R0,@_ATTR_MODE
    // asm 00003C1D: 	BU	SET_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLINTON_SHOW", 0, 0);
    UNIMPL();
}

void BLINK_FREEBE(void)
{
    // asm: 	SLEEP	5
    // asm: 	LDI	5*18,AR5
    // asm 00003C2A: BFLP1
    // asm: 	LDI	@_MODE,R4
    // asm: 	AND	MMODE,R4
    // asm: 	CMPI	MATTR,R4
    // asm: 	LDINE	70,R3
    // asm 00003C2A: 	LDIEQ	125,R3
    // asm: 	LDINE	256,R2
    // asm 00003C2D: 	LDIEQ	365,R2
    // asm 00003C30: 	FLOAT	R2
    // asm 00003C31: 	FLOAT	R3
    // asm: 	LDI	1,RC
    // asm 00003C32: 	LDL	WINT1,AR2
    // asm 00003C33: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00003C34: 	LDL	font18_white,AR2
    // asm 00003C35: 	CALL	PAL_FIND_RAW
    // asm 00003C36: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00003C37: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 00003C39: 	CMPI	MATTR,R4
    // asm 00003C3C: 	LDINE	90,R3
    // asm 00003C3F: 	LDIEQ	150,R3
    // asm 00003C41: 	LDINE	256,R2
    // asm 00003C42: 	LDIEQ	365,R2
    // asm 00003C44: 	FLOAT	R2
    // asm 00003C45: 	FLOAT	R3
    // asm: 	LDI	1,RC
    // asm 00003C46: 	LDL	WINT2,AR2
    // asm 00003C47: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00003C48: 	LDL	font18_white,AR2
    // asm 00003C49: 	CALL	PAL_FIND_RAW
    // asm 00003C4A: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00003C4B: 	STI	R0,*+AR1(TEXT_PAL)
    // ;insert frame rate equalizer
    // ;
    // asm 00003C53: 	SLEEP	1
    // asm 00003C54: 	DBU	AR5,BFLP1
    // asm 00003C55: 	DIE
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
    // asm 00003C5B: 	SUBF	*+AR2(OPOSZ),R3
    // asm 00003C5C: 	CALL	ARCTANF
    // asm 00003C5D: 	SUBF	HALFPI,R0
    // asm: 	LDF	R0,R2
    // asm 00003C5F: 	FLOAT	-7500,R0
    // asm 00003C60: 	ADDF	@ZOOMD,R0
    // asm 00003C61: 	CALL	DISTANCE_2D
    // asm 00003C63: 	LDF	R0,R6
    // asm 00003C64: 	LDF	R1,R7
    // asm 00003C65: 	LDP	@_CAMERAPOS+X
    // asm 00003C66: 	ADDF	@_CAMERAPOS+X,R6
    // asm 00003C67: 	ADDF	@_CAMERAPOS+Z,R7
    // asm: 	SETDP
    // asm 00003C69: 	LDI	30,AR5
    // asm 00003C6A: BACKLP
    // asm 00003C6A: 	LDP	@_CAMERAPOS+X
    // asm 00003C6B: 	LDF	@_CAMERAPOS+X,R0
    // asm 00003C6C: 	LDF	@_CAMERAPOS+Z,R1
    // asm: 	SUBF	R0,R6,R2
    // asm 00003C6E: 	SUBF	R1,R7,R3
    // asm: 	MPYF	0.02,R2
    // asm 00003C6F: 	MPYF	0.02,R3
    // asm 00003C70: 	ADDF	R0,R2,R0
    // asm 00003C71: 	ADDF	R1,R3,R1
    // asm 00003C72: 	STF	R0,@_CAMERAPOS+X
    // asm 00003C73: 	STF	R1,@_CAMERAPOS+Z
    // asm 00003C75: 	LDF	@_CAMERAPOS+Y,R0
    // asm 00003C76: 	SUBF	10,R0
    // asm 00003C77: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	SETDP
    // asm 00003C79: 	SLEEP	1
    // asm: 	DBU	AR5,BACKLP
    // asm 00003C7A: 	DIE
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
    // asm 00003C82: 	LDI	*AR2,R0
    // asm 00003C83: 	CALLU	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMED_OUT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void KILL_PLYR_SOUNDS(void)
{
    // asm 00003C8A: 	LDI	1000,AR2		;ENGINE RPM IDLE
    // asm: 	CALL	SENDSND
    // asm: 	LDI	SKIDB,AR2 		;KILL LOOPERS WHILE SUSPENDED
    // asm: 	CALL	KILLSNDFX
    // asm: 	LDI	SKIDC,AR2 		;KILL LOOPERS WHILE SUSPENDED
    // asm 00003C8B: 	CALL	KILLSNDFX
    // asm: 	LDI	BRAKSND,AR2
    // asm 00003C8D: 	CALL	KILLSNDFX
    // asm: 	LDI	TUNSND,AR2
    // asm 00003C8F: 	CALL	KILLSNDFX
    // asm: 	LDI	GRAVELA,AR2
    // asm 00003C91: 	CALL	KILLSNDFX
    // asm 00003C92: 	RETS
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
    // asm 00003CC4: 	CALL	ONESNDFX
    // asm 00003CC7: 	LDI	7,AR5
    // asm 00003CC8: FRAL1
    // asm 00003CC9: 	LDI	@START_HIT,R0
    // asm 00003CCA: 	BNZ	PRC_SUICIDE
    // asm: 	LDL	FRA1,AR2
    // asm: 	FLOAT	256,R2
    // asm 00003CCC: 	FLOAT	100,R3
    // asm: 	LDI	15,RC
    // asm 00003CCD: 	CALL	TEXT_ADD
    // asm 00003CCE: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00003CD0: 	SLEEP	9
    // asm 00003CD2: 	LDL	FRA2,AR2
    // asm 00003CD4: 	PUSH	AR2
    // asm 00003CD5: 	READAUD	ADJ_FREEGAME
    // asm: 	POP	AR2
    // asm 00003CD8: 	CMPI	0,R0
    // asm: 	BNE	ISFREE
    // asm 00003CDA: 	LDL	FRA1,AR2
ISFREE:
    // asm 00003CDC: 	FLOAT	256,R2
    // asm 00003CDE: 	FLOAT	150,R3
    // asm 00003CDF: 	LDI	15,RC
    // asm 00003CE0: 	CALL	TEXT_ADD
    // asm 00003CE1: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	SLEEP	10
    // asm 00003CE2: 	DBU	AR5,FRAL1
    // asm 00003CE3: 	DIE
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
    // asm 00003CF0: 	LDI	SHOWLEG_TIME,RC
    // asm 00003CF1: 	CALL	TEXT_ADDDS
    // asm 00003CF2: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00003CF3: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	STI	AR0,*+AR7(PDATA)
    // asm 00003CF4: 	STI	AR1,*+AR7(PDATA+1)
    // asm 00003CF5: 	CALL	SET18FONTDS
    // asm 00003CF7: 	LDI	@LCTSI,AR2
    // asm 00003CF8: 	FLOAT	256,R2
    // asm 00003CFB: 	FLOAT	275,R3
    // asm 00003CFE: 	LDI	SHOWLEG_TIME,RC
    // asm 00003CFF: 	CALL	TEXT_ADDDS
    // asm 00003D00: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00003D01: 	STI	AR0,*+AR7(PDATA+2)
    // asm 00003D02: 	STI	AR1,*+AR7(PDATA+3)
    // asm 00003D03: 	CALL	SET18FONTDS
    // asm 00003D09: 	FLOAT	-100,R6
    // asm 00003D0C: 	LDI	20,AR5	;20
SLLP1:
    // asm 00003D0D: FLOAT	256,R0
    // asm 00003D0E: 	SUBF	R6,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	R0,R6
    // asm 00003D0F: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D10: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D11: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D12: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D14: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm 00003D15: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D16: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D17: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D18: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm 00003D19: 	DBU	AR5,SLLP1
    // 	;CENTER IT
    // 	;
    // asm 00003D1E: 	FLOAT	256,R6
    // asm 00003D1F: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D21: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm 00003D22: 	ADDF	3,R0
    // asm 00003D23: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D24: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D25: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D26: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D28: 	SLEEP	35
    // asm 00003D2C: 	LDI	20,AR5
SLLP1A:
    // asm 00003D2D: FLOAT	-100,R0
    // asm: 	SUBF	R6,R0
    // asm 00003D2D: 	MPYF	0.2,R0
    // asm: 	ADDF	R0,R6
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D2F: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D30: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D31: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D33: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm 00003D34: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D35: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D36: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D37: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm 00003D38: 	DBU	AR5,SLLP1A
    // asm 00003D3A: 	DIE
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
    // asm 00003D42: 	FLOAT	256,R2
    // asm: 	FLOAT	50,R3
    // asm: 	LDI	SHOWLEG_TIME,RC
    // asm 00003D44: 	CALL	TEXT_ADDDS
    // asm 00003D45: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00003D46: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	STI	AR0,*+AR7(PDATA)
    // asm 00003D47: 	STI	AR1,*+AR7(PDATA+1)
    // asm 00003D48: 	CALL	SET18FONTDS
    // asm 00003D4A: 	LDI	@LNLSI,AR2
    // asm 00003D4B: 	FLOAT	256,R2
    // ;	FLOAT	275,R3
    // asm 00003D51: 	FLOAT	25,R3
    // asm 00003D52: 	LDI	SHOWLEG_TIME,RC
    // asm 00003D53: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00003D54: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00003D55: 	STI	AR0,*+AR7(PDATA+2)
    // asm: 	STI	AR1,*+AR7(PDATA+3)
    // asm 00003D56: 	CALL	SET18FONTDS
    // asm 00003D5C: 	FLOAT	600,R6
    // asm 00003D5F: 	LDI	16,AR5
SLLP2:
    // asm 00003D60: FLOAT	256,R0
    // asm 00003D61: 	SUBF	R6,R0
    // asm: 	MPYF	0.25,R0
    // asm: 	ADDF	R0,R6
    // asm 00003D62: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D63: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D64: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D65: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D67: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm 00003D68: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D69: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D6A: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D6B: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm 00003D6C: 	DBU	AR5,SLLP2
    // 	;CENTER IT
    // 	;
    // asm 00003D71: 	FLOAT	256,R6
    // asm 00003D72: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D74: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    // asm: 	STF	R6,*+AR0(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm 00003D75: 	ADDF	3,R0
    // asm 00003D76: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D77: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D78: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D79: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D7B: 	SLEEP	50
    // asm 00003D7E: 	LDI	215,AR5
SLLP2A:
    // asm 00003D7F: FLOAT	600,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.25,R0
    // asm 00003D80: 	ADDF	R0,R6
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    // asm 00003D82: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D83: 	LDI	*+AR7(PDATA+3),AR0
    // asm 00003D84: 	STF	R6,*+AR0(TEXT_POSX)
    // asm 00003D86: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm 00003D87: 	LDI	*+AR7(PDATA),AR0
    // asm 00003D88: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003D89: 	LDI	*+AR7(PDATA+2),AR0
    // asm 00003D8A: 	STF	R0,*+AR0(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm 00003D8B: 	DBU	AR5,SLLP2A
    // asm 00003D8D: 	DIE
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
    // asm 00003D96: 	ADDI	@FLAG_POS_TABLEI,AR0
    // asm 00003D97: 	LDI	*AR0,R2
    // asm 00003D98: 	SUBI	256,R2
    // asm 00003D99: 	LDI	*+AR0,R3
    // asm 00003D9A: 	SUBI	200-20,R3
    // asm 00003D9B: 	LDI	367,RC
    // asm 00003D9C: 	LDL	star,AR2
    // asm 00003D9D: 	CALL	OBJ_QMAKE
    // asm 00003D9F: 	LDIL	O_3DROT,R0
    // asm 00003DA0: 	OR	O_NOUROT|O_NOUNIV,R0
    // asm 00003DA1: 	OR	*+AR0(OFLAGS),R0
    // asm 00003DA2: 	STI	R0,*+AR0(OFLAGS)
    // asm 00003DA4: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTHP
    // asm 00003DA9: 	LDI	AR0,AR4
    // asm 00003DAA: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm 00003DAB: 	LDF	*+AR4(OPOSY),R0
    // asm 00003DAC: 	STF	R0,*+AR4(OVELY)
    // asm: 	LDF	100,R0
    // asm 00003DAD: 	CALL	SFRAND
    // asm 00003DAE: 	STF	R0,*+AR4(OPOSX)
    // asm 00003DB0: 	FLOAT	-230,R0
    // asm 00003DB1: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CREATEC	PLACE_FLAG_PROC,UTIL_C
    // asm 00003DB4: 	SONDFX	MAPSTAR2
    // asm 00003DB5: 	POP	RC
    // asm 00003DB6: 	POP	RC
    // asm 00003DB7: 	POP	R2
    // asm: 	POP	AR4
    // asm: 	POP	AR0
    // asm 00003DBA: 	RETS
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
    // asm 00003DC2: PFPLP
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	SUBF	*+AR4(OPOSX),R0
    // asm: 	MPYF	0.25,R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OVELY),R1
    // asm 00003DC3: 	SUBF	*+AR4(OPOSY),R1
    // asm 00003DC4: 	MPYF	0.25,R1
    // asm 00003DC5: 	ADDF	*+AR4(OPOSY),R1
    // asm 00003DC6: 	STF	R1,*+AR4(OPOSY)
    // asm: 	MPYF	R1,R1
    // asm 00003DC8: 	MPYF	R0,R0
    // asm 00003DC9: 	ADDF	R1,R0
    // asm 00003DCA: 	CMPF	25,R0
    // asm 00003DCB: 	BLT	ALLDN7
    // asm 00003DCD: 	LDF	*+AR4(ORADZ),R2
    // asm 00003DCE: 	ADDF	0.1,R2
    // asm 00003DCF: 	STF	R2,*+AR4(ORADZ)
    // asm 00003DD0: 	LDI	AR4,AR2
    // asm 00003DD1: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm 00003DD2: 	SLEEP	1
    // asm 00003DD3: 	DBU	AR5,PFPLP
ALLDN7:
    // asm 00003DD6: 	LDF	*+AR4(OVELX),R0
    // asm 00003DD7: 	STF	R0,*+AR4(OPOSX)
    // asm 00003DD8: 	LDF	*+AR4(OVELY),R0
    // asm 00003DDA: 	STF	R0,*+AR4(OPOSY)
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
    // asm 00003DE1: 	FLOAT	0,R2
    // asm: 	FLOAT	20,R3
    // asm 00003DE2: 	LDI	999,RC
    // asm 00003DE4: 	CALL	TEXT_ADDDS
    // asm 00003DE5: 	STI	AR0,*+AR7(PDATA+10)	;SHADOW
    // asm 00003DE6: 	STI	AR1,*+AR7(PDATA+11)	;ACTUAL
    // asm 00003DE7: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 00003DE8: 	OR	TXT_RIGHT,R0
    // asm 00003DE9: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 00003DEA: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 00003DEB: 	OR	TXT_RIGHT,R0
    // asm 00003DEC: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm 00003DEE: 	READAUD	ADJ_MPHORKPM
    // asm 00003DEF: 	CMPI	0,R0
    // asm 00003DF0: 	BEQ	ISMPHT
    // asm 00003DF2: 	LDF	@MAXMPH,R0
    // asm: 	MPYF	1.6666,R0
    // asm: 	BU	KJL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_MAXMPH", 0, 0);
    UNIMPL();
}

void ISMPHT(void)
{
    // asm 00003DF5: LDF	@MAXMPH,R0
KJL:
    // asm 00003DF6: LDF	@MAXMPH_COUNT,R1
    // asm: 	CALL	DIV_F
    // asm 00003DF7: 	FIX	R0,R2
    // asm 00003DF8: 	LDI	AR7,AR2
    // asm 00003DF9: 	ADDI	PDATA,AR2
    // asm: 	CALL	_itoa
    // asm 00003DFA: 	FLOAT	512,R2
    // asm 00003DFB: 	FLOAT	20,R3
    // asm 00003DFC: 	LDI	999,RC
    // asm 00003DFD: 	CALL	TEXT_ADDDS
    // asm 00003DFE: 	CALL	SETLGDIGITFONTDS
    // asm 00003DFF: 	STI	AR0,*+AR7(PDATA+12)
    // asm 00003E00: 	STI	AR1,*+AR7(PDATA+13)
    // asm 00003E02: 	LDL	dnums_amber,AR2
    // asm 00003E03: 	CALL	PAL_FIND_RAW
    // asm 00003E04: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00003E05: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	CLRF	R0
    // asm 00003E08: 	STF	R0,@MAXMPH
    // asm 00003E09: 	STF	R0,@MAXMPH_COUNT
ENTER_HERE:
    // asm: 	LDI	31,AR5
    // asm 00003E0C: 	CLRF	R6		;FIXED STF
    // asm 00003E0D: 	FLOAT	512,R7		;FLT STF
    // asm 00003E0E: BML
    // asm: 	FLOAT	250,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.1,R0
    // asm 00003E11: 	CMPF	8,R0
    // asm: 	LDFLT	8,R0
    // asm 00003E14: 	ADDF	R0,R6
    // asm: 	FLOAT	250,R0
    // asm 00003E15: 	CMPF	R0,R6
    // asm 00003E16: 	LDFGT	R0,R6
    // asm: 	LDI	*+AR7(PDATA+10),AR0
    // asm 00003E17: 	LDI	*+AR7(PDATA+11),AR1
    // asm: 	STF	R6,*+AR1(TEXT_POSX)
    // asm 00003E18: 	LDF	R6,R0
    // asm 00003E19: 	ADDF	3,R0
    // asm 00003E1A: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003E1C: 	FLOAT	260,R0
    // asm 00003E1D: 	SUBF	R7,R0
    // asm 00003E1E: 	MPYF	0.20,R0
    // asm 00003E20: 	CMPF	-12,R0
    // asm: 	LDFLT	-12,R0
    // asm 00003E21: 	ADDF	R0,R7
    // asm 00003E23: 	FLOAT	260,R0
    // asm: 	CMPF	R0,R7
    // asm 00003E24: 	LDFLT	R0,R7
    // asm: 	LDI	*+AR7(PDATA+12),AR0
    // asm 00003E26: 	LDI	*+AR7(PDATA+13),AR1
    // asm: 	STF	R7,*+AR1(TEXT_POSX)
    // asm 00003E27: 	LDF	R7,R0
    // asm 00003E28: 	ADDF	3,R0
    // asm 00003E29: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003E2A: 	SLEEP	1
    // asm 00003E2B: 	DBU	AR5,BML
    // asm 00003E2D: 	FLOAT	250,R0
    // asm 00003E2E: 	FLOAT	253,R1
    // asm 00003E2F: 	FLOAT	260,R2
    // asm: 	FLOAT	263,R3
    // asm 00003E32: 	LDI	*+AR7(PDATA+10),AR0
    // asm: 	STF	R1,*+AR0(TEXT_POSX)
    // asm 00003E33: 	LDI	*+AR7(PDATA+11),AR0
    // asm 00003E34: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003E36: 	LDI	*+AR7(PDATA+12),AR0
    // asm: 	STF	R3,*+AR0(TEXT_POSX)
    // asm 00003E37: 	LDI	*+AR7(PDATA+13),AR0
    // asm 00003E38: 	STF	R2,*+AR0(TEXT_POSX)
KKL:
    // asm 00003E3B: SLEEP	1
    // asm 00003E3B: 	LDI	@DO_FOLDFLAG,R0
    // asm 00003E3C: 	BZ	KKL
    // asm 00003E3E: 	SONDFX	WIPE2
    // asm: 	LDI	31,AR5
    // asm 00003E3F: 	FLOAT	250,R6		;FIXED STF
    // asm 00003E41: 	FLOAT	260,R7		;FLT STF
    // asm 00003E42: BML2
    // asm: 	FLOAT	-20,R0
    // asm 00003E43: 	SUBF	R6,R0
    // asm: 	MPYF	0.1,R0
    // asm 00003E45: 	ADDF	R0,R6
    // asm 00003E46: 	LDI	*+AR7(PDATA+10),AR0
    // asm 00003E47: 	LDI	*+AR7(PDATA+11),AR1
    // asm: 	STF	R6,*+AR1(TEXT_POSX)
    // asm 00003E48: 	LDF	R6,R0
    // asm 00003E49: 	ADDF	3,R0
    // asm 00003E4A: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003E4C: 	FLOAT	532,R0
    // asm 00003E4D: 	SUBF	R7,R0
    // asm 00003E4E: 	MPYF	0.1,R0
    // asm 00003E4F: 	ADDF	R0,R7
    // asm 00003E50: 	LDI	*+AR7(PDATA+12),AR0
    // asm 00003E51: 	LDI	*+AR7(PDATA+13),AR1
    // asm: 	STF	R7,*+AR1(TEXT_POSX)
    // asm 00003E52: 	LDF	R7,R0
    // asm 00003E53: 	ADDF	3,R0
    // asm 00003E54: 	STF	R0,*+AR0(TEXT_POSX)
    // asm 00003E56: 	SLEEP	1
    // asm 00003E57: 	DBU	AR5,BML2
    // asm 00003E59: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISMPHT", 0, 0);
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
    // asm 00003E62: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 00003E63: 	OR	TXT_RIGHT,R0
    // asm 00003E64: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 00003E65: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 00003E66: 	OR	TXT_RIGHT,R0
    // asm 00003E67: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm 00003E68: 	STI	AR0,*+AR7(PDATA+10)
    // asm 00003E69: 	STI	AR1,*+AR7(PDATA+11)
    // asm 00003E6D: 	LDI	@BONUS_WAVE,R6
    // asm: 	DEC	R6
    // asm 00003E6E: 	LDI	0,R7
    // asm 00003E6F: 	CALL	GET_TABLE_ADDR
    // asm: 	CALL	TABLE_ENTRY_READ
    // 	;
    // 	;R0	time code
    // 	;R1	init 1
    // 	;R2	init 2
    // 	;R3	init 3
    // 	;
    // asm 00003E74: 	LDI	AR7,AR1
    // asm: 	ADDI	PDATA+14,AR1
    // asm: 	AND	0FFh,R1
    // asm: 	AND	0FFh,R2
    // asm: 	AND	0FFh,R3
    // asm: 	LS	8,R2
    // asm 00003E75: 	OR	R2,R1
    // asm 00003E76: 	LS	16,R3
    // asm: 	OR	R3,R1
    // asm 00003E77: 	LDI	' ',R2
    // asm 00003E78: 	LS	24,R2
    // asm 00003E79: 	OR	R2,R1
    // asm 00003E7A: 	STI	R1,*AR1
    // asm 00003E7B: 	LDI	' ',R2
    // asm 00003E7C: 	STI	R2,*+AR1
    // asm 00003E7E: 	LDI	AR7,AR2
    // asm 00003E7F: 	ADDI	PDATA,AR2
    // asm 00003E80: 	CALL	TIME2STR
    // asm 00003E82: 	LDI	AR1,AR0
    // asm 00003E83: 	LDI	AR2,AR1
    // asm: 	LDI	AR0,AR2
    // asm 00003E84: 	CALL	STRCAT		;AR1 from above
    // asm: 	FLOAT	512,R2
    // asm 00003E87: 	FLOAT	120,R3
    // asm 00003E88: 	LDI	999,RC
    // asm 00003E89: 	CALL	TEXT_ADDDS
    // asm 00003E8A: 	CALL	SET18FONTDS
    // asm: 	STI	AR0,*+AR7(PDATA+12)
    // asm: 	STI	AR1,*+AR7(PDATA+13)
    // asm 00003E8D: 	BU	ENTER_HERE
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
    // asm 00003E93: 	SLEEP	5
    // asm 00003E94: 	LDL	HH2,AR2
    // asm 00003E96: BLAHB	STI	AR2,*+AR7(PDATA)
    // asm 00003E97: 	FLOAT	256,R2
    // asm: 	FLOAT	(150+300),R3
    // asm: 	LDI	999,RC
    // asm 00003E98: 	CALL	TEXT_ADDDS
    // asm 00003E9B: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 00003E9C: 	OR	TXT_CENTER,R0
    // asm 00003E9D: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 00003E9E: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 00003E9F: 	OR	TXT_CENTER,R0
    // asm 00003EA0: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm 00003EA1: 	LDI	AR0,AR4
    // asm 00003EA2: 	LDI	AR1,AR5
    // asm 00003EA5: 	LDL	font18_white,AR2
    // asm 00003EA6: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR4(TEXT_PAL)
    // asm 00003EA7: 	STI	R0,*+AR5(TEXT_PAL)
BONHTLP:
    // asm 00003EAA: 	LDF	*+AR4(TEXT_POSY),R0
    // asm 00003EAB: 	FLOAT	152,R1
    // asm 00003EAC: 	LDF	R1,R3
    // asm: 	SUBF	R0,R1,R1
    // asm: 	MPYF	0.1,R1
    // asm: 	ADDF	R0,R1
    // asm 00003EAE: 	CMPF	R3,R1
    // asm 00003EAF: 	LDFLT	R3,R1
    // asm: 	STF	R1,*+AR4(TEXT_POSY)
    // asm 00003EB2: 	LDF	*+AR5(TEXT_POSY),R0
    // asm: 	FLOAT	150,R1
    // asm 00003EB3: 	LDF	R1,R3
    // asm 00003EB4: 	SUBF	R0,R1,R1
    // asm 00003EB5: 	MPYF	0.1,R1
    // asm: 	ADDF	R0,R1
    // asm 00003EB6: 	CMPF	R3,R1
    // asm 00003EB7: 	LDFLT	R3,R1
    // asm 00003EB8: 	STF	R1,*+AR5(TEXT_POSY)
    // asm 00003EBA: 	CALL	TEXTTOG
    // asm 00003EBC: 	SLEEP	1
    // asm 00003EBD: 	LDI	@DO_FOLDFLAG,R0
    // asm 00003EBE: 	BZ	BONHTLP
BONHTLP2:
    // asm 00003EC0: 	LDF	*+AR4(TEXT_POSY),R0
    // asm 00003EC2: 	FLOAT	152+300,R1
    // asm 00003EC3: 	LDF	R1,R3
    // asm: 	SUBF	R0,R1,R1
    // asm: 	MPYF	0.1,R1
    // asm: 	ADDF	R0,R1
    // asm 00003EC5: 	CMPF	R3,R1
    // asm 00003EC6: 	LDFGT	R3,R1
    // asm: 	STF	R1,*+AR4(TEXT_POSY)
    // asm 00003EC9: 	LDF	*+AR5(TEXT_POSY),R0
    // asm: 	FLOAT	150+300,R1
    // asm 00003ECA: 	LDF	R1,R3
    // asm 00003ECB: 	SUBF	R0,R1,R1
    // asm 00003ECC: 	MPYF	0.1,R1
    // asm: 	ADDF	R0,R1
    // asm 00003ECD: 	CMPF	R3,R1
    // asm 00003ECE: 	LDFGT	R3,R1
    // asm 00003ECF: 	STF	R1,*+AR5(TEXT_POSY)
    // asm 00003ED1: 	SLEEP	1
    // asm 00003ED2: 	BU	BONHTLP2
    // asm 00003ED3: BADA	LDI	1,R0
    // asm 00003ED4: 	STI	R0,*+AR4(TEXT_TIKS)
    // asm 00003ED5: 	STI	R0,*+AR5(TEXT_TIKS)
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_HOTTIME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TEXTTOG(void)
{
    // asm 00003EDB: 	ADDI	@NFRAMES,R4
    // asm 00003EDC: 	CMPI	30,R4
    // asm: 	BLT	ISOFF
    // asm: 	CMPI	35,R4
    // asm: 	BLT	GAGA
    // asm: 	CLRI	R4
GAGA:
    // asm 00003EDF: 	LDL	NULLSTR5,R0
    // asm: 	BU	IBOIBO
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXTTOG", 0, 0);
    UNIMPL();
}

void ISOFF(void)
{
    // asm 00003EE0: LDI	*+AR7(PDATA),R0
IBOIBO:
    // asm 00003EE1: STI	R0,*+AR4(TEXT_PTR)
    // asm 00003EE2: 	STI	R0,*+AR5(TEXT_PTR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISOFF", 0, 0);
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
    // asm 00003EEB: 	LDI	*+AR0(TEXT_COLOR),R0
    // asm 00003EEC: 	OR	TXT_RIGHT,R0
    // asm 00003EED: 	STI	R0,*+AR0(TEXT_COLOR)
    // asm 00003EEE: 	LDI	*+AR1(TEXT_COLOR),R0
    // asm 00003EEF: 	OR	TXT_RIGHT,R0
    // asm 00003EF0: 	STI	R0,*+AR1(TEXT_COLOR)
    // asm 00003EF1: 	STI	AR0,*+AR7(PDATA+10)
    // asm 00003EF2: 	STI	AR1,*+AR7(PDATA+11)
    // asm 00003EF5: 	LDI	@DID_TIMED_OUT,R0
    // asm 00003EF6: 	BZ	DOREG3
    // asm 00003EF7: 	LDI	0,R0		;ELAPSED TIME OF 0 = DID NOT FINISH
    // asm 00003EF8: 	STI	R0,@ETIME
    // asm: 	LDL	EXPIRED,AR2
    // asm 00003EF9: 	BU	LREG3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONS_ETIME", 0, 0);
    UNIMPL();
}

void DOREG3(void)
{
    // asm 00003EFB: LDI	@STOPWATCH,R0
    // asm 00003EFB: 	STI	R0,@ETIME
    // asm 00003EFC: 	LDI	AR7,AR2
    // asm: 	ADDI	PDATA,AR2
    // asm 00003EFD: 	CALL	TIME2STR
LREG3:
    // asm 00003EFF: 	FLOAT	512,R2
    // asm 00003F00: 	FLOAT	50,R3
    // asm 00003F01: 	LDI	999,RC
    // asm 00003F02: 	CALL	TEXT_ADDDS
    // asm 00003F03: 	CALL	SET18FONTDS
    // asm: 	STI	AR0,*+AR7(PDATA+12)
    // asm: 	STI	AR1,*+AR7(PDATA+13)
    // asm 00003F07: 	LDL	font18_white,AR2
    // asm 00003F08: 	CALL	PAL_FIND_RAW
    // asm 00003F09: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00003F0A: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@STOPWATCH
    // asm 00003F0B: 	BU	ENTER_HERE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOREG3", 0, 0);
    UNIMPL();
}

void BONS_POSITION(void)
{
    // asm: 	LDI	@POSITION,AR4
    // asm: 	DEC	AR4
    // asm: 	MPYI	3,AR4
    // asm: 	ADDI	@POS_TABLEI,AR4
    // asm: 	LDI	-60,R5		;YPOS
    // asm 00003F12: 	LDI	40,AR6
    // asm 00003F13: BPL1
    // asm 00003F14: 	FLOAT	R5,R0
    // asm 00003F15: 	LDF	R0,R1
    // asm: 	FLOAT	138-60,R2
    // asm 00003F16: 	SUBRF	R2,R0
    // asm: 	MPYF	0.2,R0
    // asm 00003F17: 	ADDF	R1,R0
    // asm: 	FIX	R0,R5
    // asm 00003F19: 	LDI	*AR4,AR2
    // asm 00003F1A: 	LDI	220,R2
    // asm 00003F1B: 	ADDI	*+AR4(2),R2
    // asm 00003F1C: 	LDI	R5,R3
    // asm 00003F1D: 	LDI	TM|ZS,R4
    // asm 00003F1E: 	CALL	BLTMOD2D_DS
    // asm 00003F1F: 	LDI	*+AR4(1),AR2
    // asm 00003F20: 	LDI	220+35,R2
    // asm 00003F21: 	LDI	R5,R3
    // asm 00003F22: 	LDI	TM|ZS,R4
    // asm 00003F23: 	CALL	BLTMOD2D_DS
    // asm 00003F24: 	SLEEP	1
    // asm: 	DBU	AR6,BPL1
    // asm 00003F28: 	LDI	158,AR6
BPL2:
    // asm 00003F2A: 	LDI	*AR4,AR2
    // asm 00003F2C: 	LDI	220,R2
    // asm: 	ADDI	*+AR4(2),R2
    // asm: 	LDI	R5,R3
    // asm: 	LDI	TM|ZS,R4
    // asm 00003F2D: 	CALL	BLTMOD2D_DS
    // asm 00003F2E: 	LDI	*+AR4(1),AR2
    // asm 00003F2F: 	LDI	220+35,R2
    // asm 00003F30: 	LDI	R5,R3
    // asm 00003F31: 	LDI	TM|ZS,R4
    // asm 00003F32: 	CALL	BLTMOD2D_DS
    // asm 00003F33: 	SLEEP	1
    // asm 00003F34: 	LDI	@DO_FOLDFLAG,R0
    // asm 00003F35: 	BZ	BPL2
    // asm 00003F38: 	LDI	40,AR6
    // asm 00003F39: BPL3
    // asm: 	FLOAT	R5,R0
    // asm 00003F3B: 	LDF	R0,R1
    // asm 00003F3C: 	SUBRF	-60,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	R1,R0
    // asm 00003F3D: 	FIX	R0,R5
    // asm 00003F3E: 	LDI	*AR4,AR2
    // asm 00003F3F: 	LDI	220,R2
    // asm 00003F40: 	ADDI	*+AR4(2),R2
    // asm 00003F41: 	LDI	R5,R3
    // asm 00003F42: 	LDI	TM|ZS,R4
    // asm 00003F43: 	CALL	BLTMOD2D_DS
    // asm 00003F44: 	LDI	*+AR4(1),AR2
    // asm 00003F45: 	LDI	220+35,R2
    // asm 00003F46: 	LDI	R5,R3
    // asm 00003F47: 	LDI	TM|ZS,R4
    // asm 00003F48: 	CALL	BLTMOD2D_DS
    // asm 00003F49: 	SLEEP	1
    // asm: 	DBU	AR6,BPL3
    // asm 00003F4A: 	DIE
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
    // asm 00003F53: 	CALL	PRC_KILLALL
    // 	;WAVEFLAG, MONKEYs
    // asm: 	LDI	UTIL_C|MONKEY_T,R0
    // asm: 	LDI	CLASS_M|TYPE_M,R1
    // asm 00003F56: 	CALL	PRC_KILLALL
    // 	;LBACK_WATCH
    // asm: 	LDI	UTIL_C|BACKGRND_T,R0
    // asm: 	LDI	-1,R1
    // asm 00003F59: 	CALL	PRC_KILLALL
    // asm 00003F5B: 	CALL	DELETE_SPLAT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILL_THEM", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FIND_AND_REACTIVATE(void)
{
    // asm 00003F5F: 	PUSH	AR0
    // asm 00003F60: 	PUSH	AR4
    // asm: 	LDI	@OACTIVE,AR0
FARLP:
    // asm 00003F61: LDI	*AR0,R0
    // asm: 	BZ	FARX
    // asm 00003F61: 	LDI	R0,AR0
    // asm 00003F62: 	LDI	*+AR0(OID),R0
    // asm: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_DUDE_S,R0
    // asm 00003F63: 	BNE	NOTRUT
    // asm 00003F65: 	LDI	AR0,AR4
    // asm 00003F66: 	CALL	RUT_ANI
    // asm 00003F67: 	BU	FARLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_AND_REACTIVATE", 0, 0);
    UNIMPL();
}

void NOTRUT(void)
{
    // asm 00003F69: CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_BABE_S,R0
    // asm: 	BNE	FARLP
    // asm 00003F6A: 	LDI	AR0,AR4
    // asm 00003F6C: 	CALL	HUNGH_ANI_REENTER
    // asm 00003F6D: 	BU	FARLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTRUT", 0, 0);
    UNIMPL();
}

void FARX(void)
{
    // asm: 	POP	AR4
    // asm: 	POP	AR0
    // asm 00003F70: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FARX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void KILL_THE_REANIMATORS(void)
{
    // asm 00003F73: 	LDI	SPAWNER_C|ANIMATION_T|7,R0
    // asm 00003F74: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KILL_THE_REANIMATORS", 0, 0);
    UNIMPL();
}

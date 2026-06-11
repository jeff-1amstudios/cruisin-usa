#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/obj.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/delta.h"
#include "../include/error.h"
#include "../include/h2hobj.h"
#include "../include/comm.h"
#include "../include/racer.h"
#include "../include/intro.h"

/*
 * Source module: asm/INTRO.ASM
 */

// *----------------------------------------------------------------------------
// *INTRO SCREENS AND RELATED ROUTINES
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *
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
// *----------------------------------------------------------------------------
// *
// *
// *
// *
const char JINOW[] = "JOIN IN NOW";
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
const char WFCHAL1[] = "WAITING";
const char WFCHAL2[] = "FOR CHALLENGER";
const char WFCHAL3[] = "HOLD RADIO OR VIEW3 TO CANCEL";
/* asm: FRAMELAG	.bss	FRAMELAG,1 */
int FRAMELAG;
// *----------------------------------------------------------------------------
#define PLYPOS2YL (-400)
#define PLYPOS2ZL 2200
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *CHOOSECAR
// *
const char CCT[] = "CHOOSE CAR";
// 	;all Y's were 0
// 	;
/* asm: CHOOSENCAR	.bss	CHOOSENCAR,1 */
int CHOOSENCAR;
#define LANESIZE 1152
// 	;if a slave then ALWAYS appear on right side
// 	;
// 	;
// 		;(BECOMES PLYR PROC)
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: CAR_CHOICE_GOTTEN	.bss	CAR_CHOICE_GOTTEN,1 */
int CAR_CHOICE_GOTTEN;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *
// *THIS PROCESS:
// *	1)	MOVES THE CAMERA TO THE VEHICLE
// *	2)	LOWERS VEHICLE TO GROUND
// *	3)	TURNS VEHICLE TO FACING OUT
// *	4)	DRIVES VEHICLE FORWARD (WHILE TRACKING
// *		WITH CAMERA)
// *	5)	INITIALIZES AS PLAYERS CAR
// *	6)	TRANSFERS CONTROL TO THE PLYRS
// *		MAIN ROUTINE
// *
// *
// *
// 	;NOW MOVE TO IN FRONT OF OBJECT
// 	;TURN CAR TO FACE AWAY (SMOOTH)
// 	;LOWER CAR TO GROUND
// 	;NOW SET ALL FACING PROPERLY
// 	;
// *
// *
// *
// *
// 	;NOW MOVE TO IN FRONT OF OBJECT
// 	;TURN CAR TO FACE AWAY
// 	;LOWER CAR TO GROUND
// *
// *
// *
// *
// *NOW WE MOVE FORWARD AND INTO THE POSITION
// *OF INSIDE LANE OF ROAD PIECE (OUSR1==0102h)
// *
// 	;MOVE FORWARD LOOP
// 	;
// 	;if a slave then ALWAYS appear on right side
// 	;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: CAR_ARRAY	.bss	CAR_ARRAY,4 */
int CAR_ARRAY[4];
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *SHOW CAR STATISTICS
// *
// *USES
// *	AR4,AR5,AR6,R4,R5
// *
// *----------------------------------------------------------------------------
/* asm: CAR1PAL	.bss	CAR1PAL,129 */
int CAR1PAL[129];
/* asm: CAR2PAL	.bss	CAR2PAL,129 */
int CAR2PAL[129];
/* asm: CAR3PAL	.bss	CAR3PAL,129 */
int CAR3PAL[129];
/* asm: CAR4PAL	.bss	CAR4PAL,129 */
int CAR4PAL[129];
/* asm: CARPAL_TABLE	.word	CAR1PAL,CAR2PAL,CAR3PAL,CAR4PAL */
int CARPAL_TABLE[] = { CAR1PAL, CAR2PAL, CAR3PAL, CAR4PAL };
/* asm: CARSRCPAL_TAB	.word	cvette_p,hotrod_p,missle_p,testor_p */
int CARSRCPAL_TAB[] = { cvette_p, hotrod_p, missle_p, testor_p };
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *GENERAL PURPOSE CHOOSE CAR LOOP ROUTINE
// *
// *
// 	;THE CAR
#define RNDR_C1_DYH (PDATA+1)
#define RNDR_C1_SYH (PDATA+2)
#define RNDR_C2_DYH (PDATA+3)
#define RNDR_C2_SYH (PDATA+4)
#define RNDR_C3_DYH (PDATA+5)
#define RNDR_C3_SYH (PDATA+6)
#define RNDR_C4_DYH (PDATA+7)
#define RNDR_C4_SYH (PDATA+8)
// 	;THE LIFT
#define RNDR_L1_DYH (PDATA+9)
#define RNDR_L1_SYH (PDATA+10)
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
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *HIDDEN VEHICLES
// *
// *
// *
/* asm: IS_HIDDEN	.bss	IS_HIDDEN,1 */
int IS_HIDDEN;
/* asm: HIDDEN_TABLE	.word	jeepm,sbuspm,copcar,gtruck */
int HIDDEN_TABLE[] = { jeepm, sbuspm, copcar, gtruck };
// *----------------------------------------------------------------------------
/* asm: SPINCURR	.bss	SPINCURR,1 */
int SPINCURR;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
const char T_READY[] = "READY";
const char T_SET[] = "SET";
const char T_GO[] = "GO";
const char T_CHALLENG[] = "CHALLENGE RACE";
/* asm: TLIST	.word	T_READY,CHICK_READY,SEND_WAVEFL_READY */
/* asm: 	.word	T_SET,CHICK_SET,SEND_WAVEFL_SET */
int TLIST[] = { T_READY, CHICK_READY, SEND_WAVEFL_READY, T_SET, CHICK_SET, SEND_WAVEFL_SET };
/* asm: TLGO	.word	T_GO,CHICK_GO,SEND_WAVEFL_GO */
int TLGO[] = { T_GO, CHICK_GO, SEND_WAVEFL_GO };
/* asm: BABE_CONTROL	.bss	BABE_CONTROL,1 */
int BABE_CONTROL;
/* asm: CURR_FLAGSTATE	.bss	CURR_FLAGSTATE,1 */
int CURR_FLAGSTATE;
#define MOTION_ERROR_TIKS ((57*5))
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *DIAL ROUT
// *
// *SET THE PROPER OBJECT COLOR CYCLING
// *
/* asm: LASTCHOICE	.bss	LASTCHOICE,1 */
int LASTCHOICE;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *ATTRACT MODE TIMEOUT MECHANISM
// *SLEEP _timer TIKS THEN JUMP TO CYCLE_ATTR
// *
/* asm: _timer	.bss	_timer,1 */
int _timer;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *INSERT COINS ROUTINES
// *
// *	INSMORE		JSRPed FROM PLYR.ASM
// *	COIN_CNTDOWN	CREATED, KILLED
// *
/* asm: TROI	SPTR	"INSERT COINS" */
char *TROI = "INSERT COINS";
/* asm: ICCI	SPTR	"TO CONTINUE" */
char *ICCI = "TO CONTINUE";
/* asm: PSCI	SPTR	"PRESS START" */
char *PSCI = "PRESS START";
/* asm: SAVEDMODE	.bss	SAVEDMODE,1 */
int SAVEDMODE;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: DIRTY_SHARED	.bss	DIRTY_SHARED,1 */
int DIRTY_SHARED;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: TRAFFIC_LL	.word	light_yellowon,10,light_redon,32,light_greenon,32,-1 */
int TRAFFIC_LL[] = { light_yellowon, 10, light_redon, 32, light_greenon, 32, -1 };
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// ;*----------------------------------------------------------------------------
// ;SKYPALSI	.word	SKYPALS
// ;	romdata
// ;	.include	sky.pal
// ;SKYPALS	.word	sky1_dusk,sky1_dark,sky1_dred
// ;	.word	sky1_az,sky1_genorg,sky1_dblue
// ;	.text
// ;
// ;*
// ;*
// ;*	AR2	PARAMETERS
// ;*
// ;	.globl	SETSKYPAL
// ;SETSKYPAL:
// ;
// ;	LDI	sky1_p,AR2
// ;	CALL	PAL_FIND
// ;	LDI	R0,R2
// ;
// ;	LDI	1,AR2
// ;	ADDI	@SKYPALSI,AR2
// ;	LDI	*AR2,AR2
// ;	LDI	*AR2++,R3
// ;	CALL	PAL_SET
// ;	RETS
// ;*----------------------------------------------------------------------------

void HEAD2HEAD_LOGO_WAIT(void)
{
    // asm: 	LDI	1,AR6
    // asm: 	BU	H2HLE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEAD_LOGO_WAIT", 0, 0);
    UNIMPL();
}

void HEAD2HEAD_LOGO(void)
{
    // asm: 	CLRI	AR6
H2HLE:
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@FRAMRATE	;frame governor
    // asm: 	LDI	@_MODE,R0	;shut off infinity
    // asm: 	AND	MMODE,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDL	redhd1,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	0,R3
    // asm: 	LDI	368,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	AR0,*+AR7(PDATA)	;red (bottom)
    // asm: 	LDL	yelhd1,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	0,R3
    // asm: 	LDI	368,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	AR0,*+AR7(PDATA+1)	;yellow (bottom)
    // asm: 	LDL	big2,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	0,R3
    // asm: 	LDI	368,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	H2HPAL3,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	AR0,*+AR7(PDATA+2)	;big2
    // asm: 	FLOAT	160,R6	;60+4*25
    // asm: 	FLOAT	4000,R7			;big2
    // asm: 	LDI	25,AR5
    // asm: H2HLLP
    // asm: 	LDI	*+AR7(PDATA+1),AR0	;red (bottom)
    // asm: 	STF	R6,*+AR0(OPOSY)
    // asm: 	LDI	*+AR7(PDATA),AR0	;yellow
    // asm: 	NEGF	R6,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDI	*+AR7(PDATA+2),AR0	;big2
    // asm: 	STF	R7,*+AR0(OPOSZ)
    // asm: 	SUBF	4,R6
    // asm: 	LDLF	360,R0
    // asm: 	SUBF	R0,R7,R1
    // asm: 	MPYF	0.1,R1
    // asm: 	SUBF	R1,R7
    // asm: 	CALL	JINMSG
    // asm: 	CALL	KABOSHP
    // asm: 	BC	KABOSH
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,H2HLLP
    // asm: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm: 	CMPI	1,R0
    // asm: 	BNE	NOSND1
    // asm: 	SOND1	STAMP
NOSND1:
    // 	;cycle HEAD   2    HEAD punch
    // 	;
    // asm: 	LDI	*+AR7(PDATA),AR4	;yellow
    // asm: 	FLOAT	120,R6
    // asm: 	JSRP	THROBIT
    // asm: 	CALL	KABOSHP
    // asm: 	BC	KABOSH
    // asm: 	LDI	*+AR7(PDATA+2),AR4	;big2
    // asm: 	FLOAT	300,R6
    // asm: 	JSRP	THROBIT
    // asm: 	CALL	KABOSHP
    // asm: 	BC	KABOSH
    // asm: 	LDI	*+AR7(PDATA+1),AR4	;red (bottom)
    // asm: 	FLOAT	120,R6
    // asm: 	JSRP	THROBIT
    // asm: 	CALL	KABOSHP
    // asm: 	BC	KABOSH
    // 	;
    // 	;
    // asm: 	CMPI	1,AR6		;are we waiting for a joiner??
    // asm: 	BEQ	WFC
    // asm: 	CALL	KABOSHP
    // asm: 	BC	KABOSH
    // asm: 	SLEEP	30
    // asm: 	BR	CYCLE_ATTR
WFC:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@FRAMRATE	;frame governor
WFCLP342:
    // asm: 	LDI	@ICF,R0
    // asm: 	BGT	NOMSGG
    // asm: 	LDL	JINOW,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	100,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOMSGG:
    // asm: 	SLEEP	1
    // asm: 	CALL	KABOSHP
    // asm: 	BNC	WFCLP342
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
    // asm: 	CALL	CLEAR_LINK
    // asm: 	LDI	-2,R0
    // asm: 	STI	R0,@_ATTR_MODE
    // asm: 	BU	SET_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEAD_LOGO", 0, 0);
    UNIMPL();
}

void KABOSHP(void)
{
    // *----------------------------------------------------------------------------
    // *RETURNS CARRY SET ON KABOSH THIS MESS
    // *
    // *
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm: 	BZ	TKABOSH
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BEQ	FKABOSH
    // asm: 	CMPI	MINIT,R0
    // asm: 	BEQ	FKABOSH
    // asm: 	CMPI	MINSERT_COINS,R0
    // asm: 	BEQ	FKABOSH
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	TKABOSH
    // asm: 	LDI	@OM_LINKWAIT,R0
    // asm: 	BZ	TKABOSH
FKABOSH:
    // asm: 	CLRC
    // asm: 	RETS
TKABOSH:
    // asm: 	SETC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KABOSHP", 0, 0);
    UNIMPL();
}

void JINMSG(void)
{
    // asm: 	CMPI	1,AR6
    // asm: 	BNE	NOJINMSG
    // asm: 	LDL	JINOW,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	100,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOJINMSG:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JINMSG", 0, 0);
    UNIMPL();
}

void THROBIT(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	1,AR5
    // asm: THROBLP1
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	SUBF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	CALL	JINMSG
    // asm: 	CALL	KABOSHP
    // asm: 	BC	ENDTHROB
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,THROBLP1
    // asm: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm: 	CMPI	1,R0
    // asm: 	BNE	NOSND2
    // asm: 	SOND1	STAMP
NOSND2:
    // asm: 	LDI	4,AR5
KKGKG:
    // asm: CALL	JINMSG
    // asm: 	CALL	KABOSHP
    // asm: 	BC	ENDTHROB
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,KKGKG
    // asm: 	LDI	1,AR5
    // asm: THROBLP2
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	CALL	JINMSG
    // asm: 	CALL	KABOSHP
    // asm: 	BC	ENDTHROB
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,THROBLP2
ENDTHROB:
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THROBIT", 0, 0);
    UNIMPL();
}

void WAIT_FOR_CHALLENGER(void)
{
    // *
    // *
    // asm: 	LDI	5,R0
    // asm: 	STI	R0,@FRAMELAG
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BOILEROBJ
    // 	;IF NO TRANSMISSIONS ARE HAPPENING
    // 	;(OTHER == SINGLE PLAYER) THEN IGNORE THIS MESS AND ASSUME
    // 	;SINGLE PLAYER
    // 	;
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm: 	BZ	WFCLPXXX
    // asm: 	LDI	@NOASK_LINK,R0
    // asm: 	BNZ	WFCLPXXX
    // asm: 	LDI	@FIRST_RACE,R0
    // asm: 	BNZ	CHAHC
    // asm: 	CALL	BOILERPLATE_INIT
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
CHAHC:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_HIT
    // asm: 	STI	R0,@miniidle
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MGO,R0
    // asm: 	STI	R0,@_MODE
    // 	;if no challenger
    // asm: 	LDI	20,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	CALL	SEND_VEHICLE
WFCLP:
    // asm: 	CALL	SEND_VEHICLE
    // asm: 	LDI	@COINDROP,R0
    // asm: 	BZ	NOWORRY
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@COINDROP
    // asm: 	LDI	20,R1
    // asm: 	BU	JJFHF
NOWORRY:
    // asm: 	LDI	@_countdown,R1
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MINIT,R0
    // asm: 	LDIEQ	20,R1
    // asm: 	CMPI	MBONUS,R0
    // asm: 	LDIEQ	20,R1
JJFHF:
    // asm: STI	R1,@_countdown
    // 	;IF challenger found THEN wait until ready then exit
    // 	;
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	BABAB
    // asm: 	LDI	@OM_VEHICLE,R0
    // asm: 	BGE	WAITX		;WE'RE THERE DUDE!
BABAB:
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BNZ	HHFBF
    // asm: 	LDI	@_countdown,R0
    // asm: 	BLE	WFCLPXXX
HHFBF:
    // asm: 	LDI	@FRAMELAG,R0
    // asm: 	DEC	R0
    // asm: 	LDILT	0,R0
    // asm: 	STI	R0,@FRAMELAG
    // asm: 	BGT	NOMSG768
    // asm: 	LDL	WFCHAL1,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	100,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDL	WFCHAL2,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	146,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
NOMSG768:
    // 	;if it is a linked game AND he hasn't chosen his vehicle
    // 	;then wait for him to choose his vehicle
    // 	;
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	BABAHH
    // asm: 	SLEEP	1
    // asm: 	BU	WFCLP
BABAHH:
    // *ELP CHANGE
    // ;	LDI	@miniidle,R0
    // ;	INC	R0
    // ;	CMPI	2,R0
    // ;	LDIGE	0,R0
    // ;	STI	R0,@miniidle
    // ;	CMPI	0,R0
    // ;	BNE	JJFJFJ
    // asm: 	LDI	@SWITCHBUTS,R0
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
    // asm: 	LDI	@FRAMELAG,R0
    // asm: 	BGT	NOMSG123
    // asm: 	CALL	WAITINTROTIMER
    // asm: 	LDL	WFCHAL3,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	192,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
NOMSG123:
    // asm: 	SLEEP	1
    // asm: 	BU	WFCLP
WFCLPXXX:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@MY_LINKWAIT
    // asm: 	STI	R0,@HEAD2HEAD_ON
    // asm: 	CALL	SEND_LINKCANCELLED
    // asm: 	SLEEP	1
    // asm: 	CALL	SETONE		;1 PLAYER GAME
WAITX:
    // 	;*****
    // 	;*****  WE CAN NO LONGER ACCEPT A DUDE
    // 	;*****
    // 	;*****
    // asm: 	LDI	@BOILEROBJ,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	CALLNE	OBJ_DELETE
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MGO,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	5,R0			;so player doesn't puke (see near call)
    // asm: 	STI	R0,@_countdown
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_FOR_CHALLENGER", 0, 0);
    UNIMPL();
}

void CHECK_ENDBONUS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *an issue start game has been called and
    // *it has been determined that the other machine
    // *is either in MBONUS or MINIT.  at that point
    // *we will wait for the other machine to enter
    // *MATTR, or MINTRO
    // *
    // *
    // *
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm: 	BZ	CEBT
    // asm: 	LDI	@OM_BONUS_WAITFLAG,R0
    // asm: 	BNZ	CEBT
    // asm: 	LDI	@OM_LINKWAIT,R0
    // asm: 	BNZ	CEBT
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BNZ	CEBT
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	CEBT
    // asm: 	CLRC
    // asm: 	RETS
CEBT:
    // asm: 	SETC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_ENDBONUS", 0, 0);
    UNIMPL();
}

void WAIT_FOR_ENDBONUS(void)
{
    // asm: 	PUSHP	R6
    // asm: 	SLEEP	2
    // asm: 	CALL	CHECK_ENDBONUS
    // asm: 	BC	ISGAME5
    // asm: 	POPP	R6
    // asm: 	CMPI	1,R6
    // asm: 	BEQ	NOOBJSSS
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	H2HPAL3,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	redhd1,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	-60,R3
    // asm: 	LDI	368,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	PUSHP	AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	AR2,AR0
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	LDL	yelhd1,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	60,R3
    // asm: 	LDI	368,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	PUSHP	AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	AR2,AR0
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	LDL	big2,AR2
    // asm: 	LDI	0,R2
    // asm: 	LDI	0,R3
    // asm: 	LDI	368,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	PUSHP	AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	AR2,AR0
    // asm: 	LDL	H2HPAL3,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	BU	LKJASDFGD
NOOBJSSS:
    // asm: 	CALL	BOILERPLATE_INIT
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
LKJASDFGD:
    // asm: 	PUSHP	R6
WAIT_FOR_ENDBONUS_LP:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@BONUS_WAITFLAG
    // asm: 	LDL	WFCHAL1,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	100,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDL	WFCHAL2,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	160,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	CALL	CHECK_ENDBONUS
    // asm: 	BC	ISGAME5
    // asm: 	SLEEP	1
    // asm: 	BU	WAIT_FOR_ENDBONUS_LP
ISGAME5:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BONUS_WAITFLAG
    // asm: 	SLEEP	2
    // asm: 	POPP	R6
    // asm: 	CMPI	1,R6
    // asm: 	BEQ	NOJHASD
    // asm: 	POPP	AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	POPP	AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	POPP	AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDL	H2HPAL3,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	BU	IURENDFL
NOJHASD:
    // asm: 	LDI	@BOILEROBJ,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	CALLNE	OBJ_DELETE
IURENDFL:
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_FOR_ENDBONUS", 0, 0);
    UNIMPL();
}

void ISSUE_STARTGAME_TSEL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	1,R6
    // asm: 	BU	LKAS534
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISSUE_STARTGAME_TSEL", 0, 0);
    UNIMPL();
}

void ISSUE_STARTGAME(void)
{
    // asm: 	CLRI	R6
LKAS534:
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm: 	BZ	NOGAME
    // ;	LDI	@OM_MODE,R0
    // ;	AND	MMODE,R0
    // ;	CMPI	MGAME,R0
    // ;	BEQ	NOGAME
    // *ELP CHANGE
    // asm: 	LDI	@FIRST_RACE,R0
    // asm: 	BNZ	NOTNND
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
    // asm: 	LDI	@WAS_HEAD2HEAD_ON,R0
    // asm: 	BNZ	DOITANY4
    // asm: 	LDI	@OM_BONUS_WAITFLAG,R0
    // asm: 	BNZ	DOITANY4
    // asm: 	LDI	@_ATTR_MODE,R0
    // asm: 	CMPI	-7,R0
    // asm: 	BEQ	DOITANY4
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
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
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@LINKEDP
    // asm: 	CALL	SEND_START_GAME
    // asm: 	LDI	30,AR5
WTFORRETVAL:
    // asm: 	DEC	AR5
    // asm: 	CMPI	0,AR5
    // asm: 	BLE	NTINLK
    // asm: 	SLEEP	1
    // *ELP CHANGE
    // asm: 	LDI	@TRANSMISSION_ACTIVE,R0
    // asm: 	BZ	NOGAME
    // *ELP END CHANGE
    // asm: 	LDI	@LINKEDP,R0	;0 = invalid
    // 				;1 = linked
    // 				;2 = not linked
    // asm: 	BZ	WTFORRETVAL
    // asm: 	CMPI	1,R0
    // asm: 	BNE	NTINLK
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HEAD2HEAD_ON
    // asm: 	INCAUD	AUD_H2HGAMES
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@OM_LINKWAIT
    // asm: 	STI	R0,@MY_LINKWAIT
NTINLK:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@NOASK_LINK
    // asm: 	RETP
NOGAME:
    // *ELP CHANGE
    // asm: 	CALL	SEND_LINKCANCELLED
    // asm: 	SLEEP	1
    // *ELP END CHANGE
    // asm: 	CALL	CLEAR_LINK
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOASK_LINK
    // asm: 	CALL	SETONE
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISSUE_STARTGAME", 0, 0);
    UNIMPL();
}

void PLYR_INTRO(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	RM_SINGLE,R0
    // asm: 	STI	R0,@RACE_MODE
    // asm: 	LDI	MINTRO|MGO,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@WAS_HEAD2HEAD_ON
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@FIRST_RACE
    // asm: 	JSRP	ISSUE_STARTGAME
    // asm: 	CALL	INIT_LASTHS_TABLE		;Initialize the table for players hs entries
    // asm: 	LDI	-1,R2
    // asm: 	SETADJ	ADJ_INITIALS
    // asm: 	INCAUD	AUD_GAMENUMBER
    // asm: 	INCAUD	AUD_GAMES_START
    // asm: 	CALL	HSTDEC
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BONUS_WAVE
    // asm: 	LDF	1.0,R0
    // asm: 	STF	R0,@GAMEDIFF
    // asm: 	READAUD	AUD_UNFINISHED_GAMES
    // asm: 	PUSH	R0
    // asm: 	READAUD	AUD_UNFINISHED_GAMES_FOUND
    // asm: 	POP	R1
    // asm: 	CMPI	R1,R0
    // asm: 	BEQ	NOULOG
    // asm: 	ERRON	U,EC_UNFINISHED
    // asm: 	LDI	R1,R2
    // asm: 	SETAUD	AUD_UNFINISHED_GAMES_FOUND
NOULOG:
    // asm: 	INCAUD	AUD_NUM_UNFINISHED
    // asm: 	LDI	1,R0				;SHUFFLE DRIVIN
    // asm: 	STI	R0,@TUNE_IDX
CNR_ENTER:
    // asm: 	LDP	@IN_RESET_MODE
    // asm: 	LDI	@IN_RESET_MODE,R0
    // asm: 	SETDP
    // asm: 	BZ	CONTINUE
    // asm: 	SLEEP	1
    // asm: 	BU	CNR_ENTER
CONTINUE:
    // asm: 	SOND1	START_THEME
    // asm: 	LDI	BUT_START,R0		;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	CLRI	R0			;INITIALIZE SCORE
    // asm: 	STI	R0,@SCORE
    // asm: 	STI	R0,@END_OF_GAMEP	;END OF GAME FLAG
    // asm: 	STI	R0,@_MPH
    // asm: 	STI	R0,@STARTSECTION	;BEGIN LEG OF JOURNEY
    // asm: 	STI	R0,@CAR_CHOICE_GOTTEN
    // asm: 	STI	R0,@CHOSEN_RACE
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@IS_HIDDEN
    // asm: 	CALL	GETCMOS_VALUES
    // asm: 	CALL	INIT_PEDALCHK
    // asm: 	CALL	OBJ_INIT
    // asm: 	CALL	TEXT_INIT
    // asm: 	JSRP	TRACK_SELECTION
    // asm: 	LDI	@FIRST_RACE,R0
    // asm: 	BZ	LOAD_NEW_SELECTION
    // asm: 	CALL	INIT_PEDALCHK
    // asm: 	CALL	OBJ_INIT
    // asm: 	CALL	TEXT_INIT
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@DCALL
    // asm: 	JSRP	CHOOSE_TRANSMISSION
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@DCALL
    // asm: 	JSRP	CHOOSECAR
    // asm: 	LDI	BUT_VIEW2,R0			;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm: 	STI	R0,@BUTTON_STATUS
WFSNP:
    // asm: SLEEP	1
    // asm: 	LDI	@START_NOW_P,R0
    // asm: 	BZ	WFSNP
ALL_JOINUP:
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MINFIN,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	READAUD	ADJ_TIME_TO_START
    // asm: 	MPYI	5,R0
    // asm: 	ADDI	60,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	CREATEC	WAVEFLAG,UTIL_C|MONKEY_T
    // asm: 	CALL	CLEANUP_TRACKSEL_PALS
    // asm: 	LDI	UTIL_C|CHOOSECAR_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // 	;
    // 	;SETUP THE GAME
    // 	;
    // asm: 	CLRF	R0
    // asm: 	STF	R0,@GAME_TIMER
    // asm: 	INCAUD	AUD_NUM_BUYINS
    // asm: 	LDI	0AAh,R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	LDI	1,R0	    			;SET GAME FRAME RATE
    // asm: 	STI	R0,@FRAMRATE
    // asm: 	STI	R0,@TIMECLR
    // asm: 	STI	R0,@DRONE_DISPATCH_P
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@SCREENWIPE_DONE
    // asm: 	STI	R0,@NEXT_STARTUP
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	9,R0
    // asm: 	STI	R0,@POSITION
    // asm: 	READAUD	ADJ_CHECKPOINT_BONUS
    // asm: 	STI	R0,@CHECKPOINT_TIME_BONUS
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOAERASE
    // 	;///////////////////////////////
    // asm: 	LDI	@FIRST_RACE,R0
    // asm: 	BZ	_PLYR
    // asm: 	CLRI	R2				;FIRST WAVE
    // asm: 	SETAUD	AUD_LAST_LEG
    // asm: 	LDF	1.0,R0
    // asm: 	STF	R0,@WHEELPWR
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_INTRO", 0, 0);
    UNIMPL();
}

void CHOOSE_NEXT_RACE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	TEXT_INIT
    // asm: 	LDI	UTIL_C|TEXTP_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	PLYR_C|PLYR1_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MINFIN|MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CALL	OBJ_INIT
    // asm: 	CALL	DYNAOBJ_INIT	;init DYNAMIC OBJECTS
    // asm: 	CALL	CARB_INIT	;init CAR BLOCKS
    // asm: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@FIRST_RACE
    // 	;
    // asm: 	SLEEP	4
    // 	;
    // asm: 	BU	CNR_ENTER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOOSE_NEXT_RACE", 0, 0);
    UNIMPL();
}

void LOAD_NEW_SELECTION(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	ADDI	@FULLSETUP_TABLEI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	LDI	MINTRO|MINFIN,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDF	@START_POS+X,R0
    // asm: 	LDF	@START_POS+Y,R1
    // asm: 	LDF	@START_POS+Z,R2
    // asm: 	LDP	@_CAMERAPOS+X
    // asm: 	STF	R0,@_CAMERAPOS+X
    // asm: 	STF	R1,@_CAMERAPOS+Y
    // asm: 	STF	R2,@_CAMERAPOS+Z
    // asm: 	SETDP
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	ADDI	@RACE_STARTING_POINTSI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	STI	R0,@STARTSECTION
    // asm: 	CALL	BGD_INIT
    // asm: 	CALL	INIT_GAMELEG
    // asm: 	LDI	MGAME,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CALL	SCREENWIPE_OPEN
    // asm: 	SLEEP	6
    // asm: 	LDI	MGAME|MHUD|MINFIN,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	ADDI	@BONUS_POSTLAUNCHI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	LDI	BUT_VIEW2,R0			;BUTTON OVERWRITE (MAYBE USE MASK IN FUTURE)
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	BU	ALL_JOINUP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_NEW_SELECTION", 0, 0);
    UNIMPL();
}

void WATCH_PLYRS_CAR(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	AR4	PLYRS CAR
    // *
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
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	LDF	*+AR4(OPOSY),R1
    // asm: 	LDF	*+AR4(OPOSZ),R2
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	ADDF	*+AR2(Y),R1
    // asm: 	ADDF	*+AR2(Z),R2
    // asm: 	LDP	@_CAMERAPOS
    // asm: 	STF	R0,@_CAMERAPOS+X
    // asm: 	STF	R1,@_CAMERAPOS+Y
    // asm: 	STF	R2,@_CAMERAPOS+Z
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATCH_PLYRS_CAR", 0, 0);
    UNIMPL();
}

void INIT_GAMELEG(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@SMOKE_COUNT
    // asm: 	STI	R0,@DID_TIMED_OUT
    // asm: 	STI	R0,@CHECKPOINT_NUM
    // asm: 	STI	R0,@REAL_CHECKPOINTS
    // asm: 	STI	R0,@MOTION_STOP_HIT
    // asm: 	STI	R0,@H2H_FLAGSTATE
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@FRAMRATE
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,@ROADKILL_SOUND_TIMER
    // asm: 	CREATE	RHO_DISPATCHER,SPAWNER_C|TRAFFIC_T
    // asm: 	CREATE	SIGMA_DISPATCHER,SPAWNER_C|TRAFFIC_T
    // asm: 	CREATE	CPOINT_LIGHT,SPAWNER_C|COLORCYC_T
    // asm: 	CREATEC	POSITION_FINDER,SPAWNER_C|TRAFFIC_T
    // asm: 	CREATE	MOVEIN_HUD_EQUIP,UTIL_C
    // asm: 	LDI	SM_HALT,R0
    // asm: 	STI	R0,@SUSPEND_MODE
    // asm: 	LDI	0,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	1,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	2,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	3,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	4,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	5,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	6,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	7,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	9,R4
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_COMMP,R0
    // asm: 	BNZ	BABA66
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BZ	BABA66
    // asm: 	LDI	8,R4
BABA66:
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_GAMELEG", 0, 0);
    UNIMPL();
}

void CHOOSECAR(void)
{
    // *
    // *CAMERA POSITION IS ASSUMED TO BE SET BY THE TIME
    // *THIS ROUTINE IS REACHED.
    // *
    // *
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	ADDI	@FULLSETUP_TABLEI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	LDI	@CHOSEN_RACE,AR0
    // asm: 	ADDI	@RACE_STARTING_POINTSI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	STI	R0,@STARTSECTION
    // asm: 	CALL	BGD_INIT
    // asm: 	LDI	MINTRO|MINFIN|MWATER|MGO,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	STI	R0,@START_HIT
    // asm: 	CALL	TEXT_INIT
    // asm: 	SOND1	CHOOSEUCAR
    // asm: 	CREATEC	HIDDEN_VEHICLES,UTIL_C|CHOOSECAR_T
    // asm: 	LDI	@CCTI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	50,R3
    // asm: 	LDI	600,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	CALL	SET40FONT
JT75:
    // asm: LDI	@CAR_CHOICE_GOTTEN,R0
    // asm: 	BNZ	KIBO
    // asm: 	SLEEP	1
    // asm: 	BU	JT75
KIBO:
    // asm: 	LDL	crace_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOOSECAR", 0, 0);
    UNIMPL();
}

void THE_CAR_CHOICE_PROC(void)
{
    // ;	SLEEP	15
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@CAR_CHOICE_GOTTEN
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
    // asm: 	CALL	GETCHOICE
    // asm: 	CALL	TRANSCHOICE
    // asm: 	CALL	DIAL_ROUT
    // asm: 	CALL	SHOW_CAR_STATISTICS
    // 	;-------time remaining
    // 	;
    // asm: 	LDL	time,AR2
    // asm: 	LDI	242,R2			;R2	POS X
    // asm: 	LDI	337,R3			;R3	POS Y
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
    // asm: 	CALL	PEDALCHK
    // asm: 	BC	CCLPX
    // asm: 	CALL	INTROTIMER
    // asm: 	SLEEP	1
    // asm: 	LDI	@_countdown,R0
    // asm: 	BGT	CCLP
CCLPX:
    // *
    // *END CHOOSE CAR LOOP
    // *
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CAR_CHOICE_GOTTEN
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THE_CAR_CHOICE_PROC", 0, 0);
    UNIMPL();
}

void RAISE_DOOR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CAR IS CHOOSEN, THEN RAISE THE GARAGE DOOR
    // *
    // *
    // *
    // asm: 	SOND1	GOPEN
    // asm: 	LDI	49,AR5
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm: 	ADDI	501h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	AR0,AR4
    // asm: RDLP
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	14,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,RDLP
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAISE_DOOR", 0, 0);
    UNIMPL();
}

void GETTHECARS(void)
{
    // asm: 	LDF	@START_RADY,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDL	CAR_ARRAY,AR3
    // asm: 	LDI	@CCTABI,AR4
    // asm: 	CLRI	R4
LISTLP:
    // asm: CALL	OBJ_GET
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	FLOAT	*AR4++,R1		;GET X POSITION
    // asm: 	STF	R1,*+AR2(X)
    // asm: 	FLOAT	*AR4++,R1		;GET Y POSITION
    // asm: 	STF	R1,*+AR2(Y)
    // asm: 	FLOAT	*AR4++,R1		;GET Z POSITION
    // asm: 	STF	R1,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R0
    // asm: 	ADDF	@START_POS+X,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR2(Y),R0
    // asm: 	ADDF	@START_POS+Y,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	ADDF	@START_POS+Z,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	*AR4++,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	STI	AR0,*AR3++
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDI	AR7,AR1
    // asm: 	ADDI	PDATA,AR1
    // asm: 	ADDI	R4,AR1
    // asm: 	STI	AR0,*AR1
    // asm: 	INC	R4
    // asm: 	LDF	*AR4++,R2
    // asm: 	ADDF	@START_RADY,R2
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	STF	R2,*+AR0(ORADY)
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	*AR4++,R0
    // asm: 	STI	R0,*+AR0(OID)
    // asm: 	LDI	*AR4,R0
    // asm: 	CMPI	-1,R0
    // asm: 	BNE	LISTLP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTHECARS", 0, 0);
    UNIMPL();
}

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
    // asm: 	LDI	5-1,AR3
GBERLP:
    // asm: LDI	*AR5++,AR2
    // asm: 	LDF	*AR7,R2
    // asm: 	FLOAT	180,R3
    // asm: 	FLOAT	AR3,R0
    // asm: 	MPYF	10,R0
    // asm: 	SUBF	R0,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SETFIXEDFONTDS
    // asm: 	DBU	AR3,GBERLP
    // asm: 	LDI	@CHOSEN_VEHICLE,AR5
    // asm: 	ADDI	@TEXTTABSI,AR5
    // asm: 	LDI	*AR5,AR5
    // asm: 	LDI	5,AR3
    // asm: GNNERLP
    // asm: 	LDI	*AR5++,AR2
    // asm: 	LDF	*AR7,R2
    // asm: 	ADDF	100,R2
    // asm: 	CMPI	6,AR3
    // asm: 	LDFEQ	*AR7,R2
    // asm: 	FLOAT	180,R3
    // asm: 	FLOAT	AR3,R0
    // asm: 	MPYF	10,R0
    // asm: 	SUBF	R0,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SETFIXEDFONTDS
    // asm: 	DBU	AR3,GNNERLP
    // asm: 	POP	AR7
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_CAR_STATISTICS", 0, 0);
    UNIMPL();
}

void CLEANUP_DIMCAR_PALS(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *(AND OVERHEAD LIGHT CLEANUP)
    // *
    // *
    // asm: 	LDL	flour_lghtof,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDL	CAR1PAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDL	CAR2PAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDL	CAR3PAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDL	CAR4PAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEANUP_DIMCAR_PALS", 0, 0);
    UNIMPL();
}

void GETTHECAR(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // *
    // *PARAMETERS
    // *	AR2	RAW PALETTE	CAR1PAL
    // *	AR3	ID OF VEHICLE
    // *	IR0	RNDR_C1_DYH
    // *	IR1	INDEX OF VEHCILE
    // *
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
    // asm: 	STF	R0,*+AR7(IR0)
    // asm: 	INC	IR0
    // asm: 	STF	R0,*+AR7(IR0)
    // asm: 	STF	R0,*+AR0(OUSR1)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTHECAR", 0, 0);
    UNIMPL();
}

void ROUNDER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // asm: 	LDL	CAR1PAL,AR2
    // asm: 	LDI	481h,AR3
    // asm: 	LDI	RNDR_C1_DYH,IR0
    // asm: 	CALL	GETTHECAR
    // asm: 	LDL	CAR2PAL,AR2
    // asm: 	LDI	482h,AR3
    // asm: 	LDI	RNDR_C2_DYH,IR0
    // asm: 	CALL	GETTHECAR
    // asm: 	LDL	CAR3PAL,AR2
    // asm: 	LDI	483h,AR3
    // asm: 	LDI	RNDR_C3_DYH,IR0
    // asm: 	CALL	GETTHECAR
    // asm: 	LDL	CAR4PAL,AR2
    // asm: 	LDI	484h,AR3
    // asm: 	LDI	RNDR_C4_DYH,IR0
    // asm: 	CALL	GETTHECAR
    // asm: 	LDI	401h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR7(RNDR_L1_DYH)
    // asm: 	STF	R0,*+AR7(RNDR_L1_SYH)
    // asm: 	LDI	402h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR7(RNDR_L2_DYH)
    // asm: 	STF	R0,*+AR7(RNDR_L2_SYH)
    // asm: 	LDI	403h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR7(RNDR_L3_DYH)
    // asm: 	STF	R0,*+AR7(RNDR_L3_SYH)
    // asm: 	LDI	404h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR7(RNDR_L4_DYH)
    // asm: 	STF	R0,*+AR7(RNDR_L4_SYH)
    // asm: 	LDI	0,AR0
    // asm: 	LDF	0.5,R0
    // asm: 	CALL	CAR_DIMMER
    // asm: 	LDI	1,AR0
    // asm: 	LDF	0.5,R0
    // asm: 	CALL	CAR_DIMMER
    // asm: 	LDI	2,AR0
    // asm: 	LDF	0.5,R0
    // asm: 	CALL	CAR_DIMMER
    // asm: 	LDI	3,AR0
    // asm: 	LDF	0.5,R0
    // asm: 	CALL	CAR_DIMMER
    // asm: 	CALL	LIGHT_INIT
    // asm: 	LDI	@CHOSEN_VEHICLE,R0
    // asm: 	STI	R0,@SPINCURR
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(PDATA)
ROUNDERLP:
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm: 	LDI	*+AR7(PDATA),R0
    // asm: 	CMPI	R0,AR2
    // asm: 	BEQ	RLL
    // asm: 	STI	AR2,*+AR7(PDATA)
    // asm: 	LDI	@DCALL,R1
    // asm: 	BZ	NODO56
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	SONDFX	HYDRO
    // asm: 	POP	R0
    // asm: 	POP	AR2
NODO56:
    // asm: 	LDI	R0,IR0
    // asm: 	MPYI	C_SIZE,IR0
    // asm: 	ADDI	C_STRT,IR0
    // asm: 	INC	IR0
    // asm: 	LDF	*+AR7(IR0),R1
    // asm: 	DEC	IR0
    // asm: 	STF	R1,*+AR7(IR0)
    // asm: 	PUSH	AR2
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm: 	POP	AR2
    // asm: 	LDI	R0,AR0
    // asm: 	LDF	0.5,R0
    // asm: 	CALL	CAR_DIMMER
    // asm: 	LDI	AR2,IR0
    // asm: 	MPYI	C_SIZE,IR0
    // asm: 	ADDI	C_STRT,IR0
    // asm: 	INC	IR0
    // asm: 	LDF	*+AR7(IR0),R0
    // asm: 	DEC	IR0
    // asm: 	FLOAT	300,R1
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR7(IR0)
    // asm: 	PUSH	AR2
    // asm: 	CALL	LIGHT_ON
    // asm: 	POP	AR2
    // asm: 	LDI	AR2,AR0
    // asm: 	LDF	1.0,R0
    // asm: 	CALL	CAR_DIMMER
RLL:
    // asm: 	CALL	AFFECT_THE_CARS
    // asm: 	SLEEP	1
    // asm: 	BU	ROUNDERLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROUNDER", 0, 0);
    UNIMPL();
}

void AFFECT_THE_CARS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *AFFECT CARS
    // *
    // *
    // *PARAMETERS
    // *	INDEX OF CAR
    // *
    // *
    // *OPERATION
    // *	RAISE VEHICLE AND SPIN
    // *
    // *
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
    // asm: 	LDI	2,IR1
    // asm: 	CALL	AFFECTED_CAR
    // asm: 	LDI	484h,AR2
    // asm: 	LDI	RNDR_C4_DYH,IR0
    // asm: 	LDI	3,IR1
    // asm: 	CALL	AFFECTED_CAR
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AFFECT_THE_CARS", 0, 0);
    UNIMPL();
}

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
    // asm: 	RS	16,R0
    // asm: 	AND	SW_VIEW0_H|SW_VIEW1_H,R0
    // asm: 	BZ	SETAS_ORIGINALS
    // asm: SETAS_HIDDEN
    // asm: 	LDI	@IS_HIDDEN,R0
    // asm: 	CMPI	-1,R0
    // asm: 	CALLNE	RESET_ORIGINAL
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // ;	CMPI	1,AR2
    // ;	BEQ	HIDDEN_VEHICLES_LP
    // asm: 	CMPI	3,AR2
    // asm: 	BEQ	HIDDEN_VEHICLES_LP
    // asm: 	STI	AR2,@IS_HIDDEN
    // asm: 	ADDI	481h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	@CHOSEN_VEHICLE,AR2
    // asm: 	ADDI	@HIDDEN_TABLEI,AR2
    // asm: 	LDI	*AR2,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	BU	HIDDEN_VEHICLES_LP
SETAS_ORIGINALS:
    // asm: 	LDI	@IS_HIDDEN,R0
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	HIDDEN_VEHICLES_LP	;no one was hidden,
    // 					;just ignore
    // asm: 	CALL	RESET_ORIGINAL
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@IS_HIDDEN
    // asm: 	BU	HIDDEN_VEHICLES_LP
    // *PARAMETERS
    // *	R0	INDEX OF PREVIOUS VEHICLES
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIDDEN_VEHICLES", 0, 0);
    UNIMPL();
}

void RESET_ORIGINAL(void)
{
    // 	;insert code here to set the old vehicle
    // 	;as
    // 	;
    // asm: 	PUSH	R0
    // asm: 	LDI	R0,AR2
    // asm: 	ADDI	481h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	POP	AR2
    // asm: 	MPYI	6,AR2
    // asm: 	ADDI	@CCTABI,AR2
    // asm: 	LDI	*+AR2(3),R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_ORIGINAL", 0, 0);
    UNIMPL();
}

void AFFECTED_CAR(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *
    // *PARAMETERS
    // *	AR2	ID
    // *	IR0	DESIRED HEIGHT OFFSET of AR7
    // *	IR1	IDX OF CAR
    // *
    // *
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
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDF	*+AR1(OPOSY),R0
    // asm: 	INC	IR0
    // asm: 	SUBF	*+AR7(IR0),R0
    // asm: 	ADDI	C_OFF2LIFT,IR0
    // asm: 	ADDF	*+AR7(IR0),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	POP	AR0
    // asm: 	POP	IR0
    // asm: 	LDI	@SPINCURR,R0
    // asm: 	LDI	@CHOSEN_VEHICLE,R1
    // asm: 	CMPI	R0,R1
    // asm: 	BEQ	IBOIBO
    // asm: 	CMPI	R0,IR1
    // asm: 	BNE	N12
    // 	;track to PI
    // asm: 	LDF	*+AR0(ORADY),R2
    // asm: 	LDLF	0.392699,R1
    // asm: 	ADDF	R1,R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	PI,R0
    // asm: 	ADDF	@START_RADY,R0
    // asm: 	PUSHFL	R2
    // asm: 	LDF	R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R2,R0
    // asm: 	POPFL	R2
    // asm: 	CALL	GETTHETADIFF
    // asm: 	PUSHFL	R2
    // asm: 	LDF	R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R2,R0
    // asm: 	POPFL	R2
    // asm: 	CMPF	0.04,R0
    // asm: 	BLT	DOALL
    // asm: 	MPYF	0.10,R0
    // asm: 	ADDF	R0,R2
    // asm: 	BU	IBO45
DOALL:
    // asm: ADDF	R0,R2
    // asm: 	LDI	@CHOSEN_VEHICLE,R1
    // asm: 	STI	R1,@SPINCURR
IBO45:
    // asm: STF	R2,*+AR0(ORADY)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
IBOIBO:
    // asm: CMPI	R1,IR1
    // asm: 	BNE	N12
    // 	;just spin
    // asm: 	LDF	*+AR0(ORADY),R2
    // asm: 	ADDF	0.1,R2
    // asm: 	STF	R2,*+AR0(ORADY)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
N12:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AFFECTED_CAR", 0, 0);
    UNIMPL();
}

void CAR_DIMMER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	AR0	CAR INDEX
    // *	R0	(FL) DIMMER VALUE
    // *
    // asm: 	LDI	AR0,AR1
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

void LIGHT_INIT(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	AR2
    // asm: 	LDL	flour_lghtof,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDI	0,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm: 	LDI	1,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm: 	LDI	2,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm: 	LDI	3,AR2
    // asm: 	CALL	LIGHT_OFF
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_INIT", 0, 0);
    UNIMPL();
}

void LIGHT_OFF(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *PARAMETERS
    // *	AR2	INDEX  (0 to 3)
    // *
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
    // asm: 	POP	AR2
    // asm: 	SUBI	200h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_OFF", 0, 0);
    UNIMPL();
}

void LIGHT_ON(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *as above
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	ADDI	601h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	SUBI	200h,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LIGHT_ON", 0, 0);
    UNIMPL();
}

void INIT_PEDALCHK(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *USES
    // *	R5	AS A DEDICATED REG
    // *
    // asm: 	CLRI	R5				;FLAG : HAS THE PEDAL BEEN RELEASEDP
    // asm: 	LDI	@_pot1,R0
    // asm: 	FIX	@PEDALMN,R1
    // asm: 	ADDI	20,R1
    // asm: 	CMPI	R1,R0
    // asm: 	LDIGE	1,R5				;GE -> IT HASN'T
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_PEDALCHK", 0, 0);
    UNIMPL();
}

void GETCHOICE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSHF	R4
    // asm: 	PUSH	R4
    // asm: 	LDI	@_MODE,R4
    // asm: 	AND	MMODE,R4
    // asm: 	FLOAT	@_pot0,R3 		;GET CURRENT WHEEL
    // asm: 	FLOAT	@POSES,R1		;GET # POSES
    // asm: 	LDF	@STEERFR,R0
    // asm: 	CALL	DIV_F
    // asm: 	LDF	R0,R1
    // asm: 	MPYF	0.5,R1
    // asm: 	FLOAT	@POSE,R2		;@CHOSEN_VEHICLE,R2
    // asm: 	MPYF	R0,R2
    // asm: 	ADDF	R1,R2	 		;MIDDLE OF ZONE
    // asm: 	ADDF	@STEERMN,R2		;ADD IN MINIMUM
    // asm: 	CMPI	MINIT,R4
    // asm: 	BEQ	GETCHA
    // asm: 	STF	R2,@WHEELPOS
GETCHA:
    // asm: 	SUBF	R3,R2
    // asm: 	ABSF	R2,R3			;FIND DIFFERENCE
    // asm: 	MPYF	0.6,R0		   	;SLIGHT HYSTERESIS
    // asm: 	CMPF	R0,R3
    // asm: 	BLE	GETCHX
    // asm: 	LDF	R2,R2
    // asm: 	LDILT	1,R0
    // asm: 	LDIGE	-1,R0
    // asm: 	ADDI	@POSE,R0		;CHOSEN_VEHICLE,R0
    // asm: 	LDFLT	0,R0
    // asm: 	CMPI	@POSES,R0
    // asm: 	BLT	GETCH1
    // asm: 	LDI	@POSES,R0
    // asm: 	SUBI	1,R0
GETCH1:
    // asm: STI	R0,@POSE
GETCHX:
    // asm: 	POP	R4
    // asm: 	POPF	R4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCHOICE", 0, 0);
    UNIMPL();
}

void PEDALCHK(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *USES
    // *	R5	see above
    // *RETURNS
    // *	NC	FALSE
    // *	C	TRUE
    // *
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
    // asm: PEDALFALSE
    // asm: 	CLRC
    // asm: 	RETS
PEDALTRUE:
    // asm: 	SETC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PEDALCHK", 0, 0);
    UNIMPL();
}

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
    // asm: 	READADJ	ADJ_FREEGAME
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	FGLL
    // asm: 	CREATE	BLINK_FREEBE,UTIL_C
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	FGLL
    // asm: 	CREATE	SHOW_RACE_NAME,UTIL_C
FGLL:
    // ;	LDI	@HEAD2HEAD_ON,R0
    // ;	CALLNZ	SEND_BSYNC0
    // asm: 	SLEEP	5
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	NOBABE
    // asm: 	CREATEC	BABE_WAVEFLAG,UTIL_C
NOBABE:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@CURR_FLAGSTATE
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	NOHEAD2HEAD
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@OM_BSYNC
H2HWTLP:
    // asm: 	SLEEP	1
    // asm: 	CALL	SEND_BSYNC3
    // asm: 	LDI	@OM_BSYNC,R0
    // asm: 	CMPI	3,R0
    // asm: 	BNE	H2HWTLP
NOHEAD2HEAD:
    // 	;-----------------------------------
    // 	;	-----------------------------------
    // 	;		-----------------------------------
    // asm: 	SONDFX	STARTLINEREVS2
    // asm: 	LDI	2,AR5
    // asm: 	LDL	TLIST,AR6
WAVEFLAGLP:
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	JDJFF
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BZ	JDJFF
JFF:
    // asm: LDI	@CURR_FLAGSTATE,R0
    // asm: 	CMPI	3,R0
    // asm: 	BEQ	JUMPOUT
    // asm: 	CMPI	@H2H_FLAGSTATE,R0
    // asm: 	BLT	NXTSTAT
    // asm: 	SLEEP	1
    // asm: 	BU	JFF
NXTSTAT:
    // asm: LDI	@H2H_FLAGSTATE,R0
    // asm: 	STI	R0,@CURR_FLAGSTATE
    // asm: 	BU	KKLFF
JDJFF:
    // asm: 	SLEEP	20
KKLFF:
    // asm: 	LDI	*AR6++,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	160,R3
    // asm: 	LDI	20,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*AR6++,AR2
    // asm: 	CALL	ONESNDFX
    // 	;if H2H_ON && MASTER
    // 	;then  SEND_STATE
    // 	;
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	BABAD
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BNZ	BABAD
    // asm: 	LDI	*AR6,R0
    // asm: 	CALLU	R0
    // asm: BABAD	ADDI	1,AR6
    // asm: 	DEC	AR5
    // asm: 	CMPI	0,AR5
    // asm: 	BGT	WAVEFLAGLP
JUMPOUT:
    // asm: LDL	TLGO,AR6
    // asm: 	SLEEP	15
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@BABE_CONTROL
    // asm: 	SLEEP	5
    // asm: 	LDI	*AR6++,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	160,R3
    // asm: 	LDI	20,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDI	*AR6++,AR2
    // asm: 	CALL	ONESNDFX
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	BABAD666
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BNZ	BABAD666
    // asm: 	CALL	SEND_WAVEFL_GO
BABAD666:
    // asm: 	OR	MGO,R5			;SAVED MODE
    // asm: 	ANDN	MSLINE,R5
    // asm: 	STI	R5,@_MODE
    // asm: 	STI	R5,@STOPWATCH_CNTL	;STOPWATCH TIMER
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	BNZ	NANAD			;RETURN IF NON MOVING
    // asm: 	CALL	CHECK_MOTION_PRESENT
    // asm: 	BNE	NANAD
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R2
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	LDL	0FF80h,R1
    // asm: 	AND	R1,R2
    // asm: 	BZ	NANAD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAVEFLAG", 0, 0);
    UNIMPL();
}

void RACESEL_TIMER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@_countdown,R2
    // asm: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm: 	CALL	_itoa
    // asm: 	BUD	IT_E2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	253,R3
    // asm: 	LDI	1,RC
    // 	;---->	BUD	IT_E2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RACESEL_TIMER", 0, 0);
    UNIMPL();
}

void WAITINTROTIMER(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	FLOAT	215,R3
    // asm: 	BU	LKJAFSD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAITINTROTIMER", 0, 0);
    UNIMPL();
}

void INTROTIMER(void)
{
    // asm: 	FLOAT	350,R3
LKJAFSD:
    // asm: 	PUSHF	R3
    // asm: 	LDI	@_countdown,R2
    // asm: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm: 	CALL	_itoa
    // asm: 	POPF	R3
    // asm: 	FLOAT	256,R2
    // asm: 	LDI	1,RC
IT_E2:
    // asm: CALL	TEXT_ADD
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	CALL	SETN43FONT
    // asm: 	LDL	lgnum43_coolyelo,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INTROTIMER", 0, 0);
    UNIMPL();
}

void DIAL_ROUT(void)
{
    // asm: 	LDI	@POSE,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	LDILT	0,AR2
    // asm: 	CMPI	3,AR2
    // asm: 	LDIGT	3,AR2
    // asm: 	STPI	AR2,@CHOSEN_VEHICLE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAL_ROUT", 0, 0);
    UNIMPL();
}

void ENDPLAYER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *END OF PLAYERS GAME
    // *
    // *	COMPUTE TIME
    // *	SHOW STATISTICS
    // *	IDENTIFY GAME OVER
    // *	HSTD IF NESSESARY
    // *	GOTO ATTRACT MODE
    // *
    // *
    // asm: 	CLRI	R2
    // asm: 	SETAUD	AUD_BCREDITS
    // asm: 	LDI	AUD_NUM_UNFINISHED,AR2
    // asm: 	CALL	AUDIT_READ
    // asm: 	DEC	R0
    // asm: 	LDI	R0,R2
    // asm: 	SETAUD	AUD_NUM_UNFINISHED
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@FRAMRATE   		;RESET FRAME RATE TO ATTRACT MODE
    // ;	LDI	-5,AR2			;HSTD SHOULD BE NEXT SCREEN!
    // asm: 	LDI	-2,AR2			;HSTD SHOULD BE NEXT SCREEN!
    // asm: 	STI	AR2,@_ATTR_MODE
    // asm: 	LDI	DRONE_C,R0
    // asm: 	LDI	CLASS_M,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	SPAWNER_C,R0
    // asm: 	LDI	CLASS_M,R1
    // asm: 	CALL	PRC_KILLALL
    // *ELP CHANGE
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CLEARRDY	  	;READY FOR INTERRUPT
KK5:
    // asm: LDI	@CLEARRDY,R0
    // asm: 	BNZ	KK5
    // *ELP END CHANGE
    // *
    // *NOW CLEAN UP THE SYSTEM,. REINITIALIZE EVERYTHING AND
    // *GO INTO ATTRACT MODE
    // *
    // asm: 	CALL	TEXT_INIT
    // ;	CLRI	AR2
    // ;	CALL	SENDSND
    // ;	SOND1	GAMEOVR
    // *ELP CHANGE
    // ;	LDL	_SECggate,AR2
    // ;	CALL	LOAD_SECTION_REQ
    // *ELP END CHANGE
    // asm: 	CLRI	R0			;R0	(B0-15) PID
    // asm: 	CLRI	R1			;R1	(B0-15) MASK
    // asm: 	CALL	PRC_KILLALL
    // asm: 	BU	SET_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENDPLAYER", 0, 0);
    UNIMPL();
}

void GAME_AVAILABLEP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GAME AVAILABLE P
    // *RETURNS
    // *	CARRY SET  CREDIT ENTER - GAME IS AVAILABLE
    // *	CARRY CLR  NO CREDIT AVAILABLE
    // *	R0	CREDITS AVAILABLE
    // *
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR2
    // asm: 	READADJ	ADJ_FREE_PLAY
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	GA_TRUE
    // asm: 	CALL	GET_CREDITS_TO_START
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	CMPI	R1,R0
    // asm: 	BLT	GA_FALSE
GA_TRUE:
    // asm: 	SETC
    // asm: 	POP	AR2
    // asm: 	POP	R2
    // asm: 	RETS
GA_FALSE:
    // asm: 	CLRC
    // asm: 	POP	AR2
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GAME_AVAILABLEP", 0, 0);
    UNIMPL();
}

void _start(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *START BUTTON
    // *
    // *
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MCT,R0
    // asm: 	BEQ	CN
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BEQ	CN
    // asm: 	CMPI	MINSERT_COINS,R0
    // asm: 	BEQ	CN
    // asm: 	CMPI	MINIT,R0
    // asm: 	BEQ	CN
    // asm: 	CMPI	MINTRO,R0
    // asm: 	BNE	NOTINTRO
CN:
    // asm: LDI	1,R0
    // asm: 	STI	R0,@START_HIT
    // asm: 	DIE
NOTINTRO:
    // asm: NOTINSRT
    // asm: 	CMPI	MGAME,R0
    // asm: 	BEQ	_startX
    // asm: 	CALL	GAME_AVAILABLEP
    // asm: 	BC	CANSTART
    // 	;NO CREDITS TO START!
    // asm: 	LDI	@TEASE_COUNT,R0
    // asm: 	CMPI	0,R0
    // asm: 	BGT	_startX
    // asm: 	INC	R0
    // asm: 	STI	R0,@TEASE_COUNT
    // asm: 	SOND1	TEASE_TURNKEY
    // asm: 	DIE
CANSTART:
    // asm: 	CALL	GET_CREDITS_TO_START
    // asm: 	SUBI	R1,R0
    // asm: 	LDI	R0,R2
    // asm: 	CMPI	0,R2
    // asm: 	LDILT	0,R2
    // asm: 	SETAUD	AUD_CREDITS		;DECREMENT CREDIT COUNT
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_BCREDITS,AR2
    // asm: 	CALL	AUDIT_WRITE
    // asm: startgame
    // asm: 	LDI	@_MODE,R0
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
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_start", 0, 0);
    UNIMPL();
}

void ULTRA_PROC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	AR4	OBJECT
    // *
    // *
    // asm: 	CLRF	R6
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ULTRA_PROC", 0, 0);
    UNIMPL();
}

void ULTRA_LOGO(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	OBJ_GET
    // asm: 	RETSC
    // asm: 	LDIL	nintendo,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	50,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	AR2,AR4
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	CREATE	ULTRA_PROC,UTIL_C
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ULTRA_LOGO", 0, 0);
    UNIMPL();
}

void LOGO_SMALL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB 	DIP_COMMP,R0
    // asm: 	RETSNZ
    // asm: 	LDL	redhd1,AR2	;red (bottom)
    // asm: 	LDI	230,R2
    // asm: 	LDI	-190-60,R3
    // asm: 	LDI	926,RC	;368*2
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	RETSC
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	LDL	yelhd1,AR2	;top
    // asm: 	LDI	230,R2
    // asm: 	LDI	-190+60,R3
    // asm: 	LDI	926,RC	;368*2
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	RETSC
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	LDL	big2,AR2
    // asm: 	LDI	230,R2
    // asm: 	LDI	-190,R3
    // asm: 	LDI	924,RC	;(368*2)-2
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	RETSC
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV|O_1PAL,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDL	H2HPAL3,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOGO_SMALL", 0, 0);
    UNIMPL();
}

void SET_ATTR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *STRAIGHT LINE CODE, SYSTEM SHUTDOWN
    // *THIS ROUTINE IS BRANCHED TO, NOT CALLED!
    // *
    // *
    // asm: 	CALL	SILENT
    // asm:         LDI	@FASTSTKI,SP		;GET PAGE OF STORED ADDRESS
    // asm: 	LDI	@_ATTR_MODE,AR2		;AND INTO FP TOO
    // asm: 	CALL	WAVE
    // asm: 	BU	COLD_ENTER
CYCLE_ATTR:
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_ATTR", 0, 0);
    UNIMPL();
}

void _debug(void)
{
    // asm: 	CALL	SILENT
    // asm: 	CALL	LOAD_FIXED_PALETTES
    // asm:         LDI	@FASTSTKI,SP		;GET PAGE OF STORED ADDRESS
    // asm: 	LDI	@_ATTR_MODE,AR2		;AND INTO FP TOO
    // asm: 	DEC	AR2
    // asm: 	CMPI	-4,AR2
    // asm: 	LDILT	-1,AR2
    // asm: 	STI	AR2,@_ATTR_MODE
    // asm: 	CALL	WAVE
    // asm: 	BU	COLD_ENTER		;RESET SYSTEM RUNNING
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_debug", 0, 0);
    UNIMPL();
}

void _timeout(void)
{
    // asm: 	LDI	@_timer,AR2
    // asm: 	CALL	SLEEP
    // asm: 	BU	CYCLE_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_timeout", 0, 0);
    UNIMPL();
}

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
    // asm: 	STI	R0,@WAITTIK
KFFDA:
    // 	;
    // asm: 	LDF	@GAME_TIMER,R2
    // asm: 	MPYF	100,R2
    // asm: 	FIX	R2
    // asm: 	READAUD	AUD_TOTAL_TIME
    // asm: 	ADDI	R0,R2
    // asm: 	SETAUD	AUD_TOTAL_TIME
    // asm: 	READAUD	AUD_NUM_BUYINS
    // asm: 	LDI	R0,R1
    // asm: 	LDI	R2,R0
    // asm: 	CALL	DIV_I30
    // asm: 	LDI	R0,R2
    // asm: 	SETAUD	AUD_AVG_TIME
    // 	;
    // asm: 	LDI	20,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	CALL	SILENT
    // asm: 	SOND1	DISCODUCK
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@STOPWATCH_CNTL
    // asm: 	STI	R0,@START_HIT
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MMODE,R0
    // asm: 	OR	MINSERT_COINS|MGO,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	@TROI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	100,R3
    // asm: 	LDI	9999,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDI	AR0,AR4			;SAVE POINTERS FOR TEXT CHANGE
    // asm: 	LDI	@ICCI,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	150,R3
    // asm: 	LDI	9999,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SET40FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	CREATEC	COIN_CNTDOWN,034h
    // asm: 	READAUD	AUD_BCREDITS
    // asm: 	LDI	R0,R4			;WATCH CREDITS
    // asm: 	CLRI	R5			;TO START TOGGLE
    // asm: 	LDI	15,AR6			;wait at least 15 frames to continue
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@miniidle
    // 	;wait for a coin to be dropped in
    // 	;if coins are in change text to PRESS START
INSMORE_LP:
    // asm: 	CALL	INSERT_MORE_COINS
    // asm: 	READAUD	AUD_BCREDITS
    // asm: 	CMPI	R0,R4
    // asm: 	BEQ	NOINCTIM
    // asm: 	LDI	R0,R4
    // asm: 	LDI	20,R0
    // asm: 	STI	R0,@_countdown
NOINCTIM:
    // asm: 	LDI	R5,R5
    // asm: 	BNZ	CHECKHIT
    // asm: 	READADJ	ADJ_FREE_PLAY
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	FREEP
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm: 	CMPI	R1,R0
    // asm: 	BLT	JUSTGOON
    // ;	CMPI	0,R0
    // ;	BEQ	JUSTGOON
FREEP:
    // asm: 	LDI	@PSCI,R0		;change the text
    // asm: 	STI	R0,*+AR4(TEXT_PTR)
    // asm: 	LDI	1,R5
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	OR	BUT_START,R0
    // asm: 	STI	R0,@BUTTON_STATUS
JUSTGOON:
    // 	;check to see if plyr hit start (decrement count)
    // asm: 	LDI	R5,R5
    // asm: 	BNZ	CANWT
TOSLP:
    // 	;HITTING THE BUTTON W/O CREDITS
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BZ	NRST
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_HIT
    // asm: 	LDI	20,R0
    // asm: 	STI	R0,@_countdown
NRST:
    // asm: 	LDI	@miniidle,R0	;only every 10th frame
    // asm: 	CMPI	0,R0
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm: 	LDL	SW_RADIO|SW_VIEW0|SW_VIEW1|SW_VIEW2,R1
    // asm: 	AND	R1,R0
    // asm: 	BZ	TOSLP2
    // asm: 	CMPI	R1,R0		;but if ALL are pressed...dont decrement
    // asm: 	BEQ	TOSLP2
    // asm: 	LDI	@_countdown,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@_countdown
TOSLP2:
    // asm: 	DEC	AR6
    // asm: 	CMPI	0,AR6
    // asm: 	BGT	CANWT
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm: 	TSTB	SW_START,R0
    // asm: 	BZ	CANWT
    // ;	LDI	AR5,R0		;if no decrement -> DONT
    // ;	BNZ	CANWT
    // asm: 	LDI	@_countdown,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@_countdown
    // asm: 	LDI	15,AR6			;wait at least 15 frames to continue
CANWT:
    // asm: 	LDI	@miniidle,R0
    // asm: 	INC	R0
    // asm: 	CMPI	25,R0
    // asm: 	LDIGE	0,R0
    // asm: 	STI	R0,@miniidle
    // asm: 	SLEEP	1
    // asm: 	LDI	@_countdown,R0
    // asm: 	BNZ	INSMORE_LP
    // asm: 	CLRI	AR6
    // asm: 	BU	RETURNTOPLYR
CHECKHIT:
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BZ	TOSLP
    // 	;Secret Button Combo!!
    // 	;
    // 	;if the plyr holds all the view buttons and radio as he
    // 	;hits the start button, he gets to CRUISE the USA
    // 	;-ELP August 11,1994
    // 	;
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm: 	LDIL	(SW_RADIO|SW_VIEW0|SW_VIEW1|SW_VIEW2),R1
    // asm: 	AND	R1,R0
    // asm: 	CMPI	R1,R0
    // asm: 	BNE	NOSECRET_CRUISE
    // asm: 	LDI	RM_USA,R0
    // asm: 	STI	R0,@RACE_MODE
NOSECRET_CRUISE:
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_BCREDITS,AR2
    // asm: 	CALL	AUDIT_WRITE
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	CALL	GET_CREDITS_TO_CONTINUE
    // asm: 	SUBI	R1,R0
    // ;	DEC	R0
    // asm: 	LDILT	0,R0
    // asm: 	LDI	R0,R2
    // asm: 	SETAUD	AUD_CREDITS
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MMODE,R0
    // asm: 	OR	MGAME,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@STOPWATCH_CNTL
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	LDI	1,AR6
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	ANDN	BUT_START,R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	INCAUD	AUD_NUM_BUYINS
RETURNTOPLYR:
    // asm: 	LDI	@SAVEDMODE,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	034h,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	CALL	TEXT_INIT
    // asm: 	CALL	RESUME_TUNE_NT
    // asm: 	CLRF	R0
    // asm: 	STPF	R0,@GAME_TIMER
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSMORE", 0, 0);
    UNIMPL();
}

void COIN_CNTDOWN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PLACE THE COUNTDOWN TIMER
    // *
    // *
    // asm: 	LDI	@_countdown,R4
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
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	SLEEP	1
    // asm: 	BU	COIN_CNTDOWN_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COIN_CNTDOWN", 0, 0);
    UNIMPL();
}

void LOAD_SHARED(void)
{
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@DIRTY_SHARED
    // asm: 	LDL	_SECshared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	bottom_gtmp_p,R0		;ONE TO OVERWRITE
    // asm: 	LDI	R0,R1			;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	bottom2_gtmp_p,R0
    // asm: 	LDI	R0,R1
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	shldr2_gtmp_p,R0
    // asm: 	LDI	R0,R1
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_SHARED", 0, 0);
    UNIMPL();
}

void TRAFFIC_LIGHT(void)
{
    // asm: 	SLEEP	20
    // asm: 	LDL	TRAFFIC_LL,AR5
TLT_LP:
    // asm: 	LDI	light_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	BNC	NSSD
    // asm: 	BR	SUICIDE
NSSD:
    // asm: 	LDI	*AR5++,AR2
    // asm: 	CMPI	-1,AR2
    // asm: 	BNE	CCC
    // asm: 	LDL	TRAFFIC_LL,AR5
    // asm: 	LDI	*AR5++,AR2
CCC:
    // asm: 	LDI	R0,R2
    // asm: 	LDIL	8000000Ah,R3	;16
    // asm: 	CALL	PAL_SET
    // asm: 	LDI	*AR5++,AR2
    // asm: 	CALL	SLEEP
    // asm: 	BU	TLT_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRAFFIC_LIGHT", 0, 0);
    UNIMPL();
}

void CPOINT_LIGHT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@RGBTAB_CPI,AR4
    // asm: 	INC	AR4
    // asm: 	LDI	checks_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	BNC	SDASDFA
    // asm: 	BR	SUICIDE
SDASDFA:
    // asm: 	LDI	R0,AR6
CPL_LP:
    // asm: 	LDI	AR4,AR2
    // asm: 	LDI	AR6,R2
    // asm: 	ADDI	251,R2
    // asm: 	LDI	5,R3
    // asm: 	CALL	PAL_SET
    // asm: 	LDI	*++AR4(5),R0
    // asm: 	BNN	CNT
    // asm: 	LDI	@RGBTAB_CPI,AR4
CNT:
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CALL	SLEEP
    // asm: 	B	CPL_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CPOINT_LIGHT", 0, 0);
    UNIMPL();
}

void SHOW_RACE_NAME(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // *
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
    // asm: 	FLOAT	-100,R6
    // asm: 	LDI	20,AR6
SLLP1:
    // asm: FLOAT	256,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	R0,R6
    // asm: 	STF	R6,*+AR5(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR4(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR6,SLLP1
    // 	;CENTER IT
    // 	;
    // asm: 	FLOAT	256,R6
    // asm: 	STF	R6,*+AR5(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR4(TEXT_POSX)
    // asm: 	SLEEP	50
    // asm: 	LDI	20,AR6
SLLP1A:
    // asm: FLOAT	-100,R0
    // asm: 	SUBF	R6,R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	R0,R6
    // asm: 	STF	R6,*+AR5(TEXT_POSX)
    // asm: 	LDF	R6,R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR4(TEXT_POSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR6,SLLP1A
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOW_RACE_NAME", 0, 0);
    UNIMPL();
}

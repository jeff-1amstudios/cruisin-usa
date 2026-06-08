#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
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
#include "../include/dirq.h"
#include "../include/hud.h"

/*
 * Source module: asm/HUD.ASM
 */

// *----------------------------------------------------------------------------
// *HEADS UP DISPLAY ROUTINES
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
#define SECTION_COUNTER 0
/* asm: COUNTDOWN_BUFI	.word	COUNTDOWN_BUF */
int COUNTDOWN_BUFI = (int)(COUNTDOWN_BUF);
/* asm: MPH_BUFFERI	.word	MPH_BUFFER */
int MPH_BUFFERI = (int)(MPH_BUFFER);
/* asm: STOPBUFFI	.word	STOPBUFFER */
int STOPBUFFI = (int)(STOPBUFFER);
const char YOURLOSTTXT[] = "YOU ARE LOST";
const char OFFROADTXT[] = "OFFROAD";
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------

void MOVEIN_HUD_EQUIP(void)
{
    // asm: 	LDI	150,R0
    // asm: 	STI	R0,@MOVEIN_OFFSET
LIU8:
    // asm: LDI	@_MODE,R0
    // asm: 	TSTB	MHUD,R0
    // asm: 	BNZ	M2L
    // asm: 	SLEEP	1
    // asm: 	BU	LIU8
M2L:
    // asm: LDI	25-1,AR5
    // asm: MIHEL
    // asm: 	LDI	@MOVEIN_OFFSET,R0
    // asm: 	SUBI	6,R0
    // asm: 	STI	R0,@MOVEIN_OFFSET
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,MIHEL
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@MOVEIN_OFFSET
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVEIN_HUD_EQUIP", 0, 0);
    UNIMPL();
}

void MOVEOUT_HUD_EQUIP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	25-1,AR5
    // asm: MIHEL2
    // asm: 	LDI	@MOVEIN_OFFSET,R0
    // asm: 	ADDI	6,R0
    // asm: 	STI	R0,@MOVEIN_OFFSET
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,MIHEL2
    // asm: 	LDI	150,R0
    // asm: 	STI	R0,@MOVEIN_OFFSET
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVEOUT_HUD_EQUIP", 0, 0);
    UNIMPL();
}

void HUD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *HEADS UP DISPLAY ROUTINE
    // *CALL FROM MAIN LOOP
    // *
    // *SHOULD ONLY BE CALLED DURING GAME PLAY
    // *
    // asm: 	CALL	TACHOMETER_ANIMATE
    // asm: 	CALL	RADAR_PLOT
    // 	;
    // 	;MOTION SAFETY ON
    // 	;
    // asm: 	READAUD	ADJ_MOTION_PRESENT
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NOMOTIONMSGS
    // asm: 	LDI	@MOTION_STOP_HIT,R0
    // asm: 	BNZ	NOMOTIONMSGS
    // asm: 	LDI	@MOTION_SAFETY_ON,R0
    // asm: 	BZ	BLAHBLAH
    // asm: 	LDI	@SAFETSI,AR2
    // asm: 	ADDI	@MOTION_SAFETY_TYPE,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	310,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
BLAHBLAH:
    // 	;
    // 	;
NOMOTIONMSGS:
    // 	;-------OFF ROAD TIMER
    // 	;
    // 	;
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	NOT_OFFROAD
    // asm: 	LDI	@OFFROAD_TMR,R2
    // asm: 	CMPI	10,R2
    // asm: 	BGE	NOT_OFFROAD
    // asm: 	CMPI	3,R2
    // asm: 	BGT	NOTLOST
    // asm: 	LDL	YOURLOSTTXT,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	5,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SET18FONTDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	BU	OFJN
NOTLOST:
    // asm: 	LDL	OFFROADTXT,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	5,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SET18FONTDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
OFJN:
    // asm: 	LDI	@OFFROAD_TMR,R2
    // asm: 	LDL	OFFROADBUFF,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	30,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
NOT_OFFROAD:
    // 	;-------ELAPSED TIME,SPEED TEXT
    // 	;
    // 	;
    // asm: 	LDL	rpm,AR2
    // asm: 	LDI	420,R2			;R2	POS X
    // asm: 	ADDI	@MOVEIN_OFFSET,R2
    // asm: 	LDI	290,R3			;R3	POS Y
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
    // asm: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm: 	CMPI	AUTO_TRANSMISSION,R0
    // asm: 	BEQ	ISAUTOTRNS
    // asm: 	LDL	rpmman,AR2
    // asm: 	LDI	463,R2			;R2	POS X
    // asm: 	ADDI	@MOVEIN_OFFSET,R2
    // asm: 	LDI	380,R3			;R3	POS Y
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
    // asm: 	BU	DNDNL
ISAUTOTRNS:
    // asm: 	LDL	rpmauto,AR2
    // asm: 	LDI	470,R2			;R2	POS X
    // asm: 	ADDI	@MOVEIN_OFFSET,R2
    // asm: 	LDI	380,R3			;R3	POS Y
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
DNDNL:
    // 	;show gear digit
    // 	;
    // 	;
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDI	*+AR0(CARGEAR),AR2
    // asm: 	ADDI	@GEARI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	FLOAT	458,R2			;R2	POS X
    // asm: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm: 	ADDF	R0,R2
    // asm: 	FLOAT	360,R3			;R3	POS Y
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SETSMDIGITFONT
    // 	;show MPH or KPH
    // 	;
    // asm: 	LDL	mph,AR2
    // asm: 	LDI	0,R2			;R2	POS X
    // asm: 	SUBI	@MOVEIN_OFFSET,R2
    // asm: 	LDI	335,R3			;R3	POS Y
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
    // asm: 	READAUD	ADJ_MPHORKPM
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	ISMPHT
    // asm: 	LDL	kph,AR2
    // asm: 	LDI	50,R2			;R2	POS X
    // asm: 	SUBI	@MOVEIN_OFFSET,R2
    // asm: 	LDI	385,R3			;R3	POS Y
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
    // asm: 	BU	ISDNN
ISMPHT:
    // asm: 	LDL	mphmph,AR2
    // asm: 	LDI	50,R2			;R2	POS X
    // asm: 	SUBI	@MOVEIN_OFFSET,R2
    // asm: 	LDI	385,R3			;R3	POS Y
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
ISDNN:
    // 	;-------time remaining
    // 	;
    // asm: 	LDL	time,AR2
    // asm: 	LDI	242,R2			;R2	POS X
    // asm: 	LDI	9,R3			;R3	POS Y
    // asm: 	SUBI	@MOVEIN_OFFSET,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // 	;-------TIME (_countdown)
    // 	;
    // 	;
    // asm: 	LDI	@_countdown,R2
    // asm: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	24,R3
    // asm: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm: 	SUBF	R0,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
    // asm: 	CALL	SETN43FONT
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	LDL	lgnum43_coolyelo,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // 	;-------LAST 3/10 SECONDS OF TIME (_countdown)
    // 	;
    // 	;
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	NBEEP
    // asm: 	LDI	@_countdown,R0
    // asm: 	CMPI	10,R0
    // asm: 	BGT	NBEEP
    // asm: 	BLT	NOINITBP
    // asm: 	LDI	@LASTSEC,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BEQ	NBEEP
    // asm: 	STI	R0,@LASTSEC
    // asm: 	SOND1	GV_TENSECONDS
    // asm: 	BU	NBEEP
NOINITBP:
    // asm: 	LDI	@LASTSEC,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BEQ	NBEEP
    // asm: 	STI	R0,@LASTSEC
    // asm: 	CMPI	3,R0
    // asm: 	BGT	NBEEP
    // asm: 	SOND1	BASICBEEPH
NBEEP:
    // 	;-------SPEED
    // 	;
    // 	;
    // 	;PLOT SPEED
    // asm: 	READAUD	ADJ_MPHORKPM
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	ISMPH
    // asm: 	FLOAT	@_MPH,R2
    // asm: 	MPYF	1.6666,R2
    // asm: 	FIX	R2
    // asm: 	BU	ISKPH
ISMPH:
    // asm: LDI	@_MPH,R2
ISKPH:
    // asm: LDI	@MPH_BUFFERI,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	66,R2
    // asm: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm: 	SUBF	R0,R2
    // asm: 	FLOAT	346,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SETLGDIGITFONTDS
    // asm: 	ORM	TXT_RIGHT,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_RIGHT,*+AR1(TEXT_COLOR)
    // asm: 	LDL	dnums_amber,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // 	;-------STOPWATCH (actual elapsed time)
    // 	;
    // 	;
    // asm: 	LDI	8,R0
    // asm: 	LDI	19,R1
    // asm: 	SUBI	@MOVEIN_OFFSET,R1
    // asm: 	LDI	106,R2
    // asm: 	LDI	30,R3
    // asm: 	CALL	FILL_DITHER
    // asm: 	LDL	elap,AR2
    // asm: 	LDI	10,R2			;R2	POS X
    // asm: 	LDI	20,R3			;R3	POS Y
    // asm: 	SUBI	@MOVEIN_OFFSET,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	LDI	@STOPWATCH,R0
    // asm: 	LDI	@STOPBUFFI,AR2
    // asm: 	CALL	TIME2STR
    // asm: 	FLOAT	20,R2
    // asm: 	FLOAT	33,R3
    // asm: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm: 	SUBF	R0,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	SETSMDIGITFONTDS
    // 	;-------POSITION
    // 	;(PLAYERS RANK)
    // 	;
    // asm: 	LDI	@POSITION,AR4
    // asm: 	SUBI	1,AR4
    // asm: 	MPYI	3,AR4
    // asm: 	ADDI	@POS_TABLEI,AR4
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	415,R2
    // asm: 	ADDI	*+AR4,R2
    // asm: 	LDI	20,R3
    // asm: 	SUBI	@MOVEIN_OFFSET,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	450,R2
    // asm: 	LDI	20,R3
    // asm: 	SUBI	@MOVEIN_OFFSET,R3
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D_DS
    // asm: POS_TABLE
#if SECTION_COUNTER
    // 	;-------DEBUGGING SECTION_COUNTER
    // 	;
    // 	;
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDI	*+AR0(CARTRAK),AR0
    // asm: 	LDI	*+AR0(OUSR1),R2
    // asm: 	RS	8,R2
    // asm: 	LDL	SECIDX,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	10,R2
    // asm: 	FLOAT	50,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
#endif
    // 	;-------LOGIC: MAXMPH COMPUTATION
    // 	;(COMPUTE FOR BONUS SCREEN...)
    // 	;
    // 	;
    // asm: 	FLOAT	@_MPH,R2
    // asm: 	MPYF	@CHEAT,R2		;DO THE CHEAT THING !!!
    // asm: 	FLOAT	@NFRAMES,R0
    // asm: 	MPYF	R0,R2
    // asm: 	ADDF	@MAXMPH,R2
    // asm: 	STF	R2,@MAXMPH
    // asm: 	ADDF	@MAXMPH_COUNT,R0
    // ;	LDF	@MAXMPH_COUNT,R0
    // ;	INCF	R0
    // asm: 	STF	R0,@MAXMPH_COUNT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HUD", 0, 0);
    UNIMPL();
}

void SECTION_PALETTE_ALLOC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *ALLOCATE PALETTES FOR A SECTION
    // *
    // *PARAMETERS
    // *	AR2	PTR TO SECTION HEADER
    // *
    /* no executable asm lines detected */
    UNIMPL();
}

void alloc_section(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void HARDalloc_section(void)
{
    // asm: 	LDI	*AR2++,AR6
    // asm: 	LDI	*AR2,AR5
    // asm: 	DEC	AR5
    // asm: alloc_LPA
    // asm: 	LDI	AR6,AR2
    // asm: 	CALL	PAL_ALLOC
    // asm: 	INC	AR6
    // asm: 	DBU	AR5,alloc_LPA
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HARDalloc_section", 0, 0);
    UNIMPL();
}

void dealloc_section(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DEALLOCATE PALETTES USED FOR A SECTION
    // *
    // *
    // *PARAMETERS
    // *	AR2	SECTION POINTER
    // *
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	LDI	*AR2++,AR6
    // asm: 	LDI	*AR2,AR5
    // asm: 	DEC	AR5
    // asm: deal_LP
    // asm: 	LDI	AR6,AR2
    // asm: 	CALL	PAL_DELETE
    // asm: 	INC	AR6
    // asm: 	DBU	AR5,deal_LP
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "dealloc_section", 0, 0);
    UNIMPL();
}

void TACHOMETER_ANIMATE(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDL	TACH_GRADIENT,AR0
    // asm: 	LDL	TACHOMETER_PAL,AR1
    // asm: 	LDI	22-1,RC
    // asm: 	RPTB	LP89
    // asm: 	LDI	*AR0++,R0
LP89:
    // asm: STI	R0,*AR1++
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDF	*+AR0(CARRPM),R0
    // asm: 	MPYF	0.4583,R0		;22/48
    // ;	MPYF	0.729,R0		;35/48
    // ;	MPYF	0.6041,R0		;29/48
    // asm: 	FIX	R0
    // asm: 	CMPI	22,R0
    // asm: 	LDIGT	22,R0
    // asm: 	CMPI	0,R0
    // asm: 	LDILT	0,R0
    // asm: 	LDI	R0,RC
    // asm: 	SUBRI	22,RC
    // asm: 	LDL	TACHOMETER_PAL,AR1
    // asm: 	ADDI	R0,AR1
    // asm: 	CLRI	R1
    // asm: 	RPTS	RC
    // asm: 	STI	R1,*AR1++
    // asm: 	LDI	rpm_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	PUSH	R0
    // asm: 	LDI	R0,R2
    // asm: 	ADDI	256-22,R2
    // asm: 	LDL	TACHOMETER_PAL,AR2
    // asm: 	LDI	22,R3
    // asm: 	CALL	PAL_SET
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDI	*+AR0(CARGEAR),R0
    // asm: 	CLRI	R1		;ASSUME NEUTRAL
    // asm: 	LDL	GEARPAL,AR2
    // asm: 	CMPI	1,R0
    // asm: 	LDIEQ	4,R1
    // asm: 	CMPI	2,R0
    // asm: 	LDIEQ	3,R1
    // asm: 	CMPI	3,R0
    // asm: 	LDIEQ	2,R1
    // asm: 	CMPI	4,R0
    // asm: 	LDIEQ	1,R1
    // asm: 	POP	R2
    // asm: 	ADDI	1,R2
    // asm: 	ADDI	R1,AR2
    // asm: 	LDI	4,R3
    // asm: 	CALL	PAL_SET
    // asm: 	RETS
    // asm: GEARPAL
    // asm: 	RGB	0,255,255
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TACHOMETER_ANIMATE", 0, 0);
    UNIMPL();
}

void FILL_DITHER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	R0	X POS
    // *	R1	Y POS
    // *	R2	X WIDTH
    // *	R3	Y WIDTH
    // *
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // 	;background
    // asm: 	LDI	CC|DITHER|8,R4
    // asm: 	BD	ENTER2
    // asm: 	STI	R4,@_ACNTL
    // asm: 	CLRI	R4
    // asm: 	STI	R4,@_ACMAP
    // 	;---->	BD	ENTER2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FILL_DITHER", 0, 0);
    UNIMPL();
}

void FILL_PLOT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // ;	;background
    // ;	LDI	CC|DITHER|8,R4
    // ;	STI	R4,@_ACNTL
ENTER2:
    // asm: 	STI	R0,@(_ARPS+(0*3))
    // asm: 	STI	R0,@(_ARPS+(3*3))
    // asm: 	ADDI	R2,R0
    // asm: 	STI	R0,@(_ARPS+(1*3))
    // asm: 	STI	R0,@(_ARPS+(2*3))
    // asm: 	STI	R1,@(_ARPS+(0*3)+1)
    // asm: 	STI	R1,@(_ARPS+(1*3)+1)
    // asm: 	ADDI	R3,R1,R0
    // asm: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_ADDRL
    // asm: 	CALL	_stuff_fpga
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FILL_PLOT", 0, 0);
    UNIMPL();
}

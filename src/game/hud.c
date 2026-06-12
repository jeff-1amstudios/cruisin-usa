#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
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
#include "dirq.h"
#include "hud.h"
#include "discovered_defines.h"
#include "discovered_labels.h"

/*
 * Source module: asm/HUD.ASM
 */

void MOVEIN_HUD_EQUIP(void);
void M2L(void);
void MOVEOUT_HUD_EQUIP(void);
void HUD(void);
void NOMOTIONMSGS(void);
void NOTLOST(void);
void ISAUTOTRNS(void);
void ISMPHT(void);
void NOINITBP(void);
void ISMPH(void);
void SECTION_PALETTE_ALLOC(void);
void alloc_section(void);
void HARDalloc_section(void);
void dealloc_section(void);
void TACHOMETER_ANIMATE(void);
void FILL_DITHER(void);
void FILL_PLOT(void);

#define SECTION_COUNTER 0 //1 = TURN ON UL CRNR
/* asm: COUNTDOWN_BUF	.bss	COUNTDOWN_BUF,2 */
int COUNTDOWN_BUF[2];
/* asm: MPH_BUFFER	.bss	MPH_BUFFER,2 */
int MPH_BUFFER[2];
/* asm: _countdown	.bss	_countdown,1 */
int _countdown;
/* asm: _MPH	.bss	_MPH,1 */
int _MPH;
/* asm: SCORE	.bss	SCORE,1 */
int SCORE;
/* asm: POSITION	.bss	POSITION,1 */
int POSITION;
/* asm: MAXMPH_COUNT	.bss	MAXMPH_COUNT,1 */
int MAXMPH_COUNT;
/* asm: STOPBUFFI	.word	STOPBUFFER */
#define STOPBUFFI STOPBUFFER
/* asm: STOPBUFFER	.bss	STOPBUFFER,4 */
int STOPBUFFER[4];
const char *YOURLOSTTXT = "YOU ARE LOST";
const char *OFFROADTXT = "OFFROAD";
/* asm: OFFROADBUFF	.bss	OFFROADBUFF,2 */
int OFFROADBUFF[2];
/* *----------------------------------------------------------------------------
 */
/* asm: MOVEIN_OFFSET	.bss	MOVEIN_OFFSET,1 */
int MOVEIN_OFFSET;
/* asm: SAFETS	.word	MSAF,BSAF,OSAF,MSSM */
const char * *SAFETS[] = {
    MSAF, BSAF, OSAF, MSSM,
};
const char *MSAF = "SAFETY MAT ACTIVATED";
const char *BSAF = "SAFETY BEAM ACTIVATED";
const char *OSAF = "FAIL SAFE SWITCH ACTIVATED";
const char *MSSM = "MOTION STOP BUTTON HIT";
/* asm: GEARI	.word	GEARS */
#define GEARI GEARS
/* asm: GEARS	.word	GEARN,GEAR1,GEAR2,GEAR3,GEAR4 */
const char * *GEARS[] = {
    GEARN, GEAR1, GEAR2, GEAR3, GEAR4,
};
const char *GEAR1 = "1";
const char *GEAR2 = "2";
const char *GEAR3 = "3";
const char *GEAR4 = "4";
const char *GEARN = ";";
/* asm: LASTSEC	.bss	LASTSEC,1 */
int LASTSEC;
/* asm: POS_TABLEI	.word	POS_TABLE */
#define POS_TABLEI POS_TABLE
/* asm: POS_TABLE */
/* asm: 	.word	scred1,dst,7 */
/* asm: 	.word	scred2,dnd,0 */
/* asm: 	.word	scred3,drd,0 */
/* asm: 	.word	scred4,dth,0 */
/* asm: 	.word	scred5,dth,0 */
/* asm: 	.word	scred6,dth,0 */
/* asm: 	.word	scred7,dth,0 */
/* asm: 	.word	scred8,dth,0 */
/* asm: 	.word	scred9,dth,0 */
/* asm: 	.word	scred10,dth,-20 */
int POS_TABLE[] = {
    scred1, dst, 7,
    scred2, dnd, 0,
    scred3, drd, 0,
    scred4, dth, 0,
    scred5, dth, 0,
    scred6, dth, 0,
    scred7, dth, 0,
    scred8, dth, 0,
    scred9, dth, 0,
    scred10, dth, -20,
};
#if SECTION_COUNTER
/* 	;-------DEBUGGING SECTION_COUNTER
	;
	;
 */
/* asm: SECIDX	.bss	SECIDX,2 */
int SECIDX[2];
#endif
/* asm: TACHOMETER_PAL	.bss	TACHOMETER_PAL,32 */
int TACHOMETER_PAL[32];
/* asm: GEARPAL */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
/* asm: 	.word	0 */
int GEARPAL[] = {
    0,
    0,
    0,
    0,
};

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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVEIN_HUD_EQUIP", 0, 0);
    UNIMPL();
}

void M2L(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "M2L", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MOVEOUT_HUD_EQUIP(void)
{
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

/* *----------------------------------------------------------------------------
*HEADS UP DISPLAY ROUTINE
*CALL FROM MAIN LOOP
*
*SHOULD ONLY BE CALLED DURING GAME PLAY
*
 */
void HUD(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HUD", 0, 0);
    UNIMPL();
}

/* 	;
	;
 */
void NOMOTIONMSGS(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOMOTIONMSGS", 0, 0);
    UNIMPL();
}

void NOTLOST(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTLOST", 0, 0);
    UNIMPL();
}

void ISAUTOTRNS(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISAUTOTRNS", 0, 0);
    UNIMPL();
}

void ISMPHT(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISMPHT", 0, 0);
    UNIMPL();
}

void NOINITBP(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOINITBP", 0, 0);
    UNIMPL();
}

void ISMPH(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISMPH", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*ALLOCATE PALETTES FOR A SECTION
*
*PARAMETERS
*	AR2	PTR TO SECTION HEADER
*
 */
void SECTION_PALETTE_ALLOC(void)
{
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

/* *----------------------------------------------------------------------------
*DEALLOCATE PALETTES USED FOR A SECTION
*
*
*PARAMETERS
*	AR2	SECTION POINTER
*
 */
void dealloc_section(void)
{
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

/* *----------------------------------------------------------------------------
 */
void TACHOMETER_ANIMATE(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TACHOMETER_ANIMATE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	R0	X POS
*	R1	Y POS
*	R2	X WIDTH
*	R3	Y WIDTH
*
*
 */
void FILL_DITHER(void)
{
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

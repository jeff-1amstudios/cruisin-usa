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
#include "../include/discovered_defines.h"

/*
 * Source module: asm/HUD.ASM
 */

// *----------------------------------------------------------------------------
// *HEADS UP DISPLAY ROUTINES
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
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
// *----------------------------------------------------------------------------
/* asm: MOVEIN_OFFSET	.bss	MOVEIN_OFFSET,1 */
int MOVEIN_OFFSET;
/* asm: SAFETS	.word	MSAF,BSAF,OSAF,MSSM */
int SAFETS[] = {
    MSAF, BSAF, OSAF, MSSM,
};
const char *MSAF = "SAFETY MAT ACTIVATED";
const char *BSAF = "SAFETY BEAM ACTIVATED";
const char *OSAF = "FAIL SAFE SWITCH ACTIVATED";
const char *MSSM = "MOTION STOP BUTTON HIT";
// 	;
// 	;
// 	;-------OFF ROAD TIMER
// 	;
// 	;
// 	;-------ELAPSED TIME,SPEED TEXT
// 	;
// 	;
// 	;show gear digit
// 	;
// 	;
/* asm: GEARI	.word	GEARS */
#define GEARI GEARS
/* asm: GEARS	.word	GEARN,GEAR1,GEAR2,GEAR3,GEAR4 */
int GEARS[] = {
    GEARN, GEAR1, GEAR2, GEAR3, GEAR4,
};
const char *GEAR1 = "1";
const char *GEAR2 = "2";
const char *GEAR3 = "3";
const char *GEAR4 = "4";
const char *GEARN = ";";
// 	;show MPH or KPH
// 	;
// 	;-------time remaining
// 	;
// 	;-------TIME (_countdown)
// 	;
// 	;
// 	;-------LAST 3/10 SECONDS OF TIME (_countdown)
// 	;
// 	;
/* asm: LASTSEC	.bss	LASTSEC,1 */
int LASTSEC;
// 	;-------SPEED
// 	;
// 	;
// 	;PLOT SPEED
// 	;-------STOPWATCH (actual elapsed time)
// 	;
// 	;
// 	;-------POSITION
// 	;(PLAYERS RANK)
// 	;
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
// 	;-------DEBUGGING SECTION_COUNTER
// 	;
// 	;
/* asm: SECIDX	.bss	SECIDX,2 */
int SECIDX[2];
#endif
// 	;-------LOGIC: MAXMPH COMPUTATION
// 	;(COMPUTE FOR BONUS SCREEN...)
// 	;
// 	;
// ;	LDF	@MAXMPH_COUNT,R0
// ;	INCF	R0
// *----------------------------------------------------------------------------
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

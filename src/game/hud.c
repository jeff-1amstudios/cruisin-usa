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
void ISMPH(void);
#define SECTION_PALETTE_ALLOC HARDalloc_section
#define alloc_section HARDalloc_section
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
const char *SAFETS[] = { "SAFETY MAT ACTIVATED", "SAFETY BEAM ACTIVATED", "FAIL SAFE SWITCH ACTIVATED", "MOTION STOP BUTTON HIT" };
/* asm: GEARI	.word	GEARS */
#define GEARI GEARS
/* asm: GEARS	.word	GEARN,GEAR1,GEAR2,GEAR3,GEAR4 */
const char *GEARS[] = { ";", "1", "2", "3", "4" };
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
/* asm: .word	0 */
/* asm: .word	0 */
/* asm: .word	0 */
/* asm: .word	0 */
/* asm: RGB	0,255,255 */
int GEARPAL;

void MOVEIN_HUD_EQUIP(void)
{
    // asm 00009D0E: 	LDI	150,R0
    // asm 00009D0F: 	STI	R0,@MOVEIN_OFFSET
LIU8:
    // asm 00009D10: LDI	@_MODE,R0
    // asm 00009D11: 	TSTB	MHUD,R0
    // asm 00009D12: 	BNZ	M2L
    // asm 00009D13: 	SLEEP	1
    // asm 00009D15: 	BU	LIU8
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVEIN_HUD_EQUIP", 0, 0);
    UNIMPL();
}

void M2L(void)
{
    // asm 00009D16: LDI	25-1,AR5
    // asm 00009D17: MIHEL
    // asm 00009D17: 	LDI	@MOVEIN_OFFSET,R0
    // asm 00009D18: 	SUBI	6,R0
    // asm 00009D19: 	STI	R0,@MOVEIN_OFFSET
    // asm 00009D1A: 	SLEEP	1
    // asm 00009D1C: 	DBU	AR5,MIHEL
    // asm 00009D1D: 	CLRI	R0
    // asm 00009D1E: 	STI	R0,@MOVEIN_OFFSET
    // asm 00009D1F: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "M2L", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MOVEOUT_HUD_EQUIP(void)
{
    // asm 00009D20: 	LDI	25-1,AR5
    // asm 00009D21: MIHEL2
    // asm 00009D21: 	LDI	@MOVEIN_OFFSET,R0
    // asm 00009D22: 	ADDI	6,R0
    // asm 00009D23: 	STI	R0,@MOVEIN_OFFSET
    // asm 00009D24: 	SLEEP	1
    // asm 00009D26: 	DBU	AR5,MIHEL2
    // asm 00009D27: 	LDI	150,R0
    // asm 00009D28: 	STI	R0,@MOVEIN_OFFSET
    // asm 00009D29: 	DIE
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
    // asm 00009D2A: 	CALL	TACHOMETER_ANIMATE
    // asm 00009D2B: 	CALL	RADAR_PLOT
    // 	;
    // 	;MOTION SAFETY ON
    // 	;
    // asm 00009D2C: 	READAUD	ADJ_MOTION_PRESENT
    // asm 00009D2E: 	CMPI	0,R0
    // asm 00009D2F: 	BEQ	NOMOTIONMSGS
    // asm 00009D30: 	LDI	@MOTION_STOP_HIT,R0
    // asm 00009D31: 	BNZ	NOMOTIONMSGS
    // asm 00009D32: 	LDI	@MOTION_SAFETY_ON,R0
    // asm 00009D33: 	BZ	BLAHBLAH
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HUD", 0, 0);
    UNIMPL();
}

void ISMPH(void)
{
    // asm 00009DDD: LDI	@_MPH,R2
ISKPH:
    // asm 00009DDE: LDI	@MPH_BUFFERI,AR2
    // asm 00009DDF: 	CALL	_itoa
    // asm 00009DE0: 	FLOAT	66,R2
    // asm 00009DE1: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm 00009DE2: 	SUBF	R0,R2
    // asm 00009DE3: 	FLOAT	346,R3
    // asm 00009DE4: 	LDI	1,RC
    // asm 00009DE5: 	CALL	TEXT_ADDDS
    // asm 00009DE6: 	CALL	SETLGDIGITFONTDS
    // asm 00009DE7: 	ORM	TXT_RIGHT,*+AR0(TEXT_COLOR)
    // asm 00009DEA: 	ORM	TXT_RIGHT,*+AR1(TEXT_COLOR)
    // asm 00009DED: 	LDL	dnums_amber,AR2
    // asm 00009DEE: 	CALL	PAL_FIND_RAW
    // asm 00009DEF: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00009DF0: 	STI	R0,*+AR1(TEXT_PAL)
    // 	;-------STOPWATCH (actual elapsed time)
    // 	;
    // 	;
    // asm 00009DF1: 	LDI	8,R0
    // asm 00009DF2: 	LDI	19,R1
    // asm 00009DF3: 	SUBI	@MOVEIN_OFFSET,R1
    // asm 00009DF4: 	LDI	106,R2
    // asm 00009DF5: 	LDI	30,R3
    // asm 00009DF6: 	CALL	FILL_DITHER
    // asm 00009DF7: 	LDL	elap,AR2
    // asm 00009DF8: 	LDI	10,R2			;R2	POS X
    // asm 00009DF9: 	LDI	20,R3			;R3	POS Y
    // asm 00009DFA: 	SUBI	@MOVEIN_OFFSET,R3
    // asm 00009DFB: 	LDI	TM|ZS,R4
    // asm 00009DFC: 	CALL	BLTMOD2D_DS
    // asm 00009DFD: 	LDI	@STOPWATCH,R0
    // asm 00009DFE: 	LDI	@STOPBUFFI,AR2
    // asm 00009DFF: 	CALL	TIME2STR
    // asm 00009E00: 	FLOAT	20,R2
    // asm 00009E01: 	FLOAT	33,R3
    // asm 00009E02: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm 00009E03: 	SUBF	R0,R3
    // asm 00009E04: 	LDI	1,RC
    // asm 00009E05: 	CALL	TEXT_ADDDS
    // asm 00009E06: 	CALL	SETSMDIGITFONTDS
    // 	;-------POSITION
    // 	;(PLAYERS RANK)
    // 	;
    // asm 00009E07: 	LDI	@POSITION,AR4
    // asm 00009E08: 	SUBI	1,AR4
    // asm 00009E09: 	MPYI	3,AR4
    // asm 00009E0A: 	ADDI	@POS_TABLEI,AR4
    // asm 00009E0B: 	LDI	*AR4++,AR2
    // asm 00009E0C: 	LDI	415,R2
    // asm 00009E0D: 	ADDI	*+AR4,R2
    // asm 00009E0E: 	LDI	20,R3
    // asm 00009E0F: 	SUBI	@MOVEIN_OFFSET,R3
    // asm 00009E10: 	LDI	TM|ZS,R4
    // asm 00009E11: 	CALL	BLTMOD2D_DS
    // asm 00009E12: 	LDI	*AR4++,AR2
    // asm 00009E13: 	LDI	450,R2
    // asm 00009E14: 	LDI	20,R3
    // asm 00009E15: 	SUBI	@MOVEIN_OFFSET,R3
    // asm 00009E16: 	LDI	TM|ZS,R4
    // asm 00009E17: 	CALL	BLTMOD2D_DS
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
void HARDalloc_section(void)
{
    // asm 00009E21: 	LDI	*AR2++,AR6
    // asm 00009E22: 	LDI	*AR2,AR5
    // asm 00009E23: 	DEC	AR5
    // asm 00009E24: alloc_LPA
    // asm 00009E24: 	LDI	AR6,AR2
    // asm 00009E25: 	CALL	PAL_ALLOC
    // asm 00009E26: 	INC	AR6
    // asm 00009E27: 	DBU	AR5,alloc_LPA
    // asm 00009E28: 	RETS
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
    // asm 00009E29: 	PUSH	AR4
    // asm 00009E2A: 	PUSH	AR5
    // asm 00009E2B: 	PUSH	AR6
    // asm 00009E2C: 	LDI	*AR2++,AR6
    // asm 00009E2D: 	LDI	*AR2,AR5
    // asm 00009E2E: 	DEC	AR5
    // asm 00009E2F: deal_LP
    // asm 00009E2F: 	LDI	AR6,AR2
    // asm 00009E30: 	CALL	PAL_DELETE
    // asm 00009E31: 	INC	AR6
    // asm 00009E32: 	DBU	AR5,deal_LP
    // asm 00009E33: 	POP	AR6
    // asm 00009E34: 	POP	AR5
    // asm 00009E35: 	POP	AR4
    // asm 00009E36: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "dealloc_section", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TACHOMETER_ANIMATE(void)
{
    // asm 00009E4D: 	LDL	TACH_GRADIENT,AR0
    // asm 00009E4E: 	LDL	TACHOMETER_PAL,AR1
    // asm 00009E4F: 	LDI	22-1,RC
    // asm 00009E50: 	RPTB	LP89
    // asm 00009E51: 	LDI	*AR0++,R0
LP89:
    // asm 00009E52: STI	R0,*AR1++
    // asm 00009E53: 	LDI	@PLYCBLK,AR0
    // asm 00009E54: 	LDF	*+AR0(CARRPM),R0
    // asm 00009E55: 	MPYF	0.4583,R0		;22/48
    // ;	MPYF	0.729,R0		;35/48
    // ;	MPYF	0.6041,R0		;29/48
    // asm 00009E56: 	FIX	R0
    // asm 00009E57: 	CMPI	22,R0
    // asm 00009E58: 	LDIGT	22,R0
    // asm 00009E59: 	CMPI	0,R0
    // asm 00009E5A: 	LDILT	0,R0
    // asm 00009E5B: 	LDI	R0,RC
    // asm 00009E5C: 	SUBRI	22,RC
    // asm 00009E5D: 	LDL	TACHOMETER_PAL,AR1
    // asm 00009E5E: 	ADDI	R0,AR1
    // asm 00009E5F: 	CLRI	R1
    // asm 00009E60: 	RPTS	RC
    // asm 00009E61: 	STI	R1,*AR1++
    // asm 00009E62: 	LDI	rpm_p,AR2
    // asm 00009E63: 	CALL	PAL_FIND
    // asm 00009E64: 	PUSH	R0
    // asm 00009E65: 	LDI	R0,R2
    // asm 00009E66: 	ADDI	256-22,R2
    // asm 00009E67: 	LDL	TACHOMETER_PAL,AR2
    // asm 00009E68: 	LDI	22,R3
    // asm 00009E69: 	CALL	PAL_SET
    // asm 00009E6A: 	LDI	@PLYCBLK,AR0
    // asm 00009E6B: 	LDI	*+AR0(CARGEAR),R0
    // asm 00009E6C: 	CLRI	R1		;ASSUME NEUTRAL
    // asm 00009E6D: 	LDL	GEARPAL,AR2
    // asm 00009E6E: 	CMPI	1,R0
    // asm 00009E6F: 	LDIEQ	4,R1
    // asm 00009E70: 	CMPI	2,R0
    // asm 00009E71: 	LDIEQ	3,R1
    // asm 00009E72: 	CMPI	3,R0
    // asm 00009E73: 	LDIEQ	2,R1
    // asm 00009E74: 	CMPI	4,R0
    // asm 00009E75: 	LDIEQ	1,R1
    // asm 00009E76: 	POP	R2
    // asm 00009E77: 	ADDI	1,R2
    // asm 00009E78: 	ADDI	R1,AR2
    // asm 00009E79: 	LDI	4,R3
    // asm 00009E7A: 	CALL	PAL_SET
    // asm 00009E7B: 	RETS
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
    // asm 00009E84: 	PUSH	R0
    // asm 00009E85: 	PUSH	R1
    // asm 00009E86: 	PUSH	R2
    // asm 00009E87: 	PUSH	R3
    // asm 00009E88: 	PUSH	R4
    // 	;background
    // asm 00009E89: 	LDI	CC|DITHER|8,R4
    // asm 00009E8A: 	BD	ENTER2
    // asm 00009E8B: 	STI	R4,@_ACNTL
    // asm 00009E8C: 	CLRI	R4
    // asm 00009E8D: 	STI	R4,@_ACMAP
    // 	;---->	BD	ENTER2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FILL_DITHER", 0, 0);
    UNIMPL();
}

void FILL_PLOT(void)
{
    // asm 00009E8E: 	PUSH	R0
    // asm 00009E8F: 	PUSH	R1
    // asm 00009E90: 	PUSH	R2
    // asm 00009E91: 	PUSH	R3
    // asm 00009E92: 	PUSH	R4
    // ;	;background
    // ;	LDI	CC|DITHER|8,R4
    // ;	STI	R4,@_ACNTL
ENTER2:
    // asm 00009E93: 	STI	R0,@(_ARPS+(0*3))
    // asm 00009E94: 	STI	R0,@(_ARPS+(3*3))
    // asm 00009E95: 	ADDI	R2,R0
    // asm 00009E96: 	STI	R0,@(_ARPS+(1*3))
    // asm 00009E97: 	STI	R0,@(_ARPS+(2*3))
    // asm 00009E98: 	STI	R1,@(_ARPS+(0*3)+1)
    // asm 00009E99: 	STI	R1,@(_ARPS+(1*3)+1)
    // asm 00009E9A: 	ADDI	R3,R1,R0
    // asm 00009E9B: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm 00009E9C: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm 00009E9D: 	CLRI	R0
    // asm 00009E9E: 	STI	R0,@_ADDRL
    // asm 00009E9F: 	CALL	_stuff_fpga
    // asm 00009EA0: 	POP	R4
    // asm 00009EA1: 	POP	R3
    // asm 00009EA2: 	POP	R2
    // asm 00009EA3: 	POP	R1
    // asm 00009EA4: 	POP	R0
    // asm 00009EA5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FILL_PLOT", 0, 0);
    UNIMPL();
}

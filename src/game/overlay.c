#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "macs.h"
#include "mproc.h"
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
#include "dirq.h"
#include "port.h"
#include "overlay.h"

/*
 * Source module: asm/OVERLAY.ASM
 */

void SECTION_ROUTINE(void);
void OVERLOCK(void);
void CHECKPOINT_HIT_R(void);
void CHECKPOINT_HIT(void);
void WATERON(void);
void WATEROFF(void);
void HELISTART(void);
void HELIEND(void);
void RAILPRIME(void);
void RR_UNFREEZE(void);
void BRIDGE_OFF(void);
void BRIDGE_ON(void);
void BOFFNC(void);
void TUNNEL_ON(void);
void TUNNEL_OFF(void);
void START_SANFRAN(void);
void BEACHON(void);
void REDWOOD_START(void);
void LOOK_SANFRANCISCO(void);
void LOOK_REDWOOD(void);
void LOOK_MIDWEST(void);
void LOOK_MTRUSHMORE(void);
void LOOK_DEATHVALLEY(void);
void LOOK_GCANYON(void);
void LOOK_HOLLYWOOD(void);
void CHANGE_TUNE(void);
void SET_BGNDCOL2BROWN(void);
void TURNOFF_INFINITY(void);
void TURNON_INFINITY(void);
void TOWER_PAL_LD(void);
void TOWER_PAL_RESTORE(void);
void END_OF_GAME(void);

#define ROUTINE_TABLEI ROUTINE_TABLE

extern uintptr_t ROUTINE_TABLE[];
extern int CHECKPOINT_TIME_BONUS;
extern int REAL_CHECKPOINTS;

/*
*----------------------------------------------------------------------------
*
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

/*
*----------------------------------------------------------------------------
*When a section is loaded (from the tyco track)and the SC_ROUTINE field
*is not zero, the entry is used as an index, and executed from this table.
*
*Note that the call is made not when loading, but when the point has been
*hit.
*
*PARAMETERS
*	AR0	ROUTINE INDEX
*
*CLOBBERS	AR0
*
*/
void SECTION_ROUTINE(void)
{
    // asm 0000ACFA: 	CMPI	0,AR0
    // asm 0000ACFB: 	RETSEQ
    // asm 0000ACFC: 	ADDI	@ROUTINE_TABLEI,AR0
    // asm 0000ACFD: 	LDI	*AR0,AR0
    // asm 0000ACFE: 	CALLU	AR0
    // asm 0000ACFF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SECTION_ROUTINE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: ROUTINE_TABLE:					;routine */
/* asm: 	.word	0				;0 none leave blank */
/* asm: 	.word	CHECKPOINT_HIT			;1 */
/* asm: 	.word	WATERON				;2 */
/* asm: 	.word	WATEROFF			;3 */
/* asm: 	.word	SET_BGNDCOL2BROWN		;4 */
/* asm: 	.word	TURNOFF_INFINITY		;5 */
/* asm: 	.word	TURNON_INFINITY			;6 */
/* asm: 	.word	RR_UNFREEZE			;7 */
/* asm: 	.word	RAILPRIME			;8 */
/* asm: 	.word	HELISTART			;9 */
/* asm: 	.word	HELIEND				;10 */
/* asm: 	.word	BOFFNC				;11 */
/* asm: 	.word	TOWER_PAL_LD			;12 */
/* asm: 	.word	TOWER_PAL_RESTORE		;13 */
/* asm: 	.word	TUNNEL_ON			;14 */
/* asm: 	.word	TUNNEL_OFF			;15 */
/* asm: 	.word	BRIDGE_ON			;16 */
/* asm: 	.word	BRIDGE_OFF			;17 */
/* asm: 	.word	CHANGE_TUNE			;18 */
/* asm: 	.word	END_OF_GAME			;19 */
/* asm: 	.word	LOOK_HOLLYWOOD			;20 */
/* asm: 	.word	LOOK_GCANYON			;21 */
/* asm: 	.word	LOOK_DEATHVALLEY		;22 */
/* asm: 	.word	LOOK_MTRUSHMORE			;23 */
/* asm: 	.word	LOOK_MIDWEST			;24 */
/* asm: 	.word	LOOK_REDWOOD			;25 */
/* asm: 	.word	LOOK_SANFRANCISCO		;26 */
/* asm: 	.word	START_SANFRAN			;27 */
/* asm: 	.word	REDWOOD_START			;28 */
/* asm: 	.word	BEACHON				;29 */
/* asm: 	.word	RAMPDOWNTRAINSND		;30 */
/* asm: 	.word	OVERLOCK				;31*** */
/* asm: 	.word	BONUS1				;32 */
/* asm: 	.word	BONUS2				;33 */
/* asm: 	.word	BONUS3				;34 */
/* asm: 	.word	BONUS4				;35 */
/* asm: 	.word	BONUS5				;36 */
/* asm: 	.word	BONUS6				;37 */
/* asm: 	.word	BONUS7				;38 */
/* asm: 	.word	BONUS8				;39 */
/* asm: 	.word	BONUS9				;40 */
/* asm: 	.word	BONUS10				;41 */
/* asm: 	.word	BONUS11				;42 */
/* asm: 	.word	BONUS12				;43 */
/* asm: 	.word	BONUS13				;44 */
/* asm: 	.word	BONUS14				;45 */
uintptr_t ROUTINE_TABLE[] = {
    0, // 0 none leave blank
    (uintptr_t)CHECKPOINT_HIT, // 1
    (uintptr_t)WATERON, // 2
    (uintptr_t)WATEROFF, // 3
    (uintptr_t)SET_BGNDCOL2BROWN, // 4
    (uintptr_t)TURNOFF_INFINITY, // 5
    (uintptr_t)TURNON_INFINITY, // 6
    (uintptr_t)RR_UNFREEZE, // 7
    (uintptr_t)RAILPRIME, // 8
    (uintptr_t)HELISTART, // 9
    (uintptr_t)HELIEND, // 10
    (uintptr_t)BOFFNC, // 11
    (uintptr_t)TOWER_PAL_LD, // 12
    (uintptr_t)TOWER_PAL_RESTORE, // 13
    (uintptr_t)TUNNEL_ON, // 14
    (uintptr_t)TUNNEL_OFF, // 15
    (uintptr_t)BRIDGE_ON, // 16
    (uintptr_t)BRIDGE_OFF, // 17
    (uintptr_t)CHANGE_TUNE, // 18
    (uintptr_t)END_OF_GAME, // 19
    (uintptr_t)LOOK_HOLLYWOOD, // 20
    (uintptr_t)LOOK_GCANYON, // 21
    (uintptr_t)LOOK_DEATHVALLEY, // 22
    (uintptr_t)LOOK_MTRUSHMORE, // 23
    (uintptr_t)LOOK_MIDWEST, // 24
    (uintptr_t)LOOK_REDWOOD, // 25
    (uintptr_t)LOOK_SANFRANCISCO, // 26
    (uintptr_t)START_SANFRAN, // 27
    (uintptr_t)REDWOOD_START, // 28
    (uintptr_t)BEACHON, // 29
    (uintptr_t)RAMPDOWNTRAINSND, // 30
    (uintptr_t)OVERLOCK, // 31***
    (uintptr_t)BONUS1, // 32
    (uintptr_t)BONUS2, // 33
    (uintptr_t)BONUS3, // 34
    (uintptr_t)BONUS4, // 35
    (uintptr_t)BONUS5, // 36
    (uintptr_t)BONUS6, // 37
    (uintptr_t)BONUS7, // 38
    (uintptr_t)BONUS8, // 39
    (uintptr_t)BONUS9, // 40
    (uintptr_t)BONUS10, // 41
    (uintptr_t)BONUS11, // 42
    (uintptr_t)BONUS12, // 43
    (uintptr_t)BONUS13, // 44
    (uintptr_t)BONUS14, // 45
};
#if DEBUG
#endif

void OVERLOCK(void)
{
#if DEBUG
    // asm: 	BU	$
#endif
    // asm 0000AD2F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OVERLOCK", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CHECKPOINT_TIME_BONUS	.bss	CHECKPOINT_TIME_BONUS,1 */
int CHECKPOINT_TIME_BONUS;
/* asm: REAL_CHECKPOINTS	.bss	REAL_CHECKPOINTS,1 */
int REAL_CHECKPOINTS;

void CHECKPOINT_HIT_R(void)
{
    // asm 0000AD30: 	PUSH	R0
    // asm 0000AD31: 	PUSH	AR2
    // asm 0000AD32: 	BU	JJGH
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKPOINT_HIT_R", 0, 0);
    UNIMPL();
}

void CHECKPOINT_HIT(void)
{
    // asm 0000AD33: 	PUSH	R0
    // asm 0000AD34: 	PUSH	AR2
    // asm 0000AD35: 	INCM	@REAL_CHECKPOINTS
    // asm 0000AD38: 	CMPI	@CHECKPOINT_NUM,R0
    // asm 0000AD39: 	BLE	CPHX
JJGH:
    // asm 0000AD3A: 	CALL	SEND_CHECKPOINT		;not really send, but setup variables
    // asm 0000AD3B: 	INCM	@CHECKPOINT_NUM
    // asm 0000AD3E: 	LDI	@_countdown,R0
    // asm 0000AD3F: 	ADDI	@CHECKPOINT_TIME_BONUS,R0
    // asm 0000AD40: 	STI	R0,@_countdown
    // asm 0000AD41: 	LDI	@_MODE,R0
    // asm 0000AD42: 	AND	MMODE,R0
    // asm 0000AD43: 	CMPI	MGAME,R0
    // asm 0000AD44: 	BNE	CPHX
    // asm 0000AD45: 	SONDFX	CHECKPT
CPHX:
    // asm 0000AD47: 	POP	AR2
    // asm 0000AD48: 	POP	R0
    // asm 0000AD49: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKPOINT_HIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void WATERON(void)
{
    // asm 0000AD4A: 	LDI	@_MODE,R0
    // asm 0000AD4B: 	OR	MWATER,R0
    // asm 0000AD4C: 	STI	R0,@_MODE
    // asm 0000AD4D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERON", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void WATEROFF(void)
{
    // asm 0000AD4E: 	LDI	@_MODE,R0
    // asm 0000AD4F: 	ANDN	MWATER,R0
    // asm 0000AD50: 	STI	R0,@_MODE
    // asm 0000AD51: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATEROFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void HELISTART(void)
{
    // asm 0000AD52: 	CREATE	CHOPPER,DRONE_C|VEHICLE_T|DRNE_SIGMA
    // asm 0000AD55: 	CLRI	R0
    // asm 0000AD56: 	STI	R0,@HELI_ABORT
    // asm 0000AD57: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HELISTART", 0, 0);
    UNIMPL();
}

void HELIEND(void)
{
    // asm 0000AD58: 	LDI	1,R0
    // asm 0000AD59: 	STI	R0,@HELI_ABORT
    // asm 0000AD5A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HELIEND", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void RAILPRIME(void)
{
    // asm 0000AD5B: 	CLRI	R0
    // asm 0000AD5C: 	STI	R0,@DD_MAX_DRONES
    // asm 0000AD5D: 	STI	R0,@FREEZE_IT
    // asm 0000AD5E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAILPRIME", 0, 0);
    UNIMPL();
}

void RR_UNFREEZE(void)
{
    // asm 0000AD5F: 	LDI	1,R0
    // asm 0000AD60: 	STI	R0,@FREEZE_IT
    // asm 0000AD61: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RR_UNFREEZE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void BRIDGE_OFF(void)
{
    // asm 0000AD62: 	LDI	@_MODE,R0
    // asm 0000AD63: 	ANDN	MBRIDGE,R0
    // asm 0000AD64: 	STI	R0,@_MODE
    // asm 0000AD65: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BRIDGE_OFF", 0, 0);
    UNIMPL();
}

void BRIDGE_ON(void)
{
    // asm 0000AD66: 	LDI	@_MODE,R0
    // asm 0000AD67: 	OR	MBRIDGE,R0
    // asm 0000AD68: 	STI	R0,@_MODE
    // asm 0000AD69: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BRIDGE_ON", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void BOFFNC(void)
{
    // asm 0000AD6A: 	CALL	BRIDGE_OFF
    // asm 0000AD6B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOFFNC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void TUNNEL_ON(void)
{
    // asm 0000AD6C: 	LDI	@_MODE,R0
    // asm 0000AD6D: 	OR	MINTUNNEL,R0
    // asm 0000AD6E: 	STI	R0,@_MODE
    // asm 0000AD6F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TUNNEL_ON", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void TUNNEL_OFF(void)
{
    // asm 0000AD70: 	LDI	@_MODE,R0
    // asm 0000AD71: 	ANDN	MINTUNNEL,R0
    // asm 0000AD72: 	STI	R0,@_MODE
    // asm 0000AD73: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TUNNEL_OFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void START_SANFRAN(void)
{
    // asm 0000AD74: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "START_SANFRAN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void BEACHON(void)
{
    // asm 0000AD75: 	LDI	@_MODE,R0
    // asm 0000AD76: 	OR	MWATER,R0
    // asm 0000AD77: 	STI	R0,@_MODE
    // asm 0000AD78: 	LDI	MAX_DRONES,R0
    // asm 0000AD79: 	STI	R0,@DD_MAX_DRONES
    // asm 0000AD7A: 	LDI	60,R0
    // asm 0000AD7B: 	STI	R0,@DD_SLP
    // asm 0000AD7C: 	LDI	100,R0
    // asm 0000AD7D: 	STI	R0,@DD_VAR
    // asm 0000AD7E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BEACHON", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void REDWOOD_START(void)
{
    // asm 0000AD7F: 	LDF	10,R0
    // asm 0000AD80: 	STF	R0,@VAR_ROAD_KFACTOR			;pixels UNDER (overshoot)
    // asm 0000AD81: 	FLOAT	75,R0	;75
    // asm 0000AD82: 	STF	R0,@INFIN_CORRECT
    // asm 0000AD83: 	LDI	4,R0
    // asm 0000AD84: 	STI	R0,@DD_MAX_DRONES
    // asm 0000AD85: 	LDI	120,R0
    // asm 0000AD86: 	STI	R0,@DD_SLP
    // asm 0000AD87: 	LDI	120,R0
    // asm 0000AD88: 	STI	R0,@DD_VAR
    // asm 0000AD89: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REDWOOD_START", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void LOOK_SANFRANCISCO(void)
{
    // asm 0000AD8A: 	SONDFX	ER_SANFRANCISCO
    // asm 0000AD8C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_SANFRANCISCO", 0, 0);
    UNIMPL();
}

void LOOK_REDWOOD(void)
{
    // asm 0000AD8D: 	SONDFX	ER_REDWOOD
    // asm 0000AD8F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_REDWOOD", 0, 0);
    UNIMPL();
}

void LOOK_MIDWEST(void)
{
    // asm 0000AD90: 	SONDFX	ER_MIDWEST
    // asm 0000AD92: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_MIDWEST", 0, 0);
    UNIMPL();
}

void LOOK_MTRUSHMORE(void)
{
    // asm 0000AD93: 	SONDFX	ER_MTRUSHMORE
    // asm 0000AD95: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_MTRUSHMORE", 0, 0);
    UNIMPL();
}

void LOOK_DEATHVALLEY(void)
{
    // asm 0000AD96: 	SONDFX	ER_DEATHVALLEY
    // asm 0000AD98: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_DEATHVALLEY", 0, 0);
    UNIMPL();
}

void LOOK_GCANYON(void)
{
    // asm 0000AD99: 	SONDFX	ER_GRANDCANYON
    // asm 0000AD9B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_GCANYON", 0, 0);
    UNIMPL();
}

void LOOK_HOLLYWOOD(void)
{
    // asm 0000AD9C: 	SONDFX	ER_HOLLYWOOD
    // asm 0000AD9E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_HOLLYWOOD", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void CHANGE_TUNE(void)
{
    // asm 0000AD9F: 	CALL	CHANGE_STATION
    // asm 0000ADA0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHANGE_TUNE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SET_BGNDCOL2BROWN(void)
{
    // asm 0000ADA1: 	CALL	CHECKPOINT_HIT
    // asm 0000ADA2: 	LDIL	0984900h,R0		;want blue sky
    // asm 0000ADA5: 	STI	R0,@BGNDCOLA
    // asm 0000ADA6: 	LDI	1,R0
    // asm 0000ADA7: 	STI	R0,@NOAERASE
    // asm 0000ADA8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_BGNDCOL2BROWN", 0, 0);
    UNIMPL();
}

void TURNOFF_INFINITY(void)
{
    // asm 0000ADA9: 	LDI	@_MODE,R0
    // asm 0000ADAA: 	ANDN	MINFIN,R0
    // asm 0000ADAB: 	STI	R0,@_MODE
    // asm 0000ADAC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TURNOFF_INFINITY", 0, 0);
    UNIMPL();
}

void TURNON_INFINITY(void)
{
    // asm 0000ADAD: 	LDI	@_MODE,R0
    // asm 0000ADAE: 	OR	MINFIN,R0
    // asm 0000ADAF: 	STI	R0,@_MODE
    // asm 0000ADB0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TURNON_INFINITY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void TOWER_PAL_LD(void)
{
    // 	;find the pal which tower is in
    // 	;do a palset with the new palette
    // asm 0000ADB1: 	LDI	tower_p,AR2
    // asm 0000ADB2: 	CALL	PAL_FIND
    // asm 0000ADB3: 	LDL	tower_bgrey,AR2
    // asm 0000ADB4: 	LDI	R0,R2
    // asm 0000ADB5: 	LDI	*AR2++,R3		;GET COUNT
    // asm 0000ADB6: 	CALL	PAL_SET
    // asm 0000ADB7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOWER_PAL_LD", 0, 0);
    UNIMPL();
}

void TOWER_PAL_RESTORE(void)
{
    // asm 0000ADB8: 	PUSH	AR2
    // asm 0000ADB9: 	PUSH	R2
    // 	;not so much a restore as a validate it is set
    // 	;
    // asm 0000ADBA: 	LDI	tower_p,R0		;ONE TO OVERWRITE
    // asm 0000ADBB: 	LDI	R0,R1			;WHAT TO OVERWRITE IT WITH
    // asm 0000ADBC: 	CALL	PAL_OVERWRITE
    // asm 0000ADBD: 	POP	R2
    // asm 0000ADBE: 	POP	AR2
    // asm 0000ADBF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOWER_PAL_RESTORE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void END_OF_GAME(void)
{
    // asm 0000ADC0: 	LDI	1,R0
    // asm 0000ADC1: 	STI	R0,@END_OF_GAMEP
    // asm 0000ADC2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "END_OF_GAME", 0, 0);
    UNIMPL();
}

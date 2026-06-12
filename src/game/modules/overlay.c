#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/macs.h"
#include "../include/mproc.h"
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
#include "../include/dirq.h"
#include "../include/overlay.h"
#include "../include/discovered_defines.h"

/*
 * Source module: asm/OVERLAY.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *----------------------------------------------------------------------------
// *When a section is loaded (from the tyco track)and the SC_ROUTINE field
// *is not zero, the entry is used as an index, and executed from this table.
// *
// *Note that the call is made not when loading, but when the point has been
// *hit.
// *
// *PARAMETERS
// *	AR0	ROUTINE INDEX
// *
// *CLOBBERS	AR0
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: ROUTINE_TABLE	ROUTINE_TABLE:					;routine */
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
/* asm: 	.word	OL				;31*** */
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
int ROUTINE_TABLE[] = {
    0, // 0 none leave blank
    CHECKPOINT_HIT, // 1
    WATERON, // 2
    WATEROFF, // 3
    SET_BGNDCOL2BROWN, // 4
    TURNOFF_INFINITY, // 5
    TURNON_INFINITY, // 6
    RR_UNFREEZE, // 7
    RAILPRIME, // 8
    HELISTART, // 9
    HELIEND, // 10
    BOFFNC, // 11
    TOWER_PAL_LD, // 12
    TOWER_PAL_RESTORE, // 13
    TUNNEL_ON, // 14
    TUNNEL_OFF, // 15
    BRIDGE_ON, // 16
    BRIDGE_OFF, // 17
    CHANGE_TUNE, // 18
    END_OF_GAME, // 19
    LOOK_HOLLYWOOD, // 20
    LOOK_GCANYON, // 21
    LOOK_DEATHVALLEY, // 22
    LOOK_MTRUSHMORE, // 23
    LOOK_MIDWEST, // 24
    LOOK_REDWOOD, // 25
    LOOK_SANFRANCISCO, // 26
    START_SANFRAN, // 27
    REDWOOD_START, // 28
    BEACHON, // 29
    RAMPDOWNTRAINSND, // 30
    OL, // 31***
    BONUS1, // 32
    BONUS2, // 33
    BONUS3, // 34
    BONUS4, // 35
    BONUS5, // 36
    BONUS6, // 37
    BONUS7, // 38
    BONUS8, // 39
    BONUS9, // 40
    BONUS10, // 41
    BONUS11, // 42
    BONUS12, // 43
    BONUS13, // 44
    BONUS14, // 45
};
#if DEBUG
#endif
#endif
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: CHECKPOINT_TIME_BONUS	.bss	CHECKPOINT_TIME_BONUS,1 */
int CHECKPOINT_TIME_BONUS;
/* asm: REAL_CHECKPOINTS	.bss	REAL_CHECKPOINTS,1 */
int REAL_CHECKPOINTS;
// *----------------------------------------------------------------------------

void SECTION_ROUTINE(void)
{
    // asm: 	CMPI	0,AR0
    // asm: 	RETSEQ
    // asm: 	ADDI	@ROUTINE_TABLEI,AR0
    // asm: 	LDI	*AR0,AR0
    // asm: 	CALLU	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SECTION_ROUTINE", 0, 0);
    UNIMPL();
}

void CHECKPOINT_HIT_R(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR2
    // asm: 	BU	JJGH
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKPOINT_HIT_R", 0, 0);
    UNIMPL();
}

void CHECKPOINT_HIT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR2
    // asm: 	INCM	@REAL_CHECKPOINTS
    // asm: 	CMPI	@CHECKPOINT_NUM,R0
    // asm: 	BLE	CPHX
JJGH:
    // asm: 	CALL	SEND_CHECKPOINT		;not really send, but setup variables
    // asm: 	INCM	@CHECKPOINT_NUM
    // asm: 	LDI	@_countdown,R0
    // asm: 	ADDI	@CHECKPOINT_TIME_BONUS,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	CPHX
    // asm: 	SONDFX	CHECKPT
CPHX:
    // asm: 	POP	AR2
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKPOINT_HIT", 0, 0);
    UNIMPL();
}

void WATERON(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERON", 0, 0);
    UNIMPL();
}

void WATEROFF(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATEROFF", 0, 0);
    UNIMPL();
}

void HELISTART(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CREATE	CHOPPER,DRONE_C|VEHICLE_T|DRNE_SIGMA
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@HELI_ABORT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HELISTART", 0, 0);
    UNIMPL();
}

void HELIEND(void)
{
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HELI_ABORT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HELIEND", 0, 0);
    UNIMPL();
}

void RAILPRIME(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@DD_MAX_DRONES
    // asm: 	STI	R0,@FREEZE_IT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAILPRIME", 0, 0);
    UNIMPL();
}

void RR_UNFREEZE(void)
{
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@FREEZE_IT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RR_UNFREEZE", 0, 0);
    UNIMPL();
}

void BRIDGE_OFF(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MBRIDGE,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BRIDGE_OFF", 0, 0);
    UNIMPL();
}

void BRIDGE_ON(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MBRIDGE,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BRIDGE_ON", 0, 0);
    UNIMPL();
}

void BOFFNC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	BRIDGE_OFF
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOFFNC", 0, 0);
    UNIMPL();
}

void TUNNEL_ON(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MINTUNNEL,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TUNNEL_ON", 0, 0);
    UNIMPL();
}

void TUNNEL_OFF(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MINTUNNEL,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TUNNEL_OFF", 0, 0);
    UNIMPL();
}

void START_SANFRAN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "START_SANFRAN", 0, 0);
    UNIMPL();
}

void BEACHON(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	MAX_DRONES,R0
    // asm: 	STI	R0,@DD_MAX_DRONES
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BEACHON", 0, 0);
    UNIMPL();
}

void REDWOOD_START(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDF	10,R0
    // asm: 	STF	R0,@VAR_ROAD_KFACTOR			;pixels UNDER (overshoot)
    // asm: 	FLOAT	75,R0	;75
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDI	4,R0
    // asm: 	STI	R0,@DD_MAX_DRONES
    // asm: 	LDI	120,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	120,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REDWOOD_START", 0, 0);
    UNIMPL();
}

void LOOK_SANFRANCISCO(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	SONDFX	ER_SANFRANCISCO
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_SANFRANCISCO", 0, 0);
    UNIMPL();
}

void LOOK_REDWOOD(void)
{
    // asm: 	SONDFX	ER_REDWOOD
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_REDWOOD", 0, 0);
    UNIMPL();
}

void LOOK_MIDWEST(void)
{
    // asm: 	SONDFX	ER_MIDWEST
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_MIDWEST", 0, 0);
    UNIMPL();
}

void LOOK_MTRUSHMORE(void)
{
    // asm: 	SONDFX	ER_MTRUSHMORE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_MTRUSHMORE", 0, 0);
    UNIMPL();
}

void LOOK_DEATHVALLEY(void)
{
    // asm: 	SONDFX	ER_DEATHVALLEY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_DEATHVALLEY", 0, 0);
    UNIMPL();
}

void LOOK_GCANYON(void)
{
    // asm: 	SONDFX	ER_GRANDCANYON
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_GCANYON", 0, 0);
    UNIMPL();
}

void LOOK_HOLLYWOOD(void)
{
    // asm: 	SONDFX	ER_HOLLYWOOD
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOOK_HOLLYWOOD", 0, 0);
    UNIMPL();
}

void CHANGE_TUNE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	CHANGE_STATION
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHANGE_TUNE", 0, 0);
    UNIMPL();
}

void SET_BGNDCOL2BROWN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	CHECKPOINT_HIT
    // asm: 	LDIL	0984900h,R0		;want blue sky
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_BGNDCOL2BROWN", 0, 0);
    UNIMPL();
}

void TURNOFF_INFINITY(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MINFIN,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TURNOFF_INFINITY", 0, 0);
    UNIMPL();
}

void TURNON_INFINITY(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MINFIN,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TURNON_INFINITY", 0, 0);
    UNIMPL();
}

void TOWER_PAL_LD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // 	;find the pal which tower is in
    // 	;do a palset with the new palette
    // asm: 	LDI	tower_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	LDL	tower_bgrey,AR2
    // asm: 	LDI	R0,R2
    // asm: 	LDI	*AR2++,R3		;GET COUNT
    // asm: 	CALL	PAL_SET
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOWER_PAL_LD", 0, 0);
    UNIMPL();
}

void TOWER_PAL_RESTORE(void)
{
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R2
    // 	;not so much a restore as a validate it is set
    // 	;
    // asm: 	LDI	tower_p,R0		;ONE TO OVERWRITE
    // asm: 	LDI	R0,R1			;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	POP	R2
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOWER_PAL_RESTORE", 0, 0);
    UNIMPL();
}

void END_OF_GAME(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@END_OF_GAMEP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "END_OF_GAME", 0, 0);
    UNIMPL();
}

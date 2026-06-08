#include "../../core/cpu.h"
#include "../../core/machine.h"
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
#include "../include/cornobj.h"

/*
 * Source module: asm/SETUPS.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------

void CLEANUP_PALS(void)
{
    // asm: 	LDL	CORNPAL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	BNC	DONTDEL
    // asm: 	LDL	CORNPAL,AR2
    // asm: 	CALL	PAL_DELETE_RAW
DONTDEL:
    // asm: 	LDL	tunnel2_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	tunnel_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	thetrains_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	deserts_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	ggate_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	h280_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	bevhnred_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	lafreeway_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	deathvalley_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	arizona_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	mtrush_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	mwest_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	chicago_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	appl_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	dc_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	dc_shared_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	finale_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	theheads_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	press_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	crace_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEANUP_PALS", 0, 0);
    UNIMPL();
}

void FULLSETUP_TABLE(void)
{
    // asm: LOADSECTION_TABLE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_TABLE", 0, 0);
    UNIMPL();
}

void LOADGGPARK(void)
{
    // asm: 	LDL	_SECggate,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECcorn,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADGGPARK", 0, 0);
    UNIMPL();
}

void LOADSANFRAN(void)
{
    // asm: 	CALL	LOAD_TUNNEL2
    // asm: 	LDL	_SECggate,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADSANFRAN", 0, 0);
    UNIMPL();
}

void LOADALLH280(void)
{
    // asm: 	CALL	LOAD_TUNNEL2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADALLH280", 0, 0);
    UNIMPL();
}

void LOADH280(void)
{
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECh280,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADH280", 0, 0);
    UNIMPL();
}

void LOADREDWOOD(void)
{
    // asm: 	CALL	LOADH280
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADREDWOOD", 0, 0);
    UNIMPL();
}

void LOADBEVERLY(void)
{
    // asm: 	CALL	LOADH280
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	LDL	_SECbevhnred,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADBEVERLY", 0, 0);
    UNIMPL();
}

void LOADLAFREEWAY(void)
{
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	LDL	_SEClafreeway,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADLAFREEWAY", 0, 0);
    UNIMPL();
}

void LOADDEATHVALLEY(void)
{
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	LDL	_SECdeathvalley,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECthetrains,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADDEATHVALLEY", 0, 0);
    UNIMPL();
}

void LOADARIZONA(void)
{
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	LDL	_SECarizona,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADARIZONA", 0, 0);
    UNIMPL();
}

void LOADGCANYON(void)
{
    //  ;(MOUNT RUSHMORE)
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECmtrush,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADGCANYON", 0, 0);
    UNIMPL();
}

void LOADIOWA(void)
{
    // asm: 	LDL	_SECiowa_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECmwest,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADIOWA", 0, 0);
    UNIMPL();
}

void LOADCHICAGO(void)
{
    // asm: 	LDL	_SECthetrains,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECiowa_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECchicago,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADCHICAGO", 0, 0);
    UNIMPL();
}

void LOADINDIANA(void)
{
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECmwest,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADINDIANA", 0, 0);
    UNIMPL();
}

void LOADAPPALACHIA(void)
{
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECappl,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADAPPALACHIA", 0, 0);
    UNIMPL();
}

void LOADWASHINGTONDC(void)
{
    // asm: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECdc,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECdc_shared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADWASHINGTONDC", 0, 0);
    UNIMPL();
}

void LOAD_TUNNEL2(void)
{
    // asm: 	LDL	_SECtunnel2,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	tunnel2_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_TUNNEL2", 0, 0);
    UNIMPL();
}

void WATERON(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERON", 0, 0);
    UNIMPL();
}

void WATEROFF(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATEROFF", 0, 0);
    UNIMPL();
}

void FULLSETUP_GGPARK(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	_SECggate,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECcorn,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	LDL	ggate_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	CORNPAL,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	FLOAT	-15,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	CALL	WATERON
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	1,AR0
    // asm: 	LDI	L_LEG1_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_GGPARK", 0, 0);
    UNIMPL();
}

void FULLSETUP_SANFRAN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	_SECggate,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	LOAD_TUNNEL2
    // asm: 	LDL	ggate_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	tunnel2_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	FLOAT	-15,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	CALL	WATERON
    // asm: 	LDI	120,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	120,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	LDI	2,R0		;DISCO DUCK
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDL	shared_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	shared_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECshared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG2_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG2_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_SANFRAN", 0, 0);
    UNIMPL();
}

void FULLSETUP_H280(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	LOAD_TUNNEL2
    // asm: 	LDL	_SECh280,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	3,R0		;SPAGETTI
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	LDL	deserts_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	h280_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	FLOAT	55,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG3_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG3_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_H280", 0, 0);
    UNIMPL();
}

void FULLSETUP_REDWOOD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECh280,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	deserts_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	h280_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECbevhnred,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	bevhnred_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	CALL	WATEROFF
    // asm: 	LDI	0,R0		;MONSTER SURF
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	LDI	120,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	120,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	FLOAT	50,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // 	;load proper tunnel
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG4_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG4_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_REDWOOD", 0, 0);
    UNIMPL();
}

void LOAD_TUNNEL(void)
{
    //  ;(ACTUAL)
    // asm: 	LDL	tunnel_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECtunnel,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_TUNNEL", 0, 0);
    UNIMPL();
}

void FULLSETUP_BEVERLY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECbevhnred,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	LDL	bevhnred_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	deserts_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	CALL	WATEROFF
    // asm: 	LDI	5,R0		;STRAIGHT 2 4
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	STI	R0,@COCONUT_COUNT
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	FLOAT	45,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG5_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG5_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_BEVERLY", 0, 0);
    UNIMPL();
}

void FULLSETUP_LAFREEWAY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	LDL	_SEClafreeway,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	WATEROFF
    // asm: 	LDI	2,R0		;DISCO DUCK
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	FLOAT	35,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDL	lafreeway_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG6_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG6_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_LAFREEWAY", 0, 0);
    UNIMPL();
}

void FULLSETUP_DEATHVALLEY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	_SECdeathvalley,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECthetrains,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	CALL	WATEROFF
    // asm: 	LDI	3,R0		;SPAGETTI
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	FLOAT	55,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	bottom2_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // ;	LDL	lafreeway_PALETTES,AR2
    // ;	CALL	dealloc_section
    // asm: 	LDL	deathvalley_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	thetrains_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG7_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG7_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_DEATHVALLEY", 0, 0);
    UNIMPL();
}

void FULLSETUP_ARIZONA(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	tunnel_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	CALL	WATEROFF
    // asm: 	LDL	deserts_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	CALL	LOAD_TUNNEL
    // asm: 	LDL	arizona_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECarizona,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	1,R0		;SHUFFLE DRIVE
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	FLOAT	45,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG8_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG8_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_ARIZONA", 0, 0);
    UNIMPL();
}

void FULLSETUP_GCANYON(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECmtrush,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	WATEROFF
    // asm: 	LDL	deserts_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECdeserts,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // 	;(MOUNT RUSHMORE)
    // asm: 	LDI	3,R0		;SPAGETTI
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	FLOAT	55,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDL	mtrush_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDI	75,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	125,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG9_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG9_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_GCANYON", 0, 0);
    UNIMPL();
}

void FULLSETUP_IOWA(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	WATEROFF
    // asm: 	LDI	7,R0		;BOGGIE
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	FLOAT	45,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDL	_SECiowa_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECmwest,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	75,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	125,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	LDL	mwest_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDI	bottom_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	bottom3_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	btomwt_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG10_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG10_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	CREATE	BUG_SPAWNER_PROC,SPAWNER_C
    // asm: 	CREATE	GEESE_SPAWNER,SPAWNER_C
    // asm: 	CREATE	DEER_SPAWNER_PROC,SPAWNER_C
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_IOWA", 0, 0);
    UNIMPL();
}

void FULLSETUP_CHICAGO(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	_SECthetrains,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECchicago,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	WATEROFF
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	LDI	2,R0		;DISCO DUCK
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	FLOAT	55,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDL	thetrains_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	chicago_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG11_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG11_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_CHICAGO", 0, 0);
    UNIMPL();
}

void FULLSETUP_INDIANA(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	WATEROFF
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	btomwt_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	4,R0		;VENTURE
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	FLOAT	45,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDL	mwest_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	bottom_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	midwgras_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDL	_SECmwest,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	75,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	125,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG12_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG12_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	CREATE	BUG_SPAWNER_PROC,SPAWNER_C
    // asm: 	CREATE	GEESE_SPAWNER,SPAWNER_C
    // ;	CREATE	DEER_SPAWNER_PROC,SPAWNER_C
    // asm: 	CREATE	COW_SPAWNER_PROC,SPAWNER_C
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_INDIANA", 0, 0);
    UNIMPL();
}

void FULLSETUP_APPALACHIA(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	WATEROFF
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	btomwt_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	bottom_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	midwgras_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	5,R0		;STRAIGHT 2 4
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	FLOAT	85,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDL	appl_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECappl,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG13_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG13_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_APPALACHIA", 0, 0);
    UNIMPL();
}

void FULLSETUP_WASHINGTONDC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	WATEROFF
    // asm: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	bottom_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	midwgras_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	btomwt_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	2,R0			;DISCO DUCK
    // asm: 	STI	R0,@TUNE_IDX
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	FLOAT	45,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	LDL	dc_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	dc_shared_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	_SECdc_shared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECdc,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	LEG_INIT
    // asm: 	LDI	L_LEG14_BEGIN,AR0
    // asm: 	LS	8,AR0
    // asm: 	STI	AR0,@FINISH_LINE
    // asm: 	RS	8,AR0
    // asm: 	LDI	L_LEG14_END,AR1
    // asm: 	CALL	LEG_GENERATE_MAP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_WASHINGTONDC", 0, 0);
    UNIMPL();
}

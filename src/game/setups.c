#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "text.h"
#include "cornobj.h"

/*
 * Source module: asm/SETUPS.ASM
 */

void CLEANUP_PALS(void);
static void LOADGGPARK(void);
static void LOADSANFRAN(void);
static void LOADALLH280(void);
static void LOADH280(void);
static void LOADREDWOOD(void);
static void LOADBEVERLY(void);
static void LOADLAFREEWAY(void);
static void LOADDEATHVALLEY(void);
static void LOADARIZONA(void);
static void LOADGCANYON(void);
static void LOADIOWA(void);
static void LOADCHICAGO(void);
static void LOADINDIANA(void);
static void LOADAPPALACHIA(void);
static void LOADWASHINGTONDC(void);
static void LOAD_TUNNEL2(void);
static void WATERON(void);
static void WATEROFF(void);
static void FULLSETUP_GGPARK(void);
static void FULLSETUP_SANFRAN(void);
static void FULLSETUP_H280(void);
static void FULLSETUP_REDWOOD(void);
void LOAD_TUNNEL(void);
static void FULLSETUP_BEVERLY(void);
static void FULLSETUP_LAFREEWAY(void);
static void FULLSETUP_DEATHVALLEY(void);
static void FULLSETUP_ARIZONA(void);
static void FULLSETUP_GCANYON(void);
static void FULLSETUP_IOWA(void);
static void FULLSETUP_CHICAGO(void);
static void FULLSETUP_INDIANA(void);
static void FULLSETUP_APPALACHIA(void);
static void FULLSETUP_WASHINGTONDC(void);

#define LOADSECTION_TABLEI LOADSECTION_TABLE

static void (*FULLSETUP_TABLE[14])(void);
static void (*LOADSECTION_TABLE[14])(void);

/*
*----------------------------------------------------------------------------
*
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

// *----------------------------------------------------------------------------
void CLEANUP_PALS(void)
{
    // asm 00008C0C: 	LDL	CORNPAL,AR2
    // asm 00008C0D: 	CALL	PAL_FIND_RAW
    // asm 00008C0E: 	BNC	DONTDEL
    // asm 00008C0F: 	LDL	CORNPAL,AR2
    // asm 00008C10: 	CALL	PAL_DELETE_RAW
DONTDEL:
    // asm 00008C11: 	LDL	tunnel2_PALETTES,AR2
    // asm 00008C12: 	CALL	dealloc_section
    // asm 00008C13: 	LDL	tunnel_PALETTES,AR2
    // asm 00008C14: 	CALL	dealloc_section
    // asm 00008C15: 	LDL	thetrains_PALETTES,AR2
    // asm 00008C16: 	CALL	dealloc_section
    // asm 00008C17: 	LDL	deserts_PALETTES,AR2
    // asm 00008C18: 	CALL	dealloc_section
    // asm 00008C19: 	LDL	ggate_PALETTES,AR2
    // asm 00008C1A: 	CALL	dealloc_section
    // asm 00008C1B: 	LDL	h280_PALETTES,AR2
    // asm 00008C1C: 	CALL	dealloc_section
    // asm 00008C1D: 	LDL	bevhnred_PALETTES,AR2
    // asm 00008C1E: 	CALL	dealloc_section
    // asm 00008C1F: 	LDL	lafreeway_PALETTES,AR2
    // asm 00008C20: 	CALL	dealloc_section
    // asm 00008C21: 	LDL	deathvalley_PALETTES,AR2
    // asm 00008C22: 	CALL	dealloc_section
    // asm 00008C23: 	LDL	arizona_PALETTES,AR2
    // asm 00008C24: 	CALL	dealloc_section
    // asm 00008C25: 	LDL	mtrush_PALETTES,AR2
    // asm 00008C26: 	CALL	dealloc_section
    // asm 00008C27: 	LDL	mwest_PALETTES,AR2
    // asm 00008C28: 	CALL	dealloc_section
    // asm 00008C29: 	LDL	chicago_PALETTES,AR2
    // asm 00008C2A: 	CALL	dealloc_section
    // asm 00008C2B: 	LDL	appl_PALETTES,AR2
    // asm 00008C2C: 	CALL	dealloc_section
    // asm 00008C2D: 	LDL	dc_PALETTES,AR2
    // asm 00008C2E: 	CALL	dealloc_section
    // asm 00008C2F: 	LDL	dc_shared_PALETTES,AR2
    // asm 00008C30: 	CALL	dealloc_section
    // asm 00008C31: 	LDL	finale_PALETTES,AR2
    // asm 00008C32: 	CALL	dealloc_section
    // asm 00008C33: 	LDL	theheads_PALETTES,AR2
    // asm 00008C34: 	CALL	dealloc_section
    // asm 00008C35: 	LDL	press_PALETTES,AR2
    // asm 00008C36: 	CALL	dealloc_section
    // asm 00008C37: 	LDL	crace_PALETTES,AR2
    // asm 00008C38: 	CALL	dealloc_section
    // asm 00008C39: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEANUP_PALS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: FULLSETUP_TABLEI */
/* asm: .word	FULLSETUP_TABLE */
/* asm: romdata */
int FULLSETUP_TABLEI;
/* asm: FULLSETUP_TABLE: */
/* asm: 	.word	FULLSETUP_GGPARK */
/* asm: 	.word	FULLSETUP_SANFRAN,FULLSETUP_H280,FULLSETUP_REDWOOD */
/* asm: 	.word	FULLSETUP_BEVERLY,FULLSETUP_LAFREEWAY,FULLSETUP_DEATHVALLEY */
/* asm: 	.word	FULLSETUP_ARIZONA,FULLSETUP_GCANYON,FULLSETUP_IOWA */
/* asm: 	.word	FULLSETUP_CHICAGO,FULLSETUP_INDIANA,FULLSETUP_APPALACHIA */
/* asm: 	.word	FULLSETUP_WASHINGTONDC */
static void (*FULLSETUP_TABLE[])(void) = {
    FULLSETUP_GGPARK,
    FULLSETUP_SANFRAN, FULLSETUP_H280, FULLSETUP_REDWOOD,
    FULLSETUP_BEVERLY, FULLSETUP_LAFREEWAY, FULLSETUP_DEATHVALLEY,
    FULLSETUP_ARIZONA, FULLSETUP_GCANYON, FULLSETUP_IOWA,
    FULLSETUP_CHICAGO, FULLSETUP_INDIANA, FULLSETUP_APPALACHIA,
    FULLSETUP_WASHINGTONDC,
};
/* asm: LOADSECTION_TABLE */
/* asm: 	.word	LOADGGPARK,LOADSANFRAN,LOADALLH280,LOADREDWOOD,LOADBEVERLY,LOADLAFREEWAY */
/* asm: 	.word	LOADDEATHVALLEY,LOADARIZONA,LOADGCANYON,LOADIOWA,LOADCHICAGO */
/* asm: 	.word	LOADINDIANA,LOADAPPALACHIA,LOADWASHINGTONDC */
static void (*LOADSECTION_TABLE[])(void) = {
    LOADGGPARK, LOADSANFRAN, LOADALLH280, LOADREDWOOD, LOADBEVERLY, LOADLAFREEWAY,
    LOADDEATHVALLEY, LOADARIZONA, LOADGCANYON, LOADIOWA, LOADCHICAGO,
    LOADINDIANA, LOADAPPALACHIA, LOADWASHINGTONDC,
};

static void LOADGGPARK(void)
{
    // asm 00008C3C: 	LDL	_SECggate,AR2
    // asm 00008C3D: 	CALL	LOAD_SECTION_REQ
    // asm 00008C3E: 	LDL	_SECcorn,AR2
    // asm 00008C3F: 	CALL	LOAD_SECTION_REQ
    // asm 00008C40: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADGGPARK", 0, 0);
    UNIMPL();
}

static void LOADSANFRAN(void)
{
    // asm 00008C41: 	CALL	LOAD_TUNNEL2
    // asm 00008C42: 	LDL	_SECggate,AR2
    // asm 00008C43: 	CALL	LOAD_SECTION_REQ
    // asm 00008C44: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADSANFRAN", 0, 0);
    UNIMPL();
}

static void LOADALLH280(void)
{
    // asm 00008C45: 	CALL	LOAD_TUNNEL2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADALLH280", 0, 0);
    UNIMPL();
}

static void LOADH280(void)
{
    // asm 00008C46: 	LDL	_SECdeserts,AR2
    // asm 00008C47: 	CALL	LOAD_SECTION_REQ
    // asm 00008C48: 	LDL	_SECh280,AR2
    // asm 00008C49: 	CALL	LOAD_SECTION_REQ
    // asm 00008C4A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADH280", 0, 0);
    UNIMPL();
}

static void LOADREDWOOD(void)
{
    // asm 00008C4B: 	CALL	LOADH280
    // asm 00008C4C: 	CALL	LOAD_TUNNEL
    // asm 00008C4D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADREDWOOD", 0, 0);
    UNIMPL();
}

static void LOADBEVERLY(void)
{
    // asm 00008C4E: 	CALL	LOADH280
    // asm 00008C4F: 	CALL	LOAD_TUNNEL
    // asm 00008C50: 	LDL	_SECbevhnred,AR2
    // asm 00008C51: 	CALL	LOAD_SECTION_REQ
    // asm 00008C52: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADBEVERLY", 0, 0);
    UNIMPL();
}

static void LOADLAFREEWAY(void)
{
    // asm 00008C53: 	CALL	LOAD_TUNNEL
    // asm 00008C54: 	LDL	_SEClafreeway,AR2
    // asm 00008C55: 	CALL	LOAD_SECTION_REQ
    // asm 00008C56: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADLAFREEWAY", 0, 0);
    UNIMPL();
}

static void LOADDEATHVALLEY(void)
{
    // asm 00008C57: 	CALL	LOAD_TUNNEL
    // asm 00008C58: 	LDL	_SECdeathvalley,AR2
    // asm 00008C59: 	CALL	LOAD_SECTION_REQ
    // asm 00008C5A: 	LDL	_SECthetrains,AR2
    // asm 00008C5B: 	CALL	LOAD_SECTION_REQ
    // asm 00008C5C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADDEATHVALLEY", 0, 0);
    UNIMPL();
}

static void LOADARIZONA(void)
{
    // asm 00008C5D: 	CALL	LOAD_TUNNEL
    // asm 00008C5E: 	LDL	_SECarizona,AR2
    // asm 00008C5F: 	CALL	LOAD_SECTION_REQ
    // asm 00008C60: 	LDL	_SECdeserts,AR2
    // asm 00008C61: 	CALL	LOAD_SECTION_REQ
    // asm 00008C62: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADARIZONA", 0, 0);
    UNIMPL();
}

static void LOADGCANYON(void)
{
    // asm 00008C63: 	LDL	_SECdeserts,AR2
    // asm 00008C64: 	CALL	LOAD_SECTION_REQ
    // asm 00008C65: 	LDL	_SECmtrush,AR2
    // asm 00008C66: 	CALL	LOAD_SECTION_REQ
    // asm 00008C67: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADGCANYON", 0, 0);
    UNIMPL();
}

static void LOADIOWA(void)
{
    // asm 00008C68: 	LDL	_SECiowa_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008C69: 	CALL	LOAD_SECTION_REQ
    // asm 00008C6A: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008C6B: 	CALL	LOAD_SECTION_REQ
    // asm 00008C6C: 	LDL	_SECmwest,AR2
    // asm 00008C6D: 	CALL	LOAD_SECTION_REQ
    // asm 00008C6E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADIOWA", 0, 0);
    UNIMPL();
}

static void LOADCHICAGO(void)
{
    // asm 00008C6F: 	LDL	_SECthetrains,AR2
    // asm 00008C70: 	CALL	LOAD_SECTION_REQ
    // asm 00008C71: 	LDL	_SECiowa_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008C72: 	CALL	LOAD_SECTION_REQ
    // asm 00008C73: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008C74: 	CALL	LOAD_SECTION_REQ
    // asm 00008C75: 	LDL	_SECchicago,AR2
    // asm 00008C76: 	CALL	LOAD_SECTION_REQ
    // asm 00008C77: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADCHICAGO", 0, 0);
    UNIMPL();
}

static void LOADINDIANA(void)
{
    // asm 00008C78: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008C79: 	CALL	LOAD_SECTION_REQ
    // asm 00008C7A: 	LDL	_SECmwest,AR2
    // asm 00008C7B: 	CALL	LOAD_SECTION_REQ
    // asm 00008C7C: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008C7D: 	CALL	LOAD_SECTION_REQ
    // asm 00008C7E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADINDIANA", 0, 0);
    UNIMPL();
}

static void LOADAPPALACHIA(void)
{
    // asm 00008C7F: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008C80: 	CALL	LOAD_SECTION_REQ
    // asm 00008C81: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008C82: 	CALL	LOAD_SECTION_REQ
    // asm 00008C83: 	LDL	_SECappl,AR2
    // asm 00008C84: 	CALL	LOAD_SECTION_REQ
    // asm 00008C85: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADAPPALACHIA", 0, 0);
    UNIMPL();
}

static void LOADWASHINGTONDC(void)
{
    // asm 00008C86: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008C87: 	CALL	LOAD_SECTION_REQ
    // asm 00008C88: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008C89: 	CALL	LOAD_SECTION_REQ
    // asm 00008C8A: 	LDL	_SECdc,AR2
    // asm 00008C8B: 	CALL	LOAD_SECTION_REQ
    // asm 00008C8C: 	LDL	_SECdc_shared,AR2
    // asm 00008C8D: 	CALL	LOAD_SECTION_REQ
    // asm 00008C8E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOADWASHINGTONDC", 0, 0);
    UNIMPL();
}

static void LOAD_TUNNEL2(void)
{
    // asm 00008C8F: 	LDL	_SECtunnel2,AR2
    // asm 00008C90: 	CALL	LOAD_SECTION_REQ
    // asm 00008C91: 	LDL	tunnel2_PALETTES,AR2
    // asm 00008C92: 	CALL	alloc_section
    // asm 00008C93: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_TUNNEL2", 0, 0);
    UNIMPL();
}

static void WATERON(void)
{
    // asm 00008C94: 	LDI	@_MODE,R0
    // asm 00008C95: 	OR	MWATER,R0
    // asm 00008C96: 	STI	R0,@_MODE
    // asm 00008C97: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATERON", 0, 0);
    UNIMPL();
}

static void WATEROFF(void)
{
    // asm 00008C98: 	LDI	@_MODE,R0
    // asm 00008C99: 	ANDN	MWATER,R0
    // asm 00008C9A: 	STI	R0,@_MODE
    // asm 00008C9B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATEROFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_GGPARK(void)
{
    // asm 00008C9C: 	LDL	_SECggate,AR2
    // asm 00008C9D: 	CALL	LOAD_SECTION_REQ
    // asm 00008C9E: 	LDL	_SECcorn,AR2
    // asm 00008C9F: 	CALL	LOAD_SECTION_REQ
    // asm 00008CA0: 	LDI	1,R0
    // asm 00008CA1: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008CA2: 	LDI	60,R0
    // asm 00008CA3: 	STI	R0,@DD_SLP
    // asm 00008CA4: 	LDI	100,R0
    // asm 00008CA5: 	STI	R0,@DD_VAR
    // asm 00008CA6: 	LDL	ggate_PALETTES,AR2
    // asm 00008CA7: 	CALL	alloc_section
    // asm 00008CA8: 	LDL	CORNPAL,AR2
    // asm 00008CA9: 	CALL	PAL_ALLOC_RAW
    // asm 00008CAA: 	FLOAT	-15,R0
    // asm 00008CAB: 	STF	R0,@INFIN_CORRECT
    // asm 00008CAC: 	CALL	WATERON
    // asm 00008CAD: 	CALL	LEG_INIT
    // asm 00008CAE: 	LDI	1,AR0
    // asm 00008CAF: 	LDI	L_LEG1_END,AR1
    // asm 00008CB0: 	CALL	LEG_GENERATE_MAP
    // asm 00008CB1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_GGPARK", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_SANFRAN(void)
{
    // asm 00008CB2: 	LDL	_SECggate,AR2
    // asm 00008CB3: 	CALL	LOAD_SECTION_REQ
    // asm 00008CB4: 	CALL	LOAD_TUNNEL2
    // asm 00008CB5: 	LDL	ggate_PALETTES,AR2
    // asm 00008CB6: 	CALL	alloc_section
    // asm 00008CB7: 	LDL	tunnel2_PALETTES,AR2
    // asm 00008CB8: 	CALL	alloc_section
    // asm 00008CB9: 	FLOAT	-15,R0
    // asm 00008CBA: 	STF	R0,@INFIN_CORRECT
    // asm 00008CBB: 	CALL	WATERON
    // asm 00008CBC: 	LDI	120,R0
    // asm 00008CBD: 	STI	R0,@DD_SLP
    // asm 00008CBE: 	LDI	120,R0
    // asm 00008CBF: 	STI	R0,@DD_VAR
    // asm 00008CC0: 	LDI	1,R0
    // asm 00008CC1: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008CC2: 	LDI	2,R0		;DISCO DUCK
    // asm 00008CC3: 	STI	R0,@TUNE_IDX
    // asm 00008CC4: 	LDL	shared_PALETTES,AR2
    // asm 00008CC5: 	CALL	dealloc_section
    // asm 00008CC6: 	LDL	shared_PALETTES,AR2
    // asm 00008CC7: 	CALL	alloc_section
    // asm 00008CC8: 	LDL	_SECshared,AR2
    // asm 00008CC9: 	CALL	LOAD_SECTION_REQ
    // asm 00008CCA: 	LDI	0,R0
    // asm 00008CCB: 	STI	R0,@CHALLENGE_RACE
    // asm 00008CCC: 	CALL	LEG_INIT
    // asm 00008CCD: 	LDI	L_LEG2_BEGIN,AR0
    // asm 00008CCE: 	LS	8,AR0
    // asm 00008CCF: 	STI	AR0,@FINISH_LINE
    // asm 00008CD0: 	RS	8,AR0
    // asm 00008CD1: 	LDI	L_LEG2_END,AR1
    // asm 00008CD2: 	CALL	LEG_GENERATE_MAP
    // asm 00008CD3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_SANFRAN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_H280(void)
{
    // asm 00008CD4: 	CALL	LOAD_TUNNEL2
    // asm 00008CD5: 	LDL	_SECh280,AR2
    // asm 00008CD6: 	CALL	LOAD_SECTION_REQ
    // asm 00008CD7: 	LDL	_SECdeserts,AR2
    // asm 00008CD8: 	CALL	LOAD_SECTION_REQ
    // asm 00008CD9: 	LDI	3,R0		;SPAGETTI
    // asm 00008CDA: 	STI	R0,@TUNE_IDX
    // asm 00008CDB: 	LDI	60,R0
    // asm 00008CDC: 	STI	R0,@DD_SLP
    // asm 00008CDD: 	LDI	100,R0
    // asm 00008CDE: 	STI	R0,@DD_VAR
    // asm 00008CDF: 	LDI	0,R0
    // asm 00008CE0: 	STI	R0,@CHALLENGE_RACE
    // asm 00008CE1: 	LDI	0,R0
    // asm 00008CE2: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008CE3: 	LDL	deserts_PALETTES,AR2
    // asm 00008CE4: 	CALL	alloc_section
    // asm 00008CE5: 	LDL	h280_PALETTES,AR2
    // asm 00008CE6: 	CALL	alloc_section
    // asm 00008CE7: 	FLOAT	55,R0
    // asm 00008CE8: 	STF	R0,@INFIN_CORRECT
    // asm 00008CE9: 	LDI	@_MODE,R0
    // asm 00008CEA: 	ANDN	MWATER,R0
    // asm 00008CEB: 	STI	R0,@_MODE
    // asm 00008CEC: 	CALL	LEG_INIT
    // asm 00008CED: 	LDI	L_LEG3_BEGIN,AR0
    // asm 00008CEE: 	LS	8,AR0
    // asm 00008CEF: 	STI	AR0,@FINISH_LINE
    // asm 00008CF0: 	RS	8,AR0
    // asm 00008CF1: 	LDI	L_LEG3_END,AR1
    // asm 00008CF2: 	CALL	LEG_GENERATE_MAP
    // asm 00008CF3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_H280", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_REDWOOD(void)
{
    // asm 00008CF4: 	LDL	_SECdeserts,AR2
    // asm 00008CF5: 	CALL	LOAD_SECTION_REQ
    // asm 00008CF6: 	LDL	_SECh280,AR2
    // asm 00008CF7: 	CALL	LOAD_SECTION_REQ
    // asm 00008CF8: 	LDL	deserts_PALETTES,AR2
    // asm 00008CF9: 	CALL	alloc_section
    // asm 00008CFA: 	LDL	h280_PALETTES,AR2
    // asm 00008CFB: 	CALL	alloc_section
    // asm 00008CFC: 	LDL	_SECbevhnred,AR2
    // asm 00008CFD: 	CALL	LOAD_SECTION_REQ
    // asm 00008CFE: 	LDL	bevhnred_PALETTES,AR2
    // asm 00008CFF: 	CALL	alloc_section
    // asm 00008D00: 	CALL	WATEROFF
    // asm 00008D01: 	LDI	0,R0		;MONSTER SURF
    // asm 00008D02: 	STI	R0,@TUNE_IDX
    // asm 00008D03: 	LDI	1,R0
    // asm 00008D04: 	STI	R0,@CHALLENGE_RACE
    // asm 00008D05: 	LDI	1,R0
    // asm 00008D06: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008D07: 	LDI	120,R0
    // asm 00008D08: 	STI	R0,@DD_SLP
    // asm 00008D09: 	LDI	120,R0
    // asm 00008D0A: 	STI	R0,@DD_VAR
    // asm 00008D0B: 	FLOAT	50,R0
    // asm 00008D0C: 	STF	R0,@INFIN_CORRECT
    // 	;load proper tunnel
    // asm 00008D0D: 	CALL	LOAD_TUNNEL
    // asm 00008D0E: 	CALL	LEG_INIT
    // asm 00008D0F: 	LDI	L_LEG4_BEGIN,AR0
    // asm 00008D10: 	LS	8,AR0
    // asm 00008D11: 	STI	AR0,@FINISH_LINE
    // asm 00008D12: 	RS	8,AR0
    // asm 00008D13: 	LDI	L_LEG4_END,AR1
    // asm 00008D14: 	CALL	LEG_GENERATE_MAP
    // asm 00008D15: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_REDWOOD", 0, 0);
    UNIMPL();
}

void LOAD_TUNNEL(void)
{
    // asm 00008D16: 	LDL	tunnel_PALETTES,AR2
    // asm 00008D17: 	CALL	alloc_section
    // asm 00008D18: 	LDL	_SECtunnel,AR2
    // asm 00008D19: 	CALL	LOAD_SECTION_REQ
    // asm 00008D1A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_TUNNEL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_BEVERLY(void)
{
    // asm 00008D1B: 	CALL	LOAD_TUNNEL
    // asm 00008D1C: 	LDL	_SECdeserts,AR2
    // asm 00008D1D: 	CALL	LOAD_SECTION_REQ
    // asm 00008D1E: 	LDL	_SECbevhnred,AR2
    // asm 00008D1F: 	CALL	LOAD_SECTION_REQ
    // asm 00008D20: 	LDI	60,R0
    // asm 00008D21: 	STI	R0,@DD_SLP
    // asm 00008D22: 	LDI	100,R0
    // asm 00008D23: 	STI	R0,@DD_VAR
    // asm 00008D24: 	LDL	bevhnred_PALETTES,AR2
    // asm 00008D25: 	CALL	alloc_section
    // asm 00008D26: 	LDL	deserts_PALETTES,AR2
    // asm 00008D27: 	CALL	alloc_section
    // asm 00008D28: 	CALL	WATEROFF
    // asm 00008D29: 	LDI	5,R0		;STRAIGHT 2 4
    // asm 00008D2A: 	STI	R0,@TUNE_IDX
    // asm 00008D2B: 	LDI	0,R0
    // asm 00008D2C: 	STI	R0,@CHALLENGE_RACE
    // asm 00008D2D: 	STI	R0,@COCONUT_COUNT
    // asm 00008D2E: 	LDI	1,R0
    // asm 00008D2F: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008D30: 	FLOAT	45,R0
    // asm 00008D31: 	STF	R0,@INFIN_CORRECT
    // asm 00008D32: 	CALL	LEG_INIT
    // asm 00008D33: 	LDI	L_LEG5_BEGIN,AR0
    // asm 00008D34: 	LS	8,AR0
    // asm 00008D35: 	STI	AR0,@FINISH_LINE
    // asm 00008D36: 	RS	8,AR0
    // asm 00008D37: 	LDI	L_LEG5_END,AR1
    // asm 00008D38: 	CALL	LEG_GENERATE_MAP
    // asm 00008D39: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_BEVERLY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_LAFREEWAY(void)
{
    // asm 00008D3A: 	CALL	LOAD_TUNNEL
    // asm 00008D3B: 	LDL	_SEClafreeway,AR2
    // asm 00008D3C: 	CALL	LOAD_SECTION_REQ
    // asm 00008D3D: 	CALL	WATEROFF
    // asm 00008D3E: 	LDI	2,R0		;DISCO DUCK
    // asm 00008D3F: 	STI	R0,@TUNE_IDX
    // asm 00008D40: 	LDI	0,R0
    // asm 00008D41: 	STI	R0,@CHALLENGE_RACE
    // asm 00008D42: 	LDI	1,R0
    // asm 00008D43: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008D44: 	FLOAT	35,R0
    // asm 00008D45: 	STF	R0,@INFIN_CORRECT
    // asm 00008D46: 	LDL	lafreeway_PALETTES,AR2
    // asm 00008D47: 	CALL	alloc_section
    // asm 00008D48: 	CALL	LEG_INIT
    // asm 00008D49: 	LDI	L_LEG6_BEGIN,AR0
    // asm 00008D4A: 	LS	8,AR0
    // asm 00008D4B: 	STI	AR0,@FINISH_LINE
    // asm 00008D4C: 	RS	8,AR0
    // asm 00008D4D: 	LDI	L_LEG6_END,AR1
    // asm 00008D4E: 	CALL	LEG_GENERATE_MAP
    // asm 00008D4F: 	LDI	60,R0
    // asm 00008D50: 	STI	R0,@DD_SLP
    // asm 00008D51: 	LDI	100,R0
    // asm 00008D52: 	STI	R0,@DD_VAR
    // asm 00008D53: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_LAFREEWAY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_DEATHVALLEY(void)
{
    // asm 00008D54: 	LDL	_SECdeathvalley,AR2
    // asm 00008D55: 	CALL	LOAD_SECTION_REQ
    // asm 00008D56: 	LDL	_SECthetrains,AR2
    // asm 00008D57: 	CALL	LOAD_SECTION_REQ
    // asm 00008D58: 	CALL	LOAD_TUNNEL
    // asm 00008D59: 	CALL	WATEROFF
    // asm 00008D5A: 	LDI	3,R0		;SPAGETTI
    // asm 00008D5B: 	STI	R0,@TUNE_IDX
    // asm 00008D5C: 	LDI	1,R0
    // asm 00008D5D: 	STI	R0,@CHALLENGE_RACE
    // asm 00008D5E: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008D5F: 	FLOAT	55,R0
    // asm 00008D60: 	STF	R0,@INFIN_CORRECT
    // asm 00008D61: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008D62: 	LDI	bottom2_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008D63: 	CALL	PAL_OVERWRITE
    // ;	LDL	lafreeway_PALETTES,AR2
    // ;	CALL	dealloc_section
    // asm 00008D64: 	LDL	deathvalley_PALETTES,AR2
    // asm 00008D65: 	CALL	alloc_section
    // asm 00008D66: 	LDL	thetrains_PALETTES,AR2
    // asm 00008D67: 	CALL	alloc_section
    // asm 00008D68: 	LDI	60,R0
    // asm 00008D69: 	STI	R0,@DD_SLP
    // asm 00008D6A: 	LDI	100,R0
    // asm 00008D6B: 	STI	R0,@DD_VAR
    // asm 00008D6C: 	CALL	LEG_INIT
    // asm 00008D6D: 	LDI	L_LEG7_BEGIN,AR0
    // asm 00008D6E: 	LS	8,AR0
    // asm 00008D6F: 	STI	AR0,@FINISH_LINE
    // asm 00008D70: 	RS	8,AR0
    // asm 00008D71: 	LDI	L_LEG7_END,AR1
    // asm 00008D72: 	CALL	LEG_GENERATE_MAP
    // asm 00008D73: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_DEATHVALLEY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_ARIZONA(void)
{
    // asm 00008D74: 	LDL	tunnel_PALETTES,AR2
    // asm 00008D75: 	CALL	alloc_section
    // asm 00008D76: 	CALL	WATEROFF
    // asm 00008D77: 	LDL	deserts_PALETTES,AR2
    // asm 00008D78: 	CALL	alloc_section
    // asm 00008D79: 	CALL	LOAD_TUNNEL
    // asm 00008D7A: 	LDL	arizona_PALETTES,AR2
    // asm 00008D7B: 	CALL	alloc_section
    // asm 00008D7C: 	LDL	_SECdeserts,AR2
    // asm 00008D7D: 	CALL	LOAD_SECTION_REQ
    // asm 00008D7E: 	LDL	_SECarizona,AR2
    // asm 00008D7F: 	CALL	LOAD_SECTION_REQ
    // asm 00008D80: 	LDI	1,R0		;SHUFFLE DRIVE
    // asm 00008D81: 	STI	R0,@TUNE_IDX
    // asm 00008D82: 	LDI	0,R0
    // asm 00008D83: 	STI	R0,@CHALLENGE_RACE
    // asm 00008D84: 	LDI	1,R0
    // asm 00008D85: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008D86: 	FLOAT	45,R0
    // asm 00008D87: 	STF	R0,@INFIN_CORRECT
    // asm 00008D88: 	LDI	60,R0
    // asm 00008D89: 	STI	R0,@DD_SLP
    // asm 00008D8A: 	LDI	100,R0
    // asm 00008D8B: 	STI	R0,@DD_VAR
    // asm 00008D8C: 	CALL	LEG_INIT
    // asm 00008D8D: 	LDI	L_LEG8_BEGIN,AR0
    // asm 00008D8E: 	LS	8,AR0
    // asm 00008D8F: 	STI	AR0,@FINISH_LINE
    // asm 00008D90: 	RS	8,AR0
    // asm 00008D91: 	LDI	L_LEG8_END,AR1
    // asm 00008D92: 	CALL	LEG_GENERATE_MAP
    // asm 00008D93: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_ARIZONA", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_GCANYON(void)
{
    // asm 00008D94: 	LDL	_SECdeserts,AR2
    // asm 00008D95: 	CALL	LOAD_SECTION_REQ
    // asm 00008D96: 	LDL	_SECmtrush,AR2
    // asm 00008D97: 	CALL	LOAD_SECTION_REQ
    // asm 00008D98: 	CALL	WATEROFF
    // asm 00008D99: 	LDL	deserts_PALETTES,AR2
    // asm 00008D9A: 	CALL	alloc_section
    // asm 00008D9B: 	LDL	_SECdeserts,AR2
    // asm 00008D9C: 	CALL	LOAD_SECTION_REQ
    // 	;(MOUNT RUSHMORE)
    // asm 00008D9D: 	LDI	3,R0		;SPAGETTI
    // asm 00008D9E: 	STI	R0,@TUNE_IDX
    // asm 00008D9F: 	LDI	1,R0
    // asm 00008DA0: 	STI	R0,@CHALLENGE_RACE
    // asm 00008DA1: 	LDI	0,R0
    // asm 00008DA2: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008DA3: 	FLOAT	55,R0
    // asm 00008DA4: 	STF	R0,@INFIN_CORRECT
    // asm 00008DA5: 	LDL	mtrush_PALETTES,AR2
    // asm 00008DA6: 	CALL	alloc_section
    // asm 00008DA7: 	LDI	75,R0
    // asm 00008DA8: 	STI	R0,@DD_SLP
    // asm 00008DA9: 	LDI	125,R0
    // asm 00008DAA: 	STI	R0,@DD_VAR
    // asm 00008DAB: 	CALL	LEG_INIT
    // asm 00008DAC: 	LDI	L_LEG9_BEGIN,AR0
    // asm 00008DAD: 	LS	8,AR0
    // asm 00008DAE: 	STI	AR0,@FINISH_LINE
    // asm 00008DAF: 	RS	8,AR0
    // asm 00008DB0: 	LDI	L_LEG9_END,AR1
    // asm 00008DB1: 	CALL	LEG_GENERATE_MAP
    // asm 00008DB2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_GCANYON", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_IOWA(void)
{
    // asm 00008DB3: 	CALL	WATEROFF
    // asm 00008DB4: 	LDI	7,R0		;BOGGIE
    // asm 00008DB5: 	STI	R0,@TUNE_IDX
    // asm 00008DB6: 	LDI	0,R0
    // asm 00008DB7: 	STI	R0,@CHALLENGE_RACE
    // asm 00008DB8: 	LDI	1,R0
    // asm 00008DB9: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008DBA: 	FLOAT	45,R0
    // asm 00008DBB: 	STF	R0,@INFIN_CORRECT
    // asm 00008DBC: 	LDL	_SECiowa_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008DBD: 	CALL	LOAD_SECTION_REQ
    // asm 00008DBE: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008DBF: 	CALL	LOAD_SECTION_REQ
    // asm 00008DC0: 	LDL	_SECmwest,AR2
    // asm 00008DC1: 	CALL	LOAD_SECTION_REQ
    // asm 00008DC2: 	LDI	75,R0
    // asm 00008DC3: 	STI	R0,@DD_SLP
    // asm 00008DC4: 	LDI	125,R0
    // asm 00008DC5: 	STI	R0,@DD_VAR
    // asm 00008DC6: 	LDL	mwest_PALETTES,AR2
    // asm 00008DC7: 	CALL	alloc_section
    // asm 00008DC8: 	LDI	bottom_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008DC9: 	LDI	bottom3_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008DCA: 	CALL	PAL_OVERWRITE
    // asm 00008DCB: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008DCC: 	LDI	btomwt_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008DCD: 	CALL	PAL_OVERWRITE
    // asm 00008DCE: 	CALL	LEG_INIT
    // asm 00008DCF: 	LDI	L_LEG10_BEGIN,AR0
    // asm 00008DD0: 	LS	8,AR0
    // asm 00008DD1: 	STI	AR0,@FINISH_LINE
    // asm 00008DD2: 	RS	8,AR0
    // asm 00008DD3: 	LDI	L_LEG10_END,AR1
    // asm 00008DD4: 	CALL	LEG_GENERATE_MAP
    // asm 00008DD5: 	CREATE	BUG_SPAWNER_PROC,SPAWNER_C
    // asm 00008DD8: 	CREATE	GEESE_SPAWNER,SPAWNER_C
    // asm 00008DDB: 	CREATE	DEER_SPAWNER_PROC,SPAWNER_C
    // asm 00008DDE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_IOWA", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_CHICAGO(void)
{
    // asm 00008DDF: 	LDL	_SECthetrains,AR2
    // asm 00008DE0: 	CALL	LOAD_SECTION_REQ
    // asm 00008DE1: 	LDL	_SECchicago,AR2
    // asm 00008DE2: 	CALL	LOAD_SECTION_REQ
    // asm 00008DE3: 	CALL	WATEROFF
    // asm 00008DE4: 	LDI	1,R0
    // asm 00008DE5: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008DE6: 	LDI	2,R0		;DISCO DUCK
    // asm 00008DE7: 	STI	R0,@TUNE_IDX
    // asm 00008DE8: 	LDI	1,R0
    // asm 00008DE9: 	STI	R0,@CHALLENGE_RACE
    // asm 00008DEA: 	FLOAT	55,R0
    // asm 00008DEB: 	STF	R0,@INFIN_CORRECT
    // asm 00008DEC: 	LDL	thetrains_PALETTES,AR2
    // asm 00008DED: 	CALL	alloc_section
    // asm 00008DEE: 	LDL	chicago_PALETTES,AR2
    // asm 00008DEF: 	CALL	alloc_section
    // asm 00008DF0: 	LDI	60,R0
    // asm 00008DF1: 	STI	R0,@DD_SLP
    // asm 00008DF2: 	LDI	100,R0
    // asm 00008DF3: 	STI	R0,@DD_VAR
    // asm 00008DF4: 	CALL	LEG_INIT
    // asm 00008DF5: 	LDI	L_LEG11_BEGIN,AR0
    // asm 00008DF6: 	LS	8,AR0
    // asm 00008DF7: 	STI	AR0,@FINISH_LINE
    // asm 00008DF8: 	RS	8,AR0
    // asm 00008DF9: 	LDI	L_LEG11_END,AR1
    // asm 00008DFA: 	CALL	LEG_GENERATE_MAP
    // asm 00008DFB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_CHICAGO", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_INDIANA(void)
{
    // asm 00008DFC: 	CALL	WATEROFF
    // asm 00008DFD: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008DFE: 	CALL	LOAD_SECTION_REQ
    // asm 00008DFF: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008E00: 	LDI	btomwt_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008E01: 	CALL	PAL_OVERWRITE
    // asm 00008E02: 	LDI	4,R0		;VENTURE
    // asm 00008E03: 	STI	R0,@TUNE_IDX
    // asm 00008E04: 	LDI	0,R0
    // asm 00008E05: 	STI	R0,@CHALLENGE_RACE
    // asm 00008E06: 	LDI	1,R0
    // asm 00008E07: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008E08: 	FLOAT	45,R0
    // asm 00008E09: 	STF	R0,@INFIN_CORRECT
    // asm 00008E0A: 	LDL	mwest_PALETTES,AR2
    // asm 00008E0B: 	CALL	alloc_section
    // asm 00008E0C: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008E0D: 	CALL	LOAD_SECTION_REQ
    // asm 00008E0E: 	LDI	bottom_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008E0F: 	LDI	midwgras_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008E10: 	CALL	PAL_OVERWRITE
    // asm 00008E11: 	LDL	_SECmwest,AR2
    // asm 00008E12: 	CALL	LOAD_SECTION_REQ
    // asm 00008E13: 	LDI	75,R0
    // asm 00008E14: 	STI	R0,@DD_SLP
    // asm 00008E15: 	LDI	125,R0
    // asm 00008E16: 	STI	R0,@DD_VAR
    // asm 00008E17: 	CALL	LEG_INIT
    // asm 00008E18: 	LDI	L_LEG12_BEGIN,AR0
    // asm 00008E19: 	LS	8,AR0
    // asm 00008E1A: 	STI	AR0,@FINISH_LINE
    // asm 00008E1B: 	RS	8,AR0
    // asm 00008E1C: 	LDI	L_LEG12_END,AR1
    // asm 00008E1D: 	CALL	LEG_GENERATE_MAP
    // asm 00008E1E: 	CREATE	BUG_SPAWNER_PROC,SPAWNER_C
    // asm 00008E21: 	CREATE	GEESE_SPAWNER,SPAWNER_C
    // ;	CREATE	DEER_SPAWNER_PROC,SPAWNER_C
    // asm 00008E24: 	CREATE	COW_SPAWNER_PROC,SPAWNER_C
    // asm 00008E27: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_INDIANA", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_APPALACHIA(void)
{
    // asm 00008E28: 	CALL	WATEROFF
    // asm 00008E29: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008E2A: 	CALL	LOAD_SECTION_REQ
    // asm 00008E2B: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008E2C: 	LDI	btomwt_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008E2D: 	CALL	PAL_OVERWRITE
    // asm 00008E2E: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008E2F: 	CALL	LOAD_SECTION_REQ
    // asm 00008E30: 	LDI	bottom_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008E31: 	LDI	midwgras_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008E32: 	CALL	PAL_OVERWRITE
    // asm 00008E33: 	LDI	5,R0		;STRAIGHT 2 4
    // asm 00008E34: 	STI	R0,@TUNE_IDX
    // asm 00008E35: 	LDI	1,R0
    // asm 00008E36: 	STI	R0,@CHALLENGE_RACE
    // asm 00008E37: 	LDI	0,R0
    // asm 00008E38: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008E39: 	LDI	60,R0
    // asm 00008E3A: 	STI	R0,@DD_SLP
    // asm 00008E3B: 	LDI	100,R0
    // asm 00008E3C: 	STI	R0,@DD_VAR
    // asm 00008E3D: 	FLOAT	85,R0
    // asm 00008E3E: 	STF	R0,@INFIN_CORRECT
    // asm 00008E3F: 	LDL	appl_PALETTES,AR2
    // asm 00008E40: 	CALL	alloc_section
    // asm 00008E41: 	LDL	_SECappl,AR2
    // asm 00008E42: 	CALL	LOAD_SECTION_REQ
    // asm 00008E43: 	CALL	LEG_INIT
    // asm 00008E44: 	LDI	L_LEG13_BEGIN,AR0
    // asm 00008E45: 	LS	8,AR0
    // asm 00008E46: 	STI	AR0,@FINISH_LINE
    // asm 00008E47: 	RS	8,AR0
    // asm 00008E48: 	LDI	L_LEG13_END,AR1
    // asm 00008E49: 	CALL	LEG_GENERATE_MAP
    // asm 00008E4A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_APPALACHIA", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void FULLSETUP_WASHINGTONDC(void)
{
    // asm 00008E4B: 	CALL	WATEROFF
    // asm 00008E4C: 	LDL	_SECmwest_bottom,AR2	;in reality it just overwrites 'BOTTOM.GTM'
    // asm 00008E4D: 	CALL	LOAD_SECTION_REQ
    // asm 00008E4E: 	LDI	bottom_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008E4F: 	LDI	midwgras_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008E50: 	CALL	PAL_OVERWRITE
    // asm 00008E51: 	LDL	_SECiowa_b2,AR2		;in reality it just overwrites 'BOTTOM2.GTM'
    // asm 00008E52: 	CALL	LOAD_SECTION_REQ
    // asm 00008E53: 	LDI	bottom2_gtmp_p,R0	;ONE TO OVERWRITE
    // asm 00008E54: 	LDI	btomwt_gtmp_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00008E55: 	CALL	PAL_OVERWRITE
    // asm 00008E56: 	LDI	2,R0			;DISCO DUCK
    // asm 00008E57: 	STI	R0,@TUNE_IDX
    // asm 00008E58: 	LDI	0,R0
    // asm 00008E59: 	STI	R0,@CHALLENGE_RACE
    // asm 00008E5A: 	STI	R0,@NOLONG_VEHICLES
    // asm 00008E5B: 	FLOAT	45,R0
    // asm 00008E5C: 	STF	R0,@INFIN_CORRECT
    // asm 00008E5D: 	LDI	60,R0
    // asm 00008E5E: 	STI	R0,@DD_SLP
    // asm 00008E5F: 	LDI	100,R0
    // asm 00008E60: 	STI	R0,@DD_VAR
    // asm 00008E61: 	LDL	dc_PALETTES,AR2
    // asm 00008E62: 	CALL	alloc_section
    // asm 00008E63: 	LDL	dc_shared_PALETTES,AR2
    // asm 00008E64: 	CALL	alloc_section
    // asm 00008E65: 	LDL	_SECdc_shared,AR2
    // asm 00008E66: 	CALL	LOAD_SECTION_REQ
    // asm 00008E67: 	LDL	_SECdc,AR2
    // asm 00008E68: 	CALL	LOAD_SECTION_REQ
    // asm 00008E69: 	CALL	LEG_INIT
    // asm 00008E6A: 	LDI	L_LEG14_BEGIN,AR0
    // asm 00008E6B: 	LS	8,AR0
    // asm 00008E6C: 	STI	AR0,@FINISH_LINE
    // asm 00008E6D: 	RS	8,AR0
    // asm 00008E6E: 	LDI	L_LEG14_END,AR1
    // asm 00008E6F: 	CALL	LEG_GENERATE_MAP
    // asm 00008E70: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FULLSETUP_WASHINGTONDC", 0, 0);
    UNIMPL();
}

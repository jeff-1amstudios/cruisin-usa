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
#include "../include/delta.h"
#include "../include/drones.h"

/*
 * Source module: asm/DRONES.ASM
 */

// *----------------------------------------------------------------------------
// *COMMON DRONE ROUTINES
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *----------------------------------------------------------------------------
// *DRONE TYPES
// *	DELTA	intelligent, race vs plyr
// *	RHO	oncoming traffic + RHO WEAVER
// *	SIGMA	slow moving same direction as plyr (obstacles)
// *		+ WEAVER SIGMA
// *	COPCAR
// *	CHOPPER	the helicopter
// *
/* asm: DRONE_DISPATCH_P	.bss	DRONE_DISPATCH_P,1 */
int DRONE_DISPATCH_P;
/* asm: SUSPEND_MODE	.bss	SUSPEND_MODE,1 */
int SUSPEND_MODE;
/* asm: RACER_GRID_START	.bss	RACER_GRID_START,1 */
int RACER_GRID_START;
// *----------------------------------------------------------------------------
/* asm: DRONE_COUNT	.bss	DRONE_COUNT,1 */
int DRONE_COUNT;
/* asm: MIN_TRACK_TIME	.bss	MIN_TRACK_TIME,1 */
int MIN_TRACK_TIME;
/* asm: PSYCHO_RHO	.bss	PSYCHO_RHO,1 */
int PSYCHO_RHO;
/* asm: LANEP	.word	LANES,LANES4 */
int LANEP[] = { LANES, LANES4 };
// *----------------------------------------------------------------------------
/* asm: ONCSCREEN_CARS	.bss	ONCSCREEN_CARS,1 */
int ONCSCREEN_CARS;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *DRONE DISPATCHER
// *
// *
/* asm: DD_SLP	.bss	DD_SLP,1 */
int DD_SLP;
/* asm: DD_VAR	.bss	DD_VAR,1 */
int DD_VAR;
/* asm: DD_MAX_DRONES	.bss	DD_MAX_DRONES,1 */
int DD_MAX_DRONES;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *ADD ID TO DRONE POINTER TABLE
// *
// *PARAMETERS
// *	AR4	OBJECT
// *	AR5	CAR BLOCK
// *RETURNS
// *	R0	ID
// *
/* asm: DRONENUM	.bss	DRONENUM,1 */
int DRONENUM;
// *----------------------------------------------------------------------------
// *
// *CHECK IF AHEAD IN LINK FOR DISPATCH
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// ;	.word	blast1,blast2,blast3,blast4,blast5
// ;	.word	blast6,blast7,blast8,blast9,blast10,-1
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *SMOKE_PUFF
// *
// *PARAMETERS
// *	AR4	OBJECT THAT IS SMOKING
// *
/* asm: SMOKE_COUNT	.bss	SMOKE_COUNT,1 */
int SMOKE_COUNT;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *PARAMETERS
// *	AR5	PALM TREE OBJECT
// *
/* asm: COCONUT_COUNT	.bss	COCONUT_COUNT,1 */
int COCONUT_COUNT;
// *----------------------------------------------------------------------------
/* asm: MODELTAB	.word	cvettem,hotrodm,missle,testorm */
int MODELTAB[] = { cvettem, hotrodm, missle, testorm };
/* asm: TEXTTABS	.word	VETETXT,RODRTXT,BULLTXT,FERRTXT */
int TEXTTABS[] = { VETETXT, RODRTXT, BULLTXT, FERRTXT };
/* asm: TITLES	.word	HRT12,HRT13,HRT14,HRT15,HRT16 */
int TITLES[] = { HRT12, HRT13, HRT14, HRT15, HRT16 };
/* asm: VETETXT	.word	HRS11,HRS12,HRS13,HRS14,HRS15,HRS16 */
int VETETXT[] = { HRS11, HRS12, HRS13, HRS14, HRS15, HRS16 };
/* asm: RODRTXT	.word	HRS21,HRS22,HRS23,HRS24,HRS25,HRS26 */
int RODRTXT[] = { HRS21, HRS22, HRS23, HRS24, HRS25, HRS26 };
/* asm: BULLTXT	.word	HRS31,HRS32,HRS33,HRS34,HRS35,HRS36 */
int BULLTXT[] = { HRS31, HRS32, HRS33, HRS34, HRS35, HRS36 };
/* asm: FERRTXT	.word	HRS41,HRS42,HRS43,HRS44,HRS45,HRS46 */
int FERRTXT[] = { HRS41, HRS42, HRS43, HRS44, HRS45, HRS46 };
const char HRT12[] = "TOP SPEED:";
const char HRT13[] = "SKIDPAD:";
const char HRT14[] = "AERO COEFF:";
const char HRT15[] = "0?60 MPH:";
const char HRT16[] = "POWER:";
const char HRS11[] = "63 MUSCLE CAR";
const char HRS12[] = "145 MPH@233 KPH";
const char HRS13[] = "0=89 G";
const char HRS14[] = "0=39";
const char HRS15[] = "2=98 SEC=";
const char HRS16[] = "462HP  454 CID V8";
const char HRS21[] = "LA BOMBA";
const char HRS22[] = "142 MPH@228 KPH";
const char HRS23[] = "0=98 G";
const char HRS24[] = "0=45";
const char HRS25[] = "2=51 SEC=";
const char HRS26[] = "580HP  SUPERCHARGED V12";
const char HRS31[] = "DEVASTATOR VI";
const char HRS32[] = "147 MPH@236 KPH";
const char HRS33[] = "0=85 G";
const char HRS34[] = "0=25";
const char HRS35[] = "3=15 SEC=";
const char HRS36[] = "395HP  TWIN TURBO V6";
const char HRS41[] = "ITALIA P69";
const char HRS42[] = "144 MPH@231 KPH";
const char HRS43[] = "1=01 G";
const char HRS44[] = "0=28";
const char HRS45[] = "2=88 SEC=";
const char HRS46[] = "472HP  V12 DOHC 48V";
// *----------------------------------------------------------------------------

void POSITION_FINDER(void)
{
    // asm: 	LDI	@PLYCAR,AR4
    // asm: 	LDI	@PLYCBLK,AR5
    // asm: 	CALL	FIND_PLAYERS_POSITION
    // asm: 	SLEEP	14
    // asm: 	BU	POSITION_FINDER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POSITION_FINDER", 0, 0);
    UNIMPL();
}

void FIND_PLAYERS_POSITION(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *FIND PLAYERS POSITION, AND # OF CARS CLOSE ONSCREEN
    // *
    // *PARAMETERS
    // *	AR4	PLAYERS OBJECT
    // *	AR5	PLAYERS CARBLOCK
    // *
    // * OUTPUT:SETS @POSITION, @ONCSCREEN_CARS
    // *
    // ;	CLRI	IR0	;TEMP FLAG FOR OTHER MACHINE
    // asm: 	LDI	1,R7		;POSITION #
    // asm: 	CLRI	R6		;CARS CLOSE TO SCREEN
    // asm: 	LDI	*+AR5(CARTRAK),AR6
    // asm: 	LDI	*+AR6(OUSR1),R1
    // asm: 	LDI	@CAR_LIST,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BZ	FPPX
FPPLP:
    // asm: 	LDI	0,R5
    // asm: 	LDI	*+AR0(ODIST),R0
    // asm: 	CMPI	30000,R0
    // asm: 	BGT	NNEG
    // asm: 	CMPI	-5000,R0
    // asm: 	BLT	NNEG
    // asm: 	INC	R6
NNEG:
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RACER,R0
    // asm: 	BNE	NXTLP
    // asm: 	LDI	*+AR0(OCARBLK),AR3
    // asm: 	LDI	*+AR3(CAR_OM),R0  	;OTHER MACHINES CAR?
    // asm: 	BZ	FPP1			;NO...
    // asm: 	LDI	*+AR3(CARTRACK_ID),R0	;CHECK IF AHEAD OR BEHIND
    // asm: 	CMPI	R1,R0
    // asm: 	BNE	NXTLP1
    // ;	BEQ	FURTHER
    // ;	BLT	ISBLAMP
    // ;
    // ;	CMPI	@PLY2CAR,AR0
    // ;	BNE	NXTLP1
    // ;
    // ;	LDI	0,R0
    // ;	STI	R0,@THIS_MACHINE_AHEAD
    // ;	BU	NXTLP1
    // ;
    // ;ISBLAMP	LDI	1,R0
    // ;	STI	R0,@THIS_MACHINE_AHEAD
    // ;	BU	NXTLP1
    // ;
    // ;FURTHER
    // asm: 	CMPI	@PLY2CAR,AR0
    // asm: 	LDIEQ	1,IR0
    // ;	LDI	1,IR0
    // asm: 	STI	AR6,*+AR3(CARTRAK)  	;CALC CARDIST2CNTR DUDE
    // asm: 	LDI	AR6,AR2
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	AR3,AR5
    // asm: 	CALL	DRONE_RIDE_RIGHT
    // asm: 	STF	R0,*+AR5(CARDIST2CNTR)	;STORE DIST TO CENTER
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	B	FPP2
FPP1:
    // asm: 	LDI	*+AR0(OPLINK),AR2
    // asm: 	LDI	*+AR2(STEALTHMODE),R0  	;0=ONSCRN,-1=BEHIND ST, 1=AHEAD STEALTH
    // asm: 	BNE	NXTLP1
    // asm: 	LDI	*+AR0(OCARBLK),AR3
    // asm: 	LDI	*+AR3(CARTRAK),AR2
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	CMPI	R1,R0
    // asm: 	BNE	NXTLP1
    // *
    // *ON SAME TRACK SEGMENT
    // *
FPP2:
    // asm: 	LDI	*+AR2(OLINK4),AR2		;get the next road piece
    // asm: 	SUBF	*+AR0(OPOSX),*+AR2(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	SUBF	*+AR2(OPOSZ),R3
    // asm: 	MPYF	R3,R3
    // asm: 	ADDF	R0,R3,R4
    // asm: 	LDF	*+AR3(CARDIST2CNTR),R0		;CORRECT FOR NOT CENTERED
    // asm: 	MPYF	R0,R0
    // asm: 	SUBF	R0,R4
    // asm: 	SUBF	*+AR4(OPOSX),*+AR2(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR4(OPOSZ),R3
    // asm: 	SUBF	*+AR2(OPOSZ),R3
    // asm: 	MPYF	R3,R3
    // asm: 	ADDF	R0,R3
    // asm: 	LDF	*+AR5(CARDIST2CNTR),R0		;CORRECT FOR NOT CENTERED
    // asm: 	MPYF	R0,R0
    // asm: 	SUBF	R0,R3
    // 	;CHECK FOR LAMP
    // ;	CMPI	0,IR0
    // ;	BEQ	KKDKA
    // ;	CLRI	IR0	;clear the flag
    // ;	CMPF	R4,R3
    // ;	LDIGT	0,R0
    // ;	LDILE	1,R0
    // ;	STI	R0,@THIS_MACHINE_AHEAD
    // asm: KKDKA
    // 	;END CHECK LAMP
    // asm: 	CMPF	R4,R3
NXTLP1:
    // asm: 	LDIGT	1,R5
    // asm: 	ADDI	R5,R7
NXTLP:
    // asm: 	LDI	*+AR0(OLINK3),AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BNE	FPPLP
FPPX:
    // asm: 	STI	R7,@POSITION
    // asm: 	STI	R6,@ONCSCREEN_CARS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_PLAYERS_POSITION", 0, 0);
    UNIMPL();
}

void SIGMA_DISPATCHER(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	CONTIN
    // asm: 	DIE
CONTIN:
    // asm: 	SLEEP	30*20
SIGDSP_LP:
    // asm: 	LDI	@DD_VAR,AR2
    // asm: 	CALL	RANDU0
    // asm: 	ADDI	@DD_SLP,R0
    // asm: 	MPYI	2,R0
    // asm: 	LDI	R0,AR2
SG_DISP_S:
    // asm: 	CALL	SLEEP
    // asm: 	LDI	1,AR2
    // asm: 	LDI	@DRONE_DISPATCH_P,R0
    // asm: 	BZ	SG_DISP_S
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BEQ	SIGDSP_LP	;->STRAIGHT TO SLEEP
    // asm: 	LDI	@DRONE_COUNT,R0
    // asm: 	SUBI	1,R0
    // asm: 	CMPI	@DD_MAX_DRONES,R0
    // asm: 	BGT	SIGDSP_LP
    // asm: 	LDI	@POSITION,R1
    // asm: 	CMPI	1,R1
    // asm: 	BLE	DOIT
    // asm: 	RANDN	100		;R0 <- rand %%
    // asm: 	CMPI	4,R1
    // asm: 	BGT	OV1
    // asm: 	CMPI	70,R0
    // asm: 	BLT	DOIT
    // asm: 	BGT	SIGDSP_LP
OV1:
    // asm: CMPI	7,R1
    // asm: 	BGT	OV2
    // asm: 	CMPI	60,R0
    // asm: 	BLT	DOIT
    // asm: 	BGT	SIGDSP_LP
OV2:
    // asm: CMPI	35,R0
    // asm: 	BGT	SIGDSP_LP
DOIT:
    // ;	CALL	COP_ACTIVE	;NO SIGMAS MIXED WITH COPS
    // ;	BC	SIGDSP_LP
    // asm: 	CREATE	SIGMA_DRONE,DRONE_C|VEHICLE_T|DRNE_SIGMA
    // asm: 	BC	SIGDSP_LP
    // asm: 	LDI	AR0,AR5
NOTYET:
    // asm: SLEEP	1
    // asm: 	LDI	*+AR5(PAR4),AR4
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	CMPI	DRONE_C|VEHICLE_T|DRNE_SIGMA,R0
    // asm: 	BNE	SIGDSP_LP
    // asm: 	LDI	@DGROUP_AW,AR0
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	1,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OPOSX,R2
    // asm: 	CALL	GET_XZ_DISTANCE
    // ;	FLOAT	15000,R1
    // asm: 	FLOAT	25000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BLT	NOTYET
    // asm: 	BU	SIGDSP_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIGMA_DISPATCHER", 0, 0);
    UNIMPL();
}

void DRONE_PTR_ADD(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_OM)
    // asm: 	LDI	@DRONENUM,R0		;INCREMENT ID #
    // asm: 	ADDI	1,R0
    // asm: 	CMPI	127,R0
    // asm: 	LDIGT	10,R0			;RESET TO 10
    // asm: 	STI	R0,@DRONENUM
    // asm: 	LDI	@DIPRAM,R1
    // asm: 	TSTB	CMDP_MASTER,R1		;CHECK MASTER
    // asm: 	LDIZ	80H,R1			;YES, SET BIT 7
    // asm: 	LDINZ	0,R1
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,*+AR5(CARNUM)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_PTR_ADD", 0, 0);
    UNIMPL();
}

void DRONE_CLR(void)
{
    // asm: 	LDI	10,R0
    // asm: 	STI	R0,@DRONENUM
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_CLR", 0, 0);
    UNIMPL();
}

void RHO_DISPATCHER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // *MODIFICATION, THE PLYR MUST MOVE AT LEAST 9000 Voxels FROM THE
    // *INITIAL STARTING POSITION FOR US TO RELEASE ANY RHO DRONES, BECAUSE
    // *THEY DISAPPEAR WHEN THEY BEGIN TRACKING NO WORLD, WHICH AT THE STARTUP
    // *OF EACH LEG IS REALLY CUTTING IT
    // *
    // *
    // asm: 	CALL	DRONE_CLR 	;CLEAR OUT DRONE POINTER TABLE
    // asm: 	SLEEP	30		;WAIT A SECOND
    // asm: 	LDI	@PLYCAR,AR4
    // asm: 	LDF	*+AR4(OPOSX),R6
    // asm: 	LDF	*+AR4(OPOSZ),R7	;GRAB THE 'INITIAL' LOCATION
    // asm: 	SLEEP	30*20		;WAIT LIKE 20 SECONDS ANYWAY...
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	BEGIN_RELEASING
#if DEBUG
    // asm: 	CMPI	@PLYCAR,AR4
    // asm: 	BNE	$
#endif
RHOPSL:
    // asm: 	LDF	R6,R0
    // asm: 	LDF	R7,R2
    // asm: 	SUBF	*+AR4(OPOSX),R0
    // asm: 	SUBF	*+AR4(OPOSZ),R2
    // asm: 	MPYF	R0,R0
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R0,R2
    // asm: 	CALL	SQRT
    // asm: 	FLOAT	11000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	BEGIN_RELEASING
    // asm: 	SLEEP	1
    // asm: 	BU	RHOPSL
BEGIN_RELEASING:
RHO_DLP:
    // asm: 	LDI	@POSITION,R0
    // asm: 	CMPI	2,R0
    // asm: 	BGT	NKLD
    // asm: 	LDI	50,AR2
    // asm: 	BU	TF_DISP_S
NKLD:
    // asm: 	LDI	@DD_VAR,AR2
    // asm: 	CALL	RANDU0
    // asm: 	ADDI	@DD_SLP,R0
    // asm: 	LDI	R0,AR2
TF_DISP_S:
    // asm: 	CALL	SLEEP
    // asm: 	LDI	1,AR2
    // asm: 	LDI	@DRONE_DISPATCH_P,R0
    // asm: 	BZ	TF_DISP_S
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BEQ	RHO_DLP
    // asm: 	LDI	@DRONE_COUNT,R0
    // asm: 	CMPI	@DD_MAX_DRONES,R0
    // asm: 	BGT	RHO_DLP
    // asm: 	LDI	@POSITION,R1
    // asm: 	CMPI	2,R1
    // asm: 	BLE	DOITR
    // asm: 	RANDN	100		;R0 <- rand %%
    // asm: 	CMPI	4,R1
    // asm: 	BGT	OV1R
    // asm: 	CMPI	75,R0
    // asm: 	BLT	DOITR
    // asm: 	BGT	RHO_DLP
OV1R:
    // asm: CMPI	7,R1
    // asm: 	BGT	OV2R
    // asm: 	CMPI	60,R0
    // asm: 	BLT	DOITR
    // asm: 	BGT	RHO_DLP
OV2R:
    // asm: CMPI	35,R0
    // asm: 	BGT	RHO_DLP
DOITR:
    // asm: 	LDI	@ONCSCREEN_CARS,R0
    // asm: 	CMPI	8,R0
    // asm: 	BGT	RHO_DLP
    // asm: 	CALL	CK_LINK_DISP
    // asm: 	BZ	RHO_DLP			;NOT DISPATCHING, BEHIND IN LINK
    // asm: 	CREATE	RHO_DRONE,DRONE_C|VEHICLE_T|DRNE_RHO
    // asm: 	BU	RHO_DLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_DISPATCHER", 0, 0);
    UNIMPL();
}

void SET_DRONE_PAL(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDI	*+AR2(VEHTAB_PALTAB),AR2
    // asm: 	LDI	*AR2++,R0
    // asm: 	BZ	NO_EPALS
    // asm: 	PUSH	AR2
    // asm: 	INC	R0
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	RANDU0
    // asm: 	POP	AR2
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NO_EPALS
    // asm: 	DEC	R0
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	BNC	NO_EPALS
    // asm: 	STI	R0,*+AR4(OPAL)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
NO_EPALS:
    // asm: 	POP	AR2
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_DRONE_PAL", 0, 0);
    UNIMPL();
}

void GET_LANES(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PARAMETERS
    // *	AR2	OBJECT TO CHECK VALID LANES FOR...
    // *RETURNS
    // *	R0	0 - 2 lanes
    // *		1 - 4 lanes
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	RS	8,R0
    // asm: 	LDI	@DGROUP_COUNT,AR0
    // asm: 	INC	AR0
    // asm: 	LDI	@DGROUPSI,AR1
GL_LP:
    // asm: CMPI	*+AR1(DGRP_IDX),R0
    // asm: 	BEQ	GL_FND
    // asm: 	ADDI	DGRP_SIZE,AR1
    // asm: 	DBU	AR0,GL_LP
    // asm: 	CLRI	R0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
GL_FND:
    // asm: 	LDI	*+AR1(DGRP_FLAG),R0
    // asm: 	RS	7,R0
    // asm: 	AND	1,R0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_LANES", 0, 0);
    UNIMPL();
}

void DIST_TO_PLYR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET X/Z DISTANCE TO PLAYER
    // *
    // *PARAMETERS
    // *	AR4	OBJECT TO CHECK
    // *RETURNS
    // *	R0	FL DISTANCE TO PLAYER (IN VOXELS)
    // *
    // asm: 	PUSH	AR3
    // asm: 	PUSHFL	R1
    // asm: 	PUSHFL	R2
    // asm: 	LDPI	@PLYCAR,AR3
    // asm: 	LDF	*+AR4(OPOSX),R2
    // asm: 	SUBF	*+AR3(OPOSX),R2
    // asm: 	LDF	*+AR4(OPOSZ),R1
    // asm: 	SUBF	*+AR3(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R1,R2
    // asm: 	CALL	SQRT
#if DEBUG
    // asm: 	CMPI	0,DP
    // asm: 	BNE	$
#endif
    // asm: 	POPFL	R2
    // asm: 	POPFL	R1
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIST_TO_PLYR", 0, 0);
    UNIMPL();
}

void INIT_TRACKING_PIECE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GRAB THE AR2th PIECE ON THE CURRENT TRACK
    // *
    // *PARAMETERS
    // *	AR2	NUMBER OF PIECES IN TO START
    // *RETURNS
    // *	AR2	OBJECT POINTER TO STARTING ROAD PIECE
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDI	@DYNALIST_BEGIN,AR0
LPP:
    // asm: LDI	*+AR0(OLINK4),AR1
    // asm: 	LDI	AR1,AR0
    // asm: 	DBU	AR2,LPP
    // asm: 	LDI	AR0,AR2
#if DEBUG
    // asm: 	CMPI	0,AR2
    // asm: 	SLOCKON	EQ,"DRONES\INIT_TRACKING_PIECE .."
#endif
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_TRACKING_PIECE", 0, 0);
    UNIMPL();
}

void GET_TRACK_POS_RVS_XLANE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *TRACK APPROPRIATE PIECE
    // *
    // *GETS THE POSITION THAT WE ARE CURRENTLY TRACKING ON,
    // *IF THE POSITION IS TOO CLOSE THEN WE ADVANCE TO THE NEXT POSITION (W/WRAP
    // *AROUND)
    // *
    // *PARAMETERS
    // *	AR4			DRONE OBJECT
    // *	AR7			DRONE PROCESS
    // *	*+AR7(DELTA_TPIECE)	VALID ROAD OBJECT CURRENTLY TRACKING
    // *
    // *RETURNS
    // *	VECTORA		[X Y Z] POSITION
    // *	AR5		PTR TO TRACK ENTRY
    // *	R0		(FL) DISTANCE TO NEXT PIECE
    // *	MATRIXA		find Y Matrix (of Tracking Piece)
    // *
    // asm: 	PUSHFL	R1
    // asm: 	PUSHFL	R2
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	CALL	SUB_FUNCTION_RVS_XLANE		;GET LANE OFFSET (VECTOR A)
    // asm: 	BU	TRKP2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TRACK_POS_RVS_XLANE", 0, 0);
    UNIMPL();
}

void GET_TRACK_POS_RVS(void)
{
    // asm: 	PUSHFL	R1
    // asm: 	PUSHFL	R2
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	CALL	SUB_FUNCTION_RVS
    // asm: 	BU	TRKP2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TRACK_POS_RVS", 0, 0);
    UNIMPL();
}

void DELTA_GET_TRACK_POS(void)
{
    // asm: 	PUSHFL	R1
    // asm: 	PUSHFL	R2
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	CALL	DELTA_SUB_FUNCTION		;GET LANE OFFSET (VECTOR A)
    // asm: 	BU	TRKP2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELTA_GET_TRACK_POS", 0, 0);
    UNIMPL();
}

void GET_TRACK_POS(void)
{
    // asm: 	PUSHFL	R1
    // asm: 	PUSHFL	R2
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: TRACK_PIECE
    // asm: 	CALL	SUB_FUNCTION		;GET LANE OFFSET (VECTOR A)
TRKP2:
#if DEBUG
    // asm: 	LDI	*+AR2(OUSR1),R1
    // asm: 	SLOCKON	LT,"DRONES\GET_TRACK_POS  probably tracking a deleted object"
#endif
    // asm: 	LDF	*+AR2(OPOSX),R2		;X
    // asm: 	SUBF	*+AR4(OPOSX),R2
    // asm: 	LDF	*+AR2(OPOSZ),R1		;Z
    // asm: 	SUBF	*+AR4(OPOSZ),R1
    // asm: 	MPYF	R2,R2
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	CALL	SQRT
    // 	;R0 now has distance to next check
    // asm: DISTANCE_OK
    // asm: 	POP	AR2
    // asm: 	POPFL	R2
    // asm: 	POPFL	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TRACK_POS", 0, 0);
    UNIMPL();
}

void SUB_FUNCTION_RVS(void)
{
    // *----------------------------------------------------------------------------
    // ;*----------------------------------------------------------------------------
    // ;*
    // ;*PARAMETERS
    // ;*	AR7			DRONE PROCESS
    // ;*	AR4			DRONE OBJECT
    // ;*	*+AR7(DELTA_SPTR)	VALID STEALTH POINTER
    // ;*
    // ;*
    // ;*
    // ;*RETURNS
    // ;*	VECTORA		[X Y Z] POSITION
    // ;*	R0		(FL) DISTANCE TO NEXT PIECE
    // ;*	MATRIXA		find Y Matrix (of Tracking Piece)
    // ;*
    // ;GET_STEALTH_POS:
    // ;	PUSHFL	R1
    // ;	PUSHFL	R2
    // ;	PUSHFL	R3
    // ;	PUSH	AR2
    // ;	LDI	*+AR7(DELTA_SPTR),AR2
    // ;
    // ;	FLOAT	*+AR2(Y),R0
    // ;	FLOAT	52,R1
    // ;	SUBF	R1,R0
    // ;	STF	R0,*+AR4(OPOSY)
    // ;
    // ;;	yrot  = cr + ROAD_ITEMS[i].rads.y;
    // ;;	yrot *= 128;
    // ;;	yroti = (int) yrot;
    // ;;
    // ;;	ROAD_MAP[MAP_LENGTH].id = (ROAD_MAP[MAP_LENGTH].id & 0x0fffff) |
    // ;;				((yroti & 0x0fff) << 20);
    // ;
    // ;
    // ;	;to get the proper theta we must compute the direction to the next
    // ;	;piece via arctan2
    // ;	;
    // ;	LDI	*+AR2(4+X),R2
    // ;	LDI	*+AR2(4+Z),R3
    // ;	SUBI	*+AR2(X),R2
    // ;	SUBI	*+AR2(Z),R3
    // ;	FLOAT	R2
    // ;	FLOAT	R3
    // ;	CALL	ARCTANF
    // ;	SUBF	HALFPI,R0
    // ;	LDF	R0,R2
    // ;
    // ;
    // ;	LDI	@MATRIXAI,AR2
    // ;	CALL	FIND_YMATRIX
    // ;
    // ;	LDI	*+AR7(DELTA_SPTR),AR2
    // ;	FLOAT	*+AR2(X),R2
    // ;	SUBF	*+AR4(OPOSX),R2
    // ;	FLOAT	*+AR2(Z),R1
    // ;	SUBF	*+AR4(OPOSZ),R1
    // ;
    // ;	MPYF	R2,R2
    // ;	MPYF	R1,R1
    // ;	ADDF	R1,R2
    // ;	CALL	SQRT
    // ;
    // ;	POP	AR2
    // ;	POPFL	R3
    // ;	POPFL	R2
    // ;	POPFL	R1
    // ;	RETS
    // ;*----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GETS OFFSET OF LANE
    // *
    // *
    // *PARAMETERS
    // *	AR2	PIECE TO TRACK (ALLOCATED OBJECT)
    // *	AR4	DRONE OBJECT
    // *	AR7	DRONE PROCESS
    // *
    // *
    // *RETURNS
    // *	MATRIXA	ROTATED FOR THE LANE
    // *	VECTORA	LANE OFFSET
    // *	R2	RADIAN Y
    // *
    // *
    // *	if (obj -> OLINK4 == NULL)  {
    // *		dont know???
    // *	}
    // *	else  {
    // *
    // *
    // *	}
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSHFL	R0
    // asm: 	PUSHFL	R3
    // asm: 	LDI	*+AR2(OBLINK4),R0
    // asm: 	SLOCKON	Z,"DRONES\SUB_FUNCTION_RVS  OBLINK4 to NULL"
    // asm: 	BU	SF_ENTER2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SUB_FUNCTION_RVS", 0, 0);
    UNIMPL();
}

void SUB_FUNCTION(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSHFL	R0
    // asm: 	PUSHFL	R3
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	SLOCKON	Z,"DRONES\SUB_FUNCTION  OLINK4 to NULL"
SF_ENTER2:
    // asm: 	LDI	R0,AR0
    // asm: 	LDF	*+AR0(OPOSX),R2
    // asm: 	SUBF	*+AR2(OPOSX),R2
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	SUBF	*+AR2(OPOSZ),R3
    // asm: 	CALL	ARCTANF
    // asm: 	SUBF	HALFPI,R0
    // asm: 	LDF	R0,R2				;FIND THETA
    // asm: 	PUSHF	R2
    // asm: 	CALL	GET_LANES
    // asm: 	PUSH	AR2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX			;FIND Y MATRIX (FOR LANE OFFSETTING)
    // asm: 	LDI	*+AR7(DELTA_STATUS),AR0
    // asm: 	AND	DELTA_STATUS_LANE,AR0
    // ;	LDPI	@LANEPI,AR1
    // asm: 	LDI	@LANEPI,AR1
    // asm: 	ADDI	R0,AR1		;4 or 2 lane map?
    // asm: 	LDI	*AR1,R0
    // asm: 	ADDI	R0,AR0		;which lane?
    // asm: 	LDF	*AR0,R0
DELTA_JOININ:
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	CALL	MATRIX_MUL			;COMPUTE THE LANE OFFSET IN VECTORA
    // asm: 	POP	AR2
    // asm: 	POPF	R2
    // asm: 	POPFL	R3
    // asm: 	POPFL	R0
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SUB_FUNCTION", 0, 0);
    UNIMPL();
}

void SUB_FUNCTION_RVS_XLANE(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSHFL	R0
    // asm: 	PUSHFL	R3
    // asm: 	LDI	*+AR2(OBLINK4),R0
    // asm: 	BU	SFENTER66
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SUB_FUNCTION_RVS_XLANE", 0, 0);
    UNIMPL();
}

void SUB_FUNCTION_XLANE(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void DELTA_SUB_FUNCTION(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSHFL	R0
    // asm: 	PUSHFL	R3
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	SLOCKON	Z,"DRONES\DELTA_SUB_FUNCTION  OLINK4 to NULL"
SFENTER66:
    // asm: 	LDI	R0,AR0
    // asm: 	LDF	*+AR0(OPOSX),R2
    // asm: 	SUBF	*+AR2(OPOSX),R2
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	SUBF	*+AR2(OPOSZ),R3
    // asm: 	CALL	ARCTANF
    // asm: 	SUBF	HALFPI,R0
    // asm: 	LDF	R0,R2				;FIND THETA
    // asm: 	PUSHF	R2
    // asm: 	PUSH	AR2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX			;FIND Y MATRIX (FOR LANE OFFSETTING)
    // asm: 	LDF	*+AR7(DELTA_XLANE),R0
    // asm: 	BU	DELTA_JOININ
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELTA_SUB_FUNCTION", 0, 0);
    UNIMPL();
}

void INIT_DRONES(void)
{
    // *----------------------------------------------------------------------------
    // ;*----------------------------------------------------------------------------
    // ;*GIVEN A SECTION/ROAD ID FIND THE MAP ENTRY IN THE
    // ;*ROAD MAP WHICH CONTAINS THE ID EQUAL TO, OR GREATER
    // ;*THAN THE GIVEN ID.
    // ;*[finds GE road ID ptr]
    // ;*
    // ;*PARAMETERS
    // ;*	R2	CODED ID (from OUSR1)
    // ;*	AR7	DRONE PROCESS
    // ;*
    // ;*RETURNS
    // ;*	*+AR7(STEALTH_PTR)
    // ;*
    // ;FIND_MAPENTRY:
    // ;	PUSH	AR0
    // ;	PUSH	R0
    // ;	LDI	@ROAD_MAPI,AR0
    // ;FIND_LP
    // ;	LDI	*+AR0(3),R0
    // ;	LS	12,R0
    // ;	RS	12,R0
    // ;	CMPI	R2,R0
    // ;	BGE	FINDX
    // ;	ADDI	4,AR0
    // ;	BU	FIND_LP
    // ;FINDX
    // ;	STI	AR0,*+AR7(DELTA_SPTR)
    // ;	STI	R0,*+AR7(DELTA_LAST_OID)	;save road id
    // ;
    // ;	POP	R0
    // ;	POP	AR0
    // ;	RETS
    // ;*----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DRONE UTILITY FUNCTIONS
    // *
    // asm: 	PUSH	R0
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@CAR_LIST
    // asm: 	STI	R0,@DRONE_COUNT
    // asm: 	LDI	SM_GO,R0
    // asm: 	STI	R0,@SUSPEND_MODE
    // asm: 	LDI	40,R0
    // asm: 	STI	R0,@DD_SLP
    // asm: 	LDI	100,R0
    // asm: 	STI	R0,@DD_VAR
    // asm: 	LDI	MAX_DRONES,R0
    // asm: 	STI	R0,@DD_MAX_DRONES
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_DRONES", 0, 0);
    UNIMPL();
}

void ADD_DRONE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *ADD TO DRONE SUPP LIST
    // *
    // *PARAMETERS
    // *	AR4	OBJECT DRONE POINTER
    // *
    // asm: 	PUSH	R0
    // asm: 	LDI	@CAR_LIST,R0
    // asm: 	STI	R0,*+AR4(OLINK3)
    // asm: 	STI	AR4,@CAR_LIST
    // asm: 	INCM	@DRONE_COUNT
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADD_DRONE", 0, 0);
    UNIMPL();
}

void FREE_DRONE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *FREE_DRONE
    // *unlink DRONE from DRONE supplimentary list
    // *
    // *PARAMETERS
    // *	AR4	OBJECT DRONE POINTER
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR3
    // asm: 	LDI	@CAR_LISTI,R0
    // asm: 	SUBI	OLINK3,R0		;(we are offset pointing)
FREELP:
    // asm: LDI	R0,AR1
    // asm: 	LDI	*+AR1(OLINK3),R0
    // asm: 	SLOCKON	Z,"DRONES\FREE_DRONE  end of list found"
    // asm: 	BZ	FREEDR_X
    // asm: 	CMPI	R0,AR4
    // asm: 	BNE	FREELP
    // asm: 	LDI	*+AR4(OLINK3),R0
    // asm: 	STI	R0,*+AR1(OLINK3)	;LINK AROUND
    // asm: 	DECM	@DRONE_COUNT
#if DEBUG
    // asm: 	CMPI	0,R0
    // asm: 	SLOCKON	LT,"DRONES\FREE_DRONE  DRONE_COUNT INVALID"
#endif
FREEDR_X:
    // asm: 	POP	AR3
    // asm: 	POP	AR1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREE_DRONE", 0, 0);
    UNIMPL();
}

void EXP_PUFF(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // asm: 	LDL	40000,R0
    // asm: 	CMPI	*+AR4(ODIST),R0
    // asm: 	BLT	SUICIDE
    // asm: 	LDI	AR4,AR6
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	SUICIDE
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	@EXP_ANII,AR5
    // asm: 	LDI	*AR5,AR0
    // asm: 	STI	AR0,*+AR4(OROMDATA)
    // asm: 	LDF	*+AR6(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR6(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR6(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	ORM	O_POSTER,*+AR4(OFLAGS)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDF	6,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R6
    // asm: 	LDF	6,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R7
EXP_PUFFLP:
    // asm: 	LDL	40000,R0
    // asm: 	CMPI	*+AR4(ODIST),R0
    // asm: 	BLT	EXP_DIE
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	R7,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	*AR5++,AR0
    // asm: 	CMPI	-1,AR0
    // asm: 	BEQ	EXP_DIE
    // asm: 	STI	AR0,*+AR4(OROMDATA)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	10,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	SLEEP	1
    // asm: 	BU	EXP_PUFFLP
EXP_DIE:
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EXP_PUFF", 0, 0);
    UNIMPL();
}

void PRECOLLIDE_PLYR(void)
{
    // *----------------------------------------------------------------------------
    // ;*----------------------------------------------------------------------------
    // ;*MK_CHUNK
    // ;*
    // ;*PARAMETERS
    // ;*	AR4	HEAD OBJECT
    // ;*	AR2	ROM DATA PTR
    // ;*
    // ;*RETURNS
    // ;*	C	CANNOT GET OBJECT
    // ;*	AR0	CHUNK
    // ;*
    // ;*
    // ;	.bss	XZSPC,1
    // ;	.bss	XZMIN,1
    // ;	.bss	YSPC,1
    // ;	.bss	YMIN,1
    // ;MK_CHUNK:
    // ;	PUSH	AR2
    // ;	PUSH	R0
    // ;	PUSH	R1
    // ;	PUSH	R2
    // ;	PUSHF	R0
    // ;	PUSHF	R1
    // ;	PUSHF	R2
    // ;
    // ;
    // ;	CALL	OBJ_GET
    // ;	BC	MK_CHUNKX
    // ;
    // ;	MOVMF	*+AR4(OPOSX),*+AR0(OPOSX)
    // ;	MOVMF	*+AR4(OPOSY),*+AR0(OPOSY)
    // ;	MOVMF	*+AR4(OPOSZ),*+AR0(OPOSZ)
    // ;	STI	AR2,*+AR0(OROMDATA)
    // ;
    // ;	LDI	@XZSPC,AR2
    // ;	CALL	SRAND
    // ;	FLOAT	R0
    // ;	FLOATP	@XZMIN,R1
    // ;	SADDFR	R1,R0
    // ;	STF	R0,*+AR0(OVELX)
    // ;
    // ;	LDI	@XZSPC,AR2
    // ;	CALL	SRAND
    // ;	FLOAT	R0
    // ;	FLOAT	@XZMIN,R1
    // ;	SADDFR	R1,R0
    // ;	STF	R0,*+AR0(OVELZ)
    // ;
    // ;
    // ;	LDI	@YSPC,AR2
    // ;	CALL	RANDU0
    // ;	ADDI	@YMIN,R0
    // ;	FLOAT	R0
    // ;	NEGF	R0
    // ;	STF	R0,*+AR0(OVELY)
    // ;	LDI	AR0,AR2
    // ;	CALL	OBJ_INSERT
    // ;
    // ;
    // ;	LDI	10,AR2
    // ;	CALL	SRAND
    // ;	FLOAT	R0
    // ;	MPYF	0.01,R0
    // ;	STF	R0,*+AR0(OUSR2)
    // ;
    // ;	LDI	10,AR2
    // ;	CALL	SRAND
    // ;	FLOAT	R0
    // ;	MPYF	0.03,R0
    // ;	STF	R0,*+AR0(OUSR3)
    // ;
    // ;	CLRC
    // ;MK_CHUNKX
    // ;	POPF	R2
    // ;	POPF	R1
    // ;	POPF	R0
    // ;	POP	R2
    // ;	POP	R1
    // ;	POP	R0
    // ;	POP	AR2
    // ;	RETS
    // ;*----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PRECOLLIDE_PLYR	CHECK TO SEE IF CAR WILL COLLIDE WITH PLAYER
    // *
    // *
    // *check distance to player
    // *if within that distance
    // *
    // *examine path, is player in immediate player
    // *
    // *
    // *PARAMETERS
    // *	AR4	OBJECT
    // *	AR5	CARBLOCK
    // *	AR7	DRONE PROCESS
    // *
    // *RETURNS
    // *	CARRY SET ON COLLISION IMMINENT
    // *	CARRY CLR PATH MAINLY CLEAR
    // *
    // asm: 	CALL	DIST_TO_PLYR
    // asm: 	FLOAT	15000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	NOT_IMMINENT
    // 	;
    // 	;project ahead 5 frames
    // 	;
    // asm: 	LDI	@VECTORAI,AR3
    // asm: 	LDF	*+AR5(CARSPEED),R0
    // asm: 	MPYF	@NFRAMES,R0
    // asm: 	MPYF	5,R0
    // asm: 	STF	R0,*+AR3(Z)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR3(X)
    // asm: 	STF	R0,*+AR3(Y)
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR2,R2
    // asm: 	LDI	AR3,AR2
    // asm: 	LDI	AR3,R3
    // asm: 	CALL	MATRIX_MUL
    // 	;generate the line equation
    // 	;
    // asm: 	LDP	@_VECTORA
    // asm: 	NEGF	@_VECTORA+Z,R6		;A = Uy - Vy
    // asm: 	LDF	@_VECTORA+X,R7		;B = Vx - Ux
    // asm: 	SETDP
    // asm: 	LDF	*+AR4(OPOSX),R5
    // asm: 	MPYF	R6,R5
    // asm: 	LDF	*+AR4(OPOSZ),R3
    // asm: 	MPYF	R7,R3
    // asm: 	ADDF	R3,R5
    // asm: 	NEGF	R5			;C = -(A Ux + B Uy)
    // ;	LDPI	@CAR_LISTI,AR1
    // 	;now find the distance of a point to a line
    // 	;
    // asm: 	MPYF	R6,R6,R2
    // asm: 	MPYF	R7,R7,R3
    // asm: 	ADDF	R3,R2
    // asm: 	CALL	SQRT
    // asm: 	LDF	R0,R1
    // asm: 	LDPI	@PLYCAR,AR2
    // asm: 	LDF	*+AR2(OPOSX),R2
    // asm: 	MPYF	R6,R2
    // asm: 	LDF	*+AR2(OPOSZ),R3
    // asm: 	MPYF	R7,R3
    // asm: 	ADDF	R3,R2
    // asm: 	ADDF	R5,R2
    // asm: 	LDF	R2,R0
    // asm: 	CALL	DIV_F
    // asm: 	ABSF	R0
    // asm: 	FLOAT	400,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	NOT_IMMINENT
    // asm: IS_IMMINENT
    // asm: 	SETC
    // asm: 	RETS
NOT_IMMINENT:
    // asm: 	CLRC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRECOLLIDE_PLYR", 0, 0);
    UNIMPL();
}

void DRONE_RIDE_RIGHT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PLYR_RIDE_RIGHT	is the player riding on the right side of the road?
    // *
    // *RETURNS
    // *	CARRY CLR	ON RIGHT SIDE OF ROAD
    // *	CARRY SET	NOT ON RIGHT SIDE OF ROAD
    // *	R0		DISTANCE TO ROAD CENTER
    // *
    // *Algorythm
    // *
    // *	generate line equation by the closest track piece and the next piece
    // *	check the sign of the distance to this line (plyrs position)
    // *	and return which side of the road
    // *
    // *ANY ERRORS WILL ASSUME PLYR IS ON RITE SIDE
    // *
    // *DRONE_RIDE_RIGHT
    // *
    // *
    // *PARAMETERS
    // *	AR4	DRONE OBJ
    // *	AR5	DRONE CAR BLOCK
    // *RETURNS
    // *	R0	(FL) DISTANCE TO CENTERLINE OF ROAD
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSHFL	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	BU	RIDE_RIGHT_JOININ
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_RIDE_RIGHT", 0, 0);
    UNIMPL();
}

void PLYR_RIDE_RIGHT(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSHFL	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	LDPI	@PLYCAR,AR4
    // asm: 	CMPI	0,AR4
    // asm: 	BEQ	PRR_ONRITE
    // asm: 	LDI	*+AR4(OCARBLK),AR5
RIDE_RIGHT_JOININ:
    // asm: 	LDI	*+AR5(CARTRAK),AR2
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	BZ	PRR_ONRITE
    // asm: 	LDI	R0,AR0
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	SUBF	*+AR0(OPOSZ),R0		;A = Uy - Vy
    // asm: 	LDF	*+AR0(OPOSX),R1
    // asm: 	SUBF	*+AR2(OPOSX),R1		;B = Vx - Ux
    // asm: 	NEGF	R0,R2
    // asm: 	NEGF	R1,R3
    // asm: 	MPYF	*+AR2(OPOSX),R2
    // asm: 	MPYF	*+AR2(OPOSZ),R3
    // asm: 	ADDF	R3,R2			;C = -(A Ux + B Uy)
    // ;	NEGF	R2
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R2
    // asm: 	MPYF	R0,R0,R0
    // asm: 	MPYF	R1,R1,R2
    // asm: 	ADDF	R0,R2
    // asm: 	CALL	SQRT
    // asm: 	LDF	R0,R3
    // asm: 	POPF	R2
    // asm: 	POPF	R0
    // asm: 	MPYF	*+AR4(OPOSX),R0
    // asm: 	MPYF	*+AR4(OPOSZ),R1
    // asm: 	ADDF	R1,R0
    // asm: 	ADDF	R2,R0
    // asm: 	LDF	R3,R1
    // asm: 	CALL	DIV_F
    // asm: 	LDF	R0,R0			;dist = (A Ux + B Uy + C)/sqrt(A^2 + B^2)
    // asm: 	BLT	PRR_ONRITE
    // asm: PRR_NONRITE
    // asm: 	CLRC
    // asm: 	BU	PRR_X
PRR_ONRITE:
    // asm: 	SETC
PRR_X:
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R3
    // asm: 	POPFL	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_RIDE_RIGHT", 0, 0);
    UNIMPL();
}

void SMOKE_PUFF(void)
{
    // asm: 	LDI	@SMOKE_COUNT,R0
    // asm: 	CMPI	5,R0
    // asm: 	BGE	SUICIDE
    // asm: 	INC	R0
    // asm: 	STI	R0,@SMOKE_COUNT
    // asm: 	LDL	40000,R0
    // asm: 	CMPI	*+AR4(ODIST),R0
    // asm: 	BLT	SUICIDE
    // asm: 	LDI	AR4,AR6
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	SUICIDE
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	@SMOKE_ANII,AR5
    // asm: 	LDI	*AR5,AR0
    // asm: 	STI	AR0,*+AR4(OROMDATA)
    // asm: 	LDI	UTIL_C|SMOKE_T,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDL	bnout1_smoke,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR4(OPAL)
    // asm: 	LDF	*+AR6(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR6(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR6(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	ORM	O_POSTER|O_1PAL,*+AR4(OFLAGS)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
SMOKE_PUFFLP:
    // asm: 	LDL	40000,R0
    // asm: 	CMPI	*+AR4(ODIST),R0
    // asm: 	BLT	SMOKE_DIE
    // asm: 	LDF	6,R0
    // asm: 	CALL	SFRAND
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	6,R0
    // asm: 	CALL	SFRAND
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	*AR5++,AR0
    // asm: 	CMPI	-1,AR0
    // asm: 	BEQ	SMOKE_DIE
    // asm: 	STI	AR0,*+AR4(OROMDATA)
    // asm: 	LDI	*AR5++,AR2
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	*AR5++,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CALL	SLEEP
    // asm: 	BU	SMOKE_PUFFLP
SMOKE_DIE:
    // asm: 	LDI	@SMOKE_COUNT,R0
    // asm: 	DEC	R0
#if DEBUG
    // asm: 	BLT	$
#endif
    // asm: 	STI	R0,@SMOKE_COUNT
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOKE_PUFF", 0, 0);
    UNIMPL();
}

void DROP_COCONUTS(void)
{
    // asm: 	LDI	@COCONUT_COUNT,R0
    // asm: 	CMPI	5,R0
    // asm: 	BGE	SUICIDE
    // asm: 	INC	R0
    // asm: 	STI	R0,@COCONUT_COUNT
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	SUICIDE
    // asm: 	LDI	AR0,AR4
    // asm: 	LDF	*+AR5(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR5(OPOSY),R0
    // asm: 	FLOAT	1500,R1
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR5(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDL	coco1,R0
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_POSTER,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	NSND1
    // asm: 	SONDFX	COCONUTLOW
NSND1:
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR7(PDATA+1)	;save initial base Y
    // asm: 	CALL	OBJSCAN
    // asm: 	STF	R0,*+AR7(PDATA)
    // asm: 	RANDN	20
    // asm: 	MPYI	3,R0
    // asm: 	FLOAT	R0
    // asm: 	ADDF	20,R0
    // asm: 	STF	R0,*+AR4(OVELY)
    // asm: 	RANDN	20
    // asm: 	MPYI	3,R0
    // asm: 	FLOAT	R0
    // asm: 	SUBF	30,R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	RANDN	20
    // asm: 	MPYI	3,R0
    // asm: 	FLOAT	R0
    // asm: 	SUBF	30,R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR7(PDATA+2)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	STF	R0,*+AR4(OVELZ)
DROPLP:
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	BN	DROPCOCOKILL
    // asm: 	CMPI	20000,R0
    // asm: 	BGT	DROPCOCOKILL
    // asm: 	LDF	*+AR4(OVELY),R0
    // asm: 	ADDF	25,R0
    // asm: 	FLOAT	450,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFGT	R1,R0
    // asm: 	STF	R0,*+AR4(OVELY)
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR4(OVELX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR4(OVELZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR4(OVELY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CMPF	*+AR5(OPOSY),R0
    // asm: 	BLT	FRSL
    // asm: 	LDI	*+AR7(PDATA+2),R0
    // asm: 	BNZ	NOTINITIAL
    // asm: 	RANDN	30
    // asm: 	MPYI	3,R0
    // asm: 	FLOAT	R0
    // asm: 	SUBF	45,R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	RANDN	30
    // asm: 	MPYI	3,R0
    // asm: 	FLOAT	R0
    // asm: 	SUBF	45,R0
    // asm: 	STF	R0,*+AR4(OVELZ)
    // asm: 	BU	LLKK
NOTINITIAL:
    // asm: 	CMPI	3,R0
    // asm: 	BEQ	HOLDTOSLEEP
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	LDF	*+AR4(OVELZ),R0
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR4(OVELZ)
LLKK:
    // asm: 	LDI	*+AR7(PDATA+2),R0
    // asm: 	INC	R0
    // asm: 	STI	R0,*+AR7(PDATA+2)
    // asm: 	LDF	*+AR4(OVELY),R0
    // asm: 	CMPF	3,R0
    // asm: 	BLE	HOLDTOSLEEP
    // asm: 	MPYF	-0.5,R0
    // asm: 	STF	R0,*+AR4(OVELY)
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	NSND2
    // asm: 	RANDN	1
    // asm: 	LDI	R0,R0
    // asm: 	LDIZ	COCONUTHI,AR2
    // asm: 	LDINZ	COCONUTLOW,AR2
    // asm: 	CALL	ONESNDFX
NSND2:
FRSL:
    // asm: 	SLEEP	1
    // asm: 	BU	DROPLP
HOLDTOSLEEP:
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	BN	DROPCOCOKILL
    // asm: 	SLEEP	1
    // asm: 	B	HOLDTOSLEEP
DROPCOCOKILL:
    // asm: 	LDI	@COCONUT_COUNT,R0
    // asm: 	DEC	R0
#if DEBUG
    // asm: 	BLT	$
#endif
    // asm: 	STI	R0,@COCONUT_COUNT
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROP_COCONUTS", 0, 0);
    UNIMPL();
}

void AHEAD_OF_PLAYER_P(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *AM I AHEAD OF THE PLAYER?
    // *
    // *PARAMETERS
    // *	AR4	OBJECT
    // *	AR5	CAR BLOCK
    // *
    // *RETURNS
    // *	CARRY SET ON AHEAD OF PLAYER
    // *	CARRY CLR ON BEHIND PLAYER
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	R0
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDI	*+AR0(CARTRAK),AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	LDI	*+AR5(CARTRAK),AR1
    // asm: 	CMPI	*+AR1(OUSR1),R0
    // asm: 	BLT	AHEADOF
    // asm: 	BGT	ISBEHIND
    // asm: 	LDI	*+AR0(OLINK4),AR0	;GET NEXT OBJECT
    // asm: 	LDI	*+AR0(OLINK4),AR0	;GET NEXT OBJECT
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	SUBF	*+AR4(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR0(OPOSZ),R1
    // asm: 	SUBF	*+AR4(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R0
    // asm: 	LDI	@(_plyr1+PLY_CAR),AR1
    // asm: 	LDF	*+AR0(OPOSX),R2
    // asm: 	SUBF	*+AR1(OPOSX),R2
    // asm: 	MPYF	R2,R2
    // asm: 	LDF	*+AR0(OPOSZ),R1
    // asm: 	SUBF	*+AR1(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R2,R1
    // asm: 	CMPF	R1,R0	;WHO IS CLOSER?
    // asm: 	BLT	AHEADOF
ISBEHIND:
    // asm: 	CLRC
    // asm: 	POP	R0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
AHEADOF:
    // asm: 	SETC
    // asm: 	POP	R0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AHEAD_OF_PLAYER_P", 0, 0);
    UNIMPL();
}

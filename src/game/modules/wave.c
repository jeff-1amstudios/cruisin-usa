#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/obj.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/pall.h"
#include "../include/sndtab.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/dirq.h"
#include "../include/comm.h"
#include "../include/h2hobj.h"
#include "../include/wave.h"

/*
 * Source module: asm/WAVE.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *----------------------------------------------------------------------------
// *PARAMETERS
// *	AR2	WAVE INDEX
// *		<  0 -> ATTRACT MODE N
// *		>= 0 -> REGULAR GAME
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *JEEP DYNAMIC OBJECT SETUP
// *
// *
// *CVETT DYNAMIC OBJECT SETUP
// *
// *
// *GTRUCKP DYNAMIC OBJECT SETUP
// *
// *
// *MISSILE DYNAMIC OBJECT SETUP
// *
// *
// *PLAYERS SCHOOL BUS
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *STRUCT VEHTAB
// *	.word	model,model_palette,animation,degraded_model,degraded_model_level2
// *	.word	taillight_color_count
// *	.float	mass
// *	.word	passby,backnoise
// *	.word	pallete_table
// *	.word	drone_dyna_table
// *
// *
/* asm: VEHICLE_TABLEI	.word	VEHICLE_TABLE */
int VEHICLE_TABLEI = (int)(VEHICLE_TABLE);
// *----------------------------------------------------------------------------
// *PALETTE_TABLES :
// *
// *TABLE_NAME
// *	.word	length
// *	{
// *	.word	pal_ptr
// *	} * length
// *
// ;TESTORPAL	.word	3,testor_blue,testor_grape,testor_gold
/* asm: TESTORPAL	.word	2,testor_blue,testor_grape */
int TESTORPAL[] = { 2, testor_blue, testor_grape };
/* asm: BULLETPAL	.word	3,missle_yellow,missle_red,missle_blue */
int BULLETPAL[] = { 3, missle_yellow, missle_red, missle_blue };
// ;HOTRODPAL	.word	3,hotrod_yellow,hotrod_brtblue,hotrod_maroon
/* asm: HOTRODPAL	.word	2,hotrod_yellow,hotrod_brtblue */
int HOTRODPAL[] = { 2, hotrod_yellow, hotrod_brtblue };
/* asm: CVETTEPAL	.word	3,cvette_blue,cvette_red,cvette_purple */
int CVETTEPAL[] = { 3, cvette_blue, cvette_red, cvette_purple };
/* asm: JEEPPAL	.word	2,jeep_red,jeep_yellow */
int JEEPPAL[] = { 2, jeep_red, jeep_yellow };
// ;GTRUCKPAL	.word	2,gtruck_yellow,gtruck_bluep
/* asm: GTRUCKPAL	.word	1,gtruck_bluep */
int GTRUCKPAL[] = { 1, gtruck_bluep };
/* asm: CARAVANPAL	.word	2,caravan_redyelo,caravan_yelogrey */
int CARAVANPAL[] = { 2, caravan_redyelo, caravan_yelogrey };
/* asm: MUSCLEPAL	.word	2,muscle_yellow,muscle_green */
int MUSCLEPAL[] = { 2, muscle_yellow, muscle_green };
/* asm: PTRUCKGPAL	.word	2,ptruckg_purple,ptruckg_yellr */
int PTRUCKGPAL[] = { 2, ptruckg_purple, ptruckg_yellr };
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------

void WAVE(void)
{
    // asm: 	POP	AR7	;return addr
    // asm: 	CALL	SILENT
    // 	;CLEAR ALL RAM AND RELOAD CODE
    // 	;
    // asm: 	CLRI	R0			;CLEAR INTERNAL RAM
    // asm: 	LDIL	RAM0,AR0
    // asm: 	RPTS	2047
    // asm: 	STI	R0,*AR0++
    // asm: 	CALL	COMM_ENABLE_INT2
    // asm: 	LDI	@COUNTER_IDX,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	@COUNTER_MODE,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	@COIN_COUNTER1,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	@COIN_COUNTER2,R0
    // asm: 	PUSH	R0
    // asm: 	CALL	CLR_RAM			;CLEAR BSS SPACE
    // asm: 	POP	R0
    // asm: 	STI	R0,@COIN_COUNTER2
    // asm: 	POP	R0
    // asm: 	STI	R0,@COIN_COUNTER1
    // asm: 	POP	R0
    // asm: 	STI	R0,@COUNTER_MODE
    // asm: 	POP	R0
    // asm: 	STI	R0,@COUNTER_IDX
    // 	;RELOAD GAME CODE
    // asm: 	LDI	0,AR1			;SOURCE ADDRESS
    // asm: 	LDI	4000h,AR3		;DESINATION ADDRESS
    // asm: 	LS	8,AR3
    // asm: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm: 	LS	4,RC			;FAST RAM
    // asm: 	RPTB	LD_RAM
    // asm: 	LDI	*AR1++,R0
    // asm: LD_RAM	STI	R0,*AR3++
    // asm: 	PUSH	AR7
    // asm: 	PUSH	AR2
#if DEBUG
    // asm: 	CALL	VERIFY_CODE_INTEGRITY
#endif
    // asm: 	CALL	CLRONE	;CAN NOW BE DUAL PLAYER
    // asm: 	CALL	INIT_SYSTEM
    // asm: 	LDP	@_CAMERARAD
    // asm: 	CLRF	R0
    // asm: 	STF	R0,@_CAMERARAD+X
    // asm: 	STF	R0,@_CAMERARAD+Y
    // asm: 	STF	R0,@_CAMERARAD+Z
    // asm: 	STF	R0,@_CAMERAPOS+X
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	STF	R0,@_CAMERAPOS+Z
    // asm: 	STF	R0,@_LIGHT+Z
    // asm: 	LDF	0.707,R0
    // asm: 	STF	R0,@_LIGHT+X
    // asm: 	STF	R0,@_LIGHT+Y
    // asm: 	SETDP
    // asm: 	CALL	INIT_CUSTOM_COIN	;Set the CUSTOM SETUP in RAM
    // asm: 	LDI	@CAMERAMATRIXI,AR0
    // asm: 	CALL	INITMAT
    // asm: 	FLOAT	35,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CLEARRDY
    // asm: 	READADJ	ADJ_STEERCENTER
    // asm: 	FLOAT	R0
    // asm: 	STF	R0,@WHEELPOS
    // asm: 	CLRF	R0
    // asm: 	STF	R0,@WHEELPWR
    // asm: 	STF	R0,@WHEELVEL
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@COINOFF
    // asm: 	STI	R0,@NOLONG_VEHICLES
    // asm: 	CALL	LOAD_FIXED_PALETTES
    // asm: 	CREATE	SCAN_OBJECTS,UTIL_C
    // asm: 	POP	AR2
    // asm: 	CMPI	1,AR2
    // asm: 	BEQ	BEGIN_GAME
    // asm: 	CLRI	AR0
    // asm: 	LDP	@SWITCH3
    // asm: 	NOT	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // asm: 	LDI	*AR0,R2			;Loff
    // asm: 	SETDP
    // asm: 	AND	SW_VIEW0_H|SW_VIEW1_H|SW_VIEW2_H,R0
    // asm: 	CMPI	SW_VIEW1_H|SW_VIEW2_H,R0
    // asm: 	BEQ	CREDITS
    // asm: 	LDI	AR2,AR0
    // asm: 	ADDI	@_ATTR_WAVETABI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	RETS
    // asm: _ATTR_WAVETAB
HEAD2HEADWATCH:
    // asm: 	SLEEP	1
    // asm: 	LDI	@OM_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BEQ	ISTRUE
    // asm: 	CMPI	MINIT,R0
    // asm: 	BEQ	ISTRUE
    // asm: 	CMPI	MINSERT_COINS,R0
    // asm: 	BEQ	ISTRUE
    // asm: 	LDI	@OM_LINKWAIT,R0
    // asm: 	BZ	HEAD2HEADWATCH
ISTRUE:
    // asm: LDI	-7,R0
    // asm: 	STI	R0,@_ATTR_MODE
    // asm: 	BR	SET_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAVE", 0, 0);
    UNIMPL();
}

void HEAD2HEAD_WAIT(void)
{
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	ANDN	BUT_VIEWS,R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	CREATE	HEAD2HEAD_LOGO_WAIT,UTIL_C
    // asm: 	RETS
HIGH_SCORE:
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	ANDN	BUT_VIEWS,R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	LDI	-1,R0	;Kick start the light routine
    // asm: 	STI	R0,@OLD_BUTTON_STATUS
    // asm: 	LDI	MATTR|MHS,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CREATE	DISPLAY_HIGH_SCORES,UTIL_C
    // asm: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEAD_WAIT", 0, 0);
    UNIMPL();
}

void MIDSPIN(void)
{
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	ANDN	BUT_VIEWS,R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	CLRI	R0
    // asm: 	LDI	MATTR,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CALL	LOAD_ATTR_LEG
    // asm: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm: 	LDI	@ATTRWAVE,AR6		;DCS LOGOSOUND will play only if 0
    // ;	LDI	3,AR2
    // ;	CALL	RANDU0
    // ;	CMPI	0,R0
    // ;	BEQ	BBDD
    // ;
    // ;	CREATE	HEAD2HEAD_LOGO,UTIL_C
    // ;	RETS
    // ;BBDD
    // asm: 	CREATE	_MIDWAYSPIN,UTIL_C
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MIDSPIN", 0, 0);
    UNIMPL();
}

void MIDSPINHS(void)
{
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	ANDN	BUT_VIEWS,R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	LDL	_SECpress,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	MATTR,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CREATE	SPIN_CAR,UTIL_C
    // asm: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MIDSPINHS", 0, 0);
    UNIMPL();
}

void RACELEG(void)
{
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@TEASE_COUNT
    // asm: 	LDI	MATTR,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CREATE	ATTRACT_DELTA,DRONE_C|DELTA_ATTR_S
    // asm: 	LDI	@ATTRWAVE,R0
    // asm: 	CMPI	4,R0
    // asm: 	LDIEQ	25*30,R0
    // asm: 	LDINE	30*30,R0
    // asm: 	STI	R0,@_timer
    // asm: 	CREATE	_timeout,UTIL_C
    // asm: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RACELEG", 0, 0);
    UNIMPL();
}

void HIGH_SCORE_INI(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void THANKS(void)
{
    // asm: 	CALL	LOAD_HIGH_SCORE
    // asm: 	BU	HIGH_SCORE
    // 	;these are not cycled, they are special routines
CREDITS:
    // asm: 	CREATE	VANITY,UTIL_C
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@_ATTR_MODE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THANKS", 0, 0);
    UNIMPL();
}

void LOAD_HIGH_SCORE(void)
{
    // asm: 	CALL	FIFO_RESET
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	LDL	_SECpress,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@_ATTR_MODE
    // asm: 	RETS
    // *
    // *NOTE	ANY GAME STARTUP CODE & PROC INITS ARE CALLED/CREATED IN INTRO.ASM
    // *
BEGIN_GAME:
    // asm: 	CALL	SND_RESET_QUIET
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@LOADED
    // asm: 	CREATE	PLYR_INTRO,PLYR_C|PLYR1_T	;PLAYERS PROCESS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_HIGH_SCORE", 0, 0);
    UNIMPL();
}

void INIT_SYSTEM(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	PRC_INIT	;initialize process system
    // asm: 	CALL	OBJ_INIT	;initialize object system
    // asm: 	CALL	TEXT_INIT	;initialize text system
    // asm: 	CALL	INIT_DRONES	;initialize DRONE tracker system
    // asm: 	CALL	DYNAOBJ_INIT	;initialize DYNAMIC OBJECTS
    // asm: 	CALL	CARB_INIT	;initialize CAR BLOCKS
    // asm: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_sectime
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_SYSTEM", 0, 0);
    UNIMPL();
}

void FIXEDPAL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	RGB	0,0,0
    // asm: 	RGB	255,255,255		;1  WHITE
    // asm: 	RGB	070h,070h,070h		;2  GREY
    // asm: 	RGB	255,0,0			;3  RED
    // asm: 	RGB	0,255,0			;4  GREEN
    // asm: 	RGB	055h,055h,99h		;5  BLUE  (for the sky paled out)
    // asm: 	RGB	255,255,0		;6  YELLOW
    // asm: 	RGB	0,255,255		;7  CYAN
    // asm: 	RGB	0,0,0			;8  BLACK
    // asm: 	RGB	0,0,255			;9  TRUE BLUE
    // asm: 	RGB	0C0h,0A0h,0		;A  Amber color
    // asm: 	RGB	050h,050h,050h		;B	DARK GREY
    // asm: 	RGB	0A0h,0A0h,0A0h		;C	LIGHT GREY
    // asm: 	RGB	030h,030h,030h		;D	VERY DARK GREY
    // 					;F  -cycling color
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIXEDPAL", 0, 0);
    UNIMPL();
}

void ILLUM_PAL(void)
{
    // asm: 	RGB	06Ah,000h,000h
    // asm: 	RGB	06Fh,000h,000h
    // asm: 	RGB	074h,000h,000h
    // asm: 	RGB	079h,000h,000h
    // asm: 	RGB	07Eh,000h,000h
    // asm: 	RGB	083h,000h,000h
    // asm: 	RGB	088h,000h,000h
    // asm: 	RGB	08Dh,000h,000h
    // asm: 	RGB	092h,000h,000h
    // asm: 	RGB	097h,000h,000h
    // asm: 	RGB	09Ch,000h,000h
    // asm: 	RGB	0A1h,002h,005h
    // asm: 	RGB	0A6h,007h,00Ah
    // asm: 	RGB	0ABh,00Ch,00Fh
    // asm: 	RGB	0B0h,011h,014h
    // asm: 	RGB	0B5h,016h,019h
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ILLUM_PAL", 0, 0);
    UNIMPL();
}

void LOAD_FIXED_PALETTES(void)
{
    // *----------------------------------------------------------------------------
    // *THESE PALETTES ONLY HAVE TO BE LOADED UPON STARTUP
    // *
    // asm: 	CALL	PAL_INIT
    // asm: 	LDL	FIXEDPAL,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	CLRI	R0
    // asm: 	LDP	@COLORAM
    // asm: 	STI	R0,@COLORAM+0FFh
    // asm: 	SETDP
    // asm: 	LDL	fixedfnt_tPAL,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	ILLUM_PAL,AR2		;only 1 palette
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	lgnum43_coolyelo,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	dnums_amber,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	CALL	LOAD_VARIOUS_PALETTES	;mixed palettes
    // asm: 	LDL	shared_PALETTES,AR2
    // asm: 	CALL	HARDalloc_section
    // asm: 	LDI	sky1_p,AR2
    // asm: 	CALL	PAL_ALLOC
    // asm: 	LDL	osg10fnt_white,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	bnout1_smoke,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	H2HPAL1,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	H2HPAL2,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	H2HPAL3,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	general_PALETTES,AR2
    // asm: 	CALL	HARDalloc_section
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_FIXED_PALETTES", 0, 0);
    UNIMPL();
}

void LOAD_STARTUP_PALS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	ggate_PALETTES,AR2
    // asm: 	CALL	HARDalloc_section
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_STARTUP_PALS", 0, 0);
    UNIMPL();
}

void VEHICLE_TABLE(void)
{
    // 	;#0
    // 	;#1
    // 	;#2
    // 	;#3
    // 	;#4
    // 	;#5
    // 	;#6	//locked
    // 	;#7
    // 	;#8
    // 	;#9
    // 	;#10	//locked
    // 	;#11	//locked
    // 	;#12
    // 	;#13
    // 	;#14
    // 	;#15 copcar for the player
    // 	;#16 gtruckp for the player
    // 	;#17 sbusp for the player
    // *	number of axels,number of vertices
    // *	{
    // *	vertices, xcenter,ycenter,zcenter
    // *	} * number of axels
    // *
    // asm: DDYNA_GTRUCK
    // asm: DDYNA_FTRUCK
    // asm: DDYNA_CBUS
    // ;	.word	3,120
    // asm: DDYNA_COPCAR
    // asm: DDYNA_MUSCLE
    // asm: DDYNA_CARAVAN
    // asm: DDYNA_SBUS
    // asm: DDYNA_PTRUCKG
    // asm: DDYNA_MUSTANG
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VEHICLE_TABLE", 0, 0);
    UNIMPL();
}

void LOAD_VARIOUS_PALETTES(void)
{
    // asm: 	LDL	caravan_redyelo,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	caravan_yelogrey,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	hotrod_yellow,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	hotrod_brtblue,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // ;	.globl	hotrod_maroon
    // ;	LDL	hotrod_maroon,AR2
    // ;	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	missle_yellow,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	missle_red,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	missle_blue,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	jeep_red,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	jeep_yellow,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	cvette_blue,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	cvette_red,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	cvette_purple,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	testor_blue,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	testor_grape,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // ;	LDL	testor_gold,AR2
    // ;	CALL	PAL_ALLOC_RAW
    // ;	LDL	gtruck_yellow,AR2
    // ;	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	gtruck_bluep,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	muscle_yellow,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	muscle_green,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	ptruckg_purple,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	ptruckg_yellr,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_VARIOUS_PALETTES", 0, 0);
    UNIMPL();
}

void tower_bgrey(void)
{
    // *----------------------------------------------------------------------------
    // *OVERWRITTEN ON TOWER PALETTE, AND RESTORED IN GGATE PARK
    // *
    /* no executable asm lines detected */
    UNIMPL();
}

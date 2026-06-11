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
/* asm: TEASE_COUNT	pbss	TEASE_COUNT,1 */
int TEASE_COUNT;
/* asm: ATTR_MODEL	pbss	ATTR_MODEL,1 */
int ATTR_MODEL;
/* asm: _ATTR_MODE	pbss	_ATTR_MODE,1 */
int _ATTR_MODE;
/* asm: LOADED	pbss	LOADED,1 */
int LOADED;
// *----------------------------------------------------------------------------
// *PARAMETERS
// *	AR2	WAVE INDEX
// *		<  0 -> ATTRACT MODE N
// *		>= 0 -> REGULAR GAME
// *
/* asm: _ATTR_WAVETABI	.word	_ATTR_WAVETAB	;NOTE THE VALUES ARE NEGATVIE */
/* asm: 	.word	HEAD2HEAD_WAIT */
/* asm: 	.word	CREDITS */
/* asm: 	.word	THANKS */
/* asm: 	.word	MIDSPINHS */
/* asm: 	.word	RACELEG */
/* asm: 	.word	MIDSPIN */
/* asm: 	.word	HIGH_SCORE */
int _ATTR_WAVETABI[] = { _ATTR_WAVETAB, HEAD2HEAD_WAIT, CREDITS, THANKS, MIDSPINHS, RACELEG, MIDSPIN, HIGH_SCORE };
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: FIXEDPAL	FIXEDPAL: */
/* asm: 	.word	14 */
int FIXEDPAL = (int)(14);
// 					;F  -cycling color
/* asm: ILLUM_PAL	ILLUM_PAL: */
/* asm: 	.word	16 */
int ILLUM_PAL = (int)(16);
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
/* asm: VEHICLE_TABLE	VEHICLE_TABLE: */
/* asm: 	;#0 */
/* asm: 	.word	cvette,cvette_p,VETTTAB,0,0 */
/* asm: 	.word	1 */
int VEHICLE_TABLE[] = { cvette, cvette_p, VETTTAB, 0, 0, 1 };
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
// ;	.word	3,120
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
// *OVERWRITTEN ON TOWER PALETTE, AND RESTORED IN GGATE PARK
// *
/* asm: tower_bgrey	tower_bgrey: */
/* asm: 	.word	080000000h|256 */
/* asm: 	.word	(00h<<16)|021h,(042h<<16)|0463h,(0463h<<16)|0884h,(0884h<<16)|0884h,(0884h<<16)|0CA5h,(0CA5h<<16)|0CA5h,(0CA5h<<16)|0CA5h,(0CA5h<<16)|0CA5h,(0CA5h<<16)|0CA5h */
/* asm: 	.word	(0CA5h<<16)|0CA5h,(0CA5h<<16)|010C6h,(010C6h<<16)|0CA5h,(0CA5h<<16)|010C6h,(0CA5h<<16)|010C6h,(010C6h<<16)|010C6h,(010C6h<<16)|010C6h,(010C6h<<16)|010C6h,(010C6h<<16)|010C6h */
/* asm: 	.word	(014E7h<<16)|010C6h,(010C6h<<16)|010C6h,(010C6h<<16)|014E7h,(010C6h<<16)|010C6h,(014E7h<<16)|010C6h,(014E7h<<16)|014E7h,(014E7h<<16)|014E7h,(014E7h<<16)|014E7h,(014E7h<<16)|014E7h */
/* asm: 	.word	(014E7h<<16)|014E7h,(014E7h<<16)|014E7h,(014E7h<<16)|01908h,(01908h<<16)|01D29h,(014E7h<<16)|01908h,(014E7h<<16)|01908h,(01908h<<16)|01908h,(01908h<<16)|01908h,(01908h<<16)|01908h */
/* asm: 	.word	(01908h<<16)|014E7h,(01908h<<16)|01908h,(01908h<<16)|01D29h,(01908h<<16)|01908h,(01908h<<16)|010C6h,(01D29h<<16)|01908h,(01908h<<16)|01D29h,(01908h<<16)|01D29h,(01D29h<<16)|01908h */
/* asm: 	.word	(01908h<<16)|01D29h,(01908h<<16)|01D29h,(01908h<<16)|01D29h,(01D29h<<16)|01D29h,(01D29h<<16)|01D29h,(01D29h<<16)|01D29h,(01D29h<<16)|01908h,(01D29h<<16)|01908h,(01D29h<<16)|01D29h */
/* asm: 	.word	(01D29h<<16)|0214Ah,(01D29h<<16)|01D29h,(01D29h<<16)|01D29h,(01D29h<<16)|01D29h,(01D29h<<16)|0214Ah,(01D29h<<16)|0214Ah,(014E7h<<16)|01D29h,(01D29h<<16)|01D29h,(0214Ah<<16)|0214Ah */
/* asm: 	.word	(0214Ah<<16)|01D29h,(0214Ah<<16)|01D29h,(0214Ah<<16)|0214Ah,(0214Ah<<16)|01D29h,(0214Ah<<16)|0214Ah,(0256Bh<<16)|01D29h,(0214Ah<<16)|0214Ah,(0214Ah<<16)|0214Ah,(01D29h<<16)|0214Ah */
/* asm: 	.word	(0256Bh<<16)|014E7h,(01D29h<<16)|0214Ah,(0256Bh<<16)|0214Ah,(0214Ah<<16)|0214Ah,(0256Bh<<16)|0214Ah,(0214Ah<<16)|0256Bh,(0256Bh<<16)|0214Ah,(0214Ah<<16)|0214Ah,(0256Bh<<16)|0214Ah */
/* asm: 	.word	(0256Bh<<16)|0256Bh,(0214Ah<<16)|0256Bh,(0256Bh<<16)|0214Ah,(0256Bh<<16)|0256Bh,(0214Ah<<16)|0256Bh,(0298Ch<<16)|0256Bh,(0256Bh<<16)|0214Ah,(0256Bh<<16)|0256Bh,(0256Bh<<16)|0256Bh */
/* asm: 	.word	(0256Bh<<16)|01D29h,(0298Ch<<16)|0256Bh,(0298Ch<<16)|0256Bh,(0256Bh<<16)|0256Bh,(0298Ch<<16)|0298Ch,(0298Ch<<16)|0256Bh,(0298Ch<<16)|0298Ch,(0298Ch<<16)|01D29h,(0298Ch<<16)|0298Ch */
/* asm: 	.word	(0298Ch<<16)|0298Ch,(0298Ch<<16)|0256Bh,(0298Ch<<16)|0298Ch,(0256Bh<<16)|0298Ch,(0298Ch<<16)|0298Ch,(0298Ch<<16)|0298Ch,(02DADh<<16)|02DADh,(0298Ch<<16)|02DADh,(02DADh<<16)|0298Ch */
/* asm: 	.word	(0298Ch<<16)|02DADh,(02DADh<<16)|02DADh,(02DADh<<16)|02DADh,(0298Ch<<16)|02DADh,(02DADh<<16)|02DADh,(031CEh<<16)|02DADh,(02DADh<<16)|031CEh,(031CEh<<16)|031CEh,(031CEh<<16)|02DADh */
/* asm: 	.word	(031CEh<<16)|031CEh,(02DADh<<16)|031CEh,(031CEh<<16)|031CEh,(031CEh<<16)|035EFh,(035EFh<<16)|031CEh,(031CEh<<16)|035EFh,(03A10h<<16)|035EFh,(02DADh<<16)|03E31h,(035EFh<<16)|03E31h */
/* asm: 	.word	(03E31h<<16)|04252h,(04673h<<16)|04A94h */
/* asm: 	*---------------------------------------------------------------------------- */
int tower_bgrey[] = { 0x080000000|256, (0x00<<16)|0x021, (0x042<<16)|0x0463, (0x0463<<16)|0x0884, (0x0884<<16)|0x0884, (0x0884<<16)|0x0CA5, (0x0CA5<<16)|0x0CA5, (0x0CA5<<16)|0x0CA5, (0x0CA5<<16)|0x0CA5, (0x0CA5<<16)|0x0CA5, (0x0CA5<<16)|0x0CA5, (0x0CA5<<16)|0x010C6, (0x010C6<<16)|0x0CA5, (0x0CA5<<16)|0x010C6, (0x0CA5<<16)|0x010C6, (0x010C6<<16)|0x010C6, (0x010C6<<16)|0x010C6, (0x010C6<<16)|0x010C6, (0x010C6<<16)|0x010C6, (0x014E7<<16)|0x010C6, (0x010C6<<16)|0x010C6, (0x010C6<<16)|0x014E7, (0x010C6<<16)|0x010C6, (0x014E7<<16)|0x010C6, (0x014E7<<16)|0x014E7, (0x014E7<<16)|0x014E7, (0x014E7<<16)|0x014E7, (0x014E7<<16)|0x014E7, (0x014E7<<16)|0x014E7, (0x014E7<<16)|0x014E7, (0x014E7<<16)|0x01908, (0x01908<<16)|0x01D29, (0x014E7<<16)|0x01908, (0x014E7<<16)|0x01908, (0x01908<<16)|0x01908, (0x01908<<16)|0x01908, (0x01908<<16)|0x01908, (0x01908<<16)|0x014E7, (0x01908<<16)|0x01908, (0x01908<<16)|0x01D29, (0x01908<<16)|0x01908, (0x01908<<16)|0x010C6, (0x01D29<<16)|0x01908, (0x01908<<16)|0x01D29, (0x01908<<16)|0x01D29, (0x01D29<<16)|0x01908, (0x01908<<16)|0x01D29, (0x01908<<16)|0x01D29, (0x01908<<16)|0x01D29, (0x01D29<<16)|0x01D29, (0x01D29<<16)|0x01D29, (0x01D29<<16)|0x01D29, (0x01D29<<16)|0x01908, (0x01D29<<16)|0x01908, (0x01D29<<16)|0x01D29, (0x01D29<<16)|0x0214A, (0x01D29<<16)|0x01D29, (0x01D29<<16)|0x01D29, (0x01D29<<16)|0x01D29, (0x01D29<<16)|0x0214A, (0x01D29<<16)|0x0214A, (0x014E7<<16)|0x01D29, (0x01D29<<16)|0x01D29, (0x0214A<<16)|0x0214A, (0x0214A<<16)|0x01D29, (0x0214A<<16)|0x01D29, (0x0214A<<16)|0x0214A, (0x0214A<<16)|0x01D29, (0x0214A<<16)|0x0214A, (0x0256B<<16)|0x01D29, (0x0214A<<16)|0x0214A, (0x0214A<<16)|0x0214A, (0x01D29<<16)|0x0214A, (0x0256B<<16)|0x014E7, (0x01D29<<16)|0x0214A, (0x0256B<<16)|0x0214A, (0x0214A<<16)|0x0214A, (0x0256B<<16)|0x0214A, (0x0214A<<16)|0x0256B, (0x0256B<<16)|0x0214A, (0x0214A<<16)|0x0214A, (0x0256B<<16)|0x0214A, (0x0256B<<16)|0x0256B, (0x0214A<<16)|0x0256B, (0x0256B<<16)|0x0214A, (0x0256B<<16)|0x0256B, (0x0214A<<16)|0x0256B, (0x0298C<<16)|0x0256B, (0x0256B<<16)|0x0214A, (0x0256B<<16)|0x0256B, (0x0256B<<16)|0x0256B, (0x0256B<<16)|0x01D29, (0x0298C<<16)|0x0256B, (0x0298C<<16)|0x0256B, (0x0256B<<16)|0x0256B, (0x0298C<<16)|0x0298C, (0x0298C<<16)|0x0256B, (0x0298C<<16)|0x0298C, (0x0298C<<16)|0x01D29, (0x0298C<<16)|0x0298C, (0x0298C<<16)|0x0298C, (0x0298C<<16)|0x0256B, (0x0298C<<16)|0x0298C, (0x0256B<<16)|0x0298C, (0x0298C<<16)|0x0298C, (0x0298C<<16)|0x0298C, (0x02DAD<<16)|0x02DAD, (0x0298C<<16)|0x02DAD, (0x02DAD<<16)|0x0298C, (0x0298C<<16)|0x02DAD, (0x02DAD<<16)|0x02DAD, (0x02DAD<<16)|0x02DAD, (0x0298C<<16)|0x02DAD, (0x02DAD<<16)|0x02DAD, (0x031CE<<16)|0x02DAD, (0x02DAD<<16)|0x031CE, (0x031CE<<16)|0x031CE, (0x031CE<<16)|0x02DAD, (0x031CE<<16)|0x031CE, (0x02DAD<<16)|0x031CE, (0x031CE<<16)|0x031CE, (0x031CE<<16)|0x035EF, (0x035EF<<16)|0x031CE, (0x031CE<<16)|0x035EF, (0x03A10<<16)|0x035EF, (0x02DAD<<16)|0x03E31, (0x035EF<<16)|0x03E31, (0x03E31<<16)|0x04252, (0x04673<<16)|0x04A94 };

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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAVE", 0, 0);
    UNIMPL();
}

void HEAD2HEADWATCH(void)
{
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEADWATCH", 0, 0);
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

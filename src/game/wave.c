#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "obj.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "pall.h"
#include "sndtab.h"
#include "objects.h"
#include "text.h"
#include "dirq.h"
#include "comm.h"
#include "h2hobj.h"
#include "wave.h"
#include "discovered_defines.h"
#include "discovered_labels.h"

/*
 * Source module: asm/WAVE.ASM
 */

void WAVE(void);
void HEAD2HEAD_WAIT(void);
void HIGH_SCORE(void);
void MIDSPIN(void);
void MIDSPINHS(void);
void RACELEG(void);
#define HIGH_SCORE_INI THANKS
void THANKS(void);
void LOAD_HIGH_SCORE(void);
void BEGIN_GAME(void);
void INIT_SYSTEM(void);
void LOAD_FIXED_PALETTES(void);
void LOAD_STARTUP_PALS(void);

/* asm: TEASE_COUNT	pbss	TEASE_COUNT,1 */
int TEASE_COUNT;
/* asm: ATTR_MODEL	pbss	ATTR_MODEL,1 */
int ATTR_MODEL;
/* asm: _ATTR_MODE	pbss	_ATTR_MODE,1 */
int _ATTR_MODE;
/* asm: LOADED	pbss	LOADED,1 */
int LOADED;
/* *----------------------------------------------------------------------------
 */
/* asm: FIXEDPAL: */
/* asm: .word	14 */
/* asm: RGB	0,0,0 */
/* asm: RGB	255,255,255		;1  WHITE */
/* asm: RGB	070h,070h,070h		;2  GREY */
/* asm: RGB	255,0,0			;3  RED */
/* asm: RGB	0,255,0			;4  GREEN */
/* asm: RGB	055h,055h,99h		;5  BLUE  (for the sky paled out) */
/* asm: RGB	255,255,0		;6  YELLOW */
/* asm: RGB	0,255,255		;7  CYAN */
/* asm: RGB	0,0,0			;8  BLACK */
/* asm: RGB	0,0,255			;9  TRUE BLUE */
/* asm: RGB	0C0h,0A0h,0		;A  Amber color */
/* asm: RGB	050h,050h,050h		;B	DARK GREY */
/* asm: RGB	0A0h,0A0h,0A0h		;C	LIGHT GREY */
/* asm: RGB	030h,030h,030h		;D	VERY DARK GREY */
int FIXEDPAL;
/* asm: ILLUM_PAL: */
/* asm: .word	16 */
/* asm: RGB	06Ah,000h,000h */
/* asm: RGB	06Fh,000h,000h */
/* asm: RGB	074h,000h,000h */
/* asm: RGB	079h,000h,000h */
/* asm: RGB	07Eh,000h,000h */
/* asm: RGB	083h,000h,000h */
/* asm: RGB	088h,000h,000h */
/* asm: RGB	08Dh,000h,000h */
/* asm: RGB	092h,000h,000h */
/* asm: RGB	097h,000h,000h */
/* asm: RGB	09Ch,000h,000h */
/* asm: RGB	0A1h,002h,005h */
/* asm: RGB	0A6h,007h,00Ah */
/* asm: RGB	0ABh,00Ch,00Fh */
/* asm: RGB	0B0h,011h,014h */
/* asm: RGB	0B5h,016h,019h */
/* asm: romdata */
int ILLUM_PAL;
/* *----------------------------------------------------------------------------
 */
/* asm: COPCARTAB	;PLAYERS VERSION!!! */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	.float	0,101,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	.float	192,101,-301	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	208,101,371	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-192,101,-301	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	-208,101,371	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	0,0,0	 	;BODY XYZ CENTER OFFSET */
/* asm: 	.word	72-1		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
int COPCARTAB[] = {
    5, // #OF DYNAS-1
    0, 101, 0, // SHADOW
    3, // VERTS-1
    -1, // DYNAFLAG
    192, 101, -301, // RT REAR WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    208, 101, 371, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -192, 101, -301, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    -208, 101, 371, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    0, 0, 0, // BODY XYZ CENTER OFFSET
    72-1, // VERTS-1
    0, // DYNAFLAG
};
/* asm: HOTRODTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	.float	0,117,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	.float	211,100,-329	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	221,100,414	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-211,100,-329	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	-221,100,414	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	0,0,0	 	;BODY XYZ CENTER OFFSET */
/* asm: 	.word	131		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
int HOTRODTAB[] = {
    5, // #OF DYNAS-1
    0, 117, 0, // SHADOW
    3, // VERTS-1
    -1, // DYNAFLAG
    211, 100, -329, // RT REAR WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    221, 100, 414, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -211, 100, -329, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    -221, 100, 414, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    0, 0, 0, // BODY XYZ CENTER OFFSET
    131, // VERTS-1
    0, // DYNAFLAG
};
/* asm: TESTORTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	.float	0,62,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	.float	251,62,-333	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	251,62,285	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-251,62,285	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-251,62,-333	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	0,0,0	 	;BODY XYZ CENTER OFFSET */
/* asm: 	.word	59		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
int TESTORTAB[] = {
    5, // #OF DYNAS-1
    0, 62, 0, // SHADOW
    3, // VERTS-1
    -1, // DYNAFLAG
    251, 62, -333, // RT REAR WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    251, 62, 285, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -251, 62, 285, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -251, 62, -333, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    0, 0, 0, // BODY XYZ CENTER OFFSET
    59, // VERTS-1
    0, // DYNAFLAG
};
/* asm: JEEPTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	.float	0,119,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	.float	250,131,-285	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	250,131,355	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-250,131,-285	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	-250,131,357	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	62		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
int JEEPTAB[] = {
    5, // #OF DYNAS-1
    0, 119, 0, // SHADOW
    3, // VERTS-1
    -1, // DYNAFLAG
    250, 131, -285, // RT REAR WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    250, 131, 355, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -250, 131, -285, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    -250, 131, 357, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    0, 0, 0, // BODY XYZ CENTER OFFSET
    62, // VERTS-1
    0, // DYNAFLAG
};
/* asm: VETTTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	.float	0,-164,0	;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	.float	254,67,-296	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	228,81,376	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-254,67,-296	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	-228,81,376	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	114		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
int VETTTAB[] = {
    5, // #OF DYNAS-1
    0, -164, 0, // SHADOW
    3, // VERTS-1
    -1, // DYNAFLAG
    254, 67, -296, // RT REAR WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    228, 81, 376, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -254, 67, -296, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    -228, 81, 376, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    0, 0, 0, // BODY XYZ CENTER OFFSET
    114, // VERTS-1
    0, // DYNAFLAG
};
/* asm: GTRUCKTABP */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	.float	0,-117,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	.float	259,86,-358	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	248,90,388	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-259,86,-358	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	-248,90,388	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	131		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
int GTRUCKTABP[] = {
    5, // #OF DYNAS-1
    0, -117, 0, // SHADOW
    3, // VERTS-1
    -1, // DYNAFLAG
    259, 86, -358, // RT REAR WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    248, 90, 388, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -259, 86, -358, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    -248, 90, 388, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    0, 0, 0, // BODY XYZ CENTER OFFSET
    131, // VERTS-1
    0, // DYNAFLAG
};
/* asm: MISSILE_TAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	.float	0,-117,0		;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	.float	259,86,-358	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	248,90,388	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-259,86,-358	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	-248,90,388	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	131		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
int MISSILE_TAB[] = {
    5, // #OF DYNAS-1
    0, -117, 0, // SHADOW
    3, // VERTS-1
    -1, // DYNAFLAG
    259, 86, -358, // RT REAR WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    248, 90, 388, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -259, 86, -358, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    -248, 90, 388, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    0, 0, 0, // BODY XYZ CENTER OFFSET
    131, // VERTS-1
    0, // DYNAFLAG
};
/* asm: PSBUSTAB */
/* asm: 	.word	5		;#OF DYNAS-1 */
/* asm: 	.float	0,-127,0	;SHADOW */
/* asm: 	.word	3		;VERTS-1 */
/* asm: 	.word	-1		;DYNAFLAG */
/* asm: 	.float	246,127,-380	;RT REAR WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	247,127,573	;RT FRONT WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	-247,127,-380	;LFT REAR WHEEL	XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	1		;DYNAFLAG */
/* asm: 	.float	-247,127,573	;LFT FRONT WHEEL XYZ CENTER OFFSET */
/* asm: 	.word	23		;VERTS-1 */
/* asm: 	.word	2		;DYNAFLAG */
/* asm: 	.float	0,0,0		;BODY XYZ CENTER OFFSET */
/* asm: 	.word	91		;VERTS-1 */
/* asm: 	.word	0		;DYNAFLAG */
int PSBUSTAB[] = {
    5, // #OF DYNAS-1
    0, -127, 0, // SHADOW
    3, // VERTS-1
    -1, // DYNAFLAG
    246, 127, -380, // RT REAR WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    247, 127, 573, // RT FRONT WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    -247, 127, -380, // LFT REAR WHEEL	XYZ CENTER OFFSET
    23, // VERTS-1
    1, // DYNAFLAG
    -247, 127, 573, // LFT FRONT WHEEL XYZ CENTER OFFSET
    23, // VERTS-1
    2, // DYNAFLAG
    0, 0, 0, // BODY XYZ CENTER OFFSET
    91, // VERTS-1
    0, // DYNAFLAG
};

/* *----------------------------------------------------------------------------
*PARAMETERS
*	AR2	WAVE INDEX
*		<  0 -> ATTRACT MODE N
*		>= 0 -> REGULAR GAME
*
 */
void WAVE(void)
{
    // asm 00009307: 	POP	AR7	;return addr
    // asm 00009308: 	CALL	SILENT
    // 	;CLEAR ALL RAM AND RELOAD CODE
    // 	;
    // asm 00009309: 	CLRI	R0			;CLEAR INTERNAL RAM
    // asm 0000930A: 	LDIL	RAM0,AR0
    // asm 0000930D: 	RPTS	2047
    // asm 0000930E: 	STI	R0,*AR0++
    // asm 0000930F: 	CALL	COMM_ENABLE_INT2
    // asm 00009310: 	LDI	@COUNTER_IDX,R0
    // asm 00009311: 	PUSH	R0
    // asm 00009312: 	LDI	@COUNTER_MODE,R0
    // asm 00009313: 	PUSH	R0
    // asm 00009314: 	LDI	@COIN_COUNTER1,R0
    // asm 00009315: 	PUSH	R0
    // asm 00009316: 	LDI	@COIN_COUNTER2,R0
    // asm 00009317: 	PUSH	R0
    // asm 00009318: 	CALL	CLR_RAM			;CLEAR BSS SPACE
    // asm 00009319: 	POP	R0
    // asm 0000931A: 	STI	R0,@COIN_COUNTER2
    // asm 0000931B: 	POP	R0
    // asm 0000931C: 	STI	R0,@COIN_COUNTER1
    // asm 0000931D: 	POP	R0
    // asm 0000931E: 	STI	R0,@COUNTER_MODE
    // asm 0000931F: 	POP	R0
    // asm 00009320: 	STI	R0,@COUNTER_IDX
    // 	;RELOAD GAME CODE
    // asm 00009321: 	LDI	0,AR1			;SOURCE ADDRESS
    // asm 00009322: 	LDI	4000h,AR3		;DESINATION ADDRESS
    // asm 00009323: 	LS	8,AR3
    // asm 00009324: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm 00009325: 	LS	4,RC			;FAST RAM
    // asm 00009326: 	RPTB	LD_RAM
    // asm 00009327: 	LDI	*AR1++,R0
    // asm 00009328: LD_RAM	STI	R0,*AR3++
    // asm 00009329: 	PUSH	AR7
    // asm 0000932A: 	PUSH	AR2
#if DEBUG
    // asm: 	CALL	VERIFY_CODE_INTEGRITY
#endif
    // asm 0000932B: 	CALL	CLRONE	;CAN NOW BE DUAL PLAYER
    // asm 0000932C: 	CALL	INIT_SYSTEM
    // asm 0000932D: 	LDP	@_CAMERARAD
    // asm 0000932E: 	CLRF	R0
    // asm 0000932F: 	STF	R0,@_CAMERARAD+X
    // asm 00009330: 	STF	R0,@_CAMERARAD+Y
    // asm 00009331: 	STF	R0,@_CAMERARAD+Z
    // asm 00009332: 	STF	R0,@_CAMERAPOS+X
    // asm 00009333: 	STF	R0,@_CAMERAPOS+Y
    // asm 00009334: 	STF	R0,@_CAMERAPOS+Z
    // asm 00009335: 	STF	R0,@_LIGHT+Z
    // asm 00009336: 	LDF	0.707,R0
    // asm 00009337: 	STF	R0,@_LIGHT+X
    // asm 00009338: 	STF	R0,@_LIGHT+Y
    // asm 00009339: 	SETDP
    // asm 0000933A: 	CALL	INIT_CUSTOM_COIN	;Set the CUSTOM SETUP in RAM
    // asm 0000933B: 	LDI	@CAMERAMATRIXI,AR0
    // asm 0000933C: 	CALL	INITMAT
    // asm 0000933D: 	FLOAT	35,R0
    // asm 0000933E: 	STF	R0,@INFIN_CORRECT
    // asm 0000933F: 	LDI	1,R0
    // asm 00009340: 	STI	R0,@CLEARRDY
    // asm 00009341: 	READADJ	ADJ_STEERCENTER
    // asm 00009343: 	FLOAT	R0
    // asm 00009344: 	STF	R0,@WHEELPOS
    // asm 00009345: 	CLRF	R0
    // asm 00009346: 	STF	R0,@WHEELPWR
    // asm 00009347: 	STF	R0,@WHEELVEL
    // asm 00009348: 	CLRI	R0
    // asm 00009349: 	STI	R0,@COINOFF
    // asm 0000934A: 	STI	R0,@NOLONG_VEHICLES
    // asm 0000934B: 	CALL	LOAD_FIXED_PALETTES
    // asm 0000934C: 	CREATE	SCAN_OBJECTS,UTIL_C
    // asm 0000934F: 	POP	AR2
    // asm 00009350: 	CMPI	1,AR2
    // asm 00009351: 	BEQ	BEGIN_GAME
    // asm 00009352: 	CLRI	AR0
    // asm 00009353: 	LDP	@SWITCH3
    // asm 00009354: 	NOT	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // asm 00009355: 	LDI	*AR0,R2			;Loff
    // asm 00009356: 	SETDP
    // asm 00009357: 	AND	SW_VIEW0_H|SW_VIEW1_H|SW_VIEW2_H,R0
    // asm 00009358: 	CMPI	SW_VIEW1_H|SW_VIEW2_H,R0
    // asm 00009359: 	BEQ	CREDITS
    // asm 0000935A: 	LDI	AR2,AR0
    // asm 0000935B: 	ADDI	@_ATTR_WAVETABI,AR0
    // asm 0000935C: 	LDI	*AR0,R0
    // asm 0000935D: 	CALLU	R0
    // asm 0000935E: 	RETS
    // asm 00009367: _ATTR_WAVETAB
HEAD2HEADWATCH:
    // asm 00009367: 	SLEEP	1
    // asm 00009369: 	LDI	@OM_MODE,R0
    // asm 0000936A: 	AND	MMODE,R0
    // asm 0000936B: 	CMPI	MBONUS,R0
    // asm 0000936C: 	BEQ	ISTRUE
    // asm 0000936D: 	CMPI	MINIT,R0
    // asm 0000936E: 	BEQ	ISTRUE
    // asm 0000936F: 	CMPI	MINSERT_COINS,R0
    // asm 00009370: 	BEQ	ISTRUE
    // asm 00009371: 	LDI	@OM_LINKWAIT,R0
    // asm 00009372: 	BZ	HEAD2HEADWATCH
ISTRUE:
    // asm 00009373: LDI	-7,R0
    // asm 00009374: 	STI	R0,@_ATTR_MODE
    // asm 00009375: 	BR	SET_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAVE", 0, 0);
    UNIMPL();
}

void HEAD2HEAD_WAIT(void)
{
    // asm 00009376: 	LDI	@BUTTON_STATUS,R0
    // asm 00009377: 	ANDN	BUT_VIEWS,R0
    // asm 00009378: 	STI	R0,@BUTTON_STATUS
    // asm 00009379: 	CREATE	HEAD2HEAD_LOGO_WAIT,UTIL_C
    // asm 0000937C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HEAD2HEAD_WAIT", 0, 0);
    UNIMPL();
}

void HIGH_SCORE(void)
{
    // asm 0000937D: 	LDI	@BUTTON_STATUS,R0
    // asm 0000937E: 	ANDN	BUT_VIEWS,R0
    // asm 0000937F: 	STI	R0,@BUTTON_STATUS
    // asm 00009380: 	LDI	-1,R0	;Kick start the light routine
    // asm 00009381: 	STI	R0,@OLD_BUTTON_STATUS
    // asm 00009382: 	LDI	MATTR|MHS,R0
    // asm 00009383: 	STI	R0,@_MODE
    // asm 00009384: 	CREATE	DISPLAY_HIGH_SCORES,UTIL_C
    // asm 00009387: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm 0000938A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HIGH_SCORE", 0, 0);
    UNIMPL();
}

void MIDSPIN(void)
{
    // asm 0000938B: 	LDI	@BUTTON_STATUS,R0
    // asm 0000938C: 	ANDN	BUT_VIEWS,R0
    // asm 0000938D: 	STI	R0,@BUTTON_STATUS
    // asm 0000938E: 	CLRI	R0
    // asm 0000938F: 	LDI	MATTR,R0
    // asm 00009390: 	STI	R0,@_MODE
    // asm 00009391: 	CALL	LOAD_ATTR_LEG
    // asm 00009392: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm 00009395: 	LDI	@ATTRWAVE,AR6		;DCS LOGOSOUND will play only if 0
    // ;	LDI	3,AR2
    // ;	CALL	RANDU0
    // ;	CMPI	0,R0
    // ;	BEQ	BBDD
    // ;
    // ;	CREATE	HEAD2HEAD_LOGO,UTIL_C
    // ;	RETS
    // ;BBDD
    // asm 00009396: 	CREATE	_MIDWAYSPIN,UTIL_C
    // asm 00009399: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MIDSPIN", 0, 0);
    UNIMPL();
}

void MIDSPINHS(void)
{
    // asm 0000939A: 	LDI	@BUTTON_STATUS,R0
    // asm 0000939B: 	ANDN	BUT_VIEWS,R0
    // asm 0000939C: 	STI	R0,@BUTTON_STATUS
    // asm 0000939D: 	LDL	_SECpress,AR2
    // asm 0000939E: 	CALL	LOAD_SECTION_REQ
    // asm 0000939F: 	LDI	MATTR,R0
    // asm 000093A0: 	STI	R0,@_MODE
    // asm 000093A1: 	CREATE	SPIN_CAR,UTIL_C
    // asm 000093A4: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm 000093A7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MIDSPINHS", 0, 0);
    UNIMPL();
}

void RACELEG(void)
{
    // asm 000093A8: 	CLRI	R0
    // asm 000093A9: 	STI	R0,@TEASE_COUNT
    // asm 000093AA: 	LDI	MATTR,R0
    // asm 000093AB: 	STI	R0,@_MODE
    // asm 000093AC: 	CREATE	ATTRACT_DELTA,DRONE_C|DELTA_ATTR_S
    // asm 000093AF: 	LDI	@ATTRWAVE,R0
    // asm 000093B0: 	CMPI	4,R0
    // asm 000093B1: 	LDIEQ	25*30,R0
    // asm 000093B2: 	LDINE	30*30,R0
    // asm 000093B3: 	STI	R0,@_timer
    // asm 000093B4: 	CREATE	_timeout,UTIL_C
    // asm 000093B7: 	CREATE	HEAD2HEADWATCH,UTIL_C
    // asm 000093BA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RACELEG", 0, 0);
    UNIMPL();
}

void THANKS(void)
{
    // asm 000093BB: 	CALL	LOAD_HIGH_SCORE
    // asm 000093BC: 	BU	HIGH_SCORE
    // 	;these are not cycled, they are special routines
CREDITS:
    // asm 000093BD: 	CREATE	VANITY,UTIL_C
    // asm 000093C0: 	LDI	-1,R0
    // asm 000093C1: 	STI	R0,@_ATTR_MODE
    // asm 000093C2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "THANKS", 0, 0);
    UNIMPL();
}

void LOAD_HIGH_SCORE(void)
{
    // asm 000093C3: 	CALL	FIFO_RESET
    // asm 000093C4: 	LDI	1,R0
    // asm 000093C5: 	STI	R0,@HARD_SECTION_LOAD
    // asm 000093C6: 	LDL	_SECpress,AR2
    // asm 000093C7: 	CALL	LOAD_SECTION_REQ
    // asm 000093C8: 	LDI	-1,R0
    // asm 000093C9: 	STI	R0,@_ATTR_MODE
    // asm 000093CA: 	RETS
    // *
    // *NOTE	ANY GAME STARTUP CODE & PROC INITS ARE CALLED/CREATED IN INTRO.ASM
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_HIGH_SCORE", 0, 0);
    UNIMPL();
}

void BEGIN_GAME(void)
{
    // asm 000093CB: 	CALL	SND_RESET_QUIET
    // asm 000093CC: 	LDI	1,R0
    // asm 000093CD: 	STI	R0,@LOADED
    // asm 000093CE: 	CREATE	PLYR_INTRO,PLYR_C|PLYR1_T	;PLAYERS PROCESS
    // asm 000093D1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BEGIN_GAME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void INIT_SYSTEM(void)
{
    // asm 000093D2: 	CALL	PRC_INIT	;initialize process system
    // asm 000093D3: 	CALL	OBJ_INIT	;initialize object system
    // asm 000093D4: 	CALL	TEXT_INIT	;initialize text system
    // asm 000093D5: 	CALL	INIT_DRONES	;initialize DRONE tracker system
    // asm 000093D6: 	CALL	DYNAOBJ_INIT	;initialize DYNAMIC OBJECTS
    // asm 000093D7: 	CALL	CARB_INIT	;initialize CAR BLOCKS
    // asm 000093D8: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    // asm 000093D9: 	CLRI	R0
    // asm 000093DA: 	STI	R0,@_sectime
    // asm 000093DB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_SYSTEM", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*THESE PALETTES ONLY HAVE TO BE LOADED UPON STARTUP
*
 */
void LOAD_FIXED_PALETTES(void)
{
    // asm 000093FC: 	CALL	PAL_INIT
    // asm 000093FD: 	LDL	FIXEDPAL,AR2
    // asm 000093FE: 	CALL	PAL_ALLOC_RAW
    // asm 000093FF: 	CLRI	R0
    // asm 00009400: 	LDP	@COLORAM
    // asm 00009401: 	STI	R0,@COLORAM+0FFh
    // asm 00009402: 	SETDP
    // asm 00009403: 	LDL	fixedfnt_tPAL,AR2
    // asm 00009404: 	CALL	PAL_ALLOC_RAW
    // asm 00009405: 	LDL	ILLUM_PAL,AR2		;only 1 palette
    // asm 00009406: 	CALL	PAL_ALLOC_RAW
    // asm 00009407: 	LDL	lgnum43_coolyelo,AR2
    // asm 00009408: 	CALL	PAL_ALLOC_RAW
    // asm 00009409: 	LDL	font18_white,AR2
    // asm 0000940A: 	CALL	PAL_ALLOC_RAW
    // asm 0000940B: 	LDL	dnums_amber,AR2
    // asm 0000940C: 	CALL	PAL_ALLOC_RAW
    // asm 0000940D: 	CALL	LOAD_VARIOUS_PALETTES	;mixed palettes
    // asm 0000940E: 	LDL	shared_PALETTES,AR2
    // asm 0000940F: 	CALL	HARDalloc_section
    // asm 00009410: 	LDI	sky1_p,AR2
    // asm 00009411: 	CALL	PAL_ALLOC
    // asm 00009412: 	LDL	osg10fnt_white,AR2
    // asm 00009413: 	CALL	PAL_ALLOC_RAW
    // asm 00009414: 	LDL	bnout1_smoke,AR2
    // asm 00009415: 	CALL	PAL_ALLOC_RAW
    // asm 00009416: 	LDL	H2HPAL1,AR2
    // asm 00009417: 	CALL	PAL_ALLOC_RAW
    // asm 00009418: 	LDL	H2HPAL2,AR2
    // asm 00009419: 	CALL	PAL_ALLOC_RAW
    // asm 0000941A: 	LDL	H2HPAL3,AR2
    // asm 0000941B: 	CALL	PAL_ALLOC_RAW
    // asm 0000941C: 	LDL	general_PALETTES,AR2
    // asm 0000941D: 	CALL	HARDalloc_section
    // asm 0000941E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_FIXED_PALETTES", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void LOAD_STARTUP_PALS(void)
{
    // asm 0000941F: 	LDL	ggate_PALETTES,AR2
    // asm 00009420: 	CALL	HARDalloc_section
    // asm 00009421: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_STARTUP_PALS", 0, 0);
    UNIMPL();
}

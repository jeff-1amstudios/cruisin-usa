#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/obj.h"
#include "../include/objects.h"
#include "../include/globals.h"
#include "../include/sys.h"
#include "../include/vunit.h"
#include "../include/pall.h"
#include "../include/infin.h"
#include "../include/discovered_defines.h"

/*
 * Source module: asm/INFIN.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: CAMRADY	.bss	CAMRADY,1 */
int CAMRADY;
// *----------------------------------------------------------------------------
// *SEARCH THE ROAD OBJECTS AND FIND THE HIGHEST POSITIONED OBJECT.
// *
// *Note that the case of the player being inverted can not happen because the
// *infinity plane would freak out, and who knows what would happen then, so we
// *assume that this case is not present. Therefore we can simply say the the
// *highest valued Y position of the objects is the highest Y position, and then
// *only that value must be projected in Y.
// *
// *
#define HIGH_CLIP_LEVEL ((5000-1))
/* asm: HIGHEST_ROADY	.bss	HIGHEST_ROADY,1 */
int HIGHEST_ROADY;
/* asm: HIGHEST_ROADY_X	.bss	HIGHEST_ROADY_X,1 */
int HIGHEST_ROADY_X;
/* asm: VAR_ROAD_KFACTOR	.bss	VAR_ROAD_KFACTOR,1 */
int VAR_ROAD_KFACTOR;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *PLOT INFINITY PLANE
// *
// *VERTICES TRANSFORMED AS
// *     1	 3  5
// *
// *     0	 2  4  etc.
// *
// *
/* asm: AMOUNT_CLIPPED	.bss	AMOUNT_CLIPPED,1 */
int AMOUNT_CLIPPED;
/* asm: LOIVAL	LOIVAL	.word	-768 */
int LOIVAL = -768;
/* asm: HIGHIVAL	HIGHIVAL	.word	1536 */
int HIGHIVAL = 1536;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: INFINITY_POINTS	INFINITY_POINTS	.word	INFINPOINTS */
int INFINITY_POINTS = INFINPOINTS;
/* asm: INFINPOINTS	INFINPOINTS */
/* asm: 	.float	-1280,0,0 */
/* asm: 	.float	-1280,-255,0 */
/* asm: 	.float	-1024,0,0 */
/* asm: 	.float	-1024,-255,0 */
/* asm: 	.float	-768,0,0 */
/* asm: 	.float	-768,-255,0 */
/* asm: 	.float	-512,0,0 */
/* asm: 	.float	-512,-255,0 */
/* asm: 	.float	-256,0,0 */
/* asm: 	.float	-256,-255,0 */
/* asm: 	.float	0,0,0 */
/* asm: 	.float	0,-255,0 */
/* asm: 	.float	256,0,0 */
/* asm: 	.float	256,-255,0 */
/* asm: 	.float	512,0,0 */
/* asm: 	.float	512,-255,0 */
/* asm: 	.float	768,0,0 */
/* asm: 	.float	768,-255,0 */
/* asm: 	.float	1024,0,0 */
/* asm: 	.float	1024,-255,0 */
/* asm: 	.float	1280,0,0 */
/* asm: 	.float	1280,-255,0 */
/* asm: 	.float	1536,0,0 */
/* asm: 	.float	1536,-255,0 */
/* asm: 	.float	1792,0,0 */
/* asm: 	.float	1792,-255,0 */
/* asm: 	.float	-1280,-253,0 */
/* asm: 	.float	-1280,-1250,0 */
/* asm: 	.float	-300,-253,0 */
/* asm: 	.float	-300,-1250,0 */
/* asm: 	.float	700,-253,0 */
/* asm: 	.float	700,-1250,0 */
/* asm: 	.float	1700,-253,0 */
/* asm: 	.float	1700,-1250,0 */
int INFINPOINTS[] = {
    -1280, 0, 0,
    -1280, -255, 0,
    -1024, 0, 0,
    -1024, -255, 0,
    -768, 0, 0,
    -768, -255, 0,
    -512, 0, 0,
    -512, -255, 0,
    -256, 0, 0,
    -256, -255, 0,
    0, 0, 0,
    0, -255, 0,
    256, 0, 0,
    256, -255, 0,
    512, 0, 0,
    512, -255, 0,
    768, 0, 0,
    768, -255, 0,
    1024, 0, 0,
    1024, -255, 0,
    1280, 0, 0,
    1280, -255, 0,
    1536, 0, 0,
    1536, -255, 0,
    1792, 0, 0,
    1792, -255, 0,
    -1280, -253, 0,
    -1280, -1250, 0,
    -300, -253, 0,
    -300, -1250, 0,
    700, -253, 0,
    700, -1250, 0,
    1700, -253, 0,
    1700, -1250, 0,
};
/* asm: INFIN_POLYGONSI	INFIN_POLYGONSI	.word	BLUESKY */
#define INFIN_POLYGONSI BLUESKY
/* asm: BLUESKY	BLUESKY	.word	sky1_p,sky1_I,sky1_p,sky2_I,sky1_p,sky3_I,sky1_p,sky4_I,sky1_p,sky5_I,sky1_p,sky6_I */
/* asm: 	.word	sky1_p,sky1_I,sky1_p,sky2_I,sky1_p,sky3_I,sky1_p,sky4_I,sky1_p,sky5_I,sky1_p,sky6_I */
int BLUESKY[] = {
    sky1_p, sky1_I, sky1_p, sky2_I, sky1_p, sky3_I, sky1_p, sky4_I, sky1_p, sky5_I, sky1_p, sky6_I,
    sky1_p, sky1_I, sky1_p, sky2_I, sky1_p, sky3_I, sky1_p, sky4_I, sky1_p, sky5_I, sky1_p, sky6_I,
};
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *
// *PARAMETERS
// *	AR4	NUMBER OF VERTICES
// *	AR5	POINTERS [X Y Z]
// *	AR6	RAM BUFFER
// *
// *
// *
/* asm: INFIN_CORRECT	.bss	INFIN_CORRECT,1 */
int INFIN_CORRECT;
/* asm: WATERPOS	WATERPOS */
/* asm: 	.float	-1280,128,0 */
/* asm: 	.float	-1280,0,0 */
/* asm: 	.float	-1024,128,0 */
/* asm: 	.float	-1024,0,0 */
/* asm: 	.float	-768,128,0 */
/* asm: 	.float	-768,0,0 */
/* asm: 	.float	-512,128,0 */
/* asm: 	.float	-512,0,0 */
/* asm: 	.float	-256,128,0 */
/* asm: 	.float	-256,0,0 */
/* asm: 	.float	0,128,0 */
/* asm: 	.float	0,0,0 */
/* asm: 	.float	256,128,0 */
/* asm: 	.float	256,0,0 */
/* asm: 	.float	512,128,0 */
/* asm: 	.float	512,0,0 */
/* asm: 	.float	768,128,0 */
/* asm: 	.float	768,0,0 */
/* asm: 	.float	1024,128,0 */
/* asm: 	.float	1024,0,0 */
/* asm: 	.float	1280,128,0 */
/* asm: 	.float	1280,0,0 */
/* asm: 	.float	1536,128,0 */
/* asm: 	.float	1536,0,0 */
/* asm: 	.float	1792,128,0 */
/* asm: 	.float	1792,0,0 */
/* asm: 	*---------------------------------------------------------------------------- */
int WATERPOS[] = {
    -1280, 128, 0,
    -1280, 0, 0,
    -1024, 128, 0,
    -1024, 0, 0,
    -768, 128, 0,
    -768, 0, 0,
    -512, 128, 0,
    -512, 0, 0,
    -256, 128, 0,
    -256, 0, 0,
    0, 128, 0,
    0, 0, 0,
    256, 128, 0,
    256, 0, 0,
    512, 128, 0,
    512, 0, 0,
    768, 128, 0,
    768, 0, 0,
    1024, 128, 0,
    1024, 0, 0,
    1280, 128, 0,
    1280, 0, 0,
    1536, 128, 0,
    1536, 0, 0,
    1792, 128, 0,
    1792, 0, 0,
};

void FIND_HIGHEST_ROADY(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	PUSH	AR2
    // asm: 	FLOAT	512,R6
    // asm: 	STPF	R6,@HIGHEST_ROADY
    // asm: 	CLRF	R7			;CORRESPONDING Z POS
    // asm: 	LDI	@DYNALIST_BEGIN,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	FHRY_X
FHRYLP:
    // asm: 	LDI	*+AR2(OFLAGS),R0
    // asm: 	AND	O_LIST_M,R0
    // asm: 	CMPI	O_LIST1,R0		;OACTIVE ONLY!
    // asm: 	BNE	NOCHANCE
    // 	;use leftover CAMERAMATRIX
    // 	;
    // asm: 	LDI	@VECTORAI,AR1
    // asm: 	LDP	@_CAMERAPOS+X
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	SUBF	@_CAMERAPOS+X,R0
    // asm: 	STF	R0,*+AR1(X)
    // asm: 	LDF	*+AR2(OPOSY),R0
    // asm: 	SUBF	@_CAMERAPOS+Y,R0
    // asm: 	STF	R0,*+AR1(Y)
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	SUBF	@_CAMERAPOS+Z,R0
    // asm: 	STF	R0,*+AR1(Z)
    // asm: 	SETDP
    // asm: 	LDI	AR1,AR0
    // asm: 	LDI	@CAMERAMATRIXI,AR1	;AR1 IS 3X3 MATRIX
    // 	;matrix mul  really only concerned with Y
    // 	;
    // asm:  	MPYF	*AR0++,*AR1++,R0
    // asm: 	MPYF	*AR0,*AR1++,R2
    // asm: 	MPYF	*+AR0(1),*AR1++,R0
    // asm: 	MPYF	*-AR0(1),*AR1++,R0
    // asm: 	PUSHF	R2
    // asm: 	MPYF	*AR0,*AR1++,R2
    // asm: 	MPYF	*+AR0(1),*AR1++,R0
    // asm: 	ADDF	R0,R2
    // 	;
    // asm: 	POPF	R3
    // asm: 	LDI	*+AR2(ODIST),AR1
    // asm: 	CMPI	0,AR1
    // asm: 	BLE	NOCHANCE
    // asm: 	RS	4,AR1			;/16
    // asm: 	CMPI	HIGH_CLIP_LEVEL,AR1
    // asm: 	BGE	NOCHANCE
    // asm: 	ADDI	@INVTABI,AR1
    // asm: 	MPYF	*AR1,R3
    // asm: 	ADDF	@SCRNHXI,R3		;this is the clip level (in Y)
    // asm: 	CMPF	0,R3
    // asm: 	BLT	NOCHANCE
    // asm: 	FLOAT	512,R0
    // asm: 	CMPF	R0,R3
    // asm: 	BGT	NOCHANCE
    // asm: 	MPYF	*AR1,R2
    // asm: 	ADDF	@SCRNHYI,R2		;this is the clip level (in Y)
    // asm: 	CMPF	R2,R6
    // asm: 	LDFGT	R2,R6
    // asm: 	LDFGT	R3,R7			;SAVE X VALUE
NOCHANCE:
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BNE	FHRYLP
    // ;no kludge factor nessesary
    // asm: 	ADDF	@VAR_ROAD_KFACTOR,R6			;pixels UNDER (overshoot)
    // asm: 	CMPF	0,R6
    // asm: 	LDFLT	0,R6
    // asm: 	STPF	R6,@HIGHEST_ROADY
    // asm: 	STPF	R7,@HIGHEST_ROADY_X	;SAVE X VALUE
FHRY_X:
    // asm: 	POP	AR2
    // asm: 	POP	R7
    // asm: 	POP	R6
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_HIGHEST_ROADY", 0, 0);
    UNIMPL();
}

void INFINITY(void)
{
    // *
    // *
    // *
    // asm: 	LDI	@_MODE,R0
    // asm: 	TSTB	MINTUNNEL,R0
    // asm: 	RETSNZ
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	CALLEQ	FIND_HIGHEST_ROADY
    // 	;KEEP CAMERARAD Y IN RANGE +-2 PI
    // asm: 	LDP	@_CAMERARAD
    // asm: 	LDF	@_CAMERARAD+Y,R2
    // asm: 	SETDP
    // asm: 	BLT	INFF0
    // asm: 	CMPF	@TWOPII,R2
    // asm: 	BLT	INFF1
INFF0:
    // asm: 	SETDP
    // asm: 	CALL	NORMIT		     	;SAVE TRUNCATED VERSION
    // asm: 	LDP	@_CAMERARAD
    // asm: 	STF	R2,@_CAMERARAD+Y
    // asm: 	SETDP
INFF1:
    // asm: 	STF	R2,@CAMRADY
    // 	;
    // 	;FIND HORIZON X OFFSET
    // asm: 	MPYF	@FORMULA,R2
    // asm: 	FIX	R2
    // asm: 	CMPI	@HIGHIVAL,R2
    // asm: 	BLT	OK23
    // asm: 	SUBI	@HIGHIVAL,R2
OK23:
    // asm: 	CMPI	@LOIVAL,R2
    // asm: 	BGT	OK554
    // asm: 	ADDI	@HIGHIVAL,R2
OK554:
    // asm: 	FLOAT	R2,R6		    	;R6 = HORIZON X OFFSET
    // 	;REMOVE Y AXIS ROTATION FROM CAMERA MATRIX
    // 	;
    // 	;
    // asm: 	LDI	@MATRIXAI,AR2		;find_Ymatrix(&MATRIXA, -CAMERARAD.y);
    // asm: 	NEGF	@CAMRADY,R2		;concatmat(&MATRIXA, &CAMERAMATRIX, &MATRIXA);
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	@CAMERAMATRIXI,R2
    // asm: 	CALL	CONCATMAT
    // 	;GET Y HEIGHT OF CLIP ELEMENT
    // asm: 	LDF	@HIGHEST_ROADY,R0
    // asm: 	ADDF	25,R0
    // asm: 	LDF	@INFIN_CORRECT,R1
    // asm: 	ADDF	@SCRNHYI,R1
    // asm: 	SUBF	R1,R0
    // asm: 	LDFLT	0,R0
    // asm: 	LDI	@_MODE,R1
    // asm: 	AND	MMODE,R1
    // asm: 	CMPI	MGAME,R1
    // asm: 	LDFNE	0,R0
    // asm: 	STF	R0,@AMOUNT_CLIPPED
    // 	;
    // 	;a2c =  hsy - hy
    // 	;0 - amount to clip
    // 	;ROTATE INFINITY PLANE COORDS
    // asm: 	LDI	@INFINITY_POINTS,AR5
    // asm: 	LDI	@BLOWLISTI,AR6
    // asm: 	LDI	33,AR4
    // asm: 	CALL	TRANS_PTS
    // asm: 	LDI	sky1_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	LDI	R0,AR2
    // asm: 	LDP	@FIFO_ADDR,AR5
    // asm: 	LS	16,AR5
    // asm: 	LDI	@INFIN_POLYGONSI,AR0
    // asm: 	LDI	0FE00h,R4			;AIVI[0] = 0xff00
    // asm: 	LDI	00000h,R5			;AIVI[1] = 0x3800
    // asm: 	LDI	000FFh,R6			;AIVI[2] = 0x38FF
    // asm: 	LDI	0FEFFh,R7			;AIVI[3] = 0xffFF
    // asm: 	FIX	@AMOUNT_CLIPPED,R3
    // asm: 	LDI	11,AR4
    // asm: 	LDI	@BLOWLISTI,AR6
    // asm: 	LDI	0,AR1				;ML FIX
    // asm: 	LDP	@FIFO_STATUS
LOOP:
    // asm: LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZ	LOOP
    // asm: 	SETDP
    // asm: 	LDI	TM,R0
    // asm: 	STI	R0,*AR5				;ACNTL = 0x900;
    // ;optimize this, its the same palette ALWAYS
    // ;
    // ;
    // asm: 	LDI	*AR0++,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	STI	R0,*AR5       			;ACMAP
    // 	;ORDER:  0,1,3,2
    // asm: 	LDI	@_MODE,R0
    // asm: 	TSTB	MWATER,R0
    // asm: 	BNZ	NOCLIPPING
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	NOCLIPPING
    // 	;
    // 	;clip to horizon plane
    // 	;
    // 	;	A   B
    // 	;	       e
    // 	;	D   C
    // 	;
    // 	;
    // 	;1.  find line equation for DC
    // 	;
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	BK
    // asm: 	LDI	AR6,AR0			;C
    // asm: 	ADDI	9,AR0
    // asm: 	LDI	AR6,AR1			;D
    // asm: 	ADDI	6,AR1
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	CALL	GETLINE_EQ_2D
    // 	;
    // 	;2.  distance to line
    // 	;
    // asm: 	LDI	AR2,AR0
    // asm: 	LDI	@VECTORCI,AR1
    // asm: 	LDF	@HIGHEST_ROADY_X,R0
    // asm: 	STF	R0,*+AR1(X)
    // asm: 	LDF	@HIGHEST_ROADY,R0
    // asm: 	STF	R0,*+AR1(Y)
    // asm: 	CALL	DIST_PT2LINE
    // asm: 	POP	BK
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // 	;
    // 	;3.  abort clipping attempt if no collision exists
    // 	;
    // asm: 	LDF	R0,R0
    // asm: 	BLT	NOCLIPPING
    // asm: 	FIX	R0,R3		;amount to clip off each line
    // 	;
    // 	;compute BA slope
    // 	;
    // asm: 	LDF	*+AR6(3),R0
    // asm: 	SUBF	*+AR6(0),R0
    // asm: 	LDF	*+AR6(4),R1
    // asm: 	SUBF	*+AR6(1),R1
    // asm: 	CMPF	0,R0
    // asm: 	BEQ	II33
    // asm: 	CMPF	0,R1
    // asm: 	BNE	II
II33:
    // asm: CLRF	R0
    // asm: 	BU	III44
II:
    // asm: CALL	DIV_F
III44:
    // asm: FIX	R0,R4
    // asm: 	FIX	*AR6,R0
    // asm: 	SUBI	R4,R0
    // asm: 	STI	R0,*AR5				;ARPS[0][0]
    // asm: 	FIX	*+AR6(1),R0
    // asm: 	SUBI	R3,R0
    // asm: 	STI	R0,*AR5				;ARPS[0][1]
    // asm: 	FIX	*+AR6(3),R0
    // asm: 	STI	R0,*AR5				;ARPS[1][0]
    // asm: 	FIX	*+AR6(4),R0
    // asm: 	STI	R0,*AR5				;ARPS[1][1]
    // asm: 	FIX	*+AR6(9),R0
    // asm: 	STI	R0,*AR5				;ARPS[2][0]
    // asm: 	FIX	*+AR6(10),R0
    // asm: 	STI	R0,*AR5				;ARPS[2][1]
    // asm: 	FIX	*+AR6(6),R0
    // asm: 	SUBI	R4,R0
    // asm: 	STI	R0,*AR5				;ARPS[3][0]
    // asm: 	FIX	*+AR6(7),R0
    // asm: 	SUBI	R3,R0
    // asm: 	STI	R0,*AR5				;ARPS[3][1]
    // asm: 	NOP	*AR6++(6)
    // asm: 	LDI	0FE00h,R4			;AIVI[0] = 0xff00
    // asm: 	LDI	R4,R0
    // asm: 	RS	8,R0
    // asm: 	SUBI	R3,R0
    // asm: 	LDI	R4,R1
    // asm: 	AND	0FFh,R1
    // asm: 	LS	8,R0
    // asm: 	OR	R0,R1
    // asm: 	STI	R1,*AR5				;AIV0
    // asm: 	STI	R5,*AR5				;AIV1
    // asm: 	STI	R6,*AR5				;AIV2
    // asm: 	LDI	R7,R0
    // asm: 	RS	8,R0
    // asm: 	SUBI	R3,R0
    // asm: 	LDI	R7,R1
    // asm: 	AND	0FFh,R1
    // asm: 	LS	8,R0
    // asm: 	OR	R0,R1
    // asm: 	STI	R1,*AR5				;AIV3
    // asm: 	LDI	*AR0++,R0
    // asm: 	STI	R0,*AR5				;ADDR
    // asm: 	LDP	@FIFO_INC
    // asm: 	LDI	@FIFO_INC,R0
    // asm: 	LDI	*AR1,R0	  		;ML FIX
    // asm: 	RPTS	4
    // asm: 	NOP
    // asm: 	SETDP
    // asm: 	DBU	AR4,LOOP
    // asm: 	BU	PLOT_CONSTANTS
    // *
    // *
    // *
NOCLIPPING:
    // 	;DUMP X
    // asm: 	FIX	*AR6,R0
    // asm: 	STI	R0,*AR5				;ARPS[0][0]
    // asm: 	FIX	*+AR6(1),R0
    // asm: 	STI	R0,*AR5				;ARPS[0][1]
    // asm: 	FIX	*+AR6(3),R0
    // asm: 	STI	R0,*AR5				;ARPS[1][0]
    // asm: 	FIX	*+AR6(4),R0
    // asm: 	STI	R0,*AR5				;ARPS[1][1]
    // asm: 	FIX	*+AR6(9),R0
    // asm: 	STI	R0,*AR5				;ARPS[2][0]
    // asm: 	FIX	*+AR6(10),R0
    // asm: 	STI	R0,*AR5				;ARPS[2][1]
    // asm: 	FIX	*+AR6(6),R0
    // asm: 	STI	R0,*AR5				;ARPS[3][0]
    // asm: 	FIX	*+AR6(7),R0
    // asm: 	STI	R0,*AR5				;ARPS[3][1]
    // asm: 	NOP	*AR6++(6)
    // asm: 	STI	R4,*AR5				;AIV0
    // asm: 	STI	R5,*AR5				;AIV1
    // asm: 	STI	R6,*AR5				;AIV2
    // asm: 	STI	R7,*AR5				;AIV3
    // asm: 	LDI	*AR0++,R0
    // asm: 	STI	R0,*AR5				;ADDR
    // asm: 	LDP	@FIFO_INC
    // asm: 	LDI	@FIFO_INC,R0
    // asm: 	LDI	*AR1,R0	  		;ML FIX
    // asm: 	RPTS	4
    // asm: 	NOP
    // asm: 	SETDP
    // asm: 	DBU	AR4,LOOP
PLOT_CONSTANTS:
    // 	;
    // 	;CONSTANT COLOR POLYGONS
    // 	;
    // asm: 	LDI	sky1_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	@INFIN_POLYGONSI,AR0
    // asm: 	LDI	2,AR4
    // asm: 	LDI	@BLOWLISTI,AR6
    // asm: 	ADDI	26*3,AR6
    // asm: 	LDI	0,AR1				;ML FIX
LOOP1A:
    // asm: LDP	@FIFO_STATUS
LOOP1:
    // asm: LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZ	LOOP1
    // asm: 	SETDP
    // asm: 	LDI	30,R0			;CONSTANT COLOR = 1
    // asm: 	STI	R0,*AR5			;ACNTL
    // asm: 	STI	AR2,*AR5   		;ACMAP
    // 	;0,1,3,2
    // 	;DUMP X
    // asm: 	FIX	*AR6,R0
    // asm: 	STI	R0,*AR5			;ARPS[0][0]
    // asm: 	FIX	*+AR6(1),R0
    // asm: 	STI	R0,*AR5			;ARPS[0][1]
    // asm: 	FIX	*+AR6(3),R1
    // asm: 	STI	R1,*AR5			;ARPS[1][0]
    // asm: 	FIX	*+AR6(4),R1
    // asm: 	STI	R1,*AR5			;ARPS[1][1]
    // asm: 	FIX	*+AR6(9),R2
    // asm: 	STI	R2,*AR5			;ARPS[2][0]
    // asm: 	FIX	*+AR6(10),R2
    // asm: 	STI	R2,*AR5			;ARPS[2][1]
    // asm: 	FIX	*+AR6(6),R3
    // asm: 	STI	R3,*AR5			;ARPS[3][0]
    // asm: 	FIX	*+AR6(7),R3
    // asm: 	STI	R3,*AR5			;ARPS[3][1]
    // asm: 	NOP	*AR6++(6)
    // asm: 	STI	R4,*AR5			;AIV0
    // asm: 	STI	R5,*AR5			;AIV1
    // asm: 	STI	R6,*AR5			;AIV2
    // asm: 	STI	R7,*AR5			;AIV3
    // asm: 	LDI	*AR0++,R0
    // asm: 	STI	R0,*AR5			;ADDR
    // asm: 	LDP	@FIFO_INC
    // asm: 	LDI	@FIFO_INC,R0
    // asm: 	LDI	*AR1,R0	  		;ML FIX
    // asm: 	SETDP
    // asm: 	DBU	AR4,LOOP1A
    // asm: 	LDI	@_MODE,R0
    // asm: 	TSTB	MWATER,R0
    // asm: 	CALLNZ	WATER_INFINITY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INFINITY", 0, 0);
    UNIMPL();
}

void TRANS_PTS(void)
{
    // ;	LDF	@AMOUNT_CLIPPED,R4
    // asm: 	LDF	@INFIN_CORRECT,R7
    // ;	FLOAT	8000,R0
    // ;	MPYF	10,R0
    // ;	LDP	@_CAMERARAD
    // ;	LDF	@_CAMERARAD+X,R2
    // ;	SETDP
    // ;
    // ;	CALL	DISTANCE_2D
    // ;	LDLF	0.006401,R2
    // ;	MPYF	R2,R0
    // ;	ADDF	R0,R7
    // ;	;R1 is distance
    // asm: TRANS_LP
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	LDF	*AR5++,R0			;load src [X Y Z]
    // asm: 	ADDF	R6,R0				;add in X offset
    // asm: 	LDP	@HIGHVAL
    // asm: 	CMPF	@HIGHVAL,R0
    // asm: 	BLT	LKJ2
    // asm: 	SUBF	@HIGHVAL,R0
LKJ2:
    // asm: 	STF	R0,*AR2++
    // asm: 	ADDF	R7,*AR5++,R0	 		;ADD IN HEIGHT DUDES !!!
    // asm: 	STF	R0,*AR2++
    // asm: 	LDF	*AR5++,R0
    // asm: 	STF	R0,*AR2--(2)
    // asm: 	LDI	AR6,R3
    // asm: 	LDP	@MATRIXAI
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*AR6,R0
    // asm: 	LDP	@SCRNHXI
    // asm: 	ADDF	@SCRNHXI,R0
    // asm: 	STF	R0,*AR6++
    // asm: 	LDF	*AR6,R0
    // asm: 	MPYF	1.04,R0
    // asm: 	LDP	@SCRNHYI
    // asm: 	ADDF	@SCRNHYI,R0
    // asm: 	STF	R0,*AR6++(2)
    // asm: 	DEC	AR4
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	LDF	*AR5++,R0			;load src [X Y Z]
    // asm: 	ADDF	R6,R0				;add in module offset
    // asm: 	LDP	@HIGHVAL
    // asm: 	CMPF	@HIGHVAL,R0
    // asm: 	BLT	LKJ25
    // asm: 	SUBF	@HIGHVAL,R0
LKJ25:
    // asm: 	STF	R0,*AR2++
    // asm: 	ADDF	R7,*AR5++,R0	 		;ADD IN HEIGHT DUDES !!!
    // asm: 	STF	R0,*AR2++
    // asm: 	LDF	*AR5++,R0
    // asm: 	STF	R0,*AR2--(2)
    // asm: 	LDI	AR6,R3
    // asm: 	LDP	@MATRIXAI
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*AR6,R0
    // asm: 	LDP	@SCRNHXI
    // asm: 	ADDF	@SCRNHXI,R0
    // asm: 	STF	R0,*AR6++
    // asm: 	LDF	*AR6,R0
    // asm: 	MPYF	1.04,R0
    // asm: 	LDP	@SCRNHYI
    // asm: 	ADDF	@SCRNHYI,R0
    // asm: 	STF	R0,*AR6++(2)
    // asm: 	DBU	AR4,TRANS_LP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRANS_PTS", 0, 0);
    UNIMPL();
}

void WATER_INFINITY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DISPLAY THE WATER TO THE SCREEN
    // *
    // *THIS MUST BE CALLED AFTER INFINITY
    // *
    // asm: 	LDF	@CAMRADY,R2
    // 	;FIND HORIZON X OFFSET
    // asm: 	MPYF	@FORMULA,R2
    // asm: 	FIX	R2
    // asm: 	LDP	@HIGHIVAL
    // asm: 	CMPI	@HIGHIVAL,R2
    // asm: 	BLT	ok23a
    // asm: 	SUBI	@HIGHIVAL,R2
ok23a:
    // asm: 	LDP	@LOIVAL
    // asm: 	CMPI	@LOIVAL,R2
    // asm: 	BGT	ok24a
    // asm: 	ADDI	@HIGHIVAL,R2
ok24a:
    // asm: 	FLOAT	R2,R6		    	;R6 = HORIZON X OFFSET
    // 	;REMOVE Y AXIS ROTATION FROM CAMERAERSE MATRIX
    // 	;
    // 	;
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	NEGF	@CAMRADY,R2		;find_Ymatrix(&MATRIXA, -CAMERARAD.y);
    // asm: 	PUSH	AR2			;concatmat(&MATRIXA, &CAMERAMATRIX, &MATRIXA);
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	POP	AR2
    // asm: 	LDI	AR2,R3
    // asm: 	LDP	@CAMERAMATRIXI
    // asm: 	LDI	@CAMERAMATRIXI,R2
    // asm: 	CALL	CONCATMAT
    // 	;GET Y HEIGHT OF INFINITY PLANE
    // asm: 	LDP	@_MATRIXA			;GET HEIGHT OF HORIZON AT INFINITY
    // asm: 	LDF	@_MATRIXA+5,R7
    // asm: 	MPYF	64,R7				;MULTIPLY X 512
    // asm: 	MPYF	8,R7				;A7 = HEIGHT
    // asm: 	LDP	@_CAMERAMATRIX
    // asm: 	LDF	@_CAMERAMATRIX+7,R2  		;GET ZVECT(Y)
    // asm: 	LDF	@_CAMERAMATRIX+4,R1  		;GET YVECT(Y)
    // asm: 	SETDP
    // asm: 	LDP	@INFVAL
    // asm: 	MPYF	@INFVAL,R0
    // asm: 	CALL	DIV_F
    // asm: 	LDP	@INFPROJ
    // asm: 	MPYF	@INFPROJ,R0
    // asm: 	LDF	R0,R7
    // asm: 	NEGF	R7
    // 	;ROTATE INFINITY PLANE COORDS
    // asm: 	LDI	@WATERPOSI,AR5
    // asm: 	LDI	@BLOWLISTI,AR6
    // asm: 	LDI	26,AR4
    // asm: 	CALL	TRANS_PTS
    // asm: 	LDI	wtra_p,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	LDI	R0,AR2
    // asm: 	LDP	@FIFO_ADDR,AR5
    // asm: 	LS	16,AR5
    // asm: 	LDI	0AD00h,R4			;AIVI[0] = 0xff00
    // asm: 	LDI	00100h,R5			;AIVI[1] = 0x3800
    // asm: 	LDI	001FFh,R6			;AIVI[2] = 0x38FF
    // asm: 	LDI	0ADFFh,R7			;AIVI[3] = 0xffFF
    // asm: 	LDI	11,AR4
    // asm: 	LDI	@BLOWLISTI,AR6
    // asm: 	LDP	@FIFO_STATUS
    // asm: 	LDI	0,AR1				;ML FIX (BOGUS READ)
LOOPA:
    // asm: LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZ	LOOPA
    // asm: 	SETDP
    // asm: 	LDI	TM,R0
    // asm: 	STI	R0,*AR5				;ACNTL = 0x900;
    // asm: 	STI	AR2,*AR5       			;ACMAP
    // 	;0,1,3,2
    // 	;DUMP X
    // asm: 	FIX	*AR6,R0
    // asm: 	STI	R0,*AR5				;ARPS[0][0]
    // asm: 	FIX	*+AR6(1),R0
    // asm: 	LDI	400,R0
    // asm: 	STI	R0,*AR5				;ARPS[0][1]
    // asm: 	FIX	*+AR6(3),R0
    // asm: 	STI	R0,*AR5				;ARPS[1][0]
    // asm: 	FIX	*+AR6(4),R0
    // asm: 	STI	R0,*AR5				;ARPS[1][1]
    // asm: 	FIX	*+AR6(9),R0
    // asm: 	STI	R0,*AR5				;ARPS[2][0]
    // asm: 	FIX	*+AR6(10),R0
    // asm: 	STI	R0,*AR5				;ARPS[2][1]
    // asm: 	FIX	*+AR6(6),R0
    // asm: 	STI	R0,*AR5				;ARPS[3][0]
    // asm: 	FIX	*+AR6(7),R0
    // asm: 	LDI	400,R0
    // asm: 	STI	R0,*AR5				;ARPS[3][1]
    // asm: 	NOP	*AR6++(6)
    // asm: 	STI	R4,*AR5				;AIV0
    // asm: 	STI	R5,*AR5				;AIV1
    // asm: 	STI	R6,*AR5				;AIV2
    // asm: 	STI	R7,*AR5				;AIV3
    // asm: 	LDIL	wtra_I,R0
    // asm: 	STI	R0,*AR5				;ADDR
    // asm: 	LDP	@FIFO_INC
    // asm: 	LDI	@FIFO_INC,R0
    // asm: 	LDI	*AR1,R0	  			;ML FIX (BOGUS READ)
    // asm: 	SETDP
    // asm: 	RPTS	4
    // asm: 	NOP
    // asm: 	DBU	AR4,LOOPA
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATER_INFINITY", 0, 0);
    UNIMPL();
}

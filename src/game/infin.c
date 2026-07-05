
#include "infin.h"
#include "../core/machine.h"
#include "globals.h"
#include "macs.h"
#include "obj.h"
#include "pall.h"
#include "sys.h"
#include "vunit.h"

/*
 * Source module: asm/INFIN.ASM
 */

static void FIND_HIGHEST_ROADY(void);
void INFINITY_CUSA(void);
static void TRANS_PTS(void);
static void WATER_INFINITY(void);

#define INFIN_POLYGONSI BLUESKY
#define WATERPOSI WATERPOS

static float* INFINITY_POINTS;
static float INFINPOINTS[102];
static float WATERPOS[78];

/*
 *----------------------------------------------------------------------------
 *
 *
 *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

/* asm: CAMRADY	.bss	CAMRADY,1 */
int CAMRADY;
/*
 *----------------------------------------------------------------------------
 *SEARCH THE ROAD OBJECTS AND FIND THE HIGHEST POSITIONED OBJECT.
 *
 *Note that the case of the player being inverted can not happen because the
 *infinity plane would freak out, and who knows what would happen then, so we
 *assume that this case is not present. Therefore we can simply say the the
 *highest valued Y position of the objects is the highest Y position, and then
 *only that value must be projected in Y.
 *
 *
 */
#define HIGH_CLIP_LEVEL ((5000 - 1)) // ACTUAL # OF ENTRIES
/* asm: HIGHEST_ROADY	.bss	HIGHEST_ROADY,1 */
int HIGHEST_ROADY;
/* asm: HIGHEST_ROADY_X	.bss	HIGHEST_ROADY_X,1 */
int HIGHEST_ROADY_X;
/* asm: VAR_ROAD_KFACTOR	.bss	VAR_ROAD_KFACTOR,1 */
int VAR_ROAD_KFACTOR;

static void FIND_HIGHEST_ROADY(void) {
    // asm 00008212: 	PUSH	R0
    // asm 00008213: 	PUSH	R1
    // asm 00008214: 	PUSH	R2
    // asm 00008215: 	PUSH	R3
    // asm 00008216: 	PUSH	R6
    // asm 00008217: 	PUSH	R7
    // asm 00008218: 	PUSH	AR2
    // asm 00008219: 	FLOAT	512,R6
    // asm 0000821A: 	STPF	R6,@HIGHEST_ROADY
    // asm 0000821B: 	CLRF	R7			;CORRESPONDING Z POS
    // asm 0000821C: 	LDI	@DYNALIST_BEGIN,AR2
    // asm 0000821D: 	CMPI	0,AR2
    // asm 0000821E: 	BEQ	FHRY_X
FHRYLP:
    // asm 0000821F: 	LDI	*+AR2(OFLAGS),R0
    // asm 00008220: 	AND	O_LIST_M,R0
    // asm 00008221: 	CMPI	O_LIST1,R0		;OACTIVE ONLY!
    // asm 00008222: 	BNE	NOCHANCE
    // 	;use leftover CAMERAMATRIX
    // 	;
    // asm 00008223: 	LDI	@VECTORAI,AR1
    // asm 00008224: 	LDP	@_CAMERAPOS+X
    // asm 00008225: 	LDF	*+AR2(OPOSX),R0
    // asm 00008226: 	SUBF	@_CAMERAPOS+X,R0
    // asm 00008227: 	STF	R0,*+AR1(X)
    // asm 00008228: 	LDF	*+AR2(OPOSY),R0
    // asm 00008229: 	SUBF	@_CAMERAPOS+Y,R0
    // asm 0000822A: 	STF	R0,*+AR1(Y)
    // asm 0000822B: 	LDF	*+AR2(OPOSZ),R0
    // asm 0000822C: 	SUBF	@_CAMERAPOS+Z,R0
    // asm 0000822D: 	STF	R0,*+AR1(Z)
    // asm 0000822E: 	SETDP
    // asm 0000822F: 	LDI	AR1,AR0
    // asm 00008230: 	LDI	@CAMERAMATRIXI,AR1	;AR1 IS 3X3 MATRIX
    // 	;matrix mul  really only concerned with Y
    // 	;
    // asm 00008231:  	MPYF	*AR0++,*AR1++,R0
    // asm 00008232: 	MPYF	*AR0,*AR1++,R2
    // asm 00008233: 	MPYF	*+AR0(1),*AR1++,R0
    // asm 00008233:  || 	ADDF	R0,R2
    // asm 00008234: 	MPYF	*-AR0(1),*AR1++,R0
    // asm 00008234:  ||	ADDF	R0,R2
    // asm 00008235: 	PUSHF	R2
    // asm 00008236: 	MPYF	*AR0,*AR1++,R2
    // asm 00008237: 	MPYF	*+AR0(1),*AR1++,R0
    // asm 00008237:  || 	ADDF	R0,R2
    // asm 00008238: 	ADDF	R0,R2
    // 	;
    // asm 00008239: 	POPF	R3
    // asm 0000823A: 	LDI	*+AR2(ODIST),AR1
    // asm 0000823B: 	CMPI	0,AR1
    // asm 0000823C: 	BLE	NOCHANCE
    // asm 0000823D: 	RS	4,AR1			;/16
    // asm 0000823E: 	CMPI	HIGH_CLIP_LEVEL,AR1
    // asm 0000823F: 	BGE	NOCHANCE
    // asm 00008240: 	ADDI	@INVTABI,AR1
    // asm 00008241: 	MPYF	*AR1,R3
    // asm 00008242: 	ADDF	@SCRNHXI,R3		;this is the clip level (in Y)
    // asm 00008243: 	CMPF	0,R3
    // asm 00008244: 	BLT	NOCHANCE
    // asm 00008245: 	FLOAT	512,R0
    // asm 00008246: 	CMPF	R0,R3
    // asm 00008247: 	BGT	NOCHANCE
    // asm 00008248: 	MPYF	*AR1,R2
    // asm 00008249: 	ADDF	@SCRNHYI,R2		;this is the clip level (in Y)
    // asm 0000824A: 	CMPF	R2,R6
    // asm 0000824B: 	LDFGT	R2,R6
    // asm 0000824C: 	LDFGT	R3,R7			;SAVE X VALUE
NOCHANCE:
    // asm 0000824D: 	LDI	*+AR2(OLINK4),AR2
    // asm 0000824E: 	CMPI	0,AR2
    // asm 0000824F: 	BNE	FHRYLP
    // ;no kludge factor nessesary
    // asm 00008250: 	ADDF	@VAR_ROAD_KFACTOR,R6			;pixels UNDER (overshoot)
    // asm 00008251: 	CMPF	0,R6
    // asm 00008252: 	LDFLT	0,R6
    // asm 00008253: 	STPF	R6,@HIGHEST_ROADY
    // asm 00008254: 	STPF	R7,@HIGHEST_ROADY_X	;SAVE X VALUE
FHRY_X:
    // asm 00008255: 	POP	AR2
    // asm 00008256: 	POP	R7
    // asm 00008257: 	POP	R6
    // asm 00008258: 	POP	R3
    // asm 00008259: 	POP	R2
    // asm 0000825A: 	POP	R1
    // asm 0000825B: 	POP	R0
    // asm 0000825C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_HIGHEST_ROADY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PLOT INFINITY PLANE
 *
 *VERTICES TRANSFORMED AS
 *     1	 3  5
 *
 *     0	 2  4  etc.
 *
 *
 */
/* asm: AMOUNT_CLIPPED	.bss	AMOUNT_CLIPPED,1 */
int AMOUNT_CLIPPED;
/* asm: FORMULA		.float	-244.4619926	;(6*256)/2PI  (convert radians to length of infinity plane) */
/* asm: 	 */
static float FORMULA = -244.4619926f;
/* asm: LOWVAL		.float	-1536 */
static float LOWVAL = -1536.0f;
/* asm: HIGHVAL		.float	1536 */
/* asm: 	 */
static float HIGHVAL = 1536.0f;
/* asm: LOIVAL		.word	-768 */
static int LOIVAL = -768;
/* asm: HIGHIVAL	.word	1536 */
/* asm: 	 */
static int HIGHIVAL = 1536;
/* asm: INFPROJ		.float	0.0064 */
static float INFPROJ = 0.0064f;
/* asm: INFVAL		.float  80000 */
static float INFVAL = 80000.0f;

/*
 *
 *
 *
 */
void INFINITY_CUSA(void) {
    // asm 00008264: 	LDI	@_MODE,R0
    // asm 00008265: 	TSTB	MINTUNNEL,R0
    // asm 00008266: 	RETSNZ
    // asm 00008267: 	AND	MMODE,R0
    // asm 00008268: 	CMPI	MGAME,R0
    // asm 00008269: 	CALLEQ	FIND_HIGHEST_ROADY
    // 	;KEEP CAMERARAD Y IN RANGE +-2 PI
    // asm 0000826A: 	LDP	@_CAMERARAD
    // asm 0000826B: 	LDF	@_CAMERARAD+Y,R2
    // asm 0000826C: 	SETDP
    // asm 0000826D: 	BLT	INFF0
    // asm 0000826E: 	CMPF	@TWOPII,R2
    // asm 0000826F: 	BLT	INFF1
INFF0:
    // asm 00008270: 	SETDP
    // asm 00008271: 	CALL	NORMIT		     	;SAVE TRUNCATED VERSION
    // asm 00008272: 	LDP	@_CAMERARAD
    // asm 00008273: 	STF	R2,@_CAMERARAD+Y
    // asm 00008274: 	SETDP
INFF1:
    // asm 00008275: 	STF	R2,@CAMRADY
    // 	;
    // 	;FIND HORIZON X OFFSET
    // asm 00008276: 	MPYF	@FORMULA,R2
    // asm 00008277: 	FIX	R2
    // asm 00008278: 	CMPI	@HIGHIVAL,R2
    // asm 00008279: 	BLT	OK23
    // asm 0000827A: 	SUBI	@HIGHIVAL,R2
OK23:
    // asm 0000827B: 	CMPI	@LOIVAL,R2
    // asm 0000827C: 	BGT	OK554
    // asm 0000827D: 	ADDI	@HIGHIVAL,R2
OK554:
    // asm 0000827E: 	FLOAT	R2,R6		    	;R6 = HORIZON X OFFSET
    // 	;REMOVE Y AXIS ROTATION FROM CAMERA MATRIX
    // 	;
    // 	;
    // asm 0000827F: 	LDI	@MATRIXAI,AR2		;find_Ymatrix(&MATRIXA, -CAMERARAD.y);
    // asm 00008280: 	NEGF	@CAMRADY,R2		;concatmat(&MATRIXA, &CAMERAMATRIX, &MATRIXA);
    // asm 00008281: 	CALL	FIND_YMATRIX
    // asm 00008282: 	LDI	AR2,R3
    // asm 00008283: 	LDI	@CAMERAMATRIXI,R2
    // asm 00008284: 	CALL	CONCATMAT
    // 	;GET Y HEIGHT OF CLIP ELEMENT
    // asm 00008285: 	LDF	@HIGHEST_ROADY,R0
    // asm 00008286: 	ADDF	25,R0
    // asm 00008287: 	LDF	@INFIN_CORRECT,R1
    // asm 00008288: 	ADDF	@SCRNHYI,R1
    // asm 00008289: 	SUBF	R1,R0
    // asm 0000828A: 	LDFLT	0,R0
    // asm 0000828B: 	LDI	@_MODE,R1
    // asm 0000828C: 	AND	MMODE,R1
    // asm 0000828D: 	CMPI	MGAME,R1
    // asm 0000828E: 	LDFNE	0,R0
    // asm 0000828F: 	STF	R0,@AMOUNT_CLIPPED
    // 	;
    // 	;a2c =  hsy - hy
    // 	;0 - amount to clip
    // 	;ROTATE INFINITY PLANE COORDS
    // asm 00008290: 	LDI	@INFINITY_POINTS,AR5
    // asm 00008291: 	LDI	@BLOWLISTI,AR6
    // asm 00008292: 	LDI	33,AR4
    // asm 00008293: 	CALL	TRANS_PTS
    // asm 00008294: 	LDI	sky1_p,AR2
    // asm 00008295: 	CALL	PAL_FIND
    // asm 00008296: 	LDI	R0,AR2
    // asm 00008297: 	LDP	@FIFO_ADDR,AR5
    // asm 00008298: 	LS	16,AR5
    // asm 00008299: 	LDI	@INFIN_POLYGONSI,AR0
    // asm 0000829A: 	LDI	0FE00h,R4			;AIVI[0] = 0xff00
    // asm 0000829B: 	LDI	00000h,R5			;AIVI[1] = 0x3800
    // asm 0000829C: 	LDI	000FFh,R6			;AIVI[2] = 0x38FF
    // asm 0000829D: 	LDI	0FEFFh,R7			;AIVI[3] = 0xffFF
    // asm 0000829E: 	FIX	@AMOUNT_CLIPPED,R3
    // asm 0000829F: 	LDI	11,AR4
    // asm 000082A0: 	LDI	@BLOWLISTI,AR6
    // asm 000082A1: 	LDI	0,AR1				;ML FIX
    // asm 000082A2: 	LDP	@FIFO_STATUS
LOOP:
    // asm 000082A3: LDI	@FIFO_STATUS,R0
    // asm 000082A4: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm 000082A5: 	BNZ	LOOP
    // asm 000082A6: 	SETDP
    // asm 000082A7: 	LDI	TM,R0
    // asm 000082A8: 	STI	R0,*AR5				;ACNTL = 0x900;
    // ;optimize this, its the same palette ALWAYS
    // ;
    // ;
    // asm 000082A9: 	LDI	*AR0++,AR2
    // asm 000082AA: 	CALL	PAL_FIND
    // asm 000082AB: 	STI	R0,*AR5       			;ACMAP
    // 	;ORDER:  0,1,3,2
    // asm 000082AC: 	LDI	@_MODE,R0
    // asm 000082AD: 	TSTB	MWATER,R0
    // asm 000082AE: 	BNZ	NOCLIPPING
    // asm 000082AF: 	AND	MMODE,R0
    // asm 000082B0: 	CMPI	MGAME,R0
    // asm 000082B1: 	BNE	NOCLIPPING
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
    // asm 000082B2: 	PUSH	AR0
    // asm 000082B3: 	PUSH	AR1
    // asm 000082B4: 	PUSH	BK
    // asm 000082B5: 	LDI	AR6,AR0			;C
    // asm 000082B6: 	ADDI	9,AR0
    // asm 000082B7: 	LDI	AR6,AR1			;D
    // asm 000082B8: 	ADDI	6,AR1
    // asm 000082B9: 	LDI	@VECTORAI,AR2
    // asm 000082BA: 	CALL	GETLINE_EQ_2D
    // 	;
    // 	;2.  distance to line
    // 	;
    // asm 000082BB: 	LDI	AR2,AR0
    // asm 000082BC: 	LDI	@VECTORCI,AR1
    // asm 000082BD: 	LDF	@HIGHEST_ROADY_X,R0
    // asm 000082BE: 	STF	R0,*+AR1(X)
    // asm 000082BF: 	LDF	@HIGHEST_ROADY,R0
    // asm 000082C0: 	STF	R0,*+AR1(Y)
    // asm 000082C1: 	CALL	DIST_PT2LINE
    // asm 000082C2: 	POP	BK
    // asm 000082C3: 	POP	AR1
    // asm 000082C4: 	POP	AR0
    // 	;
    // 	;3.  abort clipping attempt if no collision exists
    // 	;
    // asm 000082C5: 	LDF	R0,R0
    // asm 000082C6: 	BLT	NOCLIPPING
    // asm 000082C7: 	FIX	R0,R3		;amount to clip off each line
    // 	;
    // 	;compute BA slope
    // 	;
    // asm 000082C8: 	LDF	*+AR6(3),R0
    // asm 000082C9: 	SUBF	*+AR6(0),R0
    // asm 000082CA: 	LDF	*+AR6(4),R1
    // asm 000082CB: 	SUBF	*+AR6(1),R1
    // asm 000082CC: 	CMPF	0,R0
    // asm 000082CD: 	BEQ	II33
    // asm 000082CE: 	CMPF	0,R1
    // asm 000082CF: 	BNE	II
II33:
    // asm 000082D0: CLRF	R0
    // asm 000082D1: 	BU	III44
II:
    // asm 000082D2: CALL	DIV_F
III44:
    // asm 000082D3: FIX	R0,R4
    // asm 000082D4: 	FIX	*AR6,R0
    // asm 000082D5: 	SUBI	R4,R0
    // asm 000082D6: 	STI	R0,*AR5				;ARPS[0][0]
    // asm 000082D7: 	FIX	*+AR6(1),R0
    // asm 000082D8: 	SUBI	R3,R0
    // asm 000082D9: 	STI	R0,*AR5				;ARPS[0][1]
    // asm 000082DA: 	FIX	*+AR6(3),R0
    // asm 000082DB: 	STI	R0,*AR5				;ARPS[1][0]
    // asm 000082DC: 	FIX	*+AR6(4),R0
    // asm 000082DD: 	STI	R0,*AR5				;ARPS[1][1]
    // asm 000082DE: 	FIX	*+AR6(9),R0
    // asm 000082DF: 	STI	R0,*AR5				;ARPS[2][0]
    // asm 000082E0: 	FIX	*+AR6(10),R0
    // asm 000082E1: 	STI	R0,*AR5				;ARPS[2][1]
    // asm 000082E2: 	FIX	*+AR6(6),R0
    // asm 000082E3: 	SUBI	R4,R0
    // asm 000082E4: 	STI	R0,*AR5				;ARPS[3][0]
    // asm 000082E5: 	FIX	*+AR6(7),R0
    // asm 000082E6: 	SUBI	R3,R0
    // asm 000082E7: 	STI	R0,*AR5				;ARPS[3][1]
    // asm 000082E8: 	NOP	*AR6++(6)
    // asm 000082E9: 	LDI	0FE00h,R4			;AIVI[0] = 0xff00
    // asm 000082EA: 	LDI	R4,R0
    // asm 000082EB: 	RS	8,R0
    // asm 000082EC: 	SUBI	R3,R0
    // asm 000082ED: 	LDI	R4,R1
    // asm 000082EE: 	AND	0FFh,R1
    // asm 000082EF: 	LS	8,R0
    // asm 000082F0: 	OR	R0,R1
    // asm 000082F1: 	STI	R1,*AR5				;AIV0
    // asm 000082F2: 	STI	R5,*AR5				;AIV1
    // asm 000082F3: 	STI	R6,*AR5				;AIV2
    // asm 000082F4: 	LDI	R7,R0
    // asm 000082F5: 	RS	8,R0
    // asm 000082F6: 	SUBI	R3,R0
    // asm 000082F7: 	LDI	R7,R1
    // asm 000082F8: 	AND	0FFh,R1
    // asm 000082F9: 	LS	8,R0
    // asm 000082FA: 	OR	R0,R1
    // asm 000082FB: 	STI	R1,*AR5				;AIV3
    // asm 000082FC: 	LDI	*AR0++,R0
    // asm 000082FD: 	STI	R0,*AR5				;ADDR
    // asm 000082FE: 	LDP	@FIFO_INC
    // asm 000082FF: 	LDI	@FIFO_INC,R0
    // asm 00008300: 	LDI	*AR1,R0	  		;ML FIX
    // asm 00008301: 	RPTS	4
    // asm 00008302: 	NOP
    // asm 00008303: 	SETDP
    // asm 00008304: 	DBU	AR4,LOOP
    // asm 00008305: 	BU	PLOT_CONSTANTS
    // *
    // *
    // *
NOCLIPPING:
    // 	;DUMP X
    // asm 00008306: 	FIX	*AR6,R0
    // asm 00008307: 	STI	R0,*AR5				;ARPS[0][0]
    // asm 00008308: 	FIX	*+AR6(1),R0
    // asm 00008309: 	STI	R0,*AR5				;ARPS[0][1]
    // asm 0000830A: 	FIX	*+AR6(3),R0
    // asm 0000830B: 	STI	R0,*AR5				;ARPS[1][0]
    // asm 0000830C: 	FIX	*+AR6(4),R0
    // asm 0000830D: 	STI	R0,*AR5				;ARPS[1][1]
    // asm 0000830E: 	FIX	*+AR6(9),R0
    // asm 0000830F: 	STI	R0,*AR5				;ARPS[2][0]
    // asm 00008310: 	FIX	*+AR6(10),R0
    // asm 00008311: 	STI	R0,*AR5				;ARPS[2][1]
    // asm 00008312: 	FIX	*+AR6(6),R0
    // asm 00008313: 	STI	R0,*AR5				;ARPS[3][0]
    // asm 00008314: 	FIX	*+AR6(7),R0
    // asm 00008315: 	STI	R0,*AR5				;ARPS[3][1]
    // asm 00008316: 	NOP	*AR6++(6)
    // asm 00008317: 	STI	R4,*AR5				;AIV0
    // asm 00008318: 	STI	R5,*AR5				;AIV1
    // asm 00008319: 	STI	R6,*AR5				;AIV2
    // asm 0000831A: 	STI	R7,*AR5				;AIV3
    // asm 0000831B: 	LDI	*AR0++,R0
    // asm 0000831C: 	STI	R0,*AR5				;ADDR
    // asm 0000831D: 	LDP	@FIFO_INC
    // asm 0000831E: 	LDI	@FIFO_INC,R0
    // asm 0000831F: 	LDI	*AR1,R0	  		;ML FIX
    // asm 00008320: 	RPTS	4
    // asm 00008321: 	NOP
    // asm 00008322: 	SETDP
    // asm 00008323: 	DBU	AR4,LOOP
PLOT_CONSTANTS:
    // 	;
    // 	;CONSTANT COLOR POLYGONS
    // 	;
    // asm 00008324: 	LDI	sky1_p,AR2
    // asm 00008325: 	CALL	PAL_FIND
    // asm 00008326: 	LDI	R0,AR2
    // asm 00008327: 	LDI	@INFIN_POLYGONSI,AR0
    // asm 00008328: 	LDI	2,AR4
    // asm 00008329: 	LDI	@BLOWLISTI,AR6
    // asm 0000832A: 	ADDI	26*3,AR6
    // asm 0000832B: 	LDI	0,AR1				;ML FIX
LOOP1A:
    // asm 0000832C: LDP	@FIFO_STATUS
LOOP1:
    // asm 0000832D: LDI	@FIFO_STATUS,R0
    // asm 0000832E: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm 0000832F: 	BNZ	LOOP1
    // asm 00008330: 	SETDP
    // asm 00008331: 	LDI	30,R0			;CONSTANT COLOR = 1
    // asm 00008332: 	STI	R0,*AR5			;ACNTL
    // asm 00008333: 	STI	AR2,*AR5   		;ACMAP
    // 	;0,1,3,2
    // 	;DUMP X
    // asm 00008334: 	FIX	*AR6,R0
    // asm 00008335: 	STI	R0,*AR5			;ARPS[0][0]
    // asm 00008336: 	FIX	*+AR6(1),R0
    // asm 00008337: 	STI	R0,*AR5			;ARPS[0][1]
    // asm 00008338: 	FIX	*+AR6(3),R1
    // asm 00008339: 	STI	R1,*AR5			;ARPS[1][0]
    // asm 0000833A: 	FIX	*+AR6(4),R1
    // asm 0000833B: 	STI	R1,*AR5			;ARPS[1][1]
    // asm 0000833C: 	FIX	*+AR6(9),R2
    // asm 0000833D: 	STI	R2,*AR5			;ARPS[2][0]
    // asm 0000833E: 	FIX	*+AR6(10),R2
    // asm 0000833F: 	STI	R2,*AR5			;ARPS[2][1]
    // asm 00008340: 	FIX	*+AR6(6),R3
    // asm 00008341: 	STI	R3,*AR5			;ARPS[3][0]
    // asm 00008342: 	FIX	*+AR6(7),R3
    // asm 00008343: 	STI	R3,*AR5			;ARPS[3][1]
    // asm 00008344: 	NOP	*AR6++(6)
    // asm 00008345: 	STI	R4,*AR5			;AIV0
    // asm 00008346: 	STI	R5,*AR5			;AIV1
    // asm 00008347: 	STI	R6,*AR5			;AIV2
    // asm 00008348: 	STI	R7,*AR5			;AIV3
    // asm 00008349: 	LDI	*AR0++,R0
    // asm 0000834A: 	STI	R0,*AR5			;ADDR
    // asm 0000834B: 	LDP	@FIFO_INC
    // asm 0000834C: 	LDI	@FIFO_INC,R0
    // asm 0000834D: 	LDI	*AR1,R0	  		;ML FIX
    // asm 0000834E: 	SETDP
    // asm 0000834F: 	DBU	AR4,LOOP1A
    // asm 00008350: 	LDI	@_MODE,R0
    // asm 00008351: 	TSTB	MWATER,R0
    // asm 00008352: 	CALLNZ	WATER_INFINITY
    // asm 00008353: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INFINITY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: INFINITY_POINTS	.word	INFINPOINTS */
#define INFINITY_POINTS INFINPOINTS
/* asm: INFINPOINTS */
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
/* asm: 	 */
/* asm: 	 */
/* asm: 	.float	-1280,-253,0 */
/* asm: 	.float	-1280,-1250,0 */
/* asm: 	 */
/* asm: 	.float	-300,-253,0 */
/* asm: 	.float	-300,-1250,0 */
/* asm: 	 */
/* asm: 	.float	700,-253,0 */
/* asm: 	.float	700,-1250,0 */
/* asm: 	 */
/* asm: 	.float	1700,-253,0 */
/* asm: 	.float	1700,-1250,0 */
/* asm: 	 */
/* asm: 	 */
static float INFINPOINTS[] = {
    -1280.0f,
    0.0f,
    0.0f,
    -1280.0f,
    -255.0f,
    0.0f,
    -1024.0f,
    0.0f,
    0.0f,
    -1024.0f,
    -255.0f,
    0.0f,
    -768.0f,
    0.0f,
    0.0f,
    -768.0f,
    -255.0f,
    0.0f,
    -512.0f,
    0.0f,
    0.0f,
    -512.0f,
    -255.0f,
    0.0f,
    -256.0f,
    0.0f,
    0.0f,
    -256.0f,
    -255.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    -255.0f,
    0.0f,
    256.0f,
    0.0f,
    0.0f,
    256.0f,
    -255.0f,
    0.0f,
    512.0f,
    0.0f,
    0.0f,
    512.0f,
    -255.0f,
    0.0f,
    768.0f,
    0.0f,
    0.0f,
    768.0f,
    -255.0f,
    0.0f,
    1024.0f,
    0.0f,
    0.0f,
    1024.0f,
    -255.0f,
    0.0f,
    1280.0f,
    0.0f,
    0.0f,
    1280.0f,
    -255.0f,
    0.0f,
    1536.0f,
    0.0f,
    0.0f,
    1536.0f,
    -255.0f,
    0.0f,
    1792.0f,
    0.0f,
    0.0f,
    1792.0f,
    -255.0f,
    0.0f,
    -1280.0f,
    -253.0f,
    0.0f,
    -1280.0f,
    -1250.0f,
    0.0f,
    -300.0f,
    -253.0f,
    0.0f,
    -300.0f,
    -1250.0f,
    0.0f,
    700.0f,
    -253.0f,
    0.0f,
    700.0f,
    -1250.0f,
    0.0f,
    1700.0f,
    -253.0f,
    0.0f,
    1700.0f,
    -1250.0f,
    0.0f,
};
/* asm: INFIN_POLYGONSI	.word	BLUESKY */
#define INFIN_POLYGONSI BLUESKY
/* asm: BLUESKY	.word	sky1_p,sky1_I,sky1_p,sky2_I,sky1_p,sky3_I,sky1_p,sky4_I,sky1_p,sky5_I,sky1_p,sky6_I */
/* asm: 	.word	sky1_p,sky1_I,sky1_p,sky2_I,sky1_p,sky3_I,sky1_p,sky4_I,sky1_p,sky5_I,sky1_p,sky6_I */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
static int BLUESKY[] = {
    sky1_p,
    sky1_I,
    sky1_p,
    sky2_I,
    sky1_p,
    sky3_I,
    sky1_p,
    sky4_I,
    sky1_p,
    sky5_I,
    sky1_p,
    sky6_I,
    sky1_p,
    sky1_I,
    sky1_p,
    sky2_I,
    sky1_p,
    sky3_I,
    sky1_p,
    sky4_I,
    sky1_p,
    sky5_I,
    sky1_p,
    sky6_I,
    // ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------
    //
    //
    // PARAMETERS
    // 	AR4	NUMBER OF VERTICES
    // 	AR5	POINTERS [X Y Z]
    // 	AR6	RAM BUFFER
    //
    //
    //
};
/* asm: INFIN_CORRECT	.bss	INFIN_CORRECT,1 */
int INFIN_CORRECT;

static void TRANS_PTS(void) {
    // ;	LDF	@AMOUNT_CLIPPED,R4
    // asm 000083D4: 	LDF	@INFIN_CORRECT,R7
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
    // asm 000083D5: TRANS_LP
    // asm 000083D5: 	LDI	@VECTORAI,AR2
    // asm 000083D6: 	LDF	*AR5++,R0			;load src [X Y Z]
    // asm 000083D7: 	ADDF	R6,R0				;add in X offset
    // asm 000083D8: 	LDP	@HIGHVAL
    // asm 000083D9: 	CMPF	@HIGHVAL,R0
    // asm 000083DA: 	BLT	LKJ2
    // asm 000083DB: 	SUBF	@HIGHVAL,R0
LKJ2:
    // asm 000083DC: 	STF	R0,*AR2++
    // asm 000083DD: 	ADDF	R7,*AR5++,R0	 		;ADD IN HEIGHT DUDES !!!
    // asm 000083DE: 	STF	R0,*AR2++
    // asm 000083DF: 	LDF	*AR5++,R0
    // asm 000083E0: 	STF	R0,*AR2--(2)
    // asm 000083E1: 	LDI	AR6,R3
    // asm 000083E2: 	LDP	@MATRIXAI
    // asm 000083E3: 	LDI	@MATRIXAI,R2
    // asm 000083E4: 	CALL	MATRIX_MUL
    // asm 000083E5: 	LDF	*AR6,R0
    // asm 000083E6: 	LDP	@SCRNHXI
    // asm 000083E7: 	ADDF	@SCRNHXI,R0
    // asm 000083E8: 	STF	R0,*AR6++
    // asm 000083E9: 	LDF	*AR6,R0
    // asm 000083EA: 	MPYF	1.04,R0
    // asm 000083EB: 	LDP	@SCRNHYI
    // asm 000083EC: 	ADDF	@SCRNHYI,R0
    // asm 000083ED: 	STF	R0,*AR6++(2)
    // asm 000083EE: 	DEC	AR4
    // asm 000083EF: 	LDI	@VECTORAI,AR2
    // asm 000083F0: 	LDF	*AR5++,R0			;load src [X Y Z]
    // asm 000083F1: 	ADDF	R6,R0				;add in module offset
    // asm 000083F2: 	LDP	@HIGHVAL
    // asm 000083F3: 	CMPF	@HIGHVAL,R0
    // asm 000083F4: 	BLT	LKJ25
    // asm 000083F5: 	SUBF	@HIGHVAL,R0
LKJ25:
    // asm 000083F6: 	STF	R0,*AR2++
    // asm 000083F7: 	ADDF	R7,*AR5++,R0	 		;ADD IN HEIGHT DUDES !!!
    // asm 000083F8: 	STF	R0,*AR2++
    // asm 000083F9: 	LDF	*AR5++,R0
    // asm 000083FA: 	STF	R0,*AR2--(2)
    // asm 000083FB: 	LDI	AR6,R3
    // asm 000083FC: 	LDP	@MATRIXAI
    // asm 000083FD: 	LDI	@MATRIXAI,R2
    // asm 000083FE: 	CALL	MATRIX_MUL
    // asm 000083FF: 	LDF	*AR6,R0
    // asm 00008400: 	LDP	@SCRNHXI
    // asm 00008401: 	ADDF	@SCRNHXI,R0
    // asm 00008402: 	STF	R0,*AR6++
    // asm 00008403: 	LDF	*AR6,R0
    // asm 00008404: 	MPYF	1.04,R0
    // asm 00008405: 	LDP	@SCRNHYI
    // asm 00008406: 	ADDF	@SCRNHYI,R0
    // asm 00008407: 	STF	R0,*AR6++(2)
    // asm 00008408: 	DBU	AR4,TRANS_LP
    // asm 00008409: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRANS_PTS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DISPLAY THE WATER TO THE SCREEN
 *
 *THIS MUST BE CALLED AFTER INFINITY
 *
 */
static void WATER_INFINITY(void) {
    // asm 0000840A: 	LDF	@CAMRADY,R2
    // 	;FIND HORIZON X OFFSET
    // asm 0000840B: 	MPYF	@FORMULA,R2
    // asm 0000840C: 	FIX	R2
    // asm 0000840D: 	LDP	@HIGHIVAL
    // asm 0000840E: 	CMPI	@HIGHIVAL,R2
    // asm 0000840F: 	BLT	ok23a
    // asm 00008410: 	SUBI	@HIGHIVAL,R2
ok23a:
    // asm 00008411: 	LDP	@LOIVAL
    // asm 00008412: 	CMPI	@LOIVAL,R2
    // asm 00008413: 	BGT	ok24a
    // asm 00008414: 	ADDI	@HIGHIVAL,R2
ok24a:
    // asm 00008415: 	FLOAT	R2,R6		    	;R6 = HORIZON X OFFSET
    // 	;REMOVE Y AXIS ROTATION FROM CAMERAERSE MATRIX
    // 	;
    // 	;
    // asm 00008416: 	LDI	@MATRIXAI,AR2
    // asm 00008417: 	NEGF	@CAMRADY,R2		;find_Ymatrix(&MATRIXA, -CAMERARAD.y);
    // asm 00008418: 	PUSH	AR2			;concatmat(&MATRIXA, &CAMERAMATRIX, &MATRIXA);
    // asm 00008419: 	CALL	FIND_YMATRIX
    // asm 0000841A: 	POP	AR2
    // asm 0000841B: 	LDI	AR2,R3
    // asm 0000841C: 	LDP	@CAMERAMATRIXI
    // asm 0000841D: 	LDI	@CAMERAMATRIXI,R2
    // asm 0000841E: 	CALL	CONCATMAT
    // 	;GET Y HEIGHT OF INFINITY PLANE
    // asm 0000841F: 	LDP	@_MATRIXA			;GET HEIGHT OF HORIZON AT INFINITY
    // asm 00008420: 	LDF	@_MATRIXA+5,R7
    // asm 00008421: 	MPYF	64,R7				;MULTIPLY X 512
    // asm 00008422: 	MPYF	8,R7				;A7 = HEIGHT
    // asm 00008423: 	LDP	@_CAMERAMATRIX
    // asm 00008424: 	LDF	@_CAMERAMATRIX+7,R2  		;GET ZVECT(Y)
    // asm 00008425: 	LDF	@_CAMERAMATRIX+4,R1  		;GET YVECT(Y)
    // asm 00008426: 	SETDP
    // asm 00008427: 	LDP	@INFVAL
    // asm 00008428: 	MPYF	@INFVAL,R0
    // asm 00008429: 	CALL	DIV_F
    // asm 0000842A: 	LDP	@INFPROJ
    // asm 0000842B: 	MPYF	@INFPROJ,R0
    // asm 0000842C: 	LDF	R0,R7
    // asm 0000842D: 	NEGF	R7
    // 	;ROTATE INFINITY PLANE COORDS
    // asm 0000842E: 	LDI	@WATERPOSI,AR5
    // asm 0000842F: 	LDI	@BLOWLISTI,AR6
    // asm 00008430: 	LDI	26,AR4
    // asm 00008431: 	CALL	TRANS_PTS
    // asm 00008432: 	LDI	wtra_p,AR2
    // asm 00008433: 	CALL	PAL_FIND
    // asm 00008434: 	LDI	R0,AR2
    // asm 00008435: 	LDP	@FIFO_ADDR,AR5
    // asm 00008436: 	LS	16,AR5
    // asm 00008437: 	LDI	0AD00h,R4			;AIVI[0] = 0xff00
    // asm 00008438: 	LDI	00100h,R5			;AIVI[1] = 0x3800
    // asm 00008439: 	LDI	001FFh,R6			;AIVI[2] = 0x38FF
    // asm 0000843A: 	LDI	0ADFFh,R7			;AIVI[3] = 0xffFF
    // asm 0000843B: 	LDI	11,AR4
    // asm 0000843C: 	LDI	@BLOWLISTI,AR6
    // asm 0000843D: 	LDP	@FIFO_STATUS
    // asm 0000843E: 	LDI	0,AR1				;ML FIX (BOGUS READ)
LOOPA:
    // asm 0000843F: LDI	@FIFO_STATUS,R0
    // asm 00008440: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm 00008441: 	BNZ	LOOPA
    // asm 00008442: 	SETDP
    // asm 00008443: 	LDI	TM,R0
    // asm 00008444: 	STI	R0,*AR5				;ACNTL = 0x900;
    // asm 00008445: 	STI	AR2,*AR5       			;ACMAP
    // 	;0,1,3,2
    // 	;DUMP X
    // asm 00008446: 	FIX	*AR6,R0
    // asm 00008447: 	STI	R0,*AR5				;ARPS[0][0]
    // asm 00008448: 	FIX	*+AR6(1),R0
    // asm 00008449: 	LDI	400,R0
    // asm 0000844A: 	STI	R0,*AR5				;ARPS[0][1]
    // asm 0000844B: 	FIX	*+AR6(3),R0
    // asm 0000844C: 	STI	R0,*AR5				;ARPS[1][0]
    // asm 0000844D: 	FIX	*+AR6(4),R0
    // asm 0000844E: 	STI	R0,*AR5				;ARPS[1][1]
    // asm 0000844F: 	FIX	*+AR6(9),R0
    // asm 00008450: 	STI	R0,*AR5				;ARPS[2][0]
    // asm 00008451: 	FIX	*+AR6(10),R0
    // asm 00008452: 	STI	R0,*AR5				;ARPS[2][1]
    // asm 00008453: 	FIX	*+AR6(6),R0
    // asm 00008454: 	STI	R0,*AR5				;ARPS[3][0]
    // asm 00008455: 	FIX	*+AR6(7),R0
    // asm 00008456: 	LDI	400,R0
    // asm 00008457: 	STI	R0,*AR5				;ARPS[3][1]
    // asm 00008458: 	NOP	*AR6++(6)
    // asm 00008459: 	STI	R4,*AR5				;AIV0
    // asm 0000845A: 	STI	R5,*AR5				;AIV1
    // asm 0000845B: 	STI	R6,*AR5				;AIV2
    // asm 0000845C: 	STI	R7,*AR5				;AIV3
    // asm 0000845D: 	LDIL	wtra_I,R0
    // asm 00008460: 	STI	R0,*AR5				;ADDR
    // asm 00008461: 	LDP	@FIFO_INC
    // asm 00008462: 	LDI	@FIFO_INC,R0
    // asm 00008463: 	LDI	*AR1,R0	  			;ML FIX (BOGUS READ)
    // asm 00008464: 	SETDP
    // asm 00008465: 	RPTS	4
    // asm 00008466: 	NOP
    // asm 00008467: 	DBU	AR4,LOOPA
    // asm 00008468: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATER_INFINITY", 0, 0);
    UNIMPL();
}

/* asm: WATERPOSI	.word	WATERPOS */
#define WATERPOSI WATERPOS
/* asm: WATERPOS */
/* asm: 	 */
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
static float WATERPOS[] = {
    -1280.0f,
    128.0f,
    0.0f,
    -1280.0f,
    0.0f,
    0.0f,
    -1024.0f,
    128.0f,
    0.0f,
    -1024.0f,
    0.0f,
    0.0f,
    -768.0f,
    128.0f,
    0.0f,
    -768.0f,
    0.0f,
    0.0f,
    -512.0f,
    128.0f,
    0.0f,
    -512.0f,
    0.0f,
    0.0f,
    -256.0f,
    128.0f,
    0.0f,
    -256.0f,
    0.0f,
    0.0f,
    0.0f,
    128.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    256.0f,
    128.0f,
    0.0f,
    256.0f,
    0.0f,
    0.0f,
    512.0f,
    128.0f,
    0.0f,
    512.0f,
    0.0f,
    0.0f,
    768.0f,
    128.0f,
    0.0f,
    768.0f,
    0.0f,
    0.0f,
    1024.0f,
    128.0f,
    0.0f,
    1024.0f,
    0.0f,
    0.0f,
    1280.0f,
    128.0f,
    0.0f,
    1280.0f,
    0.0f,
    0.0f,
    1536.0f,
    128.0f,
    0.0f,
    1536.0f,
    0.0f,
    0.0f,
    1792.0f,
    128.0f,
    0.0f,
    1792.0f,
    0.0f,
    0.0f,
    // ----------------------------------------------------------------------------
};

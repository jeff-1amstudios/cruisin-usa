#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "vunit.h"
#include "mproc.h"
#include "macs.h"
#include "obj.h"
#include "sys.h"
#include "dirq.h"
#include "sysid.h"
#include "globals.h"
#include "port.h"
#include "backgrnd.h"
#include "totala.h"

/*
 * Source module: asm/DIRQ.ASM
 */

void DIRQ(void);

/* *----------------------------------------------------------------------------
 */
#define POSTERCLIP 300
#define LOW_CLIP_LEVEL 100
#define HIGH_CLIP_LEVEL ((5000-1)) //ACTUAL # OF ENTRIES
/* asm: CAMERAPOSI	.word	_CAMERAPOS */
#define CAMERAPOSI _CAMERAPOS
/* asm: CAMERARADI	.word	_CAMERARAD */
#define CAMERARADI _CAMERARAD
/* asm: CAMERAMATRIXI	.word	_CAMERAMATRIX */
#define CAMERAMATRIXI _CAMERAMATRIX
/* asm: ASHADOW	.word	_ACNTL		;HEADS UP THE FIFO MIRROR */
int ASHADOW = _ACNTL;
/* asm: LIGHTIY	.word	_LIGHT+1 */
int LIGHTIY = _LIGHT+1;
/* asm: transmatrixI	.word	ROTATION_MATRIX */
#define transmatrixI ROTATION_MATRIX
/* asm: transvectorYI	.word	TRANSVECTOR+1 */
int transvectorYI = TRANSVECTOR+1;
/* asm: POSTERMATI	.word	POSTERMATRIX */
#define POSTERMATI POSTERMATRIX
/* asm: POSTERMAT2DI	.word	POSTERMATRIX2D */
#define POSTERMAT2DI POSTERMATRIX2D
/* asm: tmpmatI	.word	TMPMAT */
#define tmpmatI TMPMAT
/* asm: tmpmatY	.word	TMPMAT+1 */
int tmpmatY = TMPMAT+1;
/* asm: SCRNHXI	.float	SCRNHX */
float SCRNHXI = SCRNHX;
/* asm: SCRNHYI	.float	SCRNHY */
float SCRNHYI = SCRNHY;
/* asm: HIGH_CLIP_LEV8	.word	80000		;MATHEMATICAL LIMIT */
int HIGH_CLIP_LEV8 = 80000;
/* asm: MATRIXAI	.word	_MATRIXA */
#define MATRIXAI _MATRIXA
/* asm: MATRIXBI	.word	_MATRIXB */
#define MATRIXBI _MATRIXB
/* asm: MATRIXCI	.word	_MATRIXC */
#define MATRIXCI _MATRIXC
/* asm: VECTORAI	.word	_VECTORA */
#define VECTORAI _VECTORA
/* asm: VECTORBI	.word	_VECTORB */
#define VECTORBI _VECTORB
/* asm: VECTORCI	.word	_VECTORC */
#define VECTORCI _VECTORC
/* asm: VECTORDI	.word	_VECTORD */
#define VECTORDI _VECTORD
/* asm: VECTORAYI	.word	_VECTORA+1 */
int VECTORAYI = _VECTORA+1;
/* asm: POSTERMATRIX2D	fbss	POSTERMATRIX2D,4 */
int POSTERMATRIX2D[4];

/* *----------------------------------------------------------------------------
*DIRQ
*
*This is the main display loop which queues up each object list to be sent
*to DISPLAY, and performs any nessesary leg work.
*
*	In essence:
*	for all objects		   <--|
*		trivial rejection  ---|
*		translate vectors
*		plot polygons  <---|
*			hsr    ----|
*			send to asic
*		next polygon
*	next object
*
 */
void DIRQ(void)
{
    // asm 00000064: 	PUSH	R4
    // asm 00000065: 	LDI	@SYSCNTL,R0		;if the system hangs and the LED
    // asm 00000066: 	OR	LED_OFF,R0		;is on we were in this routine
    // asm 00000067: 	STI	R0,@SYSCNTL		;when it happened
    // asm 00000068: 	LDP	@SYSCNTLR
    // asm 00000069: 	STI	R0,@SYSCNTLR
    // asm 0000006A: 	SETDP
    // asm 0000006B: 	LDI	@POSTERMATI,AR2		;find the inverse Y rotation matrix "poster matrix"
    // asm 0000006C: 	LDP	@_CAMERARAD+Y
    // asm 0000006D: 	NEGF	@_CAMERARAD+Y,R2
    // asm 0000006E: 	SETDP
    // asm 0000006F: 	CALL	FIND_YMATRIX
    // asm 00000070: 	LDI	@POSTERMATI,AR2
    // asm 00000071: 	LDI	AR2,R3
    // asm 00000072: 	LDI	@CAMERAMATRIXI,R2
    // asm 00000073: 	CALL	CONCATMAT
    // asm 00000074: 	LDI	@POSTERMAT2DI,AR0
    // asm 00000075: 	LDF	*+AR2(A00),R0
    // asm 00000076: 	STF	R0,*+AR0(0)
    // asm 00000077: 	LDF	*+AR2(A02),R0
    // asm 00000078: 	STF	R0,*+AR0(1)
    // asm 00000079: 	LDF	*+AR2(A20),R0
    // asm 0000007A: 	STF	R0,*+AR0(2)
    // asm 0000007B: 	LDF	*+AR2(A22),R0
    // asm 0000007C: 	STF	R0,*+AR0(3)
#if STATISTICS
    // asm: 	LDI	0,R1	      		;ZERO OUT YOUR STATS DUDES
    // asm: 	STI	R1,@ST_OBJECTS
    // asm: 	STI	R1,@ST_POLYGONS
#endif
    // asm 0000007D: 	LDI	@OLOW_PRIORITYI,AR0
    // asm 0000007E: 	CALL	DISPLAY
    // asm 0000007F: 	SETDP
    // asm 00000080: 	LDI	@OACTIVEI,AR0		;setup pointer
    // asm 00000081: 	CALL	DISPLAY
    // asm 00000082: 	SETDP
    // asm 00000083: 	LDI	@OACTIVE_PRIORITYI,AR0
    // asm 00000084: 	CALL	DISPLAY
    // asm 00000085: 	SETDP
    // asm 00000086: 	LDI	@OHIGH_PRIORITYI,AR0
    // asm 00000087: 	CALL	DISPLAY
    // asm 00000088: 	SETDP
    // asm 00000089: 	LDI	@SYSCNTL,R0
    // asm 0000008A: 	ANDN	LED_OFF,R0
    // asm 0000008B: 	STI	R0,@SYSCNTL
    // asm 0000008C: 	LDP	@SYSCNTLR
    // asm 0000008D: 	STI	R0,@SYSCNTLR
    // asm 0000008E: 	SETDP
    // asm 0000008F: 	POP	R4
    // asm 00000090: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIRQ", 0, 0);
    UNIMPL();
}

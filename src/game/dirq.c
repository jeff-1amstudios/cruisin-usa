#include "dirq.h"
#include "../core/cpu.h"
#include "../core/machine.h"
#include "backgrnd.h"
#include "c30.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "sys.h"
#include "sysid.h"
#include "totala.h"
#include "vunit.h"

/*
 * Source module: asm/DIRQ.ASM
 */

void DIRQ(void);

#define OACTIVEI OACTIVE
#define IDLE_LISTI IDLE_LIST
#define OACTIVE_PRIORITYI OACTIVE_PRIORITY
#define OLOW_PRIORITYI OLOW_PRIORITY
#define OHIGH_PRIORITYI OHIGH_PRIORITY
#define CAMERAPOSI _CAMERAPOS
#define CAMERARADI _CAMERARAD
#define CAMERAMATRIXI _CAMERAMATRIX
#define LOCTEMPER_MATI LOCTEMPER_MAT
#define transmatrixI ROTATION_MATRIX
#define POSTERMATI POSTERMATRIX
#define POSTERMAT2DI POSTERMATRIX2D
#define BLOWLISTI BLOWLIST
#define tmpmatI TMPMAT
#define INVTABI INVTAB
#define DRIVE_LISTI DRIVE_LIST
#define CAR_LISTI CAR_LIST
#define SIGN_LISTI SIGN_LIST
#define GROUND_LISTI GROUND_LIST
#define MATRIXAI _MATRIXA
#define MATRIXBI _MATRIXB
#define MATRIXCI _MATRIXC
#define VECTORAI _VECTORA
#define VECTORBI _VECTORB
#define VECTORCI _VECTORC
#define VECTORDI _VECTORD
#define _PALLISTI _PALLIST
#define FASTSTKI FASTSTK

/*
 *v7.03
 *----------------------------------------------------------------------------
 *DISPLAY/TRANSFORMATION SYSTEM
 *
 *COPYRIGHT (C) 1994  BY TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 *
 *	Eric L Pribyl
 *	Eugene P. Jarvis
 *
 *	1.0	JUL 91	ELP	Original version	May 30, 1991
 *	2.00    NOV 91  EPJ     HEAVY OPTIMIZATIONS
 *	2.01	DEC 91  ELP	PComp support
 *	2.10	MAR 92  ELP/EPJ	Altering for target,Optimizations
 *	3.00	MAY 92  ELP	Modification for Illumination models
 *	3.01	MAY 92  ELP	POLYGON PLOT OPTIMIZATIONS
 *	3.02	AUG 92  ELP	Mods for reality
 *	3.03	AUG 92  ELP	Sorting removed
 *	3.04	SEP 92  ELP	FIFO, single palette added, optimizations
 *	3.05	OCT 92	ELP	no control panel,dithering,centroid remove
 *	3.06	OCT 92	ELP	Dynamic objects added
 *	4.00	DEC 92	EPJ/ELP	dynamic optimizations and data structure alterations
 *	4.01	JAN 93	ELP	Illuminated, TMed Objects
 *	5.00	JUL 93	ELP	DMA ASIC
 *	5.01	JUL 93	ELP	Direct stuff to DMA
 *	6.00	JUL 93	ELP	Removal of Direct Stuff to DMA (copy in 'dirqnfif.asm')
 *				.	statistics added
 *				.	optimization
 *	6.01	AUG 93	ELP	Compressed Polygon Format & Compressed Vertex Format
 *	7.00	OCT 93  ELP	test case for 2D rotations
 *	7.01	NOV 93  ELP	full blown 2D rotations
 *	7.02	APR 94	ELP	true operation for O_NOUROT
 *	7.03	JUL 94  ELP	DPs removed, statistics optional
 *	7.04	SEP 94  EPJ	GLITCH FIX
 *
 *IN ALL ROUTINES
 *	AR0 - OBJECT BLOCK
 *	AR1 - ROM POINTER
 *
 *
 */

// *----------------------------------------------------------------------------
#define POSTERCLIP 300
#define LOW_CLIP_LEVEL 100
#define HIGH_CLIP_LEVEL ((5000 - 1)) // ACTUAL # OF ENTRIES

VECTOR _CAMERAPOS;
/* asm: CAMERAPOSI		.word	_CAMERAPOS */
#define CAMERAPOSI _CAMERAPOS
/* asm: CAMERARADI		.word	_CAMERARAD */
VECTOR _CAMERARAD;
#define CAMERARADI _CAMERARAD
/* asm: CAMERAMATRIXI		.word	_CAMERAMATRIX */
/* asm: 	 */

MATRIX _CAMERAMATRIX;
#define CAMERAMATRIXI _CAMERAMATRIX
/* asm: ASHADOW			.word	_ACNTL		;HEADS UP THE FIFO MIRROR */
// uintptr_t ASHADOW = (uintptr_t)&_ACNTL;
VECTOR _LIGHT;
/* asm: LIGHTIY			.word	_LIGHT+1 */
/* asm: 	 */
// uintptr_t LIGHTIY = (uintptr_t)(_LIGHT + 1);
/* asm: transmatrixI		.word	ROTATION_MATRIX */
MATRIX ROTATION_MATRIX;
#define transmatrixI ROTATION_MATRIX
/* asm: transvectorYI		.word	TRANSVECTOR+1 */
VECTOR TRANSVECTOR;
// uintptr_t transvectorYI = (uintptr_t)(TRANSVECTOR + 1);
/* asm: POSTERMATI		.word	POSTERMATRIX */
MATRIX POSTERMATRIX;
#define POSTERMATI POSTERMATRIX
/* asm: POSTERMAT2DI		.word	POSTERMATRIX2D */
/* asm: 	 */
#define POSTERMAT2DI POSTERMATRIX2D
/* asm: tmpmatI			.word	TMPMAT */
VECTOR TMPMAT;
#define tmpmatI TMPMAT
/* asm: tmpmatY			.word	TMPMAT+1 */
// uintptr_t tmpmatY = (uintptr_t)(TMPMAT + 1);
/* asm: SCRNHXI			.float	SCRNHX */
float SCRNHXI = SCRNHX;
/* asm: SCRNHYI			.float	SCRNHY */
/* asm: 	 */
float SCRNHYI = SCRNHY;
/* asm: HIGH_CLIP_LEV8		.word	80000		;MATHEMATICAL LIMIT */
/* asm: 	 */
/* asm: 	 */
int HIGH_CLIP_LEV8 = 80000;
/* asm: MATRIXAI		.word	_MATRIXA */
MATRIX _MATRIXA;
#define MATRIXAI _MATRIXA
/* asm: MATRIXBI		.word	_MATRIXB */
MATRIX _MATRIXB;
#define MATRIXBI _MATRIXB
/* asm: MATRIXCI		.word	_MATRIXC */
MATRIX _MATRIXC;
#define MATRIXCI _MATRIXC
/* asm: VECTORAI		.word	_VECTORA */
VECTOR _VECTORA;
#define VECTORAI _VECTORA
/* asm: VECTORBI		.word	_VECTORB */
VECTOR _VECTORB;
#define VECTORBI _VECTORB
/* asm: VECTORCI		.word	_VECTORC */
VECTOR _VECTORC;
#define VECTORCI _VECTORC
/* asm: VECTORDI		.word	_VECTORD */
VECTOR _VECTORD;
#define VECTORDI _VECTORD
/* asm: VECTORAYI		.word	_VECTORA+1 */
/* asm: 	 */
// static uintptr_t VECTORAYI = (uintptr_t)(_VECTORA + 1);
/* asm: POSTERMATRIX2D	fbss	POSTERMATRIX2D,4 */
static u32 POSTERMATRIX2D[4];

static u32 CLIPRAM[CLIPRAML];

/*
 *----------------------------------------------------------------------------
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
void DIRQ(void) {
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
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

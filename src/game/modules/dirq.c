#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/vunit.h"
#include "../include/mproc.h"
#include "../include/macs.h"
#include "../include/obj.h"
#include "../include/sys.h"
#include "../include/dirq.h"
#include "../include/sysid.h"
#include "../include/globals.h"
#include "../include/dirq_defs.h"
#include "../include/backgrnd.h"
#include "../include/obj_defs.h"
#include "../include/totala.h"

/*
 * Source module: asm/DIRQ.ASM
 */

// *v7.03
// *----------------------------------------------------------------------------
// *DISPLAY/TRANSFORMATION SYSTEM
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *
// *	Eric L Pribyl
// *	Eugene P. Jarvis
// *
// *	1.0	JUL 91	ELP	Original version	May 30, 1991
// *	2.00    NOV 91  EPJ     HEAVY OPTIMIZATIONS
// *	2.01	DEC 91  ELP	PComp support
// *	2.10	MAR 92  ELP/EPJ	Altering for target,Optimizations
// *	3.00	MAY 92  ELP	Modification for Illumination models
// *	3.01	MAY 92  ELP	POLYGON PLOT OPTIMIZATIONS
// *	3.02	AUG 92  ELP	Mods for reality
// *	3.03	AUG 92  ELP	Sorting removed
// *	3.04	SEP 92  ELP	FIFO, single palette added, optimizations
// *	3.05	OCT 92	ELP	no control panel,dithering,centroid remove
// *	3.06	OCT 92	ELP	Dynamic objects added
// *	4.00	DEC 92	EPJ/ELP	dynamic optimizations and data structure alterations
// *	4.01	JAN 93	ELP	Illuminated, TMed Objects
// *	5.00	JUL 93	ELP	DMA ASIC
// *	5.01	JUL 93	ELP	Direct stuff to DMA
// *	6.00	JUL 93	ELP	Removal of Direct Stuff to DMA (copy in 'dirqnfif.asm')
// *				.	statistics added
// *				.	optimization
// *	6.01	AUG 93	ELP	Compressed Polygon Format & Compressed Vertex Format
// *	7.00	OCT 93  ELP	test case for 2D rotations
// *	7.01	NOV 93  ELP	full blown 2D rotations
// *	7.02	APR 94	ELP	true operation for O_NOUROT
// *	7.03	JUL 94  ELP	DPs removed, statistics optional
// *	7.04	SEP 94  EPJ	GLITCH FIX
// *
// *IN ALL ROUTINES
// *	AR0 - OBJECT BLOCK
// *	AR1 - ROM POINTER
// *
// *
// *----------------------------------------------------------------------------
#define POSTERCLIP 300
#define LOW_CLIP_LEVEL 100
#define HIGH_CLIP_LEVEL ((5000-1))
/* asm: CAMERAPOSI	.word	_CAMERAPOS */
#define CAMERAPOSI _CAMERAPOS
/* asm: CAMERARADI	.word	_CAMERARAD */
#define CAMERARADI _CAMERARAD
/* asm: CAMERAMATRIXI	.word	_CAMERAMATRIX */
#define CAMERAMATRIXI _CAMERAMATRIX
/* asm: ASHADOW	.word	_ACNTL		;HEADS UP THE FIFO MIRROR */
int ASHADOW = (int)(_ACNTL);
/* asm: LIGHTIY	.word	_LIGHT+1 */
int LIGHTIY = (int)(_LIGHT+1);
/* asm: transmatrixI	.word	ROTATION_MATRIX */
#define transmatrixI ROTATION_MATRIX
/* asm: transvectorYI	.word	TRANSVECTOR+1 */
int transvectorYI = (int)(TRANSVECTOR+1);
/* asm: POSTERMATI	.word	POSTERMATRIX */
#define POSTERMATI POSTERMATRIX
/* asm: POSTERMAT2DI	.word	POSTERMATRIX2D */
#define POSTERMAT2DI POSTERMATRIX2D
/* asm: tmpmatI	.word	TMPMAT */
#define tmpmatI TMPMAT
/* asm: tmpmatY	.word	TMPMAT+1 */
int tmpmatY = (int)(TMPMAT+1);
/* asm: HIGH_CLIP_LEV8	.word	80000		;MATHEMATICAL LIMIT */
int HIGH_CLIP_LEV8 = (int)(80000);
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
int VECTORAYI = (int)(_VECTORA+1);
/* asm: POSTERMATRIX2D	fbss	POSTERMATRIX2D,4 */
int POSTERMATRIX2D[4];
// *----------------------------------------------------------------------------
// *DIRQ
// *
// *This is the main display loop which queues up each object list to be sent
// *to DISPLAY, and performs any nessesary leg work.
// *
// *	In essence:
// *	for all objects		   <--|
// *		trivial rejection  ---|
// *		translate vectors
// *		plot polygons  <---|
// *			hsr    ----|
// *			send to asic
// *		next polygon
// *	next object
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *DISPLAY
// *
// *This routine displays the linked list pointed to in AR0.  Almost all
// *registers are clobbered.
// *
// *In general the following registers are dedicated
// *AR0	OBJECT POINTER
// *AR1	ROM POINTER
// *
// *
// *
// *	int	radius
// *	int	number_of_vertices|(number_of_polygons<<16)
// *	struct ROM_VERTEX  {
// *		int	X|Y<<16
// *		int	Z
// *	} * number_of_vertices
// *	struct ROM_POLYGON  {
// *		int	palnum<<16|cntl
// *		int	(v4<<24)|(v3<<16)|(v2<<8)|(v1)
// *		int	IV[0]|(IV[1]<<16)
// *		int	IV[2]|(IV[3]<<16)
// *		int	*addr_to_TM;
// *	}
// *
// *NOTE	entry point is near the bottom of the routine
// *
/* asm: BREAKOBJ	.bss	BREAKOBJ,1 */
int BREAKOBJ;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *	PLOTPOLY
// *
// *	Polygons are already transformed, now just preform HSR,
// *	and stuff the fifo with appropriate data.
// *	Note also that this only renders a block of polygons.
// *
// *	struct ROM_POLYGON  {
// *		int	palnum<<16|cntl
// *		int	(v4<<24)|(v3<<16)|(v2<<8)|(v1)
// *		int	IV[0]|(IV[1]<<16)
// *		int	IV[2]|(IV[3]<<16)
// *		int	*addr_to_TM;
// *	}
// *
// *
#endif
// 	;---->BZD	PLOT1PAL
// 	;---->BNZD	PLOTILLUM	;BR-> if it is a one palette object
// *----------------------------------------------------------------------------
// 	;---->BGTD	PLTPOLY		;YES, NO CLIP LOOP
// 	;------->BD	PLTPOLYLP
// *GET EXTERNAL VERTEX INDICIES
// *CHECK ALL Z'S <=0
// 	;------->BGED	INBNDS
// *CHECK FIFO FULL
// 	;---->BND	POLYLP_1
// *CHECK HIDDEN SURFACE REMOVAL
// 	;------->BNZD	INBNDS
// *GLITCH FIX
// 	;------>BGTD	POLYLP			;if back facing DONT PLOT
// *AR4,AR5,AR2,AR3 = FOUR VERTICES
// *IR1=PALETTE
// *
// 	;------->BNZD	CLIPIT	  	;YES SPLIT IT UP...
// *NO CLIP, BLOW IT OUT
// ;	LDI	*AR7,R0		 	;FIFO_INC
// 	;----> DBUD AR6,PLOTPOLYLP
// 	;----> DBUD AR6,PLOTPOLYLP1
// *CLIP IT
// *----------------------------------------------------------------------------
// *warning moving this to top of file will crash program ask ti why

void DIRQ(void)
{
    // asm: 	PUSH	R4
    // asm: 	LDI	@SYSCNTL,R0		;if the system hangs and the LED
    // asm: 	OR	LED_OFF,R0		;is on we were in this routine
    // asm: 	STI	R0,@SYSCNTL		;when it happened
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	SETDP
    // asm: 	LDI	@POSTERMATI,AR2		;find the inverse Y rotation matrix "poster matrix"
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	NEGF	@_CAMERARAD+Y,R2
    // asm: 	SETDP
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	@POSTERMATI,AR2
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	@CAMERAMATRIXI,R2
    // asm: 	CALL	CONCATMAT
    // asm: 	LDI	@POSTERMAT2DI,AR0
    // asm: 	LDF	*+AR2(A00),R0
    // asm: 	STF	R0,*+AR0(0)
    // asm: 	LDF	*+AR2(A02),R0
    // asm: 	STF	R0,*+AR0(1)
    // asm: 	LDF	*+AR2(A20),R0
    // asm: 	STF	R0,*+AR0(2)
    // asm: 	LDF	*+AR2(A22),R0
    // asm: 	STF	R0,*+AR0(3)
#if STATISTICS
    // asm: 	LDI	0,R1	      		;ZERO OUT YOUR STATS DUDES
    // asm: 	STI	R1,@ST_OBJECTS
    // asm: 	STI	R1,@ST_POLYGONS
#endif
    // asm: 	LDI	@OLOW_PRIORITYI,AR0
    // asm: 	CALL	DISPLAY
    // asm: 	SETDP
    // asm: 	LDI	@OACTIVEI,AR0		;setup pointer
    // asm: 	CALL	DISPLAY
    // asm: 	SETDP
    // asm: 	LDI	@OACTIVE_PRIORITYI,AR0
    // asm: 	CALL	DISPLAY
    // asm: 	SETDP
    // asm: 	LDI	@OHIGH_PRIORITYI,AR0
    // asm: 	CALL	DISPLAY
    // asm: 	SETDP
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	ANDN	LED_OFF,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	SETDP
    // asm: 	POP	R4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIRQ", 0, 0);
    UNIMPL();
}

void NEXTOBJ(void)
{
    // asm: 	LDI	R0,AR0
    // asm: 	SETDP
    // asm: 	LDI	@BREAKOBJ,R1
    // asm: 	BZ	NOBREAK_CONTINUE
    // asm: 	CMPI	R1,R0
    // asm: 	BEQ	$
NOBREAK_CONTINUE:
#if STATISTICS
    // asm: 	LDI	@ST_OBJECTS,R1
    // asm: 	INC	R1
    // asm: 	STI	R1,@ST_OBJECTS
#endif
    // *GENERATE TRANSLATION VECTOR
    // asm: 	LDI	R0,AR3			;transform the objects position
    // asm: 	LDI	*+AR0(OFLAGS),R6	;holds the OBJECTS flags
    // asm: 	LDI	*+AR0(OROMDATA),AR1	;get the romptr
    // asm: 	TSTB	O_NOUNIV,R6		;has this object have an absolute distance from
    // asm: 	BZD	TRU_UNIV		;CAMERAPOS or is it a regular object
    // asm: 	ADDI	OPOSX,AR3		;for the CAMERAMATRIX
    // asm: 	LDI	@CAMERAPOSI,AR6		;universe_center
    // asm: 	LDI	@tmpmatI,AR4		;trans = object_pos + univ_pos
    // 	;---->BZ TRU_UNIV
    // asm: 	BUD	TRANS_RET		;this object has an absolute distance from
    // asm: 	LDF	*AR3++,R3		;the cemeras position therefore its relative
    // asm: 	LDF	*AR3++,R4		;position in our calculations does not change
    // asm: 	LDF	*AR3++,R5		;
TRU_UNIV:
    // asm: 	SUBF	*AR6++,*AR3++,R3	;*trans++ = *tp++ - *up++
    // asm: 	SUBF	*AR6++,*AR3++,R4	;*trans++ = *tp++ - *up++
    // asm: 	SUBF	*AR6++,*AR3++,R5	;*trans++ = *tp++ - *up++
TRANS_RET:
    // asm: 	TSTB	O_NOUROT,R6		;is this object NOT rotatable by the CAMERAMATRIX?
    // asm: 	BZD	UNIV_ROT		;BR -> it is
    // asm: 	LDI	@CAMERAMATRIXI,AR5	;src 3x3
    // asm: 	LDI	@transvectorYI,AR6	;dst 1x3
    // asm: 	NOP
    // 	;---->BZ UNIV_ROT
    // *SPECIAL CASE WHEN WE WANT SOMETHING NOT ROTATED BY THE
    // *UNIVMATRIX  (it has absolute coordinates)
    // asm: 	STF	R3,*-AR6(1)		;TRANSVECTOR.x
    // asm: 	BUD	TRIVIAL_REJECTION
    // asm: 	STF	R4,*AR6			;TRANSVECTOR.y
    // asm: 	LDF	R5,R2			;Z value
    // asm: 	ADDI	9,AR5
    // 	;---->	BUD	TRIVIAL_REJECTION
    // ;***	TRIVIAL REJECTION AND TRANSLATION VECTOR
    // 	;AR4	objects position   X,Z
    // 	;AR5	univmatrix   *SAVE*
    // 	;AR6	TRANSLATION VECTOR FOR OBJECT
    // 	;R3	x value of position
    // 	;R4	y value of position
    // 	;R5	z value of position
    // *
    // *ALTERNATE STRATEGY OPERATE ON X,Y,Z IN R3,R4,R5
    // *LOSE SOME PARALLEL MULTS (5 INST.), BUT GAIN ON NOT USING AR4 (4 INST.)
    // *MULTIPLY WOULD BECOME REPEAT BLOCK LOOP (RUN FASTER IF PROG NOT IN FAST RAM)
    // *
    // *ROTATE OBJECTS POSITION BY CAMERAS MATRIX
    // *
UNIV_ROT:
    // asm: 	MPYF3	*AR5++,R3,R0
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm: 	MPYF3	*AR5++,*AR4,R0
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm: 	MPYF3	*AR5++,*AR4,R0
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm: 	ADDF	R1,R2
    // *
    // *CHECK FOR TRIVIAL REJECTION
    // *
TRIVIAL_REJECTION:
    // asm: 	TSTB	O_DEGRADE,R6		;does this have a degrade model?
    // asm: 	BZD	NO_DEGRADE
    // asm: 	STF	R2,*+AR6(1)		;(TRANSVECTOR.z) R2 = Z value of object
    // asm: 	FIX	R2			;convert Z distance to integer
    // asm: 	STI	R2,*+AR0(ODIST)
    // 	;---->BZ NO_DEGRADE
    // asm: 	CMPI	DEGRADE_DIST,R2
    // asm: 	LDIGT	*+AR0(ODEGRADE_ROM),AR1	;if its far enough to degrade it
    // asm: 	TSTB	O_DEGRADE2,R6
    // asm: 	BZ	NO_DEGRADE		;LEVEL 2
    // asm: 	CMPI	DEGRADE_DIST_LEVEL2,R2
    // asm: 	LDIGT	*+AR0(ODEGRADE_ROM2),AR1	;if its far enough to degrade it to level 2
NO_DEGRADE:
    // asm: 	LDI	*AR1++,R4		;get RADIUS of object
    // asm: 	ADDI	R2,R4,R3
    // asm: 	CMPI	LOW_CLIP_LEVEL,R3	;attempt to toss on Z distance
    // asm: 	BLTD	DISPLAY			;is it to close?
    // asm: 	SUBI	R4,R3
    // asm: 	SUBI	R4,R3
    // asm: 	CMPI	@HIGH_CLIP_LEV8,R3
    // 	;----> BLTD	DISPLAY
    // asm: 	BGTD	DISPLAY
    // asm: 	ADDI	R4,R3
    // asm: 	LDIN	0,R3			;KEEP IT IN RANGE
    // asm: 	LSH	-4,R3			;quickly divide by 16
    // 	;---->BGT DISPLAY
    // asm: 	CMPI	HIGH_CLIP_LEVEL,R3	;KEEP INDEX IN RANGE
    // asm: 	LDIGT	HIGH_CLIP_LEVEL,R3
    // asm: 	LDI	R3,AR3
    // asm: 	ADDI	@INVTABI,AR3
    // asm: 	FLOAT	R4			;RADIUS must be positive
    // asm: 	LDF	*AR3,R1			;get the inverse Z factor
    // asm: 	MPYF	R1,R4			;project RADIUS for distance
    // asm: 	MPYF	*AR6,R1,R3		;project Y position
    // asm: 	ADDF	R4,R3,R2
    // asm: 	ADDF	@SCRNHYI,R2
    // asm: 	BLTD	DISPLAY			;BR-> above the screen
    // asm: 	NOP				;PAD FOR DELAYED BRANCH
    // asm: 	SUBF	R4,R3
    // asm: 	CMPF	@SCRNHYI,R3
    // 	;---->BLT DISPLAY
    // asm: 	BGTD	DISPLAY			;BR-> below the screen
    // asm: 	MPYF	*-AR6(1),R1,R3	   	;project X position
    // asm: 	ADDF	R4,R3,R2
    // asm: 	ADDF	@SCRNHXI,R2
    // 	;---->BGT DISPLAY
    // asm: 	BLTD	DISPLAY			;BR-> too far to the left
    // asm: 	NOP			   	;PAD FOR DELAYED BRANCH
    // asm: 	SUBF	R4,R3
    // asm: 	CMPF	@SCRNHXI,R3
    // 	;---->BLT DISPLAY
    // asm: 	BGTD	DISPLAY			;BR-> too far to the right
    // 	;***
    // 	;***	END TRIVIAL REJECTION, WE CAN PROBABLY SEE IT
    // 	;***
    // 	;
    // 	;jump to 2D altering
    // 	;
    // asm: 	LDI	OMATRIX,AR4
    // asm: 	ADDI	AR0,AR4
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // 	;---->BGT DISPLAY			;(object is not being seen)
    // asm: 	TSTB	O_DYNAMIC,R0
    // asm: 	BNZ	ROT3D
    // asm: 	TSTB	O_ILLUM,R0
    // asm: 	BNZ	ROT3D
    // asm: 	RS	(O_3DROT_B+1),R0
    // asm: 	BC	ROT3D
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	ROT3D
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BNZ	TRANS2D
ROT3D:
    // 	;***
    // 	;***	GENERATE ROTATION MATRIX
    // 	;***
    // 	;***
    // 	;***	TEST FOR SPECIAL ROTATION CASES
    // 	;***
    // asm: 	TSTB	O_NOUROT,R6		;is this object NOT rotatable by the CAMERAMATRIX?
    // asm: 	BZ	CHECKTHEREG
    // asm: 	TSTB	O_ILLUM,R6		;kludge for illuminated objects
    // asm: 	BNZ	CHECKTHEREG		;fix in next system
    // asm: 	LDI	AR0,AR5
    // asm: 	ADDI	OMATRIX,AR5
    // asm: 	BU	VT
CHECKTHEREG:
    // *O_POSTER
    // *A poster matrix is oriented by the X/Z in the universe, but is not
    // *rotated by Y.  Simply put:  a poster is an object in the universe
    // *that is always facing the user. -> a poster.
    // asm: 	TSTB	O_POSTER,R6
    // asm: 	BZ	REGULAR
    // asm: 	FLOAT	POSTERCLIP,R0
    // asm: 	CMPF	*+AR6(1),R0		;CHECK IF TOO CLOSE
    // asm: 	BLED	VT		 	;CLIP OUT POSTERS IF TOO CLOSE
    // asm: 	NOP
    // asm: 	LDI	@POSTERMATI,AR5
    // asm: 	NOP
    // 	;---> BLED VT
    // asm: 	B	DISPLAY
REGULAR:
    // asm: 	LDI	@transmatrixI,AR3
    // asm: 	TSTB	O_NOROT,R6		;if this object is not self-orienting
    // asm: 	BZD	SELF_ORIENTING		;we can save on computations by skipping the concat
    // asm: 	LDI	5,IR1
    // asm: 	LDI	3,IR0
    // asm: 	NOP	*--AR5(9)		;RESTORE AR5 = UNIVMATRIX
    // 	;---->BZ NOW
    // asm: 	TSTB	O_IROT,R6
    // asm: 	BNZ	IDENTITY_ROTATION
    // *O_IROT		IDENTITY_ROTATION
    // *THE OBJECT IS NOT SELF-ORIENTING, THEREFORE WE MAY SIMPLY COPY
    // *THE CAMERA MATRIX INTO THE GENERAL ROTATIONAL MATRIX
    // asm: 	BUD	VT
    // asm: 	NOP
    // asm: 	LDI	@CAMERAMATRIXI,AR5
    // asm: 	NOP
    // 	;---->BU VT
    // *in this case identity rotation means that it CAN be self rotating, that is
    // *entirely dependant on the matrix of the object
IDENTITY_ROTATION:
    // asm: 	LDI	AR0,AR5
    // asm: 	ADDI	OMATRIX,AR5
    // asm: 	BU	VT
    // *STANDARD ROTATIONAL CASE
    // *
    // *Concatenate two 3x3 matrixes
    // *	AR4	- src1		(usually the objects matrix)
    // *	AR5	- src2		(usually the Cameras matrix)
    // *	AR3	- dst		(the GRM - general rotational matrix)
    // *
    // *A B C	   J K L    AJ+DK+GL BJ+EK+HL CJ+FK+IL
    // *D E F	*  M N O  = AM+DN+GO BM+EN+HO CM+FN+IO
    // *G H I     P Q R    AP+DQ+GR BP+EQ+HR CP+FQ+IR
    // *
SELF_ORIENTING:
    // asm: 	LDI	2,RC				;LOOP THREE TIMES
    // asm: 	RPTB	INLP1
    // asm: 	MPYF3	*AR5++(1),*AR4++(IR0),R0
    // asm: 	MPYF3	*AR5,*AR4++(IR0),R1
    // asm: 	MPYF3	*+AR5(1),*AR4--(IR1),R1
    // asm: 	MPYF3	*-AR5(1),*AR4++(IR0),R0
    // asm: 	MPYF3	*AR5,*AR4++(IR0),R1
    // asm: 	STF	R2,*AR3++(1)			;store MATij
    // asm: 	MPYF3	*+AR5(1),*AR4--(IR1),R1
    // asm: 	MPYF3	*-AR5(1),*AR4++(IR0),R0
    // asm: 	MPYF3	*AR5++,*AR4++(IR0),R1
    // asm: 	STF	R2,*AR3++(1)			;store MATij
    // asm: 	MPYF3	*AR5++,*AR4--(IR1),R1
    // asm: 	ADDF	R1,R2
    // asm: 	STF	R2,*AR3++(1)			;store MATij
INLP1:
    // asm: NOP	*--AR4(3)
    // 	;***
    // 	;***	VECTOR ROTATION/TRANSLATION
    // 	;***
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NEXTOBJ", 0, 0);
    UNIMPL();
}

void VECTOR_TRANSFORMATION(void)
{
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	TSTB	O_DYNAMIC,R0
    // asm: 	BNZ	DYNAMIC_OBJECT
    // asm: VECTORTRANSFULL				;return on dynamic objects
    // asm: 	LDI	@transmatrixI,AR5	;somewhat dedicated for matrix pointer
VT:
    // asm: 	LDI	*AR1++,RC		;get number of vertices to translate-1
    // asm: 	PUSH	RC
    // asm: 	AND	0FFh,RC
#if STATISTICS
    // asm: 	LDI	@ST_VERTICES,R0
    // asm: 	ADDI	RC,R0
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,@ST_VERTICES
#endif
    // asm: 	LDI	@BLOWLISTI,AR3		;blowlist pointer = AR3
    // asm: 	LDI	@tmpmatY,AR4		;TEMP VECTOR STORE
    // asm: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm: 	LDI	8,IR0
    // asm: 	FLOAT	SCREENHX,R6		;screen half x
    // asm: 	LDI	-16,BK
    // asm: 	BD	VTL
    // asm: 	FLOAT	SCREENHY,R7		;screen half x
    // asm: 	LDF	*-AR6(1),R4		;GET X TRANS
    // asm: 	LDF	*AR6,R5			;GET Y TRANS
    // 	;---->BR 	VTL
    // asm: 	BR 	VTL				; jeff added to match I450 rom
VTL:
    // asm: 	RPTB	EOVCTR
    // 	;;MATRIX MULTIPLY 1x3 * 3x3 -> blowspace
    // 	;AR0	object ptr		PRELOADED *SAVE*
    // 	;AR1	rom ptr			PRELOADED *INC*
    // 	;AR2	inverse table
    // 	;AR3	BLOWLIST ptr
    // 	;AR4	TEMP VERTEX STORE
    // 	;AR5	rotation matrix ptr	PRELOADED *SAVE*
    // 	;AR6	translation vector	PRELOADED *SAVE*
    // 	;AR7
    // 	;R0,R1  TEMP
    // 	;R2	y element/tmp value
    // 	;R3	x element/tmp value
    // 	;R4	X TRANS
    // 	;R5	Y TRANS
    // 	;R6	X SCREEN CENTER
    // 	;R7	Y SCREEN CENTER
    // 	;
    // 	;  A	   D E F       AD+BE+CF
    // 	;[ B ] * [ G H I ] = [ AG+BH+CI	]
    // 	;  C	   J K L       AJ+BK+CL
    // 	;
    // *FOR PACKED INT
    // asm: 	LDI	*AR1++,R3
    // asm: 	ASH	BK,R3,R2    		;BK=16
    // asm: 	LS	16,R3
    // asm: 	ASH	BK,R3
    // asm: 	FLOAT	R3			; x element
    // asm: 	FLOAT	R2			; y element
    // asm: 	STF	R3,*-AR4(1)		;store secondary access A
    // asm: 	FLOAT	*AR1++,R0		;C	get z element of source 1
    // 	;
    // 	;MULTIPLY BY ROTATION MATRIX
    // 	;
    // asm: 	MPYF3	*AR5++,R3,R0		;AD
    // asm: 	MPYF3	*AR5++,R2,R1		;BE
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1	;CF
    // asm: 	MPYF3	*AR5++,*-AR4(1),R0	;AG
    // asm: 	MPYF3	*AR5++,R2,R1		;BH
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1	;CI
    // asm: 	MPYF3	*AR5++,*-AR4(1),R0	;AJ
    // asm: 	MPYF3	*AR5++,*AR4,R1		;BK
    // asm: 	MPYF3	*AR5--(IR0),*+AR4(1),R1	;CL
    // asm: 	ADDF	R1,R2			;AJ+BK+CL
    // asm: 	ADDF	*+AR6(1),R2		;add in translation into [AJ+BK+CL]
    // asm: 	LDF	*-AR3(1),R1		;GET BACK NEW X
    // asm: 	FIX	R2,IR1			;find z distance for inverse lookup
    // asm: 	ASH	-4,IR1			;quickly divide by 16
    // asm: 	CMPI	HIGH_CLIP_LEVEL,IR1   	;compare against highest clip level
    // asm: 	LDIGE	HIGH_CLIP_LEVEL,IR1    	;max it at highest clip level
    // asm: 	CMPI	-80,IR1
    // asm: 	LDILT	-80,IR1
    // asm: 	ADDF	R4,R1			;ADD X TRANSLATION
    // asm: 	ADDF	R5,R3			;add Y translation
    // asm: 	MPYF	*+AR2(IR1),R1,R0    	;x = (x * inverse [z])
    // asm: 	ADDF	R6,R0			;    + SCRNHX 	       ;(screen half x)
    // asm: 	MPYF	*+AR2(IR1),R3,R0	;y = (y * inverse [z])
    // asm: 	MPYF	1.04,R0			; below 1.0 shrinks Y, above expands Y
    // asm: 	ADDF	R7,R0			;    + SCRNHY	       ;(screen half y)
EOVCTR:
    // asm: STF	R0,*AR3++(2)
    // 	;34 Instructions
POLYPOLY_ENTER:
    // asm: 	POP	BK
    // asm: 	RS	16,BK
    // asm: 	CALL	PLOTPOLY
DISPLAY:
    // asm: 	LDI	*AR0,R0
    // asm: 	BNZ	NEXTOBJ
    // asm: DISPLAYX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VECTOR_TRANSFORMATION", 0, 0);
    UNIMPL();
}

void TRANS2D(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // 	;***
    // 	;***	GENERATE ROTATION MATRIX
    // 	;***
    // asm: 	ADDI	1,AR4
    // asm: 	LDI	@transmatrixI,AR3
    // asm: 	NOP	*--AR5(8)		;RESTORE AR5 = UNIVMATRIX
    // asm: 	TSTB	O_POSTER,R6
    // asm: 	BZ	REGULAR1
    // asm: 	FLOAT	POSTERCLIP,R0
    // asm: 	CMPF	*+AR6(1),R0		;CHECK IF TOO CLOSE
    // asm: 	BLED	VT2		 	;CLIP OUT POSTERS IF TOO CLOSE
    // asm: 	NOP
    // asm: 	LDI	@POSTERMAT2DI,AR5
    // asm: 	NOP
    // 	;---> BLED VT2
    // asm: 	B	DISPLAY
REGULAR1:
    // *STANDARD ROTATIONAL CASE
    // *	AR4	- src1		(usually the objects matrix)
    // *	AR5	- src2		(usually the Cameras matrix)
    // *	AR3	- dst		(the GRM - general rotational matrix)
    // *
    // *We generate a 2D matrix (packed) via two psuedo-2d matrices
    // *
    // *A x C	   J x L    AJ+GL  CJ+IL
    // *x x x	*  x x x  =
    // *G x I     P x R    AP+GR  CP+IR
    // *
    // *		1 offset
    // *00 = 0		-1
    // *20 = 6		 5
    // *02 = 2		 1
    // *22 = 8		 7
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRANS2D", 0, 0);
    UNIMPL();
}

void SELF_ORIENTING2(void)
{
    // asm: 	LDI	5,IR0
    // asm: 	LDI	7,IR1
    // asm: 	MPYF	*-AR5(1),*-AR4(1),R0	;(AJ)
    // asm: 	MPYF	*+AR5(IR0),*+AR4(1),R1	;(GL)
    // asm: 	ADDF	R0,R1			;(AJ+GL)
    // asm: 	STF	R1,*AR3++
    // asm: 	MPYF	*+AR5(1),*-AR4(1),R0	;(CJ)
    // asm: 	MPYF	*+AR5(IR1),*+AR4(1),R1	;(IL)
    // asm: 	ADDF	R0,R1			;(CJ+IL)
    // asm: 	STF	R1,*AR3++
    // asm: 	MPYF	*-AR5(1),*+AR4(IR0),R0	;(AP)
    // asm: 	MPYF	*+AR5(IR0),*+AR4(IR1),R1 ;(GR)
    // asm: 	ADDF	R0,R1			;(AP+GR)
    // asm: 	STF	R1,*AR3++
    // asm: 	MPYF	*+AR5(1),*+AR4(IR0),R0	;(CP)
    // asm: 	MPYF	*+AR5(IR1),*+AR4(IR1),R1 ;(IR)
    // asm: 	ADDF	R0,R1			;(CP+IR)
    // asm: 	STF	R1,*AR3++
    // 	;END MATRIX MULTIPLY
    // 	;***
    // 	;***	VECTOR ROTATION/TRANSLATION
    // 	;***
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SELF_ORIENTING2", 0, 0);
    UNIMPL();
}

void VECTOR_TRANSFORMATION2(void)
{
    // asm: 	LDI	@transmatrixI,AR5	;somewhat dedicated for matrix pointer
VT2:
    // asm: 	LDI	*AR1++,RC		;get number of vertices to translate-1
    // asm: 	PUSH	RC
    // asm: 	AND	0FFh,RC			;NUM VERTICES
    // asm: 	LDI	@BLOWLISTI,AR3		;blowlist pointer = AR3
    // asm: 	LDI	@tmpmatY,AR4		;TEMP VECTOR STORE
    // asm: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm: 	BD	VTL2
    // asm: 	FLOAT	SCREENHX,R6		;screen half x
    // asm: 	LDI	3,IR0
    // asm: 	LDI	-16,BK
    // asm: 	BR 	VTL2
VTL2:
    // asm: 	RPTB	EOVCTR2
    // asm: 	LDI	*AR1++,R3
    // asm: 	ASH	BK,R3,R7    		;
    // asm: 	LS	16,R3
    // asm: 	ASH	BK,R3
    // asm: 	FLOAT	R3			;(A)  R3 <- X element
    // asm: 	FLOAT	R7			;     R2 <- Y element  (TRANSLATED ONLY)
    // asm: 	FLOAT	*AR1++,R0		;(B)  R0 <- z element
    // 	;
    // 	;MULTIPLY BY 2x2 ROTATION MATRIX
    // 	;
    // asm: 	MPYF	*AR5++,R3,R2		;(AD)
    // asm: 	MPYF	*AR5++,R0,R1		;(BE)
    // asm: 	ADDF	R2,R1
    // asm: 	MPYF	*AR5++,R3,R3		;(AD)
    // asm: 	MPYF	*AR5--(IR0),R0		;(BE)
    // asm: 	ADDF	R3,R0
    // asm: 	ADDF	*AR6,R7			;add in Y translation into [AJ+BK+CL]
    // asm: 	FIX	R0,IR1			;find z distance for inverse lookup
    // asm: 	ASH	-4,IR1			;quickly divide by 16
    // asm: 	CMPI	HIGH_CLIP_LEVEL,IR1   	;compare against highest clip level
    // asm: 	LDIGE	HIGH_CLIP_LEVEL,IR1    	;max it at highest clip level
    // asm: 	CMPI	-80,IR1
    // asm: 	LDILT	-80,IR1
    // asm: 	MPYF	*+AR2(IR1),R1,R0    	;x = (x * inverse [z])
    // asm: 	ADDF	R6,R0			;    + SCRNHX 	       ;(screen half x)
    // asm: 	MPYF	*+AR2(IR1),R7,R0	;y = (y * inverse [z])
    // asm: 	MPYF	1.04,R0			; below 1.0 shrinks Y, above expands Y
    // asm: 	ADDF	@SCRNHYI,R0		;    + SCRNHY	       ;(screen half y)
EOVCTR2:
    // asm: STF	R0,*AR3++(2)
    // asm: 	BU	POLYPOLY_ENTER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VECTOR_TRANSFORMATION2", 0, 0);
    UNIMPL();
}

void DYNAMIC_OBJECT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *DYNAMIC OBJECT
    // *
    // ;	LDI	*+AR0(OFLAGS),R0
    // ;	TSTB	O_DEGRADE,R0
    // ;	BZ	NOTDEGRADEPOSS
    // asm: 	LDI	*+AR0(ODIST),R0		;FORGET CLOSE DYNAMICS
    // asm: 	CMPI	250,R0
    // asm: 	BLTD	DISPLAY
    // asm: 	LDI	*+AR0(ODEGRADE_ROM),R0
    // asm: 	INC	R0
    // asm: 	CMPI	AR1,R0
    // asm: 	BZD	VECTORTRANSFULL
    // asm: 	LDI	*+AR0(ODEGRADE_ROM2),R0
    // asm: 	INC	R0
    // asm: 	CMPI	AR1,R0
    // 	;---->BZD	VECTORTRANSFULL
    // asm: 	BEQ	VECTORTRANSFULL
    // ;NOTDEGRADEPOSS
    // 	;AR0	OBJECT POINTER/DYNA POINTER
    // 	;AR1	rom ptr			*INC*
    // 	;AR2	tmp pointer for inverse list
    // 	;AR3	BLOWLIST ptr		*INC*
    // 	;AR4	objects position   X,Z
    // 	;AR5	rotation matrix ptr	*SAVE*
    // 	;AR6	OBJ translation vector	*SAVE*
    // 	;AR7	DYNA translation vector	*SAVE*
    // 	;AR5	univmatrix   *SAVE*
    // 	;R0	tmp value
    // 	;R1	tmp value
    // 	;R2	X element/tmp value
    // 	;R3	Y element/tmp value
    // 	;R4	Z element/tmp value
    // 	;R5	inverse tab location	*SAVE*
    // 	;R6	screen half x
    // 	;R7	screen half y
    // asm: 	PUSH	AR0
    // asm: 	LDI	*+AR0(ODYNALIST),AR0
    // asm: 	LDI	@BLOWLISTI,AR3		;blowlist pointer = AR3
    // ;	LDI	@transvectorYI,AR6	;dst 1x3
    // asm: 	LDI	@VECTORAYI,AR7
    // asm: 	LDI	*AR1++,BK		;VERTEX CNT & POLYGON CNT
#if STATISTICS
    // asm: 	LDI	BK,R0
    // asm: 	AND	0FFh,R0
    // asm: 	ADDI	@ST_VERTICES,R0
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,@ST_VERTICES
#endif
    // ;	LDI	*AR1++,R0		;VERTEX CNT & POLYGON CNT
    // ;	PUSH	R0
    // ;	LDI	@INVTABI,R5		;inverse table dedicated ptr
    // asm: 	FLOAT	SCREENHX,R6		;screen half x
    // asm: 	FLOAT	SCREENHY,R7		;screen half x
DYNALOOP:
    // asm: 	LDI	*+AR0(DYNAFLAG),R0
    // asm: 	BND	DYNASHD			;GO DO A SHADOW...
DYNREG:
    // asm: 	LDI	@tmpmatI,AR4		;trans = object_pos + univ_pos
    // asm: 	LDI	@transmatrixI,AR5	;somewhat dedicated for matrix pointer
    // asm: 	LDF	*+AR0(DYNATRANSX),R2
    // 	;------>BND	DYNASHD		;GO DO A SHADOW...
    // asm: 	LDF	*+AR0(DYNATRANSY),R3
    // asm: 	LDF	*+AR0(DYNATRANSZ),R4
    // *
    // *ROTATE TRANSLATION VECTOR BY OBJ-UNIV MATRIX AND ADD TO OBJECT TRANSLATION
    // *
    // asm: 	MPYF	*AR5++,R2,R0
    // asm: 	MPYF	*AR5++,R3,R1
    // asm: 	MPYF	*AR5++,*+AR4(1),R1
    // asm: 	MPYF	*AR5++,*AR4,R0
    // asm: 	MPYF	*AR5++,R3,R1
    // asm:   	STF	R2,*-AR7(1)		;(TRANSVECTOR.x)
    // asm: 	MPYF	*AR5++,*+AR4(1),R1
    // asm: 	MPYF	*AR5++,*AR4,R0
    // asm: 	MPYF	*AR5++,R3,R1
    // asm: 	ADDF	R0,*+AR6(1),R2
    // asm: 	MPYF	*AR5++,*+AR4(1),R1
    // asm: 	ADDF	R1,R2
    // asm: 	STF	R2,*+AR7(1)		;(TRANSVECTOR.z) R2 = Z value of object
    // asm: 	SUBI	9,AR5	       		;RESTORE AR5 OBJ-UNIV MAT
    // *CONCAT UNIV*OBJ MATRIX WITH DYNAMIC MATRIX
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	LDI	AR0,AR4
    // asm: 	ADDI	DYNAMATRIX,AR4
    // *	AR4	- src1		(usually the objects matrix)
    // *	AR5	- src2		(usually the Cameras matrix)
    // *	AR2	- dst		(the GRM - general rotational matrix)
    // *
    // *A B C	   J K L    AJ+DK+GL BJ+EK+HL CJ+FK+IL
    // *D E F	*  M N O  = AM+DN+GO BM+EN+HO CM+FN+IO
    // *G H I     P Q R    AP+DQ+GR BP+EQ+HR CP+FQ+IR
    // asm: 	LDI	5,IR1
    // asm: 	LDI	3,IR0
    // asm: 	LDI	2,RC				;LOOP THREE TIMES
    // asm: 	RPTB	DYNLP1
    // asm: 	MPYF3	*AR5++(1),*AR4++(IR0),R0
    // asm: 	MPYF3	*AR5,*AR4++(IR0),R1
    // asm: 	MPYF3	*+AR5(1),*AR4--(IR1),R1
    // asm: 	MPYF3	*-AR5(1),*AR4++(IR0),R0
    // asm: 	MPYF3	*AR5,*AR4++(IR0),R1
    // asm: 	STF	R2,*AR2++(1)			;store MATij
    // asm: 	MPYF3	*+AR5(1),*AR4--(IR1),R1
    // asm: 	MPYF3	*-AR5(1),*AR4++(IR0),R0
    // asm: 	MPYF3	*AR5++,*AR4++(IR0),R1
    // asm: 	STF	R2,*AR2++(1)			;store MATij
    // asm: 	MPYF3	*AR5++,*AR4--(IR1),R1
    // asm: 	ADDF	R1,R2
    // asm: 	STF	R2,*AR2++(1)			;store MATij
DYNLP1:
    // asm: NOP	*--AR4(3)
    // asm: 	LDI	@tmpmatY,AR4		;TEMP VECTOR STORE
    // asm: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm: 	LDI	8,IR0
    // asm: 	LDI	@MATRIXAI,AR5
    // asm: 	FLOAT	SCREENHX,R6		;screen half x
    // asm: 	FLOAT	SCREENHY,R7		;screen half x
    // asm: 	BD	DYNALP
    // asm: 	LDF	*-AR7(1),R4		;GET X TRANS
    // asm: 	LDF	*AR7,R5			;GET Y TRANS
    // asm: 	LDI	*+AR0(DYNANVERTS),RC	;number of vertices to process -1
    // 	;------>BD	DYNALP
    // asm: 	BU	DYNALP			; added by jeff to match i450 rom
    // 	;;MATRIX MULTIPLY 1x3 * 3x3 -> blowspace
    // 	;AR0	object ptr		PRELOADED *SAVE*
    // 	;AR1	rom ptr			PRELOADED *INC*
    // 	;AR2	inverse table
    // 	;AR3	BLOWLIST ptr
    // 	;AR4	TEMP VERTEX STORE
    // 	;AR5	rotation matrix ptr	PRELOADED *SAVE*
    // 	;AR6	translation vector	PRELOADED *SAVE*
    // 	;AR7
    // 	;R0,R1  TEMP
    // 	;R2	y element/tmp value
    // 	;R3	x element/tmp value
    // 	;R4	X TRANS
    // 	;R5	Y TRANS
    // 	;R6	X SCREEN CENTER
    // 	;R7	Y SCREEN CENTER
DYNALP:
    // asm: 	RPTB	EODVCTR
    // asm: 	LDI	*AR1++,R3
    // asm: 	LDI	R3,R2
    // asm: 	ASH	-16,R2
    // asm: 	FLOAT	R2		;y element
    // asm: 	LS	16,R3
    // asm: 	ASH	-16,R3
    // asm: 	FLOAT	R3		;x element
    // asm: 	SUBF	*+AR0(DYNACENTERX),R3
    // asm: 	STF	R3,*-AR4(1)		;store secondary access A
    // asm: 	SUBF	*+AR0(DYNACENTERY),R2
    // ;	FLOAT	*AR1++,R3		;get x element of source 1
    // ;	SUBF	*+AR0(DYNACENTERX),R3
    // ;
    // ;	FLOAT	*AR1++,R2		;B	get y element of source 1
    // ;||	STF	R3,*-AR4(1)		;store secondary access A
    // ;	SUBF	*+AR0(DYNACENTERY),R2
    // asm: 	FLOAT	*AR1++,R0		;C	get z element of source 1
    // asm: 	SUBF	*+AR0(DYNACENTERZ),R0
    // 	;
    // 	;MULTIPLY BY ROTATION MATRIX
    // 	;
    // asm: 	MPYF3	*AR5++,R3,R0		;AD
    // asm: 	MPYF3	*AR5++,R2,R1		;BE
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1	;CF
    // asm: 	MPYF3	*AR5++,*-AR4(1),R0	;AG
    // asm: 	MPYF3	*AR5++,R2,R1		;BH
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1	;CI
    // asm: 	MPYF3	*AR5++,*-AR4(1),R0	;AJ
    // asm: 	MPYF3	*AR5++,*AR4,R1		;BK
    // asm: 	MPYF3	*AR5--(IR0),*+AR4(1),R1	;CL
    // asm: 	ADDF	R1,R2			;AJ+BK+CL
    // asm: 	ADDF	*+AR7(1),R2		;add in translation into [AJ+BK+CL]
    // ;	LDFLT	0,R2			;it will always be positive
    // asm: 	LDF	*-AR3(1),R1		;GET BACK NEW X
    // asm: 	FIX	R2,IR1			;find z distance for inverse lookup
    // asm: 	ASH	-4,IR1			;quickly divide by 16
    // asm: 	CMPI	HIGH_CLIP_LEVEL,IR1   	;compare against highest clip level
    // asm: 	LDIGE	HIGH_CLIP_LEVEL,IR1    	;max it at highest clip level
    // asm: 	CMPI	-80,IR1
    // asm: 	LDILT	-80,IR1
    // asm: 	ADDF	R4,R1			;ADD X TRANSLATION
    // asm: 	ADDF	R5,R3			;add Y translation
    // asm: 	MPYF	*+AR2(IR1),R1,R0    	;x = (x * inverse [z])
    // asm: 	ADDF	R6,R0			;    + SCRNHX 	       ;(screen half x)
    // asm: 	MPYF	*+AR2(IR1),R3,R0	;y = (y * inverse [z])
    // asm: 	MPYF	1.04,R0			; below 1.0 shrinks Y, above expands Y
    // asm: 	ADDF	R7,R0			;    + SCRNHY	       ;(screen half y)
EODVCTR:
    // asm: STF	R0,*AR3++(2)
DYNALPX:
    // asm: 	LDI	*AR0,AR0
    // asm: 	LDI	AR0,R0
    // asm: 	BNZ	DYNALOOP
    // ;	POP	BK
    // asm: 	POP	AR0
    // asm: 	RS	16,BK
    // asm: 	CALL	PLOTPOLY
    // asm: 	LDI	*AR0,R0
    // asm: 	BNZ	NEXTOBJ
    // asm: 	RETS
    // *DYNAMIC OBJECT SHADOW
    // 	;AR0	OBJECT POINTER/DYNA POINTER
    // 	;AR1	rom ptr			*INC*
    // 	;AR2	tmp pointer for inverse list
    // 	;AR3	BLOWLIST ptr		*INC*
    // 	;AR4	objects position   X,Z
    // 	;AR5	rotation matrix ptr	*SAVE*
    // 	;AR6	OBJ translation vector	*SAVE*
    // 	;AR7	DYNA translation vector	*SAVE*
    // 	;AR5	univmatrix   *SAVE*
    // 	;R0	tmp value
    // 	;R1	tmp value
    // 	;R2	X element/tmp value
    // 	;R3	Y element/tmp value
    // 	;R4	Z element/tmp value
    // 	;R5	inverse tab location	*SAVE*
    // 	;R6	screen half x
    // 	;R7	screen half y
DYNASHD:
    // asm: 	LDI	@CAMERAMATRIXI,AR5	;CAMERA ROTATION MATRIX
    // asm: 	LDI	@INVTABI,R5		;inverse table dedicated ptr
    // *FIND CAR STRUCT IN PROCESS
    // asm: 	LDI	*+AR0(DYNAPARENT),AR4	;GET POINTER TO PARENT
    // asm: 	LDI	*+AR4(OCARBLK),AR4
    // asm: 	ADDI	CARVSIZ,AR4
    // *CHECK SHADOW TYPE
    // asm: 	LDI	*+AR4(CARSHAD-CARVSIZ),R0	;SHADOW ACTIVE
    // asm: 	BZ	NOSHAD				;NO...BLOW IT OUT
    // *IF NOT AIRBORNE DO REGULAR
    // asm: 	LDI	*+AR4(CAR_AIRF-CARVSIZ),R0
    // asm: 	OR	*+AR4(CAR_AIRB-CARVSIZ),R0
    // asm: 	BZ	DYNREG
    // asm: 	LDI	@_MODE,R4	       	;NO FLYING SHADOWS IN TUNNEL
    // asm: 	TSTB	MINTUNNEL,R4
    // asm: 	BNZ	DYNREG
    // asm: 	LDI	8,IR0
    // ;	LDI	*+AR0(DYNANVERTS),RC	;number of vertices to process -1
    // ;	LDI	RC,R0
    // ;	ADDI	1,R0
    // ;	MPYI	2,R0
    // ;	ADDI	R0,AR1			;ADJUST AR1 TO SKIP VERTICES
    // asm: 	LDI	3,RC
    // asm: 	ADDI	8,AR1			;SKIP 4 SHADOW VERTICES
    // asm: 	RPTB	EOSVCTR
    // *GET COORDS IN ORDER, ADD IN ROAD DIFF
    // asm: 	LDI	@CAMERAPOSI,AR2
    // asm: 	LDF	*AR4++,R3		;GET X COORD OF CAR POINT
    // asm: 	SUBF	*AR2,R3			;GET RELATIVE TO CAMERA
    // asm: 	LDF	*AR4++,R4		;GET Y COORD OF CAR POINT
    // asm: 	SUBF	*+AR2(1),R4		;GET RELATIVE TO CAMERA
    // asm: 	ADDF	*+AR4(1),R4		;ADD IN ROAD DIFFERENTIAL TO Y
    // asm: 	LDF	*AR4++,R2		;GET Z COORD OF CAR POINT
    // asm: 	SUBF	*+AR2(2),R2		;GET RELATIVE TO CAMERA
    // *RETURN TO DYNAMIC OBJECT LOOP WITH REG INTACT
    // *MULTIPLY BY CAMERA ROTATION MATRIX
    // asm: 	MPYF3	*AR5++,R3,R0		;AD
    // asm: 	MPYF3	*AR5++,R4,R1		;BE
    // asm: 	MPYF3	*AR5++,*+AR7(1),R1	;CF
    // asm: 	MPYF3	*AR5++,*-AR7(1),R0	;AG
    // ;	ADDF	*-AR6(1),R2		;add translation into [AD+BE+CF]
    // asm: 	MPYF3	*AR5++,R4,R1		;BH
    // asm: 	MPYF3	*AR5++,*+AR7(1),R1	;CI
    // asm: 	MPYF3	*AR5++,*-AR7(1),R0	;AJ
    // ;	ADDF	*AR6,R3			;add translation into [AG+BH+CI]
    // asm: 	MPYF3	*AR5++,R4,R1		;BK
    // asm: 	MPYF3	*AR5--(IR0),*+AR7(1),R1	;CL
    // asm: 	ADDF	R1,R2			;AJ+BK+CL
    // ;	ADDF	*+AR6(1),R2		;add in translation into [AJ+BK+CL]
    // asm: 	STF	R2,*AR3--(2)		;(TRANSVECTOR.z) R2 = Z value of object
    // asm: 	FIX	R2,R0			;find z distance for inverse lookup
    // ;	LDILT	0,R0			;it will always be positive
    // asm: 	ADDI	CARVSIZ-3,AR4		;ADDRESS NEXT WHEEL ENTRY
    // asm: 	ASH	-4,R0			;quickly divide by 16
    // asm: 	CMPI	HIGH_CLIP_LEVEL,R0   	;compare against highest clip level
    // asm: 	LDIGE	HIGH_CLIP_LEVEL,R0    	;max it at highest clip level
    // asm: 	CMPI	-80,R0
    // asm: 	LDILT	-80,R0
    // asm: 	ADDI	R5,R0,AR2		;add in start of inverse table
    // asm: 	MPYF	*AR2,*AR3,R0		;x = (x * inverse [z])
    // asm: 	ADDF	R6,R0			;    + SCRNHX 	       ;(screen half x)
    // asm: 	STF	R0,*AR3++
    // asm: 	MPYF	*AR2,*AR3,R0		;y = (y * inverse [z])
    // asm: 	MPYF	1.04,R0	; below 1.0 shrinks Y, above expands Y
    // asm: 	ADDF	R7,R0			;    + SCRNHY	       ;(screen half y)
EOSVCTR:
    // asm: STF	R0,*AR3++(2)
    // asm: 	LDI	@transmatrixI,AR5	;RESTORE MATRIX POINTER
    // asm: 	BU	DYNALPX
    // *NO SHADOW KLUDGE
NOSHAD:
    // asm: 	LDF	0,R0
    // asm: 	FLOAT	-1000,R0
    // asm: 	LDI	3,RC
    // asm: 	ADDI	8,AR1			;SKIP 4 SHADOW VERTICES
    // asm: 	RPTS	11
    // asm: 	STF	R0,*AR3++    		;STORE NULL X,Y,Z
    // asm: 	LDI	@transmatrixI,AR5	;RESTORE MATRIX POINTER
    // asm: 	BU	DYNALPX
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DYNAMIC_OBJECT", 0, 0);
    UNIMPL();
}

void CLIPCK(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CHECK THE CLIP AND DUMP VERTICES INTO INTERNAL RAM
    // *
    // *
    // *RETURN
    // *	R5 NZ=CLIP, Z=NOCLIP
    // *
    // 	;***	PRELIM CHECK
    // asm: 	FIX	*+AR4(IR0),R0		;read X value
    // asm: 	FIX	*+AR4(IR1),R0
    // asm: 	FIX	*+AR5(IR0),R0
    // asm: 	FIX	*+AR5(IR1),R0
    // asm: 	FIX	*+AR2(IR0),R0
    // asm: 	FIX	*+AR2(IR1),R0
    // asm: 	FIX	*+AR3(IR0),R0
    // asm: 	FIX	*+AR3(IR1),R0
    // asm: 	STI	R0,*AR0
    // asm: 	ABSI	R0,R5
    // asm: 	LDI	6,RC
    // asm: 	RPTB	LPP2
    // asm: 	ABSI	*--AR0,R1
LPP2:
    // asm: OR	R1,R5
    // asm: 	LSH	-10,R5
    // asm: 	RETSZ
    // 	;***	STRICT CHECK
    // asm: 	LDI	0,R5
    // asm: 	LDI	*AR0++,R0		;XMAX
    // asm: 	LDI	*AR0,R2			;YMAX
    // asm: 	LDI	2,RC
    // asm: 	RPTB	CKLP
    // asm: 	CMPI	*++AR0,R0
    // asm: 	LDILT	*AR0,R0
    // asm: 	CMPI	*AR0,R1
    // asm: 	LDIGT	*AR0,R1
    // asm: 	CMPI	*++AR0,R2
    // asm: 	LDILT	*AR0,R2
    // asm: 	CMPI	*AR0,R3
CKLP:
    // asm: LDIGT	*AR0,R3
    // asm: 	NOP	*AR0--(7)
    // asm: 	SUBI	R1,R0 		;FIND XMAX-XMIN
    // asm: 	CMPI	2047,R0
    // asm: 	LDIGT	1,R5
    // asm: 	SUBI	R3,R2		;FIND YMAX-YMIN
    // asm: 	CMPI	2047,R2
    // asm: 	LDIGT	1,R5
    // asm: 	LDI	R5,R5
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLIPCK", 0, 0);
    UNIMPL();
}

void CLIP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CLIP THE SUCKER
    // *SPLIT INTO 4 POLYGONS UL,UR,LL,LR
    // *	AR0	4 EXTERNAL VERTICES (INTS)
    // *	R0 	PALETTE
    // *	R2      FLAGS
    // *
    // asm: 	PUSH	IR0
    // asm: 	PUSH	IR1
    // asm: 	LDI	2,IR0
    // asm: 	LDI	3,IR1
    // asm: 	LDI	R2,AR3		     	;SAVE FLAGS
    // asm: 	LDI	R0,R4			;SAVE PALETTE
    // ;	LDIL	CLIPRAM,R0		;LOAD UP SCRATCH AREA
    // ;	CMPI	R0,AR0
    // ;	BNE	$
    // *CHECK TRIVIAL REJECTION
    // *X ALL NEGATIVE CASE
    // *
    // asm: 	AND	*AR0,*+AR0(IR0),R0
    // asm: 	AND	*+AR0(4),R0
    // asm: 	AND	*+AR0(6),R0
    // asm: 	BND	CLIPX	    		;ALL X <0 REJECT
    // *X ALL POSTIVE CASE
    // asm: 	LDI	511,R1
    // asm: 	SUBI	*AR0,R1,R2
    // asm: 	SUBI	*+AR0(IR0),R1,R3
    // 	;------->BND	CLIPX	    	;ALL X <0 REJECT
    // asm: 	AND	R3,R2
    // asm: 	LDI	511,R3
    // asm: 	SUBI	*+AR0(4),R3
    // asm: 	AND	R3,R2
    // asm: 	SUBI	*+AR0(6),R1
    // asm: 	AND	R1,R2
    // asm: 	BND	CLIPX			;ALL X >511 REJECT
    // *Y ALL POSTIVE CASE
    // asm: 	LDI	399,R1
    // asm: 	SUBI	*+AR0(1),R1,R2
    // asm: 	SUBI	*+AR0(IR1),R1,R3
    // 	;------->BND	CLIPX	    	;ALL X >511 REJECT
    // asm: 	AND	R3,R2
    // asm: 	LDI	511,R3
    // asm: 	SUBI	*+AR0(5),R3
    // asm: 	AND	R3,R2
    // asm: 	SUBI	*+AR0(7),R1
    // asm: 	AND	R1,R2
    // asm: 	BND	CLIPX			;ALL Y >511 REJECT
    // *Y ALL NEGATIVE CASE
    // asm: 	AND	*+AR0(1),*+AR0(IR1),R0
    // asm: 	AND	*+AR0(5),R0
    // asm: 	AND	*+AR0(7),R0
    // 	;------->BND	CLIPX	    	;ALL X >511 REJECT
    // asm: 	BN	CLIPX			;ALL Y >511 REJECT
    // *
    // *COMPUTE YOUR INTERNAL VERTICES
    // *AR0-POINTER TO POLY STACK
    // *AR1=ROM POINTER
    // *UNPACK THE SUCKERS
    // *
    // asm: 	LDI	*AR1++,R0		;UNPACK THE SUCKERS
    // asm: 	LDI	0FFH,R2
    // asm: 	AND	R2,R0,R1
    // asm: 	STI	R1,*+AR0(8)
    // asm: 	LSH	-8,R0
    // asm: 	AND	R2,R0,R1
    // asm: 	STI	R1,*+AR0(9)
    // asm: 	LSH	-8,R0
    // asm: 	AND	R2,R0,R1
    // asm: 	STI	R1,*+AR0(10)
    // asm: 	LSH	-8,R0
    // asm: 	AND	R2,R0,R1
    // asm: 	STI	R1,*+AR0(11)
    // asm: 	LDI	*AR1++,R0		;UNPACK THE SUCKERS
    // asm: 	AND	R2,R0,R1
    // asm: 	STI	R1,*+AR0(12)
    // asm: 	LSH	-8,R0
    // asm: 	AND	R2,R0,R1
    // asm: 	STI	R1,*+AR0(13)
    // asm: 	LSH	-8,R0
    // asm: 	AND	R2,R0,R1
    // asm: 	STI	R1,*+AR0(14)
    // asm: 	LSH	-8,R0
    // asm: 	AND	R2,R0,R1
    // asm: 	STI	R1,*+AR0(15)
    // asm: 	LDI	AR0,R6
    // asm:  	BD	BUSTUP
    // asm: 	LDI	R6,R7
    // asm: 	ADDI	CLIPRAML-80,R7	     	;GET LENGTH LIMIT
    // asm: 	LDI	*AR1++,R5		;GET TEXTURE MAP ADDR
    //  	;------->BD	BUSTUP
    // *
    // *POP STACK
    // *
    // *AR0 POINTS TO CURRENT TOP OF STACK
    // *0-7  XY EXT VERTS INT
    // *8-15 XY INT VERTS INT
    // *	AR0	4 EXTERNAL VERTICES (INTS)
    // *	AR1	ROM INDEX
    // *	AR3 	PALETTE
    // *	R4      FLAGS
    // *	R5	TEXTURE MAP ADDR
CLIPOP:
    // asm: 	NOP	*AR0--(16)		;POP OFF OLD ENTRY
    // ;	LDI	AR0,R1
    // ;	AND	0FH,R1
    // ;	CMPI	0DH,R1
    // ;	BNE	$
CLIP0:
    // asm: 	CMPI	AR0,R6			;ARE WE DONE
    // asm: 	BGT	CLIPDONE		;YES...WERE OUTTA HERE
    // ;	BGTD	CLIPDONE		;YES...WERE OUTTA HERE
    // ;	LDI	AR0,R1
    // ;	AND	0FH,R1
    // ;	CMPI	0DH,R1
    // ;	BNE	$
    // ;	LDIL	CLIPRAM,R0		;LOAD UP SCRATCH AREA
    // ;	LDIL	CLIPLIM,R1		;LOAD UP SCRATCH AREA
    // ;	CMPI	R0,AR0
    // ;	BLT	$
    // ;	CMPI	R1,AR0
    // ;	BGT	$
    // *
    // *CHECK TRIVIAL REJECTION
    // *
    // *X ALL NEGATIVE CASE
    // *
    // asm: 	AND	*AR0,*+AR0(IR0),R0
    // asm: 	AND	*+AR0(4),R0
    // asm: 	AND	*+AR0(6),R0
    // 	;-------->BGTD	CLIPDONE  	;YES...EXIT
    // asm: 	BND	CLIPOP	    		;ALL X <0 REJECT
    // *X ALL POSTIVE CASE
    // asm: 	LDI	511,R1
    // asm: 	SUBI	*AR0,R1,R2
    // asm: 	SUBI	*+AR0(IR0),R1,R3
    // 	;------->BND	CLIPOP	    	;ALL X <0 REJECT
    // asm: 	AND	R3,R2
    // asm: 	LDI	511,R3
    // asm: 	SUBI	*+AR0(4),R3
    // asm: 	AND	R3,R2
    // asm: 	SUBI	*+AR0(6),R1
    // asm: 	AND	R1,R2
    // asm: 	BND	CLIPOP			;ALL X >511 REJECT
    // *Y ALL POSTIVE CASE
    // asm: 	LDI	399,R1
    // asm: 	SUBI	*+AR0(1),R1,R2
    // asm: 	SUBI	*+AR0(IR1),R1,R3
    // 	;------->BND	CLIPOP	    	;ALL X >511 REJECT
    // asm: 	AND	R3,R2
    // asm: 	LDI	511,R3
    // asm: 	SUBI	*+AR0(5),R3
    // asm: 	AND	R3,R2
    // asm: 	SUBI	*+AR0(7),R1
    // asm: 	AND	R1,R2
    // asm: 	BND	CLIPOP			;ALL Y >511 REJECT
    // *Y ALL NEGATIVE CASE
    // asm: 	AND	*+AR0(1),*+AR0(IR1),R0
    // asm: 	AND	*+AR0(5),R0
    // asm: 	AND	*+AR0(7),R0
    // 	;------->BND	CLIPOP	    	;ALL X >511 REJECT
    // asm: 	BND	CLIPOP		    	;ALL Y <0 REJECT
    // *CHECK OVERSIZE OBJECT
    // asm: 	LDI	*AR0,R0			;XMAX
    // asm: 	LDI	*+AR0(1),R2		;YMAX
    // asm: 	LDI	2,RC
    // 	;------->BND	CLIPOP	    	;Y ALL NEGATIVE	REJECT
    // asm: 	RPTB	CLCKLP
    // asm: 	CMPI	*++AR0(IR0),R0
    // asm: 	LDILT	*AR0,R0
    // asm: 	CMPI	*AR0,R1
    // asm: 	LDIGT	*AR0,R1
    // asm: 	CMPI	*+AR0(1),R2
    // asm: 	LDILT	*+AR0(1),R2
    // asm: 	CMPI	*+AR0(1),R3
CLCKLP:
    // asm: LDIGT	*+AR0(1),R3
    // asm: 	SUBI	R1,R0
    // asm: 	CMPI	2047,R0
    // asm: 	BGTD	BUSTUP
    // asm: 	NOP	*AR0--(6)  		;RESET VERTEX INDEX
    // asm: 	SUBI	R3,R2
    // asm: 	CMPI	2047,R2
    // 	;------->BGTD	BUSTUP
    // asm: 	BGT	BUSTUP
    // *POLYGON IS O.K. OUTPUT IT
PCOUT:
PCWT:
    // asm: 	LDI	@FIFO_STATUS,R0		;WAIT FOR YOUR FIFO
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZ	PCWT
    // asm: 	LDI	R4,R0			;GET PALETTE READY
    // asm: 	STI	AR3,*AR7		;STORE YOUR FLAGS
    // asm: 	RPTS	7
    // asm: 	LDI	*AR0++,R0		;GET EXTERNALS
    // asm:  	STI	R0,*AR7
    // asm: 	LDI	*AR0++(IR0),R0	 	;GET YX INTERNAL 1
    // asm: 	LSH	8,R1
    // asm: 	ADDI	R1,R0
    // asm:  	STI	R0,*AR7
    // asm: 	LDI	*AR0++(IR0),R0		;GET YX INTERNAL 2
    // asm: 	LSH	8,R1
    // asm: 	ADDI	R1,R0
    // asm:  	STI	R0,*AR7
    // asm: 	LDI	*AR0++(IR0),R0	  	;GET YX INTERNAL 3
    // asm: 	LSH	8,R1
    // asm: 	ADDI	R1,R0
    // asm:  	STI	R0,*AR7
    // asm: 	LDI	*AR0++(IR0),R0	  	;GET YX INTERNAL 4
    // asm: 	LSH	8,R1
    // asm: 	ADDI	R1,R0
    // asm:  	STI	R0,*AR7
    // asm: 	STI	R5,*AR7 		;STORE TEXTURE MAP ADDR
    // asm: 	LDI	@FIFO_INC,R0		;INC YOUR FIFO
    // asm: 	NOP	*AR0--(32)		;READJUST INDEX
    //  ;	LDI	AR0,R1
    //  ;	AND	0FH,R1
    //  ;	CMPI	0DH,R1
    //  ;	BNE	$
    // asm: 	B 	CLIP0			;GET NEXT POLYGON
    // *BUSTUP A POLYGON INTO 4 OTHERS, PUT ON STACK
    // *AR0=CURRENT STACK ENTRY
BUSTUP:
    // ;	LDI	AR0,R1
    // ;	AND	0FH,R1
    // ;	CMPI	0DH,R1
    // ;	BNE	$
    // ;	LDIL	CLIPRAM,R0		;LOAD UP SCRATCH AREA
    // ;	LDIL	CLIPLIM,R1		;LOAD UP SCRATCH AREA
    // ;	CMPI	R0,AR0
    // ;	BLT	$
    // ;	CMPI	R1,AR0
    // ;	BGT	$
    // asm: 	CMPI	R7,AR0			;STACK TOO HIGH?
    // asm: 	BLT	BUSTUP0
    // asm: 	B	PCOUT			;YES, JUST DO IT...
    // *COMPUTE YOUR EXTERNAL VERTICES
BUSTUP0:
    // asm: 	LDI	AR0,AR2
    // asm: 	LDI	*+AR0(IR0),R0		;X2
    // asm: 	STI	R0,*+AR2(12H)
    // asm: 	STI	R1,*+AR2(13H)
    // asm: 	LDI	*+AR0(4),R0		;X3
    // asm: 	STI	R0,*+AR2(24H)
    // asm: 	LDI	*+AR0(5),R0		;Y3
    // asm: 	STI	R0,*+AR2(25H)
    // asm: 	LDI	*+AR0(6),R0		;X4
    // asm: 	STI	R0,*+AR2(36H)
    // asm: 	LDI	*+AR0(7),R0		;Y4
    // asm: 	STI	R0,*+AR2(37H)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;X1+X2/2=X5
    // asm: 	STI	R0,*+AR2(42H)
    // asm: 	STI	R0,*+AR2(10H)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;Y1+Y2/2=Y5
    // asm: 	STI	R0,*+AR2(43H)
    // asm: 	STI	R0,*+AR2(11H)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0		       	;X2+X3/2=X6
    // asm: 	STI	R0,*+AR2(14H)
    // asm: 	STI	R0,*+AR2(22H)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;Y2+Y3/2=Y6
    // asm: 	STI	R0,*+AR2(15H)
    // asm: 	STI	R0,*+AR2(23H)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;X3+X4/2=X7
    // asm: 	STI	R0,*+AR2(26H)
    // asm: 	STI	R0,*+AR2(34H)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;Y3+Y4/2=Y7
    // asm: 	STI	R0,*+AR2(27H)
    // asm: 	STI	R0,*+AR2(35H)
    // asm: 	LDI	*--AR0(6),R0
    // asm: 	ADDI	*+AR0(6),R0
    // asm: 	ASH	-1,R0			;X1+X4/2=X8
    // asm: 	STI	R0,*+AR2(46H)
    // asm: 	STI	R0,*+AR2(30H)
    // asm: 	LDI	*+AR0(1),R0
    // asm: 	ADDI	*+AR0(7),R0
    // asm: 	ASH	-1,R0			;Y1+Y4/2=Y8
    // asm: 	STI	R0,*+AR2(47H)
    // asm: 	STI	R0,*+AR2(31H)
    // asm: 	ADDI	*AR0,*+AR0(IR0),R0
    // asm: 	ADDI	*+AR0(4),R0
    // asm: 	ADDI	*+AR0(6),R0
    // asm: 	ASH	-2,R0			;X1+X2+X3+X4/4=X9
    // asm: 	STI	R0,*+AR2(44H)
    // asm: 	STI	R0,*+AR2(16H)
    // asm: 	STI	R0,*+AR2(20H)
    // asm: 	STI	R0,*+AR2(32H)
    // asm: 	LDI	*+AR0(1),R0
    // asm: 	ADDI	*+AR0(3),R0
    // asm: 	ADDI	*+AR0(5),R0
    // asm: 	ADDI	*+AR0(7),R0
    // asm: 	ASH	-2,R0			;Y1+Y2+Y3+Y4/4=Y9
    // asm: 	STI	R0,*+AR2(45H)
    // asm: 	STI	R0,*+AR2(17H)
    // asm: 	STI	R0,*+AR2(21H)
    // asm: 	STI	R0,*+AR2(33H)
    // *COMPUTE YOUR INTERNAL VERTICES
    // asm: 	NOP	*AR0++(8)
    // asm: 	LDI	*AR0,R0			;X1
    // asm: 	STI	R0,*+AR2(48H)
    // asm: 	STI	R1,*+AR2(49H)
    // asm: 	LDI	*+AR0(IR0),R0		;X2
    // asm: 	STI	R0,*+AR2(1AH)
    // asm: 	STI	R1,*+AR2(1BH)
    // asm: 	LDI	*+AR0(4),R0		;X3
    // asm: 	STI	R0,*+AR2(2CH)
    // asm: 	LDI	*+AR0(5),R0		;Y3
    // asm: 	STI	R0,*+AR2(2DH)
    // asm: 	LDI	*+AR0(6),R0		;X4
    // asm: 	STI	R0,*+AR2(3EH)
    // asm: 	LDI	*+AR0(7),R0		;Y4
    // asm: 	STI	R0,*+AR2(3FH)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;X1+X2/2=X5
    // asm: 	STI	R0,*+AR2(4AH)
    // asm: 	STI	R0,*+AR2(18H)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;Y1+Y2/2=Y5
    // asm: 	STI	R0,*+AR2(4BH)
    // asm: 	STI	R0,*+AR2(19H)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0		       	;X2+X3/2=X6
    // asm: 	STI	R0,*+AR2(1CH)
    // asm: 	STI	R0,*+AR2(2AH)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;Y2+Y3/2=Y6
    // asm: 	STI	R0,*+AR2(1DH)
    // asm: 	STI	R0,*+AR2(2BH)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;X3+X4/2=X7
    // asm: 	STI	R0,*+AR2(2EH)
    // asm: 	STI	R0,*+AR2(3CH)
    // asm: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm: 	ASH	-1,R0			;Y3+Y4/2=Y7
    // asm: 	STI	R0,*+AR2(2FH)
    // asm: 	STI	R0,*+AR2(3DH)
    // asm: 	LDI	*--AR0(6),R0
    // asm: 	ADDI	*+AR0(6),R0
    // asm: 	ASH	-1,R0			;X1+X4/2=X8
    // asm: 	STI	R0,*+AR2(4EH)
    // asm: 	STI	R0,*+AR2(38H)
    // asm: 	LDI	*+AR0(1),R0
    // asm: 	ADDI	*+AR0(7),R0
    // asm: 	ASH	-1,R0			;Y1+Y4/2=Y8
    // asm: 	STI	R0,*+AR2(4FH)
    // asm: 	STI	R0,*+AR2(39H)
    // asm: 	ADDI	*AR0,*+AR0(IR0),R0
    // asm: 	ADDI	*+AR0(4),R0
    // asm: 	ADDI	*+AR0(6),R0
    // asm: 	ASH	-2,R0			;X1+X2+X3+X4/4=X9
    // asm: 	STI	R0,*+AR2(4CH)
    // asm: 	STI	R0,*+AR2(1EH)
    // asm: 	STI	R0,*+AR2(28H)
    // asm: 	STI	R0,*+AR2(3AH)
    // asm: 	LDI	*+AR0(1),R0
    // asm: 	ADDI	*+AR0(3),R0
    // asm: 	ADDI	*+AR0(5),R0
    // asm: 	ADDI	*+AR0(7),R0
    // asm: 	ASH	-2,R0			;Y1+Y2+Y3+Y4/4=Y9
    // asm: 	STI	R0,*+AR2(4DH)
    // asm: 	STI	R0,*+AR2(1FH)
    // asm: 	STI	R0,*+AR2(29H)
    // asm: 	STI	R0,*+AR2(3BH)
    // *TRANSFER LAST ENTRY INTO FIRST ONE
    // asm: 	NOP	*AR2++(42H)
    // asm: 	NOP	*AR0--(6)		;POINT TO X2,Y2
    // asm: 	LDI	*AR2++,R0		;GET FIRST
    // asm: 	RPTS	12
    // asm: 	LDI	*AR2++,R0
    // asm: 	STI	R0,*AR0++(31H)		;STORE LAST ONE, RESET INDEX TO TOS
    // ;	LDI	AR0,R1
    // ;	AND	0FH,R1
    // ;	CMPI	0DH,R1
    // ;	BNE	$
    // asm: 	B	CLIPOP
CLIPX:
    // asm: 	NOP	*AR0--(16)		;RESTORE AR0
    // asm: 	ADDI	3,AR1			;SKIP THE INTERNAL VERTS AND TM CRAP
CLIPDONE:
    // asm: 	LDI	-16,R6			;RESTORE R6
    // asm: 	LDI	0FFH,R7			;RESTORE R7
    // asm: 	NOP	*AR0++(16)		;RESTORE AR0
    // asm: 	LDIL	CLIPRAM,R0		;LOAD UP SCRATCH AREA
#if DEBUG
    // asm: 	CMPI	R0,AR0
    // asm: 	BNE	$
#endif
    // asm: 	POP	IR1
    // asm: 	POP	IR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLIP", 0, 0);
    UNIMPL();
}

void PLTPOLY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PLOT A DISTANT POLYGON
    // *
    // *R7=0FFH
    // *IR0=BLOWLIST
    // *IR1=BLOWLIST+1
    // *RC=POLYGON COUNT
    // *
    // ;	PUSH	AR0
    // ;	LDI	AR6,RC
    // asm: 	BUD	PLTPOLYLP
    // asm: 	LDP	@FIFO_STATUS
    // asm: 	LDI	FIFO_ADDR>>16,AR7
    // asm: 	LSH	16,AR7
    // 	;------->BD	PLTPOLYLP
    // asm: 	BU PLTPOLYLP 		; added by jeff to match i450 rom
PLTPOLYLP:
    // asm: 	RPTB	PLTPOLL
    // asm: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm: 	AND	R7,R3,AR4
PLTPOLYLP1:
    // asm: 	MPYI	3,AR4			;V1
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR5
    // asm: 	MPYI	3,AR5			;V2
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR2
    // asm: 	MPYI	3,AR2			;V3
PLTWT:
    // 	;CHECK FIFO FULL
    // asm: 	LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZD	PLTWT
    // 	;CHECK HIDDEN SURFACE REMOVAL
    // asm: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm: 	SUBF	*+AR4(IR1),*+AR5(IR1),R2	;dy = ay - by
    // asm: 	SUBF	*+AR2(IR0),*+AR5(IR0),R0	;ex = cx - bx
    // 	;------->BNZD	PLTWT
    // asm: 	MPYF	R2,R0,R0			;ex = dy * ex
    // asm: 	MPYF	R1,R2			;ey = dx * ey
    // asm: 	SUBF	R0,R2			;ey = ey - ex
    // asm: 	BGTD	PLTLP1			;if back facing DONT PLOT
    // asm: 	LSH	-8,R3			;START GETTING V4
    // asm: 	LDI	3,R1
    // asm: 	NOP
    // 	;------->BGTD	PLTLP1
    // asm: 	LDI	*AR1++(2),R2		;get control word
    // asm: 	LSH	R6,R2,R0		;SHIFT 16 TO RIGHT
    // asm: 	MPYI	R1,R3,AR3		;GET LAST VERTEX  V4
    // asm: 	ADDI	R0,BK,AR6
    // asm: 	LSH	R6,*AR6,R0		;PALETTE->R0
    // asm:  	LSH	8,R0			;not a good way to do this fix l8r -7/14/93
    // asm: 	FIX	*+AR4(IR0),R0		;READ X1
    // asm: 	FIX	*+AR4(IR1),R0		;READ X1
    // asm: 	FIX	*+AR5(IR0),R0		;READ X2
    // asm: 	FIX	*+AR5(IR1),R0		;READ Y2
    // asm: 	FIX	*+AR2(IR0),R0		;READ X3
    // asm: 	FIX	*+AR2(IR1),R0		;READ Y3
    // asm: 	FIX	*+AR3(IR0),R0		;READ X4
    // asm: 	FIX	*+AR3(IR1),R0		;READ Y4
    // asm:  	STI	R0,*AR7
    // asm: 	LDI	*AR1++,R0
    // asm: 	LDI	*AR1++,R1
    // asm: 	LDI	*AR1++,R2
    // asm: 	LSH	-16,R0
    // asm: 	STI	R0,*AR7
    // asm:  	STI	R1,*AR7
    // asm: 	LSH	-16,R1
    // asm: 	STI	R1,*AR7
    // asm:  	STI	R2,*AR7
    // asm: PLTPOLL
    // asm: 	LDI	@FIFO_INC,R0
    // ;	POP	AR0
    // asm: 	RETS
PLTLP1:
    // asm: 	SUBI	1,RC
    // asm: 	LDI	RC,R0
    // asm: 	BNND	PLTPOLYLP1
    // asm: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm: 	SUBI	1,AR1
    // asm: 	AND	R7,R3,AR4
    // 	;----> BNND PLTPOLYLP1
    // asm: PLTXX
    // ;	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLTPOLY", 0, 0);
    UNIMPL();
}

void PLOT1PAL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PLOT POLYGONS FOR A ONE PALETTE TEXTURE MAP OBJECT
    // *
    // *R0=ODIST-ORAD
    // *RC=POLYGON COUNT
    // *
    // asm: 	CMPI	1000,R0
    // asm: 	BGTD	PLT1PAL			;YES, NO CLIP LOOP
    // asm: 	LDI	@BLOWLISTI,IR0
    // asm: 	LDI	IR0,IR1
    // asm: 	ADDI	1,IR1
    // 	;---->BGTD	PLT1PAL			;YES, NO CLIP LOOP
    // asm: 	PUSH 	AR0
    // asm: 	LDI	RC,AR6
    // asm: 	LDI	0FFH,R7			;GET MASK
    // asm: 	LDI	-16,R6			;SHIFT COUNT
    // asm: 	LDI	*+AR0(OPAL),BK
    // asm: 	LDI	@CLIPRAMI,AR0
    // asm: 	BUD	PLOTPOLYLP_1
    // asm: 	LDP	@FIFO_STATUS
    // asm: 	LDI	FIFO_ADDR>>16,AR7
    // asm: 	LSH	16,AR7
    // 	;------->BD	PLTPOLYLP_1
    // asm: 	BU	PLTPOLYLP_1				; added by jeff to match i450 rom
PLOTPOLYLP_1:
    // *GET EXTERNAL VERTEX INDICIES
    // asm: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm: 	AND	R7,R3,AR4
    // asm: PLOTPOLYLP1_1
    // asm: 	ADDI	1,IR1
    // asm: 	MPYI	3,AR4			;V1
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR5
    // asm: 	MPYI	3,AR5			;V2
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR2
    // asm: 	MPYI	3,AR2			;V3
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR3
    // asm: 	MPYI	3,AR3			;V4
    // *CHECK ALL Z'S <=0
    // asm: 	LDF	*+AR4(IR1),R0
    // asm: 	BGED	INBNDS_1
    // asm: 	AND	*+AR5(IR1),*+AR2(IR1),R0
    // asm: 	AND	*+AR3(IR1),R0
    // asm: 	SUBI	1,IR1
    // 	;------->BGED	INBNDS_1
    // asm: 	LSH	8,R0
    // asm: 	BND	POLYLP_1
    // asm: 	NOP
    // *CHECK FIFO FULL
INBNDS_1:
    // asm: 	LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // 	;---->BCD	POLYLP_1
    // asm: 	BNZD	INBNDS_1
    // *CHECK HIDDEN SURFACE REMOVAL
    // asm: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm: 	SUBF	*+AR4(IR1),*+AR5(IR1),R3	;dy = ay - by
    // asm: 	SUBF	*+AR5(IR0),*+AR2(IR0),R0	;ex = cx - bx
    // 	;------->BNZD	INBNDS_1
    // asm: 	MPYF	R3,R0				;ex = dy * ex
    // asm: 	MPYF	R2,R1				;ey = dx * ey
    // asm: 	SUBF	R1,R0				;ey = ey - ex
    // asm: 	BGTD	POLYLP_1				;if back facing DONT PLOT
    // *GLITCH FIX
    // asm: 	SUBF	*+AR2(IR1),*+AR3(IR1),R0
    // asm: 	MPYF	R3,R0
    // asm: 	CMPI	AR2,AR3
    // 	;------>BGTD	POLYLP_1			;if back facing DONT PLOT
    // asm: 	BZD	LOF2X
    // asm: 	MPYF	R3,R2
    // asm: 	OR	R2,R0
    // asm: 	LDF	R0,R0
    // asm: 	BGT	POLYLP_1
LOF2X:
    // *
    // *AR4,AR5,AR2,AR3 = FOUR VERTICES
    // *IR1=PALETTE
    // *
    // asm: 	CALL	CLIPCK
    // asm: 	BNZD	CLIPIT_1		;GO CLIP IT DUDES
    // asm: 	LDI	*AR1++(2),R2		;get control word/palette
    // asm: 	LSH	R6,R2,R0		;SHIFT 16 TO RIGHT
    // asm: 	LDI	BK,R0
    // 	;------->BNZD	CLIPIT_1	;YES SPLIT IT UP...
    // asm: 	STI	R2,*AR7
    // ;	LSH	R6,*AR4,R0		;PALETTE->R0
    // ; 	LSH	8,R0			;not a good way to do this fix l8r -7/14/93
    // *NO CLIP, BLOW IT OUT
    // asm: BLOWOUT_1
    // asm: 	RPTS	7
    // asm: 	LDI	*AR0++,R0
    // asm: 	NOP	*AR0--(8)		;READJUST INDEX DUDES...
    // asm: 	LDI	*AR1++,R0		;2 AIV packed format Y2:X2:Y1:X1
    // asm: 	STI	R0,*AR7
    // asm: 	LSH	-16,R0
    // asm: 	LDI	*AR1++,R0		;GET Y4:X4:Y3:X3
    // asm: 	STI	R0,*AR7
    // asm: 	LSH	-16,R0
    // asm: 	DBUD	AR6,PLOTPOLYLP_1
    // asm: 	LDI	*AR1++,R0
    // asm: 	STI	R0,*AR7			;STORE THE ADDR
    // asm: 	LDI	@FIFO_INC,R0
    // 	;----> DBUD AR6,PLOTPOLYLP_1
    // asm: 	POP	AR0			;RESTORE OBJECT POINTER
    // asm: 	RETS
POLYLP_1:
    // asm: 	DBUD	AR6,PLOTPOLYLP1_1
    // asm: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm: 	SUBI	1,AR1
    // asm: 	AND	R7,R3,AR4
    // 	;----> DBUD AR6,PLOTPOLYLP1_1
    // asm: 	POP	AR0
    // asm: 	RETS
    // *CLIP IT
CLIPIT_1:
    // asm: 	CALL 	CLIP
    // asm: 	DBU	AR6,PLOTPOLYLP_1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLOT1PAL", 0, 0);
    UNIMPL();
}

void PLT1PAL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PLOT A DISTANT 1 PALETTE POLYGON
    // *
    // *IR0=BLOWLIST
    // *IR1=BLOWLIST+1
    // *RC=POLYGON COUNT
    // *
    // asm: 	CMPI	50,RC			;BIG OBJECT?
    // asm: 	BLTD	PLT1PAL1
    // asm: 	LDI	0FFH,R7			;GET MASK
    // asm: 	LDI	*+AR0(OPAL),R6
    // asm: 	LDI	*+AR0(ODIST),R0
    // 	;------>BLTD	PLT1PAL1
    // asm: 	CMPI	DEGRADE_DIST,R0		;CHECK IF DISTANT
    // asm: 	BGT	PLT1PAL1		;YES, FORGET IT
    // asm: 	LDI	*+AR0(OID),R0		;CHECK FOR CAR
    // asm: 	CMPI	484H,R0	   		;OPENING FERRARI?
    // asm: 	BZ	PLTG1PAL		;YES...
    // asm: 	AND	CLASS_M,R0
    // asm: 	CMPI	DRONE_C,R0
    // asm: 	BGT	PLT1PAL1		;NOT A CAR
    // asm: 	CMPI	PLYR_C,R0
    // asm: 	BGE	PLTG1PAL		;ITS A CAR...
PLT1PAL1:
    // asm: 	BUD	PLTPOLYLP_2
    // asm: 	LDP	@FIFO_STATUS
    // asm: 	LDI	FIFO_ADDR>>16,AR7
    // asm: 	LSH	16,AR7
    // 	;------->BD	PLTPOLYLP_2
    // asm: 	BU	PLTPOLYLP_2			; added by jeff to match i450 rom
PLTPOLYLP_2:
    // asm: 	RPTB	PLTPOLL_2
    // asm: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm: 	AND	R7,R3,AR4
PLTPOLYLP1_2:
    // asm: 	MPYI	3,AR4			;V1
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR5
    // asm: 	MPYI	3,AR5			;V2
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR2
    // asm: 	MPYI	3,AR2			;V3
PLTWT_2:
    // 	;CHECK FIFO FULL
    // asm: 	LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZD	PLTWT_2
    // 	;CHECK HIDDEN SURFACE REMOVAL
    // asm: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm: 	SUBF	*+AR4(IR1),*+AR5(IR1),R2	;dy = ay - by
    // asm: 	SUBF	*+AR2(IR0),*+AR5(IR0),R0	;ex = cx - bx
    // 	;------->BNZD	PLTWT_2
    // asm: 	MPYF	R2,R0,R0			;ex = dy * ex
    // asm: 	MPYF	R1,R2			;ey = dx * ey
    // asm: 	SUBF	R0,R2			;ey = ey - ex
    // asm: 	BGTD	PLTLP1_2		;if back facing DONT PLOT
    // asm: 	LSH	-8,R3			;START GETTING V4
    // asm: 	LDI	3,R1
    // asm: 	NOP
    // 	;------->BGTD	PLTLP1_2
    // asm: 	LDI	*AR1++(2),R2		;get control word
    // asm: 	LDI	R2,R0			;SHIFT 16 TO RIGHT
    // asm: 	RS	16,R0
    // asm: 	MPYI	R1,R3,AR3		;GET LAST VERTEX  V4
    // asm: 	STI	R2,*AR7
    // asm: 	FIX	*+AR4(IR0),R0		;READ X1
    // asm: 	FIX	*+AR4(IR1),R0		;READ X1
    // asm: 	FIX	*+AR5(IR0),R0		;READ X2
    // asm: 	FIX	*+AR5(IR1),R0		;READ Y2
    // asm: 	FIX	*+AR2(IR0),R0		;READ X3
    // asm: 	FIX	*+AR2(IR1),R0		;READ Y3
    // asm: 	FIX	*+AR3(IR0),R0		;READ X4
    // asm: 	FIX	*+AR3(IR1),R0		;READ Y4
    // asm:  	STI	R0,*AR7
    // asm: 	LDI	*AR1++,R0
    // asm: 	LDI	*AR1++,R1
    // asm: 	LDI	*AR1++,R2
    // asm: 	LSH	-16,R0
    // asm: 	STI	R0,*AR7
    // asm:  	STI	R1,*AR7
    // asm: 	LSH	-16,R1
    // asm: 	STI	R1,*AR7
    // asm:  	STI	R2,*AR7
    // asm: PLTPOLL_2
    // asm: 	LDI	@FIFO_INC,R0
    // asm: 	RETS
PLTLP1_2:
    // asm: 	SUBI	1,RC
    // asm: 	LDI	RC,R0
    // asm: 	BNND	PLTPOLYLP1_2
    // asm: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm: 	SUBI	1,AR1
    // asm: 	AND	R7,R3,AR4
    // 	;----> BNND PLTPOLYLP1_2
    // asm: 	RETS
    // *****************************
    // *PLOT A DISTANT 1 PALETTE POLYGON GLITCH FIX
    // *
PLTG1PAL:
    // asm: 	BUD	PLTGPOLYLP_2
    // asm: 	LDP	@FIFO_STATUS
    // asm: 	LDI	FIFO_ADDR>>16,AR7
    // asm: 	LSH	16,AR7
    // 	;------->BD	PLTGPOLYLP_2
    // asm: 	BU	PLTGPOLYLP_2			; added by jeff to match i450 rom
PLTGPOLYLP_2:
    // asm: 	RPTB	PLTGPOLL_2
    // asm: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm: 	AND	R7,R3,AR4
PLTGPOLYLP1_2:
    // asm: 	MPYI	3,AR4			;V1
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR5
    // asm: 	MPYI	3,AR5			;V2
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR2
    // asm: 	MPYI	3,AR2			;V3
    // asm: 	LSH	-8,R3
    // asm: 	AND	R7,R3,AR3
    // asm: 	MPYI	3,AR3			;V4
PLTGWT_2:
    // 	;CHECK FIFO FULL
    // asm: 	LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZD	PLTGWT_2
    // 	;CHECK HIDDEN SURFACE REMOVAL
    // asm: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm: 	SUBF	*+AR4(IR1),*+AR5(IR1),R3	;dy = ay - by
    // asm: 	SUBF	*+AR5(IR0),*+AR2(IR0),R0	;ex = bx - cx
    // 	;------->BNZD	PLTGWT_2
    // asm: 	MPYF	R3,R0				;ex = dy * ex
    // asm: 	MPYF	R2,R1				;ey = dx * ey
    // asm: 	SUBF	R1,R0				;ey = ey - ex
    // asm: 	BGTD	PLTGLP1_2			;if back facing DONT PLOT
    // *LOF KLUDGE - CHECK FOR NON-PLANAR
    // asm: 	SUBF	*+AR2(IR1),*+AR3(IR1),R0
    // asm: 	MPYF	R3,R0
    // asm: 	CMPI	AR2,AR3
    // asm: 	BZD	LOF4X
    // asm: 	MPYF	R3,R2
    // asm: 	OR	R2,R0
    // asm: 	LDF	R0,R0
    // asm: 	BGT	PLTGLP1_2
LOF4X:
    // asm: 	LDI	*AR1++(2),R2		;get control word
    // asm: 	LDI	R2,R0			;SHIFT 16 TO RIGHT
    // asm: 	RS	16,R0
    // asm: 	STI	R2,*AR7
    // asm: 	FIX	*+AR4(IR0),R0		;READ X1
    // asm: 	FIX	*+AR4(IR1),R0		;READ X1
    // asm: 	FIX	*+AR5(IR0),R0		;READ X2
    // asm: 	FIX	*+AR5(IR1),R0		;READ Y2
    // asm: 	FIX	*+AR2(IR0),R0		;READ X3
    // asm: 	FIX	*+AR2(IR1),R0		;READ Y3
    // asm: 	FIX	*+AR3(IR0),R0		;READ X4
    // asm: 	FIX	*+AR3(IR1),R0		;READ Y4
    // asm:  	STI	R0,*AR7
    // asm: 	LDI	*AR1++,R0
    // asm: 	LDI	*AR1++,R1
    // asm: 	LDI	*AR1++,R2
    // asm: 	LSH	-16,R0
    // asm: 	STI	R0,*AR7
    // asm:  	STI	R1,*AR7
    // asm: 	LSH	-16,R1
    // asm: 	STI	R1,*AR7
    // asm:  	STI	R2,*AR7
    // asm: PLTGPOLL_2
    // asm: 	LDI	@FIFO_INC,R0
    // asm: 	RETS
PLTGLP1_2:
    // asm: 	SUBI	1,RC
    // asm: 	LDI	RC,R0
    // asm: 	BNND	PLTGPOLYLP1_2
    // asm: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm: 	SUBI	1,AR1
    // asm: 	AND	R7,R3,AR4
    // 	;----> BNND PLTGPOLYLP1_2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLT1PAL", 0, 0);
    UNIMPL();
}

void PLOTILLUM(void)
{
    // *----------------------------------------------------------------------------
    // *PLOTILLUM
    // *
    // *For most purposes this routine mirrors what PLOTPOLY does,
    // *except that it has been streamlined for use with ILLUMINATED
    // *objects
    // *
    // *v4.00:	THIS IS THE ONLY SUBSYSTEM WHICH ACCEPTS NORMALS IN THE POLYGON
    // *	DATA BLOCK.
    // *
    // *	struct ROM_ILLUM_POLYGON  {
    // *		int	cntl
    // *		float	Nx,Ny,Nz
    // *		int	(v4<<24)|(v3<<16)|(v2<<8)|(v1)
    // *	}
    // *
    // *
    // asm: 	PUSH	AR0
    // ;	LDI	*+AR0(OFLAGS),R6
    // asm: 	LSH	-16,R6
    // asm: 	AND	0FFh,R6			;get the color field out of the object flags
    // ;	LDIL	BLOWLIST,IR0
    // ;	LDI	IR0,IR1
    // ;	INC	IR1
    // asm: 	LDIL	FIFO_STATUS,AR0		;FIFO EMPTY STATUS
    // asm: 	LDI	FIFO_ADDR>>16,AR7	;FIFO ADDRESS
    // asm: 	LS	16,AR7
    // asm: 	LDI	BK,AR6			;# of polygons-1
    // asm: ILLUM_PLOTPOLYLP
    // 	;CHECK HIDDEN SURFACE REMOVAL
    // asm: 	LDI	*+AR1(4),R0		;read vertex (v4|v3|v2|v1)
    // asm: 	LDI	R0,AR4			;
    // asm: 	AND	0FFh,AR4		;v1
    // asm: 	MPYI	3,AR4
    // asm: 	LDI	R0,AR5			;
    // asm: 	RS	8,AR5			;
    // asm: 	AND	0FFh,AR5		;v2
    // asm: 	MPYI	3,AR5
    // asm: 	LDI	R0,AR2			;
    // asm: 	RS	16,AR2			;
    // asm: 	AND	0FFh,AR2		;v3
    // asm: 	MPYI	3,AR2
    // asm: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm: 	SUBF	*+AR4(IR1),*+AR5(IR1),R5	;dy = ay - by
    // asm: 	SUBF	*+AR2(IR0),*+AR5(IR0),R0	;ex = cx - bx
    // asm: 	MPYF	R5,R0,R0			;ex = dy * ex
    // asm: 	MPYF	R1,R2			;ey = dx * ey
    // asm: 	SUBF	R0,R2			;ey = ey - ex
    // asm: 	BGT	ZCLIP		;if back facing DONT PLOT
    // *
    // *GET ILLUMINATION PALETTE TO BE USED
    // *ROTATE NORMAL VECTOR BY OBJECT/UNIVERSE RAOTATION MATRIX
    // *
    // asm: ILLUM1
    // asm: 	LDI	*AR1++,R7			;get control word
    // ;	LDP	@tmpmatY			;DP loaded with low memory area
    // asm: 	LDI	@tmpmatY,AR3			;
    // asm: 	LDI	@transmatrixI,AR5		;these are in same memory area
    // asm: 	LDF	*AR1++,R3			;get the NORMAL.x
    // asm: 	LDF	*AR1++,R4			;	       .y
    // asm: 	LDF	*AR1++,R5			;	       .z
    // asm: 	NOP 	*AR5++(8)		   	;FAST ADD TO AR5
    // asm: 	MPYF	*AR5--,R5,R0
    // asm: 	MPYF	*AR5--,*AR3,R1
    // asm: 	MPYF	*AR5--,*-AR3(1),R0
    // asm: 	ADDF	R0,R2,R1
    // ; 	BND	ZCLIP1			     	;BLOW OUT OF HERE, Z NEG IS BACKFACER
    // asm: 	MPYF	*AR5--,R5,R0
    // asm: 	MPYF	*AR5--,R4,R2
    // asm: 	MPYF	*AR5--,*-AR3(1),R0
    // asm: 	MPYF	*AR5--,*+AR3(1),R0
    // asm: 	MPYF	*AR5--,R4,R3
    // asm: 	MPYF	*AR5--,*-AR3(1),R0
    // asm: 	ADDF	R0,R3,R3
    // 	;R1=Z, R2=Y, R3=X ROTATED NORMAL
    // 	;GENERATE ILLUMINATION DOT PRODUCT
    // 	;
    // asm: 	LDI	@LIGHTIY,AR5			;again, DP in same memory area
    // asm: 	MPYF	*-AR5(1),R3,R3			;generate dot product to get
    // asm: 	MPYF	*AR5,R2,R2			;illumination level
    // asm: 	MPYF	*+AR5(1),R1,R1
    // asm: 	ADDF	R2,R1,R5
    // asm: 	ADDF	R3,R5
    // asm: 	MPYF	-8,R5				;actually:  R1 = (int) ((R1*.-5)+.5)*16
    // asm: 	ADDF	8,R5				;the 2 is because illum pals begin at PAL2
    // asm: 	FIX	R5				;R1 now has illumintation level index (0-15)
    // asm: 	LDI	FASTCC,R7				;fake it out
    // ;	OR	R6,R7				;or in the illum color
    // asm: 	OR	R5,R7		;or in color spec
    // asm: 	LDI	200h,R5		;second palette
ILLUMFF:
    // asm: LDI	*AR0,R0				;FIFO_WT replacement
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZ	ILLUMFF				;ILLUM FIFO WAIT
    // asm: 	STI	R7,*AR7				;CONTROL WORD from above
    // ;	LSH	8,R5	;fix l8r
    // asm: 	STI	R5,*AR7				;PALETTE
    // asm: 	LDI	*AR1++,AR2
    // asm: 	CLRI	R3
    // asm: 	LDI	3,RC
    // asm: 	RPTB	LP1
    // asm: 	LDI	AR2,AR3
    // asm: 	LSH	R3,AR3
    // asm: 	AND	0FFh,AR3
    // asm: 	MPYI	3,AR3
    // asm: 	SUBI	8,R3
    // asm: 	FIX	*+AR3(IR0),R0
    // asm: 	FIX	*+AR3(IR1),R0
LP1:
    // asm: STI	R0,*AR7
    // asm: 	LDI	*+AR0(FIFO_INC-FIFO_STATUS),R0	;FIFO_INC replacement
    // asm: ILLUM_POLYLP
    // asm: 	DBU	AR6,ILLUM_PLOTPOLYLP
    // asm: 	POP	AR0
    // asm: 	RETS
ZCLIP:
    // asm: 	ADDI	5,AR1
    // asm: 	DBU	AR6,ILLUM_PLOTPOLYLP
    // asm: 	POP	AR0
    // asm: 	RETS
    // asm: ZCLIP1
    // asm: 	ADDI	5,AR1
    // asm: 	DBU	AR6,ILLUM_PLOTPOLYLP
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLOTILLUM", 0, 0);
    UNIMPL();
}

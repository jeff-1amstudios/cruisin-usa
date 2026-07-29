#include "dirq.h"

#include "../core/machine.h"
#include "../core/output.h"
#include "../core/validator.h"
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
#include <stdio.h>

/*
 * Source module: asm/DIRQ.ASM
 */

void DIRQ(void);
static void DISPLAY(OBJ* obj);
static void TRANS2D(void);
static void DYNAMIC_OBJECT(OBJ* obj, const u32* rom_ptr, const MATRIX* object_camera_matrix);
static void PLOTPOLY(OBJ* obj /*AR0*/, const ROM_POLYGON* polygons /*AR1*/, int polygon_count_minus_one /*BK*/);
static int CLIPCK(const c3x_f32_t* vertex1 /*AR4*/, const c3x_f32_t* vertex2 /*AR5*/, const c3x_f32_t* vertex3 /*AR2*/, const c3x_f32_t* vertex4 /*AR3*/, int* clipram /*AR0*/);
static void CLIP(int* clipram /*AR0*/, int palette_base /*R0*/, int control_word /*R2*/, const ROM_POLYGON* polygon /*AR1*/);
static void PLTPOLY(void);
static void PLOT1PAL(OBJ* obj /*AR0*/, const ROM_POLYGON* polygons /*AR1*/, int polygon_count_minus_one /*BK*/);
static void PLT1PAL(void);
static void PLOTILLUM(OBJ* obj, const ROM_ILLUM_POLYGON* polygons, int polygon_count_minus_one);

static void dirq_load_obj_matrix(MATRIX* dst, const OBJ* obj) {
    dst->a00 = C3X_STF(C3X_REG(obj->omatrix.mat00));
    dst->a01 = C3X_STF(C3X_REG(obj->omatrix.mat10));
    dst->a02 = C3X_STF(C3X_REG(obj->omatrix.mat20));
    dst->a10 = C3X_STF(C3X_REG(obj->omatrix.mat01));
    dst->a11 = C3X_STF(C3X_REG(obj->omatrix.mat11));
    dst->a12 = C3X_STF(C3X_REG(obj->omatrix.mat21));
    dst->a20 = C3X_STF(C3X_REG(obj->omatrix.mat02));
    dst->a21 = C3X_STF(C3X_REG(obj->omatrix.mat12));
    dst->a22 = C3X_STF(C3X_REG(obj->omatrix.mat22));
}

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

MATRIX LOCTEMPER_MAT;

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
c3x_reg_t SCRNHXI = C3X_INIT(SCRNHX, 0x0800000000ull);
/* asm: SCRNHYI			.float	SCRNHY */
/* asm: 	 */
c3x_reg_t SCRNHYI = C3X_INIT(SCRNHY, 0x0748000000ull);
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
static c3x_f32_t POSTERMATRIX2D[4];

static u32 CLIPRAM[CLIPRAML];

// used for debugging
OBJ* BREAKOBJ;

c3x_f32_t BLOWLIST[768];

static int g_dirq_debug_frame;
static int g_dirq_debug_objects;
static int g_dirq_debug_objects_with_rom;
static int g_dirq_debug_objects_after_z_clip;
static int g_dirq_debug_plotpoly_calls;
static int g_dirq_debug_plot1pal_calls;
static int g_dirq_debug_clipck_calls;
static int g_dirq_debug_clipck_clipped;
static int g_dirq_debug_plot1pal_polygons;
static int g_dirq_debug_plot1pal_z_rejects;
static int g_dirq_debug_plot1pal_hsr_rejects;
static int g_dirq_debug_plot1pal_glitch_rejects;
static int g_dirq_debug_plot1pal_clip_rejects;
static int g_dirq_debug_plot1pal_emits;
static int g_dirq_debug_plot1pal_first_obj_id;
static int g_dirq_debug_plot1pal_first_poly;
static c3x_reg_t g_dirq_debug_plot1pal_first_ax;
static c3x_reg_t g_dirq_debug_plot1pal_first_ay;
static c3x_reg_t g_dirq_debug_plot1pal_first_bx;
static c3x_reg_t g_dirq_debug_plot1pal_first_by;
static c3x_reg_t g_dirq_debug_plot1pal_first_cx;
static c3x_reg_t g_dirq_debug_plot1pal_first_cy;
static c3x_reg_t g_dirq_debug_plot1pal_first_cross;

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
    int syscntl;

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

    g_dirq_debug_objects = 0;
    g_dirq_debug_objects_with_rom = 0;
    g_dirq_debug_objects_after_z_clip = 0;
    g_dirq_debug_plotpoly_calls = 0;
    g_dirq_debug_plot1pal_calls = 0;
    g_dirq_debug_clipck_calls = 0;
    g_dirq_debug_clipck_clipped = 0;
    g_dirq_debug_plot1pal_polygons = 0;
    g_dirq_debug_plot1pal_z_rejects = 0;
    g_dirq_debug_plot1pal_hsr_rejects = 0;
    g_dirq_debug_plot1pal_glitch_rejects = 0;
    g_dirq_debug_plot1pal_clip_rejects = 0;
    g_dirq_debug_plot1pal_emits = 0;
    g_dirq_debug_plot1pal_first_obj_id = -1;
    g_dirq_debug_plot1pal_first_poly = -1;
    g_dirq_debug_plot1pal_first_ax = C3X_FROM_INT(0);
    g_dirq_debug_plot1pal_first_ay = C3X_FROM_INT(0);
    g_dirq_debug_plot1pal_first_bx = C3X_FROM_INT(0);
    g_dirq_debug_plot1pal_first_by = C3X_FROM_INT(0);
    g_dirq_debug_plot1pal_first_cx = C3X_FROM_INT(0);
    g_dirq_debug_plot1pal_first_cy = C3X_FROM_INT(0);
    g_dirq_debug_plot1pal_first_cross = C3X_FROM_INT(0);
    crusn_debug_output_reset_frame();

    syscntl = SYSCNTL | LED_OFF;
    SYSCNTL = syscntl;

    FIND_YMATRIX(&POSTERMATRIX, C3X_NEG(_CAMERARAD.Y));
    CONCATMAT(&POSTERMATRIX, &_CAMERAMATRIX, &POSTERMATRIX);
    POSTERMATRIX2D[0] = POSTERMATRIX.a00;
    POSTERMATRIX2D[1] = POSTERMATRIX.a02;
    POSTERMATRIX2D[2] = POSTERMATRIX.a20;
    POSTERMATRIX2D[3] = POSTERMATRIX.a22;
#if STATISTICS
    ST_OBJECTS = 0;
    ST_POLYGONS = 0;
#endif
    DISPLAY(OLOW_PRIORITY);
    DISPLAY(OACTIVE);
    DISPLAY(OACTIVE_PRIORITY);
    DISPLAY(OHIGH_PRIORITY);

    g_dirq_debug_frame += 1;

    syscntl = SYSCNTL & ~LED_OFF;
    SYSCNTL = syscntl;
}

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

static void DISPLAY(OBJ* obj /*AR0*/) {
    MATRIX object_matrix;
    u32 flags;
    const u32* rom_ptr;
    s32 clip_radius;
    const MATRIX* matrix;
    const ROM_VERTEX* vertices;
    const ROM_POLYGON* polygons;
    int counts_word;
    int vertex_count_minus_one;
    c3x_reg_t trans_x;
    c3x_reg_t trans_y;
    c3x_reg_t trans_z;
    c3x_reg_t rotated_trans_x;
    c3x_reg_t rotated_trans_y;
    c3x_reg_t rotated_trans_z;

    // DISPLAY entry point moved from bottom of function
    // asm 00000166: 	LDI	*AR0,R0
    // asm 00000167: 	BNZ	NEXTOBJ
    if (obj == NULL) {
        return;
    }
NEXTOBJ:

    // asm 00000091: 	LDI	R0,AR0
    // asm 00000092: 	SETDP
    // asm 00000093: 	LDI	@BREAKOBJ,R1
    // asm 00000094: 	BZ	NOBREAK_CONTINUE
    // asm 00000095: 	CMPI	R1,R0
    // asm 00000096: 	BEQ	$
    if (BREAKOBJ != NULL && obj == BREAKOBJ) {
        // dont repro spin loop for original debugging...
        abort();
    }
NOBREAK_CONTINUE:
    g_dirq_debug_objects += 1;
#if STATISTICS
    // asm: 	LDI	@ST_OBJECTS,R1
    // asm: 	INC	R1
    // asm: 	STI	R1,@ST_OBJECTS
    ST_OBJECTS += 1;
#endif
    flags = obj->flags;
    rom_ptr = (const u32*)obj->romdata;
    if (rom_ptr == NULL) {
        goto DISPLAY_NEXT_IMPL;
    }
    g_dirq_debug_objects_with_rom += 1;

    if ((flags & O_NOUNIV) != 0) {
        trans_x = C3X_LDF(obj->pos.X);
        trans_y = C3X_LDF(obj->pos.Y);
        trans_z = C3X_LDF(obj->pos.Z);
    } else {
        trans_x = C3X_SUB(obj->pos.X, _CAMERAPOS.X);
        trans_y = C3X_SUB(obj->pos.Y, _CAMERAPOS.Y);
        trans_z = C3X_SUB(obj->pos.Z, _CAMERAPOS.Z);
    }

    if ((flags & O_NOUROT) != 0) {
        rotated_trans_x = trans_x;
        rotated_trans_y = trans_y;
        rotated_trans_z = trans_z;
    } else {
        rotated_trans_x = C3X_ADD(C3X_ADD(C3X_MUL(_CAMERAMATRIX.a00, trans_x), C3X_MUL(_CAMERAMATRIX.a01, trans_y)), C3X_MUL(_CAMERAMATRIX.a02, trans_z));
        rotated_trans_y = C3X_ADD(C3X_ADD(C3X_MUL(_CAMERAMATRIX.a10, trans_x), C3X_MUL(_CAMERAMATRIX.a11, trans_y)), C3X_MUL(_CAMERAMATRIX.a12, trans_z));
        rotated_trans_z = C3X_ADD(C3X_ADD(C3X_MUL(_CAMERAMATRIX.a20, trans_x), C3X_MUL(_CAMERAMATRIX.a21, trans_y)), C3X_MUL(_CAMERAMATRIX.a22, trans_z));
    }
    TRANSVECTOR.X = C3X_STF(C3X_REG(rotated_trans_x));
    TRANSVECTOR.Y = C3X_STF(C3X_REG(rotated_trans_y));
    TRANSVECTOR.Z = C3X_STF(C3X_REG(rotated_trans_z));
    obj->dist = FIX(rotated_trans_z);

    if ((flags & O_DEGRADE) != 0) {
        if (obj->dist > DEGRADE_DIST && obj->degrade_rom != NULL) {
            rom_ptr = (const u32*)obj->degrade_rom;
        }
        if ((flags & O_DEGRADE2) != 0 && obj->dist > DEGRADE_DIST_LEVEL2 && obj->degrade_rom2 != NULL) {
            rom_ptr = (const u32*)obj->degrade_rom2;
        }
    }

    clip_radius = (s32)rom_ptr[0];
    if (C3X_LT(C3X_ADD(rotated_trans_z, C3X_FROM_INT(clip_radius)), C3X_FROM_INT(LOW_CLIP_LEVEL))) {
        goto DISPLAY_NEXT_IMPL;
    }
    g_dirq_debug_objects_after_z_clip += 1;
    dirq_load_obj_matrix(&object_matrix, obj);

    if ((flags & O_NOUROT) != 0 && (flags & O_ILLUM) == 0) {
        matrix = &object_matrix;
    } else if ((flags & O_POSTER) != 0) {
        if (C3X_LE(TRANSVECTOR.Z, C3X_FROM_INT(POSTERCLIP))) {
            goto DISPLAY_NEXT_IMPL;
        }
        matrix = &POSTERMATRIX;
    } else if ((flags & O_NOROT) != 0) {
        if ((flags & O_IROT) != 0) {
            matrix = &object_matrix;
        } else {
            matrix = &_CAMERAMATRIX;
        }
    } else {
        CONCATMAT(&object_matrix, &_CAMERAMATRIX, &ROTATION_MATRIX);
        matrix = &ROTATION_MATRIX;
    }

    if ((flags & O_ILLUM) != 0 && matrix != &ROTATION_MATRIX) {
        ROTATION_MATRIX = *matrix;
    }

    if ((flags & O_DYNAMIC) != 0
        && rom_ptr == (const u32*)obj->romdata) {
        DYNAMIC_OBJECT(obj, rom_ptr, matrix);
        goto DISPLAY_NEXT_IMPL;
    }

    counts_word = (int)rom_ptr[1];
    vertex_count_minus_one = counts_word & 0xffff;
    vertices = (const ROM_VERTEX*)&rom_ptr[2];
    polygons = (const ROM_POLYGON*)(vertices + (vertex_count_minus_one + 1));
#if STATISTICS
    ST_VERTICES += vertex_count_minus_one + 1;
#endif
    for (int vertex_index = 0; vertex_index <= vertex_count_minus_one; vertex_index++) {
        int packed_xy;
        c3x_reg_t x;
        c3x_reg_t y;
        c3x_reg_t z;
        c3x_reg_t rotated_x;
        c3x_reg_t rotated_y;
        c3x_reg_t rotated_z;
        c3x_reg_t world_x;
        c3x_reg_t world_y;
        c3x_reg_t world_z;
        c3x_reg_t inverse_z;
        int blow_index;
        int inverse_index;

        packed_xy = (int)vertices[vertex_index].x_y;
        x = C3X_FROM_INT((int16_t)(packed_xy & 0xffff));
        y = C3X_FROM_INT((int16_t)((u32)packed_xy >> 16));
        z = C3X_FROM_INT(vertices[vertex_index].z);

        rotated_x = C3X_ADD(C3X_ADD(C3X_MUL(matrix->a00, x), C3X_MUL(matrix->a01, y)), C3X_MUL(matrix->a02, z));
        rotated_y = C3X_ADD(C3X_ADD(C3X_MUL(matrix->a10, x), C3X_MUL(matrix->a11, y)), C3X_MUL(matrix->a12, z));
        rotated_z = C3X_ADD(C3X_ADD(C3X_MUL(matrix->a20, x), C3X_MUL(matrix->a21, y)), C3X_MUL(matrix->a22, z));

        world_x = C3X_ADD(rotated_x, TRANSVECTOR.X);
        world_y = C3X_ADD(rotated_y, TRANSVECTOR.Y);
        world_z = C3X_ADD(rotated_z, TRANSVECTOR.Z);
        inverse_index = FIX(world_z);
        inverse_index >>= 4;
        if (inverse_index > HIGH_CLIP_LEVEL) {
            inverse_index = HIGH_CLIP_LEVEL;
        }
        if (inverse_index < -80) {
            inverse_index = -80;
        }
        inverse_z = C3X_LDF(INVTAB[inverse_index]);

        blow_index = vertex_index * 3;
        BLOWLIST[blow_index] = C3X_STF(C3X_ADD(C3X_MUL(world_x, inverse_z), SCRNHXI));
        BLOWLIST[blow_index + 1] = C3X_STF(C3X_ADD(C3X_MUL(C3X_MUL(world_y, inverse_z), C3X_IMM_F32(1.04)), SCRNHYI));
        BLOWLIST[blow_index + 2] = C3X_STF(world_z);
    }

    PLOTPOLY(obj, polygons, (counts_word >> 16) & 0xffff);

DISPLAY_NEXT_IMPL:
    obj = obj->link;
    if (obj != NULL) {
        goto NEXTOBJ;
    }
    return;

    // *GENERATE TRANSLATION VECTOR
    // asm 00000097: 	LDI	R0,AR3			;transform the objects position
    // asm 00000098: 	LDI	*+AR0(OFLAGS),R6	;holds the OBJECTS flags
    // asm 00000099: 	LDI	*+AR0(OROMDATA),AR1	;get the romptr
    // asm 0000009A: 	TSTB	O_NOUNIV,R6		;has this object have an absolute distance from
    // asm 0000009B: 	BZD	TRU_UNIV		;CAMERAPOS or is it a regular object
    // asm 0000009C: 	ADDI	OPOSX,AR3		;for the CAMERAMATRIX
    // asm 0000009D: 	LDI	@CAMERAPOSI,AR6		;universe_center
    // asm 0000009E: 	LDI	@tmpmatI,AR4		;trans = object_pos + univ_pos
    // 	;---->BZ TRU_UNIV
    // asm 0000009F: 	BUD	TRANS_RET		;this object has an absolute distance from
    // asm 000000A0: 	LDF	*AR3++,R3		;the cemeras position therefore its relative
    // asm 000000A1: 	LDF	*AR3++,R4		;position in our calculations does not change
    // asm 000000A2: 	LDF	*AR3++,R5		;
TRU_UNIV:
    // asm 000000A3: 	SUBF	*AR6++,*AR3++,R3	;*trans++ = *tp++ - *up++
    // asm 000000A4: 	SUBF	*AR6++,*AR3++,R4	;*trans++ = *tp++ - *up++
    // asm 000000A5: 	SUBF	*AR6++,*AR3++,R5	;*trans++ = *tp++ - *up++
TRANS_RET:
    // asm 000000A6: 	TSTB	O_NOUROT,R6		;is this object NOT rotatable by the CAMERAMATRIX?
    // asm 000000A7: 	BZD	UNIV_ROT		;BR -> it is
    // asm 000000A8: 	LDI	@CAMERAMATRIXI,AR5	;src 3x3
    // asm 000000A9: 	LDI	@transvectorYI,AR6	;dst 1x3
    // asm 000000AA: 	NOP
    // 	;---->BZ UNIV_ROT
    // *SPECIAL CASE WHEN WE WANT SOMETHING NOT ROTATED BY THE
    // *UNIVMATRIX  (it has absolute coordinates)
    // asm 000000AB: 	STF	R3,*-AR6(1)		;TRANSVECTOR.x
    // asm 000000AC: 	BUD	TRIVIAL_REJECTION
    // asm 000000AD: 	STF	R4,*AR6			;TRANSVECTOR.y
    // asm 000000AE: 	LDF	R5,R2			;Z value
    // asm 000000AF: 	ADDI	9,AR5
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
    // asm 000000B0: 	MPYF3	*AR5++,R3,R0
    // asm 000000B0:  ||	STF	R3,*AR4			;SAVE X VECTOR VALUE (TMPMAT.x)
    // asm 000000B1: 	MPYF3	*AR5++,R4,R1
    // asm 000000B1:  ||	STF	R5,*+AR4(1)		;SAVE Z VECTOR VALUE (TMPMAT.z)
    // asm 000000B2: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm 000000B2:  ||	ADDF3	R0,R1,R2
    // asm 000000B3: 	MPYF3	*AR5++,*AR4,R0
    // asm 000000B3:  ||	ADDF3	R1,R2,R2
    // asm 000000B4: 	MPYF3	*AR5++,R4,R1
    // asm 000000B4:  ||	STF	R2,*-AR6(1)		;(TRANSVECTOR.x)
    // asm 000000B5: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm 000000B5:  ||	ADDF3	R0,R1,R2
    // asm 000000B6: 	MPYF3	*AR5++,*AR4,R0
    // asm 000000B6:  ||	ADDF3	R1,R2,R2
    // asm 000000B7: 	MPYF3	*AR5++,R4,R1
    // asm 000000B7:  ||	STF	R2,*AR6			;(TRANSVECTOR.y)
    // asm 000000B8: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm 000000B8:  ||	ADDF3	R0,R1,R2
    // asm 000000B9: 	ADDF	R1,R2
    // *
    // *CHECK FOR TRIVIAL REJECTION
    // *
TRIVIAL_REJECTION:
    // asm 000000BA: 	TSTB	O_DEGRADE,R6		;does this have a degrade model?
    // asm 000000BB: 	BZD	NO_DEGRADE
    // asm 000000BC: 	STF	R2,*+AR6(1)		;(TRANSVECTOR.z) R2 = Z value of object
    // asm 000000BD: 	FIX	R2			;convert Z distance to integer
    // asm 000000BE: 	STI	R2,*+AR0(ODIST)
    // 	;---->BZ NO_DEGRADE
    // asm 000000BF: 	CMPI	DEGRADE_DIST,R2
    // asm 000000C0: 	LDIGT	*+AR0(ODEGRADE_ROM),AR1	;if its far enough to degrade it
    // asm 000000C1: 	TSTB	O_DEGRADE2,R6
    // asm 000000C2: 	BZ	NO_DEGRADE		;LEVEL 2
    // asm 000000C3: 	CMPI	DEGRADE_DIST_LEVEL2,R2
    // asm 000000C4: 	LDIGT	*+AR0(ODEGRADE_ROM2),AR1	;if its far enough to degrade it to level 2
NO_DEGRADE:
    // asm 000000C5: 	LDI	*AR1++,R4		;get RADIUS of object
    // asm 000000C6: 	ADDI	R2,R4,R3
    // asm 000000C7: 	CMPI	LOW_CLIP_LEVEL,R3	;attempt to toss on Z distance
    // asm 000000C8: 	BLTD	DISPLAY_NEXT			;is it to close?
    // asm 000000C9: 	SUBI	R4,R3
    // asm 000000CA: 	SUBI	R4,R3
    // asm 000000CB: 	CMPI	@HIGH_CLIP_LEV8,R3
    // 	;----> BLTD	DISPLAY
    // asm 000000CC: 	BGTD	DISPLAY_NEXT
    // asm 000000CD: 	ADDI	R4,R3
    // asm 000000CE: 	LDIN	0,R3			;KEEP IT IN RANGE
    // asm 000000CF: 	LSH	-4,R3			;quickly divide by 16
    // 	;---->BGT DISPLAY
    // asm 000000D0: 	CMPI	HIGH_CLIP_LEVEL,R3	;KEEP INDEX IN RANGE
    // asm 000000D1: 	LDIGT	HIGH_CLIP_LEVEL,R3
    // asm 000000D2: 	LDI	R3,AR3
    // asm 000000D3: 	ADDI	@INVTABI,AR3
    // asm 000000D4: 	FLOAT	R4			;RADIUS must be positive
    // asm 000000D5: 	LDF	*AR3,R1			;get the inverse Z factor
    // asm 000000D6: 	MPYF	R1,R4			;project RADIUS for distance
    // asm 000000D7: 	MPYF	*AR6,R1,R3		;project Y position
    // asm 000000D8: 	ADDF	R4,R3,R2
    // asm 000000D9: 	ADDF	@SCRNHYI,R2
    // asm 000000DA: 	BLTD	DISPLAY_NEXT			;BR-> above the screen
    // asm 000000DB: 	NOP				;PAD FOR DELAYED BRANCH
    // asm 000000DC: 	SUBF	R4,R3
    // asm 000000DD: 	CMPF	@SCRNHYI,R3
    // 	;---->BLT DISPLAY
    // asm 000000DE: 	BGTD	DISPLAY_NEXT			;BR-> below the screen
    // asm 000000DF: 	MPYF	*-AR6(1),R1,R3	   	;project X position
    // asm 000000E0: 	ADDF	R4,R3,R2
    // asm 000000E1: 	ADDF	@SCRNHXI,R2
    // 	;---->BGT DISPLAY
    // asm 000000E2: 	BLTD	DISPLAY_NEXT			;BR-> too far to the left
    // asm 000000E3: 	NOP			   	;PAD FOR DELAYED BRANCH
    // asm 000000E4: 	SUBF	R4,R3
    // asm 000000E5: 	CMPF	@SCRNHXI,R3
    // 	;---->BLT DISPLAY
    // asm 000000E6: 	BGTD	DISPLAY_NEXT			;BR-> too far to the right
    // 	;***
    // 	;***	END TRIVIAL REJECTION, WE CAN PROBABLY SEE IT
    // 	;***
    // 	;
    // 	;jump to 2D altering
    // 	;
    // asm 000000E7: 	LDI	OMATRIX,AR4
    // asm 000000E8: 	ADDI	AR0,AR4
    // asm 000000E9: 	LDI	*+AR0(OFLAGS),R0
    // 	;---->BGT DISPLAY			;(object is not being seen)
    // asm 000000EA: 	TSTB	O_DYNAMIC,R0
    // asm 000000EB: 	BNZ	ROT3D
    // asm 000000EC: 	TSTB	O_ILLUM,R0
    // asm 000000ED: 	BNZ	ROT3D
    // asm 000000EE: 	RS	(O_3DROT_B+1),R0
    // asm 000000EF: 	BC	ROT3D
    // asm 000000F0: 	LDI	@_MODE,R0
    // asm 000000F1: 	AND	MMODE,R0
    // asm 000000F2: 	CMPI	MGAME,R0
    // asm 000000F3: 	BNE	ROT3D
    // asm 000000F4: 	LDI	@CAMVIEW,R0
    // asm 000000F5: 	BNZ	TRANS2D
ROT3D:
    // 	;***
    // 	;***	GENERATE ROTATION MATRIX
    // 	;***
    // 	;***
    // 	;***	TEST FOR SPECIAL ROTATION CASES
    // 	;***
    // asm 000000F6: 	TSTB	O_NOUROT,R6		;is this object NOT rotatable by the CAMERAMATRIX?
    // asm 000000F7: 	BZ	CHECKTHEREG
    // asm 000000F8: 	TSTB	O_ILLUM,R6		;kludge for illuminated objects
    // asm 000000F9: 	BNZ	CHECKTHEREG		;fix in next system
    // asm 000000FA: 	LDI	AR0,AR5
    // asm 000000FB: 	ADDI	OMATRIX,AR5
    // asm 000000FC: 	BU	VT
CHECKTHEREG:
    // *O_POSTER
    // *A poster matrix is oriented by the X/Z in the universe, but is not
    // *rotated by Y.  Simply put:  a poster is an object in the universe
    // *that is always facing the user. -> a poster.
    // asm 000000FD: 	TSTB	O_POSTER,R6
    // asm 000000FE: 	BZ	REGULAR
    // asm 000000FF: 	FLOAT	POSTERCLIP,R0
    // asm 00000100: 	CMPF	*+AR6(1),R0		;CHECK IF TOO CLOSE
    // asm 00000101: 	BLED	VT		 	;CLIP OUT POSTERS IF TOO CLOSE
    // asm 00000102: 	NOP
    // asm 00000103: 	LDI	@POSTERMATI,AR5
    // asm 00000104: 	NOP
    // 	;---> BLED VT
    // asm 00000105: 	B	DISPLAY_NEXT
REGULAR:
    // asm 00000106: 	LDI	@transmatrixI,AR3
    // asm 00000107: 	TSTB	O_NOROT,R6		;if this object is not self-orienting
    // asm 00000108: 	BZD	SELF_ORIENTING		;we can save on computations by skipping the concat
    // asm 00000109: 	LDI	5,IR1
    // asm 0000010A: 	LDI	3,IR0
    // asm 0000010B: 	NOP	*--AR5(9)		;RESTORE AR5 = UNIVMATRIX
    // 	;---->BZ NOW
    // asm 0000010C: 	TSTB	O_IROT,R6
    // asm 0000010D: 	BNZ	IDENTITY_ROTATION
    // *O_IROT		IDENTITY_ROTATION
    // *THE OBJECT IS NOT SELF-ORIENTING, THEREFORE WE MAY SIMPLY COPY
    // *THE CAMERA MATRIX INTO THE GENERAL ROTATIONAL MATRIX
    // asm 0000010E: 	BUD	VT
    // asm 0000010F: 	NOP
    // asm 00000110: 	LDI	@CAMERAMATRIXI,AR5
    // asm 00000111: 	NOP
    // 	;---->BU VT
    // *in this case identity rotation means that it CAN be self rotating, that is
    // *entirely dependant on the matrix of the object
IDENTITY_ROTATION:
    // asm 00000112: 	LDI	AR0,AR5
    // asm 00000113: 	ADDI	OMATRIX,AR5
    // asm 00000114: 	BU	VT
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
    // asm 00000115: 	LDI	2,RC				;LOOP THREE TIMES
    // asm 00000116: 	RPTB	INLP1
    // asm 00000117: 	MPYF3	*AR5++(1),*AR4++(IR0),R0
    // asm 00000118: 	MPYF3	*AR5,*AR4++(IR0),R1
    // asm 00000119: 	MPYF3	*+AR5(1),*AR4--(IR1),R1
    // asm 00000119:  ||	ADDF3	R0,R1,R2
    // asm 0000011A: 	MPYF3	*-AR5(1),*AR4++(IR0),R0
    // asm 0000011A:  ||	ADDF3	R1,R2,R2
    // asm 0000011B: 	MPYF3	*AR5,*AR4++(IR0),R1
    // asm 0000011C: 	STF	R2,*AR3++(1)			;store MATij
    // asm 0000011D: 	MPYF3	*+AR5(1),*AR4--(IR1),R1
    // asm 0000011D:  ||	ADDF3	R0,R1,R2
    // asm 0000011E: 	MPYF3	*-AR5(1),*AR4++(IR0),R0
    // asm 0000011E:  ||	ADDF3	R1,R2,R2
    // asm 0000011F: 	MPYF3	*AR5++,*AR4++(IR0),R1
    // asm 00000120: 	STF	R2,*AR3++(1)			;store MATij
    // asm 00000121: 	MPYF3	*AR5++,*AR4--(IR1),R1
    // asm 00000121:  ||	ADDF3	R0,R1,R2
    // asm 00000122: 	ADDF	R1,R2
    // asm 00000123: 	STF	R2,*AR3++(1)			;store MATij
INLP1:
    // asm 00000124: NOP	*--AR4(3)
    // 	;***
    // 	;***	VECTOR ROTATION/TRANSLATION
    // 	;***
VECTOR_TRANSFORMATION:
    // asm 00000125: 	LDI	*+AR0(OFLAGS),R0
    // asm 00000126: 	TSTB	O_DYNAMIC,R0
    // asm 00000127: 	BNZ	DYNAMIC_OBJECT
VECTORTRANSFULL: //;return on dynamic objects

    // asm 00000128: 	LDI	@transmatrixI,AR5	;somewhat dedicated for matrix pointer
VT:
    // asm 00000129: 	LDI	*AR1++,RC		;get number of vertices to translate-1
    // asm 0000012A: 	PUSH	RC
    // asm 0000012B: 	AND	0FFh,RC
#if STATISTICS
    // asm: 	LDI	@ST_VERTICES,R0
    // asm: 	ADDI	RC,R0
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,@ST_VERTICES
#endif
    // asm 0000012C: 	LDI	@BLOWLISTI,AR3		;blowlist pointer = AR3
    // asm 0000012D: 	LDI	@tmpmatY,AR4		;TEMP VECTOR STORE
    // asm 0000012E: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm 0000012F: 	LDI	8,IR0
    // asm 00000130: 	FLOAT	SCREENHX,R6		;screen half x
    // asm 00000131: 	LDI	-16,BK
    // asm 00000132: 	BD	VTL
    // asm 00000133: 	FLOAT	SCREENHY,R7		;screen half x
    // asm 00000134: 	LDF	*-AR6(1),R4		;GET X TRANS
    // asm 00000135: 	LDF	*AR6,R5			;GET Y TRANS
    // asm 00000136: 	BR 	VTL				; jeff edited to match I450 rom
VTL:
    // asm 00000140: 	RPTB	EOVCTR
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
    // asm 00000141: 	LDI	*AR1++,R3
    // asm 00000142: 	ASH	BK,R3,R2    		;BK=16
    // asm 00000143: 	LS	16,R3
    // asm 00000144: 	ASH	BK,R3
    // asm 00000145: 	FLOAT	R3			; x element
    // asm 00000146: 	FLOAT	R2			; y element
    // asm 00000147: 	STF	R3,*-AR4(1)		;store secondary access A
    // asm 00000148: 	FLOAT	*AR1++,R0		;C	get z element of source 1
    // asm 00000148:  ||	STF	R2,*AR4			;store secondary access B
    // 	;
    // 	;MULTIPLY BY ROTATION MATRIX
    // 	;
    // asm 00000149: 	MPYF3	*AR5++,R3,R0		;AD
    // asm 00000149:  ||	STF	R0,*+AR4(1)  		;store secondary access C
    // asm 0000014A: 	MPYF3	*AR5++,R2,R1		;BE
    // asm 0000014B: 	MPYF3	*AR5++,*+AR4(1),R1	;CF
    // asm 0000014B:  ||	ADDF3	R0,R1,R3		;AD+BE
    // asm 0000014C: 	MPYF3	*AR5++,*-AR4(1),R0	;AG
    // asm 0000014C:  ||	ADDF3	R1,R3,R3		;AD+BE+CF
    // asm 0000014D: 	MPYF3	*AR5++,R2,R1		;BH
    // asm 0000014D:  ||	STF	R3,*AR3++ 		;*BLOWLIST++ = [AD+BE+CF];STORE ROTATED X
    // asm 0000014E: 	MPYF3	*AR5++,*+AR4(1),R1	;CI
    // asm 0000014E:  ||	ADDF3	R0,R1,R3		;AG+BH
    // asm 0000014F: 	MPYF3	*AR5++,*-AR4(1),R0	;AJ
    // asm 0000014F:  ||	ADDF3	R1,R3,R3		;AG+BH+CI
    // asm 00000150: 	MPYF3	*AR5++,*AR4,R1		;BK
    // asm 00000151: 	MPYF3	*AR5--(IR0),*+AR4(1),R1	;CL
    // asm 00000151:  ||	ADDF3	R0,R1,R2		;AJ+BK
    // asm 00000152: 	ADDF	R1,R2			;AJ+BK+CL
    // asm 00000153: 	ADDF	*+AR6(1),R2		;add in translation into [AJ+BK+CL]
    // asm 00000154: 	LDF	*-AR3(1),R1		;GET BACK NEW X
    // asm 00000154:  ||	STF	R2,*+AR3(1)		;SAVE NEW Z
    // asm 00000155: 	FIX	R2,IR1			;find z distance for inverse lookup
    // asm 00000156: 	ASH	-4,IR1			;quickly divide by 16
    // asm 00000157: 	CMPI	HIGH_CLIP_LEVEL,IR1   	;compare against highest clip level
    // asm 00000158: 	LDIGE	HIGH_CLIP_LEVEL,IR1    	;max it at highest clip level
    // asm 00000159: 	CMPI	-80,IR1
    // asm 0000015A: 	LDILT	-80,IR1
    // asm 0000015B: 	ADDF	R4,R1			;ADD X TRANSLATION
    // asm 0000015C: 	ADDF	R5,R3			;add Y translation
    // asm 0000015D: 	MPYF	*+AR2(IR1),R1,R0    	;x = (x * inverse [z])
    // asm 0000015E: 	ADDF	R6,R0			;    + SCRNHX 	       ;(screen half x)
    // asm 0000015F: 	MPYF	*+AR2(IR1),R3,R0	;y = (y * inverse [z])
    // asm 0000015F:  ||	STF	R0,*-AR3(1)
    // asm 00000160: 	MPYF	1.04,R0			; below 1.0 shrinks Y, above expands Y
    // asm 00000161: 	ADDF	R7,R0			;    + SCRNHY	       ;(screen half y)
EOVCTR:
    // asm 00000162: STF	R0,*AR3++(2)
    // 	;34 Instructions
POLYPOLY_ENTER:
    // asm 00000163: 	POP	BK
    // asm 00000164: 	RS	16,BK
    // asm 00000165: 	CALL	PLOTPOLY
DISPLAY_NEXT:
    // asm 00000166: 	LDI	*AR0,R0
    // asm 00000167: 	BNZ	NEXTOBJ
DISPLAYX:
    // asm 00000168: 	RETS

TRANS2D:

    // 	;***
    // 	;***	GENERATE ROTATION MATRIX
    // 	;***
    // asm 00000169: 	ADDI	1,AR4
    // asm 0000016A: 	LDI	@transmatrixI,AR3
    // asm 0000016B: 	NOP	*--AR5(8)		;RESTORE AR5 = UNIVMATRIX
    // asm 0000016C: 	TSTB	O_POSTER,R6
    // asm 0000016D: 	BZ	REGULAR1
    // asm 0000016E: 	FLOAT	POSTERCLIP,R0
    // asm 0000016F: 	CMPF	*+AR6(1),R0		;CHECK IF TOO CLOSE
    // asm 00000170: 	BLED	VT2		 	;CLIP OUT POSTERS IF TOO CLOSE
    // asm 00000171: 	NOP
    // asm 00000172: 	LDI	@POSTERMAT2DI,AR5
    // asm 00000173: 	NOP
    // 	;---> BLED VT2
    // asm 00000174: 	B	DISPLAY_NEXT
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
SELF_ORIENTING2:
    // asm 00000175: 	LDI	5,IR0
    // asm 00000176: 	LDI	7,IR1
    // asm 00000177: 	MPYF	*-AR5(1),*-AR4(1),R0	;(AJ)
    // asm 00000178: 	MPYF	*+AR5(IR0),*+AR4(1),R1	;(GL)
    // asm 00000179: 	ADDF	R0,R1			;(AJ+GL)
    // asm 0000017A: 	STF	R1,*AR3++
    // asm 0000017B: 	MPYF	*+AR5(1),*-AR4(1),R0	;(CJ)
    // asm 0000017C: 	MPYF	*+AR5(IR1),*+AR4(1),R1	;(IL)
    // asm 0000017D: 	ADDF	R0,R1			;(CJ+IL)
    // asm 0000017E: 	STF	R1,*AR3++
    // asm 0000017F: 	MPYF	*-AR5(1),*+AR4(IR0),R0	;(AP)
    // asm 00000180: 	MPYF	*+AR5(IR0),*+AR4(IR1),R1 ;(GR)
    // asm 00000181: 	ADDF	R0,R1			;(AP+GR)
    // asm 00000182: 	STF	R1,*AR3++
    // asm 00000183: 	MPYF	*+AR5(1),*+AR4(IR0),R0	;(CP)
    // asm 00000184: 	MPYF	*+AR5(IR1),*+AR4(IR1),R1 ;(IR)
    // asm 00000185: 	ADDF	R0,R1			;(CP+IR)
    // asm 00000186: 	STF	R1,*AR3++
    // 	;END MATRIX MULTIPLY
    // 	;***
    // 	;***	VECTOR ROTATION/TRANSLATION
    // 	;***

VECTOR_TRANSFORMATION2:
    // asm 00000187: 	LDI	@transmatrixI,AR5	;somewhat dedicated for matrix pointer
VT2:
    // asm 00000188: 	LDI	*AR1++,RC		;get number of vertices to translate-1
    // asm 00000189: 	PUSH	RC
    // asm 0000018A: 	AND	0FFh,RC			;NUM VERTICES
    // asm 0000018B: 	LDI	@BLOWLISTI,AR3		;blowlist pointer = AR3
    // asm 0000018C: 	LDI	@tmpmatY,AR4		;TEMP VECTOR STORE
    // asm 0000018D: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm 0000018E: 	BD	VTL2
    // asm 0000018F: 	FLOAT	SCREENHX,R6		;screen half x
    // asm 00000190: 	LDI	3,IR0
    // asm 00000191: 	LDI	-16,BK
    // asm 00000192: 	BR 	VTL2				; jeff edited to match I450 rom
VTL2:
    // asm 000001A0: 	RPTB	EOVCTR2
    // asm 000001A1: 	LDI	*AR1++,R3
    // asm 000001A2: 	ASH	BK,R3,R7    		;
    // asm 000001A3: 	LS	16,R3
    // asm 000001A4: 	ASH	BK,R3
    // asm 000001A5: 	FLOAT	R3			;(A)  R3 <- X element
    // asm 000001A6: 	FLOAT	R7			;     R2 <- Y element  (TRANSLATED ONLY)
    // asm 000001A7: 	FLOAT	*AR1++,R0		;(B)  R0 <- z element
    // 	;
    // 	;MULTIPLY BY 2x2 ROTATION MATRIX
    // 	;
    // asm 000001A8: 	MPYF	*AR5++,R3,R2		;(AD)
    // asm 000001A9: 	MPYF	*AR5++,R0,R1		;(BE)
    // asm 000001A9:  ||	ADDF	*-AR6(1),R2		;add in translation into [AJ+BK+CL]
    // asm 000001AA: 	ADDF	R2,R1
    // asm 000001AB: 	MPYF	*AR5++,R3,R3		;(AD)
    // asm 000001AB:  ||	STF	R1,*AR3++ 		;*BLOWLIST++ = ROTATED X (skip Y element)
    // asm 000001AC: 	MPYF	*AR5--(IR0),R0		;(BE)
    // asm 000001AC:  ||	ADDF	*+AR6(1),R3		;add in Z translation
    // asm 000001AD: 	ADDF	R3,R0
    // asm 000001AE: 	ADDF	*AR6,R7			;add in Y translation into [AJ+BK+CL]
    // asm 000001AE:  ||	STF	R0,*+AR3(1) 		;*BLOWLIST++ = ROTATED Z
    // asm 000001AF: 	FIX	R0,IR1			;find z distance for inverse lookup
    // asm 000001B0: 	ASH	-4,IR1			;quickly divide by 16
    // asm 000001B1: 	CMPI	HIGH_CLIP_LEVEL,IR1   	;compare against highest clip level
    // asm 000001B2: 	LDIGE	HIGH_CLIP_LEVEL,IR1    	;max it at highest clip level
    // asm 000001B3: 	CMPI	-80,IR1
    // asm 000001B4: 	LDILT	-80,IR1
    // asm 000001B5: 	MPYF	*+AR2(IR1),R1,R0    	;x = (x * inverse [z])
    // asm 000001B6: 	ADDF	R6,R0			;    + SCRNHX 	       ;(screen half x)
    // asm 000001B7: 	MPYF	*+AR2(IR1),R7,R0	;y = (y * inverse [z])
    // asm 000001B7:  ||	STF	R0,*-AR3(1)
    // asm 000001B8: 	MPYF	1.04,R0			; below 1.0 shrinks Y, above expands Y
    // asm 000001B9: 	ADDF	@SCRNHYI,R0		;    + SCRNHY	       ;(screen half y)
EOVCTR2:
    ;
    // asm 000001BA: STF	R0,*AR3++(2)
    // asm 000001BB: 	BU	POLYPOLY_ENTER

}

    // *DYNAMIC OBJECT
static void DYNAMIC_OBJECT(OBJ* obj, const u32* rom_ptr, const MATRIX* object_camera_matrix) {
    DYNAOBJ* dyna;
    MATRIX dyna_matrix;
    MATRIX combined_matrix;
    const ROM_VERTEX* vertices;
    const ROM_POLYGON* polygons;
    int counts_word;
    int total_vertex_count;
    int vertex_cursor;
    int part_vertex_count;
    c3x_reg_t part_trans_x;
    c3x_reg_t part_trans_y;
    c3x_reg_t part_trans_z;

    // ;	LDI	*+AR0(OFLAGS),R0
    // ;	TSTB	O_DEGRADE,R0
    // ;	BZ	NOTDEGRADEPOSS
    // asm 000001BC: 	LDI	*+AR0(ODIST),R0		;FORGET CLOSE DYNAMICS
    // asm 000001BD: 	CMPI	250,R0
    // asm 000001BE: 	BLTD	DISPLAY_NEXT
    if (obj->dist < 250) {
        return;
    }
    // asm 000001BF: 	LDI	*+AR0(ODEGRADE_ROM),R0
    // asm 000001C0: 	INC	R0
    // asm 000001C1: 	CMPI	AR1,R0
    // asm 000001C2: 	BZD	VECTORTRANSFULL
    // asm 000001C3: 	LDI	*+AR0(ODEGRADE_ROM2),R0
    // asm 000001C4: 	INC	R0
    // asm 000001C5: 	CMPI	AR1,R0
    // 	;---->BZD	VECTORTRANSFULL
    // asm 000001C6: 	BEQ	VECTORTRANSFULL
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
    // asm 000001C7: 	PUSH	AR0
    // asm 000001C8: 	LDI	*+AR0(ODYNALIST),AR0
    dyna = obj->dynalist;
    // asm 000001C9: 	LDI	@BLOWLISTI,AR3		;blowlist pointer = AR3
    // ;	LDI	@transvectorYI,AR6	;dst 1x3
    // asm 000001CA: 	LDI	@VECTORAYI,AR7
    // asm 000001CB: 	LDI	*AR1++,BK		;VERTEX CNT & POLYGON CNT
    counts_word = (int)rom_ptr[1];
    total_vertex_count = (counts_word & 0xff) + 1;
    vertices = (const ROM_VERTEX*)&rom_ptr[2];
    polygons = (const ROM_POLYGON*)(vertices + total_vertex_count);
    vertex_cursor = 0;
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
    // asm 000001CC: 	FLOAT	SCREENHX,R6		;screen half x
    // asm 000001CD: 	FLOAT	SCREENHY,R7		;screen half x
DYNALOOP:
    if (dyna == NULL) {
        PLOTPOLY(obj, polygons, (counts_word >> 16) & 0xffff);
        return;
    }
    // asm 000001CE: 	LDI	*+AR0(DYNAFLAG),R0
    // asm 000001CF: 	BND	DYNASHD			;GO DO A SHADOW...
    if (dyna->flag < 0) {
        goto DYNASHD;
    }
DYNREG:
    // asm 000001D0: 	LDI	@tmpmatI,AR4		;trans = object_pos + univ_pos
    // asm 000001D1: 	LDI	@transmatrixI,AR5	;somewhat dedicated for matrix pointer
    // asm 000001D2: 	LDF	*+AR0(DYNATRANSX),R2
    part_trans_x = C3X_LDF(dyna->trans_x);
    // 	;------>BND	DYNASHD		;GO DO A SHADOW...
    // asm 000001D3: 	LDF	*+AR0(DYNATRANSY),R3
    part_trans_y = C3X_LDF(dyna->trans_y);
    // asm 000001D4: 	LDF	*+AR0(DYNATRANSZ),R4
    part_trans_z = C3X_LDF(dyna->trans_z);
    // *
    // *ROTATE TRANSLATION VECTOR BY OBJ-UNIV MATRIX AND ADD TO OBJECT TRANSLATION
    // *
    // asm 000001D5: 	MPYF	*AR5++,R2,R0
    // asm 000001D5:   ||	STF	R2,*AR4			;SAVE X VECTOR VALUE (TMPMAT.x)
    // asm 000001D6: 	MPYF	*AR5++,R3,R1
    // asm 000001D6:   ||	STF	R4,*+AR4(1)		;SAVE Z VECTOR VALUE (TMPMAT.z)
    // asm 000001D7: 	MPYF	*AR5++,*+AR4(1),R1
    // asm 000001D7:   ||	ADDF	R0,R1,R2
    // asm 000001D8: 	MPYF	*AR5++,*AR4,R0
    // asm 000001D8:   ||	ADDF	R1,R2,R2
    // asm 000001D9: 	MPYF	*AR5++,R3,R1
    // asm 000001D9:   ||	ADDF	*-AR6(1),R2
    // asm 000001DA:   	STF	R2,*-AR7(1)		;(TRANSVECTOR.x)
    // asm 000001DB: 	MPYF	*AR5++,*+AR4(1),R1
    // asm 000001DB:   ||	ADDF	R0,R1,R2
    // asm 000001DC: 	MPYF	*AR5++,*AR4,R0
    // asm 000001DC:   ||	ADDF	R1,R2,R2
    // asm 000001DD: 	MPYF	*AR5++,R3,R1
    // asm 000001DD:   ||	ADDF	*AR6,R2
    // asm 000001DE: 	ADDF	R0,*+AR6(1),R2
    // asm 000001DE:   ||	STF	R2,*AR7			;(TRANSVECTOR.y)
    // asm 000001DF: 	MPYF	*AR5++,*+AR4(1),R1
    // asm 000001DF:   ||	ADDF	R1,R2,R2
    // asm 000001E0: 	ADDF	R1,R2
    // asm 000001E1: 	STF	R2,*+AR7(1)		;(TRANSVECTOR.z) R2 = Z value of object
    part_trans_x = C3X_ADD(
        C3X_ADD(
            C3X_ADD(
                C3X_MUL(object_camera_matrix->a00, C3X_LDF(dyna->trans_x)),
                C3X_MUL(object_camera_matrix->a01, C3X_LDF(dyna->trans_y))),
            C3X_MUL(object_camera_matrix->a02, C3X_LDF(dyna->trans_z))),
        TRANSVECTOR.X);
    part_trans_y = C3X_ADD(
        C3X_ADD(
            C3X_ADD(
                C3X_MUL(object_camera_matrix->a10, C3X_LDF(dyna->trans_x)),
                C3X_MUL(object_camera_matrix->a11, C3X_LDF(dyna->trans_y))),
            C3X_MUL(object_camera_matrix->a12, C3X_LDF(dyna->trans_z))),
        TRANSVECTOR.Y);
    part_trans_z = C3X_ADD(
        C3X_ADD(
            C3X_ADD(
                C3X_MUL(object_camera_matrix->a20, C3X_LDF(dyna->trans_x)),
                C3X_MUL(object_camera_matrix->a21, C3X_LDF(dyna->trans_y))),
            C3X_MUL(object_camera_matrix->a22, C3X_LDF(dyna->trans_z))),
        TRANSVECTOR.Z);
    // asm 000001E2: 	SUBI	9,AR5	       		;RESTORE AR5 OBJ-UNIV MAT
    // *CONCAT UNIV*OBJ MATRIX WITH DYNAMIC MATRIX
    // asm 000001E3: 	LDI	@MATRIXAI,AR2
    // asm 000001E4: 	LDI	AR0,AR4
    // asm 000001E5: 	ADDI	DYNAMATRIX,AR4
    dyna_matrix.a00 = C3X_STF(C3X_LDF(dyna->omatrix.mat00));
    dyna_matrix.a01 = C3X_STF(C3X_LDF(dyna->omatrix.mat10));
    dyna_matrix.a02 = C3X_STF(C3X_LDF(dyna->omatrix.mat20));
    dyna_matrix.a10 = C3X_STF(C3X_LDF(dyna->omatrix.mat01));
    dyna_matrix.a11 = C3X_STF(C3X_LDF(dyna->omatrix.mat11));
    dyna_matrix.a12 = C3X_STF(C3X_LDF(dyna->omatrix.mat21));
    dyna_matrix.a20 = C3X_STF(C3X_LDF(dyna->omatrix.mat02));
    dyna_matrix.a21 = C3X_STF(C3X_LDF(dyna->omatrix.mat12));
    dyna_matrix.a22 = C3X_STF(C3X_LDF(dyna->omatrix.mat22));
    // *	AR4	- src1		(usually the objects matrix)
    // *	AR5	- src2		(usually the Cameras matrix)
    // *	AR2	- dst		(the GRM - general rotational matrix)
    // *
    // *A B C	   J K L    AJ+DK+GL BJ+EK+HL CJ+FK+IL
    // *D E F	*  M N O  = AM+DN+GO BM+EN+HO CM+FN+IO
    // *G H I     P Q R    AP+DQ+GR BP+EQ+HR CP+FQ+IR
    // asm 000001E6: 	LDI	5,IR1
    // asm 000001E7: 	LDI	3,IR0
    // asm 000001E8: 	LDI	2,RC				;LOOP THREE TIMES
    // asm 000001E9: 	RPTB	DYNLP1
    // asm 000001EA: 	MPYF3	*AR5++(1),*AR4++(IR0),R0
    // asm 000001EB: 	MPYF3	*AR5,*AR4++(IR0),R1
    // asm 000001EC: 	MPYF3	*+AR5(1),*AR4--(IR1),R1
    // asm 000001EC:  ||	ADDF3	R0,R1,R2
    // asm 000001ED: 	MPYF3	*-AR5(1),*AR4++(IR0),R0
    // asm 000001ED:  ||	ADDF3	R1,R2,R2
    // asm 000001EE: 	MPYF3	*AR5,*AR4++(IR0),R1
    // asm 000001EF: 	STF	R2,*AR2++(1)			;store MATij
    // asm 000001F0: 	MPYF3	*+AR5(1),*AR4--(IR1),R1
    // asm 000001F0:  ||	ADDF3	R0,R1,R2
    // asm 000001F1: 	MPYF3	*-AR5(1),*AR4++(IR0),R0
    // asm 000001F1:  ||	ADDF3	R1,R2,R2
    // asm 000001F2: 	MPYF3	*AR5++,*AR4++(IR0),R1
    // asm 000001F3: 	STF	R2,*AR2++(1)			;store MATij
    // asm 000001F4: 	MPYF3	*AR5++,*AR4--(IR1),R1
    // asm 000001F4:  ||	ADDF3	R0,R1,R2
    // asm 000001F5: 	ADDF	R1,R2
    // asm 000001F6: 	STF	R2,*AR2++(1)			;store MATij
DYNLP1:
    // asm 000001F7: NOP	*--AR4(3)
    CONCATMAT(&dyna_matrix, (MATRIX*)object_camera_matrix, &combined_matrix);
    // asm 000001F8: 	LDI	@tmpmatY,AR4		;TEMP VECTOR STORE
    // asm 000001F9: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm 000001FA: 	LDI	8,IR0
    // asm 000001FB: 	LDI	@MATRIXAI,AR5
    // asm 000001FC: 	FLOAT	SCREENHX,R6		;screen half x
    // asm 000001FD: 	FLOAT	SCREENHY,R7		;screen half x
    // asm 000001FE: 	BD	DYNALP
    // asm 000001FF: 	LDF	*-AR7(1),R4		;GET X TRANS
    // asm 00000200: 	LDF	*AR7,R5			;GET Y TRANS
    // asm 00000201: 	LDI	*+AR0(DYNANVERTS),RC	;number of vertices to process -1
    part_vertex_count = (int)dyna->nverts + 1;
    // asm 00000202: 	BU	DYNALP			; jeff edited to match I450 rom
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
    // asm 00000220: 	RPTB	EODVCTR
    for (int part_vertex = 0; part_vertex < part_vertex_count; ++part_vertex) {
        int vertex_index = vertex_cursor + part_vertex;
        int packed_xy = (int)vertices[vertex_index].x_y;
        c3x_reg_t x = C3X_SUB(
            C3X_FROM_INT((int16_t)(packed_xy & 0xffff)),
            C3X_LDF(dyna->center_x));
        c3x_reg_t y = C3X_SUB(
            C3X_FROM_INT((int16_t)((u32)packed_xy >> 16)),
            C3X_LDF(dyna->center_y));
        c3x_reg_t z = C3X_SUB(
            C3X_FROM_INT(vertices[vertex_index].z),
            C3X_LDF(dyna->center_z));
        c3x_reg_t rotated_x = C3X_ADD(
            C3X_ADD(C3X_MUL(combined_matrix.a00, x), C3X_MUL(combined_matrix.a01, y)),
            C3X_MUL(combined_matrix.a02, z));
        c3x_reg_t rotated_y = C3X_ADD(
            C3X_ADD(C3X_MUL(combined_matrix.a10, x), C3X_MUL(combined_matrix.a11, y)),
            C3X_MUL(combined_matrix.a12, z));
        c3x_reg_t rotated_z = C3X_ADD(
            C3X_ADD(C3X_MUL(combined_matrix.a20, x), C3X_MUL(combined_matrix.a21, y)),
            C3X_MUL(combined_matrix.a22, z));
        c3x_reg_t world_x = C3X_ADD(rotated_x, part_trans_x);
        c3x_reg_t world_y = C3X_ADD(rotated_y, part_trans_y);
        c3x_reg_t world_z = C3X_ADD(rotated_z, part_trans_z);
        int inverse_index = FIX(world_z) >> 4;
        int blow_index = vertex_index * 3;

        if (inverse_index > HIGH_CLIP_LEVEL) {
            inverse_index = HIGH_CLIP_LEVEL;
        }
        if (inverse_index < -80) {
            inverse_index = -80;
        }

        c3x_reg_t inverse_z = C3X_LDF(INVTAB[inverse_index]);
        BLOWLIST[blow_index] = C3X_STF(C3X_ADD(C3X_MUL(world_x, inverse_z), SCRNHXI));
        BLOWLIST[blow_index + 1] =
            C3X_STF(C3X_ADD(C3X_MUL(C3X_MUL(world_y, inverse_z), C3X_IMM_F32(1.04)), SCRNHYI));
        BLOWLIST[blow_index + 2] = C3X_STF(world_z);
    }
    // asm 00000221: 	LDI	*AR1++,R3
    // asm 00000222: 	LDI	R3,R2
    // asm 00000223: 	ASH	-16,R2
    // asm 00000224: 	FLOAT	R2		;y element
    // asm 00000225: 	LS	16,R3
    // asm 00000226: 	ASH	-16,R3
    // asm 00000227: 	FLOAT	R3		;x element
    // asm 00000228: 	SUBF	*+AR0(DYNACENTERX),R3
    // asm 00000229: 	STF	R3,*-AR4(1)		;store secondary access A
    // asm 0000022A: 	SUBF	*+AR0(DYNACENTERY),R2
    // ;	FLOAT	*AR1++,R3		;get x element of source 1
    // ;	SUBF	*+AR0(DYNACENTERX),R3
    // ;
    // ;	FLOAT	*AR1++,R2		;B	get y element of source 1
    // ;||	STF	R3,*-AR4(1)		;store secondary access A
    // ;	SUBF	*+AR0(DYNACENTERY),R2
    // asm 0000022B: 	FLOAT	*AR1++,R0		;C	get z element of source 1
    // asm 0000022B:  ||	STF	R2,*AR4			;store secondary access B
    // asm 0000022C: 	SUBF	*+AR0(DYNACENTERZ),R0
    // 	;
    // 	;MULTIPLY BY ROTATION MATRIX
    // 	;
    // asm 0000022D: 	MPYF3	*AR5++,R3,R0		;AD
    // asm 0000022D:  ||	STF	R0,*+AR4(1)  		;store secondary access C
    // asm 0000022E: 	MPYF3	*AR5++,R2,R1		;BE
    // asm 0000022F: 	MPYF3	*AR5++,*+AR4(1),R1	;CF
    // asm 0000022F:  ||	ADDF3	R0,R1,R3		;AD+BE
    // asm 00000230: 	MPYF3	*AR5++,*-AR4(1),R0	;AG
    // asm 00000230:  ||	ADDF3	R1,R3,R3		;AD+BE+CF
    // asm 00000231: 	MPYF3	*AR5++,R2,R1		;BH
    // asm 00000231:  ||	STF	R3,*AR3++ 		;*BLOWLIST++ = [AD+BE+CF];STORE ROTATED X
    // asm 00000232: 	MPYF3	*AR5++,*+AR4(1),R1	;CI
    // asm 00000232:  ||	ADDF3	R0,R1,R3		;AG+BH
    // asm 00000233: 	MPYF3	*AR5++,*-AR4(1),R0	;AJ
    // asm 00000233:  ||	ADDF3	R1,R3,R3		;AG+BH+CI
    // asm 00000234: 	MPYF3	*AR5++,*AR4,R1		;BK
    // asm 00000235: 	MPYF3	*AR5--(IR0),*+AR4(1),R1	;CL
    // asm 00000235:  ||	ADDF3	R0,R1,R2		;AJ+BK
    // asm 00000236: 	ADDF	R1,R2			;AJ+BK+CL
    // asm 00000237: 	ADDF	*+AR7(1),R2		;add in translation into [AJ+BK+CL]
    // ;	LDFLT	0,R2			;it will always be positive
    // asm 00000238: 	LDF	*-AR3(1),R1		;GET BACK NEW X
    // asm 00000238:  ||	STF	R2,*+AR3(1)		;SAVE NEW Z
    // asm 00000239: 	FIX	R2,IR1			;find z distance for inverse lookup
    // asm 0000023A: 	ASH	-4,IR1			;quickly divide by 16
    // asm 0000023B: 	CMPI	HIGH_CLIP_LEVEL,IR1   	;compare against highest clip level
    // asm 0000023C: 	LDIGE	HIGH_CLIP_LEVEL,IR1    	;max it at highest clip level
    // asm 0000023D: 	CMPI	-80,IR1
    // asm 0000023E: 	LDILT	-80,IR1
    // asm 0000023F: 	ADDF	R4,R1			;ADD X TRANSLATION
    // asm 00000240: 	ADDF	R5,R3			;add Y translation
    // asm 00000241: 	MPYF	*+AR2(IR1),R1,R0    	;x = (x * inverse [z])
    // asm 00000242: 	ADDF	R6,R0			;    + SCRNHX 	       ;(screen half x)
    // asm 00000243: 	MPYF	*+AR2(IR1),R3,R0	;y = (y * inverse [z])
    // asm 00000243:  ||	STF	R0,*-AR3(1)
    // asm 00000244: 	MPYF	1.04,R0			; below 1.0 shrinks Y, above expands Y
    // asm 00000245: 	ADDF	R7,R0			;    + SCRNHY	       ;(screen half y)
EODVCTR:
    // asm 00000246: STF	R0,*AR3++(2)
DYNALPX:
    // asm 00000247: 	LDI	*AR0,AR0
    vertex_cursor += part_vertex_count;
    dyna = dyna->link;
    // asm 00000248: 	LDI	AR0,R0
    // asm 00000249: 	BNZ	DYNALOOP
    goto DYNALOOP;
    // ;	POP	BK
    // asm 0000024A: 	POP	AR0
    // asm 0000024B: 	RS	16,BK
    // asm 0000024C: 	CALL	PLOTPOLY
    // asm 0000024D: 	LDI	*AR0,R0
    // asm 0000024E: 	BNZ	NEXTOBJ
    // asm 0000024F: 	RETS
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
    // asm 00000250: 	LDI	@CAMERAMATRIXI,AR5	;CAMERA ROTATION MATRIX
    // asm 00000251: 	LDI	@INVTABI,R5		;inverse table dedicated ptr
    // *FIND CAR STRUCT IN PROCESS
    // asm 00000252: 	LDI	*+AR0(DYNAPARENT),AR4	;GET POINTER TO PARENT
    // asm 00000253: 	LDI	*+AR4(OCARBLK),AR4
    // asm 00000254: 	ADDI	CARVSIZ,AR4
    if (dyna->parent == NULL || dyna->parent->carblk == NULL) {
        goto NOSHAD;
    }
    // *CHECK SHADOW TYPE
    // asm 00000255: 	LDI	*+AR4(CARSHAD-CARVSIZ),R0	;SHADOW ACTIVE
    // asm 00000256: 	BZ	NOSHAD				;NO...BLOW IT OUT
    if (dyna->parent->carblk->shadow_flag == 0) {
        goto NOSHAD;
    }
    // *IF NOT AIRBORNE DO REGULAR
    // asm 00000257: 	LDI	*+AR4(CAR_AIRF-CARVSIZ),R0
    // asm 00000258: 	OR	*+AR4(CAR_AIRB-CARVSIZ),R0
    // asm 00000259: 	BZ	DYNREG
    if (dyna->parent->carblk->front_airborne == 0
        && dyna->parent->carblk->rear_airborne == 0) {
        goto DYNREG;
    }
    // asm 0000025A: 	LDI	@_MODE,R4	       	;NO FLYING SHADOWS IN TUNNEL
    // asm 0000025B: 	TSTB	MINTUNNEL,R4
    // asm 0000025C: 	BNZ	DYNREG
    if ((_MODE & MINTUNNEL) != 0) {
        goto DYNREG;
    }
    // asm 0000025D: 	LDI	8,IR0
    // ;	LDI	*+AR0(DYNANVERTS),RC	;number of vertices to process -1
    // ;	LDI	RC,R0
    // ;	ADDI	1,R0
    // ;	MPYI	2,R0
    // ;	ADDI	R0,AR1			;ADJUST AR1 TO SKIP VERTICES
    // asm 0000025E: 	LDI	3,RC
    // asm 0000025F: 	ADDI	8,AR1			;SKIP 4 SHADOW VERTICES
    // asm 00000260: 	RPTB	EOSVCTR
    for (int shadow_vertex = 0; shadow_vertex < 4; ++shadow_vertex) {
        CAR_POINT* point = &dyna->parent->carblk->right_front + shadow_vertex;
        c3x_reg_t relative_x = C3X_SUB(point->x, _CAMERAPOS.X);
        c3x_reg_t relative_y =
            C3X_ADD(C3X_SUB(point->y, _CAMERAPOS.Y), C3X_LDF(point->road_delta_y));
        c3x_reg_t relative_z = C3X_SUB(point->z, _CAMERAPOS.Z);
        c3x_reg_t rotated_x = C3X_ADD(
            C3X_ADD(
                C3X_MUL(_CAMERAMATRIX.a00, relative_x),
                C3X_MUL(_CAMERAMATRIX.a01, relative_y)),
            C3X_MUL(_CAMERAMATRIX.a02, relative_z));
        c3x_reg_t rotated_y = C3X_ADD(
            C3X_ADD(
                C3X_MUL(_CAMERAMATRIX.a10, relative_x),
                C3X_MUL(_CAMERAMATRIX.a11, relative_y)),
            C3X_MUL(_CAMERAMATRIX.a12, relative_z));
        c3x_reg_t rotated_z = C3X_ADD(
            C3X_ADD(
                C3X_MUL(_CAMERAMATRIX.a20, relative_x),
                C3X_MUL(_CAMERAMATRIX.a21, relative_y)),
            C3X_MUL(_CAMERAMATRIX.a22, relative_z));
        int inverse_index = FIX(rotated_z) >> 4;
        int blow_index = (vertex_cursor + shadow_vertex) * 3;

        if (inverse_index > HIGH_CLIP_LEVEL) {
            inverse_index = HIGH_CLIP_LEVEL;
        }
        if (inverse_index < -80) {
            inverse_index = -80;
        }

        c3x_reg_t inverse_z = C3X_LDF(INVTAB[inverse_index]);
        BLOWLIST[blow_index] = C3X_STF(C3X_ADD(C3X_MUL(rotated_x, inverse_z), SCRNHXI));
        BLOWLIST[blow_index + 1] =
            C3X_STF(C3X_ADD(C3X_MUL(C3X_MUL(rotated_y, inverse_z), C3X_IMM_F32(1.04)), SCRNHYI));
        BLOWLIST[blow_index + 2] = C3X_STF(rotated_z);
    }
    // *GET COORDS IN ORDER, ADD IN ROAD DIFF
    // asm 00000261: 	LDI	@CAMERAPOSI,AR2
    // asm 00000262: 	LDF	*AR4++,R3		;GET X COORD OF CAR POINT
    // asm 00000263: 	SUBF	*AR2,R3			;GET RELATIVE TO CAMERA
    // asm 00000264: 	LDF	*AR4++,R4		;GET Y COORD OF CAR POINT
    // asm 00000264:  ||	STF	R3,*-AR7(1)		;SAVE X
    // asm 00000265: 	SUBF	*+AR2(1),R4		;GET RELATIVE TO CAMERA
    // asm 00000266: 	ADDF	*+AR4(1),R4		;ADD IN ROAD DIFFERENTIAL TO Y
    // asm 00000267: 	LDF	*AR4++,R2		;GET Z COORD OF CAR POINT
    // asm 00000267:  ||	STF	R4,*AR7			;STORE Y
    // asm 00000268: 	SUBF	*+AR2(2),R2		;GET RELATIVE TO CAMERA
    // *RETURN TO DYNAMIC OBJECT LOOP WITH REG INTACT
    // *MULTIPLY BY CAMERA ROTATION MATRIX
    // asm 00000269: 	MPYF3	*AR5++,R3,R0		;AD
    // asm 00000269:  ||	STF	R2,*+AR7(1)  		;STORE Z
    // asm 0000026A: 	MPYF3	*AR5++,R4,R1		;BE
    // asm 0000026B: 	MPYF3	*AR5++,*+AR7(1),R1	;CF
    // asm 0000026B:  ||	ADDF3	R0,R1,R2		;AD+BE
    // asm 0000026C: 	MPYF3	*AR5++,*-AR7(1),R0	;AG
    // asm 0000026C:  ||	ADDF3	R1,R2,R2		;AD+BE+CF
    // ;	ADDF	*-AR6(1),R2		;add translation into [AD+BE+CF]
    // asm 0000026D: 	MPYF3	*AR5++,R4,R1		;BH
    // asm 0000026D:  ||	STF	R2,*AR3++ 		;*BLOWLIST++ = [AD+BE+CF];STORE ROTATED X
    // asm 0000026E: 	MPYF3	*AR5++,*+AR7(1),R1	;CI
    // asm 0000026E:  ||	ADDF3	R0,R1,R2		;AG+BH
    // asm 0000026F: 	MPYF3	*AR5++,*-AR7(1),R0	;AJ
    // asm 0000026F:  ||	ADDF3	R1,R2,R3		;AG+BH+CI
    // ;	ADDF	*AR6,R3			;add translation into [AG+BH+CI]
    // asm 00000270: 	MPYF3	*AR5++,R4,R1		;BK
    // asm 00000270:  ||	STF	R3,*AR3++		;*BLOWLIST++ = [AG+BH+CI]
    // asm 00000271: 	MPYF3	*AR5--(IR0),*+AR7(1),R1	;CL
    // asm 00000271:  ||	ADDF3	R0,R1,R2		;AJ+BK
    // asm 00000272: 	ADDF	R1,R2			;AJ+BK+CL
    // ;	ADDF	*+AR6(1),R2		;add in translation into [AJ+BK+CL]
    // asm 00000273: 	STF	R2,*AR3--(2)		;(TRANSVECTOR.z) R2 = Z value of object
    // asm 00000274: 	FIX	R2,R0			;find z distance for inverse lookup
    // ;	LDILT	0,R0			;it will always be positive
    // asm 00000275: 	ADDI	CARVSIZ-3,AR4		;ADDRESS NEXT WHEEL ENTRY
    // asm 00000276: 	ASH	-4,R0			;quickly divide by 16
    // asm 00000277: 	CMPI	HIGH_CLIP_LEVEL,R0   	;compare against highest clip level
    // asm 00000278: 	LDIGE	HIGH_CLIP_LEVEL,R0    	;max it at highest clip level
    // asm 00000279: 	CMPI	-80,R0
    // asm 0000027A: 	LDILT	-80,R0
    // asm 0000027B: 	ADDI	R5,R0,AR2		;add in start of inverse table
    // asm 0000027C: 	MPYF	*AR2,*AR3,R0		;x = (x * inverse [z])
    // asm 0000027D: 	ADDF	R6,R0			;    + SCRNHX 	       ;(screen half x)
    // asm 0000027E: 	STF	R0,*AR3++
    // asm 0000027F: 	MPYF	*AR2,*AR3,R0		;y = (y * inverse [z])
    // asm 00000280: 	MPYF	1.04,R0	; below 1.0 shrinks Y, above expands Y
    // asm 00000281: 	ADDF	R7,R0			;    + SCRNHY	       ;(screen half y)
EOSVCTR:
    // asm 00000282: STF	R0,*AR3++(2)
    // asm 00000283: 	LDI	@transmatrixI,AR5	;RESTORE MATRIX POINTER
    // asm 00000284: 	BU	DYNALPX
    part_vertex_count = 4;
    goto DYNALPX;
    // *NO SHADOW KLUDGE
NOSHAD:
    // asm 00000285: 	LDF	0,R0
    // asm 00000286: 	FLOAT	-1000,R0
    // asm 00000287: 	LDI	3,RC
    // asm 00000288: 	ADDI	8,AR1			;SKIP 4 SHADOW VERTICES
    // asm 00000289: 	RPTS	11
    // asm 0000028A: 	STF	R0,*AR3++    		;STORE NULL X,Y,Z
    for (int shadow_vertex = 0; shadow_vertex < 4; ++shadow_vertex) {
        int blow_index = (vertex_cursor + shadow_vertex) * 3;
        BLOWLIST[blow_index] = C3X_STF(C3X_FROM_INT(-1000));
        BLOWLIST[blow_index + 1] = C3X_STF(C3X_FROM_INT(-1000));
        BLOWLIST[blow_index + 2] = C3X_STF(C3X_FROM_INT(-1000));
    }
    // asm 0000028B: 	LDI	@transmatrixI,AR5	;RESTORE MATRIX POINTER
    // asm 0000028C: 	BU	DYNALPX
    part_vertex_count = 4;
    goto DYNALPX;

    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DYNAMIC_OBJECT", 0, 0);
}

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
static void PLOTPOLY(OBJ* obj /*AR0*/, const ROM_POLYGON* polygons /*AR1*/, int polygon_count_minus_one /*BK*/) {
    int* clipram;
    const ROM_POLYGON* polygon;
    int polygon_index;

#if STATISTICS
    // asm: 	LDI	BK,R0			;# of polygons-1
    // asm: 	ADDI	1,R0
    // asm: 	ADDI	@ST_POLYGONS,R0
    // asm: 	STI	R0,@ST_POLYGONS
#endif
    // asm 0000028D: 	LDI	*+AR0(OFLAGS),R6	;test for illuminated object
    // asm 0000028E: 	TSTB	O_1PAL,R6
    // asm 0000028F: 	BNZD	PLOT1PAL		;BR-> if it is a one palette object
    // asm 00000290: 	LDI	*+AR0(ODIST),R0		;CHECK IF DISTANT
    // asm 00000291: 	SUBI	*+AR0(ORAD),R0		;FAR AWAY
    // asm 00000292: 	LDI	BK,RC			;# of polygons-1
    // 	;---->BZD	PLOT1PAL
    // asm 00000293: 	TSTB	O_ILLUM,R6
    // asm 00000294: 	BNZD	PLOTILLUM		;BR-> if it is a one palette object
    // asm 00000295: 	LDI	@BLOWLISTI,IR0
    // asm 00000296: 	LDI	IR0,IR1
    // asm 00000297: 	ADDI	1,IR1
    // 	;---->BNZD	PLOTILLUM	;BR-> if it is a one palette object
PLOTPOLY0:
    // asm 00000298: 	CMPI	1000,R0
    // asm 00000299: 	BGTD	PLTPOLY			;YES, NO CLIP LOOP
    // asm 0000029A: 	LDI	@_PALLISTI,BK
    // asm 0000029B: 	LDI	0FFH,R7			;GET MASK
    // asm 0000029C: 	LDI	-16,R6			;SHIFT COUNT
    // 	;---->BGTD	PLTPOLY		;YES, NO CLIP LOOP
    // asm 0000029D: 	PUSH 	AR0
    // asm 0000029E: 	LDI	@CLIPRAMI,AR0
    // asm 0000029F:  	LDI	RC,AR6			;GET POLY COUNT
    // asm 000002A0: 	BUD	PLOTPOLYLP
    // asm 000002A1: 	LDP	@FIFO_STATUS
    // asm 000002A2: 	LDI	FIFO_ADDR>>16,AR7
    // asm 000002A3: 	LSH	16,AR7
    // asm 000002A4: 	BU	PLTPOLYLP		; jeff edited to match I450 rom
PLOTPOLYLP:
    // *GET EXTERNAL VERTEX INDICIES
    // asm 000002C0: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 000002C1: 	AND	R7,R3,AR4
PLOTPOLYLP1:
    // asm 000002C2: 	ADDI	1,IR1
    // asm 000002C3: 	MPYI	3,AR4			;V1
    // asm 000002C4: 	LSH	-8,R3
    // asm 000002C5: 	AND	R7,R3,AR5
    // asm 000002C6: 	MPYI	3,AR5			;V2
    // asm 000002C7: 	LSH	-8,R3
    // asm 000002C8: 	AND	R7,R3,AR2
    // asm 000002C9: 	MPYI	3,AR2			;V3
    // asm 000002CA: 	LSH	-8,R3
    // asm 000002CB: 	AND	R7,R3,AR3
    // asm 000002CC: 	MPYI	3,AR3			;V4
    // *CHECK ALL Z'S <=0
    // asm 000002CD: 	LDF	*+AR4(IR1),R0
    // asm 000002CE: 	BGED	INBNDS
    // asm 000002CF: 	AND	*+AR5(IR1),*+AR2(IR1),R0
    // asm 000002D0: 	AND	*+AR3(IR1),R0
    // asm 000002D1: 	SUBI	1,IR1
    // 	;------->BGED	INBNDS
    // asm 000002D2: 	LSH	8,R0
    // asm 000002D3: 	BND	POLYLP
    // asm 000002D4: 	NOP
    // *CHECK FIFO FULL
INBNDS:
    // asm 000002D5: 	LDI	@FIFO_STATUS,R0
    // asm 000002D6: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // 	;---->BND	POLYLP_1
    // asm 000002D7: 	BNZD	INBNDS
    // *CHECK HIDDEN SURFACE REMOVAL
    // asm 000002D8: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm 000002D9: 	SUBF	*+AR4(IR1),*+AR5(IR1),R3	;dy = ay - by
    // asm 000002DA: 	SUBF	*+AR5(IR0),*+AR2(IR0),R0	;ex = cx - bx
    // 	;------->BNZD	INBNDS
    // asm 000002DB: 	MPYF	R3,R0				;ex = dy * ex
    // asm 000002DB:   ||	SUBF	*+AR5(IR1),*+AR2(IR1),R2	;ey = cy - by
    // asm 000002DC: 	MPYF	R2,R1				;ey = dx * ey
    // asm 000002DD: 	SUBF	R1,R0				;ey = ey - ex
    // asm 000002DE: 	BGTD	POLYLP				;if back facing DONT PLOT
    // *GLITCH FIX
    // asm 000002DF: 	SUBF	*+AR2(IR1),*+AR3(IR1),R0
    // asm 000002E0: 	MPYF	R3,R0
    // asm 000002E0: ||	SUBF	*+AR3(IR1),*+AR4(IR1),R3
    // asm 000002E1: 	CMPI	AR2,AR3
    // 	;------>BGTD	POLYLP			;if back facing DONT PLOT
    // asm 000002E2: 	BZD	LOF1X
    // asm 000002E3: 	MPYF	R3,R2
    // asm 000002E4: 	OR	R2,R0
    // asm 000002E5: 	LDF	R0,R0
    // asm 000002E6: 	BGT	POLYLP
LOF1X:
    // *AR4,AR5,AR2,AR3 = FOUR VERTICES
    // *IR1=PALETTE
    // *
    // asm 000002E7: 	CALL	CLIPCK
    // asm 000002E8: 	BNZD	CLIPIT			;GO CLIP IT DUDES
    // asm 000002E9: 	LDI	*AR1++(2),R2		;get control word/palette
    // asm 000002EA: 	LSH	R6,R2,R0		;SHIFT 16 TO RIGHT
    // asm 000002EB: 	ADDI	R0,BK,AR4
    // 	;------->BNZD	CLIPIT	  	;YES SPLIT IT UP...
    // asm 000002EC: 	LSH	R6,*AR4,R0		;PALETTE->R0
    // asm 000002EC:  ||	STI	R2,*AR7
    // asm 000002ED:  	LSH	8,R0			;not a good way to do this fix l8r -7/14/93
    // *NO CLIP, BLOW IT OUT
BLOWOUT:
    // asm 000002EE: 	RPTS	7
    // asm 000002EF: 	LDI	*AR0++,R0
    // asm 000002EF:  ||	STI	R0,*AR7
    // asm 000002F0: 	NOP	*AR0--(8)		;READJUST INDEX DUDES...
    // asm 000002F1: 	LDI	*AR1++,R0		;2 AIV packed format Y2:X2:Y1:X1
    // asm 000002F1:  ||	STI	R0,*AR7
    // asm 000002F2: 	STI	R0,*AR7
    // asm 000002F3: 	LSH	-16,R0
    // asm 000002F4: 	LDI	*AR1++,R0		;GET Y4:X4:Y3:X3
    // asm 000002F4:  ||	STI	R0,*AR7
    // asm 000002F5: 	STI	R0,*AR7
    // asm 000002F6: 	LSH	-16,R0
    // asm 000002F7: 	DBUD	AR6,PLOTPOLYLP
    // asm 000002F8: 	LDI	*AR1++,R0
    // asm 000002F8:  ||	STI	R0,*AR7
    // asm 000002F9: 	STI	R0,*AR7			;STORE THE ADDR
    // asm 000002FA: 	LDI	@FIFO_INC,R0
    // ;	LDI	*AR7,R0		 	;FIFO_INC
    // 	;----> DBUD AR6,PLOTPOLYLP
    // asm 000002FB: 	POP	AR0			;RESTORE OBJECT POINTER
    // asm 000002FC: 	RETS
POLYLP:
    // asm 000002FD: 	DBUD	AR6,PLOTPOLYLP1
    // asm 000002FE: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 000002FF: 	SUBI	1,AR1
    // asm 00000300: 	AND	R7,R3,AR4
    // 	;----> DBUD AR6,PLOTPOLYLP1
    // asm 00000301: 	POP	AR0
    // asm 00000302: 	RETS
    // *CLIP IT
CLIPIT:
    // asm 00000303: 	LSH	R6,*AR4,R0		;PALETTE->R0
    // asm 00000304:  	LSH	8,R0			;not a good way to do this fix l8r -7/14/93
    // asm 00000305: 	CALL 	CLIP
    // asm 00000306: 	DBU	AR6,PLOTPOLYLP
    // asm 00000307: 	POP	AR0
    // asm 00000308: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLOTPOLY", 0, 0);
    g_dirq_debug_plotpoly_calls += 1;

    if ((obj->flags & O_1PAL) != 0) {
        PLOT1PAL(obj, polygons, polygon_count_minus_one);
        return;
    }
    if ((obj->flags & O_ILLUM) != 0) {
        PLOTILLUM(obj, (const ROM_ILLUM_POLYGON*)polygons, polygon_count_minus_one);
        return;
    }

    clipram = (int*)CLIPRAM;
    polygon = polygons;
    for (polygon_index = 0; polygon_index <= polygon_count_minus_one; polygon_index++, polygon++) {
        int packed_vertices;
        int clip;
        int* clip_vertex;
        const c3x_f32_t* vertex1;
        const c3x_f32_t* vertex2;
        const c3x_f32_t* vertex3;
        const c3x_f32_t* vertex4;
        int v1;
        int v2;
        int v3;
        int v4;
        int base1;
        int base2;
        int base3;
        int base4;
        c3x_reg_t z1;
        c3x_reg_t z2;
        c3x_reg_t z3;
        c3x_reg_t z4;
        c3x_reg_t dx;
        c3x_reg_t dy;
        c3x_reg_t ex;
        c3x_reg_t ey;
        c3x_reg_t glitch_dx;
        c3x_reg_t glitch_dy;
        int control_word;
        int palette_index;
        int palette_base;

        packed_vertices = (int)polygon->vertices_4_3_2_1;

        // asm 000002C0: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
        // asm 000002C1: 	AND	R7,R3,AR4
        v1 = packed_vertices & 0xff;
        // asm 000002C2: 	ADDI	1,IR1
        // asm 000002C3: 	MPYI	3,AR4			;V1
        base1 = v1 * 3;
        // asm 000002C4: 	LSH	-8,R3
        // asm 000002C5: 	AND	R7,R3,AR5
        v2 = (packed_vertices >> 8) & 0xff;
        // asm 000002C6: 	MPYI	3,AR5			;V2
        base2 = v2 * 3;
        // asm 000002C7: 	LSH	-8,R3
        // asm 000002C8: 	AND	R7,R3,AR2
        v3 = (packed_vertices >> 16) & 0xff;
        // asm 000002C9: 	MPYI	3,AR2			;V3
        base3 = v3 * 3;
        // asm 000002CA: 	LSH	-8,R3
        // asm 000002CB: 	AND	R7,R3,AR3
        v4 = (packed_vertices >> 24) & 0xff;
        // asm 000002CC: 	MPYI	3,AR3			;V4
        base4 = v4 * 3;
        vertex1 = &BLOWLIST[base1];
        vertex2 = &BLOWLIST[base2];
        vertex3 = &BLOWLIST[base3];
        vertex4 = &BLOWLIST[base4];

        // *CHECK ALL Z'S <=0
        // asm 000002CD: 	LDF	*+AR4(IR1),R0
    z1 = C3X_LDF(vertex1[2]);
    z2 = C3X_LDF(vertex2[2]);
    z3 = C3X_LDF(vertex3[2]);
    z4 = C3X_LDF(vertex4[2]);
        if (C3X_LT(z1, C3X_FROM_INT(0)) && C3X_LT(z2, C3X_FROM_INT(0)) && C3X_LT(z3, C3X_FROM_INT(0)) && C3X_LT(z4, C3X_FROM_INT(0))) {
            goto DIRQ_POLYLP;
        }

        // *CHECK HIDDEN SURFACE REMOVAL
        // asm 000002D8: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
        // asm 000002D9: 	SUBF	*+AR4(IR1),*+AR5(IR1),R3	;dy = ay - by
        // asm 000002DA: 	SUBF	*+AR5(IR0),*+AR2(IR0),R0	;ex = cx - bx
        dx = C3X_SUB(vertex2[0], vertex1[0]);
        dy = C3X_SUB(vertex2[1], vertex1[1]);
        ex = C3X_SUB(vertex3[0], vertex2[0]);
        // asm 000002DB: 	MPYF	R3,R0				;ex = dy * ex
        // asm 000002DB:   ||	SUBF	*+AR5(IR1),*+AR2(IR1),R2	;ey = cy - by
        ey = C3X_SUB(vertex3[1], vertex2[1]);
        // asm 000002DC: 	MPYF	R2,R1				;ey = dx * ey
        // asm 000002DD: 	SUBF	R1,R0				;ey = ey - ex
        if (C3X_GT(C3X_SUB(C3X_MUL(dy, ex), C3X_MUL(dx, ey)), C3X_FROM_INT(0))) {
            goto DIRQ_POLYLP;
        }

        // *GLITCH FIX
        glitch_dx = C3X_SUB(vertex4[1], vertex3[1]);
        glitch_dy = C3X_SUB(vertex1[1], vertex4[1]);
        if (v3 != v4) {
            if ((FIX(C3X_MUL(dy, glitch_dx)) | FIX(C3X_MUL(glitch_dy, ey))) > 0) {
                goto DIRQ_POLYLP;
            }
        }

        // asm 000002E9: 	LDI	*AR1++(2),R2		;get control word/palette
        control_word = (int)polygon->palnum_and_cntl;
        // asm 000002EA: 	LSH	R6,R2,R0		;SHIFT 16 TO RIGHT
        clip = CLIPCK(vertex1, vertex2, vertex3, vertex4, clipram);
        if (clip != 0) {
            control_word = (int)polygon->palnum_and_cntl;
            palette_index = (int)((u32)control_word >> 16);
            palette_base = (_PALLIST[palette_index].ref_count_and_pal_code >> 16) << 8;
            CLIP(clipram, palette_base, control_word, polygon);
            continue;
        }
        palette_index = (int)((u32)control_word >> 16);
        // asm 000002EB: 	ADDI	R0,BK,AR4
        // asm 000002EC: 	LSH	R6,*AR4,R0		;PALETTE->R0
        palette_base = (_PALLIST[palette_index].ref_count_and_pal_code >> 16) << 8;
        // asm 000002EC:  ||	STI	R2,*AR7
        // asm 000002ED:  	LSH	8,R0			;not a good way to do this fix l8r -7/14/93

        clip_vertex = clipram;
        port_output_fpga(
            clip_vertex[0],
            clip_vertex[1],
            clip_vertex[2],
            clip_vertex[3],
            clip_vertex[4],
            clip_vertex[5],
            clip_vertex[6],
            clip_vertex[7],
            (int)(polygon->iv_0_1 & 0xffff),
            (int)((polygon->iv_0_1 >> 16) & 0xffff),
            (int)(polygon->iv_2_3 & 0xffff),
            (int)((polygon->iv_2_3 >> 16) & 0xffff),
            (int)polygon->texture_map_addr,
            palette_base,
            control_word);
    DIRQ_POLYLP:;
    }
}

// *CHECK THE CLIP AND DUMP VERTICES INTO INTERNAL RAM
// *
// *
// *RETURN
// *	R5 NZ=CLIP, Z=NOCLIP
// *
static int CLIPCK(const c3x_f32_t* vertex1 /*AR4*/, const c3x_f32_t* vertex2 /*AR5*/, const c3x_f32_t* vertex3 /*AR2*/, const c3x_f32_t* vertex4 /*AR3*/, int* clipram /*AR0*/) {
    int abs_or;
    int i;
    int xmax;
    int xmin;
    int ymax;
    int ymin;
    // 	;***	PRELIM CHECK
    // asm 00000309: 	FIX	*+AR4(IR0),R0		;read X value
    // asm 0000030A: 	FIX	*+AR4(IR1),R0
    // asm 0000030A:  ||	STI	R0,*AR0++
    // asm 0000030B: 	FIX	*+AR5(IR0),R0
    // asm 0000030B:  ||	STI	R0,*AR0++
    // asm 0000030C: 	FIX	*+AR5(IR1),R0
    // asm 0000030C:  ||	STI	R0,*AR0++
    // asm 0000030D: 	FIX	*+AR2(IR0),R0
    // asm 0000030D:  ||	STI	R0,*AR0++
    // asm 0000030E: 	FIX	*+AR2(IR1),R0
    // asm 0000030E:  ||	STI	R0,*AR0++
    // asm 0000030F: 	FIX	*+AR3(IR0),R0
    // asm 0000030F:  ||	STI	R0,*AR0++
    // asm 00000310: 	FIX	*+AR3(IR1),R0
    // asm 00000310:  ||	STI	R0,*AR0++
    // asm 00000311: 	STI	R0,*AR0
    // asm 00000312: 	ABSI	R0,R5
    // asm 00000313: 	LDI	6,RC
    // asm 00000314: 	RPTB	LPP2
    // asm 00000315: 	ABSI	*--AR0,R1
LPP2:
    // asm 00000316: OR	R1,R5
    // asm 00000317: 	LSH	-10,R5
    // asm 00000318: 	RETSZ
    // 	;***	STRICT CHECK
    // asm 00000319: 	LDI	0,R5
    // asm 0000031A: 	LDI	*AR0++,R0		;XMAX
    // asm 0000031A:  ||	LDI	*AR0,R1			;XMIN
    // asm 0000031B: 	LDI	*AR0,R2			;YMAX
    // asm 0000031B:  ||	LDI	*AR0,R3			;YMIN
    // asm 0000031C: 	LDI	2,RC
    // asm 0000031D: 	RPTB	CKLP
    // asm 0000031E: 	CMPI	*++AR0,R0
    // asm 0000031F: 	LDILT	*AR0,R0
    // asm 00000320: 	CMPI	*AR0,R1
    // asm 00000321: 	LDIGT	*AR0,R1
    // asm 00000322: 	CMPI	*++AR0,R2
    // asm 00000323: 	LDILT	*AR0,R2
    // asm 00000324: 	CMPI	*AR0,R3
CKLP:
    // asm 00000325: LDIGT	*AR0,R3
    // asm 00000326: 	NOP	*AR0--(7)
    // asm 00000327: 	SUBI	R1,R0 		;FIND XMAX-XMIN
    // asm 00000328: 	CMPI	2047,R0
    // asm 00000329: 	LDIGT	1,R5
    // asm 0000032A: 	SUBI	R3,R2		;FIND YMAX-YMIN
    // asm 0000032B: 	CMPI	2047,R2
    // asm 0000032C: 	LDIGT	1,R5
    // asm 0000032D: 	LDI	R5,R5
    // asm 0000032E: 	RETS

    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLIPCK", 0, 0);
    g_dirq_debug_clipck_calls += 1;

    clipram[0] = FIX(vertex1[0]);
    clipram[1] = FIX(vertex1[1]);
    clipram[2] = FIX(vertex2[0]);
    clipram[3] = FIX(vertex2[1]);
    clipram[4] = FIX(vertex3[0]);
    clipram[5] = FIX(vertex3[1]);
    clipram[6] = FIX(vertex4[0]);
    clipram[7] = FIX(vertex4[1]);

    abs_or = abs(clipram[7]);
    for (i = 6; i >= 0; i--) {
        abs_or |= abs(clipram[i]);
    }
    abs_or >>= 10;
    if (abs_or == 0) {
        return 0;
    }

    xmax = clipram[0];
    xmin = clipram[0];
    ymax = clipram[1];
    ymin = clipram[1];
    for (i = 1; i <= 3; i++) {
        if (clipram[i * 2] > xmax) {
            xmax = clipram[i * 2];
        }
        if (clipram[i * 2] < xmin) {
            xmin = clipram[i * 2];
        }
        if (clipram[i * 2 + 1] > ymax) {
            ymax = clipram[i * 2 + 1];
        }
        if (clipram[i * 2 + 1] < ymin) {
            ymin = clipram[i * 2 + 1];
        }
    }
    if ((xmax - xmin) > 2047) {
        g_dirq_debug_clipck_clipped += 1;
        return 1;
    }
    if ((ymax - ymin) > 2047) {
        g_dirq_debug_clipck_clipped += 1;
        return 1;
    }
    return 0;
}

// *CLIP THE SUCKER
// *SPLIT INTO 4 POLYGONS UL,UR,LL,LR
// *	AR0	4 EXTERNAL VERTICES (INTS)
// *	R0 	PALETTE
// *	R2      FLAGS
// *
static void CLIP(int* clipram /*AR0*/, int palette_base /*R0*/, int control_word /*R2*/, const ROM_POLYGON* polygon /*AR1*/) {
    int entry_count;
    int current_index;
    int* entry;
    int i;
    int x_max;
    int x_min;
    int y_max;
    int y_min;
    int x_mid_12;
    int y_mid_12;
    int x_mid_23;
    int y_mid_23;
    int x_mid_34;
    int y_mid_34;
    int x_mid_41;
    int y_mid_41;
    int x_mid_center;
    int y_mid_center;

    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLIP", 0, 0);

    // asm 0000032F: 	PUSH	IR0
    // asm 00000330: 	PUSH	IR1
    // asm 00000331: 	LDI	2,IR0
    // asm 00000332: 	LDI	3,IR1
    // asm 00000333: 	LDI	R2,AR3		     	;SAVE FLAGS
    // asm 00000334: 	LDI	R0,R4			;SAVE PALETTE
    entry_count = 0;
    // ;	LDIL	CLIPRAM,R0		;LOAD UP SCRATCH AREA
    // ;	CMPI	R0,AR0
    // ;	BNE	$
    // *CHECK TRIVIAL REJECTION
    // *X ALL NEGATIVE CASE
    // *
    // asm 00000335: 	AND	*AR0,*+AR0(IR0),R0
    // asm 00000336: 	AND	*+AR0(4),R0
    // asm 00000337: 	AND	*+AR0(6),R0
    // asm 00000338: 	BND	CLIPX	    		;ALL X <0 REJECT
    if ((clipram[0] & clipram[2] & clipram[4] & clipram[6]) < 0) {
        goto CLIPX;
    }
    // *X ALL POSTIVE CASE
    // asm 00000339: 	LDI	511,R1
    // asm 0000033A: 	SUBI	*AR0,R1,R2
    // asm 0000033B: 	SUBI	*+AR0(IR0),R1,R3
    // 	;------->BND	CLIPX	    	;ALL X <0 REJECT
    // asm 0000033C: 	AND	R3,R2
    // asm 0000033D: 	LDI	511,R3
    // asm 0000033E: 	SUBI	*+AR0(4),R3
    // asm 0000033F: 	AND	R3,R2
    // asm 00000340: 	SUBI	*+AR0(6),R1
    // asm 00000341: 	AND	R1,R2
    // asm 00000342: 	BND	CLIPX			;ALL X >511 REJECT
    if (((511 - clipram[0]) & (511 - clipram[2]) & (511 - clipram[4]) & (511 - clipram[6])) < 0) {
        goto CLIPX;
    }
    // *Y ALL POSTIVE CASE
    // asm 00000343: 	LDI	399,R1
    // asm 00000344: 	SUBI	*+AR0(1),R1,R2
    // asm 00000345: 	SUBI	*+AR0(IR1),R1,R3
    // 	;------->BND	CLIPX	    	;ALL X >511 REJECT
    // asm 00000346: 	AND	R3,R2
    // asm 00000347: 	LDI	511,R3
    // asm 00000348: 	SUBI	*+AR0(5),R3
    // asm 00000349: 	AND	R3,R2
    // asm 0000034A: 	SUBI	*+AR0(7),R1
    // asm 0000034B: 	AND	R1,R2
    // asm 0000034C: 	BND	CLIPX			;ALL Y >511 REJECT
    if (((399 - clipram[1]) & (399 - clipram[3]) & (511 - clipram[5]) & (399 - clipram[7])) < 0) {
        goto CLIPX;
    }
    // *Y ALL NEGATIVE CASE
    // asm 0000034D: 	AND	*+AR0(1),*+AR0(IR1),R0
    // asm 0000034E: 	AND	*+AR0(5),R0
    // asm 0000034F: 	AND	*+AR0(7),R0
    // 	;------->BND	CLIPX	    	;ALL X >511 REJECT
    // asm 00000350: 	BN	CLIPX			;ALL Y >511 REJECT
    if ((clipram[1] & clipram[3] & clipram[5] & clipram[7]) < 0) {
        goto CLIPX;
    }
    // *
    // *COMPUTE YOUR INTERNAL VERTICES
    // *AR0-POINTER TO POLY STACK
    // *AR1=ROM POINTER
    // *UNPACK THE SUCKERS
    // *
    // asm 00000351: 	LDI	*AR1++,R0		;UNPACK THE SUCKERS
    // asm 00000352: 	LDI	0FFH,R2
    // asm 00000353: 	AND	R2,R0,R1
    // asm 00000354: 	STI	R1,*+AR0(8)
    // asm 00000355: 	LSH	-8,R0
    // asm 00000356: 	AND	R2,R0,R1
    // asm 00000357: 	STI	R1,*+AR0(9)
    // asm 00000358: 	LSH	-8,R0
    // asm 00000359: 	AND	R2,R0,R1
    // asm 0000035A: 	STI	R1,*+AR0(10)
    // asm 0000035B: 	LSH	-8,R0
    // asm 0000035C: 	AND	R2,R0,R1
    // asm 0000035D: 	STI	R1,*+AR0(11)
    // asm 0000035E: 	LDI	*AR1++,R0		;UNPACK THE SUCKERS
    // asm 0000035F: 	AND	R2,R0,R1
    // asm 00000360: 	STI	R1,*+AR0(12)
    // asm 00000361: 	LSH	-8,R0
    // asm 00000362: 	AND	R2,R0,R1
    // asm 00000363: 	STI	R1,*+AR0(13)
    // asm 00000364: 	LSH	-8,R0
    // asm 00000365: 	AND	R2,R0,R1
    // asm 00000366: 	STI	R1,*+AR0(14)
    // asm 00000367: 	LSH	-8,R0
    // asm 00000368: 	AND	R2,R0,R1
    // asm 00000369: 	STI	R1,*+AR0(15)
    clipram[8] = (int)(polygon->iv_0_1 & 0xff);
    clipram[9] = (int)((polygon->iv_0_1 >> 8) & 0xff);
    clipram[10] = (int)((polygon->iv_0_1 >> 16) & 0xff);
    clipram[11] = (int)((polygon->iv_0_1 >> 24) & 0xff);
    clipram[12] = (int)(polygon->iv_2_3 & 0xff);
    clipram[13] = (int)((polygon->iv_2_3 >> 8) & 0xff);
    clipram[14] = (int)((polygon->iv_2_3 >> 16) & 0xff);
    clipram[15] = (int)((polygon->iv_2_3 >> 24) & 0xff);
    MAME_ASSERT_REG(0x0000036A, "R1", &clipram[15]);
    // asm 0000036A: 	LDI	AR0,R6
    // asm 0000036B:  	BD	BUSTUP
    // asm 0000036C: 	LDI	R6,R7
    // asm 0000036D: 	ADDI	CLIPRAML-80,R7	     	;GET LENGTH LIMIT
    // asm 0000036E: 	LDI	*AR1++,R5		;GET TEXTURE MAP ADDR
    current_index = 0;
    entry = clipram;
    goto BUSTUP;
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
    // asm 0000036F: 	NOP	*AR0--(16)		;POP OFF OLD ENTRY
    if (entry_count <= 0) {
        goto CLIPDONE;
    }
    current_index = entry_count - 1;
    entry = &clipram[current_index * 16];
    entry_count = current_index;
    // ;	LDI	AR0,R1
    // ;	AND	0FH,R1
    // ;	CMPI	0DH,R1
    // ;	BNE	$
CLIP0:
    // asm 00000370: 	CMPI	AR0,R6			;ARE WE DONE
    // asm 00000371: 	BGT	CLIPDONE		;YES...WERE OUTTA HERE
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
    // asm 00000372: 	AND	*AR0,*+AR0(IR0),R0
    // asm 00000373: 	AND	*+AR0(4),R0
    // asm 00000374: 	AND	*+AR0(6),R0
    // 	;-------->BGTD	CLIPDONE  	;YES...EXIT
    // asm 00000375: 	BND	CLIPOP	    		;ALL X <0 REJECT
    if ((entry[0] & entry[2] & entry[4] & entry[6]) < 0) {
        if (entry_count <= 0) {
            goto CLIPDONE;
        }
        goto CLIPOP;
    }
    // *X ALL POSTIVE CASE
    // asm 00000376: 	LDI	511,R1
    // asm 00000377: 	SUBI	*AR0,R1,R2
    // asm 00000378: 	SUBI	*+AR0(IR0),R1,R3
    // 	;------->BND	CLIPOP	    	;ALL X <0 REJECT
    // asm 00000379: 	AND	R3,R2
    // asm 0000037A: 	LDI	511,R3
    // asm 0000037B: 	SUBI	*+AR0(4),R3
    // asm 0000037C: 	AND	R3,R2
    // asm 0000037D: 	SUBI	*+AR0(6),R1
    // asm 0000037E: 	AND	R1,R2
    // asm 0000037F: 	BND	CLIPOP			;ALL X >511 REJECT
    if (((511 - entry[0]) & (511 - entry[2]) & (511 - entry[4]) & (511 - entry[6])) < 0) {
        if (entry_count <= 0) {
            goto CLIPDONE;
        }
        goto CLIPOP;
    }
    // *Y ALL POSTIVE CASE
    // asm 00000380: 	LDI	399,R1
    // asm 00000381: 	SUBI	*+AR0(1),R1,R2
    // asm 00000382: 	SUBI	*+AR0(IR1),R1,R3
    // 	;------->BND	CLIPOP	    	;ALL X >511 REJECT
    // asm 00000383: 	AND	R3,R2
    // asm 00000384: 	LDI	511,R3
    // asm 00000385: 	SUBI	*+AR0(5),R3
    // asm 00000386: 	AND	R3,R2
    // asm 00000387: 	SUBI	*+AR0(7),R1
    // asm 00000388: 	AND	R1,R2
    // asm 00000389: 	BND	CLIPOP			;ALL Y >511 REJECT
    if (((399 - entry[1]) & (399 - entry[3]) & (511 - entry[5]) & (399 - entry[7])) < 0) {
        if (entry_count <= 0) {
            goto CLIPDONE;
        }
        goto CLIPOP;
    }
    // *Y ALL NEGATIVE CASE
    // asm 0000038A: 	AND	*+AR0(1),*+AR0(IR1),R0
    // asm 0000038B: 	AND	*+AR0(5),R0
    // asm 0000038C: 	AND	*+AR0(7),R0
    // 	;------->BND	CLIPOP	    	;ALL X >511 REJECT
    // asm 0000038D: 	BND	CLIPOP		    	;ALL Y <0 REJECT
    if ((entry[1] & entry[3] & entry[5] & entry[7]) < 0) {
        if (entry_count <= 0) {
            goto CLIPDONE;
        }
        goto CLIPOP;
    }
    // *CHECK OVERSIZE OBJECT
    // asm 0000038E: 	LDI	*AR0,R0			;XMAX
    // asm 0000038E:  ||	LDI	*AR0,R1			;XMIN
    // asm 0000038F: 	LDI	*+AR0(1),R2		;YMAX
    // asm 0000038F:  ||	LDI	*+AR0(1),R3		;YMIN
    // asm 00000390: 	LDI	2,RC
    // 	;------->BND	CLIPOP	    	;Y ALL NEGATIVE	REJECT
    // asm 00000391: 	RPTB	CLCKLP
    // asm 00000392: 	CMPI	*++AR0(IR0),R0
    // asm 00000393: 	LDILT	*AR0,R0
    // asm 00000394: 	CMPI	*AR0,R1
    // asm 00000395: 	LDIGT	*AR0,R1
    // asm 00000396: 	CMPI	*+AR0(1),R2
    // asm 00000397: 	LDILT	*+AR0(1),R2
    // asm 00000398: 	CMPI	*+AR0(1),R3
CLCKLP:
    // asm 00000399: LDIGT	*+AR0(1),R3
    // asm 0000039A: 	SUBI	R1,R0
    // asm 0000039B: 	CMPI	2047,R0
    // asm 0000039C: 	BGTD	BUSTUP
    // asm 0000039D: 	NOP	*AR0--(6)  		;RESET VERTEX INDEX
    // asm 0000039E: 	SUBI	R3,R2
    // asm 0000039F: 	CMPI	2047,R2
    // 	;------->BGTD	BUSTUP
    // asm 000003A0: 	BGT	BUSTUP
    x_max = entry[0];
    x_min = entry[0];
    y_max = entry[1];
    y_min = entry[1];
    for (i = 1; i <= 3; i++) {
        if (entry[i * 2] > x_max) {
            x_max = entry[i * 2];
        }
        if (entry[i * 2] < x_min) {
            x_min = entry[i * 2];
        }
        if (entry[i * 2 + 1] > y_max) {
            y_max = entry[i * 2 + 1];
        }
        if (entry[i * 2 + 1] < y_min) {
            y_min = entry[i * 2 + 1];
        }
    }
    if ((x_max - x_min) > 2047 || (y_max - y_min) > 2047) {
        goto BUSTUP;
    }
    // *POLYGON IS O.K. OUTPUT IT
PCOUT:
PCWT:
    // asm 000003A1: 	LDI	@FIFO_STATUS,R0		;WAIT FOR YOUR FIFO
    // asm 000003A2: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm 000003A3: 	BNZ	PCWT
    // asm 000003A4: 	LDI	R4,R0			;GET PALETTE READY
    // asm 000003A5: 	STI	AR3,*AR7		;STORE YOUR FLAGS
    // asm 000003A6: 	RPTS	7
    // asm 000003A7: 	LDI	*AR0++,R0		;GET EXTERNALS
    // asm 000003A7:  ||	STI	R0,*AR7			;STORE EXTERNALS
    // asm 000003A8:  	STI	R0,*AR7
    // asm 000003A9: 	LDI	*AR0++(IR0),R0	 	;GET YX INTERNAL 1
    // asm 000003A9:  ||	LDI	*+AR0(1),R1
    // asm 000003AA: 	LSH	8,R1
    // asm 000003AB: 	ADDI	R1,R0
    // asm 000003AC:  	STI	R0,*AR7
    // asm 000003AD: 	LDI	*AR0++(IR0),R0		;GET YX INTERNAL 2
    // asm 000003AD:  ||	LDI	*+AR0(1),R1
    // asm 000003AE: 	LSH	8,R1
    // asm 000003AF: 	ADDI	R1,R0
    // asm 000003B0:  	STI	R0,*AR7
    // asm 000003B1: 	LDI	*AR0++(IR0),R0	  	;GET YX INTERNAL 3
    // asm 000003B1:  ||	LDI	*+AR0(1),R1
    // asm 000003B2: 	LSH	8,R1
    // asm 000003B3: 	ADDI	R1,R0
    // asm 000003B4:  	STI	R0,*AR7
    // asm 000003B5: 	LDI	*AR0++(IR0),R0	  	;GET YX INTERNAL 4
    // asm 000003B5:  ||	LDI	*+AR0(1),R1
    // asm 000003B6: 	LSH	8,R1
    // asm 000003B7: 	ADDI	R1,R0
    // asm 000003B8:  	STI	R0,*AR7
    // asm 000003B9: 	STI	R5,*AR7 		;STORE TEXTURE MAP ADDR
    // asm 000003BA: 	LDI	@FIFO_INC,R0		;INC YOUR FIFO
    // asm 000003BB: 	NOP	*AR0--(32)		;READJUST INDEX
    port_output_fpga(
        entry[0],
        entry[1],
        entry[2],
        entry[3],
        entry[4],
        entry[5],
        entry[6],
        entry[7],
        entry[8] + (entry[9] << 8),
        entry[10] + (entry[11] << 8),
        entry[12] + (entry[13] << 8),
        entry[14] + (entry[15] << 8),
        (int)polygon->texture_map_addr,
        palette_base,
        control_word);
    //  ;	LDI	AR0,R1
    //  ;	AND	0FH,R1
    //  ;	CMPI	0DH,R1
    //  ;	BNE	$
    // asm 000003BC: 	B 	CLIP0			;GET NEXT POLYGON
    if (entry_count <= 0) {
        goto CLIPDONE;
    }
    goto CLIPOP;
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
    // asm 000003BD: 	CMPI	R7,AR0			;STACK TOO HIGH?
    // asm 000003BE: 	BLT	BUSTUP0
    // asm 000003BF: 	B	PCOUT			;YES, JUST DO IT...
    if ((current_index * 16) >= (CLIPRAML - 80)) {
        goto PCOUT;
    }
    // *COMPUTE YOUR EXTERNAL VERTICES
BUSTUP0:
    // asm 000003C0: 	LDI	AR0,AR2
    // asm 000003C1: 	LDI	*+AR0(IR0),R0		;X2
    // asm 000003C1:  ||	LDI	*+AR0(IR1),R1		;Y2
    // asm 000003C2: 	STI	R0,*+AR2(12H)
    // asm 000003C3: 	STI	R1,*+AR2(13H)
    // asm 000003C4: 	LDI	*+AR0(4),R0		;X3
    // asm 000003C5: 	STI	R0,*+AR2(24H)
    // asm 000003C6: 	LDI	*+AR0(5),R0		;Y3
    // asm 000003C7: 	STI	R0,*+AR2(25H)
    // asm 000003C8: 	LDI	*+AR0(6),R0		;X4
    // asm 000003C9: 	STI	R0,*+AR2(36H)
    // asm 000003CA: 	LDI	*+AR0(7),R0		;Y4
    // asm 000003CB: 	STI	R0,*+AR2(37H)
    entry[18] = entry[2];
    entry[19] = entry[3];
    entry[36] = entry[4];
    entry[37] = entry[5];
    entry[54] = entry[6];
    entry[55] = entry[7];
    // asm 000003CC: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 000003CD: 	ASH	-1,R0			;X1+X2/2=X5
    // asm 000003CE: 	STI	R0,*+AR2(42H)
    // asm 000003CF: 	STI	R0,*+AR2(10H)
    x_mid_12 = (entry[0] + entry[2]) >> 1;
    entry[66] = x_mid_12;
    entry[16] = x_mid_12;
    // asm 000003D0: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 000003D1: 	ASH	-1,R0			;Y1+Y2/2=Y5
    // asm 000003D2: 	STI	R0,*+AR2(43H)
    // asm 000003D3: 	STI	R0,*+AR2(11H)
    y_mid_12 = (entry[1] + entry[3]) >> 1;
    entry[67] = y_mid_12;
    entry[17] = y_mid_12;
    // asm 000003D4: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 000003D5: 	ASH	-1,R0		       	;X2+X3/2=X6
    // asm 000003D6: 	STI	R0,*+AR2(14H)
    // asm 000003D7: 	STI	R0,*+AR2(22H)
    x_mid_23 = (entry[2] + entry[4]) >> 1;
    entry[20] = x_mid_23;
    entry[34] = x_mid_23;
    // asm 000003D8: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 000003D9: 	ASH	-1,R0			;Y2+Y3/2=Y6
    // asm 000003DA: 	STI	R0,*+AR2(15H)
    // asm 000003DB: 	STI	R0,*+AR2(23H)
    y_mid_23 = (entry[3] + entry[5]) >> 1;
    entry[21] = y_mid_23;
    entry[35] = y_mid_23;
    // asm 000003DC: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 000003DD: 	ASH	-1,R0			;X3+X4/2=X7
    // asm 000003DE: 	STI	R0,*+AR2(26H)
    // asm 000003DF: 	STI	R0,*+AR2(34H)
    x_mid_34 = (entry[4] + entry[6]) >> 1;
    entry[38] = x_mid_34;
    entry[52] = x_mid_34;
    // asm 000003E0: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 000003E1: 	ASH	-1,R0			;Y3+Y4/2=Y7
    // asm 000003E2: 	STI	R0,*+AR2(27H)
    // asm 000003E3: 	STI	R0,*+AR2(35H)
    y_mid_34 = (entry[5] + entry[7]) >> 1;
    entry[39] = y_mid_34;
    entry[53] = y_mid_34;
    // asm 000003E4: 	LDI	*--AR0(6),R0
    // asm 000003E5: 	ADDI	*+AR0(6),R0
    // asm 000003E6: 	ASH	-1,R0			;X1+X4/2=X8
    // asm 000003E7: 	STI	R0,*+AR2(46H)
    // asm 000003E8: 	STI	R0,*+AR2(30H)
    x_mid_41 = (entry[0] + entry[6]) >> 1;
    entry[70] = x_mid_41;
    entry[48] = x_mid_41;
    // asm 000003E9: 	LDI	*+AR0(1),R0
    // asm 000003EA: 	ADDI	*+AR0(7),R0
    // asm 000003EB: 	ASH	-1,R0			;Y1+Y4/2=Y8
    // asm 000003EC: 	STI	R0,*+AR2(47H)
    // asm 000003ED: 	STI	R0,*+AR2(31H)
    y_mid_41 = (entry[1] + entry[7]) >> 1;
    entry[71] = y_mid_41;
    entry[49] = y_mid_41;
    // asm 000003EE: 	ADDI	*AR0,*+AR0(IR0),R0
    // asm 000003EF: 	ADDI	*+AR0(4),R0
    // asm 000003F0: 	ADDI	*+AR0(6),R0
    // asm 000003F1: 	ASH	-2,R0			;X1+X2+X3+X4/4=X9
    // asm 000003F2: 	STI	R0,*+AR2(44H)
    // asm 000003F3: 	STI	R0,*+AR2(16H)
    // asm 000003F4: 	STI	R0,*+AR2(20H)
    // asm 000003F5: 	STI	R0,*+AR2(32H)
    x_mid_center = (entry[0] + entry[2] + entry[4] + entry[6]) >> 2;
    entry[68] = x_mid_center;
    entry[22] = x_mid_center;
    entry[32] = x_mid_center;
    entry[50] = x_mid_center;
    MAME_ASSERT_REG(0x000003F2, "R0", &x_mid_center);
    // asm 000003F6: 	LDI	*+AR0(1),R0
    // asm 000003F7: 	ADDI	*+AR0(3),R0
    // asm 000003F8: 	ADDI	*+AR0(5),R0
    // asm 000003F9: 	ADDI	*+AR0(7),R0
    // asm 000003FA: 	ASH	-2,R0			;Y1+Y2+Y3+Y4/4=Y9
    // asm 000003FB: 	STI	R0,*+AR2(45H)
    // asm 000003FC: 	STI	R0,*+AR2(17H)
    // asm 000003FD: 	STI	R0,*+AR2(21H)
    // asm 000003FE: 	STI	R0,*+AR2(33H)
    y_mid_center = (entry[1] + entry[3] + entry[5] + entry[7]) >> 2;
    entry[69] = y_mid_center;
    entry[23] = y_mid_center;
    entry[33] = y_mid_center;
    entry[51] = y_mid_center;
    // *COMPUTE YOUR INTERNAL VERTICES
    // asm 000003FF: 	NOP	*AR0++(8)
    // asm 00000400: 	LDI	*AR0,R0			;X1
    // asm 00000400:  ||	LDI	*+AR0(1),R1    		;Y1
    // asm 00000401: 	STI	R0,*+AR2(48H)
    // asm 00000402: 	STI	R1,*+AR2(49H)
    // asm 00000403: 	LDI	*+AR0(IR0),R0		;X2
    // asm 00000403:  ||	LDI	*+AR0(IR1),R1		;Y2
    // asm 00000404: 	STI	R0,*+AR2(1AH)
    // asm 00000405: 	STI	R1,*+AR2(1BH)
    // asm 00000406: 	LDI	*+AR0(4),R0		;X3
    // asm 00000407: 	STI	R0,*+AR2(2CH)
    // asm 00000408: 	LDI	*+AR0(5),R0		;Y3
    // asm 00000409: 	STI	R0,*+AR2(2DH)
    // asm 0000040A: 	LDI	*+AR0(6),R0		;X4
    // asm 0000040B: 	STI	R0,*+AR2(3EH)
    // asm 0000040C: 	LDI	*+AR0(7),R0		;Y4
    // asm 0000040D: 	STI	R0,*+AR2(3FH)
    entry[72] = entry[8];
    entry[73] = entry[9];
    entry[26] = entry[10];
    entry[27] = entry[11];
    entry[44] = entry[12];
    entry[45] = entry[13];
    entry[62] = entry[14];
    entry[63] = entry[15];
    // asm 0000040E: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 0000040F: 	ASH	-1,R0			;X1+X2/2=X5
    // asm 00000410: 	STI	R0,*+AR2(4AH)
    // asm 00000411: 	STI	R0,*+AR2(18H)
    x_mid_12 = (entry[8] + entry[10]) >> 1;
    entry[74] = x_mid_12;
    entry[24] = x_mid_12;
    // asm 00000412: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 00000413: 	ASH	-1,R0			;Y1+Y2/2=Y5
    // asm 00000414: 	STI	R0,*+AR2(4BH)
    // asm 00000415: 	STI	R0,*+AR2(19H)
    y_mid_12 = (entry[9] + entry[11]) >> 1;
    entry[75] = y_mid_12;
    entry[25] = y_mid_12;
    // asm 00000416: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 00000417: 	ASH	-1,R0		       	;X2+X3/2=X6
    // asm 00000418: 	STI	R0,*+AR2(1CH)
    // asm 00000419: 	STI	R0,*+AR2(2AH)
    x_mid_23 = (entry[10] + entry[12]) >> 1;
    entry[28] = x_mid_23;
    entry[42] = x_mid_23;
    // asm 0000041A: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 0000041B: 	ASH	-1,R0			;Y2+Y3/2=Y6
    // asm 0000041C: 	STI	R0,*+AR2(1DH)
    // asm 0000041D: 	STI	R0,*+AR2(2BH)
    y_mid_23 = (entry[11] + entry[13]) >> 1;
    entry[29] = y_mid_23;
    entry[43] = y_mid_23;
    // asm 0000041E: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 0000041F: 	ASH	-1,R0			;X3+X4/2=X7
    // asm 00000420: 	STI	R0,*+AR2(2EH)
    // asm 00000421: 	STI	R0,*+AR2(3CH)
    x_mid_34 = (entry[12] + entry[14]) >> 1;
    entry[46] = x_mid_34;
    entry[60] = x_mid_34;
    // asm 00000422: 	ADDI	*AR0++,*+AR0(IR0),R0
    // asm 00000423: 	ASH	-1,R0			;Y3+Y4/2=Y7
    // asm 00000424: 	STI	R0,*+AR2(2FH)
    // asm 00000425: 	STI	R0,*+AR2(3DH)
    y_mid_34 = (entry[13] + entry[15]) >> 1;
    entry[47] = y_mid_34;
    entry[61] = y_mid_34;
    // asm 00000426: 	LDI	*--AR0(6),R0
    // asm 00000427: 	ADDI	*+AR0(6),R0
    // asm 00000428: 	ASH	-1,R0			;X1+X4/2=X8
    // asm 00000429: 	STI	R0,*+AR2(4EH)
    // asm 0000042A: 	STI	R0,*+AR2(38H)
    x_mid_41 = (entry[8] + entry[14]) >> 1;
    entry[78] = x_mid_41;
    entry[56] = x_mid_41;
    // asm 0000042B: 	LDI	*+AR0(1),R0
    // asm 0000042C: 	ADDI	*+AR0(7),R0
    // asm 0000042D: 	ASH	-1,R0			;Y1+Y4/2=Y8
    // asm 0000042E: 	STI	R0,*+AR2(4FH)
    // asm 0000042F: 	STI	R0,*+AR2(39H)
    y_mid_41 = (entry[9] + entry[15]) >> 1;
    entry[79] = y_mid_41;
    entry[57] = y_mid_41;
    // asm 00000430: 	ADDI	*AR0,*+AR0(IR0),R0
    // asm 00000431: 	ADDI	*+AR0(4),R0
    // asm 00000432: 	ADDI	*+AR0(6),R0
    // asm 00000433: 	ASH	-2,R0			;X1+X2+X3+X4/4=X9
    // asm 00000434: 	STI	R0,*+AR2(4CH)
    // asm 00000435: 	STI	R0,*+AR2(1EH)
    // asm 00000436: 	STI	R0,*+AR2(28H)
    // asm 00000437: 	STI	R0,*+AR2(3AH)
    x_mid_center = (entry[8] + entry[10] + entry[12] + entry[14]) >> 2;
    entry[76] = x_mid_center;
    entry[30] = x_mid_center;
    entry[40] = x_mid_center;
    entry[58] = x_mid_center;
    // asm 00000438: 	LDI	*+AR0(1),R0
    // asm 00000439: 	ADDI	*+AR0(3),R0
    // asm 0000043A: 	ADDI	*+AR0(5),R0
    // asm 0000043B: 	ADDI	*+AR0(7),R0
    // asm 0000043C: 	ASH	-2,R0			;Y1+Y2+Y3+Y4/4=Y9
    // asm 0000043D: 	STI	R0,*+AR2(4DH)
    // asm 0000043E: 	STI	R0,*+AR2(1FH)
    // asm 0000043F: 	STI	R0,*+AR2(29H)
    // asm 00000440: 	STI	R0,*+AR2(3BH)
    y_mid_center = (entry[9] + entry[11] + entry[13] + entry[15]) >> 2;
    entry[77] = y_mid_center;
    entry[31] = y_mid_center;
    entry[41] = y_mid_center;
    entry[59] = y_mid_center;
    MAME_ASSERT_REG(0x0000043D, "R0", &y_mid_center);
    // *TRANSFER LAST ENTRY INTO FIRST ONE
    // asm 00000441: 	NOP	*AR2++(42H)
    // asm 00000442: 	NOP	*AR0--(6)		;POINT TO X2,Y2
    // asm 00000443: 	LDI	*AR2++,R0		;GET FIRST
    // asm 00000444: 	RPTS	12
    // asm 00000445: 	LDI	*AR2++,R0
    // asm 00000445:  ||	STI	R0,*AR0++
    // asm 00000446: 	STI	R0,*AR0++(31H)		;STORE LAST ONE, RESET INDEX TO TOS
    for (i = 0; i < 14; i++) {
        entry[2 + i] = entry[66 + i];
    }
    entry_count = current_index + 4;
    // ;	LDI	AR0,R1
    // ;	AND	0FH,R1
    // ;	CMPI	0DH,R1
    // ;	BNE	$
    // asm 00000447: 	B	CLIPOP
    goto CLIPOP;
CLIPX:
    // asm 00000448: 	NOP	*AR0--(16)		;RESTORE AR0
    // asm 00000449: 	ADDI	3,AR1			;SKIP THE INTERNAL VERTS AND TM CRAP
    goto CLIPDONE;
CLIPDONE:
    // asm 0000044A: 	LDI	-16,R6			;RESTORE R6
    // asm 0000044B: 	LDI	0FFH,R7			;RESTORE R7
    // asm 0000044C: 	NOP	*AR0++(16)		;RESTORE AR0
    // asm 0000044D: 	LDIL	CLIPRAM,R0		;LOAD UP SCRATCH AREA
    clipram = (int*)CLIPRAM;
#if DEBUG
    // asm: 	CMPI	R0,AR0
    // asm: 	BNE	$
#endif
    // asm 00000450: 	POP	IR1
    // asm 00000451: 	POP	IR0
    // asm 00000452: 	RETS
    return;
}

// *PLOT A DISTANT POLYGON
// *
// *R7=0FFH
// *IR0=BLOWLIST
// *IR1=BLOWLIST+1
// *RC=POLYGON COUNT
// *
static void PLTPOLY(void) {
    // ;	PUSH	AR0
    // ;	LDI	AR6,RC
    // asm 00000453: 	BUD	PLTPOLYLP
    // asm 00000454: 	LDP	@FIFO_STATUS
    // asm 00000455: 	LDI	FIFO_ADDR>>16,AR7
    // asm 00000456: 	LSH	16,AR7
    // asm 00000457: 	BU	PLTPOLYLP		; jeff edited to match I450 rom
PLTPOLYLP:
    // asm 00000460: 	RPTB	PLTPOLL
    // asm 00000461: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 00000462: 	AND	R7,R3,AR4
PLTPOLYLP1:
    // asm 00000463: 	MPYI	3,AR4			;V1
    // asm 00000464: 	LSH	-8,R3
    // asm 00000465: 	AND	R7,R3,AR5
    // asm 00000466: 	MPYI	3,AR5			;V2
    // asm 00000467: 	LSH	-8,R3
    // asm 00000468: 	AND	R7,R3,AR2
    // asm 00000469: 	MPYI	3,AR2			;V3
PLTWT:
    // 	;CHECK FIFO FULL
    // asm 0000046A: 	LDI	@FIFO_STATUS,R0
    // asm 0000046B: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm 0000046C: 	BNZD	PLTWT
    // 	;CHECK HIDDEN SURFACE REMOVAL
    // asm 0000046D: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm 0000046E: 	SUBF	*+AR4(IR1),*+AR5(IR1),R2	;dy = ay - by
    // asm 0000046F: 	SUBF	*+AR2(IR0),*+AR5(IR0),R0	;ex = cx - bx
    // 	;------->BNZD	PLTWT
    // asm 00000470: 	MPYF	R2,R0,R0			;ex = dy * ex
    // asm 00000470:   ||	SUBF	*+AR2(IR1),*+AR5(IR1),R2	;ey = cy - by
    // asm 00000471: 	MPYF	R1,R2			;ey = dx * ey
    // asm 00000472: 	SUBF	R0,R2			;ey = ey - ex
    // asm 00000473: 	BGTD	PLTLP1			;if back facing DONT PLOT
    // asm 00000474: 	LSH	-8,R3			;START GETTING V4
    // asm 00000475: 	LDI	3,R1
    // asm 00000476: 	NOP
    // 	;------->BGTD	PLTLP1
    // asm 00000477: 	LDI	*AR1++(2),R2		;get control word
    // asm 00000478: 	LSH	R6,R2,R0		;SHIFT 16 TO RIGHT
    // asm 00000479: 	MPYI	R1,R3,AR3		;GET LAST VERTEX  V4
    // asm 0000047A: 	ADDI	R0,BK,AR6
    // asm 0000047B: 	LSH	R6,*AR6,R0		;PALETTE->R0
    // asm 0000047B:  ||	STI	R2,*AR7
    // asm 0000047C:  	LSH	8,R0			;not a good way to do this fix l8r -7/14/93
    // asm 0000047D: 	FIX	*+AR4(IR0),R0		;READ X1
    // asm 0000047D:  ||	STI	R0,*AR7			;STUFF the PALETTE
    // asm 0000047E: 	FIX	*+AR4(IR1),R0		;READ X1
    // asm 0000047E:  ||	STI	R0,*AR7
    // asm 0000047F: 	FIX	*+AR5(IR0),R0		;READ X2
    // asm 0000047F:  ||	STI	R0,*AR7
    // asm 00000480: 	FIX	*+AR5(IR1),R0		;READ Y2
    // asm 00000480:  ||	STI	R0,*AR7
    // asm 00000481: 	FIX	*+AR2(IR0),R0		;READ X3
    // asm 00000481:  ||	STI	R0,*AR7
    // asm 00000482: 	FIX	*+AR2(IR1),R0		;READ Y3
    // asm 00000482:  ||	STI	R0,*AR7
    // asm 00000483: 	FIX	*+AR3(IR0),R0		;READ X4
    // asm 00000483:  ||	STI	R0,*AR7
    // asm 00000484: 	FIX	*+AR3(IR1),R0		;READ Y4
    // asm 00000484:  ||	STI	R0,*AR7
    // asm 00000485:  	STI	R0,*AR7
    // asm 00000486: 	LDI	*AR1++,R0
    // asm 00000487: 	LDI	*AR1++,R1
    // asm 00000488: 	LDI	*AR1++,R2
    // asm 00000488:  ||	STI	R0,*AR7
    // asm 00000489: 	LSH	-16,R0
    // asm 0000048A: 	STI	R0,*AR7
    // asm 0000048B:  	STI	R1,*AR7
    // asm 0000048C: 	LSH	-16,R1
    // asm 0000048D: 	STI	R1,*AR7
    // asm 0000048E:  	STI	R2,*AR7
PLTPOLL:
    // asm 0000048F: 	LDI	@FIFO_INC,R0
    // ;	POP	AR0
    // asm 00000490: 	RETS
PLTLP1:
    // asm 00000491: 	SUBI	1,RC
    // asm 00000492: 	LDI	RC,R0
    // asm 00000493: 	BNND	PLTPOLYLP1
    // asm 00000494: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 00000495: 	SUBI	1,AR1
    // asm 00000496: 	AND	R7,R3,AR4
    // 	;----> BNND PLTPOLYLP1
PLTXX:
    // ;	POP	AR0
    // asm 00000497: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLTPOLY", 0, 0);
    UNIMPL();
}

// *
// *PLOT POLYGONS FOR A ONE PALETTE TEXTURE MAP OBJECT
// *
// *R0=ODIST-ORAD
// *RC=POLYGON COUNT
// *
static void PLOT1PAL(OBJ* obj /*AR0*/, const ROM_POLYGON* polygons /*AR1*/, int polygon_count_minus_one /*BK*/) {
    int* clipram;
    const ROM_POLYGON* polygon;
    int polygon_index;
    // asm 00000498: 	CMPI	1000,R0
    // asm 00000499: 	BGTD	PLT1PAL			;YES, NO CLIP LOOP
    // asm 0000049A: 	LDI	@BLOWLISTI,IR0
    // asm 0000049B: 	LDI	IR0,IR1
    // asm 0000049C: 	ADDI	1,IR1
    // 	;---->BGTD	PLT1PAL			;YES, NO CLIP LOOP
    // asm 0000049D: 	PUSH 	AR0
    // asm 0000049E: 	LDI	RC,AR6
    // asm 0000049F: 	LDI	0FFH,R7			;GET MASK
    // asm 000004A0: 	LDI	-16,R6			;SHIFT COUNT
    // asm 000004A1: 	LDI	*+AR0(OPAL),BK
    // asm 000004A2: 	LDI	@CLIPRAMI,AR0
    // asm 000004A3: 	BUD	PLOTPOLYLP_1
    // asm 000004A4: 	LDP	@FIFO_STATUS
    // asm 000004A5: 	LDI	FIFO_ADDR>>16,AR7
    // asm 000004A6: 	LSH	16,AR7
    // asm 000004A7: 	BU	PLTPOLYLP_1		; jeff edited to match I450 rom
PLOTPOLYLP_1:
    // *GET EXTERNAL VERTEX INDICIES
    // asm 000004C0: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 000004C1: 	AND	R7,R3,AR4
PLOTPOLYLP1_1:
    // asm 000004C2: 	ADDI	1,IR1
    // asm 000004C3: 	MPYI	3,AR4			;V1
    // asm 000004C4: 	LSH	-8,R3
    // asm 000004C5: 	AND	R7,R3,AR5
    // asm 000004C6: 	MPYI	3,AR5			;V2
    // asm 000004C7: 	LSH	-8,R3
    // asm 000004C8: 	AND	R7,R3,AR2
    // asm 000004C9: 	MPYI	3,AR2			;V3
    // asm 000004CA: 	LSH	-8,R3
    // asm 000004CB: 	AND	R7,R3,AR3
    // asm 000004CC: 	MPYI	3,AR3			;V4
    // *CHECK ALL Z'S <=0
    // asm 000004CD: 	LDF	*+AR4(IR1),R0
    // asm 000004CE: 	BGED	INBNDS_1
    // asm 000004CF: 	AND	*+AR5(IR1),*+AR2(IR1),R0
    // asm 000004D0: 	AND	*+AR3(IR1),R0
    // asm 000004D1: 	SUBI	1,IR1
    // 	;------->BGED	INBNDS_1
    // asm 000004D2: 	LSH	8,R0
    // asm 000004D3: 	BND	POLYLP_1
    // asm 000004D4: 	NOP
    // *CHECK FIFO FULL
INBNDS_1:
    // asm 000004D5: 	LDI	@FIFO_STATUS,R0
    // asm 000004D6: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // 	;---->BCD	POLYLP_1
    // asm 000004D7: 	BNZD	INBNDS_1
    // *CHECK HIDDEN SURFACE REMOVAL
    // asm 000004D8: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm 000004D9: 	SUBF	*+AR4(IR1),*+AR5(IR1),R3	;dy = ay - by
    // asm 000004DA: 	SUBF	*+AR5(IR0),*+AR2(IR0),R0	;ex = cx - bx
    // 	;------->BNZD	INBNDS_1
    // asm 000004DB: 	MPYF	R3,R0				;ex = dy * ex
    // asm 000004DB:   ||	SUBF	*+AR5(IR1),*+AR2(IR1),R2	;ey = cy - by
    // asm 000004DC: 	MPYF	R2,R1				;ey = dx * ey
    // asm 000004DD: 	SUBF	R1,R0				;ey = ey - ex
    // asm 000004DE: 	BGTD	POLYLP_1				;if back facing DONT PLOT
    // *GLITCH FIX
    // asm 000004DF: 	SUBF	*+AR2(IR1),*+AR3(IR1),R0
    // asm 000004E0: 	MPYF	R3,R0
    // asm 000004E0: ||	SUBF	*+AR3(IR1),*+AR4(IR1),R3
    // asm 000004E1: 	CMPI	AR2,AR3
    // 	;------>BGTD	POLYLP_1			;if back facing DONT PLOT
    // asm 000004E2: 	BZD	LOF2X
    // asm 000004E3: 	MPYF	R3,R2
    // asm 000004E4: 	OR	R2,R0
    // asm 000004E5: 	LDF	R0,R0
    // asm 000004E6: 	BGT	POLYLP_1
LOF2X:
    // *
    // *AR4,AR5,AR2,AR3 = FOUR VERTICES
    // *IR1=PALETTE
    // *
    // asm 000004E7: 	CALL	CLIPCK
    // asm 000004E8: 	BNZD	CLIPIT_1		;GO CLIP IT DUDES
    // asm 000004E9: 	LDI	*AR1++(2),R2		;get control word/palette
    // asm 000004EA: 	LSH	R6,R2,R0		;SHIFT 16 TO RIGHT
    // asm 000004EB: 	LDI	BK,R0
    // 	;------->BNZD	CLIPIT_1	;YES SPLIT IT UP...
    // asm 000004EC: 	STI	R2,*AR7
    // ;	LSH	R6,*AR4,R0		;PALETTE->R0
    // ; 	LSH	8,R0			;not a good way to do this fix l8r -7/14/93
    // *NO CLIP, BLOW IT OUT
BLOWOUT_1:
    // asm 000004ED: 	RPTS	7
    // asm 000004EE: 	LDI	*AR0++,R0
    // asm 000004EE:  ||	STI	R0,*AR7
    // asm 000004EF: 	NOP	*AR0--(8)		;READJUST INDEX DUDES...
    // asm 000004F0: 	LDI	*AR1++,R0		;2 AIV packed format Y2:X2:Y1:X1
    // asm 000004F0:  ||	STI	R0,*AR7
    // asm 000004F1: 	STI	R0,*AR7
    // asm 000004F2: 	LSH	-16,R0
    // asm 000004F3: 	LDI	*AR1++,R0		;GET Y4:X4:Y3:X3
    // asm 000004F3:  ||	STI	R0,*AR7
    // asm 000004F4: 	STI	R0,*AR7
    // asm 000004F5: 	LSH	-16,R0
    // asm 000004F6: 	DBUD	AR6,PLOTPOLYLP_1
    // asm 000004F7: 	LDI	*AR1++,R0
    // asm 000004F7:  ||	STI	R0,*AR7
    // asm 000004F8: 	STI	R0,*AR7			;STORE THE ADDR
    // asm 000004F9: 	LDI	@FIFO_INC,R0
    // 	;----> DBUD AR6,PLOTPOLYLP_1
    // asm 000004FA: 	POP	AR0			;RESTORE OBJECT POINTER
    // asm 000004FB: 	RETS
POLYLP_1:
    // asm 000004FC: 	DBUD	AR6,PLOTPOLYLP1_1
    // asm 000004FD: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 000004FE: 	SUBI	1,AR1
    // asm 000004FF: 	AND	R7,R3,AR4
    // 	;----> DBUD AR6,PLOTPOLYLP1_1
    // asm 00000500: 	POP	AR0
    // asm 00000501: 	RETS
    // *CLIP IT
CLIPIT_1:
    // asm 00000502: 	CALL 	CLIP
    // asm 00000503: 	DBU	AR6,PLOTPOLYLP_1
    // asm 00000504: 	POP	AR0
    // asm 00000505: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLOT1PAL", 0, 0);
    g_dirq_debug_plot1pal_calls += 1;

    clipram = (int*)CLIPRAM;
    polygon = polygons;
    for (polygon_index = 0; polygon_index <= polygon_count_minus_one; polygon_index++, polygon++) {
        int packed_vertices;
        int clip;
        int* clip_vertex;
        const c3x_f32_t* vertex1;
        const c3x_f32_t* vertex2;
        const c3x_f32_t* vertex3;
        const c3x_f32_t* vertex4;
        int v1;
        int v2;
        int v3;
        int v4;
        int base1;
        int base2;
        int base3;
        int base4;
        c3x_reg_t z1;
        c3x_reg_t z2;
        c3x_reg_t z3;
        c3x_reg_t z4;
        c3x_reg_t dx;
        c3x_reg_t dy;
        c3x_reg_t ex;
        c3x_reg_t ey;
        c3x_reg_t glitch_dx;
        c3x_reg_t glitch_dy;
        int control_word;

        g_dirq_debug_plot1pal_polygons += 1;
        packed_vertices = (int)polygon->vertices_4_3_2_1;
        v1 = packed_vertices & 0xff;
        base1 = v1 * 3;
        v2 = (packed_vertices >> 8) & 0xff;
        base2 = v2 * 3;
        v3 = (packed_vertices >> 16) & 0xff;
        base3 = v3 * 3;
        v4 = (packed_vertices >> 24) & 0xff;
        base4 = v4 * 3;
        vertex1 = &BLOWLIST[base1];
        vertex2 = &BLOWLIST[base2];
        vertex3 = &BLOWLIST[base3];
        vertex4 = &BLOWLIST[base4];

    z1 = C3X_LDF(vertex1[2]);
    z2 = C3X_LDF(vertex2[2]);
    z3 = C3X_LDF(vertex3[2]);
    z4 = C3X_LDF(vertex4[2]);
        if (C3X_LT(z1, C3X_FROM_INT(0)) && C3X_LT(z2, C3X_FROM_INT(0)) && C3X_LT(z3, C3X_FROM_INT(0)) && C3X_LT(z4, C3X_FROM_INT(0))) {
            g_dirq_debug_plot1pal_z_rejects += 1;
            continue;
        }

        dx = C3X_SUB(vertex2[0], vertex1[0]);
        dy = C3X_SUB(vertex2[1], vertex1[1]);
        ex = C3X_SUB(vertex3[0], vertex2[0]);
        ey = C3X_SUB(vertex3[1], vertex2[1]);
        if (g_dirq_debug_plot1pal_first_obj_id < 0) {
            g_dirq_debug_plot1pal_first_obj_id = (int)obj->id;
            g_dirq_debug_plot1pal_first_poly = polygon_index;
    g_dirq_debug_plot1pal_first_ax = C3X_LDF(vertex1[0]);
    g_dirq_debug_plot1pal_first_ay = C3X_LDF(vertex1[1]);
    g_dirq_debug_plot1pal_first_bx = C3X_LDF(vertex2[0]);
    g_dirq_debug_plot1pal_first_by = C3X_LDF(vertex2[1]);
    g_dirq_debug_plot1pal_first_cx = C3X_LDF(vertex3[0]);
    g_dirq_debug_plot1pal_first_cy = C3X_LDF(vertex3[1]);
            g_dirq_debug_plot1pal_first_cross = C3X_SUB(C3X_MUL(dy, ex), C3X_MUL(dx, ey));
        }
        if (C3X_GT(C3X_SUB(C3X_MUL(dy, ex), C3X_MUL(dx, ey)), C3X_FROM_INT(0))) {
            g_dirq_debug_plot1pal_hsr_rejects += 1;
            continue;
        }

        glitch_dx = C3X_SUB(vertex4[1], vertex3[1]);
        glitch_dy = C3X_SUB(vertex1[1], vertex4[1]);
        if (v3 != v4) {
            if ((FIX(C3X_MUL(dy, glitch_dx)) | FIX(C3X_MUL(glitch_dy, ey))) > 0) {
                g_dirq_debug_plot1pal_glitch_rejects += 1;
                continue;
            }
        }

        clip = CLIPCK(vertex1, vertex2, vertex3, vertex4, clipram);
        if (clip != 0) {
            g_dirq_debug_plot1pal_clip_rejects += 1;
            control_word = (int)polygon->palnum_and_cntl;
            CLIP(clipram, (int)obj->palette, control_word, polygon);
            continue;
        }

        control_word = (int)polygon->palnum_and_cntl;
        g_dirq_debug_plot1pal_emits += 1;
        clip_vertex = clipram;
        port_output_fpga(
            clip_vertex[0],
            clip_vertex[1],
            clip_vertex[2],
            clip_vertex[3],
            clip_vertex[4],
            clip_vertex[5],
            clip_vertex[6],
            clip_vertex[7],
            (int)(polygon->iv_0_1 & 0xffff),
            (int)((polygon->iv_0_1 >> 16) & 0xffff),
            (int)(polygon->iv_2_3 & 0xffff),
            (int)((polygon->iv_2_3 >> 16) & 0xffff),
            (int)polygon->texture_map_addr,
            (int)obj->palette,
            control_word);
    }
}

// *PLOT A DISTANT 1 PALETTE POLYGON
// *
// *IR0=BLOWLIST
// *IR1=BLOWLIST+1
// *RC=POLYGON COUNT
// *
static void PLT1PAL(void) {
    // asm 00000506: 	CMPI	50,RC			;BIG OBJECT?
    // asm 00000507: 	BLTD	PLT1PAL1
    // asm 00000508: 	LDI	0FFH,R7			;GET MASK
    // asm 00000509: 	LDI	*+AR0(OPAL),R6
    // asm 0000050A: 	LDI	*+AR0(ODIST),R0
    // 	;------>BLTD	PLT1PAL1
    // asm 0000050B: 	CMPI	DEGRADE_DIST,R0		;CHECK IF DISTANT
    // asm 0000050C: 	BGT	PLT1PAL1		;YES, FORGET IT
    // asm 0000050D: 	LDI	*+AR0(OID),R0		;CHECK FOR CAR
    // asm 0000050E: 	CMPI	484H,R0	   		;OPENING FERRARI?
    // asm 0000050F: 	BZ	PLTG1PAL		;YES...
    // asm 00000510: 	AND	CLASS_M,R0
    // asm 00000511: 	CMPI	DRONE_C,R0
    // asm 00000512: 	BGT	PLT1PAL1		;NOT A CAR
    // asm 00000513: 	CMPI	PLYR_C,R0
    // asm 00000514: 	BGE	PLTG1PAL		;ITS A CAR...
PLT1PAL1:
    // asm 00000515: 	BUD	PLTPOLYLP_2
    // asm 00000516: 	LDP	@FIFO_STATUS
    // asm 00000517: 	LDI	FIFO_ADDR>>16,AR7
    // asm 00000518: 	LSH	16,AR7
    // asm 00000519: 	BU	PLTPOLYLP_2		; jeff edited to match I450 rom
PLTPOLYLP_2:
    // asm 00000520: 	RPTB	PLTPOLL_2
    // asm 00000521: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 00000522: 	AND	R7,R3,AR4
PLTPOLYLP1_2:
    // asm 00000523: 	MPYI	3,AR4			;V1
    // asm 00000524: 	LSH	-8,R3
    // asm 00000525: 	AND	R7,R3,AR5
    // asm 00000526: 	MPYI	3,AR5			;V2
    // asm 00000527: 	LSH	-8,R3
    // asm 00000528: 	AND	R7,R3,AR2
    // asm 00000529: 	MPYI	3,AR2			;V3
PLTWT_2:
    // 	;CHECK FIFO FULL
    // asm 0000052A: 	LDI	@FIFO_STATUS,R0
    // asm 0000052B: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm 0000052C: 	BNZD	PLTWT_2
    // 	;CHECK HIDDEN SURFACE REMOVAL
    // asm 0000052D: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm 0000052E: 	SUBF	*+AR4(IR1),*+AR5(IR1),R2	;dy = ay - by
    // asm 0000052F: 	SUBF	*+AR2(IR0),*+AR5(IR0),R0	;ex = cx - bx
    // 	;------->BNZD	PLTWT_2
    // asm 00000530: 	MPYF	R2,R0,R0			;ex = dy * ex
    // asm 00000530:   ||	SUBF	*+AR2(IR1),*+AR5(IR1),R2	;ey = cy - by
    // asm 00000531: 	MPYF	R1,R2			;ey = dx * ey
    // asm 00000532: 	SUBF	R0,R2			;ey = ey - ex
    // asm 00000533: 	BGTD	PLTLP1_2		;if back facing DONT PLOT
    // asm 00000534: 	LSH	-8,R3			;START GETTING V4
    // asm 00000535: 	LDI	3,R1
    // asm 00000536: 	NOP
    // 	;------->BGTD	PLTLP1_2
    // asm 00000537: 	LDI	*AR1++(2),R2		;get control word
    // asm 00000538: 	LDI	R2,R0			;SHIFT 16 TO RIGHT
    // asm 00000539: 	RS	16,R0
    // asm 0000053A: 	MPYI	R1,R3,AR3		;GET LAST VERTEX  V4
    // asm 0000053B: 	STI	R2,*AR7
    // asm 0000053C: 	FIX	*+AR4(IR0),R0		;READ X1
    // asm 0000053C:  ||	STI	R6,*AR7			;STUFF the PALETTE
    // asm 0000053D: 	FIX	*+AR4(IR1),R0		;READ X1
    // asm 0000053D:  ||	STI	R0,*AR7
    // asm 0000053E: 	FIX	*+AR5(IR0),R0		;READ X2
    // asm 0000053E:  ||	STI	R0,*AR7
    // asm 0000053F: 	FIX	*+AR5(IR1),R0		;READ Y2
    // asm 0000053F:  ||	STI	R0,*AR7
    // asm 00000540: 	FIX	*+AR2(IR0),R0		;READ X3
    // asm 00000540:  ||	STI	R0,*AR7
    // asm 00000541: 	FIX	*+AR2(IR1),R0		;READ Y3
    // asm 00000541:  ||	STI	R0,*AR7
    // asm 00000542: 	FIX	*+AR3(IR0),R0		;READ X4
    // asm 00000542:  ||	STI	R0,*AR7
    // asm 00000543: 	FIX	*+AR3(IR1),R0		;READ Y4
    // asm 00000543:  ||	STI	R0,*AR7
    // asm 00000544:  	STI	R0,*AR7
    // asm 00000545: 	LDI	*AR1++,R0
    // asm 00000546: 	LDI	*AR1++,R1
    // asm 00000547: 	LDI	*AR1++,R2
    // asm 00000547:  ||	STI	R0,*AR7
    // asm 00000548: 	LSH	-16,R0
    // asm 00000549: 	STI	R0,*AR7
    // asm 0000054A:  	STI	R1,*AR7
    // asm 0000054B: 	LSH	-16,R1
    // asm 0000054C: 	STI	R1,*AR7
    // asm 0000054D:  	STI	R2,*AR7
PLTPOLL_2:
    // asm 0000054E: 	LDI	@FIFO_INC,R0
    // asm 0000054F: 	RETS
PLTLP1_2:
    // asm 00000550: 	SUBI	1,RC
    // asm 00000551: 	LDI	RC,R0
    // asm 00000552: 	BNND	PLTPOLYLP1_2
    // asm 00000553: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 00000554: 	SUBI	1,AR1
    // asm 00000555: 	AND	R7,R3,AR4
    // 	;----> BNND PLTPOLYLP1_2
    // asm 00000556: 	RETS
    // *****************************
    // *PLOT A DISTANT 1 PALETTE POLYGON GLITCH FIX
    // *
PLTG1PAL:
    // asm 00000557: 	BUD	PLTGPOLYLP_2
    // asm 00000558: 	LDP	@FIFO_STATUS
    // asm 00000559: 	LDI	FIFO_ADDR>>16,AR7
    // asm 0000055A: 	LSH	16,AR7
    // asm 0000055B: 	BU	PLTGPOLYLP_2		; jeff edited to match I450 rom
PLTGPOLYLP_2:
    // asm 00000560: 	RPTB	PLTGPOLL_2
    // asm 00000561: 	LDI	*+AR1(1),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 00000562: 	AND	R7,R3,AR4
PLTGPOLYLP1_2:
    // asm 00000563: 	MPYI	3,AR4			;V1
    // asm 00000564: 	LSH	-8,R3
    // asm 00000565: 	AND	R7,R3,AR5
    // asm 00000566: 	MPYI	3,AR5			;V2
    // asm 00000567: 	LSH	-8,R3
    // asm 00000568: 	AND	R7,R3,AR2
    // asm 00000569: 	MPYI	3,AR2			;V3
    // asm 0000056A: 	LSH	-8,R3
    // asm 0000056B: 	AND	R7,R3,AR3
    // asm 0000056C: 	MPYI	3,AR3			;V4
PLTGWT_2:
    // 	;CHECK FIFO FULL
    // asm 0000056D: 	LDI	@FIFO_STATUS,R0
    // asm 0000056E: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm 0000056F: 	BNZD	PLTGWT_2
    // 	;CHECK HIDDEN SURFACE REMOVAL
    // asm 00000570: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
    // asm 00000571: 	SUBF	*+AR4(IR1),*+AR5(IR1),R3	;dy = ay - by
    // asm 00000572: 	SUBF	*+AR5(IR0),*+AR2(IR0),R0	;ex = bx - cx
    // 	;------->BNZD	PLTGWT_2
    // asm 00000573: 	MPYF	R3,R0				;ex = dy * ex
    // asm 00000573:   ||	SUBF	*+AR5(IR1),*+AR2(IR1),R2	;ey = by - cy
    // asm 00000574: 	MPYF	R2,R1				;ey = dx * ey
    // asm 00000575: 	SUBF	R1,R0				;ey = ey - ex
    // asm 00000576: 	BGTD	PLTGLP1_2			;if back facing DONT PLOT
    // *LOF KLUDGE - CHECK FOR NON-PLANAR
    // asm 00000577: 	SUBF	*+AR2(IR1),*+AR3(IR1),R0
    // asm 00000578: 	MPYF	R3,R0
    // asm 00000578: ||	SUBF	*+AR3(IR1),*+AR4(IR1),R3
    // asm 00000579: 	CMPI	AR2,AR3
    // asm 0000057A: 	BZD	LOF4X
    // asm 0000057B: 	MPYF	R3,R2
    // asm 0000057C: 	OR	R2,R0
    // asm 0000057D: 	LDF	R0,R0
    // asm 0000057E: 	BGT	PLTGLP1_2
LOF4X:
    // asm 0000057F: 	LDI	*AR1++(2),R2		;get control word
    // asm 00000580: 	LDI	R2,R0			;SHIFT 16 TO RIGHT
    // asm 00000581: 	RS	16,R0
    // asm 00000582: 	STI	R2,*AR7
    // asm 00000583: 	FIX	*+AR4(IR0),R0		;READ X1
    // asm 00000583:  ||	STI	R6,*AR7			;STUFF the PALETTE
    // asm 00000584: 	FIX	*+AR4(IR1),R0		;READ X1
    // asm 00000584:  ||	STI	R0,*AR7
    // asm 00000585: 	FIX	*+AR5(IR0),R0		;READ X2
    // asm 00000585:  ||	STI	R0,*AR7
    // asm 00000586: 	FIX	*+AR5(IR1),R0		;READ Y2
    // asm 00000586:  ||	STI	R0,*AR7
    // asm 00000587: 	FIX	*+AR2(IR0),R0		;READ X3
    // asm 00000587:  ||	STI	R0,*AR7
    // asm 00000588: 	FIX	*+AR2(IR1),R0		;READ Y3
    // asm 00000588:  ||	STI	R0,*AR7
    // asm 00000589: 	FIX	*+AR3(IR0),R0		;READ X4
    // asm 00000589:  ||	STI	R0,*AR7
    // asm 0000058A: 	FIX	*+AR3(IR1),R0		;READ Y4
    // asm 0000058A:  ||	STI	R0,*AR7
    // asm 0000058B:  	STI	R0,*AR7
    // asm 0000058C: 	LDI	*AR1++,R0
    // asm 0000058D: 	LDI	*AR1++,R1
    // asm 0000058E: 	LDI	*AR1++,R2
    // asm 0000058E:  ||	STI	R0,*AR7
    // asm 0000058F: 	LSH	-16,R0
    // asm 00000590: 	STI	R0,*AR7
    // asm 00000591:  	STI	R1,*AR7
    // asm 00000592: 	LSH	-16,R1
    // asm 00000593: 	STI	R1,*AR7
    // asm 00000594:  	STI	R2,*AR7
PLTGPOLL_2:
    // asm 00000595: 	LDI	@FIFO_INC,R0
    // asm 00000596: 	RETS
PLTGLP1_2:
    // asm 00000597: 	SUBI	1,RC
    // asm 00000598: 	LDI	RC,R0
    // asm 00000599: 	BNND	PLTGPOLYLP1_2
    // asm 0000059A: 	LDI	*++AR1(6),R3		;read internal vertices (v4|v3|v2|v1)
    // asm 0000059B: 	SUBI	1,AR1
    // asm 0000059C: 	AND	R7,R3,AR4
    // 	;----> BNND PLTGPOLYLP1_2
    // asm 0000059D: 	RETS

    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLT1PAL", 0, 0);
    UNIMPL();
}

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
static void PLOTILLUM(OBJ* obj, const ROM_ILLUM_POLYGON* polygons, int polygon_count_minus_one) {
    int polygon_index;

    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLOTILLUM", 0, 0);

    for (polygon_index = 0; polygon_index <= polygon_count_minus_one; polygon_index++, polygons++) {
        int packed_vertices;
        const c3x_f32_t* vertex1;
        const c3x_f32_t* vertex2;
        const c3x_f32_t* vertex3;
        const c3x_f32_t* vertex4;
        c3x_reg_t dx;
        c3x_reg_t dy;
        c3x_reg_t ex;
        c3x_reg_t ey;
        c3x_reg_t rotated_normal_x;
        c3x_reg_t rotated_normal_y;
        c3x_reg_t rotated_normal_z;
        c3x_reg_t illumination;
        int illumination_index;
        int control_word;
        c3x_reg_t normal_x;
        c3x_reg_t normal_y;
        c3x_reg_t normal_z;

        // asm 000005A7: 	LDI	*+AR1(4),R0		;read vertex (v4|v3|v2|v1)
        packed_vertices = (int)polygons->vertices_4_3_2_1;
        // asm 000005A8: 	LDI	R0,AR4
        // asm 000005A9: 	AND	0FFh,AR4		;v1
        // asm 000005AA: 	MPYI	3,AR4
        vertex1 = &BLOWLIST[(packed_vertices & 0xff) * 3];

        // asm 000005AB: 	LDI	R0,AR5
        // asm 000005AC: 	RS	8,AR5
        // asm 000005AD: 	AND	0FFh,AR5		;v2
        // asm 000005AE: 	MPYI	3,AR5
        vertex2 = &BLOWLIST[((packed_vertices >> 8) & 0xff) * 3];

        // asm 000005AF: 	LDI	R0,AR2
        // asm 000005B0: 	RS	16,AR2
        // asm 000005B1: 	AND	0FFh,AR2		;v3
        // asm 000005B2: 	MPYI	3,AR2
        vertex3 = &BLOWLIST[((packed_vertices >> 16) & 0xff) * 3];

        // asm: 	v4
        vertex4 = &BLOWLIST[((packed_vertices >> 24) & 0xff) * 3];

        // asm 000005B3: 	SUBF	*+AR4(IR0),*+AR5(IR0),R1	;dx = ax - bx
        dx = C3X_SUB(vertex1[0], vertex2[0]);
        // asm 000005B4: 	SUBF	*+AR4(IR1),*+AR5(IR1),R5	;dy = ay - by
        dy = C3X_SUB(vertex1[1], vertex2[1]);
        // asm 000005B5: 	SUBF	*+AR2(IR0),*+AR5(IR0),R0	;ex = cx - bx
        ex = C3X_SUB(vertex3[0], vertex2[0]);
        // asm 000005B6:   ||	SUBF	*+AR2(IR1),*+AR5(IR1),R2	;ey = cy - by
        ey = C3X_SUB(vertex3[1], vertex2[1]);
        // asm 000005B6: 	MPYF	R5,R0,R0			;ex = dy * ex
        // asm 000005B7: 	MPYF	R1,R2			;ey = dx * ey
        // asm 000005B8: 	SUBF	R0,R2			;ey = ey - ex
        // asm 000005B9: 	BGT	ZCLIP		;if back facing DONT PLOT
        if (C3X_GT(C3X_SUB(C3X_MUL(dx, ey), C3X_MUL(dy, ex)), C3X_FROM_INT(0))) {
            continue;
        }

        // asm 000005BA: 	LDI	*AR1++,R7			;get control word
        control_word = (int)polygons->cntl;
        // ;	LDP	@tmpmatY			;DP loaded with low memory area
        // asm 000005BB: 	LDI	@tmpmatY,AR3
        // asm 000005BC: 	LDI	@transmatrixI,AR5		;these are in same memory area
        // asm 000005BD: 	LDF	*AR1++,R3			;get the NORMAL.x
        normal_x = C3X_LOAD(polygons->nx);
        // asm 000005BE: 	LDF	*AR1++,R4			;	       .y
        normal_y = C3X_LOAD(polygons->ny);
        // asm 000005BE:  ||	STF	R3,*-AR3(1)
        // asm 000005BF: 	LDF	*AR1++,R5			;	       .z
        normal_z = C3X_LOAD(polygons->nz);
        // asm 000005BF:  ||	STF	R4,*AR3
        // asm 000005C0: 	NOP 	*AR5++(8)		   	;FAST ADD TO AR5
        // asm 000005C1: 	MPYF	*AR5--,R5,R0
        // asm 000005C2: 	MPYF	*AR5--,*AR3,R1
        // asm 000005C3: 	MPYF	*AR5--,*-AR3(1),R0
        // asm 000005C3:  ||	ADDF	R0,R1,R2
        // asm 000005C4: 	ADDF	R0,R2,R1
        rotated_normal_z = C3X_ADD(C3X_ADD(C3X_MUL(ROTATION_MATRIX.a20, normal_x), C3X_MUL(ROTATION_MATRIX.a21, normal_y)), C3X_MUL(ROTATION_MATRIX.a22, normal_z));

        // asm 000005C5: 	MPYF	*AR5--,R5,R0
        // asm 000005C6: 	MPYF	*AR5--,R4,R2
        // asm 000005C7: 	MPYF	*AR5--,*-AR3(1),R0
        // asm 000005C7:  ||	ADDF	R0,R2,R2
        // asm 000005C8: 	MPYF	*AR5--,*+AR3(1),R0
        // asm 000005C8:  ||	ADDF	R0,R2,R2
        rotated_normal_y = C3X_ADD(C3X_ADD(C3X_MUL(ROTATION_MATRIX.a10, normal_x), C3X_MUL(ROTATION_MATRIX.a11, normal_y)), C3X_MUL(ROTATION_MATRIX.a12, normal_z));

        // asm 000005C9: 	MPYF	*AR5--,R4,R3
        // asm 000005CA: 	MPYF	*AR5--,*-AR3(1),R0
        // asm 000005CA:  ||	ADDF	R0,R3,R3
        // asm 000005CB: 	ADDF	R0,R3,R3
        rotated_normal_x = C3X_ADD(C3X_ADD(C3X_MUL(ROTATION_MATRIX.a00, normal_x), C3X_MUL(ROTATION_MATRIX.a01, normal_y)), C3X_MUL(ROTATION_MATRIX.a02, normal_z));

        // asm 000005CD: 	MPYF	*-AR5(1),R3,R3
        // asm 000005CE: 	MPYF	*AR5,R2,R2
        // asm 000005CF: 	MPYF	*+AR5(1),R1,R1
        // asm 000005D0: 	ADDF	R2,R1,R5
        // asm 000005D1: 	ADDF	R3,R5
        illumination = C3X_ADD(C3X_ADD(C3X_MUL(_LIGHT.X, rotated_normal_x), C3X_MUL(_LIGHT.Y, rotated_normal_y)), C3X_MUL(_LIGHT.Z, rotated_normal_z));

        // asm 000005D2: 	MPYF	-8,R5
        // asm 000005D3: 	ADDF	8,R5
        // asm 000005D4: 	FIX	R5
        illumination_index = FIX(C3X_ADD(C3X_ADD(C3X_MUL(illumination, C3X_FROM_INT(-8)), C3X_FROM_INT(8)), C3X_IMM_F32(0.5f)));
        if (illumination_index < 0) {
            illumination_index = 0;
        } else if (illumination_index > 15) {
            illumination_index = 15;
        }
        // asm 000005D5: 	LDI	FASTCC,R7
        // asm 000005D6: 	OR	R5,R7
        control_word = FASTCC | illumination_index;

        // asm 000005D7: 	LDI	200h,R5		;second palette
        // asm 000005DB: 	STI	R7,*AR7
        // asm 000005DC: 	STI	R5,*AR7
        port_output_fpga(
            FIX(vertex1[0]),
            FIX(vertex1[1]),
            FIX(vertex2[0]),
            FIX(vertex2[1]),
            FIX(vertex3[0]),
            FIX(vertex3[1]),
            FIX(vertex4[0]),
            FIX(vertex4[1]),
            0,
            0,
            0,
            0,
            0,
            0x200,
            control_word);
    }

    (void)obj;
}
// *warning moving this to top of file will crash program ask ti why

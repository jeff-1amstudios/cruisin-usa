
#include "attrdrne.h"
#include "c30.h"
#include "cmos.h"
#include "delta.h"
#include "dirq.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "objects.h"
#include "pall.h"
#include "port.h"
#include "racer.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "validator.h"
#include "vunit.h"

#include <math.h>
#include <string.h>

/*
 * Source module: asm/ATTRDRNE.ASM
 */

typedef struct AttractViewStep AttractViewStep;

extern MATRIX _MATRIXA;
extern VECTOR _VECTORA;
extern VECTOR _VECTORB;

void LOGO_PROC(PROC* p);
static const AttractViewStep* GET_LIST_ADDR(PROC* p /*AR7*/);
static void INIT_STARTING(void);
static void INIT_WATCH(void);
static void INIT_REVERS_CUP(void);
static void INIT_CATCHUP(void);
static void INIT_LEAD(void);
static void INITROAD_VIEW(int direction_to_search /*R0*/, int distance_to_start_with /*R1*/, int position_to_follow /*R2*/,
                          c3x_reg_t rady_offset /*R3*/);
static void WATCH_VIEW(void);
static void REV_ROAD_VIEW(void);
static void ROAD_VIEW(void);
static void LEAD_VIEW(void);
static void MOVE_ROAD_CAMERA(void);
static void NEXT_ROAD(void);
static void GETCAMDIR(void);
static void SET_LANE(c3x_reg_t smoothing /*R2*/);
static void INIT_LANE(void);
static void DO_LANE_POS(c3x_reg_t lane_offset /*R3*/);
static c3x_reg_t GET_ROAD_RADY(LEG_PAYLOAD* road_obj /*AR4*/);
static OBJ* FIND_RACER(int position_to_find /*R2*/);
static c3x_reg_t FIND_CLOSEST_RACER(OBJ** closest_racer /*AR1*/);
static OBJ* FIND_TRACK(uint32_t road_id /*R2*/);
static LEG_PAYLOAD* GET_MAP_CARBLK(OBJ* car_obj /*AR0*/);
static LEG_PAYLOAD* FIND_MAP(uint32_t road_id /*R2*/);
static void INITVIEW1_VIEW(void);
static void CUT_TO_VIEW2(void);
static void INITVIEW2_VIEW(void);
static void INITVIEW3_VIEW(void);
static void FACE_FRONT(void);
static c3x_reg_t SMOOTH_VECTOR(c3x_reg_t radians /*R0*/, c3x_reg_t smoothing /*R2*/);
static void SERIOUSLY_NORMALIZE(void);
static void SET_SMOOTH_VIEW(void);
static void SMOOTH_VIEW(void);
static void ZOOM_CAMERA(void);
static void UPDATE_CAMERA(void);
static void CAMERA_HORIZON_PROJECTION(void);
static c3x_reg_t NORM_VECTOR(c3x_reg_t radians /*R0*/);
static void INIT_ATTR_LEG(PROC* p /*AR7*/);
static void ATTR_INIT_GAMELEG(void);
void LOAD_ATTR_LEG(void);

struct AttractViewStep {
    void_func_ptr init_func;
    void_func_ptr run_func;

    union {
        struct {
            int32_t stop_when_zero;
        } generic;
        struct {
            int32_t sections;
        } road;
        struct {
            int32_t target_track_id;
        } smooth;
        struct {
            int32_t frames;
            uint32_t track_id;
        } watch;
    } state;
};

static const AttractViewStep* const VIEWLIST[];
static int ATTR_WAVETAB[16];
static const AttractViewStep GGPARK_LIST[];
static const AttractViewStep BEVHILL_LIST[];
static const AttractViewStep GCANYON_LIST[];
static const AttractViewStep CHICAGO_LIST[];

/* asm: ATTRWAVE	pbss	ATTRWAVE,1 */
int ATTRWAVE;
/*
 *----------------------------------------------------------------------------
 * CAMERA variable space
 *
 */

#define CUT_PAN (PDATA + 1)
#define ZOOMVEL (PDATA + 2)
#define CAMERA_XYZR (PDATA + 3)    // 4 long
#define NEWCAMERA_XYZR (PDATA + 7) // 4 long
#define NEW_POSX (PDATA + 11)
#define NEW_POSY (PDATA + 12)
#define NEW_POSZ (PDATA + 13)
#define NEW_RADX (PDATA + 14)
#define NEW_RADY (PDATA + 15)
#define NEW_RADZ (PDATA + 16)
#define NEW_MATRIX (PDATA + 17) // 9 long
#define CAMERA_VIEW (PDATA + 26)
#define CAMERA_VEL (PDATA + 27)
#define CAROBJ (PDATA + 28)
#define CARDIS (PDATA + 29)
#define CAMERA_ACCEL (PDATA + 30)
#define CAMERA_INFIN (PDATA + 31)
#define TEMP_DATA (PDATA + 32) // 10 long
#define LIST_NUM (PDATA + 42)
#define CAMERA_LANE (PDATA + 43)
#define OBJINS (PDATA + 44)
#define CAMYOFF (PDATA + 45)

void ATTRACT_DELTA(PROC* p /*AR7*/) {
    const AttractViewStep* view_script;
    const AttractViewStep* step;
    OBJ* player_obj;
    PROC* player_proc;

    switch (p->resume_state) {
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

    // asm 000055C3: 	LDI	0,R0
    // asm 000055C4: 	STI	R0,*+AR7(OBJINS)
    p->ctx->ATTRACT_DELTA.objins = 0;
    p->ctx->ATTRACT_DELTA.cardis = C3X_STF(C3X_IMM_F32(1.0f));

    // asm 000055C5: 	LDI	@ATTRWAVE,R4
    // asm 000055C6: 	LSH	-1,R4
    // asm 000055C7: 	STI	R4,*+AR7(LIST_NUM)
    p->ctx->ATTRACT_DELTA.list_num = ATTRWAVE >> 1;
    MAME_ASSERT_REG(0x000055C8, "R4", &p->ctx->ATTRACT_DELTA.list_num);

    // asm 000055C8: 	CALL	INIT_ATTR_LEG
    INIT_ATTR_LEG(p);

    // asm 000055C9: 	LDI	4,R0
    // asm 000055CA: 	STI	R0,@NOSWAP
    NOSWAP = 4;
    MAME_ASSERT_REG(0x000055CB, "R0", &NOSWAP);

    // asm 000055CB: 	CREATE	LOGO_PROC,UTIL_C
    {
        PROC_CONTEXT* ctx = port_malloc(sizeof(PROC_CONTEXT));
        CREATE(LOGO_PROC, UTIL_C, ctx);
    }

    // asm 000055CE: 	LDI	@_MODE,R0
    // asm 000055CF: 	OR	MGO,R0
    // asm 000055D0: 	STI	R0,@_MODE
    _MODE |= MGO;
    MAME_ASSERT_REG(0x000055D1, "R0", &_MODE);

    // asm 000055D1: 	LDI	-1,R0
    // asm 000055D2: 	STI	R0,*+AR7(CUT_PAN)
    p->ctx->ATTRACT_DELTA.cut_pan = -1;

    // asm 000055D3: 	LDI	1,R0	    		;SET GAME FRAME RATE
    // asm 000055D4: 	STI	R0,@FRAMRATE
    // asm 000055D5: 	STI	R0,@TIMECLR
    // asm 000055D6: 	STI	R0,@DRONE_DISPATCH_P
    // SET GAME FRAME RATE
    FRAMRATE = 1;
    TIMECLR = 1;
    DRONE_DISPATCH_P = 1;

    // asm 000055D7: 	SLEEP	1
    SLEEP(1, 1);
    // asm 000055D9: 	LDI	@PLYCAR,AR4
    // asm 000055DA: 	LDI	*+AR4(PAR5),AR5
    // asm 000055DB: 	LDI	*+AR4(PAR4),AR4
    // asm 000055DC: 	LDI	*+AR5(PAR5),AR0
    // asm 000055DD: 	STI	AR4,@PLYCAR
    // asm 000055DE: 	STI	AR0,@PLYCBLK
    player_proc = (PROC*)PLYCAR;
    player_obj = NULL;
    if (player_proc != NULL && player_proc->ctx != NULL) {
        player_obj = player_proc->ctx->RACER_DRONE.obj;
        PLYCBLK = player_proc->ctx->RACER_DRONE.carblk;
    }
    if (player_obj != NULL) {
        PLYCAR = player_obj;
    }

    // asm 000055DF: 	LDI	90,R0
    // asm 000055E0: 	STI	R0,@_countdown
    _countdown = 90;
    MAME_ASSERT_REG(0x000055E1, "R0", &_countdown);

    // asm 000055E1: 	LDI	1,R0
    // asm 000055E2: 	STI	R0,*+AR7(CUT_PAN)
    p->ctx->ATTRACT_DELTA.cut_pan = 1;

    // asm 000055E3: 	CALL	GET_LIST_ADDR
    view_script = GET_LIST_ADDR(p);
    p->ctx->ATTRACT_DELTA.view_script = (uintptr_t)view_script;

    // asm 000055E4: 	BR	DELTA_LOOP_ENTRY
    goto DELTA_LOOP_ENTRY;

aDELTA_LOOP:
    // asm 000055E5: 	LDI	*AR6,AR0
    // asm 000055E6: 	CALLU	AR0
    view_script = (const AttractViewStep*)p->ctx->ATTRACT_DELTA.view_script;
    step = view_script;
    step->run_func();

ADELTA2:
    // asm 000055E7: 	CALL	UPDATE_CAMERA
    UPDATE_CAMERA();

    // asm 000055E8: 	SLEEP	1
    SLEEP(1, 2);
    view_script = (const AttractViewStep*)p->ctx->ATTRACT_DELTA.view_script;

    // asm 000055EA: 	LDI	@NOSWAP,R0
    // asm 000055EB: 	BEQ	ADELTA2A
    // asm 000055EC: 	SUBI	1,R0		;OK Now we have a valid frame
    // asm 000055ED: 	STI	R0,@NOSWAP
    if (NOSWAP != 0) {
        // OK Now we have a valid frame
        NOSWAP -= 1;
    }

ADELTA2A:
    // asm 000055EE: 	CMPI	0,AR5
    // asm 000055EF: 	BGT	aDELTA_LOOP
    MAME_ASSERT_REG(0x000055EF, "AR5", &p->ctx->ATTRACT_DELTA.frames_left);
    if (p->ctx->ATTRACT_DELTA.frames_left > 0) {
        goto aDELTA_LOOP;
    }

    // asm 000055F0: 	ADDI	1,AR6		;Advance to next mode
    view_script += 1; // Advance to next mode

DELTA_LOOP_ENTRY:
    // asm 000055F1: 	LDI	*AR6,AR0
    // asm 000055F2: 	CMPI	0,AR0
    // asm 000055F3: 	BNE	sDELTA_LOOP
    // ;	CALL	GET_LIST_ADDR
    // ;	BU	$
    // asm 000055F4: 	SUBI	3,AR6		;Loop on last entry
    if (view_script->init_func == NULL) {
        view_script -= 1; // Loop on last entry
    }

sDELTA_LOOP:
    // asm 000055F5: 	LDI	*+AR7(OBJINS),R0
    // asm 000055F6: 	BEQ	NO_OBJINS
    // asm 000055F7: 	LDI	R0,AR2
    // asm 000055F8: 	CALL	OBJ_INSERT			;INSERT PLAYER OBJECT
    // asm 000055F9: 	LDI	0,R0
    // asm 000055FA: 	STI	R0,*+AR7(OBJINS)
    if (p->ctx->ATTRACT_DELTA.objins != 0) {
        OBJ_INSERT(p->ctx->ATTRACT_DELTA.objins); // INSERT PLAYER OBJECT
        p->ctx->ATTRACT_DELTA.objins = 0;
    }

NO_OBJINS:
    // asm 000055FB: 	LDI	*AR6++,AR0
    // asm 000055FC: 	CALLU	AR0
    step = view_script;
    p->ctx->ATTRACT_DELTA.view_script = (uintptr_t)step;
    step->init_func();

    // asm 000055FD: 	LDI	*AR6++,AR5
    p->ctx->ATTRACT_DELTA.frames_left = step->state.generic.stop_when_zero;
    MAME_ASSERT_REG(0x000055FE, "AR5", &p->ctx->ATTRACT_DELTA.frames_left);
    p->ctx->ATTRACT_DELTA.view_script = (uintptr_t)step;

    // asm 000055FE: 	B	ADELTA2
    goto ADELTA2;
}

/*
 *----------------------------------------------------------------------------
 *----------------------------------------------------------------------------
 *CREATE THE OBJECT 'CRUISIN USA' FOR OVERLAY DURING ATTRACT MODE
 *
 */
#define LOGO_STARTZ 10000
#define LOGO_ENDX (-120)
#define LOGO_ENDY (-85)
#define LOGO_ENDZ 368
#define LOGO_FLYIN_FRAMES 125
#define LOGO_WHOOSH_FRAMES 462
#define LOGO_SPINZ 18.85 // 3 revolutions

void LOGO_PROC(PROC* p) {
    OBJ* obj;
    c3x_reg_t nframes;
    c3x_reg_t value;

    switch (p->resume_state) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 000055FF: 	CALL	OBJ_GET
    obj = OBJ_GET();

    // asm 00005600: 	BC	LOGOX
    if (obj == NULL) {
        goto LOGOX;
    }

    // asm 00005601: 	LDI	AR0,AR4
    p->ctx->LOGO_PROC.obj = obj;

    // asm 00005602: 	LDIL	logo,R0
    // asm 00005605: 	STI	R0,*+AR0(OROMDATA)
    obj->romdata = ROM_PTR(logo_ROM);

    // asm 00005606: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm 00005607: 	OR	*+AR0(OFLAGS),R0
    // asm 00005608: 	STI	R0,*+AR0(OFLAGS)
    obj->flags |= O_IROT | O_NOROT | O_NOUROT | O_NOUNIV;

    // asm 00005609: 	LDI	UTIL_C|LOGO_T,R0
    // asm 0000560A: 	STI	R0,*+AR0(OID)
    obj->id = UTIL_C | LOGO_T;

    // asm 0000560B: 	FLOAT	LOGO_STARTZ,R0
    // asm 0000560C: 	STF	R0,*+AR0(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_FROM_INT(LOGO_STARTZ));

    // asm 0000560D: 	LDI	AR0,AR2
    // asm 0000560E: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);

    // asm 0000560F: 	LDF	LOGO_SPINZ,R0
    // asm 00005610: 	STF	R0,*+AR4(ORADX)
    obj->rad.X = C3X_STF(C3X_IMM_F32(LOGO_SPINZ));

    // asm 00005611: 	LDF	0,R7		;SPEED FACTOR
    p->ctx->LOGO_PROC.speed_factor = C3X_STF(C3X_FROM_INT(0)); // SPEED FACTOR

    // asm 00005612: 	LDI	LOGO_FLYIN_FRAMES,AR5
    p->ctx->LOGO_PROC.frames_left = LOGO_FLYIN_FRAMES;

LOGO_LOOP1:
    // asm 00005613: 	SLEEP	1
    SLEEP(1, 1);

    obj = p->ctx->LOGO_PROC.obj;

    // asm 00005615: 	FLOAT	@NFRAMES,R6
    nframes = C3X_FROM_INT(NFRAMES);

    // asm 00005616: 	FLOAT	LOGO_ENDY,R0
    // asm 00005617: 	CMPF	*+AR4(OPOSY),R0
    // asm 00005618: 	BEQ	LOGO2
    if (C3X_NE(obj->pos.Y, C3X_FROM_INT(LOGO_ENDY))) {
        // asm 00005619: 	FLOAT	LOGO_ENDX,R0
        // asm 0000561A: 	SUBF	*+AR4(OPOSX),R0
        // asm 0000561B: 	MPYF	R7,R0
        // asm 0000561C: 	MPYF	R6,R0
        // asm 0000561D: 	ADDF	*+AR4(OPOSX),R0
        // asm 0000561E: 	FLOAT	LOGO_ENDX,R1
        // asm 0000561F: 	CMPF	R1,R0
        // asm 00005620: 	LDFLT	R1,R0
        // asm 00005621: 	STF	R0,*+AR4(OPOSX)
        value = C3X_MUL(C3X_SUB(C3X_FROM_INT(LOGO_ENDX), obj->pos.X), p->ctx->LOGO_PROC.speed_factor);
        value = C3X_ADD(C3X_MUL(value, nframes), obj->pos.X);
        if (C3X_LT(value, C3X_FROM_INT(LOGO_ENDX))) {
            value = C3X_FROM_INT(LOGO_ENDX);
        }
        obj->pos.X = C3X_STF(value);

        // asm 00005622: 	FLOAT	LOGO_ENDY,R0
        // asm 00005623: 	SUBF	*+AR4(OPOSY),R0
        // asm 00005624: 	MPYF	R7,R0
        // asm 00005625: 	MPYF	R6,R0
        // asm 00005626: 	ADDF	*+AR4(OPOSY),R0
        // asm 00005627: 	FLOAT	LOGO_ENDY,R1
        // asm 00005628: 	CMPF	R1,R0
        // asm 00005629: 	LDFLT	R1,R0
        // asm 0000562A: 	STF	R0,*+AR4(OPOSY)
        value = C3X_MUL(C3X_SUB(C3X_FROM_INT(LOGO_ENDY), obj->pos.Y), p->ctx->LOGO_PROC.speed_factor);
        value = C3X_ADD(C3X_MUL(value, nframes), obj->pos.Y);
        if (C3X_LT(value, C3X_FROM_INT(LOGO_ENDY))) {
            value = C3X_FROM_INT(LOGO_ENDY);
        }
        obj->pos.Y = C3X_STF(value);

        // asm 0000562B: 	ADDF	0.03,R7
        p->ctx->LOGO_PROC.speed_factor = C3X_STF(C3X_ADD(p->ctx->LOGO_PROC.speed_factor, C3X_IMM_F32(0.03)));
    }

LOGO2:
    // asm 0000562C: 	FLOAT	LOGO_ENDZ-LOGO_STARTZ,R0
    // asm 0000562D: 	FLOAT	LOGO_FLYIN_FRAMES,R1
    // asm 0000562E: 	CALL	DIV_F
    // asm 0000562F: 	MPYF	R6,R0
    // asm 00005630: 	LDF	*+AR4(OPOSZ),R1
    // asm 00005631: 	ADDF	R1,R0
    // asm 00005632: 	FLOAT	LOGO_ENDZ,R1
    // asm 00005633: 	CMPF	R1,R0
    // asm 00005634: 	LDFLT	R1,R0
    // asm 00005635: 	STF	R0,*+AR4(OPOSZ)
    value = DIV_F(C3X_FROM_INT(LOGO_ENDZ - LOGO_STARTZ), C3X_FROM_INT(LOGO_FLYIN_FRAMES));
    value = C3X_ADD(C3X_MUL(value, nframes), obj->pos.Z);
    if (C3X_LT(value, C3X_FROM_INT(LOGO_ENDZ))) {
        value = C3X_FROM_INT(LOGO_ENDZ);
    }
    obj->pos.Z = C3X_STF(value);

    // asm 00005636: 	LDF	LOGO_SPINZ,R0
    // asm 00005637: 	FLOAT	LOGO_FLYIN_FRAMES,R1
    // asm 00005638: 	CALL	DIV_F
    // asm 00005639: 	MPYF	R6,R0
    // asm 0000563A: 	LDF	*+AR4(ORADX),R2
    // asm 0000563B: 	SUBF	R0,R2
    // asm 0000563C: 	LDFN	0,R2
    // asm 0000563D: 	STF	R2,*+AR4(ORADX)
    value = C3X_MUL(DIV_F(C3X_IMM_F32(LOGO_SPINZ), C3X_FROM_INT(LOGO_FLYIN_FRAMES)), nframes);
    obj->rad.X = C3X_STF(C3X_SUB(obj->rad.X, value));
    if (C3X_GT(obj->rad.X, C3X_FROM_INT(0))) {
        obj->rad.X = C3X_STF(C3X_FROM_INT(0));
    }

    // asm 0000563E: 	LDI	AR4,AR2
    // asm 0000563F: 	ADDI	OMATRIX,AR2
    // asm 00005640: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&obj->omatrix, C3X_LDF(obj->rad.X));

    // asm 00005641: 	SUBI	@NFRAMES,AR5
    p->ctx->LOGO_PROC.frames_left -= NFRAMES;

    // asm 00005642: 	CMPI	0,AR5
    // asm 00005643: 	BP	LOGO_LOOP1
    if (p->ctx->LOGO_PROC.frames_left > 0) {
        goto LOGO_LOOP1;
    }

LOGOX:
    // asm 00005644: 	DIE
    DIE();
}

// *----------------------------------------------------------------------------
static const AttractViewStep* GET_LIST_ADDR(PROC* p /*AR7*/) {
    // asm 00005645: 	LDI	*+AR7(LIST_NUM),AR6
    // asm 00005646: 	ADDI	@VIEWLISTI,AR6
    // asm 00005647: 	LDI	*AR6,AR6
    const AttractViewStep* view_script = VIEWLIST[p->ctx->ATTRACT_DELTA.list_num];

    // asm 00005648: 	RETS
    return view_script;
}

// *----------------------------------------------------------------------------

/* asm: VIEWLIST */
/* asm: 	.word	GGPARK_LIST */
/* asm: 	.word	BEVHILL_LIST */
/* asm: 	.word	GCANYON_LIST */
/* asm: 	.word	CHICAGO_LIST */
/* asm: 	.word	GGPARK_LIST */
/* asm: 	.word	BEVHILL_LIST */
/* asm: 	.word	GCANYON_LIST */
/* asm: 	.word	CHICAGO_LIST */
/* asm: 	 */
static const AttractViewStep* const VIEWLIST[] = {
    GGPARK_LIST,
    BEVHILL_LIST,
    GCANYON_LIST,
    CHICAGO_LIST,
    GGPARK_LIST,
    BEVHILL_LIST,
    GCANYON_LIST,
    CHICAGO_LIST,
};
/* asm: GGPARK_LIST */
/* asm: 	.word	INIT_STARTING,70,ROAD_VIEW */
/* asm: 	.word	INIT_LEAD,80,LEAD_VIEW */
/* asm: 	.word	INIT_WATCH,1214h,240,WATCH_VIEW */
/* asm: 	.word	CUT_TO_VIEW2,1404h,SMOOTH_VIEW */
/* asm: 	.word	INITVIEW1_VIEW,17FBh,SMOOTH_VIEW */
/* asm: 	.word	INIT_LEAD,80,LEAD_VIEW */
/* asm: 	.word	0,0 */
/* asm: 	 */
static const AttractViewStep GGPARK_LIST[] = {
    { .init_func = INIT_STARTING, .run_func = ROAD_VIEW, .state.road = { .sections = 70 } },
    { .init_func = INIT_LEAD, .run_func = LEAD_VIEW, .state.road = { .sections = 80 } },
    { .init_func = INIT_WATCH, .run_func = WATCH_VIEW, .state.watch = { .frames = 240, .track_id = 0x1214 } },
    { .init_func = CUT_TO_VIEW2, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x1404 } },
    { .init_func = INITVIEW1_VIEW, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x17FB } },
    { .init_func = INIT_LEAD, .run_func = LEAD_VIEW, .state.road = { .sections = 80 } },
    { 0 },
};
/* asm: BEVHILL_LIST */
/* asm: 	.word	INIT_STARTING,80,ROAD_VIEW */
/* asm: 	.word	CUT_TO_VIEW2,14AFBh,SMOOTH_VIEW */
/* asm: 	.word	INITVIEW1_VIEW,14DF3h,SMOOTH_VIEW */
/* asm: 	.word	INIT_STARTING,100,ROAD_VIEW */
/* asm: 	.word	0,0 */
/* asm: 	 */
/* asm: 	 */
static const AttractViewStep BEVHILL_LIST[] = {
    { .init_func = INIT_STARTING, .run_func = ROAD_VIEW, .state.road = { .sections = 80 } },
    { .init_func = CUT_TO_VIEW2, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x14AFB } },
    { .init_func = INITVIEW1_VIEW, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x14DF3 } },
    { .init_func = INIT_STARTING, .run_func = ROAD_VIEW, .state.road = { .sections = 100 } },
    { 0 },
};
/* asm: GCANYON_LIST */
/* asm: 	.word	INIT_STARTING,70,ROAD_VIEW */
/* asm: 	.word	INIT_WATCH,2E20Ah,220,WATCH_VIEW */
/* asm: 	.word	CUT_TO_VIEW2,2E800h,SMOOTH_VIEW */
/* asm: 	.word	INITVIEW1_VIEW,2EC00h,SMOOTH_VIEW */
/* asm: 	.word	INIT_WATCH,2EF00h,240,WATCH_VIEW */
/* asm: 	.word	CUT_TO_VIEW2,30000h,SMOOTH_VIEW */
/* asm: 	.word	0,0 */
/* asm: 	 */
static const AttractViewStep GCANYON_LIST[] = {
    { .init_func = INIT_STARTING, .run_func = ROAD_VIEW, .state.road = { .sections = 70 } },
    { .init_func = INIT_WATCH, .run_func = WATCH_VIEW, .state.watch = { .frames = 220, .track_id = 0x2E20A } },
    { .init_func = CUT_TO_VIEW2, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x2E800 } },
    { .init_func = INITVIEW1_VIEW, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x2EC00 } },
    { .init_func = INIT_WATCH, .run_func = WATCH_VIEW, .state.watch = { .frames = 240, .track_id = 0x2EF00 } },
    { .init_func = CUT_TO_VIEW2, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x30000 } },
    { 0 },
};
/* asm: CHICAGO_LIST */
/* asm: 	.word	INIT_STARTING,80,ROAD_VIEW */
/* asm: 	.word	INIT_REVERS_CUP,60,REV_ROAD_VIEW */
/* asm: 	.word	CUT_TO_VIEW2,3AA0Eh,SMOOTH_VIEW */
/* asm: 	.word	INITVIEW1_VIEW,3C00Ah,SMOOTH_VIEW */
/* asm: 	.word	INIT_WATCH,3C5F5h,240,WATCH_VIEW */
/* asm: 	.word	INIT_LEAD,80,LEAD_VIEW */
/* asm: 	.word	0,0 */
/* asm: 	 */
static const AttractViewStep CHICAGO_LIST[] = {
    { .init_func = INIT_STARTING, .run_func = ROAD_VIEW, .state.road = { .sections = 80 } },
    { .init_func = INIT_REVERS_CUP, .run_func = REV_ROAD_VIEW, .state.road = { .sections = 60 } },
    { .init_func = CUT_TO_VIEW2, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x3AA0E } },
    { .init_func = INITVIEW1_VIEW, .run_func = SMOOTH_VIEW, .state.smooth = { .target_track_id = 0x3C00A } },
    { .init_func = INIT_WATCH, .run_func = WATCH_VIEW, .state.watch = { .frames = 240, .track_id = 0x3C5F5 } },
    { .init_func = INIT_LEAD, .run_func = LEAD_VIEW, .state.road = { .sections = 80 } },
    { 0 },
};

// *----------------------------------------------------------------------------
static void INIT_STARTING(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;

    // asm 00005649: 	LDI	-350,R0
    // asm 0000564A: 	FLOAT	R0,R1
    // asm 0000564B: 	STF	R1,*+AR7(CAMYOFF)
    ctx->ATTRACT_DELTA.camyoff = C3X_STF(C3X_FROM_INT(-350));
    // asm 0000564C: 	LDI	4,R0		;direction to search in
    // asm 0000564D: 	LDI	1,R1		;distance to start with
    // asm 0000564E: 	LDI	8,R2		;Position to follow
    // asm 0000564F: 	LDF	0,R3		;RADY OFFSET
    // asm 00005650: 	CALL	INITROAD_VIEW
    INITROAD_VIEW(4, 1, 8, C3X_FROM_INT(0)); // ;direction to search in / ;distance to start with / ;Position to follow / ;RADY OFFSET
    // asm 00005651: 	LDF	1,R2
    // asm 00005652: 	CALL	SET_LANE
    SET_LANE(C3X_IMM_F32(1.0f));
    // asm 00005653: 	CALL	INIT_LANE
    INIT_LANE();
    // asm 00005654: 	CALL	ROAD_VIEW
    ROAD_VIEW();
    // asm 00005655: 	CALL	UPDATE_CAMERA
    UPDATE_CAMERA();
    // asm 00005656: 	CALL	RESCAN
    RESCAN();
    // asm 00005657: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_STARTING", 0, 0);
}

// *----------------------------------------------------------------------------
static void INIT_WATCH(void) {
    // asm 00005658: 	LDI	1,R0
    // asm 00005659: 	STI	R0,*+AR7(CUT_PAN)
    // asm 0000565A: 	LDF	0,R0
    // asm 0000565B: 	STF	R0,*+AR7(CAMERA_XYZR+3)		;Reset the offset
    // asm 0000565C: 	LDI	*AR6++,R2				;READ the track ID from the VIEWLIST
    // asm 0000565D: 	CALL	FIND_MAP
INIW1:
    // asm 0000565E: 	LDI	*+AR4(3),R2		;Get ID
    // asm 0000565F: 	CALL	FIND_TRACK		;MAKE DAMN SURE WE ARE ON A LOADED TRACK SECTION!!!
    // asm 00005660: 	CMPI	0,AR2
    // asm 00005661: 	BNE	INIW2
    // asm 00005662: 	SUBI	4,AR4
    // asm 00005663: 	BR	INIW1
INIW2:
    // asm 00005664: 	PUSH	AR4
    // asm 00005665: 	LDI	4,R2
    // asm 00005666: 	CALL	FIND_RACER
    // asm 00005667: 	STI	AR4,*+AR7(CAROBJ)
    // asm 00005668: 	STI	AR4,@PLYCAR
    // asm 00005669: 	LDI	*+AR4(OCARBLK),AR0
    // asm 0000566A: 	STI	AR0,@PLYCBLK
    // asm 0000566B: 	POP	AR4
    // asm 0000566C: 	LDI	-300,R0
    // asm 0000566D: 	FLOAT	R0,R1
    // asm 0000566E: 	STF	R1,*+AR7(CAMYOFF)
    // asm 0000566F: 	ADDI	*+AR4(Y),R0
    // asm 00005670: 	FLOAT	R0
    // asm 00005671: 	STF	R0,*+AR7(NEW_POSY)
    // asm 00005672: 	FLOAT	*+AR4(X),R0
    // asm 00005673: 	STF	R0,*+AR7(NEW_POSX)
    // asm 00005674: 	FLOAT	*+AR4(Z),R0
    // asm 00005675: 	STF	R0,*+AR7(NEW_POSZ)
    // asm 00005676: 	LDI	AR4,AR2
    // asm 00005677: 	ADDI	-4,AR2
    // asm 00005678: 	LDI	*+AR2(X),R0
    // asm 00005679: 	SUBI	*+AR4(X),R0
    // asm 0000567A: 	FLOAT	R0
    // asm 0000567B: 	MPYF	R0,R0
    // asm 0000567C: 	LDI	*+AR2(Z),R1
    // asm 0000567D: 	SUBI	*+AR4(Z),R1
    // asm 0000567E: 	FLOAT	R1
    // asm 0000567F: 	MPYF	R1,R1
    // asm 00005680: 	ADDF	R0,R1,R2
    // asm 00005681: 	CALL	SQRT
    // asm 00005682: 	LDF	R0,R2
    // asm 00005683: 	LDI	*+AR2(Y),R3
    // asm 00005684: 	SUBI	*+AR4(Y),R3
    // asm 00005685: 	FLOAT	R3
    // asm 00005686: 	MPYF	1.8,R3
    // asm 00005687: 	CALL	ARCTANF
    // asm 00005688: 	CALL	NORM_VECTOR
    // asm 00005689: 	NEGF	R0
    // asm 0000568A: 	ADDF	0.09,R0		;Offset for VIEWX
    // asm 0000568B: 	STF	R0,*+AR7(NEW_RADX)
    // asm 0000568C: 	FLOAT	-2500,R3
    // asm 0000568D: 	CALL	DO_LANE_POS
    // asm 0000568E: 	LDF	-100,R6			;SMALL number for a rad
    // asm 0000568F: 	CALL	WATCH_VIEW
    // asm 00005690: 	LDF	*+AR7(NEW_RADY),R6	;Save initial value
    // asm 00005691: 	CALL	UPDATE_CAMERA
    // asm 00005692: 	PUSHF	R6
    // asm 00005693: 	CALL	RESCAN			;This hoses all RX registers
    // asm 00005694: 	POPF	R6
    // asm 00005695: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_WATCH", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
static void INIT_REVERS_CUP(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;

    // asm 00005696: 	LDI	-350,R0
    // asm 00005697: 	FLOAT	R0,R1
    // asm 00005698: 	STF	R1,*+AR7(CAMYOFF)
    ctx->ATTRACT_DELTA.camyoff = C3X_STF(C3X_FROM_INT(-350));
    // asm 00005699: 	LDI	4,R0		;direction to search in
    // asm 0000569A: 	LDI	1,R1		;distance to start with
    // asm 0000569B: 	LDI	5,R2		;Position to follow
    // asm 0000569C: 	LDF	PI,R3
    // asm 0000569D: 	CALL	INITROAD_VIEW
    INITROAD_VIEW(4, 1, 5, C3X_IMM_F32(PI)); // ;direction to search in / ;distance to start with / ;Position to follow
    // asm 0000569E: 	LDF	1,R2
    // asm 0000569F: 	CALL	SET_LANE
    SET_LANE(C3X_IMM_F32(1.0f));
    // asm 000056A0: 	CALL	INIT_LANE
    INIT_LANE();
    // asm 000056A1: 	CALL	REV_ROAD_VIEW
    REV_ROAD_VIEW();
    // asm 000056A2: 	CALL	UPDATE_CAMERA
    UPDATE_CAMERA();
    // asm 000056A3: 	CALL	RESCAN
    RESCAN();
    // asm 000056A4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_REVERS_CUP", 0, 0);
}

// *----------------------------------------------------------------------------
static void INIT_CATCHUP(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;

    // asm 000056A5: 	LDI	-350,R0
    // asm 000056A6: 	FLOAT	R0,R1
    // asm 000056A7: 	STF	R1,*+AR7(CAMYOFF)
    ctx->ATTRACT_DELTA.camyoff = C3X_STF(C3X_FROM_INT(-350));
    // asm 000056A8: 	LDI	4,R0		;direction to search in
    // asm 000056A9: 	LDI	1,R1		;distance to start with
    // asm 000056AA: 	LDI	5,R2		;Position to follow
    // asm 000056AB: 	LDF	0,R3
    // asm 000056AC: 	CALL	INITROAD_VIEW
    INITROAD_VIEW(4, 1, 5, C3X_FROM_INT(0)); // ;direction to search in / ;distance to start with / ;Position to follow
    // asm 000056AD: 	LDF	1,R2
    // asm 000056AE: 	CALL	SET_LANE
    SET_LANE(C3X_IMM_F32(1.0f));
    // asm 000056AF: 	CALL	INIT_LANE
    INIT_LANE();
    // asm 000056B0: 	CALL	ROAD_VIEW
    ROAD_VIEW();
    // asm 000056B1: 	CALL	UPDATE_CAMERA
    UPDATE_CAMERA();
    // asm 000056B2: 	CALL	RESCAN
    RESCAN();
    // asm 000056B3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_CATCHUP", 0, 0);
}

// *----------------------------------------------------------------------------
static void INIT_LEAD(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;

    // MAME_VALIDATOR_EXIT();
    // asm 000056B4: 	LDI	-300,R0
    // asm 000056B5: 	FLOAT	R0,R1
    // asm 000056B6: 	STF	R1,*+AR7(CAMYOFF)
    ctx->ATTRACT_DELTA.camyoff = C3X_STF(C3X_FROM_INT(-300));
    // asm 000056B7: 	LDI	-4,R0
    // asm 000056B8: 	LDI	1,R1
    // asm 000056B9: 	LDI	0,R2
    // asm 000056BA: 	LDF	PI,R3
    // asm 000056BB: 	CALL	INITROAD_VIEW
    INITROAD_VIEW(-4, 1, 0, C3X_IMM_F32(PI));
    // asm 000056BC: 	LDF	1,R2
    // asm 000056BD: 	CALL	SET_LANE
    SET_LANE(C3X_IMM_F32(1.0f));
    // asm 000056BE: 	CALL	INIT_LANE
    INIT_LANE();
    // asm 000056BF: 	CALL	LEAD_VIEW
    LEAD_VIEW();
    // asm 000056C0: 	CALL	UPDATE_CAMERA
    UPDATE_CAMERA();
    // asm 000056C1: 	CALL	RESCAN
    RESCAN();
    // asm 000056C2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LEAD", 0, 0);
}

// *----------------------------------------------------------------------------
static void INITROAD_VIEW(int direction_to_search /*R0*/, int distance_to_start_with /*R1*/, int position_to_follow /*R2*/,
                          c3x_reg_t rady_offset /*R3*/) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    OBJ* car_obj;
    LEG_PAYLOAD* road_obj;
    int direction_step;
    int behind_sections;

    // asm 000056C3: 	STI	R0,*+AR7(CAMERA_XYZR)
    // asm 000056C4: 	STI	R1,*+AR7(CAMERA_XYZR+1)
    // asm 000056C5: 	STI	R2,*+AR7(CAMERA_XYZR+2)
    // asm 000056C6: 	STF	R3,*+AR7(CAMERA_XYZR+3)
    ctx->ATTRACT_DELTA.camera_xyzr[0] = (u32)direction_to_search;
    ctx->ATTRACT_DELTA.camera_xyzr[1] = (u32)distance_to_start_with;
    ctx->ATTRACT_DELTA.camera_xyzr[2] = (u32)position_to_follow;
    memcpy(&ctx->ATTRACT_DELTA.camera_xyzr[3], &rady_offset, sizeof(rady_offset));
    // asm 000056C7: 	LDF	0,R0
    // asm 000056C8: 	STF	R0,*+AR7(CAMERA_ACCEL)
    ctx->ATTRACT_DELTA.camera_accel = C3X_STF(C3X_FROM_INT(0));
    // asm 000056C9: 	LDI	1,R0
    // asm 000056CA: 	STI	R0,*+AR7(CUT_PAN)
    ctx->ATTRACT_DELTA.cut_pan = 1;
    // asm 000056CB: 	LDI	*+AR7(CAMERA_XYZR+2),R2		;Position to find
    // asm 000056CC: 	CALL	FIND_RACER
    car_obj = FIND_RACER(position_to_follow); // ;Position to find
    // asm 000056CD: 	LDI	AR4,AR0
    // asm 000056CE: 	STI	AR4,*+AR7(CAROBJ)
    // asm 000056CF: 	STI	AR4,@PLYCAR
    ctx->ATTRACT_DELTA.carobj = car_obj;
    PLYCAR = car_obj;
    // asm 000056D0: 	LDI	*+AR4(OCARBLK),AR1
    // asm 000056D1: 	STI	AR1,@PLYCBLK
    PLYCBLK = car_obj != NULL ? car_obj->carblk : NULL;
    // asm 000056D2: 	CALL	GET_MAP_CARBLK
    road_obj = GET_MAP_CARBLK(car_obj);
    // asm 000056D3: 	LDI	*+AR7(CAMERA_XYZR+1),R0
    // asm 000056D4: 	MPYI	*+AR7(CAMERA_XYZR),R0
    // asm 000056D5: 	SUBI	R0,AR4		;Start this many track sections behind first place
    direction_step = direction_to_search / 4;
    behind_sections = distance_to_start_with * direction_step;
    road_obj -= behind_sections; // ;Start this many track sections behind first place
    // asm 000056D6: 	LDI	@NOSWAP,R0
    // asm 000056D7: 	CMPI	0,R0			;Don't check during init
    // asm 000056D8: 	BNE	OK_FOUNDIT
    if (NOSWAP == 0) { // ;Don't check during init
TRY_AGAIN:
        // asm 000056D9: 	LDI	*+AR4(3),R2		;Get ID
        // asm 000056DA: 	CALL	FIND_TRACK
        // asm 000056DB: 	CMPI	0,AR2
        // asm 000056DC: 	BNE	OK_FOUNDIT
        if (FIND_TRACK(road_obj->as_fixed.id) == NULL) {
            // asm 000056DD: 	ADDI	*+AR7(CAMERA_XYZR),AR4
            // asm 000056DE: 	BR	TRY_AGAIN
            road_obj += direction_step;
            goto TRY_AGAIN;
        }
    }
OK_FOUNDIT:
    // asm 000056DF: 	CALL	GET_ROAD_RADY
    // asm 000056E0: 	STF	R0,*+AR7(NEW_RADY)
    ctx->ATTRACT_DELTA.road_obj = road_obj;
    ctx->ATTRACT_DELTA.new_rady =C3X_STF(GET_ROAD_RADY(road_obj));
    MAME_ASSERT_MEM(0x000056E0, "d@(ar4+3)", &ctx->ATTRACT_DELTA.road_obj->as_fixed.id);
    // asm 000056E1: 	FLOAT	*+AR4(X),R0
    // asm 000056E2: 	STF	R0,*+AR7(NEW_POSX)
    ctx->ATTRACT_DELTA.new_posx =C3X_STF(C3X_FROM_INT(road_obj->as_fixed.pos_x));
    // asm 000056E3: 	FIX	*+AR7(CAMYOFF),R0
    // asm 000056E4: 	ADDI	*+AR4(Y),R0
    // asm 000056E5: 	FLOAT	R0
    // asm 000056E6: 	STF	R0,*+AR7(NEW_POSY)
    ctx->ATTRACT_DELTA.new_posy =
       C3X_STF(C3X_FROM_INT(road_obj->as_fixed.pos_y_with_lane_flag + FIX(ctx->ATTRACT_DELTA.camyoff)));
    // asm 000056E7: 	FLOAT	*+AR4(Z),R0
    // asm 000056E8: 	STF	R0,*+AR7(NEW_POSZ)
    ctx->ATTRACT_DELTA.new_posz =C3X_STF(C3X_FROM_INT(road_obj->as_fixed.pos_z));
    // asm 000056E9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITROAD_VIEW", 0, 0);
}

// *----------------------------------------------------------------------------
static void WATCH_VIEW(void) {
    // asm 000056EA: 	LDI	*+AR7(CAROBJ),AR0
    // asm 000056EB: 	LDF	*+AR0(OPOSX),R2		;Get the direction of the car relative to
    // asm 000056EC: 	SUBF	*+AR7(NEW_POSX),R2	;the direction the camera is moving in.
    // asm 000056ED: 	LDF	*+AR0(OPOSZ),R3
    // asm 000056EE: 	SUBF	*+AR7(NEW_POSZ),R3
    // asm 000056EF: 	CALL	ARCTANF
    // asm 000056F0: 	CALL	NORM_VECTOR
    // asm 000056F1: 	SUBF	HALFPI,R0
    // asm 000056F2: 	CALL	NORM_VECTOR
    // asm 000056F3: 	NEGF	R0
    // asm 000056F4: 	STF	R0,*+AR7(NEW_RADY)
    // asm 000056F5: WV1
    // asm 000056F5: 	LDI	AR7,R2
    // asm 000056F6: 	ADDI	NEW_RADX,R2
    // asm 000056F7: 	LDI	AR7,AR2
    // asm 000056F8: 	ADDI	NEW_MATRIX,AR2
    // asm 000056F9: 	CALL	FIND_MATRIX
    // asm 000056FA: 	LDI	@NFRAMES,R0
    // asm 000056FB: 	SUBI	R0,AR5		;Will count frames in this case
    // asm 000056FC: 	LDI	@ATTRWAVE,R0
    // asm 000056FD: 	CMPI	2,R0		;preincremented will actually be GGPARK
    // asm 000056FE: 	BNE	NO_WHOOSH
    // asm 000056FF: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm 00005701: 	CMPI	0,R0
    // asm 00005702: 	BEQ	NO_WHOOSH
    // asm 00005703: 	LDF	R6,R0
    // asm 00005704: 	SUBF	*+AR7(NEW_RADY),R0
    // asm 00005705: 	CMPF	0.18,R0			;9deg	diff
    // asm 00005706: 	BLT	NO_WHOOSH
    // asm 00005707: 	LDF	-100,R6				;do only once
    // asm 00005708: 	SOND1	WHOOSH
NO_WHOOSH:
    // asm 0000570A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATCH_VIEW", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
static void REV_ROAD_VIEW(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    CARBLK* carblk;
    c3x_reg_t car_speed;

    // asm 0000570B: 	LDI	*+AR7(CAROBJ),AR0
    // asm 0000570C: 	LDI	*+AR0(OCARBLK),AR1
    carblk = ctx->ATTRACT_DELTA.carobj != NULL ? ctx->ATTRACT_DELTA.carobj->carblk : NULL;
    // asm 0000570D: 	LDF	*+AR1(CARSPEED),R0
    car_speed = carblk != NULL ? C3X_LDF(carblk->speed) : C3X_FROM_INT(0);
    // asm 0000570E: 	MPYF	1.8,R0
    // asm 0000570F: 	STF	R0,*+AR7(CAMERA_VEL)	;Set initial camera velocity
    ctx->ATTRACT_DELTA.camera_vel = C3X_STF(C3X_MUL(car_speed, C3X_IMM_F32(1.8))); // ;Set initial camera velocity
    // asm 00005710: 	LDF	0.6,R2
    // asm 00005711: 	CALL	SET_LANE
    SET_LANE(C3X_IMM_F32(0.6));
    // asm 00005712: 	CALL	MOVE_ROAD_CAMERA
    MOVE_ROAD_CAMERA();
    // asm 00005713: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REV_ROAD_VIEW", 0, 0);
}

/* asm: ACCEL_RATE	.float	0.000002 */
static c3x_reg_t ACCEL_RATE = C3X_INIT(0.000002f, 0xED0637BD05ull);

static void ROAD_VIEW(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    OBJ* closest_racer;
    CARBLK* carblk;
    c3x_reg_t car_speed;
    c3x_reg_t distance;
    c3x_reg_t accel_step;
    c3x_reg_t cardis_scaled;
    c3x_reg_t lane_target;

    // asm 00005714: 	CALL	FIND_CLOSEST_RACER
    distance = FIND_CLOSEST_RACER(&closest_racer);
    // asm 00005715: 	CMPI	0,AR1
    // asm 00005716: 	LDIZ	0,AR5		;PASSED FIRST PLACE CHANGE MODES (AR5 = frame to switch)
    if (closest_racer != NULL) { // ;PASSED FIRST PLACE CHANGE MODES (AR5 = frame to switch)
        // asm 00005717: 	BEQ	RV1
        // asm 00005718: 	CMPI	*+AR7(CAROBJ),AR1
        // asm 00005719: 	BEQ	RV1A
        if (ctx->ATTRACT_DELTA.carobj != closest_racer) {
            // asm 0000571A: 	LDF	0,R2
            // asm 0000571B: 	STF	R2,*+AR7(CAMERA_ACCEL)
            ctx->ATTRACT_DELTA.camera_accel = C3X_STF(C3X_FROM_INT(0));
            // asm 0000571C: 	STI	AR1,*+AR7(CAROBJ)	;FOUND a NEW CAR
            // asm 0000571D: 	STI	AR1,@PLYCAR
            ctx->ATTRACT_DELTA.carobj = closest_racer; // ;FOUND a NEW CAR
            PLYCAR = closest_racer;
            // asm 0000571E: 	LDI	*+AR1(OCARBLK),AR0
            // asm 0000571F: 	STI	AR0,@PLYCBLK
            PLYCBLK = closest_racer->carblk;
            // asm 00005720: 	STF	R0,*+AR7(CARDIS)
            ctx->ATTRACT_DELTA.cardis = C3X_STF(distance);
        }
RV1A:
        // asm 00005721: 	LDF	@ACCEL_RATE,R3
        // asm 00005722: 	LDF	*+AR7(CAMERA_ACCEL),R2
        // asm 00005723: 	LDF	*+AR7(CARDIS),R1
        // asm 00005724: 	MPYF	0.6,R1
        cardis_scaled = C3X_MUL(ctx->ATTRACT_DELTA.cardis, C3X_IMM_F32(0.6));
        // asm 00005725: 	CMPF	R0,R1			;Are we half way there yet?
        accel_step = ACCEL_RATE;
        if (C3X_LE(distance, cardis_scaled)) { // ;Are we half way there yet?
            // asm 00005726: 	BLT	RV1B
            // asm 00005727: 	NEGF	R3
            accel_step = C3X_NEG(accel_step);
        }
RV1B:
        // asm 00005728: 	ADDF	R3,R2
        // asm 00005729: 	LDFLT	0,R2
        // asm 0000572A: 	STF	R2,*+AR7(CAMERA_ACCEL)
        ctx->ATTRACT_DELTA.camera_accel = C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.camera_accel, accel_step));
        if (C3X_LT(ctx->ATTRACT_DELTA.camera_accel, C3X_FROM_INT(0))) {
            ctx->ATTRACT_DELTA.camera_accel = C3X_STF(C3X_FROM_INT(0));
        }
        // asm 0000572B: 	MPYF	R0,R2			;Factor in overall distance
        // asm 0000572C: 	ADDF	1.6,R2
        lane_target = C3X_ADD(C3X_MUL(ctx->ATTRACT_DELTA.camera_accel, distance), C3X_IMM_F32(1.6)); // ;Factor in overall distance
        // asm 0000572D: 	LDI	*+AR1(OCARBLK),AR0
        // asm 0000572E: 	LDF	*+AR0(CARSPEED),R0
        carblk = closest_racer->carblk;
        car_speed = carblk != NULL ? C3X_LDF(carblk->speed) : C3X_FROM_INT(0);
        // asm 0000572F: 	MPYF	R2,R0
        // asm 00005730: 	STF	R0,*+AR7(CAMERA_VEL)	;Set initial camera velocity
        ctx->ATTRACT_DELTA.camera_vel = C3X_STF(C3X_MUL(car_speed, lane_target)); // ;Set initial camera velocity
    }
RV1:
    // asm 00005731: 	LDF	0.6,R2
    // asm 00005732: 	CALL	SET_LANE
    SET_LANE(C3X_IMM_F32(0.6));
    // asm 00005733: 	CALL	MOVE_ROAD_CAMERA
    MOVE_ROAD_CAMERA();
    // asm 00005734: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROAD_VIEW", 0, 0);
}

// *----------------------------------------------------------------------------
static void LEAD_VIEW(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    CARBLK* carblk;
    c3x_reg_t car_speed;

    MAME_VALIDATOR_EXIT();
    // asm 00005735: 	LDI	*+AR7(CAROBJ),AR0
    // asm 00005736: 	LDI	*+AR0(OCARBLK),AR1
    carblk = ctx->ATTRACT_DELTA.carobj != NULL ? ctx->ATTRACT_DELTA.carobj->carblk : NULL;
    // asm 00005737: 	LDF	*+AR1(CARSPEED),R0
    car_speed = carblk != NULL ? C3X_LDF(carblk->speed) : C3X_FROM_INT(0);
    // asm 00005738: 	MPYF	1.51,R0
    // asm 00005739: 	STF	R0,*+AR7(CAMERA_VEL)	;Set initial camera velocity
    ctx->ATTRACT_DELTA.camera_vel = C3X_STF(C3X_MUL(car_speed, C3X_IMM_F32(1.51))); // ;Set initial camera velocity
    // asm 0000573A: 	LDF	0.6,R2
    // asm 0000573B: 	CALL	SET_LANE
    SET_LANE(C3X_IMM_F32(0.6));
    // asm 0000573C: 	CALL	MOVE_ROAD_CAMERA
    MOVE_ROAD_CAMERA();
    // asm 0000573D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEAD_VIEW", 0, 0);
}

// *----------------------------------------------------------------------------
static void MOVE_ROAD_CAMERA(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    LEG_PAYLOAD* road_obj;
    LEG_PAYLOAD* next_road;
    c3x_reg_t camera_rady_offset;
    c3x_reg_t road_height;
    c3x_reg_t slope_length;
    c3x_reg_t slope_y;

    // asm 0000573E: 	LDI	@NOSWAP,R0	;If noswap is on definatly do it
    // asm 0000573F: 	BNE	MRC1
    if (NOSWAP == 0) { // ;If noswap is on definatly do it
    // asm 00005740: 	LDI	@SUSPEND_MODE,R0	;This stops the shimmer at the start of bevhills
    // asm 00005741: 	CMPI	SM_HALT,R0
    // asm 00005742: 	BEQ	MRCX
        if (SUSPEND_MODE == SM_HALT) { // ;This stops the shimmer at the start of bevhills
            goto MRCX;
        }
    }
MRC1:
    // asm 00005743: 	CALL	GETCAMDIR
    GETCAMDIR();
    // asm 00005744: 	LDI	@VECTORBI,AR2
    // asm 00005745: 	LDF	*+AR7(NEW_RADX),R0
    // asm 00005746: 	STF	R0,*+AR2(X)
    VECTORBI.X = C3X_STF(C3X_REG(ctx->ATTRACT_DELTA.new_radx));
    // asm 00005747: 	LDF	*+AR7(NEW_RADZ),R0
    // asm 00005748: 	STF	R0,*+AR2(Z)
    VECTORBI.Z = C3X_STF(C3X_REG(ctx->ATTRACT_DELTA.new_radz));
    // asm 00005749: 	LDF	*+AR7(NEW_RADY),R0
    // asm 0000574A: 	SUBF	*+AR7(CAMERA_XYZR+3),R0	;LOOSE the Offset rad for viewing
    memcpy(&camera_rady_offset, &ctx->ATTRACT_DELTA.camera_xyzr[3], sizeof(camera_rady_offset));
    road_height = C3X_SUB(ctx->ATTRACT_DELTA.new_rady, camera_rady_offset); // ;LOOSE the Offset rad for viewing
    // asm 0000574B: 	CALL	NORM_VECTOR
    road_height = NORM_VECTOR(road_height);
    // asm 0000574C: 	NEGF	R0			;Move in oposite direction looking?
    // asm 0000574D: 	STF	R0,*+AR2(Y)
    VECTORBI.Y =C3X_STF(C3X_NEG(road_height)); // ;Move in oposite direction looking?
    // asm 0000574E: 	LDI	@VECTORBI,R2
    // asm 0000574F: 	LDI	@MATRIXAI,AR2
    // asm 00005750: 	CALL	FIND_MATRIX
    FIND_MATRIX(&MATRIXAI, &VECTORBI);
    // asm 00005751: 	LDI	@VECTORAI,AR2
    // asm 00005752: 	LDF	0,R0
    // asm 00005753: 	STF	R0,*+AR2(X)
    // asm 00005754: 	STF	R0,*+AR2(Y)
    VECTORAI.X =C3X_STF(C3X_FROM_INT(0));
    VECTORAI.Y =C3X_STF(C3X_FROM_INT(0));
    // asm 00005755: 	LDF	*+AR7(CAMERA_VEL),R0
    // asm 00005756: 	FLOAT	@NFRAMES,R1
    // asm 00005757: 	MPYF	R1,R0
    // asm 00005758: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF(C3X_MUL(ctx->ATTRACT_DELTA.camera_vel, C3X_FROM_INT(NFRAMES)));
    VECTORAI.Z = VECTORAI.Z;
    // asm 00005759: 	LDI	@VECTORBI,R3
    // asm 0000575A: 	LDI	@MATRIXAI,R2
    // asm 0000575B: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &MATRIXAI, &VECTORBI);
    // asm 0000575C: 	LDI	@VECTORBI,AR2
    // asm 0000575D: 	LDF	*+AR2(X),R0
    // asm 0000575E: 	ADDF	*+AR7(NEW_POSX),R0
    // asm 0000575F: 	STF	R0,*+AR7(NEW_POSX)
    ctx->ATTRACT_DELTA.new_posx =C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.new_posx, VECTORBI.X));
    // asm 00005760: 	LDF	*+AR2(Z),R0
    // asm 00005761: 	ADDF	*+AR7(NEW_POSZ),R0
    // asm 00005762: 	STF	R0,*+AR7(NEW_POSZ)
    ctx->ATTRACT_DELTA.new_posz =C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.new_posz, VECTORBI.Z));
    // asm 00005763: 	PUSH	AR4
    // asm 00005764: 	LDI	AR7,AR4
    // asm 00005765: 	ADDI	NEW_POSX,AR4
    // asm 00005766: 	CALL	CAMSCAN
    if (CAMSCAN((VECTOR*)&ctx->ATTRACT_DELTA.new_posx, &road_height)) {
    // asm 00005767: 	POP	AR4
    // asm 00005768: 	BNC	USE_SLOPE
    // asm 00005769: 	LDF	*+AR7(CAMYOFF),R1
    // asm 0000576A: 	ADDF	R1,R0
    // asm 0000576B: 	ADDF	*+AR7(NEW_POSY),R0
    // asm 0000576C: 	STF	R0,*+AR7(NEW_POSY)
        ctx->ATTRACT_DELTA.new_posy =
           C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.new_posy, C3X_ADD(road_height, ctx->ATTRACT_DELTA.camyoff)));
    // asm 0000576D: 	BR	NO_SLOPE
        goto NO_SLOPE;
    }
USE_SLOPE:
    // ;This code should really never get used
    // asm 0000576E: 	LDI	AR4,AR2
    // asm 0000576F: 	ADDI	4,AR2
    road_obj = ctx->ATTRACT_DELTA.road_obj;
    next_road = road_obj + 1;
    // asm 00005770: 	LDI	*+AR2(Z),R0
    // asm 00005771: 	SUBI	*+AR4(Z),R0
    // asm 00005772: 	FLOAT	R0
    // asm 00005773: 	MPYF	R0,R0
    slope_length = C3X_FROM_INT(next_road->as_fixed.pos_z - road_obj->as_fixed.pos_z);
    slope_length = C3X_MUL(slope_length, slope_length);
    // asm 00005774: 	LDI	*+AR2(X),R1
    // asm 00005775: 	SUBI	*+AR4(X),R1
    // asm 00005776: 	FLOAT	R1
    // asm 00005777: 	MPYF	R1,R1
    road_height = C3X_FROM_INT(next_road->as_fixed.pos_x - road_obj->as_fixed.pos_x);
    road_height = C3X_MUL(road_height, road_height);
    // asm 00005778: 	ADDF3	R0,R1,R2	;R= SQRT (X^2 + Z^2)
    slope_length = C3X_ADD(slope_length, road_height); // ;R= SQRT (X^2 + Z^2)
    // asm 00005779: 	CALL	SQRT
    // asm 0000577A: 	LDF	R0,R1
    slope_length = SQRT(slope_length);
    // asm 0000577B: 	LDI	*+AR2(Y),R0
    // asm 0000577C: 	SUBI	*+AR4(Y),R0
    // asm 0000577D: 	FLOAT	R0
    // asm 0000577E: 	MPYF	1.05,R0		;FUDGE FACTOR
    slope_y = C3X_MUL(C3X_FROM_INT(next_road->as_fixed.pos_y_with_lane_flag - road_obj->as_fixed.pos_y_with_lane_flag), C3X_IMM_F32(1.05)); // ;FUDGE FACTOR
    // asm 0000577F: 	CALL	DIV_F
    slope_y = DIV_F(slope_y, slope_length);
    // asm 00005780: 	LDI	@VECTORAI,AR0
    // asm 00005781: 	LDF	*+AR0(Z),R1
    // asm 00005782: 	MPYF	R1,R0
    // asm 00005783: 	ADDF	*+AR7(NEW_POSY),R0
    // asm 00005784: 	STF	R0,*+AR7(NEW_POSY)
    ctx->ATTRACT_DELTA.new_posy = C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.new_posy, C3X_MUL(slope_y, VECTORAI.Z)));
    // asm 00005785: 	LDI	-400,R1		;at 1000 above the road
    // asm 00005786: 	ADDI	*+AR4(Y),R1
    // asm 00005787: 	FLOAT	R1
    // asm 00005788: 	SUBF	*+AR7(NEW_POSY),R1
    // asm 00005789: 	MPYF	0.04,R1
    // asm 0000578A: 	ADDF	*+AR7(NEW_POSY),R1
    // asm 0000578B: 	STF	R1,*+AR7(NEW_POSY)
    ctx->ATTRACT_DELTA.new_posy =
        C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.new_posy,
                C3X_MUL(C3X_SUB(C3X_FROM_INT(road_obj->as_fixed.pos_y_with_lane_flag - 400), ctx->ATTRACT_DELTA.new_posy),
                        C3X_IMM_F32(0.04))));
NO_SLOPE:
    // asm 0000578C: 	LDI	AR7,R2
    // asm 0000578D: 	ADDI	NEW_RADX,R2
    // asm 0000578E: 	LDI	AR7,AR2
    // asm 0000578F: 	ADDI	NEW_MATRIX,AR2
    // asm 00005790: 	CALL	FIND_MATRIX
    FIND_MATRIX(&ctx->ATTRACT_DELTA.new_matrix, (VECTOR*)&ctx->ATTRACT_DELTA.new_radx);
    // asm 00005791: 	CALL	NEXT_ROAD
    NEXT_ROAD();
MRCX:
    // asm 00005792: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_ROAD_CAMERA", 0, 0);
}

/*
 *---------------------------------------------------------------------------
 * Updates AR4 to point to the road segment that the camera is on
 * NOTE: uses Stealth mode
 * INPUT	AR4 = segment on
 * OUTPUT AR4 = segment on (updated)
 *
 */

static void NEXT_ROAD(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    LEG_PAYLOAD* road_obj;
    LEG_PAYLOAD* next_road;
    c3x_reg_t current_distance_squared;
    c3x_reg_t next_distance_squared;
    c3x_reg_t delta_x;
    c3x_reg_t delta_z;

    road_obj = ctx->ATTRACT_DELTA.road_obj;

NEXT_ROAD_LOOP:
    // asm 00005793: 	FLOAT	*+AR4(X),R0
    // asm 00005794: 	SUBF	*+AR7(NEW_POSX),R0
    // asm 00005795: 	MPYF	R0,R0
    delta_x = C3X_SUB(C3X_FROM_INT(road_obj->as_fixed.pos_x), ctx->ATTRACT_DELTA.new_posx);
    current_distance_squared = C3X_MUL(delta_x, delta_x);
    // asm 00005796: 	FLOAT	*+AR4(Z),R1
    // asm 00005797: 	SUBF	*+AR7(NEW_POSZ),R1
    // asm 00005798: 	MPYF	R1,R1
    delta_z = C3X_SUB(C3X_FROM_INT(road_obj->as_fixed.pos_z), ctx->ATTRACT_DELTA.new_posz);
    delta_z = C3X_MUL(delta_z, delta_z);
    // asm 00005799: 	ADDF	R1,R0
    current_distance_squared = C3X_ADD(current_distance_squared, delta_z);
    // asm 0000579A: 	LDI	AR4,AR2
    // asm 0000579B: 	ADDI	4,AR2
    next_road = road_obj + 1;
    // asm 0000579C: 	FLOAT	*+AR2(X),R1
    // asm 0000579D: 	SUBF	*+AR7(NEW_POSX),R1
    // asm 0000579E: 	MPYF	R1,R1
    delta_x = C3X_SUB(C3X_FROM_INT(next_road->as_fixed.pos_x), ctx->ATTRACT_DELTA.new_posx);
    next_distance_squared = C3X_MUL(delta_x, delta_x);
    // asm 0000579F: 	FLOAT	*+AR2(Z),R2
    // asm 000057A0: 	SUBF	*+AR7(NEW_POSZ),R2
    // asm 000057A1: 	MPYF	R2,R2
    delta_z = C3X_SUB(C3X_FROM_INT(next_road->as_fixed.pos_z), ctx->ATTRACT_DELTA.new_posz);
    delta_z = C3X_MUL(delta_z, delta_z);
    // asm 000057A2: 	ADDF	R2,R1
    next_distance_squared = C3X_ADD(next_distance_squared, delta_z);
    // asm 000057A3: 	CMPF	R1,R0
    // asm 000057A4: 	BLE	fcrx			;didn't move
    if (C3X_LE(current_distance_squared, next_distance_squared)) { // ;didn't move
        goto fcrx;
    }
    // asm 000057A5: 	ADDI	4,AR4
    road_obj = next_road;
    ctx->ATTRACT_DELTA.road_obj = road_obj;
    MAME_ASSERT_MEM(0x000057A6, "d@(ar4+3)", &ctx->ATTRACT_DELTA.road_obj->as_fixed.id);
    // ;	.if	DEBUG
    // ;	LDI	*+AR4(3),R2
    // ;	CALL	FIND_TRACK
    // ;	CMPI	0,AR2
    // ;	BNE	fcr1
    // ;	LOCKUP				;TRACK not loaded yet
    // ;fcr1
    // ;	.endif
    // asm 000057A6: 	DEC	AR5			;AR5 is the number of road segments before swithching
    ctx->ATTRACT_DELTA.frames_left -= 1; // ;AR5 is the number of road segments before swithching
    MAME_ASSERT_REG(0x000057A7, "AR5", &ctx->ATTRACT_DELTA.frames_left);
    // asm 000057A7: 	BR	NEXT_ROAD		;See if the road segment beyond is even closer
    goto NEXT_ROAD_LOOP; // ;See if the road segment beyond is even closer
fcrx:
    // asm 000057A8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NEXT_ROAD", 0, 0);
}

/*
 *---------------------------------------------------------------------------
 * Sets the cameras RADY based on the next track segments reletive position
 * NOTE: uses stealth mode
 * No I/O
 *	Sets CAMERA_RADY = (CAMERA_XYZR+3)+ RAD to next track section
 */

static void GETCAMDIR(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    LEG_PAYLOAD* road_obj;
    LEG_PAYLOAD* target_road;
    c3x_reg_t camera_rady_offset;
    c3x_reg_t smoothing;
    c3x_reg_t camera_velocity;
    c3x_reg_t nframes_float;
    c3x_reg_t smoothing_numerator;
    c3x_reg_t smoothing_divisor;
    c3x_reg_t road_rady;
    c3x_reg_t target_z;
    c3x_reg_t target_x;
    c3x_reg_t camera_rady;

    // asm 000057A9: 	LDF	*+AR7(CAMERA_VEL),R0	;Adjust the smothing factor to speed
    camera_velocity = C3X_LDF(ctx->ATTRACT_DELTA.camera_vel);
    // asm 000057AA: 	FLOAT	@NFRAMES,R1
    nframes_float = C3X_FROM_INT(NFRAMES);
    // asm 000057AB: 	MPYF	R1,R0
    smoothing_numerator = C3X_MUL(camera_velocity, nframes_float);
    // asm 000057AC: 	FLOAT	5000,R1
    smoothing_divisor = C3X_FROM_INT(5000);
    // asm 000057AD: 	CALL	DIV_F
    smoothing = DIV_F(smoothing_numerator, smoothing_divisor); // ;Adjust the smothing factor to speed
    // asm 000057AE: 	LDF	R0,R4
    road_obj = ctx->ATTRACT_DELTA.road_obj;
    // asm 000057AF: 	CALL	GET_ROAD_RADY		;Calculate the offset for lane position
    MAME_ASSERT_MEM(0x000057AF, "d@(ar4+3)", &road_obj->as_fixed.id);
    road_rady = GET_ROAD_RADY(road_obj); // ;Calculate the offset for lane position
    // asm 000057B0: 	NEGF	R0			;based on the roads direction
    // asm 000057B1: 	LDF	R0,R2
    // asm 000057B2: 	LDI	@MATRIXAI,AR2
    // asm 000057B3: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXAI, C3X_NEG(road_rady)); // ;based on the roads direction
    // asm 000057B4: 	LDI	@VECTORAI,AR2
    // asm 000057B5: 	LDF	0,R0
    // asm 000057B6: 	STF	R0,*+AR2(Y)
    // asm 000057B7: 	STF	R0,*+AR2(Z)
    VECTORAI.Y =C3X_STF(C3X_FROM_INT(0));
    VECTORAI.Z =C3X_STF(C3X_FROM_INT(0));
    // asm 000057B8: 	LDF	*+AR7(CAMERA_LANE),R0
    // asm 000057B9: 	STF	R0,*+AR2(X)
    VECTORAI.X = C3X_STF(C3X_REG(ctx->ATTRACT_DELTA.camera_lane));
    // asm 000057BA: 	LDI	@VECTORBI,R3
    // asm 000057BB: 	LDI	@MATRIXAI,R2
    // asm 000057BC: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &MATRIXAI, &VECTORBI);
    // asm 000057BD: 	LDI	AR4,AR1
    // asm 000057BE: 	ADDI	4*2,AR1
    target_road = road_obj + 2;
    // asm 000057BF: 	LDI	@VECTORBI,AR2			;Factor in the desired location to be at
    // asm 000057C0: 	FLOAT	*+AR1(Z),R2			;reletive to where we are
    // asm 000057C1: 	ADDF	*+AR2(Z),R2
    // asm 000057C2: 	SUBF	*+AR7(NEW_POSZ),R2
    target_z = C3X_SUB(C3X_ADD(C3X_FROM_INT(target_road->as_fixed.pos_z), VECTORBI.Z), ctx->ATTRACT_DELTA.new_posz); // ;reletive to where we are
    // asm 000057C3: 	FLOAT	*+AR1(X),R3
    // asm 000057C4: 	ADDF	*+AR2(X),R3
    // asm 000057C5: 	SUBF	*+AR7(NEW_POSX),R3
    target_x = C3X_SUB(C3X_ADD(C3X_FROM_INT(target_road->as_fixed.pos_x), VECTORBI.X), ctx->ATTRACT_DELTA.new_posx);
    // asm 000057C6: 	CALL	ARCTANF
    camera_rady = ARCTANF(target_z, target_x);
    // asm 000057C7: 	CALL	NORM_VECTOR
    camera_rady = NORM_VECTOR(camera_rady);
    // asm 000057C8: 	ADDF	*+AR7(CAMERA_XYZR+3),R0		;Offset for VIEW
    memcpy(&camera_rady_offset, &ctx->ATTRACT_DELTA.camera_xyzr[3], sizeof(camera_rady_offset));
    camera_rady = C3X_ADD(camera_rady, camera_rady_offset); // ;Offset for VIEW
    // asm 000057C9: 	CALL	NORM_VECTOR
    camera_rady = NORM_VECTOR(camera_rady);
    // asm 000057CA: 	SUBF	*+AR7(NEW_RADY),R0
    camera_rady = C3X_SUB(camera_rady, ctx->ATTRACT_DELTA.new_rady);
    // asm 000057CB: 	LDF	R4,R2
    // asm 000057CC: 	CALL	SMOOTH_VECTOR
    camera_rady = SMOOTH_VECTOR(camera_rady, smoothing);
    // asm 000057CD: 	ADDF	*+AR7(NEW_RADY),R0
    // asm 000057CE: 	STF	R0,*+AR7(NEW_RADY)
    ctx->ATTRACT_DELTA.new_rady = C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.new_rady, camera_rady));
    // asm 000057CF: 	LDF	0,R0
    // asm 000057D0: 	STF	R0,*+AR7(NEW_RADX)
    // asm 000057D1: 	STF	R0,*+AR7(NEW_RADZ)
    ctx->ATTRACT_DELTA.new_radx = C3X_STF(C3X_FROM_INT(0));
    ctx->ATTRACT_DELTA.new_radz = C3X_STF(C3X_FROM_INT(0));
    // asm 000057D2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCAMDIR", 0, 0);
}

/*
 *----------------------------------------------------------------------------
 *INPUT R2 = FL smoothing
 */
static void SET_LANE(c3x_reg_t smoothing /*R2*/) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    OBJ* car_obj;
    PROC* proc;
    c3x_reg_t lane_center;
    c3x_reg_t road_offset;
    c3x_reg_t lane_offset;

    // asm 000057D3: 	LDI	*+AR7(CAROBJ),AR0
    // asm 000057D4: 	LDI	*+AR0(OPLINK),AR0
    car_obj = ctx->ATTRACT_DELTA.carobj;
    proc = car_obj != NULL ? car_obj->plink : NULL;
    // asm 000057D5: 	FLOAT	1000,R1			;Offset to center of right lanes
    lane_center = C3X_FROM_INT(1000); // ;Offset to center of right lanes
    // asm 000057D6: 	LDF	*+AR0(ROADOFFSET),R0
    road_offset = (proc != NULL && proc->ctx != NULL) ? C3X_LDF(proc->ctx->RACER_DRONE.road_offset) : C3X_FROM_INT(0);
    // asm 000057D7: 	BP	SL1
    // asm 000057D8: 	NEGF	R1
    if (C3X_LE(road_offset, C3X_FROM_INT(0))) {
        lane_center = C3X_NEG(lane_center);
    }
SL1:
    // asm 000057D9: 	SUBF	R1,R0
    // ;	MPYF	0.40,R0			;Move to just inside where the car is.
    // asm 000057DA: 	MPYF	0.30,R0			;Move to just inside where the car is.
    // asm 000057DB: 	ADDF	R1,R0
    lane_offset = C3X_ADD(C3X_MUL(C3X_SUB(road_offset, lane_center), C3X_IMM_F32(0.30)), lane_center); // ;Move to just inside where the car is.
    // asm 000057DC: 	CMPF	1.0,R2
    // asm 000057DD: 	BEQ	SL2
    if (C3X_NE(smoothing, C3X_IMM_F32(1.0f))) {
        // asm 000057DE: 	SUBF	*+AR7(CAMERA_LANE),R0
        // asm 000057DF: 	MPYF	R2,R0
        // asm 000057E0: 	ADDF	*+AR7(CAMERA_LANE),R0
        // asm 000057E1: 	STF	R0,*+AR7(CAMERA_LANE)
        lane_offset = C3X_ADD(C3X_MUL(C3X_SUB(lane_offset, ctx->ATTRACT_DELTA.camera_lane), smoothing), ctx->ATTRACT_DELTA.camera_lane);
    }
SL2:
    // asm 000057E2: 	STF	R0,*+AR7(CAMERA_LANE)
    ctx->ATTRACT_DELTA.camera_lane = C3X_STF(lane_offset);
    // asm 000057E3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_LANE", 0, 0);
}

// *----------------------------------------------------------------------------
static void INIT_LANE(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;

    // asm 000057E4: 	LDF	*+AR7(CAMERA_LANE),R3
    // asm 000057E5: 	CALL	DO_LANE_POS
    DO_LANE_POS(C3X_LDF(ctx->ATTRACT_DELTA.camera_lane));
    // asm 000057E6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LANE", 0, 0);
}

/*
 *---------------------------------------------------------------------------
 *INPUT	R2 = RADIANS to offset for doing the lanes
 *	R3 = Distance from center +/-
 */

static void DO_LANE_POS(c3x_reg_t lane_offset /*R3*/) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    c3x_reg_t road_rady;
    LEG_PAYLOAD* road_obj;

    // asm 000057E7: 	PUSHF	R3
    // asm 000057E8: 	LDI	AR4,AR1
    // asm 000057E9: 	ADDI	4*2,AR1
    // asm 000057EA: 	CALL	GET_ROAD_RADY
    // asm 000057EB: 	NEGF	R0
    road_obj = ctx->ATTRACT_DELTA.road_obj;
    MAME_ASSERT_MEM(0x000057EA, "d@(ar4+3)", &road_obj->as_fixed.id);
    road_rady = C3X_NEG(GET_ROAD_RADY(road_obj));
    // asm 000057EC: 	LDF	R0,R2
    // asm 000057ED: 	LDI	@MATRIXAI,AR2
    // asm 000057EE: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXAI, road_rady);
    // asm 000057EF: 	LDI	@VECTORAI,AR2
    // asm 000057F0: 	LDF	0,R0
    // asm 000057F1: 	STF	R0,*+AR2(Y)
    // asm 000057F2: 	STF	R0,*+AR2(Z)
    VECTORAI.Y = C3X_STF(C3X_FROM_INT(0));
    VECTORAI.Z = C3X_STF(C3X_FROM_INT(0));
    // asm 000057F3: 	POPF	R0
    // asm 000057F4: 	STF	R0,*+AR2(X)
    VECTORAI.X =C3X_STF(lane_offset);
    // asm 000057F5: 	LDI	@VECTORBI,R3
    // asm 000057F6: 	LDI	@MATRIXAI,R2
    // asm 000057F7: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &MATRIXAI, &VECTORBI);
    // asm 000057F8: 	LDI	@VECTORBI,AR2
    // asm 000057F9: 	LDF	*+AR2(X),R0
    // asm 000057FA: 	ADDF	*+AR7(NEW_POSX),R0
    // asm 000057FB: 	STF	R0,*+AR7(NEW_POSX)
    ctx->ATTRACT_DELTA.new_posx =C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.new_posx, VECTORBI.X));
    // asm 000057FC: 	LDF	*+AR2(Z),R0
    // asm 000057FD: 	ADDF	*+AR7(NEW_POSZ),R0
    // asm 000057FE: 	STF	R0,*+AR7(NEW_POSZ)
    ctx->ATTRACT_DELTA.new_posz =C3X_STF(C3X_ADD(ctx->ATTRACT_DELTA.new_posz, VECTORBI.Z));
    // asm 000057FF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DO_LANE_POS", 0, 0);
}

/*
 *---------------------------------------------------------------------------
 *INPUT	AR4 = POINTER to LEG_MAP where road is
 *OUTPUT	R0 = DIRECTION of road
 */
static c3x_reg_t GET_ROAD_RADY(LEG_PAYLOAD* road_obj /*AR4*/) {
    LEG_PAYLOAD* next_road;
    c3x_reg_t delta_z;
    c3x_reg_t delta_x;
    c3x_reg_t arctan_rady;
    c3x_reg_t road_rady;

    // asm 00005800: 	PUSH	AR1
    // asm 00005801: 	PUSH	R2
    // asm 00005802: 	PUSHF	R2
    // asm 00005803: 	PUSH	R3
    // asm 00005804: 	PUSHF	R3
    // asm 00005805: 	LDI	AR4,AR1
    // asm 00005806: 	ADDI	4*2,AR1
    next_road = road_obj + 2;
    // asm 00005807: 	LDI	*+AR1(Z),R2
    // asm 00005808: 	SUBI	*+AR4(Z),R2
    // asm 00005809: 	FLOAT	R2
    delta_z = C3X_FROM_INT(next_road->as_fixed.pos_z - road_obj->as_fixed.pos_z);
    // asm 0000580A: 	LDI	*+AR1(X),R3	;Get the direction of the road
    // asm 0000580B: 	SUBI	*+AR4(X),R3
    // asm 0000580C: 	FLOAT	R3
    delta_x = C3X_FROM_INT(next_road->as_fixed.pos_x - road_obj->as_fixed.pos_x); // ;Get the direction of the road
    // asm 0000580D: 	CALL	ARCTANF
    arctan_rady = ARCTANF(delta_z, delta_x);
    // asm 0000580E: 	CALL	NORM_VECTOR
    road_rady = NORM_VECTOR(arctan_rady);
    // asm 0000580F: 	POPF	R3
    // asm 00005810: 	POP	R3
    // asm 00005811: 	POPF	R2
    // asm 00005812: 	POP	R2
    // asm 00005813: 	POP	AR1
    // asm 00005814: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_ROAD_RADY", 0, 0);
    return road_rady;
}

/*
 *---------------------------------------------------------------------------
 *This algorithm is based on a bubble sort
 *Find the RACER DRONE IN FIRST PLACE based on position in LEG_MAP
 *INPUT R2 = Position to find
 *OUTPUT AR4 = OBJ of the racer
 *
 *
 */
static OBJ* FIND_RACER(int position_to_find /*R2*/) {
    OBJ* chosen_cars[10];
    OBJ* car_obj;
    OBJ* chosen_obj;
    LEG_PAYLOAD* road_obj;
    LEG_PAYLOAD* best_road_obj;
    LEG_PAYLOAD* upper_bound;
    int duplicate;
    int pass_count;
    int place;
    int i;

    // asm 00005815: 	LDI	AR7,AR3			;clear the list
    // asm 00005816: 	ADDI	TEMP_DATA,AR3
    // asm 00005817: 	LDI	0,R0
    // asm 00005818: 	RPTS	9
    // asm 00005819: 	STI	R0,*AR3++
    for (i = 0; i < 10; i++) { // ;clear the list
        chosen_cars[i] = NULL;
    }
    // asm 0000581A: 	LDI	AR7,AR3
    // asm 0000581B: 	ADDI	TEMP_DATA,AR3
    // asm 0000581C: 	LDI	0,IR0
    // asm 0000581D: 	LDI	0,AR1
    // asm 0000581E: 	LDI	7FFFh,R3
    // asm 0000581F: 	MPYI	R3,R3			;BIG number
    upper_bound = (LEG_PAYLOAD*)(uintptr_t)UINTPTR_MAX; // ;BIG number
FR0:
    pass_count = position_to_find;
    if (pass_count <= 0) {
        pass_count = 1;
    }
    for (place = 0; place < pass_count; place++) { // ;Find the next best
        // asm 00005820: 	LDI	0,R4
        // asm 00005821: 	LDI	@CAR_LISTI,AR0		;Scan the list of car objects
        best_road_obj = NULL;
        chosen_obj = NULL; // ;Scan the list of car objects
        for (car_obj = CAR_LIST; car_obj != NULL; car_obj = (OBJ*)car_obj->link3) { // ;They are linked by OLINK3
            // asm 00005826: 	LDI	*+AR0(OID),R0			;Is this a racer?
            // asm 00005827: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RACER,R0
            if (car_obj->id != (DRONE_C | VEHICLE_T | DRNE_RACER)) { // ;Is this a racer?
                continue;
            }
            // asm 00005829: 	CALL	GET_MAP_CARBLK
            road_obj = GET_MAP_CARBLK(car_obj);
            if (road_obj == NULL) {
                continue;
            }
            // asm 0000582A: 	CMPI	AR4,R3			;Best position looking for
            if (road_obj > upper_bound) { // ;Best position looking for
                continue;
            }
            // asm 0000582C: 	CMPI	AR4,R4			;Greatest entry in the LEG_MAP wins
            if (best_road_obj != NULL && road_obj < best_road_obj) { // ;Greatest entry in the LEG_MAP wins
                continue;
            }
            // asm 0000582E: 	LDI	IR0,IR1			;WHILE ((IR0>0) && (*+AR3(IR1++) != AR0))
            duplicate = 0;
            for (i = place - 1; i >= 0; i--) {
                // asm 00005832: 	CMPI	*+AR3(IR1),AR0		;IF THIS CAR IS ALLREADY ON LIST, Then Get NEXT
                if (chosen_cars[i] == car_obj) { // ;IF THIS CAR IS ALLREADY ON LIST, Then Get NEXT
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate != 0) {
                continue;
            }
            // asm 00005835: 	LDI	AR4,R4
            // asm 00005836: 	LDI	AR0,AR1			;Set the pointer to the cars obj
            best_road_obj = road_obj;
            chosen_obj = car_obj; // ;Set the pointer to the cars obj
        }
        // asm 00005838: 	STI	AR1,*+AR3(IR0)
        chosen_cars[place] = chosen_obj;
        // asm 00005839: 	ADDI	1,IR0
        // asm 0000583A: 	LDI	R4,R3
        upper_bound = best_road_obj;
        // asm 0000583B: 	SUBI	1,R2
        if (chosen_obj == NULL) {
            break;
        }
    }
    // asm 0000583D: 	CMPI	0,AR1
    // asm 0000583E: 	BNE	FRXX
    // asm 0000583F: 	LOCKUP				;No RACERS?
    SLOCKON(chosen_cars[pass_count - 1] == NULL, "FIND_RACER");
FRXX:
    // asm 00005841: 	LDI	AR1,AR4
    // asm 00005842: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_RACER", 0, 0);
    return chosen_cars[pass_count - 1];
}

/*
 *----------------------------------------------------------------------------
 *This algorithm is based on a single pass bubble sort
 *OUTPUT
 *	R0	=	Distance to the closest Racer Drone
 *	AR1	=	Pointer to the Closest Racer Drone in front of the camera
 *	AR1	=	0, If no Racer Drones in front of the camera
 *
 */

static c3x_reg_t FIND_CLOSEST_RACER(OBJ** closest_racer /*AR1*/) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    OBJ* car_obj;
    c3x_reg_t camera_rady_offset;
    c3x_reg_t best_distance;
    c3x_reg_t road_angle;
    c3x_reg_t dx;
    c3x_reg_t dz;
    c3x_reg_t angle_delta;
    c3x_reg_t distance;
    c3x_reg_t relative_angle;

    // asm 00005843: 	FLOAT	30000,R5
    // asm 00005844: 	MPYF	R5,R5
    // asm 00005845: 	MPYF	R5,R5
    // asm 00005846: 	MPYF	R5,R5
    // asm 00005847: 	MPYF	R5,R5		;BIG F'N NUMBER
    best_distance = C3X_FROM_INT(30000);
    C3X_MPYF(best_distance, best_distance);
    C3X_MPYF(best_distance, best_distance);
    C3X_MPYF(best_distance, best_distance);
    C3X_MPYF(best_distance, best_distance); // ;BIG F'N NUMBER
    // asm 00005848: 	LDI	@CAR_LISTI,AR0		;Scane the linked list of cars
    // asm 00005849: 	SUBI	OLINK3,AR0
    // asm 0000584A: 	LDI	0,AR1
    *closest_racer = NULL;
    memcpy(&camera_rady_offset, &ctx->ATTRACT_DELTA.camera_xyzr[3], sizeof(camera_rady_offset));
FCR1:
    for (car_obj = CAR_LIST; car_obj != NULL; car_obj = (OBJ*)car_obj->link3) {
        // asm 0000584E: 	LDI	*+AR0(OID),R0
        // asm 0000584F: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RACER,R0	;Racer drones only
        if (car_obj->id != (DRONE_C | VEHICLE_T | DRNE_RACER)) { // ;Racer drones only
            continue;
        }
        // asm 00005851: 	LDF	*+AR0(OPOSZ),R2
        // asm 00005852: 	SUBF	*+AR7(NEW_POSZ),R2
        // asm 00005853: 	LDF	*+AR0(OPOSX),R3		;Get the direction of the car relative to
        // asm 00005854: 	SUBF	*+AR7(NEW_POSX),R3	;the direction the camera is moving in.
        dz = C3X_SUB(car_obj->pos.Z, ctx->ATTRACT_DELTA.new_posz);
        dx = C3X_SUB(car_obj->pos.X, ctx->ATTRACT_DELTA.new_posx); // ;Get the direction of the car relative to / ;the direction the camera is moving in.
        // asm 00005855: 	CALL	ARCTANF
        // asm 00005856: 	CALL	NORM_VECTOR
        road_angle = NORM_VECTOR(ARCTANF(dz, dx));
        // asm 00005857: 	SUBF	*+AR7(NEW_RADY),R0	;DEG from camera
        // asm 00005858: 	CALL	NORM_VECTOR
        relative_angle = NORM_VECTOR(C3X_SUB(road_angle, ctx->ATTRACT_DELTA.new_rady)); // ;DEG from camera
        // asm 00005859: 	SUBF	*+AR7(CAMERA_XYZR+3),R0	;LOOSE the Offset rad for viewing
        // asm 0000585A: 	CALL	NORM_VECTOR
        angle_delta = C3X_ABS(NORM_VECTOR(C3X_SUB(relative_angle, camera_rady_offset))); // ;LOOSE the Offset rad for viewing
        // asm 0000585B: 	ABSF	R0
        // asm 0000585C: 	CMPF	0.78,R0			;Within +/- 45deg?
        // asm 0000585D: 	BGT	FCR1			;Nop this car is beside us or behind us
        if (C3X_GT(angle_delta, C3X_IMM_F32(0.78))) { // ;Within +/- 45deg? / ;Nop this car is beside us or behind us
            continue;
        }
        // asm 0000585E: 	LDF	*+AR0(OPOSX),R0		;Now compute the distance to the car
        // asm 0000585F: 	SUBF	*+AR7(NEW_POSX),R0	;D = SQRT((X2-X1)^2+(Y2-Y1)^2)
        // asm 00005860: 	MPYF	R0,R0
        // asm 00005861: 	LDF	*+AR0(OPOSZ),R1
        // asm 00005862: 	SUBF	*+AR7(NEW_POSZ),R1
        // asm 00005863: 	MPYF	R1,R1
        // asm 00005864: 	ADDF	R0,R1,R2
        // asm 00005865: 	CALL	SQRT
        distance = SQRT(C3X_ADD(C3X_MUL(dx, dx), C3X_MUL(dz, dz))); // ;Now compute the distance to the car / ;D = SQRT((X2-X1)^2+(Y2-Y1)^2)
        // asm 00005866: 	CMPF	R0,R5
        // asm 00005867: 	LDFGT	R0,R5
        // asm 00005868: 	LDIGT	AR0,AR1
        if (C3X_LT(distance, best_distance)) {
            best_distance = distance;
            *closest_racer = car_obj;
        }
    }
FCRX:
    // asm 0000586A: 	LDF	R5,R0
    // asm 0000586B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_CLOSEST_RACER", 0, 0);
    return best_distance;
}

/*
 *---------------------------------------------------------------------------
 *
 *GET OBJECT POINTER TO ROAD SEG ID IN R2
 *INPUTS  R2=OUSR1
 *OUPTUTS AR2=POINTER TO OBJECT
 *	  AR2=0 not found
 */
static OBJ* FIND_TRACK(uint32_t road_id /*R2*/) {
    OBJ* road_obj;

    // asm 0000586C: 	LDI	@DYNALIST_TRUEBEGIN,AR2
FDL:
    for (road_obj = DYNALIST_TRUEBEGIN; road_obj != NULL; road_obj = (OBJ*)road_obj->link4) {
        // asm 0000586D: 	LDI	*+AR2(OUSR1),R0
        // asm 0000586E: 	CMPI	R0,R2
        // asm 0000586F: 	BZ	FDLX
        if ((uint32_t)road_obj->usr1 == road_id) {
            break;
        }
        // asm 00005870: 	LDI	*+AR2(OLINK4),AR2
        // asm 00005871: 	CMPI	0,AR2			;CHECK END OF THE LINE
        // asm 00005872: 	BNZ	FDL
    }
FDLX:
    // asm 00005873: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_TRACK", 0, 0);
    return road_obj;
}

/*
 *---------------------------------------------------------------------------
 *
 *GET POINTER TO MAP_LEG, FOR THE CAR IN AR4
 *INPUTS  AR0=POINTER TO CARBLK
 *OUPTUTS AR4=POINTER TO place in LEG_MAP
 *SETS CAROBJ IF GET_MAP_SEGMENT is called
 */

static LEG_PAYLOAD* GET_MAP_CARBLK(OBJ* car_obj /*AR0*/) {
    PROC* proc;
    LEG_PAYLOAD* road_obj;
    CARBLK* carblk;

    // asm 00005874: 	PUSH	R2
    // asm 00005875: 	LDI	*+AR0(OPLINK),AR2
    // asm 00005876: 	LDI	*+AR2(DELTA_SPTR),AR4
    proc = car_obj != NULL ? car_obj->plink : NULL;
    road_obj = (proc != NULL && proc->ctx != NULL) ? (LEG_PAYLOAD*)proc->ctx->RACER_DRONE.delta_sptr : NULL;
    // asm 00005877: 	LDI	*+AR2(STEALTHMODE),R0	;ARE YOU A STEALTHY BITCH?
    // asm 00005878: 	BNZ	GMSX
    if (proc == NULL || proc->ctx == NULL || proc->ctx->RACER_DRONE.stealthmode == 0) { // ;ARE YOU A STEALTHY BITCH?
        // asm 00005879: 	LDI	*+AR0(OCARBLK),AR4
        // asm 0000587A: 	LDI	*+AR4(CARTRAK),AR4	;Get the track piece the car is on
        carblk = car_obj != NULL ? car_obj->carblk : NULL;
        // asm 0000587B: 	LDI	*+AR4(OUSR1),R2
        // asm 0000587C: 	CALL	FIND_MAP
        road_obj = (carblk != NULL && carblk->closest_track_piece != 0) ? FIND_MAP((uint32_t)OBJREF_TO_PTR(carblk->closest_track_piece)->usr1) : NULL; // ;Get the track piece the car is on
    }
GMSX:
    // asm 0000587D: 	POP	R2
    // asm 0000587E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_MAP_CARBLK", 0, 0);
    return road_obj;
}

/*
 *---------------------------------------------------------------------------
 *
 *GET POINTER TO MAP_LEG, ID IN R2
 *INPUTS  R2=OUSR1
 *OUPTUTS AR4=POINTER TO place in LEG_MAP
 */

static LEG_PAYLOAD* FIND_MAP(uint32_t road_id /*R2*/) {
    LEG_PAYLOAD* road_obj;

    // asm 0000587F: 	LDI	@LEG_MAPI,AR4		;FIND THE POSITION IN THE MAP
    // asm 00005880: 	ADDI	3,AR4				;OFFSET TO ID
    road_obj = LEG_MAP; // ;FIND THE POSITION IN THE MAP / ;OFFSET TO ID
FIND_LP:
    // asm 00005881: 	LDI	*AR4++(4),R0
    // asm 00005882: 	CMPI	R2,R0
    // asm 00005883: 	BLT	FIND_LP
    while (road_obj->as_fixed.id < road_id) {
        road_obj++;
    }
    // asm 00005884: FINDX
    // asm 00005884: 	SUBI	7,AR4
    // asm 00005885: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_MAP", 0, 0);
    return road_obj;
}

static void INITVIEW1_VIEW(void) {
    // asm 00005886: 	LDI	0,R1
    // asm 00005887: 	STI	R0,*+AR7(CAMERA_VIEW)
    // asm 00005888: 	LDI	@PLYCAR,AR4
    // asm 00005889: 	LDF	0,R0
    // asm 0000588A: 	FLOAT	-150,R1
    // asm 0000588B: 	FLOAT	0,R2
    // asm 0000588C: 	LDF	0,R3
    // asm 0000588D: 	BR	SET_SMOOTH_VIEW
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITVIEW1_VIEW", 0, 0);
    UNIMPL();
}

static void CUT_TO_VIEW2(void) {
    // asm 0000588E: 	LDI	1,R0			;Don't smooth while spinning
    // asm 0000588F: 	STI	R0,*+AR7(CUT_PAN)
    // asm 00005890: 	LDI	4,R2		;FOLOW 3rd place
    // asm 00005891: 	CALL	FIND_RACER
    // asm 00005892: 	STI	AR4,@PLYCAR
    // asm 00005893: 	LDI	*+AR4(OCARBLK),AR0
    // asm 00005894: 	STI	AR0,@PLYCBLK
    // asm 00005895: 	CALL	INITVIEW2_VIEW
    // asm 00005896: 	CALL	UPDATE_CAMERA
    // asm 00005897: 	CALL	RESCAN
    // asm 00005898: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CUT_TO_VIEW2", 0, 0);
    UNIMPL();
}

static void INITVIEW2_VIEW(void) {
    // asm 00005899: 	LDI	@PLYCAR,AR4
    // asm 0000589A: 	LDI	1,R0
    // asm 0000589B: 	STI	R0,*+AR7(CAMERA_VIEW)
    // asm 0000589C: 	LDF	0,R0
    // asm 0000589D: 	FLOAT	-400,R1
    // asm 0000589E: 	FLOAT	-2200,R2
    // asm 0000589F: 	LDF	0,R3
    // asm 000058A0: 	BR	SET_SMOOTH_VIEW
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITVIEW2_VIEW", 0, 0);
    UNIMPL();
}

static void INITVIEW3_VIEW(void) {
    // asm 000058A1: 	LDI	BUT_VIEW3,R1
    // asm 000058A2: 	STI	R0,*+AR7(CAMERA_VIEW)
    // asm 000058A3: 	LDF	0,R0
    // asm 000058A4: 	FLOAT	-900,R1
    // asm 000058A5: 	FLOAT	-3840,R2
    // asm 000058A6: 	LDF	0,R3
    // asm 000058A7: 	BR	SET_SMOOTH_VIEW
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITVIEW3_VIEW", 0, 0);
    UNIMPL();
}

static void FACE_FRONT(void) {
    // asm 000058A8: 	PUSH	AR5
    // asm 000058A9: 	LDI	*+AR4(OCARBLK),AR5
    // asm 000058AA: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF CAR IS DISPLAYED
    // asm 000058AB: 	TSTB	O_LIST_M,R0
    // asm 000058AC: 	BNZ	SECOND_PERSON
    // asm 000058AD: FIRST_PERSON				;NO, Then in first person
    // asm 000058AD: 	LDI	0,R0
    // asm 000058AE: 	STI	R0,@CAMVIEW
    // asm 000058AF: 	LDI	AR7,AR2
    // asm 000058B0: 	ADDI	NEW_MATRIX,AR2
    // asm 000058B1: 	LDI	AR4,R2
    // asm 000058B2: 	ADDI	OMATRIX,R2
    // asm 000058B3: 	CALL	CPYIMAT			;Copy the cars matrix into NEW_MATRIX
    // asm 000058B4: 	LDF	0,R0
    // asm 000058B5: 	STF	R0,*+AR5(CARROT)	;NO OVERROTATION IN CORNER
    // asm 000058B6: 	LDF	0,R0
    // asm 000058B7: 	STF	R0,*+AR7(NEW_RADY)		;NOTE this is not used for first person
    // asm 000058B8: 	LDF	*+AR5(CARXLEAN),R0
    // asm 000058B9: 	MPYF	-0.7,R0			;NEGATE AND SCALE
    // asm 000058BA: 	SUBF	*+AR7(NEW_RADX),R0
    // asm 000058BB: 	LDF	0.20,R2
    // asm 000058BC: 	CALL	SMOOTH_VECTOR
    // asm 000058BD: 	ADDF	*+AR7(NEW_RADX),R0
    // asm 000058BE: 	STF	R0,*+AR7(NEW_RADX)		;NOTE this is not used for first person
    // asm 000058BF: 	LDF	*+AR5(CARZLEAN),R0  	;GET Z LEAN FACTOR
    // asm 000058C0: 	MPYF	-0.5,R0			;NEGATE AND SCALE
    // asm 000058C1: 	STF	R0,*+AR7(NEW_RADZ)
    // asm 000058C2: 	LDI	@MATRIXCI,AR2
    // asm 000058C3: 	LDI	AR7,R2
    // asm 000058C4: 	ADDI	NEW_RADX,R2
    // asm 000058C5: 	CALL	FIND_MATRIX
    // asm 000058C6: 	LDI	AR7,AR2
    // asm 000058C7: 	ADDI	NEW_MATRIX,AR2
    // asm 000058C8: 	LDI	AR2,R3
    // asm 000058C9: 	LDI	@MATRIXCI,R2
    // asm 000058CA: 	CALL	CONCATMAT
    // asm 000058CB: 	NEGF	*+AR5(CARYROT),R0	;SAVE this for switching out of first person
    // asm 000058CC: 	STF	R0,*+AR7(NEW_RADY)		;NOTE this is not used for first person
    // asm 000058CD: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm 000058CE: 	BZ	FFX
    // asm 000058CF: 	LDI	1,R0			;Don't smooth while spinning
    // asm 000058D0: 	STI	R0,*+AR7(CUT_PAN)
    // asm 000058D1: 	BR	FFX
SECOND_PERSON:
    // asm 000058D2: 	LDI	1,R0
    // asm 000058D3: 	STI	R0,@CAMVIEW
    // asm 000058D4: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm 000058D5: 	BNZ	FFX			;YES, DONT mess WITH CAMERA ANGLE
    // asm 000058D6: 	LDF	*+AR4(OVELX),R3
    // asm 000058D7: 	LDF	*+AR4(OVELZ),R2
    // asm 000058D8: 	ADDF3	R3,R2,R1
    // asm 000058D9: 	CMPF	0,R1
    // asm 000058DA: 	BNE	FFX1
    // asm 000058DB: 	NEGF	*+AR5(CARYROT),R0	;If stopped don't use the velocity
    // asm 000058DC: 	BR	FFX2
FFX1:
    // asm 000058DD: 	CALL	ARCTANF
FFX2:
    // asm 000058DE: 	CALL	NORM_VECTOR
    // asm 000058DF: 	ADDF	*+AR7(CAMERA_XYZR+3),R0
    // asm 000058E0: 	LDI	*+AR7(CUT_PAN),R2
    // asm 000058E1: 	CMPI	0,R2
    // asm 000058E2: 	BNE	FFX3				;NO smoothing in a cut
    // asm 000058E3: 	SUBF	*+AR7(NEW_RADY),R0
    // asm 000058E4: 	LDF	0.10,R2
    // ;	LDF	0.20,R2
    // asm 000058E5: 	CALL	SMOOTH_VECTOR
    // asm 000058E6: 	ADDF	*+AR7(NEW_RADY),R0
FFX3:
    // asm 000058E7: 	STF	R0,*+AR7(NEW_RADY)
    // asm 000058E8: 	LDF	0,R0
    // asm 000058E9: 	STF	R0,*+AR7(NEW_RADX)
    // asm 000058EA: 	STF	R0,*+AR7(NEW_RADZ)
    // asm 000058EB: FFX4
    // asm 000058EB: 	LDI	AR7,R2
    // asm 000058EC: 	ADDI	NEW_RADX,R2
    // asm 000058ED: 	LDI	AR7,AR2
    // asm 000058EE: 	ADDI	NEW_MATRIX,AR2
    // asm 000058EF: 	CALL	FIND_MATRIX
    // asm 000058F0: 	LDI	AR7,AR0
    // asm 000058F1: 	ADDI	NEW_POSX,AR0
    // asm 000058F2: 	CALL	CAMYADJ			;MAKE sure the camera doesn't go below the road
FFX:
    // asm 000058F3: 	POP	AR5
    // asm 000058F4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FACE_FRONT", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *Does averaging
 *INPUT
 *		R2 = average	0.20
 *		R0 = difference between the two angles
 *OUTPUT
 *		R0 = SMOOTHED difference
 */
static c3x_reg_t SMOOTH_VECTOR(c3x_reg_t radians /*R0*/, c3x_reg_t smoothing /*R2*/) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    CARBLK* carblk;
    c3x_reg_t unsmoothed;

    // asm 000058F5: 	CALL	NORM_VECTOR
    radians = NORM_VECTOR(radians);
    // asm 000058F6: 	LDI	*+AR5(CAR_SPIN),R3	;SPINNING?
    carblk = ctx->ATTRACT_DELTA.carobj != NULL ? ctx->ATTRACT_DELTA.carobj->carblk : NULL;
    // asm 000058F7: 	CMPI	1,R3
    // asm 000058F8: 	BEQ	SV1			;Yes then don't smooth
    if (carblk == NULL || carblk->spin_flag != 1) {
    // asm 000058F9: 	LDF	R0,R3		;Save of the non smoothed value
        unsmoothed = radians; // ;Save of the non smoothed value
    // asm 000058FA: 	MPYF	R2,R0	    		;ANGLE SMOOTHING
        radians = C3X_MUL(radians, smoothing); // ;ANGLE SMOOTHING
    // asm 000058FB: 	ABSF	R3,R2
        smoothing = C3X_ABS(unsmoothed);
    // asm 000058FC: 	CMPF	0.78,R2 	;Is this within 45 degrees?
    // asm 000058FD: 	BLT	SVX
        if (C3X_GE(smoothing, C3X_IMM_F32(0.78))) { // ;Is this within 45 degrees?
    // asm 000058FE: 	LDF	R3,R0		;NO, Then don't smooth the pig (PROBABLY JUST CHANGED CAMERA ANGLE)
            radians = unsmoothed; // ;NO, Then don't smooth the pig (PROBABLY JUST CHANGED CAMERA ANGLE)
            goto SV1;
        }
    } else {
SV1:
    // asm 000058FF: 	LDI	1,R3
    // asm 00005900: 	STI	R3,*+AR7(CUT_PAN)	;This is a cut, Don't smooth the matrix
        ctx->ATTRACT_DELTA.cut_pan = 1; // ;This is a cut, Don't smooth the matrix
    }
SVX:
    // asm 00005901: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOOTH_VECTOR", 0, 0);
    return radians;
}

static void SERIOUSLY_NORMALIZE(void) {
    // asm 00005902: 	ABSF	R0,R1
    // asm 00005903: 	CMPF	@TWOPII,R1
    // asm 00005904: 	BLT	NORM_VECTOR
    // asm 00005905: 	LDF	@TWOPII,R1
    // asm 00005906: 	PUSHF	R0
    // asm 00005907: 	CALL	DIV_F
    // asm 00005908: 	FIX	R0,R1
    // asm 00005909: 	FLOAT	R1
    // asm 0000590A: 	MPYF	@TWOPII,R1		;MOD 6.28
    // asm 0000590B: 	POPF	R0
    // asm 0000590C: 	SUBF	R1,R0
NORM_VECTOR:
    // asm 0000590D: 	LDF	0,R1 			;NORMALIZE DIFFERENCE
    // asm 0000590E: 	CMPF	3.14,R0
    // asm 0000590F: 	LDFGT	-6.28,R1
    // asm 00005910: 	CMPF	-3.14,R0
    // asm 00005911: 	LDFLT	6.28,R1
    // asm 00005912: 	ADDF	R1,R0
    // asm 00005913: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SERIOUSLY_NORMALIZE", 0, 0);
    UNIMPL();
}

static c3x_reg_t NORM_VECTOR(c3x_reg_t radians /*R0*/) {
    c3x_reg_t offset;

    // asm 0000590D: 	LDF	0,R1 			;NORMALIZE DIFFERENCE
    offset = C3X_FROM_INT(0); // ;NORMALIZE DIFFERENCE
    // asm 0000590E: 	CMPF	3.14,R0
    // asm 0000590F: 	LDFGT	-6.28,R1
    if (C3X_GT(radians, C3X_IMM_F32(3.14))) {
        offset = C3X_NEG(C3X_IMM_F32(6.28));
    }
    // asm 00005910: 	CMPF	-3.14,R0
    // asm 00005911: 	LDFLT	6.28,R1
    if (C3X_LT(radians, C3X_NEG(C3X_IMM_F32(3.14)))) {
        offset = C3X_IMM_F32(6.28);
    }
    // asm 00005912: 	ADDF	R1,R0
    radians = C3X_ADD(radians, offset);
    // asm 00005913: 	RETS
    return radians;
}

// *----------------------------------------------------------------------------

static void SET_SMOOTH_VIEW(void) {
    // asm 00005914: 	LDI	0,AR5			;Set AR5 for init
    // asm 00005915: 	STF	R0,*+AR7(NEWCAMERA_XYZR)
    // asm 00005916: 	STF	R1,*+AR7(NEWCAMERA_XYZR+1)
    // asm 00005917: 	STF	R2,*+AR7(NEWCAMERA_XYZR+2)
    // asm 00005918: 	STF	R3,*+AR7(NEWCAMERA_XYZR+3)
    // asm 00005919: 	LDF	0,R4
    // asm 0000591A: 	STF	R4,*+AR7(ZOOMVEL)
    // asm 0000591B: 	LDI	*+AR7(CUT_PAN),R4
    // asm 0000591C: 	BEQ	SMOOTH_VIEW
    // ;DO A CUT
    // asm 0000591D: 	STF	R0,*+AR7(CAMERA_XYZR)
    // asm 0000591E: 	STF	R1,*+AR7(CAMERA_XYZR+1)
    // asm 0000591F: 	STF	R2,*+AR7(CAMERA_XYZR+2)
    // asm 00005920: 	STF	R3,*+AR7(CAMERA_XYZR+3)
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_SMOOTH_VIEW", 0, 0);
    UNIMPL();
}

static void SMOOTH_VIEW(void) {
    // asm 00005921: 	CALL	ZOOM_CAMERA
    // asm 00005922: 	CALL	FACE_FRONT
    // asm 00005923: 	LDI	AR7,AR2
    // asm 00005924: 	ADDI	CAMERA_XYZR,AR2
    // asm 00005925: 	LDI	AR7,R2
    // asm 00005926: 	ADDI	NEW_MATRIX,R2
    // asm 00005927: 	LDI	AR7,R3
    // asm 00005928: 	ADDI	NEW_POSX,R3
    // asm 00005929: 	CALL	MATRIX_MUL
    // asm 0000592A: 	LDF	*+AR4(OPOSX),R0
    // asm 0000592B: 	SUBF	*+AR7(NEW_POSX),R0
    // asm 0000592C: 	STF	R0,*+AR7(NEW_POSX)
    // asm 0000592D: 	LDF	*+AR4(OPOSY),R0
    // asm 0000592E: 	ADDF	*+AR7(NEW_POSY),R0
    // asm 0000592F: 	STF	R0,*+AR7(NEW_POSY)
    // asm 00005930: 	LDF	*+AR4(OPOSZ),R0
    // asm 00005931: 	ADDF	*+AR7(NEW_POSZ),R0
    // asm 00005932: 	STF	R0,*+AR7(NEW_POSZ)
    // asm 00005933: 	LDI	*+AR4(OCARBLK),AR0
    // asm 00005934: 	LDI	*+AR0(CARTRAK),AR0
    // asm 00005935: 	LDI	*+AR0(OUSR1),R1
    // asm 00005936: 	CMPI	AR5,R1
    // asm 00005937: 	RETSLT
    // asm 00005938: 	LDI	0,AR5
    // ;Put car back if it was removed
    // asm 00005939: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY ON LIST
    // asm 0000593A: 	TSTB	O_LIST_M,R0
    // asm 0000593B: 	BNZ	SMOOTH_VIEWX			;YES, DONT INSERT
    // asm 0000593C: 	STI	AR4,*+AR7(OBJINS)
    // ;	LDI	AR4,AR2
    // ;	CALL	OBJ_INSERT			;INSERT PLAYER OBJECT
SMOOTH_VIEWX:
    // asm 0000593D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOOTH_VIEW", 0, 0);
    UNIMPL();
}

/* asm: ZOOMACCEL	.float	0.006 */
/* asm: 	 */
static c3x_reg_t ZOOMACCEL = C3X_INIT(0.006f, 0xF8449BA5E3ull);

static void ZOOM_CAMERA(void) {
    // asm 0000593F: 	LDI	*+AR7(CUT_PAN),R0
    // asm 00005940: 	BNE	ZOOMX
    // asm 00005941: 	LDF	*+AR7(ZOOMVEL),R0
    // asm 00005942: 	ADDF	@ZOOMACCEL,R0
    // asm 00005943: 	CMPF	0.2,R0
    // asm 00005944: 	LDFGT	0.2,R0
    // asm 00005945: 	STF	R0,*+AR7(ZOOMVEL)
    // asm 00005946: 	LDI	AR7,AR0
    // asm 00005947: 	ADDI	CAMERA_XYZR,AR0
    // asm 00005948: 	LDI	AR7,AR1
    // asm 00005949: 	ADDI	NEWCAMERA_XYZR,AR1
    // asm 0000594A: 	LDI	3,RC
    // asm 0000594B: 	RPTB	ZOOMLP
    // asm 0000594C: 	LDF	*AR1++,R2
    // asm 0000594D: 	SUBF	*AR0,R2
    // asm 0000594E: 	MPYF	R0,R2
    // asm 0000594F: 	ADDF	*AR0,R2
ZOOMLP:
    // asm 00005950: STF	R2,*AR0++
    // asm 00005951: 	LDI	*+AR7(CAMERA_VIEW),R0
    // asm 00005952: 	CMPI	0,R0
    // asm 00005953: 	BNE	ZOOM1
    // asm 00005954: 	ABSF	*+AR7(CAMERA_XYZR+2),R0
    // asm 00005955: 	CMPF	100,R0
    // asm 00005956: 	BGT	ZOOMX
    // asm 00005957: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY OFF LIST
    // asm 00005958: 	TSTB	O_LIST_M,R0
    // asm 00005959: 	BZ	ZOOMX			;YES, DONT DELETE
    // asm 0000595A: 	LDI	AR4,AR2		;IN first person
    // asm 0000595B: 	CALL	OBJ_PULL
    // asm 0000595C: 	BR	ZOOMX
ZOOM1:
    // asm 0000595D: 	ABSF	*+AR7(CAMERA_XYZR+2),R0
    // asm 0000595E: 	CMPF	100,R0
    // asm 0000595F: 	BLT	ZOOMX
    // asm 00005960: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY ON LIST
    // asm 00005961: 	TSTB	O_LIST_M,R0
    // asm 00005962: 	BNZ	ZOOMX			;YES, DONT INSERT
    // asm 00005963: 	LDI	AR4,AR2
    // asm 00005964: 	CALL	OBJ_INSERT			;INSERT PLAYER OBJECT
    // asm 00005965: 	LDI	1,R0
    // asm 00005966: 	STI	R0,*+AR7(CUT_PAN)
ZOOMX:
    // asm 00005967: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOM_CAMERA", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 * UPDATE_CAMERA
 *	INPUT	NEW_POSx/y/z,NEW_RADx/y/z,NEW_MATRIX
 *	OUTPUT	_CAMERAPOS,_CAMERAMATRIX,_CAMERARAD
 *
 */
static void UPDATE_CAMERA(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;
    c3x_f32_t* dst;
    c3x_f32_t* src;
    int i;

    // asm 00005968: 	LDI	@_CAMERAPOSI,AR0
    // asm 00005969: 	LDF	*+AR7(NEW_POSX),R0
    // asm 0000596A: 	STF	R0,*AR0
    _CAMERAPOS.X = ctx->ATTRACT_DELTA.new_posx;
    // asm 0000596B: 	LDF	*+AR7(NEW_POSY),R0
    // asm 0000596C: 	STF	R0,*+AR0(Y)
    _CAMERAPOS.Y = ctx->ATTRACT_DELTA.new_posy;
    // asm 0000596D: 	LDF	*+AR7(NEW_POSZ),R0
    // asm 0000596E: 	STF	R0,*+AR0(Z)
    _CAMERAPOS.Z = ctx->ATTRACT_DELTA.new_posz;
    // asm 0000596F: 	LDP	@_CAMERARAD
    // asm 00005970: 	LDF	*+AR7(NEW_RADX),R0
    // asm 00005971: 	STF	R0,@_CAMERARAD
    _CAMERARAD.X = ctx->ATTRACT_DELTA.new_radx;
    // asm 00005972: 	LDF	*+AR7(NEW_RADY),R0
    // asm 00005973: 	STF	R0,@_CAMERARAD+Y
    _CAMERARAD.Y = ctx->ATTRACT_DELTA.new_rady;
    // asm 00005974: 	LDF	*+AR7(NEW_RADZ),R0
    // asm 00005975: 	STF	R0,@_CAMERARAD+Z
    _CAMERARAD.Z = ctx->ATTRACT_DELTA.new_radz;
    // asm 00005976: 	SETDP
    // asm 00005977: 	LDI	@CAMERAMATRIXI,AR0
    // asm 00005978: 	LDI	AR7,AR1
    // asm 00005979: 	ADDI	NEW_MATRIX,AR1
    // asm 0000597A: 	LDI	*+AR7(CUT_PAN),R0
    // asm 0000597B: 	BEQ	SMOOTH_MATRIX
    dst = (c3x_f32_t*)&_CAMERAMATRIX;
    src = (c3x_f32_t*)&ctx->ATTRACT_DELTA.new_matrix;
    if (ctx->ATTRACT_DELTA.cut_pan != 0) {
        // asm 0000597C: 	LDI	8,RC
        // asm 0000597D: 	RPTB	loopA
        // asm 0000597E: 	LDF	*AR1++,R1
        // asm 0000597F: STF	R1,*AR0++
        for (i = 0; i < 9; i++) {
            dst[i] = src[i];
        }
        // asm 00005980: 	BR	UPCAMX
        goto UPCAMX;
    }
SMOOTH_MATRIX:
    // asm 00005981: 	LDI	8,RC
    // asm 00005982: 	RPTB	loop
    // asm 00005983: 	LDF	*AR0,R0
    // asm 00005984: 	MPYF	0.20,R0
    // asm 00005985: 	LDF	*AR1++,R1
    // asm 00005986: 	MPYF	0.80,R1
    // asm 00005987: 	ADDF	R0,R1
    // asm 00005988: STF	R1,*AR0++
    for (i = 0; i < 9; i++) {
        dst[i] = C3X_STF(C3X_ADD(C3X_MUL(dst[i], C3X_IMM_F32(0.20)), C3X_MUL(src[i], C3X_IMM_F32(0.80))));
    }
UPCAMX:
    // asm 00005989: 	CALL	CAMERA_HORIZON_PROJECTION
    CAMERA_HORIZON_PROJECTION();
    // asm 0000598A: 	LDI	0,R0
    // asm 0000598B: 	STI	R0,*+AR7(CUT_PAN)		;Ok to smooth after this
    ctx->ATTRACT_DELTA.cut_pan = 0; // ;Ok to smooth after this
    // asm 0000598C: 	RETS
    // ;*----------------------------------------------------------------------------
    // ;* MOVE_CAMERA
    // ;*
    // ;*	INPUT CAMERA_SPEED,CAMERA_VEC
    // ;*
    // ;
    // ;MOVE_CAMERA
    // ;	LDF	@NEW_CAMERA+CAMERA_SPEED,R0
    // ;	BEQ	no_movment
    // ;	LDI	@NEW_CAMERAI,R2
    // ;	ADDI	CAMERA_VECX,R2
    // ;	LDI	@MATRIXAI,AR2
    // ;	CALL	FIND_MATRIX
    // ;
    // ;	LDF	0,R0
    // ;	STF	R0,@VECTORA
    // ;	STF	R0,@VECTORA+1
    // ;	LDF	@NEW_CAMERA+CAMERA_SPEED,R0
    // ;	STF	R0,@VECTORA+2
    // ;
    // ;	LDI	@MATRIXAI,R2
    // ;	LDI	@VECTORAI,AR2
    // ;	LDI	AR2,R3
    // ;	CALL	MATRIX_MUL
    // ;
    // ;	LDF	@VECTORA,R0
    // ;	ADDF	@NEW_CAMERA+CAMERAX,R0
    // ;	STF	R0,@NEW_CAMERA+CAMERAX
    // ;
    // ;	LDF	@VECTORA+1,R0
    // ;	ADDF	@NEW_CAMERA+CAMERAY,R0
    // ;	STF	R0,@NEW_CAMERA+CAMERAY
    // ;
    // ;	LDF	@VECTORA+2,R0
    // ;	ADDF	@NEW_CAMERA+CAMERAZ,R0
    // ;	STF	R0,@NEW_CAMERA+CAMERAZ
    // ;no_movment
    // ;	RETS
    // ;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UPDATE_CAMERA", 0, 0);
}

// *----------------------------------------------------------------------------
static void CAMERA_HORIZON_PROJECTION(void) {
    PROC* p = CURRENT_PROC;
    PROC_CONTEXT* ctx = p->ctx;

    // asm 0000598D: 	LDF	*+AR7(NEW_RADX),R2
    // asm 0000598E: 	CALL	_SINE
    // asm 0000598F: 	FLOAT	512,R1
    // asm 00005990: 	MPYF	R1,R0
    // asm 00005991: 	ADDF	*+AR7(CAMERA_INFIN),R0
    // asm 00005992: 	STF	R0,@INFIN_CORRECT
    INFIN_CORRECT = C3X_ADD(C3X_MUL(_SINE(C3X_LDF(ctx->ATTRACT_DELTA.new_radx)), C3X_FROM_INT(512)), ctx->ATTRACT_DELTA.camera_infin);
    // asm 00005993: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMERA_HORIZON_PROJECTION", 0, 0);
}

/* asm: ATTR_WAVETAB */
/* asm: 	.word	0,0 */
/* asm: 	.word	L_LEG5_BEGIN+1,4 */
/* asm: 	.word	L_LEG9_BEGIN+1,8 */
/* asm: 	.word	L_LEG11_BEGIN+1,10 */
/* asm: 	.word	0,0 */
/* asm: 	.word	L_LEG5_BEGIN+1,4 */
/* asm: 	.word	L_LEG9_BEGIN+1,8 */
/* asm: 	.word	L_LEG11_BEGIN+1,10 */
/* asm: 	 */
static int ATTR_WAVETAB[] = {
    0,
    0,
    L_LEG5_BEGIN + 1,
    4,
    L_LEG9_BEGIN + 1,
    8,
    L_LEG11_BEGIN + 1,
    10,
    0,
    0,
    L_LEG5_BEGIN + 1,
    4,
    L_LEG9_BEGIN + 1,
    8,
    L_LEG11_BEGIN + 1,
    10,
};
#define ATTR_WAVETAB_LEN ((int)(sizeof(ATTR_WAVETAB) / sizeof(ATTR_WAVETAB[0])) - 1)

static void INIT_ATTR_LEG(PROC* p /*AR7*/) {
    PROC_CONTEXT* ctx;
    PROC* proc;
    int attr_wave_index;
    int dipram;

    // asm 00005994: 	LDI	@ATTRWAVE,R0
    // asm 00005995: 	BNE	NO_MUSIC
    if (ATTRWAVE == 0) {
        // 	;Wed Mar 8 11:02:10 1995
        // 	;
        // 	;
        // 	;if COMMP ON  && SLAVE then no music
        // 	;if COMMP ON && MASTER then send_attrsnd
        // 	;if COMMP OFF do regular
        // 	;
        // 	;
        // asm 00005996: 	LDI	@DIPRAM,R0
        dipram = DIPRAM;
        // asm 00005997: 	TSTB	DIP_COMMP,R0
        // asm 00005998: 	BNZ	DOTEST
        // asm 00005999: 	TSTB	CMDP_MASTER,R0
        // asm 0000599A: 	BNZ	NO_MUSIC
        if ((dipram & DIP_COMMP) == 0 && (dipram & CMDP_MASTER) == 0) {
            // asm 0000599B: 	CALL	SEND_ATTRSND
            SEND_ATTRSND();
        }
    DOTEST:
        // 	;
        // 	;
        // 	;
        // asm 0000599C: 	READADJ	ADJ_ATTRACT_MODE_SOUND
        // asm 0000599E: 	CMPI	0,R0
        // asm 0000599F: 	BEQ	NO_MUSIC
        if (READADJ(ADJ_ATTRACT_MODE_SOUND) != 0) {
            // asm 000059A0: 	SOND1	ATTR_THEME
            SOND1(ATTR_THEME);
        }
    }
NO_MUSIC:
    // asm 000059A2: 	LDI	MATTR|MINFIN,R0
    _MODE = MATTR | MINFIN;
    // asm 000059A3: 	STI	R0,@_MODE
    // asm 000059A4: 	LDL	press_PALETTES,AR2	;make sure these are gone
    // asm 000059A5: 	CALL	dealloc_section
    dealloc_section(press_PALETTES); // make sure these are gone
    // asm 000059A6: 	LDI	0,R0			;NO CHALLENGE RACES IN ATTRACT MODE
    // NO CHALLENGE RACES IN ATTRACT MODE
    CHALLENGE_RACE = 0;
    // asm 000059A7: 	STI	R0,@CHALLENGE_RACE
    // asm 000059A8: 	LDI	@ATTR_WAVETABI,AR1
    // asm 000059A9: 	ADDI	@ATTRWAVE,AR1
    attr_wave_index = ATTRWAVE;
    // asm 000059AA: 	LDI	*AR1++,R0
    // asm 000059AB: 	STI	R0,@NEXT_STARTUP
    NEXT_STARTUP = ATTR_WAVETAB[attr_wave_index];
    MAME_ASSERT_REG(0x000059AC, "R0", &NEXT_STARTUP);
    // asm 000059AC: 	LDI	*AR1,R1
    BONUS_WAVE = ATTR_WAVETAB[attr_wave_index + 1];
    // asm 000059AD: 	STI	R1,@BONUS_WAVE
    MAME_ASSERT_REG(0x000059AE, "R1", &BONUS_WAVE);
    // asm 000059AE: 	LDI	@BONUS_WAVE,AR0
    // asm 000059AF: 	ADDI	@FULLSETUP_TABLEI,AR0
    // asm 000059B0: 	LDI	*AR0,R0
    // asm 000059B1: 	CALLU	R0
    FULLSETUP_TABLE[BONUS_WAVE]();
    // asm 000059B2: 	LDF	@INFIN_CORRECT,R0
    p->ctx->ATTRACT_DELTA.camera_infin = C3X_STF((c3x_reg_t)INFIN_CORRECT);
    // asm 000059B3: 	STF	R0,*+AR7(CAMERA_INFIN)
    // asm 000059B4: 	CALL	OBJ_INIT
    OBJ_INIT();
    // asm 000059B5: 	CALL	INIT_DRONES	;init DRONE tracker system
    INIT_DRONES(); // init DRONE tracker system
    // asm 000059B6: 	CALL	DYNAOBJ_INIT	;init DYNAMIC OBJECTS
    DYNAOBJ_INIT(); // init DYNAMIC OBJECTS
    // asm 000059B7: 	CALL	CARB_INIT	;init CAR BLOCKS
    CARB_INIT(); // init CAR BLOCKS
    // asm 000059B8: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    INIT_RDDEBRIS(); // initialize ROAD DEBRIS list(s)
    // 	;
    // 	;RE INITIALIZE TRACK...
    // 	;
    // asm 000059B9: 	LDI	@BONUS_WAVE,AR0
    // asm 000059BA: 	ADDI	@BONUS_POSTLAUNCHI,AR0
    // asm 000059BB: 	LDI	*AR0,R0
    // asm 000059BC: 	CALLU	R0
    // RE INITIALIZE TRACK...
    BONUS_POSTLAUNCH[BONUS_WAVE]();
    // asm 000059BD: 	LDI	@NEXT_STARTUP,R0
    STARTSECTION = NEXT_STARTUP;
    // asm 000059BE: 	STI	R0,@STARTSECTION
    MAME_ASSERT_REG(0x000059BF, "R0", &STARTSECTION);
    // asm 000059BF: 	CALL	BGD_INIT
    BGD_INIT();
    // asm 000059C0: 	PUSH	AR7
    // asm 000059C1: 	LDI	UTIL_C|BACKGRND_T,R0
    // asm 000059C2: 	LDI	-1,R1
    // asm 000059C3: 	CALL	PRC_FIND
    proc = PRC_FIND(UTIL_C | BACKGRND_T, -1);
    // asm 000059C4: 	LDI	AR0,AR7
    // asm 000059C5: 	LDI	3,R0
    // asm 000059C6: 	STI	R0,*+AR7(PTIME)
    proc->sleep_ticks = 3;
    // asm 000059C7: 	POP	AR7
    // asm 000059C8: 	LDI	*+AR7(PDATA),R0
    // asm 000059C9: 	STI	R0,@BGNDCOLA
    BGNDCOLA = p->ctx->ATTRACT_DELTA.background_color;
    // asm 000059CA: 	LDI	1,R0
    // asm 000059CB: 	STI	R0,@NOAERASE
    NOAERASE = 1;
    // asm 000059CC: 	CLRI	R0
    OHIGH_PRIORITY = 0;
    // asm 000059CD: 	STI	R0,@OHIGH_PRIORITY
    // asm 000059CE: 	CREATE	WAVEFLAG,UTIL_C|MONKEY_T
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    CREATE(WAVEFLAG, UTIL_C | MONKEY_T, ctx);
    // asm 000059D1: 	CALL	ATTR_INIT_GAMELEG
    ATTR_INIT_GAMELEG();
    // asm 000059D2: 	LDI	MAX_DRONES,R0
    DD_MAX_DRONES = MAX_DRONES;
    // asm 000059D3: 	STI	R0,@DD_MAX_DRONES
    // asm 000059D4: 	LDI	@ATTRWAVE,R0
    // asm 000059D5: 	ADDI	2,R0
    ATTRWAVE += 2;
    // asm 000059D6: 	CMPI	ATTR_WAVETAB_LEN-1,R0
    // asm 000059D7: 	LDIGT	0,R0
    if (ATTRWAVE > ATTR_WAVETAB_LEN - 1) {
        ATTRWAVE = 0;
    }
    // asm 000059D8: 	STI	R0,@ATTRWAVE
    MAME_ASSERT_REG(0x000059D9, "R0", &ATTRWAVE);
    // asm 000059D9: 	RETS
}

// *----------------------------------------------------------------------------
static void ATTR_INIT_GAMELEG(void) {
    PROC_CONTEXT* ctx;
    PROC* proc;

    // asm 000059DA: 	CREATE	RHO_DISPATCHER,SPAWNER_C|TRAFFIC_T
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    CREATE(RHO_DISPATCHER, SPAWNER_C | TRAFFIC_T, ctx);
    // asm 000059DD: 	CREATE	SIGMA_DISPATCHER,SPAWNER_C|TRAFFIC_T
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    CREATE(SIGMA_DISPATCHER, SPAWNER_C | TRAFFIC_T, ctx);
    // asm 000059E0: 	CREATE	CPOINT_LIGHT,SPAWNER_C|COLORCYC_T
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    CREATE(CPOINT_LIGHT, SPAWNER_C | COLORCYC_T, ctx);
    // asm 000059E3: 	CREATE	POSITION_FINDER,SPAWNER_C|TRAFFIC_T
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    CREATE(POSITION_FINDER, SPAWNER_C | TRAFFIC_T, ctx);
    // asm 000059E6: 	LDI	SM_HALT,R0
    // asm 000059E7: 	STI	R0,@SUSPEND_MODE
    SUSPEND_MODE = SM_HALT;
    // asm 000059E8: 	LDI	0,R4
    // asm 000059E9: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 0;
    CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 000059EC: 	LDI	1,R4
    // asm 000059ED: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 1;
    CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 000059F0: 	LDI	2,R4
    // asm 000059F1: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 2;
    CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 000059F4: 	LDI	3,R4
    // asm 000059F5: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 3;
    CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 000059F8: 	LDI	4,R4
    // asm 000059F9: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 4;
    proc = CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 000059FC: 	STI	AR0,@PLYCAR
    // original code hack. This is not a proc, `PLYCAR` looks up the OBJ from this proc once it has executed
    PLYCAR = (OBJ*)proc;

    // asm 000059FD: 	LDI	5,R4
    // asm 000059FE: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 5;
    CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 00005A01: 	LDI	6,R4
    // asm 00005A02: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 6;
    CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 00005A05: 	LDI	7,R4
    // asm 00005A06: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 7;
    CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 00005A09: 	LDI	9,R4
    // asm 00005A0A: 	CREATE	RACER_DRONE,DRONE_C
    ctx = port_malloc(sizeof(PROC_CONTEXT));
    ctx->RACER_DRONE.rank = 9;
    CREATE(RACER_DRONE, DRONE_C, ctx);
    // asm 00005A0D: 	RETS
    // ;*----------------------------------------------------------------------------
    // ;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ATTR_INIT_GAMELEG", 0, 0);
}

void LOAD_ATTR_LEG(void) {
    // asm 00005A0E: 	LDI	@ATTRWAVE,AR0
    // asm 00005A0F: 	ADDI	@ATTR_WAVETABI,AR0
    // asm 00005A10: 	LDI	*+AR0(1),AR0
    int section_index = ATTR_WAVETAB[ATTRWAVE + 1];
#if DEBUG
    // asm: 	CMPI	0,AR0
    // asm: 	LDILT	0,AR0
    // asm: 	CMPI	13,AR0
    // asm: 	LDIGT	13,AR0
    if (section_index < 0) {
        section_index = 0;
    }
    if (section_index > 13) {
        section_index = 13;
    }
#endif
    // asm 00005A11: 	ADDI	@LOADSECTION_TABLEI,AR0
    // asm 00005A12: 	LDI	*AR0,R0
    // asm 00005A13: 	CALLU	R0
    LOADSECTION_TABLE[section_index]();
    // asm 00005A14: 	RETS
}

/*
 *----------------------------------------------------------------------------
 *----------------------------------------------------------------------------
 *OLD CODE THAT I DEARLY LIKE TO REFFERENCE
 */

/*
;*----------------------------------------------------------------------------
;*TURN CAMERA TO FOLLOW OBJECT IN AR4
;*
;*PARAMETERS
;*	AR4	OBJECT TO 'WATCH'
;*
;WATCH_OBJ:
;	LDF	*+AR4(OPOSX),R2
;	SUBF	@NEW_CAMERA+CAMERAX,R2
;	LDF	*+AR4(OPOSZ),R3
;	SUBF	@NEW_CAMERA+CAMERAZ,R3
;	CALL	ARCTANF
;	SUBF	HALFPI,R0
;
;	NEGF	R0,R2
;	STF	R2,@NEW_CAMERA+CAMERARADY
;
;	LDF	*+AR4(OPOSZ),R0
;	SUBF	@NEW_CAMERA+CAMERAZ,R0
;	MPYF	R0,R0
;	LDF	*+AR4(OPOSX),R1
;	SUBF	@NEW_CAMERA+CAMERAX,R1
;	MPYF	R1,R1
;	ADDF3	R0,R1,R2	;R= SQRT (X^2 + Z^2)
;	CALL	SQRT
;	LDF	R0,R3
;
;	LDF	*+AR4(OPOSY),R2
;	SUBF	@NEW_CAMERA+CAMERAY,R2
;
;	CALL	ARCTANF
;
;	SUBF	HALFPI,R0
;
;	STF	R0,@NEW_CAMERA+CAMERARADX
;
;	LDI	@NEW_CAMERAI,R2
;	ADDI	CAMERARADX,R2
;	LDI	@NEW_CAMERAI,AR2
;	ADDI	CAMERA_MATRIX,AR2
;	CALL	FIND_MATRIX
;
;	CAll	CAMERA_HORIZON_PROJECTION
;
;	RETS
;*----------------------------------------------------------------------------
;INITFLYBY_VIEW:
;	LDI	-1,R0
;	STI	R0,*+AR7(CUT_PAN)
;
;	FLOAT	-4300,R0
;	STF	R0,@NEW_CAMERA+CAMERA_SPEED
;
;FLYBY_VIEW:
;	LDF	@NEW_CAMERA+CAMERA_SPEED,R0
;	FLOAT	150,R1
;	ADDF	R1,R0
;	STF	R0,@NEW_CAMERA+CAMERA_SPEED
;
;	LDF	*+AR4(OPOSY),R0
;	STF	R0,@NEW_CAMERA+CAMERAY
;
;	LDF	*+AR4(ORADY),R2
;	ADDF	-1.54,R2
;	CALL	_SINE
;	FLOAT	500,R2
;	MPYF	R0,R2
;	LDF	*+AR4(OPOSX),R0
;	ADDF	R2,R0
;	STF	R0,@NEW_CAMERA+CAMERAX
;
;	LDF	*+AR4(ORADY),R2
;	ADDF	-1.54,R2
;	CALL	_COSI
;	FLOAT	500,R2
;	MPYF	R0,R2
;	LDF	*+AR4(OPOSZ),R0
;	ADDF	R2,R0
;	STF	R0,@NEW_CAMERA+CAMERAZ
;
;	LDF	*+AR4(ORADY),R0
;	SUBF	0.10,R0
;	STF	R0,@NEW_CAMERA+CAMERA_VECY
;
;	LDF	0.3,R0
;	STF	R0,@NEW_CAMERA+CAMERA_VECX
;
;	LDF	*+AR4(ORADZ),R0
;	STF	R0,@NEW_CAMERA+CAMERA_VECZ
;
;	CALL	MOVE_CAMERA
;
;	LDF	*+AR4(OPOSY),R0
;	FLOAT	-1500,R1			;FIX ELEVATION
;	ADDF	R1,R0
;	STF	R0,@NEW_CAMERA+CAMERAY
;passed
;	CALL	WATCH_OBJ
;
;	RETS
*/

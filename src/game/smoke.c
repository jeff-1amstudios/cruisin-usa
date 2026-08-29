
#include "smoke.h"
#include "../core/machine.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "sys.h"
#include "sysid.h"
#include "vunit.h"

/*
 * Source module: asm/SMOKE.ASM
 */

void SMOKE_PROC(PROC* p);
static int CREATE_SMOKE_OBJ(PROC* p /*AR7*/, c3x_reg_t z_offset /*R5*/);
static DYNAOBJ* GET_OTHER_REAR(DYNAOBJ* wheel /*AR2*/);
static DYNAOBJ* GET_REAR(OBJ* car /*AR2*/);
static DYNAOBJ* GET_REAR__tail(DYNAOBJ* dynamic_obj /*R0*/);
static void INIT_SMOKE(PROC* p /*AR7*/, OBJ* smoke_obj /*AR0*/, const int* animation /*AR1*/,
    DYNAOBJ* wheel /*AR2*/, int road_oid /*R4*/, c3x_reg_t z_offset /*R5*/);
void SORT_SMOKE(void);
void INIT_SPARK(OBJ* parent_obj /*AR4*/);
static void REPLICATE_SPARK(PROC* p /*AR7*/);
void SPARK_PROC(PROC* p);
static void INIT_COLLA_OBJS(PROC* p /*AR7*/);
void WALL_SPARK(OBJ* car_obj /*AR4*/, CARBLK* carblk /*AR5*/);
void IMPACT_SPARK(OBJ* obj0 /*AR0*/, OBJ* obj1 /*AR1*/, VECTOR* collision_point /*AR3*/);
void ROAD_IMPACT_SPARK(void);
void SKID_SPARK(void);
static int TOO_MANY_SPARKS(void);
void OBJ_MOVE(OBJ* obj /*AR2*/, c3x_reg_t frame_scale /*R1*/);
extern MATRIX _MATRIXA;
extern VECTOR _VECTORA;

#define SMOKEANII SMOKEANI
#define SMOKE2ANII SMOKE2ANI
#define SPARKANII SPARKANI

static int SMOKEANI[9];
static int SMOKE2ANI[16];
static int SPARKANI[11];

/* asm: TIRE_SMOKE_COUNT	.bss	TIRE_SMOKE_COUNT,1 */
int TIRE_SMOKE_COUNT;
/* asm: SMOKEANI */
/* asm: 	.word	bnout2,bnout4,bnout6,bnout8,bnout10,bnout12,bnout14,bnout16,-1 */
static int SMOKEANI[] = {
    bnout2_ROM,
    bnout4_ROM,
    bnout6_ROM,
    bnout8_ROM,
    bnout10_ROM,
    bnout12_ROM,
    bnout14_ROM,
    bnout16_ROM,
    -1,
};
/* asm: SMOKE2ANI */
/* asm: 	.word	bnout1,bnout2,bnout3,bnout4,bnout5,bnout6,bnout7,bnout8,bnout9,bnout10 */
/* asm: 	.word	bnout11,bnout12,bnout14,bnout15,bnout16,-1 */
/* asm: 	 */
static int SMOKE2ANI[] = {
    bnout1_ROM,
    bnout2_ROM,
    bnout3_ROM,
    bnout4_ROM,
    bnout5_ROM,
    bnout6_ROM,
    bnout7_ROM,
    bnout8_ROM,
    bnout9_ROM,
    bnout10_ROM,
    bnout11_ROM,
    bnout12_ROM,
    bnout14_ROM,
    bnout15_ROM,
    bnout16_ROM,
    -1,
};
/*
 *----------------------------------------------------------------------------
 * SMOKE	PROC
 *INPUT	AR4 = CAROBJ
 *	AR5 = CARBLK
 *Creates and maintains several puffs of smoke
 */
#define NUM_SMOKES PDATA
#define SMOKE_PAL (PDATA + 1)
#define SMOKE_OBJS (PDATA + 2)

void SMOKE_PROC(PROC* p) {
    OBJ* smoke_obj;
    const int* animation;
    c3x_reg_t movement;
    c3x_reg_t road_height;
    c3x_reg_t z_offset;
    int active_smokes;
    int smoke_index;
    int frame;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 000084B8: 	LDI	@TIRE_SMOKE_COUNT,R0
    // asm 000084B9: 	CMPI	1,R0
    // asm 000084BA: 	BGE	SMOKE_DIE
    if (TIRE_SMOKE_COUNT >= 1) {
        goto SMOKE_DIE;
    }
    // asm 000084BB: 	LDF	0,R7
    p->ctx->SMOKE_PROC.delay_frames = C3X_IMM_F32(0);
    // asm 000084BC: 	LDL	bnout1_smoke,AR2
    // asm 000084BD: 	CALL	PAL_FIND_RAW
    // asm 000084BE: 	STI	R0,*+AR7(SMOKE_PAL)
    p->ctx->SMOKE_PROC.palette = PAL_FIND_RAW((tPAL*)ROM_PTR(bnout1_smoke_ROM));
    // asm 000084BF: 	LDI	0,R0
    // asm 000084C0: 	STI	R0,*+AR7(NUM_SMOKES)
    p->ctx->SMOKE_PROC.num_smokes = 0;
    // asm 000084C1: 	LDI	1,R0
    // asm 000084C2: 	STI	R0,@TIRE_SMOKE_COUNT
    TIRE_SMOKE_COUNT = 1;
    // asm 000084C3: 	BR	SMOKELP_ENTRY
    goto SMOKELP_ENTRY;
SMOKE_PUFFLP:
    // asm 000084C4: 	LDI	0,R6
    active_smokes = 0;
    // asm 000084C5: 	LDI	0,R5
    smoke_index = 0;
    // asm 000084C6: 	LDI	SMOKE_OBJS,IR0
SMPUFFLP1:
    // asm 000084C7: 	LDI	*+AR7(IR0),AR0
    smoke_obj = p->ctx->SMOKE_PROC.smoke_objs[smoke_index];
    // asm 000084C8: 	CMPI	0,AR0
    // asm 000084C9: 	BEQ	SMPUFF1
    if (smoke_obj == NULL) {
        goto SMPUFF1;
    }
    // asm 000084CA: 	LDI	*+AR0(OUSR1),AR1
    animation = (const int*)smoke_obj->usr1;
    // asm 000084CB: 	LDI	*AR1++,R0
    frame = *animation++;
    // asm 000084CC: 	BN	SMOKE_DONE
    if (frame < 0) {
        goto SMOKE_DONE;
    }
    // asm 000084CD: 	STI	R0,*+AR0(OROMDATA)
    smoke_obj->romdata = ROM_PTR((word_addr_t)frame);
    // asm 000084CE: 	STI	AR1,*+AR0(OUSR1)
    smoke_obj->usr1 = (uintptr_t)animation;
    // asm 000084CF: 	LDI	@MATRIXAI,AR2
    // asm 000084D0: 	LDF	*+AR5(CARVROT),R2
    // asm 000084D1: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXAI, C3X_LDF(p->ctx->SMOKE_PROC.carblk->y_velocity_rotation));
    // asm 000084D2: 	CALL	CLR_VECTORA
    CLR_VECTORA();
    // asm 000084D3: 	FLOAT	@NFRAMES,R0
    movement = C3X_FROM_INT(NFRAMES);
    // asm 000084D4: 	MPYF	*+AR5(CARSPEED),R0
    movement = C3X_MUL(movement, C3X_LDF(p->ctx->SMOKE_PROC.carblk->speed));
    // asm 000084D5: 	MPYF	1.51,R0
    movement = C3X_MUL(movement, C3X_IMM_F32(1.51));
    // asm 000084D6: 	SUBF	50,R0		;Constant speed less than the player is moving
    movement = C3X_SUB(movement, C3X_IMM_F32(50)); // Constant speed less than the player is moving
    // asm 000084D7: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF(movement);
    // asm 000084D8: 	LDI	@MATRIXAI,R2
    // asm 000084D9: 	LDI	AR2,R3
    // asm 000084DA: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &MATRIXAI, &VECTORAI);
    // asm 000084DB: 	LDI	@VECTORAI,AR1
    // asm 000084DC: 	LDF	*+AR1(X),R0
    // asm 000084DD: 	ADDF	*+AR0(OPOSX),R0
    // asm 000084DE: 	STF	R0,*+AR0(OPOSX)
    smoke_obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.X), C3X_LDF(smoke_obj->pos.X)));
    // asm 000084DF: 	LDF	*+AR1(Z),R0
    // asm 000084E0: 	ADDF	*+AR0(OPOSZ),R0
    // asm 000084E1: 	STF	R0,*+AR0(OPOSZ)
    smoke_obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.Z), C3X_LDF(smoke_obj->pos.Z)));
    // asm 000084E2: 	PUSH	AR4
    // asm 000084E3: 	PUSH	IR0
    // asm 000084E4: 	PUSH	R5
    // asm 000084E5: 	LDI	AR0,AR4
    // asm 000084E6: 	ADDI	OPOSX,AR4
    // asm 000084E7: 	CALL	CAMSCAN
    CAMSCAN(&smoke_obj->pos, &road_height);
    // asm 000084E8: 	POP	R5
    // asm 000084E9: 	POP	IR0
    // asm 000084EA: 	POP	AR4
    // asm 000084EB: 	LDI	*+AR7(IR0),AR0
    // asm 000084EC: 	ADDF	*+AR0(OPOSY),R0
    // asm 000084ED: 	STF	R0,*+AR0(OPOSY)
    smoke_obj->pos.Y = C3X_STF(C3X_ADD(road_height, C3X_LDF(smoke_obj->pos.Y)));
    // asm 000084EE: 	ADDI	1,R6
    active_smokes += 1;
SMPUFF1:
    // asm 000084EF: 	ADDI	1,IR0
    // asm 000084F0: 	ADDI	1,R5
    smoke_index += 1;
    // asm 000084F1: 	CMPI	*+AR7(NUM_SMOKES),R5
    // asm 000084F2: 	BNE	SMPUFFLP1
    if (smoke_index != p->ctx->SMOKE_PROC.num_smokes) {
        goto SMPUFFLP1;
    }
    // asm 000084F3: SMOKE_CONT
    // asm 000084F3: 	CMPI	0,R6
    // asm 000084F4: 	BEQ	SMOKEX
    if (active_smokes == 0) {
        goto SMOKEX;
    }
SMOKELP_ENTRY:
    // asm 000084F5: 	LDF	0,R5
    z_offset = C3X_IMM_F32(0);
    // asm 000084F6: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm 000084F7: 	BNE	SMOKEN
    if (p->ctx->SMOKE_PROC.carblk->spin_flag != 0) {
        goto SMOKEN; // SPINNING?
    }
    // asm 000084F8: 	CMPF	0,R7
    // asm 000084F9: 	BNE	KLUDGE_MO
    if (C3X_NE(p->ctx->SMOKE_PROC.delay_frames, C3X_IMM_F32(0))) {
        goto KLUDGE_MO;
    }
    // asm 000084FA: 	LDF	2,R7			;wait N more frames
    p->ctx->SMOKE_PROC.delay_frames = C3X_IMM_F32(2); // wait N more frames
SMOKEN:
    // asm 000084FB: 	CALL	CREATE_SMOKE_OBJ
    CREATE_SMOKE_OBJ(p, z_offset);
KLUDGE_MO:
    // asm 000084FC: 	SUBF	1.0,R7
    p->ctx->SMOKE_PROC.delay_frames = C3X_SUB(p->ctx->SMOKE_PROC.delay_frames, C3X_IMM_F32(1.0));
    // asm 000084FD: 	SLEEP	1
    SLEEP(1, 1);
    // asm 000084FF: 	BR	SMOKE_PUFFLP
    goto SMOKE_PUFFLP;
SMOKE_DONE:
    // asm 00008500: 	LDI	AR0,AR2
    // asm 00008501: 	PUSH	IR0
    // asm 00008502: 	CALL	OBJ_DELETE
    OBJ_DELETE(smoke_obj);
    // asm 00008503: 	POP	IR0
    // asm 00008504: 	LDI	0,R0
    // asm 00008505: 	STI	R0,*+AR7(IR0)		;make null on list
    p->ctx->SMOKE_PROC.smoke_objs[smoke_index] = NULL; // make null on list
    // asm 00008506: 	BR	SMPUFFLP1
    goto SMPUFFLP1;
SMOKEX:
    // asm 00008507: 	LDI	0,R0
    // asm 00008508: 	STI	R0,@TIRE_SMOKE_COUNT
    TIRE_SMOKE_COUNT = 0;
SMOKE_DIE:
    // asm 00008509: 	DIE
    DIE();
}

/*
 *----------------------------------------------------------------------------
 * CREATE_SMOKE_OBJ
 *INPUT	AR4 = CAROBJ
 *	AR5 = CARBLK
 *	R5 = ZOFFSET
 *OUTPUT	SETC if no obj made; CLRC if obj made
 *Creates a puff of smoke
 */

static int CREATE_SMOKE_OBJ(PROC* p /*AR7*/, c3x_reg_t z_offset /*R5*/) {
    OBJ* smoke_obj;
    DYNAOBJ* rear_wheel;
    DYNAOBJ* other_rear_wheel;
    OBJ* road_obj;
    MATRIX* body_matrix;
    const int* animation;

    // asm 0000850A: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm 0000850B: 	CMPI	1,R0
    // asm 0000850C: 	BLT	DO_SMOKE
    if (p->ctx->SMOKE_PROC.num_smokes < 1) {
        goto DO_SMOKE;
    }
    // asm 0000850D: 	LDF	*+AR5(CARBRAKE),R0
    // asm 0000850E: 	CMPF	0.4,R0
    // asm 0000850F: 	BLT	CHECK_SKID
    if (C3X_LT(C3X_LDF(p->ctx->SMOKE_PROC.carblk->brake), C3X_IMM_F32(0.4))) {
        goto CHECK_SKID;
    }
    // asm 00008510: 	LDF	*+AR5(CARSPEED),R0
    // asm 00008511: 	CMPF	20,R0
    // asm 00008512: 	BGT	OK_SMOKE
    if (C3X_GT(C3X_LDF(p->ctx->SMOKE_PROC.carblk->speed), C3X_IMM_F32(20))) {
        goto OK_SMOKE;
    }
CHECK_SKID:
    // asm 00008513: 	LDF	*+AR5(CARSKID),R0
    // asm 00008514: 	CMPF	0.25,R0
    // asm 00008515: 	BLT	NO_SMOKE			;NO SKID ACTIVE
    if (C3X_LT(C3X_LDF(p->ctx->SMOKE_PROC.carblk->skid), C3X_IMM_F32(0.25))) {
        goto NO_SMOKE; // NO SKID ACTIVE
    }
OK_SMOKE:
    // asm 00008516: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm 00008517: 	CMPI	20,R0
    // asm 00008518: 	BGE	NO_SMOKE
    if (p->ctx->SMOKE_PROC.num_smokes >= 20) {
        goto NO_SMOKE;
    }
DO_SMOKE:
    // asm 00008519: 	LDI	AR4,AR2
    // asm 0000851A: 	CALL	GETCARBODY
    body_matrix = GETCARBODY(p->ctx->SMOKE_PROC.car_obj);
    // asm 0000851B: 	LDI	AR0,AR2
    // asm 0000851C: 	LDI	AR4,R2
    // asm 0000851D: 	ADDI	OMATRIX,R2
    // asm 0000851E: 	LDI	@MATRIXAI,R3
    // asm 0000851F: 	CALL	CONCATMATV
    CONCATMATV(body_matrix, (MATRIX*)&p->ctx->SMOKE_PROC.car_obj->omatrix, &MATRIXAI);
    // asm 00008520: 	LDI	@SMOKEANII,AR1
    animation = SMOKEANII;
    // asm 00008521: 	LDF	*+AR5(CARSKID),R0
    // asm 00008522: 	CMPF	0.5,R0
    // asm 00008523: 	LDIGT	@SMOKE2ANII,AR1
    if (C3X_GT(C3X_LDF(p->ctx->SMOKE_PROC.carblk->skid), C3X_IMM_F32(0.5))) {
        animation = SMOKE2ANII;
    }
    // asm 00008524: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm 00008525: 	LDINE	@SMOKE2ANII,AR1
    if (p->ctx->SMOKE_PROC.carblk->spin_flag != 0) {
        animation = SMOKE2ANII; // SPINNING?
    }
    // asm 00008526: 	LDI	*AR1,AR2
    // asm 00008527: 	CALL	OBJ_GETE
    smoke_obj = OBJ_GETE(ROM_PTR((word_addr_t)animation[0]));
    // asm 00008528: 	BC	CSOX
    if (smoke_obj == NULL) {
        goto CSOX;
    }
    // asm 00008529: 	LDI	AR4,AR2
    // asm 0000852A: 	CALL	GET_REAR
    rear_wheel = GET_REAR(p->ctx->SMOKE_PROC.car_obj);
    // asm 0000852B: 	PUSH	AR2
    // asm 0000852C: 	LDI	*+AR5(RR_PCOL),AR3
    road_obj = OBJREF_TO_PTR(p->ctx->SMOKE_PROC.carblk->right_rear.collided_road_object);
    // asm 0000852D: 	LDI	*+AR3(OID),R4
    // asm 0000852E: 	CALL	INIT_SMOKE
    INIT_SMOKE(p, smoke_obj, animation, rear_wheel, (int)road_obj->id, z_offset);
    // asm 0000852F: 	LDI	*AR1,AR2
    // asm 00008530: 	CALL	OBJ_GETE
    smoke_obj = OBJ_GETE(ROM_PTR((word_addr_t)animation[0]));
    // asm 00008531: 	POP	AR2
    // asm 00008532: 	BC	CSOX
    if (smoke_obj == NULL) {
        goto CSOX;
    }
    // asm 00008533: 	CALL	GET_OTHER_REAR
    other_rear_wheel = GET_OTHER_REAR(rear_wheel);
    // asm 00008534: 	LDI	*+AR5(LR_PCOL),AR3
    road_obj = OBJREF_TO_PTR(p->ctx->SMOKE_PROC.carblk->left_rear.collided_road_object);
    // asm 00008535: 	LDI	*+AR3(OID),R4
    // asm 00008536: 	CALL	INIT_SMOKE
    INIT_SMOKE(p, smoke_obj, animation, other_rear_wheel, (int)road_obj->id, z_offset);
CSOX:
    // asm 00008537: 	RETS
    return smoke_obj == NULL;
NO_SMOKE:
    // asm 00008538: 	SETC
    // asm 00008539: 	RETS
    return 1;
}

/*
 *----------------------------------------------------------------------------
 *FIND THE WHEEL IN THE DYNALIST OF THE CAR POINTED TO BY AR2
 *INPUT	AR2 = CARS OBJECT
 *OUTPUT AR0 = MATRIX OF THE BODY
 * SCRAMBLES R0,R1
 */

static DYNAOBJ* GET_OTHER_REAR(DYNAOBJ* wheel /*AR2*/) {
    // asm 0000853A: 	LDI	*AR2,R0
    DYNAOBJ* dynamic_obj = wheel->link;
    // asm 0000853B: 	BR	FBLOOP
    return GET_REAR__tail(dynamic_obj);
}

static DYNAOBJ* GET_REAR(OBJ* car /*AR2*/) {
    DYNAOBJ* dynamic_obj;

    // asm 0000853C: 	LDI	*+AR2(ODYNALIST),R0
    dynamic_obj = car->dynalist;
    // asm: 	SLOCKON	Z,"UTIL\CARPROC   dynamic objects not found"
    SLOCKON(dynamic_obj == NULL, "UTIL\\CARPROC   dynamic objects not found");
    return GET_REAR__tail(dynamic_obj);
}

static DYNAOBJ* GET_REAR__tail(DYNAOBJ* dynamic_obj /*R0*/) {
FBLOOP:
    // asm 0000853D: 	LDI	R0,AR2
    // asm 0000853E: 	LDI	*+AR2(DYNAFLAG),R1
    // asm 0000853F: 	CMPI	1,R1
    // asm 00008540: 	BZ	FOUND_REAR		;1 = rear tire
    if (dynamic_obj->flag == DYNAF_REARWHEEL) {
        goto FOUND_REAR; // 1 = rear tire
    }
    // asm 00008541: 	LDI	*AR2,R0
    dynamic_obj = dynamic_obj->link;
    // asm 00008542: 	BR	FBLOOP
    goto FBLOOP;
FOUND_REAR:
    // asm 00008543: 	RETS
    return dynamic_obj;
}

/*
 *----------------------------------------------------------------------------
 *INPUT	AR0 = obj to init
 *	AR1 = ANI to init with
 *	AR2 = wheel obj
 *	MATRIXAI - MATRIX of car
 *	R4 = OID of object over
 *	R5 = ZOFFSET
 */

static void INIT_SMOKE(PROC* p /*AR7*/, OBJ* smoke_obj /*AR0*/, const int* animation /*AR1*/,
    DYNAOBJ* wheel /*AR2*/, int road_oid /*R4*/, c3x_reg_t z_offset /*R5*/) {
    // asm 00008544: 	PUSH	AR1
    // asm 00008545: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm 00008546: 	ADDI	SMOKE_OBJS,R0
    // asm 00008547: 	LDI	R0,IR0
    // asm 00008548: 	STI	AR0,*+AR7(IR0)
    p->ctx->SMOKE_PROC.smoke_objs[p->ctx->SMOKE_PROC.num_smokes] = smoke_obj;
    // ;	LDI	@SMOKEANII,AR1
    // asm 00008549: 	STI	AR1,*+AR0(OUSR1)	;pointer to animation table
    smoke_obj->usr1 = (uintptr_t)animation; // pointer to animation table
    // asm 0000854A: 	CMPI	300h,R4
    // asm 0000854B: 	BNE	INSM1
    if (road_oid != 0x300) {
        goto INSM1;
    }
    // asm 0000854C: 	LDI	*+AR7(SMOKE_PAL),R0
    // asm 0000854D: 	STI	R0,*+AR0(OPAL)
    smoke_obj->palette = (u32)p->ctx->SMOKE_PROC.palette;
INSM1:
    // asm 0000854E: 	LDI	*AR1,R0
    // asm 0000854F: 	STI	R0,*+AR0(OROMDATA)
    smoke_obj->romdata = ROM_PTR((word_addr_t)animation[0]);
    // asm 00008550: 	LDI	PLYR_C|PLYR_SMOKE_S,R0
    // asm 00008551: 	STI	R0,*+AR0(OID)
    smoke_obj->id = PLYR_C | PLYR_SMOKE_S;
    // asm 00008552: 	LDF	40,R0
    // asm 00008553: 	CALL	SFRAND
    // asm 00008554: 	ADDF	*+AR2(DYNACENTERX),R0
    // asm 00008555: 	STF	R0,*+AR0(OPOSX)
    smoke_obj->pos.X = C3X_STF(C3X_ADD(SFRAND(C3X_IMM_F32(40)), C3X_LDF(wheel->center_x)));
    // asm 00008556: 	FLOAT	130,R0
    // asm 00008557: 	ADDF	*+AR2(DYNACENTERY),R0
    // asm 00008558: 	STF	R0,*+AR0(OPOSY)
    smoke_obj->pos.Y = C3X_STF(C3X_ADD(C3X_FROM_INT(130), C3X_LDF(wheel->center_y)));
    // asm 00008559: 	LDF	*+AR2(DYNACENTERZ),R0
    // asm 0000855A: 	ADDF	R5,R0
    // asm 0000855B: 	STF	R0,*+AR0(OPOSZ)
    smoke_obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(wheel->center_z), z_offset));
    // asm 0000855C: 	LDI	@MATRIXAI,R2
    // asm 0000855D: 	LDI	AR0,R3
    // asm 0000855E: 	ADDI	OPOSX,R3
    // asm 0000855F: 	LDI	R3,AR2
    // asm 00008560: 	CALL	MATRIX_MUL
    MATRIX_MUL(&smoke_obj->pos, &MATRIXAI, &smoke_obj->pos);
    // asm 00008561: 	LDF	*+AR0(OPOSX),R0
    // asm 00008562: 	ADDF	*+AR4(OPOSX),R0
    // asm 00008563: 	STF	R0,*+AR0(OPOSX)
    smoke_obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(smoke_obj->pos.X),
        C3X_LDF(p->ctx->SMOKE_PROC.car_obj->pos.X)));
    // asm 00008564: 	LDF	*+AR0(OPOSY),R0
    // asm 00008565: 	ADDF	*+AR4(OPOSY),R0
    // asm 00008566: 	STF	R0,*+AR0(OPOSY)
    smoke_obj->pos.Y = C3X_STF(C3X_ADD(C3X_LDF(smoke_obj->pos.Y),
        C3X_LDF(p->ctx->SMOKE_PROC.car_obj->pos.Y)));
    // asm 00008567: 	LDF	*+AR0(OPOSZ),R0
    // asm 00008568: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00008569: 	STF	R0,*+AR0(OPOSZ)
    smoke_obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(smoke_obj->pos.Z),
        C3X_LDF(p->ctx->SMOKE_PROC.car_obj->pos.Z)));
    // asm 0000856A: 	ORM	O_POSTER|O_NOCOLL,*+AR0(OFLAGS)
    smoke_obj->flags |= O_POSTER | O_NOCOLL;
    // asm 0000856D: 	LDI	AR0,AR2
    // asm 0000856E: 	CALL	OBJ_INSERT
    OBJ_INSERT(smoke_obj);
    // asm 0000856F: 	LDI	*+AR7(NUM_SMOKES),R0
    // asm 00008570: 	ADDI	1,R0			;NOTE this instruction clears the CARRY
    // asm 00008571: 	STI	R0,*+AR7(NUM_SMOKES)
    p->ctx->SMOKE_PROC.num_smokes += 1; // NOTE this instruction clears the CARRY
    // asm 00008572: 	POP	AR1
    // asm 00008573: 	RETS
}

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 */
void SORT_SMOKE(void) {
    // asm 00008574: 	LDI	PLYR_C,AR2
    // asm 00008575: 	CALL	OBJ_FIND_FIRST
    // asm 00008576: 	BNC	SORT_SMOKEX
    // asm 00008577: 	LDI	AR0,AR5
    // asm 00008578: 	LDI	AR5,AR6			;Stop when AR6 is reached
    // asm 00008579: 	LDI	@OACTIVE,AR4
    // asm 0000857A: 	LDI	AR4,AR2
SSLOOP:
    // asm 0000857B: 	CMPI	AR4,AR6
    // asm 0000857C: 	BEQ	SORT_SMOKEX		;Reached the object we linked behind
    // asm 0000857D: 	CMPI	0,AR4
    // asm 0000857E: 	BEQ	SORT_SMOKEX
    // asm 0000857F: 	LDI	*+AR4(OID),R0
    // asm 00008580: 	CMPI	PLYR_C|PLYR_SMOKE_S,R0
    // asm 00008581: 	BNE	SSLOOPEND
    // asm 00008582: 	CMPI	@OACTIVE,AR4		;First item in list?
    // asm 00008583: 	BNE	UNLINK
    // asm 00008584: 	LDI	*AR4,R0
#if DEBUG
    // asm: 	BEQ	$		;The smoke should never be the only item displayed
#endif
    // asm 00008585: 	STI	R0,@OACTIVE
    // asm 00008586: 	BR	LINK
UNLINK:
    // asm 00008587: 	LDI	*AR4,R0
    // asm 00008588: 	STI	R0,*AR2
LINK:
    // asm 00008589: 	LDI	*AR5,R1		;get object after AR5
    // asm 0000858A: 	STI	AR4,*AR5	;Link this object to AR5
    // asm 0000858B: 	STI	R1,*AR4		;Link the next object to AR4
    // asm 0000858C: 	LDI	AR4,AR5
    // asm 0000858D: 	LDI	R0,AR4
    // asm 0000858E: 	BR	SSLOOP
SSLOOPEND:
    // asm 0000858F: 	LDI	AR4,AR2		;AR2 = last object for unlinking
    // asm 00008590: 	LDI	*AR4,AR4
    // asm 00008591: 	BR	SSLOOP
SORT_SMOKEX:
    // asm 00008592: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SORT_SMOKE", 0, 0);
    UNIMPL_TODO();
}

/* asm: SPARKANI */
/* asm: 	.word	x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,-1 */
/* asm: 	 */
static int SPARKANI[] = {
    // 	.word	sparc10,sparc10,sparc11,sparc11,sparc12,sparc12,-1
    x1_ROM,
    x2_ROM,
    x3_ROM,
    x4_ROM,
    x5_ROM,
    x6_ROM,
    x7_ROM,
    x8_ROM,
    x9_ROM,
    x10_ROM,
    -1,
    // 	.word	x2,x4,x6,x8,x10,-1
};
/*
 *----------------------------------------------------------------------------
 * INIT_SPARK
 *INPUT	AR4 = object of parent
 *Creates and maintains several spark animations
 */
#define COLL_X PDATA
#define COLL_Y (PDATA + 1)
#define COLL_Z (PDATA + 2)
#define FRAME_ON (PDATA + 3)
#define CAR_OBJ (PDATA + 4)
#define CAR_BLOCK (PDATA + 5)
#define NUM_SPARKS (PDATA + 6)
#define SPARK_OBJS (PDATA + 7)

void INIT_SPARK(OBJ* parent_obj /*AR4*/) {
    PROC* spark_proc;
    PROC_CONTEXT* spark_ctx;
    CARBLK* carblk;
    MATRIX* body_matrix;
    OBJ* spark_obj;
    c3x_reg_t x_offset;
    int num_sparks;

    // asm 00008593: 	CALL	PUSHALL
    // asm 00008594: 	LDI	@_MODE,R0
    // asm 00008595: 	AND	MMODE,R0
    // asm 00008596: 	CMPI	MATTR,R0
    // asm 00008597: 	BEQ	INIT_SPARKX
    if ((_MODE & MMODE) == MATTR) {
        goto INIT_SPARKX;
    }
    // asm 00008598: 	CALL	TOO_MANY_SPARKS
    // asm 00008599: 	BC	INIT_SPARKX
    if (TOO_MANY_SPARKS()) {
        goto INIT_SPARKX;
    }
    // asm 0000859A: 	LDI	@PLYPROC,AR7
    // asm 0000859B: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    spark_ctx = port_malloc(sizeof(PROC_CONTEXT));
    spark_proc = CREATEC(SPARK_PROC, UTIL_C | SPARK_T, spark_ctx);
    // asm 0000859E: 	BC	INIT_SPARKX
    if (spark_proc == NULL) {
        goto INIT_SPARKX;
    }
    // asm 0000859F: 	LDI	AR0,AR7
    // asm 000085A0: 	STI	AR4,*+AR7(CAR_OBJ)
    spark_ctx->SPARK_PROC.car_obj = parent_obj;
    // asm 000085A1: 	LDI	*+AR4(OCARBLK),AR5
    carblk = parent_obj->carblk;
    // asm 000085A2: 	STI	AR5,*+AR7(CAR_BLOCK)
    spark_ctx->SPARK_PROC.carblk = carblk;
    // asm 000085A3: 	LDI	0,R5
    num_sparks = 0;
IS_LOOP:
    // asm 000085A4: 	LDI	@SPARKANII,AR0
    // asm 000085A5: 	LDI	*AR0,AR2
    // asm 000085A6: 	CALL	OBJ_GETE
    spark_obj = OBJ_GETE(ROM_PTR((word_addr_t)SPARKANII[0]));
    // asm 000085A7: 	BC	IS_LOOPX
    if (spark_obj == NULL) {
        goto IS_LOOPX;
    }
    // asm 000085A8: 	LDI	*+AR0(OFLAGS),R0
    // asm 000085A9: 	OR	O_NOCOLL|O_POSTER,R0
    // asm 000085AA: 	STI	R0,*+AR0(OFLAGS)
    spark_obj->flags |= O_NOCOLL | O_POSTER;
    // asm 000085AB: 	LDI	AR0,AR4
    // asm 000085AC: 	CALL	ADD_RDDEBRIS
    ADD_RDDEBRIS(spark_obj);
    // asm 000085AD: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 000085AE: 	STI	R0,*+AR4(OID)
    spark_obj->id = RDDEBRIS_C | TSC_IGNORE | TSC_SPARK_S;
    // asm 000085AF: 	LDI	R5,IR0
    // asm 000085B0: 	ADDI	SPARK_OBJS,IR0
    // asm 000085B1: 	STI	AR4,*+AR7(IR0)
    spark_ctx->SPARK_PROC.spark_objs[num_sparks] = spark_obj;
    // asm 000085B2: 	LDI	*+AR7(CAR_OBJ),AR2	;Use the body of the cars matrix
    // asm 000085B3: 	CALL	GETCARBODY		;get the over rotation of the car body
    body_matrix = GETCARBODY(spark_ctx->SPARK_PROC.car_obj);
    // asm 000085B4: 	LDI	AR0,AR2
    // asm 000085B5: 	LDI	*+AR7(CAR_OBJ),R2	;MATRIX = OVERROT+ROT of cars body
    // asm 000085B6: 	ADDI	OMATRIX,R2
    // asm 000085B7: 	LDI	AR4,R3
    // asm 000085B8: 	ADDI	OMATRIX,R3
    // asm 000085B9: 	CALL	CONCATMATV
    CONCATMATV(body_matrix, (MATRIX*)&parent_obj->omatrix, (MATRIX*)&spark_obj->omatrix);
    // asm 000085BA: 	LDF	*+AR5(CARXPLUS),R0
    // asm 000085BB: 	SUBF	*+AR5(CARXMINUS),R0
    x_offset = C3X_SUB(C3X_LDF(carblk->x_plus), C3X_LDF(carblk->x_minus));
    // asm 000085BC: 	CALL	FRAND
    x_offset = FRAND(x_offset);
    // asm 000085BD: 	ADDF	*+AR5(CARXMINUS),R0
    x_offset = C3X_ADD(x_offset, C3X_LDF(carblk->x_minus));
    // asm 000085BE: 	LDF	R0,R1
    // asm 000085BF: 	LDI	*+AR7(CAR_OBJ),AR0
    // asm 000085C0: 	LDF	*+AR0(OPOSX),R0		;Set the start point of the spark
    // asm 000085C1: 	ADDF	R1,R0
    // asm 000085C2: 	STF	R0,*+AR4(OPOSX)
    spark_obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(parent_obj->pos.X), x_offset));
    // asm 000085C3: 	LDF	*+AR0(OPOSY),R0
    // asm 000085C4: 	STF	R0,*+AR4(OPOSY)
    spark_obj->pos.Y = C3X_STF(C3X_LDF(parent_obj->pos.Y));
    // asm 000085C5: 	LDF	*+AR0(OPOSZ),R0
    // asm 000085C6: 	STF	R0,*+AR4(OPOSZ)
    spark_obj->pos.Z = C3X_STF(C3X_LDF(parent_obj->pos.Z));
    // asm 000085C7: 	FLOAT	180,R0
    // asm 000085C8: 	STF	R0,*+AR4(OVELY)		;bottom of car
    spark_obj->vel_y = C3X_STF(C3X_FROM_INT(180));
    // asm 000085C9: 	LDF	*+AR5(CARZMINUS),R0	;BACK
    // asm 000085CA: 	STF	R0,*+AR4(OVELZ)
    spark_obj->vel_z = C3X_STF(C3X_LDF(carblk->z_minus));
    // asm 000085CB: 	LDI	AR4,AR2
    // asm 000085CC: 	LDF	1,R1			;absolute move(note frame rate dependant)
    // asm 000085CD: 	CALL	OBJ_MOVE		;Offset the starting point
    OBJ_MOVE(spark_obj, C3X_IMM_F32(1));
    // asm 000085CE: 	LDI	@SPARKANII,R0
    // asm 000085CF: 	STI	R0,*+AR4(OVELZ)
    spark_ctx->SPARK_PROC.spark_animations[num_sparks] = SPARKANII;
    // asm 000085D0: 	LDF	0,R0
    // asm 000085D1: 	STF	R0,*+AR4(OVELY)
    spark_obj->vel_y = C3X_STF(C3X_IMM_F32(0));
    // asm 000085D2: 	LDI	AR4,AR2
    // asm 000085D3: 	CALL	OBJ_INSERT
    OBJ_INSERT(spark_obj);
    // asm 000085D4: 	ADDI	1,R5
    num_sparks += 1;
    // asm 000085D5: 	CMPI	6,R5
    // asm 000085D6: 	BLT	IS_LOOP
    if (num_sparks < 6) {
        goto IS_LOOP;
    }
IS_LOOPX:
    // asm 000085D7: 	CMPI	0,R5
    // asm 000085D8: 	BEQ	INIT_SPARK_KILL
    if (num_sparks == 0) {
        PRC_KILL(spark_proc);
        goto INIT_SPARKX;
    }
    // asm 000085D9: 	STI	R5,*+AR7(NUM_SPARKS)
    spark_ctx->SPARK_PROC.num_sparks = num_sparks;
INIT_SPARKX:
    // asm 000085DA: 	CALL	POPALL
    // asm 000085DB: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *Called from within SPARK_PROC. Will make a new spark based on...
 *INPUT	AR7 = PROCESS BLOCK OF SPARK_PROC
 *	R7  = FL, NUMBER OF FRAMES SINCE LAST SPARK
 */

static void REPLICATE_SPARK(PROC* p /*AR7*/) {
    OBJ* last_spark = NULL;
    OBJ* spark_obj;
    c3x_reg_t offset;
    int spark_index;

    // asm 000085DC: 	LDI	*+AR7(NUM_SPARKS),R0
    // asm 000085DD: 	CMPI	4,R0
    // asm 000085DE: 	BGE	REPSPARKX
    if (p->ctx->SPARK_PROC.num_sparks >= 4) {
        goto REPSPARKX;
    }
    // asm 000085DF: 	LDI	AR7,AR0
    // asm 000085E0: 	ADDI	SPARK_OBJS,AR0
    // asm 000085E1: 	LDI	0,R0
    spark_index = 0;
FIND_LAST_SPARKLP:
    // asm 000085E2: 	LDI	*AR0++,R1		;This loop will leave AR4 = to last NONZERO SPARK
    // asm 000085E3: 	LDINE	R1,AR4
    if (p->ctx->SPARK_PROC.spark_objs[spark_index] != NULL) {
        last_spark = p->ctx->SPARK_PROC.spark_objs[spark_index];
    }
    // asm 000085E4: 	ADDI	1,R0
    spark_index += 1;
    // asm 000085E5: 	CMPI	*+AR7(NUM_SPARKS),R0
    // asm 000085E6: 	BLT	FIND_LAST_SPARKLP
    if (spark_index < p->ctx->SPARK_PROC.num_sparks) {
        goto FIND_LAST_SPARKLP;
    }
    // asm 000085E7: 	LDI	@SPARKANII,AR2
    // asm 000085E8: 	LDI	*AR2,AR2
    // asm 000085E9: 	CALL	OBJ_GETE
    spark_obj = OBJ_GETE(ROM_PTR((word_addr_t)SPARKANII[0]));
    // asm 000085EA: 	BC	REPSPARKX
    if (spark_obj == NULL) {
        goto REPSPARKX;
    }
    // asm 000085EB: 	LDI	*+AR0(OFLAGS),R0
    // asm 000085EC: 	OR	O_NOCOLL|O_POSTER,R0
    // asm 000085ED: 	STI	R0,*+AR0(OFLAGS)
    spark_obj->flags |= O_NOCOLL | O_POSTER;
    // asm 000085EE: 	LDI	@MATRIXAI,AR2
    // asm 000085EF: 	LDF	*+AR5(CARVROT),R2
    // asm 000085F0: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXAI, C3X_LDF(p->ctx->SPARK_PROC.carblk->y_velocity_rotation));
    // asm 000085F1: 	LDF	40,R0
    // asm 000085F2: 	CALL	SFRAND
    offset = SFRAND(C3X_IMM_F32(40));
    // asm 000085F3: 	STF	R0,*+AR0(OPOSX)
    spark_obj->pos.X = C3X_STF(offset);
    // asm 000085F4: 	LDF	90,R0
    // asm 000085F5: 	CALL	SFRAND
    offset = SFRAND(C3X_IMM_F32(90));
    // asm 000085F6: 	STF	R0,*+AR0(OPOSY)
    spark_obj->pos.Y = C3X_STF(offset);
    // asm 000085F7: 	FLOAT	@NFRAMES,R0
    offset = C3X_FROM_INT(NFRAMES);
    // asm 000085F8: 	MPYF	50,R0			;Constant speed less than the player is moving
    offset = C3X_MUL(offset, C3X_IMM_F32(50));
    // asm 000085F9: 	MPYF	R7,R0			;NUMBER OF FRAMES SINCE LAST SPARK CREATED
    offset = C3X_MUL(offset, p->ctx->SPARK_PROC.delay_frames);
    // asm 000085FA: 	NEGF	R0			;OFFSET FROM LAST SPARK
    offset = C3X_NEG(offset);
    // asm 000085FB: 	STF	R0,*+AR0(OPOSZ)
    spark_obj->pos.Z = C3X_STF(offset);
    // asm 000085FC: 	LDI	AR0,AR2
    // asm 000085FD: 	ADDI	OPOSX,AR2
    // asm 000085FE: 	LDI	AR2,R3
    // asm 000085FF: 	LDI	@MATRIXAI,R2
    // asm 00008600: 	CALL	MATRIX_MUL
    MATRIX_MUL(&spark_obj->pos, &MATRIXAI, &spark_obj->pos);
    // asm 00008601: 	LDF	*+AR4(OPOSX),R0
    // asm 00008602: 	ADDF	*+AR0(OPOSX),R0
    // asm 00008603: 	STF	R0,*+AR0(OPOSX)
    spark_obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(last_spark->pos.X), C3X_LDF(spark_obj->pos.X)));
    // asm 00008604: 	LDF	*+AR4(OPOSY),R0
    // asm 00008605: 	ADDF	*+AR0(OPOSY),R0
    // asm 00008606: 	STF	R0,*+AR0(OPOSY)
    spark_obj->pos.Y = C3X_STF(C3X_ADD(C3X_LDF(last_spark->pos.Y), C3X_LDF(spark_obj->pos.Y)));
    // asm 00008607: 	LDF	*+AR4(OPOSZ),R0
    // asm 00008608: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00008609: 	STF	R0,*+AR0(OPOSZ)
    spark_obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(last_spark->pos.Z), C3X_LDF(spark_obj->pos.Z)));
    // asm 0000860A: 	LDI	*+AR7(NUM_SPARKS),AR2
    // asm 0000860B: 	ADDI	SPARK_OBJS,AR2
    // asm 0000860C: 	ADDI	AR7,AR2
    // asm 0000860D: 	STI	AR0,*AR2
    p->ctx->SPARK_PROC.spark_objs[p->ctx->SPARK_PROC.num_sparks] = spark_obj;
    // asm 0000860E: 	LDF	*+AR4(OVELY),R0
    // asm 0000860F: 	STF	R0,*+AR0(OVELY)
    spark_obj->vel_y = C3X_STF(C3X_LDF(last_spark->vel_y));
    // asm 00008610: 	LDI	*+AR7(NUM_SPARKS),R0
    // asm 00008611: 	ADDI	1,R0
    // asm 00008612: 	STI	R0,*+AR7(NUM_SPARKS)
    p->ctx->SPARK_PROC.num_sparks += 1;
    // asm 00008613: 	LDI	AR0,AR4
    // asm 00008614: 	CALL	ADD_RDDEBRIS
    ADD_RDDEBRIS(spark_obj);
    // asm 00008615: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 00008616: 	STI	R0,*+AR4(OID)
    spark_obj->id = RDDEBRIS_C | TSC_IGNORE | TSC_SPARK_S;
    // asm 00008617: 	LDI	@SPARKANII,R0
    // asm 00008618: 	STI	R0,*+AR4(OVELZ)
    p->ctx->SPARK_PROC.spark_animations[p->ctx->SPARK_PROC.num_sparks - 1] = SPARKANII;
    // asm 00008619: 	LDI	AR4,AR2
    // asm 0000861A: 	CALL	OBJ_INSERT
    OBJ_INSERT(spark_obj);
REPSPARKX:
    // asm 0000861B: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 * SPARK_PROC	PROC
 */

/*
 *INPUT	PDATA = SETUP see equates at begining of file
 *Maintains several spark animations
 */
void SPARK_PROC(PROC* p) {
    const int* animation;
    OBJ* spark_obj;
    c3x_reg_t movement;
    c3x_reg_t road_height;
    int active_sparks;
    int animation_frame;
    int collision_found;
    int spark_index;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 0000861C: 	LDF	0,R7
    p->ctx->SPARK_PROC.delay_frames = C3X_IMM_F32(0);
    // asm 0000861D: 	LDI	*+AR7(CAR_BLOCK),AR5
SSANI_LOOP:
    // asm 0000861E: 	LDI	0,R5		;SPARK ON
    spark_index = 0;
    // asm 0000861F: 	LDI	0,R6		;NUMBER OF SPARKS STILL ACTIVE
    active_sparks = 0;
SS1:
    // asm 00008620: 	LDI	R5,IR0
    // asm 00008621: 	ADDI	SPARK_OBJS,IR0
    // asm 00008622: 	LDI	*+AR7(IR0),AR4
    spark_obj = p->ctx->SPARK_PROC.spark_objs[spark_index];
    // asm 00008623: 	CMPI	0,AR4
    // asm 00008624: 	BEQ	NEXT_SPARK		;SPARK is no longer with us
    if (spark_obj == NULL) {
        goto NEXT_SPARK;
    }
    // asm 00008625: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY OFF LIST
    // asm 00008626: 	TSTB	O_LIST_M,R0
    // asm 00008627: 	BZ	SPARK_ANIX			;YES, DELETE FROM LIST
    if ((spark_obj->flags & O_LIST_M) == 0) {
        goto SPARK_ANIX;
    }
    // asm 00008628: 	LDI	*+AR4(OID),R0
    // asm 00008629: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 0000862A: 	BNE	SPARK_ANIX		;not a spark???!!!
    if (spark_obj->id != (RDDEBRIS_C | TSC_IGNORE | TSC_SPARK_S)) {
        goto SPARK_ANIX;
    }
    // asm 0000862B: 	LDI	*+AR4(OVELZ),AR6		;ANIMATION POINTER
    animation = p->ctx->SPARK_PROC.spark_animations[spark_index];
    // asm 0000862C: 	LDI	*++AR6,R0
    animation_frame = *++animation;
    // asm 0000862D: 	BN	SPARK_ANIX
    if (animation_frame < 0) {
        goto SPARK_ANIX;
    }
    // asm 0000862E: 	STI	R0,*+AR4(OROMDATA)
    spark_obj->romdata = ROM_PTR((word_addr_t)animation_frame);
    // asm 0000862F: 	STI	AR6,*+AR4(OVELZ)
    p->ctx->SPARK_PROC.spark_animations[spark_index] = animation;
    // asm 00008630: 	LDI	*+AR4(ODIST),R0
    // asm 00008631: 	CMPI	1000,R0
    // asm 00008632: 	BLE	SPARK_ANIX
    if (spark_obj->dist <= 1000) {
        goto SPARK_ANIX;
    }
    // asm 00008633: 	LDI	@MATRIXAI,AR2
    // asm 00008634: 	LDF	*+AR5(CARVROT),R2
    // asm 00008635: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXAI, C3X_LDF(p->ctx->SPARK_PROC.carblk->y_velocity_rotation));
    // asm 00008636: 	CALL	CLR_VECTORA
    CLR_VECTORA();
    // asm 00008637: 	FLOAT	@NFRAMES,R0
    movement = C3X_FROM_INT(NFRAMES);
    // asm 00008638: 	MPYF	*+AR5(CARSPEED),R0
    movement = C3X_MUL(movement, C3X_LDF(p->ctx->SPARK_PROC.carblk->speed));
    // asm 00008639: 	MPYF	1.5,R0
    movement = C3X_MUL(movement, C3X_IMM_F32(1.5));
    // asm 0000863A: 	SUBF	100,R0		;Constant speed less than the player is moving
    movement = C3X_SUB(movement, C3X_IMM_F32(100));
    // asm 0000863B: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF(movement);
    // asm 0000863C: 	LDI	@MATRIXAI,R2
    // asm 0000863D: 	LDI	AR2,R3
    // asm 0000863E: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &MATRIXAI, &VECTORAI);
    // asm 0000863F: 	LDI	@VECTORAI,AR1
    // asm 00008640: 	LDF	*+AR1(X),R0
    // asm 00008641: 	ADDF	*+AR4(OPOSX),R0
    // asm 00008642: 	STF	R0,*+AR4(OPOSX)
    spark_obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.X), C3X_LDF(spark_obj->pos.X)));
    // asm 00008643: 	LDF	*+AR1(Z),R0
    // asm 00008644: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00008645: 	STF	R0,*+AR4(OPOSZ)
    spark_obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.Z), C3X_LDF(spark_obj->pos.Z)));
    // asm 00008646: 	PUSH	AR4
    // asm 00008647: 	PUSH	IR0
    // asm 00008648: 	PUSH	R5
    // asm 00008649: 	ADDI	OPOSX,AR4
    // asm 0000864A: 	CALL	CAMSCAN
    collision_found = CAMSCAN(&spark_obj->pos, &road_height);
    // asm 0000864B: 	POP	R5
    // asm 0000864C: 	POP	IR0
    // asm 0000864D: 	POP	AR4
    // asm 0000864E: 	BNC	SPARK_ANIX	;This spark is out of this world
    if (!collision_found) {
        goto SPARK_ANIX;
    }
    // asm 0000864F: 	SUBF	*+AR4(OVELY),R0		;SPARKS OFFSET FROM THE GROUND
    road_height = C3X_SUB(road_height, C3X_LDF(spark_obj->vel_y));
    // asm 00008650: 	ADDF	*+AR4(OPOSY),R0
    road_height = C3X_ADD(road_height, C3X_LDF(spark_obj->pos.Y));
    // asm 00008651: 	STF	R0,*+AR4(OPOSY)
    spark_obj->pos.Y = C3X_STF(road_height);
    // asm 00008652: 	ADDI	1,R6
    active_sparks += 1;
NEXT_SPARK:
    // asm 00008653: 	ADDI	1,R5
    spark_index += 1;
    // asm 00008654: 	CMPI	*+AR7(NUM_SPARKS),R5
    // asm 00008655: 	BLT	SS1
    if (spark_index < p->ctx->SPARK_PROC.num_sparks) {
        goto SS1;
    }
    // asm 00008656: 	CMPI	0,R6
    // asm 00008657: 	BEQ	SPARK_DIE
    if (active_sparks == 0) {
        goto SPARK_DIE;
    }
    // asm 00008658: 	SLEEP	1
    SLEEP(1, 1);
    // asm 0000865A: 	ADDF	1,R7
    p->ctx->SPARK_PROC.delay_frames = C3X_ADD(p->ctx->SPARK_PROC.delay_frames, C3X_IMM_F32(1));
    // asm 0000865B: 	CMPF	2,R7			;WAIT NFRAMES
    // asm 0000865C: 	BNE	KLUDGE_MOFO
    if (C3X_NE(p->ctx->SPARK_PROC.delay_frames, C3X_IMM_F32(2))) {
        goto KLUDGE_MOFO;
    }
    // asm 0000865D: 	CALL	REPLICATE_SPARK		;Will replicate the latest spark still active
    REPLICATE_SPARK(p);
    // asm 0000865E: 	LDF	0,R7
    p->ctx->SPARK_PROC.delay_frames = C3X_IMM_F32(0);
KLUDGE_MOFO:
    // asm 0000865F: 	BR	SSANI_LOOP
    goto SSANI_LOOP;
SPARK_DIE:
    // asm 00008660: 	DIE
    DIE();
SPARK_ANIX:
    // asm 00008661: 	LDI	R5,IR0
    // asm 00008662: 	ADDI	SPARK_OBJS,IR0
    // asm 00008663: 	LDI	*+AR7(IR0),AR2
    // asm 00008664: 	LDI	0,R0
    // asm 00008665: 	STI	R0,*+AR7(IR0)		;COLOR THIS SPARK GONE
    p->ctx->SPARK_PROC.spark_objs[spark_index] = NULL;
    // asm 00008666: 	LDI	*+AR2(OFLAGS),R0	;CHECK IF ALREADY OFF LIST
    // asm 00008667: 	TSTB	O_LIST_M,R0
    // asm 00008668: 	BZ	NO_OBJ
    if ((spark_obj->flags & O_LIST_M) == 0) {
        goto NO_OBJ;
    }
    // asm 00008669: 	LDI	*+AR2(OID),R0
    // asm 0000866A: 	CMPI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 0000866B: 	BNE	NO_OBJ			;not a spark???!!!
    if (spark_obj->id != (RDDEBRIS_C | TSC_IGNORE | TSC_SPARK_S)) {
        goto NO_OBJ;
    }
    // asm 0000866C: 	CALL	OBJ_DELETE
    OBJ_DELETE(spark_obj);
NO_OBJ:
    // asm 0000866D: 	BR	NEXT_SPARK
    goto NEXT_SPARK;
}

/*
 *----------------------------------------------------------------------------
 * INIT_COLLA_OBJS
 *INPUT	AR0 points to proc memory
 *Creates several spark animations
 */
static void INIT_COLLA_OBJS(PROC* p /*AR7*/) {
    OBJ* car_obj;
    OBJ* spark_obj;
    c3x_reg_t position;
    c3x_reg_t x_offset;
    c3x_reg_t y_offset;
    c3x_reg_t z_offset;
    int num_sparks;

    // asm 0000866E: 	LDI	0,R5
    num_sparks = 0;
ICO_LOOP:
    // asm 0000866F: 	LDI	@SPARKANII,AR0
    // asm 00008670: 	LDI	*AR0,AR2
    // asm 00008671: 	CALL	OBJ_GETE
    spark_obj = OBJ_GETE(ROM_PTR((word_addr_t)SPARKANII[0]));
    // asm 00008672: 	BC	ICO_LOOPX		;out of objects
    if (spark_obj == NULL) {
        goto ICO_LOOPX;
    }
    // asm 00008673: 	LDI	AR0,AR4
    // asm 00008674: 	LDI	*+AR4(OFLAGS),R0
    // asm 00008675: 	OR	O_NOCOLL|O_POSTER,R0
    // asm 00008676: 	STI	R0,*+AR4(OFLAGS)
    spark_obj->flags |= O_NOCOLL | O_POSTER;
    // asm 00008677: 	CALL	ADD_RDDEBRIS
    ADD_RDDEBRIS(spark_obj);
    // asm 00008678: 	LDI	RDDEBRIS_C|TSC_IGNORE|TSC_SPARK_S,R0
    // asm 00008679: 	STI	R0,*+AR4(OID)
    spark_obj->id = RDDEBRIS_C | TSC_IGNORE | TSC_SPARK_S;
    // asm 0000867A: 	LDI	R5,IR0
    // asm 0000867B: 	ADDI	SPARK_OBJS,IR0
    // asm 0000867C: 	STI	AR4,*+AR7(IR0)
    p->ctx->SPARK_PROC.spark_objs[num_sparks] = spark_obj;
    // asm 0000867D: 	FLOAT	60,R0			;calculate the random offset that this obj will have
    x_offset = C3X_FROM_INT(60); // calculate the random offset that this obj will have
    // asm 0000867E: 	CALL	SFRAND
    x_offset = SFRAND(x_offset);
    // asm 0000867F: 	LDF	R0,R1
    // asm 00008680: 	FLOAT	-60,R0
    y_offset = C3X_FROM_INT(-60);
    // asm 00008681: 	CALL	FRAND
    y_offset = FRAND(y_offset);
    // asm 00008682: 	ADDF	20,R0
    y_offset = C3X_ADD(y_offset, C3X_IMM_F32(20));
    // asm 00008683: 	LDF	R0,R2
    // asm 00008684: 	FLOAT	60,R0
    z_offset = C3X_FROM_INT(60);
    // asm 00008685: 	CALL	SFRAND
    z_offset = SFRAND(z_offset);
    // asm 00008686: 	LDF	R0,R3
    // asm 00008687: 	LDI	*+AR7(CAR_OBJ),AR0
    car_obj = p->ctx->SPARK_PROC.car_obj;
    // asm 00008688: 	LDF	*+AR7(COLL_X),R0		;Set the start point of the spark
    position = C3X_LDF(p->ctx->SPARK_PROC.collision_offset.X); // Set the start point of the spark
    // asm 00008689: 	ADDF	R1,R0
    position = C3X_ADD(position, x_offset);
    // asm 0000868A: 	ADDF	*+AR0(OPOSX),R0
    position = C3X_ADD(position, C3X_LDF(car_obj->pos.X));
    // asm 0000868B: 	STF	R0,*+AR4(OPOSX)
    spark_obj->pos.X = C3X_STF(position);
    // asm 0000868C: 	LDF	*+AR7(COLL_Y),R0
    position = C3X_LDF(p->ctx->SPARK_PROC.collision_offset.Y);
    // asm 0000868D: 	ADDF	R2,R0
    position = C3X_ADD(position, y_offset);
    // asm 0000868E: 	ADDF	-30,R0
    position = C3X_ADD(position, C3X_IMM_F32(-30));
    // asm 0000868F: 	NEGF	R0,R1
    y_offset = C3X_NEG(position);
    // asm 00008690: 	ADDF	180,R1				;AVERAGE car hight from road
    y_offset = C3X_ADD(y_offset, C3X_IMM_F32(180)); // AVERAGE car hight from road
    // asm 00008691: 	STF	R1,*+AR4(OVELY)			;This will be used as the y offset
    spark_obj->vel_y = C3X_STF(y_offset); // This will be used as the y offset
    // asm 00008692: 	ADDF	*+AR0(OPOSY),R0
    position = C3X_ADD(position, C3X_LDF(car_obj->pos.Y));
    // asm 00008693: 	STF	R0,*+AR4(OPOSY)
    spark_obj->pos.Y = C3X_STF(position);
    // asm 00008694: 	LDF	*+AR7(COLL_Z),R0
    position = C3X_LDF(p->ctx->SPARK_PROC.collision_offset.Z);
    // asm 00008695: 	ADDF	R3,R0
    position = C3X_ADD(position, z_offset);
    // asm 00008696: 	ADDF	*+AR0(OPOSZ),R0
    position = C3X_ADD(position, C3X_LDF(car_obj->pos.Z));
    // asm 00008697: 	STF	R0,*+AR4(OPOSZ)
    spark_obj->pos.Z = C3X_STF(position);
    // asm 00008698: 	LDI	@SPARKANII,R0
    // asm 00008699: 	STI	R0,*+AR4(OVELZ)
    p->ctx->SPARK_PROC.spark_animations[num_sparks] = SPARKANII;
    // asm 0000869A: 	LDI	AR4,AR2
    // asm 0000869B: 	CALL	OBJ_INSERT
    OBJ_INSERT(spark_obj);
    // asm 0000869C: 	ADDI	1,R5
    num_sparks += 1;
    // asm 0000869D: 	CMPI	2,R5
    // asm 0000869E: 	BLT	ICO_LOOP
    if (num_sparks < 2) {
        goto ICO_LOOP;
    }
ICO_LOOPX:
    // asm 0000869F: 	CMPI	0,R5
    // asm 000086A0: 	BEQ	INIT_SPARK_KILL
    if (num_sparks == 0) {
        goto INIT_SPARK_KILL;
    }
    // asm 000086A1: 	STI	R5,*+AR7(NUM_SPARKS)
    p->ctx->SPARK_PROC.num_sparks = num_sparks;
    // asm 000086A2: 	RETS
    return;
INIT_SPARK_KILL:
    // asm 000086A3: 	LDI	AR7,AR2
    // asm 000086A4: 	LDI	0,AR7		;Stupid thing thinks I'm commiting suicide!
    // asm 000086A5: 	CALL	PRC_KILL
    PRC_KILL(p); // Stupid thing thinks I'm commiting suicide!
    // asm 000086A6: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *INPUT	AR4	Car object
 *	AR5	Car block
 *OUTPUT	NONE
 *
 * Creates sparks at position where the wall was hit
 */

void WALL_SPARK(OBJ* car_obj /*AR4*/, CARBLK* carblk /*AR5*/) {
    PROC* spark_proc;
    PROC_CONTEXT* spark_ctx;
    OBJ* track_obj;
    c3x_reg_t car_direction;
    c3x_reg_t collision_x;
    c3x_reg_t collision_z;
    c3x_reg_t distance_to_center;
    c3x_reg_t relative_angle;
    c3x_reg_t road_direction;

    MAME_ASSERT_FUNCTION_ENTRY();

    // asm 000086A7: 	CALL	PUSHALL
    // asm 000086A8: 	LDI	@_MODE,R0
    // asm 000086A9: 	AND	MMODE,R0
    // asm 000086AA: 	CMPI	MATTR,R0
    // asm 000086AB: 	BEQ	WALL_SPARKX
    if ((_MODE & MMODE) == MATTR) {
        goto WALL_SPARKX;
    }
    // asm 000086AC: 	CMPI	@PLYCAR,AR4
    // asm 000086AD: 	BNE	WALL_SPARKX		;Only work for the players car
    if (car_obj != PLYCAR) {
        goto WALL_SPARKX; // Only work for the players car
    }
    // asm 000086AE: 	LDI	*+AR5(CARTRAK),R0
    // asm 000086AF: 	BZ	WALL_SPARKX		;DUDE IS NOT ON THE ROAD
    if (carblk->closest_track_piece == 0) {
        goto WALL_SPARKX; // DUDE IS NOT ON THE ROAD
    }
    // asm 000086B0: 	CALL	TOO_MANY_SPARKS
    // asm 000086B1: 	BC	WALL_SPARKX
    if (TOO_MANY_SPARKS()) {
        goto WALL_SPARKX;
    }
    // asm 000086B2: 	LDI	@PLYPROC,AR7
    // asm 000086B3: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    spark_ctx = port_malloc(sizeof(PROC_CONTEXT));
    spark_proc = CREATEC(SPARK_PROC, UTIL_C | SPARK_T, spark_ctx);
    // asm 000086B6: 	BC	WALL_SPARKX
    if (spark_proc == NULL) {
        goto WALL_SPARKX;
    }
    // asm 000086B7: 	LDI	AR0,AR7
    // asm 000086B8: 	STI	AR4,*+AR7(CAR_OBJ)
    spark_ctx->SPARK_PROC.car_obj = car_obj;
    // asm 000086B9: 	STI	AR5,*+AR7(CAR_BLOCK)
    spark_ctx->SPARK_PROC.carblk = carblk;
    // asm 000086BA: 	LDF	*+AR5(CARYROT),R2
    // asm 000086BB: 	LDI	@MATRIXAI,AR2
    // asm 000086BC: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXAI, C3X_LDF(carblk->y_rotation));
    // ;find the angle of the car reletive to the track it is on
    // asm 000086BD: 	LDI	*+AR5(CARTRAK),R0
    track_obj = OBJREF_TO_PTR(carblk->closest_track_piece);
    // asm 000086BE: 	LDI	R0,AR2
    // asm 000086BF: 	CALL	GETRDIR
    road_direction = GETRDIR(track_obj);
    // asm 000086C0: 	LDF	R0,R2
    // ;	LDF	*+AR5(CARVROT),R2
    // asm 000086C1: 	CALL	NORMITS
    road_direction = NORMITS(road_direction);
    // asm 000086C2: 	LDF	R2,R0
    // asm 000086C3: 	LDF	*+AR5(CARYROT),R2
    car_direction = C3X_LDF(carblk->y_rotation);
    // asm 000086C4: 	CALL	NORMITS
    car_direction = NORMITS(car_direction);
    // asm 000086C5: 	SUBF	R0,R2
    relative_angle = C3X_SUB(car_direction, road_direction);
    // asm 000086C6: 	CALL	NORMITS
    relative_angle = NORMITS(relative_angle);
    // ;find the side of the road the car is on
    // asm 000086C7: 	LDF	*+AR5(CARDIST2CNTR),R0		;- = right side ;+ = left side
    distance_to_center = C3X_LDF(carblk->dist_to_center); // - = right side ;+ = left side
    // ;determine which corner of the car hit the wall
    // asm 000086C8: 	ABSF	R2,R1
    // asm 000086C9: 	CMPF	HALFPI,R1
    // asm 000086CA: 	BLT	FACINGFRONT
    if (C3X_LT(C3X_ABS(relative_angle), C3X_IMM_F32(HALFPI))) {
        goto FACINGFRONT;
    }
    // asm 000086CB: 	NEGF	R0
    distance_to_center = C3X_NEG(distance_to_center);
    // asm 000086CC: 	NEGF	R2
    relative_angle = C3X_NEG(relative_angle);
FACINGFRONT:
    // asm 000086CD: 	CMPF	0,R0			;which side is the wall?
    // asm 000086CE: 	BGT	LEFT_SIDE
    if (C3X_GT(distance_to_center, C3X_IMM_F32(0))) {
        goto LEFT_SIDE;
    }
RIGHT_SIDE:
    // asm 000086CF: 	LDF	*+AR5(CARXPLUS),R1
    collision_x = C3X_LDF(carblk->x_plus);
    // asm 000086D0: 	CMPF	0,R2
    // asm 000086D1: 	LDFGT	*+AR5(CARZMINUS),R0	;BACK
    // asm 000086D2: 	LDFLE	*+AR5(CARZPLUS),R0	;FRONT
    collision_z = C3X_GT(relative_angle, C3X_IMM_F32(0))
                      ? C3X_LDF(carblk->z_minus)
                      : C3X_LDF(carblk->z_plus);
    // asm 000086D3: 	BR	WALLS1
    goto WALLS1;
LEFT_SIDE:
    // asm 000086D4: 	LDF	*+AR5(CARXMINUS),R1
    collision_x = C3X_LDF(carblk->x_minus);
    // asm 000086D5: 	CMPF	0,R2
    // asm 000086D6: 	LDFGT	*+AR5(CARZPLUS),R0	;FRONT
    // asm 000086D7: 	LDFLE	*+AR5(CARZMINUS),R0	;BACK
    collision_z = C3X_GT(relative_angle, C3X_IMM_F32(0))
                      ? C3X_LDF(carblk->z_plus)
                      : C3X_LDF(carblk->z_minus);
WALLS1:
    // ;get the coords of that corner
    // ;transform them based on the body of the car
    // asm 000086D8: 	STF	R1,*+AR7(COLL_X)
    spark_ctx->SPARK_PROC.collision_offset.X = C3X_STF(collision_x);
    // asm 000086D9: 	STF	R0,*+AR7(COLL_Z)
    spark_ctx->SPARK_PROC.collision_offset.Z = C3X_STF(collision_z);
    // asm 000086DA: 	LDF	-90,R0
    // asm 000086DB: 	STF	R0,*+AR7(COLL_Y)
    spark_ctx->SPARK_PROC.collision_offset.Y = C3X_STF(C3X_IMM_F32(-90));
    // asm 000086DC: 	LDI	AR7,AR2
    // asm 000086DD: 	ADDI	COLL_X,AR2
    // asm 000086DE: 	LDI	AR2,R3
    // asm 000086DF: 	LDI	@MATRIXAI,R2
    // asm 000086E0: 	CALL	MATRIX_MUL
    MATRIX_MUL(&spark_ctx->SPARK_PROC.collision_offset, &MATRIXAI,
               &spark_ctx->SPARK_PROC.collision_offset);
    // ;no go and make the spark objects
    // asm 000086E1: 	CALL	INIT_COLLA_OBJS
    INIT_COLLA_OBJS(spark_proc);
WALL_SPARKX:
    // asm 000086E2: 	CALL	POPALL
    // asm 000086E3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WALL_SPARK", 0, 0);
}

/*
 *----------------------------------------------------------------------------
 *IMPACT_SPARK
 *	INPUT *-AR3(1) = XYZ
 *	INPUT	AR0,AR1 = two objects
 *
 */

void IMPACT_SPARK(OBJ* obj0 /*AR0*/, OBJ* obj1 /*AR1*/, VECTOR* collision_point /*AR3*/) {
    OBJ* player_obj;
    CARBLK* player_carblk;
    PROC* spark_proc;
    PROC_CONTEXT* spark_ctx;
    c3x_reg_t collision_offset;

    // asm 000086E4: 	CALL	PUSHALL
    // asm 000086E5: 	LDI	@_MODE,R0
    // asm 000086E6: 	AND	MMODE,R0
    // asm 000086E7: 	CMPI	MATTR,R0
    // asm 000086E8: 	BEQ	IMPACT_SPARKX
    if ((_MODE & MMODE) == MATTR) {
        goto IMPACT_SPARKX;
    }
    // asm 000086E9: 	CMPI	@PLYCAR,AR0
    // asm 000086EA: 	LDIEQ	AR0,AR4
    // asm 000086EB: 	BEQ	IMPACTED_PLAYER
    if (obj0 == PLYCAR) {
        player_obj = obj0;
        goto IMPACTED_PLAYER;
    }
    // asm 000086EC: 	CMPI	@PLYCAR,AR1
    // asm 000086ED: 	LDIEQ	AR1,AR4
    // asm 000086EE: 	BNE	IMPACT_SPARKX		;Only work for the players car
    if (obj1 != PLYCAR) {
        goto IMPACT_SPARKX;
    }
    player_obj = obj1;
IMPACTED_PLAYER:
    // asm 000086EF: 	LDI	*+AR4(OCARBLK),AR5
    player_carblk = player_obj->carblk;
    // asm 000086F0: 	LDI	*+AR5(CARTRAK),R0
    // asm 000086F1: 	BZ	IMPACT_SPARKX		;DUDE IS NOT ON THE ROAD
    if (player_carblk->closest_track_piece == 0) {
        goto IMPACT_SPARKX;
    }
    // asm 000086F2: 	CALL	TOO_MANY_SPARKS
    // asm 000086F3: 	BC	IMPACT_SPARKX
    if (TOO_MANY_SPARKS()) {
        goto IMPACT_SPARKX;
    }
    // asm 000086F4: 	LDI	@PLYPROC,AR7
    // asm 000086F5: 	CREATEC	SPARK_PROC,UTIL_C|SPARK_T
    spark_ctx = port_malloc(sizeof(PROC_CONTEXT));
    spark_proc = CREATEC(SPARK_PROC, UTIL_C | SPARK_T, spark_ctx);
    // asm 000086F8: 	BC	IMPACT_SPARKX
    if (spark_proc == NULL) {
        goto IMPACT_SPARKX;
    }
    // asm 000086F9: 	LDI	AR0,AR7
    // asm 000086FA: 	STI	AR4,*+AR7(CAR_OBJ)
    spark_ctx->SPARK_PROC.car_obj = player_obj;
    // asm 000086FB: 	STI	AR5,*+AR7(CAR_BLOCK)
    spark_ctx->SPARK_PROC.carblk = player_carblk;
    // asm 000086FC: 	LDF	*-AR3(1),R0
    // asm 000086FD: 	SUBF	*+AR4(OPOSX),R0		;Make offset from the car
    collision_offset = C3X_SUB(C3X_LDF(collision_point->X), C3X_LDF(player_obj->pos.X));
    // asm 000086FE: 	STF	R0,*+AR7(COLL_X)
    spark_ctx->SPARK_PROC.collision_offset.X = C3X_STF(collision_offset);
    // ;	LDF	*AR3,R0
    // ;	SUBF	60,R0
    // ;	SUBF	*+AR4(OPOSY),R0		;Make offset from the car
    // asm 000086FF: 	LDF	-80,R0
    collision_offset = C3X_IMM_F32(-80);
    // asm 00008700: 	STF	R0,*+AR7(COLL_Y)
    spark_ctx->SPARK_PROC.collision_offset.Y = C3X_STF(collision_offset);
    // asm 00008701: 	LDF	*+AR3(1),R0
    // asm 00008702: 	SUBF	*+AR4(OPOSZ),R0		;Make offset from the car
    collision_offset = C3X_SUB(C3X_LDF(collision_point->Z), C3X_LDF(player_obj->pos.Z));
    // asm 00008703: 	STF	R0,*+AR0(COLL_Z)
    spark_ctx->SPARK_PROC.collision_offset.Z = C3X_STF(collision_offset);
    // asm 00008704: 	CALL	INIT_COLLA_OBJS
    INIT_COLLA_OBJS(spark_proc);
IMPACT_SPARKX:
    // asm 00008705: 	CALL	POPALL
    // asm 00008706: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *ROAD_IMPACT_SPARK ASSUMSE IT IS CALLED FROM A ROUTINE THAT USED BOXSCAN IN COLLA
 *INPUT	R2 = distance to road bottom from the point the is bellow it
 *
 *
 */

void ROAD_IMPACT_SPARK(void) {
    // asm 00008707: 	RETS
    return;
    // ;	CALL	PUSHALL
    // ;
    // ;	CREATE	SPARK_COLLA_PROC,UTIL_C|SPARK_T
    // ;;find the lowest coordinate in the box
    // ;	LDI	@BOXSCRAMI,AR1		;Box endpoint coords, set above by BOXSCAN
    // ;	ADDI	24,AR1			;offset to world based
    // ;	LDF	*+AR1(Y),R0
    // ;	LDI	7,RC
    // ;	RPTB	SCANYS
    // ;	CMPF	*+AR1(Y),R0
    // ;	LDILT	AR1,AR2
    // ;	LDFLT	*+AR1(Y),R0
    // ;SCANYS	NOP	*++AR1(3)
    // ;
    // ;;Now set the point of impact t0 this
    // ;	SUBF	60,R2
    // ;	LDF	*+AR2(X),R0
    // ;	STF	R0,*+AR0(PDATA)
    // ;	LDF	*+AR2(Y),R0
    // ;	SUBF	R2,R0
    // ;	STF	R0,*+AR0(PDATA+1)
    // ;	LDF	*+AR2(Z),R0
    // ;	STF	R0,*+AR0(PDATA+2)
    // ;
    // ;;now go make the objects
    // ;	CALL	INIT_COLLA_OBJS
    // ;ROAD_IMPACT_SPARKX
    // ;	CALL	POPALL
    // ;	RETS
}

/*
 *----------------------------------------------------------------------------
 *INPUT	AR4 = CARS OBJECT
 *	AR5 = CARBLK
 */
void SKID_SPARK(void) {
    // asm 00008708: 	RETS
    return;
    // ;	CALL	PUSHALL
    // ;
    // ;	LDI	*+AR4(ODIST),R0
    // ;	CMPI	30000,R0
    // ;	BGT	SKID_SPARKX
    // ;
    // ;	CREATE	SPARK_COLLA_PROC,UTIL_C|SPARK_T
    // ;;Get the grounds elevation
    // ;	LDI	*+AR5(CARTRAK),AR1	;OK if on enbankment, grass dosn't make sparks
    // ;	LDF	*+AR1(OPOSY),R0
    // ;	SUBF	30,R0
    // ;	STF	R0,*+AR0(PDATA+1)
    // ;;Now use the velocity of the car to offset the spark behind it
    // ;	LDF	*+AR4(OVELX),R0
    // ;	MPYF	-0.5,R0
    // ;	ADDF	*+AR4(OPOSX),R0
    // ;	STF	R0,*+AR0(PDATA)
    // ;	LDF	*+AR4(OVELZ),R0
    // ;	MPYF	-0.5,R0
    // ;	ADDF	*+AR4(OPOSZ),R0
    // ;	STF	R0,*+AR0(PDATA+2)
    // ;;Go make the spark objects
    // ;	CALL	INIT_COLLA_OBJS
    // ;;done
    // ;SKID_SPARKX
    // ;	CALL	POPALL
    // ;	RETS
}

/*
 *----------------------------------------------------------------------------
 *RETURNS	R2= number of spark procs
 *		C=1 if too many
 *		C=0 if ok
 */
static int TOO_MANY_SPARKS(void) {
    PROC* proc;
    int num_spark_procs;

    // asm 00008709: 	LDI	0,R2
    num_spark_procs = 0;
    // asm 0000870A: 	LDI	@PACTIVEI,R0
    proc = PACTIVE;
    // asm 0000870B: 	BZ	TMSXCC			;NULL LIST?
    if (proc == NULL) {
        goto TMSXCC;
    }
TMS_LP:
    // asm 0000870C: 	LDI	R0,AR0
    // asm 0000870D: 	LDI	*+AR0(PID),R0
    // asm 0000870E: 	TSTB	UTIL_C,R0
    // asm 0000870F: 	BZ	TMSLPE
    if ((proc->id & UTIL_C) == 0) {
        goto TMSLPE;
    }
    // asm 00008710: 	AND	TYPE_M,R0
    // asm 00008711: 	CMPI	SPARK_T,R0
    // asm 00008712: 	BNE	TMSLPE
    if ((proc->id & TYPE_M) != SPARK_T) {
        goto TMSLPE;
    }
    // asm 00008713: 	ADDI	1,R2
    num_spark_procs += 1;
TMSLPE:
    // asm 00008714: 	LDI	*AR0,R0
    proc = proc->link;
    // asm 00008715: 	BNZ	TMS_LP
    if (proc != NULL) {
        goto TMS_LP;
    }
    // ;	CMPI	5,R2
    // asm 00008716: 	CMPI	2,R2
    // asm 00008717: 	BLT	TMSXCC
    if (num_spark_procs < 2) {
        goto TMSXCC;
    }
    // asm 00008718: 	SETC
    // asm 00008719: 	RETS
    return 1;
TMSXCC:
    // asm 0000871A: 	CLRC
    // asm 0000871B: 	RETS
    return 0;
}

void OBJ_MOVE(OBJ* obj /*AR2*/, c3x_reg_t frame_scale /*R1*/) {
    VECTOR velocity;
    c3x_reg_t movement;

    // asm 0000871C: 	LDI	AR2,AR1
    // asm 0000871D: 	ADDI	OVELX,AR2
    velocity.X = obj->vel_x;
    velocity.Y = obj->vel_y;
    velocity.Z = obj->vel_z;
    // asm 0000871E: 	LDI	AR1,R2
    // asm 0000871F: 	ADDI	OMATRIX,R2
    // asm 00008720: 	LDI	@VECTORAI,R3
    // asm 00008721: 	CALL	MATRIX_MUL
    MATRIX_MUL(&velocity, (MATRIX*)&obj->omatrix, &VECTORAI);
    // asm 00008722: 	LDI	@VECTORAI,AR0
    // asm 00008723: 	LDF	*+AR0(X),R0
    // asm 00008724: 	MPYF	R1,R0
    movement = C3X_MUL(C3X_LDF(VECTORAI.X), frame_scale);
    // asm 00008725: 	ADDF	*+AR1(OPOSX),R0
    movement = C3X_ADD(movement, C3X_LDF(obj->pos.X));
    // asm 00008726: 	STF	R0,*+AR1(OPOSX)
    obj->pos.X = C3X_STF(movement);
    // asm 00008727: 	LDF	*+AR0(Y),R0
    // asm 00008728: 	MPYF	R1,R0
    movement = C3X_MUL(C3X_LDF(VECTORAI.Y), frame_scale);
    // asm 00008729: 	ADDF	*+AR1(OPOSY),R0
    movement = C3X_ADD(movement, C3X_LDF(obj->pos.Y));
    // asm 0000872A: 	STF	R0,*+AR1(OPOSY)
    obj->pos.Y = C3X_STF(movement);
    // asm 0000872B: 	LDF	*+AR0(Z),R0
    // asm 0000872C: 	MPYF	R1,R0
    movement = C3X_MUL(C3X_LDF(VECTORAI.Z), frame_scale);
    // asm 0000872D: 	ADDF	*+AR1(OPOSZ),R0
    movement = C3X_ADD(movement, C3X_LDF(obj->pos.Z));
    // asm 0000872E: 	STF	R0,*+AR1(OPOSZ)
    obj->pos.Z = C3X_STF(movement);
    // asm 0000872F: 	RETS
    return;
}

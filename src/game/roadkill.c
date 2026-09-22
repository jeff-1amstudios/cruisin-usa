
#include "roadkill.h"
#include "../core/machine.h"
#include "cmos.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

extern VECTOR _VECTORA;
extern MATRIX _MATRIXA;

/*
 * Source module: asm/ROADKILL.ASM
 */

void PLYRROADKILL(void);
static void CHECK_COLLISION(OBJ* car_obj /*AR0*/, OBJ* roadkill_obj /*AR1*/);
static void CHECK_OFFSET(OBJ* car_obj /*AR0*/, OBJ* roadkill_obj /*AR1*/, c3x_reg_t offset /*R4*/);
int ROADKILL_FLYERP(OBJ* roadkill_obj /*AR1*/, CARBLK* carblk /*AR5*/);
void ROADKILL_HIT(OBJ* car_obj /*AR0*/, OBJ* roadkill_obj /*AR1*/);
void ROADKILL_SETKILL(OBJ* roadkill_obj /*AR1*/);
int OBJ_MOVE_GROUND(OBJ* obj /*AR4*/, c3x_reg_t height /*R1*/);
OBJ* GET_ROADKILL_TRACK(OBJ* obj /*AR4*/);
int PROC_COUNT(int pid /*R2*/);
void COW_SPAWNER_PROC(PROC* p);
static void COW_PROC(PROC* p);
void DEER_SPAWNER_PROC(PROC* p);
static void DEER_PROC(PROC* p);
static OBJ* INIT_DEER(PROC* p, c3x_reg_t height);
static OBJ* INIT_COW(PROC* p, word_addr_t model, c3x_reg_t height);
void DEER_EXPLODE(OBJ* deer /*AR1*/, CARBLK* car /*AR5*/);
static void MAKE_NOCOLL(OBJ* obj /*AR4*/);
static void DEER_BLOOD_PROC(PROC* p);
void FLYING_PARTS(OBJ* obj /*AR1*/, CARBLK* car /*AR5*/);
static void FLY_PARTS(OBJ* obj /*AR4*/, CARBLK* car /*AR5*/);
void GEESE_SPAWNER(PROC* p);
static void GEESE_PROC(PROC* p);
void BUG_SPAWNER_PROC(PROC* p);
static void SPLAT_PROC(PROC* p);
void DELETE_SPLAT(void);
static LEG_PAYLOAD* NEXT_ROAD(LEG_PAYLOAD* segment /*AR2*/, OBJ* obj /*AR4*/, int direction /*R2*/, int* moved);
static c3x_reg_t GET_ROAD_RADY(LEG_PAYLOAD* segment /*AR2*/, int direction /*R2*/);
static LEG_PAYLOAD* FIND_MAP(int id /*R2*/);

#define ROADKILL_TABI ROADKILL_TAB
#define DEERANII DEERANI
#define GEESEANII GEESEANI
#define GEESE_DIRI GEESE_DIR
#define SHIT_ANII SHIT_ANI
#define BUG_ANII BUG_ANI

extern const char PC1[];

static tROADKILL_TAB_ENTRY ROADKILL_TAB[];
static int COW_PARTS[37];
static int DEER_PARTS[35];
static int DEERANI[7];
static int GEESEANI[9];
static tGEESE_DIR_ENTRY GEESE_DIR[];

#define ROADKILL_TYPES (2 - 1)
#define RKT_DEATH 0  // UH
#define RKT_SND 1    // UH
#define RKT_PARTS 2  // UH
#define RKT_WIDTHL 3 // FL LEFT half the width
#define RKT_WIDTHR 4 // FL RIGHT half the width
#define RKT_SIZE 5
static tROADKILL_TAB_ENTRY ROADKILL_TAB[] = {
    { deerc1_ROM, EXP3, (uintptr_t)COW_PARTS, C3X_F32_INIT(-200), C3X_F32_INIT(200) },  // SOON TO BE A COW
    { deerc1_ROM, EXP3, (uintptr_t)DEER_PARTS, C3X_F32_INIT(-400), C3X_F32_INIT(400) }, // DEER
};
/* asm: ROADKILLXZ	.bss	ROADKILLXZ,2 */
c3x_f32_t ROADKILLXZ[2];
/*
 *----------------------------------------------------------------------------
 *----------------------------------------------------------------------------
 *This checks collisions between the Player and the roadkill. This is needed
 *because the roadkill are to wide to be covered completely by the POINT
 *collision routine.
 *No special list is needed. This routine simply searches for the ID's. Seeing
 *it is only checking agianst the player the overhead is nominal.
 */

void PLYRROADKILL(void)
{
    OBJ* player_obj;
    OBJ* roadkill_obj;

    // asm 00006946: 	LDI	@PLYCAR,AR0
    player_obj = PLYCAR;
    // asm 00006947: 	CMPI	0,AR0
    // asm 00006948: 	BEQ	PLYRKX			;NO player at the moment
    if (player_obj == NULL) {
        goto PLYRKX; // ;NO player at the moment
    }
    // asm 00006949: 	LDI	@OACTIVEI,AR1
    roadkill_obj = OACTIVEI;
    // asm 0000694A: 	LDI	*AR1,R0
    // asm 0000694B: 	BEQ	PLYRKX
    if (roadkill_obj == NULL) {
        goto PLYRKX;
    }
FINDLP:
    // asm 0000694C: 	LDI	R0,AR1
    // asm 0000694D: 	LDI	*+AR1(OID),R0
    // asm 0000694E: 	AND	CLASS_M|TYPE_M,R0
    // asm 0000694F: 	CMPI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm 00006950: 	CALLEQ	CHECK_COLLISION
    if ((roadkill_obj->id & (CLASS_M | TYPE_M)) == (RDDEBRIS_C | TSC_ROADKILL)) {
        CHECK_COLLISION(player_obj, roadkill_obj);
    }
    // asm 00006951: 	LDI	*AR1,R0
    roadkill_obj = roadkill_obj->link;
    // asm 00006952: 	BNZ	FINDLP
    if (roadkill_obj != NULL) {
        goto FINDLP;
    }
PLYRKX:
    // asm 00006953: 	LDF	0,R0
    // asm 00006954: 	STF	R0,@ROADKILLXZ
    ROADKILLXZ[0] = C3X_STF_IMM(0);
    // asm 00006955: 	STF	R0,@ROADKILLXZ+1
    ROADKILLXZ[1] = C3X_STF_IMM(0);
    // asm 00006956: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRROADKILL", 0, 0);
}

/*
 *----------------------------------------------------------------------------
 *AR0	= CAR OBJECT TO CHECK
 *AR1	= ROADKILL OBJECT
 */
static void CHECK_COLLISION(OBJ* car_obj /*AR0*/, OBJ* roadkill_obj /*AR1*/)
{
    int subtype;
    const tROADKILL_TAB_ENTRY* entry;
    // asm 00006957: 	LDI	*+AR1(OID),R0
    // asm 00006958: 	AND	SUBTYPE_M,R0
    subtype = roadkill_obj->id & SUBTYPE_M;
    // asm 00006959: 	BZ	CCOLLX			;PARTS ARE NOT COLLIDEABLE
    if (subtype == 0) goto CCOLLX;
    // asm 0000695A: 	CMPI	TSC_PARTS_S,R0
    // asm 0000695B: 	BEQ	CCOLLX			;NORMALL COLLISION WILL DO IT FOR PARTS
    if (subtype == TSC_PARTS_S) goto CCOLLX;
    // asm 0000695C: 	SUBI	1,R0
    // asm 0000695D: 	MPYI	RKT_SIZE,R0
    // asm 0000695E: 	LDI	R0,AR3			;AR3 = index into roadkill table
    // asm 0000695F: 	ADDI	@ROADKILL_TABI,AR3
    entry = &ROADKILL_TAB[subtype - 1];
    // asm 00006960: 	LDF	*+AR3(RKT_WIDTHL),R4
    // asm 00006961: 	CALL	CHECK_OFFSET
    CHECK_OFFSET(car_obj, roadkill_obj, C3X_LDF(entry->width_left));
    // asm 00006962: 	LDI	*+AR1(OID),R0
    // asm 00006963: 	AND	SUBTYPE_M,R0
    subtype = roadkill_obj->id & SUBTYPE_M;
    // asm 00006964: 	BZ	COFFX			;OBJECT_COLLIDED
    if (subtype == 0) return;
    // asm 00006965: 	LDF	*+AR3(RKT_WIDTHR),R4
    // asm 00006966: 	CALL	CHECK_OFFSET
    CHECK_OFFSET(car_obj, roadkill_obj, C3X_LDF(entry->width_right));
CCOLLX:
    // asm 00006967: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *AR0	= CAR OBJECT TO CHECK
 *AR1	= ROADKILL OBJECT
 *R4	= OFFSET
 */
static void CHECK_OFFSET(OBJ* car_obj /*AR0*/, OBJ* roadkill_obj /*AR1*/, c3x_reg_t offset /*R4*/)
{
    c3x_reg_t theta, displacement;
    // asm 00006968: 	PUSH	AR3
    // asm 00006969: 	LDF	*+AR1(OPOSX),R0		;Set the roadkills real XZ coords
    // asm 0000696A: 	STF	R0,@ROADKILLXZ		;A nonzero XZ will let the roadkill
    ROADKILLXZ[0] = C3X_STF(C3X_LDF(roadkill_obj->pos.X));
    // asm 0000696B: 	LDF	*+AR1(OPOSZ),R0		;know that it must use ROADKILLXZ as its
    // asm 0000696C: 	STF	R0,@ROADKILLXZ+1	;real coords. The road kill will copy them back
    ROADKILLXZ[1] = C3X_STF(C3X_LDF(roadkill_obj->pos.Z));
    // 	;Set the roadkills offset XZ coords
    // asm 0000696D: 	LDP	@_CAMERARAD+X
    // asm 0000696E: 	LDF	@_CAMERARAD+Y,R2
    theta = C3X_LDF(_CAMERARAD.Y);
    // asm 0000696F: 	SETDP
    // asm 00006970: 	CALL	NORMITS
    theta = NORMITS(theta);
    // asm 00006971: 	NEGF	R2
    theta = C3X_NEG(theta);
    // asm 00006972: 	CALL	_COSI
    displacement = _COSI(theta);
    // asm 00006973: 	MPYF	R4,R0
    displacement = C3X_MUL(displacement, offset);
    // asm 00006974: 	ADDF	*+AR1(OPOSX),R0
    displacement = C3X_ADD(displacement, C3X_LDF(roadkill_obj->pos.X));
    // asm 00006975: 	STF	R0,*+AR1(OPOSX)
    roadkill_obj->pos.X = C3X_STF(displacement);
    // asm 00006976: 	CALL	_SINE
    displacement = _SINE(theta);
    // asm 00006977: 	MPYF	R4,R0
    displacement = C3X_MUL(displacement, offset);
    // asm 00006978: 	ADDF	*+AR1(OPOSZ),R0
    displacement = C3X_ADD(displacement, C3X_LDF(roadkill_obj->pos.Z));
    // asm 00006979: 	STF	R0,*+AR1(OPOSZ)
    roadkill_obj->pos.Z = C3X_STF(displacement);
    // asm 0000697A: 	CALL	COLSGCK			;This does the actual collision check
    COLSGCK(car_obj, roadkill_obj);
    // 					;and also calls explode deer if collision exists
    // asm 0000697B: 	LDI	*+AR1(OID),R0
    // asm 0000697C: 	AND	SUBTYPE_M,R0
    // asm 0000697D: 	BZ	COFFX			;OBJECT_COLLIDED
    if ((roadkill_obj->id & SUBTYPE_M) == 0) goto COFFX;
    // asm 0000697E: 	LDF	@ROADKILLXZ,R0
    // asm 0000697F: 	STF	R0,*+AR1(OPOSX)
    roadkill_obj->pos.X = C3X_STF(C3X_LDF(ROADKILLXZ[0]));
    // asm 00006980: 	LDF	@ROADKILLXZ+1,R0
    // asm 00006981: 	STF	R0,*+AR1(OPOSZ)
    roadkill_obj->pos.Z = C3X_STF(C3X_LDF(ROADKILLXZ[1]));
COFFX:
    // asm 00006982: 	POP	AR3
    // asm 00006983: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR1	OBJECT
 *
 *RETURNS
 *	C	IS A FLYER
 *	NC	NOT A FLYER
 *
 *
 */
int ROADKILL_FLYERP(OBJ* roadkill_obj /*AR1*/, CARBLK* carblk /*AR5*/)
{
    c3x_reg_t position_sum;
    // asm 00006984: 	PUSH	R0
    // asm 00006985: 	PUSH	AR2
    // asm 00006986: 	LDI	*+AR1(ODIST),R0
    // asm 00006987: 	CMPI	30000,R0
    // asm 00006988: 	BGT	RKFPX
    if (roadkill_obj->dist > 30000) goto RKFPX;
    // asm 00006989: 	LDI	*+AR1(OID),R0
    // asm 0000698A: 	AND	SUBTYPE_M,R0
    // asm 0000698B: 	CMPI	TSC_PARTS_S,R0
    // asm 0000698C: 	BNE	NORMAL_ROADKILL
    if ((roadkill_obj->id & SUBTYPE_M) != TSC_PARTS_S) goto NORMAL_ROADKILL;
    // asm 0000698D: 	CALL	FLYING_PARTS		;HANDLE COLLISION
    FLYING_PARTS(roadkill_obj, carblk);
    // asm 0000698E: 	BR	RKFPX
    goto RKFPX;
NORMAL_ROADKILL:
    // asm 0000698F: 	CALL	ROADKILL_SETKILL
    ROADKILL_SETKILL(roadkill_obj);
    // asm 00006990: 	LDF	@ROADKILLXZ,R0
    // asm 00006991: 	ADDF	@ROADKILLXZ+1,R0
    position_sum = C3X_ADD(C3X_LDF(ROADKILLXZ[0]), C3X_LDF(ROADKILLXZ[1]));
    // asm 00006992: 	BZ	RKFP
    if (C3X_EQ_IMM(position_sum, 0)) goto RKFP;
    // asm 00006993: 	LDF	@ROADKILLXZ,R0
    // asm 00006994: 	STF	R0,*+AR1(OPOSX)
    roadkill_obj->pos.X = C3X_STF(C3X_LDF(ROADKILLXZ[0]));
    // asm 00006995: 	LDF	@ROADKILLXZ+1,R0
    // asm 00006996: 	STF	R0,*+AR1(OPOSZ)
    roadkill_obj->pos.Z = C3X_STF(C3X_LDF(ROADKILLXZ[1]));
RKFP:
    // asm 00006997: 	CALL	DEER_EXPLODE
    DEER_EXPLODE(roadkill_obj, carblk);
RKFPX:
    // asm 00006998: 	CLRC			;flying taken car of
    // asm 00006999: 	POP	AR2
    // asm 0000699A: 	POP	R0
    // asm 0000699B: 	RETS
    return 0;
}

/* asm: SOUNDTIME	.float	0.00204678	;7(1/60)/57 of a minute */
static const c3x_f32_t SOUNDTIME = C3X_F32_INIT(0.00204678f);
/* asm: ROADKILL_SOUND_TIMER	.bss	ROADKILL_SOUND_TIMER,1 */
c3x_f32_t ROADKILL_SOUND_TIMER;

void ROADKILL_HIT(OBJ* car_obj /*AR0*/, OBJ* roadkill_obj /*AR1*/)
{
    c3x_reg_t elapsed;
    int subtype, sound;
    // asm 0000699C: 	PUSH	R0
    // asm 0000699D: 	PUSH	AR1
    // asm 0000699E: 	PUSH	AR2
    // asm 0000699F: 	PUSH	AR4
    // asm 000069A0: 	LDF	@GAME_TIMER,R0
    // asm 000069A1: 	SUBF	@ROADKILL_SOUND_TIMER,R0
    elapsed = C3X_SUB(C3X_LDF(GAME_TIMER), C3X_LDF(ROADKILL_SOUND_TIMER));
    // asm 000069A2: 	CMPF	@SOUNDTIME,R0
    // asm 000069A3: 	BLT	PKQ			;not enough time has lappsed since the last sound
    if (C3X_LT(elapsed, C3X_LDF(SOUNDTIME))) goto PKQ;
    // asm 000069A4: 	LDF	@GAME_TIMER,R0
    // asm 000069A5: 	STF	R0,@ROADKILL_SOUND_TIMER
    ROADKILL_SOUND_TIMER = C3X_STF(C3X_LDF(GAME_TIMER));
    // asm 000069A6: 	LDI	*+AR1(OID),AR2
    // asm 000069A7: 	AND	SUBTYPE_M,AR2
    subtype = roadkill_obj->id & SUBTYPE_M;
    // asm 000069A8: 	CMPI	0,AR2
    // asm 000069A9: 	BZ	PKQ
    if (subtype == 0) goto PKQ;
    // asm 000069AA: 	CMPI	TSC_PARTS_S,AR2
    // asm 000069AB: 	BZ	PKQ
    if (subtype == TSC_PARTS_S) goto PKQ;
    // asm 000069AC: 	SUBI	1,AR2
    // asm 000069AD: 	MPYI	RKT_SIZE,AR2
    // asm 000069AE: 	ADDI	@ROADKILL_TABI,AR2
    // asm 000069AF: 	LDI	*+AR2(RKT_SND),AR2
    sound = ROADKILL_TAB[subtype - 1].hit_sound;
    // asm 000069B0: 	LDI	*+AR0(OID),R0		;CAR that hit the animal
    // asm 000069B1: 	AND	CLASS_M,R0
    // asm 000069B2: 	CMPI	PLYR_C,R0
    // asm 000069B3: 	BEQ	DO_PLYR
    if ((car_obj->id & CLASS_M) == PLYR_C) goto DO_PLYR;
    // asm 000069B4: 	LDI	AR0,AR4			;A drone hit the animal
    // asm 000069B5: 	CALL	DRONESND1
    DRONESND1(car_obj, sound);
    // asm 000069B6: 	BR	PKQ
    goto PKQ;
DO_PLYR:
    // asm 000069B7: 	CALL	ONESNDFX
    ONESNDFX(sound);
    // ;	LDI	*+AR1(OID),AR2
    // ;	AND	SUBTYPE_M,AR2
    // ;	CMPI	TSC_DEER_S,AR2
    // ;	BNE	PKQ
    // asm 000069B8: 	LDI	MOO,AR2
    // asm 000069B9: 	CALL	KILLSNDFX
    KILLSNDFX(MOO);
    // asm 000069BA: 	LDI	EXP1,AR2
    // asm 000069BB: 	CALL	ONESNDFX
    ONESNDFX(EXP1);
    // ;	LDI	0,R0
    // ;	LDI	0,R1
    // ;	CALL	SET_TRACK_VOL
    // asm 000069BC: 	BU	PKQ
    goto PKQ;
PKQ:
    // asm 000069BD: POP	AR4
    // asm 000069BE: 	POP	AR2
    // asm 000069BF: 	POP	AR1
    // asm 000069C0: 	POP	R0
    // asm 000069C1: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INPUT	AR1 = OBJECT
 *
 *PARAMTERS
 *
 */
void ROADKILL_SETKILL(OBJ* roadkill_obj /*AR1*/)
{
    int subtype;
    // asm 000069C2: 	PUSH	AR0
    // asm 000069C3: 	PUSH	AR3
    // asm 000069C4: 	LDI	*+AR1(OLINK4),AR0
    // asm 000069C5: 	CMPI	-1,AR0
    // asm 000069C6: 	BEQ	NNWAY
    if (roadkill_obj->link4 == (uintptr_t)-1) goto NNWAY;
    // asm 000069C7: 	LDI	*+AR1(OID),AR3
    // asm 000069C8: 	AND	SUBTYPE_M,AR3
    subtype = roadkill_obj->id & SUBTYPE_M;
    // asm 000069C9: 	SUBI	1,AR3
    // asm 000069CA: 	MPYI	RKT_SIZE,AR3
    // asm 000069CB: 	ADDI	@ROADKILL_TABI,AR3
    // asm 000069CC: 	LDI	*+AR3(RKT_DEATH),AR0
    // asm 000069CD: 	STI	AR0,*+AR1(OROMDATA)
    roadkill_obj->romdata = ROM_PTR((word_addr_t)ROADKILL_TAB[subtype - 1].death_romdata);
RS1:
    // asm 000069CE: RS1
    // asm 000069CE: 	LDI	-1,AR0
    // asm 000069CF: 	STI	AR0,*+AR4(OLINK4)
    roadkill_obj->link4 = (uintptr_t)-1;
NNWAY:
    // asm 000069D0: 	POP	AR3
    // asm 000069D1: 	POP	AR0
    // asm 000069D2: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INPUT
 *	R1 = hight off ground
 *
 *OUTPUT
 *	C = 0,no ground under object
 *	C = 1,Ground under object
 */

int OBJ_MOVE_GROUND(OBJ* obj /*AR4*/, c3x_reg_t height /*R1*/)
{
    VECTOR velocity = { obj->vel_x, obj->vel_y, obj->vel_z };
    c3x_reg_t road_height;
    int found;
    // asm 000069D3: 	LDI	AR4,AR2
    // asm 000069D4: 	ADDI	OVELX,AR2
    // asm 000069D5: 	LDI	AR4,R2
    // asm 000069D6: 	ADDI	OMATRIX,R2
    // asm 000069D7: 	LDI	@VECTORAI,R3
    // asm 000069D8: 	CALL	MATRIX_MUL
    MATRIX_MUL(&velocity, (MATRIX*)&obj->omatrix, &VECTORAI);
    // asm 000069D9: 	LDI	@VECTORAI,AR0
    // asm 000069DA: 	LDF	*+AR0(X),R0
    // asm 000069DB: 	ADDF	*+AR4(OPOSX),R0
    // asm 000069DC: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.X), C3X_LDF(obj->pos.X)));
    // asm 000069DD: 	LDF	*+AR0(Z),R0
    // asm 000069DE: 	ADDF	*+AR4(OPOSZ),R0
    // asm 000069DF: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.Z), C3X_LDF(obj->pos.Z)));
    // asm 000069E0: 	PUSH	AR4
    // asm 000069E1: 	PUSHF	R1
    // asm 000069E2: 	ADDI	OPOSX,AR4
    // asm 000069E3: 	CALL	CAMSCAN
    found = CAMSCAN(&obj->pos, &road_height);
    // asm 000069E4: 	POPF	R1
    // asm 000069E5: 	POP	AR4
    // asm 000069E6: 	BNC	OMGX
    if (!found) goto OMGX;
    // asm 000069E7: 	SUBF	R1,R0
    // asm 000069E8: 	ADDF	*+AR4(OPOSY),R0
    // asm 000069E9: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_ADD(C3X_SUB(road_height, height), C3X_LDF(obj->pos.Y)));
    // asm 000069EA: 	SETC
    return 1;
OMGX:
    // asm 000069EB: 	RETS
    return 0;
}

/*
 *----------------------------------------------------------------------------
 *Sets ORADY, OMATRIX, and AR0 to 10 tracks down the road
 *
 *
 */
OBJ* GET_ROADKILL_TRACK(OBJ* obj /*AR4*/)
{
    OBJ* track;
    OBJ* previous;
    OBJ* next;
    int remaining = 20;
    c3x_reg_t dx, dz, theta;
    // asm 000069EC: 	LDI	@PLYCBLK,AR2
    // asm 000069ED: 	LDI	*+AR2(CARTRAK),R0
    // asm 000069EE: 	LDI	R0,AR0
    // asm 000069EF: 	LDI	20,AR1
    track = OBJREF_TO_PTR(PLYCBLK->closest_track_piece);
    next = track;
GRT_FF:
    // asm 000069F0: GRT_FF
    // asm 000069F0: 	LDI	AR0,AR2
    previous = track;
    // asm 000069F1: 	LDI	R0,AR0
    track = next;
    // asm 000069F2: 	LDI	*+AR0(OLINK4),R0
    next = (OBJ*)track->link4;
    // asm 000069F3: 	BEQ	OUT_OF_TRACK
    if (next == NULL) goto OUT_OF_TRACK;
    // asm 000069F4: 	DBU	AR1,GRT_FF
    if (--remaining >= 0) goto GRT_FF;
OUT_OF_TRACK:
    // asm 000069F5: 	LDF	*+AR0(OPOSX),R2
    // asm 000069F6: 	SUBF	*+AR2(OPOSX),R2
    dx = C3X_SUB(C3X_LDF(track->pos.X), C3X_LDF(previous->pos.X));
    // asm 000069F7: 	LDF	*+AR0(OPOSZ),R3
    // asm 000069F8: 	SUBF	*+AR2(OPOSZ),R3
    dz = C3X_SUB(C3X_LDF(track->pos.Z), C3X_LDF(previous->pos.Z));
    // asm 000069F9: 	CALL	ARCTANF
    theta = ARCTANF(dx, dz);
    // asm 000069FA: 	SUBF	HALFPI,R0
    theta = C3X_SUB_IMM(theta, HALFPI);
    // asm 000069FB: 	LDF	R0,R2				;FIND THETA
    // asm 000069FC: 	STF	R2,*+AR4(ORADY)
    obj->rad.Y = C3X_STF(theta);
    // asm 000069FD: 	LDI	AR4,AR2
    // asm 000069FE: 	ADDI	OMATRIX,AR2
    // asm 000069FF: 	CALL	FIND_YMATRIX			;FIND Y MATRIX (FOR LANE OFFSETTING)
    _find_Ymatrix(&obj->omatrix, theta);
    // asm 00006A00: 	RETS
    return track;
}

/*
 *----------------------------------------------------------------------------
 *INPUT		R2= PID to count
 *RETURNS	R0= number of active procs
 *Trashes AR0,R1
 */

int PROC_COUNT(int pid /*R2*/)
{
    int count;
    PROC* proc;
    // asm 00006A01: 	LDI	0,R0
    count = 0;
    // asm 00006A02: 	LDI	@PACTIVEI,R1
    proc = PACTIVE;
    // asm 00006A03: 	RETSZ				;NULL LIST?
    if (proc == NULL) return count;
PCLP:
    // asm 00006A04: 	LDI	R1,AR0
    // asm 00006A05: 	LDI	*+AR0(PID),R1
    // asm 00006A06: 	CMPI	R2,R1
    // asm 00006A07: 	BNE	PC1
    if (proc->id != (u32)pid) goto PC1;
    // asm 00006A08: 	ADDI	1,R0
    ++count;
PC1:
    // asm 00006A09: 	LDI	*AR0,R1
    proc = proc->link;
    // asm 00006A0A: 	BNZ	PCLP
    if (proc != NULL) goto PCLP;
    // asm 00006A0B: 	RETS
    return count;
}


/*
 *----------------------------------------------------------------------------
 * COW_SPAWNER_PROC	PROC
 *This PROC has a COW
 *	CREATE	COW_SPAWNER_PROC,SPAWNER_C
 *
 */
void COW_SPAWNER_PROC(PROC* p)
{
    int palette_code, enabled;
    tPAL* palette;
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    case 2: goto PROC_RESUME_2;
    }
    // asm 00006A0C: 	READADJ	ADJ_ROADKILL
    enabled = READADJ(ADJ_ROADKILL);
    // asm 00006A0E: 	CMPI	0,R0
    // asm 00006A0F: 	BEQ	CSPX
    if (enabled == 0) goto CSPX;
    // asm 00006A10: 	LDI	adblud1_p,AR2		;overwrite the deer parts with the cow palette
    // asm 00006A11: 	CALL	PAL_FIND
    palette_code = PAL_FIND(adblud1_p);
    // asm 00006A12: 	BC	CSPSLEEP
    if (palette_code < 0) goto CSPSLEEP;
    // asm 00006A13: 	LDI	R0,R2
    // asm 00006A14: 	LDL	cow_pal,AR2
    palette = (tPAL*)ROM_PTR(cow_pal_ROM);
    // asm 00006A15: 	LDI	*AR2++,R3
    // asm 00006A16: 	CALL	PAL_SET
    PAL_SET(palette->data, (u32)palette_code, (u32)palette->flags_and_count);
CSPSLEEP:
    // asm 00006A17: 	SLEEP	1			;wait for game to start
    SLEEP(1, 1);
    // asm 00006A19: 	LDI	@_MODE,R0
    // asm 00006A1A: 	AND	MGO,R0
    // asm 00006A1B: 	BZ	CSPSLEEP
    if ((_MODE & MGO) == 0) goto CSPSLEEP;
CSPLP:
    // asm 00006A1C: 	SLEEP	30*20
    SLEEP(30 * 20, 2);
    // ;	SLEEP	30*5
    // asm 00006A1E: 	LDI	500,AR2
    // asm 00006A1F: 	CALL	RANDPER
    // asm 00006A20: 	BNC	CSPLP
    if (!RANDPER(500)) goto CSPLP;
    // asm 00006A21: 	LDI	RDDEBRIS_C|TSC_ROADKILL|TSC_COW_S,R2
    // asm 00006A22: 	CALL	PROC_COUNT
    // asm 00006A23: 	CMPI	1,R0
    // asm 00006A24: 	BGE	CSPLP
    if (PROC_COUNT(RDDEBRIS_C | TSC_ROADKILL | TSC_COW_S) >= 1) goto CSPLP;
    // asm 00006A25: 	CREATE	COW_PROC,RDDEBRIS_C|TSC_ROADKILL|TSC_COW_S
    CREATE(COW_PROC, RDDEBRIS_C | TSC_ROADKILL | TSC_COW_S, NULL);
    // asm 00006A28: 	BR	CSPLP
    goto CSPLP;
CSPX:
    // asm 00006A29: 	DIE
    DIE();
}

/*
 *----------------------------------------------------------------------------
 * COW_PROC	PROC
 *Maintains a COW
 *	CREATE	COW_PROC,RDDEBRIS_C|TSC_ROADKILL|TSC_COW_S
 */
#define LOOP_COUNT PDATA
#define TOTAL_FRAMES (PDATA + 1)

static void COW_PROC(PROC* p)
{
    c3x_reg_t loop_count, total_frames, average_frames, frames_to_cow, speed;
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    case 2: goto PROC_RESUME_2;
    }
    // asm 00006A2A: 	LDL	kow3,AR2
    // asm 00006A2B: 	FLOAT	10,R4			;Hight of a cow
    // asm 00006A2C: 	CALL	INIT_COW
    p->ctx.COW_PROC.obj = INIT_COW(p, kow3_ROM, C3X_FROM_INT(10));
    // asm 00006A2D: 	BC	COW_DIE
    if (p->ctx.COW_PROC.obj == NULL) goto COW_DIE;
    // asm 00006A2E: 	LDF	0,R0
    // asm 00006A2F: 	STF	R0,*+AR7(LOOP_COUNT)
    p->ctx.COW_PROC.loop_count = C3X_STF_IMM(0);
    // asm 00006A30: 	STF	R0,*+AR7(TOTAL_FRAMES)
    p->ctx.COW_PROC.total_frames = C3X_STF_IMM(0);
COW_WAIT:
    // asm 00006A31: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00006A33: 	LDF	1,R1
    loop_count = C3X_IMM_F32(1);
    // asm 00006A34: 	ADDF	*+AR7(LOOP_COUNT),R1
    loop_count = C3X_ADD(loop_count, C3X_LDF(p->ctx.COW_PROC.loop_count));
    // asm 00006A35: 	STF	R1,*+AR7(LOOP_COUNT)
    p->ctx.COW_PROC.loop_count = C3X_STF(loop_count);
    // asm 00006A36: 	FLOAT	@NFRAMES,R0
    total_frames = C3X_FROM_INT(NFRAMES);
    // asm 00006A37: 	ADDF	*+AR7(TOTAL_FRAMES),R0
    total_frames = C3X_ADD(total_frames, C3X_LDF(p->ctx.COW_PROC.total_frames));
    // asm 00006A38: 	STF	R0,*+AR7(TOTAL_FRAMES)
    p->ctx.COW_PROC.total_frames = C3X_STF(total_frames);
    // asm 00006A39: 	CALL	DIV_F
    average_frames = DIV_F(total_frames, loop_count);
    // asm 00006A3A: 	LDF	R0,R1
    // asm 00006A3B: 	LDI	@PLYCBLK,AR5
    // asm 00006A3C: 	MPYF	*+AR5(CARSPEED),R1
    speed = C3X_MUL(average_frames, C3X_LDF(PLYCBLK->speed));
    // asm 00006A3D: 	MPYF	1.5,R1			;CARSPEED FUDGE 'O RAMA
    speed = C3X_MUL_IMM(speed, 1.5);
    // asm 00006A3E: 	FLOAT	*+AR4(ODIST),R0
    frames_to_cow = C3X_FROM_INT(p->ctx.COW_PROC.obj->dist);
    // asm 00006A3F: 	CALL	DIV_F
    frames_to_cow = DIV_F(frames_to_cow, speed);
    // asm 00006A40: 	CMPF	20,R0			;APPROX NUMBER OF FRAMES TO COW
    // asm 00006A41: 	BGT	COW_WAIT
    if (C3X_GT_IMM(frames_to_cow, 20)) goto COW_WAIT;
    // asm 00006A42: 	SONDFX	MOO
    ONESNDFX(MOO);
COW_SLEEP:
    // asm 00006A44: 	SLEEP	100			;THIS IS NEEDED FOR COW COUNTING
    SLEEP(100, 2);
    // asm 00006A46: 	BR	COW_SLEEP
    goto COW_SLEEP;
COW_DIE:
    // asm 00006A47: 	DIE
    DIE();
}

/* asm: DEERANI */
/* asm: 	.word	edeer,edeer1,edeer2,edeer3,edeer4,edeer5,-1 */
/* asm: 	 */
static int DEERANI[] = {
    edeer_ROM,
    edeer1_ROM,
    edeer2_ROM,
    edeer3_ROM,
    edeer4_ROM,
    edeer5_ROM,
    -1,
};

/*
 *----------------------------------------------------------------------------
 * DEER_SPAWNER_PROC	PROC
 *Maintains several DEER animations
 *	CREATE	DEER_SPAWNER_PROC,SPAWNER_C
 *
 */
void DEER_SPAWNER_PROC(PROC* p)
{
    int enabled;
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    case 2: goto PROC_RESUME_2;
    }
    // asm 00006A48: 	READADJ	ADJ_ROADKILL
    enabled = READADJ(ADJ_ROADKILL);
    // asm 00006A4A: 	CMPI	0,R0
    // asm 00006A4B: 	BEQ	DSPX
    if (enabled == 0) goto DSPX;
    // asm 00006A4C: 	SLEEP	1			;wait for game to start
    SLEEP(1, 1);
    // asm 00006A4E: 	LDI	@_MODE,R0
    // asm 00006A4F: 	AND	MGO,R0
    // asm 00006A50: 	BZ	DEER_SPAWNER_PROC
    if ((_MODE & MGO) == 0) REENTER(DEER_SPAWNER_PROC);
DSPLP:
    // asm 00006A51: 	SLEEP	30*20
    SLEEP(30 * 20, 2);
    // ;	SLEEP	30*2
    // asm 00006A53: 	LDI	500,AR2
    // asm 00006A54: 	CALL	RANDPER
    // asm 00006A55: 	BNC	DSPLP
    if (!RANDPER(500)) goto DSPLP;
    // asm 00006A56: 	LDI	RDDEBRIS_C|TSC_ROADKILL|TSC_DEER_S,R2
    // asm 00006A57: 	CALL	PROC_COUNT
    // asm 00006A58: 	CMPI	1,R0
    // asm 00006A59: 	BGE	DSPLP
    if (PROC_COUNT(RDDEBRIS_C | TSC_ROADKILL | TSC_DEER_S) >= 1) goto DSPLP;
    // asm 00006A5A: 	CREATE	DEER_PROC,RDDEBRIS_C|TSC_ROADKILL|TSC_DEER_S
    CREATE(DEER_PROC, RDDEBRIS_C | TSC_ROADKILL | TSC_DEER_S, NULL);
    // asm 00006A5D: 	BR	DSPLP
    goto DSPLP;
DSPX:
    // asm 00006A5E: 	DIE
    DIE();
}

/*
 *----------------------------------------------------------------------------
 * DEER_PROC	PROC
 *Maintains several DEER animations
 *	CREATE	DEER_PROC,RDDEBRIS_C|TSC_ROADKILL|TSC_DEER_S
 */

static void DEER_PROC(PROC* p)
{
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    }
    // asm 00006A5F: 	FLOAT	219,R4			;Hight of a deer
    // asm 00006A60: 	CALL	INIT_DEER
    p->ctx.DEER_PROC.obj = INIT_DEER(p, C3X_FROM_INT(219));
    // asm 00006A61: 	BC	DEER_DIE
    if (p->ctx.DEER_PROC.obj == NULL) goto DEER_DIE;
    // asm 00006A62: 	LDI	@DEERANII,AR6
    p->ctx.DEER_PROC.animation_index = 0;
DEERANI_LOOP:
    // asm 00006A63: 	LDI	*++AR6,R0
    ++p->ctx.DEER_PROC.animation_index;
    // asm 00006A64: 	LDIN	@DEERANII,AR6
    if (DEERANI[p->ctx.DEER_PROC.animation_index] < 0) p->ctx.DEER_PROC.animation_index = 0;
    // asm 00006A65: 	LDI	*AR6,R0
    // asm 00006A66: 	STI	R0,*+AR4(OROMDATA)
    p->ctx.DEER_PROC.obj->romdata = ROM_PTR(DEERANI[p->ctx.DEER_PROC.animation_index]);
DEER_SLEEP:
    // asm 00006A67: DEER_SLEEP
    // asm 00006A67: 	SLEEP	3			;frame rate of 3
    SLEEP(3, 1);
    // asm 00006A69: 	BR	DEERANI_LOOP		;Ultimatly backgrnd or colla will kill this proc
    goto DEERANI_LOOP;
DEER_DIE:
    // asm 00006A6A: 	DIE
    DIE();
}

/*
 *----------------------------------------------------------------------------
 * INIT_DEER
 *INPUT R4 = hight to place off ground
 *Creates and maintains a DEER animation
 */

static OBJ* INIT_DEER(PROC* p, c3x_reg_t height)
{
    // asm 00006A6B: 	LDI	@DEERANII,AR6
    // asm 00006A6C: 	LDI	*AR6,AR2
    return INIT_COW(p, (word_addr_t)DEERANI[0], height);
}

static OBJ* INIT_COW(PROC* p, word_addr_t model, c3x_reg_t height)
{
    OBJ* obj;
    OBJ* track;
    c3x_reg_t x_offset, z_offset, side, limit, side_distance;
    // asm 00006A6D: 	LDF	1,R6
    side = C3X_IMM_F32(1);
    // asm 00006A6E: 	CALL	OBJ_GETE
    obj = OBJ_GETE(ROM_PTR(model));
    // asm 00006A6F: 	BC	INIT_DEERX
    if (obj == NULL) goto INIT_DEERX;
    // asm 00006A70: 	LDI	AR0,AR4
    // asm 00006A71: 	CALL	ADD_RDDEBRIS
    ADD_RDDEBRIS(obj);
    // asm 00006A72: 	LDI	1,R0			;PROC ME BABY
    // asm 00006A73: 	LS	O_PROC_B,R0
    // asm 00006A74: 	OR	*+AR4(OFLAGS),R0
    // asm 00006A75: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= O_PROC;
    // asm 00006A76: 	STI	AR7,*+AR4(OPLINK)
    obj->plink = p;
    // ;	LDI	RDDEBRIS_C|TSC_ROADKILL|TSC_DEER_S,R0
    // asm 00006A77: 	LDI	*+AR7(PID),R0		;MAKE SAME AS PROC
    // asm 00006A78: 	STI	R0,*+AR4(OID)
    obj->id = p->id;
    // asm 00006A79: 	CALL	GET_ROADKILL_TRACK
    track = GET_ROADKILL_TRACK(obj);
    // asm 00006A7A: 	FLOAT	500,R0			;calculate the random offset that this obj will have
    limit = C3X_FROM_INT(500);
    // asm 00006A7B: 	CALL	SFRAND
    x_offset = SFRAND(limit);
    // asm 00006A7C: 	LDF	R0,R1
    // asm 00006A7D: 	FLOAT	500,R0
    limit = C3X_FROM_INT(500);
    // asm 00006A7E: 	CALL	SFRAND
    z_offset = SFRAND(limit);
    // asm 00006A7F: 	LDF	R0,R2
    // asm 00006A80: 	LDF	*+AR0(OPOSX),R0			;set to road position
    // asm 00006A81: 	ADDF	R1,R0
    // asm 00006A82: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(track->pos.X), x_offset));
    // asm 00006A83: 	LDF	*+AR0(OPOSY),R0
    // asm 00006A84: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_LDF(track->pos.Y));
    // asm 00006A85: 	LDF	*+AR0(OPOSZ),R0
    // asm 00006A86: 	ADDF	R2,R0
    // asm 00006A87: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(track->pos.Z), z_offset));
    // asm 00006A88: 	FLOAT	750,R0			;Side of road
    side_distance = C3X_FROM_INT(750);
    // asm 00006A89: 	MPYF	R6,R0
    side_distance = C3X_MUL(side_distance, side);
    // asm 00006A8A: 	STF	R0,*+AR4(OVELX)
    obj->vel_x = C3X_STF(side_distance);
    // asm 00006A8B: 	LDF	R4,R1			;Hight above ground
    // asm 00006A8C: 	CALL	OBJ_MOVE_GROUND		;Offset the starting point
    OBJ_MOVE_GROUND(obj, height);
    // asm 00006A8D: 	LDI	AR4,AR2
    // asm 00006A8E: 	CALL	OBJ_INSERT
    OBJ_INSERT(obj);
    // asm 00006A8F: 	CLRC
    return obj;
INIT_DEERX:
    // asm 00006A90: 	RETS
    return NULL;
}

/* asm: SPINSPEEDF	.float	0.0002 */
/* asm: 	 */
static const c3x_f32_t SPINSPEEDF = C3X_F32_INIT(0.0002f);
/* asm: COW_PARTS */
/* asm: 	.word	deerc1,1,deerc1,1,deerc2,0,deerc2,0,deerc3,0,deerc4,0 */
/* asm: 	.word	deerc1,1,deerc1,1,deerc2,0,deerc2,0,deerc3,0,deerc4,0 */
/* asm: 	.word	deerc1,1,deerc1,1,deerc2,0,deerc2,0,deerc3,0,deerc4,0,-1 */
/* asm: 	 */
static int COW_PARTS[] = {
    deerc1_ROM,
    1,
    deerc1_ROM,
    1,
    deerc2_ROM,
    0,
    deerc2_ROM,
    0,
    deerc3_ROM,
    0,
    deerc4_ROM,
    0,
    deerc1_ROM,
    1,
    deerc1_ROM,
    1,
    deerc2_ROM,
    0,
    deerc2_ROM,
    0,
    deerc3_ROM,
    0,
    deerc4_ROM,
    0,
    deerc1_ROM,
    1,
    deerc1_ROM,
    1,
    deerc2_ROM,
    0,
    deerc2_ROM,
    0,
    deerc3_ROM,
    0,
    deerc4_ROM,
    0,
    -1,
};
/* asm: DEER_PARTS */
/* asm: 	.word	deerc1,1,deerc1,1,deerc2,0,deerc2,0,deerc3,0 */
/* asm: 	.word	deerc4,0,antler,0,antler,0,dheada,0 */
/* asm: 	.word	dheada,1,dheada,1,deerc1,1,deerc1,1,deerc2,0 */
/* asm: 	.word	deerc2,0,deerc3,0,deerc4,0,-1 */
/* asm: 	 */
static int DEER_PARTS[] = {
    deerc1_ROM,
    1,
    deerc1_ROM,
    1,
    deerc2_ROM,
    0,
    deerc2_ROM,
    0,
    deerc3_ROM,
    0,
    deerc4_ROM,
    0,
    antler_ROM,
    0,
    antler_ROM,
    0,
    dheada_ROM,
    0,
    dheada_ROM,
    1,
    dheada_ROM,
    1,
    deerc1_ROM,
    1,
    deerc1_ROM,
    1,
    deerc2_ROM,
    0,
    deerc2_ROM,
    0,
    deerc3_ROM,
    0,
    deerc4_ROM,
    0,
    -1,
};

void DEER_EXPLODE(OBJ* deer /*AR1*/, CARBLK* car /*AR5*/)
{
    const int* parts;
    OBJ* part;
    c3x_reg_t offset, spin_speed, abs_speed, spin_factor;
    int model, no_collision;
    // asm 00006A91: 	CALL	PUSHALL
    // asm 00006A92: 	LDI	AR1,AR6		;save off pointer to deer's object
    // asm 00006A93: 	LDI	*+AR6(OID),R0
    // asm 00006A94: 	AND	SUBTYPE_M,R0
    // asm 00006A95: 	SUBI	1,R0
    // asm 00006A96: 	MPYI	RKT_SIZE,R0
    // asm 00006A97: 	LDI	R0,AR1
    // asm 00006A98: 	ADDI	@ROADKILL_TABI,AR1
    // asm 00006A99: 	LDI	*+AR1(RKT_PARTS),AR1
    parts = (const int*)ROADKILL_TAB[(deer->id & SUBTYPE_M) - 1].parts_list;
    // ;	LDI	@DEER_PARTSI,AR1
    // asm 00006A9A: 	LDI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm 00006A9B: 	STI	R0,*+AR6(OID)
    deer->id = RDDEBRIS_C | TSC_ROADKILL;
MAKE_PARTSLP:
    // asm 00006A9C: 	LDI	*AR1++,R0
    model = *parts++;
    // asm 00006A9D: 	BN	DEER_EXPX
    if (model < 0) goto DEER_EXPX;
    // asm 00006A9E: 	LDI	R0,AR2
    // asm 00006A9F: 	CALL	OBJ_GETE
    part = OBJ_GETE(ROM_PTR((word_addr_t)model));
    // asm 00006AA0: 	BC	DEER_EXPX
    if (part == NULL) goto DEER_EXPX;
    // asm 00006AA1: 	PUSH	AR1
    // asm 00006AA2: 	LDI	AR0,AR4
    // asm 00006AA3: 	CALL	ADD_RDDEBRIS
    ADD_RDDEBRIS(part);
    // asm 00006AA4: 	LDI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm 00006AA5: 	STI	R0,*+AR4(OID)
    part->id = RDDEBRIS_C | TSC_ROADKILL;
    // asm 00006AA6: 	LDF	*+AR6(OPOSX),R0
    // asm 00006AA7: 	STF	R0,*+AR4(OPOSX)
    part->pos.X = C3X_STF(C3X_LDF(deer->pos.X));
    // asm 00006AA8: 	LDF	*+AR6(OPOSY),R0
    // asm 00006AA9: 	STF	R0,*+AR4(OPOSY)
    part->pos.Y = C3X_STF(C3X_LDF(deer->pos.Y));
    // asm 00006AAA: 	LDF	*+AR6(OPOSZ),R0
    // asm 00006AAB: 	STF	R0,*+AR4(OPOSZ)
    part->pos.Z = C3X_STF(C3X_LDF(deer->pos.Z));
    // asm 00006AAC: 	CALL	FLY_PARTS
    FLY_PARTS(part, car);
    // asm 00006AAD: 	LDI	AR4,AR2
    // asm 00006AAE: 	CALL	OBJ_INSERT
    OBJ_INSERT(part);
    // asm 00006AAF: 	POP	AR1
    // asm 00006AB0: 	LDI	*AR1++,R0
    no_collision = *parts++;
    // asm 00006AB1: 	CALLNE	MAKE_NOCOLL
    if (no_collision != 0) MAKE_NOCOLL(part);
    // asm 00006AB2: 	BR	MAKE_PARTSLP
    goto MAKE_PARTSLP;
DEER_EXPX:
    // asm 00006AB3: 	LDI	AR6,AR4
    // asm 00006AB4: 	NEGF	*+AR4(OVELX),R7		;Deer distance from center
    offset = C3X_NEG(C3X_LDF(deer->vel_x));
    // asm 00006AB5: 	CALL	FLY_PARTS
    FLY_PARTS(deer, car);
    // asm 00006AB6: 	CREATEC	DEER_BLOOD_PROC,UTIL_C
    CREATEC(CURRENT_PROC, DEER_BLOOD_PROC, UTIL_C, &(PROC_CONTEXT){ .DEER_BLOOD_PROC = { .obj = deer, .car = car } });
    // asm 00006AB9: 	LDI	@CAMVIEW,R0
    // asm 00006ABA: 	BZ	NO_SPINOUT		;First person
    if (CAMVIEW == 0) goto NO_SPINOUT;
    // asm 00006ABB: 	LDF	*+AR5(CARDIST2CNTR),R0
    // asm 00006ABC: 	SUBF	R0,R7			;Deer distance from center
    offset = C3X_SUB(offset, C3X_LDF(car->dist_to_center));
    // asm 00006ABD: 	LDF	*+AR5(CARSPEED),R0	;SPINSPEED = SPEED+distance to center of deer
    spin_speed = C3X_LDF(car->speed);
    // asm 00006ABE: 	CMPF	0,R7
    // asm 00006ABF: 	BGE	NO_NEG
    if (C3X_GE_IMM(offset, 0)) goto NO_NEG;
    // asm 00006AC0: 	NEGF	R0
    spin_speed = C3X_NEG(spin_speed);
NO_NEG:
    // asm 00006AC1: 	ADDF	R7,R0
    spin_speed = C3X_ADD(spin_speed, offset);
    // asm 00006AC2: 	LDF	@SPINSPEEDF,R1
    spin_factor = C3X_LDF(SPINSPEEDF);
    // asm 00006AC3: 	MPYF	R1,R0			;SPIN SPEED factor adjustment
    spin_speed = C3X_MUL(spin_speed, spin_factor);
    // asm 00006AC4: 	ABSF	R0,R1
    abs_speed = C3X_ABS(spin_speed);
    // asm 00006AC5: 	CMPF	0.06,R1
    // asm 00006AC6: 	BLT	NO_SPINOUT		;To slow to spin
    if (C3X_LT_IMM(abs_speed, 0.06)) goto NO_SPINOUT;
    // asm 00006AC7: 	STF	R0,*+AR5(CARDROT)
    car->last_y_rotation = C3X_STF(spin_speed);
    // asm 00006AC8: 	LDF	3.14,R0
    // asm 00006AC9: 	STF	R0,*+AR5(CARSPRAD)
    car->spin_radians = C3X_STF_IMM(3.14);
    // asm 00006ACA: 	LDI	1,R0
    // asm 00006ACB: 	STI	R0,*+AR5(CAR_SPIN)
    car->spin_flag = 1;
NO_SPINOUT:
    // asm 00006ACC: 	LDF	*+AR5(CARSPEED),R0		;Nuke the car that hit it
    // asm 00006ACD: 	MPYF	0.5,R0
    spin_speed = C3X_LDF(car->speed);
    // asm 00006ACE: 	STF	R0,*+AR5(CARSPEED)
    car->speed = C3X_STF(C3X_MUL_IMM(spin_speed, 0.5));
    // asm 00006ACF: 	CALL	POPALL
    // asm 00006AD0: 	RETS
    return;
}

static void MAKE_NOCOLL(OBJ* obj /*AR4*/)
{
    // asm 00006AD1: 	LDI	RDDEBRIS_C|TSC_ROADKILL|TSC_PARTS_S,R0
    // asm 00006AD2: 	STI	R0,*+AR4(OID)
    obj->id = RDDEBRIS_C | TSC_ROADKILL | TSC_PARTS_S;
    // asm 00006AD3: 	LDI	*+AR4(OFLAGS),R0
    // asm 00006AD4: 	ANDN	O_NOCOLL,R0
    // asm 00006AD5: 	STI	R0,*+AR4(OFLAGS)
    obj->flags &= ~O_NOCOLL;
    // asm 00006AD6: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *DEER_BLOOD_PROC	PROC
 *
 *INPUT
 *	AR4	= THE DEER
 *	AR5	= CARBLK THAT HIT THE DEER
 *Creates and maintains a single blood animation
 *	CREATEC	DEER_BLOOD_PROC,UTIL_C
 */

/* asm: DEERBLOOD_ANI	.word	adblud1,adblud2,adblud3,adblud4,adblud5,adblud6,-1 */
static int DEERBLOOD_ANI[] = {
    adblud1_ROM,
    adblud2_ROM,
    adblud3_ROM,
    adblud4_ROM,
    adblud5_ROM,
    adblud6_ROM,
    -1,
};

static void DEER_BLOOD_PROC(PROC* p)
{
    OBJ* blood;
    OBJ* deer;
    CARBLK* car;
    c3x_reg_t angle, sine, cosine, speed, blood_y;
    int model;
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    }
    // asm 00006AD7: 	LDL	DEERBLOOD_ANI,AR6
    deer = p->ctx.DEER_BLOOD_PROC.obj;
    car = p->ctx.DEER_BLOOD_PROC.car;
    p->ctx.DEER_BLOOD_PROC.animation_index = 0;
    // asm 00006AD8: 	LDI	*AR6++,AR2
    // asm 00006AD9: 	CALL	OBJ_GETE
    blood = OBJ_GETE(ROM_PTR((word_addr_t)DEERBLOOD_ANI[0]));
    // asm 00006ADA: 	BC	DBP_DIE
    if (blood == NULL) goto DBP_DIE;
    p->ctx.DEER_BLOOD_PROC.obj = blood;
    // asm 00006ADB: 	ORM	O_POSTER|O_NOCOLL,*+AR0(OFLAGS)
    blood->flags |= O_POSTER | O_NOCOLL;
    // asm 00006ADE: 	LDF	*+AR4(OPOSX),R0
    // asm 00006ADF: 	STF	R0,*+AR0(OPOSX)
    blood->pos.X = C3X_STF(C3X_LDF(deer->pos.X));
    // asm 00006AE0: 	LDF	*+AR4(OPOSY),R0
    // asm 00006AE1: 	ADDF	-150,R0
    blood_y = C3X_ADD_IMM(C3X_LDF(deer->pos.Y), -150);
    // asm 00006AE2: 	STF	R0,*+AR0(OPOSY)
    blood->pos.Y = C3X_STF(blood_y);
    // asm 00006AE3: 	LDF	*+AR4(OPOSZ),R0
    // asm 00006AE4: 	STF	R0,*+AR0(OPOSZ)
    blood->pos.Z = C3X_STF(C3X_LDF(deer->pos.Z));
    // asm 00006AE5: 	LDI	AR0,AR4
    // asm 00006AE6: 	LDF	*+AR5(CARVROT),R2
    angle = C3X_LDF(car->y_velocity_rotation);
    // asm 00006AE7: 	CALL	_SINE
    sine = _SINE(angle);
    // asm 00006AE8: 	NEGF	R0,R3
    sine = C3X_NEG(sine);
    // asm 00006AE9: 	CALL	_COSI
    cosine = _COSI(angle);
    // asm 00006AEA: 	LDF	*+AR5(CARSPEED),R1	;GET CURRENT SPEED
    speed = C3X_LDF(car->speed);
    // asm 00006AEB: 	MPYF	1.5,R1
    speed = C3X_MUL_IMM(speed, 1.5);
    // asm 00006AEC: 	MPYF	1.5,R1			;150% the speed of the car
    speed = C3X_MUL_IMM(speed, 1.5);
    // asm 00006AED: 	MPYF	R1,R3
    sine = C3X_MUL(sine, speed);
    // asm 00006AEE: 	MPYF	R1,R0
    cosine = C3X_MUL(cosine, speed);
    // asm 00006AEF: 	STF	R3,*+AR4(OVELX)	       	;SETUP VELOCITIES
    blood->vel_x = C3X_STF(sine);
    // asm 00006AF0: 	STF	R0,*+AR4(OVELZ)
    blood->vel_z = C3X_STF(cosine);
    // asm 00006AF1: 	CALL	OVELNADD		;Update position based on velocity
    OVELNADD(blood);
    // asm 00006AF2: 	LDI	AR4,AR2
    // asm 00006AF3: 	CALL	OBJ_INSERT
    OBJ_INSERT(blood);
DBP_LP:
    // asm 00006AF4: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00006AF6: 	LDI	*AR6++,R0
    model = DEERBLOOD_ANI[++p->ctx.DEER_BLOOD_PROC.animation_index];
    // asm 00006AF7: 	BN	DBP_DONE
    if (model < 0) goto DBP_DONE;
    // asm 00006AF8: 	STI	R0,*+AR4(OROMDATA)
    p->ctx.DEER_BLOOD_PROC.obj->romdata = ROM_PTR((word_addr_t)model);
    // asm 00006AF9: 	CALL	OVELNADD		;Update position based on velocity
    OVELNADD(p->ctx.DEER_BLOOD_PROC.obj);
    // asm 00006AFA: 	BR	DBP_LP
    goto DBP_LP;
DBP_DONE:
    // asm 00006AFB: 	LDI	AR4,AR2
    // asm 00006AFC: 	CALL	OBJ_DELETE
    OBJ_DELETE(p->ctx.DEER_BLOOD_PROC.obj);
DBP_DIE:
    // asm 00006AFD: 	DIE
    DIE();
}

/*
 *----------------------------------------------------------------------------
 *CALLABLE FROM COLLA
 */
void FLYING_PARTS(OBJ* obj /*AR1*/, CARBLK* car /*AR5*/)
{
    // asm 00006AFE: 	CALL	PUSHALL
    // asm 00006AFF: 	LDI	AR1,AR4
    // asm 00006B00: 	CALL	FLY_PARTS
    FLY_PARTS(obj, car);
    // asm 00006B01: 	CALL	MAKE_NOCOLL
    MAKE_NOCOLL(obj);
    // asm 00006B02: 	CALL	POPALL
    // asm 00006B03: 	RETS
    return;
}

/*
 *----------------------------------------------------------------------------
 *
 *FLY PARTS			;YES THIS WAS RIPPED OF FROM COLLA
 *	AR5	POINTS TO CAR OBJECT
 *	AR4	POINTS TO OBJECT HIT
 *This routine is needed to insure that the parts do fly
 *When I just simply added them the would not collide with the cars if they were going fast
 */

static void FLY_PARTS(OBJ* obj /*AR4*/, CARBLK* car /*AR5*/)
{
    c3x_reg_t scale, random_angle, random_speed, sine, cosine, speed, vertical, base, minimum;
    PROC_CONTEXT* ctx;
    PROC* fly_proc;
    // asm 00006B04: 	LDI	@CAMVIEW,R0
    // asm 00006B05: 	LDFZ	1.5,R6			;First person
    // asm 00006B06: 	LDFNZ	2.4,R6			;Not first person
    scale = (CAMVIEW == 0) ? C3X_IMM_F32(1.5) : C3X_IMM_F32(2.4);
    // asm 00006B07: 	LDF	1.0,R1
    // asm 00006B08: 	LDF	0.12,R0 		;ADD RANDOM ROTATION
    random_angle = C3X_IMM_F32(0.12);
    // asm 00006B09: 	MPYF	R6,R0
    random_angle = C3X_MUL(random_angle, scale);
    // asm 00006B0A: 	CALL	SFRAND
    random_angle = SFRAND(random_angle);
    // asm 00006B0B: 	LDF	R0,R2
    // asm 00006B0C: 	ADDF	*+AR5(CARVROT),R2
    random_angle = C3X_ADD(random_angle, C3X_LDF(car->y_velocity_rotation));
    // asm 00006B0D: 	LDF	0.50,R0			;RANDOM SPEED MULTIPLIER
    random_speed = C3X_IMM_F32(0.50);
    // asm 00006B0E: 	MPYF	R6,R0
    random_speed = C3X_MUL(random_speed, scale);
    // asm 00006B0F: 	CALL	FRAND
    random_speed = FRAND(random_speed);
    // asm 00006B10: 	LDF	R0,R1
    // asm 00006B11: 	ADDF	0.7,R1
    random_speed = C3X_ADD_IMM(random_speed, 0.7);
    // ;	MPYF	1.3,R1			;SPEEDFUDGE FACTOR
    // asm 00006B12: 	CALL	_SINE
    sine = _SINE(random_angle);
    // asm 00006B13: 	NEGF	R0,R3
    sine = C3X_NEG(sine);
    // asm 00006B14: 	CALL	_COSI
    cosine = _COSI(random_angle);
    // asm 00006B15: 	LDF	*+AR5(CARSPEED),R2	;GET CURRENT SPEED
    speed = C3X_LDF(car->speed);
    // asm 00006B16: 	CMPF	100,R2
    // asm 00006B17: 	LDFLT	100,R2
    if (C3X_LT_IMM(speed, 100)) speed = C3X_IMM_F32(100);
    // asm 00006B18: 	MPYF	R2,R1
    random_speed = C3X_MUL(random_speed, speed);
    // asm 00006B19: 	MPYF	R1,R3
    sine = C3X_MUL(sine, random_speed);
    // asm 00006B1A: 	MPYF	R1,R0
    cosine = C3X_MUL(cosine, random_speed);
    // asm 00006B1B: 	STF	R3,*+AR4(OVELX)	       	;SETUP VELOCITIES
    obj->vel_x = C3X_STF(sine);
    // asm 00006B1C: 	STF	R0,*+AR4(OVELZ)
    obj->vel_z = C3X_STF(cosine);
    // asm 00006B1D: 	LDF	-0.05,R0
    vertical = C3X_IMM_F32(-0.05);
    // asm 00006B1E: 	MPYF	R6,R0
    vertical = C3X_MUL(vertical, scale);
    // asm 00006B1F: 	CALL	FRAND
    vertical = FRAND(vertical);
    // asm 00006B20: 	LDF	-0.10,R1
    base = C3X_IMM_F32(-0.10);
    // asm 00006B21: 	MPYF	R6,R1
    base = C3X_MUL(base, scale);
    // asm 00006B22: 	ADDF	R1,R0
    vertical = C3X_ADD(vertical, base);
    // asm 00006B23: 	MPYF	1.5,R0			;SPEEDFUDGE FACTOR
    vertical = C3X_MUL_IMM(vertical, 1.5);
    // ;	MPYF	*+AR5(CARSPEED),R0	;GET CURRENT SPEED
    // asm 00006B24: 	MPYF	R2,R0
    vertical = C3X_MUL(vertical, speed);
    // asm 00006B25: 	LDF	-27,R1
    minimum = C3X_IMM_F32(-27);
    // asm 00006B26: 	MPYF	R6,R1
    minimum = C3X_MUL(minimum, scale);
    // asm 00006B27: 	CMPF	R1,R0
    // asm 00006B28: 	LDFLT	R1,R0		  	;MAX VERTICAL VELOCITY
    if (C3X_LT(vertical, minimum)) vertical = minimum;
    // asm 00006B29: 	STF	R0,*+AR4(OVELY)		;STUFF VERTICAL VELOCITY
    obj->vel_y = C3X_STF(vertical);
    // asm 00006B2A: 	LDI	1,R0
    // asm 00006B2B: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm 00006B2C: 	TSTB	*+AR4(OFLAGS),R0	;PROCESS ALREADY ACTIVE	?
    // asm 00006B2D: 	LDINZ	*+AR4(OPLINK),AR2	;YES, KILL HIM OFF...
    // asm 00006B2E: 	CALLNZ	PRC_KILL
    if (obj->flags & O_PROC) PRC_KILL(obj->plink);
    // asm 00006B2F: 	LDI	@FLYCOLLPI,AR2		;GET SIGN FLY PROCESS
    // asm 00006B30: 	LDI	DRONE_C|FLYER_T,R2
    // asm 00006B31: 	CALL	PRC_CREATE_CHILD	;CREATE A PROCESS
    ctx = NEW_PROC_CONTEXT();
    ctx->FLYCOLLP.obj = obj;
    fly_proc = PRC_CREATE_CHILD(CURRENT_PROC, FLYCOLLPI, DRONE_C | FLYER_T, ctx);
    // asm 00006B32: 	BC	FLY_PARTSX		;NOTHING AVAILABLE, QUIT
    if (fly_proc == NULL) goto FLY_PARTSX;
    // asm 00006B33: 	STI	AR0,*+AR4(OPLINK)	;SAVE PROCESS LINK
    obj->plink = fly_proc;
    // asm 00006B34: 	LDI	1,R0
    // asm 00006B35: 	LSH	O_3DROT_B,R0		;FLAG CAN AS NON-2D OPTIMIZABLE
    // asm 00006B36: 	LDI	1,R1
    // asm 00006B37: 	LSH	O_PROC_B,R1		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm 00006B38: 	ADDI	R1,R0
    // asm 00006B39: 	OR	*+AR4(OFLAGS),R0	;SET YOUR FLAGS...
    // asm 00006B3A: 	OR	O_NOCOLL,R0		;Parts can only be hit once
    // asm 00006B3B: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= O_3DROT | O_PROC | O_NOCOLL;
    // asm 00006B3C: 	LDI	0,R0			;KILL OFF GROUP REFERENCE
    // asm 00006B3D: 	STI	R0,*+AR4(OLINK2)
    obj->link2 = 0;
FLY_PARTSX:
    // asm 00006B3E: 	RETS
    return;
}

/* asm: GEESEANI: */
/* asm: 	.word	geese1,geeseb,geesec,geesed */
/* asm: 	.word	geesee,geesef,geeseg,geeseh,-1 */
/* asm: 	 */
static int GEESEANI[] = {
    geese1_ROM,
    geeseb_ROM,
    geesec_ROM,
    geesed_ROM,
    geesee_ROM,
    geesef_ROM,
    geeseg_ROM,
    geeseh_ROM,
    -1,
};
static tGEESE_DIR_ENTRY GEESE_DIR[] = {
    { 250, 1, C3X_F32_INIT(0.0f) },
    { 150, -1, C3X_F32_INIT(0.0f) },
    { 150, -1, C3X_F32_INIT(-0.13f) },
    { 250, 1, C3X_F32_INIT(-0.13f) },
};
// *----------------------------------------------------------------------------
#define SPEED PDATA
#define DIRECTION (PDATA + 1)
#define DIR_RAD (PDATA + 2)
#define NUM_SPLATS (PDATA + 3)

// *----------------------------------------------------------------------------
void GEESE_SPAWNER(PROC* p)
{
    int choice;
    const tGEESE_DIR_ENTRY* entry;
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    case 2: goto PROC_RESUME_2;
    }
    // asm 00006B3F: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00006B41: 	LDI	@PLYCAR,R0
    // asm 00006B42: 	BZ	GEESE_SPAWNER
    if (PLYCAR == NULL) REENTER(GEESE_SPAWNER);
    // asm 00006B43: 	LDI	@_MODE,R0
    // asm 00006B44: 	AND	MGO,R0
    // asm 00006B45: 	BZ	GEESE_SPAWNER
    if ((_MODE & MGO) == 0) REENTER(GEESE_SPAWNER);
GOOSE_ME:
    // asm 00006B46: 	SLEEP	30*20		;sleep approx. 20 seconds
    SLEEP(30 * 20, 2);
    // ;	SLEEP	30*5
    // asm 00006B48: 	LDI	500,AR2
    // asm 00006B49: 	CALL	RANDPER
    // asm 00006B4A: 	BNC	GOOSE_ME
    if (!RANDPER(500)) goto GOOSE_ME;
    // asm 00006B4B: 	LDI	4,AR2
    // asm 00006B4C: 	CALL	RANDU0
    choice = RANDU0(4);
    // asm 00006B4D: 	MPYI	3,R0
    // asm 00006B4E: 	LDI	@GEESE_DIRI,AR0
    // asm 00006B4F: 	ADDI	R0,AR0
    entry = &GEESE_DIR[choice];
    // asm 00006B50: 	LDI	*AR0++,R4
    // asm 00006B51: 	LDI	*AR0++,R5
    // 		;register is pushed as INT
    // asm 00006B52: 	LDF	*AR0++,R6
    // asm 00006B53: 	CREATE	GEESE_PROC,UTIL_C
    CREATE(GEESE_PROC, UTIL_C, &(PROC_CONTEXT){ .GEESE_PROC = { .initial_speed = entry->speed, .initial_direction = entry->direction, .initial_dir_rad = entry->dir_rad } });
    // asm 00006B56: 	BR	GOOSE_ME
    goto GOOSE_ME;
}

/*
 *----------------------------------------------------------------------------
 * Set by Spawner:
 *	R4	=	SPEED
 *	R5	=	DIRECTION INT (+/-1, Direction to travers road)
 *	R6	=	RADS direction FL
 */
/* asm: SHIT_ANI	.word	bdst,bdst2,bdst3,bdst4,bdst5,bdst6,-1 */
static int SHIT_ANI[] = {
    bdst_ROM,
    bdst2_ROM,
    bdst3_ROM,
    bdst4_ROM,
    bdst5_ROM,
    bdst6_ROM,
    -1,
};

static void GEESE_PROC(PROC* p)
{
    OBJ* obj;
    OBJ* track;
    LEG_PAYLOAD* map;
    LEG_PAYLOAD* player_map;
    c3x_reg_t angle, movement, road_height, distance, lateral, y_offset, facing, signed_frames, below_ground;
    int model, moved, ground_found, enabled;
    c3x_reg_t initial_speed;
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    }
    // asm 00006B58: 	FLOAT	R4
    initial_speed = C3X_FROM_INT(p->ctx.GEESE_PROC.initial_speed);
    // asm 00006B59: 	STF	R4,*+AR7(SPEED)
    p->ctx.GEESE_PROC.speed = C3X_STF(initial_speed);
    // asm 00006B5A: 	STI	R5,*+AR7(DIRECTION)
    p->ctx.GEESE_PROC.direction = p->ctx.GEESE_PROC.initial_direction;
    // asm 00006B5B: 	STF	R6,*+AR7(DIR_RAD)
    p->ctx.GEESE_PROC.dir_rad = C3X_STF(C3X_LDF(p->ctx.GEESE_PROC.initial_dir_rad));
    // asm 00006B5C: 	LDI	0,R0
    // asm 00006B5D: 	STI	R0,*+AR7(NUM_SPLATS)
    p->ctx.GEESE_PROC.num_splats = 0;
    // asm 00006B5E: 	CALL	OBJ_GET
    obj = OBJ_GET();
    // asm 00006B5F: 	BC	GEESE_DIE
    if (obj == NULL) goto GEESE_DIE;
    p->ctx.GEESE_PROC.obj = obj;
    // asm 00006B60: 	LDI	AR0,AR4
    // asm 00006B61: 	SONDFX	GEESES
    ONESNDFX(GEESES);
    // asm 00006B63: 	LDI	@PLYCBLK,AR2
    // asm 00006B64: 	LDI	*+AR2(CARTRAK),AR2	;Get the track piece the car is on
    track = OBJREF_TO_PTR(PLYCBLK->closest_track_piece);
    // asm 00006B65: 	LDI	*+AR2(OUSR1),R2
    // asm 00006B66: 	CALL	FIND_MAP
    map = FIND_MAP((int)track->usr1);
    // asm 00006B67: 	ADDI	20*4,AR2		;bump it up 40 road segments
    map += 20;
    // asm 00006B68: 	LDI	AR2,AR6			;save pointer to ROAD LEG
    p->ctx.GEESE_PROC.map_entry = map;
    // asm 00006B69: 	FLOAT	*+AR6(X),R0
    // asm 00006B6A: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_FROM_INT(map->as_fixed.pos_x));
    // asm 00006B6B: 	LDF	*+AR7(DIR_RAD),R2	;flying with the road?
    // asm 00006B6C: 	CMPF	0,R2
    // asm 00006B6D: 	BEQ	NO_XOFFSET
    if (C3X_EQ_IMM(C3X_LDF(p->ctx.GEESE_PROC.dir_rad), 0)) goto NO_XOFFSET;
    // asm 00006B6E: 	LDI	*+AR7(DIRECTION),R2
    // asm 00006B6F: 	LDI	AR6,AR2
    // asm 00006B70: 	CALL	GET_ROAD_RADY
    angle = GET_ROAD_RADY(map, p->ctx.GEESE_PROC.direction);
    // asm 00006B71: 	ADDF	HALFPI,R2
    angle = C3X_ADD_IMM(angle, HALFPI);
    // asm 00006B72: 	LDI	@MATRIXAI,AR2
    // asm 00006B73: 	CALL	FIND_YMATRIX
    _find_Ymatrix(&_MATRIXA, angle);
    // asm 00006B74: 	CALL	CLR_VECTORA		;Move the flock ('o geese off the side of the road)
    CLR_VECTORA();
    // asm 00006B75: 	FLOAT	2500,R0
    // asm 00006B76: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF_INT(2500);
    // asm 00006B77: 	LDI	@MATRIXAI,R2
    // asm 00006B78: 	LDI	AR2,R3
    // asm 00006B79: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &_MATRIXA, &VECTORAI);
    // asm 00006B7A: 	LDF	*+AR4(OPOSX),R0
    // asm 00006B7B: 	ADDF	*+AR2(X),R0
    // asm 00006B7C: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), C3X_LDF(VECTORAI.X)));
    // asm 00006B7D: 	LDF	*+AR4(OPOSZ),R0
    // asm 00006B7E: 	ADDF	*+AR2(Z),R0
    // asm 00006B7F: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Z), C3X_LDF(VECTORAI.Z)));
NO_XOFFSET:
    // asm 00006B80: 	FLOAT	-3000,R1
    y_offset = C3X_FROM_INT(-3000);
    // asm 00006B81: 	FLOAT	*+AR6(Y),R0
    // asm 00006B82: 	ADDF	R1,R0
    // asm 00006B83: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_ADD(C3X_FROM_INT(map->as_fixed.pos_y_with_lane_flag), y_offset));
    // asm 00006B84: 	FLOAT	*+AR6(Z),R0
    // asm 00006B85: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_FROM_INT(map->as_fixed.pos_z));
    // asm 00006B86: 	LDI	AR4,AR2
    // asm 00006B87: 	CALL	OBJ_INSERT
    OBJ_INSERT(obj);
GEESERST:
    // asm 00006B88: 	LDI	@GEESEANII,AR5
    p->ctx.GEESE_PROC.animation_index = 0;
GEESELP:
    // asm 00006B89: 	LDI	*AR5++,R0
    model = GEESEANI[p->ctx.GEESE_PROC.animation_index++];
    // asm 00006B8A: 	BN	GEESERST
    if (model < 0) goto GEESERST;
    // asm 00006B8B: 	STI	R0,*+AR4(OROMDATA)
    obj->romdata = ROM_PTR((word_addr_t)model);
    // asm 00006B8C: 	LDI	*+AR7(DIRECTION),R2
    // asm 00006B8D: 	LDI	AR6,AR2
    // asm 00006B8E: 	CALL	GET_ROAD_RADY
    angle = GET_ROAD_RADY(map, p->ctx.GEESE_PROC.direction);
    // asm 00006B8F: 	ADDF	*+AR7(DIR_RAD),R2
    angle = C3X_ADD(angle, C3X_LDF(p->ctx.GEESE_PROC.dir_rad));
    // asm 00006B90: 	PUSHF	R2
    // asm 00006B91: 	ADDF	PI,R2			;Geese point in wrong direction
    facing = C3X_ADD_IMM(angle, PI);
    // asm 00006B92: 	STF	R2,*+AR4(ORADY)
    obj->rad.Y = C3X_STF(facing);
    // asm 00006B93: 	LDI	AR4,AR2
    // asm 00006B94: 	ADDI	OMATRIX,AR2
    // asm 00006B95: 	CALL	FIND_YMATRIX
    _find_Ymatrix(&obj->omatrix, facing);
    // asm 00006B96: 	POPF	R2
    // asm 00006B97: 	LDI	@MATRIXAI,AR2
    // asm 00006B98: 	CALL	FIND_YMATRIX
    _find_Ymatrix(&_MATRIXA, angle);
    // asm 00006B99: 	CALL	CLR_VECTORA		;Move the flock ('o geese)
    CLR_VECTORA();
    // asm 00006B9A: 	LDI	@PLYCBLK,AR0
    // asm 00006B9B: 	LDF	*+AR0(CARSPEED),R0
    movement = C3X_LDF(PLYCBLK->speed);
    // asm 00006B9C: 	MPYF	0.4,R0
    movement = C3X_MUL_IMM(movement, 0.4);
    // asm 00006B9D: 	LDI	*+AR7(DIRECTION),R1
    // asm 00006B9E: 	MPYI	@NFRAMES,R1
    // asm 00006B9F: 	FLOAT	R1
    signed_frames = C3X_FROM_INT(p->ctx.GEESE_PROC.direction * NFRAMES);
    // asm 00006BA0: 	MPYF	R1,R0
    movement = C3X_MUL(movement, signed_frames);
    // asm 00006BA1: 	ADDF	*+AR7(SPEED),R0
    movement = C3X_ADD(movement, C3X_LDF(p->ctx.GEESE_PROC.speed));
    // asm 00006BA2: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF(movement);
    // asm 00006BA3: 	LDI	@MATRIXAI,R2
    // asm 00006BA4: 	LDI	AR2,R3
    // asm 00006BA5: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &_MATRIXA, &VECTORAI);
    // asm 00006BA6: 	LDF	*+AR4(OPOSX),R0
    // asm 00006BA7: 	ADDF	*+AR2(X),R0
    // asm 00006BA8: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), C3X_LDF(VECTORAI.X)));
    // asm 00006BA9: 	LDF	*+AR4(OPOSZ),R0
    // asm 00006BAA: 	ADDF	*+AR2(Z),R0
    // asm 00006BAB: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Z), C3X_LDF(VECTORAI.Z)));
    // asm 00006BAC: 	PUSH	AR4
    // asm 00006BAD: 	ADDI	OPOSX,AR4
    // asm 00006BAE: 	CALL	CAMSCAN
    ground_found = CAMSCAN(&obj->pos, &road_height);
    // asm 00006BAF: 	POP	AR4
    // asm 00006BB0: 	BNC	GEESE_DONE		;FLU off the edge of the earth
    if (!ground_found) goto GEESE_DONE;
    // asm 00006BB1: 	FLOAT	3000,R1
    below_ground = C3X_FROM_INT(3000);
    // asm 00006BB2: 	SUBF	R1,R0
    road_height = C3X_SUB(road_height, below_ground);
    // asm 00006BB3: 	ADDF	*+AR4(OPOSY),R0
    road_height = C3X_ADD(road_height, C3X_LDF(obj->pos.Y));
    // asm 00006BB4: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(road_height);
OFF_MAP:
    // asm 00006BB5: OFF_MAP
    // asm 00006BB5: 	LDI	AR6,AR2
    // asm 00006BB6: 	LDI	*+AR7(DIRECTION),R2
    // asm 00006BB7: 	CALL	NEXT_ROAD
    map = NEXT_ROAD(map, obj, p->ctx.GEESE_PROC.direction, &moved);
    // asm 00006BB8: 	LDI	AR2,AR6
    p->ctx.GEESE_PROC.map_entry = map;
    // asm 00006BB9: 	CMPI	@LEG_MAPI,AR6
    // asm 00006BBA: 	BLE	GEESE_DONE
    if (map <= LEG_MAP) goto GEESE_DONE;
    // asm 00006BBB: 	LDI	@PLYCBLK,AR2
    // asm 00006BBC: 	LDI	*+AR2(CARTRAK),AR2	;Get the track piece the car is on
    track = OBJREF_TO_PTR(PLYCBLK->closest_track_piece);
    // asm 00006BBD: 	LDI	*+AR2(OUSR1),R2
    // asm 00006BBE: 	CALL	FIND_MAP
    player_map = FIND_MAP((int)track->usr1);
    // asm 00006BBF: 	LDI	AR2,AR0
    // asm 00006BC0: 	ADDI	-10*4,AR2		;track sections behind the player
    // asm 00006BC1: 	CMPI	AR2,AR6
    // asm 00006BC2: 	BLE	GEESE_DONE
    if (map <= player_map - 10) goto GEESE_DONE;
    // asm 00006BC3: 	ADDI	30*4,AR0		;track sections beyond the player
    // asm 00006BC4: 	CMPI	AR0,AR6
    // asm 00006BC5: 	BGE	GEESE_DONE
    if (map >= player_map + 30) goto GEESE_DONE;
GEESE_SLEEP:
    // asm 00006BC6: GEESE_SLEEP
    // asm 00006BC6: 	SLEEP	1
    SLEEP(1, 1);
    obj = p->ctx.GEESE_PROC.obj;
    map = p->ctx.GEESE_PROC.map_entry;
    // asm 00006BC8: 	READADJ	ADJ_ROADKILL
    enabled = READADJ(ADJ_ROADKILL);
    // asm 00006BCA: 	CMPI	0,R0
    // asm 00006BCB: 	BEQ	NO_SPLAT
    if (enabled == 0) goto NO_SPLAT;
    // asm 00006BCC: 	LDI	@CAMVIEW,R0
    // asm 00006BCD: 	BNZ	NO_SPLAT		;NOT in First person
    if (CAMVIEW != 0) goto NO_SPLAT;
    // asm 00006BCE: 	LDI	*+AR4(ODIST),R0
    // asm 00006BCF: 	BN	NO_SPLAT
    if (obj->dist < 0) goto NO_SPLAT;
    // asm 00006BD0: 	CALL	DIST_TO_PLYR
    distance = DIST_TO_PLYR(obj);
    // asm 00006BD1: 	FLOAT	5000,R1
    // asm 00006BD2: 	CMPF	R1,R0
    // asm 00006BD3: 	BGT	NO_SPLAT
    if (C3X_GT_IMM(distance, 5000)) goto NO_SPLAT;
    // asm 00006BD4: 	LDI	AR6,AR2
    // asm 00006BD5: 	LDI	1,R2
    // asm 00006BD6: 	CALL	GET_ROAD_RADY
    angle = GET_ROAD_RADY(map, 1);
    // asm 00006BD7: 	NEGF	R2
    angle = C3X_NEG(angle);
    // asm 00006BD8: 	LDI	@MATRIXAI,AR2
    // asm 00006BD9: 	CALL	FIND_YMATRIX
    _find_Ymatrix(&_MATRIXA, angle);
    // asm 00006BDA: 	CALL	CLR_VECTORA
    CLR_VECTORA();
    // asm 00006BDB: 	FLOAT	*+AR6(X),R1
    // asm 00006BDC: 	LDF	*+AR4(OPOSX),R0
    // asm 00006BDD: 	SUBF	R1,R0
    // asm 00006BDE: 	STF	R0,*+AR2(X)
    VECTORAI.X = C3X_STF(C3X_SUB(C3X_LDF(obj->pos.X), C3X_FROM_INT(map->as_fixed.pos_x)));
    // asm 00006BDF: 	FLOAT	*+AR6(Z),R1
    // asm 00006BE0: 	LDF	*+AR4(OPOSZ),R0
    // asm 00006BE1: 	SUBF	R1,R0
    // asm 00006BE2: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF(C3X_SUB(C3X_LDF(obj->pos.Z), C3X_FROM_INT(map->as_fixed.pos_z)));
    // asm 00006BE3: 	LDI	@MATRIXAI,R2
    // asm 00006BE4: 	LDI	@VECTORAI,R3
    // asm 00006BE5: 	LDI	R3,AR3
    // asm 00006BE6: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &_MATRIXA, &VECTORAI);
    // asm 00006BE7: 	LDI	@VECTORAI,AR0
    // asm 00006BE8: 	LDF	*+AR0(X),R0
    // asm 00006BE9: 	LDI	@PLYCBLK,AR0
    // asm 00006BEA: 	SUBF	*+AR0(CARDIST2CNTR),R0
    // asm 00006BEB: 	ABSF	R0
    lateral = C3X_ABS(C3X_SUB(C3X_LDF(VECTORAI.X), C3X_LDF(PLYCBLK->dist_to_center)));
    // asm 00006BEC: 	FLOAT	2000,R1
    // asm 00006BED: 	CMPF	R1,R0
    // asm 00006BEE: 	BGT	NO_SPLAT
    if (C3X_GT_IMM(lateral, 2000)) goto NO_SPLAT;
    // asm 00006BEF: 	LDI	800,AR2
    // asm 00006BF0: 	CALL	RANDPER
    // asm 00006BF1: 	BNC	NO_SPLAT
    if (!RANDPER(800)) goto NO_SPLAT;
    // asm 00006BF2: 	LDI	*+AR7(NUM_SPLATS),R0
    // asm 00006BF3: 	CMPI	6,R0
    // asm 00006BF4: 	BGT	NO_SPLAT
    if (p->ctx.GEESE_PROC.num_splats > 6) goto NO_SPLAT;
    // asm 00006BF5: 	ADDI	1,R0
    // asm 00006BF6: 	STI	R0,*+AR7(NUM_SPLATS)
    ++p->ctx.GEESE_PROC.num_splats;
    // asm 00006BF7: 	LDI	@SHIT_ANII,R5
    // asm 00006BF8: 	CREATE	SPLAT_PROC,UTIL_C	;The SPLAT will hit the windshield
    CREATE(SPLAT_PROC, UTIL_C, &(PROC_CONTEXT){ .SPLAT_PROC = { .animation = SHIT_ANI } });
NO_SPLAT:
    // asm 00006BFB: 	BU	GEESELP
    goto GEESELP;
GEESE_DONE:
    // asm 00006BFC: 	LDI	AR4,AR2
    // asm 00006BFD: 	CALL	OBJ_DELETE
    OBJ_DELETE(obj);
    // asm 00006BFE: 	LDI	GEESES,AR2
    // asm 00006BFF: 	CALL	KILLSNDFX
    KILLSNDFX(GEESES);
GEESE_DIE:
    // asm 00006C00: 	DIE
    DIE();
}

/*
 *---------------------------------------------------------------------------
 *---------------------------------------------------------------------------
 *---------------------------------------------------------------------------
 *----------------------------------------------------------------------------
 * BUG_SPAWNER_PROC	PROC
 *Creates several BUG SPLAT PROCS
 *	CREATE	BUG_SPAWNER_PROC,SPAWNER_C
 *
 */
/* asm: BUG_ANI	.word	bug1,bug2,bug3,bug4,bug5,-1 */
static int BUG_ANI[] = {
    bug1_ROM,
    bug2_ROM,
    bug3_ROM,
    bug4_ROM,
    bug5_ROM,
    -1,
};

void BUG_SPAWNER_PROC(PROC* p)
{
    int enabled;
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    case 2: goto PROC_RESUME_2;
    case 3: goto PROC_RESUME_3;
    case 4: goto PROC_RESUME_4;
    }
    // asm 00006C02: 	READADJ	ADJ_ROADKILL
    enabled = READADJ(ADJ_ROADKILL);
    // asm 00006C04: 	CMPI	0,R0
    // asm 00006C05: 	BEQ	BSPX
    if (enabled == 0) goto BSPX;
    // asm 00006C06: 	SLEEP	1			;wait for game to start
    SLEEP(1, 1);
    // asm 00006C08: 	LDI	@_MODE,R0
    // asm 00006C09: 	AND	MGO,R0
    // asm 00006C0A: 	BZ	BUG_SPAWNER_PROC
    if ((_MODE & MGO) == 0) REENTER(BUG_SPAWNER_PROC);
BSPLP:
    // asm 00006C0B: 	SLEEP	30*5
    SLEEP(30 * 5, 2);
    // asm 00006C0D: 	LDI	@CAMVIEW,R0
    // asm 00006C0E: 	BNZ	BSPLP			;NOT in First person
    if (CAMVIEW != 0) goto BSPLP;
    // asm 00006C0F: 	LDI	300,AR2
    // asm 00006C10: 	CALL	RANDPER
    // asm 00006C11: 	BNC	BSPLP
    if (!RANDPER(300)) goto BSPLP;
    // asm 00006C12: 	SONDFX	BUGBUZZ
    ONESNDFX(BUGBUZZ);
    // asm 00006C14: 	SONDFX	BUGBUZZ
    ONESNDFX(BUGBUZZ);
    // asm 00006C16: 	SLEEP	10
    SLEEP(10, 3);
    // asm 00006C18: 	LDI	@BUG_ANII,R5
    // asm 00006C19: 	CREATE	SPLAT_PROC,UTIL_C	;The SPLAT will hit the windshield
    CREATE(SPLAT_PROC, UTIL_C, &(PROC_CONTEXT){ .SPLAT_PROC = { .animation = BUG_ANI } });
    // asm 00006C1C: 	SLEEP	30*20			;20 seconds before another bug can apear
    SLEEP(30 * 20, 4);
    // asm 00006C1E: 	BR	BSPLP
    goto BSPLP;
BSPX:
    // asm 00006C1F: 	DIE
    DIE();
}

/*
 *---------------------------------------------------------------------------
 *SPLAT PROC
 *R5 points to start of animation
 */

static void SPLAT_PROC(PROC* p)
{
    OBJ* obj;
    c3x_reg_t velocity, movement, vertical, angle, frame_count, x_position, y_position, random_x, fall_rate;
    int model;
    switch (PROC_RESUME_STATE) {
    case 0: MAME_ASSERT_FUNCTION_ENTRY(); break;
    case 1: goto PROC_RESUME_1;
    case 2: goto PROC_RESUME_2;
    case 3: goto PROC_RESUME_3;
    }
    // asm 00006C20: 	LDI	@PLYCBLK,AR5
    // asm 00006C21: 	LDI	R5,AR6
    p->ctx.SPLAT_PROC.animation_index = 0;
    // asm 00006C22: 	LDI	*AR6++,AR2
    model = p->ctx.SPLAT_PROC.animation[p->ctx.SPLAT_PROC.animation_index++];
    // asm 00006C23: 	CALL	OBJ_GETE
    obj = OBJ_GETE(ROM_PTR((word_addr_t)model));
    // asm 00006C24: 	LDI	AR0,AR4
    p->ctx.SPLAT_PROC.obj = obj;
    // asm 00006C25: 	LDI	PLYR_C|PLYR_SPLAT_S,R0
    // asm 00006C26: 	STI	R0,*+AR4(OID)
    obj->id = PLYR_C | PLYR_SPLAT_S;
    // asm 00006C27: 	LDF	100,R0
    random_x = C3X_IMM_F32(100);
    // asm 00006C28: 	CALL	SFRAND
    random_x = SFRAND(random_x);
    // asm 00006C29: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(random_x);
    // asm 00006C2A: 	FLOAT	-300,R0
    // asm 00006C2B: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF_INT(-300);
    // asm 00006C2C: 	FLOAT	2000,R0
    // asm 00006C2D: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF_INT(2000);
    // asm 00006C2E: 	LDI	O_POSTER|O_NOCOLL|O_NOROTS|O_NOUNIV,R0
    // asm 00006C2F: 	OR	*+AR4(OFLAGS),R0
    // asm 00006C30: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= O_POSTER | O_NOCOLL | O_NOROTS | O_NOUNIV;
    // asm 00006C31: 	LDI	AR4,AR2
    // asm 00006C32: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);
    // asm 00006C33: 	LDF	1,R0
    fall_rate = C3X_IMM_F32(1);
    // asm 00006C34: 	CALL	FRAND
    fall_rate = FRAND(fall_rate);
    // asm 00006C35: 	ADDF	1.2,R0
    fall_rate = C3X_ADD_IMM(fall_rate, 1.2);
    // asm 00006C36: 	LDF	R0,R6
    // asm 00006C37: 	MPYF	0.1,R6
    p->ctx.SPLAT_PROC.fall_rate = C3X_MUL_IMM(fall_rate, 0.1);
SPLAT_LP:
    // asm 00006C38: 	SLEEP	1
    SLEEP(1, 1);
    obj = p->ctx.SPLAT_PROC.obj;
    // asm 00006C3A: 	LDI	@CAMVIEW,R0
    // asm 00006C3B: 	BNZ	SPLAT_DONE		;NOT in First person
    if (CAMVIEW != 0) goto SPLAT_DONE;
    // asm 00006C3C: 	FLOAT	@NFRAMES,R0		;Calculate the rate that the camera is moving
    velocity = C3X_FROM_INT(NFRAMES);
    // asm 00006C3D: 	MPYF	*+AR5(CARSPEED),R0
    velocity = C3X_MUL(velocity, C3X_LDF(PLYCBLK->speed));
    // asm 00006C3E: 	MPYF	-0.8,R0			;NORMAL SPEED WOULD BE -1.5
    velocity = C3X_MUL_IMM(velocity, -0.8);
    // asm 00006C3F: 	CMPF	-120,R0			;Minimum velocity
    // asm 00006C40: 	LDFGT	-120,R0
    if (C3X_GT_IMM(velocity, -120)) velocity = C3X_IMM_F32(-120);
    // asm 00006C41: 	NEGF	R0,R1
    vertical = C3X_NEG(velocity);
    // asm 00006C42: 	MPYF	R6,R1			;Move down 1/10th the rate as coming at you
    vertical = C3X_MUL(vertical, p->ctx.SPLAT_PROC.fall_rate);
    // asm 00006C43: 	ADDF	*+AR4(OPOSY),R1
    vertical = C3X_ADD(vertical, C3X_LDF(obj->pos.Y));
    // asm 00006C44: 	STF	R1,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(vertical);
    // asm 00006C45: 	ADDF	*+AR4(OPOSZ),R0
    movement = C3X_ADD(velocity, C3X_LDF(obj->pos.Z));
    // asm 00006C46: 	FLOAT	368,R1
    // asm 00006C47: 	CMPF	R1,R0
    // asm 00006C48: 	LDFLT	R1,R0
    if (C3X_LT_IMM(movement, 368)) movement = C3X_IMM_F32(368);
    // asm 00006C49: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(movement);
    // asm 00006C4A: 	BGT	SPLAT_LP
    if (C3X_GT_IMM(movement, 368)) goto SPLAT_LP;
    // asm 00006C4B: 	LDI	BUGBUZZ,AR2
    // asm 00006C4C: 	CALL	KILLSNDFX
    KILLSNDFX(BUGBUZZ);
    // asm 00006C4D: 	SONDFX	RK_SPLAT1
    ONESNDFX(RK_SPLAT1);
    // asm 00006C4F: 	LDI	*AR6++,R4
    model = p->ctx.SPLAT_PROC.animation[p->ctx.SPLAT_PROC.animation_index++];
SPLAT_LP1:
    // asm 00006C50: 	STI	R4,*+AR4(OROMDATA)
    obj->romdata = ROM_PTR((word_addr_t)model);
    // asm 00006C51: 	SLEEP	2
    SLEEP(2, 2);
    obj = p->ctx.SPLAT_PROC.obj;
    // asm 00006C53: 	LDI	@CAMVIEW,R0
    // asm 00006C54: 	BNZ	SPLAT_DONE		;NOT in First person
    if (CAMVIEW != 0) goto SPLAT_DONE;
    // asm 00006C55: 	LDI	*AR6++,R4
    model = p->ctx.SPLAT_PROC.animation[p->ctx.SPLAT_PROC.animation_index++];
    // asm 00006C56: 	BP	SPLAT_LP1
    if (model > 0) goto SPLAT_LP1;
    // asm 00006C57: 	LDF	*+AR4(OPOSX),R2
    // asm 00006C58: 	LDF	*+AR4(OPOSY),R3
    // asm 00006C59: 	CALL	ARCTANF
    angle = ARCTANF(C3X_LDF(obj->pos.X), C3X_LDF(obj->pos.Y));
    // asm 00006C5A: 	LDF	R0,R2
    // asm 00006C5B: 	CALL	_COSI
    velocity = _COSI(angle);
    // asm 00006C5C: 	STF	R0,*+AR4(OVELX)
    obj->vel_x = C3X_STF(velocity);
    // asm 00006C5D: 	CALL	_SINE
    velocity = _SINE(angle);
    // asm 00006C5E: 	STF	R0,*+AR4(OVELY)
    obj->vel_y = C3X_STF(velocity);
SPLAT_LP2:
    // asm 00006C5F: 	SLEEP	1
    SLEEP(1, 3);
    obj = p->ctx.SPLAT_PROC.obj;
    // asm 00006C61: 	LDI	@CAMVIEW,R0
    // asm 00006C62: 	BNZ	SPLAT_DONE		;NOT in First person
    if (CAMVIEW != 0) goto SPLAT_DONE;
    // asm 00006C63: 	FLOAT	@NFRAMES,R2
    frame_count = C3X_FROM_INT(NFRAMES);
    // asm 00006C64: 	LDF	*+AR4(OVELX),R0
    velocity = C3X_LDF(obj->vel_x);
    // asm 00006C65: 	MPYF	0.2,R0
    velocity = C3X_MUL_IMM(velocity, 0.2);
    // asm 00006C66: 	ADDF	*+AR4(OVELX),R0
    velocity = C3X_ADD(velocity, C3X_LDF(obj->vel_x));
    // asm 00006C67: 	STF	R0,*+AR4(OVELX)
    obj->vel_x = C3X_STF(velocity);
    // asm 00006C68: 	MPYF	R2,R0
    x_position = C3X_MUL(velocity, frame_count);
    // asm 00006C69: 	ADDF	*+AR4(OPOSX),R0
    x_position = C3X_ADD(x_position, C3X_LDF(obj->pos.X));
    // asm 00006C6A: 	STF	R0,*+AR4(OPOSX)
    obj->pos.X = C3X_STF(x_position);
    // asm 00006C6B: 	LDF	*+AR4(OVELY),R1
    velocity = C3X_LDF(obj->vel_y);
    // asm 00006C6C: 	MPYF	0.1,R1
    velocity = C3X_MUL_IMM(velocity, 0.1);
    // asm 00006C6D: 	ADDF	*+AR4(OVELY),R1
    velocity = C3X_ADD(velocity, C3X_LDF(obj->vel_y));
    // asm 00006C6E: 	STF	R1,*+AR4(OVELY)
    obj->vel_y = C3X_STF(velocity);
    // asm 00006C6F: 	MPYF	R2,R1
    y_position = C3X_MUL(velocity, frame_count);
    // asm 00006C70: 	ADDF	*+AR4(OPOSY),R1
    y_position = C3X_ADD(y_position, C3X_LDF(obj->pos.Y));
    // asm 00006C71: 	STF	R1,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(y_position);
    // asm 00006C72: 	FLOAT	300,R2
    // asm 00006C73: 	ABSF	R0
    x_position = C3X_ABS(x_position);
    // asm 00006C74: 	CMPF	R2,R0
    // asm 00006C75: 	BGT	SPLAT_DONE
    if (C3X_GT_IMM(x_position, 300)) goto SPLAT_DONE;
    // asm 00006C76: 	CMPF	R2,R1
    // asm 00006C77: 	BLE	SPLAT_LP2
    if (C3X_LE_IMM(y_position, 300)) goto SPLAT_LP2;
SPLAT_DONE:
    // asm 00006C78: 	LDI	AR4,AR2
    // asm 00006C79: 	CALL	OBJ_DELETE
    OBJ_DELETE(obj);
SPLAT_DIE:
    // asm 00006C7A: SPLAT_DIE
    // asm 00006C7A: 	LDI	BUGBUZZ,AR2		;Make sure that it dies (it loops)
    // asm 00006C7B: 	CALL	KILLSNDFX
    KILLSNDFX(BUGBUZZ);
    // asm 00006C7C: 	DIE
    DIE();
}

void DELETE_SPLAT(void)
{
    OBJ* splat_obj;

DBSLP:
    // asm 00006C7D: 	LDI	PLYR_C|PLYR_SPLAT_S,AR2
    // asm 00006C7E: 	CALL	OBJ_FIND_FIRST_PRIORITY
    splat_obj = OBJ_FIND_FIRST_PRIORITY(PLYR_C | PLYR_SPLAT_S);
    // asm 00006C7F: 	BNC	DBSX
    if (splat_obj == NULL) {
        goto DBSX;
    }
    // asm 00006C80: 	LDI	AR0,AR2
    // asm 00006C81: 	CALL	OBJ_DELETE
    OBJ_DELETE(splat_obj);
    // asm 00006C82: 	BR	DBSLP
    goto DBSLP;
DBSX:
    // asm 00006C83: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELETE_SPLAT", 0, 0);
}

/*
 *---------------------------------------------------------------------------
 * Updates AR2 to point to the road segment that the camera is on
 * NOTE: uses Stealth mode
 * INPUT	AR2 = segment on
 *	AR4 = object moving
 *	R2  = Direction moving	INT (+/-1)
 * OUTPUT AR2 = segment on (updated)
 *	R2 = # of segments moved
 */
static LEG_PAYLOAD* NEXT_ROAD(LEG_PAYLOAD* segment /*AR2*/, OBJ* obj /*AR4*/, int direction /*R2*/, int* moved)
{
    int step, count;
    LEG_PAYLOAD* candidate;
    c3x_reg_t dx, dz, current_distance, next_distance;
    // asm 00006C84: 	LDI	R2,R4
    // asm 00006C85: 	MPYI	4,R4
    step = direction;
    // asm 00006C86: 	LDI	0,R3
    count = 0;
NEXT_ROADLP:
    // asm 00006C87: 	FLOAT	*+AR2(X),R0
    // asm 00006C88: 	SUBF	*+AR4(OPOSX),R0
    dx = C3X_SUB(C3X_FROM_INT(segment->as_fixed.pos_x), C3X_LDF(obj->pos.X));
    // asm 00006C89: 	MPYF	R0,R0
    dx = C3X_MUL(dx, dx);
    // asm 00006C8A: 	FLOAT	*+AR2(Z),R1
    // asm 00006C8B: 	SUBF	*+AR4(OPOSZ),R1
    dz = C3X_SUB(C3X_FROM_INT(segment->as_fixed.pos_z), C3X_LDF(obj->pos.Z));
    // asm 00006C8C: 	MPYF	R1,R1
    dz = C3X_MUL(dz, dz);
    // asm 00006C8D: 	ADDF	R1,R0
    current_distance = C3X_ADD(dx, dz);
    // asm 00006C8E: 	LDI	AR2,AR0
    // asm 00006C8F: 	ADDI	R4,AR0
    candidate = segment + step;
    // asm 00006C90: 	FLOAT	*+AR0(X),R1
    // asm 00006C91: 	SUBF	*+AR4(OPOSX),R1
    dx = C3X_SUB(C3X_FROM_INT(candidate->as_fixed.pos_x), C3X_LDF(obj->pos.X));
    // asm 00006C92: 	MPYF	R1,R1
    dx = C3X_MUL(dx, dx);
    // asm 00006C93: 	FLOAT	*+AR0(Z),R2
    // asm 00006C94: 	SUBF	*+AR4(OPOSZ),R2
    dz = C3X_SUB(C3X_FROM_INT(candidate->as_fixed.pos_z), C3X_LDF(obj->pos.Z));
    // asm 00006C95: 	MPYF	R2,R2
    dz = C3X_MUL(dz, dz);
    // asm 00006C96: 	ADDF	R2,R1
    next_distance = C3X_ADD(dx, dz);
    // asm 00006C97: 	CMPF	R1,R0
    // asm 00006C98: 	BLE	NEXTRX			;didn't move
    if (C3X_LE(current_distance, next_distance)) goto NEXTRX;
    // asm 00006C99: 	ADDI	R4,AR2
    segment = candidate;
    // asm 00006C9A: 	CMPI	@LEG_MAPI,AR2
    // asm 00006C9B: 	LDILT	@LEG_MAPI,AR2
    if (segment < LEG_MAP) segment = LEG_MAP;
    // asm 00006C9C: 	BLT	NEXTRX			;don't fly off the back of the map
    if (candidate < LEG_MAP) goto NEXTRX;
    // asm 00006C9D: 	ADDI	1,R3			;R3 is the number of road segments moved
    ++count;
    // asm 00006C9E: 	BR	NEXT_ROADLP		;See if the road segment beyond is even closer
    goto NEXT_ROADLP;
NEXTRX:
    // asm 00006C9F: 	LDI	R3,R2
    // asm 00006CA0: 	RETS
    if (moved != NULL) *moved = count;
    return segment;
}

/*
 *---------------------------------------------------------------------------
 *INPUT	AR2 = POINTER to LEG_MAP where road is
 *	R2  = Direction moving	INT (+/-1)
 *
 *OUTPUT	R2 = DIRECTION of road
 */
static c3x_reg_t GET_ROAD_RADY(LEG_PAYLOAD* segment /*AR2*/, int direction /*R2*/)
{
    LEG_PAYLOAD* other;
    c3x_reg_t dx, dz, angle;
    // asm 00006CA1: 	MPYI	4*2,R2
    // asm 00006CA2: 	LDI	AR2,AR1
    // asm 00006CA3: 	ADDI	R2,AR1
    other = segment + direction * 2;
    // asm 00006CA4: 	LDI	*+AR1(Z),R3
    // asm 00006CA5: 	SUBI	*+AR2(Z),R3
    // asm 00006CA6: 	FLOAT	R3
    dz = C3X_FROM_INT(other->as_fixed.pos_z - segment->as_fixed.pos_z);
    // asm 00006CA7: 	LDI	*+AR1(X),R2
    // asm 00006CA8: 	SUBI	*+AR2(X),R2
    // asm 00006CA9: 	FLOAT	R2
    dx = C3X_FROM_INT(other->as_fixed.pos_x - segment->as_fixed.pos_x);
    // asm 00006CAA: 	CALL	ARCTANF
    angle = ARCTANF(dx, dz);
    // asm 00006CAB: 	LDF	R0,R2
    // asm 00006CAC: 	CALL	NORMITS
    angle = NORMITS(angle);
    // asm 00006CAD: 	SUBF	HALFPI,R2
    angle = C3X_SUB_IMM(angle, HALFPI);
    // asm 00006CAE: 	CALL	NORMITS
    angle = NORMITS(angle);
    // asm 00006CAF: 	RETS
    return angle;
}

/*
 *---------------------------------------------------------------------------
 *
 *GET POINTER TO MAP_LEG, ID IN R2
 *INPUTS  R2=OUSR1
 *OUPTUTS AR2=POINTER TO place in LEG_MAP
 */
static LEG_PAYLOAD* FIND_MAP(int id /*R2*/)
{
    LEG_PAYLOAD* entry;
    int entry_id;
    // asm 00006CB0: 	LDI	@LEG_MAPI,AR2		;FIND THE POSITION IN THE MAP
    entry = LEG_MAP;
    // asm 00006CB1: 	ADDI	3,AR2				;OFFSET TO ID
FIND_LP:
    // asm 00006CB2: 	LDI	*AR2++(4),R0
    entry_id = (int)entry->as_fixed.id;
    ++entry;
    // asm 00006CB3: 	CMPI	R2,R0
    // asm 00006CB4: 	BLT	FIND_LP
    if (entry_id < id) goto FIND_LP;
FINDX:
    // asm 00006CB5: FINDX
    // asm 00006CB5: 	SUBI	7,AR2
    --entry;
    // asm 00006CB6: 	RETS
    return entry;
}

// *---------------------------------------------------------------------------

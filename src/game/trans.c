
#include "trans.h"
#include "../core/machine.h"
#include "cmos.h"
#include "delta.h"
#include "discovered_labels.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "objects.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

extern MATRIX _MATRIXA;
extern MATRIX _MATRIXB;
extern VECTOR _VECTORA;
extern VECTOR _VECTORC;

/*
 * Source module: asm/TRANS.ASM
 */

void CHOOSE_TRANSMISSION(PROC* p);
static void OPEN_DOOR_PROC(PROC* p);
static void WHEEL_FROM_BELOW(void);
static void CENTERTHEONE(PROC* p);
void DROPTHETURN(PROC* p);
void DROPTHECYCLE(PROC* p);
void DROPTHEWHEEL(PROC* p);
void DROPTHEOTHER(PROC* p);
static void DROP_TRANS_ELEMENT(PROC* p);
void SIDE_DOOR(void);
void DOOR_OPENING(void);
static void SNAPCURSOR(void);
void WHEEL_ROUT(void);
void DOOR_ELEMENT_DELETE_ALL(void);
void DOOR_ELEMENT_DELETE(OBJ* element /*AR2*/);
void TRANSCHOICE(void);
c3x_reg_t GET_UNIT_WHEEL(void);
static void TILE_PIECES(PROC* p);
void FIX_TRANSMISSION_SCREEN(void);
static void MOVE_PUSH_BOX(PROC* p);
void TURNTO_SELECT(PROC* p);
void CYCLE_PUSH(PROC* p);
void ADD_TO_DOOR_LIST(OBJ* obj /*AR0*/);
void ENGINE_COLOR_INIT(void);
void ENGINE_COLOR(c3x_reg_t left_brightness /*R0*/);

#define DOORLISTI DOORLIST
#define TRNTABI TRNTAB
#define CYCTABI CYCTAB

static int trans_palette_word_count(int flags_and_count) {
    return (flags_and_count & 0x0fff) >> 1;
}

static u32 interpolate_trans_palette_word(u32 true_word, u32 chrome_word, c3x_reg_t amount) {
    u32 result = 0;

    for (int half = 0; half < 2; half++) {
        int half_shift = half * 16;
        u32 true_color = (true_word >> half_shift) & 0xffffu;
        u32 chrome_color = (chrome_word >> half_shift) & 0xffffu;
        u32 result_color = 0;

        for (int component = 0; component < 3; component++) {
            int shift = component * 5;
            int true_component = (true_color >> shift) & 0x1f;
            int chrome_component = (chrome_color >> shift) & 0x1f;
            c3x_reg_t delta = C3X_SUB(C3X_FROM_INT(chrome_component), C3X_FROM_INT(true_component));
            int interpolated = FIX(C3X_ADD(C3X_FROM_INT(true_component), C3X_MUL(amount, delta)));
            result_color |= (u32)interpolated << shift;
        }
        result |= result_color << half_shift;
    }
    return result;
}

/*
 *----------------------------------------------------------------------------
 *CHOOSE TRANSMISSION AND RELATED ROUTINES
 */

/* asm: EPALRR	.bss	EPALRR,129 */
tPAL EPALRR;
/* asm: EPALRL	.bss	EPALRL,129 */
tPAL EPALRL;

/* asm: EPALR	.word	EPALRR */
tPAL* EPALR = &EPALRR;
/* asm: EPALL	.word	EPALRL */
/* asm: 	 */
tPAL* EPALL = &EPALRL;

/*
 *----------------------------------------------------------------------------
 *CHOOSE TRANSMISSION
 *
 *
 */
void CHOOSE_TRANSMISSION(PROC* p) {
    PROC_CONTEXT* child_ctx;
    c3x_reg_t engine_brightness;

    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        // MAME_ASSERT_ORDERING("CHOOSE_TRANSMISSION");
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    case 4:
        goto PROC_RESUME_4;
    case 5:
        goto PROC_RESUME_5;
    }

    // asm 00005A17: 	CALL	CLEANUP_PALS
    CLEANUP_PALS();
    // asm 00005A18: 	LDI	1,R0
    // asm 00005A19: 	STI	R0,@NOAERASE
    // asm 00005A1A: 	STI	R0,@NOSWAP
    NOAERASE = 1;
    NOSWAP = 1;
    // asm 00005A1B: 	LDI	MCT,R0
    // asm 00005A1C: 	STI	R0,@_MODE
    _MODE = MCT;
    // asm 00005A1D: 	CALL	ENGINE_COLOR_INIT
    ENGINE_COLOR_INIT();
    // asm 00005A1E: 	LDL	chostrns_grp,AR2
    // asm 00005A1F: 	CALL	LOAD_SINGLE_SECTION
    LOAD_SINGLE_SECTION((LOAD_SINGLE_SECTION_GROUP*)ROM_PTR(chostrns_grp_ROM));
    // asm 00005A20: 	CALL	FIX_TRANSMISSION_SCREEN
    FIX_TRANSMISSION_SCREEN();
    // asm 00005A21: 	LDI	@CT_LENG,AR0
    // asm 00005A22: 	LDI	@EPALL,AR2
    // asm 00005A23: 	CALL	PAL_FIND_RAW
    // asm 00005A24: 	STI	R0,*+AR0(OPAL)
    CT_LENG->palette = (u32)PAL_FIND_RAW(EPALL);
    // asm 00005A25: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005A26: 	OR	O_1PAL,R0
    // asm 00005A27: 	STI	R0,*+AR0(OFLAGS)
    CT_LENG->flags |= O_1PAL;
    // asm 00005A28: 	LDI	@CT_RENG,AR0
    // asm 00005A29: 	LDI	@EPALR,AR2
    // asm 00005A2A: 	CALL	PAL_FIND_RAW
    // asm 00005A2B: 	STI	R0,*+AR0(OPAL)
    CT_RENG->palette = (u32)PAL_FIND_RAW(EPALR);
    // asm 00005A2C: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005A2D: 	OR	O_1PAL,R0
    // asm 00005A2E: 	STI	R0,*+AR0(OFLAGS)
    CT_RENG->flags |= O_1PAL;
    // asm 00005A2F: 	LDF	1,R0
    // asm 00005A30: 	CALL	ENGINE_COLOR
    ENGINE_COLOR(C3X_IMM_F32(1));
    // asm 00005A31: 	CLRI	R0
    // asm 00005A32: 	STI	R0,@NOAERASE
    // asm 00005A33: 	STI	R0,@NOSWAP
    NOAERASE = 0;
    NOSWAP = 0;
    // asm 00005A34: 	CREATE	TILE_PIECES,UTIL_C|CHOOSETRANS_T
    child_ctx = NEW_PROC_CONTEXT();
    CREATE(TILE_PIECES, UTIL_C | CHOOSETRANS_T, child_ctx);
    // asm 00005A37: 	LDI	39,AR5
    p->ctx.CHOOSE_TRANSMISSION_FRAME.loop_count = 39;
LGF7:
    // asm 00005A38: CALL	SIDE_DOOR
    SIDE_DOOR();
    // asm 00005A39: 	CALL	WHEEL_FROM_BELOW
    WHEEL_FROM_BELOW();
    // asm 00005A3A: 	CALL	TRANSCHOICE
    TRANSCHOICE();
    // asm 00005A3B: 	PUSH	AR5
    // asm 00005A3C: 	LDF	1,R0
    // asm 00005A3D: 	CALL	ENGINE_COLOR
    ENGINE_COLOR(C3X_IMM_F32(1));
    // asm 00005A3E: 	POP	AR5
    // asm 00005A3F: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00005A41: 	DBU	AR5,LGF7
    if (p->ctx.CHOOSE_TRANSMISSION_FRAME.loop_count-- > 0)
        goto LGF7;
    // asm 00005A42: 	SONDFX	MHIT
    ONESNDFX(MHIT);
    // asm 00005A44: 	LDI	MCT|MGO|MINFIN|MWATER,R0
    // asm 00005A45: 	STI	R0,@_MODE
    _MODE = MCT | MGO | MINFIN | MWATER;
    // asm 00005A46: 	CLRI	R0
    // asm 00005A47: 	STI	R0,@NOAERASE
    // asm 00005A48: 	STI	R0,@NOSWAP
    NOAERASE = 0;
    NOSWAP = 0;
    // asm 00005A49: 	FLOAT	@_pot0,R0
    // asm 00005A4A: 	STF	R0,@GUWP
    GUWP = C3X_STF_INT(_pot0);
    // *
    // *CHOOSE TRANSMISSION LOOP
    // *
    // asm 00005A4B: 	CLRI	R0
    // asm 00005A4C: 	STI	R0,@START_HIT
    START_HIT = 0;
    // asm 00005A4D: 	LDI	10,R0
    // asm 00005A4E: 	STI	R0,@_countdown
    _countdown = 10;
    // asm 00005A4F: 	CALL	INIT_PEDALCHK
    INIT_PEDALCHK(&p->ctx.pedal_released);
CTLP:
    // asm 00005A50: 	LDI	@START_HIT,R0
    // asm 00005A51: 	BNZ	CTLPX
    if (START_HIT != 0)
        goto CTLPX;
    // asm 00005A52: 	LDI	@CT_CURSOR,AR2
    // asm 00005A53: 	LDF	*+AR2(OPOSX),R0
    // asm 00005A54: 	ADDF	101,R0
    // asm 00005A55: 	FLOAT	204,R1
    // asm 00005A56: 	CALL	DIV_F30
    engine_brightness = DIV_F30(
        C3X_ADD_IMM(C3X_LDF(CT_CURSOR->pos.X), 101),
        C3X_FROM_INT(204));
    // asm 00005A57: 	CALL	ENGINE_COLOR
    ENGINE_COLOR(engine_brightness);
    // asm 00005A58: 	CALL	TRANSCHOICE
    TRANSCHOICE();
    // asm 00005A59: 	CALL	WHEEL_ROUT
    WHEEL_ROUT();
    // 	;-------time remaining
    // 	;
    // asm 00005A5A: 	LDL	time,AR2
    // asm 00005A5B: 	LDI	242,R2			;R2	POS X
    // asm 00005A5C: 	LDI	337,R3			;R3	POS Y
    // asm 00005A5D: 	LDI	TM|ZS,R4
    // asm 00005A5E: 	CALL	BLTMOD2D
    BLTMOD2D((const BLTMOD2D_MODEL*)ROM_PTR(time_ROM), 242, 337, TM | ZS, 0, 0);
    // asm 00005A5F: 	CALL	PEDALCHK
    if (PEDALCHK(&p->ctx.pedal_released))
        goto CTLPX;
    // asm 00005A60: 	BC	CTLPX
    // asm 00005A61: 	CALL	INTROTIMER
    INTROTIMER();
    // asm 00005A62: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00005A64: 	LDI	@_countdown,R0
    // asm 00005A65: 	BGT	CTLP
    if (_countdown > 0)
        goto CTLP;
CTLPX:
    // asm 00005A66: 	SONDFX	TRANS
    ONESNDFX(TRANS);
    // asm 00005A68: 	LDF	@START_RADY,R2
    // asm 00005A69: 	LDI	@MATRIXAI,AR2
    // asm 00005A6A: 	CALL	HPFIND_YMATRIX
    HPFIND_YMATRIX(&MATRIXAI, C3X_REG(START_RADY));
    // asm 00005A6B: 	CALL	CLR_VECTORA
    // asm 00005A6C: 	FLOAT	-650,R0
    // asm 00005A6D: 	STF	R0,*+AR2(Y)
    VECTORAI.Y = C3X_STF_INT(-650);
    // asm 00005A6E: 	FLOAT	-28000,R0
    // asm 00005A6F: 	STF	R0,*+AR2(Z)
    VECTORAI.Z = C3X_STF_INT(-28000);
    // asm 00005A70: 	LDI	@MATRIXAI,R2
    // asm 00005A71: 	LDI	AR2,R3
    // asm 00005A72: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORAI, &MATRIXAI, &VECTORAI);
    // asm 00005A73: 	LDF	*+AR2(X),R2
    // asm 00005A74: 	ADDF	@START_POS+X,R2
    // asm 00005A75: 	LDP	@_CAMERAPOS+X
    // asm 00005A76: 	STF	R2,@_CAMERAPOS+X
    _CAMERAPOS.X = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.X), C3X_REG(START_POS[0])));
    // asm 00005A77: 	SETDP
    // asm 00005A78: 	LDF	*+AR2(Y),R2
    // asm 00005A79: 	ADDF	@START_POS+Y,R2
    // asm 00005A7A: 	LDP	@_CAMERAPOS+Y
    // asm 00005A7B: 	STF	R2,@_CAMERAPOS+Y
    _CAMERAPOS.Y = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.Y), C3X_REG(START_POS[1])));
    // asm 00005A7C: 	SETDP
    // asm 00005A7D: 	LDF	*+AR2(Z),R2
    // asm 00005A7E: 	ADDF	@START_POS+Z,R2
    // asm 00005A7F: 	LDP	@_CAMERAPOS+Z
    // asm 00005A80: 	STF	R2,@_CAMERAPOS+Z
    _CAMERAPOS.Z = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.Z), C3X_REG(START_POS[2])));
    // asm 00005A81: 	SETDP
    // asm 00005A82: 	CREATE	CENTERTHEONE,UTIL_C|CHOOSECAR_T
    child_ctx = NEW_PROC_CONTEXT();
    CREATE(CENTERTHEONE, UTIL_C | CHOOSECAR_T, child_ctx);
    // asm 00005A85: 	CREATE	DROPTHEOTHER,UTIL_C|CHOOSETRANS_T
    child_ctx = NEW_PROC_CONTEXT();
    CREATE(DROPTHEOTHER, UTIL_C | CHOOSETRANS_T, child_ctx);
    // asm 00005A88: 	CALL	SNAPCURSOR
    SNAPCURSOR();
    // asm 00005A89: 	NEGF	@START_RADY,R2
    // asm 00005A8A: 	LDP	@_CAMERARAD+Y
    // asm 00005A8B: 	STF	R2,@_CAMERARAD+Y
    _CAMERARAD.Y = C3X_STF(C3X_NEG(C3X_REG(START_RADY)));
    // asm 00005A8C: 	SETDP
    // asm 00005A8D: 	LDI	@CAMERAMATRIXI,AR2
    // asm 00005A8E: 	CALL	HPFIND_YMATRIX
    HPFIND_YMATRIX(&CAMERAMATRIXI, C3X_LDF(_CAMERARAD.Y));
    // asm 00005A8F: 	LDL	garage_grp,AR2
    // asm 00005A90: 	CALL	LOAD_SINGLE_SECTION_OFFSET
    LOAD_SINGLE_SECTION_OFFSET((LOAD_SINGLE_SECTION_GROUP*)ROM_PTR(garage_grp_ROM));
    // asm 00005A91: 	SLEEP	3
    SLEEP(3, 3);
    // asm 00005A93: 	SONDFX	DD1
    SONDFX(DD1);
    // asm 00005A95: 	SLEEP	7
    SLEEP(7, 4);
    // asm 00005A97: 	CREATE	MOVE_PUSH_BOX,UTIL_C|CHOOSETRANS_T
    child_ctx = NEW_PROC_CONTEXT();
    CREATE(MOVE_PUSH_BOX, UTIL_C | CHOOSETRANS_T, child_ctx);
    // asm 00005A9A: 	CALL	GETTHECARS
    GETTHECARS();
    // asm 00005A9B: 	CLRI	R0
    // asm 00005A9C: 	STI	R0,@LASTCHOICE
    LASTCHOICE = 0;
    // asm 00005A9D: 	LDI	4,R0
    // asm 00005A9E: 	STI	R0,@POSES
    POSES = 4;
    // asm 00005A9F: 	LDI	2,R0
    // asm 00005AA0: 	STI	R0,@POSE
    POSE = 2;
    // asm 00005AA1: 	LDI	1,R0
    // asm 00005AA2: 	STI	R0,@CHOSEN_VEHICLE
    CHOSEN_VEHICLE = 1;
    // asm 00005AA3: 	CREATE	ROUNDER,UTIL_C|CHOOSECAR_T
    child_ctx = NEW_PROC_CONTEXT();
    CREATE(ROUNDER, UTIL_C | CHOOSECAR_T, child_ctx);
    // asm 00005AA6: 	CLRF	R0
    // asm 00005AA7: 	STF	R0,@DOORTHETA
    DOORTHETA = C3X_STF_INT(0);
    // asm 00005AA8: 	SONDFX	DOPEN
    SONDFX(DOPEN);
    // asm 00005AAA: 	JSRP	OPEN_DOOR_PROC
    JSRP(OPEN_DOOR_PROC, 5);
    // asm 00005AB0: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void OPEN_DOOR_PROC(PROC* p) {
    c3x_reg_t camera_component;
    PROC_CONTEXT* car_choice_ctx;

    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        // MAME_ASSERT_ORDERING("OPEN_DOOR_PROC");
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 00005AB4: 	CREATE	THE_CAR_CHOICE_PROC,UTIL_C
    car_choice_ctx = NEW_PROC_CONTEXT();
    CREATE(THE_CAR_CHOICE_PROC, UTIL_C, car_choice_ctx);
    // asm 00005AB7: 	LDI	@VECTORCI,AR2
    // asm 00005AB8: 	CLRF	R0
    // asm 00005AB9: 	STF	R0,*+AR2(X)
    VECTORCI.X = C3X_STF_INT(0);
    // asm 00005ABA: 	STF	R0,*+AR2(Y)
    VECTORCI.Y = C3X_STF_INT(0);
    // asm 00005ABB: 	FLOAT	-8500,R0
    // asm 00005ABC: 	STF	R0,*+AR2(Z)
    VECTORCI.Z = C3X_STF_INT(-8500);
    // asm 00005ABD: 	LDI	@MATRIXAI,R2
    // asm 00005ABE: 	LDI	AR2,R3
    // asm 00005ABF: 	CALL	MATRIX_MUL
    MATRIX_MUL(&VECTORCI, &MATRIXAI, &VECTORCI);
    // asm 00005AC0: 	LDF	*+AR2(X),R0
    // asm 00005AC1: 	ADDF	@START_POS+X,R0
    // asm 00005AC2: 	STF	R0,*+AR2(X)
    VECTORCI.X = C3X_STF(C3X_ADD(C3X_LDF(VECTORCI.X), C3X_REG(START_POS[0])));
    // asm 00005AC3: 	LDF	*+AR2(Z),R0
    // asm 00005AC4: 	ADDF	@START_POS+Z,R0
    // asm 00005AC5: 	STF	R0,*+AR2(Z)
    VECTORCI.Z = C3X_STF(C3X_ADD(C3X_LDF(VECTORCI.Z), C3X_REG(START_POS[2])));
    // asm 00005AC6: 	LDI	30,AR4
    p->ctx.OPEN_DOOR_PROC_FRAME.loop_count = 30;
IJH:
    // asm 00005AC7: LDF	@DOORTHETA,R0
    // asm 00005AC8: 	ADDF	0.0628,R0
    // asm 00005AC9: 	STF	R0,@DOORTHETA
    DOORTHETA = C3X_STF(C3X_ADD_IMM(C3X_LDF(DOORTHETA), 0.0628));
    // asm 00005ACA: 	PUSH	AR4
    // asm 00005ACB: 	CALL	DOOR_OPENING
    DOOR_OPENING();
    // asm 00005ACC: 	POP	AR4
    // asm 00005ACD: 	LDI	@CAMERAPOSI,AR0
    // asm 00005ACE: 	LDI	@VECTORCI,AR1
    // asm 00005ACF: 	LDF	*+AR1(X),R0
    // asm 00005AD0: 	SUBF	*+AR0(X),R0
    // asm 00005AD1: 	MPYF	0.15,R0
    // asm 00005AD2: 	ADDF	*+AR0(X),R0
    // asm 00005AD3: 	STF	R0,*+AR0(X)
    camera_component = C3X_SUB(C3X_LDF(VECTORCI.X), C3X_LDF(_CAMERAPOS.X));
    camera_component = C3X_MUL_IMM(camera_component, 0.15);
    _CAMERAPOS.X = C3X_STF(C3X_ADD(camera_component, C3X_LDF(_CAMERAPOS.X)));
    // asm 00005AD4: 	LDF	*+AR1(Z),R0
    // asm 00005AD5: 	SUBF	*+AR0(Z),R0
    // asm 00005AD6: 	MPYF	0.15,R0
    // asm 00005AD7: 	ADDF	*+AR0(Z),R0
    // asm 00005AD8: 	STF	R0,*+AR0(Z)
    camera_component = C3X_SUB(C3X_LDF(VECTORCI.Z), C3X_LDF(_CAMERAPOS.Z));
    camera_component = C3X_MUL_IMM(camera_component, 0.15);
    _CAMERAPOS.Z = C3X_STF(C3X_ADD(camera_component, C3X_LDF(_CAMERAPOS.Z)));
    // asm 00005AD9: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00005ADB: 	DBU	AR4,IJH
    if (p->ctx.OPEN_DOOR_PROC_FRAME.loop_count-- > 0)
        goto IJH;
    // asm 00005ADC: 	CALL	DOOR_ELEMENT_DELETE_ALL
    DOOR_ELEMENT_DELETE_ALL();
    // asm 00005ADD: 	LDI	UTIL_C|CHOOSETRANS_T,R0
    // asm 00005ADE: 	LDI	-1,R1
    // asm 00005ADF: 	CALL	PRC_KILLALL
    PRC_KILLALL(UTIL_C | CHOOSETRANS_T, -1);
    // asm 00005AE0: 	LDI	@EPALL,AR2
    // asm 00005AE1: 	CALL	PAL_FIND_RAW
    // asm 00005AE2: 	LDI	R0,AR2
    // asm 00005AE3: 	CALL	PAL_DELETE_RAW
    PAL_DELETE_RAW(PAL_FIND_RAW(EPALL));
    // asm 00005AE4: 	LDI	@EPALR,AR2
    // asm 00005AE5: 	CALL	PAL_FIND_RAW
    // asm 00005AE6: 	LDI	R0,AR2
    // asm 00005AE7: 	CALL	PAL_DELETE_RAW
    PAL_DELETE_RAW(PAL_FIND_RAW(EPALR));
    // asm 00005AE8: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *AS THE PIECES COME SLIDING IN FROM THE SIDES,
 *THE WHEEL AND PUSH TO CHOOSE COME FROM BELOW
 *
 */
static void WHEEL_FROM_BELOW(void) {
    // asm 00005AEC: 	LDI	@CT_WHEEL,AR0
    // asm 00005AED: 	LDF	*+AR0(OPOSY),R0
    // asm 00005AEE: 	SUBF	8,R0
    // asm 00005AEF: 	STF	R0,*+AR0(OPOSY)
    CT_WHEEL->pos.Y = C3X_STF(C3X_SUB_IMM(C3X_LDF(CT_WHEEL->pos.Y), 8));
    // asm 00005AF0: 	LDI	@CT_PUSHTOCYCLE,AR0
    // asm 00005AF1: 	LDF	*+AR0(OPOSY),R0
    // asm 00005AF2: 	SUBF	8,R0
    // asm 00005AF3: 	STF	R0,*+AR0(OPOSY)
    CT_PUSHTOCYCLE->pos.Y = C3X_STF(C3X_SUB_IMM(C3X_LDF(CT_PUSHTOCYCLE->pos.Y), 8));
    // asm 00005AF4: 	LDI	@CT_TURNTOSEL,AR0
    // asm 00005AF5: 	LDF	*+AR0(OPOSY),R0
    // asm 00005AF6: 	SUBF	8,R0
    // asm 00005AF7: 	STF	R0,*+AR0(OPOSY)
    CT_TURNTOSEL->pos.Y = C3X_STF(C3X_SUB_IMM(C3X_LDF(CT_TURNTOSEL->pos.Y), 8));
    // asm 00005AF8: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CENTERTHEONE(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

    // asm 00005AF9: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005AFA: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm 00005AFB: 	LDIEQ	AUD_MANUAL_TRANS_SELECTED,AR2
    // asm 00005AFC: 	LDINE	AUD_AUTOMATIC_TRANS_SELECTED,AR2
    // asm 00005AFD: 	CALL	AUDIT_INC
    AUDIT_INC(CHOSEN_TRANSMISSION == MANUAL_TRANSMISSION ? AUD_MANUAL_TRANS_SELECTED : AUD_AUTOMATIC_TRANS_SELECTED);
    // asm 00005AFE: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005AFF: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm 00005B00: 	LDIEQ	@CT_MAN,AR4
    // asm 00005B01: 	LDINE	@CT_AUTO,AR4
    p->ctx.TRANS_CENTER_THEONE.obj = CHOSEN_TRANSMISSION == MANUAL_TRANSMISSION ? CT_MAN : CT_AUTO;
    // asm 00005B02: 	LDI	AR4,AR2
    // asm 00005B03: 	CALL	DOOR_ELEMENT_DELETE
    DOOR_ELEMENT_DELETE(p->ctx.TRANS_CENTER_THEONE.obj);
    // asm 00005B04: 	LDI	15,AR5
    p->ctx.TRANS_CENTER_THEONE.loop_count = 15;
CTOLP:
    // asm 00005B05: LDF	*+AR4(OPOSX),R0
    // asm 00005B06: 	MPYF	0.8,R0
    // asm 00005B07: 	STF	R0,*+AR4(OPOSX)
    p->ctx.TRANS_CENTER_THEONE.obj->pos.X = C3X_STF(C3X_MUL_IMM(C3X_LDF(p->ctx.TRANS_CENTER_THEONE.obj->pos.X), 0.8));
    // asm 00005B08: 	LDF	*+AR4(OPOSY),R0
    // asm 00005B09: 	MPYF	0.8,R0
    // asm 00005B0A: 	STF	R0,*+AR4(OPOSY)
    p->ctx.TRANS_CENTER_THEONE.obj->pos.Y = C3X_STF(C3X_MUL_IMM(C3X_LDF(p->ctx.TRANS_CENTER_THEONE.obj->pos.Y), 0.8));
    // asm 00005B0B: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00005B0D: 	DBU	AR5,CTOLP
    if (p->ctx.TRANS_CENTER_THEONE.loop_count-- > 0)
        goto CTOLP;
    // asm 00005B0E: 	LDF	1,R7
    // asm 00005B0F: 	LDF	1,R6
    p->ctx.TRANS_CENTER_THEONE.x_velocity = C3X_IMM_F32(1);
FLYUPL:
    // asm 00005B10: 	LDF	*+AR4(ORADX),R2
    // asm 00005B11: 	SUBF	0.1,R2
    // asm 00005B12: 	STF	R2,*+AR4(ORADX)
    p->ctx.TRANS_CENTER_THEONE.obj->rad.X = C3X_STF(C3X_SUB_IMM(C3X_LDF(p->ctx.TRANS_CENTER_THEONE.obj->rad.X), 0.1));
    // asm 00005B13: 	LDI	AR4,AR2
    // asm 00005B14: 	ADDI	OMATRIX,AR2
    // asm 00005B15: 	CALL	FIND_XMATRIX
    FIND_XMATRIX(&p->ctx.TRANS_CENTER_THEONE.obj->omatrix, C3X_LDF(p->ctx.TRANS_CENTER_THEONE.obj->rad.X));
    // asm 00005B16: 	MPYF	1.4,R6
    p->ctx.TRANS_CENTER_THEONE.x_velocity = C3X_MUL_IMM(p->ctx.TRANS_CENTER_THEONE.x_velocity, 1.4);
    // asm 00005B17: 	MPYF	1.4,R7
    // asm 00005B18: 	LDF	*+AR4(OPOSX),R0
    // asm 00005B19: 	ADDF	R6,R0
    // asm 00005B1A: 	STF	R0,*+AR4(OPOSX)
    p->ctx.TRANS_CENTER_THEONE.obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(p->ctx.TRANS_CENTER_THEONE.obj->pos.X), p->ctx.TRANS_CENTER_THEONE.x_velocity));
    // asm 00005B1B: 	LDF	*+AR4(OPOSY),R0
    // asm 00005B1C: 	SUBF	R6,R0
    // asm 00005B1D: 	STF	R0,*+AR4(OPOSY)
    p->ctx.TRANS_CENTER_THEONE.obj->pos.Y = C3X_STF(C3X_SUB(C3X_LDF(p->ctx.TRANS_CENTER_THEONE.obj->pos.Y), p->ctx.TRANS_CENTER_THEONE.x_velocity));
    // asm 00005B1E: 	LDF	*+AR4(OPOSZ),R0
    // asm 00005B1F: 	SUBF	R6,R0
    // asm 00005B20: 	STF	R0,*+AR4(OPOSZ)
    p->ctx.TRANS_CENTER_THEONE.obj->pos.Z = C3X_STF(C3X_SUB(C3X_LDF(p->ctx.TRANS_CENTER_THEONE.obj->pos.Z), p->ctx.TRANS_CENTER_THEONE.x_velocity));
    // asm 00005B21: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00005B23: 	BU	FLYUPL
    goto FLYUPL;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void DROPTHETURN(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00005B24: 	LDI	@CT_TURNTOSEL,AR4
    p->ctx.DROP_TRANS_ELEMENT.obj = CT_TURNTOSEL;
    // asm 00005B25: 	BU	J85
    PROC_CONTINUE(DROP_TRANS_ELEMENT, 1);
    return;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

void DROPTHECYCLE(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00005B26: 	LDI	@CT_PUSHTOCYCLE,AR4
    p->ctx.DROP_TRANS_ELEMENT.obj = CT_PUSHTOCYCLE;
    // asm 00005B27: 	BU	J85
    PROC_CONTINUE(DROP_TRANS_ELEMENT, 1);
    return;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

void DROPTHEWHEEL(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00005B28: 	LDI	@CT_WHEEL,AR4
    p->ctx.DROP_TRANS_ELEMENT.obj = CT_WHEEL;
    // asm 00005B29: 	BU	J85
    PROC_CONTINUE(DROP_TRANS_ELEMENT, 1);
    return;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

void DROPTHEOTHER(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 00005B2A: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005B2B: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm 00005B2C: 	LDINE	@CT_MAN,AR4
    // asm 00005B2D: 	LDIEQ	@CT_AUTO,AR4
    p->ctx.DROP_TRANS_ELEMENT.obj = CHOSEN_TRANSMISSION == MANUAL_TRANSMISSION ? CT_AUTO : CT_MAN;
    PROC_CONTINUE(DROP_TRANS_ELEMENT, 1);
    return;
}

static void DROP_TRANS_ELEMENT(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_ORDERING("J85");
        break;
    case 1:
        goto PROC_RESUME_1;
    }
J85:
    // asm 00005B2E: LDI	AR4,AR2
    // asm 00005B2F: 	CALL	DOOR_ELEMENT_DELETE
    DOOR_ELEMENT_DELETE(p->ctx.DROP_TRANS_ELEMENT.obj);
J86:
    // asm 00005B30: LDI	10,AR5
    // asm 00005B31: 	LDF	2,R6
    p->ctx.DROP_TRANS_ELEMENT.loop_count = 10;
    p->ctx.DROP_TRANS_ELEMENT.y_velocity = C3X_IMM_F32(2);
    // asm 00005B32: DTOLP
DTOLP:
    // asm 00005B32: 	LDF	*+AR4(OPOSY),R0
    // asm 00005B33: 	ADDF	R6,R0
    // asm 00005B34: 	STF	R0,*+AR4(OPOSY)
    p->ctx.DROP_TRANS_ELEMENT.obj->pos.Y = C3X_STF(C3X_ADD(C3X_LDF(p->ctx.DROP_TRANS_ELEMENT.obj->pos.Y), p->ctx.DROP_TRANS_ELEMENT.y_velocity));
    // asm 00005B35: 	MPYF	2,R6
    p->ctx.DROP_TRANS_ELEMENT.y_velocity = C3X_MUL_IMM(p->ctx.DROP_TRANS_ELEMENT.y_velocity, 2);
    // asm 00005B36: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00005B38: 	DBU	AR5,DTOLP
    if (p->ctx.DROP_TRANS_ELEMENT.loop_count-- > 0)
        goto DTOLP;
    // asm 00005B39: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SIDE_DOOR(void) {
    OBJ* obj;

    // asm 00005B3A: 	LDI	@DOORLISTI,AR4
    // asm 00005B3B: 	SUBI	OLINK2,AR4
DOLS:
    // asm 00005B3C: LDI	*+AR4(OLINK2),R0
    // asm 00005B3D: 	BZ	DOLSX
    obj = DOORLIST;
    while (obj != NULL) {
        // asm 00005B3E: 	LDI	R0,AR4
        // asm 00005B3F: 	CMPI	@CT_MAN,AR4
        // asm 00005B40: 	BEQ	DOLS
        // asm 00005B41: 	CMPI	@CT_AUTO,AR4
        // asm 00005B42: 	BEQ	DOLS
        if (obj != CT_MAN && obj != CT_AUTO) {
            // asm 00005B43: 	LDI	*+AR4(OID),R0
            // asm 00005B44: 	ANDN	0FFh,R0
            // asm 00005B45: 	CMPI	100h,R0
            // asm 00005B46: 	BNE	NTLTS
            if ((obj->id & ~0xffu) == 0x100u) {
                // asm 00005B47: 	LDF	*+AR4(OPOSX),R0
                // asm 00005B48: 	ADDF	15,R0
                // asm 00005B49: 	STF	R0,*+AR4(OPOSX)
                obj->pos.X = C3X_STF(C3X_ADD_IMM(C3X_LDF(obj->pos.X), 15));
                // asm 00005B4A: 	BU	DOLS
            } else {
            NTLTS:
                // asm 00005B4B: 	LDF	*+AR4(OPOSX),R0
                // asm 00005B4C: 	SUBF	15,R0
                // asm 00005B4D: 	STF	R0,*+AR4(OPOSX)
                obj->pos.X = C3X_STF(C3X_SUB_IMM(C3X_LDF(obj->pos.X), 15));
                // asm 00005B4E: 	BU	DOLS
            }
        }
        obj = (OBJ*)obj->link2;
    }
DOLSX:
    // asm 00005B4F: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *	OLINK2	- the link of the list
 *	OID	- 1 == left
 *		- 2 == right
 *
 *	OVEL?	- original position
 *
 *
 *
 */
/* asm: DOORTHETA	.bss	DOORTHETA,1 */
c3x_f32_t DOORTHETA;
/* asm: DOORLIST	.bss	DOORLIST,1 */
OBJ* DOORLIST;
/*
 *
 *
 */
/* asm: DO_LPPX	.float	-256 */
static const c3x_f32_t DO_LPPX = C3X_F32_INIT(-256.0f);
/* asm: DO_PPZ	.float	368 */
static const c3x_f32_t DO_PPZ = C3X_F32_INIT(368.0f);
/* asm: DO_RPPX	.float	256 */
static const c3x_f32_t DO_RPPX = C3X_F32_INIT(256.0f);

/*
 *
 *
 */
void DOOR_OPENING(void) {
    OBJ* obj;

    // asm 00005B54: 	LDF	@DOORTHETA,R2
    // asm 00005B55: 	LDI	@MATRIXAI,AR2
    // asm 00005B56: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXAI, C3X_LDF(DOORTHETA));
    // asm 00005B57: 	NEGF	@DOORTHETA,R2
    // asm 00005B58: 	LDI	@MATRIXBI,AR2
    // asm 00005B59: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&MATRIXBI, C3X_NEG(C3X_LDF(DOORTHETA)));
    // asm 00005B5A: 	LDI	@DOORLISTI,AR4
    // asm 00005B5B: 	SUBI	OLINK2,AR4
DOL:
    // asm 00005B5C: LDI	*+AR4(OLINK2),R0
    // asm 00005B5D: 	BZ	DOLX
    obj = DOORLIST;
    while (obj != NULL) {
        // asm 00005B5E: 	LDI	R0,AR4
        // asm 00005B5F: 	CALL	CLR_VECTORA
        CLR_VECTORA();
        // asm 00005B60: 	LDI	*+AR4(OID),R0
        // asm 00005B61: 	ANDN	0FFh,R0
        // asm 00005B62: 	CMPI	100h,R0
        // asm 00005B63: 	BNE	NOTLEFT
        if ((obj->id & ~0xffu) == 0x100u) {
            // asm 00005B64: 	LDF	*+AR4(OVELX),R0
            // asm 00005B65: 	SUBF	@DO_LPPX,R0
            // asm 00005B66: 	STF	R0,*+AR2(X)
            VECTORAI.X = C3X_STF(C3X_SUB(C3X_LDF(obj->vel_x), C3X_LDF(DO_LPPX)));
            // asm 00005B67: 	LDF	*+AR4(OVELZ),R0
            // asm 00005B68: 	SUBF	@DO_PPZ,R0
            // asm 00005B69: 	STF	R0,*+AR2(Z)
            VECTORAI.Z = C3X_STF(C3X_SUB(C3X_LDF(obj->vel_z), C3X_LDF(DO_PPZ)));
            // asm 00005B6A: 	LDI	@MATRIXAI,R2
            // asm 00005B6B: 	LDI	AR2,R3
            // asm 00005B6C: 	CALL	MATRIX_MUL
            MATRIX_MUL(&VECTORAI, &MATRIXAI, &VECTORAI);
            // asm 00005B6D: 	LDF	*+AR2(X),R0
            // asm 00005B6E: 	ADDF	@DO_LPPX,R0
            // asm 00005B6F: 	STF	R0,*+AR4(OPOSX)
            obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.X), C3X_LDF(DO_LPPX)));
            // asm 00005B70: 	LDF	*+AR2(Z),R0
            // asm 00005B71: 	ADDF	@DO_PPZ,R0
            // asm 00005B72: 	STF	R0,*+AR4(OPOSZ)
            obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.Z), C3X_LDF(DO_PPZ)));
            // asm 00005B73: 	LDI	@MATRIXAI,R2
            // asm 00005B74: 	LDI	AR4,AR2
            // asm 00005B75: 	ADDI	OMATRIX,AR2
            // asm 00005B76: 	CALL	CPYMAT
            CPYMAT((MATRIX*)&obj->omatrix, &MATRIXAI);
            // asm 00005B77: 	BU	KJL
        } else {
        NOTLEFT:
            // asm 00005B78: 	LDF	*+AR4(OVELX),R0
            // asm 00005B79: 	SUBF	@DO_RPPX,R0
            // asm 00005B7A: 	STF	R0,*+AR2(X)
            VECTORAI.X = C3X_STF(C3X_SUB(C3X_LDF(obj->vel_x), C3X_LDF(DO_RPPX)));
            // asm 00005B7B: 	LDF	*+AR4(OVELZ),R0
            // asm 00005B7C: 	SUBF	@DO_PPZ,R0
            // asm 00005B7D: 	STF	R0,*+AR2(Z)
            VECTORAI.Z = C3X_STF(C3X_SUB(C3X_LDF(obj->vel_z), C3X_LDF(DO_PPZ)));
            // asm 00005B7E: 	LDI	@MATRIXBI,R2
            // asm 00005B7F: 	LDI	AR2,R3
            // asm 00005B80: 	CALL	MATRIX_MUL
            MATRIX_MUL(&VECTORAI, &MATRIXBI, &VECTORAI);
            // asm 00005B81: 	LDF	*+AR2(X),R0
            // asm 00005B82: 	ADDF	@DO_RPPX,R0
            // asm 00005B83: 	STF	R0,*+AR4(OPOSX)
            obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.X), C3X_LDF(DO_RPPX)));
            // asm 00005B84: 	LDF	*+AR2(Z),R0
            // asm 00005B85: 	ADDF	@DO_PPZ,R0
            // asm 00005B86: 	STF	R0,*+AR4(OPOSZ)
            obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(VECTORAI.Z), C3X_LDF(DO_PPZ)));
            // asm 00005B87: 	LDI	@MATRIXBI,R2
            // asm 00005B88: 	LDI	AR4,AR2
            // asm 00005B89: 	ADDI	OMATRIX,AR2
            // asm 00005B8A: 	CALL	CPYMAT
            CPYMAT((MATRIX*)&obj->omatrix, &MATRIXBI);
        }
    KJL:
        // asm 00005B8B: 	BU	DOL
        obj = (OBJ*)obj->link2;
    }
DOLX:
    // asm 00005B8C: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void SNAPCURSOR(void) {
    // asm 00005B8D: 	LDI	@CT_CURSOR,AR2
    // asm 00005B8E: 	CALL	DOOR_ELEMENT_DELETE
    DOOR_ELEMENT_DELETE(CT_CURSOR);
    // asm 00005B8F: 	CALL	OBJ_DELETE
    OBJ_DELETE(CT_CURSOR);
    // asm 00005B90: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 */
/* asm: LASTCHOICEA	.bss	LASTCHOICEA,1 */
int LASTCHOICEA;

void WHEEL_ROUT(void) {
    c3x_reg_t unit_wheel;
    c3x_reg_t target_x;
    c3x_reg_t inset;
    c3x_reg_t steering_min;
    c3x_reg_t steering_max;

    // asm 00005B91: 	CALL	GET_UNIT_WHEEL
    unit_wheel = GET_UNIT_WHEEL();
    // asm 00005B92: 	LDI	@CHOSEN_TRANSMISSION,R1
    // asm 00005B93: 	LDI	@CT_CURSOR,AR2
    // asm 00005B94: 	CMPF	0.1,R0
    // asm 00005B95: 	LDFLT	0.0,R0
    if (C3X_LT_IMM(unit_wheel, 0.1))
        unit_wheel = C3X_IMM_F32(0.0);
    // asm 00005B96: 	CMPF	0.9,R0
    // asm 00005B97: 	LDFGT	1.0,R0
    if (C3X_GT_IMM(unit_wheel, 0.9))
        unit_wheel = C3X_IMM_F32(1.0);
    // asm 00005B98: 	CMPF	0.5,R0
    // asm 00005B99: 	BLT	ISL
    // asm 00005B9A: 	CMPF	0.55,R0
    // asm 00005B9B: 	BGT	ISR
    // asm 00005B9C: 	CMPI	AUTO_TRANSMISSION,R1
    // asm 00005B9D: 	BEQ	ISLT
    if (C3X_LT_IMM(unit_wheel, 0.5)) {
        if (C3X_LT_IMM(unit_wheel, 0.45))
            goto ISLT;
        if (CHOSEN_TRANSMISSION == MANUAL_TRANSMISSION)
            goto ISR;
        goto ISLT;
    }
    if (!C3X_GT_IMM(unit_wheel, 0.55) && CHOSEN_TRANSMISSION == AUTO_TRANSMISSION)
        goto ISLT;
ISR:
    // asm 00005B9E: LDF	*+AR2(OPOSX),R2
    // asm 00005B9F: 	LDF	101,R0
    // asm 00005BA0: 	SUBF	R2,R0
    // asm 00005BA1: 	MPYF	0.22,R0
    // asm 00005BA2: 	ADDF	*+AR2(OPOSX),R0
    // asm 00005BA3: 	STF	R0,*+AR2(OPOSX)
    target_x = C3X_RSUB_IMM(101, C3X_LDF(CT_CURSOR->pos.X));
    target_x = C3X_MUL_IMM(target_x, 0.22);
    CT_CURSOR->pos.X = C3X_STF(C3X_ADD(target_x, C3X_LDF(CT_CURSOR->pos.X)));
    // asm 00005BA4: 	LDI	MANUAL_TRANSMISSION,R0
    // asm 00005BA5: 	STI	R0,@CHOSEN_TRANSMISSION
    CHOSEN_TRANSMISSION = MANUAL_TRANSMISSION;
    // asm 00005BA6: 	BU	ISDN2
    goto ISDN2;
ISL:
    // asm 00005BA7: 	CMPF	0.45,R0
    // asm 00005BA8: 	BLT	ISLT
    // asm 00005BA9: 	CMPI	MANUAL_TRANSMISSION,R1
    // asm 00005BAA: 	BEQ	ISR
ISLT:
    // asm 00005BAB: LDF	*+AR2(OPOSX),R2
    // asm 00005BAC: 	LDF	-101,R0
    // asm 00005BAD: 	SUBF	R2,R0
    // asm 00005BAE: 	MPYF	0.22,R0
    // asm 00005BAF: 	ADDF	*+AR2(OPOSX),R0
    // asm 00005BB0: 	STF	R0,*+AR2(OPOSX)
    target_x = C3X_RSUB_IMM(-101, C3X_LDF(CT_CURSOR->pos.X));
    target_x = C3X_MUL_IMM(target_x, 0.22);
    CT_CURSOR->pos.X = C3X_STF(C3X_ADD(target_x, C3X_LDF(CT_CURSOR->pos.X)));
    // asm 00005BB1: 	LDI	AUTO_TRANSMISSION,R0
    // asm 00005BB2: 	STI	R0,@CHOSEN_TRANSMISSION
    CHOSEN_TRANSMISSION = AUTO_TRANSMISSION;
ISDN2:
    // asm 00005BB3: 	LDF	@STEERMN,R1
    // asm 00005BB4: 	LDF	@STEERMX,R2
    // asm 00005BB5: 	SUBF	R1,R2,R3
    steering_min = C3X_LDF(STEERMN);
    steering_max = C3X_LDF(STEERMX);
    inset = C3X_SUB(steering_max, steering_min);
    // asm 00005BB6: 	MPYF	0.2,R3
    inset = C3X_MUL_IMM(inset, 0.2);
    // asm 00005BB7: 	ADDF	R3,R1
    steering_min = C3X_ADD(steering_min, inset);
    // asm 00005BB8: 	SUBF	R3,R2
    steering_max = C3X_SUB(steering_max, inset);
    // asm 00005BB9: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005BBA: 	CMPI	AUTO_TRANSMISSION,R0
    // asm 00005BBB: 	LDFEQ	R1,R0
    // asm 00005BBC: 	LDFNE	R2,R0
    // asm 00005BBD: 	STF	R0,@WHEELPOS
    WHEELPOS = C3X_STF(CHOSEN_TRANSMISSION == AUTO_TRANSMISSION ? steering_min : steering_max);
    // asm 00005BBE: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005BBF: 	LDI	@LASTCHOICEA,R1
    // asm 00005BC0: 	CMPI	R0,R1
    // asm 00005BC1: 	BEQ	NUTHIN
    if (LASTCHOICEA == CHOSEN_TRANSMISSION)
        goto NUTHIN;
    // asm 00005BC2: 	STI	R0,@LASTCHOICEA
    LASTCHOICEA = CHOSEN_TRANSMISSION;
    // asm 00005BC3: 	CMPI	AUTO_TRANSMISSION,R0
    // asm 00005BC4: 	LDIEQ	GSWIPE1,AR2
    // asm 00005BC5: 	LDINE	GSWIPE2,AR2
    // asm 00005BC6: 	CALL	ONESNDFX
    ONESNDFX(CHOSEN_TRANSMISSION == AUTO_TRANSMISSION ? GSWIPE1 : GSWIPE2);
NUTHIN:
    // asm 00005BC7: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void DOOR_ELEMENT_DELETE_ALL(void) {
    OBJ* obj;

    // asm 00005BC8: 	LDI	@DOORLIST,R0
DEDL:
    // asm 00005BC9: LDI	R0,AR0
    // asm 00005BCA: 	CMPI	0,AR0
    // asm 00005BCB: 	BEQ	DEDX
    obj = DOORLIST;
    while (obj != NULL) {
        // asm 00005BCC: 	LDI	*+AR0(OLINK2),R0
        // asm 00005BCD: 	LDI	AR0,AR2
        // asm 00005BCE: 	CALL	OBJ_DELETE
        OBJ* next = (OBJ*)obj->link2;
        OBJ_DELETE(obj);
        obj = next;
        // asm 00005BCF: 	BU	DEDL
    }
DEDX:
    // asm 00005BD0: 	CLRI	R0
    // asm 00005BD1: 	STI	R0,@DOORLIST
    DOORLIST = NULL;
    // asm 00005BD2: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR2	ELEMENT TO DELETE
 *
 */
void DOOR_ELEMENT_DELETE(OBJ* element /*AR2*/) {
    OBJ* previous;
    OBJ* current;

    // asm 00005BD3: 	PUSH	R0
    // asm 00005BD4: 	PUSH	AR1
    // asm 00005BD5: 	LDI	@DOORLISTI,R0
    // asm 00005BD6: 	SUBI	OLINK2,R0		;(we are offset pointing)
DELLP:
    // asm 00005BD7: LDI	R0,AR1
    // asm 00005BD8: 	LDI	*+AR1(OLINK2),R0
    // asm 00005BD9: 	BZ	FREEDE_X
    previous = NULL;
    current = DOORLIST;
    while (current != NULL) {
        // asm 00005BDA: 	CMPI	R0,AR2
        // asm 00005BDB: 	BNE	DELLP
        if (current == element) {
            // asm 00005BDC: 	LDI	*+AR2(OLINK2),R0
            // asm 00005BDD: 	STI	R0,*+AR1(OLINK2)	;LINK AROUND
            if (previous == NULL) {
                DOORLIST = (OBJ*)element->link2; // LINK AROUND
            } else {
                previous->link2 = element->link2; // LINK AROUND
            }
            break;
        }
        previous = current;
        current = (OBJ*)current->link2;
    }
FREEDE_X:
    // asm 00005BDE: 	POP	AR1
    // asm 00005BDF: 	POP	R0
    // asm 00005BE0: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void TRANSCHOICE(void) {
    c3x_reg_t wheel_position;
    c3x_reg_t angle;

    // asm 00005BE1: 	CALL	GET_UNIT_WHEEL
    wheel_position = GET_UNIT_WHEEL();
    // asm 00005BE2: 	LDF	PI,R2
    angle = C3X_IMM_F32(PI);
    // asm 00005BE3: 	MPYF	R0,R2
    angle = C3X_MUL(wheel_position, angle);
    // asm 00005BE4: 	SUBF	HALFPI,R2
    angle = C3X_SUB_IMM(angle, HALFPI);
    // asm 00005BE5: 	NEGF	R2
    angle = C3X_NEG(angle);
    // asm 00005BE6: 	LDI	@CT_WHEEL,AR2
    // asm 00005BE7: 	ADDI	OMATRIX,AR2
    // asm 00005BE8: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&CT_WHEEL->omatrix, angle);
    // asm 00005BE9: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *RETURNS
 *	R0	FL 0-1.0 Steering wheel position
 *
 */
/* asm: GUWP	.bss	GUWP,1 */
c3x_f32_t GUWP = C3X_F32_INIT(0.0f);

c3x_reg_t GET_UNIT_WHEEL(void) {
    c3x_reg_t wheel_position;
    c3x_reg_t steering_range;

    // asm 00005BEA: 	FLOAT	@_pot0,R0
    wheel_position = C3X_FROM_INT(_pot0);
    // asm 00005BEB: 	LDF	@GUWP,R1
    // asm 00005BEC: 	MPYF	0.2,R0
    wheel_position = C3X_MUL_IMM(wheel_position, 0.2);
    // asm 00005BED: 	MPYF	0.8,R1
    steering_range = C3X_MUL_IMM(C3X_LDF(GUWP), 0.8);
    // asm 00005BEE: 	ADDF	R1,R0
    wheel_position = C3X_ADD(wheel_position, steering_range);
    // asm 00005BEF: 	STF	R0,@GUWP
    GUWP = C3X_STF(wheel_position);
    // asm 00005BF0: 	SUBF	@STEERMN,R0
    wheel_position = C3X_SUB(wheel_position, C3X_LDF(STEERMN));
    // asm 00005BF1: 	LDF	@STEERMX,R1
    // asm 00005BF2: 	SUBF	@STEERMN,R1
    steering_range = C3X_SUB(C3X_LDF(STEERMX), C3X_LDF(STEERMN));
    // asm 00005BF3: 	CALL	DIV_F30
    wheel_position = DIV_F30(wheel_position, steering_range);
    // asm 00005BF4: 	RETS
    return wheel_position;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void TILE_PIECES(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

    // asm 00005BF5: 	SLEEP	10
    SLEEP(10, 1);
    // asm 00005BF7: 	LDI	29,AR5
    p->ctx.TILE_PIECES.loop_count = 29;
    // asm 00005BF8: TP_LP
TP_LP:
    // asm 00005BF8: 	LDI	@CT_AUTO,AR4
    // asm 00005BF9: 	LDF	*+AR4(ORADZ),R2
    // asm 00005BFA: 	ADDF	0.3,R2
    // asm 00005BFB: 	STF	R2,*+AR4(ORADZ)
    CT_AUTO->rad.Z = C3X_STF(C3X_ADD_IMM(C3X_LDF(CT_AUTO->rad.Z), 0.3));
    // asm 00005BFC: 	LDI	AR4,AR2
    // asm 00005BFD: 	ADDI	OMATRIX,AR2
    // asm 00005BFE: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&CT_AUTO->omatrix, C3X_LDF(CT_AUTO->rad.Z));
    // asm 00005BFF: 	LDF	*+AR4(OPOSY),R0	;300, 400
    // asm 00005C00: 	SUBF	13.3,R0
    // asm 00005C01: 	STF	R0,*+AR4(OPOSY)
    CT_AUTO->pos.Y = C3X_STF(C3X_SUB_IMM(C3X_LDF(CT_AUTO->pos.Y), 13.3));
    // asm 00005C02: 	LDF	*+AR4(OPOSX),R0
    // asm 00005C03: 	SUBF	8,R0
    // asm 00005C04: 	STF	R0,*+AR4(OPOSX)
    CT_AUTO->pos.X = C3X_STF(C3X_SUB_IMM(C3X_LDF(CT_AUTO->pos.X), 8));
    // asm 00005C05: 	LDI	@CT_MAN,AR4
    // asm 00005C06: 	LDF	*+AR4(ORADZ),R2
    // asm 00005C07: 	SUBF	0.3,R2
    // asm 00005C08: 	STF	R2,*+AR4(ORADZ)
    CT_MAN->rad.Z = C3X_STF(C3X_SUB_IMM(C3X_LDF(CT_MAN->rad.Z), 0.3));
    // asm 00005C09: 	LDI	AR4,AR2
    // asm 00005C0A: 	ADDI	OMATRIX,AR2
    // asm 00005C0B: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&CT_MAN->omatrix, C3X_LDF(CT_MAN->rad.Z));
    // asm 00005C0C: 	LDF	*+AR4(OPOSY),R0
    // asm 00005C0D: 	SUBF	13.3,R0
    // asm 00005C0E: 	STF	R0,*+AR4(OPOSY)
    CT_MAN->pos.Y = C3X_STF(C3X_SUB_IMM(C3X_LDF(CT_MAN->pos.Y), 13.3));
    // asm 00005C0F: 	LDF	*+AR4(OPOSX),R0
    // asm 00005C10: 	ADDF	8,R0
    // asm 00005C11: 	STF	R0,*+AR4(OPOSX)
    CT_MAN->pos.X = C3X_STF(C3X_ADD_IMM(C3X_LDF(CT_MAN->pos.X), 8));
    // asm 00005C12: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00005C14: 	DBU	AR5,TP_LP
    if (p->ctx.TILE_PIECES.loop_count-- > 0)
        goto TP_LP;
    // asm 00005C15: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *Object Z values
 *	cursor		-3
 *	wheel		-2
 *	forward objects	-1
 *	others		0
 *
 */
/* asm: CT_CURSOR	.bss	CT_CURSOR,1 */
OBJ* CT_CURSOR;
/* asm: CT_WHEEL	.bss	CT_WHEEL,1 */
OBJ* CT_WHEEL;
/* asm: CT_PUSHTOCYCLE	.bss	CT_PUSHTOCYCLE,1 */
OBJ* CT_PUSHTOCYCLE;
/* asm: CT_LENG	.bss	CT_LENG,1 */
OBJ* CT_LENG;
/* asm: CT_RENG	.bss	CT_RENG,1 */
OBJ* CT_RENG;
/* asm: CT_MAN	.bss	CT_MAN,1 */
OBJ* CT_MAN;
/* asm: CT_AUTO	.bss	CT_AUTO,1 */
OBJ* CT_AUTO;
/* asm: CT_TURNTOSEL	.bss	CT_TURNTOSEL,1 */
OBJ* CT_TURNTOSEL;
/* asm: TRANS_HEAD	.bss	TRANS_HEAD,1 */
OBJ* TRANS_HEAD;

/*
 *
 *
 */
void FIX_TRANSMISSION_SCREEN(void) {
    OBJ* obj;
    OBJ* next_obj;
    PROC_CONTEXT* ctx;
    c3x_reg_t offset;
    u32 object_id;

    // asm 00005C16: 	LDI	-1,R6
    // asm 00005C17: 	CLRI	R0
    // asm 00005C18: 	STI	R0,@DOORLIST
    DOORLIST = NULL;
    // asm 00005C19: 	LDI	@OACTIVE,AR5
    next_obj = OACTIVE;
    TRANS_HEAD = (OBJ*)(intptr_t)-1;
FTSL:
    // asm 00005C1A: LDI	AR5,R0
    // asm 00005C1B: 	BZ	FTSLX
    if (next_obj == NULL)
        goto FTSLX;
    // asm 00005C1C: 	LDI	R0,AR0
    obj = next_obj;
    // asm 00005C1D: 	LDI	*AR0,AR5
    next_obj = obj->link;
    // asm 00005C1E: 	CMPI	-1,R6
    // asm 00005C1F: 	LDIEQ	AR0,R6
    if (TRANS_HEAD == (OBJ*)(intptr_t)-1)
        TRANS_HEAD = obj;
    // asm 00005C20: 	STI	AR5,*+AR0(OLINK2)
    obj->link2 = (uintptr_t)next_obj;
    // asm 00005C21: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005C22: 	ANDN	O_1PAL,R0
    // asm 00005C23: 	OR	O_NOUNIV|O_NOUROT,R0
    // asm 00005C24: 	STI	R0,*+AR0(OFLAGS)
    obj->flags = (obj->flags & ~O_1PAL) | O_NOUNIV | O_NOUROT;
    // asm 00005C25: 	LDF	*+AR0(OPOSX),R0
    // asm 00005C26: 	STF	R0,*+AR0(OVELX)
    obj->vel_x = C3X_STF(C3X_LDF(obj->pos.X));
    // asm 00005C27: 	LDI	*+AR0(OID),R0
    // asm 00005C28: 	ANDN	0FFh,R0
    // asm 00005C29: 	CMPI	100h,R0
    // asm 00005C2A: 	LDINE	600,R0
    // asm 00005C2B: 	LDIEQ	-600,R0
    // asm 00005C2C: 	FLOAT	R0
    offset = C3X_FROM_INT((obj->id & ~0xffu) == 0x100u ? -600 : 600);
    // asm 00005C2D: 	LDF	*+AR0(OPOSX),R1
    // asm 00005C2E: 	ADDF	R1,R0
    // asm 00005C2F: 	STF	R0,*+AR0(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), offset));
    // asm 00005C30: 	FIX	*+AR0(OPOSZ),R0
    // asm 00005C31: 	FLOAT	368,R1
    // asm 00005C32: 	STF	R1,*+AR0(OPOSZ)
    // asm 00005C33: 	STF	R1,*+AR0(OVELZ)
    obj->pos.Z = C3X_STF_INT(368);
    obj->vel_z = C3X_STF_INT(368);
    // asm 00005C34: 	LDI	*+AR0(OID),R0
    // asm 00005C35: 	AND	0FFh,R0
    object_id = obj->id & 0xffu;
    // asm 00005C36: 	CMPI	18h,R0
    // asm 00005C37: 	BNE	NOTFRNT
    // asm 00005C38: 	LDI	AR0,AR2
    // asm 00005C39: 	CALL	OBJ_PULL
    // asm 00005C3A: 	CALL	OBJ_INSERTP
    // asm 00005C3B: 	CALL	ADD_TO_DOOR_LIST
    if (object_id == 0x18) {
        OBJ_PULL(obj);
        OBJ_INSERTP(obj);
        ADD_TO_DOOR_LIST(obj);
        goto FTSL;
    }
    // asm 00005C3C: 	BU	FTSL
NOTFRNT:
    // asm 00005C3D: 	CMPI	15h,R0	;THE WHEEL?
    // asm 00005C3E: 	BNE	NOT366
    // asm 00005C3F: 	STI	AR0,@CT_WHEEL
    if (object_id == 0x15) {
        CT_WHEEL = obj;
        // asm 00005C40: 	LDI	AR0,AR2
        // asm 00005C41: 	CALL	OBJ_PULL
        // asm 00005C42: 	CALL	OBJ_INSERTP
        OBJ_PULL(obj);
        OBJ_INSERTP(obj);
        // asm 00005C43: 	LDI	*+AR0(OID),R0
        // asm 00005C44: 	ANDN	0FFh,R0
        // asm 00005C45: 	CMPI	100h,R0
        // asm 00005C46: 	LDIEQ	600,R0
        // asm 00005C47: 	LDINE	-600,R0
        // asm 00005C48: 	FLOAT	R0
        offset = C3X_FROM_INT((obj->id & ~0xffu) == 0x100u ? 600 : -600);
        // asm 00005C49: 	LDF	*+AR0(OPOSX),R1
        // asm 00005C4A: 	ADDF	R1,R0
        // asm 00005C4B: 	STF	R0,*+AR0(OPOSX)
        obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), offset));
        // asm 00005C4C: 	FLOAT	320,R0
        // asm 00005C4D: 	ADDF	*+AR0(OPOSY),R0
        // asm 00005C4E: 	STF	R0,*+AR0(OPOSY)
        obj->pos.Y = C3X_STF(C3X_ADD(C3X_FROM_INT(320), C3X_LDF(obj->pos.Y)));
        // asm 00005C4F: 	BU	FTSL
        goto FTSL;
    }
NOT366:
    // asm 00005C50: 	CMPI	14h,R0	;CURSOR?
    // asm 00005C51: 	BNE	NOTCURSOR
    // asm 00005C52: 	STI	AR0,@CT_CURSOR
    if (object_id == 0x14) {
        CT_CURSOR = obj;
        // asm 00005C53: 	LDI	AR0,AR2
        // asm 00005C54: 	CALL	OBJ_PULL
        // asm 00005C55: 	CALL	OBJ_INSERTP
        // asm 00005C56: 	CALL	ADD_TO_DOOR_LIST
        OBJ_PULL(obj);
        OBJ_INSERTP(obj);
        ADD_TO_DOOR_LIST(obj);
        goto FTSL;
    }
    // asm 00005C57: 	BU	FTSL
NOTCURSOR:
    // asm 00005C58: 	CMPI	10h,R0	;LEFT ENGINE
    // asm 00005C59: 	BNE	NOTLEN
    // asm 00005C5A: 	STI	AR0,@CT_LENG
    if (object_id == 0x10) {
        CT_LENG = obj;
        // asm 00005C5B: 	CALL	ADD_TO_DOOR_LIST
        ADD_TO_DOOR_LIST(obj);
        goto FTSL;
    }
    // asm 00005C5C: 	BU	FTSL
NOTLEN:
    // asm 00005C5D: 	CMPI	11h,R0	;RIGHT ENGINE
    // asm 00005C5E: 	BNE	NOTREN
    // asm 00005C5F: 	STI	AR0,@CT_RENG
    if (object_id == 0x11) {
        CT_RENG = obj;
        // asm 00005C60: 	CALL	ADD_TO_DOOR_LIST
        ADD_TO_DOOR_LIST(obj);
        goto FTSL;
    }
    // asm 00005C61: 	BU	FTSL
NOTREN:
    // asm 00005C62: 	CMPI	12h,R0	;AUTO BOX (LEFT)
    // asm 00005C63: 	BNE	NOTMAN
    // asm 00005C64: 	STI	AR0,@CT_MAN
    if (object_id == 0x12) {
        CT_MAN = obj;
        // asm 00005C65: 	LDI	AR0,AR2
        // asm 00005C66: 	CALL	OBJ_PULL
        // asm 00005C67: 	CALL	OBJ_INSERTP
        OBJ_PULL(obj);
        OBJ_INSERTP(obj);
        // asm 00005C68: 	FLOAT	-240,R1
        // asm 00005C69: 	LDF	*+AR0(OPOSX),R0
        // asm 00005C6A: 	ADDF	R1,R0
        // asm 00005C6B: 	STF	R0,*+AR0(OPOSX)
        obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), C3X_FROM_INT(-240)));
        // asm 00005C6C: 	LDF	*+AR0(OPOSY),R0
        // asm 00005C6D: 	FLOAT	400,R1
        // asm 00005C6E: 	ADDF	R1,R0
        // asm 00005C6F: 	STF	R0,*+AR0(OPOSY)
        obj->pos.Y = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Y), C3X_FROM_INT(400)));
        // asm 00005C70: 	LDF	9.0,R2
        // asm 00005C71: 	STF	R2,*+AR0(ORADZ)
        obj->rad.Z = C3X_STF_IMM(9.0);
        // asm 00005C72: 	LDI	AR0,AR2
        // asm 00005C73: 	ADDI	OMATRIX,AR2
        // asm 00005C74: 	CALL	FIND_ZMATRIX
        FIND_ZMATRIX(&obj->omatrix, C3X_LDF(obj->rad.Z));
        // asm 00005C75: 	LDI	*+AR0(OID),R0
        // asm 00005C76: 	ANDN	0FFh,R0
        // asm 00005C77: 	CMPI	100h,R0
        // asm 00005C78: 	LDIEQ	600,R0
        // asm 00005C79: 	LDINE	-600,R0
        // asm 00005C7A: 	FLOAT	R0
        offset = C3X_FROM_INT((obj->id & ~0xffu) == 0x100u ? 600 : -600);
        // asm 00005C7B: 	LDF	*+AR0(OPOSX),R1
        // asm 00005C7C: 	ADDF	R1,R0
        // asm 00005C7D: 	STF	R0,*+AR0(OPOSX)
        obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), offset));
        // asm 00005C7E: 	CALL	ADD_TO_DOOR_LIST
        ADD_TO_DOOR_LIST(obj);
        goto FTSL;
    }
    // asm 00005C7F: 	BU	FTSL
NOTMAN:
    // asm 00005C80: 	CMPI	13h,R0	;MANUAL BOX (RIGHT)
    // asm 00005C81: 	BNE	NOTAUTO
    // asm 00005C82: 	STI	AR0,@CT_AUTO
    if (object_id == 0x13) {
        CT_AUTO = obj;
        // asm 00005C83: 	LDI	AR0,AR2
        // asm 00005C84: 	CALL	OBJ_PULL
        // asm 00005C85: 	CALL	OBJ_INSERTP
        OBJ_PULL(obj);
        OBJ_INSERTP(obj);
        // asm 00005C86: 	FLOAT	240,R1
        // asm 00005C87: 	LDF	*+AR0(OPOSX),R0
        // asm 00005C88: 	ADDF	R1,R0
        // asm 00005C89: 	STF	R0,*+AR0(OPOSX)
        obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), C3X_FROM_INT(240)));
        // asm 00005C8A: 	LDF	*+AR0(OPOSY),R0
        // asm 00005C8B: 	FLOAT	400,R1
        // asm 00005C8C: 	ADDF	R1,R0
        // asm 00005C8D: 	STF	R0,*+AR0(OPOSY)
        obj->pos.Y = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.Y), C3X_FROM_INT(400)));
        // asm 00005C8E: 	LDF	-9.0,R2
        // asm 00005C8F: 	STF	R2,*+AR0(ORADZ)
        obj->rad.Z = C3X_STF_IMM(-9.0);
        // asm 00005C90: 	LDI	AR0,AR2
        // asm 00005C91: 	ADDI	OMATRIX,AR2
        // asm 00005C92: 	CALL	FIND_ZMATRIX
        FIND_ZMATRIX(&obj->omatrix, C3X_LDF(obj->rad.Z));
        // asm 00005C93: 	LDI	*+AR0(OID),R0
        // asm 00005C94: 	ANDN	0FFh,R0
        // asm 00005C95: 	CMPI	100h,R0
        // asm 00005C96: 	LDIEQ	600,R0
        // asm 00005C97: 	LDINE	-600,R0
        // asm 00005C98: 	FLOAT	R0
        offset = C3X_FROM_INT((obj->id & ~0xffu) == 0x100u ? 600 : -600);
        // asm 00005C99: 	LDF	*+AR0(OPOSX),R1
        // asm 00005C9A: 	ADDF	R1,R0
        // asm 00005C9B: 	STF	R0,*+AR0(OPOSX)
        obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), offset));
        // asm 00005C9C: 	CALL	ADD_TO_DOOR_LIST
        ADD_TO_DOOR_LIST(obj);
        goto FTSL;
    }
    // asm 00005C9D: 	BU	FTSL
NOTAUTO:
    // asm 00005C9E: 	CMPI	16h,R0	;PUSH TO CYCLE?
    // asm 00005C9F: 	BNE	NOTPTCYC
    // asm 00005CA0: 	STI	AR0,@CT_PUSHTOCYCLE
    if (object_id == 0x16) {
        CT_PUSHTOCYCLE = obj;
        // asm 00005CA1: 	LDI	AR0,AR2
        // asm 00005CA2: 	CALL	OBJ_PULL
        // asm 00005CA3: 	CALL	OBJ_INSERTP
        OBJ_PULL(obj);
        OBJ_INSERTP(obj);
        // asm 00005CA4: 	LDI	*+AR0(OID),R0
        // asm 00005CA5: 	ANDN	0FFh,R0
        // asm 00005CA6: 	CMPI	100h,R0
        // asm 00005CA7: 	LDIEQ	600,R0
        // asm 00005CA8: 	LDINE	-600,R0
        // asm 00005CA9: 	FLOAT	R0
        offset = C3X_FROM_INT((obj->id & ~0xffu) == 0x100u ? 600 : -600);
        // asm 00005CAA: 	LDF	*+AR0(OPOSX),R1
        // asm 00005CAB: 	ADDF	R1,R0
        // asm 00005CAC: 	STF	R0,*+AR0(OPOSX)
        obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), offset));
        // asm 00005CAD: 	FLOAT	320,R0
        // asm 00005CAE: 	ADDF	*+AR0(OPOSY),R0
        // asm 00005CAF: 	STF	R0,*+AR0(OPOSY)
        obj->pos.Y = C3X_STF(C3X_ADD(C3X_FROM_INT(320), C3X_LDF(obj->pos.Y)));
        // asm 00005CB0: 	LDI	AR0,AR4
        // asm 00005CB1: 	CREATE	CYCLE_PUSH,UTIL_C|CHOOSECAR_T
        ctx = NEW_PROC_CONTEXT();
        ctx->TRACKSEL_ANIMATION.obj = obj;
        CREATE(CYCLE_PUSH, UTIL_C | CHOOSECAR_T, ctx);
        goto FTSL;
    }
    // asm 00005CB4: 	BU	FTSL
NOTPTCYC:
    // asm 00005CB5: 	CMPI	17h,R0	;PUSH TO CYCLE?
    // asm 00005CB6: 	BNE	NOTWHELTOSEL
    // asm 00005CB7: 	STI	AR0,@CT_TURNTOSEL
    if (object_id == 0x17) {
        CT_TURNTOSEL = obj;
        // asm 00005CB8: 	LDI	AR0,AR2
        // asm 00005CB9: 	CALL	OBJ_PULL
        // asm 00005CBA: 	CALL	OBJ_INSERTP
        OBJ_PULL(obj);
        OBJ_INSERTP(obj);
        // asm 00005CBB: 	LDI	*+AR0(OID),R0
        // asm 00005CBC: 	ANDN	0FFh,R0
        // asm 00005CBD: 	CMPI	100h,R0
        // asm 00005CBE: 	LDIEQ	600,R0
        // asm 00005CBF: 	LDINE	-600,R0
        // asm 00005CC0: 	FLOAT	R0
        offset = C3X_FROM_INT((obj->id & ~0xffu) == 0x100u ? 600 : -600);
        // asm 00005CC1: 	LDF	*+AR0(OPOSX),R1
        // asm 00005CC2: 	ADDF	R1,R0
        // asm 00005CC3: 	STF	R0,*+AR0(OPOSX)
        obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(obj->pos.X), offset));
        // asm 00005CC4: 	FLOAT	320,R0
        // asm 00005CC5: 	ADDF	*+AR0(OPOSY),R0
        // asm 00005CC6: 	STF	R0,*+AR0(OPOSY)
        obj->pos.Y = C3X_STF(C3X_ADD(C3X_FROM_INT(320), C3X_LDF(obj->pos.Y)));
        // asm 00005CC7: 	LDI	AR0,AR4
        // asm 00005CC8: 	CREATE	TURNTO_SELECT,UTIL_C|CHOOSECAR_T
        ctx = NEW_PROC_CONTEXT();
        ctx->TRACKSEL_ANIMATION.obj = obj;
        CREATE(TURNTO_SELECT, UTIL_C | CHOOSECAR_T, ctx);
        goto FTSL;
    }
    // asm 00005CCB: 	BU	FTSL
NOTWHELTOSEL:
    // asm 00005CCC: 	CALL	ADD_TO_DOOR_LIST
    ADD_TO_DOOR_LIST(obj);
    // asm 00005CCD: 	BU	FTSL
    goto FTSL;
FTSLX:
    // asm 00005CCE: 	STI	R6,@TRANS_HEAD
    // asm 00005CCF: 	RETS
    return;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MOVE_PUSH_BOX(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 00005CD0: 	LDI	@CT_TURNTOSEL,AR6
    // asm 00005CD1: 	LDI	@CT_PUSHTOCYCLE,AR4
    // asm 00005CD2: 	LDI	10,AR5
    p->ctx.MOVE_PUSH_BOX.left_obj = CT_PUSHTOCYCLE;
    p->ctx.MOVE_PUSH_BOX.right_obj = CT_TURNTOSEL;
    p->ctx.MOVE_PUSH_BOX.loop_count = 10;
    // asm 00005CD3: MVPBL
MVPBL:
    // asm 00005CD3: 	LDF	*+AR4(OPOSX),R0
    // asm 00005CD4: 	ADDF	4,R0
    // asm 00005CD5: 	STF	R0,*+AR4(OPOSX)
    p->ctx.MOVE_PUSH_BOX.left_obj->pos.X = C3X_STF(C3X_ADD_IMM(C3X_LDF(p->ctx.MOVE_PUSH_BOX.left_obj->pos.X), 4));
    // asm 00005CD6: 	LDF	*+AR4(OPOSY),R0
    // asm 00005CD7: 	ADDF	3,R0
    // asm 00005CD8: 	STF	R0,*+AR4(OPOSY)
    p->ctx.MOVE_PUSH_BOX.left_obj->pos.Y = C3X_STF(C3X_ADD_IMM(C3X_LDF(p->ctx.MOVE_PUSH_BOX.left_obj->pos.Y), 3));
    // asm 00005CD9: 	LDF	*+AR6(OPOSX),R0
    // asm 00005CDA: 	ADDF	-4,R0
    // asm 00005CDB: 	STF	R0,*+AR6(OPOSX)
    p->ctx.MOVE_PUSH_BOX.right_obj->pos.X = C3X_STF(C3X_ADD_IMM(C3X_LDF(p->ctx.MOVE_PUSH_BOX.right_obj->pos.X), -4));
    // asm 00005CDC: 	LDF	*+AR6(OPOSY),R0
    // asm 00005CDD: 	ADDF	3,R0
    // asm 00005CDE: 	STF	R0,*+AR6(OPOSY)
    p->ctx.MOVE_PUSH_BOX.right_obj->pos.Y = C3X_STF(C3X_ADD_IMM(C3X_LDF(p->ctx.MOVE_PUSH_BOX.right_obj->pos.Y), 3));
    // asm 00005CDF: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00005CE1: 	DBU	AR5,MVPBL
    if (p->ctx.MOVE_PUSH_BOX.loop_count-- > 0)
        goto MVPBL;
    // asm 00005CE2: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: TRNTAB	.word	whel1,whel2,whel3,whel4,whel3,whel2,-1 */
static int TRNTAB[] = {
    whel1_ROM,
    whel2_ROM,
    whel3_ROM,
    whel4_ROM,
    whel3_ROM,
    whel2_ROM,
    -1,
};

void TURNTO_SELECT(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 00005CEB: 	LDI	@TRNTABI,AR5
    p->ctx.TRACKSEL_ANIMATION.script_index = 0;
TURNLP:
    // asm 00005CEC: LDI	*AR5++,R0
    // asm 00005CED: 	CMPI	-1,R0
    // asm 00005CEE: 	BEQ	TURNTO_SELECT
    if (TRNTAB[p->ctx.TRACKSEL_ANIMATION.script_index] < 0) {
        p->ctx.TRACKSEL_ANIMATION.script_index = 0;
    }
    // asm 00005CEF: 	STI	R0,*+AR4(OROMDATA)
    p->ctx.TRACKSEL_ANIMATION.obj->romdata = ROM_PTR(
        TRNTAB[p->ctx.TRACKSEL_ANIMATION.script_index++]);
    // asm 00005CF0: 	SLEEP	5
    SLEEP(5, 1);
    // asm 00005CF2: 	BU	TURNLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    goto TURNLP;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CYCTAB	.word	trm3,trm2,trm1,trm2,trm3,trm4,-1 */
static int CYCTAB[] = {
    trm3_ROM,
    trm2_ROM,
    trm1_ROM,
    trm2_ROM,
    trm3_ROM,
    trm4_ROM,
    -1,
};

void CYCLE_PUSH(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        // MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

    // asm 00005CFB: 	LDI	@CYCTABI,AR5
    p->ctx.TRACKSEL_ANIMATION.script_index = 0;
    // asm 00005CFC: 	SLEEP	16
    SLEEP(16, 1);
CYCLP:
    // asm 00005CFE: 	LDI	*AR5++,R0
    // asm 00005CFF: 	CMPI	-1,R0
    // asm 00005D00: 	BEQ	CYCLE_PUSH
    if (CYCTAB[p->ctx.TRACKSEL_ANIMATION.script_index] < 0) {
        p->ctx.TRACKSEL_ANIMATION.script_index = 0;
    }
    // asm 00005D01: 	STI	R0,*+AR4(OROMDATA)
    p->ctx.TRACKSEL_ANIMATION.obj->romdata = ROM_PTR(
        CYCTAB[p->ctx.TRACKSEL_ANIMATION.script_index++]);
    // asm 00005D02: 	LDI	@NFRAMES,R1
    // asm 00005D03: 	LDI	6,R0
    // asm 00005D04: 	CALL	DIV_I30
    // asm 00005D05: 	LDI	R0,AR2
    // asm 00005D06: 	CALL	PRC_SLEEP
    SLEEP(6 / NFRAMES, 2);
    // asm 00005D07: 	BU	CYCLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    goto CYCLP;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *PARAMETERS
 *	AR0	DOOR OBJECT
 *
 */
void ADD_TO_DOOR_LIST(OBJ* obj /*AR0*/) {
    // asm 00005D08: 	LDI	@DOORLIST,R0
    // asm 00005D09: 	STI	R0,*+AR0(OLINK2)
    obj->link2 = (uintptr_t)DOORLIST;
    // asm 00005D0A: 	STI	AR0,@DOORLIST
    DOORLIST = obj;
    // asm 00005D0B: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *
 */
void ENGINE_COLOR_INIT(void) {
    const tPAL* source_palette;

    // asm 00005D0C: 	LDI	@EPALR,AR0
    // asm 00005D0D: 	LDI	trmeng1_p,AR3
    // asm 00005D0E: 	ADDI	@PALROMI,AR3
    // asm 00005D0F: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    source_palette = PALROMI[trmeng1_p]; // NOW HOLDS RAM LOCATION
    // asm 00005D10: 	LDI	*AR3++,R0
    // asm 00005D11: 	STI	R0,*AR0++
    // asm 00005D12: 	LDI	R0,AR2
    // asm 00005D13: 	AND	0FFFh,AR2
    // asm 00005D14: 	RS	1,AR2
    // asm 00005D15: 	SUBI	1,AR2
L342:
    // asm 00005D16: LDI	*AR3++,R0
    // asm 00005D17: 	STI	R0,*AR0++
    // asm 00005D18: 	DBU	AR2,L342
    EPALR->flags_and_count = source_palette->flags_and_count;
    for (int i = 0; i < trans_palette_word_count(source_palette->flags_and_count); i++) {
        EPALR->data[i] = source_palette->data[i];
    }
    // asm 00005D19: 	LDI	@EPALR,AR2
    // asm 00005D1A: 	CALL	PAL_ALLOC_RAW
    PAL_ALLOC_RAW(EPALR);
    // asm 00005D1B: 	LDI	@EPALL,AR0
    // asm 00005D1C: 	LDI	trmeng1_p,AR3
    // asm 00005D1D: 	ADDI	@PALROMI,AR3
    // asm 00005D1E: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm 00005D1F: 	LDI	*AR3++,R0
    // asm 00005D20: 	STI	R0,*AR0++
    // asm 00005D21: 	LDI	R0,AR2
    // asm 00005D22: 	AND	0FFFh,AR2
    // asm 00005D23: 	RS	1,AR2
    // asm 00005D24: 	SUBI	1,AR2
L343:
    // asm 00005D25: LDI	*AR3++,R0
    // asm 00005D26: 	STI	R0,*AR0++
    // asm 00005D27: 	DBU	AR2,L343
    EPALL->flags_and_count = source_palette->flags_and_count;
    for (int i = 0; i < trans_palette_word_count(source_palette->flags_and_count); i++) {
        EPALL->data[i] = source_palette->data[i];
    }
    // asm 00005D28: 	LDI	@EPALL,AR2
    // asm 00005D29: 	CALL	PAL_ALLOC_RAW
    PAL_ALLOC_RAW(EPALL);
    // asm 00005D2A: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *PARAMETERS
 *	R0	PERCENTAGE OF LEFT BRIGHTNESS
 *
 *
 */
void ENGINE_COLOR(c3x_reg_t left_brightness /*R0*/) {
    const tPAL* true_palette;
    const tPAL* chrome_palette;
    c3x_reg_t interpolation_amount;
    c3x_reg_t clamped_brightness;
    int palette_code;

    // asm 00005D2B: 	PUSH	R5
    // asm 00005D2C: 	PUSHFL	R6
    // asm 00005D2E: 	PUSHFL	R7
    // asm 00005D30: 	LDF	R0,R7		;PERCENTAGE OF LEFT
    interpolation_amount = left_brightness; // PERCENTAGE OF LEFT
    clamped_brightness = left_brightness;
    // asm 00005D31: 	CMPF	0.08,R0
    // asm 00005D32: 	LDFLT	0,R0
    if (C3X_LT_IMM(clamped_brightness, 0.08))
        clamped_brightness = C3X_IMM_F32(0);
    // asm 00005D33: 	CMPF	0.92,R0
    // asm 00005D34: 	LDFGT	1.0,R0
    if (C3X_GT_IMM(clamped_brightness, 0.92))
        clamped_brightness = C3X_IMM_F32(1.0);
    (void)clamped_brightness;
    // asm 00005D35: 	LDI	@EPALL,AR0
    // asm 00005D36: 	LDI	trmeng1_p,AR3
    // asm 00005D37: 	ADDI	@PALROMI,AR3
    // asm 00005D38: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm 00005D39: 	LDL	trmeng1_chrome,AR2
    true_palette = PALROMI[trmeng1_p];
    chrome_palette = (const tPAL*)ROM_PTR(trmeng1_chrome_ROM);
    // asm 00005D3A: 	LDI	*AR3++,R0
    // asm 00005D3B: 	LDI	*AR2++,R0
    // asm 00005D3C: 	ANDN	0FFFh,R0
    // asm 00005D3D: 	ADDI	64,R0
    // asm 00005D3E: 	STI	R0,*AR0++
    EPALL->flags_and_count = (chrome_palette->flags_and_count & ~0x0fff) + 64;
    // asm 00005D3F: 	LDI	31,AR5
L894:
    // asm 00005D40: L894
    // asm 00005D40: 	LDI	*AR3++,R4	;get src2 (TRUE)
    // asm 00005D41: 	LDI	R4,R5
    // asm 00005D42: 	AND	01Fh,R5
    // asm 00005D43: 	FLOAT	R5
    // asm 00005D44: 	LDI	*AR2++,R0	;get src2 (TPAL)
    // asm 00005D45: 	LDI	R0,R1
    // asm 00005D46: 	AND	01Fh,R1
    // asm 00005D47: 	FLOAT	R1
    // asm 00005D48: 	SUBF	R5,R1,R6	;TPAL-TRUE->C
    // asm 00005D49: 	MPYF	R7,R6		;C*UNIT
    // asm 00005D4A: 	ADDF	R5,R6,R1	;C+A->D
    // asm 00005D4B: 	FIX	R1
    // asm 00005D4C: 	LDI	R4,R5		;TRUE
    // asm 00005D4D: 	RS	5,R5
    // asm 00005D4E: 	AND	01Fh,R5
    // asm 00005D4F: 	FLOAT	R5
    // asm 00005D50: 	LDI	R0,R2		;TPAL
    // asm 00005D51: 	RS	5,R2
    // asm 00005D52: 	AND	01Fh,R2
    // asm 00005D53: 	FLOAT	R2
    // asm 00005D54: 	SUBF	R5,R2,R6	;TRUE-TPAL->C
    // asm 00005D55: 	MPYF	R7,R6
    // asm 00005D56: 	ADDF	R5,R6,R2
    // asm 00005D57: 	FIX	R2
    // asm 00005D58: 	LS	5,R2
    // asm 00005D59: 	OR	R2,R1
    // asm 00005D5A: 	LDI	R4,R5		;TRUE
    // asm 00005D5B: 	RS	10,R5
    // asm 00005D5C: 	AND	01Fh,R5
    // asm 00005D5D: 	FLOAT	R5
    // asm 00005D5E: 	LDI	R0,R2		;TPAL
    // asm 00005D5F: 	RS	10,R2
    // asm 00005D60: 	AND	01Fh,R2
    // asm 00005D61: 	FLOAT	R2
    // asm 00005D62: 	SUBF	R5,R2,R6	;TRUE-TPAL
    // asm 00005D63: 	MPYF	R7,R6
    // asm 00005D64: 	ADDF	R5,R6,R2
    // asm 00005D65: 	FIX	R2
    // asm 00005D66: 	LS	10,R2
    // asm 00005D67: 	OR	R2,R1
    // asm 00005D68: 	PUSH	R1
    // 	;----second word----------
    // asm 00005D69: 	RS	16,R4		;get src1(B)
    // asm 00005D6A: 	RS	16,R0		;get src2(B)
    // asm 00005D6B: 	LDI	R4,R5		;TRUE
    // asm 00005D6C: 	AND	01Fh,R5
    // asm 00005D6D: 	FLOAT	R5
    // asm 00005D6E: 	LDI	R0,R1		;TPAL
    // asm 00005D6F: 	AND	01Fh,R1
    // asm 00005D70: 	FLOAT	R1
    // asm 00005D71: 	SUBF	R5,R1,R6	;TPAL-TRUE
    // asm 00005D72: 	MPYF	R7,R6
    // asm 00005D73: 	ADDF	R5,R6,R1
    // asm 00005D74: 	FIX	R1
    // asm 00005D75: 	LDI	R4,R5		;--
    // asm 00005D76: 	RS	5,R5
    // asm 00005D77: 	AND	01Fh,R5
    // asm 00005D78: 	FLOAT	R5
    // asm 00005D79: 	LDI	R0,R2
    // asm 00005D7A: 	RS	5,R2
    // asm 00005D7B: 	AND	01Fh,R2
    // asm 00005D7C: 	FLOAT	R2
    // asm 00005D7D: 	SUBF	R5,R2,R6
    // asm 00005D7E: 	MPYF	R7,R6
    // asm 00005D7F: 	ADDF	R5,R6,R2
    // asm 00005D80: 	FIX	R2
    // asm 00005D81: 	LS	5,R2
    // asm 00005D82: 	OR	R2,R1
    // asm 00005D83: 	LDI	R4,R5		;--
    // asm 00005D84: 	RS	10,R5
    // asm 00005D85: 	AND	01Fh,R5
    // asm 00005D86: 	FLOAT	R5
    // asm 00005D87: 	LDI	R0,R2
    // asm 00005D88: 	RS	10,R2
    // asm 00005D89: 	AND	01Fh,R2
    // asm 00005D8A: 	FLOAT	R2
    // asm 00005D8B: 	SUBF	R5,R2,R6
    // asm 00005D8C: 	MPYF	R7,R6
    // asm 00005D8D: 	ADDF	R5,R6,R2
    // asm 00005D8E: 	FIX	R2
    // asm 00005D8F: 	LS	10,R2
    // asm 00005D90: 	OR	R2,R1
    // asm 00005D91: 	POP	R4
    // asm 00005D92: 	LS	16,R1
    // asm 00005D93: 	OR	R4,R1
    // asm 00005D94: 	STI	R1,*AR0++
    for (int i = 0; i < 32; i++) {
        EPALL->data[i] = interpolate_trans_palette_word(true_palette->data[i], chrome_palette->data[i], interpolation_amount);
    }
    // asm 00005D95: 	DBU	AR5,L894
    // asm 00005D96: 	LDF	1.0,R0
    // asm 00005D97: 	SUBF	R7,R0,R7
    interpolation_amount = C3X_RSUB_IMM(1.0, interpolation_amount);
    // asm 00005D98: 	LDI	@EPALL,AR2
    // asm 00005D99: 	CALL	PAL_FIND_RAW
    palette_code = PAL_FIND_RAW(EPALL);
    // asm 00005D9A: 	LDI	*AR2++,R3
    // asm 00005D9B: 	LDI	R0,R2
    // asm 00005D9C: 	CALL	PAL_SET
    PAL_SET(EPALL->data, (u32)palette_code, (u32)EPALL->flags_and_count);
    // *
    // *
    // *
    // *
    // *
    // asm 00005D9D: 	LDI	@EPALR,AR0
    // asm 00005D9E: 	LDI	trmeng1_p,AR3
    // asm 00005D9F: 	ADDI	@PALROMI,AR3
    // asm 00005DA0: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm 00005DA1: 	LDL	trmeng1_chrome,AR2
    // asm 00005DA2: 	LDI	*AR3++,R0
    // asm 00005DA3: 	LDI	*AR2++,R0
    // asm 00005DA4: 	ANDN	0FFFh,R0
    // asm 00005DA5: 	ADDI	64,R0
    // asm 00005DA6: 	STI	R0,*AR0++
    EPALR->flags_and_count = (chrome_palette->flags_and_count & ~0x0fff) + 64;
    // asm 00005DA7: 	LDI	31,AR5
L894D:
    // asm 00005DA8: L894D
    // asm 00005DA8: 	LDI	*AR3++,R4	;get src2 (TRUE)
    // asm 00005DA9: 	LDI	R4,R5
    // asm 00005DAA: 	AND	01Fh,R5
    // asm 00005DAB: 	FLOAT	R5
    // asm 00005DAC: 	LDI	*AR2++,R0	;get src2 (TPAL)
    // asm 00005DAD: 	LDI	R0,R1
    // asm 00005DAE: 	AND	01Fh,R1
    // asm 00005DAF: 	FLOAT	R1
    // asm 00005DB0: 	SUBF	R5,R1,R6	;TPAL-TRUE->C
    // asm 00005DB1: 	MPYF	R7,R6		;C*UNIT
    // asm 00005DB2: 	ADDF	R5,R6,R1	;C+A->D
    // asm 00005DB3: 	FIX	R1
    // asm 00005DB4: 	LDI	R4,R5		;TRUE
    // asm 00005DB5: 	RS	5,R5
    // asm 00005DB6: 	AND	01Fh,R5
    // asm 00005DB7: 	FLOAT	R5
    // asm 00005DB8: 	LDI	R0,R2		;TPAL
    // asm 00005DB9: 	RS	5,R2
    // asm 00005DBA: 	AND	01Fh,R2
    // asm 00005DBB: 	FLOAT	R2
    // asm 00005DBC: 	SUBF	R5,R2,R6	;TRUE-TPAL->C
    // asm 00005DBD: 	MPYF	R7,R6
    // asm 00005DBE: 	ADDF	R5,R6,R2
    // asm 00005DBF: 	FIX	R2
    // asm 00005DC0: 	LS	5,R2
    // asm 00005DC1: 	OR	R2,R1
    // asm 00005DC2: 	LDI	R4,R5		;TRUE
    // asm 00005DC3: 	RS	10,R5
    // asm 00005DC4: 	AND	01Fh,R5
    // asm 00005DC5: 	FLOAT	R5
    // asm 00005DC6: 	LDI	R0,R2		;TPAL
    // asm 00005DC7: 	RS	10,R2
    // asm 00005DC8: 	AND	01Fh,R2
    // asm 00005DC9: 	FLOAT	R2
    // asm 00005DCA: 	SUBF	R5,R2,R6	;TRUE-TPAL
    // asm 00005DCB: 	MPYF	R7,R6
    // asm 00005DCC: 	ADDF	R5,R6,R2
    // asm 00005DCD: 	FIX	R2
    // asm 00005DCE: 	LS	10,R2
    // asm 00005DCF: 	OR	R2,R1
    // asm 00005DD0: 	PUSH	R1
    // 	;----second word----------
    // asm 00005DD1: 	RS	16,R4		;get src1(B)
    // asm 00005DD2: 	RS	16,R0		;get src2(B)
    // asm 00005DD3: 	LDI	R4,R5		;TRUE
    // asm 00005DD4: 	AND	01Fh,R5
    // asm 00005DD5: 	FLOAT	R5
    // asm 00005DD6: 	LDI	R0,R1		;TPAL
    // asm 00005DD7: 	AND	01Fh,R1
    // asm 00005DD8: 	FLOAT	R1
    // asm 00005DD9: 	SUBF	R5,R1,R6	;TPAL-TRUE
    // asm 00005DDA: 	MPYF	R7,R6
    // asm 00005DDB: 	ADDF	R5,R6,R1
    // asm 00005DDC: 	FIX	R1
    // asm 00005DDD: 	LDI	R4,R5		;--
    // asm 00005DDE: 	RS	5,R5
    // asm 00005DDF: 	AND	01Fh,R5
    // asm 00005DE0: 	FLOAT	R5
    // asm 00005DE1: 	LDI	R0,R2
    // asm 00005DE2: 	RS	5,R2
    // asm 00005DE3: 	AND	01Fh,R2
    // asm 00005DE4: 	FLOAT	R2
    // asm 00005DE5: 	SUBF	R5,R2,R6
    // asm 00005DE6: 	MPYF	R7,R6
    // asm 00005DE7: 	ADDF	R5,R6,R2
    // asm 00005DE8: 	FIX	R2
    // asm 00005DE9: 	LS	5,R2
    // asm 00005DEA: 	OR	R2,R1
    // asm 00005DEB: 	LDI	R4,R5		;--
    // asm 00005DEC: 	RS	10,R5
    // asm 00005DED: 	AND	01Fh,R5
    // asm 00005DEE: 	FLOAT	R5
    // asm 00005DEF: 	LDI	R0,R2
    // asm 00005DF0: 	RS	10,R2
    // asm 00005DF1: 	AND	01Fh,R2
    // asm 00005DF2: 	FLOAT	R2
    // asm 00005DF3: 	SUBF	R5,R2,R6
    // asm 00005DF4: 	MPYF	R7,R6
    // asm 00005DF5: 	ADDF	R5,R6,R2
    // asm 00005DF6: 	FIX	R2
    // asm 00005DF7: 	LS	10,R2
    // asm 00005DF8: 	OR	R2,R1
    // asm 00005DF9: 	POP	R4
    // asm 00005DFA: 	LS	16,R1
    // asm 00005DFB: 	OR	R4,R1
    // asm 00005DFC: 	STI	R1,*AR0++
    for (int i = 0; i < 32; i++) {
        EPALR->data[i] = interpolate_trans_palette_word(true_palette->data[i], chrome_palette->data[i], interpolation_amount);
    }
    // asm 00005DFD: 	DBU	AR5,L894D
    // asm 00005DFE: 	LDI	@EPALR,AR2
    // asm 00005DFF: 	CALL	PAL_FIND_RAW
    palette_code = PAL_FIND_RAW(EPALR);
    // asm 00005E00: 	LDI	*AR2++,R3
    // asm 00005E01: 	LDI	R0,R2
    // asm 00005E02: 	CALL	PAL_SET
    PAL_SET(EPALR->data, (u32)palette_code, (u32)EPALR->flags_and_count);
    // asm 00005E03: 	POPFL	R7
    // asm 00005E05: 	POPFL	R6
    // asm 00005E07: 	POP	R5
    // asm 00005E08: 	RETS
}

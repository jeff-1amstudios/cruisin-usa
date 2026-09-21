
#include "effects.h"
#include "../core/machine.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "sysid.h"
#include "vunit.h"

/*
 * Source module: asm/EFFECTS.ASM
 */

void SCREENWIPE_OPEN(void);
void SCREENWIPE_CLOSE(void);
void SCREENWIPE_OPEN_PRC_FAST(PROC* p);
void SCREENWIPE_OPEN_PRC(PROC* p);
void SCREENWIPE_CLOSE_PRC(PROC* p);
static void SCREENWIPE_OPEN_PRC_SHARED(PROC* p);
static void GET_EFFECTS_OBJS(PROC* p);
static void MEMCPY(const c3x_f32_t* source, c3x_f32_t* dest, int length_minus_one);

extern MATRIX _MATRIXA;

/*
 *----------------------------------------------------------------------------
 *ADVANCED FEATURES
 *
 *it would be best to do open and close via the same process
 *also, sideways wipes  //  or \\  up, down, left or right can be specified
 *via on word ala:
 *
 *	SCW_OPEN	.set	0
 *	SCW_CLOSE	.set	1
 *	SCW_L		.set	10h	;these specifiy the
 *	SCW_R		.set	20h	;single panels
 *	SCW_T		.set	40h	;
 *	SCW_B		.set	80h	;
 *
 *for convience:
 *	SCW_OPEN_H	.set	030h
 *	SCW_CLOSE_H	.set	031h
 *	SCW_OPEN_V	.set	0C0h
 *	SCW_CLOSE_V	.set	0C1h
 *
 *also can be added:	(S = straight instead of diagonal)
 *
 *	SCW_LS		.set	12h	;these specifiy the
 *	SCW_RS		.set	22h	;single panels
 *	SCW_TS		.set	42h	;
 *	SCW_BS		.set	82h	;
 *
 *
 *these parameters would be encoded into AR2 when calling SCREEN_WIPE
 *and from there it would be moved to AR6, and checked each tik.
 *
 */

/* asm: MONKEYOBJRVI	.word	mnkrv */
#define MONKEYOBJRVI (*crusn_machine_rom_addr(0x0000B09B))
/* asm: MONKEYOBJLVI	.word	mnklv */
#define MONKEYOBJLVI (*crusn_machine_rom_addr(0x0000B09C))
/* asm: MONKEYOBJUHI	.word	mnkuh */
#define MONKEYOBJUHI (*crusn_machine_rom_addr(0x0000B09D))
/* asm: MONKEYOBJLHI	.word	mnklh */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
#define MONKEYOBJLHI (*crusn_machine_rom_addr(0x0000B09E))
/* asm: SCREENWIPE_DONE	.bss	SCREENWIPE_DONE,1 */
int SCREENWIPE_DONE;

// *----------------------------------------------------------------------------
void SCREENWIPE_OPEN(void) {
    MAME_ASSERT_FUNCTION_ENTRY();
    // asm 0000B09F: 	CLRI	R0
    SCREENWIPE_DONE = 0;
    // asm 0000B0A0: 	STPI	R0,@SCREENWIPE_DONE
    // asm 0000B0A1: 	CREATEC	SCREENWIPE_OPEN_PRC,UTIL_C|MONKEY_T
    CREATEC(CURRENT_PROC, SCREENWIPE_OPEN_PRC, UTIL_C | MONKEY_T, NEW_PROC_CONTEXT());
    // asm 0000B0A4: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SCREENWIPE_CLOSE(void) {
    MAME_ASSERT_FUNCTION_ENTRY();
    // asm 0000B0A5: 	CLRI	R0
    SCREENWIPE_DONE = 0;
    // asm 0000B0A6: 	STPI	R0,@SCREENWIPE_DONE
    // asm 0000B0A7: 	CREATE	SCREENWIPE_CLOSE_PRC,UTIL_C|MONKEY_T
    CREATE(SCREENWIPE_CLOSE_PRC, UTIL_C | MONKEY_T, NEW_PROC_CONTEXT());
    // asm 0000B0AA: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SCREENWIPE_OPEN_PRC_FAST(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 0000B0AB: 	LONGROUT
    // asm: 	LDF	1.9,R0
    p->ctx.SCREENWIPE.entry_factor = C3X_IMM_F32(1.9);
    // asm: 	BR	L55
    PROC_CONTINUE(SCREENWIPE_OPEN_PRC_SHARED, 1);
    return;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

void SCREENWIPE_OPEN_PRC(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 0000B0AC: 	LONGROUT
    // asm: 	LDF	1.1,R0
    p->ctx.SCREENWIPE.entry_factor = C3X_IMM_F32(1.1);
    PROC_CONTINUE(SCREENWIPE_OPEN_PRC_SHARED, 1);
    return;
}

static void SCREENWIPE_OPEN_PRC_SHARED(PROC* p) {
    OBJ* obj;
    VECTOR source;

    switch (PROC_RESUME_STATE) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

    // asm: L55
    // asm: STF	R0,*+AR7(PDATA+7)
    p->ctx.SCREENWIPE.factor = C3X_STF(p->ctx.SCREENWIPE.entry_factor);
    // asm: 	CALL	GET_EFFECTS_OBJS
    GET_EFFECTS_OBJS(p);
    // asm: 	SLEEP	19
    SLEEP(19, 1);
    // asm: 	LDI	64,AR5
    p->ctx.SCREENWIPE.loop_count = 64;
    // asm: 	CLRF	R6
    p->ctx.SCREENWIPE.angle = C3X_FROM_INT(0);
    // asm: 	LDF	2,R7
    p->ctx.SCREENWIPE.offset = C3X_IMM_F32(2);
    // asm: SWCL1
SWCL1:
    // asm: 	MPYF	*+AR7(PDATA+7),R7
    p->ctx.SCREENWIPE.offset = C3X_MUL(
        C3X_LDF(p->ctx.SCREENWIPE.factor), p->ctx.SCREENWIPE.offset);
    // ;	MPYF	1.1,R7
    // asm: 	ADDF	0.2,R6
    p->ctx.SCREENWIPE.angle = C3X_ADD_IMM(p->ctx.SCREENWIPE.angle, 0.2);
    // asm: 	LDF	R6,R2
    // asm: 	LDPI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&MATRIXAI, p->ctx.SCREENWIPE.angle);
    // asm: 	LDI	*+AR7(PDATA),AR0
    obj = p->ctx.SCREENWIPE.objects[0];
    // asm: 	LDF	*+AR0(OUSR1),R0
    // asm: 	ADDF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR1)
    obj->usr1_as_float = C3X_STF(C3X_ADD(
        C3X_LDF(obj->usr1_as_float), p->ctx.SCREENWIPE.offset));
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    source.X = obj->usr1_as_float;
    source.Y = obj->usr2_as_float;
    source.Z = obj->usr3_as_float;
    MATRIX_MUL(&source, &MATRIXAI, &obj->pos);
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    MEMCPY((const c3x_f32_t*)&MATRIXAI, (c3x_f32_t*)&obj->omatrix, 8);
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    obj = p->ctx.SCREENWIPE.objects[1];
    // asm: 	LDF	*+AR0(OUSR1),R0
    // asm: 	SUBF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR1)
    obj->usr1_as_float = C3X_STF(C3X_SUB(
        C3X_LDF(obj->usr1_as_float), p->ctx.SCREENWIPE.offset));
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDPI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    source.X = obj->usr1_as_float;
    source.Y = obj->usr2_as_float;
    source.Z = obj->usr3_as_float;
    MATRIX_MUL(&source, &MATRIXAI, &obj->pos);
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    MEMCPY((const c3x_f32_t*)&MATRIXAI, (c3x_f32_t*)&obj->omatrix, 8);
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    obj = p->ctx.SCREENWIPE.objects[2];
    // asm: 	LDF	*+AR0(OUSR2),R0
    // asm: 	SUBF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR2)
    obj->usr2_as_float = C3X_STF(C3X_SUB(
        C3X_LDF(obj->usr2_as_float), p->ctx.SCREENWIPE.offset));
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    source.X = obj->usr1_as_float;
    source.Y = obj->usr2_as_float;
    source.Z = obj->usr3_as_float;
    MATRIX_MUL(&source, &MATRIXAI, &obj->pos);
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    MEMCPY((const c3x_f32_t*)&MATRIXAI, (c3x_f32_t*)&obj->omatrix, 8);
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    obj = p->ctx.SCREENWIPE.objects[3];
    // asm: 	LDF	*+AR0(OUSR2),R0
    // asm: 	ADDF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR2)
    obj->usr2_as_float = C3X_STF(C3X_ADD(
        C3X_LDF(obj->usr2_as_float), p->ctx.SCREENWIPE.offset));
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    source.X = obj->usr1_as_float;
    source.Y = obj->usr2_as_float;
    source.Z = obj->usr3_as_float;
    MATRIX_MUL(&source, &MATRIXAI, &obj->pos);
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    MEMCPY((const c3x_f32_t*)&MATRIXAI, (c3x_f32_t*)&obj->omatrix, 8);
    // asm: 	SLEEP	1
    SLEEP(1, 2);
    // asm: 	DBU	AR5,SWCL1
    p->ctx.SCREENWIPE.loop_count -= 1;
    if (p->ctx.SCREENWIPE.loop_count >= 0) {
        goto SWCL1;
    }
    // asm: 	LDI	*+AR7(PDATA),AR2
    // asm: 	CALL	OBJ_DELETE
    OBJ_DELETE(p->ctx.SCREENWIPE.objects[0]);
    // asm: 	LDI	*+AR7(PDATA+1),AR2
    // asm: 	CALL	OBJ_DELETE
    OBJ_DELETE(p->ctx.SCREENWIPE.objects[1]);
    // asm: 	LDI	*+AR7(PDATA+2),AR2
    // asm: 	CALL	OBJ_DELETE
    OBJ_DELETE(p->ctx.SCREENWIPE.objects[2]);
    // asm: 	LDI	*+AR7(PDATA+3),AR2
    // asm: 	CALL	OBJ_DELETE
    OBJ_DELETE(p->ctx.SCREENWIPE.objects[3]);
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@SCREENWIPE_DONE
    SCREENWIPE_DONE = 1;
    // asm: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void SCREENWIPE_CLOSE_PRC(PROC* p) {
    static const c3x_f32_t initial_offset = C3X_F32_INIT(28530.66038);
    OBJ* obj;
    VECTOR source;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 0000B0AD: 	LONGROUT
    // asm: 	CALL	GET_EFFECTS_OBJS
    GET_EFFECTS_OBJS(p);
    // asm: 	LDI	100,AR5
    p->ctx.SCREENWIPE.loop_count = 100;
    // asm: 	LDF	12.8,R6
    p->ctx.SCREENWIPE.angle = C3X_IMM_F32(12.8);
    // asm: 	LDLFH	28530.66038,R7
    p->ctx.SCREENWIPE.offset = C3X_LDF(initial_offset);
    // asm: SWCL1C
SWCL1C:
    // asm: 	MPYF	0.91,R7
    p->ctx.SCREENWIPE.offset = C3X_MUL_IMM(p->ctx.SCREENWIPE.offset, 0.91);
    // asm: 	SUBF	0.2,R6
    p->ctx.SCREENWIPE.angle = C3X_SUB_IMM(p->ctx.SCREENWIPE.angle, 0.2);
    // asm: 	LDF	R6,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_ZMATRIX
    FIND_ZMATRIX(&MATRIXAI, p->ctx.SCREENWIPE.angle);
    // asm: 	LDI	*+AR7(PDATA),AR0
    obj = p->ctx.SCREENWIPE.objects[0];
    // asm: 	STF	R7,*+AR0(OUSR1)
    obj->usr1_as_float = C3X_STF(p->ctx.SCREENWIPE.offset);
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    source.X = obj->usr1_as_float;
    source.Y = obj->usr2_as_float;
    source.Z = obj->usr3_as_float;
    MATRIX_MUL(&source, &MATRIXAI, &obj->pos);
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    MEMCPY((const c3x_f32_t*)&MATRIXAI, (c3x_f32_t*)&obj->omatrix, 8);
    // asm: 	LDI	*+AR7(PDATA+1),AR0
    obj = p->ctx.SCREENWIPE.objects[1];
    // asm: 	NEGF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR1)
    obj->usr1_as_float = C3X_STF(C3X_NEG(p->ctx.SCREENWIPE.offset));
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    source.X = obj->usr1_as_float;
    source.Y = obj->usr2_as_float;
    source.Z = obj->usr3_as_float;
    MATRIX_MUL(&source, &MATRIXAI, &obj->pos);
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    MEMCPY((const c3x_f32_t*)&MATRIXAI, (c3x_f32_t*)&obj->omatrix, 8);
    // asm: 	LDI	*+AR7(PDATA+2),AR0
    obj = p->ctx.SCREENWIPE.objects[2];
    // asm: 	NEGF	R7,R0
    // asm: 	STF	R0,*+AR0(OUSR2)
    obj->usr2_as_float = C3X_STF(C3X_NEG(p->ctx.SCREENWIPE.offset));
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    source.X = obj->usr1_as_float;
    source.Y = obj->usr2_as_float;
    source.Z = obj->usr3_as_float;
    MATRIX_MUL(&source, &MATRIXAI, &obj->pos);
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    MEMCPY((const c3x_f32_t*)&MATRIXAI, (c3x_f32_t*)&obj->omatrix, 8);
    // asm: 	LDI	*+AR7(PDATA+3),AR0
    obj = p->ctx.SCREENWIPE.objects[3];
    // asm: 	STF	R7,*+AR0(OUSR2)
    obj->usr2_as_float = C3X_STF(p->ctx.SCREENWIPE.offset);
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OUSR1,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    source.X = obj->usr1_as_float;
    source.Y = obj->usr2_as_float;
    source.Z = obj->usr3_as_float;
    MATRIX_MUL(&source, &MATRIXAI, &obj->pos);
    // asm: 	LDI	AR0,AR1
    // asm: 	ADDI	OMATRIX,AR1
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	8,RC
    // asm: 	CALL	MEMCPY
    MEMCPY((const c3x_f32_t*)&MATRIXAI, (c3x_f32_t*)&obj->omatrix, 8);
    // asm: 	SLEEP	1
    SLEEP(1, 1);
    // asm: 	DBU	AR5,SWCL1C
    p->ctx.SCREENWIPE.loop_count -= 1;
    if (p->ctx.SCREENWIPE.loop_count >= 0) {
        goto SWCL1C;
    }
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@SCREENWIPE_DONE
    SCREENWIPE_DONE = 1;
    // asm: 	DIE
    DIE();
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void GET_EFFECTS_OBJS(PROC* p) {
    OBJ* obj;
    word_addr_t romdata;

    // asm 0000B0AE: 	LONGROUT
    MAME_ASSERT_FUNCTION_ENTRY();
    // asm: 	LDI	@MONKEYOBJRVI,AR1
    romdata = MONKEYOBJRVI;
    // asm: 	CALL	OBJ_GET
    obj = OBJ_GET();
    // asm: 	LDI	AR0,AR4
    // asm: 	LDLH	O_3DROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    obj->flags = O_3DROT | O_NOUROT | O_NOUNIV;
    // asm: 	STI	AR0,*+AR7(PDATA)
    p->ctx.SCREENWIPE.objects[0] = obj;
    // asm: 	STI	AR1,*+AR4(OROMDATA)
    obj->romdata = ROM_PTR(romdata);
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF_INT(368);
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    obj->usr1_as_float = C3X_STF(C3X_LDF(obj->pos.X));
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OUSR2)
    obj->usr2_as_float = C3X_STF(C3X_LDF(obj->pos.Y));
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OUSR3)
    obj->usr3_as_float = C3X_STF(C3X_LDF(obj->pos.Z));
    // asm: 	LDI	@MONKEYOBJLVI,AR1
    romdata = MONKEYOBJLVI;
    // asm: 	CALL	OBJ_GET
    obj = OBJ_GET();
    // asm: 	LDI	AR0,AR4
    // asm: 	LDLH	O_3DROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    obj->flags = O_3DROT | O_NOUROT | O_NOUNIV;
    // asm: 	STI	AR0,*+AR7(PDATA+1)
    p->ctx.SCREENWIPE.objects[1] = obj;
    // asm: 	STI	AR1,*+AR4(OROMDATA)
    obj->romdata = ROM_PTR(romdata);
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF_INT(368);
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    obj->usr1_as_float = C3X_STF(C3X_LDF(obj->pos.X));
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OUSR2)
    obj->usr2_as_float = C3X_STF(C3X_LDF(obj->pos.Y));
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OUSR3)
    obj->usr3_as_float = C3X_STF(C3X_LDF(obj->pos.Z));
    // asm: 	LDI	@MONKEYOBJUHI,AR1
    romdata = MONKEYOBJUHI;
    // asm: 	CALL	OBJ_GET
    obj = OBJ_GET();
    // asm: 	LDI	AR0,AR4
    // asm: 	LDLH	O_3DROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    obj->flags = O_3DROT | O_NOUROT | O_NOUNIV;
    // asm: 	STI	AR0,*+AR7(PDATA+2)
    p->ctx.SCREENWIPE.objects[2] = obj;
    // asm: 	STI	AR1,*+AR4(OROMDATA)
    obj->romdata = ROM_PTR(romdata);
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF_INT(368);
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    obj->usr1_as_float = C3X_STF(C3X_LDF(obj->pos.X));
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OUSR2)
    obj->usr2_as_float = C3X_STF(C3X_LDF(obj->pos.Y));
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OUSR3)
    obj->usr3_as_float = C3X_STF(C3X_LDF(obj->pos.Z));
    // asm: 	LDI	@MONKEYOBJLHI,AR1
    romdata = MONKEYOBJLHI;
    // asm: 	CALL	OBJ_GET
    obj = OBJ_GET();
    // asm: 	LDI	AR0,AR4
    // asm: 	LDLH	O_3DROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    obj->flags = O_3DROT | O_NOUROT | O_NOUNIV;
    // asm: 	STI	AR0,*+AR7(PDATA+3)
    p->ctx.SCREENWIPE.objects[3] = obj;
    // asm: 	STI	AR1,*+AR4(OROMDATA)
    obj->romdata = ROM_PTR(romdata);
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF_INT(368);
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    obj->usr1_as_float = C3X_STF(C3X_LDF(obj->pos.X));
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OUSR2)
    obj->usr2_as_float = C3X_STF(C3X_LDF(obj->pos.Y));
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OUSR3)
    obj->usr3_as_float = C3X_STF(C3X_LDF(obj->pos.Z));
    // asm: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *MEMCPY
 *
 *PARAMETERS
 *	AR0	SOURCE POINTER
 *	AR1	DEST POINTER
 *	RC	LENGTH -1
 *
 */
static void MEMCPY(const c3x_f32_t* source, c3x_f32_t* dest, int length_minus_one) {
    c3x_f32_t value;
    int count;

    // asm 0000B0AF: 	LONGROUT
    MAME_ASSERT_FUNCTION_ENTRY();
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	R0
    // asm: 	RPTB	MEMCPYL
    count = length_minus_one + 1;
    // asm: 	LDI	*AR0++,R0
    value = *source++;
MEMCPYL:
    // asm: STI	R0,*AR1++
    *dest++ = value;
    count -= 1;
    if (count > 0) {
        value = *source++;
        goto MEMCPYL;
    }
    // asm: 	POP	R0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
}

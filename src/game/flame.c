
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
#include "validator.h"
#include "vunit.h"

/*
 * Source module: asm/FLAME.ASM
 */

void FLAME_PRC(PROC* p);
static int animate_child(OBJ* obj /*AR0*/, OBJ* parent /*AR6*/, MATRIX* matrix /*AR3*/, const int* animation /*AR1*/, int frame_on /*IR0*/);
void FLAMESORT(void);
MATRIX* GETCARBODY(OBJ* car /*AR2*/);
OBJ* FIND_NEXT_OBJ(OBJ* obj /*AR0*/, int oid /*R1*/);
extern MATRIX _MATRIXA;
extern MATRIX _MATRIXB;

#define FLAMEANII FLAMEANI
#define FLAME_POSI FLAME_POS

static int FLAMEANI[10];
static int FLAME_POS[21];

/*
 *----------------------------------------------------------------------------
 *
 */

/* asm: FLAME_POS */
/* asm: 	.word	180,87,-450		;Vette */
/* asm: 	.word	130,105,-586		;Coope */
/* asm: 	.word	140,107,-460		;Missle */
/* asm: 	.word	160,70,-460		;Ferrari */
/* asm: 	.word	180,87,-450		;JEEP */
/* asm: 	.word	190,87,-650		;SBUSP */
/* asm: 	.word	160,155,-500		;COPCAR */
/* asm: 	 */
static int FLAME_POS[] = {
    180,
    87,
    -450, // Vette
    130,
    105,
    -586, // Coope
    140,
    107,
    -460, // Missle
    160,
    70,
    -460, // Ferrari
    180,
    87,
    -450, // JEEP
    190,
    87,
    -650, // SBUSP
    160,
    155,
    -500, // COPCAR
};
/* asm: FLAMEANI */
/* asm: 	.word	sparc1,sparc3,sparc5,sparc7,sparc9,sparc7 */
/* asm: 	.word	sparc5,sparc3,sparc1,-1 */
/* asm: 	 */
static int FLAMEANI[] = {
    sparc1_ROM,
    sparc3_ROM,
    sparc5_ROM,
    sparc7_ROM,
    sparc9_ROM,
    sparc7_ROM,
    sparc5_ROM,
    sparc3_ROM,
    sparc1_ROM,
    -1,
};
/*
 *----------------------------------------------------------------------------
 * FLAME	PROC
 *
 */
#define FRAME_ON (PDATA + 1)
#define CARBODY_MATRIX (PDATA + 2)

void FLAME_PRC(PROC* p) {
    c3x_reg_t flame_x;
    c3x_reg_t flame_y;
    c3x_reg_t flame_z;
    const int* flame_pos;

    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 0000ADC3: 	LDI	PLYR_C|PLYR_FLAMES_S,AR2
    // asm 0000ADC4: 	CALL	OBJ_FIND_FIRST
    // asm 0000ADC5: 	BC	FLAME_DIE		;Only one set of flames at a time
    if (OBJ_FIND_FIRST(PLYR_C | PLYR_FLAMES_S) != NULL) {
        goto FLAME_DIE; // Only one set of flames at a time
    }
    // asm 0000ADC6: 	LDI	PLYR_C,AR2
    // asm 0000ADC7: 	CALL	OBJ_FIND_FIRST
    // asm 0000ADC8: 	BNC	FLAME_DIE		;IF there is no player object then bail.
    p->ctx.FLAME_PRC.player_obj = OBJ_FIND_FIRST(PLYR_C);
    if (p->ctx.FLAME_PRC.player_obj == NULL) {
        goto FLAME_DIE; // IF there is no player object then bail.
    }
    // asm 0000ADC9: 	LDI	AR0,AR6
    // asm 0000ADCA: 	LDI	*+AR0(OCARBLK),AR1
    // asm 0000ADCB: 	LDI	*+AR1(CAR_SPIN),R0
    // asm 0000ADCC: 	BNZ	FLAME_DIE			;YES, the no flames
    if (p->ctx.FLAME_PRC.player_obj->carblk->spin_flag != 0) {
        goto FLAME_DIE; // YES, then no flames
    }
    // asm 0000ADCD: 	LDI	AR0,AR2
    // asm 0000ADCE: 	CALL	GETCARBODY
    p->ctx.FLAME_PRC.body_matrix = GETCARBODY(p->ctx.FLAME_PRC.player_obj);
    // asm 0000ADCF: 	STI	AR0,*+AR7(CARBODY_MATRIX)
    // asm 0000ADD0: 	LDI	0,R0
    // asm 0000ADD1: 	STI	R0,*+AR7(FRAME_ON)	;Reset animation counter
    p->ctx.FLAME_PRC.frame_on = 0; // Reset animation counter
    // asm 0000ADD2: 	CALL	OBJ_GET
    // asm 0000ADD3: 	BC	FLAME_DIE
    p->ctx.FLAME_PRC.left_flame = OBJ_GET();
    if (p->ctx.FLAME_PRC.left_flame == NULL) {
        goto FLAME_DIE;
    }
    // asm 0000ADD4: 	LDI	AR0,AR4			;Left flame
    // ;	LDI	@CHOSEN_VEHICLE,R0
    // asm 0000ADD5: 	LDI	@CHOOSENCAR,R0
    // asm 0000ADD6: 	MPYI	3,R0
    // asm 0000ADD7: 	ADDI	@FLAME_POSI,R0
    flame_pos = &FLAME_POSI[CHOOSENCAR * 3];
    // asm 0000ADD8: 	LDI	R0,AR0
    // asm 0000ADD9: 	FLOAT	*+AR0(X),R1
    flame_x = C3X_FROM_INT(flame_pos[0]);
    // asm 0000ADDA: 	FLOAT	*+AR0(Y),R2
    flame_y = C3X_FROM_INT(flame_pos[1]);
    // asm 0000ADDB: 	FLOAT	*+AR0(Z),R3
    flame_z = C3X_FROM_INT(flame_pos[2]);
    // asm 0000ADDC: 	STF	R1,*+AR4(OVELX)
    p->ctx.FLAME_PRC.left_flame->vel_x = C3X_STF(flame_x);
    // asm 0000ADDD: 	STF	R2,*+AR4(OVELY)
    p->ctx.FLAME_PRC.left_flame->vel_y = C3X_STF(flame_y);
    // asm 0000ADDE: 	STF	R3,*+AR4(OVELZ)
    p->ctx.FLAME_PRC.left_flame->vel_z = C3X_STF(flame_z);
    // asm 0000ADDF: 	LDI	PLYR_C|PLYR_FLAMES_S,R0
    // asm 0000ADE0: 	STI	R0,*+AR4(OID)
    p->ctx.FLAME_PRC.left_flame->id = PLYR_C | PLYR_FLAMES_S;
    // asm 0000ADE1: 	CALL	OBJ_GET
    // asm 0000ADE2: 	BC	FLAME_DIE
    p->ctx.FLAME_PRC.right_flame = OBJ_GET();
    if (p->ctx.FLAME_PRC.right_flame == NULL) {
        goto FLAME_DIE;
    }
    // asm 0000ADE3: 	LDI	AR0,AR5			;Right frame
    // asm 0000ADE4: 	LDF	PI,R0
    // asm 0000ADE5: 	STF	R0,*+AR5(ORADY)
    p->ctx.FLAME_PRC.right_flame->rady = C3X_STF_IMM(PI);
    // asm 0000ADE6: 	FLOAT	180,R0
    (void)C3X_FROM_INT(180);
    // asm 0000ADE7: 	NEGF	R1			;opposite side
    flame_x = C3X_NEG(flame_x); // opposite side
    // asm 0000ADE8: 	STF	R1,*+AR5(OVELX)
    p->ctx.FLAME_PRC.right_flame->vel_x = C3X_STF(flame_x);
    // asm 0000ADE9: 	STF	R2,*+AR5(OVELY)
    p->ctx.FLAME_PRC.right_flame->vel_y = C3X_STF(flame_y);
    // asm 0000ADEA: 	STF	R3,*+AR5(OVELZ)
    p->ctx.FLAME_PRC.right_flame->vel_z = C3X_STF(flame_z);
    // asm 0000ADEB: 	LDI	PLYR_C|PLYR_FLAMES_S,R0
    // asm 0000ADEC: 	STI	R0,*+AR5(OID)
    p->ctx.FLAME_PRC.right_flame->id = PLYR_C | PLYR_FLAMES_S;
    // asm 0000ADED: 	LDI	AR4,AR2
    // asm 0000ADEE: 	CALL	OBJ_INSERT
    OBJ_INSERT(p->ctx.FLAME_PRC.left_flame);
    // asm 0000ADEF: 	LDI	AR5,AR2
    // asm 0000ADF0: 	CALL	OBJ_INSERT
    OBJ_INSERT(p->ctx.FLAME_PRC.right_flame);
    // asm 0000ADF1: 	LDI	9-1,R5
    p->ctx.FLAME_PRC.frames_remaining = 9 - 1;
FLAME_ANI_LOOP:
    // asm 0000ADF2: 	LDI	*+AR6(OCARBLK),AR1
    // asm 0000ADF3: 	LDI	*+AR1(CAR_SPIN),R0
    // asm 0000ADF4: 	BNZ	FLAME_ANIX			;YES, the kill the flames
    if (p->ctx.FLAME_PRC.player_obj->carblk->spin_flag != 0) {
        goto FLAME_ANIX; // YES, then kill the flames
    }
    // asm 0000ADF5: 	LDI	*+AR7(CARBODY_MATRIX),AR2
    // asm 0000ADF6: 	LDI	AR6,R2
    // asm 0000ADF7: 	ADDI	OMATRIX,R2
    // asm 0000ADF8: 	LDI	@MATRIXAI,R3
    // asm 0000ADF9: 	CALL	CONCATMATV
    CONCATMATV(p->ctx.FLAME_PRC.body_matrix, (MATRIX*)&p->ctx.FLAME_PRC.player_obj->omatrix, &MATRIXAI);
    // asm 0000ADFA: 	LDI	@MATRIXAI,AR3
    // asm 0000ADFB: 	LDI	AR4,AR0
    // asm 0000ADFC: 	LDI	*+AR7(FRAME_ON),IR0
    // asm 0000ADFD: 	LDI	@FLAMEANII,AR1
    // asm 0000ADFE: 	CALL	animate_child
    animate_child(p->ctx.FLAME_PRC.left_flame, p->ctx.FLAME_PRC.player_obj, &MATRIXAI, FLAMEANII,
        p->ctx.FLAME_PRC.frame_on);
    // asm 0000ADFF: 	LDI	@MATRIXAI,AR3
    // asm 0000AE00: 	LDI	*+AR7(FRAME_ON),IR0
    // asm 0000AE01: 	LDI	@FLAMEANII,AR1
    // asm 0000AE02: 	LDI	AR5,AR0
    // asm 0000AE03: 	CALL	animate_child
    p->ctx.FLAME_PRC.frame_on = animate_child(
        p->ctx.FLAME_PRC.right_flame, p->ctx.FLAME_PRC.player_obj, &MATRIXAI, FLAMEANII,
        p->ctx.FLAME_PRC.frame_on);
    // asm 0000AE04: 	STI	R0,*+AR7(FRAME_ON)
    // asm 0000AE05: 	SLEEP	1
    SLEEP(1, 1);
    // asm 0000AE07: 	SUBI	1,R5
    p->ctx.FLAME_PRC.frames_remaining -= 1;
    // asm 0000AE08: 	BP	FLAME_ANI_LOOP
    if (p->ctx.FLAME_PRC.frames_remaining >= 0) {
        goto FLAME_ANI_LOOP;
    }
FLAME_ANIX:
    // asm 0000AE09: 	LDI	PLYR_C|PLYR_FLAMES_S,AR2
    // asm 0000AE0A: 	CALL	OBJ_FIND_FIRST
    p->ctx.FLAME_PRC.left_flame = OBJ_FIND_FIRST(PLYR_C | PLYR_FLAMES_S);
    // asm 0000AE0B: 	BNC	FLANX1
    if (p->ctx.FLAME_PRC.left_flame == NULL) {
        goto FLANX1;
    }
    // asm 0000AE0C: 	LDI	AR0,AR2
    // asm 0000AE0D: 	CALL	OBJ_DELETE
    OBJ_DELETE(p->ctx.FLAME_PRC.left_flame);
    // asm 0000AE0E: 	BR	FLAME_ANIX
    goto FLAME_ANIX;
FLANX1:
FLAME_DIE:
    // asm 0000AE0F: 	DIE
    DIE();
}

/*
 *----------------------------------------------------------------------------
 *Input
 *	AR0 = object to position and ani
 *	    OVELX/Y/Z = offset from parent
 *	AR6 = object to position reletive to
 *	AR1 = Animation list
 *	IR0  = Frame on
 *OUTPUT
 *	IR0  = Next frame
 *TRASHES	AR2,R1,R2,R3
 */

static int animate_child(OBJ* obj /*AR0*/, OBJ* parent /*AR6*/, MATRIX* matrix /*AR3*/, const int* animation /*AR1*/, int frame_on /*IR0*/) {
    int frame;

    // ;Set the Frame
set_frame:
    // asm 0000AE10: 	LDI	*+AR1(IR0),R1
    frame = animation[frame_on];
    // asm 0000AE11: 	LDIN	0,IR0
    // asm 0000AE12: 	BN	set_frame
    if (frame < 0) {
        frame_on = 0;
        goto set_frame;
    }
    // asm 0000AE13: 	ADDI	1,IR0
    frame_on += 1;
    // asm 0000AE14: 	STI	R1,*+AR0(OROMDATA)
    obj->romdata = ROM_PTR((word_addr_t)frame);
    // asm 0000AE15: 	PUSH	IR0
    // ;Set the position reletive to the parent object.
    // asm 0000AE16: 	LDI	AR3,R2
    // asm 0000AE17: 	LDI	AR0,AR2
    // asm 0000AE18: 	ADDI	OMATRIX,AR2
    // asm 0000AE19: 	CALL	CPYMAT
    CPYMAT((MATRIX*)&obj->omatrix, matrix);
    // asm 0000AE1A: 	LDI	AR0,AR2		;rotate the position reletive to parent
    // asm 0000AE1B: 	ADDI	OVELX,AR2
    // asm 0000AE1C: 	LDI	AR0,R2			;Source 3X3
    // asm 0000AE1D: 	ADDI	OMATRIX,R2
    // asm 0000AE1E: 	LDI	AR0,R3			;Dest 1X3
    // asm 0000AE1F: 	ADDI	OPOSX,R3
    // asm 0000AE20: 	CALL	MATRIX_MUL
    MATRIX_MUL((VECTOR*)&obj->vel_x, (MATRIX*)&obj->omatrix, &obj->pos);
    // asm 0000AE21: 	LDF	*+AR6(OPOSX),R2		;Set the position
    // asm 0000AE22: 	ADDF	*+AR0(OPOSX),R2
    // asm 0000AE23: 	STF	R2,*+AR0(OPOSX)
    obj->pos.X = C3X_STF(C3X_ADD(C3X_LDF(parent->pos.X), C3X_LDF(obj->pos.X)));
    // asm 0000AE24: 	LDF	*+AR6(OPOSY),R2
    // asm 0000AE25: 	ADDF	*+AR0(OPOSY),R2
    // asm 0000AE26: 	STF	R2,*+AR0(OPOSY)
    obj->pos.Y = C3X_STF(C3X_ADD(C3X_LDF(parent->pos.Y), C3X_LDF(obj->pos.Y)));
    // asm 0000AE27: 	LDF	*+AR6(OPOSZ),R2
    // asm 0000AE28: 	ADDF	*+AR0(OPOSZ),R2
    // asm 0000AE29: 	STF	R2,*+AR0(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_ADD(C3X_LDF(parent->pos.Z), C3X_LDF(obj->pos.Z)));
    // asm 0000AE2A: 	LDI	@MATRIXBI,AR2
    // asm 0000AE2B: 	LDI	AR0,R2
    // asm 0000AE2C: 	ADDI	ORADX,R2
    // asm 0000AE2D: 	CALL	FIND_MATRIX
    FIND_MATRIX(&MATRIXBI, &obj->rad);
    // asm 0000AE2E: 	LDI	AR3,AR2
    // asm 0000AE2F: 	LDI	@MATRIXBI,R2
    // asm 0000AE30: 	LDI	AR0,R3
    // asm 0000AE31: 	ADDI	OMATRIX,R3
    // asm 0000AE32: 	CALL	CONCATMATV
    CONCATMATV(matrix, &MATRIXBI, (MATRIX*)&obj->omatrix);
    // asm 0000AE33: 	POP	R0
    // asm 0000AE34: 	RETS
    return frame_on;
}

/*
 *----------------------------------------------------------------------------
 *CALLED BY ZSORTWT in obj.asm just after the players car is linked in.
 *PUTS THE FLAMES JUST BEHIND THE PLAYERS CAR
 */

void FLAMESORT(void) {
    OBJ* cursor;
    OBJ* flame1;
    OBJ* flame2;
    OBJ* player;
    OBJ* after_player;
    u32 flame1_flags;
    u32 flame2_flags;

    // asm 0000AE35: 	LDI	@OACTIVE,AR0
    cursor = OACTIVE;
    // asm 0000AE36: 	CMPI	0,AR0
    // asm 0000AE37: 	BEQ	FLMSORTX		;NO LIST... NO GO
    if (cursor == NULL) {
        goto FLMSORTX;
    }
FLMSORT_LOOP:
    // asm 0000AE38: FLMSORT_LOOP
    // asm 0000AE38: 	LDI	PLYR_C|PLYR_FLAMES_S,R1
    // asm 0000AE39: 	CALL	FIND_NEXT_OBJ
    flame1 = FIND_NEXT_OBJ(cursor, PLYR_C | PLYR_FLAMES_S);
    // asm 0000AE3A: 	BC	FLMSORTX
    if (flame1 == NULL) {
        goto FLMSORTX;
    }
    // asm 0000AE3B: 	LDI	AR0,AR4
    // asm 0000AE3C: 	CALL	FIND_NEXT_OBJ
    flame2 = FIND_NEXT_OBJ(flame1, PLYR_C | PLYR_FLAMES_S);
    // asm 0000AE3D: 	BC	FLMSORTX
    if (flame2 == NULL) {
        goto FLMSORTX;
    }
    // asm 0000AE3E: 	LDI	AR0,AR5
    // asm 0000AE3F: 	LDI	*+AR4(OFLAGS),R0	;SAVE the flages
    flame1_flags = flame1->flags;
    // asm 0000AE40: 	LDI	*+AR5(OFLAGS),R1	;SAVE the flages
    flame2_flags = flame2->flags;
    // asm 0000AE41: 	LDI	AR4,AR2
    // asm 0000AE42: 	CALL	OBJ_PULL
    OBJ_PULL(flame1);
    // asm 0000AE43: 	LDI	AR5,AR2
    // asm 0000AE44: 	CALL	OBJ_PULL
    OBJ_PULL(flame2);
    // asm 0000AE45: 	STI	R0,*+AR4(OFLAGS)	;RESTORE the flags
    flame1->flags = flame1_flags;
    // asm 0000AE46: 	STI	R1,*+AR5(OFLAGS)	;RESTORE the flags
    flame2->flags = flame2_flags;
    // asm 0000AE47: 	LDI	PLYR_C,AR2
    // asm 0000AE48: 	CALL	OBJ_FIND_FIRST
    player = OBJ_FIND_FIRST(PLYR_C);
    // asm 0000AE49: 	BNC	FLMSORTX		;NO PLAYER?
    if (player == NULL) {
        goto FLMSORTX;
    }
    // asm 0000AE4A: 	STI	AR5,*AR4		;Link flame2 to flame1
    flame1->link = flame2;
    // asm 0000AE4B: 	LDI	*AR0,R0			;Get link to next object
    after_player = player->link;
    // asm 0000AE4C: 	STI	AR4,*AR0		;link the flames to the players object
    player->link = flame1;
    // asm 0000AE4D: 	STI	R0,*AR5			;link back in the object behind the player
    flame2->link = after_player;
FLMSORTX:
    // asm 0000AE4E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLAMESORT", 0, 0);
}

/*
 *----------------------------------------------------------------------------
 *FIND THE MATRIX IN THE DYNALIST OF THE CAR POINTED TO BY AR2
 *INPUT	AR2 = CARS OBJECT
 *OUTPUT AR0 = MATRIX OF THE BODY
 * SCRAMBLES R0,R1
 */

MATRIX* GETCARBODY(OBJ* car /*AR2*/) {
    DYNAOBJ* dynamic_obj;

    // asm 0000AE4F: 	LDI	*+AR2(ODYNALIST),R0
    dynamic_obj = car->dynalist;
    // asm: 	SLOCKON	Z,"UTIL\CARPROC   dynamic objects not found"
    SLOCKON(dynamic_obj == NULL, "UTIL\\CARPROC   dynamic objects not found");
FBLOOP:
    // asm 0000AE50: 	LDI	R0,AR0
    // asm 0000AE51: 	LDI	*+AR0(DYNAFLAG),R1
    // asm 0000AE52: 	BZ	FOUND_BODY		;0 = car body
    if (dynamic_obj->flag == DYNAF_BODY) {
        goto FOUND_BODY; // 0 = car body
    }
    // asm 0000AE53: 	LDI	*AR0,R0
    dynamic_obj = dynamic_obj->link;
    // asm 0000AE54: 	BR	FBLOOP
    goto FBLOOP;
FOUND_BODY:
    // asm 0000AE55: 	ADDI	DYNAMATRIX,AR0
    // asm 0000AE56: 	RETS
    return (MATRIX*)&dynamic_obj->omatrix;
}

OBJ* FIND_NEXT_OBJ(OBJ* obj /*AR0*/, int oid /*R1*/) {
    OBJ* next_obj;

    // asm 0000AE57: 	PUSH	R0
    // asm 0000AE58: 	PUSHF	R0
    // asm 0000AE59: 	PUSH	AR5
    // asm 0000AE5A: 	LDI	*AR0,AR5
    next_obj = obj->link;
OFN:
    // asm 0000AE5B: LDI	AR5,R0
    // asm 0000AE5C: 	LDI	R0,AR0
    obj = next_obj;
    if (obj == NULL) {
        goto OFNX; // ;NONE FOUND. This routine is passive
    }
    // asm 0000AE5D: 	BZ	OFNX		;NONE FOUND. This routine is passive
    // asm 0000AE5E: 	LDI	*AR0,AR5
    next_obj = obj->link;
    // asm 0000AE5F: 	LDI	*+AR0(OID),R0
    // asm 0000AE60: 	CMPI	R1,R0
    if (obj->id != (u32)oid) {
        goto OFN;
    }
    // asm 0000AE61: 	BNE	OFN
OFNX:
    // asm 0000AE62: 	CLRC
    // asm 0000AE63: 	CMPI	0,AR0
    if (obj != NULL) {
        goto OFNX1;
    }
    // asm 0000AE64: 	BNE	OFNX1
    // asm 0000AE65: 	SETC
OFNX1:
    // asm 0000AE66: 	POP	AR5
    // asm 0000AE67: 	POPF	R0
    // asm 0000AE68: 	POP	R0
    // asm 0000AE69: 	RETS
    return obj;
}

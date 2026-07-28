
#include "../core/machine.h"
#include "cmos.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "sndtab.h"
#include "sys.h"
#include "text.h"
#include "vunit.h"
#include <stdio.h>

/*
 * Source module: asm/ATTRACTA.ASM
 */

void _MIDWAYSPIN(PROC* p);
static int MSLP_CHECK(PROC* p /*AR7*/, int* sound_ticks /*AR6*/);
void MIDWAYSPINENTER(void);
void SPIN_CAR(void);
static void TEXTTOGET(void);
static void DEMOTHANKS(void);

#define SPIN_CARTABI SPIN_CARTAB
#define DEMOTHANKS_LISTI DEMOTHANKS_LIST

static uintptr_t DEMOTHANKS_LIST[];

/*
 *----------------------------------------------------------------------------
 *ATTRACT MODE
 *
 */

#define NOVANITY 1
// *----------------------------------------------------------------------------
#define startthey 4.18894205  //-deg(10)
#define startthez 0.087266462 // deg(5)
#define CREATED_DCS (PDATA + 1)
#define DECOMP_COUNT (PDATA + 1)

/*
 *----------------------------------------------------------------------------
 *AR6 is set by the creator of this routine. 0= play dcs logo sound 1=don't play
 *
 */
void _MIDWAYSPIN(PROC* p /*AR7*/) {
    OBJ* obj;

    switch (p->resume_state) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    }

    // asm 0000A966: 	LDI	0,AR6
    p->ctx->MIDWAYSPIN.sound_ticks = 0;

    // asm 0000A967: 	LDI	@_ATTR_MODE,R0
    // asm 0000A968: 	CMPI	-2,R0
    // asm 0000A969: 	BNE	NO_NINTENDO
    if (_ATTR_MODE == -2 && p->ctx->MIDWAYSPIN.attrwave == 0 && READADJ(ADJ_ATTRACT_MODE_SOUND) != 0) {
        // asm 0000A970: 	READADJ	ADJ_VOLUME		;make sure volume correct
        // asm 0000A972: 	LDI	R0,R1
        // asm 0000A973: 	CALL	SET_MASTER_VOL
        SET_MASTER_VOL(READADJ(ADJ_VOLUME)); // ;make sure volume correct

        // asm 0000A974: 	LDI	@DIPRAM,R0
        // asm 0000A975: 	TSTB	DIP_COMMP,R0
        // asm 0000A976: 	BNZ	DOMUS
        // asm 0000A977: 	TSTB	CMDP_MASTER,R0
        // asm 0000A978: 	BNZ	NO_NINTENDO
        if ((DIPRAM & DIP_COMMP) != 0 || (DIPRAM & CMDP_MASTER) == 0) {
            // asm 0000A979: 	LDI	890,AR6			;Length of sound
            p->ctx->MIDWAYSPIN.sound_ticks = 890; // ;Length of sound
            // asm 0000A97A: 	SOND1	NINTENDO_SND
            SOND1(NINTENDO_SND);
        }
    }

NO_NINTENDO:
    // asm 0000A97C: 	CALL	ULTRA_LOGO
    ULTRA_LOGO();

    // asm 0000A97D: 	LDI	0,R0
    // asm 0000A97E: 	STI	R0,*+AR7(DECOMP_COUNT)
    // ;       STI     R0,*+AR7(CREATED_DCS)
    p->ctx->MIDWAYSPIN.decomp_count = 0;

    // asm 0000A97F: 	LDI	0,R0
    // asm 0000A980: 	STI	R0,@BGNDCOLA
    BGNDCOLA = 0;

    // asm 0000A981: 	LDP	@_CAMERAPOS+Y
    // asm 0000A982: 	FLOAT	-2780,R0
    // asm 0000A983: 	STF	R0,@_CAMERAPOS+Y
    _CAMERAPOS.Y = C3X_STF(C3X_REG(C3X_FROM_INT(-2780)));

    // asm 0000A984: 	SETDP
    // asm 0000A985: 	LDL	midway,AR2
    // asm 0000A986: 	CALL	OBJ_GETE
    obj = OBJ_GETE(ROM_PTR(midway_ROM));
    p->ctx->MIDWAYSPIN.obj = obj;

#if DEBUG
    // asm: 	BC	$
    if (obj == NULL) {
        for (;;) {
        }
    }
#endif

    // asm 0000A988: 	CLRF	R0
    // asm 0000A989: 	STF	R0,*+AR4(OPOSX)
    // ;       FLOAT   -480,R0
    obj->pos.X = C3X_STF(C3X_REG(C3X_FROM_INT(0)));

    // asm 0000A98A: 	FLOAT	-910,R0
    // asm 0000A98B: 	STF	R0,*+AR4(OPOSY)
    obj->pos.Y = C3X_STF(C3X_REG(C3X_FROM_INT(-910)));

    // asm 0000A98C: 	FLOAT	5500,R0
    // asm 0000A98D: 	STF	R0,*+AR4(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_REG(C3X_FROM_INT(5500)));

    // asm 0000A98E: 	LDI	*+AR4(OFLAGS),R0
    // asm 0000A98F: 	OR	O_ILLUM|O_NOUNIV|O_NOUROT,R0
    // asm 0000A990: 	LDI	1,R1
    // asm 0000A991: 	LS	16,R1
    // asm 0000A992: 	OR	R1,R0
    // asm 0000A993: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= O_ILLUM | O_NOUNIV | O_NOUROT | (1 << 16);

    // asm 0000A994: 	CLRF	R0
    // asm 0000A995: 	STF	R0,*+AR4(ORADX)
    obj->rad.X = C3X_STF(C3X_REG(C3X_FROM_INT(0)));

    // asm 0000A996: 	LDF	startthey,R2
    // asm 0000A997: 	ADDF	HALFPI,R2
    // asm 0000A998: 	STF	R2,*+AR4(ORADY)
    obj->rad.Y = C3X_STF(C3X_REG(C3X_ADD(C3X_IMM_F32(startthey), C3X_IMM_F32(HALFPI))));

    // asm 0000A999: 	LDF	startthez,R2
    // asm 0000A99A: 	STF	R2,*+AR4(ORADZ)
    obj->rad.Z = C3X_STF(C3X_REG(C3X_IMM_F32(startthez)));

    // asm 0000A99B: 	LDI	AR4,AR2
    // asm 0000A99C: 	ADDI	OMATRIX,AR2
    // asm 0000A99D: 	LDI	AR4,R2
    // asm 0000A99E: 	ADDI	ORADX,R2
    // asm 0000A99F: 	CALL	FIND_MATRIX
    FIND_MATRIX(&obj->omatrix, &obj->rad);

    // asm 0000A9A0: 	LDI	AR4,AR2
    // asm 0000A9A1: 	CALL	OBJ_INSERTP
    OBJ_INSERTP(obj);

    // asm 0000A9A2: 	LDF	*+AR4(ORADY),R0
    // asm 0000A9A3: 	STF	R0,*+AR4(OUSR1)
    // ;       LDI     470,AR5
    obj->usr1_as_float = C3X_STF(C3X_REG(obj->rad.Y));

MSLP1:
    // asm 0000A9A4: 	LDF	0.0174539,R0
    // asm 0000A9A5: 	FLOAT	@NFRAMES,R1
    // asm 0000A9A6: 	MPYF	R1,R0
    // asm 0000A9A7: 	ADDF	*+AR4(OUSR1),R0		;ORADY
    obj->usr1_as_float = C3X_STF(C3X_REG(C3X_ADD(obj->usr1_as_float, C3X_MUL(C3X_IMM_F32(0.0174539), C3X_FROM_INT(NFRAMES))))); // ;ORADY

    // asm 0000A9A8: 	STF	R0,*+AR4(OUSR1)
    // asm 0000A9A9: 	LDP	@_CAMERARAD+Y
    // asm 0000A9AA: 	SUBF	@_CAMERARAD+Y,R0
    // asm 0000A9AB: 	SETDP
    // asm 0000A9AC: 	STF	R0,*+AR4(ORADY)
    obj->rad.Y = C3X_STF(C3X_REG(C3X_SUB(obj->usr1_as_float, _CAMERARAD.Y)));

    // asm 0000A9AD: 	LDI	AR4,AR2
    // asm 0000A9AE: 	ADDI	OMATRIX,AR2
    // asm 0000A9AF: 	LDI	AR4,R2
    // asm 0000A9B0: 	ADDI	ORADX,R2
    // asm 0000A9B1: 	CALL	FIND_MATRIX
    FIND_MATRIX(&obj->omatrix, &obj->rad);

MSLP2:
    // asm 0000A9B2: 	SLEEP	1
    SLEEP(1, 1);

    obj = p->ctx->MIDWAYSPIN.obj;

    // asm 0000A9B4: 	CALL	MSLP_CHECK
    if (MSLP_CHECK(p, &p->ctx->MIDWAYSPIN.sound_ticks)) {
        CYCLE_ATTR();
        return;
    }

    // asm 0000A9B5: 	BR	MSLP1
    goto MSLP1;
}

static int MSLP_CHECK(PROC* p /*AR7*/, int* sound_ticks /*AR6*/) {
    // asm 0000A9B6: 	SUBI	@NFRAMES,AR6
    *sound_ticks -= NFRAMES;

    // asm 0000A9B7: 	LDI	@DECOMP_ACTIVE,R0
    // asm 0000A9B8: 	CMPI	0,R0
    // asm 0000A9B9: 	BNE	MSLP3
    if (DECOMP_ACTIVE != 0) {
        return 0;
    }

    // asm 0000A9BA: 	LDI	*+AR7(DECOMP_COUNT),R0
    // asm 0000A9BB: 	ADDI	1,R0
    // asm 0000A9BC: 	STI	R0,*+AR7(DECOMP_COUNT)
    p->ctx->MIDWAYSPIN.decomp_count++;

    // asm 0000A9BD: 	CMPI	3,R0
    // asm 0000A9BE: 	BGT	MSLP4				;Done Loading
    if (p->ctx->MIDWAYSPIN.decomp_count <= 3) {
        return 0;
    }

MSLP4:
    // asm 0000A9C0: 	CMPI	0,AR6
    // asm 0000A9C1: 	RETSGT
    if (*sound_ticks > 0) {
        return 0;
    }

MSLPX:
    // asm 0000A9C2: 	LDI	0,R0
    // asm 0000A9C3: 	STI	R0,@LOADED
    LOADED = 0;

    // asm 0000A9C4: 	POP	BK		;POP return address
    // asm 0000A9C5: 	BR	CYCLE_ATTR
    return 1;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void MIDWAYSPINENTER(void) {
    // asm 0000A9C6: 	LDL	midway,AR2
    // asm 0000A9C7: 	CALL	OBJ_GETE
#if DEBUG
    // asm: 	BC	$
#endif
    // asm 0000A9C8: 	LDI	AR0,AR4
    // asm 0000A9C9: 	CLRF	R0
    // asm 0000A9CA: 	STF	R0,*+AR4(OPOSX)
    // asm 0000A9CB: 	FLOAT	-480,R0
    // asm 0000A9CC: 	STF	R0,*+AR4(OPOSY)
    // asm 0000A9CD: 	FLOAT	5500,R0
    // asm 0000A9CE: 	STF	R0,*+AR4(OPOSZ)
    // asm 0000A9CF: 	LDI	*+AR4(OFLAGS),R0
    // asm 0000A9D0: 	OR	O_ILLUM|O_NOUNIV|O_NOUROT,R0
    // asm 0000A9D1: 	LDI	1,R1
    // asm 0000A9D2: 	LS	16,R1
    // asm 0000A9D3: 	OR	R1,R0
    // asm 0000A9D4: 	STI	R0,*+AR4(OFLAGS)
    // asm 0000A9D5: 	CLRF	R0
    // asm 0000A9D6: 	STF	R0,*+AR4(ORADX)
    // asm 0000A9D7: 	LDF	startthey,R2
    // asm 0000A9D8: 	ADDF	PI,R2
    // asm 0000A9D9: 	STF	R2,*+AR4(ORADY)
    // asm 0000A9DA: 	LDF	startthez,R2
    // asm 0000A9DB: 	STF	R2,*+AR4(ORADZ)
    // asm 0000A9DC: 	LDI	AR4,AR2
    // asm 0000A9DD: 	ADDI	OMATRIX,AR2
    // asm 0000A9DE: 	LDI	AR4,R2
    // asm 0000A9DF: 	ADDI	ORADX,R2
    // asm 0000A9E0: 	CALL	FIND_MATRIX
    // asm 0000A9E1: 	LDI	AR4,AR2
    // asm 0000A9E2: 	CALL	OBJ_INSERTP
    // asm 0000A9E3: 	LDF	*+AR4(ORADY),R0
    // asm 0000A9E4: 	STF	R0,*+AR4(OUSR1)
    // asm 0000A9E5: 	LDI	690,AR5
    // asm 0000A9E6: MSLP1a
    // asm 0000A9E6: 	LDF	*+AR4(OUSR1),R0		;ORADY
    // asm 0000A9E7: 	ADDF	0.0174539,R0
    // asm 0000A9E8: 	STF	R0,*+AR4(OUSR1)
    // asm 0000A9E9: 	LDP	@_CAMERARAD+Y
    // asm 0000A9EA: 	SUBF	@_CAMERARAD+Y,R0
    // asm 0000A9EB: 	SETDP
    // asm 0000A9EC: 	STF	R0,*+AR4(ORADY)
    // asm 0000A9ED: 	LDI	AR4,AR2
    // asm 0000A9EE: 	ADDI	OMATRIX,AR2
    // asm 0000A9EF: 	LDI	AR4,R2
    // asm 0000A9F0: 	ADDI	ORADX,R2
    // asm 0000A9F1: 	CALL	FIND_MATRIX
    // asm 0000A9F2: 	SLEEP	1
    // asm 0000A9F4: 	DBU	AR5,MSLP1a
    // asm 0000A9F5: 	CALL	TEXT_INIT
    // asm 0000A9F6: 	BR	CYCLE_ATTR
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MIDWAYSPINENTER", 0, 0);
    UNIMPL();
}

/* asm: SPIN_CARTAB	.word	missle,hotrod,testor,cvette */
static int SPIN_CARTAB[] = {
    missle_ROM,
    hotrod_ROM,
    testor_ROM,
    cvette_ROM,
};

void SPIN_CAR(void) {
    // asm 0000A9F8: 	LDI	0,R0
    // asm 0000A9F9: 	STI	R0,*+AR7(DECOMP_COUNT)
    // asm 0000A9FA: 	LDIL	logo,AR2
    // asm 0000A9FD: 	CALL	OBJ_GETE
    // asm 0000A9FE: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm 0000A9FF: 	OR	*+AR0(OFLAGS),R0
    // asm 0000AA00: 	STI	R0,*+AR0(OFLAGS)
    // asm 0000AA01: 	FLOAT	-150,R0
    // asm 0000AA02: 	STF	R0,*+AR0(OPOSX)
    // asm 0000AA03: 	FLOAT	-110,R0
    // asm 0000AA04: 	STF	R0,*+AR0(OPOSY)
    // asm 0000AA05: 	FLOAT	436,R0
    // asm 0000AA06: 	STF	R0,*+AR0(OPOSZ)
    // asm 0000AA07: 	LDI	AR0,AR2
    // asm 0000AA08: 	CALL	OBJ_INSERT
    // asm 0000AA09: 	CALL	LOGO_SMALL
    // asm 0000AA0A: 	LDI	@ATTRWAVE,AR2
    // asm 0000AA0B: 	LSH	-1,AR2
    // asm 0000AA0C: 	SUBI	1,AR2
    // asm 0000AA0D: 	AND	3,AR2
    // asm 0000AA0E: 	ADDI	@SPIN_CARTABI,AR2
    // asm 0000AA0F: 	LDI	*AR2,AR2
    // asm 0000AA10: 	CALL	OBJ_GETE
    // asm 0000AA11: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm 0000AA12: 	OR	*+AR0(OFLAGS),R0
    // asm 0000AA13: 	STI	R0,*+AR0(OFLAGS)
    // asm 0000AA14: 	FLOAT	1368,R0
    // asm 0000AA15: 	STF	R0,*+AR0(OPOSZ)
    // asm 0000AA16: 	LDI	AR0,AR4
    // asm 0000AA17: 	LDF	-0.196,R2
    // asm 0000AA18: 	STF	R2,*+AR4(ORADX)
    // asm 0000AA19: 	LDF	3.534,R2
    // asm 0000AA1A: 	STF	R2,*+AR4(ORADY)
    // asm 0000AA1B: 	LDI	AR4,AR2
    // asm 0000AA1C: 	ADDI	OMATRIX,AR2
    // asm 0000AA1D: 	LDI	AR4,R2
    // asm 0000AA1E: 	ADDI	ORADX,R2
    // asm 0000AA1F: 	CALL	FIND_MATRIX
    // asm 0000AA20: 	LDI	AR4,AR2
    // asm 0000AA21: 	CALL	OBJ_INSERTP
    // asm 0000AA22: 	LDI	16,AR6
SPIN_CAR_WAIT:
    // asm 0000AA23: 	SLEEP	1
    // asm 0000AA25: 	SUBI	1,AR6
    // asm 0000AA26: 	CMPI	0,AR6
    // asm 0000AA27: 	BGT	SPIN_CAR_WAIT
    // asm 0000AA28: 	LDI	550,AR6
SPIN_CARLP:
    // asm 0000AA29: 	LDF	*+AR4(ORADY),R2
    // asm 0000AA2A: 	FLOAT	@NFRAMES,R0
    // asm 0000AA2B: 	MPYF	0.018,R0
    // asm 0000AA2C: 	ADDF	R0,R2
    // asm 0000AA2D: 	STF	R2,*+AR4(ORADY)
    // asm 0000AA2E: 	LDI	AR4,R2
    // asm 0000AA2F: 	ADDI	ORADX,R2
    // asm 0000AA30: 	LDI	AR4,AR2
    // asm 0000AA31: 	ADDI	OMATRIX,AR2
    // asm 0000AA32: 	CALL	FIND_MATRIX
    // asm 0000AA33: 	SLEEP	1
    // asm 0000AA35: 	CALL	MSLP_CHECK
    // asm 0000AA36: 	BU	SPIN_CARLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPIN_CAR", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *
 */
static void TEXTTOGET(void) {
    // asm 0000AA37: 	SLEEP	75
    // asm 0000AA39: 	LDI	@TEXT_ACTIVEI,AR0
    // asm 0000AA3A: 	CLRF	R1
TXTLP:
    // asm 0000AA3B: LDI	*AR0,R0
    // asm 0000AA3C: 	BZ	NXTWT
    // asm 0000AA3D: 	LDI	R0,AR0
    // asm 0000AA3E: 	STF	R1,*+AR0(TEXT_VELX)
    // asm 0000AA3F: 	BR	TXTLP
NXTWT:
    // asm 0000AA40: 	SLEEP	460
    // asm 0000AA42: 	LDI	@TEXT_ACTIVEI,AR0
    // asm 0000AA43: 	LDF	-6,R1
TXTLPA:
    // asm 0000AA44: LDI	*AR0,R0
    // asm 0000AA45: 	BNZ	TXTLPB
    // asm 0000AA46: 	BR	SUICIDE
TXTLPB:
    // asm 0000AA47: 	LDI	R0,AR0
    // asm 0000AA48: 	STF	R1,*+AR0(TEXT_VELY)
    // asm 0000AA49: 	BR	TXTLPA
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXTTOGET", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static const char DT1[] = "THANK YOU FOR PLAYING";
static const char DT2[] = "CRUISIN USA";
static const char DT3[] = "SNEAK PREVIEW";
static uintptr_t DEMOTHANKS_LIST[] = {
    140,
    (uintptr_t)&DT1,
    180,
    (uintptr_t)&DT2,
    220,
    (uintptr_t)&DT3,
};

static void DEMOTHANKS(void) {
    // asm 0000AA4A: 	LDI	1,R0
    // asm 0000AA4B: 	STI	R0,@COINOFF
    // asm 0000AA4C: 	CLRI	R0
    // asm 0000AA4D: 	STI	R0,@BGNDCOLA	;COLORAM
    // asm 0000AA4E: 	LDI	2,AR5
    // asm 0000AA4F: 	LDI	@DEMOTHANKS_LISTI,AR4
    // asm 0000AA50: DTLP
    // asm 0000AA50: 	FLOAT	256,R2
    // asm 0000AA51: 	FLOAT	*AR4++,R3
    // asm 0000AA52: 	LDI	*AR4++,AR2
    // asm 0000AA53: 	LDI	9999,RC
    // asm 0000AA54: 	CALL	TEXT_ADDDS
    // asm 0000AA55: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 0000AA58: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 0000AA5B: 	DBU	AR5,DTLP
    // asm 0000AA5C: 	LDI	-6,R0
    // asm 0000AA5D: 	STI	R0,@_ATTR_MODE
    // asm 0000AA5E: 	BR	MIDWAYSPINENTER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEMOTHANKS", 0, 0);
    UNIMPL();
}

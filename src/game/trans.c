#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "obj.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "delta.h"
#include "trans.h"
#include "discovered_defines.h"
#include "discovered_labels.h"

/*
 * Source module: asm/TRANS.ASM
 */

void CHOOSE_TRANSMISSION(void);
void OPEN_DOOR_PROC(void);
void WHEEL_FROM_BELOW(void);
void CENTERTHEONE(void);
void DROPTHETURN(void);
void DROPTHECYCLE(void);
void DROPTHEWHEEL(void);
void DROPTHEOTHER(void);
void SIDE_DOOR(void);
void DOOR_OPENING(void);
void SNAPCURSOR(void);
void WHEEL_ROUT(void);
void DOOR_ELEMENT_DELETE_ALL(void);
void DOOR_ELEMENT_DELETE(void);
void TRANSCHOICE(void);
void GET_UNIT_WHEEL(void);
void TILE_PIECES(void);
void FIX_TRANSMISSION_SCREEN(void);
void MOVE_PUSH_BOX(void);
void TURNTO_SELECT(void);
void CYCLE_PUSH(void);
void ADD_TO_DOOR_LIST(void);
void ENGINE_COLOR_INIT(void);
void ENGINE_COLOR(void);

/* asm: EPALR	.word	EPALRR */
int *EPALR = EPALRR;
/* asm: EPALL	.word	EPALRL */
int *EPALL = EPALRL;
/* asm: EPALRR	.bss	EPALRR,129 */
int EPALRR[129];
/* asm: EPALRL	.bss	EPALRL,129 */
int EPALRL[129];
/* *----------------------------------------------------------------------------
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
int DOORTHETA;
/* asm: DOORLIST	.bss	DOORLIST,1 */
int DOORLIST;
/* *
*
 */
/* asm: DO_LPPX	.float	-256 */
float DO_LPPX = -256.0f;
/* asm: DO_PPZ	.float	368 */
float DO_PPZ = 368.0f;
/* asm: DO_RPPX	.float	256 */
float DO_RPPX = 256.0f;
/* *----------------------------------------------------------------------------
*
*
*
 */
/* asm: LASTCHOICEA	.bss	LASTCHOICEA,1 */
int LASTCHOICEA;
/* *----------------------------------------------------------------------------
*
*RETURNS
*	R0	FL 0-1.0 Steering wheel position
*
 */
/* asm: GUWP	.bss	GUWP,1 */
int GUWP;
/* *----------------------------------------------------------------------------
*
*Object Z values
*	cursor		-3
*	wheel		-2
*	forward objects	-1
*	others		0
*
 */
/* asm: CT_CURSOR	.bss	CT_CURSOR,1 */
int CT_CURSOR;
/* asm: CT_WHEEL	.bss	CT_WHEEL,1 */
int CT_WHEEL;
/* asm: CT_PUSHTOCYCLE	.bss	CT_PUSHTOCYCLE,1 */
int CT_PUSHTOCYCLE;
/* asm: CT_LENG	.bss	CT_LENG,1 */
int CT_LENG;
/* asm: CT_RENG	.bss	CT_RENG,1 */
int CT_RENG;
/* asm: CT_MAN	.bss	CT_MAN,1 */
int CT_MAN;
/* asm: CT_AUTO	.bss	CT_AUTO,1 */
int CT_AUTO;
/* asm: CT_TURNTOSEL	.bss	CT_TURNTOSEL,1 */
int CT_TURNTOSEL;
/* asm: TRANS_HEAD	.bss	TRANS_HEAD,1 */
int TRANS_HEAD;
/* *----------------------------------------------------------------------------
 */
/* asm: TRNTAB	.word	whel1,whel2,whel3,whel4,whel3,whel2,-1 */
int TRNTAB[] = {
    whel1, whel2, whel3, whel4, whel3, whel2, -1,
};
/* *----------------------------------------------------------------------------
 */
/* asm: CYCTAB	.word	trm3,trm2,trm1,trm2,trm3,trm4,-1 */
int CYCTAB[] = {
    trm3, trm2, trm1, trm2, trm3, trm4, -1,
};

/* *----------------------------------------------------------------------------
*CHOOSE TRANSMISSION
*
*
 */
void CHOOSE_TRANSMISSION(void)
{
    // asm 00005A17: 	CALL	CLEANUP_PALS
    // asm 00005A18: 	LDI	1,R0
    // asm 00005A19: 	STI	R0,@NOAERASE
    // asm 00005A1A: 	STI	R0,@NOSWAP
    // asm 00005A1B: 	LDI	MCT,R0
    // asm 00005A1C: 	STI	R0,@_MODE
    // asm 00005A1D: 	CALL	ENGINE_COLOR_INIT
    // asm 00005A1E: 	LDL	chostrns_grp,AR2
    // asm 00005A1F: 	CALL	LOAD_SINGLE_SECTION
    // asm 00005A20: 	CALL	FIX_TRANSMISSION_SCREEN
    // asm 00005A21: 	LDI	@CT_LENG,AR0
    // asm 00005A22: 	LDI	@EPALL,AR2
    // asm 00005A23: 	CALL	PAL_FIND_RAW
    // asm 00005A24: 	STI	R0,*+AR0(OPAL)
    // asm 00005A25: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005A26: 	OR	O_1PAL,R0
    // asm 00005A27: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005A28: 	LDI	@CT_RENG,AR0
    // asm 00005A29: 	LDI	@EPALR,AR2
    // asm 00005A2A: 	CALL	PAL_FIND_RAW
    // asm 00005A2B: 	STI	R0,*+AR0(OPAL)
    // asm 00005A2C: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005A2D: 	OR	O_1PAL,R0
    // asm 00005A2E: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005A2F: 	LDF	1,R0
    // asm 00005A30: 	CALL	ENGINE_COLOR
    // asm 00005A31: 	CLRI	R0
    // asm 00005A32: 	STI	R0,@NOAERASE
    // asm 00005A33: 	STI	R0,@NOSWAP
    // asm 00005A34: 	CREATE	TILE_PIECES,UTIL_C|CHOOSETRANS_T
    // asm 00005A37: 	LDI	39,AR5
LGF7:
    // asm 00005A38: CALL	SIDE_DOOR
    // asm 00005A39: 	CALL	WHEEL_FROM_BELOW
    // asm 00005A3A: 	CALL	TRANSCHOICE
    // asm 00005A3B: 	PUSH	AR5
    // asm 00005A3C: 	LDF	1,R0
    // asm 00005A3D: 	CALL	ENGINE_COLOR
    // asm 00005A3E: 	POP	AR5
    // asm 00005A3F: 	SLEEP	1
    // asm 00005A41: 	DBU	AR5,LGF7
    // asm 00005A42: 	SONDFX	MHIT
    // asm 00005A44: 	LDI	MCT|MGO|MINFIN|MWATER,R0
    // asm 00005A45: 	STI	R0,@_MODE
    // asm 00005A46: 	CLRI	R0
    // asm 00005A47: 	STI	R0,@NOAERASE
    // asm 00005A48: 	STI	R0,@NOSWAP
    // asm 00005A49: 	FLOAT	@_pot0,R0
    // asm 00005A4A: 	STF	R0,@GUWP
    // *
    // *CHOOSE TRANSMISSION LOOP
    // *
    // asm 00005A4B: 	CLRI	R0
    // asm 00005A4C: 	STI	R0,@START_HIT
    // asm 00005A4D: 	LDI	10,R0
    // asm 00005A4E: 	STI	R0,@_countdown
    // asm 00005A4F: 	CALL	INIT_PEDALCHK
CTLP:
    // asm 00005A50: 	LDI	@START_HIT,R0
    // asm 00005A51: 	BNZ	CTLPX
    // asm 00005A52: 	LDI	@CT_CURSOR,AR2
    // asm 00005A53: 	LDF	*+AR2(OPOSX),R0
    // asm 00005A54: 	ADDF	101,R0
    // asm 00005A55: 	FLOAT	204,R1
    // asm 00005A56: 	CALL	DIV_F30
    // asm 00005A57: 	CALL	ENGINE_COLOR
    // asm 00005A58: 	CALL	TRANSCHOICE
    // asm 00005A59: 	CALL	WHEEL_ROUT
    // 	;-------time remaining
    // 	;
    // asm 00005A5A: 	LDL	time,AR2
    // asm 00005A5B: 	LDI	242,R2			;R2	POS X
    // asm 00005A5C: 	LDI	337,R3			;R3	POS Y
    // asm 00005A5D: 	LDI	TM|ZS,R4
    // asm 00005A5E: 	CALL	BLTMOD2D
    // asm 00005A5F: 	CALL	PEDALCHK
    // asm 00005A60: 	BC	CTLPX
    // asm 00005A61: 	CALL	INTROTIMER
    // asm 00005A62: 	SLEEP	1
    // asm 00005A64: 	LDI	@_countdown,R0
    // asm 00005A65: 	BGT	CTLP
CTLPX:
    // asm 00005A66: 	SONDFX	TRANS
    // asm 00005A68: 	LDF	@START_RADY,R2
    // asm 00005A69: 	LDI	@MATRIXAI,AR2
    // asm 00005A6A: 	CALL	HPFIND_YMATRIX
    // asm 00005A6B: 	CALL	CLR_VECTORA
    // asm 00005A6C: 	FLOAT	-650,R0
    // asm 00005A6D: 	STF	R0,*+AR2(Y)
    // asm 00005A6E: 	FLOAT	-28000,R0
    // asm 00005A6F: 	STF	R0,*+AR2(Z)
    // asm 00005A70: 	LDI	@MATRIXAI,R2
    // asm 00005A71: 	LDI	AR2,R3
    // asm 00005A72: 	CALL	MATRIX_MUL
    // asm 00005A73: 	LDF	*+AR2(X),R2
    // asm 00005A74: 	ADDF	@START_POS+X,R2
    // asm 00005A75: 	LDP	@_CAMERAPOS+X
    // asm 00005A76: 	STF	R2,@_CAMERAPOS+X
    // asm 00005A77: 	SETDP
    // asm 00005A78: 	LDF	*+AR2(Y),R2
    // asm 00005A79: 	ADDF	@START_POS+Y,R2
    // asm 00005A7A: 	LDP	@_CAMERAPOS+Y
    // asm 00005A7B: 	STF	R2,@_CAMERAPOS+Y
    // asm 00005A7C: 	SETDP
    // asm 00005A7D: 	LDF	*+AR2(Z),R2
    // asm 00005A7E: 	ADDF	@START_POS+Z,R2
    // asm 00005A7F: 	LDP	@_CAMERAPOS+Z
    // asm 00005A80: 	STF	R2,@_CAMERAPOS+Z
    // asm 00005A81: 	SETDP
    // asm 00005A82: 	CREATE	CENTERTHEONE,UTIL_C|CHOOSECAR_T
    // asm 00005A85: 	CREATE	DROPTHEOTHER,UTIL_C|CHOOSETRANS_T
    // asm 00005A88: 	CALL	SNAPCURSOR
    // asm 00005A89: 	NEGF	@START_RADY,R2
    // asm 00005A8A: 	LDP	@_CAMERARAD+Y
    // asm 00005A8B: 	STF	R2,@_CAMERARAD+Y
    // asm 00005A8C: 	SETDP
    // asm 00005A8D: 	LDI	@CAMERAMATRIXI,AR2
    // asm 00005A8E: 	CALL	HPFIND_YMATRIX
    // asm 00005A8F: 	LDL	garage_grp,AR2
    // asm 00005A90: 	CALL	LOAD_SINGLE_SECTION_OFFSET
    // asm 00005A91: 	SLEEP	3
    // asm 00005A93: 	SONDFX	DD1
    // asm 00005A95: 	SLEEP	7
    // asm 00005A97: 	CREATE	MOVE_PUSH_BOX,UTIL_C|CHOOSETRANS_T
    // asm 00005A9A: 	CALL	GETTHECARS
    // asm 00005A9B: 	CLRI	R0
    // asm 00005A9C: 	STI	R0,@LASTCHOICE
    // asm 00005A9D: 	LDI	4,R0
    // asm 00005A9E: 	STI	R0,@POSES
    // asm 00005A9F: 	LDI	2,R0
    // asm 00005AA0: 	STI	R0,@POSE
    // asm 00005AA1: 	LDI	1,R0
    // asm 00005AA2: 	STI	R0,@CHOSEN_VEHICLE
    // asm 00005AA3: 	CREATE	ROUNDER,UTIL_C|CHOOSECAR_T
    // asm 00005AA6: 	CLRF	R0
    // asm 00005AA7: 	STF	R0,@DOORTHETA
    // asm 00005AA8: 	SONDFX	DOPEN
    // asm 00005AAA: 	JSRP	OPEN_DOOR_PROC
    // asm 00005AB0: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOOSE_TRANSMISSION", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void OPEN_DOOR_PROC(void)
{
    // asm 00005AB4: 	CREATE	THE_CAR_CHOICE_PROC,UTIL_C
    // asm 00005AB7: 	LDI	@VECTORCI,AR2
    // asm 00005AB8: 	CLRF	R0
    // asm 00005AB9: 	STF	R0,*+AR2(X)
    // asm 00005ABA: 	STF	R0,*+AR2(Y)
    // asm 00005ABB: 	FLOAT	-8500,R0
    // asm 00005ABC: 	STF	R0,*+AR2(Z)
    // asm 00005ABD: 	LDI	@MATRIXAI,R2
    // asm 00005ABE: 	LDI	AR2,R3
    // asm 00005ABF: 	CALL	MATRIX_MUL
    // asm 00005AC0: 	LDF	*+AR2(X),R0
    // asm 00005AC1: 	ADDF	@START_POS+X,R0
    // asm 00005AC2: 	STF	R0,*+AR2(X)
    // asm 00005AC3: 	LDF	*+AR2(Z),R0
    // asm 00005AC4: 	ADDF	@START_POS+Z,R0
    // asm 00005AC5: 	STF	R0,*+AR2(Z)
    // asm 00005AC6: 	LDI	30,AR4
IJH:
    // asm 00005AC7: LDF	@DOORTHETA,R0
    // asm 00005AC8: 	ADDF	0.0628,R0
    // asm 00005AC9: 	STF	R0,@DOORTHETA
    // asm 00005ACA: 	PUSH	AR4
    // asm 00005ACB: 	CALL	DOOR_OPENING
    // asm 00005ACC: 	POP	AR4
    // asm 00005ACD: 	LDI	@CAMERAPOSI,AR0
    // asm 00005ACE: 	LDI	@VECTORCI,AR1
    // asm 00005ACF: 	LDF	*+AR1(X),R0
    // asm 00005AD0: 	SUBF	*+AR0(X),R0
    // asm 00005AD1: 	MPYF	0.15,R0
    // asm 00005AD2: 	ADDF	*+AR0(X),R0
    // asm 00005AD3: 	STF	R0,*+AR0(X)
    // asm 00005AD4: 	LDF	*+AR1(Z),R0
    // asm 00005AD5: 	SUBF	*+AR0(Z),R0
    // asm 00005AD6: 	MPYF	0.15,R0
    // asm 00005AD7: 	ADDF	*+AR0(Z),R0
    // asm 00005AD8: 	STF	R0,*+AR0(Z)
    // asm 00005AD9: 	SLEEP	1
    // asm 00005ADB: 	DBU	AR4,IJH
    // asm 00005ADC: 	CALL	DOOR_ELEMENT_DELETE_ALL
    // asm 00005ADD: 	LDI	UTIL_C|CHOOSETRANS_T,R0
    // asm 00005ADE: 	LDI	-1,R1
    // asm 00005ADF: 	CALL	PRC_KILLALL
    // asm 00005AE0: 	LDI	@EPALL,AR2
    // asm 00005AE1: 	CALL	PAL_FIND_RAW
    // asm 00005AE2: 	LDI	R0,AR2
    // asm 00005AE3: 	CALL	PAL_DELETE_RAW
    // asm 00005AE4: 	LDI	@EPALR,AR2
    // asm 00005AE5: 	CALL	PAL_FIND_RAW
    // asm 00005AE6: 	LDI	R0,AR2
    // asm 00005AE7: 	CALL	PAL_DELETE_RAW
    // asm 00005AE8: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OPEN_DOOR_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*AS THE PIECES COME SLIDING IN FROM THE SIDES,
*THE WHEEL AND PUSH TO CHOOSE COME FROM BELOW
*
 */
void WHEEL_FROM_BELOW(void)
{
    // asm 00005AEC: 	LDI	@CT_WHEEL,AR0
    // asm 00005AED: 	LDF	*+AR0(OPOSY),R0
    // asm 00005AEE: 	SUBF	8,R0
    // asm 00005AEF: 	STF	R0,*+AR0(OPOSY)
    // asm 00005AF0: 	LDI	@CT_PUSHTOCYCLE,AR0
    // asm 00005AF1: 	LDF	*+AR0(OPOSY),R0
    // asm 00005AF2: 	SUBF	8,R0
    // asm 00005AF3: 	STF	R0,*+AR0(OPOSY)
    // asm 00005AF4: 	LDI	@CT_TURNTOSEL,AR0
    // asm 00005AF5: 	LDF	*+AR0(OPOSY),R0
    // asm 00005AF6: 	SUBF	8,R0
    // asm 00005AF7: 	STF	R0,*+AR0(OPOSY)
    // asm 00005AF8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WHEEL_FROM_BELOW", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void CENTERTHEONE(void)
{
    // asm 00005AF9: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005AFA: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm 00005AFB: 	LDIEQ	AUD_MANUAL_TRANS_SELECTED,AR2
    // asm 00005AFC: 	LDINE	AUD_AUTOMATIC_TRANS_SELECTED,AR2
    // asm 00005AFD: 	CALL	AUDIT_INC
    // asm 00005AFE: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005AFF: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm 00005B00: 	LDIEQ	@CT_MAN,AR4
    // asm 00005B01: 	LDINE	@CT_AUTO,AR4
    // asm 00005B02: 	LDI	AR4,AR2
    // asm 00005B03: 	CALL	DOOR_ELEMENT_DELETE
    // asm 00005B04: 	LDI	15,AR5
CTOLP:
    // asm 00005B05: LDF	*+AR4(OPOSX),R0
    // asm 00005B06: 	MPYF	0.8,R0
    // asm 00005B07: 	STF	R0,*+AR4(OPOSX)
    // asm 00005B08: 	LDF	*+AR4(OPOSY),R0
    // asm 00005B09: 	MPYF	0.8,R0
    // asm 00005B0A: 	STF	R0,*+AR4(OPOSY)
    // asm 00005B0B: 	SLEEP	1
    // asm 00005B0D: 	DBU	AR5,CTOLP
    // asm 00005B0E: 	LDF	1,R7
    // asm 00005B0F: 	LDF	1,R6
FLYUPL:
    // asm 00005B10: 	LDF	*+AR4(ORADX),R2
    // asm 00005B11: 	SUBF	0.1,R2
    // asm 00005B12: 	STF	R2,*+AR4(ORADX)
    // asm 00005B13: 	LDI	AR4,AR2
    // asm 00005B14: 	ADDI	OMATRIX,AR2
    // asm 00005B15: 	CALL	FIND_XMATRIX
    // asm 00005B16: 	MPYF	1.4,R6
    // asm 00005B17: 	MPYF	1.4,R7
    // asm 00005B18: 	LDF	*+AR4(OPOSX),R0
    // asm 00005B19: 	ADDF	R6,R0
    // asm 00005B1A: 	STF	R0,*+AR4(OPOSX)
    // asm 00005B1B: 	LDF	*+AR4(OPOSY),R0
    // asm 00005B1C: 	SUBF	R6,R0
    // asm 00005B1D: 	STF	R0,*+AR4(OPOSY)
    // asm 00005B1E: 	LDF	*+AR4(OPOSZ),R0
    // asm 00005B1F: 	SUBF	R6,R0
    // asm 00005B20: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005B21: 	SLEEP	1
    // asm 00005B23: 	BU	FLYUPL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CENTERTHEONE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void DROPTHETURN(void)
{
    // asm 00005B24: 	LDI	@CT_TURNTOSEL,AR4
    // asm 00005B25: 	BU	J85
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROPTHETURN", 0, 0);
    UNIMPL();
}

void DROPTHECYCLE(void)
{
    // asm 00005B26: 	LDI	@CT_PUSHTOCYCLE,AR4
    // asm 00005B27: 	BU	J85
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROPTHECYCLE", 0, 0);
    UNIMPL();
}

void DROPTHEWHEEL(void)
{
    // asm 00005B28: 	LDI	@CT_WHEEL,AR4
    // asm 00005B29: 	BU	J85
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROPTHEWHEEL", 0, 0);
    UNIMPL();
}

void DROPTHEOTHER(void)
{
    // asm 00005B2A: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005B2B: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm 00005B2C: 	LDINE	@CT_MAN,AR4
    // asm 00005B2D: 	LDIEQ	@CT_AUTO,AR4
J85:
    // asm 00005B2E: LDI	AR4,AR2
    // asm 00005B2F: 	CALL	DOOR_ELEMENT_DELETE
J86:
    // asm 00005B30: LDI	10,AR5
    // asm 00005B31: 	LDF	2,R6
    // asm 00005B32: DTOLP
    // asm 00005B32: 	LDF	*+AR4(OPOSY),R0
    // asm 00005B33: 	ADDF	R6,R0
    // asm 00005B34: 	STF	R0,*+AR4(OPOSY)
    // asm 00005B35: 	MPYF	2,R6
    // asm 00005B36: 	SLEEP	1
    // asm 00005B38: 	DBU	AR5,DTOLP
    // asm 00005B39: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROPTHEOTHER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SIDE_DOOR(void)
{
    // asm 00005B3A: 	LDI	@DOORLISTI,AR4
    // asm 00005B3B: 	SUBI	OLINK2,AR4
DOLS:
    // asm 00005B3C: LDI	*+AR4(OLINK2),R0
    // asm 00005B3D: 	BZ	DOLSX
    // asm 00005B3E: 	LDI	R0,AR4
    // asm 00005B3F: 	CMPI	@CT_MAN,AR4
    // asm 00005B40: 	BEQ	DOLS
    // asm 00005B41: 	CMPI	@CT_AUTO,AR4
    // asm 00005B42: 	BEQ	DOLS
    // asm 00005B43: 	LDI	*+AR4(OID),R0
    // asm 00005B44: 	ANDN	0FFh,R0
    // asm 00005B45: 	CMPI	100h,R0
    // asm 00005B46: 	BNE	NTLTS
    // asm 00005B47: 	LDF	*+AR4(OPOSX),R0
    // asm 00005B48: 	ADDF	15,R0
    // asm 00005B49: 	STF	R0,*+AR4(OPOSX)
    // asm 00005B4A: 	BU	DOLS
NTLTS:
    // asm 00005B4B: 	LDF	*+AR4(OPOSX),R0
    // asm 00005B4C: 	SUBF	15,R0
    // asm 00005B4D: 	STF	R0,*+AR4(OPOSX)
    // asm 00005B4E: 	BU	DOLS
DOLSX:
    // asm 00005B4F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIDE_DOOR", 0, 0);
    UNIMPL();
}

/* *
*
 */
void DOOR_OPENING(void)
{
    // asm 00005B54: 	LDF	@DOORTHETA,R2
    // asm 00005B55: 	LDI	@MATRIXAI,AR2
    // asm 00005B56: 	CALL	FIND_YMATRIX
    // asm 00005B57: 	NEGF	@DOORTHETA,R2
    // asm 00005B58: 	LDI	@MATRIXBI,AR2
    // asm 00005B59: 	CALL	FIND_YMATRIX
    // asm 00005B5A: 	LDI	@DOORLISTI,AR4
    // asm 00005B5B: 	SUBI	OLINK2,AR4
DOL:
    // asm 00005B5C: LDI	*+AR4(OLINK2),R0
    // asm 00005B5D: 	BZ	DOLX
    // asm 00005B5E: 	LDI	R0,AR4
    // asm 00005B5F: 	CALL	CLR_VECTORA
    // asm 00005B60: 	LDI	*+AR4(OID),R0
    // asm 00005B61: 	ANDN	0FFh,R0
    // asm 00005B62: 	CMPI	100h,R0
    // asm 00005B63: 	BNE	NOTLEFT
    // asm 00005B64: 	LDF	*+AR4(OVELX),R0
    // asm 00005B65: 	SUBF	@DO_LPPX,R0
    // asm 00005B66: 	STF	R0,*+AR2(X)
    // asm 00005B67: 	LDF	*+AR4(OVELZ),R0
    // asm 00005B68: 	SUBF	@DO_PPZ,R0
    // asm 00005B69: 	STF	R0,*+AR2(Z)
    // asm 00005B6A: 	LDI	@MATRIXAI,R2
    // asm 00005B6B: 	LDI	AR2,R3
    // asm 00005B6C: 	CALL	MATRIX_MUL
    // asm 00005B6D: 	LDF	*+AR2(X),R0
    // asm 00005B6E: 	ADDF	@DO_LPPX,R0
    // asm 00005B6F: 	STF	R0,*+AR4(OPOSX)
    // asm 00005B70: 	LDF	*+AR2(Z),R0
    // asm 00005B71: 	ADDF	@DO_PPZ,R0
    // asm 00005B72: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005B73: 	LDI	@MATRIXAI,R2
    // asm 00005B74: 	LDI	AR4,AR2
    // asm 00005B75: 	ADDI	OMATRIX,AR2
    // asm 00005B76: 	CALL	CPYMAT
    // asm 00005B77: 	BU	KJL
NOTLEFT:
    // asm 00005B78: 	LDF	*+AR4(OVELX),R0
    // asm 00005B79: 	SUBF	@DO_RPPX,R0
    // asm 00005B7A: 	STF	R0,*+AR2(X)
    // asm 00005B7B: 	LDF	*+AR4(OVELZ),R0
    // asm 00005B7C: 	SUBF	@DO_PPZ,R0
    // asm 00005B7D: 	STF	R0,*+AR2(Z)
    // asm 00005B7E: 	LDI	@MATRIXBI,R2
    // asm 00005B7F: 	LDI	AR2,R3
    // asm 00005B80: 	CALL	MATRIX_MUL
    // asm 00005B81: 	LDF	*+AR2(X),R0
    // asm 00005B82: 	ADDF	@DO_RPPX,R0
    // asm 00005B83: 	STF	R0,*+AR4(OPOSX)
    // asm 00005B84: 	LDF	*+AR2(Z),R0
    // asm 00005B85: 	ADDF	@DO_PPZ,R0
    // asm 00005B86: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005B87: 	LDI	@MATRIXBI,R2
    // asm 00005B88: 	LDI	AR4,AR2
    // asm 00005B89: 	ADDI	OMATRIX,AR2
    // asm 00005B8A: 	CALL	CPYMAT
KJL:
    // asm 00005B8B: 	BU	DOL
DOLX:
    // asm 00005B8C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOOR_OPENING", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void SNAPCURSOR(void)
{
    // asm 00005B8D: 	LDI	@CT_CURSOR,AR2
    // asm 00005B8E: 	CALL	DOOR_ELEMENT_DELETE
    // asm 00005B8F: 	CALL	OBJ_DELETE
    // asm 00005B90: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SNAPCURSOR", 0, 0);
    UNIMPL();
}

void WHEEL_ROUT(void)
{
    // asm 00005B91: 	CALL	GET_UNIT_WHEEL
    // asm 00005B92: 	LDI	@CHOSEN_TRANSMISSION,R1
    // asm 00005B93: 	LDI	@CT_CURSOR,AR2
    // asm 00005B94: 	CMPF	0.1,R0
    // asm 00005B95: 	LDFLT	0.0,R0
    // asm 00005B96: 	CMPF	0.9,R0
    // asm 00005B97: 	LDFGT	1.0,R0
    // asm 00005B98: 	CMPF	0.5,R0
    // asm 00005B99: 	BLT	ISL
    // asm 00005B9A: 	CMPF	0.55,R0
    // asm 00005B9B: 	BGT	ISR
    // asm 00005B9C: 	CMPI	AUTO_TRANSMISSION,R1
    // asm 00005B9D: 	BEQ	ISLT
ISR:
    // asm 00005B9E: LDF	*+AR2(OPOSX),R2
    // asm 00005B9F: 	LDF	101,R0
    // asm 00005BA0: 	SUBF	R2,R0
    // asm 00005BA1: 	MPYF	0.22,R0
    // asm 00005BA2: 	ADDF	*+AR2(OPOSX),R0
    // asm 00005BA3: 	STF	R0,*+AR2(OPOSX)
    // asm 00005BA4: 	LDI	MANUAL_TRANSMISSION,R0
    // asm 00005BA5: 	STI	R0,@CHOSEN_TRANSMISSION
    // asm 00005BA6: 	BU	ISDN2
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
    // asm 00005BB1: 	LDI	AUTO_TRANSMISSION,R0
    // asm 00005BB2: 	STI	R0,@CHOSEN_TRANSMISSION
ISDN2:
    // asm 00005BB3: 	LDF	@STEERMN,R1
    // asm 00005BB4: 	LDF	@STEERMX,R2
    // asm 00005BB5: 	SUBF	R1,R2,R3
    // asm 00005BB6: 	MPYF	0.2,R3
    // asm 00005BB7: 	ADDF	R3,R1
    // asm 00005BB8: 	SUBF	R3,R2
    // asm 00005BB9: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005BBA: 	CMPI	AUTO_TRANSMISSION,R0
    // asm 00005BBB: 	LDFEQ	R1,R0
    // asm 00005BBC: 	LDFNE	R2,R0
    // asm 00005BBD: 	STF	R0,@WHEELPOS
    // asm 00005BBE: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00005BBF: 	LDI	@LASTCHOICEA,R1
    // asm 00005BC0: 	CMPI	R0,R1
    // asm 00005BC1: 	BEQ	NUTHIN
    // asm 00005BC2: 	STI	R0,@LASTCHOICEA
    // asm 00005BC3: 	CMPI	AUTO_TRANSMISSION,R0
    // asm 00005BC4: 	LDIEQ	GSWIPE1,AR2
    // asm 00005BC5: 	LDINE	GSWIPE2,AR2
    // asm 00005BC6: 	CALL	ONESNDFX
NUTHIN:
    // asm 00005BC7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WHEEL_ROUT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void DOOR_ELEMENT_DELETE_ALL(void)
{
    // asm 00005BC8: 	LDI	@DOORLIST,R0
DEDL:
    // asm 00005BC9: LDI	R0,AR0
    // asm 00005BCA: 	CMPI	0,AR0
    // asm 00005BCB: 	BEQ	DEDX
    // asm 00005BCC: 	LDI	*+AR0(OLINK2),R0
    // asm 00005BCD: 	LDI	AR0,AR2
    // asm 00005BCE: 	CALL	OBJ_DELETE
    // asm 00005BCF: 	BU	DEDL
DEDX:
    // asm 00005BD0: 	CLRI	R0
    // asm 00005BD1: 	STI	R0,@DOORLIST
    // asm 00005BD2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOOR_ELEMENT_DELETE_ALL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	AR2	ELEMENT TO DELETE
*
 */
void DOOR_ELEMENT_DELETE(void)
{
    // asm 00005BD3: 	PUSH	R0
    // asm 00005BD4: 	PUSH	AR1
    // asm 00005BD5: 	LDI	@DOORLISTI,R0
    // asm 00005BD6: 	SUBI	OLINK2,R0		;(we are offset pointing)
DELLP:
    // asm 00005BD7: LDI	R0,AR1
    // asm 00005BD8: 	LDI	*+AR1(OLINK2),R0
    // asm 00005BD9: 	BZ	FREEDE_X
    // asm 00005BDA: 	CMPI	R0,AR2
    // asm 00005BDB: 	BNE	DELLP
    // asm 00005BDC: 	LDI	*+AR2(OLINK2),R0
    // asm 00005BDD: 	STI	R0,*+AR1(OLINK2)	;LINK AROUND
FREEDE_X:
    // asm 00005BDE: 	POP	AR1
    // asm 00005BDF: 	POP	R0
    // asm 00005BE0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOOR_ELEMENT_DELETE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TRANSCHOICE(void)
{
    // asm 00005BE1: 	CALL	GET_UNIT_WHEEL
    // asm 00005BE2: 	LDF	PI,R2
    // asm 00005BE3: 	MPYF	R0,R2
    // asm 00005BE4: 	SUBF	HALFPI,R2
    // asm 00005BE5: 	NEGF	R2
    // asm 00005BE6: 	LDI	@CT_WHEEL,AR2
    // asm 00005BE7: 	ADDI	OMATRIX,AR2
    // asm 00005BE8: 	CALL	FIND_ZMATRIX
    // asm 00005BE9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRANSCHOICE", 0, 0);
    UNIMPL();
}

void GET_UNIT_WHEEL(void)
{
    // asm 00005BEA: 	FLOAT	@_pot0,R0
    // asm 00005BEB: 	LDF	@GUWP,R1
    // asm 00005BEC: 	MPYF	0.2,R0
    // asm 00005BED: 	MPYF	0.8,R1
    // asm 00005BEE: 	ADDF	R1,R0
    // asm 00005BEF: 	STF	R0,@GUWP
    // asm 00005BF0: 	SUBF	@STEERMN,R0
    // asm 00005BF1: 	LDF	@STEERMX,R1
    // asm 00005BF2: 	SUBF	@STEERMN,R1
    // asm 00005BF3: 	CALL	DIV_F30
    // asm 00005BF4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_UNIT_WHEEL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void TILE_PIECES(void)
{
    // asm 00005BF5: 	SLEEP	10
    // asm 00005BF7: 	LDI	29,AR5
    // asm 00005BF8: TP_LP
    // asm 00005BF8: 	LDI	@CT_AUTO,AR4
    // asm 00005BF9: 	LDF	*+AR4(ORADZ),R2
    // asm 00005BFA: 	ADDF	0.3,R2
    // asm 00005BFB: 	STF	R2,*+AR4(ORADZ)
    // asm 00005BFC: 	LDI	AR4,AR2
    // asm 00005BFD: 	ADDI	OMATRIX,AR2
    // asm 00005BFE: 	CALL	FIND_ZMATRIX
    // asm 00005BFF: 	LDF	*+AR4(OPOSY),R0	;300, 400
    // asm 00005C00: 	SUBF	13.3,R0
    // asm 00005C01: 	STF	R0,*+AR4(OPOSY)
    // asm 00005C02: 	LDF	*+AR4(OPOSX),R0
    // asm 00005C03: 	SUBF	8,R0
    // asm 00005C04: 	STF	R0,*+AR4(OPOSX)
    // asm 00005C05: 	LDI	@CT_MAN,AR4
    // asm 00005C06: 	LDF	*+AR4(ORADZ),R2
    // asm 00005C07: 	SUBF	0.3,R2
    // asm 00005C08: 	STF	R2,*+AR4(ORADZ)
    // asm 00005C09: 	LDI	AR4,AR2
    // asm 00005C0A: 	ADDI	OMATRIX,AR2
    // asm 00005C0B: 	CALL	FIND_ZMATRIX
    // asm 00005C0C: 	LDF	*+AR4(OPOSY),R0
    // asm 00005C0D: 	SUBF	13.3,R0
    // asm 00005C0E: 	STF	R0,*+AR4(OPOSY)
    // asm 00005C0F: 	LDF	*+AR4(OPOSX),R0
    // asm 00005C10: 	ADDF	8,R0
    // asm 00005C11: 	STF	R0,*+AR4(OPOSX)
    // asm 00005C12: 	SLEEP	1
    // asm 00005C14: 	DBU	AR5,TP_LP
    // asm 00005C15: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TILE_PIECES", 0, 0);
    UNIMPL();
}

/* *
*
 */
void FIX_TRANSMISSION_SCREEN(void)
{
    // asm 00005C16: 	LDI	-1,R6
    // asm 00005C17: 	CLRI	R0
    // asm 00005C18: 	STI	R0,@DOORLIST
    // asm 00005C19: 	LDI	@OACTIVE,AR5
FTSL:
    // asm 00005C1A: LDI	AR5,R0
    // asm 00005C1B: 	BZ	FTSLX
    // asm 00005C1C: 	LDI	R0,AR0
    // asm 00005C1D: 	LDI	*AR0,AR5
    // asm 00005C1E: 	CMPI	-1,R6
    // asm 00005C1F: 	LDIEQ	AR0,R6
    // asm 00005C20: 	STI	AR5,*+AR0(OLINK2)
    // asm 00005C21: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005C22: 	ANDN	O_1PAL,R0
    // asm 00005C23: 	OR	O_NOUNIV|O_NOUROT,R0
    // asm 00005C24: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005C25: 	LDF	*+AR0(OPOSX),R0
    // asm 00005C26: 	STF	R0,*+AR0(OVELX)
    // asm 00005C27: 	LDI	*+AR0(OID),R0
    // asm 00005C28: 	ANDN	0FFh,R0
    // asm 00005C29: 	CMPI	100h,R0
    // asm 00005C2A: 	LDINE	600,R0
    // asm 00005C2B: 	LDIEQ	-600,R0
    // asm 00005C2C: 	FLOAT	R0
    // asm 00005C2D: 	LDF	*+AR0(OPOSX),R1
    // asm 00005C2E: 	ADDF	R1,R0
    // asm 00005C2F: 	STF	R0,*+AR0(OPOSX)
    // asm 00005C30: 	FIX	*+AR0(OPOSZ),R0
    // asm 00005C31: 	FLOAT	368,R1
    // asm 00005C32: 	STF	R1,*+AR0(OPOSZ)
    // asm 00005C33: 	STF	R1,*+AR0(OVELZ)
    // asm 00005C34: 	LDI	*+AR0(OID),R0
    // asm 00005C35: 	AND	0FFh,R0
    // asm 00005C36: 	CMPI	18h,R0
    // asm 00005C37: 	BNE	NOTFRNT
    // asm 00005C38: 	LDI	AR0,AR2
    // asm 00005C39: 	CALL	OBJ_PULL
    // asm 00005C3A: 	CALL	OBJ_INSERTP
    // asm 00005C3B: 	CALL	ADD_TO_DOOR_LIST
    // asm 00005C3C: 	BU	FTSL
NOTFRNT:
    // asm 00005C3D: 	CMPI	15h,R0	;THE WHEEL?
    // asm 00005C3E: 	BNE	NOT366
    // asm 00005C3F: 	STI	AR0,@CT_WHEEL
    // asm 00005C40: 	LDI	AR0,AR2
    // asm 00005C41: 	CALL	OBJ_PULL
    // asm 00005C42: 	CALL	OBJ_INSERTP
    // asm 00005C43: 	LDI	*+AR0(OID),R0
    // asm 00005C44: 	ANDN	0FFh,R0
    // asm 00005C45: 	CMPI	100h,R0
    // asm 00005C46: 	LDIEQ	600,R0
    // asm 00005C47: 	LDINE	-600,R0
    // asm 00005C48: 	FLOAT	R0
    // asm 00005C49: 	LDF	*+AR0(OPOSX),R1
    // asm 00005C4A: 	ADDF	R1,R0
    // asm 00005C4B: 	STF	R0,*+AR0(OPOSX)
    // asm 00005C4C: 	FLOAT	320,R0
    // asm 00005C4D: 	ADDF	*+AR0(OPOSY),R0
    // asm 00005C4E: 	STF	R0,*+AR0(OPOSY)
    // asm 00005C4F: 	BU	FTSL
NOT366:
    // asm 00005C50: 	CMPI	14h,R0	;CURSOR?
    // asm 00005C51: 	BNE	NOTCURSOR
    // asm 00005C52: 	STI	AR0,@CT_CURSOR
    // asm 00005C53: 	LDI	AR0,AR2
    // asm 00005C54: 	CALL	OBJ_PULL
    // asm 00005C55: 	CALL	OBJ_INSERTP
    // asm 00005C56: 	CALL	ADD_TO_DOOR_LIST
    // asm 00005C57: 	BU	FTSL
NOTCURSOR:
    // asm 00005C58: 	CMPI	10h,R0	;LEFT ENGINE
    // asm 00005C59: 	BNE	NOTLEN
    // asm 00005C5A: 	STI	AR0,@CT_LENG
    // asm 00005C5B: 	CALL	ADD_TO_DOOR_LIST
    // asm 00005C5C: 	BU	FTSL
NOTLEN:
    // asm 00005C5D: 	CMPI	11h,R0	;RIGHT ENGINE
    // asm 00005C5E: 	BNE	NOTREN
    // asm 00005C5F: 	STI	AR0,@CT_RENG
    // asm 00005C60: 	CALL	ADD_TO_DOOR_LIST
    // asm 00005C61: 	BU	FTSL
NOTREN:
    // asm 00005C62: 	CMPI	12h,R0	;AUTO BOX (LEFT)
    // asm 00005C63: 	BNE	NOTMAN
    // asm 00005C64: 	STI	AR0,@CT_MAN
    // asm 00005C65: 	LDI	AR0,AR2
    // asm 00005C66: 	CALL	OBJ_PULL
    // asm 00005C67: 	CALL	OBJ_INSERTP
    // asm 00005C68: 	FLOAT	-240,R1
    // asm 00005C69: 	LDF	*+AR0(OPOSX),R0
    // asm 00005C6A: 	ADDF	R1,R0
    // asm 00005C6B: 	STF	R0,*+AR0(OPOSX)
    // asm 00005C6C: 	LDF	*+AR0(OPOSY),R0
    // asm 00005C6D: 	FLOAT	400,R1
    // asm 00005C6E: 	ADDF	R1,R0
    // asm 00005C6F: 	STF	R0,*+AR0(OPOSY)
    // asm 00005C70: 	LDF	9.0,R2
    // asm 00005C71: 	STF	R2,*+AR0(ORADZ)
    // asm 00005C72: 	LDI	AR0,AR2
    // asm 00005C73: 	ADDI	OMATRIX,AR2
    // asm 00005C74: 	CALL	FIND_ZMATRIX
    // asm 00005C75: 	LDI	*+AR0(OID),R0
    // asm 00005C76: 	ANDN	0FFh,R0
    // asm 00005C77: 	CMPI	100h,R0
    // asm 00005C78: 	LDIEQ	600,R0
    // asm 00005C79: 	LDINE	-600,R0
    // asm 00005C7A: 	FLOAT	R0
    // asm 00005C7B: 	LDF	*+AR0(OPOSX),R1
    // asm 00005C7C: 	ADDF	R1,R0
    // asm 00005C7D: 	STF	R0,*+AR0(OPOSX)
    // asm 00005C7E: 	CALL	ADD_TO_DOOR_LIST
    // asm 00005C7F: 	BU	FTSL
NOTMAN:
    // asm 00005C80: 	CMPI	13h,R0	;MANUAL BOX (RIGHT)
    // asm 00005C81: 	BNE	NOTAUTO
    // asm 00005C82: 	STI	AR0,@CT_AUTO
    // asm 00005C83: 	LDI	AR0,AR2
    // asm 00005C84: 	CALL	OBJ_PULL
    // asm 00005C85: 	CALL	OBJ_INSERTP
    // asm 00005C86: 	FLOAT	240,R1
    // asm 00005C87: 	LDF	*+AR0(OPOSX),R0
    // asm 00005C88: 	ADDF	R1,R0
    // asm 00005C89: 	STF	R0,*+AR0(OPOSX)
    // asm 00005C8A: 	LDF	*+AR0(OPOSY),R0
    // asm 00005C8B: 	FLOAT	400,R1
    // asm 00005C8C: 	ADDF	R1,R0
    // asm 00005C8D: 	STF	R0,*+AR0(OPOSY)
    // asm 00005C8E: 	LDF	-9.0,R2
    // asm 00005C8F: 	STF	R2,*+AR0(ORADZ)
    // asm 00005C90: 	LDI	AR0,AR2
    // asm 00005C91: 	ADDI	OMATRIX,AR2
    // asm 00005C92: 	CALL	FIND_ZMATRIX
    // asm 00005C93: 	LDI	*+AR0(OID),R0
    // asm 00005C94: 	ANDN	0FFh,R0
    // asm 00005C95: 	CMPI	100h,R0
    // asm 00005C96: 	LDIEQ	600,R0
    // asm 00005C97: 	LDINE	-600,R0
    // asm 00005C98: 	FLOAT	R0
    // asm 00005C99: 	LDF	*+AR0(OPOSX),R1
    // asm 00005C9A: 	ADDF	R1,R0
    // asm 00005C9B: 	STF	R0,*+AR0(OPOSX)
    // asm 00005C9C: 	CALL	ADD_TO_DOOR_LIST
    // asm 00005C9D: 	BU	FTSL
NOTAUTO:
    // asm 00005C9E: 	CMPI	16h,R0	;PUSH TO CYCLE?
    // asm 00005C9F: 	BNE	NOTPTCYC
    // asm 00005CA0: 	STI	AR0,@CT_PUSHTOCYCLE
    // asm 00005CA1: 	LDI	AR0,AR2
    // asm 00005CA2: 	CALL	OBJ_PULL
    // asm 00005CA3: 	CALL	OBJ_INSERTP
    // asm 00005CA4: 	LDI	*+AR0(OID),R0
    // asm 00005CA5: 	ANDN	0FFh,R0
    // asm 00005CA6: 	CMPI	100h,R0
    // asm 00005CA7: 	LDIEQ	600,R0
    // asm 00005CA8: 	LDINE	-600,R0
    // asm 00005CA9: 	FLOAT	R0
    // asm 00005CAA: 	LDF	*+AR0(OPOSX),R1
    // asm 00005CAB: 	ADDF	R1,R0
    // asm 00005CAC: 	STF	R0,*+AR0(OPOSX)
    // asm 00005CAD: 	FLOAT	320,R0
    // asm 00005CAE: 	ADDF	*+AR0(OPOSY),R0
    // asm 00005CAF: 	STF	R0,*+AR0(OPOSY)
    // asm 00005CB0: 	LDI	AR0,AR4
    // asm 00005CB1: 	CREATE	CYCLE_PUSH,UTIL_C|CHOOSECAR_T
    // asm 00005CB4: 	BU	FTSL
NOTPTCYC:
    // asm 00005CB5: 	CMPI	17h,R0	;PUSH TO CYCLE?
    // asm 00005CB6: 	BNE	NOTWHELTOSEL
    // asm 00005CB7: 	STI	AR0,@CT_TURNTOSEL
    // asm 00005CB8: 	LDI	AR0,AR2
    // asm 00005CB9: 	CALL	OBJ_PULL
    // asm 00005CBA: 	CALL	OBJ_INSERTP
    // asm 00005CBB: 	LDI	*+AR0(OID),R0
    // asm 00005CBC: 	ANDN	0FFh,R0
    // asm 00005CBD: 	CMPI	100h,R0
    // asm 00005CBE: 	LDIEQ	600,R0
    // asm 00005CBF: 	LDINE	-600,R0
    // asm 00005CC0: 	FLOAT	R0
    // asm 00005CC1: 	LDF	*+AR0(OPOSX),R1
    // asm 00005CC2: 	ADDF	R1,R0
    // asm 00005CC3: 	STF	R0,*+AR0(OPOSX)
    // asm 00005CC4: 	FLOAT	320,R0
    // asm 00005CC5: 	ADDF	*+AR0(OPOSY),R0
    // asm 00005CC6: 	STF	R0,*+AR0(OPOSY)
    // asm 00005CC7: 	LDI	AR0,AR4
    // asm 00005CC8: 	CREATE	TURNTO_SELECT,UTIL_C|CHOOSECAR_T
    // asm 00005CCB: 	BU	FTSL
NOTWHELTOSEL:
    // asm 00005CCC: 	CALL	ADD_TO_DOOR_LIST
    // asm 00005CCD: 	BU	FTSL
FTSLX:
    // asm 00005CCE: 	STI	R6,@TRANS_HEAD
    // asm 00005CCF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIX_TRANSMISSION_SCREEN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MOVE_PUSH_BOX(void)
{
    // asm 00005CD0: 	LDI	@CT_TURNTOSEL,AR6
    // asm 00005CD1: 	LDI	@CT_PUSHTOCYCLE,AR4
    // asm 00005CD2: 	LDI	10,AR5
    // asm 00005CD3: MVPBL
    // asm 00005CD3: 	LDF	*+AR4(OPOSX),R0
    // asm 00005CD4: 	ADDF	4,R0
    // asm 00005CD5: 	STF	R0,*+AR4(OPOSX)
    // asm 00005CD6: 	LDF	*+AR4(OPOSY),R0
    // asm 00005CD7: 	ADDF	3,R0
    // asm 00005CD8: 	STF	R0,*+AR4(OPOSY)
    // asm 00005CD9: 	LDF	*+AR6(OPOSX),R0
    // asm 00005CDA: 	ADDF	-4,R0
    // asm 00005CDB: 	STF	R0,*+AR6(OPOSX)
    // asm 00005CDC: 	LDF	*+AR6(OPOSY),R0
    // asm 00005CDD: 	ADDF	3,R0
    // asm 00005CDE: 	STF	R0,*+AR6(OPOSY)
    // asm 00005CDF: 	SLEEP	1
    // asm 00005CE1: 	DBU	AR5,MVPBL
    // asm 00005CE2: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_PUSH_BOX", 0, 0);
    UNIMPL();
}

void TURNTO_SELECT(void)
{
    // asm 00005CEB: 	LDI	@TRNTABI,AR5
TURNLP:
    // asm 00005CEC: LDI	*AR5++,R0
    // asm 00005CED: 	CMPI	-1,R0
    // asm 00005CEE: 	BEQ	TURNTO_SELECT
    // asm 00005CEF: 	STI	R0,*+AR4(OROMDATA)
    // asm 00005CF0: 	SLEEP	5
    // asm 00005CF2: 	BU	TURNLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TURNTO_SELECT", 0, 0);
    UNIMPL();
}

void CYCLE_PUSH(void)
{
    // asm 00005CFB: 	LDI	@CYCTABI,AR5
    // asm 00005CFC: 	SLEEP	16
CYCLP:
    // asm 00005CFE: 	LDI	*AR5++,R0
    // asm 00005CFF: 	CMPI	-1,R0
    // asm 00005D00: 	BEQ	CYCLE_PUSH
    // asm 00005D01: 	STI	R0,*+AR4(OROMDATA)
    // asm 00005D02: 	LDI	@NFRAMES,R1
    // asm 00005D03: 	LDI	6,R0
    // asm 00005D04: 	CALL	DIV_I30
    // asm 00005D05: 	LDI	R0,AR2
    // asm 00005D06: 	CALL	PRC_SLEEP
    // asm 00005D07: 	BU	CYCLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CYCLE_PUSH", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	AR0	DOOR OBJECT
*
 */
void ADD_TO_DOOR_LIST(void)
{
    // asm 00005D08: 	LDI	@DOORLIST,R0
    // asm 00005D09: 	STI	R0,*+AR0(OLINK2)
    // asm 00005D0A: 	STI	AR0,@DOORLIST
    // asm 00005D0B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADD_TO_DOOR_LIST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
*
 */
void ENGINE_COLOR_INIT(void)
{
    // asm 00005D0C: 	LDI	@EPALR,AR0
    // asm 00005D0D: 	LDI	trmeng1_p,AR3
    // asm 00005D0E: 	ADDI	@PALROMI,AR3
    // asm 00005D0F: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
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
    // asm 00005D19: 	LDI	@EPALR,AR2
    // asm 00005D1A: 	CALL	PAL_ALLOC_RAW
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
    // asm 00005D28: 	LDI	@EPALL,AR2
    // asm 00005D29: 	CALL	PAL_ALLOC_RAW
    // asm 00005D2A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENGINE_COLOR_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
*PARAMETERS
*	R0	PERCENTAGE OF LEFT BRIGHTNESS
*
*
 */
void ENGINE_COLOR(void)
{
    // asm 00005D2B: 	PUSH	R5
    // asm 00005D2C: 	PUSHFL	R6
    // asm 00005D2E: 	PUSHFL	R7
    // asm 00005D30: 	LDF	R0,R7		;PERCENTAGE OF LEFT
    // asm 00005D31: 	CMPF	0.08,R0
    // asm 00005D32: 	LDFLT	0,R0
    // asm 00005D33: 	CMPF	0.92,R0
    // asm 00005D34: 	LDFGT	1.0,R0
    // asm 00005D35: 	LDI	@EPALL,AR0
    // asm 00005D36: 	LDI	trmeng1_p,AR3
    // asm 00005D37: 	ADDI	@PALROMI,AR3
    // asm 00005D38: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm 00005D39: 	LDL	trmeng1_chrome,AR2
    // asm 00005D3A: 	LDI	*AR3++,R0
    // asm 00005D3B: 	LDI	*AR2++,R0
    // asm 00005D3C: 	ANDN	0FFFh,R0
    // asm 00005D3D: 	ADDI	64,R0
    // asm 00005D3E: 	STI	R0,*AR0++
    // asm 00005D3F: 	LDI	31,AR5
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
    // asm 00005D95: 	DBU	AR5,L894
    // asm 00005D96: 	LDF	1.0,R0
    // asm 00005D97: 	SUBF	R7,R0,R7
    // asm 00005D98: 	LDI	@EPALL,AR2
    // asm 00005D99: 	CALL	PAL_FIND_RAW
    // asm 00005D9A: 	LDI	*AR2++,R3
    // asm 00005D9B: 	LDI	R0,R2
    // asm 00005D9C: 	CALL	PAL_SET
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
    // asm 00005DA7: 	LDI	31,AR5
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
    // asm 00005DFD: 	DBU	AR5,L894D
    // asm 00005DFE: 	LDI	@EPALR,AR2
    // asm 00005DFF: 	CALL	PAL_FIND_RAW
    // asm 00005E00: 	LDI	*AR2++,R3
    // asm 00005E01: 	LDI	R0,R2
    // asm 00005E02: 	CALL	PAL_SET
    // asm 00005E03: 	POPFL	R7
    // asm 00005E05: 	POPFL	R6
    // asm 00005E07: 	POP	R5
    // asm 00005E08: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENGINE_COLOR", 0, 0);
    UNIMPL();
}

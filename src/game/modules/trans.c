#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/obj.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/delta.h"
#include "../include/trans.h"
#include "../include/discovered_defines.h"

/*
 * Source module: asm/TRANS.ASM
 */

// *----------------------------------------------------------------------------
// *CHOOSE TRANSMISSION AND RELATED ROUTINES
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: EPALR	.word	EPALRR */
int EPALR = (int)(EPALRR);
/* asm: EPALL	.word	EPALRL */
int EPALL = (int)(EPALRL);
/* asm: EPALRR	.bss	EPALRR,129 */
int EPALRR[129];
/* asm: EPALRL	.bss	EPALRL,129 */
int EPALRL[129];
// *----------------------------------------------------------------------------
// *CHOOSE TRANSMISSION
// *
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *	OLINK2	- the link of the list
// *	OID	- 1 == left
// *		- 2 == right
// *
// *	OVEL?	- original position
// *
// *
// *
/* asm: DOORTHETA	.bss	DOORTHETA,1 */
int DOORTHETA;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *
// *
/* asm: LASTCHOICEA	.bss	LASTCHOICEA,1 */
int LASTCHOICEA;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *RETURNS
// *	R0	FL 0-1.0 Steering wheel position
// *
/* asm: GUWP	.bss	GUWP,1 */
int GUWP;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *Object Z values
// *	cursor		-3
// *	wheel		-2
// *	forward objects	-1
// *	others		0
// *
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
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: TRNTAB	.word	whel1,whel2,whel3,whel4,whel3,whel2,-1 */
int TRNTAB[] = { whel1, whel2, whel3, whel4, whel3, whel2, -1 };
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: CYCTAB	.word	trm3,trm2,trm1,trm2,trm3,trm4,-1 */
int CYCTAB[] = { trm3, trm2, trm1, trm2, trm3, trm4, -1 };
// *----------------------------------------------------------------------------

void CHOOSE_TRANSMISSION(void)
{
    // asm: 	CALL	CLEANUP_PALS
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	STI	R0,@NOSWAP
    // asm: 	LDI	MCT,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CALL	ENGINE_COLOR_INIT
    // asm: 	LDL	chostrns_grp,AR2
    // asm: 	CALL	LOAD_SINGLE_SECTION
    // asm: 	CALL	FIX_TRANSMISSION_SCREEN
    // asm: 	LDI	@CT_LENG,AR0
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	@CT_RENG,AR0
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDF	1,R0
    // asm: 	CALL	ENGINE_COLOR
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	STI	R0,@NOSWAP
    // asm: 	CREATE	TILE_PIECES,UTIL_C|CHOOSETRANS_T
    // asm: 	LDI	39,AR5
LGF7:
    // asm: CALL	SIDE_DOOR
    // asm: 	CALL	WHEEL_FROM_BELOW
    // asm: 	CALL	TRANSCHOICE
    // asm: 	PUSH	AR5
    // asm: 	LDF	1,R0
    // asm: 	CALL	ENGINE_COLOR
    // asm: 	POP	AR5
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,LGF7
    // asm: 	SONDFX	MHIT
    // asm: 	LDI	MCT|MGO|MINFIN|MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	STI	R0,@NOSWAP
    // asm: 	FLOAT	@_pot0,R0
    // asm: 	STF	R0,@GUWP
    // *
    // *CHOOSE TRANSMISSION LOOP
    // *
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_HIT
    // asm: 	LDI	10,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	CALL	INIT_PEDALCHK
CTLP:
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	CTLPX
    // asm: 	LDI	@CT_CURSOR,AR2
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	ADDF	101,R0
    // asm: 	FLOAT	204,R1
    // asm: 	CALL	DIV_F30
    // asm: 	CALL	ENGINE_COLOR
    // asm: 	CALL	TRANSCHOICE
    // asm: 	CALL	WHEEL_ROUT
    // 	;-------time remaining
    // 	;
    // asm: 	LDL	time,AR2
    // asm: 	LDI	242,R2			;R2	POS X
    // asm: 	LDI	337,R3			;R3	POS Y
    // asm: 	LDI	TM|ZS,R4
    // asm: 	CALL	BLTMOD2D
    // asm: 	CALL	PEDALCHK
    // asm: 	BC	CTLPX
    // asm: 	CALL	INTROTIMER
    // asm: 	SLEEP	1
    // asm: 	LDI	@_countdown,R0
    // asm: 	BGT	CTLP
CTLPX:
    // asm: 	SONDFX	TRANS
    // asm: 	LDF	@START_RADY,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	HPFIND_YMATRIX
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	-650,R0
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	FLOAT	-28000,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R2
    // asm: 	ADDF	@START_POS+X,R2
    // asm: 	LDP	@_CAMERAPOS+X
    // asm: 	STF	R2,@_CAMERAPOS+X
    // asm: 	SETDP
    // asm: 	LDF	*+AR2(Y),R2
    // asm: 	ADDF	@START_POS+Y,R2
    // asm: 	LDP	@_CAMERAPOS+Y
    // asm: 	STF	R2,@_CAMERAPOS+Y
    // asm: 	SETDP
    // asm: 	LDF	*+AR2(Z),R2
    // asm: 	ADDF	@START_POS+Z,R2
    // asm: 	LDP	@_CAMERAPOS+Z
    // asm: 	STF	R2,@_CAMERAPOS+Z
    // asm: 	SETDP
    // asm: 	CREATE	CENTERTHEONE,UTIL_C|CHOOSECAR_T
    // asm: 	CREATE	DROPTHEOTHER,UTIL_C|CHOOSETRANS_T
    // asm: 	CALL	SNAPCURSOR
    // asm: 	NEGF	@START_RADY,R2
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	STF	R2,@_CAMERARAD+Y
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	CALL	HPFIND_YMATRIX
    // asm: 	LDL	garage_grp,AR2
    // asm: 	CALL	LOAD_SINGLE_SECTION_OFFSET
    // asm: 	SLEEP	3
    // asm: 	SONDFX	DD1
    // asm: 	SLEEP	7
    // asm: 	CREATE	MOVE_PUSH_BOX,UTIL_C|CHOOSETRANS_T
    // asm: 	CALL	GETTHECARS
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@LASTCHOICE
    // asm: 	LDI	4,R0
    // asm: 	STI	R0,@POSES
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@POSE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CHOSEN_VEHICLE
    // asm: 	CREATE	ROUNDER,UTIL_C|CHOOSECAR_T
    // asm: 	CLRF	R0
    // asm: 	STF	R0,@DOORTHETA
    // asm: 	SONDFX	DOPEN
    // asm: 	JSRP	OPEN_DOOR_PROC
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHOOSE_TRANSMISSION", 0, 0);
    UNIMPL();
}

void WHEEL_FROM_BELOW(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *AS THE PIECES COME SLIDING IN FROM THE SIDES,
    // *THE WHEEL AND PUSH TO CHOOSE COME FROM BELOW
    // *
    // asm: 	LDI	@CT_WHEEL,AR0
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	SUBF	8,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDI	@CT_PUSHTOCYCLE,AR0
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	SUBF	8,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDI	@CT_TURNTOSEL,AR0
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	SUBF	8,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WHEEL_FROM_BELOW", 0, 0);
    UNIMPL();
}

void CENTERTHEONE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm: 	LDIEQ	AUD_MANUAL_TRANS_SELECTED,AR2
    // asm: 	LDINE	AUD_AUTOMATIC_TRANS_SELECTED,AR2
    // asm: 	CALL	AUDIT_INC
    // asm: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm: 	LDIEQ	@CT_MAN,AR4
    // asm: 	LDINE	@CT_AUTO,AR4
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	DOOR_ELEMENT_DELETE
    // asm: 	LDI	15,AR5
CTOLP:
    // asm: LDF	*+AR4(OPOSX),R0
    // asm: 	MPYF	0.8,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	MPYF	0.8,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,CTOLP
    // asm: 	LDF	1,R7
    // asm: 	LDF	1,R6
FLYUPL:
    // asm: 	LDF	*+AR4(ORADX),R2
    // asm: 	SUBF	0.1,R2
    // asm: 	STF	R2,*+AR4(ORADX)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	MPYF	1.4,R6
    // asm: 	MPYF	1.4,R7
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	SUBF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	SLEEP	1
    // asm: 	BU	FLYUPL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CENTERTHEONE", 0, 0);
    UNIMPL();
}

void DROPTHETURN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@CT_TURNTOSEL,AR4
    // asm: 	BU	J85
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROPTHETURN", 0, 0);
    UNIMPL();
}

void DROPTHECYCLE(void)
{
    // asm: 	LDI	@CT_PUSHTOCYCLE,AR4
    // asm: 	BU	J85
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROPTHECYCLE", 0, 0);
    UNIMPL();
}

void DROPTHEWHEEL(void)
{
    // asm: 	LDI	@CT_WHEEL,AR4
    // asm: 	BU	J85
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROPTHEWHEEL", 0, 0);
    UNIMPL();
}

void DROPTHEOTHER(void)
{
    // asm: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm: 	CMPI	MANUAL_TRANSMISSION,R0
    // asm: 	LDINE	@CT_MAN,AR4
    // asm: 	LDIEQ	@CT_AUTO,AR4
J85:
    // asm: LDI	AR4,AR2
    // asm: 	CALL	DOOR_ELEMENT_DELETE
J86:
    // asm: LDI	10,AR5
    // asm: 	LDF	2,R6
    // asm: DTOLP
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	R6,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	MPYF	2,R6
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,DTOLP
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROPTHEOTHER", 0, 0);
    UNIMPL();
}

void SIDE_DOOR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@DOORLISTI,AR4
    // asm: 	SUBI	OLINK2,AR4
DOLS:
    // asm: LDI	*+AR4(OLINK2),R0
    // asm: 	BZ	DOLSX
    // asm: 	LDI	R0,AR4
    // asm: 	CMPI	@CT_MAN,AR4
    // asm: 	BEQ	DOLS
    // asm: 	CMPI	@CT_AUTO,AR4
    // asm: 	BEQ	DOLS
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	CMPI	100h,R0
    // asm: 	BNE	NTLTS
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	15,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	BU	DOLS
NTLTS:
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	SUBF	15,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	BU	DOLS
DOLSX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIDE_DOOR", 0, 0);
    UNIMPL();
}

void DOORLISTI(void)
{
    // *
    // *
    /* no executable asm lines detected */
    UNIMPL();
}

void DOOR_OPENING(void)
{
    // *
    // *
    // asm: 	LDF	@DOORTHETA,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	NEGF	@DOORTHETA,R2
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	@DOORLISTI,AR4
    // asm: 	SUBI	OLINK2,AR4
DOL:
    // asm: LDI	*+AR4(OLINK2),R0
    // asm: 	BZ	DOLX
    // asm: 	LDI	R0,AR4
    // asm: 	CALL	CLR_VECTORA
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	CMPI	100h,R0
    // asm: 	BNE	NOTLEFT
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	SUBF	@DO_LPPX,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDF	*+AR4(OVELZ),R0
    // asm: 	SUBF	@DO_PPZ,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R0
    // asm: 	ADDF	@DO_LPPX,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	ADDF	@DO_PPZ,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	CPYMAT
    // asm: 	BU	KJL
NOTLEFT:
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	SUBF	@DO_RPPX,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDF	*+AR4(OVELZ),R0
    // asm: 	SUBF	@DO_PPZ,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXBI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R0
    // asm: 	ADDF	@DO_RPPX,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	ADDF	@DO_PPZ,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	@MATRIXBI,R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	CPYMAT
KJL:
    // asm: 	BU	DOL
DOLX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOOR_OPENING", 0, 0);
    UNIMPL();
}

void SNAPCURSOR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@CT_CURSOR,AR2
    // asm: 	CALL	DOOR_ELEMENT_DELETE
    // asm: 	CALL	OBJ_DELETE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SNAPCURSOR", 0, 0);
    UNIMPL();
}

void WHEEL_ROUT(void)
{
    // asm: 	CALL	GET_UNIT_WHEEL
    // asm: 	LDI	@CHOSEN_TRANSMISSION,R1
    // asm: 	LDI	@CT_CURSOR,AR2
    // asm: 	CMPF	0.1,R0
    // asm: 	LDFLT	0.0,R0
    // asm: 	CMPF	0.9,R0
    // asm: 	LDFGT	1.0,R0
    // asm: 	CMPF	0.5,R0
    // asm: 	BLT	ISL
    // asm: 	CMPF	0.55,R0
    // asm: 	BGT	ISR
    // asm: 	CMPI	AUTO_TRANSMISSION,R1
    // asm: 	BEQ	ISLT
ISR:
    // asm: LDF	*+AR2(OPOSX),R2
    // asm: 	LDF	101,R0
    // asm: 	SUBF	R2,R0
    // asm: 	MPYF	0.22,R0
    // asm: 	ADDF	*+AR2(OPOSX),R0
    // asm: 	STF	R0,*+AR2(OPOSX)
    // asm: 	LDI	MANUAL_TRANSMISSION,R0
    // asm: 	STI	R0,@CHOSEN_TRANSMISSION
    // asm: 	BU	ISDN2
ISL:
    // asm: 	CMPF	0.45,R0
    // asm: 	BLT	ISLT
    // asm: 	CMPI	MANUAL_TRANSMISSION,R1
    // asm: 	BEQ	ISR
ISLT:
    // asm: LDF	*+AR2(OPOSX),R2
    // asm: 	LDF	-101,R0
    // asm: 	SUBF	R2,R0
    // asm: 	MPYF	0.22,R0
    // asm: 	ADDF	*+AR2(OPOSX),R0
    // asm: 	STF	R0,*+AR2(OPOSX)
    // asm: 	LDI	AUTO_TRANSMISSION,R0
    // asm: 	STI	R0,@CHOSEN_TRANSMISSION
ISDN2:
    // asm: 	LDF	@STEERMN,R1
    // asm: 	LDF	@STEERMX,R2
    // asm: 	SUBF	R1,R2,R3
    // asm: 	MPYF	0.2,R3
    // asm: 	ADDF	R3,R1
    // asm: 	SUBF	R3,R2
    // asm: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm: 	CMPI	AUTO_TRANSMISSION,R0
    // asm: 	LDFEQ	R1,R0
    // asm: 	LDFNE	R2,R0
    // asm: 	STF	R0,@WHEELPOS
    // asm: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm: 	LDI	@LASTCHOICEA,R1
    // asm: 	CMPI	R0,R1
    // asm: 	BEQ	NUTHIN
    // asm: 	STI	R0,@LASTCHOICEA
    // asm: 	CMPI	AUTO_TRANSMISSION,R0
    // asm: 	LDIEQ	GSWIPE1,AR2
    // asm: 	LDINE	GSWIPE2,AR2
    // asm: 	CALL	ONESNDFX
NUTHIN:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WHEEL_ROUT", 0, 0);
    UNIMPL();
}

void DOOR_ELEMENT_DELETE_ALL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@DOORLIST,R0
DEDL:
    // asm: LDI	R0,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BEQ	DEDX
    // asm: 	LDI	*+AR0(OLINK2),R0
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	BU	DEDL
DEDX:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@DOORLIST
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOOR_ELEMENT_DELETE_ALL", 0, 0);
    UNIMPL();
}

void DOOR_ELEMENT_DELETE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	AR2	ELEMENT TO DELETE
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR1
    // asm: 	LDI	@DOORLISTI,R0
    // asm: 	SUBI	OLINK2,R0		;(we are offset pointing)
DELLP:
    // asm: LDI	R0,AR1
    // asm: 	LDI	*+AR1(OLINK2),R0
    // asm: 	BZ	FREEDE_X
    // asm: 	CMPI	R0,AR2
    // asm: 	BNE	DELLP
    // asm: 	LDI	*+AR2(OLINK2),R0
    // asm: 	STI	R0,*+AR1(OLINK2)	;LINK AROUND
FREEDE_X:
    // asm: 	POP	AR1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOOR_ELEMENT_DELETE", 0, 0);
    UNIMPL();
}

void TRANSCHOICE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	GET_UNIT_WHEEL
    // asm: 	LDF	PI,R2
    // asm: 	MPYF	R0,R2
    // asm: 	SUBF	HALFPI,R2
    // asm: 	NEGF	R2
    // asm: 	LDI	@CT_WHEEL,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRANSCHOICE", 0, 0);
    UNIMPL();
}

void GET_UNIT_WHEEL(void)
{
    // asm: 	FLOAT	@_pot0,R0
    // asm: 	LDF	@GUWP,R1
    // asm: 	MPYF	0.2,R0
    // asm: 	MPYF	0.8,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,@GUWP
    // asm: 	SUBF	@STEERMN,R0
    // asm: 	LDF	@STEERMX,R1
    // asm: 	SUBF	@STEERMN,R1
    // asm: 	CALL	DIV_F30
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_UNIT_WHEEL", 0, 0);
    UNIMPL();
}

void TILE_PIECES(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	SLEEP	10
    // asm: 	LDI	29,AR5
    // asm: TP_LP
    // asm: 	LDI	@CT_AUTO,AR4
    // asm: 	LDF	*+AR4(ORADZ),R2
    // asm: 	ADDF	0.3,R2
    // asm: 	STF	R2,*+AR4(ORADZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	LDF	*+AR4(OPOSY),R0	;300, 400
    // asm: 	SUBF	13.3,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	SUBF	8,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDI	@CT_MAN,AR4
    // asm: 	LDF	*+AR4(ORADZ),R2
    // asm: 	SUBF	0.3,R2
    // asm: 	STF	R2,*+AR4(ORADZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	13.3,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	8,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,TP_LP
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TILE_PIECES", 0, 0);
    UNIMPL();
}

void FIX_TRANSMISSION_SCREEN(void)
{
    // *
    // *
    // asm: 	LDI	-1,R6
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@DOORLIST
    // asm: 	LDI	@OACTIVE,AR5
FTSL:
    // asm: LDI	AR5,R0
    // asm: 	BZ	FTSLX
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR5
    // asm: 	CMPI	-1,R6
    // asm: 	LDIEQ	AR0,R6
    // asm: 	STI	AR5,*+AR0(OLINK2)
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	OR	O_NOUNIV|O_NOUROT,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OVELX)
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	CMPI	100h,R0
    // asm: 	LDINE	600,R0
    // asm: 	LDIEQ	-600,R0
    // asm: 	FLOAT	R0
    // asm: 	LDF	*+AR0(OPOSX),R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FIX	*+AR0(OPOSZ),R0
    // asm: 	FLOAT	368,R1
    // asm: 	STF	R1,*+AR0(OPOSZ)
    // asm: 	STF	R1,*+AR0(OVELZ)
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	AND	0FFh,R0
    // asm: 	CMPI	18h,R0
    // asm: 	BNE	NOTFRNT
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	CALL	ADD_TO_DOOR_LIST
    // asm: 	BU	FTSL
NOTFRNT:
    // asm: 	CMPI	15h,R0	;THE WHEEL?
    // asm: 	BNE	NOT366
    // asm: 	STI	AR0,@CT_WHEEL
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	CMPI	100h,R0
    // asm: 	LDIEQ	600,R0
    // asm: 	LDINE	-600,R0
    // asm: 	FLOAT	R0
    // asm: 	LDF	*+AR0(OPOSX),R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	320,R0
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	BU	FTSL
NOT366:
    // asm: 	CMPI	14h,R0	;CURSOR?
    // asm: 	BNE	NOTCURSOR
    // asm: 	STI	AR0,@CT_CURSOR
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	CALL	ADD_TO_DOOR_LIST
    // asm: 	BU	FTSL
NOTCURSOR:
    // asm: 	CMPI	10h,R0	;LEFT ENGINE
    // asm: 	BNE	NOTLEN
    // asm: 	STI	AR0,@CT_LENG
    // asm: 	CALL	ADD_TO_DOOR_LIST
    // asm: 	BU	FTSL
NOTLEN:
    // asm: 	CMPI	11h,R0	;RIGHT ENGINE
    // asm: 	BNE	NOTREN
    // asm: 	STI	AR0,@CT_RENG
    // asm: 	CALL	ADD_TO_DOOR_LIST
    // asm: 	BU	FTSL
NOTREN:
    // asm: 	CMPI	12h,R0	;AUTO BOX (LEFT)
    // asm: 	BNE	NOTMAN
    // asm: 	STI	AR0,@CT_MAN
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	FLOAT	-240,R1
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	FLOAT	400,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	9.0,R2
    // asm: 	STF	R2,*+AR0(ORADZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	CMPI	100h,R0
    // asm: 	LDIEQ	600,R0
    // asm: 	LDINE	-600,R0
    // asm: 	FLOAT	R0
    // asm: 	LDF	*+AR0(OPOSX),R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	CALL	ADD_TO_DOOR_LIST
    // asm: 	BU	FTSL
NOTMAN:
    // asm: 	CMPI	13h,R0	;MANUAL BOX (RIGHT)
    // asm: 	BNE	NOTAUTO
    // asm: 	STI	AR0,@CT_AUTO
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	FLOAT	240,R1
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	FLOAT	400,R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	-9.0,R2
    // asm: 	STF	R2,*+AR0(ORADZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	CMPI	100h,R0
    // asm: 	LDIEQ	600,R0
    // asm: 	LDINE	-600,R0
    // asm: 	FLOAT	R0
    // asm: 	LDF	*+AR0(OPOSX),R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	CALL	ADD_TO_DOOR_LIST
    // asm: 	BU	FTSL
NOTAUTO:
    // asm: 	CMPI	16h,R0	;PUSH TO CYCLE?
    // asm: 	BNE	NOTPTCYC
    // asm: 	STI	AR0,@CT_PUSHTOCYCLE
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	CMPI	100h,R0
    // asm: 	LDIEQ	600,R0
    // asm: 	LDINE	-600,R0
    // asm: 	FLOAT	R0
    // asm: 	LDF	*+AR0(OPOSX),R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	320,R0
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDI	AR0,AR4
    // asm: 	CREATE	CYCLE_PUSH,UTIL_C|CHOOSECAR_T
    // asm: 	BU	FTSL
NOTPTCYC:
    // asm: 	CMPI	17h,R0	;PUSH TO CYCLE?
    // asm: 	BNE	NOTWHELTOSEL
    // asm: 	STI	AR0,@CT_TURNTOSEL
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	CMPI	100h,R0
    // asm: 	LDIEQ	600,R0
    // asm: 	LDINE	-600,R0
    // asm: 	FLOAT	R0
    // asm: 	LDF	*+AR0(OPOSX),R1
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	320,R0
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDI	AR0,AR4
    // asm: 	CREATE	TURNTO_SELECT,UTIL_C|CHOOSECAR_T
    // asm: 	BU	FTSL
NOTWHELTOSEL:
    // asm: 	CALL	ADD_TO_DOOR_LIST
    // asm: 	BU	FTSL
FTSLX:
    // asm: 	STI	R6,@TRANS_HEAD
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIX_TRANSMISSION_SCREEN", 0, 0);
    UNIMPL();
}

void MOVE_PUSH_BOX(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@CT_TURNTOSEL,AR6
    // asm: 	LDI	@CT_PUSHTOCYCLE,AR4
    // asm: 	LDI	10,AR5
    // asm: MVPBL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	4,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR6(OPOSX),R0
    // asm: 	ADDF	-4,R0
    // asm: 	STF	R0,*+AR6(OPOSX)
    // asm: 	LDF	*+AR6(OPOSY),R0
    // asm: 	ADDF	3,R0
    // asm: 	STF	R0,*+AR6(OPOSY)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,MVPBL
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_PUSH_BOX", 0, 0);
    UNIMPL();
}

void TURNTO_SELECT(void)
{
    // asm: 	LDI	@TRNTABI,AR5
TURNLP:
    // asm: LDI	*AR5++,R0
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	TURNTO_SELECT
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	SLEEP	5
    // asm: 	BU	TURNLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TURNTO_SELECT", 0, 0);
    UNIMPL();
}

void CYCLE_PUSH(void)
{
    // asm: 	LDI	@CYCTABI,AR5
    // asm: 	SLEEP	16
CYCLP:
    // asm: 	LDI	*AR5++,R0
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	CYCLE_PUSH
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	LDI	@NFRAMES,R1
    // asm: 	LDI	6,R0
    // asm: 	CALL	DIV_I30
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PRC_SLEEP
    // asm: 	BU	CYCLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CYCLE_PUSH", 0, 0);
    UNIMPL();
}

void ADD_TO_DOOR_LIST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *PARAMETERS
    // *	AR0	DOOR OBJECT
    // *
    // asm: 	LDI	@DOORLIST,R0
    // asm: 	STI	R0,*+AR0(OLINK2)
    // asm: 	STI	AR0,@DOORLIST
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADD_TO_DOOR_LIST", 0, 0);
    UNIMPL();
}

void ENGINE_COLOR_INIT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // *
    // asm: 	LDI	@EPALR,AR0
    // asm: 	LDI	trmeng1_p,AR3
    // asm: 	ADDI	@PALROMI,AR3
    // asm: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm: 	LDI	*AR3++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	R0,AR2
    // asm: 	AND	0FFFh,AR2
    // asm: 	RS	1,AR2
    // asm: 	SUBI	1,AR2
L342:
    // asm: LDI	*AR3++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	DBU	AR2,L342
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDI	@EPALL,AR0
    // asm: 	LDI	trmeng1_p,AR3
    // asm: 	ADDI	@PALROMI,AR3
    // asm: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm: 	LDI	*AR3++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	R0,AR2
    // asm: 	AND	0FFFh,AR2
    // asm: 	RS	1,AR2
    // asm: 	SUBI	1,AR2
L343:
    // asm: LDI	*AR3++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	DBU	AR2,L343
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENGINE_COLOR_INIT", 0, 0);
    UNIMPL();
}

void ENGINE_COLOR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // *PARAMETERS
    // *	R0	PERCENTAGE OF LEFT BRIGHTNESS
    // *
    // *
    // asm: 	PUSH	R5
    // asm: 	PUSHFL	R6
    // asm: 	PUSHFL	R7
    // asm: 	LDF	R0,R7		;PERCENTAGE OF LEFT
    // asm: 	CMPF	0.08,R0
    // asm: 	LDFLT	0,R0
    // asm: 	CMPF	0.92,R0
    // asm: 	LDFGT	1.0,R0
    // asm: 	LDI	@EPALL,AR0
    // asm: 	LDI	trmeng1_p,AR3
    // asm: 	ADDI	@PALROMI,AR3
    // asm: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm: 	LDL	trmeng1_chrome,AR2
    // asm: 	LDI	*AR3++,R0
    // asm: 	LDI	*AR2++,R0
    // asm: 	ANDN	0FFFh,R0
    // asm: 	ADDI	64,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	31,AR5
    // asm: L894
    // asm: 	LDI	*AR3++,R4	;get src2 (TRUE)
    // asm: 	LDI	R4,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	*AR2++,R0	;get src2 (TPAL)
    // asm: 	LDI	R0,R1
    // asm: 	AND	01Fh,R1
    // asm: 	FLOAT	R1
    // asm: 	SUBF	R5,R1,R6	;TPAL-TRUE->C
    // asm: 	MPYF	R7,R6		;C*UNIT
    // asm: 	ADDF	R5,R6,R1	;C+A->D
    // asm: 	FIX	R1
    // asm: 	LDI	R4,R5		;TRUE
    // asm: 	RS	5,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R2		;TPAL
    // asm: 	RS	5,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	SUBF	R5,R2,R6	;TRUE-TPAL->C
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R2
    // asm: 	FIX	R2
    // asm: 	LS	5,R2
    // asm: 	OR	R2,R1
    // asm: 	LDI	R4,R5		;TRUE
    // asm: 	RS	10,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R2		;TPAL
    // asm: 	RS	10,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	SUBF	R5,R2,R6	;TRUE-TPAL
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R2
    // asm: 	FIX	R2
    // asm: 	LS	10,R2
    // asm: 	OR	R2,R1
    // asm: 	PUSH	R1
    // 	;----second word----------
    // asm: 	RS	16,R4		;get src1(B)
    // asm: 	RS	16,R0		;get src2(B)
    // asm: 	LDI	R4,R5		;TRUE
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R1		;TPAL
    // asm: 	AND	01Fh,R1
    // asm: 	FLOAT	R1
    // asm: 	SUBF	R5,R1,R6	;TPAL-TRUE
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R1
    // asm: 	FIX	R1
    // asm: 	LDI	R4,R5		;--
    // asm: 	RS	5,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R2
    // asm: 	RS	5,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	SUBF	R5,R2,R6
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R2
    // asm: 	FIX	R2
    // asm: 	LS	5,R2
    // asm: 	OR	R2,R1
    // asm: 	LDI	R4,R5		;--
    // asm: 	RS	10,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R2
    // asm: 	RS	10,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	SUBF	R5,R2,R6
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R2
    // asm: 	FIX	R2
    // asm: 	LS	10,R2
    // asm: 	OR	R2,R1
    // asm: 	POP	R4
    // asm: 	LS	16,R1
    // asm: 	OR	R4,R1
    // asm: 	STI	R1,*AR0++
    // asm: 	DBU	AR5,L894
    // asm: 	LDF	1.0,R0
    // asm: 	SUBF	R7,R0,R7
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	*AR2++,R3
    // asm: 	LDI	R0,R2
    // asm: 	CALL	PAL_SET
    // *
    // *
    // *
    // *
    // *
    // asm: 	LDI	@EPALR,AR0
    // asm: 	LDI	trmeng1_p,AR3
    // asm: 	ADDI	@PALROMI,AR3
    // asm: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm: 	LDL	trmeng1_chrome,AR2
    // asm: 	LDI	*AR3++,R0
    // asm: 	LDI	*AR2++,R0
    // asm: 	ANDN	0FFFh,R0
    // asm: 	ADDI	64,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	31,AR5
    // asm: L894D
    // asm: 	LDI	*AR3++,R4	;get src2 (TRUE)
    // asm: 	LDI	R4,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	*AR2++,R0	;get src2 (TPAL)
    // asm: 	LDI	R0,R1
    // asm: 	AND	01Fh,R1
    // asm: 	FLOAT	R1
    // asm: 	SUBF	R5,R1,R6	;TPAL-TRUE->C
    // asm: 	MPYF	R7,R6		;C*UNIT
    // asm: 	ADDF	R5,R6,R1	;C+A->D
    // asm: 	FIX	R1
    // asm: 	LDI	R4,R5		;TRUE
    // asm: 	RS	5,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R2		;TPAL
    // asm: 	RS	5,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	SUBF	R5,R2,R6	;TRUE-TPAL->C
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R2
    // asm: 	FIX	R2
    // asm: 	LS	5,R2
    // asm: 	OR	R2,R1
    // asm: 	LDI	R4,R5		;TRUE
    // asm: 	RS	10,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R2		;TPAL
    // asm: 	RS	10,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	SUBF	R5,R2,R6	;TRUE-TPAL
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R2
    // asm: 	FIX	R2
    // asm: 	LS	10,R2
    // asm: 	OR	R2,R1
    // asm: 	PUSH	R1
    // 	;----second word----------
    // asm: 	RS	16,R4		;get src1(B)
    // asm: 	RS	16,R0		;get src2(B)
    // asm: 	LDI	R4,R5		;TRUE
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R1		;TPAL
    // asm: 	AND	01Fh,R1
    // asm: 	FLOAT	R1
    // asm: 	SUBF	R5,R1,R6	;TPAL-TRUE
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R1
    // asm: 	FIX	R1
    // asm: 	LDI	R4,R5		;--
    // asm: 	RS	5,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R2
    // asm: 	RS	5,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	SUBF	R5,R2,R6
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R2
    // asm: 	FIX	R2
    // asm: 	LS	5,R2
    // asm: 	OR	R2,R1
    // asm: 	LDI	R4,R5		;--
    // asm: 	RS	10,R5
    // asm: 	AND	01Fh,R5
    // asm: 	FLOAT	R5
    // asm: 	LDI	R0,R2
    // asm: 	RS	10,R2
    // asm: 	AND	01Fh,R2
    // asm: 	FLOAT	R2
    // asm: 	SUBF	R5,R2,R6
    // asm: 	MPYF	R7,R6
    // asm: 	ADDF	R5,R6,R2
    // asm: 	FIX	R2
    // asm: 	LS	10,R2
    // asm: 	OR	R2,R1
    // asm: 	POP	R4
    // asm: 	LS	16,R1
    // asm: 	OR	R4,R1
    // asm: 	STI	R1,*AR0++
    // asm: 	DBU	AR5,L894D
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	*AR2++,R3
    // asm: 	LDI	R0,R2
    // asm: 	CALL	PAL_SET
    // asm: 	POPFL	R7
    // asm: 	POPFL	R6
    // asm: 	POP	R5
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENGINE_COLOR", 0, 0);
    UNIMPL();
}

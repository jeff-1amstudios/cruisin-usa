#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/mproc.h"
#include "../include/macs.h"
#include "../include/obj.h"
#include "../include/objects.h"
#include "../include/globals.h"
#include "../include/sys.h"
#include "../include/text.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sndtab.h"

/*
 * Source module: asm/ATTRACTA.ASM
 */

// *----------------------------------------------------------------------------
// *ATTRACT MODE
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *
#define NOVANITY 1
// *----------------------------------------------------------------------------
#define startthey 4.18894205
#define startthez 0.087266462
#define CREATED_DCS (PDATA+1)
#define DECOMP_COUNT (PDATA+1)
// *----------------------------------------------------------------------------
// *AR6 is set by the creator of this routine. 0= play dcs logo sound 1=don't play
// *
// *----------------------------------------------------------------------------
/* asm: SPIN_CARTAB	SPIN_CARTAB	.word	missle,hotrod,testor,cvette */
int SPIN_CARTAB[] = {
    missle, hotrod, testor, cvette,
};
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
const char *DT1 = "THANK YOU FOR PLAYING";
const char *DT2 = "CRUISIN USA";
const char *DT3 = "SNEAK PREVIEW";
/* asm: DEMOTHANKS_LIST	DEMOTHANKS_LIST */
/* asm: 	.word	140,DT1 */
/* asm: 	.word	180,DT2 */
/* asm: 	.word	220,DT3 */
int DEMOTHANKS_LIST[] = {
    140, DT1,
    180, DT2,
    220, DT3,
};
// *----------------------------------------------------------------------------

void _MIDWAYSPIN(void)
{
    // asm: 	LDI	0,AR6
    // asm: 	LDI	@_ATTR_MODE,R0
    // asm: 	CMPI	-2,R0
    // asm: 	BNE	NO_NINTENDO
    // asm: 	LDI	@ATTRWAVE,R0
    // asm: 	BNE	NO_NINTENDO
    // asm: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NO_NINTENDO
    // asm: 	READADJ	ADJ_VOLUME		;make sure volume correct
    // asm: 	LDI	R0,R1
    // asm: 	CALL	SET_MASTER_VOL
    // 	;if COMM on and SLAVE then NO NINTENDO
    // 	;
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_COMMP,R0
    // asm: 	BNZ	DOMUS
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BNZ	NO_NINTENDO
DOMUS:
    // asm: 	LDI	890,AR6			;Length of sound
    // asm: 	SOND1	NINTENDO_SND
NO_NINTENDO:
    // asm: 	CALL	ULTRA_LOGO
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(DECOMP_COUNT)
    // ;	STI	R0,*+AR7(CREATED_DCS)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	LDP	@_CAMERAPOS+Y
    // asm: 	FLOAT	-2780,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	SETDP
    // asm: 	LDL	midway,AR2
    // asm: 	CALL	OBJ_GETE
#if DEBUG
    // asm: 	BC	$
#endif
    // asm: 	LDI	AR0,AR4
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // ;	FLOAT	-480,R0
    // asm: 	FLOAT	-910,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	FLOAT	5500,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_ILLUM|O_NOUNIV|O_NOUROT,R0
    // asm: 	LDI	1,R1
    // asm: 	LS	16,R1
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR4(ORADX)
    // asm: 	LDF	startthey,R2
    // asm: 	ADDF	HALFPI,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDF	startthez,R2
    // asm: 	STF	R2,*+AR4(ORADZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	ORADX,R2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	*+AR4(ORADY),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    // ;	LDI	470,AR5
MSLP1:
    // asm: 	LDF	0.0174539,R0
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR4(OUSR1),R0		;ORADY
    // asm: 	STF	R0,*+AR4(OUSR1)
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	SUBF	@_CAMERARAD+Y,R0
    // asm: 	SETDP
    // asm: 	STF	R0,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	ORADX,R2
    // asm: 	CALL	FIND_MATRIX
    // asm: MSLP2
    // asm: 	SLEEP	1
    // asm: 	CALL	MSLP_CHECK
    // asm: 	BR	MSLP1
    // asm: MSLP_CHECK
    // asm: 	SUBI	@NFRAMES,AR6
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm: 	CMPI	0,R0
    // asm: 	BNE	MSLP3
    // asm: 	LDI	*+AR7(DECOMP_COUNT),R0
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,*+AR7(DECOMP_COUNT)
    // asm: 	CMPI	3,R0
    // asm: 	BGT	MSLP4				;Done Loading
MSLP3:
    // asm: 	RETS
    // ;	BR	MSLP1
MSLP4:
    // asm: 	CMPI	0,AR6
    // asm: 	RETSGT
    // ;	BGT	MSLP1
    // asm: MSLPX
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@LOADED
    // asm: 	POP	BK		;POP return address
    // asm: 	BR	CYCLE_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_MIDWAYSPIN", 0, 0);
    UNIMPL();
}

void MIDWAYSPINENTER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	midway,AR2
    // asm: 	CALL	OBJ_GETE
#if DEBUG
    // asm: 	BC	$
#endif
    // asm: 	LDI	AR0,AR4
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	FLOAT	-480,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	FLOAT	5500,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_ILLUM|O_NOUNIV|O_NOUROT,R0
    // asm: 	LDI	1,R1
    // asm: 	LS	16,R1
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR4(ORADX)
    // asm: 	LDF	startthey,R2
    // asm: 	ADDF	PI,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDF	startthez,R2
    // asm: 	STF	R2,*+AR4(ORADZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	ORADX,R2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	*+AR4(ORADY),R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    // asm: 	LDI	690,AR5
    // asm: MSLP1a
    // asm: 	LDF	*+AR4(OUSR1),R0		;ORADY
    // asm: 	ADDF	0.0174539,R0
    // asm: 	STF	R0,*+AR4(OUSR1)
    // asm: 	LDP	@_CAMERARAD+Y
    // asm: 	SUBF	@_CAMERARAD+Y,R0
    // asm: 	SETDP
    // asm: 	STF	R0,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	ORADX,R2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,MSLP1a
    // asm: 	CALL	TEXT_INIT
    // asm: 	BR	CYCLE_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MIDWAYSPINENTER", 0, 0);
    UNIMPL();
}

void SPIN_CAR(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(DECOMP_COUNT)
    // asm: 	LDIL	logo,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	FLOAT	-150,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	-110,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	FLOAT	436,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	CALL	LOGO_SMALL
    // asm: 	LDI	@ATTRWAVE,AR2
    // asm: 	LSH	-1,AR2
    // asm: 	SUBI	1,AR2
    // asm: 	AND	3,AR2
    // asm: 	ADDI	@SPIN_CARTABI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	FLOAT	1368,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	AR0,AR4
    // asm: 	LDF	-0.196,R2
    // asm: 	STF	R2,*+AR4(ORADX)
    // asm: 	LDF	3.534,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	ORADX,R2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	16,AR6
SPIN_CAR_WAIT:
    // asm: 	SLEEP	1
    // asm: 	SUBI	1,AR6
    // asm: 	CMPI	0,AR6
    // asm: 	BGT	SPIN_CAR_WAIT
    // asm: 	LDI	550,AR6
SPIN_CARLP:
    // asm: 	LDF	*+AR4(ORADY),R2
    // asm: 	FLOAT	@NFRAMES,R0
    // asm: 	MPYF	0.018,R0
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	ORADX,R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	SLEEP	1
    // asm: 	CALL	MSLP_CHECK
    // asm: 	BU	SPIN_CARLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPIN_CAR", 0, 0);
    UNIMPL();
}

void TEXTTOGET(void)
{
    // *----------------------------------------------------------------------------
    // *
    // asm: 	SLEEP	75
    // asm: 	LDI	@TEXT_ACTIVEI,AR0
    // asm: 	CLRF	R1
TXTLP:
    // asm: LDI	*AR0,R0
    // asm: 	BZ	NXTWT
    // asm: 	LDI	R0,AR0
    // asm: 	STF	R1,*+AR0(TEXT_VELX)
    // asm: 	BR	TXTLP
NXTWT:
    // asm: 	SLEEP	460
    // asm: 	LDI	@TEXT_ACTIVEI,AR0
    // asm: 	LDF	-6,R1
TXTLPA:
    // asm: LDI	*AR0,R0
    // asm: 	BNZ	TXTLPB
    // asm: 	BR	SUICIDE
TXTLPB:
    // asm: 	LDI	R0,AR0
    // asm: 	STF	R1,*+AR0(TEXT_VELY)
    // asm: 	BR	TXTLPA
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEXTTOGET", 0, 0);
    UNIMPL();
}

void DEMOTHANKS(void)
{
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@COINOFF
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@BGNDCOLA	;COLORAM
    // asm: 	LDI	2,AR5
    // asm: 	LDI	@DEMOTHANKS_LISTI,AR4
    // asm: DTLP
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	*AR4++,R3
    // asm: 	LDI	*AR4++,AR2
    // asm: 	LDI	9999,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	DBU	AR5,DTLP
    // asm: 	LDI	-6,R0
    // asm: 	STI	R0,@_ATTR_MODE
    // asm: 	BR	MIDWAYSPINENTER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEMOTHANKS", 0, 0);
    UNIMPL();
}

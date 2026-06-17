#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "port.h"
#include "hottub.h"

/*
 * Source module: asm/HOTTUB.ASM
 */

void HOTTUB_SCENE(void);
void HOTTUB_WATERVOL(void);
void PREZSPEAK(void);
void START_ANIMATIONS(void);
void HT_HUNGH(void);
void HT_RUT(void);
void HT_HILLY(void);
void HT_GIRL(void);
void HT_HOTTUB(void);
void HT_RADAR(void);
void HT_STAGENT(void);
void PLAINANI_PROC(void);
void AGENTANI_PROC(void);
void POLITICAL_CARTOON_NOTICE(void);
void POLTXT(void);
void CROWD_IT(void);
void BABE_TROPHY(void);
void BABE_IBO(void);
void BABE_ANI(void);
void BABE_WAVEFLAG(void);

/* asm: BABA1	SPTR	"THE PRESIDENT OF" */
const char *BABA1 = "THE PRESIDENT OF";
/* asm: BABA2	SPTR	"THE UNITED STATES" */
const char *BABA2 = "THE UNITED STATES";

/* *----------------------------------------------------------------------------
*
*
*JSRPed from BONUS SCREEN
*
 */
void HOTTUB_SCENE(void)
{
    // asm 000089A2: 	LDI	1,R0
    // asm 000089A3: 	STI	R0,@DO_FOLDFLAG		;whoosh the text away
    // asm 000089A4: 	CLRI	R0
    // asm 000089A5: 	STI	R0,@NOAERASE
    // asm 000089A6: 	LDI	@_MODE,R0
    // asm 000089A7: 	OR	MINFIN|MWATER,R0
    // asm 000089A8: 	STI	R0,@_MODE
    // asm 000089A9: 	FLOAT	0,R0
    // asm 000089AA: 	STF	R0,@INFIN_CORRECT
    // asm 000089AB: 	CALL	OBJ_INIT
    // asm 000089AC: 	LDI	@CAMERAPOSI,AR6
    // asm 000089AD: 	FLOAT	-600,R0		;-100
    // asm 000089AE: 	STF	R0,*+AR6(Y)
    // asm 000089AF: 	CLRF	R0
    // asm 000089B0: 	STF	R0,*+AR6(X)
    // asm 000089B1: 	FLOAT	-12000,R0
    // asm 000089B2: 	STF	R0,*+AR6(Z)
    // asm 000089B3: 	LDF	0,R2
    // asm 000089B4: 	LDP	@_CAMERARAD
    // asm 000089B5: 	STF	R2,@_CAMERARAD+Y
    // asm 000089B6: 	SETDP
    // asm 000089B7: 	LDI	@CAMERAMATRIXI,AR2
    // asm 000089B8: 	CALL	FIND_YMATRIX
    // asm 000089B9: 	LDL	dc_PALETTES,AR2
    // asm 000089BA: 	CALL	dealloc_section
    // asm 000089BB: 	LDL	dc_shared_PALETTES,AR2
    // asm 000089BC: 	CALL	alloc_section
    // asm 000089BD: 	LDL	finale_PALETTES,AR2
    // asm 000089BE: 	CALL	alloc_section
    // asm 000089BF: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 000089C4: 	DMA_WT		R0
    // asm 000089C9: 	CALL	FIFO_RESET
    // asm 000089CA: 	LDI	1,R0
    // asm 000089CB: 	STI	R0,@HARD_SECTION_LOAD
    // asm 000089CC: 	LDL	_SECdc_shared,AR2
    // asm 000089CD: 	CALL	LOAD_SECTION_REQ
    // asm 000089CE: 	LDI	1,R0
    // asm 000089CF: 	STI	R0,@HARD_SECTION_LOAD
    // asm 000089D0: 	LDL	_SECfinale,AR2
    // asm 000089D1: 	CALL	LOAD_SECTION_REQ
    // asm 000089D2: 	LDL	_SECdc_shared,AR2
    // asm 000089D3: 	CALL	LOAD_SECTION_REQ
    // asm 000089D4: 	LDI	@_MODE,R0
    // asm 000089D5: 	ANDN	MINFIN|MWATER,R0
    // asm 000089D6: 	STI	R0,@_MODE
    // asm 000089D7: 	SOND1	LOWCOUNTRY
    // asm 000089D9: 	SOND1	THEPRES		;"Ladies and gentlemen...
    // asm 000089DB: 	LDL	preseal2,AR2
    // asm 000089DC: 	CLRI	R2
    // asm 000089DD: 	LDI	-30,R3
    // asm 000089DE: 	LDI	368,RC
    // asm 000089DF: 	CALL	OBJ_QMAKE
    // asm 000089E0: 	ORM	O_NOUNIV|O_POSTER|O_NOUROT,*+AR0(OFLAGS)
    // asm 000089E3: 	LDI	AR0,AR2
    // asm 000089E4: 	STI	AR2,@PRES
    // asm 000089E5: 	CALL	OBJ_INSERTP
    // asm 000089E6: 	LDI	@BABA1,AR2
    // asm 000089E7: 	FLOAT	256,R2
    // asm 000089E8: 	FLOAT	270,R3
    // asm 000089E9: 	LDI	46,RC
    // asm 000089EA: 	CALL	TEXT_ADDDS
    // asm 000089EB: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000089EE: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 000089F1: 	LDI	@BABA2,AR2
    // asm 000089F2: 	FLOAT	256,R2
    // asm 000089F3: 	FLOAT	300,R3
    // asm 000089F4: 	LDI	46,RC
    // asm 000089F5: 	CALL	TEXT_ADDDS
    // asm 000089F6: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 000089F9: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 000089FC: 	CREATE	POLITICAL_CARTOON_NOTICE,1234h
    // asm 000089FF: 	SLEEP	15
    // asm 00008A01: 	CALL	SCREENWIPE_OPEN
    // asm 00008A02: 	LDL	whitehse_grp,AR2
    // asm 00008A03: 	CALL	LOAD_SINGLE_SECTION
    // asm 00008A04: 	CALL	START_ANIMATIONS
    // asm 00008A05: 	CREATE	HOTTUB_WATERVOL,1235h
    // asm 00008A08: 	LDI	@_MODE,R0
    // asm 00008A09: 	OR	MINFIN|MWATER,R0
    // asm 00008A0A: 	STI	R0,@_MODE
    // asm 00008A0B: 	SLEEP	5
    // asm 00008A0D: 	CREATE	PREZSPEAK,1234h
    // asm 00008A10: 	SLEEP	25
    // asm 00008A12: 	LDI	@PRES,AR2
    // asm 00008A13: 	CALL	OBJ_DELETE
    // asm 00008A14: 	SLEEP	30
    // asm 00008A16: 	LDI	1,R0
    // asm 00008A17: 	STI	R0,@NOAERASE
    // asm 00008A18: 	LDI	180,AR5
    // ;	LDI	210,AR5
    // asm 00008A19: HOTTUB_LPB
    // asm 00008A19: 	LDI	@CAMERAPOSI,AR6
    // asm 00008A1A: 	LDF	*+AR6(Z),R0
    // asm 00008A1B: 	FLOAT	-1500,R1
    // asm 00008A1C: 	SUBF	R0,R1,R2
    // ;	MPYF	0.02,R2
    // asm 00008A1D: 	MPYF	0.017,R2
    // asm 00008A1E: 	ADDF	R2,R0
    // asm 00008A1F: 	STF	R0,*+AR6(Z)
    // asm 00008A20: 	LDF	*+AR6(Y),R0
    // asm 00008A21: 	FLOAT	-4300,R1
    // asm 00008A22: 	SUBF	R0,R1,R2
    // asm 00008A23: 	MPYF	0.04,R2
    // asm 00008A24: 	ADDF	R2,R0
    // asm 00008A25: 	STF	R0,*+AR6(Y)
    // asm 00008A26: 	SLEEP	1
    // asm 00008A28: 	DBU	AR5,HOTTUB_LPB
    // asm 00008A29: 	SLEEP	300
    // asm 00008A2B: 	CALL	SCREENWIPE_CLOSE
    // asm 00008A2C: 	LDI	1235h,R0
    // asm 00008A2D: 	LDI	-1,R1
    // asm 00008A2E: 	CALL	PRC_KILLALL
    // asm 00008A2F: 	LDI	FIREHYDRSND,AR2
    // asm 00008A30: 	CALL	KILLSNDFX
    // asm 00008A31: 	SOND1	CLIN8
    // asm 00008A33: 	SLEEP	15
    // asm 00008A35: 	SONDFX	CROWD1
    // asm 00008A37: 	SONDFX	CROWDROAR
    // asm 00008A39: 	LDI	100,AR5
    // asm 00008A3A: HOTTUB_LPB2
    // asm 00008A3A: 	LDI	@CAMERAPOSI,AR6
    // asm 00008A3B: 	LDF	*+AR6(Z),R0
    // asm 00008A3C: 	FLOAT	-12000,R1
    // asm 00008A3D: 	SUBF	R0,R1,R2
    // asm 00008A3E: 	MPYF	0.01,R2
    // asm 00008A3F: 	ADDF	R2,R0
    // asm 00008A40: 	STF	R0,*+AR6(Z)
    // asm 00008A41: 	CMPI	60,AR5
    // asm 00008A42: 	BNE	HOTTT
    // asm 00008A43: 	SOND1	CLIN5
    // asm 00008A45: 	LDI	3,R0 		;COOL LAST YEEHAH
    // asm 00008A46: 	LDI	130,R1
    // asm 00008A47: 	CALL	SET_TRACK_VOL
HOTTT:
    // asm 00008A48: 	SLEEP	1
    // asm 00008A4A: 	DBU	AR5,HOTTUB_LPB2
    // ;moved to bonus.asm
    // ;Wed Mar 8 09:52:21 1995
    // ;
    // ;	SLEEP	1
    // ;
    // ;	CALL	OBJ_INIT
    // ;	CALL	INIT_DRONES	;initialize DRONE tracker system
    // ;	CALL	DYNAOBJ_INIT	;initialize DYNAMIC OBJECTS
    // ;	CALL	CARB_INIT	;initialize CAR BLOCKS
    // ;	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    // ;
    // ;
    // ;	LDI	1234h,R0
    // ;	LDI	-1,R1
    // ;	CALL	PRC_KILLALL
    // ;
    // ;
    // ;	LDI	SPAWNER_C|ANIMATION_T,R0
    // ;	LDI	-1,R1
    // ;	CALL	PRC_KILLALL
    // ;
    // ;
    // ;	LDL	dc_shared_PALETTES,AR2
    // ;	CALL	dealloc_section
    // ;	LDL	finale_PALETTES,AR2
    // ;	CALL	dealloc_section
    // asm 00008A4B: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HOTTUB_SCENE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void HOTTUB_WATERVOL(void)
{
    // asm 00008A4F: 	LDI	@WATCHOBJ,AR4
    // asm 00008A50: 	LDF	*+AR4(OPOSX),R0
    // asm 00008A51: 	LDI	@CAMERAPOSI,AR0
    // asm 00008A52: 	SUBF	*+AR0(X),R0
    // asm 00008A53: 	MPYF	R0,R0
    // asm 00008A54: 	LDF	*+AR4(OPOSZ),R2
    // asm 00008A55: 	SUBF	*+AR0(Z),R2
    // asm 00008A56: 	MPYF	R2,R2
    // asm 00008A57: 	ADDF	R0,R2
    // asm 00008A58: 	CALL	SQRT
    // 	;from  0-12,000  255
    // 	;12,001-64,000  255-110
    // 	;
    // asm 00008A59: 	FLOAT	2000,R1
    // asm 00008A5A: 	CMPF	R1,R0
    // asm 00008A5B: 	BGT	NDOO
    // asm 00008A5C: 	LDI	205,R2
    // asm 00008A5D: 	BU	BIBO
NBIBO:
    // asm 00008A5E: LDI	10,R2
    // asm 00008A5F: 	BU	BIBO
NDOO:
    // asm 00008A60: 	FLOAT	12000,R2
    // asm 00008A61: 	CMPF	R2,R0
    // asm 00008A62: 	BGT	NBIBO
    // asm 00008A63: 	SUBF	R1,R0
    // asm 00008A64: 	MPYFL	0.0001,R0		;1/50,000
    // asm 00008A65: 	SUBRF	1.0,R0
    // asm 00008A66: 	MPYF	205,R0
    // asm 00008A67: 	FIX	R0,R2
    // asm 00008A68: 	CMPI	180,R2
    // asm 00008A69: 	LDIGT	180,R2
    // asm 00008A6A: BIBO	LDI	FIREHYDRSND,AR2
    // asm 00008A6B: 	CALL	AMBIENCE_SOUND
    // asm 00008A6C: 	SLEEP	1
    // asm 00008A6E: 	BU	HOTTUB_WATERVOL
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HOTTUB_WATERVOL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void PREZSPEAK(void)
{
    // asm 00008A6F: 	SLEEP	25
    // asm 00008A71: 	SONDFX	CROWDROAR
    // asm 00008A73: 	SLEEP	25
    // asm 00008A75: 	SOND1	CLIN1	;Croozin USA
    // asm 00008A77: 	SLEEP	44
    // asm 00008A79: 	SOND1	CLIN2	;Come on Down
    // asm 00008A7B: 	SONDFX	CROWD1
    // asm 00008A7D: 	SLEEP	75
    // asm 00008A7F: 	SOND1	CLIN3	;I luv this job
    // asm 00008A81: 	SONDFX	CROWDROAR
    // asm 00008A83: 	SLEEP	50
    // asm 00008A85: 	SOND1	CLIN5	;Yee HAW
    // asm 00008A87: 	SLEEP	40
    // asm 00008A89: 	SOND1	CLIN4	;Come in the hot tub
    // asm 00008A8B: 	SLEEP	60
    // asm 00008A8D: 	SOND1	CHICKCHEER
    // asm 00008A8F: 	SLEEP	70
    // asm 00008A91: 	SOND1	CLIN6	;Well Hoo Eee
    // asm 00008A93: 	SLEEP	30
    // asm 00008A95: 	SONDFX	MOO
    // asm 00008A97: 	SLEEP	40
    // asm 00008A99: 	SOND1	CLIN7	;Hey Doggie
    // asm 00008A9B: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PREZSPEAK", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void START_ANIMATIONS(void)
{
    // asm 00008A9C: 	LDI	@OACTIVEI,AR4
ODC:
    // asm 00008A9D: LDI	*AR4,R3
    // asm 00008A9E: 	BZ	ODCX
    // asm 00008A9F: 	LDI	R3,AR4
ODCL2:
    // asm 00008AA0: LDI	*+AR4(OID),R2
    // asm 00008AA1: 	CMPI	6D0h,R2
    // asm 00008AA2: 	BNE	HHTT
    // asm 00008AA3: 	CALL	HT_HUNGH
    // asm 00008AA4: 	BU	ODC
HHTT:
    // asm 00008AA5: 	CMPI	621h,R2
    // asm 00008AA6: 	BNE	NTT1
    // asm 00008AA7: 	CALL	HT_HOTTUB
    // asm 00008AA8: 	BU	ODC
NTT1:
    // asm 00008AA9: 	CMPI	622h,R2
    // asm 00008AAA: 	BNE	NTT1A
    // asm 00008AAB: 	CALL	HT_HILLY
    // asm 00008AAC: 	BU	ODC
NTT1A:
    // asm 00008AAD: 	CMPI	623h,R2
    // asm 00008AAE: 	BNE	NTT1B
    // asm 00008AAF: 	CALL	HT_GIRL
    // asm 00008AB0: 	BU	ODC
NTT1B:
    // asm 00008AB1: 	CMPI	615h,R2
    // asm 00008AB2: 	BNE	NTT2
    // asm 00008AB3: 	CALL	HT_RADAR
    // asm 00008AB4: 	BU	ODC
NTT2:
    // asm 00008AB5: CMPI	616h,R2
    // asm 00008AB6: 	BNE	ODC
    // asm 00008AB7: 	CALL	HT_STAGENT
    // asm 00008AB8: 	BU	ODC
ODCX:
    // asm 00008AB9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "START_ANIMATIONS", 0, 0);
    UNIMPL();
}

/* asm: ANI_HOTTUB	.word	htub,htub1,htub2,htub3,htub4,htub5,-1 */
int ANI_HOTTUB[] = {
    htub, htub1, htub2, htub3, htub4, htub5, -1,
};
/* asm: HILLANI	.word	hilly1,hilly2,hilly3,hilly4,hilly5,hilly6,-1 */
int HILLANI[] = {
    hilly1, hilly2, hilly3, hilly4, hilly5, hilly6, -1,
};
/* asm: GIRLANI	.word	girl1,girl2,girl3,girl4,girl5,girl6,-1 */
int GIRLANI[] = {
    girl1, girl2, girl3, girl4, girl5, girl6, -1,
};
/* asm: ANI_RADAR	.word	rad1,rad2,rad3,rad4,rad5,rad6,rad7,rad8,rad9,-1 */
int ANI_RADAR[] = {
    rad1, rad2, rad3, rad4, rad5, rad6, rad7, rad8, rad9, -1,
};
/* asm: ANI_STAG	.word	stagent1,stagent2,stagent3,5,stagent2,-1 */
int ANI_STAG[] = {
    stagent1, stagent2, stagent3, 5, stagent2, -1,
};

void HT_HUNGH(void)
{
    // asm 00008AE4: 	PUSH	AR6
    // ;	LDI	*+AR4(OPLINK),AR2
    // ;	CALL	PRC_KILL
    // ;	LDI	*+AR4(OFLAGS),R0
    // ;	ANDN	O_POSTER,R0
    // ;	STI	R0,*+AR4(OFLAGS)
    // asm 00008AE5: 	LDF	1.2,R0
    // asm 00008AE6: 	STF	R0,*+AR4(OMAT00)
    // asm 00008AE7: 	STF	R0,*+AR4(OMAT11)
    // asm 00008AE8: 	STF	R0,*+AR4(OMAT22)
    // asm 00008AE9: 	LDI	@HUNGH_ANISI,AR6
    // asm 00008AEA: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 00008AED: 	POP	AR6
    // asm 00008AEE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_HUNGH", 0, 0);
    UNIMPL();
}

void HT_RUT(void)
{
    // asm 00008AEF: 	PUSH	AR6
    // asm 00008AF0: 	LDI	*+AR4(OPLINK),AR2
    // asm 00008AF1: 	CALL	PRC_KILL
    // asm 00008AF2: 	LDI	*+AR4(OFLAGS),R0
    // asm 00008AF3: 	ANDN	O_POSTER,R0
    // asm 00008AF4: 	STI	R0,*+AR4(OFLAGS)
    // asm 00008AF5: 	LDI	@RUT_ANISI,AR6
    // asm 00008AF6: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 00008AF9: 	POP	AR6
    // asm 00008AFA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_RUT", 0, 0);
    UNIMPL();
}

/* asm: WATCHOBJ	.bss	WATCHOBJ,1 */
int WATCHOBJ;

void HT_HILLY(void)
{
    // asm 00008AFB: 	PUSH	AR6
    // asm 00008AFC: 	LDI	@HILLANII,AR6
    // asm 00008AFD: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 00008B00: 	POP	AR6
    // asm 00008B01: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_HILLY", 0, 0);
    UNIMPL();
}

void HT_GIRL(void)
{
    // asm 00008B02: 	PUSH	AR6
    // asm 00008B03: 	LDI	@GIRLANII,AR6
    // asm 00008B04: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 00008B07: 	POP	AR6
    // asm 00008B08: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_GIRL", 0, 0);
    UNIMPL();
}

void HT_HOTTUB(void)
{
    // asm 00008B09: 	PUSH	AR6
    // asm 00008B0A: 	STI	AR4,@WATCHOBJ
    // asm 00008B0B: 	LDI	@ANI_HOTTUBI,AR6
    // asm 00008B0C: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 00008B0F: 	POP	AR6
    // asm 00008B10: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_HOTTUB", 0, 0);
    UNIMPL();
}

void HT_RADAR(void)
{
    // asm 00008B11: 	PUSH	AR6
    // asm 00008B12: 	LDI	@ANI_RADARI,AR6
    // asm 00008B13: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 00008B16: 	POP	AR6
    // asm 00008B17: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_RADAR", 0, 0);
    UNIMPL();
}

void HT_STAGENT(void)
{
    // asm 00008B18: 	PUSH	AR6
    // asm 00008B19: 	LDI	@ANI_STAGI,AR6
    // asm 00008B1A: 	CREATE	AGENTANI_PROC,SPAWNER_C|ANIMATION_T
    // asm 00008B1D: 	POP	AR6
    // asm 00008B1E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_STAGENT", 0, 0);
    UNIMPL();
}

void PLAINANI_PROC(void)
{
    // asm 00008B1F: 	LDI	AR6,AR5
PLAINANI_LP:
    // asm 00008B20: 	LDI	*AR5++,R0
    // asm 00008B21: 	BLT	PLAINANI_PROC
    // asm 00008B22: 	STI	R0,*+AR4(OROMDATA)
    // asm 00008B23: 	LDI	9,R4
    // asm 00008B24: 	RANDN	3
    // asm 00008B26: 	ADDI	R0,R4
PPDD:
    // asm 00008B27: SLEEP	1
    // asm 00008B29: 	SUBI	@NFRAMES,R4
    // asm 00008B2A: 	BLE	PLAINANI_LP
    // asm 00008B2B: 	BU	PPDD
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLAINANI_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void AGENTANI_PROC(void)
{
    // asm 00008B2C: 	RANDN	50
    // asm 00008B2E: 	LDI	R0,AR2
    // asm 00008B2F: 	ADDI	20,AR2
    // asm 00008B30: 	CALL	SLEEP
    // asm 00008B31: 	LDI	AR6,AR5
AGENTANI_LP:
    // asm 00008B32: 	LDI	*AR5++,R0
    // asm 00008B33: 	BLT	AGENTANI_PROC
    // asm 00008B34: 	CMPI	5,R0
    // asm 00008B35: 	BNE	JKG
    // asm 00008B36: 	SLEEP	12
    // asm 00008B38: 	BU	AGENTANI_LP
JKG:
    // asm 00008B39: STI	R0,*+AR4(OROMDATA)
    // asm 00008B3A: 	LDI	15,R4
PPDDA:
    // asm 00008B3B: SLEEP	1
    // asm 00008B3D: 	SUBI	@NFRAMES,R4
    // asm 00008B3E: 	BLE	AGENTANI_LP
    // asm 00008B3F: 	BU	PPDDA
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AGENTANI_PROC", 0, 0);
    UNIMPL();
}

/* asm: PC1	SPTR	"CELEBRITY IMPERSONATION" */
const char *PC1 = "CELEBRITY IMPERSONATION";
/* asm: PC2	SPTR	"POLITICAL CARTOON" */
const char *PC2 = "POLITICAL CARTOON";

/* *----------------------------------------------------------------------------
 */
void POLITICAL_CARTOON_NOTICE(void)
{
    // asm 00008B42: 	LDI	@PC1,AR2
    // asm 00008B43: 	LDI	15,RC
    // asm 00008B44: 	CALL	POLTXT
    // asm 00008B45: 	SLEEP	26
    // asm 00008B47: 	LDI	@PC2,AR2
    // asm 00008B48: 	LDI	20,RC
    // asm 00008B49: 	CALL	POLTXT
    // asm 00008B4A: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POLITICAL_CARTOON_NOTICE", 0, 0);
    UNIMPL();
}

void POLTXT(void)
{
    // asm 00008B4B: 	FLOAT	256,R2
    // asm 00008B4C: 	FLOAT	20,R3
    // asm 00008B4D: 	CALL	TEXT_ADDDS
    // asm 00008B4E: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00008B51: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00008B54: 	LDL	font18_white,AR2
    // asm 00008B55: 	CALL	PAL_FIND_RAW
    // asm 00008B56: 	STI	R0,*+AR0(TEXT_PAL)
    // asm 00008B57: 	STI	R0,*+AR1(TEXT_PAL)
    // asm 00008B58: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POLTXT", 0, 0);
    UNIMPL();
}

#define BABE_ASTRT PDATA

/* *----------------------------------------------------------------------------
 */
void CROWD_IT(void)
{
    // asm 00008B59: 	LDI	255,R2
    // asm 00008B5A: 	LDI	CROWD1,AR2
    // asm 00008B5B: 	CALL	AMBIENCE_SOUND
    // asm 00008B5C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CROWD_IT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void BABE_TROPHY(void)
{
    // ;	SLEEP	6
    // asm 00008B5D: 	LDI	12,AR5
DABLP3:
    // asm 00008B5E: SLEEP	1
    // asm 00008B60: 	SUBI	@NFRAMES,AR5
    // asm 00008B61: 	CMPI	0,AR5
    // asm 00008B62: 	LDILT	0,AR5
    // asm 00008B63: 	DBU	AR5,DABLP3
    // asm 00008B64: 	SOND1	GETREADYTUNE
    // asm 00008B66: 	CALL	OBJ_GET
    // asm 00008B67: 	LDI	AR0,AR4
    // asm 00008B68: 	FLOAT	-50,R0
    // asm 00008B69: 	STF	R0,*+AR4(OPOSX)
    // asm 00008B6A: 	FLOAT	310,R0
    // asm 00008B6B: 	STF	R0,*+AR4(OPOSY)
    // asm 00008B6C: 	FLOAT	368,R0
    // asm 00008B6D: 	STF	R0,*+AR4(OPOSZ)
    // asm 00008B6E: 	LDI	*+AR4(OFLAGS),R0
    // asm 00008B6F: 	OR	O_POSTER|O_NOUROT|O_NOUNIV,R0
    // asm 00008B70: 	STI	R0,*+AR4(OFLAGS)
    // asm 00008B71: 	LDL	here1,R0
    // asm 00008B72: 	STI	R0,*+AR4(OROMDATA)
    // asm 00008B73: 	READAUD	ADJ_GIRLS
    // asm 00008B75: 	CMPI	0,R0
    // asm 00008B76: 	BEQ	NODA
    // asm 00008B77: 	LDI	AR4,AR2
    // asm 00008B78: 	CALL	OBJ_INSERTP
NODA:
    // asm 00008B79: 	LDL	BABE_TROPHY_SCRIPT,AR6
    // asm 00008B7A: 	STI	AR6,*+AR7(BABE_ASTRT)
    // asm 00008B7B: 	LDI	11,AR5
    // asm 00008B7C: BABE_LP
    // asm 00008B7C: 	LDF	*+AR4(OPOSY),R0
    // asm 00008B7D: 	SUBF	3,R0
    // asm 00008B7E: 	STF	R0,*+AR4(OPOSY)
    // asm 00008B7F: 	CALL	BABE_ANI
    // asm 00008B80: 	CALL	CROWD_IT
    // asm 00008B81: 	SLEEP	1
    // asm 00008B83: 	LDF	*+AR4(OPOSY),R0
    // asm 00008B84: 	SUBF	3,R0
    // asm 00008B85: 	STF	R0,*+AR4(OPOSY)
    // asm 00008B86: 	CALL	CROWD_IT
    // asm 00008B87: 	SLEEP	1
    // asm 00008B89: 	LDF	*+AR4(OPOSY),R0
    // asm 00008B8A: 	SUBF	3,R0
    // asm 00008B8B: 	STF	R0,*+AR4(OPOSY)
    // asm 00008B8C: 	CALL	CROWD_IT
    // asm 00008B8D: 	SLEEP	1
    // asm 00008B8F: 	DBU	AR5,BABE_LP
    // asm 00008B90: 	LDI	13,AR5
    // asm 00008B91: BBD	CALL	BABE_ANI
    // asm 00008B92: 	CALL	CROWD_IT
    // asm 00008B93: 	SLEEP	3
    // asm 00008B95: 	DBU	AR5,BBD
    // asm 00008B96: 	SONDFX	CHICK_LAUGH
    // asm 00008B98: 	LDI	27,AR5
    // asm 00008B99: BABE_LP2
    // asm 00008B99: 	CALL	BABE_ANI
    // asm 00008B9A: 	CALL	CROWD_IT
    // asm 00008B9B: 	SLEEP	3
    // asm 00008B9D: 	DBU	AR5,BABE_LP2
    // ;	LDI	25-1,AR5
    // asm 00008B9E: 	LDI	50-1,AR5
    // asm 00008B9F: BABLP3	SLEEP	1
    // asm 00008BA1: 	SUBI	@NFRAMES,AR5
    // asm 00008BA2: 	CMPI	0,AR5
    // asm 00008BA3: 	LDILT	0,AR5
    // asm 00008BA4: 	DBU	AR5,BABLP3
    // asm 00008BA5: BABE_TROPHYX
    // asm 00008BA5: 	CREATEC	BABE_IBO,UTIL_C
    // asm 00008BA8: 	LDI	*+AR7(BABE_ASTRT),R0
    // asm 00008BA9: 	STI	R0,*+AR0(BABE_ASTRT)
    // asm 00008BAA: 	RETP
    // *
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BABE_TROPHY", 0, 0);
    UNIMPL();
}

void BABE_IBO(void)
{
    // asm 00008BAE: 	LDI	6,AR5
    // asm 00008BAF: BABE_LP3
    // asm 00008BAF: 	LDF	*+AR4(OPOSY),R0
    // asm 00008BB0: 	ADDF	15,R0
    // asm 00008BB1: 	STF	R0,*+AR4(OPOSY)
    // asm 00008BB2: 	SLEEP	1
    // asm 00008BB4: 	LDF	*+AR4(OPOSY),R0
    // asm 00008BB5: 	ADDF	15,R0
    // asm 00008BB6: 	STF	R0,*+AR4(OPOSY)
    // asm 00008BB7: 	SLEEP	1
    // asm 00008BB9: 	LDF	*+AR4(OPOSY),R0
    // asm 00008BBA: 	ADDF	15,R0
    // asm 00008BBB: 	STF	R0,*+AR4(OPOSY)
    // asm 00008BBC: 	SLEEP	1
    // asm 00008BBE: 	DBU	AR5,BABE_LP3
    // asm 00008BBF: 	LDI	0,R0
    // asm 00008BC0: 	LDI	100,R1
    // asm 00008BC1: 	CALL	SET_TRACK_VOL
    // asm 00008BC2: 	SOND1	MAPTUNE
    // asm 00008BC4: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BABE_IBO", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void BABE_ANI(void)
{
    // asm 00008BC5: 	LONGROUT
    // asm: BABE_ANIKK
    // asm: 	LDI	*AR6++,R0
    // asm: 	BN	BX
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	RETS
    // asm: BX	LDI	*+AR7(BABE_ASTRT),AR6
    // asm: 	BU	BABE_ANIKK
    // asm: 	END_LONGROUT
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BABE_ANI", 0, 0);
    UNIMPL();
}

/* asm: BABE_TROPHY_SCRIPT */
/* asm: 	.word	here1 */
/* asm: 	.word	here2 */
/* asm: 	.word	here3 */
/* asm: 	.word	here4 */
/* asm: 	.word	here5 */
/* asm: 	.word	here6 */
/* asm: 	.word	-1 */
int BABE_TROPHY_SCRIPT[] = {
    here1,
    here2,
    here3,
    here4,
    here5,
    here6,
    -1,
};
/* asm: BABE_FLAG_SCRIPT */
/* asm: 	.word	flag1 */
/* asm: 	.word	flag2 */
/* asm: 	.word	flag3 */
/* asm: 	.word	flag4 */
/* asm: 	.word	flag5 */
/* asm: 	.word	flag6 */
/* asm: 	.word	flag7 */
/* asm: 	.word	-1 */
int BABE_FLAG_SCRIPT[] = {
    flag1,
    flag2,
    flag3,
    flag4,
    flag5,
    flag6,
    flag7,
    -1,
};

/* *----------------------------------------------------------------------------
*
*
*
*
*
 */
void BABE_WAVEFLAG(void)
{
    // asm 00008BD5: 	READAUD	ADJ_GIRLS
    // asm 00008BD7: 	CMPI	0,R0
    // asm 00008BD8: 	BEQ	SUICIDE
    // asm 00008BD9: 	CALL	OBJ_GET
    // asm 00008BDA: 	LDI	AR0,AR4
    // asm 00008BDB: 	FLOAT	-350,R0
    // asm 00008BDC: 	STF	R0,*+AR4(OPOSX)
    // asm 00008BDD: 	FLOAT	40,R0
    // asm 00008BDE: 	STF	R0,*+AR4(OPOSY)
    // asm 00008BDF: 	FLOAT	368,R0
    // asm 00008BE0: 	STF	R0,*+AR4(OPOSZ)
    // asm 00008BE1: 	LDI	*+AR4(OFLAGS),R0
    // asm 00008BE2: 	OR	O_POSTER|O_NOUROT|O_NOUNIV,R0
    // asm 00008BE3: 	STI	R0,*+AR4(OFLAGS)
    // asm 00008BE4: 	LDL	flag1,R0
    // asm 00008BE5: 	STI	R0,*+AR4(OROMDATA)
    // asm 00008BE6: 	LDI	AR4,AR2
    // asm 00008BE7: 	CALL	OBJ_INSERTP
    // asm 00008BE8: BABEWTLP
    // asm 00008BE8: 	LDI	@BABE_CONTROL,R0
    // asm 00008BE9: 	BNZ	BABEGO
    // asm 00008BEA: 	LDF	*+AR4(OPOSX),R0
    // asm 00008BEB: 	FLOAT	-200,R1
    // asm 00008BEC: 	CMPF	R1,R0
    // asm 00008BED: 	BGE	IBO2
    // asm 00008BEE: 	ADDF	5,R0
    // asm 00008BEF: 	STF	R0,*+AR4(OPOSX)
IBO2:
    // asm 00008BF0: 	SLEEP	1
    // asm 00008BF2: 	BU	BABEWTLP
    // asm 00008BF3: BABEGO
    // asm 00008BF3: 	LDI	17,AR5
    // asm 00008BF4: BABERST
    // asm 00008BF4: 	LDL	BABE_FLAG_SCRIPT,AR6
    // asm 00008BF5: BABE_LPWF
    // asm 00008BF5: 	LDI	*AR6++,R0
    // asm 00008BF6: 	BN	BABERST
    // asm 00008BF7: 	STI	R0,*+AR4(OROMDATA)
    // asm 00008BF8: 	SLEEP	2
    // asm 00008BFA: 	DBU	AR5,BABE_LPWF
    // asm 00008BFB: 	SLEEP	5
    // asm 00008BFD: 	LDI	16,AR5
    // asm 00008BFE: BABEOFF
    // asm 00008BFE: 	LDF	*+AR4(OPOSX),R0
    // asm 00008BFF: 	SUBF	6,R0
    // asm 00008C00: 	STF	R0,*+AR4(OPOSX)
    // asm 00008C01: 	SLEEP	1
    // asm 00008C03: 	LDF	*+AR4(OPOSX),R0
    // asm 00008C04: 	SUBF	6,R0
    // asm 00008C05: 	STF	R0,*+AR4(OPOSX)
    // asm 00008C06: 	SLEEP	1
    // asm 00008C08: 	DBU	AR5,BABEOFF
    // asm 00008C09: 	LDI	AR4,AR2
    // asm 00008C0A: 	CALL	OBJ_DELETE
    // asm 00008C0B: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BABE_WAVEFLAG", 0, 0);
    UNIMPL();
}

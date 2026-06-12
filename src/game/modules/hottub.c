#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/obj.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/hottub.h"

/*
 * Source module: asm/HOTTUB.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: BABA1	SPTR	"THE PRESIDENT OF" */
const char *BABA1 = "THE PRESIDENT OF";
/* asm: BABA2	SPTR	"THE UNITED STATES" */
const char *BABA2 = "THE UNITED STATES";
// *----------------------------------------------------------------------------
// *
// *
// *JSRPed from BONUS SCREEN
// *
// *----------------------------------------------------------------------------
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
// ;hott1,hott2,hott3,hott4,hott5,hott6,-1
/* asm: ANI_RADAR	.word	rad1,rad2,rad3,rad4,rad5,rad6,rad7,rad8,rad9,-1 */
int ANI_RADAR[] = {
    rad1, rad2, rad3, rad4, rad5, rad6, rad7, rad8, rad9, -1,
};
/* asm: ANI_STAG	.word	stagent1,stagent2,stagent3,5,stagent2,-1 */
int ANI_STAG[] = {
    stagent1, stagent2, stagent3, 5, stagent2, -1,
};
// *----------------------------------------------------------------------------
/* asm: PC1	SPTR	"CELEBRITY IMPERSONATION" */
const char *PC1 = "CELEBRITY IMPERSONATION";
/* asm: PC2	SPTR	"POLITICAL CARTOON" */
const char *PC2 = "POLITICAL CARTOON";
// *----------------------------------------------------------------------------
#define BABE_ASTRT PDATA
// *----------------------------------------------------------------------------
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
/* asm: 	*---------------------------------------------------------------------------- */
/* asm: 	* */
/* asm: 	* */
/* asm: 	* */
/* asm: 	* */
/* asm: 	* */
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
// *----------------------------------------------------------------------------

void HOTTUB_SCENE(void)
{
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@DO_FOLDFLAG		;whoosh the text away
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MINFIN|MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	FLOAT	0,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	CALL	OBJ_INIT
    // asm: 	LDI	@CAMERAPOSI,AR6
    // asm: 	FLOAT	-600,R0		;-100
    // asm: 	STF	R0,*+AR6(Y)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR6(X)
    // asm: 	FLOAT	-12000,R0
    // asm: 	STF	R0,*+AR6(Z)
    // asm: 	LDF	0,R2
    // asm: 	LDP	@_CAMERARAD
    // asm: 	STF	R2,@_CAMERARAD+Y
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDL	dc_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDL	dc_shared_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	LDL	finale_PALETTES,AR2
    // asm: 	CALL	alloc_section
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	LDL	_SECdc_shared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	LDL	_SECfinale,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	_SECdc_shared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MINFIN|MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	SOND1	LOWCOUNTRY
    // asm: 	SOND1	THEPRES		;"Ladies and gentlemen...
    // asm: 	LDL	preseal2,AR2
    // asm: 	CLRI	R2
    // asm: 	LDI	-30,R3
    // asm: 	LDI	368,RC
    // asm: 	CALL	OBJ_QMAKE
    // asm: 	ORM	O_NOUNIV|O_POSTER|O_NOUROT,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	STI	AR2,@PRES
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDI	@BABA1,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	270,R3
    // asm: 	LDI	46,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	LDI	@BABA2,AR2
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	300,R3
    // asm: 	LDI	46,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	CREATE	POLITICAL_CARTOON_NOTICE,1234h
    // asm: 	SLEEP	15
    // asm: 	CALL	SCREENWIPE_OPEN
    // asm: 	LDL	whitehse_grp,AR2
    // asm: 	CALL	LOAD_SINGLE_SECTION
    // asm: 	CALL	START_ANIMATIONS
    // asm: 	CREATE	HOTTUB_WATERVOL,1235h
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MINFIN|MWATER,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	SLEEP	5
    // asm: 	CREATE	PREZSPEAK,1234h
    // asm: 	SLEEP	25
    // asm: 	LDI	@PRES,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	SLEEP	30
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	LDI	180,AR5
    // ;	LDI	210,AR5
    // asm: HOTTUB_LPB
    // asm: 	LDI	@CAMERAPOSI,AR6
    // asm: 	LDF	*+AR6(Z),R0
    // asm: 	FLOAT	-1500,R1
    // asm: 	SUBF	R0,R1,R2
    // ;	MPYF	0.02,R2
    // asm: 	MPYF	0.017,R2
    // asm: 	ADDF	R2,R0
    // asm: 	STF	R0,*+AR6(Z)
    // asm: 	LDF	*+AR6(Y),R0
    // asm: 	FLOAT	-4300,R1
    // asm: 	SUBF	R0,R1,R2
    // asm: 	MPYF	0.04,R2
    // asm: 	ADDF	R2,R0
    // asm: 	STF	R0,*+AR6(Y)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,HOTTUB_LPB
    // asm: 	SLEEP	300
    // asm: 	CALL	SCREENWIPE_CLOSE
    // asm: 	LDI	1235h,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	LDI	FIREHYDRSND,AR2
    // asm: 	CALL	KILLSNDFX
    // asm: 	SOND1	CLIN8
    // asm: 	SLEEP	15
    // asm: 	SONDFX	CROWD1
    // asm: 	SONDFX	CROWDROAR
    // asm: 	LDI	100,AR5
    // asm: HOTTUB_LPB2
    // asm: 	LDI	@CAMERAPOSI,AR6
    // asm: 	LDF	*+AR6(Z),R0
    // asm: 	FLOAT	-12000,R1
    // asm: 	SUBF	R0,R1,R2
    // asm: 	MPYF	0.01,R2
    // asm: 	ADDF	R2,R0
    // asm: 	STF	R0,*+AR6(Z)
    // asm: 	CMPI	60,AR5
    // asm: 	BNE	HOTTT
    // asm: 	SOND1	CLIN5
    // asm: 	LDI	3,R0 		;COOL LAST YEEHAH
    // asm: 	LDI	130,R1
    // asm: 	CALL	SET_TRACK_VOL
HOTTT:
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,HOTTUB_LPB2
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
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HOTTUB_SCENE", 0, 0);
    UNIMPL();
}

void HOTTUB_WATERVOL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@WATCHOBJ,AR4
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	LDI	@CAMERAPOSI,AR0
    // asm: 	SUBF	*+AR0(X),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR4(OPOSZ),R2
    // asm: 	SUBF	*+AR0(Z),R2
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R0,R2
    // asm: 	CALL	SQRT
    // 	;from  0-12,000  255
    // 	;12,001-64,000  255-110
    // 	;
    // asm: 	FLOAT	2000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	NDOO
    // asm: 	LDI	205,R2
    // asm: 	BU	BIBO
NBIBO:
    // asm: LDI	10,R2
    // asm: 	BU	BIBO
NDOO:
    // asm: 	FLOAT	12000,R2
    // asm: 	CMPF	R2,R0
    // asm: 	BGT	NBIBO
    // asm: 	SUBF	R1,R0
    // asm: 	MPYFL	0.0001,R0		;1/50,000
    // asm: 	SUBRF	1.0,R0
    // asm: 	MPYF	205,R0
    // asm: 	FIX	R0,R2
    // asm: 	CMPI	180,R2
    // asm: 	LDIGT	180,R2
    // asm: BIBO	LDI	FIREHYDRSND,AR2
    // asm: 	CALL	AMBIENCE_SOUND
    // asm: 	SLEEP	1
    // asm: 	BU	HOTTUB_WATERVOL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HOTTUB_WATERVOL", 0, 0);
    UNIMPL();
}

void PREZSPEAK(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	SLEEP	25
    // asm: 	SONDFX	CROWDROAR
    // asm: 	SLEEP	25
    // asm: 	SOND1	CLIN1	;Croozin USA
    // asm: 	SLEEP	44
    // asm: 	SOND1	CLIN2	;Come on Down
    // asm: 	SONDFX	CROWD1
    // asm: 	SLEEP	75
    // asm: 	SOND1	CLIN3	;I luv this job
    // asm: 	SONDFX	CROWDROAR
    // asm: 	SLEEP	50
    // asm: 	SOND1	CLIN5	;Yee HAW
    // asm: 	SLEEP	40
    // asm: 	SOND1	CLIN4	;Come in the hot tub
    // asm: 	SLEEP	60
    // asm: 	SOND1	CHICKCHEER
    // asm: 	SLEEP	70
    // asm: 	SOND1	CLIN6	;Well Hoo Eee
    // asm: 	SLEEP	30
    // asm: 	SONDFX	MOO
    // asm: 	SLEEP	40
    // asm: 	SOND1	CLIN7	;Hey Doggie
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PREZSPEAK", 0, 0);
    UNIMPL();
}

void START_ANIMATIONS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@OACTIVEI,AR4
ODC:
    // asm: LDI	*AR4,R3
    // asm: 	BZ	ODCX
    // asm: 	LDI	R3,AR4
ODCL2:
    // asm: LDI	*+AR4(OID),R2
    // asm: 	CMPI	6D0h,R2
    // asm: 	BNE	HHTT
    // asm: 	CALL	HT_HUNGH
    // asm: 	BU	ODC
HHTT:
    // asm: 	CMPI	621h,R2
    // asm: 	BNE	NTT1
    // asm: 	CALL	HT_HOTTUB
    // asm: 	BU	ODC
NTT1:
    // asm: 	CMPI	622h,R2
    // asm: 	BNE	NTT1A
    // asm: 	CALL	HT_HILLY
    // asm: 	BU	ODC
NTT1A:
    // asm: 	CMPI	623h,R2
    // asm: 	BNE	NTT1B
    // asm: 	CALL	HT_GIRL
    // asm: 	BU	ODC
NTT1B:
    // asm: 	CMPI	615h,R2
    // asm: 	BNE	NTT2
    // asm: 	CALL	HT_RADAR
    // asm: 	BU	ODC
NTT2:
    // asm: CMPI	616h,R2
    // asm: 	BNE	ODC
    // asm: 	CALL	HT_STAGENT
    // asm: 	BU	ODC
ODCX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "START_ANIMATIONS", 0, 0);
    UNIMPL();
}

void HT_HUNGH(void)
{
    // asm: 	PUSH	AR6
    // ;	LDI	*+AR4(OPLINK),AR2
    // ;	CALL	PRC_KILL
    // ;	LDI	*+AR4(OFLAGS),R0
    // ;	ANDN	O_POSTER,R0
    // ;	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDF	1.2,R0
    // asm: 	STF	R0,*+AR4(OMAT00)
    // asm: 	STF	R0,*+AR4(OMAT11)
    // asm: 	STF	R0,*+AR4(OMAT22)
    // asm: 	LDI	@HUNGH_ANISI,AR6
    // asm: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	POP	AR6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_HUNGH", 0, 0);
    UNIMPL();
}

void HT_RUT(void)
{
    // asm: 	PUSH	AR6
    // asm: 	LDI	*+AR4(OPLINK),AR2
    // asm: 	CALL	PRC_KILL
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	ANDN	O_POSTER,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	@RUT_ANISI,AR6
    // asm: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	POP	AR6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_RUT", 0, 0);
    UNIMPL();
}

void HT_HILLY(void)
{
    // asm: 	PUSH	AR6
    // asm: 	LDI	@HILLANII,AR6
    // asm: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	POP	AR6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_HILLY", 0, 0);
    UNIMPL();
}

void HT_GIRL(void)
{
    // asm: 	PUSH	AR6
    // asm: 	LDI	@GIRLANII,AR6
    // asm: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	POP	AR6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_GIRL", 0, 0);
    UNIMPL();
}

void HT_HOTTUB(void)
{
    // asm: 	PUSH	AR6
    // asm: 	STI	AR4,@WATCHOBJ
    // asm: 	LDI	@ANI_HOTTUBI,AR6
    // asm: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	POP	AR6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_HOTTUB", 0, 0);
    UNIMPL();
}

void HT_RADAR(void)
{
    // asm: 	PUSH	AR6
    // asm: 	LDI	@ANI_RADARI,AR6
    // asm: 	CREATE	PLAINANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	POP	AR6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_RADAR", 0, 0);
    UNIMPL();
}

void HT_STAGENT(void)
{
    // asm: 	PUSH	AR6
    // asm: 	LDI	@ANI_STAGI,AR6
    // asm: 	CREATE	AGENTANI_PROC,SPAWNER_C|ANIMATION_T
    // asm: 	POP	AR6
    // asm: 	RETS
PLAINANI_PROC:
    // asm: 	LDI	AR6,AR5
PLAINANI_LP:
    // asm: 	LDI	*AR5++,R0
    // asm: 	BLT	PLAINANI_PROC
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	LDI	9,R4
    // asm: 	RANDN	3
    // asm: 	ADDI	R0,R4
PPDD:
    // asm: SLEEP	1
    // asm: 	SUBI	@NFRAMES,R4
    // asm: 	BLE	PLAINANI_LP
    // asm: 	BU	PPDD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HT_STAGENT", 0, 0);
    UNIMPL();
}

void AGENTANI_PROC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	RANDN	50
    // asm: 	LDI	R0,AR2
    // asm: 	ADDI	20,AR2
    // asm: 	CALL	SLEEP
    // asm: 	LDI	AR6,AR5
AGENTANI_LP:
    // asm: 	LDI	*AR5++,R0
    // asm: 	BLT	AGENTANI_PROC
    // asm: 	CMPI	5,R0
    // asm: 	BNE	JKG
    // asm: 	SLEEP	12
    // asm: 	BU	AGENTANI_LP
JKG:
    // asm: STI	R0,*+AR4(OROMDATA)
    // asm: 	LDI	15,R4
PPDDA:
    // asm: SLEEP	1
    // asm: 	SUBI	@NFRAMES,R4
    // asm: 	BLE	AGENTANI_LP
    // asm: 	BU	PPDDA
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AGENTANI_PROC", 0, 0);
    UNIMPL();
}

void POLITICAL_CARTOON_NOTICE(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@PC1,AR2
    // asm: 	LDI	15,RC
    // asm: 	CALL	POLTXT
    // asm: 	SLEEP	26
    // asm: 	LDI	@PC2,AR2
    // asm: 	LDI	20,RC
    // asm: 	CALL	POLTXT
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POLITICAL_CARTOON_NOTICE", 0, 0);
    UNIMPL();
}

void POLTXT(void)
{
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	20,R3
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm: 	LDL	font18_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR0(TEXT_PAL)
    // asm: 	STI	R0,*+AR1(TEXT_PAL)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POLTXT", 0, 0);
    UNIMPL();
}

void CROWD_IT(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	255,R2
    // asm: 	LDI	CROWD1,AR2
    // asm: 	CALL	AMBIENCE_SOUND
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CROWD_IT", 0, 0);
    UNIMPL();
}

void BABE_TROPHY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // ;	SLEEP	6
    // asm: 	LDI	12,AR5
DABLP3:
    // asm: SLEEP	1
    // asm: 	SUBI	@NFRAMES,AR5
    // asm: 	CMPI	0,AR5
    // asm: 	LDILT	0,AR5
    // asm: 	DBU	AR5,DABLP3
    // asm: 	SOND1	GETREADYTUNE
    // asm: 	CALL	OBJ_GET
    // asm: 	LDI	AR0,AR4
    // asm: 	FLOAT	-50,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	FLOAT	310,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_POSTER|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDL	here1,R0
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	READAUD	ADJ_GIRLS
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NODA
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
NODA:
    // asm: 	LDL	BABE_TROPHY_SCRIPT,AR6
    // asm: 	STI	AR6,*+AR7(BABE_ASTRT)
    // asm: 	LDI	11,AR5
    // asm: BABE_LP
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	3,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CALL	BABE_ANI
    // asm: 	CALL	CROWD_IT
    // asm: 	SLEEP	1
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	3,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CALL	CROWD_IT
    // asm: 	SLEEP	1
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	SUBF	3,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CALL	CROWD_IT
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,BABE_LP
    // asm: 	LDI	13,AR5
    // asm: BBD	CALL	BABE_ANI
    // asm: 	CALL	CROWD_IT
    // asm: 	SLEEP	3
    // asm: 	DBU	AR5,BBD
    // asm: 	SONDFX	CHICK_LAUGH
    // asm: 	LDI	27,AR5
    // asm: BABE_LP2
    // asm: 	CALL	BABE_ANI
    // asm: 	CALL	CROWD_IT
    // asm: 	SLEEP	3
    // asm: 	DBU	AR5,BABE_LP2
    // ;	LDI	25-1,AR5
    // asm: 	LDI	50-1,AR5
    // asm: BABLP3	SLEEP	1
    // asm: 	SUBI	@NFRAMES,AR5
    // asm: 	CMPI	0,AR5
    // asm: 	LDILT	0,AR5
    // asm: 	DBU	AR5,BABLP3
    // asm: BABE_TROPHYX
    // asm: 	CREATEC	BABE_IBO,UTIL_C
    // asm: 	LDI	*+AR7(BABE_ASTRT),R0
    // asm: 	STI	R0,*+AR0(BABE_ASTRT)
    // asm: 	RETP
    // *
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BABE_TROPHY", 0, 0);
    UNIMPL();
}

void BABE_IBO(void)
{
    // asm: 	LDI	6,AR5
    // asm: BABE_LP3
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	15,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	SLEEP	1
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	15,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	SLEEP	1
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	15,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,BABE_LP3
    // asm: 	LDI	0,R0
    // asm: 	LDI	100,R1
    // asm: 	CALL	SET_TRACK_VOL
    // asm: 	SOND1	MAPTUNE
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BABE_IBO", 0, 0);
    UNIMPL();
}

void BABE_ANI(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LONGROUT
BABE_ANIKK:
    // asm: 	LDI	*AR6++,R0
    // asm: 	BN	BX
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	RETS
    // asm: BX	LDI	*+AR7(BABE_ASTRT),AR6
    // asm: 	BU	BABE_ANIKK
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BABE_ANI", 0, 0);
    UNIMPL();
}

void BABE_WAVEFLAG(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // *
    // *
    // asm: 	READAUD	ADJ_GIRLS
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	SUICIDE
    // asm: 	CALL	OBJ_GET
    // asm: 	LDI	AR0,AR4
    // asm: 	FLOAT	-350,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	FLOAT	40,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	FLOAT	368,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_POSTER|O_NOUROT|O_NOUNIV,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDL	flag1,R0
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
BABEWTLP:
    // asm: 	LDI	@BABE_CONTROL,R0
    // asm: 	BNZ	BABEGO
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	FLOAT	-200,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGE	IBO2
    // asm: 	ADDF	5,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
IBO2:
    // asm: 	SLEEP	1
    // asm: 	BU	BABEWTLP
BABEGO:
    // asm: 	LDI	17,AR5
BABERST:
    // asm: 	LDL	BABE_FLAG_SCRIPT,AR6
    // asm: BABE_LPWF
    // asm: 	LDI	*AR6++,R0
    // asm: 	BN	BABERST
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	SLEEP	2
    // asm: 	DBU	AR5,BABE_LPWF
    // asm: 	SLEEP	5
    // asm: 	LDI	16,AR5
    // asm: BABEOFF
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	SUBF	6,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	SLEEP	1
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	SUBF	6,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,BABEOFF
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BABE_WAVEFLAG", 0, 0);
    UNIMPL();
}

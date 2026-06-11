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
#include "../include/roadkill.h"

/*
 * Source module: asm/ROADKILL.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
#define ROADKILL_TYPES (2-1)
#define RKT_DEATH 0
#define RKT_SND 1
#define RKT_PARTS 2
#define RKT_WIDTHL 3
#define RKT_WIDTHR 4
#define RKT_SIZE 5
/* asm: ROADKILLXZ	.bss	ROADKILLXZ,2 */
int ROADKILLXZ[2];
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *This checks collisions between the Player and the roadkill. This is needed
// *because the roadkill are to wide to be covered completely by the POINT
// *collision routine.
// *No special list is needed. This routine simply searches for the ID's. Seeing
// *it is only checking agianst the player the overhead is nominal.
// *----------------------------------------------------------------------------
// *
// *
// *
// *
/* asm: ROADKILL_SOUND_TIMER	.bss	ROADKILL_SOUND_TIMER,1 */
int ROADKILL_SOUND_TIMER;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *	.FILE	"COW.ASM"
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// * COW_PROC	PROC
// *Maintains a COW
// *	CREATE	COW_PROC,RDDEBRIS_C|TSC_ROADKILL|TSC_COW_S
#define LOOP_COUNT PDATA
#define TOTAL_FRAMES (PDATA+1)
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *	.FILE	"DEER.ASM"
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *DEER_BLOOD_PROC	PROC
// *
// *INPUT
// *	AR4	= THE DEER
// *	AR5	= CARBLK THAT HIT THE DEER
// *Creates and maintains a single blood animation
// *	CREATEC	DEER_BLOOD_PROC,UTIL_C
/* asm: DEERBLOOD_ANI	.word	adblud1,adblud2,adblud3,adblud4,adblud5,adblud6,-1 */
int DEERBLOOD_ANI[] = { adblud1, adblud2, adblud3, adblud4, adblud5, adblud6, -1 };
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *	.FILE	"GEESE.ASM"
// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: GEESEANI	GEESEANI: */
/* asm: 	.word	geese1,geeseb,geesec,geesed */
/* asm: 	.word	geesee,geesef,geeseg,geeseh,-1 */
int GEESEANI[] = { geese1, geeseb, geesec, geesed, geesee, geesef, geeseg, geeseh, -1 };
/* asm: GEESE_DIR	GEESE_DIR: */
/* asm: 	.word	250,1 */
int GEESE_DIR[] = { 250, 1 };
// *----------------------------------------------------------------------------
#define SPEED PDATA
#define DIRECTION (PDATA+1)
#define DIR_RAD (PDATA+2)
#define NUM_SPLATS (PDATA+3)
// *----------------------------------------------------------------------------
// * Set by Spawner:
// *	R4	=	SPEED
// *	R5	=	DIRECTION INT (+/-1, Direction to travers road)
// *	R6	=	RADS direction FL
/* asm: SHIT_ANI	.word	bdst,bdst2,bdst3,bdst4,bdst5,bdst6,-1 */
int SHIT_ANI[] = { bdst, bdst2, bdst3, bdst4, bdst5, bdst6, -1 };
// *---------------------------------------------------------------------------
// *---------------------------------------------------------------------------
// *---------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// * BUG_SPAWNER_PROC	PROC
// *Creates several BUG SPLAT PROCS
// *	CREATE	BUG_SPAWNER_PROC,SPAWNER_C
// *
/* asm: BUG_ANI	.word	bug1,bug2,bug3,bug4,bug5,-1 */
int BUG_ANI[] = { bug1, bug2, bug3, bug4, bug5, -1 };
// *---------------------------------------------------------------------------

void PLYRROADKILL(void)
{
    // asm: 	LDI	@PLYCAR,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BEQ	PLYRKX			;NO player at the moment
    // asm: 	LDI	@OACTIVEI,AR1
    // asm: 	LDI	*AR1,R0
    // asm: 	BEQ	PLYRKX
FINDLP:
    // asm: 	LDI	R0,AR1
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	CLASS_M|TYPE_M,R0
    // asm: 	CMPI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm: 	CALLEQ	CHECK_COLLISION
    // asm: 	LDI	*AR1,R0
    // asm: 	BNZ	FINDLP
PLYRKX:
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,@ROADKILLXZ
    // asm: 	STF	R0,@ROADKILLXZ+1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRROADKILL", 0, 0);
    UNIMPL();
}

void CHECK_COLLISION(void)
{
    // *----------------------------------------------------------------------------
    // *AR0	= CAR OBJECT TO CHECK
    // *AR1	= ROADKILL OBJECT
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	SUBTYPE_M,R0
    // asm: 	BZ	CCOLLX			;PARTS ARE NOT COLLIDEABLE
    // asm: 	CMPI	TSC_PARTS_S,R0
    // asm: 	BEQ	CCOLLX			;NORMALL COLLISION WILL DO IT FOR PARTS
    // asm: 	SUBI	1,R0
    // asm: 	MPYI	RKT_SIZE,R0
    // asm: 	LDI	R0,AR3			;AR3 = index into roadkill table
    // asm: 	ADDI	@ROADKILL_TABI,AR3
    // asm: 	LDF	*+AR3(RKT_WIDTHL),R4
    // asm: 	CALL	CHECK_OFFSET
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	SUBTYPE_M,R0
    // asm: 	BZ	COFFX			;OBJECT_COLLIDED
    // asm: 	LDF	*+AR3(RKT_WIDTHR),R4
    // asm: 	CALL	CHECK_OFFSET
CCOLLX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_COLLISION", 0, 0);
    UNIMPL();
}

void CHECK_OFFSET(void)
{
    // *----------------------------------------------------------------------------
    // *AR0	= CAR OBJECT TO CHECK
    // *AR1	= ROADKILL OBJECT
    // *R4	= OFFSET
    // asm: 	PUSH	AR3
    // asm: 	LDF	*+AR1(OPOSX),R0		;Set the roadkills real XZ coords
    // asm: 	STF	R0,@ROADKILLXZ		;A nonzero XZ will let the roadkill
    // asm: 	LDF	*+AR1(OPOSZ),R0		;know that it must use ROADKILLXZ as its
    // asm: 	STF	R0,@ROADKILLXZ+1	;real coords. The road kill will copy them back
    // 	;Set the roadkills offset XZ coords
    // asm: 	LDP	@_CAMERARAD+X
    // asm: 	LDF	@_CAMERARAD+Y,R2
    // asm: 	SETDP
    // asm: 	CALL	NORMITS
    // asm: 	NEGF	R2
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R4,R0
    // asm: 	ADDF	*+AR1(OPOSX),R0
    // asm: 	STF	R0,*+AR1(OPOSX)
    // asm: 	CALL	_SINE
    // asm: 	MPYF	R4,R0
    // asm: 	ADDF	*+AR1(OPOSZ),R0
    // asm: 	STF	R0,*+AR1(OPOSZ)
    // asm: 	CALL	COLSGCK			;This does the actual collision check
    // 					;and also calls explode deer if collision exists
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	SUBTYPE_M,R0
    // asm: 	BZ	COFFX			;OBJECT_COLLIDED
    // asm: 	LDF	@ROADKILLXZ,R0
    // asm: 	STF	R0,*+AR1(OPOSX)
    // asm: 	LDF	@ROADKILLXZ+1,R0
    // asm: 	STF	R0,*+AR1(OPOSZ)
COFFX:
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_OFFSET", 0, 0);
    UNIMPL();
}

void ROADKILL_FLYERP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *PARAMETERS
    // *	AR1	OBJECT
    // *
    // *RETURNS
    // *	C	IS A FLYER
    // *	NC	NOT A FLYER
    // *
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR1(ODIST),R0
    // asm: 	CMPI	30000,R0
    // asm: 	BGT	RKFPX
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	SUBTYPE_M,R0
    // asm: 	CMPI	TSC_PARTS_S,R0
    // asm: 	BNE	NORMAL_ROADKILL
    // asm: 	CALL	FLYING_PARTS		;HANDLE COLLISION
    // asm: 	BR	RKFPX
NORMAL_ROADKILL:
    // asm: 	CALL	ROADKILL_SETKILL
    // asm: 	LDF	@ROADKILLXZ,R0
    // asm: 	ADDF	@ROADKILLXZ+1,R0
    // asm: 	BZ	RKFP
    // asm: 	LDF	@ROADKILLXZ,R0
    // asm: 	STF	R0,*+AR1(OPOSX)
    // asm: 	LDF	@ROADKILLXZ+1,R0
    // asm: 	STF	R0,*+AR1(OPOSZ)
RKFP:
    // asm: 	CALL	DEER_EXPLODE
RKFPX:
    // asm: 	CLRC			;flying taken car of
    // asm: 	POP	AR2
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADKILL_FLYERP", 0, 0);
    UNIMPL();
}

void ROADKILL_HIT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR4
    // asm: 	LDF	@GAME_TIMER,R0
    // asm: 	SUBF	@ROADKILL_SOUND_TIMER,R0
    // asm: 	CMPF	@SOUNDTIME,R0
    // asm: 	BLT	PKQ			;not enough time has lappsed since the last sound
    // asm: 	LDF	@GAME_TIMER,R0
    // asm: 	STF	R0,@ROADKILL_SOUND_TIMER
    // asm: 	LDI	*+AR1(OID),AR2
    // asm: 	AND	SUBTYPE_M,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BZ	PKQ
    // asm: 	CMPI	TSC_PARTS_S,AR2
    // asm: 	BZ	PKQ
    // asm: 	SUBI	1,AR2
    // asm: 	MPYI	RKT_SIZE,AR2
    // asm: 	ADDI	@ROADKILL_TABI,AR2
    // asm: 	LDI	*+AR2(RKT_SND),AR2
    // asm: 	LDI	*+AR0(OID),R0		;CAR that hit the animal
    // asm: 	AND	CLASS_M,R0
    // asm: 	CMPI	PLYR_C,R0
    // asm: 	BEQ	DO_PLYR
    // asm: 	LDI	AR0,AR4			;A drone hit the animal
    // asm: 	CALL	DRONESND1
    // asm: 	BR	PKQ
DO_PLYR:
    // asm: 	CALL	ONESNDFX
    // ;	LDI	*+AR1(OID),AR2
    // ;	AND	SUBTYPE_M,AR2
    // ;	CMPI	TSC_DEER_S,AR2
    // ;	BNE	PKQ
    // asm: 	LDI	MOO,AR2
    // asm: 	CALL	KILLSNDFX
    // asm: 	LDI	EXP1,AR2
    // asm: 	CALL	ONESNDFX
    // ;	LDI	0,R0
    // ;	LDI	0,R1
    // ;	CALL	SET_TRACK_VOL
    // asm: 	BU	PKQ
PKQ:
    // asm: POP	AR4
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADKILL_HIT", 0, 0);
    UNIMPL();
}

void ROADKILL_SETKILL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *INPUT	AR1 = OBJECT
    // *
    // *PARAMTERS
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR3
    // asm: 	LDI	*+AR1(OLINK4),AR0
    // asm: 	CMPI	-1,AR0
    // asm: 	BEQ	NNWAY
    // asm: 	LDI	*+AR1(OID),AR3
    // asm: 	AND	SUBTYPE_M,AR3
    // asm: 	SUBI	1,AR3
    // asm: 	MPYI	RKT_SIZE,AR3
    // asm: 	ADDI	@ROADKILL_TABI,AR3
    // asm: 	LDI	*+AR3(RKT_DEATH),AR0
    // asm: 	STI	AR0,*+AR1(OROMDATA)
    // asm: RS1
    // asm: 	LDI	-1,AR0
    // asm: 	STI	AR0,*+AR4(OLINK4)
NNWAY:
    // asm: 	POP	AR3
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADKILL_SETKILL", 0, 0);
    UNIMPL();
}

void OBJ_MOVE_GROUND(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *INPUT
    // *	R1 = hight off ground
    // *
    // *OUTPUT
    // *	C = 0,no ground under object
    // *	C = 1,Ground under object
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OVELX,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	@VECTORAI,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORAI,AR0
    // asm: 	LDF	*+AR0(X),R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR0(Z),R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	PUSH	AR4
    // asm: 	PUSHF	R1
    // asm: 	ADDI	OPOSX,AR4
    // asm: 	CALL	CAMSCAN
    // asm: 	POPF	R1
    // asm: 	POP	AR4
    // asm: 	BNC	OMGX
    // asm: 	SUBF	R1,R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	SETC
OMGX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_MOVE_GROUND", 0, 0);
    UNIMPL();
}

void GET_ROADKILL_TRACK(void)
{
    // *----------------------------------------------------------------------------
    // *Sets ORADY, OMATRIX, and AR0 to 10 tracks down the road
    // *
    // *
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	LDI	*+AR2(CARTRAK),R0
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	20,AR1
    // asm: GRT_FF
    // asm: 	LDI	AR0,AR2
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OLINK4),R0
    // asm: 	BEQ	OUT_OF_TRACK
    // asm: 	DBU	AR1,GRT_FF
OUT_OF_TRACK:
    // asm: 	LDF	*+AR0(OPOSX),R2
    // asm: 	SUBF	*+AR2(OPOSX),R2
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	SUBF	*+AR2(OPOSZ),R3
    // asm: 	CALL	ARCTANF
    // asm: 	SUBF	HALFPI,R0
    // asm: 	LDF	R0,R2				;FIND THETA
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX			;FIND Y MATRIX (FOR LANE OFFSETTING)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_ROADKILL_TRACK", 0, 0);
    UNIMPL();
}

void PROC_COUNT(void)
{
    // *----------------------------------------------------------------------------
    // *INPUT		R2= PID to count
    // *RETURNS	R0= number of active procs
    // *Trashes AR0,R1
    // asm: 	LDI	0,R0
    // asm: 	LDI	@PACTIVEI,R1
    // asm: 	RETSZ				;NULL LIST?
PCLP:
    // asm: 	LDI	R1,AR0
    // asm: 	LDI	*+AR0(PID),R1
    // asm: 	CMPI	R2,R1
    // asm: 	BNE	PC1
    // asm: 	ADDI	1,R0
PC1:
    // asm: 	LDI	*AR0,R1
    // asm: 	BNZ	PCLP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PROC_COUNT", 0, 0);
    UNIMPL();
}

void COW_SPAWNER_PROC(void)
{
    // *----------------------------------------------------------------------------
    // * COW_SPAWNER_PROC	PROC
    // *This PROC has a COW
    // *	CREATE	COW_SPAWNER_PROC,SPAWNER_C
    // *
    // asm: 	READADJ	ADJ_ROADKILL
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	CSPX
    // asm: 	LDI	adblud1_p,AR2		;overwrite the deer parts with the cow palette
    // asm: 	CALL	PAL_FIND
    // asm: 	BC	CSPSLEEP
    // asm: 	LDI	R0,R2
    // asm: 	LDL	cow_pal,AR2
    // asm: 	LDI	*AR2++,R3
    // asm: 	CALL	PAL_SET
CSPSLEEP:
    // asm: 	SLEEP	1			;wait for game to start
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MGO,R0
    // asm: 	BZ	CSPSLEEP
CSPLP:
    // asm: 	SLEEP	30*20
    // ;	SLEEP	30*5
    // asm: 	LDI	500,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BNC	CSPLP
    // asm: 	LDI	RDDEBRIS_C|TSC_ROADKILL|TSC_COW_S,R2
    // asm: 	CALL	PROC_COUNT
    // asm: 	CMPI	1,R0
    // asm: 	BGE	CSPLP
    // asm: 	CREATE	COW_PROC,RDDEBRIS_C|TSC_ROADKILL|TSC_COW_S
    // asm: 	BR	CSPLP
CSPX:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COW_SPAWNER_PROC", 0, 0);
    UNIMPL();
}

void COW_PROC(void)
{
    // asm: 	LDL	kow3,AR2
    // asm: 	FLOAT	10,R4			;Hight of a cow
    // asm: 	CALL	INIT_COW
    // asm: 	BC	COW_DIE
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(LOOP_COUNT)
    // asm: 	STF	R0,*+AR7(TOTAL_FRAMES)
COW_WAIT:
    // asm: 	SLEEP	1
    // asm: 	LDF	1,R1
    // asm: 	ADDF	*+AR7(LOOP_COUNT),R1
    // asm: 	STF	R1,*+AR7(LOOP_COUNT)
    // asm: 	FLOAT	@NFRAMES,R0
    // asm: 	ADDF	*+AR7(TOTAL_FRAMES),R0
    // asm: 	STF	R0,*+AR7(TOTAL_FRAMES)
    // asm: 	CALL	DIV_F
    // asm: 	LDF	R0,R1
    // asm: 	LDI	@PLYCBLK,AR5
    // asm: 	MPYF	*+AR5(CARSPEED),R1
    // asm: 	MPYF	1.5,R1			;CARSPEED FUDGE 'O RAMA
    // asm: 	FLOAT	*+AR4(ODIST),R0
    // asm: 	CALL	DIV_F
    // asm: 	CMPF	20,R0			;APPROX NUMBER OF FRAMES TO COW
    // asm: 	BGT	COW_WAIT
    // asm: 	SONDFX	MOO
COW_SLEEP:
    // asm: 	SLEEP	100			;THIS IS NEEDED FOR COW COUNTING
    // asm: 	BR	COW_SLEEP
COW_DIE:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COW_PROC", 0, 0);
    UNIMPL();
}

void DEER_SPAWNER_PROC(void)
{
    // *----------------------------------------------------------------------------
    // * DEER_SPAWNER_PROC	PROC
    // *Maintains several DEER animations
    // *	CREATE	DEER_SPAWNER_PROC,SPAWNER_C
    // *
    // asm: 	READADJ	ADJ_ROADKILL
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	DSPX
    // asm: 	SLEEP	1			;wait for game to start
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MGO,R0
    // asm: 	BZ	DEER_SPAWNER_PROC
DSPLP:
    // asm: 	SLEEP	30*20
    // ;	SLEEP	30*2
    // asm: 	LDI	500,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BNC	DSPLP
    // asm: 	LDI	RDDEBRIS_C|TSC_ROADKILL|TSC_DEER_S,R2
    // asm: 	CALL	PROC_COUNT
    // asm: 	CMPI	1,R0
    // asm: 	BGE	DSPLP
    // asm: 	CREATE	DEER_PROC,RDDEBRIS_C|TSC_ROADKILL|TSC_DEER_S
    // asm: 	BR	DSPLP
DSPX:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEER_SPAWNER_PROC", 0, 0);
    UNIMPL();
}

void DEER_PROC(void)
{
    // *----------------------------------------------------------------------------
    // * DEER_PROC	PROC
    // *Maintains several DEER animations
    // *	CREATE	DEER_PROC,RDDEBRIS_C|TSC_ROADKILL|TSC_DEER_S
    // asm: 	FLOAT	219,R4			;Hight of a deer
    // asm: 	CALL	INIT_DEER
    // asm: 	BC	DEER_DIE
    // asm: 	LDI	@DEERANII,AR6
DEERANI_LOOP:
    // asm: 	LDI	*++AR6,R0
    // asm: 	LDIN	@DEERANII,AR6
    // asm: 	LDI	*AR6,R0
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: DEER_SLEEP
    // asm: 	SLEEP	3			;frame rate of 3
    // asm: 	BR	DEERANI_LOOP		;Ultimatly backgrnd or colla will kill this proc
DEER_DIE:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEER_PROC", 0, 0);
    UNIMPL();
}

void INIT_DEER(void)
{
    // *----------------------------------------------------------------------------
    // * INIT_DEER
    // *INPUT R4 = hight to place off ground
    // *Creates and maintains a DEER animation
    // asm: 	LDI	@DEERANII,AR6
    // asm: 	LDI	*AR6,AR2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_DEER", 0, 0);
    UNIMPL();
}

void INIT_COW(void)
{
    // asm: 	LDF	1,R6
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	INIT_DEERX
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	ADD_RDDEBRIS
    // asm: 	LDI	1,R0			;PROC ME BABY
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	STI	AR7,*+AR4(OPLINK)
    // ;	LDI	RDDEBRIS_C|TSC_ROADKILL|TSC_DEER_S,R0
    // asm: 	LDI	*+AR7(PID),R0		;MAKE SAME AS PROC
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	CALL	GET_ROADKILL_TRACK
    // asm: 	FLOAT	500,R0			;calculate the random offset that this obj will have
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R1
    // asm: 	FLOAT	500,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R2
    // asm: 	LDF	*+AR0(OPOSX),R0			;set to road position
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	ADDF	R2,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	FLOAT	750,R0			;Side of road
    // asm: 	MPYF	R6,R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	LDF	R4,R1			;Hight above ground
    // asm: 	CALL	OBJ_MOVE_GROUND		;Offset the starting point
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	CLRC
INIT_DEERX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_COW", 0, 0);
    UNIMPL();
}

void DEER_EXPLODE(void)
{
    // asm: 	CALL	PUSHALL
    // asm: 	LDI	AR1,AR6		;save off pointer to deer's object
    // asm: 	LDI	*+AR6(OID),R0
    // asm: 	AND	SUBTYPE_M,R0
    // asm: 	SUBI	1,R0
    // asm: 	MPYI	RKT_SIZE,R0
    // asm: 	LDI	R0,AR1
    // asm: 	ADDI	@ROADKILL_TABI,AR1
    // asm: 	LDI	*+AR1(RKT_PARTS),AR1
    // ;	LDI	@DEER_PARTSI,AR1
    // asm: 	LDI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm: 	STI	R0,*+AR6(OID)
MAKE_PARTSLP:
    // asm: 	LDI	*AR1++,R0
    // asm: 	BN	DEER_EXPX
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	DEER_EXPX
    // asm: 	PUSH	AR1
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	ADD_RDDEBRIS
    // asm: 	LDI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDF	*+AR6(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR6(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR6(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	CALL	FLY_PARTS
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	POP	AR1
    // asm: 	LDI	*AR1++,R0
    // asm: 	CALLNE	MAKE_NOCOLL
    // asm: 	BR	MAKE_PARTSLP
DEER_EXPX:
    // asm: 	LDI	AR6,AR4
    // asm: 	NEGF	*+AR4(OVELX),R7		;Deer distance from center
    // asm: 	CALL	FLY_PARTS
    // asm: 	CREATEC	DEER_BLOOD_PROC,UTIL_C
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BZ	NO_SPINOUT		;First person
    // asm: 	LDF	*+AR5(CARDIST2CNTR),R0
    // asm: 	SUBF	R0,R7			;Deer distance from center
    // asm: 	LDF	*+AR5(CARSPEED),R0	;SPINSPEED = SPEED+distance to center of deer
    // asm: 	CMPF	0,R7
    // asm: 	BGE	NO_NEG
    // asm: 	NEGF	R0
NO_NEG:
    // asm: 	ADDF	R7,R0
    // asm: 	LDF	@SPINSPEEDF,R1
    // asm: 	MPYF	R1,R0			;SPIN SPEED factor adjustment
    // asm: 	ABSF	R0,R1
    // asm: 	CMPF	0.06,R1
    // asm: 	BLT	NO_SPINOUT		;To slow to spin
    // asm: 	STF	R0,*+AR5(CARDROT)
    // asm: 	LDF	3.14,R0
    // asm: 	STF	R0,*+AR5(CARSPRAD)
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
NO_SPINOUT:
    // asm: 	LDF	*+AR5(CARSPEED),R0		;Nuke the car that hit it
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEER_EXPLODE", 0, 0);
    UNIMPL();
}

void MAKE_NOCOLL(void)
{
    // asm: 	LDI	RDDEBRIS_C|TSC_ROADKILL|TSC_PARTS_S,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	ANDN	O_NOCOLL,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_NOCOLL", 0, 0);
    UNIMPL();
}

void DEER_BLOOD_PROC(void)
{
    // asm: 	LDL	DEERBLOOD_ANI,AR6
    // asm: 	LDI	*AR6++,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	DBP_DIE
    // asm: 	ORM	O_POSTER|O_NOCOLL,*+AR0(OFLAGS)
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	-150,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	AR0,AR4
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	CALL	_SINE
    // asm: 	NEGF	R0,R3
    // asm: 	CALL	_COSI
    // asm: 	LDF	*+AR5(CARSPEED),R1	;GET CURRENT SPEED
    // asm: 	MPYF	1.5,R1
    // asm: 	MPYF	1.5,R1			;150% the speed of the car
    // asm: 	MPYF	R1,R3
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R3,*+AR4(OVELX)	       	;SETUP VELOCITIES
    // asm: 	STF	R0,*+AR4(OVELZ)
    // asm: 	CALL	OVELNADD		;Update position based on velocity
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
DBP_LP:
    // asm: 	SLEEP	1
    // asm: 	LDI	*AR6++,R0
    // asm: 	BN	DBP_DONE
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	CALL	OVELNADD		;Update position based on velocity
    // asm: 	BR	DBP_LP
DBP_DONE:
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
DBP_DIE:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEER_BLOOD_PROC", 0, 0);
    UNIMPL();
}

void FLYING_PARTS(void)
{
    // *----------------------------------------------------------------------------
    // *CALLABLE FROM COLLA
    // asm: 	CALL	PUSHALL
    // asm: 	LDI	AR1,AR4
    // asm: 	CALL	FLY_PARTS
    // asm: 	CALL	MAKE_NOCOLL
    // asm: 	CALL	POPALL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYING_PARTS", 0, 0);
    UNIMPL();
}

void FLY_PARTS(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *FLY PARTS			;YES THIS WAS RIPPED OF FROM COLLA
    // *	AR5	POINTS TO CAR OBJECT
    // *	AR4	POINTS TO OBJECT HIT
    // *This routine is needed to insure that the parts do fly
    // *When I just simply added them the would not collide with the cars if they were going fast
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	LDFZ	1.5,R6			;First person
    // asm: 	LDFNZ	2.4,R6			;Not first person
    // asm: 	LDF	1.0,R1
    // asm: 	LDF	0.12,R0 		;ADD RANDOM ROTATION
    // asm: 	MPYF	R6,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R2
    // asm: 	ADDF	*+AR5(CARVROT),R2
    // asm: 	LDF	0.50,R0			;RANDOM SPEED MULTIPLIER
    // asm: 	MPYF	R6,R0
    // asm: 	CALL	FRAND
    // asm: 	LDF	R0,R1
    // asm: 	ADDF	0.7,R1
    // ;	MPYF	1.3,R1			;SPEEDFUDGE FACTOR
    // asm: 	CALL	_SINE
    // asm: 	NEGF	R0,R3
    // asm: 	CALL	_COSI
    // asm: 	LDF	*+AR5(CARSPEED),R2	;GET CURRENT SPEED
    // asm: 	CMPF	100,R2
    // asm: 	LDFLT	100,R2
    // asm: 	MPYF	R2,R1
    // asm: 	MPYF	R1,R3
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R3,*+AR4(OVELX)	       	;SETUP VELOCITIES
    // asm: 	STF	R0,*+AR4(OVELZ)
    // asm: 	LDF	-0.05,R0
    // asm: 	MPYF	R6,R0
    // asm: 	CALL	FRAND
    // asm: 	LDF	-0.10,R1
    // asm: 	MPYF	R6,R1
    // asm: 	ADDF	R1,R0
    // asm: 	MPYF	1.5,R0			;SPEEDFUDGE FACTOR
    // ;	MPYF	*+AR5(CARSPEED),R0	;GET CURRENT SPEED
    // asm: 	MPYF	R2,R0
    // asm: 	LDF	-27,R1
    // asm: 	MPYF	R6,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0		  	;MAX VERTICAL VELOCITY
    // asm: 	STF	R0,*+AR4(OVELY)		;STUFF VERTICAL VELOCITY
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	TSTB	*+AR4(OFLAGS),R0	;PROCESS ALREADY ACTIVE	?
    // asm: 	LDINZ	*+AR4(OPLINK),AR2	;YES, KILL HIM OFF...
    // asm: 	CALLNZ	PRC_KILL
    // asm: 	LDI	@FLYCOLLPI,AR2		;GET SIGN FLY PROCESS
    // asm: 	LDI	DRONE_C|FLYER_T,R2
    // asm: 	CALL	PRC_CREATE_CHILD	;CREATE A PROCESS
    // asm: 	BC	FLY_PARTSX		;NOTHING AVAILABLE, QUIT
    // asm: 	STI	AR0,*+AR4(OPLINK)	;SAVE PROCESS LINK
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_3DROT_B,R0		;FLAG CAN AS NON-2D OPTIMIZABLE
    // asm: 	LDI	1,R1
    // asm: 	LSH	O_PROC_B,R1		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	ADDI	R1,R0
    // asm: 	OR	*+AR4(OFLAGS),R0	;SET YOUR FLAGS...
    // asm: 	OR	O_NOCOLL,R0		;Parts can only be hit once
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	0,R0			;KILL OFF GROUP REFERENCE
    // asm: 	STI	R0,*+AR4(OLINK2)
FLY_PARTSX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLY_PARTS", 0, 0);
    UNIMPL();
}

void GEESE_SPAWNER(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	SLEEP	1
    // asm: 	LDI	@PLYCAR,R0
    // asm: 	BZ	GEESE_SPAWNER
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MGO,R0
    // asm: 	BZ	GEESE_SPAWNER
GOOSE_ME:
    // asm: 	SLEEP	30*20		;sleep approx. 20 seconds
    // ;	SLEEP	30*5
    // asm: 	LDI	500,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BNC	GOOSE_ME
    // asm: 	LDI	4,AR2
    // asm: 	CALL	RANDU0
    // asm: 	MPYI	3,R0
    // asm: 	LDI	@GEESE_DIRI,AR0
    // asm: 	ADDI	R0,AR0
    // asm: 	LDI	*AR0++,R4
    // asm: 	LDI	*AR0++,R5
    // 		;register is pushed as INT
    // asm: 	LDF	*AR0++,R6
    // asm: 	CREATE	GEESE_PROC,UTIL_C
    // asm: 	BR	GOOSE_ME
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GEESE_SPAWNER", 0, 0);
    UNIMPL();
}

void GEESE_PROC(void)
{
    // asm: 	FLOAT	R4
    // asm: 	STF	R4,*+AR7(SPEED)
    // asm: 	STI	R5,*+AR7(DIRECTION)
    // asm: 	STF	R6,*+AR7(DIR_RAD)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(NUM_SPLATS)
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	GEESE_DIE
    // asm: 	LDI	AR0,AR4
    // asm: 	SONDFX	GEESES
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	LDI	*+AR2(CARTRAK),AR2	;Get the track piece the car is on
    // asm: 	LDI	*+AR2(OUSR1),R2
    // asm: 	CALL	FIND_MAP
    // asm: 	ADDI	20*4,AR2		;bump it up 40 road segments
    // asm: 	LDI	AR2,AR6			;save pointer to ROAD LEG
    // asm: 	FLOAT	*+AR6(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR7(DIR_RAD),R2	;flying with the road?
    // asm: 	CMPF	0,R2
    // asm: 	BEQ	NO_XOFFSET
    // asm: 	LDI	*+AR7(DIRECTION),R2
    // asm: 	LDI	AR6,AR2
    // asm: 	CALL	GET_ROAD_RADY
    // asm: 	ADDF	HALFPI,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CALL	CLR_VECTORA		;Move the flock ('o geese off the side of the road)
    // asm: 	FLOAT	2500,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR2(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
NO_XOFFSET:
    // asm: 	FLOAT	-3000,R1
    // asm: 	FLOAT	*+AR6(Y),R0
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	FLOAT	*+AR6(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
GEESERST:
    // asm: 	LDI	@GEESEANII,AR5
GEESELP:
    // asm: 	LDI	*AR5++,R0
    // asm: 	BN	GEESERST
    // asm: 	STI	R0,*+AR4(OROMDATA)
    // asm: 	LDI	*+AR7(DIRECTION),R2
    // asm: 	LDI	AR6,AR2
    // asm: 	CALL	GET_ROAD_RADY
    // asm: 	ADDF	*+AR7(DIR_RAD),R2
    // asm: 	PUSHF	R2
    // asm: 	ADDF	PI,R2			;Geese point in wrong direction
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	POPF	R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CALL	CLR_VECTORA		;Move the flock ('o geese)
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDF	*+AR0(CARSPEED),R0
    // asm: 	MPYF	0.4,R0
    // asm: 	LDI	*+AR7(DIRECTION),R1
    // asm: 	MPYI	@NFRAMES,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(SPEED),R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR2(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR2(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	PUSH	AR4
    // asm: 	ADDI	OPOSX,AR4
    // asm: 	CALL	CAMSCAN
    // asm: 	POP	AR4
    // asm: 	BNC	GEESE_DONE		;FLU off the edge of the earth
    // asm: 	FLOAT	3000,R1
    // asm: 	SUBF	R1,R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: OFF_MAP
    // asm: 	LDI	AR6,AR2
    // asm: 	LDI	*+AR7(DIRECTION),R2
    // asm: 	CALL	NEXT_ROAD
    // asm: 	LDI	AR2,AR6
    // asm: 	CMPI	@LEG_MAPI,AR6
    // asm: 	BLE	GEESE_DONE
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	LDI	*+AR2(CARTRAK),AR2	;Get the track piece the car is on
    // asm: 	LDI	*+AR2(OUSR1),R2
    // asm: 	CALL	FIND_MAP
    // asm: 	LDI	AR2,AR0
    // asm: 	ADDI	-10*4,AR2		;track sections behind the player
    // asm: 	CMPI	AR2,AR6
    // asm: 	BLE	GEESE_DONE
    // asm: 	ADDI	30*4,AR0		;track sections beyond the player
    // asm: 	CMPI	AR0,AR6
    // asm: 	BGE	GEESE_DONE
    // asm: GEESE_SLEEP
    // asm: 	SLEEP	1
    // asm: 	READADJ	ADJ_ROADKILL
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NO_SPLAT
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BNZ	NO_SPLAT		;NOT in First person
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	BN	NO_SPLAT
    // asm: 	CALL	DIST_TO_PLYR
    // asm: 	FLOAT	5000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	NO_SPLAT
    // asm: 	LDI	AR6,AR2
    // asm: 	LDI	1,R2
    // asm: 	CALL	GET_ROAD_RADY
    // asm: 	NEGF	R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	*+AR6(X),R1
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	FLOAT	*+AR6(Z),R1
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	@VECTORAI,R3
    // asm: 	LDI	R3,AR3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORAI,AR0
    // asm: 	LDF	*+AR0(X),R0
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	SUBF	*+AR0(CARDIST2CNTR),R0
    // asm: 	ABSF	R0
    // asm: 	FLOAT	2000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	NO_SPLAT
    // asm: 	LDI	800,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BNC	NO_SPLAT
    // asm: 	LDI	*+AR7(NUM_SPLATS),R0
    // asm: 	CMPI	6,R0
    // asm: 	BGT	NO_SPLAT
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,*+AR7(NUM_SPLATS)
    // asm: 	LDI	@SHIT_ANII,R5
    // asm: 	CREATE	SPLAT_PROC,UTIL_C	;The SPLAT will hit the windshield
NO_SPLAT:
    // asm: 	BU	GEESELP
GEESE_DONE:
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	GEESES,AR2
    // asm: 	CALL	KILLSNDFX
GEESE_DIE:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GEESE_PROC", 0, 0);
    UNIMPL();
}

void BUG_SPAWNER_PROC(void)
{
    // asm: 	READADJ	ADJ_ROADKILL
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	BSPX
    // asm: 	SLEEP	1			;wait for game to start
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MGO,R0
    // asm: 	BZ	BUG_SPAWNER_PROC
BSPLP:
    // asm: 	SLEEP	30*5
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BNZ	BSPLP			;NOT in First person
    // asm: 	LDI	300,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BNC	BSPLP
    // asm: 	SONDFX	BUGBUZZ
    // asm: 	SONDFX	BUGBUZZ
    // asm: 	SLEEP	10
    // asm: 	LDI	@BUG_ANII,R5
    // asm: 	CREATE	SPLAT_PROC,UTIL_C	;The SPLAT will hit the windshield
    // asm: 	SLEEP	30*20			;20 seconds before another bug can apear
    // asm: 	BR	BSPLP
BSPX:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BUG_SPAWNER_PROC", 0, 0);
    UNIMPL();
}

void SPLAT_PROC(void)
{
    // *---------------------------------------------------------------------------
    // *SPLAT PROC
    // *R5 points to start of animation
    // asm: 	LDI	@PLYCBLK,AR5
    // asm: 	LDI	R5,AR6
    // asm: 	LDI	*AR6++,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	PLYR_C|PLYR_SPLAT_S,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDF	100,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	FLOAT	-300,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	FLOAT	2000,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDI	O_POSTER|O_NOCOLL|O_NOROTS|O_NOUNIV,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	1,R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	1.2,R0
    // asm: 	LDF	R0,R6
    // asm: 	MPYF	0.1,R6
SPLAT_LP:
    // asm: 	SLEEP	1
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BNZ	SPLAT_DONE		;NOT in First person
    // asm: 	FLOAT	@NFRAMES,R0		;Calculate the rate that the camera is moving
    // asm: 	MPYF	*+AR5(CARSPEED),R0
    // asm: 	MPYF	-0.8,R0			;NORMAL SPEED WOULD BE -1.5
    // asm: 	CMPF	-120,R0			;Minimum velocity
    // asm: 	LDFGT	-120,R0
    // asm: 	NEGF	R0,R1
    // asm: 	MPYF	R6,R1			;Move down 1/10th the rate as coming at you
    // asm: 	ADDF	*+AR4(OPOSY),R1
    // asm: 	STF	R1,*+AR4(OPOSY)
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	FLOAT	368,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	BGT	SPLAT_LP
    // asm: 	LDI	BUGBUZZ,AR2
    // asm: 	CALL	KILLSNDFX
    // asm: 	SONDFX	RK_SPLAT1
    // asm: 	LDI	*AR6++,R4
SPLAT_LP1:
    // asm: 	STI	R4,*+AR4(OROMDATA)
    // asm: 	SLEEP	2
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BNZ	SPLAT_DONE		;NOT in First person
    // asm: 	LDI	*AR6++,R4
    // asm: 	BP	SPLAT_LP1
    // asm: 	LDF	*+AR4(OPOSX),R2
    // asm: 	LDF	*+AR4(OPOSY),R3
    // asm: 	CALL	ARCTANF
    // asm: 	LDF	R0,R2
    // asm: 	CALL	_COSI
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	CALL	_SINE
    // asm: 	STF	R0,*+AR4(OVELY)
SPLAT_LP2:
    // asm: 	SLEEP	1
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	BNZ	SPLAT_DONE		;NOT in First person
    // asm: 	FLOAT	@NFRAMES,R2
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	MPYF	0.2,R0
    // asm: 	ADDF	*+AR4(OVELX),R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	MPYF	R2,R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OVELY),R1
    // asm: 	MPYF	0.1,R1
    // asm: 	ADDF	*+AR4(OVELY),R1
    // asm: 	STF	R1,*+AR4(OVELY)
    // asm: 	MPYF	R2,R1
    // asm: 	ADDF	*+AR4(OPOSY),R1
    // asm: 	STF	R1,*+AR4(OPOSY)
    // asm: 	FLOAT	300,R2
    // asm: 	ABSF	R0
    // asm: 	CMPF	R2,R0
    // asm: 	BGT	SPLAT_DONE
    // asm: 	CMPF	R2,R1
    // asm: 	BLE	SPLAT_LP2
SPLAT_DONE:
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: SPLAT_DIE
    // asm: 	LDI	BUGBUZZ,AR2		;Make sure that it dies (it loops)
    // asm: 	CALL	KILLSNDFX
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPLAT_PROC", 0, 0);
    UNIMPL();
}

void DELETE_SPLAT(void)
{
DBSLP:
    // asm: 	LDI	PLYR_C|PLYR_SPLAT_S,AR2
    // asm: 	CALL	OBJ_FIND_FIRST_PRIORITY
    // asm: 	BNC	DBSX
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	BR	DBSLP
DBSX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELETE_SPLAT", 0, 0);
    UNIMPL();
}

void NEXT_ROAD(void)
{
    // *---------------------------------------------------------------------------
    // * Updates AR2 to point to the road segment that the camera is on
    // * NOTE: uses Stealth mode
    // * INPUT	AR2 = segment on
    // *	AR4 = object moving
    // *	R2  = Direction moving	INT (+/-1)
    // * OUTPUT AR2 = segment on (updated)
    // *	R2 = # of segments moved
    // asm: 	LDI	R2,R4
    // asm: 	MPYI	4,R4
    // asm: 	LDI	0,R3
NEXT_ROADLP:
    // asm: 	FLOAT	*+AR2(X),R0
    // asm: 	SUBF	*+AR4(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	FLOAT	*+AR2(Z),R1
    // asm: 	SUBF	*+AR4(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R0
    // asm: 	LDI	AR2,AR0
    // asm: 	ADDI	R4,AR0
    // asm: 	FLOAT	*+AR0(X),R1
    // asm: 	SUBF	*+AR4(OPOSX),R1
    // asm: 	MPYF	R1,R1
    // asm: 	FLOAT	*+AR0(Z),R2
    // asm: 	SUBF	*+AR4(OPOSZ),R2
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R2,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BLE	NEXTRX			;didn't move
    // asm: 	ADDI	R4,AR2
    // asm: 	CMPI	@LEG_MAPI,AR2
    // asm: 	LDILT	@LEG_MAPI,AR2
    // asm: 	BLT	NEXTRX			;don't fly off the back of the map
    // asm: 	ADDI	1,R3			;R3 is the number of road segments moved
    // asm: 	BR	NEXT_ROADLP		;See if the road segment beyond is even closer
NEXTRX:
    // asm: 	LDI	R3,R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NEXT_ROAD", 0, 0);
    UNIMPL();
}

void GET_ROAD_RADY(void)
{
    // *---------------------------------------------------------------------------
    // *INPUT	AR2 = POINTER to LEG_MAP where road is
    // *	R2  = Direction moving	INT (+/-1)
    // *
    // *OUTPUT	R2 = DIRECTION of road
    // asm: 	MPYI	4*2,R2
    // asm: 	LDI	AR2,AR1
    // asm: 	ADDI	R2,AR1
    // asm: 	LDI	*+AR1(Z),R3
    // asm: 	SUBI	*+AR2(Z),R3
    // asm: 	FLOAT	R3
    // asm: 	LDI	*+AR1(X),R2
    // asm: 	SUBI	*+AR2(X),R2
    // asm: 	FLOAT	R2
    // asm: 	CALL	ARCTANF
    // asm: 	LDF	R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	SUBF	HALFPI,R2
    // asm: 	CALL	NORMITS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_ROAD_RADY", 0, 0);
    UNIMPL();
}

void FIND_MAP(void)
{
    // *---------------------------------------------------------------------------
    // *
    // *GET POINTER TO MAP_LEG, ID IN R2
    // *INPUTS  R2=OUSR1
    // *OUPTUTS AR2=POINTER TO place in LEG_MAP
    // asm: 	LDI	@LEG_MAPI,AR2		;FIND THE POSITION IN THE MAP
    // asm: 	ADDI	3,AR2				;OFFSET TO ID
FIND_LP:
    // asm: 	LDI	*AR2++(4),R0
    // asm: 	CMPI	R2,R0
    // asm: 	BLT	FIND_LP
    // asm: FINDX
    // asm: 	SUBI	7,AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_MAP", 0, 0);
    UNIMPL();
}

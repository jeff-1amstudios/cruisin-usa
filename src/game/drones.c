#include "drones.h"

#include "../core/machine.h"
#include "../core/romreader.h"
#include "c30.h"
#include "cmos.h"
#include "delta.h"
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
 * Source module: asm/DRONES.ASM
 */

void FIND_PLAYERS_POSITION(OBJ* player_obj /*AR4*/, CARBLK* player_carblk /*AR5*/);
void DRONE_PTR_ADD(void);
void DRONE_CLR(void);
static void CK_LINK_DISP(void);
void SET_DRONE_PAL(void);
int GET_LANES(OBJ* obj /*AR2*/);
void DIST_TO_PLYR(void);
void INIT_TRACKING_PIECE(void);
void GET_TRACK_POS_RVS_XLANE(void);
void GET_TRACK_POS_RVS(void);
void DELTA_GET_TRACK_POS(void);
void GET_TRACK_POS(void);
void SUB_FUNCTION_RVS(void);
void SUB_FUNCTION(void);
void SUB_FUNCTION_RVS_XLANE(void);
#define SUB_FUNCTION_XLANE DELTA_SUB_FUNCTION
void DELTA_SUB_FUNCTION(void);
void INIT_DRONES(void);
void ADD_DRONE(OBJ* obj /*AR4*/);
void FREE_DRONE(void);
void EXP_PUFF(void);
void PRECOLLIDE_PLYR(void);
float DRONE_RIDE_RIGHT(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/);
float PLYR_RIDE_RIGHT(void);
void SMOKE_PUFF(void);
void DROP_COCONUTS(void);
void AHEAD_OF_PLAYER_P(void);

#define SUB_FUNCTION_XLANE DELTA_SUB_FUNCTION
#define LANEPI LANEP
#define EXP_ANII EXP_ANI
#define SMOKE_ANII SMOKE_ANI
#define MODELTABI MODELTAB
#define TEXTTABSI TEXTTABS

void COMPTRAK(void);

static int EXP_ANI[12];
static int SMOKE_ANI[28];
static const char* VETETXT[6];
static const char* RODRTXT[6];
static const char* BULLTXT[6];
static const char* FERRTXT[6];
static const char HRT12[];
static const char HRT13[];
static const char HRT14[];
static const char HRT15[];
static const char HRT16[];
static const char HRS11[];
static const char HRS12[];
static const char HRS13[];
static const char HRS14[];
static const char HRS15[];
static const char HRS16[];
static const char HRS21[];
static const char HRS22[];
static const char HRS23[];
static const char HRS24[];
static const char HRS25[];
static const char HRS26[];
static const char HRS31[];
static const char HRS32[];
static const char HRS33[];
static const char HRS34[];
static const char HRS35[];
static const char HRS36[];
static const char HRS41[];
static const char HRS42[];
static const char HRS43[];
static const char HRS44[];
static const char HRS45[];
static const char HRS46[];

/*
 *----------------------------------------------------------------------------
 *COMMON DRONE ROUTINES
 */

/*
 *----------------------------------------------------------------------------
 *DRONE TYPES
 *	DELTA	intelligent, race vs plyr
 *	RHO	oncoming traffic + RHO WEAVER
 *	SIGMA	slow moving same direction as plyr (obstacles)
 *		+ WEAVER SIGMA
 *	COPCAR
 *	CHOPPER	the helicopter
 *
 */

/* asm: DRONE_DISPATCH_P	.bss	DRONE_DISPATCH_P,1 */
int DRONE_DISPATCH_P;
/* asm: SUSPEND_MODE	.bss	SUSPEND_MODE,1 */
int SUSPEND_MODE;
/* asm: RACER_GRID_START	.bss	RACER_GRID_START,1 */
int RACER_GRID_START;
// *----------------------------------------------------------------------------
/* asm: DRONE_COUNT	.bss	DRONE_COUNT,1 */
int DRONE_COUNT;
/* asm: MIN_TRACK_TIME	.bss	MIN_TRACK_TIME,1 */
int MIN_TRACK_TIME;
/* asm: PSYCHO_RHO	.bss	PSYCHO_RHO,1 */
static int PSYCHO_RHO;
/* asm: LANEP	.word	LANES,LANES4 */
float* LANEP[] = {
    LANES,
    LANES4,
};
/* asm: LANES	.float	-576.0,-576.0,576.0,576.0	;TWO & 2/2 LANE */
float LANES[] = {
    -576.0f,
    -576.0f,
    576.0f,
    576.0f, // TWO & 2/2 LANE
};
/* asm: LANES4	.float	-1728.0,-576.0,576.0,1728.0	;TWO & 2/2 LANE */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
float LANES4[] = {
    -1728.0f,
    -576.0f,
    576.0f,
    1728.0f, // TWO & 2/2 LANE
    // ----------------------------------------------------------------------------
};
/* asm: ONCSCREEN_CARS	.bss	ONCSCREEN_CARS,1 */
int ONCSCREEN_CARS;

void POSITION_FINDER(PROC* p) {
    switch (p->resume_state) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    }
    // asm 000065CA: 	LDI	@PLYCAR,AR4
    // asm 000065CB: 	LDI	@PLYCBLK,AR5
    // asm 000065CC: 	CALL	FIND_PLAYERS_POSITION
    FIND_PLAYERS_POSITION(PLYCAR, PLYCBLK);
    // asm 000065CD: 	SLEEP	14
    SLEEP(14, 1);
    // asm 000065CF: 	BU	POSITION_FINDER
    REENTER(POSITION_FINDER);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *FIND PLAYERS POSITION, AND # OF CARS CLOSE ONSCREEN
 *
 *PARAMETERS
 *	AR4	PLAYERS OBJECT
 *	AR5	PLAYERS CARBLOCK
 *
 * OUTPUT:SETS @POSITION, @ONCSCREEN_CARS
 *
 */
void FIND_PLAYERS_POSITION(OBJ* player_obj /*AR4*/, CARBLK* player_carblk /*AR5*/) {
    OBJ* player_track_obj;
    int player_track_id;
    OBJ* obj;
    int position;
    int on_screen_cars;
    int rank_increment;
    CARBLK* carblk;
    PROC* proc;
    OBJ* track_obj;
    OBJ* next_track_obj;
    float other_dist_sq;
    float player_dist_sq;

    // ;	CLRI	IR0	;TEMP FLAG FOR OTHER MACHINE
    // asm 000065D0: 	LDI	1,R7		;POSITION #
    position = 1;
    // asm 000065D1: 	CLRI	R6		;CARS CLOSE TO SCREEN
    on_screen_cars = 0;

    // bugfix. Added null check. the first call `player_carblk` is null and the original game reads
    // junk track_id data
    if (player_carblk == NULL) {
        player_track_id = -1;
    } else {
        // asm 000065D2: 	LDI	*+AR5(CARTRAK),AR6
        player_track_obj = OBJREF_TO_PTR(player_carblk->closest_track_piece);
        // asm 000065D3: 	LDI	*+AR6(OUSR1),R1
        player_track_id = (int)player_track_obj->usr1;
    }

    // asm 000065D4: 	LDI	@CAR_LIST,AR0
    obj = CAR_LIST;
    // asm 000065D5: 	CMPI	0,AR0
    // asm 000065D6: 	BZ	FPPX
    if (obj == NULL) {
        goto FPPX;
    }
FPPLP:
    // asm 000065D7: 	LDI	0,R5
    rank_increment = 0;
    // asm 000065D8: 	LDI	*+AR0(ODIST),R0
    // asm 000065D9: 	CMPI	30000,R0
    // asm 000065DA: 	BGT	NNEG
    // asm 000065DB: 	CMPI	-5000,R0
    // asm 000065DC: 	BLT	NNEG
    // asm 000065DD: 	INC	R6
    if (obj->dist <= 30000 && obj->dist >= -5000) {
        on_screen_cars += 1;
    }
NNEG:
    // asm 000065DE: 	LDI	*+AR0(OID),R0
    MAME_ASSERT_REG(0x000065DF, "R0", &obj->id);
    // asm 000065DF: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RACER,R0
    // asm 000065E0: 	BNE	NXTLP
    if (obj->id != (DRONE_C | VEHICLE_T | DRNE_RACER)) {
        goto NXTLP;
    }
    // The startup call has no player car block. Preserve its screen count without deriving
    // rank from the placeholder track ID used by the null-pointer bugfix above.
    if (player_carblk == NULL) {
        goto NXTLP1;
    }
    // asm 000065E1: 	LDI	*+AR0(OCARBLK),AR3
    carblk = obj->carblk;
    // asm 000065E2: 	LDI	*+AR3(CAR_OM),R0  	;OTHER MACHINES CAR?
    // asm 000065E3: 	BZ	FPP1			;NO...
    if (carblk->other_machine_controls == 0) {
        goto FPP1;
    }
    // asm 000065E4: 	LDI	*+AR3(CARTRACK_ID),R0	;CHECK IF AHEAD OR BEHIND
    // asm 000065E5: 	CMPI	R1,R0
    // asm 000065E6: 	BNE	NXTLP1
    if ((int)carblk->track_id != player_track_id) {
        rank_increment = (int)carblk->track_id > player_track_id;
        goto NXTLP1;
    }
    // ;	BEQ	FURTHER
    // ;	BLT	ISBLAMP
    // ;
    // ;	CMPI	@PLY2CAR,AR0
    // ;	BNE	NXTLP1
    // ;
    // ;	LDI	0,R0
    // ;	STI	R0,@THIS_MACHINE_AHEAD
    // ;	BU	NXTLP1
    // ;
    // ;ISBLAMP	LDI	1,R0
    // ;	STI	R0,@THIS_MACHINE_AHEAD
    // ;	BU	NXTLP1
    // ;
    // ;FURTHER
    // asm 000065E7: 	CMPI	@PLY2CAR,AR0
    // asm 000065E8: 	LDIEQ	1,IR0
    // ;	LDI	1,IR0
    // asm 000065E9: 	STI	AR6,*+AR3(CARTRAK)  	;CALC CARDIST2CNTR DUDE
    carblk->closest_track_piece = OBJ_TO_REF(player_track_obj);
    // asm 000065EA: 	LDI	AR6,AR2
    // asm 000065EB: 	PUSH	AR4
    // asm 000065EC: 	PUSH	AR5
    // asm 000065ED: 	LDI	AR0,AR4
    // asm 000065EE: 	LDI	AR3,AR5
    // asm 000065EF: 	CALL	DRONE_RIDE_RIGHT
    // asm 000065F0: 	STF	R0,*+AR5(CARDIST2CNTR)	;STORE DIST TO CENTER
    carblk->dist_to_center = DRONE_RIDE_RIGHT(obj, carblk);
    // asm 000065F1: 	POP	AR5
    // asm 000065F2: 	POP	AR4
    // asm 000065F3: 	B	FPP2
    track_obj = player_track_obj;
    goto FPP2;
FPP1:
    // asm 000065F4: 	LDI	*+AR0(OPLINK),AR2
    proc = obj->plink;
    // asm 000065F5: 	LDI	*+AR2(STEALTHMODE),R0  	;0=ONSCRN,-1=BEHIND ST, 1=AHEAD STEALTH
    // asm 000065F6: 	BNE	NXTLP1
    if (proc == NULL || proc->ctx->RACER_DRONE.stealthmode != 0) {
        if (proc != NULL) {
            rank_increment = proc->ctx->RACER_DRONE.stealthmode > 0;
        }
        goto NXTLP1;
    }
    // asm 000065F7: 	LDI	*+AR0(OCARBLK),AR3
    // asm 000065F8: 	LDI	*+AR3(CARTRAK),AR2
    track_obj = OBJREF_TO_PTR(carblk->closest_track_piece);
    // asm 000065F9: 	LDI	*+AR2(OUSR1),R0
    // asm 000065FA: 	CMPI	R1,R0
    // asm 000065FB: 	BNE	NXTLP1
    if (track_obj == NULL || (int)track_obj->usr1 != player_track_id) {
        if (track_obj != NULL) {
            rank_increment = (int)track_obj->usr1 > player_track_id;
        }
        goto NXTLP1;
    }
    // *
    // *ON SAME TRACK SEGMENT
    // *
FPP2:
    // asm 000065FC: 	LDI	*+AR2(OLINK4),AR2		;get the next road piece
    next_track_obj = (OBJ*)(uintptr_t)track_obj->link4;
    // asm 000065FD: 	SUBF	*+AR0(OPOSX),*+AR2(OPOSX),R0
    // asm 000065FE: 	MPYF	R0,R0
    // asm 000065FF: 	LDF	*+AR0(OPOSZ),R3
    // asm 00006600: 	SUBF	*+AR2(OPOSZ),R3
    // asm 00006601: 	MPYF	R3,R3
    // asm 00006602: 	ADDF	R0,R3,R4
    other_dist_sq = (next_track_obj->pos.X - obj->pos.X) * (next_track_obj->pos.X - obj->pos.X);
    other_dist_sq += (obj->pos.Z - next_track_obj->pos.Z) * (obj->pos.Z - next_track_obj->pos.Z);
    // asm 00006603: 	LDF	*+AR3(CARDIST2CNTR),R0		;CORRECT FOR NOT CENTERED
    // asm 00006604: 	MPYF	R0,R0
    // asm 00006605: 	SUBF	R0,R4
    other_dist_sq -= carblk->dist_to_center * carblk->dist_to_center;
    // asm 00006606: 	SUBF	*+AR4(OPOSX),*+AR2(OPOSX),R0
    // asm 00006607: 	MPYF	R0,R0
    // asm 00006608: 	LDF	*+AR4(OPOSZ),R3
    // asm 00006609: 	SUBF	*+AR2(OPOSZ),R3
    // asm 0000660A: 	MPYF	R3,R3
    // asm 0000660B: 	ADDF	R0,R3
    player_dist_sq = (next_track_obj->pos.X - player_obj->pos.X) * (next_track_obj->pos.X - player_obj->pos.X);
    player_dist_sq += (player_obj->pos.Z - next_track_obj->pos.Z) * (player_obj->pos.Z - next_track_obj->pos.Z);
    // asm 0000660C: 	LDF	*+AR5(CARDIST2CNTR),R0		;CORRECT FOR NOT CENTERED
    // asm 0000660D: 	MPYF	R0,R0
    // asm 0000660E: 	SUBF	R0,R3
    player_dist_sq -= player_carblk->dist_to_center * player_carblk->dist_to_center;
    // 	;CHECK FOR LAMP
    // ;	CMPI	0,IR0
    // ;	BEQ	KKDKA
    // ;	CLRI	IR0	;clear the flag
    // ;	CMPF	R4,R3
    // ;	LDIGT	0,R0
    // ;	LDILE	1,R0
    // ;	STI	R0,@THIS_MACHINE_AHEAD
    // asm 0000660F: KKDKA
    // 	;END CHECK LAMP
    // asm 0000660F: 	CMPF	R4,R3
    if (player_dist_sq > other_dist_sq) {
        rank_increment = 1;
    }
NXTLP1:
    // asm 00006610: 	LDIGT	1,R5
    MAME_ASSERT_REG(0x00006611, "R5", &rank_increment);
    // asm 00006611: 	ADDI	R5,R7
    position += rank_increment;
NXTLP:
    // asm 00006612: 	LDI	*+AR0(OLINK3),AR0
    obj = (OBJ*)(uintptr_t)obj->link3;
    // asm 00006613: 	CMPI	0,AR0
    // asm 00006614: 	BNE	FPPLP
    if (obj != NULL) {
        goto FPPLP;
    }
FPPX:
    // asm 00006615: 	STI	R7,@POSITION
    POSITION = position;
    MAME_ASSERT_REG(0x00006616, "R7", &POSITION);
    // asm 00006616: 	STI	R6,@ONCSCREEN_CARS
    ONCSCREEN_CARS = on_screen_cars;
    MAME_ASSERT_REG(0x00006617, "R6", &ONCSCREEN_CARS);
    // asm 00006617: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DRONE DISPATCHER
 *
 *
 */
/* asm: DD_SLP	.bss	DD_SLP,1 */
int DD_SLP;
/* asm: DD_VAR	.bss	DD_VAR,1 */
int DD_VAR;
/* asm: DD_MAX_DRONES	.bss	DD_MAX_DRONES,1 */
int DD_MAX_DRONES;

// *----------------------------------------------------------------------------
void SIGMA_DISPATCHER(PROC* p) {
    PROC* sigma_proc;
    OBJ* sigma_obj;
    PROC_CONTEXT* sigma_ctx;
    TYCOHEADER dgroup_header;
    int sleep_ticks;
    int position;
    int random_percent;
    float sigma_distance;

    switch (p->resume_state) {
    case 0:
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    case 3:
        goto PROC_RESUME_3;
    }

    // asm 00006618: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00006619: 	BZ	CONTIN
    // asm 0000661A: 	DIE
    if (HEAD2HEAD_ON != 0) {
        DIE();
    }
CONTIN:
    // asm 0000661B: 	SLEEP	30*20
    SLEEP(30 * 20, 1);
SIGDSP_LP:
    // asm 0000661D: 	LDI	@DD_VAR,AR2
    // asm 0000661E: 	CALL	RANDU0
    sleep_ticks = RANDU0(DD_VAR);
    // asm 0000661F: 	ADDI	@DD_SLP,R0
    sleep_ticks += DD_SLP;
    // asm 00006620: 	MPYI	2,R0
    sleep_ticks *= 2;
    // asm 00006621: 	LDI	R0,AR2
SG_DISP_S:
    // asm 00006622: 	CALL	SLEEP
    SLEEP(sleep_ticks, 2);
    // asm 00006623: 	LDI	1,AR2
    // asm 00006624: 	LDI	@DRONE_DISPATCH_P,R0
    // asm 00006625: 	BZ	SG_DISP_S
    if (DRONE_DISPATCH_P == 0) {
        sleep_ticks = 1;
        goto SG_DISP_S;
    }
    // asm 00006626: 	LDI	@SUSPEND_MODE,R0
    // asm 00006627: 	CMPI	SM_HALT,R0
    // asm 00006628: 	BEQ	SIGDSP_LP	;->STRAIGHT TO SLEEP
    if (SUSPEND_MODE == SM_HALT) {
        goto SIGDSP_LP;
    }
    // asm 00006629: 	LDI	@DRONE_COUNT,R0
    // asm 0000662A: 	SUBI	1,R0
    // asm 0000662B: 	CMPI	@DD_MAX_DRONES,R0
    // asm 0000662C: 	BGT	SIGDSP_LP
    if ((DRONE_COUNT - 1) > DD_MAX_DRONES) {
        goto SIGDSP_LP;
    }
    // asm 0000662D: 	LDI	@POSITION,R1
    // asm 0000662E: 	CMPI	1,R1
    // asm 0000662F: 	BLE	DOIT
    position = POSITION;
    if (position <= 1) {
        goto DOIT;
    }
    // asm 00006630: 	RANDN	100		;R0 <- rand %%
    random_percent = RANDU0(100);
    // asm 00006632: 	CMPI	4,R1
    // asm 00006633: 	BGT	OV1
    if (position <= 4) {
        // asm 00006634: 	CMPI	70,R0
        // asm 00006635: 	BLT	DOIT
        // asm 00006636: 	BGT	SIGDSP_LP
        if (random_percent < 70) {
            goto DOIT;
        }
        goto SIGDSP_LP;
    }
OV1:
    // asm 00006637: CMPI	7,R1
    // asm 00006638: 	BGT	OV2
    if (position <= 7) {
        // asm 00006639: 	CMPI	60,R0
        // asm 0000663A: 	BLT	DOIT
        // asm 0000663B: 	BGT	SIGDSP_LP
        if (random_percent < 60) {
            goto DOIT;
        }
        goto SIGDSP_LP;
    }
OV2:
    // asm 0000663C: CMPI	35,R0
    // asm 0000663D: 	BGT	SIGDSP_LP
    if (random_percent > 35) {
        goto SIGDSP_LP;
    }
DOIT:
    // ;	CALL	COP_ACTIVE	;NO SIGMAS MIXED WITH COPS
    // ;	BC	SIGDSP_LP
    // asm 0000663E: 	CREATE	SIGMA_DRONE,DRONE_C|VEHICLE_T|DRNE_SIGMA
    sigma_ctx = port_malloc(sizeof(PROC_CONTEXT));
    sigma_proc = CREATE(SIGMA_DRONE, DRONE_C | VEHICLE_T | DRNE_SIGMA, sigma_ctx);
    // asm 00006641: 	BC	SIGDSP_LP
    if (sigma_proc == NULL) {
        goto SIGDSP_LP;
    }
    // asm 00006642: 	LDI	AR0,AR5
    p->ctx->SIGMA_DISPATCHER.sigma_proc = sigma_proc;
NOTYET:
    // asm 00006643: SLEEP	1
    SLEEP(1, 3);
    // asm 00006645: 	LDI	*+AR5(PAR4),AR4
    sigma_proc = p->ctx->SIGMA_DISPATCHER.sigma_proc;
    if (sigma_proc == NULL || sigma_proc->ctx == NULL) {
        goto SIGDSP_LP;
    }
    sigma_obj = sigma_proc->ctx->RACER_DRONE.obj;
    if (sigma_obj == NULL) {
        goto NOTYET;
    }
    // asm 00006646: 	LDI	*+AR4(OID),R0
    // asm 00006647: 	CMPI	DRONE_C|VEHICLE_T|DRNE_SIGMA,R0
    // asm 00006648: 	BNE	SIGDSP_LP
    if (sigma_obj->id != (DRONE_C | VEHICLE_T | DRNE_SIGMA)) {
        goto SIGDSP_LP;
    }
    // asm 00006649: 	LDI	@DGROUP_AW,AR0
    // asm 0000664A: 	LDI	AR0,AR2
    // asm 0000664B: 	ADDI	1,AR2
    ROM_ReadTYCOHEADER(DGROUP_AW, &dgroup_header);
    // asm 0000664C: 	LDI	AR4,R2
    // asm 0000664D: 	ADDI	OPOSX,R2
    // asm 0000664E: 	CALL	GET_XZ_DISTANCE
    sigma_distance = GET_XZ_DISTANCE(&dgroup_header.pos, &sigma_obj->pos);
    // ;	FLOAT	15000,R1
    // asm 0000664F: 	FLOAT	25000,R1
    // asm 00006650: 	CMPF	R1,R0
    // asm 00006651: 	BLT	NOTYET
    if (sigma_distance < 25000.0f) {
        goto NOTYET;
    }
    // asm 00006652: 	BU	SIGDSP_LP
    REENTER(SIGMA_DISPATCHER);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ADD ID TO DRONE POINTER TABLE
 *
 *PARAMETERS
 *	AR4	OBJECT
 *	AR5	CAR BLOCK
 *RETURNS
 *	R0	ID
 *
 */

/* asm: DRONENUM	.bss	DRONENUM,1 */
int DRONENUM;

void DRONE_PTR_ADD(void) {
    // asm 00006653: 	LDI	0,R0
    // asm 00006654: 	STI	R0,*+AR5(CAR_OM)
    // asm 00006655: 	LDI	@DRONENUM,R0		;INCREMENT ID #
    // asm 00006656: 	ADDI	1,R0
    // asm 00006657: 	CMPI	127,R0
    // asm 00006658: 	LDIGT	10,R0			;RESET TO 10
    // asm 00006659: 	STI	R0,@DRONENUM
    // asm 0000665A: 	LDI	@DIPRAM,R1
    // asm 0000665B: 	TSTB	CMDP_MASTER,R1		;CHECK MASTER
    // asm 0000665C: 	LDIZ	80H,R1			;YES, SET BIT 7
    // asm 0000665D: 	LDINZ	0,R1
    // asm 0000665E: 	OR	R1,R0
    // asm 0000665F: 	STI	R0,*+AR5(CARNUM)
    // asm 00006660: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_PTR_ADD", 0, 0);
    UNIMPL();
}

void DRONE_CLR(void) {
    // asm 00006661: 	LDI	10,R0
    // asm 00006662: 	STI	R0,@DRONENUM
    // asm 00006663: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_CLR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *MODIFICATION, THE PLYR MUST MOVE AT LEAST 9000 Voxels FROM THE
 *INITIAL STARTING POSITION FOR US TO RELEASE ANY RHO DRONES, BECAUSE
 *THEY DISAPPEAR WHEN THEY BEGIN TRACKING NO WORLD, WHICH AT THE STARTUP
 *OF EACH LEG IS REALLY CUTTING IT
 *
 *
 */
void RHO_DISPATCHER(PROC* p) {
    // asm 00006664: 	CALL	DRONE_CLR 	;CLEAR OUT DRONE POINTER TABLE
    // asm 00006665: 	SLEEP	30		;WAIT A SECOND
    // asm 00006667: 	LDI	@PLYCAR,AR4
    // asm 00006668: 	LDF	*+AR4(OPOSX),R6
    // asm 00006669: 	LDF	*+AR4(OPOSZ),R7	;GRAB THE 'INITIAL' LOCATION
    // asm 0000666A: 	SLEEP	30*20		;WAIT LIKE 20 SECONDS ANYWAY...
    // asm 0000666C: 	LDI	@_MODE,R0
    // asm 0000666D: 	AND	MMODE,R0
    // asm 0000666E: 	CMPI	MATTR,R0
    // asm 0000666F: 	BEQ	BEGIN_RELEASING
#if DEBUG
    // asm: 	CMPI	@PLYCAR,AR4
    // asm: 	BNE	$
#endif
RHOPSL:
    // asm 00006670: 	LDF	R6,R0
    // asm 00006671: 	LDF	R7,R2
    // asm 00006672: 	SUBF	*+AR4(OPOSX),R0
    // asm 00006673: 	SUBF	*+AR4(OPOSZ),R2
    // asm 00006674: 	MPYF	R0,R0
    // asm 00006675: 	MPYF	R2,R2
    // asm 00006676: 	ADDF	R0,R2
    // asm 00006677: 	CALL	SQRT
    // asm 00006678: 	FLOAT	11000,R1
    // asm 00006679: 	CMPF	R1,R0
    // asm 0000667A: 	BGT	BEGIN_RELEASING
    // asm 0000667B: 	SLEEP	1
    // asm 0000667D: 	BU	RHOPSL
BEGIN_RELEASING:
RHO_DLP:
    // asm 0000667E: 	LDI	@POSITION,R0
    // asm 0000667F: 	CMPI	2,R0
    // asm 00006680: 	BGT	NKLD
    // asm 00006681: 	LDI	50,AR2
    // asm 00006682: 	BU	TF_DISP_S
NKLD:
    // asm 00006683: 	LDI	@DD_VAR,AR2
    // asm 00006684: 	CALL	RANDU0
    // asm 00006685: 	ADDI	@DD_SLP,R0
    // asm 00006686: 	LDI	R0,AR2
TF_DISP_S:
    // asm 00006687: 	CALL	SLEEP
    // asm 00006688: 	LDI	1,AR2
    // asm 00006689: 	LDI	@DRONE_DISPATCH_P,R0
    // asm 0000668A: 	BZ	TF_DISP_S
    // asm 0000668B: 	LDI	@SUSPEND_MODE,R0
    // asm 0000668C: 	CMPI	SM_HALT,R0
    // asm 0000668D: 	BEQ	RHO_DLP
    // asm 0000668E: 	LDI	@DRONE_COUNT,R0
    // asm 0000668F: 	CMPI	@DD_MAX_DRONES,R0
    // asm 00006690: 	BGT	RHO_DLP
    // asm 00006691: 	LDI	@POSITION,R1
    // asm 00006692: 	CMPI	2,R1
    // asm 00006693: 	BLE	DOITR
    // asm 00006694: 	RANDN	100		;R0 <- rand %%
    // asm 00006696: 	CMPI	4,R1
    // asm 00006697: 	BGT	OV1R
    // asm 00006698: 	CMPI	75,R0
    // asm 00006699: 	BLT	DOITR
    // asm 0000669A: 	BGT	RHO_DLP
OV1R:
    // asm 0000669B: CMPI	7,R1
    // asm 0000669C: 	BGT	OV2R
    // asm 0000669D: 	CMPI	60,R0
    // asm 0000669E: 	BLT	DOITR
    // asm 0000669F: 	BGT	RHO_DLP
OV2R:
    // asm 000066A0: CMPI	35,R0
    // asm 000066A1: 	BGT	RHO_DLP
DOITR:
    // asm 000066A2: 	LDI	@ONCSCREEN_CARS,R0
    // asm 000066A3: 	CMPI	8,R0
    // asm 000066A4: 	BGT	RHO_DLP
    // asm 000066A5: 	CALL	CK_LINK_DISP
    // asm 000066A6: 	BZ	RHO_DLP			;NOT DISPATCHING, BEHIND IN LINK
    // asm 000066A7: 	CREATE	RHO_DRONE,DRONE_C|VEHICLE_T|DRNE_RHO
    // asm 000066AA: 	BU	RHO_DLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RHO_DISPATCHER", 0, 0);
    UNIMPL_TODO();
}

static void CK_LINK_DISP(void) {
    // asm 000066AB: 	LDI	@HEAD2HEAD_ON,R0
    // asm 000066AC: 	BZ	GODISP
    // asm 000066AD: 	CALL	COMPTRAK	;COMPARE TRACK RANKS OF PLAYERS
    // asm 000066AE: 	BLT	NOGO		;WERE BEHIND, NO DISPATCH
    // asm 000066AF: 	BGT	GODISP
    // asm 000066B0: 	NOT	@DIPRAM,R0	;CHECK MASTER
    // asm 000066B1: 	TSTB	CMDP_MASTER,R0
    // asm 000066B2: 	BNZ	GODISP		;ITS A MASTER...
NOGO:
    // asm 000066B3: 	LDI	0,R0
    // asm 000066B4: 	RETS
GODISP:
    // asm 000066B5: 	LDI	1,R0
    // asm 000066B6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CK_LINK_DISP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
void SET_DRONE_PAL(void) {
    // asm 000066B7: 	PUSH	R0
    // asm 000066B8: 	PUSH	AR2
    // asm 000066B9: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm 000066BA: 	MPYI	VEHTAB_SIZE,AR2
    // asm 000066BB: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 000066BC: 	LDI	*+AR2(VEHTAB_PALTAB),AR2
    // asm 000066BD: 	LDI	*AR2++,R0
    // asm 000066BE: 	BZ	NO_EPALS
    // asm 000066BF: 	PUSH	AR2
    // asm 000066C0: 	INC	R0
    // asm 000066C1: 	LDI	R0,AR2
    // asm 000066C2: 	CALL	RANDU0
    // asm 000066C3: 	POP	AR2
    // asm 000066C4: 	CMPI	0,R0
    // asm 000066C5: 	BEQ	NO_EPALS
    // asm 000066C6: 	DEC	R0
    // asm 000066C7: 	ADDI	R0,AR2
    // asm 000066C8: 	LDI	*AR2,AR2
    // asm 000066C9: 	CALL	PAL_FIND_RAW
    // asm 000066CA: 	BNC	NO_EPALS
    // asm 000066CB: 	STI	R0,*+AR4(OPAL)
    // asm 000066CC: 	LDI	*+AR4(OFLAGS),R0
    // asm 000066CD: 	OR	O_1PAL,R0
    // asm 000066CE: 	STI	R0,*+AR4(OFLAGS)
NO_EPALS:
    // asm 000066CF: 	POP	AR2
    // asm 000066D0: 	POP	R0
    // asm 000066D1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_DRONE_PAL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PARAMETERS
 *	AR2	OBJECT TO CHECK VALID LANES FOR...
 *RETURNS
 *	R0	0 - 2 lanes
 *		1 - 4 lanes
 *
 */
int GET_LANES(OBJ* obj /*AR2*/) {
    int section_index;
    int lane_mode;
    int i;

    // asm 000066D2: 	PUSH	AR0
    // asm 000066D3: 	PUSH	AR1
    // asm 000066D4: 	LDI	*+AR2(OUSR1),R0
    section_index = (int)obj->usr1;
    // asm 000066D5: 	RS	8,R0
    section_index >>= 8;
    MAME_ASSERT_REG(0x000066D6, "R0", &section_index);
    // asm 000066D6: 	LDI	@DGROUP_COUNT,AR0
    // asm 000066D7: 	INC	AR0
    // asm 000066D8: 	LDI	@DGROUPSI,AR1
GL_LP:
    // asm 000066D9: CMPI	*+AR1(DGRP_IDX),R0
    for (i = 0; i <= DGROUP_COUNT + 1; i++) {
        if (section_index == DGROUPSI[i].idx) {
            goto GL_FND;
        }
    // asm 000066DA: 	BEQ	GL_FND
    // asm 000066DB: 	ADDI	DGRP_SIZE,AR1
    // asm 000066DC: 	DBU	AR0,GL_LP
    }
    // asm 000066DD: 	CLRI	R0
    lane_mode = 0;
    MAME_ASSERT_REG(0x000066DE, "R0", &lane_mode);
    // asm 000066DE: 	POP	AR1
    // asm 000066DF: 	POP	AR0
    // asm 000066E0: 	RETS
    return lane_mode;
GL_FND:
    // asm 000066E1: 	LDI	*+AR1(DGRP_FLAG),R0
    lane_mode = DGROUPSI[i].flag;
    // asm 000066E2: 	RS	7,R0
    lane_mode >>= 7;
    // asm 000066E3: 	AND	1,R0
    lane_mode &= 1;
    MAME_ASSERT_REG(0x000066E4, "R0", &lane_mode);
    // asm 000066E4: 	POP	AR1
    // asm 000066E5: 	POP	AR0
    // asm 000066E6: 	RETS
    return lane_mode;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GET X/Z DISTANCE TO PLAYER
 *
 *PARAMETERS
 *	AR4	OBJECT TO CHECK
 *RETURNS
 *	R0	FL DISTANCE TO PLAYER (IN VOXELS)
 *
 */
void DIST_TO_PLYR(void) {
    // asm 000066E7: 	PUSH	AR3
    // asm 000066E8: 	PUSHFL	R1
    // asm 000066EA: 	PUSHFL	R2
    // asm 000066EC: 	LDPI	@PLYCAR,AR3
    // asm 000066ED: 	LDF	*+AR4(OPOSX),R2
    // asm 000066EE: 	SUBF	*+AR3(OPOSX),R2
    // asm 000066EF: 	LDF	*+AR4(OPOSZ),R1
    // asm 000066F0: 	SUBF	*+AR3(OPOSZ),R1
    // asm 000066F1: 	MPYF	R1,R1
    // asm 000066F2: 	MPYF	R2,R2
    // asm 000066F3: 	ADDF	R1,R2
    // asm 000066F4: 	CALL	SQRT
#if DEBUG
    // asm: 	CMPI	0,DP
    // asm: 	BNE	$
#endif
    // asm 000066F5: 	POPFL	R2
    // asm 000066F7: 	POPFL	R1
    // asm 000066F9: 	POP	AR3
    // asm 000066FA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIST_TO_PLYR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GRAB THE AR2th PIECE ON THE CURRENT TRACK
 *
 *PARAMETERS
 *	AR2	NUMBER OF PIECES IN TO START
 *RETURNS
 *	AR2	OBJECT POINTER TO STARTING ROAD PIECE
 *
 */
void INIT_TRACKING_PIECE(void) {
    // asm 000066FB: 	PUSH	AR0
    // asm 000066FC: 	PUSH	AR1
    // asm 000066FD: 	LDI	@DYNALIST_BEGIN,AR0
LPP:
    // asm 000066FE: LDI	*+AR0(OLINK4),AR1
    // asm 000066FF: 	LDI	AR1,AR0
    // asm 00006700: 	DBU	AR2,LPP
    // asm 00006701: 	LDI	AR0,AR2
#if DEBUG
    // asm: 	CMPI	0,AR2
    // asm: 	SLOCKON	EQ,"DRONES\INIT_TRACKING_PIECE .."
#endif
    // asm 00006702: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm 00006703: 	POP	AR1
    // asm 00006704: 	POP	AR0
    // asm 00006705: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_TRACKING_PIECE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *TRACK APPROPRIATE PIECE
 *
 *GETS THE POSITION THAT WE ARE CURRENTLY TRACKING ON,
 *IF THE POSITION IS TOO CLOSE THEN WE ADVANCE TO THE NEXT POSITION (W/WRAP
 *AROUND)
 *
 *PARAMETERS
 *	AR4			DRONE OBJECT
 *	AR7			DRONE PROCESS
 *	*+AR7(DELTA_TPIECE)	VALID ROAD OBJECT CURRENTLY TRACKING
 *
 *RETURNS
 *	VECTORA		[X Y Z] POSITION
 *	AR5		PTR TO TRACK ENTRY
 *	R0		(FL) DISTANCE TO NEXT PIECE
 *	MATRIXA		find Y Matrix (of Tracking Piece)
 *
 */
void GET_TRACK_POS_RVS_XLANE(void) {
    // asm 00006706: 	PUSHFL	R1
    // asm 00006708: 	PUSHFL	R2
    // asm 0000670A: 	PUSH	AR2
    // asm 0000670B: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000670C: 	CALL	SUB_FUNCTION_RVS_XLANE		;GET LANE OFFSET (VECTOR A)
    // asm 0000670D: 	BU	TRKP2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TRACK_POS_RVS_XLANE", 0, 0);
    UNIMPL();
}

void GET_TRACK_POS_RVS(void) {
    // asm 0000670E: 	PUSHFL	R1
    // asm 00006710: 	PUSHFL	R2
    // asm 00006712: 	PUSH	AR2
    // asm 00006713: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00006714: 	CALL	SUB_FUNCTION_RVS
    // asm 00006715: 	BU	TRKP2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TRACK_POS_RVS", 0, 0);
    UNIMPL();
}

void DELTA_GET_TRACK_POS(void) {
    // asm 00006716: 	PUSHFL	R1
    // asm 00006718: 	PUSHFL	R2
    // asm 0000671A: 	PUSH	AR2
    // asm 0000671B: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 0000671C: 	CALL	DELTA_SUB_FUNCTION		;GET LANE OFFSET (VECTOR A)
    // asm 0000671D: 	BU	TRKP2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELTA_GET_TRACK_POS", 0, 0);
    UNIMPL();
}

void GET_TRACK_POS(void) {
    // asm 0000671E: 	PUSHFL	R1
    // asm 00006720: 	PUSHFL	R2
    // asm 00006722: 	PUSH	AR2
    // asm 00006723: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm 00006724: TRACK_PIECE
    // asm 00006724: 	CALL	SUB_FUNCTION		;GET LANE OFFSET (VECTOR A)
TRKP2:
#if DEBUG
    // asm: 	LDI	*+AR2(OUSR1),R1
    // asm: 	SLOCKON	LT,"DRONES\GET_TRACK_POS  probably tracking a deleted object"
#endif
    // asm 00006725: 	LDF	*+AR2(OPOSX),R2		;X
    // asm 00006726: 	SUBF	*+AR4(OPOSX),R2
    // asm 00006727: 	LDF	*+AR2(OPOSZ),R1		;Z
    // asm 00006728: 	SUBF	*+AR4(OPOSZ),R1
    // asm 00006729: 	MPYF	R2,R2
    // asm 0000672A: 	MPYF	R1,R1
    // asm 0000672B: 	ADDF	R1,R2
    // asm 0000672C: 	CALL	SQRT
    // 	;R0 now has distance to next check
    // asm 0000672D: DISTANCE_OK
    // asm 0000672D: 	POP	AR2
    // asm 0000672E: 	POPFL	R2
    // asm 00006730: 	POPFL	R1
    // asm 00006732: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TRACK_POS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
;*----------------------------------------------------------------------------
;*
;*PARAMETERS
;*	AR7			DRONE PROCESS
;*	AR4			DRONE OBJECT
;*	*+AR7(DELTA_SPTR)	VALID STEALTH POINTER
;*
;*
;*
;*RETURNS
;*	VECTORA		[X Y Z] POSITION
;*	R0		(FL) DISTANCE TO NEXT PIECE
;*	MATRIXA		find Y Matrix (of Tracking Piece)
;*
;GET_STEALTH_POS:
;	PUSHFL	R1
;	PUSHFL	R2
;	PUSHFL	R3
;	PUSH	AR2
;	LDI	*+AR7(DELTA_SPTR),AR2
;
;	FLOAT	*+AR2(Y),R0
;	FLOAT	52,R1
;	SUBF	R1,R0
;	STF	R0,*+AR4(OPOSY)
;
;;	yrot  = cr + ROAD_ITEMS[i].rads.y;
;;	yrot *= 128;
;;	yroti = (int) yrot;
;;
;;	ROAD_MAP[MAP_LENGTH].id = (ROAD_MAP[MAP_LENGTH].id & 0x0fffff) |
;;				((yroti & 0x0fff) << 20);
;
;
;	;to get the proper theta we must compute the direction to the next
;	;piece via arctan2
;	;
;	LDI	*+AR2(4+X),R2
;	LDI	*+AR2(4+Z),R3
;	SUBI	*+AR2(X),R2
;	SUBI	*+AR2(Z),R3
;	FLOAT	R2
;	FLOAT	R3
;	CALL	ARCTANF
;	SUBF	HALFPI,R0
;	LDF	R0,R2
;
;
;	LDI	@MATRIXAI,AR2
;	CALL	FIND_YMATRIX
;
;	LDI	*+AR7(DELTA_SPTR),AR2
;	FLOAT	*+AR2(X),R2
;	SUBF	*+AR4(OPOSX),R2
;	FLOAT	*+AR2(Z),R1
;	SUBF	*+AR4(OPOSZ),R1
;
;	MPYF	R2,R2
;	MPYF	R1,R1
;	ADDF	R1,R2
;	CALL	SQRT
;
;	POP	AR2
;	POPFL	R3
;	POPFL	R2
;	POPFL	R1
;	RETS
;*----------------------------------------------------------------------------
*/

/*
 *----------------------------------------------------------------------------
 *GETS OFFSET OF LANE
 *
 *
 *PARAMETERS
 *	AR2	PIECE TO TRACK (ALLOCATED OBJECT)
 *	AR4	DRONE OBJECT
 *	AR7	DRONE PROCESS
 *
 *
 *RETURNS
 *	MATRIXA	ROTATED FOR THE LANE
 *	VECTORA	LANE OFFSET
 *	R2	RADIAN Y
 *
 *
 *	if (obj -> OLINK4 == NULL)  {
 *		dont know???
 *	}
 *	else  {
 *
 *
 *	}
 *
 */
void SUB_FUNCTION_RVS(void) {
    // asm 00006733: 	PUSH	AR0
    // asm 00006734: 	PUSHFL	R0
    // asm 00006736: 	PUSHFL	R3
    // asm 00006738: 	LDI	*+AR2(OBLINK4),R0
    // asm: 	SLOCKON	Z,"DRONES\SUB_FUNCTION_RVS  OBLINK4 to NULL"
    // asm 00006739: 	BU	SF_ENTER2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SUB_FUNCTION_RVS", 0, 0);
    UNIMPL();
}

void SUB_FUNCTION(void) {
    // asm 0000673A: 	PUSH	AR0
    // asm 0000673B: 	PUSHFL	R0
    // asm 0000673D: 	PUSHFL	R3
    // asm 0000673F: 	LDI	*+AR2(OLINK4),R0
    // asm: 	SLOCKON	Z,"DRONES\SUB_FUNCTION  OLINK4 to NULL"
SF_ENTER2:
    // asm 00006740: 	LDI	R0,AR0
    // asm 00006741: 	LDF	*+AR0(OPOSX),R2
    // asm 00006742: 	SUBF	*+AR2(OPOSX),R2
    // asm 00006743: 	LDF	*+AR0(OPOSZ),R3
    // asm 00006744: 	SUBF	*+AR2(OPOSZ),R3
    // asm 00006745: 	CALL	ARCTANF
    // asm 00006746: 	SUBF	HALFPI,R0
    // asm 00006747: 	LDF	R0,R2				;FIND THETA
    // asm 00006748: 	PUSHF	R2
    // asm 00006749: 	CALL	GET_LANES
    // asm 0000674A: 	PUSH	AR2
    // asm 0000674B: 	LDI	@MATRIXAI,AR2
    // asm 0000674C: 	CALL	FIND_YMATRIX			;FIND Y MATRIX (FOR LANE OFFSETTING)
    // asm 0000674D: 	LDI	*+AR7(DELTA_STATUS),AR0
    // asm 0000674E: 	AND	DELTA_STATUS_LANE,AR0
    // ;	LDPI	@LANEPI,AR1
    // asm 0000674F: 	LDI	@LANEPI,AR1
    // asm 00006750: 	ADDI	R0,AR1		;4 or 2 lane map?
    // asm 00006751: 	LDI	*AR1,R0
    // asm 00006752: 	ADDI	R0,AR0		;which lane?
    // asm 00006753: 	LDF	*AR0,R0
DELTA_JOININ:
    // asm 00006754: 	LDI	@VECTORAI,AR2
    // asm 00006755: 	STF	R0,*+AR2(X)
    // asm 00006756: 	CLRF	R0
    // asm 00006757: 	STF	R0,*+AR2(Y)
    // asm 00006758: 	STF	R0,*+AR2(Z)
    // asm 00006759: 	LDI	AR2,R3
    // asm 0000675A: 	LDI	@MATRIXAI,R2
    // asm 0000675B: 	CALL	MATRIX_MUL			;COMPUTE THE LANE OFFSET IN VECTORA
    // asm 0000675C: 	POP	AR2
    // asm 0000675D: 	POPF	R2
    // asm 0000675E: 	POPFL	R3
    // asm 00006760: 	POPFL	R0
    // asm 00006762: 	POP	AR0
    // asm 00006763: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SUB_FUNCTION", 0, 0);
    UNIMPL();
}

void SUB_FUNCTION_RVS_XLANE(void) {
    // asm 00006764: 	PUSH	AR0
    // asm 00006765: 	PUSHFL	R0
    // asm 00006767: 	PUSHFL	R3
    // asm 00006769: 	LDI	*+AR2(OBLINK4),R0
    // asm 0000676A: 	BU	SFENTER66
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SUB_FUNCTION_RVS_XLANE", 0, 0);
    UNIMPL();
}

void DELTA_SUB_FUNCTION(void) {
    // asm 0000676B: 	PUSH	AR0
    // asm 0000676C: 	PUSHFL	R0
    // asm 0000676E: 	PUSHFL	R3
    // asm 00006770: 	LDI	*+AR2(OLINK4),R0
    // asm: 	SLOCKON	Z,"DRONES\DELTA_SUB_FUNCTION  OLINK4 to NULL"
SFENTER66:
    // asm 00006771: 	LDI	R0,AR0
    // asm 00006772: 	LDF	*+AR0(OPOSX),R2
    // asm 00006773: 	SUBF	*+AR2(OPOSX),R2
    // asm 00006774: 	LDF	*+AR0(OPOSZ),R3
    // asm 00006775: 	SUBF	*+AR2(OPOSZ),R3
    // asm 00006776: 	CALL	ARCTANF
    // asm 00006777: 	SUBF	HALFPI,R0
    // asm 00006778: 	LDF	R0,R2				;FIND THETA
    // asm 00006779: 	PUSHF	R2
    // asm 0000677A: 	PUSH	AR2
    // asm 0000677B: 	LDI	@MATRIXAI,AR2
    // asm 0000677C: 	CALL	FIND_YMATRIX			;FIND Y MATRIX (FOR LANE OFFSETTING)
    // asm 0000677D: 	LDF	*+AR7(DELTA_XLANE),R0
    // asm 0000677E: 	BU	DELTA_JOININ
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELTA_SUB_FUNCTION", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
;*----------------------------------------------------------------------------
;*GIVEN A SECTION/ROAD ID FIND THE MAP ENTRY IN THE
;*ROAD MAP WHICH CONTAINS THE ID EQUAL TO, OR GREATER
;*THAN THE GIVEN ID.
;*[finds GE road ID ptr]
;*
;*PARAMETERS
;*	R2	CODED ID (from OUSR1)
;*	AR7	DRONE PROCESS
;*
;*RETURNS
;*	*+AR7(STEALTH_PTR)
;*
;FIND_MAPENTRY:
;	PUSH	AR0
;	PUSH	R0
;	LDI	@ROAD_MAPI,AR0
;FIND_LP
;	LDI	*+AR0(3),R0
;	LS	12,R0
;	RS	12,R0
;	CMPI	R2,R0
;	BGE	FINDX
;	ADDI	4,AR0
;	BU	FIND_LP
;FINDX
;	STI	AR0,*+AR7(DELTA_SPTR)
;	STI	R0,*+AR7(DELTA_LAST_OID)	;save road id
;
;	POP	R0
;	POP	AR0
;	RETS
;*----------------------------------------------------------------------------
*/

/*
 *----------------------------------------------------------------------------
 *DRONE UTILITY FUNCTIONS
 *
 */
void INIT_DRONES(void) {
    CAR_LIST = NULL;
    DRONE_COUNT = 0;

    SUSPEND_MODE = SM_GO;

    DD_SLP = 40;
    DD_VAR = 100;
    DD_MAX_DRONES = MAX_DRONES;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ADD TO DRONE SUPP LIST
 *
 *PARAMETERS
 *	AR4	OBJECT DRONE POINTER
 *
 */
void ADD_DRONE(OBJ* obj /*AR4*/) {
    // asm 0000678D: 	PUSH	R0
    // asm 0000678E: 	LDI	@CAR_LIST,R0
    // asm 0000678F: 	STI	R0,*+AR4(OLINK3)
    obj->link3 = (uintptr_t)CAR_LIST;
    // asm 00006790: 	STI	AR4,@CAR_LIST
    CAR_LIST = obj;
    // asm 00006791: 	INCM	@DRONE_COUNT
    DRONE_COUNT += 1;
    // asm 00006794: 	POP	R0
    // asm 00006795: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *FREE_DRONE
 *unlink DRONE from DRONE supplimentary list
 *
 *PARAMETERS
 *	AR4	OBJECT DRONE POINTER
 *
 */
void FREE_DRONE(void) {
    // asm 00006796: 	PUSH	R0
    // asm 00006797: 	PUSH	AR1
    // asm 00006798: 	PUSH	AR3
    // asm 00006799: 	LDI	@CAR_LISTI,R0
    // asm 0000679A: 	SUBI	OLINK3,R0		;(we are offset pointing)
FREELP:
    // asm 0000679B: LDI	R0,AR1
    // asm 0000679C: 	LDI	*+AR1(OLINK3),R0
    // asm: 	SLOCKON	Z,"DRONES\FREE_DRONE  end of list found"
    // asm 0000679D: 	BZ	FREEDR_X
    // asm 0000679E: 	CMPI	R0,AR4
    // asm 0000679F: 	BNE	FREELP
    // asm 000067A0: 	LDI	*+AR4(OLINK3),R0
    // asm 000067A1: 	STI	R0,*+AR1(OLINK3)	;LINK AROUND
    // asm 000067A2: 	DECM	@DRONE_COUNT
#if DEBUG
    // asm: 	CMPI	0,R0
    // asm: 	SLOCKON	LT,"DRONES\FREE_DRONE  DRONE_COUNT INVALID"
#endif
FREEDR_X:
    // asm 000067A5: 	POP	AR3
    // asm 000067A6: 	POP	AR1
    // asm 000067A7: 	POP	R0
    // asm 000067A8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREE_DRONE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 */
void EXP_PUFF(void) {
    // asm 000067A9: 	LDL	40000,R0
    // asm 000067AA: 	CMPI	*+AR4(ODIST),R0
    // asm 000067AB: 	BLT	SUICIDE
    // asm 000067AC: 	LDI	AR4,AR6
    // asm 000067AD: 	CALL	OBJ_GET
    // asm 000067AE: 	BC	SUICIDE
    // asm 000067AF: 	LDI	AR0,AR4
    // asm 000067B0: 	LDI	@EXP_ANII,AR5
    // asm 000067B1: 	LDI	*AR5,AR0
    // asm 000067B2: 	STI	AR0,*+AR4(OROMDATA)
    // asm 000067B3: 	LDF	*+AR6(OPOSX),R0
    // asm 000067B4: 	STF	R0,*+AR4(OPOSX)
    // asm 000067B5: 	LDF	*+AR6(OPOSY),R0
    // asm 000067B6: 	STF	R0,*+AR4(OPOSY)
    // asm 000067B7: 	LDF	*+AR6(OPOSZ),R0
    // asm 000067B8: 	STF	R0,*+AR4(OPOSZ)
    // asm 000067B9: 	ORM	O_POSTER,*+AR4(OFLAGS)
    // asm 000067BC: 	LDI	AR4,AR2
    // asm 000067BD: 	CALL	OBJ_INSERT
    // asm 000067BE: 	LDF	6,R0
    // asm 000067BF: 	CALL	SFRAND
    // asm 000067C0: 	LDF	R0,R6
    // asm 000067C1: 	LDF	6,R0
    // asm 000067C2: 	CALL	SFRAND
    // asm 000067C3: 	LDF	R0,R7
EXP_PUFFLP:
    // asm 000067C4: 	LDL	40000,R0
    // asm 000067C5: 	CMPI	*+AR4(ODIST),R0
    // asm 000067C6: 	BLT	EXP_DIE
    // asm 000067C7: 	LDF	*+AR4(OPOSX),R0
    // asm 000067C8: 	ADDF	R6,R0
    // asm 000067C9: 	STF	R0,*+AR4(OPOSX)
    // asm 000067CA: 	LDF	*+AR4(OPOSZ),R0
    // asm 000067CB: 	ADDF	R7,R0
    // asm 000067CC: 	STF	R0,*+AR4(OPOSZ)
    // asm 000067CD: 	LDI	*AR5++,AR0
    // asm 000067CE: 	CMPI	-1,AR0
    // asm 000067CF: 	BEQ	EXP_DIE
    // asm 000067D0: 	STI	AR0,*+AR4(OROMDATA)
    // asm 000067D1: 	LDF	*+AR4(OPOSY),R0
    // asm 000067D2: 	SUBF	10,R0
    // asm 000067D3: 	STF	R0,*+AR4(OPOSY)
    // asm 000067D4: 	SLEEP	1
    // asm 000067D6: 	BU	EXP_PUFFLP
EXP_DIE:
    // asm 000067D7: 	LDI	AR4,AR2
    // asm 000067D8: 	CALL	OBJ_DELETE
    // asm 000067D9: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EXP_PUFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: EXP_ANI */
/* asm: 	 */
/* asm: 	.word	dexplo1,dexplo2,dexplo3,dexplo4,dexplo5 */
/* asm: 	.word	dexplo6,dexplo7,dexplo8,dexplo9,dexplo10,dexplo11,-1 */
static int EXP_ANI[] = {
    // 	.word	blast1,blast2,blast3,blast4,blast5
    // 	.word	blast6,blast7,blast8,blast9,blast10,-1
    dexplo1_ROM,
    dexplo2_ROM,
    dexplo3_ROM,
    dexplo4_ROM,
    dexplo5_ROM,
    dexplo6_ROM,
    dexplo7_ROM,
    dexplo8_ROM,
    dexplo9_ROM,
    dexplo10_ROM,
    dexplo11_ROM,
    -1,
};
// *----------------------------------------------------------------------------

/*
;*----------------------------------------------------------------------------
;*MK_CHUNK
;*
;*PARAMETERS
;*	AR4	HEAD OBJECT
;*	AR2	ROM DATA PTR
;*
;*RETURNS
;*	C	CANNOT GET OBJECT
;*	AR0	CHUNK
;*
;*
;	.bss	XZSPC,1
;	.bss	XZMIN,1
;	.bss	YSPC,1
;	.bss	YMIN,1
;MK_CHUNK:
;	PUSH	AR2
;	PUSH	R0
;	PUSH	R1
;	PUSH	R2
;	PUSHF	R0
;	PUSHF	R1
;	PUSHF	R2
;
;
;	CALL	OBJ_GET
;	BC	MK_CHUNKX
;
;	MOVMF	*+AR4(OPOSX),*+AR0(OPOSX)
;	MOVMF	*+AR4(OPOSY),*+AR0(OPOSY)
;	MOVMF	*+AR4(OPOSZ),*+AR0(OPOSZ)
;	STI	AR2,*+AR0(OROMDATA)
;
;	LDI	@XZSPC,AR2
;	CALL	SRAND
;	FLOAT	R0
;	FLOATP	@XZMIN,R1
;	SADDFR	R1,R0
;	STF	R0,*+AR0(OVELX)
;
;	LDI	@XZSPC,AR2
;	CALL	SRAND
;	FLOAT	R0
;	FLOAT	@XZMIN,R1
;	SADDFR	R1,R0
;	STF	R0,*+AR0(OVELZ)
;
;
;	LDI	@YSPC,AR2
;	CALL	RANDU0
;	ADDI	@YMIN,R0
;	FLOAT	R0
;	NEGF	R0
;	STF	R0,*+AR0(OVELY)
;	LDI	AR0,AR2
;	CALL	OBJ_INSERT
;
;
;	LDI	10,AR2
;	CALL	SRAND
;	FLOAT	R0
;	MPYF	0.01,R0
;	STF	R0,*+AR0(OUSR2)
;
;	LDI	10,AR2
;	CALL	SRAND
;	FLOAT	R0
;	MPYF	0.03,R0
;	STF	R0,*+AR0(OUSR3)
;
;	CLRC
;MK_CHUNKX
;	POPF	R2
;	POPF	R1
;	POPF	R0
;	POP	R2
;	POP	R1
;	POP	R0
;	POP	AR2
;	RETS
;*----------------------------------------------------------------------------
*/

/*
 *----------------------------------------------------------------------------
 *PRECOLLIDE_PLYR	CHECK TO SEE IF CAR WILL COLLIDE WITH PLAYER
 *
 *
 *check distance to player
 *if within that distance
 *
 *examine path, is player in immediate player
 *
 *
 *PARAMETERS
 *	AR4	OBJECT
 *	AR5	CARBLOCK
 *	AR7	DRONE PROCESS
 *
 *RETURNS
 *	CARRY SET ON COLLISION IMMINENT
 *	CARRY CLR PATH MAINLY CLEAR
 *
 */
void PRECOLLIDE_PLYR(void) {
    // asm 000067DB: 	CALL	DIST_TO_PLYR
    // asm 000067DC: 	FLOAT	15000,R1
    // asm 000067DD: 	CMPF	R1,R0
    // asm 000067DE: 	BGT	NOT_IMMINENT
    // 	;
    // 	;project ahead 5 frames
    // 	;
    // asm 000067DF: 	LDI	@VECTORAI,AR3
    // asm 000067E0: 	LDF	*+AR5(CARSPEED),R0
    // asm 000067E1: 	MPYF	@NFRAMES,R0
    // asm 000067E2: 	MPYF	5,R0
    // asm 000067E3: 	STF	R0,*+AR3(Z)
    // asm 000067E4: 	CLRF	R0
    // asm 000067E5: 	STF	R0,*+AR3(X)
    // asm 000067E6: 	STF	R0,*+AR3(Y)
    // asm 000067E7: 	LDI	@MATRIXAI,AR2
    // asm 000067E8: 	LDF	*+AR5(CARVROT),R2
    // asm 000067E9: 	CALL	FIND_YMATRIX
    // asm 000067EA: 	LDI	AR2,R2
    // asm 000067EB: 	LDI	AR3,AR2
    // asm 000067EC: 	LDI	AR3,R3
    // asm 000067ED: 	CALL	MATRIX_MUL
    // 	;generate the line equation
    // 	;
    // asm 000067EE: 	LDP	@_VECTORA
    // asm 000067EF: 	NEGF	@_VECTORA+Z,R6		;A = Uy - Vy
    // asm 000067F0: 	LDF	@_VECTORA+X,R7		;B = Vx - Ux
    // asm 000067F1: 	SETDP
    // asm 000067F2: 	LDF	*+AR4(OPOSX),R5
    // asm 000067F3: 	MPYF	R6,R5
    // asm 000067F4: 	LDF	*+AR4(OPOSZ),R3
    // asm 000067F5: 	MPYF	R7,R3
    // asm 000067F6: 	ADDF	R3,R5
    // asm 000067F7: 	NEGF	R5			;C = -(A Ux + B Uy)
    // ;	LDPI	@CAR_LISTI,AR1
    // 	;now find the distance of a point to a line
    // 	;
    // asm 000067F8: 	MPYF	R6,R6,R2
    // asm 000067F9: 	MPYF	R7,R7,R3
    // asm 000067FA: 	ADDF	R3,R2
    // asm 000067FB: 	CALL	SQRT
    // asm 000067FC: 	LDF	R0,R1
    // asm 000067FD: 	LDPI	@PLYCAR,AR2
    // asm 000067FE: 	LDF	*+AR2(OPOSX),R2
    // asm 000067FF: 	MPYF	R6,R2
    // asm 00006800: 	LDF	*+AR2(OPOSZ),R3
    // asm 00006801: 	MPYF	R7,R3
    // asm 00006802: 	ADDF	R3,R2
    // asm 00006803: 	ADDF	R5,R2
    // asm 00006804: 	LDF	R2,R0
    // asm 00006805: 	CALL	DIV_F
    // asm 00006806: 	ABSF	R0
    // asm 00006807: 	FLOAT	400,R1
    // asm 00006808: 	CMPF	R1,R0
    // asm 00006809: 	BGT	NOT_IMMINENT
    // asm 0000680A: IS_IMMINENT
    // asm 0000680A: 	SETC
    // asm 0000680B: 	RETS
NOT_IMMINENT:
    // asm 0000680C: 	CLRC
    // asm 0000680D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRECOLLIDE_PLYR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PLYR_RIDE_RIGHT	is the player riding on the right side of the road?
 *
 *RETURNS
 *	CARRY CLR	ON RIGHT SIDE OF ROAD
 *	CARRY SET	NOT ON RIGHT SIDE OF ROAD
 *	R0		DISTANCE TO ROAD CENTER
 *
 *Algorythm
 *
 *	generate line equation by the closest track piece and the next piece
 *	check the sign of the distance to this line (plyrs position)
 *	and return which side of the road
 *
 *ANY ERRORS WILL ASSUME PLYR IS ON RITE SIDE
 *
 *DRONE_RIDE_RIGHT
 *
 *
 *PARAMETERS
 *	AR4	DRONE OBJ
 *	AR5	DRONE CAR BLOCK
 *RETURNS
 *	R0	(FL) DISTANCE TO CENTERLINE OF ROAD
 *
 */
float DRONE_RIDE_RIGHT(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/) {
    OBJ* track_obj;
    OBJ* next_track_obj;
    float a;
    float b;
    float c;
    float denominator;
    float dist;

    // asm 0000680E: 	PUSH	R1
    // asm 0000680F: 	PUSHFL	R2
    // asm 00006811: 	PUSH	R3
    // asm 00006812: 	PUSH	AR0
    // asm 00006813: 	PUSH	AR2
    // asm 00006814: 	PUSH	AR4
    // asm 00006815: 	PUSH	AR5
    // asm 00006816: 	BU	RIDE_RIGHT_JOININ
    track_obj = OBJREF_TO_PTR(carblk->closest_track_piece);
    if (track_obj == NULL) {
        return 0.0f;
    }

    // asm 00006823: 	LDI	*+AR5(CARTRAK),AR2
    // asm 00006824: 	LDI	*+AR2(OLINK4),R0
    next_track_obj = (OBJ*)track_obj->link4;
    if (next_track_obj == NULL) {
        return 0.0f;
    }

    // asm 00006827: 	LDF	*+AR2(OPOSZ),R0
    // asm 00006828: 	SUBF	*+AR0(OPOSZ),R0		;A = Uy - Vy
    a = track_obj->pos.Z - next_track_obj->pos.Z;
    // asm 00006829: 	LDF	*+AR0(OPOSX),R1
    // asm 0000682A: 	SUBF	*+AR2(OPOSX),R1		;B = Vx - Ux
    b = next_track_obj->pos.X - track_obj->pos.X;

    // asm 0000682B..0000682F
    c = -((a * track_obj->pos.X) + (b * track_obj->pos.Z));

    // asm 00006832..00006839
    denominator = sqrtf((a * a) + (b * b));
    dist = ((a * obj->pos.X) + (b * obj->pos.Z) + c) / denominator;

    return dist;
}

float PLYR_RIDE_RIGHT(void) {
    if (PLYCAR == NULL || PLYCAR->carblk == NULL) {
        return 0.0f;
    }

    // asm 00006817: 	PUSH	R1
    // asm 00006818: 	PUSHFL	R2
    // asm 0000681A: 	PUSH	R3
    // asm 0000681B: 	PUSH	AR0
    // asm 0000681C: 	PUSH	AR2
    // asm 0000681D: 	PUSH	AR4
    // asm 0000681E: 	PUSH	AR5
    // asm 0000681F: 	LDPI	@PLYCAR,AR4
    // asm 00006820: 	CMPI	0,AR4
    // asm 00006821: 	BEQ	PRR_ONRITE
    // asm 00006822: 	LDI	*+AR4(OCARBLK),AR5
RIDE_RIGHT_JOININ:
    // asm 00006823: 	LDI	*+AR5(CARTRAK),AR2
    // asm 00006824: 	LDI	*+AR2(OLINK4),R0
    // asm 00006825: 	BZ	PRR_ONRITE
    // asm 00006826: 	LDI	R0,AR0
    // asm 00006827: 	LDF	*+AR2(OPOSZ),R0
    // asm 00006828: 	SUBF	*+AR0(OPOSZ),R0		;A = Uy - Vy
    // asm 00006829: 	LDF	*+AR0(OPOSX),R1
    // asm 0000682A: 	SUBF	*+AR2(OPOSX),R1		;B = Vx - Ux
    // asm 0000682B: 	NEGF	R0,R2
    // asm 0000682C: 	NEGF	R1,R3
    // asm 0000682D: 	MPYF	*+AR2(OPOSX),R2
    // asm 0000682E: 	MPYF	*+AR2(OPOSZ),R3
    // asm 0000682F: 	ADDF	R3,R2			;C = -(A Ux + B Uy)
    // ;	NEGF	R2
    // asm 00006830: 	PUSHF	R0
    // asm 00006831: 	PUSHF	R2
    // asm 00006832: 	MPYF	R0,R0,R0
    // asm 00006833: 	MPYF	R1,R1,R2
    // asm 00006834: 	ADDF	R0,R2
    // asm 00006835: 	CALL	SQRT
    // asm 00006836: 	LDF	R0,R3
    // asm 00006837: 	POPF	R2
    // asm 00006838: 	POPF	R0
    // asm 00006839: 	MPYF	*+AR4(OPOSX),R0
    // asm 0000683A: 	MPYF	*+AR4(OPOSZ),R1
    // asm 0000683B: 	ADDF	R1,R0
    // asm 0000683C: 	ADDF	R2,R0
    // asm 0000683D: 	LDF	R3,R1
    // asm 0000683E: 	CALL	DIV_F
    // asm 0000683F: 	LDF	R0,R0			;dist = (A Ux + B Uy + C)/sqrt(A^2 + B^2)
    // asm 00006840: 	BLT	PRR_ONRITE
    // asm 00006841: PRR_NONRITE
    // asm 00006841: 	CLRC
    // asm 00006842: 	BU	PRR_X
PRR_ONRITE:
    // asm 00006843: 	SETC
PRR_X:
    // asm 00006844: 	POP	AR5
    // asm 00006845: 	POP	AR4
    // asm 00006846: 	POP	AR2
    // asm 00006847: 	POP	AR0
    // asm 00006848: 	POP	R3
    // asm 00006849: 	POPFL	R2
    // asm 0000684B: 	POP	R1
    // asm 0000684C: 	RETS
    return DRONE_RIDE_RIGHT(PLYCAR, PLYCAR->carblk);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SMOKE_PUFF
 *
 *PARAMETERS
 *	AR4	OBJECT THAT IS SMOKING
 *
 */
/* asm: SMOKE_COUNT	.bss	SMOKE_COUNT,1 */
int SMOKE_COUNT;

void SMOKE_PUFF(void) {
    // asm 0000684D: 	LDI	@SMOKE_COUNT,R0
    // asm 0000684E: 	CMPI	5,R0
    // asm 0000684F: 	BGE	SUICIDE
    // asm 00006850: 	INC	R0
    // asm 00006851: 	STI	R0,@SMOKE_COUNT
    // asm 00006852: 	LDL	40000,R0
    // asm 00006853: 	CMPI	*+AR4(ODIST),R0
    // asm 00006854: 	BLT	SUICIDE
    // asm 00006855: 	LDI	AR4,AR6
    // asm 00006856: 	CALL	OBJ_GET
    // asm 00006857: 	BC	SUICIDE
    // asm 00006858: 	LDI	AR0,AR4
    // asm 00006859: 	LDI	@SMOKE_ANII,AR5
    // asm 0000685A: 	LDI	*AR5,AR0
    // asm 0000685B: 	STI	AR0,*+AR4(OROMDATA)
    // asm 0000685C: 	LDI	UTIL_C|SMOKE_T,R0
    // asm 0000685D: 	STI	R0,*+AR4(OID)
    // asm 0000685E: 	LDL	bnout1_smoke,AR2
    // asm 0000685F: 	CALL	PAL_FIND_RAW
    // asm 00006860: 	STI	R0,*+AR4(OPAL)
    // asm 00006861: 	LDF	*+AR6(OPOSX),R0
    // asm 00006862: 	STF	R0,*+AR4(OPOSX)
    // asm 00006863: 	LDF	*+AR6(OPOSY),R0
    // asm 00006864: 	STF	R0,*+AR4(OPOSY)
    // asm 00006865: 	LDF	*+AR6(OPOSZ),R0
    // asm 00006866: 	STF	R0,*+AR4(OPOSZ)
    // asm 00006867: 	ORM	O_POSTER|O_1PAL,*+AR4(OFLAGS)
    // asm 0000686A: 	LDI	AR4,AR2
    // asm 0000686B: 	CALL	OBJ_INSERT
SMOKE_PUFFLP:
    // asm 0000686C: 	LDL	40000,R0
    // asm 0000686D: 	CMPI	*+AR4(ODIST),R0
    // asm 0000686E: 	BLT	SMOKE_DIE
    // asm 0000686F: 	LDF	6,R0
    // asm 00006870: 	CALL	SFRAND
    // asm 00006871: 	ADDF	*+AR4(OPOSX),R0
    // asm 00006872: 	STF	R0,*+AR4(OPOSX)
    // asm 00006873: 	LDF	6,R0
    // asm 00006874: 	CALL	SFRAND
    // asm 00006875: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00006876: 	STF	R0,*+AR4(OPOSZ)
    // asm 00006877: 	LDI	*AR5++,AR0
    // asm 00006878: 	CMPI	-1,AR0
    // asm 00006879: 	BEQ	SMOKE_DIE
    // asm 0000687A: 	STI	AR0,*+AR4(OROMDATA)
    // asm 0000687B: 	LDI	*AR5++,AR2
    // asm 0000687C: 	LDF	*+AR4(OPOSY),R0
    // asm 0000687D: 	SUBF	*AR5++,R0
    // asm 0000687E: 	STF	R0,*+AR4(OPOSY)
    // asm 0000687F: 	CALL	SLEEP
    // asm 00006880: 	BU	SMOKE_PUFFLP
SMOKE_DIE:
    // asm 00006881: 	LDI	@SMOKE_COUNT,R0
    // asm 00006882: 	DEC	R0
#if DEBUG
    // asm: 	BLT	$
#endif
    // asm 00006883: 	STI	R0,@SMOKE_COUNT
    // asm 00006884: 	LDI	AR4,AR2
    // asm 00006885: 	CALL	OBJ_DELETE
    // asm 00006886: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOKE_PUFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: SMOKE_ANI */
/* asm: 	.word	bnout1,1 */
/* asm: 	.float	10 */
/* asm: 	.word	bnout2,2 */
/* asm: 	.float	12 */
/* asm: 	.word	bnout3,1 */
/* asm: 	.float	16 */
/* asm: 	.word	bnout4,1 */
/* asm: 	.float	20 */
/* asm: 	.word	bnout5,1 */
/* asm: 	.float	24 */
/* asm: 	.word	bnout6,1 */
/* asm: 	.float	30 */
/* asm: 	.word	bnout7,1 */
/* asm: 	.float	35 */
/* asm: 	.word	bnout8,1 */
/* asm: 	.float	40 */
/* asm: 	.word	bnout9,1 */
/* asm: 	.float	45 */
/* asm: 	.word	-1 */
/* asm: 	 */
static int SMOKE_ANI[] = {
    bnout1_ROM,
    1,
    10,
    bnout2_ROM,
    2,
    12,
    bnout3_ROM,
    1,
    16,
    bnout4_ROM,
    1,
    20,
    bnout5_ROM,
    1,
    24,
    bnout6_ROM,
    1,
    30,
    bnout7_ROM,
    1,
    35,
    bnout8_ROM,
    1,
    40,
    bnout9_ROM,
    1,
    45,
    -1,
};
// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	AR5	PALM TREE OBJECT
 *
 */
/* asm: COCONUT_COUNT	.bss	COCONUT_COUNT,1 */
int COCONUT_COUNT;

void DROP_COCONUTS(void) {
    // asm 00006888: 	LDI	@COCONUT_COUNT,R0
    // asm 00006889: 	CMPI	5,R0
    // asm 0000688A: 	BGE	SUICIDE
    // asm 0000688B: 	INC	R0
    // asm 0000688C: 	STI	R0,@COCONUT_COUNT
    // asm 0000688D: 	CALL	OBJ_GET
    // asm 0000688E: 	BC	SUICIDE
    // asm 0000688F: 	LDI	AR0,AR4
    // asm 00006890: 	LDF	*+AR5(OPOSX),R0
    // asm 00006891: 	STF	R0,*+AR4(OPOSX)
    // asm 00006892: 	LDF	*+AR5(OPOSY),R0
    // asm 00006893: 	FLOAT	1500,R1
    // asm 00006894: 	SUBF	R1,R0
    // asm 00006895: 	STF	R0,*+AR4(OPOSY)
    // asm 00006896: 	LDF	*+AR5(OPOSZ),R0
    // asm 00006897: 	STF	R0,*+AR4(OPOSZ)
    // asm 00006898: 	LDL	coco1,R0
    // asm 00006899: 	STI	R0,*+AR4(OROMDATA)
    // asm 0000689A: 	LDI	*+AR4(OFLAGS),R0
    // asm 0000689B: 	OR	O_POSTER,R0
    // asm 0000689C: 	STI	R0,*+AR4(OFLAGS)
    // asm 0000689D: 	LDI	AR4,AR2
    // asm 0000689E: 	CALL	OBJ_INSERT
    // asm 0000689F: 	LDI	@_MODE,R0
    // asm 000068A0: 	AND	MMODE,R0
    // asm 000068A1: 	CMPI	MATTR,R0
    // asm 000068A2: 	BEQ	NSND1
    // asm 000068A3: 	SONDFX	COCONUTLOW
NSND1:
    // asm 000068A5: 	LDF	*+AR4(OPOSY),R0
    // asm 000068A6: 	STF	R0,*+AR7(PDATA+1)	;save initial base Y
    // asm 000068A7: 	CALL	OBJSCAN
    // asm 000068A8: 	STF	R0,*+AR7(PDATA)
    // asm 000068A9: 	RANDN	20
    // asm 000068AB: 	MPYI	3,R0
    // asm 000068AC: 	FLOAT	R0
    // asm 000068AD: 	ADDF	20,R0
    // asm 000068AE: 	STF	R0,*+AR4(OVELY)
    // asm 000068AF: 	RANDN	20
    // asm 000068B1: 	MPYI	3,R0
    // asm 000068B2: 	FLOAT	R0
    // asm 000068B3: 	SUBF	30,R0
    // asm 000068B4: 	ADDF	*+AR4(OPOSX),R0
    // asm 000068B5: 	STF	R0,*+AR4(OPOSX)
    // asm 000068B6: 	RANDN	20
    // asm 000068B8: 	MPYI	3,R0
    // asm 000068B9: 	FLOAT	R0
    // asm 000068BA: 	SUBF	30,R0
    // asm 000068BB: 	ADDF	*+AR4(OPOSZ),R0
    // asm 000068BC: 	STF	R0,*+AR4(OPOSZ)
    // asm 000068BD: 	CLRI	R0
    // asm 000068BE: 	STI	R0,*+AR7(PDATA+2)
    // asm 000068BF: 	CLRF	R0
    // asm 000068C0: 	STF	R0,*+AR4(OVELX)
    // asm 000068C1: 	STF	R0,*+AR4(OVELZ)
DROPLP:
    // asm 000068C2: 	LDI	*+AR4(ODIST),R0
    // asm 000068C3: 	BN	DROPCOCOKILL
    // asm 000068C4: 	CMPI	20000,R0
    // asm 000068C5: 	BGT	DROPCOCOKILL
    // asm 000068C6: 	LDF	*+AR4(OVELY),R0
    // asm 000068C7: 	ADDF	25,R0
    // asm 000068C8: 	FLOAT	450,R1
    // asm 000068C9: 	CMPF	R1,R0
    // asm 000068CA: 	LDFGT	R1,R0
    // asm 000068CB: 	STF	R0,*+AR4(OVELY)
    // asm 000068CC: 	LDF	*+AR4(OPOSX),R0
    // asm 000068CD: 	ADDF	*+AR4(OVELX),R0
    // asm 000068CE: 	STF	R0,*+AR4(OPOSX)
    // asm 000068CF: 	LDF	*+AR4(OPOSZ),R0
    // asm 000068D0: 	ADDF	*+AR4(OVELZ),R0
    // asm 000068D1: 	STF	R0,*+AR4(OPOSZ)
    // asm 000068D2: 	LDF	*+AR4(OPOSY),R0
    // asm 000068D3: 	ADDF	*+AR4(OVELY),R0
    // asm 000068D4: 	STF	R0,*+AR4(OPOSY)
    // asm 000068D5: 	CMPF	*+AR5(OPOSY),R0
    // asm 000068D6: 	BLT	FRSL
    // asm 000068D7: 	LDI	*+AR7(PDATA+2),R0
    // asm 000068D8: 	BNZ	NOTINITIAL
    // asm 000068D9: 	RANDN	30
    // asm 000068DB: 	MPYI	3,R0
    // asm 000068DC: 	FLOAT	R0
    // asm 000068DD: 	SUBF	45,R0
    // asm 000068DE: 	STF	R0,*+AR4(OVELX)
    // asm 000068DF: 	RANDN	30
    // asm 000068E1: 	MPYI	3,R0
    // asm 000068E2: 	FLOAT	R0
    // asm 000068E3: 	SUBF	45,R0
    // asm 000068E4: 	STF	R0,*+AR4(OVELZ)
    // asm 000068E5: 	BU	LLKK
NOTINITIAL:
    // asm 000068E6: 	CMPI	3,R0
    // asm 000068E7: 	BEQ	HOLDTOSLEEP
    // asm 000068E8: 	LDF	*+AR4(OVELX),R0
    // asm 000068E9: 	MPYF	0.5,R0
    // asm 000068EA: 	STF	R0,*+AR4(OVELX)
    // asm 000068EB: 	LDF	*+AR4(OVELZ),R0
    // asm 000068EC: 	MPYF	0.5,R0
    // asm 000068ED: 	STF	R0,*+AR4(OVELZ)
LLKK:
    // asm 000068EE: 	LDI	*+AR7(PDATA+2),R0
    // asm 000068EF: 	INC	R0
    // asm 000068F0: 	STI	R0,*+AR7(PDATA+2)
    // asm 000068F1: 	LDF	*+AR4(OVELY),R0
    // asm 000068F2: 	CMPF	3,R0
    // asm 000068F3: 	BLE	HOLDTOSLEEP
    // asm 000068F4: 	MPYF	-0.5,R0
    // asm 000068F5: 	STF	R0,*+AR4(OVELY)
    // asm 000068F6: 	ADDF	*+AR4(OPOSY),R0
    // asm 000068F7: 	STF	R0,*+AR4(OPOSY)
    // asm 000068F8: 	LDI	@_MODE,R0
    // asm 000068F9: 	AND	MMODE,R0
    // asm 000068FA: 	CMPI	MATTR,R0
    // asm 000068FB: 	BEQ	NSND2
    // asm 000068FC: 	RANDN	1
    // asm 000068FE: 	LDI	R0,R0
    // asm 000068FF: 	LDIZ	COCONUTHI,AR2
    // asm 00006900: 	LDINZ	COCONUTLOW,AR2
    // asm 00006901: 	CALL	ONESNDFX
NSND2:
FRSL:
    // asm 00006902: 	SLEEP	1
    // asm 00006904: 	BU	DROPLP
HOLDTOSLEEP:
    // asm 00006905: 	LDI	*+AR4(ODIST),R0
    // asm 00006906: 	BN	DROPCOCOKILL
    // asm 00006907: 	SLEEP	1
    // asm 00006909: 	B	HOLDTOSLEEP
DROPCOCOKILL:
    // asm 0000690A: 	LDI	@COCONUT_COUNT,R0
    // asm 0000690B: 	DEC	R0
#if DEBUG
    // asm: 	BLT	$
#endif
    // asm 0000690C: 	STI	R0,@COCONUT_COUNT
    // asm 0000690D: 	LDI	AR4,AR2
    // asm 0000690E: 	CALL	OBJ_DELETE
    // asm 0000690F: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DROP_COCONUTS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: MODELTAB	.word	cvettem,hotrodm,missle,testorm */
/* asm: 	 */
static int MODELTAB[] = {
    cvettem_ROM,
    hotrodm_ROM,
    missle_ROM,
    testorm_ROM,
};
/* asm: TEXTTABS	.word	VETETXT,RODRTXT,BULLTXT,FERRTXT */
/* asm: 	 */
static const char** TEXTTABS[] = {
    VETETXT,
    RODRTXT,
    BULLTXT,
    FERRTXT,
};
/* asm: TITLES	.word	HRT12,HRT13,HRT14,HRT15,HRT16 */
/* asm: 	 */
const char* TITLES[] = {
    HRT12,
    HRT13,
    HRT14,
    HRT15,
    HRT16,
};
/* asm: VETETXT	.word	HRS11,HRS12,HRS13,HRS14,HRS15,HRS16 */
static const char* VETETXT[] = {
    HRS11,
    HRS12,
    HRS13,
    HRS14,
    HRS15,
    HRS16,
};
/* asm: RODRTXT	.word	HRS21,HRS22,HRS23,HRS24,HRS25,HRS26 */
static const char* RODRTXT[] = {
    HRS21,
    HRS22,
    HRS23,
    HRS24,
    HRS25,
    HRS26,
};
/* asm: BULLTXT	.word	HRS31,HRS32,HRS33,HRS34,HRS35,HRS36 */
static const char* BULLTXT[] = {
    HRS31,
    HRS32,
    HRS33,
    HRS34,
    HRS35,
    HRS36,
};
/* asm: FERRTXT	.word	HRS41,HRS42,HRS43,HRS44,HRS45,HRS46 */
/* asm: 	 */
static const char* FERRTXT[] = {
    HRS41,
    HRS42,
    HRS43,
    HRS44,
    HRS45,
    HRS46,
};
static const char HRT12[] = "TOP SPEED:";
static const char HRT13[] = "SKIDPAD:";
static const char HRT14[] = "AERO COEFF:";
static const char HRT15[] = "0?60 MPH:";
static const char HRT16[] = "POWER:";
/* asm: TABING	.float	60,220,220,220,220,220 */
/* asm: 	 */
/* asm: 	 */
float TABING[] = {
    60.0f,
    220.0f,
    220.0f,
    220.0f,
    220.0f,
    220.0f,
};
static const char HRS11[] = "63 MUSCLE CAR";
static const char HRS12[] = "145 MPH@233 KPH";
static const char HRS13[] = "0=89 G";
static const char HRS14[] = "0=39";
static const char HRS15[] = "2=98 SEC=";
static const char HRS16[] = "462HP  454 CID V8";
static const char HRS21[] = "LA BOMBA";
static const char HRS22[] = "142 MPH@228 KPH";
static const char HRS23[] = "0=98 G";
static const char HRS24[] = "0=45";
static const char HRS25[] = "2=51 SEC=";
static const char HRS26[] = "580HP  SUPERCHARGED V12";
static const char HRS31[] = "DEVASTATOR VI";
static const char HRS32[] = "147 MPH@236 KPH";
static const char HRS33[] = "0=85 G";
static const char HRS34[] = "0=25";
static const char HRS35[] = "3=15 SEC=";
static const char HRS36[] = "395HP  TWIN TURBO V6";
static const char HRS41[] = "ITALIA P69";
static const char HRS42[] = "144 MPH@231 KPH";
static const char HRS43[] = "1=01 G";
static const char HRS44[] = "0=28";
static const char HRS45[] = "2=88 SEC=";
static const char HRS46[] = "472HP  V12 DOHC 48V";
// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *AM I AHEAD OF THE PLAYER?
 *
 *PARAMETERS
 *	AR4	OBJECT
 *	AR5	CAR BLOCK
 *
 *RETURNS
 *	CARRY SET ON AHEAD OF PLAYER
 *	CARRY CLR ON BEHIND PLAYER
 *
 */
void AHEAD_OF_PLAYER_P(void) {
    // asm 0000691F: 	PUSH	AR0
    // asm 00006920: 	PUSH	AR1
    // asm 00006921: 	PUSH	R0
    // asm 00006922: 	LDI	@PLYCBLK,AR0
    // asm 00006923: 	LDI	*+AR0(CARTRAK),AR0
    // asm 00006924: 	LDI	*+AR0(OUSR1),R0
    // asm 00006925: 	LDI	*+AR5(CARTRAK),AR1
    // asm 00006926: 	CMPI	*+AR1(OUSR1),R0
    // asm 00006927: 	BLT	AHEADOF
    // asm 00006928: 	BGT	ISBEHIND
    // asm 00006929: 	LDI	*+AR0(OLINK4),AR0	;GET NEXT OBJECT
    // asm 0000692A: 	LDI	*+AR0(OLINK4),AR0	;GET NEXT OBJECT
    // asm 0000692B: 	LDF	*+AR0(OPOSX),R0
    // asm 0000692C: 	SUBF	*+AR4(OPOSX),R0
    // asm 0000692D: 	MPYF	R0,R0
    // asm 0000692E: 	LDF	*+AR0(OPOSZ),R1
    // asm 0000692F: 	SUBF	*+AR4(OPOSZ),R1
    // asm 00006930: 	MPYF	R1,R1
    // asm 00006931: 	ADDF	R1,R0
    // asm 00006932: 	LDI	@(_plyr1+PLY_CAR),AR1
    // asm 00006933: 	LDF	*+AR0(OPOSX),R2
    // asm 00006934: 	SUBF	*+AR1(OPOSX),R2
    // asm 00006935: 	MPYF	R2,R2
    // asm 00006936: 	LDF	*+AR0(OPOSZ),R1
    // asm 00006937: 	SUBF	*+AR1(OPOSZ),R1
    // asm 00006938: 	MPYF	R1,R1
    // asm 00006939: 	ADDF	R2,R1
    // asm 0000693A: 	CMPF	R1,R0	;WHO IS CLOSER?
    // asm 0000693B: 	BLT	AHEADOF
ISBEHIND:
    // asm 0000693C: 	CLRC
    // asm 0000693D: 	POP	R0
    // asm 0000693E: 	POP	AR1
    // asm 0000693F: 	POP	AR0
    // asm 00006940: 	RETS
AHEADOF:
    // asm 00006941: 	SETC
    // asm 00006942: 	POP	R0
    // asm 00006943: 	POP	AR1
    // asm 00006944: 	POP	AR0
    // asm 00006945: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AHEAD_OF_PLAYER_P", 0, 0);
    UNIMPL();
}

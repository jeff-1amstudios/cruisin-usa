
#include "plyr.h"
#include "../core/machine.h"
#include "c30.h"
#include "cmos.h"
#include "comm.h"
#include "delta.h"
#include "dirq.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"
#include <string.h>

extern MATRIX _MATRIXA;

/*
 * Source module: asm/PLYR.ASM
 */

static void GETCARPARAM(void);
CARBLK* _CARV0(OBJ* obj /*AR4*/, int vehicle /*R0*/);
void BONUS_WAIT_LOOP(void);
void PLYR_CAR_INIT(void);
void PLYR_INTRO_ENTER(void);
void _PLYR(void);
static void CAMCHKL(void);
static void CAMCHKR(void);
static void CAMCHKLR(void);
static void CAMCHK(void);
static void CAMROT(void);
static void GETCAMPOS(void);
void CAMYADJ(void);
static void PLYONRD(void);
void DRONEGO(void);
void DRONESTOP(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/);
static void GETREV(void);
void GETRPM(void);
static void GETSKID(void);
static void CKOFRD(void);
static void GETDIR(void);
static void CARSPIN(void);
static void GETCARROT(void);
void GETSPD(void);
static void GETBRAKE(void);
void _off_brake(void);
void _on_brake(void);
static void GETPEDAL(void);
static void GETGEAR(void);
static void GETMAN(void);
void GETAUTO(void);
static void GETSTEER(void);
void _VIEW0(void);
void _VIEW1(void);
void _VIEW2(void);
static void ZOOMUP(void);
void GETTRAK(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/);
static void BACKCK(void);
static void CKBND(void);
static void TUNCHK(void);
void INBOUNDZ(void);
void DRONINBZ(void);
static void CURBCOL0(void);
static void CURBSPIN(void);
static void CURBSPN(void);
static void SOFTCURB(void);
void GETNXTRDIR(void);
static void GETRDCAR(void);
float ROADIR(OBJ* track_obj /*AR2*/);
float GETRDIR(OBJ* track_obj /*AR2*/);
static void PLYRWHL(void);
static void PLYR_SNDS(void);
void MKFXSND(void);
static void MKVFXSND(void);
void RANDSND(void);
void RANDVSND(void);
void DRONESND(void);
void DRONESND1(void);
void GETCMOS_VALUES(void);
static void CAMMATSAV(void);
static void CAMMATAVG(void);

#define ZOOMI ZOOMRAM
#define CARPARAMTABI CARPARAMTAB
#define GEARTABI GEARTAB
#define GEARACTABI GEARACTAB
#define ENGACTABI ENGACTAB
#define ENGFRI ENGFR
#define HIREDI HIRED
#define OFFREDI OFFRED
#define SCOLLTABI SCOLLTAB
#define WALLHITABI WALLHITAB
#define SKIDTABI SKIDTAB
#define PLAIRSNDI PLAIRSND
#define REVSNDTABI REVSNDTAB
#define PEDALMNI PEDALMN

void WRECK(void);
void WRECKST(void);
void COMPTRAK(void);

extern float STEERI;
static tCARPARAM CARPARAMTAB[];
static float AHEAD;
static float CATCHUP;
static float DISTCON;
static float SPDCON;
static float GEARACTAB[5];
static float ENGACTAB[20];

/*
 *----------------------------------------------------------------------------
 */

#define JARVK 0
// *THESE ARE EDGE VARIABLES AND SHOULD NOT BE MESSED WITH

/* asm: OFFROAD_TMR	.bss	OFFROAD_TMR,1 */
int OFFROAD_TMR;
/* asm: ZOOMRAM	.bss	ZOOMRAM,0 */
static int ZOOMRAM;
/* asm: ZOOMD	.bss	ZOOMD,1 */
float ZOOMD = 1.0f;
/* asm: ZOOMDD	.bss	ZOOMDD,1 */
float ZOOMDD = 1.0f;
/* asm: ZOOMDG	.bss	ZOOMDG,1 */
float ZOOMDG = 1.0f;
/* asm: ZOOMH	.bss	ZOOMH,1 */
int ZOOMH;
/* asm: ZOOMHD	.bss	ZOOMHD,1 */
float ZOOMHD = 1.0f;
/* asm: ZOOMHG	.bss	ZOOMHG,1 */
float ZOOMHG = 1.0f;
/* asm: CAMVIEW	.bss	CAMVIEW,1 */
int CAMVIEW;
/* asm: BRAKEON	.bss	BRAKEON,1 */
int BRAKEON;
/* asm: WRECKFLG	.bss	WRECKFLG,1 */
int WRECKFLG;
/* asm: REVFLG	.bss	REVFLG,1 */
int REVFLG;
/* asm: _plyr1	.bss	_plyr1,0 */
int _plyr1;
/* asm: PLYSTAT	.bss	PLYSTAT,1 */
int PLYSTAT;
/* asm: PLYCAR	.bss	PLYCAR,1 */
OBJ* PLYCAR;
/* asm: PLYPROC	.bss	PLYPROC,1 */
PROC* PLYPROC;
/* asm: PLYCBLK	.bss	PLYCBLK,1 */
CARBLK* PLYCBLK;
/* asm: OLDPLYSPD	.bss	OLDPLYSPD,1 */
float OLDPLYSPD = 1.0f;
/* asm: OLDPLYAIR	.bss	OLDPLYAIR,1 */
int OLDPLYAIR;
/* asm: PLYRFIRST	.bss	PLYRFIRST,1 */
int PLYRFIRST;
/* asm: PLAIRTIM	.bss	PLAIRTIM,1 */
int PLAIRTIM;
/* asm: CHEATACC	.bss	CHEATACC,1 */
int CHEATACC;
/* asm: CHEAT	.bss	CHEAT,1 */
float CHEAT = 1.0f;
// *PLAYER 1ST, 2ND, 3RD POSTION COORDS:

#define PLYPOS1Z 0
#define PLYPOS1Y (-150)
#if JARVK
#define PLYPOS2Z 3840
#define PLYPOS2Y (-700)
#define PLYPOS3Z 6000
#define PLYPOS3Y (-1000)
#else
#undef PLYPOS2Z
#define PLYPOS2Z 2200
#undef PLYPOS2Y
#define PLYPOS2Y (-400)
#undef PLYPOS3Z
#define PLYPOS3Z 3840
#undef PLYPOS3Y
#define PLYPOS3Y (-700)
#endif
// ;PLYPOS3Y	.set	-2700	;good heli height
#define ZOOMRATIO 0.05 // 1/ZOOMTIME FOR VIEW CHANGE
// *SWITCH BIT VALUES

#define SHIFT 4 // LO-HI SHIFT LEVER
#define BRAKE 1 // BRAKE PEDAL
// *RAM VARIABLES

/* asm: PLMSAV	.bss	PLMSAV,15 */
static int PLMSAV[15];
/* asm: PMSAV	.bss	PMSAV,9 */
static int PMSAV[9];
/* asm: ZOOMI	.word	ZOOMRAM */
/* asm: 	 */
/* asm: 	 */
#define ZOOMI ZOOMRAM
#define GRAVITY 1.20
/* asm: ROADFRICI	.float	0.0028 */
static float ROADFRICI = 0.0028f;
/* asm: OFRDFRICI	.float	0.010 */
static float OFRDFRICI = 0.010f;
/* asm: BRAKFRICI	.float	0.020 */
static float BRAKFRICI = 0.020f;
/* asm: SKIDFRICI	.float	0.003 */
static float SKIDFRICI = 0.003f;
/* asm: SPINFRICI	.float	0.015	;SPINOUT FRICTION */
/* asm: 	 */
/* asm: 	 */
static float SPINFRICI = 0.015f;
// *RPM MAX

#define NUM_RPMS 47
#define NUM_RPM 47.0
#define OVERREV 51.0
/*
 *----------------------------------------------------------------------------
 *PLAYER CAR SPECIALIZED PARAMETER TABLE
 *
 *ACCELERATION, TRACTION (0=total traction), ONROAD DAMPING, OFFROAD DAMPING
 *
 */
/*
 *STDARD .float	0.82,1.00,0.0028,0.010
 *NEWSTD	.float	0.82,0.90,0.0028,0.0060
 */
static tCARPARAM CARPARAMTAB[] = {
    // #0 MUSCLE CAR
    { 0.91, 0.60, 0.0028, 0.010 }, // ALL AROUND
    // #1 XXX
    { 0.98, 0.50, 0.0032, 0.0042 }, // ACCEL
    // #2 MISSILE
    { 0.88, 0.70, 0.0026, 0.010 }, // TOP SPEED
    // #3 FERRARI
    { 0.89, 0.50, 0.0028, 0.010 }, // HANDLING
    // HIDDEN VEHICLES
    // #4 jeep
    { 0.95, 0.60, 0.0030, 0.0039 },
    // #5 sbusp
    { 0.89, 0.50, 0.0028, 0.010 },
    // #6 copcar
    { 0.91, 0.65, 0.0028, 0.0050 },
    // #7 gtruck
    { 0.89, 0.50, 0.0028, 0.010 },
};
#define CARPARAMTABL (CARPARAMTAB1 - CARPARAMTAB) // LENGTH OF ENTRY

/*
 *----------------------------------------------------------------------------
 *GET CAR PARAMETERS FOR PLAYER
 *PARAMETERS
 *	R0	CAR # 0-3
 *	AR0	CAR BLOCK INDEX
 *LOADS PARAMETERS INTO CAR BLOCK
 *TRASHES R0,AR2
 */
static void GETCARPARAM(void) {
    // asm 00002939: 	LDI	@CARPARAMTABI,AR2
    // asm 0000293A: 	MPYI	CARPARAMTABL,R0
    // asm 0000293B: 	ADDI	R0,AR2
    // asm 0000293C: 	LDF	*AR2++,R0
    // asm 0000293D: 	STF	R0,*+AR0(CARMAXACCEL)
    // asm 0000293E: 	LDF	*AR2++,R0
    // asm 0000293F: 	STF	R0,*+AR0(CARTRACTION)
    // asm 00002940: 	LDF	*AR2++,R0
    // asm 00002941: 	STF	R0,*+AR0(CARRDFR)
    // asm 00002942: 	LDF	*AR2++,R0
    // asm 00002943: 	STF	R0,*+AR0(CAROFRDFR)
    // asm 00002944: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCARPARAM", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *INIT CAR 3 POINT SUSPENSION DATA STRUCTURE
 *PARAMETERS
 *	AR4	OBJECT BLOCK
 *	R0	VEHICLE #
 *RETURNS	AR0	POINTS TO CARBLOCK
 *
 *	NO CARRY ON FAILURE TO ALLOCATE CAR BLOCK
 *
 *TRASHED	R1-R7
 */
CARBLK* _CARV0(OBJ* obj /*AR4*/, int vehicle /*R0*/) {
    (void)vehicle;
    CARBLK* car;
    OROMDATA* romdata;

    // asm 00002945: 	PUSH	R0
    // asm 00002946: 	PUSH	AR1
    // asm 00002947: 	CALL	GETCAR
    // asm 00002948: 	BNC	CARV_ERR
    // asm 00002949: 	STI	AR0,*+AR4(OCARBLK)
    car = GETCAR();
    if (car == NULL) {
        goto CARV_ERR;
    }
    obj->carblk = car;

    // asm 0000294A: 	LDF	0,R0
    // asm 0000294B: 	RPTS	CARSIZ-1
    // asm 0000294C: 	STF  	R0,*AR0++		;CLEAR OUT THE BLOCK
    memset(car, 0, sizeof(*car));

    // asm 0000294D: 	SUBI	CARSIZ,AR0	 	;RESTORE AR2
    // asm 0000294E: 	CALL	_makbox			;GET YOUR CAR BOX
    _makbox(obj); // GET YOUR CAR BOX

    // asm 0000294F: 	LDI	*+AR4(OROMDATA),AR1	;TWEAK RADIUS SLIGHTLY
    // asm 00002950: 	FLOAT	*AR1,R0
    // asm 00002951: 	MPYF	1.1,R0
    // asm 00002952: 	FIX	R0
    // asm 00002953: 	STI	R0,*+AR4(ORAD)		;GET OBJECT RADIUS
    romdata = obj->romdata;
    obj->radius = (int)(1.1f * (float)romdata->radius); // GET OBJECT RADIUS

    // asm 00002954: 	LDI	0,R0 			;INIT FLAGS
    // asm 00002955: 	STI	R0,*+AR0(CAR_SPIN)
    // asm 00002956: 	STI	R0,*+AR0(CAR_ONROAD)
    // asm 00002957: 	STI	R0,*+AR0(CAR_AIRF)
    // asm 00002958: 	STI	R0,*+AR0(CAR_AIRB)
    // asm 00002959: 	STI	R0,*+AR0(CARPTSTR)	;POSITION TRACKING SYSTEM
    // asm 0000295A: 	STI	R0,*+AR0(CARPTSRANK)
    // asm 0000295B: 	STI	R0,*+AR0(CAR_BUMP)
    // asm 0000295C: 	STI	R0,*+AR0(CARGEAR)
    // asm 0000295D: 	STI	R0,*+AR0(CARTRANS)  	;AUTO IS DEFAULT
    // asm 0000295E: 	STI	R0,*+AR0(CARNUM)  	;CLEAR DISPATCH NUMBER
    // asm 0000295F: 	STI	R0,*+AR0(CARTRACK_ID)	;CLEAR TRACK ID
    // asm 00002960: 	STI	R0,*+AR0(CAR_OM)	;CLEAR OTHER MACHINE FLAG
    // The block was memset to 0, so these fields are already initialized:
    // AUTO IS DEFAULT
    // CLEAR DISPATCH NUMBER
    // CLEAR TRACK ID
    // CLEAR OTHER MACHINE FLAG

    // asm 00002961: 	LDF	0,R0
    // asm 00002962: 	STF	R0,*+AR0(CARRPM)
    car->rpm_x100 = 0.0f;

    // asm 00002963: 	LDI	1,R0
    // asm 00002964: 	STI	R0,*+AR0(CARSHAD)	;SHADOW ON
    car->shadow_flag = 1; // SHADOW ON

    // asm 00002965: 	LDF	@ROADFRICI,R0		;FRICTION COEFFICIENTS
    // asm 00002966: 	STF	R0,*+AR0(CARRDFR)
    // asm 00002967: 	LDF	@OFRDFRICI,R0
    // asm 00002968: 	STF	R0,*+AR0(CAROFRDFR)
    // FRICTION COEFFICIENTS
    car->road_friction = ROADFRICI;
    car->offroad_friction = OFRDFRICI;

    // asm 00002969: 	LDF	0.82,R0
    // asm 0000296A: 	STF	R0,*+AR0(CARMAXACCEL)	;SET ACCEL POWER
    // asm 0000296B: 	LDF	1.0,R0
    // asm 0000296C: 	STF	R0,*+AR0(CARMASS)	;DEFAULT CAR MASS
    // asm 0000296D: 	STF	R0,*+AR0(CARTRACTION)	;DEFAULT TRACTION COEFFICIENT
    car->max_accel = 0.82f; // SET ACCEL POWER
    car->mass = 1.0f;       // DEFAULT CAR MASS
    car->traction = 1.0f;   // DEFAULT TRACTION COEFFICIENT

    // asm 0000296E: 	LDI	1,R0
    // asm 0000296F: 	LS	O_3DROT_B,R0
    // asm 00002970: 	OR	*+AR4(OFLAGS),R0	;FLAG SYSTEM AS NON-2D OPTIMIZABLE
    // asm 00002971: 	STI	R0,*+AR4(OFLAGS)
    obj->flags |= 1u << O_3DROT_B; // FLAG SYSTEM AS NON-2D OPTIMIZABLE

    // asm 00002972: 	SETC
    return car;

CARV_ERR:
    // asm 00002973: 	POP	AR1
    // asm 00002974: 	POP	R0
    // asm 00002975: 	RETS
    return NULL;
}

void BONUS_WAIT_LOOP(void) {
    // asm 00002976: 	LDI	@DID_TIMED_OUT,R0
    // asm 00002977: 	BNZ	BWLX
    // asm 00002978: 	LDI	@PLYCAR,AR2
    // asm 00002979: 	LDI	*+AR2(OFLAGS),R0	;CHECK IF ALREADY ON LIST
    // asm 0000297A: 	TSTB	O_LIST_M,R0
    // asm 0000297B: 	BNZ	BWLX			;YES, DONT INSERT
    // asm 0000297C: 	CALL	OBJ_INSERT		;INSERT PLAYER OBJECT
BWLX:
BONUS_WAIT_LP:
    // asm 0000297D: 	LDI	@PLYCAR,AR4
    // asm 0000297E: 	LDI	@PLYCBLK,AR5
    // asm 0000297F: 	CALL	ZOOMUP			;UPDATE YOUR ZOOM
    // asm 00002980: 	SLEEP	1
    // asm 00002982: 	LDI	@_MODE,R0
    // asm 00002983: 	AND	MMODE,R0
    // asm 00002984: 	CMPI	MGAME,R0
    // asm 00002985: 	BNE	BONUS_WAIT_LP
    // asm 00002986: 	BU	PLYR_ENTER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BONUS_WAIT_LOOP", 0, 0);
    UNIMPL();
}

void PLYR_CAR_INIT(void) {
    // asm 00002987: 	STI	AR4,@PLYCAR		;INIT CAR PLAYER STRUCT
    // asm 00002988: 	STI	AR7,@PLYPROC
    // asm 00002989: 	LDI	@CHOOSENCAR,AR1
    // asm 0000298A: 	CMPI	4,AR1
    // asm 0000298B: 	BLT	DOGENRLB
    // asm 0000298C: 	LDI	AR1,R0
    // asm 0000298D: 	CMPI	4,AR1
    // asm 0000298E: 	LDIEQ	JEEP_MOD,R0
    // asm 0000298F: 	CMPI	5,AR1
    // asm 00002990: 	LDIEQ	PLYR_SBUS_MOD,R0
    // asm 00002991: 	CMPI	6,AR1
    // asm 00002992: 	LDIEQ	PLYR_COPCAR_MOD,R0
    // asm 00002993: 	CMPI	7,AR1
    // asm 00002994: 	LDIEQ	3,R0
    // asm 00002995: 	LDI	R0,AR1
DOGENRLB:
    // asm 00002996: 	LDI	AR1,AR2
    // asm 00002997: 	MPYI	VEHTAB_SIZE,AR1
    // asm 00002998: 	ADDI	@VEHICLE_TABLEI,AR1
    // asm 00002999: 	LDI	*+AR1(VEHTAB_MODEL),R1
    // asm 0000299A: 	STI	R1,*+AR4(OROMDATA)
    // asm 0000299B: 	CALL	VEHICLE_ANI_INIT	;SETUP WHEEL ANIMATION
    // asm 0000299C: 	LDF	-1,R0
    // asm 0000299D: 	STF	R0,*+AR4(OUSR1)		;skid system flag
    // asm 0000299E: 	LDI	0,R0
    // asm 0000299F: 	STI	R0,@_MPH
    // asm 000029A0: 	STI	R0,@OLDPLYAIR		;OLD AIRBORNE FLAG
    // asm 000029A1: 	STI	R0,@PLAIRTIM		;CLEAR AIR TIMER
    // asm 000029A2: 	STI	R0,@ENGVOL		;ENGINE SOUND VOLUME
    // asm 000029A3: 	STI	AR7,*+AR4(OPLINK)	;SETUP OPLINK
    // asm 000029A4: 	LDI	AR4,AR2
    // asm 000029A5: 	CALL	OBJ_INSERT	 	;INSERT SUCKER ON THE LIST
    // asm 000029A6: 	LDF	0,R0
    // asm 000029A7: 	STF	R0,@OLDPLYSPD
    // asm 000029A8: 	LDI	@CHOOSENCAR,R0
    // asm 000029A9: 	CALL	_CARV0			;INIT CAR DATA STRUCT IN PROCESS
    // asm 000029AA: 	CALL	GETCARPARAM		;GET SPECIAL PLAYER CAR PARAMETERS
    // asm 000029AB: 	STI	AR0,@PLYCBLK
    // asm 000029AC: 	LDI	PLYR_C,R0
    // asm 000029AD: 	STI	R0,*+AR0(CAR_ID)
    // asm 000029AE: 	STI	R0,*+AR4(OID)
    // asm 000029AF: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 000029B0: 	STI	R0,*+AR0(CARTRANS)	;AUTO/MANUAL SWITCH
    // asm 000029B1: 	LDF	2.0,R0
    // asm 000029B2: 	STF	R0,*+AR0(CARMASS)	;SET CAR MASS
    // asm 000029B3: 	LDI	11,R0
    // asm 000029B4: 	STPI	R0,@OFFROAD_TMR
    // asm 000029B5: 	LDF	@STEERCT,R0		;STEERING CENTER
    // asm 000029B6: 	STF	R0,@WHEELPOS
    // asm 000029B7: 	RETS	;DONE INITIALIZING PLYR CAR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_CAR_INIT", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *PARAMETERS
 *	AR4	CAR OBJECT
 */
void PLYR_INTRO_ENTER(void) {
    // asm 000029B8: 	LDI	*+AR4(OCARBLK),AR5
    // asm 000029B9: 	LDI	0,R0		 	;NEUTRAL, PLEASE
    // asm 000029BA: 	STI	R0,*+AR5(CARGEAR)
    // asm 000029BB: 	CALL	GETTRAK
    // asm 000029BC: 	LDI	*+AR5(CARTRAK),AR2
    // 	;Set Appropriate Palette
    // 	;
    // asm 000029BD: 	LDI	@CHOOSENCAR,R0
    // asm 000029BE: 	LDI	R0,AR0
    // asm 000029BF: 	CMPI	4,R0
    // asm 000029C0: 	LDIEQ	JEEP_MOD,AR0
    // asm 000029C1: 	CMPI	5,R0
    // asm 000029C2: 	LDIEQ	PTRUCKG_MOD,AR0
    // asm 000029C3: 	CMPI	6,AR0
    // asm 000029C4: 	LDIEQ	COPCAR_MOD,AR0
    // asm 000029C5: 	CMPI	7,AR0
    // asm 000029C6: 	LDIGE	GTRUCK_MOD,AR0
    // asm 000029C7: 	MPYI	VEHTAB_SIZE,AR0
    // asm 000029C8: 	ADDI	@VEHICLE_TABLEI,AR0
    // asm 000029C9: 	LDI	*+AR0(VEHTAB_PAL),AR2
    // asm 000029CA: 	CALL	PAL_FIND
    // asm 000029CB: 	STI	R0,*+AR4(OPAL)
    // 	;
    // 	;INIT CAMERA
    // 	;
    // asm 000029CC: 	LDF	@START_RADY,R2		;SETUP MATRIX
    // asm 000029CD: 	STF	R2,*+AR5(CARYROT)
    // asm 000029CE: 	STF	R2,*+AR5(CARVROT)
    // asm 000029CF: 	CLRF	R0
    // asm 000029D0: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm 000029D1: 	STF	R0,*+AR5(CARSPEED)
    // asm 000029D2: 	LDI	AR4,AR2
    // asm 000029D3: 	ADDI	OMATRIX,AR2
    // asm 000029D4: 	CALL	FIND_YMATRIX
    // 	;SET CAMERA POSITION
    // 	;
    // 	;
    // asm 000029D5: 	LDF	@START_RADY,R2		;SETUP MATRIX
    // asm 000029D6: 	LDI	@MATRIXAI,AR2
    // asm 000029D7: 	CALL	FIND_YMATRIX
    // asm 000029D8: 	LDI	AR2,R2
    // asm 000029D9: 	CALL	CLR_VECTORA
    // asm 000029DA: 	FLOAT	-20*FEET,R0
    // asm 000029DB: 	STF	R0,*+AR2(Y)
    // asm 000029DC: 	FLOAT	(-20*FEET),R0
    // asm 000029DD: 	STF	R0,*+AR2(Z)
    // asm 000029DE: 	LDI	AR2,R3
    // asm 000029DF: 	CALL	MATRIX_MUL
    // asm 000029E0: 	LDI	@CAMERAPOSI,AR3		;INIT CAMERA POSITION
    // asm 000029E1: 	LDF	*+AR4(OPOSX),R0
    // asm 000029E2: 	ADDF	*+AR2(X),R0
    // asm 000029E3: 	STF	R0,*+AR3(X)		;CAMERA X
    // asm 000029E4: 	LDF	*+AR4(OPOSY),R0
    // asm 000029E5: 	ADDF	*+AR2(Y),R0
    // asm 000029E6: 	STF	R0,*+AR3(Y)		;CAMERA Y
    // asm 000029E7: 	LDF	*+AR4(OPOSZ),R0
    // asm 000029E8: 	ADDF	*+AR2(Z),R0
    // asm 000029E9: 	STF	R0,*+AR3(Z)		;CAMERA Z
    // asm 000029EA: 	CALL	RESCAN	     		;RESET ACTIVE OBJECT LIST
    // asm 000029EB: 	LDI	1,R0
    // asm 000029EC: 	STI	R0,@CAMVIEW		;INIT CAMERA VIEW TO 3RD PERSON
    // asm 000029ED: 	BU	PLYR_INTRO_JOIN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_INTRO_ENTER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
void _PLYR(void) {
PLYR_ENTER:
    // asm 000029EE: 	CALL	OBJ_GET			;INIT PLAYER OBJECT
    // asm 000029EF: 	LDI	AR0,AR4
    // asm 000029F0: 	CALL	PLYR_CAR_INIT
    // asm 000029F1: 	LDF	@START_POS+X,R0
    // asm 000029F2: 	STF	R0,*+AR4(OPOSX)
    // asm 000029F3: 	LDF	@START_POS+Y,R0
    // asm 000029F4: 	STF	R0,*+AR4(OPOSY)
    // asm 000029F5: 	LDF	@START_POS+Z,R0
    // asm 000029F6: 	STF	R0,*+AR4(OPOSZ)
    // asm 000029F7: 	LDI	PLYR_C|PLYR1_T,R0
    // asm 000029F8: 	STI	R0,*+AR7(PID)
    // *INITIALIZE PLAYER COORD, FACING ANGLE
    // asm 000029F9: 	LDI	*+AR4(OCARBLK),AR5
    // asm 000029FA: 	CALL	GETTRAK
    // asm 000029FB: 	LDI	*+AR5(CARTRAK),AR2
    // asm 000029FC: 	LDI	@DYNALIST_TRUEBEGIN,AR2
    // asm 000029FD: 	LDI	*+AR2(OUSR1),R0
    // asm 000029FE: 	LDI	*+AR2(OLINK4),AR2	;SKIP FIRST GROUP
    // ;	ANDN	0FFh,R0
    // ;	ADDI	0100h,R0
    // ;L10	LDI	*+AR2(OLINK4),AR2	;SKIP FIRST GROUP
    // ;	CMPI	*+AR2(OUSR1),R0
    // ;	BGT	L10
    // asm 000029FF: 	LDF	*+AR2(OPOSX),R0
    // asm 00002A00: 	STF	R0,*+AR4(OPOSX)
    // asm 00002A01: 	LDF	*+AR2(OPOSY),R0
    // asm 00002A02: 	STF	R0,*+AR4(OPOSY)
    // asm 00002A03: 	LDF	*+AR2(OPOSZ),R0
    // asm 00002A04: 	STF	R0,*+AR4(OPOSZ)
    // asm 00002A05: 	CALL	GETRDIR			;GET ANGLE OF ROAD
    // *INIT CAMERA
    // asm 00002A06: 	LDF	R0,R2
    // asm 00002A07: 	STF	R2,*+AR5(CARYROT)
    // asm 00002A08: 	STF	R2,*+AR5(CARVROT)
    // asm 00002A09: 	LDI	AR4,AR2
    // asm 00002A0A: 	ADDI	OMATRIX,AR2
    // asm 00002A0B: 	CALL	FIND_YMATRIX
    // asm 00002A0C: 	NEGF	R2
    // asm 00002A0D: 	CALL	NORMITS
    // asm 00002A0E: 	LDP	@_CAMERARAD+Y
    // asm 00002A0F: 	STF	R2,@_CAMERARAD+Y	;UPDATE CAMERA RAD
    // asm 00002A10: 	SETDP
    // asm 00002A11: 	LDI	@CAMERAMATRIXI,AR2
    // asm 00002A12: 	CALL	FIND_YMATRIX
    // *OFFSET THE CAR INTO LANE 1
    // asm 00002A13: 	LDI	AR4,R2
    // asm 00002A14: 	ADDI	OMATRIX,R2
    // asm 00002A15: 	CALL	CLR_VECTORA
    // asm 00002A16: 	FLOAT	10*FEET,R0
    // 	;
    // 	;if vehicle is a slave, then offset into lane #2
    // 	;
    // 	;if a slave then ALWAYS appear on right side
    // 	;
    // 	;
    // asm 00002A17: 	LDI	@DIPRAM,R1
    // asm 00002A18: 	TSTB	DIP_COMMP,R1
    // asm 00002A19: 	BNZ	BABA
    // asm 00002A1A: 	TSTB	CMDP_MASTER,R1
    // asm 00002A1B: 	BZ	BABA
    // asm 00002A1C: 	FLOAT	LANESIZE,R1
    // asm 00002A1D: 	ADDF	R1,R0
BABA:
    // asm 00002A1E: 	STF	R0,*+AR2(X)
    // asm 00002A1F: 	LDI	AR2,R3
    // asm 00002A20: 	CALL	MATRIX_MUL
    // asm 00002A21: 	LDI	R3,AR2
    // asm 00002A22: 	LDF	*+AR4(OPOSX),R0
    // asm 00002A23: 	ADDF	*+AR2(X),R0
    // asm 00002A24: 	STF	R0,*+AR4(OPOSX)
    // asm 00002A25: 	LDF	*+AR4(OPOSY),R0
    // asm 00002A26: 	ADDF	*+AR2(Y),R0
    // asm 00002A27: 	STF	R0,*+AR4(OPOSY)
    // asm 00002A28: 	LDF	*+AR4(OPOSZ),R0
    // asm 00002A29: 	ADDF	*+AR2(Z),R0
    // asm 00002A2A: 	STF	R0,*+AR4(OPOSZ)
    // *SET CAMERA POSITION
    // asm 00002A2B: 	LDI	@CAMERAMATRIXI,AR2
    // asm 00002A2C: 	LDI	AR2,R2
    // asm 00002A2D: 	CALL	CLR_VECTORA
    // asm 00002A2E: 	FLOAT	-20*FEET,R0
    // asm 00002A2F: 	STF	R0,*+AR2(Y)
    // asm 00002A30: 	FLOAT	(-20*FEET),R0
    // asm 00002A31: 	STF	R0,*+AR2(Z)
    // asm 00002A32: 	LDI	AR2,R3
    // asm 00002A33: 	CALL	MATRIX_MUL
    // asm 00002A34: 	LDI	@CAMERAPOSI,AR3		;INIT CAMERA POSITION
    // asm 00002A35: 	LDF	*+AR4(OPOSX),R0
    // asm 00002A36: 	ADDF	*+AR2(X),R0
    // asm 00002A37: 	STF	R0,*+AR3(X)		;CAMERA X
    // asm 00002A38: 	LDF	*+AR4(OPOSY),R0
    // asm 00002A39: 	ADDF	*+AR2(Y),R0
    // asm 00002A3A: 	STF	R0,*+AR3(Y)		;CAMERA Y
    // asm 00002A3B: 	LDF	*+AR4(OPOSZ),R0
    // asm 00002A3C: 	ADDF	*+AR2(Z),R0
    // asm 00002A3D: 	STF	R0,*+AR3(Z)		;CAMERA Z
    // asm 00002A3E: 	CALL	RESCAN	     		;RESET ACTIVE OBJECT LIST
    // *CAMERA INIT
PLYR_INTRO_JOIN:
    // ;	LDF	1.0,R0			;INIT DRAFT VALUE
    // ;	STF	R0,@PLDRAFTVAL
    // asm 00002A3F: 	LDF	0,R0	 		;INITIALIZE PLAYER ZOOM POSITION
    // asm 00002A40:    	STF	R0,@ZOOMDD
    // asm 00002A41:    	STF	R0,@ZOOMHD
    // asm 00002A42: 	FLOAT	PLYPOS2Y,R0
    // asm 00002A43: 	STF	R0,@ZOOMH
    // asm 00002A44: 	STF	R0,@ZOOMHG
    // asm 00002A45: 	FLOAT	PLYPOS2Z,R0
    // asm 00002A46: 	STF	R0,@ZOOMD
    // asm 00002A47: 	STF	R0,@ZOOMDG
    // asm 00002A48: 	LDI	@VIEW1I,AR2
    // asm 00002A49: 	LDI	@CAMVIEW,R0
    // asm 00002A4A: 	LDIEQ	@VIEW0I,AR2
    // asm 00002A4B: 	LDI	1,R1
    // asm 00002A4C: 	STI	R1,@CAMVIEW		;INIT CAMERA VIEW TO 3RD PERSON
    // asm 00002A4D: 	CMPI	2,R0
    // asm 00002A4E: 	LDIEQ	@VIEW2I,AR2
    // asm 00002A4F: 	LDI	UTIL_C,R2  		;RESTORE OLD VIEW
    // asm 00002A50: 	CALL	PRC_CREATE
L883:
    // asm 00002A51: LDI	0,R0	      		;BRAKE INITIALLY OFF
    // asm 00002A52: 	STPI	R0,@BRAKEON
    // asm 00002A53: 	STI	R0,@WRECKFLG		;WRECK OFF
    // asm 00002A54: 	STI	R0,@PLYRFIRST		;TIMER FOR PLAYER IN 1ST PLACE
    // asm 00002A55: 	CALL	_off_brake
    // asm 00002A56: 	LDF	1.0,R0			;INITIALIZE THE CHEAT
    // asm 00002A57: 	STF	R0,@CHEATACC
    // asm 00002A58:  	STF	R0,@CHEAT
    // asm 00002A59: 	LDI	@_MODE,R0
    // asm 00002A5A: 	OR	MHUD,R0
    // asm 00002A5B: 	STI	R0,@_MODE
    // *
    // *PLAYER CAR LOOP
    // *
PLYRLP:
    // asm 00002A5C: LDI	@END_OF_GAMEP,R0
    // asm 00002A5D: 	BNZ	ENDPLAYER
    // asm 00002A5E: 	LDI	@NFRAMES,R2
    // asm 00002A5F: 	NEGI	R2,R1
    // asm 00002A60: 	LDI    	@POSITION,R0  		;TIMER FOR HOW LONG PLAYER IN 1ST
    // asm 00002A61: 	CMPI	1,R0
    // asm 00002A62: 	LDIZ	@PLYRFIRST,R1
    // asm 00002A63: 	ADDI	R2,R1
    // asm 00002A64: 	STI	R1,@PLYRFIRST		;TIMER FOR PLAYER IN 1ST PLACE
    // asm 00002A65: 	LDI	@_countdown,R1		;TIMEOUT?
    // asm 00002A66: 	CALLLE	TIMED_OUT		;SETBACK?
    // asm 00002A67: DONT_TIMEOUT
    // *CHECK FOR BONUS SCREEN
    // *	!!!! DO NOT REMOVE THIS CODE
    // *	!!!! EVEN IF YOU DON'T KNOW WHAT
    // *	!!!! IT DOES
    // asm 00002A67: 	LDI	@_MODE,R0
    // asm 00002A68: 	AND	MMODE,R0
    // asm 00002A69: 	CMPI	MBONUS,R0
    // asm 00002A6A: 	BEQ	BONUS_WAIT_LOOP
    // asm 00002A6B: 	LDI	@PLYCAR,AR4		;GET PLAYER CAR OBJECT
    // asm 00002A6C: 	LDI	*+AR4(OCARBLK),AR5
    // asm 00002A6D: PLYRSPD00
    // asm 00002A6D: 	LDI	@WRECKFLG,R0		;WRECK?
    // asm 00002A6E: 	BZ	PLYRSPD
    // asm 00002A6F: 	CALL	WRECK			;DO YOUR WRECK THING
    // asm 00002A70: 	CALL	INBOUNDZ		;KEEP IN BOUNDS
    // asm 00002A71: 	B	PLYRCAM			;DONT CHANGE MATRIX OR CAMERA POS
PLYRSPD:
    // asm 00002A72: 	CALL	CKOFRD			;CHECK YOUR OFFROAD COUNTER
    // asm 00002A73: 	CMPI	0,R0
    // asm 00002A74: 	CALLZ	PLYONRD			;RESET PLAYER ON ROAD
    // asm 00002A75: 	LDF	*+AR4(OMAT11),R0	;IF PLAYER FLIPPED RESET 'EM
    // asm 00002A76: 	CMPF	0.01,R0
    // asm 00002A77: 	CALLLT	PLYONRD
    // *GET CAR SPEED
    // asm 00002A78: 	CALL	GETGEAR			;GET CAR GEAR
    // asm 00002A79: 	LDI	*+AR5(CARGEAR),R1
    // asm 00002A7A: 	STI	R0,*+AR5(CARGEAR)
    // asm 00002A7B: 	CMPI	R0,R1			;CHECK FOR UPSHIFT
    // asm 00002A7C: 	BGE	PLYRSPD0		;DOWNSHIFT OR NO SHIFT
    // ;	CMPI	4,R0			;4TH GEAR?
    // ;	BZ	PLYRSPD0		;YES, NO RUBBER
    // asm 00002A7D: 	CALL	GETPEDAL		;GET GAS PEDAL VALUE
    // asm 00002A7E: 	MPYF	2,R0			;GIVE A LITTLE JOLT
    // asm 00002A7F: 	CMPF	1.4,R0			;CHECK THROTTLE
    // asm 00002A80: 	BLE 	PLYRSPD01		;NOT ENOUGH
    // asm 00002A81: 	PUSHF	R0
    // asm 00002A82: 	SONDFX	UPSHIFTSND		;MAKE YOUR UPSHIFT DUDES
    // asm 00002A84: 	CREATEC	FLAME_PRC,UTIL_C	;make child flames
    // asm 00002A87: 	CREATEC	SMOKE_PROC,UTIL_C	;make child smoke
    // ;	LDI	*+AR5(CARGEAR),AR2	;MAKE RIGHT REV ON SHIFT
    // ;	ADDI	@SHIFTSNDTABI,AR2
    // ;	LDI	*-AR2(1),AR2
    // ;	CALL	ONESND
    // asm 00002A8A: 	POPF	R0
    // asm 00002A8B: 	B	PLYRSPD01
PLYRSPD0:
    // asm 00002A8C: 	CALL	GETPEDAL		;GET GAS PEDAL VALUE
PLYRSPD01:
    // ;	NEGF	*+AR5(CARTHROTTLE),R1
    // ;	ADDF	R0,R1
    // ;	CMPF	0.3,R1
    // ;	BLT	NO_PEDAL_FLAME
    // ;	PUSHF	R0
    // ;	CREATEC	FLAME_PRC,UTIL_C	;make child flames
    // ;	POPF	R0
    // ;NO_PEDAL_FLAME
    // asm 00002A8D: 	LDI	@_MODE,R1
    // asm 00002A8E: 	TSTB	MGO,R1
    // asm 00002A8F: 	LDFZ	0,R0			;YIP, NO THROTTLE
    // asm 00002A90: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm 00002A91: 	CALL	GETBRAKE		;GET YOUR BRAKE, DUDES...
    // asm 00002A92: 	STF	R0,*+AR5(CARBRAKE)   	;STORE IT
    // ;	LDF	@PLDRAFTVAL,R0		;SET DRAFTING VALUE
    // ;	STF	R0,@DRAFTVAL
    // asm 00002A93: 	CALL	GETSPD
    // asm 00002A94: 	LDF	*+AR5(CARSPEED),R0 	;OUTPUT IN MPH
    // asm 00002A95: 	LDI	@_countdown,R1
    // asm 00002A96: 	BGT	NOTGO
    // asm 00002A97: 	LDI	SM_GO,R1		;we have to take care of the case when the plyr
    // asm 00002A98: 	STI	R1,@SUSPEND_MODE	;just 'rolls' into the checkpoint
NOTGO:
    // asm 00002A99: 	MPYF	MPH_CONVERSION,R0
    // asm 00002A9A: 	FIX	R0
    // asm 00002A9B: 	STI	R0,@_MPH
    // *GET YOUR SKID FACTOR
    // asm 00002A9C: 	CALL 	GETSKID
    // asm 00002A9D: 	LDF	*+AR5(CARSKID),R0
    // *DO YOUR TRACTION CHEAT...
    // asm 00002A9E: 	LDI	@HEAD2HEAD_ON,R1	;LINK?
    // asm 00002A9F: 	BZ	GSKD1			;FORGET IT, NOT HEAD2HEAD
    // asm 00002AA0: 	LDI	*+AR5(CAR_SPIN),R1  	;FULL SKID ON SPIN-OUT
    // asm 00002AA1: 	BNZ	GSKD1
    // asm 00002AA2:        	LDF	2.00,R1
    // asm 00002AA3: 	SUBF	@CHEAT,R1
    // asm 00002AA4: 	MPYF	R1,R0
    // asm 00002AA5: 	STF	R0,*+AR5(CARSKID)	;CHEAT SKID VALUE
GSKD1:
    // asm 00002AA6: 	MPYF	R0,R0
    // asm 00002AA7: 	CMPF	1.0,R0
    // asm 00002AA8: 	LDFGT	1.0,R0
    // asm 00002AA9: 	SUBRF	1,R0
    // asm 00002AAA: 	CMPF	0.333,R0
    // asm 00002AAB: 	LDFLT	0.333,R0
    // asm 00002AAC: 	MPYF	2.5,R0			;STEERING WHEEL POWER
    // asm 00002AAD: 	STF	R0,@WHEELPWR
    // *GET CAR DIRECTION DELTA RADIANS
    // asm 00002AAE: 	CALL	GETTRAK			;GET CLOSEST TRAKC SECTION
    // asm 00002AAF: 	CALL	GETSTEER		;RET R0=STEERING DELTA RADIANS
    // asm 00002AB0: 	CALL	GETDIR			;ADJUST DIRECTION BASED ON SKID
    // asm 00002AB1: 	PUSHF	R0
    // asm 00002AB2: 	CALL	INBOUNDZ		;KEEP IN BOUNDS
    // asm 00002AB3: 	CALL	BACKCK			;KEEP IN RIGHT DIRECTION
    // asm 00002AB4: 	POPF	R0
    // *GET INCREMENTAL ROTATION MATRIX
    // asm 00002AB5: 	LDF	R0,R2
    // asm 00002AB6: 	ADDF	*+AR5(CARROT),R2    	;GET RID OF OLD OVERROTATION
    // asm 00002AB7: 	LDI	@MATRIXAI,AR2
    // asm 00002AB8: 	CALL	FIND_YMATRIX
    // *FORM NEW ROTATION MATRIX
    // asm 00002AB9: 	LDI	AR4,R2
    // asm 00002ABA: 	ADDI	OMATRIX,R2
    // asm 00002ABB: 	LDI	R2,R3
    // asm 00002ABC: 	CALL	CONCATMAT
    // *FORM NEW VELOCITY MATRIX
    // asm 00002ABD: 	LDF	*+AR5(CARVROT),R2    	;GET VELOCITY MATRIX
    // asm 00002ABE: 	SUBF	*+AR5(CARYROT),R2
    // asm 00002ABF: 	LDI	@MATRIXBI,AR2
    // asm 00002AC0: 	CALL	FIND_YMATRIX
    // asm 00002AC1: 	LDI	AR4,R2
    // asm 00002AC2: 	ADDI	OMATRIX,R2
    // asm 00002AC3: 	LDI	AR2,R3
    // asm 00002AC4: 	CALL	CONCATMAT
    // asm 00002AC5: 	LDF  	*+AR5(CARDIST),R2	;GET DISTANCE
    // asm 00002AC6: 	MPYF	@CHEAT,R2		;DO THE CHEAT THING !!!
    // asm 00002AC7: 	LDI	@MATRIXBI,AR2
    // asm 00002AC8: 	LDI	*+AR5(CAR_AIRB),R0
    // asm 00002AC9: 	BNZ	PAIRB	    		;WERE FLYING
    // *MOVE CAR FORWARD
    // asm 00002ACA: 	LDI	AR4,R3
    // asm 00002ACB: 	ADDI	OVELX,R3
    // asm 00002ACC: 	CALL	FORWARD
PAIRB:
    // asm 00002ACD: 	CALL	OVELADD
    // *GET ROAD MATRIX
    // asm 00002ACE: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm 00002ACF: 	CALL	CAR_ROAD_COLL
    // *GET NEW CAR MATRIX
    // asm 00002AD0: 	LDF	*+AR5(CARYROT),R2
    // asm 00002AD1: 	STF	R2,*+AR4(ORADY)		;STORE CAR OBJECT RADY
    // asm 00002AD2: 	LDI	@MATRIXAI,AR2
    // asm 00002AD3: 	CALL	FIND_YMATRIX
    // asm 00002AD4: 	LDI	AR4,R2
    // asm 00002AD5: 	ADDI	OMATRIX,R2
    // asm 00002AD6: 	LDI	R2,R3
    // asm 00002AD7: 	CALL	CONCATMAT
    // *GET CAMERA ORIENTATION MATRIX
PLYRCAM:
    // *DO YOUR ZOOM JIVE
    // asm 00002AD8: 	CALL	ZOOMUP			;UPDATE YOUR ZOOM
    // asm 00002AD9: 	LDI	@CAMVIEW,R0
    // asm 00002ADA: 	BNZ	CAM3RD
    // *FIRST PERSON CAMERA
    // asm 00002ADB: CAM1ST
    // ****************************
    // asm 00002ADB: 	CALL	CAMMATSAV
    // ****************************
    // asm 00002ADC: 	LDI	@CAMERAMATRIXI,AR2
    // asm 00002ADD: 	NEGF	*+AR5(CARYROT),R2 	;FOLLOW CAR ROTATION
    // asm 00002ADE: 	LDP	@_CAMERARAD+Y
    // asm 00002ADF: 	STF	R2,@_CAMERARAD+Y	;UPDATE CAMERA RAD
    // asm 00002AE0: 	SETDP				;RESTORE DP TO ZERO PAGE
    // asm 00002AE1: 	LDI	AR4,R2
    // asm 00002AE2: 	ADDI	OMATRIX,R2
    // asm 00002AE3: 	CALL	CPYIMAT
    // asm 00002AE4: 	LDF	0,R0
    // asm 00002AE5: 	STF	R0,*+AR5(CARROT)	;NO OVERROTATION IN CORNER
    // asm 00002AE6: 	LDF	*+AR5(CARXLEAN),R2  	;GET X LEAN FACTOR
    // asm 00002AE7: 	MPYF	-0.7,R2			;NEGATE AND SCALE
    // asm 00002AE8: 	LDP	@_CAMERARAD+X
    // asm 00002AE9: 	STF	R2,@_CAMERARAD+X
    // asm 00002AEA: 	SETDP
    // asm 00002AEB: 	LDI	@MATRIXBI,AR2
    // asm 00002AEC: 	CALL	FIND_XMATRIX
    // asm 00002AED: 	LDI	AR2,AR0
    // asm 00002AEE: 	LDI	@MATRIXCI,AR1
    // asm 00002AEF: 	PUSH	AR1
    // asm 00002AF0: 	LDF	*+AR5(CARZLEAN),R2  	;GET Z LEAN FACTOR
    // asm 00002AF1: 	MPYF	-0.5,R2			;NEGATE AND SCALE
    // asm 00002AF2: 	LDI	@MATRIXAI,AR2
    // asm 00002AF3: 	CALL	FIND_ZMATRIX
    // asm 00002AF4: 	CALL	CONCAT201    		;CONCAT YOUR MATRICES
    // asm 00002AF5: 	POP	R2		  	;GET MATRIXC POINTER
    // asm 00002AF6: 	LDI	@CAMERAMATRIXI,AR2	;GET SOURCE MATRIX
    // asm 00002AF7: 	LDI	AR2,R3
    // asm 00002AF8: 	CALL	CONCATMAT
    // ****************************
    // asm 00002AF9: 	CALL	CAMMATAVG
    // *************************
    // asm 00002AFA: 	BR	CAM3RDX
    // 	;
    // 	;THIRD PERSON CAMERA
    // 	;
CAM3RD:
    // asm 00002AFB: 	CLRF	R2
    // asm 00002AFC: 	LDP	@_CAMERARAD+X
    // asm 00002AFD: 	STF	R2,@_CAMERARAD+X
    // asm 00002AFE: 	SETDP
    // asm 00002AFF: 	ABSF	*+AR4(OVELX),R0		;DONT CHANGE CAMERA DIR FOR SMALL VEL
    // asm 00002B00: 	ABSF	*+AR4(OVELZ),R1
    // asm 00002B01: 	ADDF	R0,R1
    // asm 00002B02: 	CMPF	2,R1
    // asm 00002B03: 	BLT	CAM3RD0
    // asm 00002B04: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm 00002B05: 	BNZ	CAM3RD0			;YES, DONT mess WITH CAMERA ANGLE
    // asm 00002B06: 	LDF	*+AR4(OVELX),R3
    // asm 00002B07: 	LDF	*+AR4(OVELZ),R2
    // asm 00002B08: 	CALL	ARCTANF
    // *MAX CAMERA ANGLE CHANGE
    // asm 00002B09: 	LDP	@_CAMERARAD+Y
    // asm 00002B0A: 	SUBF	@_CAMERARAD+Y,R0	;GET OLD CAMERA
    // asm 00002B0B: 	LDF	0,R1 			;NORMALIZE DIFFERENCE
    // asm 00002B0C: 	CMPF	3.14,R0
    // asm 00002B0D: 	LDFGT	-6.28,R1
    // asm 00002B0E: 	CMPF	-3.14,R0
    // asm 00002B0F: 	LDFLT	6.28,R1
    // asm 00002B10: 	ADDF	R1,R0
    // asm 00002B11: 	MPYF	0.20,R0	    		;ANGLE SMOOTHING
    // asm 00002B12: 	ADDF	@_CAMERARAD+Y,R0
    // asm 00002B13: 	SETDP
    // asm 00002B14: 	CALL	CAMCHK			;CHECK OUT NEW VALUE
    // asm 00002B15: 	BZ	CAMOK			;ITS O.K...
    // *WE'RE OFF
    // asm 00002B16: 	LDF	R0,R4	      		;SAVE NEW VALUE
    // asm 00002B17: 	CALL 	CAMROT			;GET ADJUSTED ANGLE VALUE
    // asm 00002B18: 	LDF	R0,R5			;SAVE NEW ADJUSTED VALUE
    // asm 00002B19: 	LDP	@_CAMERARAD+Y,R6	;GET OLD VALUE
    // asm 00002B1A: 	LDF	@_CAMERARAD+Y,R6	;GET OLD VALUE
    // asm 00002B1B: 	SETDP
    // asm 00002B1C: 	SUBF	R6,R5,R2
    // asm 00002B1D: 	CALL	NORMITS
    // asm 00002B1E: 	ABSF	R2,R3
    // asm 00002B1F: 	SUBF	R4,R5,R2
    // asm 00002B20: 	CALL	NORMITS
    // asm 00002B21: 	ABSF	R2
    // asm 00002B22: 	CMPF	R2,R3
    // asm 00002B23: 	BLT	CAMBAD1
    // asm 00002B24: 	LDF	R5,R0	      		;NEW ONE IS CLOSER, GO WITH CORRECTION
    // asm 00002B25: 	B	CAMOK
CAMBAD1:
CAM3RD0:
    // asm 00002B26: 	LDP	@_CAMERARAD+Y
    // asm 00002B27: 	LDF	@_CAMERARAD+Y,R0	;UPDATE CAMERA RAD
    // asm 00002B28: 	SETDP
    // asm 00002B29: 	CALL	CAMCHK			;CHECK IF OLDIE IS ON...
    // asm 00002B2A: 	BZ	CAMOK			;YES... KEEP IT
    // asm 00002B2B: 	CALL	CAMROT			;NO...GET CORRECTION
CAMOK:
    // asm 00002B2C: 	LDP	@_CAMERARAD+Y
    // asm 00002B2D: 	STF	R0,@_CAMERARAD+Y	;UPDATE CAMERA RAD
    // asm 00002B2E: 	SETDP
    // asm 00002B2F: 	LDI	@CAMERAMATRIXI,AR2	;GET SOURCE MATRIX
    // asm 00002B30: 	LDF	R0,R2
    // asm 00002B31: 	CALL	FIND_YMATRIX 		;LOAD CAMERA MATRIX
    // asm 00002B32: 	CALL 	GETCARROT   		;GET CAR OVERROTATE
    // asm 00002B33: 	NEGF	R1,R2
    // asm 00002B34: 	LDI	@MATRIXAI,AR2
    // asm 00002B35: 	CALL	FIND_YMATRIX 		;GET CARROT MATRIX
    // asm 00002B36: 	LDI	AR4,R2	  		;MULT INTO OBJ MATRIX
    // asm 00002B37: 	ADDI	OMATRIX,R2
    // asm 00002B38: 	LDI	R2,R3
    // asm 00002B39: 	CALL	CONCATMAT
CAM3RDX:
    // 	;
    // 	;GET NEW CAMERA POSITION
    // 	;GET NEW CAMERA OFFSET FROM CAR
    // 	;
    // asm 00002B3A: 	LDI	@CAMERAMATRIXI,AR2	;GET SOURCE MATRIX
    // asm 00002B3B: 	NEGF	@ZOOMD,R2		;GET ZOOM DISTANCE
    // asm 00002B3C: 	LDI	@VECTORAI,R3
    // asm 00002B3D: 	LDI	R3,AR3
    // asm 00002B3E: 	CALL	FORWARD
    // 	;
    // 	;ADD IT IN TO CAMERAPOS
    // 	;
    // asm 00002B3F: 	LDF	*+AR4(OPOSX),R0
    // asm 00002B40: 	SUBF	*AR3,R0		      	;INVERT X FOR SOME REASON
    // asm 00002B41: 	LDI	@CAMERAPOSI,AR0
    // asm 00002B42: 	STF	R0,*AR0			;X COORD
    // asm 00002B43: 	LDF	0,R0			;ADJUST WRECK HEIGHT
    // asm 00002B44: 	LDI	@WRECKFLG,R1
    // asm 00002B45: 	LDFNZ	*+AR5(CT_PRDYD),R0
    // asm 00002B46: 	MPYF	0.6,R0
    // asm 00002B47: 	ADDF	@ZOOMH,R0
    // ;	LDF	@ZOOMH,R0
    // asm 00002B48: 	ADDF	*+AR4(OPOSY),R0
    // asm 00002B49: 	ADDF	*+AR3(1),R0
    // asm 00002B4A: 	STF	R0,*+AR0(1)		;Y COORD
    // asm 00002B4B: 	LDF	*+AR4(OPOSZ),R0
    // asm 00002B4C: 	ADDF	*+AR3(2),R0
    // asm 00002B4D: 	STF	R0,*+AR0(2)		;Z COORD
    // asm 00002B4E: 	LDI	@CAMVIEW,R0
    // asm 00002B4F: 	BZ	PLYS1
    // asm 00002B50: 	CALL	CAMYADJ			;YES, ADJUST CAMERA Y ABOVE ROAD
PLYS1:
    // asm 00002B51: 	CALL	GETREV			;GET YOUR RPM'S, MAKE SOUND
    // asm 00002B52: 	CALL	PLYR_SNDS		;HANDLE SOME PLYR SOUNDS
    // asm 00002B53: 	CALL	PLMOTION
    // asm 00002B54: 	CALL	PLYRWHL
    // asm 00002B55: 	CALL	PLYR_RIDE_RIGHT		;FIND DISTANCE TO CENTER OF ROAD
    // asm 00002B56: 	STF	R0,*+AR5(CARDIST2CNTR)
    // asm 00002B57: PLYSLP
    // asm 00002B57: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00002B58: 	BZ	NOPLINK
    // asm 00002B59: 	LDI	*+AR5(CARTRAK),AR0
    // asm 00002B5A: 	LDI	*+AR0(OUSR1),R0			;read road ID
    // asm 00002B5B: 	STI	R0,*+AR5(CARTRACK_ID)		;SAVE TRACK ID
    // asm 00002B5C: 	CALL	SEND_PLAYERS_POS
    // asm 00002B5D: 	CALL	CHEATCK
NOPLINK:
    // asm 00002B5E: 	SLEEP	1
    // asm 00002B60: 	B	PLYRLP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_PLYR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *LIMIT ADVANTAGE AT CLOSE DISTANCE

// *JARV END CHANGE

/* asm: CATCHUP	.FLOAT	0.0001 */
static float CATCHUP = 0.0001f;
/* asm: AHEAD	.FLOAT	-0.0008 */
static float AHEAD = -0.0008f;
/* asm: DISTCON	.FLOAT	0.000001 */
static float DISTCON = 0.000001f;
/* asm: SPDCON	.FLOAT	0.00333 */
/* asm: 	 */
/* asm: 	 */
static float SPDCON = 0.00333f;

/*
 *----------------------------------------------------------------------------
 *CAMERA CHECK
 *
 *PARAMETERS
 *	R0	CAMERA ANGLE
 *	AR0	CAMERA POSITION XYZ POINTER
 *	AR4	PLAYER CAR
 *RETURNS
 *	CARRY SET	COLLIDE WITH ROAD
 */
static void CAMCHKL(void) {
    // asm 00002B93: 	FLOAT	-170,R1	    		;GET LEFT CORNER DIST
    // asm 00002B94: 	B	CAMCHK0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMCHKL", 0, 0);
    UNIMPL();
}

static void CAMCHKR(void) {
    // asm 00002B95: 	FLOAT	170,R1	    		;GET RIGHT CORNER DIST
CAMCHK0:
    // asm 00002B96: 	PUSHF	R0
    // asm 00002B97: 	PUSH	R3
    // asm 00002B98: 	PUSH	AR0
    // asm 00002B99: 	PUSH	AR4
    // asm 00002B9A:  	LDI	AR0,AR4
    // *CHECK RIGHT CORNER
    // *ADJUST CAMERA X,Z
    // asm 00002B9B: 	LDI	@VECTORAI,AR0  		;GET TEMP VECTOR, CORNER COORD STORE
    // asm 00002B9C: 	LDF	R0,R2			;GET CAMERA ANGLE
    // asm 00002B9D: 	CALL	_SINE
    // asm 00002B9E: 	NEGF	R0,R3
    // asm 00002B9F: 	CALL	_COSI
    // asm 00002BA0: 	MPYF	R1,R0
    // asm 00002BA1: 	MPYF	R1,R3
    // asm 00002BA2: 	ADDF	*+AR4(0),R0	 	;GET X,Y,Z COORDS
    // asm 00002BA3: 	STF	R0,*AR0
    // asm 00002BA4: 	LDF	*+AR4(1),R0
    // asm 00002BA5: 	STF	R0,*+AR0(1)
    // asm 00002BA6: 	ADDF	*+AR4(2),R3
    // asm 00002BA7: 	STF	R3,*+AR0(2)
    // asm 00002BA8: 	LDI	AR0,AR4			;POINT TO XYZ OF CAMERA
    // asm 00002BA9: 	CALL	CAMSCAN
    // asm 00002BAA: CAMCHKRX
    // asm 00002BAA: 	POP	AR4
    // asm 00002BAB: 	POP	AR0
    // asm 00002BAC: 	POP	R3
    // asm 00002BAD: 	POPF	R0
    // asm 00002BAE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMCHKR", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CAMCHKLR
 *PARAMETERS
 *	AR0	CAMERA POSITION XYZ POINTER
 *	AR4	PLAYER CAR
 *RETURNS
 *	R3=0 CAMERA O.K.,
 *	R0=1 LEFT OFF,
 *	R0=2 RIGHT OFF,
 *	R0=3 BOTH OFF
 *RETURNS
 *	Z=1 CAMERA O.K.
 */
static void CAMCHKLR(void) {
    // asm 00002BAF: 	CALL	CAMCHKL
    // asm 00002BB0: 	LDINC	1,R3
    // asm 00002BB1: 	LDIC	0,R3
    // asm 00002BB2: 	CALL	CAMCHKR
    // asm 00002BB3: 	LDINC	2,R1
    // asm 00002BB4: 	LDIC	0,R1
    // asm 00002BB5: 	ADDI	R1,R3
    // asm 00002BB6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMCHKLR", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CAMERA	CHECKER
 *PARAMETERS
 *	R0	ANGLE
 *	AR4	PLAYER CAR
 *	AR5	CAR BLOCK
 *RETURNS
 *	R3=0 CAMERA O.K.
 *	R0=1 LEFT OFF,
 *	R0=2 RIGHT OFF,
 *	R0=3 BOTH OFF
 *RETURNS
 *	Z=1 CAMERA O.K.
 */
static void CAMCHK(void) {
    // asm 00002BB7: 	PUSH	AR0
    // asm 00002BB8: 	CALL	GETCAMPOS
    // asm 00002BB9: 	LDI	AR3,AR0
    // asm 00002BBA: 	CALL	CAMCHKLR
    // asm 00002BBB: 	POP	AR0
    // asm 00002BBC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMCHK", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CAMERA	ROTATER
 *PARAMETERS
 *	R0	CURRENT ANGLE
 *	AR4	PLAYER CAR
 *	AR5	CAR BLOCK
 *RETURNS
 *	R0	ADJUSTED ANGLE
 */
static void CAMROT(void) {
    // asm 00002BBD: PUSH	AR0
    // asm 00002BBE:  	PUSH	AR2
    // asm 00002BBF: 	LDF	R0,R3
    // asm 00002BC0: 	CALL	ROADIR			;R0=ROADIR
    // asm 00002BC1: 	ADDF	R0,R3,R2
    // asm 00002BC2: 	CALL	NORMITS	      		;FIND DIRECTION
    // asm 00002BC3: 	ABSF	R2,R1		      	;STOP OSCILLATION AROUND
    // asm 00002BC4: 	CMPF	0.1,R1
    // asm 00002BC5: 	BGT	CAMROT1
    // asm 00002BC6: 	NEGF	R2,R1
    // asm 00002BC7: 	B 	CAMROT2
CAMROT1:
    // asm 00002BC8: 	LDF	R2,R2
    // asm 00002BC9: 	LDFN	0.1,R1
    // asm 00002BCA: 	LDFNN	-0.1,R1
CAMROT2:
    // asm 00002BCB: 	ADDF	R1,R3,R0		;RETURN R0=ADJUSTED ANGLE
    // asm 00002BCC:  	POP	AR2
    // asm 00002BCD:  	POP	AR0
    // asm 00002BCE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMROT", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *GET NEW CAMERA POSITION
 *PARAMETERS
 *	R0	CAMERARAD
 *	AR4	PLAYER CAR
 *RETURNS
 *	AR3=VECTORA=CAMERAPOS X,Y,Z
 */
static void GETCAMPOS(void) {
    // asm 00002BCF: 	PUSHF	R0
    // asm 00002BD0: 	PUSH	AR2
    // asm 00002BD1: 	LDI	@MATRIXAI,AR2		;GET SOURCE MATRIX
    // asm 00002BD2: 	LDF	R0,R2
    // asm 00002BD3: 	CALL	FIND_YMATRIX 		;LOAD CAMERA MATRIX
    // asm 00002BD4: 	NEGF	@ZOOMD,R2		;GET ZOOM DISTANCE
    // asm 00002BD5: 	LDI	@VECTORAI,R3
    // asm 00002BD6: 	LDI	R3,AR3
    // asm 00002BD7: 	CALL	FORWARD
    // asm 00002BD8: 	LDF	*+AR4(OPOSX),R0		;ADD IN PLAYER CAR POSITION
    // asm 00002BD9: 	SUBF	*AR3,R0		      	;INVERT X FOR SOME REASON
    // asm 00002BDA: 	STF	R0,*AR3			;X COORD
    // asm 00002BDB: 	LDF	@ZOOMH,R0
    // asm 00002BDC: 	ADDF	*+AR4(OPOSY),R0
    // asm 00002BDD: 	ADDF	*+AR3(1),R0
    // asm 00002BDE: 	STF	R0,*+AR3(1)		;Y COORD
    // asm 00002BDF: 	LDF	*+AR4(OPOSZ),R0
    // asm 00002BE0: 	ADDF	*+AR3(2),R0
    // asm 00002BE1: 	STF	R0,*+AR3(2)		;Z COORD
    // asm 00002BE2: 	POP	AR2
    // asm 00002BE3: 	POPF	R0
    // asm 00002BE4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCAMPOS", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CAMERA ADJUST - KEEP IT OUT OF THE GROUND AND BUILDINGS
 *PARAMETERS
 *	AR0	CAMERA POSITION XYZ POINTER
 *	AR4	PLAYER CAR
 */
void CAMYADJ(void) {
    // *ADJUST CAMERA Y
    // asm 00002BE5: 	PUSH	AR4
    // asm 00002BE6: 	LDI	AR0,AR4			;POINT TO XYZ OF CAMERA
    // asm 00002BE7: 	CALL	CAMSCAN
    // asm 00002BE8: 	LDI	AR4,AR0
    // asm 00002BE9: 	POP	AR4
    // asm 00002BEA: 	BNC	CAM1XX			;NO COLLISION, SKIP IT...
    // asm 00002BEB: 	FLOAT	133,R1
    // asm 00002BEC: 	SUBF	R0,R1
    // asm 00002BED: 	BLE	CAM1XX
    // asm 00002BEE: 	FLOAT	500,R0		 	;OVERPASS BYPASS
    // asm 00002BEF: 	CMPF	R0,R1
    // asm 00002BF0: 	BGT	CAM1XX
    // asm 00002BF1: 	NEGF	R1
    // asm 00002BF2: 	ADDF	*+AR0(1),R1
    // asm 00002BF3: 	STF	R1,*+AR0(1)		;CAMERA Y COORD
CAM1XX:
    // asm 00002BF4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMYADJ", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *PUT PLAYER ON ROAD
 *PARAMETERS
 *	AR4	PLAYER OBJECT
 *	AR5	PLAYER CAR
 */
static void PLYONRD(void) {
    // asm 00002BF5: 	LDF	0,R0
    // asm 00002BF6: 	STF	R0,*+AR5(CARSPRAD)
    // asm 00002BF7: 	STF	R0,*+AR5(CARDROT)
    // asm 00002BF8: 	LDI	0,R0
    // asm 00002BF9: 	STI	R0,*+AR5(CAR_SPIN)	;RESET SPIN FLAG
    // asm 00002BFA: 	STPI	R0,@WRECKFLG		;WRECK OFF
    // asm 00002BFB: 	LDI	AR4,AR0			;GET MATRIX BACK TO NORMAL
    // asm 00002BFC: 	ADDI	OMATRIX,AR0
    // asm 00002BFD: 	CALL	INITMAT
    // asm 00002BFE: 	CALL    GETTRAK			;GET CLOSEST TRACK SEGMENT
    // asm 00002BFF: 	LDF	*+AR0(OPOSX),R0		;NEW POSITION
    // asm 00002C00: 	STF	R0,*+AR4(OPOSX)
    // asm 00002C01: 	LDF	*+AR0(OPOSZ),R0
    // asm 00002C02: 	STF	R0,*+AR4(OPOSZ)
    // asm 00002C03: 	LDF	*+AR0(OPOSY),R0
    // asm 00002C04: 	STF	R0,*+AR4(OPOSY)
    // asm 00002C05: 	CALL  	ROADIR			;GET DIRECTION IN R0
    // asm 00002C06: 	STF	R0,*+AR5(CARYROT)
    // asm 00002C07: 	STF	R0,*+AR5(CARVROT)
    // asm 00002C08: 	LDF	R0,R2
    // asm 00002C09: 	NEGF	R0
    // asm 00002C0A: 	LDP	@_CAMERARAD+Y
    // asm 00002C0B: 	STF	R0,@_CAMERARAD+Y	;UPDATE CAMERA YRAD
    // asm 00002C0C: 	SETDP
    // asm 00002C0D: 	CALL	_SINE
    // asm 00002C0E: 	LDF	R0,R3
    // asm 00002C0F: 	CALL	_COSI
    // asm 00002C10: 	FLOAT	722,R1	    		;GET LANE DIST
    // asm 00002C11: 	MPYF	R1,R0
    // asm 00002C12: 	MPYF	R1,R3
    // asm 00002C13: 	ADDF	*+AR4(OPOSX),R0
    // asm 00002C14: 	STF	R0,*+AR4(OPOSX)
    // asm 00002C15: 	ADDF	*+AR4(OPOSZ),R3
    // asm 00002C16: 	STF	R3,*+AR4(OPOSZ)
    // asm 00002C17: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYONRD", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *DRONE LOOP
 *PARAMETERS
 *	R2	CURRENT STEERING DELTA ANGLE
 *	AR4	CAR OBJECT
 *	AR5	CAR DATA BLOCK
 *		CARBLK STUFF TO LOAD
 *		CARTURN      	;ANGLE OF FRONT WHEELS 		(FLOAT)
 *		CARTRACTION  	;TRACTION COEFFICIENT OF TIRES	(FLOAT)
 *		CARMAXACCEL  	;MAXIMUM ACCEL (PIX/16 MSEC)	(FLOAT)
 *		CARTHROTTLE  	;THROTTLE VALUE 0-1.0 		(FLOAT)
 *		CARBRAKE	;BRAKING FRICTION (0-1.0)	(FLOAT)
 *
 */
void DRONEGO(void) {
    // asm 00002C18: 	PUSHF	R2
    // asm 00002C19: 	CALL	GETAUTO			;GET AUTO TRANS VALUE
    // asm 00002C1A: 	STI	R0,*+AR5(CARGEAR) 	;DO THE GEAR
    // ;	LDF	1.0,R0
    // ;	STF	R0,@DRAFTVAL		;NO DRAFTING FOR DRONES...
    // asm 00002C1B: 	CALL	GETSPD
    // asm 00002C1C: 	CALL 	GETSKID
    // asm 00002C1D: 	CALL	GETRPM			;GET YOUR REVS
    // *GET CAR DIRECTION DELTA RADIANS
    // asm 00002C1E: 	POPF	R0			;GET STEERIN ANGLE
    // asm 00002C1F: 	CALL	GETDIR
    // asm 00002C20: 	PUSHF	R0
    // asm 00002C21: 	CALL	DRONINBZ
    // asm 00002C22: 	POPF	R2
    // *GET INCREMENTAL ROTATION MATRIX
    // asm 00002C23: 	LDI	@MATRIXAI,AR2
    // asm 00002C24: 	CALL	FIND_YMATRIX
    // *FORM NEW ROTATION MATRIX
    // asm 00002C25: 	LDI	AR4,R2
    // asm 00002C26: 	ADDI	OMATRIX,R2
    // asm 00002C27: 	LDI	R2,R3
    // asm 00002C28: 	CALL	CONCATMAT
    // *FORM NEW VELOCITY MATRIX
    // asm 00002C29: 	LDF	*+AR5(CARVROT),R2    	;GET VELOCITY MATRIX
    // asm 00002C2A: 	SUBF	*+AR5(CARYROT),R2
    // asm 00002C2B: 	LDI	@MATRIXBI,AR2
    // asm 00002C2C: 	CALL	FIND_YMATRIX
    // asm 00002C2D: 	LDI	AR4,R2
    // asm 00002C2E: 	ADDI	OMATRIX,R2
    // asm 00002C2F: 	LDI	AR2,R3
    // asm 00002C30: 	CALL	CONCATMAT
    // asm 00002C31: 	LDF  	*+AR5(CARDIST),R2	;GET DISTANCE
    // asm 00002C32: 	LDI	@MATRIXBI,AR2
    // asm 00002C33: 	LDI	*+AR5(CAR_AIRB),R0
    // asm 00002C34: 	BNZ	DAIRB	    		;WERE FLYING
    // *MOVE CAR FORWARD
    // asm 00002C35: 	LDI	AR4,R3
    // asm 00002C36: 	ADDI	OVELX,R3
    // asm 00002C37: 	CALL	FORWARD
DAIRB:
    // asm 00002C38: 	CALL	OVELADD
    // *GET ROAD MATRIX
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONEGO", 0, 0);
    UNIMPL();
}

void DRONESTOP(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/) {
    // asm 00002C39: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    (void)carblk;
    // asm 00002C3A: 	CALL	CAR_ROAD_COLL
    CAR_ROAD_COLL(obj, carblk);
    // *GET CAR MATRIX
    // asm 00002C3B: 	LDF	*+AR5(CARYROT),R2
    // asm 00002C3C: 	STF	R2,*+AR4(ORADY)		;STORE CAR OBJECT RADY
    obj->rad.Y = carblk->y_rotation;
    // asm 00002C3D: 	LDI	@MATRIXAI,AR2
    // asm 00002C3E: 	CALL	FIND_YMATRIX
    FIND_YMATRIX(&_MATRIXA, carblk->y_rotation);
    // *FORM NEW ROTATION MATRIX FOR CAR
    // asm 00002C3F: 	LDI	AR4,R2
    // asm 00002C40: 	ADDI	OMATRIX,R2
    // asm 00002C41: 	LDI	R2,R3
    // asm 00002C42: 	CALL	CONCATMAT
    CONCATMAT((MATRIX*)&obj->omatrix, &_MATRIXA, (MATRIX*)&obj->omatrix);
    // asm 00002C43: 	CALL	DRONE_RIDE_RIGHT	;GET DISTANCE TO CENTER OF LANE
    carblk->dist_to_center = DRONE_RIDE_RIGHT(obj, carblk);
    // asm 00002C44: 	STF	R0,*+AR5(CARDIST2CNTR)
    // stored above
    // asm 00002C45: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GEAR RATIO TABLE
 */
/* asm: GEARTAB		.float	0.0,0.60,0.35,0.21,0.15 */
static float GEARTAB[] = {
    0.0f,
    0.60f,
    0.35f,
    0.21f,
    0.15f,
};
/* asm: ENGVOL	.BSS	ENGVOL,1 */
int ENGVOL;

/*
 *----------------------------------------------------------------------------
 *GET REV FOR PLAYER CAR
 *PARAMETERS
 *	AR4	PLAYER CAR OBJECT
 *	AR5	PLAYER CAR STRUCTURE
 */
static void GETREV(void) {
    // asm 00002C4C: 	LDI	@_MODE,R1	       	;ON STARTING LINE?
    // asm 00002C4D: 	TSTB	MGO,R1
    // asm 00002C4E: 	BZ	REV0			;YES, NEUTRAL GEAR
    // asm 00002C4F: 	LDI	*+AR5(CARGEAR),R0
    // asm 00002C50: 	BZ	REV0			;NEUTRAL GEAR
    // asm 00002C51: 	LDI	*+AR5(CAR_AIRB),R0	;IN AIR?
    // asm 00002C52: 	BZ	REV1	    		;NO... REGULAR REV STUFF
    // *FREE REV NEUTRAL CASE
REV0:
    // asm 00002C53: 	CALL	GETPEDAL		;GET PEDAL 0-1
    // asm 00002C54: 	MPYF	NUM_RPM,R0
    // asm 00002C55: 	LDF	*+AR5(CARRPM),R1
    // asm 00002C56: 	FLOATP	@NFRAMES,R3
    // asm 00002C57: 	MPYF	2,R3
    // asm 00002C58: 	ADDF	R3,R1
    // asm 00002C59: 	LDF	R1,R2 			;MAX SLEW RATE LIMIT
    // asm 00002C5A: 	MPYF	2,R3
    // asm 00002C5B: 	SUBF	R3,R1			;MIN SLEW RATE LIMIT
    // asm 00002C5C: 	CMPF	R2,R0
    // asm 00002C5D: 	LDFGT	R2,R0
    // asm 00002C5E: 	CMPF	R1,R0
    // asm 00002C5F: 	LDFLT	R1,R0
    // asm 00002C60: 	B	REV3
    // *REGULAR IN GEAR CASE
REV1:
    // asm 00002C61: 	LDI	@GEARTABI,AR0
    // asm 00002C62: 	ADDI	*+AR5(CARGEAR),AR0
    // asm 00002C63: 	LDF	*+AR5(CARSPEED),R0	;GET SPEED
    // asm 00002C64: 	MPYF	*AR0,R0			;MULTIPLY BY GEAR RATIO
REV3:
    // asm 00002C65: 	FIX	*+AR5(CARRPM),R1
    // asm 00002C66: 	STF	R0,*+AR5(CARRPM)
    // asm 00002C67: 	FIX	R0,AR0
    // *R1=OLD REV
    // *AR0=NEW REV
    // *GET ENGINE VOLUME
    // *THROTTLE FACTOR
    // asm 00002C68: 	LDF	*+AR5(CARTHROTTLE),R0
    // asm 00002C69: 	MPYF	0.8,R0
    // asm 00002C6A: 	ADDF	0.2,R0
    // *SPEED FACTOR
    // asm 00002C6B: 	LDF	*+AR5(CARSPEED),R2
    // asm 00002C6C: 	MPYF	0.01,R2
    // asm 00002C6D: 	MPYF	0.333,R2
    // asm 00002C6E: 	SUBRF	1.0,R2
    // asm 00002C6F: 	MPYF	0.75,R2
    // asm 00002C70: 	ADDF	0.25,R2
    // asm 00002C71: 	CMPF	1.0,R2
    // asm 00002C72: 	LDFGT	1.0,R2
    // asm 00002C73: 	CMPF	0,R2
    // asm 00002C74: 	LDFLT	0,R2
    // asm 00002C75: 	MPYF	R2,R0
    // asm 00002C76: 	FLOAT	225,R2
    // asm 00002C77: 	MPYF	R2,R0
    // *REV FACTOR
    // asm 00002C78: 	LDF	*+AR5(CARRPM),R2
    // asm 00002C79: 	MPYF	0.02,R2
    // asm 00002C7A: 	MPYF	0.5,R2
    // asm 00002C7B: 	ADDF	0.5,R2
    // asm 00002C7C: 	MPYF	R2,R0
    // asm 00002C7D: 	FIX	R0		;NEW ENGINE VOLUME
    // *CHECK FOR CHANGE IN REV OR VOLUME
    // asm 00002C7E: 	CMPI	R1,AR0 		;NEW REV?
    // asm 00002C7F: 	BNE	REV4		;NO...
    // asm 00002C80: 	LDI	@ENGVOL,R1	;NEW ENGINE VOLUME ?
    // asm 00002C81: 	SUBI	R0,R1
    // asm 00002C82: 	ABSI	R1
    // asm 00002C83: 	CMPI	4,R1
    // asm 00002C84: 	RETSLT			;NO... QUIT
REV4:
    // asm 00002C85: 	LDI	R0,R1
    // asm 00002C86: 	STI	R0,@ENGVOL	;STORE NEW VOLUME
    // asm 00002C87: 	CMPI	0,AR0
    // asm 00002C88: 	LDILT	0,AR0
    // asm 00002C89: 	MPYI	5,AR0
    // asm 00002C8A: 	LDI	AR0,R0		;SPEED (OF OSCILLATION)
    // asm 00002C8B: 	CMPI	253,R0
    // asm 00002C8C: 	LDIGT	253,R0
    // asm 00002C8D: 	LDI	@_MODE,R2
    // asm 00002C8E: 	AND	MSLINE,R2
    // asm 00002C8F: 	BZ	REV5
    // asm 00002C90: 	LSH	-4,R1		;CUT VOLUME ON START LINE
REV5:
    // asm 00002C91: 	B	PLYR_ENGINE	;GO DO IT...
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETREV", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GETRPM	FOR DRONE CAR
 *PARAMETERS
 *	AR4	CAR OBJECT
 *	AR5	CAR STRUCTURE
 */
void GETRPM(void) {
    // asm 00002C92: 	LDI	@GEARTABI,AR0
    // asm 00002C93: 	ADDI	*+AR5(CARGEAR),AR0
    // asm 00002C94: 	LDF	*+AR5(CARSPEED),R0	;GET SPEED
    // asm 00002C95: 	MPYF	*AR0,R0			;MULTIPLY BY GEAR RATIO
    // asm 00002C96: 	CMPF	NUM_RPM,R0		;LIMIT TO RANGE
    // asm 00002C97: 	LDFGT	NUM_RPM,R0
    // asm 00002C98: 	CMPI	0,R0
    // asm 00002C99: 	LDILT	0,R0
    // asm 00002C9A: 	STF	R0,*+AR5(CARRPM)
    // asm 00002C9B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETRPM", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *GET SKID FACTOR
 *PARAMETERS
 *	AR4	CAR
 *	AR5	CAR BLOCK
 *RETURNS
 *	R0	SKID FACTOR
 *	0=NOSKID, 1.0=FULL SKID
 */
static void GETSKID(void) {
    // *CHECK SPIN OUT
    // asm 00002C9C: 	LDI	*+AR5(CAR_SPIN),R0  	;FULL SKID ON SPIN-OUT
    // asm 00002C9D: 	BEQ	GETSK00
    // asm 00002C9E:        	LDF	1.00,R0
    // asm 00002C9F: 	B	GETSKXX
    // *CHECK OVERREV
GETSK00:
    // asm 00002CA0: 	LDI	*+AR5(CARGEAR),R0	;HI GEAR?
    // asm 00002CA1: 	CMPI	4,R0
    // asm 00002CA2: 	BZ	GETSK0			;YES, NO OVERREV...
    // asm 00002CA3: 	LDF	*+AR5(CARRPM),R0
    // asm 00002CA4: 	CMPF	OVERREV,R0 		;OVERREV:
    // asm 00002CA5: 	BLT	GETSK0
    // asm 00002CA6:        	LDF	0.80,R0			;YES DO A SKID
    // asm 00002CA7: 	B	GETSKXX
    // *GET STEERING-SPEED SKID
GETSK0:
    // asm 00002CA8: 	LDF	*+AR5(CARYROT),R2
    // asm 00002CA9: 	SUBF	*+AR5(CARVROT),R2
    // asm 00002CAA: 	CALL	NORMITS
    // asm 00002CAB: 	CMPF	1.2,R2			;KEEP IN RANGE
    // asm 00002CAC: 	LDFGT	1.2,R2
    // asm 00002CAD: 	CMPF	-1.2,R2
    // asm 00002CAE: 	LDFLT	-1.2,R2
    // asm 00002CAF: 	LDF	R2,R0
    // asm 00002CB0: 	MPYF	0.2,R0		  	;ADJUST IT DUDE
    // asm 00002CB1: 	ADDF	*+AR5(CARTURN),R0
    // asm 00002CB2: 	ABSF	R0
    // asm 00002CB3: 	CMPF	0.3,R0
    // asm 00002CB4: 	LDFGT	0.3,R0			;MAX IT OUT
    // asm 00002CB5: 	MPYF	*+AR5(CARSPEED),R0
    // *GET LOW SPEED RIPOUT
    // asm 00002CB6: 	LDF	*+AR5(CARTHROTTLE),R1	;FULL THROTTLE?
    // asm 00002CB7: 	CMPF	0.90,R1
    // asm 00002CB8: 	BLT	GETSK1			;NO
    // asm 00002CB9: 	LDI	*+AR5(CARGEAR),R1	;FULL THROTTLE?
    // asm 00002CBA: 	BZ	GETSK1
    // asm 00002CBB: 	CMPI	2,R1
    // asm 00002CBC: 	BGT	GETSK1			;ONLY FIRST GEAR...
    // asm 00002CBD: 	LDF	*+AR5(CARSPEED),R1
    // ;	SUBF	65,R1
    // asm 00002CBE: 	SUBF	100,R1
    // asm 00002CBF: 	LDFGT	0,R1
    // asm 00002CC0: 	ABSF	R1
    // asm 00002CC1: 	MPYF	1.1,R1
    // asm 00002CC2: 	ADDF	R1,R0
GETSK1:
    // *GET BRAKE FACTOR
    // asm 00002CC3: 	LDF	*+AR5(CARBRAKE),R1
    // asm 00002CC4: 	MPYF	1.0,R1
    // asm 00002CC5: 	ADDF	1.0,R1
    // asm 00002CC6: 	MPYF	R1,R0			;DOUBLE IT FOR BRAKE DUDE!!!
    // *GET THROTTLE FACTOR
    // asm 00002CC7: 	LDF	*+AR5(CARTHROTTLE),R1
    // asm 00002CC8: 	MPYF	0.25,R1
    // asm 00002CC9: 	ADDF	1.0,R1
    // asm 00002CCA: 	MPYF	R1,R0			;25% BOOST FOR THROTTLE ON
    // asm 00002CCB: 	SUBF	22,R0
    // ;	SUBF	15,R0
    // asm 00002CCC: 	LDFLT	0,R0   			;NO SKID
    // ;	MPYF	0.05,R0			;DIVIDE BY 25
    // asm 00002CCD: 	MPYF	0.045,R0 		;DIVIDE BY 25
    // asm 00002CCE: GETSKX
    // asm 00002CCE: 	CMPF	1.0,R0
    // asm 00002CCF: 	LDFGT	1.0,R0
    // asm 00002CD0: 	MPYF	*+AR5(CARTRACTION),R0	;GET TRACTION COEFF.
GETSKXX:
    // asm 00002CD1: 	STF	R0,*+AR5(CARSKID) 	;NEW SKID FACTOR
    // asm 00002CD2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSKID", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CHECK FOR OFFROAD TIMEOUT
 *PARAMETERS
 *	AR5	PLAYER CAR STRUCTURE
 *RETURNS
 *	R0	RETURNED WITH OFFROAD TIMER
 */
static void CKOFRD(void) {
    // asm 00002CD3: 	LDI	AR5,AR3		       	;CHECK FOR ALL WHEELS OFF
    // asm 00002CD4: 	ADDI	CARPCOL,AR3
    // asm 00002CD5: 	LDI	0,R3
    // asm 00002CD6: 	LDI	@OFFROAD_TMR,R0
    // asm 00002CD7: 	LDI	4,RC
    // asm 00002CD8: 	RPTB	OFFRLP
    // asm 00002CD9: 	LDI	*AR3++(CARVSIZ),AR0 	;GET ROAD OBJECT INTERSECTING
    // asm 00002CDA: 	LDI	*+AR0(OID),R1		;CHECK OID
    // asm 00002CDB: 	AND	CLASS_M+TYPE_M,R1
    // asm 00002CDC: 	CMPI	ROAD_C,R1
    // asm 00002CDD: 	BZ	NOTOFF
    // asm 00002CDE: 	AND	CLASS_M,R1
    // asm 00002CDF: 	CMPI	ROAD_C,R1
    // asm 00002CE0: 	LDIZ	1,R3
    // asm 00002CE1: 	CMPI	GROUND_C,R1
OFFRLP:
    // asm 00002CE2: LDIZ	1,R3
    // asm 00002CE3: 	LDI	R3,R3 			;ANY GROUND?
    // asm 00002CE4: 	BNE	NOTOFFX			;YES
    // asm 00002CE5: 	CMPI	8,R0
    // asm 00002CE6: 	LDILE	0,R0			;RESET HIM ON 8 IF NO GROUND
    // asm 00002CE7: 	B	NOTOFFX
NOTOFF:
    // asm 00002CE8: 	LDI	20,R0
    // asm 00002CE9: 	STI	R0,@OFFROAD_TMR		;ALL WHEELS OFFROAD FLAG
NOTOFFX:
    // asm 00002CEA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKOFRD", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *GETDIR-->GET CAR DIRECTION
 *PARAMETERS
 *	R0	NEW STEERING DELTA
 *	AR4	CAR OBJECT
 *       AR5	CAR BLOCK POINTER
 *RETURNS
 *	R0	NET STEERING CHANGE
 *	CARYROT,CARVROT,CARDROT SET
 *CLOBBERED
 *	R1,R2,R3,R4,R5
 */
static void GETDIR(void) {
    // asm 00002CEB: 	LDI	*+AR5(CAR_AIRF),R1
    // asm 00002CEC: 	BZ	GETDIR1
    // asm 00002CED: 	LDF	0,R0			;NO STEERING IN AIR
    // asm 00002CEE: 	LDF	*+AR5(CARDROT),R1
    // asm 00002CEF: 	ADDF	*+AR5(CARYROT),R1
    // asm 00002CF0: 	B	GETDIR2
GETDIR1:
    // asm 00002CF1: 	LDI	*+AR5(CAR_SPIN),R1	;CHECK SPINOUT...
    // asm 00002CF2: 	BNZ	CARSPIN
    // *GET SKID FACTOR
    // asm 00002CF3: 	LDF	*+AR5(CARSKID),R4	;R4=SKID FACTOR
    // asm 00002CF4: 	LDF	1.0,R5
    // asm 00002CF5: 	SUBF	R4,R5			;R5=1-SKID FACTOR
    // asm 00002CF6: 	MPYF	0.75,R5
    // asm 00002CF7: 	ADDF	0.25,R5			;ADJUST STEER BY .5-1.0 (.5=MAX SKID)
    // asm 00002CF8: 	MPYF	R5,R0			;SKID ADJUSTED STEERING
    // *GET SLIDE DELTA ANGLE
    // asm 00002CF9: 	LDF	*+AR5(CARVROT),R3  	;VELOCITY ROTATION
    // asm 00002CFA: 	LDF	*+AR5(CARYROT),R1	;BODY ROTATION
    // asm 00002CFB: 	SUBF	R3,R1,R2		;ROTATIONAL DIFFERENCE
    // asm 00002CFC: 	CALL	NORMITS			;NORMALIZE DIFFERENCE
    // *GET RECOVERY FACTOR
    // asm 00002CFD: 	MPYF	0.095,R2
    // asm 00002CFE: 	NEGF	R2,R5
    // asm 00002CFF: 	MPYF	0.5,R5
    // asm 00002D00: 	ADDF	R5,R1
    // *GET NEW CARVROT
    // asm 00002D01: 	ADDF	R2,R3			;ADD IN RECOVERY FACTOR
    // asm 00002D02: 	ADDF	R0,R3			;ADD IN STEERING FACTOR
    // asm 00002D03: 	STF	R3,*+AR5(CARVROT)  	;VELOCITY DIRECTION
    // *GET DELTA BODY MOMENTUM
    // asm 00002D04: 	MPYF	*+AR5(CARDROT),R4	;BODY DELTA MOMENTUM ON SKID
    // asm 00002D05: 	MPYF	0.7,R4
    // asm 00002D06: 	LDF	*+AR5(CARTHROTTLE),R5
    // asm 00002D07: 	MPYF	0.5,R5
    // asm 00002D08: 	ADDF	0.5,R5
    // asm 00002D09: 	MPYF	1.2,R5
    // asm 00002D0A: 	MPYF	R5,R4			;PUMP UP BODY SLIDE...
    // asm 00002D0B: 	ADDF	R4,R0
    // asm 00002D0C: 	STF	R0,*+AR5(CARDROT)	;BODY DELTA
    // asm 00002D0D: 	ADDF	R0,R1			;ADD CARDROT TO CARYROT
GETDIR2:
    // asm 00002D0E: 	STF	R1,*+AR5(CARYROT)	;BODY DIRECTION
    // asm 00002D0F: 	RETS
    // *
    // *CAR SPINOUT
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETDIR", 0, 0);
    UNIMPL();
}

static void CARSPIN(void) {
    // asm 00002D10: 	LDI	@NFRAMES,AR3
    // asm 00002D11: 	SUBI	1,AR3			;CUT DOWN COUNT
    // asm 00002D12: 	CMPI	2,R1 			;TIMED SPIN?
    // asm 00002D13: 	BLT	SPINL0			;NO...
    // *TIMED SPIN
    // asm 00002D14: 	SUBI	1,R1			;DECREMENT TIME
    // asm 00002D15: 	SUBI	AR3,R1
    // asm 00002D16: 	CMPI	2,R1
    // asm 00002D17: 	BLE	SPINREC			;TIMES UP RECOVER
    // asm 00002D18: 	STI	R1,*+AR5(CAR_SPIN)
    // asm 00002D19: 	LDF	*+AR5(CARDROT),R1	;ROTATE THE DUDE
    // asm 00002D1A: 	ADDF	*+AR5(CARYROT),R1
    // asm 00002D1B: 	STF	R1,*+AR5(CARYROT)	;BODY DIRECTION
    // asm 00002D1C:       	B	SPINLX
    // *ANGULAR SPIN
SPINL0:
    // asm 00002D1D: 	LDF	*+AR5(CARDROT),R1	;ROTATE THE DUDE
    // asm 00002D1E: 	LDF	*+AR5(CARYROT),R2  	;VELOCITY ROTATION
    // asm 00002D1F: 	ADDF	R1,R2,R3
    // asm 00002D20: 	STF	R3,*+AR5(CARYROT)	;BODY DIRECTION
    // asm 00002D21: 	ABSF	R1,R3
    // asm 00002D22: 	SUBRF	*+AR5(CARSPRAD),R3	;READY FOR RECOVERY?
    // asm 00002D23: 	STF	R3,*+AR5(CARSPRAD)
    // asm 00002D24: 	BNN	SPINL			;SPIN NOT OVER YET...
    // asm 00002D25: 	CALL	ROADIR			;GET NEAREST TRACK PIECE ANGLE IN R0
    // *CHECK FOR DRONE
    // asm 00002D26: 	LDI	*+AR4(OID),R1
    // asm 00002D27: 	AND	CLASS_M+SUBTYPE_M,R1
    // asm 00002D28: 	CMPI	DRONE_C+DRNE_RHO,R1
    // asm 00002D29: 	BNE	REGSPIN
    // asm 00002D2A: 	LDI	*+AR4(OID),R1
    // asm 00002D2B: 	AND	TYPE_M,R1
    // asm 00002D2C: 	CMPI	DEAD_VEH_T,R1		;CHECK FOR DEAD DRONE
    // asm 00002D2D: 	BNE	DRONESPIN      		;NOT DEAD
    // asm 00002D2E: 	LDF	*+AR5(CARDROT),R0	;DECAY SPIN FOR DEAD GUY...
    // asm 00002D2F: 	MPYF	0.98,R0
    // asm 00002D30: 	ABSF	R0,R1
    // asm 00002D31: 	CMPF	0.04,R1
    // asm 00002D32: 	LDFLT	0,R0
    // asm 00002D33: 	STF	R0,*+AR5(CARDROT)
    // asm 00002D34: 	B	SPINL
DRONESPIN:
    // asm 00002D35: 	ADDF	3.14,R0			;ADJUST FOR TRAFFIC GOING AGAINST ROAD
REGSPIN:
    // asm 00002D36: 	LDF	R0,R2
    // asm 00002D37: 	SUBF	*+AR5(CARYROT),R2  	;VELOCITY ROTATION
    // asm 00002D38: 	CALL 	NORMITS			;NORMALIZE DIFFERENCE
    // asm 00002D39: 	ABSF	R2
    // asm 00002D3A: 	CMPF	0.2,R2			;ROTATIONS CLOSE?
    // asm 00002D3B: 	BLT	SPINREC0		;DO THE RECOVERY...
SPINL:
    // asm 00002D3C: 	DB	AR3,SPINL0
SPINLX:
    // asm 00002D3D: 	LDF	0,R0			;NO STEERING SPIN
    // asm 00002D3E: 	RETS
    // *RECOVER FROM SPIN DUDE
SPINREC0:
    // asm 00002D3F: 	STF	R0,*+AR5(CARYROT)	;LOAD ROAD DIRECTION
SPINREC:
    // asm 00002D40: 	LDF	*+AR5(CARYROT),R2
    // asm 00002D41: 	CALL 	NORMIT
    // asm 00002D42: 	STF	R2,*+AR5(CARYROT)	;BODY DIRECTION
    // asm 00002D43: 	STF	R2,*+AR5(CARVROT)
    // asm 00002D44: 	LDI	0,R2
    // asm 00002D45: 	STI	R2,*+AR5(CAR_SPIN)  	;CLEAR SPIN FLAG
    // asm 00002D46: 	LDF	0,R0
    // asm 00002D47: 	STF	R0,*+AR5(CARSPRAD)     	;CLEAR SPIN RADIANS
    // asm 00002D48: 	STF	R0,*+AR5(CARDROT)	;BODY DELTA
    // asm 00002D49: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARSPIN", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *GETCARROT- GET OVERROTATION
 */
static void GETCARROT(void) {
    // asm 00002D4A: 	PUSH	R3
    // asm 00002D4B: 	LDF	*+AR5(CARROT),R0      	;GET PREVIOUS VALUE
    // asm 00002D4C: 	LDF	*+AR5(CARTURN),R1
    // asm 00002D4D: 	MPYF	-0.5,R1			;ADJUST VALUE
    // asm 00002D4E: 	LDI	*+AR5(CAR_AIRF),R2 	;NO CHANGE IF IN THE AIR
    // asm 00002D4F: 	LDFNZ	R0,R1
    // asm 00002D50: 	LDF	*+AR5(CARDIST),R2	;LIMIT ROTATION SLEW RATE BY SPEED
    // asm 00002D51: 	CMPF	10,R2
    // asm 00002D52: 	LDFGT	10,R2
    // asm 00002D53: 	MPYF	0.05,R2
    // asm 00002D54: 	MPYF	0.1,R2		 	;SLEW RATE
    // asm 00002D55: 	SUBF	R1,R0,R3
    // asm 00002D56: 	ABSF	R3,R3
    // asm 00002D57: 	CMPF	R2,R3			;DONT CHANGE FOR SMALL VALUE
    // asm 00002D58: 	BLE	GETC1
    // asm 00002D59: 	SUBF	R1,R0,R3     		;LIMIT TO +-0.05
    // asm 00002D5A: 	BN	GETC0
    // asm 00002D5B: 	NEGF	R2
GETC0:
    // asm 00002D5C: 	ADDF	R2,R0,R1
GETC1:
    // asm 00002D5D: 	LDI	@WRECKFLG,R0
    // asm 00002D5E: 	LDFNZ	0,R1			;CLEAR CARROT ON WRECK
    // *EXPERIMENTAL CODE
    // ;	LDF	0,R1
    // ******************
    // asm 00002D5F: 	STF	R1,*+AR5(CARROT)
    // asm 00002D60: 	POP	R3
    // asm 00002D61: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCARROT", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *ENGINE ACCEL MULTIPLIER TABLE
 *
 */
/* asm: GEARACTABI	.word	GEARACTAB */
#define GEARACTABI GEARACTAB
/* asm: GEARACTAB */
/* asm: 	.float	0.0,1.7,1.5,1.4,1.2  		;POWER FACTOR GEAR(0-4) */
static float GEARACTAB[] = {
    0.0f,
    1.7f,
    1.5f,
    1.4f,
    1.2f, // POWER FACTOR GEAR(0-4)
};
/* asm: ENGACTABI	.word	ENGACTAB */
#define ENGACTABI ENGACTAB
/* asm: ENGACTAB */
/* asm: 	.float	1.20,1.20,0.50,0.60,0.70	;0000,0300,0600,0900,1200 */
/* asm: 	.float	0.80,0.90,1.00,1.00,1.00	;1500,1800,2100,2400,2700 */
/* asm: 	.float	1.00,1.00,1.00,1.00,0.90	;3000,3300,3600,3900,4200 */
/* asm: 	.float	0.80,0.40,0.20,0.00,0.00	;4500,4800,5100,5400,5700 */
/* asm: 	 */
static float ENGACTAB[] = {
    1.20f,
    1.20f,
    0.50f,
    0.60f,
    0.70f, // 0000,0300,0600,0900,1200
    0.80f,
    0.90f,
    1.00f,
    1.00f,
    1.00f, // 1500,1800,2100,2400,2700
    1.00f,
    1.00f,
    1.00f,
    1.00f,
    0.90f, // 3000,3300,3600,3900,4200
    0.80f,
    0.40f,
    0.20f,
    0.00f,
    0.00f, // 4500,4800,5100,5400,5700
};
// *ENGINE FRICTION
/* asm: ENGFR  	.float	0.000,0.005,0.003,0.001,0.000  	;GEAR(0-4) ENGINE FRICTION */
/* asm: 	 */
static float ENGFR[] = {
    0.000f,
    0.005f,
    0.003f,
    0.001f,
    0.000f, // GEAR(0-4) ENGINE FRICTION
};

/*
 *----------------------------------------------------------------------------
 *GET NEW CAR SPEED
 *
 *PARAMETERS
 *	AR4	CAR OBJECT
 *	AR5	CAR BLOCK
 *
 */
void GETSPD(void) {
    // asm 00002D83: 	LDI	*+AR5(CAR_AIRB),R0
    // asm 00002D84: 	BZ	GETSPD1	      		;NO AIR DUDE...
    // *AIRBORNE CASE
    // asm 00002D85: 	FLOATP	@NFRAMES,R0
    // asm 00002D86: 	MPYF	4,R0 			;GET GRAVITATIONAL ACCEL
    // asm 00002D87: 	ADDF	*+AR4(OVELY),R0
    // asm 00002D88: 	STF	R0,*+AR4(OVELY)
    // asm 00002D89: 	LDF	0,R0			;SET ACCEL, FRICT TO ZERO
    // asm 00002D8A: 	LDF	0,R3
    // asm 00002D8B: 	BR	GETSPD2
    // *CHECK SPIN OUT
GETSPD1:
    // asm 00002D8C: 	LDI	*+AR5(CAR_SPIN),R0
    // asm 00002D8D: 	BZ	GETSPD10       		;NO SPINOUT...
    // asm 00002D8E: 	LDF	0,R0			;SET ACCEL TO ZERO
    // asm 00002D8F: 	LDF	@SPINFRICI,R3
    // asm 00002D90: 	BR	GETSPD2
    // *GET ENGINE ACCEL
GETSPD10:
    // asm 00002D91: 	LDF	*+AR5(CARTHROTTLE),R0
    // asm 00002D92: 	MPYF	*+AR5(CARMAXACCEL),R0
    // asm 00002D93: 	CMPI	@PLYCAR,AR4	    	;CHEAT ACCEL
    // asm 00002D94: 	LDFNZ	1.0,R1
    // asm 00002D95: 	LDFZ	@CHEATACC,R1
    // asm 00002D96: 	MPYF	R1,R0
    // asm 00002D97: 	LDF	*+AR5(CARRPM),R1
    // asm 00002D98: 	MPYF	0.333,R1
    // asm 00002D99: 	FIX	R1,IR0		   	;GET TABLE INDEX
    // asm 00002D9A: 	CMPI	18,IR0
    // asm 00002D9B: 	LDIGT	18,IR0			;KEEP INDEX IN RANGE
    // asm 00002D9C: 	LDI	@ENGACTABI,AR0
    // asm 00002D9D: 	LDF	*+AR0(IR0),R2		;GET LO POWER FACTOR FOR GEAR
    // asm 00002D9E: 	ADDI	1,IR0
    // asm 00002D9F: 	LDF	*+AR0(IR0),R3		;GET HI POWER FACTOR FOR GEAR
    // asm 00002DA0: 	FLOAT	IR0,R4
    // asm 00002DA1: 	SUBF	R1,R4,R1
    // asm 00002DA2: 	LDFLT	0,R1  			;KEEP FACTOR IN BOUNDS
    // asm 00002DA3: 	CMPF	1.0,R1
    // asm 00002DA4: 	LDFGT	1.0,R1
    // asm 00002DA5: 	MPYF	R1,R2			;INTERPOLATE !!!
    // asm 00002DA6: 	SUBRF	1.0,R1
    // asm 00002DA7: 	MPYF	R1,R3
    // asm 00002DA8: 	ADDF	R2,R3,R1
    // asm 00002DA9: 	LDI	*+AR5(CARGEAR),IR0	;GEAR MULTIPLIER
    // asm 00002DAA: 	LDI	@GEARACTABI,AR0
    // asm 00002DAB: 	MPYF	*+AR0(IR0),R1
    // asm 00002DAC: 	LDF	1.0,R2
    // asm 00002DAD: 	LDI	*+AR5(CARTRANS),R3
    // asm 00002DAE: 	LDFZ	0.96,R2			;4% POWER LOSS AUTOMATIC
    // asm 00002DAF: 	MPYF	R2,R1
    // *CUT ACCEL ON SKID
    // asm 00002DB0: 	MPYF	R1,R0
    // asm 00002DB1: 	LDF	*+AR5(CARSKID),R1    	;CUT DOWN ACCEL ON SKID
    // asm 00002DB2: 	MPYF	0.25,R1			;ONLY 25% CUT
    // asm 00002DB3: 	SUBRF	1.0,R1
    // asm 00002DB4: 	MPYF	R1,R0			;R0=ENGINE ACCEL
    // *GET GRAVITY ACCEL
    // asm 00002DB5: 	LDF	*+AR4(OMAT21),R1	;ADD IN YOUR GRAVITY ACTION
    // asm 00002DB6: 	LDF	2,R3			;DEFAULT CONSTANT
    // asm 00002DB7: 	LDI	@_countdown,R2		;TIMEOUT?
    // asm 00002DB8: 	LDFZ	0,R3			;YES, NO GRAVITY
    // asm 00002DB9: 	LDI	@_MODE,R2		;ON START LINE?
    // asm 00002DBA: 	TSTB	MGO,R2
    // asm 00002DBB: 	LDFZ	0,R3			;YIP, NO GRAVITY
    // asm 00002DBC: 	LDF	*+AR5(CARBRAKE),R2	;BRAKE ON?
    // asm 00002DBD: 	CMPF	0.5,R2
    // asm 00002DBE: 	LDFGT	0,R3			;YES, NO GRAV
    // asm 00002DBF: 	MPYF	R3,R1			;MULTIPLY BY CONSTANT
    // asm 00002DC0: 	ADDF	R1,R0
    // *GET TOTAL FRICTION
    // *GET ROAD FRICTION
    // asm 00002DC1: 	LDI	AR5,AR3
    // asm 00002DC2: 	ADDI	CARPCOL,AR3
    // asm 00002DC3: 	LDF	0,R3
    // asm 00002DC4: 	LDI	4,RC
    // asm 00002DC5: 	RPTB	FRICLP
    // asm 00002DC6: 	LDI	*AR3++(CARVSIZ),AR0 	;GET ROAD OBJECT INTERSECTING
    // asm 00002DC7: 	LDI	*+AR0(OID),R1		;CHECK OID
    // asm 00002DC8: 	AND	CLASS_M+TYPE_M,R1
    // asm 00002DC9: 	CMPI	ROAD_C,R1
    // asm 00002DCA: 	LDFZ	*+AR5(CARRDFR),R2    	;GET ROAD FRICTION
    // asm 00002DCB: 	LDFNZ	*+AR5(CAROFRDFR),R2	;GET OFF ROAD FRICTION
    // ;	MPYF	@DRAFTVAL,R2		;ADJUST FOR DRAFT
FRICLP:
    // asm 00002DCC: ADDF	R2,R3
    // asm 00002DCD: 	MPYF	0.20,R3			;TAKE AVERAGE BASED ON WHEELS OFF
    // *JARV CHANGE
    // asm 00002DCE: 	CMPI	@PLYCAR,AR4
    // asm 00002DCF: 	BNE	FRL1
    // asm 00002DD0: 	LDF	@CHEAT,R1
    // asm 00002DD1: 	CMPF	1.09,R1
    // asm 00002DD2: 	BLE	FRL1
    // asm 00002DD3: 	ADDF	*+AR5(CARRDFR),R3 	;CUT DOWN OFF ROAD FRIC IF BEHIND
    // asm 00002DD4: 	MPYF	0.5,R3
FRL1:
    // *JARV ENDCHANGE
    // *GET SKID FRICTION
    // asm 00002DD5: 	LDF	*+AR5(CARSKID),R4    	;ADD IN SKID FACTOR
    // asm 00002DD6: 	LDF	@SKIDFRICI,R1
    // asm 00002DD7: 	MPYF	R1,R4,R5
    // asm 00002DD8: 	LDF	*+AR5(CARSPEED),R2	;CUT SKID FRICTION FOR LOW SPEED BURNOUT
    // asm 00002DD9: 	CMPF	100,R2
    // asm 00002DDA: 	BGT	FRIC0
    // asm 00002DDB: 	LDF	*+AR5(CARTHROTTLE),R2	;FULL THROTTLE?
    // asm 00002DDC: 	CMPF	0.90,R2
    // asm 00002DDD: 	BLT	FRIC0
    // asm 00002DDE: 	LDI	*+AR5(CARGEAR),R2
    // asm 00002DDF: 	CMPI	2,R2
    // asm 00002DE0: 	BLE	FRIC1			;LOW SPEED BURNOUT, NO FRICTION111
    // ;	BZ	FRIC1			;LOW SPEED BURNOUT, NO FRICTION111
FRIC0:
    // asm 00002DE1: 	ADDF	R5,R3
    // *GET BRAKE FRICTION
FRIC1:
    // asm 00002DE2: 	LDF	*+AR5(CARBRAKE),R5   	;ADD IN BRAKE FRICTION
    // asm 00002DE3: 	MPYF	@BRAKFRICI,R5
    // asm 00002DE4: 	NEGF	R4
    // asm 00002DE5: 	ADDF	1.0,R4			;BRAKE LOSES EFFECT IN SKID
    // asm 00002DE6: 	MPYF	R4,R5
    // asm 00002DE7: 	LDF	1.0,R4			;INCREASE BRAKE EFFECTIVENESS AT LO SPD
    // asm 00002DE8: 	LDF	*+AR5(CARSPEED),R1
    // asm 00002DE9: 	CMPF	40,R1
    // asm 00002DEA: 	LDFLT	1.5,R4
    // asm 00002DEB: 	CMPF	20,R1
    // asm 00002DEC: 	LDFLT	2.0,R4
    // asm 00002DED: 	MPYF	R4,R5
    // asm 00002DEE: 	ADDF	R5,R3		     	;TOTAL FRICTION
    // *GET ENGINE FRICTION
    // asm 00002DEF: 	LDI	*+AR5(CARGEAR),IR0
    // asm 00002DF0: 	LDI	@ENGFRI,AR0
    // asm 00002DF1: 	LDF	*+AR0(IR0),R4
    // asm 00002DF2: 	LDF	*+AR5(CARRPM),R5	;MORE REVS MORE FRICTION
    // asm 00002DF3: 	SUBF	42,R5			;HEAVY FRICTION ABOVE 3900
    // asm 00002DF4: 	LDFLT	0,R5
    // asm 00002DF5: 	MPYF	0.04,R5
    // asm 00002DF6: 	ADDF	1.0,R5			;APPROX RANGE 1-2
    // asm 00002DF7: 	MPYF	R4,R5
    // asm 00002DF8: GETSP22
    // asm 00002DF8: 	ADDF	R5,R3
    // *CALC NEW SPEED
GETSPD2:
    // asm 00002DF9: 	LDF	0,R5
    // asm 00002DFA: 	LDF	*+AR5(CARSPEED),R1
    // asm 00002DFB: 	LDI	@NFRAMES,RC
    // asm 00002DFC: 	CMPI	6,RC	 		;MAX IT OUT TO BE SAFE
    // asm 00002DFD: 	LDIGT	6,RC
    // asm 00002DFE: 	SUBI	1,RC
    // *R0=ACCEL
    // *R1=SPEED
    // *R3=TOTAL FRICTION
    // *R5=DISTANCE
    // *R7=ENGINE FRICTION
    // asm 00002DFF: 	RPTB	GSL0
    // asm 00002E00: 	MPYF 	R1,R3,R4	     	;BRAKE/ROAD/ENGINE FRICTION
    // asm 00002E01: 	SUBF	R4,R1			;SUBTRACT FRICTION
    // asm 00002E02: 	ADDF	R0,R1			;ADD ACCEL
GSL0:
    // asm 00002E03: ADDF	R1,R5			;ADD TO DISTANCE
    // asm 00002E04: 	CMPF	0.5,R5	      		;MINIMUM SPEED
    // asm 00002E05: 	LDFLT	0,R5
    // asm 00002E06: 	CMPF	0,R1			;NO NEGATIVE SPEED
    // asm 00002E07: 	LDFLT	0,R1
    // asm 00002E08: 	MPYF	1.5,R5			;SPEEDFUDGE
    // asm 00002E09: 	STF	R5,*+AR5(CARDIST)	;SAVE YOUR DISTANCE
    // asm 00002E0A: 	STF	R1,*+AR5(CARSPEED)	;NEW SPEED
    // asm 00002E0B: 	RETS
    // *
    // *GET BRAKE PEDAL
    // *RETURNS R0=BRAKE VALUE 0-1 (FLOAT)
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSPD", 0, 0);
    UNIMPL();
}

static void GETBRAKE(void) {
    // asm 00002E0C: 	FLOATP	@_pot2,R0
    // asm 00002E0D: 	NEGF	@BRAKEMN,R1
    // asm 00002E0E: 	ADDF	R1,R0
    // asm 00002E0F: 	CMPF	20,R0
    // asm 00002E10: 	LDFLT	0,R0
    // asm 00002E11: 	BLT	BRAKEOFF
    // asm 00002E12: 	ADDF	@BRAKEMX,R1	;ALL DP RAM IN SAME PAGE
    // asm 00002E13: 	CALL	DIV_F
    // asm 00002E14: 	CMPF	1.0,R0 		;KEEP IT IN RANGE
    // asm 00002E15: 	LDFGT	1.0,R0
    // asm 00002E16: 	CMPF	0,R0 		;KEEP IT IN RANGE
    // asm 00002E17: 	LDFLT	0,R0
    // asm 00002E18: 	MPYF	R0,R0		;SQUARE IT FOR NON-LINEAR FEEL
    // asm 00002E19: 	PUSHF	R0		;SAVE THE VALUE
    // asm 00002E1A: 	LDI	@BRAKEON,R0
    // asm 00002E1B: 	BNZ	GETBX		;BRAKE ALREADY ON
    // asm 00002E1C: 	CALL	_on_brake	;TURN ON FIRST TIME ONLY
    // asm 00002E1D: 	LDI	1,R0
    // asm 00002E1E: 	B	GETBXX
BRAKEOFF:
    // asm 00002E1F: 	PUSHF	R0		;SAVE THE VALUE
    // asm 00002E20: 	LDI	@BRAKEON,R0
    // asm 00002E21: 	BZ	GETBX	   	;ALREADY OFF DUDES
    // asm 00002E22: 	CALL	_off_brake
    // asm 00002E23: 	LDI	0,R0
GETBXX:
    // asm 00002E24: STI	R0,@BRAKEON
GETBX:
    // asm 00002E25: 	POPF	R0
    // asm 00002E26: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETBRAKE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *
 *TURN ON/OFF BRAKE LIGHTS
 *
 *PALSET- SETUP PALETTE TRANSFER
 *AR2=SOURCE DATA ADDRESSS
 *R2 =DEST PALETTE(B8-15), DEST COLOR(B0-7)
 *R3 =COUNT
 */
void _off_brake(void) {
    // asm 00002E31: 	PUSH	AR0
    // asm 00002E32: 	PUSH	AR2
    // asm 00002E33: 	PUSH	R2
    // asm 00002E34: 	PUSH	R3
    // asm 00002E35: 	LDI	@BUTTON_STATUS,R2
    // asm 00002E36: 	ANDN	BUT_TAILS,R2
    // asm 00002E37: 	STI	R2,@BUTTON_STATUS
    // asm 00002E38: 	LDI	@OFFREDI,AR2
    // asm 00002E39: 	PUSH	AR2
    // asm 00002E3A: 	BU	L888
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_off_brake", 0, 0);
    UNIMPL();
}

void _on_brake(void) {
    // asm 00002E3B: 	PUSH	AR0
    // asm 00002E3C: 	PUSH	AR2
    // asm 00002E3D: 	PUSH	R2
    // asm 00002E3E: 	PUSH	R3
    // asm 00002E3F: 	LDI	BUT_TAILS,R2
    // asm 00002E40: 	OR	@BUTTON_STATUS,R2
    // asm 00002E41: 	STI	R2,@BUTTON_STATUS
    // asm 00002E42: 	LDI	@HIREDI,AR2
    // asm 00002E43: 	PUSH	AR2
L888:
    // asm 00002E44: LDI	@CHOOSENCAR,AR0
    // asm 00002E45: 	MPYI	VEHTAB_SIZE,AR0
    // asm 00002E46: 	ADDI	@VEHICLE_TABLEI,AR0
    // asm 00002E47: 	LDI	*+AR0(VEHTAB_TAILCNT),R3
    // asm 00002E48: 	BZ	NO_COLORS
    // asm 00002E49: 	LDI	*+AR0(VEHTAB_PAL),AR2
#if DEBUG
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	$	;table entry not filled
#endif
    // asm 00002E4A: 	CALL	PAL_FIND
    // asm 00002E4B: 	ADDI	256,R0
    // asm 00002E4C: 	SUBI	R3,R0
    // asm 00002E4D: 	LDI	R0,R2
    // asm 00002E4E: 	POP	AR2
    // asm 00002E4F: 	CALL	PAL_SET
BRAK_X:
    // asm 00002E50: POP	R3
    // asm 00002E51: 	POP	R2
    // asm 00002E52: 	POP	AR2
    // asm 00002E53: 	POP	AR0
    // asm 00002E54: 	RETS
NO_COLORS:
    // asm 00002E55: 	POP	AR2
    // asm 00002E56: 	BU	BRAK_X
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_on_brake", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *GET GAS PEDAL
 *RET R0=GAS PEDAL 0-1 FRACTION
 */
static void GETPEDAL(void) {
    // asm 00002E57: 	FLOATP	@_pot1,R0
    // asm 00002E58: 	NEGF	@PEDALMN,R1
    // asm 00002E59: 	ADDF	R1,R0
    // asm 00002E5A: 	CMPF	5,R0
    // asm 00002E5B: 	LDFLT	0,R0		;GET RID OF CREEP AND NEGATIVES
    // asm 00002E5C: 	ADDF	@PEDALMX,R1
    // asm 00002E5D: 	CALL	DIV_F
    // asm 00002E5E: 	CMPF	1.0,R0 		;KEEP IT IN RANGE
    // asm 00002E5F: 	LDFGT	1.0,R0
    // asm 00002E60: 	RETS
    // *GET GEAR SHIFT
    // *AR4=CAR OBJECT
    // *AR5=CAR STRUCTURE
    // *RETURNS:
    // *R0=GEAR SHIFT VALUE 0=neutral,1,2,3,4  (INT)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETPEDAL", 0, 0);
    UNIMPL();
}

static void GETGEAR(void) {
    // asm 00002E61: 	LDI	*+AR5(CARTRANS),R0
    // asm 00002E62: 	BNZ	GETMAN
    // asm 00002E63: 	LDI	@SUSPEND_MODE,R0	;SUSPEND MODE?
    // asm 00002E64: 	CMPI	SM_HALT,R0
    // asm 00002E65: 	BNZ	GETAUTO			;NO
    // asm 00002E66: 	LDI	*+AR5(CARGEAR),R0	;YES, DONT SWITCH AUTO TRANS
    // asm 00002E67: 	RETS
    // *MANUAL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETGEAR", 0, 0);
    UNIMPL();
}

static void GETMAN(void) {
    // asm 00002E68: 	LDI	0,R0
    // asm 00002E69: 	LDI	@SWITCHBUTS,R1
    // asm 00002E6A: 	TSTB	SW_4TH,R1
    // asm 00002E6B: 	LDINZ	4,R0
    // asm 00002E6C: 	TSTB	SW_3RD,R1
    // asm 00002E6D: 	LDINZ	3,R0
    // asm 00002E6E: 	TSTB	SW_2ND,R1
    // asm 00002E6F: 	LDINZ	2,R0
    // asm 00002E70: 	TSTB	SW_1ST,R1
    // asm 00002E71: 	LDINZ	1,R0
    // asm 00002E72: 	RETS
    // *AUTOMATIC
    // *AR4=CAR OBJECT
    // *AR5=CAR STRUCTURE
    // *RETURNS:
    // *R0=GEAR SHIFT VALUE 0=neutral,1,2,3,4  (INT)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETMAN", 0, 0);
    UNIMPL();
}

void GETAUTO(void) {
    // asm 00002E73: 	LDI	0,R2
    // asm 00002E74: 	LDF	*+AR5(CARTHROTTLE),R3	;TORQUE SENSOR FOR DOWNSHIFT
    // asm 00002E75: 	CMPF	1.0,R3
    // asm 00002E76: 	LDFGT	1.0,R3
    // asm 00002E77: 	MPYF	11,R3
    // asm 00002E78: 	FIX	R3
    // asm 00002E79: 	ADDI	12,R3
    // asm 00002E7A: 	LDI	*+AR5(CARGEAR),R0
    // asm 00002E7B: 	FIX	*+AR5(CARRPM),R1
    // asm 00002E7C: 	CMPI	R3,R1
    // asm 00002E7D: 	LDILT	-1,R2	 		;DOWNSHIFT
    // asm 00002E7E: 	ADDI	18,R3
    // asm 00002E7F: 	CMPI	R3,R1
    // asm 00002E80: 	LDIGT	1,R2			;UPSHIFT
    // asm 00002E81: 	ADDI	R2,R0
    // asm 00002E82: 	CMPI	1,R0
    // asm 00002E83: 	LDILT	1,R0
    // asm 00002E84: 	CMPI	4,R0
    // asm 00002E85: 	LDIGT	4,R0			;MAX OUT AT 4TH
    // asm 00002E86: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETAUTO", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
/* asm: STEERI	.float	-0.0013 */
float STEERI = -0.0013f;

/*
 *GET STEERING ANGLE
 *AR5=CAR STRUCTURE
 *RET R0= STEERING ANGLE
 *	 SETS CARTURN VALUE
 */
static void GETSTEER(void) {
    // asm 00002E88: 	LDI	ADJ_STEERING_SENSITIVITY,AR2		;GET DIFFICULTY	ADJUST
    // asm 00002E89: 	CALL	ADJUSTMENT_READ		;R0  = 0-5 (int)
    // asm 00002E8A: 	FLOAT	R0,R4
    // asm 00002E8B: 	FLOAT	R0,R6
    // asm 00002E8C: 	SUBRF	5,R4
    // asm 00002E8D: 	MPYF	0.06,R4
    // asm 00002E8E: 	ADDF	0.5,R4
    // asm 00002E8F: 	LDF	R4,R5
    // asm 00002E90: 	SUBRF	1.0,R5
    // asm 00002E91: 	FLOAT	@_pot0,R0 		;GET POT VALUE
    // asm 00002E92: 	SUBF	@STEERCT,R0		;SUBTRACT CENTER VALUE
    // asm 00002E93: 	LDF	@STEERFR,R1
    // asm 00002E94: 	MPYF	0.5,R1
    // asm 00002E95: 	CALL	DIV_F
    // asm 00002E96: 	CMPF	-1,R0 			;LIMIT CHECK
    // asm 00002E97: 	LDFLT	-1,R0
    // asm 00002E98: 	CMPF	1,R0 			;LIMIT CHECK
    // asm 00002E99: 	LDFGT	1,R0
    // asm 00002E9A: 	MPYF	0.02,R6	    		;DESENSITIZE
    // asm 00002E9B: 	ADDF	0.90,R6
    // asm 00002E9C: 	MPYF	R6,R0
    // asm 00002E9D: 	ABSF	R0,R1			;SQUARE IT KEEPING SIGN
    // asm 00002E9E: 	MPYF	R0,R1
    // asm 00002E9F: 	MPYF	R4,R1			;RATIO OF STEERING VALUE SQUARED
    // asm 00002EA0: 	MPYF	R5,R0			;RATIO OF REGULAR
    // ;	MPYF	0.5,R1			;AVERAGE WITH REGULAR
    // ;	MPYF	0.5,R0
    // asm 00002EA1: 	ADDF	R1,R0
    // asm 00002EA2: 	LDF	R0,R1
    // asm 00002EA3: 	MPYF	-0.3,R1			;FUDGE FACTOR FOR WHEEL TURN ANGLE
    // asm 00002EA4: 	STF	R1,*+AR5(CARTURN)	;STORE WHEEL TURN VALUE
    // asm 00002EA5: 	LDF	*+AR5(CARSPEED),R1
    // asm 00002EA6: 	LDF 	R1,R2
    // asm 00002EA7: 	MPYF	R1,R1
    // asm 00002EA8: 	MPYF	0.0125,R1
    // asm 00002EA9: 	CMPF	5,R1			;LIMIT LO SPEED KICK
    // asm 00002EAA: 	LDFGT	5,R1
    // asm 00002EAB: 	MPYF	0.0625,R2   	      	;HIGH SPEED KICK
    // asm 00002EAC: 	CMPF	15,R2
    // asm 00002EAD: 	LDFGT	15,R2
    // asm 00002EAE: 	ADDF	R2,R1
    // asm 00002EAF: 	MPYF	R1,R0
    // asm 00002EB0: 	FLOATP	@NFRAMES,R1
    // asm 00002EB1: 	MPYF	R1,R0
    // asm 00002EB2: 	MPYF	@STEERI,R0
    // asm 00002EB3: 	RETS
    // *
    // *VIEW SWITCHES
    // *ZOOM TO FIRST PERSON
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSTEER", 0, 0);
    UNIMPL();
}

void _VIEW0(void) {
    // asm 00002EB4: 	FLOAT	PLYPOS1Z,R4	;Z DIST
    // asm 00002EB5: 	FLOAT	PLYPOS1Y,R5	;Y DIST
    // asm 00002EB6: 	LDI	0,R2		;CAMVIEW STATUS
    // asm 00002EB7: 	LDI	BUT_VIEW1,R3
    // asm 00002EB8:  	B	ZOOM
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_VIEW0", 0, 0);
    UNIMPL();
}

void _VIEW1(void) {
    // asm 00002EB9: 	FLOAT	PLYPOS2Z,R4	;Z DIST
    // asm 00002EBA: 	FLOAT	PLYPOS2Y,R5	;Y DIST
    // asm 00002EBB: 	LDI	1,R2		;CAMVIEW STATUS
    // asm 00002EBC: 	LDI	BUT_VIEW2,R3
    // asm 00002EBD:  	B	ZOOM
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_VIEW1", 0, 0);
    UNIMPL();
}

void _VIEW2(void) {
    // asm 00002EBE: 	FLOAT	PLYPOS3Z,R4	;Z DIST
    // asm 00002EBF: 	FLOAT	PLYPOS3Y,R5	;Y DIST
    // asm 00002EC0: 	LDI	2,R2		;CAMVIEW STATUS
    // asm 00002EC1: 	LDI	BUT_VIEW3,R3
    // *R2=VIEW (0-2)INT
    // *R3=BUTTON LITE MASK
    // *R4=PLYPOSZ FL
    // *R5=PLYPOSY FL
ZOOM:
    // asm 00002EC2: 	LDI	@_MODE,R0
    // asm 00002EC3: 	AND	MMODE,R0
    // asm 00002EC4: 	CMPI	MGAME,R0
    // asm 00002EC5: 	BNE	SUICIDE	       		;DIE IF NOT IN GAME MODE
    // asm 00002EC6: 	CMPI	@CAMVIEW,R2
    // asm 00002EC7: 	BEQ	SUICIDE			;DIE IF SAME MODE
    // asm 00002EC8: 	LDILT	VIEW2IN,AR2
    // asm 00002EC9: 	LDIGE	VIEW2OUT,AR2
    // asm 00002ECA: 	CALL	ONESNDFX
    // asm 00002ECB: 	LDI	@BUTTON_STATUS,R0	;LITE YOUR LITE
    // asm 00002ECC: 	ANDN	BUT_VIEWS,R0
    // asm 00002ECD: 	OR	R3,R0
    // asm 00002ECE: 	STI	R0,@BUTTON_STATUS
    // asm 00002ECF: 	STF	R4,@ZOOMDG		;STORE GOAL X
    // asm 00002ED0: 	STF	R5,@ZOOMHG	 	;STORE GOAL Y
    // asm 00002ED1: 	SUBF	@ZOOMD,R4
    // asm 00002ED2: 	MPYF	ZOOMRATIO,R4
    // asm 00002ED3: 	SUBF	@ZOOMH,R5
    // asm 00002ED4: 	MPYF	ZOOMRATIO,R5
    // asm 00002ED5: 	LDF	1,R0
    // asm 00002ED6: 	ABSF	R4,R6
    // asm 00002ED7: 	CMPF	25,R6			;PUMP UP FOR LOW ZOOM RATE
    // asm 00002ED8: 	LDFLT	4,R0
    // asm 00002ED9: 	MPYF	R0,R4
    // asm 00002EDA: 	MPYF	R0,R5
    // asm 00002EDB: 	STF	R4,@ZOOMDD
    // asm 00002EDC: 	STF	R5,@ZOOMHD
    // asm 00002EDD: 	LDI	@CAMVIEW,R3		;CHANGE FROM FIRST PERSON ?
    // asm 00002EDE: 	STI	R2,@CAMVIEW		;SAVE NEW CAMERA VIEW
    // asm 00002EDF: 	BNZ	ZOOMX			;NO...
    // asm 00002EE0: 	LDI	@PLYCAR,AR2
    // asm 00002EE1: 	LDI	*+AR2(OFLAGS),R0	;CHECK IF ALREADY ON LIST
    // asm 00002EE2: 	TSTB	O_LIST_M,R0
    // asm 00002EE3: 	CALLZ	OBJ_INSERT		;INSERT PLAYER OBJECT
ZOOMX:
    // asm 00002EE4: 	CALL	ZOOMUP
    // asm 00002EE5: 	BR	SUICIDE	      		;CAN IT DUDES
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_VIEW2", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *UPDATE YOUR ZOOM JIVE
 */
static void ZOOMUP(void) {
    // asm 00002EE6: 	LDF	@ZOOMDD,R2
    // asm 00002EE7: 	BZ	ZOOMUPX		;NO ACTIVE ZOOM, SKIP IT
    // asm 00002EE8: 	BND	ZOOMUP1
    // asm 00002EE9: 	LDF	@ZOOMH,R1
    // asm 00002EEA: 	ADDF	@ZOOMHD,R1
    // asm 00002EEB: 	ADDF	@ZOOMD,R2
    // 	;------->BND	ZOOMUP1
    // asm 00002EEC: 	CMPF	@ZOOMDG,R2
    // asm 00002EED: 	BGE	ZOOMDN		;DONE WITH ZOOM
    // asm 00002EEE: 	B	ZOOMUP3
ZOOMUP1:
    // asm 00002EEF: 	CMPF	@ZOOMDG,R2
    // asm 00002EF0: 	BGT	ZOOMUP3		;NOT DONE WITH ZOOM
ZOOMDN:
    // asm 00002EF1: LDI	@CAMVIEW,R0	;FIRST PERSON DONE?
    // asm 00002EF2: 	BNZ	ZOOMDN1		;NO..
    // asm 00002EF3: 	LDI	@PLYCAR,AR2
    // asm 00002EF4: 	CALL	OBJ_PULL	    	;CAN THE CAR
ZOOMDN1:
    // asm 00002EF5: LDF	0,R0
    // asm 00002EF6: 	STF	R0,@ZOOMDD	;CLEAR OUT VELOCITIES
    // asm 00002EF7: 	STF	R0,@ZOOMHD
    // asm 00002EF8: 	LDF	@ZOOMDG,R2
    // asm 00002EF9: 	LDF	@ZOOMHG,R1
ZOOMUP3:
    // asm 00002EFA: STF	R1,@ZOOMH	;UPDATE POSITION
    // asm 00002EFB: 	STF	R2,@ZOOMD
ZOOMUPX:
    // asm 00002EFC: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOMUP", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *GETTRAK	GET NEAREST TRACK SECTION
 *PARAMETERS
 *	AR4	OBJECT
 *	AR5	STRUCTURE
 *RETURNS
 *	AR0	TRACK SEGMENT STORED IN AR5(CARTRAK)
 *CLOBBERS
 *	AR2,R0,R3,R4
 */
void GETTRAK(OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/) {
    OBJ* closest_track_obj;
    OBJ* track_obj;
    float closest_dist2;
    float dx;
    float dz;
    float dist2;

    // asm 00002EFD: 	LDI	0,AR0			;CLOSEST ROAD SEGMENT INDEX
    // asm 00002EFE: 	LDI	@DYNALIST_BEGIN,R0
    // asm 00002EFF: 	BZ	GETRKX  			;NULL LIST DUDES
    closest_track_obj = NULL;
    if (DYNALIST_BEGIN == 0) {
        return;
    }

    // asm 00002F00..00002F06
    closest_dist2 = 32767.0f * 32767.0f;
    track_obj = (OBJ*)(uintptr_t)DYNALIST_BEGIN;
GETRK:
    // asm 00002F07: 	SUBF	*+AR2(OPOSX),R3,R0
    // asm 00002F08: 	MPYF	R0,R0
    // asm 00002F09: 	SUBF	*+AR2(IR0),R4,R1
    // asm 00002F0A: 	MPYF	R1,R1
    // asm 00002F0B: 	ADDF	R0,R1
    // asm 00002F0C: 	CMPF	R1,R2
    // asm 00002F0D: 	LDIGT	AR2,AR0
    // asm 00002F0E: 	LDFGT	R1,R2
    dx = obj->posx - track_obj->posx;
    dz = obj->posz - track_obj->posz;
    dist2 = (dx * dx) + (dz * dz);
    if (dist2 < closest_dist2) {
        closest_track_obj = track_obj;
        closest_dist2 = dist2;
    }
    // asm 00002F0F: GETRKL
    // asm 00002F0F: 	LDI	*+AR2(OLINK4),R0
    // asm 00002F10: 	BNZD	GETRK
    // asm 00002F11: 	LDI	R0,AR2
    // asm 00002F12: 	NOP
    // asm 00002F13: 	NOP
    // 	;---->	BNZ	GETRK
    track_obj = (OBJ*)track_obj->link4;
    if (track_obj != NULL) {
        goto GETRK;
    }
    // asm 00002F14: 	STI	AR0,*+AR5(CARTRAK)	;SAVE TRACK SECTION
    carblk->closest_track_piece = (uintptr_t)closest_track_obj;
GETRKX:
    // asm 00002F16: 	RETS
}

/*
 *----------------------------------------------------------------------------
 *CHECK IF DRIVING BACKWARDS
 *PARAMETERS
 *	AR4	CAR
 *	AR5	CAR STRUCTURE
 */
static void BACKCK(void) {
    // asm 00002F17: 	LDI	*+AR5(CAR_SPIN),R0
    // asm 00002F18: 	BNE	BACKCKX
    // asm 00002F19: 	LDF	*+AR5(CARSPEED),R4	;MUST BE MOVING SOMEWHAT
    // asm 00002F1A: 	CMPF	2,R4
    // asm 00002F1B: 	BLT	BACKCKX
    // asm 00002F1C: 	CALL	ROADIR			;GET DIRECTIONAL DIFFERENCE
    // asm 00002F1D: 	LDF	*+AR5(CARVROT),R1
    // asm 00002F1E: 	SUBF	R1,R0,R2
    // asm 00002F1F: 	CALL	NORMITS
    // asm 00002F20: 	ABSF	R2,R3			;VELOCITY BACKWARDS?
    // asm 00002F21: 	CMPF	1.75,R3
    // asm 00002F22: 	BLT	BACKCKX		      	;NO...
    // asm 00002F23: BACKCK1
    // asm 00002F23: 	LDF	1.0,R0			;SET RADIAN SPIN COUNT
    // asm 00002F24: 	STF	R0,*+AR5(CARSPRAD)
    // asm 00002F25: 	LDF	0.12,R0			;ROTATE AROUND
    // asm 00002F26: 	LDF	R2,R2
    // asm 00002F27: 	LDFN	-0.12,R0
    // asm 00002F28: 	LDI	1,R1
    // asm 00002F29: BACKCK2
    // asm 00002F29: 	STF	R0,*+AR5(CARDROT)	;SPIN HIM BACK
    // asm 00002F2A: 	STI	R1,*+AR5(CAR_SPIN)
BACKCKX:
    // asm 00002F2B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BACKCK", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CHECK OUT OF BOUNDS
 *PARAMETERS
 *	AR4	CAR
 *	AR5	CAR DATA BLOCK
 *RETURNS
 *	CS IF ANY WHEEL OUT OF BOUNDS
 *	NC IN BOUNDS
 *R0	0 = ONROAD
 *	NE= OFFROAD
 *TRASHES R0,R1,RC
 */
static void CKBND(void) {
    // asm 00002F2C: 	PUSH	AR3
    // asm 00002F2D: 	PUSH	AR0
    // asm 00002F2E: 	LDI	CARVNUM-2,RC	  	;CHECK ALL WHEELS
    // asm 00002F2F: 	LDI	AR5,AR3
    // asm 00002F30: 	ADDI	CARPCOL+CARVSIZ,AR3
    // asm 00002F31: 	LDI	0,R1
    // asm 00002F32: 	RPTB	CURBCKL
    // asm 00002F33: 	LDI	*AR3++(CARVSIZ),R0 	;GET ROAD OBJECT INTERSECTING
    // asm 00002F34: 	BZ	CURBCKX			;WE GOT NOTHING, COLLIDE 'EM
    // asm 00002F35: 	LDI	R0,AR0
    // asm 00002F36: 	LDI	*+AR0(OID),R0		;CHECK OID
    // asm 00002F37: 	AND	CLASS_M,R0
    // asm 00002F38: 	CMPI	ROAD_C,R0		;ROAD/SHOULDER TYPE?
CURBCKL:
    // asm 00002F39: LDINZ	1,R1
    // asm 00002F3A: 	CLRC
    // asm 00002F3B: 	POP	AR0
    // asm 00002F3C: 	POP	AR3
    // asm 00002F3D: 	LDI	R1,R1
    // asm 00002F3E: 	RETS
CURBCKX:
    // asm 00002F3F: 	SETC
    // asm 00002F40: 	POP	AR0
    // asm 00002F41: 	POP	AR3
    // asm 00002F42: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKBND", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CHECK CRAWL UP TUNNEL WALLS
 *PARAMETERS
 *	AR4	CAR OBJECT
 *	AR5	CAR BLOCK
 */
static void TUNCHK(void) {
    // asm 00002F43: 	LDI	@_MODE,R0     		;IN TUNNEL?
    // asm 00002F44: 	TSTB	MINTUNNEL,R0
    // asm 00002F45: 	RETSZ				;NO, EXIT
    // asm 00002F46: 	LDI	@WRECKFLG,R0     	;WRECK ON?
    // asm 00002F47: 	RETSNZ				;NO, EXIT
    // asm 00002F48: 	LDF	*+AR4(OMAT11),R0	;PLAYER TOO STEEP?
    // asm 00002F49: 	CMPF	0.50,R0
    // asm 00002F4A: 	RETSGT				;NO...
    // asm 00002F4B: 	CALL	GETNXTRDIR		;YES, SPIN HIM TO CENTER
    // asm 00002F4C: 	LDF	R0,R2
    // asm 00002F4D: 	CALL	ROADIR
    // asm 00002F4E: 	ADDF	R0,R2
    // asm 00002F4F: 	MPYF	0.5,R2
    // asm 00002F50: 	CALL	NORMITS
    // asm 00002F51: 	STF	R2,*+AR5(CARVROT)
    // asm 00002F52: 	CALL	ROADIR
    // asm 00002F53: 	SUBF	*+AR5(CARYROT),R0
    // asm 00002F54: 	LDF	R0,R2
    // asm 00002F55: 	CALL	NORMITS
    // asm 00002F56: 	LDF	R2,R0
    // asm 00002F57: 	LDF	15,R1
    // asm 00002F58: 	CALL	DIV_F
    // asm 00002F59: 	STF	R0,*+AR5(CARDROT)    	;ROTATE TOWARD ROAD SLOWLY
    // asm 00002F5A: 	LDI	15,R0
    // asm 00002F5B: 	STI	R0,*+AR5(CAR_SPIN)
    // asm 00002F5C:  	LDF	0,R0
    // asm 00002F5D: 	STF	R0,*+AR5(CARSPRAD)
    // asm 00002F5E: 	LDF	*+AR5(CARSPEED),R0
    // asm 00002F5F: 	CMPF	40,R0
    // asm 00002F60: 	LDFLT	40,R0
    // asm 00002F61: 	STF	R0,*+AR5(CARSPEED)
    // asm 00002F62: TUNCHKX
    // asm 00002F62: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TUNCHK", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CHECK CURB COLLISION
 *KEEP ON ROAD
 *PARAMETERS
 *	AR4	CAR
 *	AR5	CAR DATA BLOCK
 */
void INBOUNDZ(void) {
    // asm 00002F63: 	LDI	@_MODE,R1		;WAITING FOR START?
    // asm 00002F64: 	TSTB	MGO,R1
    // asm 00002F65: 	BZ	CURBCLX			;YES, EXIT
    // asm 00002F66: 	CALL	TUNCHK			;KEEP DUDE OFF TUNNEL WALLS
    // asm 00002F67: 	CALL	CKBND
    // asm 00002F68: 	BC	CURBCOL0		;OUT OF BOUNDS
    // asm 00002F69: 	BNE	SOFTCURB		;SOFT REPELL OFFROAD
    // asm 00002F6A: CURBCKLX
    // asm 00002F6A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INBOUNDZ", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *KEEP DRONE INBOUNDS
 *PARAMETERS
 *	AR4	CAR
 *	AR5	CAR DATA BLOCK
 */
void DRONINBZ(void) {
    // asm 00002F6B:  	LDI	*+AR4(ODIST),R0
    // asm 00002F6C: 	ASH	-1,R0
    // asm 00002F6D: 	CMPI	32000,R0		;IS DUDE CLOSE?
    // asm 00002F6E: 	BGT	DRONINBX		;NO, FORGET IT
    // asm 00002F6F: 	LDI	@_MODE,R1		;WAITING FOR START?
    // asm 00002F70: 	TSTB	MGO,R1
    // asm 00002F71: 	BZ	DRONINBX		;YES, EXIT
    // asm 00002F72: 	CALL	CKBND
    // asm 00002F73: 	BC	CURBCOL0		;OUT OF BOUNDS
DRONINBX:
    // asm 00002F74: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONINBZ", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *WE HIT A CURB DUDES
 *HARD WALL REFLECT SPIN-OUT
 */
static void CURBCOL0(void) {
    // asm 00002F75: 	LDI	*+AR4(OID),R0		;PLAYER?
    // asm 00002F76: 	AND	CLASS_M,R0
    // asm 00002F77: 	CMPI	PLYR_C,R0
    // asm 00002F78: 	BZ	CURBCOLP		;YES
    // *DRONE HIT SOUND
    // asm 00002F79: 	LDI	@WALLHITABI,AR2		;MAKE A DRONE SOUND
    // asm 00002F7A: 	LDI	3,R0
    // asm 00002F7B: 	CALL	DRONESND
    // asm 00002F7C: 	LDF	*+AR5(CARSPEED),R0	;SLOW DOWN DRONE
    // asm 00002F7D: 	MPYF	0.80,R0
    // asm 00002F7E: 	STF	R0,*+AR5(CARSPEED)
    // asm 00002F7F: 	B	CURBCOL
    // *PLAYER HIT SOUND
CURBCOLP:
    // asm 00002F80: 	LDI	@WALLHITABI,AR2		;MAKE A SOUND
    // asm 00002F81: 	LDI	3,R0
    // asm 00002F82: 	CALL	RANDSND
CURBCOL:
    // asm 00002F83: 	CALL	GETNXTRDIR    		;DIRECTION CAR TO NEXT ROAD SEG.
    // asm 00002F84: 	LDF	R0,R4
    // asm 00002F85: 	CALL	ROADIR			;REFLECT VELOCITY
    // asm 00002F86: 	SUBF	R4,R0,R2
    // asm 00002F87: 	CALL	NORMITS
    // asm 00002F88: 	LDF	R2,R3
    // asm 00002F89: 	LDF	*+AR5(CARVROT),R1
    // asm 00002F8A: 	SUBF	R1,R0,R2
    // asm 00002F8B: 	CALL	NORMITS
    // *KICK DUDE BACK	INSTANTLY
    // *R0=ROADIR
    // *R2=ROADIR-CARVROT
    // asm 00002F8C: 	PUSHF	R2
    // asm 00002F8D: 	PUSHF	R0
    // asm 00002F8E: 	LDF	*+AR5(CARSPEED),R2
    // asm 00002F8F: 	CMPF	15,R2
    // asm 00002F90: 	LDFLT	15,R2
    // asm 00002F91: 	STF	R2,*+AR5(CARSPEED)	;MIN SPEED
    // asm 00002F92: 	FLOATP	@NFRAMES,R5
    // asm 00002F93: 	MPYF 	R2,R5			;TOTAL DISTANCE TRAVELED
    // asm 00002F94: 	LDF	-1.57,R4
    // asm 00002F95: 	LDF	R3,R3
    // asm 00002F96: 	LDFN	1.57,R4
    // asm 00002F97: 	ADDF	R4,R0,R2		;REPELL DIRECTION
    // asm 00002F98: 	PUSHF	R2
    // asm 00002F99: 	SUBF	R1,R2			;FIND DIFFERENCE
    // asm 00002F9A: 	CALL	_SINE
    // asm 00002F9B: 	ABSF	R0
    // asm 00002F9C: 	MPYF	R0,R5			;ADJUST VELOCITY FOR PENETRATION ANGLE
    // asm 00002F9D: 	POPF	R2
    // asm 00002F9E: 	CALL	_COSI
    // asm 00002F9F: 	MPYF	R0,R5,R4
    // asm 00002FA0: 	CALL	_SINE
    // asm 00002FA1: 	NEGF	R0			;-SIN
    // asm 00002FA2: 	MPYF	R0,R5,R5
    // asm 00002FA3: 	ADDF	*+AR4(OPOSX),R5		;ADJUST X
    // asm 00002FA4: 	STF	R5,*+AR4(OPOSX)
    // asm 00002FA5: 	ADDF	*+AR4(OPOSZ),R4		;ADJUST Z
    // asm 00002FA6: 	STF	R4,*+AR4(OPOSZ)
    // asm 00002FA7: 	CALL	WALL_SPARK
    // asm 00002FA8: 	POPF	R0
    // asm 00002FA9: 	POPF	R2
    // *REFLECT VELOCITY
    // asm 00002FAA: 	XOR	R2,R3			;CHECK IF ALREADY GOING THE RIGHT WAY
    // asm 00002FAB: 	BN	CURBCOL1		;MOVING IN RIGHT DIRECTION
    // asm 00002FAC: 	NEGF	R2			;NEED TO REFLECT VELOCITY
CURBCOL1:
    // asm 00002FAD: 	LDI	*+AR5(CAR_SPIN),R1  	;ALREADY SPINNING?
    // asm 00002FAE: 	BNZ	CURBSPIN		;YES, SPIN SOME MORE...
    // asm 00002FAF: 	ABSF	R2,R3
CURBCOL1A:
    // asm 00002FB0: 	CMPF	1.2,R3
    // asm 00002FB1: 	BGT	CURBSPIN		;YES, SPIN THE DUDE
    // asm 00002FB2: 	CMPF	0.4,R3			;HARD BOUNCE?
    // asm 00002FB3: 	BGT	CURBSPN			;YES, SPIN THE DUDE
    // asm 00002FB4: 	MPYF	0.3,R2			;CUT DOWN BOUNCE
    // asm 00002FB5: 	MPYF	0.3,R3			;CUT DOWN BOUNCE
    // asm 00002FB6: 	CMPF	0.03,R3			;MINIMUM KICKOUT
    // asm 00002FB7: 	BGE	CURBCOL2A
    // asm 00002FB8: 	LDF	R2,R2
    // asm 00002FB9: 	LDFLT	-0.03,R2
    // asm 00002FBA: 	LDFGT	0.03,R2
CURBCOL2A:
    // asm 00002FBB: 	CMPF	0.06,R3			;MAXIMUM KICKOUT
    // asm 00002FBC: 	BLE	CURBCOL2
    // asm 00002FBD: 	LDF	R2,R2
    // asm 00002FBE: 	LDFLT	-0.06,R2
    // asm 00002FBF: 	LDFGT	0.06,R2
CURBCOL2:
    // asm 00002FC0: 	ADDF	R0,R2
    // asm 00002FC1: 	STF	R2,*+AR5(CARVROT)
    // asm 00002FC2: 	STF	R2,*+AR5(CARYROT)	;ADJUST YROT <--- CARVROT
    // asm 00002FC3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CURBCOL0", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
static void CURBSPIN(void) {
    // asm 00002FC4: 	ADDF	R0,R2
    // asm 00002FC5: 	STF	R2,*+AR5(CARVROT)
    // asm 00002FC6: 	LDF	R0,R2
    // asm 00002FC7: 	SUBF	*+AR5(CARYROT),R2	;CHECK YROT-ROAD DIRECTION
    // asm 00002FC8: 	CALL	NORMITS
    // asm 00002FC9: 	LDF	0.025,R0 		;RANDOM ROTATION VALUE
    // asm 00002FCA: 	CALL	FRAND
    // asm 00002FCB: 	ADDF	0.075,R0
    // asm 00002FCC: 	LDF	R2,R2			;CHECK SIGN
    // asm 00002FCD: 	BN	CURBSPIN1
    // asm 00002FCE: 	NEGF	R0
CURBSPIN1:
    // asm 00002FCF: 	STF	R0,*+AR5(CARDROT)	;BODY DELTA
    // asm 00002FD0: 	ABSF	R2			;CORRECTION FACTOR
    // asm 00002FD1: 	ADDF	0.1,R2
    // asm 00002FD2: 	LDF	R2,R1
    // asm 00002FD3: 	CMPF	1.0,R2			;HARD HIT?
    // asm 00002FD4: 	LDFGT	3.14,R1			;YES...
    // asm 00002FD5: 	LDF	*+AR5(CARSPEED),R0
    // asm 00002FD6: 	CMPF	30,R0
    // asm 00002FD7: 	LDFLT	R2,R1
    // asm 00002FD8: 	STF	R1,*+AR5(CARSPRAD) 	;GO AROUND AT LEAST HALFWAY
    // asm 00002FD9: 	LDI	1,R1
    // asm 00002FDA: 	STI	R1,*+AR5(CAR_SPIN)
CURBCLX:
    // asm 00002FDB: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CURBSPIN", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *SHORT CORRECTION SPIN
 */
static void CURBSPN(void) {
    // asm 00002FDC: 	ADDF	R0,R2
    // asm 00002FDD: 	STF	R2,*+AR5(CARVROT)
    // asm 00002FDE: 	LDF	R0,R2
    // asm 00002FDF: 	SUBF	*+AR5(CARYROT),R2	;CHECK YROT-ROAD DIRECTION
    // asm 00002FE0: 	CALL	NORMITS
    // asm 00002FE1: 	ABSF	R2,R3
    // asm 00002FE2: 	STF	R3,*+AR5(CARSPRAD) 	;SPIN THIS MUCH DUDES
    // asm 00002FE3: 	MPYF	0.10,R2
    // asm 00002FE4: 	STF	R2,*+AR5(CARDROT)	;BODY DELTA
    // asm 00002FE5: 	LDI	1,R1
    // asm 00002FE6: 	STI	R1,*+AR5(CAR_SPIN)
    // asm 00002FE7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CURBSPN", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *SOFT CURB HIT
 *
 */
static void SOFTCURB(void) {
    // asm 00002FE8: 	CALL	ROADIR
    // asm 00002FE9: 	LDF	R0,R1
    // asm 00002FEA: 	CALL	GETNXTRDIR    		;DIRECTION CAR TO NEXT ROAD SEG.
    // asm 00002FEB: 	SUBF	R1,R0,R2
    // asm 00002FEC: 	CALL	NORMITS
    // asm 00002FED: 	LDF	R2,R4			;CHECK DIRECTION
    // asm 00002FEE: 	LDFN	-0.1,R5			;GET RELATIVE ANGLE
    // asm 00002FEF: 	LDFNN	0.1,R5
    // asm 00002FF0: 	LDI	*+AR5(CAR_SPIN),R0  	;ALREADY SPINNING?
    // asm 00002FF1: 	BZ	SOFTCURB0		;NO
    // asm 00002FF2: 	CMPI	2,R0			;TIMED SPIN?
    // asm 00002FF3: 	BGE	SOFTCRB00		;YES
    // asm 00002FF4: 	LDF	*+AR5(CARVROT),R0	;REFLECT THE MOTHER OUT
    // asm 00002FF5: 	SUBF	R1,R0,R2
    // asm 00002FF6: 	CALL	NORMITS
    // asm 00002FF7: 	ABSF	R2
    // asm 00002FF8: 	CMPF	0.1,R2	      		;MINIMUM REFLECT
    // asm 00002FF9: 	LDFLT	0.1,R2
    // asm 00002FFA: 	LDF	R4,R4
    // asm 00002FFB: 	LDFN	-1,R5			;GET RELATIVE ANGLE
    // asm 00002FFC: 	LDFNN	1,R5
    // asm 00002FFD: 	MPYF	R5,R2
    // asm 00002FFE: 	ADDF	R1,R2
    // asm 00002FFF: 	STF	R2,*+AR5(CARVROT)
    // asm 00003000: 	RETS
SOFTCRB00:
    // asm 00003001: 	LDF	*+AR5(CARSPEED),R0	;TIMED SPIN
    // asm 00003002: 	CMPF	80,R0
    // asm 00003003: 	BLT	SOFTVELX		;SLOW TREECOL FIX
    // asm 00003004: 	B	SOFTVEL			;FAST, NEEDS CORRECTION
    // *CHECK BODY ROTATION
SOFTCURB0:
    // asm 00003005: 	LDF	*+AR5(CARYROT),R0
    // asm 00003006: 	SUBF	R1,R0,R2
    // asm 00003007: 	CALL	NORMITS
    // asm 00003008: 	LDF	R4,R4
    // asm 00003009: 	BN	SOFT10	     		;DELTA IS NEGATIVE
    // asm 0000300A: 	SUBF	R2,R5,R6
    // asm 0000300B: 	BLT	SOFTVEL			;ROTATION IS O.K.
    // asm 0000300C: 	CMPF	0.1,R6			;MAX DELTA
    // asm 0000300D: 	LDFGT	0.1,R6
    // asm 0000300E: 	B 	SOFT11
SOFT10:
    // asm 0000300F: 	SUBF	R2,R5,R6
    // asm 00003010: 	BGT	SOFTVEL			;ROTATION IS O.K.
    // asm 00003011: 	CMPF	-0.1,R6			;MAX DELTA
    // asm 00003012: 	LDFLT	-0.1,R6
SOFT11:
    // asm 00003013: 	ADDF	*+AR5(CARYROT),R6  	;ADD IN ROTATION
    // asm 00003014: 	STF	R6,*+AR5(CARYROT)
    // *CHECK VELOCITY ROTATION
SOFTVEL:
    // asm 00003015: LDF	*+AR5(CARSPEED),R0	;MINIMUM SPEED
    // asm 00003016: 	CMPF	20,R0
    // asm 00003017: 	LDFLT	20,R0
    // asm 00003018: 	STF	R0,*+AR5(CARSPEED)
    // asm 00003019: 	LDF	*+AR5(CARVROT),R0
    // asm 0000301A: 	SUBF	R1,R0,R2
    // asm 0000301B: 	CALL	NORMITS
    // asm 0000301C: 	ABSF	R2,R3
    // asm 0000301D: 	CMPF	0.6,R3
    // asm 0000301E: 	BLT	SOFTV1
    // asm 0000301F: 	LDF	R1,R0			;ROADIR->R0
    // asm 00003020: 	LDF	*+AR5(CARVROT),R1
    // asm 00003021: 	SUBF	R1,R0,R2
    // asm 00003022: 	B	CURBCOL1A
SOFTV1:
    // asm 00003023: LDF	R4,R4
    // asm 00003024: 	BN	SOFT20	     		;DELTA IS NEGATIVE
    // asm 00003025: 	SUBF	R2,R5
    // asm 00003026: 	BLT	SOFTVELX		;ROTATION IS O.K.
    // asm 00003027: 	CMPF	0.1,R5			;MAX DELTA
    // asm 00003028: 	LDFGT	0.1,R5
    // asm 00003029: 	B 	SOFT21
SOFT20:
    // asm 0000302A: SUBF	R2,R5
    // asm 0000302B: 	BGT	SOFTVELX		;ROTATION IS O.K.
    // asm 0000302C: 	CMPF	-0.1,R5			;MAX DELTA
    // asm 0000302D: 	LDFLT	-0.1,R5
SOFT21:
    // asm 0000302E: ADDF	*+AR5(CARVROT),R5  	;ADD IN ROTATION
    // asm 0000302F: 	STF	R5,*+AR5(CARVROT)
SOFTVELX:
    // asm 00003030: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SOFTCURB", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *GET DIRECTION TO NEXT ROAD SEGMENT FROM CAR
 *
 */
void GETNXTRDIR(void) {
    // asm 00003031: 	LDI	*+AR5(CARTRAK),AR2	;GET CLOSEST TRACK PIECE
    // asm 00003032: 	LDI	*+AR2(OLINK4),AR0	;GET NEXT ONE
    // asm 00003033: 	LDI	AR4,AR2
    // asm 00003034: 	B 	GETRD1
    // *GET ROAD-CAR ANGLE
    // *AR2=ROAD SEGMENT OBJECT
    // *AR4=CAR OBJECT
    // *RET R0 = RADIANS (FLOAT)
    // *CLOBBERS AR0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETNXTRDIR", 0, 0);
    UNIMPL();
}

static void GETRDCAR(void) {
    // asm 00003035: 	LDI	AR4,AR0
    // asm 00003036: 	B 	GETRD1
    // *GET ROAD DIRECTION
    // *AR5=CARBLOCK
    // *RET R0 = RADIANS (FLOAT)
    // *CLOBBERS AR0,AR2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETRDCAR", 0, 0);
    UNIMPL();
}

float ROADIR(OBJ* track_obj /*AR2*/) {
    // asm 00003037: 	LDI	*+AR5(CARTRAK),R0	;GET CLOSEST TRACK PIECE
    // asm 00003038: 	BZ	ROADIRX			;NO TRACK PIECE, EXIT
    // asm 00003039: 	LDI	R0,AR2
    // *GET ROAD DIRECTION
    // *AR2=ROAD SEGMENT OBJECT
    // *RET R0 = RADIANS (FLOAT)
    // *CLOBBERS AR0
    if (track_obj == NULL) {
        return 0.0f;
    }

    return GETRDIR(track_obj);
}

float GETRDIR(OBJ* track_obj /*AR2*/) {
    OBJ* next_track_obj;

    // asm 0000303A: 	LDI	*+AR2(OLINK4),AR0
    next_track_obj = (OBJ*)track_obj->link4;
    if (next_track_obj == NULL) {
        return 0.0f;
    }
GETRD1:
    // asm 0000303B: PUSH	R2
    // asm 0000303C: 	PUSH	R3
    // asm 0000303D: 	PUSHF	R2
    // asm 0000303E: 	PUSHF	R3
    // asm 0000303F: 	LDF	*+AR0(OPOSX),R2
    // asm 00003040: 	SUBF	*+AR2(OPOSX),R2
    // asm 00003041: 	LDF	*+AR0(OPOSZ),R3
    // asm 00003042: 	SUBF	*+AR2(OPOSZ),R3
    // asm 00003043: 	CALL	ARCTANF
    // asm 00003044: 	POPF	R3
    // asm 00003045: 	POPF	R2
    // asm 00003046: 	POP	R3
    // asm 00003047: 	POP	R2
    // asm 00003048: 	SUBF	HALFPI,R0
ROADIRX:
    // asm 00003049: RETS
    return atan2f(next_track_obj->posz - track_obj->posz, next_track_obj->posx - track_obj->posx) - HALFPI;
}

// *----------------------------------------------------------------------------
/* asm: WHLTIM	.bss	WHLTIM,1 */
int WHLTIM;
/* asm: WHLOLD	.bss	WHLOLD,1 */
int WHLOLD;

/*
 *PLAYER WHEEL ROUTINE
 *PARAMETERS
 *	AR4	OBJECT
 *	AR5	CAR STRUCTURE
 */
static void PLYRWHL(void) {
    // asm 0000304A: 	LDF	@STEERCT,R4		;STEERING CENTER
    // asm 0000304B: 	LDI	@WHLTIM,R3
    // asm 0000304C: 	LDI	@WHLOLD,R5
    // asm 0000304D: 	LDI	0,R2
    // asm 0000304E: 	LDI	0,R1
    // asm 0000304F: 	LDI	*+AR5(RF_PCOL),R0	;Right front collide with curb?
    // asm 00003050: 	BZ	PWHL0
    // asm 00003051: 	LDI	R0,AR0
    // asm 00003052: 	LDI	*+AR0(OID),R0
    // asm 00003053: 	AND	CLASS_M+TYPE_M,R0
    // asm 00003054: 	CMPI	ROAD_C+SHLDR_T,R0
    // asm 00003055: 	LDIZ	1,R1
PWHL0:
    // asm 00003056: LDI	*+AR5(LF_PCOL),R0	;Left front collide with curb?
    // asm 00003057: 	BZ	PWHL1
    // asm 00003058: 	LDI	R0,AR0
    // asm 00003059: 	LDI	*+AR0(OID),R0
    // asm 0000305A: 	AND	CLASS_M+TYPE_M,R0
    // asm 0000305B: 	CMPI	ROAD_C+SHLDR_T,R0
    // asm 0000305C: 	LDIZ	2,R2
PWHL1:
    // asm 0000305D: ADDI	R1,R2
    // asm 0000305E: 	STI	R2,@WHLOLD
    // asm 0000305F: 	BZ	PWHLX			;NO SHOULDER ACTIVE
    // asm 00003060: 	ANDN	R5,R2			;NEW SHOULDER HIT?
    // asm 00003061: 	BZ	PWHLX			;NOT NEW...
    // *COLLIDED WITH A CURB FIRST TIME
    // asm 00003062: 	CALL	ROADIR			;FIND WHICH SIDE OF ROAD HIT
    // asm 00003063: 	LDF	R0,R2
    // asm 00003064: 	CALL	GETNXTRDIR
    // asm 00003065: 	SUBF	R0,R2
    // asm 00003066: 	CALL 	NORMITS
    // asm 00003067: 	LDF	R2,R2
    // asm 00003068: 	LDFNN	15,R0
    // asm 00003069: 	LDFN	-15,R0
    // asm 0000306A: 	ADDF	R0,R4			;NEW STERRING CENTER FOR FEEDBACK
    // asm 0000306B: 	LDI	4,R3			;NEW TIMER VALUE
    // asm 0000306C: 	B	PWHLX0
PWHLX:
    // asm 0000306D: SUBI	1,R3
    // asm 0000306E: 	LDILT	0,R3
    // asm 0000306F: 	BGT	PWHLXX			;When timer reaches -1, Set WHEELPOS = STEERCT
    // *THIS POINT IS ONLY REACHED WHEN NOT COLLIDING WITH A CURB
    // *PLACE SKID CORRECT HERE
PWHLX0:
    // asm 00003070: STF	R4,@WHEELPOS
PWHLXX:
    // asm 00003071: STI	R3,@WHLTIM
    // *CHECK OFF ROAD STUFF
    // asm 00003072: 	LDI	@WHLOLD,R1  		;OFFROAD?
    // asm 00003073: 	BZ	WHLOFFX			;NO
    // asm 00003074: 	LDI	@WHLTIM,R3		;TIMER ON?
    // asm 00003075: 	BNZ	WHLOFFX			;NO
    // asm 00003076: 	LDF	@STEERCT,R4		;STEERING CENTER
    // asm 00003077: 	LDF	R4,R5
    // asm 00003078: 	ADDF	15,R4
    // asm 00003079: 	SUBF	15,R5
    // asm 0000307A: 	LDF	2,R0
    // asm 0000307B: 	CMPI	3,R1
    // asm 0000307C: 	LDFZ	4,R0
    // asm 0000307D: 	CALL	SFRAND			;If off road offset center by random number
    // asm 0000307E: 	ADDF	@WHEELPOS,R0
    // asm 0000307F: 	CMPF	R4,R0		 	;CHECK YOUR LIMITS
    // asm 00003080: 	LDFGT	R4,R0
    // asm 00003081: 	CMPF	R5,R0
    // asm 00003082: 	LDFLT	R5,R0
    // asm 00003083: 	STF	R0,@WHEELPOS
WHLOFFX:
    // asm 00003084: RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRWHL", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *SOUND TABLES
 *PLAYER COLLISION SOUND TABLE
 */
/* asm: SCOLLTAB	.word	SCOLLA,SCOLLB,SCOLLC */
static int SCOLLTAB[] = {
    SCOLLA,
    SCOLLB,
    SCOLLC,
};
// *WALL HIT SOUND TABLE
/* asm: WALLHITAB	.word	WALLHITA,WALLHITB,WALLHITC */
static int WALLHITAB[] = {
    WALLHITA,
    WALLHITB,
    WALLHITC,
};
// *SKID SOUND TABLE
/* asm: SKIDTAB		.word	SKIDB,SKIDC */
/* asm: 	 */
static int SKIDTAB[] = {
    SKIDB,
    SKIDC,
};
/* asm: PLAIRSND	.word 	RH_BABEWHOA,GL_WOOLAUGH,CHICKSCREAM */
/* asm: 	 */
static int PLAIRSND[] = {
    RH_BABEWHOA,
    GL_WOOLAUGH,
    CHICKSCREAM,
};
/* asm: REVSNDTAB	.word	SINGLEREV5,SINGLEREV6 */
/* asm: 	 */
static int REVSNDTAB[] = {
    SINGLEREV5,
    SINGLEREV6,
};

/*
;SHIFTSNDTABI	.WORD	SHIFTSNDTAB
;SHIFTSNDTAB	.WORD	FIRSTSND,SECONDSND,THIRDSND,FOURTHSND
*----------------------------------------------------------------------------
*PLAYER SOUND EFFECTS TRACKS
*TRACK 0 = MUSIC
*TRACK 1 = DRONE+PLAYER EFFECTS
*TRACK 2 = DRONE+PLAYER EFFECTS
*TRACK 3 = ENGINE
*PARAMETERS
*	AR4	PLYR CAR OBJECT
*	AR5	CAR BLOCK
*/
static void PLYR_SNDS(void) {
    // asm 00003097: 	LDI	@_MODE,R0
    // asm 00003098: 	AND	MMODE,R0
    // asm 00003099: 	CMPI	MGAME,R0
    // asm 0000309A: 	RETSNE
    // asm 0000309B: 	CALL	GETPEDAL
    // asm 0000309C: 	CMPF	0.75,R0
    // asm 0000309D: 	BLT	PSND1
    // asm 0000309E: 	LDI	@_MODE,R2
    // asm 0000309F: 	AND	MSLINE,R2
    // asm 000030A0: 	BZ	PSND0
    // asm 000030A1: 	CREATEC	FLAME_PRC,UTIL_C	;make child flames
PSND0:
    // asm 000030A4: 	LDI	@REVFLG,R0
    // asm 000030A5: 	BNE	BACKREV
    // asm 000030A6: 	CREATEC	FLAME_PRC,UTIL_C	;make child flames
    // *MAKE YOUR REV SOUND
    // *KILL OLDIES
    // asm 000030A9: 	LDI	2,R0
    // asm 000030AA: 	LDI	@REVSNDTABI,AR2 	;REVSND ON TRACK 12
    // asm 000030AB: 	CALL	RANDSND
    // asm 000030AC: 	LDI	1,R0
    // asm 000030AD: 	B	PSND2
PSND1:
    // asm 000030AE: 	CMPF	0.50,R0
    // asm 000030AF: 	LDIGE	@REVFLG,R0
    // asm 000030B0: 	LDILT	0,R0
PSND2:
    // asm 000030B1: 	STI	R0,@REVFLG
    // *CHECK BACKGROUND STARTING LINE REV SOUNDS
BACKREV:
    // asm 000030B2: 	LDI	@_MODE,R2 		;STARTING LINE MODE?
    // asm 000030B3: 	AND	MSLINE,R2
    // asm 000030B4: 	BZ	PLAIR			;NOPE
    // asm 000030B5: 	LDI	@SNDSTR+SND_SIZ+SND_PRI,R2	;CHECK TRACK1
    // asm 000030B6: 	BZ	BACKREV1
    // asm 000030B7: 	LDI	@SNDSTR+(2*SND_SIZ)+SND_PRI,R2	;CHECK TRACK2
    // asm 000030B8: 	BNZ	PLAIR
BACKREV1:
    // asm 000030B9: 	SONDFX	STARTLINEREVS2		     	;START REV WHEN NOTHING
    // *CHECK IN AIR SPEECH CALL
PLAIR:
    // *CHECK IN AIR SPEECH CALL
    // asm 000030BB: 	LDI	@WRECKFLG,R0			;IGNORE AIR SOUND ON WRECK
    // asm 000030BC: 	BNZ	PLYRSND1B
    // asm 000030BD: 	LDI	@PLAIRTIM,R0
    // asm 000030BE: 	BNZ	PLYRSND1A
    // asm 000030BF: 	LDF	*+AR5(CT_PRDYD),R0
    // asm 000030C0: 	CMPF	200,R0
    // asm 000030C1: 	BLT	PLYRSND1B
    // asm 000030C2: 	LDI	@PLAIRSNDI,AR2
    // asm 000030C3: 	LDI	3,R0
    // asm 000030C4: 	CALL	RANDSND
    // asm 000030C5: 	LDI	100,R0
PLYRSND1A:
    // asm 000030C6: 	SUBI	1,R0
    // asm 000030C7: 	STI	R0,@PLAIRTIM
PLYRSND1B:
    // *CHECK BOTTOM OUT SOUND
    // asm 000030C8: 	LDI	*+AR5(CAR_AIRB),R0
    // asm 000030C9: 	BNZ	NOBOTTOM
    // asm 000030CA: 	LDI	@OLDPLYAIR,R0
    // asm 000030CB: 	BZ	BOTX
    // asm 000030CC: 	CALL	INIT_SPARK
    // asm 000030CD: 	SONDFX	BOTTOMOUT
    // asm 000030CF: 	LDI	0,R0
NOBOTTOM:
    // asm 000030D0: 	STI	R0,@OLDPLYAIR
BOTX:
    // *CHECK SKID SOUND
    // asm 000030D1: 	LDF	*+AR5(CARSKID),R0
    // asm 000030D2: 	CMPF	0.5,R0
    // asm 000030D3: 	BLT	NO_SMOKE
    // asm 000030D4: 	CREATEC	SMOKE_PROC,UTIL_C	;make child smoke
NO_SMOKE:
    // asm 000030D7: 	LDF	*+AR5(CARSKID),R0
    // asm 000030D8: 	CMPF	0.25,R0
    // asm 000030D9: 	BLT	NOSKID			;NO SKID ACTIVE
    // asm 000030DA: 	FLOAT	115,R1
    // asm 000030DB: 	MPYF	R0,R1			;GET SKID SOUND AMPLITUDE
    // asm 000030DC: 	FIX	R1			;CONVERT TO INT
    // asm 000030DD: 	ADDI	140,R1
    // *CHECK ALREADY ACTIVE
    // asm 000030DE: 	LDI	@SNDSTR+SND_SIZ+SND_IDX,R2	;CHECK TRACK1
    // asm 000030DF: 	CMPI	SKIDB,R2
    // asm 000030E0: 	BEQ	SKIDAMP				;ALREADY SKIDDING
    // asm 000030E1: 	CMPI	SKIDC,R2
    // asm 000030E2: 	BEQ	SKIDAMP				;ALREADY SKIDDING
    // asm 000030E3: 	LDI	@SNDSTR+(2*SND_SIZ)+SND_IDX,R2	;CHECK TRACK2
    // asm 000030E4: 	CMPI	SKIDB,R2
    // asm 000030E5: 	BEQ	SKIDAMP1		  	;ALREADY SKIDDING
    // asm 000030E6: 	CMPI	SKIDC,R2
    // asm 000030E7: 	BEQ	SKIDAMP1			;ALREADY SKIDDING
    // *MAKE NEW SKID SOUND
    // asm 000030E8: 	LDF	*+AR5(CARSPEED),R0
    // asm 000030E9: 	CMPF	20,R0
    // asm 000030EA: 	BGT	NO_FLAME
    // asm 000030EB: 	CREATEC	FLAME_PRC,UTIL_C	;make child flames
NO_FLAME:
    // asm 000030EE: 	LDI	@SKIDTABI,AR2
    // asm 000030EF: 	LDI	2,R0
    // asm 000030F0: 	CALL	RANDVSND   	       		;START A SKID AND EXIT
    // asm 000030F1: 	B	SKIDX
SKIDAMP:
    // asm 000030F2: 	LDI	@SNDSTR+(SND_SIZ)+SND_VOL,R2	;VOLUME CHANGE?
    // asm 000030F3: 	LDI	1,R0
    // asm 000030F4: 	B	SKIDAMP10
SKIDAMP1:
    // asm 000030F5: 	LDI	@SNDSTR+(2*SND_SIZ)+SND_VOL,R2	;VOLUME CHANGE?
    // asm 000030F6: 	LDI	2,R0
SKIDAMP10:
    // asm 000030F7: 	SUBI	R1,R2
    // asm 000030F8: 	ABSI	R2
    // asm 000030F9: 	CMPI	10,R2				;NOT ENOUGH CHANGE FOR CALL
    // asm 000030FA: 	BLT	SKIDX
    // asm 000030FB: 	CALL	SET_TRACK_VOL 			;ADJUST YOUR VOLUME, DUDES
    // asm 000030FC: 	B	SKIDX
NOSKID:
    // asm 000030FD: 	LDI	SKIDB,AR2
    // asm 000030FE: 	CALL	KILLSNDFX
    // asm 000030FF: 	LDI	SKIDC,AR2
    // asm 00003100: 	CALL	KILLSNDFX
SKIDX:
    // *CHECK HARD BRAKE SOUND
    // asm 00003101: 	LDF	*+AR5(CARBRAKE),R0
    // asm 00003102: 	CMPF	0.8,R0
    // asm 00003103: 	BLE	NOBRAKSND
    // asm 00003104: 	LDF	*+AR5(CARSPEED),R0     	;SPEED LOW  ?
    // asm 00003105: 	CMPF	1.5,R0
    // asm 00003106: 	BLT	NOBRAKSND	       	;YES KILL SOUND
    // asm 00003107: 	CREATEC	SMOKE_PROC,UTIL_C	;make child smoke
    // asm 0000310A: 	LDI	BRAKSND,AR2
    // asm 0000310B: 	CALL	MKFXSND
    // asm 0000310C: 	B	BRAKSNDX
NOBRAKSND:
    // asm 0000310D: 	LDI	BRAKSND,AR2
    // asm 0000310E: 	CALL	KILLSNDFX
BRAKSNDX:
    // *ENGINE SPUTTER SOUND
    // asm 0000310F: 	LDF	*+AR5(CARTHROTTLE),R0
    // asm 00003110: 	CMPF	0.15,R0
    // asm 00003111: 	BGE	NOSPUTSND
    // asm 00003112: 	LDF	*+AR5(CARRPM),R0
    // asm 00003113: 	CMPF	30,R0
    // asm 00003114: 	BLE	NOSPUTSND
    // asm 00003115: 	LDI	@_MODE,R2
    // asm 00003116: 	AND	MSLINE,R2
    // asm 00003117: 	BNZ	NOSPUTSND
    // asm 00003118: 	LDI	SPUTSND,AR2
    // asm 00003119: 	CMPI	@SNDSTR+2*(SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm 0000311A: 	BZ	SPUTSNDX
    // asm 0000311B: 	CALL	ONESND
    // asm 0000311C: 	B	SPUTSNDX
NOSPUTSND:
    // asm 0000311D: 	LDI	SPUTSND,AR2
    // asm 0000311E: 	CALL	KILLSNDFX
SPUTSNDX:
    // *ROAD EFFECTS: TUNNEL SOUND
    // asm 0000311F: 	LDI	@_MODE,R4
    // asm 00003120: 	TSTB	MINTUNNEL,R4
    // asm 00003121: 	BZ	TUNOFF
    // asm 00003122: 	FIX	*+AR5(CARSPEED),R0
    // asm 00003123: 	CMPI	100,R0		 		;CLAMP TO LIMITS
    // asm 00003124: 	LDILT	100,R0
    // asm 00003125: 	CMPI	255,R0
    // asm 00003126: 	LDIGT	255,R0
    // asm 00003127: 	LDI	TUNSND,AR2
    // asm 00003128: 	CALL	MKVFXSND
    // asm 00003129: 	B	TUNSNDX
TUNOFF:
    // asm 0000312A: LDI	TUNSND,AR2
    // asm 0000312B: 	CALL	KILLSNDFX
TUNSNDX:
    // *ROAD EFFECTS: GRAVEL SOUND
    // asm 0000312C: 	LDI	*+AR5(LR_PCOL),AR2
    // asm 0000312D: 	LDI	0,R1
    // asm 0000312E: 	LDI	0,R2
    // asm 0000312F: 	LDI	*+AR2(OID),R0
    // asm 00003130: 	CMPI	300h,R0
    // asm 00003131: 	LDINZ	127,R1
    // asm 00003132: 	LDI	*+AR5(RR_PCOL),AR2
    // asm 00003133: 	LDI	*+AR2(OID),R0
    // asm 00003134: 	CMPI	300h,R0
    // asm 00003135: 	LDINZ	127,R2
    // asm 00003136: 	ADDI	R1,R2,R0
    // asm 00003137: 	BZ	NOGRAV
    // asm 00003138: GRAVEL
    // asm 00003138: 	LDF	*+AR5(CARSPEED),R1
    // asm 00003139: 	CMPF	1.0,R1
    // asm 0000313A: 	BLT	NOGRAV
    // asm 0000313B: 	LDI	GRAVELA,AR2
    // asm 0000313C: 	B	MKVFXSND
NOGRAV:
    // asm 0000313D: 	LDI	GRAVELA,AR2
    // asm 0000313E: 	CALL	KILLSNDFX
    // asm 0000313F: 	BNC	GRAVX
    // asm 00003140: 	SONDFX  BOTTOMOUT
GRAVX:
    // asm 00003142: PLSNDX
    // asm 00003142: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_SNDS", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *MAKE PLAYER EFFECTS SOUND
 *PARAMETERS
 *	AR2	SOUND
 */
void MKFXSND(void) {
    // asm 00003143: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm 00003144: 	RETSZ
    // asm 00003145: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm 00003146: 	BNZ	ONESNDFX
    // asm 00003147: MKFXSNDX
    // asm 00003147: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MKFXSND", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *MAKE PLAYER VOLUME EFFECTS BACKGROUND SOUND
 *PARAMETERS
 *	AR2	SOUND
 *	 R0	VOLUME
 */
static void MKVFXSND(void) {
    // asm 00003148: 	LDI	R0,R1				;SAVE VOLUME
    // asm 00003149: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm 0000314A: 	BNZ	MKVFX1
    // asm 0000314B: 	SUBPI	@SNDSTR+SND_SIZ+SND_VOL,R0	;CHECK TRACK1 VOLUME
    // asm 0000314C: 	ABSI	R0
    // asm 0000314D: 	CMPI	8,R0
    // asm 0000314E: 	RETSLE
    // asm 0000314F: 	LDI	1,R0	  	;TRACK #
    // asm 00003150: 	B	SET_TRACK_VOL	;R0=TRACK#,R1=VOL
MKVFX1:
    // asm 00003151: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm 00003152: 	BNZ	VOLSNDFX		     	;DO NEW SOUND DUDES
    // asm 00003153: 	SUBI	@SNDSTR+(2*SND_SIZ)+SND_VOL,R0	;CHECK TRACK2 VOLUME
    // asm 00003154: 	ABSI	R0
    // asm 00003155: 	CMPI	8,R0
    // asm 00003156: 	RETSLE
    // asm 00003157: 	LDI	2,R0	  	;TRACK #
    // asm 00003158: 	B	SET_TRACK_VOL	;R0=TRACK#,R1=VOL
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MKVFXSND", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *RANDOM SOUND ROUTINE
 *PARAMETERS
 *	R0	RANDOM RANGE (0->R0-1)
 *	AR2	TABLE ADDR	OF SOUNDS
 *TRASHED R0
 */
void RANDSND(void) {
    // asm 00003159: 	PUSH	AR2
    // asm 0000315A: 	LDI	R0,AR2
    // asm 0000315B: 	CALL	RANDU0
    // asm 0000315C: 	POP	AR2
    // asm 0000315D: 	ADDI	R0,AR2
    // asm 0000315E: 	LDI	*AR2,AR2
    // asm 0000315F: 	B	ONESNDFX
    // *RANDOM VOLUME SOUND ROUTINE
    // *PARAMETERS
    // *	R0	RANDOM RANGE (0->R0-1)
    // *	R1	VOLUME
    // *	AR2	TABLE ADDR	OF SOUNDS
    // *TRASHED R0,AR2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDSND", 0, 0);
    UNIMPL();
}

void RANDVSND(void) {
    // asm 00003160: 	PUSH	AR2
    // asm 00003161: 	LDI	R0,AR2
    // asm 00003162: 	CALL	RANDU0
    // asm 00003163: 	POP	AR2
    // asm 00003164: 	ADDI	R0,AR2
    // asm 00003165: 	LDI	*AR2,AR2
    // asm 00003166: 	LDI	R1,R0
    // asm 00003167: 	B	VOLSNDFX     		;R0=VOLUME, AR2=SOUND
    // *DRONE SOUND ROUTINE
    // *PARAMETERS
    // *	R0	RANDOM RANGE (0->R0-1)
    // *	AR2	TABLE ADDR	OF SOUNDS
    // *	AR4	DRONE CAR OBJECT
    // *COMPUTES VOLUME BASED ON DISTANCE
    // *TRASHED R0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RANDVSND", 0, 0);
    UNIMPL();
}

void DRONESND(void) {
    // asm 00003168: 	PUSH	AR2
    // asm 00003169: 	LDI	R0,AR2
    // asm 0000316A: 	CALL	RANDU0
    // asm 0000316B: 	POP	AR2
    // asm 0000316C: 	ADDI	R0,AR2
    // asm 0000316D: 	LDI	*AR2,AR2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONESND", 0, 0);
    UNIMPL();
}

void DRONESND1(void) {
    // asm 0000316E: 	FLOAT	*+AR4(ODIST),R0
    // asm 0000316F: 	RETSN				;BEHIND PLAYER NO SOUND
    // asm 00003170: 	FLOAT	10000,R1
    // asm 00003171: 	MPYF	5,R1
    // asm 00003172: 	CALL	DIV_F
    // asm 00003173: 	SUBRF	1,R0
    // asm 00003174: 	RETSN
    // asm 00003175: 	FLOAT	220,R1
    // asm 00003176: 	MPYF	R1,R0
    // asm 00003177: 	FIX	R0
    // asm 00003178: 	CMPI	220,R0
    // asm 00003179: 	LDIGT	220,R0
    // asm 0000317A: 	B	VOLSNDFX     		;R0=VOLUME, AR2=SOUND
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONESND1", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *STEERING WHEEL AND GAS PEDAL PARAMETERS
 *(VALUES ARE READ FROM CMOS AND COPIED INTO RAM FOR EASY ACCESS)
 *(COPIED AT THE START OF EACH GAME)
 *ALL FLOATS
 */
/* asm: PEDALMN	.bss	PEDALMN,1 */
float PEDALMN = 1.0f;
/* asm: PEDALMX	.bss	PEDALMX,1 */
float PEDALMX = 1.0f;
/* asm: STEERMN	.bss	STEERMN,1 */
float STEERMN = 1.0f;
/* asm: STEERMX	.bss	STEERMX,1 */
float STEERMX = 1.0f;
/* asm: STEERCT	.bss	STEERCT,1 */
float STEERCT = 1.0f;
/* asm: BRAKEMN	.bss	BRAKEMN,1 */
float BRAKEMN = 1.0f;
/* asm: BRAKEMX	.bss	BRAKEMX,1 */
float BRAKEMX = 1.0f;
/* asm: STEERFR	.bss	STEERFR,1 */
float STEERFR = 1.0f;
#define ADJ_COINMODE 0
#define ADJ_GASMIN 1
#define ADJ_GASMAX 2
#define ADJ_STEERMIN 3
#define ADJ_STEERMAX 4
#define ADJ_STEERCENTER 5
#define ADJ_BRAKEMIN 6
#define ADJ_BRAKEMAX 7

void GETCMOS_VALUES(void) {
    // asm 0000317C: 	PUSH	AR3
    // asm 0000317D: 	LDI	@PEDALMNI,AR3
    // asm 0000317E: 	LDI	ADJ_GASMIN,AR2
    // asm 0000317F: 	LDI	6,RC
    // asm 00003180: 	RPTB	CMOSALP
    // asm 00003181: 	PUSH	AR2
    // asm 00003182: 	CALL	ADJUSTMENT_READ
    // asm 00003183: 	POP	AR2
    // asm 00003184: 	FLOAT	R0
    // asm 00003185: 	STF	R0,*AR3++
CMOSALP:
    // asm 00003186: ADDI	1,AR2
    // asm 00003187: 	LDF	@STEERMX,R0
    // asm 00003188: 	SUBF	@STEERMN,R0
    // asm 00003189: 	STF	R0,@STEERFR
    // asm 0000318A: 	POP	AR3
    // asm 0000318B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCMOS_VALUES", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *PUSH CAMERA MATRIX
 */
static void CAMMATSAV(void) {
    // asm 0000318C: 	POP	BK
    // asm 0000318D:      	LDI	SP,AR0
    // asm 0000318E: 	ADDI	9,SP
    // asm 0000318F: 	LDI	@CAMERAMATRIXI,AR1
    // asm 00003190: 	LDF	*AR1++,R0
    // asm 00003191: 	RPTS	7
    // asm 00003192: 	LDF	*AR1++,R0
    // asm 00003192: ||	STF	R0,*++AR0
    // asm 00003193: 	STF	R0,*++AR0
    // asm 00003194: 	B	BK
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMMATSAV", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *CAMERA MATRIX AVERAGE
 */
static void CAMMATAVG(void) {
    // asm 00003195: 	POP	BK
    // asm 00003196: 	LDI	*+AR5(CAR_SPIN),R0	;DONT AVG IN SPIN DUDES
    // asm 00003197: 	CMPI	1,R0
    // asm 00003198: 	BEQ	CAMMATX
    // asm 00003199:      	LDI	SP,AR1
    // asm 0000319A: 	SUBI	8,AR1
    // asm 0000319B: 	LDI	@CAMERAMATRIXI,AR0
    // asm 0000319C: 	LDI	8,RC
    // asm 0000319D: 	RPTB	CAMAVG
    // asm 0000319E: 	LDF	*AR1++,R0
    // asm 0000319F: 	MPYF	0.80,R0
    // asm 000031A0: 	LDF	*AR0,R1
    // asm 000031A1: 	MPYF	0.20,R1
    // asm 000031A2: 	ADDF	R0,R1
CAMAVG:
    // asm 000031A3: STF	R1,*AR0++
CAMMATX:
    // asm 000031A4: SUBI	9,SP
    // asm 000031A5: 	B	BK
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMMATAVG", 0, 0);
    UNIMPL();
}

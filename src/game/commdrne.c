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
#include "text.h"
#include "delta.h"
#include "comm.h"
#include "racer.h"
#include "commdrne.h"

/*
 * Source module: asm/COMMDRNE.ASM
 */

void COMM_DRONE(void);
void COMM_DRONE_PTR_SORT(void);
void OM_DRONE(void);

#define STARTUP_POS_TABLEI STARTUP_POS_TABLE

static float STARTUP_POS_TABLE[78];

/*
*----------------------------------------------------------------------------
*
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

/*
*----------------------------------------------------------------------------
*Startup Position Table
*
*39 words per plyr (13*3)
*
*
*/
/* asm: STARTUP_POS_TABLE:	;PLYR1  (on PLYR2) */
/* asm: 	.float	576,-180,3287			;GG */
/* asm: 	.float	-1928700,4654,584166		;SF */
/* asm: 	.float	-2611877,14000,-628750		;H280 */
/* asm: 	.float	-3277335,15063,-1078194		;RW */
/* asm: 	.float	-2257363,21013,-1158765		;BH */
/* asm: 	.float	-2426363,10814,-2164529		;LA */
/* asm: 	.float	-4120644,20316,-3599815		;DV */
/* asm: 	.float	-4073652,24315,-3414549		;AZ */
/* asm: 	.float	-2802001,12395,-3353785		;GC */
/* asm: 	.float	-2465719,6191,-4562865		;IOWA */
/* asm: 	.float	-3156184,-9609,-4101941		;CH */
/* asm: 	.float	-3514890,-3107,-3573365		;IN */
/* asm: 	.float	-2353033,-16317,-2927294	;APPAL */
/* asm: 	 */
/* asm: 	.float	1728,-177,2875			;GG */
/* asm: 	.float	-1929780,4638,583767		;SF */
/* asm: 	.float	-2611381,13982,-627690		;H280 */
/* asm: 	.float	-3276459,15050,-1078966		;RW */
/* asm: 	.float	-2257834,21013,-1157630		;BH */
/* asm: 	.float	-2426125,10798,-2163396		;LA */
/* asm: 	.float	-4121699,20312,-3600288		;DV */
/* asm: 	.float	-4074714,24350,-3414037		;AZ */
/* asm: 	.float	-2802209,12410,-3355048		;GC */
/* asm: 	.float	-2466458,6193,-4563828		;IOWA */
/* asm: 	.float	-3157144,-9607,-4101204		;CH */
/* asm: 	.float	-3515279,-3107,-3574450		;IN */
/* asm: 	.float	-2353891,-16335,-2928112	;APPAL */
/* asm: 	 */
/* asm: 	 */
static float STARTUP_POS_TABLE[] = {
    576.0f, -180.0f, 3287.0f, // GG
    -1928700.0f, 4654.0f, 584166.0f, // SF
    -2611877.0f, 14000.0f, -628750.0f, // H280
    -3277335.0f, 15063.0f, -1078194.0f, // RW
    -2257363.0f, 21013.0f, -1158765.0f, // BH
    -2426363.0f, 10814.0f, -2164529.0f, // LA
    -4120644.0f, 20316.0f, -3599815.0f, // DV
    -4073652.0f, 24315.0f, -3414549.0f, // AZ
    -2802001.0f, 12395.0f, -3353785.0f, // GC
    -2465719.0f, 6191.0f, -4562865.0f, // IOWA
    -3156184.0f, -9609.0f, -4101941.0f, // CH
    -3514890.0f, -3107.0f, -3573365.0f, // IN
    -2353033.0f, -16317.0f, -2927294.0f, // APPAL
    // PLYR2 (on PLYR1)
    1728.0f, -177.0f, 2875.0f, // GG
    -1929780.0f, 4638.0f, 583767.0f, // SF
    -2611381.0f, 13982.0f, -627690.0f, // H280
    -3276459.0f, 15050.0f, -1078966.0f, // RW
    -2257834.0f, 21013.0f, -1157630.0f, // BH
    -2426125.0f, 10798.0f, -2163396.0f, // LA
    -4121699.0f, 20312.0f, -3600288.0f, // DV
    -4074714.0f, 24350.0f, -3414037.0f, // AZ
    -2802209.0f, 12410.0f, -3355048.0f, // GC
    -2466458.0f, 6193.0f, -4563828.0f, // IOWA
    -3157144.0f, -9607.0f, -4101204.0f, // CH
    -3515279.0f, -3107.0f, -3574450.0f, // IN
    -2353891.0f, -16335.0f, -2928112.0f, // APPAL
};
/* asm: PLY2CAR	.bss	PLY2CAR,1 */
int PLY2CAR;

/*
*----------------------------------------------------------------------------
*Communications drone
*
*
*
*/
void COMM_DRONE(void)
{
#if CDEBUG
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	$
#endif
    // asm 0000A699: 	LDI	R0,AR1
    // asm 0000A69A: 	CMPI	4,AR1
    // asm 0000A69B: 	LDIEQ	JEEP_MOD,R0
    // asm 0000A69C: 	CMPI	5,AR1
    // asm 0000A69D: 	LDIEQ	PLYR_SBUS_MOD,R0
    // asm 0000A69E: 	CMPI	6,AR1
    // asm 0000A69F: 	LDIEQ	PLYR_COPCAR_MOD,R0
    // asm 0000A6A0: 	CMPI	7,AR1
    // asm 0000A6A1: 	LDIEQ	3,R0
    // asm 0000A6A2: DOGENRLB
    // asm 0000A6A2: 	STI	R0,*+AR7(DELTA_MODEL)
    // asm 0000A6A3: 	LDI	R0,AR2
    // asm 0000A6A4: 	LDI	R0,AR5
    // asm 0000A6A5: 	MPYI	VEHTAB_SIZE,AR2
    // asm 0000A6A6: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 0000A6A7: 	LDI	*+AR2(VEHTAB_MODEL),AR2
    // asm 0000A6A8: 	CALL	OBJ_GETE
    // asm 0000A6A9: 	BC	SUICIDE
    // asm 0000A6AA: 	LDI	AR0,AR4
    // *STORE OBJECT POINTER
    // asm 0000A6AB: 	STI	AR4,@PLY2CAR
    // asm 0000A6AC: 	LDI	R4,AR1
    // asm 0000A6AD: 	ADDI	@RACER_PTRI,AR1
    // asm 0000A6AE: 	STI	AR4,*AR1
    // asm 0000A6AF: 	LDI	AR5,AR2
    // asm 0000A6B0: 	CALL	VEHICLE_ANI_INIT
    // asm 0000A6B1: 	CALL	DELTA_OINIT
    // *SET PROPER PALETTE
    // ;	LDI	*+AR7(INITINDEX),AR0
    // ;
    // ;	LDI	*+AR0(RD_PALETTE),R0
    // ;	BZ	NOOTHERPAL
    // ;
    // ;	LDI	R0,AR2
    // ;	CALL	PAL_FIND_RAW
    // ;	STI	R0,*+AR4(OPAL)
    // ;	ORM	O_1PAL,*+AR4(OFLAGS)
    // ;NOOTHERPAL
    // asm 0000A6B2: 	LDI	1,R0
    // asm 0000A6B3: 	STI	R0,*+AR5(CAR_OM)    	;SET OTHER MACHINES VEHICLE FLAG
    // asm 0000A6B4: 	LDF	*+AR5(CARRDFR),R0
    // asm 0000A6B5: 	MPYF	1.5,R0			;OFFROAD = 1.5X ONROAD
    // asm 0000A6B6: 	STF	R0,*+AR5(CAROFRDFR)
    // asm 0000A6B7: 	LDI	*+AR7(INITINDEX),AR0
    // asm 0000A6B8: 	LDF	*+AR0(RD_XLANE),R0
    // asm 0000A6B9: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm 0000A6BA: 	STF	R0,*+AR7(ROADOFFSET)
    // asm 0000A6BB: 	LDI	2,R2		  	;SET LANE STATUS
    // asm 0000A6BC: 	FLOAT	600,R1
    // asm 0000A6BD: 	CMPF	R1,R0
    // asm 0000A6BE: 	LDIGT	3,R2
    // asm 0000A6BF: 	STI	R2,*+AR7(DELTA_STATUS)
    // asm 0000A6C0: 	LDF	1.0,R0	   		;FOR ATTRACT ALWAYS 1 DIFFICULTY
    // asm 0000A6C1: 	MPYF	*+AR0(RD_MAXACCEL),R0
    // asm 0000A6C2: 	STF	R0,*+AR5(CARMAXACCEL)	;SET ACCEL POWER
    // asm 0000A6C3: 	LDF	*+AR0(RD_REL),R0
    // asm 0000A6C4: 	STF	R0,*+AR7(RELATIVITY)	;SET RELATIVITY COEFFICIENT
    // *
    // *INIT POSITION N ROAD PIECES FROM BEGINNING
    // *
    // asm 0000A6C5: 	LDI	@DYNALIST_TRUEBEGIN,AR2
    // asm 0000A6C6: 	LDI	*+AR2(OUSR1),R0
    // asm 0000A6C7: 	ANDN	0FFh,R0
    // asm 0000A6C8: 	LDI	*+AR2(OLINK4),AR2	;SKIP FIRST GROUP
    // asm 0000A6C9: 	LDI	*+AR0(RD_POSITION),AR3	;NUMBER OF PIECES AHEAD
    // asm 0000A6CA: 	CALL	SPOS_INIT		;INIT STARTING POSITION
    // 	;initialize Ytheta to the intentional direction
    // asm 0000A6CB: 	STF	R2,*+AR4(ORADY)
    // asm 0000A6CC: 	STF	R2,*+AR5(CARYROT)
    // asm 0000A6CD: 	STF	R2,*+AR5(CARVROT)
    // asm 0000A6CE: 	LDI	AR4,AR2
    // asm 0000A6CF: 	ADDI	OMATRIX,AR2
    // asm 0000A6D0: 	CALL	FIND_YMATRIX
    // asm 0000A6D1: 	LDI	DRONE_C|VEHICLE_T|DRNE_RACER,R0
    // asm 0000A6D2: 	STI	R0,*+AR4(OID)
    // asm 0000A6D3: 	STI	R0,*+AR5(CAR_ID)
    // asm 0000A6D4: 	STI	R0,*+AR7(PID)
    // *INIT THROTTLE AND BRAKE
    // asm 0000A6D5: 	CLRF	R0
    // asm 0000A6D6: 	STF	R0,*+AR5(CARBRAKE)
    // asm 0000A6D7: 	STF	R0,*+AR5(CARSPEED)
    // asm 0000A6D8: 	LDF	0.44,R0
    // asm 0000A6D9: 	STF	R0,*+AR5(CARTRACTION)
    // asm 0000A6DA: 	LDF	1.0,R0
    // asm 0000A6DB: 	STF	R0,*+AR7(DELTA_THROTTLE)
    // asm 0000A6DC: 	STF	R0,*+AR7(POWERSURGE)
    // asm 0000A6DD: 	LDF	0.05,R0
    // asm 0000A6DE: 	CALL	FRAND
    // asm 0000A6DF: 	ADDF	1.20,R0
    // asm 0000A6E0: 	STF	R0,*+AR7(POWERCATCH)	;BLAST FROM START RANDOM TIME
    // asm 0000A6E1: 	LDI	120,R0	   		;WAIT A LITTLE BEFORE UPDATE
    // asm 0000A6E2: 	STI	R0,*+AR7(SURGETIME)
    // asm 0000A6E3: 	STI	R0,*+AR7(CATCHTIME)
    // asm 0000A6E4: 	LDI	0,R0
    // asm 0000A6E5: 	STI	R0,*+AR7(STEALTHMODE)	;NO STEALTH INIT
    // asm 0000A6E6: 	LDI	AR4,AR2
    // asm 0000A6E7: 	CALL	OBJ_PULL
    // asm 0000A6E8: 	CALL	OBJ_INSERT
    // asm 0000A6E9: 	LDI	*+AR4(OCARBLK),AR5
    // asm 0000A6EA: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm 0000A6EB: 	CALL	CAR_ROAD_COLL		;MAKE SORT WORK RIGHT
    // asm 0000A6EC: 	LDF	*+AR5(CARYROT),R2
    // asm 0000A6ED: 	LDI	@MATRIXAI,AR2
    // asm 0000A6EE: 	CALL	FIND_YMATRIX
    // asm 0000A6EF: 	LDI	AR4,R2
    // asm 0000A6F0: 	ADDI	OMATRIX,R2
    // asm 0000A6F1: 	LDI	R2,R3
    // asm 0000A6F2: 	CALL	CONCATMAT      		;FIX THE MATRIX
    // asm 0000A6F3: 	LDL	h2p1,AR2
    // asm 0000A6F4: 	LDI	*+AR7(DELTA_INIT),R0
    // asm 0000A6F5: 	CMPI	8,R0
    // asm 0000A6F6: 	BEQ	NNDDF3
    // asm 0000A6F7: 	LDL	h2p2,AR2
NNDDF3:
    // asm 0000A6F8: CALL	OBJ_GETE
    // asm 0000A6F9: 	LDI	AR0,AR2
    // asm 0000A6FA: 	LDI	AR0,AR6
    // asm 0000A6FB: 	STI	AR0,@COMM_DRONE_PTR
    // asm 0000A6FC: 	CALL	OBJ_INSERT
    // asm 0000A6FD: 	LDI	1,R1
    // asm 0000A6FE: 	LS	O_3DROT_B,R1
    // asm 0000A6FF: 	LDI	*+AR6(OFLAGS),R0
    // asm 0000A700: 	OR	O_IROT|O_1PAL|O_NOROT,R0
    // asm 0000A701: 	OR	R1,R0
    // asm 0000A702: 	STI	R0,*+AR6(OFLAGS)
    // asm 0000A703: 	LDL	H2HPAL1,AR2
    // asm 0000A704: 	CALL	PAL_FIND_RAW
    // asm 0000A705: 	STI	R0,*+AR6(OPAL)
    // asm 0000A706: 	LDI	*+AR7(DELTA_INIT),R0
    // asm 0000A707: 	STI	R0,*+AR6(OCARBLK)
    // asm 0000A708: 	CMPI	@PLY2CAR,AR4
    // asm 0000A709: 	BNE	NOTPLYR
    // asm 0000A70A: 	LDI	@CHOSEN_RACE,R0
    // asm 0000A70B: 	CMPI	RACE_APPL,R0
    // asm 0000A70C: 	BGT	NOTPLYR		;washington DC doesn't count
    // asm 0000A70D: 	MPYI	3,R0
    // asm 0000A70E: 	LDI	0,AR2
    // asm 0000A70F: 	LDI	@DIPRAM,R1
    // asm 0000A710: 	TSTB	CMDP_MASTER,R1
    // 	;Z == MASTER
    // asm 0000A711: 	LDIZ	39,AR2
    // asm 0000A712: 	ADDI	R0,AR2
    // asm 0000A713: 	ADDI	@STARTUP_POS_TABLEI,AR2
    // ;	LDF	*AR2++,R0
    // ;	STF	R0,*+AR4(OPOSX)
    // ;	LDF	*AR2++,R0
    // ;	STF	R0,*+AR4(OPOSY)
    // ;	LDF	*AR2++,R0
    // ;	STF	R0,*+AR4(OPOSZ)
    // ;	LDI	AR4,AR2
    // ;	CALL	OBJ_PULL
    // ;	CALL	OBJ_INSERT
NOTPLYR:
    // asm 0000A714: 	CLRI	R5			;blink counter
COMMDRNE_LP:
    // asm 0000A715: 	PUSH	R5
    // asm 0000A716: 	PUSH	AR6
    // asm 0000A717: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm 0000A718: 	PUSH	AR4
    // asm 0000A719: 	PUSH	AR5
    // asm 0000A71A: 	CALL	ROADSCAN		;MAKE SORT WORK RIGHT
    // asm 0000A71B: 	POP	AR5
    // asm 0000A71C: 	POP	AR4
    // asm 0000A71D: 	POP	AR6
    // asm 0000A71E: 	POP	R5
    // asm 0000A71F: 	CALL	SEND_OM_TRACK
    // asm 0000A720: 	SLEEP	1
    // asm 0000A722: 	BU	COMMDRNE_LP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_DRONE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void COMM_DRONE_PTR_SORT(void)
{
    // asm 0000A723: 	LDI	@COMM_DRONE_PTR,AR6
    // asm 0000A724: 	LDI	@PLY2CAR,AR4
REGULAR:
    // asm 0000A725: LDI	*+AR4(ODIST),IR1
    // asm 0000A726: 	ASH	-4,IR1			;quickly divide by 16
    // asm 0000A727: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm 0000A728: 	LDF	*+AR2(IR1),R0
    // asm 0000A729: 	CALL	INV_F30
    // asm 0000A72A: 	CMPF	16,R0			;
    // asm 0000A72B: 	LDFLT	16,R0			;
    // asm 0000A72C: 	STF	R0,*+AR6(OMAT00)
    // asm 0000A72D: 	STF	R0,*+AR6(OMAT11)
    // asm 0000A72E: 	STF	R0,*+AR6(OMAT22)
    // asm 0000A72F: 	LDF	*+AR4(OPOSX),R0
    // asm 0000A730: 	STF	R0,*+AR6(OPOSX)
    // asm 0000A731: 	LDF	*+AR4(OPOSY),R0
    // asm 0000A732: 	FLOAT	35,R1
    // asm 0000A733: 	MPYF	*+AR6(OMAT00),R1
    // asm 0000A734: 	SUBF	R1,R0
    // asm 0000A735: 	SUBF	20,R0
    // asm 0000A736: 	STF	R0,*+AR6(OPOSY)
    // asm 0000A737: 	LDF	*+AR4(OPOSZ),R0
    // asm 0000A738: 	STF	R0,*+AR6(OPOSZ)
    // asm 0000A739: 	LDI	AR6,AR2
    // asm 0000A73A: 	CALL	OBJ_PULL
    // asm 0000A73B: 	LDI	*AR4,R0
    // asm 0000A73C: 	STI	R0,*AR6
    // asm 0000A73D: 	STI	AR6,*AR4
    // asm 0000A73E: 	LDI	*+AR4(OFLAGS),R0
    // asm 0000A73F: 	AND	O_LIST_M,R0
    // asm 0000A740: 	OR	*+AR6(OFLAGS),R0
    // asm 0000A741: 	STI	R0,*+AR6(OFLAGS)
    // ;
    // ;	CALL	OBJ_INSERT
    // asm 0000A742: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_DRONE_PTR_SORT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*
*OTHER MACHINE DRONE LOOP
*
*/
void OM_DRONE(void)
{
    // asm 0000A743: 	LDI	1,R0			;OTHER GUYS CAR NOW....
    // asm 0000A744: 	STI	R0,*+AR5(CAR_OM)
OM_DRONEL:
    // asm 0000A745: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm 0000A746: 	PUSH	AR4
    // asm 0000A747: 	PUSH	AR5
    // asm 0000A748: 	CALL	ROADSCAN		;MAKE SORT WORK RIGHT
    // asm 0000A749: 	POP	AR5
    // asm 0000A74A: 	POP	AR4
    // asm 0000A74B: 	LDF	0,R0			;CLEAR OUT SOME SHIT
    // asm 0000A74C: 	STF	R0,*+AR5(CARDROT)
    // asm 0000A74D: 	STF	R0,*+AR5(CARSPRAD)
    // asm 0000A74E: 	STF	R0,*+AR5(CARSKID)
    // asm 0000A74F: 	LDF	*+AR5(CARYROT),R0
    // asm 0000A750: 	STF	R0,*+AR4(ORADY)		;STORE CAR OBJECT RADY
    // asm 0000A751: 	LDI	0,R0
    // asm 0000A752: 	STF	R0,*+AR5(CAR_SPIN)
    // ******************************
    // *TEST FOR NO UPDATE
    // asm 0000A753: 	LDI	0,R0
    // asm 0000A754: 	STI	R0,*+AR5(CARUPD)
    // asm 0000A755: 	SLEEP	1
    // asm 0000A757: 	LDI	*+AR5(CARUPD),R0
    // asm 0000A758: 	BNE	DRONELL
    // asm 0000A759: 	NOP
DRONELL:
    // ************
    // asm 0000A75A: 	B	OM_DRONEL
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OM_DRONE", 0, 0);
    UNIMPL();
}

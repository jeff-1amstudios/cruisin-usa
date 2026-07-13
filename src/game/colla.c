#include "colla.h"

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

/*
 * Source module: asm/COLLA.ASM
 */

void CAMSCAN(void);
static void CAMSCANS(void);
void OBJSCAN(void);
void BOXSCAN(void);
static void BOXSCSUB(void);
void CAR_ROAD_COLL(void);
void ROADSCAN(void);
static void RDSCNSUB(void);
void _coll_road(void);
static void GETNMAT(void);
void _obj_coll(void);
void _makbox(OBJ* obj /*AR4*/);
void COLSCC(void);
#define PLYR_VS_DEBRIS PLYRDEBRIS
static void PLYRDEBRIS(void);
#define PLYR_VS_SIGN PLYRSIGN
static void PLYRSIGN(void);
#define DRONE_VS_DEBRIS DRONDEBRIS
static void DRONDEBRIS(void);
#define DRONE_VS_SIGN DRONSIGN
static void DRONSIGN(void);
static void DRONEPTL(void);
static void COLPOINT(void);
void COLSGCK(void);
static void HARDCOL(void);
static void RUNOVER(void);
static void FLYCOLLP(void);
static void DEBSCAN(void);
static void SIGNFALL(void);
static void TREESHAK(void);
static void FREESIGN(void);
static void ADDSIGN(void);
static void FLYCAR(void);
void FLYCARP(void);
void SEND_FLY_KILL(void);
void DECODE_FLY_KILL(void);
static void SEND_FLY_XSFER(void);
void DECODE_FLY_XSFER(void);
void GETFLYMAT(void);
#define PLYR_VS_DRONES COLSCAN
void COLSCAN(void);
#define DRONES_VS_DRONES CLDSCAN
static void CLDSCAN(void);
static void REPELL(void);
static void COLDISP(void);
static void SPINROT(void);
static void BEHINDCK(void);
static void ANGMOM(void);
static void CKBOUNCE(void);
static void COLSND(void);
void COLCHK(void);
static void GETBOX(void);
static void GETBOX0(void);
void ATTR_COLLISION(void);

#define PLYR_VS_DEBRIS PLYRDEBRIS
#define PLYR_VS_SIGN PLYRSIGN
#define DRONE_VS_DEBRIS DRONDEBRIS
#define DRONE_VS_SIGN DRONSIGN
#define PLYR_VS_DRONES COLSCAN
#define DRONES_VS_DRONES CLDSCAN
#define VLI VL
#define TNORMI TNORM
#define TVECT1I TVECT1
#define TVECT2I TVECT2
#define TMATRIXI TMATRIX
#define BOXSCRAMI BOXSCRAM
#define SAGETABI SAGETAB
#define FLYCOLLPI FLYCOLLP
#define SIGNFALLI SIGNFALL
#define TREESHAKI TREESHAK
#define SBUSI sbus_ROM
#define CBUSI cbus_ROM
#define DETHTAB1I DETHTAB1
#define DETHTAB2I DETHTAB2
#define FLYCARPI FLYCARP
#define FLYCARPXXXXI FLYCARPXXXX
#define FLYCARPXXXI FLYCARPXXX
#define DEADLPI DEADLP
#define FLYCARP0I FLYCARP
#define FLYCARSTOPI FLYCARSTOP
#define SCUPDTABI SCUPDTAB
#define SCTABI SCTAB
#define EQTABI EQTAB
#define LEQTABI LEQTAB

void WRECKST(void);
extern int WRECKFLG;
extern int CHEAT;
void DRONINBZ(void);
float ROADIR(OBJ* track_obj /*AR0*/);
int CKAHEAD(OBJ* other_obj /*AR2*/, CARBLK* other_carblk /*AR3*/, OBJ* obj /*AR4*/, CARBLK* carblk /*AR5*/);
extern const char PC2[];
void GETNXTRDIR(void);
void RANDSND(void);
void COMPTRAK(void);
void OM_DRONE(void);
void FIND_DRONE(void);
void RANDVSND(void);

static f32* EQTAB[];
static f32* LEQTAB[];

/* asm: VL	.bss	VL,4 */
int VL[4];
/* asm: TNORM	.bss	TNORM,3 */
int TNORM[3];
/* asm: TVECT1	.bss	TVECT1,3 */
int TVECT1[3];
/* asm: TVECT2	.bss	TVECT2,3 */
int TVECT2[3];
/* asm: TMATRIX	.bss	TMATRIX,9 */
int TMATRIX[9];
/* asm: COLVEL	.bss	COLVEL,1 */
float COLVEL;
/* asm: PMULT	.bss	PMULT,1 */
int PMULT;
/* asm: SPINTEMP	.bss	SPINTEMP,1 */
int SPINTEMP;

/*
 *----------------------------------------------------------------------------
 *CAMERA SCAN FOR ROAD HEIGHT
 *
 *PARAMETERS
 *	AR4	POINTS TO VECTOR CAMERA X,Y,Z
 *RETURNS
 *	R0 = CAMERA HEIGHT ABOVE THE ROAD
 *	CS = COLLISION FOUND WITH ROAD
 *	CC = NO COLLISION FOUND
 *
 */
void CAMSCAN(void) {
    // asm 00001F8B: 	LDPI	@DRIVE_LIST,R0
    // asm 00001F8C: 	CALL	CAMSCANS
    // asm 00001F8D: 	RETSC
    // asm 00001F8E: 	LDPI	@GROUND_LIST,R0
    // *FALL THRU TO CAMSCANS
    // *
    // *SCAN LIST FOR POINT-OBJECT INTERSECTION
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMSCAN", 0, 0);
    UNIMPL();
}

static void CAMSCANS(void) {
    // asm 00001F8F: 	BZ	CMSX  			;NULL LIST DUDES
    // asm 00001F90: 	LDI	R0,AR2
    // asm 00001F91: 	LDI	OPOSZ,IR1
    // asm 00001F92: 	LDF	*+AR4(2),R2		;GET POINT X
    // asm 00001F93: 	LDF	*AR4,R3			;GET POINT Z
CMS0:
    // asm 00001F94: 	SUBF	*+AR2(OPOSX),R3,R0
    // asm 00001F95: 	SUBF	*+AR2(IR1),R2,R1
    // asm 00001F96: 	MPYF	R1,R1
CMS1:
    // asm 00001F97: 	MPYF	R0,R0
    // asm 00001F98: 	ADDF	R1,R0
    // asm 00001F99: 	FLOAT	*+AR2(ORAD),R1	 	;GET ROAD RADIUS
    // asm 00001F9A: 	MPYF	R1,R1
    // asm 00001F9B: 	CMPF	R1,R0	  		;DISTANCE < RADIUS ?
    // asm 00001F9C: 	BLT	CMS2			;YES, CHECK IT OUT
    // asm 00001F9D: 	LDI	*+AR2(OLINK3),AR2
    // asm 00001F9E: 	LDI	AR2,R1
    // asm 00001F9F: 	BNZD	CMS1
    // asm 00001FA0: 	SUBF	*+AR2(OPOSX),R3,R0
    // asm 00001FA1: 	SUBF	*+AR2(IR1),R2,R1
    // asm 00001FA2: 	MPYF	R1,R1
    // 	;---->	BNZ	CMS1
    // asm 00001FA3: 	B	CMSX			;WE FAILED
    // *CHECK OUT POINT COLLISION
CMS2:
    // asm 00001FA4: 	CALL	_coll_road		;XZ POINT COLLISION WITH ROAD OBJECT?
    // asm 00001FA5: 	BNC	CMS1L
    // asm 00001FA6: 	RETS				;RETURN COLLISION VALUE
CMS1L:
    // asm 00001FA7: 	LDI	*+AR2(OLINK3),AR2
    // asm 00001FA8: 	LDI	AR2,R1
    // asm 00001FA9: 	BNZD	CMS0
    // asm 00001FAA: 	LDI	OPOSZ,IR1
    // asm 00001FAB: 	LDF	*+AR4(2),R2		;GET POINT X
    // asm 00001FAC: 	LDF	*AR4,R3			;GET POINT Z
    // 	;---->	BNZ	CMS0
CMSX:
    // asm 00001FAD: 	FLOAT	0,R0		;DEFAULT HT.
    // asm 00001FAE: 	CLRC
    // asm 00001FAF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMSCANS", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *SCAN OBJECT CENTER POINT VERSUS ROAD
 *
 *PARAMETERS
 *	AR4	OBJECT
 *RETURNS
 *	CARRY SET IF ROAD FOUND BELOW OBJECT
 */
void OBJSCAN(void) {
    // asm 00001FB0: 	PUSH	AR4
    // asm 00001FB1: 	ADDI	OPOSX,AR4
    // asm 00001FB2: 	CALL	CAMSCAN	      		;AR4=XYZ, RET R0=HT, CS=ROAD,CC=NO ROAD
    // asm 00001FB3: 	POP	AR4
    // asm 00001FB4:  	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJSCAN", 0, 0);
    UNIMPL();
}

/* asm: BOXSCRAM	FBSS	BOXSCRAM,50 */
int BOXSCRAM[50];

/*
 *
 *CHECK ROAD OBJECTS ON ROAD LIST IN RANGE
 *
 */
void BOXSCAN(void) {
    // asm 00001FB6: 	PUSH	AR5
    // asm 00001FB7: 	LDPI	@BOXSCRAMI,AR2
    // asm 00001FB8: 	LDI	AR4,AR0			;GET OBJ IN AR0 FOR GETBOX
    // asm 00001FB9: 	LDF	0.7,R0			;XMINUS MULT FACTOR
    // asm 00001FBA: 	LDF	0.7,R1			;YMINUS MULT FACTOR
    // asm 00001FBB: 	LDF	0.7,R2			;ZMINUS MULT FACTOR
    // asm 00001FBC: 	LDF	0.7,R3			;XPLUS MULT FACTOR
    // asm 00001FBD: 	LDF	1.0,R4			;YPLUS MULT FACTOR
    // asm 00001FBE: 	LDF	0.7,R5			;ZPLUS MULT FACTOR
    // asm 00001FBF: 	CALL	GETBOX0			;GET BOX POINTS FOR OBJECT 1
    // asm 00001FC0: 	FLOAT	*+AR4(ORAD),R5	 	;GET ROAD RADIUS
    // asm 00001FC1: 	FLOAT	20000,R7		;DEFAULT CAR HT. ABOVE GROUND
    // asm 00001FC2: 	LDPI	@DRIVE_LIST,R0
    // asm 00001FC3: 	CALL	BOXSCSUB
    // asm 00001FC4: 	LDPI	@GROUND_LIST,R0
    // asm 00001FC5: 	CALL	BOXSCSUB
    // asm 00001FC6: 	POP	AR5
    // asm 00001FC7: 	LDF	R7,R0
    // asm 00001FC8: 	FLOAT 	20000,R7
    // asm 00001FC9: 	CMPF	R7,R0
    // asm 00001FCA: 	BLT	BS3X
    // asm 00001FCB: 	CLRC	 			;NOTHING FOUND
    // asm 00001FCC: 	RETS
BS3X:
    // asm 00001FCD: 	SETC
    // asm 00001FCE: 	RETS
    // *
    // *SCAN BOX FOR GROUND INTERSECTION
    // *R0=LIST
    // *AR4=OBJECT
    // *R5=QUICK REJECT LIMIT
    // *R7=LOWEST HEIGHT SO FAR
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOXSCAN", 0, 0);
    UNIMPL();
}

static void BOXSCSUB(void) {
    // asm 00001FCF: 	BZ	BSCX  			;NULL LIST DUDES
    // asm 00001FD0: 	LDI	R0,AR2
    // asm 00001FD1: 	LDI	OPOSZ,IR1
BS0:
    // asm 00001FD2: 	FLOAT	*+AR4(ORAD),R0		;GET BOX RADIUS
    // asm 00001FD3: 	LDF	*+AR4(OPOSZ),R4		;GET OBJECT Z
    // asm 00001FD4: 	LDF	*+AR4(OPOSX),R3		;GET OBJECT X
    // asm 00001FD5: 	SUBF	*+AR2(OPOSX),R3,R2
    // asm 00001FD6: 	SUBF	*+AR2(IR1),R4,R1
    // asm 00001FD7: 	MPYF	R1,R1
BS1:
    // asm 00001FD8: 	MPYF	R2,R2
    // asm 00001FD9: 	ADDF	R1,R2
    // asm 00001FDA: 	FLOAT	*+AR2(ORAD),R1	 	;GET ROAD RADIUS
    // asm 00001FDB: 	ADDF	R0,R1			;ADD AND SQUARE
    // asm 00001FDC: 	MPYF	R1,R1
    // asm 00001FDD: 	CMPF	R1,R2	  		;TEST TRUE RADIUS
    // asm 00001FDE: 	BLT	BS2			;NO GO...
    // asm 00001FDF: 	LDI	*+AR2(OLINK3),AR2
    // asm 00001FE0: 	LDI	AR2,R1
    // asm 00001FE1: 	BNZD	BS1
    // asm 00001FE2: 	SUBF	*+AR2(OPOSX),R3,R2
    // asm 00001FE3: 	SUBF	*+AR2(IR1),R4,R1
    // asm 00001FE4: 	MPYF	R1,R1
    // 	;---->	BNZ	BS1
    // asm 00001FE5: 	RETS
    // *CHECK OUT POINT COLLISION
BS2:
    // asm 00001FE6: 	PUSH	AR4
    // asm 00001FE7: 	LDPI	@BOXSCRAMI,AR4
    // asm 00001FE8: 	ADDI	18H,AR4
    // asm 00001FE9: 	LDI	7,AR5			;LOOP 8 BOX POINTS
    // asm 00001FEA: BSRDLP
    // asm 00001FEA: 	CALL	_coll_road		;XZ POINT COLLISION WITH ROAD OBJECT?
    // asm 00001FEB: 	BNC	BS10			;NOPE...
    // asm 00001FEC: 	CMPF	R0,R7
    // asm 00001FED: 	LDFGT	R0,R7			;SAVE LOWEST POINT
BS10:
    // asm 00001FEE: 	NOP	*AR4++(3)     		;CHECK NEXT POINT
    // asm 00001FEF: 	DB	AR5,BSRDLP
    // asm 00001FF0: 	LDI	*+AR2(OLINK3),R0
    // asm 00001FF1: 	BNZD	BS0
    // asm 00001FF2: 	LDI	R0,AR2
    // asm 00001FF3: 	POP	AR4
    // asm 00001FF4: 	LDI	OPOSZ,IR1
    // 	;---->	BNZ	BS0
BSCX:
    // asm 00001FF5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOXSCSUB", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CHECK VEHICLE COLLISION WITH ROAD
 *
 *PARAMETERS
 *	AR4	OBJECT
 *	R3	POINTER TO CARVCT RAM AREA
 *	RAM AREA= 3*(X,Y,Z,ROAD Y,Y VEL,COLLISION OBJECT)
 *		ONROAD,AIRFRONT,AIRBORNE (1=TRUE)
 *RETURNS
 *	CARVCT AREA MODIFIED,FLAGS SET, OBJECT MATRIX ALIGNED TO ROAD
 *	NEED TO ADD Y RADIANS AFTERWARD
 *
 */
void CAR_ROAD_COLL(void) {
    // asm 00001FF6: 	PUSH	R4
    // asm 00001FF7: 	PUSH	R5
    // asm 00001FF8: 	PUSH	AR3
    // asm 00001FF9: 	PUSH	AR4
    // asm 00001FFA: 	PUSH	AR5
    // asm 00001FFB: 	PUSH	AR6
    // asm 00001FFC: 	CALL	ROADSCAN 		;GET POINT HEIGHTS
    // ****************************************************
    // *WE HAVE FOUND HEIGHT FOR ALL SUSPENSION POINTS
    // *GET NEW PLAYER MATRIX
    // asm 00001FFD: PC1XX
    // asm 00001FFD: 	LDI	*+AR6(CT_PCOL),R0  	;NO ROAD COLLISION CENTER POINT
    // asm 00001FFE: 	BZ	PC1X0
    // asm 00001FFF: 	LDI	R0,AR0
    // asm 00002000: 	LDI	*+AR0(OID),R0
    // asm 00002001: 	AND	CLASS_M+TYPE_M,R0
    // asm 00002002: 	STI	R0,*+AR6(CAR_ONROAD)	;XXX OID->ONROAD FLAG
PC1X0:
    // asm 00002003: 	LDI	AR6,AR0			;GET CARVCT SUSPENSION POINTS
    // asm 00002004: 	FLOATP	@NFRAMES,R2
    // asm 00002005: 	MPYF	4,R2			;FRAME ADJUSTED GRAVITY
    // ;	MPYF	8,R2			;FRAME ADJUSTED GRAVITY
    // asm 00002006: 	LDI	CARVNUM-1,RC 		;LOOP FOR ALL GROUND TOUCHERS
    // asm 00002007: 	RPTB	PC2
    // asm 00002008: 	LDF	*+AR0(CARPRDYD),R0	;LOAD DELTA HEIGHT
    // asm 00002009: 	CMPF	-9,R0
    // asm 0000200A: 	BGT	PC1A			;WE ARE ABOVE ROAD
    // *BELOW ROAD CASE
    // asm 0000200B: 	ADDF	-9,R0
    // asm 0000200C: 	ADDF	*+AR0(CARPY),R0		;WE ARE BELOW ROAD
    // asm 0000200D: 	BD	PC2
    // asm 0000200E: 	STF	R0,*+AR0(CARPY)		;SET TO ROAD HEIGHT
    // asm 0000200F: 	LDF	0,R0
    // asm 00002010: 	STF	R0,*+AR0(CARPYV)	;STORE NEW VELOCITY
    // 	;-------->B	PC2
    // *ABOVE ROAD CASE
PC1A:
    // asm 00002011: 	LDF	*+AR0(CARPYV),R1	;GRAVITY ACCELERATES Y VEL
    // asm 00002012: 	ADDF	R2,R1
    // asm 00002013: 	LDI	*+AR0(CARPCOL),R4	;CHECK GRAVITY TYPE
    // asm 00002014: 	BZ	PC1B
    // asm 00002015: 	LDI	R4,AR3
    // asm 00002016: 	LDI	*+AR3(OID),R4
    // asm 00002017: 	AND	CLASS_M+TYPE_M,R4
    // asm 00002018: 	CMPI	ROAD_C+LOGRAV_T,R4
    // ;	BNE	PC1AA
    // ;	NOP
    // ;PC1AA
    // asm 00002019: 	BEQ	PC1B			;LOGRAVITY SECTION?
    // asm 0000201A: 	LDF	R2,R4
    // asm 0000201B: 	MPYF	4,R4			;NO, GRAV X 4
    // asm 0000201C: 	ADDF	R4,R1
PC1B:
    // asm 0000201D: 	CMPF	R1,R0	    		;VEL GT HEIGHT?
    // asm 0000201E: 	BGT	PC2A			;NO
    // asm 0000201F: 	LDF	R0,R1			;YES LIMIT VELOCITY
PC2A:
    // asm 00002020: 	STF	R1,*+AR0(CARPYV)	;ADD VELOCITY TO HEIGHT
    // asm 00002021: PC2B
    // asm 00002021: 	ADDF	*+AR0(CARPY),R1
    // asm 00002022: 	STF	R1,*+AR0(CARPY)
PC2:
    // asm 00002023: NOP 	*AR0++(CARVSIZ)
    // *SET AIRBORNE FLAGS
    // asm 00002024: 	LDI	AR6,AR0		;GET CARVCT SUSPENSION POINTS
    // asm 00002025: 	LDI	1,R0   		;ASSUME AIRBORNE
    // asm 00002026: 	LDI	1,R1
    // asm 00002027: 	LDF	*+AR0(3),R2	;LOAD DELTA HEIGHT
    // asm 00002028:       	CMPF	72,R2		;1 FOOT OFF GROUND?
    // asm 00002029: 	LDILT	0,R0		;NO
    // asm 0000202A: 	LDF	*+AR0(9),R2
    // asm 0000202B:       	CMPF	72,R2		;1 FOOT OFF GROUND?
    // asm 0000202C: 	LDILT	0,R1		;NO
    // asm 0000202D: 	LDF	*+AR0(15),R2
    // asm 0000202E:       	CMPF	72,R2		;1 FOOT OFF GROUND?
    // asm 0000202F: 	LDILT	0,R1		;NO
    // asm 00002030: 	STI	R0,*+AR6(CAR_AIRB)
    // asm 00002031: 	STI	R1,*+AR6(CAR_AIRF)
    // asm 00002032: 	LDI	AR6,R0		     	;get suspension points in ram
    // asm 00002033: 	LDPI	@VLI,AR0		;rotate for universe etc.
    // asm 00002034: 	STI	R0,*AR0++
    // asm 00002035: 	ADDI	CARVSIZ,R0
    // asm 00002036: 	STI	R0,*AR0++
    // asm 00002037: 	ADDI	CARVSIZ,R0
    // asm 00002038: 	STI	R0,*AR0++
    // asm 00002039: 	CALL	GETNMAT			;GET NEW MATRIX
    // asm 0000203A: PCOLLX
    // asm 0000203A: 	POP	AR6
    // asm 0000203B: 	POP	AR5
    // asm 0000203C: 	POP	AR4
    // asm 0000203D: 	POP	AR3
    // asm 0000203E: 	POP	R5
    // asm 0000203F: 	POP	R4
    // asm 00002040: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAR_ROAD_COLL", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *ROADSCAN - FIND COLLISION HEIGHTS FOR ALL WHEELS
 *
 *PARAMETERS
 *	AR4	CAR OBJECT
 *	R3	CAR BLOCK
 *
 */
void ROADSCAN(void) {
    // asm 00002041: 	LDI	R3,AR6			;SAVE CARVCT RAM POINTER
    // *PROJECT CAR SUSPENSION POINTS
    // asm 00002042: 	LDI	AR4,R2
    // asm 00002043: 	ADDI	OMATRIX,R2		;POINT TO CAR MATRIX
    // asm 00002044: 	LDI	AR6,AR2
    // asm 00002045: 	ADDI	CARWHLTAB,AR2
    // asm 00002046: 	LDF	*+AR4(OPOSX),R1		;GET Y OBJECT OFFSET
    // asm 00002047: 	LDF	*+AR4(OPOSY),R4		;GET Y OBJECT OFFSET
    // asm 00002048: 	LDF	*+AR4(OPOSZ),R5		;GET Z OBJECT OFFSET
    // asm 00002049: 	LDI	2,IR0
    // asm 0000204A: 	LDI	CARVNUM-1,RC		;LOOP FOR ALL POINTS
    // asm 0000204B: 	RPTB	LOOP
    // asm 0000204C: 	LDI	R3,AR3
    // asm 0000204D: 	CALL	MATRIX_MUL
    // *ADD IN X,Z OFFSETS
    // asm 0000204E: 	ADDF	R1,*AR3,R0
    // asm 0000204F: 	ADDF	R4,*+AR3(1),R0
    // asm 0000204F: ||	STF	R0,*AR3
    // asm 00002050: 	STF	R0,*+AR3(1)
    // ;	NEGF	R0			;DEFAULT COLLISION DELTA = - HEIGHT
    // asm 00002051: 	LDF	0,R0			;CLEAR DEFAULT HEIGHT
    // asm 00002052: 	STF	R0,*+AR3(3)
    // asm 00002053: 	LDI	0,R0
    // asm 00002054: 	STI	R0,*+AR3(5)		;CLEAR COLLISION OBJECT
    // asm 00002055: 	ADDF	R5,*+AR3(IR0),R0
    // asm 00002056: 	STF	R0,*+AR3(2)
    // asm 00002057: 	ADDI	3,AR2
LOOP:
    // asm 00002058: ADDI	6,R3
    // asm 00002059: 	CLRI	R0
    // asm 0000205A: 	STI	R0,*+AR6(CAR_ONROAD)
    // *CHECK ROAD OBJECTS ON ROAD LIST IN RANGE
    // asm 0000205B: 	LDPI	@DRIVE_LIST,R0
    // asm 0000205C: 	CALL	RDSCNSUB
    // *WE HAVE SCANNED ROADLIST
    // *IF INCOMPLETE SCAN DO GROUND LIST
    // asm 0000205D: 	LDI	*+AR6(CT_PCOL),R0	;CHECK COLLISION...
    // asm 0000205E: 	OR	*+AR6(LF_PCOL),R0	;CHECK COLLISION...
    // asm 0000205F: 	OR	*+AR6(RF_PCOL),R0	;CHECK COLLISION...
    // asm 00002060: 	OR	*+AR6(RR_PCOL),R0	;CHECK COLLISION...
    // asm 00002061: 	OR	*+AR6(LR_PCOL),R0	;CHECK COLLISION...
    // asm 00002062: 	LDINZ	1,R0
    // asm 00002063: 	STI	R0,*+AR6(CAR_ONROAD)	;ANY WHEEL ON IS ONROAD
    // asm 00002064: 	LDI	0,R1
    // asm 00002065: 	LDI	*+AR6(CT_PCOL),R0	;CHECK COLLISION...
    // asm 00002066: 	LDIZ	1,R1			;SET FLAG IF NONE
    // ;       BZ	PC3A
    // asm 00002067: 	LDI	*+AR6(LF_PCOL),R0	;CHECK COLLISION...
    // asm 00002068: 	LDIZ	2,R1
    // ;	BZ	PC3A
    // asm 00002069: 	LDI	*+AR6(RF_PCOL),R0	;CHECK COLLISION...
    // asm 0000206A: 	LDIZ	3,R1
    // ;	BZ	PC3A
    // asm 0000206B: 	LDI	*+AR6(RR_PCOL),R0	;CHECK COLLISION...
    // asm 0000206C: 	LDIZ	4,R1
    // ;	BZ	PC3A
    // asm 0000206D: 	LDI	*+AR6(LR_PCOL),R0	;CHECK COLLISION...
    // asm 0000206E: 	LDIZ	5,R1
    // ;	BZ	PC3A
    // asm 0000206F: 	LDI	R1,R1			;ALL WHEELS ON ROAD?
    // asm 00002070: 	BZ	PC3X			;YES...WERE DONE
    // *OFF ROAD- CHECK OUT GROUND LIST
    // asm 00002071: PC3A
    // asm 00002071: 	LDPI	@GROUND_LIST,R0
    // asm 00002072: 	CALL	RDSCNSUB
    // ;	LDI	0,R1
    // ;	LDI	*+AR6(CT_PCOL),R0	;CHECK COLLISION...
    // ;	LDIZ	1,R1			;SET FLAG IF NONE
    // ;	LDI	*+AR6(LF_PCOL),R0	;CHECK COLLISION...
    // ;	LDIZ	2,R1
    // ;	LDI	*+AR6(RF_PCOL),R0	;CHECK COLLISION...
    // ;	LDIZ	3,R1
    // ;	LDI	*+AR6(RR_PCOL),R0	;CHECK COLLISION...
    // ;	LDIZ	4,R1
    // ;	LDI	*+AR6(LR_PCOL),R0	;CHECK COLLISION...
    // ;	LDIZ	5,R1
    // ;	LDI	R1,R1			;ALL WHEELS ON ROAD?
    // ;	BZ	PC3X			;YES...WERE DONE
    // ;	NOP				;TRAP HERE
PC3X:
    // asm 00002073: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADSCAN", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *ROAD SCAN SUBROUTINE
 *
 *PARAMETERS
 *	R0	LIST TO SCAN
 *	AR4	CAR OBJECT
 *
 */
static void RDSCNSUB(void) {
    // asm 00002074:       	BZD	RDSCNX		  	;NULL LIST
    // asm 00002075: 	LDI	R0,AR2
    // asm 00002076: 	LDI	OPOSZ,IR1
    // asm 00002077: 	FLOAT	*+AR4(ORAD),R0		;GET BOX RADIUS
    //       	;------>BZD	RDSCNX		;NULL LIST
    // asm 00002078: 	LDF	*+AR4(OPOSZ),R4		;GET OBJECT Z
RS0:
    // asm 00002079: 	LDF	*+AR4(OPOSX),R3		;GET OBJECT X
RS1:
    // asm 0000207A: 	SUBF	*+AR2(OPOSX),R3,R2
    // asm 0000207B: 	SUBF	*+AR2(IR1),R4,R1
    // asm 0000207C: 	MPYF	R1,R1
    // asm 0000207D: 	MPYF	R2,R2
    // asm 0000207E: 	ADDF	R1,R2
    // asm 0000207F: 	FLOAT	*+AR2(ORAD),R1	 	;GET ROAD RADIUS
    // asm 00002080: 	ADDF	R0,R1			;ADD AND SQUARE
    // asm 00002081: 	MPYF	R1,R1
    // asm 00002082: 	CMPF	R1,R2	  		;TEST TRUE RADIUS
    // asm 00002083: 	BLT	RS2			;CHECK IT OUT !
    // asm 00002084: RS1L
    // asm 00002084: 	LDI	*+AR2(OLINK3),AR2
    // asm 00002085: 	LDI	AR2,R1
    // asm 00002086: 	BNZD	RS1
    // asm 00002087: 	SUBF	*+AR2(OPOSX),R3,R2
    // asm 00002088: 	SUBF	*+AR2(IR1),R4,R1
    // asm 00002089: 	MPYF	R1,R1
    // 	;---->	BNZ	RS1
    // asm 0000208A: 	RETS
    // *CHECK OUT ROAD COLLISION
RS2:
    // asm 0000208B: 	PUSH	AR4
    // asm 0000208C: 	LDI	AR6,AR4
    // asm 0000208D: 	LDI	CARVNUM-1,AR5		;LOOP ALL POINTS
    // asm 0000208E: RS3LP
    // asm 0000208E: 	LDI	*+AR4(CARPCOL),R1	;CHECK PRIOR COLLISION...
    // asm 0000208F: 	BZ	RS300			;NOPE, SCAN ON...
    // asm 00002090: 	ABSF	*+AR4(CARPRDYD),R1
    // asm 00002091: 	FLOAT	2000,R2
    // asm 00002092: 	CMPF	R2,R1
    // asm 00002093: 	BLT	RS30			;ALREADY CLOSE, NO RESCAN...
    // asm 00002094: 	CALL	_coll_road		;XZ POINT COLLISION WITH ROAD OBJECT?
    // asm 00002095: 	BNC	RS30			;NOPE...
    // asm 00002096: 	LDF	*+AR4(CARPRDYD),R1
    // asm 00002097: 	BNN	RS297
    // asm 00002098:      	LDF	R0,R0
    // asm 00002099: 	BNN	RS301			;OLD=NEG, NEW=POS, GO W/ POS
    // *BOTH NEGATIVE CASE
    // asm 0000209A: 	CMPF	R0,R1
    // asm 0000209B: 	BGT	RS30
    // asm 0000209C: 	B	RS301
    // *OLD IS POSITIVE
RS297:
    // asm 0000209D: 	LDF	R0,R0
    // asm 0000209E: 	BNN	RS299
    // *OLD IS POS, NEW IS NEG
    // asm 0000209F: 	FLOAT	-1000,R2
    // asm 000020A0: 	CMPF	R2,R0
    // asm 000020A1: 	BGT	RS301
    // asm 000020A2: 	B	RS30
    // *BOTH POSITIVE CASE
RS299:
    // asm 000020A3: 	CMPF	R0,R1			;TAKE LEAST POSITIVE
    // asm 000020A4: 	BLT	RS30
    // asm 000020A5: 	B	RS301
RS300:
    // asm 000020A6: 	CALL	_coll_road		;XZ POINT COLLISION WITH ROAD OBJECT?
    // asm 000020A7: 	BNC	RS30			;NOPE...
RS301:
    // asm 000020A8: 	STF	R0,*+AR4(CARPRDYD)		;SAVE ROAD Y DELTA
    // asm 000020A9: 	STI	AR2,*+AR4(CARPCOL) 	;SAVE COLLISION OBJECT
RS30:
    // asm 000020AA: 	NOP	*AR4++(CARVSIZ)
    // asm 000020AB: 	DB	AR5,RS3LP
    // asm 000020AC: 	POP	AR4
    // asm 000020AD: RS3L
    // asm 000020AD: 	LDI	*+AR2(OLINK3),AR2
    // asm 000020AE: 	LDI	AR2,R0
    // asm 000020AF: 	BNZD	RS0
    // asm 000020B0: 	LDI	OPOSZ,IR1
    // asm 000020B1: 	FLOAT	*+AR4(ORAD),R0		;GET BOX RADIUS
    // asm 000020B2: 	LDF	*+AR4(OPOSZ),R4		;GET OBJECT Z
    // 	;---->	BNZ	RS0
RDSCNX:
    // asm 000020B3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RDSCNSUB", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *coll_road(OBJECTP _obj)
 *PARMETERS
 *	AR2	colliding object
 *	AR4	POINT VECTOR X,Y,Z
 *RETURNS
 *	R0	-HEIGHT ABOVE ROAD
 *		CARRY SET IF ROAD PIECE COLLISION
 *
 */
void _coll_road(void) {
    // asm 000020B4: 	LDI	AR4,R2		;GET POINT INTO R2
    // asm 000020B5: 	CALL	_obj_coll
    // asm 000020B6: 	BNC	CRX		;NO COLLISION BAIL OUT WITH CARRY CLEAR
    // *
    // *WE HAVE A COLLISION
    // *FIND Y HEIGHT RETURN IN R0
    // *
    // 	;generate a (unit) normal for the actual plane
    // asm 000020B7: 	PUSH	R4
    // asm 000020B8: 	PUSH	R5
    // asm 000020B9: 	PUSH	AR2
    // asm 000020BA: 	PUSH	R6
    // asm 000020BB: 	PUSHF	R6
    // asm 000020BC: 	PUSH	R7
    // asm 000020BD: 	PUSHF	R7
    // asm 000020BE: 	LDPI	@VLI,AR2		;rotate for universe etc.
    // asm 000020BF: 	LDPI	@TNORMI,AR0
    // asm 000020C0: 	CALL	GEN_NORMAL		;gen_normal(&A,&B,&C,&N);
    // asm 000020C1: 	LDPI	@VLI,AR1
    // asm 000020C2: 	LDI	*AR1,AR1
    // asm 000020C3: 	MPYF	*AR1++,*AR0++,R0
    // asm 000020C4: 	NEGF	*AR0,R1			;N.y
    // asm 000020C5: 	MPYF	*AR1++,*AR0++,R2
    // asm 000020C6: 	ADDF	R2,R0			;D = - (N.x * bufferx[vert[0]] +
    // asm 000020C7: 	MPYF	*AR1++,*AR0++,R2	;       N.y * buffery[vert[0]] +
    // asm 000020C8: 	ADDF	R2,R0			;       N.z * bufferz[vert[0]]);
    // asm 000020C9: 	NEGF	R0			;D = ((D/(-N.y)));
    // asm 000020CA: 	CALL	DIV_F30			;(R0/R1)->R0 clobbers r0,r1,ar0,ar1
    // asm 000020CB: 	POPF	R7
    // asm 000020CC: 	POP	R7
    // asm 000020CD: 	POPF	R6
    // asm 000020CE: 	POP	R6
    // asm 000020CF: 	POP	AR2
    // asm 000020D0: 	POP	R5
    // asm 000020D1: 	POP	R4
    // asm 000020D2: 	SETC				;WE GOT ONE
CRX:
    // asm 000020D3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_coll_road", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *GET MATRIX TO ALIGN WITH NORMAL, Z AND X AXES ROTATION ONLY
 *
 *VL1=POINTER TO 3 POINTS
 *AR4=OBJECT
 *AR6=CARVCT RAM AREA
 *
 */
static void GETNMAT(void) {
    // 	;*GENERATE A (UNIT) NORMAL FOR THE PLANE
    // asm 000020D4: 	LDPI	@VLI,AR2		;rotate for universe etc.
    // asm 000020D5: 	LDPI	@TNORMI,AR0
    // asm 000020D6: 	CALL	GEN_NORMAL		;gen_normal(&A,&B,&C,&N);
    // asm 000020D7: 	LDI	AR0,AR2
    // asm 000020D8: 	CALL	NORMALIZE		;normalize(&N);
    // asm 000020D9: 	LDPI	@TMATRIXI,AR3
    // *LOAD 2ND COLUMN OF ROTATION MATRIX (Y AXIS)
    // asm 000020DA: 	LDF	*AR2,R0		     	;2ND COLUMN ROT MATRIX IS NORMAL VECTOR
    // asm 000020DB: 	STF	R0,*+AR3(3)
    // asm 000020DC: 	LDF	*+AR2(1),R1
    // asm 000020DD: 	STF	R1,*+AR3(4)
    // asm 000020DE: 	LDF	*+AR2(2),R2
    // asm 000020DF: 	STF	R2,*+AR3(5)
    // *LOAD 1ST COLUMN OF ROTATION MATRIX (X AXIS)
    // asm 000020E0: 	LDI	AR3,AR2
    // asm 000020E1: 	STF	R1,*AR2			;X
    // asm 000020E2: 	NEGF	R0			;-N2
    // asm 000020E3: 	STF	R0,*+AR2(1)		;Y
    // asm 000020E4: 	CLRF	R1
    // asm 000020E5: 	STF	R1,*+AR2(2)		;Z
    // asm 000020E6: 	CALL	NORMALIZE		;normalize(&N);
    // *LOAD 3RD COLUMN OF ROTATION MATRIX (Z AXIS)
    // ;	LDI	AR3,AR2
    // ;	ADDI	6,AR2			;POINT AR2 TO THIRD COLUMN OF MATRIX
    // ;
    // ;	LDF	*+AR3(4),R1		;GET N2
    // ;	STF	R1,*+AR2(2)		;Z
    // ;
    // ;
    // ;	LDF	*+AR3(5),R0		;GET N3
    // ;	NEGF	R0
    // ;	STF	R0,*+AR2(1)		;Y
    // ;
    // ;	CLRF	R1
    // ;	STF	R1,*+AR2(0)		;X
    // *COMPUTE CROSS PRODUCT
    // asm 000020E7: 	ADDI	3,AR2			;POINT AR2 TO THIRD COLUMN OF MATRIX
    // asm 000020E8: 	LDI	2,IR0
    // asm 000020E9: 	MPYF	*+AR3(1),*+AR2(IR0),R0	;U2*V3
    // asm 000020EA: 	MPYF	*+AR3(IR0),*+AR2(1),R1	;U3*V2
    // asm 000020EB: 	SUBF	R1,R0
    // asm 000020EC: 	STF	R0,*+AR3(6)
    // asm 000020ED: 	MPYF	*+AR3(IR0),*AR2,R0	;U3*V1
    // asm 000020EE: 	MPYF	*AR3,*+AR2(IR0),R1	;U1*V3
    // asm 000020EF: 	SUBF	R1,R0
    // asm 000020F0: 	STF	R0,*+AR3(7)
    // asm 000020F1: 	MPYF	*AR3,*+AR2(1),R0	;U1*V2
    // asm 000020F2: 	MPYF	*+AR3(1),*AR2,R1	;U2*V1
    // asm 000020F3: 	SUBF	R1,R0
    // asm 000020F4: 	STF	R0,*+AR3(8)
    // asm 000020F5: 	ADDI	3,AR2
    // asm 000020F6: 	CALL	NORMALIZE		;normalize(&N);
    // *INVERT MATRIX AND STORE IN OBJECT
    // asm 000020F7: 	LDI	AR3,R2
    // asm 000020F8: 	LDI	AR4,AR2
    // asm 000020F9: 	ADDI	OMATRIX,AR2
    // asm 000020FA: 	CALL	CPYIMAT     		;invert matrix and stuff in object
    // ***GET CAR HEIGHT AND LOAD IT INTO CAR
    // asm 000020FB: 	LDF	*+AR6(1),R0		;GET Y HEIGHT FIRST POINT
    // asm 000020FC: 	SUBF	*+AR6(CARWHLTAB+1),R0
    // asm 000020FD: 	STF	R0,*+AR4(OPOSY)
    // asm 000020FE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETNMAT", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CHECK OBJECT COLLISION WITH VECTOR IN X/Z SPACE
 *
 *PARAMETERS
 *	AR2	OBJECT POINTER
 *	R2	VECTOR POINTER
 *
 *RETURNS
 *	CS=collision, CC= No collision
 *	VL contains 3 pointers which point to VECTORs which
 *	define the normal to this polygon where the 1st collision has
 *	been found.
 *
 *TRASHES
 *	R0,R1,R2,R3
 *
 */
void _obj_coll(void) {
    // asm 000020FF: 	PUSH	R4
    // asm 00002100: 	PUSH	R5
    // asm 00002101: 	PUSH	AR1
    // asm 00002102: 	PUSH	AR3
    // asm 00002103: 	PUSH	AR4
    // asm 00002104: 	PUSH	AR5
    // asm 00002105: 	PUSH	AR6
    // asm 00002106: 	PUSH	AR7
    // asm 00002107: 	LDI	*+AR2(OROMDATA),AR4
    // asm 00002108: 	ADDI	1,AR4			;skip object diameter
    // asm 00002109: 	LDI	AR2,AR5
    // asm 0000210A: 	ADDI	OMATRIX,AR5		;rotational matrix
    // asm 0000210B: 	LDPI	@BLOWLISTI,AR3		;blowlist pointer = AR3
    // asm 0000210C: 	LDI	@transvectorYI,AR6	;transvector temp store
    // asm 0000210D: 	LDI	@tmpmatY,AR7		;TEMP VECTOR STORE
    // asm 0000210E: 	LDI	8,IR0
    // asm 0000210F: 	LDI	R2,AR1			;create translation (TRANS = OBJPOS - COLLPOS)
    // asm 00002110: 	LDF	*+AR2(OPOSX),R0
    // asm 00002111: 	SUBF	*AR1++,R0
    // asm 00002112: 	STF	R0,*-AR6(1)		;transvector.x
    // asm 00002113: 	LDF	*+AR2(OPOSY),R0
    // asm 00002114: 	SUBF	*AR1++,R0
    // asm 00002115: 	STF	R0,*AR6			;transvector.y
    // asm 00002116: 	LDF	*+AR2(OPOSZ),R0
    // asm 00002117: 	SUBF	*AR1,R0
    // asm 00002118: 	STF	R0,*+AR6(1)		;transvector.z
    // asm 00002119: 	LDI	*AR4++,RC
    // asm 0000211A: 	LDI	RC,BK
    // asm 0000211B: 	AND	0FFh,RC
    // asm 0000211C: 	RPTB	EOTV
    // asm 0000211D: 	LDI	*AR4++,R4
    // asm 0000211E: 	LDI	R4,R3
    // asm 0000211F: 	ASH	-16,R4
    // asm 00002120: 	LS	16,R3
    // asm 00002121: 	ASH	-16,R3
    // asm 00002122: 	FLOAT	R3
    // asm 00002123: 	FLOAT	R4
    // asm 00002124:  	STF	R3,*-AR7(1)
    // asm 00002125: 	FLOAT	*AR4++,R2		;get z element of source 1
    // asm 00002125:  ||	STF	R4,*AR7
    // *
    // *MULTIPLY BY ROTATION MATRIX
    // *AND ADD TRANSLATION (IN THAT ORDER)
    // *
    // asm 00002126: 	MPYF3	*AR5++,R3,R0
    // asm 00002126:  ||	STF	R2,*+AR7(1)  		;STORE OUT Z ELEMENT
    // asm 00002127: 	MPYF3	*AR5++,R4,R1
    // asm 00002128: 	MPYF3	*AR5++,*+AR7(1),R1
    // asm 00002128:  ||	ADDF3	R0,R1,R2
    // asm 00002129: 	MPYF3	*AR5++,*-AR7(1),R0
    // asm 00002129:  ||	ADDF3	R1,R2,R2
    // asm 0000212A: 	ADDF	*-AR6(1),R2		;*blowlist++   += translation[X]
    // asm 0000212B: 	MPYF3	*AR5++,R4,R1
    // asm 0000212B:  ||	STF	R2,*AR3++ 		;STORE ROTATED X
    // asm 0000212C: 	MPYF3	*AR5++,*+AR7(1),R1
    // asm 0000212C:  ||	ADDF3	R0,R1,R2
    // asm 0000212D: 	MPYF3	*AR5++,*-AR7(1),R0
    // asm 0000212D:  ||	ADDF3	R1,R2,R3
    // asm 0000212E: 	ADDF	*AR6,R3			;*blowlist++   += translation[Y]
    // asm 0000212F: 	MPYF3	*AR5++,R4,R1
    // asm 0000212F:  ||	STF	R3,*AR3++		;STORE ROTATED Y
    // asm 00002130: 	MPYF3	*AR5--(IR0),*+AR7(1),R1
    // asm 00002130:  ||	ADDF3	R0,R1,R2
    // asm 00002131: 	ADDF	R1,R2			;FORM ROTATED Z
    // asm 00002132: 	ADDF	*+AR6(1),R2		;ADD IN TRANSLATION Z
EOTV:
    // asm 00002133: STF	R2,*AR3++		;STORE Z
    // asm 00002134: 	LDPI	@BLOWLISTI,IR1		;blowlist pointer = IR1
    // asm 00002135: 	LDI	IR1,IR0
    // asm 00002136: 	ADDI	2,IR0
    // asm 00002137: 	LDI	0FFh,R4
    // asm 00002138: 	LDI	-8,R5
    // ;	BD	VLINST
    // asm 00002139: 	INC	AR4			;skip control of 1st polygon
    // asm 0000213A: 	LDI	BK,RC
    // asm 0000213B: 	RS	16,RC
    // 	;----->BD	VLINST
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_obj_coll", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*CHECK OBJECT COLLISION WITH VECTOR IN X/Z SPACE
*
*THIS ROUTINE MUST BE FIRST BECAUSE OF .ALIGN BUG IN LINKER/ASSEMBLER
;	.align
*/

/*
;	NOP
;	NOP
;	NOP
*/

/*
 *----------------------------------------------------------------------------
 *MAKBOX	GET XYZPLUS-MINUS VALUES FOR CAR
 *       MAKE WHEEL OFFSET TABLE
 *PARAMETERS
 *	AR4	OBJECT WITH OCARBLK
 *RETURNS
 *	GETS CARXYZPLUS/MINUS IN OCARBLK
 *	     CARWHLTAB IN OCARBLK
 *TRASHES
 *	R0-R2
 */
void _makbox(OBJ* obj /*AR4*/) {
    (void)obj;
    // asm 00002185: 	PUSH	R3
    // asm 00002186: 	PUSH	R4
    // asm 00002187: 	PUSH	R5
    // asm 00002188: 	PUSH	R6
    // asm 00002189: 	PUSHF	R6
    // asm 0000218A: 	PUSH 	AR0
    // asm 0000218B: 	LDI	*+AR4(OROMDATA),AR0
    // asm 0000218C: 	ADDI	1,AR0			;skip object diameter
    // asm 0000218D: 	LDI	*AR0++,RC     		;GET VERTEX COUNT
    // asm 0000218E: 	AND	0FFh,RC
    // asm 0000218F: 	SUBI	1,RC
    // asm 00002190: 	LDI	*AR0++,R3		;GET Y:X
    // asm 00002191: 	LDI	R3,R4
    // asm 00002192: 	LS	16,R3
    // asm 00002193: 	ASH	-16,R3			;UNPACK X  X1->R3 OXPLUS
    // asm 00002194: 	ASH	-16,R4			;UNPACK Y  Y1->R4 OYPLUS
    // asm 00002195: 	FLOAT	R3
    // asm 00002196: 	FLOAT	R4
    // asm 00002197: 	FLOAT	*AR0++,R5		;Z1->R5 OZPLUS
    // asm 00002198: 	LDF	R3,R0			;X1->R0 OXMINUS
    // asm 00002199: 	LDF	R4,R1			;Y1->R1 OYMINUS
    // asm 0000219A: 	LDF	R5,R2			;Z1->R2 OZMINUS
    // asm 0000219B: 	RPTB	MBVL
    // asm 0000219C: 	LDI	*AR0,R6			;GET Y:X
    // asm 0000219D: 	LS	16,R6
    // asm 0000219E: 	ASH	-16,R6			;UNPACK X
    // asm 0000219F: 	FLOAT	R6
    // asm 000021A0: 	CMPF	R3,R6			;CHECK X(N) > XPLUS
    // asm 000021A1: 	LDFGT	R6,R3
    // asm 000021A2: 	CMPF	R0,R6			;CHECK X(N) < XMINUS
    // asm 000021A3: 	LDFLT	R6,R0
    // asm 000021A4: 	LDI	*AR0++,R6		;GET Y:X
    // asm 000021A5: 	ASH	-16,R6			;UNPACK Y
    // asm 000021A6: 	FLOAT	R6
    // asm 000021A7: 	CMPF	R4,R6			;CHECK Y(N) > YPLUS
    // asm 000021A8: 	LDFGT	R6,R4
    // asm 000021A9: 	CMPF	R1,R6			;CHECK Y(N) < YMINUS
    // asm 000021AA: 	LDFLT	R6,R1
    // asm 000021AB: 	FLOAT	*AR0++,R6		;XN
    // asm 000021AC: 	CMPF	R5,R6			;CHECK Z(N) > ZPLUS
    // asm 000021AD: 	LDFGT	R6,R5
    // asm 000021AE: 	CMPF	R2,R6			;CHECK Z(N) < ZMINUS
MBVL:
    // asm 000021AF: LDFLT	R6,R2
    // asm 000021B0: 	LDI	*+AR4(OCARBLK),AR0
    // *STORE WHEEL OFFSET TABLE
    // asm 000021B1: 	LDF	0,R6
    // asm 000021B2: 	STF	R6,*+AR0(CARWHLTAB+0) 		;CENTER POINT BOTTOM
    // asm 000021B3: 	STF	R4,*+AR0(CARWHLTAB+1)
    // asm 000021B4: 	STF	R6,*+AR0(CARWHLTAB+2)
    // asm 000021B5: 	STF	R3,*+AR0(CARWHLTAB+3)		;RT FRONT BOTTOM
    // asm 000021B6: 	STF	R4,*+AR0(CARWHLTAB+4)
    // asm 000021B7: 	STF	R5,*+AR0(CARWHLTAB+5)
    // asm 000021B8: 	STF	R0,*+AR0(CARWHLTAB+6)		;LFT FRONT BOTTOM
    // asm 000021B9: 	STF	R4,*+AR0(CARWHLTAB+7)
    // asm 000021BA: 	STF	R5,*+AR0(CARWHLTAB+8)
    // asm 000021BB: 	STF	R0,*+AR0(CARWHLTAB+9)	  	;LFT REAR BOTTOM
    // asm 000021BC: 	STF	R4,*+AR0(CARWHLTAB+10)
    // asm 000021BD: 	STF	R2,*+AR0(CARWHLTAB+11)
    // asm 000021BE: 	STF	R3,*+AR0(CARWHLTAB+12)		;RT REAR BOTTOM
    // asm 000021BF: 	STF	R4,*+AR0(CARWHLTAB+13)
    // asm 000021C0: 	STF	R2,*+AR0(CARWHLTAB+14)
    // *STORE XYZ PLUS/MINUS
    // asm 000021C1: 	ADDF	25.0,R0			;MAKE IT A LITTLE SMALLER
    // asm 000021C2: 	STF	R0,*+AR0(CARXMINUS)
    // asm 000021C3: 	STF	R1,*+AR0(CARYMINUS)
    // ;	ADDF	15.0,R2			;MAKE IT A LITTLE SMALLER
    // asm 000021C4: 	STF	R2,*+AR0(CARZMINUS)
    // asm 000021C5: 	ADDF	-25.0,R3		;MAKE IT A LITTLE SMALLER
    // asm 000021C6: 	STF	R3,*+AR0(CARXPLUS)
    // asm 000021C7: 	STF	R4,*+AR0(CARYPLUS)
    // ;	ADDF	-15.0,R5		;MAKE IT A LITTLE SMALLER
    // asm 000021C8: 	STF	R5,*+AR0(CARZPLUS)
    // asm 000021C9: 	POP	AR0
    // asm 000021CA: 	POPF	R6
    // asm 000021CB: 	POP	R6
    // asm 000021CC: 	POP	R5
    // asm 000021CD: 	POP	R4
    // asm 000021CE: 	POP	R3
    // asm 000021CF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_makbox", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *COLLISION SCAN
 *
 */
void COLSCC(void) {
    u32 mode = _MODE & MMODE; /* MAKE SURE MODE IS IN GAME */

    if (mode == MATTR) {  /* cm */
        ATTR_COLLISION(); /* cm */
        return;
    }

    if (mode != MGAME) {
        return;
    }

    /*
     * ROOT PROCESS
     *
     * Original sets:
     *   AR7 = &PACTIVE
     *
     * This probably matters because some collision routines may call
     * process-related helpers expecting AR7 to be the root process/list node.
     * In C, only keep this if your mproc layer needs current/root context.
     */
    CURRENT_PROC = NULL; /* or PRC_ROOT / &PACTIVE sentinel, depending on your design */

    /*
     * TEST CODE
     *
     * CKRAD();
     */

    COLSCAN();      /* PLAYER VS. DRONES */
    CLDSCAN();      /* DRONES VS. DRONES */
    PLYRSIGN();     /* PLAYER VS. SIGNS, POLES, TREES */
    PLYRDEBRIS();   /* PLAYER VS. ROAD DEBRIS */
    DRONSIGN();     /* DRONES VS. SIGNS, POLES, TREES */
    DRONDEBRIS();   /* DRONES VS. ROAD DEBRIS */
    PLYRROADKILL(); /* PLAYER VS. ROADKILL */
    DEBSCAN();      /* CLEAR OUT DEAD DEBRIS */
}

void ATTR_COLLISION(void) {
    // if (_ATTR_MODE == -3)      /* BUG IN FUTURE */
    //     ...

    /*
     * This check makes sure the code is not active
     * during the high score display. Thus allowing
     * use to set MATTR mode during high score
     * display while in the attract mode.
     */
    if (_MODE & MHS) {
        return;
    }

    /*
     * ROOT PROCESS
     *
     * Original:
     *   AR7 = &PACTIVE
     */
    // CURRENT_PROC = NULL;

    CLDSCAN();    /* DRONES VS. DRONES */
    DRONSIGN();   /* DRONES VS. SIGNS, POLES, TREES */
    DRONDEBRIS(); /* DRONES VS. ROAD DEBRIS */
    DEBSCAN();    /* CLEAR OUT DEAD DEBRIS */
}

/*
 *----------------------------------------------------------------------------
 *PLAYER COLLIDE WITH DEBRIS
 *
 */
static void PLYRDEBRIS(void) {
    // asm 000021E9: 	LDPI	@_plyr1+PLY_CAR,AR0	;GET PLAYER CAR
    // asm 000021EA: 	LDPI	@ROAD_DEBRISI,AR1
    // asm 000021EB: 	B	COLPOINT
    // *
    // *PLAYER COLLIDE WITH SIGNS
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRDEBRIS", 0, 0);
    UNIMPL();
}

static void PLYRSIGN(void) {
    // asm 000021EC: 	LDPI	@_plyr1+PLY_CAR,AR0	;GET PLAYER CAR
    // asm 000021ED: 	LDPI	@SIGN_LISTI,AR1
    // asm 000021EE: 	B	COLPOINT
    // *
    // *DRONE COLLIDE WITH DEBRIS
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRSIGN", 0, 0);
    UNIMPL();
}

static void DRONDEBRIS(void) {
    // asm 000021EF: 	LDPI	@ROAD_DEBRISI,AR1
    // asm 000021F0: 	B	DRONEPT
    // *
    // *DRONE COLLIDE WITH SIGNS
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONDEBRIS", 0, 0);
    UNIMPL_TODO();
}

static void DRONSIGN(void) {
    // asm 000021F1: 	LDPI	@SIGN_LISTI,AR1
DRONEPT:
    // asm 000021F2: 	LDPI	@CAR_LIST,R0	 	;GET LIST AND CHECK NULL
    // asm 000021F3: 	LDI	R0,AR0
    // asm 000021F4: 	RETSZ
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONSIGN", 0, 0);
    UNIMPL_TODO();
}

static void DRONEPTL(void) {
    // asm 000021F5: 	LDI	*+AR0(OID),R0
    // asm 000021F6: 	CMPI	DRONE_C|HELICOPTER,R0
    // asm 000021F7: 	BEQ	DRONEPT1
    // asm 000021F8: 	LDI	*+AR0(ODIST),R1	  	;IF DUDE OFFSCREEN, IGNORE
    // asm 000021F9: 	BN	DRONEPT1
    // asm 000021FA: 	LDI	25000,R2       		;CHECK IF FAR DISTANT
    // asm 000021FB: 	MPYI	2,R2
    // asm 000021FC: 	CMPI	R2,R1
    // asm 000021FD: 	BGT	DRONEPT1		;IF FAR IN DISTANCE, IGNORE
    // asm 000021FE: 	PUSH	AR1
    // asm 000021FF: 	PUSH	AR0
    // asm 00002200: 	CALL	COLPOINT	       	;CHECK CAR/POINT COLLISION
    // asm 00002201: 	POP	AR0
    // asm 00002202: 	POP	AR1
DRONEPT1:
    // asm 00002203: 	LDI	*+AR0(OLINK3),R0
    // asm 00002204: 	LDI	R0,AR0
    // asm 00002205: 	BNZ	DRONEPTL
    // asm 00002206: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONEPTL", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *SIGN/TREE COLLISION SCAN WITH CAR
 *
 *CHECK PLAYER CAR AGAINST LIST
 *PARAMETERS
 *	AR0	CAR OBJECT
 *	AR1	ADDRESS OF LIST HEADER
 *
 *DO NOT MODIFY THE BEHAVIOR THAT HAS BEEN DEFINED HERE:
 *	  only the TYPE is used to determine behavior
 *	  object is NOT pulled from any list (SIGN or OTHERWISE)
 *
 *
 */
static void COLPOINT(void) {
    // asm 00002207: 	BD	CARSCL0			;GET FIRST GUY
    // asm 00002208: 	LDF	*+AR0(OPOSX),R2		;GET X COORD
    // asm 00002209: 	LDF	*+AR0(OPOSZ),R3		;GET Z COORD
    // asm 0000220A: 	SUBI	OLINK3,AR1		;SETUP INDEXING
    // 	;------->BU CARSCL0
CARSCLP0:
    // asm 0000220B: 	MPYF	R4,R4
    // asm 0000220C: 	ADDF	R0,R4
    // asm 0000220D: 	CMPF	R5,R4	 		;ARE WE WITHIN RADIUS?
    // asm 0000220E: 	BGT	CARSCL			;NO, KEEP GOING
    // asm 0000220F: 	LDI	*+AR1(OFLAGS),R0
    // asm 00002210: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // asm 00002211: 	BNZ	CARSCL
    // asm 00002212: 	CALL 	COLSGCK
    // asm 00002213: 	LDF	*+AR0(OPOSX),R2		;GET X COORD
    // asm 00002214: 	LDF	*+AR0(OPOSZ),R3		;GET Z COORD
CARSCL0:
    // asm 00002215: 	FLOAT	*+AR0(ORAD),R5	 	;GET CAR RADIUS
    // asm 00002216: 	LDI	OPOSZ,IR0
    // asm 00002217: 	MPYF	R5,R5
CARSCL:
    // asm 00002218: 	LDI	*+AR1(OLINK3),AR1
    // asm 00002219: 	LDI	AR1,R0
    // asm 0000221A: 	BNZD	CARSCLP0
    // asm 0000221B: 	SUBF	*+AR1(OPOSX),R2,R0
    // asm 0000221C: 	MPYF	R0,R0
    // asm 0000221D: 	SUBF	*+AR1(IR0),R3,R4
    // ********BNZD	CARSCLP0
    // asm 0000221E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLPOINT", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CHECK POINT VERSUS XZ BOX
 *
 *PARAMETERS
 *	AR0	POINTS TO CAR OBJECT
 *	AR1	POINTS TO SIGN/POLE OBJECT
 *
 */
void COLSGCK(void) {
    // asm 0000221F: 	PUSH	AR0
    // asm 00002220: 	PUSH	AR1
    // asm 00002221: 	LDPI	@BLOWLISTI,AR2
    // asm 00002222: 	CALL	GETBOX			;GET BOX POINTS FOR OBJECT 1
    // *
    // *CHECK 4 LINE EQUATIONS FOR BOTTOM OF CAR
    // *PT 2-6-7-3
    // asm 00002223: 	LDPI	@LEQTABI,AR2
    // asm 00002224: 	LDI	3,RC 			;DO 4 EQUATIONS
    // asm 00002225: 	RPTB	CSGLNEQ
    // asm 00002226: 	LDI	*AR2++,AR3		;GET 2 POINTS
    // asm 00002227: 	LDI	*AR2,AR4
    // asm 00002228: 	SUBF	*+AR3(1),*+AR4(1),R0	;A
    // asm 00002229: 	SUBF	*-AR4(1),*-AR3(1),R1	;B
    // asm 0000222A: 	MPYF	R0,*-AR3(1),R2
    // asm 0000222B: 	MPYF	R1,*+AR3(1),R3
    // asm 0000222C: 	ADDF	R3,R2
    // asm 0000222D: 	NEGF	R2			;C
    // *EVALUATE THE POINT
    // asm 0000222E: 	MPYF	*+AR1(OPOSX),R0		;AX
    // asm 0000222F: 	MPYF	*+AR1(OPOSZ),R1		;BZ
    // asm 00002230: 	ADDF	R0,R1
    // asm 00002231: 	ADDF	R1,R2
    // asm 00002232: 	BLE	COLSGCX
CSGLNEQ:
    // asm 00002233: NOP
    // *
    // *GOT A COLLISION
    // *CHECK TYPE
    // *
    // asm 00002234: 	LDI	*+AR0(OCARBLK),AR5	;GET VELOCITY DIRECTION
    // asm 00002235: 	LDI	*+AR1(OID),R0
    // asm 00002236: 	AND	TYPE_M,R0
    // asm 00002237: 	CMPI	TSC_IGNORE,R0
    // asm 00002238: 	BEQ	SIGN_IGNORE
    // ;herein lies the start of our bugs...
    // ;
    // ;
    // ;
    // asm 00002239: 	LDI	*+AR1(OID),R0
    // asm 0000223A: 	AND	CLASS_M|TYPE_M,R0
    // asm 0000223B: 	CMPI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm 0000223C: 	BEQ	ROADKILL
    // asm 0000223D: 	LDI	*+AR1(OID),R0
    // asm 0000223E: 	AND	TYPE_M,R0
    // asm 0000223F: 	CMPI	TSC_FLYING,R0
    // asm 00002240: 	BEQ	FLYCOLL
    // asm 00002241: 	CMPI	TSC_RUNOVER,R0
    // asm 00002242: 	BEQ	RUNOVER
    // *
    // *IMMOBILE SIGN
    // *
    // *PARAMETERS
    // *	AR0	POINTS TO CAR OBJECT
    // *	AR1	POINTS TO SIGN/POLE OBJECT
    // *	AR5	CARBLOCK
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLSGCK", 0, 0);
    UNIMPL();
}

static void HARDCOL(void) {
    // asm 00002243: 	LDI	*+AR1(OID),R0
    // asm 00002244: 	AND	CLASS_M|TYPE_M|SUBTYPE_M,R0
    // asm 00002245: 	CMPI	TSIGN_C|TSC_IMMOBILE|TSC_V_PALM,R0
    // asm 00002246: 	BNE	NOTCOCONUT
    // asm 00002247: 	PUSH	R0
    // asm 00002248: 	PUSH	R2
    // asm 00002249: 	PUSH	AR0
    // asm 0000224A: 	PUSH	AR2
    // asm 0000224B: 	PUSH	AR3
    // asm 0000224C: 	PUSH	AR5
    // asm 0000224D: 	LDI	AR0,AR5		;parent object
    // asm 0000224E: 	RANDN	3
    // asm 00002250: 	LDI	R0,AR3
    // asm 00002251: LL88
    // asm 00002251: 	CREATE	DROP_COCONUTS,TSIGN_C|TSC_IMMOBILE|TSC_V_PALM
    // asm 00002254: 	DBU	AR3,LL88
    // asm 00002255: 	POP	AR5
    // asm 00002256: 	POP	AR3
    // asm 00002257: 	POP	AR2
    // asm 00002258: 	POP	AR0
    // asm 00002259: 	POP	R2
    // asm 0000225A: 	POP	R0
    // asm 0000225B: 	BU	DOREPEL
NOTCOCONUT:
    // asm 0000225C: 	AND	TYPE_M,R0		;REDWOODS MUST NOT GET KNOCKED OVER
    // asm 0000225D: 	CMPI	TSC_HARD,R0
    // asm 0000225E: 	BNE	RUNOVER
    // *REPELL THE SUCKERS
DOREPEL:
    // asm 0000225F: 	CALL	REPELL
    // asm 00002260: 	MPYF	*AR2,R0,R1 			;X REPELL
    // asm 00002261: 	MPYF	*+AR2(2),R0		     	;Z REPELL
    // asm 00002262: 	ADDF	*+AR0(OPOSX),R1			;REPELL THE SUCKER (AR0)
    // asm 00002263: 	ADDF	*+AR0(OPOSZ),R0
    // asm 00002264: 	STF	R1,*+AR0(OPOSX)
    // asm 00002265: 	STF	R0,*+AR0(OPOSZ)
    // asm 00002266: 	LDF	*+AR5(CARSPEED),R2    		;GOING FAST???
    // asm 00002267: 	MPYF	0.6,R2
    // asm 00002268: 	CMPF	37,R2				;MINIMUM SPEED VALUE
    // asm 00002269: 	LDFLT	37,R2
    // asm 0000226A: 	STF	R2,*+AR5(CARSPEED)		;REVERSE SPEED
    // asm 0000226B: 	BLT	HARDCOL00			;YES,SPINOUT
    // asm 0000226C: 	LDI	500,AR2				;STRAIGHT OR SPINNER?
    // asm 0000226D: 	CALL	RANDPER
    // asm 0000226E: 	BC	HARDCOL1			;SPINNER...
    // *STRAIGHT KICKBACK
HARDCOL00:
    // asm 0000226F: 	LDI	60,R0				;SPIN COUNT
    // asm 00002270: 	STI	R0,*+AR5(CAR_SPIN)
    // asm 00002271: 	LDI	AR0,AR4
    // asm 00002272: 	CALL	GETNXTRDIR
    // asm 00002273: 	LDI	AR4,AR0
    // asm 00002274: 	LDF	R0,R2
    // asm 00002275: 	SUBF	*+AR5(CARYROT),R2
    // asm 00002276: 	CALL	NORMITS
    // asm 00002277: 	LDF	R2,R2
    // asm 00002278: 	LDFGT	0.02,R0
    // asm 00002279: 	LDFLE	-0.02,R0
    // asm 0000227A: 	B	HARDCOL2			;STORE DROT, SET VROT
    // *SPIN THE DUDE
HARDCOL1:
    // asm 0000227B: 	LDI	1,R0				;SPIN THE DUDE
    // asm 0000227C: 	STI	R0,*+AR5(CAR_SPIN)
    // asm 0000227D: 	LDF	3.14,R1				;SET 180 MIN SPIN
    // asm 0000227E: 	STF	R1,*+AR5(CARSPRAD)
    // asm 0000227F: 	LDF	0.1,R0		 		;GET ROTATION AMOUNT
HARDCOL2:
    // asm 00002280: 	STF	R0,*+AR5(CARDROT)
    // asm 00002281: 	LDPI	@VECTORAI,AR2    		;COMPUTE REPULSION VECTOR
    // asm 00002282: 	LDF	*AR2,R2
    // asm 00002283: 	LDF	*+AR2(2),R3
    // asm 00002284: 	CALL	ARCTANF
    // asm 00002285: 	SUBF	1.57,R0
    // asm 00002286: 	SUBF	*+AR5(CARVROT),R0
    // asm 00002287: 	LDF	R0,R2
    // asm 00002288: 	CALL	NORMITS
    // asm 00002289: 	ABSF	R2
    // asm 0000228A: 	CMPF	1.57,R2
    // asm 0000228B: 	BLT	HARDCOL3
    // asm 0000228C: 	LDF	*+AR5(CARVROT),R2		;REVERSE VELOCITY
    // asm 0000228D: 	LDF	R2,R3
    // asm 0000228E: 	ADDF	3.14,R2
    // asm 0000228F: 	CALL	NORMITS
    // asm 00002290: 	STF	R2,*+AR5(CARVROT)
    // *SHAKE THE TREE
    // asm 00002291: 	LDI	*+AR1(OID),R0
    // asm 00002292: 	AND	TYPE_M,R0
    // asm 00002293: 	CMPI	TSC_HARD,R0
    // asm 00002294: 	BEQ	HARDCOL3
    // asm 00002295: 	LDF	R3,R2
    // asm 00002296: 	LDI	AR1,AR2		 	;FORM OMATRIX POINTER
    // asm 00002297: 	ADDI	OMATRIX,AR2		;STUFF
    // asm 00002298: 	CALL    FIND_YMATRIX		;NEW MATRIX
    // asm 00002299: 	LDI	AR1,AR4			;GET SIGN OBJECT POINTER
    // asm 0000229A: 	LDI	*+AR4(OFLAGS),R0	;MAKE IT SELF ROTATING
    // asm 0000229B: 	ANDN	O_POSTER,R0
    // asm 0000229C: 	LDI	1,R1			;SET 3D ROTATION BIT
    // asm 0000229D: 	LS	O_3DROT_B,R1
    // asm 0000229E: 	OR	R1,R0
    // asm 0000229F: 	STI	R0,*+AR4(OFLAGS)
    // asm 000022A0: 	LDF	*+AR5(CARSPEED),R7	;HIT CAR SPEED
    // asm 000022A1: 	MPYF	0.04,R7		   	;FALL RATE BASED UPON VELOCITY
    // asm 000022A2: 	CMPF	0.13,R7
    // asm 000022A3: 	LDFLT	0.13,R7
    // asm 000022A4: 	CMPF	1.0,R7
    // asm 000022A5: 	LDFGT	1.0,R7
    // asm 000022A6: 	LDPI	@TREESHAKI,AR2		;GET SIGN FALL PROCESS
    // asm 000022A7: 	LDI	DRONE_C|FLYER_T,R2
    // asm 000022A8: 	CALL	PRC_CREATE_CHILD		;CREATE A CHILD PROCESS
HARDCOL3:
    // asm 000022A9: 	SONDFX	POLESND			;MAKE SOUND
    // asm 000022AB: 	B	COLSGCX
    // *
    // *ROADKILL OBJECT
    // *	AR0	POINTS TO CAR OBJECT
    // *	AR1	POINTS TO ROADKILL OBJECT HIT
    // *
ROADKILL:
    // asm 000022AC: 	LDI	1,R0
    // asm 000022AD: 	STI	R0,*+AR1(OCARBLK)
    // asm 000022AE: 	CALL	ROADKILL_HIT		;MAKE A SOUND
    // asm 000022AF: 	CALL	ROADKILL_FLYERP
    // asm 000022B0: 	BC	FLYCOLL
    // asm 000022B1: 	B	COLSGCX
    // *
    // *FLYING OBJECT
    // *	AR0	POINTS TO CAR OBJECT
    // *	AR1	POINTS TO OBJECT HIT
FLYCOLL:
    // asm 000022B2: 	LDF	*+AR0(OPOSY),R0		;MAKE SURE HEIGHT IS CLOSE
    // asm 000022B3: 	SUBF	*+AR1(OPOSY),R0
    // asm 000022B4: 	ABSF	R0
    // asm 000022B5: 	FLOAT	250,R1
    // asm 000022B6: 	CMPF	R1,R0
    // asm 000022B7: 	BGT	COLSGCX
    // asm 000022B8: 	LDF	0.10,R0 		;ADD RANDOM ROTATION
    // asm 000022B9: 	CALL	SFRAND
    // asm 000022BA: 	LDF	R0,R2
    // asm 000022BB: 	ADDF	*+AR5(CARVROT),R2
    // asm 000022BC: 	LDF	0.65,R0			;RANDOM SPEED MULTIPLIER
    // asm 000022BD: 	CALL	FRAND
    // asm 000022BE: 	LDF	R0,R1
    // asm 000022BF: 	ADDF	0.8,R1
    // asm 000022C0: 	MPYF	1.5,R1			;SPEEDFUDGE FACTOR
    // asm 000022C1: 	CALL	_SINE
    // asm 000022C2: 	NEGF	R0,R3
    // asm 000022C3: 	CALL	_COSI
    // asm 000022C4: 	MPYF	*+AR5(CARSPEED),R1	;GET CURRENT SPEED
    // asm 000022C5: 	MPYF	R1,R3
    // asm 000022C6: 	MPYF	R1,R0
    // asm 000022C7: 	STF	R3,*+AR1(OVELX)	       	;SETUP VELOCITIES
    // asm 000022C8: 	STF	R0,*+AR1(OVELZ)
    // asm 000022C9: 	LDF	-0.3,R0
    // asm 000022CA: 	CALL	FRAND
    // asm 000022CB: 	ADDF	-0.2,R0
    // asm 000022CC: 	MPYF	1.5,R0			;SPEEDFUDGE FACTOR
    // asm 000022CD: 	MPYF	*+AR5(CARSPEED),R0	;GET CURRENT SPEED
    // asm 000022CE: 	CMPF	-65,R0
    // asm 000022CF: 	LDFLT	-65,R0		  	;MAX VERTICAL VELOCITY
    // asm 000022D0: 	STF	R0,*+AR1(OVELY)		;STUFF VERTICAL VELOCITY
    // asm 000022D1: 	LDI	AR1,AR4			;GET SIGN OBJECT POINTER
    // asm 000022D2: 	LDI	AR0,AR3			;SAVE CAR OBJECT
    // asm 000022D3: 	LDI	1,R0
    // asm 000022D4: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm 000022D5: 	TSTB	*+AR4(OFLAGS),R0	;PROCESS ALREADY ACTIVE	?
    // asm 000022D6: 	LDINZ	*+AR4(OPLINK),AR2	;YES, KILL HIM OFF...
    // asm 000022D7: 	BZ	CLLL1
    // asm 000022D8: 	CALL	PRC_KILL		;DONT FUCK WITH THIS PRIBYL!!!!
CLLL1:
    // asm 000022D9: 	LDPI	@FLYCOLLPI,AR2		;GET SIGN FLY PROCESS
    // asm 000022DA: 	LDI	DRONE_C|FLYER_T,R2
    // asm 000022DB: 	CALL	PRC_CREATE_CHILD		;CREATE A CHILD PROCESS
    // asm 000022DC: 	BC	COLSGCX			;NOTHING AVAILABLE, QUIT
    // asm 000022DD: 	STI	AR0,*+AR4(OPLINK)	;SAVE PROCESS LINK
    // asm 000022DE: 	LDI	1,R0
    // asm 000022DF: 	LSH	O_3DROT_B,R0		;FLAG CAN AS NON-2D OPTIMIZABLE
    // asm 000022E0: 	LDI	1,R1
    // asm 000022E1: 	LSH	O_PROC_B,R1		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm 000022E2: 	ADDI	R1,R0
    // asm 000022E3: 	OR	*+AR4(OFLAGS),R0	;SET YOUR FLAGS...
    // asm 000022E4: 	STI	R0,*+AR4(OFLAGS)
    // asm 000022E5: 	LDI	0,R0			;KILL OFF GROUP REFERENCE
    // asm 000022E6: 	STI	R0,*+AR4(OLINK2)
    // *GET SOUND FOR SAWHORSE/DRUM HIT
    // asm 000022E7: 	LDI	*+AR4(OID),R2
    // asm 000022E8: 	LDI	R2,R0
    // asm 000022E9: 	AND	TYPE_M,R0
    // asm 000022EA: 	CMPI	TSC_ROADKILL,R0
    // asm 000022EB: 	BNE	KLFD
    // asm 000022EC: 	CALL	ROADKILL_SETKILL
    // asm 000022ED: 	BU	COLSGCX
KLFD:
    // asm 000022EE: 	AND	SUBTYPE_M,R2
    // asm 000022EF: 	CMPI	@PLYCAR,AR3		;PLAYERS CAR?
    // asm 000022F0: 	BNZ	FLYCOLL1		;NO...
    // asm 000022F1: 	CMPI	RDD_55GAL,R2
    // asm 000022F2: 	LDIEQ	DRUMSND,AR2
    // asm 000022F3: 	LDINE	SIGNSND,AR2
    // asm 000022F4: 	CALL	ONESNDFX
    // asm 000022F5: 	B	COLSGCX
FLYCOLL1:
    // asm 000022F6: 	CMPI	RDD_55GAL,R2
    // asm 000022F7: 	LDIEQ	DRMBNCE,AR2
    // asm 000022F8: 	LDINE	DSIGNSND,AR2
    // asm 000022F9: 	B	COLSGCX0
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HARDCOL", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *RUNOVER SIGN
 *REMOVE SIGN FROM SIGN SUP LIST
 *START SIGN PROCESS TO MAKE IT FALL
 *
 *PARAMETERS
 *	AR0	POINTS TO CAR OBJECT
 *	AR1	POINTS TO SIGN/POLE OBJECT
 *	AR5	CARBLOCK
 */
static void RUNOVER(void) {
    // asm 000022FA: 	LDF	*+AR5(CARVROT),R2
    // asm 000022FB: 	LDI	AR1,AR2		 	;FORM OMATRIX POINTER
    // asm 000022FC: 	ADDI	OMATRIX,AR2		;STUFF
    // asm 000022FD: 	CALL    FIND_YMATRIX		;NEW MATRIX
    // asm 000022FE: 	LDI	AR1,AR4			;GET SIGN OBJECT POINTER
    // asm 000022FF: 	LDI	AR0,AR3			;SAVE CAR OBJECT
    // asm 00002300: 	LDI	*+AR4(OFLAGS),R0	;MAKE IT SELF ROTATING
    // asm 00002301: 	ANDN	O_POSTER,R0
    // asm 00002302: 	STI	R0,*+AR4(OFLAGS)
    // asm 00002303: 	LDF	*+AR5(CARSPEED),R7	;HIT CAR SPEED
    // asm 00002304: 	LDF	R7,R6
    // asm 00002305: 	MPYF	0.2,R6
    // asm 00002306: 	CMPF	10,R6
    // asm 00002307: 	LDFLT	10,R6
    // asm 00002308: 	CMPF	R6,R7
    // asm 00002309: 	LDFLT	R7,R6
    // asm 0000230A: 	LDF	R7,R5
    // asm 0000230B: 	SUBF	R6,R5
    // asm 0000230C: 	STF	R5,*+AR5(CARSPEED)
    // asm 0000230D: 	MPYF	0.03,R7		   	;FALL RATE BASED UPON VELOCITY
    // asm 0000230E: 	CMPF	0.1,R7
    // asm 0000230F: 	LDFLT	0.1,R7
    // asm 00002310: 	CMPF	0.7,R7
    // asm 00002311: 	LDFGT	0.7,R7
    // asm 00002312: 	CALL	FREESIGN		;GET SIGN OFF LIST
    // asm 00002313: 	LDPI	@SIGNFALLI,AR2		;GET SIGN FALL PROCESS
    // asm 00002314: 	LDI	DRONE_C|FLYER_T,R2
    // asm 00002315: 	CALL	PRC_CREATE_CHILD		;CREATE A CHILD PROCESS
    // asm 00002316: 	LDI	*+AR4(OID),R0
    // asm 00002317: 	AND	SUBTYPE_M,R0
    // asm 00002318: 	LDI	SIGNSND,AR2    		;DEFAULT SOUND
    // asm 00002319: 	CMPI	TSC_R_SAGE,R0
    // asm 0000231A: 	BNZ	RUNOV0
    // asm 0000231B: 	LDI	5,AR2
    // asm 0000231C: 	CALL	RANDU0
    // asm 0000231D: 	ADDI	@SAGETABI,R0
    // asm 0000231E: 	LDI	R0,AR2
    // asm 0000231F: 	LDI	*AR2,AR2
    // asm 00002320: 	B	RUNOV00
RUNOV0:
    // asm 00002321: 	CMPI	TSC_R_POLE,R0
    // asm 00002322: 	LDIZ	DONGSND,AR2
    // asm 00002323: 	CMPI	TSC_R_LAMPPOST,R0
    // asm 00002324: 	LDIZ	LAMPSND,AR2
RUNOV00:
    // asm 00002325: 	CMPI	@PLYCAR,AR3		;PLAYERS CAR?
    // asm 00002326: 	BNZ	COLSGCX0		;NO...
    // asm 00002327: 	CALL	ONESNDFX
    // asm 00002328: 	B	COLSGCX
COLSGCX0:
    // asm 00002329: 	CALL	DRONESND1
SIGN_IGNORE:
COLSGCX:
    // asm 0000232A: 	POP	AR1
    // asm 0000232B: 	POP	AR0
    // asm 0000232C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUNOVER", 0, 0);
    UNIMPL();
}

/* asm: SAGETAB	 .WORD	SAGESND,SAGESND1,SAGESND2,SAGESND3,SAGESND */
static int SAGETAB[] = {
    SAGESND,
    SAGESND1,
    SAGESND2,
    SAGESND3,
    SAGESND,
};
/*
 *----------------------------------------------------------------------------
 *FLYING SIGN COLLISION PROCESS
 *
 *AR4= SIGN OBJECT
 *
 */

static void FLYCOLLP(void) {
    // asm 00002334: 	LDF	0.2,R0
    // asm 00002335: 	CALL	SFRAND
    // asm 00002336: 	LDF	R0,R2
    // asm 00002337: 	LDI	AR7,AR2
    // asm 00002338: 	ADDI	PDATA+2,AR2		;STORE MATRIX PDATA+2
    // asm 00002339: 	CALL    FIND_XMATRIX
    // asm 0000233A: 	LDPI	@MATRIXAI,AR2
    // asm 0000233B: 	LDF	0.1,R0
    // asm 0000233C: 	CALL	SFRAND
    // asm 0000233D: 	LDF	R0,R2
    // asm 0000233E: 	CALL    FIND_YMATRIX
    // asm 0000233F: 	LDI	AR7,R2	  		;ROTATE THE SUCKER
    // asm 00002340: 	ADDI	PDATA+2,R2
    // asm 00002341: 	LDI	R2,R3
    // asm 00002342: 	CALL	CONCATMAT
FLYCOLP0:
    // ;	LDI	*+AR4(OFLAGS),R0	;CHECK IF OBJECT ACTIVE, EXISTS
    // ;	AND	O_LIST_M,R0
    // ;	CMPI	O_LIST0,R0
    // ;	BZ	FLYSTOP  		;ERROR DOES NOT EXIST
    // ;	CMPI	O_LIST2,R0		;OBJECT NOT ACTIVE, SPLIT
    // ;	BZ	FLYSTOP
    // asm 00002343: 	LDPI	@NFRAMES,AR6	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm 00002344: 	SUBI	1,AR6
    // asm 00002345: FLYCOLPL
    // asm 00002345: 	LDI	AR7,R2	  		;ROTATE THE SUCKER
    // asm 00002346: 	ADDI	PDATA+2,R2
    // asm 00002347: 	LDI	AR4,AR2
    // asm 00002348: 	ADDI	OMATRIX,AR2
    // asm 00002349: 	LDI	AR2,R3
    // asm 0000234A: 	CALL	CONCATMAT
    // asm 0000234B: 	DBU	AR6,FLYCOLPL
    // asm 0000234C: 	CALL	OVELNADD		;UPDATE VELOCITIES
    // asm 0000234D: 	LDI	0,R0
    // asm 0000234E: 	STI	R0,*+AR4(OUSR1)		;INDICATE IN MOTION, RE-SORT
    // asm 0000234F: 	FLOATP	@NFRAMES,R2
    // asm 00002350: 	MPYF	2,R2			;FRAME ADJUSTED GRAVITY
    // asm 00002351: 	ADDF	*+AR4(OVELY),R2
    // asm 00002352: 	STF	R2,*+AR4(OVELY)
    // asm 00002353: 	CMPF	100,R2
    // asm 00002354: 	BGT	FLYSTOP
    // asm 00002355: 	CALL	OBJSCAN
    // asm 00002356: 	BNC	FLYCSLP			;OFF THE MAP
    // *WERE OVER THE ROAD
    // asm 00002357: FLYROAD
    // asm 00002357: 	FLOAT	155,R1 			;HT OF DRUM/SAWHORSE
    // asm 00002358: 	LDI	*+AR4(OID),R2		;ROADKILL DOESN'T BOUNCE
    // asm 00002359: 	CMPI	RDDEBRIS_C|TSC_ROADKILL,R2
    // asm 0000235A: 	LDFEQ	35,R1
    // asm 0000235B:  	CMPF	R1,R0
    // asm 0000235C: 	BGT	FLYCSLP			;WERE ABOVE GROUND
    // *WE HIT THE GROUND DUDES
    // asm 0000235D: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // asm 0000235E: 	BN	FLYCSLP			;WERE GOING UP IGNORE IT
    // asm 0000235F: 	SUBRF	R1,R0			;SET HIM ON THE GROUND
    // asm 00002360: 	ADDF	*+AR4(OPOSY),R0
    // asm 00002361: 	CMPF	20,R2          		;CHECK FOR MINIMUM
    // asm 00002362: 	BLT	FLYSTOP			;TIME TO STOP
    // asm 00002363: 	MPYF	-0.5,R2
    // asm 00002364: 	STF	R2,*+AR4(OVELY)
    // asm 00002365: 	LDF	*+AR4(OVELX),R0		;CUT DOWN VELOCITIES
    // asm 00002366: 	MPYF	0.5,R0
    // asm 00002367: 	STF	R0,*+AR4(OVELX)
    // asm 00002368: 	LDF	*+AR4(OVELZ),R1		;CUT DOWN VELOCITIES
    // asm 00002369: 	MPYF	0.5,R1
    // asm 0000236A: 	STF	R1,*+AR4(OVELZ)
    // *MAKE BOUNCE SOUND
    // asm 0000236B: 	FLOAT	*+AR4(ODIST),R0
    // asm 0000236C: 	BN	FLYCSLP			;BEHIND PLAYER NO SOUND
    // asm 0000236D: 	CALL	INV_F30
    // asm 0000236E: 	FLOAT	5000,R1
    // asm 0000236F: 	MPYF	R1,R0
    // asm 00002370: 	ABSF	*+AR4(OVELY),R2	       	;SOUND PROPORTIONAL TO VERT VELOCITY
    // asm 00002371: 	CMPF	30,R2
    // asm 00002372: 	LDFGT	30,R2
    // asm 00002373: 	MPYF	5.0,R2
    // asm 00002374: 	MPYF	R2,R0
    // asm 00002375: 	FIX	R0
    // asm 00002376: 	CMPI	140,R0
    // asm 00002377: 	LDIGT	140,R0
    // asm 00002378: 	LDI	*+AR4(OID),R1
    // asm 00002379: 	AND	SUBTYPE_M,R1
    // asm 0000237A: 	CMPI	RDD_55GAL,R1
    // asm 0000237B: 	LDIEQ	DRMBNCE,AR2
    // asm 0000237C: 	LDINE	SAWBNCE,AR2
    // asm 0000237D: 	CALL	VOLSNDFX
FLYCSLP:
    // asm 0000237E: 	SLEEP	1
    // asm 00002380: 	B	FLYCOLP0
FLYSTOP:
    // asm 00002381: 	LDI	1,R0
    // asm 00002382: 	LSH	O_PROC_B,R0		;CLEAR PROCESS BIT
    // asm 00002383: 	NOT	R0
    // asm 00002384: 	AND	*+AR4(OFLAGS),R0
    // asm 00002385: 	STI	R0,*+AR4(OFLAGS)
    // asm 00002386: 	LDI	*+AR4(OID),R0
    // asm 00002387: 	CMPI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm 00002388: 	BNE	NOT_ROADKILL
    // asm 00002389: 	LDI	AR4,AR2
    // asm 0000238A: 	CALL	OBJ_DELETE
NOT_ROADKILL:
    // asm 0000238B: 	BR 	SUICIDE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYCOLLP", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *
 *KILL OFFSCREEN ROAD DEBRIS
 *
 */
static void DEBSCAN(void) {
    // asm 0000238C: 	LDI	@ROAD_DEBRIS,R0
    // asm 0000238D: 	B	DEBSCL1
DEBSCL0:
    // asm 0000238E: 	LDI	*+AR2(OLINK2),R0 	;IN BACKGROUND GROUP?
    // asm 0000238F: 	BNZ	DEBSCL			;YES, SKIP IT...
    // asm 00002390: 	LDI	*+AR2(OFLAGS),R0
    // asm 00002391: 	AND	O_LIST_M,R0
    // asm 00002392: 	CMPI	O_LIST2,R0		;OBJECT ACTIVE?
    // asm 00002393: 	BNZ	DEBSCL			;YES, FAGIT ABOUDIT
    // asm 00002394: 	LDI	*+AR2(OLINK3),R0
    // asm 00002395: 	CALL	OBJ_DELETE			;OBJECT INACTIVE, CAN IT + PROCESS
    // asm 00002396: 	B	DEBSCL1
DEBSCL:
    // asm 00002397: 	LDI	*+AR2(OLINK3),R0
DEBSCL1:
    // asm 00002398: 	LDI	R0,AR2
    // asm 00002399: 	BNZ	DEBSCL0
    // asm 0000239A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEBSCAN", 0, 0);
    UNIMPL_TODO();
}

/*
 *----------------------------------------------------------------------------
 *FALLING SIGN PROCESS
 *
 *PARAMETERS
 *	AR4	SIGN OBJECT
 *	R7	ROTATION DELTA
 *
 */

static void SIGNFALL(void) {
    // asm 0000239C: 	LDF	0,R6
SIGNFALP:
    // asm 0000239D: 	ADDF	R7,R6 			;ACCUMULATE RADIANS
    // asm 0000239E: 	CMPF	1.5,R6			;CHECK DONE
    // asm 0000239F: 	BLT	SIGNFALP0		;NOPE...
    // asm 000023A0: 	SUBF	1.5,R6			;SUBTRACT OUT EXCESS
    // asm 000023A1: 	SUBF	R6,R7
    // asm 000023A2: 	LDF	1.6,R6			;SIGNAL WERE DONE
SIGNFALP0:
    // asm 000023A3: 	LDF	R7,R2
    // asm 000023A4: 	LDPI	@MATRIXAI,AR2  		;GET TEMP STORE
    // asm 000023A5: 	CALL    FIND_XMATRIX		;NEW MATRIX
    // asm 000023A6: 	LDI	AR4,R2
    // asm 000023A7: 	ADDI	OMATRIX,R2
    // asm 000023A8: 	LDI	R2,R3
    // asm 000023A9: 	CALL	CONCATMAT
    // asm 000023AA: 	SLEEP	1
    // asm 000023AC: 	CMPF	1.5,R6
    // asm 000023AD: 	BLT	SIGNFALP   		;LOOP TIL DONE
    // asm 000023AE: 	BR	SUICIDE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SIGNFALL", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *TREE SHAKE PROCESS
 *
 *PARAMETERS
 *	AR4	SIGN OBJECT
 *	R7	ROTATION DELTA
 *
 */

static void TREESHAK(void) {
    // *SHAKE IT FORWARD
    // asm 000023B0: 	LDF	R7,R2
    // asm 000023B1: 	LDPI	@MATRIXAI,AR2  		;GET TEMP STORE
    // asm 000023B2: 	CALL    FIND_XMATRIX		;NEW MATRIX
    // asm 000023B3: 	LDI	AR4,R2
    // asm 000023B4: 	ADDI	OMATRIX,R2
    // asm 000023B5: 	LDI	R2,R3
    // asm 000023B6: 	CALL	CONCATMAT
    // asm 000023B7: 	SLEEP	1
    // *SHAKE IT BACK
    // asm 000023B9: 	LDI	3,AR6			;# FRAMES/SHAKE
    // asm 000023BA: 	MPYF	-0.40,R7     		;DAMP IT
TREESHKL:
    // asm 000023BB: 	LDF	R7,R2
    // ;	LDP	@MATRIXAI
    // asm 000023BC: 	LDPI	@MATRIXAI,AR2  		;GET TEMP STORE
    // asm 000023BD: 	CALL    FIND_XMATRIX		;NEW MATRIX
    // asm 000023BE: 	LDI	AR4,R2
    // asm 000023BF: 	ADDI	OMATRIX,R2
    // asm 000023C0: 	LDI	R2,R3
    // asm 000023C1: 	CALL	CONCATMAT
    // asm 000023C2: 	SLEEP	1
    // asm 000023C4: 	DBU	AR6,TREESHKL
    // asm 000023C5: TREESHKL1
    // asm 000023C5: 	LDI	3,AR6			;# FRAMES/SHAKE
    // asm 000023C6: 	MPYF	-0.6,R7     		;REVERSE IT
    // asm 000023C7: 	ABSF	R7,R0
    // asm 000023C8: 	CMPF	0.01,R0
    // asm 000023C9: 	BGT	TREESHKL
    // asm 000023CA: 	LDI	*+AR4(OFLAGS),R0	;MAKE IT A POSTER AGAIN
    // asm 000023CB: 	OR	O_POSTER,R0
    // asm 000023CC: 	LDI	1,R1			;CLR 3D ROTATION BIT
    // asm 000023CD: 	LS	O_3DROT_B,R1
    // asm 000023CE: 	ANDN	R1,R0
    // asm 000023CF: 	STI	R0,*+AR4(OFLAGS)
    // asm 000023D0: 	BR	SUICIDE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TREESHAK", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *FREESIGN
 *unlink SIGN from SIGN supplementary list
 *PARAMETERS
 *	AR4	OBJECT SIGN POINTER
 *	R0 IS TRASHED
 */
static void FREESIGN(void) {
    // asm 000023D1: 	PUSH	AR1
    // ;	LDP	@SIGN_LISTI
    // asm 000023D2: 	LDPI	@SIGN_LISTI,R0
    // asm 000023D3: 	SUBI	OLINK3,R0		;(we are offset pointing)
SFREELP:
    // asm 000023D4: LDI	R0,AR1
    // asm 000023D5: 	LDI	*+AR1(OLINK3),R0
#if DEBUG
    // asm: 	BZ	$			;lockup on end of list found
#else
    // asm 000023D6: 	RETSZ
#endif
    // asm 000023D7: 	CMPI	R0,AR4
    // asm 000023D8: 	BNE	SFREELP
    // asm 000023D9: 	LDI	*+AR4(OLINK3),R0
    // asm 000023DA: 	STI	R0,*+AR1(OLINK3)	;LINK AROUND
    // asm 000023DB: 	LDI	1,R1
    // asm 000023DC: 	LS	O_SIGN_SUPP_B,R1
    // asm 000023DD: 	LDI	*+AR4(OFLAGS),R0
    // asm 000023DE: 	ANDN	R1,R0     		;TURN OFF SUPP LIST FLAGS
    // asm 000023DF: 	LDI	1,R1
    // asm 000023E0: 	LS	O_3DROT_B,R1
    // asm 000023E1: 	OR	R1,R0
    // asm 000023E2: 	STI	R0,*+AR4(OFLAGS)
    // asm 000023E3: 	POP	AR1
    // asm 000023E4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREESIGN", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *ADDSIGN
 *LINK SIGN TO SIGN sup list
 *PARAMETERS
 *	AR4	OBJECT SIGN POINTER
 *	R0 IS TRASHED
 */
static void ADDSIGN(void) {
    // asm 000023E5: 	LDPI	@SIGN_LIST,R0
    // asm 000023E6: 	STI	AR4,@SIGN_LIST
    // asm 000023E7: 	STI	R0,*+AR4(OLINK3)
    // asm 000023E8: 	LDI	1,R1
    // asm 000023E9: 	LS	O_SIGN_SUPP_B,R1
    // asm 000023EA: 	OR	*+AR4(OFLAGS),R0
    // asm 000023EB: 	STI	R0,*+AR4(OFLAGS)
    // asm 000023EC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADDSIGN", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *
 *FLYING CAR WRECK
 *
 *	AR0	POINTS TO PLAYER CAR
 *	AR1	POINTS TO CAR TO SEND FLYING
 *	AR4	POINTS TO PLAYER CAR BLOCK
 *	AR5	POINTS TO DRONE CAR BLOCK
 *
 */
/* asm: SBUSI	.word	sbus */
#define SBUSI sbus_ROM
/* asm: CBUSI	.word	cbus */
/* asm: 	 */
#define CBUSI cbus_ROM

static void FLYCAR(void) {
    // asm 000023EF: 	PUSH	AR0
    // asm 000023F0: 	PUSH	AR1
    // asm 000023F1: 	PUSH	AR3
    // asm 000023F2: 	PUSH	AR4
    // asm 000023F3: 	PUSH	AR5
    // asm 000023F4: 	LDI	*+AR0(OCARBLK),AR3	;GET PLAYER'S CAR
    // asm 000023F5: 	LDI	AR1,AR4			;GET DRONE CAR OBJECT POINTER
    // asm 000023F6: 	LDI	*+AR4(OCARBLK),AR5	;GET DRONE CAR BLOCK
    // asm 000023F7: 	LDI	@HEAD2HEAD_ON,R0
    // asm 000023F8: 	BZ	NOTLINKED
    // asm 000023F9: 	LDI	*+AR5(CAR_OM),R0    	;OTHER MACHINES CAR?
    // asm 000023FA: 	BNE	L78G			;YES SKIP FLYING STUFF
NOTLINKED:
    // asm 000023FB: 	LDI	1,R0
    // asm 000023FC: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm 000023FD: 	TSTB	*+AR1(OFLAGS),R0	;PROCESS ALREADY ACTIVE	?
    // asm 000023FE: 	BZ	FLYCAR0			;NO
    // asm 000023FF: 	LDI	*+AR1(OPLINK),R0	;YES, KILL OFF DRONE PROCESS
    // asm 00002400: 	BZ	FLYCAR0
    // asm 00002401: 	LDI	R0,AR2
    // asm 00002402: 	CALL	PRC_KILL
    // asm 00002403: 	LDI	0,R0
    // asm 00002404: 	STI	R0,*+AR1(OPLINK)
FLYCAR0:
    // asm 00002405: 	LDI	*+AR1(OFLAGS),R0
    // asm 00002406: 	TSTB	O_DYNAMIC,R0
    // asm 00002407: 	BZ	FLYCAR1
    // asm 00002408: 	LDI	*+AR1(ORADZ),R0   	;KILL WHEEL SPINNER, LEANER PROCESS
    // asm 00002409: 	BZ	FLYCAR1
    // asm 0000240A: 	LDI	R0,AR2
    // asm 0000240B: 	CALL	PRC_KILL
    // asm 0000240C: 	LDI	0,R0
    // asm 0000240D: 	STI	R0,*+AR1(ORADZ)
FLYCAR1:
    // asm 0000240E: 	LDI	0,R0
    // asm 0000240F: 	STI	R0,*+AR5(CARSHAD)	;TURN OFF SHADOW
    // asm 00002410: 	LDF	0.10,R0 		;ADD RANDOM ROTATION
    // asm 00002411: 	CALL	SFRAND
    // asm 00002412: 	ADDF	*+AR3(CARVROT),R0
    // asm 00002413: 	STF	R0,*+AR5(CARVROT)
    // asm 00002414: 	LDF	0.45,R0			;RANDOM SPEED MULTIPLIER
    // asm 00002415: 	CALL	FRAND
    // asm 00002416: 	ADDF	0.8,R0
    // asm 00002417: 	MPYF	0.75,R0
    // asm 00002418: 	LDF	*+AR5(CARMASS),R1	;DECREASE THROW BY MASS
    // asm 00002419: 	CALL	DIV_F
    // asm 0000241A: 	MPYF	*+AR3(CARSPEED),R0	;COMPUTE DRONE SPEED FROM PLAYER SPD
    // asm 0000241B: 	STF	R0,*+AR5(CARSPEED)	;STORE NEW DRONE SPEED
    // asm 0000241C: 	LDF	-0.3,R0
    // asm 0000241D: 	CALL	FRAND
    // asm 0000241E: 	ADDF	-0.2,R0
    // asm 0000241F: 	MPYF	1.5,R0			;SPEEDFUDGE FACTOR
    // asm 00002420: 	MPYF	*+AR3(CARSPEED),R0	;GET CURRENT SPEED
    // asm 00002421: 	CMPF	-65,R0
    // asm 00002422: 	LDFLT	-65,R0		  	;MAX VERTICAL VELOCITY
    // asm 00002423: 	STF	R0,*+AR1(OVELY)		;STUFF VERTICAL VELOCITY
    // asm 00002424: 	LDPI	@FLYCARPI,AR2		;GET SIGN FLY PROCESS
    // asm 00002425: 	LDI	DRONE_C|FLYER_T,R2
    // asm 00002426: 	CALL	PRC_CREATE_CHILD		;CREATE A CHILD PROCESS
    // asm 00002427: 	BC	L78G
    // asm 00002428: 	STI	AR0,*+AR4(OPLINK)	;SAVE LINK
    // asm 00002429: 	LDI	1,R0
    // asm 0000242A: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm 0000242B: 	OR	*+AR4(OFLAGS),R0	;SET ATTACHED PROCESS FLAG
    // asm 0000242C: 	STI	R0,*+AR4(OFLAGS)
L78G:
    // asm 0000242D: LDF	*+AR3(CARSPEED),R0  	;CUT SPEED OF PLAYER
    // asm 0000242E: 	MPYF	0.5,R0
    // asm 0000242F: 	MPYF	@CHEAT,R0		;BOOST SPEED ON CHEAT
    // asm 00002430: 	MPYF	@CHEAT,R0		;BOOST SPEED ON CHEAT
    // asm 00002431: 	STF	R0,*+AR3(CARSPEED)
    // asm 00002432: 	MPYF	1.25,R0
    // asm 00002433: 	LDF	*+AR5(CARSPEED),R1	;KEEP FLYING CAR OUT FRONT
    // asm 00002434: 	CMPF	R0,R1
    // asm 00002435: 	LDFLT	R0,R1
    // asm 00002436: 	STF	R1,*+AR5(CARSPEED)
    // *GET PLAYER CAR VELOCITY, SPIN
    // asm 00002437: 	LDI	@WRECKFLG,R0		;WRECK ON?
    // asm 00002438: 	BNE	FLY0			;YES, DONT START A NEW ONE
    // asm 00002439: 	LDI	400,AR2	    		;TOTAL WRECK PLAYER
    // asm 0000243A: 	CALL	RANDPER			;NO
    // asm 0000243B: 	BNC	FLY0
    // asm 0000243C: 	CALL	WRECKST			;START YOUR WRECK
    // asm 0000243D: 	LDI	@DETHTAB2I,AR2
    // asm 0000243E: 	LDI	4,R0
    // asm 0000243F: 	B	FLYCARXX
FLY0:
    // asm 00002440: 	LDPI	@CAMVIEW,R0
    // asm 00002441: 	BZ	FLY1
    // asm 00002442: 	LDI	750,AR2
    // asm 00002443: 	CALL	RANDPER
    // asm 00002444: 	BC	FLY3	    		;SPIN SOMETIMES IN 3RD PERSON
    // *FIRST PERSON
FLY1:
    // asm 00002445: 	LDF	*+AR3(CARVROT),R0      	;REVERSE VELOCITY
    // asm 00002446: 	ADDF	3.14,R0
    // asm 00002447: 	STF	R0,*+AR3(CARVROT)
    // asm 00002448: 	LDF	0,R1
    // asm 00002449: 	LDI	15,R0			;REVERSE FOR 15 COUNT
    // asm 0000244A: 	B	FLYCARX
FLY3:
    // asm 0000244B: 	LDF	3.14,R0			;SPIN HIM AROUND
    // asm 0000244C: 	STF	R0,*+AR3(CARSPRAD)
    // asm 0000244D: 	LDF	0.04,R0
    // asm 0000244E: 	CALL	SFRAND
    // asm 0000244F: 	LDF	R0,R0
    // asm 00002450: 	LDFGT	0.08,R1
    // asm 00002451: 	LDFLE	-0.08,R1
    // asm 00002452: 	ADDF	R0,R1
    // asm 00002453: 	LDI	1,R0
FLYCARX:
    // asm 00002454: 	STF	R1,*+AR3(CARDROT)
    // asm 00002455: 	STI	R0,*+AR3(CAR_SPIN)
    // asm 00002456: 	LDI	@DETHTAB1I,AR2
    // asm 00002457: 	LDI	8,R0
FLYCARXX:
    // asm 00002458: 	LDI	*+AR1(OROMDATA),R1  	;CHECK FOR A BUS...
    // asm 00002459: 	CMPI	@SBUSI,R1
    // asm 0000245A: 	BNZ	FC00
    // asm 0000245B: 	LDI	KIDSCREAM2,AR2
    // asm 0000245C: 	B	FC01
FC00:
    // asm 0000245D: 	CMPI	@CBUSI,R1
    // asm 0000245E: 	BNZ	FC02
    // asm 0000245F: 	LDI	ROAR,AR2
FC01:
    // asm 00002460: 	CALL	ONESNDFX
    // asm 00002461: 	B	FC03
FC02:
    // asm 00002462: 	CALL	RANDSND
FC03:
    // asm 00002463: 	POP	AR5
    // asm 00002464: 	POP	AR4
    // asm 00002465: 	POP	AR3
    // asm 00002466: 	POP	AR1
    // asm 00002467: 	POP	AR0
    // asm 00002468: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYCAR", 0, 0);
    UNIMPL();
}

/* asm: DETHTAB1	.WORD	MDETHSCREAM2,MDETHSCREAM4,EXP1,EXP3 */
/* asm: 	.WORD	NDETHSCREAM1,NDETHSCREAM3,NDETHSCREAM4,NDETHSCREAM7 */
/* asm: 	 */
static int DETHTAB1[] = {
    MDETHSCREAM2,
    MDETHSCREAM4,
    EXP1,
    EXP3,
    NDETHSCREAM1,
    NDETHSCREAM3,
    NDETHSCREAM4,
    NDETHSCREAM7,
};
/* asm: DETHTAB2	.WORD	MFDETHSCREAM1,MFDETHSCREAM2,BCHEER,EXP2 */
/* asm: 	 */
static int DETHTAB2[] = {
    MFDETHSCREAM1,
    MFDETHSCREAM2,
    BCHEER,
    EXP2,
};

/*
 *
 *FLYING CAR PROCESS
 *AR4=DRONE PROCESS
 *AR5=DRONE CAR BLOCK
 *PDATA=   X RAD
 *PDATA+1= Y RAD
 *PDATA+2= Z RAD
 *PDATA+3= X RAD TOTAL
 *PDATA+4= Y RAD	TOTAL
 *PDATA+5= Z RAD	TOTAL
 *PDATA+6=MATRIX
 *
 */
void FLYCARP(void) {
    // *GET YOUR RADIANS
    // asm 00002478: 	LDF	0.2,R0
    // asm 00002479: 	CALL	SFRAND
    // asm 0000247A: 	STF	R0,*+AR7(PDATA)	  	;X RADIANS
    // asm 0000247B: 	LDF	0.1,R0
    // asm 0000247C: 	CALL	SFRAND
    // asm 0000247D: 	STF	R0,*+AR7(PDATA+1)	;Y RADIANS
    // asm 0000247E: 	LDF	0,R0
    // asm 0000247F: 	STF	R0,*+AR7(PDATA+2)	;Z RADIANS
    // asm 00002480: 	LDF	0,R0
    // asm 00002481: 	STF	R0,*+AR7(PDATA+3)	;X RADIAN TOTAL
    // asm 00002482: 	STF	R0,*+AR7(PDATA+5)  	;Z RADIAN TOTAL
    // asm 00002483: 	LDF	*+AR5(CARYROT),R0	;GET CAR Y ROT
    // asm 00002484: 	STF	R0,*+AR7(PDATA+4)
FLYCARP0:
    // asm 00002485: 	LDI	@SUSPEND_MODE,R0       	;WAIT IN SUSPEND MODE
    // asm 00002486: 	CMPI	SM_HALT,R0
    // asm 00002487: 	BZ	FLYCARSLP
    // asm 00002488: 	LDI	0,R4
    // asm 00002489: 	LDI	*+AR4(ODIST),R0		;OUT OF RANGE ??
    // asm 0000248A: 	CMPI	-6000,R0
    // asm 0000248B: 	BLT	FLYCARPXX   		;END THIS FARCE
    // asm 0000248C: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm 0000248D: 	CALL	ROADSCAN
    // asm 0000248E: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR DATA AREA
    // asm 0000248F: 	FLOATP	@NFRAMES,R1	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm 00002490: 	LDF	*+AR7(PDATA),R0		;ACCUMULATE X RADIANS
    // asm 00002491: 	MPYF	R1,R0
    // asm 00002492: 	ADDF	*+AR7(PDATA+3),R0
    // asm 00002493: 	STF	R0,*+AR7(PDATA+3)
    // asm 00002494: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // asm 00002495: 	MPYF	R1,R0
    // asm 00002496: 	ADDF	*+AR7(PDATA+4),R0
    // asm 00002497: 	STF	R0,*+AR7(PDATA+4)
    // ;	LDF	*+AR7(PDATA+2),R0 	;ACCUMULATE Z RADIANS
    // ;	MPYF	R1,R0
    // ;	ADDF	*+AR7(PDATA+5),R0
    // ;	STF	R0,*+AR7(PDATA+5)
    // asm 00002498: 	CALL	GETFLYMAT		;COMPUTE MATRICES
    // *CONVERT CARVROT,CARSPEED TO OVELX, OVELZ
    // asm 00002499: 	LDF	*+AR5(CARVROT),R2
    // asm 0000249A: 	ADDF	1.57,R2		   	;CORRECT FOR 90 DEGREE ERROR
    // asm 0000249B: 	CALL	_SINE
    // asm 0000249C: 	LDF	*+AR5(CARSPEED),R3
    // asm 0000249D: 	MPYF	R3,R0
    // asm 0000249E: 	STF	R0,*+AR4(OVELZ)	  	;CONVERT TO CARVROT, CARSPEED TO XZVEL
    // asm 0000249F: 	CALL	_COSI
    // asm 000024A0: 	MPYF	R3,R0
    // asm 000024A1: 	STF	R0,*+AR4(OVELX)
    // asm 000024A2: 	CALL	OVELNADD		;UPDATE VELOCITIES
    // asm 000024A3: 	FLOATP	@NFRAMES,R2
    // asm 000024A4: 	MPYF	2,R2			;FRAME ADJUSTED GRAVITY
    // asm 000024A5: 	ADDF	*+AR4(OVELY),R2
    // asm 000024A6: 	STF	R2,*+AR4(OVELY)
    // asm 000024A7: 	FLOAT	300,R1		  	;GOING DOWN TOO MUCH?
    // asm 000024A8: 	CMPF	R1,R2
    // asm 000024A9:  	BGT	FLYCARPXXX		;ABORT THE DUDE...
    // asm 000024AA: 	CALL	GETTRAK
    // asm 000024AB: 	LDPI	@_MODE,R0
    // asm 000024AC: 	TSTB	MBRIDGE,R0		;ON BRIDGE?
    // asm 000024AD: 	CALLZ	DRONINBZ		;CHECK BOUNDS IF NO BRIDGE
    // asm 000024AE: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // asm 000024AF: 	BN	FLYCARSLP		;WERE GOING UP IGNORE IT
    // asm 000024B0: 	CALL	BOXSCAN		      	;KEEP FALLING!!!
    // asm 000024B1: 	BNC	FLYCARSLP
    // *WERE OVER THE ROAD
    // asm 000024B2: FLYCROAD
    // asm 000024B2: 	LDF	R0,R0
    // asm 000024B3: 	BGT	FLYCARSLP		;WERE ABOVE GROUND
    // *WE HIT THE GROUND DUDES
    // asm 000024B4: 	LDF	R0,R2			;Save for offseting sparks
    // asm 000024B5: 	ADDF	30,R2			;IMPACT_SPARKS uses this
    // asm 000024B6: 	CALL	ROAD_IMPACT_SPARK
    // asm 000024B7: 	ADDF	*+AR4(OPOSY),R0		;SET HIM ON THE GROUND
    // asm 000024B8: 	STF	R0,*+AR4(OPOSY)
    // asm 000024B9: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // ;	CMPF	20,R2          		;CHECK FOR MINIMUM
    // asm 000024BA: 	CMPF	35,R2          		;CHECK FOR MINIMUM
    // asm 000024BB: 	BLT	FLYCARSTOP0    		;TIME TO STOP
    // asm 000024BC: FLYCROAD1
    // asm 000024BC: 	MPYF	-0.5,R2
    // asm 000024BD: 	STF	R2,*+AR4(OVELY)
    // asm 000024BE: 	LDF     *+AR5(CARSPEED),R0	;CUT SPEED IN HALF
    // asm 000024BF: 	MPYF	0.5,R0
    // asm 000024C0: 	STF	R0,*+AR5(CARSPEED)
    // ;	LDF	*+AR4(OVELX),R0		;CUT DOWN VELOCITIES
    // ;	MPYF	0.5,R0
    // ;	STF	R0,*+AR4(OVELX)
    // ;	LDF	*+AR4(OVELZ),R1		;CUT DOWN VELOCITIES
    // ;	MPYF	0.5,R1
    // ;	STF	R1,*+AR4(OVELZ)
    // asm 000024C1: 	LDF	*+AR7(PDATA),R0		;CUT DOWN SPIN
    // asm 000024C2: 	MPYF	0.5,R0
    // asm 000024C3: 	STF	R0,*+AR7(PDATA)
    // asm 000024C4: 	LDF	*+AR7(PDATA+1),R0
    // asm 000024C5: 	MPYF	0.5,R0
    // asm 000024C6: 	STF	R0,*+AR7(PDATA+1)
    // ;	LDF	*+AR7(PDATA+2),R0
    // ;	MPYF	0.5,R0
    // ;	STF	R0,*+AR7(PDATA+2)
    // *MAKE BOUNCE SOUND
    // asm 000024C7: 	LDPI	@SCOLLTABI,AR2	   	;RANDOM COLLISION CRUNCH
    // asm 000024C8: 	LDI	3,R0
    // asm 000024C9: 	CALL	DRONESND
FLYCARSLP:
    // asm 000024CA: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 000024CB: 	CALLNZ	SEND_FLY_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm 000024CC: 	SLEEP	1
    // asm 000024CE: 	B	FLYCARP0
    // *ROTATE TO QUIESCENT STATE
FLYCARSTOP0:
    // ;	CALL	GETCARVSPD		;CONVERT XVEL,ZVEL TO CARSPEED, CARVROT
FLYCARSTOP:
    // asm 000024CF: 	LDI	@SUSPEND_MODE,R0       	;WAIT IN SUSPEND MODE
    // asm 000024D0: 	CMPI	SM_HALT,R0
    // asm 000024D1: 	BZ	FLYSTOPSLP
    // asm 000024D2: 	LDI	1,R4
    // asm 000024D3: 	LDI	*+AR4(ODIST),R0		;OUT OF RANGE ??
    // asm 000024D4: 	CMPI	-6000,R0
    // asm 000024D5: 	BLT	FLYCARPXX   		;END THIS FARCE
    // asm 000024D6: 	FLOAT	500,R0
    // asm 000024D7: 	STF	R0,*+AR4(OVELY)		;FORCE ONTO GROUND
    // asm 000024D8: 	LDF	*+AR5(CARVROT),R2
    // asm 000024D9: 	ADDF	1.57,R2		   	;CORRECT FOR 90 DEGREE ERROR
    // asm 000024DA: 	CALL	_SINE
    // asm 000024DB: 	LDF	*+AR5(CARSPEED),R3
    // asm 000024DC: 	MPYF	R3,R0
    // asm 000024DD: 	STF	R0,*+AR4(OVELZ)	  	;CONVERT TO CARVROT, CARSPEED TO XZVEL
    // asm 000024DE: 	CALL	_COSI
    // asm 000024DF: 	MPYF	R3,R0
    // asm 000024E0: 	STF	R0,*+AR4(OVELX)
    // asm 000024E1: 	LDF	*+AR5(CARDROT),R0
    // asm 000024E2: 	STF	R0,*+AR7(PDATA+1)	;GET Y SPIN
    // asm 000024E3: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm 000024E4: 	CALL	ROADSCAN
    // asm 000024E5: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR DATA AREA
    // asm 000024E6: 	LDPI	@NFRAMES,RC	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm 000024E7: 	SUBI	1,RC
    // asm 000024E8: 	RPTB	FLYCSTL
    // asm 000024E9: 	LDF	*+AR4(OVELX),R0
    // asm 000024EA: 	MPYF	0.98,R0			;DECAY VELOCITY
    // asm 000024EB: 	STF	R0,*+AR4(OVELX)
    // asm 000024EC: 	ADDF	*+AR4(OPOSX),R0
    // asm 000024ED: 	STF	R0,*+AR4(OPOSX)
    // asm 000024EE: 	LDF	*+AR4(OVELY),R0
    // asm 000024EF: 	ADDF	*+AR4(OPOSY),R0
    // asm 000024F0: 	STF	R0,*+AR4(OPOSY)
    // asm 000024F1: 	LDF	*+AR4(OVELZ),R0
    // asm 000024F2: 	MPYF	0.98,R0			;DECAY VELOCITY
    // asm 000024F3: 	STF	R0,*+AR4(OVELZ)
    // asm 000024F4: 	ADDF	*+AR4(OPOSZ),R0
    // asm 000024F5: 	STF	R0,*+AR4(OPOSZ)
    // asm 000024F6: 	LDF	*+AR7(PDATA),R0		;ACCUMULATE X RADIANS
    // ;	MPYF	0.97,R0  		;DAMP IT
    // asm 000024F7: 	MPYF	0.96,R0  		;DAMP IT
    // asm 000024F8: 	STF	R0,*+AR7(PDATA)		;CUT DOWN ROCK
    // asm 000024F9: 	ADDF	*+AR7(PDATA+3),R0
    // asm 000024FA: 	STF	R0,*+AR7(PDATA+3)
    // asm 000024FB: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // ;	MPYF	0.985,R0  		;DAMP IT
    // asm 000024FC: 	MPYF	0.98,R0  		;DAMP IT
    // asm 000024FD: 	STF	R0,*+AR7(PDATA+1)	;CUT DOWN ROCK
    // asm 000024FE: 	STF	R0,*+AR5(CARDROT)	;STORE IN CAR STRUCTURE
    // asm 000024FF: 	ADDF	*+AR7(PDATA+4),R0
    // asm 00002500: 	STF	R0,*+AR7(PDATA+4)
    // ;	LDF	*+AR7(PDATA+2),R0 	;ACCUMULATE Z RADIANS
    // ;	ADDF	*+AR7(PDATA+5),R0
    // ;	STF	R0,*+AR7(PDATA+5)
    // asm 00002501: 	LDF	*+AR7(PDATA+3),R2	;CHECK TOTAL X RADIANS
    // asm 00002502: 	CALL	NORMITS
    // asm 00002503: 	ABSF	R2,R3
    // asm 00002504: 	LDF	*+AR5(CARSPEED),R0	;DECAY SPEED
    // asm 00002505: 	MPYF	0.98,R0
    // asm 00002506: 	STF	R0,*+AR5(CARSPEED)
    // *CHECK FOR DONE...
    // asm 00002507: 	CMPF	10,R0	       		;SPEED DECAYED?
    // asm 00002508: 	BGT	FLYCSTP0	     	;NO, KEEP GOING...
    // asm 00002509: 	ABSF	*+AR5(CARDROT),R0	;GET ROTATE
    // asm 0000250A: 	ABSF	*+AR7(PDATA),R1	    	;ADD IN ROCK
    // asm 0000250B: 	ADDF	R0,R1
    // asm 0000250C: 	MPYF	10,R1	    		;GET IN RANGE
    // asm 0000250D: 	CMPF	0.02,R1			;PETERED OUT?
    // asm 0000250E: 	BGT	FLYCSTP0		;NO, KEEP GOING
    // asm 0000250F: 	CMPF	0.2,R3			;RIGHT SIDE UP?
    // asm 00002510: 	BGT	FLYCSTP00		;NOPE...
    // asm 00002511: 	LDF	0,R3			;STRAIGHTEN HIM UP!!!
    // asm 00002512: 	STF	R3,*+AR7(PDATA+3)
    // asm 00002513: 	LDI	2,R0
    // asm 00002514: 	B	FLYCCC			;YES, TIME TO STOP
FLYCSTP00:
    // asm 00002515: 	CMPF	2.95,R3			;UPSIDE DOWN?
    // asm 00002516: 	BLT	FLYCSTP0		;NOPE
    // asm 00002517: FLYCSTP
    // asm 00002517: 	LDI	1,R0	 		;WERE DONE DUDES...
    // asm 00002518: 	B	FLYCCC
    // *ACCELERATE X ROTATION
FLYCSTP0:
    // asm 00002519: 	LDF	R2,R2
    // asm 0000251A: 	BN	FLYCSTP1
    // asm 0000251B: 	CMPF	1.57,R2
    // asm 0000251C: 	B 	FLYCSTP2
FLYCSTP1:
    // asm 0000251D: 	CMPF	-1.57,R2
FLYCSTP2:
    // asm 0000251E: 	LDFLT	-0.01,R0
    // asm 0000251F: 	LDFGE	0.01,R0
    // asm 00002520: 	MPYF	0.4,R0
    // asm 00002521: 	ADDF	*+AR7(PDATA),R0
    // asm 00002522: 	LDF	*+AR7(PDATA),R1
    // asm 00002523: 	XOR	R0,R1,R2 		;CHECK FOR SIGN CHANGE- MAKE SOUND
    // asm 00002524: 	BNN	FLYCSTL			;NO SOUND
    // asm 00002525: 	ABSF	*+AR7(PDATA+3),R2	;CHECK IF AMPLITUDE BIG ENOUGH
    // asm 00002526: 	CALL	NORMITS
    // asm 00002527: 	ABSF	R2
    // asm 00002528: 	CMPF	0.08,R2
    // asm 00002529: 	BLT	FLYCSTL			;TOO SMALL OF A ROCK
    // asm 0000252A: 	CMPF	3.06,R2
    // asm 0000252B: 	BGT	FLYCSTL			;TOO SMALL OF A UPSIDE DOWN ROCK
    // asm 0000252C: 	LDI	BOTTOMOUT,AR2		;MAKE BOTTOMOUT SOUND
    // asm 0000252D: 	PUSHF	R0
    // asm 0000252E: 	CALL	DRONESND1
    // asm 0000252F: 	POPF	R0
FLYCSTL:
    // asm 00002530: STF	R0,*+AR7(PDATA)		;ACCELERATE ROTATION
    // asm 00002531: 	LDI	0,R0			;DONE FLAG
FLYCCC:
    // asm 00002532: 	PUSH	R0
    // asm 00002533: 	CALL	GETTRAK
    // asm 00002534: 	CALL	DRONINBZ		;CHECK BOUNDS
    // asm 00002535: 	CALL	GETFLYMAT
    // asm 00002536: 	CALL	BOXSCAN		      	;KEEP FALLING!!!
    // asm 00002537: 	POP	R1			;CLEAN STACK
    // asm 00002538: 	BNC	FLYCARSTP
    // asm 00002539: 	ADDF	*+AR4(OPOSY),R0		;SET HIM ON THE GROUND
    // asm 0000253A: 	STF	R0,*+AR4(OPOSY)
    // asm 0000253B: 	CALL	SKID_SPARK
    // asm 0000253C: 	LDI	R1,R1			;DONE
    // asm 0000253D: 	BNE	FLYCARSTP		;YESSAH
FLYSTOPSLP:
    // asm 0000253E: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 0000253F: 	CALLNZ	SEND_FLY_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm 00002540: 	SLEEP	1
    // asm 00002542: 	B	FLYCARSTOP
FLYCARSTP:
    // asm 00002543: 	CMPI	2,R1
    // asm 00002544: 	BZ	DEADCAR			;RIGHT SIDE UP CARCASS
    // ;	LDPI	@SCOLLTABI,AR2	   	;RANDOM COLLISION CRUNCH
    // ;	LDI	3,R0
    // ;	CALL	DRONESND
    // *
    // *CAR IS UPSIDE DOWN
    // *
    // asm 00002545: 	LDI	*+AR4(OID),R0
    // asm 00002546: 	ANDN	TYPE_M,R0
    // asm 00002547: 	OR	DEAD_VEH_T,R0
    // asm 00002548: 	LDF	0,R0			;CLEAR OUT THE SPEED
    // asm 00002549: 	STF	R0,*+AR5(CARSPEED)
    // *WAIT FOR OFFSCREEN
FLYCARWT:
    // asm 0000254A: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 0000254B: 	CALLNZ	SEND_FLY_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm 0000254C: 	SLEEP	1
    // asm 0000254E: 	LDI	1,R4			;STATE #
    // asm 0000254F: 	LDI	*+AR4(ODIST),R0	     	;OUT OF RANGE?
    // asm 00002550: 	CMPI	-6000,R0
    // asm 00002551: 	BLT	FLYCARPXX		;YES CLEAN IT UP...
    // asm 00002552: 	LDF	*+AR5(CARSPEED),R0	;ARE WE HIT???
    // asm 00002553: 	BZ	FLYCARWT		;NO, JUST WAIT
    // *
    // *UPSIDE DOWN CAR IS HIT
    // *
    // ;	LDF	*+AR5(CARVROT),R2
    // ;	ADDF	1.57,R2		   	;CORRECT FOR 90 DEGREE ERROR
    // ;	CALL	_SINE
    // ;	LDF	*+AR5(CARSPEED),R3
    // ;	MPYF	R3,R0
    // ;	STF	R0,*+AR4(OVELX)	  	;CONVERT TO CARVROT, CARSPEED TO XZVEL
    // ;	CALL	_COSI
    // ;	MPYF	R3,R0
    // ;	STF	R0,*+AR4(OVELZ)
    // asm 00002554: 	LDF	0.1,R0		       	;ROCK HIM A LITTLE
    // asm 00002555: 	CALL	SFRAND
    // asm 00002556: 	STF	R0,*+AR7(PDATA)	  	;X RADIANS RATE
    // asm 00002557: 	B	FLYCARSTOP		;GO ROCK AND ROLL
    // *CLEAN UP THE MESS...
FLYCARPXX:
    // asm 00002558: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 00002559: 	BZ	FLYCARPXXXX		;NOPE...
    // asm 0000255A: 	CALL	COMPTRAK 		;OTHER GUY BEHIND?
    // asm 0000255B: 	BLE	FLYCARPXXX		;NO KILL THE DUDE...
    // asm 0000255C: 	CALL	SEND_FLY_XSFER
    // asm 0000255D: 	BR	OM_DRONE		;CONTROL SWAPS TO OTHER MACHINE
FLYCARPXXX:
    // asm 0000255E: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 0000255F: 	CALLNZ	SEND_FLY_KILL		;SEND YOUR POSITION TO LINKED GAME
FLYCARPXXXX:
    // asm 00002560: 	CALL	FREE_DRONE
    // asm 00002561: 	LDI	AR5,AR2
    // asm 00002562: 	CALL	DELCAR
    // asm 00002563: 	LDI	1,R0
    // asm 00002564: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm 00002565: 	XOR	*+AR4(OFLAGS),R0	;WIPE OUT PROCESS BIT
    // asm 00002566: 	STI	R0,*+AR4(OFLAGS)
    // asm 00002567: 	LDI	AR4,AR2
    // asm 00002568: 	CALL	OBJ_DELETE
    // asm 00002569: 	DIE
    // *
    // *CAR IS RIGHT SIDE UP
    // *
DEADCAR:
    // asm 0000256A: 	LDI	1,R0
    // asm 0000256B: 	STI	R0,*+AR5(CARSHAD) 	;TURN BACK ON THE SHADOW
    // asm 0000256C: 	LDI	*+AR4(OID),R0
    // asm 0000256D: 	ANDN	TYPE_M,R0
    // asm 0000256E: 	OR	DEAD_VEH_T,R0
    // asm 0000256F: 	STI	R0,*+AR4(OID)
    // asm 00002570: 	STI	R0,*+AR5(CAR_ID)
    // asm 00002571: 	STI	R0,*+AR7(PID)
    // asm 00002572: 	CLRF	R0
    // asm 00002573: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm 00002574: 	STF	R0,*+AR5(CARDROT)
    // asm 00002575: 	STF	R0,*+AR5(CARSPEED)
    // asm 00002576: 	LDF	*+AR7(PDATA+4),R0 	;GET Y ROTATION ORIENTATION
    // asm 00002577: 	STF	R0,*+AR5(CARVROT)
    // asm 00002578: 	STF	R0,*+AR5(CARYROT)
    // asm 00002579: 	LDI	0,R0
    // asm 0000257A: 	STI	R0,*+AR5(CAR_SPIN)	;CLEAR SPIN
DEADLP:
    // asm 0000257B: 	LDI	2,R4
    // asm 0000257C: 	LDI	*+AR4(ODIST),R0	      	;DIE OFF WHEN OFFSCREEN
    // asm 0000257D: 	CMPI	-6000,R0
    // asm 0000257E: 	BLT	FLYCARPXX
    // asm 0000257F: 	LDF	0,R2			;NO STEERING
    // asm 00002580: 	CALL	DRONEGO
    // asm 00002581: 	CALL	GETTRAK
    // asm 00002582: DEADSLP
    // asm 00002582: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm 00002583: 	CALLNZ	SEND_FLY_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm 00002584: 	SLEEP	1
    // asm 00002586: 	B	DEADLP
    // *
    // *KILL OFF FLY MESSAGE
    // *AR4= OBJECT
    // *AR5= CAR BLOCK
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYCARP", 0, 0);
    UNIMPL();
}

void SEND_FLY_KILL(void) {
    // asm 00002587: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00002588: 	LDI	CB_FLY_KILL,R1
    // asm 00002589: 	STI	R1,*AR2
    // asm 0000258A: 	LDI	*+AR5(CARNUM),R0
    // asm 0000258B: 	STI	R0,*+AR2(1)
    // asm 0000258C: 	LDI	2-1,RC
    // asm 0000258D: 	CALL	MESSAGE_ADD
    // asm 0000258E: 	RETS
    // *
    // *KILL OFF FLY
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_FLY_KILL", 0, 0);
    UNIMPL();
}

void DECODE_FLY_KILL(void) {
    // asm 0000258F: 	CALL	FIND_DRONE  		;GET DRONE OBJ IN AR0
    // asm 00002590: 	BNZ	DRKX
    // asm 00002591: 	LDI	*+AR0(OPLINK),AR7
    // asm 00002592: 	LDI	@FLYCARPXXXXI,R2 	;KILL THE SOMBITCH
    // asm 00002593: 	STI	R2,*+AR7(PWAKE)		;CHANGE WAKE-UP ADDR
DRKX:
    // asm 00002594: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_FLY_KILL", 0, 0);
    UNIMPL();
}

/*
 *
 *KILL OFF FLY MESSAGE
 *AR4= OBJECT
 *AR5= CAR BLOCK
 *AR7= PROCESS
 *R4=  STATE PARAMETER
 *
 */
static void SEND_FLY_XSFER(void) {
    // asm 00002597: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 00002598: 	LDI	CB_FLY_XSFER,R1
    // asm 00002599: 	STI	R1,*AR2++		;SEND HEADER
    // asm 0000259A: 	LDI	*+AR5(CARNUM),R1	;SEND ID
    // asm 0000259B: 	STI	R1,*AR2++
    // asm 0000259C: 	STI	R4,*AR2++		;SEND STATE
    // asm 0000259D: 	LDI	AR7,AR0
    // asm 0000259E: 	ADDI	PDATA,AR0		;GET PDATA 0-5
    // asm 0000259F: 	LDI	5,RC
    // asm 000025A0: 	RPTB	SENDP
    // asm 000025A1: 	LDI	*AR0++,R0
    // asm 000025A2: 	STI	R0,*AR2++
    // asm 000025A3: 	LSH	-8,R0
    // asm 000025A4: 	STI	R0,*AR2++
    // asm 000025A5: 	LSH	-8,R0
    // asm 000025A6: 	STI	R0,*AR2++
    // asm 000025A7: 	LSH	-8,R0
SENDP:
    // asm 000025A8: STI	R0,*AR2++
    // asm 000025A9: 	LDI	27-1,RC
    // asm 000025AA: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm 000025AB: 	CALL	MESSAGE_ADD
    // asm 000025AC: 	RETS
    // *
    // *GET A FLYER FROM OTHER GAME
    // *AR2=MESSAGE BUFFER
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_FLY_XSFER", 0, 0);
    UNIMPL();
}

void DECODE_FLY_XSFER(void) {
    // asm 000025AD: 	CALL	FIND_DRONE  		;GET DRONE OBJ IN AR0
    // asm 000025AE: 	BNZ	DFXX
    // asm 000025AF: 	LSH	R2,*AR2++,R4		;GET STATE
    // asm 000025B0: 	LDI	*+AR0(OPLINK),AR1
    // asm 000025B1: 	ADDI	PDATA,AR1		;GET PDATA 0-5
    // asm 000025B2: 	LDI	8,R5
    // asm 000025B3: 	LDI	5,RC
    // asm 000025B4: 	RPTB	DECP
    // asm 000025B5: 	LSH	R2,*AR2++,R0
    // asm 000025B6: 	LSH	R3,*AR2++,R1
    // asm 000025B7: 	ADDI	R1,R0
    // asm 000025B8: 	ADDI	*AR2++,R0
    // asm 000025B9: 	LSH	R5,*AR2++,R1
    // asm 000025BA: 	ADDI	R1,R0
DECP:
    // asm 000025BB: STI	R0,*AR1++
    // asm 000025BC: 	LDI	*+AR0(OCARBLK),AR5
    // asm 000025BD: 	LDI	*+AR0(OPLINK),AR7
    // asm 000025BE: 	LDI	0,R0			;OUR CAR NOW....
    // asm 000025BF: 	STI	R0,*+AR5(CAR_OM)
    // asm 000025C0: 	LDI	*+AR5(CARTRACK_ID),R2	;GET TRACK ID
    // asm 000025C1: 	LDI	@FLYCARPXXXI,R5 	;KILL THE SOMBITCH
    // asm 000025C2: 	LDI	@DYNALIST_END,AR0	;GET FURTHEST ROAD ID
    // asm 000025C3: 	LDI	*+AR0(OUSR1),R0
    // asm 000025C4: 	CMPI	R0,R2
    // asm 000025C5: 	BGT	DFX1			;TOO FAR OUT, DIE
    // asm 000025C6: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm 000025C7: 	LDI	*+AR0(OUSR1),R0
    // asm 000025C8: 	CMPI	R0,R2
    // asm 000025C9: 	BLT	DFX1			;BEHIND US KILL HIM
    // asm 000025CA: 	LDI	@DEADLPI,R5		;DEFAULT
    // asm 000025CB: 	CMPI	0,R4	  		;FLYIN'
    // asm 000025CC: 	LDIZ	@FLYCARP0I,R5
    // asm 000025CD: 	CMPI	1,R4	  		;ROCKIN'
    // asm 000025CE: 	LDIZ	@FLYCARSTOPI,R5
DFX1:
    // asm 000025CF: 	STI	R5,*+AR7(PWAKE)		;CHANGE WAKE-UP ADDR
    // asm 000025D0: 	RETS
DFXX:
    // asm 000025D1: 	ADDI	25,AR2			;SKIP REST OF MESSAGE
    // asm 000025D2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_FLY_XSFER", 0, 0);
    UNIMPL();
}

/* asm: FLYCARP0I	.WORD	FLYCARP */
#define FLYCARP0I FLYCARP

/*
 *----------------------------------------------------------------------------
 *GET MATRIX FOR FLYING CAR
 *
 *PARAMETERS
 *	PDATA+3,4,5 = X,Y,ZRAD
 *	AR4	OBJECT
 *	AR7	PROCESS
 *
 *R2,R3,AR2 TRASHED
 *
 */
void GETFLYMAT(void) {
    // asm 000025D6: 	LDF	*+AR7(PDATA+5),R2
    // asm 000025D7: 	LDI	AR4,AR2
    // asm 000025D8: 	ADDI	OMATRIX,AR2
    // asm 000025D9: 	CALL    FIND_ZMATRIX
    // asm 000025DA: 	LDPI	@MATRIXAI,AR2
    // asm 000025DB: 	LDF	*+AR7(PDATA+3),R2
    // asm 000025DC: 	CALL    FIND_XMATRIX
    // asm 000025DD: 	LDI	AR4,R2
    // asm 000025DE: 	ADDI	OMATRIX,R2
    // asm 000025DF: 	LDI	R2,R3
    // asm 000025E0: 	CALL	CONCATMAT		;FORMULATE COMBINED MATRIX
    // asm 000025E1: 	LDPI	@MATRIXAI,AR2
    // asm 000025E2: 	LDF	*+AR7(PDATA+4),R2
    // asm 000025E3: 	CALL    FIND_YMATRIX
    // asm 000025E4: 	LDI	AR4,R2
    // asm 000025E5: 	ADDI	OMATRIX,R2
    // asm 000025E6: 	LDI	R2,R3
    // asm 000025E7: 	B	CONCATMAT		;FORMULATE COMBINED MATRIX
    // ;*
    // ;*CONVERT XVEL,ZVEL TO CARSPEED,CARVROT
    // ;*AR4=OBJECT
    // ;*AR5=CAR OBJECT
    // ;*
    // ;GETCARVSPD
    // ;
    // ;	LDF	*+AR4(OVELX),R2		;UPDATE CARVROT, CARSPEED
    // ;	LDF	*+AR4(OVELZ),R3
    // ;	CALL	ARCTANF
    // ;	SUBF	1.57,R0
    // ;	STF	R0,*+AR5(CARVROT)
    // ;
    // ;	MPYF	R2,R2
    // ;	MPYF	R3,R3
    // ;	ADDF	R3,R2
    // ;	CALL	SQRT
    // ;	STF	R0,*+AR5(CARSPEED)
    // ;
    // ;	RETS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETFLYMAT", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *COLLISION SCAN
 *
 *CHECK OBJECT AGAINST LIST
 *
 *PARAMETERS
 *	AR0	OBJECT
 *	AR1	ADDRESS OF LIST HEADER
 *
 */
void COLSCAN(void) {
    // asm 000025E8: 	BD	COLSCL0
    // asm 000025E9: 	LDI	@_plyr1+PLY_CAR,AR0	;GET PLAYER CAR
    // asm 000025EA: 	LDI	@CAR_LISTI,AR1
    // asm 000025EB: 	SUBI	OLINK3,AR1		;SETUP INDEXING
    // ********B	COLSCL0
COLSCLP0:
    // asm 000025EC: 	BNZD	COLSCL		    	;DONT COLLIDE DUDES...
    // asm 000025ED: 	SUBF	*+AR1(OPOSX),R2,R0
    // asm 000025EE: 	MPYF	R0,R0
    // asm 000025EF: 	SUBF	*+AR1(IR0),R3,R4
    // ********BNZD	COLSCL
    // asm 000025F0: 	MPYF	R4,R4
    // asm 000025F1: 	ADDF	R0,R4
    // asm 000025F2: 	FLOAT	*+AR1(ORAD),R1
    // asm 000025F3: 	ADDF	R5,R1
    // asm 000025F4: 	MPYF	R1,R1			;SQUARE THE RADIUS LENGTH
    // asm 000025F5: 	CMPF	R1,R4	 		;ARE WE WITHIN RADIUS?
    // asm 000025F6: 	BGT	COLSCL
    // asm 000025F7: 	CALL	COLCHK			;CHECK OUT COLLISION FURTHER
    // asm 000025F8: 	BNC	COLSCL0			;NO COLLIDE
    // asm 000025F9: 	LDI	*+AR0(OCARBLK),AR4	;CHECK FOR LOW SPEED PLOW
    // asm 000025FA: 	LDF	*+AR4(CARSPEED),R0
    // asm 000025FB: 	LDI	*+AR1(OCARBLK),AR5	;CHECK FOR LOW SPEED PLOW
    // asm 000025FC: 	ADDF	*+AR5(CARSPEED),R0
    // asm 000025FD: 	CMPF	100,R0
    // asm 000025FE: 	LDFGT	100,R0
    // asm 000025FF: 	SUBRF	100,R0
    // asm 00002600: 	MPYF	0.02,R0
    // asm 00002601: 	ADDF	1.0,R0
    // asm 00002602: 	STF	R0,@PMULT		;SPEED MULTIPLIER
    // asm 00002603: 	B	COLDISP
COLSCL0:
    // asm 00002604: 	LDI	OPOSZ,IR0
    // asm 00002605: 	LDF	*+AR0(OPOSX),R2		;GET X COORD
    // asm 00002606: 	LDF	*+AR0(OPOSZ),R3		;GET Z COORD
    // asm 00002607: 	FLOAT	*+AR0(ORAD),R5		;GET SUCKERS RADIUS
COLSCL:
    // asm 00002608: 	LDI	*+AR1(OLINK3),R0
    // asm 00002609: 	BNZD	COLSCLP0
    // asm 0000260A: 	LDI	R0,AR1
    // asm 0000260B: 	LDI	*+AR1(OFLAGS),R0
    // asm 0000260C: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // ********BNZD	COLSCLP0
    // asm 0000260D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLSCAN", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *DRONE COLLISION SCAN
 *CHECK DRONES AGAINST DRONES
 *CHECK OBJECT AGAINST LIST
 *
 *PARAMETERS
 *	AR0	OBJECT
 *	AR1	ADDRESS OF LIST HEADER
 *
 */
static void CLDSCAN(void) {
    // asm 0000260E: 	LDPI	@CAR_LIST,R0	 	;GET LIST AND CHECK NULL
    // asm 0000260F: 	BNZD	CLDSCL0
    // asm 00002610: 	LDI	R0,AR0
    // asm 00002611: 	LDI	R0,AR1
    // asm 00002612: 	NOP
    // 	;------->BNZD CLDSCL0
    // asm 00002613: 	RETS
CLDSCLP0:
    // asm 00002614: 	BNZD	CLDSCL			;NOCOL BIT SET
    // asm 00002615: 	SUBF	*+AR1(OPOSX),R2,R0
    // asm 00002616: 	MPYF	R0,R0
    // asm 00002617: 	SUBF	*+AR1(IR0),R3,R4
    // 	;---->  BNZD	CLDSCL		;NOCOL BIT SET
    // asm 00002618: 	MPYF	R4,R4
    // asm 00002619: 	ADDF	R0,R4
    // asm 0000261A: 	FLOAT	*+AR1(ORAD),R1
    // asm 0000261B: 	ADDF	R5,R1
    // asm 0000261C: 	MPYF	R1,R1			;SQUARE THE RADIUS LENGTH
    // asm 0000261D: 	CMPF	R1,R4	 		;ARE WE WITHIN RADIUS?
    // asm 0000261E: 	BGT	CLDSCL
    // asm 0000261F: 	CALL	COLCHK			;CHECK OUT COLLISION FURTHER
    // asm 00002620: 	BC	COLDISP
CLDSCL0:
    // asm 00002621: 	LDI	*+AR0(OFLAGS),R0
    // asm 00002622: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // asm 00002623: 	BNZ	CLDSCL1			;NON COLLIDABLE STEALTH OBJECT
    // asm 00002624: 	LDF	*+AR0(OPOSX),R2		;GET X COORD
    // asm 00002625: 	LDF	*+AR0(OPOSZ),R3		;GET Z COORD
    // asm 00002626: 	LDI	OPOSZ,IR0
    // asm 00002627: 	FLOAT	*+AR0(ORAD),R5		;GET SUCKERS RADIUS
CLDSCL:
    // asm 00002628: 	LDI	*+AR1(OLINK3),R0
    // asm 00002629: 	BNZD	CLDSCLP0
    // asm 0000262A: 	LDI	R0,AR1
    // asm 0000262B: 	LDI	*+AR1(OFLAGS),R0
    // asm 0000262C: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // 	;------->BNZD	CLDSCLP0
CLDSCL1:
    // asm 0000262D: 	LDI	*+AR0(OLINK3),R0    	;GET NEXT LIST
    // asm 0000262E: 	BNZD	CLDSCL0
    // asm 0000262F: 	LDI	R0,AR1
    // asm 00002630: 	LDI	R0,AR0
    // asm 00002631: 	NOP
    // 	;------->BNZD CLDSCL0
    // asm 00002632: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLDSCAN", 0, 0);
    UNIMPL_TODO();
}

/*
 *----------------------------------------------------------------------------
 *REPELL COLLISION OBJECTS
 *
 *PARAMETERS
 *	AR0		OBJECT 0
 *	AR1		OBJECT 1
 *RETURNS
 *	AR2	POINTS TO NORMALIZED REPULSION VECTOR
 *	R0	VELOCITY MAGNITUDE OF REPULSION
 *
 *FIND REPULSION AXIS
 *
 */
static void REPELL(void) {
    // asm 00002633: 	LDPI	@VECTORAI,AR2		;COMPUTE REPULSION VECTOR
    // asm 00002634: 	LDF	*+AR0(OPOSX),R0
    // asm 00002635: 	SUBF	*+AR1(OPOSX),R0
    // asm 00002636: 	STF	R0,*AR2
    // asm 00002637: 	LDF	0,R0
    // asm 00002638: 	STF	R0,*+AR2(1)
    // asm 00002639: 	LDF	*+AR0(OPOSZ),R0
    // asm 0000263A: 	SUBF	*+AR1(OPOSZ),R0
    // asm 0000263B: 	STF	R0,*+AR2(2)
    // asm 0000263C: 	CALL	NORMALIZE		;NORMALIZE IT
    // *FIND RELATIVE VELOCITY MAGNITUDE
    // asm 0000263D: 	LDF	*+AR0(OVELX),R0
    // asm 0000263E: 	SUBF	*+AR1(OVELX),R0
    // asm 0000263F: 	MPYF	R0,R0
    // asm 00002640: 	LDF	*+AR0(OVELZ),R1
    // asm 00002641: 	SUBF	*+AR1(OVELZ),R1
    // asm 00002642: 	MPYF	R1,R1
    // asm 00002643: 	ADDF	R0,R1,R2
    // asm 00002644: 	BR	SQRT
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REPELL", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *COLLISION CHECK
 *
 *PARAMETERS
 *	AR0		OBJECT 0
 *	AR1		OBJECT 1
 *	*-AR3(1)	COLLISION PT
 *
 */
static void COLDISP(void) {
    // *SET COLLISION BITS
    // asm 00002645: 	LDI	*+AR0(OCARBLK),AR4
    // asm 00002646: 	LDI	*+AR1(OCARBLK),AR5
    // *check vs helicopter -> not a normal collision
    // asm 00002647: 	LDI	*+AR1(OID),R2
    // asm 00002648: 	CMPI	DRONE_C|HELICOPTER,R2
    // asm 00002649: 	BNE	NOTHELI
    // asm 0000264A: 	SUBF	*+AR1(OPOSX),*+AR0(OPOSX),R0
    // asm 0000264B: 	MPYF	R0,R0
    // asm 0000264C: 	LDF	*+AR0(OPOSY),R2
    // asm 0000264D: 	SUBF	*+AR1(OPOSY),R2
    // asm 0000264E: 	MPYF	R2,R2
    // asm 0000264F: 	ADDF	R0,R2
    // asm 00002650: 	LDF	*+AR0(OPOSZ),R0
    // asm 00002651: 	SUBF	*+AR1(OPOSZ),R0
    // asm 00002652: 	MPYF	R0,R0
    // asm 00002653: 	ADDF	R0,R2
    // asm 00002654: 	CALL	SQRT
    // asm 00002655: 	LDF	*+AR1(ORAD),R1
    // asm 00002656: 	MPYF	0.5,R1
    // asm 00002657: 	CMPF	R1,R0
    // asm 00002658: 	RETSGT
NOTHELI:
    // asm 00002659: 	LDI	*+AR0(OID),R2
    // asm 0000265A: 	AND	CLASS_M|TYPE_M,R2
    // asm 0000265B: 	CMPI	DRONE_C|RAILROAD,R2
    // asm 0000265C: 	BNE	NTRN
    // asm 0000265D: 	CMPI	*+AR1(OID),R2
    // asm 0000265E: 	RETSEQ
NTRN:
    // asm 0000265F: 	LDF	*+AR0(OPOSY),R0		;MAKE SURE HEIGHT IS CLOSE
    // asm 00002660: 	SUBF	*+AR1(OPOSY),R0
    // asm 00002661: 	ABSF	R0
    // asm 00002662: 	FLOAT	750,R1
    // asm 00002663: 	CMPF	R1,R0
    // asm 00002664: 	RETSGT				;IF HEIGHT TO FAR AWAY, FORGET IT...
    // asm 00002665: 	CALL	IMPACT_SPARK
    // *REPELL CARS
    // asm 00002666: 	CALL	REPELL	  		;R0=REPULSION MAGNITUDE
    // asm 00002667: 	CALL	COLSND			;MAKE YOUR SOUND...
    // asm 00002668: 	MPYF	0.5,R0	   		;ADJUST MAGNITUDE FOR 1/2 EACH OBJECT
    // asm 00002669: 	LDF	R0,R1
    // asm 0000266A: 	MPYF	*AR2,R0			;MULTIPLY BY X,Z DIRECTIONAL VECTOR
    // asm 0000266B: 	MPYF	*+AR2(2),R1
    // asm 0000266C: 	LDF	*+AR0(OPOSX),R2		;REPELL THE SUCKER (AR0)
    // asm 0000266D: 	LDF	*+AR0(OPOSZ),R3
    // asm 0000266E: 	ADDF	R0,R2
    // asm 0000266F: 	ADDF	R1,R3
    // asm 00002670: 	STF	R2,*+AR0(OPOSX)
    // asm 00002671: 	STF	R3,*+AR0(OPOSZ)
    // asm 00002672: 	LDI	*+AR1(OID),R2
    // asm 00002673: 	AND	CLASS_M|TYPE_M,R2
    // asm 00002674: 	CMPI	DRONE_C|RAILROAD,R2
    // asm 00002675: 	BEQ	FLYTRAIN
    // asm 00002676: 	LDF	*+AR1(OPOSX),R2		;REPELL THE SUCKER (AR1)
    // asm 00002677: 	LDF	*+AR1(OPOSZ),R3
    // asm 00002678: 	SUBF	R0,R2
    // asm 00002679: 	SUBF	R1,R3
    // asm 0000267A: 	STF	R2,*+AR1(OPOSX)
    // asm 0000267B: 	STF	R3,*+AR1(OPOSZ)
    // *
    // *ELASTIC COLLSION IN X AND Z
    // *FIND X AND Z VELOCITIES OF OBJ AR0
    // *	AR4	OCARBLK OBJECT AR0
    // *
    // * COMPUTE:	R4=XV1
    // *		R5=ZV1
    // *		R6=XV2
    // *		R7=ZV2
    // asm 0000267C: 	LDI	*+AR0(OCARBLK),AR4
    // asm 0000267D: 	LDF	*+AR4(CARVROT),R2
    // ;	STF	R2,@CAR1VROTI
    // asm 0000267E: 	ADDF	@HALFPII,R2	 	;CORRECT FOR 90 DEGREE ERROR
    // asm 0000267F: 	CALL	_SINE
    // asm 00002680: 	LDF	*+AR4(CARSPEED),R3
    // ;	STF	R3,@CAR1SPEEDI
    // asm 00002681: 	MPYF	R3,R0,R5		;V1Zi (INIT ZV OBJECT 1)
    // asm 00002682: 	CALL	_COSI
    // asm 00002683: 	MPYF	R3,R0,R4		;V1Xi (INIT XV OBJECT 1)
    // *FIND X AND Z VELOCITIES OF OBJ AR1
    // *AR5=OCARBLK OBJECT AR1
    // asm 00002684: 	LDI	*+AR1(OCARBLK),AR5
    // asm 00002685: 	LDF	*+AR5(CARVROT),R2
    // ;	STF	R2,@CAR2VROTI
    // asm 00002686: 	ADDF	@HALFPII,R2	   	;CORRECT FOR 90 DEGREE ERROR
    // asm 00002687: 	CALL	_SINE
    // asm 00002688: 	LDF	*+AR5(CARSPEED),R3
    // ;	STF	R3,@CAR2SPEEDI
    // asm 00002689: 	MPYF	R3,R0,R7		;V2Zi (INIT ZV OBJECT 2)
    // asm 0000268A: 	CALL	_COSI
    // asm 0000268B: 	MPYF	R3,R0,R6		;V2Xi (INIT XV OBJECT 2)
    // *CHECK FOR FLYING COLLISION
    // asm 0000268C: 	CMPI	@PLYCAR,AR0		;PLAYERS CAR?
    // asm 0000268D: 	BNZ	COLDISP0	  	;NO
    // ***************
    // ;	LDF	*+AR4(CARSPEED),R0	;PLAYER SPEED HIGH ENOUGH
    // ;	CMPF	60,R0
    // ;	BGT	FLYCAR		   	;FLY THE SUCKER...
    // ;	B 	COLDISP0
    // ******************
    // asm 0000268E: 	SUBF	R4,R6,R0
    // asm 0000268F: 	MPYF	R0,R0
    // asm 00002690: 	SUBF	R5,R7,R1
    // asm 00002691: 	MPYF	R1,R1
    // asm 00002692: 	ADDF	R1,R0			;FIND CLOSING SPEED SQUARED
    // asm 00002693: 	FLOAT	20000,R1 		;BIG MAGNITUDE ?
    // asm 00002694: 	MPYF	2,R1			;2X30000=60000
    // asm 00002695: 	CMPF	R1,R0
    // asm 00002696: 	BLT	COLDISP0		;NOT A FLYER
    // asm 00002697: 	PUSH	AR2			;SAVE REPULSION VECTOR DUDES...
    // asm 00002698: 	FLOAT	70,R1	   		;GET PROBABILITY FUNCTION
    // asm 00002699: 	CALL	DIV_F
    // asm 0000269A: 	FIX	R0,AR2
    // asm 0000269B: 	CALL	RANDPER
    // asm 0000269C: 	POP	AR2
    // asm 0000269D: 	BNC	COLDISP0      		;NOT A FLYER
    // asm 0000269E: 	LDF	*+AR4(CARSPEED),R0	;PLAYER SPEED HIGH ENOUGH
    // asm 0000269F: 	FLOAT	160,R1
    // asm 000026A0: 	CMPF	R1,R0
    // asm 000026A1: 	BGT	FLYCAR		   	;FLY THE SUCKER...
COLDISP0:
    // asm 000026A2: 	PUSH	AR3	 		;SAVE COLLISION POINT
    // asm 000026A3: 	LDPI	@MATRIXAI,AR3  		;GET TEMP STORE
    // *COMPUTE INELASTIC VELOCITY
    // asm 000026A4: 	LDF	*+AR4(CARMASS),R1	;GET MASS1
    // asm 000026A5: 	CMPI	@PLYCAR,AR0
    // asm 000026A6: 	BNE	COLIN1
    // asm 000026A7: 	MPYF	@CHEAT,R1		;BOOST MASS ON CHEAT
    // asm 000026A8: 	MPYF	@CHEAT,R1
    // asm 000026A9: 	MPYF	@CHEAT,R1
COLIN1:
    // asm 000026AA: 	PUSHF	R1
    // asm 000026AB: 	MPYF	R4,R1,R2		;M1XV1
    // asm 000026AC: 	MPYF	R5,R1,R3		;M1ZV1
    // asm 000026AD: 	LDF	*+AR5(CARMASS),R0	;GET MASS2
    // asm 000026AE: 	MPYF	R6,R0,R1		;M2XV2
    // asm 000026AF: 	ADDF	R1,R2
    // asm 000026B0: 	MPYF	R7,R0,R1		;M2ZV2
    // asm 000026B1: 	ADDF	R1,R3
    // asm 000026B2: 	POPF	R1
    // asm 000026B3: 	ADDF	R1,R0			;GET M1+M2
    // ;	ADDF	*+AR4(CARMASS),R0	;GET M1+M2
    // asm 000026B4: 	CALL	INV_F30
    // asm 000026B5: 	MPYF	R0,R2			;INELASTIC XV
    // asm 000026B6: 	MPYF	R0,R3			;INELASTIC ZV
    // asm 000026B7: 	STF	R2,*+AR3(4)	  	;SAVE INELASTIC XV
    // asm 000026B8: 	STF	R3,*+AR3(5)	  	;SAVE INELASTIC ZV
    // *COMPUTE (M1-M2)/(M1+M2)
    // asm 000026B9: 	LDF	*+AR4(CARMASS),R1
    // asm 000026BA: 	ADDF	*+AR5(CARMASS),R1
    // asm 000026BB: 	STF	R1,*+AR3(3)	  	;SAVE M1+M2
    // asm 000026BC: 	LDF	*+AR4(CARMASS),R0
    // asm 000026BD: 	SUBF	*+AR5(CARMASS),R0
    // asm 000026BE: 	CALL	DIV_F
    // asm 000026BF: 	STF	R0,*AR3
    // *COMPUTE 2*M2/(M1+M2)
    // asm 000026C0: 	LDF	*+AR3(3),R1
    // asm 000026C1: 	LDF	*+AR5(CARMASS),R0
    // asm 000026C2: 	MPYF	2,R0
    // asm 000026C3: 	CALL	DIV_F
    // asm 000026C4: 	STF	R0,*+AR3(1)
    // *COMPUTE 2*M1/(M1+M2)
    // asm 000026C5: 	LDF	*+AR3(3),R1
    // asm 000026C6: 	LDF	*+AR4(CARMASS),R0
    // asm 000026C7: 	MPYF	2,R0
    // asm 000026C8: 	CALL	DIV_F
    // asm 000026C9: 	STF	R0,*+AR3(2)  		;SAVE 2*M1/(M1+M2)
    // *X VELOCITY CASE OBJECT 1
    // asm 000026CA: 	MPYF	*AR3,R4,R0		;V1Xf = V1Xi(M1-M2)/(M1+M2)
    // asm 000026CB: 	MPYF	*+AR3(1),R6,R1		;       + V2Xi(2*M2)/(M1+M2)
    // asm 000026CC: 	ADDF	R1,R0,R2		;V1XF
    // *Z VELOCITY CASE OBJECT 1
    // asm 000026CD: 	MPYF	*AR3,R5,R0		;V1Zf = V1Zi(M1-M2)/(M1+M2)
    // asm 000026CE: 	MPYF	*+AR3(1),R7,R1		;       + V2Zi(2*M2)/(M1+M2)
    // asm 000026CF: 	ADDF	R1,R0,R3		;V1ZF
    // *ADD INELASTIC VELOCITY OBJECT 1
    // asm 000026D0: 	LDF	*+AR3(4),R0
    // asm 000026D1: 	LDF	*+AR3(5),R1
    // asm 000026D2: 	MPYF	0.75,R0
    // asm 000026D3: 	MPYF	0.75,R1
    // asm 000026D4: 	MPYF	0.25,R2
    // asm 000026D5: 	MPYF	0.25,R3
    // asm 000026D6:   	ADDF	R0,R2
    // asm 000026D7: 	ADDF	R1,R3
    // *ADD REPULSION VELOCITY	OBJECT 1
    // asm 000026D8: 	LDF	10.0,R0    		;VELOCITY REPULSION CONSTANT
    // ;	LDF	*+AR4(CARMASS),R1	;DIVIDE BY MASS
    // ;	CALL	DIV_F
    // asm 000026D9: 	LDF	*AR2,R1		  	;X REPULSION VELOCITY
    // asm 000026DA: 	MPYF	R0,R1
    // asm 000026DB: 	ADDF	R1,R2
    // asm 000026DC: 	LDF	*+AR2(2),R1		;Z REPULSION VELOCITY
    // asm 000026DD: 	MPYF	R0,R1
    // asm 000026DE: 	ADDF	R1,R3
    // **********debugging stuff
    // ;	STF	R4,@CAR1XVI	      	;SAVE YOUR VELOCITIES
    // ;	STF	R5,@CAR1ZVI
    // ;	STF	R6,@CAR2XVI
    // ;	STF	R7,@CAR2ZVI
    // *************************end debug stuff
    // asm 000026DF: 	CALL	ARCTANF
    // asm 000026E0: 	SUBPF	@HALFPII,R0
    // *STORE VEL THETA, SPEED
    // asm 000026E1: 	PUSHF	R0
    // ;	STF	R0,*+AR4(CARVROT)
    // asm 000026E2: 	MPYF	R2,R2
    // asm 000026E3: 	MPYF	R3,R3
    // asm 000026E4: 	ADDF	R3,R2
    // asm 000026E5: 	CALL	SQRT
    // asm 000026E6: 	STF	R0,*+AR4(CARSPEED)
    // ;	STF	R0,@CAR1SPEEDF		;SAVE FOR DEBUG
    // *X VELOCITY CASE OBJECT 2
    // asm 000026E7: 	NEGF	*AR3,R0			;(M2-M1)/(M1+M2)
    // asm 000026E8: 	STF	R0,*AR3
    // asm 000026E9: 	MPYF	*AR3,R6,R0		;V2Xf = V2Xi(M1-M2)/(M1+M2)
    // asm 000026EA: 	LDF	*+AR3(2),R3
    // asm 000026EB: 	MPYF	R3,R4,R1		;       + V1Xi(2*M1)/(M1+M2)
    // asm 000026EC: 	ADDF	R1,R0,R2		;V2XF
    // *Z VELOCITY CASE OBJECT 2
    // asm 000026ED: 	MPYF	*AR3,R7,R0		;V1Zf = V1Zi(M1-M2)/(M1+M2)
    // asm 000026EE: 	MPYF	R3,R5,R1		;       + V2Zi(2*M2)/(M1+M2)
    // asm 000026EF: 	ADDF	R1,R0,R3		;V2ZF
    // *ADD INELASTIC VELOCITY OBJECT 2
    // asm 000026F0: 	LDF	*+AR3(4),R0
    // asm 000026F1: 	LDF	*+AR3(5),R1
    // asm 000026F2: 	MPYF	0.75,R0
    // asm 000026F3: 	MPYF	0.75,R1
    // asm 000026F4: 	MPYF	0.25,R2
    // asm 000026F5: 	MPYF	0.25,R3
    // asm 000026F6: 	ADDF	R0,R2
    // asm 000026F7: 	ADDF	R1,R3
    // asm 000026F8: 	CMPI	@PLYCAR,AR0		;HIT BY PLAYERS CAR?
    // asm 000026F9: 	BNE	ZZZ1
    // asm 000026FA: 	MPYF	@PMULT,R2	       	;SLOW SPEED MULTIPLIER
    // asm 000026FB: 	MPYF	@PMULT,R3
ZZZ1:
    // *ADD REPULSION VELOCITY	OBJECT 2
    // asm 000026FC: 	LDF	-10.0,R0    		;VELOCITY REPULSION CONSTANT
    //  ;	LDF	*+AR5(CARMASS),R1	;DIVIDE BY MASS
    //  ;	CALL	DIV_F
    // asm 000026FD: 	LDF	*AR2,R1		  	;X REPULSION VELOCITY
    // asm 000026FE: 	MPYF	R0,R1
    // asm 000026FF: 	ADDF	R1,R2			;ADD TO XV
    // asm 00002700: 	LDF	*+AR2(2),R1		;Z REPULSION VELOCITY
    // asm 00002701: 	MPYF	R0,R1
    // asm 00002702: 	ADDF	R1,R3			;ADD TO ZV
    // asm 00002703: 	CALL	ARCTANF
    // asm 00002704: 	LDP	HALFPII
    // asm 00002705: 	SUBF	@HALFPII,R0
    // *STORE VEL THETA, SPEED
    // asm 00002706: 	PUSHF	R0	 		;SAVE NEW CARVROT
    // asm 00002707: 	MPYF	R2,R2
    // asm 00002708: 	MPYF	R3,R3
    // asm 00002709: 	ADDF	R3,R2
    // asm 0000270A: 	CALL	SQRT
    // asm 0000270B: 	STF	R0,*+AR5(CARSPEED)
    // ;	STF	R0,@CAR2SPEEDF		;SAVE FOR DEBUG
    // *NORMALIZE VELOCITY ROTATIONS OBJECT 2
    // asm 0000270C:   	POPF	R2
    // asm 0000270D: 	LDF	0.333,R0		;ADD A LITTLE RANDOM DIRECTION
    // asm 0000270E: 	CALL	SFRAND
    // asm 0000270F: 	ADDF	R0,R2
    // asm 00002710: 	CALL	NORMITS
    // asm 00002711: 	STF	R2,*+AR5(CARVROT)
    // ;	STF	R2,@CAR2VROTF		;SAVE FOR DEBUG
    // *NORMALIZE VELOCITY ROTATIONS OBJECT 1
    // asm 00002712:   	POPF	R2
    // asm 00002713: 	CALL	NORMITS
    // asm 00002714: 	STF	R2,*+AR4(CARVROT)
    // ;	STF	R2,@CAR1VROTF		;SAVE FOR DEBUG
    // asm 00002715: 	POP	AR3
    // *
    // *FIND ROTATIONAL FORCE AND DIRECTION IN XZ PLANE
    // *
    // *ORIGIN = OBJECT CENTER
    // *VECTOR 1= COLLISION POINT
    // *VECTOR 2= COLLISION POINT + RELATIVE VELOCITY
    // *AR0=OBJECT 0
    // *AR1=OBJECT 1
    // *R4= XV OBJ 1 (initial)
    // *R5= ZV OBJ 1 (initial)
    // *R6= XV OBJ 2 (initial)
    // *R7= ZV OBJ 2 (initial)
    // *AR3-1=COLLISION PT
    // *AR4 =CAR STRUCT AR0
    // *AR5 =CAR STRUCT AR1
    // *
    // *CHECK FOR SPIN
    // *
    // asm 00002716: 	SUBF   	R6,R4,R0	;GET RELATIVE XV
    // asm 00002717: 	SUBF   	R7,R5,R1
    // asm 00002718: 	CALL	SPINROT
    // asm 00002719: COLDSP30
    // asm 00002719: 	PUSH	AR1
    // asm 0000271A: 	PUSH	AR0
    // asm 0000271B: 	PUSH	AR1
    // asm 0000271C: 	LDI	AR5,AR1		;SWAP AR4,AR5
    // asm 0000271D: 	LDI	AR4,AR5
    // asm 0000271E: 	LDI	AR1,AR4
    // asm 0000271F: 	LDI	AR0,AR1
    // asm 00002720: 	POP	AR0		;SWAP AR0,AR1
    // asm 00002721: 	SUBF  	R4,R6,R0	;GET RELATIVE XV
    // asm 00002722: 	SUBF  	R5,R7,R1	;GET RELATIVE ZV
    // asm 00002723: 	CALL	SPINROT
    // asm 00002724: 	POP	AR0
    // asm 00002725: 	POP	AR1
    // asm 00002726: COLDSPX
    // asm 00002726: 	RETS	       	       	;FOR NOW DUDES
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLDISP", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *SPINROT	CALCULATE SPIN ROTATION
 *
 *PARAMETERS
 *	AR0	OBJECT THAT HIT ME
 *	AR1	OBJECT
 *	AR3	COLLISION POINT VECTOR
 *	AR4	CAR BLOCK POINTER OBJECT THAT HIT ME
 *	AR5	CAR BLOCK POINTER
 *	R0	XV RELATIVE
 *	R1	ZV RELATIVE
 *
 *CALCS
 *    	*+AR5(CARDROT)  AMOUNT TO SPIN CAR
 *    	*+AR5(CAR_BUMP) 0=SMALL COLLISION, 1=BIG COLLISION
 *    	*+AR5(CAR_SPIN)	SPIN TIME/ FLAG
 *    	*+AR5(CARSPRAD)	RADIANS TO SPIN
 */

/* asm: PLYRBEHIND	.BSS	PLYRBEHIND,1 */
int PLYRBEHIND;

// *
static void SPINROT(void) {
    // asm 00002727: 	LDI	0,R2
    // asm 00002728: 	STI	R2,@PLYRBEHIND		;PLAYER HIT FROM BEHIND FLAG
    // asm 00002729: 	PUSHF	R0
    // asm 0000272A: 	PUSHF	R1
    // asm 0000272B: 	MPYF	R0,R0	   		;GET INTENSITY OF RELATIVE SPEED
    // asm 0000272C: 	MPYF	R1,R1
    // asm 0000272D: 	ADDF	R0,R1,R2
    // asm 0000272E: 	CALL	SQRT
    // asm 0000272F: 	LDF	R0,R3			;SAVE INTENSITY
    // asm 00002730: 	CMPF	20,R0			;SET BUMP FLAG FOR BIGGIE
    // asm 00002731: 	LDIGT	1,R2
    // asm 00002732: 	LDILE	0,R2
    // asm 00002733: 	STI	R2,*+AR5(CAR_BUMP)
    // asm 00002734: 	POPF	R1
    // asm 00002735: 	POPF	R0
    // asm 00002736: 	PUSHF	R3	      		;SAVE INTENSITY
    // asm 00002737: 	LDPI	@VECTORAI,AR2
    // asm 00002738: 	LDF	*+AR1(OPOSX),R2	   	;GET OBJECT2 XZ CENTER ORIGIN
    // asm 00002739: 	SUBF3	R2,*-AR3(1),R2		;GET RELATIVE POSITION OF COLLISION PT.
    // asm 0000273A: 	STF	R2,*AR2
    // asm 0000273B: 	LDF	*+AR1(OPOSZ),R2
    // asm 0000273C:  	SUBF3	R2,*+AR3(1),R2
    // asm 0000273D: 	STF	R2,*+AR2(1)
    // *GET RELATIVE VELOCITY OBJECT AR1 PERSPECTIVE
    // asm 0000273E: 	MPYF	0.5,R0		;MAKE THIS SMALL
    // asm 0000273F: 	ADDF	*AR2,R0
    // asm 00002740: 	STF	R0,*+AR2(2)
    // asm 00002741: 	MPYF	0.5,R1		;MAKE THIS SMALL
    // asm 00002742: 	ADDF	*+AR2(1),R1
    // asm 00002743: 	STF	R1,*+AR2(3)
    // *COMPUTE CROSS PRODUCT
    // asm 00002744: 	MPYF	*AR2,R1,R1
    // asm 00002745: 	MPYF	*+AR2(1),R0,R0
    // asm 00002746: 	SUBF	R0,R1 			;THIS CROSS PRODUCT SIGN
    // asm 00002747: 	PUSHF	R1			;SAVE THE SIGN DUDES
    // *GET ROTATIONAL INTENSITY
    // *NORMALIZE VECTORS
    // *NORMALIZE RADIUS VECTOR
    // asm 00002748: 	MPYF	*AR2,*AR2,R3
    // asm 00002749: 	MPYF	*+AR2(1),*+AR2(1),R1
    // asm 0000274A: 	ADDF	R1,R3,R2
    // asm 0000274B: 	CALL	SQRT
    // asm 0000274C: 	PUSHF	R0			;SAVE THE RADIUS LENGTH
    // asm 0000274D: 	CALL	INV_F30
    // asm 0000274E: 	MPYF	*AR2,R0,R1
    // asm 0000274F: 	STF	R1,*AR2
    // asm 00002750: 	MPYF	*+AR2(1),R0,R1
    // asm 00002751: 	STF	R1,*+AR2(1)
    // *NORMALIZE IMPACT VECTOR
    // asm 00002752: 	LDF	*+AR2(2),R1
    // asm 00002753: 	MPYF	R1,R1,R3
    // asm 00002754: 	LDF	*+AR2(3),R1
    // asm 00002755: 	MPYF	R1,R1
    // asm 00002756: 	ADDF	R1,R3,R2
    // asm 00002757: 	CALL	SQRT
    // asm 00002758: 	CALL	INV_F30
    // asm 00002759: 	LDF	R0,R1
    // asm 0000275A: 	MPYF	*+AR2(2),R0
    // asm 0000275B: 	MPYF	*+AR2(3),R1
    // *COMPUTE DOT PRODUCT TO GET ANGLE
    // *COSINE=DOT PRODUCT
    // asm 0000275C: 	MPYF	*AR2,R0
    // asm 0000275D: 	MPYF	*+AR2(1),R1
    // asm 0000275E: 	ADDF	R0,R1			;R3=DOT PRODUCT
    // asm 0000275F: 	ABSF	R1
    // asm 00002760: 	SUBRF	1.0,R1
    // asm 00002761: 	POPF	R0			;GET RADIUS LENGTH
    // asm 00002762: 	MPYF	R0,R1
    // asm 00002763: 	MPYF	2,R1			;FUDGE FACTOR
    // asm 00002764: 	POPF	R0			;GET SIGN
    // asm 00002765:        	LDFN	-15.0,R0		;LOAD FUDGE FACTOR
    // asm 00002766:        	LDFNN	15.0,R0
    // asm 00002767: 	MPYF	R1,R0			;DO IT DUDE
    // asm 00002768: 	LDF	*+AR5(CARMASS),R1	;ADJUST FOR MASS
    // asm 00002769: 	CMPF	2.0,R1
    // asm 0000276A: 	LDFGE	10.0,R1			;HEAVY MASS ADJUSTMENT
    // asm 0000276B: 	CALL	DIV_F
    // asm 0000276C: 	STF	R0,@SPINTEMP		;SPIN TIME TEMP
    // *
    // *GET THE SPIN TIME
    // *
    // *R0	ROTATION SPEED (FLOAT)
    // *AR1	OBJECT
    // *AR5	CAR BLOCK POINTER
    // *
    // asm 0000276D: 	POPF	R3			;GET INTENSITY OF COLLISION
    // asm 0000276E: 	STF	R3,@COLVEL		;COLLISION RELATIVE VELOCITY
    // asm 0000276F: 	LDI	*+AR5(CAR_SPIN),R1	;CHECK IF ALREADY SPINNING
    // asm 00002770: 	CMPI	1,R1
    // asm 00002771: 	BZ	SPINNIT			;YES...
    // asm 00002772: 	CMPI	@PLYCAR,AR1		;PLAYERS CAR?
    // asm 00002773: 	BNZ	DRONESPIN
    // *PLAYER SPIN
    // *
    // *R0	ROTATION SPEED (FLOAT)
    // *R3	COLLSION RELATIVE SPEED
    // *AR1	OBJECT
    // *AR5	CAR BLOCK POINTER
    // *
    // asm 00002774: PLYRSPIN
    // asm 00002774: 	CALL	BEHINDCK		;CHECK IF PLAYER HIT FROM BEHIND
    // asm 00002775: 	CMPF	50,R3
    // asm 00002776: 	BGT	PLSPIN1			;BIG BUMP...
    // asm 00002777: 	CALL	CKBOUNCE
    // asm 00002778: 	BNC	SPINBUMP
    // asm 00002779: 	B	SPINBOUNCE
PLSPIN1:
    // asm 0000277A: 	CMPF	100,R3
    // asm 0000277B: 	BGT	PLBIG			;SPIN, RELATIVE VELOCITY LARGE
    // asm 0000277C: 	LDI	500,AR2			;SPIN PROBABILITY
    // asm 0000277D: 	CALL	RANDPER
    // asm 0000277E: 	BC	PLSPIN2	      		;NORMAL SPIN
    // asm 0000277F: 	CALL	CKBOUNCE
    // asm 00002780: 	BC	SPINBOUNCE	   	;DO A BOUNCE
PLSPIN2:
    // asm 00002781: 	LDI	@CAMVIEW,R2
    // asm 00002782: 	LDINZ	150,AR2
    // asm 00002783: 	LDIZ	100,AR2			;LESS SPIN 1ST PERSON
    // asm 00002784: 	CALL	RANDPER
    // asm 00002785: 	BC	PSPINNIT       		;NORMAL SPIN
    // asm 00002786: 	B	SPINBUMP
PLBIG:
    // asm 00002787: 	LDI	@CAMVIEW,R2
    // asm 00002788: 	LDINZ	500,AR2
    // asm 00002789: 	LDIZ	250,AR2			;LESS SPIN 1ST PERSON
    // asm 0000278A: 	CALL	RANDPER
    // asm 0000278B: 	BC	PSPINNIT       		;NORMAL SPIN
    // asm 0000278C: 	CALL	CKBOUNCE
    // asm 0000278D: 	BC	SPINBOUNCE	   	;DO A BOUNCE
    // asm 0000278E: 	B	SPINBUMP
    // *
    // *CHECK IF PLAYER HIT FROM BEHIND
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPINROT", 0, 0);
    UNIMPL();
}

static void BEHINDCK(void) {
    // asm 0000278F: 	PUSHF	R0
    // asm 00002790: 	PUSHF	R3
    // asm 00002791: 	PUSH	AR2
    // asm 00002792: 	PUSH	AR3
    // asm 00002793: 	PUSH	AR4
    // asm 00002794: 	PUSH	AR0
    // asm 00002795: 	LDI	AR0,AR2
    // asm 00002796: 	LDI	AR4,AR3
    // asm 00002797:       	LDI	AR1,AR4
    // asm 00002798: 	CALL	CKAHEAD	       		;IS PLAYER AHEAD?
    // asm 00002799: 	LDIGE	0,R0			;NO
    // asm 0000279A: 	LDILT	1,R0			;YES
    // asm 0000279B: 	STI	R0,@PLYRBEHIND
    // asm 0000279C: 	POP	AR0
    // asm 0000279D: 	POP	AR4
    // asm 0000279E: 	POP	AR3
    // asm 0000279F: 	POP	AR2
    // asm 000027A0: 	POPF	R3
    // asm 000027A1: 	POPF	R0
    // asm 000027A2: 	RETS
    // *
    // *CHECK PLAYER SPINNOUT
    // *
PSPINNIT:
    // asm 000027A3: 	LDI	@PLYRBEHIND,R2
    // asm 000027A4: 	BNE	SPINBUMP		;YES, JUST BUMP THE DUDE
    // asm 000027A5: 	B 	SPINNIT			;NO, SPIN 'EM OUT
SPINBOUNCE:
    // asm 000027A6: 	LDF	*+AR5(CARSPEED),R4
    // asm 000027A7: 	CMPF	20,R4		      	;MINIMUM SPEED VALUE
    // asm 000027A8: 	LDFLT	20,R4
    // asm 000027A9: 	STF	R4,*+AR5(CARSPEED)    	;REVERSE SPEED
    // asm 000027AA: 	MPYF	1.5,R4
    // asm 000027AB: 	FIX	R4,R0			;BOUNCE TIME
    // asm 000027AC: 	CMPI	60,R0
    // asm 000027AD: 	LDIGT	60,R0			;MAX AT 40
    // asm 000027AE: 	STI	R0,*+AR5(CAR_SPIN)
    // asm 000027AF: 	FLOAT	R0,R1
    // asm 000027B0: 	LDF	R2,R0	  		;MOVE TO MIDDLE
    // asm 000027B1: 	CALL	DIV_F
    // asm 000027B2: 	MPYF	-6.0,R0
    // ;	LDF	R2,R2			;CORRECTION FACTOR
    // ;	LDFGT	-0.04,R0
    // ;	LDFLE	0.04,R0
    // ;;	LDF	0,R0
    // asm 000027B3: 	STF	R0,*+AR5(CARDROT)
    // asm 000027B4: 	LDF	0,R1			;CARSPRAD
    // asm 000027B5: 	B	SPINXX
    // *DRONE SPIN
    // *R0	ROTATION SPEED (FLOAT)
    // *R3	COLLSION RELATIVE SPEED
DRONESPIN:
    // asm 000027B6: 	ABSF	R0,R1	     		;COMPUTE SPIN PROBABILITY
    // asm 000027B7: 	CMPF	0.1,R1
    // asm 000027B8: 	BLT	SPINBUMP		;NO SPIN, TOO SMALL
    // asm 000027B9: 	CMPF	30,R3
    // asm 000027BA: 	BLT	SPINBUMP		;NO SPIN, RELATIVE VELOCITY SMALL
    // asm 000027BB: 	CMPF	140,R3
    // asm 000027BC: 	BGT	DSPIN			;SPIN, RELATIVE VELOCITY LARGE
    // asm 000027BD: 	FIX	R3,AR2			;GET SPIN PROBABILITY
    // asm 000027BE: 	SUBI	30,AR2
    // asm 000027BF: 	MPYI	6,AR2
    // asm 000027C0: 	ADDI	160,AR2
    // asm 000027C1: 	CALL	RANDPER
    // asm 000027C2: 	BNC	SPINBUMP
    // *
    // *DRONE TOTAL SPINOUT
    // *
DSPIN:
    // asm 000027C3: 	ABSF	@SPINTEMP,R2     	;GET SPIN MAGNITUDE
    // asm 000027C4: 	CMPF	10,R2
    // asm 000027C5: 	LDFGT	10,R2
    // asm 000027C6: 	MPYF	0.1,R2
    // asm 000027C7: 	MPYF	0.08,R2
    // asm 000027C8: 	LDF	0.06,R0	     		;GET SOME RANDOMNESS IN SPIN RATE
    // asm 000027C9: 	CALL	FRAND
    // asm 000027CA: 	ADDF	0.04,R0
    // asm 000027CB: 	ADDF	R0,R2
    // asm 000027CC: 	LDF	@SPINTEMP,R0		;GET SIGN (SPIN DIRECTION)
    // asm 000027CD: 	BNN	DSPIN1
    // asm 000027CE: 	NEGF	R2
DSPIN1:
    // asm 000027CF: 	LDF	6.28,R0			;ONCE OR TWICE AROUND
    // asm 000027D0: 	CALL	FRAND
    // asm 000027D1: 	LDF	3.14,R1			;CARSPRAD
    // asm 000027D2: 	ADDF	R0,R1
    // asm 000027D3: 	LDF	*+AR5(CARMASS),R0  	;HEAVY MASS ?
    // asm 000027D4: 	CMPF	2.0,R0
    // asm 000027D5: 	BLT	DSPIN0			;NOPE
    // asm 000027D6: 	MPYF	0.5,R2			;CUT DOWN SPIN SPEED
    // asm 000027D7: 	LDF	3.14,R1			;CARSPRAD
DSPIN0:
    // asm 000027D8: 	CALL	ANGMOM
    // asm 000027D9: 	STF	R2,*+AR5(CARDROT)
    // asm 000027DA: 	LDI	1,R0   			;SET RADIAN SPIN FLAG
    // asm 000027DB: 	B	SPINX
    // *
    // * SPINNIT: TOTAL SPINOUT
    // *SET RANGE TO .08 -.12 DUDES
    // *
SPINNIT:
    // asm 000027DC: 	ABSF	@SPINTEMP,R2     	;GET SPIN MAGNITUDE
    // asm 000027DD: 	CMPF	10,R2
    // asm 000027DE: 	LDFGT	10,R2
    // asm 000027DF: 	MPYF	0.1,R2
    // asm 000027E0: 	MPYF	0.08,R2
    // asm 000027E1: 	ADDF	0.1,R2
    // asm 000027E2: 	LDF	@SPINTEMP,R0		;GET SIGN (SPIN DIRECTION)
    // asm 000027E3: 	BNN	SPINTM0
    // asm 000027E4: 	NEGF	R2
SPINTM0:
    // asm 000027E5: 	CALL	ANGMOM
    // asm 000027E6: 	CMPI	@PLYCAR,AR1		;PLAYERS CAR?
    // asm 000027E7: 	LDFZ	0.08,R1			;PLAYER MIN
    // asm 000027E8: 	LDFNZ	0.02,R1			;DRONE MIN
    // asm 000027E9: 	ABSF	R2,R0  			;MINIMUM SPIN RATE FOR PLAYER
    // asm 000027EA: 	CMPF	R1,R0
    // asm 000027EB: 	BGT	SPINTM1
    // asm 000027EC: 	LDF	R2,R2
    // asm 000027ED: 	LDFLT	-1,R2
    // asm 000027EE: 	LDFGE	1,R2
    // asm 000027EF: 	MPYF	R1,R2
SPINTM1:
    // asm 000027F0: 	STF	R2,*+AR5(CARDROT)
    // asm 000027F1: 	LDI	*+AR5(CAR_SPIN),R1	;CHECK IF ALREADY SPINNING
    // asm 000027F2: 	CMPI	1,R1
    // asm 000027F3: 	LDFZ	*+AR5(CARSPRAD),R1	;YES...	SAVE CARSPRAD
    // asm 000027F4: 	LDFNZ	3.14,R1			;NO NEW CARSPRAD
    // asm 000027F5: 	LDI	1,R0
    // asm 000027F6: 	B	SPINX
    // *
    // *MOMENTARY BUMP SPIN
    // *R3=RELATIVE VELOCITY OF HIT
    // *
SPINBUMP:
    // asm 000027F7: 	LDF	@SPINTEMP,R2
    // asm 000027F8: 	CMPF	0.05,R2
    // asm 000027F9: 	LDFGT	0.05,R2
    // asm 000027FA: 	CMPF	-0.05,R2
    // asm 000027FB: 	LDFLT	-0.05,R2
    // asm 000027FC: 	CALL	ANGMOM			;ADJUST ANGULAR MOMENTUM
    // asm 000027FD: 	CMPF	80,R3
    // asm 000027FE: 	LDFGT	80,R3
    // asm 000027FF: 	MPYF	0.0125,R3		;ADJUST	ROTATE FOR REL VEL
    // asm 00002800: 	MPYF	R3,R2
    // asm 00002801: 	LDF	@COLVEL,R0
    // asm 00002802: 	MPYF	0.4,R0
    // asm 00002803: 	CMPF	35,R0
    // asm 00002804: 	LDFGT	35,R0
    // asm 00002805: 	CALL	FRAND
    // asm 00002806: 	CMPI	@PLYCAR,AR1		;ARE WE PLAYERS CAR?
    // asm 00002807: 	BNE	SPINB0
    // asm 00002808: 	MPYF	0.7,R0			;LESS SPIN FOR PLAYER
SPINB0:
    // asm 00002809: 	CMPF	8,R0
    // asm 0000280A: 	LDFLT	8,R0
    // asm 0000280B: 	FIX	R0
    // asm 0000280C: 	CMPI	@PLYCAR,AR0		;HIT BY PLAYERS CAR?
    // asm 0000280D: 	BNZ	SPINBUMP1		;NO
    // asm 0000280E: 	ADDI	5,R0			;YES, BOOST SPIN TIME
SPINBUMP1:
    // asm 0000280F: 	LDI	@PLYRBEHIND,R1		;REDUCE FOR PLAYER HIT BEHIND
    // asm 00002810: 	BZ	SPINBX
    // asm 00002811: 	CMPI	14,R0			;MAX TIME AT 14
    // asm 00002812: 	LDIGT	14,R0
    // asm 00002813: 	LSH	-1,R0			;DIVIDE TIME BY 1/2
SPINBX:
    // asm 00002814: 	STF	R2,*+AR5(CARDROT)
    // asm 00002815: 	LDF	0,R1			;CARSPRAD
SPINX:
    // asm 00002816: 	STI	R0,*+AR5(CAR_SPIN)
SPINXX:
    // asm 00002817: 	STF	R1,*+AR5(CARSPRAD)
    // asm 00002818: 	LDF	*+AR1(OMAT11),R0	;IF CAR FLIPPED, REVERSE CARDROT
    // asm 00002819: 	BGE	SPINXXX
    // asm 0000281A: 	NEGF	*+AR5(CARDROT),R0
    // asm 0000281B: 	STF	R0,*+AR5(CARDROT)
SPINXXX:
    // asm 0000281C: 	RETS
    // *
    // *ANGULAR MOMENTUM
    // *R2=NEW ANGULAR MOMENTUM
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BEHINDCK", 0, 0);
    UNIMPL();
}

static void ANGMOM(void) {
    // asm 0000281D: 	LDI	*+AR5(CAR_SPIN),R3 	;ADD IN EXISTING INERTIA
    // asm 0000281E: 	BZ	ANGM1
    // asm 0000281F: 	LDF	*+AR5(CARDROT),R3	;GET OLD MOMENTUM
    // asm 00002820: 	MPYF	0.5,R3	     		;FUDGE FACTOR
    // asm 00002821: 	ADDF	R3,R2
    // asm 00002822: 	MPYF	0.67,R2
ANGM1:
    // asm 00002823: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ANGMOM", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *
 *CHECK FOR BOUNCE ALLOWED
 *AR5 CAR BLOCK
 *C=O.K., NC=FAIL
 *R0=ROADIR
 *R2=YROT-ROADIR
 *
 */
static void CKBOUNCE(void) {
    // asm 00002824: 	CALL	ROADIR			;GET DIRECTIONAL DIFFERENCE
    // asm 00002825: 	LDF	*+AR5(CARVROT),R1
    // asm 00002826: 	SUBF	R1,R0,R2
    // asm 00002827: 	CALL	NORMITS
    // asm 00002828: 	ABSF	R2,R3			;VELOCITY BACKWARDS?
    // asm 00002829: 	CMPF	1.75,R3
    // asm 0000282A: 	BLT	CKBNCX		      	;NO...
    // asm 0000282B: 	LDF	*+AR5(CARYROT),R2	;JUST BOUNCE HIM WITH TIMED SPIN
    // asm 0000282C: 	SUBF	R0,R2
    // asm 0000282D: 	CALL	NORMITS
    // asm 0000282E: 	ABSF	R2,R3
    // asm 0000282F: 	CMPF	1.4,R3
    // asm 00002830: 	BGT	CKBNCX			;DIRECTION OUT OF RANGE
    // asm 00002831: 	SETC
    // asm 00002832: 	RETS
CKBNCX:
    // asm 00002833: 	CLRC
    // asm 00002834: 	RETS
    // *
    // *CAR TO CAR COLLISION SOUND
    // *AR0=VEHICLE #1
    // *AR1=VEHICLE #2
    // *R0=IMPACT SPEED
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKBOUNCE", 0, 0);
    UNIMPL();
}

static void COLSND(void) {
    // asm 00002835: 	PUSHF	R0
    // asm 00002836: 	PUSH	AR0
    // asm 00002837: 	PUSH	AR1
    // asm 00002838: 	PUSH	AR2
    // asm 00002839: 	FLOAT	@NFRAMES,R1
    // asm 0000283A: 	CALL	DIV_F			;ADJUST IMPACT FOR FRAME RATE
    // asm 0000283B: 	LDF	R0,R1
    // asm 0000283C: 	MPYF	0.01,R1
    // asm 0000283D: 	MPYF	0.7,R1
    // asm 0000283E: 	CMPF	1,R1
    // asm 0000283F: 	LDFGT	1,R1
    // asm 00002840: 	MPYF	128,R1
    // asm 00002841: 	ADDF	127,R1
    // asm 00002842: 	FIX	R1	   		;VOLUME ADJUSTER
    // asm 00002843: 	LDF	*+AR1(OMAT11),R0	;IS DRONE FLIPPED?
    // asm 00002844: 	LDILT	@SCUPDTABI,AR2		;UPSIDE DOWN HIT SOUND
    // asm 00002845: 	LDILT	4,R0			;FOUR TABLE ENTRIES
    // asm 00002846: 	BLT	COLSND1
    // asm 00002847: 	LDI	@SCTABI,AR2
    // asm 00002848: 	CMPI	220,R1
    // asm 00002849: 	LDILT	5,R0
    // asm 0000284A: 	LDIGE	3,R0
COLSND1:
    // asm 0000284B: 	CMPI	@PLYCAR,AR0		;PLAYERS CAR?
    // asm 0000284C: 	BNZ	DRCOLSND		;NO, DO DRONE SOUND
    // asm 0000284D: 	CALL	RANDVSND       		;DO COLLISION SOUND+EXIT
    // asm 0000284E: 	B	COLSNDX
    // * DRONE VS. DRONE
DRCOLSND:
    // asm 0000284F: 	PUSH	AR4
    // asm 00002850: 	LDI	AR0,AR4
    // asm 00002851: 	CALL	DRONESND
    // asm 00002852: 	POP	AR4
COLSNDX:
    // asm 00002853: 	POP	AR2
    // asm 00002854: 	POP	AR1
    // asm 00002855: 	POP	AR0
    // asm 00002856: 	POPF	R0
    // asm 00002857: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLSND", 0, 0);
    UNIMPL();
}

/* asm: SCUPDTAB	.word	SCOLLF,SCOLLF,SCOLLG,SCOLLH */
static int SCUPDTAB[] = {
    SCOLLF,
    SCOLLF,
    SCOLLG,
    SCOLLH,
};
/* asm: SCTAB		.WORD	SCOLLA,SCOLLB,SCOLLC,SCOLLD,SCOLLE */
/* asm: 	 */
/* asm: 	 */
static int SCTAB[] = {
    SCOLLA,
    SCOLLB,
    SCOLLC,
    SCOLLD,
    SCOLLE,
    // ----------------------------------------------------------------------------
    // DISPATCH COLLISION
    // ----------------------------------------------------------------------------
    // CHECK COLLISION BETWEEN 2 OBJECTS
    //
    // PARAMETERS
    // 	AR0	OBJECT 1
    // 	AR1	OBJECT 2
    // RETURNS
    // 	CARRY SET FOR COLLISION
    //
    // BLOWLIST ALLOCATIONS
    // BLOWLIST+00 : OBJ0 BOX PTS (ORIGINAL)
    // BLOWLIST+24 : OBJ0 BOX PTS (ROTATED, TRANLSATED)
    // BLOWLIST+48 : OBJ1 BOX PTS (ORIGINAL)
    // BLOWLIST+72 : OBJ1 BOX PTS (ROTATED, TRANSLATED)
    // BLOWLIST+96 : OBJ0 PLANE EQUATIONS
    // BLOWLIST+120: OBJ1 PLANE EQUATIONS
};

void COLCHK(void) {
    // asm 00002863:  	PUSH	R2
    // asm 00002864: 	PUSH	R3
    // asm 00002865: 	PUSH	R5
    // asm 00002866: 	PUSH	IR0
    // asm 00002867: 	PUSH	AR0
    // asm 00002868: 	PUSH	AR1
    // asm 00002869: 	LDPI	@BLOWLISTI,AR2
    // asm 0000286A: 	CALL	GETBOX			;GET BOX POINTS FOR OBJECT 1
    // asm 0000286B: 	LDI	AR1,AR0
    // asm 0000286C: 	CALL	GETBOX			;GET BOX POINTS FOR OBJECT 2
    // *GET 6 EQUATIONS FOR BOX
    // *GET PLANE EQUATION
    // *4 COEFFICIENTS PER EQUATION
    // *N1,N2,N3,P.N WHERE N=NORMAL VECTOR, P= 1ST POINT IN LIST
    // *
    // asm 0000286D: 	LDI	AR2,AR0			;STORE NORMAL VECTORS HERE
    // asm 0000286E: 	LDPI	@EQTABI,AR2
    // asm 0000286F: 	LDI	11,RC 			;DO 2 X 6 EQUATIONS
    // asm 00002870: 	RPTB	PLANEQ
    // asm 00002871: 	PUSH	AR2
    // asm 00002872: 	CALL	GEN_NORMAL		;GETS NORMAL VECTOR
    // asm 00002873: 	POP	AR2
    // asm 00002874: 	LDI	*AR2++(3),AR3		;GET FIRST POINT
    // asm 00002875: 	MPYF	*AR0++,*AR3++,R0   	;COMPUTE DOT PRODUCT
    // asm 00002876: 	MPYF	*AR0++,*AR3++,R1
    // asm 00002877: 	MPYF	*AR0++,*AR3++,R0
    // asm 00002877: ||	ADDF	R0,R1,R2
    // asm 00002878: 	ADDF	R0,R2
PLANEQ:
    // asm 00002879: STF	R2,*AR0++		;SAVE DOT PRODUCT
    // *CHECK POINTS OBJ1 VS EQ OBJ0
    // asm 0000287A: 	SUBI	48,AR0			;GET OBJ 0 EQUATION BASE ADDR
    // asm 0000287B: 	LDI	AR0,AR3
    // asm 0000287C: 	SUBI	23,AR3			;GET INDEX PROJ POINTS OBJ 1+1
    // asm 0000287D: 	LDI	7,AR4 			;DO 8 POINTS
    // asm 0000287E: PNTCKL0
    // asm 0000287E: 	LDI	AR0,AR2
    // asm 0000287F: 	LDI	5,RC 			;DO 6 EQUATIONS
    // asm 00002880: 	RPTB	EQCHK0
    // asm 00002881: 	MPYF	*AR2++,*-AR3(1),R0
    // asm 00002882: 	MPYF	*AR2++,*AR3,R1
    // asm 00002883: 	MPYF	*AR2++,*+AR3(1),R0
    // asm 00002883: ||	ADDF	R0,R1,R2
    // asm 00002884: 	ADDF	R0,R2
    // asm 00002885: 	CMPF	*AR2++,R2
    // asm 00002886: 	BLT	PNTNXT0			;THIS POINT FAILED, GET A NEW ONE
EQCHK0:
    // asm 00002887: NOP
    // asm 00002888: 	BU	GOTCOL			;GOT A COLLISION
PNTNXT0:
    // asm 00002889: 	NOP	*AR3++(3)
    // asm 0000288A: 	DBU	AR4,PNTCKL0
    // *CHECK POINTS OBJ0 VS EQ OBJ1
    // asm 0000288B: 	ADDI	24,AR0			;GET OBJ1 EQUATION BASE
    // asm 0000288C: 	LDI	AR0,AR3
    // asm 0000288D: 	SUBI	95,AR3			;GET INDEX PROJ POINTS OBJ 1+1
    // asm 0000288E: 	LDI	7,AR4 			;DO 8 POINTS
    // asm 0000288F: PNTCKL1
    // asm 0000288F: 	LDI	AR0,AR2
    // asm 00002890: 	LDI	5,RC 			;DO 6 EQUATIONS
    // asm 00002891: 	RPTB	EQCHK1
    // asm 00002892: 	MPYF	*AR2++,*-AR3(1),R0
    // asm 00002893: 	MPYF	*AR2++,*AR3,R1
    // asm 00002894: 	MPYF	*AR2++,*+AR3(1),R0
    // asm 00002894: ||	ADDF	R0,R1,R2
    // asm 00002895: 	ADDF	R0,R2
    // asm 00002896: 	CMPF	*AR2++,R2
    // asm 00002897: 	BLT	PNTNXT1			;THIS POINT FAILED, GET A NEW ONE
EQCHK1:
    // asm 00002898: NOP
    // asm 00002899: 	BU	GOTCOL			;GOT A COLLISION
PNTNXT1:
    // asm 0000289A: 	NOP	*AR3++(3)
    // asm 0000289B: 	DBU	AR4,PNTCKL1
    // asm 0000289C: 	CLRC				;NO COLLISION
    // asm 0000289D: 	POP	AR1
    // asm 0000289E: 	POP	AR0
COLCHKX:
    // asm 0000289F: 	POP	IR0
    // asm 000028A0: 	POP	R5
    // asm 000028A1: 	POP	R3
    // asm 000028A2:  	POP	R2
    // asm 000028A3: 	RETS
GOTCOL:
    // asm 000028A4: 	POP	AR1  			;GET COLLIDING OBJECTS
    // asm 000028A5: 	POP	AR0
    // asm 000028A6: 	SETC
    // asm 000028A7: 	BU	COLCHKX
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLCHK", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *
 *PROJECT BOX POINTS FOR OBJECT AR0
 *R0=SIZING PARAMETER (1.00=TRUE SIZE)
 *AR0= OBJECT
 *AR2= STORAGE AREA FOR POINTS AND PROJECTION (48 WORDS)
 *RET AR2= END OF STROAGE AREA+1
 *TRASHES R0-R5
 *
 */
static void GETBOX(void) {
    // asm 000028A8: 	LDF	1.0,R0		;XMINUS MULT FACTOR
    // asm 000028A9: 	LDF	1.0,R1		;YMINUS MULT FACTOR
    // asm 000028AA: 	LDF	1.0,R2		;ZMINUS MULT FACTOR
    // asm 000028AB: 	LDF	1.0,R3		;XPLUS MULT FACTOR
    // asm 000028AC: 	LDF	1.0,R4		;YPLUS MULT FACTOR
    // asm 000028AD: 	LDF	1.0,R5		;ZPLUS MULT FACTOR
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETBOX", 0, 0);
    UNIMPL();
}

static void GETBOX0(void) {
    // asm 000028AE: 	PUSH	AR4
    // asm 000028AF: 	PUSH	AR5
    // asm 000028B0: 	PUSH	AR6
    // *GET 8 POINTS FOR OBJ 0
    // asm 000028B1: 	LDI	*+AR0(OCARBLK),AR5
    // asm 000028B2: 	MPYF	*+AR5(CARXMINUS),R0
    // asm 000028B3: 	MPYF	*+AR5(CARYMINUS),R1
    // asm 000028B4: 	MPYF	*+AR5(CARZMINUS),R2
    // asm 000028B5: 	MPYF	*+AR5(CARXPLUS),R3
    // asm 000028B6: 	MPYF	*+AR5(CARYPLUS),R4
    // asm 000028B7: 	MPYF	*+AR5(CARZPLUS),R5
    // asm 000028B8: 	LDI	2,IR0
    // asm 000028B9: 	PUSH	AR2
    // asm 000028BA: 	NOP	*AR2++
    // asm 000028BB: 	STF	R0,*-AR2(1)    	;X1
    // asm 000028BB: ||	STF	R1,*AR2++(IR0)	;Y1
    // asm 000028BC: 	STF	R2,*-AR2(1)	;Z1
    // asm 000028BC: ||	STF	R3,*AR2++(IR0)	;X2
    // asm 000028BD: 	STF	R1,*-AR2(1)	;Y2
    // asm 000028BD: ||	STF	R2,*AR2++(IR0)	;Z2
    // asm 000028BE: 	STF	R0,*-AR2(1)    	;X3
    // asm 000028BE: ||	STF	R4,*AR2++(IR0)	;Y3
    // asm 000028BF: 	STF	R2,*-AR2(1)	;Z3
    // asm 000028BF: ||	STF	R3,*AR2++(IR0)	;X4
    // asm 000028C0: 	STF	R4,*-AR2(1)	;Y4
    // asm 000028C0: ||	STF	R2,*AR2++(IR0)	;Z4
    // asm 000028C1: 	STF	R0,*-AR2(1)    	;X5
    // asm 000028C1: ||	STF	R1,*AR2++(IR0)	;Y5
    // asm 000028C2: 	STF	R5,*-AR2(1)	;Z5
    // asm 000028C2: ||	STF	R3,*AR2++(IR0)	;X6
    // asm 000028C3: 	STF	R1,*-AR2(1)	;Y6
    // asm 000028C3: ||	STF	R5,*AR2++(IR0)	;Z6
    // asm 000028C4: 	STF	R0,*-AR2(1)    	;X7
    // asm 000028C4: ||	STF	R4,*AR2++(IR0)	;Y7
    // asm 000028C5: 	STF	R5,*-AR2(1)	;Z7
    // asm 000028C5: ||	STF	R3,*AR2++(IR0)	;X8
    // asm 000028C6: 	STF	R4,*-AR2(1)	;Y8
    // asm 000028C6: ||	STF	R5,*AR2++	;Z8
    // *
    // *ROTATE POINTS FOR OBJ 0
    // *AR0=OBJ 0
    // *AR1=OBJ 1
    // *AR2=RAM1 STORAGE AREA
    // *AR3=
    // *AR4=INDEX TO POINTS
    // *AR5=ROTATIONAL MATRIX
    // *AR6=TRANSVECTOR
    // *
    // asm 000028C7: 	LDI	AR0,AR5
    // asm 000028C8: 	ADDI	OMATRIX,AR5		;rotational matrix
    // asm 000028C9: 	POP	AR4			;GET POINTER TO POINT LIST
    // asm 000028CA: 	SUBI	2,AR4			;SETUP AT -2
    // asm 000028CB: 	LDPI	@transvectorYI,AR6	;transvector temp store
    // asm 000028CC: 	LDI	8,IR0
    // asm 000028CD: 	LDF	*+AR0(OPOSX),R0
    // asm 000028CE: 	LDF	*+AR0(OPOSY),R1
    // asm 000028CF: 	STF	R0,*-AR6(1)		;transvector.x
    // asm 000028CF:  ||	STF	R1,*AR6			;transvector.y
    // asm 000028D0: 	LDF	*+AR0(OPOSZ),R0
    // asm 000028D1: 	STF	R0,*+AR6(1)		;transvector.z
    // asm 000028D2: 	LDI	7,RC
    // asm 000028D3: 	RPTB	EOCV
    // asm 000028D4: 	LDF	*++AR4(3),R4
    // *
    // *MULTIPLY BY ROTATION MATRIX
    // *AND ADD TRANSLATION (IN THAT ORDER)
    // *
    // asm 000028D5: 	MPYF3	*AR5++,*-AR4(1),R0
    // asm 000028D6: 	MPYF3	*AR5++,R4,R1
    // asm 000028D7: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm 000028D7:  ||	ADDF3	R0,R1,R2
    // asm 000028D8: 	MPYF3	*AR5++,*-AR4(1),R0
    // asm 000028D8:  ||	ADDF3	R1,R2,R2
    // asm 000028D9: 	ADDF	*-AR6(1),R2		;*blowlist++   += translation[X]
    // asm 000028DA: 	MPYF3	*AR5++,R4,R1
    // asm 000028DA:  ||	STF	R2,*AR2++ 		;STORE ROTATED X
    // asm 000028DB: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm 000028DB:  ||	ADDF3	R0,R1,R2
    // asm 000028DC: 	MPYF3	*AR5++,*-AR4(1),R0
    // asm 000028DC:  ||	ADDF3	R1,R2,R3
    // asm 000028DD: 	ADDF	*AR6,R3			;*blowlist++   += translation[Y]
    // asm 000028DE: 	MPYF3	*AR5++,R4,R1
    // asm 000028DE:  ||	STF	R3,*AR2++		;STORE ROTATED Y
    // asm 000028DF: 	MPYF3	*AR5--(IR0),*+AR4(1),R1
    // asm 000028DF:  ||	ADDF3	R0,R1,R2
    // asm 000028E0: 	ADDF	R1,R2			;FORM ROTATED Z
    // asm 000028E1: 	ADDF	*+AR6(1),R2		;ADD IN TRANSLATION Z
EOCV:
    // asm 000028E2: STF	R2,*AR2++		;STORE Z
    // asm 000028E3: 	POP	AR6
    // asm 000028E4: 	POP	AR5
    // asm 000028E5: 	POP	AR4
    // asm 000028E6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETBOX0", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *TABLE OF 6 BOX EQUATIONS
 *CLOCKWISE ORDER
 */
#define VCTO (BLOWLIST + 24)
#define VCTO1 (BLOWLIST + 72)
/* asm: EQTABI	.WORD	EQTAB */
#define EQTABI EQTAB
/* asm: EQTAB */
/* asm: 	.WORD	VCTO+(3*0),VCTO+(3*2),VCTO+(3*3)	;FRONT */
/* asm: 	.WORD	VCTO+(3*0),VCTO+(3*4),VCTO+(3*6)	;LSIDE */
/* asm: 	.WORD	VCTO+(3*0),VCTO+(3*1),VCTO+(3*5) 	;BOTTOM */
/* asm: 	.WORD	VCTO+(3*2),VCTO+(3*6),VCTO+(3*7)	;TOP */
/* asm: 	.WORD	VCTO+(3*1),VCTO+(3*3),VCTO+(3*7)	;RSIDE */
/* asm: 	.WORD	VCTO+(3*7),VCTO+(3*6),VCTO+(3*4)	;BACK */
/* asm: 	.WORD	VCTO1+(3*0),VCTO1+(3*2),VCTO1+(3*3)	;FRONT */
/* asm: 	.WORD	VCTO1+(3*0),VCTO1+(3*4),VCTO1+(3*6)	;LSIDE */
/* asm: 	.WORD	VCTO1+(3*0),VCTO1+(3*1),VCTO1+(3*5) 	;BOTTOM */
/* asm: 	.WORD	VCTO1+(3*2),VCTO1+(3*6),VCTO1+(3*7)	;TOP */
/* asm: 	.WORD	VCTO1+(3*1),VCTO1+(3*3),VCTO1+(3*7)	;RSIDE */
/* asm: 	.WORD	VCTO1+(3*7),VCTO1+(3*6),VCTO1+(3*4)	;BACK */
/* asm: 	 */
static f32* EQTAB[] = {
    // OBJECT 0
    VCTO + (3 * 0),
    VCTO + (3 * 2),
    VCTO + (3 * 3), // FRONT
    VCTO + (3 * 0),
    VCTO + (3 * 4),
    VCTO + (3 * 6), // LSIDE
    VCTO + (3 * 0),
    VCTO + (3 * 1),
    VCTO + (3 * 5), // BOTTOM
    VCTO + (3 * 2),
    VCTO + (3 * 6),
    VCTO + (3 * 7), // TOP
    VCTO + (3 * 1),
    VCTO + (3 * 3),
    VCTO + (3 * 7), // RSIDE
    VCTO + (3 * 7),
    VCTO + (3 * 6),
    VCTO + (3 * 4), // BACK
    // OBJECT 1
    VCTO1 + (3 * 0),
    VCTO1 + (3 * 2),
    VCTO1 + (3 * 3), // FRONT
    VCTO1 + (3 * 0),
    VCTO1 + (3 * 4),
    VCTO1 + (3 * 6), // LSIDE
    VCTO1 + (3 * 0),
    VCTO1 + (3 * 1),
    VCTO1 + (3 * 5), // BOTTOM
    VCTO1 + (3 * 2),
    VCTO1 + (3 * 6),
    VCTO1 + (3 * 7), // TOP
    VCTO1 + (3 * 1),
    VCTO1 + (3 * 3),
    VCTO1 + (3 * 7), // RSIDE
    VCTO1 + (3 * 7),
    VCTO1 + (3 * 6),
    VCTO1 + (3 * 4), // BACK
};
// * LINE EQUATION TABLE PTS 2-6-7-3

/* asm: LEQTABI	.WORD	LEQTAB */
#define LEQTABI LEQTAB
/* asm: LEQTAB */
/* asm: 	.WORD	VCTO+(3*2)+1,VCTO+(3*6)+1,VCTO+(3*7)+1 */
/* asm: 	.WORD	VCTO+(3*3)+1,VCTO+(3*2)+1 */
static f32* LEQTAB[] = {
    VCTO + (3 * 2) + 1,
    VCTO + (3 * 6) + 1,
    VCTO + (3 * 7) + 1,
    VCTO + (3 * 3) + 1,
    VCTO + (3 * 2) + 1,
};

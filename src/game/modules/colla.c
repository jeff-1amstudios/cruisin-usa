#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
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
#include "../include/dirq.h"
#include "../include/delta.h"
#include "../include/comm.h"
#include "../include/colla.h"

/*
 * Source module: asm/COLLA.ASM
 */

// *----------------------------------------------------------------------------
// *COLLISION SYSTEM
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: VLI	.word	VL */
int VLI = (int)(VL);
/* asm: TNORMI	.word	TNORM */
int TNORMI = (int)(TNORM);
/* asm: TVECT1I	.word	TVECT1 */
int TVECT1I = (int)(TVECT1);
/* asm: TVECT2I	.word	TVECT2 */
int TVECT2I = (int)(TVECT2);
// *----------------------------------------------------------------------------
// *CAMERA SCAN FOR ROAD HEIGHT
// *
// *PARAMETERS
// *	AR4	POINTS TO VECTOR CAMERA X,Y,Z
// *RETURNS
// *	R0 = CAMERA HEIGHT ABOVE THE ROAD
// *	CS = COLLISION FOUND WITH ROAD
// *	CC = NO COLLISION FOUND
// *
// *----------------------------------------------------------------------------
// *SCAN COLLSION BOX VS. ROAD
// *
// *AR4=OBJECT
// *RET=R0=HT ABOVE ROAD (MIN)
// *CS=FOUND A HIT, CC= NOTHING BELOW
/* asm: BOXSCRAMI	.word	BOXSCRAM */
int BOXSCRAMI = (int)(BOXSCRAM);
// *----------------------------------------------------------------------------
// *CHECK OBJECT COLLISION WITH VECTOR IN X/Z SPACE
// *
// *THIS ROUTINE MUST BE FIRST BECAUSE OF .ALIGN BUG IN LINKER/ASSEMBLER
// ;	.align
// ;	NOP
// ;	NOP
// ;	NOP
// ;	LDI	*AR4++(5),R1
// *
// *CHECK POLYGON PT COLLISION
// *AR1,AR3,AR5,AR6= PTR TO VERTICES
// *IMPLIED COLLISION POINT IS 0,0,0
// *
// 	;------>BZD	VLTRI
// *----------------------------------------------------------------------------
// ;	LDI	@_ATTR_MODE,R0
// ;	CMPI	-3,R0		;BUG IN FUTURE
// 				;display while in the attract mode
// *----------------------------------------------------------------------------
// *FLYING SIGN COLLISION PROCESS
// *
// *AR4= SIGN OBJECT
// *
/* asm: FLYCOLLPI	.word	FLYCOLLP */
int FLYCOLLPI = (int)(FLYCOLLP);
// *----------------------------------------------------------------------------
// *
// *KILL OFFSCREEN ROAD DEBRIS
// *
// *----------------------------------------------------------------------------
// *FALLING SIGN PROCESS
// *
// *PARAMETERS
// *	AR4	SIGN OBJECT
// *	R7	ROTATION DELTA
// *
/* asm: SIGNFALLI	.word	SIGNFALL */
int SIGNFALLI = (int)(SIGNFALL);
// *----------------------------------------------------------------------------
// *TREE SHAKE PROCESS
// *
// *PARAMETERS
// *	AR4	SIGN OBJECT
// *	R7	ROTATION DELTA
// *
/* asm: TREESHAKI	.word	TREESHAK */
int TREESHAKI = (int)(TREESHAK);
// *SHAKE IT FORWARD
// *SHAKE IT BACK
// ;	LDP	@MATRIXAI
// *----------------------------------------------------------------------------
// *
// *FLYING CAR WRECK
// *
// *	AR0	POINTS TO PLAYER CAR
// *	AR1	POINTS TO CAR TO SEND FLYING
// *	AR4	POINTS TO PLAYER CAR BLOCK
// *	AR5	POINTS TO DRONE CAR BLOCK
// *
/* asm: SBUSI	.word	sbus */
int SBUSI = (int)(sbus);
/* asm: CBUSI	.word	cbus */
int CBUSI = (int)(cbus);
// *----------------------------------------------------------------------------
// *SPINROT	CALCULATE SPIN ROTATION
// *
// *PARAMETERS
// *	AR0	OBJECT THAT HIT ME
// *	AR1	OBJECT
// *	AR3	COLLISION POINT VECTOR
// *	AR4	CAR BLOCK POINTER OBJECT THAT HIT ME
// *	AR5	CAR BLOCK POINTER
// *	R0	XV RELATIVE
// *	R1	ZV RELATIVE
// *
// *CALCS
// *    	*+AR5(CARDROT)  AMOUNT TO SPIN CAR
// *    	*+AR5(CAR_BUMP) 0=SMALL COLLISION, 1=BIG COLLISION
// *    	*+AR5(CAR_SPIN)	SPIN TIME/ FLAG
// *    	*+AR5(CARSPRAD)	RADIANS TO SPIN
// *----------------------------------------------------------------------------
// *
// *CHECK FOR BOUNCE ALLOWED
// *AR5 CAR BLOCK
// *C=O.K., NC=FAIL
// *R0=ROADIR
// *R2=YROT-ROADIR
// *
// *
// *CAR TO CAR COLLISION SOUND
// *AR0=VEHICLE #1
// *AR1=VEHICLE #2
// *R0=IMPACT SPEED
// *
// * DRONE VS. DRONE
/* asm: SCUPDTABI	.word	SCUPDTAB */
int SCUPDTABI = (int)(SCUPDTAB);
/* asm: SCUPDTAB	.word	SCOLLF,SCOLLF,SCOLLG,SCOLLH */
int SCUPDTAB[] = { SCOLLF, SCOLLF, SCOLLG, SCOLLH };
/* asm: SCTABI	.word	SCTAB */
int SCTABI = (int)(SCTAB);
/* asm: SCTAB	.word	SCOLLA,SCOLLB,SCOLLC,SCOLLD,SCOLLE */
int SCTAB[] = { SCOLLA, SCOLLB, SCOLLC, SCOLLD, SCOLLE };
// *----------------------------------------------------------------------------
// *TABLE OF 6 BOX EQUATIONS
// *CLOCKWISE ORDER
#define VCTO (BLOWLIST+24)
#define VCTO1 (BLOWLIST+72)
/* asm: EQTABI	.word	EQTAB */
int EQTABI = (int)(EQTAB);
// * OBJECT 0
// * OBJECT 1
// * LINE EQUATION TABLE PTS 2-6-7-3
/* asm: LEQTABI	.word	LEQTAB */
int LEQTABI = (int)(LEQTAB);

void CAMSCAN(void)
{
    // asm: 	LDPI	@DRIVE_LIST,R0
    // asm: 	CALL	CAMSCANS
    // asm: 	RETSC
    // asm: 	LDPI	@GROUND_LIST,R0
    // *FALL THRU TO CAMSCANS
    // *
    // *SCAN LIST FOR POINT-OBJECT INTERSECTION
    // asm: CAMSCANS
    // asm: 	BZ	CMSX  			;NULL LIST DUDES
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	OPOSZ,IR1
    // asm: 	LDF	*+AR4(2),R2		;GET POINT X
    // asm: 	LDF	*AR4,R3			;GET POINT Z
CMS0:
    // asm: 	SUBF	*+AR2(OPOSX),R3,R0
    // asm: 	SUBF	*+AR2(IR1),R2,R1
    // asm: 	MPYF	R1,R1
CMS1:
    // asm: 	MPYF	R0,R0
    // asm: 	ADDF	R1,R0
    // asm: 	FLOAT	*+AR2(ORAD),R1	 	;GET ROAD RADIUS
    // asm: 	MPYF	R1,R1
    // asm: 	CMPF	R1,R0	  		;DISTANCE < RADIUS ?
    // asm: 	BLT	CMS2			;YES, CHECK IT OUT
    // asm: 	LDI	*+AR2(OLINK3),AR2
    // asm: 	LDI	AR2,R1
    // asm: 	BNZD	CMS1
    // asm: 	SUBF	*+AR2(OPOSX),R3,R0
    // asm: 	SUBF	*+AR2(IR1),R2,R1
    // asm: 	MPYF	R1,R1
    // 	;---->	BNZ	CMS1
    // asm: 	B	CMSX			;WE FAILED
    // *CHECK OUT POINT COLLISION
CMS2:
    // asm: 	CALL	_coll_road		;XZ POINT COLLISION WITH ROAD OBJECT?
    // asm: 	BNC	CMS1L
    // asm: 	RETS				;RETURN COLLISION VALUE
CMS1L:
    // asm: 	LDI	*+AR2(OLINK3),AR2
    // asm: 	LDI	AR2,R1
    // asm: 	BNZD	CMS0
    // asm: 	LDI	OPOSZ,IR1
    // asm: 	LDF	*+AR4(2),R2		;GET POINT X
    // asm: 	LDF	*AR4,R3			;GET POINT Z
    // 	;---->	BNZ	CMS0
CMSX:
    // asm: 	FLOAT	0,R0		;DEFAULT HT.
    // asm: 	CLRC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMSCAN", 0, 0);
    UNIMPL();
}

void OBJSCAN(void)
{
    // *----------------------------------------------------------------------------
    // *SCAN OBJECT CENTER POINT VERSUS ROAD
    // *
    // *PARAMETERS
    // *	AR4	OBJECT
    // *RETURNS
    // *	CARRY SET IF ROAD FOUND BELOW OBJECT
    // asm: 	PUSH	AR4
    // asm: 	ADDI	OPOSX,AR4
    // asm: 	CALL	CAMSCAN	      		;AR4=XYZ, RET R0=HT, CS=ROAD,CC=NO ROAD
    // asm: 	POP	AR4
    // asm:  	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJSCAN", 0, 0);
    UNIMPL();
}

void BOXSCAN(void)
{
    // *
    // *CHECK ROAD OBJECTS ON ROAD LIST IN RANGE
    // *
    // asm: 	PUSH	AR5
    // asm: 	LDPI	@BOXSCRAMI,AR2
    // asm: 	LDI	AR4,AR0			;GET OBJ IN AR0 FOR GETBOX
    // asm: 	LDF	0.7,R0			;XMINUS MULT FACTOR
    // asm: 	LDF	0.7,R1			;YMINUS MULT FACTOR
    // asm: 	LDF	0.7,R2			;ZMINUS MULT FACTOR
    // asm: 	LDF	0.7,R3			;XPLUS MULT FACTOR
    // asm: 	LDF	1.0,R4			;YPLUS MULT FACTOR
    // asm: 	LDF	0.7,R5			;ZPLUS MULT FACTOR
    // asm: 	CALL	GETBOX0			;GET BOX POINTS FOR OBJECT 1
    // asm: 	FLOAT	*+AR4(ORAD),R5	 	;GET ROAD RADIUS
    // asm: 	FLOAT	20000,R7		;DEFAULT CAR HT. ABOVE GROUND
    // asm: 	LDPI	@DRIVE_LIST,R0
    // asm: 	CALL	BOXSCSUB
    // asm: 	LDPI	@GROUND_LIST,R0
    // asm: 	CALL	BOXSCSUB
    // asm: 	POP	AR5
    // asm: 	LDF	R7,R0
    // asm: 	FLOAT 	20000,R7
    // asm: 	CMPF	R7,R0
    // asm: 	BLT	BS3X
    // asm: 	CLRC	 			;NOTHING FOUND
    // asm: 	RETS
BS3X:
    // asm: 	SETC
    // asm: 	RETS
    // *
    // *SCAN BOX FOR GROUND INTERSECTION
    // *R0=LIST
    // *AR4=OBJECT
    // *R5=QUICK REJECT LIMIT
    // *R7=LOWEST HEIGHT SO FAR
    // *
    // asm: BOXSCSUB
    // asm: 	BZ	BSCX  			;NULL LIST DUDES
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	OPOSZ,IR1
BS0:
    // asm: 	FLOAT	*+AR4(ORAD),R0		;GET BOX RADIUS
    // asm: 	LDF	*+AR4(OPOSZ),R4		;GET OBJECT Z
    // asm: 	LDF	*+AR4(OPOSX),R3		;GET OBJECT X
    // asm: 	SUBF	*+AR2(OPOSX),R3,R2
    // asm: 	SUBF	*+AR2(IR1),R4,R1
    // asm: 	MPYF	R1,R1
BS1:
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R1,R2
    // asm: 	FLOAT	*+AR2(ORAD),R1	 	;GET ROAD RADIUS
    // asm: 	ADDF	R0,R1			;ADD AND SQUARE
    // asm: 	MPYF	R1,R1
    // asm: 	CMPF	R1,R2	  		;TEST TRUE RADIUS
    // asm: 	BLT	BS2			;NO GO...
    // asm: 	LDI	*+AR2(OLINK3),AR2
    // asm: 	LDI	AR2,R1
    // asm: 	BNZD	BS1
    // asm: 	SUBF	*+AR2(OPOSX),R3,R2
    // asm: 	SUBF	*+AR2(IR1),R4,R1
    // asm: 	MPYF	R1,R1
    // 	;---->	BNZ	BS1
    // asm: 	RETS
    // *CHECK OUT POINT COLLISION
BS2:
    // asm: 	PUSH	AR4
    // asm: 	LDPI	@BOXSCRAMI,AR4
    // asm: 	ADDI	18H,AR4
    // asm: 	LDI	7,AR5			;LOOP 8 BOX POINTS
    // asm: BSRDLP
    // asm: 	CALL	_coll_road		;XZ POINT COLLISION WITH ROAD OBJECT?
    // asm: 	BNC	BS10			;NOPE...
    // asm: 	CMPF	R0,R7
    // asm: 	LDFGT	R0,R7			;SAVE LOWEST POINT
BS10:
    // asm: 	NOP	*AR4++(3)     		;CHECK NEXT POINT
    // asm: 	DB	AR5,BSRDLP
    // asm: 	LDI	*+AR2(OLINK3),R0
    // asm: 	BNZD	BS0
    // asm: 	LDI	R0,AR2
    // asm: 	POP	AR4
    // asm: 	LDI	OPOSZ,IR1
    // 	;---->	BNZ	BS0
BSCX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BOXSCAN", 0, 0);
    UNIMPL();
}

void CAR_ROAD_COLL(void)
{
    // *----------------------------------------------------------------------------
    // *CHECK VEHICLE COLLISION WITH ROAD
    // *
    // *PARAMETERS
    // *	AR4	OBJECT
    // *	R3	POINTER TO CARVCT RAM AREA
    // *	RAM AREA= 3*(X,Y,Z,ROAD Y,Y VEL,COLLISION OBJECT)
    // *		ONROAD,AIRFRONT,AIRBORNE (1=TRUE)
    // *RETURNS
    // *	CARVCT AREA MODIFIED,FLAGS SET, OBJECT MATRIX ALIGNED TO ROAD
    // *	NEED TO ADD Y RADIANS AFTERWARD
    // *
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	CALL	ROADSCAN 		;GET POINT HEIGHTS
    // ****************************************************
    // *WE HAVE FOUND HEIGHT FOR ALL SUSPENSION POINTS
    // *GET NEW PLAYER MATRIX
    // asm: PC1XX
    // asm: 	LDI	*+AR6(CT_PCOL),R0  	;NO ROAD COLLISION CENTER POINT
    // asm: 	BZ	PC1X0
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	AND	CLASS_M+TYPE_M,R0
    // asm: 	STI	R0,*+AR6(CAR_ONROAD)	;XXX OID->ONROAD FLAG
PC1X0:
    // asm: 	LDI	AR6,AR0			;GET CARVCT SUSPENSION POINTS
    // asm: 	FLOATP	@NFRAMES,R2
    // asm: 	MPYF	4,R2			;FRAME ADJUSTED GRAVITY
    // ;	MPYF	8,R2			;FRAME ADJUSTED GRAVITY
    // asm: 	LDI	CARVNUM-1,RC 		;LOOP FOR ALL GROUND TOUCHERS
    // asm: 	RPTB	PC2
    // asm: 	LDF	*+AR0(CARPRDYD),R0	;LOAD DELTA HEIGHT
    // asm: 	CMPF	-9,R0
    // asm: 	BGT	PC1A			;WE ARE ABOVE ROAD
    // *BELOW ROAD CASE
    // asm: 	ADDF	-9,R0
    // asm: 	ADDF	*+AR0(CARPY),R0		;WE ARE BELOW ROAD
    // asm: 	BD	PC2
    // asm: 	STF	R0,*+AR0(CARPY)		;SET TO ROAD HEIGHT
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR0(CARPYV)	;STORE NEW VELOCITY
    // 	;-------->B	PC2
    // *ABOVE ROAD CASE
PC1A:
    // asm: 	LDF	*+AR0(CARPYV),R1	;GRAVITY ACCELERATES Y VEL
    // asm: 	ADDF	R2,R1
    // asm: 	LDI	*+AR0(CARPCOL),R4	;CHECK GRAVITY TYPE
    // asm: 	BZ	PC1B
    // asm: 	LDI	R4,AR3
    // asm: 	LDI	*+AR3(OID),R4
    // asm: 	AND	CLASS_M+TYPE_M,R4
    // asm: 	CMPI	ROAD_C+LOGRAV_T,R4
    // ;	BNE	PC1AA
    // ;	NOP
    // ;PC1AA
    // asm: 	BEQ	PC1B			;LOGRAVITY SECTION?
    // asm: 	LDF	R2,R4
    // asm: 	MPYF	4,R4			;NO, GRAV X 4
    // asm: 	ADDF	R4,R1
PC1B:
    // asm: 	CMPF	R1,R0	    		;VEL GT HEIGHT?
    // asm: 	BGT	PC2A			;NO
    // asm: 	LDF	R0,R1			;YES LIMIT VELOCITY
PC2A:
    // asm: 	STF	R1,*+AR0(CARPYV)	;ADD VELOCITY TO HEIGHT
    // asm: PC2B
    // asm: 	ADDF	*+AR0(CARPY),R1
    // asm: 	STF	R1,*+AR0(CARPY)
PC2:
    // asm: NOP 	*AR0++(CARVSIZ)
    // *SET AIRBORNE FLAGS
    // asm: 	LDI	AR6,AR0		;GET CARVCT SUSPENSION POINTS
    // asm: 	LDI	1,R0   		;ASSUME AIRBORNE
    // asm: 	LDI	1,R1
    // asm: 	LDF	*+AR0(3),R2	;LOAD DELTA HEIGHT
    // asm:       	CMPF	72,R2		;1 FOOT OFF GROUND?
    // asm: 	LDILT	0,R0		;NO
    // asm: 	LDF	*+AR0(9),R2
    // asm:       	CMPF	72,R2		;1 FOOT OFF GROUND?
    // asm: 	LDILT	0,R1		;NO
    // asm: 	LDF	*+AR0(15),R2
    // asm:       	CMPF	72,R2		;1 FOOT OFF GROUND?
    // asm: 	LDILT	0,R1		;NO
    // asm: 	STI	R0,*+AR6(CAR_AIRB)
    // asm: 	STI	R1,*+AR6(CAR_AIRF)
    // asm: 	LDI	AR6,R0		     	;get suspension points in ram
    // asm: 	LDPI	@VLI,AR0		;rotate for universe etc.
    // asm: 	STI	R0,*AR0++
    // asm: 	ADDI	CARVSIZ,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	ADDI	CARVSIZ,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	CALL	GETNMAT			;GET NEW MATRIX
    // asm: PCOLLX
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAR_ROAD_COLL", 0, 0);
    UNIMPL();
}

void ROADSCAN(void)
{
    // *----------------------------------------------------------------------------
    // *ROADSCAN - FIND COLLISION HEIGHTS FOR ALL WHEELS
    // *
    // *PARAMETERS
    // *	AR4	CAR OBJECT
    // *	R3	CAR BLOCK
    // *
    // asm: 	LDI	R3,AR6			;SAVE CARVCT RAM POINTER
    // *PROJECT CAR SUSPENSION POINTS
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2		;POINT TO CAR MATRIX
    // asm: 	LDI	AR6,AR2
    // asm: 	ADDI	CARWHLTAB,AR2
    // asm: 	LDF	*+AR4(OPOSX),R1		;GET Y OBJECT OFFSET
    // asm: 	LDF	*+AR4(OPOSY),R4		;GET Y OBJECT OFFSET
    // asm: 	LDF	*+AR4(OPOSZ),R5		;GET Z OBJECT OFFSET
    // asm: 	LDI	2,IR0
    // asm: 	LDI	CARVNUM-1,RC		;LOOP FOR ALL POINTS
    // asm: 	RPTB	LOOP
    // asm: 	LDI	R3,AR3
    // asm: 	CALL	MATRIX_MUL
    // *ADD IN X,Z OFFSETS
    // asm: 	ADDF	R1,*AR3,R0
    // asm: 	ADDF	R4,*+AR3(1),R0
    // asm: 	STF	R0,*+AR3(1)
    // ;	NEGF	R0			;DEFAULT COLLISION DELTA = - HEIGHT
    // asm: 	LDF	0,R0			;CLEAR DEFAULT HEIGHT
    // asm: 	STF	R0,*+AR3(3)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR3(5)		;CLEAR COLLISION OBJECT
    // asm: 	ADDF	R5,*+AR3(IR0),R0
    // asm: 	STF	R0,*+AR3(2)
    // asm: 	ADDI	3,AR2
LOOP:
    // asm: ADDI	6,R3
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR6(CAR_ONROAD)
    // *CHECK ROAD OBJECTS ON ROAD LIST IN RANGE
    // asm: 	LDPI	@DRIVE_LIST,R0
    // asm: 	CALL	RDSCNSUB
    // *WE HAVE SCANNED ROADLIST
    // *IF INCOMPLETE SCAN DO GROUND LIST
    // asm: 	LDI	*+AR6(CT_PCOL),R0	;CHECK COLLISION...
    // asm: 	OR	*+AR6(LF_PCOL),R0	;CHECK COLLISION...
    // asm: 	OR	*+AR6(RF_PCOL),R0	;CHECK COLLISION...
    // asm: 	OR	*+AR6(RR_PCOL),R0	;CHECK COLLISION...
    // asm: 	OR	*+AR6(LR_PCOL),R0	;CHECK COLLISION...
    // asm: 	LDINZ	1,R0
    // asm: 	STI	R0,*+AR6(CAR_ONROAD)	;ANY WHEEL ON IS ONROAD
    // asm: 	LDI	0,R1
    // asm: 	LDI	*+AR6(CT_PCOL),R0	;CHECK COLLISION...
    // asm: 	LDIZ	1,R1			;SET FLAG IF NONE
    // ;       BZ	PC3A
    // asm: 	LDI	*+AR6(LF_PCOL),R0	;CHECK COLLISION...
    // asm: 	LDIZ	2,R1
    // ;	BZ	PC3A
    // asm: 	LDI	*+AR6(RF_PCOL),R0	;CHECK COLLISION...
    // asm: 	LDIZ	3,R1
    // ;	BZ	PC3A
    // asm: 	LDI	*+AR6(RR_PCOL),R0	;CHECK COLLISION...
    // asm: 	LDIZ	4,R1
    // ;	BZ	PC3A
    // asm: 	LDI	*+AR6(LR_PCOL),R0	;CHECK COLLISION...
    // asm: 	LDIZ	5,R1
    // ;	BZ	PC3A
    // asm: 	LDI	R1,R1			;ALL WHEELS ON ROAD?
    // asm: 	BZ	PC3X			;YES...WERE DONE
    // *OFF ROAD- CHECK OUT GROUND LIST
    // asm: PC3A
    // asm: 	LDPI	@GROUND_LIST,R0
    // asm: 	CALL	RDSCNSUB
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
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROADSCAN", 0, 0);
    UNIMPL();
}

void RDSCNSUB(void)
{
    // *----------------------------------------------------------------------------
    // *ROAD SCAN SUBROUTINE
    // *
    // *PARAMETERS
    // *	R0	LIST TO SCAN
    // *	AR4	CAR OBJECT
    // *
    // asm:       	BZD	RDSCNX		  	;NULL LIST
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	OPOSZ,IR1
    // asm: 	FLOAT	*+AR4(ORAD),R0		;GET BOX RADIUS
    //       	;------>BZD	RDSCNX		;NULL LIST
    // asm: 	LDF	*+AR4(OPOSZ),R4		;GET OBJECT Z
RS0:
    // asm: 	LDF	*+AR4(OPOSX),R3		;GET OBJECT X
RS1:
    // asm: 	SUBF	*+AR2(OPOSX),R3,R2
    // asm: 	SUBF	*+AR2(IR1),R4,R1
    // asm: 	MPYF	R1,R1
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R1,R2
    // asm: 	FLOAT	*+AR2(ORAD),R1	 	;GET ROAD RADIUS
    // asm: 	ADDF	R0,R1			;ADD AND SQUARE
    // asm: 	MPYF	R1,R1
    // asm: 	CMPF	R1,R2	  		;TEST TRUE RADIUS
    // asm: 	BLT	RS2			;CHECK IT OUT !
    // asm: RS1L
    // asm: 	LDI	*+AR2(OLINK3),AR2
    // asm: 	LDI	AR2,R1
    // asm: 	BNZD	RS1
    // asm: 	SUBF	*+AR2(OPOSX),R3,R2
    // asm: 	SUBF	*+AR2(IR1),R4,R1
    // asm: 	MPYF	R1,R1
    // 	;---->	BNZ	RS1
    // asm: 	RETS
    // *CHECK OUT ROAD COLLISION
RS2:
    // asm: 	PUSH	AR4
    // asm: 	LDI	AR6,AR4
    // asm: 	LDI	CARVNUM-1,AR5		;LOOP ALL POINTS
    // asm: RS3LP
    // asm: 	LDI	*+AR4(CARPCOL),R1	;CHECK PRIOR COLLISION...
    // asm: 	BZ	RS300			;NOPE, SCAN ON...
    // asm: 	ABSF	*+AR4(CARPRDYD),R1
    // asm: 	FLOAT	2000,R2
    // asm: 	CMPF	R2,R1
    // asm: 	BLT	RS30			;ALREADY CLOSE, NO RESCAN...
    // asm: 	CALL	_coll_road		;XZ POINT COLLISION WITH ROAD OBJECT?
    // asm: 	BNC	RS30			;NOPE...
    // asm: 	LDF	*+AR4(CARPRDYD),R1
    // asm: 	BNN	RS297
    // asm:      	LDF	R0,R0
    // asm: 	BNN	RS301			;OLD=NEG, NEW=POS, GO W/ POS
    // *BOTH NEGATIVE CASE
    // asm: 	CMPF	R0,R1
    // asm: 	BGT	RS30
    // asm: 	B	RS301
    // *OLD IS POSITIVE
RS297:
    // asm: 	LDF	R0,R0
    // asm: 	BNN	RS299
    // *OLD IS POS, NEW IS NEG
    // asm: 	FLOAT	-1000,R2
    // asm: 	CMPF	R2,R0
    // asm: 	BGT	RS301
    // asm: 	B	RS30
    // *BOTH POSITIVE CASE
RS299:
    // asm: 	CMPF	R0,R1			;TAKE LEAST POSITIVE
    // asm: 	BLT	RS30
    // asm: 	B	RS301
RS300:
    // asm: 	CALL	_coll_road		;XZ POINT COLLISION WITH ROAD OBJECT?
    // asm: 	BNC	RS30			;NOPE...
RS301:
    // asm: 	STF	R0,*+AR4(CARPRDYD)		;SAVE ROAD Y DELTA
    // asm: 	STI	AR2,*+AR4(CARPCOL) 	;SAVE COLLISION OBJECT
RS30:
    // asm: 	NOP	*AR4++(CARVSIZ)
    // asm: 	DB	AR5,RS3LP
    // asm: 	POP	AR4
    // asm: RS3L
    // asm: 	LDI	*+AR2(OLINK3),AR2
    // asm: 	LDI	AR2,R0
    // asm: 	BNZD	RS0
    // asm: 	LDI	OPOSZ,IR1
    // asm: 	FLOAT	*+AR4(ORAD),R0		;GET BOX RADIUS
    // asm: 	LDF	*+AR4(OPOSZ),R4		;GET OBJECT Z
    // 	;---->	BNZ	RS0
RDSCNX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RDSCNSUB", 0, 0);
    UNIMPL();
}

void _coll_road(void)
{
    // *----------------------------------------------------------------------------
    // *coll_road(OBJECTP _obj)
    // *PARMETERS
    // *	AR2	colliding object
    // *	AR4	POINT VECTOR X,Y,Z
    // *RETURNS
    // *	R0	-HEIGHT ABOVE ROAD
    // *		CARRY SET IF ROAD PIECE COLLISION
    // *
    // asm: 	LDI	AR4,R2		;GET POINT INTO R2
    // asm: 	CALL	_obj_coll
    // asm: 	BNC	CRX		;NO COLLISION BAIL OUT WITH CARRY CLEAR
    // *
    // *WE HAVE A COLLISION
    // *FIND Y HEIGHT RETURN IN R0
    // *
    // 	;generate a (unit) normal for the actual plane
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R6
    // asm: 	PUSHF	R6
    // asm: 	PUSH	R7
    // asm: 	PUSHF	R7
    // asm: 	LDPI	@VLI,AR2		;rotate for universe etc.
    // asm: 	LDPI	@TNORMI,AR0
    // asm: 	CALL	GEN_NORMAL		;gen_normal(&A,&B,&C,&N);
    // asm: 	LDPI	@VLI,AR1
    // asm: 	LDI	*AR1,AR1
    // asm: 	MPYF	*AR1++,*AR0++,R0
    // asm: 	NEGF	*AR0,R1			;N.y
    // asm: 	MPYF	*AR1++,*AR0++,R2
    // asm: 	ADDF	R2,R0			;D = - (N.x * bufferx[vert[0]] +
    // asm: 	MPYF	*AR1++,*AR0++,R2	;       N.y * buffery[vert[0]] +
    // asm: 	ADDF	R2,R0			;       N.z * bufferz[vert[0]]);
    // asm: 	NEGF	R0			;D = ((D/(-N.y)));
    // asm: 	CALL	DIV_F30			;(R0/R1)->R0 clobbers r0,r1,ar0,ar1
    // asm: 	POPF	R7
    // asm: 	POP	R7
    // asm: 	POPF	R6
    // asm: 	POP	R6
    // asm: 	POP	AR2
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	SETC				;WE GOT ONE
CRX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_coll_road", 0, 0);
    UNIMPL();
}

void GETNMAT(void)
{
    // *----------------------------------------------------------------------------
    // *GET MATRIX TO ALIGN WITH NORMAL, Z AND X AXES ROTATION ONLY
    // *
    // *VL1=POINTER TO 3 POINTS
    // *AR4=OBJECT
    // *AR6=CARVCT RAM AREA
    // *
    // 	;*GENERATE A (UNIT) NORMAL FOR THE PLANE
    // asm: 	LDPI	@VLI,AR2		;rotate for universe etc.
    // asm: 	LDPI	@TNORMI,AR0
    // asm: 	CALL	GEN_NORMAL		;gen_normal(&A,&B,&C,&N);
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	NORMALIZE		;normalize(&N);
    // asm: 	LDPI	@TMATRIXI,AR3
    // *LOAD 2ND COLUMN OF ROTATION MATRIX (Y AXIS)
    // asm: 	LDF	*AR2,R0		     	;2ND COLUMN ROT MATRIX IS NORMAL VECTOR
    // asm: 	STF	R0,*+AR3(3)
    // asm: 	LDF	*+AR2(1),R1
    // asm: 	STF	R1,*+AR3(4)
    // asm: 	LDF	*+AR2(2),R2
    // asm: 	STF	R2,*+AR3(5)
    // *LOAD 1ST COLUMN OF ROTATION MATRIX (X AXIS)
    // asm: 	LDI	AR3,AR2
    // asm: 	STF	R1,*AR2			;X
    // asm: 	NEGF	R0			;-N2
    // asm: 	STF	R0,*+AR2(1)		;Y
    // asm: 	CLRF	R1
    // asm: 	STF	R1,*+AR2(2)		;Z
    // asm: 	CALL	NORMALIZE		;normalize(&N);
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
    // asm: 	ADDI	3,AR2			;POINT AR2 TO THIRD COLUMN OF MATRIX
    // asm: 	LDI	2,IR0
    // asm: 	MPYF	*+AR3(1),*+AR2(IR0),R0	;U2*V3
    // asm: 	MPYF	*+AR3(IR0),*+AR2(1),R1	;U3*V2
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR3(6)
    // asm: 	MPYF	*+AR3(IR0),*AR2,R0	;U3*V1
    // asm: 	MPYF	*AR3,*+AR2(IR0),R1	;U1*V3
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR3(7)
    // asm: 	MPYF	*AR3,*+AR2(1),R0	;U1*V2
    // asm: 	MPYF	*+AR3(1),*AR2,R1	;U2*V1
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR3(8)
    // asm: 	ADDI	3,AR2
    // asm: 	CALL	NORMALIZE		;normalize(&N);
    // *INVERT MATRIX AND STORE IN OBJECT
    // asm: 	LDI	AR3,R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	CPYIMAT     		;invert matrix and stuff in object
    // ***GET CAR HEIGHT AND LOAD IT INTO CAR
    // asm: 	LDF	*+AR6(1),R0		;GET Y HEIGHT FIRST POINT
    // asm: 	SUBF	*+AR6(CARWHLTAB+1),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETNMAT", 0, 0);
    UNIMPL();
}

void _obj_coll(void)
{
    // *----------------------------------------------------------------------------
    // *CHECK OBJECT COLLISION WITH VECTOR IN X/Z SPACE
    // *
    // *PARAMETERS
    // *	AR2	OBJECT POINTER
    // *	R2	VECTOR POINTER
    // *
    // *RETURNS
    // *	CS=collision, CC= No collision
    // *	VL contains 3 pointers which point to VECTORs which
    // *	define the normal to this polygon where the 1st collision has
    // *	been found.
    // *
    // *TRASHES
    // *	R0,R1,R2,R3
    // *
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	LDI	*+AR2(OROMDATA),AR4
    // asm: 	ADDI	1,AR4			;skip object diameter
    // asm: 	LDI	AR2,AR5
    // asm: 	ADDI	OMATRIX,AR5		;rotational matrix
    // asm: 	LDPI	@BLOWLISTI,AR3		;blowlist pointer = AR3
    // asm: 	LDI	@transvectorYI,AR6	;transvector temp store
    // asm: 	LDI	@tmpmatY,AR7		;TEMP VECTOR STORE
    // asm: 	LDI	8,IR0
    // asm: 	LDI	R2,AR1			;create translation (TRANS = OBJPOS - COLLPOS)
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	SUBF	*AR1++,R0
    // asm: 	STF	R0,*-AR6(1)		;transvector.x
    // asm: 	LDF	*+AR2(OPOSY),R0
    // asm: 	SUBF	*AR1++,R0
    // asm: 	STF	R0,*AR6			;transvector.y
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	SUBF	*AR1,R0
    // asm: 	STF	R0,*+AR6(1)		;transvector.z
    // asm: 	LDI	*AR4++,RC
    // asm: 	LDI	RC,BK
    // asm: 	AND	0FFh,RC
    // asm: 	RPTB	EOTV
    // asm: 	LDI	*AR4++,R4
    // asm: 	LDI	R4,R3
    // asm: 	ASH	-16,R4
    // asm: 	LS	16,R3
    // asm: 	ASH	-16,R3
    // asm: 	FLOAT	R3
    // asm: 	FLOAT	R4
    // asm:  	STF	R3,*-AR7(1)
    // asm: 	FLOAT	*AR4++,R2		;get z element of source 1
    // *
    // *MULTIPLY BY ROTATION MATRIX
    // *AND ADD TRANSLATION (IN THAT ORDER)
    // *
    // asm: 	MPYF3	*AR5++,R3,R0
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5++,*+AR7(1),R1
    // asm: 	MPYF3	*AR5++,*-AR7(1),R0
    // asm: 	ADDF	*-AR6(1),R2		;*blowlist++   += translation[X]
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5++,*+AR7(1),R1
    // asm: 	MPYF3	*AR5++,*-AR7(1),R0
    // asm: 	ADDF	*AR6,R3			;*blowlist++   += translation[Y]
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5--(IR0),*+AR7(1),R1
    // asm: 	ADDF	R1,R2			;FORM ROTATED Z
    // asm: 	ADDF	*+AR6(1),R2		;ADD IN TRANSLATION Z
EOTV:
    // asm: STF	R2,*AR3++
    // asm: 	LDPI	@BLOWLISTI,IR1		;blowlist pointer = IR1
    // asm: 	LDI	IR1,IR0
    // asm: 	ADDI	2,IR0
    // asm: 	LDI	0FFh,R4
    // asm: 	LDI	-8,R5
    // ;	BD	VLINST
    // asm: 	INC	AR4			;skip control of 1st polygon
    // asm: 	LDI	BK,RC
    // asm: 	RS	16,RC
    // 	;----->BD	VLINST
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_obj_coll", 0, 0);
    UNIMPL();
}

void _makbox(void)
{
    // *----------------------------------------------------------------------------
    // *MAKBOX	GET XYZPLUS-MINUS VALUES FOR CAR
    // *       MAKE WHEEL OFFSET TABLE
    // *PARAMETERS
    // *	AR4	OBJECT WITH OCARBLK
    // *RETURNS
    // *	GETS CARXYZPLUS/MINUS IN OCARBLK
    // *	     CARWHLTAB IN OCARBLK
    // *TRASHES
    // *	R0-R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	R6
    // asm: 	PUSHF	R6
    // asm: 	PUSH 	AR0
    // asm: 	LDI	*+AR4(OROMDATA),AR0
    // asm: 	ADDI	1,AR0			;skip object diameter
    // asm: 	LDI	*AR0++,RC     		;GET VERTEX COUNT
    // asm: 	AND	0FFh,RC
    // asm: 	SUBI	1,RC
    // asm: 	LDI	*AR0++,R3		;GET Y:X
    // asm: 	LDI	R3,R4
    // asm: 	LS	16,R3
    // asm: 	ASH	-16,R3			;UNPACK X  X1->R3 OXPLUS
    // asm: 	ASH	-16,R4			;UNPACK Y  Y1->R4 OYPLUS
    // asm: 	FLOAT	R3
    // asm: 	FLOAT	R4
    // asm: 	FLOAT	*AR0++,R5		;Z1->R5 OZPLUS
    // asm: 	LDF	R3,R0			;X1->R0 OXMINUS
    // asm: 	LDF	R4,R1			;Y1->R1 OYMINUS
    // asm: 	LDF	R5,R2			;Z1->R2 OZMINUS
    // asm: 	RPTB	MBVL
    // asm: 	LDI	*AR0,R6			;GET Y:X
    // asm: 	LS	16,R6
    // asm: 	ASH	-16,R6			;UNPACK X
    // asm: 	FLOAT	R6
    // asm: 	CMPF	R3,R6			;CHECK X(N) > XPLUS
    // asm: 	LDFGT	R6,R3
    // asm: 	CMPF	R0,R6			;CHECK X(N) < XMINUS
    // asm: 	LDFLT	R6,R0
    // asm: 	LDI	*AR0++,R6		;GET Y:X
    // asm: 	ASH	-16,R6			;UNPACK Y
    // asm: 	FLOAT	R6
    // asm: 	CMPF	R4,R6			;CHECK Y(N) > YPLUS
    // asm: 	LDFGT	R6,R4
    // asm: 	CMPF	R1,R6			;CHECK Y(N) < YMINUS
    // asm: 	LDFLT	R6,R1
    // asm: 	FLOAT	*AR0++,R6		;XN
    // asm: 	CMPF	R5,R6			;CHECK Z(N) > ZPLUS
    // asm: 	LDFGT	R6,R5
    // asm: 	CMPF	R2,R6			;CHECK Z(N) < ZMINUS
MBVL:
    // asm: LDFLT	R6,R2
    // asm: 	LDI	*+AR4(OCARBLK),AR0
    // *STORE WHEEL OFFSET TABLE
    // asm: 	LDF	0,R6
    // asm: 	STF	R6,*+AR0(CARWHLTAB+0) 		;CENTER POINT BOTTOM
    // asm: 	STF	R4,*+AR0(CARWHLTAB+1)
    // asm: 	STF	R6,*+AR0(CARWHLTAB+2)
    // asm: 	STF	R3,*+AR0(CARWHLTAB+3)		;RT FRONT BOTTOM
    // asm: 	STF	R4,*+AR0(CARWHLTAB+4)
    // asm: 	STF	R5,*+AR0(CARWHLTAB+5)
    // asm: 	STF	R0,*+AR0(CARWHLTAB+6)		;LFT FRONT BOTTOM
    // asm: 	STF	R4,*+AR0(CARWHLTAB+7)
    // asm: 	STF	R5,*+AR0(CARWHLTAB+8)
    // asm: 	STF	R0,*+AR0(CARWHLTAB+9)	  	;LFT REAR BOTTOM
    // asm: 	STF	R4,*+AR0(CARWHLTAB+10)
    // asm: 	STF	R2,*+AR0(CARWHLTAB+11)
    // asm: 	STF	R3,*+AR0(CARWHLTAB+12)		;RT REAR BOTTOM
    // asm: 	STF	R4,*+AR0(CARWHLTAB+13)
    // asm: 	STF	R2,*+AR0(CARWHLTAB+14)
    // *STORE XYZ PLUS/MINUS
    // asm: 	ADDF	25.0,R0			;MAKE IT A LITTLE SMALLER
    // asm: 	STF	R0,*+AR0(CARXMINUS)
    // asm: 	STF	R1,*+AR0(CARYMINUS)
    // ;	ADDF	15.0,R2			;MAKE IT A LITTLE SMALLER
    // asm: 	STF	R2,*+AR0(CARZMINUS)
    // asm: 	ADDF	-25.0,R3		;MAKE IT A LITTLE SMALLER
    // asm: 	STF	R3,*+AR0(CARXPLUS)
    // asm: 	STF	R4,*+AR0(CARYPLUS)
    // ;	ADDF	-15.0,R5		;MAKE IT A LITTLE SMALLER
    // asm: 	STF	R5,*+AR0(CARZPLUS)
    // asm: 	POP	AR0
    // asm: 	POPF	R6
    // asm: 	POP	R6
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_makbox", 0, 0);
    UNIMPL();
}

void COLSCC(void)
{
    // *----------------------------------------------------------------------------
    // *COLLISION SCAN
    // *
    // asm: 	LDPI	@_MODE,R0	   	;MAKE SURE MODE IS IN GAME
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0		;cm
    // asm: 	BEQ	ATTR_COLLISION		;cm
    // asm: 	CMPI	MGAME,R0
    // asm: 	BNE	COLSCCX
    // asm: 	LDI	@PACTIVEI,AR7	      	;ROOT PROCESS
    // *************TEST CODE
    // ;	CALL 	CKRAD
    // ****************TEST CODE
    // asm: 	CALL	COLSCAN			;PLAYER VS. DRONES
    // asm: 	CALL	CLDSCAN			;DRONES VS. DRONES
    // asm:  	CALL	PLYRSIGN		;PLAYER VS. SIGNS, POLES, TREES
    // asm: 	CALL	PLYRDEBRIS		;PLAYER VS. ROAD DEBRIS
    // asm:  	CALL	DRONSIGN		;DRONES VS. SIGNS, POLES, TREES
    // asm: 	CALL	DRONDEBRIS		;DRONES VS. ROAD DEBRIS
    // asm: 	CALL	PLYRROADKILL		;PLAYER VS. ROADKILL
    // asm: 	CALL	DEBSCAN			;CLEAR OUT DEAD DEBRIS
COLSCCX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLSCC", 0, 0);
    UNIMPL();
}

void PLYR_VS_DEBRIS(void)
{
    // *----------------------------------------------------------------------------
    // *PLAYER COLLIDE WITH DEBRIS
    // *
    // asm: PLYRDEBRIS
    // asm: 	LDPI	@_plyr1+PLY_CAR,AR0	;GET PLAYER CAR
    // asm: 	LDPI	@ROAD_DEBRISI,AR1
    // asm: 	B	COLPOINT
    // *
    // *PLAYER COLLIDE WITH SIGNS
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_VS_DEBRIS", 0, 0);
    UNIMPL();
}

void PLYR_VS_SIGN(void)
{
    // asm: PLYRSIGN
    // asm: 	LDPI	@_plyr1+PLY_CAR,AR0	;GET PLAYER CAR
    // asm: 	LDPI	@SIGN_LISTI,AR1
    // asm: 	B	COLPOINT
    // *
    // *DRONE COLLIDE WITH DEBRIS
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_VS_SIGN", 0, 0);
    UNIMPL();
}

void DRONE_VS_DEBRIS(void)
{
    // asm: DRONDEBRIS
    // asm: 	LDPI	@ROAD_DEBRISI,AR1
    // asm: 	B	DRONEPT
    // *
    // *DRONE COLLIDE WITH SIGNS
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_VS_DEBRIS", 0, 0);
    UNIMPL();
}

void DRONE_VS_SIGN(void)
{
    // asm: DRONSIGN
    // asm: 	LDPI	@SIGN_LISTI,AR1
DRONEPT:
    // asm: 	LDPI	@CAR_LIST,R0	 	;GET LIST AND CHECK NULL
    // asm: 	LDI	R0,AR0
    // asm: 	RETSZ
DRONEPTL:
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	CMPI	DRONE_C|HELICOPTER,R0
    // asm: 	BEQ	DRONEPT1
    // asm: 	LDI	*+AR0(ODIST),R1	  	;IF DUDE OFFSCREEN, IGNORE
    // asm: 	BN	DRONEPT1
    // asm: 	LDI	25000,R2       		;CHECK IF FAR DISTANT
    // asm: 	MPYI	2,R2
    // asm: 	CMPI	R2,R1
    // asm: 	BGT	DRONEPT1		;IF FAR IN DISTANCE, IGNORE
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR0
    // asm: 	CALL	COLPOINT	       	;CHECK CAR/POINT COLLISION
    // asm: 	POP	AR0
    // asm: 	POP	AR1
DRONEPT1:
    // asm: 	LDI	*+AR0(OLINK3),R0
    // asm: 	LDI	R0,AR0
    // asm: 	BNZ	DRONEPTL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_VS_SIGN", 0, 0);
    UNIMPL();
}

void COLPOINT(void)
{
    // *----------------------------------------------------------------------------
    // *SIGN/TREE COLLISION SCAN WITH CAR
    // *
    // *CHECK PLAYER CAR AGAINST LIST
    // *PARAMETERS
    // *	AR0	CAR OBJECT
    // *	AR1	ADDRESS OF LIST HEADER
    // *
    // *DO NOT MODIFY THE BEHAVIOR THAT HAS BEEN DEFINED HERE:
    // *	  only the TYPE is used to determine behavior
    // *	  object is NOT pulled from any list (SIGN or OTHERWISE)
    // *
    // *
    // asm: 	BD	CARSCL0			;GET FIRST GUY
    // asm: 	LDF	*+AR0(OPOSX),R2		;GET X COORD
    // asm: 	LDF	*+AR0(OPOSZ),R3		;GET Z COORD
    // asm: 	SUBI	OLINK3,AR1		;SETUP INDEXING
    // 	;------->BU CARSCL0
CARSCLP0:
    // asm: 	MPYF	R4,R4
    // asm: 	ADDF	R0,R4
    // asm: 	CMPF	R5,R4	 		;ARE WE WITHIN RADIUS?
    // asm: 	BGT	CARSCL			;NO, KEEP GOING
    // asm: 	LDI	*+AR1(OFLAGS),R0
    // asm: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // asm: 	BNZ	CARSCL
    // asm: 	CALL 	COLSGCK
    // asm: 	LDF	*+AR0(OPOSX),R2		;GET X COORD
    // asm: 	LDF	*+AR0(OPOSZ),R3		;GET Z COORD
CARSCL0:
    // asm: 	FLOAT	*+AR0(ORAD),R5	 	;GET CAR RADIUS
    // asm: 	LDI	OPOSZ,IR0
    // asm: 	MPYF	R5,R5
CARSCL:
    // asm: 	LDI	*+AR1(OLINK3),AR1
    // asm: 	LDI	AR1,R0
    // asm: 	BNZD	CARSCLP0
    // asm: 	SUBF	*+AR1(OPOSX),R2,R0
    // asm: 	MPYF	R0,R0
    // asm: 	SUBF	*+AR1(IR0),R3,R4
    // ********BNZD	CARSCLP0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLPOINT", 0, 0);
    UNIMPL();
}

void COLSGCK(void)
{
    // *----------------------------------------------------------------------------
    // *CHECK POINT VERSUS XZ BOX
    // *
    // *PARAMETERS
    // *	AR0	POINTS TO CAR OBJECT
    // *	AR1	POINTS TO SIGN/POLE OBJECT
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDPI	@BLOWLISTI,AR2
    // asm: 	CALL	GETBOX			;GET BOX POINTS FOR OBJECT 1
    // *
    // *CHECK 4 LINE EQUATIONS FOR BOTTOM OF CAR
    // *PT 2-6-7-3
    // asm: 	LDPI	@LEQTABI,AR2
    // asm: 	LDI	3,RC 			;DO 4 EQUATIONS
    // asm: 	RPTB	CSGLNEQ
    // asm: 	LDI	*AR2++,AR3		;GET 2 POINTS
    // asm: 	LDI	*AR2,AR4
    // asm: 	SUBF	*+AR3(1),*+AR4(1),R0	;A
    // asm: 	SUBF	*-AR4(1),*-AR3(1),R1	;B
    // asm: 	MPYF	R0,*-AR3(1),R2
    // asm: 	MPYF	R1,*+AR3(1),R3
    // asm: 	ADDF	R3,R2
    // asm: 	NEGF	R2			;C
    // *EVALUATE THE POINT
    // asm: 	MPYF	*+AR1(OPOSX),R0		;AX
    // asm: 	MPYF	*+AR1(OPOSZ),R1		;BZ
    // asm: 	ADDF	R0,R1
    // asm: 	ADDF	R1,R2
    // asm: 	BLE	COLSGCX
CSGLNEQ:
    // asm: NOP
    // *
    // *GOT A COLLISION
    // *CHECK TYPE
    // *
    // asm: 	LDI	*+AR0(OCARBLK),AR5	;GET VELOCITY DIRECTION
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	TYPE_M,R0
    // asm: 	CMPI	TSC_IGNORE,R0
    // asm: 	BEQ	SIGN_IGNORE
    // ;herein lies the start of our bugs...
    // ;
    // ;
    // ;
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	CLASS_M|TYPE_M,R0
    // asm: 	CMPI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm: 	BEQ	ROADKILL
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	TYPE_M,R0
    // asm: 	CMPI	TSC_FLYING,R0
    // asm: 	BEQ	FLYCOLL
    // asm: 	CMPI	TSC_RUNOVER,R0
    // asm: 	BEQ	RUNOVER
    // *
    // *IMMOBILE SIGN
    // *
    // *PARAMETERS
    // *	AR0	POINTS TO CAR OBJECT
    // *	AR1	POINTS TO SIGN/POLE OBJECT
    // *	AR5	CARBLOCK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLSGCK", 0, 0);
    UNIMPL();
}

void HARDCOL(void)
{
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	CLASS_M|TYPE_M|SUBTYPE_M,R0
    // asm: 	CMPI	TSIGN_C|TSC_IMMOBILE|TSC_V_PALM,R0
    // asm: 	BNE	NOTCOCONUT
    // asm: 	PUSH	R0
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR5
    // asm: 	LDI	AR0,AR5		;parent object
    // asm: 	RANDN	3
    // asm: 	LDI	R0,AR3
    // asm: LL88
    // asm: 	CREATE	DROP_COCONUTS,TSIGN_C|TSC_IMMOBILE|TSC_V_PALM
    // asm: 	DBU	AR3,LL88
    // asm: 	POP	AR5
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R2
    // asm: 	POP	R0
    // asm: 	BU	DOREPEL
NOTCOCONUT:
    // asm: 	AND	TYPE_M,R0		;REDWOODS MUST NOT GET KNOCKED OVER
    // asm: 	CMPI	TSC_HARD,R0
    // asm: 	BNE	RUNOVER
    // *REPELL THE SUCKERS
DOREPEL:
    // asm: 	CALL	REPELL
    // asm: 	MPYF	*AR2,R0,R1 			;X REPELL
    // asm: 	MPYF	*+AR2(2),R0		     	;Z REPELL
    // asm: 	ADDF	*+AR0(OPOSX),R1			;REPELL THE SUCKER (AR0)
    // asm: 	ADDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R1,*+AR0(OPOSX)
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDF	*+AR5(CARSPEED),R2    		;GOING FAST???
    // asm: 	MPYF	0.6,R2
    // asm: 	CMPF	37,R2				;MINIMUM SPEED VALUE
    // asm: 	LDFLT	37,R2
    // asm: 	STF	R2,*+AR5(CARSPEED)		;REVERSE SPEED
    // asm: 	BLT	HARDCOL00			;YES,SPINOUT
    // asm: 	LDI	500,AR2				;STRAIGHT OR SPINNER?
    // asm: 	CALL	RANDPER
    // asm: 	BC	HARDCOL1			;SPINNER...
    // *STRAIGHT KICKBACK
HARDCOL00:
    // asm: 	LDI	60,R0				;SPIN COUNT
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	GETNXTRDIR
    // asm: 	LDI	AR4,AR0
    // asm: 	LDF	R0,R2
    // asm: 	SUBF	*+AR5(CARYROT),R2
    // asm: 	CALL	NORMITS
    // asm: 	LDF	R2,R2
    // asm: 	LDFGT	0.02,R0
    // asm: 	LDFLE	-0.02,R0
    // asm: 	B	HARDCOL2			;STORE DROT, SET VROT
    // *SPIN THE DUDE
HARDCOL1:
    // asm: 	LDI	1,R0				;SPIN THE DUDE
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
    // asm: 	LDF	3.14,R1				;SET 180 MIN SPIN
    // asm: 	STF	R1,*+AR5(CARSPRAD)
    // asm: 	LDF	0.1,R0		 		;GET ROTATION AMOUNT
HARDCOL2:
    // asm: 	STF	R0,*+AR5(CARDROT)
    // asm: 	LDPI	@VECTORAI,AR2    		;COMPUTE REPULSION VECTOR
    // asm: 	LDF	*AR2,R2
    // asm: 	LDF	*+AR2(2),R3
    // asm: 	CALL	ARCTANF
    // asm: 	SUBF	1.57,R0
    // asm: 	SUBF	*+AR5(CARVROT),R0
    // asm: 	LDF	R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2
    // asm: 	CMPF	1.57,R2
    // asm: 	BLT	HARDCOL3
    // asm: 	LDF	*+AR5(CARVROT),R2		;REVERSE VELOCITY
    // asm: 	LDF	R2,R3
    // asm: 	ADDF	3.14,R2
    // asm: 	CALL	NORMITS
    // asm: 	STF	R2,*+AR5(CARVROT)
    // *SHAKE THE TREE
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	AND	TYPE_M,R0
    // asm: 	CMPI	TSC_HARD,R0
    // asm: 	BEQ	HARDCOL3
    // asm: 	LDF	R3,R2
    // asm: 	LDI	AR1,AR2		 	;FORM OMATRIX POINTER
    // asm: 	ADDI	OMATRIX,AR2		;STUFF
    // asm: 	CALL    FIND_YMATRIX		;NEW MATRIX
    // asm: 	LDI	AR1,AR4			;GET SIGN OBJECT POINTER
    // asm: 	LDI	*+AR4(OFLAGS),R0	;MAKE IT SELF ROTATING
    // asm: 	ANDN	O_POSTER,R0
    // asm: 	LDI	1,R1			;SET 3D ROTATION BIT
    // asm: 	LS	O_3DROT_B,R1
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDF	*+AR5(CARSPEED),R7	;HIT CAR SPEED
    // asm: 	MPYF	0.04,R7		   	;FALL RATE BASED UPON VELOCITY
    // asm: 	CMPF	0.13,R7
    // asm: 	LDFLT	0.13,R7
    // asm: 	CMPF	1.0,R7
    // asm: 	LDFGT	1.0,R7
    // asm: 	LDPI	@TREESHAKI,AR2		;GET SIGN FALL PROCESS
    // asm: 	LDI	DRONE_C|FLYER_T,R2
    // asm: 	CALL	PRC_CREATE_CHILD		;CREATE A CHILD PROCESS
HARDCOL3:
    // asm: 	SONDFX	POLESND			;MAKE SOUND
    // asm: 	B	COLSGCX
    // *
    // *ROADKILL OBJECT
    // *	AR0	POINTS TO CAR OBJECT
    // *	AR1	POINTS TO ROADKILL OBJECT HIT
    // *
ROADKILL:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR1(OCARBLK)
    // asm: 	CALL	ROADKILL_HIT		;MAKE A SOUND
    // asm: 	CALL	ROADKILL_FLYERP
    // asm: 	BC	FLYCOLL
    // asm: 	B	COLSGCX
    // *
    // *FLYING OBJECT
    // *	AR0	POINTS TO CAR OBJECT
    // *	AR1	POINTS TO OBJECT HIT
FLYCOLL:
    // asm: 	LDF	*+AR0(OPOSY),R0		;MAKE SURE HEIGHT IS CLOSE
    // asm: 	SUBF	*+AR1(OPOSY),R0
    // asm: 	ABSF	R0
    // asm: 	FLOAT	250,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	COLSGCX
    // asm: 	LDF	0.10,R0 		;ADD RANDOM ROTATION
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R2
    // asm: 	ADDF	*+AR5(CARVROT),R2
    // asm: 	LDF	0.65,R0			;RANDOM SPEED MULTIPLIER
    // asm: 	CALL	FRAND
    // asm: 	LDF	R0,R1
    // asm: 	ADDF	0.8,R1
    // asm: 	MPYF	1.5,R1			;SPEEDFUDGE FACTOR
    // asm: 	CALL	_SINE
    // asm: 	NEGF	R0,R3
    // asm: 	CALL	_COSI
    // asm: 	MPYF	*+AR5(CARSPEED),R1	;GET CURRENT SPEED
    // asm: 	MPYF	R1,R3
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R3,*+AR1(OVELX)	       	;SETUP VELOCITIES
    // asm: 	STF	R0,*+AR1(OVELZ)
    // asm: 	LDF	-0.3,R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	-0.2,R0
    // asm: 	MPYF	1.5,R0			;SPEEDFUDGE FACTOR
    // asm: 	MPYF	*+AR5(CARSPEED),R0	;GET CURRENT SPEED
    // asm: 	CMPF	-65,R0
    // asm: 	LDFLT	-65,R0		  	;MAX VERTICAL VELOCITY
    // asm: 	STF	R0,*+AR1(OVELY)		;STUFF VERTICAL VELOCITY
    // asm: 	LDI	AR1,AR4			;GET SIGN OBJECT POINTER
    // asm: 	LDI	AR0,AR3			;SAVE CAR OBJECT
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	TSTB	*+AR4(OFLAGS),R0	;PROCESS ALREADY ACTIVE	?
    // asm: 	LDINZ	*+AR4(OPLINK),AR2	;YES, KILL HIM OFF...
    // asm: 	BZ	CLLL1
    // asm: 	CALL	PRC_KILL		;DONT FUCK WITH THIS PRIBYL!!!!
CLLL1:
    // asm: 	LDPI	@FLYCOLLPI,AR2		;GET SIGN FLY PROCESS
    // asm: 	LDI	DRONE_C|FLYER_T,R2
    // asm: 	CALL	PRC_CREATE_CHILD		;CREATE A CHILD PROCESS
    // asm: 	BC	COLSGCX			;NOTHING AVAILABLE, QUIT
    // asm: 	STI	AR0,*+AR4(OPLINK)	;SAVE PROCESS LINK
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_3DROT_B,R0		;FLAG CAN AS NON-2D OPTIMIZABLE
    // asm: 	LDI	1,R1
    // asm: 	LSH	O_PROC_B,R1		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	ADDI	R1,R0
    // asm: 	OR	*+AR4(OFLAGS),R0	;SET YOUR FLAGS...
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	0,R0			;KILL OFF GROUP REFERENCE
    // asm: 	STI	R0,*+AR4(OLINK2)
    // *GET SOUND FOR SAWHORSE/DRUM HIT
    // asm: 	LDI	*+AR4(OID),R2
    // asm: 	LDI	R2,R0
    // asm: 	AND	TYPE_M,R0
    // asm: 	CMPI	TSC_ROADKILL,R0
    // asm: 	BNE	KLFD
    // asm: 	CALL	ROADKILL_SETKILL
    // asm: 	BU	COLSGCX
KLFD:
    // asm: 	AND	SUBTYPE_M,R2
    // asm: 	CMPI	@PLYCAR,AR3		;PLAYERS CAR?
    // asm: 	BNZ	FLYCOLL1		;NO...
    // asm: 	CMPI	RDD_55GAL,R2
    // asm: 	LDIEQ	DRUMSND,AR2
    // asm: 	LDINE	SIGNSND,AR2
    // asm: 	CALL	ONESNDFX
    // asm: 	B	COLSGCX
FLYCOLL1:
    // asm: 	CMPI	RDD_55GAL,R2
    // asm: 	LDIEQ	DRMBNCE,AR2
    // asm: 	LDINE	DSIGNSND,AR2
    // asm: 	B	COLSGCX0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HARDCOL", 0, 0);
    UNIMPL();
}

void RUNOVER(void)
{
    // *----------------------------------------------------------------------------
    // *RUNOVER SIGN
    // *REMOVE SIGN FROM SIGN SUP LIST
    // *START SIGN PROCESS TO MAKE IT FALL
    // *
    // *PARAMETERS
    // *	AR0	POINTS TO CAR OBJECT
    // *	AR1	POINTS TO SIGN/POLE OBJECT
    // *	AR5	CARBLOCK
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	LDI	AR1,AR2		 	;FORM OMATRIX POINTER
    // asm: 	ADDI	OMATRIX,AR2		;STUFF
    // asm: 	CALL    FIND_YMATRIX		;NEW MATRIX
    // asm: 	LDI	AR1,AR4			;GET SIGN OBJECT POINTER
    // asm: 	LDI	AR0,AR3			;SAVE CAR OBJECT
    // asm: 	LDI	*+AR4(OFLAGS),R0	;MAKE IT SELF ROTATING
    // asm: 	ANDN	O_POSTER,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDF	*+AR5(CARSPEED),R7	;HIT CAR SPEED
    // asm: 	LDF	R7,R6
    // asm: 	MPYF	0.2,R6
    // asm: 	CMPF	10,R6
    // asm: 	LDFLT	10,R6
    // asm: 	CMPF	R6,R7
    // asm: 	LDFLT	R7,R6
    // asm: 	LDF	R7,R5
    // asm: 	SUBF	R6,R5
    // asm: 	STF	R5,*+AR5(CARSPEED)
    // asm: 	MPYF	0.03,R7		   	;FALL RATE BASED UPON VELOCITY
    // asm: 	CMPF	0.1,R7
    // asm: 	LDFLT	0.1,R7
    // asm: 	CMPF	0.7,R7
    // asm: 	LDFGT	0.7,R7
    // asm: 	CALL	FREESIGN		;GET SIGN OFF LIST
    // asm: 	LDPI	@SIGNFALLI,AR2		;GET SIGN FALL PROCESS
    // asm: 	LDI	DRONE_C|FLYER_T,R2
    // asm: 	CALL	PRC_CREATE_CHILD		;CREATE A CHILD PROCESS
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	AND	SUBTYPE_M,R0
    // asm: 	LDI	SIGNSND,AR2    		;DEFAULT SOUND
    // asm: 	CMPI	TSC_R_SAGE,R0
    // asm: 	BNZ	RUNOV0
    // asm: 	LDI	5,AR2
    // asm: 	CALL	RANDU0
    // asm: 	ADDI	@SAGETABI,R0
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	B	RUNOV00
RUNOV0:
    // asm: 	CMPI	TSC_R_POLE,R0
    // asm: 	LDIZ	DONGSND,AR2
    // asm: 	CMPI	TSC_R_LAMPPOST,R0
    // asm: 	LDIZ	LAMPSND,AR2
RUNOV00:
    // asm: 	CMPI	@PLYCAR,AR3		;PLAYERS CAR?
    // asm: 	BNZ	COLSGCX0		;NO...
    // asm: 	CALL	ONESNDFX
    // asm: 	B	COLSGCX
COLSGCX0:
    // asm: 	CALL	DRONESND1
SIGN_IGNORE:
COLSGCX:
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RUNOVER", 0, 0);
    UNIMPL();
}

void FLYCOLLP(void)
{
    // asm: 	LDF	0.2,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	PDATA+2,AR2		;STORE MATRIX PDATA+2
    // asm: 	CALL    FIND_XMATRIX
    // asm: 	LDPI	@MATRIXAI,AR2
    // asm: 	LDF	0.1,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R2
    // asm: 	CALL    FIND_YMATRIX
    // asm: 	LDI	AR7,R2	  		;ROTATE THE SUCKER
    // asm: 	ADDI	PDATA+2,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMAT
FLYCOLP0:
    // ;	LDI	*+AR4(OFLAGS),R0	;CHECK IF OBJECT ACTIVE, EXISTS
    // ;	AND	O_LIST_M,R0
    // ;	CMPI	O_LIST0,R0
    // ;	BZ	FLYSTOP  		;ERROR DOES NOT EXIST
    // ;	CMPI	O_LIST2,R0		;OBJECT NOT ACTIVE, SPLIT
    // ;	BZ	FLYSTOP
    // asm: 	LDPI	@NFRAMES,AR6	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm: 	SUBI	1,AR6
    // asm: FLYCOLPL
    // asm: 	LDI	AR7,R2	  		;ROTATE THE SUCKER
    // asm: 	ADDI	PDATA+2,R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	DBU	AR6,FLYCOLPL
    // asm: 	CALL	OVELNADD		;UPDATE VELOCITIES
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR4(OUSR1)		;INDICATE IN MOTION, RE-SORT
    // asm: 	FLOATP	@NFRAMES,R2
    // asm: 	MPYF	2,R2			;FRAME ADJUSTED GRAVITY
    // asm: 	ADDF	*+AR4(OVELY),R2
    // asm: 	STF	R2,*+AR4(OVELY)
    // asm: 	CMPF	100,R2
    // asm: 	BGT	FLYSTOP
    // asm: 	CALL	OBJSCAN
    // asm: 	BNC	FLYCSLP			;OFF THE MAP
    // *WERE OVER THE ROAD
    // asm: FLYROAD
    // asm: 	FLOAT	155,R1 			;HT OF DRUM/SAWHORSE
    // asm: 	LDI	*+AR4(OID),R2		;ROADKILL DOESN'T BOUNCE
    // asm: 	CMPI	RDDEBRIS_C|TSC_ROADKILL,R2
    // asm: 	LDFEQ	35,R1
    // asm:  	CMPF	R1,R0
    // asm: 	BGT	FLYCSLP			;WERE ABOVE GROUND
    // *WE HIT THE GROUND DUDES
    // asm: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // asm: 	BN	FLYCSLP			;WERE GOING UP IGNORE IT
    // asm: 	SUBRF	R1,R0			;SET HIM ON THE GROUND
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	CMPF	20,R2          		;CHECK FOR MINIMUM
    // asm: 	BLT	FLYSTOP			;TIME TO STOP
    // asm: 	MPYF	-0.5,R2
    // asm: 	STF	R2,*+AR4(OVELY)
    // asm: 	LDF	*+AR4(OVELX),R0		;CUT DOWN VELOCITIES
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	LDF	*+AR4(OVELZ),R1		;CUT DOWN VELOCITIES
    // asm: 	MPYF	0.5,R1
    // asm: 	STF	R1,*+AR4(OVELZ)
    // *MAKE BOUNCE SOUND
    // asm: 	FLOAT	*+AR4(ODIST),R0
    // asm: 	BN	FLYCSLP			;BEHIND PLAYER NO SOUND
    // asm: 	CALL	INV_F30
    // asm: 	FLOAT	5000,R1
    // asm: 	MPYF	R1,R0
    // asm: 	ABSF	*+AR4(OVELY),R2	       	;SOUND PROPORTIONAL TO VERT VELOCITY
    // asm: 	CMPF	30,R2
    // asm: 	LDFGT	30,R2
    // asm: 	MPYF	5.0,R2
    // asm: 	MPYF	R2,R0
    // asm: 	FIX	R0
    // asm: 	CMPI	140,R0
    // asm: 	LDIGT	140,R0
    // asm: 	LDI	*+AR4(OID),R1
    // asm: 	AND	SUBTYPE_M,R1
    // asm: 	CMPI	RDD_55GAL,R1
    // asm: 	LDIEQ	DRMBNCE,AR2
    // asm: 	LDINE	SAWBNCE,AR2
    // asm: 	CALL	VOLSNDFX
FLYCSLP:
    // asm: 	SLEEP	1
    // asm: 	B	FLYCOLP0
FLYSTOP:
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_PROC_B,R0		;CLEAR PROCESS BIT
    // asm: 	NOT	R0
    // asm: 	AND	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	CMPI	RDDEBRIS_C|TSC_ROADKILL,R0
    // asm: 	BNE	NOT_ROADKILL
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
NOT_ROADKILL:
    // asm: 	BR 	SUICIDE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYCOLLP", 0, 0);
    UNIMPL();
}

void FREESIGN(void)
{
    // *----------------------------------------------------------------------------
    // *FREESIGN
    // *unlink SIGN from SIGN supplementary list
    // *PARAMETERS
    // *	AR4	OBJECT SIGN POINTER
    // *	R0 IS TRASHED
    // asm: 	PUSH	AR1
    // ;	LDP	@SIGN_LISTI
    // asm: 	LDPI	@SIGN_LISTI,R0
    // asm: 	SUBI	OLINK3,R0		;(we are offset pointing)
SFREELP:
    // asm: LDI	R0,AR1
    // asm: 	LDI	*+AR1(OLINK3),R0
#if DEBUG
    // asm: 	BZ	$			;lockup on end of list found
#else
    // asm: 	RETSZ
#endif
    // asm: 	CMPI	R0,AR4
    // asm: 	BNE	SFREELP
    // asm: 	LDI	*+AR4(OLINK3),R0
    // asm: 	STI	R0,*+AR1(OLINK3)	;LINK AROUND
    // asm: 	LDI	1,R1
    // asm: 	LS	O_SIGN_SUPP_B,R1
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	ANDN	R1,R0     		;TURN OFF SUPP LIST FLAGS
    // asm: 	LDI	1,R1
    // asm: 	LS	O_3DROT_B,R1
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	POP	AR1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREESIGN", 0, 0);
    UNIMPL();
}

void ADDSIGN(void)
{
    // *----------------------------------------------------------------------------
    // *ADDSIGN
    // *LINK SIGN TO SIGN sup list
    // *PARAMETERS
    // *	AR4	OBJECT SIGN POINTER
    // *	R0 IS TRASHED
    // asm: 	LDPI	@SIGN_LIST,R0
    // asm: 	STI	AR4,@SIGN_LIST
    // asm: 	STI	R0,*+AR4(OLINK3)
    // asm: 	LDI	1,R1
    // asm: 	LS	O_SIGN_SUPP_B,R1
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADDSIGN", 0, 0);
    UNIMPL();
}

void FLYCAR(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	LDI	*+AR0(OCARBLK),AR3	;GET PLAYER'S CAR
    // asm: 	LDI	AR1,AR4			;GET DRONE CAR OBJECT POINTER
    // asm: 	LDI	*+AR4(OCARBLK),AR5	;GET DRONE CAR BLOCK
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	NOTLINKED
    // asm: 	LDI	*+AR5(CAR_OM),R0    	;OTHER MACHINES CAR?
    // asm: 	BNE	L78G			;YES SKIP FLYING STUFF
NOTLINKED:
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	TSTB	*+AR1(OFLAGS),R0	;PROCESS ALREADY ACTIVE	?
    // asm: 	BZ	FLYCAR0			;NO
    // asm: 	LDI	*+AR1(OPLINK),R0	;YES, KILL OFF DRONE PROCESS
    // asm: 	BZ	FLYCAR0
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PRC_KILL
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR1(OPLINK)
FLYCAR0:
    // asm: 	LDI	*+AR1(OFLAGS),R0
    // asm: 	TSTB	O_DYNAMIC,R0
    // asm: 	BZ	FLYCAR1
    // asm: 	LDI	*+AR1(ORADZ),R0   	;KILL WHEEL SPINNER, LEANER PROCESS
    // asm: 	BZ	FLYCAR1
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PRC_KILL
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR1(ORADZ)
FLYCAR1:
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CARSHAD)	;TURN OFF SHADOW
    // asm: 	LDF	0.10,R0 		;ADD RANDOM ROTATION
    // asm: 	CALL	SFRAND
    // asm: 	ADDF	*+AR3(CARVROT),R0
    // asm: 	STF	R0,*+AR5(CARVROT)
    // asm: 	LDF	0.45,R0			;RANDOM SPEED MULTIPLIER
    // asm: 	CALL	FRAND
    // asm: 	ADDF	0.8,R0
    // asm: 	MPYF	0.75,R0
    // asm: 	LDF	*+AR5(CARMASS),R1	;DECREASE THROW BY MASS
    // asm: 	CALL	DIV_F
    // asm: 	MPYF	*+AR3(CARSPEED),R0	;COMPUTE DRONE SPEED FROM PLAYER SPD
    // asm: 	STF	R0,*+AR5(CARSPEED)	;STORE NEW DRONE SPEED
    // asm: 	LDF	-0.3,R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	-0.2,R0
    // asm: 	MPYF	1.5,R0			;SPEEDFUDGE FACTOR
    // asm: 	MPYF	*+AR3(CARSPEED),R0	;GET CURRENT SPEED
    // asm: 	CMPF	-65,R0
    // asm: 	LDFLT	-65,R0		  	;MAX VERTICAL VELOCITY
    // asm: 	STF	R0,*+AR1(OVELY)		;STUFF VERTICAL VELOCITY
    // asm: 	LDPI	@FLYCARPI,AR2		;GET SIGN FLY PROCESS
    // asm: 	LDI	DRONE_C|FLYER_T,R2
    // asm: 	CALL	PRC_CREATE_CHILD		;CREATE A CHILD PROCESS
    // asm: 	BC	L78G
    // asm: 	STI	AR0,*+AR4(OPLINK)	;SAVE LINK
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	OR	*+AR4(OFLAGS),R0	;SET ATTACHED PROCESS FLAG
    // asm: 	STI	R0,*+AR4(OFLAGS)
L78G:
    // asm: LDF	*+AR3(CARSPEED),R0  	;CUT SPEED OF PLAYER
    // asm: 	MPYF	0.5,R0
    // asm: 	MPYF	@CHEAT,R0		;BOOST SPEED ON CHEAT
    // asm: 	MPYF	@CHEAT,R0		;BOOST SPEED ON CHEAT
    // asm: 	STF	R0,*+AR3(CARSPEED)
    // asm: 	MPYF	1.25,R0
    // asm: 	LDF	*+AR5(CARSPEED),R1	;KEEP FLYING CAR OUT FRONT
    // asm: 	CMPF	R0,R1
    // asm: 	LDFLT	R0,R1
    // asm: 	STF	R1,*+AR5(CARSPEED)
    // *GET PLAYER CAR VELOCITY, SPIN
    // asm: 	LDI	@WRECKFLG,R0		;WRECK ON?
    // asm: 	BNE	FLY0			;YES, DONT START A NEW ONE
    // asm: 	LDI	400,AR2	    		;TOTAL WRECK PLAYER
    // asm: 	CALL	RANDPER			;NO
    // asm: 	BNC	FLY0
    // asm: 	CALL	WRECKST			;START YOUR WRECK
    // asm: 	LDI	@DETHTAB2I,AR2
    // asm: 	LDI	4,R0
    // asm: 	B	FLYCARXX
FLY0:
    // asm: 	LDPI	@CAMVIEW,R0
    // asm: 	BZ	FLY1
    // asm: 	LDI	750,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BC	FLY3	    		;SPIN SOMETIMES IN 3RD PERSON
    // *FIRST PERSON
FLY1:
    // asm: 	LDF	*+AR3(CARVROT),R0      	;REVERSE VELOCITY
    // asm: 	ADDF	3.14,R0
    // asm: 	STF	R0,*+AR3(CARVROT)
    // asm: 	LDF	0,R1
    // asm: 	LDI	15,R0			;REVERSE FOR 15 COUNT
    // asm: 	B	FLYCARX
FLY3:
    // asm: 	LDF	3.14,R0			;SPIN HIM AROUND
    // asm: 	STF	R0,*+AR3(CARSPRAD)
    // asm: 	LDF	0.04,R0
    // asm: 	CALL	SFRAND
    // asm: 	LDF	R0,R0
    // asm: 	LDFGT	0.08,R1
    // asm: 	LDFLE	-0.08,R1
    // asm: 	ADDF	R0,R1
    // asm: 	LDI	1,R0
FLYCARX:
    // asm: 	STF	R1,*+AR3(CARDROT)
    // asm: 	STI	R0,*+AR3(CAR_SPIN)
    // asm: 	LDI	@DETHTAB1I,AR2
    // asm: 	LDI	8,R0
FLYCARXX:
    // asm: 	LDI	*+AR1(OROMDATA),R1  	;CHECK FOR A BUS...
    // asm: 	CMPI	@SBUSI,R1
    // asm: 	BNZ	FC00
    // asm: 	LDI	KIDSCREAM2,AR2
    // asm: 	B	FC01
FC00:
    // asm: 	CMPI	@CBUSI,R1
    // asm: 	BNZ	FC02
    // asm: 	LDI	ROAR,AR2
FC01:
    // asm: 	CALL	ONESNDFX
    // asm: 	B	FC03
FC02:
    // asm: 	CALL	RANDSND
FC03:
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    // *
    // *FLYING CAR PROCESS
    // *AR4=DRONE PROCESS
    // *AR5=DRONE CAR BLOCK
    // *PDATA=   X RAD
    // *PDATA+1= Y RAD
    // *PDATA+2= Z RAD
    // *PDATA+3= X RAD TOTAL
    // *PDATA+4= Y RAD	TOTAL
    // *PDATA+5= Z RAD	TOTAL
    // *PDATA+6=MATRIX
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYCAR", 0, 0);
    UNIMPL();
}

void FLYCARP(void)
{
    // *GET YOUR RADIANS
    // asm: 	LDF	0.2,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(PDATA)	  	;X RADIANS
    // asm: 	LDF	0.1,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(PDATA+1)	;Y RADIANS
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(PDATA+2)	;Z RADIANS
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(PDATA+3)	;X RADIAN TOTAL
    // asm: 	STF	R0,*+AR7(PDATA+5)  	;Z RADIAN TOTAL
    // asm: 	LDF	*+AR5(CARYROT),R0	;GET CAR Y ROT
    // asm: 	STF	R0,*+AR7(PDATA+4)
FLYCARP0:
    // asm: 	LDI	@SUSPEND_MODE,R0       	;WAIT IN SUSPEND MODE
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BZ	FLYCARSLP
    // asm: 	LDI	0,R4
    // asm: 	LDI	*+AR4(ODIST),R0		;OUT OF RANGE ??
    // asm: 	CMPI	-6000,R0
    // asm: 	BLT	FLYCARPXX   		;END THIS FARCE
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	CALL	ROADSCAN
    // asm: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR DATA AREA
    // asm: 	FLOATP	@NFRAMES,R1	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm: 	LDF	*+AR7(PDATA),R0		;ACCUMULATE X RADIANS
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(PDATA+3),R0
    // asm: 	STF	R0,*+AR7(PDATA+3)
    // asm: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(PDATA+4),R0
    // asm: 	STF	R0,*+AR7(PDATA+4)
    // ;	LDF	*+AR7(PDATA+2),R0 	;ACCUMULATE Z RADIANS
    // ;	MPYF	R1,R0
    // ;	ADDF	*+AR7(PDATA+5),R0
    // ;	STF	R0,*+AR7(PDATA+5)
    // asm: 	CALL	GETFLYMAT		;COMPUTE MATRICES
    // *CONVERT CARVROT,CARSPEED TO OVELX, OVELZ
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	ADDF	1.57,R2		   	;CORRECT FOR 90 DEGREE ERROR
    // asm: 	CALL	_SINE
    // asm: 	LDF	*+AR5(CARSPEED),R3
    // asm: 	MPYF	R3,R0
    // asm: 	STF	R0,*+AR4(OVELZ)	  	;CONVERT TO CARVROT, CARSPEED TO XZVEL
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R3,R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	CALL	OVELNADD		;UPDATE VELOCITIES
    // asm: 	FLOATP	@NFRAMES,R2
    // asm: 	MPYF	2,R2			;FRAME ADJUSTED GRAVITY
    // asm: 	ADDF	*+AR4(OVELY),R2
    // asm: 	STF	R2,*+AR4(OVELY)
    // asm: 	FLOAT	300,R1		  	;GOING DOWN TOO MUCH?
    // asm: 	CMPF	R1,R2
    // asm:  	BGT	FLYCARPXXX		;ABORT THE DUDE...
    // asm: 	CALL	GETTRAK
    // asm: 	LDPI	@_MODE,R0
    // asm: 	TSTB	MBRIDGE,R0		;ON BRIDGE?
    // asm: 	CALLZ	DRONINBZ		;CHECK BOUNDS IF NO BRIDGE
    // asm: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // asm: 	BN	FLYCARSLP		;WERE GOING UP IGNORE IT
    // asm: 	CALL	BOXSCAN		      	;KEEP FALLING!!!
    // asm: 	BNC	FLYCARSLP
    // *WERE OVER THE ROAD
    // asm: FLYCROAD
    // asm: 	LDF	R0,R0
    // asm: 	BGT	FLYCARSLP		;WERE ABOVE GROUND
    // *WE HIT THE GROUND DUDES
    // asm: 	LDF	R0,R2			;Save for offseting sparks
    // asm: 	ADDF	30,R2			;IMPACT_SPARKS uses this
    // asm: 	CALL	ROAD_IMPACT_SPARK
    // asm: 	ADDF	*+AR4(OPOSY),R0		;SET HIM ON THE GROUND
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // ;	CMPF	20,R2          		;CHECK FOR MINIMUM
    // asm: 	CMPF	35,R2          		;CHECK FOR MINIMUM
    // asm: 	BLT	FLYCARSTOP0    		;TIME TO STOP
    // asm: FLYCROAD1
    // asm: 	MPYF	-0.5,R2
    // asm: 	STF	R2,*+AR4(OVELY)
    // asm: 	LDF     *+AR5(CARSPEED),R0	;CUT SPEED IN HALF
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // ;	LDF	*+AR4(OVELX),R0		;CUT DOWN VELOCITIES
    // ;	MPYF	0.5,R0
    // ;	STF	R0,*+AR4(OVELX)
    // ;	LDF	*+AR4(OVELZ),R1		;CUT DOWN VELOCITIES
    // ;	MPYF	0.5,R1
    // ;	STF	R1,*+AR4(OVELZ)
    // asm: 	LDF	*+AR7(PDATA),R0		;CUT DOWN SPIN
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR7(PDATA)
    // asm: 	LDF	*+AR7(PDATA+1),R0
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR7(PDATA+1)
    // ;	LDF	*+AR7(PDATA+2),R0
    // ;	MPYF	0.5,R0
    // ;	STF	R0,*+AR7(PDATA+2)
    // *MAKE BOUNCE SOUND
    // asm: 	LDPI	@SCOLLTABI,AR2	   	;RANDOM COLLISION CRUNCH
    // asm: 	LDI	3,R0
    // asm: 	CALL	DRONESND
FLYCARSLP:
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_FLY_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	SLEEP	1
    // asm: 	B	FLYCARP0
    // *ROTATE TO QUIESCENT STATE
FLYCARSTOP0:
    // ;	CALL	GETCARVSPD		;CONVERT XVEL,ZVEL TO CARSPEED, CARVROT
FLYCARSTOP:
    // asm: 	LDI	@SUSPEND_MODE,R0       	;WAIT IN SUSPEND MODE
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BZ	FLYSTOPSLP
    // asm: 	LDI	1,R4
    // asm: 	LDI	*+AR4(ODIST),R0		;OUT OF RANGE ??
    // asm: 	CMPI	-6000,R0
    // asm: 	BLT	FLYCARPXX   		;END THIS FARCE
    // asm: 	FLOAT	500,R0
    // asm: 	STF	R0,*+AR4(OVELY)		;FORCE ONTO GROUND
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	ADDF	1.57,R2		   	;CORRECT FOR 90 DEGREE ERROR
    // asm: 	CALL	_SINE
    // asm: 	LDF	*+AR5(CARSPEED),R3
    // asm: 	MPYF	R3,R0
    // asm: 	STF	R0,*+AR4(OVELZ)	  	;CONVERT TO CARVROT, CARSPEED TO XZVEL
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R3,R0
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	LDF	*+AR5(CARDROT),R0
    // asm: 	STF	R0,*+AR7(PDATA+1)	;GET Y SPIN
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	CALL	ROADSCAN
    // asm: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR DATA AREA
    // asm: 	LDPI	@NFRAMES,RC	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm: 	SUBI	1,RC
    // asm: 	RPTB	FLYCSTL
    // asm: 	LDF	*+AR4(OVELX),R0
    // asm: 	MPYF	0.98,R0			;DECAY VELOCITY
    // asm: 	STF	R0,*+AR4(OVELX)
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OVELY),R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OVELZ),R0
    // asm: 	MPYF	0.98,R0			;DECAY VELOCITY
    // asm: 	STF	R0,*+AR4(OVELZ)
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(PDATA),R0		;ACCUMULATE X RADIANS
    // ;	MPYF	0.97,R0  		;DAMP IT
    // asm: 	MPYF	0.96,R0  		;DAMP IT
    // asm: 	STF	R0,*+AR7(PDATA)		;CUT DOWN ROCK
    // asm: 	ADDF	*+AR7(PDATA+3),R0
    // asm: 	STF	R0,*+AR7(PDATA+3)
    // asm: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // ;	MPYF	0.985,R0  		;DAMP IT
    // asm: 	MPYF	0.98,R0  		;DAMP IT
    // asm: 	STF	R0,*+AR7(PDATA+1)	;CUT DOWN ROCK
    // asm: 	STF	R0,*+AR5(CARDROT)	;STORE IN CAR STRUCTURE
    // asm: 	ADDF	*+AR7(PDATA+4),R0
    // asm: 	STF	R0,*+AR7(PDATA+4)
    // ;	LDF	*+AR7(PDATA+2),R0 	;ACCUMULATE Z RADIANS
    // ;	ADDF	*+AR7(PDATA+5),R0
    // ;	STF	R0,*+AR7(PDATA+5)
    // asm: 	LDF	*+AR7(PDATA+3),R2	;CHECK TOTAL X RADIANS
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2,R3
    // asm: 	LDF	*+AR5(CARSPEED),R0	;DECAY SPEED
    // asm: 	MPYF	0.98,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // *CHECK FOR DONE...
    // asm: 	CMPF	10,R0	       		;SPEED DECAYED?
    // asm: 	BGT	FLYCSTP0	     	;NO, KEEP GOING...
    // asm: 	ABSF	*+AR5(CARDROT),R0	;GET ROTATE
    // asm: 	ABSF	*+AR7(PDATA),R1	    	;ADD IN ROCK
    // asm: 	ADDF	R0,R1
    // asm: 	MPYF	10,R1	    		;GET IN RANGE
    // asm: 	CMPF	0.02,R1			;PETERED OUT?
    // asm: 	BGT	FLYCSTP0		;NO, KEEP GOING
    // asm: 	CMPF	0.2,R3			;RIGHT SIDE UP?
    // asm: 	BGT	FLYCSTP00		;NOPE...
    // asm: 	LDF	0,R3			;STRAIGHTEN HIM UP!!!
    // asm: 	STF	R3,*+AR7(PDATA+3)
    // asm: 	LDI	2,R0
    // asm: 	B	FLYCCC			;YES, TIME TO STOP
FLYCSTP00:
    // asm: 	CMPF	2.95,R3			;UPSIDE DOWN?
    // asm: 	BLT	FLYCSTP0		;NOPE
    // asm: FLYCSTP
    // asm: 	LDI	1,R0	 		;WERE DONE DUDES...
    // asm: 	B	FLYCCC
    // *ACCELERATE X ROTATION
FLYCSTP0:
    // asm: 	LDF	R2,R2
    // asm: 	BN	FLYCSTP1
    // asm: 	CMPF	1.57,R2
    // asm: 	B 	FLYCSTP2
FLYCSTP1:
    // asm: 	CMPF	-1.57,R2
FLYCSTP2:
    // asm: 	LDFLT	-0.01,R0
    // asm: 	LDFGE	0.01,R0
    // asm: 	MPYF	0.4,R0
    // asm: 	ADDF	*+AR7(PDATA),R0
    // asm: 	LDF	*+AR7(PDATA),R1
    // asm: 	XOR	R0,R1,R2 		;CHECK FOR SIGN CHANGE- MAKE SOUND
    // asm: 	BNN	FLYCSTL			;NO SOUND
    // asm: 	ABSF	*+AR7(PDATA+3),R2	;CHECK IF AMPLITUDE BIG ENOUGH
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2
    // asm: 	CMPF	0.08,R2
    // asm: 	BLT	FLYCSTL			;TOO SMALL OF A ROCK
    // asm: 	CMPF	3.06,R2
    // asm: 	BGT	FLYCSTL			;TOO SMALL OF A UPSIDE DOWN ROCK
    // asm: 	LDI	BOTTOMOUT,AR2		;MAKE BOTTOMOUT SOUND
    // asm: 	PUSHF	R0
    // asm: 	CALL	DRONESND1
    // asm: 	POPF	R0
FLYCSTL:
    // asm: STF	R0,*+AR7(PDATA)		;ACCELERATE ROTATION
    // asm: 	LDI	0,R0			;DONE FLAG
FLYCCC:
    // asm: 	PUSH	R0
    // asm: 	CALL	GETTRAK
    // asm: 	CALL	DRONINBZ		;CHECK BOUNDS
    // asm: 	CALL	GETFLYMAT
    // asm: 	CALL	BOXSCAN		      	;KEEP FALLING!!!
    // asm: 	POP	R1			;CLEAN STACK
    // asm: 	BNC	FLYCARSTP
    // asm: 	ADDF	*+AR4(OPOSY),R0		;SET HIM ON THE GROUND
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	CALL	SKID_SPARK
    // asm: 	LDI	R1,R1			;DONE
    // asm: 	BNE	FLYCARSTP		;YESSAH
FLYSTOPSLP:
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_FLY_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	SLEEP	1
    // asm: 	B	FLYCARSTOP
FLYCARSTP:
    // asm: 	CMPI	2,R1
    // asm: 	BZ	DEADCAR			;RIGHT SIDE UP CARCASS
    // ;	LDPI	@SCOLLTABI,AR2	   	;RANDOM COLLISION CRUNCH
    // ;	LDI	3,R0
    // ;	CALL	DRONESND
    // *
    // *CAR IS UPSIDE DOWN
    // *
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	ANDN	TYPE_M,R0
    // asm: 	OR	DEAD_VEH_T,R0
    // asm: 	LDF	0,R0			;CLEAR OUT THE SPEED
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // *WAIT FOR OFFSCREEN
FLYCARWT:
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_FLY_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	SLEEP	1
    // asm: 	LDI	1,R4			;STATE #
    // asm: 	LDI	*+AR4(ODIST),R0	     	;OUT OF RANGE?
    // asm: 	CMPI	-6000,R0
    // asm: 	BLT	FLYCARPXX		;YES CLEAN IT UP...
    // asm: 	LDF	*+AR5(CARSPEED),R0	;ARE WE HIT???
    // asm: 	BZ	FLYCARWT		;NO, JUST WAIT
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
    // asm: 	LDF	0.1,R0		       	;ROCK HIM A LITTLE
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(PDATA)	  	;X RADIANS RATE
    // asm: 	B	FLYCARSTOP		;GO ROCK AND ROLL
    // *CLEAN UP THE MESS...
FLYCARPXX:
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	BZ	FLYCARPXXXX		;NOPE...
    // asm: 	CALL	COMPTRAK 		;OTHER GUY BEHIND?
    // asm: 	BLE	FLYCARPXXX		;NO KILL THE DUDE...
    // asm: 	CALL	SEND_FLY_XSFER
    // asm: 	BR	OM_DRONE		;CONTROL SWAPS TO OTHER MACHINE
FLYCARPXXX:
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_FLY_KILL		;SEND YOUR POSITION TO LINKED GAME
FLYCARPXXXX:
    // asm: 	CALL	FREE_DRONE
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	DELCAR
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	XOR	*+AR4(OFLAGS),R0	;WIPE OUT PROCESS BIT
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	DIE
    // *
    // *CAR IS RIGHT SIDE UP
    // *
DEADCAR:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR5(CARSHAD) 	;TURN BACK ON THE SHADOW
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	ANDN	TYPE_M,R0
    // asm: 	OR	DEAD_VEH_T,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	STI	R0,*+AR5(CAR_ID)
    // asm: 	STI	R0,*+AR7(PID)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm: 	STF	R0,*+AR5(CARDROT)
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // asm: 	LDF	*+AR7(PDATA+4),R0 	;GET Y ROTATION ORIENTATION
    // asm: 	STF	R0,*+AR5(CARVROT)
    // asm: 	STF	R0,*+AR5(CARYROT)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_SPIN)	;CLEAR SPIN
DEADLP:
    // asm: 	LDI	2,R4
    // asm: 	LDI	*+AR4(ODIST),R0	      	;DIE OFF WHEN OFFSCREEN
    // asm: 	CMPI	-6000,R0
    // asm: 	BLT	FLYCARPXX
    // asm: 	LDF	0,R2			;NO STEERING
    // asm: 	CALL	DRONEGO
    // asm: 	CALL	GETTRAK
    // asm: DEADSLP
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_FLY_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	SLEEP	1
    // asm: 	B	DEADLP
    // *
    // *KILL OFF FLY MESSAGE
    // *AR4= OBJECT
    // *AR5= CAR BLOCK
    // *
    // asm: SEND_FLY_KILL
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_FLY_KILL,R1
    // asm: 	STI	R1,*AR2
    // asm: 	LDI	*+AR5(CARNUM),R0
    // asm: 	STI	R0,*+AR2(1)
    // asm: 	LDI	2-1,RC
    // asm: 	CALL	MESSAGE_ADD
    // asm: 	RETS
    // *
    // *KILL OFF FLY
    // *
    // asm: DECODE_FLY_KILL
    // asm: 	CALL	FIND_DRONE  		;GET DRONE OBJ IN AR0
    // asm: 	BNZ	DRKX
    // asm: 	LDI	*+AR0(OPLINK),AR7
    // asm: 	LDI	@FLYCARPXXXXI,R2 	;KILL THE SOMBITCH
    // asm: 	STI	R2,*+AR7(PWAKE)		;CHANGE WAKE-UP ADDR
DRKX:
    // asm: 	RETS
    // *
    // *KILL OFF FLY MESSAGE
    // *AR4= OBJECT
    // *AR5= CAR BLOCK
    // *AR7= PROCESS
    // *R4=  STATE PARAMETER
    // *
    // asm: SEND_FLY_XSFER
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	LDI	CB_FLY_XSFER,R1
    // asm: 	STI	R1,*AR2++		;SEND HEADER
    // asm: 	LDI	*+AR5(CARNUM),R1	;SEND ID
    // asm: 	STI	R1,*AR2++
    // asm: 	STI	R4,*AR2++		;SEND STATE
    // asm: 	LDI	AR7,AR0
    // asm: 	ADDI	PDATA,AR0		;GET PDATA 0-5
    // asm: 	LDI	5,RC
    // asm: 	RPTB	SENDP
    // asm: 	LDI	*AR0++,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LSH	-8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LSH	-8,R0
    // asm: 	STI	R0,*AR2++
    // asm: 	LSH	-8,R0
SENDP:
    // asm: STI	R0,*AR2++
    // asm: 	LDI	27-1,RC
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	CALL	MESSAGE_ADD
    // asm: 	RETS
    // *
    // *GET A FLYER FROM OTHER GAME
    // *AR2=MESSAGE BUFFER
    // *
    // asm: DECODE_FLY_XSFER
    // asm: 	CALL	FIND_DRONE  		;GET DRONE OBJ IN AR0
    // asm: 	BNZ	DFXX
    // asm: 	LSH	R2,*AR2++,R4		;GET STATE
    // asm: 	LDI	*+AR0(OPLINK),AR1
    // asm: 	ADDI	PDATA,AR1		;GET PDATA 0-5
    // asm: 	LDI	8,R5
    // asm: 	LDI	5,RC
    // asm: 	RPTB	DECP
    // asm: 	LSH	R2,*AR2++,R0
    // asm: 	LSH	R3,*AR2++,R1
    // asm: 	ADDI	R1,R0
    // asm: 	ADDI	*AR2++,R0
    // asm: 	LSH	R5,*AR2++,R1
    // asm: 	ADDI	R1,R0
DECP:
    // asm: STI	R0,*AR1++
    // asm: 	LDI	*+AR0(OCARBLK),AR5
    // asm: 	LDI	*+AR0(OPLINK),AR7
    // asm: 	LDI	0,R0			;OUR CAR NOW....
    // asm: 	STI	R0,*+AR5(CAR_OM)
    // asm: 	LDI	*+AR5(CARTRACK_ID),R2	;GET TRACK ID
    // asm: 	LDI	@FLYCARPXXXI,R5 	;KILL THE SOMBITCH
    // asm: 	LDI	@DYNALIST_END,AR0	;GET FURTHEST ROAD ID
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	CMPI	R0,R2
    // asm: 	BGT	DFX1			;TOO FAR OUT, DIE
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	CMPI	R0,R2
    // asm: 	BLT	DFX1			;BEHIND US KILL HIM
    // asm: 	LDI	@DEADLPI,R5		;DEFAULT
    // asm: 	CMPI	0,R4	  		;FLYIN'
    // asm: 	LDIZ	@FLYCARP0I,R5
    // asm: 	CMPI	1,R4	  		;ROCKIN'
    // asm: 	LDIZ	@FLYCARSTOPI,R5
DFX1:
    // asm: 	STI	R5,*+AR7(PWAKE)		;CHANGE WAKE-UP ADDR
    // asm: 	RETS
DFXX:
    // asm: 	ADDI	25,AR2			;SKIP REST OF MESSAGE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYCARP", 0, 0);
    UNIMPL();
}

void GETFLYMAT(void)
{
    // *----------------------------------------------------------------------------
    // *GET MATRIX FOR FLYING CAR
    // *
    // *PARAMETERS
    // *	PDATA+3,4,5 = X,Y,ZRAD
    // *	AR4	OBJECT
    // *	AR7	PROCESS
    // *
    // *R2,R3,AR2 TRASHED
    // *
    // asm: 	LDF	*+AR7(PDATA+5),R2
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL    FIND_ZMATRIX
    // asm: 	LDPI	@MATRIXAI,AR2
    // asm: 	LDF	*+AR7(PDATA+3),R2
    // asm: 	CALL    FIND_XMATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMAT		;FORMULATE COMBINED MATRIX
    // asm: 	LDPI	@MATRIXAI,AR2
    // asm: 	LDF	*+AR7(PDATA+4),R2
    // asm: 	CALL    FIND_YMATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	B	CONCATMAT		;FORMULATE COMBINED MATRIX
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETFLYMAT", 0, 0);
    UNIMPL();
}

void PLYR_VS_DRONES(void)
{
    // *----------------------------------------------------------------------------
    // *COLLISION SCAN
    // *
    // *CHECK OBJECT AGAINST LIST
    // *
    // *PARAMETERS
    // *	AR0	OBJECT
    // *	AR1	ADDRESS OF LIST HEADER
    // *
    // asm: COLSCAN
    // asm: 	BD	COLSCL0
    // asm: 	LDI	@_plyr1+PLY_CAR,AR0	;GET PLAYER CAR
    // asm: 	LDI	@CAR_LISTI,AR1
    // asm: 	SUBI	OLINK3,AR1		;SETUP INDEXING
    // ********B	COLSCL0
COLSCLP0:
    // asm: 	BNZD	COLSCL		    	;DONT COLLIDE DUDES...
    // asm: 	SUBF	*+AR1(OPOSX),R2,R0
    // asm: 	MPYF	R0,R0
    // asm: 	SUBF	*+AR1(IR0),R3,R4
    // ********BNZD	COLSCL
    // asm: 	MPYF	R4,R4
    // asm: 	ADDF	R0,R4
    // asm: 	FLOAT	*+AR1(ORAD),R1
    // asm: 	ADDF	R5,R1
    // asm: 	MPYF	R1,R1			;SQUARE THE RADIUS LENGTH
    // asm: 	CMPF	R1,R4	 		;ARE WE WITHIN RADIUS?
    // asm: 	BGT	COLSCL
    // asm: 	CALL	COLCHK			;CHECK OUT COLLISION FURTHER
    // asm: 	BNC	COLSCL0			;NO COLLIDE
    // asm: 	LDI	*+AR0(OCARBLK),AR4	;CHECK FOR LOW SPEED PLOW
    // asm: 	LDF	*+AR4(CARSPEED),R0
    // asm: 	LDI	*+AR1(OCARBLK),AR5	;CHECK FOR LOW SPEED PLOW
    // asm: 	ADDF	*+AR5(CARSPEED),R0
    // asm: 	CMPF	100,R0
    // asm: 	LDFGT	100,R0
    // asm: 	SUBRF	100,R0
    // asm: 	MPYF	0.02,R0
    // asm: 	ADDF	1.0,R0
    // asm: 	STF	R0,@PMULT		;SPEED MULTIPLIER
    // asm: 	B	COLDISP
COLSCL0:
    // asm: 	LDI	OPOSZ,IR0
    // asm: 	LDF	*+AR0(OPOSX),R2		;GET X COORD
    // asm: 	LDF	*+AR0(OPOSZ),R3		;GET Z COORD
    // asm: 	FLOAT	*+AR0(ORAD),R5		;GET SUCKERS RADIUS
COLSCL:
    // asm: 	LDI	*+AR1(OLINK3),R0
    // asm: 	BNZD	COLSCLP0
    // asm: 	LDI	R0,AR1
    // asm: 	LDI	*+AR1(OFLAGS),R0
    // asm: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // ********BNZD	COLSCLP0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYR_VS_DRONES", 0, 0);
    UNIMPL();
}

void DRONES_VS_DRONES(void)
{
    // *----------------------------------------------------------------------------
    // *DRONE COLLISION SCAN
    // *CHECK DRONES AGAINST DRONES
    // *CHECK OBJECT AGAINST LIST
    // *
    // *PARAMETERS
    // *	AR0	OBJECT
    // *	AR1	ADDRESS OF LIST HEADER
    // *
    // asm: CLDSCAN
    // asm: 	LDPI	@CAR_LIST,R0	 	;GET LIST AND CHECK NULL
    // asm: 	BNZD	CLDSCL0
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	R0,AR1
    // asm: 	NOP
    // 	;------->BNZD CLDSCL0
    // asm: 	RETS
CLDSCLP0:
    // asm: 	BNZD	CLDSCL			;NOCOL BIT SET
    // asm: 	SUBF	*+AR1(OPOSX),R2,R0
    // asm: 	MPYF	R0,R0
    // asm: 	SUBF	*+AR1(IR0),R3,R4
    // 	;---->  BNZD	CLDSCL		;NOCOL BIT SET
    // asm: 	MPYF	R4,R4
    // asm: 	ADDF	R0,R4
    // asm: 	FLOAT	*+AR1(ORAD),R1
    // asm: 	ADDF	R5,R1
    // asm: 	MPYF	R1,R1			;SQUARE THE RADIUS LENGTH
    // asm: 	CMPF	R1,R4	 		;ARE WE WITHIN RADIUS?
    // asm: 	BGT	CLDSCL
    // asm: 	CALL	COLCHK			;CHECK OUT COLLISION FURTHER
    // asm: 	BC	COLDISP
CLDSCL0:
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // asm: 	BNZ	CLDSCL1			;NON COLLIDABLE STEALTH OBJECT
    // asm: 	LDF	*+AR0(OPOSX),R2		;GET X COORD
    // asm: 	LDF	*+AR0(OPOSZ),R3		;GET Z COORD
    // asm: 	LDI	OPOSZ,IR0
    // asm: 	FLOAT	*+AR0(ORAD),R5		;GET SUCKERS RADIUS
CLDSCL:
    // asm: 	LDI	*+AR1(OLINK3),R0
    // asm: 	BNZD	CLDSCLP0
    // asm: 	LDI	R0,AR1
    // asm: 	LDI	*+AR1(OFLAGS),R0
    // asm: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // 	;------->BNZD	CLDSCLP0
CLDSCL1:
    // asm: 	LDI	*+AR0(OLINK3),R0    	;GET NEXT LIST
    // asm: 	BNZD	CLDSCL0
    // asm: 	LDI	R0,AR1
    // asm: 	LDI	R0,AR0
    // asm: 	NOP
    // 	;------->BNZD CLDSCL0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONES_VS_DRONES", 0, 0);
    UNIMPL();
}

void REPELL(void)
{
    // *----------------------------------------------------------------------------
    // *REPELL COLLISION OBJECTS
    // *
    // *PARAMETERS
    // *	AR0		OBJECT 0
    // *	AR1		OBJECT 1
    // *RETURNS
    // *	AR2	POINTS TO NORMALIZED REPULSION VECTOR
    // *	R0	VELOCITY MAGNITUDE OF REPULSION
    // *
    // *FIND REPULSION AXIS
    // *
    // asm: 	LDPI	@VECTORAI,AR2		;COMPUTE REPULSION VECTOR
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	SUBF	*+AR1(OPOSX),R0
    // asm: 	STF	R0,*AR2
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR2(1)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	SUBF	*+AR1(OPOSZ),R0
    // asm: 	STF	R0,*+AR2(2)
    // asm: 	CALL	NORMALIZE		;NORMALIZE IT
    // *FIND RELATIVE VELOCITY MAGNITUDE
    // asm: 	LDF	*+AR0(OVELX),R0
    // asm: 	SUBF	*+AR1(OVELX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR0(OVELZ),R1
    // asm: 	SUBF	*+AR1(OVELZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R0,R1,R2
    // asm: 	BR	SQRT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REPELL", 0, 0);
    UNIMPL();
}

void COLDISP(void)
{
    // *----------------------------------------------------------------------------
    // *COLLISION CHECK
    // *
    // *PARAMETERS
    // *	AR0		OBJECT 0
    // *	AR1		OBJECT 1
    // *	*-AR3(1)	COLLISION PT
    // *
    // *SET COLLISION BITS
    // asm: 	LDI	*+AR0(OCARBLK),AR4
    // asm: 	LDI	*+AR1(OCARBLK),AR5
    // *check vs helicopter -> not a normal collision
    // asm: 	LDI	*+AR1(OID),R2
    // asm: 	CMPI	DRONE_C|HELICOPTER,R2
    // asm: 	BNE	NOTHELI
    // asm: 	SUBF	*+AR1(OPOSX),*+AR0(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	SUBF	*+AR1(OPOSY),R2
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R0,R2
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	SUBF	*+AR1(OPOSZ),R0
    // asm: 	MPYF	R0,R0
    // asm: 	ADDF	R0,R2
    // asm: 	CALL	SQRT
    // asm: 	LDF	*+AR1(ORAD),R1
    // asm: 	MPYF	0.5,R1
    // asm: 	CMPF	R1,R0
    // asm: 	RETSGT
NOTHELI:
    // asm: 	LDI	*+AR0(OID),R2
    // asm: 	AND	CLASS_M|TYPE_M,R2
    // asm: 	CMPI	DRONE_C|RAILROAD,R2
    // asm: 	BNE	NTRN
    // asm: 	CMPI	*+AR1(OID),R2
    // asm: 	RETSEQ
NTRN:
    // asm: 	LDF	*+AR0(OPOSY),R0		;MAKE SURE HEIGHT IS CLOSE
    // asm: 	SUBF	*+AR1(OPOSY),R0
    // asm: 	ABSF	R0
    // asm: 	FLOAT	750,R1
    // asm: 	CMPF	R1,R0
    // asm: 	RETSGT				;IF HEIGHT TO FAR AWAY, FORGET IT...
    // asm: 	CALL	IMPACT_SPARK
    // *REPELL CARS
    // asm: 	CALL	REPELL	  		;R0=REPULSION MAGNITUDE
    // asm: 	CALL	COLSND			;MAKE YOUR SOUND...
    // asm: 	MPYF	0.5,R0	   		;ADJUST MAGNITUDE FOR 1/2 EACH OBJECT
    // asm: 	LDF	R0,R1
    // asm: 	MPYF	*AR2,R0			;MULTIPLY BY X,Z DIRECTIONAL VECTOR
    // asm: 	MPYF	*+AR2(2),R1
    // asm: 	LDF	*+AR0(OPOSX),R2		;REPELL THE SUCKER (AR0)
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	ADDF	R0,R2
    // asm: 	ADDF	R1,R3
    // asm: 	STF	R2,*+AR0(OPOSX)
    // asm: 	STF	R3,*+AR0(OPOSZ)
    // asm: 	LDI	*+AR1(OID),R2
    // asm: 	AND	CLASS_M|TYPE_M,R2
    // asm: 	CMPI	DRONE_C|RAILROAD,R2
    // asm: 	BEQ	FLYTRAIN
    // asm: 	LDF	*+AR1(OPOSX),R2		;REPELL THE SUCKER (AR1)
    // asm: 	LDF	*+AR1(OPOSZ),R3
    // asm: 	SUBF	R0,R2
    // asm: 	SUBF	R1,R3
    // asm: 	STF	R2,*+AR1(OPOSX)
    // asm: 	STF	R3,*+AR1(OPOSZ)
    // *
    // *ELASTIC COLLSION IN X AND Z
    // *FIND X AND Z VELOCITIES OF OBJ AR0
    // *	AR4	OCARBLK OBJECT AR0
    // *
    // * COMPUTE:	R4=XV1
    // *		R5=ZV1
    // *		R6=XV2
    // *		R7=ZV2
    // asm: 	LDI	*+AR0(OCARBLK),AR4
    // asm: 	LDF	*+AR4(CARVROT),R2
    // ;	STF	R2,@CAR1VROTI
    // asm: 	ADDF	@HALFPII,R2	 	;CORRECT FOR 90 DEGREE ERROR
    // asm: 	CALL	_SINE
    // asm: 	LDF	*+AR4(CARSPEED),R3
    // ;	STF	R3,@CAR1SPEEDI
    // asm: 	MPYF	R3,R0,R5		;V1Zi (INIT ZV OBJECT 1)
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R3,R0,R4		;V1Xi (INIT XV OBJECT 1)
    // *FIND X AND Z VELOCITIES OF OBJ AR1
    // *AR5=OCARBLK OBJECT AR1
    // asm: 	LDI	*+AR1(OCARBLK),AR5
    // asm: 	LDF	*+AR5(CARVROT),R2
    // ;	STF	R2,@CAR2VROTI
    // asm: 	ADDF	@HALFPII,R2	   	;CORRECT FOR 90 DEGREE ERROR
    // asm: 	CALL	_SINE
    // asm: 	LDF	*+AR5(CARSPEED),R3
    // ;	STF	R3,@CAR2SPEEDI
    // asm: 	MPYF	R3,R0,R7		;V2Zi (INIT ZV OBJECT 2)
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R3,R0,R6		;V2Xi (INIT XV OBJECT 2)
    // *CHECK FOR FLYING COLLISION
    // asm: 	CMPI	@PLYCAR,AR0		;PLAYERS CAR?
    // asm: 	BNZ	COLDISP0	  	;NO
    // ***************
    // ;	LDF	*+AR4(CARSPEED),R0	;PLAYER SPEED HIGH ENOUGH
    // ;	CMPF	60,R0
    // ;	BGT	FLYCAR		   	;FLY THE SUCKER...
    // ;	B 	COLDISP0
    // ******************
    // asm: 	SUBF	R4,R6,R0
    // asm: 	MPYF	R0,R0
    // asm: 	SUBF	R5,R7,R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R0			;FIND CLOSING SPEED SQUARED
    // asm: 	FLOAT	20000,R1 		;BIG MAGNITUDE ?
    // asm: 	MPYF	2,R1			;2X30000=60000
    // asm: 	CMPF	R1,R0
    // asm: 	BLT	COLDISP0		;NOT A FLYER
    // asm: 	PUSH	AR2			;SAVE REPULSION VECTOR DUDES...
    // asm: 	FLOAT	70,R1	   		;GET PROBABILITY FUNCTION
    // asm: 	CALL	DIV_F
    // asm: 	FIX	R0,AR2
    // asm: 	CALL	RANDPER
    // asm: 	POP	AR2
    // asm: 	BNC	COLDISP0      		;NOT A FLYER
    // asm: 	LDF	*+AR4(CARSPEED),R0	;PLAYER SPEED HIGH ENOUGH
    // asm: 	FLOAT	160,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	FLYCAR		   	;FLY THE SUCKER...
COLDISP0:
    // asm: 	PUSH	AR3	 		;SAVE COLLISION POINT
    // asm: 	LDPI	@MATRIXAI,AR3  		;GET TEMP STORE
    // *COMPUTE INELASTIC VELOCITY
    // asm: 	LDF	*+AR4(CARMASS),R1	;GET MASS1
    // asm: 	CMPI	@PLYCAR,AR0
    // asm: 	BNE	COLIN1
    // asm: 	MPYF	@CHEAT,R1		;BOOST MASS ON CHEAT
    // asm: 	MPYF	@CHEAT,R1
    // asm: 	MPYF	@CHEAT,R1
COLIN1:
    // asm: 	PUSHF	R1
    // asm: 	MPYF	R4,R1,R2		;M1XV1
    // asm: 	MPYF	R5,R1,R3		;M1ZV1
    // asm: 	LDF	*+AR5(CARMASS),R0	;GET MASS2
    // asm: 	MPYF	R6,R0,R1		;M2XV2
    // asm: 	ADDF	R1,R2
    // asm: 	MPYF	R7,R0,R1		;M2ZV2
    // asm: 	ADDF	R1,R3
    // asm: 	POPF	R1
    // asm: 	ADDF	R1,R0			;GET M1+M2
    // ;	ADDF	*+AR4(CARMASS),R0	;GET M1+M2
    // asm: 	CALL	INV_F30
    // asm: 	MPYF	R0,R2			;INELASTIC XV
    // asm: 	MPYF	R0,R3			;INELASTIC ZV
    // asm: 	STF	R2,*+AR3(4)	  	;SAVE INELASTIC XV
    // asm: 	STF	R3,*+AR3(5)	  	;SAVE INELASTIC ZV
    // *COMPUTE (M1-M2)/(M1+M2)
    // asm: 	LDF	*+AR4(CARMASS),R1
    // asm: 	ADDF	*+AR5(CARMASS),R1
    // asm: 	STF	R1,*+AR3(3)	  	;SAVE M1+M2
    // asm: 	LDF	*+AR4(CARMASS),R0
    // asm: 	SUBF	*+AR5(CARMASS),R0
    // asm: 	CALL	DIV_F
    // asm: 	STF	R0,*AR3
    // *COMPUTE 2*M2/(M1+M2)
    // asm: 	LDF	*+AR3(3),R1
    // asm: 	LDF	*+AR5(CARMASS),R0
    // asm: 	MPYF	2,R0
    // asm: 	CALL	DIV_F
    // asm: 	STF	R0,*+AR3(1)
    // *COMPUTE 2*M1/(M1+M2)
    // asm: 	LDF	*+AR3(3),R1
    // asm: 	LDF	*+AR4(CARMASS),R0
    // asm: 	MPYF	2,R0
    // asm: 	CALL	DIV_F
    // asm: 	STF	R0,*+AR3(2)  		;SAVE 2*M1/(M1+M2)
    // *X VELOCITY CASE OBJECT 1
    // asm: 	MPYF	*AR3,R4,R0		;V1Xf = V1Xi(M1-M2)/(M1+M2)
    // asm: 	MPYF	*+AR3(1),R6,R1		;       + V2Xi(2*M2)/(M1+M2)
    // asm: 	ADDF	R1,R0,R2		;V1XF
    // *Z VELOCITY CASE OBJECT 1
    // asm: 	MPYF	*AR3,R5,R0		;V1Zf = V1Zi(M1-M2)/(M1+M2)
    // asm: 	MPYF	*+AR3(1),R7,R1		;       + V2Zi(2*M2)/(M1+M2)
    // asm: 	ADDF	R1,R0,R3		;V1ZF
    // *ADD INELASTIC VELOCITY OBJECT 1
    // asm: 	LDF	*+AR3(4),R0
    // asm: 	LDF	*+AR3(5),R1
    // asm: 	MPYF	0.75,R0
    // asm: 	MPYF	0.75,R1
    // asm: 	MPYF	0.25,R2
    // asm: 	MPYF	0.25,R3
    // asm:   	ADDF	R0,R2
    // asm: 	ADDF	R1,R3
    // *ADD REPULSION VELOCITY	OBJECT 1
    // asm: 	LDF	10.0,R0    		;VELOCITY REPULSION CONSTANT
    // ;	LDF	*+AR4(CARMASS),R1	;DIVIDE BY MASS
    // ;	CALL	DIV_F
    // asm: 	LDF	*AR2,R1		  	;X REPULSION VELOCITY
    // asm: 	MPYF	R0,R1
    // asm: 	ADDF	R1,R2
    // asm: 	LDF	*+AR2(2),R1		;Z REPULSION VELOCITY
    // asm: 	MPYF	R0,R1
    // asm: 	ADDF	R1,R3
    // **********debugging stuff
    // ;	STF	R4,@CAR1XVI	      	;SAVE YOUR VELOCITIES
    // ;	STF	R5,@CAR1ZVI
    // ;	STF	R6,@CAR2XVI
    // ;	STF	R7,@CAR2ZVI
    // *************************end debug stuff
    // asm: 	CALL	ARCTANF
    // asm: 	SUBPF	@HALFPII,R0
    // *STORE VEL THETA, SPEED
    // asm: 	PUSHF	R0
    // ;	STF	R0,*+AR4(CARVROT)
    // asm: 	MPYF	R2,R2
    // asm: 	MPYF	R3,R3
    // asm: 	ADDF	R3,R2
    // asm: 	CALL	SQRT
    // asm: 	STF	R0,*+AR4(CARSPEED)
    // ;	STF	R0,@CAR1SPEEDF		;SAVE FOR DEBUG
    // *X VELOCITY CASE OBJECT 2
    // asm: 	NEGF	*AR3,R0			;(M2-M1)/(M1+M2)
    // asm: 	STF	R0,*AR3
    // asm: 	MPYF	*AR3,R6,R0		;V2Xf = V2Xi(M1-M2)/(M1+M2)
    // asm: 	LDF	*+AR3(2),R3
    // asm: 	MPYF	R3,R4,R1		;       + V1Xi(2*M1)/(M1+M2)
    // asm: 	ADDF	R1,R0,R2		;V2XF
    // *Z VELOCITY CASE OBJECT 2
    // asm: 	MPYF	*AR3,R7,R0		;V1Zf = V1Zi(M1-M2)/(M1+M2)
    // asm: 	MPYF	R3,R5,R1		;       + V2Zi(2*M2)/(M1+M2)
    // asm: 	ADDF	R1,R0,R3		;V2ZF
    // *ADD INELASTIC VELOCITY OBJECT 2
    // asm: 	LDF	*+AR3(4),R0
    // asm: 	LDF	*+AR3(5),R1
    // asm: 	MPYF	0.75,R0
    // asm: 	MPYF	0.75,R1
    // asm: 	MPYF	0.25,R2
    // asm: 	MPYF	0.25,R3
    // asm: 	ADDF	R0,R2
    // asm: 	ADDF	R1,R3
    // asm: 	CMPI	@PLYCAR,AR0		;HIT BY PLAYERS CAR?
    // asm: 	BNE	ZZZ1
    // asm: 	MPYF	@PMULT,R2	       	;SLOW SPEED MULTIPLIER
    // asm: 	MPYF	@PMULT,R3
ZZZ1:
    // *ADD REPULSION VELOCITY	OBJECT 2
    // asm: 	LDF	-10.0,R0    		;VELOCITY REPULSION CONSTANT
    //  ;	LDF	*+AR5(CARMASS),R1	;DIVIDE BY MASS
    //  ;	CALL	DIV_F
    // asm: 	LDF	*AR2,R1		  	;X REPULSION VELOCITY
    // asm: 	MPYF	R0,R1
    // asm: 	ADDF	R1,R2			;ADD TO XV
    // asm: 	LDF	*+AR2(2),R1		;Z REPULSION VELOCITY
    // asm: 	MPYF	R0,R1
    // asm: 	ADDF	R1,R3			;ADD TO ZV
    // asm: 	CALL	ARCTANF
    // asm: 	LDP	HALFPII
    // asm: 	SUBF	@HALFPII,R0
    // *STORE VEL THETA, SPEED
    // asm: 	PUSHF	R0	 		;SAVE NEW CARVROT
    // asm: 	MPYF	R2,R2
    // asm: 	MPYF	R3,R3
    // asm: 	ADDF	R3,R2
    // asm: 	CALL	SQRT
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // ;	STF	R0,@CAR2SPEEDF		;SAVE FOR DEBUG
    // *NORMALIZE VELOCITY ROTATIONS OBJECT 2
    // asm:   	POPF	R2
    // asm: 	LDF	0.333,R0		;ADD A LITTLE RANDOM DIRECTION
    // asm: 	CALL	SFRAND
    // asm: 	ADDF	R0,R2
    // asm: 	CALL	NORMITS
    // asm: 	STF	R2,*+AR5(CARVROT)
    // ;	STF	R2,@CAR2VROTF		;SAVE FOR DEBUG
    // *NORMALIZE VELOCITY ROTATIONS OBJECT 1
    // asm:   	POPF	R2
    // asm: 	CALL	NORMITS
    // asm: 	STF	R2,*+AR4(CARVROT)
    // ;	STF	R2,@CAR1VROTF		;SAVE FOR DEBUG
    // asm: 	POP	AR3
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
    // asm: 	SUBF   	R6,R4,R0	;GET RELATIVE XV
    // asm: 	SUBF   	R7,R5,R1
    // asm: 	CALL	SPINROT
    // asm: COLDSP30
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDI	AR5,AR1		;SWAP AR4,AR5
    // asm: 	LDI	AR4,AR5
    // asm: 	LDI	AR1,AR4
    // asm: 	LDI	AR0,AR1
    // asm: 	POP	AR0		;SWAP AR0,AR1
    // asm: 	SUBF  	R4,R6,R0	;GET RELATIVE XV
    // asm: 	SUBF  	R5,R7,R1	;GET RELATIVE ZV
    // asm: 	CALL	SPINROT
    // asm: 	POP	AR0
    // asm: 	POP	AR1
    // asm: COLDSPX
    // asm: 	RETS	       	       	;FOR NOW DUDES
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLDISP", 0, 0);
    UNIMPL();
}

void SPINROT(void)
{
    // *
    // asm: 	LDI	0,R2
    // asm: 	STI	R2,@PLYRBEHIND		;PLAYER HIT FROM BEHIND FLAG
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R1
    // asm: 	MPYF	R0,R0	   		;GET INTENSITY OF RELATIVE SPEED
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R0,R1,R2
    // asm: 	CALL	SQRT
    // asm: 	LDF	R0,R3			;SAVE INTENSITY
    // asm: 	CMPF	20,R0			;SET BUMP FLAG FOR BIGGIE
    // asm: 	LDIGT	1,R2
    // asm: 	LDILE	0,R2
    // asm: 	STI	R2,*+AR5(CAR_BUMP)
    // asm: 	POPF	R1
    // asm: 	POPF	R0
    // asm: 	PUSHF	R3	      		;SAVE INTENSITY
    // asm: 	LDPI	@VECTORAI,AR2
    // asm: 	LDF	*+AR1(OPOSX),R2	   	;GET OBJECT2 XZ CENTER ORIGIN
    // asm: 	SUBF3	R2,*-AR3(1),R2		;GET RELATIVE POSITION OF COLLISION PT.
    // asm: 	STF	R2,*AR2
    // asm: 	LDF	*+AR1(OPOSZ),R2
    // asm:  	SUBF3	R2,*+AR3(1),R2
    // asm: 	STF	R2,*+AR2(1)
    // *GET RELATIVE VELOCITY OBJECT AR1 PERSPECTIVE
    // asm: 	MPYF	0.5,R0		;MAKE THIS SMALL
    // asm: 	ADDF	*AR2,R0
    // asm: 	STF	R0,*+AR2(2)
    // asm: 	MPYF	0.5,R1		;MAKE THIS SMALL
    // asm: 	ADDF	*+AR2(1),R1
    // asm: 	STF	R1,*+AR2(3)
    // *COMPUTE CROSS PRODUCT
    // asm: 	MPYF	*AR2,R1,R1
    // asm: 	MPYF	*+AR2(1),R0,R0
    // asm: 	SUBF	R0,R1 			;THIS CROSS PRODUCT SIGN
    // asm: 	PUSHF	R1			;SAVE THE SIGN DUDES
    // *GET ROTATIONAL INTENSITY
    // *NORMALIZE VECTORS
    // *NORMALIZE RADIUS VECTOR
    // asm: 	MPYF	*AR2,*AR2,R3
    // asm: 	MPYF	*+AR2(1),*+AR2(1),R1
    // asm: 	ADDF	R1,R3,R2
    // asm: 	CALL	SQRT
    // asm: 	PUSHF	R0			;SAVE THE RADIUS LENGTH
    // asm: 	CALL	INV_F30
    // asm: 	MPYF	*AR2,R0,R1
    // asm: 	STF	R1,*AR2
    // asm: 	MPYF	*+AR2(1),R0,R1
    // asm: 	STF	R1,*+AR2(1)
    // *NORMALIZE IMPACT VECTOR
    // asm: 	LDF	*+AR2(2),R1
    // asm: 	MPYF	R1,R1,R3
    // asm: 	LDF	*+AR2(3),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R3,R2
    // asm: 	CALL	SQRT
    // asm: 	CALL	INV_F30
    // asm: 	LDF	R0,R1
    // asm: 	MPYF	*+AR2(2),R0
    // asm: 	MPYF	*+AR2(3),R1
    // *COMPUTE DOT PRODUCT TO GET ANGLE
    // *COSINE=DOT PRODUCT
    // asm: 	MPYF	*AR2,R0
    // asm: 	MPYF	*+AR2(1),R1
    // asm: 	ADDF	R0,R1			;R3=DOT PRODUCT
    // asm: 	ABSF	R1
    // asm: 	SUBRF	1.0,R1
    // asm: 	POPF	R0			;GET RADIUS LENGTH
    // asm: 	MPYF	R0,R1
    // asm: 	MPYF	2,R1			;FUDGE FACTOR
    // asm: 	POPF	R0			;GET SIGN
    // asm:        	LDFN	-15.0,R0		;LOAD FUDGE FACTOR
    // asm:        	LDFNN	15.0,R0
    // asm: 	MPYF	R1,R0			;DO IT DUDE
    // asm: 	LDF	*+AR5(CARMASS),R1	;ADJUST FOR MASS
    // asm: 	CMPF	2.0,R1
    // asm: 	LDFGE	10.0,R1			;HEAVY MASS ADJUSTMENT
    // asm: 	CALL	DIV_F
    // asm: 	STF	R0,@SPINTEMP		;SPIN TIME TEMP
    // *
    // *GET THE SPIN TIME
    // *
    // *R0	ROTATION SPEED (FLOAT)
    // *AR1	OBJECT
    // *AR5	CAR BLOCK POINTER
    // *
    // asm: 	POPF	R3			;GET INTENSITY OF COLLISION
    // asm: 	STF	R3,@COLVEL		;COLLISION RELATIVE VELOCITY
    // asm: 	LDI	*+AR5(CAR_SPIN),R1	;CHECK IF ALREADY SPINNING
    // asm: 	CMPI	1,R1
    // asm: 	BZ	SPINNIT			;YES...
    // asm: 	CMPI	@PLYCAR,AR1		;PLAYERS CAR?
    // asm: 	BNZ	DRONESPIN
    // *PLAYER SPIN
    // *
    // *R0	ROTATION SPEED (FLOAT)
    // *R3	COLLSION RELATIVE SPEED
    // *AR1	OBJECT
    // *AR5	CAR BLOCK POINTER
    // *
    // asm: PLYRSPIN
    // asm: 	CALL	BEHINDCK		;CHECK IF PLAYER HIT FROM BEHIND
    // asm: 	CMPF	50,R3
    // asm: 	BGT	PLSPIN1			;BIG BUMP...
    // asm: 	CALL	CKBOUNCE
    // asm: 	BNC	SPINBUMP
    // asm: 	B	SPINBOUNCE
PLSPIN1:
    // asm: 	CMPF	100,R3
    // asm: 	BGT	PLBIG			;SPIN, RELATIVE VELOCITY LARGE
    // asm: 	LDI	500,AR2			;SPIN PROBABILITY
    // asm: 	CALL	RANDPER
    // asm: 	BC	PLSPIN2	      		;NORMAL SPIN
    // asm: 	CALL	CKBOUNCE
    // asm: 	BC	SPINBOUNCE	   	;DO A BOUNCE
PLSPIN2:
    // asm: 	LDI	@CAMVIEW,R2
    // asm: 	LDINZ	150,AR2
    // asm: 	LDIZ	100,AR2			;LESS SPIN 1ST PERSON
    // asm: 	CALL	RANDPER
    // asm: 	BC	PSPINNIT       		;NORMAL SPIN
    // asm: 	B	SPINBUMP
PLBIG:
    // asm: 	LDI	@CAMVIEW,R2
    // asm: 	LDINZ	500,AR2
    // asm: 	LDIZ	250,AR2			;LESS SPIN 1ST PERSON
    // asm: 	CALL	RANDPER
    // asm: 	BC	PSPINNIT       		;NORMAL SPIN
    // asm: 	CALL	CKBOUNCE
    // asm: 	BC	SPINBOUNCE	   	;DO A BOUNCE
    // asm: 	B	SPINBUMP
    // *
    // *CHECK IF PLAYER HIT FROM BEHIND
    // *
    // asm: BEHINDCK
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R3
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR0
    // asm: 	LDI	AR0,AR2
    // asm: 	LDI	AR4,AR3
    // asm:       	LDI	AR1,AR4
    // asm: 	CALL	CKAHEAD	       		;IS PLAYER AHEAD?
    // asm: 	LDIGE	0,R0			;NO
    // asm: 	LDILT	1,R0			;YES
    // asm: 	STI	R0,@PLYRBEHIND
    // asm: 	POP	AR0
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POPF	R3
    // asm: 	POPF	R0
    // asm: 	RETS
    // *
    // *CHECK PLAYER SPINNOUT
    // *
PSPINNIT:
    // asm: 	LDI	@PLYRBEHIND,R2
    // asm: 	BNE	SPINBUMP		;YES, JUST BUMP THE DUDE
    // asm: 	B 	SPINNIT			;NO, SPIN 'EM OUT
SPINBOUNCE:
    // asm: 	LDF	*+AR5(CARSPEED),R4
    // asm: 	CMPF	20,R4		      	;MINIMUM SPEED VALUE
    // asm: 	LDFLT	20,R4
    // asm: 	STF	R4,*+AR5(CARSPEED)    	;REVERSE SPEED
    // asm: 	MPYF	1.5,R4
    // asm: 	FIX	R4,R0			;BOUNCE TIME
    // asm: 	CMPI	60,R0
    // asm: 	LDIGT	60,R0			;MAX AT 40
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
    // asm: 	FLOAT	R0,R1
    // asm: 	LDF	R2,R0	  		;MOVE TO MIDDLE
    // asm: 	CALL	DIV_F
    // asm: 	MPYF	-6.0,R0
    // ;	LDF	R2,R2			;CORRECTION FACTOR
    // ;	LDFGT	-0.04,R0
    // ;	LDFLE	0.04,R0
    // ;;	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARDROT)
    // asm: 	LDF	0,R1			;CARSPRAD
    // asm: 	B	SPINXX
    // *DRONE SPIN
    // *R0	ROTATION SPEED (FLOAT)
    // *R3	COLLSION RELATIVE SPEED
DRONESPIN:
    // asm: 	ABSF	R0,R1	     		;COMPUTE SPIN PROBABILITY
    // asm: 	CMPF	0.1,R1
    // asm: 	BLT	SPINBUMP		;NO SPIN, TOO SMALL
    // asm: 	CMPF	30,R3
    // asm: 	BLT	SPINBUMP		;NO SPIN, RELATIVE VELOCITY SMALL
    // asm: 	CMPF	140,R3
    // asm: 	BGT	DSPIN			;SPIN, RELATIVE VELOCITY LARGE
    // asm: 	FIX	R3,AR2			;GET SPIN PROBABILITY
    // asm: 	SUBI	30,AR2
    // asm: 	MPYI	6,AR2
    // asm: 	ADDI	160,AR2
    // asm: 	CALL	RANDPER
    // asm: 	BNC	SPINBUMP
    // *
    // *DRONE TOTAL SPINOUT
    // *
DSPIN:
    // asm: 	ABSF	@SPINTEMP,R2     	;GET SPIN MAGNITUDE
    // asm: 	CMPF	10,R2
    // asm: 	LDFGT	10,R2
    // asm: 	MPYF	0.1,R2
    // asm: 	MPYF	0.08,R2
    // asm: 	LDF	0.06,R0	     		;GET SOME RANDOMNESS IN SPIN RATE
    // asm: 	CALL	FRAND
    // asm: 	ADDF	0.04,R0
    // asm: 	ADDF	R0,R2
    // asm: 	LDF	@SPINTEMP,R0		;GET SIGN (SPIN DIRECTION)
    // asm: 	BNN	DSPIN1
    // asm: 	NEGF	R2
DSPIN1:
    // asm: 	LDF	6.28,R0			;ONCE OR TWICE AROUND
    // asm: 	CALL	FRAND
    // asm: 	LDF	3.14,R1			;CARSPRAD
    // asm: 	ADDF	R0,R1
    // asm: 	LDF	*+AR5(CARMASS),R0  	;HEAVY MASS ?
    // asm: 	CMPF	2.0,R0
    // asm: 	BLT	DSPIN0			;NOPE
    // asm: 	MPYF	0.5,R2			;CUT DOWN SPIN SPEED
    // asm: 	LDF	3.14,R1			;CARSPRAD
DSPIN0:
    // asm: 	CALL	ANGMOM
    // asm: 	STF	R2,*+AR5(CARDROT)
    // asm: 	LDI	1,R0   			;SET RADIAN SPIN FLAG
    // asm: 	B	SPINX
    // *
    // * SPINNIT: TOTAL SPINOUT
    // *SET RANGE TO .08 -.12 DUDES
    // *
SPINNIT:
    // asm: 	ABSF	@SPINTEMP,R2     	;GET SPIN MAGNITUDE
    // asm: 	CMPF	10,R2
    // asm: 	LDFGT	10,R2
    // asm: 	MPYF	0.1,R2
    // asm: 	MPYF	0.08,R2
    // asm: 	ADDF	0.1,R2
    // asm: 	LDF	@SPINTEMP,R0		;GET SIGN (SPIN DIRECTION)
    // asm: 	BNN	SPINTM0
    // asm: 	NEGF	R2
SPINTM0:
    // asm: 	CALL	ANGMOM
    // asm: 	CMPI	@PLYCAR,AR1		;PLAYERS CAR?
    // asm: 	LDFZ	0.08,R1			;PLAYER MIN
    // asm: 	LDFNZ	0.02,R1			;DRONE MIN
    // asm: 	ABSF	R2,R0  			;MINIMUM SPIN RATE FOR PLAYER
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	SPINTM1
    // asm: 	LDF	R2,R2
    // asm: 	LDFLT	-1,R2
    // asm: 	LDFGE	1,R2
    // asm: 	MPYF	R1,R2
SPINTM1:
    // asm: 	STF	R2,*+AR5(CARDROT)
    // asm: 	LDI	*+AR5(CAR_SPIN),R1	;CHECK IF ALREADY SPINNING
    // asm: 	CMPI	1,R1
    // asm: 	LDFZ	*+AR5(CARSPRAD),R1	;YES...	SAVE CARSPRAD
    // asm: 	LDFNZ	3.14,R1			;NO NEW CARSPRAD
    // asm: 	LDI	1,R0
    // asm: 	B	SPINX
    // *
    // *MOMENTARY BUMP SPIN
    // *R3=RELATIVE VELOCITY OF HIT
    // *
SPINBUMP:
    // asm: 	LDF	@SPINTEMP,R2
    // asm: 	CMPF	0.05,R2
    // asm: 	LDFGT	0.05,R2
    // asm: 	CMPF	-0.05,R2
    // asm: 	LDFLT	-0.05,R2
    // asm: 	CALL	ANGMOM			;ADJUST ANGULAR MOMENTUM
    // asm: 	CMPF	80,R3
    // asm: 	LDFGT	80,R3
    // asm: 	MPYF	0.0125,R3		;ADJUST	ROTATE FOR REL VEL
    // asm: 	MPYF	R3,R2
    // asm: 	LDF	@COLVEL,R0
    // asm: 	MPYF	0.4,R0
    // asm: 	CMPF	35,R0
    // asm: 	LDFGT	35,R0
    // asm: 	CALL	FRAND
    // asm: 	CMPI	@PLYCAR,AR1		;ARE WE PLAYERS CAR?
    // asm: 	BNE	SPINB0
    // asm: 	MPYF	0.7,R0			;LESS SPIN FOR PLAYER
SPINB0:
    // asm: 	CMPF	8,R0
    // asm: 	LDFLT	8,R0
    // asm: 	FIX	R0
    // asm: 	CMPI	@PLYCAR,AR0		;HIT BY PLAYERS CAR?
    // asm: 	BNZ	SPINBUMP1		;NO
    // asm: 	ADDI	5,R0			;YES, BOOST SPIN TIME
SPINBUMP1:
    // asm: 	LDI	@PLYRBEHIND,R1		;REDUCE FOR PLAYER HIT BEHIND
    // asm: 	BZ	SPINBX
    // asm: 	CMPI	14,R0			;MAX TIME AT 14
    // asm: 	LDIGT	14,R0
    // asm: 	LSH	-1,R0			;DIVIDE TIME BY 1/2
SPINBX:
    // asm: 	STF	R2,*+AR5(CARDROT)
    // asm: 	LDF	0,R1			;CARSPRAD
SPINX:
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
SPINXX:
    // asm: 	STF	R1,*+AR5(CARSPRAD)
    // asm: 	LDF	*+AR1(OMAT11),R0	;IF CAR FLIPPED, REVERSE CARDROT
    // asm: 	BGE	SPINXXX
    // asm: 	NEGF	*+AR5(CARDROT),R0
    // asm: 	STF	R0,*+AR5(CARDROT)
SPINXXX:
    // asm: 	RETS
    // *
    // *ANGULAR MOMENTUM
    // *R2=NEW ANGULAR MOMENTUM
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPINROT", 0, 0);
    UNIMPL();
}

void ANGMOM(void)
{
    // asm: 	LDI	*+AR5(CAR_SPIN),R3 	;ADD IN EXISTING INERTIA
    // asm: 	BZ	ANGM1
    // asm: 	LDF	*+AR5(CARDROT),R3	;GET OLD MOMENTUM
    // asm: 	MPYF	0.5,R3	     		;FUDGE FACTOR
    // asm: 	ADDF	R3,R2
    // asm: 	MPYF	0.67,R2
ANGM1:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ANGMOM", 0, 0);
    UNIMPL();
}

void COLCHK(void)
{
    // *----------------------------------------------------------------------------
    // *DISPATCH COLLISION
    // *----------------------------------------------------------------------------
    // *CHECK COLLISION BETWEEN 2 OBJECTS
    // *
    // *PARAMETERS
    // *	AR0	OBJECT 1
    // *	AR1	OBJECT 2
    // *RETURNS
    // *	CARRY SET FOR COLLISION
    // *
    // *BLOWLIST ALLOCATIONS
    // *BLOWLIST+00 : OBJ0 BOX PTS (ORIGINAL)
    // *BLOWLIST+24 : OBJ0 BOX PTS (ROTATED, TRANLSATED)
    // *BLOWLIST+48 : OBJ1 BOX PTS (ORIGINAL)
    // *BLOWLIST+72 : OBJ1 BOX PTS (ROTATED, TRANSLATED)
    // *BLOWLIST+96 : OBJ0 PLANE EQUATIONS
    // *BLOWLIST+120: OBJ1 PLANE EQUATIONS
    // asm:  	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R5
    // asm: 	PUSH	IR0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDPI	@BLOWLISTI,AR2
    // asm: 	CALL	GETBOX			;GET BOX POINTS FOR OBJECT 1
    // asm: 	LDI	AR1,AR0
    // asm: 	CALL	GETBOX			;GET BOX POINTS FOR OBJECT 2
    // *GET 6 EQUATIONS FOR BOX
    // *GET PLANE EQUATION
    // *4 COEFFICIENTS PER EQUATION
    // *N1,N2,N3,P.N WHERE N=NORMAL VECTOR, P= 1ST POINT IN LIST
    // *
    // asm: 	LDI	AR2,AR0			;STORE NORMAL VECTORS HERE
    // asm: 	LDPI	@EQTABI,AR2
    // asm: 	LDI	11,RC 			;DO 2 X 6 EQUATIONS
    // asm: 	RPTB	PLANEQ
    // asm: 	PUSH	AR2
    // asm: 	CALL	GEN_NORMAL		;GETS NORMAL VECTOR
    // asm: 	POP	AR2
    // asm: 	LDI	*AR2++(3),AR3		;GET FIRST POINT
    // asm: 	MPYF	*AR0++,*AR3++,R0   	;COMPUTE DOT PRODUCT
    // asm: 	MPYF	*AR0++,*AR3++,R1
    // asm: 	MPYF	*AR0++,*AR3++,R0
    // asm: 	ADDF	R0,R2
PLANEQ:
    // asm: STF	R2,*AR0++
    // *CHECK POINTS OBJ1 VS EQ OBJ0
    // asm: 	SUBI	48,AR0			;GET OBJ 0 EQUATION BASE ADDR
    // asm: 	LDI	AR0,AR3
    // asm: 	SUBI	23,AR3			;GET INDEX PROJ POINTS OBJ 1+1
    // asm: 	LDI	7,AR4 			;DO 8 POINTS
    // asm: PNTCKL0
    // asm: 	LDI	AR0,AR2
    // asm: 	LDI	5,RC 			;DO 6 EQUATIONS
    // asm: 	RPTB	EQCHK0
    // asm: 	MPYF	*AR2++,*-AR3(1),R0
    // asm: 	MPYF	*AR2++,*AR3,R1
    // asm: 	MPYF	*AR2++,*+AR3(1),R0
    // asm: 	ADDF	R0,R2
    // asm: 	CMPF	*AR2++,R2
    // asm: 	BLT	PNTNXT0			;THIS POINT FAILED, GET A NEW ONE
EQCHK0:
    // asm: NOP
    // asm: 	BU	GOTCOL			;GOT A COLLISION
PNTNXT0:
    // asm: 	NOP	*AR3++(3)
    // asm: 	DBU	AR4,PNTCKL0
    // *CHECK POINTS OBJ0 VS EQ OBJ1
    // asm: 	ADDI	24,AR0			;GET OBJ1 EQUATION BASE
    // asm: 	LDI	AR0,AR3
    // asm: 	SUBI	95,AR3			;GET INDEX PROJ POINTS OBJ 1+1
    // asm: 	LDI	7,AR4 			;DO 8 POINTS
    // asm: PNTCKL1
    // asm: 	LDI	AR0,AR2
    // asm: 	LDI	5,RC 			;DO 6 EQUATIONS
    // asm: 	RPTB	EQCHK1
    // asm: 	MPYF	*AR2++,*-AR3(1),R0
    // asm: 	MPYF	*AR2++,*AR3,R1
    // asm: 	MPYF	*AR2++,*+AR3(1),R0
    // asm: 	ADDF	R0,R2
    // asm: 	CMPF	*AR2++,R2
    // asm: 	BLT	PNTNXT1			;THIS POINT FAILED, GET A NEW ONE
EQCHK1:
    // asm: NOP
    // asm: 	BU	GOTCOL			;GOT A COLLISION
PNTNXT1:
    // asm: 	NOP	*AR3++(3)
    // asm: 	DBU	AR4,PNTCKL1
    // asm: 	CLRC				;NO COLLISION
    // asm: 	POP	AR1
    // asm: 	POP	AR0
COLCHKX:
    // asm: 	POP	IR0
    // asm: 	POP	R5
    // asm: 	POP	R3
    // asm:  	POP	R2
    // asm: 	RETS
GOTCOL:
    // asm: 	POP	AR1  			;GET COLLIDING OBJECTS
    // asm: 	POP	AR0
    // asm: 	SETC
    // asm: 	BU	COLCHKX
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLCHK", 0, 0);
    UNIMPL();
}

void GETBOX(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *PROJECT BOX POINTS FOR OBJECT AR0
    // *R0=SIZING PARAMETER (1.00=TRUE SIZE)
    // *AR0= OBJECT
    // *AR2= STORAGE AREA FOR POINTS AND PROJECTION (48 WORDS)
    // *RET AR2= END OF STROAGE AREA+1
    // *TRASHES R0-R5
    // *
    // asm: 	LDF	1.0,R0		;XMINUS MULT FACTOR
    // asm: 	LDF	1.0,R1		;YMINUS MULT FACTOR
    // asm: 	LDF	1.0,R2		;ZMINUS MULT FACTOR
    // asm: 	LDF	1.0,R3		;XPLUS MULT FACTOR
    // asm: 	LDF	1.0,R4		;YPLUS MULT FACTOR
    // asm: 	LDF	1.0,R5		;ZPLUS MULT FACTOR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETBOX", 0, 0);
    UNIMPL();
}

void GETBOX0(void)
{
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // *GET 8 POINTS FOR OBJ 0
    // asm: 	LDI	*+AR0(OCARBLK),AR5
    // asm: 	MPYF	*+AR5(CARXMINUS),R0
    // asm: 	MPYF	*+AR5(CARYMINUS),R1
    // asm: 	MPYF	*+AR5(CARZMINUS),R2
    // asm: 	MPYF	*+AR5(CARXPLUS),R3
    // asm: 	MPYF	*+AR5(CARYPLUS),R4
    // asm: 	MPYF	*+AR5(CARZPLUS),R5
    // asm: 	LDI	2,IR0
    // asm: 	PUSH	AR2
    // asm: 	NOP	*AR2++
    // asm: 	STF	R0,*-AR2(1)    	;X1
    // asm: 	STF	R2,*-AR2(1)	;Z1
    // asm: 	STF	R1,*-AR2(1)	;Y2
    // asm: 	STF	R0,*-AR2(1)    	;X3
    // asm: 	STF	R2,*-AR2(1)	;Z3
    // asm: 	STF	R4,*-AR2(1)	;Y4
    // asm: 	STF	R0,*-AR2(1)    	;X5
    // asm: 	STF	R5,*-AR2(1)	;Z5
    // asm: 	STF	R1,*-AR2(1)	;Y6
    // asm: 	STF	R0,*-AR2(1)    	;X7
    // asm: 	STF	R5,*-AR2(1)	;Z7
    // asm: 	STF	R4,*-AR2(1)	;Y8
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
    // asm: 	LDI	AR0,AR5
    // asm: 	ADDI	OMATRIX,AR5		;rotational matrix
    // asm: 	POP	AR4			;GET POINTER TO POINT LIST
    // asm: 	SUBI	2,AR4			;SETUP AT -2
    // asm: 	LDPI	@transvectorYI,AR6	;transvector temp store
    // asm: 	LDI	8,IR0
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	LDF	*+AR0(OPOSY),R1
    // asm: 	STF	R0,*-AR6(1)		;transvector.x
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR6(1)		;transvector.z
    // asm: 	LDI	7,RC
    // asm: 	RPTB	EOCV
    // asm: 	LDF	*++AR4(3),R4
    // *
    // *MULTIPLY BY ROTATION MATRIX
    // *AND ADD TRANSLATION (IN THAT ORDER)
    // *
    // asm: 	MPYF3	*AR5++,*-AR4(1),R0
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm: 	MPYF3	*AR5++,*-AR4(1),R0
    // asm: 	ADDF	*-AR6(1),R2		;*blowlist++   += translation[X]
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5++,*+AR4(1),R1
    // asm: 	MPYF3	*AR5++,*-AR4(1),R0
    // asm: 	ADDF	*AR6,R3			;*blowlist++   += translation[Y]
    // asm: 	MPYF3	*AR5++,R4,R1
    // asm: 	MPYF3	*AR5--(IR0),*+AR4(1),R1
    // asm: 	ADDF	R1,R2			;FORM ROTATED Z
    // asm: 	ADDF	*+AR6(1),R2		;ADD IN TRANSLATION Z
EOCV:
    // asm: STF	R2,*AR2++
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETBOX0", 0, 0);
    UNIMPL();
}

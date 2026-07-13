
#include "rroad.h"
#include "../core/machine.h"
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
#include "vunit.h"

/*
 * Source module: asm/RROAD.ASM
 */

void RRSTART_ENGINE(void);
void RRSTART_BOXCAR(void);
static void CARFORWARD(void);
static void CARFORWARD_ENGINE(void);
void RAMPDOWNTRAINSND(void);
static void RAMPDOWNTRAINSND_PROC(void);
void FLYTRAIN(void);
static void FLYTRAINP(void);
void DEADCARA(void);
void GETFLYMAT_TRAIN(void);

#define FLYTRAINPI FLYTRAINP

/* asm: FREEZE_IT	.bss	FREEZE_IT,1 */
int FREEZE_IT;
/* asm: COLL_POS	.bss	COLL_POS,3 */
static int COLL_POS[3];
/* asm: THE_TRAIN_SPEED	.bss	THE_TRAIN_SPEED,1 */
int THE_TRAIN_SPEED;
/* asm: KILL_TRAIN	.bss	KILL_TRAIN,1 */
int KILL_TRAIN;
/* asm: COLL_DIST	.float	117465.0	;distance to collision from engine start */
/* asm: 	 */
static float COLL_DIST = 117465.0f;
#define TRAIN_SPEED 240
#define TRAIN_SPEED_MIN 170
#define TRAIN_SPEED_MAX 280

// *----------------------------------------------------------------------------
void RRSTART_ENGINE(void) {
    // asm 00009B6E: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00009B6F: 	RETSNZ
    // asm 00009B70: 	LONGROUT
    // asm: 	PUSHFL	R0
    // asm: 	PUSHFL	R1
    // asm: 	PUSHFL	R2
    // asm: 	PUSHFL	R3
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@FREEZE_IT
    // asm: 	CREATEL	CARFORWARD_ENGINE,DRONE_C|RAILROAD|1
    // asm: 	STI	AR0,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR0(PDATA)
    // 	;compute the collision point
    // asm: 	LDF	@COLL_DIST,R2
    // asm: 	LDI	@VECTORBI,AR2
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	STF	R2,*+AR2(Z)
    // asm: 	LDF	*+AR4(ORADY),R2
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR2,R2
    // asm: 	LDI	@VECTORBI,AR2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,@COLL_POS+X
    // ;	LDF	*+AR2(Y),R0
    // ;	ADDF	*+AR4(OPOSY),R0
    // ;	STF	R0,@COLL_POS+Y
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,@COLL_POS+Z
    // asm: 	FLOAT	TRAIN_SPEED,R0
    // asm: 	STF	R0,@THE_TRAIN_SPEED
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POPFL	R3
    // asm: 	POPFL	R2
    // asm: 	POPFL	R1
    // asm: 	POPFL	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RRSTART_ENGINE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void RRSTART_BOXCAR(void) {
    // asm 00009B71: 	LDI	@HEAD2HEAD_ON,R0
    // asm 00009B72: 	RETSNZ
    // asm 00009B73: 	LONGROUT
    // asm: 	PUSH	R0
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR2
    // asm: 	CREATEL	CARFORWARD,DRONE_C|RAILROAD
    // asm: 	STI	AR0,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR0(PDATA)
    // asm: 	POP	AR2
    // asm: 	POP	R2
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RRSTART_BOXCAR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CARFORWARD(void) {
    // asm 00009B74: 	LDI	DRONE_C|RAILROAD|1,R0
    // asm 00009B75: 	LDI	-1,R1
    // asm 00009B76: 	CALL	PRC_FIND
    // asm 00009B77: 	LDI	AR0,AR2
    // asm 00009B78: 	CALL	PRC_FOLLOW		;FOLLOW THE ENGINE!
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARFORWARD", 0, 0);
    UNIMPL();
}

static void CARFORWARD_ENGINE(void) {
    // asm 00009B79: 	LDI	*+AR4(OID),IR0
    // asm 00009B7A: 	AND	0Fh,IR0
    // asm 00009B7B: 	STI	IR0,*+AR4(OUSR1)	;SAVE INDEX
    // asm 00009B7C: 	LDI	13,R0
    // asm 00009B7D: 	CALL	_CARV0			;RETURNS BLOCK PTR IN AR0
    // asm 00009B7E: 	BC	KKG
    // asm 00009B7F: 	LDI	1,R0
    // asm 00009B80: 	LS	O_PROC_B,R0
    // asm 00009B81: 	LDI	*+AR4(OFLAGS),R1
    // asm 00009B82: 	ANDN	R0,R1
    // asm 00009B83: 	STI	R1,*+AR4(OFLAGS)
    // asm 00009B84: 	DIE
KKG:
    // asm 00009B85: LDI	AR0,AR5
    // asm 00009B86: 	LDI	DRONE_C|RAILROAD,R0
    // asm 00009B87: 	STI	R0,*+AR4(OID)
    // asm 00009B88: 	STI	R0,*+AR5(CAR_ID)
    // asm 00009B89: 	STI	R0,*+AR7(PID)
    // asm 00009B8A: 	CALL	ADD_DRONE
    // asm 00009B8B: 	LDI	*+AR4(OLINK2),R0
    // asm 00009B8C: 	ADDI	0500h,R0
    // asm 00009B8D: 	STI	R0,*+AR7(PDATA+2)
    // asm 00009B8E: 	CLRI	R0
    // asm 00009B8F: 	STI	R0,*+AR4(OLINK2)	;do not delete
    // asm 00009B90: 	CLRF	R0
    // asm 00009B91: 	STF	R0,*+AR5(CARBRAKE)
    // asm 00009B92: 	LDF	*+AR5(CARRDFR),R0
    // asm 00009B93: 	STF	R0,*+AR5(CAROFRDFR)
    // asm 00009B94: 	LDF	*+AR4(ORADY),R2
    // asm 00009B95: 	CALL	NORMITS
    // asm 00009B96: 	STF	R2,*+AR4(ORADY)
    // asm 00009B97: 	STF	R2,*+AR5(CARYROT)
    // asm 00009B98: 	STF	R2,*+AR5(CARVROT)
    // asm 00009B99: 	LDI	AR4,AR2
    // asm 00009B9A: 	ADDI	OMATRIX,AR2
    // asm 00009B9B: 	CALL	FIND_YMATRIX
    // *Initialize the distance to collision point
    // asm 00009B9C: 	LDF	@COLL_DIST,R2
    // asm 00009B9D: 	MPYF	2,R2
    // asm 00009B9E: 	STF	R2,*+AR7(DELTA_PLYRDIST)
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARFORWARD_ENGINE", 0, 0);
    UNIMPL();
}

// *CHECK IF WAY PAST CROSSING

/*
;
;	find ETA to collision with plyr, and with this vehicle
;
*/

// ;	FLOAT	TRAIN_SPEED,R0

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void RAMPDOWNTRAINSND(void) {
    // asm 00009C15: 	CREATE	RAMPDOWNTRAINSND_PROC,UTIL_C
    // asm 00009C18: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMPDOWNTRAINSND", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void RAMPDOWNTRAINSND_PROC(void) {
    // asm 00009C19: 	LONGROUT
    // asm: 	LDI	255,R5
RAMPDOWNTRAINSND_LP:
    // asm: 	SUBI	5,R5
    // asm: 	BLT	RAMPDOWNTRAINSND_DEATH
    // asm: 	LDI	R5,R2
    // asm: 	LDI	RR_TRAIN,AR2
    // asm: 	CMPI	@SNDSTR+SND_SIZ+SND_IDX,AR2	;CHECK TRACK1
    // asm: 	BEQ	RDT1
    // asm: 	CMPI	@SNDSTR+(2*SND_SIZ)+SND_IDX,AR2	;CHECK TRACK2
    // asm: 	BNE	RDTX				;SOUND NOT ACTIVE, LEAVE...
RDT1:
    // asm: 	CALL	AMBIENCE_SOUND
    // asm: 	SLEEP	1
    // asm: 	BR	RAMPDOWNTRAINSND_LP
RAMPDOWNTRAINSND_DEATH:
    // asm: 	LDI	RR_TRAIN,AR2
    // asm: 	CALL	KILLSNDFX
RDTX:
    // asm: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMPDOWNTRAINSND_PROC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *FLYING TRAIN WRECK
 *
 *	AR0	POINTS TO PLAYER CAR
 *	AR1	POINTS TO CAR TO SEND FLYING
 *	AR4	POINTS TO PLAYER CAR BLOCK
 *	AR5	POINTS TO DRONE CAR BLOCK
 */
void FLYTRAIN(void) {
    // asm 00009C1A: 	LONGROUT
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // ;	SONDFX	TRCOL1SND		;TRAIN CRASH SOUND
    // asm: 	SONDFX	TRCOL2SND		;TRAIN CRASH SOUND
    // asm: 	LDI	*+AR0(OCARBLK),AR3
    // asm: 	LDI	*+AR1(OCARBLK),AR5
    // asm: 	LDF	*+AR3(CARSPEED),R0
    // asm: 	FLOAT	150,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	IS_FLYING
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR5(CAR_BUMP)
    // asm: 	BU	FLY1A
IS_FLYING:
    // asm: 	LDI	@PLYCAR,AR0
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	TSTB	*+AR1(OFLAGS),R0	;PROCESS ALREADY ACTIVE	?
    // asm: 	BZ	FLYCAR0A
    // asm: 	LDI	*+AR1(OPLINK),R0	;YES, KILL OFF DRONE PROCESS
    // asm: 	BZ	FLYCAR0A
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PRC_KILL
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR1(OPLINK)
FLYCAR0A:
    // asm: 	LDI	*+AR0(OCARBLK),AR3	;GET PLAYER'S CAR
    // asm: 	LDI	*+AR1(OCARBLK),AR5	;GET DRONE TRAIN BLOCK
    // asm: 	LDI	AR1,AR4
    // asm: 	LDF	0.20,R0 		;ADD RANDOM ROTATION
    // asm: 	CALL	FRAND
    // asm: 	ADDF	*+AR5(CARVROT),R0
    // asm: 	STF	R0,*+AR5(CARVROT)
    // asm: 	LDF	-0.3,R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	-0.2,R0
    // asm: 	MPYF	1.5,R0			;SPEEDFUDGE FACTOR
    // asm: 	MPYF	*+AR3(CARSPEED),R0	;GET CURRENT SPEED
    // asm: 	CMPF	-65,R0
    // asm: 	LDFLT	-65,R0		  	;MAX VERTICAL VELOCITY
    // asm: 	STF	R0,*+AR1(OVELY)		;STUFF VERTICAL VELOCITY
    // asm: 	LDI	@FLYTRAINPI,AR2		;GET SIGN FLY PROCESS
    // asm: 	CALL	PRC_CREATE_CHILD	;CREATE A CHILD PROCESS
    // asm: 	STI	AR0,*+AR1(OPLINK)	;SAVE LINK
    // asm: 	LDI	1,R0
    // asm: 	LSH	O_PROC_B,R0		;PROCESS BIT MASK IN OBJECT STRUCT
    // asm: 	OR	*+AR1(OFLAGS),R0	;SET ATTACHED PROCESS FLAG
    // asm: 	STI	R0,*+AR1(OFLAGS)
    // asm: 	LDI	*+AR1(OLINK3),R0	;ANY MORE CARS???
    // asm: 	BZ 	FLYDONE			;NO
    // asm: 	LDI	R0,AR1
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	CMPI	DRONE_C|RAILROAD,R0
    // asm: 	BZ	IS_FLYING
    // *GET PLAYER CAR VELOCITY, SPIN
FLYDONE:
    // asm: 	LDF	*+AR3(CARSPEED),R0  	;CUT SPEED OF PLAYER
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR3(CARSPEED)
FLY1A:
    // asm: 	LDF	*+AR3(CARVROT),R0
    // asm: 	ADDF	PI,R0
    // asm: 	STF	R0,*+AR3(CARVROT)
    // asm: 	LDF	*+AR3(CARSPEED),R0
    // asm: 	CMPF	80,R0
    // asm: 	LDFLT	80,R0
    // asm: 	STF	R0,*+AR3(CARSPEED)
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR3(CAR_SPIN)
    // asm: 	LDF	12.0,R0
    // asm: 	STF	R0,*+AR3(CARSPRAD)
    // asm: 	LDF	0.1,R0
    // asm: 	STF	R0,*+AR3(CARDROT)
    // asm: FLYCARXA
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYTRAIN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

/*
 *
 *FLYING CAR PROCESS
 *AR4=DRONE CAR OBJECT
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
static void FLYTRAINP(void) {
    // *GET YOUR RADIANS
    // asm 00009C1C: 	LDF	0.2,R0
    // asm 00009C1D: 	CALL	SFRAND
    // asm 00009C1E: 	STF	R0,*+AR7(PDATA)	  	;Z RADIANS
    // asm 00009C1F: 	LDF	0.05,R0
    // asm 00009C20: 	CALL	SFRAND
    // asm 00009C21: 	STF	R0,*+AR7(PDATA+1)	;Y RADIANS
    // asm 00009C22: 	LDF	0,R0
    // asm 00009C23: 	STF	R0,*+AR7(PDATA+2)	;X RADIANS (unused)
    // asm 00009C24: 	LDF	0,R0
    // asm 00009C25: 	STF	R0,*+AR7(PDATA+3)	;Z RADIAN TOTAL
    // asm 00009C26: 	STF	R0,*+AR7(PDATA+5)  	;X RADIAN TOTAL
    // asm 00009C27: 	LDF	*+AR5(CARYROT),R0	;GET CAR Y ROT
    // asm 00009C28: 	STF	R0,*+AR7(PDATA+4)
FLYCARP0A:
    // asm 00009C29: 	LDI	*+AR4(ODIST),R0		;OUT OF RANGE ??
    // asm 00009C2A: 	CMPI	-6000,R0
    // asm 00009C2B: 	BLT	RROAD_DIE   		;END THIS FARCE
    // asm 00009C2C: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm 00009C2D: 	CALL	ROADSCAN
    // asm 00009C2E: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR DATA AREA
    // asm 00009C2F: 	FLOAT	@NFRAMES,R1	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm 00009C30: 	LDF	*+AR7(PDATA),R0		;ACCUMULATE X RADIANS
    // asm 00009C31: 	MPYF	R1,R0
    // asm 00009C32: 	ADDF	*+AR7(PDATA+3),R0
    // asm 00009C33: 	STF	R0,*+AR7(PDATA+3)
    // asm 00009C34: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // asm 00009C35: 	MPYF	R1,R0
    // asm 00009C36: 	ADDF	*+AR7(PDATA+4),R0
    // asm 00009C37: 	STF	R0,*+AR7(PDATA+4)
    // asm 00009C38: 	CALL	GETFLYMAT_TRAIN		;COMPUTE MATRICES
    // *CONVERT CARVROT,CARSPEED TO OVELX, OVELZ
    // asm 00009C39: 	LDF	*+AR5(CARVROT),R2
    // asm 00009C3A: 	ADDF	1.57,R2		   	;CORRECT FOR 90 DEGREE ERROR
    // asm 00009C3B: 	CALL	_SINE
    // asm 00009C3C: 	LDF	*+AR5(CARSPEED),R3
    // asm 00009C3D: 	MPYF	R3,R0
    // asm 00009C3E: 	STF	R0,*+AR4(OVELZ)	  	;CONVERT TO CARVROT, CARSPEED TO XZVEL
    // asm 00009C3F: 	CALL	_COSI
    // asm 00009C40: 	MPYF	R3,R0
    // asm 00009C41: 	STF	R0,*+AR4(OVELX)
    // asm 00009C42: 	CALL	OVELNADD		;UPDATE VELOCITIES
    // asm 00009C43: 	FLOAT	@NFRAMES,R2
    // asm 00009C44: 	MPYF	2,R2			;FRAME ADJUSTED GRAVITY
    // asm 00009C45: 	ADDF	*+AR4(OVELY),R2
    // asm 00009C46: 	STF	R2,*+AR4(OVELY)
    // asm 00009C47: 	FLOAT	300,R1		  	;GOING DOWN TOO MUCH?
    // asm 00009C48: 	CMPF	R1,R2
    // asm 00009C49: 	BGT	RROAD_DIE		;ABORT THE DUDE...
    // asm 00009C4A: 	CALL	GETTRAK
    // asm 00009C4B: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // asm 00009C4C: 	BN	FLYCARSLPA		;WERE GOING UP IGNORE IT
    // asm 00009C4D: 	CALL	BOXSCAN		      	;KEEP FALLING!!!
    // asm 00009C4E: 	BNC	FLYCARSLPA
    // *WERE OVER THE ROAD
    // asm 00009C4F: FLYCROADA
    // asm 00009C4F: 	LDF	R0,R0
    // asm 00009C50: 	BGT	FLYCARSLPA		;WERE ABOVE GROUND
    // *WE HIT THE GROUND DUDES
    // asm 00009C51: 	ADDF	*+AR4(OPOSY),R0		;SET HIM ON THE GROUND
    // asm 00009C52: 	STF	R0,*+AR4(OPOSY)
    // asm 00009C53: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // asm 00009C54: 	CMPF	35,R2          		;CHECK FOR MINIMUM
    // asm 00009C55: 	BLT	FLYCARSTOP0A    	;TIME TO STOP
    // asm 00009C56: FLYCROAD1A
    // asm 00009C56: 	MPYF	-0.5,R2
    // asm 00009C57: 	STF	R2,*+AR4(OVELY)
    // asm 00009C58: 	LDF     *+AR5(CARSPEED),R0	;CUT SPEED IN HALF
    // asm 00009C59: 	MPYF	0.5,R0
    // asm 00009C5A: 	STF	R0,*+AR5(CARSPEED)
    // asm 00009C5B: 	LDF	*+AR7(PDATA),R0		;CUT DOWN SPIN
    // asm 00009C5C: 	MPYF	0.5,R0
    // asm 00009C5D: 	STF	R0,*+AR7(PDATA)
    // asm 00009C5E: 	LDF	*+AR7(PDATA+1),R0
    // asm 00009C5F: 	MPYF	0.5,R0
    // asm 00009C60: 	STF	R0,*+AR7(PDATA+1)
    // *MAKE BOUNCE SOUND
    // asm 00009C61: 	LDI	@SCOLLTABI,AR2	   	;RANDOM COLLISION CRUNCH
    // asm 00009C62: 	LDI	3,R0
    // asm 00009C63: 	CALL	DRONESND
FLYCARSLPA:
    // asm 00009C64: 	SLEEP	1
    // asm 00009C66: 	B	FLYCARP0A
    // *ROTATE TO QUIESCENT STATE
    // *
FLYCARSTOP0A:
FLYCARSTOPA:
    // asm 00009C67: 	LDI	*+AR4(ODIST),R0		;OUT OF RANGE ??
    // asm 00009C68: 	CMPI	-6000,R0
    // asm 00009C69: 	BLT	RROAD_DIE   		;END THIS FARCE
    // asm 00009C6A: 	FLOAT	500,R0
    // asm 00009C6B: 	STF	R0,*+AR4(OVELY)		;FORCE ONTO GROUND
    // asm 00009C6C: 	LDF	*+AR5(CARVROT),R2
    // asm 00009C6D: 	ADDF	1.57,R2		   	;CORRECT FOR 90 DEGREE ERROR
    // asm 00009C6E: 	CALL	_SINE
    // asm 00009C6F: 	LDF	*+AR5(CARSPEED),R3
    // asm 00009C70: 	MPYF	R3,R0
    // asm 00009C71: 	STF	R0,*+AR4(OVELZ)	  	;CONVERT TO CARVROT, CARSPEED TO XZVEL
    // asm 00009C72: 	CALL	_COSI
    // asm 00009C73: 	MPYF	R3,R0
    // asm 00009C74: 	STF	R0,*+AR4(OVELX)
    // asm 00009C75: 	LDF	*+AR5(CARDROT),R0
    // asm 00009C76: 	STF	R0,*+AR7(PDATA+1)	;GET Y SPIN
    // asm 00009C77: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm 00009C78: 	CALL	ROADSCAN
    // asm 00009C79: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR DATA AREA
    // asm 00009C7A: 	LDI	@NFRAMES,RC	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm 00009C7B: 	SUBI	1,RC
    // asm 00009C7C: 	RPTB	FLYCSTLA
    // asm 00009C7D: 	LDF	*+AR4(OVELX),R0
    // asm 00009C7E: 	MPYF	0.98,R0			;DECAY VELOCITY
    // asm 00009C7F: 	STF	R0,*+AR4(OVELX)
    // asm 00009C80: 	ADDF	*+AR4(OPOSX),R0
    // asm 00009C81: 	STF	R0,*+AR4(OPOSX)
    // asm 00009C82: 	LDF	*+AR4(OVELY),R0
    // asm 00009C83: 	ADDF	*+AR4(OPOSY),R0
    // asm 00009C84: 	STF	R0,*+AR4(OPOSY)
    // asm 00009C85: 	LDF	*+AR4(OVELZ),R0
    // asm 00009C86: 	MPYF	0.98,R0			;DECAY VELOCITY
    // asm 00009C87: 	STF	R0,*+AR4(OVELZ)
    // asm 00009C88: 	ADDF	*+AR4(OPOSZ),R0
    // asm 00009C89: 	STF	R0,*+AR4(OPOSZ)
    // asm 00009C8A: 	LDF	*+AR7(PDATA),R0		;ACCUMULATE X RADIANS
    // asm 00009C8B: 	MPYF	0.96,R0  		;DAMP IT
    // asm 00009C8C: 	STF	R0,*+AR7(PDATA)		;CUT DOWN ROCK
    // asm 00009C8D: 	ADDF	*+AR7(PDATA+3),R0
    // asm 00009C8E: 	STF	R0,*+AR7(PDATA+3)
    // asm 00009C8F: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // asm 00009C90: 	MPYF	0.98,R0  		;DAMP IT
    // asm 00009C91: 	STF	R0,*+AR7(PDATA+1)	;CUT DOWN ROCK
    // asm 00009C92: 	STF	R0,*+AR5(CARDROT)	;STORE IN CAR STRUCTURE
    // asm 00009C93: 	ADDF	*+AR7(PDATA+4),R0
    // asm 00009C94: 	STF	R0,*+AR7(PDATA+4)
    // asm 00009C95: 	LDF	*+AR7(PDATA+3),R2	;CHECK TOTAL X RADIANS
    // asm 00009C96: 	CALL	NORMITS
    // asm 00009C97: 	ABSF	R2,R3
    // asm 00009C98: 	LDF	*+AR5(CARSPEED),R0	;DECAY SPEED
    // asm 00009C99: 	MPYF	0.98,R0
    // asm 00009C9A: 	STF	R0,*+AR5(CARSPEED)
    // *CHECK FOR DONE...
    // asm 00009C9B: 	CMPF	10,R0	       		;SPEED DECAYED?
    // asm 00009C9C: 	BGT	FLYCSTP0A	     	;NO, KEEP GOING...
    // asm 00009C9D: 	ABSF	*+AR5(CARDROT),R0	;GET ROTATE
    // asm 00009C9E: 	ABSF	*+AR7(PDATA),R1	    	;ADD IN ROCK
    // asm 00009C9F: 	ADDF	R0,R1
    // asm 00009CA0: 	MPYF	10,R1	    		;GET IN RANGE
    // asm 00009CA1: 	CMPF	0.02,R1			;PETERED OUT?
    // asm 00009CA2: 	BGT	FLYCSTP0A		;NO, KEEP GOING
    // asm 00009CA3: 	CMPF	0.2,R3			;RIGHT SIDE UP?
    // asm 00009CA4: 	BGT	FLYCSTP00A		;NOPE...
    // asm 00009CA5: 	LDF	0,R3			;STRAIGHTEN HIM UP!!!
    // asm 00009CA6: 	STF	R3,*+AR7(PDATA+3)
    // asm 00009CA7: 	LDI	2,R0
    // asm 00009CA8: 	B	FLYCCCA			;YES, TIME TO STOP
FLYCSTP00A:
    // asm 00009CA9: 	CMPF	2.95,R3			;UPSIDE DOWN?
    // asm 00009CAA: 	BLT	FLYCSTP0A		;NOPE
    // asm 00009CAB: FLYCSTPA
    // asm 00009CAB: 	LDI	1,R0	 		;WERE DONE DUDES...
    // asm 00009CAC: 	B	FLYCCCA
    // *ACCELERATE X ROTATION
FLYCSTP0A:
    // asm 00009CAD: 	LDF	R2,R2
    // asm 00009CAE: 	BN	FLYCSTP1A
    // asm 00009CAF: 	CMPF	1.57,R2
    // asm 00009CB0: 	B 	FLYCSTP2A
FLYCSTP1A:
    // asm 00009CB1: 	CMPF	-1.57,R2
FLYCSTP2A:
    // asm 00009CB2: 	LDFLT	-0.01,R0
    // asm 00009CB3: 	LDFGE	0.01,R0
    // asm 00009CB4: 	MPYF	0.4,R0
    // asm 00009CB5: 	ADDF	*+AR7(PDATA),R0
    // asm 00009CB6: 	LDF	*+AR7(PDATA),R1
    // asm 00009CB7: 	XOR	R0,R1,R2 		;CHECK FOR SIGN CHANGE- MAKE SOUND
    // asm 00009CB8: 	BNN	FLYCSTLA			;NO SOUND
    // asm 00009CB9: 	ABSF	*+AR7(PDATA+3),R2	;CHECK IF AMPLITUDE BIG ENOUGH
    // asm 00009CBA: 	CALL	NORMITS
    // asm 00009CBB: 	ABSF	R2
    // asm 00009CBC: 	CMPF	0.08,R2
    // asm 00009CBD: 	BLT	FLYCSTLA			;TOO SMALL OF A ROCK
    // asm 00009CBE: 	CMPF	3.06,R2
    // asm 00009CBF: 	BGT	FLYCSTLA			;TOO SMALL OF A UPSIDE DOWN ROCK
    // asm 00009CC0: 	LDI	BOTTOMOUT,AR2		;MAKE BOTTOMOUT SOUND
    // asm 00009CC1: 	PUSHF	R0
    // asm 00009CC2: 	CALL	DRONESND1
    // asm 00009CC3: 	POPF	R0
FLYCSTLA:
    // asm 00009CC4: STF	R0,*+AR7(PDATA)		;ACCELERATE ROTATION
    // asm 00009CC5: 	LDI	0,R0			;DONE FLAG
FLYCCCA:
    // asm 00009CC6: 	PUSH	R0
    // asm 00009CC7: 	CALL	GETTRAK
    // ;	CALL	DRONINBZ		;CHECK BOUNDS
    // asm 00009CC8: 	CALL	GETFLYMAT_TRAIN
    // asm 00009CC9: 	CALL	BOXSCAN		      	;KEEP FALLING!!!
    // asm 00009CCA: 	POP	R1			;CLEAN STACK
    // asm 00009CCB: 	BNC	FLYCARSTPA
    // asm 00009CCC: 	ADDF	*+AR4(OPOSY),R0		;SET HIM ON THE GROUND
    // asm 00009CCD: 	STF	R0,*+AR4(OPOSY)
    // asm 00009CCE: 	LDI	R1,R1			;DONE
    // asm 00009CCF: 	BNE	FLYCARSTPA		;YESSAH
    // asm 00009CD0: 	SLEEP	1
    // asm 00009CD2: 	B	FLYCARSTOPA
FLYCARSTPA:
    // asm 00009CD3: 	CMPI	2,R1
    // asm 00009CD4: 	BZ	DEADCARA		;RIGHT SIDE UP CARCASS
    // *
    // *CAR IS UPSIDE DOWN
    // *
    // asm 00009CD5: 	LDI	*+AR4(OID),R0
    // asm 00009CD6: 	ANDN	TYPE_M,R0
    // asm 00009CD7: 	OR	DEAD_VEH_T,R0
    // asm 00009CD8: 	LDF	0,R0			;CLEAR OUT THE SPEED
    // asm 00009CD9: 	STF	R0,*+AR5(CARSPEED)
    // *WAIT FOR OFFSCREEN
FLYCARWTA:
    // asm 00009CDA: 	SLEEP	1
    // asm 00009CDC: 	LDI	*+AR4(ODIST),R0	     	;OUT OF RANGE?
    // asm 00009CDD: 	CMPI	-6000,R0
    // asm 00009CDE: 	BLT	RROAD_DIE		;YES CLEAN IT UP...
    // asm 00009CDF: 	LDF	*+AR5(CARSPEED),R0	;ARE WE HIT???
    // asm 00009CE0: 	BZ	FLYCARWTA		;NO, JUST WAIT
    // *
    // *UPSIDE DOWN CAR IS HIT
    // *
    // asm 00009CE1: 	LDF	0.1,R0		       	;ROCK HIM A LITTLE
    // asm 00009CE2: 	CALL	SFRAND
    // asm 00009CE3: 	STF	R0,*+AR7(PDATA)	  	;X RADIANS RATE
    // asm 00009CE4: 	B	FLYCARSTOPA		;GO ROCK AND ROLL
    // *CLEAN UP THE MESS...
    // *
RROAD_DIE:
    // asm 00009CE5: 	CALL	FREE_DRONE
    // asm 00009CE6: 	LDI	1,R0
    // asm 00009CE7: 	LS	O_PROC_B,R0
    // asm 00009CE8: 	LDI	*+AR4(OFLAGS),R1
    // asm 00009CE9: 	ANDN	R0,R1
    // asm 00009CEA: 	STI	R1,*+AR4(OFLAGS)
    // asm 00009CEB: 	LDI	AR4,AR2
    // asm 00009CEC: 	CALL	OBJ_DELETE
    // asm 00009CED: 	LDI	AR5,AR2
    // asm 00009CEE: 	CALL	DELCAR
    // asm 00009CEF: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYTRAINP", 0, 0);
    UNIMPL();
}

/*
 *----------------------------------------------------------------------------
 *CAR IS RIGHT SIDE UP
 *
 */
void DEADCARA(void) {
    // asm 00009CF0: 	LDI	*+AR4(OID),R0
    // asm 00009CF1: 	ANDN	TYPE_M,R0
    // asm 00009CF2: 	OR	DEAD_VEH_T,R0
    // asm 00009CF3: 	STI	R0,*+AR4(OID)
    // asm 00009CF4: 	STI	R0,*+AR5(CAR_ID)
    // asm 00009CF5: 	STI	R0,*+AR7(PID)
    // asm 00009CF6: 	CLRF	R0
    // asm 00009CF7: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm 00009CF8: 	STF	R0,*+AR5(CARDROT)
    // asm 00009CF9: 	STF	R0,*+AR5(CARSPEED)
    // asm 00009CFA: 	LDF	1,R0
    // asm 00009CFB: 	STF	R0,*+AR5(CARBRAKE)
    // asm 00009CFC: 	LDF	*+AR7(PDATA+4),R0 	;GET Y ROTATION ORIENTATION
    // asm 00009CFD: 	STF	R0,*+AR5(CARVROT)
    // asm 00009CFE: 	STF	R0,*+AR5(CARYROT)
    // asm 00009CFF: 	LDI	0,R0
    // asm 00009D00: 	STI	R0,*+AR5(CAR_SPIN)	;CLEAR SPIN
DEADLPA:
    // asm 00009D01: 	LDI	*+AR4(ODIST),R0	      	;DIE OFF WHEN OFFSCREEN
    // asm 00009D02: 	CMPI	-6000,R0
    // asm 00009D03: 	BLT	RROAD_DIE
    // asm 00009D04: 	LDF	0,R2			;NO STEERING
    // asm 00009D05: 	CALL	DRONEGO
    // asm 00009D06: 	CALL	GETTRAK
    // asm 00009D07: DEADSLPA
    // asm 00009D07: 	SLEEP	1
    // asm 00009D09: 	B	DEADLPA
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEADCARA", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GET MATRIX FOR FLYING CAR
 *
 *
 *PARAMETERS
 *	PDATA+3,4,5 = Z,Y,XRAD		;PDATA+3,4,5 = X,Y,ZRAD
 *	AR4	OBJECT
 *	AR7	PROCESS
 *
 *TRASHES
 *	R2,R3,AR2
 *
 */
void GETFLYMAT_TRAIN(void) {
    // asm 00009D0A: 	LONGROUT
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	LDF	*+AR7(PDATA+3),R2
    // asm: 	CALL    FIND_ZMATRIX
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	LDF	*+AR7(PDATA+4),R2
    // asm: 	CALL    FIND_YMATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	BR	CONCATMAT		;FORMULATE COMBINED MATRIX
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETFLYMAT_TRAIN", 0, 0);
    UNIMPL();
}

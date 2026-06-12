#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "obj.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "delta.h"
#include "rroad.h"

/*
 * Source module: asm/RROAD.ASM
 */

/* asm: FREEZE_IT	.bss	FREEZE_IT,1 */
int FREEZE_IT;
/* asm: COLL_POS	.bss	COLL_POS,3 */
int COLL_POS[3];
/* asm: THE_TRAIN_SPEED	.bss	THE_TRAIN_SPEED,1 */
int THE_TRAIN_SPEED;
/* asm: KILL_TRAIN	.bss	KILL_TRAIN,1 */
int KILL_TRAIN;
/* asm: COLL_DIST	.float	117465.0	;distance to collision from engine start */
float COLL_DIST = 117465.0f;
#define TRAIN_SPEED 240
#define TRAIN_SPEED_MIN 170
#define TRAIN_SPEED_MAX 280
/* *----------------------------------------------------------------------------
 */

/* *----------------------------------------------------------------------------
 */
void RRSTART_ENGINE(void)
{
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	RETSNZ
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RRSTART_ENGINE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RRSTART_BOXCAR(void)
{
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	RETSNZ
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RRSTART_BOXCAR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void CARFORWARD(void)
{
    // asm: 	LDI	DRONE_C|RAILROAD|1,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_FIND
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	PRC_FOLLOW		;FOLLOW THE ENGINE!
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARFORWARD", 0, 0);
    UNIMPL();
}

void CARFORWARD_ENGINE(void)
{
    // asm: 	LDI	*+AR4(OID),IR0
    // asm: 	AND	0Fh,IR0
    // asm: 	STI	IR0,*+AR4(OUSR1)	;SAVE INDEX
    // asm: 	LDI	13,R0
    // asm: 	CALL	_CARV0			;RETURNS BLOCK PTR IN AR0
    // asm: 	BC	KKG
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	LDI	*+AR4(OFLAGS),R1
    // asm: 	ANDN	R0,R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // asm: 	DIE
KKG:
    // asm: LDI	AR0,AR5
    // asm: 	LDI	DRONE_C|RAILROAD,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	STI	R0,*+AR5(CAR_ID)
    // asm: 	STI	R0,*+AR7(PID)
    // asm: 	CALL	ADD_DRONE
    // asm: 	LDI	*+AR4(OLINK2),R0
    // asm: 	ADDI	0500h,R0
    // asm: 	STI	R0,*+AR7(PDATA+2)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR4(OLINK2)	;do not delete
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR5(CARBRAKE)
    // asm: 	LDF	*+AR5(CARRDFR),R0
    // asm: 	STF	R0,*+AR5(CAROFRDFR)
    // asm: 	LDF	*+AR4(ORADY),R2
    // asm: 	CALL	NORMITS
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	STF	R2,*+AR5(CARYROT)
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // *Initialize the distance to collision point
    // asm: 	LDF	@COLL_DIST,R2
    // asm: 	MPYF	2,R2
    // asm: 	STF	R2,*+AR7(DELTA_PLYRDIST)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARFORWARD_ENGINE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RAMPDOWNTRAINSND(void)
{
    // asm: 	CREATE	RAMPDOWNTRAINSND_PROC,UTIL_C
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMPDOWNTRAINSND", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RAMPDOWNTRAINSND_PROC(void)
{
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMPDOWNTRAINSND_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*FLYING TRAIN WRECK
*
*	AR0	POINTS TO PLAYER CAR
*	AR1	POINTS TO CAR TO SEND FLYING
*	AR4	POINTS TO PLAYER CAR BLOCK
*	AR5	POINTS TO DRONE CAR BLOCK
 */
void FLYTRAIN(void)
{
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYTRAIN", 0, 0);
    UNIMPL();
}

/* *
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
void FLYTRAINP(void)
{
    // *GET YOUR RADIANS
    // asm: 	LDF	0.2,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(PDATA)	  	;Z RADIANS
    // asm: 	LDF	0.05,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(PDATA+1)	;Y RADIANS
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(PDATA+2)	;X RADIANS (unused)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(PDATA+3)	;Z RADIAN TOTAL
    // asm: 	STF	R0,*+AR7(PDATA+5)  	;X RADIAN TOTAL
    // asm: 	LDF	*+AR5(CARYROT),R0	;GET CAR Y ROT
    // asm: 	STF	R0,*+AR7(PDATA+4)
FLYCARP0A:
    // asm: 	LDI	*+AR4(ODIST),R0		;OUT OF RANGE ??
    // asm: 	CMPI	-6000,R0
    // asm: 	BLT	RROAD_DIE   		;END THIS FARCE
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	CALL	ROADSCAN
    // asm: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR DATA AREA
    // asm: 	FLOAT	@NFRAMES,R1	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm: 	LDF	*+AR7(PDATA),R0		;ACCUMULATE X RADIANS
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(PDATA+3),R0
    // asm: 	STF	R0,*+AR7(PDATA+3)
    // asm: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(PDATA+4),R0
    // asm: 	STF	R0,*+AR7(PDATA+4)
    // asm: 	CALL	GETFLYMAT_TRAIN		;COMPUTE MATRICES
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
    // asm: 	FLOAT	@NFRAMES,R2
    // asm: 	MPYF	2,R2			;FRAME ADJUSTED GRAVITY
    // asm: 	ADDF	*+AR4(OVELY),R2
    // asm: 	STF	R2,*+AR4(OVELY)
    // asm: 	FLOAT	300,R1		  	;GOING DOWN TOO MUCH?
    // asm: 	CMPF	R1,R2
    // asm: 	BGT	RROAD_DIE		;ABORT THE DUDE...
    // asm: 	CALL	GETTRAK
    // asm: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // asm: 	BN	FLYCARSLPA		;WERE GOING UP IGNORE IT
    // asm: 	CALL	BOXSCAN		      	;KEEP FALLING!!!
    // asm: 	BNC	FLYCARSLPA
    // *WERE OVER THE ROAD
    // asm: FLYCROADA
    // asm: 	LDF	R0,R0
    // asm: 	BGT	FLYCARSLPA		;WERE ABOVE GROUND
    // *WE HIT THE GROUND DUDES
    // asm: 	ADDF	*+AR4(OPOSY),R0		;SET HIM ON THE GROUND
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OVELY),R2		;GET VERTICAL VELOCITY
    // asm: 	CMPF	35,R2          		;CHECK FOR MINIMUM
    // asm: 	BLT	FLYCARSTOP0A    	;TIME TO STOP
    // asm: FLYCROAD1A
    // asm: 	MPYF	-0.5,R2
    // asm: 	STF	R2,*+AR4(OVELY)
    // asm: 	LDF     *+AR5(CARSPEED),R0	;CUT SPEED IN HALF
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // asm: 	LDF	*+AR7(PDATA),R0		;CUT DOWN SPIN
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR7(PDATA)
    // asm: 	LDF	*+AR7(PDATA+1),R0
    // asm: 	MPYF	0.5,R0
    // asm: 	STF	R0,*+AR7(PDATA+1)
    // *MAKE BOUNCE SOUND
    // asm: 	LDI	@SCOLLTABI,AR2	   	;RANDOM COLLISION CRUNCH
    // asm: 	LDI	3,R0
    // asm: 	CALL	DRONESND
FLYCARSLPA:
    // asm: 	SLEEP	1
    // asm: 	B	FLYCARP0A
    // *ROTATE TO QUIESCENT STATE
    // *
FLYCARSTOP0A:
FLYCARSTOPA:
    // asm: 	LDI	*+AR4(ODIST),R0		;OUT OF RANGE ??
    // asm: 	CMPI	-6000,R0
    // asm: 	BLT	RROAD_DIE   		;END THIS FARCE
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
    // asm: 	LDI	@NFRAMES,RC	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm: 	SUBI	1,RC
    // asm: 	RPTB	FLYCSTLA
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
    // asm: 	MPYF	0.96,R0  		;DAMP IT
    // asm: 	STF	R0,*+AR7(PDATA)		;CUT DOWN ROCK
    // asm: 	ADDF	*+AR7(PDATA+3),R0
    // asm: 	STF	R0,*+AR7(PDATA+3)
    // asm: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // asm: 	MPYF	0.98,R0  		;DAMP IT
    // asm: 	STF	R0,*+AR7(PDATA+1)	;CUT DOWN ROCK
    // asm: 	STF	R0,*+AR5(CARDROT)	;STORE IN CAR STRUCTURE
    // asm: 	ADDF	*+AR7(PDATA+4),R0
    // asm: 	STF	R0,*+AR7(PDATA+4)
    // asm: 	LDF	*+AR7(PDATA+3),R2	;CHECK TOTAL X RADIANS
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2,R3
    // asm: 	LDF	*+AR5(CARSPEED),R0	;DECAY SPEED
    // asm: 	MPYF	0.98,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // *CHECK FOR DONE...
    // asm: 	CMPF	10,R0	       		;SPEED DECAYED?
    // asm: 	BGT	FLYCSTP0A	     	;NO, KEEP GOING...
    // asm: 	ABSF	*+AR5(CARDROT),R0	;GET ROTATE
    // asm: 	ABSF	*+AR7(PDATA),R1	    	;ADD IN ROCK
    // asm: 	ADDF	R0,R1
    // asm: 	MPYF	10,R1	    		;GET IN RANGE
    // asm: 	CMPF	0.02,R1			;PETERED OUT?
    // asm: 	BGT	FLYCSTP0A		;NO, KEEP GOING
    // asm: 	CMPF	0.2,R3			;RIGHT SIDE UP?
    // asm: 	BGT	FLYCSTP00A		;NOPE...
    // asm: 	LDF	0,R3			;STRAIGHTEN HIM UP!!!
    // asm: 	STF	R3,*+AR7(PDATA+3)
    // asm: 	LDI	2,R0
    // asm: 	B	FLYCCCA			;YES, TIME TO STOP
FLYCSTP00A:
    // asm: 	CMPF	2.95,R3			;UPSIDE DOWN?
    // asm: 	BLT	FLYCSTP0A		;NOPE
    // asm: FLYCSTPA
    // asm: 	LDI	1,R0	 		;WERE DONE DUDES...
    // asm: 	B	FLYCCCA
    // *ACCELERATE X ROTATION
FLYCSTP0A:
    // asm: 	LDF	R2,R2
    // asm: 	BN	FLYCSTP1A
    // asm: 	CMPF	1.57,R2
    // asm: 	B 	FLYCSTP2A
FLYCSTP1A:
    // asm: 	CMPF	-1.57,R2
FLYCSTP2A:
    // asm: 	LDFLT	-0.01,R0
    // asm: 	LDFGE	0.01,R0
    // asm: 	MPYF	0.4,R0
    // asm: 	ADDF	*+AR7(PDATA),R0
    // asm: 	LDF	*+AR7(PDATA),R1
    // asm: 	XOR	R0,R1,R2 		;CHECK FOR SIGN CHANGE- MAKE SOUND
    // asm: 	BNN	FLYCSTLA			;NO SOUND
    // asm: 	ABSF	*+AR7(PDATA+3),R2	;CHECK IF AMPLITUDE BIG ENOUGH
    // asm: 	CALL	NORMITS
    // asm: 	ABSF	R2
    // asm: 	CMPF	0.08,R2
    // asm: 	BLT	FLYCSTLA			;TOO SMALL OF A ROCK
    // asm: 	CMPF	3.06,R2
    // asm: 	BGT	FLYCSTLA			;TOO SMALL OF A UPSIDE DOWN ROCK
    // asm: 	LDI	BOTTOMOUT,AR2		;MAKE BOTTOMOUT SOUND
    // asm: 	PUSHF	R0
    // asm: 	CALL	DRONESND1
    // asm: 	POPF	R0
FLYCSTLA:
    // asm: STF	R0,*+AR7(PDATA)		;ACCELERATE ROTATION
    // asm: 	LDI	0,R0			;DONE FLAG
FLYCCCA:
    // asm: 	PUSH	R0
    // asm: 	CALL	GETTRAK
    // ;	CALL	DRONINBZ		;CHECK BOUNDS
    // asm: 	CALL	GETFLYMAT_TRAIN
    // asm: 	CALL	BOXSCAN		      	;KEEP FALLING!!!
    // asm: 	POP	R1			;CLEAN STACK
    // asm: 	BNC	FLYCARSTPA
    // asm: 	ADDF	*+AR4(OPOSY),R0		;SET HIM ON THE GROUND
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDI	R1,R1			;DONE
    // asm: 	BNE	FLYCARSTPA		;YESSAH
    // asm: 	SLEEP	1
    // asm: 	B	FLYCARSTOPA
FLYCARSTPA:
    // asm: 	CMPI	2,R1
    // asm: 	BZ	DEADCARA		;RIGHT SIDE UP CARCASS
    // *
    // *CAR IS UPSIDE DOWN
    // *
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	ANDN	TYPE_M,R0
    // asm: 	OR	DEAD_VEH_T,R0
    // asm: 	LDF	0,R0			;CLEAR OUT THE SPEED
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // *WAIT FOR OFFSCREEN
FLYCARWTA:
    // asm: 	SLEEP	1
    // asm: 	LDI	*+AR4(ODIST),R0	     	;OUT OF RANGE?
    // asm: 	CMPI	-6000,R0
    // asm: 	BLT	RROAD_DIE		;YES CLEAN IT UP...
    // asm: 	LDF	*+AR5(CARSPEED),R0	;ARE WE HIT???
    // asm: 	BZ	FLYCARWTA		;NO, JUST WAIT
    // *
    // *UPSIDE DOWN CAR IS HIT
    // *
    // asm: 	LDF	0.1,R0		       	;ROCK HIM A LITTLE
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(PDATA)	  	;X RADIANS RATE
    // asm: 	B	FLYCARSTOPA		;GO ROCK AND ROLL
    // *CLEAN UP THE MESS...
    // *
RROAD_DIE:
    // asm: 	CALL	FREE_DRONE
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	LDI	*+AR4(OFLAGS),R1
    // asm: 	ANDN	R0,R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	DELCAR
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLYTRAINP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CAR IS RIGHT SIDE UP
*
 */
void DEADCARA(void)
{
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
    // asm: 	LDF	1,R0
    // asm: 	STF	R0,*+AR5(CARBRAKE)
    // asm: 	LDF	*+AR7(PDATA+4),R0 	;GET Y ROTATION ORIENTATION
    // asm: 	STF	R0,*+AR5(CARVROT)
    // asm: 	STF	R0,*+AR5(CARYROT)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_SPIN)	;CLEAR SPIN
DEADLPA:
    // asm: 	LDI	*+AR4(ODIST),R0	      	;DIE OFF WHEN OFFSCREEN
    // asm: 	CMPI	-6000,R0
    // asm: 	BLT	RROAD_DIE
    // asm: 	LDF	0,R2			;NO STEERING
    // asm: 	CALL	DRONEGO
    // asm: 	CALL	GETTRAK
    // asm: DEADSLPA
    // asm: 	SLEEP	1
    // asm: 	B	DEADLPA
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEADCARA", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void GETFLYMAT_TRAIN(void)
{
    // asm: 	LONGROUT
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
    // asm: 	END_LONGROUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETFLYMAT_TRAIN", 0, 0);
    UNIMPL();
}

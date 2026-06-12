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
#include "../include/delta.h"
#include "../include/racer.h"
#include "../include/comm.h"
#include "../include/racer_defs.h"

/*
 * Source module: asm/RACER.ASM
 */

// *----------------------------------------------------------------------------
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: RACER_DRONE_INITTABI	.word	RACER_DRONE_INITTAB */
#define RACER_DRONE_INITTABI RACER_DRONE_INITTAB
/* asm: FINISHNUM	.bss	FINISHNUM,1 */
int FINISHNUM;
/* asm: RACER_PTR	.bss	RACER_PTR,10 */
int RACER_PTR[10];
/* asm: OM_TRACK_LO	.bss	OM_TRACK_LO,1 */
int OM_TRACK_LO;
/* asm: OM_TRACK_HI	.bss	OM_TRACK_HI,1 */
int OM_TRACK_HI;
// *----------------------------------------------------------------------------
// *
// *PARAMETERS
// *	R0	POSITION ACHIEVED FOR THIS RACE
// *
// *MODIFIES : ADJ_DIFFICULTY,ADJ_DIFF_LOCAL (CMOS)
// *
/* asm: GMAX	.word	100000 */
int GMAX = 100000;
/* asm: DIFFTABI	.word	DIFFTAB */
#define DIFFTABI DIFFTAB
/* asm: DIFFTAB */
/* asm: 	.float	0 		;GG */
/* asm: 	.float	-0.03 		;SF */
/* asm: 	.float	-0.02 		;101 */
/* asm: 	.float	-0.03 		;REDWD */
/* asm: 	.float	-0.03 		;BEVH */
/* asm: 	.float	0 		;LA */
/* asm: 	.float	-0.03 		;DV */
/* asm: 	.float	-0.01 		;AZ */
/* asm: 	.float	-0.02 		;GCAN */
/* asm: 	.float	-0.03 		;IOWA */
/* asm: 	.float	-0.03 		;CHI */
/* asm: 	.float	-0.02 		;IND */
/* asm: 	.float	-0.02 		;APP */
/* asm: 	.float	0 		;DC */
/* asm: 	*---------------------------------------------------------------------------- */
/* asm: 	* */
/* asm: 	*	1.	INIT */
/* asm: 	*	2.	SPREAD */
/* asm: 	*	3.	RUN */
/* asm: 	* */
/* asm: 	*PARAMETERS */
/* asm: 	*	R4	RANK */
/* asm: 	*	AR7	PROCESS */
/* asm: 	* */
/* asm: 	*	if R4 > 7 then */
/* asm: 	*		this drone is possibly a linked drone */
/* asm: 	*	end if */
/* asm: 	* */
int DIFFTAB[] = {
    0, // GG
    -0.03, // SF
    -0.02, // 101
    -0.03, // REDWD
    -0.03, // BEVH
    0, // LA
    -0.03, // DV
    -0.01, // AZ
    -0.02, // GCAN
    -0.03, // IOWA
    -0.03, // CHI
    -0.02, // IND
    -0.02, // APP
    0, // DC
};
// *
// *TRANSFER ACTIVE RACER
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *AM I AHEAD OF THIS VEHICLE?
// *
// *PARAMETERS
// *	AR4	MY OBJECT
// *	AR5	MY CAR BLOCK
// *	AR2	TEST OBJECT
// *	AR3	TEST CAR BLOCK
// *
// *RETURNS
// *	BGE AR2 CAR AHEAD OF AR4 CAR
// *	BLT AR2 CAR BEHIND AR4 CAR
// *	AR0,R0,R1,R2 CLOBBERED
// *----------------------------------------------------------------------------
/* asm: ROADOBSTAB	.BSS	ROADOBSTAB,50 */
int ROADOBSTAB[50];
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: WACKER	.word	3D20AH */
int WACKER = 0x3D20A;
/* asm: LAKEL	.word	3EF0CH */
int LAKEL = 0x3EF0C;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *CHECK CAR OBSTACLE
// *
// *PARAMETERS
// *	AR2	OBSTACLE CAR
// *	AR4	DRONE CAR
// *	AR5	DRONE CAR BLOCK
// *	R0	TABLE CODE 1000= PLAYER, 2000=DRONE
// *RETURNS
// *	R0	CLOSING TIME (800H=OUT OF RANGE)
// *
/* asm: CARTMP1	.BSS	CARTMP1,1 */
int CARTMP1;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *FLYING CAR PROCESS
// *
// *
// *PARAMETERS
// *	AR4	DRONE PROCESS
// *	AR5	DRONE CAR BLOCK
// *	PDATA=   X RAD
// *	PDATA+1= Y RAD
// *	PDATA+2= Z RAD
// *	PDATA+3= X RAD TOTAL
// *	PDATA+4= Y RAD	TOTAL
// *	PDATA+5= Z RAD	TOTAL
// *	PDATA+6=MATRIX
// *	PDATA+20=INIT HT
// *	PDATA+21=CAMVIEW SAVE
// *
// *----------------------------------------------------------------------------

void DIFF_CHANGE(void)
{
    // asm: 	CMPI	1,R0
    // asm: 	LDFZ	0.07,R1
    // asm: 	LDFNZ	-0.035,R1
    // asm: 	ADDF	@GAMEDIFF,R1
    // asm: 	CMPF	1.25,R1
    // asm: 	LDFGT	1.25,R1
    // asm: 	CMPF	0.8,R1
    // asm: 	LDFLT	0.8,R1
    // asm: 	STF	R1,@GAMEDIFF
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIFF_CHANGE", 0, 0);
    UNIMPL();
}

void GETDIFF(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET YOUR DIFFICULTY
    // *
    // *GET DIFFICULTY	ADJUST
    // asm: 	LDI	ADJ_DIFFICULTY,AR2		;GET DIFFICULTY	ADJUST
    // asm: 	CALL	ADJUSTMENT_READ			;100000=1.0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	0.04,R0
    // asm: 	ADDF	0.80,R0	      			;GLOBAL DIFF 0.80-1.16
    // *GET GAME DIFFICULTY
    // asm: 	LDI	@HEAD2HEAD_ON,R1
    // asm: 	LDFZ	@GAMEDIFF,R1
    // asm: 	BZ	GD1
    // asm: 	LDF	@OM_DIFF,R1
    // asm: 	CMPF	1.16,R1
    // asm: 	LDFGT	1.16,R1
    // asm: 	CMPF	0.8,R1
    // asm: 	LDFLT	0.8,R1
    // asm: 	ADDF	@GAMEDIFF,R1
    // asm: 	MPYF	0.5,R1
GD1:
    // asm: 	CMPF	1.16,R1
    // asm: 	LDFGT	1.16,R1
    // asm: 	CMPF	0.8,R1
    // asm: 	LDFLT	0.8,R1
    // asm: 	SUBF	1.0,R1
    // *GET TRACK DIFFICULTY
    // *GET CURRENT RACE
    // asm: 	LDI	@BONUS_WAVE,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	LDILT	0,AR2
    // asm: 	CMPI	13,AR2
    // asm: 	LDIGT	13,AR2
    // asm: 	ADDI	@DIFFTABI,AR2
    // asm: 	LDF	*AR2,R2
    // asm: 	ADDF	R1,R0
    // asm: 	ADDF	R2,R0
    // asm: 	CMPF	0.75,R0
    // asm: 	LDFLT	0.75,R0
    // asm: 	CMPF	1.25,R0
    // asm: 	LDFGT	1.25,R0				;ABSOLUTE MAX/MINS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETDIFF", 0, 0);
    UNIMPL();
}

void RACER_DRONE(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *	1.	INIT
    // *	2.	SPREAD
    // *	3.	RUN
    // *
    // *PARAMETERS
    // *	R4	RANK
    // *	AR7	PROCESS
    // *
    // *	if R4 > 7 then
    // *		this drone is possibly a linked drone
    // *	end if
    // *
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(FINISHDIST)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@FINISHNUM		;INITIALIZE FINISH COUNT
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(PASSCNT) 	;INITIALIZE PASSING SOUND INHIBIT
    // asm: 	STI	R4,*+AR7(DELTA_INIT)
    // asm: 	LDI	R4,AR0
    // asm: 	MPYI	RD_SIZE,AR0
    // asm: 	ADDI	@RACER_DRONE_INITTABI,AR0
    // asm: 	STI	AR0,*+AR7(INITINDEX)
    // asm: 	CMPI	8,R4
    // asm: 	BLT	NOTLINKED
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	NOTLINKED
    // asm: 	LDI	@OM_VEHICLE,R0
#if CDEBUG
    // asm: 	BN	$
#endif
    // asm: 	BR	COMM_DRONE
NOTLINKED:
    // asm: 	LDI	*+AR0(RD_MODEL),R0
JJKK:
    // asm: STI	R0,*+AR7(DELTA_MODEL)
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	R0,AR5
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDI	*+AR2(VEHTAB_MODEL),AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	SUICIDE
    // asm: 	LDI	AR0,AR4
    // *STORE THE POINTER TO THE OBJECT
    // 	;
    // asm: 	LDI	R4,AR1
    // asm: 	ADDI	@RACER_PTRI,AR1
    // asm: 	STI	AR4,*AR1
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	VEHICLE_ANI_INIT
    // asm: 	CALL	DELTA_OINIT
    // *SET PROPER PALETTE
    // asm: 	LDI	*+AR7(INITINDEX),AR0
    // asm: 	LDI	*+AR0(RD_PALETTE),R0
    // asm: 	BZ	NOOTHERPAL
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR4(OPAL)
    // asm: 	ORM	O_1PAL,*+AR4(OFLAGS)
NOOTHERPAL:
    // asm: 	LDF	*+AR5(CARRDFR),R0
    // asm: 	MPYF	1.5,R0			;OFFROAD = 1.5X ONROAD
    // asm: 	STF	R0,*+AR5(CAROFRDFR)
    // asm: 	LDI	*+AR7(INITINDEX),AR0
    // asm: 	LDF	*+AR0(RD_XLANE),R0
    // asm: 	STF	R0,*+AR7(DELTA_XLANE)
    // asm: 	STF	R0,*+AR7(ROADOFFSET)
    // asm: 	LDI	2,R2		  	;SET LANE STATUS
    // asm: 	FLOAT	600,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDIGT	3,R2
    // asm: 	STI	R2,*+AR7(DELTA_STATUS)
    // asm: 	LDI	@_MODE,R1
    // asm: 	AND	MMODE,R1
    // asm: 	CMPI	MATTR,R1
    // asm: 	LDFZ	1.0,R0	   		;FOR ATTRACT ALWAYS 1 DIFFICULTY
    // asm: 	CALLNZ	GETDIFF
    // asm: 	MPYF	*+AR0(RD_MAXACCEL),R0
    // asm: 	STF	R0,*+AR5(CARMAXACCEL)	;SET ACCEL POWER
    // asm: 	LDF	*+AR0(RD_REL),R0
    // asm: 	STF	R0,*+AR7(RELATIVITY)	;SET RELATIVITY COEFFICIENT
    // *
    // *INIT POSITION N ROAD PIECES FROM BEGINNING
    // *
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR2
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	ANDN	0FFh,R0
    // asm: 	LDI	*+AR2(OLINK4),AR2	;SKIP FIRST GROUP
    // asm: 	LDI	*+AR0(RD_POSITION),AR3	;NUMBER OF PIECES AHEAD
    // asm: 	CALL	SPOS_INIT		;INIT STARTING POSITION
    // 	;initialize Ytheta to the intentional direction
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	STF	R2,*+AR5(CARYROT)
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	DRONE_C|VEHICLE_T|DRNE_RACER,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	STI	R0,*+AR5(CAR_ID)
    // asm: 	STI	R0,*+AR7(PID)
    // *INIT THROTTLE AND BRAKE
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR5(CARBRAKE)
    // asm: 	LDF	0.44,R0
    // asm: 	STF	R0,*+AR5(CARTRACTION)
    // asm: 	LDF	1.0,R0
    // asm: 	STF	R0,*+AR7(DELTA_THROTTLE)
    // asm: 	STF	R0,*+AR7(POWERSURGE)
    // asm: 	LDF	0.05,R0
    // asm: 	CALL	FRAND
    // asm: 	ADDF	1.20,R0
    // asm: 	STF	R0,*+AR7(POWERCATCH)	;BLAST FROM START RANDOM TIME
    // asm: 	LDI	120,R0	   		;WAIT A LITTLE BEFORE UPDATE
    // asm: 	STI	R0,*+AR7(SURGETIME)
    // asm: 	STI	R0,*+AR7(CATCHTIME)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(STEALTHMODE)	;NO STEALTH INIT
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	OBJ_INSERT
    // ;	LDF	0.42,R0
    // ;	STF	R0,*+AR5(CARMAXACCEL)	;SET ACCEL POWER
    // *******************
    // asm: 	LDI	*+AR7(DELTA_INIT),R2	;GET CAR #
    // asm: 	STI	R2,*+AR5(CARNUM)	;SAVE IN CAR STRUCTURE
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	BZ	NOTLINKED3		;NO...
    // asm: 	LDI	@DIPRAM,R1     		;MASTER?
    // asm: 	TSTB	CMDP_MASTER,R1
    // asm: 	LDIZ	1,R0			;YES, MASTER
    // asm: 	LDINZ	0,R0			;SLAVE
    // asm: 	LSH	-1,R2
    // asm: 	AND	1,R2			;MASK FOR ODD/EVEN BIT
    // asm: 	XOR	R2,R0			;ODDS-MASTER, EVEN-SLAVE
    // asm: 	STI	R0,*+AR5(CAR_OM)
    // asm: 	BNZ	OM_DRONE		;OTHER MACHINE CONTROL
NOTLINKED3:
    // *
    // *RACING DRONE LOOP (ONTRACK)
    // *
RACER_LP:
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BNE	RACE_ON
    // asm: 	CALL	GETTRAK
    // asm: 	CALL	DRONESTOP
    // asm: 	B	RACER_SLP
RACE_ON:
    // asm: 	LDI	*+AR5(CARTRAK),AR0		;CHECK FOR FINISH LINE
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	LSH	-8,R0
    // asm: 	CMPI	@FINISH_ID,R0
    // asm: 	CALLGE	RACE_FIN
    // asm: 	CALL	RPASS	 			;CHECK FOR PASSING SOUND
    // *CHECK IF WE ARE ON THE TRACK
CKSTEALTH:
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR2		;GET CLOSEST ROAD ID
    // asm: 	LDI	*+AR2(OLINK4),AR2		;LINK A COUPLE AHEAD
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	LDI	@DYNALIST_END,AR2		;GET FURTHEST ROAD ID
    // asm: 	LDI	*+AR2(OUSR1),R1
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R2	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	CMPI	R0,R2
    // asm: 	BLT	LO_STEALTH
    // asm: 	CMPI	R1,R2				;LAST REAL SEGMENT
    // asm: 	BGE	HI_STEALTH
    // *CHECK DISTANCE TO TRACKING PIECE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2		;Get tracking piece
    // asm: 	SUBF	*+AR4(OPOSX),*+AR2(OPOSX),R2	;DELTA X
    // asm: 	LDF	*+AR2(OPOSZ),R1			;DELTA Z
    // asm: 	SUBF	*+AR4(OPOSZ),R1
    // asm: 	MPYF	R2,R2
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // asm: 	MPYF	R1,R1
    // asm: 	CMPF	R1,R2				;DISTANCE LT 5000 ?
    // asm: 	BLT	NUTRACK				;NO...KEEP ON TRUCKIN'
    // asm: 	LDI	*+AR2(OLINK4),R0		;Get link to NEXT piece
    // asm: 	BZ	ONTRACK				;OUT IN THE DISTANCE
    // asm: 	LDI	R0,AR0
    // asm: 	SUBF	*+AR4(OPOSX),*+AR0(OPOSX),R1	;DELTA X
    // asm: 	LDF	*+AR0(OPOSZ),R0			;DELTA Z
    // asm: 	SUBF	*+AR4(OPOSZ),R0
    // asm: 	MPYF	R0,R0
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	R2,R1	       			;DIST TRACK CLOSER?
    // asm: 	BGT	ONTRACK				;NO, WERE ON TRACK
    // asm: 	NOP
NUTRACK:
    // asm: 	LDI	*+AR2(OLINK4),R0		;Get link to NEXT piece
    // asm: 	BZ	HI_STEALTH
    // asm: 	STI	R0,*+AR7(DELTA_TPIECE)		;save ptr
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(OUSR1),R0			;read road ID
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)	;save road ID
    // asm: 	B	CKSTEALTH			;CHECK NEW SECTION
    // *R2=DIST TO TPIECE SQUARED
    // *AR2=TRACKING PIECE
ONTRACK:
    // asm: 	CALL	SQRT		      	;GET ACTUAL DISTANCE
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm: 	LDFLE	30,R1			;if 0 or less assume 30 mph
    // asm: 	FLOATP	@NFRAMES,R2
    // asm: 	MPYF	R2,R1
    // asm: 	CALL	DIV_F			;R0/R1 (distance to piece/speed) -> # frames to achieve
    // asm: 	PUSHF	R0
    // *GET TRACKING GOAL
    // asm: 	CALL	CKPCOL			;CHECK PLAYER COLLISION
    // asm: 	CALL	OBSTABINIT   		;CLEAR ROAD OBSTACLE MAP
    // asm: 	CALL	CARSCAN
    // asm: 	CALL	PLSCAN	      		;TEST TO SCAN PLAYER ON TRACK
    // asm: 	CALL	GETRDOFFSET
    // asm: 	STF	R0,*+AR7(ROADOFFSET)
    // *GET DIRECTION ANGLE FOR TRACKING PIECE
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2		;Get tracking piece
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	BNZ	ROADTRAK1
    // asm: 	LOCKUP
ROADTRAK1:
    // asm: 	LDI	R0,AR0
    // asm: 	SUBF	*+AR2(OPOSX),*+AR0(OPOSX),R2
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	SUBF	*+AR2(OPOSZ),R3
    // asm: 	CALL	ARCTANF
    // asm: 	SUBF	HALFPI,R0
    // asm: 	LDF	R0,R2		      	;FIND THETA
    // asm: 	CALL	_SINE
    // asm: 	LDF	R0,R1
    // asm: 	CALL	_COSI
    // asm: 	MPYF	*+AR7(ROADOFFSET),R0   	;X LANE OFFSET
    // asm: 	MPYF	*+AR7(ROADOFFSET),R1	;Z LANE OFFSET
    // asm: 	SUBF	*+AR4(OPOSX),*+AR2(OPOSX),R2
    // asm: 	ADDF	R0,R2
    // asm: 	LDF	*+AR2(OPOSZ),R3		;COMPUTE DELTA Z TO GOAL
    // asm: 	SUBF	*+AR4(OPOSZ),R3
    // asm: 	ADDF	R1,R3
    // 	;POSITION TRACKING
    // 	;
    // 	;Set steering, trottle, gear, and brake
    // 	;and call DRONEGO
    // 	;find the theta delta to this position
    // 	;
    // *GET XZ ANGLE TO GOAL
    // asm: 	CALL	ARCTANF
    // asm: 	SUBF	HALFPI,R0		;R0	DESIRED THETA (float)
    // asm:  	LDF	*+AR4(ORADY),R2		;R2	CURRENT THETA
    // asm: 	CALL	GETTHETADIFF		;->R0	THETA DELTA (float)
    // asm: 	POPF	R1   			;GET TIME
    // asm: 	FIX	R1
    // asm:  	FLOAT	R1
    // asm: 	SUBF	1,R1			;DBG
    // asm: 	BZ	NODIV
    // asm: 	CALL	DIV_F			;-> R0
NODIV:
    // *THROTTLE ONLY IF RACE IS ON
    // asm: 	PUSHF	R0
    // asm: 	CALL	GETPOWER			;UPDATE POWER STUFF
    // asm: 	LDF	*+AR7(DELTA_THROTTLE),R0	;HAS THE RACE BEGUN?
    // asm: 	LDI	@_MODE,R1
    // asm: 	TSTB	MGO,R1
    // asm: 	LDFZ	0,R0				;N -> THROTTLE = 0
    // asm: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm: 	POPF	R0
    // asm: 	MPYF	2.5,R0				;OVERSTEERing EFFECT
    // asm: 	CMPF	0.09,R0
    // asm: 	LDFGT	0.09,R0
    // asm: 	CMPF	-0.09,R0
    // asm: 	LDFLT	-0.09,R0
    // asm: 	PUSHF	R0				;save steering value
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	*+AR5(CARSPEED),R1
    // asm: 	BZD	NOTURN
    // asm: 	LDFZ	0,R0
    // asm: 	MPYF	0.416,R1
    // asm: 	MPYF	@STEERI,R1
    // 	;---->	BZD	NOTURN
    // asm: 	CALL	DIV_F
    // asm: 	CMPF	-0.3,R0 		;LIMIT CHECK
    // asm: 	LDFLT	-0.3,R0
    // asm: 	CMPF	0.3,R0 			;LIMIT CHECK
    // asm: 	LDFGT	0.3,R0
NOTURN:
    // asm: 	STF	R0,*+AR5(CARTURN)
    // asm: 	CALL	DRONE_RIDE_RIGHT		;FIND DISTANCE TO CENTER OF ROAD
    // asm: 	STF	R0,*+AR5(CARDIST2CNTR)
    // asm: 	CALL	GETTRAK
    // asm: 	POPF	R2
    // asm: 	CALL	DRONEGO
    // asm: 	NEGF	*+AR5(CARTURN),R0		;FIX CARTURN SIGN
    // asm: 	STF	R0,*+AR5(CARTURN)
RACER_SLP:
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_RACER_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: NOTLINKED4
    // asm: 	SLEEP	1
    // asm: 	B	RACER_LP
    // *
    // *CHECK RACE FINISH
    // *
    // asm: RACE_FIN
    // asm: 	LDF	*+AR7(FINISHDIST),R0
    // asm: 	BNZ	RACFIN1
    // asm: 	LDF	0.7,R0
    // asm: 	CALL	SFRAND
    // asm: 	STF	R0,*+AR7(FINISHROT)
    // asm: 	LDI	@FINISHNUM,R0
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,@FINISHNUM
    // asm: 	SUBRI	10,R0
    // asm: 	MPYI	2800,R0
    // asm: 	ADDI	3500,R0
    // asm: 	FLOAT	R0
RACFIN1:
    // asm: 	LDF	*+AR5(CARSPEED),R2
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R2
    // asm: 	SUBF	R2,R0
    // asm: 	STF	R0,*+AR7(FINISHDIST)
    // asm: 	BLT	RACEDONE
    // asm:    	RETS
RACEDONE:
    // asm: 	LDI	*+AR7(STEALTHMODE),R0		;CHECK STEALTH
    // asm: 	BZ	RD0
    // asm:    	LDF	*+AR5(CARSPEED),R0
    // asm: 	BZ	RD0
    // asm: 	FLOAT	300,R0	      			;ONE TIME RANDOM ADD
    // asm: 	CALL 	SFRAND
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	FLOAT	300,R0
    // asm: 	CALL 	SFRAND
    // asm: 	ADDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
RD0:
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARSPEED)
    // asm: 	STF	R0,*+AR5(CARMAXACCEL)
    // asm: 	LDI	*+AR7(STEALTHMODE),R0		;CHECK STEALTH
    // asm: 	RETSNZ					;YES, KEEP WAITING
    // asm: 	POP	R0				;CAN CALL
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_NOCOLL,R0			;SET NON COLLIDE FLAG
    // asm: 	STI	R0,*+AR4(OFLAGS)
RDL:
    // asm: 	CALL	GETTRAK
    // asm: 	CALL	ROADIR
    // asm: 	ADDF	*+AR7(FINISHROT),R0
    // asm: 	STF	R0,*+AR5(CARYROT)
    // asm: 	CALL	DRONESTOP
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_RACER_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	SLEEP	1    			;JUST HANG THERE
    // ;	SLEEP	10    			;JUST HANG THERE
    // asm: 	B	RDL
    // *
    // *CHECK PLAYER COLLISION
    // *AR4=DRONE CAR
    // *
    // asm: CKPCOL
    // asm: 	LDI	@PLYCAR,AR2		;GET PLAYER CAR OBJECT
    // asm: 	LDI	*+AR2(OCARBLK),AR3
    // asm: 	CALL	CKAHEAD
    // asm: 	BN	CKPX 		;CAR BEHIND, IGNORE
    // *GET CURRENT DISTANCE
    // asm: 	SUBF	*+AR4(OPOSX),*+AR2(OPOSX),R6
    // asm: 	LDF	*+AR2(OPOSZ),R7
    // asm: 	SUBF	*+AR4(OPOSZ),R7
    // asm: 	MPYF	R6,R6,R1
    // asm: 	MPYF	R7,R7,R2
    // asm: 	ADDF	R1,R2			;distance^2
    // asm: 	FLOAT	3000,R0
    // asm: 	MPYF	R0,R0
    // asm: 	CMPF	R2,R0
    // asm: 	BLT	CARCHKX			;TOO FAR AWAY
    // asm: 	LDF	*+AR5(CARSPEED),R0
    // asm: 	SUBF	*+AR3(CARSPEED),R0
    // asm: 	CMPF	30,R0
    // asm: 	BLT	CKPX
    // asm: 	NOP
CKPX:
    // asm: 	RETS
    // *
    // *ENTER DISTANT STEALTH MODE
    // *
HI_STEALTH:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(STEALTHMODE)		;HI STEALTH FLAG
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R2	;GRAB THE LAST KNOWN VALID OID
    // asm: 	CALL	FIND_MAP			;*+AR7(DELTA_SPTR),*+AR7(DELTA_LAST_OID)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_NOCOLL,R0			;SET NON COLLIDE FLAG
    // asm: 	STI	R0,*+AR4(OFLAGS)
HI_STLP:
    // *CHECK FINISH LINE
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R0	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	LSH	-8,R0
    // asm: 	CMPI	@FINISH_ID,R0
    // asm: 	CALLGE	RACE_FIN
    // *CHECK IF CURRENT SECTION ON TRACK LIST
    // asm: 	LDI	@DYNALIST_END,AR2		;GET FURTHEST ROAD ID
    // asm: 	LDI	*+AR2(OUSR1),R1
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R2	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	CMPI	R1,R2
    // asm: 	BLT	REENTER				;WERE OFF ACTIVE TRACK
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	CALLNZ	CKTRANSHI
    // asm: 	CALL	STEALTH_UPDATE
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_RACER_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	SLEEP	1
    // asm: 	B	HI_STLP
HI_ST_END:
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_RACER_POS		;SEND YOUR POSITION TO LINKED GAME
    // ;	SLEEP	10
    // asm: 	SLEEP	1
    // asm: 	B	HI_ST_END
    // *
    // *ENTER CLOSE STEALTH MODE
    // *
LO_STEALTH:
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,*+AR7(STEALTHMODE)		;LO STEALTH FLAG
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R2	;GET POINTER TO STEALTH MAP
    // asm: 	CALL	FIND_MAP			;
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_NOCOLL,R0			;SET NON COLLIDE FLAG
    // asm: 	STI	R0,*+AR4(OFLAGS)
LO_STLP:
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR2		;GET CLOSEST ROAD ID
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R2	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	CMPI	R0,R2
    // asm: 	BGT	REENTER				;REENTER THE SYSTEM DUDES...
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	CALLNZ	CKTRANSLO
    // asm: 	CALL	STEALTH_UPDATE
    // asm: 	LDI	@HEAD2HEAD_ON,R0    	;HEAD 2 HEAD RACE???
    // asm: 	CALLNZ	SEND_RACER_POS		;SEND YOUR POSITION TO LINKED GAME
    // asm: 	SLEEP	1
    // asm: 	B	LO_STLP
    // *
    // *REENTER REGULAR SYSTEM
    // *R2=OID OF SECTION BEING TRACKED
    // *
REENTER:
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(STEALTHMODE)	;NO STEALTH INIT
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	ANDN	O_NOCOLL,R0		;CLEAR NON-COLLIDE FLAG
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	CALL	FIND_DYNA		;GET TRACKING PIECE POINTER
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm: 	CALL	GETTRAK	      		;GET CLOSEST ROAD SECT ->AR0
    // asm: 	CALL	ROADIR			;GET RADIANS FOR ORIENTATION
    // asm: 	LDF	R0,R2
    // asm: 	STF	R2,*+AR4(ORADY)
    // asm: 	STF	R2,*+AR5(CARYROT)
    // asm: 	STF	R2,*+AR5(CARVROT)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARDROT)	;CLEAN UP REENTRY
    // asm: 	STF	R0,*+AR5(CARSPRAD)
    // asm: 	STF	R0,*+AR5(CARSKID)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
    // asm: 	LDI	AR4,AR2	  		;GET NEW MATRIX
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	FLOAT	1000,R0			;PUT DUDE DOWN INTO ROAD
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	B	RACER_LP
    // *
    // *CHECK TRANSFER TO OTHER MACHINE ON LINK
    // *LO STEALTH CASE
    // asm: CKTRANSLO
    // asm: 	CALL	COMPTRAK
    // asm: 	BLE	CKTLX
    // asm: 	CALL	CKRANGE
    // asm: 	BZ	SEND_RACER_XSFER
CKTLX:
    // asm: 	RETS
    // *HI STEALTH CASE
    // asm: CKTRANSHI
    // asm: 	CALL	COMPTRAK
    // asm: 	BGE	CKTHX
    // asm: 	CALL	CKRANGE
    // asm: 	BZ	SEND_RACER_XSFER
CKTHX:
    // asm: 	RETS
    // *
    // *CHECK IF CAR TRACK ID IS IN OTHER PLAYERS RANGE
    // *RETURNS EQ =IN RANGE, NE=OUT OF RANGE
    // asm: CKRANGE
    // asm: 	LDI	*+AR7(DELTA_LAST_OID),R2	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	CMPI	@OM_TRACK_LO,R2
    // asm: 	BLT	CKRFAIL
    // asm: 	CMPI	@OM_TRACK_HI,R2
    // asm: 	BGT	CKRFAIL
    // asm: 	LDI	0,R0
    // asm: 	RETS
CKRFAIL:
    // asm: 	LDI	1,R0
    // asm: 	RETS
    // *
    // *COMPARE TO PLAYERS RANK
    // *RETURNS:GT IF WE'RE AHEAD OF OTHER PLAYER
    // *R0=OTHER PLAYER TRACK ID, R1= OUR TRACK ID
    // *DESTROYS AR0,AR1
    // *
    // asm: COMPTRAK
    // asm: 	LDI	@PLY2CAR,AR0
    // asm: 	LDI	*+AR0(OCARBLK),AR1
    // asm: 	LDI	*+AR1(CARTRACK_ID),R0
    // asm: 	LDI	@PLYCBLK,AR1
    // asm: 	LDI	*+AR1(CARTRACK_ID),R1
    // asm: 	CMPI	R0,R1
    // asm: 	RETS
    // *
    // *TRANSFER CAR TO OTHER MACHINE
    // *AR4=CAR
    // *AR5=CAR BLOCK
    // *AR7=PROCESS
    // *
SEND_RACER_XSFER:
    // asm: 	LDI	CB_RACER_XSFER,R0 	;MESSAGE HEADER
    // *SEND MESSAGE HEADER
    // asm: 	LDI	@COMMQ_TMP_BUFFI,AR2
    // asm: 	STI	R0,*AR2
    // *SEND CAR ID #
    // asm: 	LDI	*+AR7(DELTA_INIT),R0
    // asm: 	STI	R0,*+AR2(1)
    // asm: 	LDI	1,RC
    // asm: 	CALL	MESSAGE_ADD
    // asm: 	POP	R0			;CLEAN OFF STACK
    // asm: 	B 	OM_DRONE  		;NOW CONTROLLED BY OTHER MACHINE
    // *
    // *GET A CAR FROM OTHER MACHINE
    // *AR4=CAR
    // *AR5=CAR BLOCK
    // *AR7=PROCESS
    // *
    // *NOTE SHOULD ADD IN FUTURE: CAR_SPIN,CARSPRAD,CARGEAR,CARRPM,CARSKID
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RACER_DRONE", 0, 0);
    UNIMPL();
}

void DECODE_RACER_XSFER(void)
{
    // asm: 	LDI	*AR2++,R0
    // asm: 	LDI	@IGNORE_UPDATES,R1
    // asm: 	RETSNZ
    // asm: 	LSH	-16,R0  		;GET CAR #
    // asm: 	BLT	LINKRECX
    // asm: 	CMPI	7,R0
    // asm: 	BGT	LINKRECX		;RANGE CHECK
    // asm: 	ADDI	@RACER_PTRI,R0
    // asm: 	LDI	R0,AR4
    // asm: 	LDI	*AR4,R0		 	;GET POINTER
    // asm: 	BZ	LINKRECX
    // asm: 	LDI	R0,AR4
    // asm: 	LDI	*+AR4(OPLINK),AR7
    // *INITIALIZE SHIT
    // asm: 	LDF	1.0,R0
    // asm: 	STF	R0,*+AR7(DELTA_THROTTLE)
    // asm: 	STF	R0,*+AR7(POWERSURGE)
    // asm: 	STF	R0,*+AR7(POWERCATCH)	;BLAST FROM START RANDOM TIME
    // asm: 	LDI	75,R0	   		;WAIT A LITTLE BEFORE UPDATE
    // asm: 	STI	R0,*+AR7(SURGETIME)
    // asm: 	LDI	27,R0	   		;WAIT A LITTLE BEFORE UPDATE
    // asm: 	STI	R0,*+AR7(CATCHTIME)
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(STEALTHMODE)  	;SET STEALTH INIT
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_OM)	;CAR BELONGS TO ME NOW!!!
    // *CHECK FOR HI OR LO STEALTH ENTRY
    // asm: 	LDI	*+AR5(CARTRACK_ID),R1		;GET TRACK ID
    // asm: 	STI	R1,*+AR7(DELTA_LAST_OID)
    // asm: 	LDI	@DYNALIST_END,AR0		;GET FURTHEST ROAD ID
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	CMPI	R1,R0
    // asm: 	BGT	LINKREC1
    // asm: 	LDI	@HI_STEALTHI,R2
    // asm: 	B	LINKREC3
LINKREC1:
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR0		;GET CLOSEST ROAD ID
    // asm: 	LDI	*+AR0(OLINK4),AR0
    // asm: 	LDI	*+AR0(OLINK4),AR0
    // asm: 	LDI	*+AR0(OLINK4),AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	CMPI	R1,R0
    // asm: 	LDILT	@ACTIVE_XSFERI,R2
    // asm: 	LDIGE	@LO_STEALTHI,R2
LINKREC3:
    // asm: 	STI	R2,*+AR7(PWAKE)			;CHANGE WAKE-UP ADDR
    // asm: 	RETS
LINKRECX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DECODE_RACER_XSFER", 0, 0);
    UNIMPL();
}

void ACTIVE_XSFER(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(STEALTHMODE)		;NO STEALTH INIT
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	ANDN	O_NOCOLL,R0			;CLEAR NON-COLLIDE FLAG
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	*+AR5(CARTRACK_ID),R2		;GET TRACK ID
    // asm: 	STI	R2,*+AR7(DELTA_LAST_OID)	;CHECK TO SEE IF IT IS IN THE RANGE
    // asm: 	CALL	FIND_DYNA			;GET TRACKING PIECE POINTER
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARDROT)	;CLEAN UP REENTRY
    // asm: 	STF	R0,*+AR5(CARSPRAD)
    // asm: 	STF	R0,*+AR5(CARSKID)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CAR_SPIN)
    // asm: 	B	RACER_LP
    // *
    // *UPDATE STEALTH POSITION
    // *LINEAR TRACK TO NEXT TRACK SEGMENT
    // *
STEALTH_UPDATE:
    // asm: 	LDI	*+AR7(DELTA_SPTR),AR0  		;STEP TO NEXT PIECE?
    // asm: 	FLOAT  	*+AR0(X),R2
    // asm: 	SUBF	*+AR4(OPOSX),R2
    // asm: 	FLOAT  	*+AR0(Z),R3
    // asm: 	SUBF	*+AR4(OPOSZ),R3
    // asm: 	MPYF	R2,R2,R0
    // asm: 	MPYF	R3,R3,R1
    // asm: 	ADDF	R1,R0
    // asm: 	FLOAT	5000,R1				;TO THE NEXT PIECE
    // asm: 	MPYF	R1,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	STLUP1
    // asm: 	ADDI	4,AR0
    // asm: 	STI	AR0,*+AR7(DELTA_SPTR)
    // asm: 	LDI	*+AR0(3),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)	;SAVE THE LAST KNOWN VALID OID
    // asm: 	CMPI	-1,R0				;CHECK END OF THE LINE
    // asm:       	BNZ	STEALTH_UPDATE
    // asm: 	POP	R0				;END OF LINE JUST SLEEP
    // asm: 	B	HI_ST_END
STLUP1:
    // asm: 	FLOAT	*+AR0(Y),R0	       		;GET Y POSITION SET
    // ;	LDF	*+AR0(Y),R0	       		;GET Y POSITION SET
    // asm: 	FLOAT	250,R1
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSY)			;ADJUST YOUR Y
    // asm: 	LDI	@VECTORAI,AR2 			;GET DIRECTIONAL VELOCITY
    // asm: 	STF	R2,*AR2	  			;DELTA X
    // asm: 	LDF	0,R0				;DELTA Y
    // asm: 	STF	R0,*+AR2(1)
    // asm: 	STF	R3,*+AR2(2)			;DELTA Z
    // asm: 	CALL	NORMALIZE
    // asm: 	CALL	GETRPM
    // asm: 	CALL	GETAUTO
    // asm: 	STI	R0,*+AR5(CARGEAR)	 	;DO THE GEAR
    // asm: 	LDI	*+AR7(DELTA_SPTR),R0  		;STEP TO NEXT PIECE?
    // asm: 	STI	R0,*+AR5(CARTRAK)		;PUT THIS INTO CARTRAK FOR NOW
    // asm: 	CALL	GETPOWER			;UPDATE POWER STUFF
    // asm: 	CALL	GETSTSPD			;GET STEALTH SPEED
    // asm: 	MPYF	*AR2,R5,R0		 	;X DELTA
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	MPYF	*+AR2(2),R5			;Z DELTA
    // asm: 	ADDF	*+AR4(OPOSZ),R5
    // asm: 	STF	R5,*+AR4(OPOSZ)
    // asm: 	RETS
    // *
    // *GET RACER POWER
    // *
    // asm: GETPOWER
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR7(CATCHTIME),R0 	;CHECK IF TIME FOR POWER CHANGE
    // asm: 	DEC	R0
    // asm: 	STI	R0,*+AR7(CATCHTIME)
    // asm: 	BNN	GETSURGE		;NOPE
    // *GET NEW UPDATE TIMER
    // asm: 	LDI	35,AR2
    // asm: 	CALL	RANDU
    // asm: 	ADDI	10,R0
    // asm: 	STI	R0,*+AR7(CATCHTIME)
    // *GET DISTANCE TO PLAYER
    // asm: 	LDI	@PLYCAR,AR0
    // asm: 	SUBF	*+AR4(OPOSX),*+AR0(OPOSX),R1
    // asm: 	LDF	*+AR0(OPOSZ),R2
    // asm: 	SUBF	*+AR4(OPOSZ),R2
    // asm: 	MPYF	R1,R1
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R1,R2
    // asm: 	CALL	SQRT
    // asm: 	LDI	*+AR7(STEALTHMODE),R3		;HI,LO OR NO STEALTH?
    // asm: 	BZ	PDIST
    // asm: 	LDI	R3,R1
    // asm: 	LDI	0,R2
    // asm: 	B	PDIST1
PDIST:
    // asm: 	LDI	*+AR0(OCARBLK),AR1
    // asm: 	LDI	*+AR1(CARTRAK),AR2
    // asm: 	LDI	*+AR2(OUSR1),R2
    // asm: 	LDI	*+AR5(CARTRAK),AR3
    // asm: 	LDI	*+AR3(OUSR1),R1
PDIST1:
    // asm: 	MPYF	0.01,R0
    // asm: 	MPYF	0.01,R0
    // asm: 	MPYF	0.07,R0
    // asm: 	CMPF	0.4,R0			;MAX OUT AT +-60%
    // asm: 	LDFGT	0.4,R0
    // asm: 	LDF	1.0,R3			;SIGN
    // asm: 	CMPI	R2,R1
    // asm: 	LDFZ	0,R3			;EVEN RACE
    // asm: 	LDFGT	-1.0,R3
    // asm: 	BLE	PDIST1A
    // asm: 	CMPF	0.1,R0
    // asm: 	LDFLT	0.1,R0			;MINIMUM IF AHEAD
    // asm: 	B	PDIST1B
PDIST1A:
    // asm: 	CMPF	0.2,R0
    // asm: 	LDFLT	0.2,R0			;MINIMUM IF BEHIND
PDIST1B:
    // asm: 	MPYF	R3,R0,R3		;DO THE SIGN
    // asm: 	FLOAT	@PLYRFIRST,R0	  	;TIME IN FIRST
    // asm: 	BZ	PDIST2
    // asm: 	MPYF	0.01,R0
    // asm: 	MPYF	0.15,R0
    // asm: 	CALL	FRAND
    // asm: 	CMPF	0.15,R0
    // asm: 	LDFGT	0.15,R0			;BOOST FOR LONG TIME FIRST
    // asm:        	ADDF	R0,R3			;A LITTLE BOOST IF PLAYER = #1
PDIST2:
    // asm: 	MPYF	*+AR7(RELATIVITY),R3	;ADJUST FOR RELATIVITY
    // asm: 	ADDF	1.0,R3
    // asm: 	STF	R3,*+AR7(POWERCATCH)
GETSURGE:
    // asm: 	LDI	*+AR7(SURGETIME),R0 	;CHECK IF TIME FOR POWER CHANGE
    // asm: 	DEC	R0
    // asm: 	STI	R0,*+AR7(SURGETIME)
    // asm: 	BNN	POWERX
    // asm: 	LDI 	90,AR2			;GET NEW TIME
    // asm: 	CALL	RANDU
    // asm: 	ADDI	30,AR2
    // asm: 	STI	R0,*+AR7(SURGETIME)
    // asm: 	LDF	0.12,R0	       		;GET NEW POWERSURGE
    // asm: 	CALL	SFRAND
    // asm: 	ADDF	1.0,R0
    // asm: 	STF	R0,*+AR7(POWERSURGE)
POWERX:
    // asm: 	LDF	*+AR7(POWERSURGE),R0
    // asm: 	MPYF	*+AR7(POWERCATCH),R0
    // asm: 	STF	R0,*+AR7(DELTA_THROTTLE)
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ACTIVE_XSFER", 0, 0);
    UNIMPL();
}

void GETSTSPD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET STEALTH CAR SPEED
    // *
    // *PARAMETERS
    // *	AR4	CAR OBJECT
    // *	AR5	CAR BLOCK
    // *
    // *RETURNS
    // *	 R5	DISTANCE TRAVELLED
    // *
    // *GET ENGINE ACCEL
    // asm: 	LDF	*+AR7(DELTA_THROTTLE),R0	;HAS THE RACE BEGUN?
    // asm: 	LDI	@_MODE,R1
    // asm: 	TSTB	MGO,R1
    // asm: 	LDFZ	0,R0				;N -> THROTTLE = 0
    // asm: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm: 	MPYF	*+AR5(CARMAXACCEL),R0
    // asm: 	LDF	*+AR5(CARRPM),R1
    // asm: 	MPYF	0.333,R1
    // asm: 	FIX	R1,IR0		   	;GET TABLE INDEX
    // asm: 	CMPI	18,IR0
    // asm: 	LDIGT	18,IR0			;KEEP INDEX IN RANGE
    // asm: 	LDI	@ENGACTABI,AR0
    // asm: 	LDF	*+AR0(IR0),R2		;GET LO POWER FACTOR FOR GEAR
    // asm: 	ADDI	1,IR0
    // asm: 	LDF	*+AR0(IR0),R3		;GET HI POWER FACTOR FOR GEAR
    // asm: 	FLOAT	IR0,R4
    // asm: 	SUBF	R1,R4,R1
    // asm: 	LDFLT	0,R1  			;KEEP FACTOR IN BOUNDS
    // asm: 	CMPF	1.0,R1
    // asm: 	LDFGT	1.0,R1
    // asm: 	MPYF	R1,R2			;INTERPOLATE !!!
    // asm: 	SUBRF	1.0,R1
    // asm: 	MPYF	R1,R3
    // asm: 	ADDF	R2,R3,R1
    // asm: 	LDI	*+AR5(CARGEAR),IR0	;GEAR MULTIPLIER
    // asm: 	LDI	@GEARACTABI,AR0
    // asm: 	MPYF	*+AR0(IR0),R1
    // asm: 	MPYF	0.97,R1			;3% POWER LOSS AUTO
    // asm: 	MPYF	R1,R0
    // *GET TOTAL FRICTION
    // *GET ROAD FRICTION
    // asm: 	LDF	*+AR5(CARRDFR),R3    	;GET ROAD FRICTION
    // *GET ENGINE FRICTION
    // asm: 	LDI	*+AR5(CARGEAR),IR0
    // asm: 	LDI	@ENGFRI,AR0
    // asm: 	LDF	*+AR0(IR0),R4
    // asm: 	LDF	*+AR5(CARRPM),R5	;MORE REVS MORE FRICTION
    // asm: 	SUBF	42,R5			;HEAVY FRICTION ABOVE 3900
    // asm: 	LDFLT	0,R5
    // asm: 	MPYF	0.04,R5
    // asm: 	ADDF	1.0,R5			;APPROX RANGE 1-2
    // asm: 	MPYF	R4,R5
    // asm: GETSP22
    // asm: 	ADDF	R5,R3
    // *CALC NEW SPEED
    // asm: GETSPD2
    // asm: 	LDF	0,R5
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm: 	LDPI	@NFRAMES,RC
    // asm: 	SUBI	1,RC
    // *R0=ACCEL
    // *R1=SPEED
    // *R3=TOTAL FRICTION
    // *R5=DISTANCE
    // *R7=ENGINE FRICTION
    // asm: 	RPTB	GSL0
    // asm: 	MPYF 	R1,R3,R4	     	;BRAKE/ROAD/ENGINE FRICTION
    // asm: 	SUBF	R4,R1			;SUBTRACT FRICTION
    // asm: 	ADDF	R0,R1			;ADD ACCEL
GSL0:
    // asm: ADDF	R1,R5
    // asm: 	MPYF	1.5,R5			;SPEEDFUDGE
    // asm: 	STF	R5,*+AR5(CARDIST)	;SAVE YOUR DISTANCE
    // asm: 	STF	R1,*+AR5(CARSPEED)	;NEW SPEED
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETSTSPD", 0, 0);
    UNIMPL();
}

void FIND_DYNA(void)
{
    // *----------------------------------------------------------------------------
    // *---------------------------------------------------------------------------
    // *GET OBJECT POINTER TO ROAD SEG ID IN R2
    // *
    // *PARAMETERS
    // *	R2	OUSR1
    // *RETURNS
    // *	AR2	POINTER TO OBJECT
    // *
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR2
FDL:
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	CMPI	R0,R2
    // asm: 	BZ	FDLX
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	CMPI	0,AR2			;CHECK END OF THE LINE
    // asm: 	BNZ	FDL
    // asm: 	LOCKUP				;ERROR COULDN'T FIND THE SUCKER
    // asm: 	POP	R0
    // asm: 	DIE
FDLX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_DYNA", 0, 0);
    UNIMPL();
}

void FIND_MAP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GIVEN A SECTION/ROAD ID FIND THE MAP ENTRY IN THE
    // *ROAD MAP WHICH CONTAINS THE ID EQUAL TO, OR GREATER
    // *THAN THE GIVEN ID.
    // *[finds GE road ID ptr]
    // *
    // *PARAMETERS
    // *	R2	CODED ID (from OUSR1)
    // *	AR7	DRONE PROCESS
    // *
    // *RETURNS
    // *	AR0 - POINTS TO MAP ENTRY
    // *	MAP ENTRY-->*+AR7(DELTA_SPTR)
    // *	MAP FLAGS-->*+AR7(DELTA_LAST_OID)
    // *
    // asm: 	LDI	@LEG_MAPI,AR0
    // asm: 	ADDI	3,AR0				;OFFSET TO ID
FIND_LP:
    // asm: 	LDI	*AR0++(4),R0
    // asm: 	CMPI	R2,R0
    // asm: 	BLT	FIND_LP
    // asm: FINDX
    // asm: 	SUBI	7,AR0
    // asm: 	STI	AR0,*+AR7(DELTA_SPTR)
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)	;save road id
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_MAP", 0, 0);
    UNIMPL();
}

void CKAHEAD(void)
{
    // asm: 	LDI	*+AR3(CARTRAK),AR0
    // asm: 	LDI	*+AR0(OUSR1),R0
    // asm: 	LDI	*+AR5(CARTRAK),AR0
    // asm: 	CMPI	*+AR0(OUSR1),R0
    // asm: 	RETSNZ				;RETURN VALUE IF NOT THE SAME
    // asm: 	LDI	*+AR0(OLINK4),R0	;GET NEXT ROAD OBJECT
    // asm: 	LDINZ	R0,AR0			;HANDLE END OF LIST PROBLEM
    // asm: 	LDI	*+AR0(OLINK4),R0	;GET NEXT ROAD OBJECT
    // asm: 	LDINZ	R0,AR0			;HANDLE END OF LIST
    // asm: 	SUBF	*+AR4(OPOSX),*+AR0(OPOSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR0(OPOSZ),R1
    // asm: 	SUBF	*+AR4(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R0
    // asm: 	SUBF	*+AR2(OPOSX),*+AR0(OPOSX),R2
    // asm: 	MPYF	R2,R2
    // asm: 	LDF	*+AR0(OPOSZ),R1
    // asm: 	SUBF	*+AR2(OPOSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R2,R1
    // asm: 	CMPF	R1,R0			;WHO IS CLOSER?
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CKAHEAD", 0, 0);
    UNIMPL();
}

void CARSCAN(void)
{
    // *----------------------------------------------------------------------------
    // *SCAN CAR LIST FOR OBSTACLES TO AVOID
    // *BUILD OBSTACLE ARRAY
    // *ROAD IS 5000 WIDE SO 50 UNITS X 100
    // *CAR IS 8 UNITS WIDE
    // *
    // *PARAMETERS
    // *	AR4	CAR OBJECT
    // *	AR5	CAR STRUCTURE
    // *
    // asm: 	LDI	@CAR_LIST,R0
    // asm: 	BZ	OBSCX
OBSCL:
    // asm: 	LDI	R0,AR2
    // asm: 	CMPI	AR2,AR4
    // asm: 	BZ	OBSCLP			;BR -> DONT COMPARE AGAINST OURSELF
    // asm: 	LDI	*+AR2(OFLAGS),R0	;STEALTH MODE?
    // asm: 	TSTB	O_NOCOLL,R0		;check non-collide flag
    // asm: 	BNZ	OBSCLP		    	;YES, DONT BOTHER
    // asm: 	LDI	2000H,R0
    // asm: 	CALL	CARCHEK
OBSCLP:
    // asm: 	LDI	*+AR2(OLINK3),R0
    // asm: 	BNZ	OBSCL
OBSCX:
    // asm: 	RETS
    // *GO FOR OPEN PATH
    // *IF NO OPENING BRAKE ONLY FOR IMMINENT COLLISION 10 FRAMES OR LESS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARSCAN", 0, 0);
    UNIMPL();
}

void PLSCAN(void)
{
    // asm: 	LDPI	@PLYCAR,AR2		;GET PLAYER CAR OBJECT
    // asm: 	LDI	*+AR2(OCARBLK),AR3
    // asm: 	LDI	4000H,R0
    // asm: 	CALL	CARCHEK
    // asm: 	STI	R0,@PLYRCLOSE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLSCAN", 0, 0);
    UNIMPL();
}

void OBSTABINIT(void)
{
    // *
    // *OBSTACLE TABLE INIT
    // *
    // asm: 	LDI	0,R0
    // asm: 	LDI	@ROADOBSTABI,AR0
    // asm: 	RPTS	49
    // asm: 	STI	R0,*AR0++		;CLEAR OUT THE TABLE
    // *CHICAGO KLUDGE
    // *GET CURRENT RACE
    // asm: 	LDI	@BONUS_WAVE,R0
    // asm: 	CMPI	10,R0
    // asm: 	BNE	OI4			;NOT CHICAGO
    // asm: 	LDI	1000H,R0	  	;BLOCK OUT LANES 2 LANE CASE
    // asm: 	LDI	@ROADOBSTABI,AR0
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	LDI	*+AR2(OUSR1),R1
    // asm: 	CMPI	@WACKER,R1
    // asm: 	BLT	OIX			;STRAIGHT 4 LANE
    // asm: 	CMPI	@LAKEL,R1
    // asm: 	BGT	OI1
    // asm: 	LDI	*+AR7(DELTA_INIT),R1	;CHECK STARTING POSITION
    // asm: 	LSH	-1,R1
    // asm: 	LDIC	0,R1			;BLOCKOUT ONE SIDE
    // asm: 	LDINC	23,R1
    // asm: 	ADDI	R1,AR0
    // asm: 	LDI	26,RC
    // asm: 	RPTB	OILP00
OILP00:
    // asm: STI	R0,*AR0++
    // asm: 	RETS
OI1:
    // asm: 	LDI	*+AR7(DELTA_INIT),R1	;CHECK STARTING POSITION
    // asm: 	CMPI	3,R1
    // asm: 	BLT	OI10
    // asm: 	LSH	-1,R1
    // asm: 	BC	OI2
OI10:
    // asm: 	LDI	13,RC
    // asm: 	RPTB	OILP0
    // asm: 	STI	R0,*+AR0(36)		;BLOCK OUT BOTH ENDS
OILP0:
    // asm: STI	R0,*AR0++
    // asm: 	RETS
OI2:
    // asm: 	STI	R0,*+AR0(36)
    // asm: 	STI	R0,*+AR0(37)		;EL TRACK PILLAR BLOCKOUT
    // asm: 	STI	R0,*+AR0(38)
    // asm:  	STI	R0,*+AR0(39)
    // asm: 	STI	R0,*+AR0(40)
    // asm: 	STI	R0,*+AR0(09)
    // asm: 	STI	R0,*+AR0(10)
    // asm: 	STI	R0,*+AR0(11)
    // asm: 	STI	R0,*+AR0(12)
    // asm: 	STI	R0,*+AR0(13)
    // asm: 	RETS
    // *END CHICAGO KLUDGE
OI4:
    // asm: 	LDI	*+AR7(DELTA_TPIECE),AR2
    // asm: 	CALL	GET_LANES	 	;GET # OF LANES
    // asm: 	LDI	R0,R0
    // asm: 	BNZ	OIX			;FOUR LANE CASE
    // asm: 	LDI	1000H,R0	  	;BLOCK OUT LANES 2 LANE CASE
    // asm: 	LDI	@ROADOBSTABI,AR0
    // asm: 	LDI	11,RC
    // asm: 	RPTB	OILP
    // asm: 	STI	R0,*+AR0(38)		;BLOCK OUT BOTH ENDS
OILP:
    // asm: STI	R0,*AR0++
OIX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBSTABINIT", 0, 0);
    UNIMPL();
}

void GETRDOFFSET(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *EXAMINE ROAD OBSTACLE MAP TO GET OFFSET CHANGE IF ANY
    // *
    // *PARAMETERS
    // *	AR4	CAR
    // *	AR5	CAR BLOCK
    // *	AR7	PROCESS
    // *RETURNS
    // *	R0	NEW ROAD OFFSET
    // *TRASHES AR2
    // *
    // asm: 	LDI	0,R6		;LFT SCAN INIT
    // asm: 	LDI	0,R7		;RT SCAN INIT
    // asm: 	NEGF	*+AR5(CARDIST2CNTR),R0
    // asm: 	MPYF	0.01,R0		;DIVIDE BY 100
    // asm: 	FIX	R0
    // asm: 	ADDI	21,R0	     	;LEFT EDGE OF ZONE
    // asm: 	CMPI	41,R0		;MIN AND MAXES
    // asm: 	LDIGT	41,R0
    // asm: 	CMPI	0,R0		;MIN AND MAXES
    // asm: 	LDILT	0,R0
    // asm: 	LDI	@ROADOBSTABI,AR0
    // asm: 	ADDI	R0,AR0
    // *CHECK CURRENT GAP
    // asm: 	LDI	*AR0++,R1
    // asm: 	RPTS	6
    // asm: 	OR	*AR0++,R1
    // asm: 	BZ	OK			;WERE O.K.
    // *CHECK FOR PLAYER
    // asm: 	CMPI	4000H,R1  		;PLAYER PRESENT?
    // asm: 	BLT	S000			;NO
    // asm: 	AND	0FFFH,R1       		;BRAKE IF REAL CLOSE...
    // asm: 	CMPI	20,R1
    // asm: 	BGT	S00			;NOT REAL CLOSE
    // ;Check for COPCAR
    // asm: 	LDI	*+AR5(CAR_ID),R2
    // asm: 	CMPI	DRONE_C|VEHICLE_T|DRNE_COPCAR,R2
    // asm: 	BEQ	S000			;COPCARS DON'T SLOW DOWN FOR THE PLAYER
    // asm: 	LDI	@PLYCBLK,AR2		;GET PLAYER CAR OBJECT
    // asm: 	LDF	*+AR2(CARSPEED),R2	;PLAYER SPEED
    // asm: 	SUBF	*+AR5(CARSPEED),R2
    // asm: 	CMPF	10,R2
    // asm: 	BGT	S000			;DONT BRAKE, PLAYER SPEED GREATER
    // asm: 	FLOAT	R1
    // asm: 	MPYF	0.01,R1			;BRAKE FACTOR CALC
    // asm: 	ADDF	0.80,R1
    // asm: 	MPYF	*+AR5(CARSPEED),R1
    // asm: 	STF	R1,*+AR5(CARSPEED)	;CUT SPEED
S00:
S000:
    // *WE HAVE AN OBSTACLE
    // *SCAN RIGHT 0--->49
    // *ADJUST COUNT FOR END
    // asm: 	LDI	R0,R3
    // asm: 	LDI	30,R2		;SEARCH 20 SPACES RIGHT
    // asm: 	ADDI	R2,R3
    // asm: 	SUBI	49,R3
    // asm: 	BLE	SR00
    // asm: 	SUBI	R3,R2		;DECREASE COUNT TO AVOID OVERSCAN
SR00:
    // asm: 	LDI	@ROADOBSTABI,AR0
    // asm: 	ADDI	R0,AR0
    // asm: 	LDI	0,R1		;SEQUENCE COUNT
SR0:
    // asm: 	LDI	*AR0++,R0
    // asm: 	LDINZ	0,R1 		;NON ZERO RESET COUNT
    // asm: 	ADDI	1,R1
    // asm: 	CMPI	8,R1
    // asm: 	BGT	FOUNDR	 	;FOUND A GAP
    // asm: 	SUBI	1,R2
    // asm: 	BNZ	SR0
    // asm:       	B 	LEFTSCAN	;FOUND NOTHING
FOUNDR:
    // asm: 	SUBI	4,AR0		;GET CENTER INDEX
    // asm: 	LDI	AR0,R6		;SAVE VALUE
    // *WE HAVE AN OBSTACLE
    // *SCAN LEFT 49--->0
LEFTSCAN:
    // asm: 	NEGF	*+AR5(CARDIST2CNTR),R0
    // asm: 	MPYF	0.01,R0		;DIVIDE BY 100
    // asm: 	FIX	R0
    // asm: 	ADDI	29,R0	     	;RT EDGE OF ZONE
    // asm: 	CMPI	49,R0		;MIN AND MAXES
    // asm: 	LDIGT	49,R0
    // asm: 	CMPI	7,R0		;MIN AND MAXES
    // asm: 	BLT	DONE		;NOT ENOUGH ROOM ON THE LEFT
    // asm: 	LDI	@ROADOBSTABI,AR0
    // asm: 	ADDI	R0,AR0
    // asm: 	LDI	30,R2		;COUNT
    // asm: 	SUBI	R2,R0
    // asm: 	BGT	SL00
    // asm: 	ADDI	R0,R2		;DECREASE COUNT TO AVOID OVERSCAN
SL00:
    // asm: 	LDI	0,R1		;SEQUENCE COUNT
SL0:
    // asm: 	LDI	*AR0--,R0
    // asm: 	LDINZ	0,R1 		;NON ZERO RESET COUNT
    // asm: 	ADDI	1,R1
    // asm: 	CMPI	8,R1
    // asm: 	BGT	FOUNDL	 	;FOUND A GAP
    // asm: 	SUBI	1,R2
    // asm: 	BNZ	SL0
    // asm: 	B 	DONE
    // *
    // *FOUND A GAP ON THE LEFT
    // *AR0 HAS GAP
    // *
FOUNDL:
    // asm: 	ADDI	4,AR0
    // asm: 	LDI	AR0,R7
    // *
    // *FOUND A GAP
    // *R6=LEFT GAP, R7=RIGHT GAP
    // *
DONE:
    // asm: 	ADDI	R6,R7,R0
    // asm: 	BZ	FAIL			;NO GAP FOUND, GO WITH OLDIE
    // asm: 	LDI	R6,R6			;CHECK LFT FAIL
    // asm: 	LDIZ	R7,R6
    // asm: 	LDI	R7,R7			;CHECK RT FAIL
    // asm: 	LDIZ	R6,R7
    // asm: 	SUBI	@ROADOBSTABI,R6		;FIND CLOSEST DIRECTION
    // asm: 	SUBI	@ROADOBSTABI,R7
    // asm: 	LDF	*+AR7(ROADOFFSET),R0
    // asm: 	MPYF	0.01,R0			;DIVIDE BY 100
    // asm: 	FIX	R0
    // asm: 	SUBI	R6,R0,R2		;FIND CLOSEST RT OR LFT
    // asm: 	SUBI	R7,R0,R3
    // asm: 	ABSI	R2
    // asm: 	ABSI	R3
    // asm: 	CMPI	R2,R3
    // asm: 	LDILT	R7,R0
    // asm: 	LDIGE	R6,R0
    // asm: 	SUBI	25,R0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	100,R0	    		;NEW OFFSET
    // asm: ROXX
    // asm: 	RETS
FAIL:
    // asm: 	NOP
    // asm: OK					;CURRENT OFFSET IS O.K.
    // asm: 	LDF	*+AR7(ROADOFFSET),R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETRDOFFSET", 0, 0);
    UNIMPL();
}

void CARCHEK(void)
{
    // asm: 	STI	R0,@CARTMP1   		;SAVE CAR CODE
    // asm: 	LDI	*+AR2(OCARBLK),AR3
    // asm: 	CALL	CKAHEAD
    // asm: 	BN	CARCHKX 		;CAR BEHIND, IGNORE
    // *GET CURRENT DISTANCE
    // asm: 	SUBF	*+AR4(OPOSX),*+AR2(OPOSX),R6
    // asm: 	LDF	*+AR2(OPOSZ),R7
    // asm: 	SUBF	*+AR4(OPOSZ),R7
    // asm: 	MPYF	R6,R6,R1
    // asm: 	MPYF	R7,R7,R2
    // asm: 	ADDF	R1,R2			;distance^2
    // asm: 	FLOAT	30000,R0
    // asm: 	MPYF	R0,R0
    // asm: 	CMPF	R2,R0
    // asm: 	BLT	CARCHKX			;TOO FAR AWAY
    // *CHECK IF CAR BLOCKING THE SIDE
    // asm: 	LDF	*+AR3(CARZPLUS),R1
    // asm: 	ADDF	*+AR5(CARZPLUS),R1
    // asm: 	ADDF	200,R1			;LEAVE A LITTLE ROOM
    // asm: 	MPYF	R1,R1			;GET 1/2 Z1+Z2 SQUARED
    // asm: 	CMPF	R1,R2			;CLOSE ENOUGH ?
    // asm: 	BGT	SIDECKX			;NO...
    // asm: 	LDF     *+AR5(CARDIST2CNTR),R1
    // asm: 	SUBF	*+AR3(CARDIST2CNTR),R1
    // asm: 	ABSF	R1
    // asm: 	FLOAT	400,R3
    // asm: 	CMPF	R3,R1
    // asm: 	BLT	SIDECKX			;OFFSET TOO CLOSE
    // asm: 	LDF	10,R0
    // asm: 	PUSHF	R0
    // asm: 	B	GOTONE
SIDECKX:
    // asm: 	CALL	SQRT
    // asm: 	LDF	R0,R5
    // *GET DISTANCE T+1
    // *CAR 0 T+1
    // asm: 	LDF	*+AR5(CARSPEED),R1
    // asm: 	LDF	*+AR5(CARVROT),R2
    // asm: 	CALL	_SINE
    // asm: 	NEGF	R0
    // asm: 	MPYF	R0,R1,R3
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R0,R1,R4
    // asm: 	SUBF	R3,R6
    // asm: 	SUBF	R4,R7
    // *CAR 1 T+1
    // asm: 	LDF	*+AR3(CARSPEED),R1
    // asm: 	LDF	*+AR3(CARVROT),R2
    // asm: 	CALL	_SINE
    // asm: 	NEGF	R0
    // asm: 	MPYF	R0,R1,R3
    // asm: 	CALL	_COSI
    // asm: 	MPYF	R0,R1,R4
    // asm: 	ADDF	R3,R6
    // asm: 	ADDF	R4,R7
    // asm: 	MPYF	R6,R6,R1
    // asm: 	MPYF	R7,R7,R2
    // asm: 	ADDF	R1,R2			;distance^2
    // asm: 	CALL	SQRT			;DISTANCE T+1
    // asm: 	SUBF	R5,R0
    // asm: 	BGE	CARCHKX			;MOVING APART
    // *FIND CLOSING RATE
    // asm: 	ABSF	R0,R1
    // asm:  	MPYF	1.5,R1			;VELOCITY FUDGE FACTOR
    // asm: 	ABSF	R6    			;ADJUST DELTA X FOR CAR	THICKNESS
    // asm: 	FLOAT	400,R0
    // asm: 	SUBF	R0,R6
    // asm: 	LDFLT	0,R6
    // asm: 	ABSF	R7    			;ADJUST DELTA Z FOR CAR THICKNESS
    // asm: 	FLOAT	1000,R0
    // asm: 	SUBF	R0,R7
    // asm: 	LDFLT	0,R7
    // asm: 	MPYF	R6,R6,R0
    // asm: 	MPYF	R7,R7,R2
    // asm: 	ADDF	R0,R2			;distance^2
    // asm: 	CALL	SQRT			;DISTANCE T+1
    // asm: 	CALL	DIV_F			;CLOSING FRAMES
    // asm: 	CMPF	200,R0
    // asm: 	BGT	CARCHKX			;TOO FAR AWAY
    // asm: 	PUSHF	R0
    // *GOT A LIVE ONE
    // *BLANK OUT TABLE SPACE
GOTONE:
    // asm: 	CALL	GETPV 			;GET PERPENDICULAR VELOCITY
    // asm: 	POPF	R1
    // asm: 	MPYF	R1,R0,R2		;PROJECTED OFFSET
    // asm: 	FIX	R1,IR1			;SAVE TIME
    // asm: 	SUBF	*+AR3(CARDIST2CNTR),R2
    // asm: 	MPYF	0.01,R2
    // asm: 	FIX	R2,IR0
    // asm: 	ADDI	25,IR0			;ADJUST FOR ROAD CENTER (25), CAR THICKNESS (4)
    // asm: 	CALL	GETWIDTH		;GET CAR WIDTH
    // asm: 	MPYF	0.01,R0
    // asm: 	ABSF	R0
    // asm: 	CMPF	20,R0
    // asm: 	LDFGT	20,R0
    // asm: 	FIX	R0,R0
    // asm: 	ADDI	3,R0
    // asm: 	LDI	R0,RC			;WIDTH COUNT
    // asm: 	LSH	-1,R0			;DIVIDE BY 2
    // asm: 	SUBI	R0,IR0			;CENTER UP CAR AREA
    // asm: 	LDI	@ROADOBSTABI,AR0
    // asm: 	LDI	@CARTMP1,R1		;GET CODE TO STORE
    // asm: 	ADDI	IR1,R1			;ADD IN THE TIME
    // asm: 	RPTB	LOADLP
    // asm: 	CMPI	0,IR0
    // asm: 	BLT	LOADLP
    // asm: 	CMPI	49,IR0
    // asm: 	BGT	LOADLPX
    // asm: 	STI	R1,*+AR0(IR0)  		;PUT IN ENTRY IN TABLE
LOADLP:
    // asm: ADDI	1,IR0
LOADLPX:
    // asm: 	LDI	IR1,R0			;RETURN TIME DUDES
    // asm: 	RETS
CARCHKX:
    // asm: 	LDI	800H,R0		;FOUND NO CLOSING TIME
    // asm:        	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CARCHEK", 0, 0);
    UNIMPL();
}

void GETPV(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET PERPENDICULAR VELOCITY
    // *OF CAR TO ROAD
    // *
    // *PARAMETERS
    // *	AR2	CAR IN QUESTION
    // *	AR3	CAR BLOCK
    // *RETURNS
    // *	R0	PERP SPEED +RIGHT -LEFT
    // *	R2	ANGLE
    // *
    // asm: 	LDI	*+AR2(OCARBLK),AR3
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR3(CARTRAK),AR2
    // asm: 	CALL	GETRDIR	 		;R0= RADIANS
    // asm: 	SUBF	*+AR3(CARVROT),R0
    // asm: 	LDF	R0,R2
    // asm: 	CALL	_SINE
    // asm: 	MPYF	*+AR3(CARSPEED),R0
    // asm: 	MPYF	1.5,R0
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETPV", 0, 0);
    UNIMPL();
}

void GETWIDTH(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GET CAR WIDTH ON ROAD
    // *
    // *PARAMETERS
    // *	AR2	CAR IN QUESTION
    // *	AR3	CAR BLOCK
    // *RETURNS
    // *	R0	WIDTH
    // *	R1	HEIGHT
    // *
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR3(CARTRAK),AR2
    // asm: 	CALL	GETRDIR	 		;R0= RADIANS
    // asm: 	POP	AR2
    // asm: 	SUBF	*+AR3(CARVROT),R0
    // asm: 	LDF	R0,R2
    // asm: 	CALL	_SINE
    // asm: 	LDF	R0,R1
    // asm: 	CALL	_COSI
    // asm: 	LDF	*+AR3(CARXPLUS),R2
    // asm: 	SUBF	*+AR3(CARXMINUS),R2
    // asm: 	LDF	*+AR3(CARZPLUS),R3
    // asm: 	SUBF	*+AR3(CARZMINUS),R3
    // asm: 	ABSF	R0
    // asm: 	ABSF	R1
    // asm: 	MPYF	R0,R2,R4	     	;COS*X+SIN*Z WIDTH TERMS
    // asm: 	MPYF	R1,R3,R5
    // asm: 	MPYF	R1,R2,R2		;SIN*X+COS*Z HEIGHT TERMS
    // asm: 	MPYF	R0,R3,R3
    // asm: 	ADDF	R4,R5,R0 		;WIDTH
    // asm: 	ADDF	R2,R3,R1		;HEIGHT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETWIDTH", 0, 0);
    UNIMPL();
}

void RPASS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CHECK FOR RACER PASSING SOUND
    // *
    // *PARAMETERS
    // *	AR4	RACER CAR OBJ
    // *	AR5	RACER CAR STRUCT
    // *
    // asm: 	LDI	*+AR7(PASSCNT),R0      	;INHIBIT PASSING SOUND?
    // asm: 	BZ	RPASS1
    // asm: 	SUBI	1,R0
    // asm: 	LDIN	0,R0
    // asm: 	STI	R0,*+AR7(PASSCNT)
    // asm: 	RETS
RPASS1:
    // asm: 	LDI	@PLYCAR,AR2		;GET PLAYER CAR OBJECT
    // asm: 	LDI	*+AR2(OCARBLK),AR3
    // asm: 	LDF	*+AR5(CARSPEED),R3	;SPEED GREATER?
    // asm: 	SUBF	*+AR3(CARSPEED),R3
    // asm: 	CMPF	40,R3
    // asm: 	BGT	RPASS2
    // asm: 	ABSF	R3
    // asm: 	CMPF	20,R3
    // asm: 	BLT	RPASSX
    // asm: 	CMPF	60,R3
    // asm: 	LDFGT	60,R3
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	CMPI	0,R0
    // asm: 	BLT	RPASSX
    // asm: 	CMPI	3000,R0
    // asm: 	BGT	RPASSX
    // asm: 	B	RPASS3
RPASS2:
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	CMPI	1500,R0
    // asm: 	BLT	RPASSX
    // asm: 	CMPI	5000,R0
    // asm: 	BGT	RPASSX
RPASS3:
    // asm: 	LDF	*+AR5(CARDIST2CNTR),R0
    // asm: 	SUBF	*+AR3(CARDIST2CNTR),R0
    // asm: 	ABSF	R0,R1
    // asm: 	FLOAT	4000,R0
    // asm: 	CALL	DIV_F
    // asm: 	CMPF	1.0,R0
    // asm: 	LDFGT	1.0,R0
    // asm: 	FLOAT	255,R1
    // asm: 	MPYF	R1,R0			;VOLUME FACTOR
    // asm: 	FIX	R0
    // asm: 	LDI	SMCARPASS,AR2		;DIFFERENT SOUND DIFF VEL
    // asm: 	CMPF	40,R3
    // asm: 	LDIGT	FCARPASS,AR2
    // asm: 	CMPF	60,R3
    // asm: 	BLT	RPASS4
    // asm: 	FIX	R3
    // asm: 	LSH	-1,R3
    // asm: 	LDIC	SPCARPASS,AR2
RPASS4:
    // asm: 	CALL	VOLSNDFX
    // asm: 	LDI	40,R0
    // asm: 	STI	R0,*+AR7(PASSCNT)
RPASSX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RPASS", 0, 0);
    UNIMPL();
}

void SPOS_INIT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *INITIALIZE RACER STARTING POSITION
    // *
    // *PARAMETERS
    // *	AR4	OBJECT
    // *	AR2	TRAKING OBJECT PLAYER STARTS OUT ON
    // *	AR3	RANK FORWARD
    // *RETURNS
    // *	R2	(FL) FACING THETA
    // *
    // *
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	LDI	@VECTORBI,AR5
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	STF	R0,*+AR5(X)
    // asm: 	LDF	*+AR2(OPOSY),R0
    // asm: 	STF	R0,*+AR5(Y)
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	STF	R0,*+AR5(Z)
    // asm: 	LDI	*+AR2(OLINK4),AR2	;GET *NEXT* PIECE
LOOP56:
    // asm: 	LDF	*+AR2(OPOSX),R2
    // asm: 	SUBF	*+AR5(X),R2
    // asm: 	LDF	*+AR2(OPOSZ),R3
    // asm: 	SUBF	*+AR5(Z),R3
    // asm: 	CALL	ARCTANF
    // asm: 	SUBF	HALFPI,R0
    // asm: 	LDF	R0,R2			;THIS IS INITIAL FACING
    // asm: 	CMPI	0,AR3
    // asm: 	BEQ	OFFANDDONE
    // asm: 	FLOAT	5500,R0
    // asm: 	CALL	DISTANCE_2D
    // asm: 	ADDF	*+AR5(X),R0
    // asm: 	STF	R0,*+AR5(X)
    // asm: 	ADDF	*+AR5(Z),R1
    // asm: 	STF	R1,*+AR5(Z)
    // asm: 	DEC	AR3
    // asm: 	CMPI	0,AR3
    // asm: 	BLE	OFFANDDONE
LOOP56A:
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	SUBF	*+AR5(X),R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR2(OPOSZ),R2
    // asm: 	SUBF	*+AR5(Z),R2
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R0,R2
    // asm: 	CALL	SQRT
    // asm: 	FLOAT	5000,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGT	LOOP56
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	BU	LOOP56A
OFFANDDONE:
    // asm: 	STI	AR2,*+AR7(DELTA_TPIECE)
L874:
    // asm: LDI	*+AR2(OUSR1),R0
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm: 	PUSHFL	R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	CALL	CLR_VECTORA
    // asm: 	LDF	*+AR7(DELTA_XLANE),R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORAI,AR0
    // asm: 	LDF	*+AR5(X),R0
    // asm: 	ADDF	*+AR0(X),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR5(Y),R0
    // asm: 	SUBF	*+AR5(CARWHLTAB+1),R0
    // asm: 	ADDF	*+AR0(Y),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR5(Z),R0
    // asm: 	ADDF	*+AR0(Z),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	POPFL	R2
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPOS_INIT", 0, 0);
    UNIMPL();
}

void WRECKST(void)
{
    // *----------------------------------------------------------------------------
    // *START PLAYER WRECK
    // *
    // *PARAMETERS
    // *	AR4	PLAYER OBJECT
    // *	AR5	PLAYER CAR BLOCK
    // *	AR7	PROCESS BLOCK
    // *
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR7
    // asm: 	LDI	@PLYCAR,AR4
    // asm: 	LDI	@PLYCBLK,AR5
    // asm: 	LDI	@PLYPROC,AR7
    // asm: 	LDI	@CAMVIEW,R0
    // asm: 	STI	R0,*+AR7(PDATA+21)	;SAVE OLD VIEW...
    // asm: 	BNZ	WRKST1
    // asm: 	LDI	@VIEW2I,AR2
    // asm: 	LDI	UTIL_C,R2
    // asm: 	CALL	PRC_CREATE		   	;SETUP VIEW 1 IF FIRST PERSON
WRKST1:
    // asm: 	LDF	*+AR5(CT_PRDYD),R0 	;DELTA Y TO ROAD
    // asm: 	ADDF	*+AR4(OPOSY),R0		;ADD Y POSTION OF CAR
    // asm: 	STF	R0,*+AR7(PDATA+20)	;ABSOLUTE Y OF ROAD
    // *GET YOUR RADIANS
    // asm: 	LDF	0.104,R0
    // asm: 	STF	R0,*+AR7(PDATA)	  	;X RADIANS
    // asm: 	LDF	0.104,R0
    // asm: 	STF	R0,*+AR7(PDATA+1)	;Y RADIANS
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(PDATA+2)	;Z RADIANS
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(PDATA+3)	;X RADIAN TOTAL
    // asm: 	STF	R0,*+AR7(PDATA+5)  	;Z RADIAN TOTAL
    // asm: 	LDF	*+AR5(CARYROT),R0	;GET CAR Y ROT
    // asm: 	STF	R0,*+AR7(PDATA+4)
    // asm: 	LDF	-60,R0
    // asm: 	STF	R0,*+AR4(OVELY)		;STUFF VERTICAL VELOCITY
    // asm: 	LDI	1,R0	    		;SET WRECK FLAG
    // asm: 	STI	R0,@WRECKFLG
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CARSHAD)	;TURN OFF SHADOW
    // asm: 	POP	AR7
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WRECKST", 0, 0);
    UNIMPL();
}

void WRECK(void)
{
    // asm: 	CALL	GETTRAK			;KEEP UPDATING YOUR TRACKER
    // asm: 	FLOAT	@NFRAMES,R1	 	;ADJUST MATRIX FOR FRAME COUNT
    // asm: 	LDF	*+AR7(PDATA),R0		;ACCUMULATE X RADIANS
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(PDATA+3),R0
    // asm: 	STF	R0,*+AR7(PDATA+3)
    // asm: 	LDF	*+AR7(PDATA+1),R0      	;ACCUMULATE Y RADIANS
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(PDATA+4),R0
    // asm: 	STF	R0,*+AR7(PDATA+4)
    // asm: 	LDF	*+AR7(PDATA+2),R0 	;ACCUMULATE Z RADIANS
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(PDATA+5),R0
    // asm: 	STF	R0,*+AR7(PDATA+5)
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
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	CALL	ROADSCAN		;CHECK WHEEL HITS
    // asm: 	LDI	*+AR4(OCARBLK),AR5	;GET CAR DATA AREA
    // *KEEP Y ABOVE THE ROAD
    // asm: 	LDI	*+AR5(CT_PCOL),R0
    // asm: 	BZ	WRECK1
    // asm: 	LDF	*+AR5(CT_PRDYD),R0	;DELTA Y TO ROAD
    // asm: 	ADDF	*+AR4(OPOSY),R0		;ADD Y POSTION OF CAR
    // asm: 	LDF	R0,R1
    // asm: 	SUBF	*+AR7(PDATA+20),R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	STF	R1,*+AR7(PDATA+20)	;SAVE NEW BASE LINE
    // *CHECK FOR THE END
WRECK1:
    // asm: 	FLOAT	@NFRAMES,R2
    // asm: 	MPYF	2,R2			;FRAME ADJUSTED GRAVITY
    // asm: 	ADDF	*+AR4(OVELY),R2
    // asm: 	STF	R2,*+AR4(OVELY)
    // asm: 	CMPF	60,R2
    // asm: 	RETSLT				;NOT OVER YET
    // *END IT ALL
    // asm: 	LDI	0,R0			;END IT ALL
    // asm: 	STI	R0,@WRECKFLG
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR5(CARSHAD)	;TURN ON SHADOW
    // asm: 	SONDFX	BOTTOMOUT		;BOTTOM OUT SOUND
    // asm: 	LDI	*+AR7(PDATA+21),R0	;RETURN TO FIRST PERSON?
    // asm: 	BNZ	WRECKSLP
    // asm: 	LDI	@VIEW0I,AR2
    // asm: 	LDI	UTIL_C,R2
    // asm: 	CALL	PRC_CREATE		   	;RETURN TO 1ST PERSON
WRECKSLP:
    // asm: 	RETS
    // *
    // *PLAYER DRAFT CHECK
    // *AR4=PLAYER OBJECT
    // *AR5=PLAYER CAR BLOCK
    // *
    // ;PLYRDRAFT:
    // ;*DECAY DRAFT VALUE
    // ;	LDF	@PLDRAFTVAL,R0
    // ;	SUBRF	1.0,R0
    // ;	MPYF	0.97,R0
    // ;	SUBRF	1.0,R0
    // ;	CMPF	1.0,R0
    // ;	LDFGT	1.0,R0
    // ;	STF	R0,@PLDRAFTVAL
    // ;
    // ;*SCAN DRONES FOR POSSIBLE DRAFT
    // ;
    // ;	LDI	*+AR5(CARTRAK),AR0
    // ;	LDI	*+AR0(OUSR1),R5		;GET CURRENT CAR TRACK
    // ;
    // ;	LDF	*+AR5(CARDIST2CNTR),R6 	;GET OUR LANE POSITION
    // ;	LDI	@CAR_LIST,AR2
    // ;	LDI	AR2,R0
    // ;	BZ	DRAFTX
    // ;DRAFTL
    // ;	LDI	*+AR2(OFLAGS),R0
    // ;	TSTB	O_NOCOLL,R0		;STEALTH MODE ?
    // ;	BNZ	DRAFTLP		    	;YES, DONT BOTHER
    // ;	LDI	*+AR2(OCARBLK),AR3	;QUICK FRONT/BACK CHECK
    // ;
    // ;	LDF	*+AR3(CARDIST2CNTR),R2 	;GET LANE POSITION
    // ;	SUBF	R6,R2,R4
    // ;	ABSF	R4
    // ;	CMPF	250,R4
    // ;	BGT	DRAFTLP
    // ;
    // ;	LDF	*+AR3(CARSPEED),R0	;IS SPEED CLOSE???
    // ;	SUBF	*+AR5(CARSPEED),R0
    // ;	ABSF	R0
    // ;	CMPF	50,R0
    // ;	BGT	DRAFTLP
    // ;
    // ;	LDI	*+AR3(CARTRAK),AR0
    // ;	CMPI	*+AR0(OUSR1),R5
    // ;	BGT	DRAFTLP
    // ;	BLT	DRAFT1
    // ;
    // ;	CALL	CKAHEAD
    // ;	BLT	DRAFTLP
    // ;DRAFT1
    // ;	SUBF	*+AR4(OPOSX),*+AR2(OPOSX),R2	;DELTA X
    // ;
    // ;	LDF	*+AR2(OPOSZ),R1			;DELTA Z
    // ;	SUBF	*+AR4(OPOSZ),R1
    // ;
    // ;	MPYF	R2,R2
    // ;	MPYF	R1,R1
    // ;	ADDF	R1,R2
    // ;
    // ;	FLOAT	5000,R1				;TO THE NEXT ROADPIECE
    // ;	MPYF	R1,R1
    // ;	CMPF	R1,R2				;DISTANCE LT 5000 ?
    // ;	BGT	DRAFTLP
    // ;	CALL	SQRT
    // ;
    // ;	FLOAT	1000,R1
    // ;	SUBF	R1,R0
    // ;	LDFLT	0,R0
    // ;
    // ;	MPYF	0.1,R0
    // ;	ADDF	R4,R0
    // ;
    // ;	MPYF	0.01,R0
    // ;	MPYF	0.02,R0
    // ;	ADDF	0.87,R0
    // ;
    // ;	ADDF	@PLDRAFTVAL,R0	  		;GRADUAL EFFECT
    // ;	MPYF	0.5,R0
    // ;	STF	R0,@PLDRAFTVAL
    // ;	RETS
    // ;
    // ;*WE HAVE A DRAFT HERE
    // ;
    // ;DRAFTLP
    // ;	LDI	*+AR2(OLINK3),AR2
    // ;	LDI	AR2,R0
    // ;	BNZ	DRAFTL
    // ;DRAFTX
    // ;	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WRECK", 0, 0);
    UNIMPL();
}

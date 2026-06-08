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
#include "../include/comm.h"
#include "../include/racer.h"
#include "../include/h2hobj.h"
#include "../include/commdrne.h"

/*
 * Source module: asm/COMMDRNE.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *----------------------------------------------------------------------------
// *Startup Position Table
// *
// *39 words per plyr (13*3)
// *
// *
/* asm: STARTUP_POS_TABLEI	.word	STARTUP_POS_TABLE */
int STARTUP_POS_TABLEI = (int)(STARTUP_POS_TABLE);
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------

void STARTUP_POS_TABLE(void)
{
    //  ;PLYR1  (on PLYR2)
    // 	;PLYR2 (on PLYR1)
    /* no executable asm lines detected */
    UNIMPL();
}

void COMM_DRONE(void)
{
    // *----------------------------------------------------------------------------
    // *Communications drone
    // *
    // *
    // *
#if CDEBUG
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	$
#endif
    // asm: 	LDI	R0,AR1
    // asm: 	CMPI	4,AR1
    // asm: 	LDIEQ	JEEP_MOD,R0
    // asm: 	CMPI	5,AR1
    // asm: 	LDIEQ	PLYR_SBUS_MOD,R0
    // asm: 	CMPI	6,AR1
    // asm: 	LDIEQ	PLYR_COPCAR_MOD,R0
    // asm: 	CMPI	7,AR1
    // asm: 	LDIEQ	3,R0
    // asm: DOGENRLB
    // asm: 	STI	R0,*+AR7(DELTA_MODEL)
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	R0,AR5
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDI	*+AR2(VEHTAB_MODEL),AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	BC	SUICIDE
    // asm: 	LDI	AR0,AR4
    // *STORE OBJECT POINTER
    // asm: 	STI	AR4,@PLY2CAR
    // asm: 	LDI	R4,AR1
    // asm: 	ADDI	@RACER_PTRI,AR1
    // asm: 	STI	AR4,*AR1
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	VEHICLE_ANI_INIT
    // asm: 	CALL	DELTA_OINIT
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
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR5(CAR_OM)    	;SET OTHER MACHINES VEHICLE FLAG
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
    // asm: 	LDF	1.0,R0	   		;FOR ATTRACT ALWAYS 1 DIFFICULTY
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
    // asm: 	STF	R0,*+AR5(CARSPEED)
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
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	CALL	CAR_ROAD_COLL		;MAKE SORT WORK RIGHT
    // asm: 	LDF	*+AR5(CARYROT),R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	R2,R3
    // asm: 	CALL	CONCATMAT      		;FIX THE MATRIX
    // asm: 	LDL	h2p1,AR2
    // asm: 	LDI	*+AR7(DELTA_INIT),R0
    // asm: 	CMPI	8,R0
    // asm: 	BEQ	NNDDF3
    // asm: 	LDL	h2p2,AR2
NNDDF3:
    // asm: CALL	OBJ_GETE
    // asm: 	LDI	AR0,AR2
    // asm: 	LDI	AR0,AR6
    // asm: 	STI	AR0,@COMM_DRONE_PTR
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDI	1,R1
    // asm: 	LS	O_3DROT_B,R1
    // asm: 	LDI	*+AR6(OFLAGS),R0
    // asm: 	OR	O_IROT|O_1PAL|O_NOROT,R0
    // asm: 	OR	R1,R0
    // asm: 	STI	R0,*+AR6(OFLAGS)
    // asm: 	LDL	H2HPAL1,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR6(OPAL)
    // asm: 	LDI	*+AR7(DELTA_INIT),R0
    // asm: 	STI	R0,*+AR6(OCARBLK)
    // asm: 	CMPI	@PLY2CAR,AR4
    // asm: 	BNE	NOTPLYR
    // asm: 	LDI	@CHOSEN_RACE,R0
    // asm: 	CMPI	RACE_APPL,R0
    // asm: 	BGT	NOTPLYR		;washington DC doesn't count
    // asm: 	MPYI	3,R0
    // asm: 	LDI	0,AR2
    // asm: 	LDI	@DIPRAM,R1
    // asm: 	TSTB	CMDP_MASTER,R1
    // 	;Z == MASTER
    // asm: 	LDIZ	39,AR2
    // asm: 	ADDI	R0,AR2
    // asm: 	ADDI	@STARTUP_POS_TABLEI,AR2
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
    // asm: 	CLRI	R5			;blink counter
COMMDRNE_LP:
    // asm: 	PUSH	R5
    // asm: 	PUSH	AR6
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	CALL	ROADSCAN		;MAKE SORT WORK RIGHT
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR6
    // asm: 	POP	R5
    // asm: 	CALL	SEND_OM_TRACK
    // asm: 	SLEEP	1
    // asm: 	BU	COMMDRNE_LP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_DRONE", 0, 0);
    UNIMPL();
}

void COMM_DRONE_PTR_SORT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@COMM_DRONE_PTR,AR6
    // asm: 	LDI	@PLY2CAR,AR4
REGULAR:
    // asm: LDI	*+AR4(ODIST),IR1
    // asm: 	ASH	-4,IR1			;quickly divide by 16
    // asm: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm: 	LDF	*+AR2(IR1),R0
    // asm: 	CALL	INV_F30
    // asm: 	CMPF	16,R0			;
    // asm: 	LDFLT	16,R0			;
    // asm: 	STF	R0,*+AR6(OMAT00)
    // asm: 	STF	R0,*+AR6(OMAT11)
    // asm: 	STF	R0,*+AR6(OMAT22)
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR6(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	FLOAT	35,R1
    // asm: 	MPYF	*+AR6(OMAT00),R1
    // asm: 	SUBF	R1,R0
    // asm: 	SUBF	20,R0
    // asm: 	STF	R0,*+AR6(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR6(OPOSZ)
    // asm: 	LDI	AR6,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	LDI	*AR4,R0
    // asm: 	STI	R0,*AR6
    // asm: 	STI	AR6,*AR4
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	AND	O_LIST_M,R0
    // asm: 	OR	*+AR6(OFLAGS),R0
    // asm: 	STI	R0,*+AR6(OFLAGS)
    // ;
    // ;	CALL	OBJ_INSERT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_DRONE_PTR_SORT", 0, 0);
    UNIMPL();
}

void OM_DRONE(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *OTHER MACHINE DRONE LOOP
    // *
    // asm: 	LDI	1,R0			;OTHER GUYS CAR NOW....
    // asm: 	STI	R0,*+AR5(CAR_OM)
OM_DRONEL:
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	CALL	ROADSCAN		;MAKE SORT WORK RIGHT
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	LDF	0,R0			;CLEAR OUT SOME SHIT
    // asm: 	STF	R0,*+AR5(CARDROT)
    // asm: 	STF	R0,*+AR5(CARSPRAD)
    // asm: 	STF	R0,*+AR5(CARSKID)
    // asm: 	LDF	*+AR5(CARYROT),R0
    // asm: 	STF	R0,*+AR4(ORADY)		;STORE CAR OBJECT RADY
    // asm: 	LDI	0,R0
    // asm: 	STF	R0,*+AR5(CAR_SPIN)
    // ******************************
    // *TEST FOR NO UPDATE
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CARUPD)
    // asm: 	SLEEP	1
    // asm: 	LDI	*+AR5(CARUPD),R0
    // asm: 	BNE	DRONELL
    // asm: 	NOP
DRONELL:
    // ************
    // asm: 	B	OM_DRONEL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OM_DRONE", 0, 0);
    UNIMPL();
}

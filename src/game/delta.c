#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "obj.h"
#include "macs.h"
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

/*
 * Source module: asm/DELTA.ASM
 */

/* *----------------------------------------------------------------------------
*DRONE DYNA ANIMATION PROCESS
*
*	SPINS WHEELS	(ONLY!)
*
*PARAMETERS
*	AR4	CAR OBJECT
*	AR5	CAR BLOCK
*	R6	X RADIANS	;FOR WHEEL SPIN
*USES
*	AR3	DYNA OBJECT
*	AR4	MASTER OBJECT
*	AR5	CAR BLOCK
*	AR6	DYNAMATRIX
*	R4	OLD ORADY
*	R5	Z RADIANS FOR BODY LEAN
*	R6	X RADIANS FOR BODY LEAN
*	R7	OLD CAR SPEED
*	PDATA	OLD CAR ORADY
*	PDATA+1 BODY LEAN Z RADIANS
*	PDATA+2 X RADIANS FOR WHEEL SPIN
*
 */

/* *----------------------------------------------------------------------------
*GENERAL DRONE OBJECT INITIALIZATION
*
*PARAMETERS
*	AR4	OBJECT (NOT INSERTED)
*	AR7	PROCESS (DELTA STYLE)
*RETURNS
*	AR5	CAR BLOCK
*
 */
void DELTA_OINIT(void)
{
    // asm: 	LDI	*+AR7(DELTA_MODEL),R0
    // asm: 	CALL	_CARV0			;RETURNS BLOCK PTR IN AR0
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	CALL	ADD_DRONE
    // asm: 	STI	AR7,*+AR4(OPLINK)
    // asm: 	LDI	1,R0
    // asm: 	LS	O_PROC_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	DM_NORMAL,R0
    // asm: 	STI	R0,*+AR7(DELTA_MODE)
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	LDF	THROTTLE_INIT,R0	;INITIALIZE THROTTLE AND BRAKE
    // asm: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR5(CARBRAKE)
    // asm: 	LDF	MAX_ACCEL_INIT,R0
    // asm: 	STF	R0,*+AR5(CARMAXACCEL)	;SET ACCEL POWER
    // asm: 	LDF	1.0,R0
    // asm: 	STF	R0,*+AR7(DELTA_THROTTLE)
    // asm: 	LDI	0101h,R0		;SET THE STARTUP LAST KNOWN OID
    // asm: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm: 	MPYI	VEHTAB_SIZE,AR2
    // asm: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm: 	LDF	*+AR2(VEHTAB_MASS),R0
    // asm: 	STF	R0,*+AR5(CARMASS)
    // asm: 	LDI	*+AR2(VEHTAB_DMODEL),R0
    // asm: 	BZ	NOTDEGRADE
    // asm: 	STI	R0,*+AR4(ODEGRADE_ROM)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_DEGRADE,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	*+AR2(VEHTAB_DMODEL2),R0
    // asm: 	BZ	NOTDEGRADE
    // asm: 	STI	R0,*+AR4(ODEGRADE_ROM2)
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	OR	O_DEGRADE2,R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
NOTDEGRADE:
    // asm: 	LDI	*+AR4(OID),R0
    // asm: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RHO,R0
    // asm: 	BEQ	NODYNAX1
    // asm: 	LDI	*+AR2(VEHTAB_DDYNATAB),R0
    // asm: 	BZ	NODYNAX1
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	DDYNA_INIT
NODYNAX1:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELTA_OINIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*DDYNA_INIT
*
*DRONE (STRIPPED DOWN) DYNAMIC OBJECT
*
*GET DYNAMIC OBJECTS FOR A CAR
*
*PARAMETERS
*	AR2	TABLE POINTER
*	AR4	CAR OBJECT
*
 */
void DDYNA_INIT(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R6
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	LDI	O_DYNAMIC,R0	 	;MAKE PARENT OBJECT DYNAMIC
    // asm: 	LDI	*+AR4(OFLAGS),R1
    // asm: 	TSTB	R0,R1
    // asm: 	BNZ	VANIX
    // asm: 	OR	R0,R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // 	;INITIALIZE CENTERXYZ,TRANSXYZ,VERTS
    // 	;
    // asm: 	LDI	AR4,AR3
    // asm: 	ADDI	ODYNALIST,AR3
    // asm: 	LDI	*AR2++,RC		;GET DYNAMIC OBJECT COUNT
    // asm: 	DEC	RC
    // asm: 	LDI	*AR2++,R6		;GET NUMBER OF VERTICES
    // asm: 	RPTB	WHEELLP
    // asm:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
    // asm: 	STI	AR0,*AR3
    // asm: 	LDI	*AR2++,R0
    // asm: 	SUBI	R0,R6
    // asm: 	SUBI	1,R0
    // asm: 	STI	R0,*+AR0(DYNANVERTS)
    // asm: 	FLOAT	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERX)
    // asm: 	STF	R0,*+AR0(DYNATRANSX)
    // asm: 	FLOAT	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERY)
    // asm: 	STF	R0,*+AR0(DYNATRANSY)
    // asm: 	FLOAT	*AR2++,R0
    // asm: 	STF	R0,*+AR0(DYNACENTERZ)
    // asm: 	STF	R0,*+AR0(DYNATRANSZ)
WHEELLP:
    // asm: LDI	AR0,AR3
    // asm:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
    // asm: 	STI	AR0,*AR3
    // asm: 	SUBI	1,R6
    // asm: 	STI	R6,*+AR0(DYNANVERTS)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR0(DYNACENTERX)
    // asm: 	STF	R0,*+AR0(DYNATRANSX)
    // asm: 	STF	R0,*+AR0(DYNACENTERY)
    // asm: 	STF	R0,*+AR0(DYNATRANSY)
    // asm: 	STF	R0,*+AR0(DYNACENTERZ)
    // asm: 	STF	R0,*+AR0(DYNATRANSZ)
    // asm: 	LDI	AR0,AR3
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR3			;LAST LINK IS ZERO, DUDES
    // asm: 	ADDI	DYNAMATRIX,AR0
    // asm: 	CALL	INITMAT
    // *GET A CAR PROCESS
    // asm: 	LDI	@DRONE_DANI_PROCI,AR2
    // asm: 	LDI	DRONE_C|ANI_T,R2	;PID
    // asm: 	CALL	PRC_CREATE_CHILD
    // asm: 	STI	AR0,*+AR4(ORADZ)	;DOUBLING AS A PROC PTR
VANIX:
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	POP	R6
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DDYNA_INIT", 0, 0);
    UNIMPL();
}

void DRONE_DANI_PROC(void)
{
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	LDF	0,R6	 		;INIT SPIN RADIANS
    // asm: 	STF	R6,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
    // asm: 	LDF	*+AR5(CARSPEED),R0	;INIT SPEED
    // asm: 	LDF	R0,R7
CARPROCL:
    // asm: 	LDI	3,AR2	  		;SLEEP TIME
    // asm: 	LDI	@SUSPEND_MODE,R0
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BEQ	CARSLP
    // asm: 	LDI	*+AR4(ODIST),R0
    // asm: 	CMPI	20000,R0		;FAR OFF JUST SLEEP
    // asm: 	BGT	CARSLP
    // 	;
    // 	;GET WHEEL SPIN MATRIX
    // 	;
    // asm: 	LDF	*+AR5(CARSPEED),R2
    // asm: 	MPYF	0.02,R2   		;FUDGE FACTOR
    // asm: 	ADDF	*+AR7(PDATA+2),R2
    // asm: 	STF	R2,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
    // asm: 	LDI	@MATRIXBI,AR2		;GET X SPIN IN MATRIXB
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	AR2,AR3			;SAVE REAR WHEEL MATRIX PTR
    // 	;STUFF YOUR DYNAMIC MATRICES
    // 	;
    // asm: 	LDI	*+AR4(ODYNALIST),R0
    // asm: 	SLOCKON	Z,"DELTA\DRONE_DANI_PROC 1"
CDTOP:
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	BZ	CARSLP
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	DYNAMATRIX,AR2
    // 	;STUFF WHEEL
    // 	;
    // asm: 	LDF	*AR3++,R0
    // asm: 	RPTS	7
    // asm: 	LDF	*AR3++,R0
    // asm:  	STF	R0,*AR2++
    // asm: 	NOP	*AR3--(9)
    // asm: 	LDI	*AR0,R0
    // asm: 	BNZ	CDTOP
CARSLP:
    // asm: 	SLEEP	3
    // asm: 	B 	CARPROCL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_DANI_PROC", 0, 0);
    UNIMPL();
}

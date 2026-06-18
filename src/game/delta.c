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

void DELTA_OINIT(void);
void DDYNA_INIT(void);
void DRONE_DANI_PROC(void);

#define DRONE_DANI_PROCI DRONE_DANI_PROC

/*
*----------------------------------------------------------------------------
*DELTA DRONE
*
*COPYRIGHT (C) 1994  BY TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

/*
*----------------------------------------------------------------------------
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
    // asm 0000AE6A: 	LDI	*+AR7(DELTA_MODEL),R0
    // asm 0000AE6B: 	CALL	_CARV0			;RETURNS BLOCK PTR IN AR0
    // asm 0000AE6C: 	LDI	AR4,AR2
    // asm 0000AE6D: 	CALL	OBJ_INSERT
    // asm 0000AE6E: 	CALL	ADD_DRONE
    // asm 0000AE6F: 	STI	AR7,*+AR4(OPLINK)
    // asm 0000AE70: 	LDI	1,R0
    // asm 0000AE71: 	LS	O_PROC_B,R0
    // asm 0000AE72: 	OR	*+AR4(OFLAGS),R0
    // asm 0000AE73: 	STI	R0,*+AR4(OFLAGS)
    // asm 0000AE74: 	LDI	DM_NORMAL,R0
    // asm 0000AE75: 	STI	R0,*+AR7(DELTA_MODE)
    // asm 0000AE76: 	LDI	*+AR4(OCARBLK),AR5
    // asm 0000AE77: 	LDF	THROTTLE_INIT,R0	;INITIALIZE THROTTLE AND BRAKE
    // asm 0000AE78: 	STF	R0,*+AR5(CARTHROTTLE)
    // asm 0000AE79: 	CLRF	R0
    // asm 0000AE7A: 	STF	R0,*+AR5(CARBRAKE)
    // asm 0000AE7B: 	LDF	MAX_ACCEL_INIT,R0
    // asm 0000AE7C: 	STF	R0,*+AR5(CARMAXACCEL)	;SET ACCEL POWER
    // asm 0000AE7D: 	LDF	1.0,R0
    // asm 0000AE7E: 	STF	R0,*+AR7(DELTA_THROTTLE)
    // asm 0000AE7F: 	LDI	0101h,R0		;SET THE STARTUP LAST KNOWN OID
    // asm 0000AE80: 	STI	R0,*+AR7(DELTA_LAST_OID)
    // asm 0000AE81: 	LDI	*+AR7(DELTA_MODEL),AR2
    // asm 0000AE82: 	MPYI	VEHTAB_SIZE,AR2
    // asm 0000AE83: 	ADDI	@VEHICLE_TABLEI,AR2
    // asm 0000AE84: 	LDF	*+AR2(VEHTAB_MASS),R0
    // asm 0000AE85: 	STF	R0,*+AR5(CARMASS)
    // asm 0000AE86: 	LDI	*+AR2(VEHTAB_DMODEL),R0
    // asm 0000AE87: 	BZ	NOTDEGRADE
    // asm 0000AE88: 	STI	R0,*+AR4(ODEGRADE_ROM)
    // asm 0000AE89: 	LDI	*+AR4(OFLAGS),R0
    // asm 0000AE8A: 	OR	O_DEGRADE,R0
    // asm 0000AE8B: 	STI	R0,*+AR4(OFLAGS)
    // asm 0000AE8C: 	LDI	*+AR2(VEHTAB_DMODEL2),R0
    // asm 0000AE8D: 	BZ	NOTDEGRADE
    // asm 0000AE8E: 	STI	R0,*+AR4(ODEGRADE_ROM2)
    // asm 0000AE8F: 	LDI	*+AR4(OFLAGS),R0
    // asm 0000AE90: 	OR	O_DEGRADE2,R0
    // asm 0000AE91: 	STI	R0,*+AR4(OFLAGS)
NOTDEGRADE:
    // asm 0000AE92: 	LDI	*+AR4(OID),R0
    // asm 0000AE93: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RHO,R0
    // asm 0000AE94: 	BEQ	NODYNAX1
    // asm 0000AE95: 	LDI	*+AR2(VEHTAB_DDYNATAB),R0
    // asm 0000AE96: 	BZ	NODYNAX1
    // asm 0000AE97: 	LDI	R0,AR2
    // asm 0000AE98: 	CALL	DDYNA_INIT
NODYNAX1:
    // asm 0000AE99: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELTA_OINIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
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
    // asm 0000AE9A: 	PUSH	R1
    // asm 0000AE9B: 	PUSH	R2
    // asm 0000AE9C: 	PUSH	R6
    // asm 0000AE9D: 	PUSH	AR0
    // asm 0000AE9E: 	PUSH	AR2
    // asm 0000AE9F: 	PUSH	AR3
    // asm 0000AEA0: 	LDI	O_DYNAMIC,R0	 	;MAKE PARENT OBJECT DYNAMIC
    // asm 0000AEA1: 	LDI	*+AR4(OFLAGS),R1
    // asm 0000AEA2: 	TSTB	R0,R1
    // asm 0000AEA3: 	BNZ	VANIX
    // asm 0000AEA4: 	OR	R0,R1
    // asm 0000AEA5: 	STI	R1,*+AR4(OFLAGS)
    // 	;INITIALIZE CENTERXYZ,TRANSXYZ,VERTS
    // 	;
    // asm 0000AEA6: 	LDI	AR4,AR3
    // asm 0000AEA7: 	ADDI	ODYNALIST,AR3
    // asm 0000AEA8: 	LDI	*AR2++,RC		;GET DYNAMIC OBJECT COUNT
    // asm 0000AEA9: 	DEC	RC
    // asm 0000AEAA: 	LDI	*AR2++,R6		;GET NUMBER OF VERTICES
    // asm 0000AEAB: 	RPTB	WHEELLP
    // asm 0000AEAC:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
    // asm 0000AEAD: 	STI	AR0,*AR3
    // asm 0000AEAE: 	LDI	*AR2++,R0
    // asm 0000AEAF: 	SUBI	R0,R6
    // asm 0000AEB0: 	SUBI	1,R0
    // asm 0000AEB1: 	STI	R0,*+AR0(DYNANVERTS)
    // asm 0000AEB2: 	FLOAT	*AR2++,R0
    // asm 0000AEB3: 	STF	R0,*+AR0(DYNACENTERX)
    // asm 0000AEB4: 	STF	R0,*+AR0(DYNATRANSX)
    // asm 0000AEB5: 	FLOAT	*AR2++,R0
    // asm 0000AEB6: 	STF	R0,*+AR0(DYNACENTERY)
    // asm 0000AEB7: 	STF	R0,*+AR0(DYNATRANSY)
    // asm 0000AEB8: 	FLOAT	*AR2++,R0
    // asm 0000AEB9: 	STF	R0,*+AR0(DYNACENTERZ)
    // asm 0000AEBA: 	STF	R0,*+AR0(DYNATRANSZ)
WHEELLP:
    // asm 0000AEBB: LDI	AR0,AR3
    // asm 0000AEBC:  	CALL	GETDYNA	     		;LINK HIM INTO LIST
    // asm 0000AEBD: 	STI	AR0,*AR3
    // asm 0000AEBE: 	SUBI	1,R6
    // asm 0000AEBF: 	STI	R6,*+AR0(DYNANVERTS)
    // asm 0000AEC0: 	CLRF	R0
    // asm 0000AEC1: 	STF	R0,*+AR0(DYNACENTERX)
    // asm 0000AEC2: 	STF	R0,*+AR0(DYNATRANSX)
    // asm 0000AEC3: 	STF	R0,*+AR0(DYNACENTERY)
    // asm 0000AEC4: 	STF	R0,*+AR0(DYNATRANSY)
    // asm 0000AEC5: 	STF	R0,*+AR0(DYNACENTERZ)
    // asm 0000AEC6: 	STF	R0,*+AR0(DYNATRANSZ)
    // asm 0000AEC7: 	LDI	AR0,AR3
    // asm 0000AEC8: 	LDI	0,R0
    // asm 0000AEC9: 	STI	R0,*AR3			;LAST LINK IS ZERO, DUDES
    // asm 0000AECA: 	ADDI	DYNAMATRIX,AR0
    // asm 0000AECB: 	CALL	INITMAT
    // *GET A CAR PROCESS
    // asm 0000AECC: 	LDI	@DRONE_DANI_PROCI,AR2
    // asm 0000AECD: 	LDI	DRONE_C|ANI_T,R2	;PID
    // asm 0000AECE: 	CALL	PRC_CREATE_CHILD
    // asm 0000AECF: 	STI	AR0,*+AR4(ORADZ)	;DOUBLING AS A PROC PTR
VANIX:
    // asm 0000AED0: 	POP	AR3
    // asm 0000AED1: 	POP	AR2
    // asm 0000AED2: 	POP	AR0
    // asm 0000AED3: 	POP	R6
    // asm 0000AED4: 	POP	R2
    // asm 0000AED5: 	POP	R1
    // asm 0000AED6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DDYNA_INIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
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

void DRONE_DANI_PROC(void)
{
    // asm 0000AED8: 	LDI	*+AR4(OCARBLK),AR5
    // asm 0000AED9: 	LDF	0,R6	 		;INIT SPIN RADIANS
    // asm 0000AEDA: 	STF	R6,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
    // asm 0000AEDB: 	LDF	*+AR5(CARSPEED),R0	;INIT SPEED
    // asm 0000AEDC: 	LDF	R0,R7
CARPROCL:
    // asm 0000AEDD: 	LDI	3,AR2	  		;SLEEP TIME
    // asm 0000AEDE: 	LDI	@SUSPEND_MODE,R0
    // asm 0000AEDF: 	CMPI	SM_HALT,R0
    // asm 0000AEE0: 	BEQ	CARSLP
    // asm 0000AEE1: 	LDI	*+AR4(ODIST),R0
    // asm 0000AEE2: 	CMPI	20000,R0		;FAR OFF JUST SLEEP
    // asm 0000AEE3: 	BGT	CARSLP
    // 	;
    // 	;GET WHEEL SPIN MATRIX
    // 	;
    // asm 0000AEE4: 	LDF	*+AR5(CARSPEED),R2
    // asm 0000AEE5: 	MPYF	0.02,R2   		;FUDGE FACTOR
    // asm 0000AEE6: 	ADDF	*+AR7(PDATA+2),R2
    // asm 0000AEE7: 	STF	R2,*+AR7(PDATA+2)	;SAVE WHEEL X RADIANS
    // asm 0000AEE8: 	LDI	@MATRIXBI,AR2		;GET X SPIN IN MATRIXB
    // asm 0000AEE9: 	CALL	FIND_XMATRIX
    // asm 0000AEEA: 	LDI	AR2,AR3			;SAVE REAR WHEEL MATRIX PTR
    // 	;STUFF YOUR DYNAMIC MATRICES
    // 	;
    // asm 0000AEEB: 	LDI	*+AR4(ODYNALIST),R0
    // asm: 	SLOCKON	Z,"DELTA\DRONE_DANI_PROC 1"
CDTOP:
    // asm 0000AEEC: 	LDI	R0,AR0
    // asm 0000AEED: 	LDI	*AR0,R0
    // asm 0000AEEE: 	BZ	CARSLP
    // asm 0000AEEF: 	LDI	AR0,AR2
    // asm 0000AEF0: 	ADDI	DYNAMATRIX,AR2
    // 	;STUFF WHEEL
    // 	;
    // asm 0000AEF1: 	LDF	*AR3++,R0
    // asm 0000AEF2: 	RPTS	7
    // asm 0000AEF3: 	LDF	*AR3++,R0
    // asm 0000AEF3:  ||	STF	R0,*AR2++
    // asm 0000AEF4:  	STF	R0,*AR2++
    // asm 0000AEF5: 	NOP	*AR3--(9)
    // asm 0000AEF6: 	LDI	*AR0,R0
    // asm 0000AEF7: 	BNZ	CDTOP
CARSLP:
    // asm 0000AEF8: 	SLEEP	3
    // asm 0000AEFA: 	B 	CARPROCL
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONE_DANI_PROC", 0, 0);
    UNIMPL();
}

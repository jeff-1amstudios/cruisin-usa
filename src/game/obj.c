#include "../core/cpu.h"
#include "../core/machine.h"
#include "vunit.h"
#include "mproc.h"
#include "obj.h"
#include "pall.h"
#include "sysid.h"
#include "sys.h"
#include "macs.h"
#include "error.h"
#include "globals.h"
#include "obj_defs.h"

/*
 * Source module: asm/OBJ.ASM
 */

/* asm: OACTIVE	.bss	OACTIVE,1 */
int OACTIVE;
/* asm: OFREE	.bss	OFREE,1 */
int OFREE;
/* asm: IDLE_LIST	.bss	IDLE_LIST,1 */
int IDLE_LIST;
/* asm: OACTIVE_PRIORITY	.bss	OACTIVE_PRIORITY,1 */
int OACTIVE_PRIORITY;
/* asm: OLOW_PRIORITY	.bss	OLOW_PRIORITY,1 */
int OLOW_PRIORITY;
/* asm: OHIGH_PRIORITY	.bss	OHIGH_PRIORITY,1 */
int OHIGH_PRIORITY;
/* asm: OACTIVECNT	.bss	OACTIVECNT,1 */
int OACTIVECNT;
/* asm: OFREECNT	.bss	OFREECNT,1 */
int OFREECNT;
/* asm: OMAX_OBJECTS	.bss	OMAX_OBJECTS,1 */
int OMAX_OBJECTS;
/* asm: OBJSTR	hibss	OBJSTR,OBJSIZ*NUM_OBJECTS */
int OBJSTR[OBJSIZ*NUM_OBJECTS];
/* asm: COMM_DRONE_PTR	.bss	COMM_DRONE_PTR,1 */
int COMM_DRONE_PTR;
/* asm: PLYRTEMP	.BSS	PLYRTEMP,1 */
int PLYRTEMP;
/* asm: ACTIVEHI1	.word	75000		;HI LIMIT FOR INACTIVE-ACTIVE */
int ACTIVEHI1 = 75000;
/* asm: ACTIVEHI	.word	80000		;HI LIMIT FOR ACTIVE-INACTIVE */
int ACTIVEHI = 80000;
#define ACTIVELO (-5000) //LO LIMIT INACTIVE OBJECT LIST

/* *----------------------------------------------------------------------------
*INITIALIZE OBJECT DATA STRUCTURES
*
 */
void OBJ_INIT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	LDI	NUM_OBJECTS,R0
    // asm: 	STI	R0,@OFREECNT
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@OACTIVE
    // asm: 	STI	R0,@IDLE_LIST
    // asm: 	STI	R0,@OACTIVE_PRIORITY
    // asm: 	STI	R0,@OACTIVECNT
    // asm: 	STI	R0,@OMAX_OBJECTS
    // asm: 	STI	R0,@OLOW_PRIORITY
    // asm: 	STI	R0,@OHIGH_PRIORITY
    // asm: 	STI	R0,@DRIVE_LIST
    // asm: 	STI	R0,@CAR_LIST
    // asm: 	STI	R0,@SIGN_LIST
    // asm: 	STI	R0,@GROUND_LIST
    // asm: 	STI	R0,@COMM_DRONE_PTR
    // asm: 	LDI	@OFREEI,AR0	 	;GET FREE POINTER
    // asm: 	LDI	@OBJSTRI,AR1
    // asm: 	LDI	NUM_OBJECTS-1,RC
    // asm: 	RPTB	OINITL
    // asm: 	STI	AR1,*AR0
    // asm: 	LDI	AR1,AR0
OINITL:
    // asm: ADDI	OBJSIZ,AR1
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*OBJ_GETE
*	GET EXTENDED OBJECT
*
*PARAMETERS
*	AR2	ROMDATA POINTER
*
*RETURNS
*	(SUCCESSFUL)
*	CARRY CLR
*	AR0	OBJECT POINTER
*	(UNSUCCESSFUL)
*	CARRY SET
*
 */
void OBJ_GETE(void)
{
    // asm: 	CALL	OBJ_GET
    // asm: 	RETSC				;NO OBJECTS AVAILABLE RETURN ERROR CODE
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	STI	AR2,*+AR0(OROMDATA)
    // asm: 	LDI	*AR2,R0			;get RADIUS of object
    // asm: 	STI	R0,*+AR0(ORAD)		;SAVE THE SILLY RADIUS
    // asm: 	LDI	*-AR2,R0		;GET CONTROL WORD
    // asm: 	TSTB	EOBJ_ILLUM,R0
    // asm: 	BZ	NOT_ILLUM
    // asm: 	LDI	*+AR0(OFLAGS),R1
    // asm: 	OR	O_ILLUM,R1
    // asm: 	STI	R1,*+AR0(OFLAGS)
NOT_ILLUM:
    // asm: 	TSTB	EOBJ_1PAL,R0
    // asm: 	BZ	NOT_1PAL
    // asm: 	LDI	*+AR0(OFLAGS),R1
    // asm: 	OR	O_1PAL,R1
    // asm: 	STI	R1,*+AR0(OFLAGS)
    // asm: 	LDI	R0,AR2
    // asm: 	AND	0FFfh,AR2
    // asm: 	CALL	PAL_FIND
    // asm: 	BC	$
    // asm: 	STI	R0,*+AR0(OPAL)
NOT_1PAL:
    // asm: 	CLRC
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_GETE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*OBJ_GET
*	ALLOCATE AN OBJECT BLOCK FROM THE OBJECT FREE LIST
*
*RETURNS
*	(SUCCESSFUL)
*		CARRY CLR
*		AR0	OBJECT POINTER
*	(UNSUCCESSFUL)
*		CARRY SET
*
*
 */
void OBJ_GET(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@OFREE,R0
    // asm: 	LDI	R0,AR0
    // asm: 	BZ	NOOBJ			;lockup if out of objects
    // asm: 	LDI	*AR0,R0
    // asm: 	STI	R0,@OFREE		;and update free list
    // asm: 	LDI	@OFREECNT,R0
    // asm: 	SUBI	1,R0
    // asm: 	STI	R0,@OFREECNT
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	STI	R0,*+AR0(OPLINK)
    // asm: 	STI	R0,*+AR0(OUSR1)
    // asm: 	STI	R0,*+AR0(OID)
    // asm: 	STI	R0,*+AR0(OLINK2)
    // asm: 	STI	R0,*+AR0(OLINK3)
    // asm: 	STI	R0,*+AR0(OLINK4)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	STF	R0,*+AR0(OVELX)
    // asm: 	STF	R0,*+AR0(OVELY)
    // asm: 	STF	R0,*+AR0(OVELZ)
    // asm: 	STF	R0,*+AR0(ORADX)
    // asm: 	STF	R0,*+AR0(ORADY)		;CLEAR RADIANS TO AVOID LOCKUP
    // asm: 	STF	R0,*+AR0(ORADZ)
    // asm: 	STF	R0,*+AR0(OMATRIX+1)
    // asm: 	STF	R0,*+AR0(OMATRIX+2)
    // asm: 	STF	R0,*+AR0(OMATRIX+3)
    // asm: 	STF	R0,*+AR0(OMATRIX+5)
    // asm: 	STF	R0,*+AR0(OMATRIX+6)
    // asm: 	STF	R0,*+AR0(OMATRIX+7)
    // asm: 	LDF	1.0,R0
    // asm: 	STF	R0,*+AR0(OMATRIX+0)
    // asm: 	STF	R0,*+AR0(OMATRIX+4)
    // asm: 	STF	R0,*+AR0(OMATRIX+8)
    // asm: 	CLRC
    // asm: 	POP	R0
    // asm: 	RETS
NOOBJ:
    // asm: 	ERRON	U,EC_OBJ|ET_ALLOC
    // asm: 	SETC
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_GET", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*COMPUTE DISTANCE OF OBJECT
*
*PARAMETERS
*	AR2	OBJECT
*
*RETURNS
*	R0	ODIST
*		STORES IN *+AR2(ODIST)
 */
void GETDIST(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R1
    // asm: 	PUSHF	R2
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	LDF	*+AR2(OPOSY),R1
    // asm: 	LDF	*+AR2(OPOSZ),R2
    // asm: 	LDP	@_CAMERAPOS
    // asm: 	SUBF	@_CAMERAPOS,R0	  	;ADJUST FOR UNIVERSE RELATIVE
    // asm: 	SUBF	@_CAMERAPOS+1,R1
    // asm: 	SUBF	@_CAMERAPOS+2,R2
    // 	;MULTIPLY BY ROTATION MATRIX TO FIND Z TERM
    // asm: 	MPYF	@_CAMERAMATRIX+6,R0	;LAST COLUMN ONLY NEEDED
    // asm: 	MPYF	@_CAMERAMATRIX+7,R1
    // asm: 	MPYF	@_CAMERAMATRIX+8,R2
    // asm: 	SETDP
    // asm: 	ADDF	R0,R1
    // asm: 	ADDF	R1,R2
    // asm: 	FIX	R2,R0
    // asm: 	STI	R0,*+AR2(ODIST)
    // asm: 	POPF	R2
    // asm: 	POPF	R1
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETDIST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*INSERT PRIORITY OBJECT ON LIST
*
*PARAMETERS
*	AR2	OBJECT
*
 */
void OBJ_INSERTP(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	CALL	GETDIST	 		;SETUP (ODIST) FIELD
    // asm: 	LDI	*+AR2(OFLAGS),R0	;OR IN PROPER FLAG
    // asm: 	OR	O_LIST3,R0
    // asm: 	STI	R0,*+AR2(OFLAGS)
    // asm: 	LDI	@OACTIVE_PRIORITYI,AR1	;INSERT TO HEAD OF PROCESS ACTIVE LIST
    // asm: 	LDI	*+AR2(ODIST),R0		;GET CURRENT Z COORD OF OBJECT
INSOBJLP:
    // asm: 	LDI	AR1,AR0			;AR0 IS PREVIOUS LINK
    // asm: 	LDI	*AR1,R1			;CHECK END OF LIST
    // asm: 	BZ	INS_AT_ENDP
    // asm: 	LDI	R1,AR1
    // asm: 	CMPI	*+AR1(ODIST),R0
    // asm: 	BLE	INSOBJLP     		;KEEP GOING DUDE
INS_AT_ENDP:
    // asm: 	STI	R1,*AR2			;LINK TO NEXT
    // asm: 	STI	AR2,*AR0		;LINK FROM PREVIOUS
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_INSERTP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*INSERT LOW PRIORITY OBJECT
*
*THIS IS NOT THE SAME AS OTHER LISTS, FOR THIS LIST THE OBJECT MUST
*BE DELINKED AND FREED BY THE USER
*LIST IS NOT SORTED
*
*PARAMETERS
*	AR2	OBJECT TO LINK IN
*
*
 */
void OBJ_INSERTLP(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@OLOW_PRIORITY,R0
    // asm: 	STI	R0,*+AR2(OLINK)
    // asm: 	STI	AR2,@OLOW_PRIORITY
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_INSERTLP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*THIS IS NOT THE SAME AS OTHER LISTS, FOR THIS LIST THE OBJECT MUST
*BE DELINKED AND FREED BY THE USER
*LIST IS NOT SORTED
*
*PARAMETERS
*	AR2	OBJECT TO LINK IN
*
 */
void OBJ_INSERTHP(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@OHIGH_PRIORITY,R0
    // asm: 	STI	R0,*+AR2(OLINK)
    // asm: 	STI	AR2,@OHIGH_PRIORITY
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_INSERTHP", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*INSERT OBJECT ON OBJECT LIST
*
*PARAMETERS
*	AR2	OBJECT
*
 */
void OBJ_INSERT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	CALL	GETDIST	 		;SETUP (ODIST) FIELD
    // asm: 	CMPI	ACTIVELO,R0		;TOO FAR BEHIND?
    // asm: 	BLT	INSIDLE			;YES
    // ;	ASH	-2,R0
    // asm: 	CMPI	@ACTIVEHI,R0		;TOO FAR AHEAD?
    // asm: 	BLT	INSACT			;NO...
    // asm: INSIDLE			    		;INSERT ON IDLE LIST
    // asm: 	LDI	*+AR2(OFLAGS),R0	;OR in proper flag
    // asm: 	OR	O_LIST2,R0
    // asm: 	STI	R0,*+AR2(OFLAGS)
    // asm: 	LDI	@IDLE_LISTI,AR1		;insert to head of process active list
    // asm: 	BD	INSOBJX
    // asm: 	LDI	*AR1,R0			;INSERT AT HEAD OF LIST
    // asm: 	STI	R0,*AR2
    // asm: 	STI	AR2,*AR1		;PUT IT IN FRONT OF IDLE LIST
    // 	;------>BD	INSOBJX
    // asm: INSACT					;INSERT ON ACTIVE LIST
    // asm: 	LDI	*+AR2(OFLAGS),R0	;OR in proper flag
    // asm: 	OR	O_LIST1,R0
    // asm: 	STI	R0,*+AR2(OFLAGS)
    // asm: 	LDI	@OACTIVEI,AR1		;insert to head of process active list
    // asm: 	LDI	*+AR2(ODIST),R0		;GET CURRENT Z COORD OF OBJECT
INSOBJL:
    // asm: 	LDI	AR1,AR0			;AR0 IS PREVIOUS LINK
    // asm: 	LDI	*AR1,R1			;CHECK END OF LIST
    // asm: 	BZ	INS_AT_END
    // asm: 	LDI	R1,AR1
    // asm: 	CMPI	*+AR1(ODIST),R0
    // asm: 	BLT	INSOBJL	     		;KEEP GOING DUDE
INS_AT_END:
    // asm: 	STI	R1,*AR2			;LINK TO NEXT
    // asm: 	STI	AR2,*AR0		;LINK FROM PREVIOUS
INSOBJX:
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R1
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_INSERT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
*PARAMETERS
*	AR2	ID
*RETURNS
*	CARRY CLR NOT FOUND
*	CARRY SET FOUND
*	AR0	FIRST OBJECT FOUND
*
 */
void OBJ_FIND_FIRST_PRIORITY(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@OACTIVE_PRIORITYI,AR0
    // asm: 	BU	L89
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FIND_FIRST_PRIORITY", 0, 0);
    UNIMPL();
}

void OBJ_FIND_FIRST(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@OACTIVEI,AR0
L89:
    // asm: LDI	*AR0,R0
    // asm: 	BZ	FF_ERR
FF_LP:
    // asm: 	LDI	R0,AR0
    // asm: 	CMPI	*+AR0(OID),AR2
    // asm: 	BEQ	FF_OK
    // asm: 	LDI	*AR0,R0
    // asm: 	BNZ	FF_LP
FF_ERR:
    // asm: 	CLRC
    // asm: 	POP	R0
    // asm: 	RETS
FF_OK:
    // asm: 	SETC
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FIND_FIRST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void OBJ_FREE_GROUND(void)
{
    // asm: 	PUSH	R1
    // asm: 	BUD	DELSLP
    // asm: 	PUSH	AR1
    // asm: 	LDI	@GROUND_LISTI,R1		;we must find dead object to link around
    // asm: 	SUBI	OLINK3,R1
    // 	;---->	BUD	DELSLP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FREE_GROUND", 0, 0);
    UNIMPL();
}

void OBJ_FREE_SIGN(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR1
    // asm: 	LDI	@SIGN_LISTI,R1		;we must find dead object to link around
    // asm: 	SUBI	OLINK3,R1
DELSLP:
    // asm: LDI	R1,AR1
    // asm: 	LDI	*+AR1(OLINK3),R1
    // asm: 	ERRON	Z,EC_OBJ|1		;lockup on end of list found
    // asm: 	BZ	NOT_ON_SUPPXLIST
    // asm: 	CMPI	R1,AR2
    // asm: 	BNE	DELSLP
    // asm: 	LDI	*+AR2(OLINK3),R1
    // asm: 	STI	R1,*+AR1(OLINK3)	;LINK AROUND
NOT_ON_SUPPXLIST:
    // asm: 	POP	AR1
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FREE_SIGN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void OBJ_FREE_DRIVE(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR1
    // asm: 	LDI	@DRIVE_LISTI,R1		;we must find dead object to link around
    // asm: 	SUBI	OLINK3,R1
DELRLP:
    // asm: LDI	R1,AR1
    // asm: 	LDI	*+AR1(OLINK3),R1
    // asm: 	ERRON	Z,EC_OBJ|2		;lockup on end of list found
    // asm: 	BZ	NOT_ON_SUPPROADLIST
    // asm: 	CMPI	R1,AR2
    // asm: 	BNE	DELRLP
    // asm: 	LDI	*+AR2(OLINK3),R1
    // asm: 	STI	R1,*+AR1(OLINK3)	;LINK AROUND
NOT_ON_SUPPROADLIST:
    // asm: 	POP	AR1
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FREE_DRIVE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void OBJ_FREE_PROC(void)
{
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR2(OPLINK),AR2
    // asm: 	CALL	PRC_KILL
    // asm: 	POP	AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FREE_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*DELETE AN OBJECT
*
*PARAMETERS
*	AR2	OBJECT
*
*NOTE
*	If there is any major change to this code
*	please check DELETE_GROUP in BACKGRND.ASM
*	this is a speeded up version of this subroutine.
*
 */
void OBJ_DELETE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
#if DEBUG
    // asm: 	LDI	0C5C5h,R0		;RANDOM KNOWN #
    // asm: 	STI	R0,*+AR2(OUSR1)
    // asm: 	LDI	0AB5Bh,R0		;RANDOM KNOWN #
    // asm: 	STI	R0,*+AR2(OLINK4)
#endif
    // 	;
    // 	;eliminate any dynamic objects associated with the object...
    // 	;
    // asm: 	LDI	*+AR2(OFLAGS),R0
    // asm: 	TSTB	O_DYNAMIC,R0
    // asm: 	BZ	NOTDYNAMIC
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR2(ODYNALIST),AR0
DYNDEL:
    // asm: LDI	*AR0,R0			;LINK TO NEXT
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	DELDYNA
    // asm: 	LDI	R0,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BNE	DYNDEL
    // asm: 	POP	AR2
    // ;	PUSH	AR2
    // ;	LDI	*+AR2(ORADZ),AR2
    // ;	CALL	KILL
    // ;	POP	AR2
NOTDYNAMIC:
    // ;	.if	DEBUG
    // ;	LDI	*+AR2(OFLAGS),R0
    // ;	AND	O_LIST_M,R0
    // ;	LOCKON	Z
    // ;	BZ	$			;this object has invalid list field
    // ;					;note this may change if we have more
    // ;					;then 3 lists
    // ;	.endif
    // 	;Mirror these checks in BACKGRND.ASM
    // 	;	'DELETE_GROUP'
    // 	;
    // asm: 	LDI	*+AR2(OFLAGS),R0	;KILL PROC ASSOCIATED WITH OBJ
    // asm: 	RS	(O_DEBRIS_B+1),R0
    // asm: 	CALLC	FREE_RDDEBRIS
    // asm: 	LDI	*+AR2(OFLAGS),R0	;KILL PROC ASSOCIATED WITH OBJ
    // asm: 	RS	(O_PROC_B+1),R0
    // asm: 	CALLC	OBJ_FREE_PROC
    // asm: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF DRIVE LIST
    // asm: 	RS	(O_DRIVE_SUPP_B+1),R0
    // asm: 	CALLC	OBJ_FREE_DRIVE
    // asm: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF GROUND LIST
    // asm: 	RS	(O_GROUND_B+1),R0
    // asm: 	CALLC	OBJ_FREE_GROUND
    // asm: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF SIGN LIST
    // asm: 	RS	(O_SIGN_SUPP_B+1),R0
    // asm: 	CALLC	OBJ_FREE_SIGN
    // 	;THE OBJECT CAN BE EITHER ON THE ACTIVE LIST, THE IDLE LIST,
    // 	;OR THE ACTIVE PRIORITY LIST.  ANYTHING ELSE IS AN ERROR.
    // 	;
    // asm: 	LDI	*+AR2(OFLAGS),R0
    // asm: 	AND	O_LIST_M,R0
    // asm: 	LDI	@OACTIVEI,R1
    // asm: 	CMPI	O_LIST2,R0
    // asm: 	LDIEQ	@IDLE_LISTI,R1
    // asm: 	CMPI	O_LIST3,R0
    // asm: 	LDIEQ	@OACTIVE_PRIORITYI,R1
DELLP:
    // asm: LDI	R1,AR1		;WE MUST FIND DEAD OBJECT TO LINK AROUND
    // asm: 	LDI	*AR1,R1
    // asm: 	ERRON	Z,EC_OBJ|ET_DELETE
    // asm: 	BZ	DELOBJX
    // asm: 	CMPI	R1,AR2
    // asm: 	BNE	DELLP
    // asm: 	LDI	*AR2,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	@OFREE,R1
    // asm: 	STI	R1,*AR2
    // asm: 	STI	AR2,@OFREE
    // asm: 	LDI	@OFREECNT,R0		;INCREMENT FREE OBJECT COUNT
    // asm: 	ADDI	1,R0
    // asm: 	STI	R0,@OFREECNT
    // asm: 	CLRI	R0
    // asm: 	STI	R0,*+AR2(OLINK2)	;CLEAR SEARCH ID
    // asm: 	STI	R0,*+AR2(OFLAGS)
DELOBJX:
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_DELETE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*OBJ_DELETE_CLASS
*
*SEARCHES THE ACTIVE LIST FOR ALL OCCURANCES OF SPECIFIED ID
*AND DELETES THAT OBJECT
*
*PARAMETERS
*	R0	ID
*	R1	MASK
*
 */
void OBJ_DELETE_CLASS(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR2
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	AND	R1,R0
    // asm: 	LDI	@OACTIVEI,AR0
ODC:
    // asm: LDI	*AR0,R3
    // asm: 	BZ	ODCX
    // asm: 	LDI	R3,AR0
ODCL2:
    // asm: LDI	*+AR0(OID),R2
    // asm: 	AND	R1,R2
    // asm: 	CMPI	R0,R2
    // asm: 	BNE	ODC
    // asm: 	LDI	*AR0,R3
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	R3,R3
    // asm: 	LDI	R3,AR0
    // asm: 	BNZ	ODCL2
ODCX:
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR2
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_DELETE_CLASS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PULL AN OBJECT FROM ACTIVE LIST
*
*Most of the same effect as delobj, except that the object is not
*inserted onto the free list, allowing the user to do hold onto
*the object.
*
*PARAMETERS
*	AR2	OBJECT
*
 */
void OBJ_PULL(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	LDI	*+AR2(OFLAGS),R0
    // asm: 	AND	O_LIST_M,R0
    // asm: 	LDI	@OACTIVEI,R1		;we must find dead object to link around
    // asm: 	CMPI	O_LIST2,R0
    // asm: 	LDIEQ	@IDLE_LISTI,R1		;we must find dead object to link around
    // asm: 	CMPI	O_LIST3,R0
    // asm: 	LDIEQ	@OACTIVE_PRIORITYI,R1	;we must find dead object to link around
PULLP:
    // asm: 	LDI	R1,AR1
    // asm: 	LDI	*AR1,R1
    // asm: 	ERRON	Z,EC_OBJ|4
    // asm: 	BZ	PULOBJ_X
    // asm: 	CMPI	R1,AR2
    // asm: 	BNE	PULLP
    // asm: 	LDI	*AR2,R1
    // asm: 	STI	R1,*AR1			;LINK AROUND
    // asm: 	LDI	*+AR2(OFLAGS),R0
    // asm: 	ANDN	O_LIST_M,R0
    // asm: 	STI	R0,*+AR2(OFLAGS)
PULOBJ_X:
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_PULL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*LINK OBJECT INTO FREE LIST
*(NO ERROR CHECKING)
*
*PARAMETERS
*	AR2	OBJECT
*
 */
void OBJ_FREE(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@OFREE,R0
    // asm: 	STI	R0,*AR2
    // asm: 	STI	AR2,@OFREE
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FREE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SORT OBJECTS BY CURRENT Z
*WAIT FOR NEXT INTERRUPT
*
*CLOBBERS
*	R0-R4,AR0-AR5,DP,etc.
*CALL BY MAINLINE CODE
*
 */
void ZSORTWT(void)
{
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CLEARRDY	  	;READY FOR INTERRUPT
ZSORTWL:
    // asm: 	CLRI	R6			;FLAG FOR SORT (NOTHING SORTED YET)
    // asm: 	LDI	@OACTIVEI,AR0		;INSERT TO HEAD OF PROCESS ACTIVE LIST
    // asm: 	LDI	*AR0,AR1		;GET FIRST ELEMENT
    // asm: 	LDI	AR1,R1	  		;BOGUS TEST AR REG'S DON'T SET FLAGS
    // asm: 	BZ	ZSWTX	     		;NULL LIST
    // asm: 	LDI	*AR1,AR2		;GET NEXT ELEMENT
    // asm: 	LDI	AR2,R1
    // asm: 	BZ	ZSWTX			;ONLY ONE ELEMENT ON LIST
ZSWTLP:
    // asm: 	LDI	@CLEARRDY,R0		;DONE WHEN IN SIGNALS FRAME IS DONE
    // asm: 	BZD	ZSWTXX
    // asm: 	LDI	*+AR1(ODIST),R0
    // asm: 	LDI	*+AR2(ODIST),R1
    // asm: 	CMPI	R1,R0
    // 	;------>BNZD	ZSWTXX
    // asm: 	BGE	ZWPRIOK 		;PRIORITY IS O.K.  (AR1 is further than AR2)
    // asm: DOSWAP
    // 	;SWAP EM DUDES
    // asm: 	LDI	1,R6			;the list has changed
    // asm: 	STI	AR2,*AR0		;POINT N-1 TO N+1
    // asm: 	LDI	*AR2,R1			;GET N+2
    // asm: 	STI	R1,*AR1			;POINT N TO N+2
    // asm: 	STI	AR1,*AR2		;POINT N+1 TO N
    // asm: 	BNZD	ZSWTLP
    // asm: 	LDI	AR2,AR0		  	;NEW PREVIOUS(N-1)
    // asm: 	LDI	R1,AR2			;NEW NEXT(N+1)
    // asm: 	LDI	R1,R1
    // 	;---->	BNZD	ZSWTLP
    // asm: 	LDI	R6,R6			;ANY SWAPS?
    // asm: 	BZ	ZSWTXX			;NO DONE...
    // asm: 	B	ZSORTWL	      		;START OVER AT THE BEGINNING
ZWPRIOK:
    // asm: 	LDI	*AR2,R1			;NEW NEXT LINK
    // asm: 	BNZD	ZSWTLP
    // asm: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    // asm: 	LDI	AR2,AR1			;AR0=PREVIOUS
    // asm: 	LDI	R1,AR2
    // 	;---->	BNZD	ZSWTLP
    // asm: 	BR	ZSORTWL			;START OVER AT THE BEGINNING
ZSWTX:
ZSWTXX:
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MHS,R0
    // asm: 	RETSNZ				;Don't do player and drones in HIGH SCORE MODE
    // asm: 	CALL	PLYRDLINK     		;DELINK PLAYER
    // asm: 	CALL	DRONESORT
    // asm: 	CALL	DEBRIS_SORT
    // asm: 	CALL	PLYRSORT		;LINK IN PLAYER
    // asm: 	CALL	SORT_SMOKE
    // asm: 	CALL	FLAMESORT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZSORTWT", 0, 0);
    UNIMPL();
}

/* *-----------------------------------------------------------------------------
*DELINK THE PLAYER --> SO NO INTERFERENCE WITH DRONESORT
*
*PLYRDLINK
*
 */
void PLYRDLINK(void)
{
    // asm: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
PSORTNXT:
    // asm: 	LDI	*AR1,R0
    // asm: 	BZD	PDLINKX			;NOBODY HOME
    // asm: 	LDI	AR1,AR0
    // asm: 	LDI	R0,AR1
    // asm: 	LDI	*+AR1(OID),R1
    // 	;------>BZD	PSORTX
    // asm: PSORTL
    // asm: 	CMPI	PLYR_C,R1
    // asm: 	BNE	PSORTNXT
    // *FOUND THE PLYR, DELINK 'EM
    // asm: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
    // asm: 	STI	R0,*AR0			;LINK AROUND THE DUDE
PDLINKX:
    // asm: 	STI	AR1,@PLYRTEMP		;SAVE THE DUDE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRDLINK", 0, 0);
    UNIMPL();
}

/* *-----------------------------------------------------------------------------
*SORT PLYR CAR WITH DRONE CARS INTO ROAD WITH PRIORITY
*
*PLAYER INSERTED BEFORE DRONE WITH CLOSER ODIST
*IF NO DRONE CLOSER, INSERT AT END OF LIST
*
 */
void PLYRSORT(void)
{
    // asm: 	LDI	@PLYRTEMP,R0		;GET PLAYER
    // asm: 	BZD	PSORTX	     		;NO PLAYER, HANG IT UP...
    // asm: 	LDI	R0,AR5
    // asm: 	LDI	*+AR5(ODIST),R2	     	;GET PLAYER DISTANCE
    // asm: 	LDF	*+AR5(OPOSX),R3	     	;GET PLAYER X COORD
    // 	;-----> BZD	PSORTX	       	;NO PLAYER, HANG IT UP...
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR5			;ZERO OUT PLAYERS LINK
    // asm: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
    // asm: 	BR	PSRT1NXT
PSRT1L:
    // asm: 	AND	CLASS_M|TYPE_M,R1	;CHECK FOR A DRONE
    // asm: 	CMPI	DRONE_C|VEHICLE_T,R1
    // asm: 	BNE	PSRT1NXT
    // *FOUND A DRONE, COMPARE ODIST
    // asm: 	CMPI	*+AR1(ODIST),R2	     	;GET DRONE DISTANCE
    // asm: 	BLT	PSRT1NXT		;DRONE IN BACK, IGNORE HIM
    // asm: 	LDF	*+AR1(OPOSX),R1
    // asm: 	SUBF	R3,R1
    // asm: 	ABSF	R1
    // asm: 	FIX	R1
    // asm: 	CMPI	2000,R1			;MUST BE WITHIN X LIMIT (NOT OFFSCREEN)
    // asm: 	BGT	PSRT1NXT		;DRONE IN FRONT, WERE DONE
    // asm: 	LDI	AR5,AR2
PSRT2A:
    // asm: 	LDI	*AR2,R1			;DRONE IN FRONT, LINK EM INTO CHAIN
    // asm: 	BZ	PSRT2
    // asm: 	LDI	R1,AR2
    // asm: 	BR	PSRT2A
PSRT2:
    // asm: 	LDI	*AR1,R0			;REMOVE DRONE FORM OBJECT LIST
    // asm: 	STI	R0,*AR0
    // asm: 	STI	AR1,*AR2		;LINK DRONE TO TEMP PLAYER LIST
    // asm: 	STI	R1,*AR1			;ZERO OUT LAST LINK
    // asm: 	LDI	AR0,AR1
PSRT1NXT:
    // asm: 	LDI	*AR1,R0
    // asm: 	BNZD	PSRT1L
    // asm: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    // asm: 	LDI	R0,AR1
    // asm: 	LDI	*+AR1(OID),R1
    // 	;------>BNZD	PSRT1L
    // *INSERT HIM
    // asm: 	STI	AR5,*AR0		;LINK IN PLAYER CHAIN
PSORTX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRSORT", 0, 0);
    UNIMPL();
}

/* *-----------------------------------------------------------------------------
*SORT DRONE CARS INTO ROAD WITH PRIORITY
*
*FIND CLOSEST ROAD PIECE DRONE IS ON TOP
*THEN ZSORT DRONE CLOSER BASED ON ODIST
*
*USES
*	AR4	POINTER TO DRONES
*
 */
void DRONESORT(void)
{
    // 	;PULL LIST OF DRONES OFF OBJECT LIST
    // asm: 	BUD	DSORTNXT
    // asm: 	NOP
    // ;	PUSH	R2
    // asm: 	LDI	0,AR4			;INIT DRONE LIST HEADER
    // asm: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
    // 	;------>BD DSORTNXT     	;GO GET FIRST ELEMENT
DSORTL:
    // asm: 	AND	CLASS_M,R1
    // asm: 	CMPI	DRONE_C,R1
    // asm: 	BNE	DSORTNXT
    // *FOUND A DRONE, DELINK 'EM
    // asm: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
    // asm: 	STI	R0,*AR0
    // asm: 	STI	AR4,*AR1		;LINK HIM INTO TEMP LIST
    // asm: 	LDI	AR1,AR4
    // asm: 	LDI	AR0,AR1
DSORTNXT:
    // asm: 	LDI	*AR1,R0
    // asm: 	BNZD	DSORTL
    // asm: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    // asm: 	LDI	R0,AR1
    // asm: 	LDI	*+AR1(OID),R1
    // 	;------>BNZD	DSORTL
    // ;	LDI	R0,R0
    // ;	BNZ	DSORTL
    // asm: DSORTX
    // *PROCESS DRONE LIST
    // *FOR EACH DRONE PUT IT AFTER HIGHEST PRIORITY ROAD SEG INTERSECTED
    // asm: 	LDI	AR4,R0	    		;NULL LIST?
    // asm: 	BZ	DSORTXX			;YES, QUIT
NXTDRONE:
    // *GET ROAD COLLISION POINTERS
    // asm: 	LDI	AR4,AR5
    // asm: 	LDI	*AR4,AR4		;MOVE TO NEXT DRONE
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	LDI	*+AR5(OCARBLK),AR0
    // asm: 	LDI	*+AR0(CARPCOL),AR1		;ROAD COLL CNT
    // asm: 	LDI	*+AR0(CARVSIZ+CARPCOL),AR2	;ROAD COLL RF
    // asm: 	LDI	*+AR0((2*CARVSIZ)+CARPCOL),AR3	;ROAD COLL LF
    // asm: 	LDI	*+AR0((3*CARVSIZ)+CARPCOL),AR4	;ROAD COLL LR
    // asm: 	LDI	*+AR0((4*CARVSIZ)+CARPCOL),AR5	;ROAD COLL RR
    // asm: 	LDI	*+AR1(ODIST),R1
    // asm: 	LDI	*+AR2(ODIST),R2
    // asm: 	LDI	*+AR3(ODIST),R3
    // asm: 	LDI	*+AR4(ODIST),R4
    // asm: 	LDI	*+AR5(ODIST),R5
    // asm: 	LDI	7FFFH,R6
    // asm: 	LSH	16,R6			;GET +INFINITY
    // asm: 	LDI	AR1,R0			;LOAD INFINITY INTO NULL POINTERS
    // asm: 	LDIZ	R6,R1
    // asm: 	LDI	AR2,R0
    // asm: 	LDIZ	R6,R2
    // asm: 	LDI	AR3,R0
    // asm: 	LDIZ	R6,R3
    // asm: 	LDI	AR4,R0
    // asm: 	LDIZ	R6,R4
    // asm: 	LDI	AR5,R0
    // asm: 	LDIZ	R6,R5
    // asm: 	CMPI	R1,R2
    // asm: 	LDIGT	AR1,AR2
    // asm: 	LDIGT	R1,R2
    // asm: 	CMPI	R2,R3
    // asm: 	LDIGT	AR2,AR3
    // asm: 	LDIGT	R2,R3
    // asm: 	CMPI	R3,R4
    // asm: 	LDIGT	AR3,AR4
    // asm: 	LDIGT	R3,R4
    // asm: 	CMPI	R4,R5
    // asm: 	LDIGT	AR4,AR5
    // asm: 	LDI	AR5,AR3
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
    // asm: 	LDI	AR3,R3		 	;CHECK NULL
    // asm: 	BZ	DSORTL1X	 	;NULL DUDE, Z SORT ONLY
DSORTL1:
    // asm: 	LDI	*AR1,R0			;CHECK END OF LIST
    // asm: 	BNZ	DSORTL2
DSORTL1X:
    // asm: 	LDI	@OACTIVEI,AR1		;CANT FIND TRACK PIECE, Z SORT FROM TOP
    // asm: 	LDI	AR1,R1
    // asm: 	BNZ	DZSORTUP		;LIST NOT NULL
    // asm: 	LDI	*AR2,R2
    // asm: 	STI	R2,*AR3			;OBJECT LIST IS NULL, PUT CAR AT HEAD
    // asm: 	STI	AR3,*AR2
DSORTL2:
    // asm: 	LDI	R0,AR1
    // asm: 	CMPI	R0,R3
    // asm: 	BNZ	DSORTL1
    // *
    // *SORT PAST SHOULDER PIECES
    // *AR1=OBJECT TO INSERT AFTER IN OBJECT LIST
    // *AR5=DRONE
    // *
DZSORTUP:
    // asm: 	LDI	*+AR0(CARPCOL),AR3     	;ON THE ROAD?
    // asm: 	LDI	*+AR3(OID),R0
    // asm: 	CMPI	0300H,R0
    // asm: 	BNZ	DZSORTUP1		;NOPE, FORGET SHOULDER JIVE
    // asm: 	LDI	*+AR5(ODIST),R1	     	;GET DRONE DISTANCE
    // asm: 	LDI	ROAD_C+SHLDR_T,R2	;GET SOULDER ID
    // asm: 	LDI	CLASS_M,R3
    // asm: 	BU	DSL11
DSLP1:
    // asm: 	BGT	DSL000	 		;ROAD OR CAR, CHECK IT OUT..
    // asm: 	AND	R3,R4,R5
    // asm: 	CMPI	TSIGN_C,R5		;SIGN OR TREE, NEED TO CHECK PRIORITY ?
    // asm: 	BNZ	DSL00			;NOPE, BLOW IT OFF...
DSL000:
    // asm: 	CMPI	*+AR2(ODIST),R1
    // asm: 	BGE	DSDONE	 		;PRIORITY IS O.K., WERE DONE WITH DRONE
DSL00:
    // asm: 	LDI	AR2,AR1
DSL11:
    // asm: 	LDI	*AR1,R0
    // asm: 	BNZD	DSLP1
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	*+AR2(OID),R4
    // asm: 	CMPI	R4,R2
    // 	;----->	BNZD	DSLP1
    // asm: 	B	DSDONE
    // *Z SORT IT UPWARDS
    // *AR1=OBJECT TO INSERT AFTER IN OBJECT LIST
    // *AR5=DRONE
DZSORTUP1:
    // asm: 	LDI	*+AR5(ODIST),R1	     	;GET DRONE DISTANCE
    // asm: 	BU	DSL1
DSLP:
    // asm: 	BGE	DSDONE	 		;PRIORITY IS O.K., WERE DONE WITH DRONE
    // asm: 	LDI	AR2,AR1
DSL1:
    // asm: 	LDI	*AR1,R0
    // asm: 	BNZD	DSLP
    // asm: 	LDI	R0,AR2
    // asm: 	CMPI	*+AR2(ODIST),R1
    // asm: 	NOP
    // 	;----->	BNZD	DSLP
DSDONE:
    // asm:         LDI	*AR1,R0			;WERE DONE... INSERT THE SUCKA
    // asm: 	STI	AR5,*AR1
    // asm: 	STI	R0,*AR5
    // *GET NEXT DRONE DUDES
    // asm: DSORTLLL
    // asm:        	LDI	AR4,R0
    // asm: 	BNZ	NXTDRONE
DSORTXX:
    // asm: 	RETS				;WE QUIT...
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONESORT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SCAN ACTIVE LIST
*PUT DISTANT OBJECTS ON INACTIVE LIST
*
 */
void OSCAN(void)
{
    // asm: 	BUD	OSCANNXT
    // asm: 	LDI	@ACTIVEHI,R4 		;GET FAR LIMIT
    // asm: 	LDI	@IDLE_LISTI,AR5		;IN SAME PAGE
    // asm: 	LDI	@OACTIVEI,AR1
    // 	;------>BD OSCANNXT     	;GO GET FIRST ELEMENT
OSCANL:
    // asm: 	LDI	*+AR1(ODIST),R0	    	;ODIST TOO NEGATIVE?
    // asm: 	CMPI	ACTIVELO,R0
    // asm: 	BLE	OSCANACT		;YES, BLOW IT OUT
    // asm: 	SUBI	R4,R0			;ODIST TOO POSITIVE
    // asm: 	BLE	OSCANNXT		;NO...
    // asm: 	SUBI	*+AR1(ORAD),R0		;CHECK RADIUS TO MAKE SURE
    // asm: 	BLE	OSCANNXT
    // asm: 	NOP
    // *FOUND DISTANT ELEMENT, XSFER ACTIVE TO INACTIVE
OSCANACT:
    // asm: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
    // asm: 	STI	R0,*AR6
    // asm: 	LDI	*+AR1(OFLAGS),R0	;SWITCH LIST FLAG
    // asm: 	XOR	O_LIST2+O_LIST1,R0
    // asm: 	STI	R0,*+AR1(OFLAGS)
    // asm: 	LDI	*AR5,R0
    // asm: 	STI	R0,*AR1			;LINK HIM INTO INACTIVE LIST
    // asm: 	STI	AR1,*AR5
    // asm: 	LDI	AR6,AR1
OSCANNXT:
    // asm: 	LDI	*AR1,R0
    // asm: 	BNZD	OSCANL
    // asm: 	LDI	AR1,AR6			;AR6=PREVIOUS-1 LINK
    // asm: 	LDI	R0,AR1
    // asm: 	NOP
    // 	;------>BNZD	OSCANL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OSCAN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SCAN INACTIVE LIST FOR ACTIVES
*PUT DISTANT OBJECTS ON INACTIVE LIST
*IF RADIUS IS CLOSE, ACTIVATE
*IF ANGLE COSINE POSITIVE (+-90 DEGREES), ACTIVATE
*
 */
void ISCAN(void)
{
    // asm: 	FLOAT	ACTIVELO,R3		;GET CLOSE LIMIT
    // asm: 	LDI	@ACTIVEHI1,R4 		;GET FAR LIMIT
    // asm: 	LDI	@CAMERAPOSI,AR4		;POINTER TO CAMERA STRUCT
    // asm: 	LDI	@IDLE_LISTI,AR1		;IN SAME PAGE
    // asm: 	LDI	@OACTIVEI,AR5
    // asm: 	LDI	@CAMERAMATRIXI,AR3
    // asm: 	ADDI	1,AR4 			;POINT TO Y ELEMENT
    // asm: 	BUD	ISCANNXT
    // asm: 	ADDI	7,AR3			;LAST COLUMN OF MATRIX
    // asm: 	LDI	OPOSY,IR0
    // asm: 	LDI	OPOSZ,IR1
    // 	;------>BD ISCANNXT     	;GO GET FIRST ELEMENT
ISCANL:
    // asm: 	SUBF	*AR4,*+AR1(IR0),R6	;OYPOS-CAMERAPOSY
    // asm: 	SUBF	*+AR4(1),*+AR1(IR1),R7	;OZPOS-CAMERAPOSZ
    // asm: 	MPYF    *-AR3(1),R5,R0
    // asm: 	MPYF    *AR3,R6,R1
    // asm: 	MPYF    *+AR3(1),R7,R2
    // asm: 	ADDF	R1,R0
    // asm: 	ADDF	R2,R0
    // asm: 	CMPF	R3,R0
    // asm: 	BLED	ISCANNXT
    // asm: 	FIX	R0,R1
    // asm: 	STI	R1,*+AR1(ODIST)		;SETUP ODIST
    // asm: 	NOP
    // 	;------>BLED	ISCANNXT
    // asm: 	SUBI	R4,R1
    // asm: 	SUBI	*+AR1(ORAD),R1		;CHECK RADIUS TO MAKE SURE
    // asm: 	BGT	ISCANNXT
    // *FOUND CLOSE ELEMENT, XSFER INACTIVE TO ACTIVE
    // asm: ISCANACT
    // asm: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
    // asm: 	STI	R0,*AR6
    // asm: 	LDI	*+AR1(OFLAGS),R0	   	;SWITCH LIST FLAG
    // asm: 	XOR	O_LIST2+O_LIST1,R0
    // asm: 	STI	R0,*+AR1(OFLAGS)
    // asm: 	LDI	*AR5,R0
    // asm: 	STI	R0,*AR1			;LINK HIM INTO INACTIVE LIST
    // asm: 	STI	AR1,*AR5
    // asm: 	LDI	AR6,AR1
ISCANNXT:
    // asm: 	LDI	*AR1,R0
    // asm: 	BNZD	ISCANL
    // asm: 	LDI	AR1,AR6			;AR6=PREVIOUS-1 LINK
    // asm: 	LDI	R0,AR1
    // asm: 	SUBF	*-AR4(1),*+AR1(OPOSX),R5   	;GET LENGTH OF OBJ VECTOR
    // 	;------>BNZD	ISCANL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ISCAN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*RESCAN	TOTALLY RESCAN ALL OBJECTS ACTIVE AND INACTIVE
*	FOR CAMERA VIEW CHANGE
*
*PARAMETERS
*	CAMERAPOS AND CAMERAMATRIX WITH NEW VALUES
*
 */
void RESCAN(void)
{
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // *COMBINE ACTIVE INACTIVE LISTS
    // asm: 	LDI	@IDLE_LISTI,AR5
    // asm: 	LDI	@OACTIVEI,AR1
    // asm: 	LDI	*AR1,R0
    // asm: 	BZ	RESCAN1			;ACTIVE LIST NULL, FORGET IT
RESCAN0:
    // asm: 	LDI	R0,AR2
    // asm: 	LDI	*+AR2(OFLAGS),R0       	;SWITCH LIST FLAG
    // asm: 	XOR	O_LIST2+O_LIST1,R0
    // asm: 	STI	R0,*+AR2(OFLAGS)
    // asm: 	LDI	*AR2,R0
    // asm: 	BNZ	RESCAN0
    // asm: 	LDI	*AR5,R0			;FIRST ELEMENT INACTIVE LIST
    // asm: 	STI	R0,*AR2			;LINK TO LAST ELEMENT OF ACTIVE LIST
    // asm: 	LDI	*AR1,R0
    // asm: 	STI	R0,*AR5			;POINT INACTIVE LIST TO ACTIVE LIST
    // asm: 	LDI	0,R0   			;CLEAR OUT ACTIVE LIST
    // asm: 	STI	R0,*AR1
RESCAN1:
    // asm: 	CALL	ISCAN			;FORM NEW ACTIVE INACTIVE LISTS
    // asm: 	CALL	ZSORTACT		;BUBBLE SORT ACTIVE LIST
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESCAN", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*BUBBLE SORT PRIORITY LIST UNTIL DONE
*
 */
void ZSORTPRIOR(void)
{
ZSORTA1P:
    // asm: 	LDI	0,R2			;CLEAR EXCHANGE FLAG
    // asm: 	LDI	@OACTIVE_PRIORITYI,AR0
    // asm: 	LDI	*AR0,AR1		;GET FIRST ELEMENT
    // asm: 	LDI	AR1,R1
    // asm: 	BZ	ZSORTXP		     	;NULL LIST
    // asm: 	LDI	*AR1,AR2		;GET NEXT ELEMENT
    // asm: 	LDI	AR2,R1
    // asm: 	BZ	ZSORTXP			;ONLY ONE ELEMENT ON LIST
    // asm: 	LDI	*+AR1(ODIST),R0
ZSLPP:
    // asm: 	LDI	*+AR2(ODIST),R1
    // asm: 	CMPI	R1,R0
    // asm: 	BGE	PRIOKP	 		;PRIORITY IS O.K.
    // *SWAP EM DUDES
    // asm: 	LDI	1,R2
    // asm: 	STI	AR2,*AR0		;POINT N-1 TO N+1
    // asm: 	LDI	*AR2,R1			;GET N+2
    // asm: 	STI	R1,*AR1			;POINT N TO N+2
    // asm: 	STI	AR1,*AR2		;POINT N+1 TO N
    // asm: 	LDI	AR2,AR0		  	;NEW PREVIOUS(N-1)
    // asm: 	LDI	R1,AR2			;NEW NEXT(N+1)
    // asm: 	LDI	R1,R1
    // asm: 	BNZ	ZSLPP
    // asm: 	BR	ZSORTXP
PRIOKP:
    // asm: 	LDI	R1,R0
    // asm: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    // asm: 	LDI	AR2,AR1			;AR0=PREVIOUS
    // asm: 	LDI	*AR2,R1
    // asm: 	LDI	R1,AR2
    // asm: 	BNZ	ZSLPP
ZSORTXP:
    // asm: LDI	R2,R2
    // asm: 	BNZ	ZSORTA1P
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZSORTPRIOR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*BUBBLE SORT ACTIVE LIST UNTIL DONE
*
 */
void ZSORTACT(void)
{
ZSORTA1:
    // asm: 	LDI	0,R2			;CLEAR EXCHANGE FLAG
    // asm: 	LDI	@OACTIVEI,AR0
    // asm: 	LDI	*AR0,AR1		;GET FIRST ELEMENT
    // asm: 	LDI	AR1,R1
    // asm: 	BZ	ZSORTX		     	;NULL LIST
    // asm: 	LDI	*AR1,AR2		;GET NEXT ELEMENT
    // asm: 	LDI	AR2,R1
    // asm: 	BZ	ZSORTX			;ONLY ONE ELEMENT ON LIST
    // asm: 	LDI	*+AR1(ODIST),R0
ZSLP:
    // asm: 	LDI	*+AR2(ODIST),R1
    // asm: 	CMPI	R1,R0
    // asm: 	BGE	PRIOK	 		;PRIORITY IS O.K.
    // *SWAP EM DUDES
    // asm: 	LDI	1,R2
    // asm: 	STI	AR2,*AR0		;POINT N-1 TO N+1
    // asm: 	LDI	*AR2,R1			;GET N+2
    // asm: 	STI	R1,*AR1			;POINT N TO N+2
    // asm: 	STI	AR1,*AR2		;POINT N+1 TO N
    // asm: 	LDI	AR2,AR0		  	;NEW PREVIOUS(N-1)
    // asm: 	LDI	R1,AR2			;NEW NEXT(N+1)
    // asm: 	LDI	R1,R1
    // asm: 	BNZ	ZSLP
    // asm: 	BR	ZSORTX
PRIOK:
    // asm: 	LDI	R1,R0
    // asm: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    // asm: 	LDI	AR2,AR1			;AR0=PREVIOUS
    // asm: 	LDI	*AR2,R1
    // asm: 	LDI	R1,AR2
    // asm: 	BNZ	ZSLP
ZSORTX:
    // asm: 	LDI	R2,R2
    // asm: 	BNZ	ZSORTA1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZSORTACT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*OBJECTP	make_obj(int *rom,int px,int py,int pz,float rx,float ry)
*
*PARAMETERS
*	AR2	ROMDATA					  ^
*	RC	POS X					  |
*	RS	POS Y					ignored
*	RE	POS Z
*	R2	RAD Y
*RETURNS
*	R0	OBJECT POINTER
*
*
 */
void OBJ_MAKE(void)
{
    // asm: 	CALL	OBJ_GET
    // asm: 	ERRON	C,EC_OBJ|5
    // asm: 	RETSC
    // asm: 	STI	AR2,*+AR0(OROMDATA)
    // asm: 	STF	R3,*+AR0(ORADY)
    // asm: 	FLOAT	RC,R2
    // asm: 	STF	R2,*+AR0(OPOSX)
    // asm: 	FLOAT	RS,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	FLOAT	RE,R2
    // asm: 	STF	R2,*+AR0(OPOSZ)
    // asm: 	LDF	*+AR0(ORADY),R2
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR0,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_MAKE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*QWICK MAKE OBJ
*
*PARAMETERS
*	AR2	ROM POINTER
*	R2	POSX
*	R3	POSY
*	RC	POSZ
*
*RETURNS
*	AR0	OBJECT
*
 */
void OBJ_QMAKE(void)
{
    // asm: 	CALL	OBJ_GET
    // asm: 	RETSC
    // asm: 	STI	AR2,*+AR0(OROMDATA)
    // asm: 	FLOAT	R2
    // asm: 	STF	R2,*+AR0(OPOSX)
    // asm: 	FLOAT	R3
    // asm: 	STF	R3,*+AR0(OPOSY)
    // asm: 	FLOAT	RC,R2
    // asm: 	STF	R2,*+AR0(OPOSZ)
    // asm: 	CLRC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_QMAKE", 0, 0);
    UNIMPL();
}

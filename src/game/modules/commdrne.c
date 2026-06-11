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
// 	;PLYR2 (on PLYR1)
/* asm: PLY2CAR	.bss	PLY2CAR,1 */
int PLY2CAR;
// *----------------------------------------------------------------------------
// *Communications drone
// *
// *
// *
#endif
// *STORE OBJECT POINTER
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
// *
// *INIT POSITION N ROAD PIECES FROM BEGINNING
// *
// 	;initialize Ytheta to the intentional direction
// *INIT THROTTLE AND BRAKE
// 	;Z == MASTER
// ;	LDF	*AR2++,R0
// ;	STF	R0,*+AR4(OPOSX)
// ;	LDF	*AR2++,R0
// ;	STF	R0,*+AR4(OPOSY)
// ;	LDF	*AR2++,R0
// ;	STF	R0,*+AR4(OPOSZ)
// ;	LDI	AR4,AR2
// ;	CALL	OBJ_PULL
// ;	CALL	OBJ_INSERT
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------

void COMM_DRONE_PTR_SORT(void)
{
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

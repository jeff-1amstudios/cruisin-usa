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

/*
 * Source module: asm/FLAME.ASM
 */

// *----------------------------------------------------------------------------
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
/* asm: FLAMEANII	.word	FLAMEANI */
int FLAMEANII = (int)(FLAMEANI);
/* asm: FLAME_POSI	.word	FLAME_POS */
int FLAME_POSI = (int)(FLAME_POS);
// *----------------------------------------------------------------------------
// * FLAME	PROC
// *
#define FRAME_ON (PDATA+1)
#define CARBODY_MATRIX (PDATA+2)
// *----------------------------------------------------------------------------

void FLAME_PRC(void)
{
    // asm: 	LDI	PLYR_C|PLYR_FLAMES_S,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	BC	FLAME_DIE		;Only one set of flames at a time
    // asm: 	LDI	PLYR_C,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	BNC	FLAME_DIE		;IF there is no player object then bail.
    // asm: 	LDI	AR0,AR6
    // asm: 	LDI	*+AR0(OCARBLK),AR1
    // asm: 	LDI	*+AR1(CAR_SPIN),R0
    // asm: 	BNZ	FLAME_DIE			;YES, the no flames
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	GETCARBODY
    // asm: 	STI	AR0,*+AR7(CARBODY_MATRIX)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(FRAME_ON)	;Reset animation counter
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	FLAME_DIE
    // asm: 	LDI	AR0,AR4			;Left flame
    // ;	LDI	@CHOSEN_VEHICLE,R0
    // asm: 	LDI	@CHOOSENCAR,R0
    // asm: 	MPYI	3,R0
    // asm: 	ADDI	@FLAME_POSI,R0
    // asm: 	LDI	R0,AR0
    // asm: 	FLOAT	*+AR0(X),R1
    // asm: 	FLOAT	*+AR0(Y),R2
    // asm: 	FLOAT	*+AR0(Z),R3
    // asm: 	STF	R1,*+AR4(OVELX)
    // asm: 	STF	R2,*+AR4(OVELY)
    // asm: 	STF	R3,*+AR4(OVELZ)
    // asm: 	LDI	PLYR_C|PLYR_FLAMES_S,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	FLAME_DIE
    // asm: 	LDI	AR0,AR5			;Right frame
    // asm: 	LDF	PI,R0
    // asm: 	STF	R0,*+AR5(ORADY)
    // asm: 	FLOAT	180,R0
    // asm: 	NEGF	R1			;opposite side
    // asm: 	STF	R1,*+AR5(OVELX)
    // asm: 	STF	R2,*+AR5(OVELY)
    // asm: 	STF	R3,*+AR5(OVELZ)
    // asm: 	LDI	PLYR_C|PLYR_FLAMES_S,R0
    // asm: 	STI	R0,*+AR5(OID)
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDI	9-1,R5
FLAME_ANI_LOOP:
    // asm: 	LDI	*+AR6(OCARBLK),AR1
    // asm: 	LDI	*+AR1(CAR_SPIN),R0
    // asm: 	BNZ	FLAME_ANIX			;YES, the kill the flames
    // asm: 	LDI	*+AR7(CARBODY_MATRIX),AR2
    // asm: 	LDI	AR6,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	@MATRIXAI,R3
    // asm: 	CALL	CONCATMATV
    // asm: 	LDI	@MATRIXAI,AR3
    // asm: 	LDI	AR4,AR0
    // asm: 	LDI	*+AR7(FRAME_ON),IR0
    // asm: 	LDI	@FLAMEANII,AR1
    // asm: 	CALL	animate_child
    // asm: 	LDI	@MATRIXAI,AR3
    // asm: 	LDI	*+AR7(FRAME_ON),IR0
    // asm: 	LDI	@FLAMEANII,AR1
    // asm: 	LDI	AR5,AR0
    // asm: 	CALL	animate_child
    // asm: 	STI	R0,*+AR7(FRAME_ON)
    // asm: 	SLEEP	1
    // asm: 	SUBI	1,R5
    // asm: 	BP	FLAME_ANI_LOOP
FLAME_ANIX:
    // asm: 	LDI	PLYR_C|PLYR_FLAMES_S,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	BNC	FLANX1
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	BR	FLAME_ANIX
FLANX1:
FLAME_DIE:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLAME_PRC", 0, 0);
    UNIMPL();
}

void animate_child(void)
{
    // *----------------------------------------------------------------------------
    // *Input
    // *	AR0 = object to position and ani
    // *	    OVELX/Y/Z = offset from parent
    // *	AR6 = object to position reletive to
    // *	AR1 = Animation list
    // *	IR0  = Frame on
    // *OUTPUT
    // *	IR0  = Next frame
    // *TRASHES	AR2,R1,R2,R3
    // ;Set the Frame
set_frame:
    // asm: 	LDI	*+AR1(IR0),R1
    // asm: 	LDIN	0,IR0
    // asm: 	BN	set_frame
    // asm: 	ADDI	1,IR0
    // asm: 	STI	R1,*+AR0(OROMDATA)
    // asm: 	PUSH	IR0
    // ;Set the position reletive to the parent object.
    // asm: 	LDI	AR3,R2
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	CPYMAT
    // asm: 	LDI	AR0,AR2		;rotate the position reletive to parent
    // asm: 	ADDI	OVELX,AR2
    // asm: 	LDI	AR0,R2			;Source 3X3
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	LDI	AR0,R3			;Dest 1X3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR6(OPOSX),R2		;Set the position
    // asm: 	ADDF	*+AR0(OPOSX),R2
    // asm: 	STF	R2,*+AR0(OPOSX)
    // asm: 	LDF	*+AR6(OPOSY),R2
    // asm: 	ADDF	*+AR0(OPOSY),R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	LDF	*+AR6(OPOSZ),R2
    // asm: 	ADDF	*+AR0(OPOSZ),R2
    // asm: 	STF	R2,*+AR0(OPOSZ)
    // asm: 	LDI	@MATRIXBI,AR2
    // asm: 	LDI	AR0,R2
    // asm: 	ADDI	ORADX,R2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	LDI	AR3,AR2
    // asm: 	LDI	@MATRIXBI,R2
    // asm: 	LDI	AR0,R3
    // asm: 	ADDI	OMATRIX,R3
    // asm: 	CALL	CONCATMATV
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "animate_child", 0, 0);
    UNIMPL();
}

void FLAMESORT(void)
{
    // *----------------------------------------------------------------------------
    // *CALLED BY ZSORTWT in obj.asm just after the players car is linked in.
    // *PUTS THE FLAMES JUST BEHIND THE PLAYERS CAR
    // asm: 	LDI	@OACTIVE,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BEQ	FLMSORTX		;NO LIST... NO GO
    // asm: FLMSORT_LOOP
    // asm: 	LDI	PLYR_C|PLYR_FLAMES_S,R1
    // asm: 	CALL	FIND_NEXT_OBJ
    // asm: 	BC	FLMSORTX
    // asm: 	LDI	AR0,AR4
    // asm: 	CALL	FIND_NEXT_OBJ
    // asm: 	BC	FLMSORTX
    // asm: 	LDI	AR0,AR5
    // asm: 	LDI	*+AR4(OFLAGS),R0	;SAVE the flages
    // asm: 	LDI	*+AR5(OFLAGS),R1	;SAVE the flages
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	LDI	AR5,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	STI	R0,*+AR4(OFLAGS)	;RESTORE the flags
    // asm: 	STI	R1,*+AR5(OFLAGS)	;RESTORE the flags
    // asm: 	LDI	PLYR_C,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	BNC	FLMSORTX		;NO PLAYER?
    // asm: 	STI	AR5,*AR4		;Link flame2 to flame1
    // asm: 	LDI	*AR0,R0			;Get link to next object
    // asm: 	STI	AR4,*AR0		;link the flames to the players object
    // asm: 	STI	R0,*AR5			;link back in the object behind the player
FLMSORTX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLAMESORT", 0, 0);
    UNIMPL();
}

void GETCARBODY(void)
{
    // *----------------------------------------------------------------------------
    // *FIND THE MATRIX IN THE DYNALIST OF THE CAR POINTED TO BY AR2
    // *INPUT	AR2 = CARS OBJECT
    // *OUTPUT AR0 = MATRIX OF THE BODY
    // * SCRAMBLES R0,R1
    // asm: 	LDI	*+AR2(ODYNALIST),R0
    // asm: 	SLOCKON	Z,"UTIL\CARPROC   dynamic objects not found"
FBLOOP:
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(DYNAFLAG),R1
    // asm: 	BZ	FOUND_BODY		;0 = car body
    // asm: 	LDI	*AR0,R0
    // asm: 	BR	FBLOOP
FOUND_BODY:
    // asm: 	ADDI	DYNAMATRIX,AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCARBODY", 0, 0);
    UNIMPL();
}

void FIND_NEXT_OBJ(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	AR5
    // asm: 	LDI	*AR0,AR5
OFN:
    // asm: LDI	AR5,R0
    // asm: 	LDI	R0,AR0
    // asm: 	BZ	OFNX		;NONE FOUND. This routine is passive
    // asm: 	LDI	*AR0,AR5
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	CMPI	R1,R0
    // asm: 	BNE	OFN
OFNX:
    // asm: 	CLRC
    // asm: 	CMPI	0,AR0
    // asm: 	BNE	OFNX1
    // asm: 	SETC
OFNX1:
    // asm: 	POP	AR5
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_NEXT_OBJ", 0, 0);
    UNIMPL();
}

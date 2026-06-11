#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/wheel.h"

/*
 * Source module: asm/WHEEL.ASM
 */

// *----------------------------------------------------------------------------
// *FORCE FEEDBACK WHEEL CODE
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
#define WHEEL_KILL 0
#define WHEEL 0x0995000
/* asm: POSE	.bss	POSE,1 */
int POSE;
/* asm: lpot0	pbss	lpot0,1 */
int lpot0;
/* asm: WHEELPWR	pbss	WHEELPWR,1 */
int WHEELPWR;
/* asm: WHEELPOS	.bss	WHEELPOS,1 */
int WHEELPOS;
/* asm: WHEELOUT	.bss	WHEELOUT,1 */
int WHEELOUT;
/* asm: WHEELMAX	.bss	WHEELMAX,1 */
int WHEELMAX;
/* asm: WHEELVEL	pbss	WHEELVEL,1 */
int WHEELVEL;
/* asm: DAMPPWR	.bss	DAMPPWR,1 */
int DAMPPWR;
// *----------------------------------------------------------------------------
// *WHEEL HANDLER
// *
// *PARAMETERS
// *	WHEELPWR	FORCE
// *	WHEELPOS	POSITION DESIRED
// *
// *----------------------------------------------------------------------------

void NUWHEEL(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	LDF	0,R1	   		;DEFAULT POSITION POWER MULTIPLIER
    // asm: 	LDF	126,R2	   		;DEFAULT MAX POWER
    // asm: 	LDF	2.5,R3	   		;DEFAULT DAMP POWER MULTIPLIER
    // ;	LDF	0,R1	   		;DEFAULT POSITION POWER MULTIPLIER
    // ;	LDF	126,R2	   		;DEFAULT MAX POWER
    // ;	LDF	2.5,R3	   		;DEFAULT DAMP POWER MULTIPLIER
    // asm: 	CMPI	MGAME,R0		;GAME
    // asm: 	BNE	NUWHL1
    // asm: 	LDF	@WHEELPWR,R1
    // asm: 	LDF	@WHEELPWR,R3
    // asm: 	LDF	126,R2
    // asm: 	B	NUWHL
NUWHL1:
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BNE	NBON
    // asm: 	LDF	2,R1
    // ;	LDF	64,R2
    // ;	LDF	2,R3
    // asm: 	B	NUWHL
NBON:
    // asm: 	CMPI	MINSERT_COINS,R0
    // asm: 	BNE	NIC
    // asm: 	LDF	0,R1		;2
    // ;	LDF	64,R2
    // ;	LDF	2,R3
    // asm: 	B	NUWHL
NIC:
    // asm: 	CMPI	MINTRO,R0		;INTRO
    // asm: 	BNE	NUWHL2
    // asm: 	LDF	0.5,R1
    // ;	LDF	64,R2
    // ;	LDF	0.5,R3
    // asm: 	B	NUWHL
NUWHL2:
    // asm: 	CMPI	MINIT,R0		;INITIAL ENTRY
    // asm: 	BNE	NUWHL3
    // asm: 	LDF	0.75,R1
    // ;	LDF	64,R2
    // ;	LDF	0.75,R3
    // asm: 	BU	NUWHL
NUWHL3:
    // asm: 	CMPI	MDIAG,R0
    // asm: 	BNE	NUWHL
    // asm: 	CLRF	R3
    // ;	BU	NUWHL
NUWHL:
    // asm: 	STF	R1,@WHEELPWR
    // asm: 	STF	R2,@WHEELMAX
    // asm: 	STF	R3,@DAMPPWR
    // asm: 	LDF	@WHEELPOS,R0
    // asm: 	FLOAT	@_pot0,R1
    // asm: 	SUBF	R1,R0
    // asm: 	LDF	@lpot0,R2
    // asm: 	STF	R1,@lpot0
    // asm: 	SUBF	R2,R1,R2		;curr - prev  (velocity)
    // asm: 	CMPF	-22,R2			;LIMIT VELOCITY
    // asm: 	LDFLT	-22,R2
    // asm: 	CMPF	22,R2
    // asm: 	LDFGT	22,R2
    // asm: 	LDF	@WHEELVEL,R1
    // asm: 	MPYF	0.55,R1
    // asm: 	MPYF	0.45,R2
    // asm: 	ADDF	R1,R2
    // asm: 	STF	R2,@WHEELVEL		;SAVE VELOCITY AVERAGE
    // asm: 	MPYF	2.2,R2
    // asm: 	MPYF	@DAMPPWR,R2
    // asm: 	MPYF	@WHEELPWR,R0
    // asm: 	SUBF	R2,R0			;ADD TO POSITION
    // asm: 	LDF	@WHEELMAX,R1	  	;MAX OUTPUT LEVEL
    // asm: 	CMPF	R1,R0
    // asm: 	LDFGT	R1,R0
    // asm: 	NEGF	R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,@WHEELOUT		;SAVE OUTPUT NUMBER
    // asm: 	FIX	R0
    // asm: 	AND	0FFH,R0
    // asm: 	CALL	TOWHEEL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NUWHEEL", 0, 0);
    UNIMPL();
}

void TOWHEEL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *THIS IS THE ONLY ROUTINE WHICH WRITES TO THE WHEEL
    // *
    // *
    // *PARAMETERS
    // *	R0	BYTE TO SEND TO WHEEL
    // *
    // *
    // asm: 	PUSH	DP
    // asm: 	PUSHM	R0,R1
    // asm: 	PUSH	R0
    // asm: 	LDP	@WHEEL
    // asm: 	LDI	0FF04h,R0		;SET ADDRESS TO 4
    // asm: 	LDI	0F704h,R1
    // asm: 	STI	R0,@WHEEL
    // asm: 	STI	R1,@WHEEL
    // asm: 	STI	R0,@WHEEL
    // asm: 	POP	R0
    // asm: 	LDI	R0,R1
    // asm: 	OR	0FF00h,R0
    // asm: 	STI	R0,@WHEEL
    // asm: 	OR	0FB00h,R1
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	STI	R1,@WHEEL
    // asm: 	POP	R1
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	STI	R0,@WHEEL
    // asm: 	POP	R0
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOWHEEL", 0, 0);
    UNIMPL();
}

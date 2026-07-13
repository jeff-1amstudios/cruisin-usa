
#include "wheel.h"
#include "../core/machine.h"
#include "globals.h"
#include "macs.h"
#include "sys.h"

/*
 * Source module: asm/WHEEL.ASM
 */

void NUWHEEL(void);
void TOWHEEL(void);

/*
 *----------------------------------------------------------------------------
 *FORCE FEEDBACK WHEEL CODE
 *
 */

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
float WHEELOUT;
/* asm: WHEELMAX	.bss	WHEELMAX,1 */
int WHEELMAX;
/* asm: WHEELVEL	pbss	WHEELVEL,1 */
int WHEELVEL;
/* asm: DAMPPWR	.bss	DAMPPWR,1 */
int DAMPPWR;

/*
 *----------------------------------------------------------------------------
 *WHEEL HANDLER
 *
 *PARAMETERS
 *	WHEELPWR	FORCE
 *	WHEELPOS	POSITION DESIRED
 *
 */
void NUWHEEL(void) {
    // asm 0000AFF2: 	LDI	@_MODE,R0
    // asm 0000AFF3: 	AND	MMODE,R0
    // asm 0000AFF4: 	LDF	0,R1	   		;DEFAULT POSITION POWER MULTIPLIER
    // asm 0000AFF5: 	LDF	126,R2	   		;DEFAULT MAX POWER
    // asm 0000AFF6: 	LDF	2.5,R3	   		;DEFAULT DAMP POWER MULTIPLIER
    // ;	LDF	0,R1	   		;DEFAULT POSITION POWER MULTIPLIER
    // ;	LDF	126,R2	   		;DEFAULT MAX POWER
    // ;	LDF	2.5,R3	   		;DEFAULT DAMP POWER MULTIPLIER
    // asm 0000AFF7: 	CMPI	MGAME,R0		;GAME
    // asm 0000AFF8: 	BNE	NUWHL1
    // asm 0000AFF9: 	LDF	@WHEELPWR,R1
    // asm 0000AFFA: 	LDF	@WHEELPWR,R3
    // asm 0000AFFB: 	LDF	126,R2
    // asm 0000AFFC: 	B	NUWHL
NUWHL1:
    // asm 0000AFFD: 	CMPI	MBONUS,R0
    // asm 0000AFFE: 	BNE	NBON
    // asm 0000AFFF: 	LDF	2,R1
    // ;	LDF	64,R2
    // ;	LDF	2,R3
    // asm 0000B000: 	B	NUWHL
NBON:
    // asm 0000B001: 	CMPI	MINSERT_COINS,R0
    // asm 0000B002: 	BNE	NIC
    // asm 0000B003: 	LDF	0,R1		;2
    // ;	LDF	64,R2
    // ;	LDF	2,R3
    // asm 0000B004: 	B	NUWHL
NIC:
    // asm 0000B005: 	CMPI	MINTRO,R0		;INTRO
    // asm 0000B006: 	BNE	NUWHL2
    // asm 0000B007: 	LDF	0.5,R1
    // ;	LDF	64,R2
    // ;	LDF	0.5,R3
    // asm 0000B008: 	B	NUWHL
NUWHL2:
    // asm 0000B009: 	CMPI	MINIT,R0		;INITIAL ENTRY
    // asm 0000B00A: 	BNE	NUWHL3
    // asm 0000B00B: 	LDF	0.75,R1
    // ;	LDF	64,R2
    // ;	LDF	0.75,R3
    // asm 0000B00C: 	BU	NUWHL
NUWHL3:
    // asm 0000B00D: 	CMPI	MDIAG,R0
    // asm 0000B00E: 	BNE	NUWHL
    // asm 0000B00F: 	CLRF	R3
    // ;	BU	NUWHL
NUWHL:
    // asm 0000B010: 	STF	R1,@WHEELPWR
    // asm 0000B011: 	STF	R2,@WHEELMAX
    // asm 0000B012: 	STF	R3,@DAMPPWR
    // asm 0000B013: 	LDF	@WHEELPOS,R0
    // asm 0000B014: 	FLOAT	@_pot0,R1
    // asm 0000B015: 	SUBF	R1,R0
    // asm 0000B016: 	LDF	@lpot0,R2
    // asm 0000B017: 	STF	R1,@lpot0
    // asm 0000B018: 	SUBF	R2,R1,R2		;curr - prev  (velocity)
    // asm 0000B019: 	CMPF	-22,R2			;LIMIT VELOCITY
    // asm 0000B01A: 	LDFLT	-22,R2
    // asm 0000B01B: 	CMPF	22,R2
    // asm 0000B01C: 	LDFGT	22,R2
    // asm 0000B01D: 	LDF	@WHEELVEL,R1
    // asm 0000B01E: 	MPYF	0.55,R1
    // asm 0000B01F: 	MPYF	0.45,R2
    // asm 0000B020: 	ADDF	R1,R2
    // asm 0000B021: 	STF	R2,@WHEELVEL		;SAVE VELOCITY AVERAGE
    // asm 0000B022: 	MPYF	2.2,R2
    // asm 0000B023: 	MPYF	@DAMPPWR,R2
    // asm 0000B024: 	MPYF	@WHEELPWR,R0
    // asm 0000B025: 	SUBF	R2,R0			;ADD TO POSITION
    // asm 0000B026: 	LDF	@WHEELMAX,R1	  	;MAX OUTPUT LEVEL
    // asm 0000B027: 	CMPF	R1,R0
    // asm 0000B028: 	LDFGT	R1,R0
    // asm 0000B029: 	NEGF	R1
    // asm 0000B02A: 	CMPF	R1,R0
    // asm 0000B02B: 	LDFLT	R1,R0
    // asm 0000B02C: 	STF	R0,@WHEELOUT		;SAVE OUTPUT NUMBER
    // asm 0000B02D: 	FIX	R0
    // asm 0000B02E: 	AND	0FFH,R0
    // asm 0000B02F: 	CALL	TOWHEEL
    // asm 0000B030: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NUWHEEL", 0, 0);
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *THIS IS THE ONLY ROUTINE WHICH WRITES TO THE WHEEL
 *
 *
 *PARAMETERS
 *	R0	BYTE TO SEND TO WHEEL
 *
 *
 */
void TOWHEEL(void) {
    // asm 0000B031: 	PUSH	DP
    // asm 0000B032: 	PUSHM	R0,R1
    // asm 0000B034: 	PUSH	R0
    // asm 0000B035: 	LDP	@WHEEL
    // asm 0000B036: 	LDI	0FF04h,R0		;SET ADDRESS TO 4
    // asm 0000B037: 	LDI	0F704h,R1
    // asm 0000B038: 	STI	R0,@WHEEL
    // asm 0000B039: 	STI	R1,@WHEEL
    // asm 0000B03A: 	STI	R0,@WHEEL
    // asm 0000B03B: 	POP	R0
    // asm 0000B03C: 	LDI	R0,R1
    // asm 0000B03D: 	OR	0FF00h,R0
    // asm 0000B03E: 	STI	R0,@WHEEL
    // asm 0000B03F: 	OR	0FB00h,R1
    // asm 0000B040: 	NOP
    // asm 0000B041: 	NOP
    // asm 0000B042: 	STI	R1,@WHEEL
    // asm 0000B043: 	POP	R1
    // asm 0000B044: 	NOP
    // asm 0000B045: 	NOP
    // asm 0000B046: 	STI	R0,@WHEEL
    // asm 0000B047: 	POP	R0
    // asm 0000B048: 	POP	DP
    // asm 0000B049: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TOWHEEL", 0, 0);
    UNIMPL();
}

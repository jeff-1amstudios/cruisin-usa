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
#include "roadblck.h"

/*
 * Source module: asm/ROADBLCK.ASM
 */

void INIT_RDDEBRIS(void);
void ADD_RDDEBRIS(void);
void FREE_RDDEBRIS(void);
void DEBRIS_SORT(void);
void DBINSLP(void);

/* asm: ROAD_DEBRIS	.bss	ROAD_DEBRIS,1 */
int ROAD_DEBRIS;
/* *----------------------------------------------------------------------------
*SORT YOUR DEBRIS IN THE WORLD
*
*PARAMETERS
*	AR4	PULL LIST HEADER
*
*
 */
/* asm: MAXDIST	.float	9999999999.0  		;A VERY LARGE NUMBER */
float MAXDIST = 9999999999.0f;

/* *----------------------------------------------------------------------------
 */
void INIT_RDDEBRIS(void)
{
    // asm: 	CLRI	R0
    // asm: 	STPI	R0,@ROAD_DEBRIS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_RDDEBRIS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*INSERT OBJECT ONTO THE ROAD DEBRIS LIST
*
*take object and insert on list and insert the closest
*road object to it
*
*OBJECT STRUCTURE ENTRIES USED:
*	OLINK3	list link
*	OUSR1	linked object
*
*PARAMETERS
*	AR4	OBJECT
*
*
 */
void ADD_RDDEBRIS(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@ROAD_DEBRIS,R0
#if DEBUG
    // asm: 	CMPI	AR4,R0
    // asm: 	BEQ	$			;Linking to ones self is not good
#endif
    // asm: 	STI	R0,*+AR4(OLINK3)
    // asm: 	STI	AR4,@ROAD_DEBRIS
    // asm: 	LDI	1,R0
    // asm: 	LS	O_DEBRIS_B,R0
    // asm: 	OR	*+AR4(OFLAGS),R0
    // asm: 	STI	R0,*+AR4(OFLAGS)
    // asm: 	LDI	RDDEBRIS_C|TSC_FLYING,R0
    // asm: 	STI	R0,*+AR4(OID)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR4(OUSR1)
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADD_RDDEBRIS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PULL OBJECT FROM ROAD DEBRIS LIST
*
*PARAMETERS
*	AR2	OBJECT
*
 */
void FREE_RDDEBRIS(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR1
    // asm: 	LDPI	@ROAD_DEBRISI,R0
    // asm: 	SUBI	OLINK3,R0		;(we are offset pointing)
FREELP:
    // asm: LDI	R0,AR1
    // asm: 	LDI	*+AR1(OLINK3),R0
#if DEBUG
    // asm: 	BZ	$			;lockup on end of list found
#endif
    // asm: 	BZ	FREEDR_X
    // asm: 	CMPI	R0,AR2
    // asm: 	BNE	FREELP
    // asm: 	LDI	*+AR2(OLINK3),R0
    // asm: 	STI	R0,*+AR1(OLINK3)	;LINK AROUND
FREEDR_X:
    // asm: 	POP	AR1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREE_RDDEBRIS", 0, 0);
    UNIMPL();
}

/* *
 */
void DEBRIS_SORT(void)
{
    // 	;PULL FLYING DEBRIS FROM THE LIST
    // 	;
    // asm: 	BUD	DSORTNXT
    // asm: 	NOP
    // asm: 	LDI	0,AR4			;INIT DRONE LIST HEADER
    // asm: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
    // 	;---->	BD DSORTNXT     	;GO GET FIRST ELEMENT
DSORTL:
    // asm: 	AND	CLASS_M,R1
    // asm: 	CMPI	RDDEBRIS_C,R1
    // asm: 	BNE	DSORTNXT
    // *FOUND DEBRIS, DELINK IT
    // asm: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
    // asm: 	STI	R0,*AR0			;LINK AROUND DUDE
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
    // asm: DSORTX
    // *INSERT DEBRIS ON OBJECT LIST
    // asm: 	LDPI	@DYNALIST_BEGIN,R5	;ANY ROAD LIST?
    // asm: 	BNZD	IDB1
    // asm: 	LDI	OPOSZ,IR0  		;GET Z INDEX
    // asm: 	LDI	AR4,R0	   		;GET HEADER
    // asm: 	NOP
    // 	;---->	BNZD	IDB1		;GO LOOP...
    // asm: 	B	IDBX			;WERE DONE, NULL LIST
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEBRIS_SORT", 0, 0);
    UNIMPL();
}

void DBINSLP(void)
{
    // asm: 	LDI	*+AR4(OUSR1),R0		;DO WE HAVE A LINK
    // asm: 	LDI	R0,AR5
    // asm: 	BZ	GETRK0			;NO, GET A NEW ONE
    // asm: 	LDI	*+AR5(OFLAGS),R1	;BELIEVE THAT WE ARE
    // asm: 	AND	O_LIST_M,R1		;CHECK VALID LIST..
    // asm: 	BNZ	GETLINK			;ON A REAL LIST
GETRK0:
    // asm: 	LDF	*+AR4(OPOSX),R3
    // asm: 	LDF	*+AR4(OPOSZ),R4
GETRK:
    // asm: 	SUBF	*+AR2(OPOSX),R3,R0
    // asm: 	MPYF	R0,R0
    // asm: 	SUBF	*+AR2(IR0),R4,R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R0,R1
    // asm: 	CMPF	R1,R2
    // asm: 	LDIGT	AR2,AR5
    // asm: 	LDFGT	R1,R2
    // asm: GETRKL
    // asm: 	LDI	*+AR2(OLINK4),R0
    // asm: 	BNZD	GETRK
    // asm: 	LDI	R0,AR2
    // asm: 	NOP
    // asm: 	NOP
    // 	;---->	BNZ	GETRK
    // asm: 	STI	AR5,*+AR4(OUSR1)	;SAVE LINK
    // asm: 	LDI	*+AR5(OFLAGS),R1	;BELIEVE THAT WE ARE
    // asm: 	AND	O_LIST_M,R1		;ON THE SAME LIST
GETLINK:
    // asm: 	LDI	*+AR4(OFLAGS),R2	;CHANGE LIST STATUS IF NECESSARY
    // asm: 	ANDN	O_LIST_M,R2
    // asm: 	OR	R2,R1
    // asm: 	STI	R1,*+AR4(OFLAGS)
    // *
    // *SORT PAST SHOULDER PIECES
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MBONUS,R0
    // asm: 	BNE	SHDONE
    // asm: 	LDI	AR5,AR1
SHLOOP:
    // asm: 	LDI	AR1,AR5
    // asm: 	LDI	*AR5,R0
    // asm: 	LDI	R0,AR1
    // asm: 	BZ	SHDONE
    // asm: 	LDI	*+AR1(OID),R0
    // asm: 	CMPI	310H,R0
    // asm: 	BZ	SHLOOP
    // asm: 	CMPI	400H,R0
    // asm: 	BZ	SHLOOP
    // asm: 	CMPI	40AH,R0
    // asm: 	BZ	SHLOOP
SHDONE:
    // asm: 	LDI	*AR5,R1			;GET HIS LINK
    // asm: 	LDI	*AR4,R0			;GET OUR LINK
    // asm: 	STI	AR4,*AR5		;HE LINKS TO US
    // asm: 	STI	R1,*AR4			;WE LINK TO IT
    // asm: 	LDI	R0,AR4
IDB1:
    // asm: 	BNED	DBINSLP
    // asm: 	LDI	R5,AR2			;GET ROAD LIST HEADER
    // asm: 	LDI	R5,AR5			;INITIAL CLOSEST CANDIDATE
    // asm: 	LDF	@MAXDIST,R2    		;INITIAL DISTANCE (INFINITY)
    // 	;---->	BNED	DBINSLP
IDBX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DBINSLP", 0, 0);
    UNIMPL();
}

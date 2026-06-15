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
    // asm 0000AF85: 	CLRI	R0
    // asm 0000AF86: 	STPI	R0,@ROAD_DEBRIS
    // asm 0000AF87: 	RETS
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
    // asm 0000AF88: 	PUSH	R0
    // asm 0000AF89: 	LDI	@ROAD_DEBRIS,R0
#if DEBUG
    // asm: 	CMPI	AR4,R0
    // asm: 	BEQ	$			;Linking to ones self is not good
#endif
    // asm 0000AF8A: 	STI	R0,*+AR4(OLINK3)
    // asm 0000AF8B: 	STI	AR4,@ROAD_DEBRIS
    // asm 0000AF8C: 	LDI	1,R0
    // asm 0000AF8D: 	LS	O_DEBRIS_B,R0
    // asm 0000AF8E: 	OR	*+AR4(OFLAGS),R0
    // asm 0000AF8F: 	STI	R0,*+AR4(OFLAGS)
    // asm 0000AF90: 	LDI	RDDEBRIS_C|TSC_FLYING,R0
    // asm 0000AF91: 	STI	R0,*+AR4(OID)
    // asm 0000AF92: 	LDI	0,R0
    // asm 0000AF93: 	STI	R0,*+AR4(OUSR1)
    // asm 0000AF94: 	POP	R0
    // asm 0000AF95: 	RETS
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
    // asm 0000AF96: 	PUSH	R0
    // asm 0000AF97: 	PUSH	AR1
    // asm 0000AF98: 	LDPI	@ROAD_DEBRISI,R0
    // asm 0000AF99: 	SUBI	OLINK3,R0		;(we are offset pointing)
FREELP:
    // asm 0000AF9A: LDI	R0,AR1
    // asm 0000AF9B: 	LDI	*+AR1(OLINK3),R0
#if DEBUG
    // asm: 	BZ	$			;lockup on end of list found
#endif
    // asm 0000AF9C: 	BZ	FREEDR_X
    // asm 0000AF9D: 	CMPI	R0,AR2
    // asm 0000AF9E: 	BNE	FREELP
    // asm 0000AF9F: 	LDI	*+AR2(OLINK3),R0
    // asm 0000AFA0: 	STI	R0,*+AR1(OLINK3)	;LINK AROUND
FREEDR_X:
    // asm 0000AFA1: 	POP	AR1
    // asm 0000AFA2: 	POP	R0
    // asm 0000AFA3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FREE_RDDEBRIS", 0, 0);
    UNIMPL();
}

/* *
 */
void DEBRIS_SORT(void)
{
    // 	;PULL FLYING DEBRIS FROM THE LIST
    // 	;
    // asm 0000AFA5: 	BUD	DSORTNXT
    // asm 0000AFA6: 	NOP
    // asm 0000AFA7: 	LDI	0,AR4			;INIT DRONE LIST HEADER
    // asm 0000AFA8: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
    // 	;---->	BD DSORTNXT     	;GO GET FIRST ELEMENT
DSORTL:
    // asm 0000AFA9: 	AND	CLASS_M,R1
    // asm 0000AFAA: 	CMPI	RDDEBRIS_C,R1
    // asm 0000AFAB: 	BNE	DSORTNXT
    // *FOUND DEBRIS, DELINK IT
    // asm 0000AFAC: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
    // asm 0000AFAD: 	STI	R0,*AR0			;LINK AROUND DUDE
    // asm 0000AFAE: 	STI	AR4,*AR1		;LINK HIM INTO TEMP LIST
    // asm 0000AFAF: 	LDI	AR1,AR4
    // asm 0000AFB0: 	LDI	AR0,AR1
DSORTNXT:
    // asm 0000AFB1: 	LDI	*AR1,R0
    // asm 0000AFB2: 	BNZD	DSORTL
    // asm 0000AFB3: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    // asm 0000AFB4: 	LDI	R0,AR1
    // asm 0000AFB5: 	LDI	*+AR1(OID),R1
    // 	;------>BNZD	DSORTL
    // asm 0000AFB6: DSORTX
    // *INSERT DEBRIS ON OBJECT LIST
    // asm 0000AFB6: 	LDPI	@DYNALIST_BEGIN,R5	;ANY ROAD LIST?
    // asm 0000AFB7: 	BNZD	IDB1
    // asm 0000AFB8: 	LDI	OPOSZ,IR0  		;GET Z INDEX
    // asm 0000AFB9: 	LDI	AR4,R0	   		;GET HEADER
    // asm 0000AFBA: 	NOP
    // 	;---->	BNZD	IDB1		;GO LOOP...
    // asm 0000AFBB: 	B	IDBX			;WERE DONE, NULL LIST
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEBRIS_SORT", 0, 0);
    UNIMPL();
}

void DBINSLP(void)
{
    // asm 0000AFBC: 	LDI	*+AR4(OUSR1),R0		;DO WE HAVE A LINK
    // asm 0000AFBD: 	LDI	R0,AR5
    // asm 0000AFBE: 	BZ	GETRK0			;NO, GET A NEW ONE
    // asm 0000AFBF: 	LDI	*+AR5(OFLAGS),R1	;BELIEVE THAT WE ARE
    // asm 0000AFC0: 	AND	O_LIST_M,R1		;CHECK VALID LIST..
    // asm 0000AFC1: 	BNZ	GETLINK			;ON A REAL LIST
GETRK0:
    // asm 0000AFC2: 	LDF	*+AR4(OPOSX),R3
    // asm 0000AFC3: 	LDF	*+AR4(OPOSZ),R4
GETRK:
    // asm 0000AFC4: 	SUBF	*+AR2(OPOSX),R3,R0
    // asm 0000AFC5: 	MPYF	R0,R0
    // asm 0000AFC6: 	SUBF	*+AR2(IR0),R4,R1
    // asm 0000AFC7: 	MPYF	R1,R1
    // asm 0000AFC8: 	ADDF	R0,R1
    // asm 0000AFC9: 	CMPF	R1,R2
    // asm 0000AFCA: 	LDIGT	AR2,AR5
    // asm 0000AFCB: 	LDFGT	R1,R2
    // asm 0000AFCC: GETRKL
    // asm 0000AFCC: 	LDI	*+AR2(OLINK4),R0
    // asm 0000AFCD: 	BNZD	GETRK
    // asm 0000AFCE: 	LDI	R0,AR2
    // asm 0000AFCF: 	NOP
    // asm 0000AFD0: 	NOP
    // 	;---->	BNZ	GETRK
    // asm 0000AFD1: 	STI	AR5,*+AR4(OUSR1)	;SAVE LINK
    // asm 0000AFD2: 	LDI	*+AR5(OFLAGS),R1	;BELIEVE THAT WE ARE
    // asm 0000AFD3: 	AND	O_LIST_M,R1		;ON THE SAME LIST
GETLINK:
    // asm 0000AFD4: 	LDI	*+AR4(OFLAGS),R2	;CHANGE LIST STATUS IF NECESSARY
    // asm 0000AFD5: 	ANDN	O_LIST_M,R2
    // asm 0000AFD6: 	OR	R2,R1
    // asm 0000AFD7: 	STI	R1,*+AR4(OFLAGS)
    // *
    // *SORT PAST SHOULDER PIECES
    // asm 0000AFD8: 	LDI	@_MODE,R0
    // asm 0000AFD9: 	AND	MMODE,R0
    // asm 0000AFDA: 	CMPI	MBONUS,R0
    // asm 0000AFDB: 	BNE	SHDONE
    // asm 0000AFDC: 	LDI	AR5,AR1
SHLOOP:
    // asm 0000AFDD: 	LDI	AR1,AR5
    // asm 0000AFDE: 	LDI	*AR5,R0
    // asm 0000AFDF: 	LDI	R0,AR1
    // asm 0000AFE0: 	BZ	SHDONE
    // asm 0000AFE1: 	LDI	*+AR1(OID),R0
    // asm 0000AFE2: 	CMPI	310H,R0
    // asm 0000AFE3: 	BZ	SHLOOP
    // asm 0000AFE4: 	CMPI	400H,R0
    // asm 0000AFE5: 	BZ	SHLOOP
    // asm 0000AFE6: 	CMPI	40AH,R0
    // asm 0000AFE7: 	BZ	SHLOOP
SHDONE:
    // asm 0000AFE8: 	LDI	*AR5,R1			;GET HIS LINK
    // asm 0000AFE9: 	LDI	*AR4,R0			;GET OUR LINK
    // asm 0000AFEA: 	STI	AR4,*AR5		;HE LINKS TO US
    // asm 0000AFEB: 	STI	R1,*AR4			;WE LINK TO IT
    // asm 0000AFEC: 	LDI	R0,AR4
IDB1:
    // asm 0000AFED: 	BNED	DBINSLP
    // asm 0000AFEE: 	LDI	R5,AR2			;GET ROAD LIST HEADER
    // asm 0000AFEF: 	LDI	R5,AR5			;INITIAL CLOSEST CANDIDATE
    // asm 0000AFF0: 	LDF	@MAXDIST,R2    		;INITIAL DISTANCE (INFINITY)
    // 	;---->	BNED	DBINSLP
IDBX:
    // asm 0000AFF1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DBINSLP", 0, 0);
    UNIMPL();
}

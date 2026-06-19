#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "vunit.h"
#include "globals.h"
#include "pall.h"
#include "totala.h"

/*
 * Source module: asm/TOTALA.ASM
 */

void _rdma(void);
void _stuff_fpga(void);
void BLTMOD2D_NOPAL(void);
void BLTMOD2D(void);
void BLTMOD2D_DS(void);

#define AIVI _AIVI

#define NOFIFO 0
/* asm: AIVI	.word	_AIVI */
/* asm: 	 */
#define AIVI _AIVI
/* asm: _ACNTL	.bss	_ACNTL,1 */
int _ACNTL;
/* asm: _ACMAP	.bss	_ACMAP,1 */
int _ACMAP;
/* asm: _ARPS	.bss	_ARPS,12 */
int _ARPS[12];
/* asm: _AIVI	.bss	_AIVI,4 */
int _AIVI[4];
/* asm: _ADDRL	.bss	_ADDRL,1 */
int _ADDRL;

/*
*----------------------------------------------------------------------------
*	rdma(x1,y1,x2,y2,palette,addr)
*
*PARAMETERS
*	AR2	x1
*	R2	y1
*	R3	x2
*	RC	y2
*	RS	palette
*	RE	addr
*
*
*/
void _rdma(void)
{
    // asm 0000AEFC: 	STI	RS,@_ACMAP
    // asm 0000AEFD: 	STI	RE,@_ADDRL
    // asm 0000AEFE: 	STI	AR2,@_ARPS+0		;AX
    // asm 0000AEFF: 	STI	AR2,@_ARPS+9		;DX
    // asm 0000AF00: 	STI	R2,@_ARPS+1		;AY
    // asm 0000AF01: 	STI	R2,@_ARPS+4		;BY
    // asm 0000AF02: 	STI	R3,@_ARPS+3		;BX
    // asm 0000AF03: 	STI	R3,@_ARPS+6		;CX
    // asm 0000AF04: 	STI	RC,@_ARPS+7		;CY
    // asm 0000AF05: 	STI	RC,@_ARPS+10		;DY
    // 	;BU	_stuff_fpga
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rdma", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*
*COPY THE CONTENTS OF _ACNTL TO _ADDRL INTO THE FIFO
*
*/
void _stuff_fpga(void)
{
    // asm 0000AF06: 	PUSH	AR0
    // asm 0000AF07: 	PUSH	AR1
    // asm 0000AF08: 	PUSH	R0
    // asm 0000AF09: 	PUSH	R1
    // asm 0000AF0A: 	PUSH	RC
    // asm 0000AF0B: 	PUSH	RS
    // asm 0000AF0C: 	PUSH	RE
#if STATISTICS
    // asm: 	LDI	@ST_POLYGONS,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@ST_POLYGONS
#endif
    // asm 0000AF0D: 	LDP	@FIFO_STATUS
STUFFWT:
    // asm 0000AF0E: LDI	@FIFO_STATUS,R0
    // asm 0000AF0F: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm 0000AF10: 	BNZ	STUFFWT
    // asm 0000AF11: 	SETDP
    // asm 0000AF12: 	FIFO_WT	R0
    // asm 0000AF17: 	LDP	@FIFO_ADDR,AR0
    // asm 0000AF18: 	LS	16,AR0
    // asm 0000AF19: 	LDI	@ASHADOW,AR1
    // asm 0000AF1A: 	LDI	*AR1++,R0
    // asm 0000AF1B: 	STI	R0,*AR0
    // asm 0000AF1B:  ||	LDI	*AR1++,R0
    // asm 0000AF1C: 	STI	R0,*AR0
    // asm 0000AF1D: 	LDI	3,RC
    // asm 0000AF1E: 	RPTB	LP1
    // asm 0000AF1F: 	LDI	*AR1++,R0			;x
    // asm 0000AF20: 	STI	R0,*AR0
    // asm 0000AF21: 	LDI	*AR1++(2),R0			;y
LP1:
    // asm 0000AF22: STI	R0,*AR0
    // asm 0000AF23:  	LDI	*AR1++,R0			;IVs
    // asm 0000AF24:  	STI	R0,*AR0
    // asm 0000AF24:  ||	LDI	*AR1++,R0
    // asm 0000AF25:  	STI	R0,*AR0
    // asm 0000AF25:  ||	LDI	*AR1++,R0
    // asm 0000AF26:  	STI	R0,*AR0
    // asm 0000AF26:  ||	LDI	*AR1++,R0
    // asm 0000AF27:  	STI	R0,*AR0
    // asm 0000AF27:  ||	LDI	*AR1++,R0			;addr
    // asm 0000AF28: 	STI	R0,*AR0
    // asm 0000AF29: 	LDP	@FIFO_INC
    // asm 0000AF2A: 	LDI	@FIFO_INC,R0
    // asm 0000AF2B: 	SETDP
    // asm 0000AF2C: 	CLRI	AR3
    // asm 0000AF2D: 	LDI	*AR3,R0
    // asm 0000AF2E:  	POP	RE
    // asm 0000AF2F: 	POP	RS
    // asm 0000AF30: 	POP	RC
    // asm 0000AF31: 	POP	R1
    // asm 0000AF32: 	POP	R0
    // asm 0000AF33: 	POP	AR1
    // asm 0000AF34: 	POP	AR0
    // asm 0000AF35: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_stuff_fpga", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*BITBLIT MODEL AS 2D IMAGE
*
*LOCKUP ON
*
*
*
*Internal vertices are used to get the image parameters
*
*PARAMETERS
*	AR2	MODEL POINTER
*	R2	POS X
*	R3	POS Y
*	R4	CNTL
*
*	BLTMOD2D_NOPAL
*		RS	PAL
*
*	.word	nV-1|(nP-1)<<16
*	{
*	.word	x,-y,z
*	} * nV
*	{
*	.word	control|pal<<16
*	.word	v1|v2<<8|v3<<16|v4<<24
*	.word	iv01,iv23
*	.word	addr
*	} * NUM_POLYGONS
*
*	This is expected:
*
*	Offset	Contents
*	0	radius
*	1	nV|nP	== 3
*	2	[x y z]
*	4	[x y z]
*	6	[x y z]
*	8	[x y z]
*	10	cntl|pal
*	11	verts
*	12	iv01
*	13	iv23
*	14	addr
*
*
*/
void BLTMOD2D_NOPAL(void)
{
    // asm 0000AF36: 	PUSHM	R0,R1,R2,R3,R4,R5,R6,R7
    // asm 0000AF3E: 	PUSHM	AR0,AR1,AR2,AR3
    // asm 0000AF42: 	BU	SKIPPALENTRY
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLTMOD2D_NOPAL", 0, 0);
    UNIMPL();
}

void BLTMOD2D(void)
{
    // asm 0000AF43: 	PUSHM	R0,R1,R2,R3,R4,R5,R6,R7
    // asm 0000AF4B: 	PUSHM	AR0,AR1,AR2,AR3
    // asm 0000AF4F: 	LDI	*+AR2(10),AR0		;IMAGE PAL
    // asm 0000AF50: 	RS	16,AR0
    // asm 0000AF51: 	ADDI	@_PALLISTI,AR0
    // asm 0000AF52: 	LDI	*AR0,RS
    // asm 0000AF53: 	RS	8,RS
SKIPPALENTRY:
    // asm 0000AF54: 	STI	R4,@_ACNTL
#if DEBUG
    // asm: 	LDI	*+AR2(1),R0
    // asm: 	CMPI	3,R0
    // asm: 	BNE	$		;it should have 4 vertices and 1 polygon
#endif
    // asm 0000AF55: 	LDI	-16,R0
    // asm 0000AF56: 	LDI	*+AR2(12),R4
    // asm 0000AF57: 	STI	R4,@_AIVI
    // asm 0000AF58: 	LSH	R0,R4,R5
    // asm 0000AF59: 	STI	R5,@_AIVI+1
    // asm 0000AF5A: 	LDI	*+AR2(13),R6
    // asm 0000AF5B: 	STI	R6,@_AIVI+2
    // asm 0000AF5C: 	LSH	R0,R6,R7
    // asm 0000AF5D: 	STI	R7,@_AIVI+3
    // asm 0000AF5E: 	LDI	*+AR2(14),RE		;IMAGE ADDR
    // asm 0000AF5F: 	PUSH	R3
    // asm 0000AF60: 	AND	0FFh,R4
    // asm 0000AF61: 	AND	0FFh,R6
    // asm 0000AF62: 	LDI	R2,AR2
    // asm 0000AF63: 	SUBI	R4,R6		;find xsize
    // asm 0000AF64: 	ADDI	R6,R2,R3	;add xsize to dest
    // asm 0000AF65: 	POP	R2
    // asm 0000AF66: 	RS	8,R7
    // asm 0000AF67: 	AND	0FFh,R7		;find y size
    // asm 0000AF68: 	ADDI	R7,R2,R0
    // asm 0000AF69: 	LDI	R0,RC
    // asm 0000AF6A: 	CALL	_rdma
    // asm 0000AF6B: 	POPM	AR3,AR2,AR1,AR0
    // asm 0000AF6F: 	POPM	R7,R6,R5,R4,R3,R2,R1,R0
    // asm 0000AF77: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLTMOD2D", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void BLTMOD2D_DS(void)
{
    // asm 0000AF78: 	PUSHM	R2,R3,R4
    // asm 0000AF7B: 	ADDI	2,R2
    // asm 0000AF7C: 	ADDI	2,R3
    // asm 0000AF7D: 	LDI	TM|ZS|NZR,R4
    // asm 0000AF7E: 	CALL	BLTMOD2D
    // asm 0000AF7F: 	POPM	R4,R3,R2
    // asm 0000AF82: 	CALL	BLTMOD2D
    // asm 0000AF83: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLTMOD2D_DS", 0, 0);
    UNIMPL();
}

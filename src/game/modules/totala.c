#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/vunit.h"
#include "../include/globals.h"
#include "../include/pall.h"
#include "../include/totala.h"

/*
 * Source module: asm/TOTALA.ASM
 */

// *----------------------------------------------------------------------------
// *TOTALA.ASM
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
#define NOFIFO 0
/* asm: AIVI	AIVI	.word	_AIVI */
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
// *----------------------------------------------------------------------------
// *	rdma(x1,y1,x2,y2,palette,addr)
// *
// *PARAMETERS
// *	AR2	x1
// *	R2	y1
// *	R3	x2
// *	RC	y2
// *	RS	palette
// *	RE	addr
// *
// *
// *----------------------------------------------------------------------------

void _rdma(void)
{
    // asm: 	STI	RS,@_ACMAP
    // asm: 	STI	RE,@_ADDRL
    // asm: 	STI	AR2,@_ARPS+0		;AX
    // asm: 	STI	AR2,@_ARPS+9		;DX
    // asm: 	STI	R2,@_ARPS+1		;AY
    // asm: 	STI	R2,@_ARPS+4		;BY
    // asm: 	STI	R3,@_ARPS+3		;BX
    // asm: 	STI	R3,@_ARPS+6		;CX
    // asm: 	STI	RC,@_ARPS+7		;CY
    // asm: 	STI	RC,@_ARPS+10		;DY
    // 	;BU	_stuff_fpga
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rdma", 0, 0);
    UNIMPL();
}

void _stuff_fpga(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *COPY THE CONTENTS OF _ACNTL TO _ADDRL INTO THE FIFO
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	RC
    // asm: 	PUSH	RS
    // asm: 	PUSH	RE
#if STATISTICS
    // asm: 	LDI	@ST_POLYGONS,R0
    // asm: 	INC	R0
    // asm: 	STI	R0,@ST_POLYGONS
#endif
    // asm: 	LDP	@FIFO_STATUS
    // asm: STUFFWT	LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_MAX_FLAG,R0
    // asm: 	BNZ	STUFFWT
    // asm: 	SETDP
    // asm: 	FIFO_WT	R0
    // asm: 	LDP	@FIFO_ADDR,AR0
    // asm: 	LS	16,AR0
    // asm: 	LDI	@ASHADOW,AR1
    // asm: 	LDI	*AR1++,R0
    // asm: 	STI	R0,*AR0
    // asm: 	STI	R0,*AR0
    // asm: 	LDI	3,RC
    // asm: 	RPTB	LP1
    // asm: 	LDI	*AR1++,R0			;x
    // asm: 	STI	R0,*AR0
    // asm: 	LDI	*AR1++(2),R0			;y
LP1:
    // asm: STI	R0,*AR0
    // asm:  	LDI	*AR1++,R0			;IVs
    // asm:  	STI	R0,*AR0
    // asm:  	STI	R0,*AR0
    // asm:  	STI	R0,*AR0
    // asm:  	STI	R0,*AR0
    // asm: 	STI	R0,*AR0
    // asm: 	LDP	@FIFO_INC
    // asm: 	LDI	@FIFO_INC,R0
    // asm: 	SETDP
    // asm: 	CLRI	AR3
    // asm: 	LDI	*AR3,R0
    // asm:  	POP	RE
    // asm: 	POP	RS
    // asm: 	POP	RC
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_stuff_fpga", 0, 0);
    UNIMPL();
}

void BLTMOD2D_NOPAL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *BITBLIT MODEL AS 2D IMAGE
    // *
    // *LOCKUP ON
    // *
    // *
    // *
    // *Internal vertices are used to get the image parameters
    // *
    // *PARAMETERS
    // *	AR2	MODEL POINTER
    // *	R2	POS X
    // *	R3	POS Y
    // *	R4	CNTL
    // *
    // *	BLTMOD2D_NOPAL
    // *		RS	PAL
    // *
    // *	.word	nV-1|(nP-1)<<16
    // *	{
    // *	.word	x,-y,z
    // *	} * nV
    // *	{
    // *	.word	control|pal<<16
    // *	.word	v1|v2<<8|v3<<16|v4<<24
    // *	.word	iv01,iv23
    // *	.word	addr
    // *	} * NUM_POLYGONS
    // *
    // *	This is expected:
    // *
    // *	Offset	Contents
    // *	0	radius
    // *	1	nV|nP	== 3
    // *	2	[x y z]
    // *	4	[x y z]
    // *	6	[x y z]
    // *	8	[x y z]
    // *	10	cntl|pal
    // *	11	verts
    // *	12	iv01
    // *	13	iv23
    // *	14	addr
    // *
    // *
    // asm: 	PUSHM	R0,R1,R2,R3,R4,R5,R6,R7
    // asm: 	PUSHM	AR0,AR1,AR2,AR3
    // asm: 	BU	SKIPPALENTRY
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLTMOD2D_NOPAL", 0, 0);
    UNIMPL();
}

void BLTMOD2D(void)
{
    // asm: 	PUSHM	R0,R1,R2,R3,R4,R5,R6,R7
    // asm: 	PUSHM	AR0,AR1,AR2,AR3
    // asm: 	LDI	*+AR2(10),AR0		;IMAGE PAL
    // asm: 	RS	16,AR0
    // asm: 	ADDI	@_PALLISTI,AR0
    // asm: 	LDI	*AR0,RS
    // asm: 	RS	8,RS
SKIPPALENTRY:
    // asm: 	STI	R4,@_ACNTL
#if DEBUG
    // asm: 	LDI	*+AR2(1),R0
    // asm: 	CMPI	3,R0
    // asm: 	BNE	$		;it should have 4 vertices and 1 polygon
#endif
    // asm: 	LDI	-16,R0
    // asm: 	LDI	*+AR2(12),R4
    // asm: 	STI	R4,@_AIVI
    // asm: 	LSH	R0,R4,R5
    // asm: 	STI	R5,@_AIVI+1
    // asm: 	LDI	*+AR2(13),R6
    // asm: 	STI	R6,@_AIVI+2
    // asm: 	LSH	R0,R6,R7
    // asm: 	STI	R7,@_AIVI+3
    // asm: 	LDI	*+AR2(14),RE		;IMAGE ADDR
    // asm: 	PUSH	R3
    // asm: 	AND	0FFh,R4
    // asm: 	AND	0FFh,R6
    // asm: 	LDI	R2,AR2
    // asm: 	SUBI	R4,R6		;find xsize
    // asm: 	ADDI	R6,R2,R3	;add xsize to dest
    // asm: 	POP	R2
    // asm: 	RS	8,R7
    // asm: 	AND	0FFh,R7		;find y size
    // asm: 	ADDI	R7,R2,R0
    // asm: 	LDI	R0,RC
    // asm: 	CALL	_rdma
    // asm: 	POPM	AR3,AR2,AR1,AR0
    // asm: 	POPM	R7,R6,R5,R4,R3,R2,R1,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLTMOD2D", 0, 0);
    UNIMPL();
}

void BLTMOD2D_DS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSHM	R2,R3,R4
    // asm: 	ADDI	2,R2
    // asm: 	ADDI	2,R3
    // asm: 	LDI	TM|ZS|NZR,R4
    // asm: 	CALL	BLTMOD2D
    // asm: 	POPM	R4,R3,R2
    // asm: 	CALL	BLTMOD2D
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLTMOD2D_DS", 0, 0);
    UNIMPL();
}

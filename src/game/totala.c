
#include "totala.h"
#include "../core/machine.h"
#include "../core/output.h"
#include "globals.h"
#include "macs.h"
#include "pall.h"
#include "validator.h"
#include "vunit.h"

/*
 * Source module: asm/TOTALA.ASM
 */

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
void _rdma(int x1, int y1, int x2, int y2, int palette, int image_addr) {
    // asm 0000AEFC: 	STI	RS,@_ACMAP
    _ACMAP = palette;
    // asm 0000AEFD: 	STI	RE,@_ADDRL
    _ADDRL = image_addr;
    // asm 0000AEFE: 	STI	AR2,@_ARPS+0		;AX
    _ARPS[0] = x1;
    // asm 0000AEFF: 	STI	AR2,@_ARPS+9		;DX
    _ARPS[9] = x1;
    // asm 0000AF00: 	STI	R2,@_ARPS+1		;AY
    _ARPS[1] = y1;
    // asm 0000AF01: 	STI	R2,@_ARPS+4		;BY
    _ARPS[4] = y1;
    // asm 0000AF02: 	STI	R3,@_ARPS+3		;BX
    _ARPS[3] = x2;
    // asm 0000AF03: 	STI	R3,@_ARPS+6		;CX
    _ARPS[6] = x2;
    // asm 0000AF04: 	STI	RC,@_ARPS+7		;CY
    _ARPS[7] = y2;
    // asm 0000AF05: 	STI	RC,@_ARPS+10		;DY
    _ARPS[10] = y2;
    // 	;BU	_stuff_fpga
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    _stuff_fpga();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *COPY THE CONTENTS OF _ACNTL TO _ADDRL INTO THE FIFO
 *
 */
void _stuff_fpga(void) {
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
    port_output_fpga(
        _ARPS[0], _ARPS[1], _ARPS[3], _ARPS[4],
        _ARPS[6], _ARPS[7], _ARPS[9], _ARPS[10],
        _AIVI[0], _AIVI[1], _AIVI[2], _AIVI[3],
        _ADDRL, _ACMAP, _ACNTL);
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
void BLTMOD2D_NOPAL(const BLTMOD2D_MODEL* model, int x, int y, int control, int palette) {
    // asm 0000AF36: 	PUSHM	R0,R1,R2,R3,R4,R5,R6,R7
    // asm 0000AF3E: 	PUSHM	AR0,AR1,AR2,AR3
    // asm 0000AF42: 	BU	SKIPPALENTRY
    BLTMOD2D(model, x, y, control, 1, palette);
}

void BLTMOD2D(const BLTMOD2D_MODEL* model, int x, int y, int control, int skip_palette, int palette) {
    int palette_index;
    tPALLIST_ENTRY* palette_entry;
    int iv01;
    int iv1;
    int iv23;
    int iv3;
    int image_addr;
    int x2;
    int y2;

    if (skip_palette) goto SKIPPALENTRY;

    // asm 0000AF43: 	PUSHM	R0,R1,R2,R3,R4,R5,R6,R7
    // asm 0000AF4B: 	PUSHM	AR0,AR1,AR2,AR3
    // asm 0000AF4F: 	LDI	*+AR2(10),AR0		;IMAGE PAL
    palette_index = model->control_palette;
    // asm 0000AF50: 	RS	16,AR0
    palette_index = (int)((u32)palette_index >> 16);
    // asm 0000AF51: 	ADDI	@_PALLISTI,AR0
    palette_entry = &_PALLIST[palette_index];
    // asm 0000AF52: 	LDI	*AR0,RS
    palette = palette_entry->ref_count_and_pal_code;
    // asm 0000AF53: 	RS	8,RS
    palette >>= 8;
SKIPPALENTRY:
    // asm 0000AF54: 	STI	R4,@_ACNTL
    _ACNTL = control;
#if DEBUG
    // asm: 	LDI	*+AR2(1),R0
    // asm: 	CMPI	3,R0
    // asm: 	BNE	$		;it should have 4 vertices and 1 polygon
    SLOCKON(model->vertex_polygon_count != 3, "BLTMOD2D model must have four vertices and one polygon");
#endif
    // asm 0000AF55: 	LDI	-16,R0
    // asm 0000AF56: 	LDI	*+AR2(12),R4
    iv01 = model->iv01;
    // asm 0000AF57: 	STI	R4,@_AIVI
    _AIVI[0] = iv01;
    // asm 0000AF58: 	LSH	R0,R4,R5
    iv1 = (int)((u32)iv01 >> 16);
    // asm 0000AF59: 	STI	R5,@_AIVI+1
    _AIVI[1] = iv1;
    // asm 0000AF5A: 	LDI	*+AR2(13),R6
    iv23 = model->iv23;
    // asm 0000AF5B: 	STI	R6,@_AIVI+2
    _AIVI[2] = iv23;
    // asm 0000AF5C: 	LSH	R0,R6,R7
    iv3 = (int)((u32)iv23 >> 16);
    // asm 0000AF5D: 	STI	R7,@_AIVI+3
    _AIVI[3] = iv3;
    // asm 0000AF5E: 	LDI	*+AR2(14),RE		;IMAGE ADDR
    image_addr = model->image_addr;
    // asm 0000AF5F: 	PUSH	R3
    // asm 0000AF60: 	AND	0FFh,R4
    iv01 &= 0xFF;
    // asm 0000AF61: 	AND	0FFh,R6
    iv23 &= 0xFF;
    // asm 0000AF62: 	LDI	R2,AR2
    // asm 0000AF63: 	SUBI	R4,R6		;find xsize
    iv23 -= iv01; // find xsize
    // asm 0000AF64: 	ADDI	R6,R2,R3	;add xsize to dest
    x2 = x + iv23; // add xsize to dest
    // asm 0000AF65: 	POP	R2
    // asm 0000AF66: 	RS	8,R7
    iv3 = (int)((u32)iv3 >> 8);
    // asm 0000AF67: 	AND	0FFh,R7		;find y size
    iv3 &= 0xFF; // find y size
    // asm 0000AF68: 	ADDI	R7,R2,R0
    y2 = y + iv3;
    // asm 0000AF69: 	LDI	R0,RC
    // asm 0000AF6A: 	CALL	_rdma
    _rdma(x, y, x2, y2, palette, image_addr);
    // asm 0000AF6B: 	POPM	AR3,AR2,AR1,AR0
    // asm 0000AF6F: 	POPM	R7,R6,R5,R4,R3,R2,R1,R0
    // asm 0000AF77: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void BLTMOD2D_DS(const BLTMOD2D_MODEL* model, int x, int y, int control) {
    // asm 0000AF78: 	PUSHM	R2,R3,R4
    // asm 0000AF7B: 	ADDI	2,R2
    // asm 0000AF7C: 	ADDI	2,R3
    // asm 0000AF7D: 	LDI	TM|ZS|NZR,R4
    // asm 0000AF7E: 	CALL	BLTMOD2D
    BLTMOD2D(model, x + 2, y + 2, TM | ZS | NZR, 0, 0);
    // asm 0000AF7F: 	POPM	R4,R3,R2
    // asm 0000AF82: 	CALL	BLTMOD2D
    BLTMOD2D(model, x, y, control, 0, 0);
    // asm 0000AF83: 	RETS
}

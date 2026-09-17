
#include "../core/machine.h"
#include "macs.h"
#include "routs.h"

/*
 * Source module: asm/LINE.ASM
 */

void _rectangle(int x1, int y1, int x2, int y2, int color);
void _line(int x0, int y0, int x1, int y1, int color);

void _pixel(int x, int y, int color);

/*
******************************************************
* FUNCTION DEF : _rectangle
******************************************************
*/
void _rectangle(int x1, int y1, int x2, int y2, int color) {
    // asm: PUSH	FP
    // asm: LDI	SP,FP
    // asm: ADDI	3,SP
    // asm: PUSH	R4
    // asm: PUSH	R5
    // asm: STI	RS,*+FP(1)
    // asm: STI	R3,*+FP(3)
    // asm: STI	AR2,*+FP(2)
    // asm: LDI	R2,R5
    // asm: LDI	RC,R4
    // asm: PUSH DP
    // asm: LDI	R5,R2
    // asm: LDI	R5,RC
    // asm: CALL	_line
    _line(x1, y1, x2, y1, color);
    // asm: LDI	*+FP(2),AR2
    // asm: LDI	R4,R2
    // asm: LDI	*+FP(3),R3
    // asm: LDI	R4,RC
    // asm: LDI	*+FP(1),RS
    // asm: CALL	_line
    _line(x1, y2, x2, y2, color);
    // asm: LDI	*+FP(2),AR2
    // asm: LDI	R5,R2
    // asm: LDI	*+FP(2),R3
    // asm: LDI	R4,RC
    // asm: LDI	*+FP(1),RS
    // asm: CALL	_line
    _line(x1, y1, x1, y2, color);
    // asm: LDI	*+FP(3),AR2
    // asm: LDI	R5,R2
    // asm: LDI	*+FP(3),R3
    // asm: LDI	R4,RC
    // asm: LDI	*+FP(1),RS
    // asm: CALL	_line
    _line(x2, y1, x2, y2, color);
    // asm: POP DP
EPI0_1:
    // asm 0000AC4D: 	LDI	*-FP(1),R1
    // asm 0000AC4E: 	LDI	*FP,FP
    // asm 0000AC4F: 	BD	R1
    // asm 0000AC50: 	POP	R5
    // asm 0000AC51: 	POP	R4
    // asm 0000AC52: 	SUBI	5,SP
    // ***	B	R1	;BRANCH OCCURS
    return;
}

void _line(int x0, int y0, int x1, int y1, int color) {
    int i;
    int remaining;
    int temporary;
    int integer_delta;
    c3x_reg_t x;
    c3x_reg_t y;
    c3x_reg_t x_delta;
    c3x_reg_t y_delta;
    c3x_reg_t x_distance;
    c3x_reg_t y_distance;
    c3x_reg_t reciprocal;

    // asm: PUSH	FP
    // asm: LDI	SP,FP
    // asm: ADDI	3,SP
    // asm: PUSH	R4
    // asm: PUSH	R5
    // asm: PUSHF	R6
    // asm: PUSHF	R7
    // asm: STI	RS,*+FP(1)
    // asm: STI	R2,*+FP(3)
    // asm: STI	AR2,*+FP(2)
    // asm: LDI	R3,RS
    // asm: PUSH DP
    // asm: CMPI	*+FP(2),RS
    // asm: BZ	L32
    if (x1 == x0) {
        goto L32;
    }

    // asm: CMPI	RC,R2
    // asm: BZ	L26
    if (y0 == y1) {
        goto L26;
    }

    // asm: SUBI	R2,RC,R0
    integer_delta = y1 - y0;
    // asm: FLOAT	R0,R2
    y_distance = C3X_FROM_INT(integer_delta);
    // asm: LDF	R2,R0
    // asm: CALL	INV_F30
    reciprocal = INV_F30(y_distance);
    // asm: RND	R0
    reciprocal = C3X_RND(reciprocal);
    // asm: LDI	*+FP(2),R1
    // asm: SUBI	R1,RS,R1
    integer_delta = x1 - x0;
    // asm: FLOAT	R1,R3
    x_distance = C3X_FROM_INT(integer_delta);
    // asm: MPYF	R0,R3,R7
    x_delta = C3X_MUL(x_distance, reciprocal);

    // asm: CMPF	1.0,R7
    // asm: BGT	L14
    if (C3X_GT(x_delta, C3X_IMM_F32(1.0))) {
        goto L14;
    }
    // asm: CMPF	-1.0,R7
    // asm: BLT	L14
    if (C3X_LT(x_delta, C3X_IMM_F32(-1.0))) {
        goto L14;
    }

    // asm: CMPI	*+FP(3),RC
    // asm: BGE	L5
    if (y1 >= y0) {
        goto L5;
    }
    // asm: LDI	*+FP(2),R2
    temporary = x0;
    // asm: STI	RS,*+FP(2)
    x0 = x1;
    // asm: LDI	*+FP(3),R3
    integer_delta = y0;
    // asm: STI	RC,*+FP(3)
    y0 = y1;
    // asm: LDI	R3,RC
    y1 = integer_delta;
    // asm: SUBI	*+FP(3),R3
    integer_delta -= y0;
    // asm: FLOAT	R3,R0
    reciprocal = C3X_FROM_INT(integer_delta);
    // asm: CALL	INV_F30
    reciprocal = INV_F30(reciprocal);
    // asm: RND	R0
    reciprocal = C3X_RND(reciprocal);
    // asm: SUBI	*+FP(2),R2
    temporary -= x0;
    // asm: FLOAT	R2,R7
    x_delta = C3X_FROM_INT(temporary);
    // asm: MPYF	R0,R7
    x_delta = C3X_MUL(x_delta, reciprocal);
L5:
    // asm: FLOAT	*+FP(2),R6
    x = C3X_FROM_INT(x0);
    // asm: CMPF	0,R7
    // asm: BGT	L10
    if (C3X_GT(x_delta, C3X_IMM_F32(0))) {
        goto L10;
    }

    // asm: LDI	*+FP(3),R4
    i = y0;
    // asm: CMPI	RC,R4
    // asm: BGT	L38
    if (i > y1) {
        goto L38;
    }
    // asm: SUBI	R4,RC,R5
    remaining = y1 - i;
L44:
    // asm: FIX	R6,AR2
    // asm: LDI	R4,R2
    // asm: LDI	*+FP(1),R3
    // asm: CALL	_pixel
    _pixel(FIX(x), i, color);
    // asm: SUBI	1,R5
    remaining -= 1;
    // asm: BGED	L44
    // asm: ADDF	R7,R6
    x = C3X_ADD(x, x_delta);
    // asm:	NOP
    // asm: ADDI	1,R4
    i += 1;
    if (remaining >= 0) {
        goto L44;
    }
    // asm: B	L38
    goto L38;

L10:
    // asm: LDI	*+FP(3),R4
    i = y0;
    // asm: CMPI	RC,R4
    // asm: BGT	L38
    if (i > y1) {
        goto L38;
    }
    // asm: SUBI	R4,RC,R5
    remaining = y1 - i;
L43:
    // asm: FIX	R6,AR2
    // asm: LDI	R4,R2
    // asm: LDI	*+FP(1),R3
    // asm: CALL	_pixel
    _pixel(FIX(x), i, color);
    // asm: SUBI	1,R5
    remaining -= 1;
    // asm: BGED	L43
    // asm: ADDF	R7,R6
    x = C3X_ADD(x, x_delta);
    // asm:	NOP
    // asm: ADDI	1,R4
    i += 1;
    if (remaining >= 0) {
        goto L43;
    }
    // asm: B	L38
    goto L38;

L14:
    // asm: CMPI	*+FP(2),RS
    // asm: BLT	L16
    if (x1 < x0) {
        goto L16;
    }
    // asm: LDF	R3,R0
    reciprocal = x_distance;
    // asm: CALL	INV_F30
    reciprocal = INV_F30(reciprocal);
    // asm: BD	L17
    // asm: RND	R0
    reciprocal = C3X_RND(reciprocal);
    // asm:	NOP
    // asm: MPYF	R0,R2,R7
    y_delta = C3X_MUL(y_distance, reciprocal);
    goto L17;

L16:
    // asm: LDI	*+FP(2),R2
    temporary = x0;
    // asm: STI	RS,*+FP(2)
    x0 = x1;
    // asm: LDI	R2,RS
    x1 = temporary;
    // asm: LDI	*+FP(3),R2
    temporary = y0;
    // asm: STI	RC,*+FP(3)
    y0 = y1;
    // asm: LDI	*+FP(2),R0
    // asm: SUBI	R0,RS,R1
    integer_delta = x1 - x0;
    // asm: FLOAT	R1,R0
    reciprocal = C3X_FROM_INT(integer_delta);
    // asm: CALL	INV_F30
    reciprocal = INV_F30(reciprocal);
    // asm: RND	R0
    reciprocal = C3X_RND(reciprocal);
    // asm: SUBI	*+FP(3),R2
    temporary -= y0;
    // asm: FLOAT	R2,R7
    y_delta = C3X_FROM_INT(temporary);
    // asm: MPYF	R0,R7
    y_delta = C3X_MUL(y_delta, reciprocal);
L17:
    // asm: FLOAT	*+FP(3),R6
    y = C3X_FROM_INT(y0);
    // asm: CMPF	0,R7
    // asm: BGT	L22
    if (C3X_GT(y_delta, C3X_IMM_F32(0))) {
        goto L22;
    }

    // asm: LDI	*+FP(2),R4
    i = x0;
    // asm: CMPI	RS,R4
    // asm: BGT	L38
    if (i > x1) {
        goto L38;
    }
    // asm: SUBI	R4,RS,R5
    remaining = x1 - i;
L42:
    // asm: LDI	R4,AR2
    // asm: FIX	R6,R2
    // asm: LDI	*+FP(1),R3
    // asm: CALL	_pixel
    _pixel(i, FIX(y), color);
    // asm: SUBI	1,R5
    remaining -= 1;
    // asm: BGED	L42
    // asm: ADDF	R7,R6
    y = C3X_ADD(y, y_delta);
    // asm:	NOP
    // asm: ADDI	1,R4
    i += 1;
    if (remaining >= 0) {
        goto L42;
    }
    // asm: B	L38
    goto L38;

L22:
    // asm: LDI	*+FP(2),R4
    i = x0;
    // asm: CMPI	RS,R4
    // asm: BGT	L38
    if (i > x1) {
        goto L38;
    }
    // asm: SUBI	R4,RS,R5
    remaining = x1 - i;
L41:
    // asm: LDI	R4,AR2
    // asm: FIX	R6,R2
    // asm: LDI	*+FP(1),R3
    // asm: CALL	_pixel
    _pixel(i, FIX(y), color);
    // asm: SUBI	1,R5
    remaining -= 1;
    // asm: BGED	L41
    // asm: ADDF	R7,R6
    y = C3X_ADD(y, y_delta);
    // asm:	NOP
    // asm: ADDI	1,R4
    i += 1;
    if (remaining >= 0) {
        goto L41;
    }
    // asm: B	L38
    goto L38;

L26:
    // asm: CMPI	*+FP(2),RS
    // asm: BGE	L28
    if (x1 >= x0) {
        goto L28;
    }
    // asm: LDI	*+FP(2),R2
    temporary = x0;
    // asm: STI	RS,*+FP(2)
    x0 = x1;
    // asm: LDI	R2,RS
    x1 = temporary;
L28:
    // asm: LDI	*+FP(2),R4
    i = x0;
    // asm: CMPI	RS,R4
    // asm: BGT	L38
    if (i > x1) {
        goto L38;
    }
    // asm: SUBI	R4,RS,R5
    remaining = x1 - i;
L40:
    // asm: LDI	R4,AR2
    // asm: LDI	*+FP(3),R2
    // asm: LDI	*+FP(1),R3
    // asm: CALL	_pixel
    _pixel(i, y0, color);
    // asm: ADDI	1,R4
    i += 1;
    // asm: SUBI	1,R5
    remaining -= 1;
    // asm: BGE	L40
    if (remaining >= 0) {
        goto L40;
    }
    // asm: B	L38
    goto L38;

L32:
    // asm: CMPI	RC,R2
    // asm: BLE	L34
    if (y0 <= y1) {
        goto L34;
    }
    // asm: STI	RC,*+FP(3)
    temporary = y0;
    y0 = y1;
    // asm: LDI	R2,RC
    y1 = temporary;
L34:
    // asm: LDI	*+FP(3),R4
    i = y0;
    // asm: CMPI	RC,R4
    // asm: BGT	L38
    if (i > y1) {
        goto L38;
    }
    // asm: SUBI	R4,RC,R5
    remaining = y1 - i;
L39:
    // asm: LDI	*+FP(2),AR2
    // asm: LDI	R4,R2
    // asm: LDI	*+FP(1),R3
    // asm: CALL	_pixel
    _pixel(x0, i, color);
    // asm: ADDI	1,R4
    i += 1;
    // asm: SUBI	1,R5
    remaining -= 1;
    // asm: BGE	L39
    if (remaining >= 0) {
        goto L39;
    }
L38:
    // asm: POP DP
EPI0_2:
    // asm 0000ACF2: 	LDI	*-FP(1),R1
    // asm 0000ACF3: 	LDI	*FP,FP
    // asm 0000ACF4: 	POPF	R7
    // asm 0000ACF5: 	POPF	R6
    // asm 0000ACF6: 	BD	R1
    // asm 0000ACF7: 	POP	R5
    // asm 0000ACF8: 	POP	R4
    // asm 0000ACF9: 	SUBI	5,SP
    // ***	B	R1	;BRANCH OCCURS
    return;
}

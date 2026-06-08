#include "../../core/cpu.h"
#include "../../core/machine.h"

/*
 * Source module: asm/LINE.ASM
 */

// ******************************************************
// *    TMS320C30 C COMPILER     Version 4.50
// ******************************************************
// ;	C:\BIN\ac30.exe -mr -ic:\c30tools -x -q line.c e:\line.if
// ;	C:\BIN\opt30.exe -m -r -a -q -O2 e:\line.if e:\line.opt
// ;	C:\BIN\cg30.exe -o -n -b -m -p -a -c -q e:\line.opt line.asm e:\line.tmp
#define FP AR3
// ******************************************************
// * FUNCTION DEF : _rectangle
// ******************************************************

void _rectangle(void)
{
    // asm: 	PUSH	FP
    // asm: 	LDI	SP,FP
    // asm: 	ADDI	3,SP
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // *
    // * R4	assigned to parameter y2
    // * R5	assigned to parameter y1
    // *
    // asm: 	STI	RS,*+FP(1)
    // asm: 	STI	R3,*+FP(3)
    // asm: 	STI	AR2,*+FP(2)
    // asm: 	LDI	R2,R5
    // asm: 	LDI	RC,R4
    // asm:  PUSH DP
    // asm: 	LDI	R5,R2
    // asm: 	LDI	R5,RC
    // asm: 	CALL	_line
    // asm: 	LDI	*+FP(2),AR2
    // asm: 	LDI	R4,R2
    // asm: 	LDI	*+FP(3),R3
    // asm: 	LDI	R4,RC
    // asm: 	LDI	*+FP(1),RS
    // asm: 	CALL	_line
    // asm: 	LDI	*+FP(2),AR2
    // asm: 	LDI	R5,R2
    // asm: 	LDI	*+FP(2),R3
    // asm: 	LDI	R4,RC
    // asm: 	LDI	*+FP(1),RS
    // asm: 	CALL	_line
    // asm: 	LDI	*+FP(3),AR2
    // asm: 	LDI	R5,R2
    // asm: 	LDI	*+FP(3),R3
    // asm: 	LDI	R4,RC
    // asm: 	LDI	*+FP(1),RS
    // asm: 	CALL	_line
    // asm:  POP DP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rectangle", 0, 0);
    UNIMPL();
}

void EPI0_1(void)
{
    // asm: 	LDI	*-FP(1),R1
    // asm: 	LDI	*FP,FP
    // asm: 	BD	R1
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	SUBI	5,SP
    // ***	B	R1	;BRANCH OCCURS
    // ******************************************************
    // * FUNCTION DEF : _line
    // ******************************************************
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_1", 0, 0);
    UNIMPL();
}

void _line(void)
{
    // asm: 	PUSH	FP
    // asm: 	LDI	SP,FP
    // asm: 	ADDI	3,SP
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSHF	R6
    // asm: 	PUSHF	R7
    // *
    // * R2	assigned to variable  t
    // * R3	assigned to variable  t
    // * R4	assigned to variable  i
    // * R6	assigned to variable  x
    // * R6	assigned to variable  y
    // * R7	assigned to variable  xd
    // * R7	assigned to variable  yd
    // * RC	assigned to parameter y2
    // * RS	assigned to parameter x2
    // *
    // asm: 	STI	RS,*+FP(1)
    // asm: 	STI	R2,*+FP(3)
    // asm: 	STI	AR2,*+FP(2)
    // asm: 	LDI	R3,RS
    // asm:  PUSH DP
    // asm: 	CMPI	*+FP(2),RS
    // asm: 	BZ	L32
    // asm: 	CMPI	RC,R2
    // asm: 	BZ	L26
    // asm: 	SUBI	R2,RC,R0
    // asm: 	FLOAT	R0,R2
    // asm: 	LDF	R2,R0
    // asm: 	CALL	INV_F30
    // asm: 	RND	R0
    // asm: 	LDI	*+FP(2),R1
    // asm: 	SUBI	R1,RS,R1
    // asm: 	FLOAT	R1,R3
    // asm: 	MPYF	R0,R3,R7
    // asm: 	CMPF	1.0,R7
    // asm: 	BGT	L14
    // asm: 	CMPF	-1.0,R7
    // asm: 	BLT	L14
    // asm: 	CMPI	*+FP(3),RC
    // asm: 	BGE	L5
    // asm: 	LDI	*+FP(2),R2
    // asm: 	STI	RS,*+FP(2)
    // asm: 	LDI	*+FP(3),R3
    // asm: 	STI	RC,*+FP(3)
    // asm: 	LDI	R3,RC
    // asm: 	SUBI	*+FP(3),R3
    // asm: 	FLOAT	R3,R0
    // asm: 	CALL	INV_F30
    // asm: 	RND	R0
    // asm: 	SUBI	*+FP(2),R2
    // asm: 	FLOAT	R2,R7
    // asm: 	MPYF	R0,R7
L5:
    // asm: 	FLOAT	*+FP(2),R6
    // asm: 	CMPF	0,R7
    // asm: 	BGT	L10
    // asm: 	LDI	*+FP(3),R4
    // asm: 	CMPI	RC,R4
    // asm: 	BGT	L38
    // asm: 	SUBI	R4,RC,R5
L44:
    // asm: 	FIX	R6,AR2
    // asm: 	LDI	R4,R2
    // asm: 	LDI	*+FP(1),R3
    // asm: 	CALL	_pixel
    // asm: 	SUBI	1,R5
    // asm: 	BGED	L44
    // asm: 	ADDF	R7,R6
    // asm: 	NOP
    // asm: 	ADDI	1,R4
    // ***	BGE	L44	;BRANCH OCCURS
    // asm: 	B	L38
L10:
    // asm: 	LDI	*+FP(3),R4
    // asm: 	CMPI	RC,R4
    // asm: 	BGT	L38
    // asm: 	SUBI	R4,RC,R5
L43:
    // asm: 	FIX	R6,AR2
    // asm: 	LDI	R4,R2
    // asm: 	LDI	*+FP(1),R3
    // asm: 	CALL	_pixel
    // asm: 	SUBI	1,R5
    // asm: 	BGED	L43
    // asm: 	ADDF	R7,R6
    // asm: 	NOP
    // asm: 	ADDI	1,R4
    // ***	BGE	L43	;BRANCH OCCURS
    // asm: 	B	L38
L14:
    // asm: 	CMPI	*+FP(2),RS
    // asm: 	BLT	L16
    // asm: 	LDF	R3,R0
    // asm: 	CALL	INV_F30
    // asm: 	BD	L17
    // asm: 	RND	R0
    // asm: 	NOP
    // asm: 	MPYF	R0,R2,R7
    // ***	B	L17	;BRANCH OCCURS
L16:
    // asm: 	LDI	*+FP(2),R2
    // asm: 	STI	RS,*+FP(2)
    // asm: 	LDI	R2,RS
    // asm: 	LDI	*+FP(3),R2
    // asm: 	STI	RC,*+FP(3)
    // asm: 	LDI	*+FP(2),R0
    // asm: 	SUBI	R0,RS,R1
    // asm: 	FLOAT	R1,R0
    // asm: 	CALL	INV_F30
    // asm: 	RND	R0
    // asm: 	SUBI	*+FP(3),R2
    // asm: 	FLOAT	R2,R7
    // asm: 	MPYF	R0,R7
L17:
    // asm: 	FLOAT	*+FP(3),R6
    // asm: 	CMPF	0,R7
    // asm: 	BGT	L22
    // asm: 	LDI	*+FP(2),R4
    // asm: 	CMPI	RS,R4
    // asm: 	BGT	L38
    // asm: 	SUBI	R4,RS,R5
L42:
    // asm: 	LDI	R4,AR2
    // asm: 	FIX	R6,R2
    // asm: 	LDI	*+FP(1),R3
    // asm: 	CALL	_pixel
    // asm: 	SUBI	1,R5
    // asm: 	BGED	L42
    // asm: 	ADDF	R7,R6
    // asm: 	NOP
    // asm: 	ADDI	1,R4
    // ***	BGE	L42	;BRANCH OCCURS
    // asm: 	B	L38
L22:
    // asm: 	LDI	*+FP(2),R4
    // asm: 	CMPI	RS,R4
    // asm: 	BGT	L38
    // asm: 	SUBI	R4,RS,R5
L41:
    // asm: 	LDI	R4,AR2
    // asm: 	FIX	R6,R2
    // asm: 	LDI	*+FP(1),R3
    // asm: 	CALL	_pixel
    // asm: 	SUBI	1,R5
    // asm: 	BGED	L41
    // asm: 	ADDF	R7,R6
    // asm: 	NOP
    // asm: 	ADDI	1,R4
    // ***	BGE	L41	;BRANCH OCCURS
    // asm: 	B	L38
L26:
    // asm: 	CMPI	*+FP(2),RS
    // asm: 	BGE	L28
    // asm: 	LDI	*+FP(2),R2
    // asm: 	STI	RS,*+FP(2)
    // asm: 	LDI	R2,RS
L28:
    // asm: 	LDI	*+FP(2),R4
    // asm: 	CMPI	RS,R4
    // asm: 	BGT	L38
    // asm: 	SUBI	R4,RS,R5
L40:
    // asm: 	LDI	R4,AR2
    // asm: 	LDI	*+FP(3),R2
    // asm: 	LDI	*+FP(1),R3
    // asm: 	CALL	_pixel
    // asm: 	ADDI	1,R4
    // asm: 	SUBI	1,R5
    // asm: 	BGE	L40
    // asm: 	B	L38
L32:
    // asm: 	CMPI	RC,R2
    // asm: 	BLE	L34
    // asm: 	STI	RC,*+FP(3)
    // asm: 	LDI	R2,RC
L34:
    // asm: 	LDI	*+FP(3),R4
    // asm: 	CMPI	RC,R4
    // asm: 	BGT	L38
    // asm: 	SUBI	R4,RC,R5
L39:
    // asm: 	LDI	*+FP(2),AR2
    // asm: 	LDI	R4,R2
    // asm: 	LDI	*+FP(1),R3
    // asm: 	CALL	_pixel
    // asm: 	ADDI	1,R4
    // asm: 	SUBI	1,R5
    // asm: 	BGE	L39
L38:
    // asm:  POP DP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_line", 0, 0);
    UNIMPL();
}

void EPI0_2(void)
{
    // asm: 	LDI	*-FP(1),R1
    // asm: 	LDI	*FP,FP
    // asm: 	POPF	R7
    // asm: 	POPF	R6
    // asm: 	BD	R1
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	SUBI	5,SP
    // ***	B	R1	;BRANCH OCCURS
    // ******************************************************
    // * UNDEFINED REFERENCES                               *
    // ******************************************************
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_2", 0, 0);
    UNIMPL();
}

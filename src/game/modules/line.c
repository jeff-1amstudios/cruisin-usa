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
// ***	B	R1	;BRANCH OCCURS
// ***	BGE	L44	;BRANCH OCCURS
// ***	BGE	L43	;BRANCH OCCURS
// ***	B	L17	;BRANCH OCCURS
// ***	BGE	L42	;BRANCH OCCURS
// ***	BGE	L41	;BRANCH OCCURS
// ***	B	R1	;BRANCH OCCURS
// ******************************************************
// * UNDEFINED REFERENCES                               *
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

void _line(void)
{
    // ******************************************************
    // * FUNCTION DEF : _line
    // ******************************************************
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_line", 0, 0);
    UNIMPL();
}

void L38(void)
{
    // asm:  POP DP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "L38", 0, 0);
    UNIMPL();
}

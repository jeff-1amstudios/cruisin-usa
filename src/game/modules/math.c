#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/dirq.h"
#include "../include/math.h"

/*
 * Source module: asm/MATH.ASM
 */

// *----------------------------------------------------------------------------
// *MATH ROUTINES
// *
// *COPYRIGHT (C) 1994 BY TV GAMES,INC.
// *ALL RIGHTS RESERVED
// *
// *See also:	ROUTS.ASM	Divide,Invert,Modulus
// *		HPMATH.C	higher precision functions
// *
// *----------------------------------------------------------------------------
// *	float	COSI(float theta)
// *	float	SINE(float theta)
// *COSINE FUNCTION
// *
// *PARAMETERS
// *	R2	VALUE IN RADIANS 0-INFINITY
// *	R0	RETURN VALUE
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: SINTABLEI	.word	SINTABLE */
int SINTABLEI = (int)(SINTABLE);
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: LOCTEMPER_MAT2I	.word	LOCTEMPER_MAT2 */
int LOCTEMPER_MAT2I = (int)(LOCTEMPER_MAT2);
// *----------------------------------------------------------------------------

void _COSI(void)
{
    // asm: 	LDF	R2,R0
    // asm: 	ADDF	@HALFPII,R0	;offset for COS
    // asm: 	B	SINE0
    // *
    // *SINE FUNCTION
    // *
    // *PARAMETERS
    // *	R2	VALUE IN RADIANS 0-INFINITY
    // *	R0	RETURN VALUE
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_COSI", 0, 0);
    UNIMPL();
}

void _SINE(void)
{
    // asm: 	LDF	R2,R0
SINE0:
    // asm: PUSH	AR0
    // asm: 	BGE	NOTNEG
    // asm: 	NEGF	R0		;fix negative case
    // asm: 	ADDF	@PII,R0
NOTNEG:
    // asm: MPYF	@FORMULA,R0
    // asm: 	ADDF	0.5,R0		;ROUND THE SUCKER
    // asm: 	FIX	R0		;get a raw index
    // asm: 	AND	3FFh,R0
    // asm: 	CMPI	512,R0
    // asm: 	BLT	ALLOK
    // asm: 	SUBI	512,R0
    // asm: 	CMPI	256,R0
    // asm: 	BLT	PERFECT2
    // asm: 	SUBI	512,R0
    // asm: 	NEGI	R0,R0
PERFECT2:
    // asm: 	ADDI	@SINTABLEI,R0
    // asm: 	LDI	R0,AR0
    // asm: 	NEGF	*AR0,R0
    // asm: 	POP	AR0
    // asm: 	RETS
ALLOK:
    // asm: 	CMPI	256,R0
    // asm: 	BLT	PERFECT
    // asm: 	SUBI	512,R0
    // asm: 	NEGI	R0,R0
PERFECT:
    // asm: ADDI	@SINTABLEI,R0
    // asm: 	LDI	R0,AR0
    // asm: 	LDF	*AR0,R0
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_SINE", 0, 0);
    UNIMPL();
}

void NORMITS(void)
{
    // *----------------------------------------------------------------------------
    // *PARAMETERS
    // *	R2	RADIANS
    // *OUTPUT
    // *	R2	IN RANGE -PI TO +PI
    // *	N,Z BITS SET FOR R2
    // *
    // asm: 	MPYF	@RADFORM,R2
    // asm: 	FIX	R2
    // asm: 	LS	16,R2
    // asm: 	RS	16,R2
    // asm: 	CMPI	7FFFH,R2
    // asm: 	BLT	NMS1
    // asm: 	ADDI	8000H,R2
    // asm: 	ADDI	8000H,R2
NMS1:
    // asm: 	FLOAT	R2
    // asm: 	MPYF	@RADFORMI,R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NORMITS", 0, 0);
    UNIMPL();
}

void NORMIT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PARAMETERS
    // *	R2	RADIANS
    // *OUTPUT
    // *	R2	IN RANGE 0 TO 2PI
    // *
    // asm: 	MPYF	@RADFORM,R2
    // asm: 	FIX	R2
    // asm: 	LS	16,R2
    // asm: 	RS	16,R2
    // asm: 	FLOAT	R2
    // asm: 	MPYF	@RADFORMI,R2
    // asm: NORMCHKL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NORMIT", 0, 0);
    UNIMPL();
}

void ARCTANF(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *ARCTANF - GET ARCTAN ( R3/R2) IN RADIANS
    // *float	arctanf(float x,float y)  <- not true C style
    // *
    // *PARAMETERS
    // *	R2	X (float)
    // *	R3	Y (float)
    // *RETURNS
    // *	R0	RADIANS (FLOAT)
    // *
    // asm: _arctanf
    // asm: 	PUSH	R1
    // asm: 	PUSHF	R1
    // asm: 	PUSH	R4
    // asm: 	PUSH	AR0
    // 	;GET QUADRANT
    // asm: 	LDI	0,R1
    // asm: 	LDF	R2,R2
    // asm: 	LDIN	2,R1		;GET SIGN BITS
    // asm: 	LDI	0,R4
    // asm: 	LDF	R3,R3
    // asm: 	LDIN	4,R4
    // asm: 	ADDI	R1,R4
    // 	;CHECK Y>X
    // asm: 	ABSF	R2,R0
    // asm: 	ABSF	R3,R1
    // asm: 	CMPF	R0,R1
    // asm: 	BLT	ATF0
    // asm: 	BD	AT1
    // asm: 	LDF	R2,R0
    // asm: 	LDF	R3,R1
    // asm: 	ADDI 	1,R4 		;R4 = OCTANT
    // 	;---->BD	AT1
ATF0:
    // asm: 	LDF	R2,R1
    // asm: 	LDF	R3,R0
AT1:
    // asm: 	CALL	DIV_F
    // asm: 	ABSF	R0
    // asm: 	LDF	128.0,R1
    // asm: 	MPYF	R1,R0
    // asm: 	FIX	R0,AR0
    // asm: 	ADDI	@ATTABV,AR0
    // asm: 	LDF	*AR0,R0
    // asm: 	LSH	1,R4
    // asm: 	LDI	@OFFTABV,AR0
    // asm: 	ADDI	R4,AR0
    // asm: 	MPYF	*AR0++,R0
    // asm: 	ADDF	*AR0,R0
    // asm: 	POP	AR0
    // asm: 	POP	R4
    // asm:       	POPF	R1
    // asm:       	POP	R1
    // asm: 	RETS
    // *
    // *OFFSET TABLE
    // *OFFSET, MULTIPLIER
    // *
    // asm: ATOFFTAB
    // *
    // *ARCTAN TABLE 0-45
    // *
    // asm: ATTAB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ARCTANF", 0, 0);
    UNIMPL();
}

void FIND_MATRIX(void)
{
    // *----------------------------------------------------------------------------
    // *FIND MATRIX
    // *
    // *PARAMETERS
    // *	AR2	DESTINATION 3X3
    // *	R2	SOURCE RADIANS 1X3
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R1
    // asm: 	PUSHF	R2
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // 	;GET SINES AND COSINES
    // asm: 	LDI	@LOCTEMPER_MAT2I,AR1
    // asm: 	LDI	R2,AR3
    // asm: 	LDI	2,RC
    // asm: 	RPTB	FM1
    // asm: 	LDF	*AR3++,R2
    // asm: 	CALL	_SINE
    // asm: 	STF	R0,*+AR1(3)
    // asm: 	CALL	_COSI
FM1:
    // asm: STF	R0,*AR1++
    // asm: 	NOP	*--AR1(2)			;CENTER COSINE INDEX
    // asm: 	LDI	AR1,AR3
    // asm: 	ADDI	3,AR3				;CENTER SINE INDEX
    // asm: 	MPYF	*+AR1(1),*+AR1(0),R0		;CZ*CY
    // asm: 	STF	R0,*AR2++		 	;A(0,0)=CZ*CY
    // asm: 	MPYF	*+AR3(1),*+AR1(0),R0		;SZ*CY
    // asm: 	NEGF	*+AR3(0),R2			;-SY
    // asm: 	MPYF	*-AR3(1),*+AR3(0),R0		;SX*SY
    // asm: 	MPYF	*+AR1(1),R0,R1			;SX*SY*CZ
    // asm: 	MPYF	*-AR1(1),*+AR3(1),R2		;CX*SZ
    // asm: 	SUBF	R2,R1
    // asm: 	MPYF	*+AR3(1),R0,R0			;SZ*(SX*SY)
    // asm: 	MPYF	*-AR1(1),*+AR1(1),R1		;CX*CZ
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*AR2++			;A(1,1)= SX*SY*SZ+CX*CZ
    // asm: 	MPYF	*-AR3(1),*+AR1(0),R0
    // asm: 	MPYF	*+AR3(0),R1,R1			;SY*(CX*CZ)
    // asm: 	MPYF	*-AR3(1),*+AR3(1),R0		;SX*SZ
    // asm: 	ADDF	R1,R0
    // asm: 	MPYF	*+AR3(0),R2,R2
    // asm: 	MPYF	*-AR3(1),*+AR1(1),R0
    // asm: 	MPYF	*-AR1(1),*+AR1(0),R1		;CX*CY
    // asm: 	STF	R2,*AR2++			;A(2,1)= CX*SY*SZ-SX*CZ
    // asm: 	STF	R1,*AR2--(8)			;A(2,2)= CX*CY
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POPF	R3
    // asm: 	POPF	R2
    // asm: 	POPF	R1
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_MATRIX", 0, 0);
    UNIMPL();
}

void FIND_XMATRIX(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *	    1  0  0
    // * Xrot = {  0  c  s }
    // *	    0 -s  c
    // *
    // *PARAMETERS
    // *	AR2	DESTINATION 3X3 MATRIX
    // *	R2	SOURCE RADIANS
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	_COSI
    // asm: 	STF	R0,*+AR2(A11)
    // asm: 	STF	R0,*+AR2(A22)
    // asm: 	CALL	_SINE
    // asm: 	STF	R0,*+AR2(A12)
    // asm: 	NEGF	R0
    // asm: 	STF	R0,*+AR2(A21)
    // asm: 	LDF	1,R0
    // asm: 	STF	R0,*+AR2(A00)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR2(A01)
    // asm: 	STF	R0,*+AR2(A02)
    // asm: 	STF	R0,*+AR2(A10)
    // asm: 	STF	R0,*+AR2(A20)
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_XMATRIX", 0, 0);
    UNIMPL();
}

void FIND_YMATRIX(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *	    c  0 -s
    // * Yrot = {  0  1  0 }
    // *	    s  0  c
    // *
    // *PARAMETERS
    // *	AR2	DESTINATION 3X3 MATRIX
    // *	R2	SOURCE RADIANS
    // *
    /* no executable asm lines detected */
    UNIMPL();
}

void _find_Ymatrix(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	_COSI
    // asm: 	STF	R0,*+AR2(A00)
    // asm: 	STF	R0,*+AR2(A22)
    // asm: 	CALL	_SINE
    // asm: 	STF	R0,*+AR2(A20)
    // asm: 	NEGF	R0
    // asm: 	STF	R0,*+AR2(A02)
    // asm: 	LDF	1,R0
    // asm: 	STF	R0,*+AR2(A11)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR2(A01)
    // asm: 	STF	R0,*+AR2(A10)
    // asm: 	STF	R0,*+AR2(A12)
    // asm: 	STF	R0,*+AR2(A21)
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_find_Ymatrix", 0, 0);
    UNIMPL();
}

void HPFIND_YMATRIX(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *	    c  0 -s
    // * Yrot = {  0  1  0 }
    // *	    s  0  c
    // *
    // *PARAMETERS
    // *	AR2	DESTINATION 3X3 MATRIX
    // *	R2	SOURCE RADIANS
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	R1
    // asm: 	PUSHF	R1
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R2
    // asm: 	PUSH	R3
    // asm: 	PUSHF	R3
    // asm: 	PUSH	R4
    // asm: 	PUSHF	R4
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R2
    // asm: 	CALL	_HPcos
    // asm: 	STF	R0,*+AR2(A00)
    // asm: 	STF	R0,*+AR2(A22)
    // asm: 	POPF	R2
    // asm: 	POP	R2
    // asm: 	CALL	_HPsin
    // asm: 	STF	R0,*+AR2(A20)
    // asm: 	NEGF	R0
    // asm: 	STF	R0,*+AR2(A02)
    // asm: 	LDF	1,R0
    // asm: 	STF	R0,*+AR2(A11)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR2(A01)
    // asm: 	STF	R0,*+AR2(A10)
    // asm: 	STF	R0,*+AR2(A12)
    // asm: 	STF	R0,*+AR2(A21)
    // asm: 	POPF	R4
    // asm: 	POP	R4
    // asm: 	POPF	R3
    // asm: 	POP	R3
    // asm: 	POPF	R2
    // asm: 	POP	R2
    // asm: 	POPF	R1
    // asm: 	POP	R1
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HPFIND_YMATRIX", 0, 0);
    UNIMPL();
}

void FIND_ZMATRIX(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // * 	    c s 0
    // * Zrot = { -s c 0 }
    // * 	    0 0 1
    // *
    // *PARAMETERS
    // *	AR2	DESTINATION 3X3 MATRIX
    // *	R2	SOURCE RADIANS
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	CALL	_COSI
    // asm: 	STF	R0,*+AR2(A00)
    // asm: 	STF	R0,*+AR2(A11)
    // asm: 	CALL	_SINE
    // asm: 	STF	R0,*+AR2(A01)
    // asm: 	NEGF	R0
    // asm: 	STF	R0,*+AR2(A10)
    // asm: 	LDF	1,R0
    // asm: 	STF	R0,*+AR2(A22)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR2(A02)
    // asm: 	STF	R0,*+AR2(A12)
    // asm: 	STF	R0,*+AR2(A20)
    // asm: 	STF	R0,*+AR2(A21)
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_ZMATRIX", 0, 0);
    UNIMPL();
}

void INITMAT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *INITIALIZE MATRIX TO IDENTITY
    // *
    // *PARAMETERS
    // *	AR0	POINTER TO MATRIX
    // *
    // asm: 	POP	BK			;RETURN ADDRESS
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	LDF	1.0,R0
    // asm: 	STF	R0,*AR0
    // asm: 	STF	R0,*+AR0(4)
    // asm: 	STF	R0,*+AR0(8)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR0(1)
    // asm: 	STF	R0,*+AR0(2)
    // asm: 	STF	R0,*+AR0(3)
    // asm: 	STF	R0,*+AR0(5)
    // asm: 	STF	R0,*+AR0(6)
    // asm: 	BUD	BK
    // asm: 	STF	R0,*+AR0(7)
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // 	;---->	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITMAT", 0, 0);
    UNIMPL();
}

void VECTLEN(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *float	vectlen(VECTOR *);
    // *
    // *PARAMETERS
    // *	AR2	PTS TO VECTOR
    // *RETURNS
    // *	R0	LENGTH OF VECTOR
    // *CLOBBERS
    // *	R1,R2
    // *
    // asm: 	LDF	*AR2++,R2
    // asm: 	MPYF	R2,R2
    // asm: 	LDF	*AR2++,R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	BUD	SQRT		;fall into SQRT
    // asm: 	LDF	*AR2--(2),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // 	;---->BU NOW INTO SQRT routine
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VECTLEN", 0, 0);
    UNIMPL();
}

void CPYMAT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	cpymat(MATRIX *dst,MATRIX *src);
    // *
    // *PARAMETERS
    // *	AR2	DEST MATRIX
    // *	R2	SOURCE MATRIX
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	LDI	R2,AR0
    // asm: 	LDF	*AR0++,R0
    // asm: 	RPTS	7
    // asm: 	LDF	*AR0++,R0
    // asm: 	STF	R0,*AR2--(8)
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CPYMAT", 0, 0);
    UNIMPL();
}

void CPYIMAT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	cpyimat(MATRIX *d,MATRIX *s)
    // *
    // *
    // *PARAMETERS
    // *	AR2	DEST
    // *	R2	SOURCE
    // *RETURNS
    // *	AR2	INVERSE MATRIX (ROWS & COLUMNS XCHNGED)
    // *
    // *WARNING SOURCE CANNOT BE SAME AS DEST
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	AR0
    // asm: 	LDI	R2,AR0
    // asm: 	LDF	*AR0++(2),R0   	;0->0
    // asm: 	LDF	*+AR0(1),R0	;3->1
    // asm: 	STF	R0,*AR2++
    // asm: 	LDF	*+AR0(4),R0	;6->2
    // asm: 	LDF	*-AR0(1),R0	;1->3
    // asm: 	STF	R0,*AR2++
    // asm: 	LDF	*+AR0(2),R0	;4->4
    // asm: 	STF	R0,*AR2++
    // asm: 	LDF	*+AR0(5),R0	;7->5
    // asm: 	LDF	*+AR0(0),R0	;2->6
    // asm: 	STF	R0,*AR2++
    // asm: 	LDF	*+AR0(3),R0	;5->7
    // asm: 	STF	R0,*AR2++
    // asm: 	LDF	*+AR0(6),R0	;8->8
    // asm: 	STF	R0,*AR2--(8)
    // asm: 	POP	AR0
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CPYIMAT", 0, 0);
    UNIMPL();
}

void CLR_VECTORA(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CLEARS VECTOR A and RETURNS POINTER TO IT IN AR2
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*AR2
    // asm: 	STF	R0,*+AR2(1)
    // asm: 	STF	R0,*+AR2(2)
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR_VECTORA", 0, 0);
    UNIMPL();
}

void MATRIX_MUL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	matrix_mul(VECTOR *src,MATRIX *m3x3,VECTOR *dst)
    // *
    // *PARAMETERS
    // *	AR2	SRC 1x3
    // *	R2	SRC 3x3
    // *	R3	DST 1x3
    // *RETURNS
    // *	*R3 modified, all regs saved
    // *
    // *NOTE SRC 1x3 and DST 1x3 may be equal
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	AR1
    // asm: 	LDI	R2,AR1	    		;AR1 IS 3X3 MATRIX
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R2
    // asm: 	MPYF	*AR2++,*AR1++,R0
    // asm: 	MPYF	*AR2,*AR1++,R2
    // asm: 	MPYF	*+AR2(1),*AR1++,R0
    // asm: 	MPYF	*-AR2(1),*AR1++,R0
    // asm: 	PUSHF	R2
    // asm: 	MPYF	*AR2,*AR1++,R2
    // asm: 	MPYF	*+AR2(1),*AR1++,R0
    // asm: 	MPYF	*-AR2(1),*AR1++,R0
    // asm: 	PUSHF	R2
    // asm: 	MPYF	*+AR2(1),*+AR1(1),R2
    // asm: 	MPYF	*AR2--(1),*AR1,R0
    // asm: 	LDI	R3,AR1
    // asm: 	ADDF	R2,R0
    // asm: 	POPF	R2
    // asm: 	STF	R0,*+AR1(2)
    // asm: 	POPF	R0
    // asm: 	STF	R0,*AR1
    // asm: 	POPF	R2
    // asm: 	POP	R2
    // asm: 	POP	AR1
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MATRIX_MUL", 0, 0);
    UNIMPL();
}

void NORMALIZE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	normalize(VECTOR *V)
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R1
    // asm: 	CALL	VECTLEN
    // asm: 	CALL	INV_F30
    // asm: 	MPYF	*AR2,R0,R1
    // asm: 	STF	R1,*AR2++
    // asm: 	MPYF	*AR2,R0,R1
    // asm: 	STF	R1,*AR2++
    // asm: 	MPYF	*AR2,R0,R1
    // asm: 	STF	R1,*AR2--(2)
    // asm: 	POPF	R1
    // asm: 	POPF	R0
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NORMALIZE", 0, 0);
    UNIMPL();
}

void NORMAT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *NORMAT		NORMALIZE MATRIX
    // *
    // *PARAMETERS
    // *	AR2	POINTS TO MATRIX
    // *
    // *AR0,AR1,R0,R1,R2 TRASHED
    // *
    // 	;NORMALIZE ROWS
    // asm: 	LDI	2,RC
    // asm: 	RPTB	NORMROW
    // 	;GET LENGTH
    // asm: 	LDF	*AR2++(3),R2
    // asm: 	MPYF	R2,R2
    // asm: 	LDF	*AR2++(3),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	LDF	*AR2--(6),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R2
    // asm: 	CALL	SQRT	 	;sqrt(R2)-->R0
    // asm: 	CALL	INV_F30		;INV(R0)-->R0
    // asm: 	MPYF	*AR2,R0,R1
    // asm: 	STF	R1,*AR2++(3)
    // asm: 	MPYF	*AR2,R0,R1
    // asm: 	STF	R1,*AR2++(3)
    // asm: 	MPYF	*AR2,R0,R1
NORMROW:
    // asm: STF	R1,*AR2--(5)
    // asm: 	NOP	*AR2--(3)	;RESTORE AR2 MATRIX POINTER
    // 	;NORMALIZE COLUMNS
    // asm: 	CALL	NORMALIZE
    // asm: 	NOP	*AR2++(3)
    // asm: 	CALL	NORMALIZE
    // asm: 	NOP	*AR2++(3)
    // asm: 	CALL	NORMALIZE
    // asm: 	NOP	*AR2--(6)	;RESTORE AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NORMAT", 0, 0);
    UNIMPL();
}

void GEN_NORMAL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *void	gen_normal(VECTOR *A,VECTOR *B,VECTOR *C,VECTOR *N)
    // *
    // *PARAMETERS
    // *	AR2	PTS TO PTRA,PTRB,PTRC
    // *	AR0	PTS TO NORMAL VECTOR TO RETURN
    // *
    // *CLOBBERS
    // *	R0-R7,AR2
    // *
    // asm: 	PUSH	AR0
    // asm: 	LDI	*+AR2(1),AR0		;B
    // asm: 	LDI	*+AR2(2),R3		;C
    // asm: 	LDI	*AR2,AR2		;A
    // asm: 	SUBF	*AR0++,*AR2++,R0	;D
    // asm: 	SUBF	*AR0,*AR2++,R1
    // asm: 	SUBF	*+AR0(1),*AR2++,R2
    // asm: 	LDI	R3,AR2			;C
    // asm: 	SUBF	*-AR0(1),*AR2++,R3	;E
    // asm: 	SUBF	*AR0,*AR2++,R4
    // asm: 	SUBF	*+AR0(1),*AR2++,R5
    // asm: 	POP	AR0
    // asm: 	MPYF	R1,R5,R6		;dy*ez
    // asm: 	MPYF	R2,R4,R7		;dz*ey
    // asm: 	SUBF	R7,R6
    // asm: 	STF	R6,*AR0
    // asm: 	MPYF	R2,R3,R6		;dz*ex
    // asm: 	MPYF	R0,R5,R7		;dx*ez
    // asm: 	SUBF	R7,R6
    // asm: 	STF	R6,*+AR0(1)
    // asm: 	MPYF	R0,R4,R6
    // asm: 	MPYF	R1,R3,R7
    // asm: 	SUBF	R7,R6
    // asm: 	STF	R6,*+AR0(2)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GEN_NORMAL", 0, 0);
    UNIMPL();
}

void CONCATMATV(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CONCATENATE MATRICES, VOLATILE
    // *void	concatmat(MATRIX *s1, MATRIX *s2, MATRIX *d);
    // *
    // *PARAMETERS
    // *	AR2	s1 SOURCE_1 MATRIX
    // *	R2	s2 SOURCE_2 MATRIX
    // *	R3	d  DEST MATRIX
    // *
    // *NOTE	__d can NOT be equal to either __s1 or __s2.
    // *
    // *
    // *
    // *__s1	   __s2	    		__d
    // *A B C	   J K L    AJ+DK+GL BJ+EK+HL CJ+FK+IL
    // *D E F	*  M N O  = AM+DN+GO BM+EN+HO CM+FN+IO
    // *G H I     P Q R    AP+DQ+GR BP+EQ+HR CP+FQ+IR
    // *
    // asm: 	LDI	R2,AR0
    // asm: 	LDI	R3,AR1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CONCATMATV", 0, 0);
    UNIMPL();
}

void CONCAT201(void)
{
    // asm: 	LDI	5,IR1
    // asm: 	LDI	3,IR0
    // asm: 	LDI	2,RC
    // asm: 	RPTB	INLP2
    // *LOOP THREE TIMES
    // asm: 	MPYF3	*AR0++,*AR2++(IR0),R0
    // asm: 	MPYF3	*AR0,*AR2++(IR0),R1
    // asm: 	MPYF3	*+AR0(1),*AR2--(IR1),R1
    // asm: 	MPYF3	*-AR0(1),*AR2++(IR0),R0
    // asm: 	MPYF3	*AR0,*AR2++(IR0),R1
    // asm: 	STF	R2,*AR1++(1)			;store MATij
    // asm: 	MPYF3	*+AR0(1),*AR2--(IR1),R1
    // asm: 	MPYF3	*-AR0(1),*AR2++(IR0),R0
    // asm: 	MPYF3	*AR0++,*AR2++(IR0),R1
    // asm: 	STF	R2,*AR1++(1)			;store MATij
    // asm: 	MPYF3	*AR0++,*AR2--(IR1),R1
    // asm: 	ADDF	R1,R2
    // asm: 	STF	R2,*AR1++(1)			;store MATij
INLP2:
    // asm: SUBI	3,AR2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CONCAT201", 0, 0);
    UNIMPL();
}

void CONCATMAT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CONCATENATE MATRICES
    // *
    // *this is the non-volatile concatmat,  if the destination is not the same as
    // *either source use concatmatv because it doesn't do the extra copy (for
    // *avoiding overwrites)
    // *
    // *void	concatmat(MATRIX *s1,MATRIX *s2,MATRIX *d);
    // *
    // *PARAMETERS
    // *	AR2	s1 SOURCE_1 MATRIX
    // *	R2	s2 SOURCE_2 MATRIX
    // *	R3	d  DEST MATRIX
    // *
    // *
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R3
    // asm: 	LDI	@LOCTEMPER_MATI,R3	;from DIRQ
    // asm: 	CALL	CONCATMATV
    // asm: 	POP	AR2
    // asm: 	LDI	@LOCTEMPER_MATI,R2
    // asm: 	CALL	CPYMAT
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CONCATMAT", 0, 0);
    UNIMPL();
}

void GETTHETADIFF(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GETTHETADIFF
    // *compute radY delta (as smallest possible turn)
    // *
    // *PARAMETERS
    // *	R0	DESIRED THETA (float)
    // *	R2	CURRENT THETA (float)
    // *RETURNS
    // *	R0	THETA DELTA (float)
    // *
    // asm: 	PUSHF	R1
    // asm: 	SUBF	R2,R0
    // asm: 	ABSF	R0,R1
    // asm: 	CMPF	@PII,R1
    // asm: 	BLT	NONEG
    // asm: 	LDF	R0,R0
    // asm: 	BN	ISNEG
    // asm: 	SUBF	@TWOPII,R0
    // asm: 	POPF	R1
    // asm: 	RETS
ISNEG:
    // asm: ADDF	@TWOPII,R0
NONEG:
    // asm: POPF	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTHETADIFF", 0, 0);
    UNIMPL();
}

void DIST_PT2LINE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *	LDI	AR2,AR0
    // *	LDP	@VECTORBI
    // *	LDI	@VECTORBI,AR1
    // *	LDF	*+AR4(OPOSX),R0
    // *	STF	R0,*+AR1(X)
    // *	CLRF	R0
    // *	STF	R0,*+AR1(Y)
    // *	LDF	*+AR4(OPOSZ),R0
    // *	STF	R0,*+AR1(Z)
    // *
    // *	CALL	DIST_PT2LINE
    // *
    // *PARAMETERS
    // *	AR0	LINE EQUATION (2D) A B C
    // *	AR1	2D POINT [X Y]
    // *
    // *RETURNS
    // *	R0	FL	DISTANCE TO LINE (SIGNED)
    // *
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R1
    // asm: 	PUSHF	R2
    // asm: 	MPYF	*+AR1(X),*+AR0(A),R0
    // asm: 	MPYF	*+AR1(Y),*+AR0(B),R1
    // asm: 	ADDF	R0,R1
    // asm: 	ADDF	*+AR0(C),R1
    // asm: 	MPYF	*+AR0(A),*+AR0(A),R0
    // asm: 	MPYF	*+AR0(B),*+AR0(B),R2
    // asm: 	ADDF	R0,R2
    // asm: 	CALL	SQRT
    // asm: 	LDF	R0,R2
    // asm: 	LDF	R1,R0
    // asm: 	LDF	R2,R1
    // asm: 	CALL	DIV_F
    // asm: 	POPF	R2
    // asm: 	POPF	R1
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIST_PT2LINE", 0, 0);
    UNIMPL();
}

void GETLINE_EQ_2D(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *	LDI	@VECTORAI,AR0
    // *	LDI	@VECTORBI,AR1
    // *	CALL	GETLINE_EQ
    // *
    // *PARAMETERS
    // *	AR0	P1
    // *	AR1	P2
    // *	AR2	SPACE FOR LINE EQUATION
    // *
    // *RETURNS
    // *	AR2	VECTOR AS A B C
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R1
    // asm: 	PUSHF	R2
    // asm: 	PUSHF	R3
    // asm: 	SUBF	*+AR1(X),*+AR0(X),R0
    // asm: 	SUBF	*+AR1(Y),*+AR0(Y),R1
    // asm: 	MPYF	R0,*+AR1(X),R2
    // asm: 	MPYF	R1,*+AR1(Y),R3
    // asm: 	ADDF	R3,R2
    // asm: 	NEGF	R2
    // asm: 	STF	R0,*+AR2(A)
    // asm: 	STF	R1,*+AR2(B)
    // asm: 	STF	R2,*+AR2(C)
    // asm: 	POPF	R3
    // asm: 	POPF	R2
    // asm: 	POPF	R1
    // asm: 	POPF	R0
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETLINE_EQ_2D", 0, 0);
    UNIMPL();
}

void SCALE_MATRIX(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *see Foley & van Damn 2ed pg215
    // *
    // *scale matrix	Sx  0   0
    // *		0   Sy  0
    // *		0   0   Sz
    // *
    // *Once a FIND_MATRIX is complete you can pump that matrix through this
    // *routine and the object will be scaled by R2.  (Obviously each of the
    // *X Y and Z elements can be scaled independantly, but this might be
    // *cumbersome, and it would be better suited for a different routine.
    // *
    // *Note that a 3x3 x 3x3 -> 3x3 matrix by the scale would be:
    // *
    // * ASx BSy CSz
    // * DSx ESy FSz  (by eliminating all the multiply by zeros)
    // * GSx HSy ISz
    // *
    // *PARAMETERS
    // *	AR2	MATRIX to be scaled
    // *	R2	SCALE FACTOR
    // *
    // *
    // *RETURNS
    // *	AR2	MATRIX modified for scaling
    // *
    // *
    // asm: 	PUSH	R3
    // asm: 	PUSHF	R3
    // asm: 	LDF	*+AR2(A00),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A00)
    // asm: 	LDF	*+AR2(A01),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A01)
    // asm: 	LDF	*+AR2(A02),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A02)
    // asm: 	LDF	*+AR2(A10),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A10)
    // asm: 	LDF	*+AR2(A11),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A11)
    // asm: 	LDF	*+AR2(A12),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A12)
    // asm: 	LDF	*+AR2(A20),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A20)
    // asm: 	LDF	*+AR2(A21),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A21)
    // asm: 	LDF	*+AR2(A22),R3
    // asm: 	MPYF	R2,R3
    // asm: 	STF	R3,*+AR2(A22)
    // asm: 	POPF	R3
    // asm: 	POP	R3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCALE_MATRIX", 0, 0);
    UNIMPL();
}

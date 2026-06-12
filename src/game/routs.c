#include "../core/cpu.h"
#include "../core/machine.h"

/*
 * Source module: asm/ROUTS.ASM
 */

void DIV_F(void);
void DIV_I(void);
void zero(void);
void DIV_U30(void);
void zerob(void);
void INV_F30(void);
void MOD_I30(void);
void MOD_U30(void);
void onec(void);
void zeroc(void);
void SQRT(void);

/* *----------------------------------------------------------------------------
*DIVF	Floating point divide function
*
*PARAMETERS	u in R0, v in R1
*	R0	FL u
*	R1	FL v
*RETURNS
*	R0	FL R0/R1
*STATUS		Set from result in R0
*OPERATION	Result = (1/v) * u.
*
*CLOBBERS	R0,R1,BK
*
*----------------------------------------------------------------------------
*	DIV_F - Floating point division
*
*	Algorithm:
*	Given v = a * 2**e
*	x[0] = 1.0 * 2**(-e-1)
*	for (i = 1; i <= 5; i++)
*	 x[i] = x[i-1] * (2.0 - v * x[i-1])
*
*	The single-precision floating-point format is accurate to 6.9
*	decimal places.	The single-precision format is accurate to
*	2**-23 = 1.192E-7, so we would like to have that much accuracy
*	in the final result.
*
*	The algorithm's error at an iteration i (e[i]) is defined as
*		e[i] = 1 - v * x[i]
*	It can also be shown that e[i+1] = e[i] * e[i].
*
*	Cycles: 40
*
 */
void DIV_F(void)
{
    // asm: DIV_F30
    // asm: 	POP	BK	;Pop return address
    // asm: 	PUSH	R2	;Save R2: integer part
    // asm: 	PUSHF	R2	;Save R2: floating point part
    // asm: 	PUSHF	R1	;SAVE THE SIGN
    // asm: 	PUSHF	R0	;Save u (dividend)
    // ;	LDI	R1,AR0	;Save mantissa of v to remember sign
    // asm: 	ABSF	R1	;The algorithm uses v = |v|.
    // 	;
    // 	;	Extract the exponent of v.
    // 	;
    // asm: 	PUSHF	R1
    // asm: 	POP	R2
    // asm: 	ASH	-24,R2	;The 8 LSBs of R2 contain the exponent of v.
    // ;
    // ;A few comments on boundary conditions.	If e = -128, then v = 0.  The
    // ;following x[0] calculation yields R2 = --128 - 1 = 127 and the algorithm will
    // ;overflow and saturate since x[0] is large.  This seems reasonable.  If e =
    // ;127, the R2 = -127 - 1 = -128.	Thus x[0] = 0 and this will cause the
    // ;algorithm to yield zero.  Since the mantissa of v is always between 1 and 2,
    // ;this is also reasonable.  As a result, boundary conditions are handled
    // ;automatically in a reasonable fashion.
    // ;
    // ;	x[0] formation given the exponent of v.
    // ;
    // asm: 	NEGI	R2
    // asm: 	SUBI	1,R2		;Now we have -e-1, the exponent of x[0].
    // asm: 	ASH	24,R2
    // asm: 	PUSH	R2
    // asm: 	POPF	R2		;Now R2 = x[0] = 1.0 * 2**(-e-1).
    // 	;
    // 	;Now the iterations begin.
    // 	;
    // asm: 	MPYF	R2,R1,R0	;R0 = v * x[0]
    // asm: 	SUBRF	2.0,R0		;R0 = 2.0 - v * x[0]
    // asm: 	MPYF	R0,R2		;R2 = x[1] = x[0] * (2.0 - v * x[0])
    // asm: 	MPYF	R2,R1,R0	;R0 = v * x[1]
    // asm: 	SUBRF	2.0,R0		;R0 = 2.0 - v * x[1]
    // asm: 	MPYF	R0,R2		;R2 = x[2] = x[1] * (2.0 - v * x[1])
    // asm: 	MPYF	R2,R1,R0	;R0 = v * x[2]
    // asm: 	SUBRF	2.0,R0		;R0 = 2.0 - v * x[2]
    // asm: 	MPYF	R0,R2		;R2 = x[3] = x[2] * (2.0 - v * x[2])
    // asm: 	MPYF	R2,R1,R0	;R0 = v * x[3]
    // asm: 	SUBRF	2.0,R0		;R0 = 2.0 - v * x[3]
    // asm: 	MPYF	R0,R2		;R2 = x[4] = x[3] * (2.0 - v * x[3])
    // asm: 	RND	R2		;This minimizes error in the LSBs.
    // 	;
    // 	;For the last iteration we use the formulation:
    // 	;x[5] = (x[4] * (1.0 - (v * x[4]))) + x[4]
    // 	;
    // asm: 	MPYF	R2,R1,R0	;R0 = v * x[4] = 1.0..01.. => 1
    // asm: 	SUBRF	1.0,R0		;R0 = 1.0 - v * x[4] = 0.0..01... => 0
    // asm: 	MPYF	R2,R0		;R0 = x[4] * (1.0 - v * x[4])
    // asm: 	ADDF	R0,R2,R1	;R0 = x[5] = (x[4]*(1.0-(v*x[4])))+x[4]
    // 	;
    // 	;R1 contains 1/v.	Multiply by u to get result.
    // 	;
    // asm: 	RND	R1		;Round since this is follow by a MPYF.
    // asm: 	POPF	R0		;Pop u
    // asm: 	MPYF	R1,R0		;Result = u * (1/v)
    // 	;
    // 	;Branch (delayed) return.	Use delay slots to negate the result if v < 0.
    // 	;
    // asm: 	NEGF	R0,R1		;R1 = -(1/|v|)
    // asm: 	POPF	R2		;CHECK ORIGINAL SIGN DUDES... (SETS SIGN FLAG)
    // asm: 	BD	BK		;Delayed branch to return
    // asm: 	LDFN	R1,R0		;If v < 0, then R1 = -R1 (BASED ON POPF R2)
    // asm: 	POPF	R2		;Restore R2: floating point part
    // asm: 	POP	R2		;Restore R2: integer part
    // 	;---->B	BK		;BRANCH OCCURS (RETURN)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIV_F", 0, 0);
    UNIMPL();
}

void DIV_I(void)
{
    // asm: DIV_I30
    // 	;
    // 	;Determine sign of result.	Get absolute value of operands.
    // 	;
    // asm: 	XOR	R0,R1,SIGN	;get the sign
    // asm: 	ABSI	R0		;make dividend positive
    // asm: 	BVD	div_32	 	;if still negative, escape
    // asm: 	ABSI	R1		;make divisor positive
    // asm: 	LDI	R1,V		;save in V
    // asm: 	CMPI	R0,V		;divisor > dividend ?
    // asm: 	BHID	zero		; if so, return 0
    // 	;
    // 	;Normalize operands.	Use difference in exponents as shift count
    // 	;for divisor, and as repeat count for SUBC.
    // 	;
    // asm: 	FLOAT	R1,TEMP		;normalize divisor
    // asm: 	PUSHF	TEMP		;push as float
    // asm: 	POP	EXP		;pop as int
    // asm: 	FLOAT	R0,TEMP		;normalize dividend
    // asm: 	PUSHF	TEMP		;push as float
    // asm: 	POP	COUNT		;pop as int
    // asm: 	LSH	-24,EXP		;divisor exponent
    // asm: 	LSH	-24,COUNT	;dividend exponent
    // asm: 	SUBI	EXP,COUNT	;get difference in exponents
    // asm: 	LSH	COUNT,V		;align divisor with dividend
    // 	;
    // 	;Do COUNT+1 subtract & shifts.
    // 	;
    // asm: 	RPTS	COUNT
    // asm: 	SUBC	V,R0
    // 	;
    // 	; Mask off the lower COUNT+1 bits of R0
    // 	;
    // asm: 	SUBRI	31,COUNT	;shift count is (32 - (COUNT+1))
    // asm: 	LSH	COUNT,R0	;shift left
    // asm: 	NEGI	COUNT
    // asm: 	LSH	COUNT,R0	;shift right to get result
    // 	;Check sign and negate result if necessary.
return:
    // asm: 	POP	RC		;return address
    // asm: 	NEGI	R0,TEMP		;negate result
    // asm: 	BD	RC		;delayed branch to return
    // asm: 	CMPI	0,SIGN		;check sign
    // asm: 	LDIN	TEMP,R0		;if set, use negative result
    // asm: 	CMPI	0,R0		;set status from result
    // 	;---->	B	RC		;BRANCH OCCURS (RETURN)
    // 	;The following code handles cases of a full 32-bit dividend.	This occurs
    // 	;when R0 = abs(R0) = 080000000h.	Handle this by calling the unsigned divide
    // 	;function, then negating the result if necessary.
div_32:
    // asm: 	PUSH	SIGN		;remember sign
    // asm: 	CALL	DIV_U30		;do divide
    // asm: 	POP	SIGN		;restore sign
    // asm: 	B	return		;return
    // 	;***Return zero.
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIV_I", 0, 0);
    UNIMPL();
}

void zero(void)
{
    // asm: 	LDI	0,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "zero", 0, 0);
    UNIMPL();
}

void DIV_U30(void)
{
    // asm: 	CMPI	R0,R1		;divisor > dividend ?
    // asm: 	BHI	zerob		;	if so, return 0
    // asm: 	LDI	R1,V		;move divisor to AR1
    // 	;
    // 	;If top bit of dividend is set, handle specially.
    // 	;
    // asm: 	CMPI	0,R0 		;check top bit
    // asm: 	BLTD	div_32b	 	;get divisor exponent, then jump.
    // 	;
    // 	;Get divisor exponent by converting to float.
    // 	;
    // asm: 	FLOAT	V,TEMP	 	;normalize divisor
    // asm: 	PUSHF	TEMP		;push as float
    // asm: 	POP	EXP		;pop as int to get exponent
    // 	;
    // 	;31 or less bits in dividend.	Get dividend exponent.
    // 	;
    // asm: 	FLOAT	R0,TEMP		;normalize dividend
    // asm: 	PUSHF	TEMP		;push as float
    // asm: 	POP	COUNT		;pop as int to get exponent
    // 	;
    // 	;Use difference in exponents as shift count to line up MSBs.
    // 	;
    // asm: 	LSH	-24,COUNT	;divisor exponent
    // asm: 	LSH	-24,EXP		;dividend exponent
    // asm: 	SUBI	EXP,COUNT	;difference
    // asm: 	LSH	COUNT,V		;shift divisor up
    // 	;
    // 	;Do COUNT+1 subtract & shifts.
    // 	;
    // asm: 	RPTS	COUNT
    // asm: 	SUBC	V,R0
    // 	;
    // 	; Mask off the lower COUNT+1 bits of U and return.
    // 	;
    // asm: 	POP	RC		;return address
    // asm: 	SUBRI	31,COUNT	;shift count is (32 - (COUNT+1))
    // asm: 	BD	RC		;delayed branch to return
    // asm: 	LSH	COUNT,R0	;shift left
    // asm: 	NEGI	COUNT
    // asm: 	LSH	COUNT,R0	;shift right to get result
    // 	;---->	B	RC		;BRANCH OCCURS (RETURN)
    // ;
    // ;The following code handles cases of a full 32-bit dividend.	Before
    // ;SUBC can be used, the top bit must be cleared (otherwise SUBC can
    // ;possibly shift a significant 1 out the top of the dividend).	This
    // ;is accomplished by first doing a normal subtraction, then proceeding
    // ;with SUBCs.
    // ;
div_32b:
    // ;
    // ;If the top bit of the divisor is set too, the quotient is 1.
    // ;Otherwise, shift the divisor up to line up the MSBs.
    // ;
    // asm: 	CMPI	0,V		;check divisor
    // asm: 	BLTD	one		;if top bit set, quotient is 1
    // asm: 	LSH	-24,EXP		;divisor exponent
    // asm: 	SUBRI	31,EXP		;shift count
    // asm: 	LSH	EXP,V		;shift up to line up MSBs
    // 	;
    // 	;Now MSBs are aligned.	Do first SUBC by hand, and save off the first
    // 	;quotient digit.	Then, shift divisor right rather than shifting dividend
    // 	;left.	This leaves a 0 in the top bit of the dividend.
    // 	;
    // asm: 	LDI	1,QMASK		;initialize MSB of quotient
    // asm: 	LSH	EXP,QMASK	;create a mask for the MSBs
    // asm: 	SUBI	1,QMASK		;mask is (2 << COUNT) - 1
    // asm: 	SUBI	V,R0,TEMP	;subtract
    // asm: 	LDIHS	TEMP,R0		;if positive, replace dividend
    // asm: 	LDIHS	1,MSBQ		;	and set quotient to 1
    // asm: 	LDILO	0,MSBQ		;if negative, set quotient to 0
    // asm: 	LSH	EXP,MSBQ	;shift MSB into position
    // asm: 	LSH	-1,V		;shift divisor down
    // asm: 	SUBI	1,EXP		;first iteration is done
    // 	;
    // 	;Do EXP subtract & shifts.
    // 	;
    // asm: 	RPTS	EXP
    // asm: 	SUBC	V,R0
    // 	;
    // 	;MSB of the quotient is in MSBQ.	LSBs are in the lower COUNT bits of
    // 	;R0.
    // 	;
    // asm: 	POP	RC		;return address
    // asm: 	BD	RC		;delayed branch to return
    // asm: 	AND	QMASK,R0	;mask off LSBs
    // asm: 	OR	MSBQ,R0		;MSB of quotient
    // asm: 	NOP
    // 	;---->	B	RC		;BRANCH OCCURS (RETURN)
    // 	;
    // 	; Return one.
    // 	;
one:
    // asm: LDI	1,R0
    // asm: 	RETS
    // 	;
    // 	; Return zero.
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIV_U30", 0, 0);
    UNIMPL();
}

void zerob(void)
{
    // asm: LDI	0,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "zerob", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*INVF	Floating point inverse function
*
*
*PARAMETERS		v in R0
*RETURNS		1/v in R0
*Status			Not set from result (!!!)
*Registers used		R0, R1, BK
*
*	Algorithm:
*	Given v = a * 2**e
*	x[0] = 1.0 * 2**(-e-1)
*	for (i = 1;i <= 5;i++)
*	 x[i] = x[i-1] * (2.0 - v * x[i-1])
*
*	The single-precision floating-point format is accurate to 6.9
*	Given v = a * 2**e
*	x[0] = 1.0 * 2**(-e-1)
*	for (i = 1;i <= 5;i++)
*	 x[i] = x[i-1] * (2.0 - v * x[i-1])
*
*	The single-precision floating-point format is accurate to 6.9
*	decimal places.	The single-precision format is accurate to
*	2**-23 = 1.192E-7, so we would like to have that much accuracy
*	in the final result.
*
*	The algorithm's error at an iteration i (e[i]) is defined as
*		e[i] = 1 - v * x[i]
*	It can also be shown that e[i+1] = e[i] * e[i].
*	Cycles: 36
*
 */
void INV_F30(void)
{
    // asm: 	POP	BK		;Pop return address
    // asm: 	PUSH	R2		;Save R2: integer part
    // asm: 	PUSHF	R2		;Save R2: floating point part
    // asm: 	PUSHF	R0
    // asm: 	ABSF	R0		;The algorithm uses v = |v|.
    // 	;
    // 	;	Extract the exponent of v.
    // 	;
    // asm: 	PUSHF	R0
    // asm: 	POP	R1
    // asm: 	ASH	-24,R1		;The 8 LSBs of R1 contain the exponent of v.
    // ;
    // ;A few comments on boundary conditions.	If e = -128, then v = 0.	The
    // ;following x[0] calculation yields R1 = --128 - 1 = 127 and the algorithm will
    // ;overflow and saturate since x[0] is large.	This seems reasonable.	If e =
    // ;127, the R1 = -127 - 1 = -128.	Thus x[0] = 0 and this will cause the
    // ;algorithm to yield zero.	Since the mantissa of v is always between 1 and 2,
    // ;this is also reasonable.	As a result, boundary conditions are handled
    // ;automatically in a reasonable fashion.
    // ;
    // ;	x[0] formation given the exponent of v.
    // ;
    // asm: 	NEGI	R1
    // asm: 	SUBI	1,R1		;Now we have -e-1, the exponent of x[0].
    // asm: 	ASH	24,R1
    // asm: 	PUSH	R1
    // asm: 	POPF	R1		;Now R1 = x[0] = 1.0 * 2**(-e-1).
    // 	;
    // 	;Now the iterations begin.
    // 	;
    // asm: 	MPYF	R1,R0,R2	;R2 = v * x[0]
    // asm: 	SUBRF	2.0,R2		;R2 = 2.0 - v * x[0]
    // asm: 	MPYF	R2,R1		;R1 = x[1] = x[0] * (2.0 - v * x[0])
    // asm: 	MPYF	R1,R0,R2	;R2 = v * x[1]
    // asm: 	SUBRF	2.0,R2		;R2 = 2.0 - v * x[1]
    // asm: 	MPYF	R2,R1		;R1 = x[2] = x[1] * (2.0 - v * x[1])
    // asm: 	MPYF	R1,R0,R2	;R2 = v * x[2]
    // asm: 	SUBRF	2.0,R2		;R2 = 2.0 - v * x[2]
    // asm: 	MPYF	R2,R1		;R1 = x[3] = x[2] * (2.0 - v * x[2])
    // asm: 	MPYF	R1,R0,R2	;R2 = v * x[3]
    // asm: 	SUBRF	2.0,R2		;R2 = 2.0 - v * x[3]
    // asm: 	MPYF	R2,R1		;R1 = x[4] = x[3] * (2.0 - v * x[3])
    // asm: 	RND	R1		;This minimizes error in the LSBs.
    // 	;
    // 	;For the last iteration we use the formulation:
    // 	;x[5] = (x[4] * (1.0 - (v * x[4]))) + x[4]
    // 	;
    // asm: 	MPYF	R1,R0,R2	;R2 = v * x[4] = 1.0..01.. => 1
    // asm: 	SUBRF	1.0,R2		;R2 = 1.0 - v * x[4] = 0.0..01... => 0
    // asm: 	MPYF	R1,R2		;R2 = x[4] * (1.0 - v * x[4])
    // asm: 	ADDF	R2,R1,R0	;R0 = x[5] = (x[4]*(1.0-(v*x[4])))+x[4]
    // 	;
    // 	;Return (delayed). Use delay slots to negate the result if v < 0.
    // 	;
    // asm: 	NEGF	R0,R1		;R1 = -(1/|v|)
    // asm: 	POPF	R2		;CHECK ORIGINAL SIGN DUDES... (SETS SIGN FLAG)
    // asm: 	BD	BK		;Delayed branch to return
    // asm: 	LDFN	R1,R0		;If v < 0, then R1 = -R1 (BASED ON POPF R2)
    // asm: 	POPF	R2		;Restore R2: floating point part
    // asm: 	POP	R2		;Restore R2: integer part
    // 	;---->B	BK		;BRANCH OCCURS (RETURN)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INV_F30", 0, 0);
    UNIMPL();
}

void MOD_I30(void)
{
    // 	;
    // 	;Determine sign of result.	Get absolute value of operands.
    // 	;
    // asm: 	LDI	R0,SIGN		;sign of result same as dividend
    // asm: 	ABSI	R0		;make dividend positive
    // asm: 	BVD	mod_32	 	;if still negative, escape
    // asm: 	ABSI	R1		;make divisor positive
    // asm: 	LDI	R1,V		;save in V
    // asm: 	CMPI	R0,V		;divisor > dividend ?
    // asm: 	BHID	returnc	 	; if so, return dividend
    // 	;
    // 	;Normalize operands.	Use difference in exponents as shift count
    // 	;for divisor, and as repeat count for SUBC.
    // 	;
    // asm: 	FLOAT	R1,TEMP		;normalize divisor
    // asm: 	PUSHF	TEMP		;push as float
    // asm: 	POP	EXP		;pop as int
    // asm: 	FLOAT	R0,TEMP		;normalize dividend
    // asm: 	PUSHF	TEMP		;push as float
    // asm: 	POP	COUNT		;pop as int
    // asm: 	LSH	-24,EXP		;get divisor exponent
    // asm: 	LSH	-24,COUNT	;get dividend exponent
    // asm: 	SUBI	EXP,COUNT	;get difference in exponents
    // asm: 	LSH	COUNT,V		;align divisor with dividend
    // 	;
    // 	;Do COUNT+1 subtract & shifts.
    // 	;
    // asm: 	RPTS	COUNT
    // asm: 	SUBC	V,R0
    // 	;
    // 	; Remainder is in upper bits of R0
    // 	;
    // asm: 	ADDI	1,COUNT		;shift count is -(COUNT+1)
    // asm: 	NEGI	COUNT
    // asm: 	LSH	COUNT,R0	;shift right
    // 	;
    // 	; Check sign and negate result if necessary.
    // 	;
returnc:
    // asm: 	POP	RC		;return address
    // asm: 	NEGI	R0,TEMP		;negate result
    // asm: 	BD	RC		;delayed branch to return
    // asm: 	CMPI	0,SIGN	 	;check sign
    // asm: 	LDIN	TEMP,R0		;if set, use negative result
    // asm: 	CMPI	0,R0		;set status on result
    // 	;---->	B	RC		;BRANCH OCCURS (RETURN)
    // ;
    // ;The following code handles cases of a full 32-bit dividend.	This occurs
    // ;when R0 = abs(R0) = 080000000h.	Handle this by calling the unsigned mod
    // ;function, then negating the result if necessary.
    // ;
mod_32:
    // asm: 	PUSH	SIGN		;remember sign
    // asm: 	CALL	MOD_U30		;do divide
    // asm: 	POP	SIGN		;restore sign
    // asm: 	B	return	 	;return
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOD_I30", 0, 0);
    UNIMPL();
}

void MOD_U30(void)
{
    // asm: 	CMPI	R0,R1		;divisor > dividend ?
    // asm: 	BHI	zerob		;	if so, return dividend
    // asm: 	LDI	R1,V		;load divisor
    // 	;
    // 	;If top bit of dividend is set, handle specially.
    // 	;
    // asm: 	CMPI	0,R0 		;check top bit
    // asm: 	BLTD	mod_32c	 	;get divisor exponent, then jump.
    // 	;
    // 	;Get divisor exponent by converting to float.
    // 	;
    // asm: 	FLOAT	V,TEMP	 	;normalize divisor
    // asm: 	PUSHF	TEMP		;push as float
    // asm: 	POP	EXP		;pop as int to get exponent
    // 	;
    // 	;31 or less bits in dividend.	Get dividend exponent.
    // 	;
    // asm: 	FLOAT	R0,TEMP		;normalize dividend
    // asm: 	PUSHF	TEMP		;push as float
    // asm: 	POP	COUNT		;pop as int to get exponent
    // 	;
    // 	;Use difference in exponents as shift count to line up MSBs.
    // 	;
    // asm: 	LSH	-24,EXP		;divisor exponent
    // asm: 	LSH	-24,COUNT	;dividend exponent
    // asm: 	SUBI	EXP,COUNT	;difference
    // asm: 	LSH	COUNT,V		;shift divisor up
    // 	;
    // 	;Do COUNT+1 subtract & shifts.
    // 	;
    // asm: 	RPTS	COUNT
    // asm: 	SUBC	V,R0
    // 	;
    // 	; Remainder is in upper 31-COUNT bits.
    // 	;
    // asm: 	POP	RC		;return address
    // asm: 	BD	RC		;delayed branch to return
    // asm: 	ADDI	1,COUNT		;shift count is COUNT+1
    // asm: 	NEGI	COUNT		;negate for right shift
    // asm: 	LSH	COUNT,R0	;shift to get result
    // 	;---->	B	RC	;BRANCH OCCURS (RETURN)
    // ;
    // ;The following code handles cases of a full 32-bit dividend.	Before
    // ;SUBC can be used, the top bit must be cleared (otherwise SUBC can
    // ;possibly shift a significant 1 out the top of the dividend).	This
    // ;is accomplished by first doing a normal subtraction, then proceeding
    // ;with SUBCs.
    // ;
mod_32c:
    // ;
    // ;If the top bit of the divisor is set too, the remainder is simply
    // ;the difference between the dividend and divisor.	Otherwise, shift
    // ;the divisor up to line up the MSBs.
    // ;
    // asm: 	CMPI	0,V		;check divisor
    // asm: 	BLTD	onec		;if negative, remainder is diff
    // asm: 	LSH	-24,EXP		;divisor exponent
    // asm: 	SUBRI	31,EXP	 	;shift count = 31 - exp
    // asm: 	NEGI	EXP,COUNT	;used later as shift count
    // asm: 	LSH	EXP,V		;shift up to line up MSBs
    // 	;
    // 	;Now MSBs are aligned.	Do first SUBC by hand using a plain subtraction.
    // 	;Then, shift divisor right rather than shifting dividend left.	This leaves
    // 	;a 0 in the top bit of the dividend.
    // 	;
    // asm: 	SUBI	V,R0,TEMP	;subtract
    // asm: 	LDIHS	TEMP,R0		;if positive, replace dividend
    // asm: 	SUBI	1,EXP		;first iteration is done
    // asm: 	LSH	-1,V		;shift divisor down
    // 	;
    // 	;Do EXP subtract & shifts.
    // 	;
    // asm: 	RPTS	EXP
    // asm: 	SUBC	V,R0
    // 	;
    // 	;Quotient is in EXP+1 LSBs;shift remainder (in MSBs) down.
    // 	;
    // asm: 	LSH	COUNT,R0	;COUNT contains -(EXP+1)
    // asm: 	RETS
    // 	;
    // 	; Return (dividend - divisor).
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOD_U30", 0, 0);
    UNIMPL();
}

void onec(void)
{
    // asm: SUBI	R1,R0
    // asm: 	RETS
    // 	;
    // 	; Return dividend.
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "onec", 0, 0);
    UNIMPL();
}

void zeroc(void)
{
    // asm: CMPI	0,R0		;set status from result
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "zeroc", 0, 0);
    UNIMPL();
}

void SQRT(void)
{
    // asm: 	LDF	R2,R0
    // asm: 	RETSLE			;return the value if <= 0
    // asm: 	PUSH	R1
    // asm: 	PUSHF	R1
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R2
    // asm: 	MPYF	2.0,R2		;add a rounding bit in exponent
    // asm: 	PUSHF	R2		;push x as float
    // asm: 	POP	R1		;pop as int
    // asm: 	ASH	-25,R1		;e = exponent(x) / 2
    // 	;
    // 	;determine initial estimate .25 * 2**(-e/2)
    // 	;
    // asm: 	NEGI	R1		;negate exponent
    // asm: 	ASH	24,R1	 	;shift into place
    // asm: 	PUSH	R1		;push as int
    // asm: 	POPF	R1		;pop as float
    // asm: 	MPYF	0.25,R2		;remove rounding bit
    // 	;
    // 	;iterate 5 times
    // 	;
    // asm: 	MPYF	R1,R1,R0	;R0 = x[0] * x[0]
    // asm: 	MPYF	R2,R0	 	;R0 = (v/2) * x[0] * x[0]
    // asm: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[0] * x[0]
    // asm: 	MPYF	R0,R1		;x[1] = x[0] * (1.5 - v/2 * x[0] * x[0])
    // 	;2
    // asm: 	RND	R1
    // asm: 	MPYF	R1,R1,R0	;R0 = x[1] * x[1]
    // asm: 	MPYF	R2,R0	 	;R0 = (v/2) * x[1] * x[1]
    // asm: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[1] * x[1]
    // asm: 	MPYF	R0,R1		;x[2] = x[1] * (1.5 - v/2 * x[1] * x[1])
    // 	;3
    // asm: 	RND	R1
    // asm: 	MPYF	R1,R1,R0	;R0 = x[2] * x[2]
    // asm: 	MPYF	R2,R0	 	;R0 = (v/2) * x[2] * x[2]
    // asm: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[2] * x[2]
    // asm: 	MPYF	R0,R1		;x[3] = x[2] * (1.5 - v/2 * x[2] * x[2])
    // 	;4
    // asm: 	RND	R1
    // asm: 	MPYF	R1,R1,R0	;R0 = x[3] * x[3]
    // asm: 	MPYF	R2,R0	 	;R0 = (v/2) * x[3] * x[3]
    // asm: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[3] * x[3]
    // asm: 	MPYF	R0,R1		;x[4] = x[3] * (1.5 - v/2 * x[3] * x[3])
    // 	;5
    // asm: 	RND	R1
    // asm: 	MPYF	R1,R1,R0	;R0 = x[4] * x[4]
    // asm: 	MPYF	R2,R0	 	;R0 = (v/2) * x[4] * x[4]
    // asm: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[4] * x[4]
    // asm: 	MPYF	R0,R1		;x[5] = x[4] * (1.5 - v/2 * x[4] * x[4])
    // asm: 	RND	R1
    // asm: 	POPF	R2
    // asm: 	POP	R2
    // asm: 	MPYF	R2,R1,R0	;sqrt(x) = x * sqrt(1/x)
    // asm: 	POPF	R1
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SQRT", 0, 0);
    UNIMPL();
}

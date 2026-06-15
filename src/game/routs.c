#include "../core/cpu.h"
#include "../core/machine.h"

/*
 * Source module: asm/ROUTS.ASM
 */

#define DIV_F DIV_F30
void DIV_F30(void);
#define DIV_I DIV_I30
void DIV_I30(void);
void DIV_U30(void);
void INV_F30(void);
void MOD_I30(void);
void MOD_U30(void);
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
void DIV_F30(void)
{
    // asm 0000A534: 	POP	BK	;Pop return address
    // asm 0000A535: 	PUSH	R2	;Save R2: integer part
    // asm 0000A536: 	PUSHF	R2	;Save R2: floating point part
    // asm 0000A537: 	PUSHF	R1	;SAVE THE SIGN
    // asm 0000A538: 	PUSHF	R0	;Save u (dividend)
    // ;	LDI	R1,AR0	;Save mantissa of v to remember sign
    // asm 0000A539: 	ABSF	R1	;The algorithm uses v = |v|.
    // 	;
    // 	;	Extract the exponent of v.
    // 	;
    // asm 0000A53A: 	PUSHF	R1
    // asm 0000A53B: 	POP	R2
    // asm 0000A53C: 	ASH	-24,R2	;The 8 LSBs of R2 contain the exponent of v.
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
    // asm 0000A53D: 	NEGI	R2
    // asm 0000A53E: 	SUBI	1,R2		;Now we have -e-1, the exponent of x[0].
    // asm 0000A53F: 	ASH	24,R2
    // asm 0000A540: 	PUSH	R2
    // asm 0000A541: 	POPF	R2		;Now R2 = x[0] = 1.0 * 2**(-e-1).
    // 	;
    // 	;Now the iterations begin.
    // 	;
    // asm 0000A542: 	MPYF	R2,R1,R0	;R0 = v * x[0]
    // asm 0000A543: 	SUBRF	2.0,R0		;R0 = 2.0 - v * x[0]
    // asm 0000A544: 	MPYF	R0,R2		;R2 = x[1] = x[0] * (2.0 - v * x[0])
    // asm 0000A545: 	MPYF	R2,R1,R0	;R0 = v * x[1]
    // asm 0000A546: 	SUBRF	2.0,R0		;R0 = 2.0 - v * x[1]
    // asm 0000A547: 	MPYF	R0,R2		;R2 = x[2] = x[1] * (2.0 - v * x[1])
    // asm 0000A548: 	MPYF	R2,R1,R0	;R0 = v * x[2]
    // asm 0000A549: 	SUBRF	2.0,R0		;R0 = 2.0 - v * x[2]
    // asm 0000A54A: 	MPYF	R0,R2		;R2 = x[3] = x[2] * (2.0 - v * x[2])
    // asm 0000A54B: 	MPYF	R2,R1,R0	;R0 = v * x[3]
    // asm 0000A54C: 	SUBRF	2.0,R0		;R0 = 2.0 - v * x[3]
    // asm 0000A54D: 	MPYF	R0,R2		;R2 = x[4] = x[3] * (2.0 - v * x[3])
    // asm 0000A54E: 	RND	R2		;This minimizes error in the LSBs.
    // 	;
    // 	;For the last iteration we use the formulation:
    // 	;x[5] = (x[4] * (1.0 - (v * x[4]))) + x[4]
    // 	;
    // asm 0000A54F: 	MPYF	R2,R1,R0	;R0 = v * x[4] = 1.0..01.. => 1
    // asm 0000A550: 	SUBRF	1.0,R0		;R0 = 1.0 - v * x[4] = 0.0..01... => 0
    // asm 0000A551: 	MPYF	R2,R0		;R0 = x[4] * (1.0 - v * x[4])
    // asm 0000A552: 	ADDF	R0,R2,R1	;R0 = x[5] = (x[4]*(1.0-(v*x[4])))+x[4]
    // 	;
    // 	;R1 contains 1/v.	Multiply by u to get result.
    // 	;
    // asm 0000A553: 	RND	R1		;Round since this is follow by a MPYF.
    // asm 0000A554: 	POPF	R0		;Pop u
    // asm 0000A555: 	MPYF	R1,R0		;Result = u * (1/v)
    // 	;
    // 	;Branch (delayed) return.	Use delay slots to negate the result if v < 0.
    // 	;
    // asm 0000A556: 	NEGF	R0,R1		;R1 = -(1/|v|)
    // asm 0000A557: 	POPF	R2		;CHECK ORIGINAL SIGN DUDES... (SETS SIGN FLAG)
    // asm 0000A558: 	BD	BK		;Delayed branch to return
    // asm 0000A559: 	LDFN	R1,R0		;If v < 0, then R1 = -R1 (BASED ON POPF R2)
    // asm 0000A55A: 	POPF	R2		;Restore R2: floating point part
    // asm 0000A55B: 	POP	R2		;Restore R2: integer part
    // 	;---->B	BK		;BRANCH OCCURS (RETURN)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIV_F30", 0, 0);
    UNIMPL();
}

void DIV_I30(void)
{
    // 	;
    // 	;Determine sign of result.	Get absolute value of operands.
    // 	;
    // asm 0000A55C: 	XOR	R0,R1,SIGN	;get the sign
    // asm 0000A55D: 	ABSI	R0		;make dividend positive
    // asm 0000A55E: 	BVD	div_32	 	;if still negative, escape
    // asm 0000A55F: 	ABSI	R1		;make divisor positive
    // asm 0000A560: 	LDI	R1,V		;save in V
    // asm 0000A561: 	CMPI	R0,V		;divisor > dividend ?
    // asm 0000A562: 	BHID	zero		; if so, return 0
    // 	;
    // 	;Normalize operands.	Use difference in exponents as shift count
    // 	;for divisor, and as repeat count for SUBC.
    // 	;
    // asm 0000A563: 	FLOAT	R1,TEMP		;normalize divisor
    // asm 0000A564: 	PUSHF	TEMP		;push as float
    // asm 0000A565: 	POP	EXP		;pop as int
    // asm 0000A566: 	FLOAT	R0,TEMP		;normalize dividend
    // asm 0000A567: 	PUSHF	TEMP		;push as float
    // asm 0000A568: 	POP	COUNT		;pop as int
    // asm 0000A569: 	LSH	-24,EXP		;divisor exponent
    // asm 0000A56A: 	LSH	-24,COUNT	;dividend exponent
    // asm 0000A56B: 	SUBI	EXP,COUNT	;get difference in exponents
    // asm 0000A56C: 	LSH	COUNT,V		;align divisor with dividend
    // 	;
    // 	;Do COUNT+1 subtract & shifts.
    // 	;
    // asm 0000A56D: 	RPTS	COUNT
    // asm 0000A56E: 	SUBC	V,R0
    // 	;
    // 	; Mask off the lower COUNT+1 bits of R0
    // 	;
    // asm 0000A56F: 	SUBRI	31,COUNT	;shift count is (32 - (COUNT+1))
    // asm 0000A570: 	LSH	COUNT,R0	;shift left
    // asm 0000A571: 	NEGI	COUNT
    // asm 0000A572: 	LSH	COUNT,R0	;shift right to get result
    // 	;Check sign and negate result if necessary.
    // asm 0000A573: return
    // asm 0000A573: 	POP	RC		;return address
    // asm 0000A574: 	NEGI	R0,TEMP		;negate result
    // asm 0000A575: 	BD	RC		;delayed branch to return
    // asm 0000A576: 	CMPI	0,SIGN		;check sign
    // asm 0000A577: 	LDIN	TEMP,R0		;if set, use negative result
    // asm 0000A578: 	CMPI	0,R0		;set status from result
    // 	;---->	B	RC		;BRANCH OCCURS (RETURN)
    // 	;The following code handles cases of a full 32-bit dividend.	This occurs
    // 	;when R0 = abs(R0) = 080000000h.	Handle this by calling the unsigned divide
    // 	;function, then negating the result if necessary.
div_32:
    // asm 0000A579: 	PUSH	SIGN		;remember sign
    // asm 0000A57A: 	CALL	DIV_U30		;do divide
    // asm 0000A57B: 	POP	SIGN		;restore sign
    // asm 0000A57C: 	B	return		;return
    // 	;***Return zero.
zero:
    // asm 0000A57D: 	LDI	0,R0
    // asm 0000A57E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIV_I30", 0, 0);
    UNIMPL();
}

void DIV_U30(void)
{
    // asm 0000A57F: 	CMPI	R0,R1		;divisor > dividend ?
    // asm 0000A580: 	BHI	zerob		;	if so, return 0
    // asm 0000A581: 	LDI	R1,V		;move divisor to AR1
    // 	;
    // 	;If top bit of dividend is set, handle specially.
    // 	;
    // asm 0000A582: 	CMPI	0,R0 		;check top bit
    // asm 0000A583: 	BLTD	div_32b	 	;get divisor exponent, then jump.
    // 	;
    // 	;Get divisor exponent by converting to float.
    // 	;
    // asm 0000A584: 	FLOAT	V,TEMP	 	;normalize divisor
    // asm 0000A585: 	PUSHF	TEMP		;push as float
    // asm 0000A586: 	POP	EXP		;pop as int to get exponent
    // 	;
    // 	;31 or less bits in dividend.	Get dividend exponent.
    // 	;
    // asm 0000A587: 	FLOAT	R0,TEMP		;normalize dividend
    // asm 0000A588: 	PUSHF	TEMP		;push as float
    // asm 0000A589: 	POP	COUNT		;pop as int to get exponent
    // 	;
    // 	;Use difference in exponents as shift count to line up MSBs.
    // 	;
    // asm 0000A58A: 	LSH	-24,COUNT	;divisor exponent
    // asm 0000A58B: 	LSH	-24,EXP		;dividend exponent
    // asm 0000A58C: 	SUBI	EXP,COUNT	;difference
    // asm 0000A58D: 	LSH	COUNT,V		;shift divisor up
    // 	;
    // 	;Do COUNT+1 subtract & shifts.
    // 	;
    // asm 0000A58E: 	RPTS	COUNT
    // asm 0000A58F: 	SUBC	V,R0
    // 	;
    // 	; Mask off the lower COUNT+1 bits of U and return.
    // 	;
    // asm 0000A590: 	POP	RC		;return address
    // asm 0000A591: 	SUBRI	31,COUNT	;shift count is (32 - (COUNT+1))
    // asm 0000A592: 	BD	RC		;delayed branch to return
    // asm 0000A593: 	LSH	COUNT,R0	;shift left
    // asm 0000A594: 	NEGI	COUNT
    // asm 0000A595: 	LSH	COUNT,R0	;shift right to get result
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
    // asm 0000A596: 	CMPI	0,V		;check divisor
    // asm 0000A597: 	BLTD	one		;if top bit set, quotient is 1
    // asm 0000A598: 	LSH	-24,EXP		;divisor exponent
    // asm 0000A599: 	SUBRI	31,EXP		;shift count
    // asm 0000A59A: 	LSH	EXP,V		;shift up to line up MSBs
    // 	;
    // 	;Now MSBs are aligned.	Do first SUBC by hand, and save off the first
    // 	;quotient digit.	Then, shift divisor right rather than shifting dividend
    // 	;left.	This leaves a 0 in the top bit of the dividend.
    // 	;
    // asm 0000A59B: 	LDI	1,QMASK		;initialize MSB of quotient
    // asm 0000A59C: 	LSH	EXP,QMASK	;create a mask for the MSBs
    // asm 0000A59D: 	SUBI	1,QMASK		;mask is (2 << COUNT) - 1
    // asm 0000A59E: 	SUBI	V,R0,TEMP	;subtract
    // asm 0000A59F: 	LDIHS	TEMP,R0		;if positive, replace dividend
    // asm 0000A5A0: 	LDIHS	1,MSBQ		;	and set quotient to 1
    // asm 0000A5A1: 	LDILO	0,MSBQ		;if negative, set quotient to 0
    // asm 0000A5A2: 	LSH	EXP,MSBQ	;shift MSB into position
    // asm 0000A5A3: 	LSH	-1,V		;shift divisor down
    // asm 0000A5A4: 	SUBI	1,EXP		;first iteration is done
    // 	;
    // 	;Do EXP subtract & shifts.
    // 	;
    // asm 0000A5A5: 	RPTS	EXP
    // asm 0000A5A6: 	SUBC	V,R0
    // 	;
    // 	;MSB of the quotient is in MSBQ.	LSBs are in the lower COUNT bits of
    // 	;R0.
    // 	;
    // asm 0000A5A7: 	POP	RC		;return address
    // asm 0000A5A8: 	BD	RC		;delayed branch to return
    // asm 0000A5A9: 	AND	QMASK,R0	;mask off LSBs
    // asm 0000A5AA: 	OR	MSBQ,R0		;MSB of quotient
    // asm 0000A5AB: 	NOP
    // 	;---->	B	RC		;BRANCH OCCURS (RETURN)
    // 	;
    // 	; Return one.
    // 	;
one:
    // asm 0000A5AC: LDI	1,R0
    // asm 0000A5AD: 	RETS
    // 	;
    // 	; Return zero.
    // 	;
zerob:
    // asm 0000A5AE: LDI	0,R0
    // asm 0000A5AF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIV_U30", 0, 0);
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
    // asm 0000A5B0: 	POP	BK		;Pop return address
    // asm 0000A5B1: 	PUSH	R2		;Save R2: integer part
    // asm 0000A5B2: 	PUSHF	R2		;Save R2: floating point part
    // asm 0000A5B3: 	PUSHF	R0
    // asm 0000A5B4: 	ABSF	R0		;The algorithm uses v = |v|.
    // 	;
    // 	;	Extract the exponent of v.
    // 	;
    // asm 0000A5B5: 	PUSHF	R0
    // asm 0000A5B6: 	POP	R1
    // asm 0000A5B7: 	ASH	-24,R1		;The 8 LSBs of R1 contain the exponent of v.
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
    // asm 0000A5B8: 	NEGI	R1
    // asm 0000A5B9: 	SUBI	1,R1		;Now we have -e-1, the exponent of x[0].
    // asm 0000A5BA: 	ASH	24,R1
    // asm 0000A5BB: 	PUSH	R1
    // asm 0000A5BC: 	POPF	R1		;Now R1 = x[0] = 1.0 * 2**(-e-1).
    // 	;
    // 	;Now the iterations begin.
    // 	;
    // asm 0000A5BD: 	MPYF	R1,R0,R2	;R2 = v * x[0]
    // asm 0000A5BE: 	SUBRF	2.0,R2		;R2 = 2.0 - v * x[0]
    // asm 0000A5BF: 	MPYF	R2,R1		;R1 = x[1] = x[0] * (2.0 - v * x[0])
    // asm 0000A5C0: 	MPYF	R1,R0,R2	;R2 = v * x[1]
    // asm 0000A5C1: 	SUBRF	2.0,R2		;R2 = 2.0 - v * x[1]
    // asm 0000A5C2: 	MPYF	R2,R1		;R1 = x[2] = x[1] * (2.0 - v * x[1])
    // asm 0000A5C3: 	MPYF	R1,R0,R2	;R2 = v * x[2]
    // asm 0000A5C4: 	SUBRF	2.0,R2		;R2 = 2.0 - v * x[2]
    // asm 0000A5C5: 	MPYF	R2,R1		;R1 = x[3] = x[2] * (2.0 - v * x[2])
    // asm 0000A5C6: 	MPYF	R1,R0,R2	;R2 = v * x[3]
    // asm 0000A5C7: 	SUBRF	2.0,R2		;R2 = 2.0 - v * x[3]
    // asm 0000A5C8: 	MPYF	R2,R1		;R1 = x[4] = x[3] * (2.0 - v * x[3])
    // asm 0000A5C9: 	RND	R1		;This minimizes error in the LSBs.
    // 	;
    // 	;For the last iteration we use the formulation:
    // 	;x[5] = (x[4] * (1.0 - (v * x[4]))) + x[4]
    // 	;
    // asm 0000A5CA: 	MPYF	R1,R0,R2	;R2 = v * x[4] = 1.0..01.. => 1
    // asm 0000A5CB: 	SUBRF	1.0,R2		;R2 = 1.0 - v * x[4] = 0.0..01... => 0
    // asm 0000A5CC: 	MPYF	R1,R2		;R2 = x[4] * (1.0 - v * x[4])
    // asm 0000A5CD: 	ADDF	R2,R1,R0	;R0 = x[5] = (x[4]*(1.0-(v*x[4])))+x[4]
    // 	;
    // 	;Return (delayed). Use delay slots to negate the result if v < 0.
    // 	;
    // asm 0000A5CE: 	NEGF	R0,R1		;R1 = -(1/|v|)
    // asm 0000A5CF: 	POPF	R2		;CHECK ORIGINAL SIGN DUDES... (SETS SIGN FLAG)
    // asm 0000A5D0: 	BD	BK		;Delayed branch to return
    // asm 0000A5D1: 	LDFN	R1,R0		;If v < 0, then R1 = -R1 (BASED ON POPF R2)
    // asm 0000A5D2: 	POPF	R2		;Restore R2: floating point part
    // asm 0000A5D3: 	POP	R2		;Restore R2: integer part
    // 	;---->B	BK		;BRANCH OCCURS (RETURN)
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INV_F30", 0, 0);
    UNIMPL();
}

void MOD_I30(void)
{
    // 	;
    // 	;Determine sign of result.	Get absolute value of operands.
    // 	;
    // asm 0000A5D4: 	LDI	R0,SIGN		;sign of result same as dividend
    // asm 0000A5D5: 	ABSI	R0		;make dividend positive
    // asm 0000A5D6: 	BVD	mod_32	 	;if still negative, escape
    // asm 0000A5D7: 	ABSI	R1		;make divisor positive
    // asm 0000A5D8: 	LDI	R1,V		;save in V
    // asm 0000A5D9: 	CMPI	R0,V		;divisor > dividend ?
    // asm 0000A5DA: 	BHID	returnc	 	; if so, return dividend
    // 	;
    // 	;Normalize operands.	Use difference in exponents as shift count
    // 	;for divisor, and as repeat count for SUBC.
    // 	;
    // asm 0000A5DB: 	FLOAT	R1,TEMP		;normalize divisor
    // asm 0000A5DC: 	PUSHF	TEMP		;push as float
    // asm 0000A5DD: 	POP	EXP		;pop as int
    // asm 0000A5DE: 	FLOAT	R0,TEMP		;normalize dividend
    // asm 0000A5DF: 	PUSHF	TEMP		;push as float
    // asm 0000A5E0: 	POP	COUNT		;pop as int
    // asm 0000A5E1: 	LSH	-24,EXP		;get divisor exponent
    // asm 0000A5E2: 	LSH	-24,COUNT	;get dividend exponent
    // asm 0000A5E3: 	SUBI	EXP,COUNT	;get difference in exponents
    // asm 0000A5E4: 	LSH	COUNT,V		;align divisor with dividend
    // 	;
    // 	;Do COUNT+1 subtract & shifts.
    // 	;
    // asm 0000A5E5: 	RPTS	COUNT
    // asm 0000A5E6: 	SUBC	V,R0
    // 	;
    // 	; Remainder is in upper bits of R0
    // 	;
    // asm 0000A5E7: 	ADDI	1,COUNT		;shift count is -(COUNT+1)
    // asm 0000A5E8: 	NEGI	COUNT
    // asm 0000A5E9: 	LSH	COUNT,R0	;shift right
    // 	;
    // 	; Check sign and negate result if necessary.
    // 	;
    // asm 0000A5EA: returnc
    // asm 0000A5EA: 	POP	RC		;return address
    // asm 0000A5EB: 	NEGI	R0,TEMP		;negate result
    // asm 0000A5EC: 	BD	RC		;delayed branch to return
    // asm 0000A5ED: 	CMPI	0,SIGN	 	;check sign
    // asm 0000A5EE: 	LDIN	TEMP,R0		;if set, use negative result
    // asm 0000A5EF: 	CMPI	0,R0		;set status on result
    // 	;---->	B	RC		;BRANCH OCCURS (RETURN)
    // ;
    // ;The following code handles cases of a full 32-bit dividend.	This occurs
    // ;when R0 = abs(R0) = 080000000h.	Handle this by calling the unsigned mod
    // ;function, then negating the result if necessary.
    // ;
mod_32:
    // asm 0000A5F0: 	PUSH	SIGN		;remember sign
    // asm 0000A5F1: 	CALL	MOD_U30		;do divide
    // asm 0000A5F2: 	POP	SIGN		;restore sign
    // asm 0000A5F3: 	B	return	 	;return
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOD_I30", 0, 0);
    UNIMPL();
}

void MOD_U30(void)
{
    // asm 0000A5F4: 	CMPI	R0,R1		;divisor > dividend ?
    // asm 0000A5F5: 	BHI	zerob		;	if so, return dividend
    // asm 0000A5F6: 	LDI	R1,V		;load divisor
    // 	;
    // 	;If top bit of dividend is set, handle specially.
    // 	;
    // asm 0000A5F7: 	CMPI	0,R0 		;check top bit
    // asm 0000A5F8: 	BLTD	mod_32c	 	;get divisor exponent, then jump.
    // 	;
    // 	;Get divisor exponent by converting to float.
    // 	;
    // asm 0000A5F9: 	FLOAT	V,TEMP	 	;normalize divisor
    // asm 0000A5FA: 	PUSHF	TEMP		;push as float
    // asm 0000A5FB: 	POP	EXP		;pop as int to get exponent
    // 	;
    // 	;31 or less bits in dividend.	Get dividend exponent.
    // 	;
    // asm 0000A5FC: 	FLOAT	R0,TEMP		;normalize dividend
    // asm 0000A5FD: 	PUSHF	TEMP		;push as float
    // asm 0000A5FE: 	POP	COUNT		;pop as int to get exponent
    // 	;
    // 	;Use difference in exponents as shift count to line up MSBs.
    // 	;
    // asm 0000A5FF: 	LSH	-24,EXP		;divisor exponent
    // asm 0000A600: 	LSH	-24,COUNT	;dividend exponent
    // asm 0000A601: 	SUBI	EXP,COUNT	;difference
    // asm 0000A602: 	LSH	COUNT,V		;shift divisor up
    // 	;
    // 	;Do COUNT+1 subtract & shifts.
    // 	;
    // asm 0000A603: 	RPTS	COUNT
    // asm 0000A604: 	SUBC	V,R0
    // 	;
    // 	; Remainder is in upper 31-COUNT bits.
    // 	;
    // asm 0000A605: 	POP	RC		;return address
    // asm 0000A606: 	BD	RC		;delayed branch to return
    // asm 0000A607: 	ADDI	1,COUNT		;shift count is COUNT+1
    // asm 0000A608: 	NEGI	COUNT		;negate for right shift
    // asm 0000A609: 	LSH	COUNT,R0	;shift to get result
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
    // asm 0000A60A: 	CMPI	0,V		;check divisor
    // asm 0000A60B: 	BLTD	onec		;if negative, remainder is diff
    // asm 0000A60C: 	LSH	-24,EXP		;divisor exponent
    // asm 0000A60D: 	SUBRI	31,EXP	 	;shift count = 31 - exp
    // asm 0000A60E: 	NEGI	EXP,COUNT	;used later as shift count
    // asm 0000A60F: 	LSH	EXP,V		;shift up to line up MSBs
    // 	;
    // 	;Now MSBs are aligned.	Do first SUBC by hand using a plain subtraction.
    // 	;Then, shift divisor right rather than shifting dividend left.	This leaves
    // 	;a 0 in the top bit of the dividend.
    // 	;
    // asm 0000A610: 	SUBI	V,R0,TEMP	;subtract
    // asm 0000A611: 	LDIHS	TEMP,R0		;if positive, replace dividend
    // asm 0000A612: 	SUBI	1,EXP		;first iteration is done
    // asm 0000A613: 	LSH	-1,V		;shift divisor down
    // 	;
    // 	;Do EXP subtract & shifts.
    // 	;
    // asm 0000A614: 	RPTS	EXP
    // asm 0000A615: 	SUBC	V,R0
    // 	;
    // 	;Quotient is in EXP+1 LSBs;shift remainder (in MSBs) down.
    // 	;
    // asm 0000A616: 	LSH	COUNT,R0	;COUNT contains -(EXP+1)
    // asm 0000A617: 	RETS
    // 	;
    // 	; Return (dividend - divisor).
    // 	;
onec:
    // asm 0000A618: SUBI	R1,R0
    // asm 0000A619: 	RETS
    // 	;
    // 	; Return dividend.
    // 	;
zeroc:
    // asm 0000A61A: CMPI	0,R0
    // asm 0000A61B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOD_U30", 0, 0);
    UNIMPL();
}

void SQRT(void)
{
    // asm 0000A61C: 	LDF	R2,R0
    // asm 0000A61D: 	RETSLE			;return the value if <= 0
    // asm 0000A61E: 	PUSH	R1
    // asm 0000A61F: 	PUSHF	R1
    // asm 0000A620: 	PUSH	R2
    // asm 0000A621: 	PUSHF	R2
    // asm 0000A622: 	MPYF	2.0,R2		;add a rounding bit in exponent
    // asm 0000A623: 	PUSHF	R2		;push x as float
    // asm 0000A624: 	POP	R1		;pop as int
    // asm 0000A625: 	ASH	-25,R1		;e = exponent(x) / 2
    // 	;
    // 	;determine initial estimate .25 * 2**(-e/2)
    // 	;
    // asm 0000A626: 	NEGI	R1		;negate exponent
    // asm 0000A627: 	ASH	24,R1	 	;shift into place
    // asm 0000A628: 	PUSH	R1		;push as int
    // asm 0000A629: 	POPF	R1		;pop as float
    // asm 0000A62A: 	MPYF	0.25,R2		;remove rounding bit
    // 	;
    // 	;iterate 5 times
    // 	;
    // asm 0000A62B: 	MPYF	R1,R1,R0	;R0 = x[0] * x[0]
    // asm 0000A62C: 	MPYF	R2,R0	 	;R0 = (v/2) * x[0] * x[0]
    // asm 0000A62D: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[0] * x[0]
    // asm 0000A62E: 	MPYF	R0,R1		;x[1] = x[0] * (1.5 - v/2 * x[0] * x[0])
    // 	;2
    // asm 0000A62F: 	RND	R1
    // asm 0000A630: 	MPYF	R1,R1,R0	;R0 = x[1] * x[1]
    // asm 0000A631: 	MPYF	R2,R0	 	;R0 = (v/2) * x[1] * x[1]
    // asm 0000A632: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[1] * x[1]
    // asm 0000A633: 	MPYF	R0,R1		;x[2] = x[1] * (1.5 - v/2 * x[1] * x[1])
    // 	;3
    // asm 0000A634: 	RND	R1
    // asm 0000A635: 	MPYF	R1,R1,R0	;R0 = x[2] * x[2]
    // asm 0000A636: 	MPYF	R2,R0	 	;R0 = (v/2) * x[2] * x[2]
    // asm 0000A637: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[2] * x[2]
    // asm 0000A638: 	MPYF	R0,R1		;x[3] = x[2] * (1.5 - v/2 * x[2] * x[2])
    // 	;4
    // asm 0000A639: 	RND	R1
    // asm 0000A63A: 	MPYF	R1,R1,R0	;R0 = x[3] * x[3]
    // asm 0000A63B: 	MPYF	R2,R0	 	;R0 = (v/2) * x[3] * x[3]
    // asm 0000A63C: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[3] * x[3]
    // asm 0000A63D: 	MPYF	R0,R1		;x[4] = x[3] * (1.5 - v/2 * x[3] * x[3])
    // 	;5
    // asm 0000A63E: 	RND	R1
    // asm 0000A63F: 	MPYF	R1,R1,R0	;R0 = x[4] * x[4]
    // asm 0000A640: 	MPYF	R2,R0	 	;R0 = (v/2) * x[4] * x[4]
    // asm 0000A641: 	SUBRF	1.5,R0		;R0 = 1.5 - (v/2) * x[4] * x[4]
    // asm 0000A642: 	MPYF	R0,R1		;x[5] = x[4] * (1.5 - v/2 * x[4] * x[4])
    // asm 0000A643: 	RND	R1
    // asm 0000A644: 	POPF	R2
    // asm 0000A645: 	POP	R2
    // asm 0000A646: 	MPYF	R2,R1,R0	;sqrt(x) = x * sqrt(1/x)
    // asm 0000A647: 	POPF	R1
    // asm 0000A648: 	POP	R1
    // asm 0000A649: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SQRT", 0, 0);
    UNIMPL();
}

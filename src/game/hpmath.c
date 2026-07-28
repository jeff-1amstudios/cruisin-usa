// From asm/HPMATH.C
#include "hpmath.h"

// c3x-lint: allow-c3x-f32 -- polynomial coefficients require register precision.

#include <math.h>

#define BITS 23          /* There are 23 bits in the mantissa     */
#define MAXX 88.72283906 /* ln(HUGE_VAL)                          */
#define MAXH 89.41598624 /* ln(HUGE_VAL) + ln(2)                  */
#define TWO23 8388608    /* 2 ^ BITS                              */
#define XBIG 8.664339757 /* (BITS/2 + 1) * ln(2)                  */

/* macros used in sin and cos */

#define INVSPI 0.31830988618379067154
#define HALFPI 1.57079632679489661923

#define C1 3.140625
#define C2 9.67653589793e-4

#define R1 -0.1666665668e+0
#define R2 0.8333025139e-2
#define R3 -0.1980741872e-3
#define R4 0.2601903036e-5

/*	HPsin() - High Precision sine
 *
 *	Based on the algorithm from "Software Manual for the Elementary
 *	Functions", Cody and Waite, Prentice Hall 1980, chapter 8.
 *
 *	N = round(x / PI)
 *	f = x - N * PI
 *	g = f * f
 *	R = polynomial expansion
 *
 *	result = f + f * R
 *
 *	if x < 0, result = - result
 *	if N is even, result = - result
 *
 *	This will return the wrong result for x >= MAXINT * PI
 */
c3x_reg_t _HPsin(c3x_reg_t x) {
    c3x_reg_t d, y, xn, f, g, rg;
    c3x_reg_t sgn = C3X_LT(x, C3X_FROM_INT(0)) ? C3X_FROM_INT(-1) : C3X_FROM_INT(1);
    int n;

    x = C3X_ABS(x);
    n = c3x_fix(C3X_ADD(C3X_MUL(x, C3X_F32(INVSPI)), C3X_F32(0.5)));
    xn = C3X_FROM_INT(n);

    /*
     * if n is odd, negate the sign
     */
    if (n % 2)
        sgn = C3X_NEG(sgn);

    /*
     * f = x - xn * PI (but mathematically more stable)
     */
    f = C3X_SUB(C3X_SUB(x, C3X_MUL(xn, C3X_F32(C1))), C3X_MUL(xn, C3X_F32(C2)));

    /*
     * determine polynomial expression
     */
    g = C3X_MUL(f, f);

    rg = C3X_MUL(C3X_ADD(C3X_MUL(C3X_ADD(C3X_MUL(C3X_ADD(C3X_MUL(C3X_F32(R4), g), C3X_F32(R3)), g), C3X_F32(R2)), g), C3X_F32(R1)), g);

    return C3X_MUL(sgn, C3X_ADD(f, C3X_MUL(f, rg)));
}

/*	HPcos() - High Precision Cosine
 *
 *	Based on the algorithm from "Software Manual for the Elementary
 *	Functions", Cody and Waite, Prentice Hall 1980, chapter 8.
 *
 *	N = round(x / PI + 1/2) - 0.5
 *	f = x - N * PI
 *	g = f * f
 *	R = polynomial expression
 *
 *	result = f + f * R
 *	if N is even, result = - result
 *
 *	This will return the wrong result for x >= MAXINT * PI
 */
c3x_reg_t _HPcos(c3x_reg_t x) {
    c3x_reg_t sgn; /* the sign of the result */
    c3x_reg_t xn, f, g, rg;
    c3x_reg_t rounded_index;
    int n;

    /*
     * cos(x) = cos(-x)
     */
    x = C3X_ABS(x);

    /*
     * n = round(x/PI + 1/2) (can be rounded this way, since positive number)
     */
    rounded_index = C3X_ADD(C3X_MUL(C3X_ADD(x, C3X_F32(HALFPI)), C3X_F32(INVSPI)), C3X_F32(0.5));
    n = c3x_fix(rounded_index);
    xn = C3X_SUB(C3X_FROM_INT(n), C3X_F32(0.5));

    /*
     * if n is odd, negate the sign
     */
    sgn = (n % 2) ? C3X_FROM_INT(-1) : C3X_FROM_INT(1);

    /*
     * f = x - xn * PI (but more mathematically stable)
     */
    f = C3X_SUB(C3X_SUB(x, C3X_MUL(xn, C3X_F32(C1))), C3X_MUL(xn, C3X_F32(C2)));

    /*
     * determine polynomial expression
     */
    g = C3X_MUL(f, f);

    rg = C3X_MUL(C3X_F32(R4), g);
    rg = C3X_ADD(rg, C3X_F32(R3));
    rg = C3X_MUL(g, rg);
    rg = C3X_ADD(rg, C3X_F32(R2));
    rg = C3X_MUL(g, rg);
    rg = C3X_ADD(rg, C3X_F32(R1));
    rg = C3X_MUL(g, rg);
    rg = C3X_MUL(f, rg);
    f = C3X_ADD(f, rg);
    return C3X_MUL(sgn, f);
}

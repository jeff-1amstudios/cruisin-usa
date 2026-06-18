#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "dirq.h"
#include "port.h"
#include "math.h"

/*
 * Source module: asm/MATH.ASM
 */

void _COSI(void);
void _SINE(void);
void NORMITS(void);
void NORMIT(void);
void ARCTANF(void);
void FIND_MATRIX(void);
void FIND_XMATRIX(void);
#define FIND_YMATRIX _find_Ymatrix
void _find_Ymatrix(void);
void HPFIND_YMATRIX(void);
void FIND_ZMATRIX(void);
void INITMAT(void);
void VECTLEN(void);
void CPYMAT(void);
void CPYIMAT(void);
void CLR_VECTORA(void);
void MATRIX_MUL(void);
void NORMALIZE(void);
void NORMAT(void);
void GEN_NORMAL(void);
void CONCATMATV(void);
void CONCAT201(void);
void CONCATMAT(void);
void GETTHETADIFF(void);
void DIST_PT2LINE(void);
void GETLINE_EQ_2D(void);
void SCALE_MATRIX(void);

#define FIND_YMATRIX _find_Ymatrix
#define SINTABLEI SINTABLE
#define LOCTEMPER_MAT2I LOCTEMPER_MAT2

extern float SINTABLE[];
extern float FORMULA;
extern float HALFPII;
extern float PII;
extern float TWOPII;
extern float INVFORM;
extern float RADFORM;
extern float RADFORMI;
extern float *ATTABV;
extern float *OFFTABV;
extern float ATOFFTAB[];
extern float ATTAB[];
extern int LOCTEMPER_MAT2[];

/*
*----------------------------------------------------------------------------
*MATH ROUTINES
*
*COPYRIGHT (C) 1994 BY TV GAMES,INC.
*ALL RIGHTS RESERVED
*
*See also:	ROUTS.ASM	Divide,Invert,Modulus
*		HPMATH.C	higher precision functions
*
*/

/*
*----------------------------------------------------------------------------
*	float	COSI(float theta)
*	float	SINE(float theta)
*COSINE FUNCTION
*
*PARAMETERS
*	R2	VALUE IN RADIANS 0-INFINITY
*	R0	RETURN VALUE
*
*/
void _COSI(void)
{
    // asm 0000952D: 	LDF	R2,R0
    // asm 0000952E: 	ADDF	@HALFPII,R0	;offset for COS
    // asm 0000952F: 	B	SINE0
    // *
    // *SINE FUNCTION
    // *
    // *PARAMETERS
    // *	R2	VALUE IN RADIANS 0-INFINITY
    // *	R0	RETURN VALUE
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_COSI", 0, 0);
    UNIMPL();
}

void _SINE(void)
{
    // asm 00009530: 	LDF	R2,R0
SINE0:
    // asm 00009531: PUSH	AR0
    // asm 00009532: 	BGE	NOTNEG
    // asm 00009533: 	NEGF	R0		;fix negative case
    // asm 00009534: 	ADDF	@PII,R0
NOTNEG:
    // asm 00009535: MPYF	@FORMULA,R0
    // asm 00009536: 	ADDF	0.5,R0		;ROUND THE SUCKER
    // asm 00009537: 	FIX	R0		;get a raw index
    // asm 00009538: 	AND	3FFh,R0
    // asm 00009539: 	CMPI	512,R0
    // asm 0000953A: 	BLT	ALLOK
    // asm 0000953B: 	SUBI	512,R0
    // asm 0000953C: 	CMPI	256,R0
    // asm 0000953D: 	BLT	PERFECT2
    // asm 0000953E: 	SUBI	512,R0
    // asm 0000953F: 	NEGI	R0,R0
PERFECT2:
    // asm 00009540: 	ADDI	@SINTABLEI,R0
    // asm 00009541: 	LDI	R0,AR0
    // asm 00009542: 	NEGF	*AR0,R0
    // asm 00009543: 	POP	AR0
    // asm 00009544: 	RETS
ALLOK:
    // asm 00009545: 	CMPI	256,R0
    // asm 00009546: 	BLT	PERFECT
    // asm 00009547: 	SUBI	512,R0
    // asm 00009548: 	NEGI	R0,R0
PERFECT:
    // asm 00009549: ADDI	@SINTABLEI,R0
    // asm 0000954A: 	LDI	R0,AR0
    // asm 0000954B: 	LDF	*AR0,R0
    // asm 0000954C: 	POP	AR0
    // asm 0000954D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_SINE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: SINTABLE	;257 entries */
/* asm: 	.float	0.000000,0.006136,0.012272,0.018407,0.024541,0.030675,0.036807,0.042938 */
/* asm: 	.float	0.049068,0.055195,0.061321,0.067444,0.073565,0.079682,0.085797,0.091909 */
/* asm: 	.float	0.098017,0.104122,0.110222,0.116319,0.122411,0.128498,0.134581,0.140658 */
/* asm: 	.float	0.146730,0.152797,0.158858,0.164913,0.170962,0.177004,0.183040,0.189069 */
/* asm: 	.float	0.195090,0.201105,0.207111,0.213110,0.219101,0.225084,0.231058,0.237024 */
/* asm: 	.float	0.242980,0.248928,0.254866,0.260794,0.266713,0.272621,0.278520,0.284408 */
/* asm: 	.float	0.290285,0.296151,0.302006,0.307850,0.313682,0.319502,0.325310,0.331106 */
/* asm: 	.float	0.336890,0.342661,0.348419,0.354163,0.359895,0.365613,0.371317,0.377007 */
/* asm: 	.float	0.382683,0.388345,0.393992,0.399624,0.405241,0.410843,0.416429,0.422000 */
/* asm: 	.float	0.427555,0.433094,0.438616,0.444122,0.449611,0.455083,0.460538,0.465976 */
/* asm: 	.float	0.471396,0.476799,0.482183,0.487550,0.492898,0.498227,0.503538,0.508830 */
/* asm: 	.float	0.514102,0.519356,0.524589,0.529803,0.534997,0.540171,0.545325,0.550458 */
/* asm: 	.float	0.555570,0.560661,0.565732,0.570781,0.575808,0.580814,0.585798,0.590760 */
/* asm: 	.float	0.595699,0.600616,0.605511,0.610383,0.615232,0.620057,0.624860,0.629638 */
/* asm: 	.float	0.634393,0.639125,0.643832,0.648515,0.653173,0.657807,0.662416,0.667000 */
/* asm: 	.float	0.671559,0.676093,0.680601,0.685084,0.689541,0.693972,0.698376,0.702755 */
/* asm: 	.float	0.707107,0.711432,0.715731,0.720003,0.724247,0.728465,0.732655,0.736817 */
/* asm: 	.float	0.740951,0.745058,0.749137,0.753187,0.757209,0.761203,0.765168,0.769104 */
/* asm: 	.float	0.773011,0.776889,0.780738,0.784557,0.788347,0.792107,0.795837,0.799538 */
/* asm: 	.float	0.803208,0.806848,0.810458,0.814037,0.817585,0.821103,0.824590,0.828046 */
/* asm: 	.float	0.831470,0.834863,0.838225,0.841555,0.844854,0.848121,0.851356,0.854559 */
/* asm: 	.float	0.857729,0.860867,0.863973,0.867047,0.870088,0.873096,0.876071,0.879013 */
/* asm: 	.float	0.881922,0.884798,0.887640,0.890449,0.893225,0.895967,0.898675,0.901349 */
/* asm: 	.float	0.903990,0.906596,0.909169,0.911707,0.914210,0.916680,0.919114,0.921515 */
/* asm: 	.float	0.923880,0.926211,0.928507,0.930767,0.932993,0.935184,0.937340,0.939460 */
/* asm: 	.float	0.941545,0.943594,0.945608,0.947586,0.949529,0.951436,0.953307,0.955142 */
/* asm: 	.float	0.956941,0.958704,0.960431,0.962122,0.963777,0.965395,0.966977,0.968523 */
/* asm: 	.float	0.970032,0.971504,0.972940,0.974340,0.975703,0.977029,0.978318,0.979570 */
/* asm: 	.float	0.980786,0.981964,0.983106,0.984210,0.985278,0.986308,0.987302,0.988258 */
/* asm: 	.float	0.989177,0.990059,0.990903,0.991710,0.992480,0.993212,0.993907,0.994565 */
/* asm: 	.float	0.995185,0.995768,0.996313,0.996821,0.997291,0.997723,0.998118,0.998476 */
/* asm: 	.float	0.998796,0.999078,0.999322,0.999529,0.999699,0.999831,0.999925,0.999981 */
/* asm: 	.float	1.0 */
float SINTABLE[] = {
    0.000000f, 0.006136f, 0.012272f, 0.018407f, 0.024541f, 0.030675f, 0.036807f, 0.042938f,
    0.049068f, 0.055195f, 0.061321f, 0.067444f, 0.073565f, 0.079682f, 0.085797f, 0.091909f,
    0.098017f, 0.104122f, 0.110222f, 0.116319f, 0.122411f, 0.128498f, 0.134581f, 0.140658f,
    0.146730f, 0.152797f, 0.158858f, 0.164913f, 0.170962f, 0.177004f, 0.183040f, 0.189069f,
    0.195090f, 0.201105f, 0.207111f, 0.213110f, 0.219101f, 0.225084f, 0.231058f, 0.237024f,
    0.242980f, 0.248928f, 0.254866f, 0.260794f, 0.266713f, 0.272621f, 0.278520f, 0.284408f,
    0.290285f, 0.296151f, 0.302006f, 0.307850f, 0.313682f, 0.319502f, 0.325310f, 0.331106f,
    0.336890f, 0.342661f, 0.348419f, 0.354163f, 0.359895f, 0.365613f, 0.371317f, 0.377007f,
    0.382683f, 0.388345f, 0.393992f, 0.399624f, 0.405241f, 0.410843f, 0.416429f, 0.422000f,
    0.427555f, 0.433094f, 0.438616f, 0.444122f, 0.449611f, 0.455083f, 0.460538f, 0.465976f,
    0.471396f, 0.476799f, 0.482183f, 0.487550f, 0.492898f, 0.498227f, 0.503538f, 0.508830f,
    0.514102f, 0.519356f, 0.524589f, 0.529803f, 0.534997f, 0.540171f, 0.545325f, 0.550458f,
    0.555570f, 0.560661f, 0.565732f, 0.570781f, 0.575808f, 0.580814f, 0.585798f, 0.590760f,
    0.595699f, 0.600616f, 0.605511f, 0.610383f, 0.615232f, 0.620057f, 0.624860f, 0.629638f,
    0.634393f, 0.639125f, 0.643832f, 0.648515f, 0.653173f, 0.657807f, 0.662416f, 0.667000f,
    0.671559f, 0.676093f, 0.680601f, 0.685084f, 0.689541f, 0.693972f, 0.698376f, 0.702755f,
    0.707107f, 0.711432f, 0.715731f, 0.720003f, 0.724247f, 0.728465f, 0.732655f, 0.736817f,
    0.740951f, 0.745058f, 0.749137f, 0.753187f, 0.757209f, 0.761203f, 0.765168f, 0.769104f,
    0.773011f, 0.776889f, 0.780738f, 0.784557f, 0.788347f, 0.792107f, 0.795837f, 0.799538f,
    0.803208f, 0.806848f, 0.810458f, 0.814037f, 0.817585f, 0.821103f, 0.824590f, 0.828046f,
    0.831470f, 0.834863f, 0.838225f, 0.841555f, 0.844854f, 0.848121f, 0.851356f, 0.854559f,
    0.857729f, 0.860867f, 0.863973f, 0.867047f, 0.870088f, 0.873096f, 0.876071f, 0.879013f,
    0.881922f, 0.884798f, 0.887640f, 0.890449f, 0.893225f, 0.895967f, 0.898675f, 0.901349f,
    0.903990f, 0.906596f, 0.909169f, 0.911707f, 0.914210f, 0.916680f, 0.919114f, 0.921515f,
    0.923880f, 0.926211f, 0.928507f, 0.930767f, 0.932993f, 0.935184f, 0.937340f, 0.939460f,
    0.941545f, 0.943594f, 0.945608f, 0.947586f, 0.949529f, 0.951436f, 0.953307f, 0.955142f,
    0.956941f, 0.958704f, 0.960431f, 0.962122f, 0.963777f, 0.965395f, 0.966977f, 0.968523f,
    0.970032f, 0.971504f, 0.972940f, 0.974340f, 0.975703f, 0.977029f, 0.978318f, 0.979570f,
    0.980786f, 0.981964f, 0.983106f, 0.984210f, 0.985278f, 0.986308f, 0.987302f, 0.988258f,
    0.989177f, 0.990059f, 0.990903f, 0.991710f, 0.992480f, 0.993212f, 0.993907f, 0.994565f,
    0.995185f, 0.995768f, 0.996313f, 0.996821f, 0.997291f, 0.997723f, 0.998118f, 0.998476f,
    0.998796f, 0.999078f, 0.999322f, 0.999529f, 0.999699f, 0.999831f, 0.999925f, 0.999981f,
    1.0f,
};
/* asm: FORMULA	.float	162.9746617		;256/(PI/2) */
/* asm: 	 */
float FORMULA = 162.9746617f;
/* asm: HALFPII	.float	HALFPI */
float HALFPII = HALFPI;
/* asm: PII	.float	PI */
float PII = PI;
/* asm: TWOPII	.float	TWOPI */
float TWOPII = TWOPI;
/* asm: INVFORM	.float	0.012265625	;1/FORMULA */
float INVFORM = 0.012265625f;
// *----------------------------------------------------------------------------

/* asm: RADFORM		.float	10430.37835	;65535/(2PI) */
float RADFORM = 10430.37835f;
/* asm: RADFORMI	.float	0.000095873	;1/(65535/(2PI)) */
/* asm: 	 */
/* asm: 	 */
float RADFORMI = 0.000095873f;

/*
*----------------------------------------------------------------------------
*PARAMETERS
*	R2	RADIANS
*OUTPUT
*	R2	IN RANGE -PI TO +PI
*	N,Z BITS SET FOR R2
*
*/
void NORMITS(void)
{
    // asm 00009556: 	MPYF	@RADFORM,R2
    // asm 00009557: 	FIX	R2
    // asm 00009558: 	LS	16,R2
    // asm 00009559: 	RS	16,R2
    // asm 0000955A: 	CMPI	7FFFH,R2
    // asm 0000955B: 	BLT	NMS1
    // asm 0000955C: 	ADDI	8000H,R2
    // asm 0000955D: 	ADDI	8000H,R2
NMS1:
    // asm 0000955E: 	FLOAT	R2
    // asm 0000955F: 	MPYF	@RADFORMI,R2
    // asm 00009560: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NORMITS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*PARAMETERS
*	R2	RADIANS
*OUTPUT
*	R2	IN RANGE 0 TO 2PI
*
*/
void NORMIT(void)
{
    // asm 00009561: 	MPYF	@RADFORM,R2
    // asm 00009562: 	FIX	R2
    // asm 00009563: 	LS	16,R2
    // asm 00009564: 	RS	16,R2
    // asm 00009565: 	FLOAT	R2
    // asm 00009566: 	MPYF	@RADFORMI,R2
    // asm 00009567: NORMCHKL
    // asm 00009567: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NORMIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*ARCTANF - GET ARCTAN ( R3/R2) IN RADIANS
*float	arctanf(float x,float y)  <- not true C style
*
*PARAMETERS
*	R2	X (float)
*	R3	Y (float)
*RETURNS
*	R0	RADIANS (FLOAT)
*
*/
void ARCTANF(void)
{
    // asm 00009568: _arctanf
    // asm 00009568: 	PUSH	R1
    // asm 00009569: 	PUSHF	R1
    // asm 0000956A: 	PUSH	R4
    // asm 0000956B: 	PUSH	AR0
    // 	;GET QUADRANT
    // asm 0000956C: 	LDI	0,R1
    // asm 0000956D: 	LDF	R2,R2
    // asm 0000956E: 	LDIN	2,R1		;GET SIGN BITS
    // asm 0000956F: 	LDI	0,R4
    // asm 00009570: 	LDF	R3,R3
    // asm 00009571: 	LDIN	4,R4
    // asm 00009572: 	ADDI	R1,R4
    // 	;CHECK Y>X
    // asm 00009573: 	ABSF	R2,R0
    // asm 00009574: 	ABSF	R3,R1
    // asm 00009575: 	CMPF	R0,R1
    // asm 00009576: 	BLT	ATF0
    // asm 00009577: 	BD	AT1
    // asm 00009578: 	LDF	R2,R0
    // asm 00009579: 	LDF	R3,R1
    // asm 0000957A: 	ADDI 	1,R4 		;R4 = OCTANT
    // 	;---->BD	AT1
ATF0:
    // asm 0000957B: 	LDF	R2,R1
    // asm 0000957C: 	LDF	R3,R0
AT1:
    // asm 0000957D: 	CALL	DIV_F
    // asm 0000957E: 	ABSF	R0
    // asm 0000957F: 	LDF	128.0,R1
    // asm 00009580: 	MPYF	R1,R0
    // asm 00009581: 	FIX	R0,AR0
    // asm 00009582: 	ADDI	@ATTABV,AR0
    // asm 00009583: 	LDF	*AR0,R0
    // asm 00009584: 	LSH	1,R4
    // asm 00009585: 	LDI	@OFFTABV,AR0
    // asm 00009586: 	ADDI	R4,AR0
    // asm 00009587: 	MPYF	*AR0++,R0
    // asm 00009588: 	ADDF	*AR0,R0
    // asm 00009589: 	POP	AR0
    // asm 0000958A: 	POP	R4
    // asm 0000958B:       	POPF	R1
    // asm 0000958C:       	POP	R1
    // asm 0000958D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ARCTANF", 0, 0);
    UNIMPL();
}

/* asm: ATTABV	.word	ATTAB */
float *ATTABV = ATTAB;
/* asm: OFFTABV	.word	ATOFFTAB */
float *OFFTABV = ATOFFTAB;
/* asm: ATOFFTAB */
/* asm: 	.float	1		;0-45 */
/* asm: 	.float	0 */
/* asm: 	.float	-1		;45-90 */
/* asm: 	.float	1.5707 */
/* asm: 	.float	-1		;135-180 */
/* asm: 	.float	3.1416 */
/* asm: 	.float	1		;90-135 */
/* asm: 	.float	1.5707 */
/* asm: 	.float	-1		;315-360 */
/* asm: 	.float	6.2831 */
/* asm: 	.float	1		;270-315 */
/* asm: 	.float	4.7123 */
/* asm: 	.float	1		;180-225 */
/* asm: 	.float	3.1416 */
/* asm: 	.float	-1		;225-270 */
/* asm: 	.float	4.7123 */
/* asm: 	 */
float ATOFFTAB[] = {
    1.0f, // 0-45
    0.0f,
    -1.0f, // 45-90
    1.5707f,
    -1.0f, // 135-180
    3.1416f,
    1.0f, // 90-135
    1.5707f,
    -1.0f, // 315-360
    6.2831f,
    1.0f, // 270-315
    4.7123f,
    1.0f, // 180-225
    3.1416f,
    -1.0f, // 225-270
    4.7123f,
};
/*
*
*ARCTAN TABLE 0-45
*
*/
/* asm: ATTAB */
/* asm: 	.float	0.000000,0.007812,0.015624,0.023433,0.031240,0.039043,0.046841 */
/* asm: 	.float	0.054633,0.062419,0.070197,0.077967,0.085727,0.093477,0.101215 */
/* asm: 	.float	0.108942,0.116655,0.124355,0.132040,0.139709,0.147361,0.154997 */
/* asm: 	.float	0.162614,0.170212,0.177790,0.185348,0.192884,0.200399,0.207890 */
/* asm: 	.float	0.215358,0.222801,0.230220,0.237612,0.244979,0.252318,0.259630 */
/* asm: 	.float	0.266913,0.274167,0.281392,0.288587,0.295752,0.302885,0.309986 */
/* asm: 	.float	0.317056,0.324092,0.331096,0.338066,0.345002,0.351904,0.358771 */
/* asm: 	.float	0.365602,0.372398,0.379159,0.385883,0.392570,0.399221,0.405834 */
/* asm: 	.float	0.412410,0.418949,0.425450,0.431912,0.438337,0.444722,0.451070 */
/* asm: 	.float	0.457378,0.463648,0.469878,0.476069,0.482221,0.488334,0.494407 */
/* asm: 	.float	0.500441,0.506435,0.512389,0.518304,0.524180,0.530015,0.535811 */
/* asm: 	.float	0.541568,0.547284,0.552962,0.558599,0.564198,0.569756,0.575276 */
/* asm: 	.float	0.580756,0.586198,0.591600,0.596963,0.602287,0.607573,0.612820 */
/* asm: 	.float	0.618029,0.623199,0.628332,0.633426,0.638482,0.643501,0.648482 */
/* asm: 	.float	0.653426,0.658333,0.663203,0.668036,0.672833,0.677593,0.682317 */
/* asm: 	.float	0.687005,0.691657,0.696273,0.700854,0.705400,0.709912,0.714388 */
/* asm: 	.float	0.718830,0.723238,0.727611,0.731951,0.736257,0.740530,0.744770 */
/* asm: 	.float	0.748977,0.753151,0.757293,0.761403,0.765480,0.769526,0.773541 */
/* asm: 	.float	0.777524,0.781477,0.785398 */
float ATTAB[] = {
    0.000000f, 0.007812f, 0.015624f, 0.023433f, 0.031240f, 0.039043f, 0.046841f,
    0.054633f, 0.062419f, 0.070197f, 0.077967f, 0.085727f, 0.093477f, 0.101215f,
    0.108942f, 0.116655f, 0.124355f, 0.132040f, 0.139709f, 0.147361f, 0.154997f,
    0.162614f, 0.170212f, 0.177790f, 0.185348f, 0.192884f, 0.200399f, 0.207890f,
    0.215358f, 0.222801f, 0.230220f, 0.237612f, 0.244979f, 0.252318f, 0.259630f,
    0.266913f, 0.274167f, 0.281392f, 0.288587f, 0.295752f, 0.302885f, 0.309986f,
    0.317056f, 0.324092f, 0.331096f, 0.338066f, 0.345002f, 0.351904f, 0.358771f,
    0.365602f, 0.372398f, 0.379159f, 0.385883f, 0.392570f, 0.399221f, 0.405834f,
    0.412410f, 0.418949f, 0.425450f, 0.431912f, 0.438337f, 0.444722f, 0.451070f,
    0.457378f, 0.463648f, 0.469878f, 0.476069f, 0.482221f, 0.488334f, 0.494407f,
    0.500441f, 0.506435f, 0.512389f, 0.518304f, 0.524180f, 0.530015f, 0.535811f,
    0.541568f, 0.547284f, 0.552962f, 0.558599f, 0.564198f, 0.569756f, 0.575276f,
    0.580756f, 0.586198f, 0.591600f, 0.596963f, 0.602287f, 0.607573f, 0.612820f,
    0.618029f, 0.623199f, 0.628332f, 0.633426f, 0.638482f, 0.643501f, 0.648482f,
    0.653426f, 0.658333f, 0.663203f, 0.668036f, 0.672833f, 0.677593f, 0.682317f,
    0.687005f, 0.691657f, 0.696273f, 0.700854f, 0.705400f, 0.709912f, 0.714388f,
    0.718830f, 0.723238f, 0.727611f, 0.731951f, 0.736257f, 0.740530f, 0.744770f,
    0.748977f, 0.753151f, 0.757293f, 0.761403f, 0.765480f, 0.769526f, 0.773541f,
    0.777524f, 0.781477f, 0.785398f,
};
// *----------------------------------------------------------------------------

/* asm: LOCTEMPER_MAT2	fbss	LOCTEMPER_MAT2,12 */
int LOCTEMPER_MAT2[12];

/*
*----------------------------------------------------------------------------
*FIND MATRIX
*
*PARAMETERS
*	AR2	DESTINATION 3X3
*	R2	SOURCE RADIANS 1X3
*
*/
void FIND_MATRIX(void)
{
    // asm 00009591: 	PUSH	R0
    // asm 00009592: 	PUSH	R1
    // asm 00009593: 	PUSH	R2
    // asm 00009594: 	PUSHF	R0
    // asm 00009595: 	PUSHF	R1
    // asm 00009596: 	PUSHF	R2
    // asm 00009597: 	PUSH	AR1
    // asm 00009598: 	PUSH	AR2
    // asm 00009599: 	PUSH	AR3
    // 	;GET SINES AND COSINES
    // asm 0000959A: 	LDI	@LOCTEMPER_MAT2I,AR1
    // asm 0000959B: 	LDI	R2,AR3
    // asm 0000959C: 	LDI	2,RC
    // asm 0000959D: 	RPTB	FM1
    // asm 0000959E: 	LDF	*AR3++,R2
    // asm 0000959F: 	CALL	_SINE
    // asm 000095A0: 	STF	R0,*+AR1(3)
    // asm 000095A1: 	CALL	_COSI
FM1:
    // asm 000095A2: STF	R0,*AR1++
    // asm 000095A3: 	NOP	*--AR1(2)			;CENTER COSINE INDEX
    // asm 000095A4: 	LDI	AR1,AR3
    // asm 000095A5: 	ADDI	3,AR3				;CENTER SINE INDEX
    // asm 000095A6: 	MPYF	*+AR1(1),*+AR1(0),R0		;CZ*CY
    // asm 000095A7: 	STF	R0,*AR2++		 	;A(0,0)=CZ*CY
    // asm 000095A8: 	MPYF	*+AR3(1),*+AR1(0),R0		;SZ*CY
    // asm 000095A9: 	NEGF	*+AR3(0),R2			;-SY
    // asm 000095A9:  ||	STF	R0,*AR2++			;A(0,1)=SZ*CY
    // asm 000095AA: 	MPYF	*-AR3(1),*+AR3(0),R0		;SX*SY
    // asm 000095AB: 	MPYF	*+AR1(1),R0,R1			;SX*SY*CZ
    // asm 000095AB:  ||	STF	R2,*AR2++			;A(0,2)=-SY
    // asm 000095AC: 	MPYF	*-AR1(1),*+AR3(1),R2		;CX*SZ
    // asm 000095AD: 	SUBF	R2,R1
    // asm 000095AE: 	MPYF	*+AR3(1),R0,R0			;SZ*(SX*SY)
    // asm 000095AE:  ||	STF	R1,*AR2++			;A(1,0)=SX*SY*SZ-CX*SZ
    // asm 000095AF: 	MPYF	*-AR1(1),*+AR1(1),R1		;CX*CZ
    // asm 000095B0: 	ADDF	R1,R0
    // asm 000095B1: 	STF	R0,*AR2++			;A(1,1)= SX*SY*SZ+CX*CZ
    // asm 000095B2: 	MPYF	*-AR3(1),*+AR1(0),R0
    // asm 000095B3: 	MPYF	*+AR3(0),R1,R1			;SY*(CX*CZ)
    // asm 000095B3:  ||	STF	R0,*AR2++			;A(1,2)= SX*CY
    // asm 000095B4: 	MPYF	*-AR3(1),*+AR3(1),R0		;SX*SZ
    // asm 000095B5: 	ADDF	R1,R0
    // asm 000095B6: 	MPYF	*+AR3(0),R2,R2
    // asm 000095B6:  ||	STF	R0,*AR2++			;A(2,0)= CX*SY*CZ+SX*SZ
    // asm 000095B7: 	MPYF	*-AR3(1),*+AR1(1),R0
    // asm 000095B8: 	MPYF	*-AR1(1),*+AR1(0),R1		;CX*CY
    // asm 000095B8:  ||	SUBF	R0,R2
    // asm 000095B9: 	STF	R2,*AR2++			;A(2,1)= CX*SY*SZ-SX*CZ
    // asm 000095BA: 	STF	R1,*AR2--(8)			;A(2,2)= CX*CY
    // asm 000095BB: 	POP	AR3
    // asm 000095BC: 	POP	AR2
    // asm 000095BD: 	POP	AR1
    // asm 000095BE: 	POPF	R3
    // asm 000095BF: 	POPF	R2
    // asm 000095C0: 	POPF	R1
    // asm 000095C1: 	POP	R3
    // asm 000095C2: 	POP	R2
    // asm 000095C3: 	POP	R1
    // asm 000095C4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_MATRIX", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*	    1  0  0
* Xrot = {  0  c  s }
*	    0 -s  c
*
*PARAMETERS
*	AR2	DESTINATION 3X3 MATRIX
*	R2	SOURCE RADIANS
*
*/
void FIND_XMATRIX(void)
{
    // asm 000095C5: 	PUSH	R0
    // asm 000095C6: 	PUSHF	R0
    // asm 000095C7: 	CALL	_COSI
    // asm 000095C8: 	STF	R0,*+AR2(A11)
    // asm 000095C9: 	STF	R0,*+AR2(A22)
    // asm 000095CA: 	CALL	_SINE
    // asm 000095CB: 	STF	R0,*+AR2(A12)
    // asm 000095CC: 	NEGF	R0
    // asm 000095CD: 	STF	R0,*+AR2(A21)
    // asm 000095CE: 	LDF	1,R0
    // asm 000095CF: 	STF	R0,*+AR2(A00)
    // asm 000095D0: 	CLRF	R0
    // asm 000095D1: 	STF	R0,*+AR2(A01)
    // asm 000095D2: 	STF	R0,*+AR2(A02)
    // asm 000095D3: 	STF	R0,*+AR2(A10)
    // asm 000095D4: 	STF	R0,*+AR2(A20)
    // asm 000095D5: 	POPF	R0
    // asm 000095D6: 	POP	R0
    // asm 000095D7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_XMATRIX", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*	    c  0 -s
* Yrot = {  0  1  0 }
*	    s  0  c
*
*PARAMETERS
*	AR2	DESTINATION 3X3 MATRIX
*	R2	SOURCE RADIANS
*
*/
void _find_Ymatrix(void)
{
    // asm 000095D8: 	PUSH	R0
    // asm 000095D9: 	PUSHF	R0
    // asm 000095DA: 	CALL	_COSI
    // asm 000095DB: 	STF	R0,*+AR2(A00)
    // asm 000095DC: 	STF	R0,*+AR2(A22)
    // asm 000095DD: 	CALL	_SINE
    // asm 000095DE: 	STF	R0,*+AR2(A20)
    // asm 000095DF: 	NEGF	R0
    // asm 000095E0: 	STF	R0,*+AR2(A02)
    // asm 000095E1: 	LDF	1,R0
    // asm 000095E2: 	STF	R0,*+AR2(A11)
    // asm 000095E3: 	CLRF	R0
    // asm 000095E4: 	STF	R0,*+AR2(A01)
    // asm 000095E5: 	STF	R0,*+AR2(A10)
    // asm 000095E6: 	STF	R0,*+AR2(A12)
    // asm 000095E7: 	STF	R0,*+AR2(A21)
    // asm 000095E8: 	POPF	R0
    // asm 000095E9: 	POP	R0
    // asm 000095EA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_find_Ymatrix", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*	    c  0 -s
* Yrot = {  0  1  0 }
*	    s  0  c
*
*PARAMETERS
*	AR2	DESTINATION 3X3 MATRIX
*	R2	SOURCE RADIANS
*
*/
void HPFIND_YMATRIX(void)
{
    // asm 000095EB: 	PUSH	R0
    // asm 000095EC: 	PUSHF	R0
    // asm 000095ED: 	PUSH	R1
    // asm 000095EE: 	PUSHF	R1
    // asm 000095EF: 	PUSH	R2
    // asm 000095F0: 	PUSHF	R2
    // asm 000095F1: 	PUSH	R3
    // asm 000095F2: 	PUSHF	R3
    // asm 000095F3: 	PUSH	R4
    // asm 000095F4: 	PUSHF	R4
    // asm 000095F5: 	PUSH	R2
    // asm 000095F6: 	PUSHF	R2
    // asm 000095F7: 	CALL	_HPcos
    // asm 000095F8: 	STF	R0,*+AR2(A00)
    // asm 000095F9: 	STF	R0,*+AR2(A22)
    // asm 000095FA: 	POPF	R2
    // asm 000095FB: 	POP	R2
    // asm 000095FC: 	CALL	_HPsin
    // asm 000095FD: 	STF	R0,*+AR2(A20)
    // asm 000095FE: 	NEGF	R0
    // asm 000095FF: 	STF	R0,*+AR2(A02)
    // asm 00009600: 	LDF	1,R0
    // asm 00009601: 	STF	R0,*+AR2(A11)
    // asm 00009602: 	CLRF	R0
    // asm 00009603: 	STF	R0,*+AR2(A01)
    // asm 00009604: 	STF	R0,*+AR2(A10)
    // asm 00009605: 	STF	R0,*+AR2(A12)
    // asm 00009606: 	STF	R0,*+AR2(A21)
    // asm 00009607: 	POPF	R4
    // asm 00009608: 	POP	R4
    // asm 00009609: 	POPF	R3
    // asm 0000960A: 	POP	R3
    // asm 0000960B: 	POPF	R2
    // asm 0000960C: 	POP	R2
    // asm 0000960D: 	POPF	R1
    // asm 0000960E: 	POP	R1
    // asm 0000960F: 	POPF	R0
    // asm 00009610: 	POP	R0
    // asm 00009611: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HPFIND_YMATRIX", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
* 	    c s 0
* Zrot = { -s c 0 }
* 	    0 0 1
*
*PARAMETERS
*	AR2	DESTINATION 3X3 MATRIX
*	R2	SOURCE RADIANS
*
*/
void FIND_ZMATRIX(void)
{
    // asm 00009612: 	PUSH	R0
    // asm 00009613: 	PUSHF	R0
    // asm 00009614: 	CALL	_COSI
    // asm 00009615: 	STF	R0,*+AR2(A00)
    // asm 00009616: 	STF	R0,*+AR2(A11)
    // asm 00009617: 	CALL	_SINE
    // asm 00009618: 	STF	R0,*+AR2(A01)
    // asm 00009619: 	NEGF	R0
    // asm 0000961A: 	STF	R0,*+AR2(A10)
    // asm 0000961B: 	LDF	1,R0
    // asm 0000961C: 	STF	R0,*+AR2(A22)
    // asm 0000961D: 	CLRF	R0
    // asm 0000961E: 	STF	R0,*+AR2(A02)
    // asm 0000961F: 	STF	R0,*+AR2(A12)
    // asm 00009620: 	STF	R0,*+AR2(A20)
    // asm 00009621: 	STF	R0,*+AR2(A21)
    // asm 00009622: 	POPF	R0
    // asm 00009623: 	POP	R0
    // asm 00009624: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_ZMATRIX", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*INITIALIZE MATRIX TO IDENTITY
*
*PARAMETERS
*	AR0	POINTER TO MATRIX
*
*/
void INITMAT(void)
{
    // asm 00009625: 	POP	BK			;RETURN ADDRESS
    // asm 00009626: 	PUSH	R0
    // asm 00009627: 	PUSHF	R0
    // asm 00009628: 	LDF	1.0,R0
    // asm 00009629: 	STF	R0,*AR0
    // asm 0000962A: 	STF	R0,*+AR0(4)
    // asm 0000962B: 	STF	R0,*+AR0(8)
    // asm 0000962C: 	LDF	0,R0
    // asm 0000962D: 	STF	R0,*+AR0(1)
    // asm 0000962E: 	STF	R0,*+AR0(2)
    // asm 0000962F: 	STF	R0,*+AR0(3)
    // asm 00009630: 	STF	R0,*+AR0(5)
    // asm 00009631: 	STF	R0,*+AR0(6)
    // asm 00009632: 	BUD	BK
    // asm 00009633: 	STF	R0,*+AR0(7)
    // asm 00009634: 	POPF	R0
    // asm 00009635: 	POP	R0
    // 	;---->	RETS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITMAT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*float	vectlen(VECTOR *);
*
*PARAMETERS
*	AR2	PTS TO VECTOR
*RETURNS
*	R0	LENGTH OF VECTOR
*CLOBBERS
*	R1,R2
*
*/
void VECTLEN(void)
{
    // asm 00009636: 	LDF	*AR2++,R2
    // asm 00009637: 	MPYF	R2,R2
    // asm 00009638: 	LDF	*AR2++,R1
    // asm 00009639: 	MPYF	R1,R1
    // asm 0000963A: 	ADDF	R1,R2
    // asm 0000963B: 	BUD	SQRT		;fall into SQRT
    // asm 0000963C: 	LDF	*AR2--(2),R1
    // asm 0000963D: 	MPYF	R1,R1
    // asm 0000963E: 	ADDF	R1,R2
    // 	;---->BU NOW INTO SQRT routine
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VECTLEN", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*void	cpymat(MATRIX *dst,MATRIX *src);
*
*PARAMETERS
*	AR2	DEST MATRIX
*	R2	SOURCE MATRIX
*
*/
void CPYMAT(void)
{
    // asm 0000963F: 	PUSH	AR0
    // asm 00009640: 	PUSH	R0
    // asm 00009641: 	PUSHF	R0
    // asm 00009642: 	LDI	R2,AR0
    // asm 00009643: 	LDF	*AR0++,R0
    // asm 00009644: 	RPTS	7
    // asm 00009645: 	LDF	*AR0++,R0
    // asm 00009645:  ||	STF	R0,*AR2++
    // asm 00009646: 	STF	R0,*AR2--(8)
    // asm 00009647: 	POPF	R0
    // asm 00009648: 	POP	R0
    // asm 00009649: 	POP	AR0
    // asm 0000964A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CPYMAT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*void	cpyimat(MATRIX *d,MATRIX *s)
*
*
*PARAMETERS
*	AR2	DEST
*	R2	SOURCE
*RETURNS
*	AR2	INVERSE MATRIX (ROWS & COLUMNS XCHNGED)
*
*WARNING SOURCE CANNOT BE SAME AS DEST
*/
void CPYIMAT(void)
{
    // asm 0000964B: 	PUSH	R0
    // asm 0000964C: 	PUSHF	R0
    // asm 0000964D: 	PUSH	AR0
    // asm 0000964E: 	LDI	R2,AR0
    // asm 0000964F: 	LDF	*AR0++(2),R0   	;0->0
    // asm 00009650: 	LDF	*+AR0(1),R0	;3->1
    // asm 00009650:  ||	STF	R0,*AR2++
    // asm 00009651: 	STF	R0,*AR2++
    // asm 00009652: 	LDF	*+AR0(4),R0	;6->2
    // asm 00009653: 	LDF	*-AR0(1),R0	;1->3
    // asm 00009653:  ||	STF	R0,*AR2++
    // asm 00009654: 	STF	R0,*AR2++
    // asm 00009655: 	LDF	*+AR0(2),R0	;4->4
    // asm 00009656: 	STF	R0,*AR2++
    // asm 00009657: 	LDF	*+AR0(5),R0	;7->5
    // asm 00009658: 	LDF	*+AR0(0),R0	;2->6
    // asm 00009658:  ||	STF	R0,*AR2++
    // asm 00009659: 	STF	R0,*AR2++
    // asm 0000965A: 	LDF	*+AR0(3),R0	;5->7
    // asm 0000965B: 	STF	R0,*AR2++
    // asm 0000965C: 	LDF	*+AR0(6),R0	;8->8
    // asm 0000965D: 	STF	R0,*AR2--(8)
    // asm 0000965E: 	POP	AR0
    // asm 0000965F: 	POPF	R0
    // asm 00009660: 	POP	R0
    // asm 00009661: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CPYIMAT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*CLEARS VECTOR A and RETURNS POINTER TO IT IN AR2
*
*/
void CLR_VECTORA(void)
{
    // asm 00009662: 	PUSH	R0
    // asm 00009663: 	PUSHF	R0
    // asm 00009664: 	LDI	@VECTORAI,AR2
    // asm 00009665: 	CLRF	R0
    // asm 00009666: 	STF	R0,*AR2
    // asm 00009667: 	STF	R0,*+AR2(1)
    // asm 00009668: 	STF	R0,*+AR2(2)
    // asm 00009669: 	POPF	R0
    // asm 0000966A: 	POP	R0
    // asm 0000966B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR_VECTORA", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*void	matrix_mul(VECTOR *src,MATRIX *m3x3,VECTOR *dst)
*
*PARAMETERS
*	AR2	SRC 1x3
*	R2	SRC 3x3
*	R3	DST 1x3
*RETURNS
*	*R3 modified, all regs saved
*
*NOTE SRC 1x3 and DST 1x3 may be equal
*
*/
void MATRIX_MUL(void)
{
    // asm 0000966C: 	PUSH	R0
    // asm 0000966D: 	PUSHF	R0
    // asm 0000966E: 	PUSH	AR1
    // asm 0000966F: 	LDI	R2,AR1	    		;AR1 IS 3X3 MATRIX
    // asm 00009670: 	PUSH	R2
    // asm 00009671: 	PUSHF	R2
    // asm 00009672: 	MPYF	*AR2++,*AR1++,R0
    // asm 00009673: 	MPYF	*AR2,*AR1++,R2
    // asm 00009674: 	MPYF	*+AR2(1),*AR1++,R0
    // asm 00009674:  || 	ADDF	R0,R2
    // asm 00009675: 	MPYF	*-AR2(1),*AR1++,R0
    // asm 00009675:  ||	ADDF	R0,R2
    // asm 00009676: 	PUSHF	R2
    // asm 00009677: 	MPYF	*AR2,*AR1++,R2
    // asm 00009678: 	MPYF	*+AR2(1),*AR1++,R0
    // asm 00009678:  || 	ADDF	R0,R2
    // asm 00009679: 	MPYF	*-AR2(1),*AR1++,R0
    // asm 00009679:  ||	ADDF	R0,R2
    // asm 0000967A: 	PUSHF	R2
    // asm 0000967B: 	MPYF	*+AR2(1),*+AR1(1),R2
    // asm 0000967C: 	MPYF	*AR2--(1),*AR1,R0
    // asm 0000967C:  || 	ADDF	R0,R2
    // asm 0000967D: 	LDI	R3,AR1
    // asm 0000967E: 	ADDF	R2,R0
    // asm 0000967F: 	POPF	R2
    // asm 00009680: 	STF	R0,*+AR1(2)
    // asm 00009681: 	POPF	R0
    // asm 00009682: 	STF	R0,*AR1
    // asm 00009682:  ||	STF	R2,*+AR1(1)
    // asm 00009683: 	POPF	R2
    // asm 00009684: 	POP	R2
    // asm 00009685: 	POP	AR1
    // asm 00009686: 	POPF	R0
    // asm 00009687: 	POP	R0
    // asm 00009688: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MATRIX_MUL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*void	normalize(VECTOR *V)
*/
void NORMALIZE(void)
{
    // asm 00009689: 	PUSH	R0
    // asm 0000968A: 	PUSH	R1
    // asm 0000968B: 	PUSHF	R0
    // asm 0000968C: 	PUSHF	R1
    // asm 0000968D: 	CALL	VECTLEN
    // asm 0000968E: 	CALL	INV_F30
    // asm 0000968F: 	MPYF	*AR2,R0,R1
    // asm 00009690: 	STF	R1,*AR2++
    // asm 00009691: 	MPYF	*AR2,R0,R1
    // asm 00009692: 	STF	R1,*AR2++
    // asm 00009693: 	MPYF	*AR2,R0,R1
    // asm 00009694: 	STF	R1,*AR2--(2)
    // asm 00009695: 	POPF	R1
    // asm 00009696: 	POPF	R0
    // asm 00009697: 	POP	R1
    // asm 00009698: 	POP	R0
    // asm 00009699: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NORMALIZE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*NORMAT		NORMALIZE MATRIX
*
*PARAMETERS
*	AR2	POINTS TO MATRIX
*
*AR0,AR1,R0,R1,R2 TRASHED
*
*/
void NORMAT(void)
{
    // 	;NORMALIZE ROWS
    // asm 0000969A: 	LDI	2,RC
    // asm 0000969B: 	RPTB	NORMROW
    // 	;GET LENGTH
    // asm 0000969C: 	LDF	*AR2++(3),R2
    // asm 0000969D: 	MPYF	R2,R2
    // asm 0000969E: 	LDF	*AR2++(3),R1
    // asm 0000969F: 	MPYF	R1,R1
    // asm 000096A0: 	ADDF	R1,R2
    // asm 000096A1: 	LDF	*AR2--(6),R1
    // asm 000096A2: 	MPYF	R1,R1
    // asm 000096A3: 	ADDF	R1,R2
    // asm 000096A4: 	CALL	SQRT	 	;sqrt(R2)-->R0
    // asm 000096A5: 	CALL	INV_F30		;INV(R0)-->R0
    // asm 000096A6: 	MPYF	*AR2,R0,R1
    // asm 000096A7: 	STF	R1,*AR2++(3)
    // asm 000096A8: 	MPYF	*AR2,R0,R1
    // asm 000096A9: 	STF	R1,*AR2++(3)
    // asm 000096AA: 	MPYF	*AR2,R0,R1
NORMROW:
    // asm 000096AB: STF	R1,*AR2--(5)
    // asm 000096AC: 	NOP	*AR2--(3)	;RESTORE AR2 MATRIX POINTER
    // 	;NORMALIZE COLUMNS
    // asm 000096AD: 	CALL	NORMALIZE
    // asm 000096AE: 	NOP	*AR2++(3)
    // asm 000096AF: 	CALL	NORMALIZE
    // asm 000096B0: 	NOP	*AR2++(3)
    // asm 000096B1: 	CALL	NORMALIZE
    // asm 000096B2: 	NOP	*AR2--(6)	;RESTORE AR2
    // asm 000096B3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NORMAT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*void	gen_normal(VECTOR *A,VECTOR *B,VECTOR *C,VECTOR *N)
*
*PARAMETERS
*	AR2	PTS TO PTRA,PTRB,PTRC
*	AR0	PTS TO NORMAL VECTOR TO RETURN
*
*CLOBBERS
*	R0-R7,AR2
*
*/
void GEN_NORMAL(void)
{
    // asm 000096B4: 	PUSH	AR0
    // asm 000096B5: 	LDI	*+AR2(1),AR0		;B
    // asm 000096B6: 	LDI	*+AR2(2),R3		;C
    // asm 000096B7: 	LDI	*AR2,AR2		;A
    // asm 000096B8: 	SUBF	*AR0++,*AR2++,R0	;D
    // asm 000096B9: 	SUBF	*AR0,*AR2++,R1
    // asm 000096BA: 	SUBF	*+AR0(1),*AR2++,R2
    // asm 000096BB: 	LDI	R3,AR2			;C
    // asm 000096BC: 	SUBF	*-AR0(1),*AR2++,R3	;E
    // asm 000096BD: 	SUBF	*AR0,*AR2++,R4
    // asm 000096BE: 	SUBF	*+AR0(1),*AR2++,R5
    // asm 000096BF: 	POP	AR0
    // asm 000096C0: 	MPYF	R1,R5,R6		;dy*ez
    // asm 000096C1: 	MPYF	R2,R4,R7		;dz*ey
    // asm 000096C2: 	SUBF	R7,R6
    // asm 000096C3: 	STF	R6,*AR0
    // asm 000096C4: 	MPYF	R2,R3,R6		;dz*ex
    // asm 000096C5: 	MPYF	R0,R5,R7		;dx*ez
    // asm 000096C6: 	SUBF	R7,R6
    // asm 000096C7: 	STF	R6,*+AR0(1)
    // asm 000096C8: 	MPYF	R0,R4,R6
    // asm 000096C9: 	MPYF	R1,R3,R7
    // asm 000096CA: 	SUBF	R7,R6
    // asm 000096CB: 	STF	R6,*+AR0(2)
    // asm 000096CC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GEN_NORMAL", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*CONCATENATE MATRICES, VOLATILE
*void	concatmat(MATRIX *s1, MATRIX *s2, MATRIX *d);
*
*PARAMETERS
*	AR2	s1 SOURCE_1 MATRIX
*	R2	s2 SOURCE_2 MATRIX
*	R3	d  DEST MATRIX
*
*NOTE	__d can NOT be equal to either __s1 or __s2.
*
*
*
*__s1	   __s2	    		__d
*A B C	   J K L    AJ+DK+GL BJ+EK+HL CJ+FK+IL
*D E F	*  M N O  = AM+DN+GO BM+EN+HO CM+FN+IO
*G H I     P Q R    AP+DQ+GR BP+EQ+HR CP+FQ+IR
*
*/
void CONCATMATV(void)
{
    // asm 000096CD: 	LDI	R2,AR0
    // asm 000096CE: 	LDI	R3,AR1
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CONCATMATV", 0, 0);
    UNIMPL();
}

void CONCAT201(void)
{
    // asm 000096CF: 	LDI	5,IR1
    // asm 000096D0: 	LDI	3,IR0
    // asm 000096D1: 	LDI	2,RC
    // asm 000096D2: 	RPTB	INLP2
    // *LOOP THREE TIMES
    // asm 000096D3: 	MPYF3	*AR0++,*AR2++(IR0),R0
    // asm 000096D4: 	MPYF3	*AR0,*AR2++(IR0),R1
    // asm 000096D5: 	MPYF3	*+AR0(1),*AR2--(IR1),R1
    // asm 000096D5:   ||	ADDF3	R0,R1,R2
    // asm 000096D6: 	MPYF3	*-AR0(1),*AR2++(IR0),R0
    // asm 000096D6:   ||	ADDF3	R1,R2,R2
    // asm 000096D7: 	MPYF3	*AR0,*AR2++(IR0),R1
    // asm 000096D8: 	STF	R2,*AR1++(1)			;store MATij
    // asm 000096D9: 	MPYF3	*+AR0(1),*AR2--(IR1),R1
    // asm 000096D9:   ||	ADDF3	R0,R1,R2
    // asm 000096DA: 	MPYF3	*-AR0(1),*AR2++(IR0),R0
    // asm 000096DA:   ||	ADDF3	R1,R2,R2
    // asm 000096DB: 	MPYF3	*AR0++,*AR2++(IR0),R1
    // asm 000096DC: 	STF	R2,*AR1++(1)			;store MATij
    // asm 000096DD: 	MPYF3	*AR0++,*AR2--(IR1),R1
    // asm 000096DD:   ||	ADDF3	R0,R1,R2
    // asm 000096DE: 	ADDF	R1,R2
    // asm 000096DF: 	STF	R2,*AR1++(1)			;store MATij
INLP2:
    // asm 000096E0: SUBI	3,AR2
    // asm 000096E1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CONCAT201", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*CONCATENATE MATRICES
*
*this is the non-volatile concatmat,  if the destination is not the same as
*either source use concatmatv because it doesn't do the extra copy (for
*avoiding overwrites)
*
*void	concatmat(MATRIX *s1,MATRIX *s2,MATRIX *d);
*
*PARAMETERS
*	AR2	s1 SOURCE_1 MATRIX
*	R2	s2 SOURCE_2 MATRIX
*	R3	d  DEST MATRIX
*
*
*/
void CONCATMAT(void)
{
    // asm 000096E2: 	PUSH	AR0
    // asm 000096E3: 	PUSH	AR1
    // asm 000096E4: 	PUSH	AR0
    // asm 000096E5: 	PUSH	R1
    // asm 000096E6: 	PUSH	R2
    // asm 000096E7: 	PUSH	R3
    // asm 000096E8: 	PUSH	R3
    // asm 000096E9: 	LDI	@LOCTEMPER_MATI,R3	;from DIRQ
    // asm 000096EA: 	CALL	CONCATMATV
    // asm 000096EB: 	POP	AR2
    // asm 000096EC: 	LDI	@LOCTEMPER_MATI,R2
    // asm 000096ED: 	CALL	CPYMAT
    // asm 000096EE: 	POP	R3
    // asm 000096EF: 	POP	R2
    // asm 000096F0: 	POP	R1
    // asm 000096F1: 	POP	R0
    // asm 000096F2: 	POP	AR1
    // asm 000096F3: 	POP	AR0
    // asm 000096F4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CONCATMAT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*GETTHETADIFF
*compute radY delta (as smallest possible turn)
*
*PARAMETERS
*	R0	DESIRED THETA (float)
*	R2	CURRENT THETA (float)
*RETURNS
*	R0	THETA DELTA (float)
*
*/
void GETTHETADIFF(void)
{
    // asm 000096F5: 	PUSHF	R1
    // asm 000096F6: 	SUBF	R2,R0
    // asm 000096F7: 	ABSF	R0,R1
    // asm 000096F8: 	CMPF	@PII,R1
    // asm 000096F9: 	BLT	NONEG
    // asm 000096FA: 	LDF	R0,R0
    // asm 000096FB: 	BN	ISNEG
    // asm 000096FC: 	SUBF	@TWOPII,R0
    // asm 000096FD: 	POPF	R1
    // asm 000096FE: 	RETS
ISNEG:
    // asm 000096FF: ADDF	@TWOPII,R0
NONEG:
    // asm 00009700: POPF	R1
    // asm 00009701: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETTHETADIFF", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*
*	LDI	AR2,AR0
*	LDP	@VECTORBI
*	LDI	@VECTORBI,AR1
*	LDF	*+AR4(OPOSX),R0
*	STF	R0,*+AR1(X)
*	CLRF	R0
*	STF	R0,*+AR1(Y)
*	LDF	*+AR4(OPOSZ),R0
*	STF	R0,*+AR1(Z)
*
*	CALL	DIST_PT2LINE
*
*PARAMETERS
*	AR0	LINE EQUATION (2D) A B C
*	AR1	2D POINT [X Y]
*
*RETURNS
*	R0	FL	DISTANCE TO LINE (SIGNED)
*
*
*/
void DIST_PT2LINE(void)
{
    // asm 00009702: 	PUSH	R1
    // asm 00009703: 	PUSH	R2
    // asm 00009704: 	PUSHF	R1
    // asm 00009705: 	PUSHF	R2
    // asm 00009706: 	MPYF	*+AR1(X),*+AR0(A),R0
    // asm 00009707: 	MPYF	*+AR1(Y),*+AR0(B),R1
    // asm 00009708: 	ADDF	R0,R1
    // asm 00009709: 	ADDF	*+AR0(C),R1
    // asm 0000970A: 	MPYF	*+AR0(A),*+AR0(A),R0
    // asm 0000970B: 	MPYF	*+AR0(B),*+AR0(B),R2
    // asm 0000970C: 	ADDF	R0,R2
    // asm 0000970D: 	CALL	SQRT
    // asm 0000970E: 	LDF	R0,R2
    // asm 0000970F: 	LDF	R1,R0
    // asm 00009710: 	LDF	R2,R1
    // asm 00009711: 	CALL	DIV_F
    // asm 00009712: 	POPF	R2
    // asm 00009713: 	POPF	R1
    // asm 00009714: 	POP	R2
    // asm 00009715: 	POP	R1
    // asm 00009716: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIST_PT2LINE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*	LDI	@VECTORAI,AR0
*	LDI	@VECTORBI,AR1
*	CALL	GETLINE_EQ
*
*PARAMETERS
*	AR0	P1
*	AR1	P2
*	AR2	SPACE FOR LINE EQUATION
*
*RETURNS
*	AR2	VECTOR AS A B C
*
*/
void GETLINE_EQ_2D(void)
{
    // asm 00009717: 	PUSH	R0
    // asm 00009718: 	PUSH	R1
    // asm 00009719: 	PUSH	R2
    // asm 0000971A: 	PUSH	R3
    // asm 0000971B: 	PUSHF	R0
    // asm 0000971C: 	PUSHF	R1
    // asm 0000971D: 	PUSHF	R2
    // asm 0000971E: 	PUSHF	R3
    // asm 0000971F: 	SUBF	*+AR1(X),*+AR0(X),R0
    // asm 00009720: 	SUBF	*+AR1(Y),*+AR0(Y),R1
    // asm 00009721: 	MPYF	R0,*+AR1(X),R2
    // asm 00009722: 	MPYF	R1,*+AR1(Y),R3
    // asm 00009723: 	ADDF	R3,R2
    // asm 00009724: 	NEGF	R2
    // asm 00009725: 	STF	R0,*+AR2(A)
    // asm 00009726: 	STF	R1,*+AR2(B)
    // asm 00009727: 	STF	R2,*+AR2(C)
    // asm 00009728: 	POPF	R3
    // asm 00009729: 	POPF	R2
    // asm 0000972A: 	POPF	R1
    // asm 0000972B: 	POPF	R0
    // asm 0000972C: 	POP	R3
    // asm 0000972D: 	POP	R2
    // asm 0000972E: 	POP	R1
    // asm 0000972F: 	POP	R0
    // asm 00009730: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETLINE_EQ_2D", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*see Foley & van Damn 2ed pg215
*
*scale matrix	Sx  0   0
*		0   Sy  0
*		0   0   Sz
*
*Once a FIND_MATRIX is complete you can pump that matrix through this
*routine and the object will be scaled by R2.  (Obviously each of the
*X Y and Z elements can be scaled independantly, but this might be
*cumbersome, and it would be better suited for a different routine.
*
*Note that a 3x3 x 3x3 -> 3x3 matrix by the scale would be:
*
* ASx BSy CSz
* DSx ESy FSz  (by eliminating all the multiply by zeros)
* GSx HSy ISz
*
*PARAMETERS
*	AR2	MATRIX to be scaled
*	R2	SCALE FACTOR
*
*
*RETURNS
*	AR2	MATRIX modified for scaling
*
*
*/
void SCALE_MATRIX(void)
{
    // asm 00009731: 	PUSH	R3
    // asm 00009732: 	PUSHF	R3
    // asm 00009733: 	LDF	*+AR2(A00),R3
    // asm 00009734: 	MPYF	R2,R3
    // asm 00009735: 	STF	R3,*+AR2(A00)
    // asm 00009736: 	LDF	*+AR2(A01),R3
    // asm 00009737: 	MPYF	R2,R3
    // asm 00009738: 	STF	R3,*+AR2(A01)
    // asm 00009739: 	LDF	*+AR2(A02),R3
    // asm 0000973A: 	MPYF	R2,R3
    // asm 0000973B: 	STF	R3,*+AR2(A02)
    // asm 0000973C: 	LDF	*+AR2(A10),R3
    // asm 0000973D: 	MPYF	R2,R3
    // asm 0000973E: 	STF	R3,*+AR2(A10)
    // asm 0000973F: 	LDF	*+AR2(A11),R3
    // asm 00009740: 	MPYF	R2,R3
    // asm 00009741: 	STF	R3,*+AR2(A11)
    // asm 00009742: 	LDF	*+AR2(A12),R3
    // asm 00009743: 	MPYF	R2,R3
    // asm 00009744: 	STF	R3,*+AR2(A12)
    // asm 00009745: 	LDF	*+AR2(A20),R3
    // asm 00009746: 	MPYF	R2,R3
    // asm 00009747: 	STF	R3,*+AR2(A20)
    // asm 00009748: 	LDF	*+AR2(A21),R3
    // asm 00009749: 	MPYF	R2,R3
    // asm 0000974A: 	STF	R3,*+AR2(A21)
    // asm 0000974B: 	LDF	*+AR2(A22),R3
    // asm 0000974C: 	MPYF	R2,R3
    // asm 0000974D: 	STF	R3,*+AR2(A22)
    // asm 0000974E: 	POPF	R3
    // asm 0000974F: 	POP	R3
    // asm 00009750: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SCALE_MATRIX", 0, 0);
    UNIMPL();
}

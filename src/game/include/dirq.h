#ifndef DIRQ_H
#define DIRQ_H

/* Generated from asm/DIRQ.EQU. */
/*
 * DIRQ.EQU
 * COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * ALL INTERNAL RAM VARIABLES MUST BE DEFINED HERE, THIS .EQU IS NOT
 * AUTOMATICALLY INCLUDED IN FILES (ALPHA.H) THEREFORE TO REFERENCE
 * THESE VARIABLES YOU MUST ACTUALLY INCLUDE THIS FILE, OR ASSUME
 * THAT THESE VARIABLES ARE EXTERNAL.  - THE ONLY PROBLEM WITH ASSUMING
 * EXTERNAL VARIABLES IS THAT YOU CAN NOT USE AN EXTERNAL VARIABLE IN
 * ...
 */

#define _CAMERAPOS (RAM0+0) /* 1x3 Camera position vector (floating point) */
#define _CAMERAVEL (RAM0+3) /* 1x3 Camera velocity vector */
#define _CAMERARAD (RAM0+6) /* 1x3 Camera radians vector */
#define _CAMERAMATRIX (RAM0+9) /* 3x3 Camera rotation matrix */
#define _LIGHT (RAM0+18) /* 1x3 universe illumination vector */
#define TMPMAT (RAM0+21) /* 1x3 temp vector */
#define ROTATION_MATRIX (RAM0+24) /* 3x3 translator matrix */
#define TRANSVECTOR (RAM0+33) /* 1x3 translator vector */
#define LOCTEMPER_MAT (RAM0+36) /* 3x3 scratch matrix for other routines */
#define POSTERMATRIX (RAM0+45) /* 3x3 poster style matrix (X/Z) else Frnt face */
#define _MATRIXA (RAM0+55) /* 3x3 these are three 3x3 scratch matrixes */
#define _MATRIXB (RAM0+65) /* 3x3 they may NOT be used by any routine */
#define _MATRIXC (RAM0+75) /* 3x3 called via the interrupt */
#define _VECTORA (RAM0+85) /* 1x3 ditto here except 1x3s */
#define _VECTORB (RAM0+90) /* 1x3 */
#define _VECTORC (RAM0+95) /* 1x3 */
#define _VECTORD (RAM0+100) /* 1x3 */
#define CLIPRAM (RAM0+137) /* (320w) RECURSIVE CLIP STACK(320 WORDS) */
#define CLIPRAML 320
#define CLIPLIM (RAM0+137+320-80)
#define FASTSTK RAM1 /* 256 WORDS */
#define BLOWLIST (RAM1+256) /* TRANSFORMED POINTS STORED (256 vertices) (768 WORDS) */

#endif /* DIRQ_H */

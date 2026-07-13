#ifndef DIRQ_H
#define DIRQ_H

#include "../core/port.h"

/* Generated from asm/DIRQ.EQU. */

// DIRQ.EQU
// ALL INTERNAL RAM VARIABLES MUST BE DEFINED HERE, THIS .EQU IS NOT
// AUTOMATICALLY INCLUDED IN FILES (ALPHA.H) THEREFORE TO REFERENCE
// THESE VARIABLES YOU MUST ACTUALLY INCLUDE THIS FILE, OR ASSUME
// THAT THESE VARIABLES ARE EXTERNAL.  - THE ONLY PROBLEM WITH ASSUMING
// EXTERNAL VARIABLES IS THAT YOU CAN NOT USE AN EXTERNAL VARIABLE IN
// A .SET STATEMENT.
//
// INTERNAL RAM VARIABLES

// asm: _CAMERAPOS	.set	RAM0+0	;1x3 Camera position vector (floating point)
// #define _CAMERAPOS (RAM0+0) //1x3 Camera position vector (floating point)

// asm: _CAMERAVEL	.set	RAM0+3	;1x3 Camera velocity vector
// #define _CAMERAVEL (RAM0+3) //1x3 Camera velocity vector

// asm: _CAMERARAD	.set	RAM0+6	;1x3 Camera radians vector
// #define _CAMERARAD (RAM0+6) //1x3 Camera radians vector

// asm: _CAMERAMATRIX	.set	RAM0+9	;3x3 Camera rotation matrix
// #define _CAMERAMATRIX (RAM0 + 9) // 3x3 Camera rotation matrix

// asm: _LIGHT		.set	RAM0+18	;1x3 universe illumination vector
// #define _LIGHT (RAM0 + 18) // 1x3 universe illumination vector

// DIRQ PERCULIARITIES
//
// asm: TMPMAT		.set	RAM0+21	;1x3 temp vector
// #define TMPMAT (RAM0 + 21) // 1x3 temp vector

// asm: ROTATION_MATRIX	.set	RAM0+24	;3x3 translator matrix
// #define ROTATION_MATRIX (RAM0 + 24) // 3x3 translator matrix

// asm: TRANSVECTOR	.set	RAM0+33	;1x3 translator vector
// #define TRANSVECTOR (RAM0 + 33) // 1x3 translator vector

// asm: LOCTEMPER_MAT	.set	RAM0+36	;3x3 scratch matrix for other routines
// #define LOCTEMPER_MAT (RAM0 + 36) // 3x3 scratch matrix for other routines

// asm: POSTERMATRIX	.set	RAM0+45	;3x3 poster style matrix (X/Z) else Frnt face
// #define POSTERMATRIX (RAM0 + 45) // 3x3 poster style matrix (X/Z) else Frnt face

// USER SPACE
// temporary user scratch space
// NOT intended for system use.
//
// asm: _MATRIXA	.set	RAM0+55	;3x3 these are three 3x3 scratch matrixes
// #define _MATRIXA (RAM0 + 55) // 3x3 these are three 3x3 scratch matrixes

// asm: _MATRIXB	.set	RAM0+65	;3x3 they may NOT be used by any routine
// #define _MATRIXB (RAM0 + 65) // 3x3 they may NOT be used by any routine

// asm: _MATRIXC	.set	RAM0+75	;3x3 called via the interrupt
// #define _MATRIXC (RAM0 + 75) // 3x3 called via the interrupt

// asm: _VECTORA	.set	RAM0+85	;1x3 ditto here except 1x3s
// #define _VECTORA (RAM0 + 85) // 1x3 ditto here except 1x3s

// asm: _VECTORB	.set	RAM0+90	;1x3
// #define _VECTORB (RAM0 + 90) // 1x3

// asm: _VECTORC	.set	RAM0+95	;1x3
// #define _VECTORC (RAM0 + 95) // 1x3

// asm: _VECTORD	.set	RAM0+100 ;1x3
// #define _VECTORD (RAM0 + 100) // 1x3

// asm: CLIPRAM		.set	RAM0+137	;(320w) RECURSIVE CLIP STACK(320 WORDS)
// #define CLIPRAM (RAM0 + 137) //(320w) RECURSIVE CLIP STACK(320 WORDS)

// asm: CLIPRAML	.set	320
#define CLIPRAML 320

// asm: CLIPLIM		.set	RAM0+137+320-80	;
#define CLIPLIM (RAM0 + 137 + 320 - 80)

// next_entry   unknown	.set	RAM0+473+-???	;(551 ??? remain)

//
// RAM0+512 to RAM0+1023
// is now reserved for use in FBSS space
// see mac.equ FBSS .macro definition for use, basically to allocate a space
// in ON-CHIP RAM use the example
// FBSS	symbolname,size
//

//
// insert new variable definitions here
//

// EACH RAM BLOCK HAS 1024 WORDS

// asm: FASTSTK			.set	RAM1		;256 WORDS
#define FASTSTK RAM1 // 256 WORDS

// asm: BLOWLIST		.set	RAM1+256	;TRANSFORMED POINTS STORED (256 vertices) (768 WORDS)
// #define BLOWLIST (RAM1 + 256) // TRANSFORMED POINTS STORED (256 vertices) (768 WORDS)

#endif /* DIRQ_H */

#include "../core/cpu.h"
#include "../core/machine.h"

/*
 * Source module: asm/FONT.ASM
 */

static int _font1[399];

/*
*----------------------------------------------------------------------------
*FONT.ASM
*
*COPYRIGHT (C) 1994  BY TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*SLOW ROM
*
*/

/* asm: _font1 */
/* asm: 	.word	0001100b */
/* asm: 	.word	0111000b */
/* asm: 	.word	1110000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1110000b */
/* asm: 	.word	0111000b */
/* asm: 	.word	0001100b */
/* asm: 	 */
/* asm: 	.word	0011000b */
/* asm: 	.word	0001110b */
/* asm: 	.word	0000111b */
/* asm: 	.word	0000011b */
/* asm: 	.word	0000111b */
/* asm: 	.word	0001110b */
/* asm: 	.word	0011000b */
/* asm: 	 */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	 */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001000b */
/* asm: 	.word	0001000b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0001000b */
/* asm: 	.word	0001000b */
/* asm: 	.word	0000000b */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	 */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	 */
/* asm: 	.word	0000001b */
/* asm: 	.word	0000011b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1100000b */
/* asm: 	 */
/* asm: 	.word	0111100b	;0 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	 */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1111110b */
/* asm: 	 */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0011100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	0100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	 */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	0111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	1111110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	 */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.word	0111110b	;9 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000000b */
/* asm: 	 */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000100b */
/* asm: 	 */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0001100b */
/* asm: 	 */
/* asm: 	.word	0000000b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0000000b */
/* asm: 	 */
/* asm: 	.word	0110000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	 */
/* asm: 	.word	0011100b */
/* asm: 	.word	0110100b */
/* asm: 	.word	0000100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0011000b */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.word	0000001b */
/* asm: 	.word	0000011b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1100000b */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.word	1111100b	;A */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	 */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	 */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	 */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111110b */
/* asm: 	 */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	 */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1101110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111110b */
/* asm: 	 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	 */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	1100010b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1101100b */
/* asm: 	.word	1111000b */
/* asm: 	.word	1101100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100010b */
/* asm: 	 */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111110b */
/* asm: 	 */
/* asm: 	.word	1100011b */
/* asm: 	.word	1110111b */
/* asm: 	.word	1101011b */
/* asm: 	.word	1101011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1110110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1101110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	 */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	 */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1101110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011010b */
/* asm: 	 */
/* asm: 	 */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1111000b */
/* asm: 	.word	1101100b */
/* asm: 	.word	1100110b */
/* asm: 	 */
/* asm: 	.word	0111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	1111100b */
/* asm: 	 */
/* asm: 	.word	1111110b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	 */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	0110110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0011100b */
/* asm: 	.word	0011100b */
/* asm: 	 */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1101011b */
/* asm: 	.word	1111111b */
/* asm: 	.word	1111111b */
/* asm: 	.word	1110111b */
/* asm: 	.word	1100011b */
/* asm: 	 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	 */
/* asm: 	.word	1111110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111110b */
/* asm: 	 */
/* asm: 	.word	0111000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0111000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000011b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0111000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111000b */
/* asm: 	.word	0010000b */
/* asm: 	.word	0101000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0100000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0010000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	 */
static int _font1[] = {
    12,
    56,
    112,
    96,
    112,
    56,
    12,
    24,
    14,
    7,
    3,
    7,
    14,
    24,
    102,
    60,
    24,
    126,
    24,
    60,
    102,
    0,
    8,
    8,
    62,
    8,
    8,
    0,
    0,
    0,
    0,
    0,
    12,
    12,
    24,
    0,
    0,
    0,
    62,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    12,
    12,
    1,
    3,
    6,
    12,
    24,
    48,
    96,
    60, // 0
    102,
    102,
    102,
    102,
    102,
    60,
    12,
    28,
    12,
    12,
    12,
    12,
    12,
    60,
    102,
    6,
    12,
    24,
    48,
    126,
    60,
    102,
    6,
    28,
    6,
    102,
    60,
    38,
    102,
    102,
    126,
    6,
    6,
    6,
    126,
    96,
    96,
    60,
    6,
    6,
    60,
    62,
    96,
    96,
    124,
    102,
    102,
    60,
    126,
    6,
    6,
    6,
    6,
    6,
    6,
    60,
    102,
    102,
    60,
    102,
    102,
    60,
    62, // 9
    102,
    102,
    62,
    6,
    6,
    6,
    0,
    12,
    12,
    0,
    12,
    12,
    0,
    0,
    12,
    12,
    0,
    12,
    12,
    4,
    12,
    24,
    48,
    96,
    48,
    24,
    12,
    0,
    62,
    62,
    0,
    62,
    62,
    0,
    48,
    24,
    12,
    6,
    12,
    24,
    48,
    28,
    52,
    4,
    12,
    24,
    0,
    24,
    1,
    3,
    6,
    12,
    24,
    48,
    96,
    // 	.word	0011100b
    // 	.word	1100110b
    // 	.word	1000110b
    // 	.word	1011110b
    // 	.word	1001100b
    // 	.word	1100000b
    // 	.word	0111110b
    124, // A
    102,
    102,
    126,
    102,
    102,
    102,
    124,
    102,
    102,
    124,
    102,
    102,
    124,
    60,
    102,
    96,
    96,
    96,
    102,
    60,
    124,
    102,
    102,
    102,
    102,
    102,
    124,
    126,
    96,
    96,
    120,
    96,
    96,
    126,
    126,
    96,
    96,
    120,
    96,
    96,
    96,
    60,
    102,
    96,
    96,
    110,
    102,
    62,
    102,
    102,
    102,
    126,
    102,
    102,
    102,
    60,
    24,
    24,
    24,
    24,
    24,
    60,
    6,
    6,
    6,
    6,
    6,
    102,
    60,
    98,
    102,
    108,
    120,
    108,
    102,
    98,
    96,
    96,
    96,
    96,
    96,
    96,
    126,
    99,
    119,
    107,
    107,
    99,
    99,
    99,
    102,
    118,
    126,
    126,
    110,
    102,
    102,
    60,
    102,
    102,
    102,
    102,
    102,
    60,
    124,
    102,
    102,
    124,
    96,
    96,
    96,
    24,
    60,
    102,
    110,
    102,
    60,
    26,
    124,
    102,
    102,
    124,
    120,
    108,
    102,
    62,
    96,
    96,
    60,
    6,
    6,
    124,
    126,
    24,
    24,
    24,
    24,
    24,
    24,
    102,
    102,
    102,
    102,
    102,
    102,
    60,
    99,
    99,
    99,
    54,
    62,
    28,
    28,
    99,
    99,
    107,
    127,
    127,
    119,
    99,
    102,
    102,
    60,
    24,
    60,
    102,
    102,
    102,
    102,
    60,
    24,
    24,
    24,
    24,
    126,
    6,
    12,
    24,
    48,
    96,
    126,
    56,
    48,
    48,
    48,
    48,
    48,
    56,
    96,
    48,
    24,
    12,
    6,
    3,
    0,
    56,
    24,
    24,
    24,
    24,
    24,
    56,
    16,
    40,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    126,
    32,
    48,
    16,
    0,
    0,
    0,
    0,
};

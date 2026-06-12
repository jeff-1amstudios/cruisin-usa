#include "../../core/cpu.h"
#include "../../core/machine.h"

/*
 * Source module: asm/FONT.ASM
 */

// *----------------------------------------------------------------------------
// *FONT.ASM
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *SLOW ROM
// *
/* asm: _font1	_font1 */
/* asm: 	.word	0001100b */
/* asm: 	.word	0111000b */
/* asm: 	.word	1110000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1110000b */
/* asm: 	.word	0111000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0001110b */
/* asm: 	.word	0000111b */
/* asm: 	.word	0000011b */
/* asm: 	.word	0000111b */
/* asm: 	.word	0001110b */
/* asm: 	.word	0011000b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001000b */
/* asm: 	.word	0001000b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0001000b */
/* asm: 	.word	0001000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000001b */
/* asm: 	.word	0000011b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0111100b	;0 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0011100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0111110b	;9 */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	0011100b */
/* asm: 	.word	0110100b */
/* asm: 	.word	0000100b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0000000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0000001b */
/* asm: 	.word	0000011b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1100000b */
/* asm: 	;	.word	0011100b */
/* asm: 	;	.word	1100110b */
/* asm: 	;	.word	1000110b */
/* asm: 	;	.word	1011110b */
/* asm: 	;	.word	1001100b */
/* asm: 	;	.word	1100000b */
/* asm: 	;	.word	0111110b */
/* asm: 	.word	1111100b	;A */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1101110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100010b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1101100b */
/* asm: 	.word	1111000b */
/* asm: 	.word	1101100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100010b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1110111b */
/* asm: 	.word	1101011b */
/* asm: 	.word	1101011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1110110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1111110b */
/* asm: 	.word	1101110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1101110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011010b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1111000b */
/* asm: 	.word	1101100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	1111100b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	0110110b */
/* asm: 	.word	0111110b */
/* asm: 	.word	0011100b */
/* asm: 	.word	0011100b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1101011b */
/* asm: 	.word	1111111b */
/* asm: 	.word	1111111b */
/* asm: 	.word	1110111b */
/* asm: 	.word	1100011b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0111100b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	1100110b */
/* asm: 	.word	0111100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0011000b */
/* asm: 	.word	1111110b */
/* asm: 	.word	0000110b */
/* asm: 	.word	0001100b */
/* asm: 	.word	0011000b */
/* asm: 	.word	0110000b */
/* asm: 	.word	1100000b */
/* asm: 	.word	1111110b */
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
int _font1[] = {
    0001100b,
    0111000b,
    1110000b,
    1100000b,
    1110000b,
    0111000b,
    0001100b,
    0011000b,
    0001110b,
    0000111b,
    0000011b,
    0000111b,
    0001110b,
    0011000b,
    1100110b,
    0111100b,
    0011000b,
    1111110b,
    0011000b,
    0111100b,
    1100110b,
    0000000b,
    0001000b,
    0001000b,
    0111110b,
    0001000b,
    0001000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0001100b,
    0001100b,
    0011000b,
    0000000b,
    0000000b,
    0000000b,
    0111110b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0001100b,
    0001100b,
    0000001b,
    0000011b,
    0000110b,
    0001100b,
    0011000b,
    0110000b,
    1100000b,
    0111100b, // 0
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    0111100b,
    0001100b,
    0011100b,
    0001100b,
    0001100b,
    0001100b,
    0001100b,
    0001100b,
    0111100b,
    1100110b,
    0000110b,
    0001100b,
    0011000b,
    0110000b,
    1111110b,
    0111100b,
    1100110b,
    0000110b,
    0011100b,
    0000110b,
    1100110b,
    0111100b,
    0100110b,
    1100110b,
    1100110b,
    1111110b,
    0000110b,
    0000110b,
    0000110b,
    1111110b,
    1100000b,
    1100000b,
    0111100b,
    0000110b,
    0000110b,
    0111100b,
    0111110b,
    1100000b,
    1100000b,
    1111100b,
    1100110b,
    1100110b,
    0111100b,
    1111110b,
    0000110b,
    0000110b,
    0000110b,
    0000110b,
    0000110b,
    0000110b,
    0111100b,
    1100110b,
    1100110b,
    0111100b,
    1100110b,
    1100110b,
    0111100b,
    0111110b, // 9
    1100110b,
    1100110b,
    0111110b,
    0000110b,
    0000110b,
    0000110b,
    0000000b,
    0001100b,
    0001100b,
    0000000b,
    0001100b,
    0001100b,
    0000000b,
    0000000b,
    0001100b,
    0001100b,
    0000000b,
    0001100b,
    0001100b,
    0000100b,
    0001100b,
    0011000b,
    0110000b,
    1100000b,
    0110000b,
    0011000b,
    0001100b,
    0000000b,
    0111110b,
    0111110b,
    0000000b,
    0111110b,
    0111110b,
    0000000b,
    0110000b,
    0011000b,
    0001100b,
    0000110b,
    0001100b,
    0011000b,
    0110000b,
    0011100b,
    0110100b,
    0000100b,
    0001100b,
    0011000b,
    0000000b,
    0011000b,
    0000001b,
    0000011b,
    0000110b,
    0001100b,
    0011000b,
    0110000b,
    1100000b,
    1111100b, // A
    1100110b,
    1100110b,
    1111110b,
    1100110b,
    1100110b,
    1100110b,
    1111100b,
    1100110b,
    1100110b,
    1111100b,
    1100110b,
    1100110b,
    1111100b,
    0111100b,
    1100110b,
    1100000b,
    1100000b,
    1100000b,
    1100110b,
    0111100b,
    1111100b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    1111100b,
    1111110b,
    1100000b,
    1100000b,
    1111000b,
    1100000b,
    1100000b,
    1111110b,
    1111110b,
    1100000b,
    1100000b,
    1111000b,
    1100000b,
    1100000b,
    1100000b,
    0111100b,
    1100110b,
    1100000b,
    1100000b,
    1101110b,
    1100110b,
    0111110b,
    1100110b,
    1100110b,
    1100110b,
    1111110b,
    1100110b,
    1100110b,
    1100110b,
    0111100b,
    0011000b,
    0011000b,
    0011000b,
    0011000b,
    0011000b,
    0111100b,
    0000110b,
    0000110b,
    0000110b,
    0000110b,
    0000110b,
    1100110b,
    0111100b,
    1100010b,
    1100110b,
    1101100b,
    1111000b,
    1101100b,
    1100110b,
    1100010b,
    1100000b,
    1100000b,
    1100000b,
    1100000b,
    1100000b,
    1100000b,
    1111110b,
    1100011b,
    1110111b,
    1101011b,
    1101011b,
    1100011b,
    1100011b,
    1100011b,
    1100110b,
    1110110b,
    1111110b,
    1111110b,
    1101110b,
    1100110b,
    1100110b,
    0111100b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    0111100b,
    1111100b,
    1100110b,
    1100110b,
    1111100b,
    1100000b,
    1100000b,
    1100000b,
    0011000b,
    0111100b,
    1100110b,
    1101110b,
    1100110b,
    0111100b,
    0011010b,
    1111100b,
    1100110b,
    1100110b,
    1111100b,
    1111000b,
    1101100b,
    1100110b,
    0111110b,
    1100000b,
    1100000b,
    0111100b,
    0000110b,
    0000110b,
    1111100b,
    1111110b,
    0011000b,
    0011000b,
    0011000b,
    0011000b,
    0011000b,
    0011000b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    0111100b,
    1100011b,
    1100011b,
    1100011b,
    0110110b,
    0111110b,
    0011100b,
    0011100b,
    1100011b,
    1100011b,
    1101011b,
    1111111b,
    1111111b,
    1110111b,
    1100011b,
    1100110b,
    1100110b,
    0111100b,
    0011000b,
    0111100b,
    1100110b,
    1100110b,
    1100110b,
    1100110b,
    0111100b,
    0011000b,
    0011000b,
    0011000b,
    0011000b,
    1111110b,
    0000110b,
    0001100b,
    0011000b,
    0110000b,
    1100000b,
    1111110b,
    0111000b,
    0110000b,
    0110000b,
    0110000b,
    0110000b,
    0110000b,
    0111000b,
    1100000b,
    0110000b,
    0011000b,
    0001100b,
    0000110b,
    0000011b,
    0000000b,
    0111000b,
    0011000b,
    0011000b,
    0011000b,
    0011000b,
    0011000b,
    0111000b,
    0010000b,
    0101000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
    1111110b,
    0100000b,
    0110000b,
    0010000b,
    0000000b,
    0000000b,
    0000000b,
    0000000b,
};

void font(void)
{
    UNIMPL();
}

#ifndef TEXT_H
#define TEXT_H

/* Generated from asm/TEXT.EQU. */

// TEXT.EQU
// 
// COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// ALL RIGHTS RESERVED
// 
// ROUTINES

// DATA STRUCTURES

// STRUCT FONTENTRY
// asm: FONTENT_PRE	.set	0	;preceding pixel spaces (low 16 bits)
#define FONTENT_PRE 0 //preceding pixel spaces (low 16 bits)
// asm: FONTENT_TRAIL	.set	0	;trailing pixel spaces (high 16 bits)
#define FONTENT_TRAIL 0 //trailing pixel spaces (high 16 bits)
// asm: FONTENT_XSTART	.set	1
#define FONTENT_XSTART 1
// asm: FONTENT_XEND	.set	2
#define FONTENT_XEND 2
// asm: FONTENT_YSTART	.set	3
#define FONTENT_YSTART 3
// asm: FONTENT_SIZE	.set	4
#define FONTENT_SIZE 4
// ENDSTRUCT

// ----------------------------------------------------------------------------
// TEXT OUTPUT STRUCTURE  (TEXT.ASM)
// 
// STRUCT TEXT
// asm: TEXT_LINK	.set	00	;UH	dynamic link to next structure
#define TEXT_LINK 00 //UH	dynamic link to next structure
// asm: TEXT_PTR 	.set	01	;UH	pointer to the compacted text
#define TEXT_PTR 01 //UH	pointer to the compacted text
// asm: TEXT_FONT	.set	02	;UH	point to the font table
#define TEXT_FONT 02 //UH	point to the font table
// asm: TEXT_POSX	.set	03	;FL	position X
#define TEXT_POSX 03 //FL	position X
// asm: TEXT_POSY	.set	04	;FL	position Y
#define TEXT_POSY 04 //FL	position Y
// asm: TEXT_VELX	.set	05	;FL	velocity X
#define TEXT_VELX 05 //FL	velocity X
// asm: TEXT_VELY	.set	06	;FL	velocity Y
#define TEXT_VELY 06 //FL	velocity Y
// asm: TEXT_TIKS	.set	07	;UH	number of tiks to removal
#define TEXT_TIKS 07 //UH	number of tiks to removal
// asm: TEXT_COLOR	.set	08	;UH	color of text and flags
#define TEXT_COLOR 08 //UH	color of text and flags
// asm: TEXT_HEIGHT	.set	09	;UH	height of the image (DEFAULT = 28)
#define TEXT_HEIGHT 09 //UH	height of the image (DEFAULT = 28)
// asm: TEXT_ADDR	.set	10	;UH	text dBASE
#define TEXT_ADDR 10 //UH	text dBASE
// asm: TEXT_IMG	.set	11	;UH	image addr
#define TEXT_IMG 11 //UH	image addr
// asm: TEXT_PAL	.set	12	;
#define TEXT_PAL 12
// asm: TEXT_SIZ	.set	13	;SIZ	size of text structure
#define TEXT_SIZ 13 //SIZ	size of text structure
// ENDSTRUCT
// asm: NUM_TEXTS	.set	160
#define NUM_TEXTS 160
// 
// TEXT FLAGS
// 
// Flags are stored in upper 24 bits of TEXT_COLOR entry.
// 
// asm: TXT_CENTER	.set	0100h	;center the text
#define TXT_CENTER 0x0100 //center the text
// asm: TXT_RIGHT	.set	0200h	;right justify the text
#define TXT_RIGHT 0x0200 //right justify the text
// asm: TXT_NOPULL	.set	0400h	;remain onscreen - do not decrement tiks
#define TXT_NOPULL 0x0400 //remain onscreen - do not decrement tiks
// asm: TXT_NRZ		.set	0800h	;replace all nonzero colors with COLOR
#define TXT_NRZ 0x0800 //replace all nonzero colors with COLOR
// asm: TXT_COLOR	.set	00FFh	;the color for NZR
#define TXT_COLOR 0x00FF //the color for NZR
// ----------------------------------------------------------------------------

#endif /* TEXT_H */

#ifndef TEXT_H
#define TEXT_H

/* Generated from asm/TEXT.EQU. */
/*
 * TEXT.EQU
 * COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * ROUTINES
 * DATA STRUCTURES
 * STRUCT FONTENTRY
 * ENDSTRUCT
 * ----------------------------------------------------------------------------
 * ...
 */

/* Original .globl symbols in this module:
 *   TEXT_INIT
 *   TEXT_ADDDS
 *   TEXT_ADD1
 *   TEXT_ADD
 *   TEXT_OUTPUT
 *   STRCPY
 *   STRCAT
 *   SETFIXEDFONTDS
 *   SETFIXEDFONT
 *   SETFIXEDFONT15DS
 *   SET18FONT
 *   SET18FONTDS
 *   SET40FONT
 *   SET40FONTDS
 *   SET12FONT
 *   SET12FONTDS
 *   SETN43FONT
 *   SETN43FONTDS
 *   SETLGDIGITFONT
 *   SETLGDIGITFONTDS
 *   SETSMDIGITFONT
 *   SETSMDIGITFONTDS
 *   TEXT_ACTIVEI
 *   TEXT_FREE_COUNT
 *   TEXT_ACTIVEI
 *   HIGHLIGHTN
 *   STRCAT
 *   STRCPY
 *   TEXT_FREEI
 *   TEXT_FREEZE
 */

#define FONTENT_PRE 0 /* preceding pixel spaces (low 16 bits) */
#define FONTENT_TRAIL 0 /* trailing pixel spaces (high 16 bits) */
#define FONTENT_XSTART 1
#define FONTENT_XEND 2
#define FONTENT_YSTART 3
#define FONTENT_SIZE 4
#define TEXT_LINK 00 /* UH	dynamic link to next structure */
#define TEXT_PTR 01 /* UH	pointer to the compacted text */
#define TEXT_FONT 02 /* UH	point to the font table */
#define TEXT_POSX 03 /* FL	position X */
#define TEXT_POSY 04 /* FL	position Y */
#define TEXT_VELX 05 /* FL	velocity X */
#define TEXT_VELY 06 /* FL	velocity Y */
#define TEXT_TIKS 07 /* UH	number of tiks to removal */
#define TEXT_COLOR 08 /* UH	color of text and flags */
#define TEXT_HEIGHT 09 /* UH	height of the image (DEFAULT = 28) */
#define TEXT_ADDR 10 /* UH	text dBASE */
#define TEXT_IMG 11 /* UH	image addr */
#define TEXT_PAL 12
#define TEXT_SIZ 13 /* SIZ	size of text structure */
#define NUM_TEXTS 160
#define TXT_CENTER 0x0100 /* center the text */
#define TXT_RIGHT 0x0200 /* right justify the text */
#define TXT_NOPULL 0x0400 /* remain onscreen - do not decrement tiks */
#define TXT_NRZ 0x0800 /* replace all nonzero colors with COLOR */
#define TXT_COLOR 0x00FF /* the color for NZR */

#endif /* TEXT_H */

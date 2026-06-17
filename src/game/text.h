#ifndef TEXT_H
#define TEXT_H

/* Generated from asm/TEXT.EQU. */

// TEXT.EQU
// 
// COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// ALL RIGHTS RESERVED
// 
// ROUTINES

// ----------------------------------------------------------------------------
// TEXT OUTPUT STRUCTURE  (TEXT.ASM)
// 
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

// DATA STRUCTURES
// asm: 	.globl	TEXT_ACTIVEI
#define TEXT_ACTIVEI TEXT_ACTIVE

// asm: 	.globl	TEXT_FREE_COUNT
extern int TEXT_FREE_COUNT;

// asm: 	.globl	TEXT_FREEI
#define TEXT_FREEI TEXT_FREE

// asm: 	.globl	TEXT_FREEZE
extern int TEXT_FREEZE;

// asm: 	.globl	TEXT_INIT
void TEXT_INIT(void);

// asm: 	.globl	TEXT_ADDDS
void TEXT_ADDDS(void);

// asm: 	.globl	TEXT_ADD1
void TEXT_ADD1(void);

// asm: 	.globl	TEXT_ADD
void TEXT_ADD(void);

// asm: 	.globl	TEXT_OUTPUT
void TEXT_OUTPUT(void);

// asm: 	.globl	STRCPY
void STRCPY(void);

// asm: 	.globl	STRCAT
void STRCAT(void);

// asm: 	.globl	SETFIXEDFONTDS,SETFIXEDFONT
void SETFIXEDFONTDS(void);

void SETFIXEDFONT(void);

// asm: 	.globl	SET18FONT,SET18FONTDS
void SET18FONT(void);

void SET18FONTDS(void);

// asm: 	.globl	SET40FONT,SET40FONTDS
void SET40FONT(void);

void SET40FONTDS(void);

// asm: 	.globl	SET12FONT,SET12FONTDS
void SET12FONT(void);

void SET12FONTDS(void);

// asm: 	.globl	SETN43FONT,SETN43FONTDS
void SETN43FONT(void);

void SETN43FONTDS(void);

// asm: 	.globl	SETLGDIGITFONT,SETLGDIGITFONTDS
void SETLGDIGITFONT(void);

void SETLGDIGITFONTDS(void);

// asm: 	.globl	SETSMDIGITFONT,SETSMDIGITFONTDS
void SETSMDIGITFONT(void);

void SETSMDIGITFONTDS(void);

// asm: 	.globl	HIGHLIGHTN
void HIGHLIGHTN(void);

// TEXT.ASM
// asm: 	hibss	TEXT_LIST,NUM_TEXTS*TEXT_SIZ
extern int TEXT_LIST[];
// asm: 	.bss	TEXT_FREE,1
extern int TEXT_FREE;
// asm: 	.bss	TEXT_ACTIVE,1
extern int TEXT_ACTIVE;
// asm: 	.bss	TEXT_FREE_COUNT,1
extern int TEXT_FREE_COUNT;
// asm: 	.bss	TEXT_FREEZE,1
extern int TEXT_FREEZE;

#endif /* TEXT_H */

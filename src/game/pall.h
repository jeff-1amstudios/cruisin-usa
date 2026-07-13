#ifndef PALL_H
#define PALL_H

#include "../core/port.h"

/* Generated from asm/PALL.EQU. */

// PALL.EQU
// ROUTINES

//
// asm: PALNUM	.set	128
#define PALNUM 128

// asm: PALCNT 	.set	0
#define PALCNT 0

// asm: PALID	.set	1
#define PALID 1

// asm: PALTCNT	.set	0		   	;WORD COUNT
#define PALTCNT 0 // WORD COUNT

// asm: PALSRC	.set	1			;SOURCE ADDR
#define PALSRC 1 // SOURCE ADDR

// asm: PALDEST	.set	2			;DEST ADDR
#define PALDEST 2 // DEST ADDR

// asm: 	.globl	PTTRAM,PALLISTI
extern int PTTRAM[];

#define PALLISTI _PALLIST

// asm: 	.globl	PALROMI
#define PALROMI _PALROM

// asm: 	.globl	PAL_INIT		;CLEAR ALLOCATION/TRANSFER TABLES
void PAL_INIT(void);

// asm: 	.globl	PAL_ALLOC
tPALETTE_CODE PAL_ALLOC(u32 pal_index);

// asm: 	.globl	PAL_ALLOC_RAW
tPALETTE_CODE PAL_ALLOC_RAW(tPAL* palette_source);

// asm: 	.globl	PAL_FIND
int PAL_FIND(u32 pal_index);

// asm: 	.globl	PAL_FIND_RAW
int PAL_FIND_RAW(const tPAL* palette_source);

// asm: 	.globl	PAL_SET
void PAL_SET(uint32_t* src, uint32_t destPaletteColor, uint32_t count);

// asm: 	.globl	PAL_DELETE
void PAL_DELETE(int palette_code /*AR2*/);

// asm: 	.globl	PAL_DELETE_RAW
void PAL_DELETE_RAW(int actual_palette_index /*AR2*/);

// asm: 	.globl	PAL_DIMMER
void PAL_DIMMER(void);

// asm: 	.globl	PAL_XFER		;TRANSFER THE PALETTES IN TRANS-RAM
void PAL_XFER(void);

// asm: 	.globl	PAL_OVERWRITE
void PAL_OVERWRITE(void);

// PALL.ASM
// asm: 	.bss  	PALRAM,PALNUM		;ACTIVE PALETTES
extern int PALRAM[];
// asm: 	.bss	RAWLOCS,PALNUM
// extern int RAWLOCS[];
// asm: 	.bss	PTTRAM,PALNUM*3		;PALLETTE TRANSFER RAM
extern int PTTRAM[];
// asm: 	.bss	PALXFER_ACTIVE,1
extern PALXFER* PALXFER_ACTIVE;
// asm: 	.bss	PALXFER_FREE,1
extern PALXFER* PALXFER_FREE;
// asm: 	.bss	PALXFER_AVAILABLE_P,1
extern int PALXFER_AVAILABLE_P;
// asm: 	.bss	PALXFER_STR,PALX_SIZE*NXFER_PALS
extern PALXFER PALXFER_STR[];

#endif /* PALL_H */

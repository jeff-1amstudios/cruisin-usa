#ifndef PALL_H
#define PALL_H

/* Generated from asm/PALL.ASM. */

// asm: 	.bss  	PALRAM,PALNUM		;ACTIVE PALETTES
extern int PALRAM[PALNUM];
// asm: 	.bss	RAWLOCS,PALNUM
extern int RAWLOCS[PALNUM];
// asm: 	.bss	PTTRAM,PALNUM*3		;PALLETTE TRANSFER RAM
extern int PTTRAM[PALNUM*3];
// asm: 	.bss	PALXFER_ACTIVE,1
extern int PALXFER_ACTIVE;
// asm: 	.bss	PALXFER_FREE,1
extern int PALXFER_FREE;
// asm: 	.bss	PALXFER_AVAILABLE_P,1
extern int PALXFER_AVAILABLE_P;
// asm: 	.bss	PALXFER_STR,PALX_SIZE*NXFER_PALS
extern int PALXFER_STR[PALX_SIZE*NXFER_PALS];

#endif /* PALL_H */

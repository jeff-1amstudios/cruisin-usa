#ifndef PALL_H
#define PALL_H

/* Generated from asm/PALL.ASM. */

// asm: 	.bss  	PALRAM,PALNUM		;ACTIVE PALETTES
// addr: 0x0000C9BD
extern int PALRAM[PALNUM];
// asm: 	.bss	RAWLOCS,PALNUM
// addr: 0x0000CA3D
extern int RAWLOCS[PALNUM];
// asm: 	.bss	PTTRAM,PALNUM*3		;PALLETTE TRANSFER RAM
// addr: 0x0000CABD
extern int PTTRAM[PALNUM*3];
// asm: 	.bss	PALXFER_ACTIVE,1
// addr: 0x0000CC3F
extern int PALXFER_ACTIVE;
// asm: 	.bss	PALXFER_FREE,1
// addr: 0x0000CC40
extern int PALXFER_FREE;
// asm: 	.bss	PALXFER_AVAILABLE_P,1
// addr: 0x0000CC41
extern int PALXFER_AVAILABLE_P;
// asm: 	.bss	PALXFER_STR,PALX_SIZE*NXFER_PALS
// addr: 0x0000CC42
extern int PALXFER_STR[PALX_SIZE*NXFER_PALS];

#endif /* PALL_H */

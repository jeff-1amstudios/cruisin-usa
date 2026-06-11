#ifndef COLLA_H
#define COLLA_H

/* Generated from asm/COLLA.ASM. */

// asm: 	.bss	VL,4
// addr: 0x0000E5A7
extern int VL[4];
// asm: 	.bss	TNORM,3
// addr: 0x0000E5AB
extern int TNORM[3];
// asm: 	.bss	TVECT1,3
// addr: 0x0000E5AE
extern int TVECT1[3];
// asm: 	.bss	TVECT2,3
// addr: 0x0000E5B1
extern int TVECT2[3];
// asm: 	.bss	TMATRIX,9
// addr: 0x0000E5B4
extern int TMATRIX[9];
// asm: 	.bss	COLVEL,1	;RELATIVE COLLISION VELOCITY
// addr: 0x0000E5BD
extern int COLVEL;
// asm: 	.bss	PMULT,1  	;PLAYER SLOW COLLISION VELOCITY MULTIPLIER
// addr: 0x0000E5BE
extern int PMULT;
// asm: 	.bss	SPINTEMP,1
// addr: 0x0000E5BF
extern int SPINTEMP;
// asm: 	FBSS	BOXSCRAM,50
// addr: 0x008099E4
extern int BOXSCRAM[50];
// asm: 	.BSS	PLYRBEHIND,1
// addr: 0x0000E5C0
extern int PLYRBEHIND;

#endif /* COLLA_H */

#ifndef RACER_H
#define RACER_H

/* Generated from asm/RACER.ASM. */

// asm: 	.bss	FINISHNUM,1		;NUMBER OF DRONES FINISHED
// addr: 0x0000E8D2
extern int FINISHNUM;
// asm: 		.bss	RACER_PTR,10
// addr: 0x0000E8D3
extern int RACER_PTR[10];
// asm: 	.bss	OM_TRACK_LO,1
// addr: 0x0000E8DD
extern int OM_TRACK_LO;
// asm: 	.bss	OM_TRACK_HI,1
// addr: 0x0000E8DE
extern int OM_TRACK_HI;
// asm: 	.BSS	ROADOBSTAB,50  		;ARRAY OF ROAD OBSTACLES
// addr: 0x0000E8DF
extern int ROADOBSTAB[50];
// asm: 	.BSS	PLYRCLOSE,1		;CLOSING VELOCITY OF DRONE ON PLAYER
// addr: 0x0000E911
extern int PLYRCLOSE;
// asm: 	.BSS	CARTMP1,1
// addr: 0x0000E912
extern int CARTMP1;

#endif /* RACER_H */

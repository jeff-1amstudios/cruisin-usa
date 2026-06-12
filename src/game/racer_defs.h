#ifndef RACER_H
#define RACER_H

/* Generated from asm/RACER.ASM. */

// asm: 	.bss	FINISHNUM,1		;NUMBER OF DRONES FINISHED
extern int FINISHNUM;
// asm: 		.bss	RACER_PTR,10
extern int RACER_PTR[10];
// asm: 	.bss	OM_TRACK_LO,1
extern int OM_TRACK_LO;
// asm: 	.bss	OM_TRACK_HI,1
extern int OM_TRACK_HI;
// asm: 	.BSS	ROADOBSTAB,50  		;ARRAY OF ROAD OBSTACLES
extern int ROADOBSTAB[50];
// asm: 	.BSS	PLYRCLOSE,1		;CLOSING VELOCITY OF DRONE ON PLAYER
extern int PLYRCLOSE;
// asm: 	.BSS	CARTMP1,1
extern int CARTMP1;

#endif /* RACER_H */

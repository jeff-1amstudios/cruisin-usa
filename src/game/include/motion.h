#ifndef MOTION_H
#define MOTION_H

/* Generated from asm/MOTION.ASM. */

// asm: 	.bss	MOTION_NOT_ON,1
// addr: 0x0099EB7E
extern int MOTION_NOT_ON;
// asm: 	.bss	MOTION_STOP_HIT,1
// addr: 0x0000EB7F
extern int MOTION_STOP_HIT;
// asm: 	.bss	MOTION_RCV_TIKS,1
// addr: 0x0000EB80
extern int MOTION_RCV_TIKS;
// asm: 	.bss	ABORT_QUERY,1
// addr: 0x0000EB81
extern int ABORT_QUERY;
// asm: 	.bss	WAITTIK,1
// addr: 0x0000EB82
extern int WAITTIK;
// asm: 	.bss	MOTION_SAFETY_ON,1
// addr: 0x0000EB83
extern int MOTION_SAFETY_ON;
// asm: 	.bss	MOTION_SAFETY_TYPE,1
// addr: 0x0000EB84
extern int MOTION_SAFETY_TYPE;
// asm: 	.bss	sPOTVALUE,15
// addr: 0x0000EB85
extern int sPOTVALUE[15];
// asm: 	.bss	MOTION_CMDSTR,80
// addr: 0x0000EB94
extern int MOTION_CMDSTR[80];
// asm: 	.bss	LAST_ZF,1
// addr: 0x0099EBE4
extern int LAST_ZF;
// asm: 	.bss	LAST_XL,1
// addr: 0x0099EBE5
extern int LAST_XL;
// asm: 	.bss	LAST_YR,1
// addr: 0x0099EBE6
extern int LAST_YR;
// asm: 	.bss	LATCHED_ERROR,1
// addr: 0x0000EBE7
extern int LATCHED_ERROR;
// asm: 	.bss	GALIL_STATUS_X,1
// addr: 0x0000EBE8
extern int GALIL_STATUS_X;
// asm: 	.bss	GALIL_STATUS_Y,1
// addr: 0x0000EBE9
extern int GALIL_STATUS_Y;
// asm: 	.bss	GALIL_STATUS_Z,1
// addr: 0x0000EBEA
extern int GALIL_STATUS_Z;

#endif /* MOTION_H */

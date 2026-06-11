#ifndef OBJ_H
#define OBJ_H

/* Generated from asm/OBJ.ASM. */

// asm: 	.bss	OACTIVE,1		;OBJECT ACTIVE LIST
// addr: 0x0000C9B2
extern int OACTIVE;
// asm: 	.bss	OFREE,1		 	;OBJECT FREE LIST
// addr: 0x0000C9B3
extern int OFREE;
// asm: 	.bss	IDLE_LIST,1	 	;OBJECT idle elements list
// addr: 0x0000C9B4
extern int IDLE_LIST;
// asm: 	.bss	OACTIVE_PRIORITY,1	;OBJECT priority list
// addr: 0x0000C9B5
extern int OACTIVE_PRIORITY;
// asm: 	.bss	OLOW_PRIORITY,1		;OBJECT supplimental object list
// addr: 0x0000C9B6
extern int OLOW_PRIORITY;
// asm: 	.bss	OHIGH_PRIORITY,1	;OBJECT higest priority list
// addr: 0x0000C9B7
extern int OHIGH_PRIORITY;
// asm: 	.bss	OACTIVECNT,1		;STATISTICAL ANALYSIS ONLY
// addr: 0x0000C9B8
extern int OACTIVECNT;
// asm: 	.bss	OFREECNT,1
// addr: 0x0000C9B9
extern int OFREECNT;
// asm: 	.bss	OMAX_OBJECTS,1		;STATISTICAL ANALYSIS ONLY
// addr: 0x0000C9BA
extern int OMAX_OBJECTS;
// asm: 	hibss	OBJSTR,OBJSIZ*NUM_OBJECTS
// addr: 0x00010585
extern int OBJSTR[OBJSIZ*NUM_OBJECTS];
// asm: 	.bss	COMM_DRONE_PTR,1
// addr: 0x0000C9BB
extern int COMM_DRONE_PTR;
// asm:      	.BSS	PLYRTEMP,1		;PLAYER OBJECT STORE
// addr: 0x0000C9BC
extern int PLYRTEMP;

#endif /* OBJ_H */

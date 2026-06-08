#ifndef OBJ_H
#define OBJ_H

/* Generated from asm/OBJ.ASM. */

// asm: 	.bss	OACTIVE,1		;OBJECT ACTIVE LIST
#define OACTIVE 0x0000C9B2
// asm: 	.bss	OFREE,1		 	;OBJECT FREE LIST
#define OFREE 0x0000C9B3
// asm: 	.bss	IDLE_LIST,1	 	;OBJECT idle elements list
#define IDLE_LIST 0x0000C9B4
// asm: 	.bss	OACTIVE_PRIORITY,1	;OBJECT priority list
#define OACTIVE_PRIORITY 0x0000C9B5
// asm: 	.bss	OLOW_PRIORITY,1		;OBJECT supplimental object list
#define OLOW_PRIORITY 0x0000C9B6
// asm: 	.bss	OHIGH_PRIORITY,1	;OBJECT higest priority list
#define OHIGH_PRIORITY 0x0000C9B7
// asm: 	.bss	OACTIVECNT,1		;STATISTICAL ANALYSIS ONLY
#define OACTIVECNT 0x0000C9B8
// asm: 	.bss	OFREECNT,1
#define OFREECNT 0x0000C9B9
// asm: 	.bss	OMAX_OBJECTS,1		;STATISTICAL ANALYSIS ONLY
#define OMAX_OBJECTS 0x0000C9BA
// asm: 	hibss	OBJSTR,OBJSIZ*NUM_OBJECTS
#define OBJSTR 0x00010585
// asm: 	.bss	COMM_DRONE_PTR,1
#define COMM_DRONE_PTR 0x0000C9BB
// asm:      	.BSS	PLYRTEMP,1		;PLAYER OBJECT STORE
#define PLYRTEMP 0x0000C9BC

#endif /* OBJ_H */

#ifndef OBJ_H
#define OBJ_H

/* Generated from asm/OBJ.ASM. */

// asm: 	.bss	OACTIVE,1		;OBJECT ACTIVE LIST
extern int OACTIVE;
// asm: 	.bss	OFREE,1		 	;OBJECT FREE LIST
extern int OFREE;
// asm: 	.bss	IDLE_LIST,1	 	;OBJECT idle elements list
extern int IDLE_LIST;
// asm: 	.bss	OACTIVE_PRIORITY,1	;OBJECT priority list
extern int OACTIVE_PRIORITY;
// asm: 	.bss	OLOW_PRIORITY,1		;OBJECT supplimental object list
extern int OLOW_PRIORITY;
// asm: 	.bss	OHIGH_PRIORITY,1	;OBJECT higest priority list
extern int OHIGH_PRIORITY;
// asm: 	.bss	OACTIVECNT,1		;STATISTICAL ANALYSIS ONLY
extern int OACTIVECNT;
// asm: 	.bss	OFREECNT,1
extern int OFREECNT;
// asm: 	.bss	OMAX_OBJECTS,1		;STATISTICAL ANALYSIS ONLY
extern int OMAX_OBJECTS;
// asm: 	hibss	OBJSTR,OBJSIZ*NUM_OBJECTS
extern int OBJSTR[OBJSIZ*NUM_OBJECTS];
// asm: 	.bss	COMM_DRONE_PTR,1
extern int COMM_DRONE_PTR;
// asm:      	.BSS	PLYRTEMP,1		;PLAYER OBJECT STORE
extern int PLYRTEMP;

#endif /* OBJ_H */

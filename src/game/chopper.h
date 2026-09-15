#ifndef CHOPPER_H
#define CHOPPER_H

#include "../core/port.h"

/* Generated from asm/CHOPPER.ASM. */

// CHOPPER.ASM
// asm: 	.bss	HELI_ABORT,1	;P	abort chopper routine?
extern int HELI_ABORT;

void CHOPPER(PROC* p);
OBJ* GET_CLOSEST_TRAK(OBJ* obj);

#endif /* CHOPPER_H */

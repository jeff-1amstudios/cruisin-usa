#ifndef DRONES_H
#define DRONES_H

#include "port.h"

/* Generated from asm/DRONES.ASM. */

// DRONES.ASM
// asm: 	.bss	DRONE_DISPATCH_P,1	;FLAG dispatching active???
extern int DRONE_DISPATCH_P;
// asm: 	.bss	SUSPEND_MODE,1		;ALL DRONES *MUST* CHECK EVERY FRAME
extern int SUSPEND_MODE;
// asm: 	.bss	RACER_GRID_START,1	;Starting Index
extern int RACER_GRID_START;
// asm: 	.bss	DRONE_COUNT,1		;UD	NUMBER OF ACTIVE DRONES
extern int DRONE_COUNT;
// asm: 	.bss	ONCSCREEN_CARS,1
extern int ONCSCREEN_CARS;
// asm: 	.bss	DD_SLP,1	;MINIMUM SLEEP VALUE (def=40)
extern int DD_SLP;
// asm: 	.bss	DD_VAR,1	;SLEEP TIME VARIENCE (def=100)
extern int DD_VAR;
// asm: 	.bss	DD_MAX_DRONES,1	;MAX DRONES (def=MAX_DRONES)
extern int DD_MAX_DRONES;
// asm: 	.bss	DRONENUM,1
extern int DRONENUM;
// asm: 	.bss	SMOKE_COUNT,1
extern int SMOKE_COUNT;
// asm: 	.bss	COCONUT_COUNT,1
extern int COCONUT_COUNT;

#endif /* DRONES_H */

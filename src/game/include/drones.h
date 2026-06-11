#ifndef DRONES_H
#define DRONES_H

/* Generated from asm/DRONES.ASM. */

// asm: 	.bss	DRONE_DISPATCH_P,1	;FLAG dispatching active???
// addr: 0x0080E61B
extern int DRONE_DISPATCH_P;
// asm: 	.bss	SUSPEND_MODE,1		;ALL DRONES *MUST* CHECK EVERY FRAME
// addr: 0x0000E61C
extern int SUSPEND_MODE;
// asm: 	.bss	RACER_GRID_START,1	;Starting Index
// addr: 0x0000E61D
extern int RACER_GRID_START;
// asm: 	.bss	DRONE_COUNT,1		;UD	NUMBER OF ACTIVE DRONES
// addr: 0x0000E61E
extern int DRONE_COUNT;
// asm: 	.bss	ONCSCREEN_CARS,1
// addr: 0x0000E621
extern int ONCSCREEN_CARS;
// asm: 	.bss	DD_SLP,1	;MINIMUM SLEEP VALUE (def=40)
// addr: 0x0000E622
extern int DD_SLP;
// asm: 	.bss	DD_VAR,1	;SLEEP TIME VARIENCE (def=100)
// addr: 0x0000E623
extern int DD_VAR;
// asm: 	.bss	DD_MAX_DRONES,1	;MAX DRONES (def=MAX_DRONES)
// addr: 0x0000E624
extern int DD_MAX_DRONES;
// asm: 	.bss	DRONENUM,1
// addr: 0x0000E625
extern int DRONENUM;
// asm: 	.bss	SMOKE_COUNT,1
// addr: 0x0000E626
extern int SMOKE_COUNT;
// asm: 	.bss	COCONUT_COUNT,1
// addr: 0x0000E627
extern int COCONUT_COUNT;

#endif /* DRONES_H */

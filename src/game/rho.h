#ifndef RHO_H
#define RHO_H

#include "port.h"

/* Generated from asm/RHO.ASM. */

// RHO.ASM
// asm: 	.bss	NOLONG_VEHICLES,1
extern int NOLONG_VEHICLES;
// asm: 	.bss	RHOFLAG,1
extern int RHOFLAG;
// asm: 	.bss	RHOPAL,1
extern int RHOPAL;
void DECODE_RHO_KILL(void);
void DECODE_RHO_XSFER(void);

#endif /* RHO_H */

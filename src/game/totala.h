#ifndef TOTALA_H
#define TOTALA_H

#include "../core/port.h"
#include "types.h"

/* Generated from asm/TOTALA.ASM. */

// TOTALA.ASM
// asm: 	.bss	_ACNTL,1
extern int _ACNTL;
// asm: 	.bss	_ACMAP,1
extern int _ACMAP;
// asm: 	.bss	_AIVI,4
extern int _AIVI[];
// asm: 	.bss	_ADDRL,1
extern int _ADDRL;

void _rdma(int x1, int y1, int x2, int y2, int palette, int image_addr);
void _stuff_fpga(void);
void BLTMOD2D_NOPAL(const BLTMOD2D_MODEL* model, int x, int y, int control, int palette);
void BLTMOD2D(const BLTMOD2D_MODEL* model, int x, int y, int control, int skip_palette, int palette);
void BLTMOD2D_DS(const BLTMOD2D_MODEL* model, int x, int y, int control);

#endif /* TOTALA_H */

#ifndef FONT1A_H
#define FONT1A_H

#include "port.h"

/* Generated from asm/FONT1A.ASM. */

// FONT1A.ASM
// asm: 	.bss	ftoa_tmp,2
extern int ftoa_tmp[];
void _ftoa(void);
void _itoa(void);
void _pixel(int x, int y, int color);
void _outtextxyc(const char *string, int x, int y, int color);
void _fill(int x1, int y1, int x2, int y2, int color);
void _itoaLZ(void);
void HEX2ASC(void);

#endif /* FONT1A_H */

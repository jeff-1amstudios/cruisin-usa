#ifndef FONT1A_H
#define FONT1A_H

#include "port.h"

/* Generated from asm/FONT1A.ASM. */

// FONT1A.ASM
// asm: 	.bss	ftoa_tmp,2
extern int ftoa_tmp[];
void _ftoa(void);
void _itoa(void);
void _pixel(void);
void _outtextxyc(void);
void _fill(void);
void _itoaLZ(void);
void HEX2ASC(void);

#endif /* FONT1A_H */

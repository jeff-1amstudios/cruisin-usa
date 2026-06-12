#include "../core/cpu.h"
#include "../core/machine.h"

/*
 * Source module: asm/H2HOBJ.ASM
 */

/* asm: _SEChead2head:		;(16345 lines, 102.16%) */
/* asm: 	.word	0C15000h	;Src (Compressed) Addr */
/* asm: 	.word	0BEFA00h	;Dest Addr */
int _SEChead2head[] = {
    0x0C15000, // Src (Compressed) Addr
    0x0BEFA00, // Dest Addr
};

void h2hobj(void)
{
    UNIMPL();
}

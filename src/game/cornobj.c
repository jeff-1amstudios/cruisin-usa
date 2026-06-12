#include "../core/cpu.h"
#include "../core/machine.h"

/*
 * Source module: asm/CORNOBJ.ASM
 */

/* asm: _SECcorn:		;(15779 lines, 98.62%) */
/* asm: 	.word	0C16800h	;Src (Compressed) Addr */
/* asm: 	.word	0BE9600h	;Dest Addr */
int _SECcorn[2] = {
    0x0C16800, // Src (Compressed) Addr
    0x0BE9600, // Dest Addr
};

void cornobj(void)
{
    UNIMPL();
}

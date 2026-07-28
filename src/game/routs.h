#ifndef ROUTS_H
#define ROUTS_H

#include "../core/port.h"

/* Generated from asm/ROUTS.ASM. */

// ROUTS.ASM
c3x_reg_t SQRT(c3x_reg_t x /*R2*/);
c3x_reg_t DIV_F30(c3x_reg_t u, c3x_reg_t v);
void DIV_I30(void);
void DIV_U30(void);
c3x_reg_t INV_F30(c3x_reg_t);
void MOD_I30(void);
void MOD_U30(void);

#endif /* ROUTS_H */

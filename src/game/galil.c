#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"

/*
 * Source module: asm/GALIL.ASM
 */

extern int GALIL_RESPONSE[];

/*
*----------------------------------------------------------------------------
*
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

/* asm: GALIL_RESPONSE	.bss	GALIL_RESPONSE,80 */
int GALIL_RESPONSE[80];

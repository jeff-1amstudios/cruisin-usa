
#include "../core/machine.h"
#include "cmos.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/GALIL.ASM
 */

/*
 *----------------------------------------------------------------------------
 *
 *
 *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 */

/* asm: GALIL_RESPONSE	.bss	GALIL_RESPONSE,80 */
static int GALIL_RESPONSE[80];


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

/* asm: GALIL_RESPONSE	.bss	GALIL_RESPONSE,80 */
static int GALIL_RESPONSE[80];

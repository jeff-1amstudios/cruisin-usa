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
#include "port.h"

/*
 * Source module: asm/GALIL.ASM
 */

/* asm: GALIL_RESPONSE	.bss	GALIL_RESPONSE,80 */
int GALIL_RESPONSE[80];

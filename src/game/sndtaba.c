#include "../core/cpu.h"
#include "../core/machine.h"
#include "sys.h"
#include "port.h"

/*
 * Source module: asm/SNDTABA.ASM
 */

#define FRAMES 0x10000
#define SPEECH (0x0FF|ENINT|CHAN1)

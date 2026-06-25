#include "config.h"
#include "../game/vunit.h"

u32 port_get_dipswitches(void) {
    // 2 switches
    return 0 | (DIP_COMMP | CMDP_MASTER) << 16;
}

u32 port_get_commpal(void) {
    return 4;
}

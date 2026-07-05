#include "input.h"
#include "../game/vunit.h"

u32 port_get_dipswitches(void) {
    // 2 switches
    return 0 | (DIP_COMMP | CMDP_MASTER) << 16;
}

u32 port_get_commpal(void) {
    return 4;
}

u32 port_get_switch1() {
    return 0;
}

u32 port_get_switch2() {
    return 0;
}

u32 port_get_switch3() {
    return 0;
}

#ifndef CRUSN_INPUT_H
#define CRUSN_INPUT_H

#include "port_types.h"

void port_handle_input(void);
u32 port_get_dipswitches(void);
u32 port_get_commpal(void);
u32 port_get_switch1(void);
u32 port_get_switch2(void);
u32 port_get_switch3(void);

#endif

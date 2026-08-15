#ifndef CRUSN_INPUT_H
#define CRUSN_INPUT_H

#include "port_types.h"

#define PORT_STEERING_CENTER 128

void port_handle_input(void);
void port_sample_steering(void);
int port_take_steering_detent(void);
void port_set_steering(int value);
u32 port_get_dipswitches(void);
u32 port_get_commpal(void);
u32 port_get_switch1(void);
u32 port_get_switch2(void);
u32 port_get_switch3(void);
int port_get_steering(void);
int port_get_accelerator(void);
int port_get_brake(void);

#endif

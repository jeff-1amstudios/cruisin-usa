#ifndef CRUSN_OUTPUT_H
#define CRUSN_OUTPUT_H

#include "port_types.h"

void port_output_comm_io(u32 value);

int port_output_fpga(
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    int x4,
    int y4,
    int uv0,
    int uv1,
    int uv2,
    int uv3,
    int texture_base,
    int palette_base,
    int control);

void crusn_debug_output_reset_frame(void);
int crusn_debug_output_get_quad_count(void);
int crusn_debug_output_get_pixel_count(void);

#endif

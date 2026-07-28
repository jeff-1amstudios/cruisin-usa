#include "romreader.h"

#include "../game/macs.h"

c3x_reg_t ROM_ConsumeFloat(tyco_stream_t* ptr) {
    c3x_reg_t value = C3X_LOAD((u32)**ptr);
    *ptr += 1;
    return value;
}

s32 ROM_ConsumeS32(tyco_stream_t* ptr) {
    s32 value = **ptr;
    *ptr += 1;
    return value;
}

c3x_reg_t ROM_ParseFloat(s32 val) {
    return C3X_LOAD((u32)val);
}

u32 ROM_ReadU32(s32* ptr) {
    return (u32)*ptr;
}

void ROM_ReadTYCOHEADER(tyco_stream_t ptr, TYCOHEADER* header) {
    header->flag = ptr[0];
    header->pos.X = ROM_ParseFloat(ptr[1]);
    header->pos.Y = ROM_ParseFloat(ptr[2]);
    header->pos.Z = ROM_ParseFloat(ptr[3]);
    header->rad_y = ROM_ParseFloat(ptr[4]);
    header->group = ptr[5];
}

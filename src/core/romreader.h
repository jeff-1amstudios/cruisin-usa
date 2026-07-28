#ifndef CRUSN_ROMREADER_H
#define CRUSN_ROMREADER_H

#include "../game/types.h"

c3x_reg_t ROM_ParseFloat(s32 val);
u32 ROM_ReadU32(s32* ptr);

c3x_reg_t ROM_ConsumeFloat(tyco_stream_t* ptr);
s32 ROM_ConsumeS32(tyco_stream_t* ptr);

void ROM_ReadTYCOHEADER(tyco_stream_t ptr, TYCOHEADER* header);

#endif /* CRUSN_ROMREADER_H */

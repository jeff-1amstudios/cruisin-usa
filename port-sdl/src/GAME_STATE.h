#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>
#include <stdint.h>

enum {
    MMODE = 0x0000F,
    MDIAG = 0x00001,
    MATTR = 0x00002,
    MINTRO = 0x00003,
    MGAME = 0x00004,
    MBONUS = 0x00005,
    MINIT = 0x00006,
    MINSERT_COINS = 0x00007,
    MCT = 0x00008,
    MHS = 0x00020,
    MINFIN = 0x00040,
    MWATER = 0x00080,
    MHUD = 0x00100,
    MGO = 0x00200,
    MINTUNNEL = 0x00400,
};

extern uint32_t _MODE;
extern int _ATTR_MODE;
extern uint32_t START_HIT;
extern uint32_t g_frame_counter;

void PLATFORM_RENDER_MODE_SCREEN(void);
void CLR_RAM(void);
void COMM_ENABLE_INT2(void);

#endif

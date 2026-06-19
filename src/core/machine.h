#ifndef CRUSN_MACHINE_H
#define CRUSN_MACHINE_H

#include <stddef.h>

#include "memory.h"

enum {
    CRUSN_SCREEN_WIDTH = 512,
    CRUSN_SCREEN_HEIGHT = 400,
    CRUSN_SCREEN_WORDS = CRUSN_SCREEN_WIDTH * CRUSN_SCREEN_HEIGHT,
    CRUSN_CMOS_WORDS = 0x2000,
    CRUSN_COLORRAM_WORDS = 0x8000,
    CRUSN_TIMER_WORDS = 0x19,
    CRUSN_RAM_WORDS = 0x200000,
};

typedef struct crusn_machine {
    crusn_trace trace;
    crusn_memory_map memory;
    u32 *ram_words;
    u32 *screen_words;
    u32 *cmos_words;
    u32 *colorram_words;
    u32 *timer_words;
    size_t ram_word_count;
    size_t screen_word_count;
    size_t cmos_word_count;
    size_t colorram_word_count;
    size_t timer_word_count;
    int frame_counter;
} crusn_machine;

extern crusn_machine *g_crusn_machine;

#define crusn_mem_rd32(ADDR) crusn_mem_rd32_map(&g_crusn_machine->memory, (ADDR))
#define crusn_mem_wr32(ADDR, VALUE) crusn_mem_wr32_map(&g_crusn_machine->memory, (ADDR), (VALUE))

int crusn_machine_init(crusn_machine *machine);
void crusn_machine_shutdown(crusn_machine *machine);
void crusn_machine_tick(crusn_machine *machine);

#endif

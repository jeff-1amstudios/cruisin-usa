#ifndef CRUSN_MACHINE_H
#define CRUSN_MACHINE_H

#include <stddef.h>

#include "memory.h"

enum {
    CRUSN_SCREEN_WIDTH = 512,
    CRUSN_SCREEN_HEIGHT = 400,
    CRUSN_SCREEN_WORDS = CRUSN_SCREEN_WIDTH * CRUSN_SCREEN_HEIGHT,
    CRUSN_RAM_WORDS = 0x200000,
};

typedef struct crusn_machine {
    crusn_trace trace;
    crusn_memory_map memory;
    u32 *ram_words;
    u32 *screen_words;
    size_t ram_word_count;
    size_t screen_word_count;
    int frame_counter;
} crusn_machine;

extern crusn_machine *g_crusn_machine;

int crusn_machine_init(crusn_machine *machine);
void crusn_machine_shutdown(crusn_machine *machine);
void crusn_machine_tick(crusn_machine *machine);

#endif

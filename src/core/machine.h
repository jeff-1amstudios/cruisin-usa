#ifndef CRUSN_MACHINE_H
#define CRUSN_MACHINE_H

#include <stddef.h>

#include "cpu.h"
#include "memory.h"

enum {
    CRUSN_SCREEN_WIDTH = 512,
    CRUSN_SCREEN_HEIGHT = 400,
    CRUSN_SCREEN_WORDS = 0x40000,
    CRUSN_CMOS_WORDS = 0x2000,
    CRUSN_COLORRAM_WORDS = 0x8000,
    CRUSN_TIMER_WORDS = 0x19,
    CRUSN_RAM_WORDS = 0x200000,
    CRUSN_TRANSLATION_STACK_WORDS = 256,
};

typedef struct crusn_machine {
    crusn_trace trace;
    crusn_memory_map memory;
    u32* rom_words;
    u32* ram_words;
    u32* screen_words;
    u32* cmos_words;
    u32* colorram_words;
    u32* timer_words;
    size_t rom_word_count;
    size_t ram_word_count;
    size_t screen_word_count;
    size_t cmos_word_count;
    size_t colorram_word_count;
    size_t timer_word_count;
    int frame_counter;
    u32 translation_stack[CRUSN_TRANSLATION_STACK_WORDS];
    size_t translation_stack_top;
} crusn_machine;

extern crusn_machine* g_crusn_machine;

#define crusn_mem_rd32(ADDR) crusn_mem_rd32_map(&g_crusn_machine->memory, (ADDR))
#define crusn_mem_wr32(ADDR, VALUE) crusn_mem_wr32_map(&g_crusn_machine->memory, (ADDR), (VALUE))
#define ROM_ADDR(SYM) crusn_machine_rom_addr((word_addr_t)(SYM))
#define COLOROM_ADDR(ADDR) crusn_machine_colorram_addr((word_addr_t)(ADDR))

int crusn_machine_init(crusn_machine* machine);
void crusn_machine_shutdown(crusn_machine* machine);
void crusn_machine_tick(crusn_machine* machine);
void crusn_machine_decode_screen_argb8888(const crusn_machine* machine, u32* dst_pixels, size_t dst_pitch_bytes);
int crusn_machine_dump_screen_bmp(const crusn_machine* machine, const char* path);
u32* crusn_machine_rom_addr(word_addr_t addr);
u32* crusn_machine_colorram_addr(word_addr_t addr);
void crusn_machine_push_u32(u32 value);
u32 crusn_machine_pop_u32(void);
void crusn_machine_push_reg32(crusn_reg32 value);
crusn_reg32 crusn_machine_pop_reg32(void);

#endif

#ifndef CRUSN_MACHINE_H
#define CRUSN_MACHINE_H

#include <stddef.h>

#include "memory.h"

enum {

    CRUSN_ROM_WORDS = 0x280000,
    CRUSN_WAVERAM_WORDS = 0x1FFFFF,
    CRUSN_SCREEN_WIDTH = 512,
    CRUSN_SCREEN_HEIGHT = 400,
    CRUSN_SCREEN_WORDS = 0x40000,
    CRUSN_SCREEN_PAGES = 2,
    CRUSN_SCREEN_TOTAL_WORDS = CRUSN_SCREEN_WORDS * CRUSN_SCREEN_PAGES,
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
    u32* screen_page_words[CRUSN_SCREEN_PAGES];
    u32* cmos_words;
    u32* colorram_words;
    u32* timer_words;
    size_t rom_word_count;
    size_t ram_word_count;
    size_t screen_word_count;
    size_t cmos_word_count;
    size_t colorram_word_count;
    size_t timer_word_count;
    int display_page_index;
    int write_page_index;
    int frame_counter;
    u32 translation_stack[CRUSN_TRANSLATION_STACK_WORDS];
    size_t translation_stack_top;
} crusn_machine;

extern crusn_machine* g_crusn_machine;
extern s32 crusn_rom_words[CRUSN_ROM_WORDS];
extern s32 crusn_waveram[CRUSN_WAVERAM_WORDS];

#define crusn_mem_rd32(ADDR) crusn_mem_rd32_map(&g_crusn_machine->memory, (ADDR))
#define crusn_mem_wr32(ADDR, VALUE) crusn_mem_wr32_map(&g_crusn_machine->memory, (ADDR), (VALUE))
#define ROM_PTR(SYM) (&crusn_rom_words[(size_t)((word_addr_t)(SYM) - CRUSN_ROM_BASE)])
#define WAVERAM_PTR(SYM) (&crusn_waveram[(size_t)((word_addr_t)(SYM) - CRUSN_WAVERAM_BASE)])
#define COLOROM_ADDR(ADDR) crusn_machine_colorram_addr((word_addr_t)(ADDR))

int crusn_machine_init(crusn_machine* machine);
void crusn_machine_shutdown(crusn_machine* machine);
void crusn_machine_tick(crusn_machine* machine);
void crusn_machine_decode_screen_argb8888(const crusn_machine* machine, u32* dst_pixels, size_t dst_pitch_bytes);
int crusn_machine_dump_screen_bmp(
    const crusn_machine* machine, const u32* screen_words, const u32* colorram_words, const char* path);
u32* crusn_machine_rom_addr(word_addr_t addr);
u32* crusn_machine_colorram_addr(word_addr_t addr);
u32* crusn_machine_screen_page(const crusn_machine* machine, int page_index);
void crusn_machine_set_screen_pages(crusn_machine* machine, int display_page_index, int write_page_index);
void crusn_machine_clear_screen_page(crusn_machine* machine, int page_index);
u32 crusn_read_u32(const u32** cursor);
s32 crusn_read_s32(const u32** cursor);
f32 crusn__read_f32(const u32** cursor);

void crusn_yield_display_interrupt(void);

#endif

#include "memory.h"

#include <assert.h>

enum {
    CRUSN_RAM_BASE = 0x00000000u,
    CRUSN_SCREEN_BASE = 0x0900000u,
    CRUSN_COMMPAL_ADDR = 0x0990000u,
    CRUSN_DIPSW_ADDR = 0x0992000u,
    CRUSN_CMOS_BASE = 0x09C0000u,
    CRUSN_COLORRAM_BASE = 0x09E0000u,
    CRUSN_TIMER_BASE = 0x808020u,
};

static crusn_memory_region *crusn_find_region(crusn_memory_map *memory, word_addr_t addr)
{
    crusn_memory_region *regions[] = { &memory->ram, &memory->screen, &memory->cmos, &memory->colorram, &memory->timer };
    size_t i;

    for (i = 0; i < sizeof(regions) / sizeof(regions[0]); ++i) {
        crusn_memory_region *region = regions[i];
        word_addr_t end = region->base + (word_addr_t)region->word_count;
        if (addr >= region->base && addr < end) {
            return region;
        }
    }

    return NULL;
}

static const crusn_memory_region *crusn_find_region_const(const crusn_memory_map *memory, word_addr_t addr)
{
    const crusn_memory_region *regions[] = { &memory->ram, &memory->screen, &memory->cmos, &memory->colorram, &memory->timer };
    size_t i;

    for (i = 0; i < sizeof(regions) / sizeof(regions[0]); ++i) {
        const crusn_memory_region *region = regions[i];
        word_addr_t end = region->base + (word_addr_t)region->word_count;
        if (addr >= region->base && addr < end) {
            return region;
        }
    }

    return NULL;
}

void crusn_memory_init(
    crusn_memory_map *memory,
    u32 *ram_words,
    size_t ram_word_count,
    u32 *screen_words,
    size_t screen_word_count,
    u32 *cmos_words,
    size_t cmos_word_count,
    u32 *colorram_words,
    size_t colorram_word_count,
    u32 *timer_words,
    size_t timer_word_count,
    crusn_trace *trace
)
{
    memory->ram.name = "ram";
    memory->ram.base = CRUSN_RAM_BASE;
    memory->ram.words = ram_words;
    memory->ram.word_count = ram_word_count;

    memory->screen.name = "screen";
    memory->screen.base = CRUSN_SCREEN_BASE;
    memory->screen.words = screen_words;
    memory->screen.word_count = screen_word_count;

    memory->cmos.name = "cmos";
    memory->cmos.base = CRUSN_CMOS_BASE;
    memory->cmos.words = cmos_words;
    memory->cmos.word_count = cmos_word_count;

    memory->colorram.name = "colorram";
    memory->colorram.base = CRUSN_COLORRAM_BASE;
    memory->colorram.words = colorram_words;
    memory->colorram.word_count = colorram_word_count;

    memory->timer.name = "timer";
    memory->timer.base = CRUSN_TIMER_BASE;
    memory->timer.words = timer_words;
    memory->timer.word_count = timer_word_count;

    memory->trace = trace;
}

u32 crusn_mem_rd32_map(const crusn_memory_map *memory, word_addr_t addr)
{
    const crusn_memory_region *region = crusn_find_region_const(memory, addr);
    size_t offset;

    if (addr == CRUSN_COMMPAL_ADDR || addr == CRUSN_DIPSW_ADDR) {
        return 0;
    }

    assert(region != NULL);
    offset = (size_t)(addr - region->base);
    assert(offset < region->word_count);

    return region->words[offset];
}

void crusn_mem_wr32_map(crusn_memory_map *memory, word_addr_t addr, u32 value)
{
    crusn_memory_region *region = crusn_find_region(memory, addr);
    size_t offset;

    assert(region != NULL);
    offset = (size_t)(addr - region->base);
    assert(offset < region->word_count);

    region->words[offset] = value;
}

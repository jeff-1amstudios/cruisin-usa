#ifndef CRUSN_MEMORY_H
#define CRUSN_MEMORY_H

#include <stddef.h>

#include "port_types.h"
#include "trace.h"

typedef u32 word_addr_t;

typedef struct crusn_memory_region {
    const char *name;
    word_addr_t base;
    u32 *words;
    size_t word_count;
} crusn_memory_region;

typedef struct crusn_memory_map {
    crusn_memory_region ram;
    crusn_memory_region screen;
    crusn_trace *trace;
} crusn_memory_map;

void crusn_memory_init(
    crusn_memory_map *memory,
    u32 *ram_words,
    size_t ram_word_count,
    u32 *screen_words,
    size_t screen_word_count,
    crusn_trace *trace
);

u32 crusn_mem_rd32(const crusn_memory_map *memory, word_addr_t addr);
void crusn_mem_wr32(crusn_memory_map *memory, word_addr_t addr, u32 value);

#endif

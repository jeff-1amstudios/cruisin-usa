#ifndef CRUSN_ROM_H
#define CRUSN_ROM_H
#include <stddef.h>
#include "port_types.h"
#define PORTABLE_ROM_PATH "roms/crusnusa.zip"
#define PORTABLE_ROM_PART_PATH "roms/crusnusa.zip.part"
/* NULL output validates the required 4.5 game entries without loading them. */
int portable_rom_load(const char* path, u32* out_words, size_t out_word_count);
#endif

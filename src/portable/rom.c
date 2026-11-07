#include "rom.h"
#include "machine.h"
#include "miniz.h"
#include <stdlib.h>
#include <string.h>

typedef struct crusn_rom_zip_entry {
    const char* name;
    size_t byte_offset;
} crusn_rom_zip_entry;

static const crusn_rom_zip_entry CRUSN_ROM_ZIP_ENTRIES[] = {
    { "v4.5_4-11-95_cruisn_usa_u10_86b3.u10", 0x000000 },
    { "v4.5_4-11-95_cruisn_usa_u11_6d73.u11", 0x000001 },
    { "v4.5_4-11-95_cruisn_usa_u12_4b32.u12", 0x000002 },
    { "v4.5_4-11-95_cruisn_usa_u13_430e.u13", 0x000003 },
    { "l1_cruisin_u.s.a._game_rom_u14.u14", 0x200000 },
    { "l1_cruisin_u.s.a._game_rom_u15.u15", 0x200001 },
    { "l1_cruisin_u.s.a._game_rom_u16.u16", 0x200002 },
    { "l1_cruisin_u.s.a._game_rom_u17.u17", 0x200003 },
    { "l1_cruisin_u.s.a._game_rom_u18.u18", 0x400000 },
    { "l1_cruisin_u.s.a._game_rom_u19.u19", 0x400001 },
    { "l1_cruisin_u.s.a._game_rom_u20.u20", 0x400002 },
    { "l1_cruisin_u.s.a._game_rom_u21.u21", 0x400003 },
    { "l1_cruisin_u.s.a._game_rom_u22.u22", 0x600000 },
    { "l1_cruisin_u.s.a._game_rom_u23.u23", 0x600001 },
    { "l1_cruisin_u.s.a._game_rom_u24.u24", 0x600002 },
    { "l1_cruisin_u.s.a._game_rom_u25.u25", 0x600003 },
    { "l1_cruisin_u.s.a._game_rom_u26.u26", 0x800000 },
    { "l1_cruisin_u.s.a._game_rom_u27.u27", 0x800001 },
    { "l1_cruisin_u.s.a._game_rom_u28.u28", 0x800002 },
    { "l1_cruisin_u.s.a._game_rom_u29.u29", 0x800003 },
};

int portable_rom_load(const char* path, u32* out_words, size_t out_word_count) {
    mz_zip_archive archive = { 0 };
    mz_zip_archive_file_stat file_stat;
    unsigned char* lane_data = NULL;
    const crusn_rom_zip_entry* rom_entry;
    size_t entry_index;
    size_t lane_byte;
    size_t word_index;
    int zip_index;
    int archive_open = 0;
    int result = -1;

    if (out_words != NULL) {
        memset(out_words, 0, out_word_count * sizeof(*out_words));
    }
    lane_data = malloc(CRUSN_PROGRAM_ROM_WORDS);
    if (lane_data == NULL) {
        goto cleanup;
    }
    if (!mz_zip_reader_init_file(&archive, path, 0)) {
        goto cleanup;
    }
    archive_open = 1;

    for (entry_index = 0; entry_index < sizeof(CRUSN_ROM_ZIP_ENTRIES) / sizeof(CRUSN_ROM_ZIP_ENTRIES[0]); entry_index++) {
        rom_entry = &CRUSN_ROM_ZIP_ENTRIES[entry_index];
        zip_index = mz_zip_reader_locate_file(&archive, rom_entry->name, NULL, 0);
        if (zip_index < 0
            || !mz_zip_reader_file_stat(&archive, (mz_uint)zip_index, &file_stat)
            || file_stat.m_uncomp_size != CRUSN_PROGRAM_ROM_WORDS
            || !mz_zip_reader_extract_to_mem(&archive, (mz_uint)zip_index, lane_data, CRUSN_PROGRAM_ROM_WORDS, 0)
            || mz_crc32(MZ_CRC32_INIT, lane_data, CRUSN_PROGRAM_ROM_WORDS) != file_stat.m_crc32) {
            goto cleanup;
        }

        if (out_words == NULL) {
            continue;
        }
        for (lane_byte = 0; lane_byte < CRUSN_PROGRAM_ROM_WORDS; lane_byte++) {
            word_index = (rom_entry->byte_offset >> 2) + lane_byte;
            if (word_index >= out_word_count) {
                goto cleanup;
            }
            out_words[word_index] |= (u32)lane_data[lane_byte] << ((rom_entry->byte_offset & 3u) * 8u);
        }
    }
    result = 0;

cleanup:
    if (archive_open) {
        mz_zip_reader_end(&archive);
    }
    free(lane_data);
    return result;
}

#include "machine.h"
#include "cpu.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

crusn_machine* g_crusn_machine = NULL;
u32 crusn_rom_words[CRUSN_ROM_WORDS];

static const char* CRUSN_ROM_PATH = "roms/crusnusa45_maindata_interleaved.bin";

static void crusn_write_le16(FILE* fp, unsigned value) {
    fputc((int)(value & 0xFFu), fp);
    fputc((int)((value >> 8) & 0xFFu), fp);
}

static void crusn_write_le32(FILE* fp, unsigned value) {
    fputc((int)(value & 0xFFu), fp);
    fputc((int)((value >> 8) & 0xFFu), fp);
    fputc((int)((value >> 16) & 0xFFu), fp);
    fputc((int)((value >> 24) & 0xFFu), fp);
}

static unsigned char crusn_expand_5_to_8(unsigned value) {
    return (unsigned char)((value << 3) | (value >> 2));
}

static int crusn_load_rom_words(const char* path, u32* out_words, size_t out_word_count) {
    FILE* fp;
    unsigned char* bytes = NULL;
    long file_size_long;
    size_t file_size;
    size_t i;

    assert(path != NULL);
    assert(out_words != NULL);
    assert(out_word_count > 0);

    fp = fopen(path, "rb");
    if (fp == NULL) {
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }
    file_size_long = ftell(fp);
    if (file_size_long < 0) {
        fclose(fp);
        return -1;
    }
    if (fseek(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        return -1;
    }

    file_size = (size_t)file_size_long;
    if ((file_size & 3u) != 0u) {
        fclose(fp);
        return -1;
    }

    bytes = malloc(file_size);
    if (bytes == NULL) {
        fclose(fp);
        return -1;
    }
    if (fread(bytes, 1, file_size, fp) != file_size) {
        free(bytes);
        fclose(fp);
        return -1;
    }
    fclose(fp);

    if (file_size != out_word_count * sizeof(*out_words)) {
        free(bytes);
        return -1;
    }

    /* The raw interleaved ROM is byte-laid out so little-endian decode matches the TMS word view. */
    for (i = 0; i < out_word_count; ++i) {
        size_t off = i * 4u;
        out_words[i] = (u32)bytes[off]
            | ((u32)bytes[off + 1] << 8)
            | ((u32)bytes[off + 2] << 16)
            | ((u32)bytes[off + 3] << 24);
    }

    free(bytes);
    return 0;
}

u32* crusn_machine_rom_addr(word_addr_t addr) {
    crusn_machine* machine = g_crusn_machine;
    size_t offset;

    assert(machine != NULL);
    assert(addr >= machine->memory.rom.base);
    offset = (size_t)(addr - machine->memory.rom.base);
    assert(offset < machine->memory.rom.word_count);

    return &crusn_rom_words[offset];
}

u32* crusn_machine_colorram_addr(word_addr_t addr) {
    crusn_machine* machine = g_crusn_machine;
    size_t offset;

    assert(machine != NULL);
    assert(addr >= machine->memory.colorram.base);
    offset = (size_t)(addr - machine->memory.colorram.base);
    assert(offset < machine->memory.colorram.word_count);

    return &machine->colorram_words[offset];
}

void crusn_machine_decode_screen_argb8888(const crusn_machine* machine, u32* dst_pixels, size_t dst_pitch_bytes) {
    size_t y;
    size_t x;

    for (y = 0; y < CRUSN_SCREEN_HEIGHT; ++y) {
        const u32* src_row = &machine->screen_words[y * CRUSN_SCREEN_WIDTH];
        u32* dst_row = (u32*)((unsigned char*)dst_pixels + (y * dst_pitch_bytes));

        for (x = 0; x < CRUSN_SCREEN_WIDTH; ++x) {
            u32 pixel = src_row[x];

            if ((pixel & 0xFFFF0000u) != 0u) {
                dst_row[x] = pixel;
            } else if ((pixel & 0xFFFFu) < machine->colorram_word_count) {
                u32 rgb555 = machine->colorram_words[pixel & 0xFFFFu] & 0x7FFFu;
                unsigned char r = crusn_expand_5_to_8((rgb555 >> 10) & 0x1Fu);
                unsigned char g = crusn_expand_5_to_8((rgb555 >> 5) & 0x1Fu);
                unsigned char b = crusn_expand_5_to_8(rgb555 & 0x1Fu);

                dst_row[x] = 0xFF000000u | ((u32)r << 16) | ((u32)g << 8) | (u32)b;
            } else {
                dst_row[x] = 0xFF000000u;
            }
        }
    }
}

int crusn_machine_init(crusn_machine* machine) {
    memset(machine, 0, sizeof(*machine));
    crusn_cpu_reset();

    machine->rom_words = crusn_rom_words;
    machine->rom_word_count = CRUSN_ROM_WORDS;

    if (crusn_load_rom_words(CRUSN_ROM_PATH, crusn_rom_words, CRUSN_ROM_WORDS) != 0) {
        return -1;
    }

    machine->ram_word_count = CRUSN_RAM_WORDS;
    machine->screen_word_count = CRUSN_SCREEN_WORDS;
    machine->cmos_word_count = CRUSN_CMOS_WORDS;
    machine->colorram_word_count = CRUSN_COLORRAM_WORDS;
    machine->timer_word_count = CRUSN_TIMER_WORDS;
    machine->ram_words = calloc(machine->ram_word_count, sizeof(*machine->ram_words));
    machine->screen_words = calloc(machine->screen_word_count, sizeof(*machine->screen_words));
    machine->cmos_words = calloc(machine->cmos_word_count, sizeof(*machine->cmos_words));
    machine->colorram_words = calloc(machine->colorram_word_count, sizeof(*machine->colorram_words));
    machine->timer_words = calloc(machine->timer_word_count, sizeof(*machine->timer_words));

    if (machine->ram_words == NULL || machine->screen_words == NULL || machine->cmos_words == NULL || machine->colorram_words == NULL || machine->timer_words == NULL) {
        crusn_machine_shutdown(machine);
        return -1;
    }

    crusn_trace_init(&machine->trace, stdout);
    crusn_memory_init(
        &machine->memory,
        machine->rom_words,
        machine->rom_word_count,
        machine->ram_words,
        machine->ram_word_count,
        machine->screen_words,
        machine->screen_word_count,
        machine->cmos_words,
        machine->cmos_word_count,
        machine->colorram_words,
        machine->colorram_word_count,
        machine->timer_words,
        machine->timer_word_count,
        &machine->trace);

    g_crusn_machine = machine;

    return 0;
}

void crusn_machine_shutdown(crusn_machine* machine) {
    free(machine->ram_words);
    free(machine->screen_words);
    free(machine->cmos_words);
    free(machine->colorram_words);
    free(machine->timer_words);
    machine->rom_words = NULL;
    machine->ram_words = NULL;
    machine->screen_words = NULL;
    machine->cmos_words = NULL;
    machine->colorram_words = NULL;
    machine->timer_words = NULL;
    machine->rom_word_count = 0;
    machine->ram_word_count = 0;
    machine->screen_word_count = 0;
    machine->cmos_word_count = 0;
    machine->colorram_word_count = 0;
    machine->timer_word_count = 0;
    if (g_crusn_machine == machine) {
        g_crusn_machine = NULL;
    }
}

void crusn_machine_tick(crusn_machine* machine) {
    int x;
    int y;

    for (y = 0; y < CRUSN_SCREEN_HEIGHT; ++y) {
        for (x = 0; x < CRUSN_SCREEN_WIDTH; ++x) {
            u32 r = (u32)((x + machine->frame_counter) & 0xFF);
            u32 g = (u32)((y + machine->frame_counter) & 0xFF);
            u32 b = (u32)((x + y + machine->frame_counter) & 0xFF);
            machine->screen_words[(y * CRUSN_SCREEN_WIDTH) + x] = 0xFF000000u | (r << 16) | (g << 8) | b;
        }
    }

    TRACE_EVENT(&machine->trace, "frame", "tick", (u32)machine->frame_counter, 0);
    machine->frame_counter++;
}

int crusn_machine_dump_screen_bmp(
    const crusn_machine* machine, const u32* screen_words, const u32* colorram_words, const char* path) {
    FILE* fp;
    const unsigned width = CRUSN_SCREEN_WIDTH;
    const unsigned height = CRUSN_SCREEN_HEIGHT;
    const unsigned row_size = width * 4u;
    const unsigned pixel_bytes = row_size * height;
    const unsigned file_size = 14u + 40u + pixel_bytes;
    u32* pixels;
    unsigned y;

    if (machine == NULL || screen_words == NULL || colorram_words == NULL || path == NULL) {
        return -1;
    }

    fp = fopen(path, "wb");
    if (fp == NULL) {
        return -1;
    }

    fputc('B', fp);
    fputc('M', fp);
    crusn_write_le32(fp, file_size);
    crusn_write_le16(fp, 0);
    crusn_write_le16(fp, 0);
    crusn_write_le32(fp, 14u + 40u);
    crusn_write_le32(fp, 40u);
    crusn_write_le32(fp, width);
    crusn_write_le32(fp, (unsigned)(-(int)height));
    crusn_write_le16(fp, 1);
    crusn_write_le16(fp, 32);
    crusn_write_le32(fp, 0);
    crusn_write_le32(fp, pixel_bytes);
    crusn_write_le32(fp, 0);
    crusn_write_le32(fp, 0);
    crusn_write_le32(fp, 0);
    crusn_write_le32(fp, 0);

    pixels = malloc(pixel_bytes);
    if (pixels == NULL) {
        fclose(fp);
        return -1;
    }

    for (y = 0; y < CRUSN_SCREEN_HEIGHT; ++y) {
        const u32* src_row = &screen_words[y * CRUSN_SCREEN_WIDTH];
        u32* dst_row = (u32*)((unsigned char*)pixels + (y * row_size));
        unsigned x;

        for (x = 0; x < CRUSN_SCREEN_WIDTH; ++x) {
            u32 pixel = src_row[x];

            if ((pixel & 0xFFFF0000u) != 0u) {
                dst_row[x] = pixel;
            } else if ((pixel & 0xFFFFu) < machine->colorram_word_count) {
                u32 rgb555 = colorram_words[pixel & 0xFFFFu] & 0x7FFFu;
                unsigned char r = crusn_expand_5_to_8((rgb555 >> 10) & 0x1Fu);
                unsigned char g = crusn_expand_5_to_8((rgb555 >> 5) & 0x1Fu);
                unsigned char b = crusn_expand_5_to_8(rgb555 & 0x1Fu);

                dst_row[x] = 0xFF000000u | ((u32)r << 16) | ((u32)g << 8) | (u32)b;
            } else {
                dst_row[x] = 0xFF000000u;
            }
        }
    }

    for (y = 0; y < height; ++y) {
        const u32* row = (const u32*)((const unsigned char*)pixels + ((size_t)y * row_size));
        unsigned x;

        for (x = 0; x < width; ++x) {
            u32 pixel = row[x];
            unsigned char r = (unsigned char)((pixel >> 16) & 0xFFu);
            unsigned char g = (unsigned char)((pixel >> 8) & 0xFFu);
            unsigned char b = (unsigned char)(pixel & 0xFFu);

            fputc((int)b, fp);
            fputc((int)g, fp);
            fputc((int)r, fp);
            fputc(0, fp);
        }
    }

    free(pixels);
    fclose(fp);
    return 0;
}

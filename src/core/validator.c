#include "validator.h"
#include "machine.h"

#include <dlfcn.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdnoreturn.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* g_validate_log;
static int g_validate_maps_loaded;

typedef struct VALIDATE_SYMBOL_ENTRY {
    uintptr_t address;
    char name[128];
} VALIDATE_SYMBOL_ENTRY;

typedef struct VALIDATE_SYMBOL_MAP {
    VALIDATE_SYMBOL_ENTRY* entries;
    size_t count;
    uintptr_t link_base;
} VALIDATE_SYMBOL_MAP;

static VALIDATE_SYMBOL_MAP g_port_map;
static VALIDATE_SYMBOL_MAP g_address_map;

typedef enum VALIDATE_KIND {
    VALIDATE_KIND_WORD,
    VALIDATE_KIND_WORD_ARRAY,
    VALIDATE_KIND_FILE,
} VALIDATE_KIND;

typedef struct VALIDATE_ENTRY {
    VALIDATE_KIND kind;
    uint32_t word_value;
    uint32_t word_values[32];
    size_t word_count;
    char file_path[260];
} VALIDATE_ENTRY;

static FILE* open_validate_log(void) {
    if (g_validate_log != NULL) {
        return g_validate_log;
    }

    g_validate_log = fopen("error.log", "r");
    if (g_validate_log == NULL) {
        fprintf(stderr, "validate_word: failed to open error.log\n");
        fflush(stderr);
        abort();
    }

    return g_validate_log;
}

static VALIDATE_SYMBOL_ENTRY* reserve_symbol_entry(VALIDATE_SYMBOL_MAP* map) {
    VALIDATE_SYMBOL_ENTRY* grown_entries =
        realloc(map->entries, (map->count + 1) * sizeof(VALIDATE_SYMBOL_ENTRY));
    if (grown_entries == NULL) {
        fprintf(stderr, "validator: failed to grow symbol map\n");
        fflush(stderr);
        abort();
    }
    map->entries = grown_entries;
    return &map->entries[map->count++];
}

static const char* strip_macho_prefix(const char* name) {
    if (name[0] == '_') {
        return name + 1;
    }
    return name;
}

static void load_port_map(VALIDATE_SYMBOL_MAP* out_map, const char* path) {
    char line[512];
    bool in_symbols = false;
    FILE* map = fopen(path, "r");
    if (map == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), map) != NULL) {
        char* end = NULL;
        char* name_start = NULL;
        char raw_name[128];
        uintptr_t address = 0;

        if (!in_symbols) {
            if (strncmp(line, "# Symbols:", 10) == 0) {
                in_symbols = true;
            }
            continue;
        }

        if (strncmp(line, "0x", 2) != 0) {
            continue;
        }

        address = (uintptr_t)strtoull(line, &end, 16);
        if (end == line) {
            continue;
        }

        name_start = strrchr(line, ']');
        if (name_start == NULL) {
            continue;
        }
        name_start += 1;
        while (*name_start == ' ' || *name_start == '\t') {
            name_start++;
        }

        if (sscanf(name_start, "%127s", raw_name) == 1) {
            VALIDATE_SYMBOL_ENTRY* entry = reserve_symbol_entry(out_map);
            const char* normalized = strip_macho_prefix(raw_name);
            entry->address = address;
            snprintf(entry->name, sizeof(entry->name), "%s", normalized);
            if (strcmp(raw_name, "__mh_execute_header") == 0) {
                out_map->link_base = address;
            }
        }
    }

    fclose(map);
}

static void load_address_map(VALIDATE_SYMBOL_MAP* out_map, const char* path) {
    char line[512];
    FILE* map = fopen(path, "r");
    if (map == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), map) != NULL) {
        unsigned int address = 0;
        char name[128];
        if (sscanf(line, " %*4[0-9A-Fa-f]:%8X %127s", &address, name) == 2) {
            VALIDATE_SYMBOL_ENTRY* entry = reserve_symbol_entry(out_map);
            entry->address = (uintptr_t)address;
            snprintf(entry->name, sizeof(entry->name), "%s", name);
        }
    }

    fclose(map);
}

static void load_validate_maps(void) {
    if (g_validate_maps_loaded) {
        return;
    }

    load_port_map(&g_port_map, "build/port.map");
    if (g_port_map.count == 0) {
        load_port_map(&g_port_map, "port.map");
    }
    load_address_map(&g_address_map, "tools/ida/address.map");

    if (g_port_map.count == 0 || g_port_map.link_base == 0) {
        fprintf(stderr, "validator: failed to load port.map symbols\n");
        fflush(stderr);
        abort();
    }
    if (g_address_map.count == 0) {
        fprintf(stderr, "validator: failed to load tools/ida/address.map symbols\n");
        fflush(stderr);
        abort();
    }

    g_validate_maps_loaded = 1;
}

static const char* lookup_map_name_by_address(const VALIDATE_SYMBOL_MAP* map, uintptr_t address) {
    size_t index;
    for (index = 0; index < map->count; ++index) {
        if (map->entries[index].address == address) {
            return map->entries[index].name;
        }
    }
    return NULL;
}

static const char* lookup_port_symbol_name(const void* ptr) {
    Dl_info info;
    uintptr_t slide;
    uintptr_t runtime_address;

    load_validate_maps();
    if (dladdr(ptr, &info) == 0 || info.dli_fbase == NULL) {
        return NULL;
    }
    slide = (uintptr_t)info.dli_fbase - g_port_map.link_base;
    runtime_address = (uintptr_t)ptr - slide;
    return lookup_map_name_by_address(&g_port_map, runtime_address);
}

static const char* lookup_rom_symbol_name(uint32_t address) {
    load_validate_maps();
    return lookup_map_name_by_address(&g_address_map, address);
}

static int lookup_rom_word_address(const void* ptr, uint32_t* out_address) {
    crusn_machine* machine = g_crusn_machine;
    uintptr_t base;
    uintptr_t target;
    size_t offset_words;

    if (machine == NULL || machine->rom_words == NULL || out_address == NULL) {
        return 0;
    }

    base = (uintptr_t)machine->rom_words;
    target = (uintptr_t)ptr;
    if (target < base) {
        return 0;
    }

    offset_words = (size_t)((target - base) / sizeof(*machine->rom_words));
    if (base + (offset_words * sizeof(*machine->rom_words)) != target) {
        return 0;
    }
    if (offset_words >= machine->rom_word_count) {
        return 0;
    }

    *out_address = machine->memory.rom.base + (uint32_t)offset_words;
    return 1;
}

static noreturn void validate_fail(
    const char* caller_file,
    int caller_line,
    const char* name,
    const char* message,
    const char* expected,
    const char* actual
) {
    fprintf(
        stderr,
        "%s:%d: validate_word(%s) failed: %s; expected %s but was %s\n",
        caller_file,
        caller_line,
        name,
        message,
        expected,
        actual
    );
    fflush(stderr);
    abort();
}

static void validate_pass_word(const char* name, uint32_t value) {
    fprintf(stderr, "validate ok %s: 0x%08" PRIX32 "\n", name, value);
    fflush(stderr);
}

static void validate_pass_arg(const char* reg_name, const char* symbol_name) {
    fprintf(stderr, "validate ok %s: %s\n", reg_name, symbol_name);
    fflush(stderr);
}

static void validate_pass_arg_rom(const char* reg_name, uint32_t rom_address) {
    const char* symbol_name = lookup_rom_symbol_name(rom_address);

    if (symbol_name != NULL) {
        fprintf(stderr, "validate ok %s: %s\n", reg_name, symbol_name);
    } else {
        fprintf(stderr, "validate ok %s: 0x%08" PRIX32 "\n", reg_name, rom_address);
    }
    fflush(stderr);
}

static int read_next_validate_line(char* out_name, size_t out_name_size, VALIDATE_ENTRY* out_entry) {
    char line[512];
    FILE* log = open_validate_log();

    while (fgets(line, sizeof(line), log) != NULL) {
        char name_buf[128];
        char file_buf[260];
        unsigned int value = 0;
        char* values_start;
        int matched = sscanf(line, "validate %127[^:]: 0x%X", name_buf, &value);
        if (matched != 2) {
            matched = sscanf(line, "\xEF\xBB\xBFvalidate %127[^:]: 0x%X", name_buf, &value);
        }
        if (matched == 2) {
            snprintf(out_name, out_name_size, "%s", name_buf);
            out_entry->kind = VALIDATE_KIND_WORD;
            out_entry->word_value = (uint32_t)value;
            out_entry->word_count = 0;
            out_entry->file_path[0] = '\0';
            return 1;
        }

        matched = sscanf(line, "validate %127[^:]:", name_buf);
        if (matched != 1) {
            matched = sscanf(line, "\xEF\xBB\xBFvalidate %127[^:]:", name_buf);
        }
        if (matched == 1) {
            values_start = strchr(line, ':');
            if (values_start != NULL) {
                char* cursor = values_start + 1;
                size_t value_count = 0;
                while (*cursor == ' ' || *cursor == '\t') {
                    cursor++;
                }
                while (strncmp(cursor, "0x", 2) == 0 || strncmp(cursor, "0X", 2) == 0) {
                    char* end = NULL;
                    unsigned long parsed = strtoul(cursor, &end, 16);
                    if (end == cursor || value_count >= (sizeof(out_entry->word_values) / sizeof(out_entry->word_values[0]))) {
                        value_count = 0;
                        break;
                    }
                    out_entry->word_values[value_count++] = (uint32_t)parsed;
                    cursor = end;
                    while (*cursor == ' ' || *cursor == '\t') {
                        cursor++;
                    }
                }
                if (value_count > 1) {
                    snprintf(out_name, out_name_size, "%s", name_buf);
                    out_entry->kind = VALIDATE_KIND_WORD_ARRAY;
                    out_entry->word_value = 0;
                    out_entry->word_count = value_count;
                    out_entry->file_path[0] = '\0';
                    return 1;
                }
            }
        }

        matched = sscanf(line, "validate %127[^:]: file=%259s", name_buf, file_buf);
        if (matched != 2) {
            matched = sscanf(line, "\xEF\xBB\xBFvalidate %127[^:]: file=%259s", name_buf, file_buf);
        }
        if (matched == 2) {
            snprintf(out_name, out_name_size, "%s", name_buf);
            out_entry->kind = VALIDATE_KIND_FILE;
            out_entry->word_value = 0;
            out_entry->word_count = 0;
            snprintf(out_entry->file_path, sizeof(out_entry->file_path), "%s", file_buf);
            return 1;
        }
    }

    return 0;
}

static uint8_t* read_validate_dump(const char* path, size_t* out_size) {
    FILE* dump = fopen(path, "rb");
    uint8_t* buffer;
    long file_size;

    if (dump == NULL) {
        return NULL;
    }

    if (fseek(dump, 0, SEEK_END) != 0) {
        fclose(dump);
        return NULL;
    }

    file_size = ftell(dump);
    if (file_size < 0) {
        fclose(dump);
        return NULL;
    }

    if (fseek(dump, 0, SEEK_SET) != 0) {
        fclose(dump);
        return NULL;
    }

    *out_size = (size_t)file_size;
    buffer = malloc((size_t)file_size);
    if (buffer == NULL && file_size != 0) {
        fclose(dump);
        return NULL;
    }

    if ((size_t)file_size != 0 && fread(buffer, 1, (size_t)file_size, dump) != (size_t)file_size) {
        free(buffer);
        fclose(dump);
        return NULL;
    }

    fclose(dump);
    return buffer;
}

static void validate_dump_bytes(
    const char* caller_file, int caller_line, const char* name, const void* ptr, const char* dump_path
) {
    char expected_buf[64];
    char actual_buf[64];
    char message_buf[96];
    size_t dump_size = 0;
    size_t index = 0;
    const uint8_t* actual_bytes = ptr;
    uint8_t* expected_bytes = read_validate_dump(dump_path, &dump_size);

    if (expected_bytes == NULL && dump_size != 0) {
        validate_fail(caller_file, caller_line, name, "failed to read validate dump file", dump_path, "<unreadable>");
    }

    while (index + sizeof(uint32_t) <= dump_size) {
        uint32_t expected_word = 0;
        uint32_t actual_word = 0;
        memcpy(&expected_word, expected_bytes + index, sizeof(expected_word));
        memcpy(&actual_word, actual_bytes + index, sizeof(actual_word));
        if (expected_word != actual_word) {
            snprintf(message_buf, sizeof(message_buf), "value mismatch at word +0x%zX", index / sizeof(uint32_t));
            snprintf(expected_buf, sizeof(expected_buf), "0x%08" PRIX32, expected_word);
            snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, actual_word);
            free(expected_bytes);
            validate_fail(caller_file, caller_line, name, message_buf, expected_buf, actual_buf);
        }
        index += sizeof(uint32_t);
    }

    while (index < dump_size) {
        if (expected_bytes[index] != actual_bytes[index]) {
            snprintf(message_buf, sizeof(message_buf), "value mismatch at byte +0x%zX", index);
            snprintf(expected_buf, sizeof(expected_buf), "0x%02X", expected_bytes[index]);
            snprintf(actual_buf, sizeof(actual_buf), "0x%02X", actual_bytes[index]);
            free(expected_bytes);
            validate_fail(caller_file, caller_line, name, message_buf, expected_buf, actual_buf);
        }
        index += 1;
    }

    free(expected_bytes);
}

static void validate_word_array(
    const char* caller_file, int caller_line, const char* name, const void* ptr, const VALIDATE_ENTRY* entry
) {
    char expected_buf[64];
    char actual_buf[64];
    char message_buf[96];
    const uint32_t* actual_words = ptr;
    size_t index;

    for (index = 0; index < entry->word_count; ++index) {
        if (entry->word_values[index] != actual_words[index]) {
            snprintf(message_buf, sizeof(message_buf), "value mismatch at word +0x%zX", index);
            snprintf(expected_buf, sizeof(expected_buf), "0x%08" PRIX32, entry->word_values[index]);
            snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, actual_words[index]);
            validate_fail(caller_file, caller_line, name, message_buf, expected_buf, actual_buf);
        }
    }
}

void mame_validate_word_impl(const char* caller_file, int caller_line, const char* name, const void* ptr) {
    char actual_name[128];
    VALIDATE_ENTRY entry;
    char expected_buf[32];
    char actual_buf[32];
    uint32_t expected_value = *(const uint32_t*)ptr;

    if (!read_next_validate_line(actual_name, sizeof(actual_name), &entry)) {
        validate_fail(caller_file, caller_line, name, "reached end of error.log before next validate line", "<validate line>", "<eof>");
    }

    if (strcmp(actual_name, name) != 0) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", name);
        snprintf(actual_buf, sizeof(actual_buf), "%s", actual_name);
        validate_fail(caller_file, caller_line, name, "variable name mismatch", expected_buf, actual_buf);
    }

    if (entry.kind == VALIDATE_KIND_FILE) {
        validate_dump_bytes(caller_file, caller_line, name, ptr, entry.file_path);
        return;
    }

    if (entry.kind == VALIDATE_KIND_WORD_ARRAY) {
        validate_word_array(caller_file, caller_line, name, ptr, &entry);
        return;
    }

    if (entry.word_value != expected_value) {
        snprintf(expected_buf, sizeof(expected_buf), "0x%08" PRIX32, entry.word_value);
        snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, expected_value);
        validate_fail(caller_file, caller_line, name, "value mismatch", expected_buf, actual_buf);
    }

    validate_pass_word(name, expected_value);
}

void mame_validate_arg_impl(const char* caller_file, int caller_line, const char* name, const void* ptr) {
    char actual_name[128];
    char expected_buf[128];
    char actual_buf[128];
    VALIDATE_ENTRY entry;
    const char* expected_symbol;
    const char* actual_symbol;
    uint32_t expected_rom_address = 0;

    expected_symbol = lookup_port_symbol_name(ptr);

    if (!read_next_validate_line(actual_name, sizeof(actual_name), &entry)) {
        validate_fail(caller_file, caller_line, name, "reached end of error.log before next validate line", "<validate line>", "<eof>");
    }

    if (strcmp(actual_name, name) != 0) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", name);
        snprintf(actual_buf, sizeof(actual_buf), "%s", actual_name);
        validate_fail(caller_file, caller_line, name, "register name mismatch", expected_buf, actual_buf);
    }

    if (entry.kind != VALIDATE_KIND_WORD) {
        validate_fail(caller_file, caller_line, name, "validate line kind mismatch", "<word>", "<non-word>");
    }

    if (expected_symbol == NULL) {
        if (!lookup_rom_word_address(ptr, &expected_rom_address)) {
            snprintf(actual_buf, sizeof(actual_buf), "%p", ptr);
            validate_fail(caller_file, caller_line, name, "address not found in port.map", "<mapped symbol or ROM address>", actual_buf);
        }
        if (entry.word_value != expected_rom_address) {
            snprintf(expected_buf, sizeof(expected_buf), "0x%08" PRIX32, expected_rom_address);
            snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, entry.word_value);
            validate_fail(caller_file, caller_line, name, "ROM pointer mismatch", expected_buf, actual_buf);
        }
        validate_pass_arg_rom(name, expected_rom_address);
        return;
    }

    actual_symbol = lookup_rom_symbol_name(entry.word_value);
    if (actual_symbol == NULL) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", expected_symbol);
        snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, entry.word_value);
        validate_fail(caller_file, caller_line, name, "ROM address not found in address.map", expected_buf, actual_buf);
    }

    if (strcmp(expected_symbol, actual_symbol) != 0) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", expected_symbol);
        snprintf(actual_buf, sizeof(actual_buf), "%s (%s=0x%08" PRIX32 ")", actual_symbol, actual_name, entry.word_value);
        validate_fail(caller_file, caller_line, name, "symbol name mismatch", expected_buf, actual_buf);
    }

    validate_pass_arg(name, expected_symbol);
}

#include "validator.h"
#include "c3x_float.h"
#include "machine.h"

#include <dlfcn.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

static FILE* g_validate_log;
static int g_validate_maps_loaded;
int mame_validate_disabled = 0;
static int print_oks = 0;
static int abort_on_error = 1;
static int fail_on_wrong_consumer = 0;
static int g_validate_log_line_number = 0;
static int validate_log_exhausted = 0;
static int validate_current_call_failed = 0;
static int g_validate_environment_checked = 0;

static void validate_fail(
    const char* caller_file,
    int caller_line,
    int validate_line_number,
    const char* name,
    const char* message,
    const char* expected,
    const char* actual);
static void validate_warn_log_exhausted(const char* caller_file, int caller_line, const char* name);

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
    VALIDATE_KIND_FILE,
} VALIDATE_KIND;

typedef struct VALIDATE_ENTRY {
    VALIDATE_KIND kind;
    uint32_t word_value;
    char file_path[260];
    int line_number;
    char writer_file[128];
    int writer_line;
} VALIDATE_ENTRY;

static void validate_fail(
    const char* caller_file,
    int caller_line,
    int validate_line_number,
    const char* name,
    const char* message,
    const char* expected,
    const char* actual);

void mame_validate_abort_on_error(int abort) {
    abort_on_error = abort;
}

void mame_validate_fail_on_wrong_consumer(int enabled) {
    fail_on_wrong_consumer = enabled;
}

void mame_validate_print_oks_on(void) {
    print_oks = 1;
}
void mame_validate_print_oks_off(void) {
    print_oks = 0;
}

static void fail() {
    if (abort_on_error) {
        abort();
    }
}

static int should_skip_validation(void) {
    if (!g_validate_environment_checked) {
        if (getenv("CRUSN_DISABLE_MAME_VALIDATION") != NULL) {
            mame_validate_disabled = 1;
        }
        g_validate_environment_checked = 1;
    }

    return mame_validate_disabled || validate_log_exhausted;
}

static int validate_failed(void) {
    return validate_current_call_failed;
}

static FILE* open_validate_log(void) {
    if (g_validate_log != NULL) {
        return g_validate_log;
    }

    g_validate_log = fopen("mame_validate/mame.log", "r");
    if (g_validate_log == NULL) {
        fprintf(stderr, "validator: failed to open mame.log\n");
        fflush(stderr);
        fail();
    }

    g_validate_log_line_number = 0;
    validate_log_exhausted = 0;

    return g_validate_log;
}

static VALIDATE_SYMBOL_ENTRY* reserve_symbol_entry(VALIDATE_SYMBOL_MAP* map) {
    VALIDATE_SYMBOL_ENTRY* grown_entries = realloc(map->entries, (map->count + 1) * sizeof(VALIDATE_SYMBOL_ENTRY));
    if (grown_entries == NULL) {
        fprintf(stderr, "validator: failed to grow symbol map\n");
        fflush(stderr);
        fail();
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

static const char* basename_only(const char* path) {
    const char* slash;

    if (path == NULL) {
        return "<unknown>";
    }

    slash = strrchr(path, '/');
    return slash != NULL ? slash + 1 : path;
}

static void parse_validate_writer_context(const char* line, VALIDATE_ENTRY* out_entry) {
    const char* comma;
    char writer_file[128];
    int writer_line = 0;

    out_entry->writer_file[0] = '\0';
    out_entry->writer_line = 0;

    if (line == NULL || out_entry == NULL) {
        return;
    }

    comma = strrchr(line, ',');
    if (comma == NULL) {
        return;
    }

    if (sscanf(comma + 1, " %127[^:]:%d", writer_file, &writer_line) == 2) {
        snprintf(out_entry->writer_file, sizeof(out_entry->writer_file), "%s", writer_file);
        out_entry->writer_line = writer_line;
    }
}

static int validate_entry_has_wrong_consumer(const char* caller_file, int caller_line, const VALIDATE_ENTRY* entry) {
    const char* caller_basename;

    if (entry == NULL || entry->writer_file[0] == '\0') {
        return 0;
    }

    caller_basename = basename_only(caller_file);
    return strcmp(entry->writer_file, caller_basename) != 0 || entry->writer_line != caller_line;
}

static int validate_wrong_consumer(
    const char* caller_file, int caller_line, const char* name, const VALIDATE_ENTRY* entry) {
    char expected_buf[160];
    char actual_buf[160];

    if (!validate_entry_has_wrong_consumer(caller_file, caller_line, entry)) {
        return 0;
    }

    snprintf(expected_buf, sizeof(expected_buf), "%s:%d", entry->writer_file, entry->writer_line);
    snprintf(actual_buf, sizeof(actual_buf), "%s:%d", basename_only(caller_file), caller_line);

    if (fail_on_wrong_consumer) {
        validate_fail(
            caller_file,
            caller_line,
            entry->line_number,
            name,
            "validate line consumed by different callsite than original writer",
            expected_buf,
            actual_buf);
        return 1;
    }

    fprintf(stderr, "warning: original writer was %s\n", expected_buf);
    fflush(stderr);

    return 0;
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
        fail();
    }
    if (g_address_map.count == 0) {
        fprintf(stderr, "validator: failed to load tools/ida/address.map symbols\n");
        fflush(stderr);
        fail();
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

static int lookup_map_address_by_name(const VALIDATE_SYMBOL_MAP* map, const char* name, uint32_t* out_address) {
    size_t index;

    if (name == NULL || out_address == NULL) {
        return 0;
    }

    for (index = 0; index < map->count; ++index) {
        if (strcmp(map->entries[index].name, name) == 0) {
            *out_address = (uint32_t)map->entries[index].address;
            return 1;
        }
    }

    return 0;
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

static int lookup_rom_symbol_address(const char* name, uint32_t* out_address) {
    load_validate_maps();
    return lookup_map_address_by_name(&g_address_map, name, out_address);
}

static int lookup_function_rom_address(const void* address, uint32_t* out_address) {
    Dl_info info;
    const char* symbol_name;
    const char* normalized_name;

    if (address == NULL || out_address == NULL) {
        return 0;
    }

    if (dladdr(address, &info) == 0 || info.dli_sname == NULL) {
        return 0;
    }

    symbol_name = info.dli_sname;
    normalized_name = strip_macho_prefix(symbol_name);
    return lookup_rom_symbol_address(normalized_name, out_address);
}

static int lookup_caller_breakpoint_address(const void* return_address, uint32_t* out_address) {
    return lookup_function_rom_address(return_address, out_address);
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

static void validate_fail(
    const char* caller_file,
    int caller_line,
    int validate_line_number,
    const char* name,
    const char* message,
    const char* expected,
    const char* actual) {
    fprintf(
        stderr,
        "%s:%d: validation(%s) failed at mame.log:%d: %s; expected %s but was %s\n",
        caller_file,
        caller_line,
        name,
        validate_line_number,
        message,
        expected,
        actual);
    fflush(stderr);
    validate_current_call_failed = 1;
    fail();
}

static void validate_warn_log_exhausted(const char* caller_file, int caller_line, const char* name) {
    fprintf(
        stderr,
        "%s:%d: validation(%s) warning: reached end of mame.log before next validate line\n",
        caller_file,
        caller_line,
        name);
    fflush(stderr);
}

static void validate_fail_with_detail(
    const char* caller_file,
    int caller_line,
    int validate_line_number,
    const char* name,
    const char* message,
    const char* expected,
    const char* actual,
    const char* detail) {
    if (detail != NULL && detail[0] != '\0') {
        fprintf(
            stderr,
            "%s:%d: validation(%s) failed at mame.log:%d: %s; expected %s but was %s. %s\n",
            caller_file,
            caller_line,
            name,
            validate_line_number,
            message,
            expected,
            actual,
            detail);
    } else {
        fprintf(
            stderr,
            "%s:%d: validation(%s) failed at mame.log:%d: %s; expected %s but was %s\n",
            caller_file,
            caller_line,
            name,
            validate_line_number,
            message,
            expected,
            actual);
    }
    fflush(stderr);
    validate_current_call_failed = 1;
    fail();
}

static void validate_pass_word(
    const char* caller_file,
    int caller_line,
    const char* name,
    uint32_t value,
    const VALIDATE_ENTRY* entry) {
    const char* caller_basename = basename_only(caller_file);

    validate_wrong_consumer(caller_file, caller_line, name, entry);

    if (print_oks) {
        fprintf(
            stderr,
            "mame.log:%d %s=0x%08" PRIX32 ", consumer %s:%d\n",
            entry->line_number,
            name,
            value,
            caller_basename,
            caller_line);
        fflush(stderr);
    }
}

static void validate_pass_arg(
    const char* caller_file,
    int caller_line,
    const char* reg_name,
    const char* symbol_name,
    const VALIDATE_ENTRY* entry) {
    const char* caller_basename = basename_only(caller_file);

    validate_wrong_consumer(caller_file, caller_line, reg_name, entry);

    if (print_oks) {
        fprintf(
            stderr,
            "mame.log:%d %s=%s, consumer %s:%d\n",
            entry->line_number,
            reg_name,
            symbol_name,
            caller_basename,
            caller_line);
        fflush(stderr);
    }
}

static void validate_pass_arg_rom(
    const char* caller_file,
    int caller_line,
    const char* reg_name,
    uint32_t rom_address,
    const VALIDATE_ENTRY* entry) {
    const char* caller_basename = basename_only(caller_file);

    validate_wrong_consumer(caller_file, caller_line, reg_name, entry);

    if (print_oks) {
        const char* symbol_name = lookup_rom_symbol_name(rom_address);

        if (symbol_name != NULL) {
            fprintf(
                stderr,
                "mame.log:%d %s=%s, consumer %s:%d\n",
                entry->line_number,
                reg_name,
                symbol_name,
                caller_basename,
                caller_line);
        } else {
            fprintf(
                stderr,
                "mame.log:%d %s=0x%08" PRIX32 ", consumer %s:%d\n",
                entry->line_number,
                reg_name,
                rom_address,
                caller_basename,
                caller_line);
        }
        fflush(stderr);
    }
}

static void format_float_reg_name(const char* reg_name, char* out_name, size_t out_name_size) {
    snprintf(out_name, out_name_size, "%sF", reg_name);
}

static int read_next_validate_line(char* out_name, size_t out_name_size, VALIDATE_ENTRY* out_entry) {
    char line[512];
    FILE* log = open_validate_log();

    if (validate_log_exhausted) {
        return 0;
    }

    while (fgets(line, sizeof(line), log) != NULL) {
        g_validate_log_line_number += 1;
        char name_buf[128];
        char file_buf[260];
        unsigned int value = 0;
        int matched = sscanf(line, "validate %127[^:]: 0x%X", name_buf, &value);
        if (matched != 2) {
            matched = sscanf(line, "\xEF\xBB\xBFvalidate %127[^:]: 0x%X", name_buf, &value);
        }
        if (matched == 2) {
            snprintf(out_name, out_name_size, "%s", name_buf);
            out_entry->kind = VALIDATE_KIND_WORD;
            out_entry->word_value = (uint32_t)value;
            out_entry->file_path[0] = '\0';
            out_entry->line_number = g_validate_log_line_number;
            parse_validate_writer_context(line, out_entry);
            return 1;
        }

        matched = sscanf(line, "validate %127[^:]: file=%259s", name_buf, file_buf);
        if (matched != 2) {
            matched = sscanf(line, "\xEF\xBB\xBFvalidate %127[^:]: file=%259s", name_buf, file_buf);
        }
        if (matched == 2) {
            char* comma = strchr(file_buf, ',');
            if (comma != NULL) {
                *comma = '\0';
            }
            snprintf(out_name, out_name_size, "%s", name_buf);
            out_entry->kind = VALIDATE_KIND_FILE;
            out_entry->word_value = 0;
            snprintf(out_entry->file_path, sizeof(out_entry->file_path), "mame_validate/%s", file_buf);
            out_entry->line_number = g_validate_log_line_number;
            parse_validate_writer_context(line, out_entry);
            return 1;
        }
    }

    validate_log_exhausted = 1;
    return 0;
}

static int read_next_validate_reg_word(
    const char* caller_file,
    int caller_line,
    const char* failure_name,
    const char* expected_reg_name,
    VALIDATE_ENTRY* out_entry) {
    char actual_name[128];
    char expected_buf[128];
    char actual_buf[128];

    if (!read_next_validate_line(actual_name, sizeof(actual_name), out_entry)) {
        validate_warn_log_exhausted(caller_file, caller_line, failure_name);
        return 0;
    }

    if (validate_wrong_consumer(caller_file, caller_line, failure_name, out_entry)) {
        return 0;
    }

    if (strcmp(actual_name, expected_reg_name) != 0) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", expected_reg_name);
        snprintf(actual_buf, sizeof(actual_buf), "%s", actual_name);
        validate_fail(caller_file, caller_line, out_entry->line_number, failure_name, "register name mismatch", expected_buf, actual_buf);
        return 0;
    }

    if (out_entry->kind != VALIDATE_KIND_WORD) {
        validate_fail(caller_file, caller_line, out_entry->line_number, failure_name, "validate line kind mismatch", "<word>", "<non-word>");
        return 0;
    }

    return 1;
}

static void validate_reg_word_value_impl(
    const char* caller_file,
    int caller_line,
    const char* failure_name,
    const char* expected_reg_name,
    const void* ptr,
    uint32_t wiggle_room) {
    VALIDATE_ENTRY entry;
    char expected_buf[32];
    char actual_buf[32];
    char reason_buf[64];
    uint32_t expected_value = 0;
    uint32_t diff = 0;

    memcpy(&expected_value, ptr, sizeof(expected_value));
    if (!read_next_validate_reg_word(caller_file, caller_line, failure_name, expected_reg_name, &entry)) {
        return;
    }

    diff = entry.word_value > expected_value ? (entry.word_value - expected_value) : (expected_value - entry.word_value);
    if (diff > wiggle_room) {
        snprintf(expected_buf, sizeof(expected_buf), "0x%08" PRIX32, entry.word_value);
        snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, expected_value);
        if (wiggle_room != 0) {
            snprintf(reason_buf, sizeof(reason_buf), "value mismatch (wiggle=0x%08" PRIX32 ")", wiggle_room);
            validate_fail(caller_file, caller_line, entry.line_number, failure_name, reason_buf, expected_buf, actual_buf);
            return;
        }
        validate_fail(caller_file, caller_line, entry.line_number, failure_name, "value mismatch", expected_buf, actual_buf);
        return;
    }

    validate_pass_word(caller_file, caller_line, expected_reg_name, expected_value, &entry);
}

static void validate_reg_float_value_impl(
    const char* caller_file,
    int caller_line,
    const char* failure_name,
    const char* expected_reg_name,
    const void* ptr,
    uint32_t wiggle_room) {
    VALIDATE_ENTRY entry;
    char expected_buf[64];
    char actual_buf[64];
    char reason_buf[96];
    float expected_value = 0.0f;
    float actual_value = 0.0f;
    uint32_t actual_word = 0;
    uint32_t decimal_places = wiggle_room != 0 ? wiggle_room - 1 : 2;
    double decimal_scale;
    double allowed_difference;

    if (!read_next_validate_reg_word(caller_file, caller_line, failure_name, expected_reg_name, &entry)) {
        return;
    }

    memcpy(&expected_value, &entry.word_value, sizeof(expected_value));
    actual_value = C3X_TO_FLOAT(*(const c3x_reg_t*)ptr);
    memcpy(&actual_word, &actual_value, sizeof(actual_word));

    decimal_scale = pow(10.0, (double)decimal_places);
    allowed_difference = 1.0 / decimal_scale;

    uint32_t expected_ordered = (entry.word_value & 0x80000000u) != 0
        ? ~entry.word_value
        : entry.word_value | 0x80000000u;
    uint32_t actual_ordered = (actual_word & 0x80000000u) != 0
        ? ~actual_word
        : actual_word | 0x80000000u;
    uint32_t ulp_diff = expected_ordered > actual_ordered
        ? expected_ordered - actual_ordered
        : actual_ordered - expected_ordered;

    if ((!isfinite(expected_value) || !isfinite(actual_value))
            ? expected_value != actual_value
            : fabs((double)actual_value - (double)expected_value) > allowed_difference && ulp_diff > 2) {
        snprintf(expected_buf, sizeof(expected_buf), "%g (0x%08" PRIX32 ")", expected_value, entry.word_value);
        snprintf(actual_buf, sizeof(actual_buf), "%g (0x%08" PRIX32 ")", actual_value, actual_word);
        snprintf(
            reason_buf,
            sizeof(reason_buf),
            "value mismatch at %" PRIu32 " decimal places",
            decimal_places);
        validate_fail(caller_file, caller_line, entry.line_number, failure_name, reason_buf, expected_buf, actual_buf);
        return;
    }

    validate_pass_word(caller_file, caller_line, expected_reg_name, actual_word, &entry);
}

static void validate_stored_float_value_impl(
    const char* caller_file,
    int caller_line,
    const char* failure_name,
    const char* expected_reg_name,
    const void* ptr,
    uint32_t wiggle_room) {
    c3x_reg_t value = C3X_LDF(*(const c3x_f32_t*)ptr);
    validate_reg_float_value_impl(caller_file, caller_line, failure_name, expected_reg_name, &value, wiggle_room);
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

static int should_dump_screen0_validation_bitmaps(const char* name, size_t dump_size) {
    return strcmp(name, "SCREEN0") == 0 && dump_size >= (CRUSN_SCREEN_WIDTH * CRUSN_SCREEN_HEIGHT * sizeof(uint32_t));
}

static void dump_screen0_validation_bitmaps(const char* name, const uint8_t* expected_bytes, const uint8_t* actual_bytes, size_t dump_size) {
    if (!should_dump_screen0_validation_bitmaps(name, dump_size)) {
        return;
    }
    if (g_crusn_machine == NULL || expected_bytes == NULL || actual_bytes == NULL) {
        return;
    }

    (void)crusn_machine_dump_screen_bmp(
        g_crusn_machine,
        (const u32*)expected_bytes,
        g_crusn_machine->colorram_words,
        "build/validate_SCREEN0_expected.bmp");
    (void)crusn_machine_dump_screen_bmp(
        g_crusn_machine,
        (const u32*)actual_bytes,
        g_crusn_machine->colorram_words,
        "build/validate_SCREEN0_actual.bmp");
}

static int write_validate_dump(const char* path, const uint8_t* bytes, size_t size) {
    FILE* dump;

    if (path == NULL || bytes == NULL) {
        return 0;
    }

    dump = fopen(path, "wb");
    if (dump == NULL) {
        return 0;
    }

    if (size != 0 && fwrite(bytes, 1, size, dump) != size) {
        fclose(dump);
        return 0;
    }

    if (fclose(dump) != 0) {
        return 0;
    }

    return 1;
}

static void dump_nonmatching_region(
    const char* name,
    const char* expected_bin_path,
    const uint8_t* expected_bytes,
    const uint8_t* actual_bytes,
    size_t dump_size,
    char* actual_bin_path,
    size_t actual_bin_path_size) {
    const char* extension;
    size_t prefix_length;

    if (actual_bin_path_size != 0) {
        actual_bin_path[0] = '\0';
    }

    if (expected_bin_path == NULL || actual_bytes == NULL) {
        return;
    }

    extension = strrchr(expected_bin_path, '.');
    if (extension != NULL && strcmp(extension, ".bin") == 0) {
        prefix_length = (size_t)(extension - expected_bin_path);
        snprintf(actual_bin_path, actual_bin_path_size, "%.*s-actual.bin", (int)prefix_length, expected_bin_path);
    } else {
        snprintf(actual_bin_path, actual_bin_path_size, "%s-actual.bin", expected_bin_path);
    }

    if (actual_bin_path[0] != '\0') {
        (void)write_validate_dump(actual_bin_path, actual_bytes, dump_size);
    }

    dump_screen0_validation_bitmaps(name, expected_bytes, actual_bytes, dump_size);
}

static void append_validate_dump_paths(
    char* detail_buf,
    size_t detail_buf_size,
    const char* expected_bin_path,
    const char* actual_bin_path) {
    snprintf(
        detail_buf,
        detail_buf_size,
        "Expected dump: %s. Actual dump: %s.",
        expected_bin_path != NULL ? expected_bin_path : "<unknown>",
        (actual_bin_path != NULL && actual_bin_path[0] != '\0') ? actual_bin_path : "<failed to write actual dump>");
}

static void validate_region(
    const char* caller_file,
    int caller_line,
    int validate_line_number,
    const char* name,
    const void* ptr,
    const char* dump_path,
    size_t expected_size) {
    char expected_buf[320];
    char actual_buf[320];
    char actual_dump_path[320];
    char message_buf[128];
    char detail_buf[512];
    size_t dump_size = 0;
    size_t actual_dump_size = 0;
    size_t index = 0;
    const uint8_t* actual_bytes = ptr;
    uint8_t* expected_bytes = read_validate_dump(dump_path, &dump_size);

    if (expected_bytes == NULL && dump_size != 0) {
        validate_fail(caller_file, caller_line, validate_line_number, name, "failed to read validate dump file", dump_path, "<unreadable>");
        return;
    }

    actual_dump_size = expected_size != 0 ? expected_size : dump_size;

    if (expected_size != 0 && dump_size != expected_size) {
        dump_nonmatching_region(name, dump_path, expected_bytes, actual_bytes, actual_dump_size, actual_dump_path, sizeof(actual_dump_path));
        snprintf(message_buf, sizeof(message_buf), "dump size mismatch");
        append_validate_dump_paths(detail_buf, sizeof(detail_buf), dump_path, actual_dump_path);
        snprintf(expected_buf, sizeof(expected_buf), "0x%zX bytes", expected_size);
        snprintf(actual_buf, sizeof(actual_buf), "0x%zX bytes", dump_size);
        free(expected_bytes);
        validate_fail_with_detail(caller_file, caller_line, validate_line_number, name, message_buf, expected_buf, actual_buf, detail_buf);
        return;
    }

    while (index + sizeof(uint32_t) <= dump_size) {
        uint32_t expected_word = 0;
        uint32_t actual_word = 0;
        memcpy(&expected_word, expected_bytes + index, sizeof(expected_word));
        memcpy(&actual_word, actual_bytes + index, sizeof(actual_word));
        if (expected_word != actual_word) {
            dump_nonmatching_region(name, dump_path, expected_bytes, actual_bytes, actual_dump_size, actual_dump_path, sizeof(actual_dump_path));
            snprintf(message_buf, sizeof(message_buf), "value mismatch at word +0x%zX", index / sizeof(uint32_t));
            append_validate_dump_paths(detail_buf, sizeof(detail_buf), dump_path, actual_dump_path);
            snprintf(expected_buf, sizeof(expected_buf), "0x%08" PRIX32, expected_word);
            snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, actual_word);
            free(expected_bytes);
            validate_fail_with_detail(caller_file, caller_line, validate_line_number, name, message_buf, expected_buf, actual_buf, detail_buf);
            return;
        }
        index += sizeof(uint32_t);
    }

    while (index < dump_size) {
        if (expected_bytes[index] != actual_bytes[index]) {
            dump_nonmatching_region(name, dump_path, expected_bytes, actual_bytes, actual_dump_size, actual_dump_path, sizeof(actual_dump_path));
            snprintf(message_buf, sizeof(message_buf), "value mismatch at byte +0x%zX", index);
            append_validate_dump_paths(detail_buf, sizeof(detail_buf), dump_path, actual_dump_path);
            snprintf(expected_buf, sizeof(expected_buf), "0x%02X", expected_bytes[index]);
            snprintf(actual_buf, sizeof(actual_buf), "0x%02X", actual_bytes[index]);
            free(expected_bytes);
            validate_fail_with_detail(caller_file, caller_line, validate_line_number, name, message_buf, expected_buf, actual_buf, detail_buf);
            return;
        }
        index += 1;
    }

    free(expected_bytes);
    if (print_oks) {
        fprintf(stderr, "mame.log:%d %s, consumer %s:%d\n", validate_line_number, name, basename_only(caller_file), caller_line);
    }
}

void mame_validate_arg_sym_impl(const char* caller_file, int caller_line, const char* name, const void* ptr) {
    char actual_name[128];
    char expected_buf[128];
    char actual_buf[128];
    VALIDATE_ENTRY entry;
    const char* expected_symbol;
    const char* actual_symbol;
    uint32_t expected_rom_address = 0;

    if (should_skip_validation()) {
        return;
    }

    validate_current_call_failed = 0;

    expected_symbol = lookup_port_symbol_name(ptr);

    if (!read_next_validate_line(actual_name, sizeof(actual_name), &entry)) {
        validate_warn_log_exhausted(caller_file, caller_line, name);
        return;
    }

    if (validate_wrong_consumer(caller_file, caller_line, name, &entry)) {
        return;
    }

    if (strcmp(actual_name, name) != 0) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", name);
        snprintf(actual_buf, sizeof(actual_buf), "%s", actual_name);
        validate_fail(caller_file, caller_line, entry.line_number, name, "register name mismatch", expected_buf, actual_buf);
        return;
    }

    if (entry.kind != VALIDATE_KIND_WORD) {
        validate_fail(caller_file, caller_line, entry.line_number, name, "validate line kind mismatch", "<word>", "<non-word>");
        return;
    }

    if (expected_symbol == NULL) {
        if (!lookup_rom_word_address(ptr, &expected_rom_address)) {
            snprintf(actual_buf, sizeof(actual_buf), "%p", ptr);
            validate_fail(caller_file, caller_line, entry.line_number, name, "address not found in port.map", "<mapped symbol or ROM address>", actual_buf);
            return;
        }
        if (entry.word_value != expected_rom_address) {
            snprintf(expected_buf, sizeof(expected_buf), "0x%08" PRIX32, expected_rom_address);
            snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, entry.word_value);
            validate_fail(caller_file, caller_line, entry.line_number, name, "ROM pointer mismatch", expected_buf, actual_buf);
            return;
        }
        validate_pass_arg_rom(caller_file, caller_line, name, expected_rom_address, &entry);
        return;
    }

    actual_symbol = lookup_rom_symbol_name(entry.word_value);
    if (actual_symbol == NULL) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", expected_symbol);
        snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, entry.word_value);
        validate_fail(caller_file, caller_line, entry.line_number, name, "ROM address not found in address.map", expected_buf, actual_buf);
        return;
    }

    if (strcmp(expected_symbol, actual_symbol) != 0) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", expected_symbol);
        snprintf(actual_buf, sizeof(actual_buf), "%s (%s=0x%08" PRIX32 ")", actual_symbol, actual_name, entry.word_value);
        validate_fail(caller_file, caller_line, entry.line_number, name, "symbol name mismatch", expected_buf, actual_buf);
        return;
    }

    validate_pass_arg(caller_file, caller_line, name, expected_symbol, &entry);
}

void mame_validate_arg_impl(const char* caller_file, int caller_line, const char* name, const void* ptr) {
    uint32_t breakpoint_address = 0;
    void* return_address = __builtin_return_address(0);
    char actual_buf[64];

    if (should_skip_validation()) {
        return;
    }

    validate_current_call_failed = 0;

    if (!lookup_caller_breakpoint_address(return_address, &breakpoint_address)) {
        snprintf(actual_buf, sizeof(actual_buf), "%p", return_address);
        validate_fail(
            caller_file,
            caller_line,
            g_validate_log_line_number + 1,
            name,
            "caller function address not found in address.map",
            "<mapped function symbol>",
            actual_buf);
        return;
    }

    mame_assert_reg_at_addr_impl(caller_file, caller_line, breakpoint_address, name, ptr, MAME_VALIDATE_REG_KIND_WORD, 0);
}

void mame_assert_arg_float_impl(const char* caller_file, int caller_line, const char* name, const void* ptr) {
    uint32_t breakpoint_address = 0;
    void* return_address = __builtin_return_address(0);
    char actual_buf[64];

    if (should_skip_validation()) {
        return;
    }

    validate_current_call_failed = 0;

    if (!lookup_caller_breakpoint_address(return_address, &breakpoint_address)) {
        snprintf(actual_buf, sizeof(actual_buf), "%p", return_address);
        validate_fail(
            caller_file,
            caller_line,
            g_validate_log_line_number + 1,
            name,
            "caller function address not found in address.map",
            "<mapped function symbol>",
            actual_buf);
        return;
    }

    mame_assert_reg_at_addr_impl(caller_file, caller_line, breakpoint_address, name, ptr, MAME_VALIDATE_REG_KIND_FLOAT, 0);
}

void mame_validate_exit_impl(const char* caller_file, int caller_line) {
    if (should_skip_validation()) {
        return;
    }

    validate_current_call_failed = 0;

    (void)caller_file;
    (void)caller_line;
}

void mame_assert_reg_at_addr_impl(
    const char* caller_file,
    int caller_line,
    uint32_t breakpoint_address,
    const char* reg_name,
    const void* ptr,
    MAME_VALIDATE_REG_KIND reg_kind,
    uint32_t wiggle_room) {
    char float_reg_name[128];

    if (should_skip_validation()) {
        return;
    }

    validate_current_call_failed = 0;

    (void)breakpoint_address;

    if (reg_kind == MAME_VALIDATE_REG_KIND_FLOAT || reg_kind == MAME_VALIDATE_REG_KIND_STORED_FLOAT) {
        format_float_reg_name(reg_name, float_reg_name, sizeof(float_reg_name));
        if (reg_kind == MAME_VALIDATE_REG_KIND_STORED_FLOAT) {
            validate_stored_float_value_impl(caller_file, caller_line, reg_name, float_reg_name, ptr, wiggle_room);
        } else {
            validate_reg_float_value_impl(caller_file, caller_line, reg_name, float_reg_name, ptr, wiggle_room);
        }
        return;
    }

    validate_reg_word_value_impl(caller_file, caller_line, reg_name, reg_name, ptr, wiggle_room);
}

void mame_validate_region_at_addr_impl(
    const char* caller_file,
    int caller_line,
    uint32_t breakpoint_address,
    const char* name,
    uint32_t region_address,
    const void* ptr,
    uint32_t word_count) {
    char actual_name[128];
    char expected_buf[64];
    char actual_buf[64];
    VALIDATE_ENTRY entry;

    if (should_skip_validation()) {
        return;
    }

    validate_current_call_failed = 0;

    (void)breakpoint_address;
    (void)region_address;

    if (!read_next_validate_line(actual_name, sizeof(actual_name), &entry)) {
        validate_warn_log_exhausted(caller_file, caller_line, name);
        return;
    }

    if (validate_wrong_consumer(caller_file, caller_line, name, &entry)) {
        return;
    }

    if (strcmp(actual_name, name) != 0) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", name);
        snprintf(actual_buf, sizeof(actual_buf), "%s", actual_name);
        validate_fail(caller_file, caller_line, entry.line_number, name, "variable name mismatch", expected_buf, actual_buf);
        return;
    }

    if (entry.kind != VALIDATE_KIND_FILE) {
        validate_fail(caller_file, caller_line, entry.line_number, name, "validate line kind mismatch", "<file>", "<non-file>");
        return;
    }

    validate_region(caller_file, caller_line, entry.line_number, name, ptr, entry.file_path, (size_t)word_count * sizeof(uint32_t));
}

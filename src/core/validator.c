#include "validator.h"

#include <inttypes.h>
#include <stdnoreturn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* g_validate_log;

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

static int read_next_validate_line(char* out_name, size_t out_name_size, uint32_t* out_value) {
    char line[512];
    FILE* log = open_validate_log();

    while (fgets(line, sizeof(line), log) != NULL) {
        char name_buf[128];
        unsigned int value = 0;
        int matched = sscanf(line, "validate %127[^:]: 0x%X", name_buf, &value);
        if (matched != 2) {
            matched = sscanf(line, "\xEF\xBB\xBFvalidate %127[^:]: 0x%X", name_buf, &value);
        }
        if (matched != 2) {
            continue;
        }

        snprintf(out_name, out_name_size, "%s", name_buf);
        *out_value = (uint32_t)value;
        return 1;
    }

    return 0;
}

void mame_validate_word_impl(const char* caller_file, int caller_line, const char* name, const void* ptr) {
    char actual_name[128];
    char expected_buf[32];
    char actual_buf[32];
    uint32_t actual_value = 0;
    uint32_t expected_value = *(const uint32_t*)ptr;

    if (!read_next_validate_line(actual_name, sizeof(actual_name), &actual_value)) {
        validate_fail(caller_file, caller_line, name, "reached end of error.log before next validate line", "<validate line>", "<eof>");
    }

    if (strcmp(actual_name, name) != 0) {
        snprintf(expected_buf, sizeof(expected_buf), "%s", name);
        snprintf(actual_buf, sizeof(actual_buf), "%s", actual_name);
        validate_fail(caller_file, caller_line, name, "variable name mismatch", expected_buf, actual_buf);
    }

    if (actual_value != expected_value) {
        snprintf(expected_buf, sizeof(expected_buf), "0x%08" PRIX32, actual_value);
        snprintf(actual_buf, sizeof(actual_buf), "0x%08" PRIX32, expected_value);
        validate_fail(caller_file, caller_line, name, "value mismatch", expected_buf, actual_buf);
    }
}

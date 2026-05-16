#include "TEXT_DEBUG.h"

#include <stdio.h>
#include <string.h>

#define TEXT_DEBUG_MAX_ENTRIES 256
#define TEXT_DEBUG_MAX_CHARS 160

typedef struct TextDebugEntry {
    char text[TEXT_DEBUG_MAX_CHARS];
    int x;
    int y;
    int color_index;
} TextDebugEntry;

static TextDebugEntry g_entries[TEXT_DEBUG_MAX_ENTRIES];
static int g_count;

void TEXT_DEBUG_RECORD(const char *text, int x, int y, int color_index) {
    if (!text || g_count >= TEXT_DEBUG_MAX_ENTRIES) {
        return;
    }
    TextDebugEntry *e = &g_entries[g_count++];
    strncpy(e->text, text, TEXT_DEBUG_MAX_CHARS - 1);
    e->text[TEXT_DEBUG_MAX_CHARS - 1] = '\0';
    e->x = x;
    e->y = y;
    e->color_index = color_index;
}

void TEXT_DEBUG_DUMP_AND_CLEAR(void) {
    for (int i = 0; i < g_count; i++) {
        const TextDebugEntry *e = &g_entries[i];
        printf("TEXT_OUTPUT: x=%d y=%d color=%d text=\"%s\"\n", e->x, e->y, e->color_index, e->text);
    }
    fflush(stdout);
    g_count = 0;
}

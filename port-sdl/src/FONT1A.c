#include "FONT1A.h"

#include <stddef.h>

#include "FONT.h"
#include "TRACE.h"
#include "TEXT_DEBUG.h"

#include "NOT_IMPLEMENTED.h"
static const uint32_t *g_palette_rgba;
static int g_palette_len;
static uint32_t *g_framebuffer;
static int g_width;
static int g_height;

void FONT1A_set_palette(const uint32_t *palette_rgba, int palette_len) {
    TRACE();
    g_palette_rgba = palette_rgba;
    g_palette_len = palette_len;
}

void FONT1A_set_framebuffer(uint32_t *framebuffer, int width, int height) {
    TRACE();
    g_framebuffer = framebuffer;
    g_width = width;
    g_height = height;
}

void _pixel(int x, int y, int color_index) {
    if (!g_framebuffer || !g_palette_rgba || g_palette_len <= 0) {
        return;
    }
    if (x < 0 || x >= g_width || y < 0 || y >= g_height) {
        return;
    }
    const int idx = color_index & 0x0F;
    if (idx >= g_palette_len) {
        return;
    }
    g_framebuffer[y * g_width + x] = g_palette_rgba[idx];
}

void _outtextxyc(const char *text, int x, int y, int color_index) {
    TRACE();
    TEXT_DEBUG_RECORD(text, x, y, color_index);
    for (size_t i = 0; text[i] != '\0'; i++) {
        const unsigned char ch = (unsigned char)text[i];
        if (ch != ' ') {
            for (int row = 0; row < 7; row++) {
                uint16_t bits = 0;
                const int glyph_index = (int)ch - '(';
                if (glyph_index >= 0) {
                    const int base = glyph_index * 7 + row;
                    if (base >= 0 && base < kFont1RowsCount) {
                        bits = kFont1Rows[base];
                    }
                }
                for (int col = 0; col < 8; col++) {
                    if ((bits << col) & 0x80) {
                        _pixel(x + col, y + row, color_index);
                    }
                }
            }
        }
        x += 8;
    }
}

void _ftoa(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _itoaLZ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _itoa(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HEX2ASC(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _fill(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

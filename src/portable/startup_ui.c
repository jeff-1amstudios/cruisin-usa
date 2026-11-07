#include "startup.h"
#include <stdio.h>
#include <string.h>

/* Original embedded 5x7 bitmap font. Independent of all ROM assets. */
static const unsigned char glyphs[96][7] = {
    [0] = { 0, 0, 0, 0, 0, 0, 0 },
    [33] = { 14, 17, 17, 31, 17, 17, 17 },
    [34] = { 30, 17, 17, 30, 17, 17, 30 },
    [35] = { 15, 16, 16, 16, 16, 16, 15 },
    [36] = { 30, 17, 17, 17, 17, 17, 30 },
    [37] = { 31, 16, 16, 30, 16, 16, 31 },
    [38] = { 31, 16, 16, 30, 16, 16, 16 },
    [39] = { 15, 16, 16, 23, 17, 17, 15 },
    [40] = { 17, 17, 17, 31, 17, 17, 17 },
    [41] = { 31, 4, 4, 4, 4, 4, 31 },
    [42] = { 7, 2, 2, 2, 18, 18, 12 },
    [43] = { 17, 18, 20, 24, 20, 18, 17 },
    [44] = { 16, 16, 16, 16, 16, 16, 31 },
    [45] = { 17, 27, 21, 21, 17, 17, 17 },
    [46] = { 17, 25, 21, 19, 17, 17, 17 },
    [47] = { 14, 17, 17, 17, 17, 17, 14 },
    [48] = { 30, 17, 17, 30, 16, 16, 16 },
    [49] = { 14, 17, 17, 17, 21, 18, 13 },
    [50] = { 30, 17, 17, 30, 20, 18, 17 },
    [51] = { 15, 16, 16, 14, 1, 1, 30 },
    [52] = { 31, 4, 4, 4, 4, 4, 4 },
    [53] = { 17, 17, 17, 17, 17, 17, 14 },
    [54] = { 17, 17, 17, 17, 17, 10, 4 },
    [55] = { 17, 17, 17, 21, 21, 27, 17 },
    [56] = { 17, 17, 10, 4, 10, 17, 17 },
    [57] = { 17, 17, 10, 4, 4, 4, 4 },
    [58] = { 31, 1, 2, 4, 8, 16, 31 },
    [16] = { 14, 17, 19, 21, 25, 17, 14 },
    [17] = { 4, 12, 4, 4, 4, 4, 14 },
    [18] = { 14, 17, 1, 2, 4, 8, 31 },
    [19] = { 30, 1, 1, 14, 1, 1, 30 },
    [20] = { 2, 6, 10, 18, 31, 2, 2 },
    [21] = { 31, 16, 16, 30, 1, 1, 30 },
    [22] = { 14, 16, 16, 30, 17, 17, 14 },
    [23] = { 31, 1, 2, 4, 8, 8, 8 },
    [24] = { 14, 17, 17, 14, 17, 17, 14 },
    [25] = { 14, 17, 17, 15, 1, 1, 14 },
    [14] = { 0, 0, 0, 0, 0, 6, 6 },
    [13] = { 0, 0, 0, 31, 0, 0, 0 },
    [15] = { 1, 1, 2, 4, 8, 16, 16 },
    [26] = { 0, 6, 6, 0, 6, 6, 0 },
    [5] = { 25, 25, 2, 4, 8, 19, 19 },
    [31] = { 14, 17, 1, 2, 4, 0, 4 },
    [8] = { 2, 4, 8, 8, 8, 4, 2 },
    [9] = { 8, 4, 2, 2, 2, 4, 8 },
    [7] = { 4, 4, 0, 0, 0, 0, 0 },
};

static void rect(u32* pixels, int width, int height, int x, int y, int w, int h, u32 color) {
    for (int row = y; row < y + h && row < height; ++row) {
        for (int col = x; col < x + w && col < width; ++col) {
            if (row >= 0 && col >= 0) pixels[row * width + col] = color;
        }
    }
}

static void text(u32* pixels, int width, int height, int x, int y, const char* string, int scale, u32 color) {
    int left = x;
    while (*string) {
        unsigned char ch = (unsigned char)*string++;
        if (ch == '\n' || x + 5 * scale > width - left) {
            x = left;
            y += 9 * scale;
            if (ch == '\n') continue;
        }
        if (ch >= 'a' && ch <= 'z') ch -= 'a' - 'A';
        if (ch < 32 || ch > 127) ch = '?';
        for (int row = 0; row < 7; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (glyphs[ch - 32][row] & (1u << (4 - col))) {
                    rect(pixels, width, height, x + col * scale, y + row * scale, scale, scale, color);
                }
            }
        }
        x += 6 * scale;
    }
}

void portable_startup_draw(const portable_startup* startup, u32* pixels, int width, int height, unsigned ticks) {
    const u32 white = 0xffe4edf5;
    const u32 cyan = 0xff54d9e8;
    const char* title = "CRUIS'N USA";
    const char* status = "CHECKING ROM...";
    char progress[80] = "";
    rect(pixels, width, height, 0, 0, width, height, 0xff111c28);
    text(pixels, width, height, (width - (int)strlen(title) * 18) / 2, 78, title, 3, white);
    switch (startup->phase) {
    case PORTABLE_STARTUP_DOWNLOAD:
        status = "DOWNLOADING ROM...";
        if (startup->download.total) {
            unsigned percent = (unsigned)(startup->download.received * 100 / startup->download.total);
            if (percent > 100) percent = 100;
            snprintf(progress, sizeof(progress), "%u%%  %.1f / %.1f MB", percent,
                startup->download.received / 1048576.0, startup->download.total / 1048576.0);
            rect(pixels, width, height, 48, 222, width - 96, 12, 0xff283c50);
            rect(pixels, width, height, 48, 222, (width - 96) * (int)percent / 100, 12, cyan);
        } else {
            snprintf(progress, sizeof(progress), "%.1f MB RECEIVED", startup->download.received / 1048576.0);
            rect(pixels, width, height, 48, 222, width - 96, 12, 0xff283c50);
            rect(pixels, width, height, 48 + (int)((ticks / 12) % (unsigned)(width - 136)), 222, 40, 12, cyan);
        }
        break;
    case PORTABLE_STARTUP_VALIDATE: status = "VALIDATING ROM..."; break;
    case PORTABLE_STARTUP_SAVE: status = "SAVING ROM CACHE..."; break;
    case PORTABLE_STARTUP_READY: status = "STARTING..."; break;
    case PORTABLE_STARTUP_ERROR: status = "ROM LOAD FAILED"; break;
    default: break;
    }
    text(pixels, width, height, 48, 166, status, 2, startup->phase == PORTABLE_STARTUP_ERROR ? 0xffff947e : cyan);
    text(pixels, width, height, 48, 198, progress, 1, white);
    if (startup->phase == PORTABLE_STARTUP_ERROR) {
        text(pixels, width, height, 48, 210, startup->message, 1, white);
        text(pixels, width, height, 48, 280, "PRESS R TO RETRY", 2, cyan);
    }
    text(pixels, width, height, 48, height - 60, "SELECT OR DROP CRUSNUSA.ZIP TO LOAD LOCALLY", 1, white);
}

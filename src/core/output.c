#include "output.h"
#include "machine.h"
#include "vunit.h"

static int crusn_min4(int a, int b, int c, int d) {
    int min = a;

    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }
    if (d < min) {
        min = d;
    }

    return min;
}

static int crusn_max4(int a, int b, int c, int d) {
    int max = a;

    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }
    if (d > max) {
        max = d;
    }

    return max;
}

static unsigned crusn_waveram_texel_at(size_t texel_index) {
    size_t word_index = texel_index >> 1;
    u32 word;

    if (word_index >= CRUSN_WAVERAM_WORDS) {
        return 0;
    }

    word = crusn_waveram[word_index];
    if ((texel_index & 1u) != 0u) {
        return (word >> 8) & 0xffu;
    }

    return word & 0xffu;
}

int port_output_fpga(
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    int x4,
    int y4,
    int uv0,
    int uv1,
    int uv2,
    int uv3,
    int texture_base,
    int palette_base,
    int control) {
    int dest_min_x;
    int dest_max_x;
    int dest_min_y;
    int dest_max_y;
    int src_left;
    int src_right;
    int src_top;
    int src_bottom;
    int dest_width;
    int dest_height;
    int src_width;
    int src_height;
    int textured;
    int zero_suppress;
    int nonzero_replace;
    int dither;

    int src_x;
    size_t texel_index;
    unsigned texel;

    u32* write_page_words;

    dest_min_x = crusn_min4(x1, x2, x3, x4);
    dest_max_x = crusn_max4(x1, x2, x3, x4);
    dest_min_y = crusn_min4(y1, y2, y3, y4);
    dest_max_y = crusn_max4(y1, y2, y3, y4);

    if (dest_min_x < 0) {
        dest_min_x = 0;
    }
    if (dest_min_y < 0) {
        dest_min_y = 0;
    }
    if (dest_max_x >= CRUSN_SCREEN_WIDTH) {
        dest_max_x = CRUSN_SCREEN_WIDTH - 1;
    }
    if (dest_max_y >= CRUSN_SCREEN_HEIGHT) {
        dest_max_y = CRUSN_SCREEN_HEIGHT - 1;
    }
    if (dest_min_x > dest_max_x || dest_min_y > dest_max_y) {
        return 0;
    }

    src_left = uv0 & 0xff;
    src_right = uv1 & 0xff;
    src_top = (uv0 >> 8) & 0xff;
    src_bottom = crusn_max4((uv1 >> 8) & 0xff, (uv2 >> 8) & 0xff, (uv3 >> 8) & 0xff, src_top);

    if (src_right < src_left) {
        int tmp = src_left;
        src_left = src_right;
        src_right = tmp;
    }
    if (src_bottom < src_top) {
        int tmp = src_top;
        src_top = src_bottom;
        src_bottom = tmp;
    }

    dest_width = dest_max_x - dest_min_x + 1;
    dest_height = dest_max_y - dest_min_y + 1;
    src_width = (src_right - src_left) + 1;
    src_height = src_bottom - src_top;
    if (src_height <= 0) {
        src_height = 1;
    }

    textured = (control & TM) != 0;
    zero_suppress = (control & ZS) != 0;
    nonzero_replace = (control & NZR) != 0;
    dither = (control & DITHER) != 0;
    write_page_words = crusn_machine_screen_page(g_crusn_machine, g_crusn_machine->write_page_index);

    for (int y = dest_min_y; y <= dest_max_y; ++y) {
        int src_y;

        if (dest_height > 1) {
            src_y = src_top + (((y - dest_min_y) * (src_height - 1)) / (dest_height - 1));
        } else {
            src_y = src_top;
        }

        for (int x = dest_min_x; x <= dest_max_x; ++x) {
            u32 color_index;
            u32 pixel;

            if (dither && ((x ^ y) & 1) != 0) {
                continue;
            }

            if (!textured) {
                color_index = (u32)(palette_base + (control & COLOR));
            } else {

                if (dest_width > 1) {
                    src_x = src_left + (((x - dest_min_x) * (src_width - 1)) / (dest_width - 1));
                } else {
                    src_x = src_left;
                }

                texel_index = ((size_t)texture_base * 256u) + ((size_t)src_y * 256u) + (size_t)src_x;
                texel = crusn_waveram_texel_at(texel_index);
                if (zero_suppress && texel == 0u) {
                    continue;
                }

                if (nonzero_replace && texel != 0u) {
                    color_index = (u32)(palette_base + (control & COLOR));
                } else {
                    color_index = (u32)(palette_base + (int)texel);
                }
            }
            pixel = color_index;
            write_page_words[(y * CRUSN_SCREEN_WIDTH) + x] = pixel;
        }
    }

    return 0;
}

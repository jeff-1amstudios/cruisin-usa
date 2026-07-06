#include "output.h"
#include "machine.h"
#include "vunit.h"

static int g_crusn_debug_output_quads;
static int g_crusn_debug_output_pixels;

typedef struct crusn_fpga_vertex {
    float x;
    float y;
    float u;
    float v;
} crusn_fpga_vertex;

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

static int crusn_clamp_int(int value, int min, int max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }

    return value;
}

static float crusn_min3f(float a, float b, float c) {
    float min = a;

    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }

    return min;
}

static float crusn_max3f(float a, float b, float c) {
    float max = a;

    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }

    return max;
}

static float crusn_edge_function(float ax, float ay, float bx, float by, float px, float py) {
    return ((px - ax) * (by - ay)) - ((py - ay) * (bx - ax));
}

static void crusn_rasterize_triangle(
    const crusn_fpga_vertex* v0,
    const crusn_fpga_vertex* v1,
    const crusn_fpga_vertex* v2,
    int textured,
    int zero_suppress,
    int nonzero_replace,
    int dither,
    int texture_base,
    int palette_base,
    int control,
    u32* write_page_words) {
    float area = crusn_edge_function(v0->x, v0->y, v1->x, v1->y, v2->x, v2->y);
    int min_x;
    int max_x;
    int min_y;
    int max_y;

    if (area == 0.0f) {
        return;
    }

    min_x = crusn_clamp_int((int)crusn_min3f(v0->x, v1->x, v2->x), 0, CRUSN_SCREEN_WIDTH - 1);
    max_x = crusn_clamp_int((int)crusn_max3f(v0->x, v1->x, v2->x), 0, CRUSN_SCREEN_WIDTH - 1);
    min_y = crusn_clamp_int((int)crusn_min3f(v0->y, v1->y, v2->y), 0, CRUSN_SCREEN_HEIGHT - 1);
    max_y = crusn_clamp_int((int)crusn_max3f(v0->y, v1->y, v2->y), 0, CRUSN_SCREEN_HEIGHT - 1);

    if (min_x > max_x || min_y > max_y) {
        return;
    }

    for (int y = min_y; y <= max_y; ++y) {
        float py = (float)y + 0.5f;

        for (int x = min_x; x <= max_x; ++x) {
            float px = (float)x + 0.5f;
            float w0 = crusn_edge_function(v1->x, v1->y, v2->x, v2->y, px, py);
            float w1 = crusn_edge_function(v2->x, v2->y, v0->x, v0->y, px, py);
            float w2 = crusn_edge_function(v0->x, v0->y, v1->x, v1->y, px, py);
            u32 color_index;
            u32 pixel;

            if ((area > 0.0f && (w0 < 0.0f || w1 < 0.0f || w2 < 0.0f))
                || (area < 0.0f && (w0 > 0.0f || w1 > 0.0f || w2 > 0.0f))) {
                continue;
            }

            if (dither && ((x ^ y) & 1) != 0) {
                continue;
            }

            if (!textured) {
                color_index = (u32)(palette_base + (control & COLOR));
            } else {
                float inv_area = 1.0f / area;
                float u = ((w0 * v0->u) + (w1 * v1->u) + (w2 * v2->u)) * inv_area;
                float v = ((w0 * v0->v) + (w1 * v1->v) + (w2 * v2->v)) * inv_area;
                int src_x = crusn_clamp_int((int)u, 0, 255);
                int src_y = crusn_clamp_int((int)v, 0, 255);
                size_t texel_index = ((size_t)texture_base * 256u) + ((size_t)src_y * 256u) + (size_t)src_x;
                unsigned texel = crusn_waveram_texel_at(texel_index);

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
            g_crusn_debug_output_pixels += 1;
        }
    }
}

void crusn_debug_output_reset_frame(void) {
    g_crusn_debug_output_quads = 0;
    g_crusn_debug_output_pixels = 0;
}

int crusn_debug_output_get_quad_count(void) {
    return g_crusn_debug_output_quads;
}

int crusn_debug_output_get_pixel_count(void) {
    return g_crusn_debug_output_pixels;
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
    int textured;
    int zero_suppress;
    int nonzero_replace;
    int dither;
    crusn_fpga_vertex vertices[4];
    u32* write_page_words;

    textured = (control & TM) != 0;
    zero_suppress = (control & ZS) != 0;
    nonzero_replace = (control & NZR) != 0;
    dither = (control & DITHER) != 0;
    write_page_words = crusn_machine_screen_page(g_crusn_machine, g_crusn_machine->write_page_index);

    vertices[0].x = (float)x1;
    vertices[0].y = (float)y1;
    vertices[0].u = (float)(uv0 & 0xff);
    vertices[0].v = (float)((uv0 >> 8) & 0xff);

    vertices[1].x = (float)x2;
    vertices[1].y = (float)y2;
    vertices[1].u = (float)(uv1 & 0xff);
    vertices[1].v = (float)((uv1 >> 8) & 0xff);

    vertices[2].x = (float)x3;
    vertices[2].y = (float)y3;
    vertices[2].u = (float)(uv2 & 0xff);
    vertices[2].v = (float)((uv2 >> 8) & 0xff);

    vertices[3].x = (float)x4;
    vertices[3].y = (float)y4;
    vertices[3].u = (float)(uv3 & 0xff);
    vertices[3].v = (float)((uv3 >> 8) & 0xff);

    g_crusn_debug_output_quads += 1;

    crusn_rasterize_triangle(
        &vertices[0],
        &vertices[1],
        &vertices[2],
        textured,
        zero_suppress,
        nonzero_replace,
        dither,
        texture_base,
        palette_base,
        control,
        write_page_words);
    crusn_rasterize_triangle(
        &vertices[0],
        &vertices[2],
        &vertices[3],
        textured,
        zero_suppress,
        nonzero_replace,
        dither,
        texture_base,
        palette_base,
        control,
        write_page_words);

    return 0;
}

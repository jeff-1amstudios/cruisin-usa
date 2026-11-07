#include "output.h"
#include "machine.h"
#include "vunit.h"

#include <math.h>

static int g_crusn_debug_output_quads;
static int g_crusn_debug_output_pixels;
static u32 g_crusn_comm_io;

int wireframe_mode = 0;

void port_output_comm_io(u32 value) {
    g_crusn_comm_io = value;
}

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

static void crusn_draw_wireframe_line(int x0, int y0, int x1, int y1, u32* write_page_words) {
    int dx = x1 > x0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 > y0 ? y0 - y1 : y1 - y0;
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    for (;;) {
        if (x0 >= 0 && x0 < CRUSN_SCREEN_WIDTH && y0 >= 0 && y0 < CRUSN_SCREEN_HEIGHT) {
            write_page_words[(y0 * CRUSN_SCREEN_WIDTH) + x0] = 0xffffffffu;
            g_crusn_debug_output_pixels += 1;
        }

        if (x0 == x1 && y0 == y1) {
            break;
        }

        if ((error * 2) >= dy) {
            error += dy;
            x0 += sx;
        }
        if ((error * 2) <= dx) {
            error += dx;
            y0 += sy;
        }
    }
}

typedef struct crusn_fpga_edge {
    const crusn_fpga_vertex* v1;
    const crusn_fpga_vertex* v2;
    float dxdy;
    float dudy;
    float dvdy;
} crusn_fpga_edge;

static int crusn_round_coordinate(float value) {
    float integer_part = floorf(value);
    float fractional_part = value - integer_part;

    return (int)integer_part + (fractional_part > 0.5f ? 1 : 0);
}

static void crusn_make_vertices_inclusive(crusn_fpga_vertex* vertices) {
    unsigned right_mask = 0;
    unsigned bottom_mask = 0;
    unsigned equal_mask = 0;

    for (int i = 0; i < 4; ++i) {
        const crusn_fpga_vertex* current = &vertices[i];
        const crusn_fpga_vertex* next = &vertices[(i + 1) & 3];

        if (next->x == current->x && next->y == current->y) {
            equal_mask |= 1u << i;
        }
        if (next->y > current->y || (next->y == current->y && next->x < current->x)) {
            right_mask |= 1u << i;
        }
        if (next->x < current->x || (next->x == current->x && next->y < current->y)) {
            bottom_mask |= 1u << i;
        }
    }

    if (equal_mask == 0x0fu) {
        return;
    }

    for (int i = 0; i < 4; ++i) {
        int effective_index = i;

        while ((equal_mask & (1u << effective_index)) != 0u) {
            effective_index = (effective_index + 1) & 3;
        }
        if ((right_mask & (1u << effective_index)) != 0u) {
            vertices[i].x += 0.001f;
        }
        if ((bottom_mask & (1u << effective_index)) != 0u) {
            vertices[i].y += 0.001f;
        }
    }
}

static int crusn_build_edge_list(
    const crusn_fpga_vertex* vertices,
    int min_vertex,
    int max_vertex,
    int direction,
    crusn_fpga_edge* edges) {
    int edge_count = 0;

    for (int current = min_vertex; current != max_vertex; current = (current + direction + 4) & 3) {
        int next = (current + direction + 4) & 3;
        float inverse_dy;

        if (vertices[current].y == vertices[next].y) {
            continue;
        }

        inverse_dy = 1.0f / (vertices[next].y - vertices[current].y);
        edges[edge_count].v1 = &vertices[current];
        edges[edge_count].v2 = &vertices[next];
        edges[edge_count].dxdy = (vertices[next].x - vertices[current].x) * inverse_dy;
        edges[edge_count].dudy = (vertices[next].u - vertices[current].u) * inverse_dy;
        edges[edge_count].dvdy = (vertices[next].v - vertices[current].v) * inverse_dy;
        edge_count += 1;
    }

    return edge_count;
}

static void crusn_rasterize_quad(
    crusn_fpga_vertex* vertices,
    int textured,
    int zero_suppress,
    int nonzero_replace,
    int dither,
    int texture_base,
    int palette_base,
    int control,
    u32* write_page_words) {
    crusn_fpga_edge forward_edges[3];
    crusn_fpga_edge backward_edges[3];
    const crusn_fpga_edge* left_edge;
    const crusn_fpga_edge* right_edge;
    int forward_edge_count;
    int backward_edge_count;
    int left_edge_count;
    int right_edge_count;
    int left_edge_index = 0;
    int right_edge_index = 0;
    int min_vertex = 0;
    int max_vertex = 0;
    int min_y;
    int max_y;

    for (int i = 1; i < 4; ++i) {
        if (vertices[i].y < vertices[min_vertex].y) {
            min_vertex = i;
        } else if (vertices[i].y > vertices[max_vertex].y) {
            max_vertex = i;
        }
    }

    min_y = crusn_clamp_int(crusn_round_coordinate(vertices[min_vertex].y), 0, CRUSN_SCREEN_HEIGHT);
    max_y = crusn_clamp_int(crusn_round_coordinate(vertices[max_vertex].y), 0, CRUSN_SCREEN_HEIGHT);
    if (min_y >= max_y) {
        return;
    }

    forward_edge_count = crusn_build_edge_list(vertices, min_vertex, max_vertex, 1, forward_edges);
    backward_edge_count = crusn_build_edge_list(vertices, min_vertex, max_vertex, -1, backward_edges);
    if (forward_edge_count == 0 || backward_edge_count == 0) {
        return;
    }

    if ((forward_edges[0].v1 == backward_edges[0].v1 && forward_edges[0].dxdy < backward_edges[0].dxdy)
        || (forward_edges[0].v1 != backward_edges[0].v1 && forward_edges[0].v1->x < backward_edges[0].v1->x)) {
        left_edge = forward_edges;
        left_edge_count = forward_edge_count;
        right_edge = backward_edges;
        right_edge_count = backward_edge_count;
    } else {
        left_edge = backward_edges;
        left_edge_count = backward_edge_count;
        right_edge = forward_edges;
        right_edge_count = forward_edge_count;
    }

    /* V-unit texture parameters are interpolated across each quad span, not across two triangles. */
    for (int y = min_y; y < max_y; ++y) {
        float sample_y = (float)y + 0.5f;
        float start_x;
        float stop_x;
        int start_x_int;
        int stop_x_int;
        float left_dy;
        float right_dy;
        float inverse_width;
        float left_u;
        float right_u;
        float left_v;
        float right_v;
        float du_dx_float;
        float dv_dx_float;
        int u;
        int v;
        int du_dx;
        int dv_dx;

        while (left_edge_index + 1 < left_edge_count
               && sample_y > left_edge[left_edge_index].v2->y
               && sample_y < vertices[max_vertex].y) {
            left_edge_index += 1;
        }
        while (right_edge_index + 1 < right_edge_count
               && sample_y > right_edge[right_edge_index].v2->y
               && sample_y < vertices[max_vertex].y) {
            right_edge_index += 1;
        }

        start_x = left_edge[left_edge_index].v1->x
            + (sample_y - left_edge[left_edge_index].v1->y) * left_edge[left_edge_index].dxdy;
        stop_x = right_edge[right_edge_index].v1->x
            + (sample_y - right_edge[right_edge_index].v1->y) * right_edge[right_edge_index].dxdy;
        start_x_int = crusn_round_coordinate(start_x);
        stop_x_int = crusn_round_coordinate(stop_x);
        if (start_x_int > stop_x_int) {
            int temporary = start_x_int;
            start_x_int = stop_x_int;
            stop_x_int = temporary;
        }
        if (start_x_int >= stop_x_int || stop_x == start_x) {
            continue;
        }

        left_dy = sample_y - left_edge[left_edge_index].v1->y;
        right_dy = sample_y - right_edge[right_edge_index].v1->y;
        inverse_width = 1.0f / (stop_x - start_x);
        left_u = left_edge[left_edge_index].v1->u + left_dy * left_edge[left_edge_index].dudy;
        right_u = right_edge[right_edge_index].v1->u + right_dy * right_edge[right_edge_index].dudy;
        left_v = left_edge[left_edge_index].v1->v + left_dy * left_edge[left_edge_index].dvdy;
        right_v = right_edge[right_edge_index].v1->v + right_dy * right_edge[right_edge_index].dvdy;
        du_dx_float = (right_u - left_u) * inverse_width;
        dv_dx_float = (right_v - left_v) * inverse_width;
        u = (int)(left_u + ((float)start_x_int + 0.5f - start_x) * du_dx_float);
        v = (int)(left_v + ((float)start_x_int + 0.5f - start_x) * dv_dx_float);
        du_dx = (int)du_dx_float;
        dv_dx = (int)dv_dx_float;

        if (start_x_int < 0) {
            u += -start_x_int * du_dx;
            v += -start_x_int * dv_dx;
            start_x_int = 0;
        }
        stop_x_int = crusn_clamp_int(stop_x_int, 0, CRUSN_SCREEN_WIDTH);

        for (int x = start_x_int; x < stop_x_int; ++x) {
            u32 color_index;
            u32 pixel;

            if (dither && ((x ^ y) & 1) != 0) {
                u += du_dx;
                v += dv_dx;
                continue;
            }

            if (!textured) {
                color_index = (u32)(palette_base + (control & COLOR));
            } else {
                int src_x = u >> 16;
                int src_y = (v >> 16) & 0xff;
                size_t texel_index = ((size_t)texture_base * 256u) + ((size_t)src_y * 256u) + (size_t)src_x;
                unsigned texel = crusn_waveram_texel_at(texel_index);

                if (zero_suppress && texel == 0u) {
                    u += du_dx;
                    v += dv_dx;
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
            u += du_dx;
            v += dv_dx;
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

    vertices[0].x = (float)x1 + 0.5f;
    vertices[0].y = (float)y1 + 0.5f;
    vertices[0].u = (float)(uv0 & 0xff) * 65536.0f + 32768.0f;
    vertices[0].v = (float)((uv0 >> 8) & 0xff) * 65536.0f + 32768.0f;

    vertices[1].x = (float)x2 + 0.5f;
    vertices[1].y = (float)y2 + 0.5f;
    vertices[1].u = (float)(uv1 & 0xff) * 65536.0f + 32768.0f;
    vertices[1].v = (float)((uv1 >> 8) & 0xff) * 65536.0f + 32768.0f;

    vertices[2].x = (float)x3 + 0.5f;
    vertices[2].y = (float)y3 + 0.5f;
    vertices[2].u = (float)(uv2 & 0xff) * 65536.0f + 32768.0f;
    vertices[2].v = (float)((uv2 >> 8) & 0xff) * 65536.0f + 32768.0f;

    vertices[3].x = (float)x4 + 0.5f;
    vertices[3].y = (float)y4 + 0.5f;
    vertices[3].u = (float)(uv3 & 0xff) * 65536.0f + 32768.0f;
    vertices[3].v = (float)((uv3 >> 8) & 0xff) * 65536.0f + 32768.0f;

    crusn_make_vertices_inclusive(vertices);

    g_crusn_debug_output_quads += 1;

    if (wireframe_mode) {
        crusn_draw_wireframe_line(x1, y1, x2, y2, write_page_words);
        crusn_draw_wireframe_line(x2, y2, x3, y3, write_page_words);
        crusn_draw_wireframe_line(x3, y3, x4, y4, write_page_words);
        crusn_draw_wireframe_line(x4, y4, x1, y1, write_page_words);
        return 0;
    }

    crusn_rasterize_quad(
        vertices,
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

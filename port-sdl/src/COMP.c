#include "COMP.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "TRACE.h"

#include "NOT_IMPLEMENTED.h"
#define LOAD_QUEUE_CAPACITY 32

#define BITS_MIN 9
#define BITS_MAX 12
#define END_OF_STREAM 256
#define BUMP_CODE 257
#define FLUSH_CODE 258
#define FIRST_CODE 259
#define MAX_CODE ((1 << BITS_MAX) - 1)

typedef struct DictEntry {
    int parent;
    uint8_t ch;
} DictEntry;

typedef struct BitReader {
    const uint8_t *data;
    size_t len;
    size_t bit_pos;
} BitReader;

typedef struct LoadQueue {
    const SectionControl *items[LOAD_QUEUE_CAPACITY];
    int head;
    int tail;
    int count;
} LoadQueue;

static bool g_decomp_active;
static LoadQueue g_queue;

static bool queue_push(const SectionControl *section) {
    TRACE();
    if (g_queue.count >= LOAD_QUEUE_CAPACITY) {
        return false;
    }
    g_queue.items[g_queue.tail] = section;
    g_queue.tail = (g_queue.tail + 1) % LOAD_QUEUE_CAPACITY;
    g_queue.count++;
    return true;
}

static const SectionControl *queue_pop(void) {
    if (g_queue.count <= 0) {
        return NULL;
    }
    const SectionControl *section = g_queue.items[g_queue.head];
    g_queue.head = (g_queue.head + 1) % LOAD_QUEUE_CAPACITY;
    g_queue.count--;
    return section;
}

static int bitreader_read(BitReader *br, int nbits) {
    TRACE();
    if (!br || nbits <= 0 || nbits > 16) {
        return -1;
    }
    if ((br->bit_pos + (size_t)nbits) > (br->len * 8u)) {
        return -1;
    }

    int value = 0;
    for (int i = 0; i < nbits; i++) {
        const size_t bit_index = br->bit_pos + (size_t)i;
        const size_t byte_index = bit_index >> 3;
        const int bit_in_byte = 7 - (int)(bit_index & 7u);
        const int bit = (br->data[byte_index] >> bit_in_byte) & 1;
        value = (value << 1) | bit;
    }

    br->bit_pos += (size_t)nbits;
    return value;
}

static bool decode_symbol(
    int code,
    const DictEntry *dict,
    uint8_t *stack,
    size_t stack_cap,
    size_t *stack_count,
    uint8_t *first_ch
) {
    if (!dict || !stack || !stack_count || !first_ch || code < 0) {
        return false;
    }

    size_t n = 0;
    int c = code;

    while (c > 255) {
        if (c > MAX_CODE || n >= stack_cap) {
            return false;
        }
        stack[n++] = dict[c].ch;
        c = dict[c].parent;
        if (c < 0) {
            return false;
        }
    }

    if (n >= stack_cap) {
        return false;
    }

    stack[n++] = (uint8_t)c;
    *first_ch = (uint8_t)c;
    *stack_count = n;
    return true;
}

static size_t DECOMPRESS_STREAM(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_len) {
    TRACE();
    if (!src || !dst || src_len == 0 || dst_len == 0) {
        return 0;
    }

    DictEntry dict[MAX_CODE + 1];
    uint8_t decode_stack[MAX_CODE + 1];

    BitReader br = {src, src_len, 0};

    int code_bits = BITS_MIN;
    int next_code = FIRST_CODE;
    int old_code = bitreader_read(&br, code_bits);

    if (old_code < 0 || old_code == END_OF_STREAM) {
        return 0;
    }

    size_t out = 0;
    if (old_code >= 0 && old_code <= 255) {
        dst[out++] = (uint8_t)old_code;
    } else {
        return 0;
    }

    while (out < dst_len) {
        int new_code = bitreader_read(&br, code_bits);
        if (new_code < 0) {
            break;
        }

        if (new_code == END_OF_STREAM) {
            break;
        }

        if (new_code == FLUSH_CODE) {
            code_bits = BITS_MIN;
            next_code = FIRST_CODE;
            old_code = bitreader_read(&br, code_bits);
            if (old_code < 0 || old_code == END_OF_STREAM) {
                break;
            }
            if (old_code < 0 || old_code > 255) {
                break;
            }
            dst[out++] = (uint8_t)old_code;
            continue;
        }

        if (new_code == BUMP_CODE) {
            if (code_bits < BITS_MAX) {
                code_bits++;
            }
            continue;
        }

        size_t n = 0;
        uint8_t first_ch = 0;

        if (new_code >= next_code) {
            size_t old_n = 0;
            uint8_t old_first = 0;
            if (!decode_symbol(old_code, dict, decode_stack, sizeof(decode_stack), &old_n, &old_first)) {
                break;
            }
            if (old_n >= sizeof(decode_stack)) {
                break;
            }
            decode_stack[old_n++] = old_first;
            n = old_n;
            first_ch = old_first;
        } else {
            if (!decode_symbol(new_code, dict, decode_stack, sizeof(decode_stack), &n, &first_ch)) {
                break;
            }
        }

        while (n > 0 && out < dst_len) {
            dst[out++] = decode_stack[--n];
        }

        if (next_code <= MAX_CODE) {
            dict[next_code].parent = old_code;
            dict[next_code].ch = first_ch;
            next_code++;
        }

        old_code = new_code;
    }

    return out;
}

static void DECOMPRESS(const SectionControl *section) {
    TRACE();
    g_decomp_active = true;

    size_t out = 0;
    if (section && section->dst && section->src && section->src_len > 0 && section->dst_len > 0) {
        out = DECOMPRESS_STREAM(section->src, section->src_len, section->dst, section->dst_len);
    }

    if (section && section->name) {
        fprintf(stderr, "[COMP] loaded section %s (%zu bytes)\n", section->name, out);
    }

    g_decomp_active = false;
}

void DECOMPRESS_PROC(void) {
    TRACE();
    if (g_decomp_active) {
        return;
    }
    const SectionControl *next = queue_pop();
    if (next) {
        DECOMPRESS(next);
    }
}

void LOAD_SECTION_REQ(const SectionControl *section) {
    TRACE();
    if (!section) {
        return;
    }

    if (g_decomp_active) {
        if (!queue_push(section)) {
            fprintf(stderr, "[COMP] load queue full, dropping %s\n", section->name ? section->name : "<unnamed>");
        }
        return;
    }

    DECOMPRESS(section);
    DECOMPRESS_PROC();
}

void INPUT_BITS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PUTC(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SAVE_DECOMP_REGS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RESTORE_DECOMP_REGS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void BOOT_PACIFY_SCREEN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void REQWAIT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

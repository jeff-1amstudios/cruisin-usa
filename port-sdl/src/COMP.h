#ifndef COMP_H
#define COMP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct SectionControl {
    const char *name;
    const uint8_t *src;
    uint8_t *dst;
    size_t src_len;
    size_t dst_len;
} SectionControl;

void DECOMPRESS_PROC(void);
void LOAD_SECTION_REQ(const SectionControl *section);

#endif

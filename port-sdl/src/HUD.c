#include "NOT_IMPLEMENTED.h"
#include "TRACE.h"
#include "STUB.h"
#include "HUD.h"

/* Scaffold generated from HUD.ASM */
static const unsigned int *g_section_header_words;
void PAL_ALLOC(void);
void PAL_DELETE(void);

void HUD_SET_SECTION_HEADER(const unsigned int *header_words) {
    TRACE();
    g_section_header_words = header_words;
}

void MOVEIN_HUD_EQUIP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void MOVEOUT_HUD_EQUIP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SECTION_PALETTE_ALLOC(void) {
    TRACE();
    alloc_section();
}

void alloc_section(void) {
    TRACE();
    if (g_section_header_words == 0) {
        return;
    }
    {
        unsigned int pal_index = g_section_header_words[0];
        unsigned int count = g_section_header_words[1];
        if (count == 0) {
            return;
        }
        count -= 1;
        do {
            (void)pal_index;
            PAL_ALLOC();
            pal_index += 1;
        } while (count-- != 0);
    }
}

void HARDalloc_section(void) {
    TRACE();
    alloc_section();
}

void dealloc_section(void) {
    TRACE();
    if (g_section_header_words == 0) {
        return;
    }
    {
        unsigned int pal_index = g_section_header_words[0];
        unsigned int count = g_section_header_words[1];
        if (count == 0) {
            return;
        }
        count -= 1;
        do {
            (void)pal_index;
            PAL_DELETE();
            pal_index += 1;
        } while (count-- != 0);
    }
}

void TACHOMETER_ANIMATE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void FILL_DITHER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void FILL_PLOT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HUD(void) {
    TRACE();
    STUB();
}

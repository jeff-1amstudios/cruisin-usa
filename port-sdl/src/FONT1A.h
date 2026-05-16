#ifndef FONT1A_H
#define FONT1A_H

#include <stdint.h>

void FONT1A_set_palette(const uint32_t *palette_rgba, int palette_len);
void FONT1A_set_framebuffer(uint32_t *framebuffer, int width, int height);
void _pixel(int x, int y, int color_index);
void _outtextxyc(const char *text, int x, int y, int color_index);

#endif

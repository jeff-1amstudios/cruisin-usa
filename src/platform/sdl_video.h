#ifndef CRUSN_SDL_VIDEO_H
#define CRUSN_SDL_VIDEO_H

#include <SDL.h>

#include "../portable/machine.h"

typedef struct crusn_video {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int bilinear;
} crusn_video;

int crusn_video_init(crusn_video* video, int fullscreen, int bilinear);
int crusn_video_present_pixels(crusn_video* video, const u32* pixels);
void crusn_video_shutdown(crusn_video* video);
int crusn_video_present(crusn_video* video, const crusn_machine* machine);

#endif

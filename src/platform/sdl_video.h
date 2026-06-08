#ifndef CRUSN_SDL_VIDEO_H
#define CRUSN_SDL_VIDEO_H

#include <SDL.h>

#include "../core/machine.h"

typedef struct crusn_video {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} crusn_video;

int crusn_video_init(crusn_video *video);
void crusn_video_shutdown(crusn_video *video);
int crusn_video_present(crusn_video *video, const crusn_machine *machine);

#endif

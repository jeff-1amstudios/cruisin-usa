#include "sdl_video.h"

#include <stdlib.h>

int crusn_video_init(crusn_video* video) {
    video->window = SDL_CreateWindow(
        "Cruis'n USA Port",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        CRUSN_SCREEN_WIDTH * 2,
        CRUSN_SCREEN_HEIGHT * 2,
        SDL_WINDOW_RESIZABLE);
    if (video->window == NULL) {
        return -1;
    }

    video->renderer = SDL_CreateRenderer(video->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (video->renderer == NULL) {
        crusn_video_shutdown(video);
        return -1;
    }

    video->texture = SDL_CreateTexture(
        video->renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        CRUSN_SCREEN_WIDTH,
        CRUSN_SCREEN_HEIGHT);
    if (video->texture == NULL) {
        crusn_video_shutdown(video);
        return -1;
    }

    return 0;
}

void crusn_video_shutdown(crusn_video* video) {
    if (video->texture != NULL) {
        SDL_DestroyTexture(video->texture);
        video->texture = NULL;
    }
    if (video->renderer != NULL) {
        SDL_DestroyRenderer(video->renderer);
        video->renderer = NULL;
    }
    if (video->window != NULL) {
        SDL_DestroyWindow(video->window);
        video->window = NULL;
    }
}

int crusn_video_present(crusn_video* video, const crusn_machine* machine) {
    u32* pixels;
    int pitch;

    if (SDL_LockTexture(video->texture, NULL, (void**)&pixels, &pitch) != 0) {
        return -1;
    }

    crusn_machine_decode_screen_argb8888(machine, pixels, (size_t)pitch);
    SDL_UnlockTexture(video->texture);

    if (SDL_RenderClear(video->renderer) != 0) {
        return -1;
    }

    if (SDL_RenderCopy(video->renderer, video->texture, NULL, NULL) != 0) {
        return -1;
    }

    SDL_RenderPresent(video->renderer);
    return 0;
}

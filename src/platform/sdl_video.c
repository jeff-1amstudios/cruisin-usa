#include "sdl_video.h"

#include <stdlib.h>

int crusn_video_init(crusn_video* video, int fullscreen, int bilinear) {
    Uint32 window_flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;
    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    const char* validation_realtime = getenv("CRUSN_VALIDATE_REALTIME");

    if (getenv("CRUSN_ENABLE_MAME_VALIDATION") != NULL &&
        validation_realtime != NULL && validation_realtime[0] == '0') {
        renderer_flags = SDL_RENDERER_SOFTWARE;
    }

    video->window = SDL_CreateWindow(
        "Cruis'n USA 4.5",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        window_flags);
    if (video->window == NULL) {
        return -1;
    }

    video->renderer = SDL_CreateRenderer(video->window, -1, renderer_flags);
    if (video->renderer == NULL) {
        crusn_video_shutdown(video);
        return -1;
    }

    if (SDL_RenderSetLogicalSize(video->renderer, CRUSN_SCREEN_WIDTH, CRUSN_SCREEN_HEIGHT) != 0) {
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

    if (SDL_SetTextureScaleMode(
            video->texture,
            bilinear ? SDL_ScaleModeLinear : SDL_ScaleModeNearest) != 0) {
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

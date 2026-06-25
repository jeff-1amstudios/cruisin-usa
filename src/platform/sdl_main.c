#include <SDL.h>
#include <stdio.h>

#include "../core/machine.h"
#include "../game/globals.h"
#include "sdl_video.h"

static crusn_machine* g_display_machine;
static crusn_video* g_display_video;
static int* g_display_running;

void crusn_yield_display_interrupt(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *g_display_running = 0;
        }
    }

    if (crusn_video_present(g_display_video, g_display_machine) != 0) {
        fprintf(stderr, "Failed to present frame: %s\n", SDL_GetError());
        *g_display_running = 0;
    }

    SDL_Delay(500);
}

int main(void) {
    crusn_machine machine;
    crusn_video video = { 0 };
    int running = 1;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (crusn_machine_init(&machine) != 0) {
        fprintf(stderr, "Failed to initialize machine\n");
        SDL_Quit();
        return 1;
    }

    if (crusn_video_init(&video) != 0) {
        fprintf(stderr, "Failed to initialize video: %s\n", SDL_GetError());
        crusn_machine_shutdown(&machine);
        SDL_Quit();
        return 1;
    }

    g_display_machine = &machine;
    g_display_video = &video;
    g_display_running = &running;

    _c_int00();

    while (running) {
        INT0();
        crusn_yield_display_interrupt();
    }

    crusn_video_shutdown(&video);
    crusn_machine_shutdown(&machine);
    SDL_Quit();
    return 0;
}

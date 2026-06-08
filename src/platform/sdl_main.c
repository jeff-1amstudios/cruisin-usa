#include <SDL.h>
#include <stdio.h>

#include "../core/machine.h"
#include "sdl_video.h"

int main(void)
{
    crusn_machine machine;
    crusn_video video = {0};
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

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        crusn_machine_tick(&machine);
        if (crusn_video_present(&video, &machine) != 0) {
            fprintf(stderr, "Failed to present frame: %s\n", SDL_GetError());
            break;
        }
    }

    crusn_video_shutdown(&video);
    crusn_machine_shutdown(&machine);
    SDL_Quit();
    return 0;
}

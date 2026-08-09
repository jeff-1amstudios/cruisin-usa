#include <SDL.h>
#include <stdio.h>

#include "../core/machine.h"
#include "../core/validator.h"
#include "../game/globals.h"
#include "sdl_video.h"

static crusn_machine* g_display_machine;
static crusn_video* g_display_video;
static int* g_display_running;
static Uint64 g_attract_start_counter;
static int g_measure_attract_timing;

extern void MAINLOOP(void);

void crusn_measure_attract_start(void) {
    if (getenv("CRUSN_MEASURE_ATTRACT_TIMING") == NULL) {
        return;
    }

    g_measure_attract_timing = 1;
    g_attract_start_counter = SDL_GetPerformanceCounter();
    fprintf(stderr, "ATTRACT_DELTA timing start\n");
}

int crusn_measure_attract_end(void) {
    if (!g_measure_attract_timing) {
        return 0;
    }

    Uint64 elapsed_counter = SDL_GetPerformanceCounter() - g_attract_start_counter;
    double elapsed_ms = (double)elapsed_counter * 1000.0 / (double)SDL_GetPerformanceFrequency();
    fprintf(stderr, "LOADBEVERLY after %.3f ms\n", elapsed_ms);
    *g_display_running = 0;
    return 1;
}

static void crusn_pump_events(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *g_display_running = 0;
        }
    }
}

void crusn_yield_display_interrupt(void) {
    crusn_pump_events();

    if (crusn_video_present(g_display_video, g_display_machine) != 0) {
        fprintf(stderr, "Failed to present frame: %s\n", SDL_GetError());
        *g_display_running = 0;
    }

    // SDL_Delay(100);
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

    mame_validate_fail_on_wrong_consumer(1);

    _c_int00();

    const double int0_period = 1.0 / 60.0;
    const double counter_frequency = (double)SDL_GetPerformanceFrequency();
    Uint64 previous_counter = SDL_GetPerformanceCounter();
    double int0_accumulator = int0_period;

    while (running) {
        Uint64 current_counter = SDL_GetPerformanceCounter();
        int0_accumulator += (double)(current_counter - previous_counter) / counter_frequency;
        previous_counter = current_counter;

        while (int0_accumulator >= int0_period) {
            INT0();
            int0_accumulator -= int0_period;
        }

        if (INFRAMES >= FRAMRATE) {
            MAINLOOP();
            crusn_yield_display_interrupt();
        } else {
            crusn_pump_events();
            SDL_Delay(1);
        }
    }

    crusn_video_shutdown(&video);
    crusn_machine_shutdown(&machine);
    SDL_Quit();
    return 0;
}

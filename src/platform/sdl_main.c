#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/audio.h"
#include "../core/input.h"
#include "../core/machine.h"
#include "../core/validator.h"
#include "../game/cmos.h"
#include "../game/cusa.h"
#include "../game/globals.h"
#include "sdl_video.h"

static crusn_machine* g_display_machine;
static crusn_video* g_display_video;
static int* g_display_running;
static Uint64 g_attract_start_counter;
static int g_measure_attract_timing;

extern void MAINLOOP(void);

static int crusn_free_play_enabled(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--no-free-play") == 0) {
            return 0;
        }
    }

    return 1;
}

static int crusn_sound_enabled(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--no-sound") == 0) {
            return 0;
        }
    }

    return 1;
}

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

    port_handle_input();
}

void crusn_yield_display_interrupt(void) {
    crusn_pump_events();

    if (crusn_video_present(g_display_video, g_display_machine) != 0) {
        fprintf(stderr, "Failed to present frame: %s\n", SDL_GetError());
        *g_display_running = 0;
    }

    // SDL_Delay(100);
}

int main(int argc, char* argv[]) {
    crusn_machine machine;
    crusn_video video = { 0 };
    int running = 1;
    int free_play = crusn_free_play_enabled(argc, argv);
    int sound = crusn_sound_enabled(argc, argv);
    Uint32 sdl_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;

    if (sound) {
        sdl_flags |= SDL_INIT_AUDIO;
    }

    if (SDL_Init(sdl_flags) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (crusn_machine_init(&machine) != 0) {
        fprintf(stderr, "Failed to initialize machine\n");
        SDL_Quit();
        return 1;
    }

    if (sound && portable_audio_init("roms/crusnusa.zip") != 0) {
        fprintf(stderr, "Failed to initialize audio\n");
        crusn_machine_shutdown(&machine);
        SDL_Quit();
        return 1;
    }

    if (crusn_video_init(&video) != 0) {
        fprintf(stderr, "Failed to initialize video: %s\n", SDL_GetError());
        if (sound) {
            portable_audio_shutdown();
        }
        crusn_machine_shutdown(&machine);
        SDL_Quit();
        return 1;
    }

    g_display_machine = &machine;
    g_display_video = &video;
    g_display_running = &running;

    mame_validate_fail_on_wrong_consumer(1);

    _c_int00();

    if (free_play) {
        ADJUSTMENT_WRITE(ADJ_FREE_PLAY, 1);
    }
    ADJUSTMENT_WRITE(ADJ_GIRLS, 0);

    const Uint64 counter_frequency = SDL_GetPerformanceFrequency();
    Uint64 previous_counter = SDL_GetPerformanceCounter();
    Uint64 int0_accumulator = counter_frequency;

    while (running) {
        Uint64 current_counter = SDL_GetPerformanceCounter();
        int0_accumulator += (current_counter - previous_counter) * TIKS_PER_SECOND;
        previous_counter = current_counter;

        while (int0_accumulator >= counter_frequency) {
            INT0();
            int0_accumulator -= counter_frequency;
        }

        /* ZSORTWT raises CLEARRDY and the original MAINLOOP waits for INT0
           to clear it before dispatching processes for another frame. */
        if (INFRAMES >= FRAMRATE + 1 && CLEARRDY == 0) {
            MAINLOOP();
            crusn_yield_display_interrupt();
        } else {
            crusn_pump_events();
            SDL_Delay(1);
        }
    }

    crusn_video_shutdown(&video);
    if (sound) {
        portable_audio_shutdown();
    }
    crusn_machine_shutdown(&machine);
    SDL_Quit();
    return 0;
}

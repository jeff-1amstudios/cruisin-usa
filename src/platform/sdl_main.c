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
#include "../game/hstdp.h"
#include "sdl_video.h"

static crusn_machine* g_display_machine;
static crusn_video* g_display_video;
static int* g_display_running;
static Uint64 g_attract_start_counter;
static int g_measure_attract_timing;
static int g_skip_boot_screen;

extern void MAINLOOP(void);

typedef struct crusn_options {
    int free_play;
    int sound;
    int girls;
    int race_time;
    int windowed;
    int bilinear;
    int test_highscore_input;
    int skip_boot_screen;
} crusn_options;

static int process_args(int argc, char* argv[], crusn_options* options) {
    const char prefix[] = "--race-time=";

    options->free_play = 1;
    options->sound = 1;
    options->girls = -1;
    options->race_time = 0;
    options->windowed = 0;
    options->bilinear = 1;
    options->test_highscore_input = 0;
    options->skip_boot_screen = 1;

    for (int i = 1; i < argc; ++i) {
        const char* value = NULL;

        if (strcmp(argv[i], "--no-free-play") == 0) {
            options->free_play = 0;
        } else if (strcmp(argv[i], "--no-sound") == 0) {
            options->sound = 0;
        } else if (strcmp(argv[i], "--girls") == 0) {
            options->girls = 1;
        } else if (strcmp(argv[i], "--no-girls") == 0) {
            options->girls = 0;
        } else if (strcmp(argv[i], "--window") == 0) {
            options->windowed = 1;
        } else if (strcmp(argv[i], "--no-bilinear") == 0) {
            options->bilinear = 0;
        } else if (strcmp(argv[i], "--test-highscore-input") == 0) {
            options->test_highscore_input = 1;
        } else if (strcmp(argv[i], "--no-skip-boot-screen") == 0) {
            options->skip_boot_screen = 0;
        } else if (strncmp(argv[i], prefix, sizeof(prefix) - 1) == 0) {
            value = argv[i] + sizeof(prefix) - 1;
        } else if (strcmp(argv[i], "--race-time") == 0 && i + 1 < argc) {
            value = argv[++i];
        } else {
            fprintf(stderr, "Unknown argument '%s'\n", argv[i]);
            return -1;
        }

        if (value != NULL) {
            char* end = NULL;
            long seconds = strtol(value, &end, 10);
            if (*value == '\0' || *end != '\0' || seconds <= 0 ||
                seconds > 3600 || seconds % 5 != 0) {
                fprintf(stderr, "Invalid --race-time value '%s' (expected 5-3600 seconds in 5-second steps)\n", value);
                return -1;
            }
            options->race_time = (int)seconds;
        }
    }

    return 0;
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

static void crusn_pace_validation_frame(int frame_ticks) {
    static Uint64 next_frame_counter;
    const char* realtime = getenv("CRUSN_VALIDATE_REALTIME");
    const Uint64 counter_frequency = SDL_GetPerformanceFrequency();
    const Uint64 frame_duration =
        (Uint64)frame_ticks * counter_frequency / TIKS_PER_SECOND;
    Uint64 current_counter;

    if (realtime == NULL || realtime[0] != '1' || frame_ticks <= 0) {
        return;
    }

    current_counter = SDL_GetPerformanceCounter();
    if (next_frame_counter == 0 || current_counter > next_frame_counter + counter_frequency) {
        next_frame_counter = current_counter;
    }
    next_frame_counter += frame_duration;

    while (current_counter < next_frame_counter) {
        Uint64 remaining_ms =
            (next_frame_counter - current_counter) * 1000 / counter_frequency;
        if (remaining_ms > 1) {
            SDL_Delay((Uint32)(remaining_ms - 1));
        }
        current_counter = SDL_GetPerformanceCounter();
    }
}

void crusn_yield_display_interrupt(void) {
    static Uint64 fps_interval_start;
    static unsigned int fps_frame_count;

    crusn_pump_events();

    if (crusn_video_present(g_display_video, g_display_machine) != 0) {
        fprintf(stderr, "Failed to present frame: %s\n", SDL_GetError());
        *g_display_running = 0;
    }

    Uint64 current_counter = SDL_GetPerformanceCounter();
    if (fps_interval_start == 0) {
        fps_interval_start = current_counter;
    }
    fps_frame_count++;

    Uint64 elapsed_counter = current_counter - fps_interval_start;
    Uint64 counter_frequency = SDL_GetPerformanceFrequency();
    if (elapsed_counter >= counter_frequency) {
        double fps = (double)fps_frame_count * (double)counter_frequency / (double)elapsed_counter;
        printf("FPS: %.1f\n", fps);
        fflush(stdout);
        fps_interval_start = current_counter;
        fps_frame_count = 0;
    }

    // SDL_Delay(100);
}

int crusn_should_skip_boot_screen(void) {
    return g_skip_boot_screen;
}

int main(int argc, char* argv[]) {
    crusn_machine machine;
    crusn_video video = { 0 };
    crusn_options options;
    int running = 1;
    Uint32 sdl_flags;

    if (process_args(argc, argv, &options) != 0) {
        return 1;
    }
    g_skip_boot_screen = options.skip_boot_screen;
    sdl_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    if (options.sound) {
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

    if (options.sound && portable_audio_init("roms/crusnusa.zip") != 0) {
        fprintf(stderr, "Failed to initialize audio\n");
        crusn_machine_shutdown(&machine);
        SDL_Quit();
        return 1;
    }

    if (crusn_video_init(&video, !options.windowed, options.bilinear) != 0) {
        fprintf(stderr, "Failed to initialize video: %s\n", SDL_GetError());
        if (options.sound) {
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

    if (options.race_time > 0) {
        ADJUSTMENT_WRITE(ADJ_TIME_TO_START, (options.race_time - 60) / 5);
    }
    if (options.free_play) {
        ADJUSTMENT_WRITE(ADJ_FREE_PLAY, 1);
    }
    if (options.girls >= 0) {
        ADJUSTMENT_WRITE(ADJ_GIRLS, options.girls);
    }
    if (options.test_highscore_input) {
        START_HIGH_SCORE_INPUT_TEST();
    }

    const Uint64 counter_frequency = SDL_GetPerformanceFrequency();
    Uint64 previous_counter = SDL_GetPerformanceCounter();
    Uint64 int0_accumulator = counter_frequency;
    const int deterministic_validation = getenv("CRUSN_ENABLE_MAME_VALIDATION") != NULL;

    while (running) {
        if (deterministic_validation) {
            if (mame_validation_replay_started()) {
                int frame_ticks = mame_validate_frame_ticks();
                while (INFRAMES < frame_ticks) {
                    INT0();
                }
                MAINLOOP();
                crusn_yield_display_interrupt();
                crusn_pace_validation_frame(frame_ticks);
                continue;
            }
            /*
             * Validation compares one emulated frame at a time. Driving INT0
             * from wall time makes added diagnostics change the number of IRQ
             * ticks between MAINLOOP calls, and therefore changes game state.
             */
            INT0();
            if (INFRAMES >= FRAMRATE + 1 && CLEARRDY == 0) {
                int frame_ticks = INFRAMES;
                MAINLOOP();
                crusn_yield_display_interrupt();
                crusn_pace_validation_frame(frame_ticks);
            } else {
                crusn_pump_events();
            }
            continue;
        }

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
    if (options.sound) {
        portable_audio_shutdown();
    }
    crusn_machine_shutdown(&machine);
    SDL_Quit();
    return 0;
}

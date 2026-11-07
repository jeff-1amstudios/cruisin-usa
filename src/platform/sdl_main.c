#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <limits.h>
#include <unistd.h>
#endif

#include "../portable/audio.h"
#include "../portable/input.h"
#include "../portable/machine.h"
#include "../portable/validator.h"
#include "../game/cmos.h"
#include "../game/cusa.h"
#include "../game/globals.h"
#include "../game/hstdp.h"
#include "sdl_video.h"
#include "../portable/startup.h"
#include "../portable/rom.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EM_JS(void, crusn_notify_game_started, (), {
    if (Module.onGameStarted) Module.onGameStarted();
});
#endif
#ifndef CRUSN_ROM_URL
#define CRUSN_ROM_URL "https://archive.org/download/crusnusa/crusnusa.zip"
#endif

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
    int widescreen;
    int bilinear;
    int test_highscore_input;
    int skip_boot_screen;
    const char* rom_url;
} crusn_options;

typedef struct crusn_app {
    crusn_machine machine;
    crusn_video video;
    crusn_options options;
    portable_startup startup;
    u32 startup_pixels[CRUSN_SCREEN_WIDTH * CRUSN_SCREEN_HEIGHT];
    int running;
    int game_started;
    int machine_ready;
    int audio_ready;
    int failed;
    Uint64 previous_counter;
    Uint64 int0_accumulator;
} crusn_app;

static crusn_app g_app;

#ifdef __APPLE__
static void crusn_use_bundle_parent_directory(const char* executable_path) {
    char resolved_path[PATH_MAX];
    char* bundle_suffix;
    char* app_name;

    if (access("roms/crusnusa.zip", R_OK) == 0 ||
        realpath(executable_path, resolved_path) == NULL) {
        return;
    }

    bundle_suffix = strstr(resolved_path, ".app/Contents/MacOS/");
    if (bundle_suffix == NULL) {
        return;
    }

    *bundle_suffix = '\0';
    app_name = strrchr(resolved_path, '/');
    if (app_name == NULL) {
        return;
    }
    *app_name = '\0';

    if (chdir(resolved_path) != 0) {
        fprintf(stderr, "Unable to use application directory for ROM cache\n");
    }
}
#endif

static int process_args(int argc, char* argv[], crusn_options* options) {
    const char prefix[] = "--race-time=";

    options->free_play = 1;
    options->sound = 1;
    options->girls = -1;
    options->race_time = 0;
    options->windowed = 0;
    options->widescreen = 0;
    options->bilinear = 1;
    options->test_highscore_input = 0;
    options->skip_boot_screen = 1;
    options->rom_url = getenv("CRUSN_ROM_URL");
    if (options->rom_url == NULL) options->rom_url = CRUSN_ROM_URL;

    for (int i = 1; i < argc; ++i) {
        const char* value = NULL;

        if (strcmp(argv[i], "--rom-url") == 0 && i + 1 < argc) {
            options->rom_url = argv[++i];
        } else if (strncmp(argv[i], "--rom-url=", 10) == 0) {
            options->rom_url = argv[i] + 10;
        } else if (strcmp(argv[i], "--no-free-play") == 0) {
            options->free_play = 0;
        } else if (strcmp(argv[i], "--no-sound") == 0) {
            options->sound = 0;
        } else if (strcmp(argv[i], "--girls") == 0) {
            options->girls = 1;
        } else if (strcmp(argv[i], "--no-girls") == 0) {
            options->girls = 0;
        } else if (strcmp(argv[i], "--widescreen") == 0) {
            options->widescreen = 1;
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
        if (!g_app.game_started) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r &&
                g_app.startup.phase == PORTABLE_STARTUP_ERROR) {
                portable_startup_retry(&g_app.startup);
            }
            if (event.type == SDL_DROPFILE) {
                portable_startup_import(&g_app.startup, event.drop.file);
            }
        }
        if (event.type == SDL_DROPFILE) SDL_free(event.drop.file);
    }

    if (g_app.game_started) port_handle_input();
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

static void crusn_shutdown(void) {
    portable_startup_shutdown(&g_app.startup);
    if (g_app.audio_ready) portable_audio_shutdown();
    if (g_app.machine_ready) crusn_machine_shutdown(&g_app.machine);
    crusn_video_shutdown(&g_app.video);
    SDL_Quit();
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
}

static int crusn_start_game(void) {
    crusn_set_widescreen(g_app.options.widescreen && getenv("CRUSN_ENABLE_MAME_VALIDATION") == NULL);
    if (crusn_machine_init(&g_app.machine) != 0) {
        snprintf(g_app.startup.message, sizeof(g_app.startup.message), "Failed to initialize machine");
        return -1;
    }
    g_app.machine_ready = 1;
    if (g_app.options.sound) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0 || portable_audio_init(PORTABLE_ROM_PATH) != 0) {
            snprintf(g_app.startup.message, sizeof(g_app.startup.message), "Failed to initialize audio");
            crusn_machine_shutdown(&g_app.machine);
            g_app.machine_ready = 0;
            return -1;
        }
        g_app.audio_ready = 1;
    }
    g_app.game_started = 1;
    mame_validate_fail_on_wrong_consumer(1);

    _c_int00();

    if (g_app.options.race_time > 0) {
        ADJUSTMENT_WRITE(ADJ_TIME_TO_START, (g_app.options.race_time - 60) / 5);
    }
    if (g_app.options.free_play) {
        ADJUSTMENT_WRITE(ADJ_FREE_PLAY, 1);
    }
    if (g_app.options.girls >= 0) {
        ADJUSTMENT_WRITE(ADJ_GIRLS, g_app.options.girls);
    }
    if (g_app.options.test_highscore_input) {
        START_HIGH_SCORE_INPUT_TEST();
    }


    g_app.previous_counter = SDL_GetPerformanceCounter();
    g_app.int0_accumulator = SDL_GetPerformanceFrequency();
#ifdef __EMSCRIPTEN__
    crusn_notify_game_started();
#endif
    return 0;
}

static void crusn_step(void* userdata) {
    (void)userdata;
    if (!g_app.running) {
        crusn_shutdown();
        return;
    }
    if (!g_app.game_started) {
        crusn_pump_events();
        if (!g_app.running) return;
        if (g_app.startup.phase == PORTABLE_STARTUP_READY) {
            if (crusn_start_game() != 0) g_app.startup.phase = PORTABLE_STARTUP_ERROR;
            return;
        }
        portable_startup_tick(&g_app.startup);
        portable_startup_draw(&g_app.startup, g_app.startup_pixels,
            CRUSN_SCREEN_WIDTH, CRUSN_SCREEN_HEIGHT, SDL_GetTicks());
        if (crusn_video_present_pixels(&g_app.video, g_app.startup_pixels) != 0) {
            fprintf(stderr, "Failed to draw startup UI: %s\n", SDL_GetError());
            g_app.failed = 1;
            g_app.running = 0;
        }
#ifndef __EMSCRIPTEN__
        SDL_Delay(1);
#endif
        return;
    }
    const Uint64 counter_frequency = SDL_GetPerformanceFrequency();
    const int deterministic_validation = getenv("CRUSN_ENABLE_MAME_VALIDATION") != NULL;

    if (deterministic_validation) {
        if (mame_validation_replay_started()) {
            int frame_ticks = mame_validate_frame_ticks();
            while (INFRAMES < frame_ticks) {
                INT0();
            }
            MAINLOOP();
            crusn_yield_display_interrupt();
            crusn_pace_validation_frame(frame_ticks);
            return;
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
        return;
    }

    Uint64 current_counter = SDL_GetPerformanceCounter();
#ifdef __EMSCRIPTEN__
    /* A suspended/background tab must not accumulate seconds of IRQ work. */
    if (current_counter - g_app.previous_counter > counter_frequency / 4) {
        g_app.previous_counter = current_counter;
        g_app.int0_accumulator = 0;
    }
#endif
    g_app.int0_accumulator += (current_counter - g_app.previous_counter) * TIKS_PER_SECOND;
    g_app.previous_counter = current_counter;

    while (g_app.int0_accumulator >= counter_frequency) {
        INT0();
        g_app.int0_accumulator -= counter_frequency;
    }

    /* ZSORTWT raises CLEARRDY and the original MAINLOOP waits for INT0
       to clear it before dispatching processes for another frame. */
    if (INFRAMES >= FRAMRATE + 1 && CLEARRDY == 0) {
        MAINLOOP();
        crusn_yield_display_interrupt();
    } else {
        crusn_pump_events();
#ifndef __EMSCRIPTEN__
        SDL_Delay(1);
#endif
    }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE int crusn_can_import(void) {
    return !g_app.game_started && g_app.startup.phase != PORTABLE_STARTUP_READY &&
        g_app.startup.phase != PORTABLE_STARTUP_SAVE;
}
EMSCRIPTEN_KEEPALIVE void crusn_import_rom(void) {
    if (crusn_can_import()) portable_startup_import(&g_app.startup, NULL);
}
EMSCRIPTEN_KEEPALIVE void crusn_retry_download(void) {
    if (!g_app.game_started && g_app.startup.phase == PORTABLE_STARTUP_ERROR) {
        portable_startup_retry(&g_app.startup);
    }
}
#endif

int main(int argc, char* argv[]) {
#ifdef __APPLE__
    crusn_use_bundle_parent_directory(argv[0]);
#endif
    if (process_args(argc, argv, &g_app.options) != 0) return 1;
#ifdef __EMSCRIPTEN__
    g_app.options.windowed = 1;
    g_app.options.skip_boot_screen = 1;
#endif
    g_skip_boot_screen = g_app.options.skip_boot_screen;
    g_app.running = 1;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    if (crusn_video_init(&g_app.video, !g_app.options.windowed, g_app.options.bilinear) != 0) {
        fprintf(stderr, "Failed to initialize video: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    g_display_machine = &g_app.machine;
    g_display_video = &g_app.video;
    g_display_running = &g_app.running;
    portable_startup_init(&g_app.startup, g_app.options.rom_url, g_app.options.sound);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(crusn_step, NULL, 0, 1);
#else
    while (g_app.running) crusn_step(NULL);
    crusn_shutdown();
#endif
    return g_app.failed;
}

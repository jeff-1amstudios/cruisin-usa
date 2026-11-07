/* Headless integration probe: exercises production startup/download code without
   calling any translated game functions or requiring a window/audio device. */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif
#include "startup.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char** argv) {
    portable_startup startup;
    if (argc < 2) return 2;
    portable_startup_init(&startup, argv[1], argc > 3 && strcmp(argv[3], "--sound") == 0);
    if (argc > 3 && strcmp(argv[3], "--retry") == 0) portable_startup_retry(&startup);
    if (argc > 2 && argv[2][0]) portable_startup_import(&startup, argv[2]);
    time_t deadline = time(NULL) + 15;
    while (time(NULL) < deadline && startup.phase != PORTABLE_STARTUP_READY &&
           startup.phase != PORTABLE_STARTUP_ERROR) {
        portable_startup_tick(&startup);
#ifdef _WIN32
        Sleep(1);
#else
        const struct timespec delay = { 0, 1000000 };
        nanosleep(&delay, NULL);
#endif
    }
    int ok = startup.phase == PORTABLE_STARTUP_READY;
    printf("%s: %s\n", ok ? "READY" : "ERROR", startup.message);
    portable_startup_shutdown(&startup);
    return ok ? 0 : 1;
}

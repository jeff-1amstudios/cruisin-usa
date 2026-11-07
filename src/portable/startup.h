#ifndef CRUSN_STARTUP_H
#define CRUSN_STARTUP_H
#include "download.h"
#include "port_types.h"
typedef enum portable_startup_phase {
    PORTABLE_STARTUP_CHECK, PORTABLE_STARTUP_DOWNLOAD,
    PORTABLE_STARTUP_VALIDATE, PORTABLE_STARTUP_SAVE,
    PORTABLE_STARTUP_READY, PORTABLE_STARTUP_ERROR
} portable_startup_phase;
typedef struct portable_startup {
    portable_startup_phase phase;
    portable_download download;
    const char* url;
    char message[160];
    int candidate;
    int force_download;
    int require_sound;
} portable_startup;
void portable_startup_init(portable_startup* startup, const char* url, int require_sound);
void portable_startup_tick(portable_startup* startup);
void portable_startup_retry(portable_startup* startup);
/* NULL path means the host already wrote PORTABLE_ROM_PART_PATH. */
int portable_startup_import(portable_startup* startup, const char* path);
void portable_startup_shutdown(portable_startup* startup);
/* Draw into a host-independent ARGB framebuffer. */
void portable_startup_draw(const portable_startup* startup, u32* pixels, int width, int height, unsigned ticks);
#endif

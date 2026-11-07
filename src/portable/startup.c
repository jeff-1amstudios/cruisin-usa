#include "startup.h"
#include "rom.h"
#include "dcs_board.h"
#include <stdio.h>
#include <string.h>

static void fail(portable_startup* startup, const char* message) {
    snprintf(startup->message, sizeof(startup->message), "%s", message);
    startup->phase = PORTABLE_STARTUP_ERROR;
}

void portable_startup_init(portable_startup* startup, const char* url, int require_sound) {
    memset(startup, 0, sizeof(*startup));
    startup->url = url;
    startup->require_sound = require_sound;
    if (portable_cache_prepare(startup->message, sizeof(startup->message)) != 0) {
        startup->phase = PORTABLE_STARTUP_ERROR;
    }
}

void portable_startup_retry(portable_startup* startup) {
    portable_download_cancel(&startup->download);
    startup->candidate = 0;
    startup->force_download = 1;
    startup->message[0] = '\0';
    startup->phase = PORTABLE_STARTUP_CHECK;
    if (portable_cache_prepare(startup->message, sizeof(startup->message)) != 0) {
        startup->phase = PORTABLE_STARTUP_ERROR;
    }
}

int portable_startup_import(portable_startup* startup, const char* path) {
    if (startup->phase == PORTABLE_STARTUP_READY || startup->phase == PORTABLE_STARTUP_SAVE) {
        return -1;
    }
    portable_download_cancel(&startup->download);
    if (path != NULL) {
        unsigned char buffer[16384];
        size_t count;
        size_t total = 0;
        int ok = 1;
        FILE* source = fopen(path, "rb");
        FILE* target = source ? fopen(PORTABLE_ROM_PART_PATH, "wb") : NULL;
        if (target == NULL) {
            if (source) fclose(source);
            fail(startup, "Unable to open ROM file");
            return -1;
        }
        while ((count = fread(buffer, 1, sizeof(buffer), source)) != 0) {
            total += count;
            if (total > PORTABLE_DOWNLOAD_LIMIT || fwrite(buffer, 1, count, target) != count) {
                ok = 0;
                break;
            }
        }
        if (ferror(source)) ok = 0;
        fclose(source);
        if (fclose(target) != 0) ok = 0;
        if (!ok) {
            remove(PORTABLE_ROM_PART_PATH);
            fail(startup, "Unable to copy ROM file (64 MB limit)");
            return -1;
        }
    }
    startup->candidate = 1;
    startup->phase = PORTABLE_STARTUP_VALIDATE;
    return 0;
}

void portable_startup_tick(portable_startup* startup) {
    switch (startup->phase) {
    case PORTABLE_STARTUP_CHECK: {
        FILE* existing = startup->force_download ? NULL : fopen(PORTABLE_ROM_PATH, "rb");
        if (existing) {
            fclose(existing);
            startup->candidate = 0;
            startup->phase = PORTABLE_STARTUP_VALIDATE;
        } else if (startup->url == NULL || startup->url[0] == '\0') {
            fail(startup, "ROM missing - select or drop crusnusa.zip");
        } else {
            startup->phase = PORTABLE_STARTUP_DOWNLOAD;
            if (portable_download_begin(&startup->download, startup->url, PORTABLE_ROM_PART_PATH) != 0) {
                fail(startup, startup->download.error);
                remove(PORTABLE_ROM_PART_PATH);
            }
        }
        break;
    }
    case PORTABLE_STARTUP_DOWNLOAD:
        portable_download_poll(&startup->download);
        if (startup->download.status == PORTABLE_DOWNLOAD_ERROR) {
            fail(startup, startup->download.error);
            portable_download_cancel(&startup->download);
            remove(PORTABLE_ROM_PART_PATH);
        } else if (startup->download.status == PORTABLE_DOWNLOAD_DONE) {
            portable_download_cancel(&startup->download);
            startup->candidate = 1;
            startup->phase = PORTABLE_STARTUP_VALIDATE;
        }
        break;
    case PORTABLE_STARTUP_VALIDATE: {
        const char* path = startup->candidate ? PORTABLE_ROM_PART_PATH : PORTABLE_ROM_PATH;
        if (portable_rom_load(path, NULL, 0) != 0) {
            if (startup->candidate) remove(PORTABLE_ROM_PART_PATH);
            fail(startup, "Invalid ROM ZIP - expected the version 4.5 set");
            break;
        }
        if (startup->require_sound) {
            char error[160];
            if (dcs_board_validate_roms(path, error, sizeof(error)) != 0) {
                if (startup->candidate) remove(PORTABLE_ROM_PART_PATH);
                fail(startup, error);
                break;
            }
        }
        if (startup->candidate) {
            /* Only replace the cache after the candidate passes validation. */
            if (portable_cache_commit(PORTABLE_ROM_PART_PATH, PORTABLE_ROM_PATH) != 0) {
                fail(startup, "Unable to replace cached ROM - check permissions");
                break;
            }
            portable_cache_save_begin();
            startup->phase = PORTABLE_STARTUP_SAVE;
        } else {
            startup->phase = PORTABLE_STARTUP_READY;
        }
        break;
    }
    case PORTABLE_STARTUP_SAVE: {
        int result = portable_cache_save_poll();
        if (result != 0) {
            /* A storage quota failure must not prevent this session playing.
               The valid ROM remains available in the in-memory filesystem. */
            if (result < 0) fprintf(stderr, "ROM cache persistence failed; using this session only\n");
            startup->phase = PORTABLE_STARTUP_READY;
        }
        break;
    }
    default:
        break;
    }
}

void portable_startup_shutdown(portable_startup* startup) {
    portable_download_cancel(&startup->download);
    remove(PORTABLE_ROM_PART_PATH);
}

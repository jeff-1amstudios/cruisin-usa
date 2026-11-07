#include "../portable/download.h"

#include <emscripten.h>
#include <emscripten/fetch.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct web_download {
    emscripten_fetch_t* fetch;
    portable_download* owner;
    char* path;
} web_download;

static void completed(emscripten_fetch_t* fetch) {
    web_download* state = fetch->userData;
    portable_download* download = state->owner;
    download->received = fetch->numBytes;
    if (fetch->status < 200 || fetch->status >= 300 || fetch->numBytes > PORTABLE_DOWNLOAD_LIMIT) {
        download->status = PORTABLE_DOWNLOAD_ERROR;
        snprintf(download->error, sizeof(download->error), "ROM download failed (HTTP %u or size limit)", fetch->status);
        return;
    }
    FILE* file = fopen(state->path, "wb");
    int ok = file != NULL;
    if (file) {
        if (fwrite(fetch->data, 1, (size_t)fetch->numBytes, file) != fetch->numBytes) ok = 0;
        if (fclose(file) != 0) ok = 0;
    }
    download->status = ok ? PORTABLE_DOWNLOAD_DONE : PORTABLE_DOWNLOAD_ERROR;
    if (!ok) snprintf(download->error, sizeof(download->error), "Unable to save ROM download");
}

static void failed(emscripten_fetch_t* fetch) {
    web_download* state = fetch->userData;
    state->owner->status = PORTABLE_DOWNLOAD_ERROR;
    snprintf(state->owner->error, sizeof(state->owner->error),
        "Download failed (HTTP %u) - check URL and CORS", fetch->status);
}

static void progress(emscripten_fetch_t* fetch) {
    web_download* state = fetch->userData;
    state->owner->received = fetch->dataOffset + fetch->numBytes;
    state->owner->total = fetch->totalBytes;
    if (state->owner->received > PORTABLE_DOWNLOAD_LIMIT || fetch->totalBytes > PORTABLE_DOWNLOAD_LIMIT) {
        state->owner->status = PORTABLE_DOWNLOAD_ERROR;
        snprintf(state->owner->error, sizeof(state->owner->error), "ROM download exceeds the 64 MB limit");
        /* Cancellation happens on the next startup tick, outside this callback. */
    }
}

void portable_download_cancel(portable_download* download) {
    web_download* state = download->backend;
    if (state) {
        if (state->fetch) emscripten_fetch_close(state->fetch);
        free(state->path);
        free(state);
    }
    download->backend = NULL;
}

int portable_download_begin(portable_download* download, const char* url, const char* path) {
    portable_download_cancel(download);
    memset(download, 0, sizeof(*download));
    if (strncmp(url, "https://", 8) != 0 && strncmp(url, "http://", 7) != 0) goto fail;
    web_download* state = calloc(1, sizeof(*state));
    if (state == NULL) goto fail;
    download->backend = state;
    state->owner = download;
    /* Internet Archive's download redirects omit CORS headers. Its official
       /cors/ route serves the same public bytes with browser access enabled. */
    char* browser_url = NULL;
    const char archive_prefix[] = "https://archive.org/download/";
    if (strncmp(url, archive_prefix, sizeof(archive_prefix) - 1) == 0) {
        browser_url = malloc(strlen(url) + 1);
        if (!browser_url) goto fail;
        sprintf(browser_url, "https://archive.org/cors/%s", url + sizeof(archive_prefix) - 1);
        url = browser_url;
    }
    state->path = malloc(strlen(path) + 1);
    if (!state->path) { free(browser_url); goto fail; }
    strcpy(state->path, path);
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.timeoutMSecs = 180000;
    attr.userData = state;
    attr.onsuccess = completed;
    attr.onerror = failed;
    attr.onprogress = progress;
    download->status = PORTABLE_DOWNLOAD_ACTIVE;
    state->fetch = emscripten_fetch(&attr, url);
    free(browser_url);
    if (!state->fetch) goto fail;
    return 0;
fail:
    portable_download_cancel(download);
    download->status = PORTABLE_DOWNLOAD_ERROR;
    snprintf(download->error, sizeof(download->error), "Unable to start ROM download");
    return -1;
}

void portable_download_poll(portable_download* download) { (void)download; }
int portable_cache_prepare(char* error, size_t error_size) {
    /* The web shell restores IDBFS before main starts. */
    if (mkdir("roms", 0755) != 0 && errno != EEXIST) {
        snprintf(error, error_size, "Unable to create ROM cache: %s", strerror(errno));
        return -1;
    }
    return 0;
}
int portable_cache_commit(const char* candidate, const char* destination) {
    return rename(candidate, destination);
}
EM_JS(void, portable_cache_save_begin, (), {
    Module.crusnCacheSave = 0;
    FS.syncfs(false, function(error) {
        Module.crusnCacheSave = error ? -1 : 1;
        if (error) console.warn('ROM cache could not be saved', error);
    });
});
EM_JS(int, portable_cache_save_poll, (), { return Module.crusnCacheSave || 0; });

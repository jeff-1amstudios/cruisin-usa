#include "../portable/download.h"

#include <curl/curl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#endif

typedef struct native_download {
    CURLM* multi;
    CURL* easy;
    FILE* file;
    portable_download* owner;
    char curl_error[CURL_ERROR_SIZE];
} native_download;

static size_t receive_bytes(char* data, size_t size, size_t count, void* userdata) {
    native_download* state = userdata;
    portable_download* download = state->owner;
    size_t bytes = size * count;
    if (download->received + bytes > PORTABLE_DOWNLOAD_LIMIT) {
        snprintf(download->error, sizeof(download->error), "ROM download exceeds the 64 MB limit");
        return 0;
    }
    size_t written = fwrite(data, 1, bytes, state->file);
    download->received += written;
    return written;
}

static int progress(void* userdata, curl_off_t total, curl_off_t received,
                    curl_off_t upload_total, curl_off_t uploaded) {
    portable_download* download = userdata;
    (void)received;
    (void)upload_total;
    (void)uploaded;
    download->total = total > 0 ? (uint64_t)total : 0;
    return total > PORTABLE_DOWNLOAD_LIMIT;
}

void portable_download_cancel(portable_download* download) {
    native_download* state = download->backend;
    if (state) {
        if (state->multi && state->easy) curl_multi_remove_handle(state->multi, state->easy);
        if (state->easy) curl_easy_cleanup(state->easy);
        if (state->multi) curl_multi_cleanup(state->multi);
        if (state->file) fclose(state->file);
        free(state);
    }
    download->backend = NULL;
}

int portable_download_begin(portable_download* download, const char* url, const char* path) {
    static int curl_initialized;
    portable_download_cancel(download);
    memset(download, 0, sizeof(*download));
    if (!curl_initialized) {
        if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) goto fail;
        curl_initialized = 1;
    }
    native_download* state = calloc(1, sizeof(*state));
    if (state == NULL) goto fail;
    download->backend = state;
    state->owner = download;
    state->file = fopen(path, "wb");
    state->multi = curl_multi_init();
    state->easy = curl_easy_init();
    if (!state->file || !state->multi || !state->easy) goto fail;
#define SET(option, value) if (curl_easy_setopt(state->easy, option, value) != CURLE_OK) goto fail
    SET(CURLOPT_URL, url);
#if LIBCURL_VERSION_NUM >= 0x075500
    if (curl_version_info(CURLVERSION_NOW)->version_num >= 0x075500) {
        SET(CURLOPT_PROTOCOLS_STR, "http,https");
        SET(CURLOPT_REDIR_PROTOCOLS_STR, "http,https");
    } else
#endif
    {
        /* macOS may supply an older libcurl at runtime than the build SDK. */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
        SET(CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
        SET(CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    }
#ifdef _WIN32
    char ca_bundle[MAX_PATH];
    DWORD path_length = GetModuleFileNameA(NULL, ca_bundle, sizeof(ca_bundle));
    char* slash = path_length && path_length < sizeof(ca_bundle) ? strrchr(ca_bundle, '\\') : NULL;
    if (slash && (size_t)(slash + 1 - ca_bundle) + sizeof("curl-ca-bundle.crt") <= sizeof(ca_bundle)) {
        strcpy(slash + 1, "curl-ca-bundle.crt");
        FILE* certificates = fopen(ca_bundle, "rb");
        if (certificates) {
            fclose(certificates);
            SET(CURLOPT_CAINFO, ca_bundle);
        }
    }
#endif
    SET(CURLOPT_FOLLOWLOCATION, 1L);
    SET(CURLOPT_MAXREDIRS, 5L);
    SET(CURLOPT_FAILONERROR, 1L);
    SET(CURLOPT_CONNECTTIMEOUT, 15L);
    SET(CURLOPT_TIMEOUT, 180L);
    SET(CURLOPT_LOW_SPEED_LIMIT, 1024L);
    SET(CURLOPT_LOW_SPEED_TIME, 30L);
    SET(CURLOPT_NOSIGNAL, 1L);
    SET(CURLOPT_MAXFILESIZE_LARGE, (curl_off_t)PORTABLE_DOWNLOAD_LIMIT);
    SET(CURLOPT_WRITEFUNCTION, receive_bytes);
    SET(CURLOPT_WRITEDATA, state);
    SET(CURLOPT_XFERINFOFUNCTION, progress);
    SET(CURLOPT_XFERINFODATA, download);
    SET(CURLOPT_NOPROGRESS, 0L);
    SET(CURLOPT_ERRORBUFFER, state->curl_error);
#undef SET
    if (curl_multi_add_handle(state->multi, state->easy) != CURLM_OK) goto fail;
    download->status = PORTABLE_DOWNLOAD_ACTIVE;
    return 0;
fail:
    snprintf(download->error, sizeof(download->error), "Unable to start ROM download");
    portable_download_cancel(download);
    download->status = PORTABLE_DOWNLOAD_ERROR;
    return -1;
}

void portable_download_poll(portable_download* download) {
    native_download* state = download->backend;
    int running;
    int pending;
    if (!state || download->status != PORTABLE_DOWNLOAD_ACTIVE) return;
    if (curl_multi_perform(state->multi, &running) != CURLM_OK) {
        snprintf(download->error, sizeof(download->error), "Network transfer failed");
        download->status = PORTABLE_DOWNLOAD_ERROR;
        return;
    }
    CURLMsg* message;
    while ((message = curl_multi_info_read(state->multi, &pending)) != NULL) {
        if (message->msg != CURLMSG_DONE) continue;
        int close_result = fclose(state->file);
        state->file = NULL;
        if (message->data.result == CURLE_OK && close_result == 0) {
            download->status = PORTABLE_DOWNLOAD_DONE;
        } else {
            if (download->error[0] == '\0') {
                snprintf(download->error, sizeof(download->error), "%.150s",
                    close_result != 0 ? "Unable to save ROM download" :
                    state->curl_error[0] ? state->curl_error : curl_easy_strerror(message->data.result));
            }
            download->status = PORTABLE_DOWNLOAD_ERROR;
        }
    }
}

int portable_cache_prepare(char* error, size_t error_size) {
#ifdef _WIN32
    int result = _mkdir("roms");
#else
    int result = mkdir("roms", 0755);
#endif
    if (result != 0 && errno != EEXIST) {
        snprintf(error, error_size, "Unable to create roms directory: %s", strerror(errno));
        return -1;
    }
    return 0;
}

int portable_cache_commit(const char* candidate, const char* destination) {
#ifdef _WIN32
    return MoveFileExA(candidate, destination, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) ? 0 : -1;
#else
    return rename(candidate, destination);
#endif
}

void portable_cache_save_begin(void) {}
int portable_cache_save_poll(void) { return 1; }

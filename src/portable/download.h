#ifndef CRUSN_DOWNLOAD_H
#define CRUSN_DOWNLOAD_H
#include <stddef.h>
#include <stdint.h>
/* Implemented by the host. No host types escape this interface. */
#define PORTABLE_DOWNLOAD_LIMIT (64u * 1024u * 1024u)
typedef enum portable_download_status {
    PORTABLE_DOWNLOAD_IDLE, PORTABLE_DOWNLOAD_ACTIVE,
    PORTABLE_DOWNLOAD_DONE, PORTABLE_DOWNLOAD_ERROR
} portable_download_status;
typedef struct portable_download {
    void* backend;
    portable_download_status status;
    uint64_t received;
    uint64_t total;
    char error[160];
} portable_download;
int portable_download_begin(portable_download* download, const char* url, const char* path);
void portable_download_poll(portable_download* download);
void portable_download_cancel(portable_download* download);
int portable_cache_prepare(char* error, size_t error_size);
int portable_cache_commit(const char* candidate, const char* destination);
void portable_cache_save_begin(void);
/* 0 = saving, 1 = saved, -1 = failed. */
int portable_cache_save_poll(void);
#endif

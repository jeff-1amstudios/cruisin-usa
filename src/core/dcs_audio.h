#ifndef CRUSN_DCS_AUDIO_H
#define CRUSN_DCS_AUDIO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { DCS_AUDIO_SAMPLE_RATE = 31250 };

typedef struct dcs_pcm_track {
    const int16_t* samples;
    size_t sample_count;
    int looping;
} dcs_pcm_track;

typedef struct dcs_pcm_bank dcs_pcm_bank;

dcs_pcm_bank* dcs_audio_decode_roms(const char* zip_path, char* error, size_t error_size);
void dcs_audio_free(dcs_pcm_bank* bank);
const dcs_pcm_track* dcs_audio_track(const dcs_pcm_bank* bank, unsigned track_number);
const dcs_pcm_track* dcs_audio_engine_loop(const dcs_pcm_bank* bank);
size_t dcs_audio_decoded_track_count(const dcs_pcm_bank* bank);
size_t dcs_audio_decoded_sample_count(const dcs_pcm_bank* bank);

#ifdef __cplusplus
}
#endif

#endif


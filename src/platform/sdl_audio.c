#include "../core/audio.h"

#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../core/dcs_audio.h"
#include "../game/sndtaba.h"

enum {
    CRUSN_AUDIO_CHANNELS = 4,
    DCS_COMMAND_KILL_ALL = 0,
    DCS_COMMAND_KILL_CHANNEL_0 = 995,
    DCS_COMMAND_KILL_CHANNEL_1 = 996,
    DCS_COMMAND_KILL_CHANNEL_2 = 997,
    DCS_COMMAND_KILL_CHANNEL_3 = 998,
    DCS_COMMAND_ENGINE_IDLE = 1000,
    DCS_COMMAND_MASTER_VOLUME = 0x55aa,
    DCS_COMMAND_TRACK_VOLUME_BASE = 0x55ab,
    DCS_COMMAND_PLAYER_ENGINE = 0x55cc,
};

typedef struct crusn_audio_voice {
    const dcs_pcm_track* track;
    uint64_t position;
    int sound_index;
    unsigned priority_word;
    int volume;
} crusn_audio_voice;

typedef struct crusn_engine_voice {
    const dcs_pcm_track* track;
    size_t source_position;
    uint32_t source_phase;
    uint32_t source_increment;
    uint32_t source_threshold;
    uint32_t output_phase;
    int native_sample_a;
    int native_sample_b;
    int primed;
    int volume;
} crusn_engine_voice;

typedef struct crusn_audio_state {
    SDL_AudioDeviceID device;
    dcs_pcm_bank* bank;
    crusn_audio_voice voices[CRUSN_AUDIO_CHANNELS];
    crusn_engine_voice engine;
    uint32_t master_gain;
    int channel_volume[CRUSN_AUDIO_CHANNELS];
    int output_frequency;
    int output_channels;
    uint32_t source_step;
    int pending_command;
} crusn_audio_state;

static crusn_audio_state g_audio;

static int clamp_u8(int value) {
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

static int16_t clamp_s16(int value) {
    if (value < -32768)
        return -32768;
    if (value > 32767)
        return 32767;
    return (int16_t)value;
}

static uint32_t dcs_master_gain(int volume) {
    unsigned value = (unsigned)clamp_u8(volume);
    if (value == 0)
        return 0;

    uint16_t x = 0x3fff;
    uint16_t y = 0x7d98;
    for (int bit = 0; bit < 8; ++bit) {
        if ((value & 1u) == 0)
            x = (uint16_t)(((uint32_t)x * y) >> 15);
        y = (uint16_t)(((uint32_t)y * y) >> 15);
        value >>= 1;
    }
    return (uint32_t)x << 1;
}

static int engine_next_native_sample(crusn_engine_voice* engine) {
    const int sample = engine->track->samples[engine->source_position];

    engine->source_phase += engine->source_increment;
    while (engine->source_phase >= engine->source_threshold) {
        engine->source_phase -= engine->source_threshold;
        engine->source_position++;
        if (engine->source_position >= engine->track->sample_count)
            engine->source_position = 0;
    }
    return sample;
}

static void mix_audio(void* userdata, Uint8* stream, int byte_count) {
    crusn_audio_state* audio = userdata;
    int16_t* output = (int16_t*)stream;
    const int frames = byte_count / ((int)sizeof(*output) * audio->output_channels);

    for (int out = 0; out < frames; ++out) {
        int mixed = 0;
        for (int channel = 0; channel < CRUSN_AUDIO_CHANNELS; ++channel) {
            crusn_audio_voice* voice = &audio->voices[channel];
            if (voice->track == NULL)
                continue;

            const uint64_t end = (uint64_t)voice->track->sample_count << 16;
            if (voice->position >= end) {
                if (voice->track->looping)
                    voice->position %= end;
                else {
                    memset(voice, 0, sizeof(*voice));
                    voice->sound_index = -1;
                    continue;
                }
            }

            const size_t position = (size_t)(voice->position >> 16);
            const size_t next = position + 1 < voice->track->sample_count
                ? position + 1
                : (voice->track->looping ? 0 : position);
            const uint32_t fraction = (uint32_t)voice->position & 0xffffu;
            const int a = voice->track->samples[position];
            const int b = voice->track->samples[next];
            const int sample = a + (int)(((int64_t)(b - a) * fraction) >> 16);
            const int gain = voice->volume * audio->channel_volume[channel];
            mixed += sample * gain / (255 * 255);
            voice->position += audio->source_step;
        }

        if (audio->engine.track != NULL && audio->engine.source_increment != 0 && audio->engine.volume != 0) {
            if (!audio->engine.primed) {
                audio->engine.native_sample_a = engine_next_native_sample(&audio->engine);
                audio->engine.native_sample_b = engine_next_native_sample(&audio->engine);
                audio->engine.primed = 1;
            }

            const int sample = audio->engine.native_sample_a
                + (int)(((int64_t)(audio->engine.native_sample_b - audio->engine.native_sample_a)
                            * audio->engine.output_phase)
                    >> 16);
            mixed += sample * audio->engine.volume / 255;
            audio->engine.output_phase += audio->source_step;
            while (audio->engine.output_phase >= 0x10000u) {
                audio->engine.output_phase -= 0x10000u;
                audio->engine.native_sample_a = audio->engine.native_sample_b;
                audio->engine.native_sample_b = engine_next_native_sample(&audio->engine);
            }
        }

        const int16_t sample = clamp_s16((int)((int64_t)mixed * audio->master_gain / 32766));
        for (int channel = 0; channel < audio->output_channels; ++channel)
            output[out * audio->output_channels + channel] = sample;
    }
}

int portable_audio_init(const char* dcs_rom_zip) {
    SDL_AudioSpec wanted;
    SDL_AudioSpec obtained;
    char error[256];

    memset(&g_audio, 0, sizeof(g_audio));
    g_audio.master_gain = dcs_master_gain(255);
    for (int channel = 0; channel < CRUSN_AUDIO_CHANNELS; ++channel) {
        g_audio.channel_volume[channel] = 255;
        g_audio.voices[channel].sound_index = -1;
    }

    g_audio.bank = dcs_audio_decode_roms(dcs_rom_zip, error, sizeof(error));
    if (g_audio.bank == NULL) {
        fprintf(stderr, "DCS audio decode failed: %s\n", error);
        return -1;
    }
    g_audio.engine.track = dcs_audio_engine_loop(g_audio.bank);

    memset(&wanted, 0, sizeof(wanted));
    wanted.freq = 48000;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = 2;
    wanted.samples = 1024;
    wanted.callback = mix_audio;
    wanted.userdata = &g_audio;
    g_audio.device = SDL_OpenAudioDevice(NULL, 0, &wanted, &obtained, 0);
    if (g_audio.device == 0) {
        fprintf(stderr, "SDL audio open failed: %s\n", SDL_GetError());
        dcs_audio_free(g_audio.bank);
        g_audio.bank = NULL;
        return -1;
    }
    if (obtained.format != AUDIO_S16SYS || obtained.channels == 0) {
        fprintf(stderr, "SDL audio returned an unsupported format\n");
        SDL_CloseAudioDevice(g_audio.device);
        dcs_audio_free(g_audio.bank);
        memset(&g_audio, 0, sizeof(g_audio));
        return -1;
    }
    g_audio.output_frequency = obtained.freq;
    g_audio.output_channels = obtained.channels;
    g_audio.source_step = (uint32_t)(((uint64_t)DCS_AUDIO_SAMPLE_RATE << 16) / obtained.freq);

    fprintf(stderr, "DCS audio: decoded %zu tracks, %.1f MiB PCM\n",
        dcs_audio_decoded_track_count(g_audio.bank),
        (double)(dcs_audio_decoded_sample_count(g_audio.bank) * sizeof(int16_t)) / (1024.0 * 1024.0));
    SDL_PauseAudioDevice(g_audio.device, 0);
    return 0;
}

void portable_audio_shutdown(void) {
    if (g_audio.device != 0)
        SDL_CloseAudioDevice(g_audio.device);
    dcs_audio_free(g_audio.bank);
    memset(&g_audio, 0, sizeof(g_audio));
}

static void stop_all_voices(void) {
    memset(g_audio.voices, 0, sizeof(g_audio.voices));
    for (int i = 0; i < CRUSN_AUDIO_CHANNELS; ++i)
        g_audio.voices[i].sound_index = -1;
}

static void set_player_engine(int speed, int volume) {
    const unsigned value = (unsigned)clamp_u8(speed);
    uint32_t increment = 0;
    uint32_t threshold = 1;
    if (value != 0) {
        if (value < 128) {
            increment = value + 127;
            threshold = 255;
        } else {
            increment = (value & 127) + 127;
            threshold = 127;
        }
    }

    if (g_audio.engine.source_increment != increment
        || g_audio.engine.source_threshold != threshold) {
        g_audio.engine.source_increment = increment;
        g_audio.engine.source_threshold = threshold;
        g_audio.engine.primed = 0;
    }
    g_audio.engine.volume = clamp_u8(volume);
}

void portable_audio_send_command(int sound_code) {
    const unsigned command = (unsigned)sound_code & 0xffffu;
    const crusn_sound_entry* entry;
    crusn_audio_voice* voice;
    const dcs_pcm_track* track;
    int channel;

    if (g_audio.device == 0)
        return;
    SDL_LockAudioDevice(g_audio.device);

    if (g_audio.pending_command != 0) {
        if (g_audio.pending_command == DCS_COMMAND_MASTER_VOLUME) {
            g_audio.master_gain = dcs_master_gain((int)(command >> 8));
        } else if (g_audio.pending_command >= DCS_COMMAND_TRACK_VOLUME_BASE
            && g_audio.pending_command
                < DCS_COMMAND_TRACK_VOLUME_BASE + CRUSN_AUDIO_CHANNELS) {
            channel = g_audio.pending_command - DCS_COMMAND_TRACK_VOLUME_BASE;
            g_audio.channel_volume[channel] = clamp_u8((int)(command >> 8));
        } else if (g_audio.pending_command == DCS_COMMAND_PLAYER_ENGINE) {
            set_player_engine((int)(command >> 8), (int)(command & 0xff));
        }
        g_audio.pending_command = 0;
        SDL_UnlockAudioDevice(g_audio.device);
        return;
    }

    if (command == DCS_COMMAND_MASTER_VOLUME
        || (command >= DCS_COMMAND_TRACK_VOLUME_BASE
            && command < DCS_COMMAND_TRACK_VOLUME_BASE + CRUSN_AUDIO_CHANNELS)
        || command == DCS_COMMAND_PLAYER_ENGINE) {
        g_audio.pending_command = (int)command;
    } else if (command == DCS_COMMAND_KILL_ALL) {
        stop_all_voices();
    } else if (command >= DCS_COMMAND_KILL_CHANNEL_0
        && command <= DCS_COMMAND_KILL_CHANNEL_3) {
        channel = (int)(command - DCS_COMMAND_KILL_CHANNEL_0);
        memset(&g_audio.voices[channel], 0, sizeof(g_audio.voices[channel]));
        g_audio.voices[channel].sound_index = -1;
    } else if (command == DCS_COMMAND_ENGINE_IDLE) {
        g_audio.engine.source_increment = 127;
        g_audio.engine.source_threshold = 255;
        g_audio.engine.primed = 0;
    } else {
        entry = crusn_sound_entry_from_dcs_track(command, &channel);
        track = dcs_audio_track(g_audio.bank, command);
        if (entry != NULL && track != NULL
            && channel >= 0 && channel < CRUSN_AUDIO_CHANNELS) {
            voice = &g_audio.voices[channel];
            voice->track = track;
            voice->position = 0;
            voice->sound_index = -1;
            voice->priority_word = entry->priority_word;
            voice->volume = g_audio.channel_volume[channel];
        }
    }

    SDL_UnlockAudioDevice(g_audio.device);
}

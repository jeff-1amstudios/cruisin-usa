#include "../core/audio.h"

#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../core/dcs_board.h"

typedef struct crusn_audio_state {
    SDL_AudioDeviceID device;
    dcs_board* board;
    uint32_t source_step;
    uint32_t source_phase;
    int source_a;
    int source_b;
    int primed;
    int output_channels;
} crusn_audio_state;

static crusn_audio_state g_audio;

static int16_t clamp_s16(int value) {
    if (value < -32768)
        return -32768;
    if (value > 32767)
        return 32767;
    return (int16_t)value;
}

static void mix_audio(void* userdata, Uint8* stream, int byte_count) {
    crusn_audio_state* audio = userdata;
    int16_t* output = (int16_t*)stream;
    const int frames = byte_count / ((int)sizeof(*output) * audio->output_channels);

    if (!audio->primed) {
        audio->source_a = dcs_board_next_sample(audio->board);
        audio->source_b = dcs_board_next_sample(audio->board);
        audio->primed = 1;
    }
    for (int frame = 0; frame < frames; ++frame) {
        const int sample = audio->source_a
            + (int)(((int64_t)(audio->source_b - audio->source_a)
                        * audio->source_phase) >> 16);
        const int16_t clamped = clamp_s16(sample);
        for (int channel = 0; channel < audio->output_channels; ++channel)
            output[frame * audio->output_channels + channel] = clamped;

        audio->source_phase += audio->source_step;
        while (audio->source_phase >= 0x10000u) {
            audio->source_phase -= 0x10000u;
            audio->source_a = audio->source_b;
            audio->source_b = dcs_board_next_sample(audio->board);
        }
    }
}

int portable_audio_init(const char* dcs_rom_zip) {
    SDL_AudioSpec wanted;
    SDL_AudioSpec obtained;
    char error[256];

    memset(&g_audio, 0, sizeof(g_audio));
    g_audio.board = dcs_board_create(dcs_rom_zip, error, sizeof(error));
    if (g_audio.board == NULL) {
        fprintf(stderr, "DCS board startup failed: %s\n", error);
        return -1;
    }

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
        dcs_board_destroy(g_audio.board);
        memset(&g_audio, 0, sizeof(g_audio));
        return -1;
    }
    if (obtained.format != AUDIO_S16SYS || obtained.channels == 0) {
        fprintf(stderr, "SDL audio returned an unsupported format\n");
        SDL_CloseAudioDevice(g_audio.device);
        dcs_board_destroy(g_audio.board);
        memset(&g_audio, 0, sizeof(g_audio));
        return -1;
    }
    g_audio.output_channels = obtained.channels;
    g_audio.source_step = (uint32_t)(((uint64_t)DCS_BOARD_SAMPLE_RATE << 16)
        / (uint32_t)obtained.freq);

    fprintf(stderr, "DCS audio: ADSP-2105 board emulation at %d Hz\n",
        DCS_BOARD_SAMPLE_RATE);
    SDL_PauseAudioDevice(g_audio.device, 0);
    return 0;
}

void portable_audio_shutdown(void) {
    if (g_audio.device != 0)
        SDL_CloseAudioDevice(g_audio.device);
    dcs_board_destroy(g_audio.board);
    memset(&g_audio, 0, sizeof(g_audio));
}

void portable_audio_send_command(int sound_code) {
    if (g_audio.device == 0)
        return;
    SDL_LockAudioDevice(g_audio.device);
    dcs_board_send_command(g_audio.board, (uint16_t)sound_code);
    SDL_UnlockAudioDevice(g_audio.device);
}

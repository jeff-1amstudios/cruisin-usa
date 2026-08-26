#ifndef CORE_AUDIO_H
#define CORE_AUDIO_H

int portable_audio_init(const char* dcs_rom_zip);
void portable_audio_shutdown(void);
void portable_audio_send_command(int sound_code);

#endif

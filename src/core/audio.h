#ifndef CRUSN_AUDIO_H
#define CRUSN_AUDIO_H

int crusn_audio_init(const char* dcs_rom_zip);
void crusn_audio_shutdown(void);
void crusn_audio_set_master_volume(int volume);
void crusn_audio_set_channel_volume(int channel, int volume);
void crusn_audio_play_sound(int channel, int sound_index, int volume);
int crusn_audio_kill_sound(int sound_index);
void crusn_audio_play_track(int dcs_track);
void crusn_audio_stop_all(void);
void crusn_audio_set_player_engine(int speed, int volume);

#endif

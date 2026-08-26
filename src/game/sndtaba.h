#ifndef CRUSN_SNDTABA_H
#define CRUSN_SNDTABA_H

#include <stddef.h>

typedef struct crusn_sound_entry {
    unsigned priority_word;
    unsigned duration_ticks;
    unsigned dcs_track;
} crusn_sound_entry;

const crusn_sound_entry* crusn_sound_entry_from_index(int sound_index);
size_t crusn_sound_entry_count(void);

#endif


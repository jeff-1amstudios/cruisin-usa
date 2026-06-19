#include "cpu.h"
#include "machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

crusn_machine *g_crusn_machine = NULL;

int crusn_machine_init(crusn_machine *machine)
{
    memset(machine, 0, sizeof(*machine));
    crusn_cpu_reset();

    machine->ram_word_count = CRUSN_RAM_WORDS;
    machine->screen_word_count = CRUSN_SCREEN_WORDS;
    machine->cmos_word_count = CRUSN_CMOS_WORDS;
    machine->colorram_word_count = CRUSN_COLORRAM_WORDS;
    machine->timer_word_count = CRUSN_TIMER_WORDS;
    machine->ram_words = calloc(machine->ram_word_count, sizeof(*machine->ram_words));
    machine->screen_words = calloc(machine->screen_word_count, sizeof(*machine->screen_words));
    machine->cmos_words = calloc(machine->cmos_word_count, sizeof(*machine->cmos_words));
    machine->colorram_words = calloc(machine->colorram_word_count, sizeof(*machine->colorram_words));
    machine->timer_words = calloc(machine->timer_word_count, sizeof(*machine->timer_words));

    if (machine->ram_words == NULL || machine->screen_words == NULL || machine->cmos_words == NULL || machine->colorram_words == NULL || machine->timer_words == NULL) {
        crusn_machine_shutdown(machine);
        return -1;
    }

    crusn_trace_init(&machine->trace, stdout);
    crusn_memory_init(
        &machine->memory,
        machine->ram_words,
        machine->ram_word_count,
        machine->screen_words,
        machine->screen_word_count,
        machine->cmos_words,
        machine->cmos_word_count,
        machine->colorram_words,
        machine->colorram_word_count,
        machine->timer_words,
        machine->timer_word_count,
        &machine->trace
    );

    g_crusn_machine = machine;

    return 0;
}

void crusn_machine_shutdown(crusn_machine *machine)
{
    free(machine->ram_words);
    free(machine->screen_words);
    free(machine->cmos_words);
    free(machine->colorram_words);
    free(machine->timer_words);
    machine->ram_words = NULL;
    machine->screen_words = NULL;
    machine->cmos_words = NULL;
    machine->colorram_words = NULL;
    machine->timer_words = NULL;
    machine->ram_word_count = 0;
    machine->screen_word_count = 0;
    machine->cmos_word_count = 0;
    machine->colorram_word_count = 0;
    machine->timer_word_count = 0;
    if (g_crusn_machine == machine) {
        g_crusn_machine = NULL;
    }
}

void crusn_machine_tick(crusn_machine *machine)
{
    int x;
    int y;

    for (y = 0; y < CRUSN_SCREEN_HEIGHT; ++y) {
        for (x = 0; x < CRUSN_SCREEN_WIDTH; ++x) {
            u32 r = (u32)((x + machine->frame_counter) & 0xFF);
            u32 g = (u32)((y + machine->frame_counter) & 0xFF);
            u32 b = (u32)((x + y + machine->frame_counter) & 0xFF);
            machine->screen_words[(y * CRUSN_SCREEN_WIDTH) + x] = 0xFF000000u | (r << 16) | (g << 8) | b;
        }
    }

    TRACE_EVENT(&machine->trace, "frame", "tick", (u32)machine->frame_counter, 0);
    machine->frame_counter++;
}

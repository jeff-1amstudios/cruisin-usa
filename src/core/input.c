#include "input.h"
#include <SDL.h>

#include "../game/vunit.h"

static u32 switch1 = UINT32_MAX;
static u32 switch2 = UINT32_MAX;
static u32 switch3 = UINT32_MAX;

int done_enter = 0;

void port_handle_input(void) {
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    switch1 = UINT32_MAX;
    switch2 = UINT32_MAX;
    switch3 = UINT32_MAX;

    if (keyboard[SDL_SCANCODE_RETURN] || keyboard[SDL_SCANCODE_KP_ENTER]) {
        switch1 &= ~SW_START;
    }
    if (done_enter == 400) {
        switch1 &= ~SW_START;
        printf("done enter\n");
    }
    done_enter++;
}

u32 port_get_dipswitches(void) {
    // 2 switches
    return 0 | (DIP_COMMP | CMDP_MASTER) << 16;
}

u32 port_get_commpal(void) {
    return 4;
}

u32 port_get_switch1(void) {
    return switch1;
}

u32 port_get_switch2(void) {
    return switch2;
}

u32 port_get_switch3(void) {
    return switch3;
}

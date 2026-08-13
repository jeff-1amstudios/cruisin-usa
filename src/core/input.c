#include "input.h"
#include <SDL.h>

#include "../game/vunit.h"

static u32 switch1 = UINT32_MAX;
static u32 switch2 = UINT32_MAX;
static u32 switch3 = UINT32_MAX;
static int steering = 128;
static int steering_direction;
static int steering_detent;

int input_frame_counter = 0;

void port_handle_input(void) {
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    switch1 = UINT32_MAX;
    switch2 = UINT32_MAX;
    switch3 = UINT32_MAX;

    if (keyboard[SDL_SCANCODE_RETURN] || keyboard[SDL_SCANCODE_KP_ENTER]) {
        switch1 &= ~SW_START;
    }
    int new_steering_direction = (keyboard[SDL_SCANCODE_RIGHT] != 0) -
        (keyboard[SDL_SCANCODE_LEFT] != 0);
    if (new_steering_direction != 0 &&
        new_steering_direction != steering_direction) {
        steering_detent = new_steering_direction;
    }
    steering_direction = new_steering_direction;
    if (input_frame_counter == 400) {
        // switch1 &= ~SW_START;
        printf("done enter\n");
    }
    input_frame_counter++;
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

void port_sample_steering(void) {
    /* Called from READIO at the emulated A/D sampling boundary. */
    steering += steering_direction * 2;
    if (steering < 0)
        steering = 0;
    if (steering > 255)
        steering = 255;
}

int port_get_steering(void) {
    return steering;
}

int port_take_steering_detent(void) {
    int detent = steering_detent;
    steering_detent = 0;
    return detent;
}

void port_set_steering(int value) {
    steering = value;
    if (steering < 0)
        steering = 0;
    if (steering > 255)
        steering = 255;
}

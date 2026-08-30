#include "input.h"
#include <SDL.h>

#include "../game/vunit.h"

static u32 switch1 = UINT32_MAX;
static u32 switch2 = UINT32_MAX;
static u32 switch3 = UINT32_MAX;
static int steering = PORT_STEERING_CENTER;
static int accelerator;
static int brake;
static int steering_direction;
static int steering_detent;

#define STEERING_KEY_STEP 5
#define STEERING_CENTER_STEP 5

int input_frame_counter = 0;

void port_handle_input(void) {
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    switch1 = UINT32_MAX;
    switch2 = UINT32_MAX;
    switch3 = UINT32_MAX;

    if (keyboard[SDL_SCANCODE_RETURN] || keyboard[SDL_SCANCODE_KP_ENTER]) {
        switch1 &= ~SW_START;
    }
    // if (input_frame_counter % 200 == 0) {
    //     switch1 &= ~SW_START;
    // }
    int new_steering_direction = (keyboard[SDL_SCANCODE_RIGHT] != 0) - (keyboard[SDL_SCANCODE_LEFT] != 0);
    if (new_steering_direction != 0 && new_steering_direction != steering_direction) {
        steering_detent = new_steering_direction;
    }
    steering_direction = new_steering_direction;
    accelerator = (keyboard[SDL_SCANCODE_UP] || keyboard[SDL_SCANCODE_W]) ? 255 : 0;
    brake = (keyboard[SDL_SCANCODE_DOWN] || keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_SPACE]) ? 255 : 0;
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
    if (steering_direction != 0) {
        steering += steering_direction * STEERING_KEY_STEP;
    } else if (steering < PORT_STEERING_CENTER) {
        steering += STEERING_CENTER_STEP;
        if (steering > PORT_STEERING_CENTER)
            steering = PORT_STEERING_CENTER;
    } else if (steering > PORT_STEERING_CENTER) {
        steering -= STEERING_CENTER_STEP;
        if (steering < PORT_STEERING_CENTER)
            steering = PORT_STEERING_CENTER;
    }
    if (steering < 0)
        steering = 0;
    if (steering > 255)
        steering = 255;
}

int port_get_steering(void) {
    return steering;
}

int port_get_accelerator(void) {
    return accelerator;
}

int port_get_brake(void) {
    return brake;
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

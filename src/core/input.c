#include "input.h"
#include <SDL.h>
#include <stdlib.h>

#include "../game/vunit.h"

static u32 switch1 = UINT32_MAX;
static u32 switch2 = UINT32_MAX;
static u32 switch3 = UINT32_MAX;
static float steering = PORT_STEERING_CENTER;
static int accelerator;
static int brake;
static int steering_direction;
static int steering_detent;

#define STEERING_HALF_RANGE 127.0f
#define STEERING_LIMIT 0.90f
#define STEERING_PRESS_RATE 0.04f
#define STEERING_RETURN_RATE 0.10f

int input_frame_counter = 0;

void port_handle_input(void) {
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    switch1 = UINT32_MAX;
    switch2 = UINT32_MAX;
    switch3 = UINT32_MAX;

    if (keyboard[SDL_SCANCODE_RETURN] || keyboard[SDL_SCANCODE_KP_ENTER]) {
        switch1 &= ~SW_START;
    }
    if (getenv("CRUSN_AUTO_START") != NULL && input_frame_counter % 200 == 0) {
        switch1 &= ~SW_START;
    }
    int new_steering_direction = (keyboard[SDL_SCANCODE_RIGHT] != 0) - (keyboard[SDL_SCANCODE_LEFT] != 0);
    if (new_steering_direction != 0 && new_steering_direction != steering_direction) {
        steering_detent = new_steering_direction;
    }
    steering_direction = new_steering_direction;
    accelerator = 255; // (keyboard[SDL_SCANCODE_UP] || keyboard[SDL_SCANCODE_W]) ? 255 : 0;
    brake = (keyboard[SDL_SCANCODE_DOWN] || keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_SPACE]) ? 255 : 0;
    input_frame_counter++;
}

u32 port_get_dipswitches(void) {
    // Match MAME's default cabinet configuration: linking off and this
    // cabinet designated as master (CMDP_MASTER is active-low).
    return DIP_COMMP << 16;
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
    float target;

    /* Called from READIO at the emulated A/D sampling boundary. */
    if (steering_direction != 0) {
        /* A key applies force to a spring-loaded wheel. Approach a capped
           target quickly near center and progressively more slowly near the
           end stop, retaining fractional movement between A/D samples. */
        target = PORT_STEERING_CENTER +
            steering_direction * STEERING_HALF_RANGE * STEERING_LIMIT;
        steering += (target - steering) * STEERING_PRESS_RATE;
    } else {
        steering += (PORT_STEERING_CENTER - steering) * STEERING_RETURN_RATE;
    }
    if (steering < 0)
        steering = 0;
    if (steering > 255)
        steering = 255;
}

int port_get_steering(void) {
    return (int)(steering + 0.5f);
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

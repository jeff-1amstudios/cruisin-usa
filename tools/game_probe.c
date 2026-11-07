/* Deterministic native/Wasm core smoke test. Host services are inert; this
   probes translated execution and framebuffer parity without SDL or a browser. */
#include "machine.h"
#include "input.h"
#include "audio.h"
#include "cusa.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

extern void MAINLOOP(void);
extern int _MODE;
int input_frame_counter;
void port_handle_input(void) {}
void port_sample_steering(void) {}
int port_take_steering_detent(void) { return 0; }
void port_set_steering(int value) { (void)value; }
u32 port_get_dipswitches(void) { return 0; }
u32 port_get_commpal(void) { return 0; }
u32 port_get_switch1(void) { return UINT32_MAX; }
u32 port_get_switch2(void) { return UINT32_MAX; }
u32 port_get_switch3(void) { return UINT32_MAX; }
int port_get_steering(void) { return PORT_STEERING_CENTER; }
int port_get_accelerator(void) { return 0; }
int port_get_brake(void) { return 0; }
void portable_audio_send_command(int code) { (void)code; }
void crusn_yield_display_interrupt(void) {}
int crusn_should_skip_boot_screen(void) { return 1; }
void crusn_measure_attract_start(void) {}
int crusn_measure_attract_end(void) { return 0; }

int main(int argc, char** argv) {
    crusn_machine machine;
    static u32 pixels[CRUSN_SCREEN_WIDTH * CRUSN_SCREEN_HEIGHT];
    int ticks = argc > 1 ? atoi(argv[1]) : 285;
    if (crusn_machine_init(&machine) != 0) return 1;
    _c_int00();
    for (int tick = 1; tick <= ticks; ++tick) {
        INT0();
        if (INFRAMES >= FRAMRATE + 1 && CLEARRDY == 0) MAINLOOP();
        if (tick % TIKS_PER_SECOND == 0) {
            crusn_machine_decode_screen_argb8888(&machine, pixels, CRUSN_SCREEN_WIDTH * sizeof(*pixels));
            u32 hash = 2166136261u;
            for (size_t index = 0; index < sizeof(pixels) / sizeof(*pixels); ++index) {
                hash = (hash ^ pixels[index]) * 16777619u;
            }
            printf("tick=%d mode=%x screen=%08x\n", tick, _MODE, hash);
        }
    }
    crusn_machine_shutdown(&machine);
    return 0;
}

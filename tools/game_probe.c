/* Deterministic native/Wasm core smoke test. Host services are inert; this
   probes translated execution and framebuffer parity without SDL or a browser. */
#include "machine.h"
#include "input.h"
#include "audio.h"
#include "output.h"
#include "cusa.h"
#include "globals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Exercise both hardware pages and the added margins before starting the game. */
static void check_render_pages(crusn_machine* machine) {
    const int width = crusn_render_width(machine);
    const int offset = crusn_render_x_offset(machine);
    u32* first = crusn_machine_render_page(machine, 0);
    u32* second = crusn_machine_render_page(machine, 1);
    const word_addr_t base = machine->memory.screen.base;
    SCREEN_FILL(base, 5, CRUSN_SCREEN_WORDS - 1);
    for (size_t i = 0; i < (size_t)width * CRUSN_SCREEN_HEIGHT; ++i) assert(first[i] == 5);
    assert(second[width * 100 + offset] == 0);
    SCREEN_FILL(base, 6, 0);
    assert(first[offset] == 6);
    if (offset != 0) assert(first[0] == 5);
    crusn_mem_wr32(base + CRUSN_SCREEN_WORDS + 100 * CRUSN_SCREEN_WIDTH + 10, 7);
    assert(second[width * 100 + offset + 10] == 7);
    assert(first[width * 100 + offset + 10] == 5);
    crusn_machine_set_screen_pages(machine, 0, 1);
    port_output_fpga(-offset, 0, CRUSN_SCREEN_WIDTH + offset - 1, 0,
        CRUSN_SCREEN_WIDTH + offset - 1, 399, -offset, 399,
        0, 0, 0, 0, 0, 0, 9);
    assert(second[width * 100] == 9);
    assert(second[width * 100 + width - 1] == 9);
    assert(first[width * 100] == 5);
    crusn_machine_set_screen_pages(machine, 1, 0);
    machine->colorram_words[9] = 0x7c00;
    u32* decoded = calloc((size_t)width * CRUSN_SCREEN_HEIGHT, sizeof(u32));
    assert(decoded != NULL);
    crusn_machine_decode_screen_argb8888(machine, decoded, width * sizeof(u32));
    assert(decoded[width * 100] == 0xffff0000u);
    assert(decoded[width * 100 + width - 1] == 0xffff0000u);
    free(decoded);
    crusn_machine_clear_screen_page(machine, 1);
    for (size_t i = 0; i < (size_t)width * CRUSN_SCREEN_HEIGHT; ++i) assert(second[i] == 0);
    puts("Render page, fill, pixel-write, rasterization, and decode checks passed");
}

int main(int argc, char** argv) {
    crusn_machine machine;
    const char* dump_path = NULL;
    int check_render = 0;
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "--widescreen") == 0) crusn_set_widescreen(1);
        else if (strcmp(argv[i], "--check-render") == 0) check_render = 1;
        else if (strcmp(argv[i], "--dump") == 0 && i + 1 < argc) dump_path = argv[++i];
        else return 2;
    }
    int ticks = argc > 1 ? atoi(argv[1]) : 285;
    if (crusn_machine_init(&machine) != 0) return 1;
    if (check_render) {
        check_render_pages(&machine);
        crusn_machine_shutdown(&machine);
        return 0;
    }
    const int width = crusn_render_width(&machine);
    const size_t pixel_count = (size_t)width * CRUSN_SCREEN_HEIGHT;
    u32* pixels = calloc(pixel_count, sizeof(*pixels));
    if (pixels == NULL) { crusn_machine_shutdown(&machine); return 1; }
    _c_int00();
    for (int tick = 1; tick <= ticks; ++tick) {
        INT0();
        if (INFRAMES >= FRAMRATE + 1 && CLEARRDY == 0) MAINLOOP();
        if (tick % TIKS_PER_SECOND == 0) {
            crusn_machine_decode_screen_argb8888(&machine, pixels, width * sizeof(*pixels));
            u32 hash = 2166136261u;
            for (size_t index = 0; index < pixel_count; ++index) {
                hash = (hash ^ pixels[index]) * 16777619u;
            }
            printf("tick=%d mode=%x screen=%08x\n", tick, _MODE, hash);
        }
    }
    if (dump_path != NULL && crusn_machine_dump_screen_bmp(&machine,
            crusn_machine_screen_page(&machine, machine.display_page_index),
            machine.colorram_words, dump_path) != 0) {
        free(pixels); crusn_machine_shutdown(&machine); return 1;
    }
    free(pixels);
    crusn_machine_shutdown(&machine);
    return 0;
}

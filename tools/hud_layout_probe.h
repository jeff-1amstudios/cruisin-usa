/* Render the real HUD at both widths against a blank background. Compare its
   left, center, and right groups, including deferred text and shadows. */
#include "hud.h"
#include "text.h"
#include "cmos.h"
#include "sys.h"
#include "pall.h"

static void render_hud_layout(crusn_machine* machine, int width) {
    machine->render_width = width;
    crusn_machine_set_screen_pages(machine, 0, 0);
    crusn_machine_clear_screen_page(machine, 0);
    while (PALXFER_ACTIVE != NULL) PAL_XFER();
    TEXT_INIT();
    HUD();
    TEXT_OUTPUT();
    while (PALXFER_ACTIVE != NULL) PAL_XFER();
}

static void check_hud_layout(crusn_machine* machine) {
    const int original_width = machine->render_width;
    const size_t old_count = CRUSN_SCREEN_WIDTH * CRUSN_SCREEN_HEIGHT;
    const size_t wide_count = CRUSN_WIDE_SCREEN_WIDTH * CRUSN_SCREEN_HEIGHT;
    u32* reference = calloc(old_count, sizeof(u32));
    u32* expected = calloc(wide_count, sizeof(u32));
    int cases = 0;
    assert(reference != NULL && expected != NULL && machine->wide_screen_words != NULL);
    assert(PLYCAR != NULL && PLYCBLK != NULL);
    OFFROAD_TMR = 0;
    STOPWATCH = 24 * 55;
    _countdown = 51;
    _MPH = 128;
    PLYCBLK->gear = 4;
    PLYCBLK->rpm_x100 = C3X_STF_INT(34);
    for (int mode = 0; mode < 2; ++mode) {
        _MODE = (mode ? MBONUS : MGAME) | MHUD;
        for (int transmission = 0; transmission < 2; ++transmission) {
            CHOSEN_TRANSMISSION = transmission;
            for (int units = 0; units < 2; ++units) {
                ADJUSTMENT_WRITE(ADJ_MPHORKPM, units);
                for (int position = 1; position <= 10; ++position) {
                    POSITION = position;
                    for (int animation = 0; animation < 3; ++animation) {
                        MOVEIN_OFFSET = animation == 0 ? 0 : animation == 1 ? 30 : 150;
                        render_hud_layout(machine, CRUSN_SCREEN_WIDTH);
                        memcpy(reference, crusn_machine_render_page(machine, 0), old_count * sizeof(u32));
                        memset(expected, 0, wide_count * sizeof(u32));
                        for (int y = 0; y < CRUSN_SCREEN_HEIGHT; ++y) {
                            for (int x = 0; x < CRUSN_SCREEN_WIDTH; ++x) {
                                /* HUD groups occupy disjoint original bands. */
                                const int destination = x < 128 ? x : x >= 384 ? x + 200 : x + 100;
                                expected[y * CRUSN_WIDE_SCREEN_WIDTH + destination] = reference[y * CRUSN_SCREEN_WIDTH + x];
                            }
                        }
                        render_hud_layout(machine, CRUSN_WIDE_SCREEN_WIDTH);
                        const u32* actual = crusn_machine_render_page(machine, 0);
                        for (size_t i = 0; i < wide_count; ++i) {
                            if (actual[i] != expected[i]) {
                                fprintf(stderr, "HUD mismatch: mode=%d transmission=%d units=%d position=%d slide=%d pixel=(%zu,%zu) expected=%x actual=%x\n",
                                    mode, transmission, units, position, MOVEIN_OFFSET,
                                    i % CRUSN_WIDE_SCREEN_WIDTH, i / CRUSN_WIDE_SCREEN_WIDTH, expected[i], actual[i]);
                                exit(1);
                            }
                        }
                        ++cases;
                    }
                }
            }
        }
    }
    /* Leave a representative race HUD for --dump. */
    _MODE = MGAME | MHUD;
    POSITION = 6;
    CHOSEN_TRANSMISSION = AUTO_TRANSMISSION;
    ADJUSTMENT_WRITE(ADJ_MPHORKPM, 0);
    MOVEIN_OFFSET = 0;
    render_hud_layout(machine, original_width);
    free(reference);
    free(expected);
    printf("HUD layout checks passed: %d race/finish, transmission, unit, position, and slide combinations\n", cases);
}

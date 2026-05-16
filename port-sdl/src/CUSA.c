#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "FONT1A.h"
#include "COMP.h"
#include "NOT_IMPLEMENTED.h"
#include "TRACE.h"
#include "STUB.h"
#include "GAME_STATE.h"
#include "WAVE.h"
#include "PROCESS.h"
#include "INTRO.h"
#include "SND.h"
#include "PLYR.h"
#include "DIAG.h"
#include "COIN.h"
#include "COMM.h"
#include "COMMQ.h"
#include "CMOS.h"
#include "COMMDRNE.h"
#include "DIRQ.h"
#include "CUSA.h"
#include "TEXT.h"
#include "OBJ.h"
#include "INFIN.h"
#include "HUD.h"
#include "MOTION.h"
#include "UTIL.h"
#include "MEMTEST.h"
#include "COLLA.h"

static const char *startup0 = "          CRUISN USA (TM)     HEAD 2 HEAD";
static const char *startup1 = "  (C) 1994 NINTENDO, DEVELOPED BY TV GAMES, INC.";
static const char *startup2 = "MANUFACTURED AND SOLD BY MIDWAY MANUFACTURING COMPANY";
static const char *startup3 = "                  UNDER LICENSE.";
static const char *dlts = "DOWNLOADING TEXTURES";
static const char *version_stamp = "VERSION  4.4";
static const char *internal_vers = "I440";
static const char *date_stamp = "1994-00-00";

static const char *M1 = "LOADING OS-WMS...";
static const char *M2 = "I-NODES ALLOCATED,  DCS DECODED (14-1715)";
static const char *M3 = "WMS SATELLITE COMM, CHANNEL 3 ACTIVE LYBIRP";
static const char *M4 = "USR/ELP/CUSA>NETLINK WMS 14.32.86.1 -K CIRE";
static const char *M5 = "CONNECTING TO HOST...";
static const char *M6 = "CONNECTION ESTABLISHED, PUBLIC-KEY: CIRE";
static const char *M7 = "USR/ELP/CUSA>FTP GET CUSA";
static const char *M8 = "37940813 BYTES RECEIVED OK";
static const char *M9 = "USR/ELP/CUSA>TV30 CUSA /L";

static uint32_t palette_rgba[16];
static uint32_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

static SDL_Window *g_window;
static SDL_Renderer *g_renderer;
static SDL_Texture *g_texture;
static void _debug(void);
static bool g_running = true;
static uint32_t g_startup_ticks;
static bool g_injected_start_proc;
static int g_injected_start_frames;





enum {
    SW_START = 0x00000004,
};

uint32_t _MODE = MDIAG;
uint32_t START_HIT;
static uint32_t SWITCHBUTS;
static uint32_t SWRAM[3]; /* N-1, N-2, edge */
static uint32_t DIAG_ACTIVE;
int _ATTR_MODE = -1;
static bool g_key_start_down;
uint32_t g_frame_counter;
static uint32_t CLEARRDY;
static uint32_t NOAERASE;

static inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b) {
    TRACE();
    return 0xFF000000u | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

static void PLATFORM_INIT_BOOT_PALETTE(void) {
    TRACE();
    for (int i = 0; i < 16; i++) {
        palette_rgba[i] = rgba(0, 0, 0);
    }
    palette_rgba[0] = rgba(0, 0, 0);
    palette_rgba[1] = rgba(255, 255, 255);
    palette_rgba[3] = rgba(192, 0, 0);
    palette_rgba[11] = rgba(160, 160, 160);
}

void PLATFORM_CLEAR_SCREEN(void) {
    TRACE();
    memset(framebuffer, 0, sizeof(framebuffer));
}

static void PLATFORM_POLL_EVENTS(void) {
    TRACE();
    const uint32_t now = SDL_GetTicks();
    if (!g_injected_start_proc && (now - g_startup_ticks) >= 2000u) {
        /* Platform test hook: synthesize START key press once after boot. */
        g_key_start_down = true;
        g_injected_start_frames = 3;
        g_injected_start_proc = true;
    } else if (g_injected_start_frames > 0) {
        g_injected_start_frames--;
        if (g_injected_start_frames == 0) {
            g_key_start_down = false;
        }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            g_running = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            g_running = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            g_key_start_down = true;
        }
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
            g_key_start_down = false;
        }
    }
}

static void PRESENT_FRAME(void) {
    TRACE();
    SDL_UpdateTexture(g_texture, NULL, framebuffer, SCREEN_WIDTH * (int)sizeof(uint32_t));
    SDL_RenderClear(g_renderer);
    SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);
    SDL_RenderPresent(g_renderer);
}


/* Platform-only visualization shim (non-original). Remove once DIRQ/TEXT_OUTPUT are fully translated. */
void PLATFORM_RENDER_MODE_SCREEN(void) {
    TRACE();
    /* Platform boundary only: no placeholder text rendering. */
}


static void _debug(void) { TRACE(); STUB(); }


static ProcFn g_swtab[32] = {
    COIN1, COIN2, _start, NULL,
    NULL, NULL, SERV_COIN, COIN3,
    NULL, NULL, NULL, NULL,
    NULL, NULL, COIN4, NULL,
    NULL, RADIO_BUT, NULL, _debug,
    _VIEW0, _VIEW1, _VIEW2, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL
};



static void MESSAGE1(void) {
    TRACE();
    CLRSCRN();
    _outtextxyc(startup0, 1, 40, 1);
    _outtextxyc(startup1, 1, 70, 1);
    _outtextxyc(startup2, 1, 80, 1);
    _outtextxyc(startup3, 1, 90, 1);
    _outtextxyc(dlts, 40, 110, 11);
    _outtextxyc(version_stamp, 40, 130, 1);
    _outtextxyc(internal_vers, 260, 150, 1);
    _outtextxyc(date_stamp, 40, 150, 1);
    _outtextxyc("LINK MASTER MACHINE", 40, 160, 1);
    _outtextxyc("U38 LINK PAL INSTALLED", 40, 170, 1);
    _outtextxyc(M1, 1, 190, 11);
}

static void MSG1(void) {
    TRACE();
    _outtextxyc(M2, 1, 200, 11);
    _outtextxyc(M3, 1, 220, 11);
    _outtextxyc(M4, 1, 230, 11);
    _outtextxyc(M5, 1, 240, 11);
}

static void MSG2(void) {
    TRACE();
    _outtextxyc(M6, 1, 260, 11);
    _outtextxyc(M7, 1, 270, 11);
}

static void MSG3(void) {
    TRACE();
    _outtextxyc(M8, 1, 280, 11);
    _outtextxyc(M9, 1, 290, 11);
}

static void READIO(void) {
    TRACE();

    /* Platform substitution: synthesize only START switch from SDL key state. */
    uint32_t curr_closed = 0;
    if (g_key_start_down) {
        curr_closed |= SW_START;
    }

    SWITCHBUTS = curr_closed;

    /* ASM semantics: edge when previous two samples are open and current is closed. */
    const uint32_t prev_n1_closed = SWRAM[0];
    const uint32_t prev_n2_closed = SWRAM[1];
    const uint32_t prev_n1_open = ~prev_n1_closed;
    const uint32_t prev_n2_open = ~prev_n2_closed;

    uint32_t edges = (prev_n1_open & prev_n2_open & curr_closed);
    SWRAM[2] |= edges;

    SWRAM[1] = SWRAM[0];
    SWRAM[0] = curr_closed;
}
static void CHECKDIAG(void) {
    TRACE();
    if (DIAG_ACTIVE != 0) {
        ENTER_DIAG();
    }
}
static void FIFO_RESET(void) {
    TRACE();
    /* Platform substitution: no hardware FIFO. */
}
static void TIMER_RESET(void) {
    TRACE();
    g_frame_counter = 0;
}
static void TIMERESET(void) {
    TRACE();
    g_frame_counter = 0;
}
static void CRT_REG_SETUP(void) {
    TRACE();
    /* Platform substitution: original programs CRT timing registers and display interrupt line. */
    if (g_renderer) {
        SDL_RenderSetLogicalSize(g_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}
static void VERIFY_CODE_INTEGRITY(void) {
    TRACE();
    /* Platform substitution: code integrity check disabled in current milestone. */
}
static void CLR_PBSS(void) {
    TRACE();
    /* Protected BSS clear: represented by resetting persistent startup state. */
    _ATTR_MODE = -1;
}
void CLR_RAM(void) {
    TRACE();
    memset(SWRAM, 0, sizeof(SWRAM));
    SWITCHBUTS = 0;
    DIAG_ACTIVE = 0;
    g_key_start_down = false;
    g_frame_counter = 0;
    PRC_INIT();
}
static void FEED_WATCHDOG(void) {
    TRACE();
    /* Platform substitution: no watchdog in SDL host process. */
}
static void ENABLEGIE(void) {
    TRACE();
    /* Platform substitution: interrupt-enable return on DSP has no host equivalent. */
}

static void SWDISP(void) {
    TRACE();
    uint32_t r3 = SWRAM[2];
    r3 = (r3 << 4) >> 4; /* mask garbage high nibble, as asm does */
    if (r3 == 0) {
        return;
    }

    SWRAM[2] = 0;

    while (r3 != 0) {
        int bit = 0;
        while (((r3 & 1u) == 0u) && bit < 31) {
            bit++;
            r3 >>= 1;
        }

        ProcFn fn = g_swtab[bit & 31];
        if (fn) {
            PRC_CREATE(fn);
        }

        if (r3 != 0) {
            r3 >>= 1;
        }
    }
}


void COLD_ENTER(void) {
    TRACE();
    /* Platform substitution: emulate clear-ready handshake without interrupt-driven video clear. */
    FIFO_RESET();
    CLEARRDY = 1;
    NOAERASE = 0;
    CLEARRDY = 0;

    COMMQ_READY_TO_SEND();
    COMM_ROUTINE();
    COMMQ_PACKET_INIT();
    DECODE_BUFFER();
    PRC_DISPATCH();
    COMMQ_READY_TO_SEND();
    /* ENTER2 label block (inlined from CUSA.ASM). */
    COMM_ROUTINE();
    COMMQ_PACKET_INIT();
    DECODE_BUFFER();

    INFINITY();
    COMM_DRONE_PTR_SORT();
    DIRQ();

    HUD();
    INSERT_COINS();
    VOLUME_DISPLAY();

    LEVEL_THE_MOTION();
    CHECKDIAG();
    SWDISP();
    COLSCC();

    PRC_DISPATCH();
    COMMQ_READY_TO_SEND();
    TEXT_OUTPUT();
}

static void MAINLOOP(void) {
    TRACE();
    while (g_running) {
        PLATFORM_POLL_EVENTS();
        if (!g_running) {
            break;
        }

        READIO();
        FIFO_RESET();
        DECOMPRESS_PROC();
        ZSORTWT();

        /* ENTER2 label block (inlined from CUSA.ASM). */
        COMM_ROUTINE();
        COMMQ_PACKET_INIT();
        DECODE_BUFFER();

        INFINITY();
        COMM_DRONE_PTR_SORT();
        DIRQ();

        HUD();
        INSERT_COINS();
        VOLUME_DISPLAY();

        LEVEL_THE_MOTION();
        CHECKDIAG();
        SWDISP();
        COLSCC();

        PRC_DISPATCH();
        COMMQ_READY_TO_SEND();
        TEXT_OUTPUT();
        PRESENT_FRAME();
    }
}

static void C_INT00(void) {
    TRACE();
    CRT_REG_SETUP();
    TEST_STATIC_CHIPS();
    TEST_CHIPS();
    VERIFY_CODE_INTEGRITY();
    CLR_PBSS();
    CLR_RAM();
    FEED_WATCHDOG();

    MESSAGE1();
    PRESENT_FRAME();

    MSG1();
    PRESENT_FRAME();

    LOAD_FIXED_PALETTES();
    INIT_SYSTEM();
    READIO();
    CHECKDIAG();

    FIFO_RESET();
    COMM_INIT();
    TIMER_RESET();
    RESET_SNDBRD();
    FIFO_RESET();
    ENABLEGIE();
    COMM_ENABLE_INT2();
    FEED_WATCHDOG();

    INITIALIZATION_MOTION_CHECK();

    static uint8_t SECshared_dst[1];
    static const SectionControl _SECshared = {"_SECshared", NULL, SECshared_dst, 0, sizeof(SECshared_dst)};
    LOAD_SECTION_REQ(&_SECshared);
    static uint8_t SECskys_dst[1];
    static const SectionControl _SECskys = {"_SECskys", NULL, SECskys_dst, 0, sizeof(SECskys_dst)};
    LOAD_SECTION_REQ(&_SECskys);

    MSG2();
    PRESENT_FRAME();

    static uint8_t SECgeneral_dst[1];
    static const SectionControl _SECgeneral = {"_SECgeneral", NULL, SECgeneral_dst, 0, sizeof(SECgeneral_dst)};
    LOAD_SECTION_REQ(&_SECgeneral);
    static uint8_t SEChead2head_dst[1];
    static const SectionControl _SEChead2head = {"_SEChead2head", NULL, SEChead2head_dst, 0, sizeof(SEChead2head_dst)};
    LOAD_SECTION_REQ(&_SEChead2head);

    MSG3();
    PRESENT_FRAME();

    static uint8_t SECpress_dst[1];
    static const SectionControl _SECpress = {"_SECpress", NULL, SECpress_dst, 0, sizeof(SECpress_dst)};
    LOAD_SECTION_REQ(&_SECpress);
    AUDIT_WRITE();
    SET_MASTER_VOL();
    _ATTR_MODE = -1;
    WAVE();

    FIFO_RESET();

    VALIDATE_CMOS();
    INIT_HSTD_TABLES();

    TIMERESET();
    COMMQ_PACKET_INIT();
}

static void _c_int00(void) { TRACE(); NOT_IMPLEMENTED(); }
static void DIAG_RETURN(void) { TRACE(); NOT_IMPLEMENTED(); }
static void INT0(void) { TRACE(); NOT_IMPLEMENTED(); }
static void VOL_MINUS(void) { TRACE(); NOT_IMPLEMENTED(); }
static void VOL_PLUS(void) { TRACE(); NOT_IMPLEMENTED(); }
static void DIAG_TOGGLE(void) { TRACE(); NOT_IMPLEMENTED(); }
static void ATODINT(void) { TRACE(); NOT_IMPLEMENTED(); }
static void CLEAR_ONCHIPRAM(void) { TRACE(); NOT_IMPLEMENTED(); }
static void BUTTONS(void) { TRACE(); NOT_IMPLEMENTED(); }
static void DIAG_BUTTONS(void) { TRACE(); NOT_IMPLEMENTED(); }
static void WAIT_FOR_VBLANK(void) { TRACE(); NOT_IMPLEMENTED(); }
static void ERROR_TRAP(void) { TRACE(); NOT_IMPLEMENTED(); }
static void TIMER_READ(void) { TRACE(); NOT_IMPLEMENTED(); }
static void TIMEREC(void) { TRACE(); NOT_IMPLEMENTED(); }
static void CHECK_STATE(void) { TRACE(); NOT_IMPLEMENTED(); }
static void ABORT_STATE(void) { TRACE(); NOT_IMPLEMENTED(); }
static void DASHLIGHT(void) { TRACE(); NOT_IMPLEMENTED(); }
static void CMOS_ERROR(void) { TRACE(); NOT_IMPLEMENTED(); }
static void VERSION_UPDATE(void) { TRACE(); NOT_IMPLEMENTED(); }
static void FAKEDIAG(void) { TRACE(); NOT_IMPLEMENTED(); }
static void FEED_WATCHDOG_HARD(void) { TRACE(); NOT_IMPLEMENTED(); }

int main(void) {
    TRACE();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    g_startup_ticks = SDL_GetTicks();

    g_window = SDL_CreateWindow(
        "Cruis'n USA Boot Text (SDL2)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH * 2,
        SCREEN_HEIGHT * 2,
        SDL_WINDOW_SHOWN
    );
    if (!g_window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return 1;
    }

    g_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!g_texture) {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return 1;
    }

    PLATFORM_INIT_BOOT_PALETTE();
    /* COMP.ASM has no COMP_Init symbol; current decompressor queue starts zero-initialized. */
    FONT1A_set_palette(palette_rgba, 16);
    FONT1A_set_framebuffer(framebuffer, SCREEN_WIDTH, SCREEN_HEIGHT);

    C_INT00();
    MAINLOOP();

    SDL_DestroyTexture(g_texture);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
    return 0;
}

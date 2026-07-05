#include "cusa.h"
#include "../core/input.h"

#include "../core/machine.h"
#include "../core/port.h"
#include "../core/validator.h"
#include "bssstart.h"
#include "c30.h"
#include "checksum.h"
#include "cmos.h"
#include "comm.h"
#include "dirq.h"
#include "error.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "objects.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/CUSA.ASM
 */

void _c_int00(void);
void COLD_ENTER(void);
void ENABLEGIE(void);
void INT0(void);
static void READIO(void);
static void VOL_MINUS(void);
static void VOL_PLUS(void);
static void DIAG_TOGGLE(void);
static void ATODINT(void);
static void SWDISP(void);
void CHECKDIAG(void);
static void CLR_PBSS(void);
void CLR_RAM(void);
static void CLEAR_ONCHIPRAM(void);
static void BUTTONS(void);
static void DIAG_BUTTONS(void);
void WAIT_FOR_VBLANK(void);
static void CRT_REG_SETUP(void);
static void ERROR_TRAP(void);
void FIFO_RESET(void);
void TIMER_RESET(void);
void TIMER_READ(void);
static void TIMERESET(void);
static void MESSAGE1(void);
static void MSG1(void);
static void MSG2(void);
static void MSG3(void);
static void CHECK_STATE(void);
static void DASHLIGHT(void);
static void CMOS_ERROR(void);
static void VERSION_UPDATE(void);
void FAKEDIAG(void);
void FEED_WATCHDOG(void);
void FEED_WATCHDOG_HARD(void);
void VERIFY_CODE_INTEGRITY(void);
static float TIMEREC(void);

#define SWRAMI SWRAM
#define SWTABI SWTAB
#define TIMERAMI TIMERAM
#define STATE_TABLEI STATE_TABLE
#define BUTTONI BUTTII
#define BSSSTARTI BSSSTART

void HIDDEN_DIAG(void);
void SET_CONTROLS(void);
void FFRSUB(void);
extern int DIAGPAL[];

static void* SWTAB[32];
static int CRT_REG_SETUP_STR[12];
static const char LINKDISABLED[];
static const char IAMMASTER[];
static const char IAMSLAVE[];
static const char TPALI[];
static const char TPALNI[];

#define FLOAT_TIK 0.000292397f

/*
 *----------------------------------------------------------------------------
 *			CRUIS'N USA
 *
 *
 *	EUGENE JARVIS			DIRECTOR
 *
 *	ERIC PRIBYL			SOFTWARE
 *
 *	MARK LOFFREDO			HARDWARE
 *	GLENN SHIPP			HARDWARE
 *
 *	TED BARBER			GRAPHICS
 *	SHAWN COOPER			GRAPHICS
 *
 *	MATT BOOTY			SOUND FX
 *	VINCE PONTARELLI		MUSIC
 *
 *
 *	COPYRIGHT (C) 1994 NINTENDO, DEVELOPED BY TV GAMES, INC.
 *	MANUFACTURED AND SOLD BY MIDWAY MANUFACTURING COMPANY UNDER LICENSE
 *
 *
 */

#define MEMTESTS 1
#define TIKS_PER_SECOND 57
/* asm: COMMINTM	fbss	COMMINTM,1 */
int COMMINTM;
// 				;INT2_M  if linked

#if 0
static const char startup0[] = "DO NOT EVEN CONSIDER RELEASING THESE ROMS";
static const char startup2[] = "   THIS VERSION HAS A FATAL BUG";
static const char dlts[] = "TEST VERSION";
const char VERSION_STAMP[] = "VERSION NOT FOR RELEASE";
#else
static const char startup0[] = "          CRUISN USA (TM)     HEAD 2 HEAD";
static const char startup1[] = "  (C) 1994 NINTENDO, DEVELOPED BY TV GAMES, INC.";
static const char startup2[] = "MANUFACTURED AND SOLD BY MIDWAY MANUFACTURING COMPANY";
static const char startup3[] = "                  UNDER LICENSE.";
static const char dlts[] = "DOWNLOADING TEXTURES";
/*
;Release of linked version will be 4.0
;	I = I4000
;	VID = 40
;
*/
const char VERSION_STAMP[] = "VERSION  4.5";
#endif
static const char INTERNAL_VERS[] = "I450";
#define VERSION_ID 40
/*
        ;Because of managements inability to deal with software projects,
        ;or being able to keep the version control straight
        ;we have now moved into Internal and External versions of the program
        ;VERSION_STAMP contains whatever the management wants to call it.
        ;If management asks call it 'version' or 'release'.
        ;'actual version' or 'internal version' or 'internal' is what
        ;to use for us.
        ;INTERNAL_VERS is the actual version of the software.
        ;Keep in mind that multiple releases exist under the external
        ;version name, and the true software version can be gotten from
        ;the title page.
*/

static const char M1[] = "LOADING OS-WMS...";
static const char M2[] = "I-NODES ALLOCATED,  DCS DECODED (14-1715)";
static const char M3[] = "WMS SATELLITE COMM, CHANNEL 3 ACTIVE LYBIRP";
static const char M4[] = "USR/ELP/CUSA>NETLINK WMS 14.32.86.1 -K CIRE";
static const char M5[] = "CONNECTING TO HOST...";
static const char M6[] = "CONNECTION ESTABLISHED, PUBLIC-KEY: CIRE";
static const char M7[] = "USR/ELP/CUSA>FTP GET CUSA";
static const char M8[] = "37940813 BYTES RECEIVED OK";
static const char M9[] = "USR/ELP/CUSA>TV30 CUSA /L";
// 	;*** RAM DEFINITIONS
/* asm: _MODE	pbss	_MODE,1 */
int _MODE;
/* asm: STATE	pbss	STATE,1 */
static int STATE;
/* asm: NFRAMES	.bss	NFRAMES,1 */
int NFRAMES;
/* asm: INFRAMES	.bss	INFRAMES,1 */
int INFRAMES;
/* asm: FRAMRATE	.bss	FRAMRATE,1 */
int FRAMRATE;
/* asm: IFRAMES	.bss	IFRAMES,1 */
int IFRAMES;
/* asm: OFRAMES	.bss	OFRAMES,1 */
static int OFRAMES;
/* asm: ERRORN	.bss	ERRORN,1 */
int ERRORN;
/* asm: ERRORO	.bss	ERRORO,1 */
int ERRORO;
/* asm: FRAMETIME	.bss	FRAMETIME,1 */
int FRAMETIME;
/* asm: SWITCHBUTS	.bss	SWITCHBUTS,1 */
int SWITCHBUTS;
/* asm: BGNDCOLA	.bss	BGNDCOLA,1 */
int BGNDCOLA;
/* asm: DIAG_ACTIVE	.bss	DIAG_ACTIVE,1 */
int DIAG_ACTIVE;
/* asm: GAME_TIMER	.bss	GAME_TIMER,1 */
float GAME_TIMER;
/* asm: SYSCNTL	pbss	SYSCNTL,1 */
int SYSCNTL;
/* asm: _pot0	pbss	_pot0,1 */
int _pot0;
/* asm: _pot1	.bss	_pot1,1 */
int _pot1;
/* asm: _pot2	.bss	_pot2,1 */
int _pot2;
/* asm: RDPOT	.bss	RDPOT,1 */
int RDPOT;
/* asm: _sectime	.bss	_sectime,1 */
int _sectime;
/* asm: CLEARRDY	.bss	CLEARRDY,1 */
int CLEARRDY;
/* asm: NOAERASE	.bss	NOAERASE,1 */
int NOAERASE;
/* asm: NOSWAP	.bss	NOSWAP,1 */
int NOSWAP;
/* asm: DISPLAY_PAGE	.bss	DISPLAY_PAGE,1 */
int DISPLAY_PAGE;
/* asm: MPROC_TIK	fbss	MPROC_TIK,1 */
int MPROC_TIK;
/* asm: WDHIT	fbss	WDHIT,1 */
int WDHIT;

// *----------------------------------------------------------------------------
void _c_int00(void) {
    int i;

#if 0 // jeff ignore for now
    // asm 00004AE6: 	LDI	02h,IOF			;TV30 KLUDGE
    // asm 00004AE7: 	CLRI	R0			;must fix COMM setup asap
    // asm 00004AE8: 	LDP	@COMMINTM
    // asm 00004AE9: 	STI	R0,@COMMINTM
    R0.s = 0;
    COMMINTM = R0.s;

    // asm 00004AEA: 	LDP	@CPU_WS
    // asm 00004AEB: 	LDI	1068h,R0
    // asm 00004AEC: 	STI	R0,@CPU_WS
    R0.u = 0x1068u;
    crusn_mem_wr32(CPU_WS, R0.u);

    // asm 00004AED: 	LDP	@FIFO_CONTROL
    // asm 00004AEE: 	LDI	FIFO_CONTROL_INIT,R0
    // asm 00004AEF: 	STI	R0,@FIFO_CONTROL
    R0.u = FIFO_CONTROL_INIT;
    crusn_mem_wr32(FIFO_CONTROL, R0.u);

    // asm 00004AF0: 	LDI	800h,ST			;ENABLE THE CACHE, DISABLE INTERRUPTS
    // asm 00004AF1:         LDP	@FASTSTKI
    // asm 00004AF2:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm 00004AF3: 	DINT				;DISABLE INTERUPTS
    ST = 0x800u;
    SP = FASTSTKI;

    // asm 00004AF9: 	CLRI	R0
    // asm 00004AFA: 	LDP	@CMOS_WP_WORD
    // asm 00004AFB: 	STI	R0,@CMOS_WP_WORD	;DISABLE CMOS WRITES
    R0.s = 0;
    crusn_mem_wr32(CMOS_WP_WORD, R0.u);

    // asm 00004AFC: 	LDIL	RAM0,AR0		;CLEARS CMOS_WP_WORD_SHADOW
    // asm 00004AFF: 	RPTS	2047
    // asm 00004B00: 	STI	R0,*AR0++
    for (i = 0; i < 2048; ++i) {
        crusn_mem_wr32(RAM0 + (u32)i, R0.u);
    }

    // 	;
    // 	;CHECK TO SEE IF THIS IS A WATCHDOG
    // 	;
    // 	;POWERUP IGNORE GLITCH FIX
    // asm 00004B01: 	LDP	@991050h 		;DOG TEST
    // asm 00004B02: 	LDI	@991050h,R0
    // asm 00004B03: 	LDP	@WDHIT
    // asm 00004B04: 	STI	R0,@WDHIT		;SAVE YOUR DOGGIE
    R0.u = crusn_mem_rd32(0x991050u);
    WDHIT = R0.s;

    // asm 00004B05: 	LDP	@SYSCNTLR
    // asm 00004B06: 	LDI	SYSCNTL_INIT,R0		;INIT SYSCNTL
    // asm 00004B07: 	STI	R0,@SYSCNTLR
    R0.u = SYSCNTL_INIT;
    crusn_mem_wr32(SYSCNTLR, R0.u);

    // asm 00004B08: 	LDP	@DMA_SETUP
    // asm 00004B09: 	LDI	DMA_SETUP_INIT,R0	;INIT DMA
    // asm 00004B0A: 	STI	R0,@DMA_SETUP
    R0.u = DMA_SETUP_INIT;
    crusn_mem_wr32(DMA_SETUP, R0.u);

    // asm 00004B0B: 	LDP	@FIFO_SIZE
    // asm 00004B0C: 	LDI	270,R0			;INIT FIFO SIZE
    // asm 00004B0D: 	STI	R0,@FIFO_SIZE
    R0.s = 270;
    crusn_mem_wr32(FIFO_SIZE, R0.u);

    // asm 00004B0E: 	CALL	CRT_REG_SETUP		;INIT CRT
    CRT_REG_SETUP();
    // asm 00004B0F: 	SETDP
#if MEMTESTS
    // asm 00004B10: 	CALL	TEST_STATIC_CHIPS	;TEST THE INSTALLABLE CHIPS
    TEST_STATIC_CHIPS();
#endif
#if DEBUG
    // 	;
    // 	;COPY THE 'ROM' into the 'ROM'
    // 	;
    // 	;*this only works in RAM
    // 	;and, if used in ROM will cause a lockup
    // 	;
    // asm: 	LDI	40h,AR1			;SOURCE ADDRESS
    // asm: 	LDI	0C000h,AR2		;DESINATION ADDRESS
    // asm: 	LS	8,AR2
    // asm: 	ADDI	40h,AR2
    // asm: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm: 	LS	4,RC			;FAST RAM
    // asm: 	SUBI	41h,RC
    // asm: 	RPTB	LD_DBG1
    // asm: 	LDI	*AR1++,R0
    AR1 = 0x40u;
    AR2 = 0x0C00040u;
    RC = (0x1000u << 4) - 0x41u;
    for (i = 0; i <= (int)RC; ++i) {
        R0.u = crusn_mem_rd32(AR1++);
        // asm 00004B11: STI	R0,*AR2++
        crusn_mem_wr32(AR2++, R0.u);
    }
#endif
    // asm 00004B11: 	LDI	0,AR1			;SOURCE ADDRESS
    // asm 00004B12: 	LDI	4000h,AR2		;DESINATION ADDRESS
    // asm 00004B13: 	LS	8,AR2
    // asm 00004B14: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm 00004B15: 	LS	4,RC			;FAST RAM
    // asm 00004B16: 	RPTB	LD_RAM
    // asm 00004B17: 	LDI	*AR1++,R0
    AR1 = 0;
    AR2 = 0x00400000u;
    RC = 0x1000u << 4;
    for (i = 0; i <= (int)RC; ++i) {
        R0.u = crusn_mem_rd32(AR1++);
        // asm 00004B18: STI	R0,*AR2++
        crusn_mem_wr32(AR2++, R0.u);
    }

    // asm 00004B19: 	NOP
    // asm 00004B1A: 	NOP				;DELAY FOR PIPELINE
    // asm 00004B1B: 	NOP
    // asm 00004B1C: 	LDI	22h,IOF
    // asm 00004B1D: 	NOP
    // asm 00004B1E: 	NOP				;DELAY FOR PIPELINE
    // asm 00004B1F: 	NOP
    // asm 00004B20: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm 00004B24: 	CLRI	R0
    // asm 00004B25: 	LDP	@COMMINTM
    // asm 00004B26: 	STI	R0,@COMMINTM
    R0.s = 0;
    COMMINTM = R0.s;
    // asm 00004B27: 	SETDP
#if MEMTESTS
    // asm 00004B28: 	CALL	TEST_CHIPS		;TEST THE INSTALLABLE CHIPS
    TEST_CHIPS();
#endif
    // asm 00004B29: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm 00004B2D: 	DINT
    // asm 00004B33:         LDP	@FASTSTKI
    // asm 00004B34:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    SP = FASTSTKI;
#if MEMTESTS == 0
    // 	;For RAM (development) version, we must copy the
    // 	;program into what would be the ROM
    // 	;
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	ANDN	10h,R0
    // asm: 	STI	R0,@SYSCNTL
    R0.s = SYSCNTL;
    R0.s &= ~0x10;
    SYSCNTL = R0.s;
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    crusn_mem_wr32(SYSCNTLR, R0.u);
    // asm: 	SETDP
    // asm: 	LDI	040h,AR1		;SOURCE ADDRESS
    // asm: 	LDI	0C000h,AR2		;DESINATION ADDRESS
    // asm: 	LS	8,AR2
    // asm: 	ADDI	040h,AR2
    // asm: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm: 	LS	4,RC			;FAST RAM
    // asm: 	SUBI	41h,RC
    // asm: 	RPTB	LD_RAM3
    // asm: 	LDI	*AR1++,R0
    AR1 = 0x40u;
    AR2 = 0x0C00040u;
    RC = (0x1000u << 4) - 0x41u;
    for (i = 0; i <= (int)RC; ++i) {
        R0.u = crusn_mem_rd32(AR1++);
        // asm 00004B35: STI	R0,*AR2++
        crusn_mem_wr32(AR2++, R0.u);
    }
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	OR	10h,R0
    // asm: 	STI	R0,@SYSCNTL
    R0.s = SYSCNTL;
    R0.s |= 0x10;
    SYSCNTL = R0.s;
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    crusn_mem_wr32(SYSCNTLR, R0.u);
    // asm: 	SETDP
#endif

#endif

    SETADJ(ADJ_OUTOFDIAG, 0);
    goto DR1;

DIAG_RETURN:
    WDHIT = 8; // prevent false watchdog

DR1:

#if DEBUG
    VERIFY_CODE_INTEGRITY();
#endif

    *COLOROM_ADDR(COLORAM) = 0;
    DIAG_ACTIVE = 0;

    CLR_PBSS();
    CLR_RAM();

    _MODE = MDIAG;
    SYSCNTL = SYSCNTL_INIT;

    FEED_WATCHDOG();

    MESSAGE1();
    MAME_VALIDATE_REGION_AT_ADDR(0x00004B5E, "SCREEN0", SCREEN0, g_crusn_machine->screen_words, CRUSN_SCREEN_WORDS);
    crusn_yield_display_interrupt();
    MSG1();
    crusn_yield_display_interrupt();
    MAME_VALIDATE_REGION_AT_ADDR(0x00004B5F, "SCREEN0", SCREEN0, g_crusn_machine->screen_words, CRUSN_SCREEN_WORDS);

    PREVX = 240;
    DELTA = 1;

    crusn_mem_wr32(TIMER_CNTL1, 0x200 | 1);
    crusn_mem_wr32(TIMER_PERIOD1, -1);

    // clear_timer_ram_48_floats();

    RAND = 0x5A5A5A5A;

    LOAD_FIXED_PALETTES();
    INIT_SYSTEM();

    READIO();
    CHECKDIAG();

    FIFO_RESET();
    COMM_INIT();

    TIMER_RESET();

    DINT();
    RESET_SNDBRD();
    EINT();

    FIFO_RESET();

    ENABLEGIE();

    COMM_ENABLE_INT2();

    INITIALIZATION_MOTION_CHECK();

    SETADJ(ADJ_OUTOFDIAG, 0);

    HARD_SECTION_LOAD = 1;
    LOAD_SECTION_REQ(&SECshared);
    MAME_VALIDATE_REGION_AT_ADDR(0x00004B8B, "_SECshared-decompressed", 0x0A00000, SECshared.dest_addr, 0x1AB00);

    HARD_SECTION_LOAD = 1;
    LOAD_SECTION_REQ(&SECskys_CUSA);
    MAME_VALIDATE_REGION_AT_ADDR(0x00004B8F, "SECskys_CUSA-decompressed", 0x0A1AB00, SECskys_CUSA.dest_addr, 0x30000);

    MSG2();

    HARD_SECTION_LOAD = 1;
    BOOT_PACIFY_SCREEN_P = 1;
    LOAD_SECTION_REQ(&SECgeneral_CUSA);
    MAME_VALIDATE_REGION_AT_ADDR(0x00004B95, "SECgeneral_CUSA-decompressed", 0x0A52900, SECgeneral_CUSA.dest_addr, 0x136280);

    HARD_SECTION_LOAD = 1;
    BOOT_PACIFY_SCREEN_P = 1;
    LOAD_SECTION_REQ(&SEChead2head);
    MAME_VALIDATE_REGION_AT_ADDR(0x00004B9A, "SEChead2head-decompressed", 0x0BEFA00, SEChead2head.dest_addr, 0x1000);

    MSG3();

    HARD_SECTION_LOAD = 1;
    BOOT_PACIFY_SCREEN_P = 1;
    LOAD_SECTION_REQ(&SECpress);
    MAME_VALIDATE_REGION_AT_ADDR(0x00004BA0, "SECpress-decompressed", 0x0B88B80, SECpress.dest_addr, 0x1000);

    AUDIT_WRITE(AUD_BCREDITS, 0);

    READADJ(ADJ_VOLUME);
    SET_MASTER_VOL();

    if (WDHIT & 8)
        INCAUD(AUD_NUM_WATCHDOGS);

    _newbut = 0;
    _ATTR_MODE = -1;

    WAVE(_ATTR_MODE);

    FIFO_RESET();

    // IE = INT0_M | INT3_M | COMMINTM;
    // IF = 0;
    ENABLEGIE();

    // if (READAUD(AUD_VERSION) != VERSION_ID) {
    //     VERSION_UPDATE();
    //     return;
    // }

    if (VALIDATE_CMOS()) {
        CMOS_ERROR();
        return;
    }

    if (READAUD(ADJ_ACTUALHSTDRESET) <= 0) {
        INIT_HSTD_TABLES();
        int reset_val = READAUD(ADJ_HIGHSCORE_RESET);
        SETADJ(ADJ_ACTUALHSTDRESET, reset_val);
    }

    NFRAMES = 1;
    ERRORO = 0;
    ERRORN = 0;

    TIMERESET();
    COMMQ_PACKET_INIT();
}

// *----------------------------------------------------------------------------
void MAINLOOP(void) {
    // for (;;) {
    //  FIFO_CLRP(); // IS THE FIFO CLEAR
    //  DMA_WT();
    FIFO_RESET();

    TIMEREC();
    DECOMPRESS_PROC();
    TIMEREC();

    // while (INFRAMES < FRAMRATE) {
    //     // 1 FRAME MINIMUM
    // }

    TIMEREC();
    ZSORTWT(); // WAIT FOR PAGE WHILE SORTING
    TIMEREC();

    // while (CLEARRDY != 0) {
    //     // DONE WHEN INT SIGNALS PAGE SWAPPED
    // }

    // COMMUNICATION XFER
    COMM_ROUTINE();
    COMMQ_PACKET_INIT();
    DECODE_BUFFER();

    NFRAMES = INFRAMES; // SAVE FOR ALL CURRENT PROCESSES
    INFRAMES = 0;       // CLEAR INTERRUPT COUNTER

    FRAMETIME = TIMEREC(); // SAVE THE FRAMETIME

    TIMERESET();

    // FILL FIFO STUFF
    if (_MODE & MINFIN) {
        INFINITY_CUSA();
    }

    TIMEREC();

    if (COMM_DRONE_PTR) {
        COMM_DRONE_PTR_SORT();
    }

    DIRQ(); // DISPLAY THE POLYGONS

    TIMEREC();

    // FILL FIFO STUFF
    if (_MODE & MHUD) {
        switch (_MODE & MMODE) {
        case MGAME:
        case MBONUS:
            HUD(); // HEADS UP DISPLAY (GAME MODE ONLY)
            break;
        }
    }

    TIMEREC();

    switch (_MODE & MMODE) { // THIS MASK IS IMPORTANT!!!!
    case MATTR:
        INSERT_COINS();
        break;
    }

    VOLUME_DISPLAY();

    TIMEREC();

    // MOTION CABINET LEVELING
    if ((_MODE & MMODE) != MGAME) { // IF NOT IN GAME
        LEVEL_THE_MOTION();
    }

    CHECKDIAG(); // JUMP TO DIAGNOSTICS?
    SWDISP();    // DISPATCH ANY SWITCH PROCESSES

    COLSCC();

    MPROC_TIK = 0;

    PRC_DISPATCH(); // EXECUTE PROCESSES

    COMMQ_READY_TO_SEND();

    TEXT_OUTPUT(); // END FILL FIFO STUFF

    TIMEREC();

    if ((_MODE & MMODE) != MATTR) {
        return;
    }

    // we are in attract mode, set appropriately
    if ((DIPRAM & CMDP_MASTER) == 0) {
        return;
    }

    if (OM_ATTR_MODE == OLD_OM_ATTR_MODE) {
        return;
    }

    // WE MAY ONLY SYNC ON THE MIDWAY SPIN
    if (OM_ATTR_MODE != -2) {
        return;
    }

    OM_ATTR_MODE = OM_ATTR_MODE;
    _ATTR_MODE = OM_ATTR_MODE;

    SILENT();

    WAVE(_ATTR_MODE);

    COLD_ENTER();
    // return;
    //}
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *WE HAVE A RADICAL SYSTEM CHANGE
 *WE WANT TO
 *1.	LET EXECUTE ANY PROCESSES THAT WE HAVE BEGUN
 *2.	CLEAR THE SCREEN (WHILE STILL VIEWING OLD SCREEN)
 *3.	PLOT THE NEW SCREEN
 *4.	CONTINUE WITH THE SYSTEM
 *
 */
void COLD_ENTER(void) {
    // asm 00004C2C: 	FIFO_CLRP	R0		;is the fifo clear
    // asm 00004C31: 	DMA_WT		R0
    // asm 00004C36: 	CALL	FIFO_RESET
    // asm 00004C37: 	LDI	1,R0
    // asm 00004C38: 	STI	R0,@CLEARRDY		;set clear ready to happen...
    // asm 00004C39: 	CLRI	R0
    // asm 00004C3A: 	STI	R0,@NOAERASE		;turn auto-erase ON
C_WAIT:
    // asm 00004C3B: LDI	@CLEARRDY,R0		;wait for it to clear
    // asm 00004C3C: 	BNZ	C_WAIT
    // asm 00004C3D: 	CALL	COMMQ_READY_TO_SEND
    // asm 00004C3E: 	CALL	COMM_ROUTINE
    // asm 00004C3F: 	CALL	COMMQ_PACKET_INIT
    // asm 00004C40: 	CALL	DECODE_BUFFER
    // asm 00004C41: 	CALL	PRC_DISPATCH
    // asm 00004C42: 	CALL	COMMQ_READY_TO_SEND
    // asm 00004C43: 	BR	ENTER2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLD_ENTER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INTERRUPT ROUTINE
 *
 *1.	initially mask out all interupts we dont want to occur
 *2.	enable GIE of ST by calling a routine that RETI's
 *	(allows debugger to function)
 *3.	increment number of frames passed since last screen update
 *
 *
 */
void ENABLEGIE(void) {
    // asm 00004C44: 	RETI
    UNIMPL_TODO();
}

void INT0(void) {
    int r0;
    int r1;
    int r5;
    float float_tik;

    // asm 00004C45: 	PUSH	ST
    // asm 00004C46: 	LDI	INT1_M,IE	;disable everything except TV30 interrupt & comm int
    // asm 00004C47: 	ANDN	INT0_M,IF	;we wont irq ourself
    // asm 00004C48: 	PUSH	DP
    // asm 00004C49: 	PUSH	R0
    // asm 00004C4A: 	LDP	@COMMINTM
    // asm 00004C4B: 	OR	@COMMINTM,IE
    // asm 00004C4C: 	LDP	@CPU_WS
    // asm 00004C4D: 	LDI	@CPU_WS,R0
    // asm 00004C4E: 	PUSH	R0
    // asm 00004C4F: 	LDI	SOFT_WS,R0
    // asm 00004C50: 	STI	R0,@CPU_WS
    // asm 00004C51: 	PUSH	R1
    // asm 00004C52: 	PUSH	R2
    // asm 00004C53: 	PUSH	R3
    // asm 00004C54: 	PUSH	R4
    // asm 00004C55: 	PUSH	R5
    // asm 00004C56: 	PUSH	R6
    // asm 00004C57: 	PUSH	R7
    // asm 00004C58: 	PUSHF	R0
    // asm 00004C59: 	PUSHF	R1
    // asm 00004C5A: 	PUSHF	R2
    // asm 00004C5B: 	PUSHF	R3
    // asm 00004C5C: 	PUSHF	R4
    // asm 00004C5D: 	PUSHF	R5
    // asm 00004C5E: 	PUSHF	R6
    // asm 00004C5F: 	PUSHF	R7
    // asm 00004C60: 	PUSH	RC
    // asm 00004C61: 	PUSH	RS
    // asm 00004C62: 	PUSH	RE
    // asm 00004C63: 	PUSH	BK
    // asm 00004C64: 	PUSH	IR0
    // asm 00004C65: 	PUSH	IR1
    // asm 00004C66: 	PUSH	AR0
    // asm 00004C67: 	PUSH	AR1
    // asm 00004C68: 	PUSH	AR2
    // asm 00004C69: 	PUSH	AR3
    // asm 00004C6A: 	PUSH	AR4
    // asm 00004C6B: 	PUSH	AR5
    // asm 00004C6C: 	PUSH	AR6
    // asm 00004C6D: 	PUSH	AR7
    // asm 00004C6E: 	SETDP
    // asm 00004C6F: 	CALL	PAL_XFER		;TRANSFER QUEUED PALETTES
    PAL_XFER();
    // asm 00004C70: 	LDI	@BGNDCOLA,R0		;SET BACKGROUND COLOR
    r0 = BGNDCOLA;
    // asm 00004C71: 	LDP	@COLORAM
    // asm 00004C72: 	STI	R0,@COLORAM
    crusn_mem_wr32(COLORAM, (u32)r0);
    // asm 00004C73: 	SETDP				;RESET DP
    // asm 00004C74: 	CALL	COMM_MASTER_SEND_SYNC
    COMM_MASTER_SEND_SYNC();
    // asm 00004C75: 	CALL	ENABLEGIE
    ENABLEGIE();
    // asm 00004C76: 	LDI	@_MODE,R0
    r0 = _MODE;
    // asm 00004C77: 	AND	MMODE,R0
    r0 &= MMODE;
    // asm 00004C78: 	CMPI	MDIAG,R0
    if (r0 == MDIAG) {
        goto NTEST;
    }
    // asm 00004C79: 	BEQ	NTEST
    // asm 00004C7A: 	LDP	@MPROC_TIK
    // asm 00004C7B: 	LDI	@MPROC_TIK,R0
    r0 = MPROC_TIK;
    // asm 00004C7C: 	SETDP
    // asm 00004C7D: 	INC	R0
    r0 += 1;
    // asm 00004C7E: 	CMPI	300,R0
    if (r0 <= 300) {
        goto REGIT;
    }
    // asm 00004C7F: 	BLE	REGIT
    // asm 00004C80: 	ERRON	U,EC_WATCHDOG2
    ERRON(EC_WATCHDOG2);
    // asm 00004C88: 	BU	_c_int00
    _c_int00();
    return;
REGIT:
    // asm 00004C89: LDP	@MPROC_TIK
    // asm 00004C8A: 	STI	R0,@MPROC_TIK
    MPROC_TIK = r0;
    // asm 00004C8B: 	SETDP
NTEST:
    // asm 00004C8C: 	CALL	FEED_WATCHDOG
    FEED_WATCHDOG();
    // asm 00004C8D: 	LDI	@CLEARRDY,R0
    // asm 00004C8E: 	BZ	NCLRSCR
    if (CLEARRDY != 0) {
        // asm 00004C8F: 	LDI	0,R0
        // asm 00004C90: 	STI	R0,@CLEARRDY		;RESET SCREEN CLEAR FLAG
        CLEARRDY = 0;
        // asm 00004C91: 	STI	R0,@IFRAMES	 	;RESET INTERRUPT FRAME COUNTER
        IFRAMES = 0;
        // asm 00004C92: 	LDI	@MOTION_RCV_TIKS,R0	;MOTION RECEIVE ERROR
        // asm 00004C93: 	DEC	R0
        // asm 00004C94: 	STI	R0,@MOTION_RCV_TIKS
        MOTION_RCV_TIKS -= 1;
        // asm 00004C95: 	LDI	@NOSWAP,R0		;PAGE SWAP?
        // asm 00004C96: 	BNZ	NCLRSCR
        if (NOSWAP == 0) {
            // asm 00004C97: 	LDI	@DISPLAY_PAGE,R0	;PAGE SWAP FLAG
            // asm 00004C98: 	XOR	1,R0
            // asm 00004C99: 	STI	R0,@DISPLAY_PAGE
            DISPLAY_PAGE ^= 1;
            // asm 00004C9A: 	BNZ	PAGE1
            if (DISPLAY_PAGE == 0) {
                // asm 00004C9B: 	CALL	SETPAGE1
                SETPAGE1();
                // asm 00004C9C: 	CALL	FASTCLR0
                FASTCLR0();
                // asm 00004C9D: 	B	DN_PAGE
            } else {
                // asm 00004C9E: CALL	SETPAGE0
                SETPAGE0();
                // asm 00004C9F: 	CALL	FASTCLR1
                FASTCLR1();
            }
        }
    }
DN_PAGE:
NCLRSCR:
    // asm 00004CA0: 	LDI	@STOPWATCH_CNTL,R0	;STOPWATCH TIMER
    // asm 00004CA1: 	BZ	NOSTOPWUPDT
    if (STOPWATCH_CNTL != 0) {
        // asm 00004CA2: 	INCM	@STOPWATCH
        STOPWATCH += 1;
    }
NOSTOPWUPDT:
    // asm 00004CA5: 	INCM	@_sectime		;ONE SECOND TIMER
    _sectime += 1;
    // asm 00004CA8: 	CMPI	TIKS_PER_SECOND,R0
    // asm 00004CA9: 	BLT	NOTASEC
    r0 = _sectime;
    if (_sectime >= TIKS_PER_SECOND) {
        // asm 00004CAA: 	CLRI	R0
        r0 = 0;
        // asm 00004CAB: 	STI	R0,@_sectime
        _sectime = 0;
        // asm 00004CAC: 	LDI	@_MODE,R5
        r5 = _MODE;
        // asm 00004CAD: 	AND	MMODE,R5
        r5 &= MMODE;
        // asm 00004CAE: 	CMPI	MDIAG,R5
        if (r5 == MDIAG) {
            goto JJ88;
        }
        // asm 00004CAF: 	BEQ	JJ88
        // asm 00004CB0: 	INCAUD	AUD_POWERON_TIME
        AUDIT_INC(AUD_POWERON_TIME);
        // asm 00004CB2: 	CMPI	MATTR,R5
        if (r5 == MATTR) {
            goto JJ88;
        }
        // asm 00004CB3: 	BEQ	JJ88
        // asm 00004CB4: 	INCAUD	AUD_GAMEON_TIME
        AUDIT_INC(AUD_GAMEON_TIME);
        // asm 00004CB6: 	CMPI	MGAME,R5
        if (r5 != MGAME) {
            goto JJ88;
        }
        // asm 00004CB7: 	BNE	JJ88
        // asm 00004CB8: 	LDI	@CAMVIEW,AR2
        // asm 00004CB9: 	ADDI	AUD_VIEW1_TIME,AR2
        // asm 00004CBA: 	CALL	AUDIT_INC
        AUDIT_INC(AUD_VIEW1_TIME + CAMVIEW);
    }
JJ88:
    // asm 00004CBB: 	LDI	@OFFROAD_TMR,R0	 	;OFFROAD TIMER
    // asm 00004CBC: 	SUBI	1,R0
    OFFROAD_TMR -= 1;
    // asm 00004CBD: 	LDILT	0,R0
    if (OFFROAD_TMR < 0) {
        OFFROAD_TMR = 0;
    }
    // asm 00004CBE: 	STI	R0,@OFFROAD_TMR
    // asm 00004CBF: 	LDI	@_countdown,R0		;LO DP TIME REMAINING FOR PLAYER
    // asm 00004CC0: 	BLE	NOTASEC
    if (_countdown <= 0) {
        goto NOTASEC;
    }
    // asm 00004CC1: 	LDI	@_MODE,R1		;RACE MUST BE HAPPENING TO DECREMENT COUNT
    // asm 00004CC2: 	TSTB	MGO,R1
    if ((_MODE & MGO) == 0) {
        goto NOTASEC;
    }
    // asm 00004CC3: 	BZ	NOTASEC
    // asm 00004CC4: 	SUBI	1,R0
    // asm 00004CC5: 	STI	R0,@_countdown
    _countdown -= 1;
NOTASEC:
    // asm 00004CC6: 	LDF	@FLOAT_TIK,R0

    // asm 00004CC7: 	ADDF	@GAME_TIMER,R0
    GAME_TIMER += FLOAT_TIK;
    // asm 00004CC8: 	STF	R0,@GAME_TIMER
    // asm 00004CC9: NOTINGAME
    // asm 00004CC9: 	INCM	@INFRAMES		;increment number of frames passed since last screen switch
    INFRAMES += 1;
    // asm 00004CCC: 	INCMF	@IFRAMES
    IFRAMES += 1;
    // asm 00004CCF: 	CALL	NUWHEEL			;motorized wheel
    NUWHEEL();
    // asm 00004CD0: 	CALL	SNDPROC			;sound processor
    SNDPROC();
    // asm 00004CD1: 	CALL	READIO			;read the switches
    READIO();
    // asm 00004CD2: 	CALL	RANDOM			;randomize further...
    RANDOM();
    // asm 00004CD3: 	CALL	BUTTONS			;lighted buttons routine
    BUTTONS();
    // asm 00004CD4: 	CALL	COIN_COUNTER		;coin counter routine
    COIN_COUNTER();
    // asm 00004CD5: 	CALL	CHECK_STATE
    CHECK_STATE();
    // asm 00004CD6: 	CALL	LATCH_ERROR		;motion error
    LATCH_ERROR();
    // asm 00004CD7: 	ANDN	INT0_M,IF		;set 60Hz IRQ no longer pending
    // asm 00004CD8: 	POP	AR7
    // asm 00004CD9: 	POP	AR6
    // asm 00004CDA: 	POP	AR5
    // asm 00004CDB: 	POP	AR4
    // asm 00004CDC: 	POP	AR3
    // asm 00004CDD: 	POP	AR2
    // asm 00004CDE: 	POP	AR1
    // asm 00004CDF: 	POP	AR0
    // asm 00004CE0: 	POP	IR1
    // asm 00004CE1: 	POP	IR0
    // asm 00004CE2: 	POP	BK
    // asm 00004CE3: 	POP	RE
    // asm 00004CE4: 	POP	RS
    // asm 00004CE5: 	POP	RC
    // asm 00004CE6: 	POPF	R7
    // asm 00004CE7: 	POPF	R6
    // asm 00004CE8: 	POPF	R5
    // asm 00004CE9: 	POPF	R4
    // asm 00004CEA: 	POPF	R3
    // asm 00004CEB: 	POPF	R2
    // asm 00004CEC: 	POPF	R1
    // asm 00004CED: 	POPF	R0
    // asm 00004CEE: 	POP	R7
    // asm 00004CEF: 	POP	R6
    // asm 00004CF0: 	POP	R5
    // asm 00004CF1: 	POP	R4
    // asm 00004CF2: 	POP	R3
    // asm 00004CF3: 	POP	R2
    // asm 00004CF4: 	POP	R1
    // asm 00004CF5: 	POP	R0
    // asm 00004CF6: 	LDP	@CPU_WS
    // asm 00004CF7: 	STI	R0,@CPU_WS
    // asm 00004CF8: 	POP	R0
    // asm 00004CF9: 	LDI	INT1_M|INT3_M|INT0_M,IE
    // asm 00004CFA: 	LDP	@COMMINTM
    // asm 00004CFB: 	OR	@COMMINTM,IE
    // asm 00004CFC: 	POP	DP
    // asm 00004CFD: 	POP	ST
    // asm 00004CFE: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INT0", (u32)DISPLAY_PAGE, (u32)ACTIVE_SCREEN);
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *READ IO  SWITCHES AND POTS
 *
 *
 */
/* asm: _newbut	pbss	_newbut,0 */
int _newbut;
/* asm: SWRAM	pbss	SWRAM,3 */
int SWRAM[3];
/* asm: DIPRAM	pbss	DIPRAM,1 */
int DIPRAM;

static void READIO(void) {

    DIPRAM = port_get_dipswitches() >> 16;
    // TODO
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void VOL_MINUS(void) {
    // asm 00004D39: 	PUSH	R0
    // asm 00004D3A: 	PUSH	R1
    // asm 00004D3B: 	PUSH	R2
    // asm 00004D3C: 	PUSH	R3
    // asm 00004D3D: 	PUSH	AR2
    // asm 00004D3E: 	LDP	@SWRAM+2
    // asm 00004D3F: 	LDI	@SWRAM+2,R0
    // asm 00004D40: 	ANDN	SW_VOLMINUS,R0
    // asm 00004D41: 	STI	R0,@SWRAM+2
    // asm 00004D42: 	SETDP
    // asm 00004D43: 	LDI	60,R0
    // asm 00004D44: 	STI	R0,@VOLUME_ACTIVE
    // asm 00004D45: 	READADJ	ADJ_MIN_VOL_LEVEL
    // asm 00004D47: 	LDI	R0,R3
    // asm 00004D48: 	READADJ	ADJ_VOLUME
    // asm 00004D4A: 	DEC	R0
    // asm 00004D4B: 	CMPI	R3,R0
    // asm 00004D4C: 	LDILT	R3,R0
    // asm 00004D4D: 	BU	VOLJN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VOL_MINUS", 0, 0);
    UNIMPL();
}

static void VOL_PLUS(void) {
    // asm 00004D4E: 	PUSH	R0
    // asm 00004D4F: 	PUSH	R1
    // asm 00004D50: 	PUSH	R2
    // asm 00004D51: 	PUSH	R3
    // asm 00004D52: 	PUSH	AR2
    // asm 00004D53: 	LDP	@SWRAM+2
    // asm 00004D54: 	LDI	@SWRAM+2,R0
    // asm 00004D55: 	ANDN	SW_VOLPLUS,R0
    // asm 00004D56: 	STI	R0,@SWRAM+2
    // asm 00004D57: 	SETDP
    // asm 00004D58: 	LDI	60,R0
    // asm 00004D59: 	STI	R0,@VOLUME_ACTIVE
    // asm 00004D5A: 	READADJ	ADJ_MIN_VOL_LEVEL
    // asm 00004D5C: 	LDI	R0,R3
    // asm 00004D5D: 	READADJ	ADJ_VOLUME
    // asm 00004D5F: 	INC	R0
    // asm 00004D60: 	CMPI	R3,R0
    // asm 00004D61: 	LDILT	R3,R0
VOLJN:
    // asm 00004D62: 	CMPI	0,R0
    // asm 00004D63: 	LDILE	0,R0
    // asm 00004D64: 	CMPI	255,R0		;??? strange - but 249-255 cut off
    // asm 00004D65: 	LDIGT	255,R0
    // asm 00004D66: 	LDI	R0,R2
    // asm 00004D67: 	PUSH	R0
    // asm 00004D68: 	SETADJ	ADJ_VOLUME
    // asm 00004D6A: 	POP	R1
    // asm 00004D6B: 	CALL	SET_MASTER_VOL
    // asm 00004D6C: 	POP	AR2
    // asm 00004D6D: 	POP	R3
    // asm 00004D6E: 	POP	R2
    // asm 00004D6F: 	POP	R1
    // asm 00004D70: 	POP	R0
    // asm 00004D71: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VOL_PLUS", 0, 0);
    UNIMPL();
}

static void DIAG_TOGGLE(void) {
    // asm 00004D72: 	PUSH	R0
    // asm 00004D73: 	LDI	@_MODE,R0
    // asm 00004D74: 	AND	MMODE,R0
    // asm 00004D75: 	CMPI	MDIAG,R0
    // asm 00004D76: 	BEQ	DTXX
    // asm 00004D77: 	LDI	@SWRAM+2,R0
    // asm 00004D78: 	ANDN	010h,R0
    // asm 00004D79: 	STI	R0,@SWRAM+2
    // asm 00004D7A: 	LDI	@DIAG_ACTIVE,R0
    // asm 00004D7B: 	LDIZ	1,R0
    // asm 00004D7C: 	LDINZ	0,R0
    // asm 00004D7D: 	STI	R0,@DIAG_ACTIVE
DTXX:
    // asm 00004D7E: POP	R0
    // asm 00004D7F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_TOGGLE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *OPTIMIZED 9/14/93-ELP
 */
static void ATODINT(void) {
    // asm 00004D80: 	PUSH	ST
    // asm 00004D81: 	PUSH	IE
    // asm 00004D82: 	PUSH	DP
    // asm 00004D83: 	LDP	@COMMINTM
    // asm 00004D84: 	LDI	@COMMINTM,IE
    // asm 00004D85: 	SETDP
    // asm 00004D86: 	CALL	ENABLEGIE
    // asm 00004D87: 	PUSH	R0
    // asm 00004D88: 	PUSHF	R0
    // asm 00004D89: 	PUSH	R6
    // asm 00004D8A: 	LDP	@CPU_WS
    // asm 00004D8B: 	PUSH	R1
    // asm 00004D8C: 	PUSHF	R1
    // asm 00004D8D: 	PUSH	AR0
    // asm 00004D8E: 	LDI	@CPU_WS,R6		;SET WAIT STATES
    // asm 00004D8F: 	LDI	1090h,R0
    // asm 00004D90: 	STI	R0,@CPU_WS
    // asm 00004D91: 	LDP	@RDPOT			;WHICH POT TO READ?
    // asm 00004D92: 	LDI	@RDPOT,R0
    // asm 00004D93: 	BNZD	RDFOOT
    // asm 00004D94: 	LDP	@SYSCNTL
    // asm 00004D95: 	LDI	@SYSCNTL,R1
    // asm 00004D96: 	ANDN	ATOD_RD,R1
    // 	;---->BNZD	RDFOOT
    // 					;RDSTEER
    // asm 00004D97: 	LDP	@SYSCNTLR
    // asm 00004D98: 	STI	R1,@SYSCNTLR
    // asm 00004D99: 	CLRI	AR0
    // asm 00004D9A: 	LDP	@ATOD_R
    // asm 00004D9B: 	LDI	@ATOD_R,R1
    // asm 00004D9C: 	LDI	*AR0,R0
    // asm 00004D9D: 	LDP	@_pot0
    // asm 00004D9E: 	RS	24,R1
    // asm 00004D9F: 	FLOAT	R1
    // asm 00004DA0: 	FLOAT	@_pot0,R0
    // 	;MAXIMUM SLEW RATE
    // 	;
    // asm 00004DA1: 	SUBF	R0,R1
    // asm 00004DA2: 	CMPF	65,R1
    // asm 00004DA3: 	LDFGT	65,R1
    // asm 00004DA4: 	CMPF	-65,R1
    // asm 00004DA5: 	LDFLT	-65,R1
    // asm 00004DA6: 	ADDF	R0,R1
    // asm 00004DA7: 	MPYF	0.33,R1	  		;SMOOTH WHEEL A TAD
    // asm 00004DA8: 	MPYF	0.67,R0
    // asm 00004DA9: 	ADDF	R1,R0
    // asm 00004DAA: 	FIX	R0
    // asm 00004DAB: 	STI	R0,@_pot0
    // asm 00004DAC: NOSMOOTH1
    // asm 00004DAC: 	LDP	@SYSCNTL
    // asm 00004DAD: 	LDI	@SYSCNTL,R0		;ACTUALLY WE SIGNAL A READ OF THE
    // asm 00004DAE: 	LDP	@SYSCNTLR		;HAVE A VALUE
    // asm 00004DAF: 	ANDN	ATOD_WR,R0		;POT(S), THEY INTERRUPT US WHEN THEY
    // asm 00004DB0: 	STI	R0,@SYSCNTLR		;HAVE A VALUE
    // asm 00004DB1: 	LDP	@ATOD_R
    // asm 00004DB2: 	CLRI	AR0
    // asm 00004DB3: 	LDI	05h,R0			;SETUP TO READ GAS PEDAL
    // asm 00004DB4: 	LS	24,R0
    // asm 00004DB5: 	STI	R0,@ATOD_R
    // asm 00004DB6: 	LDI	*AR0,R0
    // asm 00004DB7: 	BUD	EXITR
    // asm 00004DB8: 	LDP	@RDPOT
    // asm 00004DB9: 	LDI	1,R0
    // asm 00004DBA: 	STI	R0,@RDPOT
    // 	;---->	BUD	EXITR
RDFOOT:
    // asm 00004DBB: 	CMPI	1,R0
    // asm 00004DBC: 	BNED	RDBRAKE
    // asm 00004DBD: 	LDP	@SYSCNTLR
    // asm 00004DBE: 	STI	R1,@SYSCNTLR
    // asm 00004DBF: 	LDP	@ATOD_R			;READ GAS PEDAL
    // 	;---->	BNED	RDBRAKE
    // asm 00004DC0: 	CLRI	AR0
    // asm 00004DC1: 	LDI	@ATOD_R,R1
    // asm 00004DC2: 	LDI	*AR0,R0
    // asm 00004DC3: 	LDP	@_pot1
    // asm 00004DC4: 	RS	24,R1
    // asm 00004DC5: 	FLOAT	R1
    // asm 00004DC6: 	FLOAT	@_pot1,R0
    // asm 00004DC7: 	MPYF	0.33,R1	  		;SMOOTH A TAD
    // asm 00004DC8: 	MPYF	0.67,R0
    // asm 00004DC9: 	ADDF	R1,R0
    // asm 00004DCA: 	FIX	R0
    // asm 00004DCB: 	STI	R0,@_pot1
    // asm 00004DCC: 	LDP	@SYSCNTL
    // asm 00004DCD: 	LDI	@SYSCNTL,R0		;ACTUALLY WE SIGNAL A READ OF THE
    // asm 00004DCE: 	LDP	SYSCNTLR
    // asm 00004DCF: 	ANDN	ATOD_WR,R0		;POT(S), THEY INTERRUPT US WHEN THEY
    // asm 00004DD0: 	STI	R0,@SYSCNTLR
    // asm 00004DD1: 	LDP	@ATOD_R
    // asm 00004DD2: 	LDI	06h,R0			;SETUP TO READ BRAKE PEDAL
    // asm 00004DD3: 	LS	24,R0
    // asm 00004DD4: 	CLRI	AR0
    // asm 00004DD5: 	STI	R0,@ATOD_R
    // asm 00004DD6: 	LDI	*AR0,R0
    // asm 00004DD7: 	BUD	EXITR
    // asm 00004DD8: 	LDP	@RDPOT
    // asm 00004DD9: 	LDI	2,R0
    // asm 00004DDA: 	STI	R0,@RDPOT
    // 	;---->	BUD	EXITR
RDBRAKE:
    // asm 00004DDB: 	CLRI	AR0
    // asm 00004DDC: 	LDI	@ATOD_R,R1		;READ BRAKE PEDAL
    // asm 00004DDD: 	LDI	*AR0,R0
    // asm 00004DDE: 	LDP	@_pot2
    // asm 00004DDF: 	RS	24,R1
    // asm 00004DE0: 	FLOAT	R1
    // asm 00004DE1: 	FLOAT	@_pot2,R0
    // asm 00004DE2: 	MPYF	0.25,R1	  		;SMOOTH A TAD
    // asm 00004DE3: 	MPYF	0.75,R0
    // asm 00004DE4: 	ADDF	R1,R0
    // asm 00004DE5: 	FIX	R0
    // asm 00004DE6: 	STI	R0,@_pot2
    // asm 00004DE7: 	LDP	@RDPOT
    // asm 00004DE8: 	LDI	3,R0
    // asm 00004DE9: 	STI	R0,@RDPOT
EXITR:
    // asm 00004DEA: 	LDP	@CPU_WS
    // asm 00004DEB: 	POP	AR0
    // asm 00004DEC: 	POPF	R1
    // asm 00004DED: 	POP	R1
    // asm 00004DEE: 	STI	R6,@CPU_WS
    // asm 00004DEF: 	POP	R6
    // asm 00004DF0: 	POPF	R0
    // asm 00004DF1: 	POP	R0
    // asm 00004DF2: 	POP	DP
    // asm 00004DF3: 	ANDN	INT3_M,IF		;masking a2d int. (no longer pending)
    // asm 00004DF4: 	POP	IE
    // asm 00004DF5: 	POP	ST
    // asm 00004DF6: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ATODINT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SWITCH DISPATCH	 START PROCESSES FOR SWITCHES
 *
 */
static void SWDISP(void) {
    // ;	LDP	@SWRAM
    // asm 00004DF7: 	LDI	@SWRAM+2,R3
    // asm 00004DF8: 	LS	4,R3 		;MASK GARBAGE
    // asm 00004DF9: 	RS	4,R3
    // asm 00004DFA: 	BZ	SWSTX	 	;NO EDGES ACTIVATED
    // asm 00004DFB: 	LDI	0,R1
    // asm 00004DFC: 	STI	R1,@SWRAM+2	;CLEAR OUT EDGES
    // asm 00004DFD: 	LDI	-1,R1
SWTLP:
    // asm 00004DFE: 	ADDI	1,R1
    // asm 00004DFF: 	LSH	-1,R3
    // asm 00004E00: 	BNC	SWTLP
    // ;	LDP	@SWTABI
    // asm 00004E01: 	LDI	@SWTABI,AR0
    // asm 00004E02:      	ADDI	R1,AR0
    // asm 00004E03: 	LDI	*AR0,AR2	;GET WAKEUP
    // asm 00004E04: 	CMPI	0,AR2
    // asm 00004E05: 	BEQ	NOGPROC
    // asm 00004E06: 	LDI	SPAWNER_C|SPWN_SWITCH_T,R2
    // asm 00004E07: 	CALL	PRC_CREATE	;R2=PID, AR2=WAKEUP ADDR
NOGPROC:
    // asm 00004E08: LDI	R3,R3
    // asm 00004E09: 	BNZ	SWTLP
SWSTX:
    // asm 00004E0A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SWDISP", 0, 0);
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SWITCH ROUTINE TABLE
 *0 = NO PROCESS TO CREATE
 */
/* asm: SWTABI	.word	SWTAB */
#define SWTABI SWTAB
/* asm: SWTAB */
/* asm: 	.word	COIN1  		;00000001 SW_COIN1	(COIN.ASM) */
/* asm: 	.word	COIN2		;00000002 SW_COIN2 	(COIN.ASM) */
/* asm: 	.word	_start		;00000004 START		(INTRO.ASM) */
/* asm: 	.word	0		;00000008 */
/* asm: 	.word	0		;00000010 SW_DIAG	(DIAG.ASM) */
/* asm: 	.word	0		;00000020 */
/* asm: 	.word	SERV_COIN	;00000040 SW_COINSRV	(COIN.ASM) */
/* asm: 	.word	COIN3		;00000080 */
/* asm: 	.word	0		;00000100 */
/* asm: 	.word	0		;00000200 */
/* asm: 	.word	0		;00000400 */
/* asm: 	.word	0		;00000800 */
/* asm: 	.word	0		;00001000 */
/* asm: 	.word	0		;00002000 */
/* asm: 	.word	COIN4		;00004000 */
/* asm: 	.word	0		;00008000 */
/* asm: 	.word	0		;00010000 BRAKE */
/* asm: 	.word	RADIO_BUT	;00020000 RADIO (OLD ABORT) */
/* asm: 	.word	0		;00040000 LOW */
/* asm: 	.word	_debug		;00080000 DEBUG */
/* asm: 	.word	_VIEW0		;00100000 VIEW0 */
/* asm: 	.word	_VIEW1		;00200000 VIEW1 */
/* asm: 	.word	_VIEW2		;00400000 VIEW2 */
/* asm: 	.word	0		;00800000 VIEW4 */
/* asm: 	.word	0		;01000000 */
/* asm: 	.word	0		;02000000 */
/* asm: 	.word	0		;04000000 */
/* asm: 	.word	0		;08000000 */
/* asm: 	.word	0		;10000000 */
/* asm: 	.word	0		;20000000 */
/* asm: 	.word	0		;40000000 */
/* asm: 	.word	0		;80000000 */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
static void* SWTAB[] = {
    COIN1,     // 00000001 SW_COIN1	(COIN.ASM)
    COIN2,     // 00000002 SW_COIN2 	(COIN.ASM)
    _start,    // 00000004 START		(INTRO.ASM)
    0,         // 00000008
    0,         // 00000010 SW_DIAG	(DIAG.ASM)
    0,         // 00000020
    SERV_COIN, // 00000040 SW_COINSRV	(COIN.ASM)
    COIN3,     // 00000080
    0,         // 00000100
    0,         // 00000200
    0,         // 00000400
    0,         // 00000800
    0,         // 00001000
    0,         // 00002000
    COIN4,     // 00004000
    0,         // 00008000
    0,         // 00010000 BRAKE
    RADIO_BUT, // 00020000 RADIO (OLD ABORT)
    0,         // 00040000 LOW
    _debug,    // 00080000 DEBUG
    _VIEW0,    // 00100000 VIEW0
    _VIEW1,    // 00200000 VIEW1
    _VIEW2,    // 00400000 VIEW2
    0,         // 00800000 VIEW4
    0,         // 01000000
    0,         // 02000000
    0,         // 04000000
    0,         // 08000000
    0,         // 10000000
    0,         // 20000000
    0,         // 40000000
    0,         // 80000000
    // ----------------------------------------------------------------------------
};

// *----------------------------------------------------------------------------
void CHECKDIAG(void) {
    // asm 00004E2C: 	LDI	@DIAG_ACTIVE,R0
    // asm 00004E2D: 	RETSZ
    // asm 00004E2E: 	BR	ENTER_DIAG
    // ;	RETS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKDIAG", 0, 0);
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

static void CLR_PBSS(void) {
}

// *----------------------------------------------------------------------------

// ;RAM_BSSEND	.word	01F7FFh
/* asm: RAM_BSSEND	.word	01EFFFh		;save protected hi bss ram */
/* asm: 	 */
static int RAM_BSSEND = 0x01EFFF;

void CLR_RAM(void) {
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *THESE MUST BE CALLED *BEFORE* PROGRAM IS COPIED INTO FAST RAM
 *
 */
static void CLEAR_ONCHIPRAM(void) {
    // asm 00004E58: 	LDIL	809800h,AR0
    // asm 00004E5B: 	CLRI	R0
    // asm 00004E5C: 	LDI	2047h,RC
    // asm 00004E5D: 	RPTS	RC
    // asm 00004E5E: 	STI	R0,*AR0++
    // asm 00004E5F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_ONCHIPRAM", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 */
#define SPACER 20
/* asm: BUTTON_TIKS	fbss	BUTTON_TIKS,1 */
int BUTTON_TIKS;
/* asm: BUTTON_STATUS	pbss	BUTTON_STATUS,1 */
int BUTTON_STATUS;
/* asm: OLD_BUTTON_STATUS	.bss	OLD_BUTTON_STATUS,1 */
int OLD_BUTTON_STATUS;

// *
static void BUTTONS(void) {
    // asm 00004E60: 	LDI	@_MODE,R0
    // asm 00004E61: 	AND	MMODE,R0
    // asm 00004E62: 	CMPI	MDIAG,R0
    // asm 00004E63: 	BEQ	DIAG_BUTTONS
    // asm 00004E64: 	CMPI	MATTR,R0
    // asm 00004E65: 	BNE	BUT3
    // asm 00004E66: 	READAUD	ADJ_FREE_PLAY
    // asm 00004E68: 	CMPI	1,R0
    // asm 00004E69: 	BNE	JUSTGOON
    // asm 00004E6A: 	LDI	@BUTTON_STATUS,R0
    // asm 00004E6B: 	OR	BUT_START,R0
    // asm 00004E6C: 	STI	R0,@BUTTON_STATUS
    // asm 00004E6D: 	BU	BUT3
JUSTGOON:
    // asm 00004E6E: 	READAUD	AUD_CREDITS
    // asm 00004E70: 	CMPI	0,R0
    // asm 00004E71: 	BLE	DASHLIGHT
BUT3:
    // asm 00004E72: 	LDI	@BUTTON_STATUS,R0
    // asm 00004E73: 	TSTB	BUT_START,R0
    // asm 00004E74: 	BZ	NOSTART
    // asm 00004E75: 	LDP	@BUTTON_TIKS
    // asm 00004E76: 	LDI	@BUTTON_TIKS,R1
    // asm 00004E77: 	INC	R1
    // asm 00004E78: 	STI	R1,@BUTTON_TIKS
    // asm 00004E79: 	SETDP
    // asm 00004E7A: 	CMPI	30,R1
    // asm 00004E7B: 	BLT	NOSTART
    // asm 00004E7C: 	ANDN	BUT_START,R0		;TURN IT OFF
    // asm 00004E7D: 	CMPI	60,R1
    // asm 00004E7E: 	BLT	NOSTART
    // asm 00004E7F:  	CLRI	R1
    // asm 00004E80: 	LDP	@BUTTON_TIKS
    // asm 00004E81: 	STI	R1,@BUTTON_TIKS
    // asm 00004E82: 	SETDP
NOSTART:
    // asm 00004E83: 	LDI	@HEAD2HEAD_ON,R2
    // asm 00004E84: 	BNZ	PAPAFFD
    // asm 00004E85: 	CMPI	@OLD_BUTTON_STATUS,R0
    // asm 00004E86: 	RETSEQ
PAPAFFD:
    // asm 00004E87: STI	R0,@OLD_BUTTON_STATUS
BUTLITE:
    // asm 00004E88: 	LDIL	SOUND,AR0
    // asm 00004E8B: 	LDI	0FF05h,R1
    // asm 00004E8C: 	LDI	0F705h,R2
    // asm 00004E8D: 	STI	R1,*AR0
    // asm 00004E8E: 	RPTS	SPACER
    // asm 00004E8F: 	NOP
    // asm 00004E90: 	STI	R2,*AR0
    // asm 00004E91: 	RPTS	SPACER
    // asm 00004E92: 	NOP
    // asm 00004E93: 	STI	R1,*AR0
    // asm 00004E94: 	RPTS	SPACER
    // asm 00004E95: 	NOP
    // asm 00004E96: 	LDI	@HEAD2HEAD_ON,R2
    // asm 00004E97: 	BZ	KKDAK
    // asm 00004E98: 	LDI	@THIS_MACHINE_AHEAD,R2
    // asm 00004E99: 	BNZ	KKDAK
    // asm 00004E9A: 	AND	0FFh,R0
    // asm 00004E9B: 	ANDN	BUT_FRONT,R0
    // asm 00004E9C: 	BU	FDDDA
KKDAK:
    // asm 00004E9D: AND	0FFh,R0
    // asm 00004E9E: 	OR	BUT_FRONT,R0
FDDDA:
    // asm 00004E9F: LDI	R0,R1
    // asm 00004EA0: 	OR	0FF00h,R0
    // asm 00004EA1: 	OR	0FB00h,R1
    // asm 00004EA2: 	STI	R0,*AR0
    // asm 00004EA3: 	RPTS	SPACER
    // asm 00004EA4: 	NOP
    // asm 00004EA5: 	STI	R1,*AR0
    // asm 00004EA6: 	RPTS	SPACER
    // asm 00004EA7: 	NOP
    // asm 00004EA8: 	STI	R0,*AR0
    // asm 00004EA9: 	RPTS	SPACER
    // asm 00004EAA: 	NOP
    // asm 00004EAB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BUTTONS", 0, 0);
    UNIMPL_TODO();
}

static void DIAG_BUTTONS(void) {
    // asm 00004EAC: 	LDP	@BUTTON_STATUS
    // asm 00004EAD: 	LDI	@BUTTON_STATUS,R0
    // asm 00004EAE: 	SETDP
    // asm 00004EAF: 	LDP	@BUTTON_TIKS
    // asm 00004EB0: 	LDI	@BUTTON_TIKS,R1
    // asm 00004EB1: 	INC	R1
    // asm 00004EB2: 	STI	R1,@BUTTON_TIKS
    // asm 00004EB3: 	SETDP
    // asm 00004EB4: 	CMPI	30,R1
    // asm 00004EB5: 	BLT	DGBT
    // asm 00004EB6: 	CLRI	R0
    // asm 00004EB7: 	CMPI	50,R1
    // asm 00004EB8: 	BLT	DGBT
    // asm 00004EB9: 	CLRI	R1
    // asm 00004EBA: 	LDP	@BUTTON_TIKS
    // asm 00004EBB: 	STI	R1,@BUTTON_TIKS
    // asm 00004EBC: 	SETDP
DGBT:
    // asm 00004EBD: 	LDIL	SOUND,AR0
    // asm 00004EC0: 	LDI	0FF05h,R1
    // asm 00004EC1: 	LDI	0F705h,R2
    // asm 00004EC2: 	STI	R1,*AR0
    // asm 00004EC3: 	RPTS	SPACER
    // asm 00004EC4: 	NOP
    // asm 00004EC5: 	STI	R2,*AR0
    // asm 00004EC6: 	RPTS	SPACER
    // asm 00004EC7: 	NOP
    // asm 00004EC8: 	STI	R1,*AR0
    // asm 00004EC9: 	RPTS	SPACER
    // asm 00004ECA: 	NOP
    // asm 00004ECB: 	LDI	R0,R1
    // asm 00004ECC: 	OR	0FF00h,R0
    // asm 00004ECD: 	OR	0FB00h,R1
    // asm 00004ECE: 	STI	R0,*AR0
    // asm 00004ECF: 	RPTS	SPACER
    // asm 00004ED0: 	NOP
    // asm 00004ED1: 	STI	R1,*AR0
    // asm 00004ED2: 	RPTS	SPACER
    // asm 00004ED3: 	NOP
    // asm 00004ED4: 	STI	R0,*AR0
    // asm 00004ED5: 	RPTS	SPACER
    // asm 00004ED6: 	NOP
    // asm 00004ED7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_BUTTONS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *WAIT FOR INTERRUPT AT VBLANK
 *
 */
void WAIT_FOR_VBLANK(void) {
    // asm 00004ED8: 	LDI	@INFRAMES,R0
WAIT1:
    // asm 00004ED9: CMPI	@INFRAMES,R0		;wait for a hot one
    // asm 00004EDA: 	BZ	WAIT1
    // asm 00004EDB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_FOR_VBLANK", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CRT_REG_SETUP_STRI */
/* asm: .word	CRT_REG_SETUP_STR */
/* asm: romdata */
static int CRT_REG_SETUP_STRI;
/* asm: CRT_REG_SETUP_STR */
/* asm: 	.word	399|CRT_SETUP_ICSYNC	;CRT_SETUP */
/* asm: 	.word	01ffh		;CRT_HADDRINC */
/* asm: 	.word	01feh		;CRT_HBLKSTART */
/* asm: 	.word	020eh		;CRT_HSYNCSTART */
/* asm: 	.word	0227h		;CRT_HSYNCEND */
/* asm: 	.word	0299h		;CRT_HBLKEND */
/* asm: 	.word	029ah		;CRT_HTTL */
/* asm: 	 */
/* asm: 	.word	018eh		;CRT_VBLKSTART */
/* asm: 	.word	0191h		;CRT_SYNCSTART */
/* asm: 	.word	0194h		;CRT_SYNCEND */
/* asm: 	.word	01afh		;CRT_VBLK */
/* asm: 	.word	01b0h		;CRT_VTTL */
/* asm: 	 */
/* asm: 	 */
static int CRT_REG_SETUP_STR[] = {
    399 | CRT_SETUP_ICSYNC, // CRT_SETUP
    0x01ff,                 // CRT_HADDRINC
    0x01fe,                 // CRT_HBLKSTART
    0x020e,                 // CRT_HSYNCSTART
    0x0227,                 // CRT_HSYNCEND
    0x0299,                 // CRT_HBLKEND
    0x029a,                 // CRT_HTTL
    0x018e,                 // CRT_VBLKSTART
    0x0191,                 // CRT_SYNCSTART
    0x0194,                 // CRT_SYNCEND
    0x01af,                 // CRT_VBLK
    0x01b0,                 // CRT_VTTL
    // before syncing
    // 	.word	400|CRT_SETUP_ICSYNC	;CRT_SETUP
    // 	.word	01ffh		;CRT_HADDRINC
    // 	.word	01feh		;CRT_HBLKSTART
    // 	.word	020eh		;CRT_HSYNCSTART
    // 	.word	0227h		;CRT_HSYNCEND
    // 	.word	0299h		;CRT_HBLKEND
    // 	.word	029ah		;CRT_HTTL
    // 	.word	018fh		;CRT_VBLKSTART
    // 	.word	0192h		;CRT_SYNCSTART
    // 	.word	0195h		;CRT_SYNCEND
    // 	.word	01b0h		;CRT_VBLK
    // 	.word	01b0h		;CRT_VTTL
};

static void CRT_REG_SETUP(void) {
    // asm 00004EDD: 	PUSHM	AR0,AR1,R0,DP
    // asm 00004EE1: 	LDIL	CRT_SETUP,AR0
    // asm 00004EE4: 	LDP	@CRT_REG_SETUP_STRI
    // asm 00004EE5: 	LDI	@CRT_REG_SETUP_STRI,AR1
    // asm 00004EE6: 	LDI	11,RC
    // asm 00004EE7: 	RPTB	CRTRGLP
    // asm 00004EE8: 	LDI	*AR1++,R0
CRTRGLP:
    // asm 00004EE9: STI	R0,*AR0++
    // asm 00004EEA: 	POPM	DP,R0,AR1,AR0
    // asm 00004EEE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CRT_REG_SETUP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *IF THIS IS EVER HIT STOP AND FIND OUT WHERE IT IS
 *COMING FROM
 *
 */
static void ERROR_TRAP(void) {
    // asm 00004EEF: 	CALL	ENABLEGIE
    // asm 00004EF0: 	NOP
    // asm 00004EF1: 	NOP
    // asm 00004EF2: 	NOP
    // asm: 	SLOCKON	U,"ERROR_TRAP INT"
#if DEBUG
    // asm: 	BU	$
#endif
    // asm 00004EF3: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ERROR_TRAP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void FIFO_RESET(void) {
    // TODO?
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
#if STATISTICS
/* asm: ST_POLYGONS	.bss	ST_POLYGONS,1 */
int ST_POLYGONS;
/* asm: ST_VERTICES	.bss	ST_VERTICES,1 */
int ST_VERTICES;
/* asm: ST_OBJECTS	.bss	ST_OBJECTS,1 */
int ST_OBJECTS;
#endif
// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *1ms   = 10 counts
 *1mils = 10000
 *1frame should = 165000 (roughly)
 *
 */
void TIMER_RESET(void) {
    // TIMER_CNTL1 |= TMR_RESET_GO;
    // TOOD
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void TIMER_READ(void) {
    // asm 00004F13: 	PUSH	DP
    // asm 00004F14: 	LDP	@TIMER_CNTR1
    // asm 00004F15: 	LDI	@TIMER_CNTR1,R0
    // asm 00004F16: 	POP	DP
    // asm 00004F17: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMER_READ", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/* asm: TIMEFRAME	.bss	TIMEFRAME,1 */
int TIMEFRAME;
/* asm: TIMEX	.bss	TIMEX,1 */
float* TIMEX;
/* asm: TIMECLR	.bss	TIMECLR,1 */
int TIMECLR;
/* asm: TIMERAM	.bss	TIMERAM,50 */
float TIMERAM[50];

// *----------------------------------------------------------------------------
static void TIMERESET(void) {
    TIMER_RESET();

    TIMEX = TIMERAM;

    if (TIMECLR != 0) {
        TIMECLR = 0;
        TIMEFRAME = 0.0f;

        /*
         * CLEAR OUT COUNT AREA
         */
        for (int i = 0; i < 48; i++) {
            TIMERAM[i] = 0.0f;
        }

        return;
    }

    TIMEFRAME += 1.0f;

    float inv_frame = INV_F30(TIMEFRAME);

    TIMEX = TIMERAM;

    for (int i = 0; i < 16; i++) {
        float t = TIMERAM[i];

        /*
         * This corresponds to:
         *   LDF  *AR0++,R1
         *   ADDF *+AR0(15),R1
         *   STF  R1,*+AR0(15)
         *   MPYF R0,R1
         *   STF  R1,*+AR0(31)
         *
         * Because AR0 post-increments before the +15/+31 offsets,
         * this updates TIMERAM[i + 16] and TIMERAM[i + 32].
         */
        TIMERAM[i + 16] += t;
        TIMERAM[i + 32] = TIMERAM[i + 16] * inv_frame;
    }

    // TIMERAM[0..15]   // current frame timing records
    // TIMERAM[16..31]  // accumulated totals
    // TIMERAM[32..47]  // averaged/scaled timing values
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static float TIMEREC(void) {
    float t;

    t = (float)TIMER_CNTR1;
    *TIMEX++ = t; // SAVE THE INDEX
    return t;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MESSAGE1(void) {
    CLRSCRN();
    SETPAGE0();
    crusn_mem_wr32(COLORAM + 0x000u, 0);
    crusn_mem_wr32(COLORAM + 0x001u, (uint32_t)-1); // white
    crusn_mem_wr32(COLORAM + 0x003u, 0x06000u);     // red
    crusn_mem_wr32(COLORAM + 0x00Bu, 0x0294Au);     // grey

    ACTIVE_SCREEN = SCREEN0; // set active screen to 1 (writeable)

    TEXTIT(startup0, 1, 40, 1);
    TEXTIT(startup1, 1, 70, 1);
    TEXTIT(startup2, 1, 80, 1);
    TEXTIT(startup3, 1, 90, 1);
    TEXTIT(dlts, 40, 110, 11);
    TEXTIT(VERSION_STAMP, 40, 130, 1);
    TEXTIT(INTERNAL_VERS, 260, 150, 1);
    TEXTIT(DATE_STAMP, 40, 150, 1);

    uint32_t dipsw = port_get_dipswitches() >> 16;
    const char* link_status = LINKDISABLED;
    if ((dipsw & DIP_COMMP) == 0) {
        link_status = (dipsw & CMDP_MASTER) == 0 ? IAMMASTER : IAMSLAVE;
    }
    TEXTIT(link_status, 40, 160, 1);
    // 	;test if link pal is installed
    // 	;
    // 	;
    uint32_t commpal = port_get_commpal() & 0x0Fu;
    const char* pal_status = commpal == 4 ? TPALI : TPALNI; // BAD PAL ?
    TEXTIT(pal_status, 40, 170, 1);
    // 	;
    // 	;
    _rectangle(239, 110, 301, 117, 11);
    TEXTIT(M1, 1, 190, 11);
}

/* asm: MSG_CNT	.bss	MSG_CNT,1 */
static int MSG_CNT;

static void MSG1(void) {
    TEXTIT(M2, 1, 200, 11);
    TEXTIT(M3, 1, 220, 11);
    TEXTIT(M4, 1, 230, 11);
    TEXTIT(M5, 1, 240, 11);
}

static void MSG2(void) {
    TEXTIT(M6, 1, 260, 11);
    TEXTIT(M7, 1, 270, 11);
}

/* asm: LINKDISABLED	SPTR	"LINK DISABLED BY U97  DIP6 OFF" */
static const char LINKDISABLED[] = "LINK DISABLED BY U97  DIP6 OFF";
/* asm: IAMMASTER	SPTR	"LINK MASTER MACHINE" */
static const char IAMMASTER[] = "LINK MASTER MACHINE";
/* asm: IAMSLAVE	SPTR	"LINK SLAVE MACHINE" */
static const char IAMSLAVE[] = "LINK SLAVE MACHINE";
/* asm: TPALI		SPTR	"U38 LINK PAL INSTALLED" */
static const char TPALI[] = "U38 LINK PAL INSTALLED";
/* asm: TPALNI		SPTR	"U38 LINK PAL NOT INSTALLED" */
static const char TPALNI[] = "U38 LINK PAL NOT INSTALLED";

static void MSG3(void) {
    TEXTIT(M8, 1, 280, 11);
    TEXTIT(M9, 1, 290, 11);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SECRET OCTOPUS
 *
 *
 */
#define NUM_STATES 7
/* asm: STATE_TABLE	.word	SW_VIEW0|SW_VIEW2 */
/* asm: 	.word	SW_VIEW2 */
/* asm: 	.word	SW_VIEW1|SW_VIEW2 */
/* asm: 	.word	SW_VIEW1 */
/* asm: 	.word	SW_VIEW0|SW_VIEW1 */
/* asm: 	.word	SW_VIEW0 */
/* asm: 	.word	SW_VIEW0|SW_RADIO */
static int STATE_TABLE[] = {
    SW_VIEW0 | SW_VIEW2,
    SW_VIEW2,
    SW_VIEW1 | SW_VIEW2,
    SW_VIEW1,
    SW_VIEW0 | SW_VIEW1,
    SW_VIEW0,
    SW_VIEW0 | SW_RADIO,
};
// *
/* asm: STATE_MASK	.word	SW_VIEW0|SW_VIEW1|SW_VIEW2|SW_RADIO */
static int STATE_MASK = SW_VIEW0 | SW_VIEW1 | SW_VIEW2 | SW_RADIO;
/* asm: STATE_NUM	.bss	STATE_NUM,1 */
int STATE_NUM;
/* asm: STATE_TIK	.bss	STATE_TIK,1 */
int STATE_TIK;

/*
 *
 *
 *
 */
static void CHECK_STATE(void) {
    // asm 00004FD3: 	LDI	@_MODE,R0
    // asm 00004FD4: 	AND	MMODE,R0
    // asm 00004FD5: 	CMPI	MATTR,R0
    // asm 00004FD6: 	BNE	ABORT_STATE
    // asm 00004FD7: 	LDI	@SWITCHBUTS,R2
    // asm 00004FD8: 	LDI	@STATE_NUM,AR0
    // asm 00004FD9: 	LDI	AR0,R0
    // asm 00004FDA: 	ADDI	@STATE_TABLEI,AR0
    // asm 00004FDB: 	LDI	*AR0,R1
    // asm 00004FDC: 	AND	@STATE_MASK,R2
    // asm 00004FDD: 	CMPI	R1,R2		;ADVANCE TO NEXT STATE?
    // asm 00004FDE: 	BEQ	NEXTSTATE
    // asm 00004FDF: 	LDI	@STATE_NUM,R1
    // asm 00004FE0: 	BZ	ABORT_STATE
    // asm 00004FE1: 	DEC	R1
    // asm 00004FE2: 	LDI	R1,AR0
    // asm 00004FE3: 	ADDI	@STATE_TABLEI,AR0
    // asm 00004FE4: 	LDI	*AR0,R1
    // asm 00004FE5: 	CMPI	R1,R2		;IS IT THE PREVIOUS STATE??
    // asm 00004FE6: 	BNE	ABORT_STATE
    // asm 00004FE7: 	LDI	@STATE_TIK,R0
    // asm 00004FE8: 	DEC	R0		;ABORT STATE
    // asm 00004FE9: 	BLT	ABORT_STATE
    // asm 00004FEA: 	STI	R0,@STATE_TIK
    // asm 00004FEB: 	RETS
NEXTSTATE:
    // asm 00004FEC: 	LDI	@STATE_NUM,R0
    // asm 00004FED: 	INC	R0
    // asm 00004FEE: 	CMPI	NUM_STATES,R0
    // asm 00004FEF: 	BEQ	DODIAG
    // asm 00004FF0: 	STI	R0,@STATE_NUM
    // asm 00004FF1: 	LDI	120,R0
    // asm 00004FF2: 	STI	R0,@STATE_TIK
    // asm 00004FF3: 	RETS
DODIAG:
    // asm 00004FF4: 	CLRI	R0
    // asm 00004FF5: 	STI	R0,@STATE_NUM
    // asm 00004FF6: 	STI	R0,@STATE_TIK
    // asm 00004FF7: 	BR	HIDDEN_DIAG
    // *
    // *
    // *
ABORT_STATE:
    // asm 00004FF8: 	CLRI	R0
    // asm 00004FF9: 	STI	R0,@STATE_NUM
    // asm 00004FFA: 	STI	R0,@STATE_TIK
    // asm 00004FFB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_STATE", 0, 0);
    UNIMPL_TODO();
}

// *----------------------------------------------------------------------------

/* asm: BUTTON_IBO	.bss	BUTTON_IBO,1 */
int BUTTON_IBO;
/* asm: BUTTON_TIK	.bss	BUTTON_TIK,1 */
int BUTTON_TIK;
// *----------------------------------------------------------------------------
/* asm: BUTTONI	.word	BUTTII */
#define BUTTONI BUTTII
/* asm: BUTTII	.word	BUT_VIEW1,BUT_VIEW2,BUT_VIEW3,BUT_VIEW2 */
static int BUTTII[] = {
    BUT_VIEW1,
    BUT_VIEW2,
    BUT_VIEW3,
    BUT_VIEW2,
};

// *
static void DASHLIGHT(void) {
    // asm 00005001: 	LDI	@BUTTON_TIK,R0
    // asm 00005002: 	INC	R0
    // asm 00005003: 	LDILT	0,R0
    // asm 00005004: 	STI	R0,@BUTTON_TIK
    // asm 00005005: 	CMPI	60,R0
    // asm 00005006: 	RETSLT
    // asm 00005007: 	LDI	0,R0
    // asm 00005008: 	STI	R0,@BUTTON_TIK
    // asm 00005009: 	LDI	@BUTTON_IBO,R0
    // asm 0000500A: 	INC	R0
    // asm 0000500B: 	CMPI	3,R0
    // asm 0000500C: 	LDIGE	0,R0
    // asm 0000500D: 	CMPI	0,R0
    // asm 0000500E: 	LDILT	0,R0
    // asm 0000500F: 	STI	R0,@BUTTON_IBO
    // asm 00005010: 	LDI	R0,AR0
    // asm 00005011: 	ADDI	@BUTTONI,AR0
    // asm 00005012: 	LDI	*AR0,R0
    // asm 00005013: 	B	BUTLITE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DASHLIGHT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CMOS_ERROR(void) {
    // asm 00005014: 	LDI	8,AR6
    // asm 00005015: FLASH_LP
    // asm 00005015: 	CALL	CLRSCRN			;CMOS WAS RESET
    // asm 00005016: 	CALL	SETPAGE0
    // asm 00005017: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 0000501A: 	STI	R0,@ACTIVE_SCREEN
    // asm 0000501B: 	LDI	100,AR5
    // asm 0000501C: FLASH_LP1
    // asm 0000501C: 	LDI	1,RC
    // asm 0000501D: 	TEXTITT	"CMOS ERROR",216,190
    // asm 00005021: 	TEXTITT	"FACTORY ADJUSTMENTS RESTORED",144,200
    // asm 00005025: 	DBU	AR5,FLASH_LP1
    // asm 00005026: 	CALL	FEED_WATCHDOG
    // asm 00005027: 	DBU	AR6,FLASH_LP
    // asm 00005028: 	CALL	FAKEDIAG
    // asm 00005029: 	CALL	SET_CONTROLS
    // asm 0000502A: 	BR	DIAG_RETURN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_ERROR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void VERSION_UPDATE(void) {
    // asm 0000502B: 	LDI	8,AR6
    // asm 0000502C: VFLASH_LP
    // asm 0000502C: 	CALL	CLRSCRN			;CMOS WAS RESET
    // asm 0000502D: 	CALL	SETPAGE0
    // asm 0000502E: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 00005031: 	STI	R0,@ACTIVE_SCREEN
    // asm 00005032: 	LDI	100,AR5
    // asm 00005033: VFLASH_LP1
    // asm 00005033: 	LDI	1,RC
    // asm 00005034: 	TEXTITT	"VERSION UPDATED",196,190
    // asm 00005038: 	TEXTITT	"FACTORY SETTINGS RESTORED",156,200
    // asm 0000503C: 	DBU	AR5,VFLASH_LP1
    // asm 0000503D: 	CALL	FEED_WATCHDOG
    // asm 0000503E: 	DBU	AR6,VFLASH_LP
    // ;RESET YOUR SHIT
    // asm 0000503F: 	CALL	FAKEDIAG
    // asm 00005040: 	CALL	FFRSUB
    // asm 00005041: 	LDI	VERSION_ID,R2
    // asm 00005042: 	SETAUD	AUD_VERSION
    // asm 00005044: 	BR	DIAG_RETURN
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VERSION_UPDATE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void FAKEDIAG(void) {
    // asm 00005045: 	CALL	INIT_SYSTEM
    // asm 00005046: 	CALL	PAL_INIT
    // asm 00005047: 	LDL	DIAGPAL,AR2
    // asm 00005048: 	CALL	PAL_ALLOC_RAW
    // asm 00005049: 	LDI	4210H,R0
    // asm 0000504A: 	STI	R0,@BGNDCOLA
    // asm 0000504B: 	LDI	MDIAG,R0		;set mode appropriate
    // asm 0000504C: 	STI	R0,@_MODE
    // asm 0000504D:  	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FAKEDIAG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void FEED_WATCHDOG(void) {
    // asm 0000504E: 	PUSH	DP
    // asm 0000504F: 	PUSH	R0
    // asm 00005050: 	LDP	@CPU_WS
    // asm 00005051: 	LDI	@CPU_WS,R0
    // asm 00005052: 	PUSH	R0
    // asm 00005053: 	LDI	SOFT_WS,R0
    // asm 00005054: 	STI	R0,@CPU_WS
    // asm 00005055: 	LDP	@SYSCNTL
    // asm 00005056: 	LDI	@SYSCNTL,R0

    // asm 00005057: 	XOR	WDOG,R0

    // asm 00005058: 	STI	R0,@SYSCNTL
    SYSCNTL = SYSCNTL ^ WDOG;
    // asm 00005059: 	LDP	@SYSCNTLR
    // asm 0000505A: 	STI	R0,@SYSCNTLR
    // asm 0000505B: 	POP	R0
    // asm 0000505C: 	LDP	@CPU_WS
    // asm 0000505D: 	STI	R0,@CPU_WS
    // asm 0000505E: 	POP	R0
    // asm 0000505F: 	POP	DP
    // asm 00005060: 	RETS
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: SYSCNTL_OC	fbss	SYSCNTL_OC,1 */
static int SYSCNTL_OC;

void FEED_WATCHDOG_HARD(void) {
    // asm 00005061: 	PUSH	DP
    // asm 00005062: 	PUSH	R0
    // asm 00005063: 	LDP	@CPU_WS
    // asm 00005064: 	LDI	@CPU_WS,R0
    // asm 00005065: 	PUSH	R0
    // asm 00005066: 	LDI	1068h,R0
    // asm 00005067: 	STI	R0,@CPU_WS
    // asm 00005068: 	LDP	@SYSCNTL
    // asm 00005069: 	LDI	@SYSCNTL,R0
    // asm 0000506A: 	XOR	WDOG,R0
    // asm 0000506B: 	STI	R0,@SYSCNTL
    // asm 0000506C: 	LDP	@SYSCNTLR
    // asm 0000506D: 	STI	R0,@SYSCNTLR
    // asm 0000506E: 	POP	R0
    // asm 0000506F: 	LDP	@CPU_WS
    // asm 00005070: 	STI	R0,@CPU_WS
    // asm 00005071: 	POP	R0
    // asm 00005072: 	POP	DP
    // asm 00005073: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FEED_WATCHDOG_HARD", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void VERIFY_CODE_INTEGRITY(void) {
#if DEBUG
    // 	;
    // 	;verify that the code has not been corrupted
    // 	;
    // 	;*this only works in RAM
    // 	;and, if used in ROM will cause a lockup
    // 	;
    // asm: 	LDI	40h,AR1			;SOURCE ADDRESS
    // asm: 	LDI	0C00h,AR2		;DESINATION ADDRESS
    // asm: 	LS	12,AR2
    // asm: 	ADDI	40h,AR2
    // asm: 	LDI	@BSSSTARTI,RC
    // asm: 	SUBI	041h,RC
    // asm: 	RPTB	LD_CHK1
    // asm: 	LDI	*AR1++,R0
    // asm: 	CMPI	*AR2++,R0
#if CODE_CHECK
    // asm: 	BNE	$
#endif
LD_CHK1:
    // asm 00005075: NOP
#endif
    // asm 00005075: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VERIFY_CODE_INTEGRITY", 0, 0);
    UNIMPL();
}

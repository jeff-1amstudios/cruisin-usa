#include "../core/cpu.h"
#include "../core/machine.h"
#include "mproc.h"
#include "macs.h"
#include "obj.h"
#include "globals.h"
#include "sys.h"
#include "c30.h"
#include "text.h"
#include "vunit.h"
#include "cmos.h"
#include "sndtab.h"
#include "pall.h"
#include "sysid.h"
#include "dirq.h"
#include "error.h"
#include "comm.h"
#include "checksum.h"
#include "cusa.h"
#include "bssstart.h"

/*
 * Source module: asm/CUSA.ASM
 */

void _c_int00(void);
static void MAINLOOP(void);
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
static void TIMEREC(void);
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

#define SWRAMI SWRAM
#define SWTABI SWTAB
#define TIMERAMI TIMERAM
#define STATE_TABLEI STATE_TABLE
#define BUTTONI BUTTII
#define BSSSTARTI BSSSTART

void HIDDEN_DIAG(void);
void SET_CONTROLS(void);
void FFRSUB(void);
extern int DIAGPAL;

static void *SWTAB[32];
static int CRT_REG_SETUP_STR[12];
static const char LINKDISABLED[];
static const char IAMMASTER[];
static const char IAMSLAVE[];
static const char TPALI[];
static const char TPALNI[];

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
const char VERSION_STAMP[] = "VERSION  4.4";
#endif
static const char INTERNAL_VERS[] = "I440";
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
int GAME_TIMER;
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

// *----------------------------------------------------------------------------
void _c_int00(void)
{
    // asm 00004AE6: 	LDI	02h,IOF			;TV30 KLUDGE
    // asm 00004AE7: 	CLRI	R0			;must fix COMM setup asap
    // asm 00004AE8: 	LDP	@COMMINTM
    // asm 00004AE9: 	STI	R0,@COMMINTM
    // asm 00004AEA: 	LDP	@CPU_WS
    // asm 00004AEB: 	LDI	1068h,R0
    // asm 00004AEC: 	STI	R0,@CPU_WS
    // asm 00004AED: 	LDP	@FIFO_CONTROL
    // asm 00004AEE: 	LDI	FIFO_CONTROL_INIT,R0
    // asm 00004AEF: 	STI	R0,@FIFO_CONTROL
    // asm 00004AF0: 	LDI	800h,ST			;ENABLE THE CACHE, DISABLE INTERRUPTS
    // asm 00004AF1:         LDP	@FASTSTKI
    // asm 00004AF2:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm 00004AF3: 	DINT				;DISABLE INTERUPTS
    // asm 00004AF9: 	CLRI	R0
    // asm 00004AFA: 	LDP	@CMOS_WP_WORD
    // asm 00004AFB: 	STI	R0,@CMOS_WP_WORD	;DISABLE CMOS WRITES
    // asm 00004AFC: 	LDIL	RAM0,AR0		;CLEARS CMOS_WP_WORD_SHADOW
    // asm 00004AFF: 	RPTS	2047
    // asm 00004B00: 	STI	R0,*AR0++
    // 	;
    // 	;CHECK TO SEE IF THIS IS A WATCHDOG
    // 	;
    // 	;POWERUP IGNORE GLITCH FIX
    // asm 00004B01: 	LDP	@991050h 		;DOG TEST
    // asm 00004B02: 	LDI	@991050h,R0
    // asm 00004B03: 	LDP	@WDHIT
    // asm 00004B04: 	STI	R0,@WDHIT		;SAVE YOUR DOGGIE
    // asm 00004B05: 	LDP	@SYSCNTLR
    // asm 00004B06: 	LDI	SYSCNTL_INIT,R0		;INIT SYSCNTL
    // asm 00004B07: 	STI	R0,@SYSCNTLR
    // asm 00004B08: 	LDP	@DMA_SETUP
    // asm 00004B09: 	LDI	DMA_SETUP_INIT,R0	;INIT DMA
    // asm 00004B0A: 	STI	R0,@DMA_SETUP
    // asm 00004B0B: 	LDP	@FIFO_SIZE
    // asm 00004B0C: 	LDI	270,R0			;INIT FIFO SIZE
    // asm 00004B0D: 	STI	R0,@FIFO_SIZE
    // asm 00004B0E: 	CALL	CRT_REG_SETUP		;INIT CRT
    // asm 00004B0F: 	SETDP
#if MEMTESTS
    // asm 00004B10: 	CALL	TEST_STATIC_CHIPS	;TEST THE INSTALLABLE CHIPS
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
LD_DBG1:
    // asm 00004B11: STI	R0,*AR2++
#endif
    // asm 00004B11: 	LDI	0,AR1			;SOURCE ADDRESS
    // asm 00004B12: 	LDI	4000h,AR2		;DESINATION ADDRESS
    // asm 00004B13: 	LS	8,AR2
    // asm 00004B14: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm 00004B15: 	LS	4,RC			;FAST RAM
    // asm 00004B16: 	RPTB	LD_RAM
    // asm 00004B17: 	LDI	*AR1++,R0
LD_RAM:
    // asm 00004B18: STI	R0,*AR2++
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
    // asm 00004B27: 	SETDP
#if MEMTESTS
    // asm 00004B28: 	CALL	TEST_CHIPS		;TEST THE INSTALLABLE CHIPS
#endif
    // asm 00004B29: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm 00004B2D: 	DINT
    // asm 00004B33:         LDP	@FASTSTKI
    // asm 00004B34:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
#if MEMTESTS == 0
    // 	;For RAM (development) version, we must copy the
    // 	;program into what would be the ROM
    // 	;
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	ANDN	10h,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
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
LD_RAM3:
    // asm 00004B35: STI	R0,*AR2++
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	OR	10h,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	SETDP
#endif
    // asm 00004B35: 	CLRI	R2
    // asm 00004B36: 	SETAUD	ADJ_OUTOFDIAG
    // asm 00004B38:       	BU	DR1 			;SKIP DOGGIE
DIAG_RETURN:
    // asm 00004B39: 	LDI	8,R0	      		;PREVENT FALSE DOGGIE
    // asm 00004B3A: 	LDP	@WDHIT
    // asm 00004B3B: 	STI	R0,@WDHIT		;SAVE YOUR DOGGIE
DR1:
    // asm 00004B3C: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm 00004B40: 	DINT
    // asm 00004B46:         LDP	@FASTSTKI
    // asm 00004B47:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
#if DEBUG
    // asm: 	CALL	VERIFY_CODE_INTEGRITY
#endif
    // 	;LOAD CODE BACK INTO RAM
    // 	;
    // 	;
    // asm 00004B48: 	LDI	040h,AR1		;SOURCE ADDRESS
    // asm 00004B49: 	LDI	0C000h,AR2		;DESINATION ADDRESS
    // asm 00004B4A: 	LS	8,AR2
    // asm 00004B4B: 	ADDI	040h,AR2
    // asm 00004B4C: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm 00004B4D: 	LS	4,RC			;FAST RAM
    // asm 00004B4E: 	RPTB	LD_RAM2
    // asm 00004B4F: 	LDI	*AR2++,R0
LD_RAM2:
    // asm 00004B50: STI	R0,*AR1++
    // asm 00004B51: 	CLRI	R0
    // asm 00004B52: 	LDP	@COLORAM
    // asm 00004B53: 	STI	R0,@COLORAM
    // asm 00004B54: 	SETDP
    // asm 00004B55: 	STI	R0,@DIAG_ACTIVE
    // asm 00004B56: 	CALL	CLR_PBSS
    // asm 00004B57: 	CALL	CLR_RAM			;INIT .BSS TO 0
    // asm 00004B58: 	LDI	MDIAG,R0		;MELLOW THE WHEEL
    // asm 00004B59: 	STI	R0,@_MODE
    // asm 00004B5A: 	LDI	SYSCNTL_INIT,R0		;INIT SYSCNTL RAM SHADOW
    // asm 00004B5B: 	STI	R0,@SYSCNTL
    // asm 00004B5C: 	CALL	FEED_WATCHDOG
    // asm 00004B5D: 	CALL	MESSAGE1		;DISPLAY STARTUP MESSAGE TO SCREEN
    // asm 00004B5E: 	CALL	MSG1
    // asm 00004B5F: 	LDI	240,R0			;X MIN
    // asm 00004B60: 	STI	R0,@PREVX
    // asm 00004B61: 	LDI	1,R0
    // asm 00004B62: 	STI	R0,@DELTA
    // asm 00004B63: 	LDP	@TIMER_CNTL1
    // asm 00004B64: 	LDI	200h|1,R0		;INIT TIMER MUMBO-JUMBO
    // asm 00004B65: 	STI	R0,@TIMER_CNTL1		;
    // asm 00004B66: 	LDI	-1,R0			;
    // asm 00004B67: 	STI	R0,@TIMER_PERIOD1	;
    // asm 00004B68: 	SETDP
    // asm 00004B69: 	LDF	0,R0
    // asm 00004B6A: 	LDI	@TIMERAMI,AR0
    // asm 00004B6B: 	RPTS	47	 		;CLEAR OUT COUNT AREA
    // asm 00004B6C: 	STF	R0,*AR0++
    // asm 00004B6D: 	LDL	5A5A5A5Ah,R0	 	;INIT RANDOM NUMBER SEED
    // asm 00004B6E: 	STI	R0,@RAND
    // asm 00004B6F: 	CALL	LOAD_FIXED_PALETTES	;LOAD BASE PALETTES
    // asm 00004B70: 	CALL	INIT_SYSTEM		;INIT REST OF SYSTEM
    // asm 00004B71: 	CALL	READIO			;GET I/O STATUS (W/INTs OFF)
    // asm 00004B72: 	CALL	CHECKDIAG		;GOTO DIAGNOSTICS PREDICATE
    // asm 00004B73: 	CALL	FIFO_RESET		;CLEAR THE FIFO
    // asm 00004B74: 	CALL	COMM_INIT
    // asm 00004B75: 	CALL	TIMER_RESET
    // asm 00004B76: 	DINT
    // asm 00004B7C: 	CALL	RESET_SNDBRD		;RESET SOUND BOARD (and DB)
    // asm 00004B7D: 	EINT
    // asm 00004B7E: 	CALL	FIFO_RESET
    // asm 00004B7F: 	LDI	INT0_M|INT3_M,IE	;ENABLE INTERRUPTS
    // asm 00004B80: 	AND     0,IF
    // asm 00004B81: 	CALL	ENABLEGIE
    // asm 00004B82: 	CALL	COMM_ENABLE_INT2
    // asm 00004B83: 	CALL	INITIALIZATION_MOTION_CHECK
    // asm 00004B84: 	CLRI	R2
    // asm 00004B85: 	SETAUD	ADJ_OUTOFDIAG
    // asm 00004B87: 	LDI	1,R0			;INIT BASE OF WAVERAM
    // asm 00004B88: 	STI	R0,@HARD_SECTION_LOAD
    // asm 00004B89: 	LDL	_SECshared,AR2
    // asm 00004B8A: 	CALL	LOAD_SECTION_REQ
    // asm 00004B8B: 	LDI	1,R0
    // asm 00004B8C: 	STI	R0,@HARD_SECTION_LOAD
    // asm 00004B8D: 	LDL	_SECskys,AR2
    // asm 00004B8E: 	CALL	LOAD_SECTION_REQ
    // asm 00004B8F: 	CALL	MSG2
    // asm 00004B90: 	LDI	1,R0
    // asm 00004B91: 	STI	R0,@HARD_SECTION_LOAD
    // asm 00004B92: 	STI	R0,@BOOT_PACIFY_SCREEN_P
    // asm 00004B93: 	LDL	_SECgeneral,AR2
    // asm 00004B94: 	CALL	LOAD_SECTION_REQ
    // asm 00004B95: 	LDI	1,R0
    // asm 00004B96: 	STI	R0,@HARD_SECTION_LOAD
    // asm 00004B97: 	STI	R0,@BOOT_PACIFY_SCREEN_P
    // asm 00004B98: 	LDL	_SEChead2head,AR2
    // asm 00004B99: 	CALL	LOAD_SECTION_REQ
    // asm 00004B9A: 	CALL	MSG3
    // asm 00004B9B: 	LDI	1,R0			;LOAD 1ST WAVE
    // asm 00004B9C: 	STI	R0,@HARD_SECTION_LOAD
    // asm 00004B9D: 	STI	R0,@BOOT_PACIFY_SCREEN_P
    // asm 00004B9E: 	LDL	_SECpress,AR2
    // asm 00004B9F: 	CALL	LOAD_SECTION_REQ
    // asm 00004BA0: 	LDI	0,R2
    // asm 00004BA1: 	LDI	AUD_BCREDITS,AR2
    // asm 00004BA2: 	CALL	AUDIT_WRITE
    // asm 00004BA3: 	READADJ	ADJ_VOLUME
    // asm 00004BA5: 	LDI	R0,R1
    // asm 00004BA6: 	CALL	SET_MASTER_VOL
    // asm 00004BA7: 	LDP	@WDHIT			;LOG WATCHDOG HIT
    // asm 00004BA8: 	LDI	@WDHIT,R0
    // asm 00004BA9: 	SETDP
    // asm 00004BAA: 	AND	8,R0 			;CHECK B3
    // asm 00004BAB: 	BNZ	NOPEIT
    // asm 00004BAC: 	INCAUD	AUD_NUM_WATCHDOGS
NOPEIT:
    // asm 00004BAE: 	CLRI	R0
    // asm 00004BAF: 	STI	R0,@_newbut
    // asm 00004BB0: 	LDI	-1,AR2
    // asm 00004BB1: 	STI	AR2,@_ATTR_MODE
    // asm 00004BB2: 	CALL	WAVE			;setup 1st wave
    // asm 00004BB3: 	CALL	FIFO_RESET
    // asm 00004BB4: 	LDI	INT0_M|INT3_M,IE	;ENABLE INTERRUPTS
    // asm 00004BB5: 	LDP	@COMMINTM
    // asm 00004BB6: 	OR	@COMMINTM,IE
    // asm 00004BB7: 	SETDP
    // asm 00004BB8: 	AND     0,IF
    // asm 00004BB9: 	CALL	ENABLEGIE
    // asm 00004BBA: 	READAUD	AUD_VERSION		;version update???
    // asm 00004BBC: 	CMPI	VERSION_ID,R0
    // asm 00004BBD: 	BNE	VERSION_UPDATE
    // asm 00004BBE: 	CALL	VALIDATE_CMOS		;DIAGNOSTIC CHECK ON CMOS
    // asm 00004BBF: 	BC	CMOS_ERROR		;wait till its better
    // asm 00004BC0: 	READAUD	ADJ_ACTUALHSTDRESET	;CHECK FOR HI SCORE RESET
    // asm 00004BC2: 	CMPI	0,R0
    // asm 00004BC3: 	BGT	NODO1
    // asm 00004BC4: 	CALL	INIT_HSTD_TABLES
    // asm 00004BC5: 	READAUD	ADJ_HIGHSCORE_RESET
    // asm 00004BC7: 	LDI	R0,R2
    // asm 00004BC8: 	SETADJ	ADJ_ACTUALHSTDRESET
NODO1:
    // asm 00004BCA: LDI	1,R0
    // asm 00004BCB: 	STI	R0,@NFRAMES
    // asm 00004BCC: 	LDI	0,R0
    // asm 00004BCD: 	STI	R0,@ERRORO
    // asm 00004BCE: 	STI	R0,@ERRORN
    // asm 00004BCF: 	CALL	TIMERESET
    // asm 00004BD0: 	CALL	COMMQ_PACKET_INIT
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_c_int00", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
static void MAINLOOP(void)
{
    // asm 00004BD1: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 00004BD6: 	DMA_WT		R0
    // asm 00004BDB: 	CALL	FIFO_RESET
    // asm 00004BDC: 	CALL	TIMEREC
    // asm 00004BDD: 	CALL	DECOMPRESS_PROC
    // asm 00004BDE: 	CALL	TIMEREC
MWAIT0:
    // asm 00004BDF: LDI	@INFRAMES,R0
    // asm 00004BE0: 	CMPI	@FRAMRATE,R0		;1 FRAME MINIMUM
    // asm 00004BE1: 	BLT	MWAIT0
    // asm 00004BE2: 	CALL	TIMEREC
    // asm 00004BE3: 	CALL	ZSORTWT			;WAIT FOR PAGE WHILE SORTING
    // asm 00004BE4: 	CALL	TIMEREC
MWAIT:
    // asm 00004BE5: LDI	@CLEARRDY,R0		;DONE WHEN INT SIGNALS PAGE SWAPPED
    // asm 00004BE6: 	BNZ	MWAIT
ENTER2:
    // 	;COMMUNICATION XFER
    // 	;
    // asm 00004BE7: 	CALL	COMM_ROUTINE
    // asm 00004BE8: 	CALL	COMMQ_PACKET_INIT
    // asm 00004BE9: 	CALL	DECODE_BUFFER
    // asm 00004BEA: 	LDI	@INFRAMES,R0
    // asm 00004BEB: 	CLRI	R1
    // asm 00004BEC: 	STI	R1,@INFRAMES		;CLEAR INTERRUPT COUNTER
    // asm 00004BED: 	STI	R0,@NFRAMES		;SAVE FOR ALL CURRENT PROCESSES
    // asm 00004BEE: 	CALL	TIMEREC
    // asm 00004BEF: 	STF	R0,@FRAMETIME		;SAVE THE FRAMETIME
    // asm 00004BF0: 	CALL	TIMERESET
    // asm 00004BF1: 	LDI	@_MODE,R4		;FILL FIFO STUFF
    // asm 00004BF2: 	TSTB	MINFIN,R4		;
    // asm 00004BF3: 	CALLNZ	INFINITY
    // asm 00004BF4: 	CALL	TIMEREC
    // asm 00004BF5: 	LDI	@COMM_DRONE_PTR,R0
    // asm 00004BF6: 	CALLNZ	COMM_DRONE_PTR_SORT
    // asm 00004BF7: 	CALL	DIRQ			;DISPLAY THE POLYGONS
    // asm 00004BF8: 	CALL	TIMEREC
    // asm 00004BF9: 	LDI	@_MODE,R4		;FILL FIFO STUFF
    // asm 00004BFA: 	TSTB	MHUD,R4
    // asm 00004BFB: 	BZ	NHUD
    // asm 00004BFC: 	AND	MMODE,R4
    // asm 00004BFD: 	CMPI	MGAME,R4
    // asm 00004BFE: 	BEQ	DOIT8
    // asm 00004BFF: 	CMPI	MBONUS,R4
    // asm 00004C00: 	BNE	NHUD
DOIT8:
    // asm 00004C01: CALL	HUD			;HEADS UP DISPLAY (GAME MODE ONLY)
NHUD:
    // asm 00004C02: 	CALL	TIMEREC
    // asm 00004C03: 	LDI	@_MODE,R4
    // asm 00004C04: 	AND	MMODE,R4		;THIS MASK IS IMPORTANT!!!!
    // asm 00004C05: 	CMPI	MATTR,R4
    // asm 00004C06: 	CALLEQ	INSERT_COINS
    // asm 00004C07: 	CALL	VOLUME_DISPLAY
    // asm 00004C08: 	CALL	TIMEREC
    // asm 00004C09: 	LDI	@_MODE,R0		;MOTION CABINET LEVELING
    // asm 00004C0A: 	AND	MMODE,R4		;IF NOT IN GAME
    // asm 00004C0B: 	CMPI	MGAME,R4
    // asm 00004C0C: 	BEQ	NODO555
    // asm 00004C0D: 	CALL	LEVEL_THE_MOTION
NODO555:
    // asm 00004C0E: 	CALL	CHECKDIAG		;JUMP TO DIAGNOSTICS?
    // asm 00004C0F: 	CALL	SWDISP			;DISPATCH ANY SWITCH PROCESSES
    // asm 00004C10: 	CALL	COLSCC
    // asm 00004C11: 	CLRI	R0
    // asm 00004C12: 	LDP	@MPROC_TIK
    // asm 00004C13: 	STI	R0,@MPROC_TIK
    // asm 00004C14: 	SETDP
    // asm 00004C15: 	CALL	PRC_DISPATCH		;EXECUTE PROCESSES
    // asm 00004C16: 	CALL	COMMQ_READY_TO_SEND
    // asm 00004C17: 	CALL	TEXT_OUTPUT		;END FILL FIFO STUFF
    // asm 00004C18: 	CALL	TIMEREC
    // asm 00004C19: 	LDI	@_MODE,R0
    // asm 00004C1A: 	AND	MMODE,R0
    // asm 00004C1B: 	CMPI	MATTR,R0
    // asm 00004C1C: 	BNE	MAINLOOP
    // 	;we are in attract mode, set appropriately
    // asm 00004C1D: 	LDI	@DIPRAM,R0
    // asm 00004C1E: 	TSTB	CMDP_MASTER,R0
    // asm 00004C1F: 	BZ	MAINLOOP
    // asm 00004C20: 	LDI	@OM_ATTR_MODE,AR2
    // asm 00004C21: 	CMPI	@OLD_OM_ATTR_MODE,AR2
    // asm 00004C22: 	BEQ	MAINLOOP
    // asm 00004C23: 	CMPI	-2,AR2		;WE MAY ONLY SYNC ON THE MIDWAY SPIN
    // asm 00004C24: 	BNE	MAINLOOP
    // asm 00004C25: 	STI	AR2,@OM_ATTR_MODE
    // asm 00004C26: 	STI	AR2,@_ATTR_MODE
    // asm 00004C27: 	CALL	SILENT
    // asm 00004C28:         LDI	@FASTSTKI,SP		;GET PAGE OF STORED ADDRESS
    // asm 00004C29: 	LDI	@_ATTR_MODE,AR2		;AND INTO FP TOO
    // asm 00004C2A: 	CALL	WAVE
    // asm 00004C2B: 	BU	COLD_ENTER
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAINLOOP", 0, 0);
    UNIMPL();
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
void COLD_ENTER(void)
{
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
void ENABLEGIE(void)
{
    // asm 00004C44: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENABLEGIE", 0, 0);
    UNIMPL();
}

void INT0(void)
{
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
    // asm 00004C70: 	LDI	@BGNDCOLA,R0		;SET BACKGROUND COLOR
    // asm 00004C71: 	LDP	@COLORAM
    // asm 00004C72: 	STI	R0,@COLORAM
    // asm 00004C73: 	SETDP				;RESET DP
    // asm 00004C74: 	CALL	COMM_MASTER_SEND_SYNC
    // asm 00004C75: 	CALL	ENABLEGIE
    // asm 00004C76: 	LDI	@_MODE,R0
    // asm 00004C77: 	AND	MMODE,R0
    // asm 00004C78: 	CMPI	MDIAG,R0
    // asm 00004C79: 	BEQ	NTEST
    // asm 00004C7A: 	LDP	@MPROC_TIK
    // asm 00004C7B: 	LDI	@MPROC_TIK,R0
    // asm 00004C7C: 	SETDP
    // asm 00004C7D: 	INC	R0
    // asm 00004C7E: 	CMPI	300,R0
    // asm 00004C7F: 	BLE	REGIT
    // asm 00004C80: 	ERRON	U,EC_WATCHDOG2
    // asm 00004C88: 	BU	_c_int00
REGIT:
    // asm 00004C89: LDP	@MPROC_TIK
    // asm 00004C8A: 	STI	R0,@MPROC_TIK
    // asm 00004C8B: 	SETDP
NTEST:
    // asm 00004C8C: 	CALL	FEED_WATCHDOG
    // asm 00004C8D: 	LDI	@CLEARRDY,R0
    // asm 00004C8E: 	BZ	NCLRSCR
    // asm 00004C8F: 	LDI	0,R0
    // asm 00004C90: 	STI	R0,@CLEARRDY		;RESET SCREEN CLEAR FLAG
    // asm 00004C91: 	STI	R0,@IFRAMES	 	;RESET INTERRUPT FRAME COUNTER
    // asm 00004C92: 	LDI	@MOTION_RCV_TIKS,R0	;MOTION RECEIVE ERROR
    // asm 00004C93: 	DEC	R0
    // asm 00004C94: 	STI	R0,@MOTION_RCV_TIKS
    // asm 00004C95: 	LDI	@NOSWAP,R0		;PAGE SWAP?
    // asm 00004C96: 	BNZ	NCLRSCR
    // asm 00004C97: 	LDI	@DISPLAY_PAGE,R0	;PAGE SWAP FLAG
    // asm 00004C98: 	XOR	1,R0
    // asm 00004C99: 	STI	R0,@DISPLAY_PAGE
    // asm 00004C9A: 	BNZ	PAGE1
    // asm 00004C9B: 	CALL	SETPAGE1
    // asm 00004C9C: 	CALL	FASTCLR0
    // asm 00004C9D: 	B	DN_PAGE
PAGE1:
    // asm 00004C9E: CALL	SETPAGE0
    // asm 00004C9F: 	CALL	FASTCLR1
DN_PAGE:
NCLRSCR:
    // asm 00004CA0: 	LDI	@STOPWATCH_CNTL,R0	;STOPWATCH TIMER
    // asm 00004CA1: 	BZ	NOSTOPWUPDT
    // asm 00004CA2: 	INCM	@STOPWATCH
NOSTOPWUPDT:
    // asm 00004CA5: 	INCM	@_sectime		;ONE SECOND TIMER
    // asm 00004CA8: 	CMPI	TIKS_PER_SECOND,R0
    // asm 00004CA9: 	BLT	NOTASEC
    // asm 00004CAA: 	CLRI	R0
    // asm 00004CAB: 	STI	R0,@_sectime
    // asm 00004CAC: 	LDI	@_MODE,R5
    // asm 00004CAD: 	AND	MMODE,R5
    // asm 00004CAE: 	CMPI	MDIAG,R5
    // asm 00004CAF: 	BEQ	JJ88
    // asm 00004CB0: 	INCAUD	AUD_POWERON_TIME
    // asm 00004CB2: 	CMPI	MATTR,R5
    // asm 00004CB3: 	BEQ	JJ88
    // asm 00004CB4: 	INCAUD	AUD_GAMEON_TIME
    // asm 00004CB6: 	CMPI	MGAME,R5
    // asm 00004CB7: 	BNE	JJ88
    // asm 00004CB8: 	LDI	@CAMVIEW,AR2
    // asm 00004CB9: 	ADDI	AUD_VIEW1_TIME,AR2
    // asm 00004CBA: 	CALL	AUDIT_INC
JJ88:
    // asm 00004CBB: 	LDI	@OFFROAD_TMR,R0	 	;OFFROAD TIMER
    // asm 00004CBC: 	SUBI	1,R0
    // asm 00004CBD: 	LDILT	0,R0
    // asm 00004CBE: 	STI	R0,@OFFROAD_TMR
    // asm 00004CBF: 	LDI	@_countdown,R0		;LO DP TIME REMAINING FOR PLAYER
    // asm 00004CC0: 	BLE	NOTASEC
    // asm 00004CC1: 	LDI	@_MODE,R1		;RACE MUST BE HAPPENING TO DECREMENT COUNT
    // asm 00004CC2: 	TSTB	MGO,R1
    // asm 00004CC3: 	BZ	NOTASEC
    // asm 00004CC4: 	SUBI	1,R0
    // asm 00004CC5: 	STI	R0,@_countdown
NOTASEC:
    // asm 00004CC6: 	LDF	@FLOAT_TIK,R0
    // asm 00004CC7: 	ADDF	@GAME_TIMER,R0
    // asm 00004CC8: 	STF	R0,@GAME_TIMER
    // asm 00004CC9: NOTINGAME
    // asm 00004CC9: 	INCM	@INFRAMES		;increment number of frames passed since last screen switch
    // asm 00004CCC: 	INCMF	@IFRAMES
    // asm 00004CCF: 	CALL	NUWHEEL			;motorized wheel
    // asm 00004CD0: 	CALL	SNDPROC			;sound processor
    // asm 00004CD1: 	CALL	READIO			;read the switches
    // asm 00004CD2: 	CALL	RANDOM			;randomize further...
    // asm 00004CD3: 	CALL	BUTTONS			;lighted buttons routine
    // asm 00004CD4: 	CALL	COIN_COUNTER		;coin counter routine
    // asm 00004CD5: 	CALL	CHECK_STATE
    // asm 00004CD6: 	CALL	LATCH_ERROR		;motion error
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
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INT0", 0, 0);
    UNIMPL();
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

static void READIO(void)
{
    // asm 00004D00: 	CLRI	AR0			;for Loff production board timing problem
    // asm 00004D01: 	LDP	@DIPSW
    // asm 00004D02: 	LDI	@DIPSW,R0
    // asm 00004D03: 	LDI	*AR0,R2
    // asm 00004D04: 	LDP	@DIPRAM
    // asm 00004D05: 	RS	16,R0
    // asm 00004D06: 	STI	R0,@DIPRAM
    // asm 00004D07: 	LDP	@SWITCH3
    // asm 00004D08: 	LDI	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // asm 00004D09: 	LDI	*AR0,R2			;Loff
    // asm 00004D0A: 	LS	16,R0
    // asm 00004D0B: 	LDI	@SWITCH1,R1
    // asm 00004D0C: 	LDI	*AR0,R2			;Loff
    // asm 00004D0D: 	LS	16,R1
    // asm 00004D0E: 	LDP	@FASTSTKI
    // asm 00004D0F: 	RS	16,R1
    // asm 00004D10: 	OR	R1,R0
    // asm 00004D11: 	NOT	R0,R1
    // asm 00004D12: 	LDI	@SWRAMI,AR0
    // asm 00004D13: 	STI	R1,@SWITCHBUTS
    // 	;
    // 	;SCAN SWITCHES, TRIGGER ON LEADING EDGE
    // 	;
    // asm 00004D14: 	AND	*AR0,*+AR0(1),R2	;PREVIOUS TWO OPEN
    // asm 00004D15: 	AND	R1,R2			;CURRENT ONE CLOSED
    // asm 00004D16: 	OR	*+AR0(2),R2		;OR INTO EDGE LIST
    // asm 00004D17: 	STI	R2,*+AR0(2)		;STORE EDGE WORD 1=EDGE TRIGGERED
    // asm 00004D18: 	LDI	*AR0,R1			;GET N-1
    // asm 00004D19: 	STI	R1,*+AR0(1)		;N-1 --> N-2
    // asm 00004D1A: 	STI	R0,*AR0			;N --> N-1
    // asm 00004D1B: 	PUSH	DP
    // asm 00004D1C: 	LDI	@SYSCNTL,R0	      	;actually we signal a read of the
    // asm 00004D1D: 	LDP	@SYSCNTLR		;have a value
    // asm 00004D1E: 	ANDN	ATOD_WR,R0	      	;pot(s), they interrupt us when they
    // asm 00004D1F: 	STI	R0,@SYSCNTLR		;have a value
    // asm 00004D20: 	POP	DP
    // asm 00004D21: 	CLRI	AR0
    // asm 00004D22: 	LDP	@ATOD_R
    // asm 00004D23: 	LDI	04h,R0
    // asm 00004D24: 	LS	24,R0
    // asm 00004D25: 	STI	R0,@ATOD_R
    // asm 00004D26: 	LDI	*AR0,R2
    // asm 00004D27: 	SETDP
    // asm 00004D28: 	CLRI	R0
    // asm 00004D29: 	STI	R0,@RDPOT
    // 	;THESE ARE SPECIAL ROUTINES WHICH ARE CALLED, *NOT* CREATED
    // 	;FOR PROCESSES.
    // 	;	THIS INCLUDES:
    // 	;		VOL+
    // 	;		VOL-
    // 	;		DIAG BUTTON
    // 	;NO OTHER ROUTINES SHOULD BE HERE
    // asm 00004D2A: 	LDI	@SWRAM+2,R0
    // asm 00004D2B: 	TSTB	SW_DIAG,R0
    // asm 00004D2C: 	CALLNZ	DIAG_TOGGLE
    // asm 00004D2D: 	LDI	@_MODE,R0
    // asm 00004D2E: 	AND	MMODE,R0
    // asm 00004D2F: 	CMPI	MATTR,R0
    // asm 00004D30: 	BEQ	NIGY
    // asm 00004D31: 	CMPI	MDIAG,R0
    // asm 00004D32: 	BEQ	NIGY
    // asm 00004D33: 	LDI	@SWITCHBUTS,R0
    // asm 00004D34: 	TSTB	SW_VOLPLUS,R0
    // asm 00004D35: 	CALLNZ	VOL_PLUS
    // asm 00004D36: 	TSTB	SW_VOLMINUS,R0
    // asm 00004D37: 	CALLNZ	VOL_MINUS
NIGY:
    // asm 00004D38: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "READIO", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void VOL_MINUS(void)
{
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

static void VOL_PLUS(void)
{
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

static void DIAG_TOGGLE(void)
{
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
static void ATODINT(void)
{
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
static void SWDISP(void)
{
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
    UNIMPL();
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
static void *SWTAB[] = {
    COIN1, // 00000001 SW_COIN1	(COIN.ASM)
    COIN2, // 00000002 SW_COIN2 	(COIN.ASM)
    _start, // 00000004 START		(INTRO.ASM)
    0, // 00000008
    0, // 00000010 SW_DIAG	(DIAG.ASM)
    0, // 00000020
    SERV_COIN, // 00000040 SW_COINSRV	(COIN.ASM)
    COIN3, // 00000080
    0, // 00000100
    0, // 00000200
    0, // 00000400
    0, // 00000800
    0, // 00001000
    0, // 00002000
    COIN4, // 00004000
    0, // 00008000
    0, // 00010000 BRAKE
    RADIO_BUT, // 00020000 RADIO (OLD ABORT)
    0, // 00040000 LOW
    _debug, // 00080000 DEBUG
    _VIEW0, // 00100000 VIEW0
    _VIEW1, // 00200000 VIEW1
    _VIEW2, // 00400000 VIEW2
    0, // 00800000 VIEW4
    0, // 01000000
    0, // 02000000
    0, // 04000000
    0, // 08000000
    0, // 10000000
    0, // 20000000
    0, // 40000000
    0, // 80000000
    // ----------------------------------------------------------------------------
};

// *----------------------------------------------------------------------------
void CHECKDIAG(void)
{
    // asm 00004E2C: 	LDI	@DIAG_ACTIVE,R0
    // asm 00004E2D: 	RETSZ
    // asm 00004E2E: 	BR	ENTER_DIAG
    // ;	RETS
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKDIAG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

static void CLR_PBSS(void)
{
    // asm 00004E31: 	PUSH	R0
    // asm 00004E32: 	PUSH	AR0
    // asm 00004E33: 	PUSH	RC
    // asm 00004E34: 	PUSH	RS
    // asm 00004E35: 	PUSH	RE
    // asm 00004E36: 	LDI	@PBSS_PTR,AR0
    // asm 00004E37: 	LDI	@PBSS_BSSEND,RC
    // asm 00004E38: 	SUBI	AR0,RC
    // asm 00004E39: 	SUBI	1,RC
    // asm 00004E3A: 	CLRI	R0
    // asm 00004E3B: 	RPTB	PRAMCLP
PRAMCLP:
    // asm 00004E3C: STI	R0,*AR0++
    // asm 00004E3D: 	CALL	CLEAR_LINK
    // asm 00004E3E: 	POP	RE
    // asm 00004E3F: 	POP	RS
    // asm 00004E40: 	POP	RC
    // asm 00004E41: 	POP	AR0
    // asm 00004E42: 	POP	R0
    // asm 00004E43: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR_PBSS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// ;RAM_BSSEND	.word	01F7FFh
/* asm: RAM_BSSEND	.word	01EFFFh		;save protected hi bss ram */
/* asm: 	 */
static int RAM_BSSEND = 0x01EFFF;

void CLR_RAM(void)
{
    // asm 00004E46: 	PUSH	R0
    // asm 00004E47: 	PUSH	AR0
    // asm 00004E48: 	PUSH	RC
    // asm 00004E49: 	PUSH	RS
    // asm 00004E4A: 	PUSH	RE
    // asm 00004E4B: 	LDI	@RAM_PTR,AR0
    // asm 00004E4C: 	LDI	@RAM_BSSEND,RC
    // asm 00004E4D: 	SUBI	AR0,RC
    // asm 00004E4E: 	SUBI	1,RC
    // asm 00004E4F: 	CLRI	R0
    // asm 00004E50: 	RPTB	RAMCLP
RAMCLP:
    // asm 00004E51: STI	R0,*AR0++
    // asm 00004E52: 	POP	RE
    // asm 00004E53: 	POP	RS
    // asm 00004E54: 	POP	RC
    // asm 00004E55: 	POP	AR0
    // asm 00004E56: 	POP	R0
    // asm 00004E57: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR_RAM", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*THESE MUST BE CALLED *BEFORE* PROGRAM IS COPIED INTO FAST RAM
*
*/
static void CLEAR_ONCHIPRAM(void)
{
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
static void BUTTONS(void)
{
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
    UNIMPL();
}

static void DIAG_BUTTONS(void)
{
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
void WAIT_FOR_VBLANK(void)
{
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
    399|CRT_SETUP_ICSYNC, // CRT_SETUP
    0x01ff, // CRT_HADDRINC
    0x01fe, // CRT_HBLKSTART
    0x020e, // CRT_HSYNCSTART
    0x0227, // CRT_HSYNCEND
    0x0299, // CRT_HBLKEND
    0x029a, // CRT_HTTL
    0x018e, // CRT_VBLKSTART
    0x0191, // CRT_SYNCSTART
    0x0194, // CRT_SYNCEND
    0x01af, // CRT_VBLK
    0x01b0, // CRT_VTTL
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

static void CRT_REG_SETUP(void)
{
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
static void ERROR_TRAP(void)
{
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
void FIFO_RESET(void)
{
    // asm 00004EF4: 	DINT
    // asm 00004EFA: 	LDP	@SYSCNTL
    // asm 00004EFB: 	LDI	@SYSCNTL,R0		;READ SHADOW LOCATION
    // asm 00004EFC: 	LDI	R0,R1
    // asm 00004EFD: 	ANDN	1,R1			;MASK OUT FIFORS
    // asm 00004EFE: 	LDP	@SYSCNTLR
    // asm 00004EFF: 	STI	R1,@SYSCNTLR		;STROBE SYSTEM
    // asm 00004F00: 	STI	R0,@SYSCNTLR		;THEN BACK TO NORMAL
    // asm 00004F01: 	EINT
    // asm 00004F02: 	LDP	@FIFO_CONTROL		;RESET COUNTER ON FIFO  (SAFETY FEATURE)
    // asm 00004F03: 	LDI	FIFO_CONTROL_DMA_RUNSEL|FIFO_CONTROL_FIFO_RST,R0
    // asm 00004F04: 	STI	R0,@FIFO_CONTROL
    // asm 00004F05: 	NOP
    // asm 00004F06: 	LDI	FIFO_CONTROL_DMA_RUNSEL,R0
    // asm 00004F07: 	STI	R0,@FIFO_CONTROL
    // asm 00004F08: 	SETDP
    // asm 00004F09: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIFO_RESET", 0, 0);
    UNIMPL();
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
void TIMER_RESET(void)
{
    // asm 00004F0A: 	PUSH	DP
    // asm 00004F0B: 	PUSH	R0
    // asm 00004F0C: 	LDP	@TIMER_CNTL1
    // asm 00004F0D: 	LDI	@TIMER_CNTL1,R0
    // asm 00004F0E: 	OR	TMR_RESET_GO,R0
    // asm 00004F0F: 	STI	R0,@TIMER_CNTL1
    // asm 00004F10: 	POP	R0
    // asm 00004F11: 	POP	DP
    // asm 00004F12: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMER_RESET", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void TIMER_READ(void)
{
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
int TIMEX;
/* asm: TIMECLR	.bss	TIMECLR,1 */
int TIMECLR;
/* asm: TIMERAM	.bss	TIMERAM,50 */
int TIMERAM[50];

// *----------------------------------------------------------------------------
static void TIMERESET(void)
{
    // asm 00004F19: 	PUSH	DP
    // asm 00004F1A: 	CALL	TIMER_RESET
    // asm 00004F1B: 	SETDP
    // asm 00004F1C: 	LDI	@TIMERAMI,AR0
    // asm 00004F1D: 	STI	AR0,@TIMEX
    // asm 00004F1E: 	LDI	@TIMECLR,R0
    // asm 00004F1F: 	BNZ	TIMEL1
    // asm 00004F20: 	LDF	@TIMEFRAME,R0
    // asm 00004F21: 	ADDF	1,R0
    // asm 00004F22: 	STF	R0,@TIMEFRAME
    // asm 00004F23: 	CALL	INV_F30
    // asm 00004F24: 	LDI	@TIMERAMI,AR0
    // asm 00004F25: 	STI	AR0,@TIMEX
    // asm 00004F26: 	LDI	15,RC
    // asm 00004F27: 	RPTB	TIMELP
    // asm 00004F28: 	LDF	*AR0++,R1
    // asm 00004F29: 	ADDF	*+AR0(15),R1
    // asm 00004F2A: 	STF	R1,*+AR0(15)
    // asm 00004F2B:  	MPYF	R0,R1
TIMELP:
    // asm 00004F2C: STF	R1,*+AR0(31)
    // asm 00004F2D: 	POP	DP
    // asm 00004F2E: 	RETS
TIMEL1:
    // asm 00004F2F: 	LDI	0,R0
    // asm 00004F30: 	STI	R0,@TIMECLR
    // asm 00004F31: 	LDF	0,R0
    // asm 00004F32: 	STF	R0,@TIMEFRAME
    // asm 00004F33: 	LDI	@TIMERAMI,AR0
    // asm 00004F34: 	RPTS	47	 		;CLEAR OUT COUNT AREA
    // asm 00004F35: 	STF	R0,*AR0++
    // asm 00004F36: 	POP	DP
    // asm 00004F37: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMERESET", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void TIMEREC(void)
{
    // asm 00004F38: 	PUSH	DP
    // asm 00004F39: 	PUSH	AR0
    // asm 00004F3A: 	LDP	@TIMER_CNTR1
    // asm 00004F3B: 	FLOAT	@TIMER_CNTR1,R0
    // asm 00004F3C: 	LDP	@TIMEX
    // asm 00004F3D: 	LDI	@TIMEX,AR0
    // asm 00004F3E: 	STF	R0,*AR0++ 	;SAVE THE INDEX
    // asm 00004F3F: 	STI	AR0,@TIMEX
    // asm 00004F40: 	POP	AR0
    // asm 00004F41: 	POP	DP
    // asm 00004F42: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMEREC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void MESSAGE1(void)
{
    // asm 00004F43: 	CLRI	R0
    // asm 00004F44: 	LDP	@9E0000h
    // asm 00004F45: 	STI	R0,@9E0000h
    // asm 00004F46: 	LDI	-1,R0
    // asm 00004F47: 	STI	R0,@9E0001h		;white
    // asm 00004F48: 	LDI	06000h,R0
    // asm 00004F49: 	STI	R0,@9E0003h		;red
    // asm 00004F4A: 	LDI	0294Ah,R0
    // asm 00004F4B: 	STI	R0,@9E000Bh		;grey
    // asm 00004F4C: 	SETDP
    // asm 00004F4D: 	CALL	CLRSCRN
    // asm 00004F4E: 	CALL	SETPAGE0
    // asm 00004F4F: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 00004F52: 	STI	R0,@ACTIVE_SCREEN
    // asm 00004F53: 	LDI	1,RC
    // asm 00004F54: 	TEXTIT	startup0,1,40
    // asm 00004F58: 	TEXTIT	startup1,1,70
    // asm 00004F5C: 	TEXTIT	startup2,1,80
    // asm 00004F60: 	TEXTIT	startup3,1,90
    // asm 00004F64: 	LDI	11,RC
    // asm 00004F65: 	TEXTIT	dlts,40,110
    // asm 00004F69: 	LDI	1,RC
    // asm 00004F6A: 	TEXTIT	VERSION_STAMP,40,130
    // asm 00004F6E: 	TEXTIT	INTERNAL_VERS,260,150
    // asm 00004F72: 	TEXTIT	DATE_STAMP,40,150
    // asm 00004F76: 	CLRI	AR0
    // asm 00004F77: 	LDP	@DIPSW
    // asm 00004F78: 	LDI	@DIPSW,R0
    // asm 00004F79: 	LDI	*AR0,AR0
    // asm 00004F7A: 	RS	16,R0
    // asm 00004F7B: 	SETDP
    // asm 00004F7C: 	TSTB	DIP_COMMP,R0
    // asm 00004F7D: 	BZ	DODOIBO
    // asm 00004F7E: 	LDI	@LINKDISABLED,AR2
    // asm 00004F7F: 	BU	HJSADF
DODOIBO:
    // asm 00004F80: TSTB	CMDP_MASTER,R0
    // asm 00004F81: 	LDIZ	@IAMMASTER,AR2
    // asm 00004F82: 	LDINZ	@IAMSLAVE,AR2
HJSADF:
    // asm 00004F83: LDI	40,R2
    // asm 00004F84: 	LDI	160,R3
    // asm 00004F85: 	CALL	_outtextxyc
    // 	;test if link pal is installed
    // 	;
    // 	;
    // asm 00004F86: 	CLRI	AR0
    // asm 00004F87: 	LDP	@COMMPAL
    // asm 00004F88: 	LDI	@COMMPAL,R0
    // asm 00004F89: 	LDI	*AR0,R1
    // asm 00004F8A: 	SETDP
    // asm 00004F8B: 	AND	0FH,R0
    // asm 00004F8C: 	CMPI	4,R0			;BAD PAL ?
    // asm 00004F8D: 	LDIZ	@TPALI,AR2
    // asm 00004F8E: 	LDINZ	@TPALNI,AR2
    // asm 00004F8F: 	LDI	40,R2
    // asm 00004F90: 	LDI	170,R3
    // asm 00004F91: 	LDI	1,RC
    // asm 00004F92: 	CALL	_outtextxyc
    // 	;
    // 	;
    // asm 00004F93: 	LDI	239,AR2
    // asm 00004F94: 	LDI	110,R2
    // asm 00004F95: 	LDI	301,R3
    // asm 00004F96: 	LDI	117,RC
    // asm 00004F97: 	LDI	11,RS
    // asm 00004F98: 	CALL	_rectangle
    // asm 00004F99: 	LDI	11,RC
    // asm 00004F9A: 	TEXTIT	M1,1,190
    // asm 00004F9E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MESSAGE1", 0, 0);
    UNIMPL();
}

/* asm: MSG_CNT	.bss	MSG_CNT,1 */
static int MSG_CNT;

static void MSG1(void)
{
    // asm 00004F9F: LDI	11,RC
    // asm 00004FA0: 	TEXTIT	M2,1,200
    // asm 00004FA4: 	TEXTIT	M3,1,220
    // asm 00004FA8: 	TEXTIT	M4,1,230
    // asm 00004FAC: 	TEXTIT	M5,1,240
    // asm 00004FB0: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSG1", 0, 0);
    UNIMPL();
}

static void MSG2(void)
{
    // asm 00004FB1: LDI	11,RC
    // asm 00004FB2: 	TEXTIT	M6,1,260
    // asm 00004FB6: 	TEXTIT	M7,1,270
    // asm 00004FBA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSG2", 0, 0);
    UNIMPL();
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

static void MSG3(void)
{
    // asm 00004FC0: LDI	11,RC
    // asm 00004FC1: 	TEXTIT	M8,1,280
    // asm 00004FC5: 	TEXTIT	M9,1,290
    // asm 00004FC9: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSG3", 0, 0);
    UNIMPL();
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
    SW_VIEW0|SW_VIEW2,
    SW_VIEW2,
    SW_VIEW1|SW_VIEW2,
    SW_VIEW1,
    SW_VIEW0|SW_VIEW1,
    SW_VIEW0,
    SW_VIEW0|SW_RADIO,
};
// *
/* asm: STATE_MASK	.word	SW_VIEW0|SW_VIEW1|SW_VIEW2|SW_RADIO */
static int STATE_MASK = SW_VIEW0|SW_VIEW1|SW_VIEW2|SW_RADIO;
/* asm: STATE_NUM	.bss	STATE_NUM,1 */
int STATE_NUM;
/* asm: STATE_TIK	.bss	STATE_TIK,1 */
int STATE_TIK;

/*
*
*
*
*/
static void CHECK_STATE(void)
{
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
    UNIMPL();
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
    BUT_VIEW1, BUT_VIEW2, BUT_VIEW3, BUT_VIEW2,
};

// *
static void DASHLIGHT(void)
{
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
static void CMOS_ERROR(void)
{
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
static void VERSION_UPDATE(void)
{
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

void FAKEDIAG(void)
{
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
void FEED_WATCHDOG(void)
{
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
    // asm 00005059: 	LDP	@SYSCNTLR
    // asm 0000505A: 	STI	R0,@SYSCNTLR
    // asm 0000505B: 	POP	R0
    // asm 0000505C: 	LDP	@CPU_WS
    // asm 0000505D: 	STI	R0,@CPU_WS
    // asm 0000505E: 	POP	R0
    // asm 0000505F: 	POP	DP
    // asm 00005060: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FEED_WATCHDOG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: SYSCNTL_OC	fbss	SYSCNTL_OC,1 */
static int SYSCNTL_OC;

void FEED_WATCHDOG_HARD(void)
{
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

void VERIFY_CODE_INTEGRITY(void)
{
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

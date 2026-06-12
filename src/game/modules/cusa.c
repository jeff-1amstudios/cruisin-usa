#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/mproc.h"
#include "../include/macs.h"
#include "../include/obj.h"
#include "../include/objects.h"
#include "../include/globals.h"
#include "../include/sys.h"
#include "../include/c30.h"
#include "../include/text.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/sysid.h"
#include "../include/dirq.h"
#include "../include/error.h"
#include "../include/comm.h"
#include "../include/checksum.h"
#include "../include/cusa.h"
#include "../include/bssstart.h"

/*
 * Source module: asm/CUSA.ASM
 */

// *----------------------------------------------------------------------------
// *			CRUIS'N USA
// *
// *
// *	EUGENE JARVIS			DIRECTOR
// *
// *	ERIC PRIBYL			SOFTWARE
// *
// *	MARK LOFFREDO			HARDWARE
// *	GLENN SHIPP			HARDWARE
// *
// *	TED BARBER			GRAPHICS
// *	SHAWN COOPER			GRAPHICS
// *
// *	MATT BOOTY			SOUND FX
// *	VINCE PONTARELLI		MUSIC
// *
// *
// *	COPYRIGHT (C) 1994 NINTENDO, DEVELOPED BY TV GAMES, INC.
// *	MANUFACTURED AND SOLD BY MIDWAY MANUFACTURING COMPANY UNDER LICENSE
// *
// *
#define MEMTESTS 1
#define TIKS_PER_SECOND 57
/* asm: COMMINTM	fbss	COMMINTM,1 */
int COMMINTM;
// 				;INT2_M  if linked
#if 0
const char *startup0 = "DO NOT EVEN CONSIDER RELEASING THESE ROMS";
const char *startup2 = "   THIS VERSION HAS A FATAL BUG";
const char *dlts = "TEST VERSION";
const char *VERSION_STAMP = "VERSION NOT FOR RELEASE";
#else
const char *startup0 = "          CRUISN USA (TM)     HEAD 2 HEAD";
const char *startup1 = "  (C) 1994 NINTENDO, DEVELOPED BY TV GAMES, INC.";
const char *startup2 = "MANUFACTURED AND SOLD BY MIDWAY MANUFACTURING COMPANY";
const char *startup3 = "                  UNDER LICENSE.";
const char *dlts = "DOWNLOADING TEXTURES";
// ;Release of linked version will be 4.0
// ;	I = I4000
// ;	VID = 40
// ;
const char *VERSION_STAMP = "VERSION  4.4";
#endif
const char *INTERNAL_VERS = "I440";
#define VERSION_ID 40
// 	;Because of managements inability to deal with software projects,
// 	;or being able to keep the version control straight
// 	;we have now moved into Internal and External versions of the program
// 	;VERSION_STAMP contains whatever the management wants to call it.
// 	;If management asks call it 'version' or 'release'.
// 	;'actual version' or 'internal version' or 'internal' is what
// 	;to use for us.
// 	;INTERNAL_VERS is the actual version of the software.
// 	;Keep in mind that multiple releases exist under the external
// 	;version name, and the true software version can be gotten from
// 	;the title page.
const char *M1 = "LOADING OS-WMS...";
const char *M2 = "I-NODES ALLOCATED,  DCS DECODED (14-1715)";
const char *M3 = "WMS SATELLITE COMM, CHANNEL 3 ACTIVE LYBIRP";
const char *M4 = "USR/ELP/CUSA>NETLINK WMS 14.32.86.1 -K CIRE";
const char *M5 = "CONNECTING TO HOST...";
const char *M6 = "CONNECTION ESTABLISHED, PUBLIC-KEY: CIRE";
const char *M7 = "USR/ELP/CUSA>FTP GET CUSA";
const char *M8 = "37940813 BYTES RECEIVED OK";
const char *M9 = "USR/ELP/CUSA>TV30 CUSA /L";
// 	;*** RAM DEFINITIONS
/* asm: _MODE	pbss	_MODE,1 */
int _MODE;
/* asm: STATE	pbss	STATE,1 */
int STATE;
/* asm: NFRAMES	.bss	NFRAMES,1 */
int NFRAMES;
/* asm: INFRAMES	.bss	INFRAMES,1 */
int INFRAMES;
/* asm: FRAMRATE	.bss	FRAMRATE,1 */
int FRAMRATE;
/* asm: IFRAMES	.bss	IFRAMES,1 */
int IFRAMES;
/* asm: OFRAMES	.bss	OFRAMES,1 */
int OFRAMES;
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
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *READ IO  SWITCHES AND POTS
// *
// *
/* asm: _newbut	pbss	_newbut,0 */
int _newbut;
/* asm: SWRAM	pbss	SWRAM,3 */
int SWRAM[3];
/* asm: DIPRAM	pbss	DIPRAM,1 */
int DIPRAM;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *SWITCH ROUTINE TABLE
// *0 = NO PROCESS TO CREATE
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
/* asm: 	*---------------------------------------------------------------------------- */
/* asm: 	*---------------------------------------------------------------------------- */
int SWTAB[] = {
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
};
// *----------------------------------------------------------------------------
/* asm: PB1	.usect	pbsss,1 */
int PB1[pbsss];
/* asm: PB2	.usect	pbsse,1 */
int PB2[pbsse];
/* asm: PBSS_PTR	.word	PB1 */
int PBSS_PTR = PB1;
/* asm: PBSS_BSSEND	.word	PB2 */
int PBSS_BSSEND = PB2;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *CLR RAM  (SLOW RAM)
// *make sure this is *NOT* done after C initializations
// *make sure nothing exists on the stack yet
// *
// *
/* asm: RAM_PTR	.word	BSSSTART */
int RAM_PTR = BSSSTART;
// ;RAM_BSSEND	.word	01F7FFh
/* asm: RAM_BSSEND	.word	01EFFFh		;save protected hi bss ram */
int RAM_BSSEND = 0x01EFFF;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *
#define SPACER 20
/* asm: BUTTON_TIKS	fbss	BUTTON_TIKS,1 */
int BUTTON_TIKS;
/* asm: BUTTON_STATUS	pbss	BUTTON_STATUS,1 */
int BUTTON_STATUS;
/* asm: OLD_BUTTON_STATUS	.bss	OLD_BUTTON_STATUS,1 */
int OLD_BUTTON_STATUS;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: CRT_REG_SETUP_STR */
/* asm: 	.word	399|CRT_SETUP_ICSYNC	;CRT_SETUP */
/* asm: 	.word	01ffh		;CRT_HADDRINC */
/* asm: 	.word	01feh		;CRT_HBLKSTART */
/* asm: 	.word	020eh		;CRT_HSYNCSTART */
/* asm: 	.word	0227h		;CRT_HSYNCEND */
/* asm: 	.word	0299h		;CRT_HBLKEND */
/* asm: 	.word	029ah		;CRT_HTTL */
/* asm: 	.word	018eh		;CRT_VBLKSTART */
/* asm: 	.word	0191h		;CRT_SYNCSTART */
/* asm: 	.word	0194h		;CRT_SYNCEND */
/* asm: 	.word	01afh		;CRT_VBLK */
/* asm: 	.word	01b0h		;CRT_VTTL */
/* asm: 	;before syncing */
/* asm: 	;	.word	400|CRT_SETUP_ICSYNC	;CRT_SETUP */
/* asm: 	;	.word	01ffh		;CRT_HADDRINC */
/* asm: 	;	.word	01feh		;CRT_HBLKSTART */
/* asm: 	;	.word	020eh		;CRT_HSYNCSTART */
/* asm: 	;	.word	0227h		;CRT_HSYNCEND */
/* asm: 	;	.word	0299h		;CRT_HBLKEND */
/* asm: 	;	.word	029ah		;CRT_HTTL */
/* asm: 	;	.word	018fh		;CRT_VBLKSTART */
/* asm: 	;	.word	0192h		;CRT_SYNCSTART */
/* asm: 	;	.word	0195h		;CRT_SYNCEND */
/* asm: 	;	.word	01b0h		;CRT_VBLK */
/* asm: 	;	.word	01b0h		;CRT_VTTL */
int CRT_REG_SETUP_STR[] = {
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
};
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *INTERRUPT VECTORS
// *
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
/* asm: TIMEFRAME	.bss	TIMEFRAME,1 */
int TIMEFRAME;
/* asm: TIMEX	.bss	TIMEX,1 */
int TIMEX;
/* asm: TIMECLR	.bss	TIMECLR,1 */
int TIMECLR;
/* asm: TIMERAM	.bss	TIMERAM,50 */
int TIMERAM[50];
#define COMMPAL 0x0990000 //COMMUNICATIONS PAL
// 	;
// 	;
/* asm: MSG_CNT	.bss	MSG_CNT,1 */
int MSG_CNT;
/* asm: LINKDISABLED	SPTR	"LINK DISABLED BY U97  DIP6 OFF" */
const char *LINKDISABLED = "LINK DISABLED BY U97  DIP6 OFF";
/* asm: IAMMASTER	SPTR	"LINK MASTER MACHINE" */
const char *IAMMASTER = "LINK MASTER MACHINE";
/* asm: IAMSLAVE	SPTR	"LINK SLAVE MACHINE" */
const char *IAMSLAVE = "LINK SLAVE MACHINE";
/* asm: TPALI		SPTR	"U38 LINK PAL INSTALLED" */
const char *TPALI = "U38 LINK PAL INSTALLED";
/* asm: TPALNI		SPTR	"U38 LINK PAL NOT INSTALLED" */
const char *TPALNI = "U38 LINK PAL NOT INSTALLED";
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *SECRET OCTOPUS
// *
// *
#define NUM_STATES 7
/* asm: STATE_TABLE	.word	SW_VIEW0|SW_VIEW2 */
/* asm: 	.word	SW_VIEW2 */
/* asm: 	.word	SW_VIEW1|SW_VIEW2 */
/* asm: 	.word	SW_VIEW1 */
/* asm: 	.word	SW_VIEW0|SW_VIEW1 */
/* asm: 	.word	SW_VIEW0 */
/* asm: 	.word	SW_VIEW0|SW_RADIO */
int STATE_TABLE[] = {
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
int STATE_MASK = SW_VIEW0|SW_VIEW1|SW_VIEW2|SW_RADIO;
// *
/* asm: STATE_NUM	.bss	STATE_NUM,1 */
int STATE_NUM;
/* asm: STATE_TIK	.bss	STATE_TIK,1 */
int STATE_TIK;
// *----------------------------------------------------------------------------
/* asm: BUTTON_IBO	.bss	BUTTON_IBO,1 */
int BUTTON_IBO;
/* asm: BUTTON_TIK	.bss	BUTTON_TIK,1 */
int BUTTON_TIK;
// *----------------------------------------------------------------------------
/* asm: BUTTONI	.word	BUTTII */
#define BUTTONI BUTTII
/* asm: BUTTII	.word	BUT_VIEW1,BUT_VIEW2,BUT_VIEW3,BUT_VIEW2 */
int BUTTII[] = {
    BUT_VIEW1, BUT_VIEW2, BUT_VIEW3, BUT_VIEW2,
};
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: SYSCNTL_OC	fbss	SYSCNTL_OC,1 */
int SYSCNTL_OC;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// 	;
// 	;verify that the code has not been corrupted
// 	;
// 	;*this only works in RAM
// 	;and, if used in ROM will cause a lockup
// 	;
#if CODE_CHECK
#endif
#endif
// *----------------------------------------------------------------------------

void _c_int00(void)
{
    // asm: 	LDI	02h,IOF			;TV30 KLUDGE
    // asm: 	CLRI	R0			;must fix COMM setup asap
    // asm: 	LDP	@COMMINTM
    // asm: 	STI	R0,@COMMINTM
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	1068h,R0
    // asm: 	STI	R0,@CPU_WS
    // asm: 	LDP	@FIFO_CONTROL
    // asm: 	LDI	FIFO_CONTROL_INIT,R0
    // asm: 	STI	R0,@FIFO_CONTROL
    // asm: 	LDI	800h,ST			;ENABLE THE CACHE, DISABLE INTERRUPTS
    // asm:         LDP	@FASTSTKI
    // asm:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
    // asm: 	DINT				;DISABLE INTERUPTS
    // asm: 	CLRI	R0
    // asm: 	LDP	@CMOS_WP_WORD
    // asm: 	STI	R0,@CMOS_WP_WORD	;DISABLE CMOS WRITES
    // asm: 	LDIL	RAM0,AR0		;CLEARS CMOS_WP_WORD_SHADOW
    // asm: 	RPTS	2047
    // asm: 	STI	R0,*AR0++
    // 	;
    // 	;CHECK TO SEE IF THIS IS A WATCHDOG
    // 	;
    // 	;POWERUP IGNORE GLITCH FIX
    // asm: 	LDP	@991050h 		;DOG TEST
    // asm: 	LDI	@991050h,R0
    // asm: 	LDP	@WDHIT
    // asm: 	STI	R0,@WDHIT		;SAVE YOUR DOGGIE
    // asm: 	LDP	@SYSCNTLR
    // asm: 	LDI	SYSCNTL_INIT,R0		;INIT SYSCNTL
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	LDP	@DMA_SETUP
    // asm: 	LDI	DMA_SETUP_INIT,R0	;INIT DMA
    // asm: 	STI	R0,@DMA_SETUP
    // asm: 	LDP	@FIFO_SIZE
    // asm: 	LDI	270,R0			;INIT FIFO SIZE
    // asm: 	STI	R0,@FIFO_SIZE
    // asm: 	CALL	CRT_REG_SETUP		;INIT CRT
    // asm: 	SETDP
#if MEMTESTS
    // asm: 	CALL	TEST_STATIC_CHIPS	;TEST THE INSTALLABLE CHIPS
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
    // asm: LD_DBG1	STI	R0,*AR2++
#endif
    // asm: 	LDI	0,AR1			;SOURCE ADDRESS
    // asm: 	LDI	4000h,AR2		;DESINATION ADDRESS
    // asm: 	LS	8,AR2
    // asm: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm: 	LS	4,RC			;FAST RAM
    // asm: 	RPTB	LD_RAM
    // asm: 	LDI	*AR1++,R0
    // asm: LD_RAM	STI	R0,*AR2++
    // asm: 	NOP
    // asm: 	NOP				;DELAY FOR PIPELINE
    // asm: 	NOP
    // asm: 	LDI	22h,IOF
    // asm: 	NOP
    // asm: 	NOP				;DELAY FOR PIPELINE
    // asm: 	NOP
    // asm: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm: 	CLRI	R0
    // asm: 	LDP	@COMMINTM
    // asm: 	STI	R0,@COMMINTM
    // asm: 	SETDP
#if MEMTESTS
    // asm: 	CALL	TEST_CHIPS		;TEST THE INSTALLABLE CHIPS
#endif
    // asm: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm: 	DINT
    // asm:         LDP	@FASTSTKI
    // asm:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
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
    // asm: LD_RAM3	STI	R0,*AR2++
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	OR	10h,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	SETDP
#endif
    // asm: 	CLRI	R2
    // asm: 	SETAUD	ADJ_OUTOFDIAG
    // asm:       	BU	DR1 			;SKIP DOGGIE
DIAG_RETURN:
    // asm: 	LDI	8,R0	      		;PREVENT FALSE DOGGIE
    // asm: 	LDP	@WDHIT
    // asm: 	STI	R0,@WDHIT		;SAVE YOUR DOGGIE
DR1:
    // asm: 	SOFTWTM	R0		    	;SET WAIT STATES
    // asm: 	DINT
    // asm:         LDP	@FASTSTKI
    // asm:         LDI	@FASTSTKI,SP		;LOAD THE ADDRESS INTO SP
#if DEBUG
    // asm: 	CALL	VERIFY_CODE_INTEGRITY
#endif
    // 	;LOAD CODE BACK INTO RAM
    // 	;
    // 	;
    // asm: 	LDI	040h,AR1		;SOURCE ADDRESS
    // asm: 	LDI	0C000h,AR2		;DESINATION ADDRESS
    // asm: 	LS	8,AR2
    // asm: 	ADDI	040h,AR2
    // asm: 	LDI	1000h,RC		;COPY THE PROGRAM INTO
    // asm: 	LS	4,RC			;FAST RAM
    // asm: 	RPTB	LD_RAM2
    // asm: 	LDI	*AR2++,R0
    // asm: LD_RAM2	STI	R0,*AR1++
    // asm: 	CLRI	R0
    // asm: 	LDP	@COLORAM
    // asm: 	STI	R0,@COLORAM
    // asm: 	SETDP
    // asm: 	STI	R0,@DIAG_ACTIVE
    // asm: 	CALL	CLR_PBSS
    // asm: 	CALL	CLR_RAM			;INIT .BSS TO 0
    // asm: 	LDI	MDIAG,R0		;MELLOW THE WHEEL
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	SYSCNTL_INIT,R0		;INIT SYSCNTL RAM SHADOW
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	CALL	FEED_WATCHDOG
    // asm: 	CALL	MESSAGE1		;DISPLAY STARTUP MESSAGE TO SCREEN
    // asm: 	CALL	MSG1
    // asm: 	LDI	240,R0			;X MIN
    // asm: 	STI	R0,@PREVX
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@DELTA
    // asm: 	LDP	@TIMER_CNTL1
    // asm: 	LDI	200h|1,R0		;INIT TIMER MUMBO-JUMBO
    // asm: 	STI	R0,@TIMER_CNTL1		;
    // asm: 	LDI	-1,R0			;
    // asm: 	STI	R0,@TIMER_PERIOD1	;
    // asm: 	SETDP
    // asm: 	LDF	0,R0
    // asm: 	LDI	@TIMERAMI,AR0
    // asm: 	RPTS	47	 		;CLEAR OUT COUNT AREA
    // asm: 	STF	R0,*AR0++
    // asm: 	LDL	5A5A5A5Ah,R0	 	;INIT RANDOM NUMBER SEED
    // asm: 	STI	R0,@RAND
    // asm: 	CALL	LOAD_FIXED_PALETTES	;LOAD BASE PALETTES
    // asm: 	CALL	INIT_SYSTEM		;INIT REST OF SYSTEM
    // asm: 	CALL	READIO			;GET I/O STATUS (W/INTs OFF)
    // asm: 	CALL	CHECKDIAG		;GOTO DIAGNOSTICS PREDICATE
    // asm: 	CALL	FIFO_RESET		;CLEAR THE FIFO
    // asm: 	CALL	COMM_INIT
    // asm: 	CALL	TIMER_RESET
    // asm: 	DINT
    // asm: 	CALL	RESET_SNDBRD		;RESET SOUND BOARD (and DB)
    // asm: 	EINT
    // asm: 	CALL	FIFO_RESET
    // asm: 	LDI	INT0_M|INT3_M,IE	;ENABLE INTERRUPTS
    // asm: 	AND     0,IF
    // asm: 	CALL	ENABLEGIE
    // asm: 	CALL	COMM_ENABLE_INT2
    // asm: 	CALL	INITIALIZATION_MOTION_CHECK
    // asm: 	CLRI	R2
    // asm: 	SETAUD	ADJ_OUTOFDIAG
    // asm: 	LDI	1,R0			;INIT BASE OF WAVERAM
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	LDL	_SECshared,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	LDL	_SECskys,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	MSG2
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	STI	R0,@BOOT_PACIFY_SCREEN_P
    // asm: 	LDL	_SECgeneral,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	STI	R0,@BOOT_PACIFY_SCREEN_P
    // asm: 	LDL	_SEChead2head,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	CALL	MSG3
    // asm: 	LDI	1,R0			;LOAD 1ST WAVE
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	STI	R0,@BOOT_PACIFY_SCREEN_P
    // asm: 	LDL	_SECpress,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDI	0,R2
    // asm: 	LDI	AUD_BCREDITS,AR2
    // asm: 	CALL	AUDIT_WRITE
    // asm: 	READADJ	ADJ_VOLUME
    // asm: 	LDI	R0,R1
    // asm: 	CALL	SET_MASTER_VOL
    // asm: 	LDP	@WDHIT			;LOG WATCHDOG HIT
    // asm: 	LDI	@WDHIT,R0
    // asm: 	SETDP
    // asm: 	AND	8,R0 			;CHECK B3
    // asm: 	BNZ	NOPEIT
    // asm: 	INCAUD	AUD_NUM_WATCHDOGS
NOPEIT:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_newbut
    // asm: 	LDI	-1,AR2
    // asm: 	STI	AR2,@_ATTR_MODE
    // asm: 	CALL	WAVE			;setup 1st wave
    // asm: 	CALL	FIFO_RESET
    // asm: 	LDI	INT0_M|INT3_M,IE	;ENABLE INTERRUPTS
    // asm: 	LDP	@COMMINTM
    // asm: 	OR	@COMMINTM,IE
    // asm: 	SETDP
    // asm: 	AND     0,IF
    // asm: 	CALL	ENABLEGIE
    // asm: 	READAUD	AUD_VERSION		;version update???
    // asm: 	CMPI	VERSION_ID,R0
    // asm: 	BNE	VERSION_UPDATE
    // asm: 	CALL	VALIDATE_CMOS		;DIAGNOSTIC CHECK ON CMOS
    // asm: 	BC	CMOS_ERROR		;wait till its better
    // asm: 	READAUD	ADJ_ACTUALHSTDRESET	;CHECK FOR HI SCORE RESET
    // asm: 	CMPI	0,R0
    // asm: 	BGT	NODO1
    // asm: 	CALL	INIT_HSTD_TABLES
    // asm: 	READAUD	ADJ_HIGHSCORE_RESET
    // asm: 	LDI	R0,R2
    // asm: 	SETADJ	ADJ_ACTUALHSTDRESET
NODO1:
    // asm: LDI	1,R0
    // asm: 	STI	R0,@NFRAMES
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@ERRORO
    // asm: 	STI	R0,@ERRORN
    // asm: 	CALL	TIMERESET
    // asm: 	CALL	COMMQ_PACKET_INIT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_c_int00", 0, 0);
    UNIMPL();
}

void MAINLOOP(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // asm: 	CALL	TIMEREC
    // asm: 	CALL	DECOMPRESS_PROC
    // asm: 	CALL	TIMEREC
MWAIT0:
    // asm: LDI	@INFRAMES,R0
    // asm: 	CMPI	@FRAMRATE,R0		;1 FRAME MINIMUM
    // asm: 	BLT	MWAIT0
    // asm: 	CALL	TIMEREC
    // asm: 	CALL	ZSORTWT			;WAIT FOR PAGE WHILE SORTING
    // asm: 	CALL	TIMEREC
MWAIT:
    // asm: LDI	@CLEARRDY,R0		;DONE WHEN INT SIGNALS PAGE SWAPPED
    // asm: 	BNZ	MWAIT
ENTER2:
    // 	;COMMUNICATION XFER
    // 	;
    // asm: 	CALL	COMM_ROUTINE
    // asm: 	CALL	COMMQ_PACKET_INIT
    // asm: 	CALL	DECODE_BUFFER
    // asm: 	LDI	@INFRAMES,R0
    // asm: 	CLRI	R1
    // asm: 	STI	R1,@INFRAMES		;CLEAR INTERRUPT COUNTER
    // asm: 	STI	R0,@NFRAMES		;SAVE FOR ALL CURRENT PROCESSES
    // asm: 	CALL	TIMEREC
    // asm: 	STF	R0,@FRAMETIME		;SAVE THE FRAMETIME
    // asm: 	CALL	TIMERESET
    // asm: 	LDI	@_MODE,R4		;FILL FIFO STUFF
    // asm: 	TSTB	MINFIN,R4		;
    // asm: 	CALLNZ	INFINITY
    // asm: 	CALL	TIMEREC
    // asm: 	LDI	@COMM_DRONE_PTR,R0
    // asm: 	CALLNZ	COMM_DRONE_PTR_SORT
    // asm: 	CALL	DIRQ			;DISPLAY THE POLYGONS
    // asm: 	CALL	TIMEREC
    // asm: 	LDI	@_MODE,R4		;FILL FIFO STUFF
    // asm: 	TSTB	MHUD,R4
    // asm: 	BZ	NHUD
    // asm: 	AND	MMODE,R4
    // asm: 	CMPI	MGAME,R4
    // asm: 	BEQ	DOIT8
    // asm: 	CMPI	MBONUS,R4
    // asm: 	BNE	NHUD
DOIT8:
    // asm: CALL	HUD			;HEADS UP DISPLAY (GAME MODE ONLY)
NHUD:
    // asm: 	CALL	TIMEREC
    // asm: 	LDI	@_MODE,R4
    // asm: 	AND	MMODE,R4		;THIS MASK IS IMPORTANT!!!!
    // asm: 	CMPI	MATTR,R4
    // asm: 	CALLEQ	INSERT_COINS
    // asm: 	CALL	VOLUME_DISPLAY
    // asm: 	CALL	TIMEREC
    // asm: 	LDI	@_MODE,R0		;MOTION CABINET LEVELING
    // asm: 	AND	MMODE,R4		;IF NOT IN GAME
    // asm: 	CMPI	MGAME,R4
    // asm: 	BEQ	NODO555
    // asm: 	CALL	LEVEL_THE_MOTION
NODO555:
    // asm: 	CALL	CHECKDIAG		;JUMP TO DIAGNOSTICS?
    // asm: 	CALL	SWDISP			;DISPATCH ANY SWITCH PROCESSES
    // asm: 	CALL	COLSCC
    // asm: 	CLRI	R0
    // asm: 	LDP	@MPROC_TIK
    // asm: 	STI	R0,@MPROC_TIK
    // asm: 	SETDP
    // asm: 	CALL	PRC_DISPATCH		;EXECUTE PROCESSES
    // asm: 	CALL	COMMQ_READY_TO_SEND
    // asm: 	CALL	TEXT_OUTPUT		;END FILL FIFO STUFF
    // asm: 	CALL	TIMEREC
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BNE	MAINLOOP
    // 	;we are in attract mode, set appropriately
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BZ	MAINLOOP
    // asm: 	LDI	@OM_ATTR_MODE,AR2
    // asm: 	CMPI	@OLD_OM_ATTR_MODE,AR2
    // asm: 	BEQ	MAINLOOP
    // asm: 	CMPI	-2,AR2		;WE MAY ONLY SYNC ON THE MIDWAY SPIN
    // asm: 	BNE	MAINLOOP
    // asm: 	STI	AR2,@OM_ATTR_MODE
    // asm: 	STI	AR2,@_ATTR_MODE
    // asm: 	CALL	SILENT
    // asm:         LDI	@FASTSTKI,SP		;GET PAGE OF STORED ADDRESS
    // asm: 	LDI	@_ATTR_MODE,AR2		;AND INTO FP TOO
    // asm: 	CALL	WAVE
    // asm: 	BU	COLD_ENTER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAINLOOP", 0, 0);
    UNIMPL();
}

void COLD_ENTER(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *WE HAVE A RADICAL SYSTEM CHANGE
    // *WE WANT TO
    // *1.	LET EXECUTE ANY PROCESSES THAT WE HAVE BEGUN
    // *2.	CLEAR THE SCREEN (WHILE STILL VIEWING OLD SCREEN)
    // *3.	PLOT THE NEW SCREEN
    // *4.	CONTINUE WITH THE SYSTEM
    // *
    // asm: 	FIFO_CLRP	R0		;is the fifo clear
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CLEARRDY		;set clear ready to happen...
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@NOAERASE		;turn auto-erase ON
C_WAIT:
    // asm: LDI	@CLEARRDY,R0		;wait for it to clear
    // asm: 	BNZ	C_WAIT
    // asm: 	CALL	COMMQ_READY_TO_SEND
    // asm: 	CALL	COMM_ROUTINE
    // asm: 	CALL	COMMQ_PACKET_INIT
    // asm: 	CALL	DECODE_BUFFER
    // asm: 	CALL	PRC_DISPATCH
    // asm: 	CALL	COMMQ_READY_TO_SEND
    // asm: 	BR	ENTER2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COLD_ENTER", 0, 0);
    UNIMPL();
}

void ENABLEGIE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *INTERRUPT ROUTINE
    // *
    // *1.	initially mask out all interupts we dont want to occur
    // *2.	enable GIE of ST by calling a routine that RETI's
    // *	(allows debugger to function)
    // *3.	increment number of frames passed since last screen update
    // *
    // *
    // asm: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENABLEGIE", 0, 0);
    UNIMPL();
}

void INT0(void)
{
    // asm: 	PUSH	ST
    // asm: 	LDI	INT1_M,IE	;disable everything except TV30 interrupt & comm int
    // asm: 	ANDN	INT0_M,IF	;we wont irq ourself
    // asm: 	PUSH	DP
    // asm: 	PUSH	R0
    // asm: 	LDP	@COMMINTM
    // asm: 	OR	@COMMINTM,IE
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	@CPU_WS,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	SOFT_WS,R0
    // asm: 	STI	R0,@CPU_WS
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	PUSHF	R0
    // asm: 	PUSHF	R1
    // asm: 	PUSHF	R2
    // asm: 	PUSHF	R3
    // asm: 	PUSHF	R4
    // asm: 	PUSHF	R5
    // asm: 	PUSHF	R6
    // asm: 	PUSHF	R7
    // asm: 	PUSH	RC
    // asm: 	PUSH	RS
    // asm: 	PUSH	RE
    // asm: 	PUSH	BK
    // asm: 	PUSH	IR0
    // asm: 	PUSH	IR1
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	SETDP
    // asm: 	CALL	PAL_XFER		;TRANSFER QUEUED PALETTES
    // asm: 	LDI	@BGNDCOLA,R0		;SET BACKGROUND COLOR
    // asm: 	LDP	@COLORAM
    // asm: 	STI	R0,@COLORAM
    // asm: 	SETDP				;RESET DP
    // asm: 	CALL	COMM_MASTER_SEND_SYNC
    // asm: 	CALL	ENABLEGIE
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MDIAG,R0
    // asm: 	BEQ	NTEST
    // asm: 	LDP	@MPROC_TIK
    // asm: 	LDI	@MPROC_TIK,R0
    // asm: 	SETDP
    // asm: 	INC	R0
    // asm: 	CMPI	300,R0
    // asm: 	BLE	REGIT
    // asm: 	ERRON	U,EC_WATCHDOG2
    // asm: 	BU	_c_int00
REGIT:
    // asm: LDP	@MPROC_TIK
    // asm: 	STI	R0,@MPROC_TIK
    // asm: 	SETDP
NTEST:
    // asm: 	CALL	FEED_WATCHDOG
    // asm: 	LDI	@CLEARRDY,R0
    // asm: 	BZ	NCLRSCR
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CLEARRDY		;RESET SCREEN CLEAR FLAG
    // asm: 	STI	R0,@IFRAMES	 	;RESET INTERRUPT FRAME COUNTER
    // asm: 	LDI	@MOTION_RCV_TIKS,R0	;MOTION RECEIVE ERROR
    // asm: 	DEC	R0
    // asm: 	STI	R0,@MOTION_RCV_TIKS
    // asm: 	LDI	@NOSWAP,R0		;PAGE SWAP?
    // asm: 	BNZ	NCLRSCR
    // asm: 	LDI	@DISPLAY_PAGE,R0	;PAGE SWAP FLAG
    // asm: 	XOR	1,R0
    // asm: 	STI	R0,@DISPLAY_PAGE
    // asm: 	BNZ	PAGE1
    // asm: 	CALL	SETPAGE1
    // asm: 	CALL	FASTCLR0
    // asm: 	B	DN_PAGE
PAGE1:
    // asm: CALL	SETPAGE0
    // asm: 	CALL	FASTCLR1
DN_PAGE:
NCLRSCR:
    // asm: 	LDI	@STOPWATCH_CNTL,R0	;STOPWATCH TIMER
    // asm: 	BZ	NOSTOPWUPDT
    // asm: 	INCM	@STOPWATCH
NOSTOPWUPDT:
    // asm: 	INCM	@_sectime		;ONE SECOND TIMER
    // asm: 	CMPI	TIKS_PER_SECOND,R0
    // asm: 	BLT	NOTASEC
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_sectime
    // asm: 	LDI	@_MODE,R5
    // asm: 	AND	MMODE,R5
    // asm: 	CMPI	MDIAG,R5
    // asm: 	BEQ	JJ88
    // asm: 	INCAUD	AUD_POWERON_TIME
    // asm: 	CMPI	MATTR,R5
    // asm: 	BEQ	JJ88
    // asm: 	INCAUD	AUD_GAMEON_TIME
    // asm: 	CMPI	MGAME,R5
    // asm: 	BNE	JJ88
    // asm: 	LDI	@CAMVIEW,AR2
    // asm: 	ADDI	AUD_VIEW1_TIME,AR2
    // asm: 	CALL	AUDIT_INC
JJ88:
    // asm: 	LDI	@OFFROAD_TMR,R0	 	;OFFROAD TIMER
    // asm: 	SUBI	1,R0
    // asm: 	LDILT	0,R0
    // asm: 	STI	R0,@OFFROAD_TMR
    // asm: 	LDI	@_countdown,R0		;LO DP TIME REMAINING FOR PLAYER
    // asm: 	BLE	NOTASEC
    // asm: 	LDI	@_MODE,R1		;RACE MUST BE HAPPENING TO DECREMENT COUNT
    // asm: 	TSTB	MGO,R1
    // asm: 	BZ	NOTASEC
    // asm: 	SUBI	1,R0
    // asm: 	STI	R0,@_countdown
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INT0", 0, 0);
    UNIMPL();
}

void READIO(void)
{
    // asm: 	CLRI	AR0			;for Loff production board timing problem
    // asm: 	LDP	@DIPSW
    // asm: 	LDI	@DIPSW,R0
    // asm: 	LDI	*AR0,R2
    // asm: 	LDP	@DIPRAM
    // asm: 	RS	16,R0
    // asm: 	STI	R0,@DIPRAM
    // asm: 	LDP	@SWITCH3
    // asm: 	LDI	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // asm: 	LDI	*AR0,R2			;Loff
    // asm: 	LS	16,R0
    // asm: 	LDI	@SWITCH1,R1
    // asm: 	LDI	*AR0,R2			;Loff
    // asm: 	LS	16,R1
    // asm: 	LDP	@FASTSTKI
    // asm: 	RS	16,R1
    // asm: 	OR	R1,R0
    // asm: 	NOT	R0,R1
    // asm: 	LDI	@SWRAMI,AR0
    // asm: 	STI	R1,@SWITCHBUTS
    // 	;
    // 	;SCAN SWITCHES, TRIGGER ON LEADING EDGE
    // 	;
    // asm: 	AND	*AR0,*+AR0(1),R2	;PREVIOUS TWO OPEN
    // asm: 	AND	R1,R2			;CURRENT ONE CLOSED
    // asm: 	OR	*+AR0(2),R2		;OR INTO EDGE LIST
    // asm: 	STI	R2,*+AR0(2)		;STORE EDGE WORD 1=EDGE TRIGGERED
    // asm: 	LDI	*AR0,R1			;GET N-1
    // asm: 	STI	R1,*+AR0(1)		;N-1 --> N-2
    // asm: 	STI	R0,*AR0			;N --> N-1
    // asm: 	PUSH	DP
    // asm: 	LDI	@SYSCNTL,R0	      	;actually we signal a read of the
    // asm: 	LDP	@SYSCNTLR		;have a value
    // asm: 	ANDN	ATOD_WR,R0	      	;pot(s), they interrupt us when they
    // asm: 	STI	R0,@SYSCNTLR		;have a value
    // asm: 	POP	DP
    // asm: 	CLRI	AR0
    // asm: 	LDP	@ATOD_R
    // asm: 	LDI	04h,R0
    // asm: 	LS	24,R0
    // asm: 	STI	R0,@ATOD_R
    // asm: 	LDI	*AR0,R2
    // asm: 	SETDP
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@RDPOT
    // 	;THESE ARE SPECIAL ROUTINES WHICH ARE CALLED, *NOT* CREATED
    // 	;FOR PROCESSES.
    // 	;	THIS INCLUDES:
    // 	;		VOL+
    // 	;		VOL-
    // 	;		DIAG BUTTON
    // 	;NO OTHER ROUTINES SHOULD BE HERE
    // asm: 	LDI	@SWRAM+2,R0
    // asm: 	TSTB	SW_DIAG,R0
    // asm: 	CALLNZ	DIAG_TOGGLE
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BEQ	NIGY
    // asm: 	CMPI	MDIAG,R0
    // asm: 	BEQ	NIGY
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm: 	TSTB	SW_VOLPLUS,R0
    // asm: 	CALLNZ	VOL_PLUS
    // asm: 	TSTB	SW_VOLMINUS,R0
    // asm: 	CALLNZ	VOL_MINUS
NIGY:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "READIO", 0, 0);
    UNIMPL();
}

void VOL_MINUS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR2
    // asm: 	LDP	@SWRAM+2
    // asm: 	LDI	@SWRAM+2,R0
    // asm: 	ANDN	SW_VOLMINUS,R0
    // asm: 	STI	R0,@SWRAM+2
    // asm: 	SETDP
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@VOLUME_ACTIVE
    // asm: 	READADJ	ADJ_MIN_VOL_LEVEL
    // asm: 	LDI	R0,R3
    // asm: 	READADJ	ADJ_VOLUME
    // asm: 	DEC	R0
    // asm: 	CMPI	R3,R0
    // asm: 	LDILT	R3,R0
    // asm: 	BU	VOLJN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VOL_MINUS", 0, 0);
    UNIMPL();
}

void VOL_PLUS(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR2
    // asm: 	LDP	@SWRAM+2
    // asm: 	LDI	@SWRAM+2,R0
    // asm: 	ANDN	SW_VOLPLUS,R0
    // asm: 	STI	R0,@SWRAM+2
    // asm: 	SETDP
    // asm: 	LDI	60,R0
    // asm: 	STI	R0,@VOLUME_ACTIVE
    // asm: 	READADJ	ADJ_MIN_VOL_LEVEL
    // asm: 	LDI	R0,R3
    // asm: 	READADJ	ADJ_VOLUME
    // asm: 	INC	R0
    // asm: 	CMPI	R3,R0
    // asm: 	LDILT	R3,R0
VOLJN:
    // asm: 	CMPI	0,R0
    // asm: 	LDILE	0,R0
    // asm: 	CMPI	255,R0		;??? strange - but 249-255 cut off
    // asm: 	LDIGT	255,R0
    // asm: 	LDI	R0,R2
    // asm: 	PUSH	R0
    // asm: 	SETADJ	ADJ_VOLUME
    // asm: 	POP	R1
    // asm: 	CALL	SET_MASTER_VOL
    // asm: 	POP	AR2
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VOL_PLUS", 0, 0);
    UNIMPL();
}

void DIAG_TOGGLE(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MDIAG,R0
    // asm: 	BEQ	DTXX
    // asm: 	LDI	@SWRAM+2,R0
    // asm: 	ANDN	010h,R0
    // asm: 	STI	R0,@SWRAM+2
    // asm: 	LDI	@DIAG_ACTIVE,R0
    // asm: 	LDIZ	1,R0
    // asm: 	LDINZ	0,R0
    // asm: 	STI	R0,@DIAG_ACTIVE
DTXX:
    // asm: POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_TOGGLE", 0, 0);
    UNIMPL();
}

void ATODINT(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *OPTIMIZED 9/14/93-ELP
    // asm: 	PUSH	ST
    // asm: 	PUSH	IE
    // asm: 	PUSH	DP
    // asm: 	LDP	@COMMINTM
    // asm: 	LDI	@COMMINTM,IE
    // asm: 	SETDP
    // asm: 	CALL	ENABLEGIE
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	R6
    // asm: 	LDP	@CPU_WS
    // asm: 	PUSH	R1
    // asm: 	PUSHF	R1
    // asm: 	PUSH	AR0
    // asm: 	LDI	@CPU_WS,R6		;SET WAIT STATES
    // asm: 	LDI	1090h,R0
    // asm: 	STI	R0,@CPU_WS
    // asm: 	LDP	@RDPOT			;WHICH POT TO READ?
    // asm: 	LDI	@RDPOT,R0
    // asm: 	BNZD	RDFOOT
    // asm: 	LDP	@SYSCNTL
    // asm: 	LDI	@SYSCNTL,R1
    // asm: 	ANDN	ATOD_RD,R1
    // 	;---->BNZD	RDFOOT
    // 					;RDSTEER
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R1,@SYSCNTLR
    // asm: 	CLRI	AR0
    // asm: 	LDP	@ATOD_R
    // asm: 	LDI	@ATOD_R,R1
    // asm: 	LDI	*AR0,R0
    // asm: 	LDP	@_pot0
    // asm: 	RS	24,R1
    // asm: 	FLOAT	R1
    // asm: 	FLOAT	@_pot0,R0
    // 	;MAXIMUM SLEW RATE
    // 	;
    // asm: 	SUBF	R0,R1
    // asm: 	CMPF	65,R1
    // asm: 	LDFGT	65,R1
    // asm: 	CMPF	-65,R1
    // asm: 	LDFLT	-65,R1
    // asm: 	ADDF	R0,R1
    // asm: 	MPYF	0.33,R1	  		;SMOOTH WHEEL A TAD
    // asm: 	MPYF	0.67,R0
    // asm: 	ADDF	R1,R0
    // asm: 	FIX	R0
    // asm: 	STI	R0,@_pot0
    // asm: NOSMOOTH1
    // asm: 	LDP	@SYSCNTL
    // asm: 	LDI	@SYSCNTL,R0		;ACTUALLY WE SIGNAL A READ OF THE
    // asm: 	LDP	@SYSCNTLR		;HAVE A VALUE
    // asm: 	ANDN	ATOD_WR,R0		;POT(S), THEY INTERRUPT US WHEN THEY
    // asm: 	STI	R0,@SYSCNTLR		;HAVE A VALUE
    // asm: 	LDP	@ATOD_R
    // asm: 	CLRI	AR0
    // asm: 	LDI	05h,R0			;SETUP TO READ GAS PEDAL
    // asm: 	LS	24,R0
    // asm: 	STI	R0,@ATOD_R
    // asm: 	LDI	*AR0,R0
    // asm: 	BUD	EXITR
    // asm: 	LDP	@RDPOT
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@RDPOT
    // 	;---->	BUD	EXITR
RDFOOT:
    // asm: 	CMPI	1,R0
    // asm: 	BNED	RDBRAKE
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R1,@SYSCNTLR
    // asm: 	LDP	@ATOD_R			;READ GAS PEDAL
    // 	;---->	BNED	RDBRAKE
    // asm: 	CLRI	AR0
    // asm: 	LDI	@ATOD_R,R1
    // asm: 	LDI	*AR0,R0
    // asm: 	LDP	@_pot1
    // asm: 	RS	24,R1
    // asm: 	FLOAT	R1
    // asm: 	FLOAT	@_pot1,R0
    // asm: 	MPYF	0.33,R1	  		;SMOOTH A TAD
    // asm: 	MPYF	0.67,R0
    // asm: 	ADDF	R1,R0
    // asm: 	FIX	R0
    // asm: 	STI	R0,@_pot1
    // asm: 	LDP	@SYSCNTL
    // asm: 	LDI	@SYSCNTL,R0		;ACTUALLY WE SIGNAL A READ OF THE
    // asm: 	LDP	SYSCNTLR
    // asm: 	ANDN	ATOD_WR,R0		;POT(S), THEY INTERRUPT US WHEN THEY
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	LDP	@ATOD_R
    // asm: 	LDI	06h,R0			;SETUP TO READ BRAKE PEDAL
    // asm: 	LS	24,R0
    // asm: 	CLRI	AR0
    // asm: 	STI	R0,@ATOD_R
    // asm: 	LDI	*AR0,R0
    // asm: 	BUD	EXITR
    // asm: 	LDP	@RDPOT
    // asm: 	LDI	2,R0
    // asm: 	STI	R0,@RDPOT
    // 	;---->	BUD	EXITR
RDBRAKE:
    // asm: 	CLRI	AR0
    // asm: 	LDI	@ATOD_R,R1		;READ BRAKE PEDAL
    // asm: 	LDI	*AR0,R0
    // asm: 	LDP	@_pot2
    // asm: 	RS	24,R1
    // asm: 	FLOAT	R1
    // asm: 	FLOAT	@_pot2,R0
    // asm: 	MPYF	0.25,R1	  		;SMOOTH A TAD
    // asm: 	MPYF	0.75,R0
    // asm: 	ADDF	R1,R0
    // asm: 	FIX	R0
    // asm: 	STI	R0,@_pot2
    // asm: 	LDP	@RDPOT
    // asm: 	LDI	3,R0
    // asm: 	STI	R0,@RDPOT
EXITR:
    // asm: 	LDP	@CPU_WS
    // asm: 	POP	AR0
    // asm: 	POPF	R1
    // asm: 	POP	R1
    // asm: 	STI	R6,@CPU_WS
    // asm: 	POP	R6
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	POP	DP
    // asm: 	ANDN	INT3_M,IF		;masking a2d int. (no longer pending)
    // asm: 	POP	IE
    // asm: 	POP	ST
    // asm: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ATODINT", 0, 0);
    UNIMPL();
}

void SWDISP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SWITCH DISPATCH	 START PROCESSES FOR SWITCHES
    // *
    // ;	LDP	@SWRAM
    // asm: 	LDI	@SWRAM+2,R3
    // asm: 	LS	4,R3 		;MASK GARBAGE
    // asm: 	RS	4,R3
    // asm: 	BZ	SWSTX	 	;NO EDGES ACTIVATED
    // asm: 	LDI	0,R1
    // asm: 	STI	R1,@SWRAM+2	;CLEAR OUT EDGES
    // asm: 	LDI	-1,R1
SWTLP:
    // asm: 	ADDI	1,R1
    // asm: 	LSH	-1,R3
    // asm: 	BNC	SWTLP
    // ;	LDP	@SWTABI
    // asm: 	LDI	@SWTABI,AR0
    // asm:      	ADDI	R1,AR0
    // asm: 	LDI	*AR0,AR2	;GET WAKEUP
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	NOGPROC
    // asm: 	LDI	SPAWNER_C|SPWN_SWITCH_T,R2
    // asm: 	CALL	PRC_CREATE	;R2=PID, AR2=WAKEUP ADDR
NOGPROC:
    // asm: LDI	R3,R3
    // asm: 	BNZ	SWTLP
SWSTX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SWDISP", 0, 0);
    UNIMPL();
}

void CHECKDIAG(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@DIAG_ACTIVE,R0
    // asm: 	RETSZ
    // asm: 	BR	ENTER_DIAG
    // ;	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKDIAG", 0, 0);
    UNIMPL();
}

void CLR_PBSS(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	RC
    // asm: 	PUSH	RS
    // asm: 	PUSH	RE
    // asm: 	LDI	@PBSS_PTR,AR0
    // asm: 	LDI	@PBSS_BSSEND,RC
    // asm: 	SUBI	AR0,RC
    // asm: 	SUBI	1,RC
    // asm: 	CLRI	R0
    // asm: 	RPTB	PRAMCLP
PRAMCLP:
    // asm: STI	R0,*AR0++
    // asm: 	CALL	CLEAR_LINK
    // asm: 	POP	RE
    // asm: 	POP	RS
    // asm: 	POP	RC
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR_PBSS", 0, 0);
    UNIMPL();
}

void CLR_RAM(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	RC
    // asm: 	PUSH	RS
    // asm: 	PUSH	RE
    // asm: 	LDI	@RAM_PTR,AR0
    // asm: 	LDI	@RAM_BSSEND,RC
    // asm: 	SUBI	AR0,RC
    // asm: 	SUBI	1,RC
    // asm: 	CLRI	R0
    // asm: 	RPTB	RAMCLP
RAMCLP:
    // asm: STI	R0,*AR0++
    // asm: 	POP	RE
    // asm: 	POP	RS
    // asm: 	POP	RC
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLR_RAM", 0, 0);
    UNIMPL();
}

void CLEAR_ONCHIPRAM(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *THESE MUST BE CALLED *BEFORE* PROGRAM IS COPIED INTO FAST RAM
    // *
    // asm: 	LDIL	809800h,AR0
    // asm: 	CLRI	R0
    // asm: 	LDI	2047h,RC
    // asm: 	RPTS	RC
    // asm: 	STI	R0,*AR0++
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_ONCHIPRAM", 0, 0);
    UNIMPL();
}

void BUTTONS(void)
{
    // *
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MDIAG,R0
    // asm: 	BEQ	DIAG_BUTTONS
    // asm: 	CMPI	MATTR,R0
    // asm: 	BNE	BUT3
    // asm: 	READAUD	ADJ_FREE_PLAY
    // asm: 	CMPI	1,R0
    // asm: 	BNE	JUSTGOON
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	OR	BUT_START,R0
    // asm: 	STI	R0,@BUTTON_STATUS
    // asm: 	BU	BUT3
JUSTGOON:
    // asm: 	READAUD	AUD_CREDITS
    // asm: 	CMPI	0,R0
    // asm: 	BLE	DASHLIGHT
BUT3:
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	TSTB	BUT_START,R0
    // asm: 	BZ	NOSTART
    // asm: 	LDP	@BUTTON_TIKS
    // asm: 	LDI	@BUTTON_TIKS,R1
    // asm: 	INC	R1
    // asm: 	STI	R1,@BUTTON_TIKS
    // asm: 	SETDP
    // asm: 	CMPI	30,R1
    // asm: 	BLT	NOSTART
    // asm: 	ANDN	BUT_START,R0		;TURN IT OFF
    // asm: 	CMPI	60,R1
    // asm: 	BLT	NOSTART
    // asm:  	CLRI	R1
    // asm: 	LDP	@BUTTON_TIKS
    // asm: 	STI	R1,@BUTTON_TIKS
    // asm: 	SETDP
NOSTART:
    // asm: 	LDI	@HEAD2HEAD_ON,R2
    // asm: 	BNZ	PAPAFFD
    // asm: 	CMPI	@OLD_BUTTON_STATUS,R0
    // asm: 	RETSEQ
PAPAFFD:
    // asm: STI	R0,@OLD_BUTTON_STATUS
BUTLITE:
    // asm: 	LDIL	SOUND,AR0
    // asm: 	LDI	0FF05h,R1
    // asm: 	LDI	0F705h,R2
    // asm: 	STI	R1,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R2,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R1,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	LDI	@HEAD2HEAD_ON,R2
    // asm: 	BZ	KKDAK
    // asm: 	LDI	@THIS_MACHINE_AHEAD,R2
    // asm: 	BNZ	KKDAK
    // asm: 	AND	0FFh,R0
    // asm: 	ANDN	BUT_FRONT,R0
    // asm: 	BU	FDDDA
KKDAK:
    // asm: AND	0FFh,R0
    // asm: 	OR	BUT_FRONT,R0
FDDDA:
    // asm: LDI	R0,R1
    // asm: 	OR	0FF00h,R0
    // asm: 	OR	0FB00h,R1
    // asm: 	STI	R0,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R1,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R0,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	RETS
DIAG_BUTTONS:
    // asm: 	LDP	@BUTTON_STATUS
    // asm: 	LDI	@BUTTON_STATUS,R0
    // asm: 	SETDP
    // asm: 	LDP	@BUTTON_TIKS
    // asm: 	LDI	@BUTTON_TIKS,R1
    // asm: 	INC	R1
    // asm: 	STI	R1,@BUTTON_TIKS
    // asm: 	SETDP
    // asm: 	CMPI	30,R1
    // asm: 	BLT	DGBT
    // asm: 	CLRI	R0
    // asm: 	CMPI	50,R1
    // asm: 	BLT	DGBT
    // asm: 	CLRI	R1
    // asm: 	LDP	@BUTTON_TIKS
    // asm: 	STI	R1,@BUTTON_TIKS
    // asm: 	SETDP
DGBT:
    // asm: 	LDIL	SOUND,AR0
    // asm: 	LDI	0FF05h,R1
    // asm: 	LDI	0F705h,R2
    // asm: 	STI	R1,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R2,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R1,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	LDI	R0,R1
    // asm: 	OR	0FF00h,R0
    // asm: 	OR	0FB00h,R1
    // asm: 	STI	R0,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R1,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	STI	R0,*AR0
    // asm: 	RPTS	SPACER
    // asm: 	NOP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BUTTONS", 0, 0);
    UNIMPL();
}

void WAIT_FOR_VBLANK(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *WAIT FOR INTERRUPT AT VBLANK
    // *
    // asm: 	LDI	@INFRAMES,R0
WAIT1:
    // asm: CMPI	@INFRAMES,R0		;wait for a hot one
    // asm: 	BZ	WAIT1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_FOR_VBLANK", 0, 0);
    UNIMPL();
}

void CRT_REG_SETUP(void)
{
    // asm: 	PUSHM	AR0,AR1,R0,DP
    // asm: 	LDIL	CRT_SETUP,AR0
    // asm: 	LDP	@CRT_REG_SETUP_STRI
    // asm: 	LDI	@CRT_REG_SETUP_STRI,AR1
    // asm: 	LDI	11,RC
    // asm: 	RPTB	CRTRGLP
    // asm: 	LDI	*AR1++,R0
CRTRGLP:
    // asm: STI	R0,*AR0++
    // asm: 	POPM	DP,R0,AR1,AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CRT_REG_SETUP", 0, 0);
    UNIMPL();
}

void ERROR_TRAP(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *IF THIS IS EVER HIT STOP AND FIND OUT WHERE IT IS
    // *COMING FROM
    // *
    // asm: 	CALL	ENABLEGIE
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	SLOCKON	U,"ERROR_TRAP INT"
#if DEBUG
    // asm: 	BU	$
#endif
    // asm: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ERROR_TRAP", 0, 0);
    UNIMPL();
}

void FIFO_RESET(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	DINT
    // asm: 	LDP	@SYSCNTL
    // asm: 	LDI	@SYSCNTL,R0		;READ SHADOW LOCATION
    // asm: 	LDI	R0,R1
    // asm: 	ANDN	1,R1			;MASK OUT FIFORS
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R1,@SYSCNTLR		;STROBE SYSTEM
    // asm: 	STI	R0,@SYSCNTLR		;THEN BACK TO NORMAL
    // asm: 	EINT
    // asm: 	LDP	@FIFO_CONTROL		;RESET COUNTER ON FIFO  (SAFETY FEATURE)
    // asm: 	LDI	FIFO_CONTROL_DMA_RUNSEL|FIFO_CONTROL_FIFO_RST,R0
    // asm: 	STI	R0,@FIFO_CONTROL
    // asm: 	NOP
    // asm: 	LDI	FIFO_CONTROL_DMA_RUNSEL,R0
    // asm: 	STI	R0,@FIFO_CONTROL
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIFO_RESET", 0, 0);
    UNIMPL();
}

void TIMER_RESET(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *1ms   = 10 counts
    // *1mils = 10000
    // *1frame should = 165000 (roughly)
    // *
    // asm: 	PUSH	DP
    // asm: 	PUSH	R0
    // asm: 	LDP	@TIMER_CNTL1
    // asm: 	LDI	@TIMER_CNTL1,R0
    // asm: 	OR	TMR_RESET_GO,R0
    // asm: 	STI	R0,@TIMER_CNTL1
    // asm: 	POP	R0
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMER_RESET", 0, 0);
    UNIMPL();
}

void TIMER_READ(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	DP
    // asm: 	LDP	@TIMER_CNTR1
    // asm: 	LDI	@TIMER_CNTR1,R0
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMER_READ", 0, 0);
    UNIMPL();
}

void TIMERESET(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	DP
    // asm: 	CALL	TIMER_RESET
    // asm: 	SETDP
    // asm: 	LDI	@TIMERAMI,AR0
    // asm: 	STI	AR0,@TIMEX
    // asm: 	LDI	@TIMECLR,R0
    // asm: 	BNZ	TIMEL1
    // asm: 	LDF	@TIMEFRAME,R0
    // asm: 	ADDF	1,R0
    // asm: 	STF	R0,@TIMEFRAME
    // asm: 	CALL	INV_F30
    // asm: 	LDI	@TIMERAMI,AR0
    // asm: 	STI	AR0,@TIMEX
    // asm: 	LDI	15,RC
    // asm: 	RPTB	TIMELP
    // asm: 	LDF	*AR0++,R1
    // asm: 	ADDF	*+AR0(15),R1
    // asm: 	STF	R1,*+AR0(15)
    // asm:  	MPYF	R0,R1
TIMELP:
    // asm: STF	R1,*+AR0(31)
    // asm: 	POP	DP
    // asm: 	RETS
TIMEL1:
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@TIMECLR
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,@TIMEFRAME
    // asm: 	LDI	@TIMERAMI,AR0
    // asm: 	RPTS	47	 		;CLEAR OUT COUNT AREA
    // asm: 	STF	R0,*AR0++
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMERESET", 0, 0);
    UNIMPL();
}

void TIMEREC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	DP
    // asm: 	PUSH	AR0
    // asm: 	LDP	@TIMER_CNTR1
    // asm: 	FLOAT	@TIMER_CNTR1,R0
    // asm: 	LDP	@TIMEX
    // asm: 	LDI	@TIMEX,AR0
    // asm: 	STF	R0,*AR0++ 	;SAVE THE INDEX
    // asm: 	STI	AR0,@TIMEX
    // asm: 	POP	AR0
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIMEREC", 0, 0);
    UNIMPL();
}

void MESSAGE1(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CLRI	R0
    // asm: 	LDP	@9E0000h
    // asm: 	STI	R0,@9E0000h
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,@9E0001h		;white
    // asm: 	LDI	06000h,R0
    // asm: 	STI	R0,@9E0003h		;red
    // asm: 	LDI	0294Ah,R0
    // asm: 	STI	R0,@9E000Bh		;grey
    // asm: 	SETDP
    // asm: 	CALL	CLRSCRN
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDI	1,RC
    // asm: 	TEXTIT	startup0,1,40
    // asm: 	TEXTIT	startup1,1,70
    // asm: 	TEXTIT	startup2,1,80
    // asm: 	TEXTIT	startup3,1,90
    // asm: 	LDI	11,RC
    // asm: 	TEXTIT	dlts,40,110
    // asm: 	LDI	1,RC
    // asm: 	TEXTIT	VERSION_STAMP,40,130
    // asm: 	TEXTIT	INTERNAL_VERS,260,150
    // asm: 	TEXTIT	DATE_STAMP,40,150
    // asm: 	CLRI	AR0
    // asm: 	LDP	@DIPSW
    // asm: 	LDI	@DIPSW,R0
    // asm: 	LDI	*AR0,AR0
    // asm: 	RS	16,R0
    // asm: 	SETDP
    // asm: 	TSTB	DIP_COMMP,R0
    // asm: 	BZ	DODOIBO
    // asm: 	LDI	@LINKDISABLED,AR2
    // asm: 	BU	HJSADF
DODOIBO:
    // asm: TSTB	CMDP_MASTER,R0
    // asm: 	LDIZ	@IAMMASTER,AR2
    // asm: 	LDINZ	@IAMSLAVE,AR2
HJSADF:
    // asm: LDI	40,R2
    // asm: 	LDI	160,R3
    // asm: 	CALL	_outtextxyc
    // 	;test if link pal is installed
    // 	;
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MESSAGE1", 0, 0);
    UNIMPL();
}

void MSG1(void)
{
    // asm: LDI	11,RC
    // asm: 	TEXTIT	M2,1,200
    // asm: 	TEXTIT	M3,1,220
    // asm: 	TEXTIT	M4,1,230
    // asm: 	TEXTIT	M5,1,240
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSG1", 0, 0);
    UNIMPL();
}

void MSG2(void)
{
    // asm: LDI	11,RC
    // asm: 	TEXTIT	M6,1,260
    // asm: 	TEXTIT	M7,1,270
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSG2", 0, 0);
    UNIMPL();
}

void MSG3(void)
{
    // asm: LDI	11,RC
    // asm: 	TEXTIT	M8,1,280
    // asm: 	TEXTIT	M9,1,290
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSG3", 0, 0);
    UNIMPL();
}

void CHECK_STATE(void)
{
    // *
    // *
    // *
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MATTR,R0
    // asm: 	BNE	ABORT_STATE
    // asm: 	LDI	@SWITCHBUTS,R2
    // asm: 	LDI	@STATE_NUM,AR0
    // asm: 	LDI	AR0,R0
    // asm: 	ADDI	@STATE_TABLEI,AR0
    // asm: 	LDI	*AR0,R1
    // asm: 	AND	@STATE_MASK,R2
    // asm: 	CMPI	R1,R2		;ADVANCE TO NEXT STATE?
    // asm: 	BEQ	NEXTSTATE
    // asm: 	LDI	@STATE_NUM,R1
    // asm: 	BZ	ABORT_STATE
    // asm: 	DEC	R1
    // asm: 	LDI	R1,AR0
    // asm: 	ADDI	@STATE_TABLEI,AR0
    // asm: 	LDI	*AR0,R1
    // asm: 	CMPI	R1,R2		;IS IT THE PREVIOUS STATE??
    // asm: 	BNE	ABORT_STATE
    // asm: 	LDI	@STATE_TIK,R0
    // asm: 	DEC	R0		;ABORT STATE
    // asm: 	BLT	ABORT_STATE
    // asm: 	STI	R0,@STATE_TIK
    // asm: 	RETS
NEXTSTATE:
    // asm: 	LDI	@STATE_NUM,R0
    // asm: 	INC	R0
    // asm: 	CMPI	NUM_STATES,R0
    // asm: 	BEQ	DODIAG
    // asm: 	STI	R0,@STATE_NUM
    // asm: 	LDI	120,R0
    // asm: 	STI	R0,@STATE_TIK
    // asm: 	RETS
DODIAG:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@STATE_NUM
    // asm: 	STI	R0,@STATE_TIK
    // asm: 	BR	HIDDEN_DIAG
    // *
    // *
    // *
ABORT_STATE:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@STATE_NUM
    // asm: 	STI	R0,@STATE_TIK
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_STATE", 0, 0);
    UNIMPL();
}

void DASHLIGHT(void)
{
    // *
    // asm: 	LDI	@BUTTON_TIK,R0
    // asm: 	INC	R0
    // asm: 	LDILT	0,R0
    // asm: 	STI	R0,@BUTTON_TIK
    // asm: 	CMPI	60,R0
    // asm: 	RETSLT
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@BUTTON_TIK
    // asm: 	LDI	@BUTTON_IBO,R0
    // asm: 	INC	R0
    // asm: 	CMPI	3,R0
    // asm: 	LDIGE	0,R0
    // asm: 	CMPI	0,R0
    // asm: 	LDILT	0,R0
    // asm: 	STI	R0,@BUTTON_IBO
    // asm: 	LDI	R0,AR0
    // asm: 	ADDI	@BUTTONI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	B	BUTLITE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DASHLIGHT", 0, 0);
    UNIMPL();
}

void CMOS_ERROR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	8,AR6
    // asm: FLASH_LP
    // asm: 	CALL	CLRSCRN			;CMOS WAS RESET
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDI	100,AR5
    // asm: FLASH_LP1
    // asm: 	LDI	1,RC
    // asm: 	TEXTITT	"CMOS ERROR",216,190
    // asm: 	TEXTITT	"FACTORY ADJUSTMENTS RESTORED",144,200
    // asm: 	DBU	AR5,FLASH_LP1
    // asm: 	CALL	FEED_WATCHDOG
    // asm: 	DBU	AR6,FLASH_LP
    // asm: 	CALL	FAKEDIAG
    // asm: 	CALL	SET_CONTROLS
    // asm: 	BR	DIAG_RETURN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_ERROR", 0, 0);
    UNIMPL();
}

void VERSION_UPDATE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	8,AR6
    // asm: VFLASH_LP
    // asm: 	CALL	CLRSCRN			;CMOS WAS RESET
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDI	100,AR5
    // asm: VFLASH_LP1
    // asm: 	LDI	1,RC
    // asm: 	TEXTITT	"VERSION UPDATED",196,190
    // asm: 	TEXTITT	"FACTORY SETTINGS RESTORED",156,200
    // asm: 	DBU	AR5,VFLASH_LP1
    // asm: 	CALL	FEED_WATCHDOG
    // asm: 	DBU	AR6,VFLASH_LP
    // ;RESET YOUR SHIT
    // asm: 	CALL	FAKEDIAG
    // asm: 	CALL	FFRSUB
    // asm: 	LDI	VERSION_ID,R2
    // asm: 	SETAUD	AUD_VERSION
    // asm: 	BR	DIAG_RETURN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VERSION_UPDATE", 0, 0);
    UNIMPL();
}

void FAKEDIAG(void)
{
    // asm: 	CALL	INIT_SYSTEM
    // asm: 	CALL	PAL_INIT
    // asm: 	LDL	DIAGPAL,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDI	4210H,R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	LDI	MDIAG,R0		;set mode appropriate
    // asm: 	STI	R0,@_MODE
    // asm:  	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FAKEDIAG", 0, 0);
    UNIMPL();
}

void FEED_WATCHDOG(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	DP
    // asm: 	PUSH	R0
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	@CPU_WS,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	SOFT_WS,R0
    // asm: 	STI	R0,@CPU_WS
    // asm: 	LDP	@SYSCNTL
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	XOR	WDOG,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	POP	R0
    // asm: 	LDP	@CPU_WS
    // asm: 	STI	R0,@CPU_WS
    // asm: 	POP	R0
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FEED_WATCHDOG", 0, 0);
    UNIMPL();
}

void FEED_WATCHDOG_HARD(void)
{
    // asm: 	PUSH	DP
    // asm: 	PUSH	R0
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	@CPU_WS,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	1068h,R0
    // asm: 	STI	R0,@CPU_WS
    // asm: 	LDP	@SYSCNTL
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	XOR	WDOG,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	POP	R0
    // asm: 	LDP	@CPU_WS
    // asm: 	STI	R0,@CPU_WS
    // asm: 	POP	R0
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FEED_WATCHDOG_HARD", 0, 0);
    UNIMPL();
}

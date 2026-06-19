#ifndef CUSA_H
#define CUSA_H

#include "port.h"

/* Generated from asm/CUSA.ASM. */

// CUSA.ASM
// asm: 	fbss	COMMINTM,1	;0       if NON-linked
extern int COMMINTM;
// asm: 	pbss	_MODE,1		;machine mode
extern int _MODE;
// asm: 	.bss	NFRAMES,1	;number of frames passed since last update
extern int NFRAMES;
// asm: 	.bss	INFRAMES,1
extern int INFRAMES;
// asm: 	.bss	FRAMRATE,1	;MIN frames allowed-1
extern int FRAMRATE;
// asm: 	.bss	IFRAMES,1
extern int IFRAMES;
// asm: 	.bss	ERRORN,1
extern int ERRORN;
// asm: 	.bss	ERRORO,1
extern int ERRORO;
// asm: 	.bss	FRAMETIME,1	;TMR_FRAME
extern int FRAMETIME;
// asm: 	.bss	SWITCHBUTS,1
extern int SWITCHBUTS;
// asm: 	.bss	BGNDCOLA,1	;color 0 in COLORRAM
extern int BGNDCOLA;
// asm: 	.bss	DIAG_ACTIVE,1
extern int DIAG_ACTIVE;
// asm: 	.bss	GAME_TIMER,1	;FL	minutes since last buyin
extern int GAME_TIMER;
// asm: 	pbss	SYSCNTL,1	;shadow location
extern int SYSCNTL;
// asm: 	pbss	_pot0,1		;steering wheel
extern int _pot0;
// asm: 	.bss	_pot1,1		;accel pedal
extern int _pot1;
// asm: 	.bss	_pot2,1		;brake pedal
extern int _pot2;
// asm: 	.bss	RDPOT,1
extern int RDPOT;
// asm: 	.bss	_sectime,1	;one second timer
extern int _sectime;
// asm: 	.bss	CLEARRDY,1	;clear screen is ready (1 = swap pages and clear screen)
extern int CLEARRDY;
// asm: 	.bss	NOAERASE,1	;NO AUTOERASE FLAG
extern int NOAERASE;
// asm: 	.bss	NOSWAP,1	;NO PAGE SWAPPING
extern int NOSWAP;
// asm: 	.bss	DISPLAY_PAGE,1
extern int DISPLAY_PAGE;
// asm: 	fbss	MPROC_TIK,1
extern int MPROC_TIK;
// asm: 	fbss	WDHIT,1
extern int WDHIT;
// asm: 	pbss	_newbut,0
extern int _newbut;
// asm: 	pbss	SWRAM,3       		;SWITCH STATE STORE N,N-1,N-2
extern int SWRAM[];
// asm: 	pbss	DIPRAM,1		;DIP SWITCH RAM COPY
extern int DIPRAM;
// asm: 	fbss	BUTTON_TIKS,1
extern int BUTTON_TIKS;
// asm: 	pbss	BUTTON_STATUS,1
extern int BUTTON_STATUS;
// asm: 	.bss	OLD_BUTTON_STATUS,1
extern int OLD_BUTTON_STATUS;
// asm: 	.bss	TIMEFRAME,1
extern int TIMEFRAME;
// asm: 	.bss	TIMEX,1
extern int TIMEX;
// asm: 	.bss	TIMECLR,1
extern int TIMECLR;
// asm: 	.bss	TIMERAM,50
extern int TIMERAM[];
// asm: 		.bss	STATE_NUM,1
extern int STATE_NUM;
// asm: 		.bss	STATE_TIK,1
extern int STATE_TIK;
// asm: 	.bss	BUTTON_IBO,1
extern int BUTTON_IBO;
// asm: 	.bss	BUTTON_TIK,1
extern int BUTTON_TIK;
void FAKEDIAG(void);
void FEED_WATCHDOG_HARD(void);

#endif /* CUSA_H */

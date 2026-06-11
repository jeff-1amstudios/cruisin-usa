#ifndef CUSA_H
#define CUSA_H

/* Generated from asm/CUSA.ASM. */

// asm: 	fbss	COMMINTM,1	;0       if NON-linked
// addr: 0x008099DF
extern int COMMINTM;
// asm: 	pbss	_MODE,1		;machine mode
// addr: 0x0000C8F5
extern int _MODE;
// asm: 	.bss	NFRAMES,1	;number of frames passed since last update
// addr: 0x0000C95F
extern int NFRAMES;
// asm: 	.bss	INFRAMES,1
// addr: 0x0000C960
extern int INFRAMES;
// asm: 	.bss	FRAMRATE,1	;MIN frames allowed-1
// addr: 0x0000C961
extern int FRAMRATE;
// asm: 	.bss	IFRAMES,1
// addr: 0x0000C962
extern int IFRAMES;
// asm: 	.bss	ERRORN,1
// addr: 0x0000C964
extern int ERRORN;
// asm: 	.bss	ERRORO,1
// addr: 0x0000C965
extern int ERRORO;
// asm: 	.bss	FRAMETIME,1	;TMR_FRAME
// addr: 0x0000C966
extern int FRAMETIME;
// asm: 	.bss	SWITCHBUTS,1
// addr: 0x0000C967
extern int SWITCHBUTS;
// asm: 	.bss	BGNDCOLA,1	;color 0 in COLORRAM
// addr: 0x0000C968
extern int BGNDCOLA;
// asm: 	.bss	DIAG_ACTIVE,1
// addr: 0x0000C969
extern int DIAG_ACTIVE;
// asm: 	.bss	GAME_TIMER,1	;FL	minutes since last buyin
// addr: 0x0000C96A
extern int GAME_TIMER;
// asm: 	pbss	SYSCNTL,1	;shadow location
// addr: 0x0000C8F7
extern int SYSCNTL;
// asm: 	pbss	_pot0,1		;steering wheel
// addr: 0x0000C8F8
extern int _pot0;
// asm: 	.bss	_pot1,1		;accel pedal
// addr: 0x0000C96B
extern int _pot1;
// asm: 	.bss	_pot2,1		;brake pedal
// addr: 0x0000C96C
extern int _pot2;
// asm: 	.bss	RDPOT,1
// addr: 0x0000C96D
extern int RDPOT;
// asm: 	.bss	_sectime,1	;one second timer
// addr: 0x0000C96E
extern int _sectime;
// asm: 	.bss	CLEARRDY,1	;clear screen is ready (1 = swap pages and clear screen)
// addr: 0x0000C96F
extern int CLEARRDY;
// asm: 	.bss	NOAERASE,1	;NO AUTOERASE FLAG
// addr: 0x0000C970
extern int NOAERASE;
// asm: 	.bss	NOSWAP,1	;NO PAGE SWAPPING
// addr: 0x0000C971
extern int NOSWAP;
// asm: 	.bss	DISPLAY_PAGE,1
// addr: 0x0000C972
extern int DISPLAY_PAGE;
// asm: 	fbss	MPROC_TIK,1
// addr: 0x008099E0
extern int MPROC_TIK;
// asm: 	fbss	WDHIT,1
// addr: 0x008099E1
extern int WDHIT;
// asm: 	pbss	_newbut,0
// addr: 0x0000C8F9
extern int _newbut;
// asm: 	pbss	SWRAM,3       		;SWITCH STATE STORE N,N-1,N-2
// addr: 0x0000C8F9
extern int SWRAM[3];
// asm: 	pbss	DIPRAM,1		;DIP SWITCH RAM COPY
// addr: 0x0000C8FC
extern int DIPRAM;
// asm: 	fbss	BUTTON_TIKS,1
// addr: 0x008099E2
extern int BUTTON_TIKS;
// asm: 	pbss	BUTTON_STATUS,1
// addr: 0x0000C8FD
extern int BUTTON_STATUS;
// asm: 	.bss	OLD_BUTTON_STATUS,1
// addr: 0x0000C973
extern int OLD_BUTTON_STATUS;
// asm: 	.bss	TIMEFRAME,1
// addr: 0x0000C974
extern int TIMEFRAME;
// asm: 	.bss	TIMEX,1
// addr: 0x0000C975
extern int TIMEX;
// asm: 	.bss	TIMECLR,1
// addr: 0x0000C976
extern int TIMECLR;
// asm: 	.bss	TIMERAM,50
// addr: 0x0000C977
extern int TIMERAM[50];
// asm: 		.bss	STATE_NUM,1
// addr: 0x0000C9AA
extern int STATE_NUM;
// asm: 		.bss	STATE_TIK,1
// addr: 0x0000C9AB
extern int STATE_TIK;
// asm: 	.bss	BUTTON_IBO,1
// addr: 0x0000C9AC
extern int BUTTON_IBO;
// asm: 	.bss	BUTTON_TIK,1
// addr: 0x0000C9AD
extern int BUTTON_TIK;

#endif /* CUSA_H */

#ifndef CUSA_H
#define CUSA_H

/* Generated from asm/CUSA.ASM. */

// asm: 	fbss	COMMINTM,1	;0       if NON-linked
#define COMMINTM 0x008099DF
// asm: 	pbss	_MODE,1		;machine mode
#define _MODE 0x0000C8F5
// asm: 	.bss	NFRAMES,1	;number of frames passed since last update
#define NFRAMES 0x0000C95F
// asm: 	.bss	INFRAMES,1
#define INFRAMES 0x0000C960
// asm: 	.bss	FRAMRATE,1	;MIN frames allowed-1
#define FRAMRATE 0x0000C961
// asm: 	.bss	IFRAMES,1
#define IFRAMES 0x0000C962
// asm: 	.bss	ERRORN,1
#define ERRORN 0x0000C964
// asm: 	.bss	ERRORO,1
#define ERRORO 0x0000C965
// asm: 	.bss	FRAMETIME,1	;TMR_FRAME
#define FRAMETIME 0x0000C966
// asm: 	.bss	SWITCHBUTS,1
#define SWITCHBUTS 0x0000C967
// asm: 	.bss	BGNDCOLA,1	;color 0 in COLORRAM
#define BGNDCOLA 0x0000C968
// asm: 	.bss	DIAG_ACTIVE,1
#define DIAG_ACTIVE 0x0000C969
// asm: 	.bss	GAME_TIMER,1	;FL	minutes since last buyin
#define GAME_TIMER 0x0000C96A
// asm: 	pbss	SYSCNTL,1	;shadow location
#define SYSCNTL 0x0000C8F7
// asm: 	pbss	_pot0,1		;steering wheel
#define _pot0 0x0000C8F8
// asm: 	.bss	_pot1,1		;accel pedal
#define _pot1 0x0000C96B
// asm: 	.bss	_pot2,1		;brake pedal
#define _pot2 0x0000C96C
// asm: 	.bss	RDPOT,1
#define RDPOT 0x0000C96D
// asm: 	.bss	_sectime,1	;one second timer
#define _sectime 0x0000C96E
// asm: 	.bss	CLEARRDY,1	;clear screen is ready (1 = swap pages and clear screen)
#define CLEARRDY 0x0000C96F
// asm: 	.bss	NOAERASE,1	;NO AUTOERASE FLAG
#define NOAERASE 0x0000C970
// asm: 	.bss	NOSWAP,1	;NO PAGE SWAPPING
#define NOSWAP 0x0000C971
// asm: 	.bss	DISPLAY_PAGE,1
#define DISPLAY_PAGE 0x0000C972
// asm: 	fbss	MPROC_TIK,1
#define MPROC_TIK 0x008099E0
// asm: 	fbss	WDHIT,1
#define WDHIT 0x008099E1
// asm: 	pbss	_newbut,0
#define _newbut 0x0000C8F9
// asm: 	pbss	SWRAM,3       		;SWITCH STATE STORE N,N-1,N-2
#define SWRAM 0x0000C8F9
// asm: 	pbss	DIPRAM,1		;DIP SWITCH RAM COPY
#define DIPRAM 0x0000C8FC
// asm: 	fbss	BUTTON_TIKS,1
#define BUTTON_TIKS 0x008099E2
// asm: 	pbss	BUTTON_STATUS,1
#define BUTTON_STATUS 0x0000C8FD
// asm: 	.bss	OLD_BUTTON_STATUS,1
#define OLD_BUTTON_STATUS 0x0000C973
// asm: 	.bss	TIMEFRAME,1
#define TIMEFRAME 0x0000C974
// asm: 	.bss	TIMEX,1
#define TIMEX 0x0000C975
// asm: 	.bss	TIMECLR,1
#define TIMECLR 0x0000C976
// asm: 	.bss	TIMERAM,50
#define TIMERAM 0x0000C977
// asm: 		.bss	STATE_NUM,1
#define STATE_NUM 0x0000C9AA
// asm: 		.bss	STATE_TIK,1
#define STATE_TIK 0x0000C9AB
// asm: 	.bss	BUTTON_IBO,1
#define BUTTON_IBO 0x0000C9AC
// asm: 	.bss	BUTTON_TIK,1
#define BUTTON_TIK 0x0000C9AD

#endif /* CUSA_H */

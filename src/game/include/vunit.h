#ifndef VUNIT_H
#define VUNIT_H

/* Generated from asm/VUNIT.EQU. */

// VUNIT.EQU
// 
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED

// asm: DEBUG		.set	0	;0 = NO DEBUG CODE
#define DEBUG 0 //0 = NO DEBUG CODE
// asm: PRINTER		.set	0	;1 = PRINTER IS AVAILABLE
#define PRINTER 0 //1 = PRINTER IS AVAILABLE
// asm: STATISTICS	.set	0	;1 = BOG OUT STATISTICS GENERATION
#define STATISTICS 0 //1 = BOG OUT STATISTICS GENERATION
// asm: CODE_CHECK	.set	0	;1 = LOCK ON CODE INTEGRITY ERROR
#define CODE_CHECK 0 //1 = LOCK ON CODE INTEGRITY ERROR
// COMMP		.set	1	;1 = COMMUNICATION CODE ACTIVE
// see also: COMM.EQU

// ----------------------------------------------------------------------------
// MEMORY MAP
// 
// asm: FASTRAM		.set	0000000h	;PROGRAM RAM
#define FASTRAM 0x0000000 //PROGRAM RAM
// asm: SCREEN		.set	0900000h 	;VIDEO SCREEN RAM (512 HORIZ X 1024 VERT)
#define SCREEN 0x0900000 //VIDEO SCREEN RAM (512 HORIZ X 1024 VERT)
// asm: SCREEN0		.set	0900000h 	;PAGE 0		3 HW STATES
#define SCREEN0 0x0900000 //PAGE 0		3 HW STATES
// asm: SCREEN1		.set	0940000h 	;PAGE 1		PAL BITS 15-8, COL BITS 7-0
#define SCREEN1 0x0940000 //PAGE 1		PAL BITS 15-8, COL BITS 7-0
// asm: CMOS		.set	09C0000H 	;MASK=0FF000000h, LEN = 02000h   4 SW STATES
#define CMOS 0x09C0000 //MASK=0FF000000h, LEN = 02000h   4 SW STATES
// asm: COLORAM		.set	09E0000h 	;32K X 24 BITS RGB	2 SW STATES
#define COLORAM 0x09E0000 //32K X 24 BITS RGB	2 SW STATES
// asm: WAVERAM		.set	0A00000h	;WAVE RAM DMA ACCESSABLE 2D IMAGE STORE
#define WAVERAM 0x0A00000 //WAVE RAM DMA ACCESSABLE 2D IMAGE STORE

// asm: SND2		.set	09A0000h	;ONBOARD NEW SOUND PORT
#define SND2 0x09A0000 //ONBOARD NEW SOUND PORT

// asm: OUT1		.set	0996000h 	;IDE
#define OUT1 0x0996000 //IDE
// asm: SOUND		.set	0995000h 	;SOUND OUTPUT PORT
#define SOUND 0x0995000 //SOUND OUTPUT PORT
// asm: DIPSW		.set	0992000h 	;DIP SWITCHES
#define DIPSW 0x0992000 //DIP SWITCHES

// asm: SWITCH1		.set	0991060h 	;SWITCH INPUTS	(bits 15-8)
#define SWITCH1 0x0991060 //SWITCH INPUTS	(bits 15-8)
// asm: SWITCH2		.set	0991050h 	;SWITCH INPUTS	(bits 15-8)
#define SWITCH2 0x0991050 //SWITCH INPUTS	(bits 15-8)
// asm: SWITCH3		.set	0991030h 	;SWITCH INPUTS	(bits 15-8)
#define SWITCH3 0x0991030 //SWITCH INPUTS	(bits 15-8)

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// DMA CONTROL WORD
// 
// asm: DITHER		.set	02000h		;DITHER
#define DITHER 0x02000 //DITHER
// asm: CLIPEN		.set	01000h		;DO NOT CLIP THIS POLYGON
#define CLIPEN 0x01000 //DO NOT CLIP THIS POLYGON
// asm: ZS		.set	00800h		;ZERO SUPRESS
#define ZS 0x00800 //ZERO SUPRESS
// asm: NZR		.set	00400h		;NON-ZERO REPLACEMENT
#define NZR 0x00400 //NON-ZERO REPLACEMENT
// asm: METHOD		.set	00300h		;METHOD OF PLOTTING
#define METHOD 0x00300 //METHOD OF PLOTTING
// asm: FASTCC		.set	00200h		;FAST CONSTANT COLOR (NO IVs OR ADDR)
#define FASTCC 0x00200 //FAST CONSTANT COLOR (NO IVs OR ADDR)
// asm: TM		.set	00100h		;TEXTURE MAPPING
#define TM 0x00100 //TEXTURE MAPPING
// asm: CC		.set	00000h		;CONSTANT COLOR
#define CC 0x00000 //CONSTANT COLOR
// asm: COLOR		.set	000FFh		;COLOR FIELD
#define COLOR 0x000FF //COLOR FIELD
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FIFO EQUATES
// 

// asm: FIFO_STATUS	.set	0980082h	;READ ANYTIME, GENERAL STATUS (4 BITS) (READ ONLY)
#define FIFO_STATUS 0x0980082 //READ ANYTIME, GENERAL STATUS (4 BITS) (READ ONLY)
// asm: FIFO_STATUS_FD_CRITICAL	.set	08h	;FIFO DATA BUS IS BEING USED
#define FIFO_STATUS_FD_CRITICAL 0x08 //FIFO DATA BUS IS BEING USED
// asm: FIFO_STATUS_MAX_FLAG	.set	04h	;FIFO IS MAXED OUT (NO ENTRIES LEFT)
#define FIFO_STATUS_MAX_FLAG 0x04 //FIFO IS MAXED OUT (NO ENTRIES LEFT)
// asm: FIFO_STATUS_DMA_ACTIVE	.set	02h	;DMA IS PLOTTING AN IMAGE TO THE SCREEN (even if no fifo used)
#define FIFO_STATUS_DMA_ACTIVE 0x02 //DMA IS PLOTTING AN IMAGE TO THE SCREEN (even if no fifo used)
// asm: FIFO_STATUS_FIFO_NEMPTY	.set	01h	;FIFO IS NOT EMPTY
#define FIFO_STATUS_FIFO_NEMPTY 0x01 //FIFO IS NOT EMPTY

// asm: FIFO_CONTROL	.set	0980080h	;READ/WRITE  CONTROL REGISTER 0
#define FIFO_CONTROL 0x0980080 //READ/WRITE  CONTROL REGISTER 0
// asm: FIFO_CONTROL_DMA_RUNSEL	.set	08h	;(0) -> (1) ENABLES DMA TO RUN  user should set to 1 on powerup, is 0 on powerup otherwise
#define FIFO_CONTROL_DMA_RUNSEL 0x08 //(0) -> (1) ENABLES DMA TO RUN  user should set to 1 on powerup, is 0 on powerup otherwise
// asm: FIFO_CONTROL_COUNTER_LD	.set	04h	;(0) -> (0) ENABLES USER TO LOAD # OF ENTIRES IN FIFO  (mostly useless)
#define FIFO_CONTROL_COUNTER_LD 0x04 //(0) -> (0) ENABLES USER TO LOAD # OF ENTIRES IN FIFO  (mostly useless)
// asm: FIFO_CONTROL_FIFO_RST	.set	02h	;(1) -> (0) RESETS THE FIFO COUNTER (NOT FIFO)  (set to 1)
#define FIFO_CONTROL_FIFO_RST 0x02 //(1) -> (0) RESETS THE FIFO COUNTER (NOT FIFO)  (set to 1)
// asm: FIFO_CONTROL_RST_CT_CHN	.set	01h	;(1) -> (0) CRT CONTROLLER RESET user should set to 0 on powerup
#define FIFO_CONTROL_RST_CT_CHN 0x01 //(1) -> (0) CRT CONTROLLER RESET user should set to 0 on powerup

// asm: FIFO_CONTROL_INIT	.set	FIFO_CONTROL_DMA_RUNSEL
#define FIFO_CONTROL_INIT FIFO_CONTROL_DMA_RUNSEL

// asm: FIFO_CNTR	.set	0980000h	;READ	FIFO COUNTER 15-0
#define FIFO_CNTR 0x0980000 //READ	FIFO COUNTER 15-0
// asm: FIFO_SIZE	.set	0980041h	;WRITE	MAX ENTRIES FOR FIFO  (271 FOR 4k FIFO)
#define FIFO_SIZE 0x0980041 //WRITE	MAX ENTRIES FOR FIFO  (271 FOR 4k FIFO)
// asm: FIFO_INC	.set	0980083h	;READ	THIS PERFORMS THE SAME FUNCTION AS FIFO
#define FIFO_INC 0x0980083 //READ	THIS PERFORMS THE SAME FUNCTION AS FIFO
// STATUS, BUT ALSO INCREMENTS THE FIFO ENTRIES
// asm: FIFO_ADDR	.set	0600000h	;WRITE
#define FIFO_ADDR 0x0600000 //WRITE

// 
// THIS IS UNTESTED 7/13/93
// to access the 'other' wave ram bank (for WRITING!!!)
// 1)	set FIFO_CONTROL_DMA_RUNSEL <- 0
// 2)	dma will not initiate another BLIT
// 3)	check the FIFO_STATUS_FD_CRITICAL in FIFO_STATUS
// until == 0
// 4)	now able to update DMA_SETUP  (DMA_WAVE_RAM_BANK)
// 5)	set FIFO_CONTROL_DMA_RUNSEL <- 1
// 
// This method is also usable for opto counters.
// 

// 
// DMA EQUATES
// 
// 

// asm: DMA_SETUP	.set	0980040h	;READ/WRITE	A VERY IMPORTANT REGISTER
#define DMA_SETUP 0x0980040 //READ/WRITE	A VERY IMPORTANT REGISTER
// asm: DMA_WDVD_DISABLE	.set	08000h	;0 0	LSI TESTING ALWAYS SET TO 0  (HW BACKDOOR)
#define DMA_WDVD_DISABLE 0x08000 //0 0	LSI TESTING ALWAYS SET TO 0  (HW BACKDOOR)
// asm: DMA_CHIP_TRISTATE	.set	04000h	;0 0	LSI TESTING ALWAYS SET TO 0  (HW BACKDOOR)
#define DMA_CHIP_TRISTATE 0x04000 //0 0	LSI TESTING ALWAYS SET TO 0  (HW BACKDOOR)
// asm: DMA_CHIP_DISABLE	.set	02000h	;0 0	IF SET TO 1, CHIP LOCKUP ONLY UNDOABLE BY CHIP RESET
#define DMA_CHIP_DISABLE 0x02000 //0 0	IF SET TO 1, CHIP LOCKUP ONLY UNDOABLE BY CHIP RESET
// asm: DMA_TEST_TOGGLE		.set	01000h	;0 0	LSI TESTING ALWAYS SET TO 0  (HW BACKDOOR)
#define DMA_TEST_TOGGLE 0x01000 //0 0	LSI TESTING ALWAYS SET TO 0  (HW BACKDOOR)
// asm: DMA_TEST_MODE		.set	00800h	;0 0	LSI TESTING ALWAYS SET TO 0  (HW BACKDOOR)
#define DMA_TEST_MODE 0x00800 //0 0	LSI TESTING ALWAYS SET TO 0  (HW BACKDOOR)
// asm: DMA_MODE_32		.set	00400h	;0 0	FUTURE EXPANSION ALWAYS SET TO 0
#define DMA_MODE_32 0x00400 //0 0	FUTURE EXPANSION ALWAYS SET TO 0
// asm: DMA_FIFO_INC_DISABLE2	.set	00200h	;0 1	HW KLUDGE SEE MARK ALWAYS SET TO 1
#define DMA_FIFO_INC_DISABLE2 0x00200 //0 1	HW KLUDGE SEE MARK ALWAYS SET TO 1
// asm: DMA_FIFO_INC_DISABLE	.set	00100h	;0 0	HW KLUDGE TO DISABLE
#define DMA_FIFO_INC_DISABLE 0x00100 //0 0	HW KLUDGE TO DISABLE
// asm: DMA_DITHER_PHASE	.set	00080h	;0 0	(0 = EVEN ON, 1 = EVEN OFF)
#define DMA_DITHER_PHASE 0x00080 //0 0	(0 = EVEN ON, 1 = EVEN OFF)
// asm: DMA_POLY_ABORT_DISABLE	.set	00040h	;0 0	(1 == DISABLE THE BONEHEAD ABORTION, EX ALL NEG.))
#define DMA_POLY_ABORT_DISABLE 0x00040 //0 0	(1 == DISABLE THE BONEHEAD ABORTION, EX ALL NEG.))
// asm: DMA_REVERSE_WRITE_DIS	.set	00020h	;0 0	(1 == DISABLE THE FAST MATH REVERSE PLOTTING)
#define DMA_REVERSE_WRITE_DIS 0x00020 //0 0	(1 == DISABLE THE FAST MATH REVERSE PLOTTING)
// asm: DMA_NO_FIFO_BIT		.set	00010h	;0 0	"DONT GRAB DATA FROM THE FIFO, DMA REGS ALREADY SETUP"
#define DMA_NO_FIFO_BIT 0x00010 //0 0	"DONT GRAB DATA FROM THE FIFO, DMA REGS ALREADY SETUP"
// asm: DMA_WAVE_RAM_BANK	.set	00008h	;0 0	FOR WRITING INTO THE BANK
#define DMA_WAVE_RAM_BANK 0x00008 //0 0	FOR WRITING INTO THE BANK
// asm: DMA_DMA_WRITE_PAGE	.set	00004h	;0 0	WHICH PAGE TO PLOT (0 = 0, 1 = 1)
#define DMA_DMA_WRITE_PAGE 0x00004 //0 0	WHICH PAGE TO PLOT (0 = 0, 1 = 1)
// asm: DMA_ROW_TRANSFER_ENABLE	.set	00002h	;0 0	SRT ENABLE (UNTESTED 7/13/93)
#define DMA_ROW_TRANSFER_ENABLE 0x00002 //0 0	SRT ENABLE (UNTESTED 7/13/93)
// asm: DMA_VIDEO_PAG_DISPLAYED	.set	00001h	;0 0	WHICH PAGE TO SCREEN (0 = 0, 1 = 1)
#define DMA_VIDEO_PAG_DISPLAYED 0x00001 //0 0	WHICH PAGE TO SCREEN (0 = 0, 1 = 1)

// asm: DMA_SETUP_INIT		.set	DMA_FIFO_INC_DISABLE2
#define DMA_SETUP_INIT DMA_FIFO_INC_DISABLE2
// DMA_SETUP_INIT		.set	DMA_FIFO_INC_DISABLE2|DMA_NO_FIFO_BIT

// asm: DMA_CTRL	.set	0980000h	;DMA CONTROL
#define DMA_CTRL 0x0980000 //DMA CONTROL
// asm: DMA_CMAP	.set	0980001h	;PALETTE NUMBER
#define DMA_CMAP 0x0980001 //PALETTE NUMBER
// asm: DMA_AX		.set	0980002h	;BITMAP AX (BITS 15-0)
#define DMA_AX 0x0980002 //BITMAP AX (BITS 15-0)
// asm: DMA_AY		.set	0980003h	;
#define DMA_AY 0x0980003
// asm: DMA_AZ		.set	0980004h	;
#define DMA_AZ 0x0980004
// asm: DMA_BX		.set	0980005h	;BITMAP BX (BITS 15-0)
#define DMA_BX 0x0980005 //BITMAP BX (BITS 15-0)
// asm: DMA_BY		.set	0980006h	;
#define DMA_BY 0x0980006
// asm: DMA_BZ		.set	0980007h	;
#define DMA_BZ 0x0980007
// asm: DMA_CX		.set	0980008h	;BITMAP CX (BITS 15-0)
#define DMA_CX 0x0980008 //BITMAP CX (BITS 15-0)
// asm: DMA_CY		.set	0980009h	;
#define DMA_CY 0x0980009
// asm: DMA_CZ		.set	098000Ah	;
#define DMA_CZ 0x098000A
// asm: DMA_DX		.set	098000Bh	;BITMAP DX (BITS 15-0)
#define DMA_DX 0x098000B //BITMAP DX (BITS 15-0)
// asm: DMA_DY		.set	098000Ch	;
#define DMA_DY 0x098000C
// asm: DMA_DZ		.set	098000Dh	;
#define DMA_DZ 0x098000D
// asm: DMA_IVA		.set	098000Eh	;SOURCE AY (BITS 15-8) AX (BITS 7-0)
#define DMA_IVA 0x098000E //SOURCE AY (BITS 15-8) AX (BITS 7-0)
// asm: DMA_IVB		.set	098000Fh	;SOURCE BY (BITS 15-8) BX (BITS 7-0)
#define DMA_IVB 0x098000F //SOURCE BY (BITS 15-8) BX (BITS 7-0)
// asm: DMA_IVC		.set	0980010h	;SOURCE CY (BITS 15-8) CX (BITS 7-0)
#define DMA_IVC 0x0980010 //SOURCE CY (BITS 15-8) CX (BITS 7-0)
// asm: DMA_IVD		.set	0980011h	;SOURCE DY (BITS 15-8) DX (BITS 7-0)
#define DMA_IVD 0x0980011 //SOURCE DY (BITS 15-8) DX (BITS 7-0)
// asm: DMA_LINE	.set	0980012h	;Y LINE OFFSET (BITS 14-0)
#define DMA_LINE 0x0980012 //Y LINE OFFSET (BITS 14-0)

// DMA_CTRL BITS
// asm: DMA_DITHER	.set	2000h		;DITHER
#define DMA_DITHER 0x2000 //DITHER
// asm: DMA_CLIPEN	.set	1000h		;CLIP ENABLE
#define DMA_CLIPEN 0x1000 //CLIP ENABLE
// asm: DMA_ZWRSUP	.set	0800h		;ZWR SUPPRESS
#define DMA_ZWRSUP 0x0800 //ZWR SUPPRESS
// asm: DMA_NZR		.set	0400h		;NON ZERO REPLACE
#define DMA_NZR 0x0400 //NON ZERO REPLACE
// asm: DMA_METH1	.set	0200h		;FAST CONSTANT COLORED
#define DMA_METH1 0x0200 //FAST CONSTANT COLORED
// asm: DMA_METH0	.set	0100h		;TEXTURE MAP
#define DMA_METH0 0x0100 //TEXTURE MAP
// asm: DMA_COLOR	.set	00FFh		;COLOR FIELD
#define DMA_COLOR 0x00FF //COLOR FIELD

// 
// CRT CONTROL REGISTERS
// 
// 
// asm: CRT_VCNT	.set	0980020h	;READ	the vertical line count (bits 0-8)
#define CRT_VCNT 0x0980020 //READ	the vertical line count (bits 0-8)

// asm: CRT_SETUP	.set	0980020h	;WRITE	CRT SETUP REGISTER
#define CRT_SETUP 0x0980020 //WRITE	CRT SETUP REGISTER
// asm: CRT_HADDRINC	.set	0980021h	;WRITE	horizontal addr increment(bits 09-00)	01ff
#define CRT_HADDRINC 0x0980021 //WRITE	horizontal addr increment(bits 09-00)	01ff
// asm: CRT_HBLKSTART	.set	0980022h	;WRITE	horizontal blank start	(bits 09-00)	01fe
#define CRT_HBLKSTART 0x0980022 //WRITE	horizontal blank start	(bits 09-00)	01fe
// asm: CRT_HSYNCSTART	.set	0980023h	;WRITE	horizontal sync start	(bits 09-00)	0226
#define CRT_HSYNCSTART 0x0980023 //WRITE	horizontal sync start	(bits 09-00)	0226
// asm: CRT_HSYNCEND	.set	0980024h	;WRITE	horizontal sync end	(bits 09-00)	025e
#define CRT_HSYNCEND 0x0980024 //WRITE	horizontal sync end	(bits 09-00)	025e
// asm: CRT_HBLKEND	.set	0980025h	;WRITE	horizontal blank end	(bits 09-00)	029f
#define CRT_HBLKEND 0x0980025 //WRITE	horizontal blank end	(bits 09-00)	029f
// asm: CRT_HTTL	.set	0980026h	;WRITE	horizontal total	(bits 09-00)	02a0
#define CRT_HTTL 0x0980026 //WRITE	horizontal total	(bits 09-00)	02a0
// asm: CRT_VBLKSTART	.set	0980027h	;WRITE	vert blank start	(bits 08-00)	018f
#define CRT_VBLKSTART 0x0980027 //WRITE	vert blank start	(bits 08-00)	018f
// asm: CRT_SYNCSTART	.set	0980028h	;WRITE	sync start		(bits 08-00)	0195
#define CRT_SYNCSTART 0x0980028 //WRITE	sync start		(bits 08-00)	0195
// asm: CRT_SYNCEND	.set	0980029h	;WRITE	sync end		(bits 08-00)	0198
#define CRT_SYNCEND 0x0980029 //WRITE	sync end		(bits 08-00)	0198
// asm: CRT_VBLK	.set	098002Ah	;WRITE	blank end		(bits 08-00)	01b0
#define CRT_VBLK 0x098002A //WRITE	blank end		(bits 08-00)	01b0
// asm: CRT_VTTL	.set	098002Bh	;WRITE	vertical total		(bits 08-00)	01b0
#define CRT_VTTL 0x098002B //WRITE	vertical total		(bits 08-00)	01b0

// asm: CRT_SETUP_DIVIDE	.set	8000h	;DIVIDE VIDCLK BY 2
#define CRT_SETUP_DIVIDE 0x8000 //DIVIDE VIDCLK BY 2
// asm: CRT_SETUP_CSYNCV	.set	4000h	;CSYNC OUT ON VERT
#define CRT_SETUP_CSYNCV 0x4000 //CSYNC OUT ON VERT
// asm: CRT_SETUP_CSYNCH	.set	2000h	;CSYNC OUT ON HORZ
#define CRT_SETUP_CSYNCH 0x2000 //CSYNC OUT ON HORZ
// asm: CRT_SETUP_ISYNCV	.set	1000h	;INVERT VSYNC
#define CRT_SETUP_ISYNCV 0x1000 //INVERT VSYNC
// asm: CRT_SETUP_ISYNCH	.set	0800h	;INVERT HSYNC
#define CRT_SETUP_ISYNCH 0x0800 //INVERT HSYNC
// asm: CRT_SETUP_ICSYNC	.set	0400h	;INVERT CSYNC
#define CRT_SETUP_ICSYNC 0x0400 //INVERT CSYNC
// asm: CRT_SETUP_RESERVED	.set	0200h	;RESERVED
#define CRT_SETUP_RESERVED 0x0200 //RESERVED
// asm: CRT_SETUP_DISP_INT_LN	.set	01FFh	;MASK DISPLAY INT LINE
#define CRT_SETUP_DISP_INT_LN 0x01FF //MASK DISPLAY INT LINE

// asm: CRT_SETUP_INIT		.set	399|CRT_SETUP_CSYNCH
#define CRT_SETUP_INIT (399|CRT_SETUP_CSYNCH)
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// SYSTEM CONTROL FLAGS
// asm: SYSCNTLR	.set	0994000h	;this is the REAL LOCATION (8 bits only)
#define SYSCNTLR 0x0994000 //this is the REAL LOCATION (8 bits only)
// asm: 		.globl	_SYSCNTL	;this is the SHADOW LOCATION
// asm: LED_OFF		.set	0000080h	;turn led off
#define LED_OFF 0x0000080 //turn led off

// asm: ATOD_RD		.set	040h
#define ATOD_RD 0x040
// asm: ATOD_WR		.set	020h
#define ATOD_WR 0x020
// asm: ATOD_MASK	.set	060h
#define ATOD_MASK 0x060
// asm: ATOD_R		.set	0993000h	;ATOD register
#define ATOD_R 0x0993000 //ATOD register

// asm: WDOG		.set	0008h		;WATCHDOG
#define WDOG 0x0008 //WATCHDOG
// 
// once every 1.6 seconds
// worst case 1.2 seconds
// 

// asm: GENERAL_WP	.set	010h		;WRITE PROTECT VECTOR,IMAGE
#define GENERAL_WP 0x010 //WRITE PROTECT VECTOR,IMAGE

// asm: SND2_RESET	.set	002h		;0 = RESET ONBOARD SOUND SYSTEM
#define SND2_RESET 0x002 //0 = RESET ONBOARD SOUND SYSTEM

// asm: SYSCNTL_INIT	.set	0FFh		;FIFO_RESET|FIFO_RETRANS|RUN_SELECT|ATOD_MASK|GENERAL_WP
#define SYSCNTL_INIT 0x0FF //FIFO_RESET|FIFO_RETRANS|RUN_SELECT|ATOD_MASK|GENERAL_WP

// asm: CMOS_WP_WORD	.set	0995020h	;CMOS WRITE PROTECT WORD
#define CMOS_WP_WORD 0x0995020 //CMOS WRITE PROTECT WORD
// asm: CMOS_WP		.set	0C00h		;CMOS WP DISABLED  (ON = 0)
#define CMOS_WP 0x0C00 //CMOS WP DISABLED  (ON = 0)
// ----------------------------------------------------------------------------

// asm: MINUS_CHAR	.set	'>'
#define MINUS_CHAR '>'

// ----------------------------------------------------------------------------
// SWITCH EQUATES
// 
// SWITCH =   (SWITCH1 >> 16) | (SWITCH3 << 16)
// 
// 

// asm: SW_COIN1	.set	00000001h
#define SW_COIN1 0x00000001
// asm: SW_COIN2	.set	00000002h
#define SW_COIN2 0x00000002
// asm: SW_START	.set	00000004h
#define SW_START 0x00000004
// asm: SW_TILT		.set	00000008h
#define SW_TILT 0x00000008
// asm: SW_DIAG		.set	00000010h
#define SW_DIAG 0x00000010
// SW_RES		.set	00000020h
// asm: SW_COINSRV	.set	00000040h
#define SW_COINSRV 0x00000040
// asm: SW_COIN3	.set	00000080h
#define SW_COIN3 0x00000080
// asm: SW_VOLMINUS	.set	00000100h
#define SW_VOLMINUS 0x00000100
// asm: SW_VOLPLUS	.set	00000200h
#define SW_VOLPLUS 0x00000200
// asm: SW_4TH		.set	00000400h	;4th
#define SW_4TH 0x00000400 //4th
// asm: SW_3RD		.set	00000800h	;3rd
#define SW_3RD 0x00000800 //3rd
// asm: SW_2ND		.set	00001000h	;2nd
#define SW_2ND 0x00001000 //2nd
// asm: SW_1ST		.set	00002000h	;1st
#define SW_1ST 0x00002000 //1st
// asm: SW_COIN4	.set	00004000h
#define SW_COIN4 0x00004000
// SW_RES		.set	00008000h
// asm: SW_BRAKE	.set	00010000h
#define SW_BRAKE 0x00010000
// asm: SW_RADIO	.set	00020000h
#define SW_RADIO 0x00020000
// asm: SW_LOW		.set	00040000h
#define SW_LOW 0x00040000
// asm: SW_DEBUG	.set	00080000h
#define SW_DEBUG 0x00080000
// asm: SW_VIEW		.set	00100000h
#define SW_VIEW 0x00100000
// asm: SW_VIEW0	.set	00100000h
#define SW_VIEW0 0x00100000
// asm: SW_VIEW1	.set	00200000h
#define SW_VIEW1 0x00200000
// asm: SW_VIEW2	.set	00400000h
#define SW_VIEW2 0x00400000
// asm: SW_VIEW3	.set	00800000h
#define SW_VIEW3 0x00800000

// WHEN SHIFTED DOWN 16 BITS
// asm: SW_BRAKE_H	.set	0001h
#define SW_BRAKE_H 0x0001
// asm: SW_RADIO_H	.set	0002h
#define SW_RADIO_H 0x0002
// asm: SW_LOW_H	.set	0004h
#define SW_LOW_H 0x0004
// asm: SW_DEBUG_H	.set	0008h
#define SW_DEBUG_H 0x0008
// asm: SW_VIEW_H	.set	0010h
#define SW_VIEW_H 0x0010
// asm: SW_VIEW0_H	.set	0010h
#define SW_VIEW0_H 0x0010
// asm: SW_VIEW1_H	.set	0020h
#define SW_VIEW1_H 0x0020
// asm: SW_VIEW2_H	.set	0040h
#define SW_VIEW2_H 0x0040
// asm: SW_VIEW3_H	.set	0080h
#define SW_VIEW3_H 0x0080
// ----------------------------------------------------------------------------

// asm: 	.globl	DIPRAM
extern int DIPRAM;
// ----------------------------------------------------------------------------
// DIP SWITCH SETTINGS
// SW 1 (RESET)
// SW 2
// asm: DIP_DIAG	.set	  80h	;#1	DIAGNOSTIC MODE
#define DIP_DIAG 0x80 //#1	DIAGNOSTIC MODE
// asm: DIP_MOTION	.set	  40h	;#2	MOTION CABINET
#define DIP_MOTION 0x40 //#2	MOTION CABINET
// asm: DIP_STANDUP	.set	  20h	;#3	STANDUP
#define DIP_STANDUP 0x20 //#3	STANDUP
// asm: DIP_FREEZE	.set	  10h	;#4	FREEZE SCREEN
#define DIP_FREEZE 0x10 //#4	FREEZE SCREEN
// .set	   8h	;#5
// asm: DIP_COMMP	.set	   4h	;#6	IS LINKING PRESENT?
#define DIP_COMMP 0x4 //#6	IS LINKING PRESENT?
// asm: DIP_LINK0	.set	   2h	;#7	see comm dip settings
#define DIP_LINK0 0x2 //#7	see comm dip settings
// asm: DIP_LINK1	.set	   1h	;#8	see comm dip settings
#define DIP_LINK1 0x1 //#8	see comm dip settings
// SW 3
// .set	8000h	;#1	| \
// .set	4000h	;#2	|   \
// .set	2000h	;#3	|      COIN MODE
// .set	1000h	;#4	|
// .set	 800h	;#5	|   /
// .set	 400h	;#6	| /
// .set	 200h	;#7	(potential extra coin mode)
// asm: DIP_COINCNTR	.set	 100h	;#8	FREE PLAY
#define DIP_COINCNTR 0x100 //#8	FREE PLAY
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// DIP SWITCHES
// 
// asm: CMDP_M		.set	3h
#define CMDP_M 0x3
// asm: CMDP_MASTER	.set	1h
#define CMDP_MASTER 0x1
// asm: CMDP_SLAVE	.set	0
#define CMDP_SLAVE 0
// ----------------------------------------------------------------------------

#endif /* VUNIT_H */

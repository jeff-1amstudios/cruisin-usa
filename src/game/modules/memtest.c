#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/macs.h"
#include "../include/sys.h"
#include "../include/diag.h"
#include "../include/cksum.h"
#include "../include/globals.h"
#include "../include/checksum.h"
#include "../include/memtest.h"

/*
 * Source module: asm/MEMTEST.ASM
 */

// *----------------------------------------------------------------------------
// *MEMTEST.ASM
// *
// *COPYRIGHT (C) 1994  BY TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *CHIP TEST ROUTINES
// *
// *
// *MEMORY WAIT STATE ACCESS NOTES
// *
// *NORMAL=WAIT STATES SET TO SW=2 OR HARDWARE WHICHEVER RELEASES FIRST
// *FOR COLOR RAM SET T0 SW=1 OR HARDWARE WHICHEVER RELEASES FIRST
// *FOR BITMAP OR WAVERAM MODE HARDWARE ONLY
// *AFTER BITMAP READ, READ 0 AFTERWARD TO RESET HDWE.
// *FOR A-D SET WAIT TO SW=4 ONLY
// *
// 			;CHECKSUM_GEN	.set	1
// *----------------------------------------------------------------------------
/* asm: RANDI	.word	5A5A5A5Ah */
int RANDI = (int)(0x5A5A5A5A);
// *----------------------------------------------------------------------------
// *ROM CHIP DEFINES
// *	.word	START_ADDR,LENGTH,MASK,SHIFT,CHKSUM_PTR
// ;EPROM
/* asm: RTU26	.word	0E00000h,080000h,0000000FFh,0,CHKSUME00 */
int RTU26[] = { 0x0E00000, 0x080000, 0x0000000FF, 0, CHKSUME00 };
/* asm: RTU22	.word	0D80000h,080000h,0000000FFh,0,CHKSUMD80 */
int RTU22[] = { 0x0D80000, 0x080000, 0x0000000FF, 0, CHKSUMD80 };
/* asm: RTU18	.word	0D00000h,080000h,0000000FFh,0,CHKSUMD00 */
int RTU18[] = { 0x0D00000, 0x080000, 0x0000000FF, 0, CHKSUMD00 };
/* asm: RTU14	.word	0C80000h,080000h,0000000FFh,0,CHKSUMC80 */
int RTU14[] = { 0x0C80000, 0x080000, 0x0000000FF, 0, CHKSUMC80 };
/* asm: RTU10	.word	0C00000h,080000h,0000000FFh,0,CHKSUMC00 */
int RTU10[] = { 0x0C00000, 0x080000, 0x0000000FF, 0, CHKSUMC00 };
/* asm: RTU27	.word	0E00000h,080000h,00000FF00h,-8,CHKSUME01 */
int RTU27[] = { 0x0E00000, 0x080000, 0x00000FF00, -8, CHKSUME01 };
/* asm: RTU23	.word	0D80000h,080000h,00000FF00h,-8,CHKSUMD81 */
int RTU23[] = { 0x0D80000, 0x080000, 0x00000FF00, -8, CHKSUMD81 };
/* asm: RTU19	.word	0D00000h,080000h,00000FF00h,-8,CHKSUMD01 */
int RTU19[] = { 0x0D00000, 0x080000, 0x00000FF00, -8, CHKSUMD01 };
/* asm: RTU15	.word	0C80000h,080000h,00000FF00h,-8,CHKSUMC81 */
int RTU15[] = { 0x0C80000, 0x080000, 0x00000FF00, -8, CHKSUMC81 };
/* asm: RTU11	.word	0C00000h,080000h,00000FF00h,-8,CHKSUMC01 */
int RTU11[] = { 0x0C00000, 0x080000, 0x00000FF00, -8, CHKSUMC01 };
/* asm: RTU28	.word	0E00000h,080000h,000FF0000h,-16,CHKSUME02 */
int RTU28[] = { 0x0E00000, 0x080000, 0x000FF0000, -16, CHKSUME02 };
/* asm: RTU24	.word	0D80000h,080000h,000FF0000h,-16,CHKSUMD82 */
int RTU24[] = { 0x0D80000, 0x080000, 0x000FF0000, -16, CHKSUMD82 };
/* asm: RTU20	.word	0D00000h,080000h,000FF0000h,-16,CHKSUMD02 */
int RTU20[] = { 0x0D00000, 0x080000, 0x000FF0000, -16, CHKSUMD02 };
/* asm: RTU16	.word	0C80000h,080000h,000FF0000h,-16,CHKSUMC82 */
int RTU16[] = { 0x0C80000, 0x080000, 0x000FF0000, -16, CHKSUMC82 };
/* asm: RTU12	.word	0C00000h,080000h,000FF0000h,-16,CHKSUMC02 */
int RTU12[] = { 0x0C00000, 0x080000, 0x000FF0000, -16, CHKSUMC02 };
/* asm: RTU29	.word	0E00000h,080000h,0FF000000h,-24,CHKSUME03 */
int RTU29[] = { 0x0E00000, 0x080000, 0x0FF000000, -24, CHKSUME03 };
/* asm: RTU25	.word	0D80000h,080000h,0FF000000h,-24,CHKSUMD83 */
int RTU25[] = { 0x0D80000, 0x080000, 0x0FF000000, -24, CHKSUMD83 };
/* asm: RTU21	.word	0D00000h,080000h,0FF000000h,-24,CHKSUMD03 */
int RTU21[] = { 0x0D00000, 0x080000, 0x0FF000000, -24, CHKSUMD03 };
/* asm: RTU17	.word	0C80000h,080000h,0FF000000h,-24,CHKSUMC83 */
int RTU17[] = { 0x0C80000, 0x080000, 0x0FF000000, -24, CHKSUMC83 };
/* asm: RTU13	.word	0C00000h,080000h,0FF000000h,-24,CHKSUMC03 */
int RTU13[] = { 0x0C00000, 0x080000, 0x0FF000000, -24, CHKSUMC03 };
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *RAM CHIP DEFINES
// *	.word	START_ADDR,LENGTH,MASK,REPETITIONS,WAIT_STATE_MODE,PRINTABLE?,UNUMBER
// ;WAVE RAM
/* asm: RTU72	.word	0A00000h,080000h,00000000Fh,1,1000h,2 */
int RTU72[] = { 0x0A00000, 0x080000, 0x00000000F, 1, 0x1000, 2 };
/* asm: RTU69	.word	0A00000h,080000h,0000000F0h,1,1000h,2 */
int RTU69[] = { 0x0A00000, 0x080000, 0x0000000F0, 1, 0x1000, 2 };
/* asm: RTU70	.word	0A00000h,080000h,000000F00h,1,1000h,2 */
int RTU70[] = { 0x0A00000, 0x080000, 0x000000F00, 1, 0x1000, 2 };
/* asm: RTU71	.word	0A00000h,080000h,00000F000h,1,1000h,2 */
int RTU71[] = { 0x0A00000, 0x080000, 0x00000F000, 1, 0x1000, 2 };
/* asm: RTU76	.word	0A00001h,080000h,00000000Fh,1,1000h,2 */
int RTU76[] = { 0x0A00001, 0x080000, 0x00000000F, 1, 0x1000, 2 };
/* asm: RTU73	.word	0A00001h,080000h,0000000F0h,1,1000h,2 */
int RTU73[] = { 0x0A00001, 0x080000, 0x0000000F0, 1, 0x1000, 2 };
/* asm: RTU74	.word	0A00001h,080000h,000000F00h,1,1000h,2 */
int RTU74[] = { 0x0A00001, 0x080000, 0x000000F00, 1, 0x1000, 2 };
/* asm: RTU75	.word	0A00001h,080000h,00000F000h,1,1000h,2 */
int RTU75[] = { 0x0A00001, 0x080000, 0x00000F000, 1, 0x1000, 2 };
// ;COLOR RAM
/* asm: RTU85	.word	09E0000h,08000h,000FF00h,1,SOFT_WS,1 */
int RTU85[] = { 0x09E0000, 0x08000, 0x000FF00, 1, SOFT_WS, 1 };
/* asm: RTU87	.word	09E0000h,08000h,00000FFh,1,SOFT_WS,1 */
int RTU87[] = { 0x09E0000, 0x08000, 0x00000FF, 1, SOFT_WS, 1 };
// ;VIDEO RAM
/* asm: RTU102	.word	0900000h,020000h,0000000FFh,1,1000h,2 */
int RTU102[] = { 0x0900000, 0x020000, 0x0000000FF, 1, 0x1000, 2 };
/* asm: RTU95	.word	0900000h,020000h,00000FF00h,1,1000h,2 */
int RTU95[] = { 0x0900000, 0x020000, 0x00000FF00, 1, 0x1000, 2 };
/* asm: RTU94	.word	0900001h,020000h,0000000FFh,1,1000h,2 */
int RTU94[] = { 0x0900001, 0x020000, 0x0000000FF, 1, 0x1000, 2 };
/* asm: RTU101	.word	0900001h,020000h,00000FF00h,1,1000h,2 */
int RTU101[] = { 0x0900001, 0x020000, 0x00000FF00, 1, 0x1000, 2 };
// ;FAST RAM
/* asm: RTU57	.word	0400000h,020000h,00000FF00h,1,SOFT_WS,1 */
int RTU57[] = { 0x0400000, 0x020000, 0x00000FF00, 1, SOFT_WS, 1 };
/* asm: RTU56	.word	0400000h,020000h,0000000FFh,1,SOFT_WS,1 */
int RTU56[] = { 0x0400000, 0x020000, 0x0000000FF, 1, SOFT_WS, 1 };
/* asm: RTU60	.word	0400000h,020000h,0FF000000h,1,SOFT_WS,1 */
int RTU60[] = { 0x0400000, 0x020000, 0x0FF000000, 1, SOFT_WS, 1 };
/* asm: RTU59	.word	0400000h,020000h,000FF0000h,1,SOFT_WS,1 */
int RTU59[] = { 0x0400000, 0x020000, 0x000FF0000, 1, SOFT_WS, 1 };
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *VECTOR PTR,XmYm,XM,YM,U_NUM
// *
const char tt_[] = ":UTP:";
const char tt_[] = ":UTP:";
#define ISRAM 0
#define ISROM 1
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// ;color ram
// ;bitmap
// *----------------------------------------------------------------------------
// *STRUCT	CHIP_TEST_TABLE
#define CTT_TESTTAB 0
#define CTT_MINX 1
#define CTT_MINY 2
#define CTT_MAXX 3
#define CTT_MAXY 4
#define CTT_U 5
#define CTT_RAMROM 6
#define CTT_SIZE 7
// *ENDSTRUCT
// *----------------------------------------------------------------------------
// *TEST_CHIPS	Controller Routine
// *
// *
// *	1.	THE STACK IS ONCHIP, THEREFORE WE CAN ASSUME
// *		THAT IT WORKS, THEREFORE WE MAY USE CALLs
// *		TO SUBROUTINES.
// *	2.	DESIGNED TO BE RUN OUT OF ROM
// *	3.	INTERRUPTS SHOULD BE TURNED OFF
// *	4.	AFTERWARDS RAM SHOULD BE CLEARED
// *	5.	ON CHIP RAM IS NOT TESTED (OBVIOUSLY)
// *	6.	ROUTINE IS CALLED (SEE #1)
// *	7.	SPACE MUST BE RESERVED FOR THE FOLLOWING TWO
// *		ROUTINES ON ONCHIP RAM:
// *			RAMTEST (this file)
// *			_pixel	(FONT1A.ASM)
// *		BECAUSE THESE ROUTINES CHANGE WAIT STATES (WHICH
// *		CAN NOT HAPPEN DURING PROGRAM FETCH FROM SLOW
// *		ROM (see #2)).
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *FOR THE DISPLAY TO WORK WE MUST HAVE OPERATING PROPERLY:
// *	VIDEO RAM (8 CHIPS)
// *	COLOR RAM (2 CHIPS)
// *SO, WE TEST THESE (NOT PLOTTING TO THE SCREEN), AND SAVE THE RESULTS,
// *THEN WE BEGIN PLOTTING THE SCREEN, INITIALIZATING THESE SPACES WITH THE
// *RESULTS.
// *
// *
/* asm: BASICS_RAM	fbss	BASICS_RAM,10 */
int BASICS_RAM[10];
const char MEMTST1[] = "CRUISN USA (TM)";
// 	;Static *MUST* be working to get this far,
// 	;so mark them ALL as cool
// 	;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *ROM CHIP CHECKSUMS
// *
#if CHECKSUM_GEN
/* asm: CHKSUMC00	.word	-1 */
int CHKSUMC00 = (int)(-1);
/* asm: CHKSUMC80	.word	-1 */
int CHKSUMC80 = (int)(-1);
/* asm: CHKSUMD00	.word	-1 */
int CHKSUMD00 = (int)(-1);
/* asm: CHKSUMD80	.word	-1 */
int CHKSUMD80 = (int)(-1);
/* asm: CHKSUME00	.word	-1 */
int CHKSUME00 = (int)(-1);
/* asm: CHKSUMC01	.word	-1 */
int CHKSUMC01 = (int)(-1);
/* asm: CHKSUMC81	.word	-1 */
int CHKSUMC81 = (int)(-1);
/* asm: CHKSUMD01	.word	-1 */
int CHKSUMD01 = (int)(-1);
/* asm: CHKSUMD81	.word	-1 */
int CHKSUMD81 = (int)(-1);
/* asm: CHKSUME01	.word	-1 */
int CHKSUME01 = (int)(-1);
/* asm: CHKSUMC02	.word	-1 */
int CHKSUMC02 = (int)(-1);
/* asm: CHKSUMC82	.word	-1 */
int CHKSUMC82 = (int)(-1);
/* asm: CHKSUMD02	.word	-1 */
int CHKSUMD02 = (int)(-1);
/* asm: CHKSUMD82	.word	-1 */
int CHKSUMD82 = (int)(-1);
/* asm: CHKSUME02	.word	-1 */
int CHKSUME02 = (int)(-1);
/* asm: CHKSUMC03	.word	-1 */
int CHKSUMC03 = (int)(-1);
/* asm: CHKSUMC83	.word	-1 */
int CHKSUMC83 = (int)(-1);
/* asm: CHKSUMD03	.word	-1 */
int CHKSUMD03 = (int)(-1);
/* asm: CHKSUMD83	.word	-1 */
int CHKSUMD83 = (int)(-1);
/* asm: CHKSUME03	.word	-1 */
int CHKSUME03 = (int)(-1);
/* asm: CCHKSUMC00	.word	0 */
int CCHKSUMC00 = (int)(0);
/* asm: CCHKSUMC80	.word	0 */
int CCHKSUMC80 = (int)(0);
/* asm: CCHKSUMD00	.word	0 */
int CCHKSUMD00 = (int)(0);
/* asm: CCHKSUMD80	.word	0 */
int CCHKSUMD80 = (int)(0);
/* asm: CCHKSUME00	.word	0 */
int CCHKSUME00 = (int)(0);
/* asm: CCHKSUMC01	.word	0 */
int CCHKSUMC01 = (int)(0);
/* asm: CCHKSUMC81	.word	0 */
int CCHKSUMC81 = (int)(0);
/* asm: CCHKSUMD01	.word	0 */
int CCHKSUMD01 = (int)(0);
/* asm: CCHKSUMD81	.word	0 */
int CCHKSUMD81 = (int)(0);
/* asm: CCHKSUME01	.word	0 */
int CCHKSUME01 = (int)(0);
/* asm: CCHKSUMC02	.word	0 */
int CCHKSUMC02 = (int)(0);
/* asm: CCHKSUMC82	.word	0 */
int CCHKSUMC82 = (int)(0);
/* asm: CCHKSUMD02	.word	0 */
int CCHKSUMD02 = (int)(0);
/* asm: CCHKSUMD82	.word	0 */
int CCHKSUMD82 = (int)(0);
/* asm: CCHKSUME02	.word	0 */
int CCHKSUME02 = (int)(0);
/* asm: CCHKSUMC03	.word	0 */
int CCHKSUMC03 = (int)(0);
/* asm: CCHKSUMC83	.word	0 */
int CCHKSUMC83 = (int)(0);
/* asm: CCHKSUMD03	.word	0 */
int CCHKSUMD03 = (int)(0);
/* asm: CCHKSUMD83	.word	0 */
int CCHKSUMD83 = (int)(0);
/* asm: CCHKSUME03	.word	0 */
int CCHKSUME03 = (int)(0);
#else
/* asm: CHKSUMC00	.word	CHECKSUM_C00 */
int CHKSUMC00 = (int)(CHECKSUM_C00);
/* asm: CHKSUMC80	.word	CHECKSUM_C80 */
int CHKSUMC80 = (int)(CHECKSUM_C80);
/* asm: CHKSUMD00	.word	CHECKSUM_D00 */
int CHKSUMD00 = (int)(CHECKSUM_D00);
/* asm: CHKSUMD80	.word	CHECKSUM_D80 */
int CHKSUMD80 = (int)(CHECKSUM_D80);
/* asm: CHKSUME00	.word	CHECKSUM_E00 */
int CHKSUME00 = (int)(CHECKSUM_E00);
/* asm: CHKSUMC01	.word	CHECKSUM_C01 */
int CHKSUMC01 = (int)(CHECKSUM_C01);
/* asm: CHKSUMC81	.word	CHECKSUM_C81 */
int CHKSUMC81 = (int)(CHECKSUM_C81);
/* asm: CHKSUMD01	.word	CHECKSUM_D01 */
int CHKSUMD01 = (int)(CHECKSUM_D01);
/* asm: CHKSUMD81	.word	CHECKSUM_D81 */
int CHKSUMD81 = (int)(CHECKSUM_D81);
/* asm: CHKSUME01	.word	CHECKSUM_E01 */
int CHKSUME01 = (int)(CHECKSUM_E01);
/* asm: CHKSUMC02	.word	CHECKSUM_C02 */
int CHKSUMC02 = (int)(CHECKSUM_C02);
/* asm: CHKSUMC82	.word	CHECKSUM_C82 */
int CHKSUMC82 = (int)(CHECKSUM_C82);
/* asm: CHKSUMD02	.word	CHECKSUM_D02 */
int CHKSUMD02 = (int)(CHECKSUM_D02);
/* asm: CHKSUMD82	.word	CHECKSUM_D82 */
int CHKSUMD82 = (int)(CHECKSUM_D82);
/* asm: CHKSUME02	.word	CHECKSUM_E02 */
int CHKSUME02 = (int)(CHECKSUM_E02);
/* asm: CHKSUMC03	.word	CHECKSUM_C03 */
int CHKSUMC03 = (int)(CHECKSUM_C03);
/* asm: CHKSUMC83	.word	CHECKSUM_C83 */
int CHKSUMC83 = (int)(CHECKSUM_C83);
/* asm: CHKSUMD03	.word	CHECKSUM_D03 */
int CHKSUMD03 = (int)(CHECKSUM_D03);
/* asm: CHKSUMD83	.word	CHECKSUM_D83 */
int CHKSUMD83 = (int)(CHECKSUM_D83);
/* asm: CHKSUME03	.word	CHECKSUM_E03 */
int CHKSUME03 = (int)(CHECKSUM_E03);
/* asm: CCHKSUMC00	.word	~CHECKSUM_C00 */
int CCHKSUMC00 = (int)(~CHECKSUM_C00);
/* asm: CCHKSUMC80	.word	~CHECKSUM_C80 */
int CCHKSUMC80 = (int)(~CHECKSUM_C80);
/* asm: CCHKSUMD00	.word	~CHECKSUM_D00 */
int CCHKSUMD00 = (int)(~CHECKSUM_D00);
/* asm: CCHKSUMD80	.word	~CHECKSUM_D80 */
int CCHKSUMD80 = (int)(~CHECKSUM_D80);
/* asm: CCHKSUME00	.word	~CHECKSUM_E00 */
int CCHKSUME00 = (int)(~CHECKSUM_E00);
/* asm: CCHKSUMC01	.word	~CHECKSUM_C01 */
int CCHKSUMC01 = (int)(~CHECKSUM_C01);
/* asm: CCHKSUMC81	.word	~CHECKSUM_C81 */
int CCHKSUMC81 = (int)(~CHECKSUM_C81);
/* asm: CCHKSUMD01	.word	~CHECKSUM_D01 */
int CCHKSUMD01 = (int)(~CHECKSUM_D01);
/* asm: CCHKSUMD81	.word	~CHECKSUM_D81 */
int CCHKSUMD81 = (int)(~CHECKSUM_D81);
/* asm: CCHKSUME01	.word	~CHECKSUM_E01 */
int CCHKSUME01 = (int)(~CHECKSUM_E01);
/* asm: CCHKSUMC02	.word	~CHECKSUM_C02 */
int CCHKSUMC02 = (int)(~CHECKSUM_C02);
/* asm: CCHKSUMC82	.word	~CHECKSUM_C82 */
int CCHKSUMC82 = (int)(~CHECKSUM_C82);
/* asm: CCHKSUMD02	.word	~CHECKSUM_D02 */
int CCHKSUMD02 = (int)(~CHECKSUM_D02);
/* asm: CCHKSUMD82	.word	~CHECKSUM_D82 */
int CCHKSUMD82 = (int)(~CHECKSUM_D82);
/* asm: CCHKSUME02	.word	~CHECKSUM_E02 */
int CCHKSUME02 = (int)(~CHECKSUM_E02);
/* asm: CCHKSUMC03	.word	~CHECKSUM_C03 */
int CCHKSUMC03 = (int)(~CHECKSUM_C03);
/* asm: CCHKSUMC83	.word	~CHECKSUM_C83 */
int CCHKSUMC83 = (int)(~CHECKSUM_C83);
/* asm: CCHKSUMD03	.word	~CHECKSUM_D03 */
int CCHKSUMD03 = (int)(~CHECKSUM_D03);
/* asm: CCHKSUMD83	.word	~CHECKSUM_D83 */
int CCHKSUMD83 = (int)(~CHECKSUM_D83);
/* asm: CCHKSUME03	.word	~CHECKSUM_E03 */
int CCHKSUME03 = (int)(~CHECKSUM_E03);
#endif
// *----------------------------------------------------------------------------

void _word(void)
{
    // asm: CTTP:,:MNX:,:MNY:,:MXX:,:MXY:,tt?,:RAMROM:
    TRACE_EVENT(&g_crusn_machine->trace, "function", ".word", 0, 0);
    UNIMPL();
}

void TEST_STATIC_CHIPS(void)
{
    // asm: 	DINT
    // asm: 	SETDP
    // asm: 	CALL	COPY_RAMTEST
    // asm: 	LDL	BASICS_RAM,AR5
    // asm: 	LDI	3,AR6
    // asm: 	LDL	STATIC_TABLE,AR4
    // asm: TSTBL1A
    // asm: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm: 	LDI	*AR0++,AR2
    // asm: 	LDI	*AR0++,R2
    // asm: 	LDI	*AR0++,R3
    // asm: 	LDI	*AR0++,RC
    // asm: 	LDI	*AR0++,RS
    // asm: 	LDI	*AR0++,BK
    // asm: 	CALL	FEED_WATCHDOG_HARD
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR4
    // asm: 	CALL	RAMTEST
    // asm: 	POP	AR4
    // asm: 	POP	AR5
    // asm: 	POP	AR6
    // asm: 	POP	AR0
    // asm: 	STI	R0,*AR5++		;SAVE RESULTS
    // asm: 	CMPI	1,R0
    // asm: 	BEQ	IS_STATIC_ERROR
    // asm: 	LDI	*++AR4(CTT_SIZE),R0
    // asm: 	DBU	AR6,TSTBL1A
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_STATIC_CHIPS", 0, 0);
    UNIMPL();
}

void GET_AR4_DIGITS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*+AR4(CTT_U),AR0
    // asm: 	LDI	*AR0,AR0
    // asm: 	LDI	AR0,R1
    // asm: 	RS	8,R1
    // asm: 	AND	0FFh,R1
    // asm: 	SUBI	'0',R1
    // asm: 	LDI	AR0,R2
    // asm: 	RS	16,R2
    // asm: 	AND	0FFh,R2
    // asm: 	CMPI	0,R2
    // asm: 	BEQ	R1R2ZER
    // asm: 	SUBI	'0',R2
    // asm: 	LDI	AR0,R3
    // asm: 	RS	24,R3
    // asm: 	AND	0FFh,R3
    // asm: 	CMPI	0,R3
    // asm: 	BEQ	R3ZERO
    // asm: 	SUBI	'0',R3
    // asm: 	BU	NZERO
R1R2ZER:
    // asm: CLRI	R2
R3ZERO:
    // asm: CLRI	R3
NZERO:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_AR4_DIGITS", 0, 0);
    UNIMPL();
}

void BLINK_AND_CONTINUE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	RS
    // asm: 	PUSH	RE
    // asm: 	PUSH	RC
    // asm: 	PUSH	AR0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	CALL	GET_AR4_DIGITS
    // asm: 	CALL	BLINK_DIGITS
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	AR0
    // asm: 	POP	RC
    // asm: 	POP	RE
    // asm: 	POP	RS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLINK_AND_CONTINUE", 0, 0);
    UNIMPL();
}

void IS_STATIC_ERROR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	GET_AR4_DIGITS
ELOOP:
    // asm: 	CALL	BLINK_DIGITS
    // asm: 	BU	ELOOP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "IS_STATIC_ERROR", 0, 0);
    UNIMPL();
}

void LLED_ON(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *THIS ROUTINE TAKES OVER THE CPU, AND RETURNS
    // *
    // *PARAMETERS
    // *	R1 DIGIT 1
    // *	R2 DIGIT 2
    // *	R3 DIGIT 3
    // *
    // *
    // asm: 	LDI	@SYSCNTL,R0		;if the system hangs and the LED
    // asm: 	ANDN	LED_OFF,R0		;is on we were in this routine
    // asm: 	STI	R0,@SYSCNTL		;when it happened
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LLED_ON", 0, 0);
    UNIMPL();
}

void LLED_OFF(void)
{
    // asm: 	LDI	@SYSCNTL,R0
    // asm: 	OR	LED_OFF,R0
    // asm: 	STI	R0,@SYSCNTL
    // asm: 	LDP	@SYSCNTLR
    // asm: 	STI	R0,@SYSCNTLR
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LLED_OFF", 0, 0);
    UNIMPL();
}

void BLINK_DIGITS(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR4
    // asm: 	LDI	R1,AR4		;#NUMBER OF ITERATIONS
    // asm: 	DEC	AR4
    // asm: 	CMPI	0,AR4
    // asm: 	BLT	BDL1X
    // asm: BDL1
    // asm: 	CALL	LLED_ON
    // asm: 	LDIL	8000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	CALL	LLED_OFF
    // asm: 	LDIL	8000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	DBU	AR4,BDL1
BDL1X:
    // asm: 	LDIL	35000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	LDI	R2,AR4		;#NUMBER OF ITERATIONS
    // asm: 	DEC	AR4
    // asm: 	CMPI	0,AR4
    // asm: 	BLT	BDL2X
    // asm: BDL2
    // asm: 	CALL	LLED_ON
    // asm: 	LDIL	8000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	CALL	LLED_OFF
    // asm: 	LDIL	8000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	DBU	AR4,BDL2
BDL2X:
    // asm: 	LDIL	35000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	LDI	R3,AR4		;#NUMBER OF ITERATIONS
    // asm: 	DEC	AR4
    // asm: 	CMPI	0,AR4
    // asm: 	BLT	BDL3X
    // asm: BDL3	CALL	LLED_ON
    // asm: 	LDIL	8000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	CALL	LLED_OFF
    // asm: 	LDIL	8000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	DBU	AR4,BDL3
BDL3X:
    // asm: 	LDIL	35000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	LDIL	35000000,RC
    // asm: 	RPTS	RC
    // asm: 	NOP
    // asm: 	POP	AR4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLINK_DIGITS", 0, 0);
    UNIMPL();
}

void TEST_CHIPS(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	DP
    // asm: 	LDP	@9E0000h
    // asm: 	CLRI	R0			;set background to 0
    // asm: 	STI	R0,@9E0000h
    // asm: 	POP	DP
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	DINT
    // asm: 	CALL	COPY_RAMTEST
    // asm: 	CALL	TEST_BASICS
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"CPU BOARD TEST",50,40
    // 		;
    // 		;NOW CHECK AND PLOT ACCORDINGLY
    // 		;
    // asm: 	LDL	TESTING_CHIPS,AR4
TESTLP1:
    // asm: 	LDI	*+AR4(CTT_RAMROM),R0
    // asm: 	BZ	CHECK_AS_RAM	;(W/ SCRAMBLE)
    // asm: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm: 	LDI	*AR0++,AR2	;ADDR
    // asm: 	LDI	*AR0++,RC	;LENGTH
    // asm: 	LDI	*AR0++,R2	;MASK
    // asm: 	LDI	*AR0++,R3	;SHIFT
    // asm: 	CALL	GENERATE_CHECKSUM
    // asm: 	LDI	*AR0,AR0
    // asm: 	LDI	*AR0,R1
    // asm: 	CMPI	R1,R0
    // asm: 	LDIEQ	COL_GREEN,RS
    // asm: 	LDINE	COL_RED,RS
    // ;	CMPI	COL_RED,RS
    // ;	CALLEQ	BLINK_AND_CONTINUE
    // 	;CHECKSUM PART
    // asm: 	BU	DN_CHK
CHECK_AS_RAM:
    // asm: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm: 	CMPI	-1,AR0
    // asm: 	BEQ	TEST_CHIPSX
    // asm: 	LDI	*AR0++,AR2
    // asm: 	LDI	*AR0++,R2
    // asm: 	LDI	*AR0++,R3
    // asm: 	LDI	*AR0++,RC
    // asm: 	LDI	*AR0++,RS
    // asm: 	LDI	*AR0++,BK
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR4
    // asm: 	CALL	RAMTEST
    // asm: 	POP	AR4
    // asm: 	POP	AR0
    // asm: 	LDI	R0,R0
    // asm: 	LDIZ	COL_GREEN,RS
    // ;	LDIZ	COL_GREY,RS
    // asm: 	LDINZ	COL_RED,RS
    // ;	CMPI	COL_RED,RS
    // ;	CALLEQ	BLINK_AND_CONTINUE
DN_CHK:
    // asm: 	LDI	*+AR4(CTT_MINX),AR2
    // asm: 	INC	AR2
    // asm: 	LDI	*+AR4(CTT_MINY),R2
    // asm: 	INC	R2
    // asm: 	LDI	*+AR4(CTT_MAXX),R3
    // asm: 	DEC	R3
    // asm: 	LDI	*+AR4(CTT_MAXY),RC
    // asm: 	DEC	RC
    // asm: 	CALL	_fill
    // asm: 	LDI	*+AR4(CTT_U),AR2
    // asm: 	LDI	*+AR4(CTT_MAXX),R2
    // asm: 	LDI	*+AR4(CTT_MAXY),R3
    // asm: 	SUBI	*+AR4(CTT_MINX),R2
    // asm: 	SUBI	*+AR4(CTT_MINY),R3
    // asm: 	RS	1,R2
    // asm: 	RS	1,R3
    // asm: 	ADDI	*+AR4(CTT_MINX),R2
    // asm: 	ADDI	*+AR4(CTT_MINY),R3
    // asm: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*++AR4(CTT_SIZE),R0
    // asm: 	BNZ	TESTLP1
TEST_CHIPSX:
    // asm: 	CALL	CMOS_CHIP_TEST
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_CHIPS", 0, 0);
    UNIMPL();
}

void PLOT_OUTLINE_OF_CHIPS(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@CHIPTEST_TABLEI,AR4
POOCL:
    // asm: 	LDI	*+AR4(CTT_MINX),AR2
    // asm: 	LDI	*+AR4(CTT_MINY),R2
    // asm: 	LDI	*+AR4(CTT_MAXX),R3
    // asm: 	LDI	*+AR4(CTT_MAXY),RC
    // asm: 	LDI	COL_WHITE,RS
    // asm: 	PUSH	DP
    // asm: 	CALL	_rectangle
    // asm: 	POP	DP
    // asm: 	LDI	*+AR4(CTT_U),AR2
    // asm: 	LDI	*+AR4(CTT_MAXX),R2
    // asm: 	LDI	*+AR4(CTT_MAXY),R3
    // asm: 	SUBI	*+AR4(CTT_MINX),R2
    // asm: 	SUBI	*+AR4(CTT_MINY),R3
    // asm: 	RS	1,R2
    // asm: 	RS	1,R3
    // asm: 	ADDI	*+AR4(CTT_MINX),R2
    // asm: 	ADDI	*+AR4(CTT_MINY),R3
    // asm: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm: 	LDI	COL_DGREY,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	*++AR4(CTT_SIZE),R0
    // asm: 	BNZ	POOCL
    // asm: 	CALL	CMOS_CHIP_DISPLAY
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLOT_OUTLINE_OF_CHIPS", 0, 0);
    UNIMPL();
}

void CMOS_CHIP_DISPLAY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	CMOS_CHIP,AR4
    // asm: 	LDI	*+AR4(CTT_MINX),AR2
    // asm: 	LDI	*+AR4(CTT_MINY),R2
    // asm: 	LDI	*+AR4(CTT_MAXX),R3
    // asm: 	LDI	*+AR4(CTT_MAXY),RC
    // asm: 	LDI	COL_WHITE,RS
    // asm: 	PUSH	DP
    // asm: 	CALL	_rectangle
    // asm: 	POP	DP
    // asm: 	LDI	*+AR4(CTT_U),AR2
    // asm: 	LDI	*+AR4(CTT_MAXX),R2
    // asm: 	LDI	*+AR4(CTT_MAXY),R3
    // asm: 	SUBI	*+AR4(CTT_MINX),R2
    // asm: 	SUBI	*+AR4(CTT_MINY),R3
    // asm: 	RS	1,R2
    // asm: 	RS	1,R3
    // asm: 	ADDI	*+AR4(CTT_MINX),R2
    // asm: 	ADDI	*+AR4(CTT_MINY),R3
    // asm: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm: 	LDI	COL_DGREY,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_CHIP_DISPLAY", 0, 0);
    UNIMPL();
}

void CMOS_CHIP_TEST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	AR4
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	LDL	CMOS_CHIP,AR4
    // asm: 	LDIL	5A5A5A5Ah,R3
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	88h,R1
    // asm: 	STI	R1,@CPU_WS
    // asm: 	SETDP
    // asm: 	CMOS_WP_OFF
    // asm: 	NOP			;DELAY FOR TIMING...
    // asm: 	LDI	5,AR5
KKLL1:
    // asm: LDI	R3,R2
    // asm: 	AND	0FFh,R2
    // asm: 	LS	24,R2
    // asm: 	LDI	AUD_CHIPTEST_DEDICATED,AR2
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	STI	R2,*AR2
    // asm: 	RS	24,R2
    // asm: 	LDL	0C00000h,AR0		;DUMMY READ
    // asm: 	LDI	*AR0,R0
    // asm: 	LDI	*AR2,R0
    // asm: 	RS	24,R0
    // asm: 	CMPI	R2,R0
    // asm: 	BNE	CM_ISERROR
    // asm: 	LDI	R3,R1
    // asm: 	LSH	1,R3
    // asm: 	XOR	R3,R1
    // asm: 	BNN	RND2
    // asm: 	OR	1,R3
RND2:
    // asm: MPYI	794Fh,R3
    // asm: 	DBU	AR5,KKLL1
    // asm: 	CMOS_WP_ON
    // 	;NO ERROR
    // asm: 	LDI	COL_GREEN,RS
    // asm: 	BU	KKJJ
CM_ISERROR:
    // asm: 	CMOS_WP_ON
    // asm: 	LDI	COL_RED,RS
KKJJ:
    // ;	CMPI	COL_RED,RS
    // ;	CALLEQ	BLINK_AND_CONTINUE
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	1048h,R0
    // asm: 	STI	R0,@CPU_WS
    // asm: 	SETDP
    // asm: 	LDI	*+AR4(CTT_MINX),AR2
    // asm: 	INC	AR2
    // asm: 	LDI	*+AR4(CTT_MINY),R2
    // asm: 	INC	R2
    // asm: 	LDI	*+AR4(CTT_MAXX),R3
    // asm: 	DEC	R3
    // asm: 	LDI	*+AR4(CTT_MAXY),RC
    // asm: 	DEC	RC
    // asm: 	CALL	_fill
    // asm: 	LDI	*+AR4(CTT_U),AR2
    // asm: 	LDI	*+AR4(CTT_MAXX),R2
    // asm: 	LDI	*+AR4(CTT_MAXY),R3
    // asm: 	SUBI	*+AR4(CTT_MINX),R2
    // asm: 	SUBI	*+AR4(CTT_MINY),R3
    // asm: 	RS	1,R2
    // asm: 	RS	1,R3
    // asm: 	ADDI	*+AR4(CTT_MINX),R2
    // asm: 	ADDI	*+AR4(CTT_MINY),R3
    // asm: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm: 	LDI	COL_BLACK,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_CHIP_TEST", 0, 0);
    UNIMPL();
}

void TEST_BASICS(void)
{
    // asm: 	LDL	BASICS_RAM,AR5
    // asm: 	LDI	5,AR6
    // asm: 	LDL	CHIPTEST_TABLE,AR4
    // asm: TSTBL1
    // asm: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm: 	LDI	*AR0++,AR2
    // asm: 	LDI	*AR0++,R2
    // asm: 	LDI	*AR0++,R3
    // asm: 	LDI	*AR0++,RC
    // asm: 	LDI	*AR0++,RS
    // asm: 	LDI	*AR0++,BK
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR4
    // asm: 	CALL	RAMTEST
    // asm: 	POP	AR4
    // asm: 	POP	AR5
    // asm: 	POP	AR6
    // asm: 	POP	AR0
    // asm: 	STI	R0,*AR5++		;SAVE RESULTS
    // asm: 	CMPI	0,R0
    // asm: 	CALLNZ	BLINK_AND_CONTINUE
    // asm: 	LDI	*++AR4(CTT_SIZE),R0
    // asm: 	DBU	AR6,TSTBL1
    // asm: 	CALL	HARD_LOAD_DIAGPAL
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@COLORAM
    // asm: 	CALL	CLRSCRN			;we are NOT page flipping
    // asm: 	PUSH	DP
    // asm: 	LINE	20,50,490,50,COL_WHITE
    // asm: 	LINE	20,50,20,360,COL_WHITE
    // asm: 	LINE	490,50,490,360,COL_WHITE
    // asm: 	LINE	20,360,80,360,COL_WHITE
    // asm: 	LINE	80,360,80,340,COL_WHITE
    // asm: 	LINE	80,340,100,340,COL_WHITE
    // asm: 	LINE	100,340,100,360,COL_WHITE
    // asm: 	LINE	100,360,200,360,COL_WHITE
    // asm: 	LINE	200,360,200,340,COL_WHITE
    // asm: 	LINE	200,340,220,340,COL_WHITE
    // asm: 	LINE	220,340,220,360,COL_WHITE
    // asm: 	LINE	220,360,490,360,COL_WHITE
    // asm: 	POP	DP
    // asm: 	CALL	PLOT_OUTLINE_OF_CHIPS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_BASICS", 0, 0);
    UNIMPL();
}

void RAMTEST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *TEST RAM AREA
    // *CALLING PARAMETERS
    // *
    // *PARAMETERS
    // *	AR2	START ADDR
    // *	R2	LENGTH WORDS
    // *	R3	MASK (FOR BYTE WIDE CHIPS)
    // *	RC	# OF PASSES
    // *	RS	WAIT STATE CODE
    // *	BK	INCREMENT (USUALLY 1)
    // *
    // *RETURNS
    // *	R0	1 ON ERROR
    // *	R0	0 ON NO ERROR
    // *
    // *CLOBBERS
    // *	DP,RS,RE,RC
    // *	R0,R1,R2,R3,R4,R5
    // *	AR0,AR1,AR2,AR3
    // *USES
    // *	AR6	WATCHDOG FEEDER
    // *
    // *
    // asm: 	LDL	BLOWLIST,R0
    // asm: 	BU	R0
    // 		;THIS IS THE ACTUAL ROUTINE, BUT IT MUST
    // 		;BE PLACED IN ON-CHIP MEMORY
    // 		;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMTEST", 0, 0);
    UNIMPL();
}

void RAMTEST_REAL(void)
{
    // asm: 	PUSH	DP
    // asm: 	PUSH	R6
    // asm: 	PUSH	AR6
    // asm: 	LCALL	FEED_WATCHDOG_HARD
    // asm: 	LDP	@CPU_WS		;set hardware wait states
    // asm: 	LDI	@CPU_WS,R0
    // asm: 	PUSH	R0
    // asm: 	STI	RS,@CPU_WS
    // asm: 	LDI	0,AR3		;DUMMY READ ADDRESS
    // asm: 	LDI	*AR3,R5		;DUMMY READ
    // asm: 	LDP	@RANDI
    // asm: 	LDI	@RANDI,R4	;SETUP R4=RANDOM # REGISTER
    // asm: 	LDI	RC,AR0		;NUMBER OF PASSES
    // asm: 	DEC	AR0
    // asm: 	DEC	R2		;LENGTH OF CHECK
    // asm: 	LDI	R2,AR4
    // asm: RAMTST0
    // asm: 	LDI	AR2,AR1		;GET ADDRESS
    // asm: 	LDI	R4,R0		;GET RANDOM # STARTING POINT
    // *WRITE RANDOM NUMBERS TO RAM
    // asm: RAMTST1
    // asm: 	LDI	R0,R1
    // asm: 	LSH	1,R0
    // asm: 	LDI	0,R6
    // asm: 	XOR	R0,R1
    // asm: 	LDIN	2,R6		;	BNN	RAMTST2
    // asm: 	OR	R6,R0		;	OR	2,R0
    // 				;RAMTST2
    // asm: 	DBUD	AR4,RAMTST1
    // asm: 	STI	R0,*AR1
    // asm: 	ADDI	BK,AR1
LKKK:
    // asm: LDI	*AR3,R5
    // *READ RANDOM NUMBERS FROM RAM
    // asm: 	LDI	AR2,AR1		;GET ADDRESS
    // asm: 	LDI	R4,R0		;GET RANDOM # STARTING POINT
    // asm: 	LDI	R0,R1
    // asm: 	LSH	1,R0
    // asm: 	CLRI	R6
    // asm: 	CLRI	R7
    // asm: 	LDI	R2,AR4		;LENGTH
    // asm: 	XOR	R0,R1
    // asm: 	LDIN	2,R6		;	BNN	RAMTST4
    // asm: RAMTST3
    // asm: 	OR	R6,R0		;	OR	2,R0
    // 				;RAMTST4
    // asm: 	INC	R7
    // asm: 	LDI	*AR1,R1
    // asm: 	ADDI	BK,AR1
    // asm: 	LDI	*AR3,R5		;DUMMY READ
    // asm: 	AND	R3,R1
    // asm: 	LDI	R0,R4
    // asm: 	AND	R3,R0
    // asm: 	CMPI	R1,R0
    // asm: 	BNE	RAMERR
    // asm: 	LDI	R4,R0
    // asm: 	LDI	R0,R1		;mirrored from above
    // asm: 	LSH	1,R0
    // 	;---->BNE	RAMERR
    // asm: 	DBUD	AR4,RAMTST3
JJJH:
    // asm: CLRI	R6
    // asm: 	XOR	R0,R1
    // asm: 	LDIN	2,R6		;	BNN	RAMTST4
    // 	;---->	DBUD	AR4,RAMTST3
    // asm: RAMRPT
    // asm: 	DBU	AR0,RAMTST0
    // asm: 	CLRI	R0		;IS GOOD RAM
ENDING:
    // asm: 	POP	R1		;FIND WAIT STATES
    // asm: 	LDP	@CPU_WS		;set original wait states
    // asm: 	STI	R1,@CPU_WS
    // asm: 	POP	AR6
    // asm: 	POP	R6
    // asm: 	POP	DP
    // asm: 	RETS
RAMERR:
    // asm: 	LDI	1,R0		;IS BAD RAM
    // asm: 	BU	ENDING
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMTEST_REAL", 0, 0);
    UNIMPL();
}

void RAMTEST_END(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void COPY_RAMTEST(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDL	RAMTEST_END,AR0
    // asm: 	LDL	RAMTEST_REAL,AR1
    // asm: 	SUBI	AR1,AR0			;GET LENGTH
    // asm: 	DEC	AR0
    // asm: 	LDL	BLOWLIST,AR2
    // asm: 	LDI	AR0,RC
    // asm: 	RPTB	BBCP
    // asm: 	LDI	*AR1++,R0
    // asm: BBCP	STI	R0,*AR2++
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COPY_RAMTEST", 0, 0);
    UNIMPL();
}

void GENERATE_CHECKSUM(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *GENERATE_CHECKSUM
    // *
    // *GENERATES A 16 BIT CHECKSUM
    // *
    // *
    // *PARAMETERS
    // *	AR2	START ADDR
    // *	RC	LENGTH WORDS
    // *	R2	MASK
    // *	R3	SHIFT
    // *
    // *RETURNS
    // *	R0	PART CHECKSUM
    // *
    // asm: 	PUSH	R1
    // asm: 	PUSH	RC
    // asm: 	PUSH	AR2
    // asm: 	LCALL	FEED_WATCHDOG
    // asm: 	CLRI	R0
    // asm: 	DEC	RC
    // asm: 	RPTB	CHKSUMG
    // asm: 	LDI	*AR2++,R1
    // asm: 	AND	R2,R1
    // asm: 	LSH	R3,R1
CHKSUMG:
    // asm: ADDI	R1,R0
    // asm: 	LS	16,R0
    // asm: 	RS	16,R0
    // asm: 	POP	AR2
    // asm: 	POP	RC
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GENERATE_CHECKSUM", 0, 0);
    UNIMPL();
}

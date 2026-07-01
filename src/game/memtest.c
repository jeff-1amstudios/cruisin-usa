
#include "memtest.h"
#include "../core/machine.h"
#include "c30.h"
#include "checksum.h"
#include "cksum.h"
#include "cmos.h"
#include "diag.h"
#include "globals.h"
#include "macs.h"
#include "sys.h"
#include "vunit.h"

/*
 * Source module: asm/MEMTEST.ASM
 */

void TEST_STATIC_CHIPS(void);
static void GET_AR4_DIGITS(void);
static void BLINK_AND_CONTINUE(void);
static void IS_STATIC_ERROR(void);
static void LLED_ON(void);
static void LLED_OFF(void);
static void BLINK_DIGITS(void);
void TEST_CHIPS(void);
static void PLOT_OUTLINE_OF_CHIPS(void);
static void CMOS_CHIP_DISPLAY(void);
static void CMOS_CHIP_TEST(void);
static void TEST_BASICS(void);
static void RAMTEST(void);
static void RAMTEST_REAL(void);
static void RAMERR(void);
static void RAMTEST_END(void);
static void COPY_RAMTEST(void);
static void GENERATE_CHECKSUM(void);

#define CHIPTEST_TABLEI CHIPTEST_TABLE
#define CHKSUMC00 CHECKSUM_C00
#define CHKSUMC80 CHECKSUM_C80
#define CHKSUMD00 CHECKSUM_D00
#define CHKSUMD80 CHECKSUM_D80
#define CHKSUME00 CHECKSUM_E00
#define CHKSUMC01 CHECKSUM_C01
#define CHKSUMC81 CHECKSUM_C81
#define CHKSUMD01 CHECKSUM_D01
#define CHKSUMD81 CHECKSUM_D81
#define CHKSUME01 CHECKSUM_E01
#define CHKSUMC02 CHECKSUM_C02
#define CHKSUMC82 CHECKSUM_C82
#define CHKSUMD02 CHECKSUM_D02
#define CHKSUMD82 CHECKSUM_D82
#define CHKSUME02 CHECKSUM_E02
#define CHKSUMC03 CHECKSUM_C03
#define CHKSUMC83 CHECKSUM_C83
#define CHKSUMD03 CHECKSUM_D03
#define CHKSUMD83 CHECKSUM_D83
#define CHKSUME03 CHECKSUM_E03

void FEED_WATCHDOG_HARD(void);

static int CHIPTEST_TABLE;

/*
 *----------------------------------------------------------------------------
 *MEMTEST.ASM
 *
 *COPYRIGHT (C) 1994  BY TV GAMES, INC.
 *ALL RIGHTS RESERVED
 *
 *CHIP TEST ROUTINES
 *
 *
 *MEMORY WAIT STATE ACCESS NOTES
 *
 *NORMAL=WAIT STATES SET TO SW=2 OR HARDWARE WHICHEVER RELEASES FIRST
 *FOR COLOR RAM SET T0 SW=1 OR HARDWARE WHICHEVER RELEASES FIRST
 *FOR BITMAP OR WAVERAM MODE HARDWARE ONLY
 *AFTER BITMAP READ, READ 0 AFTERWARD TO RESET HDWE.
 *FOR A-D SET WAIT TO SW=4 ONLY
 *
 */

// 			;CHECKSUM_GEN	.set	1

// *----------------------------------------------------------------------------
/* asm: RANDI		.word	5A5A5A5Ah */
/* asm: 	 */
/* asm: 	 */
/* asm: 	 */
static int RANDI = 0x5A5A5A5A;
// ;EPROM
/* asm: RTU26	.word	0E00000h,080000h,0000000FFh,0,CHKSUME00 */
static int RTU26[] = {
    0x0E00000,
    0x080000,
    0x0000000FF,
    0,
    CHKSUME00,
};
/* asm: RTU22	.word	0D80000h,080000h,0000000FFh,0,CHKSUMD80 */
static int RTU22[] = {
    0x0D80000,
    0x080000,
    0x0000000FF,
    0,
    CHKSUMD80,
};
/* asm: RTU18	.word	0D00000h,080000h,0000000FFh,0,CHKSUMD00 */
static int RTU18[] = {
    0x0D00000,
    0x080000,
    0x0000000FF,
    0,
    CHKSUMD00,
};
/* asm: RTU14	.word	0C80000h,080000h,0000000FFh,0,CHKSUMC80 */
static int RTU14[] = {
    0x0C80000,
    0x080000,
    0x0000000FF,
    0,
    CHKSUMC80,
};
/* asm: RTU10	.word	0C00000h,080000h,0000000FFh,0,CHKSUMC00 */
/* asm: 	 */
static int RTU10[] = {
    0x0C00000,
    0x080000,
    0x0000000FF,
    0,
    CHKSUMC00,
};
/* asm: RTU27	.word	0E00000h,080000h,00000FF00h,-8,CHKSUME01 */
static int RTU27[] = {
    0x0E00000,
    0x080000,
    0x00000FF00,
    -8,
    CHKSUME01,
};
/* asm: RTU23	.word	0D80000h,080000h,00000FF00h,-8,CHKSUMD81 */
static int RTU23[] = {
    0x0D80000,
    0x080000,
    0x00000FF00,
    -8,
    CHKSUMD81,
};
/* asm: RTU19	.word	0D00000h,080000h,00000FF00h,-8,CHKSUMD01 */
static int RTU19[] = {
    0x0D00000,
    0x080000,
    0x00000FF00,
    -8,
    CHKSUMD01,
};
/* asm: RTU15	.word	0C80000h,080000h,00000FF00h,-8,CHKSUMC81 */
static int RTU15[] = {
    0x0C80000,
    0x080000,
    0x00000FF00,
    -8,
    CHKSUMC81,
};
/* asm: RTU11	.word	0C00000h,080000h,00000FF00h,-8,CHKSUMC01 */
/* asm: 	 */
static int RTU11[] = {
    0x0C00000,
    0x080000,
    0x00000FF00,
    -8,
    CHKSUMC01,
};
/* asm: RTU28	.word	0E00000h,080000h,000FF0000h,-16,CHKSUME02 */
static int RTU28[] = {
    0x0E00000,
    0x080000,
    0x000FF0000,
    -16,
    CHKSUME02,
};
/* asm: RTU24	.word	0D80000h,080000h,000FF0000h,-16,CHKSUMD82 */
static int RTU24[] = {
    0x0D80000,
    0x080000,
    0x000FF0000,
    -16,
    CHKSUMD82,
};
/* asm: RTU20	.word	0D00000h,080000h,000FF0000h,-16,CHKSUMD02 */
static int RTU20[] = {
    0x0D00000,
    0x080000,
    0x000FF0000,
    -16,
    CHKSUMD02,
};
/* asm: RTU16	.word	0C80000h,080000h,000FF0000h,-16,CHKSUMC82 */
static int RTU16[] = {
    0x0C80000,
    0x080000,
    0x000FF0000,
    -16,
    CHKSUMC82,
};
/* asm: RTU12	.word	0C00000h,080000h,000FF0000h,-16,CHKSUMC02 */
/* asm: 	 */
static int RTU12[] = {
    0x0C00000,
    0x080000,
    0x000FF0000,
    -16,
    CHKSUMC02,
};
/* asm: RTU29	.word	0E00000h,080000h,0FF000000h,-24,CHKSUME03 */
static int RTU29[] = {
    0x0E00000,
    0x080000,
    0x0FF000000,
    -24,
    CHKSUME03,
};
/* asm: RTU25	.word	0D80000h,080000h,0FF000000h,-24,CHKSUMD83 */
static int RTU25[] = {
    0x0D80000,
    0x080000,
    0x0FF000000,
    -24,
    CHKSUMD83,
};
/* asm: RTU21	.word	0D00000h,080000h,0FF000000h,-24,CHKSUMD03 */
static int RTU21[] = {
    0x0D00000,
    0x080000,
    0x0FF000000,
    -24,
    CHKSUMD03,
};
/* asm: RTU17	.word	0C80000h,080000h,0FF000000h,-24,CHKSUMC83 */
static int RTU17[] = {
    0x0C80000,
    0x080000,
    0x0FF000000,
    -24,
    CHKSUMC83,
};
/* asm: RTU13	.word	0C00000h,080000h,0FF000000h,-24,CHKSUMC03 */
static int RTU13[] = {
    0x0C00000,
    0x080000,
    0x0FF000000,
    -24,
    CHKSUMC03,
};
// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *RAM CHIP DEFINES
 *	.word	START_ADDR,LENGTH,MASK,REPETITIONS,WAIT_STATE_MODE,PRINTABLE?,UNUMBER
 */

// ;WAVE RAM
/* asm: RTU72	.word	0A00000h,080000h,00000000Fh,1,1000h,2 */
static int RTU72[] = {
    0x0A00000,
    0x080000,
    0x00000000F,
    1,
    0x1000,
    2,
};
/* asm: RTU69	.word	0A00000h,080000h,0000000F0h,1,1000h,2 */
static int RTU69[] = {
    0x0A00000,
    0x080000,
    0x0000000F0,
    1,
    0x1000,
    2,
};
/* asm: RTU70	.word	0A00000h,080000h,000000F00h,1,1000h,2 */
static int RTU70[] = {
    0x0A00000,
    0x080000,
    0x000000F00,
    1,
    0x1000,
    2,
};
/* asm: RTU71	.word	0A00000h,080000h,00000F000h,1,1000h,2 */
/* asm: 	 */
static int RTU71[] = {
    0x0A00000,
    0x080000,
    0x00000F000,
    1,
    0x1000,
    2,
};
/* asm: RTU76	.word	0A00001h,080000h,00000000Fh,1,1000h,2 */
static int RTU76[] = {
    0x0A00001,
    0x080000,
    0x00000000F,
    1,
    0x1000,
    2,
};
/* asm: RTU73	.word	0A00001h,080000h,0000000F0h,1,1000h,2 */
static int RTU73[] = {
    0x0A00001,
    0x080000,
    0x0000000F0,
    1,
    0x1000,
    2,
};
/* asm: RTU74	.word	0A00001h,080000h,000000F00h,1,1000h,2 */
static int RTU74[] = {
    0x0A00001,
    0x080000,
    0x000000F00,
    1,
    0x1000,
    2,
};
/* asm: RTU75	.word	0A00001h,080000h,00000F000h,1,1000h,2 */
/* asm: 	 */
static int RTU75[] = {
    0x0A00001,
    0x080000,
    0x00000F000,
    1,
    0x1000,
    2,
};
// ;COLOR RAM
/* asm: RTU85	.word	09E0000h,08000h,000FF00h,1,SOFT_WS,1 */
static int RTU85[] = {
    0x09E0000,
    0x08000,
    0x000FF00,
    1,
    SOFT_WS,
    1,
};
/* asm: RTU87	.word	09E0000h,08000h,00000FFh,1,SOFT_WS,1 */
/* asm: 	 */
static int RTU87[] = {
    0x09E0000,
    0x08000,
    0x00000FF,
    1,
    SOFT_WS,
    1,
};
// ;VIDEO RAM
/* asm: RTU102	.word	0900000h,020000h,0000000FFh,1,1000h,2 */
static int RTU102[] = {
    0x0900000,
    0x020000,
    0x0000000FF,
    1,
    0x1000,
    2,
};
/* asm: RTU95	.word	0900000h,020000h,00000FF00h,1,1000h,2 */
static int RTU95[] = {
    0x0900000,
    0x020000,
    0x00000FF00,
    1,
    0x1000,
    2,
};
/* asm: RTU94	.word	0900001h,020000h,0000000FFh,1,1000h,2 */
static int RTU94[] = {
    0x0900001,
    0x020000,
    0x0000000FF,
    1,
    0x1000,
    2,
};
/* asm: RTU101	.word	0900001h,020000h,00000FF00h,1,1000h,2 */
/* asm: 	 */
static int RTU101[] = {
    0x0900001,
    0x020000,
    0x00000FF00,
    1,
    0x1000,
    2,
};
// ;FAST RAM
/* asm: RTU57	.word	0400000h,020000h,00000FF00h,1,SOFT_WS,1 */
static int RTU57[] = {
    0x0400000,
    0x020000,
    0x00000FF00,
    1,
    SOFT_WS,
    1,
};
/* asm: RTU56	.word	0400000h,020000h,0000000FFh,1,SOFT_WS,1 */
static int RTU56[] = {
    0x0400000,
    0x020000,
    0x0000000FF,
    1,
    SOFT_WS,
    1,
};
/* asm: RTU60	.word	0400000h,020000h,0FF000000h,1,SOFT_WS,1 */
static int RTU60[] = {
    0x0400000,
    0x020000,
    0x0FF000000,
    1,
    SOFT_WS,
    1,
};
/* asm: RTU59	.word	0400000h,020000h,000FF0000h,1,SOFT_WS,1 */
static int RTU59[] = {
    0x0400000,
    0x020000,
    0x000FF0000,
    1,
    SOFT_WS,
    1,
};
// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *VECTOR PTR,XmYm,XM,YM,U_NUM
 *
 */

#define ISRAM 0
#define ISROM 1
// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
/* asm: CHIPTEST_TABLEI	.word	CHIPTEST_TABLE */
#define CHIPTEST_TABLEI CHIPTEST_TABLE
/* asm: CHIPTEST_TABLE */
/* asm: CHIPMC	RTU85,130,250,60,16,"U85",ISRAM		;COLOR RAM */
/* asm: CHIPMC	RTU87,130,270,60,16,"U87",ISRAM */
/* asm: CHIPMAC	RTU102,300,300,350,316,"U102",ISRAM	;VIDEO RAM */
/* asm: CHIPMAC	RTU95,300,280,350,296,"U95",ISRAM */
/* asm: CHIPMAC	RTU94,240,280,290,296,"U94",ISRAM */
/* asm: CHIPMAC	RTU101,240,300,290,316,"U101",ISRAM */
static int CHIPTEST_TABLE;
/* asm: STATIC_TABLE */
/* asm: CHIPMC	RTU56,250,195,40,15,"U56",ISROM */
/* asm: CHIPMC	RTU57,300,195,40,15,"U57",ISROM		;FAST RAM */
/* asm: CHIPMC	RTU59,370,195,40,15,"U59",ISROM */
/* asm: CHIPMC	RTU60,420,195,40,15,"U60",ISROM */
static int STATIC_TABLE;
/* asm: TESTING_CHIPS */
/* asm: CHIPMAC	RTU26,240,60, 290,80, "U26",ISROM	;BIG EPROM WORLD */
/* asm: CHIPMAC	RTU22,240,85, 290,105,"U22",ISROM */
/* asm: CHIPMAC	RTU18,240,110,290,130,"U18",ISROM */
/* asm: CHIPMAC	RTU14,240,135,290,155,"U14",ISROM */
/* asm: CHIPMAC	RTU10,240,160,290,180,"U10",ISROM */
/* asm: CHIPMAC	RTU27,300,60, 350,80, "U27",ISROM	;E0 */
/* asm: CHIPMAC	RTU23,300,85, 350,105,"U23",ISROM	;D8 */
/* asm: CHIPMAC	RTU19,300,110,350,130,"U19",ISROM	;D0 */
/* asm: CHIPMAC	RTU15,300,135,350,155,"U15",ISROM	;C8 */
/* asm: CHIPMAC	RTU11,300,160,350,180,"U11",ISROM	;C0 */
/* asm: CHIPMAC	RTU28,360,60, 410,80, "U28",ISROM */
/* asm: CHIPMAC	RTU24,360,85, 410,105,"U24",ISROM */
/* asm: CHIPMAC	RTU20,360,110,410,130,"U20",ISROM */
/* asm: CHIPMAC	RTU16,360,135,410,155,"U16",ISROM */
/* asm: CHIPMAC	RTU12,360,160,410,180,"U12",ISROM */
/* asm: CHIPMAC	RTU29,420,60, 470,80, "U29",ISROM */
/* asm: CHIPMAC	RTU25,420,85, 470,105,"U25",ISROM */
/* asm: CHIPMAC	RTU21,420,110,470,130,"U21",ISROM */
/* asm: CHIPMAC	RTU17,420,135,470,155,"U17",ISROM */
/* asm: CHIPMAC	RTU13,420,160,470,180,"U13",ISROM */
/* asm: CHIPMC	RTU69,360,220,40,14,"U69",ISRAM */
/* asm: CHIPMC	RTU70,360,240,40,14,"U70",ISRAM */
/* asm: CHIPMC	RTU71,360,260,40,14,"U71",ISRAM */
/* asm: CHIPMC	RTU72,360,280,40,14,"U72",ISRAM		;WAVE RAM */
/* asm: CHIPMC	RTU73,410,220,40,14,"U73",ISRAM */
/* asm: CHIPMC	RTU74,410,240,40,14,"U74",ISRAM */
/* asm: CHIPMC	RTU75,410,260,40,14,"U75",ISRAM */
/* asm: CHIPMC	RTU76,410,280,40,14,"U76",ISRAM		;WAVE RAM */
static int TESTING_CHIPS;
/* asm: CMOS_CHIP */
/* asm: CHIPMC	RTU31,130,120,40,18,"U31",ISROM		;CMOS CHIP */
static int CMOS_CHIP;

void TEST_STATIC_CHIPS(void) {
    // asm 00006300: 	DINT
    // asm 00006306: 	SETDP
    // asm 00006307: 	CALL	COPY_RAMTEST
    // asm 00006308: 	LDL	BASICS_RAM,AR5
    // asm 00006309: 	LDI	3,AR6
    // asm 0000630A: 	LDL	STATIC_TABLE,AR4
    // asm 0000630B: TSTBL1A
    // asm 0000630B: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm 0000630C: 	LDI	*AR0++,AR2
    // asm 0000630D: 	LDI	*AR0++,R2
    // asm 0000630E: 	LDI	*AR0++,R3
    // asm 0000630F: 	LDI	*AR0++,RC
    // asm 00006310: 	LDI	*AR0++,RS
    // asm 00006311: 	LDI	*AR0++,BK
    // asm 00006312: 	CALL	FEED_WATCHDOG_HARD
    // asm 00006313: 	PUSH	AR0
    // asm 00006314: 	PUSH	AR6
    // asm 00006315: 	PUSH	AR5
    // asm 00006316: 	PUSH	AR4
    // asm 00006317: 	CALL	RAMTEST
    // asm 00006318: 	POP	AR4
    // asm 00006319: 	POP	AR5
    // asm 0000631A: 	POP	AR6
    // asm 0000631B: 	POP	AR0
    // asm 0000631C: 	STI	R0,*AR5++		;SAVE RESULTS
    // asm 0000631D: 	CMPI	1,R0
    // asm 0000631E: 	BEQ	IS_STATIC_ERROR
    // asm 0000631F: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 00006320: 	DBU	AR6,TSTBL1A
    // asm 00006321: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_STATIC_CHIPS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void GET_AR4_DIGITS(void) {
    // asm 00006322: 	LDI	*+AR4(CTT_U),AR0
    // asm 00006323: 	LDI	*AR0,AR0
    // asm 00006324: 	LDI	AR0,R1
    // asm 00006325: 	RS	8,R1
    // asm 00006326: 	AND	0FFh,R1
    // asm 00006327: 	SUBI	'0',R1
    // asm 00006328: 	LDI	AR0,R2
    // asm 00006329: 	RS	16,R2
    // asm 0000632A: 	AND	0FFh,R2
    // asm 0000632B: 	CMPI	0,R2
    // asm 0000632C: 	BEQ	R1R2ZER
    // asm 0000632D: 	SUBI	'0',R2
    // asm 0000632E: 	LDI	AR0,R3
    // asm 0000632F: 	RS	24,R3
    // asm 00006330: 	AND	0FFh,R3
    // asm 00006331: 	CMPI	0,R3
    // asm 00006332: 	BEQ	R3ZERO
    // asm 00006333: 	SUBI	'0',R3
    // asm 00006334: 	BU	NZERO
R1R2ZER:
    // asm 00006335: CLRI	R2
R3ZERO:
    // asm 00006336: CLRI	R3
NZERO:
    // asm 00006337: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_AR4_DIGITS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void BLINK_AND_CONTINUE(void) {
    // asm 00006338: 	PUSH	RS
    // asm 00006339: 	PUSH	RE
    // asm 0000633A: 	PUSH	RC
    // asm 0000633B: 	PUSH	AR0
    // asm 0000633C: 	PUSH	R1
    // asm 0000633D: 	PUSH	R2
    // asm 0000633E: 	PUSH	R3
    // asm 0000633F: 	CALL	GET_AR4_DIGITS
    // asm 00006340: 	CALL	BLINK_DIGITS
    // asm 00006341: 	POP	R3
    // asm 00006342: 	POP	R2
    // asm 00006343: 	POP	R1
    // asm 00006344: 	POP	AR0
    // asm 00006345: 	POP	RC
    // asm 00006346: 	POP	RE
    // asm 00006347: 	POP	RS
    // asm 00006348: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLINK_AND_CONTINUE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void IS_STATIC_ERROR(void) {
    // asm 00006349: 	CALL	GET_AR4_DIGITS
ELOOP:
    // asm 0000634A: 	CALL	BLINK_DIGITS
    // asm 0000634B: 	BU	ELOOP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "IS_STATIC_ERROR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *
 *THIS ROUTINE TAKES OVER THE CPU, AND RETURNS
 *
 *PARAMETERS
 *	R1 DIGIT 1
 *	R2 DIGIT 2
 *	R3 DIGIT 3
 *
 *
 */
static void LLED_ON(void) {
    // asm 0000634C: 	LDI	@SYSCNTL,R0		;if the system hangs and the LED
    // asm 0000634D: 	ANDN	LED_OFF,R0		;is on we were in this routine
    // asm 0000634E: 	STI	R0,@SYSCNTL		;when it happened
    // asm 0000634F: 	LDP	@SYSCNTLR
    // asm 00006350: 	STI	R0,@SYSCNTLR
    // asm 00006351: 	SETDP
    // asm 00006352: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LLED_ON", 0, 0);
    UNIMPL();
}

static void LLED_OFF(void) {
    // asm 00006353: 	LDI	@SYSCNTL,R0
    // asm 00006354: 	OR	LED_OFF,R0
    // asm 00006355: 	STI	R0,@SYSCNTL
    // asm 00006356: 	LDP	@SYSCNTLR
    // asm 00006357: 	STI	R0,@SYSCNTLR
    // asm 00006358: 	SETDP
    // asm 00006359: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LLED_OFF", 0, 0);
    UNIMPL();
}

static void BLINK_DIGITS(void) {
    // asm 0000635A: 	PUSH	R0
    // asm 0000635B: 	PUSH	R1
    // asm 0000635C: 	PUSH	R2
    // asm 0000635D: 	PUSH	R3
    // asm 0000635E: 	PUSH	AR4
    // asm 0000635F: 	LDI	R1,AR4		;#NUMBER OF ITERATIONS
    // asm 00006360: 	DEC	AR4
    // asm 00006361: 	CMPI	0,AR4
    // asm 00006362: 	BLT	BDL1X
    // asm 00006363: BDL1
    // asm 00006363: 	CALL	LLED_ON
    // asm 00006364: 	LDIL	8000000,RC
    // asm 00006367: 	RPTS	RC
    // asm 00006368: 	NOP
    // asm 00006369: 	CALL	LLED_OFF
    // asm 0000636A: 	LDIL	8000000,RC
    // asm 0000636D: 	RPTS	RC
    // asm 0000636E: 	NOP
    // asm 0000636F: 	DBU	AR4,BDL1
BDL1X:
    // asm 00006370: 	LDIL	35000000,RC
    // asm 00006373: 	RPTS	RC
    // asm 00006374: 	NOP
    // asm 00006375: 	LDI	R2,AR4		;#NUMBER OF ITERATIONS
    // asm 00006376: 	DEC	AR4
    // asm 00006377: 	CMPI	0,AR4
    // asm 00006378: 	BLT	BDL2X
    // asm 00006379: BDL2
    // asm 00006379: 	CALL	LLED_ON
    // asm 0000637A: 	LDIL	8000000,RC
    // asm 0000637D: 	RPTS	RC
    // asm 0000637E: 	NOP
    // asm 0000637F: 	CALL	LLED_OFF
    // asm 00006380: 	LDIL	8000000,RC
    // asm 00006383: 	RPTS	RC
    // asm 00006384: 	NOP
    // asm 00006385: 	DBU	AR4,BDL2
BDL2X:
    // asm 00006386: 	LDIL	35000000,RC
    // asm 00006389: 	RPTS	RC
    // asm 0000638A: 	NOP
    // asm 0000638B: 	LDI	R3,AR4		;#NUMBER OF ITERATIONS
    // asm 0000638C: 	DEC	AR4
    // asm 0000638D: 	CMPI	0,AR4
    // asm 0000638E: 	BLT	BDL3X
BDL3:
    // asm 0000638F: CALL	LLED_ON
    // asm 00006390: 	LDIL	8000000,RC
    // asm 00006393: 	RPTS	RC
    // asm 00006394: 	NOP
    // asm 00006395: 	CALL	LLED_OFF
    // asm 00006396: 	LDIL	8000000,RC
    // asm 00006399: 	RPTS	RC
    // asm 0000639A: 	NOP
    // asm 0000639B: 	DBU	AR4,BDL3
BDL3X:
    // asm 0000639C: 	LDIL	35000000,RC
    // asm 0000639F: 	RPTS	RC
    // asm 000063A0: 	NOP
    // asm 000063A1: 	LDIL	35000000,RC
    // asm 000063A4: 	RPTS	RC
    // asm 000063A5: 	NOP
    // asm 000063A6: 	POP	AR4
    // asm 000063A7: 	POP	R3
    // asm 000063A8: 	POP	R2
    // asm 000063A9: 	POP	R1
    // asm 000063AA: 	POP	R0
    // asm 000063AB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLINK_DIGITS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

void TEST_CHIPS(void) {
    // asm 000063AC: 	PUSH	DP
    // asm 000063AD: 	LDP	@9E0000h
    // asm 000063AE: 	CLRI	R0			;set background to 0
    // asm 000063AF: 	STI	R0,@9E0000h
    // asm 000063B0: 	POP	DP
    // asm 000063B1: 	CALL	SETPAGE0
    // asm 000063B2: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 000063B5: 	STI	R0,@ACTIVE_SCREEN
    // asm 000063B6: 	DINT
    // asm 000063BC: 	CALL	COPY_RAMTEST
    // asm 000063BD: 	CALL	TEST_BASICS
    // asm 000063BE: 	LDI	COL_VDGREY,RC
    // asm 000063BF: 	TEXTITT	"CPU BOARD TEST",50,40
    // 		;
    // 		;NOW CHECK AND PLOT ACCORDINGLY
    // 		;
    // asm 000063C3: 	LDL	TESTING_CHIPS,AR4
TESTLP1:
    // asm 000063C4: 	LDI	*+AR4(CTT_RAMROM),R0
    // asm 000063C5: 	BZ	CHECK_AS_RAM	;(W/ SCRAMBLE)
    // asm 000063C6: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm 000063C7: 	LDI	*AR0++,AR2	;ADDR
    // asm 000063C8: 	LDI	*AR0++,RC	;LENGTH
    // asm 000063C9: 	LDI	*AR0++,R2	;MASK
    // asm 000063CA: 	LDI	*AR0++,R3	;SHIFT
    // asm 000063CB: 	CALL	GENERATE_CHECKSUM
    // asm 000063CC: 	LDI	*AR0,AR0
    // asm 000063CD: 	LDI	*AR0,R1
    // asm 000063CE: 	CMPI	R1,R0
    // asm 000063CF: 	LDIEQ	COL_GREEN,RS
    // asm 000063D0: 	LDINE	COL_RED,RS
    // ;	CMPI	COL_RED,RS
    // ;	CALLEQ	BLINK_AND_CONTINUE
    // 	;CHECKSUM PART
    // asm 000063D1: 	BU	DN_CHK
CHECK_AS_RAM:
    // asm 000063D2: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm 000063D3: 	CMPI	-1,AR0
    // asm 000063D4: 	BEQ	TEST_CHIPSX
    // asm 000063D5: 	LDI	*AR0++,AR2
    // asm 000063D6: 	LDI	*AR0++,R2
    // asm 000063D7: 	LDI	*AR0++,R3
    // asm 000063D8: 	LDI	*AR0++,RC
    // asm 000063D9: 	LDI	*AR0++,RS
    // asm 000063DA: 	LDI	*AR0++,BK
    // asm 000063DB: 	PUSH	AR0
    // asm 000063DC: 	PUSH	AR4
    // asm 000063DD: 	CALL	RAMTEST
    // asm 000063DE: 	POP	AR4
    // asm 000063DF: 	POP	AR0
    // asm 000063E0: 	LDI	R0,R0
    // asm 000063E1: 	LDIZ	COL_GREEN,RS
    // ;	LDIZ	COL_GREY,RS
    // asm 000063E2: 	LDINZ	COL_RED,RS
    // ;	CMPI	COL_RED,RS
    // ;	CALLEQ	BLINK_AND_CONTINUE
DN_CHK:
    // asm 000063E3: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 000063E4: 	INC	AR2
    // asm 000063E5: 	LDI	*+AR4(CTT_MINY),R2
    // asm 000063E6: 	INC	R2
    // asm 000063E7: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 000063E8: 	DEC	R3
    // asm 000063E9: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 000063EA: 	DEC	RC
    // asm 000063EB: 	CALL	_fill
    // asm 000063EC: 	LDI	*+AR4(CTT_U),AR2
    // asm 000063ED: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 000063EE: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 000063EF: 	SUBI	*+AR4(CTT_MINX),R2
    // asm 000063F0: 	SUBI	*+AR4(CTT_MINY),R3
    // asm 000063F1: 	RS	1,R2
    // asm 000063F2: 	RS	1,R3
    // asm 000063F3: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 000063F4: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 000063F5: 	SUBI	15,R2
    // asm 000063F6: 	SUBI	4,R3
    // asm 000063F7: 	LDI	COL_BLACK,RC
    // asm 000063F8: 	CALL	_outtextxyc
    // asm 000063F9: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 000063FA: 	BNZ	TESTLP1
TEST_CHIPSX:
    // asm 000063FB: 	CALL	CMOS_CHIP_TEST
    // asm 000063FC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_CHIPS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void PLOT_OUTLINE_OF_CHIPS(void) {
    // asm 000063FD: 	LDI	@CHIPTEST_TABLEI,AR4
POOCL:
    // asm 000063FE: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 000063FF: 	LDI	*+AR4(CTT_MINY),R2
    // asm 00006400: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 00006401: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 00006402: 	LDI	COL_WHITE,RS
    // asm 00006403: 	PUSH	DP
    // asm 00006404: 	CALL	_rectangle
    // asm 00006405: 	POP	DP
    // asm 00006406: 	LDI	*+AR4(CTT_U),AR2
    // asm 00006407: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 00006408: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 00006409: 	SUBI	*+AR4(CTT_MINX),R2
    // asm 0000640A: 	SUBI	*+AR4(CTT_MINY),R3
    // asm 0000640B: 	RS	1,R2
    // asm 0000640C: 	RS	1,R3
    // asm 0000640D: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 0000640E: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 0000640F: 	SUBI	15,R2
    // asm 00006410: 	SUBI	4,R3
    // asm 00006411: 	LDI	COL_DGREY,RC
    // asm 00006412: 	CALL	_outtextxyc
    // asm 00006413: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 00006414: 	BNZ	POOCL
    // asm 00006415: 	CALL	CMOS_CHIP_DISPLAY
    // asm 00006416: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLOT_OUTLINE_OF_CHIPS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CMOS_CHIP_DISPLAY(void) {
    // asm 00006417: 	LDL	CMOS_CHIP,AR4
    // asm 00006418: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 00006419: 	LDI	*+AR4(CTT_MINY),R2
    // asm 0000641A: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 0000641B: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 0000641C: 	LDI	COL_WHITE,RS
    // asm 0000641D: 	PUSH	DP
    // asm 0000641E: 	CALL	_rectangle
    // asm 0000641F: 	POP	DP
    // asm 00006420: 	LDI	*+AR4(CTT_U),AR2
    // asm 00006421: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 00006422: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 00006423: 	SUBI	*+AR4(CTT_MINX),R2
    // asm 00006424: 	SUBI	*+AR4(CTT_MINY),R3
    // asm 00006425: 	RS	1,R2
    // asm 00006426: 	RS	1,R3
    // asm 00006427: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 00006428: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 00006429: 	SUBI	15,R2
    // asm 0000642A: 	SUBI	4,R3
    // asm 0000642B: 	LDI	COL_DGREY,RC
    // asm 0000642C: 	CALL	_outtextxyc
    // asm 0000642D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_CHIP_DISPLAY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void CMOS_CHIP_TEST(void) {
    // asm 0000642E: 	PUSH	AR4
    // asm 0000642F: 	PUSH	R0
    // asm 00006430: 	PUSH	R1
    // asm 00006431: 	PUSH	R2
    // asm 00006432: 	PUSH	R3
    // asm 00006433: 	LDL	CMOS_CHIP,AR4
    // asm 00006434: 	LDIL	5A5A5A5Ah,R3
    // asm 00006437: 	LDP	@CPU_WS
    // asm 00006438: 	LDI	88h,R1
    // asm 00006439: 	STI	R1,@CPU_WS
    // asm 0000643A: 	SETDP
    // asm 0000643B: 	CMOS_WP_OFF
    // asm 0000643C: 	NOP			;DELAY FOR TIMING...
    // asm 0000643D: 	LDI	5,AR5
KKLL1:
    // asm 0000643E: LDI	R3,R2
    // asm 0000643F: 	AND	0FFh,R2
    // asm 00006440: 	LS	24,R2
    // asm 00006441: 	LDI	AUD_CHIPTEST_DEDICATED,AR2
    // asm 00006442: 	LS	2,AR2
    // asm 00006443: 	ADDI	@CMOSI,AR2
    // asm 00006444: 	STI	R2,*AR2
    // asm 00006445: 	RS	24,R2
    // asm 00006446: 	LDL	0C00000h,AR0		;DUMMY READ
    // asm 00006447: 	LDI	*AR0,R0
    // asm 00006448: 	LDI	*AR2,R0
    // asm 00006449: 	RS	24,R0
    // asm 0000644A: 	CMPI	R2,R0
    // asm 0000644B: 	BNE	CM_ISERROR
    // asm 0000644C: 	LDI	R3,R1
    // asm 0000644D: 	LSH	1,R3
    // asm 0000644E: 	XOR	R3,R1
    // asm 0000644F: 	BNN	RND2
    // asm 00006450: 	OR	1,R3
RND2:
    // asm 00006451: MPYI	794Fh,R3
    // asm 00006452: 	DBU	AR5,KKLL1
    // asm 00006453: 	CMOS_WP_ON
    // 	;NO ERROR
    // asm 00006454: 	LDI	COL_GREEN,RS
    // asm 00006455: 	BU	KKJJ
CM_ISERROR:
    // asm 00006456: 	CMOS_WP_ON
    // asm 00006457: 	LDI	COL_RED,RS
KKJJ:
    // ;	CMPI	COL_RED,RS
    // ;	CALLEQ	BLINK_AND_CONTINUE
    // asm 00006458: 	LDP	@CPU_WS
    // asm 00006459: 	LDI	1048h,R0
    // asm 0000645A: 	STI	R0,@CPU_WS
    // asm 0000645B: 	SETDP
    // asm 0000645C: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 0000645D: 	INC	AR2
    // asm 0000645E: 	LDI	*+AR4(CTT_MINY),R2
    // asm 0000645F: 	INC	R2
    // asm 00006460: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 00006461: 	DEC	R3
    // asm 00006462: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 00006463: 	DEC	RC
    // asm 00006464: 	CALL	_fill
    // asm 00006465: 	LDI	*+AR4(CTT_U),AR2
    // asm 00006466: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 00006467: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 00006468: 	SUBI	*+AR4(CTT_MINX),R2
    // asm 00006469: 	SUBI	*+AR4(CTT_MINY),R3
    // asm 0000646A: 	RS	1,R2
    // asm 0000646B: 	RS	1,R3
    // asm 0000646C: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 0000646D: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 0000646E: 	SUBI	15,R2
    // asm 0000646F: 	SUBI	4,R3
    // asm 00006470: 	LDI	COL_BLACK,RC
    // asm 00006471: 	CALL	_outtextxyc
    // asm 00006472: 	POP	R3
    // asm 00006473: 	POP	R2
    // asm 00006474: 	POP	R1
    // asm 00006475: 	POP	R0
    // asm 00006476: 	POP	AR4
    // asm 00006477: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_CHIP_TEST", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *FOR THE DISPLAY TO WORK WE MUST HAVE OPERATING PROPERLY:
 *	VIDEO RAM (8 CHIPS)
 *	COLOR RAM (2 CHIPS)
 *SO, WE TEST THESE (NOT PLOTTING TO THE SCREEN), AND SAVE THE RESULTS,
 *THEN WE BEGIN PLOTTING THE SCREEN, INITIALIZATING THESE SPACES WITH THE
 *RESULTS.
 *
 *
 */
/* asm: BASICS_RAM	fbss	BASICS_RAM,10 */
int BASICS_RAM[10];

static void TEST_BASICS(void) {
    // asm 00006478: 	LDL	BASICS_RAM,AR5
    // asm 00006479: 	LDI	5,AR6
    // asm 0000647A: 	LDL	CHIPTEST_TABLE,AR4
    // asm 0000647B: TSTBL1
    // asm 0000647B: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm 0000647C: 	LDI	*AR0++,AR2
    // asm 0000647D: 	LDI	*AR0++,R2
    // asm 0000647E: 	LDI	*AR0++,R3
    // asm 0000647F: 	LDI	*AR0++,RC
    // asm 00006480: 	LDI	*AR0++,RS
    // asm 00006481: 	LDI	*AR0++,BK
    // asm 00006482: 	PUSH	AR0
    // asm 00006483: 	PUSH	AR6
    // asm 00006484: 	PUSH	AR5
    // asm 00006485: 	PUSH	AR4
    // asm 00006486: 	CALL	RAMTEST
    // asm 00006487: 	POP	AR4
    // asm 00006488: 	POP	AR5
    // asm 00006489: 	POP	AR6
    // asm 0000648A: 	POP	AR0
    // asm 0000648B: 	STI	R0,*AR5++		;SAVE RESULTS
    // asm 0000648C: 	CMPI	0,R0
    // asm 0000648D: 	CALLNZ	BLINK_AND_CONTINUE
    // asm 0000648E: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 0000648F: 	DBU	AR6,TSTBL1
    // asm 00006490: 	CALL	HARD_LOAD_DIAGPAL
    // asm 00006491: 	CLRI	R0
    // asm 00006492: 	STI	R0,@COLORAM
    // asm 00006493: 	CALL	CLRSCRN			;we are NOT page flipping
    // asm 00006494: 	PUSH	DP
    // asm 00006495: 	LINE	20,50,490,50,COL_WHITE
    // asm 0000649B: 	LINE	20,50,20,360,COL_WHITE
    // asm 000064A1: 	LINE	490,50,490,360,COL_WHITE
    // asm 000064A7: 	LINE	20,360,80,360,COL_WHITE
    // asm 000064AD: 	LINE	80,360,80,340,COL_WHITE
    // asm 000064B3: 	LINE	80,340,100,340,COL_WHITE
    // asm 000064B9: 	LINE	100,340,100,360,COL_WHITE
    // asm 000064BF: 	LINE	100,360,200,360,COL_WHITE
    // asm 000064C5: 	LINE	200,360,200,340,COL_WHITE
    // asm 000064CB: 	LINE	200,340,220,340,COL_WHITE
    // asm 000064D1: 	LINE	220,340,220,360,COL_WHITE
    // asm 000064D7: 	LINE	220,360,490,360,COL_WHITE
    // asm 000064DD: 	POP	DP
    // asm 000064DE: 	CALL	PLOT_OUTLINE_OF_CHIPS
    // asm 000064DF: 	LDI	COL_VDGREY,RC
    // asm 000064E0: 	TEXTIT	MEMTST1,50,20
    // asm 000064E4: 	TEXTIT	DATE_STAMP,50,30
    // asm 000064E8: 	TEXTIT	VERSION_STAMP,270,30
    // asm 000064EC: 	LDL	BASICS_RAM,AR5
    // asm 000064ED: 	LDI	9,AR6
    // asm 000064EE: 	LDL	CHIPTEST_TABLE,AR4
    // asm 000064EF: TSTBL2
    // asm 000064EF: 	LDI	*AR5++,R0		;SAVE RESULTS
    // asm 000064F0: 	LDIZ	COL_GREEN,RS
    // asm 000064F1: 	LDINZ	COL_RED,RS
    // asm 000064F2: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 000064F3: 	INC	AR2
    // asm 000064F4: 	LDI	*+AR4(CTT_MINY),R2
    // asm 000064F5: 	INC	R2
    // asm 000064F6: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 000064F7: 	DEC	R3
    // asm 000064F8: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 000064F9: 	DEC	RC
    // asm 000064FA: 	CALL	_fill
    // asm 000064FB: 	LDI	*+AR4(CTT_U),AR2
    // asm 000064FC: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 000064FD: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 000064FE: 	SUBI	*+AR4(CTT_MINX),R2
    // asm 000064FF: 	SUBI	*+AR4(CTT_MINY),R3
    // asm 00006500: 	RS	1,R2
    // asm 00006501: 	RS	1,R3
    // asm 00006502: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 00006503: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 00006504: 	SUBI	15,R2
    // asm 00006505: 	SUBI	4,R3
    // asm 00006506: 	LDI	COL_BLACK,RC
    // asm 00006507: 	CALL	_outtextxyc
    // asm 00006508: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 00006509: 	DBU	AR6,TSTBL2
    // 	;Static *MUST* be working to get this far,
    // 	;so mark them ALL as cool
    // 	;
    // asm 0000650A: 	LDI	3,AR6
    // asm 0000650B: 	LDL	STATIC_TABLE,AR4
    // asm 0000650C: TSTBL2C
    // asm 0000650C: 	LDI	COL_GREEN,RS
    // asm 0000650D: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 0000650E: 	INC	AR2
    // asm 0000650F: 	LDI	*+AR4(CTT_MINY),R2
    // asm 00006510: 	INC	R2
    // asm 00006511: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 00006512: 	DEC	R3
    // asm 00006513: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 00006514: 	DEC	RC
    // asm 00006515: 	CALL	_fill
    // asm 00006516: 	LDI	*+AR4(CTT_U),AR2
    // asm 00006517: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 00006518: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 00006519: 	SUBI	*+AR4(CTT_MINX),R2
    // asm 0000651A: 	SUBI	*+AR4(CTT_MINY),R3
    // asm 0000651B: 	RS	1,R2
    // asm 0000651C: 	RS	1,R3
    // asm 0000651D: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 0000651E: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 0000651F: 	SUBI	15,R2
    // asm 00006520: 	SUBI	4,R3
    // asm 00006521: 	LDI	COL_BLACK,RC
    // asm 00006522: 	CALL	_outtextxyc
    // asm 00006523: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 00006524: 	DBU	AR6,TSTBL2C
    // asm 00006525: 	CALL	CMOS_CHIP_TEST
    // asm 00006526: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_BASICS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *TEST RAM AREA
 *CALLING PARAMETERS
 *
 *PARAMETERS
 *	AR2	START ADDR
 *	R2	LENGTH WORDS
 *	R3	MASK (FOR BYTE WIDE CHIPS)
 *	RC	# OF PASSES
 *	RS	WAIT STATE CODE
 *	BK	INCREMENT (USUALLY 1)
 *
 *RETURNS
 *	R0	1 ON ERROR
 *	R0	0 ON NO ERROR
 *
 *CLOBBERS
 *	DP,RS,RE,RC
 *	R0,R1,R2,R3,R4,R5
 *	AR0,AR1,AR2,AR3
 *USES
 *	AR6	WATCHDOG FEEDER
 *
 *
 */
static void RAMTEST(void) {
    // asm 00006527: 	LDL	BLOWLIST,R0
    // asm 00006528: 	BU	R0
    // 		;THIS IS THE ACTUAL ROUTINE, BUT IT MUST
    // 		;BE PLACED IN ON-CHIP MEMORY
    // 		;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMTEST", 0, 0);
    UNIMPL();
}

static void RAMTEST_REAL(void) {
    // asm 00006529: 	PUSH	DP
    // asm 0000652A: 	PUSH	R6
    // asm 0000652B: 	PUSH	AR6
    // asm 0000652C: 	LCALL	FEED_WATCHDOG_HARD
    // asm 00006536: 	LDP	@CPU_WS		;set hardware wait states
    // asm 00006537: 	LDI	@CPU_WS,R0
    // asm 00006538: 	PUSH	R0
    // asm 00006539: 	STI	RS,@CPU_WS
    // asm 0000653A: 	LDI	0,AR3		;DUMMY READ ADDRESS
    // asm 0000653B: 	LDI	*AR3,R5		;DUMMY READ
    // asm 0000653C: 	LDP	@RANDI
    // asm 0000653D: 	LDI	@RANDI,R4	;SETUP R4=RANDOM # REGISTER
    // asm 0000653E: 	LDI	RC,AR0		;NUMBER OF PASSES
    // asm 0000653F: 	DEC	AR0
    // asm 00006540: 	DEC	R2		;LENGTH OF CHECK
    // asm 00006541: 	LDI	R2,AR4
    // asm 00006542: RAMTST0
    // asm 00006542: 	LDI	AR2,AR1		;GET ADDRESS
    // asm 00006543: 	LDI	R4,R0		;GET RANDOM # STARTING POINT
    // *WRITE RANDOM NUMBERS TO RAM
    // asm 00006544: RAMTST1
    // asm 00006544: 	LDI	R0,R1
    // asm 00006545: 	LSH	1,R0
    // asm 00006546: 	LDI	0,R6
    // asm 00006547: 	XOR	R0,R1
    // asm 00006548: 	LDIN	2,R6		;	BNN	RAMTST2
    // asm 00006549: 	OR	R6,R0		;	OR	2,R0
    // 				;RAMTST2
    // asm 0000654A: 	DBUD	AR4,RAMTST1
    // asm 0000654B: 	STI	R0,*AR1
    // asm 0000654C: 	ADDI	BK,AR1
LKKK:
    // asm 0000654D: LDI	*AR3,R5		;DUMMY READ
    // *READ RANDOM NUMBERS FROM RAM
    // asm 0000654E: 	LDI	AR2,AR1		;GET ADDRESS
    // asm 0000654F: 	LDI	R4,R0		;GET RANDOM # STARTING POINT
    // asm 00006550: 	LDI	R0,R1
    // asm 00006551: 	LSH	1,R0
    // asm 00006552: 	CLRI	R6
    // asm 00006553: 	CLRI	R7
    // asm 00006554: 	LDI	R2,AR4		;LENGTH
    // asm 00006555: 	XOR	R0,R1
    // asm 00006556: 	LDIN	2,R6		;	BNN	RAMTST4
    // asm 00006557: RAMTST3
    // asm 00006557: 	OR	R6,R0		;	OR	2,R0
    // 				;RAMTST4
    // asm 00006558: 	INC	R7
    // asm 00006559: 	LDI	*AR1,R1
    // asm 0000655A: 	ADDI	BK,AR1
    // asm 0000655B: 	LDI	*AR3,R5		;DUMMY READ
    // asm 0000655C: 	AND	R3,R1
    // asm 0000655D: 	LDI	R0,R4
    // asm 0000655E: 	AND	R3,R0
    // asm 0000655F: 	CMPI	R1,R0
    // asm 00006560: 	BNE	RAMERR
    // asm 00006561: 	LDI	R4,R0
    // asm 00006562: 	LDI	R0,R1		;mirrored from above
    // asm 00006563: 	LSH	1,R0
    // 	;---->BNE	RAMERR
    // asm 00006564: 	DBUD	AR4,RAMTST3
JJJH:
    // asm 00006565: CLRI	R6
    // asm 00006566: 	XOR	R0,R1
    // asm 00006567: 	LDIN	2,R6		;	BNN	RAMTST4
    // 	;---->	DBUD	AR4,RAMTST3
    // asm 00006568: RAMRPT
    // asm 00006568: 	DBU	AR0,RAMTST0
    // asm 00006569: 	CLRI	R0		;IS GOOD RAM
ENDING:
    // asm 0000656A: 	POP	R1		;FIND WAIT STATES
    // asm 0000656B: 	LDP	@CPU_WS		;set original wait states
    // asm 0000656C: 	STI	R1,@CPU_WS
    // asm 0000656D: 	POP	AR6
    // asm 0000656E: 	POP	R6
    // asm 0000656F: 	POP	DP
    // asm 00006570: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMTEST_REAL", 0, 0);
    UNIMPL();
}

static void RAMERR(void) {
    // asm 00006571: 	LDI	1,R0		;IS BAD RAM
    // asm 00006572: 	BU	ENDING
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMERR", 0, 0);
    UNIMPL();
}

static void RAMTEST_END(void) {
    /* no executable asm lines detected */
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
static void COPY_RAMTEST(void) {
    // asm 00006573: 	LDL	RAMTEST_END,AR0
    // asm 00006574: 	LDL	RAMTEST_REAL,AR1
    // asm 00006575: 	SUBI	AR1,AR0			;GET LENGTH
    // asm 00006576: 	DEC	AR0
    // asm 00006577: 	LDL	BLOWLIST,AR2
    // asm 00006578: 	LDI	AR0,RC
    // asm 00006579: 	RPTB	BBCP
    // asm 0000657A: 	LDI	*AR1++,R0
BBCP:
    // asm 0000657B: STI	R0,*AR2++
    // asm 0000657C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COPY_RAMTEST", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *GENERATE_CHECKSUM
 *
 *GENERATES A 16 BIT CHECKSUM
 *
 *
 *PARAMETERS
 *	AR2	START ADDR
 *	RC	LENGTH WORDS
 *	R2	MASK
 *	R3	SHIFT
 *
 *RETURNS
 *	R0	PART CHECKSUM
 *
 */
static void GENERATE_CHECKSUM(void) {
    // asm 0000657D: 	PUSH	R1
    // asm 0000657E: 	PUSH	RC
    // asm 0000657F: 	PUSH	AR2
    // asm 00006580: 	LCALL	FEED_WATCHDOG
    // asm 0000658A: 	CLRI	R0
    // asm 0000658B: 	DEC	RC
    // asm 0000658C: 	RPTB	CHKSUMG
    // asm 0000658D: 	LDI	*AR2++,R1
    // asm 0000658E: 	AND	R2,R1
    // asm 0000658F: 	LSH	R3,R1
CHKSUMG:
    // asm 00006590: ADDI	R1,R0
    // asm 00006591: 	LS	16,R0
    // asm 00006592: 	RS	16,R0
    // asm 00006593: 	POP	AR2
    // asm 00006594: 	POP	RC
    // asm 00006595: 	POP	R1
    // asm 00006596: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GENERATE_CHECKSUM", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ROM CHIP CHECKSUMS
 *
 */

#if CHECKSUM_GEN
/* asm: CHKSUMC00	.word	-1 */
#define CHKSUMC00 CHECKSUM_C00
/* asm: CHKSUMC80	.word	-1 */
#define CHKSUMC80 CHECKSUM_C80
/* asm: CHKSUMD00	.word	-1 */
#define CHKSUMD00 CHECKSUM_D00
/* asm: CHKSUMD80	.word	-1 */
#define CHKSUMD80 CHECKSUM_D80
/* asm: CHKSUME00	.word	-1 */
/* asm: 	 */
#define CHKSUME00 CHECKSUM_E00
/* asm: CHKSUMC01	.word	-1 */
#define CHKSUMC01 CHECKSUM_C01
/* asm: CHKSUMC81	.word	-1 */
#define CHKSUMC81 CHECKSUM_C81
/* asm: CHKSUMD01	.word	-1 */
#define CHKSUMD01 CHECKSUM_D01
/* asm: CHKSUMD81	.word	-1 */
#define CHKSUMD81 CHECKSUM_D81
/* asm: CHKSUME01	.word	-1 */
/* asm: 	 */
#define CHKSUME01 CHECKSUM_E01
/* asm: CHKSUMC02	.word	-1 */
#define CHKSUMC02 CHECKSUM_C02
/* asm: CHKSUMC82	.word	-1 */
#define CHKSUMC82 CHECKSUM_C82
/* asm: CHKSUMD02	.word	-1 */
#define CHKSUMD02 CHECKSUM_D02
/* asm: CHKSUMD82	.word	-1 */
#define CHKSUMD82 CHECKSUM_D82
/* asm: CHKSUME02	.word	-1 */
/* asm: 	 */
#define CHKSUME02 CHECKSUM_E02
/* asm: CHKSUMC03	.word	-1 */
#define CHKSUMC03 CHECKSUM_C03
/* asm: CHKSUMC83	.word	-1 */
#define CHKSUMC83 CHECKSUM_C83
/* asm: CHKSUMD03	.word	-1 */
#define CHKSUMD03 CHECKSUM_D03
/* asm: CHKSUMD83	.word	-1 */
#define CHKSUMD83 CHECKSUM_D83
/* asm: CHKSUME03	.word	-1 */
/* asm: 	 */
/* asm: 	 */
#define CHKSUME03 CHECKSUM_E03
/* asm: CCHKSUMC00	.word	0 */
static int CCHKSUMC00 = 0;
/* asm: CCHKSUMC80	.word	0 */
static int CCHKSUMC80 = 0;
/* asm: CCHKSUMD00	.word	0 */
static int CCHKSUMD00 = 0;
/* asm: CCHKSUMD80	.word	0 */
static int CCHKSUMD80 = 0;
/* asm: CCHKSUME00	.word	0 */
/* asm: 	 */
static int CCHKSUME00 = 0;
/* asm: CCHKSUMC01	.word	0 */
static int CCHKSUMC01 = 0;
/* asm: CCHKSUMC81	.word	0 */
static int CCHKSUMC81 = 0;
/* asm: CCHKSUMD01	.word	0 */
static int CCHKSUMD01 = 0;
/* asm: CCHKSUMD81	.word	0 */
static int CCHKSUMD81 = 0;
/* asm: CCHKSUME01	.word	0 */
/* asm: 	 */
static int CCHKSUME01 = 0;
/* asm: CCHKSUMC02	.word	0 */
static int CCHKSUMC02 = 0;
/* asm: CCHKSUMC82	.word	0 */
static int CCHKSUMC82 = 0;
/* asm: CCHKSUMD02	.word	0 */
static int CCHKSUMD02 = 0;
/* asm: CCHKSUMD82	.word	0 */
static int CCHKSUMD82 = 0;
/* asm: CCHKSUME02	.word	0 */
/* asm: 	 */
static int CCHKSUME02 = 0;
/* asm: CCHKSUMC03	.word	0 */
static int CCHKSUMC03 = 0;
/* asm: CCHKSUMC83	.word	0 */
static int CCHKSUMC83 = 0;
/* asm: CCHKSUMD03	.word	0 */
static int CCHKSUMD03 = 0;
/* asm: CCHKSUMD83	.word	0 */
static int CCHKSUMD83 = 0;
/* asm: CCHKSUME03	.word	0 */
static int CCHKSUME03 = 0;
#else
/* asm: CHKSUMC00	.word	CHECKSUM_C00 */
#define CHKSUMC00 CHECKSUM_C00
/* asm: CHKSUMC80	.word	CHECKSUM_C80 */
#define CHKSUMC80 CHECKSUM_C80
/* asm: CHKSUMD00	.word	CHECKSUM_D00 */
#define CHKSUMD00 CHECKSUM_D00
/* asm: CHKSUMD80	.word	CHECKSUM_D80 */
#define CHKSUMD80 CHECKSUM_D80
/* asm: CHKSUME00	.word	CHECKSUM_E00 */
/* asm: 	 */
#define CHKSUME00 CHECKSUM_E00
/* asm: CHKSUMC01	.word	CHECKSUM_C01 */
#define CHKSUMC01 CHECKSUM_C01
/* asm: CHKSUMC81	.word	CHECKSUM_C81 */
#define CHKSUMC81 CHECKSUM_C81
/* asm: CHKSUMD01	.word	CHECKSUM_D01 */
#define CHKSUMD01 CHECKSUM_D01
/* asm: CHKSUMD81	.word	CHECKSUM_D81 */
#define CHKSUMD81 CHECKSUM_D81
/* asm: CHKSUME01	.word	CHECKSUM_E01 */
/* asm: 	 */
#define CHKSUME01 CHECKSUM_E01
/* asm: CHKSUMC02	.word	CHECKSUM_C02 */
#define CHKSUMC02 CHECKSUM_C02
/* asm: CHKSUMC82	.word	CHECKSUM_C82 */
#define CHKSUMC82 CHECKSUM_C82
/* asm: CHKSUMD02	.word	CHECKSUM_D02 */
#define CHKSUMD02 CHECKSUM_D02
/* asm: CHKSUMD82	.word	CHECKSUM_D82 */
#define CHKSUMD82 CHECKSUM_D82
/* asm: CHKSUME02	.word	CHECKSUM_E02 */
/* asm: 	 */
#define CHKSUME02 CHECKSUM_E02
/* asm: CHKSUMC03	.word	CHECKSUM_C03 */
#define CHKSUMC03 CHECKSUM_C03
/* asm: CHKSUMC83	.word	CHECKSUM_C83 */
#define CHKSUMC83 CHECKSUM_C83
/* asm: CHKSUMD03	.word	CHECKSUM_D03 */
#define CHKSUMD03 CHECKSUM_D03
/* asm: CHKSUMD83	.word	CHECKSUM_D83 */
#define CHKSUMD83 CHECKSUM_D83
/* asm: CHKSUME03	.word	CHECKSUM_E03 */
/* asm: 	 */
#define CHKSUME03 CHECKSUM_E03
/* asm: CCHKSUMC00	.word	~CHECKSUM_C00 */
static int CCHKSUMC00 = ~CHECKSUM_C00;
/* asm: CCHKSUMC80	.word	~CHECKSUM_C80 */
static int CCHKSUMC80 = ~CHECKSUM_C80;
/* asm: CCHKSUMD00	.word	~CHECKSUM_D00 */
static int CCHKSUMD00 = ~CHECKSUM_D00;
/* asm: CCHKSUMD80	.word	~CHECKSUM_D80 */
static int CCHKSUMD80 = ~CHECKSUM_D80;
/* asm: CCHKSUME00	.word	~CHECKSUM_E00 */
/* asm: 	 */
static int CCHKSUME00 = ~CHECKSUM_E00;
/* asm: CCHKSUMC01	.word	~CHECKSUM_C01 */
static int CCHKSUMC01 = ~CHECKSUM_C01;
/* asm: CCHKSUMC81	.word	~CHECKSUM_C81 */
static int CCHKSUMC81 = ~CHECKSUM_C81;
/* asm: CCHKSUMD01	.word	~CHECKSUM_D01 */
static int CCHKSUMD01 = ~CHECKSUM_D01;
/* asm: CCHKSUMD81	.word	~CHECKSUM_D81 */
static int CCHKSUMD81 = ~CHECKSUM_D81;
/* asm: CCHKSUME01	.word	~CHECKSUM_E01 */
/* asm: 	 */
static int CCHKSUME01 = ~CHECKSUM_E01;
/* asm: CCHKSUMC02	.word	~CHECKSUM_C02 */
static int CCHKSUMC02 = ~CHECKSUM_C02;
/* asm: CCHKSUMC82	.word	~CHECKSUM_C82 */
static int CCHKSUMC82 = ~CHECKSUM_C82;
/* asm: CCHKSUMD02	.word	~CHECKSUM_D02 */
static int CCHKSUMD02 = ~CHECKSUM_D02;
/* asm: CCHKSUMD82	.word	~CHECKSUM_D82 */
static int CCHKSUMD82 = ~CHECKSUM_D82;
/* asm: CCHKSUME02	.word	~CHECKSUM_E02 */
/* asm: 	 */
static int CCHKSUME02 = ~CHECKSUM_E02;
/* asm: CCHKSUMC03	.word	~CHECKSUM_C03 */
static int CCHKSUMC03 = ~CHECKSUM_C03;
/* asm: CCHKSUMC83	.word	~CHECKSUM_C83 */
static int CCHKSUMC83 = ~CHECKSUM_C83;
/* asm: CCHKSUMD03	.word	~CHECKSUM_D03 */
static int CCHKSUMD03 = ~CHECKSUM_D03;
/* asm: CCHKSUMD83	.word	~CHECKSUM_D83 */
static int CCHKSUMD83 = ~CHECKSUM_D83;
/* asm: CCHKSUME03	.word	~CHECKSUM_E03 */
/* asm: 	 */
static int CCHKSUME03 = ~CHECKSUM_E03;
#endif

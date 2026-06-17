#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "vunit.h"
#include "cmos.h"
#include "macs.h"
#include "sys.h"
#include "diag.h"
#include "cksum.h"
#include "globals.h"
#include "checksum.h"
#include "port.h"
#include "memtest.h"

/*
 * Source module: asm/MEMTEST.ASM
 */

void TEST_STATIC_CHIPS(void);
void GET_AR4_DIGITS(void);
void BLINK_AND_CONTINUE(void);
void IS_STATIC_ERROR(void);
void LLED_ON(void);
void LLED_OFF(void);
void BLINK_DIGITS(void);
void TEST_CHIPS(void);
void PLOT_OUTLINE_OF_CHIPS(void);
void CMOS_CHIP_DISPLAY(void);
void CMOS_CHIP_TEST(void);
void TEST_BASICS(void);
void RAMTEST(void);
void RAMTEST_REAL(void);
void RAMERR(void);
void RAMTEST_END(void);
void COPY_RAMTEST(void);
void GENERATE_CHECKSUM(void);

/* *----------------------------------------------------------------------------
 */
/* asm: RANDI	.word	5A5A5A5Ah */
int RANDI = 0x5A5A5A5A;
/* ;EPROM
 */
/* asm: RTU26	.word	0E00000h,080000h,0000000FFh,0,CHKSUME00 */
int RTU26[] = {
    0x0E00000, 0x080000, 0x0000000FF, 0, CHKSUME00,
};
/* asm: RTU22	.word	0D80000h,080000h,0000000FFh,0,CHKSUMD80 */
int RTU22[] = {
    0x0D80000, 0x080000, 0x0000000FF, 0, CHKSUMD80,
};
/* asm: RTU18	.word	0D00000h,080000h,0000000FFh,0,CHKSUMD00 */
int RTU18[] = {
    0x0D00000, 0x080000, 0x0000000FF, 0, CHKSUMD00,
};
/* asm: RTU14	.word	0C80000h,080000h,0000000FFh,0,CHKSUMC80 */
int RTU14[] = {
    0x0C80000, 0x080000, 0x0000000FF, 0, CHKSUMC80,
};
/* asm: RTU10	.word	0C00000h,080000h,0000000FFh,0,CHKSUMC00 */
int RTU10[] = {
    0x0C00000, 0x080000, 0x0000000FF, 0, CHKSUMC00,
};
/* asm: RTU27	.word	0E00000h,080000h,00000FF00h,-8,CHKSUME01 */
int RTU27[] = {
    0x0E00000, 0x080000, 0x00000FF00, -8, CHKSUME01,
};
/* asm: RTU23	.word	0D80000h,080000h,00000FF00h,-8,CHKSUMD81 */
int RTU23[] = {
    0x0D80000, 0x080000, 0x00000FF00, -8, CHKSUMD81,
};
/* asm: RTU19	.word	0D00000h,080000h,00000FF00h,-8,CHKSUMD01 */
int RTU19[] = {
    0x0D00000, 0x080000, 0x00000FF00, -8, CHKSUMD01,
};
/* asm: RTU15	.word	0C80000h,080000h,00000FF00h,-8,CHKSUMC81 */
int RTU15[] = {
    0x0C80000, 0x080000, 0x00000FF00, -8, CHKSUMC81,
};
/* asm: RTU11	.word	0C00000h,080000h,00000FF00h,-8,CHKSUMC01 */
int RTU11[] = {
    0x0C00000, 0x080000, 0x00000FF00, -8, CHKSUMC01,
};
/* asm: RTU28	.word	0E00000h,080000h,000FF0000h,-16,CHKSUME02 */
int RTU28[] = {
    0x0E00000, 0x080000, 0x000FF0000, -16, CHKSUME02,
};
/* asm: RTU24	.word	0D80000h,080000h,000FF0000h,-16,CHKSUMD82 */
int RTU24[] = {
    0x0D80000, 0x080000, 0x000FF0000, -16, CHKSUMD82,
};
/* asm: RTU20	.word	0D00000h,080000h,000FF0000h,-16,CHKSUMD02 */
int RTU20[] = {
    0x0D00000, 0x080000, 0x000FF0000, -16, CHKSUMD02,
};
/* asm: RTU16	.word	0C80000h,080000h,000FF0000h,-16,CHKSUMC82 */
int RTU16[] = {
    0x0C80000, 0x080000, 0x000FF0000, -16, CHKSUMC82,
};
/* asm: RTU12	.word	0C00000h,080000h,000FF0000h,-16,CHKSUMC02 */
int RTU12[] = {
    0x0C00000, 0x080000, 0x000FF0000, -16, CHKSUMC02,
};
/* asm: RTU29	.word	0E00000h,080000h,0FF000000h,-24,CHKSUME03 */
int RTU29[] = {
    0x0E00000, 0x080000, 0x0FF000000, -24, CHKSUME03,
};
/* asm: RTU25	.word	0D80000h,080000h,0FF000000h,-24,CHKSUMD83 */
int RTU25[] = {
    0x0D80000, 0x080000, 0x0FF000000, -24, CHKSUMD83,
};
/* asm: RTU21	.word	0D00000h,080000h,0FF000000h,-24,CHKSUMD03 */
int RTU21[] = {
    0x0D00000, 0x080000, 0x0FF000000, -24, CHKSUMD03,
};
/* asm: RTU17	.word	0C80000h,080000h,0FF000000h,-24,CHKSUMC83 */
int RTU17[] = {
    0x0C80000, 0x080000, 0x0FF000000, -24, CHKSUMC83,
};
/* asm: RTU13	.word	0C00000h,080000h,0FF000000h,-24,CHKSUMC03 */
int RTU13[] = {
    0x0C00000, 0x080000, 0x0FF000000, -24, CHKSUMC03,
};
/* ;WAVE RAM
 */
/* asm: RTU72	.word	0A00000h,080000h,00000000Fh,1,1000h,2 */
int RTU72[] = {
    0x0A00000, 0x080000, 0x00000000F, 1, 0x1000, 2,
};
/* asm: RTU69	.word	0A00000h,080000h,0000000F0h,1,1000h,2 */
int RTU69[] = {
    0x0A00000, 0x080000, 0x0000000F0, 1, 0x1000, 2,
};
/* asm: RTU70	.word	0A00000h,080000h,000000F00h,1,1000h,2 */
int RTU70[] = {
    0x0A00000, 0x080000, 0x000000F00, 1, 0x1000, 2,
};
/* asm: RTU71	.word	0A00000h,080000h,00000F000h,1,1000h,2 */
int RTU71[] = {
    0x0A00000, 0x080000, 0x00000F000, 1, 0x1000, 2,
};
/* asm: RTU76	.word	0A00001h,080000h,00000000Fh,1,1000h,2 */
int RTU76[] = {
    0x0A00001, 0x080000, 0x00000000F, 1, 0x1000, 2,
};
/* asm: RTU73	.word	0A00001h,080000h,0000000F0h,1,1000h,2 */
int RTU73[] = {
    0x0A00001, 0x080000, 0x0000000F0, 1, 0x1000, 2,
};
/* asm: RTU74	.word	0A00001h,080000h,000000F00h,1,1000h,2 */
int RTU74[] = {
    0x0A00001, 0x080000, 0x000000F00, 1, 0x1000, 2,
};
/* asm: RTU75	.word	0A00001h,080000h,00000F000h,1,1000h,2 */
int RTU75[] = {
    0x0A00001, 0x080000, 0x00000F000, 1, 0x1000, 2,
};
/* ;COLOR RAM
 */
/* asm: RTU85	.word	09E0000h,08000h,000FF00h,1,SOFT_WS,1 */
int RTU85[] = {
    0x09E0000, 0x08000, 0x000FF00, 1, SOFT_WS, 1,
};
/* asm: RTU87	.word	09E0000h,08000h,00000FFh,1,SOFT_WS,1 */
int RTU87[] = {
    0x09E0000, 0x08000, 0x00000FF, 1, SOFT_WS, 1,
};
/* ;VIDEO RAM
 */
/* asm: RTU102	.word	0900000h,020000h,0000000FFh,1,1000h,2 */
int RTU102[] = {
    0x0900000, 0x020000, 0x0000000FF, 1, 0x1000, 2,
};
/* asm: RTU95	.word	0900000h,020000h,00000FF00h,1,1000h,2 */
int RTU95[] = {
    0x0900000, 0x020000, 0x00000FF00, 1, 0x1000, 2,
};
/* asm: RTU94	.word	0900001h,020000h,0000000FFh,1,1000h,2 */
int RTU94[] = {
    0x0900001, 0x020000, 0x0000000FF, 1, 0x1000, 2,
};
/* asm: RTU101	.word	0900001h,020000h,00000FF00h,1,1000h,2 */
int RTU101[] = {
    0x0900001, 0x020000, 0x00000FF00, 1, 0x1000, 2,
};
/* ;FAST RAM
 */
/* asm: RTU57	.word	0400000h,020000h,00000FF00h,1,SOFT_WS,1 */
int RTU57[] = {
    0x0400000, 0x020000, 0x00000FF00, 1, SOFT_WS, 1,
};
/* asm: RTU56	.word	0400000h,020000h,0000000FFh,1,SOFT_WS,1 */
int RTU56[] = {
    0x0400000, 0x020000, 0x0000000FF, 1, SOFT_WS, 1,
};
/* asm: RTU60	.word	0400000h,020000h,0FF000000h,1,SOFT_WS,1 */
int RTU60[] = {
    0x0400000, 0x020000, 0x0FF000000, 1, SOFT_WS, 1,
};
/* asm: RTU59	.word	0400000h,020000h,000FF0000h,1,SOFT_WS,1 */
int RTU59[] = {
    0x0400000, 0x020000, 0x000FF0000, 1, SOFT_WS, 1,
};
#define ISRAM 0
#define ISROM 1
/* *----------------------------------------------------------------------------
 */
/* asm: CHIPTEST_TABLEI	.word	CHIPTEST_TABLE */
#define CHIPTEST_TABLEI CHIPTEST_TABLE
/* asm: CHIPTEST_TABLE */
/* asm: CHIPMC	RTU85,130,250,60,16,"U85",ISRAM		;COLOR RAM */
/* asm: CHIPMC	RTU87,130,270,60,16,"U87",ISRAM */
/* asm: CHIPMAC	RTU102,300,300,350,316,"U102",ISRAM	;VIDEO RAM */
/* asm: CHIPMAC	RTU95,300,280,350,296,"U95",ISRAM */
/* asm: CHIPMAC	RTU94,240,280,290,296,"U94",ISRAM */
/* asm: CHIPMAC	RTU101,240,300,290,316,"U101",ISRAM */
int CHIPTEST_TABLE;
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
int TESTING_CHIPS;
/* asm: CMOS_CHIP */
/* asm: CHIPMC	RTU31,130,120,40,18,"U31",ISROM		;CMOS CHIP */
int CMOS_CHIP;

void TEST_STATIC_CHIPS(void)
{
    // asm: 	DINT
    // asm: 	SETDP
    // asm: 	CALL	COPY_RAMTEST
    // asm: 	LDL	BASICS_RAM,AR5
    // asm: 	LDI	3,AR6
    // asm: 	LDL	STATIC_TABLE,AR4
    // asm 00006300: TSTBL1A
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
    // asm 00006300: 	PUSH	AR4
    // asm 00006306: 	CALL	RAMTEST
    // asm 00006307: 	POP	AR4
    // asm 00006308: 	POP	AR5
    // asm 00006309: 	POP	AR6
    // asm 0000630A: 	POP	AR0
    // asm: 	STI	R0,*AR5++		;SAVE RESULTS
    // asm 0000630B: 	CMPI	1,R0
    // asm 0000630C: 	BEQ	IS_STATIC_ERROR
    // asm 0000630D: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 0000630E: 	DBU	AR6,TSTBL1A
    // asm 0000630F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_STATIC_CHIPS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void GET_AR4_DIGITS(void)
{
    // asm 00006313: 	LDI	*+AR4(CTT_U),AR0
    // asm 00006314: 	LDI	*AR0,AR0
    // asm 00006315: 	LDI	AR0,R1
    // asm 00006316: 	RS	8,R1
    // asm 00006317: 	AND	0FFh,R1
    // asm 00006318: 	SUBI	'0',R1
    // asm 0000631A: 	LDI	AR0,R2
    // asm 0000631B: 	RS	16,R2
    // asm 0000631C: 	AND	0FFh,R2
    // asm: 	CMPI	0,R2
    // asm 0000631D: 	BEQ	R1R2ZER
    // asm 0000631E: 	SUBI	'0',R2
    // asm 00006320: 	LDI	AR0,R3
    // asm 00006321: 	RS	24,R3
    // asm: 	AND	0FFh,R3
    // asm: 	CMPI	0,R3
    // asm: 	BEQ	R3ZERO
    // asm: 	SUBI	'0',R3
    // asm: 	BU	NZERO
R1R2ZER:
    // asm 00006322: CLRI	R2
R3ZERO:
    // asm 00006322: CLRI	R3
NZERO:
    // asm 00006324: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_AR4_DIGITS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void BLINK_AND_CONTINUE(void)
{
    // asm 0000632A: 	PUSH	RS
    // asm 0000632B: 	PUSH	RE
    // asm 0000632C: 	PUSH	RC
    // asm 0000632D: 	PUSH	AR0
    // asm: 	PUSH	R1
    // asm 0000632E: 	PUSH	R2
    // asm 0000632F: 	PUSH	R3
    // asm 00006330: 	CALL	GET_AR4_DIGITS
    // asm 00006331: 	CALL	BLINK_DIGITS
    // asm 00006332: 	POP	R3
    // asm 00006333: 	POP	R2
    // asm 00006334: 	POP	R1
    // asm 00006335: 	POP	AR0
    // asm 00006336: 	POP	RC
    // asm: 	POP	RE
    // asm 00006337: 	POP	RS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLINK_AND_CONTINUE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void IS_STATIC_ERROR(void)
{
    // asm 00006338: 	CALL	GET_AR4_DIGITS
ELOOP:
    // asm 0000633B: 	CALL	BLINK_DIGITS
    // asm 0000633C: 	BU	ELOOP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "IS_STATIC_ERROR", 0, 0);
    UNIMPL();
}

/* *
*THIS ROUTINE TAKES OVER THE CPU, AND RETURNS
*
*PARAMETERS
*	R1 DIGIT 1
*	R2 DIGIT 2
*	R3 DIGIT 3
*
*
 */
void LLED_ON(void)
{
    // asm: 	LDI	@SYSCNTL,R0		;if the system hangs and the LED
    // asm: 	ANDN	LED_OFF,R0		;is on we were in this routine
    // asm: 	STI	R0,@SYSCNTL		;when it happened
    // asm: 	LDP	@SYSCNTLR
    // asm 00006349: 	STI	R0,@SYSCNTLR
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
    // asm 0000634C: 	PUSH	AR4
    // asm 0000634E: 	LDI	R1,AR4		;#NUMBER OF ITERATIONS
    // asm 0000634F: 	DEC	AR4
    // asm 00006350: 	CMPI	0,AR4
    // asm 00006351: 	BLT	BDL1X
    // asm 00006352: BDL1
    // asm: 	CALL	LLED_ON
    // asm 00006353: 	LDIL	8000000,RC
    // asm 00006354: 	RPTS	RC
    // asm 00006355: 	NOP
    // asm 00006356: 	CALL	LLED_OFF
    // asm 00006358: 	LDIL	8000000,RC
    // asm 00006359: 	RPTS	RC
    // asm: 	NOP
    // asm: 	DBU	AR4,BDL1
    // asm 0000635A: BDL1X
    // asm 0000635E: 	LDIL	35000000,RC
    // asm: 	RPTS	RC
    // asm 0000635F: 	NOP
    // asm 00006362: 	LDI	R2,AR4		;#NUMBER OF ITERATIONS
    // asm: 	DEC	AR4
    // asm 00006363: 	CMPI	0,AR4
    // asm: 	BLT	BDL2X
    // asm 00006364: BDL2
    // asm 00006367: 	CALL	LLED_ON
    // asm 00006368: 	LDIL	8000000,RC
    // asm 00006369: 	RPTS	RC
    // asm: 	NOP
    // asm 0000636A: 	CALL	LLED_OFF
    // asm 0000636D: 	LDIL	8000000,RC
    // asm 0000636E: 	RPTS	RC
    // asm 0000636F: 	NOP
    // asm: 	DBU	AR4,BDL2
    // asm 00006370: BDL2X
    // asm 00006373: 	LDIL	35000000,RC
    // asm 00006374: 	RPTS	RC
    // asm: 	NOP
    // asm 00006375: 	LDI	R3,AR4		;#NUMBER OF ITERATIONS
    // asm 00006376: 	DEC	AR4
    // asm 00006377: 	CMPI	0,AR4
    // asm 00006378: 	BLT	BDL3X
    // asm 00006379: BDL3	CALL	LLED_ON
    // asm 00006379: 	LDIL	8000000,RC
    // asm 0000637A: 	RPTS	RC
    // asm 0000637D: 	NOP
    // asm 0000637E: 	CALL	LLED_OFF
    // asm 0000637F: 	LDIL	8000000,RC
    // asm 00006380: 	RPTS	RC
    // asm 00006383: 	NOP
    // asm: 	DBU	AR4,BDL3
    // asm 00006385: BDL3X
    // asm: 	LDIL	35000000,RC
    // asm 00006386: 	RPTS	RC
    // asm 00006389: 	NOP
    // asm 0000638B: 	LDIL	35000000,RC
    // asm 0000638C: 	RPTS	RC
    // asm 0000638D: 	NOP
    // asm 0000638F: 	POP	AR4
    // asm 00006390: 	POP	R3
    // asm 00006393: 	POP	R2
    // asm 00006394: 	POP	R1
    // asm 00006395: 	POP	R0
    // asm 00006396: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "BLINK_DIGITS", 0, 0);
    UNIMPL();
}

void TEST_CHIPS(void)
{
    // asm: 	PUSH	DP
    // asm: 	LDP	@9E0000h
    // asm 0000639C: 	CLRI	R0			;set background to 0
    // asm 0000639F: 	STI	R0,@9E0000h
    // asm 000063A0: 	POP	DP
    // asm 000063A1: 	CALL	SETPAGE0
    // asm 000063A4: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 000063A5: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	DINT
    // asm 000063A6: 	CALL	COPY_RAMTEST
    // asm 000063A7: 	CALL	TEST_BASICS
    // asm 000063AA: 	LDI	COL_VDGREY,RC
    // asm 000063AB: 	TEXTITT	"CPU BOARD TEST",50,40
    // 		;
    // 		;NOW CHECK AND PLOT ACCORDINGLY
    // 		;
    // asm: 	LDL	TESTING_CHIPS,AR4
TESTLP1:
    // asm 000063AD: 	LDI	*+AR4(CTT_RAMROM),R0
    // asm 000063AE: 	BZ	CHECK_AS_RAM	;(W/ SCRAMBLE)
    // asm 000063B0: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm: 	LDI	*AR0++,AR2	;ADDR
    // asm: 	LDI	*AR0++,RC	;LENGTH
    // asm 000063B1: 	LDI	*AR0++,R2	;MASK
    // asm 000063B2: 	LDI	*AR0++,R3	;SHIFT
    // asm 000063B5: 	CALL	GENERATE_CHECKSUM
    // asm 000063BC: 	LDI	*AR0,AR0
    // asm 000063BD: 	LDI	*AR0,R1
    // asm: 	CMPI	R1,R0
    // asm: 	LDIEQ	COL_GREEN,RS
    // asm 000063BE: 	LDINE	COL_RED,RS
    // ;	CMPI	COL_RED,RS
    // ;	CALLEQ	BLINK_AND_CONTINUE
    // 	;CHECKSUM PART
    // asm 000063C4: 	BU	DN_CHK
CHECK_AS_RAM:
    // asm 000063C6: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm 000063C7: 	CMPI	-1,AR0
    // asm 000063C8: 	BEQ	TEST_CHIPSX
    // asm 000063CA: 	LDI	*AR0++,AR2
    // asm 000063CB: 	LDI	*AR0++,R2
    // asm: 	LDI	*AR0++,R3
    // asm 000063CC: 	LDI	*AR0++,RC
    // asm 000063CD: 	LDI	*AR0++,RS
    // asm 000063CE: 	LDI	*AR0++,BK
    // asm 000063CF: 	PUSH	AR0
    // asm 000063D0: 	PUSH	AR4
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
    // asm 000063D5: 	INC	AR2
    // asm 000063D6: 	LDI	*+AR4(CTT_MINY),R2
    // asm 000063D7: 	INC	R2
    // asm 000063D8: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 000063D9: 	DEC	R3
    // asm 000063DA: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 000063DB: 	DEC	RC
    // asm 000063DC: 	CALL	_fill
    // asm 000063DE: 	LDI	*+AR4(CTT_U),AR2
    // asm 000063DF: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 000063E0: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 000063E1: 	SUBI	*+AR4(CTT_MINX),R2
    // asm: 	SUBI	*+AR4(CTT_MINY),R3
    // asm 000063E2: 	RS	1,R2
    // asm: 	RS	1,R3
    // asm: 	ADDI	*+AR4(CTT_MINX),R2
    // asm: 	ADDI	*+AR4(CTT_MINY),R3
    // asm: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm: 	LDI	COL_BLACK,RC
    // asm 000063E3: 	CALL	_outtextxyc
    // asm 000063E5: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 000063E6: 	BNZ	TESTLP1
TEST_CHIPSX:
    // asm 000063E9: 	CALL	CMOS_CHIP_TEST
    // asm 000063EB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_CHIPS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void PLOT_OUTLINE_OF_CHIPS(void)
{
    // asm 000063F1: 	LDI	@CHIPTEST_TABLEI,AR4
POOCL:
    // asm 000063F3: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 000063F4: 	LDI	*+AR4(CTT_MINY),R2
    // asm 000063F5: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 000063F6: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 000063F7: 	LDI	COL_WHITE,RS
    // asm 000063F8: 	PUSH	DP
    // asm: 	CALL	_rectangle
    // asm 000063F9: 	POP	DP
    // asm: 	LDI	*+AR4(CTT_U),AR2
    // asm: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 000063FB: 	LDI	*+AR4(CTT_MAXY),R3
    // asm: 	SUBI	*+AR4(CTT_MINX),R2
    // asm 000063FC: 	SUBI	*+AR4(CTT_MINY),R3
    // asm: 	RS	1,R2
    // asm: 	RS	1,R3
    // asm: 	ADDI	*+AR4(CTT_MINX),R2
    // asm: 	ADDI	*+AR4(CTT_MINY),R3
    // asm: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm 000063FD: 	LDI	COL_DGREY,RC
    // asm: 	CALL	_outtextxyc
    // asm 000063FF: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 00006400: 	BNZ	POOCL
    // asm 00006402: 	CALL	CMOS_CHIP_DISPLAY
    // asm 00006403: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLOT_OUTLINE_OF_CHIPS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void CMOS_CHIP_DISPLAY(void)
{
    // asm 00006408: 	LDL	CMOS_CHIP,AR4
    // asm 00006409: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 0000640A: 	LDI	*+AR4(CTT_MINY),R2
    // asm 0000640B: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 0000640C: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 0000640D: 	LDI	COL_WHITE,RS
    // asm 0000640E: 	PUSH	DP
    // asm 0000640F: 	CALL	_rectangle
    // asm 00006410: 	POP	DP
    // asm 00006412: 	LDI	*+AR4(CTT_U),AR2
    // asm: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 00006413: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 00006414: 	SUBI	*+AR4(CTT_MINX),R2
    // asm: 	SUBI	*+AR4(CTT_MINY),R3
    // asm 00006415: 	RS	1,R2
    // asm 00006416: 	RS	1,R3
    // asm: 	ADDI	*+AR4(CTT_MINX),R2
    // asm: 	ADDI	*+AR4(CTT_MINY),R3
    // asm: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm: 	LDI	COL_DGREY,RC
    // asm 00006417: 	CALL	_outtextxyc
    // asm 00006418: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_CHIP_DISPLAY", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void CMOS_CHIP_TEST(void)
{
    // asm 0000641E: 	PUSH	AR4
    // asm 0000641F: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm 00006420: 	PUSH	R2
    // asm 00006421: 	PUSH	R3
    // asm 00006422: 	LDL	CMOS_CHIP,AR4
    // asm 00006423: 	LDIL	5A5A5A5Ah,R3
    // asm 00006425: 	LDP	@CPU_WS
    // asm 00006426: 	LDI	88h,R1
    // asm 00006427: 	STI	R1,@CPU_WS
    // asm 00006428: 	SETDP
    // asm 0000642A: 	CMOS_WP_OFF
    // asm 0000642B: 	NOP			;DELAY FOR TIMING...
    // asm 0000642D: 	LDI	5,AR5
KKLL1:
    // asm 0000642E: LDI	R3,R2
    // asm: 	AND	0FFh,R2
    // asm: 	LS	24,R2
    // asm 0000642E: 	LDI	AUD_CHIPTEST_DEDICATED,AR2
    // asm 0000642F: 	LS	2,AR2
    // asm 00006430: 	ADDI	@CMOSI,AR2
    // asm 00006433: 	STI	R2,*AR2
    // asm 00006434: 	RS	24,R2
    // asm 00006438: 	LDL	0C00000h,AR0		;DUMMY READ
    // asm 00006439: 	LDI	*AR0,R0
    // asm: 	LDI	*AR2,R0
    // asm 0000643B: 	RS	24,R0
    // asm: 	CMPI	R2,R0
    // asm 0000643D: 	BNE	CM_ISERROR
    // asm: 	LDI	R3,R1
    // asm 0000643F: 	LSH	1,R3
    // asm 00006440: 	XOR	R3,R1
    // asm: 	BNN	RND2
    // asm 00006441: 	OR	1,R3
RND2:
    // asm 00006442: MPYI	794Fh,R3
    // asm 00006443: 	DBU	AR5,KKLL1
    // asm: 	CMOS_WP_ON
    // 	;NO ERROR
    // asm: 	LDI	COL_GREEN,RS
    // asm 00006446: 	BU	KKJJ
CM_ISERROR:
    // asm 00006448: 	CMOS_WP_ON
    // asm 00006449: 	LDI	COL_RED,RS
KKJJ:
    // ;	CMPI	COL_RED,RS
    // ;	CALLEQ	BLINK_AND_CONTINUE
    // asm 0000644E: 	LDP	@CPU_WS
    // asm 0000644F: 	LDI	1048h,R0
    // asm 00006450: 	STI	R0,@CPU_WS
    // asm 00006451: 	SETDP
    // asm 00006453: 	LDI	*+AR4(CTT_MINX),AR2
    // asm: 	INC	AR2
    // asm: 	LDI	*+AR4(CTT_MINY),R2
    // asm: 	INC	R2
    // asm 00006454: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 00006455: 	DEC	R3
    // asm: 	LDI	*+AR4(CTT_MAXY),RC
    // asm: 	DEC	RC
    // asm 00006456: 	CALL	_fill
    // asm: 	LDI	*+AR4(CTT_U),AR2
    // asm: 	LDI	*+AR4(CTT_MAXX),R2
    // asm: 	LDI	*+AR4(CTT_MAXY),R3
    // asm: 	SUBI	*+AR4(CTT_MINX),R2
    // asm: 	SUBI	*+AR4(CTT_MINY),R3
    // asm: 	RS	1,R2
    // asm 00006458: 	RS	1,R3
    // asm 00006459: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 0000645A: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 0000645B: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm: 	LDI	COL_BLACK,RC
    // asm 0000645C: 	CALL	_outtextxyc
    // asm 0000645E: 	POP	R3
    // asm 0000645F: 	POP	R2
    // asm 00006460: 	POP	R1
    // asm 00006461: 	POP	R0
    // asm 00006462: 	POP	AR4
    // asm 00006463: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_CHIP_TEST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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

void TEST_BASICS(void)
{
    // asm: 	LDL	BASICS_RAM,AR5
    // asm 00006472: 	LDI	5,AR6
    // asm 00006473: 	LDL	CHIPTEST_TABLE,AR4
    // asm 00006475: TSTBL1
    // asm 00006476: 	LDI	*+AR4(CTT_TESTTAB),AR0
    // asm 00006477: 	LDI	*AR0++,AR2
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
    // asm 00006479: 	CMPI	0,R0
    // asm 0000647A: 	CALLNZ	BLINK_AND_CONTINUE
    // asm 0000647B: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 0000647C: 	DBU	AR6,TSTBL1
    // asm 00006481: 	CALL	HARD_LOAD_DIAGPAL
    // asm 00006482: 	CLRI	R0
    // asm 00006483: 	STI	R0,@COLORAM
    // asm 00006484: 	CALL	CLRSCRN			;we are NOT page flipping
    // asm 00006485: 	PUSH	DP
    // asm 00006486: 	LINE	20,50,490,50,COL_WHITE
    // asm 00006487: 	LINE	20,50,20,360,COL_WHITE
    // asm 00006488: 	LINE	490,50,490,360,COL_WHITE
    // asm 0000648A: 	LINE	20,360,80,360,COL_WHITE
    // asm 0000648B: 	LINE	80,360,80,340,COL_WHITE
    // asm: 	LINE	80,340,100,340,COL_WHITE
    // asm 0000648C: 	LINE	100,340,100,360,COL_WHITE
    // asm 0000648D: 	LINE	100,360,200,360,COL_WHITE
    // asm: 	LINE	200,360,200,340,COL_WHITE
    // asm: 	LINE	200,340,220,340,COL_WHITE
    // asm 0000648E: 	LINE	220,340,220,360,COL_WHITE
    // asm 0000648F: 	LINE	220,360,490,360,COL_WHITE
    // asm: 	POP	DP
    // asm: 	CALL	PLOT_OUTLINE_OF_CHIPS
    // asm 00006494: 	LDI	COL_VDGREY,RC
    // asm 00006495: 	TEXTIT	MEMTST1,50,20
    // asm 0000649B: 	TEXTIT	DATE_STAMP,50,30
    // asm 000064A1: 	TEXTIT	VERSION_STAMP,270,30
    // asm 000064AD: 	LDL	BASICS_RAM,AR5
    // asm 000064B3: 	LDI	9,AR6
    // asm 000064B9: 	LDL	CHIPTEST_TABLE,AR4
    // asm 000064C5: TSTBL2
    // asm 000064CB: 	LDI	*AR5++,R0		;SAVE RESULTS
    // asm 000064D1: 	LDIZ	COL_GREEN,RS
    // asm 000064D7: 	LDINZ	COL_RED,RS
    // asm 000064DE: 	LDI	*+AR4(CTT_MINX),AR2
    // asm: 	INC	AR2
    // asm: 	LDI	*+AR4(CTT_MINY),R2
    // asm: 	INC	R2
    // asm: 	LDI	*+AR4(CTT_MAXX),R3
    // asm: 	DEC	R3
    // asm 000064DF: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 000064E0: 	DEC	RC
    // asm 000064E4: 	CALL	_fill
    // asm: 	LDI	*+AR4(CTT_U),AR2
    // asm: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 000064EC: 	LDI	*+AR4(CTT_MAXY),R3
    // asm 000064ED: 	SUBI	*+AR4(CTT_MINX),R2
    // asm 000064EE: 	SUBI	*+AR4(CTT_MINY),R3
    // asm: 	RS	1,R2
    // asm: 	RS	1,R3
    // asm 000064EF: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 000064F0: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 000064F1: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm: 	LDI	COL_BLACK,RC
    // asm 000064F2: 	CALL	_outtextxyc
    // asm 000064F4: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 000064F5: 	DBU	AR6,TSTBL2
    // 	;Static *MUST* be working to get this far,
    // 	;so mark them ALL as cool
    // 	;
    // asm: 	LDI	3,AR6
    // asm 000064FB: 	LDL	STATIC_TABLE,AR4
    // asm 000064FC: TSTBL2C
    // asm 000064FD: 	LDI	COL_GREEN,RS
    // asm 000064FE: 	LDI	*+AR4(CTT_MINX),AR2
    // asm 000064FF: 	INC	AR2
    // asm 00006500: 	LDI	*+AR4(CTT_MINY),R2
    // asm 00006501: 	INC	R2
    // asm 00006502: 	LDI	*+AR4(CTT_MAXX),R3
    // asm 00006503: 	DEC	R3
    // asm 00006504: 	LDI	*+AR4(CTT_MAXY),RC
    // asm 00006505: 	DEC	RC
    // asm 00006506: 	CALL	_fill
    // asm: 	LDI	*+AR4(CTT_U),AR2
    // asm 00006508: 	LDI	*+AR4(CTT_MAXX),R2
    // asm 00006509: 	LDI	*+AR4(CTT_MAXY),R3
    // asm: 	SUBI	*+AR4(CTT_MINX),R2
    // asm: 	SUBI	*+AR4(CTT_MINY),R3
    // asm: 	RS	1,R2
    // asm: 	RS	1,R3
    // asm: 	ADDI	*+AR4(CTT_MINX),R2
    // asm 0000650A: 	ADDI	*+AR4(CTT_MINY),R3
    // asm 0000650B: 	SUBI	15,R2
    // asm: 	SUBI	4,R3
    // asm 0000650C: 	LDI	COL_BLACK,RC
    // asm 0000650D: 	CALL	_outtextxyc
    // asm 0000650E: 	LDI	*++AR4(CTT_SIZE),R0
    // asm 0000650F: 	DBU	AR6,TSTBL2C
    // asm 00006511: 	CALL	CMOS_CHIP_TEST
    // asm 00006512: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TEST_BASICS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void RAMTEST(void)
{
    // asm: 	LDL	BLOWLIST,R0
    // asm: 	BU	R0
    // 		;THIS IS THE ACTUAL ROUTINE, BUT IT MUST
    // 		;BE PLACED IN ON-CHIP MEMORY
    // 		;
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
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
    // asm 00006527: 	LDI	@RANDI,R4	;SETUP R4=RANDOM # REGISTER
    // asm: 	LDI	RC,AR0		;NUMBER OF PASSES
    // asm: 	DEC	AR0
    // asm: 	DEC	R2		;LENGTH OF CHECK
    // asm: 	LDI	R2,AR4
    // asm 00006529: RAMTST0
    // asm 0000652B: 	LDI	AR2,AR1		;GET ADDRESS
    // asm: 	LDI	R4,R0		;GET RANDOM # STARTING POINT
    // *WRITE RANDOM NUMBERS TO RAM
    // asm 00006536: RAMTST1
    // asm 00006536: 	LDI	R0,R1
    // asm 00006537: 	LSH	1,R0
    // asm 00006538: 	LDI	0,R6
    // asm 00006539: 	XOR	R0,R1
    // asm 0000653B: 	LDIN	2,R6		;	BNN	RAMTST2
    // asm 0000653C: 	OR	R6,R0		;	OR	2,R0
    // 				;RAMTST2
    // asm: 	DBUD	AR4,RAMTST1
    // asm 0000653F: 	STI	R0,*AR1
    // asm: 	ADDI	BK,AR1
LKKK:
    // asm 00006540: LDI	*AR3,R5
    // *READ RANDOM NUMBERS FROM RAM
    // asm 00006542: 	LDI	AR2,AR1		;GET ADDRESS
    // asm 00006543: 	LDI	R4,R0		;GET RANDOM # STARTING POINT
    // asm: 	LDI	R0,R1
    // asm: 	LSH	1,R0
    // asm 00006545: 	CLRI	R6
    // asm 00006546: 	CLRI	R7
    // asm 00006548: 	LDI	R2,AR4		;LENGTH
    // asm 00006549: 	XOR	R0,R1
    // asm: 	LDIN	2,R6		;	BNN	RAMTST4
    // asm 0000654B: RAMTST3
    // asm 0000654B: 	OR	R6,R0		;	OR	2,R0
    // 				;RAMTST4
    // asm 0000654D: 	INC	R7
    // asm: 	LDI	*AR1,R1
    // asm: 	ADDI	BK,AR1
    // asm: 	LDI	*AR3,R5		;DUMMY READ
    // asm 0000654F: 	AND	R3,R1
    // asm: 	LDI	R0,R4
    // asm 00006550: 	AND	R3,R0
    // asm 00006551: 	CMPI	R1,R0
    // asm: 	BNE	RAMERR
    // asm 00006553: 	LDI	R4,R0
    // asm: 	LDI	R0,R1		;mirrored from above
    // asm: 	LSH	1,R0
    // 	;---->BNE	RAMERR
    // asm 00006556: 	DBUD	AR4,RAMTST3
JJJH:
    // asm 00006557: CLRI	R6
    // asm: 	XOR	R0,R1
    // asm 00006557: 	LDIN	2,R6		;	BNN	RAMTST4
    // 	;---->	DBUD	AR4,RAMTST3
    // asm 00006559: RAMRPT
    // asm 00006559: 	DBU	AR0,RAMTST0
    // asm: 	CLRI	R0		;IS GOOD RAM
ENDING:
    // asm 0000655D: 	POP	R1		;FIND WAIT STATES
    // asm 0000655E: 	LDP	@CPU_WS		;set original wait states
    // asm 0000655F: 	STI	R1,@CPU_WS
    // asm 00006560: 	POP	AR6
    // asm: 	POP	R6
    // asm 00006561: 	POP	DP
    // asm 00006562: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMTEST_REAL", 0, 0);
    UNIMPL();
}

void RAMERR(void)
{
    // asm: 	LDI	1,R0		;IS BAD RAM
    // asm 00006564: 	BU	ENDING
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RAMERR", 0, 0);
    UNIMPL();
}

void RAMTEST_END(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void COPY_RAMTEST(void)
{
    // asm: 	LDL	RAMTEST_END,AR0
    // asm 00006569: 	LDL	RAMTEST_REAL,AR1
    // asm: 	SUBI	AR1,AR0			;GET LENGTH
    // asm 0000656A: 	DEC	AR0
    // asm 0000656C: 	LDL	BLOWLIST,AR2
    // asm 0000656E: 	LDI	AR0,RC
    // asm 0000656F: 	RPTB	BBCP
    // asm 00006570: 	LDI	*AR1++,R0
    // asm 00006571: BBCP	STI	R0,*AR2++
    // asm 00006571: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COPY_RAMTEST", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
void GENERATE_CHECKSUM(void)
{
    // asm: 	PUSH	R1
    // asm 0000657C: 	PUSH	RC
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

#if CHECKSUM_GEN
/* asm: CHKSUMC00	.word	-1 */
int CHKSUMC00 = -1;
/* asm: CHKSUMC80	.word	-1 */
int CHKSUMC80 = -1;
/* asm: CHKSUMD00	.word	-1 */
int CHKSUMD00 = -1;
/* asm: CHKSUMD80	.word	-1 */
int CHKSUMD80 = -1;
/* asm: CHKSUME00	.word	-1 */
int CHKSUME00 = -1;
/* asm: CHKSUMC01	.word	-1 */
int CHKSUMC01 = -1;
/* asm: CHKSUMC81	.word	-1 */
int CHKSUMC81 = -1;
/* asm: CHKSUMD01	.word	-1 */
int CHKSUMD01 = -1;
/* asm: CHKSUMD81	.word	-1 */
int CHKSUMD81 = -1;
/* asm: CHKSUME01	.word	-1 */
int CHKSUME01 = -1;
/* asm: CHKSUMC02	.word	-1 */
int CHKSUMC02 = -1;
/* asm: CHKSUMC82	.word	-1 */
int CHKSUMC82 = -1;
/* asm: CHKSUMD02	.word	-1 */
int CHKSUMD02 = -1;
/* asm: CHKSUMD82	.word	-1 */
int CHKSUMD82 = -1;
/* asm: CHKSUME02	.word	-1 */
int CHKSUME02 = -1;
/* asm: CHKSUMC03	.word	-1 */
int CHKSUMC03 = -1;
/* asm: CHKSUMC83	.word	-1 */
int CHKSUMC83 = -1;
/* asm: CHKSUMD03	.word	-1 */
int CHKSUMD03 = -1;
/* asm: CHKSUMD83	.word	-1 */
int CHKSUMD83 = -1;
/* asm: CHKSUME03	.word	-1 */
int CHKSUME03 = -1;
/* asm: CCHKSUMC00	.word	0 */
int CCHKSUMC00 = 0;
/* asm: CCHKSUMC80	.word	0 */
int CCHKSUMC80 = 0;
/* asm: CCHKSUMD00	.word	0 */
int CCHKSUMD00 = 0;
/* asm: CCHKSUMD80	.word	0 */
int CCHKSUMD80 = 0;
/* asm: CCHKSUME00	.word	0 */
int CCHKSUME00 = 0;
/* asm: CCHKSUMC01	.word	0 */
int CCHKSUMC01 = 0;
/* asm: CCHKSUMC81	.word	0 */
int CCHKSUMC81 = 0;
/* asm: CCHKSUMD01	.word	0 */
int CCHKSUMD01 = 0;
/* asm: CCHKSUMD81	.word	0 */
int CCHKSUMD81 = 0;
/* asm: CCHKSUME01	.word	0 */
int CCHKSUME01 = 0;
/* asm: CCHKSUMC02	.word	0 */
int CCHKSUMC02 = 0;
/* asm: CCHKSUMC82	.word	0 */
int CCHKSUMC82 = 0;
/* asm: CCHKSUMD02	.word	0 */
int CCHKSUMD02 = 0;
/* asm: CCHKSUMD82	.word	0 */
int CCHKSUMD82 = 0;
/* asm: CCHKSUME02	.word	0 */
int CCHKSUME02 = 0;
/* asm: CCHKSUMC03	.word	0 */
int CCHKSUMC03 = 0;
/* asm: CCHKSUMC83	.word	0 */
int CCHKSUMC83 = 0;
/* asm: CCHKSUMD03	.word	0 */
int CCHKSUMD03 = 0;
/* asm: CCHKSUMD83	.word	0 */
int CCHKSUMD83 = 0;
/* asm: CCHKSUME03	.word	0 */
int CCHKSUME03 = 0;
#else
/* asm: CHKSUMC00	.word	CHECKSUM_C00 */
int CHKSUMC00 = CHECKSUM_C00;
/* asm: CHKSUMC80	.word	CHECKSUM_C80 */
int CHKSUMC80 = CHECKSUM_C80;
/* asm: CHKSUMD00	.word	CHECKSUM_D00 */
int CHKSUMD00 = CHECKSUM_D00;
/* asm: CHKSUMD80	.word	CHECKSUM_D80 */
int CHKSUMD80 = CHECKSUM_D80;
/* asm: CHKSUME00	.word	CHECKSUM_E00 */
int CHKSUME00 = CHECKSUM_E00;
/* asm: CHKSUMC01	.word	CHECKSUM_C01 */
int CHKSUMC01 = CHECKSUM_C01;
/* asm: CHKSUMC81	.word	CHECKSUM_C81 */
int CHKSUMC81 = CHECKSUM_C81;
/* asm: CHKSUMD01	.word	CHECKSUM_D01 */
int CHKSUMD01 = CHECKSUM_D01;
/* asm: CHKSUMD81	.word	CHECKSUM_D81 */
int CHKSUMD81 = CHECKSUM_D81;
/* asm: CHKSUME01	.word	CHECKSUM_E01 */
int CHKSUME01 = CHECKSUM_E01;
/* asm: CHKSUMC02	.word	CHECKSUM_C02 */
int CHKSUMC02 = CHECKSUM_C02;
/* asm: CHKSUMC82	.word	CHECKSUM_C82 */
int CHKSUMC82 = CHECKSUM_C82;
/* asm: CHKSUMD02	.word	CHECKSUM_D02 */
int CHKSUMD02 = CHECKSUM_D02;
/* asm: CHKSUMD82	.word	CHECKSUM_D82 */
int CHKSUMD82 = CHECKSUM_D82;
/* asm: CHKSUME02	.word	CHECKSUM_E02 */
int CHKSUME02 = CHECKSUM_E02;
/* asm: CHKSUMC03	.word	CHECKSUM_C03 */
int CHKSUMC03 = CHECKSUM_C03;
/* asm: CHKSUMC83	.word	CHECKSUM_C83 */
int CHKSUMC83 = CHECKSUM_C83;
/* asm: CHKSUMD03	.word	CHECKSUM_D03 */
int CHKSUMD03 = CHECKSUM_D03;
/* asm: CHKSUMD83	.word	CHECKSUM_D83 */
int CHKSUMD83 = CHECKSUM_D83;
/* asm: CHKSUME03	.word	CHECKSUM_E03 */
int CHKSUME03 = CHECKSUM_E03;
/* asm: CCHKSUMC00	.word	~CHECKSUM_C00 */
int CCHKSUMC00 = ~CHECKSUM_C00;
/* asm: CCHKSUMC80	.word	~CHECKSUM_C80 */
int CCHKSUMC80 = ~CHECKSUM_C80;
/* asm: CCHKSUMD00	.word	~CHECKSUM_D00 */
int CCHKSUMD00 = ~CHECKSUM_D00;
/* asm: CCHKSUMD80	.word	~CHECKSUM_D80 */
int CCHKSUMD80 = ~CHECKSUM_D80;
/* asm: CCHKSUME00	.word	~CHECKSUM_E00 */
int CCHKSUME00 = ~CHECKSUM_E00;
/* asm: CCHKSUMC01	.word	~CHECKSUM_C01 */
int CCHKSUMC01 = ~CHECKSUM_C01;
/* asm: CCHKSUMC81	.word	~CHECKSUM_C81 */
int CCHKSUMC81 = ~CHECKSUM_C81;
/* asm: CCHKSUMD01	.word	~CHECKSUM_D01 */
int CCHKSUMD01 = ~CHECKSUM_D01;
/* asm: CCHKSUMD81	.word	~CHECKSUM_D81 */
int CCHKSUMD81 = ~CHECKSUM_D81;
/* asm: CCHKSUME01	.word	~CHECKSUM_E01 */
int CCHKSUME01 = ~CHECKSUM_E01;
/* asm: CCHKSUMC02	.word	~CHECKSUM_C02 */
int CCHKSUMC02 = ~CHECKSUM_C02;
/* asm: CCHKSUMC82	.word	~CHECKSUM_C82 */
int CCHKSUMC82 = ~CHECKSUM_C82;
/* asm: CCHKSUMD02	.word	~CHECKSUM_D02 */
int CCHKSUMD02 = ~CHECKSUM_D02;
/* asm: CCHKSUMD82	.word	~CHECKSUM_D82 */
int CCHKSUMD82 = ~CHECKSUM_D82;
/* asm: CCHKSUME02	.word	~CHECKSUM_E02 */
int CCHKSUME02 = ~CHECKSUM_E02;
/* asm: CCHKSUMC03	.word	~CHECKSUM_C03 */
int CCHKSUMC03 = ~CHECKSUM_C03;
/* asm: CCHKSUMC83	.word	~CHECKSUM_C83 */
int CCHKSUMC83 = ~CHECKSUM_C83;
/* asm: CCHKSUMD03	.word	~CHECKSUM_D03 */
int CCHKSUMD03 = ~CHECKSUM_D03;
/* asm: CCHKSUMD83	.word	~CHECKSUM_D83 */
int CCHKSUMD83 = ~CHECKSUM_D83;
/* asm: CCHKSUME03	.word	~CHECKSUM_E03 */
int CCHKSUME03 = ~CHECKSUM_E03;
#endif

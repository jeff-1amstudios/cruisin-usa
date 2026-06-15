#include "../core/cpu.h"
#include "../core/machine.h"
#include "sys.h"
#include "vunit.h"
#include "c30.h"
#include "cmos.h"
#include "macs.h"
#include "globals.h"

/*
 * Source module: asm/CMOS.ASM
 */

void CMOS_ON_C(void);
void CMOS_OFF_C(void);
void CMOS_WPON_C(void);
void CMOS_WPOFF_C(void);
void VALIDATE_CMOS(void);
void RESET_BOOKKEEPING(void);
void VERIFY_ADJUSTMENTS_ACCURACY(void);
void RESET_ADJUSTMENTS(void);
void CHECKSUMGEN_ADJ(void);
void AUDIT_INC(void);
void AUDIT_ADD(void);
void ADJUSTMENT_READ(void);
void AUDIT_READ(void);
void ADJUSTMENT_WRITE(void);
void AUDIT_WRITE(void);
void AUDIT_WRITE_ADJ(void);
void _rd_cw(void);
void _wr_cw(void);
void _rd_cwR(void);
void _wr_cwR(void);
void DEFAULT_TABLE(void);
void DEFAULT_TABLE_TOTAL(void);
void INIT_LASTHS_TABLE(void);
void UPDATE_LASTHS(void);
void CHECK_LASTHS(void);
void INIT_HSTD_TABLES(void);
void VALIDATE_HSTD_TABLES(void);
void GET_TABLE_ADDR(void);
void TABLE_ENTRY_WRITE(void);
void TABLE_ENTRY_WRITE0(void);
void TABLE_ENTRY_READ(void);
void CHECK_RACE_TABLE(void);
void INSERT_HERE(void);
void INSERT_TABLE_ENTRY(void);

/* *----------------------------------------------------------------------------
 */
/* *----------------------------------------------------------------------------
 */
#define DEFAULT_TABLE_SIZE 2
#define NUM_TABLES 14

/* *----------------------------------------------------------------------------
 */
void CMOS_ON_C(void)
{
    // asm 0000996A: 	PUSH	DP
    // asm 0000996B: 	LDP	@CPU_WS
    // asm 0000996C: 	LDI	CMOS_WS,R1
    // asm 0000996D: 	STI	R1,@CPU_WS
    // asm 0000996E: 	POP	DP
    // asm 0000996F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_ON_C", 0, 0);
    UNIMPL();
}

void CMOS_OFF_C(void)
{
    // asm 00009970: 	PUSH	DP
    // asm 00009971: 	LDP	@CPU_WS
    // asm 00009972: 	LDI	SOFT_WS,R1
    // asm 00009973: 	STI	R1,@CPU_WS
    // asm 00009974: 	POP	DP
    // asm 00009975: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_OFF_C", 0, 0);
    UNIMPL();
}

void CMOS_WPON_C(void)
{
    // asm 00009976: 	PUSH	DP
    // asm 00009977: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm 00009978: 	LDI	@CMOS_WP_WORD_SHADOW,R1
    // asm 00009979: 	AND	0F00h,R1
    // asm 0000997A: 	LDP	@CMOS_WP_WORD
    // asm 0000997B: 	STI	R1,@CMOS_WP_WORD
    // asm 0000997C: 	POP	DP
    // asm 0000997D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_WPON_C", 0, 0);
    UNIMPL();
}

void CMOS_WPOFF_C(void)
{
    // asm 0000997E: 	PUSH	DP
    // asm 0000997F: 	LDP	@CMOS_WP_WORD_SHADOW
    // asm 00009980: 	LDI	@CMOS_WP_WORD_SHADOW,R1
    // asm 00009981: 	AND	0F00h,R1
    // asm 00009982: 	LDP	@CMOS_WP_WORD
    // asm 00009983: 	OR	CMOS_WP,R1
    // asm 00009984: 	STI	R1,@CMOS_WP_WORD
    // asm 00009985: 	POP	DP
    // asm 00009986: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CMOS_WPOFF_C", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*VALIDATE_CMOS
*
*THIS ROUTINE VERIFIES THAT THE CMOS WE ARE USING
*IS CORRECT.  IF NOT, THE VALUES ARE RESET TO THE DEFAULTS.
*CALL ON STARTUP
*
*
 */
void VALIDATE_CMOS(void)
{
    // asm 00009987: 	READAUD	AUD_PCREDITS
    // asm 00009989: 	LDI	R0,R2
    // asm 0000998A: 	CMPI	10,R0
    // asm 0000998B: 	LDIGT	10,R2
    // asm 0000998C: 	CMPI	0,R0
    // asm 0000998D: 	LDILT	0,R2
    // asm 0000998E: 	SETAUD	AUD_PCREDITS
    // asm 00009990: 	READAUD	AUD_CREDITS
    // asm 00009992: 	LDI	R0,R2
    // asm 00009993: 	CMPI	30,R0
    // asm 00009994: 	LDIGT	30,R2
    // asm 00009995: 	CMPI	0,R0
    // asm 00009996: 	LDILT	0,R2
    // asm 00009997: 	SETAUD	AUD_CREDITS
    // asm 00009999: 	CALL	VALIDATE_HSTD_TABLES
    // asm 0000999A: 	CALL	VERIFY_ADJUSTMENTS_ACCURACY
    // asm 0000999B: 	CLRC
    // asm 0000999C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VALIDATE_CMOS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*RESET_BOOKKEEPING
*
*	RESET THE BOOKKEEPING CMOS PAGE (GENERATE THE NEW CHECKSUM)
*
 */
void RESET_BOOKKEEPING(void)
{
    // asm 0000999D: 	PUSH	R0
    // asm 0000999E: 	PUSH	R2
    // asm 0000999F: 	PUSH	AR2
    // asm 000099A0: 	PUSH	AR4
    // 	;erase AUDITS
    // asm 000099A1: 	LDI	(NUM_AUDITS-MAX_ADJUSTMENTS)-1,AR4
    // asm 000099A2: 	LDI	@CMOSI,AR2
    // asm 000099A3: 	ADDI	(MAX_ADJUSTMENTS+1)<<2,AR2
    // asm 000099A4: 	CLRI	R2
RBLP:
    // asm 000099A5: CALL	_wr_cw
    // asm 000099A6: 	DBU	AR4,RBLP
    // asm 000099A7: 	POP	AR4
    // asm 000099A8: 	POP	AR2
    // asm 000099A9: 	POP	R2
    // asm 000099AA: 	POP	R0
    // asm 000099AB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_BOOKKEEPING", 0, 0);
    UNIMPL();
}

/* *
 */
void VERIFY_ADJUSTMENTS_ACCURACY(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	AR2
    // asm 000099AC: 	PUSH	AR5
    // asm 000099AD: 	PUSH	AR6
    // asm 000099AE: 	CLRI	AR5
    // asm 000099AF: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR6
VAALP:
    // asm 000099B1: 	LDI	AR5,AR2
    // asm 000099B2: 	CALL	AUDIT_READ	;R0 = ADJUSTMENT VALUE
    // asm 000099B4: 	LDI	*AR6++,R1
    // asm 000099B5: 	LDI	*AR6++,R2
    // asm: 	LDI	*AR6++,R3
    // asm 000099B7: 	CMPI	R1,R0
    // asm: 	BLT	DORST
    // asm 000099B8: 	CMPI	R2,R0
    // asm 000099B9: 	BLE	NORST
DORST:
    // asm 000099BB: LDI	R3,R2
    // asm 000099BB: 	LDI	AR5,AR2
    // asm 000099BC: 	CALL	ADJUSTMENT_WRITE
NORST:
    // asm 000099BE: INC	AR5
    // asm: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm 000099BF: 	BLT	VAALP
    // asm 000099C1: 	POP	AR6
    // asm: 	POP	AR5
    // asm 000099C2: 	POP	AR2
    // asm 000099C3: 	POP	R4
    // asm 000099C4: 	POP	R3
    // asm: 	POP	R2
    // asm 000099C5: 	POP	R1
    // asm 000099C6: 	POP	R0
    // asm 000099C7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VERIFY_ADJUSTMENTS_ACCURACY", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RESET_ADJUSTMENTS(void)
{
    // asm 000099CD: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm 000099CE: 	PUSH	AR6
    // asm 000099D0: 	CLRI	AR5
    // asm 000099D1: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR6
VAALP2:
    // asm 000099D3: 	LDI	*AR6++,R2
    // asm 000099D4: 	LDI	*AR6++,R2
    // asm: 	LDI	*AR6++,R2		;RESET VALUE
    // asm 000099D5: 	LDI	AR5,AR2
    // asm 000099D6: 	CALL	AUDIT_WRITE
    // asm 000099D7: 	INC	AR5
    // asm 000099D8: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm 000099D9: 	BLT	VAALP2
    // asm: 	CALL	GETCOIN_DEFAULT
    // ;	LDP	@DIPSW
    // ;	LDI	@DIPSW,R2
    // ;	SETDP
    // ;	RS	24,R2
    // ;	NOT	R2
    // ;	LDI	0,R0
    // ;	LDI	7,RC
    // ;	RPTB	INVBLP
    // ;	LSH	-1,R2
    // ;INVBLP	ROLC	R0
    // ;	AND	07Fh,R0
    // ;	LDI	R0,R2
    // ;Now check to see if the dipswitch setting is a valid coinmode
    // asm: 	LDL	VERIFY_ADJUSTMENTS_ACCURACYTAB,AR2
    // asm: 	LDI	ADJ_COINMODE,R0
    // asm: 	MPYI	3,R0
    // asm: 	ADDI	R0,AR2
    // asm: 	LDI	*+AR2,R0
    // asm: 	CMPI	R0,R2
    // asm 000099E0: 	LDIGT	*+AR2(2),R2
    // asm 000099E2: 	LDI	ADJ_COINMODE,AR2
    // asm 000099E3: 	CALL	ADJUSTMENT_WRITE
    // asm 000099E6: 	CALL	CHECKSUMGEN_ADJ
    // asm: 	LDI	R0,R2
    // asm 000099E7: 	SETAUD	ADJ_CHECKSUM
    // asm 000099E8: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR2
    // asm 000099E9: 	POP	R3
    // asm 000099EA: 	POP	R2
    // asm 000099EB: 	POP	R1
    // asm 000099ED: 	POP	R0
    // asm 000099EE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_ADJUSTMENTS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*GENERATE CHECKSUM FOR THE ADJUSTMENTS
*
*RETURNS
*	R0	ADJUSTMENT CHECKSUM
*
 */
void CHECKSUMGEN_ADJ(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR5
    // asm: 	CLRI	AR5
    // asm: 	CLRI	R1
VAALP3:
    // asm 000099F6: 	LDI	AR5,AR2
    // asm 000099F7: 	CALL	AUDIT_READ	;R0 = ADJUSTMENT VALUE
    // asm: 	ADDI	R0,R1
    // asm 000099F9: 	INC	AR5
    // asm: 	CMPI	NUM_ADJUSTMENTS,AR5
    // asm 000099FA: 	BLT	VAALP3
    // asm 000099FC: 	LDI	R1,R0
    // asm: 	POP	AR5
    // asm 000099FD: 	POP	AR2
    // asm 000099FE: 	POP	R1
    // asm 000099FF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECKSUMGEN_ADJ", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*void	audit_inc(int index)
*
*PARAMETERS
*	AR2	INDEX
*RETURNS
*	R0	AUDIT VALUE
*
 */
void AUDIT_INC(void)
{
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CALL	_rd_cw
    // asm: 	NOP	*AR2--(4)
    // asm: 	ADDI	1,R0
    // asm: 	LDI	R0,R2
    // asm 00009A05: 	CALL	_wr_cw
    // asm 00009A06: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_INC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*void	audit_add(int index)
*
*PARAMETERS
*	AR2	INDEX
*	R2	VALUE
*RETURNS
*	R0	AUDIT VALUE
*
 */
void AUDIT_ADD(void)
{
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CALL	_rd_cw
    // asm: 	NOP	*AR2--(4)
    // asm: 	ADDI	R0,R2
    // asm: 	CALL	_wr_cw
    // asm 00009A0D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_ADD", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*AUDIT_READ
*
*PARAMETERS
*	AR2	INDEX
*RETURNS
*	R0	AUDIT VALUE
*
 */
void ADJUSTMENT_READ(void)
{
    /* no executable asm lines detected */
    UNIMPL();
}

void AUDIT_READ(void)
{
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CALL	_rd_cw
    // asm: 	NOP	*AR2--(4)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_READ", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
*PARAMETERS
*	AR2	INDEX
*	R2	VALUE
*RETURNS
*	R0	AUDIT VALUE
 */
void ADJUSTMENT_WRITE(void)
{
    // asm: 	CALL	AUDIT_WRITE_ADJ
    // asm: 	CALL	CHECKSUMGEN_ADJ
    // asm: 	LDI	R0,R2
    // asm: 	LDI	ADJ_CHECKSUM,AR2
    // asm: 	CALL	AUDIT_WRITE_ADJ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADJUSTMENT_WRITE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*AUDIT_WRITE
*
*PARAMETERS
*	AR2	INDEX
*	R2	VALUE
*RETURNS
*	R0	AUDIT VALUE
*
 */
void AUDIT_WRITE(void)
{
#if DEBUG
    // asm: 	CMPI	NUM_ADJUSTMENTS,AR2
    // asm: 	BLT	$	;AUDIT BEING USED TO WRITE ADJUSTMENT
#endif
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_WRITE", 0, 0);
    UNIMPL();
}

void AUDIT_WRITE_ADJ(void)
{
    // asm: 	LS	2,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CALL	_wr_cw
    // asm: 	NOP	*AR2--(4)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "AUDIT_WRITE_ADJ", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*int	rd_cw(int addr);
*READ CMOS WORD
*
*PARAMETERS
*	AR2	ADDR
*RETURNS
*	R0	WORD
*	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
 */
void _rd_cw(void)
{
    // asm: 	PUSH	R1
    // asm: 	CMOS_ON
    // asm: 	NOP			;DELAY FOR TIMING...
    // asm: 	LDI	*AR2++,R0
    // asm: 	RS	24,R0
    // asm 00009A24: 	LS	8,R0
    // asm 00009A26: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm 00009A27: 	OR	R1,R0
    // asm 00009A28: 	LS	8,R0
    // asm: 	LDI	*AR2++,R1
    // asm 00009A2A: 	RS	24,R1
    // asm 00009A2B: 	OR	R1,R0
    // asm 00009A2C: 	LS	8,R0
    // asm: 	LDI	*AR2++,R1
    // asm 00009A2E: 	RS	24,R1
    // asm 00009A2F: 	OR	R1,R0
    // asm 00009A31: 	CMOS_OFF
    // asm: 	POP	R1
    // asm 00009A32: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rd_cw", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*void	wr_cw(int addr, int value);
*WRITE CMOS WORD
*
*PARAMETERS
*	AR2	LOC
*	R2	WORD
*RETURNS
*	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
*
 */
void _wr_cw(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	CMOS_ON
    // asm: 	CMOS_WP_OFF
    // asm: 	NOP			;DELAY FOR TIMING...
    // asm 00009A39: 	STI	R2,*AR2++
    // asm 00009A3A: 	LS	8,R2
    // asm 00009A3B: 	STI	R2,*AR2++
    // asm 00009A3C: 	LS	8,R2
    // asm 00009A3D: 	STI	R2,*AR2++
    // asm: 	LS	8,R2
    // asm 00009A3E: 	STI	R2,*AR2++
    // asm 00009A3F: 	CMOS_WP_ON
    // asm 00009A40: 	CMOS_OFF
    // asm 00009A41: 	POP	R3
    // asm 00009A42: 	POP	R2
    // asm 00009A43: 	POP	R1
    // asm 00009A44: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_wr_cw", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*int	rd_cmR(int addr);
*READ CMOS WORD REVERSED  (MOTOROLA STYLE)
*
*PARAMETERS
*	AR2	LOC
*RETURNS
*	R0	WORD
*	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
*
 */
void _rd_cwR(void)
{
    // asm: 	PUSH	R1
    // asm: 	CMOS_ON
    // asm: 	NOP			;DELAY FOR TIMING...
    // asm: 	LDI	*AR2++,R0
    // asm: 	RS	24,R0
    // asm 00009A4C: 	LDI	*AR2++,R1
    // asm 00009A4D: 	RS	24,R1
    // asm: 	LS	8,R1
    // asm 00009A4E: 	OR	R1,R0
    // asm: 	LDI	*AR2++,R1
    // asm 00009A50: 	RS	24,R1
    // asm 00009A51: 	LS	16,R1
    // asm 00009A52: 	OR	R1,R0
    // asm: 	LDI	*AR2++,R1
    // asm 00009A54: 	RS	24,R1
    // asm 00009A55: 	LS	24,R1
    // asm 00009A56: 	OR	R1,R0
    // asm: 	CMOS_OFF
    // asm 00009A58: 	POP	R1
    // asm 00009A59: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rd_cwR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*void	wr_cmR(int addr, int value);
*WRITE CMOS WORD REVERSED
*
*PARAMETERS
*	AR2	LOC
*	R2	WORD
*RETURNS
*	AR2	INCREMENTED TO NEXT ENTRY IN CMOS
*
 */
void _wr_cwR(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	LDI	R2,R3
    // asm: 	LS	24,R3
    // asm: 	CMOS_ON
    // asm 00009A5F: 	CMOS_WP_OFF
    // asm 00009A61: 	NOP			;DELAY FOR TIMING...
    // asm 00009A63: 	STI	R3,*AR2++
    // asm 00009A64: 	LDI	R2,R3
    // asm 00009A65: 	RS	8,R3
    // asm: 	LS	24,R3
    // asm 00009A66: 	STI	R3,*AR2++
    // asm 00009A67: 	LDI	R2,R3
    // asm 00009A68: 	RS	16,R3
    // asm 00009A69: 	LS	24,R3
    // asm 00009A6A: 	STI	R3,*AR2++
    // asm: 	RS	24,R2
    // asm 00009A6C: 	LS	24,R3
    // asm 00009A6D: 	STI	R2,*AR2++
    // asm 00009A6E: 	CMOS_WP_ON
    // asm 00009A6F: 	CMOS_OFF
    // asm: 	POP	R3
    // asm 00009A70: 	POP	R2
    // asm 00009A71: 	POP	R1
    // asm 00009A72: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_wr_cwR", 0, 0);
    UNIMPL();
}

void DEFAULT_TABLE(void)
{
    // asm: 	TABLEENT	'T','V','G',2,01,3
    // asm: 	TABLEENT	'E','L','P',2,02,3
    // asm: 	TABLEENT	'E','P','J',2,03,3
    // asm: 	TABLEENT	'M','L',' ',2,04,3
    // asm: 	TABLEENT	'G','W','S',2,06,3
    // asm: 	TABLEENT	'S','X','C',2,08,3
    // asm: 	TABLEENT	'T','E','D',2,10,3
    // asm: 	TABLEENT	'M','E','Y',2,12,3
    // asm: 	TABLEENT	'M','M','V',2,14,3
    // asm: 	TABLEENT	'B','D','P',2,16,3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEFAULT_TABLE", 0, 0);
    UNIMPL();
}

void DEFAULT_TABLE_TOTAL(void)
{
    // asm: 	TABLEENT	'T','V','G',28,00,3
    // asm: 	TABLEENT	'E','L','P',28,10,3
    // asm: 	TABLEENT	'E','P','J',28,20,3
    // asm: 	TABLEENT	'M','L',' ',28,30,3
    // asm: 	TABLEENT	'G','W','S',28,40,3
    // asm: 	TABLEENT	'S','X','C',28,50,3
    // asm: 	TABLEENT	'T','E','D',30,00,3
    // asm: 	TABLEENT	'M','E','Y',31,10,3
    // asm: 	TABLEENT	'M','M','V',32,20,3
    // asm: 	TABLEENT	'B','D','P',34,30,3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DEFAULT_TABLE_TOTAL", 0, 0);
    UNIMPL();
}

/* *INIT_LASTHS_TABLE
*CHECK_LASTHS
*----------------------------------------------------------------------------
*Initialize the LAST HS SET CMOS table
*
*This table is to keep track of the High Score entries set (if any at all)
*by the last player.
*There is one byte per Race location.
* -1 = not set by last player. 0-9 = Position in this table for that race
 */
void INIT_LASTHS_TABLE(void)
{
    // asm: 	LDI	NUM_TABLES+1,R6
    // asm: 	LDI	0,R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	LDI	-1,R2
    // asm: 	LDI	14,AR5
    // asm: ILT_LP
    // asm: 	CALL	_wr_cw
    // asm: 	DBU	AR5,ILT_LP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LASTHS_TABLE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*	R6 = RACE NUMBER
*	R7 = ENTRY NUMBER
 */
void UPDATE_LASTHS(void)
{
    // asm 00009AA7: 	PUSH	AR2
    // asm 00009AA8: 	PUSH	R0
    // asm 00009AA9: 	PUSH	R2
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	LDI	R6,R0
    // asm: 	LDI	NUM_TABLES+1,R6
    // asm: 	LDI	0,R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	MPYI	4,R0		;4 bytes per word
    // asm 00009AAA: 	ADDI	R0,AR2
    // asm 00009AAB: 	POP	R7
    // asm 00009AAC: 	LDI	R7,R2
    // asm 00009AAD: 	CALL	_wr_cw
    // asm 00009AAF: 	POP	R6
    // asm 00009AB0: 	POP	R2
    // asm 00009AB1: 	POP	R0
    // asm 00009AB2: 	POP	AR2
    // asm 00009AB3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UPDATE_LASTHS", 0, 0);
    UNIMPL();
}

void CHECK_LASTHS(void)
{
    // asm 00009ABC: 	LDI	NUM_TABLES+1,R6
    // asm: 	LDI	0,R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	MPYI	4,R1		;4 bytes per word
    // asm: 	ADDI	R1,AR2
    // asm: 	CALL	_rd_cw
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_LASTHS", 0, 0);
    UNIMPL();
}

void INIT_HSTD_TABLES(void)
{
    // asm: 	CLRI	R6
    // asm: 	LDI	15-1,AR5
    // asm 00009AC4: ITL1
    // asm: 	CLRI	R7
    // asm: 	LDI	10-1,AR4
    // asm: 	LDL	DEFAULT_TABLE,AR1
    // asm: 	CMPI	14,R6			;the 15th entry is the totals for the entire country
    // asm: 	LDIEQ	@DEFAULT_TABLE_TOTALI,AR1
    // asm 00009AC4: ITL2
    // asm: 	CALL	GET_TABLE_ADDR
    // asm 00009AC4: 	CALL	TABLE_ENTRY_WRITE
    // asm: 	ADDI	DEFAULT_TABLE_SIZE,AR1
    // asm 00009AC6: 	INC	R7
    // asm 00009AC7: 	DBU	AR4,ITL2
    // asm 00009AC8: 	INC	R6
    // asm 00009AC9: 	DBU	AR5,ITL1
    // asm: 	CALL	INIT_LASTHS_TABLE
    // asm 00009ACB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_HSTD_TABLES", 0, 0);
    UNIMPL();
}

void VALIDATE_HSTD_TABLES(void)
{
    // asm 00009AD1: 	CLRI	R6
    // asm: 	LDI	10-1,AR5
    // asm 00009AD2: ITL1A
    // asm 00009AD3: 	CLRI	R7
    // asm: 	LDI	10-1,AR4
    // asm: 	LDL	DEFAULT_TABLE,AR1
    // asm 00009AD4: ITL2A
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	CALL	TABLE_ENTRY_READ
    // asm 00009AD4: 	CMPI	' ',R1
    // asm 00009AD5: 	BEQ	J1
    // asm: 	CMPI	'0',R1
    // asm 00009AD6: 	BLT	RESETALL
    // asm 00009AD7: 	CMPI	'Z',R1
    // asm 00009AD8: 	BGT	RESETALL
J1:
    // asm 00009AD9: CMPI	' ',R2
    // asm 00009AD9: 	BEQ	J2
    // asm 00009ADA: 	CMPI	'0',R2
    // asm: 	BLT	RESETALL
    // asm 00009ADB: 	CMPI	'Z',R2
    // asm 00009ADC: 	BGT	RESETALL
J2:
    // asm 00009ADD: CMPI	' ',R3
    // asm 00009ADE: 	BEQ	J3
    // asm 00009ADF: 	CMPI	'0',R3
    // asm 00009AE0: 	BLT	RESETALL
    // asm 00009AE1: 	CMPI	'Z',R3
    // asm 00009AE2: 	BGT	RESETALL
J3:
    // asm 00009AE4: CMPI	0,R4
    // asm 00009AE5: 	BLT	RESETALL
    // asm 00009AE6: 	CMPI	10,R4
    // asm 00009AE7: 	BGT	RESETALL
    // asm 00009AE9: 	CMPI	4*60*60,R0
    // asm 00009AEA: 	BGT	RESETALL
    // asm 00009AEB: 	CMPI	0,R0
    // asm 00009AEC: 	BLT	RESETALL
    // asm 00009AED: 	ADDI	DEFAULT_TABLE_SIZE,AR1
    // asm 00009AEE: 	INC	R7
    // asm 00009AEF: 	DBU	AR4,ITL2A
    // asm: 	INC	R6
    // asm 00009AF1: 	DBU	AR5,ITL1A
    // asm 00009AF2: 	RETS
RESETALL:
    // asm: 	CALL	INIT_HSTD_TABLES
    // asm 00009AF5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "VALIDATE_HSTD_TABLES", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*PARAMETERS
*	R6	RACE NUMBER
*	R7	ENTRY NUMBER
*
*RETURNS
*	AR2	ADDRESS
*
 */
void GET_TABLE_ADDR(void)
{
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	MPYI	RACE_TABLE_SIZE,R6
    // asm: 	MPYI	TE_SIZE,R7
    // asm: 	ADDI	R6,R7,AR2
    // asm: 	ADDI	MAX_AUDITS*4,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	POP	R7
    // asm 00009AFD: 	POP	R6
    // asm 00009AFE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_TABLE_ADDR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*WRITE ENTRY IN TABLE
*
*PARAMETER
*	AR2	ADDRESS OF ENTRY
*	AR1	TABLE ENTRY
*RETURNS
*	(AR2 INCREMENTED TO THE NEXT ENTRY)
*
*
 */
void TABLE_ENTRY_WRITE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	AR1
    // asm: 	LDI	*AR1++,R0
    // asm: 	LDI	*AR1++,R1
    // asm: 	LDI	R1,R2
    // asm: 	RS	8,R2
    // asm 00009B08: 	LDI	R1,R3
    // asm 00009B09: 	RS	16,R3
    // asm: 	LDI	R1,R4
    // asm 00009B0B: 	RS	24,R4
    // asm 00009B0C: 	POP	AR1
    // asm 00009B0E: 	BU	TEWL1
    // *
    // *PARAMETERS
    // *	R0	TIME CODE
    // *	R1	Initial 1
    // *	R2	Initial 2
    // *	R3	Initial 3
    // *	R4	POSITION
    // *	AR2	ADDR
    // *
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_WRITE", 0, 0);
    UNIMPL();
}

void TABLE_ENTRY_WRITE0(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
TEWL1:
    // asm: 	PUSH	R2
    // asm: 	LDI	R0,R2
    // asm: 	CALL	_wr_cw
    // asm: 	POP	R2
    // asm 00009B17: 	PUSH	R1
    // asm 00009B18: 	CMOS_ON
    // asm 00009B19: 	CMOS_WP_OFF
    // ;	NOP			;DELAY FOR TIMING...
    // asm: 	POP	R1
    // asm 00009B1B: 	LS	24,R1
    // asm 00009B1C: 	STI	R1,*AR2++
    // asm 00009B1D: 	LS	24,R2
    // asm: 	STI	R2,*AR2++
    // asm 00009B1E: 	LS	24,R3
    // asm 00009B1F: 	STI	R3,*AR2++
    // asm 00009B20: 	LS	24,R4
    // asm: 	STI	R4,*AR2++
    // asm 00009B21: 	CMOS_WP_ON
    // asm: 	CMOS_OFF
    // asm 00009B23: 	POP	R3
    // asm 00009B24: 	POP	R2
    // asm 00009B25: 	POP	R1
    // asm 00009B26: 	POP	R0
    // asm 00009B27: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_WRITE0", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*READ A TABLE ENTRY
*
*
*PARAMETERS
*	AR2	ADDR
*RETURNS
*	R0	TIME CODE
*	R1	Initial 1
*	R2	Initial 2
*	R3	Initial 3
*	R4	POSITION
*	(AR2 INCREMENTED TO THE NEXT ENTRY)
*
 */
void TABLE_ENTRY_READ(void)
{
    // asm: 	CALL	_rd_cw
    // asm: 	PUSH	R0
    // asm: 	CMOS_ON
    // asm: 	CMOS_WP_OFF
    // asm: 	LDI	*AR2++,R1
    // asm: 	RS	24,R1
    // asm: 	LDI	*AR2++,R2
    // asm 00009B31: 	RS	24,R2
    // asm: 	LDI	*AR2++,R3
    // asm 00009B32: 	RS	24,R3
    // asm: 	LDI	*AR2++,R4
    // asm 00009B33: 	RS	24,R4
    // asm: 	PUSH	R1
    // asm 00009B35: 	CMOS_WP_ON	;This trashes register R1
    // asm 00009B36: 	CMOS_OFF
    // asm 00009B37: 	POP	R1
    // asm 00009B38: 	POP	R0
    // asm 00009B39: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TABLE_ENTRY_READ", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*CHECK_TABLE
*
*THIS ROUTINE CHECKS THE SPECIFIED TABLE TO SEE WHETHER OR NOT
*THE PLAYER CAN ENTER INTO THE TABLE
*
*PARAMETERS
*	R0	TIME CODE (TO CHECK)
*	R1	RACE INDEX
*RETURNS
*	R0	{-1}     = NO, MAY NOT ENTER
*	R0	{0...9}  = INDEX TO ENTER TABLE
*
 */
void CHECK_RACE_TABLE(void)
{
    // asm: 	PUSH	R0
    // asm: 	LDI	R1,R6
    // asm: 	CLRI	R7
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	POP	R6
    // asm: 	CLRI	R5		;RETURN INDEX
NXTENT:
    // asm 00009B43: CALL	TABLE_ENTRY_READ
    // asm: 	CMPI	R0,R6
    // asm: 	BLT	INSERT_HERE
    // asm 00009B44: 	INC	R5
    // asm 00009B45: 	CMPI	10,R5
    // asm 00009B46: 	BLT	NXTENT
    // asm: 	LDI	-1,R0
    // asm 00009B48: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_RACE_TABLE", 0, 0);
    UNIMPL();
}

void INSERT_HERE(void)
{
    // asm 00009B4B: 	LDI	R5,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_HERE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*PARAMETERS
*	R0	TIME CODE
*	R1	Initial 1
*	R2	Initial 2
*	R3	Initial 3
*	R4	POSITION
*	R5	INDEX #
*	R6	RACE #
*
*
*
 */
void INSERT_TABLE_ENTRY(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	LDI	NUM_ENTRIES_PER_RACE-2,R7	;Start at position 8
    // asm: 	CALL	GET_TABLE_ADDR
    // asm 00009B53: 	CMPI	9,R5
    // asm 00009B54: 	BGE	ITLX
ITEL:
    // asm 00009B56: CALL	TABLE_ENTRY_READ
    // asm 00009B57: 	CALL	TABLE_ENTRY_WRITE0	;This uses the registers as input instead of *AR1
    // asm: 	NOP	*AR2--(TE_SIZE*3)
    // asm 00009B59: 	DEC	R7
    // asm 00009B5A: 	CMPI	R5,R7
    // asm: 	BGE	ITEL				;Copy down position including where inserting
ITLX:
    // asm 00009B5C: POP	R7
    // asm: 	CALL	GET_TABLE_ADDR			;Get the ADDR of the insert point
    // asm 00009B5D: 	POP	R4
    // asm 00009B5E: 	POP	R3
    // asm: 	POP	R2
    // asm 00009B5F: 	POP	R1
    // asm 00009B60: 	POP	R0
    // asm 00009B61: 	CALL	TABLE_ENTRY_WRITE0	;This uses the registers as input instead of *AR1
    // asm: 	CALL	UPDATE_LASTHS
    // asm 00009B63: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_TABLE_ENTRY", 0, 0);
    UNIMPL();
}

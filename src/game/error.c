#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "mproc.h"
#include "obj.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "diag.h"
#include "error.h"
#include "port.h"

/*
 * Source module: asm/ERROR.ASM
 */

void ERROR_LOG(void);
void ERROR_LOG_DISPLAY(void);

extern const char NO[];

/*
*----------------------------------------------------------------------------
*
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

#define ERR_SIZE 16
#define MAX_LOGS 30
#define PSS_AR7 0
#define PSS_AR6 1
#define PSS_AR5 2
#define PSS_AR4 3
#define PSS_AR3 4
#define PSS_AR2 5
#define PSS_AR1 6
#define PSS_AR0 7
#define PSS_DP 8
#define PSS_SP 9
#define PSS_R0 10

// *----------------------------------------------------------------------------
void ERROR_LOG(void)
{
    // asm 0000AB4A: 	PUSH	R1
    // asm 0000AB4B: 	PUSH	R2
    // asm 0000AB4C: 	PUSH	R3
    // asm 0000AB4D: 	PUSH	R4
    // asm 0000AB4E: 	PUSH	R0
    // asm 0000AB4F: 	PUSH	R0
    // asm 0000AB50: 	PUSH	DP
    // asm 0000AB51: 	PUSH	AR0
    // asm 0000AB52: 	PUSH	AR1
    // asm 0000AB53: 	PUSH	AR2
    // asm 0000AB54: 	PUSH	AR3
    // asm 0000AB55: 	PUSH	AR4
    // asm 0000AB56: 	PUSH	AR5
    // asm 0000AB57: 	PUSH	AR6
    // asm 0000AB58: 	PUSH	AR7
    // asm 0000AB59: 	LDI	SP,AR3			;SAVE FRAME POINTER!! (all regs saved)
    // asm 0000AB5A: 	SETDP
    // asm 0000AB5B: 	READAUD	AUD_LOG_COUNT
    // asm 0000AB5D: 	INC	R0
    // asm 0000AB5E: 	CMPI	MAX_LOGS-1,R0
    // asm 0000AB5F: 	BGE	ERROR_X
    // asm 0000AB60: 	LDI	R0,R2
    // asm 0000AB61: 	SETAUD	AUD_LOG_COUNT
    // asm 0000AB63: 	DEC	R0
    // asm 0000AB64: 	MPYI	ERR_SIZE*4,R0		;ERR_SIZE * CMOS_WORD_SIZE
    // asm 0000AB65: 	LDI	R0,AR2
    // asm 0000AB66: 	ADDI	RACE_TABLE_SIZE*NUM_RACES+NUM_RACES,AR2
    // asm 0000AB67: 	ADDI	MAX_AUDITS*4,AR2
    // asm 0000AB68: 	ADDI	@CMOSI,AR2
    // asm 0000AB69: 	LDI	*-AR3(PSS_R0),R2	;ERROR TYPE
    // asm 0000AB6A: 	CALL	_wr_cw
    // asm 0000AB6B: 	LDI	@CURRENT_PROC,AR0
    // asm 0000AB6C: 	LDI	*+AR0(PWAKE),R2
    // asm 0000AB6D: 	CALL	_wr_cw
    // asm 0000AB6E: 	LDI	*-AR3(PSS_DP),R2
    // asm 0000AB6F: 	CALL	_wr_cw
    // asm 0000AB70: 	LDI	*-AR3(PSS_SP),R2
    // asm 0000AB71: 	CALL	_wr_cw
    // asm 0000AB72: 	LDI	*-AR3(PSS_AR0),R2
    // asm 0000AB73: 	CALL	_wr_cw
    // asm 0000AB74: 	LDI	*-AR3(PSS_AR1),R2
    // asm 0000AB75: 	CALL	_wr_cw
    // asm 0000AB76: 	LDI	*-AR3(PSS_AR2),R2
    // asm 0000AB77: 	CALL	_wr_cw
    // asm 0000AB78: 	LDI	*-AR3(PSS_AR2),AR0
    // asm 0000AB79: 	LDI	*+AR0(OID),R2
    // ;	LDI	*-AR3(PSS_AR3),R2
    // asm 0000AB7A: 	CALL	_wr_cw
    // asm 0000AB7B: 	LDI	*-AR3(PSS_AR4),R2
    // asm 0000AB7C: 	CALL	_wr_cw
    // asm 0000AB7D: 	LDI	@_MODE,R2
    // ;	LDI	*-AR3(PSS_AR5),R2
    // asm 0000AB7E: 	CALL	_wr_cw
    // asm 0000AB7F: 	LDI	*-AR3(PSS_AR2),AR0
    // asm 0000AB80: 	LDI	*+AR0(PID),R2
    // ;	LDI	*-AR3(PSS_AR6),R2
    // asm 0000AB81: 	CALL	_wr_cw
    // asm 0000AB82: 	LDI	*-AR3(PSS_AR7),R2
    // asm 0000AB83: 	CALL	_wr_cw
    // asm 0000AB84: 	LDI	@CURRENT_PROC,AR0
    // asm 0000AB85: 	LDI	*+AR0(PID),R2
    // asm 0000AB86: 	CALL	_wr_cw
    // asm 0000AB87: 	PUSH	AR2
    // asm 0000AB88: 	READAUD	AUD_GAMENUMBER
    // asm 0000AB8A: 	POP	AR2
    // asm 0000AB8B: 	LDI	R0,R2
    // asm 0000AB8C: 	CALL	_wr_cw
    // asm 0000AB8D: 	PUSH	AR2
    // asm 0000AB8E: 	READAUD	AUD_NUM_UNFINISHED
    // asm 0000AB90: 	POP	AR2
    // asm 0000AB91: 	LDI	R0,R2
    // asm 0000AB92: 	CALL	_wr_cw
    // asm 0000AB93: 	PUSH	AR2
    // asm 0000AB94: 	LDI	@PLYCAR,AR2
    // asm 0000AB95: 	LDI	*+AR2(CARTRAK),AR2
    // asm 0000AB96: 	LDI	*+AR2(OUSR1),R2
    // asm 0000AB97: 	POP	AR2
    // asm 0000AB98: 	CALL	_wr_cw
ERROR_X:
    // asm 0000AB99: 	POP	AR7
    // asm 0000AB9A: 	POP	AR6
    // asm 0000AB9B: 	POP	AR5
    // asm 0000AB9C: 	POP	AR4
    // asm 0000AB9D: 	POP	AR3
    // asm 0000AB9E: 	POP	AR2
    // asm 0000AB9F: 	POP	AR1
    // asm 0000ABA0: 	POP	AR0
    // asm 0000ABA1: 	POP	DP
    // asm 0000ABA2: 	POP	R0
    // asm 0000ABA3: 	POP	R0
    // asm 0000ABA4: 	POP	R4
    // asm 0000ABA5: 	POP	R3
    // asm 0000ABA6: 	POP	R2
    // asm 0000ABA7: 	POP	R1
    // asm 0000ABA8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ERROR_LOG", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*TO BE REACHED VIA THE CONSOLE (NO SWITCHES)
*
*	ONCE ENTERED, USER MUST SCROLL THROUGH ALL
*	OF THE LOGS.  ROUTINE THEN RETURNS.
*
*
*/
void ERROR_LOG_DISPLAY(void)
{
    // asm 0000ABA9: 	CALL	CLRSCRN
    // asm 0000ABAA: 	CALL	SETPAGE0
    // asm 0000ABAB: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 0000ABAE: 	STI	R0,@ACTIVE_SCREEN
    // asm 0000ABAF: 	LDI	COL_VDGREY,RC
    // asm 0000ABB0: 	TEXTITT	"VIEW 1  TO EXIT",10,310
    // asm 0000ABB4: 	TEXTITT	"VIEW 3  NEXT PAGE",10,320
    // asm 0000ABB8: 	TEXTITT	"VIEW 3 + VIEW 2 + VIEW 1 CLEAR LOG",10,330
ERRLOG_D:
    // asm 0000ABBC: 	LDI	RACE_TABLE_SIZE*NUM_RACES+NUM_RACES,AR2
    // asm 0000ABBD: 	ADDI	MAX_AUDITS*4,AR2
    // asm 0000ABBE: 	ADDI	@CMOSI,AR2
    // asm 0000ABBF: 	CLRI	AR6
NEXT_PAGE:
    // asm 0000ABC0: 	PUSH	AR2
    // asm 0000ABC1: 	IWINDOW	5,35,510,295
    // ;	FILL	10,40,510,300,0
    // asm 0000ABC6: 	POP	AR2
    // asm 0000ABC7: 	PUSH	AR2
    // asm 0000ABC8: 	READAUD	AUD_LOG_COUNT
    // asm 0000ABCA: 	POP	AR2
    // asm 0000ABCB: 	CMPI	0,R0
    // asm 0000ABCC: 	BGT	ERRLOG_D2
    // asm 0000ABCD: 	TEXTITT	"NO ERRORS",10,50
ERRLOG_D2:
    // asm 0000ABD1: 	LDI	5,R4
    // asm 0000ABD2: 	LDI	10,R6
    // asm 0000ABD3: 	LDI	50,R7
BIGLOOP:
    // asm 0000ABD4: 	PUSH	AR2
    // asm 0000ABD5: 	READAUD	AUD_LOG_COUNT
    // asm 0000ABD7: 	POP	AR2
    // asm 0000ABD8: 	CMPI	R0,AR6
    // asm 0000ABD9: 	BEQ	LLPPT
    // asm 0000ABDA: 	CMPI	0,R0
    // asm 0000ABDB: 	BEQ	LLPPT
    // asm 0000ABDC: 	CMPI	MAX_LOGS,AR6
    // asm 0000ABDD: 	BGE	ERRLOG_D
    // asm 0000ABDE: 	PUSH	AR2
    // asm 0000ABDF: 	LDI	@OBJSTRI,AR2
    // asm 0000ABE0: 	LDI	AR6,R2
    // asm 0000ABE1: 	INC	R2
    // asm 0000ABE2: 	CALL	_itoa
    // asm 0000ABE3: 	LDI	135,R2
    // asm 0000ABE4: 	LDI	R7,R3
    // asm 0000ABE5: 	SUBI	10,R3
    // asm 0000ABE6: 	LDI	@OBJSTRI,AR2
    // asm 0000ABE7: 	LDI	DEFAULT_COLOR,RC
    // asm 0000ABE8: 	CALL	_outtextxyc
    // asm 0000ABE9: 	LDI	100,R2
    // asm 0000ABEA: 	LDI	R7,R3
    // asm 0000ABEB: 	SUBI	10,R3
    // asm 0000ABEC: 	LDL	TIBO,AR2
    // asm 0000ABED: 	LDI	DEFAULT_COLOR,RC
    // asm 0000ABEE: 	CALL	_outtextxyc
    // asm 0000ABEF: 	POP	AR2
    // 	;R6  X Position
    // 	;R7  Y Height
    // 	;
    // asm 0000ABF0: 	LDI	ERR_SIZE-1,AR5
    // asm 0000ABF1: ONE_LOG
    // asm 0000ABF1: 	CALL	_rd_cw
    // asm 0000ABF2: 	LDI	R0,R2
    // asm 0000ABF3: 	PUSH	AR2
    // asm 0000ABF4: 	LDI	@OBJSTRI,AR2
    // asm 0000ABF5: 	CALL	HEX2ASC
    // asm 0000ABF6: 	LDI	R6,R2
    // asm 0000ABF7: 	LDI	R7,R3
    // asm 0000ABF8: 	LDI	@OBJSTRI,AR2
    // asm 0000ABF9: 	LDI	DEFAULT_COLOR,RC
    // asm 0000ABFA: 	CALL	_outtextxyc
    // asm 0000ABFB: 	POP	AR2
    // asm 0000ABFC: 	ADDI	70,R6
    // asm 0000ABFD: 	CMPI	450,R6
    // asm 0000ABFE: 	BLT	ION
    // asm 0000ABFF: 	LDI	10,R6
    // asm 0000AC00: 	ADDI	10,R7
ION:
    // asm 0000AC01: DBU	AR5,ONE_LOG
    // asm 0000AC02: 	LDI	10,R6
    // asm 0000AC03: 	ADDI	20,R7
    // asm 0000AC04: 	INC	AR6
    // asm 0000AC05: 	DEC	R4
    // asm 0000AC06: 	BLT	LLPPT
    // asm 0000AC07: 	BU	BIGLOOP
LLPPT:
    // asm 0000AC08: 	CALL	WAIT_FOR_VBLANK
    // asm 0000AC09: 	LDI	@_newbut,R1		;debounce
    // asm 0000AC0A: 	TSTB	SW_DIAG,R1		;start to return to main
    // asm 0000AC0B: 	BZ	BOOKX
    // asm 0000AC0C: 	RS	16,R1
    // asm 0000AC0D: 	LDI	R1,R0
    // asm 0000AC0E: 	AND	SW_VIEW1_H|SW_VIEW0_H|SW_VIEW2_H,R0
    // ;	CMPI	SW_VIEW1_H|SW_VIEW0_H|SW_VIEW2_H,R0
    // asm 0000AC0F: 	BNE	NOCLEAR
    // asm 0000AC10: 	CLRI	R2
    // asm 0000AC11: 	SETAUD	AUD_LOG_COUNT
    // asm 0000AC13: 	CALL	WAIT_FOR_VBLANK
    // asm 0000AC14: 	CALL	WAIT_FOR_VBLANK
    // asm 0000AC15: 	CALL	WAIT_FOR_VBLANK
    // asm 0000AC16: 	BU	ERROR_LOG_DISPLAY
NOCLEAR:
    // asm 0000AC17: 	TSTB	SW_VIEW2_H,R1
    // asm 0000AC18: 	BNZ	NOT_NEXT_PAGE
    // asm 0000AC19: 	PUSH	AR2
    // asm 0000AC1A: 	READAUD	AUD_LOG_COUNT
    // asm 0000AC1C: 	POP	AR2
    // asm 0000AC1D: 	CMPI	R0,AR6
    // asm 0000AC1E: 	BGE	ERRLOG_D
    // asm 0000AC1F: 	BU	NEXT_PAGE
NOT_NEXT_PAGE:
    // asm 0000AC20: 	TSTB	SW_VIEW0_H,R1
    // asm 0000AC21: 	BZ	BOOKX2
    // asm 0000AC22: 	BU	LLPPT
BOOKX2:
    // asm 0000AC23: LDI	@_newbut,R1		;debounce
    // asm 0000AC24: 	RS	16,R1
    // asm 0000AC25: 	TSTB	SW_VIEW0_H,R1		;start to return to main
    // asm 0000AC26: 	BNZ	BOOKX2
    // asm 0000AC27: 	RETS
BOOKX:
    // asm 0000AC28: NOT	@_newbut,R0
    // asm 0000AC29: 	AND	SW_DIAG,R0
    // asm 0000AC2A: 	BNZ	BOOKX
    // asm 0000AC2B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ERROR_LOG_DISPLAY", 0, 0);
    UNIMPL();
}

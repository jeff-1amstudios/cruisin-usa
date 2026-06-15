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

/*
 * Source module: asm/ERROR.ASM
 */

void ERROR_LOG(void);
void ERROR_LOG_DISPLAY(void);
void ION(void);

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
const char *TIBO = "LOG";

/* *----------------------------------------------------------------------------
 */
void ERROR_LOG(void)
{
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	R0
    // asm: 	PUSH	R0
    // asm: 	PUSH	DP
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	AR6
    // asm: 	PUSH	AR7
    // asm: 	LDI	SP,AR3			;SAVE FRAME POINTER!! (all regs saved)
    // asm 0000AB4B: 	SETDP
    // asm 0000AB4C: 	READAUD	AUD_LOG_COUNT
    // asm 0000AB4D: 	INC	R0
    // asm 0000AB4E: 	CMPI	MAX_LOGS-1,R0
    // asm: 	BGE	ERROR_X
    // asm 0000AB50: 	LDI	R0,R2
    // asm 0000AB51: 	SETAUD	AUD_LOG_COUNT
    // asm 0000AB52: 	DEC	R0
    // asm 0000AB53: 	MPYI	ERR_SIZE*4,R0		;ERR_SIZE * CMOS_WORD_SIZE
    // asm 0000AB54: 	LDI	R0,AR2
    // asm 0000AB56: 	ADDI	RACE_TABLE_SIZE*NUM_RACES+NUM_RACES,AR2
    // asm 0000AB57: 	ADDI	MAX_AUDITS*4,AR2
    // asm 0000AB58: 	ADDI	@CMOSI,AR2
    // asm 0000AB59: 	LDI	*-AR3(PSS_R0),R2	;ERROR TYPE
    // asm: 	CALL	_wr_cw
    // asm 0000AB5A: 	LDI	@CURRENT_PROC,AR0
    // asm 0000AB5B: 	LDI	*+AR0(PWAKE),R2
    // asm 0000AB5D: 	CALL	_wr_cw
    // asm 0000AB5F: 	LDI	*-AR3(PSS_DP),R2
    // asm: 	CALL	_wr_cw
    // asm 0000AB60: 	LDI	*-AR3(PSS_SP),R2
    // asm 0000AB61: 	CALL	_wr_cw
    // asm 0000AB64: 	LDI	*-AR3(PSS_AR0),R2
    // asm 0000AB65: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_AR1),R2
    // asm 0000AB66: 	CALL	_wr_cw
    // asm 0000AB67: 	LDI	*-AR3(PSS_AR2),R2
    // asm 0000AB68: 	CALL	_wr_cw
    // asm 0000AB69: 	LDI	*-AR3(PSS_AR2),AR0
    // asm 0000AB6A: 	LDI	*+AR0(OID),R2
    // ;	LDI	*-AR3(PSS_AR3),R2
    // asm 0000AB6C: 	CALL	_wr_cw
    // asm 0000AB6D: 	LDI	*-AR3(PSS_AR4),R2
    // asm: 	CALL	_wr_cw
    // asm 0000AB6F: 	LDI	@_MODE,R2
    // ;	LDI	*-AR3(PSS_AR5),R2
    // asm 0000AB71: 	CALL	_wr_cw
    // asm 0000AB72: 	LDI	*-AR3(PSS_AR2),AR0
    // asm 0000AB73: 	LDI	*+AR0(PID),R2
    // ;	LDI	*-AR3(PSS_AR6),R2
    // asm 0000AB75: 	CALL	_wr_cw
    // asm 0000AB76: 	LDI	*-AR3(PSS_AR7),R2
    // asm 0000AB77: 	CALL	_wr_cw
    // asm 0000AB78: 	LDI	@CURRENT_PROC,AR0
    // asm 0000AB79: 	LDI	*+AR0(PID),R2
    // asm: 	CALL	_wr_cw
    // asm 0000AB7B: 	PUSH	AR2
    // asm 0000AB7C: 	READAUD	AUD_GAMENUMBER
    // asm: 	POP	AR2
    // asm 0000AB7D: 	LDI	R0,R2
    // asm: 	CALL	_wr_cw
    // asm: 	PUSH	AR2
    // asm 0000AB7F: 	READAUD	AUD_NUM_UNFINISHED
    // asm 0000AB80: 	POP	AR2
    // asm: 	LDI	R0,R2
    // asm 0000AB81: 	CALL	_wr_cw
    // asm 0000AB83: 	PUSH	AR2
    // asm: 	LDI	@PLYCAR,AR2
    // asm 0000AB84: 	LDI	*+AR2(CARTRAK),AR2
    // asm 0000AB85: 	LDI	*+AR2(OUSR1),R2
    // asm 0000AB86: 	POP	AR2
    // asm: 	CALL	_wr_cw
    // asm 0000AB88: ERROR_X
    // asm 0000AB8A: 	POP	AR7
    // asm 0000AB8B: 	POP	AR6
    // asm 0000AB8C: 	POP	AR5
    // asm: 	POP	AR4
    // asm 0000AB8D: 	POP	AR3
    // asm 0000AB8E: 	POP	AR2
    // asm 0000AB90: 	POP	AR1
    // asm 0000AB91: 	POP	AR0
    // asm 0000AB92: 	POP	DP
    // asm: 	POP	R0
    // asm 0000AB94: 	POP	R0
    // asm 0000AB95: 	POP	R4
    // asm 0000AB96: 	POP	R3
    // asm 0000AB97: 	POP	R2
    // asm 0000AB98: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ERROR_LOG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*TO BE REACHED VIA THE CONSOLE (NO SWITCHES)
*
*	ONCE ENTERED, USER MUST SCROLL THROUGH ALL
*	OF THE LOGS.  ROUTINE THEN RETURNS.
*
*
 */
void ERROR_LOG_DISPLAY(void)
{
    // asm 0000ABA3: 	CALL	CLRSCRN
    // asm 0000ABA4: 	CALL	SETPAGE0
    // asm 0000ABA6: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 0000ABA7: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"VIEW 1  TO EXIT",10,310
    // asm: 	TEXTITT	"VIEW 3  NEXT PAGE",10,320
    // asm: 	TEXTITT	"VIEW 3 + VIEW 2 + VIEW 1 CLEAR LOG",10,330
    // asm 0000ABA9: ERRLOG_D
    // asm: 	LDI	RACE_TABLE_SIZE*NUM_RACES+NUM_RACES,AR2
    // asm: 	ADDI	MAX_AUDITS*4,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CLRI	AR6
NEXT_PAGE:
    // asm: 	PUSH	AR2
    // asm 0000ABAB: 	IWINDOW	5,35,510,295
    // ;	FILL	10,40,510,300,0
    // asm: 	POP	AR2
    // asm 0000ABB0: 	PUSH	AR2
    // asm 0000ABB4: 	READAUD	AUD_LOG_COUNT
    // asm 0000ABB8: 	POP	AR2
    // asm: 	CMPI	0,R0
    // asm: 	BGT	ERRLOG_D2
    // asm: 	TEXTITT	"NO ERRORS",10,50
    // asm 0000ABBC: ERRLOG_D2
    // asm 0000ABBD: 	LDI	5,R4
    // asm 0000ABBE: 	LDI	10,R6
    // asm 0000ABBF: 	LDI	50,R7
    // asm 0000ABC0: BIGLOOP
    // asm: 	PUSH	AR2
    // asm 0000ABC0: 	READAUD	AUD_LOG_COUNT
    // asm 0000ABC1: 	POP	AR2
    // asm: 	CMPI	R0,AR6
    // asm 0000ABC6: 	BEQ	LLPPT
    // asm 0000ABC7: 	CMPI	0,R0
    // asm 0000ABC8: 	BEQ	LLPPT
    // asm 0000ABCB: 	CMPI	MAX_LOGS,AR6
    // asm 0000ABCC: 	BGE	ERRLOG_D
    // asm: 	PUSH	AR2
    // asm 0000ABD3: 	LDI	@OBJSTRI,AR2
    // asm: 	LDI	AR6,R2
    // asm 0000ABD4: 	INC	R2
    // asm 0000ABD5: 	CALL	_itoa
    // asm 0000ABD7: 	LDI	135,R2
    // asm 0000ABD8: 	LDI	R7,R3
    // asm 0000ABD9: 	SUBI	10,R3
    // asm: 	LDI	@OBJSTRI,AR2
    // asm 0000ABDA: 	LDI	DEFAULT_COLOR,RC
    // asm 0000ABDB: 	CALL	_outtextxyc
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ERROR_LOG_DISPLAY", 0, 0);
    UNIMPL();
}

void ION(void)
{
    // asm 0000ABF3: DBU	AR5,ONE_LOG
    // asm: 	LDI	10,R6
    // asm 0000ABF4: 	ADDI	20,R7
    // asm: 	INC	AR6
    // asm 0000ABF7: 	DEC	R4
    // asm 0000ABF8: 	BLT	LLPPT
    // asm 0000ABF9: 	BU	BIGLOOP
LLPPT:
    // asm 0000ABFD: 	CALL	WAIT_FOR_VBLANK
    // asm 0000AC01: 	LDI	@_newbut,R1		;debounce
    // asm: 	TSTB	SW_DIAG,R1		;start to return to main
    // asm 0000AC02: 	BZ	BOOKX
    // asm: 	RS	16,R1
    // asm: 	LDI	R1,R0
    // asm 0000AC05: 	AND	SW_VIEW1_H|SW_VIEW0_H|SW_VIEW2_H,R0
    // ;	CMPI	SW_VIEW1_H|SW_VIEW0_H|SW_VIEW2_H,R0
    // asm 0000AC07: 	BNE	NOCLEAR
    // asm: 	CLRI	R2
    // asm: 	SETAUD	AUD_LOG_COUNT
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm 0000AC08: 	CALL	WAIT_FOR_VBLANK
    // asm: 	BU	ERROR_LOG_DISPLAY
NOCLEAR:
    // asm 0000AC0A: 	TSTB	SW_VIEW2_H,R1
    // asm 0000AC0B: 	BNZ	NOT_NEXT_PAGE
    // asm 0000AC0C: 	PUSH	AR2
    // asm: 	READAUD	AUD_LOG_COUNT
    // asm 0000AC0D: 	POP	AR2
    // asm 0000AC0E: 	CMPI	R0,AR6
    // asm: 	BGE	ERRLOG_D
    // asm 0000AC10: 	BU	NEXT_PAGE
NOT_NEXT_PAGE:
    // asm 0000AC14: 	TSTB	SW_VIEW0_H,R1
    // asm 0000AC15: 	BZ	BOOKX2
    // asm 0000AC16: 	BU	LLPPT
    // asm 0000AC17: BOOKX2	LDI	@_newbut,R1		;debounce
    // asm 0000AC17: 	RS	16,R1
    // asm 0000AC18: 	TSTB	SW_VIEW0_H,R1		;start to return to main
    // asm: 	BNZ	BOOKX2
    // asm 0000AC19: 	RETS
    // asm 0000AC1D: BOOKX	NOT	@_newbut,R0
    // asm 0000AC1E: 	AND	SW_DIAG,R0
    // asm: 	BNZ	BOOKX
    // asm 0000AC1F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ION", 0, 0);
    UNIMPL();
}

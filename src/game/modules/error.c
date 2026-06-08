#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/obj.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/diag.h"
#include "../include/error.h"

/*
 * Source module: asm/ERROR.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *STRUCT ERROR
#define ERR_PROC_ID 0
#define ERR_WAKE_ADDR 1
#define ERR_DP 2
#define ERR_SP 3
#define ERR_AR0 4
#define ERR_AR1 5
#define ERR_AR2 6
#define ERR_AR3 7
#define ERR_AR4 8
#define ERR_AR5 9
#define ERR_AR6 10
#define ERR_AR7 11
#define ERR_AR4_OID 12
#define ERR_RACENUM 13
#define ERR_UNFINISHED_GAME 14
#define ERR_CLOSETRAK 15
// *ENDSTRUCT
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
// *----------------------------------------------------------------------------

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
    // asm: 	SETDP
    // asm: 	READAUD	AUD_LOG_COUNT
    // asm: 	INC	R0
    // asm: 	CMPI	MAX_LOGS-1,R0
    // asm: 	BGE	ERROR_X
    // asm: 	LDI	R0,R2
    // asm: 	SETAUD	AUD_LOG_COUNT
    // asm: 	DEC	R0
    // asm: 	MPYI	ERR_SIZE*4,R0		;ERR_SIZE * CMOS_WORD_SIZE
    // asm: 	LDI	R0,AR2
    // asm: 	ADDI	RACE_TABLE_SIZE*NUM_RACES+NUM_RACES,AR2
    // asm: 	ADDI	MAX_AUDITS*4,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	LDI	*-AR3(PSS_R0),R2	;ERROR TYPE
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	@CURRENT_PROC,AR0
    // asm: 	LDI	*+AR0(PWAKE),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_DP),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_SP),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_AR0),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_AR1),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_AR2),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_AR2),AR0
    // asm: 	LDI	*+AR0(OID),R2
    // ;	LDI	*-AR3(PSS_AR3),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_AR4),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	@_MODE,R2
    // ;	LDI	*-AR3(PSS_AR5),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_AR2),AR0
    // asm: 	LDI	*+AR0(PID),R2
    // ;	LDI	*-AR3(PSS_AR6),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	*-AR3(PSS_AR7),R2
    // asm: 	CALL	_wr_cw
    // asm: 	LDI	@CURRENT_PROC,AR0
    // asm: 	LDI	*+AR0(PID),R2
    // asm: 	CALL	_wr_cw
    // asm: 	PUSH	AR2
    // asm: 	READAUD	AUD_GAMENUMBER
    // asm: 	POP	AR2
    // asm: 	LDI	R0,R2
    // asm: 	CALL	_wr_cw
    // asm: 	PUSH	AR2
    // asm: 	READAUD	AUD_NUM_UNFINISHED
    // asm: 	POP	AR2
    // asm: 	LDI	R0,R2
    // asm: 	CALL	_wr_cw
    // asm: 	PUSH	AR2
    // asm: 	LDI	@PLYCAR,AR2
    // asm: 	LDI	*+AR2(CARTRAK),AR2
    // asm: 	LDI	*+AR2(OUSR1),R2
    // asm: 	POP	AR2
    // asm: 	CALL	_wr_cw
ERROR_X:
    // asm: 	POP	AR7
    // asm: 	POP	AR6
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	DP
    // asm: 	POP	R0
    // asm: 	POP	R0
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ERROR_LOG", 0, 0);
    UNIMPL();
}

void ERROR_LOG_DISPLAY(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *TO BE REACHED VIA THE CONSOLE (NO SWITCHES)
    // *
    // *	ONCE ENTERED, USER MUST SCROLL THROUGH ALL
    // *	OF THE LOGS.  ROUTINE THEN RETURNS.
    // *
    // *
    // asm: 	CALL	CLRSCRN
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDI	COL_VDGREY,RC
    // asm: 	TEXTITT	"VIEW 1  TO EXIT",10,310
    // asm: 	TEXTITT	"VIEW 3  NEXT PAGE",10,320
    // asm: 	TEXTITT	"VIEW 3 + VIEW 2 + VIEW 1 CLEAR LOG",10,330
ERRLOG_D:
    // asm: 	LDI	RACE_TABLE_SIZE*NUM_RACES+NUM_RACES,AR2
    // asm: 	ADDI	MAX_AUDITS*4,AR2
    // asm: 	ADDI	@CMOSI,AR2
    // asm: 	CLRI	AR6
NEXT_PAGE:
    // asm: 	PUSH	AR2
    // asm: 	IWINDOW	5,35,510,295
    // ;	FILL	10,40,510,300,0
    // asm: 	POP	AR2
    // asm: 	PUSH	AR2
    // asm: 	READAUD	AUD_LOG_COUNT
    // asm: 	POP	AR2
    // asm: 	CMPI	0,R0
    // asm: 	BGT	ERRLOG_D2
    // asm: 	TEXTITT	"NO ERRORS",10,50
ERRLOG_D2:
    // asm: 	LDI	5,R4
    // asm: 	LDI	10,R6
    // asm: 	LDI	50,R7
BIGLOOP:
    // asm: 	PUSH	AR2
    // asm: 	READAUD	AUD_LOG_COUNT
    // asm: 	POP	AR2
    // asm: 	CMPI	R0,AR6
    // asm: 	BEQ	LLPPT
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	LLPPT
    // asm: 	CMPI	MAX_LOGS,AR6
    // asm: 	BGE	ERRLOG_D
    // asm: 	PUSH	AR2
    // asm: 	LDI	@OBJSTRI,AR2
    // asm: 	LDI	AR6,R2
    // asm: 	INC	R2
    // asm: 	CALL	_itoa
    // asm: 	LDI	135,R2
    // asm: 	LDI	R7,R3
    // asm: 	SUBI	10,R3
    // asm: 	LDI	@OBJSTRI,AR2
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	LDI	100,R2
    // asm: 	LDI	R7,R3
    // asm: 	SUBI	10,R3
    // asm: 	LDL	TIBO,AR2
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	POP	AR2
    // 	;R6  X Position
    // 	;R7  Y Height
    // 	;
    // asm: 	LDI	ERR_SIZE-1,AR5
    // asm: ONE_LOG
    // asm: 	CALL	_rd_cw
    // asm: 	LDI	R0,R2
    // asm: 	PUSH	AR2
    // asm: 	LDI	@OBJSTRI,AR2
    // asm: 	CALL	HEX2ASC
    // asm: 	LDI	R6,R2
    // asm: 	LDI	R7,R3
    // asm: 	LDI	@OBJSTRI,AR2
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	CALL	_outtextxyc
    // asm: 	POP	AR2
    // asm: 	ADDI	70,R6
    // asm: 	CMPI	450,R6
    // asm: 	BLT	ION
    // asm: 	LDI	10,R6
    // asm: 	ADDI	10,R7
ION:
    // asm: DBU	AR5,ONE_LOG
    // asm: 	LDI	10,R6
    // asm: 	ADDI	20,R7
    // asm: 	INC	AR6
    // asm: 	DEC	R4
    // asm: 	BLT	LLPPT
    // asm: 	BU	BIGLOOP
LLPPT:
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	LDI	@_newbut,R1		;debounce
    // asm: 	TSTB	SW_DIAG,R1		;start to return to main
    // asm: 	BZ	BOOKX
    // asm: 	RS	16,R1
    // asm: 	LDI	R1,R0
    // asm: 	AND	SW_VIEW1_H|SW_VIEW0_H|SW_VIEW2_H,R0
    // ;	CMPI	SW_VIEW1_H|SW_VIEW0_H|SW_VIEW2_H,R0
    // asm: 	BNE	NOCLEAR
    // asm: 	CLRI	R2
    // asm: 	SETAUD	AUD_LOG_COUNT
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	BU	ERROR_LOG_DISPLAY
NOCLEAR:
    // asm: 	TSTB	SW_VIEW2_H,R1
    // asm: 	BNZ	NOT_NEXT_PAGE
    // asm: 	PUSH	AR2
    // asm: 	READAUD	AUD_LOG_COUNT
    // asm: 	POP	AR2
    // asm: 	CMPI	R0,AR6
    // asm: 	BGE	ERRLOG_D
    // asm: 	BU	NEXT_PAGE
NOT_NEXT_PAGE:
    // asm: 	TSTB	SW_VIEW0_H,R1
    // asm: 	BZ	BOOKX2
    // asm: 	BU	LLPPT
    // asm: BOOKX2	LDI	@_newbut,R1		;debounce
    // asm: 	RS	16,R1
    // asm: 	TSTB	SW_VIEW0_H,R1		;start to return to main
    // asm: 	BNZ	BOOKX2
    // asm: 	RETS
    // asm: BOOKX	NOT	@_newbut,R0
    // asm: 	AND	SW_DIAG,R0
    // asm: 	BNZ	BOOKX
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ERROR_LOG_DISPLAY", 0, 0);
    UNIMPL();
}

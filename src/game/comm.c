#include "../core/cpu.h"
#include "../core/machine.h"
#include "c30.h"
#include "obj.h"
#include "macs.h"
#include "mproc.h"
#include "vunit.h"
#include "cmos.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "comm.h"
#include "commq.h"

/*
 * Source module: asm/COMM.ASM
 */

void COMM_INIT(void);
void COMM_ENABLE_INT2(void);
void COMM_MASTER_SEND_SYNC(void);
void SETONE(void);
void CLRONE(void);
void COMM_ROUTINE(void);
void COMM_MASTER(void);
void COMM_IRQ(void);
void DO_SLAVE_SYNC(void);
void COMM_SLAVE(void);

#define RECEIVE_BUFFERI RECEIVE_BUFFER
#define COMM_IOI COMM_IO

extern int COMM_MASTER_ERROR_CNT;
extern int COMM_MASTER_TRANSES;
extern int COMM_SLAVE_ERROR_CNT;
extern int COMM_SLAVE_TRANSES;
extern int COMM_SLAVE_NREADY;
extern int COMMFLAG;
extern int ONEFLAG;
extern int TRANSMISSION_ACTIVE;
extern int TRANSMISSION_DEAD;

/*
*----------------------------------------------------------------------------
*
*
*COPYRIGHT (C) 1994 BY  TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

#define TIME_DELAY 18
/* asm: COMM_MASTER_ERROR_CNT	fbss	COMM_MASTER_ERROR_CNT,1 */
int COMM_MASTER_ERROR_CNT;
/* asm: COMM_MASTER_TRANSES	fbss	COMM_MASTER_TRANSES,1 */
int COMM_MASTER_TRANSES;
/* asm: COMM_SLAVE_ERROR_CNT	fbss	COMM_SLAVE_ERROR_CNT,1 */
int COMM_SLAVE_ERROR_CNT;
/* asm: COMM_SLAVE_TRANSES	fbss	COMM_SLAVE_TRANSES,1 */
int COMM_SLAVE_TRANSES;
/* asm: COMM_SLAVE_NREADY	fbss	COMM_SLAVE_NREADY,1 */
int COMM_SLAVE_NREADY;
/* asm: COMMFLAG	.bss	COMMFLAG,1 */
int COMMFLAG;
/* asm: ONEFLAG	pbss	ONEFLAG,1 */
int ONEFLAG;
/* asm: TRANSMISSION_ACTIVE	pbss	TRANSMISSION_ACTIVE,1 */
int TRANSMISSION_ACTIVE;
/* asm: TRANSMISSION_DEAD	pbss	TRANSMISSION_DEAD,1 */
int TRANSMISSION_DEAD;

// *----------------------------------------------------------------------------
void COMM_INIT(void)
{
    // asm 00007F3F: 	PUSH	DP
    // asm 00007F40: 	LDP	@DIPRAM
    // asm 00007F41: 	LDI	@DIPRAM,R1
    // asm 00007F42: 	LDI	0,R0
    // asm 00007F43: 	STI	R0,@ONEFLAG		;CLEAR OUT ONE PLAYER FLAG
    // asm 00007F44: 	LDI	C_SLA,R0
    // asm 00007F45: 	TSTB	CMDP_MASTER,R1
    // asm 00007F46: 	LDIZ	C_MAS,R0
    // asm 00007F47: 	LS	16,R0
    // asm 00007F48: 	LDP	@COMMDP
    // asm 00007F49: 	STI	R0,@COMM_IO
    // asm 00007F4A: 	LDI	C_CE,R0
    // asm 00007F4B: 	LS	16,R0
    // asm 00007F4C: 	STI	R0,@COMM_CTL
    // asm 00007F4D: 	POP	DP
    // asm 00007F4E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_INIT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void COMM_ENABLE_INT2(void)
{
    // 	;
    // 	;if slave then enable the interrupt 2
    // 	;(comm int 2)
    // 	;
    // asm 00007F51: 	LDP	@DIPRAM
    // asm 00007F52: 	LDI	@DIPRAM,R1
    // asm 00007F53: 	TSTB	DIP_COMMP,R1
    // asm 00007F54: 	BNZ	BABA
    // asm 00007F55: 	TSTB	CMDP_MASTER,R1
    // asm 00007F56: 	BZ	BABA
    // asm 00007F57: 	LDI	INT2_M,R1
    // asm 00007F58: 	LDP	@COMMINTM
    // asm 00007F59: 	STI	R1,@COMMINTM
BABA:
    // asm 00007F5A: LDP	@COMMINTM
    // asm 00007F5B: 	OR	@COMMINTM,IE
    // asm 00007F5C: 	SETDP
    // asm 00007F5D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_ENABLE_INT2", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void COMM_MASTER_SEND_SYNC(void)
{
    // asm 00007F5E: 	LDI	@COMMFLAG,R0		;DONT INTERRUPT COMMUNICATIONS
    // asm 00007F5F: 	RETSNZ
    // asm 00007F60: 	LDI	@DIPRAM,R0
    // asm 00007F61: 	TSTB	CMDP_MASTER,R0
    // asm 00007F62: 	RETSNZ
    // asm 00007F63: 	DINT
    // asm 00007F69: 	ANDN	INT2_M,IE		;DISABLE LINK INT2
    // asm 00007F6A: 	LDI	@COMM_IOI,AR5		;SETUP I/O REGISTER
    // *TRIGGER INTERRUPT IN SLAVE
    // asm 00007F6B: 	LDI	*AR5,R0
    // asm 00007F6C: 	RS	16,R0
    // asm 00007F6D: 	AND	C_C2,R0
    // asm 00007F6E: 	LDI	R0,R1
    // asm 00007F6F: 	OR	C_MAS|C_IRQ,R0
    // asm 00007F70: 	LS	16,R0
    // asm 00007F71: 	STI	R0,*AR5			;INT2 HI COMM_IO
    // asm 00007F72: 	NOP
    // asm 00007F73: 	NOP
    // asm 00007F74: 	NOP
    // asm 00007F75: 	LDI	C_MAS,R0
    // asm 00007F76: 	OR	R1,R0
    // asm 00007F77: 	LS	16,R0
    // asm 00007F78: 	STI	R0,*AR5			;INT2 LO COMM_IO
    // asm 00007F79: 	ANDN	INT2_M,IF	   	;REMOVE ANY LATCHED INT
    // asm 00007F7A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_MASTER_SEND_SYNC", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*
*SET ONE PLAYER GAME (NO LINK)
*
*/
void SETONE(void)
{
    // asm 00007F7B: 	PUSH	R0
    // asm 00007F7C: 	PUSH	AR0
    // asm 00007F7D: 	CLRI	R0
    // asm 00007F7E: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm 00007F7F: 	LDI	1,R0
    // asm 00007F80: 	STI	R0,@ONEFLAG
    // asm 00007F81: 	LDI	@DIPRAM,R0
    // asm 00007F82: 	TSTB	CMDP_MASTER,R0
    // asm 00007F83: 	LDIZ	C_C2|C_MAS,R0	;MASTER CASE SEND C2=1
    // asm 00007F84: 	LDINZ	C_C1|C_SLA,R0	;SLAVE CASE SEND C1=1
    // asm 00007F85: 	B	ONEX
    // *
    // *CLR ONE PLAYER GAME (NO LINK)
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETONE", 0, 0);
    UNIMPL();
}

void CLRONE(void)
{
    // asm 00007F86: 	PUSH	R0
    // asm 00007F87: 	PUSH	AR0
    // asm 00007F88: 	LDI	1,R0
    // asm 00007F89: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm 00007F8A: 	LDI	0,R0
    // asm 00007F8B: 	STI	R0,@ONEFLAG
    // asm 00007F8C: 	LDI	@DIPRAM,R0
    // asm 00007F8D: 	TSTB	CMDP_MASTER,R0
    // asm 00007F8E: 	LDIZ	C_MAS,R0	;MASTER CASE
    // asm 00007F8F: 	LDINZ	C_SLA,R0	;SLAVE CASE
ONEX:
    // asm 00007F90: 	LDI	@COMM_IOI,AR0 	;SETUP I/O REGISTER
    // asm 00007F91: 	LS	16,R0
    // asm 00007F92: 	STI	R0,*AR0
    // asm 00007F93: 	POP	AR0
    // asm 00007F94: 	POP	R0
    // asm 00007F95: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRONE", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*COMMUNICATIONS LINK
*
*/
#define COMMPAL 0x0990000 //COMMUNICATIONS PAL

void COMM_ROUTINE(void)
{
    // asm 00007F96: 	CLRI	AR7
    // asm 00007F97: 	LDP	@COMMPAL
    // asm 00007F98: 	LDI	@COMMPAL,R0
    // asm 00007F99: 	LDI	*AR7,R1
    // asm 00007F9A: 	SETDP
    // asm 00007F9B: 	AND	0FH,R0
    // asm 00007F9C: 	CMPI	4,R0			;BAD PAL ?
    // asm 00007F9D: 	RETSNZ				;YES....QUIT
    // asm 00007F9E: 	LDI	@DIPRAM,R0		;CHECK FOR COMMUNICATIONS
    // asm 00007F9F: 	TSTB	DIP_COMMP,R0
    // asm 00007FA0: 	RETSNZ				;NO
COMM_HOLDFORA2D:
    // asm 00007FA1: 	LDI	@RDPOT,R1
    // asm 00007FA2: 	CMPI	3,R1
    // asm 00007FA3: 	BNZ	COMM_HOLDFORA2D
    // asm 00007FA4: 	LDI	1,R1
    // asm 00007FA5: 	STI	R1,@COMMFLAG		;DONT INTERRUPT COMMUNICATIONS
    // asm 00007FA6: 	TSTB	CMDP_MASTER,R0	   	;CHECK MASTER/SLAVE
    // asm 00007FA7: 	BNZ	COMM_SLAVE
    // *
    // *MASTER MUST ALWAYS 0 <- C_IRQE
    // *
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_ROUTINE", 0, 0);
    UNIMPL();
}

void COMM_MASTER(void)
{
    // asm 00007FA8: 	SETDP
    // asm 00007FA9: 	LDI	@COMM_IOI,AR5		;SETUP I/O REGISTER
    // *CHECK ONE PLAYER MODE
    // asm 00007FAA: 	LDI	@ONEFLAG,R0
    // asm 00007FAB: 	BZ	CM1
    // asm 00007FAC: 	RETS		      		;WE'RE IN ONE PLAYER, EXIT
CM1:
    // asm 00007FAD: 	LDI	@SEND_BUFFER_A_LEN,R6	;16 bits
    // asm 00007FAE: 	LDI	@SEND_BUFFER_AI,AR2
    // asm 00007FAF: 	CMPI	0,R6
    // asm 00007FB0: 	BLE	CMERRORL		;ZERO LENGTH, NO MESSAGE
    // asm 00007FB1: 	CMPI	240H,R6
    // asm 00007FB2: 	BGE	CMERRORL		;LENGTH ERROR
    // asm 00007FB3:  	ADDI	R6,AR2,AR0		;ADD 2 NULLS TO THE END DUDES
    // asm 00007FB4: 	LDI	CB_NULL,R2
    // asm 00007FB5: 	STI	R2,*AR0
    // asm 00007FB6: 	STI	R2,*+AR0(1)
    // *BEGIN SYNC ROUTINE
    // asm 00007FB7: 	LDI	C_MAS,R0		;send C2=0
    // asm 00007FB8: 	LS	16,R0
    // asm 00007FB9: 	STI	R0,*AR5
    // asm 00007FBA: 	LDI	120,R3
    // asm 00007FBB: 	LDI	@TRANSMISSION_DEAD,R0 	;GET TIMEOUT VALUES
    // asm 00007FBC: 	BZ	WTLPMI
    // asm 00007FBD: 	LDI	1,R3			;WAIT A FRAME IF DEAD
    // asm 00007FBE: 	ADDI	@INFRAMES,R3
    // asm 00007FBF: 	CMPI	4,R3
    // asm 00007FC0: 	LDIGT	4,R3
WTLPMI:
    // asm 00007FC1: LDI	*AR5,R0	 		;GET C0,C1
    // asm 00007FC2: 	RS	16,R0
    // asm 00007FC3: 	TSTB	C_C1,R0			;SLAVE C1=1 (ONE PLAYER MODE)
    // asm 00007FC4: 	BZ	CM2			;NO
    // asm 00007FC5: 	LDI	0,R0			;SLAVE IN ONE PLAYER MODE, EXIT
    // asm 00007FC6: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm 00007FC7: 	STI	R0,@TRANSMISSION_DEAD
    // asm 00007FC8: 	RETS
CM2:
    // asm 00007FC9: 	TSTB	C_C0,R0	      		;WAIT FOR SLAVE READY SIGNAL
    // asm 00007FCA: 	BNZ	CM3			;GOT IT...
    // asm 00007FCB: 	CMPI	@INFRAMES,R3		;WAITING TOO LONG?
    // asm 00007FCC: 	BGT	WTLPMI			;NO, KEEP LOOPING
    // asm 00007FCD: 	B	CMERRORDEAD		;YES, ITS DEAD...
CM3:
    // asm 00007FCE: 	LDI	0,R0
    // asm 00007FCF: 	STI	R0,@TRANSMISSION_DEAD
    // asm 00007FD0: 	LDI	1,R0
    // asm 00007FD1: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm 00007FD2: 	LDI	C_C2|C_MAS,R0	;SEND C2=1
    // asm 00007FD3: 	LS	16,R0
    // asm 00007FD4: 	STI	R0,*AR5
    // asm 00007FD5: 	ADDI	1,R3		;A LITTLE MORE TIME
WTLPMI2:
    // asm 00007FD6: 	CMPI	@INFRAMES,R3	;WAITING TOO LONG?
    // asm 00007FD7: 	BLE	CMERRORDEAD	;YES, ITS DEAD...
    // asm 00007FD8: 	LDI	*AR5,R0		;WAIT C0=0
    // asm 00007FD9: 	RS	16,R0
    // asm 00007FDA: 	TSTB	C_C0,R0
    // asm 00007FDB: 	BNZ	WTLPMI2
    // asm 00007FDC: 	LDI	C_MAS,R0	;send C2=0
    // asm 00007FDD: 	LS	16,R0
    // asm 00007FDE: 	STI	R0,*AR5
    // 	;
    // 	;END SYNC ROUTINE
    // asm 00007FDF: 	DINT
WTLPMI3:
    // asm 00007FE5: LDI	*AR5,R0	 	;WAIT C1=1
    // asm 00007FE6: 	RS	16,R0
    // asm 00007FE7: 	TSTB	C_C1,R0
    // asm 00007FE8: 	BZ	WTLPMI3
    // asm 00007FE9: 	LDI	C_MAS,R0
    // asm 00007FEA: 	LS	16,R0
    // asm 00007FEB: 	STI	R0,*AR5
    // asm 00007FEC: 	LDI	C_SND,R0		;SET DATA CONTROL FOR SEND
    // asm 00007FED: 	LS	16,R0
    // asm 00007FEE: 	STI	R0,*+AR5(CTL)		;COMM_CTL
    // *
    // *SEND MESSAGE LENGTH TO SLAVE
    // *16 BITS LENGTH
    // *MESSAGE LENGTH IS MULT OF 2
    // *
    // asm 00007FEF: 	ADDI	1,R6
    // asm 00007FF0: 	RS	1,R6
    // asm 00007FF1: 	LDI	R6,AR6
    // asm 00007FF2: 	NOT	R6,R7
    // asm 00007FF3: 	LDI	R7,R5
    // asm 00007FF4: 	LSH	-8,R7
    // asm 00007FF5: 	LDI	R6,R4
    // asm 00007FF6: 	LSH	-8,R6
    // asm 00007FF7: 	AND	0FFH,R7
    // asm 00007FF8: 	AND	0FFH,R6
    // asm 00007FF9: 	AND	0FFH,R5
    // asm 00007FFA: 	AND	0FFH,R4
    // *SEND LENGTH  (BYTE 1 MSB)
    // asm 00007FFB: 	OR	C_MAS|C_C2,R6	; (C2=1)
    // asm 00007FFC: 	LS	16,R6
    // asm 00007FFD: 	STI	R6,*AR5		;SEND C2 HIGH WITH DATA
    // asm 00007FFE: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 00007FFF: 	NOP
    // *SEND LENGTH  (BYTE 2 LSB)
    // asm 00008000: 	OR	C_MAS,R4	; (C2=1)
    // asm 00008001: 	LS	16,R4
    // asm 00008002: 	STI	R4,*AR5		;SEND C2 HIGH WITH DATA
    // asm 00008003: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 00008004: 	NOP
    // *SEND LENGTH  (BYTE 3 MSB COMPLEMENTED)
    // asm 00008005: 	OR	C_MAS|C_C2,R7	; (C2=1)
    // asm 00008006: 	LS	16,R7
    // asm 00008007: 	STI	R7,*AR5		;SEND C2 HIGH WITH DATA
    // asm 00008008: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 00008009: 	NOP
    // *SEND LENGTH  (BYTE 4 LSB COMPLEMENTED)
    // asm 0000800A: 	OR	C_MAS,R5
    // asm 0000800B: 	LS	16,R5
    // asm 0000800C: 	STI	R5,*AR5		;SEND C2 LOW WITH DATA
    // asm 0000800D: 	RPTS	65
    // asm 0000800E: 	NOP
    // asm 0000800F: 	DEC	AR6
    // asm 00008010: 	LDI	0FFh,R4
    // *
    // * SEND MASTERS BUFFER
    // *
    // asm 00008011: 	LDI	0,R3		;CHECKSUM
    // *byte1
    // asm 00008012: 	AND	*AR2++,R4,R6
    // asm 00008013: 	ADDI	R6,R3		;FORM CHECKSUM
    // asm 00008014: 	OR	C_MAS|C_C2,R6	;(C2=1)
    // asm 00008015: MSDLP
    // asm 00008015: 	LS	16,R6
    // asm 00008016: 	STI	R6,*AR5
    // asm 00008017: 	RPTS	TIME_DELAY
    // asm 00008018: 	NOP
    // *byte2
    // asm 00008019: 	AND	*AR2++,R4,R6
    // asm 0000801A: 	ADDI	R6,R3
    // asm 0000801B: 	OR	C_MAS,R6	;(C2=0)
    // asm 0000801C: 	LS	16,R6
    // asm 0000801D: 	STI	R6,*AR5
    // asm 0000801E: 	RPTS	TIME_DELAY
    // asm 0000801F: 	NOP
    // asm 00008020: 	DBUD	AR6,MSDLP
    // asm 00008021: 	AND	*AR2++,R4,R6
    // asm 00008022: 	ADDI	R6,R3		;FORM CHECKSUM
    // asm 00008023: 	OR	C_MAS|C_C2,R6	; (C2=1)
    // 	;---->DBUD	AR6,MSDLP
    // asm 00008024: 	ANDN	C_MAS|C_C2,R6	;ONE TOO MANY...
    // asm 00008025: 	SUBI	R6,R3
    // *SEND THE CHECKSUM
    // asm 00008026: 	AND	0FFFFH,R3	;MASK CKSUM TO 16 BITS
    // asm 00008027: 	LDI	R3,R6	     	;SEND MSB
    // asm 00008028: 	RS	8,R6
    // asm 00008029: 	OR	C_MAS|C_C2,R6	;(C2=1)
    // asm 0000802A: 	LS	16,R6
    // asm 0000802B: 	STI	R6,*AR5		;SEND 2 HIGH
    // asm 0000802C: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 0000802D: 	NOP
    // asm 0000802E: 	LS	24,R3
    // asm 0000802F: 	RS	24,R3
    // asm 00008030: 	OR	C_MAS,R3	;(C2=0)
    // asm 00008031: 	LS	16,R3
    // asm 00008032: 	STI	R3,*AR5		;SEND 2 HIGH
    // asm 00008033: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 00008034: 	NOP
    // asm 00008035: 	LDI	C_RCV,R0	;CHANGE CONTROL TO RECEIVE
    // asm 00008036: 	LS	16,R0
    // asm 00008037: 	STI	R0,*+AR5(CTL)	;@COMM_CTL
    // *
    // * WAIT FOR SLAVE TO SIGNIFY DONE (C1=0)
    // *
    // asm 00008038: 	LDI	30,R5			;TIMEOUT
CML1XX:
    // asm 00008039: DEC	R5
    // asm 0000803A: 	BLT	CMERROR10
    // asm 0000803B: 	LDI	*AR5,R0
    // asm 0000803C: 	RS	16,R0
    // asm 0000803D: 	TSTB	C_C1,R0
    // asm 0000803E: 	BNZ	CML1XX
    // *
    // * MASTER TELLS SLAVE IT IS LISTENING (C2=1)
    // *
    // asm 0000803F: 	LDI	@RECEIVE_BUFFERI,AR2 	;SETUP RECEIVE BUFFER
    // asm 00008040: 	LDI	C_C2|C_MAS,R0  		;SET C2=HI
    // asm 00008041: 	LS	16,R0
    // asm 00008042: 	STI	R0,*AR5
    // * GET SLAVES BUFFER LENGTH
    // * BUFFER LENGTH IS X 2
    // * 4 BYTES LENGTH MSB, LSB, COMP MSB, COMP LSB
    // *
    // asm 00008043: 	LDP	COMM_IO
    // asm 00008044: 	LDI	C_C1,R5
    // asm 00008045: 	LS	16,R5
    // asm 00008046: 	LDI	300,AR3
PWL2M:
    // asm 00008047: TSTB	@COMM_IO,R5
    // asm 00008048: 	DBZ	AR3,PWL2M
    // asm 00008049: 	TSTB	8000H,AR3
    // asm 0000804A: 	BNZD	CMERROR1		;ERROR IN TIMEOUT
    // asm 0000804B: 	NOP
    // asm 0000804C: 	LDI	300,AR3
    // asm 0000804D: 	LDI	@COMM_IO,R7	    	;GET MSB
    // 	;---->BNZD	CMERROR1
PWL3M:
    // asm 0000804E: TSTB	@COMM_IO,R5
    // asm 0000804F: 	DBNZ	AR3,PWL3M
    // asm 00008050: 	TSTB	8000H,AR3
    // asm 00008051: 	BNZD	CMERROR2		;ERROR IN TIMEOUT
    // asm 00008052: 	NOP
    // asm 00008053: 	LDI	@COMM_IO,R6		;GET LSB
    // asm 00008054: 	LS	8,R6
    // 	;---->BNZD	CMERROR2
    // asm 00008055: 	LDI	300,AR3
PWL4M:
    // asm 00008056: TSTB	@COMM_IO,R5
    // asm 00008057: 	DBZ	AR3,PWL4M
    // asm 00008058: 	TSTB	8000H,AR3
    // asm 00008059: 	BNZD	CMERROR1		;ERROR IN TIMEOUT
    // asm 0000805A: 	NOP
    // asm 0000805B: 	LDI	300,AR3
    // asm 0000805C: 	NOT	@COMM_IO,R4		;GET MSB COMPLEMENT
    // 	;---->BNZD	CMERROR1
PWL5M:
    // asm 0000805D: TSTB	@COMM_IO,R5
    // asm 0000805E: 	DBNZ	AR3,PWL5M
    // asm 0000805F: 	TSTB	8000H,AR3
    // asm 00008060: 	BNZD	CMERROR2		;ERROR IN TIMEOUT
    // asm 00008061: 	NOP
    // asm 00008062: 	NOT	@COMM_IO,R2		;GET LSB COMPLEMENT
    // asm 00008063: 	LS	8,R2
    // 	;---->BNZD	CMERROR2
    // asm 00008064: 	LS	8,R4
    // asm 00008065: 	LS	8,R7
    // asm 00008066: 	RS	24,R2
    // asm 00008067: 	RS	24,R4
    // asm 00008068: 	RS	24,R6
    // asm 00008069: 	RS	24,R7
    // asm 0000806A: 	CMPI	R2,R6
    // asm 0000806B: 	BNE	CMERROR3		;ERROR IN LENGTH
    // asm 0000806C: 	CMPI	R4,R7
    // asm 0000806D: 	BNE	CMERROR3		;ERROR IN LENGTH
    // asm 0000806E: 	LSH	8,R7
    // asm 0000806F: 	ADDI	R6,R7,AR6
    // asm 00008070: 	CMPI	140H,AR6		;LENGTH ERROR
    // asm 00008071: 	BGE	CMERROR3		;TOO LONG
    // asm 00008072: 	CMPI	0,AR6			;SLAVE LENGTH ZERO...
    // asm 00008073: 	BEQ	CMERROR3
    // asm 00008074: 	DEC	AR6	 		;THIS IS THE LENGTH INDEX
    // *
    // * GET SLAVES BUFFER
    // *
    // asm 00008075: 	SETDP
    // asm 00008076: 	STI	AR6,@RBUFF_LEN
    // asm 00008077: 	LDP	@COMM_IO
    // asm 00008078: 	CLRI	R3			;RESET CKSUM
    // asm 00008079: 	LDI	0FFH,R2
    // asm 0000807A: 	LSH	16,R2
    // asm 0000807B: 	LDI	300,AR3
WL2M:
    // asm 0000807C: TSTB	@COMM_IO,R5
    // asm 0000807D: 	DBZ	AR3,WL2M
    // asm 0000807E: 	TSTB	8000H,AR3
    // asm 0000807F: 	BNZD	CMERROR4		;YES...QUIT
    // asm 00008080: 	LDI	300,AR3
    // asm 00008081: 	NOP
    // asm 00008082: 	AND	R2,*AR5,R6
    // 	;---->	BNZD	CMERROR4
    // asm 00008083: 	ADDI	R6,R3
    // asm 00008084: 	STI	R6,*AR2++
WL3M:
    // asm 00008085: TSTB	*AR5,R5
    // asm 00008086: 	DBNZ	AR3,WL3M
    // asm 00008087: 	TSTB	8000H,AR3
    // asm 00008088: 	BNZD	CMERROR5		;YES...QUIT
    // asm 00008089: 	NOP
    // asm 0000808A: 	NOP
    // asm 0000808B: 	AND	R2,*AR5,R6
    // 	;---->	BNZD	CMERROR5
    // asm 0000808C: 	DBUD	AR6,WL2M
    // asm 0000808D: 	ADDI	R6,R3
    // asm 0000808E: 	STI	R6,*AR2++
    // asm 0000808F: 	LDI	300,AR3
    // 	;---->DBUD	AR6,WL2M
    // *GET SLAVES CKSUM
WL2MC:
    // asm 00008090: TSTB	@COMM_IO,R5
    // asm 00008091: 	DBZ	AR3,WL2MC
    // asm 00008092: 	TSTB	8000H,AR3
    // asm 00008093: 	BNZD	CMERROR6	       	;YES...QUIT
    // asm 00008094: 	NOP
    // asm 00008095: 	LDI	300,AR3
    // asm 00008096: 	LDI	@COMM_IO,R6		;MSB CKSUM
    // 	;---->	BNZD	CMERROR6
WL3MC:
    // asm 00008097: TSTB	@COMM_IO,R5
    // asm 00008098: 	DBNZ	AR3,WL3MC
    // asm 00008099: 	TSTB	8000H,AR3
    // asm 0000809A: 	BNZD	CMERROR7	       	;YES...QUIT
    // asm 0000809B: 	RS	8,R6
    // asm 0000809C: 	AND	0FF00h,R6
    // asm 0000809D: 	LDI	@COMM_IO,R7		;LSB CKSUM
    // 	;---->	BNZD	CMERROR7
    // asm 0000809E: 	RS	16,R7
    // asm 0000809F: 	AND	0FFh,R7
    // asm 000080A0: 	ADDI	R6,R7
    // asm 000080A1: 	RS	16,R3
    // asm 000080A2: 	CMPI	R7,R3
    // asm 000080A3: 	BNE	CMERROR8	       	;CKSUM ERROR
    // asm 000080A4: M0LENGTH
    // asm 000080A4: COMM_MASTER_X
    // asm 000080A4: 	LDP	@COMM_MASTER_TRANSES
    // asm 000080A5: 	INCM	@COMM_MASTER_TRANSES
COMM_MASTER_ERR_X:
    // asm 000080A8: 	LDI	C_MAS,R0	      	;SET C2=0
    // asm 000080A9: 	LS	16,R0
    // asm 000080AA: 	STI	R0,*AR5
    // asm 000080AB: 	LDI	C_RCV,R0	      	;SET DATA CONTROL TO RECEIVE
    // asm 000080AC: 	LS	16,R0
    // asm 000080AD: 	STI	R0,*+AR5(CTL)
    // asm 000080AE: 	SETDP
    // asm 000080AF: 	LDI	0,R0
    // asm 000080B0: 	STI	R0,@COMMFLAG		;COMMUNICATIONS IS OVER
    // asm 000080B1: 	EINT
    // asm 000080B2: 	RETS
CMERROR1:
    // asm 000080B3: B	COMM_MASTER_ERROR   	;LENGTH TIMEOUT
CMERROR2:
    // asm 000080B4: B	COMM_MASTER_ERROR	;LENGTH TIMEOUT
CMERROR3:
    // asm 000080B5: B	COMM_MASTER_ERROR	;BAD LENGTH
CMERROR4:
    // asm 000080B6: B	COMM_MASTER_ERROR	;DATA TIMEOUT
CMERROR5:
    // asm 000080B7: B	COMM_MASTER_ERROR	;DATA TIMEOUT
CMERROR6:
    // asm 000080B8: B	COMM_MASTER_ERROR	;CKSUM TIMEOUT
CMERROR7:
    // asm 000080B9: B	COMM_MASTER_ERROR	;CKSUM TIMEOUT
CMERROR8:
    // asm 000080BA: B	COMM_MASTER_ERROR	;BAD CKSUM
CMERROR9:
    // asm 000080BB: B	COMM_MASTER_ERROR	;MASTER SEND TIMEOUT
CMERROR10:
    // asm 000080BC: B	COMM_MASTER_ERROR	;MASTER END TIMEOUT (SLAVE ERR)
CMERRORL:
    // asm 000080BD: B	COMM_MASTER_ERROR	;LENGTH TOO LONG
CMERRORDEAD:
    // asm 000080BE: 		LDI	1,R0		      	;YES OTHER GAME IS DEAD, LEAVE...
    // asm 000080BF: 		STI	R0,@TRANSMISSION_DEAD
    // asm 000080C0: 		B	COMM_MASTER_ERROR	;WENT DEAD...
COMM_MASTER_ERROR:
    // asm 000080C1: 	LDP	@COMM_MASTER_TRANSES
    // asm 000080C2: 	INCM	@COMM_MASTER_ERROR_CNT
    // asm 000080C5: 	SETDP
    // asm 000080C6: 	LDI	0,R0
    // asm 000080C7: 	STI	R0,@RBUFF_LEN	   	;RECEIVE BUFFER CLEARED	ON ERROR
    // ;	LDI	C_C2|C_MAS,R0  		;SET C2=HI
    // ;	LS	16,R0
    // ;	STI	R0,*AR5
    // asm 000080C8: 	B	COMM_MASTER_ERR_X
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_MASTER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void COMM_IRQ(void)
{
    // asm 000080C9: 	PUSH	ST
    // asm 000080CA: 	PUSH	DP
    // asm 000080CB: 	PUSH	R0
    // asm 000080CC: 	ANDN	INT2_M,IF
    // 	;determine if I am a master being interrupted
    // 	;
    // asm 000080CD: 	LDP	@DIPRAM
    // asm 000080CE: 	LDI	@DIPRAM,R0
    // asm 000080CF: 	TSTB	CMDP_MASTER,R0
    // asm 000080D0: 	BNZ	DO_SLAVE_SYNC
    // asm 000080D1: 	POP	R0
    // asm 000080D2: 	POP	DP
    // asm 000080D3: 	POP	ST
    // asm 000080D4: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_IRQ", 0, 0);
    UNIMPL();
}

void DO_SLAVE_SYNC(void)
{
    // asm 000080D5: 	PUSH	R1
    // asm 000080D6: 	PUSH	IE
    // asm 000080D7: 	LDI	INT1_M|INT2_M,IE	;disable everything except TV30 interrupt & comm int
    // asm 000080D8: 	ANDN	INT2_M,IF		;we wont irq ourself
    // asm 000080D9: 	LDP	@CPU_WS
    // asm 000080DA: 	LDI	@CPU_WS,R0
    // asm 000080DB: 	PUSH	R0
    // asm 000080DC: 	LDI	SOFT_WS,R0
    // asm 000080DD: 	STI	R0,@CPU_WS
    // 	;sync the systems...
    // 	;
    // 	;
    // asm 000080DE: 	LDP	@FIFO_CONTROL
    // asm 000080DF: 	LDI	@FIFO_CONTROL,R0
    // asm 000080E0: 	ANDN	FIFO_CONTROL_DMA_RUNSEL,R0
    // asm 000080E1: 	STI	R0,@FIFO_CONTROL
WTLP:
    // asm 000080E2: LDI	@FIFO_STATUS,R0
    // asm 000080E3: 	AND	FIFO_STATUS_FD_CRITICAL,R0
    // asm 000080E4: 	BNZ	WTLP
    // ;same	LDP	@CRT_VCNT
    // asm 000080E5: 	LDI	@CRT_VCNT,R0
    // asm 000080E6: 	AND	1FFh,R0
    // ;	CMPI	400,R0
    // ;	LDIEQ	1b0h,R0
    // ;	LDIGT	1b1h,R0
    // ;	LDILT	1afh,R0
    // asm 000080E7: 	LDI	1B0H,R1
    // asm 000080E8: 	SUBI	400,R0
    // asm 000080E9: 	CMPI	-1,R0	 			;GRACE AREA DUDES
    // asm 000080EA: 	LDILT	1afh,R1
    // asm 000080EB: 	CMPI	1,R0
    // asm 000080EC: 	LDIGT	1b1h,R1
    // asm 000080ED: 	STI	R1,@CRT_VTTL
    // asm 000080EE: 	LDI	@FIFO_CONTROL,R0
    // asm 000080EF: 	OR	FIFO_CONTROL_DMA_RUNSEL,R0
    // asm 000080F0: 	STI	R0,@FIFO_CONTROL
    // asm 000080F1: 	POP	R0
    // asm 000080F2: 	LDP	@CPU_WS
    // asm 000080F3: 	STI	R0,@CPU_WS
    // asm 000080F4: 	POP	IE
    // asm 000080F5: 	POP	R1
    // asm 000080F6: 	POP	R0
    // asm 000080F7: 	POP	DP
    // ;	LDI	INT1_M|INT2_M|INT3_M|INT0_M,IE
    // asm 000080F8: 	POP	ST
    // asm 000080F9: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DO_SLAVE_SYNC", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*CALLED FROM THE MAIN LOOP
*
*SET C0 HIGH
*WAIT FOR C2 TO GO HIGH
*BEGIN TRANSFER
*END OF TRANSFER
*SET ALL LOW
*
*
*/
void COMM_SLAVE(void)
{
    // asm 000080FA: 	SETDP
    // asm 000080FB: 	LDI	@COMM_IOI,AR5		;AR5=COMM I/O ADDRESS
    // asm 000080FC: 	LDI	@RECEIVE_BUFFERI,AR2	;AR2=RECEIVE BUFFER
    // asm 000080FD: 	LDI	C_C2,R5			;R5=C_C2 MASK
    // asm 000080FE: 	LS	16,R5
    // *CHECK ONE PLAYER MODE
    // asm 000080FF: 	LDI	@ONEFLAG,R0
    // asm 00008100: 	BZ	CS1
    // asm 00008101: 	RETS				;WE'RE IN ONE PLAYER, EXIT
CS1:
    // asm 00008102: 	TSTB	*AR5,R5
    // asm 00008103: 	BZ	CS2			;C2=1, MASTER IN ONE PLAYER, EXIT
    // asm 00008104: 	LDI	0,R0
    // asm 00008105: 	STI	R0,@TRANSMISSION_DEAD
    // asm 00008106: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm 00008107: 	RETS
    // *SYNC UP
CS2:
    // asm 00008108: 	LDI	120,R3			;WAIT 120 FRAMES IF NOT DEAD
    // asm 00008109: 	LDI	@TRANSMISSION_DEAD,R0 	;GET TIMEOUT VALUES
    // asm 0000810A: 	BZ	CS20
    // asm 0000810B: 	LDI	1,R3			;WAIT 1 IF DEAD
    // asm 0000810C: 	ADDI	@INFRAMES,R3
    // asm 0000810D: 	CMPI	4,R3
    // asm 0000810E: 	LDIGT	4,R3
CS20:
    // asm 0000810F: 	LDI	1,R0
    // asm 00008110: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm 00008111: 	LDI	C_C0|C_SLA,R0
    // asm 00008112: 	LS	16,R0
    // asm 00008113: 	STI	R0,*AR5			;SEND C0=1
WTLPI:
    // asm 00008114: 	CMPI	@INFRAMES,R3		;WAITING TOO LONG?
    // asm 00008115: 	BLE	CSERRORDEAD		;YES, ITS DEAD...
    // asm 00008116: 	TSTB	*AR5,R5			;WAIT C2=1
    // asm 00008117: 	BZ	WTLPI
    // asm 00008118: 	LDI	C_SLA,R0		;SEND C0=0
    // asm 00008119: 	LS	16,R0
    // asm 0000811A: 	STI	R0,*AR5
    // asm 0000811B: 	LDI	0,R0
    // asm 0000811C: 	STI	R0,@TRANSMISSION_DEAD
    // asm 0000811D: 	LDI	10000,AR3   		;TIMEOUT VALUE
    // asm 0000811E: WTLPI2
    // asm 0000811E: 	TSTB	*AR5,R5			;WAIT C2=0
    // asm 0000811F: 	DBNZ	AR3,WTLPI2
    // asm 00008120: 	TSTB	8000H,AR3		;TIMEOUT?
    // asm 00008121: 	BZ	CS3			;NO
    // asm 00008122: 	RETS				;YES, LEAVE
    // *END SYNC
CS3:
    // asm 00008123: 	DINT
    // *SET DATA CONTROL TO RECEIVE		;CM_A 1
    // asm 00008129: 	LDI	C_RCV,R0
    // asm 0000812A: 	LS	16,R0
    // asm 0000812B: 	STI	R0,*+AR5(CTL)
    // *TELL MASTER WE ARE LISTENING (C0=0,C1=1)
    // asm 0000812C: 	LDI	C_C1|C_SLA,R0
    // asm 0000812D: 	LS	16,R0
    // asm 0000812E: 	STI	R0,*AR5
    // *GET MASTERS BUFFER LENGTH
    // *BUFFER LENGTH IS X 2
    // *FIRST BYTE IS LENGTH
    // *SECOND BYTE IS COMPLEMENT OF LENGTH
    // asm 0000812F: 	LDP	@COMM_IO
    // asm 00008130: 	LDI	300,AR3
PWL2:
    // asm 00008131: TSTB	@COMM_IO,R5
    // asm 00008132: 	DBZ	AR3,PWL2
    // asm 00008133: 	TSTB	8000H,AR3
    // asm 00008134: 	BNZD	CSERROR1		;ERROR IN TIMEOUT
    // asm 00008135: 	NOP
    // asm 00008136: 	LDI	300,AR3
    // asm 00008137: 	LDI	@COMM_IO,R7	    	;GET MSB
    // 	;---->BNZD	CSERROR1
PWL3:
    // asm 00008138: TSTB	@COMM_IO,R5
    // asm 00008139: 	DBNZ	AR3,PWL3
    // asm 0000813A: 	TSTB	8000H,AR3
    // asm 0000813B: 	BNZD	CSERROR2		;ERROR IN TIMEOUT
    // asm 0000813C: 	NOP
    // asm 0000813D: 	LDI	@COMM_IO,R6		;GET LSB
    // asm 0000813E: 	LS	8,R6
    // 	;---->BNZD	CSERROR2
    // asm 0000813F: 	LDI	300,AR3
PWL4:
    // asm 00008140: TSTB	@COMM_IO,R5
    // asm 00008141: 	DBZ	AR3,PWL4
    // asm 00008142: 	TSTB	8000H,AR3
    // asm 00008143: 	BNZD	CSERROR1		;ERROR IN TIMEOUT
    // asm 00008144: 	NOP
    // asm 00008145: 	LDI	300,AR3
    // asm 00008146: 	NOT	@COMM_IO,R4		;GET MSB COMPLEMENT
    // 	;---->BNZD	CSERROR1
PWL5:
    // asm 00008147: TSTB	@COMM_IO,R5
    // asm 00008148: 	DBNZ	AR3,PWL5
    // asm 00008149: 	TSTB	8000H,AR3
    // asm 0000814A: 	BNZD	CSERROR2		;ERROR IN TIMEOUT
    // asm 0000814B: 	NOP
    // asm 0000814C: 	NOT	@COMM_IO,R2		;GET LSB COMPLEMENT
    // asm 0000814D: 	LS	8,R2
    // 	;---->BND	CSERROR2
    // asm 0000814E: 	LS	8,R4
    // asm 0000814F: 	LS	8,R7
    // asm 00008150: 	RS	24,R2
    // asm 00008151: 	RS	24,R4
    // asm 00008152: 	RS	24,R6
    // asm 00008153: 	RS	24,R7
    // asm 00008154: 	CMPI	R2,R6
    // asm 00008155: 	BNE	CSERROR3		;ERROR IN LENGTH
    // asm 00008156: 	CMPI	R4,R7
    // asm 00008157: 	BNE	CSERROR3		;ERROR IN LENGTH
    // asm 00008158: 	LSH	8,R7
    // asm 00008159: 	ADDI	R6,R7,AR6
    // asm 0000815A: 	DEC	AR6	 		;THIS IS THE LENGTH INDEX
    // asm 0000815B: 	CMPI	140H,AR6		;LENGTH ERROR
    // asm 0000815C: 	BGE	CSERROR3		;TOO LONG
    // *
    // *GET MASTERS BUFFER
    // *
    // asm 0000815D: 	SETDP
    // asm 0000815E: 	STI	AR6,@RBUFF_LEN
    // asm 0000815F: 	LDP	@COMM_IO
    // asm 00008160: 	CLRI	R3			;RESET CKSUM
    // asm 00008161: 	LDI	0FFH,R2
    // asm 00008162: 	LSH	16,R2
    // asm 00008163: 	LDI	300,AR3
WL2:
    // asm 00008164: TSTB	@COMM_IO,R5
    // asm 00008165: 	DBZ	AR3,WL2
    // asm 00008166: 	TSTB	8000H,AR3
    // asm 00008167: 	BNZD	CSERROR4		;YES...QUIT
    // asm 00008168: 	LDI	300,AR3
    // asm 00008169: 	NOP
    // asm 0000816A: 	AND	R2,*AR5,R6
    // 	;---->	BNZD	CSERROR4		;YES...QUIT
    // asm 0000816B: 	ADDI	R6,R3
    // asm 0000816C: 	STI	R6,*AR2++
WL3:
    // asm 0000816D: TSTB	*AR5,R5
    // asm 0000816E: 	DBNZ	AR3,WL3
    // asm 0000816F: 	TSTB	8000H,AR3
    // asm 00008170: 	BNZD	CSERROR5		;YES...QUIT
    // asm 00008171: 	NOP
    // asm 00008172: 	NOP
    // asm 00008173: 	AND	R2,*AR5,R6
    // 	;---->	BNZD	CSERROR5		;YES...QUIT
    // asm 00008174: 	DBUD	AR6,WL2
    // asm 00008175: 	ADDI	R6,R3
    // asm 00008176: 	STI	R6,*AR2++
    // asm 00008177: 	LDI	300,AR3
    // *GET MASTERS CKSUM
WL2C:
    // asm 00008178: TSTB	@COMM_IO,R5
    // asm 00008179: 	DBZ	AR3,WL2C
    // asm 0000817A: 	TSTB	8000H,AR3
    // asm 0000817B: 	BNZD	CSERROR6	       	;YES...QUIT
    // asm 0000817C: 	NOP
    // asm 0000817D: 	LDI	300,AR3
    // asm 0000817E: 	LDI	@COMM_IO,R6		;MSB CKSUM
    // 	;---->	BNZD	CSERROR6	       	;YES...QUIT
WL3C:
    // asm 0000817F: TSTB	@COMM_IO,R5
    // asm 00008180: 	DBNZ	AR3,WL3C
    // asm 00008181: 	TSTB	8000H,AR3
    // asm 00008182: 	BNZD	CSERROR7	       	;YES...QUIT
    // asm 00008183: 	RS	8,R6
    // asm 00008184: 	AND	0FF00h,R6
    // asm 00008185: 	LDI	@COMM_IO,R7		;LSB CKSUM
    // 	;---->	BNZD	CSERROR7	       	;YES...QUIT
    // asm 00008186: 	RS	16,R7
    // asm 00008187: 	AND	0FFh,R7
    // asm 00008188: 	ADDI	R6,R7
    // asm 00008189: 	RS	16,R3
    // asm 0000818A: 	CMPI	R7,R3
    // asm 0000818B: 	BNE	CSERROR8	       	;CKSUM ERROR
    // *	LOWER SLAVES C1
    // *	(tell master we are prepared to send)
    // *
    // asm 0000818C: 	SETDP
    // asm 0000818D: 	LDI	C_SLA,R0
    // asm 0000818E: 	LS	16,R0
    // asm 0000818F: 	STI	R0,*AR5
    // *
    // * SLAVE SEND
    // *
    // * WAIT FOR MASTER TO LISTEN (C2=1)
    // asm 00008190: 	LDI	50,R5			;TIMEOUT
CML1S:
    // asm 00008191: DEC	R5
    // asm 00008192: 	BLT	CSERROR9
    // asm 00008193: 	LDI	*AR5,R0
    // asm 00008194: 	RS	16,R0
    // asm 00008195: 	TSTB	C_C2,R0
    // asm 00008196: 	BZ	CML1S
    // asm 00008197: 	LDI	C_SND,R0		;CHANGE DATA BUFFER DIRECTION TO SEND
    // asm 00008198: 	LS	16,R0
    // asm 00008199: 	STI	R0,*+AR5(CTL)
    // * SEND SLAVE LENGTH
    // *SEND MESSAGE LENGTH TO MASTER
    // *MESSAGE LENGTH IS MULTIPLE OF 2
    // asm 0000819A: 	SETDP
    // asm 0000819B: 	LDI	@SEND_BUFFER_A_LEN,R6
    // asm 0000819C: 	LDI	@SEND_BUFFER_AI,AR2
    // asm 0000819D:  	ADDI	R6,AR2,AR0	;ADD 2 NULLS TO THE END DUDES
    // asm 0000819E: 	LDI	CB_NULL,R2
    // asm 0000819F: 	STI	R2,*AR0
    // asm 000081A0: 	STI	R2,*+AR0(1)
    // asm 000081A1: 	ADDI	1,R6
    // asm 000081A2: 	RS	1,R6
    // asm 000081A3: 	LDI	R6,AR6
    // asm 000081A4: 	NOT	R6,R7
    // asm 000081A5: 	LDI	R7,R5
    // asm 000081A6: 	LSH	-8,R7
    // asm 000081A7: 	LDI	R6,R4
    // asm 000081A8: 	LSH	-8,R6
    // asm 000081A9: 	AND	0FFH,R7
    // asm 000081AA: 	AND	0FFH,R6
    // asm 000081AB: 	AND	0FFH,R5
    // asm 000081AC: 	AND	0FFH,R4
    // *SEND LENGTH  (BYTE 1 MSB)
    // asm 000081AD: 	OR	C_SLA|C_C1,R6	; (C2=1)
    // asm 000081AE: 	LS	16,R6
    // asm 000081AF: 	STI	R6,*AR5		;SEND C2 HIGH WITH DATA
    // asm 000081B0: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 000081B1: 	NOP
    // *SEND LENGTH  (BYTE 2 LSB)
    // asm 000081B2: 	OR	C_SLA,R4	; (C2=1)
    // asm 000081B3: 	LS	16,R4
    // asm 000081B4: 	STI	R4,*AR5		;SEND C2 HIGH WITH DATA
    // asm 000081B5: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 000081B6: 	NOP
    // *SEND LENGTH  (BYTE 3 MSB COMPLEMENTED)
    // asm 000081B7: 	OR	C_SLA|C_C1,R7	; (C2=1)
    // asm 000081B8: 	LS	16,R7
    // asm 000081B9: 	STI	R7,*AR5		;SEND C2 HIGH WITH DATA
    // asm 000081BA: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 000081BB: 	NOP
    // *SEND LENGTH  (BYTE 4 LSB COMPLEMENTED)
    // asm 000081BC: 	OR	C_SLA,R5
    // asm 000081BD: 	LS	16,R5
    // asm 000081BE: 	STI	R5,*AR5		;SEND C2 LOW WITH DATA
    // asm 000081BF: 	RPTS	65
    // asm 000081C0: 	NOP
    // asm 000081C1: 	DEC	AR6
    // asm 000081C2: 	LDI	0FFh,R4
    // *
    // * SLAVE SEND BUFFER TO MASTER
    // *
    // asm 000081C3: 	LDI	0,R3		;CHECKSUM
    // *byte1
    // asm 000081C4: 	AND	*AR2++,R4,R6
    // asm 000081C5: 	ADDI	R6,R3		;FORM CHECKSUM
    // asm 000081C6: 	OR	C_SLA|C_C1,R6	;(C1=1)
    // asm 000081C7: SSDLP
    // asm 000081C7: 	LS	16,R6
    // asm 000081C8: 	STI	R6,*AR5
    // asm 000081C9: 	RPTS	TIME_DELAY
    // asm 000081CA: 	NOP
    // *byte2
    // asm 000081CB: 	AND	*AR2++,R4,R6
    // asm 000081CC: 	ADDI	R6,R3
    // asm 000081CD: 	OR	C_SLA,R6	;(C1=0)
    // asm 000081CE: 	LS	16,R6
    // asm 000081CF: 	STI	R6,*AR5
    // asm 000081D0: 	RPTS	TIME_DELAY
    // asm 000081D1: 	NOP
    // asm 000081D2: 	DBUD	AR6,SSDLP
    // asm 000081D3: 	AND	*AR2++,R4,R6
    // asm 000081D4: 	ADDI	R6,R3		;FORM CHECKSUM
    // asm 000081D5: 	OR	C_SLA|C_C1,R6	; (C2=1)
    // 	;---->DBUD	AR6,SSDLP
    // asm 000081D6: 	ANDN	C_SLA|C_C1,R6
    // asm 000081D7: 	SUBI	R6,R3		;ONE OVER!!!
    // *SEND THE CHECKSUM
    // asm 000081D8: 	AND	0FFFFH,R3	;MASK CKSUM TO 16 BITS
    // asm 000081D9: 	LDI	R3,R6	     	;SEND MSB
    // asm 000081DA: 	RS	8,R6
    // asm 000081DB: 	OR	C_SLA|C_C1,R6	;(C1=1)
    // asm 000081DC: 	LS	16,R6
    // asm 000081DD: 	STI	R6,*AR5		;SEND DATA
    // asm 000081DE: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm 000081DF: 	NOP
    // asm 000081E0: 	LS	24,R3
    // asm 000081E1: 	RS	24,R3
    // asm 000081E2: 	OR	C_SLA,R3	;(C1=0)
    // asm 000081E3: 	LS	16,R3
    // asm 000081E4: 	STI	R3,*AR5		;SEND DATA
    // asm 000081E5: 	RPTS	TIME_DELAY    	;HOLD FOR A SEC
    // asm 000081E6: 	NOP
    // *WAIT FOR MASTER TO INDICATE O.K. C2=0
    // asm 000081E7: LENGTHOF0
    // asm 000081E7: 	LDI	50,R5	       	;TIMEOUT
CML1WT:
    // asm 000081E8: DEC	R5
    // asm 000081E9: 	BLT	CSERROR11
    // asm 000081EA: 	LDI	*AR5,R0
    // asm 000081EB: 	RS	16,R0
    // asm 000081EC: 	TSTB	C_C2,R0
    // asm 000081ED: 	BNZ	CML1WT
    // asm 000081EE: COMM_SLAVE_X
    // asm 000081EE: 	LDP	@COMM_SLAVE_TRANSES
    // asm 000081EF: 	INCM	@COMM_SLAVE_TRANSES
COMM_SLAVE_ERR_X:
    // asm 000081F2: 	SETDP
    // asm 000081F3: 	LDI	C_SLA,R0  		;SET C0=0,C1=0, NO ERROR
    // asm 000081F4: 	LS	16,R0
    // asm 000081F5: 	STI	R0,*AR5
    // asm 000081F6: 	LDI	C_RCV,R0		;CHANGE CONTROL TO RECEIVE
    // asm 000081F7: 	LS	16,R0
    // asm 000081F8: 	STI	R0,*+AR5(CTL)		;@COMM_CTL
    // asm 000081F9: 	LDI	0,R0
    // asm 000081FA: 	STI	R0,@COMMFLAG		;COMMUNICATIONS IS OVER
    // asm 000081FB: 	EINT
    // asm 000081FC: 	RETS
CSERROR1:
    // asm 000081FD: B COMM_SLAVE_ERROR ;LENGTH TIMEOUT
CSERROR2:
    // asm 000081FE: B COMM_SLAVE_ERROR ;LENGTH TIMEOUT
CSERROR3:
    // asm 000081FF: B COMM_SLAVE_ERROR ;BAD LENGTH
CSERROR4:
    // asm 00008200: B COMM_SLAVE_ERROR ;DATA TIMEOUT
CSERROR5:
    // asm 00008201: B COMM_SLAVE_ERROR ;DATA TIMEOUT
CSERROR6:
    // asm 00008202: B COMM_SLAVE_ERROR ;CKSUM TIMEOUT
CSERROR7:
    // asm 00008203: B COMM_SLAVE_ERROR ;CKSUM TIMEOUT
CSERROR8:
    // asm 00008204: B COMM_SLAVE_ERROR ;BAD CKSUM
CSERROR9:
    // asm 00008205: B COMM_SLAVE_ERROR ;SLAVE SEND TIMEOUT
CSERROR11:
    // asm 00008206: B COMM_SLAVE_ERROR ;MASTER ERROR END TIMEOUT
CSERRORDEAD:
    // asm 00008207: 		LDI	1,R0		      	;YES OTHER GAME IS DEAD, LEAVE...
    // asm 00008208: 		STI	R0,@TRANSMISSION_DEAD
    // asm 00008209: 		B	COMM_SLAVE_ERROR	;WENT DEAD...
COMM_SLAVE_ERROR:
    // asm 0000820A: 	LDP	@COMM_SLAVE_ERROR_CNT
    // asm 0000820B: 	INCM	@COMM_SLAVE_ERROR_CNT
    // asm 0000820E: 	SETDP
    // asm 0000820F: 	LDI	0,R0
    // asm 00008210: 	STI	R0,@RBUFF_LEN	   	;RECEIVE BUFFER CLEARED	ON ERROR
    // asm 00008211: 	BU	COMM_SLAVE_ERR_X
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_SLAVE", 0, 0);
    UNIMPL();
}

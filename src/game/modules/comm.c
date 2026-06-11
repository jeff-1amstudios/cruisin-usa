#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/obj.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/comm.h"
#include "../include/comm_defs.h"
#include "../include/commq.h"

/*
 * Source module: asm/COMM.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
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
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *COMMUNICATIONS LINK
// *
#define COMMPAL 0x0990000
// *----------------------------------------------------------------------------

void COMM_INIT(void)
{
    // asm: 	PUSH	DP
    // asm: 	LDP	@DIPRAM
    // asm: 	LDI	@DIPRAM,R1
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@ONEFLAG		;CLEAR OUT ONE PLAYER FLAG
    // asm: 	LDI	C_SLA,R0
    // asm: 	TSTB	CMDP_MASTER,R1
    // asm: 	LDIZ	C_MAS,R0
    // asm: 	LS	16,R0
    // asm: 	LDP	@COMMDP
    // asm: 	STI	R0,@COMM_IO
    // asm: 	LDI	C_CE,R0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,@COMM_CTL
    // asm: 	POP	DP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_INIT", 0, 0);
    UNIMPL();
}

void COMM_ENABLE_INT2(void)
{
    // *----------------------------------------------------------------------------
    // 	;
    // 	;if slave then enable the interrupt 2
    // 	;(comm int 2)
    // 	;
    // asm: 	LDP	@DIPRAM
    // asm: 	LDI	@DIPRAM,R1
    // asm: 	TSTB	DIP_COMMP,R1
    // asm: 	BNZ	BABA
    // asm: 	TSTB	CMDP_MASTER,R1
    // asm: 	BZ	BABA
    // asm: 	LDI	INT2_M,R1
    // asm: 	LDP	@COMMINTM
    // asm: 	STI	R1,@COMMINTM
    // asm: BABA	LDP	@COMMINTM
    // asm: 	OR	@COMMINTM,IE
    // asm: 	SETDP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_ENABLE_INT2", 0, 0);
    UNIMPL();
}

void COMM_MASTER_SEND_SYNC(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@COMMFLAG,R0		;DONT INTERRUPT COMMUNICATIONS
    // asm: 	RETSNZ
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	RETSNZ
    // asm: 	DINT
    // asm: 	ANDN	INT2_M,IE		;DISABLE LINK INT2
    // asm: 	LDI	@COMM_IOI,AR5		;SETUP I/O REGISTER
    // *TRIGGER INTERRUPT IN SLAVE
    // asm: 	LDI	*AR5,R0
    // asm: 	RS	16,R0
    // asm: 	AND	C_C2,R0
    // asm: 	LDI	R0,R1
    // asm: 	OR	C_MAS|C_IRQ,R0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5			;INT2 HI COMM_IO
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	LDI	C_MAS,R0
    // asm: 	OR	R1,R0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5			;INT2 LO COMM_IO
    // asm: 	ANDN	INT2_M,IF	   	;REMOVE ANY LATCHED INT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_MASTER_SEND_SYNC", 0, 0);
    UNIMPL();
}

void SETONE(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *SET ONE PLAYER GAME (NO LINK)
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@ONEFLAG
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	LDIZ	C_C2|C_MAS,R0	;MASTER CASE SEND C2=1
    // asm: 	LDINZ	C_C1|C_SLA,R0	;SLAVE CASE SEND C1=1
    // asm: 	B	ONEX
    // *
    // *CLR ONE PLAYER GAME (NO LINK)
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SETONE", 0, 0);
    UNIMPL();
}

void CLRONE(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@ONEFLAG
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	LDIZ	C_MAS,R0	;MASTER CASE
    // asm: 	LDINZ	C_SLA,R0	;SLAVE CASE
ONEX:
    // asm: 	LDI	@COMM_IOI,AR0 	;SETUP I/O REGISTER
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR0
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLRONE", 0, 0);
    UNIMPL();
}

void COMM_ROUTINE(void)
{
    // asm: 	CLRI	AR7
    // asm: 	LDP	@COMMPAL
    // asm: 	LDI	@COMMPAL,R0
    // asm: 	LDI	*AR7,R1
    // asm: 	SETDP
    // asm: 	AND	0FH,R0
    // asm: 	CMPI	4,R0			;BAD PAL ?
    // asm: 	RETSNZ				;YES....QUIT
    // asm: 	LDI	@DIPRAM,R0		;CHECK FOR COMMUNICATIONS
    // asm: 	TSTB	DIP_COMMP,R0
    // asm: 	RETSNZ				;NO
COMM_HOLDFORA2D:
    // asm: 	LDI	@RDPOT,R1
    // asm: 	CMPI	3,R1
    // asm: 	BNZ	COMM_HOLDFORA2D
    // asm: 	LDI	1,R1
    // asm: 	STI	R1,@COMMFLAG		;DONT INTERRUPT COMMUNICATIONS
    // asm: 	TSTB	CMDP_MASTER,R0	   	;CHECK MASTER/SLAVE
    // asm: 	BNZ	COMM_SLAVE
    // *
    // *MASTER MUST ALWAYS 0 <- C_IRQE
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_ROUTINE", 0, 0);
    UNIMPL();
}

void COMM_MASTER(void)
{
    // asm: 	SETDP
    // asm: 	LDI	@COMM_IOI,AR5		;SETUP I/O REGISTER
    // *CHECK ONE PLAYER MODE
    // asm: 	LDI	@ONEFLAG,R0
    // asm: 	BZ	CM1
    // asm: 	RETS		      		;WE'RE IN ONE PLAYER, EXIT
CM1:
    // asm: 	LDI	@SEND_BUFFER_A_LEN,R6	;16 bits
    // asm: 	LDI	@SEND_BUFFER_AI,AR2
    // asm: 	CMPI	0,R6
    // asm: 	BLE	CMERRORL		;ZERO LENGTH, NO MESSAGE
    // asm: 	CMPI	240H,R6
    // asm: 	BGE	CMERRORL		;LENGTH ERROR
    // asm:  	ADDI	R6,AR2,AR0		;ADD 2 NULLS TO THE END DUDES
    // asm: 	LDI	CB_NULL,R2
    // asm: 	STI	R2,*AR0
    // asm: 	STI	R2,*+AR0(1)
    // *BEGIN SYNC ROUTINE
    // asm: 	LDI	C_MAS,R0		;send C2=0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // asm: 	LDI	120,R3
    // asm: 	LDI	@TRANSMISSION_DEAD,R0 	;GET TIMEOUT VALUES
    // asm: 	BZ	WTLPMI
    // asm: 	LDI	1,R3			;WAIT A FRAME IF DEAD
    // asm: 	ADDI	@INFRAMES,R3
    // asm: 	CMPI	4,R3
    // asm: 	LDIGT	4,R3
WTLPMI:
    // asm: LDI	*AR5,R0	 		;GET C0,C1
    // asm: 	RS	16,R0
    // asm: 	TSTB	C_C1,R0			;SLAVE C1=1 (ONE PLAYER MODE)
    // asm: 	BZ	CM2			;NO
    // asm: 	LDI	0,R0			;SLAVE IN ONE PLAYER MODE, EXIT
    // asm: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm: 	STI	R0,@TRANSMISSION_DEAD
    // asm: 	RETS
CM2:
    // asm: 	TSTB	C_C0,R0	      		;WAIT FOR SLAVE READY SIGNAL
    // asm: 	BNZ	CM3			;GOT IT...
    // asm: 	CMPI	@INFRAMES,R3		;WAITING TOO LONG?
    // asm: 	BGT	WTLPMI			;NO, KEEP LOOPING
    // asm: 	B	CMERRORDEAD		;YES, ITS DEAD...
CM3:
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@TRANSMISSION_DEAD
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm: 	LDI	C_C2|C_MAS,R0	;SEND C2=1
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // asm: 	ADDI	1,R3		;A LITTLE MORE TIME
WTLPMI2:
    // asm: 	CMPI	@INFRAMES,R3	;WAITING TOO LONG?
    // asm: 	BLE	CMERRORDEAD	;YES, ITS DEAD...
    // asm: 	LDI	*AR5,R0		;WAIT C0=0
    // asm: 	RS	16,R0
    // asm: 	TSTB	C_C0,R0
    // asm: 	BNZ	WTLPMI2
    // asm: 	LDI	C_MAS,R0	;send C2=0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // 	;
    // 	;END SYNC ROUTINE
    // asm: 	DINT
WTLPMI3:
    // asm: LDI	*AR5,R0	 	;WAIT C1=1
    // asm: 	RS	16,R0
    // asm: 	TSTB	C_C1,R0
    // asm: 	BZ	WTLPMI3
    // asm: 	LDI	C_MAS,R0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // asm: 	LDI	C_SND,R0		;SET DATA CONTROL FOR SEND
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*+AR5(CTL)		;COMM_CTL
    // *
    // *SEND MESSAGE LENGTH TO SLAVE
    // *16 BITS LENGTH
    // *MESSAGE LENGTH IS MULT OF 2
    // *
    // asm: 	ADDI	1,R6
    // asm: 	RS	1,R6
    // asm: 	LDI	R6,AR6
    // asm: 	NOT	R6,R7
    // asm: 	LDI	R7,R5
    // asm: 	LSH	-8,R7
    // asm: 	LDI	R6,R4
    // asm: 	LSH	-8,R6
    // asm: 	AND	0FFH,R7
    // asm: 	AND	0FFH,R6
    // asm: 	AND	0FFH,R5
    // asm: 	AND	0FFH,R4
    // *SEND LENGTH  (BYTE 1 MSB)
    // asm: 	OR	C_MAS|C_C2,R6	; (C2=1)
    // asm: 	LS	16,R6
    // asm: 	STI	R6,*AR5		;SEND C2 HIGH WITH DATA
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // *SEND LENGTH  (BYTE 2 LSB)
    // asm: 	OR	C_MAS,R4	; (C2=1)
    // asm: 	LS	16,R4
    // asm: 	STI	R4,*AR5		;SEND C2 HIGH WITH DATA
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // *SEND LENGTH  (BYTE 3 MSB COMPLEMENTED)
    // asm: 	OR	C_MAS|C_C2,R7	; (C2=1)
    // asm: 	LS	16,R7
    // asm: 	STI	R7,*AR5		;SEND C2 HIGH WITH DATA
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // *SEND LENGTH  (BYTE 4 LSB COMPLEMENTED)
    // asm: 	OR	C_MAS,R5
    // asm: 	LS	16,R5
    // asm: 	STI	R5,*AR5		;SEND C2 LOW WITH DATA
    // asm: 	RPTS	65
    // asm: 	NOP
    // asm: 	DEC	AR6
    // asm: 	LDI	0FFh,R4
    // *
    // * SEND MASTERS BUFFER
    // *
    // asm: 	LDI	0,R3		;CHECKSUM
    // *byte1
    // asm: 	AND	*AR2++,R4,R6
    // asm: 	ADDI	R6,R3		;FORM CHECKSUM
    // asm: 	OR	C_MAS|C_C2,R6	;(C2=1)
    // asm: MSDLP
    // asm: 	LS	16,R6
    // asm: 	STI	R6,*AR5
    // asm: 	RPTS	TIME_DELAY
    // asm: 	NOP
    // *byte2
    // asm: 	AND	*AR2++,R4,R6
    // asm: 	ADDI	R6,R3
    // asm: 	OR	C_MAS,R6	;(C2=0)
    // asm: 	LS	16,R6
    // asm: 	STI	R6,*AR5
    // asm: 	RPTS	TIME_DELAY
    // asm: 	NOP
    // asm: 	DBUD	AR6,MSDLP
    // asm: 	AND	*AR2++,R4,R6
    // asm: 	ADDI	R6,R3		;FORM CHECKSUM
    // asm: 	OR	C_MAS|C_C2,R6	; (C2=1)
    // 	;---->DBUD	AR6,MSDLP
    // asm: 	ANDN	C_MAS|C_C2,R6	;ONE TOO MANY...
    // asm: 	SUBI	R6,R3
    // *SEND THE CHECKSUM
    // asm: 	AND	0FFFFH,R3	;MASK CKSUM TO 16 BITS
    // asm: 	LDI	R3,R6	     	;SEND MSB
    // asm: 	RS	8,R6
    // asm: 	OR	C_MAS|C_C2,R6	;(C2=1)
    // asm: 	LS	16,R6
    // asm: 	STI	R6,*AR5		;SEND 2 HIGH
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // asm: 	LS	24,R3
    // asm: 	RS	24,R3
    // asm: 	OR	C_MAS,R3	;(C2=0)
    // asm: 	LS	16,R3
    // asm: 	STI	R3,*AR5		;SEND 2 HIGH
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // asm: 	LDI	C_RCV,R0	;CHANGE CONTROL TO RECEIVE
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*+AR5(CTL)	;@COMM_CTL
    // *
    // * WAIT FOR SLAVE TO SIGNIFY DONE (C1=0)
    // *
    // asm: 	LDI	30,R5			;TIMEOUT
CML1XX:
    // asm: DEC	R5
    // asm: 	BLT	CMERROR10
    // asm: 	LDI	*AR5,R0
    // asm: 	RS	16,R0
    // asm: 	TSTB	C_C1,R0
    // asm: 	BNZ	CML1XX
    // *
    // * MASTER TELLS SLAVE IT IS LISTENING (C2=1)
    // *
    // asm: 	LDI	@RECEIVE_BUFFERI,AR2 	;SETUP RECEIVE BUFFER
    // asm: 	LDI	C_C2|C_MAS,R0  		;SET C2=HI
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // * GET SLAVES BUFFER LENGTH
    // * BUFFER LENGTH IS X 2
    // * 4 BYTES LENGTH MSB, LSB, COMP MSB, COMP LSB
    // *
    // asm: 	LDP	COMM_IO
    // asm: 	LDI	C_C1,R5
    // asm: 	LS	16,R5
    // asm: 	LDI	300,AR3
PWL2M:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBZ	AR3,PWL2M
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CMERROR1		;ERROR IN TIMEOUT
    // asm: 	NOP
    // asm: 	LDI	300,AR3
    // asm: 	LDI	@COMM_IO,R7	    	;GET MSB
    // 	;---->BNZD	CMERROR1
PWL3M:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBNZ	AR3,PWL3M
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CMERROR2		;ERROR IN TIMEOUT
    // asm: 	NOP
    // asm: 	LDI	@COMM_IO,R6		;GET LSB
    // asm: 	LS	8,R6
    // 	;---->BNZD	CMERROR2
    // asm: 	LDI	300,AR3
PWL4M:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBZ	AR3,PWL4M
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CMERROR1		;ERROR IN TIMEOUT
    // asm: 	NOP
    // asm: 	LDI	300,AR3
    // asm: 	NOT	@COMM_IO,R4		;GET MSB COMPLEMENT
    // 	;---->BNZD	CMERROR1
PWL5M:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBNZ	AR3,PWL5M
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CMERROR2		;ERROR IN TIMEOUT
    // asm: 	NOP
    // asm: 	NOT	@COMM_IO,R2		;GET LSB COMPLEMENT
    // asm: 	LS	8,R2
    // 	;---->BNZD	CMERROR2
    // asm: 	LS	8,R4
    // asm: 	LS	8,R7
    // asm: 	RS	24,R2
    // asm: 	RS	24,R4
    // asm: 	RS	24,R6
    // asm: 	RS	24,R7
    // asm: 	CMPI	R2,R6
    // asm: 	BNE	CMERROR3		;ERROR IN LENGTH
    // asm: 	CMPI	R4,R7
    // asm: 	BNE	CMERROR3		;ERROR IN LENGTH
    // asm: 	LSH	8,R7
    // asm: 	ADDI	R6,R7,AR6
    // asm: 	CMPI	140H,AR6		;LENGTH ERROR
    // asm: 	BGE	CMERROR3		;TOO LONG
    // asm: 	CMPI	0,AR6			;SLAVE LENGTH ZERO...
    // asm: 	BEQ	CMERROR3
    // asm: 	DEC	AR6	 		;THIS IS THE LENGTH INDEX
    // *
    // * GET SLAVES BUFFER
    // *
    // asm: 	SETDP
    // asm: 	STI	AR6,@RBUFF_LEN
    // asm: 	LDP	@COMM_IO
    // asm: 	CLRI	R3			;RESET CKSUM
    // asm: 	LDI	0FFH,R2
    // asm: 	LSH	16,R2
    // asm: 	LDI	300,AR3
WL2M:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBZ	AR3,WL2M
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CMERROR4		;YES...QUIT
    // asm: 	LDI	300,AR3
    // asm: 	NOP
    // asm: 	AND	R2,*AR5,R6
    // 	;---->	BNZD	CMERROR4
    // asm: 	ADDI	R6,R3
    // asm: 	STI	R6,*AR2++
WL3M:
    // asm: TSTB	*AR5,R5
    // asm: 	DBNZ	AR3,WL3M
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CMERROR5		;YES...QUIT
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	AND	R2,*AR5,R6
    // 	;---->	BNZD	CMERROR5
    // asm: 	DBUD	AR6,WL2M
    // asm: 	ADDI	R6,R3
    // asm: 	STI	R6,*AR2++
    // asm: 	LDI	300,AR3
    // 	;---->DBUD	AR6,WL2M
    // *GET SLAVES CKSUM
WL2MC:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBZ	AR3,WL2MC
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CMERROR6	       	;YES...QUIT
    // asm: 	NOP
    // asm: 	LDI	300,AR3
    // asm: 	LDI	@COMM_IO,R6		;MSB CKSUM
    // 	;---->	BNZD	CMERROR6
WL3MC:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBNZ	AR3,WL3MC
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CMERROR7	       	;YES...QUIT
    // asm: 	RS	8,R6
    // asm: 	AND	0FF00h,R6
    // asm: 	LDI	@COMM_IO,R7		;LSB CKSUM
    // 	;---->	BNZD	CMERROR7
    // asm: 	RS	16,R7
    // asm: 	AND	0FFh,R7
    // asm: 	ADDI	R6,R7
    // asm: 	RS	16,R3
    // asm: 	CMPI	R7,R3
    // asm: 	BNE	CMERROR8	       	;CKSUM ERROR
    // asm: M0LENGTH
    // asm: COMM_MASTER_X
    // asm: 	LDP	@COMM_MASTER_TRANSES
    // asm: 	INCM	@COMM_MASTER_TRANSES
COMM_MASTER_ERR_X:
    // asm: 	LDI	C_MAS,R0	      	;SET C2=0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // asm: 	LDI	C_RCV,R0	      	;SET DATA CONTROL TO RECEIVE
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*+AR5(CTL)
    // asm: 	SETDP
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@COMMFLAG		;COMMUNICATIONS IS OVER
    // asm: 	EINT
    // asm: 	RETS
CMERROR1:
    // asm: B	COMM_MASTER_ERROR   	;LENGTH TIMEOUT
CMERROR2:
    // asm: B	COMM_MASTER_ERROR	;LENGTH TIMEOUT
CMERROR3:
    // asm: B	COMM_MASTER_ERROR	;BAD LENGTH
CMERROR4:
    // asm: B	COMM_MASTER_ERROR	;DATA TIMEOUT
CMERROR5:
    // asm: B	COMM_MASTER_ERROR	;DATA TIMEOUT
CMERROR6:
    // asm: B	COMM_MASTER_ERROR	;CKSUM TIMEOUT
CMERROR7:
    // asm: B	COMM_MASTER_ERROR	;CKSUM TIMEOUT
CMERROR8:
    // asm: B	COMM_MASTER_ERROR	;BAD CKSUM
CMERROR9:
    // asm: B	COMM_MASTER_ERROR
CMERROR10:
    // asm: B	COMM_MASTER_ERROR	;MASTER END TIMEOUT (SLAVE ERR)
CMERRORL:
    // asm: B	COMM_MASTER_ERROR	;LENGTH TOO LONG
CMERRORDEAD:
    // asm: 		LDI	1,R0		      	;YES OTHER GAME IS DEAD, LEAVE...
    // asm: 		STI	R0,@TRANSMISSION_DEAD
    // asm: 		B	COMM_MASTER_ERROR	;WENT DEAD...
COMM_MASTER_ERROR:
    // asm: 	LDP	@COMM_MASTER_TRANSES
    // asm: 	INCM	@COMM_MASTER_ERROR_CNT
    // asm: 	SETDP
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@RBUFF_LEN	   	;RECEIVE BUFFER CLEARED	ON ERROR
    // ;	LDI	C_C2|C_MAS,R0  		;SET C2=HI
    // ;	LS	16,R0
    // ;	STI	R0,*AR5
    // asm: 	B	COMM_MASTER_ERR_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_MASTER", 0, 0);
    UNIMPL();
}

void COMM_IRQ(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	PUSH	ST
    // asm: 	PUSH	DP
    // asm: 	PUSH	R0
    // asm: 	ANDN	INT2_M,IF
    // 	;determine if I am a master being interrupted
    // 	;
    // asm: 	LDP	@DIPRAM
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BNZ	DO_SLAVE_SYNC
    // asm: 	POP	R0
    // asm: 	POP	DP
    // asm: 	POP	ST
    // asm: 	RETI
DO_SLAVE_SYNC:
    // asm: 	PUSH	R1
    // asm: 	PUSH	IE
    // asm: 	LDI	INT1_M|INT2_M,IE	;disable everything except TV30 interrupt & comm int
    // asm: 	ANDN	INT2_M,IF		;we wont irq ourself
    // asm: 	LDP	@CPU_WS
    // asm: 	LDI	@CPU_WS,R0
    // asm: 	PUSH	R0
    // asm: 	LDI	SOFT_WS,R0
    // asm: 	STI	R0,@CPU_WS
    // 	;sync the systems...
    // 	;
    // 	;
    // asm: 	LDP	@FIFO_CONTROL
    // asm: 	LDI	@FIFO_CONTROL,R0
    // asm: 	ANDN	FIFO_CONTROL_DMA_RUNSEL,R0
    // asm: 	STI	R0,@FIFO_CONTROL
WTLP:
    // asm: LDI	@FIFO_STATUS,R0
    // asm: 	AND	FIFO_STATUS_FD_CRITICAL,R0
    // asm: 	BNZ	WTLP
    // ;same	LDP	@CRT_VCNT
    // asm: 	LDI	@CRT_VCNT,R0
    // asm: 	AND	1FFh,R0
    // ;	CMPI	400,R0
    // ;	LDIEQ	1b0h,R0
    // ;	LDIGT	1b1h,R0
    // ;	LDILT	1afh,R0
    // asm: 	LDI	1B0H,R1
    // asm: 	SUBI	400,R0
    // asm: 	CMPI	-1,R0	 			;GRACE AREA DUDES
    // asm: 	LDILT	1afh,R1
    // asm: 	CMPI	1,R0
    // asm: 	LDIGT	1b1h,R1
    // asm: 	STI	R1,@CRT_VTTL
    // asm: 	LDI	@FIFO_CONTROL,R0
    // asm: 	OR	FIFO_CONTROL_DMA_RUNSEL,R0
    // asm: 	STI	R0,@FIFO_CONTROL
    // asm: 	POP	R0
    // asm: 	LDP	@CPU_WS
    // asm: 	STI	R0,@CPU_WS
    // asm: 	POP	IE
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	DP
    // ;	LDI	INT1_M|INT2_M|INT3_M|INT0_M,IE
    // asm: 	POP	ST
    // asm: 	RETI
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_IRQ", 0, 0);
    UNIMPL();
}

void COMM_SLAVE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *CALLED FROM THE MAIN LOOP
    // *
    // *SET C0 HIGH
    // *WAIT FOR C2 TO GO HIGH
    // *BEGIN TRANSFER
    // *END OF TRANSFER
    // *SET ALL LOW
    // *
    // *
    // asm: 	SETDP
    // asm: 	LDI	@COMM_IOI,AR5		;AR5=COMM I/O ADDRESS
    // asm: 	LDI	@RECEIVE_BUFFERI,AR2	;AR2=RECEIVE BUFFER
    // asm: 	LDI	C_C2,R5			;R5=C_C2 MASK
    // asm: 	LS	16,R5
    // *CHECK ONE PLAYER MODE
    // asm: 	LDI	@ONEFLAG,R0
    // asm: 	BZ	CS1
    // asm: 	RETS				;WE'RE IN ONE PLAYER, EXIT
CS1:
    // asm: 	TSTB	*AR5,R5
    // asm: 	BZ	CS2			;C2=1, MASTER IN ONE PLAYER, EXIT
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@TRANSMISSION_DEAD
    // asm: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm: 	RETS
    // *SYNC UP
CS2:
    // asm: 	LDI	120,R3			;WAIT 120 FRAMES IF NOT DEAD
    // asm: 	LDI	@TRANSMISSION_DEAD,R0 	;GET TIMEOUT VALUES
    // asm: 	BZ	CS20
    // asm: 	LDI	1,R3			;WAIT 1 IF DEAD
    // asm: 	ADDI	@INFRAMES,R3
    // asm: 	CMPI	4,R3
    // asm: 	LDIGT	4,R3
CS20:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@TRANSMISSION_ACTIVE
    // asm: 	LDI	C_C0|C_SLA,R0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5			;SEND C0=1
WTLPI:
    // asm: 	CMPI	@INFRAMES,R3		;WAITING TOO LONG?
    // asm: 	BLE	CSERRORDEAD		;YES, ITS DEAD...
    // asm: 	TSTB	*AR5,R5			;WAIT C2=1
    // asm: 	BZ	WTLPI
    // asm: 	LDI	C_SLA,R0		;SEND C0=0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@TRANSMISSION_DEAD
    // asm: 	LDI	10000,AR3   		;TIMEOUT VALUE
    // asm: WTLPI2
    // asm: 	TSTB	*AR5,R5			;WAIT C2=0
    // asm: 	DBNZ	AR3,WTLPI2
    // asm: 	TSTB	8000H,AR3		;TIMEOUT?
    // asm: 	BZ	CS3			;NO
    // asm: 	RETS				;YES, LEAVE
    // *END SYNC
CS3:
    // asm: 	DINT
    // *SET DATA CONTROL TO RECEIVE		;CM_A 1
    // asm: 	LDI	C_RCV,R0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*+AR5(CTL)
    // *TELL MASTER WE ARE LISTENING (C0=0,C1=1)
    // asm: 	LDI	C_C1|C_SLA,R0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // *GET MASTERS BUFFER LENGTH
    // *BUFFER LENGTH IS X 2
    // *FIRST BYTE IS LENGTH
    // *SECOND BYTE IS COMPLEMENT OF LENGTH
    // asm: 	LDP	@COMM_IO
    // asm: 	LDI	300,AR3
PWL2:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBZ	AR3,PWL2
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CSERROR1		;ERROR IN TIMEOUT
    // asm: 	NOP
    // asm: 	LDI	300,AR3
    // asm: 	LDI	@COMM_IO,R7	    	;GET MSB
    // 	;---->BNZD	CSERROR1
PWL3:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBNZ	AR3,PWL3
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CSERROR2		;ERROR IN TIMEOUT
    // asm: 	NOP
    // asm: 	LDI	@COMM_IO,R6		;GET LSB
    // asm: 	LS	8,R6
    // 	;---->BNZD	CSERROR2
    // asm: 	LDI	300,AR3
PWL4:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBZ	AR3,PWL4
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CSERROR1		;ERROR IN TIMEOUT
    // asm: 	NOP
    // asm: 	LDI	300,AR3
    // asm: 	NOT	@COMM_IO,R4		;GET MSB COMPLEMENT
    // 	;---->BNZD	CSERROR1
PWL5:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBNZ	AR3,PWL5
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CSERROR2		;ERROR IN TIMEOUT
    // asm: 	NOP
    // asm: 	NOT	@COMM_IO,R2		;GET LSB COMPLEMENT
    // asm: 	LS	8,R2
    // 	;---->BND	CSERROR2
    // asm: 	LS	8,R4
    // asm: 	LS	8,R7
    // asm: 	RS	24,R2
    // asm: 	RS	24,R4
    // asm: 	RS	24,R6
    // asm: 	RS	24,R7
    // asm: 	CMPI	R2,R6
    // asm: 	BNE	CSERROR3		;ERROR IN LENGTH
    // asm: 	CMPI	R4,R7
    // asm: 	BNE	CSERROR3		;ERROR IN LENGTH
    // asm: 	LSH	8,R7
    // asm: 	ADDI	R6,R7,AR6
    // asm: 	DEC	AR6	 		;THIS IS THE LENGTH INDEX
    // asm: 	CMPI	140H,AR6		;LENGTH ERROR
    // asm: 	BGE	CSERROR3		;TOO LONG
    // *
    // *GET MASTERS BUFFER
    // *
    // asm: 	SETDP
    // asm: 	STI	AR6,@RBUFF_LEN
    // asm: 	LDP	@COMM_IO
    // asm: 	CLRI	R3			;RESET CKSUM
    // asm: 	LDI	0FFH,R2
    // asm: 	LSH	16,R2
    // asm: 	LDI	300,AR3
WL2:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBZ	AR3,WL2
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CSERROR4		;YES...QUIT
    // asm: 	LDI	300,AR3
    // asm: 	NOP
    // asm: 	AND	R2,*AR5,R6
    // 	;---->	BNZD	CSERROR4		;YES...QUIT
    // asm: 	ADDI	R6,R3
    // asm: 	STI	R6,*AR2++
WL3:
    // asm: TSTB	*AR5,R5
    // asm: 	DBNZ	AR3,WL3
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CSERROR5		;YES...QUIT
    // asm: 	NOP
    // asm: 	NOP
    // asm: 	AND	R2,*AR5,R6
    // 	;---->	BNZD	CSERROR5		;YES...QUIT
    // asm: 	DBUD	AR6,WL2
    // asm: 	ADDI	R6,R3
    // asm: 	STI	R6,*AR2++
    // asm: 	LDI	300,AR3
    // *GET MASTERS CKSUM
WL2C:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBZ	AR3,WL2C
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CSERROR6	       	;YES...QUIT
    // asm: 	NOP
    // asm: 	LDI	300,AR3
    // asm: 	LDI	@COMM_IO,R6		;MSB CKSUM
    // 	;---->	BNZD	CSERROR6	       	;YES...QUIT
WL3C:
    // asm: TSTB	@COMM_IO,R5
    // asm: 	DBNZ	AR3,WL3C
    // asm: 	TSTB	8000H,AR3
    // asm: 	BNZD	CSERROR7	       	;YES...QUIT
    // asm: 	RS	8,R6
    // asm: 	AND	0FF00h,R6
    // asm: 	LDI	@COMM_IO,R7		;LSB CKSUM
    // 	;---->	BNZD	CSERROR7	       	;YES...QUIT
    // asm: 	RS	16,R7
    // asm: 	AND	0FFh,R7
    // asm: 	ADDI	R6,R7
    // asm: 	RS	16,R3
    // asm: 	CMPI	R7,R3
    // asm: 	BNE	CSERROR8	       	;CKSUM ERROR
    // *	LOWER SLAVES C1
    // *	(tell master we are prepared to send)
    // *
    // asm: 	SETDP
    // asm: 	LDI	C_SLA,R0
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // *
    // * SLAVE SEND
    // *
    // * WAIT FOR MASTER TO LISTEN (C2=1)
    // asm: 	LDI	50,R5			;TIMEOUT
CML1S:
    // asm: DEC	R5
    // asm: 	BLT	CSERROR9
    // asm: 	LDI	*AR5,R0
    // asm: 	RS	16,R0
    // asm: 	TSTB	C_C2,R0
    // asm: 	BZ	CML1S
    // asm: 	LDI	C_SND,R0		;CHANGE DATA BUFFER DIRECTION TO SEND
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*+AR5(CTL)
    // * SEND SLAVE LENGTH
    // *SEND MESSAGE LENGTH TO MASTER
    // *MESSAGE LENGTH IS MULTIPLE OF 2
    // asm: 	SETDP
    // asm: 	LDI	@SEND_BUFFER_A_LEN,R6
    // asm: 	LDI	@SEND_BUFFER_AI,AR2
    // asm:  	ADDI	R6,AR2,AR0	;ADD 2 NULLS TO THE END DUDES
    // asm: 	LDI	CB_NULL,R2
    // asm: 	STI	R2,*AR0
    // asm: 	STI	R2,*+AR0(1)
    // asm: 	ADDI	1,R6
    // asm: 	RS	1,R6
    // asm: 	LDI	R6,AR6
    // asm: 	NOT	R6,R7
    // asm: 	LDI	R7,R5
    // asm: 	LSH	-8,R7
    // asm: 	LDI	R6,R4
    // asm: 	LSH	-8,R6
    // asm: 	AND	0FFH,R7
    // asm: 	AND	0FFH,R6
    // asm: 	AND	0FFH,R5
    // asm: 	AND	0FFH,R4
    // *SEND LENGTH  (BYTE 1 MSB)
    // asm: 	OR	C_SLA|C_C1,R6	; (C2=1)
    // asm: 	LS	16,R6
    // asm: 	STI	R6,*AR5		;SEND C2 HIGH WITH DATA
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // *SEND LENGTH  (BYTE 2 LSB)
    // asm: 	OR	C_SLA,R4	; (C2=1)
    // asm: 	LS	16,R4
    // asm: 	STI	R4,*AR5		;SEND C2 HIGH WITH DATA
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // *SEND LENGTH  (BYTE 3 MSB COMPLEMENTED)
    // asm: 	OR	C_SLA|C_C1,R7	; (C2=1)
    // asm: 	LS	16,R7
    // asm: 	STI	R7,*AR5		;SEND C2 HIGH WITH DATA
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // *SEND LENGTH  (BYTE 4 LSB COMPLEMENTED)
    // asm: 	OR	C_SLA,R5
    // asm: 	LS	16,R5
    // asm: 	STI	R5,*AR5		;SEND C2 LOW WITH DATA
    // asm: 	RPTS	65
    // asm: 	NOP
    // asm: 	DEC	AR6
    // asm: 	LDI	0FFh,R4
    // *
    // * SLAVE SEND BUFFER TO MASTER
    // *
    // asm: 	LDI	0,R3		;CHECKSUM
    // *byte1
    // asm: 	AND	*AR2++,R4,R6
    // asm: 	ADDI	R6,R3		;FORM CHECKSUM
    // asm: 	OR	C_SLA|C_C1,R6	;(C1=1)
    // asm: SSDLP
    // asm: 	LS	16,R6
    // asm: 	STI	R6,*AR5
    // asm: 	RPTS	TIME_DELAY
    // asm: 	NOP
    // *byte2
    // asm: 	AND	*AR2++,R4,R6
    // asm: 	ADDI	R6,R3
    // asm: 	OR	C_SLA,R6	;(C1=0)
    // asm: 	LS	16,R6
    // asm: 	STI	R6,*AR5
    // asm: 	RPTS	TIME_DELAY
    // asm: 	NOP
    // asm: 	DBUD	AR6,SSDLP
    // asm: 	AND	*AR2++,R4,R6
    // asm: 	ADDI	R6,R3		;FORM CHECKSUM
    // asm: 	OR	C_SLA|C_C1,R6	; (C2=1)
    // 	;---->DBUD	AR6,SSDLP
    // asm: 	ANDN	C_SLA|C_C1,R6
    // asm: 	SUBI	R6,R3		;ONE OVER!!!
    // *SEND THE CHECKSUM
    // asm: 	AND	0FFFFH,R3	;MASK CKSUM TO 16 BITS
    // asm: 	LDI	R3,R6	     	;SEND MSB
    // asm: 	RS	8,R6
    // asm: 	OR	C_SLA|C_C1,R6	;(C1=1)
    // asm: 	LS	16,R6
    // asm: 	STI	R6,*AR5		;SEND DATA
    // asm: 	RPTS	TIME_DELAY		;HOLD FOR A SEC
    // asm: 	NOP
    // asm: 	LS	24,R3
    // asm: 	RS	24,R3
    // asm: 	OR	C_SLA,R3	;(C1=0)
    // asm: 	LS	16,R3
    // asm: 	STI	R3,*AR5		;SEND DATA
    // asm: 	RPTS	TIME_DELAY    	;HOLD FOR A SEC
    // asm: 	NOP
    // *WAIT FOR MASTER TO INDICATE O.K. C2=0
    // asm: LENGTHOF0
    // asm: 	LDI	50,R5	       	;TIMEOUT
CML1WT:
    // asm: DEC	R5
    // asm: 	BLT	CSERROR11
    // asm: 	LDI	*AR5,R0
    // asm: 	RS	16,R0
    // asm: 	TSTB	C_C2,R0
    // asm: 	BNZ	CML1WT
    // asm: COMM_SLAVE_X
    // asm: 	LDP	@COMM_SLAVE_TRANSES
    // asm: 	INCM	@COMM_SLAVE_TRANSES
COMM_SLAVE_ERR_X:
    // asm: 	SETDP
    // asm: 	LDI	C_SLA,R0  		;SET C0=0,C1=0, NO ERROR
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*AR5
    // asm: 	LDI	C_RCV,R0		;CHANGE CONTROL TO RECEIVE
    // asm: 	LS	16,R0
    // asm: 	STI	R0,*+AR5(CTL)		;@COMM_CTL
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@COMMFLAG		;COMMUNICATIONS IS OVER
    // asm: 	EINT
    // asm: 	RETS
CSERROR1:
    // asm: B COMM_SLAVE_ERROR ;LENGTH TIMEOUT
CSERROR2:
    // asm: B COMM_SLAVE_ERROR ;LENGTH TIMEOUT
CSERROR3:
    // asm: B COMM_SLAVE_ERROR ;BAD LENGTH
CSERROR4:
    // asm: B COMM_SLAVE_ERROR ;DATA TIMEOUT
CSERROR5:
    // asm: B COMM_SLAVE_ERROR ;DATA TIMEOUT
CSERROR6:
    // asm: B COMM_SLAVE_ERROR ;CKSUM TIMEOUT
CSERROR7:
    // asm: B COMM_SLAVE_ERROR ;CKSUM TIMEOUT
CSERROR8:
    // asm: B COMM_SLAVE_ERROR ;BAD CKSUM
CSERROR9:
    // asm: B COMM_SLAVE_ERROR ;SLAVE SEND TIMEOUT
CSERROR11:
    // asm: B COMM_SLAVE_ERROR ;MASTER ERROR END TIMEOUT
CSERRORDEAD:
    // asm: 		LDI	1,R0		      	;YES OTHER GAME IS DEAD, LEAVE...
    // asm: 		STI	R0,@TRANSMISSION_DEAD
    // asm: 		B	COMM_SLAVE_ERROR	;WENT DEAD...
COMM_SLAVE_ERROR:
    // asm: 	LDP	@COMM_SLAVE_ERROR_CNT
    // asm: 	INCM	@COMM_SLAVE_ERROR_CNT
    // asm: 	SETDP
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@RBUFF_LEN	   	;RECEIVE BUFFER CLEARED	ON ERROR
    // asm: 	BU	COMM_SLAVE_ERR_X
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_SLAVE", 0, 0);
    UNIMPL();
}

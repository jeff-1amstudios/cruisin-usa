#ifndef COMM_H
#define COMM_H

#include "../core/port.h"

/* Generated from asm/COMM.EQU. */

// COMM.EQU
//
// COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// ALL RIGHTS RESERVED

// asm: CDEBUG		.set	0
#define CDEBUG 0

// ----------------------------------------------------------------------------
// COMMUNICATION BUFFER
//
// in BUFSTATS
//
//
// asm: BS_RDY_M	.set	003h
#define BS_RDY_M 0x003

// asm: BS_RDYA		.set	001h
#define BS_RDYA 0x001

// asm: BS_RDYB		.set	002h
#define BS_RDYB 0x002

// asm: BS_FIL_M	.set	030h
#define BS_FIL_M 0x030

// asm: BS_FILA		.set	010h
#define BS_FILA 0x010

// asm: BS_FILB		.set	020h
#define BS_FILB 0x020

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// COMMUNICATION BLOCKS
// (1 byte, all unique, NO masking)
//

// asm: CB_NULL		.set	0	;Do nothing (Padder)
#define CB_NULL 0 // Do nothing (Padder)

// asm: CB_MODE		.set	1	;This is my MODE
#define CB_MODE 1 // This is my MODE

// asm: CB_TIMECODE	.set	2	;This is my _countdown
#define CB_TIMECODE 2 // This is my _countdown

// asm: CB_RACENUM	.set	3	;This is my CHOSEN_RACE
#define CB_RACENUM 3 // This is my CHOSEN_RACE

// asm: CB_VEHICLE	.set	4	;This is my CHOSEN_VEHICLE
#define CB_VEHICLE 4 // This is my CHOSEN_VEHICLE

// asm: CB_START_GAME	.set	5	;I HAVE BOUGHT INTO MY LINKED SYSTEM
#define CB_START_GAME 5 // I HAVE BOUGHT INTO MY LINKED SYSTEM

// asm: CB_FINISHED	.set	6	;I have finished the Race
#define CB_FINISHED 6 // I have finished the Race

// asm: CB_CAR_UPDATE	.set	7	;UPDATE DATA BLOCK FOR CAR
#define CB_CAR_UPDATE 7 // UPDATE DATA BLOCK FOR CAR

// asm: CB_END_GAME	.set	8	;I HAVE ENDED THE GAME (ON MY SYSTEM)
#define CB_END_GAME 8 // I HAVE ENDED THE GAME (ON MY SYSTEM)

// asm: CB_HEAD2HEADT	.set	9	;LINKED MESSAGE
#define CB_HEAD2HEADT 9 // LINKED MESSAGE

// asm: CB_HEAD2HEADF	.set	10	;LINKED MESSAGE
#define CB_HEAD2HEADF 10 // LINKED MESSAGE

// asm: CB_LINKCANCELLED	.set	11	;cancelled the linkup
#define CB_LINKCANCELLED 11 // cancelled the linkup

// asm: CB_RACER_UPDATE	.set	12	;UPDATE DATA BLOCK FOR CAR
#define CB_RACER_UPDATE 12 // UPDATE DATA BLOCK FOR CAR

// asm: CB_RACER_XSFER	.set	13	;XSFER DRONE TO OTHER MACHINE CONTROL
#define CB_RACER_XSFER 13 // XSFER DRONE TO OTHER MACHINE CONTROL

// asm: CB_RACER_KILL	.set	14	;KILL CAR ID
#define CB_RACER_KILL 14 // KILL CAR ID

// asm: CB_RHO_CREATE	.set	15	;CREATE CAR ID
#define CB_RHO_CREATE 15 // CREATE CAR ID

// asm: CB_RHO_KILL	.set	16	;KILL CAR ID
#define CB_RHO_KILL 16 // KILL CAR ID

// asm: CB_RHO_XSFER	.set	17	;XSFER CAR ID
#define CB_RHO_XSFER 17 // XSFER CAR ID

// asm: CB_RHO_UPDATE	.set	18	;UPDATE CAR ID
#define CB_RHO_UPDATE 18 // UPDATE CAR ID

// asm: CB_FLY_KILL	.set	19	;KILL CAR ID
#define CB_FLY_KILL 19 // KILL CAR ID

// asm: CB_FLY_XSFER	.set	20	;XSFER CAR ID
#define CB_FLY_XSFER 20 // XSFER CAR ID

// asm: CB_FLY_UPDATE	.set	21	;UPDATE CAR ID
#define CB_FLY_UPDATE 21 // UPDATE CAR ID

// asm: CB_WAVEFL_READY	.set	22
#define CB_WAVEFL_READY 22

// asm: CB_WAVEFL_SET	.set	23
#define CB_WAVEFL_SET 23

// asm: CB_WAVEFL_GO	.set	24
#define CB_WAVEFL_GO 24

// asm: CB_OM_TRACK	.set	25
#define CB_OM_TRACK 25

// asm: CB_BONUS_SYNC0	.set	26
#define CB_BONUS_SYNC0 26

// asm: CB_BONUS_SYNC1	.set	27
#define CB_BONUS_SYNC1 27

// asm: CB_BONUS_SYNC2	.set	28
#define CB_BONUS_SYNC2 28

// asm: CB_BONUS_SYNC3	.set	29
#define CB_BONUS_SYNC3 29

// asm: CB_CHANGE_MUSIC	.set	30
#define CB_CHANGE_MUSIC 30

// asm: CB_COINDROP	.set	31
#define CB_COINDROP 31

// asm: CB_DIAGNOSTIC	.set	32
#define CB_DIAGNOSTIC 32

// asm: CB_ATTRSND	.set	33
#define CB_ATTRSND 33

// asm: CB_LASTMSG	.set	34
#define CB_LASTMSG 34

//
//
// ----------------------------------------------------------------------------

// asm: COMMDP		.set	0997000h	;FOR DP
#define COMMDP 0x0997000 // FOR DP

// asm: COMM_CTL	.set	0997001h	;direction
#define COMM_CTL 0x0997001 // direction

// asm: C_DE		.set	04000h	;data enable
#define C_DE 0x04000 // data enable

// asm: C_CE		.set	02000h	;control enable
#define C_CE 0x02000 // control enable

// ALWAYS 1 ALWAYS

// asm: C_SND		.set	C_DE|C_CE
#define C_SND (C_DE | C_CE)

// asm: C_RCV		.set	C_CE
#define C_RCV C_CE

// default for everything is high

// asm: COMM_IO		.set	0997000h	;data and control
#define COMM_IO 0x0997000 // data and control

// asm: C_IRQ		.set	00800h	;MA  NOT control, is actually interrupt 1 = INT, 0 = nothing
#define C_IRQ 0x00800 // MA  NOT control, is actually interrupt 1 = INT, 0 = nothing

// asm: C_C2		.set	00400h	;MA  just control
#define C_C2 0x00400 // MA  just control

// asm: C_C1		.set	00200h	;SL  just control
#define C_C1 0x00200 // SL  just control

// asm: C_C0		.set	00100h	;SL  just control
#define C_C0 0x00100 // SL  just control

// asm: C_IRQE		.set	08000h	;MA  NOT control enable, control enable
#define C_IRQE 0x08000 // MA  NOT control enable, control enable

// asm: C_CE2		.set	04000h	;MA  just control
#define C_CE2 0x04000 // MA  just control

// asm: C_CE1		.set	02000h	;SL  just control
#define C_CE1 0x02000 // SL  just control

// asm: C_CE0		.set	01000h	;SL  just control
#define C_CE0 0x01000 // SL  just control

// asm: C_DMASK		.set	000FFh
#define C_DMASK 0x000FF

// we dedicate the lines by master/slave
//
// asm: C_MAS	.set	C_IRQE|C_CE2
#define C_MAS (C_IRQE | C_CE2)

// asm: C_SLA	.set	C_CE1|C_CE0
#define C_SLA (C_CE1 | C_CE0)

// FOR COMMQ.ASM
//
//

// asm: COMM_BUFFER_SIZE	.set	600
#define COMM_BUFFER_SIZE 600

// asm: COMM_Q_FREEZE		.set	99Ah	;LOGICAL == "I HAVE NOTHING TO SEND"
#define COMM_Q_FREEZE 0x99A // LOGICAL == "I HAVE NOTHING TO SEND"

// asm: COMM_Q_OPEN		.set	878h	;LOGICAL == "I AM WORKING ON TRYING TO SEND SOMETHING"
#define COMM_Q_OPEN 0x878 // LOGICAL == "I AM WORKING ON TRYING TO SEND SOMETHING"

// asm: COMM_Q_READY		.set	45Bh	;LOGICAL == "I HAVE SOMETHING READY TO SEND"
#define COMM_Q_READY 0x45B // LOGICAL == "I HAVE SOMETHING READY TO SEND"

//
// OM_STATE
//
// Lower two bytes are the state
//
// OMS_
//
//
// asm: OMS_VEHICLE_M	.set	0E0h
#define OMS_VEHICLE_M 0x0E0

// asm: OMS_XVEH	.set	080h
#define OMS_XVEH 0x080

// asm: OMS_RACE_M	.set	00Fh
#define OMS_RACE_M 0x00F

// use RACE_xx in sys.equ for actual race index

// asm: OMS_CPOINT_M	.set	00F00h		;this tells the mode
#define OMS_CPOINT_M 0x00F00 // this tells the mode

// asm: OMS_CPOINT1	.set	00100h
#define OMS_CPOINT1 0x00100

// asm: OMS_CPOINT2	.set	00200h
#define OMS_CPOINT2 0x00200

// asm: OMS_CPOINT3	.set	00400h
#define OMS_CPOINT3 0x00400

// asm: OMS_FINISHLINE	.set	00800h
#define OMS_FINISHLINE 0x00800

// ***	COMM.ASM
// asm: 	.globl	 RECEIVE_BUFFERI
#define RECEIVE_BUFFERI RECEIVE_BUFFER

// asm: 	.globl	 COMMQ_TMP_BUFFI
#define COMMQ_TMP_BUFFI COMMQ_TMP_BUFF

// asm: 	.globl	 COMMQ_TMP_BUFF
extern int COMMQ_TMP_BUFF[];

// asm: 	.globl	 SEND_BUFFER_A
extern int SEND_BUFFER_A[];

// asm: 	.globl	 RECEIVE_BUFFER
extern int RECEIVE_BUFFER[];

// asm: 	.globl	 RBUFF_LEN
extern int RBUFF_LEN;

extern int OM_MODE;

extern int OM_CHOSEN_RACE;

// asm: 	.globl	 SEND_BSYNC0
#define SEND_BSYNC0 SEND_BSYNC3

// asm: 	.globl	 SEND_BSYNC1
#define SEND_BSYNC1 SEND_BSYNC3

// asm: 	.globl	 SEND_BSYNC2
#define SEND_BSYNC2 SEND_BSYNC3

// ***	COMMQ.ASM
// asm: 	.globl	 SEND_CHANGE_MUSIC
void SEND_CHANGE_MUSIC(void);

// asm: 	.globl	 MESSAGE_ADD,MESSAGE_ADD_SB
void MESSAGE_ADD(void);

void MESSAGE_ADD_SB(int message /*AR2*/);

// asm: 	.globl	 DECODE_BUFFER,DECODE_BUFFER_SLAVE
void DECODE_BUFFER(void);

// asm: 	.globl	 SEND_TIMECODE
void SEND_TIMECODE(void);

// asm: 	.globl	 SEND_MODE,OM_MODE
void SEND_MODE(void);

// asm: 	.globl	 SEND_RACENUM,OM_CHOSEN_RACE
void SEND_RACENUM(void);

// asm: 	.globl	 SEND_PLAYERS_POS
void SEND_PLAYERS_POS(void);

// asm: 	.globl	 SEND_OM_TRACK
void SEND_OM_TRACK(void);

// asm: 	.globl	 SEND_BSYNC3
void SEND_BSYNC3(void);

// COMM.ASM
// asm: 	fbss	COMM_MASTER_ERROR_CNT,1
extern int COMM_MASTER_ERROR_CNT;
// asm: 	fbss	COMM_MASTER_TRANSES,1
extern int COMM_MASTER_TRANSES;
// asm: 	fbss	COMM_SLAVE_ERROR_CNT,1
extern int COMM_SLAVE_ERROR_CNT;
// asm: 	fbss	COMM_SLAVE_TRANSES,1
extern int COMM_SLAVE_TRANSES;
// asm: 	.bss	COMMFLAG,1		;COMMUNICATIONS IS OVER
extern int COMMFLAG;
// asm: 	pbss 	ONEFLAG,1
extern int ONEFLAG;
// asm: 	pbss	TRANSMISSION_ACTIVE,1	;1=TRUE
extern int TRANSMISSION_ACTIVE;
// asm: 	pbss	TRANSMISSION_DEAD,1	;1=DEAD LINK
extern int TRANSMISSION_DEAD;

#endif /* COMM_H */

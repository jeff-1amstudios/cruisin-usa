#ifndef COMM_H
#define COMM_H

/* Generated from asm/COMM.EQU. */
/*
 * COMM.EQU
 * COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * ----------------------------------------------------------------------------
 * COMMUNICATION BUFFER
 * in BUFSTATS
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * ...
 */

/* Original .globl symbols in this module:
 *   RECEIVE_BUFFERI
 *   COMMQ_TMP_BUFFI
 *   COMMQ_TMP_BUFF
 *   SEND_BUFFER_A
 *   SEND_BUFFER_B
 *   RECEIVE_BUFFER
 *   RECEIVE_BUFFER_SIZE
 *   SEND_BUFFER_SIZE
 *   CURR_RCV_BUFFER
 *   COMMQ_STATUS
 *   SBUFF_PTR
 *   SBUFF_PTR_STRT
 *   SBUFF_LEN
 *   RBUFF_PTR
 *   RBUFF_LEN
 *   SEND_CHANGE_MUSIC
 *   COMMQ_INIT_F
 *   COMMQ_INIT
 *   COMMQ_FREEZE
 *   COMMQ_READY
 *   MESSAGE_ADD
 *   MESSAGE_ADD_SB
 *   DECODE_BUFFER
 *   DECODE_BUFFER_SLAVE
 *   SEND_TIMECODE
 *   SEND_MODE
 *   OM_MODE
 *   SEND_RACENUM
 *   OM_CHOSEN_RACE
 *   SEND_PLAYERS_POS
 *   OM_POS
 *   SEND_OM_TRACK
 *   SEND_BSYNC0
 *   SEND_BSYNC1
 *   SEND_BSYNC2
 *   SEND_BSYNC3
 */

#define CDEBUG 0
#define BS_RDY_M 0x003
#define BS_RDYA 0x001
#define BS_RDYB 0x002
#define BS_FIL_M 0x030
#define BS_FILA 0x010
#define BS_FILB 0x020
#define CB_NULL 0 /* Do nothing (Padder) */
#define CB_MODE 1 /* This is my MODE */
#define CB_TIMECODE 2 /* This is my _countdown */
#define CB_RACENUM 3 /* This is my CHOSEN_RACE */
#define CB_VEHICLE 4 /* This is my CHOSEN_VEHICLE */
#define CB_START_GAME 5 /* I HAVE BOUGHT INTO MY LINKED SYSTEM */
#define CB_FINISHED 6 /* I have finished the Race */
#define CB_CAR_UPDATE 7 /* UPDATE DATA BLOCK FOR CAR */
#define CB_END_GAME 8 /* I HAVE ENDED THE GAME (ON MY SYSTEM) */
#define CB_HEAD2HEADT 9 /* LINKED MESSAGE */
#define CB_HEAD2HEADF 10 /* LINKED MESSAGE */
#define CB_LINKCANCELLED 11 /* cancelled the linkup */
#define CB_RACER_UPDATE 12 /* UPDATE DATA BLOCK FOR CAR */
#define CB_RACER_XSFER 13 /* XSFER DRONE TO OTHER MACHINE CONTROL */
#define CB_RACER_KILL 14 /* KILL CAR ID */
#define CB_RHO_CREATE 15 /* CREATE CAR ID */
#define CB_RHO_KILL 16 /* KILL CAR ID */
#define CB_RHO_XSFER 17 /* XSFER CAR ID */
#define CB_RHO_UPDATE 18 /* UPDATE CAR ID */
#define CB_FLY_KILL 19 /* KILL CAR ID */
#define CB_FLY_XSFER 20 /* XSFER CAR ID */
#define CB_FLY_UPDATE 21 /* UPDATE CAR ID */
#define CB_WAVEFL_READY 22
#define CB_WAVEFL_SET 23
#define CB_WAVEFL_GO 24
#define CB_OM_TRACK 25
#define CB_BONUS_SYNC0 26
#define CB_BONUS_SYNC1 27
#define CB_BONUS_SYNC2 28
#define CB_BONUS_SYNC3 29
#define CB_CHANGE_MUSIC 30
#define CB_COINDROP 31
#define CB_DIAGNOSTIC 32
#define CB_ATTRSND 33
#define CB_LASTMSG 34
#define COMMDP 0x0997000 /* FOR DP */
#define COMM_CTL 0x0997001 /* direction */
#define C_DE 0x04000 /* data enable */
#define C_CE 0x02000 /* control enable */
#define C_SND (C_DE|C_CE)
#define C_RCV C_CE
#define COMM_IO 0x0997000 /* data and control */
#define C_IRQ 0x00800 /* MA  NOT control, is actually interrupt 1 = INT, 0 = nothing */
#define C_C2 0x00400 /* MA  just control */
#define C_C1 0x00200 /* SL  just control */
#define C_C0 0x00100 /* SL  just control */
#define C_IRQE 0x08000 /* MA  NOT control enable, control enable */
#define C_CE2 0x04000 /* MA  just control */
#define C_CE1 0x02000 /* SL  just control */
#define C_CE0 0x01000 /* SL  just control */
#define C_DMASK 0x000FF
#define C_MAS (C_IRQE|C_CE2)
#define C_SLA (C_CE1|C_CE0)
#define COMM_BUFFER_SIZE 600
#define COMM_Q_FREEZE 0x99A /* LOGICAL == "I HAVE NOTHING TO SEND" */
#define COMM_Q_OPEN 0x878 /* LOGICAL == "I AM WORKING ON TRYING TO SEND SOMETHING" */
#define COMM_Q_READY 0x45B /* LOGICAL == "I HAVE SOMETHING READY TO SEND" */
#define OMS_VEHICLE_M 0x0E0
#define OMS_XVEH 0x080
#define OMS_RACE_M 0x00F
#define OMS_CPOINT_M 0x00F00 /* this tells the mode */
#define OMS_CPOINT1 0x00100
#define OMS_CPOINT2 0x00200
#define OMS_CPOINT3 0x00400
#define OMS_FINISHLINE 0x00800

#endif /* COMM_H */

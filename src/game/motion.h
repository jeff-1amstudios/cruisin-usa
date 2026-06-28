#ifndef MOTION_H
#define MOTION_H

#include "../core/port.h"

/* Generated from asm/MOTION.ASM. */

// MOTION.ASM
// asm: 	.bss	MOTION_NOT_ON,1
extern int MOTION_NOT_ON;
// asm: 	.bss	MOTION_STOP_HIT,1
extern int MOTION_STOP_HIT;
// asm: 	.bss	MOTION_RCV_TIKS,1
extern int MOTION_RCV_TIKS;
// asm: 	.bss	ABORT_QUERY,1
extern int ABORT_QUERY;
// asm: 	.bss	WAITTIK,1
extern int WAITTIK;
// asm: 	.bss	MOTION_SAFETY_ON,1
extern int MOTION_SAFETY_ON;
// asm: 	.bss	MOTION_SAFETY_TYPE,1
extern int MOTION_SAFETY_TYPE;
// asm: 	.bss	sPOTVALUE,15
extern int sPOTVALUE[];
// asm: 	.bss	MOTION_CMDSTR,80
extern int MOTION_CMDSTR[];
// asm: 	.bss	LAST_ZF,1
extern int LAST_ZF;
// asm: 	.bss	LAST_XL,1
extern int LAST_XL;
// asm: 	.bss	LAST_YR,1
extern int LAST_YR;
// asm: 	.bss	LATCHED_ERROR,1
extern int LATCHED_ERROR;
// asm: 	.bss	GALIL_STATUS_X,1
extern int GALIL_STATUS_X;
// asm: 	.bss	GALIL_STATUS_Y,1
extern int GALIL_STATUS_Y;
// asm: 	.bss	GALIL_STATUS_Z,1
extern int GALIL_STATUS_Z;
void CHECK_MOTION_DIP(void);
void CHECK_MOTION_PRESENT(void);
void SEND_MOTOR_OFF(void);
void SEND_CMD(void);
void WAIT_ACK(void);
void ABORT_RESET_GALIL(void);
void DIAG_MOTION_TESTX(void);
void DIAG_MOTION_TESTY(void);
void DIAG_MOTION_TESTZ(void);
void DIAG_MOTION_BURNIN(void);
void CLEAR_LATCH_ERROR(void);

#endif /* MOTION_H */

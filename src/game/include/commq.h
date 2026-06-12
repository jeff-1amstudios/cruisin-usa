#ifndef COMMQ_H
#define COMMQ_H

/* Generated from asm/COMMQ.ASM. */

// asm: 	.bss	IGNORE_UPDATES,1
extern int IGNORE_UPDATES;
// asm: 	fbss	COMMQ_TMP_BUFF,80
extern int COMMQ_TMP_BUFF[80];
// asm: 	phibss	SEND_BUFFER_A,COMM_BUFFER_SIZE
extern int SEND_BUFFER_A[COMM_BUFFER_SIZE];
// asm: 	pbss	SEND_BUFFER_A_LEN,1
extern int SEND_BUFFER_A_LEN;
// asm: 	pbss	RBUFF_LEN,1
extern int RBUFF_LEN;
// asm: 	phibss	RECEIVE_BUFFER,COMM_BUFFER_SIZE
extern int RECEIVE_BUFFER[COMM_BUFFER_SIZE];
// asm: 	pbss	OM_DIFF,1
extern int OM_DIFF;
// asm: 	pbss	OM_MODE,1
extern int OM_MODE;
// asm: 	pbss	OM_CHOSEN_RACE,1
extern int OM_CHOSEN_RACE;
// asm: 	pbss	OM_VEHICLE,1
extern int OM_VEHICLE;
// asm: 	pbss	OM_STATE,1
extern int OM_STATE;
// asm: 	pbss	MY_STATE,1
extern int MY_STATE;
// asm: 	pbss	OM_ATTR_MODE,1
extern int OM_ATTR_MODE;
// asm: 	pbss	OLD_OM_ATTR_MODE,1
extern int OLD_OM_ATTR_MODE;
// asm: 	pbss	OM_LINKWAIT,1
extern int OM_LINKWAIT;
// asm: 	.bss	MY_LINKWAIT,1
extern int MY_LINKWAIT;
// asm: 	pbss	LINKEDP,1
extern int LINKEDP;
// asm: 	pbss	OM_HIDDEN_ON,1
extern int OM_HIDDEN_ON;
// asm: 	pbss	OM_RACE_MODE,1
extern int OM_RACE_MODE;
// asm: 	.bss	BSYNC,1
extern int BSYNC;
// asm: 	pbss	OM_BSYNC,1
extern int OM_BSYNC;
// asm: 	pbss	OM_POSITION,1
extern int OM_POSITION;
// asm: 	pbss	HEAD2HEAD_ON,1
extern int HEAD2HEAD_ON;
// asm: 	.bss	SAVED_PLY2CAR,1
extern int SAVED_PLY2CAR;
// asm: 	.bss	COINDROP,1
extern int COINDROP;
// asm: 	fbss	OM_DIAGVALUE,1
extern int OM_DIAGVALUE;
// asm: 	fbss	DIAGVALUE,1
extern int DIAGVALUE;

#endif /* COMMQ_H */

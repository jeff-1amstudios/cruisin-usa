#ifndef COMM_H
#define COMM_H

/* Generated from asm/COMM.ASM. */

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

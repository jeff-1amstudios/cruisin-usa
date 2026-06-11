#ifndef COMM_H
#define COMM_H

/* Generated from asm/COMM.ASM. */

// asm: 	fbss	COMM_MASTER_ERROR_CNT,1
// addr: 0x00809A16
extern int COMM_MASTER_ERROR_CNT;
// asm: 	fbss	COMM_MASTER_TRANSES,1
// addr: 0x00809A17
extern int COMM_MASTER_TRANSES;
// asm: 	fbss	COMM_SLAVE_ERROR_CNT,1
// addr: 0x00809A18
extern int COMM_SLAVE_ERROR_CNT;
// asm: 	fbss	COMM_SLAVE_TRANSES,1
// addr: 0x00809A19
extern int COMM_SLAVE_TRANSES;
// asm: 	.bss	COMMFLAG,1		;COMMUNICATIONS IS OVER
// addr: 0x0000E5C1
extern int COMMFLAG;
// asm: 	pbss 	ONEFLAG,1
// addr: 0x0000C900
extern int ONEFLAG;
// asm: 	pbss	TRANSMISSION_ACTIVE,1	;1=TRUE
// addr: 0x0000C901
extern int TRANSMISSION_ACTIVE;
// asm: 	pbss	TRANSMISSION_DEAD,1	;1=DEAD LINK
// addr: 0x0000C902
extern int TRANSMISSION_DEAD;

#endif /* COMM_H */

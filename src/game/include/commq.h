#ifndef COMMQ_H
#define COMMQ_H

/* Generated from asm/COMMQ.ASM. */

// asm: 	.bss	IGNORE_UPDATES,1
#define IGNORE_UPDATES 0x0000E5C3
// asm: 	fbss	COMMQ_TMP_BUFF,80
#define COMMQ_TMP_BUFF 0x00809A1B
// asm: 	phibss	SEND_BUFFER_A,COMM_BUFFER_SIZE
#define SEND_BUFFER_A 0x0001F000
// asm: 	pbss	SEND_BUFFER_A_LEN,1
#define SEND_BUFFER_A_LEN 0x0000C903
// asm: 	pbss	RBUFF_LEN,1
#define RBUFF_LEN 0x0000C904
// asm: 	phibss	RECEIVE_BUFFER,COMM_BUFFER_SIZE
#define RECEIVE_BUFFER 0x0001F258
// asm: 	pbss	OM_DIFF,1
#define OM_DIFF 0x0000C905
// asm: 	pbss	OM_MODE,1
#define OM_MODE 0x0000C906
// asm: 	pbss	OM_CHOSEN_RACE,1
#define OM_CHOSEN_RACE 0x0000C907
// asm: 	pbss	OM_VEHICLE,1
#define OM_VEHICLE 0x0000C908
// asm: 	pbss	OM_STATE,1
#define OM_STATE 0x0000C909
// asm: 	pbss	MY_STATE,1
#define MY_STATE 0x0000C90A
// asm: 	pbss	OM_ATTR_MODE,1
#define OM_ATTR_MODE 0x0000C90B
// asm: 	pbss	OLD_OM_ATTR_MODE,1
#define OLD_OM_ATTR_MODE 0x0000C90C
// asm: 	pbss	OM_LINKWAIT,1
#define OM_LINKWAIT 0x0000C90D
// asm: 	.bss	MY_LINKWAIT,1
#define MY_LINKWAIT 0x0000E5C4
// asm: 	pbss	LINKEDP,1
#define LINKEDP 0x0000C90E
// asm: 	pbss	OM_HIDDEN_ON,1
#define OM_HIDDEN_ON 0x0080C90F
// asm: 	pbss	OM_RACE_MODE,1
#define OM_RACE_MODE 0x0080C910
// asm: 	.bss	BSYNC,1
#define BSYNC 0x0000E5C5
// asm: 	pbss	OM_BSYNC,1
#define OM_BSYNC 0x0000C911
// asm: 	pbss	OM_POSITION,1
#define OM_POSITION 0x0000C912
// asm: 	pbss	HEAD2HEAD_ON,1
#define HEAD2HEAD_ON 0x0000C913
// asm: 	.bss	SAVED_PLY2CAR,1
#define SAVED_PLY2CAR 0x0000E5C6
// asm: 	.bss	COINDROP,1
#define COINDROP 0x0000E5C7
// asm: 	fbss	OM_DIAGVALUE,1
#define OM_DIAGVALUE 0x00009A6B
// asm: 	fbss	DIAGVALUE,1
#define DIAGVALUE 0x00009A6C

#endif /* COMMQ_H */

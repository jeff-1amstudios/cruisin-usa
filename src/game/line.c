#include "../core/cpu.h"
#include "../core/machine.h"
#include "port.h"

/*
 * Source module: asm/LINE.ASM
 */

void _rectangle(void);
void EPI0_1(void);
void _line(void);
void EPI0_2(void);

#define FP AR3

/* ******************************************************
* FUNCTION DEF : _rectangle
******************************************************
 */
void _rectangle(void)
{
    // asm 0000AC2C: 	PUSH	FP
    // asm 0000AC2D: 	LDI	SP,FP
    // asm 0000AC2E: 	ADDI	3,SP
    // asm 0000AC2F: 	PUSH	R4
    // asm 0000AC30: 	PUSH	R5
    // *
    // * R4	assigned to parameter y2
    // * R5	assigned to parameter y1
    // *
    // asm 0000AC31: 	STI	RS,*+FP(1)
    // asm 0000AC32: 	STI	R3,*+FP(3)
    // asm 0000AC33: 	STI	AR2,*+FP(2)
    // asm 0000AC34: 	LDI	R2,R5
    // asm 0000AC35: 	LDI	RC,R4
    // asm 0000AC36:  PUSH DP
    // asm 0000AC37: 	LDI	R5,R2
    // asm 0000AC38: 	LDI	R5,RC
    // asm 0000AC39: 	CALL	_line
    // asm 0000AC3A: 	LDI	*+FP(2),AR2
    // asm 0000AC3B: 	LDI	R4,R2
    // asm 0000AC3C: 	LDI	*+FP(3),R3
    // asm 0000AC3D: 	LDI	R4,RC
    // asm 0000AC3E: 	LDI	*+FP(1),RS
    // asm 0000AC3F: 	CALL	_line
    // asm 0000AC40: 	LDI	*+FP(2),AR2
    // asm 0000AC41: 	LDI	R5,R2
    // asm 0000AC42: 	LDI	*+FP(2),R3
    // asm 0000AC43: 	LDI	R4,RC
    // asm 0000AC44: 	LDI	*+FP(1),RS
    // asm 0000AC45: 	CALL	_line
    // asm 0000AC46: 	LDI	*+FP(3),AR2
    // asm 0000AC47: 	LDI	R5,R2
    // asm 0000AC48: 	LDI	*+FP(3),R3
    // asm 0000AC49: 	LDI	R4,RC
    // asm 0000AC4A: 	LDI	*+FP(1),RS
    // asm 0000AC4B: 	CALL	_line
    // asm 0000AC4C:  POP DP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_rectangle", 0, 0);
    UNIMPL();
}

void EPI0_1(void)
{
    // asm 0000AC4D: 	LDI	*-FP(1),R1
    // asm 0000AC4E: 	LDI	*FP,FP
    // asm 0000AC4F: 	BD	R1
    // asm 0000AC50: 	POP	R5
    // asm 0000AC51: 	POP	R4
    // asm 0000AC52: 	SUBI	5,SP
    // ***	B	R1	;BRANCH OCCURS
    // ******************************************************
    // * FUNCTION DEF : _line
    // ******************************************************
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_1", 0, 0);
    UNIMPL();
}

void _line(void)
{
    // asm 0000AC53: 	PUSH	FP
    // asm 0000AC54: 	LDI	SP,FP
    // asm 0000AC55: 	ADDI	3,SP
    // asm 0000AC56: 	PUSH	R4
    // asm 0000AC57: 	PUSH	R5
    // asm 0000AC58: 	PUSHF	R6
    // asm 0000AC59: 	PUSHF	R7
    // *
    // * R2	assigned to variable  t
    // * R3	assigned to variable  t
    // * R4	assigned to variable  i
    // * R6	assigned to variable  x
    // * R6	assigned to variable  y
    // * R7	assigned to variable  xd
    // * R7	assigned to variable  yd
    // * RC	assigned to parameter y2
    // * RS	assigned to parameter x2
    // *
    // asm 0000AC5A: 	STI	RS,*+FP(1)
    // asm 0000AC5B: 	STI	R2,*+FP(3)
    // asm 0000AC5C: 	STI	AR2,*+FP(2)
    // asm 0000AC5D: 	LDI	R3,RS
    // asm 0000AC5E:  PUSH DP
    // asm 0000AC5F: 	CMPI	*+FP(2),RS
    // asm 0000AC60: 	BZ	L32
    // asm 0000AC61: 	CMPI	RC,R2
    // asm 0000AC62: 	BZ	L26
    // asm 0000AC63: 	SUBI	R2,RC,R0
    // asm 0000AC64: 	FLOAT	R0,R2
    // asm 0000AC65: 	LDF	R2,R0
    // asm 0000AC66: 	CALL	INV_F30
    // asm 0000AC67: 	RND	R0
    // asm 0000AC68: 	LDI	*+FP(2),R1
    // asm 0000AC69: 	SUBI	R1,RS,R1
    // asm 0000AC6A: 	FLOAT	R1,R3
    // asm 0000AC6B: 	MPYF	R0,R3,R7
    // asm 0000AC6C: 	CMPF	1.0,R7
    // asm 0000AC6D: 	BGT	L14
    // asm 0000AC6E: 	CMPF	-1.0,R7
    // asm 0000AC6F: 	BLT	L14
    // asm 0000AC70: 	CMPI	*+FP(3),RC
    // asm 0000AC71: 	BGE	L5
    // asm 0000AC72: 	LDI	*+FP(2),R2
    // asm 0000AC73: 	STI	RS,*+FP(2)
    // asm 0000AC74: 	LDI	*+FP(3),R3
    // asm 0000AC75: 	STI	RC,*+FP(3)
    // asm 0000AC76: 	LDI	R3,RC
    // asm 0000AC77: 	SUBI	*+FP(3),R3
    // asm 0000AC78: 	FLOAT	R3,R0
    // asm 0000AC79: 	CALL	INV_F30
    // asm 0000AC7A: 	RND	R0
    // asm 0000AC7B: 	SUBI	*+FP(2),R2
    // asm 0000AC7C: 	FLOAT	R2,R7
    // asm 0000AC7D: 	MPYF	R0,R7
L5:
    // asm 0000AC7E: 	FLOAT	*+FP(2),R6
    // asm 0000AC7F: 	CMPF	0,R7
    // asm 0000AC80: 	BGT	L10
    // asm 0000AC81: 	LDI	*+FP(3),R4
    // asm 0000AC82: 	CMPI	RC,R4
    // asm 0000AC83: 	BGT	L38
    // asm 0000AC84: 	SUBI	R4,RC,R5
L44:
    // asm 0000AC85: 	FIX	R6,AR2
    // asm 0000AC86: 	LDI	R4,R2
    // asm 0000AC87: 	LDI	*+FP(1),R3
    // asm 0000AC88: 	CALL	_pixel
    // asm 0000AC89: 	SUBI	1,R5
    // asm 0000AC8A: 	BGED	L44
    // asm 0000AC8B: 	ADDF	R7,R6
    // asm 0000AC8C: 	NOP
    // asm 0000AC8D: 	ADDI	1,R4
    // ***	BGE	L44	;BRANCH OCCURS
    // asm 0000AC8E: 	B	L38
L10:
    // asm 0000AC8F: 	LDI	*+FP(3),R4
    // asm 0000AC90: 	CMPI	RC,R4
    // asm 0000AC91: 	BGT	L38
    // asm 0000AC92: 	SUBI	R4,RC,R5
L43:
    // asm 0000AC93: 	FIX	R6,AR2
    // asm 0000AC94: 	LDI	R4,R2
    // asm 0000AC95: 	LDI	*+FP(1),R3
    // asm 0000AC96: 	CALL	_pixel
    // asm 0000AC97: 	SUBI	1,R5
    // asm 0000AC98: 	BGED	L43
    // asm 0000AC99: 	ADDF	R7,R6
    // asm 0000AC9A: 	NOP
    // asm 0000AC9B: 	ADDI	1,R4
    // ***	BGE	L43	;BRANCH OCCURS
    // asm 0000AC9C: 	B	L38
L14:
    // asm 0000AC9D: 	CMPI	*+FP(2),RS
    // asm 0000AC9E: 	BLT	L16
    // asm 0000AC9F: 	LDF	R3,R0
    // asm 0000ACA0: 	CALL	INV_F30
    // asm 0000ACA1: 	BD	L17
    // asm 0000ACA2: 	RND	R0
    // asm 0000ACA3: 	NOP
    // asm 0000ACA4: 	MPYF	R0,R2,R7
    // ***	B	L17	;BRANCH OCCURS
L16:
    // asm 0000ACA5: 	LDI	*+FP(2),R2
    // asm 0000ACA6: 	STI	RS,*+FP(2)
    // asm 0000ACA7: 	LDI	R2,RS
    // asm 0000ACA8: 	LDI	*+FP(3),R2
    // asm 0000ACA9: 	STI	RC,*+FP(3)
    // asm 0000ACAA: 	LDI	*+FP(2),R0
    // asm 0000ACAB: 	SUBI	R0,RS,R1
    // asm 0000ACAC: 	FLOAT	R1,R0
    // asm 0000ACAD: 	CALL	INV_F30
    // asm 0000ACAE: 	RND	R0
    // asm 0000ACAF: 	SUBI	*+FP(3),R2
    // asm 0000ACB0: 	FLOAT	R2,R7
    // asm 0000ACB1: 	MPYF	R0,R7
L17:
    // asm 0000ACB2: 	FLOAT	*+FP(3),R6
    // asm 0000ACB3: 	CMPF	0,R7
    // asm 0000ACB4: 	BGT	L22
    // asm 0000ACB5: 	LDI	*+FP(2),R4
    // asm 0000ACB6: 	CMPI	RS,R4
    // asm 0000ACB7: 	BGT	L38
    // asm 0000ACB8: 	SUBI	R4,RS,R5
L42:
    // asm 0000ACB9: 	LDI	R4,AR2
    // asm 0000ACBA: 	FIX	R6,R2
    // asm 0000ACBB: 	LDI	*+FP(1),R3
    // asm 0000ACBC: 	CALL	_pixel
    // asm 0000ACBD: 	SUBI	1,R5
    // asm 0000ACBE: 	BGED	L42
    // asm 0000ACBF: 	ADDF	R7,R6
    // asm 0000ACC0: 	NOP
    // asm 0000ACC1: 	ADDI	1,R4
    // ***	BGE	L42	;BRANCH OCCURS
    // asm 0000ACC2: 	B	L38
L22:
    // asm 0000ACC3: 	LDI	*+FP(2),R4
    // asm 0000ACC4: 	CMPI	RS,R4
    // asm 0000ACC5: 	BGT	L38
    // asm 0000ACC6: 	SUBI	R4,RS,R5
L41:
    // asm 0000ACC7: 	LDI	R4,AR2
    // asm 0000ACC8: 	FIX	R6,R2
    // asm 0000ACC9: 	LDI	*+FP(1),R3
    // asm 0000ACCA: 	CALL	_pixel
    // asm 0000ACCB: 	SUBI	1,R5
    // asm 0000ACCC: 	BGED	L41
    // asm 0000ACCD: 	ADDF	R7,R6
    // asm 0000ACCE: 	NOP
    // asm 0000ACCF: 	ADDI	1,R4
    // ***	BGE	L41	;BRANCH OCCURS
    // asm 0000ACD0: 	B	L38
L26:
    // asm 0000ACD1: 	CMPI	*+FP(2),RS
    // asm 0000ACD2: 	BGE	L28
    // asm 0000ACD3: 	LDI	*+FP(2),R2
    // asm 0000ACD4: 	STI	RS,*+FP(2)
    // asm 0000ACD5: 	LDI	R2,RS
L28:
    // asm 0000ACD6: 	LDI	*+FP(2),R4
    // asm 0000ACD7: 	CMPI	RS,R4
    // asm 0000ACD8: 	BGT	L38
    // asm 0000ACD9: 	SUBI	R4,RS,R5
L40:
    // asm 0000ACDA: 	LDI	R4,AR2
    // asm 0000ACDB: 	LDI	*+FP(3),R2
    // asm 0000ACDC: 	LDI	*+FP(1),R3
    // asm 0000ACDD: 	CALL	_pixel
    // asm 0000ACDE: 	ADDI	1,R4
    // asm 0000ACDF: 	SUBI	1,R5
    // asm 0000ACE0: 	BGE	L40
    // asm 0000ACE1: 	B	L38
L32:
    // asm 0000ACE2: 	CMPI	RC,R2
    // asm 0000ACE3: 	BLE	L34
    // asm 0000ACE4: 	STI	RC,*+FP(3)
    // asm 0000ACE5: 	LDI	R2,RC
L34:
    // asm 0000ACE6: 	LDI	*+FP(3),R4
    // asm 0000ACE7: 	CMPI	RC,R4
    // asm 0000ACE8: 	BGT	L38
    // asm 0000ACE9: 	SUBI	R4,RC,R5
L39:
    // asm 0000ACEA: 	LDI	*+FP(2),AR2
    // asm 0000ACEB: 	LDI	R4,R2
    // asm 0000ACEC: 	LDI	*+FP(1),R3
    // asm 0000ACED: 	CALL	_pixel
    // asm 0000ACEE: 	ADDI	1,R4
    // asm 0000ACEF: 	SUBI	1,R5
    // asm 0000ACF0: 	BGE	L39
L38:
    // asm 0000ACF1:  POP DP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "_line", 0, 0);
    UNIMPL();
}

void EPI0_2(void)
{
    // asm 0000ACF2: 	LDI	*-FP(1),R1
    // asm 0000ACF3: 	LDI	*FP,FP
    // asm 0000ACF4: 	POPF	R7
    // asm 0000ACF5: 	POPF	R6
    // asm 0000ACF6: 	BD	R1
    // asm 0000ACF7: 	POP	R5
    // asm 0000ACF8: 	POP	R4
    // asm 0000ACF9: 	SUBI	5,SP
    // ***	B	R1	;BRANCH OCCURS
    // ******************************************************
    // * UNDEFINED REFERENCES                               *
    // ******************************************************
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_2", 0, 0);
    UNIMPL();
}

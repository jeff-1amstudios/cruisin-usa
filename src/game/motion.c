#include "../core/cpu.h"
#include "../core/machine.h"
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
#include "diag.h"
#include "motion.h"

/*
 * Source module: asm/MOTION.ASM
 */

void INITIALIZATION_MOTION_CHECK(void);
void JAJA66(void);
void NO_ERRORMIL(void);
void NO_UP_SAFETY_ERROR(void);
void GOT_ALL_UPPERS(void);
void LLLLT(void);
void NO_LO_SAFETY_ERROR(void);
void GOT_ALL_LOWERS(void);
void SEND_MOTOR_OFF(void);
void SEND_MOTOR_OFF_NO_RESET(void);
void MOTOFF_DONE(void);
void NNE1(void);
void WAIT500(void);
void CHECK_MOTION_DIP(void);
void CHECK_MOTION_PRESENT(void);
void DIAG_MOTION_TESTZ(void);
void DIAG_MOTION_TESTX(void);
void DIAG_MOTION_TESTY(void);
void DIAG_MOTION_BURNIN(void);
void JAJA(void);
void PLMOTION(void);
void NO_NEW_MOTION_ERRORS(void);
void NO_PREVMOTION_ERRORS(void);
void KKGK(void);
void NOTTHIS(void);
void MOTION_SCALE_ENTER(void);
void N76(void);
void LEVEL_THE_MOTION(void);
void DOWNLOAD_PROGRAM(void);
void DNF(void);
void SEND_CMD(void);
void KKKII(void);
void RESET_GALIL(void);
void ABORT_RESET_GALIL(void);
void MOTION_DLPROG(void);
void GALIL_ERR1(void);
void WAITMSEC(void);
void CHECK_MOTION_ERROR(void);
void CME_NO_MOTION_ERRORS(void);
void CLEAR_LATCH_ERROR(void);
void LATCH_ERROR(void);
void G_READY(void);
void G_CHAR(void);
void G_STRING(void);
void NOTPERR(void);
void NOTQSTATUS(void);
void NOTQQERRORX(void);
void NOTQQERRORY(void);
void SHOWENCODER(void);
void NOTQQERRORZ(void);
void GS1(void);
void GS2(void);
void GSX(void);
void GHX(void);
void ASCIIHEX(void);
void ASCERR(void);
void WAIT_ACK(void);
void EXITL2(void);
void NO_RESPONSE(void);
void WAIT_ACK_REAL(void);
void WAR1(void);
void EXITL2A(void);
void UPPER_LIMIT_ERROR(void);
void LOWER_LIMIT_ERROR(void);

#define MIN_MOVE_DIST 200
/* asm: MOTION_NOT_ON	.bss	MOTION_NOT_ON,1 */
int MOTION_NOT_ON;
/* asm: MOTION_STOP_HIT	.bss	MOTION_STOP_HIT,1 */
int MOTION_STOP_HIT;
/* asm: MOTION_RCV_TIKS	.bss	MOTION_RCV_TIKS,1 */
int MOTION_RCV_TIKS;
/* asm: MOTOFF_PROG */
/* asm: 	.word	MOTOROFF1 */
/* asm: 	.word	MOTOROFF2 */
/* asm: 	.word	MOTOROFF3 */
/* asm: 	.word	MOTOROFF4 */
/* asm: 	.word	MOTOROFF5 */
/* asm: 	.word	MOTOROFF6 */
/* asm: 	.word	MOTOROFF7 */
/* asm: 	.word	MOTOROFF8 */
/* asm: 	.word	MOTOROFF9 */
/* asm: 	.word	MOTOROFF10 */
/* asm: 	.word	MOTOROFF11 */
/* asm: 	.word	MOTOROFF12 */
/* asm: 	.word	0 */
int MOTOFF_PROG[] = {
    MOTOROFF1,
    MOTOROFF2,
    MOTOROFF3,
    MOTOROFF4,
    MOTOROFF5,
    MOTOROFF6,
    MOTOROFF7,
    MOTOROFF8,
    MOTOROFF9,
    MOTOROFF10,
    MOTOROFF11,
    MOTOROFF12,
    0,
};
/* asm: BABAID	.word	JJG */
int *BABAID = JJG;
/* asm: JJG	.word	MM0,MM1,MM2,MM3,MM4,MM5,MM6,MM7 */
const char *JJG[] = {
    MM0, MM1, MM2, MM3, MM4, MM5, MM6, MM7,
};
const char *MM0 = "UNKNOWN ERROR";
const char *MM1 = "MAT NOT PLUGGED IN";
const char *MM2 = "MAT STEPPED ON";
const char *MM3 = "SAFETY BEAM PATH BROKEN";
const char *MM4 = "SAFETY BEAM DETECTOR NOT RECEIVING";
const char *MM5 = "SAFETY BEAM LIGHT NOT EMITTING";
const char *MM6 = "FAIL SAFE SWITCH ENGAGED";
const char *MM7 = "FAIL SAFE SWITCH NOT CONNECT PROPERLY";
const char *bbd = "MOTION BURNIN";
/* asm: ABORT_QUERY	.bss	ABORT_QUERY,1 */
int ABORT_QUERY;
#define MOTION_ERROR_TIKS ((57*5))
/* asm: WAITTIK	.bss	WAITTIK,1 */
int WAITTIK;
/* asm: MOTION_SAFETY_ON	.bss	MOTION_SAFETY_ON,1 */
int MOTION_SAFETY_ON;
/* asm: MOTION_SAFETY_TYPE	.bss	MOTION_SAFETY_TYPE,1 */
int MOTION_SAFETY_TYPE;
const char *MSSM = "MOTION STOP BUTTON HIT";
/* asm: JAJA555	JAJA555
	.bss	sPOTVALUE,15 */
int JAJA555[sPOTVALUE];
/* asm: MOTION_CMDSTR	.bss	MOTION_CMDSTR,80 */
int MOTION_CMDSTR[80];
/* asm: LAST_ZF	.bss	LAST_ZF,1 */
int LAST_ZF;
/* asm: LAST_XL	.bss	LAST_XL,1 */
int LAST_XL;
/* asm: LAST_YR	.bss	LAST_YR,1 */
int LAST_YR;
/* asm: THEPROGL */
/* asm: 	.word	EDIT,T5,T6,T7,T8,T9 */
/* asm: 	.word	T10,T11,T12,T13,T14,T15,T16,T17,T18,T19 */
/* asm: 	.word	T20,T21,T22,T23,T24,T25,T26,T27,T28,T29 */
/* asm: 	.word	T30,T31,T32,T33,T34,T35,T36,T37,T38,T39 */
/* asm: 	.word	T40,T41,T42,T43,T44,T45,T46,T47,T48,T49 */
/* asm: 	.word	T50,T51,T52,T53,T54,T55,T56,T57,T58,T59 */
/* asm: 	.word	T60 */
/* asm: 	.word	T1,T2,T3 */
/* asm: 	.word	T66X,T67X,T68X */
/* asm: 	.word	T66Y,T67Y,T68Y */
/* asm: 	.word	T66Z,T67Z,T68Z */
/* asm: 	.word	T69 */
/* asm: 	.word	T70,T71,T72,T73,T74,T75,T76,T77,T78,T79 */
/* asm: 	.word	T80,T81,T82,T83,T84 */
/* asm: 	.word	0 */
int THEPROGL[] = {
    EDIT, T5, T6, T7, T8, T9,
    T10, T11, T12, T13, T14, T15, T16, T17, T18, T19,
    T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
    T40, T41, T42, T43, T44, T45, T46, T47, T48, T49,
    T50, T51, T52, T53, T54, T55, T56, T57, T58, T59,
    T60,
    T1, T2, T3,
    T66X, T67X, T68X,
    T66Y, T67Y, T68Y,
    T66Z, T67Z, T68Z,
    T69,
    T70, T71, T72, T73, T74, T75, T76, T77, T78, T79,
    T80, T81, T82, T83, T84,
    0,
};
/* *----------------------------------------------------------------------------
*
*
*RETURNS
*	C	IF ERROR
*	NC	IF NOT ERROR
*
*
 */
/* asm: CME_MASK	.word	0FF80h */
int CME_MASK = 0x0FF80;
/* asm: LATCHED_ERROR	.bss	LATCHED_ERROR,1 */
int LATCHED_ERROR;
/* asm: LLG6X	.word	L4ABX */
const char *LLG6X = L4ABX;
/* asm: LLG6Y	.word	L4ABY */
const char *LLG6Y = L4ABY;
/* asm: LLG6Z	.word	L4ABZ */
const char *LLG6Z = L4ABZ;
const char *L4ABX = "X ENCODER FAILURE";
const char *L4ABY = "Y ENCODER FAILURE";
const char *L4ABZ = "Z ENCODER FAILURE";
/* asm: GALIL_STATUS_X	.bss	GALIL_STATUS_X,1 */
int GALIL_STATUS_X;
/* asm: GALIL_STATUS_Y	.bss	GALIL_STATUS_Y,1 */
int GALIL_STATUS_Y;
/* asm: GALIL_STATUS_Z	.bss	GALIL_STATUS_Z,1 */
int GALIL_STATUS_Z;
const char *PXC1 = "XA=";
const char *PYC1 = "YA=";
const char *PZC1 = "ZA=";

/* *----------------------------------------------------------------------------
*This is the initial check of the galil board.
*The CMOS value will be set to ON if cool else OFF
*No other routine may turn it ON.
*No other routine will ever download or initialize galil
*This routine may only be called at powerup
*
*
*
*
 */
void INITIALIZATION_MOTION_CHECK(void)
{
    // asm 00004541: 	CALL	CLEAR_LATCH_ERROR
    // asm 00004542: 	LDI	1,R2
    // asm 00004543: 	SETAUD	ADJ_MOTION_PRESENT
    // asm 00004545: 	LDI	0,R2
    // asm 00004546: 	SETAUD	AUD_RESET_TOTALLY
    // asm 00004548: 	CALL	CHECK_MOTION_DIP
    // asm 00004549: 	RETSNZ
    // asm 0000454A: 	LDI	1,RC
    // asm 0000454B: 	TEXTITT	"MOTION SETUP TESTING...",1,250
    // asm 0000454F:         CALL    RESET_GALIL
    // asm 00004550:         CALL    WAIT_ACK_REAL
    // asm 00004551: 	CALL	CHECK_MOTION_PRESENT
    // asm 00004552: 	BNE	INIT_MOTION_ERROROUT
    // ;send it UP, for .5 inch  (1000)
    // ;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITIALIZATION_MOTION_CHECK", 0, 0);
    UNIMPL();
}

void JAJA66(void)
{
    // asm 0000456A: LDL	MINIT9,AR2	;lift off the failsafes (PR 4000,4000,4000)
JAJA887:
    // asm 0000456B: CALL    SEND_CMD
    // asm 0000456C:         CALL    WAIT_ACK
    // asm 0000456D: 	LDI	100,AR3
GLLLT:
    // asm 0000456E: CALL	WAIT_FOR_VBLANK
    // asm 0000456F: 	DBU	AR3,GLLLT
    // asm 00004570: 	CALL	ABORT_RESET_GALIL
    // 	;
    // 	;check for safety errors
    // 	;hold for 10 seconds until all errors
    // 	;are cleared.
    // 	;
SAFETY_ERROR_CHK:
    // asm 00004571: 	LDI	57*10,AR3
    // asm 00004572: MILOOP2
    // asm 00004572: 	CLRI	AR2
    // asm 00004573: 	LDP	@991030h
    // asm 00004574: 	LDI	@991030h,R0
    // asm 00004575: 	LDI	*AR2,AR2
    // asm 00004576: 	SETDP
    // asm 00004577: 	AND	@CME_MASK,R0
    // asm 00004578: 	BZ	NO_ERRORMIL
    // asm 00004579: 	LDI	1,RC
    // asm 0000457A: 	TEXTITT	"MOTION ERROR",50,270
    // asm 0000457E: 	TEXTITT	"CLEAR MATS AND BEAM TO CONTINUE",50,280
    // asm 00004582: 	CALL	WAIT_FOR_VBLANK
    // asm 00004583: 	DBU	AR3,MILOOP2
    // asm 00004584: 	BU	INIT_MOTION_ERROROUT
    // 	;
    // 	;no safety errors, continue on
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JAJA66", 0, 0);
    UNIMPL();
}

void NO_ERRORMIL(void)
{
    // asm 00004585: 	LDI	2000,R0
    // asm 00004586: 	CALL	WAITMSEC
    // asm 00004587: 	CALL	ABORT_RESET_GALIL
    // asm 00004588: 	FILL	50,270,500,289,0
    // asm 0000458E: 	LDL	MINIT1,AR2	;send it up
    // asm 0000458F:         CALL    SEND_CMD
    // asm 00004590:         CALL    WAIT_ACK
    // asm 00004591: 	LDI	10,AR3
LLLT:
    // asm 00004592: CALL	WAIT_FOR_VBLANK
    // asm 00004593: 	DBU	AR3,LLLT
    // asm 00004594: 	LDI	-1,R0
    // asm 00004595: 	STI	R0,@GALIL_STATUS_X
    // asm 00004596: 	STI	R0,@GALIL_STATUS_Y
    // asm 00004597: 	STI	R0,@GALIL_STATUS_Z
    // asm 00004598: 	LDI	57*9,AR3
    // asm 00004599: MILOOP1
    // asm 00004599: 	PUSH	AR3
    // asm 0000459A: 	LDL	MINITQX,AR2	;query X
    // asm 0000459B:         CALL    SEND_CMD
    // asm 0000459C:         CALL    WAIT_ACK
    // asm 0000459D: 	LDL	MINITQY,AR2	;query Y
    // asm 0000459E:         CALL    SEND_CMD
    // asm 0000459F:         CALL    WAIT_ACK
    // asm 000045A0: 	LDL	MINITQZ,AR2	;query Z
    // asm 000045A1:         CALL    SEND_CMD
    // asm 000045A2:         CALL    WAIT_ACK
    // asm 000045A3: 	POP	AR3
    // asm 000045A4: 	CALL	WAIT_FOR_VBLANK
    // 	;insert checking for limit switches
    // 	;
    // 	;
    // asm 000045A5: 	LDI	@GALIL_STATUS_X,R0
    // asm 000045A6: 	TSTB	08h,R0
    // asm 000045A7: 	BNZ	NO_UP_LIM_YET
    // asm 000045A8: 	LDI	@GALIL_STATUS_Y,R0
    // asm 000045A9: 	TSTB	08h,R0
    // asm 000045AA: 	BNZ	NO_UP_LIM_YET
    // asm 000045AB: 	LDI	@GALIL_STATUS_Z,R0
    // asm 000045AC: 	TSTB	08h,R0
    // asm 000045AD: 	BZ	GOT_ALL_UPPERS
    // 	;if all limit switches are activated, then exit
NO_UP_LIM_YET:
    // asm 000045AE: 	CLRI	AR2
    // asm 000045AF: 	LDP	@991030h
    // asm 000045B0: 	LDI	@991030h,R0
    // asm 000045B1: 	LDI	*AR2,AR2
    // asm 000045B2: 	SETDP
    // asm 000045B3: 	AND	@CME_MASK,R0
    // asm 000045B4: 	BZ	NO_UP_SAFETY_ERROR
    // asm 000045B5: 	BU	SAFETY_ERROR_CHK
    // 	;check for mats or optos
    // 	;start over if error
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NO_ERRORMIL", 0, 0);
    UNIMPL();
}

void NO_UP_SAFETY_ERROR(void)
{
    // asm 000045B6: 	DBU	AR3,MILOOP1
    // asm 000045B7: 	BU	UPPER_LIMIT_ERROR
    // 	;here we would error out
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NO_UP_SAFETY_ERROR", 0, 0);
    UNIMPL();
}

void GOT_ALL_UPPERS(void)
{
    // 	;check failsafes
    // 	;
    // 	;
    // asm 000045B8: 	CALL	CHECK_MOTION_ERROR
    // asm 000045B9: 	BC	INIT_MOTION_ERROROUT
    // asm 000045BA: 	CLRI	AR2
    // asm 000045BB: 	LDP	@991030h
    // asm 000045BC: 	LDI	@991030h,R0
    // asm 000045BD: 	LDI	*AR2,AR2
    // asm 000045BE: 	SETDP
    // asm 000045BF: 	AND	@CME_MASK,R0
    // asm 000045C0: 	RS	8,R0
    // asm 000045C1: 	AND	0F0h,R0
    // asm 000045C2: 	CMPI	060h,R0
    // asm 000045C3: 	BGE	INIT_MOTION_ERROROUT
    // 	;
    // 	;---> BR if FAILSAFES STILL ON
    // 	;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GOT_ALL_UPPERS", 0, 0);
    UNIMPL();
}

void LLLLT(void)
{
    // asm 000045CD: CALL	WAIT_FOR_VBLANK
    // asm 000045CE: 	DBU	AR3,LLLLT
    // 	;
    // 	;check lower limits
    // 	;
    // asm 000045CF: 	LDI	57*9,AR3
    // asm 000045D0: MILOOP1LOW
    // asm 000045D0: 	PUSH	AR3
    // asm 000045D1: 	LDL	MINITQX,AR2	;query X
    // asm 000045D2:         CALL    SEND_CMD
    // asm 000045D3:         CALL    WAIT_ACK
    // asm 000045D4: 	LDL	MINITQY,AR2	;query Y
    // asm 000045D5:         CALL    SEND_CMD
    // asm 000045D6:         CALL    WAIT_ACK
    // asm 000045D7: 	LDL	MINITQZ,AR2	;query Z
    // asm 000045D8:         CALL    SEND_CMD
    // asm 000045D9:         CALL    WAIT_ACK
    // asm 000045DA: 	POP	AR3
    // asm 000045DB: 	CALL	WAIT_FOR_VBLANK
    // 	;insert checking for limit switches
    // 	;
    // 	;
    // asm 000045DC: 	LDI	@GALIL_STATUS_X,R0
    // asm 000045DD: 	TSTB	04h,R0
    // asm 000045DE: 	BNZ	NO_LO_LIM_YET
    // asm 000045DF: 	LDI	@GALIL_STATUS_Y,R0
    // asm 000045E0: 	TSTB	04h,R0
    // asm 000045E1: 	BNZ	NO_LO_LIM_YET
    // asm 000045E2: 	LDI	@GALIL_STATUS_Z,R0
    // asm 000045E3: 	TSTB	04h,R0
    // asm 000045E4: 	BZ	GOT_ALL_LOWERS
    // 	;if all limit switches are activated, then exit
NO_LO_LIM_YET:
    // asm 000045E5: 	CLRI	AR2
    // asm 000045E6: 	LDP	@991030h
    // asm 000045E7: 	LDI	@991030h,R0
    // asm 000045E8: 	LDI	*AR2,AR2
    // asm 000045E9: 	SETDP
    // asm 000045EA: 	AND	@CME_MASK,R0
    // asm 000045EB: 	BZ	NO_LO_SAFETY_ERROR
    // asm 000045EC: 	BU	SAFETY_ERROR_CHK
    // 	;check for mats or optos
    // 	;start over if error
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LLLLT", 0, 0);
    UNIMPL();
}

void NO_LO_SAFETY_ERROR(void)
{
    // asm 000045ED: 	DBU	AR3,MILOOP1LOW
    // asm 000045EE: 	BU	LOWER_LIMIT_ERROR
    // 	;here we would error out
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NO_LO_SAFETY_ERROR", 0, 0);
    UNIMPL();
}

void GOT_ALL_LOWERS(void)
{
    // 	;
    // 	;initialize the stuff
    // 	;
    // asm 000045EF:         LDL     PP,AR2
    // asm 000045F0:         CALL    SEND_CMD
    // asm 000045F1:         CALL    WAIT_ACK
    // asm 000045F2: 	CALL	CHECK_MOTION_PRESENT
    // asm 000045F3: 	BNE	INIT_MOTION_ERROROUT
    // asm 000045F4: 	LDI	57*3,AR3
LLLTA:
    // asm 000045F5: CALL	WAIT_FOR_VBLANK
    // asm 000045F6: 	DBU	AR3,LLLTA
    // asm 000045F7:         LDL     PP1,AR2
    // asm 000045F8:         CALL    SEND_CMD
    // asm 000045F9:         CALL    WAIT_ACK
    // asm 000045FA:         LDL     PP2,AR2
    // asm 000045FB:         CALL    SEND_CMD
    // asm 000045FC:         CALL    WAIT_ACK
    // asm 000045FD:         LDL     PP3,AR2
    // asm 000045FE:         CALL    SEND_CMD
    // asm 000045FF:         CALL    WAIT_ACK
    // asm 00004600:         LDL     PP4,AR2
    // asm 00004601:         CALL    SEND_CMD
    // asm 00004602:         CALL    WAIT_ACK
    // 	;hang out for a moment
    // asm 00004603: 	CALL	WAIT_FOR_VBLANK
    // 	;
    // 	;now dl the program.
    // 	;
    // asm 00004604: 	CALL	CLEAR_LATCH_ERROR
    // asm 00004605: 	CALL	ABORT_RESET_GALIL
    // asm 00004606: 	CALL	MOTION_DLPROG
    // asm 00004607: 	CLRI	AR2
    // asm 00004608: 	LDP	@991030h
    // asm 00004609: 	LDI	@991030h,R0
    // asm 0000460A: 	LDI	*AR2,AR2
    // asm 0000460B: 	SETDP
    // asm 0000460C: 	AND	@CME_MASK,R0
    // asm 0000460D: 	BNZ	INIT_MOTION_ERROROUT
    // asm 0000460E: 	LDI	@LATCHED_ERROR,R0
    // asm 0000460F: 	BNZ	INIT_MOTION_ERROROUT
    // 	;
    // 	;execute the program
    // 	;we are now guarenteed to be cool
    // 	;
    // asm 00004610: 	FILL	1,250,500,350,0
    // asm 00004616: 	LDI	1,RC
    // asm 00004617: 	TEXTITT	"MOTION SYSTEM OK",300,240
    // asm 0000461B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GOT_ALL_LOWERS", 0, 0);
    UNIMPL();
}

void SEND_MOTOR_OFF(void)
{
    // asm 0000461C:         CALL    RESET_GALIL
    // asm 0000461D:         CALL    WAIT_ACK_REAL
    // 	;bong sound
    // asm 0000461E: 	LDI	984,AR2
    // asm 0000461F: 	CALL	SENDSND
    // 	;2.5 sec delay
    // asm 00004620: 	LDI	2500,R0
    // asm 00004621: 	CALL	WAITMSEC
    // asm 00004622: 	LDL	MOTOROFF12,AR2
    // asm 00004623: 	CALL	SEND_CMD
    // asm 00004624:         CALL    WAIT_ACK
    // asm 00004625: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_MOTOR_OFF", 0, 0);
    UNIMPL();
}

void SEND_MOTOR_OFF_NO_RESET(void)
{
    // asm 00004626: 	LDL	MOTOROFF0,AR2
    // asm 00004627: 	CALL	SEND_CMD
    // asm 00004628: 	CALL    WAIT_ACK
    // asm 00004629: 	FILL	1,200,500,390,0
    // asm 0000462F: 	LDI	500,R0			;WAIT
    // asm 00004630: 	CALL	WAITMSEC
    // asm 00004631: 	LDL	MOTOFF_PROG,AR4
MOTOFF_LOOP:
    // asm 00004632: 	LDI	1,RC
    // asm 00004633: 	TEXTITT	"SLOWLY TURNING OFF MOTORS",1,200
    // asm 00004637: 	LDI	984,AR2			;BONG
    // asm 00004638: 	CALL	SENDSND
    // asm 00004639: 	LDI	2000,R0			;WAIT
    // asm 0000463A: 	CALL	WAITMSEC
    // asm 0000463B: 	LDI	*AR4++,AR2		;SEND NEXT TORQUE LEVEL
    // asm 0000463C: 	CMPI	0,AR2
    // asm 0000463D: 	BEQ	MOTOFF_DONE
    // asm 0000463E: 	CALL	SEND_CMD
    // asm 0000463F: 	CALL	WAIT_ACK
    // asm 00004640: 	FILL	1,200,500,390,0		;CLEAR MESSAGE
    // asm 00004646: 	LDI	500,R0			;WAIT
    // asm 00004647: 	CALL	WAITMSEC
    // asm 00004648: 	LDI	*AR4++,AR2		;SEND NEXT TORQUE LEVEL
    // asm 00004649: 	CMPI	0,AR2
    // asm 0000464A: 	BEQ	MOTOFF_DONE
    // asm 0000464B: 	CALL	SEND_CMD
    // asm 0000464C: 	CALL	WAIT_ACK
    // asm 0000464D: 	BU	MOTOFF_LOOP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_MOTOR_OFF_NO_RESET", 0, 0);
    UNIMPL();
}

void MOTOFF_DONE(void)
{
    // asm 0000464E: 	FILL	1,200,500,390,0
    // asm 00004654: 	RETS
INIT_MOTION_ERROROUT:
    // asm 00004655: 	CALL    SEND_MOTOR_OFF_NO_RESET
    // asm 00004656: 	CLRI	R2
    // asm 00004657: 	SETAUD	ADJ_MOTION_PRESENT
    // 	;show message on screen
    // 	;
    // asm 00004659: 	FILL	1,250,500,350,0
    // asm 0000465F: 	LDI	4,AR5
GDLPAA:
    // asm 00004660: LDI	1,RC
    // asm 00004661: 	TEXTITT	"MOTION SYSTEM HAS ERRORS",100,250
    // asm 00004665: 	TEXTITT	"MOTION SYSTEM DEACTIVATED",100,260
    // asm 00004669: 	LDI	984,AR2
    // asm 0000466A: 	CALL	SENDSND
    // asm 0000466B: 	LDI	450,R0
    // asm 0000466C: 	CALL	WAITMSEC
    // asm 0000466D: 	FILL	1,250,500,350,0
    // asm 00004673: 	LDI	150,R0
    // asm 00004674: 	CALL	WAITMSEC
    // asm 00004675: 	DBU	AR5,GDLPAA
    // asm 00004676: 	CLRI	AR2
    // asm 00004677: 	LDP	@991030h
    // asm 00004678: 	LDI	@991030h,R0
    // asm 00004679: 	LDI	*AR2,AR2
    // asm 0000467A: 	SETDP
    // asm 0000467B: 	RS	12,R0		;right justify
    // asm 0000467C: 	AND	0Fh,R0
    // asm 0000467D: 	CMPI	8,R0
    // asm 0000467E: 	BLT	NNE1
    // asm 0000467F: 	LDI	1,RC
    // asm 00004680: 	TEXTITT	"BOARD NOT PLUGGED IN",100,250
    // asm 00004684: 	BU	FFFD
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTOFF_DONE", 0, 0);
    UNIMPL();
}

void NNE1(void)
{
    // asm 00004685: 	LDI	R0,AR2
    // asm 00004686: 	ADDI	@BABAID,AR2
    // asm 00004687: 	LDI	*AR2,AR2
    // asm 00004688: 	LDI	1,RC
    // asm 00004689: 	LDI	100,R2
    // asm 0000468A: 	LDI	250,R3
    // asm 0000468B: 	CALL	_outtextxyc
FFFD:
    // asm 0000468C: 	LDI	3000,R0
    // asm 0000468D: 	CALL	WAITMSEC
    // asm 0000468E: 	FILL	1,250,500,350,0
    // asm 00004694: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NNE1", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*WAIT 500
*
 */
void WAIT500(void)
{
    // ;	RPTS	490
    // asm 0000469A: 	RPTS	100
    // asm 0000469B: 	NOP
    // asm 0000469C: 	PUSH	R0
    // asm 0000469D: 	LDI	@_MODE,R0
    // asm 0000469E: 	AND	MMODE,R0
    // asm 0000469F: 	CMPI	MDIAG,R0
    // asm 000046A0: 	BNE	WT500X
    // asm 000046A1: 	CALL	FEED_WATCHDOG 	;FEED THE DOG WHEN IN DIAGNOSTICS
WT500X:
    // asm 000046A2: 	POP	R0
    // asm 000046A3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT500", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*CHECK FOR MOTION DIPSWITCH
*RET NE IF MOTION DIPSWITCH OFF
*
 */
void CHECK_MOTION_DIP(void)
{
    // asm 000046A4: 	PUSH	AR2
    // asm 000046A5: 	CLRI	AR2
    // asm 000046A6: 	LDP	@DIPSW
    // asm 000046A7: 	LDI	@DIPSW,R0	;READ DIPSWITCH
    // asm 000046A8: 	LDI	*AR2,AR2
    // asm 000046A9: 	POP	AR2
    // asm 000046AA: 	RS	16,R0
    // asm 000046AB: 	SETDP
    // asm 000046AC: 	TSTB	DIP_MOTION,R0	;MOTION CABINET?
    // asm 000046AD: 	RETS
    // *
    // *CHECK IF MOTION PRESENT
    // *RET NE IF MOTION NOT PRESENT
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_MOTION_DIP", 0, 0);
    UNIMPL();
}

void CHECK_MOTION_PRESENT(void)
{
    // asm 000046AE: 	READAUD	ADJ_MOTION_PRESENT
    // asm 000046B0: 	CMPI	1,R0
    // asm 000046B1:  	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_MOTION_PRESENT", 0, 0);
    UNIMPL();
}

void DIAG_MOTION_TESTZ(void)
{
    // asm 000046B2: 	LDI	1,IR1
    // asm 000046B3: 	BU	MOTION_BURNIN_J
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_MOTION_TESTZ", 0, 0);
    UNIMPL();
}

void DIAG_MOTION_TESTX(void)
{
    // asm 000046B4: 	LDI	2,IR1
    // asm 000046B5: 	BU	MOTION_BURNIN_J
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_MOTION_TESTX", 0, 0);
    UNIMPL();
}

void DIAG_MOTION_TESTY(void)
{
    // asm 000046B6: 	LDI	4,IR1
    // asm 000046B7: 	BU	MOTION_BURNIN_J
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_MOTION_TESTY", 0, 0);
    UNIMPL();
}

void DIAG_MOTION_BURNIN(void)
{
    // asm 000046B8: 	LDI	7,IR1
MOTION_BURNIN_J:
    // asm 000046B9: 	CALL	CHECK_MOTION_DIP
    // asm 000046BA: 	BNZ	GALIL_ERR1		;RETURN IF NON MOVING
    // asm 000046BB: 	CALL	CHECK_MOTION_PRESENT
    // asm 000046BC: 	BNE	GALIL_ERROR
    // ;	CALL	INITIALIZE_GALIL
    // asm 000046BD: 	CALL	INITIALIZATION_MOTION_CHECK
    // asm 000046BE: 	CALL	CHECK_MOTION_PRESENT
    // asm 000046BF: 	BNE	GALIL_ERROR
    // asm 000046C0: 	CALL	CLRSCRN
    // asm 000046C1: 	CALL	SETPAGE0
    // asm 000046C2: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm 000046C5: 	STPI	R0,@ACTIVE_SCREEN
    // asm 000046C6: 	CLRI	AR4			;page index
    // asm 000046C7: 	LDI	DEFAULT_COLOR,RC
    // asm 000046C8: 	TEXTIT	bbd,50,330
    // asm 000046CC: 	CLRF	R5
    // asm 000046CD: 	CLRF	R6
    // asm 000046CE: 	CLRF	R7
MBI_LP:
    // asm 000046CF: 	CLRI	AR2
    // asm 000046D0: 	LDP	@991030h
    // asm 000046D1: 	LDI	@991030h,R0
    // asm 000046D2: 	LDI	*AR2,AR2
    // asm 000046D3: 	SETDP
    // asm 000046D4: 	RS	8,R0
    // asm 000046D5: 	LS	24,R0
    // asm 000046D6: 	CMPI	0,R0
    // asm 000046D7: 	BEQ	JAJA
    // asm 000046D8: 	CALL	MOTION_SAFETY_SWITCHES_DIAG
    // asm 000046D9: 	BU	ENTER_DIAG
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_MOTION_BURNIN", 0, 0);
    UNIMPL();
}

void JAJA(void)
{
    // asm 000046DA: 	TSTB	1,IR1
    // asm 000046DB: 	BZ	JJ5
    // asm 000046DC: 	ADDFL	0.03,R5
    // asm 000046DD: 	CMPF	TWOPI,R5
    // asm 000046DE: 	BLT	JJ5
    // asm 000046DF: 	SUBF	TWOPI,R5
JJ5:
    // asm 000046E0: 	TSTB	2,IR1
    // asm 000046E1: 	BZ	JJ6
    // asm 000046E2: 	ADDFL	0.05,R6
    // asm 000046E3: 	CMPF	TWOPI,R6
    // asm 000046E4: 	BLT	JJ6
    // asm 000046E5: 	SUBF	TWOPI,R6
JJ6:
    // asm 000046E6: 	TSTB	4,IR1
    // asm 000046E7: 	BZ	JJ7
    // asm 000046E8: 	ADDFL	0.04,R7
    // asm 000046E9: 	CMPF	TWOPI,R7
    // asm 000046EA: 	BLT	JJ7
    // asm 000046EB: 	SUBF	TWOPI,R7
JJ7:
    // asm 000046EC: 	LDF	R7,R2
    // asm 000046ED: 	PUSHFL	R5
    // asm 000046EF: 	PUSHFL	R6
    // asm 000046F1: 	PUSHFL	R7
    // asm 000046F3: 	CALL	_HPsin
    // asm 000046F4: 	LDF	R0,R4
    // asm 000046F5: 	POPFL	R7
    // asm 000046F7: 	POPFL	R6
    // asm 000046F9: 	POPFL	R5
    // asm 000046FB: 	LDF	R6,R2
    // asm 000046FC: 	PUSHFL	R4
    // asm 000046FE: 	PUSHFL	R5
    // asm 00004700: 	PUSHFL	R6
    // asm 00004702: 	PUSHFL	R7
    // asm 00004704: 	CALL	_HPsin
    // asm 00004705: 	LDF	R0,R3
    // asm 00004706: 	POPFL	R7
    // asm 00004708: 	POPFL	R6
    // asm 0000470A: 	POPFL	R5
    // asm 0000470C: 	POPFL	R4
    // asm 0000470E: 	LDF	R5,R2
    // asm 0000470F: 	PUSHFL	R3
    // asm 00004711: 	PUSHFL	R4
    // asm 00004713: 	PUSHFL	R5
    // asm 00004715: 	PUSHFL	R6
    // asm 00004717: 	PUSHFL	R7
    // asm 00004719: 	CALL	_HPsin
    // asm 0000471A: 	LDF	R0,R2
    // asm 0000471B: 	POPFL	R7
    // asm 0000471D: 	POPFL	R6
    // asm 0000471F: 	POPFL	R5
    // asm 00004721: 	POPFL	R4
    // asm 00004723: 	POPFL	R3
    // asm 00004725: 	PUSHFL	R5
    // asm 00004727: 	PUSHFL	R6
    // asm 00004729: 	PUSHFL	R7
    // 	;R2	front
    // 	;R3	left
    // 	;R4	right
    // 	;
    // 	;-1>=x>=1
    // asm 0000472B: 	MPYF	0.8,R2
    // asm 0000472C: 	MPYF	0.8,R3
    // asm 0000472D: 	MPYF	0.8,R4
    // 	;-.8>=x>=.8
    // asm 0000472E: 	CALL	MOTION_SCALE_ENTER
    // asm 0000472F: 	CALL	WAIT_FOR_VBLANK
    // asm 00004730: 	POPFL	R7
    // asm 00004732: 	POPFL	R6
    // asm 00004734: 	POPFL	R5
    // asm 00004736: 	LDI	@_newbut,R0
    // asm 00004737: 	LDI	R0,R1
    // asm 00004738: 	TSTB	SW_DIAG,R1		;move up
    // asm 00004739: 	BNZ	MBI_LP
    // asm 0000473A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "JAJA", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PLAYER CABINET MOTION
*
*PARAMETERS
*	AR4	PLAYER CAR OBJECT
*	AR5	PLAYER CAR STRUCTURE
*
*CONTROL AXES:
*		X=LFT REAR 0=GROUND, 50000=FULL HEIGHT
*		Y=RT REAR  0=GROUND, 50000=FULL HEIGHT
*		Z=FRONT	   0=GROUND, 50000=FULL HEIGHT
*
 */
void PLMOTION(void)
{
    // asm 0000473B: 	CALL	CHECK_MOTION_DIP
    // asm 0000473C: 	RETSNZ			;RETURN IF NON MOVING
    // asm 0000473D: 	LDI	@MOTION_STOP_HIT,R0
    // asm 0000473E: 	RETSNZ
    // 	;
    // 	;check for an abort
    // 	;
    // asm 0000473F: 	LDI	@ABORT_QUERY,R0
    // asm 00004740: 	DEC	R0
    // asm 00004741: 	STI	R0,@ABORT_QUERY
    // asm 00004742: 	BGT	NOCHECK
    // asm 00004743: 	LDI	30,R0
    // asm 00004744: 	STI	R0,@ABORT_QUERY
    // asm 00004745: 	LDL	ABORTQ,AR2
    // asm 00004746: 	CALL	SEND_CMD
    // asm 00004747: 	CALL	WAIT_ACK
NOCHECK:
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLMOTION", 0, 0);
    UNIMPL();
}

void NO_NEW_MOTION_ERRORS(void)
{
    // asm 00004757: 	SUBI	@NFRAMES,R0
    // asm 00004758: 	LDILT	0,R0
    // asm 00004759: 	STI	R0,@WAITTIK
    // asm 0000475A: 	CMPI	0,R0
    // asm 0000475B: 	RETSGT
    // 	;here is where we want to test to see if we are done with the error
    // 	;and how we are to recover
    // asm 0000475C: 	CALL	ABORT_RESET_GALIL		;recover latch from mark
    // asm 0000475D: 	CLRI	AR2
    // asm 0000475E: 	LDP	@991030h	;IF we have a motion error after
    // asm 0000475F: 	LDI	@991030h,R0	;we attempted to reset, THEN
    // asm 00004760: 	LDI	*AR2,AR2
    // asm 00004761: 	SETDP			;try again next frame
    // asm 00004762: 	LDL	0FF80h,R1	;
    // asm 00004763: 	AND	R1,R0		;
    // asm 00004764: 	RETSNZ			;
    // ;	CALL	MOTION_DLPROG
    // asm 00004765: RESETMOTIONANYWAY
    // asm 00004765: 	LDL	XQ,AR2				;tell galil to continue executing program
    // asm 00004766: 	CALL	SEND_CMD
    // asm 00004767: 	CALL	WAIT_ACK
    // asm 00004768: 	CLRI	R0
    // asm 00004769: 	STI	R0,@MOTION_SAFETY_ON
    // ;	LDI	300,R0
    // ;	STI	R0,@MOTION_RCV_TIKS
    // asm 0000476A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NO_NEW_MOTION_ERRORS", 0, 0);
    UNIMPL();
}

void NO_PREVMOTION_ERRORS(void)
{
    // ;	LDI	@LATCHED_ERROR,R0
    // ;	BZ	NONELAT
    // ;
    // ;	CALL	CLEAR_LATCH_ERROR
    // ;	CALL	ABORT_RESET_GALIL		;recover latch from mark
    // ;	CALL	MOTION_DLPROG
    // ;
    // ;	BU	RESETMOTIONANYWAY
    // ;NONELAT
    // 	;IF galil is NOT responding
    // 	;attempt to reset serious
    // 	;
    // asm 0000476B: 	LDI	@MOTION_RCV_TIKS,R0
    // asm 0000476C: 	BGT	NOTTLRST
    // asm 0000476D: 	LDI	1,R2
    // asm 0000476E: 	SETAUD	AUD_RESET_TOTALLY
    // asm 00004770: 	LDI	0,R2
    // asm 00004771: 	SETAUD	ADJ_MOTION_PRESENT
    // asm 00004773: 	RETS
    // ;
    // ;	CALL    RESET_GALIL
    // ;	CALL    WAIT_ACK_REAL
    // ;
    // ;
    // ;	LDS	"JG -26000,-2,AR2
    // ;	CALL    SEND_CMD
    // ;	CALL    WAIT_ACK
    // ;
    // ;	CALL	CHECK_MOTION_PRESENT
    // ;	BEQ	KKGK
    // ;
    // ;	LDI	1,R2
    // ;	SETAUD	ADJ_MOTION_PRESENT
    // asm 00004774: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NO_PREVMOTION_ERRORS", 0, 0);
    UNIMPL();
}

void KKGK(void)
{
    // asm 00004775: 	LDL     PP1,AR2
    // asm 00004776: 	CALL    SEND_CMD
    // asm 00004777: 	CALL    WAIT_ACK
    // asm 00004778: 	LDL     PP2,AR2
    // asm 00004779: 	CALL    SEND_CMD
    // asm 0000477A: 	CALL    WAIT_ACK
    // asm 0000477B: 	LDL     PP3,AR2
    // asm 0000477C: 	CALL    SEND_CMD
    // asm 0000477D: 	CALL    WAIT_ACK
    // asm 0000477E: 	LDL     PP4,AR2
    // asm 0000477F: 	CALL    SEND_CMD
    // asm 00004780: 	CALL    WAIT_ACK
    // 	;
    // 	;dl the program.
    // 	;
    // asm 00004781: 	CALL	CLEAR_LATCH_ERROR
    // asm 00004782: 	CALL	MOTION_DLPROG
    // asm 00004783: 	LDI	300,R0
    // asm 00004784: 	STI	R0,@MOTION_RCV_TIKS
NOTTLRST:
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KKGK", 0, 0);
    UNIMPL();
}

void NOTTHIS(void)
{
    // asm 000047B5: KDS
NO_MOTION_ERRORS:
    // asm 000047B5: 	LDI	@MOTION_NOT_ON,R0
    // asm 000047B6: 	RETSNZ
    // asm 000047B7: 	CALL	CHECK_MOTION_PRESENT
    // asm 000047B8: 	RETSNE
    // asm 000047B9: 	LDF	*+AR5(RF_PY),R5
    // asm 000047BA: 	ADDF	*+AR5(LF_PY),R5
    // asm 000047BB: 	ADDF	*+AR5(LR_PY),R5
    // asm 000047BC: 	ADDF	*+AR5(RR_PY),R5
    // asm 000047BD: 	MPYF	0.25,R5
    // *FIND AVERAGE Y POSITION = R5
    // asm 000047BE: 	LDF	*+AR5(RF_PY),R2
    // asm 000047BF: 	ADDF	*+AR5(LF_PY),R2
    // asm 000047C0: 	MPYF	0.5,R2
    // asm 000047C1: 	LDF	*+AR5(LR_PY),R3
    // asm 000047C2: 	LDF	*+AR5(RR_PY),R4
    // *GET Y HEIGHT DELTA TO CENTER
    // asm 000047C3: 	SUBRF	R5,R2		;Z (FRONT) AXIS DELTA
    // asm 000047C4: 	SUBRF	R5,R3		;X (LREAR) AXIS DELTA
    // asm 000047C5: 	SUBRF	R5,R4		;Y (RREAR) AXIS DELTA
    // asm 000047C6: 	FLOAT	150,R0		;DIVIDE BY RADIUS FOR PROPORTIONALITY
    // asm 000047C7: 	CALL	INV_F30
    // *COMPUTE EQUATION M=2X-X*X/150
    // asm 000047C8: 	LDF	2,R1
    // asm 000047C9: 	MPYF	R1,R2,R5   	;2*Z
    // asm 000047CA: 	MPYF	R2,R2
    // asm 000047CB: 	MPYF	R0,R2
    // asm 000047CC: 	SUBF	R2,R5,R2
    // asm 000047CD: 	MPYF	R0,R2
    // asm 000047CE: 	MPYF	R1,R3,R5   	;2*X
    // asm 000047CF: 	MPYF	R3,R3
    // asm 000047D0: 	MPYF	R0,R3
    // asm 000047D1: 	SUBF	R3,R5,R3
    // asm 000047D2: 	MPYF	R0,R3
    // asm 000047D3: 	MPYF	R1,R4,R5   	;2*Y
    // asm 000047D4: 	MPYF	R4,R4
    // asm 000047D5: 	MPYF	R0,R4
    // asm 000047D6: 	SUBF	R4,R5,R4
    // asm 000047D7: 	MPYF	R0,R4
    // ;	MPYF	R0,R2
    // ;	MPYF	R0,R3
    // ;	MPYF	R0,R4
    // ;
    // asm 000047D8: 	CMPF	0.9,R2		;RESTRICT RANGE TO -0.9->0.9
    // asm 000047D9: 	LDFGT	0.9,R2
    // asm 000047DA: 	CMPF	-0.9,R2
    // asm 000047DB: 	LDFLT	-0.9,R2
    // asm 000047DC: 	CMPF	0.9,R3		;RESTRICT RANGE TO -0.9->0.9
    // asm 000047DD: 	LDFGT	0.9,R3
    // asm 000047DE: 	CMPF	-0.9,R3
    // asm 000047DF: 	LDFLT	-0.9,R3
    // asm 000047E0: 	CMPF	0.9,R4		;RESTRICT RANGE TO -0.9->0.9
    // asm 000047E1: 	LDFGT	0.9,R4
    // asm 000047E2: 	CMPF	-0.9,R4
    // asm 000047E3: 	LDFLT	-0.9,R4
    // *GET LEAN ANGLES 20% OF MOTION (10% OF MOTION RESERVED)
    // *FRONT-BACK (XLEAN)
    // asm 000047E4: 	LDF	*+AR5(CARXLEAN),R1		;RADIAN LEAN -0.1->0.1
    // asm 000047E5: 	MPYF	6,R1
    // asm 000047E6: 	ADDF	R1,R3
    // asm 000047E7: 	ADDF	R1,R4
    // asm 000047E8: 	SUBF	R1,R2
    // *LEFT-RIGHT (ZLEAN)
    // asm 000047E9: 	LDF	*+AR5(CARZLEAN),R1    	;RADIAN LEAN -0.1->0.1
    // asm 000047EA: 	MPYF	6,R1
    // asm 000047EB: 	ADDF	R1,R4
    // asm 000047EC: 	SUBF	R1,R3
    // *LIMIT MOTION +-1.0
    // asm 000047ED: 	CMPF	1.0,R2
    // asm 000047EE: 	LDFGT	1.0,R2
    // asm 000047EF: 	CMPF	1.0,R3
    // asm 000047F0: 	LDFGT	1.0,R3
    // asm 000047F1: 	CMPF	1.0,R4
    // asm 000047F2: 	LDFGT	1.0,R4
    // asm 000047F3: 	CMPF	-1.0,R2
    // asm 000047F4: 	LDFLT	-1.0,R2
    // asm 000047F5: 	CMPF	-1.0,R3
    // asm 000047F6: 	LDFLT	-1.0,R3
    // asm 000047F7: 	CMPF	-1.0,R4
    // asm 000047F8: 	LDFLT	-1.0,R4
    // *SEND STUFF TO MOTION PLATFORM
    // *R2=Z FRONT		-1.0->1.0
    // *R3=X LFT REAR		-1.0->1.0
    // *R4=Y RT REAR		-1.0->1.0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTTHIS", 0, 0);
    UNIMPL();
}

void MOTION_SCALE_ENTER(void)
{
    // 	;NOW SCALE INTO THE RANGE USEABLE
    // 	;BY OR MOTION SYSTEM
    // 	;
    // asm 000047F9: 	FLOAT	17000,R1
    // asm 000047FA: 	MPYF	R1,R2
    // asm 000047FB: 	MPYF	R1,R3
    // asm 000047FC: 	MPYF	R1,R4
    // asm 000047FD: 	FLOAT	26000,R1
    // asm 000047FE: 	ADDF	R1,R2
    // asm 000047FF: 	ADDF	R1,R3
    // asm 00004800: 	ADDF	R1,R4
    // asm 00004801: 	FIX	R2
    // asm 00004802: 	FIX	R3
    // asm 00004803: 	FIX	R4
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_SCALE_ENTER", 0, 0);
    UNIMPL();
}

void N76(void)
{
    // asm 0000481A: 	POP	R2		;RT REAR (Y)
    // asm 0000481B: 	LDI	@LAST_YR,R1
    // asm 0000481C: 	SUBI	R2,R1
    // asm 0000481D: 	ABSI	R1
    // asm 0000481E: 	CMPI	MIN_MOVE_DIST,R1
    // asm 0000481F: 	BLT	N77
    // asm 00004820: 	STI	R0,@LAST_YR
    // asm 00004821: 	LDL	sPOTVALUE,AR2
    // asm 00004822: 	CALL	_itoa
    // asm 00004823: 	LDL	PYC1,AR0
    // asm 00004824: 	LDL	MOTION_CMDSTR,AR1
    // asm 00004825: 	CALL	STRCPY
    // asm 00004826: 	LDL	MOTION_CMDSTR,AR0
    // asm 00004827: 	LDL	sPOTVALUE,AR1
    // asm 00004828: 	CALL	STRCAT
    // asm 00004829: 	LDL	MOTION_CMDSTR,AR0
    // asm 0000482A: 	LDL	PYC2,AR1
    // asm 0000482B: 	CALL	STRCAT
    // asm 0000482C: 	LDI	AR0,AR2
    // asm 0000482D:         CALL    SEND_CMD
    // asm 0000482E:         CALL    WAIT_ACK
N77:
    // asm 0000482F: 	POP	R2		;LT REAR (X)
    // asm 00004830: 	LDI	@LAST_XL,R1
    // asm 00004831: 	SUBI	R2,R1
    // asm 00004832: 	ABSI	R1
    // asm 00004833: 	CMPI	MIN_MOVE_DIST,R1
    // asm 00004834: 	BLT	N78
    // asm 00004835: 	STI	R0,@LAST_XL
    // asm 00004836: 	LDL	sPOTVALUE,AR2
    // asm 00004837: 	CALL	_itoa
    // asm 00004838: 	LDL	PXC1,AR0
    // asm 00004839: 	LDL	MOTION_CMDSTR,AR1
    // asm 0000483A: 	CALL	STRCPY
    // asm 0000483B: 	LDL	MOTION_CMDSTR,AR0
    // asm 0000483C: 	LDL	sPOTVALUE,AR1
    // asm 0000483D: 	CALL	STRCAT
    // asm 0000483E: 	LDL	MOTION_CMDSTR,AR0
    // asm 0000483F: 	LDL	PXC2,AR1
    // asm 00004840: 	CALL	STRCAT
    // asm 00004841: 	LDI	AR0,AR2
    // asm 00004842:         CALL    SEND_CMD
    // asm 00004843:         CALL    WAIT_ACK
N78:
    // asm 00004844: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "N76", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void LEVEL_THE_MOTION(void)
{
    // asm 00004845: 	CALL	CHECK_MOTION_DIP
    // asm 00004846: 	RETSNZ			;RETURN IF NON MOVING
    // asm 00004847: 	CALL	CHECK_MOTION_PRESENT
    // asm 00004848: 	RETSNE
    // ;	LDF	-1.26,R2
    // ;	LDF	-1.26,R3
    // ;	LDF	-1.26,R4
    // ;	BU	MOTION_SCALE_ENTER
    // asm 00004849: 	LDI	0,R2
    // asm 0000484A: 	STI	R2,@LAST_ZF
    // asm 0000484B: 	LDL	sPOTVALUE,AR2
    // asm 0000484C: 	CALL	_itoa
    // asm 0000484D: 	LDL	PZC1,AR0
    // asm 0000484E: 	LDL	MOTION_CMDSTR,AR1
    // asm 0000484F: 	CALL	STRCPY
    // asm 00004850: 	LDL	MOTION_CMDSTR,AR0
    // asm 00004851: 	LDL	sPOTVALUE,AR1
    // asm 00004852: 	CALL	STRCAT
    // asm 00004853: 	LDL	MOTION_CMDSTR,AR0
    // asm 00004854: 	LDL	PZC2,AR1
    // asm 00004855: 	CALL	STRCAT
    // asm 00004856: 	LDI	AR0,AR2
    // asm 00004857:         CALL    SEND_CMD
    // asm 00004858:         CALL    WAIT_ACK
    // asm 00004859: 	LDI	0,R2		;RT REAR (Y)
    // asm 0000485A: 	LDL	sPOTVALUE,AR2
    // asm 0000485B: 	CALL	_itoa
    // asm 0000485C: 	LDL	PYC1,AR0
    // asm 0000485D: 	LDL	MOTION_CMDSTR,AR1
    // asm 0000485E: 	CALL	STRCPY
    // asm 0000485F: 	LDL	MOTION_CMDSTR,AR0
    // asm 00004860: 	LDL	sPOTVALUE,AR1
    // asm 00004861: 	CALL	STRCAT
    // asm 00004862: 	LDL	MOTION_CMDSTR,AR0
    // asm 00004863: 	LDL	PYC2,AR1
    // asm 00004864: 	CALL	STRCAT
    // asm 00004865: 	LDI	AR0,AR2
    // asm 00004866:         CALL    SEND_CMD
    // asm 00004867:         CALL    WAIT_ACK
    // asm 00004868: 	CLRI	R2		;LT REAR (X)
    // asm 00004869: 	LDL	sPOTVALUE,AR2
    // asm 0000486A: 	CALL	_itoa
    // asm 0000486B: 	LDL	PXC1,AR0
    // asm 0000486C: 	LDL	MOTION_CMDSTR,AR1
    // asm 0000486D: 	CALL	STRCPY
    // asm 0000486E: 	LDL	MOTION_CMDSTR,AR0
    // asm 0000486F: 	LDL	sPOTVALUE,AR1
    // asm 00004870: 	CALL	STRCAT
    // asm 00004871: 	LDL	MOTION_CMDSTR,AR0
    // asm 00004872: 	LDL	PXC2,AR1
    // asm 00004873: 	CALL	STRCAT
    // asm 00004874: 	LDI	AR0,AR2
    // asm 00004875:         CALL    SEND_CMD
    // asm 00004876:         CALL    WAIT_ACK
    // asm 00004877: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEVEL_THE_MOTION", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void DOWNLOAD_PROGRAM(void)
{
IBO654:
    // asm 000048D5: 	LDI	*AR4++,AR2
    // asm 000048D6: 	CMPI	0,AR2
    // asm 000048D7: 	BEQ	DNF
    // asm 000048D8: 	CALL	SEND_CMD
    // asm 000048D9: 	CALL	WAIT_ACK	;#
    // asm 000048DA: 	CALL	WAIT_FOR_VBLANK
    // asm 000048DB: 	BU	IBO654
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOWNLOAD_PROGRAM", 0, 0);
    UNIMPL();
}

void DNF(void)
{
    // asm 000048DC: 	LDL	ENDED,AR2
    // asm 000048DD: 	CALL	SEND_CMD
    // asm 000048DE: 	CALL	WAIT_ACK	;:
    // asm 000048DF: 	LDL	XQ,AR2
    // asm 000048E0: 	CALL	SEND_CMD
    // asm 000048E1: 	CALL	WAIT_ACK
    // ;
    // ;eric, is this correct? - ml
    // ;
    // ;	LDL	T10,AR2		;;set actuators
    // ;	CALL	SEND_CMD
    // ;	CALL	WAIT_ACK
    // ;
    // ;	LDL	T11,AR2
    // ;	CALL	SEND_CMD
    // ;	CALL	WAIT_ACK
    // ;
    // ;	LDL	T12,AR2
    // ;	CALL	SEND_CMD
    // ;	CALL	WAIT_ACK
    // asm 000048E2: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DNF", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*SEND COMMAND TO GALIL MOTION CONTROL BOARD
*
*
*FF09
*F709
*FF09
*
*FF00
*FB00
*FF00
*
*
*PARAMETERS
*	AR2	PTR TO STR
*
*
 */
void SEND_CMD(void)
{
    // asm 000048E3: 	PUSH	R0
    // asm 000048E4: 	PUSH	R1
    // asm 000048E5: 	PUSH	R2
    // asm 000048E6: 	PUSH	R3
    // asm 000048E7: 	PUSH	R4
    // asm 000048E8: 	PUSH	R5
    // asm 000048E9: 	PUSH	R6
    // asm 000048EA: 	PUSH	R7
    // asm 000048EB: 	DINT
    // asm 000048F1: 	LDIL	SOUND,AR3
    // asm 000048F4: 	LDI	0FF09h,R0
    // asm 000048F5: 	LDI	0F709h,R1
    // asm 000048F6: 	STI	R0,*AR3
    // asm 000048F7: 	CALL	WAIT500
    // asm 000048F8: 	STI	R1,*AR3
    // asm 000048F9: 	CALL	WAIT500
    // asm 000048FA: 	STI	R0,*AR3
    // asm 000048FB: 	CALL	WAIT500
    // asm 000048FC: 	LDI	0FF00h,R2
    // asm 000048FD: 	LDI	0FB00h,R3
    // asm 000048FE: 	CLRI	R5		;SHIFT COUNT
LLKK:
    // asm 000048FF: CMPI	-32,R5
    // asm 00004900: 	BNE	REGLP
    // asm 00004901: 	CLRI	R5
    // asm 00004902: 	NOP	*AR2++
REGLP:
    // asm 00004903: 	LDI	*AR2,R4
    // asm 00004904: 	LSH	R5,R4
    // asm 00004905: 	SUBI	8,R5
    // asm 00004906: 	AND	0FFh,R4
    // asm 00004907: 	CMPI	0,R4
    // asm 00004908: 	BZ	KKKII
    // asm 00004909: 	OR	R4,R2,R6
    // asm 0000490A: 	OR	R4,R3,R7
    // asm 0000490B: 	STI	R6,*AR3
    // asm 0000490C: 	CALL	WAIT500
    // asm 0000490D: 	STI	R7,*AR3
    // asm 0000490E: 	CALL	WAIT500
    // asm 0000490F: 	STI	R6,*AR3
    // asm 00004910: 	CALL	WAIT500
    // asm 00004911: 	BU	LLKK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_CMD", 0, 0);
    UNIMPL();
}

void KKKII(void)
{
    // asm 00004912: 	EINT
    // asm 00004913: 	POP	R7
    // asm 00004914: 	POP	R6
    // asm 00004915: 	POP	R5
    // asm 00004916: 	POP	R4
    // asm 00004917: 	POP	R3
    // asm 00004918: 	POP	R2
    // asm 00004919: 	POP	R1
    // asm 0000491A: 	POP	R0
    // asm 0000491B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "KKKII", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RESET_GALIL(void)
{
    // asm 0000491C: 	DINT
    // asm 00004922:         LDIL    SOUND,AR3
    // asm 00004925:         LDI     0FF0Ch,R0
    // asm 00004926:         LDI     0F70Ch,R1
    // asm 00004927:         STI     R0,*AR3
    // asm 00004928:         CALL	WAIT500
    // asm 00004929:         STI     R1,*AR3
    // asm 0000492A:         CALL	WAIT500
    // asm 0000492B:         STI     R0,*AR3
    // asm 0000492C:         CALL	WAIT500
    // asm 0000492D:         LDI     0FF00h,R0
    // asm 0000492E:         LDI     0FB00h,R1
    // asm 0000492F:         STI     R0,*AR3
    // asm 00004930:         CALL	WAIT500
    // asm 00004931:         STI     R1,*AR3
    // asm 00004932:         CALL	WAIT500
    // asm 00004933:         STI     R0,*AR3
    // asm 00004934:         CALL	WAIT500
    // asm 00004935:         LDI     0FF0bh,R0
    // asm 00004936:         LDI     0F70bh,R1
    // asm 00004937:         STI     R0,*AR3
    // asm 00004938:         CALL	WAIT500
    // asm 00004939:         STI     R1,*AR3
    // asm 0000493A:         CALL	WAIT500
    // asm 0000493B:         STI     R0,*AR3
    // asm 0000493C:         CALL	WAIT500
    // asm 0000493D:         LDI     0FF00h,R0
    // asm 0000493E:         LDI     0FB00h,R1
    // asm 0000493F:         STI     R0,*AR3
    // asm 00004940:         CALL	WAIT500
    // asm 00004941:         STI     R1,*AR3
    // asm 00004942:         CALL	WAIT500
    // asm 00004943:         STI     R0,*AR3
    // asm 00004944:         CALL	WAIT500
    // asm 00004945: 	EINT
    // asm 00004946:         RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_GALIL", 0, 0);
    UNIMPL();
}

void ABORT_RESET_GALIL(void)
{
    // asm 00004947: 	DINT
    // asm 0000494D:         LDIL    SOUND,AR3
    // 	;
    // 	;in essence we are clearing the abort, but NOT reseting galil
    // 	;
    // asm 00004950:         LDI     0FF0eh,R0	;  0Ch = reset bit addr
    // asm 00004951:         LDI     0F70eh,R1	;  0Eh = abort/reset bit addr
    // asm 00004952:         STI     R0,*AR3
    // asm 00004953:         CALL	WAIT500
    // asm 00004954:         STI     R1,*AR3
    // asm 00004955:         CALL	WAIT500
    // asm 00004956:         STI     R0,*AR3
    // asm 00004957:         CALL	WAIT500
    // asm 00004958:         LDI     0FF00h,R0
    // asm 00004959:         LDI     0FB00h,R1
    // asm 0000495A:         STI     R0,*AR3
    // asm 0000495B:         CALL	WAIT500
    // asm 0000495C:         STI     R1,*AR3
    // asm 0000495D:         CALL	WAIT500
    // asm 0000495E:         STI     R0,*AR3
    // asm 0000495F:         CALL	WAIT500
    // asm 00004960: 	EINT
    // asm 00004961: 	LDL	XQ,AR2				;tell galil to continue executing program
    // asm 00004962: 	CALL	SEND_CMD
    // asm 00004963: 	CALL	WAIT_ACK
    // asm 00004964:         RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ABORT_RESET_GALIL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MOTION_DLPROG(void)
{
    // asm 00004965: 	CALL	CHECK_MOTION_DIP
    // asm 00004966: 	RETSNZ			;RETURN IF NON MOVING
    // asm 00004967: 	CALL	CHECK_MOTION_PRESENT
    // asm 00004968: 	RETSNE
    // asm 00004969: 	LDL	THEPROGL,AR4
    // asm 0000496A: 	CALL	DOWNLOAD_PROGRAM
    // asm 0000496B: 	CLRI	R0
    // asm 0000496C: 	STI	R0,@LAST_ZF
    // asm 0000496D: 	STI	R0,@LAST_XL
    // asm 0000496E: 	STI	R0,@LAST_YR
    // asm 0000496F: 	CLRI	R0
    // asm 00004970: 	STI	R0,@MOTION_NOT_ON
    // asm 00004971: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_DLPROG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*MOTION DIPSWITCH OFF
 */
void GALIL_ERR1(void)
{
    // asm 00004972: 	CALL	SETPAGE0
    // asm 00004973: 	LDIL	SCREEN0,R0
    // asm 00004976: 	STI	R0,@ACTIVE_SCREEN
    // asm 00004977: 	LDI	2,AR5
    // asm 00004978: GDLP
    // asm 00004978: 	LDI	0,RC
    // asm 00004979: 	TEXTITT	"WARNING MOTION DIP SWITCH OFF",150,130
    // asm 0000497D: 	LDI	250,R0
    // asm 0000497E: 	CALL	WAITMSEC
    // asm 0000497F: 	LDI	COL_WHITE,RC
    // asm 00004980: 	TEXTITT	"WARNING MOTION DIP SWITCH OFF",150,130
    // asm 00004984: 	LDI	984,AR2
    // asm 00004985: 	CALL	SENDSND
    // asm 00004986: 	LDI	250,R0
    // asm 00004987: 	CALL	WAITMSEC
    // asm 00004988: 	DBU	AR5,GDLP
    // asm 00004989: 	RETS
    // *MOTION NO DETECTED
GALIL_ERROR:
    // ;	.globl	MOTION_SAFETY_SWITCHES_DIAG
    // ;	CALL	MOTION_SAFETY_SWITCHES_DIAG
    // ;	BU	ENTER_DIAG
    // ;	CALL	CLRSCRN
    // asm 0000498A: 	CALL	SETPAGE0
    // asm 0000498B: 	LDIL	SCREEN0,R0
    // asm 0000498E: 	STI	R0,@ACTIVE_SCREEN
    // asm 0000498F: 	LDI	2,AR5
    // asm 00004990: GLP
    // asm 00004990: 	LDI	0,RC
    // asm 00004991: 	TEXTITT	"WARNING MOTION NOT DETECTED",150,130
    // asm 00004995: 	LDI	250,R0
    // asm 00004996: 	CALL	WAITMSEC
    // asm 00004997: 	LDI	COL_WHITE,RC
    // asm 00004998: 	TEXTITT	"WARNING MOTION NOT DETECTED",150,130
    // asm 0000499C: 	LDI	984,AR2
    // asm 0000499D: 	CALL	SENDSND
    // asm 0000499E: 	LDI	250,R0
    // asm 0000499F: 	CALL	WAITMSEC
    // asm 000049A0: 	DBU	AR5,GLP
    // asm 000049A1: 	RETS
    // *
    // *WAIT R0 X MSEC
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GALIL_ERR1", 0, 0);
    UNIMPL();
}

void WAITMSEC(void)
{
    // asm 000049A2: 	MPYI	5000,R0
WTMLP:
    // asm 000049A3: 	SUBI	1,R0
    // asm 000049A4: 	BGE	WTMLP
    // asm 000049A5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAITMSEC", 0, 0);
    UNIMPL();
}

void CHECK_MOTION_ERROR(void)
{
    // asm 000049A7: 	PUSH	R0
    // asm 000049A8: 	PUSH	AR0
    // asm 000049A9: 	CLRI	AR0
    // asm 000049AA: 	LDP	@991030h
    // asm 000049AB: 	LDI	@991030h,R0
    // asm 000049AC: 	LDI	*AR0,AR0
    // asm 000049AD: 	SETDP
    // asm 000049AE: 	AND	@CME_MASK,R0
    // asm 000049AF: 	BZ	CME_NO_MOTION_ERRORS
    // asm 000049B0: 	SETC
    // asm 000049B1: 	POP	AR0
    // asm 000049B2: 	POP	R0
    // asm 000049B3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_MOTION_ERROR", 0, 0);
    UNIMPL();
}

void CME_NO_MOTION_ERRORS(void)
{
    // asm 000049B4: 	CLRC
    // asm 000049B5: 	POP	AR0
    // asm 000049B6: 	POP	R0
    // asm 000049B7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CME_NO_MOTION_ERRORS", 0, 0);
    UNIMPL();
}

void CLEAR_LATCH_ERROR(void)
{
    // asm 000049B8: 	DINT
    // asm 000049BE: 	CLRI	R0
    // asm 000049BF: 	STI	R0,@LATCHED_ERROR
    // asm 000049C0: 	EINT
    // asm 000049C1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_LATCH_ERROR", 0, 0);
    UNIMPL();
}

void LATCH_ERROR(void)
{
    // asm 000049C2: 	CALL	CHECK_MOTION_DIP
    // asm 000049C3: 	RETSNZ			;RETURN IF NON MOVING
    // asm 000049C4: 	CALL	CHECK_MOTION_ERROR
    // asm 000049C5: 	RETSNC
    // asm 000049C6: 	LDI	1,R0
    // asm 000049C7: 	STI	R0,@LATCHED_ERROR
    // asm 000049C8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LATCH_ERROR", 0, 0);
    UNIMPL();
}

/* *
*CHECK GALIL READY
*RET CS IF GALIL READY W/ CHAR
*
 */
void G_READY(void)
{
    // asm 000049C9:         LDIL    SOUND,AR3
    // asm 000049CC:         LDI     0FF0Ah,R0
    // asm 000049CD:         LDI     0F70ah,R1
    // asm 000049CE:         STI     R0,*AR3
    // asm 000049CF:         CALL 	WAIT500
    // asm 000049D0:         STI     R1,*AR3
    // asm 000049D1:         CALL 	WAIT500
    // asm 000049D2:         STI     R0,*AR3
    // asm 000049D3:         CALL	WAIT500
    // asm 000049D4:         LDI     0FF00h,R0
    // asm 000049D5:         LDI     0Fb00h,R1
    // asm 000049D6:         STI     R0,*AR3
    // asm 000049D7:         CALL 	WAIT500
    // asm 000049D8:         STI     R1,*AR3
    // asm 000049D9:         CALL 	WAIT500
    // asm 000049DA:         LDI     *AR3,R1		;read status
    // asm 000049DB:         STI     R0,*AR3
    // asm 000049DC:         CALL 	WAIT500
    // asm 000049DD: 	LSH	17,R1		;PUT INTO CARRY
    // asm 000049DE: 	RETS
    // *
    // *GET GALIL CHARACTER
    // *RET R1= 8 BIT ASCII CODE B0-B7
    // *RET CS IF CHAR PRESENT
    // *RET CC IF NO CHAR PRESENT
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "G_READY", 0, 0);
    UNIMPL();
}

void G_CHAR(void)
{
    // asm 000049DF: 	CALL	G_READY
    // asm 000049E0: 	RETSNC			;RETURN IF NOTHING
    // asm 000049E1: 	CALL	G_READY
    // asm 000049E2: 	RETSNC			;RETURN IF NOTHING
    // asm 000049E3:         LDIL    SOUND,AR3
    // asm 000049E6:         LDI     0FF08h,R0
    // asm 000049E7:         LDI     0F708h,R1
    // asm 000049E8:         STI     R0,*AR3
    // asm 000049E9:         CALL 	WAIT500
    // asm 000049EA:         STI     R1,*AR3
    // asm 000049EB:         CALL 	WAIT500
    // asm 000049EC:         STI     R0,*AR3
    // asm 000049ED:         CALL 	WAIT500
    // asm 000049EE:         LDI     0FF00h,R0
    // asm 000049EF:         LDI     0Fb00h,R1
    // asm 000049F0:         STI     R0,*AR3
    // asm 000049F1:         CALL 	WAIT500
    // asm 000049F2:         STI     R1,*AR3
    // asm 000049F3:         CALL 	WAIT500
    // asm 000049F4:         LDI     *AR3,R1
    // asm 000049F5:         RS      8,R1
    // asm 000049F6:         AND     0FFh,R1
    // asm 000049F7:         STI     R0,*AR3
    // asm 000049F8:         CALL 	WAIT500
    // asm 000049F9: 	SETC
    // asm 000049FA: 	RETS
    // *
    // *GET GALIL STRING
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "G_CHAR", 0, 0);
    UNIMPL();
}

void G_STRING(void)
{
GS0:
    // asm 000049FB: 	CALL	G_CHAR	   		;CHARACTER READY?
    // asm 000049FC: 	BNC	GSX			;NO...
    // asm 000049FD: 	CMPI	'G',R1	;position error
    // asm 000049FE: 	BNE	NOTPERR	;attempt motion recalibration
    // asm 000049FF: 	LDI	1,R2
    // asm 00004A00: 	SETAUD	AUD_RESET_TOTALLY
    // asm 00004A02: 	LDI	0,R2
    // asm 00004A03: 	SETAUD	ADJ_MOTION_PRESENT
    // asm 00004A05: 	BU	GS0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "G_STRING", 0, 0);
    UNIMPL();
}

void NOTPERR(void)
{
    // asm 00004A06: 	CMPI	'V',R1			;STATUS ANYONE?
    // asm 00004A07: 	BNE	NOTQSTATUS
    // asm 00004A08: 	CALL	G_HEX
    // asm 00004A09: 	AND	6h,R1
    // asm 00004A0A: 	CMPI	6h,R1
    // asm 00004A0B: 	BZ	GS0
    // asm 00004A0C: 	LDI	@WAITTIK,R1
    // asm 00004A0D: 	CMPI	2,R1
    // asm 00004A0E: 	LDILT	2,R1
    // asm 00004A0F: 	STI	R1,@WAITTIK
    // asm 00004A10: 	BU	GS0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTPERR", 0, 0);
    UNIMPL();
}

void NOTQSTATUS(void)
{
    // asm 00004A11: 	CMPI	'W',R1			;IS IT AN ERROR??
    // asm 00004A12: 	BNE	NOTQQERRORX		;
    // asm 00004A13: 	LDI	@LLG6X,AR2
    // asm 00004A14: 	BU	SHOWENCODER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTQSTATUS", 0, 0);
    UNIMPL();
}

void NOTQQERRORX(void)
{
    // asm 00004A15: 	CMPI	'S',R1			;IS IT AN ERROR??
    // asm 00004A16: 	BNE	NOTQQERRORY		;
    // asm 00004A17: 	LDI	@LLG6Y,AR2
    // asm 00004A18: 	BU	SHOWENCODER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTQQERRORX", 0, 0);
    UNIMPL();
}

void NOTQQERRORY(void)
{
    // asm 00004A19: 	CMPI	'Q',R1			;IS IT AN ERROR??
    // asm 00004A1A: 	BNE	NOTQQERRORZ		;
    // asm 00004A1B: 	LDI	@LLG6Z,AR2
    // asm 00004A1C: 	BU	SHOWENCODER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTQQERRORY", 0, 0);
    UNIMPL();
}

void SHOWENCODER(void)
{
    // asm 00004A1D: 	FLOAT	256,R2
    // asm 00004A1E: 	FLOAT	110,R3
    // asm 00004A1F: 	LDI	60,RC
    // asm 00004A20: 	CALL	TEXT_ADDDS
    // asm 00004A21: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    // asm 00004A24: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    // asm 00004A27: 	CALL	SET40FONTDS
    // asm 00004A28: 	CLRI	R2
    // asm 00004A29: 	SETAUD	ADJ_MOTION_PRESENT
    // ;
    // ; galil program will take care of this - ml
    // ;
    // ;	;send shutdown commands
    // ;	LDL	MOTOROFF,AR2
    // ;	CALL	SEND_CMD
    // ;	RETS
    // asm 00004A2B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SHOWENCODER", 0, 0);
    UNIMPL();
}

void NOTQQERRORZ(void)
{
    // asm 00004A2C: NOTQQERROR
    // asm 00004A2C: 	CMPI	'X',R1
    // asm 00004A2D: 	BNE	GS1
    // asm 00004A2E: 	CALL	G_HEX
    // asm 00004A2F: 	BNC	GSX
    // asm 00004A30: 	STI	R1,@GALIL_STATUS_X
    // asm 00004A31: 	B	GS0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTQQERRORZ", 0, 0);
    UNIMPL();
}

void GS1(void)
{
    // asm 00004A32: 	CMPI	'Y',R1
    // asm 00004A33: 	BNE	GS2
    // asm 00004A34: 	CALL	G_HEX
    // asm 00004A35: 	BNC	GSX
    // asm 00004A36: 	STI	R1,@GALIL_STATUS_Y
    // asm 00004A37: 	B	GS0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GS1", 0, 0);
    UNIMPL();
}

void GS2(void)
{
    // asm 00004A38: 	CMPI	'Z',R1
    // asm 00004A39: 	BNE	GS0
    // asm 00004A3A: 	CALL	G_HEX
    // asm 00004A3B: 	BNC	GSX
    // asm 00004A3C: 	STI	R1,@GALIL_STATUS_Z
    // asm 00004A3D: 	B	GS0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GS2", 0, 0);
    UNIMPL();
}

void GSX(void)
{
    // asm 00004A3E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GSX", 0, 0);
    UNIMPL();
}

void GHX(void)
{
    // asm 00004A50: 	CLRC
    // asm 00004A51: 	RETS
    // *
    // *CHANGE ASCIIHEX TO HEX 4 BITS
    // *R1=ASCII
    // *RET R1 =HEX
    // *CC IF INVALID
    // *CS IF VALID
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GHX", 0, 0);
    UNIMPL();
}

void ASCIIHEX(void)
{
    // asm 00004A52: 	CMPI	47H,R1
    // asm 00004A53: 	BGE	ASCERR
    // asm 00004A54: 	SUBI	30H,R1
    // asm 00004A55: 	BN	ASCERR
    // asm 00004A56: 	CMPI	9,R1
    // asm 00004A57: 	BLE	AH1
    // asm 00004A58: 	CMPI	11H,R1
    // asm 00004A59: 	BLT	ASCERR
    // asm 00004A5A: 	SUBI	7,R1
AH1:
    // asm 00004A5B: 	SETC
    // asm 00004A5C: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASCIIHEX", 0, 0);
    UNIMPL();
}

void ASCERR(void)
{
    // asm 00004A5D: 	CLRC
    // asm 00004A5E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASCERR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void WAIT_ACK(void)
{
    // asm 00004A5F: 	DINT
    // asm 00004A65: 	LDI	1000,R6		;EXCESS CHARACTER TIMEOUT
WAIT_ACK_LOOP:
    // asm 00004A66: 	CALL	G_READY
    // asm 00004A67: 	BNC	EXITL2 		;NOTHING THERE DUDES...
    // asm 00004A68: 	LDI	300,R0
    // asm 00004A69: 	STI	R0,@MOTION_RCV_TIKS
    // asm 00004A6A: 	CALL	G_STRING
    // asm 00004A6B: 	BNC	EXITL2 		;NOTHING THERE DUDES...
    // asm 00004A6C: 	DEC	R6
    // asm 00004A6D: 	BZ	NO_RESPONSE	;TOO MANY CHARACTERS
    // asm 00004A6E:         BU      WAIT_ACK_LOOP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_ACK", 0, 0);
    UNIMPL();
}

void EXITL2(void)
{
    // asm 00004A6F: 	EINT
    // asm 00004A70:         RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EXITL2", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void NO_RESPONSE(void)
{
    // asm 00004A71: 	EINT
    // asm 00004A72: 	LDI	0,R2
    // asm 00004A73: 	SETAUD	ADJ_MOTION_PRESENT
    // asm 00004A75: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NO_RESPONSE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void WAIT_ACK_REAL(void)
{
    // asm 00004A76: 	LDI	10000,R6	;NO RESPONSE TIMEOUT
WAR0:
    // asm 00004A77: 	CALL	G_READY
    // asm 00004A78: 	BC	WAR1
    // asm 00004A79: 	DEC	R6
    // asm 00004A7A: 	BZ	NO_RESPONSE
    // asm 00004A7B: 	B	WAR0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_ACK_REAL", 0, 0);
    UNIMPL();
}

void WAR1(void)
{
    // asm 00004A7C: 	LDI	1000,R6		;EXCESS CHARACTER TIMEOUT
WAR1L:
    // asm 00004A7D: 	CALL	G_CHAR
    // asm 00004A7E: 	BNC	EXITL2A		;NO MORE CHARACTERS
    // asm 00004A7F: 	DEC	R6
    // asm 00004A80: 	BZ	NO_RESPONSE	;TOO MANY CHARACTERS
    // asm 00004A81:         BU      WAR1L
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAR1", 0, 0);
    UNIMPL();
}

void EXITL2A(void)
{
    // asm 00004A82:         RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EXITL2A", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
 */
void UPPER_LIMIT_ERROR(void)
{
    // ;
    // ;	CALL	RESET_GALIL
    // ;       CALL    WAIT_ACK_REAL
    // ;
    // ;	;send shutdown commands
    // ;	LDL	MOTOROFF,AR2
    // ;	CALL	SEND_CMD
    // ;
    // asm 00004A83: 	CALL    SEND_MOTOR_OFF_NO_RESET
    // asm 00004A84: 	CLRI	R2
    // asm 00004A85: 	SETAUD	ADJ_MOTION_PRESENT
    // asm 00004A87: 	LDI	5,AR5
    // asm 00004A88: ULLERR
    // asm 00004A88: 	FILL	1,200,500,390,0
    // asm 00004A8E: 	LDI	50,R0
    // asm 00004A8F: 	CALL	WAITMSEC
    // asm 00004A90: 	LDI	1,RC
    // asm 00004A91: 	TEXTITT	"UPPER LIMIT SWITCH ERROR",1,200
    // asm 00004A95: 	LDI	@GALIL_STATUS_X,R0
    // asm 00004A96: 	TSTB	08h,R0
    // asm 00004A97: 	BZ	ULL1
    // asm 00004A98: 	TEXTITT	"X AXIS LIMIT SWITCH NOT FOUND",1,210
ULL1:
    // asm 00004A9C: LDI	@GALIL_STATUS_Y,R0
    // asm 00004A9D: 	TSTB	08h,R0
    // asm 00004A9E: 	BZ	ULL2
    // asm 00004A9F: 	TEXTITT	"Y AXIS LIMIT SWITCH NOT FOUND",1,220
ULL2:
    // asm 00004AA3: LDI	@GALIL_STATUS_Z,R0
    // asm 00004AA4: 	TSTB	08h,R0
    // asm 00004AA5: 	BZ	ULL3
    // asm 00004AA6: 	TEXTITT	"Z AXIS LIMIT SWITCH NOT FOUND",1,230
ULL3:
    // asm 00004AAA: 	LDI	450,R0
    // asm 00004AAB: 	CALL	WAITMSEC
    // asm 00004AAC: 	DBU	AR5,ULLERR
    // asm 00004AAD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UPPER_LIMIT_ERROR", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void LOWER_LIMIT_ERROR(void)
{
    // ;
    // ;	CALL	RESET_GALIL
    // ;        CALL    WAIT_ACK_REAL
    // ;
    // ;	;send shutdown commands
    // ;	LDL	MOTOROFF,AR2
    // ;	CALL	SEND_CMD
    // ;
    // asm 00004AAE: 	CALL    SEND_MOTOR_OFF_NO_RESET
    // asm 00004AAF: 	CLRI	R2
    // asm 00004AB0: 	SETAUD	ADJ_MOTION_PRESENT
    // asm 00004AB2: 	FILL	1,200,500,390,0
    // asm 00004AB8: 	LDI	5,AR5
    // asm 00004AB9: LLLERR
    // asm 00004AB9: 	FILL	1,200,500,390,0
    // asm 00004ABF: 	LDI	50,R0
    // asm 00004AC0: 	CALL	WAITMSEC
    // asm 00004AC1: 	LDI	1,RC
    // asm 00004AC2: 	TEXTITT	"LOWER LIMIT SWITCH ERROR",1,200
    // asm 00004AC6: 	LDI	@GALIL_STATUS_X,R0
    // asm 00004AC7: 	TSTB	04h,R0
    // asm 00004AC8: 	BZ	LLL1
    // asm 00004AC9: 	TEXTITT	"X AXIS LIMIT SWITCH NOT FOUND",1,210
LLL1:
    // asm 00004ACD: LDI	@GALIL_STATUS_Y,R0
    // asm 00004ACE: 	TSTB	04h,R0
    // asm 00004ACF: 	BZ	LLL2
    // asm 00004AD0: 	TEXTITT	"Y AXIS LIMIT SWITCH NOT FOUND",1,220
LLL2:
    // asm 00004AD4: LDI	@GALIL_STATUS_Z,R0
    // asm 00004AD5: 	TSTB	04h,R0
    // asm 00004AD6: 	BZ	LLL3
    // asm 00004AD7: 	TEXTITT	"Z AXIS LIMIT SWITCH NOT FOUND",1,230
LLL3:
    // asm 00004ADB: 	LDI	450,R0
    // asm 00004ADC: 	CALL	WAITMSEC
    // asm 00004ADD: 	DBU	AR5,LLLERR
    // asm 00004ADE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOWER_LIMIT_ERROR", 0, 0);
    UNIMPL();
}

#include "../../core/cpu.h"
#include "../../core/machine.h"
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
#include "../include/diag.h"
#include "../include/motion.h"

/*
 * Source module: asm/MOTION.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY	TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *
// *
// *Misc Notes:
// *
// *	The Galil board is unreliable under conditions of abort, and can
// *	become confused.  To illustrate this point D/L a program, send a
// *	HW Abort, then send an XQ command, and send a HW Abort.  We have been
// *	able to repeatedly shut the Galil board down, so that it will not
// *	respond (under normal AB circumstances even under abort, Galil will
// *	still return a prompt).
// *
// *	We have to place the following redundancies into our code to assure
// *	safe handling of the motion control:
// *
// *	1)	within the program the TL (torque limit) must be tested to
// *		insure no runaway motors (test incombination of movement)
// *	2)	check HW value for safety controls
// *	3)	every thirty frames the Galil board is polled for abort status
// *	4)	every sixty frames (when motion is OK) we must make sure
// *		that we are receiving characters
// *
// *
// *
#define MIN_MOVE_DIST 200
/* asm: MOTION_NOT_ON	.bss	MOTION_NOT_ON,1 */
int MOTION_NOT_ON;
/* asm: MOTION_STOP_HIT	.bss	MOTION_STOP_HIT,1 */
int MOTION_STOP_HIT;
/* asm: MOTION_RCV_TIKS	.bss	MOTION_RCV_TIKS,1 */
int MOTION_RCV_TIKS;
// *----------------------------------------------------------------------------
// *This is the initial check of the galil board.
// *The CMOS value will be set to ON if cool else OFF
// *No other routine may turn it ON.
// *No other routine will ever download or initialize galil
// *This routine may only be called at powerup
// *
// *
// *
// *
// 	;test limit switches if not found in 3 seconds shutdown
// 	;motion
// 	;
// 	;
// 	;
// 	;check for safety errors
// 	;hold for 10 seconds until all errors
// 	;are cleared.
// 	;
// 	;
// 	;no safety errors, continue on
// 	;
// 	;insert checking for limit switches
// 	;
// 	;
// 	;if all limit switches are activated, then exit
// 	;check for mats or optos
// 	;start over if error
// 	;here we would error out
// 	;check failsafes
// 	;
// 	;
// 	;
// 	;---> BR if FAILSAFES STILL ON
// 	;
// 	;
// 	;check lower limits
// 	;
// 	;insert checking for limit switches
// 	;
// 	;
// 	;if all limit switches are activated, then exit
// 	;check for mats or optos
// 	;start over if error
// 	;here we would error out
// 	;
// 	;initialize the stuff
// 	;
// 	;hang out for a moment
// 	;
// 	;now dl the program.
// 	;
// 	;
// 	;execute the program
// 	;we are now guarenteed to be cool
// 	;
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *an intialization error has occurred:
// *	shutdown and dont use the galil
// *
// *
// ;MOTOROFF	.string	"ST;MO",13,0
// ;MOTOROFF	.string	"ST",13,0
// ;MOTOROFF	.string	"TL 0.6,0.6,1.8;JG 90000,90000,90000;BG;WT 4000;TL 0.5,0.5,1.6;WT 4000;TL 0.3,0.3,1.4;WT 4000;ST XYZ;MO",13,0
// ;
// ;MOTOROFF1	.string	"TL 0.6,0.6,1.8;JG 90000,90000,90000;BG",13,0
// ;MOTOROFF2	.string	"TL 0.5,0.5,1.6;WT 3000;TL 0.4,0.4,1.4;WT 3000;TL 0.3,0.3,1.2",13,0
// ;MOTOROFF3	.string	"TL 0.2,0.2,1.0;WT 3000;TL 0.1,0.1,0.8;WT 3000;TL 0.0,0.0,0.6",13,0
// ;MOTOROFF4	.string	"ST XYZ;MO",13,0
// ;MOTOROFF4	.string	0
// ;
/* asm: BABAID	.word	JJG */
int BABAID = (int)(JJG);
/* asm: JJG	.word	MM0,MM1,MM2,MM3,MM4,MM5,MM6,MM7 */
int JJG[] = { MM0, MM1, MM2, MM3, MM4, MM5, MM6, MM7 };
const char *MM0 = "UNKNOWN ERROR";
const char *MM1 = "MAT NOT PLUGGED IN";
const char *MM2 = "MAT STEPPED ON";
const char *MM3 = "SAFETY BEAM PATH BROKEN";
const char *MM4 = "SAFETY BEAM DETECTOR NOT RECEIVING";
const char *MM5 = "SAFETY BEAM LIGHT NOT EMITTING";
const char *MM6 = "FAIL SAFE SWITCH ENGAGED";
const char *MM7 = "FAIL SAFE SWITCH NOT CONNECT PROPERLY";
// 	;maybe reported as 'error 48'
// *----------------------------------------------------------------------------
// ;if EVER failsafe problem MOTOR OFF
// 	;now message on screen
// 	;countdown 10 seconds
// 	;if in 10 seconds error not cleared
// 	;no motion for game
// ;move it up
// ;read status
// ;check for upper limit switches
// ;if not found in 3 seconds shut down NO MOTION
// ;check failsafes and all errors
// ;if failsafes still engaged msg "NO MOTION FOR THIS GAME"
// ;other safety error  10 seconds to clear if not clear NO MOTION FOR GAME
// ;dl the program
// ;has error been latched?
// ;F->continue
// ;T->dl program again if > 2x than NO MOTION
// ;xq the program
// ;if ST from galil (during gameplay) MOTION OFF until reset
// ;	hit the ABORT LINE ???
// ;	stop sending commands
// ;TS
// *----------------------------------------------------------------------------
const char *bbd = "MOTION BURNIN";
// 	;
// 	;MOTION SAFETY CODE
// 	;
#define MOTION_ERROR_TIKS ((57*5))
/* asm: WAITTIK	.bss	WAITTIK,1 */
int WAITTIK;
// 	;here is where we want to test to see if we are done with the error
// 	;and how we are to recover
// ;	CALL	MOTION_DLPROG
// ;	LDI	300,R0
// ;	STI	R0,@MOTION_RCV_TIKS
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
// 	;
// 	;dl the program.
// 	;
/* asm: MOTION_SAFETY_ON	.bss	MOTION_SAFETY_ON,1 */
int MOTION_SAFETY_ON;
/* asm: MOTION_SAFETY_TYPE	.bss	MOTION_SAFETY_TYPE,1 */
int MOTION_SAFETY_TYPE;
// 	;we do have an error
// 	;
const char *MSSM = "MOTION STOP BUTTON HIT";
// *FIND AVERAGE Y POSITION = R5
// *GET Y HEIGHT DELTA TO CENTER
// *COMPUTE EQUATION M=2X-X*X/150
// ;	MPYF	R0,R2
// ;	MPYF	R0,R3
// ;	MPYF	R0,R4
// ;
// *GET LEAN ANGLES 20% OF MOTION (10% OF MOTION RESERVED)
// *FRONT-BACK (XLEAN)
// *LEFT-RIGHT (ZLEAN)
// *LIMIT MOTION +-1.0
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// ;	.word	T60,T1,T2,T3,T4,T61,T63,T64,T65,0
// ;	.word	T60,T1,T2,T3,T4,T61,T62,T63,T64,T65,0
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *clear the abort (restore communication to galil)
// *
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *MOTION DIPSWITCH OFF
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *
// *
// *RETURNS
// *	C	IF ERROR
// *	NC	IF NOT ERROR
// *
// *
/* asm: CME_MASK	.word	0FF80h */
int CME_MASK = (int)(0x0FF80);
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
/* asm: LLG6X	.word	L4ABX */
int LLG6X = (int)(L4ABX);
/* asm: LLG6Y	.word	L4ABY */
int LLG6Y = (int)(L4ABY);
/* asm: LLG6Z	.word	L4ABZ */
int LLG6Z = (int)(L4ABZ);
const char *L4ABX = "X ENCODER FAILURE";
const char *L4ABY = "Y ENCODER FAILURE";
const char *L4ABZ = "Z ENCODER FAILURE";
// ;
// ; galil program will take care of this - ml
// ;
// ;	;send shutdown commands
// ;	LDL	MOTOROFF,AR2
// ;	CALL	SEND_CMD
// ;	RETS
// *----------------------------------------------------------------------------
/* asm: GALIL_STATUS_X	.bss	GALIL_STATUS_X,1 */
int GALIL_STATUS_X;
/* asm: GALIL_STATUS_Y	.bss	GALIL_STATUS_Y,1 */
int GALIL_STATUS_Y;
/* asm: GALIL_STATUS_Z	.bss	GALIL_STATUS_Z,1 */
int GALIL_STATUS_Z;
// *----------------------------------------------------------------------------
// *
// *GET HEX STRING
// *
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *We must take into consideration:
// *	IF a limit switch is bogus, we can compensate by
// *	doing:
// *
// *	"JG -26000,-26000,-26000;BG",13,"WT 3100",13,
// *	"SH",13
// *	"PR 500,500,500"		;just to relieve the pressure a bit...
// *	"DP 0,0,0",13
// *	"SP 15000,15000,15000",13,0
// *	etc... etc...
// *
// *	NO NOT TRUE
// *
// *AC	ACCERATION
// *BG	BEGIN MOTION
// *DC	DECELERATION
// *DP	DEFINE POSITION AS
// *IT	INDEPENDANT TIME CONSTANT (SMOOTHING FUNCTION)
// *JG	JOG (MOVE AT SPEED)
// *JP	JUMP
// *MO	MOTOR OFF
// *PA	POSITION ABSOLUTE
// *PR	POSITION RELATIVE
// *SP	SPEED
// *WT	WAIT
// *XQ	EXECUTE (LABEL)
// *#<>	LABEL
// *EN	END OF PROGRAM
// *ED	EDIT (OPEN BUFFER)
// *^Q	(17/CTRL-Q) (END OF BUFFER)
// *
// ;T25	.string	"JP #CHKX,(_TTX*_TTX)>21",13,0
// ;T26	.string	"JP #CHKY,(_TTY*_TTY)>21",13,0
// ;T27	.string	"JP #CHKZ,(_TTZ*_TTZ)>21",13,0
// ;T35	.string	"JP #LOOP,(_TTX*_TTX)<7",13,0
// ;T45	.string	"JP #LOOP,(_TTY*_TTY)<7",13,0
// ;T55	.string	"JP #LOOP,(_TTZ*_TTZ)<7",13,0		;LOFF DLTA November 9,1994
const char *PXC1 = "XA=";
const char *PYC1 = "YA=";
const char *PZC1 = "ZA=";
// *----------------------------------------------------------------------------

void INITIALIZATION_MOTION_CHECK(void)
{
    // asm: 	CALL	CLEAR_LATCH_ERROR
    // asm: 	LDI	1,R2
    // asm: 	SETAUD	ADJ_MOTION_PRESENT
    // asm: 	LDI	0,R2
    // asm: 	SETAUD	AUD_RESET_TOTALLY
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	RETSNZ
    // asm: 	LDI	1,RC
    // asm: 	TEXTITT	"MOTION SETUP TESTING...",1,250
    // asm:         CALL    RESET_GALIL
    // asm:         CALL    WAIT_ACK_REAL
    // asm: 	CALL	CHECK_MOTION_PRESENT
    // asm: 	BNE	INIT_MOTION_ERROROUT
    // ;send it UP, for .5 inch  (1000)
    // ;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITIALIZATION_MOTION_CHECK", 0, 0);
    UNIMPL();
}

void SEND_MOTOR_OFF(void)
{
    // asm:         CALL    RESET_GALIL
    // asm:         CALL    WAIT_ACK_REAL
    // 	;bong sound
    // asm: 	LDI	984,AR2
    // asm: 	CALL	SENDSND
    // 	;2.5 sec delay
    // asm: 	LDI	2500,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	LDL	MOTOROFF12,AR2
    // asm: 	CALL	SEND_CMD
    // asm:         CALL    WAIT_ACK
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_MOTOR_OFF", 0, 0);
    UNIMPL();
}

void SEND_MOTOR_OFF_NO_RESET(void)
{
    // asm: 	LDL	MOTOROFF0,AR2
    // asm: 	CALL	SEND_CMD
    // asm: 	CALL    WAIT_ACK
    // asm: 	FILL	1,200,500,390,0
    // asm: 	LDI	500,R0			;WAIT
    // asm: 	CALL	WAITMSEC
    // asm: 	LDL	MOTOFF_PROG,AR4
MOTOFF_LOOP:
    // asm: 	LDI	1,RC
    // asm: 	TEXTITT	"SLOWLY TURNING OFF MOTORS",1,200
    // asm: 	LDI	984,AR2			;BONG
    // asm: 	CALL	SENDSND
    // asm: 	LDI	2000,R0			;WAIT
    // asm: 	CALL	WAITMSEC
    // asm: 	LDI	*AR4++,AR2		;SEND NEXT TORQUE LEVEL
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	MOTOFF_DONE
    // asm: 	CALL	SEND_CMD
    // asm: 	CALL	WAIT_ACK
    // asm: 	FILL	1,200,500,390,0		;CLEAR MESSAGE
    // asm: 	LDI	500,R0			;WAIT
    // asm: 	CALL	WAITMSEC
    // asm: 	LDI	*AR4++,AR2		;SEND NEXT TORQUE LEVEL
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	MOTOFF_DONE
    // asm: 	CALL	SEND_CMD
    // asm: 	CALL	WAIT_ACK
    // asm: 	BU	MOTOFF_LOOP
MOTOFF_DONE:
    // asm: 	FILL	1,200,500,390,0
    // asm: 	RETS
INIT_MOTION_ERROROUT:
    // asm: 	CALL    SEND_MOTOR_OFF_NO_RESET
    // asm: 	CLRI	R2
    // asm: 	SETAUD	ADJ_MOTION_PRESENT
    // 	;show message on screen
    // 	;
    // asm: 	FILL	1,250,500,350,0
    // asm: 	LDI	4,AR5
GDLPAA:
    // asm: LDI	1,RC
    // asm: 	TEXTITT	"MOTION SYSTEM HAS ERRORS",100,250
    // asm: 	TEXTITT	"MOTION SYSTEM DEACTIVATED",100,260
    // asm: 	LDI	984,AR2
    // asm: 	CALL	SENDSND
    // asm: 	LDI	450,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	FILL	1,250,500,350,0
    // asm: 	LDI	150,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	DBU	AR5,GDLPAA
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R0
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	12,R0		;right justify
    // asm: 	AND	0Fh,R0
    // asm: 	CMPI	8,R0
    // asm: 	BLT	NNE1
    // asm: 	LDI	1,RC
    // asm: 	TEXTITT	"BOARD NOT PLUGGED IN",100,250
    // asm: 	BU	FFFD
NNE1:
    // asm: 	LDI	R0,AR2
    // asm: 	ADDI	@BABAID,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	LDI	1,RC
    // asm: 	LDI	100,R2
    // asm: 	LDI	250,R3
    // asm: 	CALL	_outtextxyc
FFFD:
    // asm: 	LDI	3000,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	FILL	1,250,500,350,0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_MOTOR_OFF_NO_RESET", 0, 0);
    UNIMPL();
}

void WAIT500(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *WAIT 500
    // *
    // ;	RPTS	490
    // asm: 	RPTS	100
    // asm: 	NOP
    // asm: 	PUSH	R0
    // asm: 	LDI	@_MODE,R0
    // asm: 	AND	MMODE,R0
    // asm: 	CMPI	MDIAG,R0
    // asm: 	BNE	WT500X
    // asm: 	CALL	FEED_WATCHDOG 	;FEED THE DOG WHEN IN DIAGNOSTICS
WT500X:
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT500", 0, 0);
    UNIMPL();
}

void CHECK_MOTION_DIP(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *CHECK FOR MOTION DIPSWITCH
    // *RET NE IF MOTION DIPSWITCH OFF
    // *
    // asm: 	PUSH	AR2
    // asm: 	CLRI	AR2
    // asm: 	LDP	@DIPSW
    // asm: 	LDI	@DIPSW,R0	;READ DIPSWITCH
    // asm: 	LDI	*AR2,AR2
    // asm: 	POP	AR2
    // asm: 	RS	16,R0
    // asm: 	SETDP
    // asm: 	TSTB	DIP_MOTION,R0	;MOTION CABINET?
    // asm: 	RETS
    // *
    // *CHECK IF MOTION PRESENT
    // *RET NE IF MOTION NOT PRESENT
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_MOTION_DIP", 0, 0);
    UNIMPL();
}

void CHECK_MOTION_PRESENT(void)
{
    // asm: 	READAUD	ADJ_MOTION_PRESENT
    // asm: 	CMPI	1,R0
    // asm:  	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_MOTION_PRESENT", 0, 0);
    UNIMPL();
}

void DIAG_MOTION_TESTZ(void)
{
    // asm: 	LDI	1,IR1
    // asm: 	BU	MOTION_BURNIN_J
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_MOTION_TESTZ", 0, 0);
    UNIMPL();
}

void DIAG_MOTION_TESTX(void)
{
    // asm: 	LDI	2,IR1
    // asm: 	BU	MOTION_BURNIN_J
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_MOTION_TESTX", 0, 0);
    UNIMPL();
}

void DIAG_MOTION_TESTY(void)
{
    // asm: 	LDI	4,IR1
    // asm: 	BU	MOTION_BURNIN_J
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_MOTION_TESTY", 0, 0);
    UNIMPL();
}

void DIAG_MOTION_BURNIN(void)
{
    // asm: 	LDI	7,IR1
MOTION_BURNIN_J:
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	BNZ	GALIL_ERR1		;RETURN IF NON MOVING
    // asm: 	CALL	CHECK_MOTION_PRESENT
    // asm: 	BNE	GALIL_ERROR
    // ;	CALL	INITIALIZE_GALIL
    // asm: 	CALL	INITIALIZATION_MOTION_CHECK
    // asm: 	CALL	CHECK_MOTION_PRESENT
    // asm: 	BNE	GALIL_ERROR
    // asm: 	CALL	CLRSCRN
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0		;set active screen to 1 (writeable)
    // asm: 	STPI	R0,@ACTIVE_SCREEN
    // asm: 	CLRI	AR4			;page index
    // asm: 	LDI	DEFAULT_COLOR,RC
    // asm: 	TEXTIT	bbd,50,330
    // asm: 	CLRF	R5
    // asm: 	CLRF	R6
    // asm: 	CLRF	R7
MBI_LP:
    // asm: 	CLRI	AR2
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R0
    // asm: 	LDI	*AR2,AR2
    // asm: 	SETDP
    // asm: 	RS	8,R0
    // asm: 	LS	24,R0
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	JAJA
    // asm: 	CALL	MOTION_SAFETY_SWITCHES_DIAG
    // asm: 	BU	ENTER_DIAG
JAJA:
    // asm: 	TSTB	1,IR1
    // asm: 	BZ	JJ5
    // asm: 	ADDFL	0.03,R5
    // asm: 	CMPF	TWOPI,R5
    // asm: 	BLT	JJ5
    // asm: 	SUBF	TWOPI,R5
JJ5:
    // asm: 	TSTB	2,IR1
    // asm: 	BZ	JJ6
    // asm: 	ADDFL	0.05,R6
    // asm: 	CMPF	TWOPI,R6
    // asm: 	BLT	JJ6
    // asm: 	SUBF	TWOPI,R6
JJ6:
    // asm: 	TSTB	4,IR1
    // asm: 	BZ	JJ7
    // asm: 	ADDFL	0.04,R7
    // asm: 	CMPF	TWOPI,R7
    // asm: 	BLT	JJ7
    // asm: 	SUBF	TWOPI,R7
JJ7:
    // asm: 	LDF	R7,R2
    // asm: 	PUSHFL	R5
    // asm: 	PUSHFL	R6
    // asm: 	PUSHFL	R7
    // asm: 	CALL	_HPsin
    // asm: 	LDF	R0,R4
    // asm: 	POPFL	R7
    // asm: 	POPFL	R6
    // asm: 	POPFL	R5
    // asm: 	LDF	R6,R2
    // asm: 	PUSHFL	R4
    // asm: 	PUSHFL	R5
    // asm: 	PUSHFL	R6
    // asm: 	PUSHFL	R7
    // asm: 	CALL	_HPsin
    // asm: 	LDF	R0,R3
    // asm: 	POPFL	R7
    // asm: 	POPFL	R6
    // asm: 	POPFL	R5
    // asm: 	POPFL	R4
    // asm: 	LDF	R5,R2
    // asm: 	PUSHFL	R3
    // asm: 	PUSHFL	R4
    // asm: 	PUSHFL	R5
    // asm: 	PUSHFL	R6
    // asm: 	PUSHFL	R7
    // asm: 	CALL	_HPsin
    // asm: 	LDF	R0,R2
    // asm: 	POPFL	R7
    // asm: 	POPFL	R6
    // asm: 	POPFL	R5
    // asm: 	POPFL	R4
    // asm: 	POPFL	R3
    // asm: 	PUSHFL	R5
    // asm: 	PUSHFL	R6
    // asm: 	PUSHFL	R7
    // 	;R2	front
    // 	;R3	left
    // 	;R4	right
    // 	;
    // 	;-1>=x>=1
    // asm: 	MPYF	0.8,R2
    // asm: 	MPYF	0.8,R3
    // asm: 	MPYF	0.8,R4
    // 	;-.8>=x>=.8
    // asm: 	CALL	MOTION_SCALE_ENTER
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	POPFL	R7
    // asm: 	POPFL	R6
    // asm: 	POPFL	R5
    // asm: 	LDI	@_newbut,R0
    // asm: 	LDI	R0,R1
    // asm: 	TSTB	SW_DIAG,R1		;move up
    // asm: 	BNZ	MBI_LP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DIAG_MOTION_BURNIN", 0, 0);
    UNIMPL();
}

void PLMOTION(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *PLAYER CABINET MOTION
    // *
    // *PARAMETERS
    // *	AR4	PLAYER CAR OBJECT
    // *	AR5	PLAYER CAR STRUCTURE
    // *
    // *CONTROL AXES:
    // *		X=LFT REAR 0=GROUND, 50000=FULL HEIGHT
    // *		Y=RT REAR  0=GROUND, 50000=FULL HEIGHT
    // *		Z=FRONT	   0=GROUND, 50000=FULL HEIGHT
    // *
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	RETSNZ			;RETURN IF NON MOVING
    // asm: 	LDI	@MOTION_STOP_HIT,R0
    // asm: 	RETSNZ
    // 	;
    // 	;check for an abort
    // 	;
    // asm: 	LDI	@ABORT_QUERY,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@ABORT_QUERY
    // asm: 	BGT	NOCHECK
    // asm: 	LDI	30,R0
    // asm: 	STI	R0,@ABORT_QUERY
    // asm: 	LDL	ABORTQ,AR2
    // asm: 	CALL	SEND_CMD
    // asm: 	CALL	WAIT_ACK
NOCHECK:
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLMOTION", 0, 0);
    UNIMPL();
}

void MOTION_SCALE_ENTER(void)
{
    // *SEND STUFF TO MOTION PLATFORM
    // *R2=Z FRONT		-1.0->1.0
    // *R3=X LFT REAR		-1.0->1.0
    // *R4=Y RT REAR		-1.0->1.0
    // 	;NOW SCALE INTO THE RANGE USEABLE
    // 	;BY OR MOTION SYSTEM
    // 	;
    // asm: 	FLOAT	17000,R1
    // asm: 	MPYF	R1,R2
    // asm: 	MPYF	R1,R3
    // asm: 	MPYF	R1,R4
    // asm: 	FLOAT	26000,R1
    // asm: 	ADDF	R1,R2
    // asm: 	ADDF	R1,R3
    // asm: 	ADDF	R1,R4
    // asm: 	FIX	R2
    // asm: 	FIX	R3
    // asm: 	FIX	R4
    // asm: JAJA555
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // ;	PUSHFL	R3
    // ;	PUSHFL	R4
    // asm: 	LDI	@LAST_ZF,R1
    // asm: 	SUBI	R2,R1
    // asm: 	ABSI	R1
    // asm: 	CMPI	MIN_MOVE_DIST,R1
    // asm: 	BLT	N76
    // asm: 	STI	R2,@LAST_ZF
    // asm: 	LDL	sPOTVALUE,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDL	PZC1,AR0
    // asm: 	LDL	MOTION_CMDSTR,AR1
    // asm: 	CALL	STRCPY
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	sPOTVALUE,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	PZC2,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	AR0,AR2
    // asm:         CALL    SEND_CMD
    // asm:         CALL    WAIT_ACK
N76:
    // asm: 	POP	R2		;RT REAR (Y)
    // asm: 	LDI	@LAST_YR,R1
    // asm: 	SUBI	R2,R1
    // asm: 	ABSI	R1
    // asm: 	CMPI	MIN_MOVE_DIST,R1
    // asm: 	BLT	N77
    // asm: 	STI	R0,@LAST_YR
    // asm: 	LDL	sPOTVALUE,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDL	PYC1,AR0
    // asm: 	LDL	MOTION_CMDSTR,AR1
    // asm: 	CALL	STRCPY
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	sPOTVALUE,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	PYC2,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	AR0,AR2
    // asm:         CALL    SEND_CMD
    // asm:         CALL    WAIT_ACK
N77:
    // asm: 	POP	R2		;LT REAR (X)
    // asm: 	LDI	@LAST_XL,R1
    // asm: 	SUBI	R2,R1
    // asm: 	ABSI	R1
    // asm: 	CMPI	MIN_MOVE_DIST,R1
    // asm: 	BLT	N78
    // asm: 	STI	R0,@LAST_XL
    // asm: 	LDL	sPOTVALUE,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDL	PXC1,AR0
    // asm: 	LDL	MOTION_CMDSTR,AR1
    // asm: 	CALL	STRCPY
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	sPOTVALUE,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	PXC2,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	AR0,AR2
    // asm:         CALL    SEND_CMD
    // asm:         CALL    WAIT_ACK
N78:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_SCALE_ENTER", 0, 0);
    UNIMPL();
}

void LEVEL_THE_MOTION(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	RETSNZ			;RETURN IF NON MOVING
    // asm: 	CALL	CHECK_MOTION_PRESENT
    // asm: 	RETSNE
    // ;	LDF	-1.26,R2
    // ;	LDF	-1.26,R3
    // ;	LDF	-1.26,R4
    // ;	BU	MOTION_SCALE_ENTER
    // asm: 	LDI	0,R2
    // asm: 	STI	R2,@LAST_ZF
    // asm: 	LDL	sPOTVALUE,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDL	PZC1,AR0
    // asm: 	LDL	MOTION_CMDSTR,AR1
    // asm: 	CALL	STRCPY
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	sPOTVALUE,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	PZC2,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	AR0,AR2
    // asm:         CALL    SEND_CMD
    // asm:         CALL    WAIT_ACK
    // asm: 	LDI	0,R2		;RT REAR (Y)
    // asm: 	LDL	sPOTVALUE,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDL	PYC1,AR0
    // asm: 	LDL	MOTION_CMDSTR,AR1
    // asm: 	CALL	STRCPY
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	sPOTVALUE,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	PYC2,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	AR0,AR2
    // asm:         CALL    SEND_CMD
    // asm:         CALL    WAIT_ACK
    // asm: 	CLRI	R2		;LT REAR (X)
    // asm: 	LDL	sPOTVALUE,AR2
    // asm: 	CALL	_itoa
    // asm: 	LDL	PXC1,AR0
    // asm: 	LDL	MOTION_CMDSTR,AR1
    // asm: 	CALL	STRCPY
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	sPOTVALUE,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDL	MOTION_CMDSTR,AR0
    // asm: 	LDL	PXC2,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	AR0,AR2
    // asm:         CALL    SEND_CMD
    // asm:         CALL    WAIT_ACK
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEVEL_THE_MOTION", 0, 0);
    UNIMPL();
}

void DOWNLOAD_PROGRAM(void)
{
    // ;INITPROGL
    // ;	.word	EDIT,T5,T6,T7,T8,T9
    // ;	.word	T10,T11,T13,T14,T15,T16,T17,T18,T19
    // ;	.word	T20,T21,T22,T23,T25,T26,T27,T28,T29
    // ;	.word	T30,T31,T32,T33,T34,T35,T36,T37,T38,T39
    // ;	.word	T40,T41,T42,T43,T44,T45,T46,T47,T48,T49
    // ;	.word	T50,T51,T52,T53,T54,T55,T56,T57,T58,T59
    // ;	.word	T60
    // ;
    // ;	.word	T1,T2,T3
    // ;	.word	T66,T67,T68,T69
    // ;	.word	T70,T71,T72,T73,T74,T75,T76,T77,T78,T79
    // ;	.word	T80,T81,T82,T83,T84
    // ;	.word	0
    // ;
    // *----------------------------------------------------------------------------
IBO654:
    // asm: 	LDI	*AR4++,AR2
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	DNF
    // asm: 	CALL	SEND_CMD
    // asm: 	CALL	WAIT_ACK	;#
    // asm: 	CALL	WAIT_FOR_VBLANK
    // asm: 	BU	IBO654
DNF:
    // asm: 	LDL	ENDED,AR2
    // asm: 	CALL	SEND_CMD
    // asm: 	CALL	WAIT_ACK	;:
    // asm: 	LDL	XQ,AR2
    // asm: 	CALL	SEND_CMD
    // asm: 	CALL	WAIT_ACK
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
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DOWNLOAD_PROGRAM", 0, 0);
    UNIMPL();
}

void SEND_CMD(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *SEND COMMAND TO GALIL MOTION CONTROL BOARD
    // *
    // *
    // *FF09
    // *F709
    // *FF09
    // *
    // *FF00
    // *FB00
    // *FF00
    // *
    // *
    // *PARAMETERS
    // *	AR2	PTR TO STR
    // *
    // *
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSH	R6
    // asm: 	PUSH	R7
    // asm: 	DINT
    // asm: 	LDIL	SOUND,AR3
    // asm: 	LDI	0FF09h,R0
    // asm: 	LDI	0F709h,R1
    // asm: 	STI	R0,*AR3
    // asm: 	CALL	WAIT500
    // asm: 	STI	R1,*AR3
    // asm: 	CALL	WAIT500
    // asm: 	STI	R0,*AR3
    // asm: 	CALL	WAIT500
    // asm: 	LDI	0FF00h,R2
    // asm: 	LDI	0FB00h,R3
    // asm: 	CLRI	R5		;SHIFT COUNT
LLKK:
    // asm: CMPI	-32,R5
    // asm: 	BNE	REGLP
    // asm: 	CLRI	R5
    // asm: 	NOP	*AR2++
REGLP:
    // asm: 	LDI	*AR2,R4
    // asm: 	LSH	R5,R4
    // asm: 	SUBI	8,R5
    // asm: 	AND	0FFh,R4
    // asm: 	CMPI	0,R4
    // asm: 	BZ	KKKII
    // asm: 	OR	R4,R2,R6
    // asm: 	OR	R4,R3,R7
    // asm: 	STI	R6,*AR3
    // asm: 	CALL	WAIT500
    // asm: 	STI	R7,*AR3
    // asm: 	CALL	WAIT500
    // asm: 	STI	R6,*AR3
    // asm: 	CALL	WAIT500
    // asm: 	BU	LLKK
KKKII:
    // asm: 	EINT
    // asm: 	POP	R7
    // asm: 	POP	R6
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SEND_CMD", 0, 0);
    UNIMPL();
}

void RESET_GALIL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	DINT
    // asm:         LDIL    SOUND,AR3
    // asm:         LDI     0FF0Ch,R0
    // asm:         LDI     0F70Ch,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         LDI     0FF00h,R0
    // asm:         LDI     0FB00h,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         LDI     0FF0bh,R0
    // asm:         LDI     0F70bh,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         LDI     0FF00h,R0
    // asm:         LDI     0FB00h,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm: 	EINT
    // asm:         RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RESET_GALIL", 0, 0);
    UNIMPL();
}

void ABORT_RESET_GALIL(void)
{
    // asm: 	DINT
    // asm:         LDIL    SOUND,AR3
    // 	;
    // 	;in essence we are clearing the abort, but NOT reseting galil
    // 	;
    // asm:         LDI     0FF0eh,R0	;  0Ch = reset bit addr
    // asm:         LDI     0F70eh,R1	;  0Eh = abort/reset bit addr
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         LDI     0FF00h,R0
    // asm:         LDI     0FB00h,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL	WAIT500
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm: 	EINT
    // asm: 	LDL	XQ,AR2				;tell galil to continue executing program
    // asm: 	CALL	SEND_CMD
    // asm: 	CALL	WAIT_ACK
    // asm:         RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ABORT_RESET_GALIL", 0, 0);
    UNIMPL();
}

void MOTION_DLPROG(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	RETSNZ			;RETURN IF NON MOVING
    // asm: 	CALL	CHECK_MOTION_PRESENT
    // asm: 	RETSNE
    // asm: 	LDL	THEPROGL,AR4
    // asm: 	CALL	DOWNLOAD_PROGRAM
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@LAST_ZF
    // asm: 	STI	R0,@LAST_XL
    // asm: 	STI	R0,@LAST_YR
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@MOTION_NOT_ON
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOTION_DLPROG", 0, 0);
    UNIMPL();
}

void GALIL_ERROR(void)
{
    // *MOTION NO DETECTED
    // ;	.globl	MOTION_SAFETY_SWITCHES_DIAG
    // ;	CALL	MOTION_SAFETY_SWITCHES_DIAG
    // ;	BU	ENTER_DIAG
    // ;	CALL	CLRSCRN
    // asm: 	CALL	SETPAGE0
    // asm: 	LDIL	SCREEN0,R0
    // asm: 	STI	R0,@ACTIVE_SCREEN
    // asm: 	LDI	2,AR5
    // asm: GLP
    // asm: 	LDI	0,RC
    // asm: 	TEXTITT	"WARNING MOTION NOT DETECTED",150,130
    // asm: 	LDI	250,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	LDI	COL_WHITE,RC
    // asm: 	TEXTITT	"WARNING MOTION NOT DETECTED",150,130
    // asm: 	LDI	984,AR2
    // asm: 	CALL	SENDSND
    // asm: 	LDI	250,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	DBU	AR5,GLP
    // asm: 	RETS
    // *
    // *WAIT R0 X MSEC
    // *
    // asm: WAITMSEC
    // asm: 	MPYI	5000,R0
WTMLP:
    // asm: 	SUBI	1,R0
    // asm: 	BGE	WTMLP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GALIL_ERROR", 0, 0);
    UNIMPL();
}

void CHECK_MOTION_ERROR(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	CLRI	AR0
    // asm: 	LDP	@991030h
    // asm: 	LDI	@991030h,R0
    // asm: 	LDI	*AR0,AR0
    // asm: 	SETDP
    // asm: 	AND	@CME_MASK,R0
    // asm: 	BZ	CME_NO_MOTION_ERRORS
    // asm: 	SETC
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
CME_NO_MOTION_ERRORS:
    // asm: 	CLRC
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_MOTION_ERROR", 0, 0);
    UNIMPL();
}

void CLEAR_LATCH_ERROR(void)
{
    // asm: 	DINT
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@LATCHED_ERROR
    // asm: 	EINT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_LATCH_ERROR", 0, 0);
    UNIMPL();
}

void LATCH_ERROR(void)
{
    // asm: 	CALL	CHECK_MOTION_DIP
    // asm: 	RETSNZ			;RETURN IF NON MOVING
    // asm: 	CALL	CHECK_MOTION_ERROR
    // asm: 	RETSNC
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@LATCHED_ERROR
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LATCH_ERROR", 0, 0);
    UNIMPL();
}

void G_READY(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *CHECK GALIL READY
    // *RET CS IF GALIL READY W/ CHAR
    // *
    // asm:         LDIL    SOUND,AR3
    // asm:         LDI     0FF0Ah,R0
    // asm:         LDI     0F70ah,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL 	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL 	WAIT500
    // asm:         STI     R0,*AR3
    // asm:         CALL	WAIT500
    // asm:         LDI     0FF00h,R0
    // asm:         LDI     0Fb00h,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL 	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL 	WAIT500
    // asm:         LDI     *AR3,R1		;read status
    // asm:         STI     R0,*AR3
    // asm:         CALL 	WAIT500
    // asm: 	LSH	17,R1		;PUT INTO CARRY
    // asm: 	RETS
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
    // asm: 	CALL	G_READY
    // asm: 	RETSNC			;RETURN IF NOTHING
    // asm: 	CALL	G_READY
    // asm: 	RETSNC			;RETURN IF NOTHING
    // asm:         LDIL    SOUND,AR3
    // asm:         LDI     0FF08h,R0
    // asm:         LDI     0F708h,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL 	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL 	WAIT500
    // asm:         STI     R0,*AR3
    // asm:         CALL 	WAIT500
    // asm:         LDI     0FF00h,R0
    // asm:         LDI     0Fb00h,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL 	WAIT500
    // asm:         STI     R1,*AR3
    // asm:         CALL 	WAIT500
    // asm:         LDI     *AR3,R1
    // asm:         RS      8,R1
    // asm:         AND     0FFh,R1
    // asm:         STI     R0,*AR3
    // asm:         CALL 	WAIT500
    // asm: 	SETC
    // asm: 	RETS
    // *
    // *GET GALIL STRING
    // *
    TRACE_EVENT(&g_crusn_machine->trace, "function", "G_CHAR", 0, 0);
    UNIMPL();
}

void G_STRING(void)
{
GS0:
    // asm: 	CALL	G_CHAR	   		;CHARACTER READY?
    // asm: 	BNC	GSX			;NO...
    // asm: 	CMPI	'G',R1	;position error
    // asm: 	BNE	NOTPERR	;attempt motion recalibration
    // asm: 	LDI	1,R2
    // asm: 	SETAUD	AUD_RESET_TOTALLY
    // asm: 	LDI	0,R2
    // asm: 	SETAUD	ADJ_MOTION_PRESENT
    // asm: 	BU	GS0
NOTPERR:
    // asm: 	CMPI	'V',R1			;STATUS ANYONE?
    // asm: 	BNE	NOTQSTATUS
    // asm: 	CALL	G_HEX
    // asm: 	AND	6h,R1
    // asm: 	CMPI	6h,R1
    // asm: 	BZ	GS0
    // asm: 	LDI	@WAITTIK,R1
    // asm: 	CMPI	2,R1
    // asm: 	LDILT	2,R1
    // asm: 	STI	R1,@WAITTIK
    // asm: 	BU	GS0
NOTQSTATUS:
    // asm: 	CMPI	'W',R1			;IS IT AN ERROR??
    // asm: 	BNE	NOTQQERRORX		;
    // asm: 	LDI	@LLG6X,AR2
    // asm: 	BU	SHOWENCODER
NOTQQERRORX:
    // asm: 	CMPI	'S',R1			;IS IT AN ERROR??
    // asm: 	BNE	NOTQQERRORY		;
    // asm: 	LDI	@LLG6Y,AR2
    // asm: 	BU	SHOWENCODER
NOTQQERRORY:
    // asm: 	CMPI	'Q',R1			;IS IT AN ERROR??
    // asm: 	BNE	NOTQQERRORZ		;
    // asm: 	LDI	@LLG6Z,AR2
    // asm: 	BU	SHOWENCODER
    TRACE_EVENT(&g_crusn_machine->trace, "function", "G_STRING", 0, 0);
    UNIMPL();
}

void ASCIIHEX(void)
{
    // *
    // *CHANGE ASCIIHEX TO HEX 4 BITS
    // *R1=ASCII
    // *RET R1 =HEX
    // *CC IF INVALID
    // *CS IF VALID
    // *
    // asm: 	CMPI	47H,R1
    // asm: 	BGE	ASCERR
    // asm: 	SUBI	30H,R1
    // asm: 	BN	ASCERR
    // asm: 	CMPI	9,R1
    // asm: 	BLE	AH1
    // asm: 	CMPI	11H,R1
    // asm: 	BLT	ASCERR
    // asm: 	SUBI	7,R1
AH1:
    // asm: 	SETC
    // asm: 	RETS
ASCERR:
    // asm: 	CLRC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASCIIHEX", 0, 0);
    UNIMPL();
}

void WAIT_ACK(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	DINT
    // asm: 	LDI	1000,R6		;EXCESS CHARACTER TIMEOUT
WAIT_ACK_LOOP:
    // asm: 	CALL	G_READY
    // asm: 	BNC	EXITL2 		;NOTHING THERE DUDES...
    // asm: 	LDI	300,R0
    // asm: 	STI	R0,@MOTION_RCV_TIKS
    // asm: 	CALL	G_STRING
    // asm: 	BNC	EXITL2 		;NOTHING THERE DUDES...
    // asm: 	DEC	R6
    // asm: 	BZ	NO_RESPONSE	;TOO MANY CHARACTERS
    // asm:         BU      WAIT_ACK_LOOP
EXITL2:
    // asm: 	EINT
    // asm:         RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_ACK", 0, 0);
    UNIMPL();
}

void NO_RESPONSE(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	EINT
    // asm: 	LDI	0,R2
    // asm: 	SETAUD	ADJ_MOTION_PRESENT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NO_RESPONSE", 0, 0);
    UNIMPL();
}

void WAIT_ACK_REAL(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // asm: 	LDI	10000,R6	;NO RESPONSE TIMEOUT
WAR0:
    // asm: 	CALL	G_READY
    // asm: 	BC	WAR1
    // asm: 	DEC	R6
    // asm: 	BZ	NO_RESPONSE
    // asm: 	B	WAR0
WAR1:
    // asm: 	LDI	1000,R6		;EXCESS CHARACTER TIMEOUT
WAR1L:
    // asm: 	CALL	G_CHAR
    // asm: 	BNC	EXITL2A		;NO MORE CHARACTERS
    // asm: 	DEC	R6
    // asm: 	BZ	NO_RESPONSE	;TOO MANY CHARACTERS
    // asm:         BU      WAR1L
EXITL2A:
    // asm:         RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WAIT_ACK_REAL", 0, 0);
    UNIMPL();
}

void UPPER_LIMIT_ERROR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // *
    // *
    // *
    // ;
    // ;	CALL	RESET_GALIL
    // ;       CALL    WAIT_ACK_REAL
    // ;
    // ;	;send shutdown commands
    // ;	LDL	MOTOROFF,AR2
    // ;	CALL	SEND_CMD
    // ;
    // asm: 	CALL    SEND_MOTOR_OFF_NO_RESET
    // asm: 	CLRI	R2
    // asm: 	SETAUD	ADJ_MOTION_PRESENT
    // asm: 	LDI	5,AR5
    // asm: ULLERR
    // asm: 	FILL	1,200,500,390,0
    // asm: 	LDI	50,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	LDI	1,RC
    // asm: 	TEXTITT	"UPPER LIMIT SWITCH ERROR",1,200
    // asm: 	LDI	@GALIL_STATUS_X,R0
    // asm: 	TSTB	08h,R0
    // asm: 	BZ	ULL1
    // asm: 	TEXTITT	"X AXIS LIMIT SWITCH NOT FOUND",1,210
ULL1:
    // asm: LDI	@GALIL_STATUS_Y,R0
    // asm: 	TSTB	08h,R0
    // asm: 	BZ	ULL2
    // asm: 	TEXTITT	"Y AXIS LIMIT SWITCH NOT FOUND",1,220
ULL2:
    // asm: LDI	@GALIL_STATUS_Z,R0
    // asm: 	TSTB	08h,R0
    // asm: 	BZ	ULL3
    // asm: 	TEXTITT	"Z AXIS LIMIT SWITCH NOT FOUND",1,230
ULL3:
    // asm: 	LDI	450,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	DBU	AR5,ULLERR
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UPPER_LIMIT_ERROR", 0, 0);
    UNIMPL();
}

void LOWER_LIMIT_ERROR(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // ;
    // ;	CALL	RESET_GALIL
    // ;        CALL    WAIT_ACK_REAL
    // ;
    // ;	;send shutdown commands
    // ;	LDL	MOTOROFF,AR2
    // ;	CALL	SEND_CMD
    // ;
    // asm: 	CALL    SEND_MOTOR_OFF_NO_RESET
    // asm: 	CLRI	R2
    // asm: 	SETAUD	ADJ_MOTION_PRESENT
    // asm: 	FILL	1,200,500,390,0
    // asm: 	LDI	5,AR5
    // asm: LLLERR
    // asm: 	FILL	1,200,500,390,0
    // asm: 	LDI	50,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	LDI	1,RC
    // asm: 	TEXTITT	"LOWER LIMIT SWITCH ERROR",1,200
    // asm: 	LDI	@GALIL_STATUS_X,R0
    // asm: 	TSTB	04h,R0
    // asm: 	BZ	LLL1
    // asm: 	TEXTITT	"X AXIS LIMIT SWITCH NOT FOUND",1,210
LLL1:
    // asm: LDI	@GALIL_STATUS_Y,R0
    // asm: 	TSTB	04h,R0
    // asm: 	BZ	LLL2
    // asm: 	TEXTITT	"Y AXIS LIMIT SWITCH NOT FOUND",1,220
LLL2:
    // asm: LDI	@GALIL_STATUS_Z,R0
    // asm: 	TSTB	04h,R0
    // asm: 	BZ	LLL3
    // asm: 	TEXTITT	"Z AXIS LIMIT SWITCH NOT FOUND",1,230
LLL3:
    // asm: 	LDI	450,R0
    // asm: 	CALL	WAITMSEC
    // asm: 	DBU	AR5,LLLERR
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOWER_LIMIT_ERROR", 0, 0);
    UNIMPL();
}

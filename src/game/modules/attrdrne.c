#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/c30.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/obj.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/dirq.h"
#include "../include/delta.h"
#include "../include/attrdrne.h"
#include "../include/discovered_defines.h"

/*
 * Source module: asm/ATTRDRNE.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
/* asm: ATTRWAVE	pbss	ATTRWAVE,1 */
int ATTRWAVE;
// *----------------------------------------------------------------------------
// * CAMERA variable space
// *
#define CUT_PAN (PDATA+1)
#define ZOOMVEL (PDATA+2)
#define CAMERA_XYZR (PDATA+3)
#define NEWCAMERA_XYZR (PDATA+7)
#define NEW_POSX (PDATA+11)
#define NEW_POSY (PDATA+12)
#define NEW_POSZ (PDATA+13)
#define NEW_RADX (PDATA+14)
#define NEW_RADY (PDATA+15)
#define NEW_RADZ (PDATA+16)
#define NEW_MATRIX (PDATA+17)
#define CAMERA_VIEW (PDATA+26)
#define CAMERA_VEL (PDATA+27)
#define CAROBJ (PDATA+28)
#define CARDIS (PDATA+29)
#define CAMERA_ACCEL (PDATA+30)
#define CAMERA_INFIN (PDATA+31)
#define TEMP_DATA (PDATA+32)
#define LIST_NUM (PDATA+42)
#define CAMERA_LANE (PDATA+43)
#define OBJINS (PDATA+44)
#define CAMYOFF (PDATA+45)
// *----------------------------------------------------------------------------
// *
// ;	.data
// ;STARTXPOS	.float	-1754318
// ;STARTYPOS	.float	2345
// ;STARTZPOS	.float	1314000
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *CREATE THE OBJECT 'CRUISIN USA' FOR OVERLAY DURING ATTRACT MODE
// *
#define LOGO_STARTZ 10000
#define LOGO_ENDX (-120)
#define LOGO_ENDY (-85)
#define LOGO_ENDZ 368
#define LOGO_FLYIN_FRAMES 125
#define LOGO_WHOOSH_FRAMES 462
#define LOGO_SPINZ 18.85
// *----------------------------------------------------------------------------
// *THE first call is to intialize
// *THE first call is to intialize
// *THE first call is to intialize
// *THE first call is to intialize
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
#define ATTR_WAVETAB_LEN ($-ATTR_WAVETAB-1)
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *OLD CODE THAT I DEARLY LIKE TO REFFERENCE
// ;*----------------------------------------------------------------------------
// ;*TURN CAMERA TO FOLLOW OBJECT IN AR4
// ;*
// ;*PARAMETERS
// ;*	AR4	OBJECT TO 'WATCH'
// ;*
// ;WATCH_OBJ:
// ;	LDF	*+AR4(OPOSX),R2
// ;	SUBF	@NEW_CAMERA+CAMERAX,R2
// ;	LDF	*+AR4(OPOSZ),R3
// ;	SUBF	@NEW_CAMERA+CAMERAZ,R3
// ;	CALL	ARCTANF
// ;	SUBF	HALFPI,R0
// ;
// ;	NEGF	R0,R2
// ;	STF	R2,@NEW_CAMERA+CAMERARADY
// ;
// ;	LDF	*+AR4(OPOSZ),R0
// ;	SUBF	@NEW_CAMERA+CAMERAZ,R0
// ;	MPYF	R0,R0
// ;	LDF	*+AR4(OPOSX),R1
// ;	SUBF	@NEW_CAMERA+CAMERAX,R1
// ;	MPYF	R1,R1
// ;	ADDF3	R0,R1,R2	;R= SQRT (X^2 + Z^2)
// ;	CALL	SQRT
// ;	LDF	R0,R3
// ;
// ;	LDF	*+AR4(OPOSY),R2
// ;	SUBF	@NEW_CAMERA+CAMERAY,R2
// ;
// ;	CALL	ARCTANF
// ;
// ;	SUBF	HALFPI,R0
// ;
// ;	STF	R0,@NEW_CAMERA+CAMERARADX
// ;
// ;	LDI	@NEW_CAMERAI,R2
// ;	ADDI	CAMERARADX,R2
// ;	LDI	@NEW_CAMERAI,AR2
// ;	ADDI	CAMERA_MATRIX,AR2
// ;	CALL	FIND_MATRIX
// ;
// ;	CAll	CAMERA_HORIZON_PROJECTION
// ;
// ;	RETS
// ;*----------------------------------------------------------------------------
// ;INITFLYBY_VIEW:
// ;	LDI	-1,R0
// ;	STI	R0,*+AR7(CUT_PAN)
// ;
// ;	FLOAT	-4300,R0
// ;	STF	R0,@NEW_CAMERA+CAMERA_SPEED
// ;
// ;FLYBY_VIEW:
// ;	LDF	@NEW_CAMERA+CAMERA_SPEED,R0
// ;	FLOAT	150,R1
// ;	ADDF	R1,R0
// ;	STF	R0,@NEW_CAMERA+CAMERA_SPEED
// ;
// ;	LDF	*+AR4(OPOSY),R0
// ;	STF	R0,@NEW_CAMERA+CAMERAY
// ;
// ;	LDF	*+AR4(ORADY),R2
// ;	ADDF	-1.54,R2
// ;	CALL	_SINE
// ;	FLOAT	500,R2
// ;	MPYF	R0,R2
// ;	LDF	*+AR4(OPOSX),R0
// ;	ADDF	R2,R0
// ;	STF	R0,@NEW_CAMERA+CAMERAX
// ;
// ;	LDF	*+AR4(ORADY),R2
// ;	ADDF	-1.54,R2
// ;	CALL	_COSI
// ;	FLOAT	500,R2
// ;	MPYF	R0,R2
// ;	LDF	*+AR4(OPOSZ),R0
// ;	ADDF	R2,R0
// ;	STF	R0,@NEW_CAMERA+CAMERAZ
// ;
// ;	LDF	*+AR4(ORADY),R0
// ;	SUBF	0.10,R0
// ;	STF	R0,@NEW_CAMERA+CAMERA_VECY
// ;
// ;	LDF	0.3,R0
// ;	STF	R0,@NEW_CAMERA+CAMERA_VECX
// ;
// ;	LDF	*+AR4(ORADZ),R0
// ;	STF	R0,@NEW_CAMERA+CAMERA_VECZ
// ;
// ;	CALL	MOVE_CAMERA
// ;
// ;	LDF	*+AR4(OPOSY),R0
// ;	FLOAT	-1500,R1			;FIX ELEVATION
// ;	ADDF	R1,R0
// ;	STF	R0,@NEW_CAMERA+CAMERAY
// ;passed
// ;	CALL	WATCH_OBJ
// ;
// ;	RETS

void ATTRACT_DELTA(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(OBJINS)
    // asm: 	LDI	@ATTRWAVE,R4
    // asm: 	LSH	-1,R4
    // asm: 	STI	R4,*+AR7(LIST_NUM)
    // asm: 	CALL	INIT_ATTR_LEG
    // asm: 	LDI	4,R0
    // asm: 	STI	R0,@NOSWAP
    // asm: 	CREATE	LOGO_PROC,UTIL_C
    // asm: 	LDI	@_MODE,R0
    // asm: 	OR	MGO,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	-1,R0
    // asm: 	STI	R0,*+AR7(CUT_PAN)
    // asm: 	LDI	1,R0	    		;SET GAME FRAME RATE
    // asm: 	STI	R0,@FRAMRATE
    // asm: 	STI	R0,@TIMECLR
    // asm: 	STI	R0,@DRONE_DISPATCH_P
    // asm: 	SLEEP	1
    // asm: 	LDI	@PLYCAR,AR4
    // asm: 	LDI	*+AR4(PAR5),AR5
    // asm: 	LDI	*+AR4(PAR4),AR4
    // asm: 	LDI	*+AR5(PAR5),AR0
    // asm: 	STI	AR4,@PLYCAR
    // asm: 	STI	AR0,@PLYCBLK
    // asm: 	LDI	90,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(CUT_PAN)
    // asm: 	CALL	GET_LIST_ADDR
    // asm: 	BR	DELTA_LOOP_ENTRY
aDELTA_LOOP:
    // asm: 	LDI	*AR6,AR0
    // asm: 	CALLU	AR0
ADELTA2:
    // asm: 	CALL	UPDATE_CAMERA
    // asm: 	SLEEP	1
    // asm: 	LDI	@NOSWAP,R0
    // asm: 	BEQ	ADELTA2A
    // asm: 	SUBI	1,R0		;OK Now we have a valid frame
    // asm: 	STI	R0,@NOSWAP
ADELTA2A:
    // asm: 	CMPI	0,AR5
    // asm: 	BGT	aDELTA_LOOP
    // asm: 	ADDI	1,AR6		;Advance to next mode
DELTA_LOOP_ENTRY:
    // asm: 	LDI	*AR6,AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BNE	sDELTA_LOOP
    // ;	CALL	GET_LIST_ADDR
    // ;	BU	$
    // asm: 	SUBI	3,AR6		;Loop on last entry
sDELTA_LOOP:
    // asm: 	LDI	*+AR7(OBJINS),R0
    // asm: 	BEQ	NO_OBJINS
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	OBJ_INSERT			;INSERT PLAYER OBJECT
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(OBJINS)
NO_OBJINS:
    // asm: 	LDI	*AR6++,AR0
    // asm: 	CALLU	AR0
    // asm: 	LDI	*AR6++,AR5
    // asm: 	B	ADELTA2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ATTRACT_DELTA", 0, 0);
    UNIMPL();
}

void LOGO_PROC(void)
{
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	LOGOX
    // asm: 	LDI	AR0,AR4
    // asm: 	LDIL	logo,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	UTIL_C|LOGO_T,R0
    // asm: 	STI	R0,*+AR0(OID)
    // asm: 	FLOAT	LOGO_STARTZ,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	LOGO_SPINZ,R0
    // asm: 	STF	R0,*+AR4(ORADX)
    // asm: 	LDF	0,R7		;SPEED FACTOR
    // asm: 	LDI	LOGO_FLYIN_FRAMES,AR5
LOGO_LOOP1:
    // asm: 	SLEEP	1
    // asm: 	FLOAT	@NFRAMES,R6
    // asm: 	FLOAT	LOGO_ENDY,R0
    // asm: 	CMPF	*+AR4(OPOSY),R0
    // asm: 	BEQ	LOGO2
    // asm: 	FLOAT	LOGO_ENDX,R0
    // asm: 	SUBF	*+AR4(OPOSX),R0
    // asm: 	MPYF	R7,R0
    // asm: 	MPYF	R6,R0
    // asm: 	ADDF	*+AR4(OPOSX),R0
    // asm: 	FLOAT	LOGO_ENDX,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	FLOAT	LOGO_ENDY,R0
    // asm: 	SUBF	*+AR4(OPOSY),R0
    // asm: 	MPYF	R7,R0
    // asm: 	MPYF	R6,R0
    // asm: 	ADDF	*+AR4(OPOSY),R0
    // asm: 	FLOAT	LOGO_ENDY,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	ADDF	0.03,R7
LOGO2:
    // asm: 	FLOAT	LOGO_ENDZ-LOGO_STARTZ,R0
    // asm: 	FLOAT	LOGO_FLYIN_FRAMES,R1
    // asm: 	CALL	DIV_F
    // asm: 	MPYF	R6,R0
    // asm: 	LDF	*+AR4(OPOSZ),R1
    // asm: 	ADDF	R1,R0
    // asm: 	FLOAT	LOGO_ENDZ,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	LOGO_SPINZ,R0
    // asm: 	FLOAT	LOGO_FLYIN_FRAMES,R1
    // asm: 	CALL	DIV_F
    // asm: 	MPYF	R6,R0
    // asm: 	LDF	*+AR4(ORADX),R2
    // asm: 	SUBF	R0,R2
    // asm: 	LDFN	0,R2
    // asm: 	STF	R2,*+AR4(ORADX)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	SUBI	@NFRAMES,AR5
    // asm: 	CMPI	0,AR5
    // asm: 	BP	LOGO_LOOP1
LOGOX:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOGO_PROC", 0, 0);
    UNIMPL();
}

void GET_LIST_ADDR(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*+AR7(LIST_NUM),AR6
    // asm: 	ADDI	@VIEWLISTI,AR6
    // asm: 	LDI	*AR6,AR6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_LIST_ADDR", 0, 0);
    UNIMPL();
}

void INIT_STARTING(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	-350,R0
    // asm: 	FLOAT	R0,R1
    // asm: 	STF	R1,*+AR7(CAMYOFF)
    // asm: 	LDI	4,R0		;direction to search in
    // asm: 	LDI	1,R1		;distance to start with
    // asm: 	LDI	8,R2		;Position to follow
    // asm: 	LDF	0,R3		;RADY OFFSET
    // asm: 	CALL	INITROAD_VIEW
    // asm: 	LDF	1,R2
    // asm: 	CALL	SET_LANE
    // asm: 	CALL	INIT_LANE
    // asm: 	CALL	ROAD_VIEW
    // asm: 	CALL	UPDATE_CAMERA
    // asm: 	CALL	RESCAN
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_STARTING", 0, 0);
    UNIMPL();
}

void INIT_WATCH(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(CUT_PAN)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(CAMERA_XYZR+3)		;Reset the offset
    // asm: 	LDI	*AR6++,R2				;READ the track ID from the VIEWLIST
    // asm: 	CALL	FIND_MAP
INIW1:
    // asm: 	LDI	*+AR4(3),R2		;Get ID
    // asm: 	CALL	FIND_TRACK		;MAKE DAMN SURE WE ARE ON A LOADED TRACK SECTION!!!
    // asm: 	CMPI	0,AR2
    // asm: 	BNE	INIW2
    // asm: 	SUBI	4,AR4
    // asm: 	BR	INIW1
INIW2:
    // asm: 	PUSH	AR4
    // asm: 	LDI	4,R2
    // asm: 	CALL	FIND_RACER
    // asm: 	STI	AR4,*+AR7(CAROBJ)
    // asm: 	STI	AR4,@PLYCAR
    // asm: 	LDI	*+AR4(OCARBLK),AR0
    // asm: 	STI	AR0,@PLYCBLK
    // asm: 	POP	AR4
    // asm: 	LDI	-300,R0
    // asm: 	FLOAT	R0,R1
    // asm: 	STF	R1,*+AR7(CAMYOFF)
    // asm: 	ADDI	*+AR4(Y),R0
    // asm: 	FLOAT	R0
    // asm: 	STF	R0,*+AR7(NEW_POSY)
    // asm: 	FLOAT	*+AR4(X),R0
    // asm: 	STF	R0,*+AR7(NEW_POSX)
    // asm: 	FLOAT	*+AR4(Z),R0
    // asm: 	STF	R0,*+AR7(NEW_POSZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	-4,AR2
    // asm: 	LDI	*+AR2(X),R0
    // asm: 	SUBI	*+AR4(X),R0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDI	*+AR2(Z),R1
    // asm: 	SUBI	*+AR4(Z),R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R0,R1,R2
    // asm: 	CALL	SQRT
    // asm: 	LDF	R0,R2
    // asm: 	LDI	*+AR2(Y),R3
    // asm: 	SUBI	*+AR4(Y),R3
    // asm: 	FLOAT	R3
    // asm: 	MPYF	1.8,R3
    // asm: 	CALL	ARCTANF
    // asm: 	CALL	NORM_VECTOR
    // asm: 	NEGF	R0
    // asm: 	ADDF	0.09,R0		;Offset for VIEWX
    // asm: 	STF	R0,*+AR7(NEW_RADX)
    // asm: 	FLOAT	-2500,R3
    // asm: 	CALL	DO_LANE_POS
    // asm: 	LDF	-100,R6			;SMALL number for a rad
    // asm: 	CALL	WATCH_VIEW
    // asm: 	LDF	*+AR7(NEW_RADY),R6	;Save initial value
    // asm: 	CALL	UPDATE_CAMERA
    // asm: 	PUSHF	R6
    // asm: 	CALL	RESCAN			;This hoses all RX registers
    // asm: 	POPF	R6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_WATCH", 0, 0);
    UNIMPL();
}

void INIT_REVERS_CUP(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	-350,R0
    // asm: 	FLOAT	R0,R1
    // asm: 	STF	R1,*+AR7(CAMYOFF)
    // asm: 	LDI	4,R0		;direction to search in
    // asm: 	LDI	1,R1		;distance to start with
    // asm: 	LDI	5,R2		;Position to follow
    // asm: 	LDF	PI,R3
    // asm: 	CALL	INITROAD_VIEW
    // asm: 	LDF	1,R2
    // asm: 	CALL	SET_LANE
    // asm: 	CALL	INIT_LANE
    // asm: 	CALL	REV_ROAD_VIEW
    // asm: 	CALL	UPDATE_CAMERA
    // asm: 	CALL	RESCAN
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_REVERS_CUP", 0, 0);
    UNIMPL();
}

void INIT_CATCHUP(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	-350,R0
    // asm: 	FLOAT	R0,R1
    // asm: 	STF	R1,*+AR7(CAMYOFF)
    // asm: 	LDI	4,R0		;direction to search in
    // asm: 	LDI	1,R1		;distance to start with
    // asm: 	LDI	5,R2		;Position to follow
    // asm: 	LDF	0,R3
    // asm: 	CALL	INITROAD_VIEW
    // asm: 	LDF	1,R2
    // asm: 	CALL	SET_LANE
    // asm: 	CALL	INIT_LANE
    // asm: 	CALL	ROAD_VIEW
    // asm: 	CALL	UPDATE_CAMERA
    // asm: 	CALL	RESCAN
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_CATCHUP", 0, 0);
    UNIMPL();
}

void INIT_LEAD(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	-300,R0
    // asm: 	FLOAT	R0,R1
    // asm: 	STF	R1,*+AR7(CAMYOFF)
    // asm: 	LDI	-4,R0
    // asm: 	LDI	1,R1
    // asm: 	LDI	0,R2
    // asm: 	LDF	PI,R3
    // asm: 	CALL	INITROAD_VIEW
    // asm: 	LDF	1,R2
    // asm: 	CALL	SET_LANE
    // asm: 	CALL	INIT_LANE
    // asm: 	CALL	LEAD_VIEW
    // asm: 	CALL	UPDATE_CAMERA
    // asm: 	CALL	RESCAN
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LEAD", 0, 0);
    UNIMPL();
}

void INITROAD_VIEW(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	STI	R0,*+AR7(CAMERA_XYZR)
    // asm: 	STI	R1,*+AR7(CAMERA_XYZR+1)
    // asm: 	STI	R2,*+AR7(CAMERA_XYZR+2)
    // asm: 	STF	R3,*+AR7(CAMERA_XYZR+3)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(CAMERA_ACCEL)
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(CUT_PAN)
    // asm: 	LDI	*+AR7(CAMERA_XYZR+2),R2		;Position to find
    // asm: 	CALL	FIND_RACER
    // asm: 	LDI	AR4,AR0
    // asm: 	STI	AR4,*+AR7(CAROBJ)
    // asm: 	STI	AR4,@PLYCAR
    // asm: 	LDI	*+AR4(OCARBLK),AR1
    // asm: 	STI	AR1,@PLYCBLK
    // asm: 	CALL	GET_MAP_CARBLK
    // asm: 	LDI	*+AR7(CAMERA_XYZR+1),R0
    // asm: 	MPYI	*+AR7(CAMERA_XYZR),R0
    // asm: 	SUBI	R0,AR4		;Start this many track sections behind first place
    // asm: 	LDI	@NOSWAP,R0
    // asm: 	CMPI	0,R0			;Don't check during init
    // asm: 	BNE	OK_FOUNDIT
TRY_AGAIN:
    // asm: 	LDI	*+AR4(3),R2		;Get ID
    // asm: 	CALL	FIND_TRACK
    // asm: 	CMPI	0,AR2
    // asm: 	BNE	OK_FOUNDIT
    // asm: 	ADDI	*+AR7(CAMERA_XYZR),AR4
    // asm: 	BR	TRY_AGAIN
OK_FOUNDIT:
    // asm: 	CALL	GET_ROAD_RADY
    // asm: 	STF	R0,*+AR7(NEW_RADY)
    // asm: 	FLOAT	*+AR4(X),R0
    // asm: 	STF	R0,*+AR7(NEW_POSX)
    // asm: 	FIX	*+AR7(CAMYOFF),R0
    // asm: 	ADDI	*+AR4(Y),R0
    // asm: 	FLOAT	R0
    // asm: 	STF	R0,*+AR7(NEW_POSY)
    // asm: 	FLOAT	*+AR4(Z),R0
    // asm: 	STF	R0,*+AR7(NEW_POSZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITROAD_VIEW", 0, 0);
    UNIMPL();
}

void WATCH_VIEW(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*+AR7(CAROBJ),AR0
    // asm: 	LDF	*+AR0(OPOSX),R2		;Get the direction of the car relative to
    // asm: 	SUBF	*+AR7(NEW_POSX),R2	;the direction the camera is moving in.
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	SUBF	*+AR7(NEW_POSZ),R3
    // asm: 	CALL	ARCTANF
    // asm: 	CALL	NORM_VECTOR
    // asm: 	SUBF	HALFPI,R0
    // asm: 	CALL	NORM_VECTOR
    // asm: 	NEGF	R0
    // asm: 	STF	R0,*+AR7(NEW_RADY)
    // asm: WV1
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	NEW_RADX,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	NEW_MATRIX,AR2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	LDI	@NFRAMES,R0
    // asm: 	SUBI	R0,AR5		;Will count frames in this case
    // asm: 	LDI	@ATTRWAVE,R0
    // asm: 	CMPI	2,R0		;preincremented will actually be GGPARK
    // asm: 	BNE	NO_WHOOSH
    // asm: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NO_WHOOSH
    // asm: 	LDF	R6,R0
    // asm: 	SUBF	*+AR7(NEW_RADY),R0
    // asm: 	CMPF	0.18,R0			;9deg	diff
    // asm: 	BLT	NO_WHOOSH
    // asm: 	LDF	-100,R6				;do only once
    // asm: 	SOND1	WHOOSH
NO_WHOOSH:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "WATCH_VIEW", 0, 0);
    UNIMPL();
}

void REV_ROAD_VIEW(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*+AR7(CAROBJ),AR0
    // asm: 	LDI	*+AR0(OCARBLK),AR1
    // asm: 	LDF	*+AR1(CARSPEED),R0
    // asm: 	MPYF	1.8,R0
    // asm: 	STF	R0,*+AR7(CAMERA_VEL)	;Set initial camera velocity
    // asm: 	LDF	0.6,R2
    // asm: 	CALL	SET_LANE
    // asm: 	CALL	MOVE_ROAD_CAMERA
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "REV_ROAD_VIEW", 0, 0);
    UNIMPL();
}

void ROAD_VIEW(void)
{
    // asm: 	CALL	FIND_CLOSEST_RACER
    // asm: 	CMPI	0,AR1
    // asm: 	LDIZ	0,AR5		;PASSED FIRST PLACE CHANGE MODES (AR5 = frame to switch)
    // asm: 	BEQ	RV1
    // asm: 	CMPI	*+AR7(CAROBJ),AR1
    // asm: 	BEQ	RV1A
    // asm: 	LDF	0,R2
    // asm: 	STF	R2,*+AR7(CAMERA_ACCEL)
    // asm: 	STI	AR1,*+AR7(CAROBJ)	;FOUND a NEW CAR
    // asm: 	STI	AR1,@PLYCAR
    // asm: 	LDI	*+AR1(OCARBLK),AR0
    // asm: 	STI	AR0,@PLYCBLK
    // asm: 	STF	R0,*+AR7(CARDIS)
RV1A:
    // asm: 	LDF	@ACCEL_RATE,R3
    // asm: 	LDF	*+AR7(CAMERA_ACCEL),R2
    // asm: 	LDF	*+AR7(CARDIS),R1
    // asm: 	MPYF	0.6,R1
    // asm: 	CMPF	R0,R1			;Are we half way there yet?
    // asm: 	BLT	RV1B
    // asm: 	NEGF	R3
RV1B:
    // asm: 	ADDF	R3,R2
    // asm: 	LDFLT	0,R2
    // asm: 	STF	R2,*+AR7(CAMERA_ACCEL)
    // asm: 	MPYF	R0,R2			;Factor in overall distance
    // asm: 	ADDF	1.6,R2
    // asm: 	LDI	*+AR1(OCARBLK),AR0
    // asm: 	LDF	*+AR0(CARSPEED),R0
    // asm: 	MPYF	R2,R0
    // asm: 	STF	R0,*+AR7(CAMERA_VEL)	;Set initial camera velocity
RV1:
    // asm: 	LDF	0.6,R2
    // asm: 	CALL	SET_LANE
    // asm: 	CALL	MOVE_ROAD_CAMERA
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ROAD_VIEW", 0, 0);
    UNIMPL();
}

void LEAD_VIEW(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	*+AR7(CAROBJ),AR0
    // asm: 	LDI	*+AR0(OCARBLK),AR1
    // asm: 	LDF	*+AR1(CARSPEED),R0
    // asm: 	MPYF	1.51,R0
    // asm: 	STF	R0,*+AR7(CAMERA_VEL)	;Set initial camera velocity
    // asm: 	LDF	0.6,R2
    // asm: 	CALL	SET_LANE
    // asm: 	CALL	MOVE_ROAD_CAMERA
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LEAD_VIEW", 0, 0);
    UNIMPL();
}

void MOVE_ROAD_CAMERA(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	@NOSWAP,R0	;If noswap is on definatly do it
    // asm: 	BNE	MRC1
    // asm: 	LDI	@SUSPEND_MODE,R0	;This stops the shimmer at the start of bevhills
    // asm: 	CMPI	SM_HALT,R0
    // asm: 	BEQ	MRCX
MRC1:
    // asm: 	CALL	GETCAMDIR
    // asm: 	LDI	@VECTORBI,AR2
    // asm: 	LDF	*+AR7(NEW_RADX),R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDF	*+AR7(NEW_RADZ),R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDF	*+AR7(NEW_RADY),R0
    // asm: 	SUBF	*+AR7(CAMERA_XYZR+3),R0	;LOOSE the Offset rad for viewing
    // asm: 	CALL	NORM_VECTOR
    // asm: 	NEGF	R0			;Move in oposite direction looking?
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	LDI	@VECTORBI,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	LDF	*+AR7(CAMERA_VEL),R0
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@VECTORBI,R3
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORBI,AR2
    // asm: 	LDF	*+AR2(X),R0
    // asm: 	ADDF	*+AR7(NEW_POSX),R0
    // asm: 	STF	R0,*+AR7(NEW_POSX)
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	ADDF	*+AR7(NEW_POSZ),R0
    // asm: 	STF	R0,*+AR7(NEW_POSZ)
    // asm: 	PUSH	AR4
    // asm: 	LDI	AR7,AR4
    // asm: 	ADDI	NEW_POSX,AR4
    // asm: 	CALL	CAMSCAN
    // asm: 	POP	AR4
    // asm: 	BNC	USE_SLOPE
    // asm: 	LDF	*+AR7(CAMYOFF),R1
    // asm: 	ADDF	R1,R0
    // asm: 	ADDF	*+AR7(NEW_POSY),R0
    // asm: 	STF	R0,*+AR7(NEW_POSY)
    // asm: 	BR	NO_SLOPE
USE_SLOPE:
    // ;This code should really never get used
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	4,AR2
    // asm: 	LDI	*+AR2(Z),R0
    // asm: 	SUBI	*+AR4(Z),R0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	R0,R0
    // asm: 	LDI	*+AR2(X),R1
    // asm: 	SUBI	*+AR4(X),R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF3	R0,R1,R2	;R= SQRT (X^2 + Z^2)
    // asm: 	CALL	SQRT
    // asm: 	LDF	R0,R1
    // asm: 	LDI	*+AR2(Y),R0
    // asm: 	SUBI	*+AR4(Y),R0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	1.05,R0		;FUDGE FACTOR
    // asm: 	CALL	DIV_F
    // asm: 	LDI	@VECTORAI,AR0
    // asm: 	LDF	*+AR0(Z),R1
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(NEW_POSY),R0
    // asm: 	STF	R0,*+AR7(NEW_POSY)
    // asm: 	LDI	-400,R1		;at 1000 above the road
    // asm: 	ADDI	*+AR4(Y),R1
    // asm: 	FLOAT	R1
    // asm: 	SUBF	*+AR7(NEW_POSY),R1
    // asm: 	MPYF	0.04,R1
    // asm: 	ADDF	*+AR7(NEW_POSY),R1
    // asm: 	STF	R1,*+AR7(NEW_POSY)
NO_SLOPE:
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	NEW_RADX,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	NEW_MATRIX,AR2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	CALL	NEXT_ROAD
MRCX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_ROAD_CAMERA", 0, 0);
    UNIMPL();
}

void NEXT_ROAD(void)
{
    // *---------------------------------------------------------------------------
    // * Updates AR4 to point to the road segment that the camera is on
    // * NOTE: uses Stealth mode
    // * INPUT	AR4 = segment on
    // * OUTPUT AR4 = segment on (updated)
    // *
    // asm: 	FLOAT	*+AR4(X),R0
    // asm: 	SUBF	*+AR7(NEW_POSX),R0
    // asm: 	MPYF	R0,R0
    // asm: 	FLOAT	*+AR4(Z),R1
    // asm: 	SUBF	*+AR7(NEW_POSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R1,R0
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	4,AR2
    // asm: 	FLOAT	*+AR2(X),R1
    // asm: 	SUBF	*+AR7(NEW_POSX),R1
    // asm: 	MPYF	R1,R1
    // asm: 	FLOAT	*+AR2(Z),R2
    // asm: 	SUBF	*+AR7(NEW_POSZ),R2
    // asm: 	MPYF	R2,R2
    // asm: 	ADDF	R2,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BLE	fcrx			;didn't move
    // asm: 	ADDI	4,AR4
    // ;	.if	DEBUG
    // ;	LDI	*+AR4(3),R2
    // ;	CALL	FIND_TRACK
    // ;	CMPI	0,AR2
    // ;	BNE	fcr1
    // ;	LOCKUP				;TRACK not loaded yet
    // ;fcr1
    // ;	.endif
    // asm: 	DEC	AR5			;AR5 is the number of road segments before swithching
    // asm: 	BR	NEXT_ROAD		;See if the road segment beyond is even closer
fcrx:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NEXT_ROAD", 0, 0);
    UNIMPL();
}

void GETCAMDIR(void)
{
    // *---------------------------------------------------------------------------
    // * Sets the cameras RADY based on the next track segments reletive position
    // * NOTE: uses stealth mode
    // * No I/O
    // *	Sets CAMERA_RADY = (CAMERA_XYZR+3)+ RAD to next track section
    // asm: 	LDF	*+AR7(CAMERA_VEL),R0	;Adjust the smothing factor to speed
    // asm: 	FLOAT	@NFRAMES,R1
    // asm: 	MPYF	R1,R0
    // asm: 	FLOAT	5000,R1
    // asm: 	CALL	DIV_F
    // asm: 	LDF	R0,R4
    // asm: 	CALL	GET_ROAD_RADY		;Calculate the offset for lane position
    // asm: 	NEGF	R0			;based on the roads direction
    // asm: 	LDF	R0,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDF	*+AR7(CAMERA_LANE),R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDI	@VECTORBI,R3
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	AR4,AR1
    // asm: 	ADDI	4*2,AR1
    // asm: 	LDI	@VECTORBI,AR2			;Factor in the desired location to be at
    // asm: 	FLOAT	*+AR1(Z),R2			;reletive to where we are
    // asm: 	ADDF	*+AR2(Z),R2
    // asm: 	SUBF	*+AR7(NEW_POSZ),R2
    // asm: 	FLOAT	*+AR1(X),R3
    // asm: 	ADDF	*+AR2(X),R3
    // asm: 	SUBF	*+AR7(NEW_POSX),R3
    // asm: 	CALL	ARCTANF
    // asm: 	CALL	NORM_VECTOR
    // asm: 	ADDF	*+AR7(CAMERA_XYZR+3),R0		;Offset for VIEW
    // asm: 	CALL	NORM_VECTOR
    // asm: 	SUBF	*+AR7(NEW_RADY),R0
    // asm: 	LDF	R4,R2
    // asm: 	CALL	SMOOTH_VECTOR
    // asm: 	ADDF	*+AR7(NEW_RADY),R0
    // asm: 	STF	R0,*+AR7(NEW_RADY)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(NEW_RADX)
    // asm: 	STF	R0,*+AR7(NEW_RADZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GETCAMDIR", 0, 0);
    UNIMPL();
}

void SET_LANE(void)
{
    // *----------------------------------------------------------------------------
    // *INPUT R2 = FL smoothing
    // asm: 	LDI	*+AR7(CAROBJ),AR0
    // asm: 	LDI	*+AR0(OPLINK),AR0
    // asm: 	FLOAT	1000,R1			;Offset to center of right lanes
    // asm: 	LDF	*+AR0(ROADOFFSET),R0
    // asm: 	BP	SL1
    // asm: 	NEGF	R1
SL1:
    // asm: 	SUBF	R1,R0
    // ;	MPYF	0.40,R0			;Move to just inside where the car is.
    // asm: 	MPYF	0.30,R0			;Move to just inside where the car is.
    // asm: 	ADDF	R1,R0
    // asm: 	CMPF	1.0,R2
    // asm: 	BEQ	SL2
    // asm: 	SUBF	*+AR7(CAMERA_LANE),R0
    // asm: 	MPYF	R2,R0
    // asm: 	ADDF	*+AR7(CAMERA_LANE),R0
    // asm: 	STF	R0,*+AR7(CAMERA_LANE)
SL2:
    // asm: 	STF	R0,*+AR7(CAMERA_LANE)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_LANE", 0, 0);
    UNIMPL();
}

void INIT_LANE(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDF	*+AR7(CAMERA_LANE),R3
    // asm: 	CALL	DO_LANE_POS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LANE", 0, 0);
    UNIMPL();
}

void DO_LANE_POS(void)
{
    // *---------------------------------------------------------------------------
    // *INPUT	R2 = RADIANS to offset for doing the lanes
    // *	R3 = Distance from center +/-
    // asm: 	PUSHF	R3
    // asm: 	LDI	AR4,AR1
    // asm: 	ADDI	4*2,AR1
    // asm: 	CALL	GET_ROAD_RADY
    // asm: 	NEGF	R0
    // asm: 	LDF	R0,R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR2(Y)
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	POPF	R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDI	@VECTORBI,R3
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	@VECTORBI,AR2
    // asm: 	LDF	*+AR2(X),R0
    // asm: 	ADDF	*+AR7(NEW_POSX),R0
    // asm: 	STF	R0,*+AR7(NEW_POSX)
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	ADDF	*+AR7(NEW_POSZ),R0
    // asm: 	STF	R0,*+AR7(NEW_POSZ)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DO_LANE_POS", 0, 0);
    UNIMPL();
}

void GET_ROAD_RADY(void)
{
    // *---------------------------------------------------------------------------
    // *INPUT	AR4 = POINTER to LEG_MAP where road is
    // *OUTPUT	R0 = DIRECTION of road
    // asm: 	PUSH	AR1
    // asm: 	PUSH	R2
    // asm: 	PUSHF	R2
    // asm: 	PUSH	R3
    // asm: 	PUSHF	R3
    // asm: 	LDI	AR4,AR1
    // asm: 	ADDI	4*2,AR1
    // asm: 	LDI	*+AR1(Z),R2
    // asm: 	SUBI	*+AR4(Z),R2
    // asm: 	FLOAT	R2
    // asm: 	LDI	*+AR1(X),R3	;Get the direction of the road
    // asm: 	SUBI	*+AR4(X),R3
    // asm: 	FLOAT	R3
    // asm: 	CALL	ARCTANF
    // asm: 	CALL	NORM_VECTOR
    // asm: 	POPF	R3
    // asm: 	POP	R3
    // asm: 	POPF	R2
    // asm: 	POP	R2
    // asm: 	POP	AR1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_ROAD_RADY", 0, 0);
    UNIMPL();
}

void FIND_RACER(void)
{
    // *---------------------------------------------------------------------------
    // *This algorithm is based on a bubble sort
    // *Find the RACER DRONE IN FIRST PLACE based on position in LEG_MAP
    // *INPUT R2 = Position to find
    // *OUTPUT AR4 = OBJ of the racer
    // *
    // *
    // asm: 	LDI	AR7,AR3			;clear the list
    // asm: 	ADDI	TEMP_DATA,AR3
    // asm: 	LDI	0,R0
    // asm: 	RPTS	9
    // asm: 	STI	R0,*AR3++
    // asm: 	LDI	AR7,AR3
    // asm: 	ADDI	TEMP_DATA,AR3
    // asm: 	LDI	0,IR0
    // asm: 	LDI	0,AR1
    // asm: 	LDI	7FFFh,R3
    // asm: 	MPYI	R3,R3			;BIG number
FR0:
    // asm: 	LDI	0,R4
    // asm: 	LDI	@CAR_LISTI,AR0		;Scan the list of car objects
    // asm: 	SUBI	OLINK3,AR0
FR1:
    // asm: 	LDI	*+AR0(OLINK3),AR0	;They are linked by OLINK3
    // asm: 	CMPI	0,AR0
    // asm: 	BEQ	FRX
    // asm: 	LDI	*+AR0(OID),R0			;Is this a racer?
    // asm: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RACER,R0
    // asm: 	BNE	FR1
    // asm: 	CALL	GET_MAP_CARBLK
    // asm: 	CMPI	AR4,R3			;Best position looking for
    // asm: 	BLT	FR1
    // asm: 	CMPI	AR4,R4			;Greatest entry in the LEG_MAP wins
    // asm: 	BGT	FR1
    // asm: 	LDI	IR0,IR1			;WHILE ((IR0>0) && (*+AR3(IR1++) != AR0))
FR2A:
    // asm: 	SUBI	1,IR1
    // asm: 	CMPI	0,IR1
    // asm: 	BN	FR2B
    // asm: FR2
    // asm: 	CMPI	*+AR3(IR1),AR0		;IF THIS CAR IS ALLREADY ON LIST, Then Get NEXT
    // asm: 	BEQ	FR1
    // asm: 	BR	FR2A
FR2B:
    // asm: 	LDI	AR4,R4
    // asm: 	LDI	AR0,AR1			;Set the pointer to the cars obj
    // asm: 	BU	FR1
FRX:
    // asm: 	STI	AR1,*+AR3(IR0)
    // asm: 	ADDI	1,IR0
    // asm: 	LDI	R4,R3
    // asm: 	SUBI	1,R2
    // asm: 	BP	FR0			;Find the next best
    // asm: 	CMPI	0,AR1
    // asm: 	BNE	FRXX
    // asm: 	LOCKUP				;No RACERS?
FRXX:
    // asm: 	LDI	AR1,AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_RACER", 0, 0);
    UNIMPL();
}

void FIND_CLOSEST_RACER(void)
{
    // *----------------------------------------------------------------------------
    // *This algorithm is based on a single pass bubble sort
    // *OUTPUT
    // *	R0	=	Distance to the closest Racer Drone
    // *	AR1	=	Pointer to the Closest Racer Drone in front of the camera
    // *	AR1	=	0, If no Racer Drones in front of the camera
    // *
    // asm: 	FLOAT	30000,R5
    // asm: 	MPYF	R5,R5
    // asm: 	MPYF	R5,R5
    // asm: 	MPYF	R5,R5
    // asm: 	MPYF	R5,R5		;BIG F'N NUMBER
    // asm: 	LDI	@CAR_LISTI,AR0		;Scane the linked list of cars
    // asm: 	SUBI	OLINK3,AR0
    // asm: 	LDI	0,AR1
FCR1:
    // asm: 	LDI	*+AR0(OLINK3),AR0
    // asm: 	CMPI	0,AR0
    // asm: 	BEQ	FCRX
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RACER,R0	;Racer drones only
    // asm: 	BNE	FCR1
    // asm: 	LDF	*+AR0(OPOSZ),R2
    // asm: 	SUBF	*+AR7(NEW_POSZ),R2
    // asm: 	LDF	*+AR0(OPOSX),R3		;Get the direction of the car relative to
    // asm: 	SUBF	*+AR7(NEW_POSX),R3	;the direction the camera is moving in.
    // asm: 	CALL	ARCTANF
    // asm: 	CALL	NORM_VECTOR
    // asm: 	SUBF	*+AR7(NEW_RADY),R0	;DEG from camera
    // asm: 	CALL	NORM_VECTOR
    // asm: 	SUBF	*+AR7(CAMERA_XYZR+3),R0	;LOOSE the Offset rad for viewing
    // asm: 	CALL	NORM_VECTOR
    // asm: 	ABSF	R0
    // asm: 	CMPF	0.78,R0			;Within +/- 45deg?
    // asm: 	BGT	FCR1			;Nop this car is beside us or behind us
    // asm: 	LDF	*+AR0(OPOSX),R0		;Now compute the distance to the car
    // asm: 	SUBF	*+AR7(NEW_POSX),R0	;D = SQRT((X2-X1)^2+(Y2-Y1)^2)
    // asm: 	MPYF	R0,R0
    // asm: 	LDF	*+AR0(OPOSZ),R1
    // asm: 	SUBF	*+AR7(NEW_POSZ),R1
    // asm: 	MPYF	R1,R1
    // asm: 	ADDF	R0,R1,R2
    // asm: 	CALL	SQRT
    // asm: 	CMPF	R0,R5
    // asm: 	LDFGT	R0,R5
    // asm: 	LDIGT	AR0,AR1
    // asm: 	BU	FCR1
FCRX:
    // asm: 	LDF	R5,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_CLOSEST_RACER", 0, 0);
    UNIMPL();
}

void FIND_TRACK(void)
{
    // *---------------------------------------------------------------------------
    // *
    // *GET OBJECT POINTER TO ROAD SEG ID IN R2
    // *INPUTS  R2=OUSR1
    // *OUPTUTS AR2=POINTER TO OBJECT
    // *	  AR2=0 not found
    // asm: 	LDI	@DYNALIST_TRUEBEGIN,AR2
FDL:
    // asm: 	LDI	*+AR2(OUSR1),R0
    // asm: 	CMPI	R0,R2
    // asm: 	BZ	FDLX
    // asm: 	LDI	*+AR2(OLINK4),AR2
    // asm: 	CMPI	0,AR2			;CHECK END OF THE LINE
    // asm: 	BNZ	FDL
FDLX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_TRACK", 0, 0);
    UNIMPL();
}

void GET_MAP_CARBLK(void)
{
    // *---------------------------------------------------------------------------
    // *
    // *GET POINTER TO MAP_LEG, FOR THE CAR IN AR4
    // *INPUTS  AR0=POINTER TO CARBLK
    // *OUPTUTS AR4=POINTER TO place in LEG_MAP
    // *SETS CAROBJ IF GET_MAP_SEGMENT is called
    // asm: 	PUSH	R2
    // asm: 	LDI	*+AR0(OPLINK),AR2
    // asm: 	LDI	*+AR2(DELTA_SPTR),AR4
    // asm: 	LDI	*+AR2(STEALTHMODE),R0	;ARE YOU A STEALTHY BITCH?
    // asm: 	BNZ	GMSX
    // asm: 	LDI	*+AR0(OCARBLK),AR4
    // asm: 	LDI	*+AR4(CARTRAK),AR4	;Get the track piece the car is on
    // asm: 	LDI	*+AR4(OUSR1),R2
    // asm: 	CALL	FIND_MAP
GMSX:
    // asm: 	POP	R2
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "GET_MAP_CARBLK", 0, 0);
    UNIMPL();
}

void FIND_MAP(void)
{
    // *---------------------------------------------------------------------------
    // *
    // *GET POINTER TO MAP_LEG, ID IN R2
    // *INPUTS  R2=OUSR1
    // *OUPTUTS AR4=POINTER TO place in LEG_MAP
    // asm: 	LDI	@LEG_MAPI,AR4		;FIND THE POSITION IN THE MAP
    // asm: 	ADDI	3,AR4				;OFFSET TO ID
FIND_LP:
    // asm: 	LDI	*AR4++(4),R0
    // asm: 	CMPI	R2,R0
    // asm: 	BLT	FIND_LP
    // asm: FINDX
    // asm: 	SUBI	7,AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_MAP", 0, 0);
    UNIMPL();
}

void INITVIEW1_VIEW(void)
{
    // asm: 	LDI	0,R1
    // asm: 	STI	R0,*+AR7(CAMERA_VIEW)
    // asm: 	LDI	@PLYCAR,AR4
    // asm: 	LDF	0,R0
    // asm: 	FLOAT	-150,R1
    // asm: 	FLOAT	0,R2
    // asm: 	LDF	0,R3
    // asm: 	BR	SET_SMOOTH_VIEW
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITVIEW1_VIEW", 0, 0);
    UNIMPL();
}

void CUT_TO_VIEW2(void)
{
    // asm: 	LDI	1,R0			;Don't smooth while spinning
    // asm: 	STI	R0,*+AR7(CUT_PAN)
    // asm: 	LDI	4,R2		;FOLOW 3rd place
    // asm: 	CALL	FIND_RACER
    // asm: 	STI	AR4,@PLYCAR
    // asm: 	LDI	*+AR4(OCARBLK),AR0
    // asm: 	STI	AR0,@PLYCBLK
    // asm: 	CALL	INITVIEW2_VIEW
    // asm: 	CALL	UPDATE_CAMERA
    // asm: 	CALL	RESCAN
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CUT_TO_VIEW2", 0, 0);
    UNIMPL();
}

void INITVIEW2_VIEW(void)
{
    // asm: 	LDI	@PLYCAR,AR4
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(CAMERA_VIEW)
    // asm: 	LDF	0,R0
    // asm: 	FLOAT	-400,R1
    // asm: 	FLOAT	-2200,R2
    // asm: 	LDF	0,R3
    // asm: 	BR	SET_SMOOTH_VIEW
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITVIEW2_VIEW", 0, 0);
    UNIMPL();
}

void INITVIEW3_VIEW(void)
{
    // asm: 	LDI	BUT_VIEW3,R1
    // asm: 	STI	R0,*+AR7(CAMERA_VIEW)
    // asm: 	LDF	0,R0
    // asm: 	FLOAT	-900,R1
    // asm: 	FLOAT	-3840,R2
    // asm: 	LDF	0,R3
    // asm: 	BR	SET_SMOOTH_VIEW
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INITVIEW3_VIEW", 0, 0);
    UNIMPL();
}

void FACE_FRONT(void)
{
    // asm: 	PUSH	AR5
    // asm: 	LDI	*+AR4(OCARBLK),AR5
    // asm: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF CAR IS DISPLAYED
    // asm: 	TSTB	O_LIST_M,R0
    // asm: 	BNZ	SECOND_PERSON
    // asm: FIRST_PERSON				;NO, Then in first person
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@CAMVIEW
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	NEW_MATRIX,AR2
    // asm: 	LDI	AR4,R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	CALL	CPYIMAT			;Copy the cars matrix into NEW_MATRIX
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR5(CARROT)	;NO OVERROTATION IN CORNER
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(NEW_RADY)		;NOTE this is not used for first person
    // asm: 	LDF	*+AR5(CARXLEAN),R0
    // asm: 	MPYF	-0.7,R0			;NEGATE AND SCALE
    // asm: 	SUBF	*+AR7(NEW_RADX),R0
    // asm: 	LDF	0.20,R2
    // asm: 	CALL	SMOOTH_VECTOR
    // asm: 	ADDF	*+AR7(NEW_RADX),R0
    // asm: 	STF	R0,*+AR7(NEW_RADX)		;NOTE this is not used for first person
    // asm: 	LDF	*+AR5(CARZLEAN),R0  	;GET Z LEAN FACTOR
    // asm: 	MPYF	-0.5,R0			;NEGATE AND SCALE
    // asm: 	STF	R0,*+AR7(NEW_RADZ)
    // asm: 	LDI	@MATRIXCI,AR2
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	NEW_RADX,R2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	NEW_MATRIX,AR2
    // asm: 	LDI	AR2,R3
    // asm: 	LDI	@MATRIXCI,R2
    // asm: 	CALL	CONCATMAT
    // asm: 	NEGF	*+AR5(CARYROT),R0	;SAVE this for switching out of first person
    // asm: 	STF	R0,*+AR7(NEW_RADY)		;NOTE this is not used for first person
    // asm: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm: 	BZ	FFX
    // asm: 	LDI	1,R0			;Don't smooth while spinning
    // asm: 	STI	R0,*+AR7(CUT_PAN)
    // asm: 	BR	FFX
SECOND_PERSON:
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@CAMVIEW
    // asm: 	LDI	*+AR5(CAR_SPIN),R0	;SPINNING?
    // asm: 	BNZ	FFX			;YES, DONT mess WITH CAMERA ANGLE
    // asm: 	LDF	*+AR4(OVELX),R3
    // asm: 	LDF	*+AR4(OVELZ),R2
    // asm: 	ADDF3	R3,R2,R1
    // asm: 	CMPF	0,R1
    // asm: 	BNE	FFX1
    // asm: 	NEGF	*+AR5(CARYROT),R0	;If stopped don't use the velocity
    // asm: 	BR	FFX2
FFX1:
    // asm: 	CALL	ARCTANF
FFX2:
    // asm: 	CALL	NORM_VECTOR
    // asm: 	ADDF	*+AR7(CAMERA_XYZR+3),R0
    // asm: 	LDI	*+AR7(CUT_PAN),R2
    // asm: 	CMPI	0,R2
    // asm: 	BNE	FFX3				;NO smoothing in a cut
    // asm: 	SUBF	*+AR7(NEW_RADY),R0
    // asm: 	LDF	0.10,R2
    // ;	LDF	0.20,R2
    // asm: 	CALL	SMOOTH_VECTOR
    // asm: 	ADDF	*+AR7(NEW_RADY),R0
FFX3:
    // asm: 	STF	R0,*+AR7(NEW_RADY)
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR7(NEW_RADX)
    // asm: 	STF	R0,*+AR7(NEW_RADZ)
    // asm: FFX4
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	NEW_RADX,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	NEW_MATRIX,AR2
    // asm: 	CALL	FIND_MATRIX
    // asm: 	LDI	AR7,AR0
    // asm: 	ADDI	NEW_POSX,AR0
    // asm: 	CALL	CAMYADJ			;MAKE sure the camera doesn't go below the road
FFX:
    // asm: 	POP	AR5
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FACE_FRONT", 0, 0);
    UNIMPL();
}

void SMOOTH_VECTOR(void)
{
    // *----------------------------------------------------------------------------
    // *Does averaging
    // *INPUT
    // *		R2 = average	0.20
    // *		R0 = difference between the two angles
    // *OUTPUT
    // *		R0 = SMOOTHED difference
    // asm: 	CALL	NORM_VECTOR
    // asm: 	LDI	*+AR5(CAR_SPIN),R3	;SPINNING?
    // asm: 	CMPI	1,R3
    // asm: 	BEQ	SV1			;Yes then don't smooth
    // asm: 	LDF	R0,R3		;Save of the non smoothed value
    // asm: 	MPYF	R2,R0	    		;ANGLE SMOOTHING
    // asm: 	ABSF	R3,R2
    // asm: 	CMPF	0.78,R2 	;Is this within 45 degrees?
    // asm: 	BLT	SVX
    // asm: 	LDF	R3,R0		;NO, Then don't smooth the pig (PROBABLY JUST CHANGED CAMERA ANGLE)
SV1:
    // asm: 	LDI	1,R3
    // asm: 	STI	R3,*+AR7(CUT_PAN)	;This is a cut, Don't smooth the matrix
SVX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SMOOTH_VECTOR", 0, 0);
    UNIMPL();
}

void SERIOUSLY_NORMALIZE(void)
{
    // asm: 	ABSF	R0,R1
    // asm: 	CMPF	@TWOPII,R1
    // asm: 	BLT	NORM_VECTOR
    // asm: 	LDF	@TWOPII,R1
    // asm: 	PUSHF	R0
    // asm: 	CALL	DIV_F
    // asm: 	FIX	R0,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	@TWOPII,R1		;MOD 6.28
    // asm: 	POPF	R0
    // asm: 	SUBF	R1,R0
NORM_VECTOR:
    // asm: 	LDF	0,R1 			;NORMALIZE DIFFERENCE
    // asm: 	CMPF	3.14,R0
    // asm: 	LDFGT	-6.28,R1
    // asm: 	CMPF	-3.14,R0
    // asm: 	LDFLT	6.28,R1
    // asm: 	ADDF	R1,R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SERIOUSLY_NORMALIZE", 0, 0);
    UNIMPL();
}

void SET_SMOOTH_VIEW(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDI	0,AR5			;Set AR5 for init
    // asm: 	STF	R0,*+AR7(NEWCAMERA_XYZR)
    // asm: 	STF	R1,*+AR7(NEWCAMERA_XYZR+1)
    // asm: 	STF	R2,*+AR7(NEWCAMERA_XYZR+2)
    // asm: 	STF	R3,*+AR7(NEWCAMERA_XYZR+3)
    // asm: 	LDF	0,R4
    // asm: 	STF	R4,*+AR7(ZOOMVEL)
    // asm: 	LDI	*+AR7(CUT_PAN),R4
    // asm: 	BEQ	SMOOTH_VIEW
    // ;DO A CUT
    // asm: 	STF	R0,*+AR7(CAMERA_XYZR)
    // asm: 	STF	R1,*+AR7(CAMERA_XYZR+1)
    // asm: 	STF	R2,*+AR7(CAMERA_XYZR+2)
    // asm: 	STF	R3,*+AR7(CAMERA_XYZR+3)
SMOOTH_VIEW:
    // asm: 	CALL	ZOOM_CAMERA
    // asm: 	CALL	FACE_FRONT
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	CAMERA_XYZR,AR2
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	NEW_MATRIX,R2
    // asm: 	LDI	AR7,R3
    // asm: 	ADDI	NEW_POSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	SUBF	*+AR7(NEW_POSX),R0
    // asm: 	STF	R0,*+AR7(NEW_POSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR7(NEW_POSY),R0
    // asm: 	STF	R0,*+AR7(NEW_POSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR7(NEW_POSZ),R0
    // asm: 	STF	R0,*+AR7(NEW_POSZ)
    // asm: 	LDI	*+AR4(OCARBLK),AR0
    // asm: 	LDI	*+AR0(CARTRAK),AR0
    // asm: 	LDI	*+AR0(OUSR1),R1
    // asm: 	CMPI	AR5,R1
    // asm: 	RETSLT
    // asm: 	LDI	0,AR5
    // ;Put car back if it was removed
    // asm: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY ON LIST
    // asm: 	TSTB	O_LIST_M,R0
    // asm: 	BNZ	SMOOTH_VIEWX			;YES, DONT INSERT
    // asm: 	STI	AR4,*+AR7(OBJINS)
    // ;	LDI	AR4,AR2
    // ;	CALL	OBJ_INSERT			;INSERT PLAYER OBJECT
SMOOTH_VIEWX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SET_SMOOTH_VIEW", 0, 0);
    UNIMPL();
}

void ZOOM_CAMERA(void)
{
    // asm: 	LDI	*+AR7(CUT_PAN),R0
    // asm: 	BNE	ZOOMX
    // asm: 	LDF	*+AR7(ZOOMVEL),R0
    // asm: 	ADDF	@ZOOMACCEL,R0
    // asm: 	CMPF	0.2,R0
    // asm: 	LDFGT	0.2,R0
    // asm: 	STF	R0,*+AR7(ZOOMVEL)
    // asm: 	LDI	AR7,AR0
    // asm: 	ADDI	CAMERA_XYZR,AR0
    // asm: 	LDI	AR7,AR1
    // asm: 	ADDI	NEWCAMERA_XYZR,AR1
    // asm: 	LDI	3,RC
    // asm: 	RPTB	ZOOMLP
    // asm: 	LDF	*AR1++,R2
    // asm: 	SUBF	*AR0,R2
    // asm: 	MPYF	R0,R2
    // asm: 	ADDF	*AR0,R2
ZOOMLP:
    // asm: STF	R2,*AR0++
    // asm: 	LDI	*+AR7(CAMERA_VIEW),R0
    // asm: 	CMPI	0,R0
    // asm: 	BNE	ZOOM1
    // asm: 	ABSF	*+AR7(CAMERA_XYZR+2),R0
    // asm: 	CMPF	100,R0
    // asm: 	BGT	ZOOMX
    // asm: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY OFF LIST
    // asm: 	TSTB	O_LIST_M,R0
    // asm: 	BZ	ZOOMX			;YES, DONT DELETE
    // asm: 	LDI	AR4,AR2		;IN first person
    // asm: 	CALL	OBJ_PULL
    // asm: 	BR	ZOOMX
ZOOM1:
    // asm: 	ABSF	*+AR7(CAMERA_XYZR+2),R0
    // asm: 	CMPF	100,R0
    // asm: 	BLT	ZOOMX
    // asm: 	LDI	*+AR4(OFLAGS),R0	;CHECK IF ALREADY ON LIST
    // asm: 	TSTB	O_LIST_M,R0
    // asm: 	BNZ	ZOOMX			;YES, DONT INSERT
    // asm: 	LDI	AR4,AR2
    // asm: 	CALL	OBJ_INSERT			;INSERT PLAYER OBJECT
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,*+AR7(CUT_PAN)
ZOOMX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZOOM_CAMERA", 0, 0);
    UNIMPL();
}

void UPDATE_CAMERA(void)
{
    // *----------------------------------------------------------------------------
    // *----------------------------------------------------------------------------
    // * UPDATE_CAMERA
    // *	INPUT	NEW_POSx/y/z,NEW_RADx/y/z,NEW_MATRIX
    // *	OUTPUT	_CAMERAPOS,_CAMERAMATRIX,_CAMERARAD
    // *
    // asm: 	LDI	@_CAMERAPOSI,AR0
    // asm: 	LDF	*+AR7(NEW_POSX),R0
    // asm: 	STF	R0,*AR0
    // asm: 	LDF	*+AR7(NEW_POSY),R0
    // asm: 	STF	R0,*+AR0(Y)
    // asm: 	LDF	*+AR7(NEW_POSZ),R0
    // asm: 	STF	R0,*+AR0(Z)
    // asm: 	LDP	@_CAMERARAD
    // asm: 	LDF	*+AR7(NEW_RADX),R0
    // asm: 	STF	R0,@_CAMERARAD
    // asm: 	LDF	*+AR7(NEW_RADY),R0
    // asm: 	STF	R0,@_CAMERARAD+Y
    // asm: 	LDF	*+AR7(NEW_RADZ),R0
    // asm: 	STF	R0,@_CAMERARAD+Z
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR0
    // asm: 	LDI	AR7,AR1
    // asm: 	ADDI	NEW_MATRIX,AR1
    // asm: 	LDI	*+AR7(CUT_PAN),R0
    // asm: 	BEQ	SMOOTH_MATRIX
    // asm: 	LDI	8,RC
    // asm: 	RPTB	loopA
    // asm: 	LDF	*AR1++,R1
loopA:
    // asm: STF	R1,*AR0++
    // asm: 	BR	UPCAMX
SMOOTH_MATRIX:
    // asm: 	LDI	8,RC
    // asm: 	RPTB	loop
    // asm: 	LDF	*AR0,R0
    // asm: 	MPYF	0.20,R0
    // asm: 	LDF	*AR1++,R1
    // asm: 	MPYF	0.80,R1
    // asm: 	ADDF	R0,R1
loop:
    // asm: STF	R1,*AR0++
UPCAMX:
    // asm: 	CALL	CAMERA_HORIZON_PROJECTION
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR7(CUT_PAN)		;Ok to smooth after this
    // asm: 	RETS
    // ;*----------------------------------------------------------------------------
    // ;* MOVE_CAMERA
    // ;*
    // ;*	INPUT CAMERA_SPEED,CAMERA_VEC
    // ;*
    // ;
    // ;MOVE_CAMERA
    // ;	LDF	@NEW_CAMERA+CAMERA_SPEED,R0
    // ;	BEQ	no_movment
    // ;	LDI	@NEW_CAMERAI,R2
    // ;	ADDI	CAMERA_VECX,R2
    // ;	LDI	@MATRIXAI,AR2
    // ;	CALL	FIND_MATRIX
    // ;
    // ;	LDF	0,R0
    // ;	STF	R0,@VECTORA
    // ;	STF	R0,@VECTORA+1
    // ;	LDF	@NEW_CAMERA+CAMERA_SPEED,R0
    // ;	STF	R0,@VECTORA+2
    // ;
    // ;	LDI	@MATRIXAI,R2
    // ;	LDI	@VECTORAI,AR2
    // ;	LDI	AR2,R3
    // ;	CALL	MATRIX_MUL
    // ;
    // ;	LDF	@VECTORA,R0
    // ;	ADDF	@NEW_CAMERA+CAMERAX,R0
    // ;	STF	R0,@NEW_CAMERA+CAMERAX
    // ;
    // ;	LDF	@VECTORA+1,R0
    // ;	ADDF	@NEW_CAMERA+CAMERAY,R0
    // ;	STF	R0,@NEW_CAMERA+CAMERAY
    // ;
    // ;	LDF	@VECTORA+2,R0
    // ;	ADDF	@NEW_CAMERA+CAMERAZ,R0
    // ;	STF	R0,@NEW_CAMERA+CAMERAZ
    // ;no_movment
    // ;	RETS
    // ;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UPDATE_CAMERA", 0, 0);
    UNIMPL();
}

void CAMERA_HORIZON_PROJECTION(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	LDF	*+AR7(NEW_RADX),R2
    // asm: 	CALL	_SINE
    // asm: 	FLOAT	512,R1
    // asm: 	MPYF	R1,R0
    // asm: 	ADDF	*+AR7(CAMERA_INFIN),R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	RETS
    // asm: ATTR_WAVETAB
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CAMERA_HORIZON_PROJECTION", 0, 0);
    UNIMPL();
}

void INIT_ATTR_LEG(void)
{
    // asm: 	LDI	@ATTRWAVE,R0
    // asm: 	BNE	NO_MUSIC
    // 	;Wed Mar 8 11:02:10 1995
    // 	;
    // 	;
    // 	;if COMMP ON  && SLAVE then no music
    // 	;if COMMP ON && MASTER then send_attrsnd
    // 	;if COMMP OFF do regular
    // 	;
    // 	;
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	DIP_COMMP,R0
    // asm: 	BNZ	DOTEST
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BNZ	NO_MUSIC
    // asm: 	CALL	SEND_ATTRSND
DOTEST:
    // 	;
    // 	;
    // 	;
    // asm: 	READADJ	ADJ_ATTRACT_MODE_SOUND
    // asm: 	CMPI	0,R0
    // asm: 	BEQ	NO_MUSIC
    // asm: 	SOND1	ATTR_THEME
NO_MUSIC:
    // asm: 	LDI	MATTR|MINFIN,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDL	press_PALETTES,AR2	;make sure these are gone
    // asm: 	CALL	dealloc_section
    // asm: 	LDI	0,R0			;NO CHALLENGE RACES IN ATTRACT MODE
    // asm: 	STI	R0,@CHALLENGE_RACE
    // asm: 	LDI	@ATTR_WAVETABI,AR1
    // asm: 	ADDI	@ATTRWAVE,AR1
    // asm: 	LDI	*AR1++,R0
    // asm: 	STI	R0,@NEXT_STARTUP
    // asm: 	LDI	*AR1,R1
    // asm: 	STI	R1,@BONUS_WAVE
    // asm: 	LDI	@BONUS_WAVE,AR0
    // asm: 	ADDI	@FULLSETUP_TABLEI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	LDF	@INFIN_CORRECT,R0
    // asm: 	STF	R0,*+AR7(CAMERA_INFIN)
    // asm: 	CALL	OBJ_INIT
    // asm: 	CALL	INIT_DRONES	;init DRONE tracker system
    // asm: 	CALL	DYNAOBJ_INIT	;init DYNAMIC OBJECTS
    // asm: 	CALL	CARB_INIT	;init CAR BLOCKS
    // asm: 	CALL	INIT_RDDEBRIS	;initialize ROAD DEBRIS list(s)
    // 	;
    // 	;RE INITIALIZE TRACK...
    // 	;
    // asm: 	LDI	@BONUS_WAVE,AR0
    // asm: 	ADDI	@BONUS_POSTLAUNCHI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	LDI	@NEXT_STARTUP,R0
    // asm: 	STI	R0,@STARTSECTION
    // asm: 	CALL	BGD_INIT
    // asm: 	PUSH	AR7
    // asm: 	LDI	UTIL_C|BACKGRND_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_FIND
    // asm: 	LDI	AR0,AR7
    // asm: 	LDI	3,R0
    // asm: 	STI	R0,*+AR7(PTIME)
    // asm: 	POP	AR7
    // asm: 	LDI	*+AR7(PDATA),R0
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@OHIGH_PRIORITY
    // asm: 	CREATE	WAVEFLAG,UTIL_C|MONKEY_T
    // asm: 	CALL	ATTR_INIT_GAMELEG
    // asm: 	LDI	MAX_DRONES,R0
    // asm: 	STI	R0,@DD_MAX_DRONES
    // asm: 	LDI	@ATTRWAVE,R0
    // asm: 	ADDI	2,R0
    // asm: 	CMPI	ATTR_WAVETAB_LEN-1,R0
    // asm: 	LDIGT	0,R0
    // asm: 	STI	R0,@ATTRWAVE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_ATTR_LEG", 0, 0);
    UNIMPL();
}

void ATTR_INIT_GAMELEG(void)
{
    // *----------------------------------------------------------------------------
    // asm: 	CREATE	RHO_DISPATCHER,SPAWNER_C|TRAFFIC_T
    // asm: 	CREATE	SIGMA_DISPATCHER,SPAWNER_C|TRAFFIC_T
    // asm: 	CREATE	CPOINT_LIGHT,SPAWNER_C|COLORCYC_T
    // asm: 	CREATE	POSITION_FINDER,SPAWNER_C|TRAFFIC_T
    // asm: 	LDI	SM_HALT,R0
    // asm: 	STI	R0,@SUSPEND_MODE
    // asm: 	LDI	0,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	1,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	2,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	3,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	4,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	STI	AR0,@PLYCAR
    // asm: 	LDI	5,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	6,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	7,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	LDI	9,R4
    // asm: 	CREATE	RACER_DRONE,DRONE_C
    // asm: 	RETS
    // ;*----------------------------------------------------------------------------
    // ;
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ATTR_INIT_GAMELEG", 0, 0);
    UNIMPL();
}

void LOAD_ATTR_LEG(void)
{
    // asm: 	LDI	@ATTRWAVE,AR0
    // asm: 	ADDI	@ATTR_WAVETABI,AR0
    // asm: 	LDI	*+AR0(1),AR0
#if DEBUG
    // asm: 	CMPI	0,AR0
    // asm: 	LDILT	0,AR0
    // asm: 	CMPI	13,AR0
    // asm: 	LDIGT	13,AR0
#endif
    // asm: 	ADDI	@LOADSECTION_TABLEI,AR0
    // asm: 	LDI	*AR0,R0
    // asm: 	CALLU	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "LOAD_ATTR_LEG", 0, 0);
    UNIMPL();
}

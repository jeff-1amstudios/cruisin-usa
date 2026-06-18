#include "../core/cpu.h"
#include "../core/machine.h"
#include "mproc.h"
#include "macs.h"
#include "obj.h"
#include "objects.h"
#include "globals.h"
#include "sys.h"
#include "text.h"
#include "vunit.h"
#include "cmos.h"
#include "sndtab.h"
#include "pall.h"
#include "sysid.h"
#include "hstdp.h"

/*
 * Source module: asm/HSTDP.ASM
 */

void ENTER_INITIALS(void);
void PEDALWT(void);
void ENTERTEXT(void);
void CREATE_ENTERTEXT(void);
void MAKE_CENTER(void);
void HSTD_TIMER(void);
void INSERT_INITS(void);
void INTO_TABLE_P(void);
void CHECK_FIRST_TIME(void);
void CALC_TOTAL_ELAPSED(void);
void PRESS_CODE_ENTRY(void);
void INIT_PRESS_OBJECTS(void);
void MAKE_NEW_MARQ(void);
void MOVE_PRESSB(void);
void MOVE_BIGLET_PRESS(void);
void POS_SCROLLB(void);
void ADJUST_ROLLERS(void);
void FIND_ROLLER(void);
void SPIN_ROLLERS(void);
void MOVE_PLAYERS_LETTERS(void);
void MAKE_PLAYERS_PLATE(void);
void INI_PLAYERS_BOLTS(void);
void FLY_BOLT(void);
void OBJ_FIND(void);
void OBJ_FIND_NEXT(void);
void OBJ_GFIND(void);
void OBJ_GFIND_NEXT(void);
void OBJ_TAG(void);
void OBJ_TAGALL(void);
void OBJ_MOVY_GROUP(void);
void OBJ_DEL_GROUP(void);
void FIND_ALL_PLATES(void);
void FIND_PLATES(void);
void MAKE_NUMBERS(void);
void MAKE_TIME(void);
void FORMAT_NUM(void);
void CREATE_LETTERS(void);
void ASCII_TO_OBJ(void);
void PRINT3D(void);
void DISPLAY_HIGH_SCORES(void);
void DISPLAY_HS(void);
void FLASH_LETTERS_PROC(void);
void FLASH_LETTERS(void);
void DELETE_PRESS_OBJECTS(void);
void FIX_PLATES(void);
void FLY_PLATES(void);
void DISPLAY_HSTEXT(void);
void ENTER_HSTEXT(void);
void INIT_LOGO(void);
void RBMATTR_CHECK(void);

#define PLATE_LETTERSI PLATE_LETTERS
#define THREED_LETTERSI THREED_LETTERS
#define EIPI EIP
#define SCROLLBTABI SCROLLBTAB
#define DELISTI DELIST
#define NUMTABI NUMTAB
#define FLASH_PALSI FLASH_PALS
#define scroll_whiteI scroll_white

extern const char DT1[];
extern const char MP1[];
extern const char MP2[];

extern int NUMTAB[];
extern int THREED_LETTERS[];
extern int PLATE_LETTERS[];
extern const char EIP[];
extern int SCROLLBTAB[];
extern int LONGEST_TIME;
extern int FLASH_PALS[];
extern int DELIST[];
extern int RADIO_HS_SHADOW;
extern int RADIO_HS_SWITCH;

/*
*----------------------------------------------------------------------------
*HSTD PROCESSES
*
*COPYRIGHT (C) 1994  BY TV GAMES, INC.
*ALL RIGHTS RESERVED
*
*/

#define FLASH_ON 1 //TURN THIS OFF TO LOOSE FLASHING
// ;These are for the license plate and pressing of the plate

#define PRESS_DIAM 271 //NOTE: The plate is on the bottom surface
#define PRESS_RADX 1.5708
#define ROLLER_ZOFF 620
#define PRESS_STARTZ (-1300)
#define PRESS_LASTZ (-2100) //-1900
#define PRESS_TRAVELZ (PRESS_LASTZ-PRESS_STARTZ)
// ;PRESS_STARTY	.set	-30
#define PRESS_STARTY (-45)
#define PRESS_LASTY (-100)
#define PRESS_TRAVELY (PRESS_LASTY-PRESS_STARTY)
#define HIGH_SCORE_GROUP 0x200
/* asm: NUMTAB		.word	dzero,done,dtwo,dthree,dfour,dfive,dsix,dseven,deight,dnine */
/* asm: 	 */
int NUMTAB[] = {
    dzero, done, dtwo, dthree, dfour, dfive, dsix, dseven, deight, dnine,
};
/* asm: THREED_LETTERS	.word	ma,mb,mc,md,me,mf,mg,mh,mi,mj,mk,ml,mm,mn,mo,mp,mq,mr */
/* asm: 	.word	ms,mt,mu,mv,mw,mx,my,mz */
int THREED_LETTERS[] = {
    ma, mb, mc, md, me, mf, mg, mh, mi, mj, mk, ml, mm, mn, mo, mp, mq, mr,
    ms, mt, mu, mv, mw, mx, my, mz,
};
#define THREED_END ($-THREED_LETTERS)
#define RUB ('Z'-'A'+1)
/* asm: PLATE_LETTERS	.word	pa,pb,pc,pd,pe,pf,pg,ph,pi,pj,pk,pl,pm,pn,po,pp,pq,pr */
/* asm: 	.word	ps,pt,pu,pv,pw,px,py,pz,arrow,p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,pend */
int PLATE_LETTERS[] = {
    pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk, pl, pm, pn, po, pp, pq, pr,
    ps, pt, pu, pv, pw, px, py, pz, arrow, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, pend,
};
#define LASTCHAR (LAST_LETTER-PLATE_LETTERS-1)
const char EIP[] = "ENTER INITIALS";
// *----------------------------------------------------------------------------
#define LETTER0 (PDATA+3)
#define LETTER1 (PDATA+4)
#define LETTER2 (PDATA+5)
#define darkp_pal (PDATA+6)
#define medp_pal (PDATA+7)
#define lightp_pal (PDATA+8)
#define lightp1_pal (PDATA+9)
#define INIT0 PDATA //ptr to obj 0 (1st char)
#define INIT1 (PDATA+1) //1
#define INIT2 (PDATA+2) //2
#define STRFREE (PDATA+3) //steering wheel amount of free spin
#define STRSTRT (PDATA+4) //steering wheel minimum
#define PEDTRIG (PDATA+5) //middle of pedal (pedal trigger)
#define OLDPOT0 (PDATA+6)
#define FLASH_PROC (PDATA+6) //Not used at the same time as OLDPOT0
#define INITI0 (PDATA+7) //initial index
#define INITI1 (PDATA+8) //initial index
#define INITI2 (PDATA+9) //initial index
#define PLATEOBJ (PDATA+10)
#define PRESSOBJ (PDATA+11)
#define BPRESSOBJ (PDATA+12)
#define SCROLLBOBJ (PDATA+13)
#define FRAMEOBJ (PDATA+14) //No longer used
#define MISPLATEOBJ (PDATA+15)
#define HSPOINTER (PDATA+16)
#define PLACE (PDATA+17)
#define WHITE_PAL (PDATA+18)
#define ARMSOBJ (PDATA+19)
#define TEMP_STR (PDATA+20) //10 long
#define CAMX (PDATA+30)
#define CAMY (PDATA+31)
#define CAMZ (PDATA+32)
#define RACE_NUMBER (PDATA+33)
#define GREY_PAL (PDATA+34)

// *----------------------------------------------------------------------------
void ENTER_INITIALS(void)
{
    // asm 000031A6: 	CALL	INTO_TABLE_P
    // asm 000031A7: 	BC	GOODENOUGH
    // asm 000031A8: 	RETP		;NOPE, didn't make it
GOODENOUGH:
    // asm 000031AC: 	STI	R0,*+AR7(PLACE)
    // ;Wait for LOADING TO END
    // asm 000031AD: 	LDI	1,R0
    // asm 000031AE: 	STI	R0,@NOSWAP
    // asm 000031AF: 	LDI	0,R4
MSLP2:
    // asm 000031B0: 	LDI	@DECOMP_ACTIVE,R0
    // asm 000031B1: 	BNZ	MSLP3
    // asm 000031B2: 	ADDI	1,R4
    // asm 000031B3: 	CMPI	3,R4
    // asm 000031B4: 	BGT	MSLPX				;Done Loading
MSLP3:
    // asm 000031B5: 	SLEEP	1
    // asm 000031B7: 	BU	MSLP2
MSLPX:
    // asm 000031B8: 	LDI	0,R0
    // asm 000031B9: 	STI	R0,@NOAERASE
    // asm 000031BA: 	CALL	SILENT
    // asm 000031BB: 	SOND1	ENTER_INITS_THEME	;Play the Initials entry theme
    // asm 000031BD: 	CALL	OBJ_INIT		;Zero out object data list pointers
    // asm 000031BE: 	CALL	TEXT_INIT
    // asm 000031BF: 	FLOAT	-512,R0
    // asm 000031C0: 	STF	R0,@INFIN_CORRECT
    // asm 000031C1: 	LDI	@_MODE,R0
    // asm 000031C2: 	ANDN	MMODE|MWATER|MBRIDGE|MINTUNNEL,R0
    // asm 000031C3: 	OR	MINIT|MINFIN|MHS,R0
    // asm 000031C4: 	STI	R0,@_MODE
    // asm 000031C5: 	LDI	UTIL_C|TEXTP_T,R0
    // asm 000031C6: 	LDI	-1,R1
    // asm 000031C7: 	CALL	PRC_KILLALL
    // asm 000031C8: 	CLRI	R0			;Black background
    // asm 000031C9: 	STI	R0,@BGNDCOLA
    // asm 000031CA: 	LDP	@_CAMERAPOS+X		;Initialize the camera
    // asm 000031CB: 	LDF	-24,R0
    // asm 000031CC: 	STF	R0,@_CAMERAPOS+X
    // asm 000031CD: 	FLOAT	PRESS_STARTY,R0
    // asm 000031CE: 	STF	R0,@_CAMERAPOS+Y
    // asm 000031CF: 	FLOAT	PRESS_STARTZ,R0
    // asm 000031D0: 	STF	R0,@_CAMERAPOS+Z
    // asm 000031D1: 	CLRF	R2
    // asm 000031D2: 	STF	R2,@_CAMERARAD+Y
    // asm 000031D3: 	SETDP
    // asm 000031D4: 	LDI	@CAMERAMATRIXI,AR2
    // asm 000031D5: 	CALL	FIND_YMATRIX
    // ;	CALL	CLEANUP_PALS
    // asm 000031D6: 	CALL	LOAD_FIXED_PALETTES
    // asm 000031D7: 	LDL	press_PALETTES,AR2
    // asm 000031D8: 	CALL	HARDalloc_section
    // asm 000031D9: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm 000031DE: 	DMA_WT		R0
    // asm 000031E3: 	CALL	FIFO_RESET		;This will load the wave ram before continuing on
    // asm 000031E4: 	LDI	1,R0
    // asm 000031E5: 	STI	R0,@HARD_SECTION_LOAD
    // asm 000031E6: 	LDL	_SECpress,AR2
    // asm 000031E7: 	CALL	LOAD_SECTION_REQ
    // asm 000031E8: 	LDL	scroll_white,AR2	;This is the palette for the letters
    // asm 000031E9: 	CALL	PAL_ALLOC_RAW
    // asm 000031EA: 	STI	R0,*+AR7(WHITE_PAL)
    // asm 000031EB: 	LDL	press_grp,AR2
    // asm 000031EC: 	CALL	LOAD_SINGLE_SECTION
    // asm 000031ED: 	CALL	INIT_PRESS_OBJECTS	;Go and set up pointers to special Objects
    // asm 000031EE: 	CALL	RESCAN
    // asm 000031EF: 	LDI	0,R0
    // asm 000031F0: 	STI	R0,@NOSWAP
    // asm 000031F1: 	LDI	LASTCHAR+1,R0			;Set the steering wheel selection
    // asm 000031F2: 	STI	R0,@POSES
    // asm 000031F3: 	LDI	LASTCHAR/2,R0
    // asm 000031F4: 	STI	R0,@POSE
    // asm 000031F5: 	LDI	20,R0			;Set the number of seconds to enter your initials
    // asm 000031F6: 	STI	R0,@_countdown
    // 	;GET THE OBJECTS
    // 	;
    // asm 000031F7: 	LDL	scroll_gr2,AR2	;Palette used for the Letters on the bottom of the press
    // asm 000031F8: 	CALL	PAL_ALLOC_RAW
    // asm 000031F9: 	STI	R0,*+AR7(GREY_PAL)
    // asm 000031FA: 	CALL	CHECK_FIRST_TIME
    // asm 000031FB: 	BC	PRESS_CODE_ENTRY
    // asm 000031FC: 	LDI	*+AR7(WHITE_PAL),R4
    // asm 000031FD: 	FLOAT	-LETTER_SIZEX-LETTER_SIZEX/4,R5	;Position of the first letter
    // asm 000031FE: 	FLOAT	-103+LETTER_YOFF,R6
    // asm 000031FF: 	FLOAT	-PRESS_DIAM-73,R7
    // asm 00003200: 	LDI	' ',R0
    // asm 00003201: 	STI	R0,*+AR7(INITI0)
    // asm 00003202: 	STI	R0,*+AR7(INITI1)
    // asm 00003203: 	STI	R0,*+AR7(INITI2)
    // asm 00003204: 	LDL	pa,AR2			;Create the first Letter
    // asm 00003205: 	CALL	OBJ_GETE
    // asm 00003206: 	STI	AR0,*+AR7(INIT0)
    // asm 00003207: 	STI	R4,*+AR0(OPAL)
    // asm 00003208: 	STF	R5,*+AR0(OPOSX)
    // asm 00003209: 	STF	R6,*+AR0(OPOSY)
    // asm 0000320A: 	STF	R7,*+AR0(OPOSZ)
    // asm 0000320B: 	FLOAT	LETTER_SIZEX,R0
    // asm 0000320C: 	ADDF	R0,R5
    // asm 0000320D: 	LDI	AR0,AR2
    // asm 0000320E: 	CALL	OBJ_INSERT
    // asm 0000320F: 	LDL	po,AR2			;Create the second letter
    // asm 00003210: 	CALL	OBJ_GETE
    // asm 00003211: 	STI	AR0,*+AR7(INIT1)
    // asm 00003212: 	STI	R4,*+AR0(OPAL)
    // asm 00003213: 	STF	R5,*+AR0(OPOSX)
    // asm 00003214: 	STF	R6,*+AR0(OPOSY)
    // asm 00003215: 	STF	R7,*+AR0(OPOSZ)
    // asm 00003216: 	FLOAT	LETTER_SIZEX,R0
    // asm 00003217: 	ADDF	R0,R5
    // asm 00003218: 	LDL	po,AR2			;Create the third letter
    // asm 00003219: 	CALL	OBJ_GETE
    // asm 0000321A: 	STI	AR0,*+AR7(INIT2)
    // asm 0000321B: 	STI	R4,*+AR0(OPAL)
    // asm 0000321C: 	STF	R5,*+AR0(OPOSX)
    // asm 0000321D: 	STF	R6,*+AR0(OPOSY)
    // asm 0000321E: 	STF	R7,*+AR0(OPOSZ)
    // asm 0000321F: 	CREATE	ENTERTEXT,SPAWNER_C	;This will slide in the text "ENTER INITIALS"
    // asm 00003222: 	CLRI	AR5			;character index
    // asm 00003223: 	LDF	@STEERCT,R0
    // asm 00003224: 	STF	R0,@WHEELPOS
    // asm 00003225: 	CALL	GETCHOICE	;READS the steering wheel, uses POSES = number of choices
    // asm 00003226: 	LDI	@POSE,R4	;On return POSE = Choice wheel is pointing at.
    // asm 00003227: 	STI	R4,*+AR7(OLDPOT0)
    // asm 00003228: 	LDI	0,R0		;CLEAR OUT LEFT OVER START HIT
    // asm 00003229: 	STI	R0,@START_HIT
    // asm 0000322A: 	LDF	@PEDALMN,R0
    // asm 0000322B: 	LDF	@PEDALMX,R1
    // asm 0000322C: 	SUBF	R0,R1
    // asm 0000322D: 	FIX	R1
    // asm 0000322E: 	RS	1,R1
    // asm 0000322F: 	FIX	@PEDALMN,R0
    // asm 00003230: 	ADDI	R0,R1
    // asm 00003231: 	STI	R1,*+AR7(PEDTRIG)
    // asm 00003232: 	LDI	0,AR6			;Set debounce counter to 0
    // ;	BR	PEDALWT
    // asm 00003233: 	LDI	1,R0
    // asm 00003234: 	STI	R0,@PEDHIT		;not touched
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTER_INITIALS", 0, 0);
    UNIMPL();
}

// *ELP END CHANGE

// ;This does a back space

void PEDALWT(void)
{
    // ;	LDI	@_countdown,R0
    // ;	BLE	EIML
    // ;
    // ;	SLEEP	1
    // ;
    // ;	;wait for release of pedal
    // ;	LDI	@_pot1,R0
    // ;	LDI	*+AR7(PEDTRIG),R1
    // ;	CMPI	R1,R0
    // ;	BGE	PEDALWT
NOTRIG:
    // asm 000032A1: 	CALL	HSTD_TIMER
    // asm 000032A2: 	CALL	POS_SCROLLB
    // asm 000032A3: 	SLEEP	1
    // asm 000032A5: 	BR	EIML
DONE_ENTRY:
    // asm 000032A6: 	CMPI	LASTCHAR,R4
    // asm 000032A7: 	BNE	DE1
    // asm 000032A8: 	LDI	AR1,AR2
    // asm 000032A9: 	CALL	OBJ_PULL
DE1:
    // asm 000032AA: 	LDI	*+AR7(INITI2),R2	;Create string for initial entry
    // asm 000032AB: 	LSH	8,R2
    // asm 000032AC: 	OR	*+AR7(INITI1),R2
    // asm 000032AD: 	LSH	8,R2
    // asm 000032AE: 	OR	*+AR7(INITI0),R2
    // asm 000032AF: 	SETADJ	ADJ_INITIALS
    // asm 000032B1: 	BR	PRESS_CODE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PEDALWT", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*This proc slides in the WORDS "ENTER INITIALS"
*/
void ENTERTEXT(void)
{
    // asm 000032B2: 	FLOAT	616,R2			;XPOS
    // asm 000032B3: 	CALL	CREATE_ENTERTEXT
    // asm 000032B4: 	LDF	-6,R0
    // asm 000032B5: 	STF	R0,*+AR4(TEXT_VELX)
    // asm 000032B6: 	STF	R0,*+AR5(TEXT_VELX)
    // asm 000032B7: 	SLEEP	60
    // asm 000032B9: 	SONDFX	WELCOME
    // asm 000032BB: 	CLRF	R0
    // asm 000032BC: 	STF	R0,*+AR4(TEXT_VELX)
    // asm 000032BD: 	STF	R0,*+AR5(TEXT_VELX)
ET0:
    // asm 000032BE: 	SLEEP	1
    // asm 000032C0: 	BR	ET0			;THIS PROC GETS KILLED BY PRESS_CODE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTERTEXT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*PARAMETERS	R2 = XPOS
*/
void CREATE_ENTERTEXT(void)
{
    // asm 000032C1: 	LDI	@EIPI,AR2		;enter initials	string
    // asm 000032C2: 	FLOAT	254,R3			;YPOS
    // asm 000032C3: 	LDI	9999,RC			;JIFFYS TO DISPLAY (INDEFINATLY)
    // asm 000032C4: 	CALL	TEXT_ADDDS
    // asm 000032C5: 	CALL	MAKE_CENTER
    // asm 000032C6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CREATE_ENTERTEXT", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
void MAKE_CENTER(void)
{
    // asm 000032C7: 	LDI	AR0,AR4
    // asm 000032C8: 	LDI	AR1,AR5
    // asm 000032C9: 	ORM	TXT_CENTER,*+AR4(TEXT_COLOR)
    // asm 000032CC: 	ORM	TXT_CENTER,*+AR5(TEXT_COLOR)
    // asm 000032CF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_CENTER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
void HSTD_TIMER(void)
{
    // asm 000032D0: 	LDI	@_countdown,R2
    // asm 000032D1: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm 000032D2: 	CALL	_itoa
    // asm 000032D3: 	FLOAT	256,R2
    // asm 000032D4: 	FLOAT	343,R3			;YPOS
    // asm 000032D5: 	LDI	1,RC
    // asm 000032D6: 	CALL	TEXT_ADDDS
    // asm 000032D7: 	PUSH	AR4
    // asm 000032D8: 	PUSH	AR5
    // asm 000032D9: 	CALL	MAKE_CENTER
    // asm 000032DA: 	POP	AR5
    // asm 000032DB: 	POP	AR4
    // asm 000032DC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HSTD_TIMER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
void INSERT_INITS(void)
{
    // asm 000032DD: 	LDI	@SCORE,R0
    // asm 000032DE: 	LDI	*+AR7(INITI0),R1
    // asm 000032DF: 	LDI	*+AR7(INITI1),R2
    // asm 000032E0: 	LDI	*+AR7(INITI2),R3
    // asm 000032E1: 	LDI	0,R4			;POSITION FINISHED IN
    // asm 000032E2: 	LDI	*+AR7(PLACE),R5
    // asm 000032E3: 	LDI	*+AR7(RACE_NUMBER),R6
    // asm 000032E4: 	CALL	INSERT_TABLE_ENTRY
    // asm 000032E5: 	LDI	*+AR7(RACE_NUMBER),R1
    // asm 000032E6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_INITS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
* USES BONUS_WAVE and GAMETRAKI to determine...
*
*RETURNS C = 1 PLAYER WILL MAKE IT INTO THE HS TABLE
*	 C = 0 PLAYER WILL NOT MAKE IT
*/
void INTO_TABLE_P(void)
{
    // asm 000032E7: 	CALL	VALIDATE_HSTD_TABLES
    // 	;check check score if good enough to enter
    // 	;the hstd table
    // asm 000032E8: 	LDI	@BONUS_WAVE,R1
    // asm 000032E9: 	SUBI	1,R1
    // asm 000032EA: 	STI	R1,*+AR7(RACE_NUMBER)
    // asm 000032EB: 	LDI	R1,R0
    // asm 000032EC: 	MPYI	GT_SIZE,R0
    // asm 000032ED: 	ADDI	@GAMETRAKI,R0
    // asm 000032EE: 	LDI	R0,AR0
    // ;	LDI	*+AR0(GT_ETIME),R0
    // asm 000032EF: 	LDI	@ETIME,R0
    // asm 000032F0: 	CMPI	14,R1
    // asm 000032F1: 	CALLEQ	CALC_TOTAL_ELAPSED	;A call to ENTER_INITIALS WITH BONUS_WAVE =15 WILL
    // asm 000032F2: 	CMPI	0,R0			;CHECK SF TO DC
    // asm 000032F3: 	BEQ	ITP1			;Did Not Finish this Race (DNF)
    // asm 000032F4: 	CALL	CHECK_RACE_TABLE
    // asm 000032F5: 	CMPI	-1,R0
    // asm 000032F6: 	BNE	ITP2
ITP1:
    // asm 000032F7: 	CLRC
    // asm 000032F8: 	RETS
ITP2:
    // asm 000032F9: 	SETC
    // asm 000032FA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INTO_TABLE_P", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*RETURNS C = 1 PLAYER HAS ALLREADY ENTERED INITS
*	 C = 0 PLAYER HAS NOT ENTERED INITS
*SETS PLAYER LETTERS IF FOUND
* INITI0-INITI2
*/
void CHECK_FIRST_TIME(void)
{
    // asm 000032FB: 	READADJ	ADJ_INITIALS
    // asm 000032FD: 	CMPI	-1,R0
    // asm 000032FE: 	BNE	NOT_FIRST_TIME
    // asm 000032FF: 	CLRC
    // asm 00003300: 	RETS
NOT_FIRST_TIME:
    // asm 00003301: 	LDI	' ',R1			;Just incase the initials have been corrupted
    // asm 00003302: 	STI	R1,*+AR7(INITI1)
    // asm 00003303: 	STI	R1,*+AR7(INITI2)
    // asm 00003304: 	LDI	AR7,AR0
    // asm 00003305: 	ADDI	INITI0,AR0
NFTLP:
    // asm 00003306: 	LDI	R0,R1
    // asm 00003307: 	AND	0FFh,R1
    // asm 00003308: 	BEQ	NFTX
    // asm 00003309: 	STI	R1,*AR0++
    // asm 0000330A: 	LSH	-8,R0
    // asm 0000330B: 	BNE	NFTLP
NFTX:
    // asm 0000330C: 	SETC
    // asm 0000330D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_FIRST_TIME", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*Below Is code only used for the plate stamping sequence
*
*
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*R0 = total elapsed time
*R0 = 0, did not finish
*/
void CALC_TOTAL_ELAPSED(void)
{
    // asm 0000330E: 	PUSH	R1
    // asm 0000330F: 	PUSH	R2
    // asm 00003310: 	PUSH	AR0
    // asm 00003311: 	LDI	@GAMETRAKI,AR0
    // asm 00003312: 	LDI	13,R1
    // asm 00003313: 	LDI	0,R2
CTELP:
    // asm 00003314: 	LDI	*+AR0(GT_ETIME),R0
    // asm 00003315: 	BEQ	CTEX			;Did Not Finish this Race (DNF)
    // asm 00003316: 	ADDI	R0,R2
    // asm 00003317: 	ADDI	GT_SIZE,AR0
    // asm 00003318: 	SUBI	1,R1
    // asm 00003319: 	BP	CTELP
    // asm 0000331A: 	LDI	R2,R0
CTEX:
    // asm 0000331B: 	POP	AR0
    // asm 0000331C: 	POP	R2
    // asm 0000331D: 	POP	R1
    // asm 0000331E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CALC_TOTAL_ELAPSED", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*	This routine is branched to from within a proccess
*	It will stamp the letters on the plate then put the plate onto the wall
*
*/
#define PRESS_FRAMES 60 //120
#define PLACE_FRAMES 40
#define ROLLER_TRAVEL 12.7117 //2*PI = 1 revolution PI = 3.14
#define ARM_FRAMES 10 //20
#define ARM_START 0
#define ARM_BOTTOM (ARM_START+240)
#define ARM_TRAVEL (ARM_BOTTOM-ARM_START)
#define STAMP_FRAMES 4
#define STAMP_SHAKE 7
#define ARMS2 0x8A

void PRESS_CODE_ENTRY(void)
{
    // ;Put the letters on the bottom of the press
    // asm 0000331F: 	LDI	*+AR7(WHITE_PAL),R4
    // asm 00003320: 	FLOAT	-LETTER_SIZEX-LETTER_SIZEX/4,R5	;Position of the first letter
    // asm 00003321: 	FLOAT	-103+LETTER_YOFF,R6
    // asm 00003322: 	FLOAT	-PRESS_DIAM-73,R7
    // asm 00003323: 	LDI	*+AR7(INITI0),AR2			;Create the first Letter
    // asm 00003324: 	CALL	ASCII_TO_OBJ
    // asm 00003325: 	STI	AR0,*+AR7(INIT0)
    // asm 00003326: 	STI	R4,*+AR0(OPAL)
    // asm 00003327: 	STF	R5,*+AR0(OPOSX)
    // asm 00003328: 	STF	R6,*+AR0(OPOSY)
    // asm 00003329: 	STF	R7,*+AR0(OPOSZ)
    // asm 0000332A: 	FLOAT	LETTER_SIZEX,R0
    // asm 0000332B: 	ADDF	R0,R5
    // asm 0000332C: 	LDI	*+AR7(INITI0),AR2
    // asm 0000332D: 	CMPI	' ',AR2
    // asm 0000332E: 	BEQ	NOINS0
    // asm 0000332F: 	LDI	AR0,AR2
    // asm 00003330: 	CALL	OBJ_INSERT
NOINS0:
    // asm 00003331: 	LDI	*+AR7(INITI1),AR2			;Create the first Letter
    // asm 00003332: 	CALL	ASCII_TO_OBJ
    // asm 00003333: 	STI	AR0,*+AR7(INIT1)
    // asm 00003334: 	STI	R4,*+AR0(OPAL)
    // asm 00003335: 	STF	R5,*+AR0(OPOSX)
    // asm 00003336: 	STF	R6,*+AR0(OPOSY)
    // asm 00003337: 	STF	R7,*+AR0(OPOSZ)
    // asm 00003338: 	FLOAT	LETTER_SIZEX,R0
    // asm 00003339: 	ADDF	R0,R5
    // asm 0000333A: 	LDI	*+AR7(INITI1),AR2
    // asm 0000333B: 	CMPI	' ',AR2
    // asm 0000333C: 	BEQ	NOINS1
    // asm 0000333D: 	LDI	AR0,AR2
    // asm 0000333E: 	CALL	OBJ_INSERT
NOINS1:
    // asm 0000333F: 	LDI	*+AR7(INITI2),AR2			;Create the first Letter
    // asm 00003340: 	CALL	ASCII_TO_OBJ
    // asm 00003341: 	STI	AR0,*+AR7(INIT2)
    // asm 00003342: 	STI	R4,*+AR0(OPAL)
    // asm 00003343: 	STF	R5,*+AR0(OPOSX)
    // asm 00003344: 	STF	R6,*+AR0(OPOSY)
    // asm 00003345: 	STF	R7,*+AR0(OPOSZ)
    // asm 00003346: 	LDI	*+AR7(INITI2),AR2
    // asm 00003347: 	CMPI	' ',AR2
    // asm 00003348: 	BEQ	NOINS2
    // asm 00003349: 	LDI	AR0,AR2
    // asm 0000334A: 	CALL	OBJ_INSERT
NOINS2:
    // asm 0000334B: 	SONDFX	WELCOME
    // *ELP CHANGE
    // asm 0000334D: 	PUSHP	AR5
    // 	;SLEEP	60
    // asm 00003350: 	LDI	30-1,AR5
PAPA35:
    // asm 00003351: LDI	@START_HIT,R0
    // asm 00003352: 	BNZ	PAPA35X
    // asm 00003353: 	SUBI	@NFRAMES,AR5
    // asm 00003354: 	CMPI	0,AR5
    // asm 00003355: 	LDILT	0,AR5
    // asm 00003356: 	SLEEP	1
    // asm 00003358: 	DBU	AR5,PAPA35
PAPA35X:
    // asm 00003359: 	POPP	AR5
    // *ELP END CHANGE
    // ;FALL INTO PRESS CODE
PRESS_CODE:
    // asm 0000335C: 	LDI	SPAWNER_C,R0		;KILL ENTERTEXT
    // asm 0000335D: 	LDI	CLASS_M,R1
    // asm 0000335E: 	CALL	PRC_KILLALL
    // asm 0000335F: 	CALL	TEXT_INIT
    // asm 00003360: 	CLRI	R0
    // asm 00003361: 	STI	R0,@START_HIT
    // asm 00003362: 	LDI	0,R0
    // asm 00003363: 	STI	R0,@FRAMRATE   		;RESET FRAME RATE TO ATTRACT MODE
    // asm 00003364: 	SONDFX	DOPEN
    // asm 00003366: 	LDI	*+AR7(RACE_NUMBER),R1
    // asm 00003367: 	LDI	R1,R0
    // asm 00003368: 	MPYI	GT_SIZE,R0
    // asm 00003369: 	ADDI	@GAMETRAKI,R0
    // asm 0000336A: 	LDI	R0,AR0
    // asm 0000336B: 	LDI	*+AR0(GT_ETIME),R0
    // asm 0000336C: 	CMPI	14,R1
    // asm 0000336D: 	CALLZ	CALC_TOTAL_ELAPSED
    // asm 0000336E: 	STI	R0,@SCORE
    // asm 0000336F: 	CALL	INSERT_INITS
    // asm 00003370: 	STI	R0,@SCORE
    // asm 00003371: 	CALL	FIND_PLATES		;Set up the High score display board
    // asm 00003372: 	LDI	0,R0
    // asm 00003373: 	STI	R0,@_countdown
    // asm 00003374: 	CALL	ADJUST_ROLLERS
    // asm 00003375: 	CALL	ENTER_HSTEXT
    // asm 00003376: 	FLOAT	-1950,R2
    // asm 00003377: 	LDI	HIGH_SCORE_GROUP,R1		;High score group
    // asm 00003378: 	CALL	OBJ_MOVY_GROUP
    // asm 00003379: 	LDI	*+AR7(MISPLATEOBJ),AR2
    // asm 0000337A: 	CALL	OBJ_PULL			;Loose the plate in the players place
    // asm 0000337B: 	CALL	INI_PLAYERS_BOLTS		;Move them into position to fly
    // asm 0000337C: 	LDI	5,R1
    // asm 0000337D: 	CALL	OBJ_FIND
    // asm 0000337E: 	FLOAT	-2000,R0
    // asm 0000337F: 	LDF	*+AR0(OPOSY),R1
    // asm 00003380: 	ADDF	R0,R1
    // asm 00003381: 	STF	R1,*+AR0(OPOSY)
    // asm 00003382: 	LDI	8,R1
    // asm 00003383: 	CALL	OBJ_FIND
    // asm 00003384: 	FLOAT	-2000,R0
    // asm 00003385: 	LDF	*+AR0(OPOSY),R1
    // asm 00003386: 	ADDF	R0,R1
    // asm 00003387: 	STF	R1,*+AR0(OPOSY)
    // asm 00003388: 	LDI	*+AR7(SCROLLBOBJ),AR2	;LOOSE THE WHEEL AND IT's LETTERS
    // asm 00003389: 	CALL	OBJ_PULL
    // asm 0000338A: PRA0					;LOOSE THE LETTERS ON THE FRAME
    // asm 0000338A: 	LDI	MARQLET,R1
    // asm 0000338B: 	CALL	OBJ_FIND
    // asm 0000338C: 	BC	PRA
    // asm 0000338D: 	LDI	AR0,AR2
    // asm 0000338E: 	CALL	OBJ_DELETE
    // asm 0000338F: 	BR	PRA0
PRA:
    // ;Pan the camera back,pull back the frame,rotate the press down,
    // ;and scroll the plate in on the rollers
    // asm 00003390: 	FLOAT	PRESS_FRAMES,R1		;Pan camera to correct y
    // asm 00003391: 	FLOAT	PRESS_TRAVELY,R0
    // asm 00003392: 	CALL	DIV_F			;PRESS_TRAVELZ/PRESS_FRAMES
    // asm 00003393: 	LDP	@_CAMERAPOS+Y
    // asm 00003394: 	ADDF	@_CAMERAPOS+Y,R0
    // asm 00003395: 	STF	R0,@_CAMERAPOS+Y
    // asm 00003396: 	SETDP
    // asm 00003397: 	FLOAT	PRESS_FRAMES,R1		;Pan camera back
    // asm 00003398: 	FLOAT	PRESS_TRAVELZ,R0
    // asm 00003399: 	CALL	DIV_F			;PRESS_TRAVELZ/PRESS_FRAMES
    // asm 0000339A: 	LDP	@_CAMERAPOS+Z
    // asm 0000339B: 	ADDF	@_CAMERAPOS+Z,R0
    // asm 0000339C: 	STF	R0,@_CAMERAPOS+Z
    // asm 0000339D: 	SETDP
    // asm 0000339E: 	FLOAT	PRESS_FRAMES-1,R1	;Rotate press down
    // asm 0000339F: 	LDF	PRESS_RADX,R0
    // asm 000033A0: 	CALL	DIV_F			;PRESS_RADX/PRESS_FRAMES
    // asm 000033A1: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm 000033A2: 	LDF	*+AR0(ORADX),R2
    // asm 000033A3: 	SUBF	R0,R2
    // asm 000033A4: 	STF	R2,*+AR0(ORADX)
    // asm 000033A5: 	LDI	AR0,AR2
    // asm 000033A6: 	ADDI	OMATRIX,AR2
    // asm 000033A7: 	CALL	FIND_XMATRIX
    // asm 000033A8: 	FLOAT	PRESS_FRAMES,R1		;Pull the frame back
    // asm 000033A9: 	FLOAT	-2000,R0
    // asm 000033AA: 	CALL	DIV_F			;-2000/PRESS_FRAMES
    // ;	LDI	*+AR7(FRAMEOBJ),AR0
    // asm 000033AB: 	LDI	FRAME,R1
    // asm 000033AC: 	CALL	OBJ_FIND
    // asm 000033AD: 	BC	PRA2
PRA1:
    // asm 000033AE: 	LDF	*+AR0(OPOSZ),R2
    // asm 000033AF: 	ADDF	R0,R2
    // asm 000033B0: 	STF	R2,*+AR0(OPOSZ)
    // asm 000033B1: 	CALL	OBJ_FIND_NEXT
    // asm 000033B2: 	BNC	PRA1
PRA2:
    // asm 000033B3: 	CALL	MOVE_PRESSB		;Move the bottom of the press reletive to the press
    // asm 000033B4: 	CALL	MOVE_BIGLET_PRESS	;Move the letters reletive to the bottom of the press
    // asm 000033B5: 	LDI	*+AR7(PRESSOBJ),AR0		;Spin the rollers until press is rotated all the way down
    // asm 000033B6: 	LDF	*+AR0(ORADX),R2
    // asm 000033B7: 	CMPF	-0.01309,R2
    // asm 000033B8: 	BLE	PR0A
    // asm 000033B9: 	FLOAT	PRESS_FRAMES,R1
    // asm 000033BA: 	LDF	ROLLER_TRAVEL,R0
    // asm 000033BB: 	CALL	DIV_F			;PRESS_TRAVELZ/PRESS_FRAMES
    // asm 000033BC: 	CALL	SPIN_ROLLERS		;Spin rollers also moves the plate
    // asm 000033BD: 	SLEEP	1
    // *ELP CHANGE
    // asm 000033BF: 	LDI	@START_HIT,R0
    // asm 000033C0: 	BZ	PRA
    // ;	BR	PRA
    // *ELP END CHANGE
PR0A:
    // asm 000033C1: 	LDI	FRAME,R1
    // asm 000033C2: 	CALL	OBJ_FIND
    // asm 000033C3: 	BC	PR0A2
    // asm 000033C4: 	LDI	AR0,AR2
    // asm 000033C5: 	CALL	OBJ_DELETE
    // asm 000033C6: 	BR	PR0A
PR0A2:
    // asm 000033C7: 	SONDFX	HYDRO
    // *ELP CHANGE
    // 	;SLEEP	10
    // asm 000033C9: 	SLEEP	5
    // *ELP END CHANGE
PR0:
    // *ELP CHANGE
    // asm 000033CB: 	LDI	@START_HIT,R0
    // asm 000033CC: 	BNZ	PR1
    // *ELP END CHANGE
    // ;Move the Press and Arms down to stamp
    // asm 000033CD: 	FLOAT	ARM_FRAMES,R1
    // asm 000033CE: 	FLOAT	ARM_TRAVEL,R0
    // asm 000033CF: 	CALL	DIV_F			;ARM_TRAVEL/ARM_FRAMES
    // asm 000033D0: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm 000033D1: 	LDF	*+AR0(OPOSY),R2
    // asm 000033D2: 	ADDF	R0,R2
    // asm 000033D3: 	STF	R2,*+AR0(OPOSY)
    // asm 000033D4: 	PUSHF	R0
    // asm 000033D5: 	CALL	MOVE_PRESSB
    // asm 000033D6: 	CALL	MOVE_BIGLET_PRESS
    // asm 000033D7: 	POPF	R0
    // asm 000033D8: 	LDI	*+AR7(ARMSOBJ),AR0
    // asm 000033D9: 	LDF	*+AR0(OPOSY),R2
    // asm 000033DA: 	ADDF	R0,R2
    // asm 000033DB: 	STF	R2,*+AR0(OPOSY)
    // asm 000033DC: 	FLOAT	ARM_BOTTOM,R0
    // asm 000033DD: 	CMPF	R0,R2
    // asm 000033DE: 	BGE	PR1
    // asm 000033DF: 	SLEEP	1
    // asm 000033E1: 	BR	PR0
PR1:
    // asm 000033E2: 	SONDFX	STAMP
    // asm 000033E4: 	SONDFX	STAMP1
    // ;STAMP
    // asm 000033E6: 	LDI	*+AR7(INITI0),R0
    // asm 000033E7: 	LDI	*+AR7(INIT0),AR2
    // asm 000033E8: 	CMPI	' ',R0
    // asm 000033E9: 	BEQ	PR1A1
    // asm 000033EA: 	CALL	OBJ_PULL
PR1A1:
    // asm 000033EB: 	LDI	*+AR7(INITI1),R0
    // asm 000033EC: 	LDI	*+AR7(INIT1),AR2
    // asm 000033ED: 	CMPI	' ',R0
    // asm 000033EE: 	BEQ	PR1A2
    // asm 000033EF: 	CALL	OBJ_PULL
PR1A2:
    // asm 000033F0: 	LDI	*+AR7(INITI2),R0
    // asm 000033F1: 	LDI	*+AR7(INIT2),AR2
    // asm 000033F2: 	CMPI	' ',R0
    // asm 000033F3: 	BEQ	PR1A3
    // asm 000033F4: 	CALL	OBJ_PULL
PR1A3:
    // asm 000033F5: 	LDI	STAMP_FRAMES,AR5
    // asm 000033F6: PR1A
    // *ELP CHANGE
    // asm 000033F6: 	LDI	@START_HIT,R0
    // asm 000033F7: 	BNZ	PR1ZZZ
    // *ELP END CHANGE
    // asm 000033F8: 	FLOAT	STAMP_SHAKE,R0
    // asm 000033F9: 	LDP	@_CAMERAPOS+Y
    // asm 000033FA: 	ADDF	@_CAMERAPOS+Y,R0
    // asm 000033FB: 	STF	R0,@_CAMERAPOS+Y
    // asm 000033FC: 	SETDP
    // asm 000033FD: 	SLEEP	1
    // asm 000033FF: 	FLOAT	STAMP_SHAKE,R0
    // asm 00003400: 	NEGF	R0
    // asm 00003401: 	LDP	@_CAMERAPOS+Y
    // asm 00003402: 	ADDF	@_CAMERAPOS+Y,R0
    // asm 00003403: 	STF	R0,@_CAMERAPOS+Y
    // asm 00003404: 	SETDP
    // asm 00003405: 	SLEEP	1
    // asm 00003407: 	DBU	AR5,PR1A
    // asm 00003408: 	FLOAT	PRESS_LASTY,R0
    // asm 00003409: 	LDP	@_CAMERAPOS+Y
    // asm 0000340A: 	STF	R0,@_CAMERAPOS+Y
    // asm 0000340B: 	SETDP
    // *ELP CHANGE
    // 	;SLEEP	6
    // asm 0000340C: 	SLEEP	3
    // *ELP END CHANGE
PR1ZZZ:
    // asm 0000340E: 	CALL	MAKE_PLAYERS_PLATE
PR2:
    // *ELP CHANGE
    // asm 0000340F: 	LDI	@START_HIT,R0
    // asm 00003410: 	BNZ	PR2A
    // *ELP END CHANGE
    // ;PULL press up
    // asm 00003411: 	CALL	MOVE_PRESSB
    // asm 00003412: 	FLOAT	ARM_FRAMES,R1
    // asm 00003413: 	FLOAT	ARM_TRAVEL,R0
    // asm 00003414: 	NEGF	R0
    // asm 00003415: 	CALL	DIV_F			;ARM_TRAVEL/ARM_FRAMES
    // asm 00003416: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm 00003417: 	LDF	*+AR0(OPOSY),R2
    // asm 00003418: 	ADDF	R0,R2
    // asm 00003419: 	STF	R2,*+AR0(OPOSY)
    // asm 0000341A: 	LDI	*+AR7(ARMSOBJ),AR0
    // asm 0000341B: 	LDF	*+AR0(OPOSY),R2
    // asm 0000341C: 	ADDF	R0,R2
    // asm 0000341D: 	STF	R2,*+AR0(OPOSY)
    // asm 0000341E: 	FLOAT	ARM_START,R0
    // asm 0000341F: 	CMPF	R0,R2
    // asm 00003420: 	BLE	PR2A
    // asm 00003421: 	SLEEP	1
    // asm 00003423: 	BR	PR2
PR2A:
    // ;Rotate the camera up
    // asm 00003424: 	SONDFX	DOPEN
    // asm 00003426: 	LDP	@_CAMERAPOS
    // asm 00003427: 	LDF	@_CAMERAPOS,R0
    // asm 00003428: 	LDF	@_CAMERAPOS+Y,R1
    // asm 00003429: 	LDF	@_CAMERAPOS+Z,R2
    // asm 0000342A: 	SETDP
    // asm 0000342B: 	STF	R0,*+AR7(CAMX)
    // asm 0000342C: 	STF	R1,*+AR7(CAMY)
    // asm 0000342D: 	STF	R2,*+AR7(CAMZ)
    // asm 0000342E: 	LDI	ROT_FRAMES-1,AR6
    // asm 0000342F: PR2AA
    // *ELP CHANGE
    // asm 0000342F: 	LDI	@START_HIT,R0
    // asm 00003430: 	BNZ	PR2A1
    // *ELP END CHANGE
    // asm 00003431: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm 00003432: 	FLOAT	-50,R0
    // asm 00003433: 	LDF	*+AR0(OPOSY),R2
    // asm 00003434: 	ADDF	R0,R2
    // asm 00003435: 	STF	R2,*+AR0(OPOSY)
    // asm 00003436: 	LDI	*+AR7(ARMSOBJ),AR0
    // asm 00003437: 	LDF	*+AR0(OPOSY),R2
    // asm 00003438: 	ADDF	R0,R2
    // asm 00003439: 	STF	R2,*+AR0(OPOSY)
    // asm 0000343A: 	LDI	*+AR7(BPRESSOBJ),AR0
    // asm 0000343B: 	LDF	*+AR0(OPOSY),R2
    // asm 0000343C: 	ADDF	R0,R2
    // asm 0000343D: 	STF	R2,*+AR0(OPOSY)
    // asm 0000343E: 	LDI	ARMS2,R1
    // asm 0000343F: 	CALL	OBJ_FIND
    // asm 00003440: 	LDF	*+AR0(OPOSY),R2
    // asm 00003441: 	ADDF	R0,R2
    // asm 00003442: 	STF	R2,*+AR0(OPOSY)
    // asm 00003443: 	CALL	OBJ_FIND_NEXT
    // asm 00003444: 	LDF	*+AR0(OPOSY),R2
    // asm 00003445: 	ADDF	R0,R2
    // asm 00003446: 	STF	R2,*+AR0(OPOSY)
    // asm 00003447: 	FLOAT	ROT_FRAMES,R1		;Pan camera to correct y
    // asm 00003448: 	LDF	CAM_RADX,R0
    // asm 00003449: 	CALL	DIV_F
    // asm 0000344A: 	LDP	@_CAMERARAD
    // asm 0000344B: 	LDF	@_CAMERARAD,R2
    // asm 0000344C: 	ADDF	R0,R2
    // asm 0000344D: 	STF	R2,@_CAMERARAD
    // asm 0000344E: 	SETDP
    // asm 0000344F: 	LDI	@CAMERAMATRIXI,AR2
    // asm 00003450: 	CALL	FIND_XMATRIX
    // asm 00003451: 	NEGF	R2
    // asm 00003452: 	LDI	@MATRIXAI,AR2
    // asm 00003453: 	CALL	FIND_XMATRIX
    // asm 00003454: 	LDI	@MATRIXAI,R2
    // asm 00003455: 	LDI	AR7,AR2
    // asm 00003456: 	ADDI	CAMX,AR2
    // asm 00003457: 	LDI	@CAMERAPOSI,R3
    // asm 00003458: 	CALL	MATRIX_MUL
    // asm 00003459: 	SLEEP	1
    // asm 0000345B: 	DBU	AR6,PR2AA
    // ;Scroll plate left,pan camera left with th plate;ZOOM IN;ZOOM OUT
    // asm 0000345C: 	SONDFX	GOPEN
    // asm 0000345E: 	LDF	0.00,R6			;AR5 is speed of rotation
PR2A1:
    // *ELP CHANGE
    // asm 0000345F: 	LDI	@START_HIT,R0
    // asm 00003460: 	BNZ	PR2A3
    // *ELP END CHANGE
    // asm 00003461: 	LDF	R6,R0
    // *ELP CHANGE
    // 	;ADDF	0.02,R6	 		;ACCELERATE
    // asm 00003462: 	ADDF	0.04,R6	 		;ACCELERATE
    // *ELP END CHANGE
    // ;	ADDF	0.01,R6	 		;ACCELERATE
    // asm 00003463: 	CALL	SPIN_ROLLERS
    // asm 00003464: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 00003465: 	LDF	*+AR0(OPOSX),R0
    // asm 00003466: 	SUBF	*+AR7(CAMX),R0
    // *ELP CHANGE
    // 	;MPYF	0.20,R0
    // asm 00003467: 	MPYF	0.40,R0
    // *ELP END CHANGE
    // asm 00003468: 	ADDF	*+AR7(CAMX),R0
    // asm 00003469: 	FLOAT	cam_left_stop,R1
    // asm 0000346A: 	CMPF	R1,R0
    // asm 0000346B: 	LDFLT	R1,R0
    // asm 0000346C: 	STF	R0,*+AR7(CAMX)
    // asm 0000346D: 	LDF	R0,R3
    // asm 0000346E: 	FLOAT	zoomin_stop,R4
    // asm 0000346F: 	FLOAT	zoomout_stop,R5
    // asm 00003470: 	FLOAT	-2500,R2		;Accelerating or decelerating?
    // asm 00003471: 	CMPF	R2,R3			;R3 set above to = PLATE OPOSX
    // asm 00003472: 	LDFGE	R4,R0			;Accelerating
    // asm 00003473: 	LDFLT	R5,R0			;Deccelerating
    // asm 00003474: 	SUBF	*+AR7(CAMZ),R0
    // *ELP CHANGE
    // 	;MPYF	0.1,R0
    // asm 00003475: 	MPYF	0.2,R0
    // *ELP END CHANGE
    // ;	MPYF	0.05,R0
    // asm 00003476: 	ADDF	*+AR7(CAMZ),R0
    // asm 00003477: 	CMPF	R4,R0			;Range check the zpos
    // asm 00003478: 	LDFGT	R4,R0			;to large
    // asm 00003479: 	CMPF	R5,R0
    // asm 0000347A: 	LDFLT	R5,R0			;to small
    // asm 0000347B: 	STF	R0,*+AR7(CAMZ)
    // asm 0000347C: 	LDP	@_CAMERARAD
    // asm 0000347D: 	LDF	@_CAMERARAD,R2
    // asm 0000347E: 	SETDP
    // asm 0000347F: 	NEGF	R2
    // asm 00003480: 	LDI	@MATRIXAI,AR2
    // asm 00003481: 	CALL	FIND_XMATRIX
    // asm 00003482: 	LDI	@MATRIXAI,R2
    // asm 00003483: 	LDI	AR7,AR2
    // asm 00003484: 	ADDI	CAMX,AR2
    // asm 00003485: 	LDI	@CAMERAPOSI,R3
    // asm 00003486: 	CALL	MATRIX_MUL
    // asm 00003487: 	CALL	ADJUST_ROLLERS
    // asm 00003488: 	SLEEP	1
    // asm 0000348A: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 0000348B: 	LDF	*+AR0(OPOSX),R0
    // asm 0000348C: 	FLOAT	-2500,R1
    // asm 0000348D: 	CMPF	R1,R0
    // asm 0000348E: 	BGE	PR2A2
    // *ELP CHANGE
    // asm 0000348F: 	ADDF	-0.08,R6	 		;DEACCELERATE (MUST BE TWICE ACCELERATE)
    // *ELP END CHANGE
    // asm 00003490: 	CMPF	0.02,R6
    // asm 00003491: 	LDFN	0.02,R6
    // ;	ADDF	-0.02,R6	 		;DEACCELERATE (MUST BE TWICE ACCELERATE)
    // ;	CMPF	0.01,R6
    // ;	LDFN	0.01,R6
PR2A2:
    // asm 00003492: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 00003493: 	LDF	*+AR0(OPOSX),R0
    // asm 00003494: 	FLOAT	left_stop,R1
    // asm 00003495: 	CMPF	R1,R0
    // asm 00003496: 	BGE	PR2A1
    // ;Flip the plate up on end 90deg
PR2A3:
    // *ELP CHANGE
    // asm 00003497: 	LDI	@START_HIT,R0
    // asm 00003498: 	BNZ	PR3B
    // *ELP END CHANGE
    // asm 00003499: 	LDI	*+AR7(PLATEOBJ),AR2
    // asm 0000349A: 	LDF	0.15708,R2
    // asm 0000349B: 	ADDF	*+AR2(ORADX),R2
    // asm 0000349C: 	CMPF	HALFPI,R2
    // asm 0000349D: 	LDFGT	HALFPI,R2
    // asm 0000349E: 	STF	R2,*+AR2(ORADX)
    // asm 0000349F: 	ADDI	OMATRIX,AR2
    // asm 000034A0: 	CALL	FIND_XMATRIX
    // asm 000034A1: 	LDI	*+AR7(PLATEOBJ),AR2	;Setup the offset for the bottom of the plate
    // asm 000034A2: 	LDF	0,R0
    // asm 000034A3: 	STF	R0,*+AR2(OVELX)
    // asm 000034A4: 	STF	R0,*+AR2(OVELY)
    // asm 000034A5: 	FLOAT	-156,R0
    // asm 000034A6: 	STF	R0,*+AR2(OVELZ)
    // asm 000034A7: 	LDI	*+AR7(PLATEOBJ),R2
    // asm 000034A8: 	ADDI	OMATRIX,R2
    // asm 000034A9: 	ADDI	OVELX,AR2
    // asm 000034AA: 	LDI	AR2,R3
    // asm 000034AB: 	CALL	MATRIX_MUL
    // asm 000034AC: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 000034AD: 	FLOAT	PLATE_ZOFF,R0
    // asm 000034AE: 	ADDF	*+AR0(OVELZ),R0
    // asm 000034AF: 	STF	R0,*+AR0(OPOSZ)
    // asm 000034B0: 	FLOAT	610,R0
    // asm 000034B1: 	ADDF	*+AR0(OVELY),R0
    // asm 000034B2: 	STF	R0,*+AR0(OPOSY)
    // asm 000034B3: 	LDF	0,R0			;No x movement
    // asm 000034B4: 	CALL	MOVE_PLAYERS_LETTERS
    // asm 000034B5: 	SLEEP	1
    // asm 000034B7: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 000034B8: 	LDF	*+AR0(ORADX),R2
    // asm 000034B9: 	CMPF	HALFPI,R2
    // asm 000034BA: 	BNE	PR2A3
    // ;Put the plate onto the wall
    // asm 000034BB: 	SONDFX	WIPE4
    // ;	FLOAT	PRESS_LASTY,R0
    // ;	STF	R0,*+AR7(CAMY)
    // ;	FLOAT	PRESS_LASTZ,R0
    // ;	STF	R0,*+AR7(CAMZ)
    // asm 000034BD: 	LDI	PLACE_FRAMES-1,AR5
    // asm 000034BE: PR3
    // asm 000034BE: 	FLOAT	PLACE_FRAMES,R1
    // asm 000034BF: 	LDF	PLACE_ENDRADX,R0
    // asm 000034C0: 	SUBF	HALFPI,R0		;Allready rotated 90deg
    // asm 000034C1: 	CALL	DIV_F
    // asm 000034C2: 	LDI	*+AR7(PLATEOBJ),AR2
    // asm 000034C3: 	ADDF	*+AR2(ORADX),R0
    // asm 000034C4: 	CMPF	PLACE_ENDRADX,R0
    // asm 000034C5: 	LDFGT	PLACE_ENDRADX,R0
    // asm 000034C6: 	STF	R0,*+AR2(ORADX)
    // asm 000034C7: 	LDF	R0,R2
    // asm 000034C8: 	ADDI	OMATRIX,AR2
    // asm 000034C9: 	CALL	FIND_XMATRIX
    // asm 000034CA: 	LDI	*+AR7(MISPLATEOBJ),AR2
    // asm 000034CB: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 000034CC: 	LDF	*+AR2(OPOSY),R0
    // asm 000034CD: 	SUBF	*+AR0(OPOSY),R0
    // asm 000034CE: 	FLOAT	AR5,R1
    // asm 000034CF: 	CALL	DIV_F
    // asm 000034D0: 	LDF	R0,R2
    // asm 000034D1: 	ADDF	*+AR0(OPOSY),R0
    // asm 000034D2: 	STF	R0,*+AR0(OPOSY)
    // asm 000034D3: 	CMPF	*+AR7(CAMY),R0
    // asm 000034D4: 	BGT	PR3A			;Don't move the camera until the plate is above center
    // asm 000034D5: 	SUBF	*+AR7(CAMY),R0
    // asm 000034D6: 	MPYF	0.20,R0
    // asm 000034D7: 	ADDF	*+AR7(CAMY),R0
    // asm 000034D8: 	FLOAT	-2150,R1
    // asm 000034D9: 	CMPF	R1,R0
    // asm 000034DA: 	LDFLT	R1,R0
    // asm 000034DB: 	STF	R0,*+AR7(CAMY)
PR3A:
    // asm 000034DC: 	LDF	*+AR2(OPOSZ),R0
    // asm 000034DD: 	SUBF	*+AR0(OPOSZ),R0
    // asm 000034DE: 	FLOAT	AR5,R1
    // asm 000034DF: 	CALL	DIV_F
    // asm 000034E0: 	LDF	R0,R2
    // asm 000034E1: 	ADDF	*+AR0(OPOSZ),R0
    // asm 000034E2: 	STF	R0,*+AR0(OPOSZ)
    // asm 000034E3: 	LDF	*+AR2(OPOSX),R0
    // asm 000034E4: 	SUBF	*+AR0(OPOSX),R0
    // asm 000034E5: 	FLOAT	AR5,R1
    // asm 000034E6: 	CALL	DIV_F
    // asm 000034E7: 	LDF	R0,R2
    // asm 000034E8: 	ADDF	*+AR0(OPOSX),R0
    // asm 000034E9: 	STF	R0,*+AR0(OPOSX)
    // asm 000034EA: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 000034EB: 	LDF	R2,R0
    // asm 000034EC: 	NEGF	R0
    // asm 000034ED: 	CALL	MOVE_PLAYERS_LETTERS
    // asm 000034EE: 	FLOAT	ROT_FRAMES,R1		;Pan camera to correct y
    // asm 000034EF: 	LDF	-CAM_RADX,R0
    // asm 000034F0: 	CALL	DIV_F
    // asm 000034F1: 	LDP	@_CAMERARAD
    // asm 000034F2: 	LDF	@_CAMERARAD,R2
    // asm 000034F3: 	ADDF	R0,R2
    // asm 000034F4: 	CMPF	-0.01,R2
    // asm 000034F5: 	LDFGT	0,R2
    // asm 000034F6: 	STF	R2,@_CAMERARAD
    // asm 000034F7: 	SETDP
    // asm 000034F8: 	LDI	@CAMERAMATRIXI,AR2
    // asm 000034F9: 	CALL	FIND_XMATRIX
    // asm 000034FA: 	NEGF	R2
    // asm 000034FB: 	LDI	@MATRIXAI,AR2
    // asm 000034FC: 	CALL	FIND_XMATRIX
    // asm 000034FD: 	LDI	@MATRIXAI,R2
    // asm 000034FE: 	LDI	AR7,AR2
    // asm 000034FF: 	ADDI	CAMX,AR2
    // asm 00003500: 	LDI	@CAMERAPOSI,R3
    // asm 00003501: 	CALL	MATRIX_MUL
    // asm 00003502: 	SLEEP	1
    // asm 00003504: 	DBU	AR5,PR3
    // ;Now fly the bolts in
    // asm 00003505: 	LDI	*+AR7(PLACE),R4	;Calculate the first bolts object number
    // asm 00003506: 	MPYI	4,R4
    // asm 00003507: 	ADDI	FIRST_BOLT,R4
    // asm 00003508: 	LDI	3,AR5
PR3B:
    // *ELP CHANGE
    // asm 00003509: 	LDI	@START_HIT,R0
    // asm 0000350A: 	BNZ	PR3BA
    // *ELP END CHANGE
    // asm 0000350B: 	LDI	R4,R1
    // asm 0000350C: 	CALL	OBJ_FIND
    // asm 0000350D: 	BC	PR3BA
    // asm 0000350E: 	LDI	AR0,AR4
    // asm 0000350F: 	CREATE	FLY_BOLT,UTIL_C
    // *ELP CHANGE
    // 	;SLEEP	15
    // asm 00003512: 	SLEEP	6
    // *ELP END CHANGE
    // asm 00003514: 	ADDI	1,R4
    // asm 00003515: 	DBU	AR5,PR3B
PR3BA:
    // *ELP CHANGE
    // asm 00003516: 	LDI	@START_HIT,R0
    // asm 00003517: 	BNZ	PR5A
    // *ELP END CHANGE
    // ;If not in top 5 skip the pan up
    // asm 00003518: 	LDI	*+AR7(PLACE),R0
    // asm 00003519: 	CMPI	4,R0
    // asm 0000351A: 	BGT	PR5A
    // ;Continue to pan the camera to the top of the highscore table
PR4A:
    // *ELP CHANGE
    // asm 0000351B: 	LDI	@START_HIT,R0
    // asm 0000351C: 	BNZ	PR5A
    // *ELP END CHANGE
    // asm 0000351D: 	SLEEP	1
    // asm 0000351F: 	LDP	@_CAMERAPOS+Y
    // asm 00003520: 	LDF	@_CAMERAPOS+Y,R0
    // asm 00003521: 	ADDF	-20,R0
    // asm 00003522: 	FLOAT	-2150,R1
    // asm 00003523: 	CMPF	R1,R0
    // asm 00003524: 	LDFLT	R1,R0
    // asm 00003525: 	STF	R0,@_CAMERAPOS+Y
    // asm 00003526: 	SETDP
    // asm 00003527: 	CMPF	R1,R0
    // asm 00003528: 	BNE	PR4A
PR5A:
    // asm 00003529: 	LDL	plate_darkp,AR2
    // asm 0000352A: 	CALL	PAL_ALLOC_RAW
    // asm 0000352B: 	STI	R0,*+AR7(darkp_pal)
    // asm 0000352C: 	LDL	plate_medp,AR2
    // asm 0000352D: 	CALL	PAL_ALLOC_RAW
    // asm 0000352E: 	STI	R0,*+AR7(medp_pal)
    // asm 0000352F: 	LDL	plate_lightp,AR2
    // asm 00003530: 	CALL	PAL_ALLOC_RAW
    // asm 00003531: 	STI	R0,*+AR7(lightp_pal)
    // asm 00003532: 	LDL	plate_lightp1,AR2
    // asm 00003533: 	CALL	PAL_ALLOC_RAW
    // asm 00003534: 	STI	R0,*+AR7(lightp1_pal)
    // *ELP CHANGE
    // asm 00003535: 	LDI	@START_HIT,R0
    // asm 00003536: 	BNZ	PRESSCODEX2
    // *ELP END CHANGE
    // asm 00003537: 	LDI	*+AR7(RACE_NUMBER),R4
    // asm 00003538: 	CREATEC	FLASH_LETTERS_PROC,UTIL_C
    // asm 0000353B: 	STI	AR0,*+AR7(FLASH_PROC)
    // asm 0000353C: PRESS_CODEX
    // ;	SLEEP	60
    // *ELP CHANGE
    // 	;SLEEP	90
    // asm 0000353C: 	SLEEP	30
    // *ELP END CHANGE
    // asm 0000353E: 	SOND1	CLOSING
PRESSCODEX2:
    // *ELP CHANGE
    // 	;CREATE	UNFOLDMAP_NOPAL,UTIL_C
    // asm 00003540: 	CREATE	UNFOLDMAP,UTIL_C
    // 	;SLEEP	40
    // asm 00003543: 	SLEEP	20
    // *ELP END CHANGE
    // asm 00003545: 	LDI	*+AR7(FLASH_PROC),AR2
    // asm 00003546: 	CALL	PRC_KILL
    // asm 00003547: 	LDL	press_PALETTES,AR2
    // asm 00003548: 	CALL	dealloc_section
    // asm 00003549: 	LDI	*+AR7(GREY_PAL),AR2
    // asm 0000354A: 	CALL	PAL_DELETE_RAW
    // asm 0000354B: 	LDI	*+AR7(WHITE_PAL),AR2
    // asm 0000354C: 	CALL	PAL_DELETE_RAW
    // asm 0000354D: 	LDI	*+AR7(darkp_pal),AR2
    // asm 0000354E: 	CALL	PAL_DELETE_RAW
    // asm 0000354F: 	LDI	*+AR7(medp_pal),AR2
    // asm 00003550: 	CALL	PAL_DELETE_RAW
    // asm 00003551: 	LDI	*+AR7(lightp_pal),AR2
    // asm 00003552: 	CALL	PAL_DELETE_RAW
    // asm 00003553: 	LDI	*+AR7(lightp1_pal),AR2
    // asm 00003554: 	CALL	PAL_DELETE_RAW
    // asm 00003555: 	LDI	@_MODE,R0
    // asm 00003556: 	ANDN	MMODE|MINFIN|MHS,R0
    // asm 00003557: 	OR	MBONUS,R0
    // asm 00003558: 	STI	R0,@_MODE
    // asm 00003559: 	RETP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRESS_CODE_ENTRY", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
#define FRAME 0x85
#define SCROLLB 0x86
#define PRESS 0x87
#define PRESSB 0x88
#define ARMS 0x89
#define PLATE 0x8B
#define MARQLET 0x8C
#define PLATE_ZOFF (ROLLER_ZOFF-140)

void INIT_PRESS_OBJECTS(void)
{
    // asm 0000355D: 	LDI	9h,R1
    // asm 0000355E: 	CALL	OBJ_FIND
    // asm 0000355F: 	LDI	AR0,AR2
    // asm 00003560: 	CALL	OBJ_PULL
    // asm 00003561: 	LDI	81h,R1
    // asm 00003562: 	CALL	OBJ_FIND
    // asm 00003563: 	LDI	AR0,AR2
    // asm 00003564: 	CALL	OBJ_DELETE
    // asm 00003565: 	LDI	82h,R1
    // asm 00003566: 	CALL	OBJ_FIND
    // asm 00003567: 	LDI	AR0,AR2
    // asm 00003568: 	CALL	OBJ_DELETE
    // asm 00003569: 	LDI	83h,R1
    // asm 0000356A: 	CALL	OBJ_FIND
    // asm 0000356B: 	LDI	AR0,AR2
    // asm 0000356C: 	CALL	OBJ_DELETE
    // asm 0000356D: 	LDI	@OACTIVEI,AR0
    // asm 0000356E: 	LDI	*AR0,AR5
FPO:
    // asm 0000356F: LDI	AR5,R0
    // asm 00003570: 	BZ	FPOX
    // asm 00003571: 	LDI	R0,AR0
    // asm 00003572: 	LDI	*AR0,AR5
    // asm 00003573: 	LDI	*+AR0(OID),R0
    // asm 00003574: 	AND	0FFh,R0
    // asm 00003575: 	CMPI	PRESS,R0
    // asm 00003576: 	BNE	FPO1
    // asm 00003577: 	STI	AR0,*+AR7(PRESSOBJ)
    // asm 00003578: 	LDF	PRESS_RADX,R2
    // asm 00003579: 	STF	R2,*+AR0(ORADX)
    // asm 0000357A: 	LDI	AR0,AR2
    // asm 0000357B: 	ADDI	OMATRIX,AR2
    // asm 0000357C: 	CALL	FIND_XMATRIX
    // asm 0000357D: 	BR	FPLE
FPO1:
    // asm 0000357E: 	LDI	*+AR0(OID),R0
    // asm 0000357F: 	AND	0FFh,R0
    // asm 00003580: 	CMPI	PRESSB,R0
    // asm 00003581: 	BNE	FPO1A
    // asm 00003582: 	STI	AR0,*+AR7(BPRESSOBJ)
    // asm 00003583: 	BR	FPLE
FPO1A:
    // asm 00003584: 	CMPI	PLATE,R0
    // asm 00003585: 	BNE	FPO3
    // asm 00003586: 	STI	AR0,*+AR7(PLATEOBJ)
    // asm 00003587: 	LDF	*+AR0(OPOSZ),R2
    // asm 00003588: 	FLOAT	PLATE_ZOFF,R0
    // asm 00003589: 	ADDF	R0,R2
    // asm 0000358A: 	STF	R2,*+AR0(OPOSZ)
    // asm 0000358B: 	BR	FPLE
FPO3:
    // asm 0000358C: 	CMPI	ARMS,R0
    // asm 0000358D: 	BNE	FPO4
    // asm 0000358E: 	STI	AR0,*+AR7(ARMSOBJ)
    // asm 0000358F: 	BR	FPLE
FPO4:
    // asm 00003590: 	CMPI	SCROLLB,R0
    // asm 00003591: 	BNE	FPO6
    // asm 00003592: 	LDI	*+AR7(WHITE_PAL),R2	;This is the palette for the letters
    // asm 00003593: 	STI	R2,*+AR0(OPAL)
    // asm 00003594: 	STI	AR0,*+AR7(SCROLLBOBJ)
    // asm 00003595: 	BR	FPLE
FPO6:
    // ;	CMPI	FRAME,R0
    // ;	BNE	FPO7
    // ;	STI	AR0,*+AR7(FRAMEOBJ)
    // asm 00003596: 	BR	FPLE
    // asm 00003597: FPO7
FPLE:
    // asm 00003597: 	BR	FPO
FPOX:
    // asm 00003598: 	CALL	MOVE_PRESSB
    // asm 00003599: 	CALL	MAKE_NEW_MARQ
    // asm 0000359A: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_PRESS_OBJECTS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*This proc prints the name of the location on the marque
*/

void MAKE_NEW_MARQ(void)
{
    // ;	LDI	5,R1
    // ;	CALL	OBJ_FIND
    // ;	RETSC
    // ;	LDI	AR0,AR4
    // ;	LDI	*+AR4(OROMDATA),AR2
    // ;	CALL	OBJ_GETE
    // ;	RETSC
    // ;	FLOAT	-24,R0
    // ;	STF	R0,*+AR0(OPOSX)
    // ;	FLOAT	-800+83,R0
    // ;	STF	R0,*+AR0(OPOSY)
    // ;	FLOAT	-341,R0
    // ;	STF	R0,*+AR0(OPOSZ)
    // ;	LDI	FRAME,R0	;ID
    // ;	STI	R0,*+AR0(OID)
    // ;	LDI	AR0,AR2
    // ;	CALL	OBJ_INSERT
    // ;
    // ;	LDI	8,R1
    // ;	CALL	OBJ_FIND
    // ;	RETSC
    // ;	LDI	AR0,AR4
    // ;	LDI	*+AR4(OROMDATA),AR2
    // ;	CALL	OBJ_GETE
    // ;	RETSC
    // ;	FLOAT	-24,R0
    // ;	STF	R0,*+AR0(OPOSX)
    // ;	FLOAT	-865+83,R0
    // ;	STF	R0,*+AR0(OPOSY)
    // ;	FLOAT	-341,R0
    // ;	STF	R0,*+AR0(OPOSZ)
    // ;	LDI	FRAME,R0	;ID
    // ;	STI	R0,*+AR0(OID)
    // ;	LDI	AR0,AR2
    // ;	CALL	OBJ_INSERT
    // asm 0000359B: 	FLOAT	-710+83,R3
    // asm 0000359C: 	LDI	*+AR7(RACE_NUMBER),AR2
    // asm 0000359D: 	ADDI	@LEG_NAMESI,AR2
    // asm 0000359E: 	LDI	*AR2,AR2
    // asm 0000359F: 	FLOAT	-24,R2
    // asm 000035A0: 	FLOAT	-341,R4
    // asm 000035A1: 	LDI	MARQLET,R6	;ID
    // asm 000035A2: 	CALL	PRINT3D
    // asm 000035A3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_NEW_MARQ", 0, 0);
    UNIMPL();
}

void MOVE_PRESSB(void)
{
    // asm 000035A4: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm 000035A5: 	LDI	*+AR7(BPRESSOBJ),AR2		;The PRESS OWNS the plate
    // asm 000035A6: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm 000035A7: 	ADDI	OMATRIX,AR2
    // asm 000035A8: 	CALL	FIND_XMATRIX
    // asm 000035A9: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm 000035AA: 	LDI	*+AR7(SCROLLBOBJ),AR2
    // asm 000035AB: 	ADDI	OMATRIX,AR2
    // asm 000035AC: 	CALL	FIND_XMATRIX
    // asm 000035AD: 	LDI	*+AR7(BPRESSOBJ),AR2		;The PRESS OWNS the plate
    // asm 000035AE: 	LDI	*+AR7(SCROLLBOBJ),AR3
    // asm 000035AF: 	FLOAT	PRESS_DIAM,R1
    // asm 000035B0: 	ADDF	60,R1
    // asm 000035B1: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm 000035B2: 	STF	R2,*+AR2(ORADX)
    // asm 000035B3: 	STF	R2,*+AR3(ORADX)
    // asm 000035B4: 	CALL	_SINE
    // asm 000035B5: 	NEGF	R0
    // asm 000035B6: 	MPYF3	R0,R1,R3
    // asm 000035B7: 	ADDF	*+AR0(OPOSZ),R3		;Translated and offset Z
    // asm 000035B8: 	STF	R3,*+AR2(OPOSZ)
    // asm 000035B9: 	ADDF	1,R3			;Ensure that the letters are behind pressb
    // asm 000035BA: 	STF	R3,*+AR3(OPOSZ)
    // asm 000035BB: 	CALL	_COSI
    // asm 000035BC: 	MPYF3	R0,R1,R3
    // asm 000035BD: 	FLOAT	ARM_BOTTOM+PRESS_DIAM,R2
    // asm 000035BE: 	ADDF	*+AR0(OPOSY),R3
    // asm 000035BF: 	CMPF	R2,R3
    // asm 000035C0: 	LDFGT	R2,R3
    // asm 000035C1: 	STF	R3,*+AR2(OPOSY)		;Translated And OFFSET Y
    // asm 000035C2: 	STF	R3,*+AR3(OPOSY)		;Translated And OFFSET Y
    // asm 000035C3: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_PRESSB", 0, 0);
    UNIMPL();
}

void MOVE_BIGLET_PRESS(void)
{
    // asm 000035C4: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm 000035C5: 	LDI	*+AR7(INIT0),AR2
    // asm 000035C6: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm 000035C7: 	ABSF	R2
    // asm 000035C8: 	ADDF	-HALFPI,R2
    // asm 000035C9: 	STF	R2,*+AR2(ORADX)
    // asm 000035CA: 	PUSH	AR2
    // asm 000035CB: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm 000035CC: 	LDI	@MATRIXAI,AR2
    // asm 000035CD: 	CALL	FIND_XMATRIX
    // asm 000035CE: 	LDI	@VECTORAI,AR1
    // asm 000035CF: 	LDF	0,R2
    // asm 000035D0: 	STF	R2,*AR1
    // asm 000035D1: 	FLOAT	PRESS_DIAM+60+13,R2
    // asm 000035D2: 	STF	R2,*+AR1(1)
    // asm 000035D3: 	FLOAT	-98,R2
    // asm 000035D4: 	STF	R2,*+AR1(2)
    // asm 000035D5: 	LDI	@VECTORAI,AR2
    // asm 000035D6: 	LDI	@MATRIXAI,R2
    // asm 000035D7: 	LDI	AR2,R3
    // asm 000035D8: 	CALL	MATRIX_MUL
    // asm 000035D9: 	POP	AR2
    // asm 000035DA: 	LDI	@VECTORAI,AR1
    // asm 000035DB: 	FLOAT	ARM_BOTTOM+PRESS_DIAM+13,R2
    // asm 000035DC: 	LDF	*+AR0(OPOSY),R3
    // asm 000035DD: 	ADDF	*+AR1(1),R3
    // asm 000035DE: 	CMPF	R2,R3
    // asm 000035DF: 	LDFGT	R2,R3
    // asm 000035E0: 	STF	R3,*+AR2(OPOSY)		;Translated And OFFSET Y
    // asm 000035E1: 	LDF	*+AR0(OPOSZ),R3
    // asm 000035E2: 	ADDF	*+AR1(2),R3
    // asm 000035E3: 	STF	R3,*+AR2(OPOSZ)		;Translated And OFFSET Z
    // asm 000035E4: 	LDI	*+AR7(INIT0),AR0	;Now use the first letter to set the rest
    // asm 000035E5: 	LDI	0,R4
MPB:
    // asm 000035E6: 	LDI	INIT0,IR0
    // asm 000035E7: 	ADDI	R4,IR0
    // asm 000035E8: 	LDI	*+AR7(IR0),AR2
    // asm 000035E9: 	LDF	*+AR0(OPOSY),R3		;Translated and offset Y
    // asm 000035EA: 	STF	R3,*+AR2(OPOSY)
    // asm 000035EB: 	LDF	*+AR0(OPOSZ),R3		;Translated and offset Z
    // asm 000035EC: 	STF	R3,*+AR2(OPOSZ)
    // asm 000035ED: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm 000035EE: 	STF	R2,*+AR2(ORADX)
    // asm 000035EF: 	ADDI	OMATRIX,AR2
    // asm 000035F0: 	CALL	FIND_XMATRIX
    // asm 000035F1: 	ADDI	1,R4
    // asm 000035F2: 	CMPI	3,R4
    // asm 000035F3: 	BNE	MPB
    // asm 000035F4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_BIGLET_PRESS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
* This positions the bar of letters during the Name Entry
*
*
*/

/* asm: SCROLLBTAB	;	A    B	  C   D	  E   F	  G   H	  I   J	  K   L	  M   N	  O */
/* asm: 	.word	1057,1010,960,911,855,803,753,696,648,598,546,494,426,367,307 */
/* asm: 	.word	252,193,138,82,24,-35,-97,-161,-230,-292,-347,-401,-465,-517,-573 */
/* asm: 	.word	-633,-697,-757,-820,-880,-945,-1007,-1063 */
/* asm: 	 */
int SCROLLBTAB[] = {
    1057, 1010, 960, 911, 855, 803, 753, 696, 648, 598, 546, 494, 426, 367, 307,
    //       P   Q   R   S  T  U   V    W    X    Y    Z	RUB 0    1    2
    252, 193, 138, 82, 24, -35, -97, -161, -230, -292, -347, -401, -465, -517, -573,
    // 	3	4   5   6   7     8    9     END
    -633, -697, -757, -820, -880, -945, -1007, -1063,
};

void POS_SCROLLB(void)
{
    // asm 0000361B: 	LDI	*+AR7(SCROLLBOBJ),AR2
    // asm 0000361C: 	LDI	@POSE,R2		;POSITION of the STEERING WHEEL
    // asm 0000361D: 	CMPI	LASTCHAR,R2
    // asm 0000361E: 	LDIGT	LASTCHAR,R2
    // asm 0000361F: 	LDI	@SCROLLBTABI,AR1
    // asm 00003620: 	ADDI	R2,AR1
    // asm 00003621: 	FLOAT	*AR1,R2
    // asm 00003622: 	ADDF	-24,R2
    // asm 00003623: 	SUBF	*+AR2(OPOSX),R2
    // asm 00003624: 	MPYF	0.50,R2
    // asm 00003625: 	ADDF	*+AR2(OPOSX),R2
    // asm 00003626: 	STF	R2,*+AR2(OPOSX)
    // asm 00003627: 	LDI	@POSE,R2		;POSITION of the STEERING WHEEL
    // asm 00003628: 	CMPI	*+AR2(OVELX),R2
    // asm 00003629: 	BEQ	POSBX
    // asm 0000362A: 	STI	R2,*+AR2(OVELX)
    // asm 0000362B: 	SONDFX	LETTER_RIGHT
POSBX:
    // asm 0000362D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POS_SCROLLB", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*ADJUST_ROLLERS	This routine places the rollers in a arc so that there
*		priorities are correct.
*IO = NONE
*
*/
#define ROLLER_GROUP 0x400

void ADJUST_ROLLERS(void)
{
    // asm 0000362E: 	LDP	@_CAMERAPOS
    // asm 0000362F: 	LDF	@_CAMERAPOS,R4
    // asm 00003630: 	SETDP
    // asm 00003631: 	LDF	0,R3
ADJRL:
    // asm 00003632: 	FLOAT	246,R2
    // asm 00003633: 	MPYF	R3,R2
    // asm 00003634: 	ADDF	R4,R2
    // asm 00003635: 	CALL	FIND_ROLLER
    // asm 00003636: 	FLOAT	ROLLER_ZOFF,R2
    // asm 00003637: 	ADDF	R3,R2
    // asm 00003638: 	ADDF	-11,R2
    // asm 00003639: 	STF	R2,*+AR2(OPOSZ)
    // asm 0000363A: 	FLOAT	-246,R2
    // asm 0000363B: 	MPYF	R3,R2
    // asm 0000363C: 	ADDF	R4,R2
    // asm 0000363D: 	CALL	FIND_ROLLER
    // asm 0000363E: 	FLOAT	ROLLER_ZOFF,R2
    // asm 0000363F: 	ADDF	R3,R2
    // asm 00003640: 	ADDF	-11,R2
    // asm 00003641: 	STF	R2,*+AR2(OPOSZ)
    // asm 00003642: 	ADDF	1,R3
    // asm 00003643: 	CMPF	7,R3
    // asm 00003644: 	BNE	ADJRL
    // asm 00003645: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADJUST_ROLLERS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*FIND_ROLLER Finds the roller with XPOS closest to R2
*	PARAMETERS	R2 = XPOS to look for
*	 RETURNS	AR2 points to closest roller
*/

void FIND_ROLLER(void)
{
    // asm 00003646: 	PUSH	R3			;R3 is used by the routine that calls this
    // asm 00003647: 	PUSHF	R3
    // asm 00003648: 	LDI	ROLLER_GROUP,R1			;Roller Group
    // asm 00003649: 	CALL	OBJ_GFIND
    // asm 0000364A: 	FLOAT	32000,R3		;ANY large number will do
    // asm 0000364B: 	LDI	AR0,AR2			;Make sure AR2 points to something
FRL:
    // asm 0000364C: 	LDF	*+AR0(OPOSX),R0
    // asm 0000364D: 	SUBF	R2,R0
    // asm 0000364E: 	ABSF	R0
    // asm 0000364F: 	CMPF	R3,R0
    // asm 00003650: 	LDFLT	R0,R3
    // asm 00003651: 	LDILT	AR0,AR2
    // asm 00003652: 	CALL	OBJ_GFIND_NEXT
    // asm 00003653: 	BNC	FRL
    // asm 00003654: 	POPF	R3
    // asm 00003655: 	POP	R3
    // asm 00003656: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_ROLLER", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
#define ROLLER_DIAM 111

void SPIN_ROLLERS(void)
{
    // asm 00003657: 	LDI	ROLLER_GROUP,R1		;Roller group
    // asm 00003658: 	CALL	OBJ_GFIND
SR0:
    // asm 00003659: 	LDF	*+AR0(ORADZ),R2
    // asm 0000365A: 	ADDF	R0,R2
    // asm 0000365B: 	STF	R2,*+AR0(ORADZ)
    // asm 0000365C: 	LDI	AR0,AR2
    // asm 0000365D: 	ADDI	OMATRIX,AR2
    // asm 0000365E: 	CALL	FIND_ZMATRIX
    // asm 0000365F: 	CALL	OBJ_GFIND_NEXT
    // asm 00003660: 	BNC	SR0
    // ;NOW move the plate
    // asm 00003661: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 00003662: 	FLOAT	ROLLER_DIAM,R1
    // asm 00003663: 	MPYF	R1,R0		;R = 111, R0 = RADS rotated
    // 				;Circumfrence = 2*PI*R NOTE R0=2PI/THETA
    // asm 00003664: 	LDF	*+AR0(OPOSX),R2
    // asm 00003665: 	SUBF	R0,R2
    // asm 00003666: 	STF	R2,*+AR0(OPOSX)
    // asm 00003667: 	CALL	MOVE_PLAYERS_LETTERS
    // asm 00003668: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPIN_ROLLERS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*Moves the LEtters on the plate being stamped
*PARAMETERS	R0 = X distance to move the letters
*
*/

void MOVE_PLAYERS_LETTERS(void)
{
    // asm 00003669: 	PUSH	R6
    // asm 0000366A: 	PUSHF	R6
    // asm 0000366B: 	LDI	*+AR7(PLACE),R2
    // asm 0000366C: 	ADDI	16,R2
    // asm 0000366D: 	LDI	1,R1
    // asm 0000366E: 	LSH	R2,R1
    // asm 0000366F: 	CALL	OBJ_GFIND
    // asm 00003670: 	BC	MPLX			;They haven't been made yet
    // asm 00003671: 	LDI	*+AR7(PLATEOBJ),AR1
    // asm 00003672: 	LDF	*+AR1(ORADX),R2
    // asm 00003673: 	ADDF	HALFPI,R2
    // asm 00003674: 	FLOAT	LETTER_YOFF,R3
    // asm 00003675: 	ADDF	*+AR1(OPOSY),R3
    // asm 00003676: 	LDF	*+AR1(OPOSZ),R4
    // asm 00003677: 	SUBF	1,R4			;Make sure the letters are in front
MPL1:
    // asm 00003678: 	LDF	*+AR0(OPOSX),R5
    // asm 00003679: 	SUBF	R0,R5
    // asm 0000367A: 	STF	R5,*+AR0(OPOSX)
    // asm 0000367B: 	STF	R3,*+AR0(OPOSY)
    // asm 0000367C: 	STF	R4,*+AR0(OPOSZ)
    // asm 0000367D: 	LDF	*+AR0(ORADX),R6
    // asm 0000367E: 	CMPF	-PI,R6
    // asm 0000367F: 	BEQ	MPL2
    // asm 00003680: 	LDI	AR0,AR2
    // asm 00003681: 	ADDI	OMATRIX,AR2
    // asm 00003682: 	CALL	FIND_XMATRIX
MPL2:
    // asm 00003683: 	CALL	OBJ_GFIND_NEXT
    // asm 00003684: 	BNC	MPL1
MPLX:
    // asm 00003685: 	POPF	R6
    // asm 00003686: 	POP	R6
    // asm 00003687: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_PLAYERS_LETTERS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
void MAKE_PLAYERS_PLATE(void)
{
    // asm 00003688: 	LDI	*+AR7(RACE_NUMBER),R6			;Race number
    // asm 00003689: 	LDI	*+AR7(PLACE),R7				;Entry number
    // asm 0000368A: 	CALL	GET_TABLE_ADDR
    // asm 0000368B: 	CALL	TABLE_ENTRY_READ
    // asm 0000368C: 	STI	R1,*+AR7(LETTER0)
    // asm 0000368D: 	STI	R2,*+AR7(LETTER1)
    // asm 0000368E: 	STI	R3,*+AR7(LETTER2)
    // asm 0000368F: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm 00003690: 	LDI	*+AR7(PLACE),R1
    // asm 00003691: 	CALL	CREATE_LETTERS
    // asm 00003692: 	LDF	0,R0
    // asm 00003693: 	CALL	MOVE_PLAYERS_LETTERS
    // asm 00003694: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_PLAYERS_PLATE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
#define FIRST_BOLT 0x30

void INI_PLAYERS_BOLTS(void)
{
    // asm 00003695: 	LDI	*+AR7(PLACE),R1	;Calculate the first bolts object number
    // asm 00003696: 	MPYI	4,R1
    // asm 00003697: 	ADDI	FIRST_BOLT,R1
    // asm 00003698: 	LDI	3,AR5
    // asm 00003699: IPB1
    // asm 00003699: 	CALL	OBJ_FIND
    // asm 0000369A: 	BC	IPBX
    // asm 0000369B: 	FLOAT	-2500,R0
    // asm 0000369C: 	STF	R0,*+AR0(OPOSZ)
    // asm 0000369D: 	ADDI	1,R1
    // asm 0000369E: 	DBU	AR5,IPB1
IPBX:
    // asm 0000369F: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INI_PLAYERS_BOLTS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*PROC 	FLY BOLT
*
*/
void FLY_BOLT(void)
{
    // asm 000036A0: 	SONDFX	DD2
    // asm 000036A2: 	LDI	15,AR5
    // asm 000036A3: FBL
    // asm 000036A3: 	FLOAT	1000,R1
    // asm 000036A4: 	LDF	*+AR4(OPOSZ),R3
    // asm 000036A5: 	SUBF	R3,R1
    // asm 000036A6: 	MPYF	0.24,R1
    // asm 000036A7: 	ADDF	*+AR4(OPOSZ),R1
    // asm 000036A8: 	STF	R1,*+AR4(OPOSZ)
    // asm 000036A9: 	LDF	*+AR4(ORADZ),R2		;Now spin the little fckers
    // asm 000036AA: 	ADDF	0.0873,R2
    // asm 000036AB: 	STF	R2,*+AR4(ORADZ)
    // asm 000036AC: 	LDI	AR4,AR2
    // asm 000036AD: 	ADDI	OMATRIX,AR2
    // asm 000036AE: 	CALL	FIND_ZMATRIX
    // asm 000036AF: 	SLEEP	1
    // asm 000036B1: 	DBU	AR5,FBL
    // asm 000036B2: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLY_BOLT", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*Below Is code That is shared by both the High score entry and display
*
*
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*/

/*
*----------------------------------------------------------------------------
*	OBJ_FIND	Finds the object in the Active list
*	PARAMETERS
*	R1 =	ID of the object
*	RETURNS
*	AR0		Points to the first object in the list with that ID
*	CARRY = SET if not found
*/

void OBJ_FIND(void)
{
    // asm 000036B3: 	PUSH	R0
    // asm 000036B4: 	PUSHF	R0
    // asm 000036B5: 	PUSH	AR5
    // asm 000036B6: 	LDI	@OACTIVEI,AR0
    // asm 000036B7: 	LDI	*AR0,AR5
OFIND:
    // asm 000036B8: LDI	AR5,R0		;Search for the plate
    // asm 000036B9: 	BZ	OH_NO
    // asm 000036BA: 	LDI	R0,AR0
    // asm 000036BB: 	LDI	*AR0,AR5
    // asm 000036BC: 	LDI	*+AR0(OID),R0
    // asm 000036BD: 	AND	0FFh,R0
    // asm 000036BE: 	CMPI	R1,R0
    // asm 000036BF: 	BNE	OFIND
    // asm 000036C0: 	CLRC			;GOT IT
O_FOUND:
    // asm 000036C1: 	POP	AR5
    // asm 000036C2: 	POPF	R0
    // asm 000036C3: 	POP	R0
    // asm 000036C4: 	RETS
OH_NO:
    // asm 000036C5: 	SETC
    // asm 000036C6: 	BR	O_FOUND		;OBJECT NOT found
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FIND", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*	OBJ_FIND_NEXT	Find the next instance of the object
*	PARAMETERS
*	AR0		The First instance of the object.
*	RETURNS
*	AR0		Points to the next object in the list with that ID
*	CARRY = SET if not found
*/

void OBJ_FIND_NEXT(void)
{
    // asm 000036C7: 	PUSH	R0
    // asm 000036C8: 	PUSHF	R0
    // asm 000036C9: 	PUSH	AR5
    // asm 000036CA: 	LDI	*AR0,AR5
OFN:
    // asm 000036CB: LDI	AR5,R0
    // asm 000036CC: 	LDI	R0,AR0
    // asm 000036CD: 	BZ	OFNX		;NONE FOUND. This routine is passive
    // asm 000036CE: 	LDI	*AR0,AR5
    // asm 000036CF: 	LDI	*+AR0(OID),R0
    // asm 000036D0: 	AND	0FFh,R0
    // asm 000036D1: 	CMPI	R1,R0
    // asm 000036D2: 	BNE	OFN
OFNX:
    // asm 000036D3: 	CLRC
    // asm 000036D4: 	CMPI	0,AR0
    // asm 000036D5: 	BNE	OFNX1
    // asm 000036D6: 	SETC
OFNX1:
    // asm 000036D7: 	POP	AR5
    // asm 000036D8: 	POPF	R0
    // asm 000036D9: 	POP	R0
    // asm 000036DA: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FIND_NEXT", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*	OBJ_GFIND	Finds the FIRST object in the GROUP on the Active list
*	PARAMETERS
*	R1 =	BIT in the ID that determines the group
*	RETURNS
*	AR0		Points to the first object in the list with that ID
*	CARRY = SET if not found
*/

void OBJ_GFIND(void)
{
    // asm 000036DB: 	PUSH	R0
    // asm 000036DC: 	PUSHF	R0
    // asm 000036DD: 	PUSH	AR5
    // asm 000036DE: 	LDI	@OACTIVEI,AR0
    // asm 000036DF: 	LDI	*AR0,AR5
OGFIND:
    // asm 000036E0: LDI	AR5,R0		;Search for the plate
    // asm 000036E1: 	BZ	OH_GNO
    // asm 000036E2: 	LDI	R0,AR0
    // asm 000036E3: 	LDI	*AR0,AR5
    // asm 000036E4: 	LDI	*+AR0(OID),R0
    // asm 000036E5: 	TSTB	R1,R0
    // asm 000036E6: 	BZ	OGFIND
    // asm 000036E7: 	CLRC
    // asm 000036E8: O_GFOUND			;GOT IT
    // asm 000036E8: 	POP	AR5
    // asm 000036E9: 	POPF	R0
    // asm 000036EA: 	POP	R0
    // asm 000036EB: 	RETS
OH_GNO:
    // asm 000036EC: 	SETC
    // asm 000036ED: 	LDI	0,AR0
    // asm 000036EE: 	BR	O_GFOUND		;OBJECT NOT found
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_GFIND", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*	OBJ_GFIND_NEXT	Find the next instance of the GROUP
*	PARAMETERS
*	AR0		The First instance of the object.
*	R1 =	BIT in the ID that determines the group
*
*	RETURNS
*	AR0		Points to the next object in the list with that ID
*	CARRY = SET if not found
*/

void OBJ_GFIND_NEXT(void)
{
    // asm 000036EF: 	PUSH	R0
    // asm 000036F0: 	PUSHF	R0
    // asm 000036F1: 	PUSH	AR5
    // asm 000036F2: 	LDI	*AR0,AR5
OGFN:
    // asm 000036F3: LDI	AR5,R0
    // asm 000036F4: 	LDI	R0,AR0
    // asm 000036F5: 	BZ	OGFNX		;NONE FOUND. This routine is passive
    // asm 000036F6: 	LDI	*AR0,AR5
    // asm 000036F7: 	LDI	*+AR0(OID),R0
    // asm 000036F8: 	TSTB	R1,R0
    // asm 000036F9: 	BZ	OGFN
OGFNX:
    // asm 000036FA: 	CLRC
    // asm 000036FB: 	CMPI	0,AR0
    // asm 000036FC: 	BNE	OGFNX1
    // asm 000036FD: 	SETC
OGFNX1:
    // asm 000036FE: 	POP	AR5
    // asm 000036FF: 	POPF	R0
    // asm 00003700: 	POP	R0
    // asm 00003701: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_GFIND_NEXT", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*This code can be deleted after I can change the ID numbers for the PRESS Group
* PARAMETERS AR3 = packed list of object ID's to tag
*	R2  = What to tage them with. NOTE R2 will be orred with the ID
*/
void OBJ_TAG(void)
{
OTAG0:
    // asm 00003702: 	LDI	*AR3++,R1	;Get ID looking for
    // asm 00003703: 	CMPI	0FFh,R1		;Is it a range of ID's?
    // asm 00003704: 	BLE	OTAG
    // asm 00003705: 	LDI	R1,R0		;YES, Decode the end
    // asm 00003706: 	AND	0FFh,R0
    // asm 00003707: 	LSH	-8,R1		;Decode the start
OTAG1:
    // asm 00003708: 	CALL	OBJ_TAGALL	;Remove the range of objects
    // asm 00003709: 	ADDI	1,R1
    // asm 0000370A: 	CMPI	R1,R0
    // asm 0000370B: 	BGE	OTAG1
    // asm 0000370C: 	BR	OTAG0
OTAG:
    // asm 0000370D: 	CMPI	0,R1
    // asm 0000370E: 	BEQ	OTAGX
    // asm 0000370F: 	CALL	OBJ_TAGALL
    // asm 00003710: 	BR	OTAG0
OTAGX:
    // asm 00003711: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_TAG", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*This code can be deleted after I can change the ID numbers for the PRESS Group
*	R1 = Object looking for
*	R2  = What to tage them with. NOTE R2 will be orred with the ID
*/
void OBJ_TAGALL(void)
{
    // asm 00003712: 	CALL	OBJ_FIND
    // asm 00003713: 	BC	OTAX
    // asm 00003714: 	LDI	*+AR0(OID),R3
    // asm 00003715: 	OR	R2,R3
    // asm 00003716: 	STI	R3,*+AR0(OID)
OTA1:
    // asm 00003717: 	CALL	OBJ_FIND_NEXT
    // asm 00003718: 	BC	OTAX
    // asm 00003719: 	LDI	*+AR0(OID),R3
    // asm 0000371A: 	OR	R2,R3
    // asm 0000371B: 	STI	R3,*+AR0(OID)
    // asm 0000371C: 	BR	OTA1
OTAX:
    // asm 0000371D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_TAGALL", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*OBJ_MOVY_GROUP
*	PARAMETERS	R1 = Bit of OID to search for
*	PARAMETERS	R2 = FLOAT amount to move YPOS
*
*/
void OBJ_MOVY_GROUP(void)
{
    // asm 0000371E: 	PUSH	R3
    // asm 0000371F: 	PUSHF	R3
    // asm 00003720: 	CALL	OBJ_GFIND
    // asm 00003721: 	BC	OMYG
    // asm 00003722: 	LDF	*+AR0(OPOSY),R3
    // asm 00003723: 	ADDF	R2,R3
    // asm 00003724: 	STF	R3,*+AR0(OPOSY)
OMYG1:
    // asm 00003725: 	CALL	OBJ_GFIND_NEXT
    // asm 00003726: 	BC	OMYG
    // asm 00003727: 	LDF	*+AR0(OPOSY),R3
    // asm 00003728: 	ADDF	R2,R3
    // asm 00003729: 	STF	R3,*+AR0(OPOSY)
    // asm 0000372A: 	BR	OMYG1
OMYG:
    // asm 0000372B: 	POPF	R3
    // asm 0000372C: 	POP	R3
    // asm 0000372D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_MOVY_GROUP", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*OBJ_DEL_GROUP
*	PARAMETERS	R1 = Bit of OID to search for
*
*
*/
void OBJ_DEL_GROUP(void)
{
    // asm 0000372E: 	CALL	OBJ_GFIND
    // asm 0000372F: 	BC	ODG
    // asm 00003730: 	LDI	AR0,AR2
    // asm 00003731: 	CALL	OBJ_PULL
ODG1:
    // asm 00003732: 	CALL	OBJ_GFIND_NEXT
    // asm 00003733: 	BC	ODG
    // asm 00003734: 	LDI	AR0,AR2
    // asm 00003735: 	CALL	OBJ_PULL
    // asm 00003736: 	BR	ODG1
ODG:
    // asm 00003737: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_DEL_GROUP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------
void FIND_ALL_PLATES(void)
{
    // asm 00003738: 	LDI	15,R0			;HE WILL NEVER GET TO THIS PLACE
    // asm 00003739: 	STI	R0,*+AR7(PLACE)
    // asm 0000373A: 	CALL	FIND_PLATES
    // asm 0000373B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_ALL_PLATES", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*FIND_PLATES Creates the objects for the name and the score
*
*
*
*/

#define NUM_PLATES 9
#define FIRST_PLATE 0x20

void FIND_PLATES(void)
{
    // asm 0000373C: 	PUSH	AR5
    // asm 0000373D: 	LDI	*+AR7(RACE_NUMBER),R6		;Race number
    // asm 0000373E: 	LDI	0,R7				;Entry number
    // asm 0000373F: 	CALL	GET_TABLE_ADDR
    // asm 00003740: 	STI	AR2,*+AR7(HSPOINTER)
    // asm 00003741: 	LDI	0,AR5
MP0:
    // asm 00003742: 	CMPI	*+AR7(PLACE),AR5
    // asm 00003743: 	BEQ	MP1		;Skip creating the letters and score for the player
    // asm 00003744: 	LDI	*+AR7(HSPOINTER),AR2
    // asm 00003745: 	CALL	TABLE_ENTRY_READ
    // asm 00003746: 	STI	AR2,*+AR7(HSPOINTER)
    // asm 00003747: 	STI	R1,*+AR7(LETTER0)
    // asm 00003748: 	STI	R2,*+AR7(LETTER1)
    // asm 00003749: 	STI	R3,*+AR7(LETTER2)
    // asm 0000374A: 	CALL	MAKE_TIME
    // asm 0000374B: 	LDI	AR5,R1
    // asm 0000374C: 	ADDI	FIRST_PLATE,R1
    // asm 0000374D: 	CALL	OBJ_FIND	;Get the pointer to the plate
    // asm 0000374E: 	LDI	AR5,R1
    // asm 0000374F: 	CALL	CREATE_LETTERS
MP2:
    // asm 00003750: 	ADDI	1,AR5
    // asm 00003751: 	CMPI	NUM_PLATES,AR5
    // asm 00003752: 	BLE	MP0
    // asm 00003753: 	POP	AR5
    // asm 00003754: 	RETS
    // asm 00003755: MP1				;Skip this plate
    // asm 00003755: 	LDI	*+AR7(HSPOINTER),AR2
    // asm 00003756: 	CALL	TABLE_ENTRY_READ
    // asm 00003757: 	STI	AR2,*+AR7(HSPOINTER)
    // asm 00003758: 	LDI	AR5,R1
    // asm 00003759: 	ADDI	FIRST_PLATE,R1
    // asm 0000375A: 	CALL	OBJ_FIND	;Get the pointer to the plate
    // asm 0000375B: 	STI	AR0,*+AR7(MISPLATEOBJ)	;Save its pointer for latter use
    // asm 0000375C: 	LDI	@SCORE,R0
    // asm 0000375D: 	CALL	MAKE_TIME
    // asm 0000375E: 	BR	MP2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_PLATES", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*MAKE_NUMBERS
*	R0	= INTEGER SCORE
*	AR5	= PLACE
*
*
*/
#define FIRST_NUMBER 0x10

void MAKE_NUMBERS(void)
{
    // asm 0000375F: 	LDI	R0,R2
    // asm 00003760: 	LDI	AR7,AR2
    // asm 00003761: 	ADDI	TEMP_STR,AR2
    // asm 00003762: 	CALL	_itoa
    // asm 00003763: 	CALL	FORMAT_NUM
    // asm 00003764: 	LDI	AR7,AR4
    // asm 00003765: 	ADDI	TEMP_STR,AR4
    // asm 00003766: 	LDI	FIRST_NUMBER,R1
    // asm 00003767: 	ADDI	AR5,R1			;AR5 = the place
    // asm 00003768: 	CALL	OBJ_FIND
    // asm 00003769: 	LDI	*AR4++,IR0
    // asm 0000376A: 	LDI	@NUMTABI,AR1
    // asm 0000376B: 	LDI	*+AR1(IR0),R0
    // asm 0000376C: 	STI	R0,*+AR0(OROMDATA)	;Replace it with the proper number
    // asm 0000376D: 	LDI	HIGH_SCORE_GROUP,R0		;Make this part of the High Score group
    // asm 0000376E: 	STI	R0,*+AR0(OID)
    // asm 0000376F: 	LDI	5,AR6		;Now make 6 more
    // asm 00003770: MNLOOP
    // asm 00003770: 	LDI	AR0,AR1
    // asm 00003771: 	LDI	*AR4++,IR0
    // asm 00003772: 	LDI	@NUMTABI,AR0
    // asm 00003773: 	LDI	*+AR0(IR0),AR2
    // asm 00003774: 	CALL	OBJ_GETE
    // asm 00003775: 	LDI	HIGH_SCORE_GROUP,R0		;Make this part of the High Score group
    // asm 00003776: 	STI	R0,*+AR0(OID)
    // asm 00003777: 	LDF	*+AR1(OPOSX),R0
    // asm 00003778: 	FLOAT	-145,R1			;WIDTH OF ONE NUMBER
    // asm 00003779: 	ADDF	R1,R0
    // asm 0000377A: 	STF	R0,*+AR0(OPOSX)
    // asm 0000377B: 	LDF	*+AR1(OPOSY),R0
    // asm 0000377C: 	STF	R0,*+AR0(OPOSY)
    // asm 0000377D: 	LDF	*+AR1(OPOSZ),R0
    // asm 0000377E: 	STF	R0,*+AR0(OPOSZ)
    // asm 0000377F: 	LDF	0,R2
    // asm 00003780: 	STF	R2,*+AR0(ORADX)
    // asm 00003781: 	LDI	AR0,AR2
    // asm 00003782: 	ADDI	OMATRIX,AR2
    // asm 00003783: 	CALL	FIND_XMATRIX
    // asm 00003784: 	LDI	AR0,AR2
    // asm 00003785: 	CALL	OBJ_INSERT
    // asm 00003786: 	DBU	AR6,MNLOOP
    // asm 00003787: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_NUMBERS", 0, 0);
    UNIMPL();
}

/* asm: LONGEST_TIME	.word	198000 */
int LONGEST_TIME = 198000;

void MAKE_TIME(void)
{
    // asm 00003788: 	CMPI	0,R0			;Range check time for bogus values
    // asm 00003789: 	LDILT	0,R0
    // asm 0000378A: 	LDI	@LONGEST_TIME,R1
    // asm 0000378B: 	CMPI	R1,R0
    // asm 0000378C: 	LDIGT	R1,R0
    // asm 0000378D: 	CALL	CVTTIME
    // asm 0000378E: 	MPYI	100,R1
    // asm 0000378F: 	ADDI	R1,R0
    // asm 00003790: 	MPYI	10000,R2
    // asm 00003791: 	ADDI	R0,R2
    // ;	CALL	CONVERT_TIME
    // asm 00003792: 	LDI	AR7,AR2
    // asm 00003793: 	ADDI	TEMP_STR,AR2
    // asm 00003794: 	CALL	_itoa
    // asm 00003795: 	CALL	FORMAT_NUM
    // asm 00003796: 	LDI	AR7,AR4
    // asm 00003797: 	ADDI	TEMP_STR,AR4
    // asm 00003798: 	LDI	FIRST_NUMBER,R1
    // asm 00003799: 	ADDI	AR5,R1			;AR5 = the place
    // asm 0000379A: 	CALL	OBJ_FIND
    // asm 0000379B: 	LDI	*AR4++,IR0
    // asm 0000379C: 	LDI	@NUMTABI,AR1
    // asm 0000379D: 	LDI	*+AR1(IR0),R0
    // asm 0000379E: 	STI	R0,*+AR0(OROMDATA)	;Replace it with the proper number
    // asm 0000379F: 	LDI	HIGH_SCORE_GROUP,R0		;Make this part of the High Score group
    // asm 000037A0: 	STI	R0,*+AR0(OID)
    // asm 000037A1: 	LDI	6,AR6		;Now make 6 more
    // asm 000037A2: MTLOOP
    // asm 000037A2: 	LDI	AR0,AR1
    // asm 000037A3: 	FLOAT	-145,R1			;WIDTH OF ONE NUMBER
    // asm 000037A4: 	CMPI	5,AR6
    // asm 000037A5: 	BEQ	MT1
    // asm 000037A6: 	CMPI	2,AR6
    // asm 000037A7: 	BNE	MT2
MT1:
    // asm 000037A8: 	LDL	dcol,AR2
    // asm 000037A9: 	CALL	OBJ_GETE
    // asm 000037AA: 	FLOAT	-82,R1			;WIDTH OF ONE COLON
    // asm 000037AB: 	BR	MT3
MT2:
    // asm 000037AC: 	LDI	*AR4++,IR0
    // asm 000037AD: 	LDI	@NUMTABI,AR0
    // asm 000037AE: 	LDI	*+AR0(IR0),AR2
    // asm 000037AF: 	CALL	OBJ_GETE
MT3:
    // asm 000037B0: 	LDI	HIGH_SCORE_GROUP,R0		;Make this part of the High Score group
    // asm 000037B1: 	STI	R0,*+AR0(OID)
    // asm 000037B2: 	LDF	*+AR1(OPOSX),R0
    // asm 000037B3: 	ADDF	R1,R0
    // asm 000037B4: 	STF	R0,*+AR0(OPOSX)
    // asm 000037B5: 	LDF	*+AR1(OPOSY),R0
    // asm 000037B6: 	STF	R0,*+AR0(OPOSY)
    // asm 000037B7: 	LDF	*+AR1(OPOSZ),R0
    // asm 000037B8: 	STF	R0,*+AR0(OPOSZ)
    // asm 000037B9: 	LDF	0,R2
    // asm 000037BA: 	STF	R2,*+AR0(ORADX)
    // asm 000037BB: 	LDI	AR0,AR2
    // asm 000037BC: 	ADDI	OMATRIX,AR2
    // asm 000037BD: 	CALL	FIND_XMATRIX
    // asm 000037BE: 	LDI	AR0,AR2
    // asm 000037BF: 	CALL	OBJ_INSERT
    // asm 000037C0: 	DBU	AR6,MTLOOP
    // asm 000037C1: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_TIME", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*READS ASCI NUMBER and reformats it to 7 digit display
*	AR2 = string
*/
void FORMAT_NUM(void)
{
    // asm 000037C2: 	LDI	0,R1
    // asm 000037C3: 	LDI	0,R2
    // asm 000037C4: 	LDI	AR2,AR1
FORMN_LOOP:
    // asm 000037C5: 	CMPI	0,R2
    // asm 000037C6: 	BNE	FORMNA
    // asm 000037C7: 	LDI	*AR2++,R0
FORMNA:
    // asm 000037C8: LDI	R0,R3
    // asm 000037C9: 	LSH	R2,R3
    // asm 000037CA: 	SUBI	8,R2
    // asm 000037CB: 	CMPI	-24,R2
    // asm 000037CC: 	LDILT	0,R2
    // asm 000037CD: 	AND	0FFh,R3
    // asm 000037CE: 	CMPI	MINUS_CHAR,R3
    // asm 000037CF: 	BEQ	FORMN			;Disregard minus sign
    // asm 000037D0: 	CMPI	0,R3
    // asm 000037D1: 	BEQ	FN_PAD			;Reached end
    // asm 000037D2: 	SUBI	'0',R3
    // asm 000037D3: 	PUSH	R3
    // asm 000037D4: 	ADDI	1,R1
FORMN:
    // asm 000037D5: 	CMPI	6,R1
    // asm 000037D6: 	BLE	FORMN_LOOP
FN_PAD:
    // asm 000037D7: 	SUBI	1,R1
    // asm 000037D8: 	LDI	R1,RC
    // asm 000037D9: 	RPTB	FORMN1
    // asm 000037DA: 	POP	R0
FORMN1:
    // asm 000037DB: STI	R0,*AR1++
    // asm 000037DC: 	CMPI	6,R1
    // asm 000037DD: 	BEQ	FORMNX
    // asm 000037DE: 	LDI	0,R0
    // asm 000037DF: 	NEGI	R1
    // asm 000037E0: 	ADDI	6,R1
    // asm 000037E1: 	BN	FORMNX
    // asm 000037E2: 	LDI	R1,RC
    // asm 000037E3: 	RPTB	FORMN2
FORMN2:
    // asm 000037E4: STI	R0,*AR1++
FORMNX:
    // asm 000037E5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FORMAT_NUM", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*	PARAMETERS	AR0 = POINTER TO PLATE that owns this
*		LETTER0-LETTER2 = THE three letters
*		R1  = Place the player is in
*/
#define LETTER_SIZEX 120
#define LETTER_XOFF (-120)
#define LETTER_YOFF (-12)

void CREATE_LETTERS(void)
{
    // asm 000037E6: 	LDI	AR7,AR3
    // asm 000037E7: 	ADDI	LETTER0,AR3
    // asm 000037E8: 	ADDI	16,R1
    // asm 000037E9: 	LDI	1,R2
    // asm 000037EA: 	LSH	R1,R2
    // asm 000037EB: 	OR	HIGH_SCORE_GROUP,R2		;Make this part of the High Score group
    // asm 000037EC: 	LDI	AR0,AR1
    // asm 000037ED: 	LDF	0,R4
CRLLOOP:
    // asm 000037EE: 	LDI	*AR3++,AR2
    // asm 000037EF: 	AND	07Fh,AR2		;EXTRACT the CHAR
    // asm 000037F0: 	CMPI	0,AR2
    // asm 000037F1: 	BEQ	CRLX			;String Zero terminated
    // asm 000037F2: 	CALL	ASCII_TO_OBJ
    // asm 000037F3: 	BC	CRLX				;OUT OF OBJECTS... BAIL
    // asm 000037F4: 	FLOAT	LETTER_SIZEX,R0
    // asm 000037F5: 	MPYF	R4,R0
    // asm 000037F6: 	FLOAT	LETTER_XOFF,R1
    // asm 000037F7: 	ADDF	R1,R0
    // asm 000037F8: 	ADDF	*+AR1(OPOSX),R0
    // asm 000037F9: 	STF	R0,*+AR0(OPOSX)
    // asm 000037FA: 	FLOAT	LETTER_YOFF,R0
    // asm 000037FB: 	ADDF	*+AR1(OPOSY),R0
    // asm 000037FC: 	STF	R0,*+AR0(OPOSY)
    // asm 000037FD: 	LDF	*+AR1(OPOSZ),R0
    // asm 000037FE: 	SUBF	1,R0			;Make sure that it is on top of the plate
    // asm 000037FF: 	STF	R0,*+AR0(OPOSZ)
    // asm 00003800: 	LDI	*+AR7(WHITE_PAL),R0
    // asm 00003801: 	STI	R0,*+AR0(OPAL)
    // asm 00003802: 	STI	R2,*+AR0(OID)
    // asm 00003803: 	PUSH	R2
    // asm 00003804: 	LDF	0,R2
    // asm 00003805: 	LDI	*-AR3(1),AR2
    // asm 00003806: 	CMPI	20h,AR2		;is it a space?
    // asm 00003807: 	LDFEQ	-PI,R2
    // asm 00003808: 	STF	R2,*+AR0(ORADX)
    // asm 00003809: 	LDI	AR0,AR2
    // asm 0000380A: 	ADDI	OMATRIX,AR2
    // asm 0000380B: 	CALL	FIND_XMATRIX
    // asm 0000380C: 	POP	R2
    // asm 0000380D: 	LDI	AR0,AR2
    // asm 0000380E: 	CALL	OBJ_INSERT
    // asm 0000380F: 	ADDF	1,R4
    // asm 00003810: 	CMPF	2,R4
    // asm 00003811: 	BLE	CRLLOOP
CRLX:
    // asm 00003812: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CREATE_LETTERS", 0, 0);
    UNIMPL();
}

void ASCII_TO_OBJ(void)
{
    // asm 00003813: 	CMPI	'0',AR2
    // asm 00003814: 	LDILT	'0',AR2
    // asm 00003815: 	CMPI	'9',AR2
    // asm 00003816: 	BGT	CRLL
    // asm 00003817: 	ADDI	'Z'+2-'0',AR2
    // asm 00003818: 	BR	CRL1
CRLL:
    // asm 00003819: 	CMPI	'Z',AR2
    // asm 0000381A: 	LDIGT	'Z',AR2
CRL1:
    // asm 0000381B: 	SUBI	'A',AR2
    // asm 0000381C: 	CMPI	0,AR2
    // asm 0000381D: 	LDILT	0,AR2
    // asm 0000381E: 	CMPI	LASTCHAR,AR2
    // asm 0000381F: 	LDIGT	LASTCHAR,AR2
    // asm 00003820: 	ADDI	@PLATE_LETTERSI,AR2
    // asm 00003821: 	LDI	*AR2,AR2
    // asm 00003822: 	CALL	OBJ_GETE
    // asm 00003823: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASCII_TO_OBJ", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*	PARAMETERS	AR2 = POINTER TO STRING
*		R2  = Xpos FL
*		R3  = Ypos FL
*		R4  = Zpos FL
*		R6  = ID
*/
#define LETTER3D_SIZEX 80

void PRINT3D(void)
{
    // asm 00003824: 	LDI	AR2,AR1			;I use AR2 as input for continuity with the 2d print
    // asm 00003825: 	LDI	0,R1
    // asm 00003826: 	PUSH	R1			;This will terminate the string being pulled off the stack
    // asm 00003827: 	FLOAT	LETTER3D_SIZEX/2,R5
PR3DFS:
    // asm 00003828: 	LDI	*AR1,R0			;Count the number of letters in this group
    // asm 00003829: 	LSH	R1,R0
    // asm 0000382A: 	ADDI	-8,R1
    // asm 0000382B: 	CMPI	-24,R1
    // asm 0000382C: 	BGE	PR3DA
    // asm 0000382D: 	LDI	0,R1
    // asm 0000382E: 	NOP	*AR1++
PR3DA:
    // asm 0000382F: 	AND	7Fh,R0
    // asm 00003830: 	PUSH	R0
    // asm 00003831: 	ADDI	1,IR0
    // asm 00003832: 	ADDF	R5,R2			;Center the text's Xpos
    // asm 00003833: 	CMPI	0,R0
    // asm 00003834: 	BNE	PR3DFS
    // asm 00003835: 	SUBF	R5,R2			;Correction for zero terminator
    // asm 00003836: 	SUBF	R5,R2			;Correction for first letter
    // asm 00003837: 	POP	AR0			;POP the zero terminator
PR3DLOOP:
    // asm 00003838: 	POP	AR0
    // asm 00003839: 	CMPI	0,AR0			;Not stripped by loop above
    // asm 0000383A: 	BEQ	PR3DX			;String Zero terminated
    // asm 0000383B: 	CMPI	20h,AR0			;if its a space, skip it
    // asm 0000383C: 	BEQ	PR3DNEXT
    // asm 0000383D: 	CMPI	'0',AR0
    // asm 0000383E: 	LDIZ	'O',AR0
    // asm 0000383F: 	CMPI	'1',AR0
    // asm 00003840: 	LDIZ	'I',AR0
    // asm 00003841: 	SUBI	'A',AR0
    // asm 00003842: 	CMPI	0,AR0
    // asm 00003843: 	LDILT	0,AR0
    // asm 00003844: 	CMPI	THREED_END,AR0
    // asm 00003845: 	LDIGT	THREED_END,AR0
    // asm 00003846: 	ADDI	@THREED_LETTERSI,AR0
    // asm 00003847: 	LDI	*AR0,AR2
    // asm 00003848: 	CALL	OBJ_GETE
#if DEBUG
    // asm: 	BC	$
#endif
    // asm 00003849: 	STF	R2,*+AR0(OPOSX)
    // asm 0000384A: 	STF	R3,*+AR0(OPOSY)
    // asm 0000384B: 	STF	R4,*+AR0(OPOSZ)
    // asm 0000384C: 	STI	R6,*+AR0(OID)
    // asm 0000384D: 	PUSHF	R2
    // asm 0000384E: 	LDF	0,R2
    // asm 0000384F: 	LDI	AR0,AR2
    // asm 00003850: 	ADDI	OMATRIX,AR2
    // asm 00003851: 	CALL	FIND_YMATRIX
    // asm 00003852: 	POPF	R2
    // asm 00003853: 	LDI	AR0,AR2
    // asm 00003854: 	CALL	OBJ_INSERT
PR3DNEXT:
    // asm 00003855: 	FLOAT	LETTER3D_SIZEX,R0
    // asm 00003856: 	SUBF	R0,R2
    // asm 00003857: 	BR	PR3DLOOP
PR3DX:
    // asm 00003858: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT3D", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*Below Is code only used to display the high score and is not used during high
*score entry
*
*
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*/

#define HS_ZOOM 40
#define HS_STARTY 650
// ;HS_ENDY 	.set	-280
#define HS_ENDY (-210)
#define HS_YDIFF ((HS_ENDY-HS_STARTY)/HS_ZOOM)
#define HS_STARTZ (-4200)
#define HS_ENDZ (-1960) //-800
#define HS_ZDIFF ((HS_ENDZ-HS_STARTZ)/HS_ZOOM)

void DISPLAY_HIGH_SCORES(void)
{
    // asm 00003859: 	LDL	scroll_white,AR2
    // asm 0000385A: 	CALL	PAL_ALLOC_RAW
    // asm 0000385B: 	LDL	plate_darkp,AR2
    // asm 0000385C: 	CALL	PAL_ALLOC_RAW
    // asm 0000385D: 	LDL	plate_medp,AR2
    // asm 0000385E: 	CALL	PAL_ALLOC_RAW
    // asm 0000385F: 	LDL	plate_lightp,AR2
    // asm 00003860: 	CALL	PAL_ALLOC_RAW
    // asm 00003861: 	LDL	plate_lightp1,AR2
    // asm 00003862: 	CALL	PAL_ALLOC_RAW
    // asm 00003863: 	LDL	press_PALETTES,AR2	;Load in the palettes
    // asm 00003864: 	CALL	alloc_section
    // asm 00003865: 	LDI	0,R0			;IGNORE PRVIOUS BUTTON PRESSES
    // asm 00003866: 	STI	R0,@RADIO_HS_SWITCH
    // asm 00003867: 	STI	R0,@RADIO_HS_SHADOW
    // asm 00003868: 	LDI	@ATTRWAVE,AR5
DHSLOOP:
    // asm 00003869: 	CLRI	AR0
    // ;	LDP	@SWITCH3
    // ;	NOT	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // ;	LDI	*AR0,R2			;Loff
    // ;	SETDP
    // ;	AND	SW_RADIO_H,R0
    // ;	BNZ	DHSLOOP		;Wait for radio button to come up
    // asm 0000386A: 	LDI	AR5,AR6
    // asm 0000386B: 	CREATE	DISPLAY_HS,UTIL_C|DISPLAYHS_T
    // asm 0000386E: 	LDI	AR0,AR4			;Save PROC incase we need to kill it latter
    // asm 0000386F: 	LDI	30*7,AR6		;Sleep 7 seconds
    // asm 00003870: DHSWAIT
    // asm 00003870: 	SLEEP	1
    // ;	CLRI	AR0
    // ;	LDP	@SWITCH3
    // ;	NOT	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // ;	LDI	*AR0,R2			;Loff
    // ;	SETDP
    // ;	AND	SW_RADIO_H,R0
    // ;	BNZ	DHSNEXT	;if radio button pressed skip to next HS
    // asm 00003872: 	LDI	@RADIO_HS_SWITCH,R0
    // asm 00003873: 	CMPI	@RADIO_HS_SHADOW,R0	;Edge trigger
    // asm 00003874: 	BZ	DHSNOBUT
    // asm 00003875: 	STI	R0,@RADIO_HS_SHADOW
    // asm 00003876: 	CMPI	-1,R0			;First trigger
    // asm 00003877: 	LDIEQ	-1,AR5			;First trigger set level = -1 (DHSNEXT WILL INC)
    // asm 00003878: 	BR	DHSNEXT
DHSNOBUT:
    // asm 00003879: 	DBU	AR6,DHSWAIT
    // asm 0000387A: 	CALL	OBJ_INIT	;initialize object system (ERASE OLD OBJECTS)
    // asm 0000387B: 	ADDI	1,AR5
    // asm 0000387C: 	CMPI	15,AR5
    // asm 0000387D: 	BGE	DHSLOOPX
    // asm 0000387E: 	TSTB	1,AR5
    // asm 0000387F: 	BNE	DHSLOOP
DHSLOOPX:
    // asm 00003880: 	BR	CYCLE_ATTR
DHSNEXT:
    // asm 00003881: 	ADDI	1,AR5
    // asm 00003882: 	CMPI	15,AR5
    // asm 00003883: 	BGE	DHSLOOPX
    // asm 00003884: 	LDI	UTIL_C|DISPLAYHS_T,R0	;KILL THE OLD PROCS OFF
    // asm 00003885: 	LDI	CLASS_M|TYPE_M,R1
    // asm 00003886: 	CALL	PRC_KILLALL
    // asm 00003887: 	CALL	OBJ_INIT	;initialize object system (ERASE OLD OBJECTS)
    // asm 00003888: 	BR	DHSLOOP
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_HIGH_SCORES", 0, 0);
    UNIMPL();
}

void DISPLAY_HS(void)
{
    // asm 00003889: 	STI	AR6,*+AR7(RACE_NUMBER)		;Passed by parent proc
    // asm 0000388A: 	LDL	scroll_white,AR2
    // asm 0000388B: 	CALL	PAL_FIND_RAW
    // asm 0000388C: 	STI	R0,*+AR7(WHITE_PAL)
    // asm 0000388D: 	LDI	0,R0
    // asm 0000388E: 	STI	R0,@NOAERASE
    // asm 0000388F: 	LDP	@_CAMERAPOS+X		;Set the initial camera position
    // asm 00003890: 	FLOAT	-4700,R0
    // asm 00003891: 	STF	R0,@_CAMERAPOS+X
    // asm 00003892: 	FLOAT	HS_STARTY,R0
    // asm 00003893: 	STF	R0,@_CAMERAPOS+Y
    // asm 00003894: 	FLOAT	HS_STARTZ,R0
    // asm 00003895: 	STF	R0,@_CAMERAPOS+Z
    // asm 00003896: 	CLRF	R2
    // asm 00003897: 	STF	R2,@_CAMERARAD+Y
    // asm 00003898: 	SETDP
    // asm 00003899: 	LDI	@CAMERAMATRIXI,AR2
    // asm 0000389A: 	CALL	FIND_YMATRIX
    // asm 0000389B: 	LDL	press_grp,AR2		;Load the objects
    // asm 0000389C: 	CALL	LOAD_SINGLE_SECTION
    // asm 0000389D: 	CALL	DELETE_PRESS_OBJECTS	;Loose the extra stuff used for the name entry
    // asm 0000389E: 	CALL	FIND_ALL_PLATES
    // asm 0000389F: 	CALL	FIX_PLATES		;Init the plates zpos for the fly in
    // asm 000038A0: 	FLOAT	120,R2			;Adjust the plates y position for the new Marqee
    // asm 000038A1: 	LDI	HIGH_SCORE_GROUP,R1		;High score group
    // asm 000038A2: 	CALL	OBJ_MOVY_GROUP
    // asm 000038A3: 	LDI	9h,R1
    // asm 000038A4: 	CALL	OBJ_FIND
    // asm 000038A5: 	LDI	AR0,AR2
    // asm 000038A6: 	CALL	OBJ_PULL
    // asm 000038A7: 	CALL	DISPLAY_HSTEXT
    // asm 000038A8: 	CALL	RESCAN			;Make sure all these changes are shown the next display
    // asm 000038A9: 	LDI	*+AR7(RACE_NUMBER),R4
    // asm 000038AA: 	CREATEC	FLASH_LETTERS_PROC,UTIL_C|DISPLAYHS_T|FLASH_ST
    // asm 000038AD: 	STI	AR0,*+AR7(FLASH_PROC)
DHS0:
    // asm 000038AE: 	SLEEP	1
    // asm 000038B0: 	FLOAT	250,R0
    // asm 000038B1: 	CALL	FLY_PLATES		;Fly the plates onto the back wall
    // asm 000038B2: 	FLOAT	998,R1
    // asm 000038B3: 	CMPF	R1,R3
    // asm 000038B4: 	BLT	DHS0
    // asm 000038B5: 	SLEEP	30*2
    // asm 000038B7: 	LDI	HS_ZOOM-1,AR6
    // asm 000038B8: DHS1
    // asm 000038B8: 	SLEEP	1
    // asm 000038BA: 	FLOAT	HS_ZDIFF,R1		;Zoom the camera up close
    // asm 000038BB: 	LDP	@_CAMERAPOS
    // asm 000038BC: 	FLOAT	HS_YDIFF,R1		;Pan the y so that the marqee is at the top
    // asm 000038BD: 	LDF	@_CAMERAPOS+Y,R0
    // asm 000038BE: 	ADDF	R1,R0
    // asm 000038BF: 	STF	R0,@_CAMERAPOS+Y
    // asm 000038C0: 	FLOAT	HS_ZDIFF,R1
    // asm 000038C1: 	LDF	@_CAMERAPOS+Z,R0
    // asm 000038C2: 	ADDF	R1,R0
    // asm 000038C3: 	STF	R0,@_CAMERAPOS+Z
    // asm 000038C4: 	SETDP
    // asm 000038C5: 	DBU	AR6,DHS1
    // asm 000038C6: 	SLEEP	30*2			;Pause to see last few names
    // asm 000038C8: 	LDI	*+AR7(FLASH_PROC),AR2
    // asm 000038C9: 	CALL	PRC_KILL
    // asm 000038CA: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_HS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*PROC
*R4 = race number
*/
/* asm: FLASH_PALS */
/* asm: 	.word	plate_medp,plate_lightp,plate_lightp1,plate_lightp,-1 */
int FLASH_PALS[] = {
    plate_medp, plate_lightp, plate_lightp1, plate_lightp, -1,
};

void FLASH_LETTERS_PROC(void)
{
#if FLASH_ON == 1
    // asm 000038CD: 	LDI	R4,R1
    // asm 000038CE: 	CALL	CHECK_LASTHS
    // asm 000038CF: 	CMPI	-1,R0
    // asm 000038D0: 	BEQ	FLASH_LOCK
    // asm 000038D1: 	ADDI	16,R0
    // asm 000038D2: 	LDI	1,R4
    // asm 000038D3: 	LSH	R0,R4
    // asm 000038D4: 	OR	HIGH_SCORE_GROUP,R4		;Make this part of the High Score group
    // asm 000038D5: 	LDI	@FLASH_PALSI,AR6
FLASH_LOOP:
    // asm 000038D6: 	LDI	*AR6,R0
    // asm 000038D7: 	LDIN	@FLASH_PALSI,AR6
    // asm 000038D8: 	LDIN	*AR6++,R0			;THIS will increment allways
    // asm 000038D9: 	PUSH	R0
    // asm 000038DA: 	LDI	200,AR2
    // asm 000038DB: 	CALL	RANDPER
    // asm 000038DC: 	POP	R0
    // asm 000038DD: 	LDIC	@scroll_whiteI,R0
    // asm 000038DE: 	CALL	FLASH_LETTERS
    // asm 000038DF: 	SLEEP	6
    // asm 000038E1: 	BR	FLASH_LOOP
#endif
FLASH_LOCK:
    // asm 000038E2: 	SLEEP	1
    // asm 000038E4: 	BR	FLASH_LOCK
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_LETTERS_PROC", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*R0 OPAL
*R4 OID
*/
void FLASH_LETTERS(void)
{
    // asm 000038E5: 	LDI	R0,AR2
    // asm 000038E6: 	CALL	PAL_FIND_RAW
    // asm 000038E7: 	LDI	R4,AR2
    // asm 000038E8: 	CALL	OBJ_FIND_FIRST
    // asm 000038E9: 	BNC	FLASHX
FLASH_LP:
    // asm 000038EA: 	STI	R0,*+AR0(OPAL)
    // asm 000038EB: 	LDI	AR2,R1
    // asm 000038EC: 	CALL	FIND_NEXT_OBJ
    // asm 000038ED: 	BNC	FLASH_LP
FLASHX:
    // asm 000038EE: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_LETTERS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*REMOVES the objects in the list of ID's
*NOTE if the value of the ID is > FF it will eliminate the range (inclusive)
*
*/

/* asm: DELIST		.word	808Bh,0104h,3057h,0 */
/* asm: 	 */
/* asm: 	 */
int DELIST[] = {
    0x808B, 0x0104, 0x3057, 0,
};

void DELETE_PRESS_OBJECTS(void)
{
    // asm 000038F3: 	LDI	@DELISTI,AR3	;TAG them with 100h
    // asm 000038F4: 	LDI	100h,R2
    // asm 000038F5: 	CALL	OBJ_TAG
    // asm 000038F6: 	LDI	100h,R1		;Now Bag them
    // asm 000038F7: 	CALL	OBJ_DEL_GROUP
    // asm 000038F8: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELETE_PRESS_OBJECTS", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*	FIX_PLATES
*
*/
void FIX_PLATES(void)
{
    // asm 000038F9: 	PUSH	AR5
    // asm 000038FA: 	FLOAT	-5000,R0		;Initial Z offset
    // asm 000038FB: 	LDI	0,AR5
    // asm 000038FC: 	LDI	1h,R2
    // asm 000038FD: 	LSH	16,R2
FIXPL:
    // asm 000038FE: 	LDI	AR5,R1
    // asm 000038FF: 	ADDI	FIRST_PLATE,R1
    // asm 00003900: 	CALL	OBJ_FIND
    // asm 00003901: 	LDF	*+AR0(OPOSZ),R3		;Offset the plates Z
    // asm 00003902: 	ADDF	R0,R3
    // asm 00003903: 	STF	R3,*+AR0(OPOSZ)
    // asm 00003904: 	SUBF	1,R3
    // asm 00003905: FIXPL1
    // asm 00003905: 	LDI	R2,R1
    // asm 00003906: 	LSH	1,R2
    // asm 00003907: 	CALL	OBJ_GFIND
    // asm 00003908: 	BC	FIXPL3			;No letters on this plate, move on. (useually hosed CMOS)
FIXPL2:
    // asm 00003909: 	STF	R3,*+AR0(OPOSZ)		;Now do the letters on it
    // asm 0000390A: 	CALL	OBJ_GFIND_NEXT
    // asm 0000390B: 	BNC	FIXPL2			;do as many as there are
FIXPL3:
    // asm 0000390C: 	FLOAT	-500,R1			;PLate to plate offset
    // asm 0000390D: 	ADDF	R1,R0
    // asm 0000390E: 	ADDI	1,AR5
    // asm 0000390F: 	CMPI	9,AR5
    // asm 00003910: 	BLE	FIXPL
    // asm 00003911: 	POP	AR5
    // asm 00003912: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIX_PLATES", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*	FLY PLATES
*	R0 = how far to move
*	R3 = position of the last plate
*/
void FLY_PLATES(void)
{
    // asm 00003913: 	PUSH	AR5
    // asm 00003914: 	LDI	0,AR5
    // asm 00003915: 	LDI	1h,R2
    // asm 00003916: 	LSH	16,R2
FLPL:
    // asm 00003917: 	LDI	AR5,R1
    // asm 00003918: 	ADDI	FIRST_PLATE,R1
    // asm 00003919: 	CALL	OBJ_FIND
    // asm 0000391A: 	LDF	*+AR0(OPOSZ),R1
    // asm 0000391B: 	ADDF	R0,R1
    // asm 0000391C: 	FLOAT	1000,R3
    // asm 0000391D: 	CMPF	R3,R1		;BLEW by destination?
    // asm 0000391E: 	LDFGT	R3,R1
    // asm 0000391F: 	STF	R1,*+AR0(OPOSZ)
    // asm 00003920: 	LDF	R1,R3
    // asm 00003921: 	SUBF	1,R3
    // asm 00003922: FLPL1
    // asm 00003922: 	LDI	R2,R1
    // asm 00003923: 	LSH	1,R2
    // asm 00003924: 	CALL	OBJ_GFIND
    // asm 00003925: 	BC	FLPL3			;No letters ,hosed CMOS?
FLPL2:
    // asm 00003926: 	STF	R3,*+AR0(OPOSZ)		;Now do the letters on it
    // asm 00003927: 	CALL	OBJ_GFIND_NEXT
    // asm 00003928: 	BNC	FLPL2			;do as many as there are
FLPL3:
    // asm 00003929: 	ADDI	1,AR5
    // asm 0000392A: 	CMPI	9,AR5
    // asm 0000392B: 	BLE	FLPL
    // asm 0000392C: 	POP	AR5
    // asm 0000392D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLY_PLATES", 0, 0);
    UNIMPL();
}

/*
*----------------------------------------------------------------------------
*This proc prints the name of the location on the marque
*/

void DISPLAY_HSTEXT(void)
{
    // asm 0000392E: 	FLOAT	-910,R3
    // asm 0000392F: 	BR	RACE_TEXT
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_HSTEXT", 0, 0);
    UNIMPL();
}

void ENTER_HSTEXT(void)
{
    // asm 00003930: 	FLOAT	-910-2000,R3			;The marquee is moved up 2000
RACE_TEXT:
    // asm 00003931: 	LDI	*+AR7(RACE_NUMBER),AR2
    // asm 00003932: 	ADDI	@LEG_NAMESI,AR2
    // asm 00003933: 	LDI	*AR2,AR2
    // asm 00003934: 	FLOAT	-4700,R2
    // asm 00003935: 	FLOAT	-301,R4
    // asm 00003936: 	LDI	0,R6	;ID
    // asm 00003937: 	CALL	PRINT3D
    // asm 00003938: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTER_HSTEXT", 0, 0);
    UNIMPL();
}

#define LOGO_X (-250)
#define LOGO_Y (-170)
#define LOGO_Z (368*2)

void INIT_LOGO(void)
{
    // asm 00003939: 	CALL	OBJ_GET
    // asm 0000393A: 	BC	LOGOX
    // asm 0000393B: 	LDI	AR0,AR4
    // asm 0000393C: 	LDIL	logo,R0
    // asm 0000393F: 	STI	R0,*+AR0(OROMDATA)
    // asm 00003940: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm 00003941: 	OR	*+AR0(OFLAGS),R0
    // asm 00003942: 	STI	R0,*+AR0(OFLAGS)
    // asm 00003943: 	FLOAT	LOGO_X,R0
    // asm 00003944: 	STF	R0,*+AR0(OPOSX)
    // asm 00003945: 	FLOAT	LOGO_Y,R0
    // asm 00003946: 	STF	R0,*+AR0(OPOSY)
    // asm 00003947: 	FLOAT	LOGO_Z,R0
    // asm 00003948: 	STF	R0,*+AR0(OPOSZ)
    // asm 00003949: 	LDI	AR0,AR2
    // asm 0000394A: 	CALL	OBJ_INSERTP
LOGOX:
    // asm 0000394B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LOGO", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
*----------------------------------------------------------------------------
*BRANCHED TO FROM RADIO_BUT IN SND.asm
*IS A PROC
*/

/* asm: RADIO_HS_SHADOW	.bss	RADIO_HS_SHADOW,1 */
int RADIO_HS_SHADOW;
/* asm: RADIO_HS_SWITCH	.bss	RADIO_HS_SWITCH,1 */
int RADIO_HS_SWITCH;

void RBMATTR_CHECK(void)
{
    // asm 0000394C: 	LDI	@_MODE,R0
    // asm 0000394D: 	TSTB	MHS,R0		;IN HS DISPLAY?
    // asm 0000394E: 	BEQ	RBMCX
    // asm 0000394F: 	LDI	@RADIO_HS_SWITCH,R0
    // asm 00003950: 	SUBI	1,R0
    // asm 00003951: 	STI	R0,@RADIO_HS_SWITCH
RBMCX:
    // asm 00003952: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RBMATTR_CHECK", 0, 0);
    UNIMPL();
}

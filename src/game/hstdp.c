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
#include "discovered_labels.h"

/*
 * Source module: asm/HSTDP.ASM
 */

void ENTER_INITIALS(void);
void MSLPX(void);
void EIML(void);
void EMIL1(void);
void TRIGPOTCHK(void);
void DORUB(void);
void NOTBCKSPC(void);
void DONE_ENTRY(void);
void ENTERTEXT(void);
void CREATE_ENTERTEXT(void);
void MAKE_CENTER(void);
void HSTD_TIMER(void);
void INSERT_INITS(void);
void INTO_TABLE_P(void);
void ITP2(void);
void CHECK_FIRST_TIME(void);
void NOT_FIRST_TIME(void);
void CALC_TOTAL_ELAPSED(void);
void PRESS_CODE_ENTRY(void);
void PRA(void);
void PR0A2(void);
void PR1(void);
void PR2A(void);
void PR2AA(void);
void PR2A1(void);
void PR3(void);
void INIT_PRESS_OBJECTS(void);
void FPO1(void);
void FPO1A(void);
void FPO3(void);
void FPO4(void);
void FPO6(void);
void FPO7(void);
void FPOX(void);
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
void OH_NO(void);
void OBJ_FIND_NEXT(void);
void OBJ_GFIND(void);
void OH_GNO(void);
void OBJ_GFIND_NEXT(void);
void OBJ_TAG(void);
void OTAG(void);
void OTAGX(void);
void OBJ_TAGALL(void);
void OTAX(void);
void OBJ_MOVY_GROUP(void);
void OMYG(void);
void OBJ_DEL_GROUP(void);
void ODG(void);
void FIND_ALL_PLATES(void);
void FIND_PLATES(void);
void MAKE_NUMBERS(void);
void MAKE_TIME(void);
void MT2(void);
void FORMAT_NUM(void);
void CREATE_LETTERS(void);
void ASCII_TO_OBJ(void);
void CRLL(void);
void PRINT3D(void);
void PR3DX(void);
void DISPLAY_HIGH_SCORES(void);
void DHSNOBUT(void);
void DHSNEXT(void);
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

#define FLASH_ON 1 //TURN THIS OFF TO LOOSE FLASHING
#define PRESS_DIAM 271 //NOTE: The plate is on the bottom surface
#define PRESS_RADX 1.5708
#define ROLLER_ZOFF 620
#define PRESS_STARTZ (-1300)
#define PRESS_LASTZ (-2100) //-1900
#define PRESS_TRAVELZ (PRESS_LASTZ-PRESS_STARTZ)
/* ;PRESS_STARTY	.set	-30
 */
#define PRESS_STARTY (-45)
#define PRESS_LASTY (-100)
#define PRESS_TRAVELY (PRESS_LASTY-PRESS_STARTY)
#define HIGH_SCORE_GROUP 0x200
/* asm: NUMTAB	.word	dzero,done,dtwo,dthree,dfour,dfive,dsix,dseven,deight,dnine */
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
const char *EIP = "ENTER INITIALS";
/* *----------------------------------------------------------------------------
 */
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
/* asm: PEDHIT	.bss	PEDHIT,1 */
int PEDHIT;
/* *----------------------------------------------------------------------------
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
#define CAM_RADX (-0.393)
#define ROT_FRAMES 40
#define left_stop (-5000)
#define cam_left_stop (-4700)
#define zoomin_stop (-1300)
#define zoomout_stop (-2100)
#define PLACE_ENDRADX 4.71239
/* *----------------------------------------------------------------------------
 */
#define FRAME 0x85
#define SCROLLB 0x86
#define PRESS 0x87
#define PRESSB 0x88
#define ARMS 0x89
#define PLATE 0x8B
#define MARQLET 0x8C
#define PLATE_ZOFF (ROLLER_ZOFF-140)
/* asm: SCROLLBTAB	;	A    B	  C   D	  E   F	  G   H	  I   J	  K   L	  M   N	  O */
/* asm: 	.word	1057,1010,960,911,855,803,753,696,648,598,546,494,426,367,307 */
/* asm: 	.word	252,193,138,82,24,-35,-97,-161,-230,-292,-347,-401,-465,-517,-573 */
/* asm: 	.word	-633,-697,-757,-820,-880,-945,-1007,-1063 */
int SCROLLBTAB[] = {
    1057, 1010, 960, 911, 855, 803, 753, 696, 648, 598, 546, 494, 426, 367, 307,
    252, 193, 138, 82, 24, -35, -97, -161, -230, -292, -347, -401, -465, -517, -573,
    -633, -697, -757, -820, -880, -945, -1007, -1063,
};
/* *----------------------------------------------------------------------------
*ADJUST_ROLLERS	This routine places the rollers in a arc so that there
*		priorities are correct.
*IO = NONE
*
 */
#define ROLLER_GROUP 0x400
/* *----------------------------------------------------------------------------
 */
#define ROLLER_DIAM 111
/* *----------------------------------------------------------------------------
 */
#define FIRST_BOLT 0x30
#define NUM_PLATES 9
#define FIRST_PLATE 0x20
/* *----------------------------------------------------------------------------
*MAKE_NUMBERS
*	R0	= INTEGER SCORE
*	AR5	= PLACE
*
*
 */
#define FIRST_NUMBER 0x10
/* asm: LONGEST_TIME	.word	198000 */
int LONGEST_TIME = 198000;
/* *----------------------------------------------------------------------------
*	PARAMETERS	AR0 = POINTER TO PLATE that owns this
*		LETTER0-LETTER2 = THE three letters
*		R1  = Place the player is in
 */
#define LETTER_SIZEX 120
#define LETTER_XOFF (-120)
#define LETTER_YOFF (-12)
/* *----------------------------------------------------------------------------
*	PARAMETERS	AR2 = POINTER TO STRING
*		R2  = Xpos FL
*		R3  = Ypos FL
*		R4  = Zpos FL
*		R6  = ID
 */
#define LETTER3D_SIZEX 80
#define HS_ZOOM 40
#define HS_STARTY 650
/* ;HS_ENDY 	.set	-280
 */
#define HS_ENDY (-210)
#define HS_YDIFF ((HS_ENDY-HS_STARTY)/HS_ZOOM)
#define HS_STARTZ (-4200)
#define HS_ENDZ (-1960) //-800
#define HS_ZDIFF ((HS_ENDZ-HS_STARTZ)/HS_ZOOM)
/* *----------------------------------------------------------------------------
*PROC
*R4 = race number
 */
/* asm: FLASH_PALS */
/* asm: 	.word	plate_medp,plate_lightp,plate_lightp1,plate_lightp,-1 */
int FLASH_PALS[] = {
    plate_medp, plate_lightp, plate_lightp1, plate_lightp, -1,
};
/* asm: DELIST	.word	808Bh,0104h,3057h,0 */
int DELIST[] = {
    0x808B, 0x0104, 0x3057, 0,
};
#define LOGO_X (-250)
#define LOGO_Y (-170)
#define LOGO_Z (368*2)
/* asm: RADIO_HS_SHADOW	.bss	RADIO_HS_SHADOW,1 */
int RADIO_HS_SHADOW;
/* asm: RADIO_HS_SWITCH	.bss	RADIO_HS_SWITCH,1 */
int RADIO_HS_SWITCH;

/* *----------------------------------------------------------------------------
 */
void ENTER_INITIALS(void)
{
    // asm: 	CALL	INTO_TABLE_P
    // asm: 	BC	GOODENOUGH
    // asm: 	RETP		;NOPE, didn't make it
GOODENOUGH:
    // asm: 	STI	R0,*+AR7(PLACE)
    // ;Wait for LOADING TO END
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@NOSWAP
    // asm: 	LDI	0,R4
MSLP2:
    // asm: 	LDI	@DECOMP_ACTIVE,R0
    // asm: 	BNZ	MSLP3
    // asm: 	ADDI	1,R4
    // asm: 	CMPI	3,R4
    // asm: 	BGT	MSLPX				;Done Loading
MSLP3:
    // asm: 	SLEEP	1
    // asm: 	BU	MSLP2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTER_INITIALS", 0, 0);
    UNIMPL();
}

void MSLPX(void)
{
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	CALL	SILENT
    // asm: 	SOND1	ENTER_INITS_THEME	;Play the Initials entry theme
    // asm: 	CALL	OBJ_INIT		;Zero out object data list pointers
    // asm: 	CALL	TEXT_INIT
    // asm: 	FLOAT	-512,R0
    // asm: 	STF	R0,@INFIN_CORRECT
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MMODE|MWATER|MBRIDGE|MINTUNNEL,R0
    // asm: 	OR	MINIT|MINFIN|MHS,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	LDI	UTIL_C|TEXTP_T,R0
    // asm: 	LDI	-1,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	CLRI	R0			;Black background
    // asm: 	STI	R0,@BGNDCOLA
    // asm: 	LDP	@_CAMERAPOS+X		;Initialize the camera
    // asm: 	LDF	-24,R0
    // asm: 	STF	R0,@_CAMERAPOS+X
    // asm: 	FLOAT	PRESS_STARTY,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	FLOAT	PRESS_STARTZ,R0
    // asm: 	STF	R0,@_CAMERAPOS+Z
    // asm: 	CLRF	R2
    // asm: 	STF	R2,@_CAMERARAD+Y
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // ;	CALL	CLEANUP_PALS
    // asm: 	CALL	LOAD_FIXED_PALETTES
    // asm: 	LDL	press_PALETTES,AR2
    // asm: 	CALL	HARDalloc_section
    // asm: 	FIFO_CLRP	R0		;IS THE FIFO CLEAR
    // asm: 	DMA_WT		R0
    // asm: 	CALL	FIFO_RESET		;This will load the wave ram before continuing on
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@HARD_SECTION_LOAD
    // asm: 	LDL	_SECpress,AR2
    // asm: 	CALL	LOAD_SECTION_REQ
    // asm: 	LDL	scroll_white,AR2	;This is the palette for the letters
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	STI	R0,*+AR7(WHITE_PAL)
    // asm: 	LDL	press_grp,AR2
    // asm: 	CALL	LOAD_SINGLE_SECTION
    // asm: 	CALL	INIT_PRESS_OBJECTS	;Go and set up pointers to special Objects
    // asm: 	CALL	RESCAN
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@NOSWAP
    // asm: 	LDI	LASTCHAR+1,R0			;Set the steering wheel selection
    // asm: 	STI	R0,@POSES
    // asm: 	LDI	LASTCHAR/2,R0
    // asm: 	STI	R0,@POSE
    // asm: 	LDI	20,R0			;Set the number of seconds to enter your initials
    // asm: 	STI	R0,@_countdown
    // 	;GET THE OBJECTS
    // 	;
    // asm: 	LDL	scroll_gr2,AR2	;Palette used for the Letters on the bottom of the press
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	STI	R0,*+AR7(GREY_PAL)
    // asm: 	CALL	CHECK_FIRST_TIME
    // asm: 	BC	PRESS_CODE_ENTRY
    // asm: 	LDI	*+AR7(WHITE_PAL),R4
    // asm: 	FLOAT	-LETTER_SIZEX-LETTER_SIZEX/4,R5	;Position of the first letter
    // asm: 	FLOAT	-103+LETTER_YOFF,R6
    // asm: 	FLOAT	-PRESS_DIAM-73,R7
    // asm: 	LDI	' ',R0
    // asm: 	STI	R0,*+AR7(INITI0)
    // asm: 	STI	R0,*+AR7(INITI1)
    // asm: 	STI	R0,*+AR7(INITI2)
    // asm: 	LDL	pa,AR2			;Create the first Letter
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(INIT0)
    // asm: 	STI	R4,*+AR0(OPAL)
    // asm: 	STF	R5,*+AR0(OPOSX)
    // asm: 	STF	R6,*+AR0(OPOSY)
    // asm: 	STF	R7,*+AR0(OPOSZ)
    // asm: 	FLOAT	LETTER_SIZEX,R0
    // asm: 	ADDF	R0,R5
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	LDL	po,AR2			;Create the second letter
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(INIT1)
    // asm: 	STI	R4,*+AR0(OPAL)
    // asm: 	STF	R5,*+AR0(OPOSX)
    // asm: 	STF	R6,*+AR0(OPOSY)
    // asm: 	STF	R7,*+AR0(OPOSZ)
    // asm: 	FLOAT	LETTER_SIZEX,R0
    // asm: 	ADDF	R0,R5
    // asm: 	LDL	po,AR2			;Create the third letter
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(INIT2)
    // asm: 	STI	R4,*+AR0(OPAL)
    // asm: 	STF	R5,*+AR0(OPOSX)
    // asm: 	STF	R6,*+AR0(OPOSY)
    // asm: 	STF	R7,*+AR0(OPOSZ)
    // asm: 	CREATE	ENTERTEXT,SPAWNER_C	;This will slide in the text "ENTER INITIALS"
    // asm: 	CLRI	AR5			;character index
    // asm: 	LDF	@STEERCT,R0
    // asm: 	STF	R0,@WHEELPOS
    // asm: 	CALL	GETCHOICE	;READS the steering wheel, uses POSES = number of choices
    // asm: 	LDI	@POSE,R4	;On return POSE = Choice wheel is pointing at.
    // asm: 	STI	R4,*+AR7(OLDPOT0)
    // asm: 	LDI	0,R0		;CLEAR OUT LEFT OVER START HIT
    // asm: 	STI	R0,@START_HIT
    // asm: 	LDF	@PEDALMN,R0
    // asm: 	LDF	@PEDALMX,R1
    // asm: 	SUBF	R0,R1
    // asm: 	FIX	R1
    // asm: 	RS	1,R1
    // asm: 	FIX	@PEDALMN,R0
    // asm: 	ADDI	R0,R1
    // asm: 	STI	R1,*+AR7(PEDTRIG)
    // asm: 	LDI	0,AR6			;Set debounce counter to 0
    // ;	BR	PEDALWT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MSLPX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
 */
void EIML(void)
{
    // asm: 	CMPI	0,AR6
    // asm: 	BNE	TOSLP2
    // asm: 	LDI	@SWITCHBUTS,R0
    // asm: 	LDL	SW_RADIO|SW_VIEW0|SW_VIEW1|SW_VIEW2,R1
    // asm: 	AND	R1,R0
    // asm: 	BZ	TOSLP2
    // asm: 	LDI	@_countdown,R0
    // asm: 	DEC	R0
    // asm: 	STI	R0,@_countdown
    // asm: 	LDI	2,AR6
TOSLP2:
    // asm: 	CMPI	0,AR6
    // asm: 	BEQ	TOSLP3
    // asm: 	SUBI	1,AR6
TOSLP3:
    // asm: 	CALL	GETCHOICE	;READS the steering wheel, uses POSES = number of choices
    // asm: 	LDI	@POSE,R4	;On return POSE = Choice wheel is pointing at.
    // asm: 	CMPI	LASTCHAR,R4
    // asm: 	LDIGT	LASTCHAR,R4
    // asm: 	CMPI	0,R4
    // asm: 	LDILT	0,R4
    // asm: 	LDI	*+AR7(OLDPOT0),R1
    // asm: 	STI	R4,*+AR7(OLDPOT0)
    // asm: 	ADDI	R1,R4
    // asm: 	RS	1,R4
    // asm: 	LDI	R4,R0
    // asm: 	CMPI	LASTCHAR,R0
    // asm: 	LDIGT	LASTCHAR,R0
    // asm: 	ADDI	@PLATE_LETTERSI,R0	;Add in the start of the Letter table
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR1		;Get the ROM pointer to the image the letter will use
    // asm: 	LDI	AR7,AR0
    // asm: 	ADDI	INIT0,AR0		;GET index to which letter we are on
    // asm: 	ADDI	AR5,AR0
    // asm: 	LDI	*AR0,AR0
    // asm: 	LDI	*+AR7(WHITE_PAL),R0
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	AR1,*+AR0(OROMDATA)	;Set the ROM point of the object for this leter
    // asm: 	LDI	AR7,AR0
    // asm: 	ADDI	INITI0,AR0
    // asm: 	ADDI	AR5,AR0			;AR5 = which letter 0-2
    // asm: 	LDI	R4,R0
    // asm: 	CMPI	LASTCHAR,R0
    // asm: 	LDIGE	' ',R0
    // asm: 	BGE	EMIL1A
    // asm: 	CMPI	RUB+1,R0
    // asm: 	BLT	EMIL1
    // asm: 	ADDI	'0'-RUB-1,R0
    // asm: 	BR	EMIL1A
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EIML", 0, 0);
    UNIMPL();
}

void EMIL1(void)
{
    // asm: 	ADDI	'A',R0			;TRANSLATE BACK TO ASCII
EMIL1A:
    // asm: 	STI	R0,*AR0			;initial index SET THE LETTER
    // asm: 	LDI	@_countdown,R0
    // asm: 	BLE	DOTRIG			;Out of time, then auto trigger the next letter
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	DOTRIG
    // *ELP CHANGE
    // asm: 	LDI	@_pot1,R0
    // asm: 	CMPI	*+AR7(PEDTRIG),R0
    // asm: 	BGE	TRIGPOTCHK
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@PEDHIT
    // asm: 	BU	NOTRIG
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EMIL1", 0, 0);
    UNIMPL();
}

void TRIGPOTCHK(void)
{
    // asm: 	LDI	@PEDHIT,R1
    // asm: 	CMPI	1,R1
    // asm: 	BEQ	NOTRIG
    // asm: 	LDI	1,R0
    // asm: 	STI	R0,@PEDHIT
    // *ELP END CHANGE
DOTRIG:
    // asm: 	LDI	@_countdown,R0
    // asm: 	BZ	DT1			;If out of time don't decrement
    // asm: 	LDI	20,R0			;Set the number of seconds to enter your initials
    // asm: 	STI	R0,@_countdown
DT1:
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_HIT
    // asm: 	LDI	AR7,AR0
    // asm: 	ADDI	INIT0,AR0
    // asm: 	ADDI	AR5,AR0
    // asm: 	CMPI	RUB,R4
    // asm: 	BNE	NOTBCKSPC
    // asm: 	LDI	@_countdown,R0
    // asm: 	BGT	DORUB			;Out of time?
    // asm: 	LDI	LASTCHAR,R4		;FORCE END
    // asm: 	LDI	AR7,AR0			;Put a space in the position the RUB was in
    // asm: 	ADDI	INITI0,AR0
    // asm: 	ADDI	AR5,AR0			;AR5 = which letter 0-2
    // asm: 	LDI	' ',R0
    // asm: 	STI	R0,*AR0
    // asm: 	BR	NOTBCKSPC		;THIS WILL BAIL CORRECTLY
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TRIGPOTCHK", 0, 0);
    UNIMPL();
}

void DORUB(void)
{
    // asm: 	CMPI	0,AR5			;if char[0] then dont backspace
    // asm: 	BEQ	PEDALWT
    // ;This does a back space
    // asm: 	LDI	*AR0,AR2
    // asm: 	CALL	OBJ_PULL			;Get rid of the letter on
    // asm: 	DEC	AR5			;AR5 = LETTER on
    // asm: 	BR	PEDALWT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DORUB", 0, 0);
    UNIMPL();
}

void NOTBCKSPC(void)
{
    // asm: 	LDI	AR7,AR1
    // asm: 	ADDI	INIT0,AR1		;GET index to the last letter we are on
    // asm: 	ADDI	AR5,AR1
    // asm: 	LDI	*AR1,AR1
    // asm: 	LDI	*+AR7(GREY_PAL),R0
    // asm: 	STI	R0,*+AR1(OPAL)
    // asm: 	LDI	*+AR1(OROMDATA),R1	;Set the ROM point of the object for this leter
    // asm: 	CMPI	LASTCHAR,R4
    // asm: 	BEQ	DONE_ENTRY
    // asm: 	INC	AR5
    // asm: 	CMPI	3,AR5
    // asm: 	BGE	DONE_ENTRY
    // asm: 	INC	AR0
    // asm: 	LDI	*AR0,AR2
    // asm: 	STI	R1,*+AR2(OROMDATA)	;Set the ROM point of the object for this leter
    // asm: 	CALL	OBJ_INSERT		;Add the next letters object
    // asm: 	SONDFX	CHOOSE_LETTER
PEDALWT:
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
    // asm: 	CALL	HSTD_TIMER
    // asm: 	CALL	POS_SCROLLB
    // asm: 	SLEEP	1
    // asm: 	BR	EIML
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOTBCKSPC", 0, 0);
    UNIMPL();
}

void DONE_ENTRY(void)
{
    // asm: 	CMPI	LASTCHAR,R4
    // asm: 	BNE	DE1
    // asm: 	LDI	AR1,AR2
    // asm: 	CALL	OBJ_PULL
DE1:
    // asm: 	LDI	*+AR7(INITI2),R2	;Create string for initial entry
    // asm: 	LSH	8,R2
    // asm: 	OR	*+AR7(INITI1),R2
    // asm: 	LSH	8,R2
    // asm: 	OR	*+AR7(INITI0),R2
    // asm: 	SETADJ	ADJ_INITIALS
    // asm: 	BR	PRESS_CODE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DONE_ENTRY", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*This proc slides in the WORDS "ENTER INITIALS"
 */
void ENTERTEXT(void)
{
    // asm: 	FLOAT	616,R2			;XPOS
    // asm: 	CALL	CREATE_ENTERTEXT
    // asm: 	LDF	-6,R0
    // asm: 	STF	R0,*+AR4(TEXT_VELX)
    // asm: 	STF	R0,*+AR5(TEXT_VELX)
    // asm: 	SLEEP	60
    // asm: 	SONDFX	WELCOME
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR4(TEXT_VELX)
    // asm: 	STF	R0,*+AR5(TEXT_VELX)
ET0:
    // asm: 	SLEEP	1
    // asm: 	BR	ET0			;THIS PROC GETS KILLED BY PRESS_CODE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTERTEXT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PARAMETERS	R2 = XPOS
 */
void CREATE_ENTERTEXT(void)
{
    // asm: 	LDI	@EIPI,AR2		;enter initials	string
    // asm: 	FLOAT	254,R3			;YPOS
    // asm: 	LDI	9999,RC			;JIFFYS TO DISPLAY (INDEFINATLY)
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	CALL	MAKE_CENTER
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CREATE_ENTERTEXT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MAKE_CENTER(void)
{
    // asm: 	LDI	AR0,AR4
    // asm: 	LDI	AR1,AR5
    // asm: 	ORM	TXT_CENTER,*+AR4(TEXT_COLOR)
    // asm: 	ORM	TXT_CENTER,*+AR5(TEXT_COLOR)
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_CENTER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void HSTD_TIMER(void)
{
    // asm: 	LDI	@_countdown,R2
    // asm: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	256,R2
    // asm: 	FLOAT	343,R3			;YPOS
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADDDS
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	CALL	MAKE_CENTER
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HSTD_TIMER", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void INSERT_INITS(void)
{
    // asm: 	LDI	@SCORE,R0
    // asm: 	LDI	*+AR7(INITI0),R1
    // asm: 	LDI	*+AR7(INITI1),R2
    // asm: 	LDI	*+AR7(INITI2),R3
    // asm: 	LDI	0,R4			;POSITION FINISHED IN
    // asm: 	LDI	*+AR7(PLACE),R5
    // asm: 	LDI	*+AR7(RACE_NUMBER),R6
    // asm: 	CALL	INSERT_TABLE_ENTRY
    // asm: 	LDI	*+AR7(RACE_NUMBER),R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INSERT_INITS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
* USES BONUS_WAVE and GAMETRAKI to determine...
*
*RETURNS C = 1 PLAYER WILL MAKE IT INTO THE HS TABLE
*	 C = 0 PLAYER WILL NOT MAKE IT
 */
void INTO_TABLE_P(void)
{
    // asm: 	CALL	VALIDATE_HSTD_TABLES
    // 	;check check score if good enough to enter
    // 	;the hstd table
    // asm: 	LDI	@BONUS_WAVE,R1
    // asm: 	SUBI	1,R1
    // asm: 	STI	R1,*+AR7(RACE_NUMBER)
    // asm: 	LDI	R1,R0
    // asm: 	MPYI	GT_SIZE,R0
    // asm: 	ADDI	@GAMETRAKI,R0
    // asm: 	LDI	R0,AR0
    // ;	LDI	*+AR0(GT_ETIME),R0
    // asm: 	LDI	@ETIME,R0
    // asm: 	CMPI	14,R1
    // asm: 	CALLEQ	CALC_TOTAL_ELAPSED	;A call to ENTER_INITIALS WITH BONUS_WAVE =15 WILL
    // asm: 	CMPI	0,R0			;CHECK SF TO DC
    // asm: 	BEQ	ITP1			;Did Not Finish this Race (DNF)
    // asm: 	CALL	CHECK_RACE_TABLE
    // asm: 	CMPI	-1,R0
    // asm: 	BNE	ITP2
ITP1:
    // asm: 	CLRC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INTO_TABLE_P", 0, 0);
    UNIMPL();
}

void ITP2(void)
{
    // asm: 	SETC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ITP2", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*RETURNS C = 1 PLAYER HAS ALLREADY ENTERED INITS
*	 C = 0 PLAYER HAS NOT ENTERED INITS
*SETS PLAYER LETTERS IF FOUND
* INITI0-INITI2
 */
void CHECK_FIRST_TIME(void)
{
    // asm: 	READADJ	ADJ_INITIALS
    // asm: 	CMPI	-1,R0
    // asm: 	BNE	NOT_FIRST_TIME
    // asm: 	CLRC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CHECK_FIRST_TIME", 0, 0);
    UNIMPL();
}

void NOT_FIRST_TIME(void)
{
    // asm: 	LDI	' ',R1			;Just incase the initials have been corrupted
    // asm: 	STI	R1,*+AR7(INITI1)
    // asm: 	STI	R1,*+AR7(INITI2)
    // asm: 	LDI	AR7,AR0
    // asm: 	ADDI	INITI0,AR0
NFTLP:
    // asm: 	LDI	R0,R1
    // asm: 	AND	0FFh,R1
    // asm: 	BEQ	NFTX
    // asm: 	STI	R1,*AR0++
    // asm: 	LSH	-8,R0
    // asm: 	BNE	NFTLP
NFTX:
    // asm: 	SETC
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "NOT_FIRST_TIME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
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
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR0
    // asm: 	LDI	@GAMETRAKI,AR0
    // asm: 	LDI	13,R1
    // asm: 	LDI	0,R2
CTELP:
    // asm: 	LDI	*+AR0(GT_ETIME),R0
    // asm: 	BEQ	CTEX			;Did Not Finish this Race (DNF)
    // asm: 	ADDI	R0,R2
    // asm: 	ADDI	GT_SIZE,AR0
    // asm: 	SUBI	1,R1
    // asm: 	BP	CTELP
    // asm: 	LDI	R2,R0
CTEX:
    // asm: 	POP	AR0
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CALC_TOTAL_ELAPSED", 0, 0);
    UNIMPL();
}

void PRESS_CODE_ENTRY(void)
{
    // ;Put the letters on the bottom of the press
    // asm: 	LDI	*+AR7(WHITE_PAL),R4
    // asm: 	FLOAT	-LETTER_SIZEX-LETTER_SIZEX/4,R5	;Position of the first letter
    // asm: 	FLOAT	-103+LETTER_YOFF,R6
    // asm: 	FLOAT	-PRESS_DIAM-73,R7
    // asm: 	LDI	*+AR7(INITI0),AR2			;Create the first Letter
    // asm: 	CALL	ASCII_TO_OBJ
    // asm: 	STI	AR0,*+AR7(INIT0)
    // asm: 	STI	R4,*+AR0(OPAL)
    // asm: 	STF	R5,*+AR0(OPOSX)
    // asm: 	STF	R6,*+AR0(OPOSY)
    // asm: 	STF	R7,*+AR0(OPOSZ)
    // asm: 	FLOAT	LETTER_SIZEX,R0
    // asm: 	ADDF	R0,R5
    // asm: 	LDI	*+AR7(INITI0),AR2
    // asm: 	CMPI	' ',AR2
    // asm: 	BEQ	NOINS0
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
NOINS0:
    // asm: 	LDI	*+AR7(INITI1),AR2			;Create the first Letter
    // asm: 	CALL	ASCII_TO_OBJ
    // asm: 	STI	AR0,*+AR7(INIT1)
    // asm: 	STI	R4,*+AR0(OPAL)
    // asm: 	STF	R5,*+AR0(OPOSX)
    // asm: 	STF	R6,*+AR0(OPOSY)
    // asm: 	STF	R7,*+AR0(OPOSZ)
    // asm: 	FLOAT	LETTER_SIZEX,R0
    // asm: 	ADDF	R0,R5
    // asm: 	LDI	*+AR7(INITI1),AR2
    // asm: 	CMPI	' ',AR2
    // asm: 	BEQ	NOINS1
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
NOINS1:
    // asm: 	LDI	*+AR7(INITI2),AR2			;Create the first Letter
    // asm: 	CALL	ASCII_TO_OBJ
    // asm: 	STI	AR0,*+AR7(INIT2)
    // asm: 	STI	R4,*+AR0(OPAL)
    // asm: 	STF	R5,*+AR0(OPOSX)
    // asm: 	STF	R6,*+AR0(OPOSY)
    // asm: 	STF	R7,*+AR0(OPOSZ)
    // asm: 	LDI	*+AR7(INITI2),AR2
    // asm: 	CMPI	' ',AR2
    // asm: 	BEQ	NOINS2
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
NOINS2:
    // asm: 	SONDFX	WELCOME
    // *ELP CHANGE
    // asm: 	PUSHP	AR5
    // 	;SLEEP	60
    // asm: 	LDI	30-1,AR5
PAPA35:
    // asm: LDI	@START_HIT,R0
    // asm: 	BNZ	PAPA35X
    // asm: 	SUBI	@NFRAMES,AR5
    // asm: 	CMPI	0,AR5
    // asm: 	LDILT	0,AR5
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,PAPA35
PAPA35X:
    // asm: 	POPP	AR5
    // *ELP END CHANGE
    // ;FALL INTO PRESS CODE
PRESS_CODE:
    // asm: 	LDI	SPAWNER_C,R0		;KILL ENTERTEXT
    // asm: 	LDI	CLASS_M,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	CALL	TEXT_INIT
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@START_HIT
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@FRAMRATE   		;RESET FRAME RATE TO ATTRACT MODE
    // asm: 	SONDFX	DOPEN
    // asm: 	LDI	*+AR7(RACE_NUMBER),R1
    // asm: 	LDI	R1,R0
    // asm: 	MPYI	GT_SIZE,R0
    // asm: 	ADDI	@GAMETRAKI,R0
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*+AR0(GT_ETIME),R0
    // asm: 	CMPI	14,R1
    // asm: 	CALLZ	CALC_TOTAL_ELAPSED
    // asm: 	STI	R0,@SCORE
    // asm: 	CALL	INSERT_INITS
    // asm: 	STI	R0,@SCORE
    // asm: 	CALL	FIND_PLATES		;Set up the High score display board
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@_countdown
    // asm: 	CALL	ADJUST_ROLLERS
    // asm: 	CALL	ENTER_HSTEXT
    // asm: 	FLOAT	-1950,R2
    // asm: 	LDI	HIGH_SCORE_GROUP,R1		;High score group
    // asm: 	CALL	OBJ_MOVY_GROUP
    // asm: 	LDI	*+AR7(MISPLATEOBJ),AR2
    // asm: 	CALL	OBJ_PULL			;Loose the plate in the players place
    // asm: 	CALL	INI_PLAYERS_BOLTS		;Move them into position to fly
    // asm: 	LDI	5,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	FLOAT	-2000,R0
    // asm: 	LDF	*+AR0(OPOSY),R1
    // asm: 	ADDF	R0,R1
    // asm: 	STF	R1,*+AR0(OPOSY)
    // asm: 	LDI	8,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	FLOAT	-2000,R0
    // asm: 	LDF	*+AR0(OPOSY),R1
    // asm: 	ADDF	R0,R1
    // asm: 	STF	R1,*+AR0(OPOSY)
    // asm: 	LDI	*+AR7(SCROLLBOBJ),AR2	;LOOSE THE WHEEL AND IT's LETTERS
    // asm: 	CALL	OBJ_PULL
    // asm: PRA0					;LOOSE THE LETTERS ON THE FRAME
    // asm: 	LDI	MARQLET,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	BC	PRA
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	BR	PRA0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRESS_CODE_ENTRY", 0, 0);
    UNIMPL();
}

void PRA(void)
{
    // ;Pan the camera back,pull back the frame,rotate the press down,
    // ;and scroll the plate in on the rollers
    // asm: 	FLOAT	PRESS_FRAMES,R1		;Pan camera to correct y
    // asm: 	FLOAT	PRESS_TRAVELY,R0
    // asm: 	CALL	DIV_F			;PRESS_TRAVELZ/PRESS_FRAMES
    // asm: 	LDP	@_CAMERAPOS+Y
    // asm: 	ADDF	@_CAMERAPOS+Y,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	SETDP
    // asm: 	FLOAT	PRESS_FRAMES,R1		;Pan camera back
    // asm: 	FLOAT	PRESS_TRAVELZ,R0
    // asm: 	CALL	DIV_F			;PRESS_TRAVELZ/PRESS_FRAMES
    // asm: 	LDP	@_CAMERAPOS+Z
    // asm: 	ADDF	@_CAMERAPOS+Z,R0
    // asm: 	STF	R0,@_CAMERAPOS+Z
    // asm: 	SETDP
    // asm: 	FLOAT	PRESS_FRAMES-1,R1	;Rotate press down
    // asm: 	LDF	PRESS_RADX,R0
    // asm: 	CALL	DIV_F			;PRESS_RADX/PRESS_FRAMES
    // asm: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm: 	LDF	*+AR0(ORADX),R2
    // asm: 	SUBF	R0,R2
    // asm: 	STF	R2,*+AR0(ORADX)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	FLOAT	PRESS_FRAMES,R1		;Pull the frame back
    // asm: 	FLOAT	-2000,R0
    // asm: 	CALL	DIV_F			;-2000/PRESS_FRAMES
    // ;	LDI	*+AR7(FRAMEOBJ),AR0
    // asm: 	LDI	FRAME,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	BC	PRA2
PRA1:
    // asm: 	LDF	*+AR0(OPOSZ),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSZ)
    // asm: 	CALL	OBJ_FIND_NEXT
    // asm: 	BNC	PRA1
PRA2:
    // asm: 	CALL	MOVE_PRESSB		;Move the bottom of the press reletive to the press
    // asm: 	CALL	MOVE_BIGLET_PRESS	;Move the letters reletive to the bottom of the press
    // asm: 	LDI	*+AR7(PRESSOBJ),AR0		;Spin the rollers until press is rotated all the way down
    // asm: 	LDF	*+AR0(ORADX),R2
    // asm: 	CMPF	-0.01309,R2
    // asm: 	BLE	PR0A
    // asm: 	FLOAT	PRESS_FRAMES,R1
    // asm: 	LDF	ROLLER_TRAVEL,R0
    // asm: 	CALL	DIV_F			;PRESS_TRAVELZ/PRESS_FRAMES
    // asm: 	CALL	SPIN_ROLLERS		;Spin rollers also moves the plate
    // asm: 	SLEEP	1
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BZ	PRA
    // ;	BR	PRA
    // *ELP END CHANGE
PR0A:
    // asm: 	LDI	FRAME,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	BC	PR0A2
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	BR	PR0A
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRA", 0, 0);
    UNIMPL();
}

void PR0A2(void)
{
    // asm: 	SONDFX	HYDRO
    // *ELP CHANGE
    // 	;SLEEP	10
    // asm: 	SLEEP	5
    // *ELP END CHANGE
PR0:
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR1
    // *ELP END CHANGE
    // ;Move the Press and Arms down to stamp
    // asm: 	FLOAT	ARM_FRAMES,R1
    // asm: 	FLOAT	ARM_TRAVEL,R0
    // asm: 	CALL	DIV_F			;ARM_TRAVEL/ARM_FRAMES
    // asm: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	PUSHF	R0
    // asm: 	CALL	MOVE_PRESSB
    // asm: 	CALL	MOVE_BIGLET_PRESS
    // asm: 	POPF	R0
    // asm: 	LDI	*+AR7(ARMSOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	FLOAT	ARM_BOTTOM,R0
    // asm: 	CMPF	R0,R2
    // asm: 	BGE	PR1
    // asm: 	SLEEP	1
    // asm: 	BR	PR0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PR0A2", 0, 0);
    UNIMPL();
}

void PR1(void)
{
    // asm: 	SONDFX	STAMP
    // asm: 	SONDFX	STAMP1
    // ;STAMP
    // asm: 	LDI	*+AR7(INITI0),R0
    // asm: 	LDI	*+AR7(INIT0),AR2
    // asm: 	CMPI	' ',R0
    // asm: 	BEQ	PR1A1
    // asm: 	CALL	OBJ_PULL
PR1A1:
    // asm: 	LDI	*+AR7(INITI1),R0
    // asm: 	LDI	*+AR7(INIT1),AR2
    // asm: 	CMPI	' ',R0
    // asm: 	BEQ	PR1A2
    // asm: 	CALL	OBJ_PULL
PR1A2:
    // asm: 	LDI	*+AR7(INITI2),R0
    // asm: 	LDI	*+AR7(INIT2),AR2
    // asm: 	CMPI	' ',R0
    // asm: 	BEQ	PR1A3
    // asm: 	CALL	OBJ_PULL
PR1A3:
    // asm: 	LDI	STAMP_FRAMES,AR5
    // asm: PR1A
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR1ZZZ
    // *ELP END CHANGE
    // asm: 	FLOAT	STAMP_SHAKE,R0
    // asm: 	LDP	@_CAMERAPOS+Y
    // asm: 	ADDF	@_CAMERAPOS+Y,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	SETDP
    // asm: 	SLEEP	1
    // asm: 	FLOAT	STAMP_SHAKE,R0
    // asm: 	NEGF	R0
    // asm: 	LDP	@_CAMERAPOS+Y
    // asm: 	ADDF	@_CAMERAPOS+Y,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	SETDP
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,PR1A
    // asm: 	FLOAT	PRESS_LASTY,R0
    // asm: 	LDP	@_CAMERAPOS+Y
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	SETDP
    // *ELP CHANGE
    // 	;SLEEP	6
    // asm: 	SLEEP	3
    // *ELP END CHANGE
PR1ZZZ:
    // asm: 	CALL	MAKE_PLAYERS_PLATE
PR2:
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR2A
    // *ELP END CHANGE
    // ;PULL press up
    // asm: 	CALL	MOVE_PRESSB
    // asm: 	FLOAT	ARM_FRAMES,R1
    // asm: 	FLOAT	ARM_TRAVEL,R0
    // asm: 	NEGF	R0
    // asm: 	CALL	DIV_F			;ARM_TRAVEL/ARM_FRAMES
    // asm: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	LDI	*+AR7(ARMSOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	FLOAT	ARM_START,R0
    // asm: 	CMPF	R0,R2
    // asm: 	BLE	PR2A
    // asm: 	SLEEP	1
    // asm: 	BR	PR2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PR1", 0, 0);
    UNIMPL();
}

void PR2A(void)
{
    // ;Rotate the camera up
    // asm: 	SONDFX	DOPEN
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PR2A", 0, 0);
    UNIMPL();
}

void PR2AA(void)
{
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR2A1
    // *ELP END CHANGE
    // asm: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm: 	FLOAT	-50,R0
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	LDI	*+AR7(ARMSOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	LDI	*+AR7(BPRESSOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	LDI	ARMS2,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	CALL	OBJ_FIND_NEXT
    // asm: 	LDF	*+AR0(OPOSY),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSY)
    // asm: 	FLOAT	ROT_FRAMES,R1		;Pan camera to correct y
    // asm: 	LDF	CAM_RADX,R0
    // asm: 	CALL	DIV_F
    // asm: 	LDP	@_CAMERARAD
    // asm: 	LDF	@_CAMERARAD,R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,@_CAMERARAD
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	NEGF	R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	CAMX,AR2
    // asm: 	LDI	@CAMERAPOSI,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	SLEEP	1
    // asm: 	DBU	AR6,PR2AA
    // ;Scroll plate left,pan camera left with th plate;ZOOM IN;ZOOM OUT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PR2AA", 0, 0);
    UNIMPL();
}

void PR2A1(void)
{
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR2A3
    // *ELP END CHANGE
    // asm: 	LDF	R6,R0
    // *ELP CHANGE
    // 	;ADDF	0.02,R6	 		;ACCELERATE
    // asm: 	ADDF	0.04,R6	 		;ACCELERATE
    // *ELP END CHANGE
    // ;	ADDF	0.01,R6	 		;ACCELERATE
    // asm: 	CALL	SPIN_ROLLERS
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	SUBF	*+AR7(CAMX),R0
    // *ELP CHANGE
    // 	;MPYF	0.20,R0
    // asm: 	MPYF	0.40,R0
    // *ELP END CHANGE
    // asm: 	ADDF	*+AR7(CAMX),R0
    // asm: 	FLOAT	cam_left_stop,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,*+AR7(CAMX)
    // asm: 	LDF	R0,R3
    // asm: 	FLOAT	zoomin_stop,R4
    // asm: 	FLOAT	zoomout_stop,R5
    // asm: 	FLOAT	-2500,R2		;Accelerating or decelerating?
    // asm: 	CMPF	R2,R3			;R3 set above to = PLATE OPOSX
    // asm: 	LDFGE	R4,R0			;Accelerating
    // asm: 	LDFLT	R5,R0			;Deccelerating
    // asm: 	SUBF	*+AR7(CAMZ),R0
    // *ELP CHANGE
    // 	;MPYF	0.1,R0
    // asm: 	MPYF	0.2,R0
    // *ELP END CHANGE
    // ;	MPYF	0.05,R0
    // asm: 	ADDF	*+AR7(CAMZ),R0
    // asm: 	CMPF	R4,R0			;Range check the zpos
    // asm: 	LDFGT	R4,R0			;to large
    // asm: 	CMPF	R5,R0
    // asm: 	LDFLT	R5,R0			;to small
    // asm: 	STF	R0,*+AR7(CAMZ)
    // asm: 	LDP	@_CAMERARAD
    // asm: 	LDF	@_CAMERARAD,R2
    // asm: 	SETDP
    // asm: 	NEGF	R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	CAMX,AR2
    // asm: 	LDI	@CAMERAPOSI,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	CALL	ADJUST_ROLLERS
    // asm: 	SLEEP	1
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	FLOAT	-2500,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGE	PR2A2
    // *ELP CHANGE
    // asm: 	ADDF	-0.08,R6	 		;DEACCELERATE (MUST BE TWICE ACCELERATE)
    // *ELP END CHANGE
    // asm: 	CMPF	0.02,R6
    // asm: 	LDFN	0.02,R6
    // ;	ADDF	-0.02,R6	 		;DEACCELERATE (MUST BE TWICE ACCELERATE)
    // ;	CMPF	0.01,R6
    // ;	LDFN	0.01,R6
PR2A2:
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	FLOAT	left_stop,R1
    // asm: 	CMPF	R1,R0
    // asm: 	BGE	PR2A1
    // ;Flip the plate up on end 90deg
PR2A3:
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR3B
    // *ELP END CHANGE
    // asm: 	LDI	*+AR7(PLATEOBJ),AR2
    // asm: 	LDF	0.15708,R2
    // asm: 	ADDF	*+AR2(ORADX),R2
    // asm: 	CMPF	HALFPI,R2
    // asm: 	LDFGT	HALFPI,R2
    // asm: 	STF	R2,*+AR2(ORADX)
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	*+AR7(PLATEOBJ),AR2	;Setup the offset for the bottom of the plate
    // asm: 	LDF	0,R0
    // asm: 	STF	R0,*+AR2(OVELX)
    // asm: 	STF	R0,*+AR2(OVELY)
    // asm: 	FLOAT	-156,R0
    // asm: 	STF	R0,*+AR2(OVELZ)
    // asm: 	LDI	*+AR7(PLATEOBJ),R2
    // asm: 	ADDI	OMATRIX,R2
    // asm: 	ADDI	OVELX,AR2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	FLOAT	PLATE_ZOFF,R0
    // asm: 	ADDF	*+AR0(OVELZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	FLOAT	610,R0
    // asm: 	ADDF	*+AR0(OVELY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	0,R0			;No x movement
    // asm: 	CALL	MOVE_PLAYERS_LETTERS
    // asm: 	SLEEP	1
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	LDF	*+AR0(ORADX),R2
    // asm: 	CMPF	HALFPI,R2
    // asm: 	BNE	PR2A3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PR2A1", 0, 0);
    UNIMPL();
}

void PR3(void)
{
    // asm: 	FLOAT	PLACE_FRAMES,R1
    // asm: 	LDF	PLACE_ENDRADX,R0
    // asm: 	SUBF	HALFPI,R0		;Allready rotated 90deg
    // asm: 	CALL	DIV_F
    // asm: 	LDI	*+AR7(PLATEOBJ),AR2
    // asm: 	ADDF	*+AR2(ORADX),R0
    // asm: 	CMPF	PLACE_ENDRADX,R0
    // asm: 	LDFGT	PLACE_ENDRADX,R0
    // asm: 	STF	R0,*+AR2(ORADX)
    // asm: 	LDF	R0,R2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	*+AR7(MISPLATEOBJ),AR2
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	LDF	*+AR2(OPOSY),R0
    // asm: 	SUBF	*+AR0(OPOSY),R0
    // asm: 	FLOAT	AR5,R1
    // asm: 	CALL	DIV_F
    // asm: 	LDF	R0,R2
    // asm: 	ADDF	*+AR0(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	CMPF	*+AR7(CAMY),R0
    // asm: 	BGT	PR3A			;Don't move the camera until the plate is above center
    // asm: 	SUBF	*+AR7(CAMY),R0
    // asm: 	MPYF	0.20,R0
    // asm: 	ADDF	*+AR7(CAMY),R0
    // asm: 	FLOAT	-2150,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,*+AR7(CAMY)
PR3A:
    // asm: 	LDF	*+AR2(OPOSZ),R0
    // asm: 	SUBF	*+AR0(OPOSZ),R0
    // asm: 	FLOAT	AR5,R1
    // asm: 	CALL	DIV_F
    // asm: 	LDF	R0,R2
    // asm: 	ADDF	*+AR0(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDF	*+AR2(OPOSX),R0
    // asm: 	SUBF	*+AR0(OPOSX),R0
    // asm: 	FLOAT	AR5,R1
    // asm: 	CALL	DIV_F
    // asm: 	LDF	R0,R2
    // asm: 	ADDF	*+AR0(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	LDF	R2,R0
    // asm: 	NEGF	R0
    // asm: 	CALL	MOVE_PLAYERS_LETTERS
    // asm: 	FLOAT	ROT_FRAMES,R1		;Pan camera to correct y
    // asm: 	LDF	-CAM_RADX,R0
    // asm: 	CALL	DIV_F
    // asm: 	LDP	@_CAMERARAD
    // asm: 	LDF	@_CAMERARAD,R2
    // asm: 	ADDF	R0,R2
    // asm: 	CMPF	-0.01,R2
    // asm: 	LDFGT	0,R2
    // asm: 	STF	R2,@_CAMERARAD
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	NEGF	R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	CAMX,AR2
    // asm: 	LDI	@CAMERAPOSI,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,PR3
    // ;Now fly the bolts in
    // asm: 	LDI	*+AR7(PLACE),R4	;Calculate the first bolts object number
    // asm: 	MPYI	4,R4
    // asm: 	ADDI	FIRST_BOLT,R4
    // asm: 	LDI	3,AR5
PR3B:
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR3BA
    // *ELP END CHANGE
    // asm: 	LDI	R4,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	BC	PR3BA
    // asm: 	LDI	AR0,AR4
    // asm: 	CREATE	FLY_BOLT,UTIL_C
    // *ELP CHANGE
    // 	;SLEEP	15
    // asm: 	SLEEP	6
    // *ELP END CHANGE
    // asm: 	ADDI	1,R4
    // asm: 	DBU	AR5,PR3B
PR3BA:
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR5A
    // *ELP END CHANGE
    // ;If not in top 5 skip the pan up
    // asm: 	LDI	*+AR7(PLACE),R0
    // asm: 	CMPI	4,R0
    // asm: 	BGT	PR5A
    // ;Continue to pan the camera to the top of the highscore table
PR4A:
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PR5A
    // *ELP END CHANGE
    // asm: 	SLEEP	1
    // asm: 	LDP	@_CAMERAPOS+Y
    // asm: 	LDF	@_CAMERAPOS+Y,R0
    // asm: 	ADDF	-20,R0
    // asm: 	FLOAT	-2150,R1
    // asm: 	CMPF	R1,R0
    // asm: 	LDFLT	R1,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	SETDP
    // asm: 	CMPF	R1,R0
    // asm: 	BNE	PR4A
PR5A:
    // asm: 	LDL	plate_darkp,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	STI	R0,*+AR7(darkp_pal)
    // asm: 	LDL	plate_medp,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	STI	R0,*+AR7(medp_pal)
    // asm: 	LDL	plate_lightp,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	STI	R0,*+AR7(lightp_pal)
    // asm: 	LDL	plate_lightp1,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	STI	R0,*+AR7(lightp1_pal)
    // *ELP CHANGE
    // asm: 	LDI	@START_HIT,R0
    // asm: 	BNZ	PRESSCODEX2
    // *ELP END CHANGE
    // asm: 	LDI	*+AR7(RACE_NUMBER),R4
    // asm: 	CREATEC	FLASH_LETTERS_PROC,UTIL_C
    // asm: 	STI	AR0,*+AR7(FLASH_PROC)
    // asm: PRESS_CODEX
    // ;	SLEEP	60
    // *ELP CHANGE
    // 	;SLEEP	90
    // asm: 	SLEEP	30
    // *ELP END CHANGE
    // asm: 	SOND1	CLOSING
PRESSCODEX2:
    // *ELP CHANGE
    // 	;CREATE	UNFOLDMAP_NOPAL,UTIL_C
    // asm: 	CREATE	UNFOLDMAP,UTIL_C
    // 	;SLEEP	40
    // asm: 	SLEEP	20
    // *ELP END CHANGE
    // asm: 	LDI	*+AR7(FLASH_PROC),AR2
    // asm: 	CALL	PRC_KILL
    // asm: 	LDL	press_PALETTES,AR2
    // asm: 	CALL	dealloc_section
    // asm: 	LDI	*+AR7(GREY_PAL),AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDI	*+AR7(WHITE_PAL),AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDI	*+AR7(darkp_pal),AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDI	*+AR7(medp_pal),AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDI	*+AR7(lightp_pal),AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDI	*+AR7(lightp1_pal),AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	LDI	@_MODE,R0
    // asm: 	ANDN	MMODE|MINFIN|MHS,R0
    // asm: 	OR	MBONUS,R0
    // asm: 	STI	R0,@_MODE
    // asm: 	RETP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PR3", 0, 0);
    UNIMPL();
}

void INIT_PRESS_OBJECTS(void)
{
    // asm: 	LDI	9h,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	LDI	81h,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	82h,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	83h,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	@OACTIVEI,AR0
    // asm: 	LDI	*AR0,AR5
FPO:
    // asm: LDI	AR5,R0
    // asm: 	BZ	FPOX
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR5
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	AND	0FFh,R0
    // asm: 	CMPI	PRESS,R0
    // asm: 	BNE	FPO1
    // asm: 	STI	AR0,*+AR7(PRESSOBJ)
    // asm: 	LDF	PRESS_RADX,R2
    // asm: 	STF	R2,*+AR0(ORADX)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	BR	FPLE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_PRESS_OBJECTS", 0, 0);
    UNIMPL();
}

void FPO1(void)
{
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	AND	0FFh,R0
    // asm: 	CMPI	PRESSB,R0
    // asm: 	BNE	FPO1A
    // asm: 	STI	AR0,*+AR7(BPRESSOBJ)
    // asm: 	BR	FPLE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FPO1", 0, 0);
    UNIMPL();
}

void FPO1A(void)
{
    // asm: 	CMPI	PLATE,R0
    // asm: 	BNE	FPO3
    // asm: 	STI	AR0,*+AR7(PLATEOBJ)
    // asm: 	LDF	*+AR0(OPOSZ),R2
    // asm: 	FLOAT	PLATE_ZOFF,R0
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSZ)
    // asm: 	BR	FPLE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FPO1A", 0, 0);
    UNIMPL();
}

void FPO3(void)
{
    // asm: 	CMPI	ARMS,R0
    // asm: 	BNE	FPO4
    // asm: 	STI	AR0,*+AR7(ARMSOBJ)
    // asm: 	BR	FPLE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FPO3", 0, 0);
    UNIMPL();
}

void FPO4(void)
{
    // asm: 	CMPI	SCROLLB,R0
    // asm: 	BNE	FPO6
    // asm: 	LDI	*+AR7(WHITE_PAL),R2	;This is the palette for the letters
    // asm: 	STI	R2,*+AR0(OPAL)
    // asm: 	STI	AR0,*+AR7(SCROLLBOBJ)
    // asm: 	BR	FPLE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FPO4", 0, 0);
    UNIMPL();
}

void FPO6(void)
{
    // ;	CMPI	FRAME,R0
    // ;	BNE	FPO7
    // ;	STI	AR0,*+AR7(FRAMEOBJ)
    // asm: 	BR	FPLE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FPO6", 0, 0);
    UNIMPL();
}

void FPO7(void)
{
FPLE:
    // asm: 	BR	FPO
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FPO7", 0, 0);
    UNIMPL();
}

void FPOX(void)
{
    // asm: 	CALL	MOVE_PRESSB
    // asm: 	CALL	MAKE_NEW_MARQ
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FPOX", 0, 0);
    UNIMPL();
}

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
    // asm: 	FLOAT	-710+83,R3
    // asm: 	LDI	*+AR7(RACE_NUMBER),AR2
    // asm: 	ADDI	@LEG_NAMESI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	FLOAT	-24,R2
    // asm: 	FLOAT	-341,R4
    // asm: 	LDI	MARQLET,R6	;ID
    // asm: 	CALL	PRINT3D
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_NEW_MARQ", 0, 0);
    UNIMPL();
}

void MOVE_PRESSB(void)
{
    // asm: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm: 	LDI	*+AR7(BPRESSOBJ),AR2		;The PRESS OWNS the plate
    // asm: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm: 	LDI	*+AR7(SCROLLBOBJ),AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	*+AR7(BPRESSOBJ),AR2		;The PRESS OWNS the plate
    // asm: 	LDI	*+AR7(SCROLLBOBJ),AR3
    // asm: 	FLOAT	PRESS_DIAM,R1
    // asm: 	ADDF	60,R1
    // asm: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm: 	STF	R2,*+AR2(ORADX)
    // asm: 	STF	R2,*+AR3(ORADX)
    // asm: 	CALL	_SINE
    // asm: 	NEGF	R0
    // asm: 	MPYF3	R0,R1,R3
    // asm: 	ADDF	*+AR0(OPOSZ),R3		;Translated and offset Z
    // asm: 	STF	R3,*+AR2(OPOSZ)
    // asm: 	ADDF	1,R3			;Ensure that the letters are behind pressb
    // asm: 	STF	R3,*+AR3(OPOSZ)
    // asm: 	CALL	_COSI
    // asm: 	MPYF3	R0,R1,R3
    // asm: 	FLOAT	ARM_BOTTOM+PRESS_DIAM,R2
    // asm: 	ADDF	*+AR0(OPOSY),R3
    // asm: 	CMPF	R2,R3
    // asm: 	LDFGT	R2,R3
    // asm: 	STF	R3,*+AR2(OPOSY)		;Translated And OFFSET Y
    // asm: 	STF	R3,*+AR3(OPOSY)		;Translated And OFFSET Y
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_PRESSB", 0, 0);
    UNIMPL();
}

void MOVE_BIGLET_PRESS(void)
{
    // asm: 	LDI	*+AR7(PRESSOBJ),AR0
    // asm: 	LDI	*+AR7(INIT0),AR2
    // asm: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm: 	ABSF	R2
    // asm: 	ADDF	-HALFPI,R2
    // asm: 	STF	R2,*+AR2(ORADX)
    // asm: 	PUSH	AR2
    // asm: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	@VECTORAI,AR1
    // asm: 	LDF	0,R2
    // asm: 	STF	R2,*AR1
    // asm: 	FLOAT	PRESS_DIAM+60+13,R2
    // asm: 	STF	R2,*+AR1(1)
    // asm: 	FLOAT	-98,R2
    // asm: 	STF	R2,*+AR1(2)
    // asm: 	LDI	@VECTORAI,AR2
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	POP	AR2
    // asm: 	LDI	@VECTORAI,AR1
    // asm: 	FLOAT	ARM_BOTTOM+PRESS_DIAM+13,R2
    // asm: 	LDF	*+AR0(OPOSY),R3
    // asm: 	ADDF	*+AR1(1),R3
    // asm: 	CMPF	R2,R3
    // asm: 	LDFGT	R2,R3
    // asm: 	STF	R3,*+AR2(OPOSY)		;Translated And OFFSET Y
    // asm: 	LDF	*+AR0(OPOSZ),R3
    // asm: 	ADDF	*+AR1(2),R3
    // asm: 	STF	R3,*+AR2(OPOSZ)		;Translated And OFFSET Z
    // asm: 	LDI	*+AR7(INIT0),AR0	;Now use the first letter to set the rest
    // asm: 	LDI	0,R4
MPB:
    // asm: 	LDI	INIT0,IR0
    // asm: 	ADDI	R4,IR0
    // asm: 	LDI	*+AR7(IR0),AR2
    // asm: 	LDF	*+AR0(OPOSY),R3		;Translated and offset Y
    // asm: 	STF	R3,*+AR2(OPOSY)
    // asm: 	LDF	*+AR0(OPOSZ),R3		;Translated and offset Z
    // asm: 	STF	R3,*+AR2(OPOSZ)
    // asm: 	LDF	*+AR0(ORADX),R2		;Get rotation of the PRESS
    // asm: 	STF	R2,*+AR2(ORADX)
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	ADDI	1,R4
    // asm: 	CMPI	3,R4
    // asm: 	BNE	MPB
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_BIGLET_PRESS", 0, 0);
    UNIMPL();
}

void POS_SCROLLB(void)
{
    // asm: 	LDI	*+AR7(SCROLLBOBJ),AR2
    // asm: 	LDI	@POSE,R2		;POSITION of the STEERING WHEEL
    // asm: 	CMPI	LASTCHAR,R2
    // asm: 	LDIGT	LASTCHAR,R2
    // asm: 	LDI	@SCROLLBTABI,AR1
    // asm: 	ADDI	R2,AR1
    // asm: 	FLOAT	*AR1,R2
    // asm: 	ADDF	-24,R2
    // asm: 	SUBF	*+AR2(OPOSX),R2
    // asm: 	MPYF	0.50,R2
    // asm: 	ADDF	*+AR2(OPOSX),R2
    // asm: 	STF	R2,*+AR2(OPOSX)
    // asm: 	LDI	@POSE,R2		;POSITION of the STEERING WHEEL
    // asm: 	CMPI	*+AR2(OVELX),R2
    // asm: 	BEQ	POSBX
    // asm: 	STI	R2,*+AR2(OVELX)
    // asm: 	SONDFX	LETTER_RIGHT
POSBX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "POS_SCROLLB", 0, 0);
    UNIMPL();
}

void ADJUST_ROLLERS(void)
{
    // asm: 	LDP	@_CAMERAPOS
    // asm: 	LDF	@_CAMERAPOS,R4
    // asm: 	SETDP
    // asm: 	LDF	0,R3
ADJRL:
    // asm: 	FLOAT	246,R2
    // asm: 	MPYF	R3,R2
    // asm: 	ADDF	R4,R2
    // asm: 	CALL	FIND_ROLLER
    // asm: 	FLOAT	ROLLER_ZOFF,R2
    // asm: 	ADDF	R3,R2
    // asm: 	ADDF	-11,R2
    // asm: 	STF	R2,*+AR2(OPOSZ)
    // asm: 	FLOAT	-246,R2
    // asm: 	MPYF	R3,R2
    // asm: 	ADDF	R4,R2
    // asm: 	CALL	FIND_ROLLER
    // asm: 	FLOAT	ROLLER_ZOFF,R2
    // asm: 	ADDF	R3,R2
    // asm: 	ADDF	-11,R2
    // asm: 	STF	R2,*+AR2(OPOSZ)
    // asm: 	ADDF	1,R3
    // asm: 	CMPF	7,R3
    // asm: 	BNE	ADJRL
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ADJUST_ROLLERS", 0, 0);
    UNIMPL();
}

void FIND_ROLLER(void)
{
    // asm: 	PUSH	R3			;R3 is used by the routine that calls this
    // asm: 	PUSHF	R3
    // asm: 	LDI	ROLLER_GROUP,R1			;Roller Group
    // asm: 	CALL	OBJ_GFIND
    // asm: 	FLOAT	32000,R3		;ANY large number will do
    // asm: 	LDI	AR0,AR2			;Make sure AR2 points to something
FRL:
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	SUBF	R2,R0
    // asm: 	ABSF	R0
    // asm: 	CMPF	R3,R0
    // asm: 	LDFLT	R0,R3
    // asm: 	LDILT	AR0,AR2
    // asm: 	CALL	OBJ_GFIND_NEXT
    // asm: 	BNC	FRL
    // asm: 	POPF	R3
    // asm: 	POP	R3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_ROLLER", 0, 0);
    UNIMPL();
}

void SPIN_ROLLERS(void)
{
    // asm: 	LDI	ROLLER_GROUP,R1		;Roller group
    // asm: 	CALL	OBJ_GFIND
SR0:
    // asm: 	LDF	*+AR0(ORADZ),R2
    // asm: 	ADDF	R0,R2
    // asm: 	STF	R2,*+AR0(ORADZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	CALL	OBJ_GFIND_NEXT
    // asm: 	BNC	SR0
    // ;NOW move the plate
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	FLOAT	ROLLER_DIAM,R1
    // asm: 	MPYF	R1,R0		;R = 111, R0 = RADS rotated
    // 				;Circumfrence = 2*PI*R NOTE R0=2PI/THETA
    // asm: 	LDF	*+AR0(OPOSX),R2
    // asm: 	SUBF	R0,R2
    // asm: 	STF	R2,*+AR0(OPOSX)
    // asm: 	CALL	MOVE_PLAYERS_LETTERS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "SPIN_ROLLERS", 0, 0);
    UNIMPL();
}

void MOVE_PLAYERS_LETTERS(void)
{
    // asm: 	PUSH	R6
    // asm: 	PUSHF	R6
    // asm: 	LDI	*+AR7(PLACE),R2
    // asm: 	ADDI	16,R2
    // asm: 	LDI	1,R1
    // asm: 	LSH	R2,R1
    // asm: 	CALL	OBJ_GFIND
    // asm: 	BC	MPLX			;They haven't been made yet
    // asm: 	LDI	*+AR7(PLATEOBJ),AR1
    // asm: 	LDF	*+AR1(ORADX),R2
    // asm: 	ADDF	HALFPI,R2
    // asm: 	FLOAT	LETTER_YOFF,R3
    // asm: 	ADDF	*+AR1(OPOSY),R3
    // asm: 	LDF	*+AR1(OPOSZ),R4
    // asm: 	SUBF	1,R4			;Make sure the letters are in front
MPL1:
    // asm: 	LDF	*+AR0(OPOSX),R5
    // asm: 	SUBF	R0,R5
    // asm: 	STF	R5,*+AR0(OPOSX)
    // asm: 	STF	R3,*+AR0(OPOSY)
    // asm: 	STF	R4,*+AR0(OPOSZ)
    // asm: 	LDF	*+AR0(ORADX),R6
    // asm: 	CMPF	-PI,R6
    // asm: 	BEQ	MPL2
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
MPL2:
    // asm: 	CALL	OBJ_GFIND_NEXT
    // asm: 	BNC	MPL1
MPLX:
    // asm: 	POPF	R6
    // asm: 	POP	R6
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVE_PLAYERS_LETTERS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MAKE_PLAYERS_PLATE(void)
{
    // asm: 	LDI	*+AR7(RACE_NUMBER),R6			;Race number
    // asm: 	LDI	*+AR7(PLACE),R7				;Entry number
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	CALL	TABLE_ENTRY_READ
    // asm: 	STI	R1,*+AR7(LETTER0)
    // asm: 	STI	R2,*+AR7(LETTER1)
    // asm: 	STI	R3,*+AR7(LETTER2)
    // asm: 	LDI	*+AR7(PLATEOBJ),AR0
    // asm: 	LDI	*+AR7(PLACE),R1
    // asm: 	CALL	CREATE_LETTERS
    // asm: 	LDF	0,R0
    // asm: 	CALL	MOVE_PLAYERS_LETTERS
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_PLAYERS_PLATE", 0, 0);
    UNIMPL();
}

void INI_PLAYERS_BOLTS(void)
{
    // asm: 	LDI	*+AR7(PLACE),R1	;Calculate the first bolts object number
    // asm: 	MPYI	4,R1
    // asm: 	ADDI	FIRST_BOLT,R1
    // asm: 	LDI	3,AR5
    // asm: IPB1
    // asm: 	CALL	OBJ_FIND
    // asm: 	BC	IPBX
    // asm: 	FLOAT	-2500,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	ADDI	1,R1
    // asm: 	DBU	AR5,IPB1
IPBX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INI_PLAYERS_BOLTS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*PROC 	FLY BOLT
*
 */
void FLY_BOLT(void)
{
    // asm: 	SONDFX	DD2
    // asm: 	LDI	15,AR5
    // asm: FBL
    // asm: 	FLOAT	1000,R1
    // asm: 	LDF	*+AR4(OPOSZ),R3
    // asm: 	SUBF	R3,R1
    // asm: 	MPYF	0.24,R1
    // asm: 	ADDF	*+AR4(OPOSZ),R1
    // asm: 	STF	R1,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR4(ORADZ),R2		;Now spin the little fckers
    // asm: 	ADDF	0.0873,R2
    // asm: 	STF	R2,*+AR4(ORADZ)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_ZMATRIX
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,FBL
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLY_BOLT", 0, 0);
    UNIMPL();
}

void OBJ_FIND(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	AR5
    // asm: 	LDI	@OACTIVEI,AR0
    // asm: 	LDI	*AR0,AR5
OFIND:
    // asm: LDI	AR5,R0		;Search for the plate
    // asm: 	BZ	OH_NO
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR5
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	AND	0FFh,R0
    // asm: 	CMPI	R1,R0
    // asm: 	BNE	OFIND
    // asm: 	CLRC			;GOT IT
O_FOUND:
    // asm: 	POP	AR5
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FIND", 0, 0);
    UNIMPL();
}

void OH_NO(void)
{
    // asm: 	SETC
    // asm: 	BR	O_FOUND		;OBJECT NOT found
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OH_NO", 0, 0);
    UNIMPL();
}

void OBJ_FIND_NEXT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	AR5
    // asm: 	LDI	*AR0,AR5
OFN:
    // asm: LDI	AR5,R0
    // asm: 	LDI	R0,AR0
    // asm: 	BZ	OFNX		;NONE FOUND. This routine is passive
    // asm: 	LDI	*AR0,AR5
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	AND	0FFh,R0
    // asm: 	CMPI	R1,R0
    // asm: 	BNE	OFN
OFNX:
    // asm: 	CLRC
    // asm: 	CMPI	0,AR0
    // asm: 	BNE	OFNX1
    // asm: 	SETC
OFNX1:
    // asm: 	POP	AR5
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_FIND_NEXT", 0, 0);
    UNIMPL();
}

void OBJ_GFIND(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	AR5
    // asm: 	LDI	@OACTIVEI,AR0
    // asm: 	LDI	*AR0,AR5
OGFIND:
    // asm: LDI	AR5,R0		;Search for the plate
    // asm: 	BZ	OH_GNO
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	*AR0,AR5
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	TSTB	R1,R0
    // asm: 	BZ	OGFIND
    // asm: 	CLRC
    // asm: O_GFOUND			;GOT IT
    // asm: 	POP	AR5
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_GFIND", 0, 0);
    UNIMPL();
}

void OH_GNO(void)
{
    // asm: 	SETC
    // asm: 	LDI	0,AR0
    // asm: 	BR	O_GFOUND		;OBJECT NOT found
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OH_GNO", 0, 0);
    UNIMPL();
}

void OBJ_GFIND_NEXT(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSHF	R0
    // asm: 	PUSH	AR5
    // asm: 	LDI	*AR0,AR5
OGFN:
    // asm: LDI	AR5,R0
    // asm: 	LDI	R0,AR0
    // asm: 	BZ	OGFNX		;NONE FOUND. This routine is passive
    // asm: 	LDI	*AR0,AR5
    // asm: 	LDI	*+AR0(OID),R0
    // asm: 	TSTB	R1,R0
    // asm: 	BZ	OGFN
OGFNX:
    // asm: 	CLRC
    // asm: 	CMPI	0,AR0
    // asm: 	BNE	OGFNX1
    // asm: 	SETC
OGFNX1:
    // asm: 	POP	AR5
    // asm: 	POPF	R0
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_GFIND_NEXT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*This code can be deleted after I can change the ID numbers for the PRESS Group
* PARAMETERS AR3 = packed list of object ID's to tag
*	R2  = What to tage them with. NOTE R2 will be orred with the ID
 */
void OBJ_TAG(void)
{
OTAG0:
    // asm: 	LDI	*AR3++,R1	;Get ID looking for
    // asm: 	CMPI	0FFh,R1		;Is it a range of ID's?
    // asm: 	BLE	OTAG
    // asm: 	LDI	R1,R0		;YES, Decode the end
    // asm: 	AND	0FFh,R0
    // asm: 	LSH	-8,R1		;Decode the start
OTAG1:
    // asm: 	CALL	OBJ_TAGALL	;Remove the range of objects
    // asm: 	ADDI	1,R1
    // asm: 	CMPI	R1,R0
    // asm: 	BGE	OTAG1
    // asm: 	BR	OTAG0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_TAG", 0, 0);
    UNIMPL();
}

void OTAG(void)
{
    // asm: 	CMPI	0,R1
    // asm: 	BEQ	OTAGX
    // asm: 	CALL	OBJ_TAGALL
    // asm: 	BR	OTAG0
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OTAG", 0, 0);
    UNIMPL();
}

void OTAGX(void)
{
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OTAGX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*This code can be deleted after I can change the ID numbers for the PRESS Group
*	R1 = Object looking for
*	R2  = What to tage them with. NOTE R2 will be orred with the ID
 */
void OBJ_TAGALL(void)
{
    // asm: 	CALL	OBJ_FIND
    // asm: 	BC	OTAX
    // asm: 	LDI	*+AR0(OID),R3
    // asm: 	OR	R2,R3
    // asm: 	STI	R3,*+AR0(OID)
OTA1:
    // asm: 	CALL	OBJ_FIND_NEXT
    // asm: 	BC	OTAX
    // asm: 	LDI	*+AR0(OID),R3
    // asm: 	OR	R2,R3
    // asm: 	STI	R3,*+AR0(OID)
    // asm: 	BR	OTA1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_TAGALL", 0, 0);
    UNIMPL();
}

void OTAX(void)
{
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OTAX", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*OBJ_MOVY_GROUP
*	PARAMETERS	R1 = Bit of OID to search for
*	PARAMETERS	R2 = FLOAT amount to move YPOS
*
 */
void OBJ_MOVY_GROUP(void)
{
    // asm: 	PUSH	R3
    // asm: 	PUSHF	R3
    // asm: 	CALL	OBJ_GFIND
    // asm: 	BC	OMYG
    // asm: 	LDF	*+AR0(OPOSY),R3
    // asm: 	ADDF	R2,R3
    // asm: 	STF	R3,*+AR0(OPOSY)
OMYG1:
    // asm: 	CALL	OBJ_GFIND_NEXT
    // asm: 	BC	OMYG
    // asm: 	LDF	*+AR0(OPOSY),R3
    // asm: 	ADDF	R2,R3
    // asm: 	STF	R3,*+AR0(OPOSY)
    // asm: 	BR	OMYG1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_MOVY_GROUP", 0, 0);
    UNIMPL();
}

void OMYG(void)
{
    // asm: 	POPF	R3
    // asm: 	POP	R3
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OMYG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*OBJ_DEL_GROUP
*	PARAMETERS	R1 = Bit of OID to search for
*
*
 */
void OBJ_DEL_GROUP(void)
{
    // asm: 	CALL	OBJ_GFIND
    // asm: 	BC	ODG
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
ODG1:
    // asm: 	CALL	OBJ_GFIND_NEXT
    // asm: 	BC	ODG
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	BR	ODG1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_DEL_GROUP", 0, 0);
    UNIMPL();
}

void ODG(void)
{
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ODG", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void FIND_ALL_PLATES(void)
{
    // asm: 	LDI	15,R0			;HE WILL NEVER GET TO THIS PLACE
    // asm: 	STI	R0,*+AR7(PLACE)
    // asm: 	CALL	FIND_PLATES
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_ALL_PLATES", 0, 0);
    UNIMPL();
}

void FIND_PLATES(void)
{
    // asm: 	PUSH	AR5
    // asm: 	LDI	*+AR7(RACE_NUMBER),R6		;Race number
    // asm: 	LDI	0,R7				;Entry number
    // asm: 	CALL	GET_TABLE_ADDR
    // asm: 	STI	AR2,*+AR7(HSPOINTER)
    // asm: 	LDI	0,AR5
MP0:
    // asm: 	CMPI	*+AR7(PLACE),AR5
    // asm: 	BEQ	MP1		;Skip creating the letters and score for the player
    // asm: 	LDI	*+AR7(HSPOINTER),AR2
    // asm: 	CALL	TABLE_ENTRY_READ
    // asm: 	STI	AR2,*+AR7(HSPOINTER)
    // asm: 	STI	R1,*+AR7(LETTER0)
    // asm: 	STI	R2,*+AR7(LETTER1)
    // asm: 	STI	R3,*+AR7(LETTER2)
    // asm: 	CALL	MAKE_TIME
    // asm: 	LDI	AR5,R1
    // asm: 	ADDI	FIRST_PLATE,R1
    // asm: 	CALL	OBJ_FIND	;Get the pointer to the plate
    // asm: 	LDI	AR5,R1
    // asm: 	CALL	CREATE_LETTERS
MP2:
    // asm: 	ADDI	1,AR5
    // asm: 	CMPI	NUM_PLATES,AR5
    // asm: 	BLE	MP0
    // asm: 	POP	AR5
    // asm: 	RETS
    // asm: MP1				;Skip this plate
    // asm: 	LDI	*+AR7(HSPOINTER),AR2
    // asm: 	CALL	TABLE_ENTRY_READ
    // asm: 	STI	AR2,*+AR7(HSPOINTER)
    // asm: 	LDI	AR5,R1
    // asm: 	ADDI	FIRST_PLATE,R1
    // asm: 	CALL	OBJ_FIND	;Get the pointer to the plate
    // asm: 	STI	AR0,*+AR7(MISPLATEOBJ)	;Save its pointer for latter use
    // asm: 	LDI	@SCORE,R0
    // asm: 	CALL	MAKE_TIME
    // asm: 	BR	MP2
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIND_PLATES", 0, 0);
    UNIMPL();
}

void MAKE_NUMBERS(void)
{
    // asm: 	LDI	R0,R2
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	TEMP_STR,AR2
    // asm: 	CALL	_itoa
    // asm: 	CALL	FORMAT_NUM
    // asm: 	LDI	AR7,AR4
    // asm: 	ADDI	TEMP_STR,AR4
    // asm: 	LDI	FIRST_NUMBER,R1
    // asm: 	ADDI	AR5,R1			;AR5 = the place
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDI	*AR4++,IR0
    // asm: 	LDI	@NUMTABI,AR1
    // asm: 	LDI	*+AR1(IR0),R0
    // asm: 	STI	R0,*+AR0(OROMDATA)	;Replace it with the proper number
    // asm: 	LDI	HIGH_SCORE_GROUP,R0		;Make this part of the High Score group
    // asm: 	STI	R0,*+AR0(OID)
    // asm: 	LDI	5,AR6		;Now make 6 more
    // asm: MNLOOP
    // asm: 	LDI	AR0,AR1
    // asm: 	LDI	*AR4++,IR0
    // asm: 	LDI	@NUMTABI,AR0
    // asm: 	LDI	*+AR0(IR0),AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	LDI	HIGH_SCORE_GROUP,R0		;Make this part of the High Score group
    // asm: 	STI	R0,*+AR0(OID)
    // asm: 	LDF	*+AR1(OPOSX),R0
    // asm: 	FLOAT	-145,R1			;WIDTH OF ONE NUMBER
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR1(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR1(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDF	0,R2
    // asm: 	STF	R2,*+AR0(ORADX)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	DBU	AR6,MNLOOP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_NUMBERS", 0, 0);
    UNIMPL();
}

void MAKE_TIME(void)
{
    // asm: 	CMPI	0,R0			;Range check time for bogus values
    // asm: 	LDILT	0,R0
    // asm: 	LDI	@LONGEST_TIME,R1
    // asm: 	CMPI	R1,R0
    // asm: 	LDIGT	R1,R0
    // asm: 	CALL	CVTTIME
    // asm: 	MPYI	100,R1
    // asm: 	ADDI	R1,R0
    // asm: 	MPYI	10000,R2
    // asm: 	ADDI	R0,R2
    // ;	CALL	CONVERT_TIME
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	TEMP_STR,AR2
    // asm: 	CALL	_itoa
    // asm: 	CALL	FORMAT_NUM
    // asm: 	LDI	AR7,AR4
    // asm: 	ADDI	TEMP_STR,AR4
    // asm: 	LDI	FIRST_NUMBER,R1
    // asm: 	ADDI	AR5,R1			;AR5 = the place
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDI	*AR4++,IR0
    // asm: 	LDI	@NUMTABI,AR1
    // asm: 	LDI	*+AR1(IR0),R0
    // asm: 	STI	R0,*+AR0(OROMDATA)	;Replace it with the proper number
    // asm: 	LDI	HIGH_SCORE_GROUP,R0		;Make this part of the High Score group
    // asm: 	STI	R0,*+AR0(OID)
    // asm: 	LDI	6,AR6		;Now make 6 more
    // asm: MTLOOP
    // asm: 	LDI	AR0,AR1
    // asm: 	FLOAT	-145,R1			;WIDTH OF ONE NUMBER
    // asm: 	CMPI	5,AR6
    // asm: 	BEQ	MT1
    // asm: 	CMPI	2,AR6
    // asm: 	BNE	MT2
MT1:
    // asm: 	LDL	dcol,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	FLOAT	-82,R1			;WIDTH OF ONE COLON
    // asm: 	BR	MT3
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAKE_TIME", 0, 0);
    UNIMPL();
}

void MT2(void)
{
    // asm: 	LDI	*AR4++,IR0
    // asm: 	LDI	@NUMTABI,AR0
    // asm: 	LDI	*+AR0(IR0),AR2
    // asm: 	CALL	OBJ_GETE
MT3:
    // asm: 	LDI	HIGH_SCORE_GROUP,R0		;Make this part of the High Score group
    // asm: 	STI	R0,*+AR0(OID)
    // asm: 	LDF	*+AR1(OPOSX),R0
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	LDF	*+AR1(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR1(OPOSZ),R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDF	0,R2
    // asm: 	STF	R2,*+AR0(ORADX)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	DBU	AR6,MTLOOP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MT2", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*READS ASCI NUMBER and reformats it to 7 digit display
*	AR2 = string
 */
void FORMAT_NUM(void)
{
    // asm: 	LDI	0,R1
    // asm: 	LDI	0,R2
    // asm: 	LDI	AR2,AR1
FORMN_LOOP:
    // asm: 	CMPI	0,R2
    // asm: 	BNE	FORMNA
    // asm: 	LDI	*AR2++,R0
FORMNA:
    // asm: LDI	R0,R3
    // asm: 	LSH	R2,R3
    // asm: 	SUBI	8,R2
    // asm: 	CMPI	-24,R2
    // asm: 	LDILT	0,R2
    // asm: 	AND	0FFh,R3
    // asm: 	CMPI	MINUS_CHAR,R3
    // asm: 	BEQ	FORMN			;Disregard minus sign
    // asm: 	CMPI	0,R3
    // asm: 	BEQ	FN_PAD			;Reached end
    // asm: 	SUBI	'0',R3
    // asm: 	PUSH	R3
    // asm: 	ADDI	1,R1
FORMN:
    // asm: 	CMPI	6,R1
    // asm: 	BLE	FORMN_LOOP
FN_PAD:
    // asm: 	SUBI	1,R1
    // asm: 	LDI	R1,RC
    // asm: 	RPTB	FORMN1
    // asm: 	POP	R0
FORMN1:
    // asm: STI	R0,*AR1++
    // asm: 	CMPI	6,R1
    // asm: 	BEQ	FORMNX
    // asm: 	LDI	0,R0
    // asm: 	NEGI	R1
    // asm: 	ADDI	6,R1
    // asm: 	BN	FORMNX
    // asm: 	LDI	R1,RC
    // asm: 	RPTB	FORMN2
FORMN2:
    // asm: STI	R0,*AR1++
FORMNX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FORMAT_NUM", 0, 0);
    UNIMPL();
}

void CREATE_LETTERS(void)
{
    // asm: 	LDI	AR7,AR3
    // asm: 	ADDI	LETTER0,AR3
    // asm: 	ADDI	16,R1
    // asm: 	LDI	1,R2
    // asm: 	LSH	R1,R2
    // asm: 	OR	HIGH_SCORE_GROUP,R2		;Make this part of the High Score group
    // asm: 	LDI	AR0,AR1
    // asm: 	LDF	0,R4
CRLLOOP:
    // asm: 	LDI	*AR3++,AR2
    // asm: 	AND	07Fh,AR2		;EXTRACT the CHAR
    // asm: 	CMPI	0,AR2
    // asm: 	BEQ	CRLX			;String Zero terminated
    // asm: 	CALL	ASCII_TO_OBJ
    // asm: 	BC	CRLX				;OUT OF OBJECTS... BAIL
    // asm: 	FLOAT	LETTER_SIZEX,R0
    // asm: 	MPYF	R4,R0
    // asm: 	FLOAT	LETTER_XOFF,R1
    // asm: 	ADDF	R1,R0
    // asm: 	ADDF	*+AR1(OPOSX),R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	LETTER_YOFF,R0
    // asm: 	ADDF	*+AR1(OPOSY),R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	LDF	*+AR1(OPOSZ),R0
    // asm: 	SUBF	1,R0			;Make sure that it is on top of the plate
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	*+AR7(WHITE_PAL),R0
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	STI	R2,*+AR0(OID)
    // asm: 	PUSH	R2
    // asm: 	LDF	0,R2
    // asm: 	LDI	*-AR3(1),AR2
    // asm: 	CMPI	20h,AR2		;is it a space?
    // asm: 	LDFEQ	-PI,R2
    // asm: 	STF	R2,*+AR0(ORADX)
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	POP	R2
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
    // asm: 	ADDF	1,R4
    // asm: 	CMPF	2,R4
    // asm: 	BLE	CRLLOOP
CRLX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CREATE_LETTERS", 0, 0);
    UNIMPL();
}

void ASCII_TO_OBJ(void)
{
    // asm: 	CMPI	'0',AR2
    // asm: 	LDILT	'0',AR2
    // asm: 	CMPI	'9',AR2
    // asm: 	BGT	CRLL
    // asm: 	ADDI	'Z'+2-'0',AR2
    // asm: 	BR	CRL1
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ASCII_TO_OBJ", 0, 0);
    UNIMPL();
}

void CRLL(void)
{
    // asm: 	CMPI	'Z',AR2
    // asm: 	LDIGT	'Z',AR2
CRL1:
    // asm: 	SUBI	'A',AR2
    // asm: 	CMPI	0,AR2
    // asm: 	LDILT	0,AR2
    // asm: 	CMPI	LASTCHAR,AR2
    // asm: 	LDIGT	LASTCHAR,AR2
    // asm: 	ADDI	@PLATE_LETTERSI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CRLL", 0, 0);
    UNIMPL();
}

void PRINT3D(void)
{
    // asm: 	LDI	AR2,AR1			;I use AR2 as input for continuity with the 2d print
    // asm: 	LDI	0,R1
    // asm: 	PUSH	R1			;This will terminate the string being pulled off the stack
    // asm: 	FLOAT	LETTER3D_SIZEX/2,R5
PR3DFS:
    // asm: 	LDI	*AR1,R0			;Count the number of letters in this group
    // asm: 	LSH	R1,R0
    // asm: 	ADDI	-8,R1
    // asm: 	CMPI	-24,R1
    // asm: 	BGE	PR3DA
    // asm: 	LDI	0,R1
    // asm: 	NOP	*AR1++
PR3DA:
    // asm: 	AND	7Fh,R0
    // asm: 	PUSH	R0
    // asm: 	ADDI	1,IR0
    // asm: 	ADDF	R5,R2			;Center the text's Xpos
    // asm: 	CMPI	0,R0
    // asm: 	BNE	PR3DFS
    // asm: 	SUBF	R5,R2			;Correction for zero terminator
    // asm: 	SUBF	R5,R2			;Correction for first letter
    // asm: 	POP	AR0			;POP the zero terminator
PR3DLOOP:
    // asm: 	POP	AR0
    // asm: 	CMPI	0,AR0			;Not stripped by loop above
    // asm: 	BEQ	PR3DX			;String Zero terminated
    // asm: 	CMPI	20h,AR0			;if its a space, skip it
    // asm: 	BEQ	PR3DNEXT
    // asm: 	CMPI	'0',AR0
    // asm: 	LDIZ	'O',AR0
    // asm: 	CMPI	'1',AR0
    // asm: 	LDIZ	'I',AR0
    // asm: 	SUBI	'A',AR0
    // asm: 	CMPI	0,AR0
    // asm: 	LDILT	0,AR0
    // asm: 	CMPI	THREED_END,AR0
    // asm: 	LDIGT	THREED_END,AR0
    // asm: 	ADDI	@THREED_LETTERSI,AR0
    // asm: 	LDI	*AR0,AR2
    // asm: 	CALL	OBJ_GETE
#if DEBUG
    // asm: 	BC	$
#endif
    // asm: 	STF	R2,*+AR0(OPOSX)
    // asm: 	STF	R3,*+AR0(OPOSY)
    // asm: 	STF	R4,*+AR0(OPOSZ)
    // asm: 	STI	R6,*+AR0(OID)
    // asm: 	PUSHF	R2
    // asm: 	LDF	0,R2
    // asm: 	LDI	AR0,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	POPF	R2
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERT
PR3DNEXT:
    // asm: 	FLOAT	LETTER3D_SIZEX,R0
    // asm: 	SUBF	R0,R2
    // asm: 	BR	PR3DLOOP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PRINT3D", 0, 0);
    UNIMPL();
}

void PR3DX(void)
{
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PR3DX", 0, 0);
    UNIMPL();
}

void DISPLAY_HIGH_SCORES(void)
{
    // asm: 	LDL	scroll_white,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	plate_darkp,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	plate_medp,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	plate_lightp,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	plate_lightp1,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDL	press_PALETTES,AR2	;Load in the palettes
    // asm: 	CALL	alloc_section
    // asm: 	LDI	0,R0			;IGNORE PRVIOUS BUTTON PRESSES
    // asm: 	STI	R0,@RADIO_HS_SWITCH
    // asm: 	STI	R0,@RADIO_HS_SHADOW
    // asm: 	LDI	@ATTRWAVE,AR5
DHSLOOP:
    // asm: 	CLRI	AR0
    // ;	LDP	@SWITCH3
    // ;	NOT	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // ;	LDI	*AR0,R2			;Loff
    // ;	SETDP
    // ;	AND	SW_RADIO_H,R0
    // ;	BNZ	DHSLOOP		;Wait for radio button to come up
    // asm: 	LDI	AR5,AR6
    // asm: 	CREATE	DISPLAY_HS,UTIL_C|DISPLAYHS_T
    // asm: 	LDI	AR0,AR4			;Save PROC incase we need to kill it latter
    // asm: 	LDI	30*7,AR6		;Sleep 7 seconds
    // asm: DHSWAIT
    // asm: 	SLEEP	1
    // ;	CLRI	AR0
    // ;	LDP	@SWITCH3
    // ;	NOT	@SWITCH3,R0   		;READ HARDWARE 0=CLOSED, 1=OPEN
    // ;	LDI	*AR0,R2			;Loff
    // ;	SETDP
    // ;	AND	SW_RADIO_H,R0
    // ;	BNZ	DHSNEXT	;if radio button pressed skip to next HS
    // asm: 	LDI	@RADIO_HS_SWITCH,R0
    // asm: 	CMPI	@RADIO_HS_SHADOW,R0	;Edge trigger
    // asm: 	BZ	DHSNOBUT
    // asm: 	STI	R0,@RADIO_HS_SHADOW
    // asm: 	CMPI	-1,R0			;First trigger
    // asm: 	LDIEQ	-1,AR5			;First trigger set level = -1 (DHSNEXT WILL INC)
    // asm: 	BR	DHSNEXT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_HIGH_SCORES", 0, 0);
    UNIMPL();
}

void DHSNOBUT(void)
{
    // asm: 	DBU	AR6,DHSWAIT
    // asm: 	CALL	OBJ_INIT	;initialize object system (ERASE OLD OBJECTS)
    // asm: 	ADDI	1,AR5
    // asm: 	CMPI	15,AR5
    // asm: 	BGE	DHSLOOPX
    // asm: 	TSTB	1,AR5
    // asm: 	BNE	DHSLOOP
DHSLOOPX:
    // asm: 	BR	CYCLE_ATTR
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DHSNOBUT", 0, 0);
    UNIMPL();
}

void DHSNEXT(void)
{
    // asm: 	ADDI	1,AR5
    // asm: 	CMPI	15,AR5
    // asm: 	BGE	DHSLOOPX
    // asm: 	LDI	UTIL_C|DISPLAYHS_T,R0	;KILL THE OLD PROCS OFF
    // asm: 	LDI	CLASS_M|TYPE_M,R1
    // asm: 	CALL	PRC_KILLALL
    // asm: 	CALL	OBJ_INIT	;initialize object system (ERASE OLD OBJECTS)
    // asm: 	BR	DHSLOOP
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DHSNEXT", 0, 0);
    UNIMPL();
}

void DISPLAY_HS(void)
{
    // asm: 	STI	AR6,*+AR7(RACE_NUMBER)		;Passed by parent proc
    // asm: 	LDL	scroll_white,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,*+AR7(WHITE_PAL)
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@NOAERASE
    // asm: 	LDP	@_CAMERAPOS+X		;Set the initial camera position
    // asm: 	FLOAT	-4700,R0
    // asm: 	STF	R0,@_CAMERAPOS+X
    // asm: 	FLOAT	HS_STARTY,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	FLOAT	HS_STARTZ,R0
    // asm: 	STF	R0,@_CAMERAPOS+Z
    // asm: 	CLRF	R2
    // asm: 	STF	R2,@_CAMERARAD+Y
    // asm: 	SETDP
    // asm: 	LDI	@CAMERAMATRIXI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDL	press_grp,AR2		;Load the objects
    // asm: 	CALL	LOAD_SINGLE_SECTION
    // asm: 	CALL	DELETE_PRESS_OBJECTS	;Loose the extra stuff used for the name entry
    // asm: 	CALL	FIND_ALL_PLATES
    // asm: 	CALL	FIX_PLATES		;Init the plates zpos for the fly in
    // asm: 	FLOAT	120,R2			;Adjust the plates y position for the new Marqee
    // asm: 	LDI	HIGH_SCORE_GROUP,R1		;High score group
    // asm: 	CALL	OBJ_MOVY_GROUP
    // asm: 	LDI	9h,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	CALL	DISPLAY_HSTEXT
    // asm: 	CALL	RESCAN			;Make sure all these changes are shown the next display
    // asm: 	LDI	*+AR7(RACE_NUMBER),R4
    // asm: 	CREATEC	FLASH_LETTERS_PROC,UTIL_C|DISPLAYHS_T|FLASH_ST
    // asm: 	STI	AR0,*+AR7(FLASH_PROC)
DHS0:
    // asm: 	SLEEP	1
    // asm: 	FLOAT	250,R0
    // asm: 	CALL	FLY_PLATES		;Fly the plates onto the back wall
    // asm: 	FLOAT	998,R1
    // asm: 	CMPF	R1,R3
    // asm: 	BLT	DHS0
    // asm: 	SLEEP	30*2
    // asm: 	LDI	HS_ZOOM-1,AR6
    // asm: DHS1
    // asm: 	SLEEP	1
    // asm: 	FLOAT	HS_ZDIFF,R1		;Zoom the camera up close
    // asm: 	LDP	@_CAMERAPOS
    // asm: 	FLOAT	HS_YDIFF,R1		;Pan the y so that the marqee is at the top
    // asm: 	LDF	@_CAMERAPOS+Y,R0
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,@_CAMERAPOS+Y
    // asm: 	FLOAT	HS_ZDIFF,R1
    // asm: 	LDF	@_CAMERAPOS+Z,R0
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,@_CAMERAPOS+Z
    // asm: 	SETDP
    // asm: 	DBU	AR6,DHS1
    // asm: 	SLEEP	30*2			;Pause to see last few names
    // asm: 	LDI	*+AR7(FLASH_PROC),AR2
    // asm: 	CALL	PRC_KILL
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_HS", 0, 0);
    UNIMPL();
}

void FLASH_LETTERS_PROC(void)
{
#if FLASH_ON == 1
    // asm: 	LDI	R4,R1
    // asm: 	CALL	CHECK_LASTHS
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	FLASH_LOCK
    // asm: 	ADDI	16,R0
    // asm: 	LDI	1,R4
    // asm: 	LSH	R0,R4
    // asm: 	OR	HIGH_SCORE_GROUP,R4		;Make this part of the High Score group
    // asm: 	LDI	@FLASH_PALSI,AR6
FLASH_LOOP:
    // asm: 	LDI	*AR6,R0
    // asm: 	LDIN	@FLASH_PALSI,AR6
    // asm: 	LDIN	*AR6++,R0			;THIS will increment allways
    // asm: 	PUSH	R0
    // asm: 	LDI	200,AR2
    // asm: 	CALL	RANDPER
    // asm: 	POP	R0
    // asm: 	LDIC	@scroll_whiteI,R0
    // asm: 	CALL	FLASH_LETTERS
    // asm: 	SLEEP	6
    // asm: 	BR	FLASH_LOOP
#endif
FLASH_LOCK:
    // asm: 	SLEEP	1
    // asm: 	BR	FLASH_LOCK
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_LETTERS_PROC", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*R0 OPAL
*R4 OID
 */
void FLASH_LETTERS(void)
{
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R4,AR2
    // asm: 	CALL	OBJ_FIND_FIRST
    // asm: 	BNC	FLASHX
FLASH_LP:
    // asm: 	STI	R0,*+AR0(OPAL)
    // asm: 	LDI	AR2,R1
    // asm: 	CALL	FIND_NEXT_OBJ
    // asm: 	BNC	FLASH_LP
FLASHX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLASH_LETTERS", 0, 0);
    UNIMPL();
}

void DELETE_PRESS_OBJECTS(void)
{
    // asm: 	LDI	@DELISTI,AR3	;TAG them with 100h
    // asm: 	LDI	100h,R2
    // asm: 	CALL	OBJ_TAG
    // asm: 	LDI	100h,R1		;Now Bag them
    // asm: 	CALL	OBJ_DEL_GROUP
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DELETE_PRESS_OBJECTS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*	FIX_PLATES
*
 */
void FIX_PLATES(void)
{
    // asm: 	PUSH	AR5
    // asm: 	FLOAT	-5000,R0		;Initial Z offset
    // asm: 	LDI	0,AR5
    // asm: 	LDI	1h,R2
    // asm: 	LSH	16,R2
FIXPL:
    // asm: 	LDI	AR5,R1
    // asm: 	ADDI	FIRST_PLATE,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDF	*+AR0(OPOSZ),R3		;Offset the plates Z
    // asm: 	ADDF	R0,R3
    // asm: 	STF	R3,*+AR0(OPOSZ)
    // asm: 	SUBF	1,R3
    // asm: FIXPL1
    // asm: 	LDI	R2,R1
    // asm: 	LSH	1,R2
    // asm: 	CALL	OBJ_GFIND
    // asm: 	BC	FIXPL3			;No letters on this plate, move on. (useually hosed CMOS)
FIXPL2:
    // asm: 	STF	R3,*+AR0(OPOSZ)		;Now do the letters on it
    // asm: 	CALL	OBJ_GFIND_NEXT
    // asm: 	BNC	FIXPL2			;do as many as there are
FIXPL3:
    // asm: 	FLOAT	-500,R1			;PLate to plate offset
    // asm: 	ADDF	R1,R0
    // asm: 	ADDI	1,AR5
    // asm: 	CMPI	9,AR5
    // asm: 	BLE	FIXPL
    // asm: 	POP	AR5
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FIX_PLATES", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*	FLY PLATES
*	R0 = how far to move
*	R3 = position of the last plate
 */
void FLY_PLATES(void)
{
    // asm: 	PUSH	AR5
    // asm: 	LDI	0,AR5
    // asm: 	LDI	1h,R2
    // asm: 	LSH	16,R2
FLPL:
    // asm: 	LDI	AR5,R1
    // asm: 	ADDI	FIRST_PLATE,R1
    // asm: 	CALL	OBJ_FIND
    // asm: 	LDF	*+AR0(OPOSZ),R1
    // asm: 	ADDF	R0,R1
    // asm: 	FLOAT	1000,R3
    // asm: 	CMPF	R3,R1		;BLEW by destination?
    // asm: 	LDFGT	R3,R1
    // asm: 	STF	R1,*+AR0(OPOSZ)
    // asm: 	LDF	R1,R3
    // asm: 	SUBF	1,R3
    // asm: FLPL1
    // asm: 	LDI	R2,R1
    // asm: 	LSH	1,R2
    // asm: 	CALL	OBJ_GFIND
    // asm: 	BC	FLPL3			;No letters ,hosed CMOS?
FLPL2:
    // asm: 	STF	R3,*+AR0(OPOSZ)		;Now do the letters on it
    // asm: 	CALL	OBJ_GFIND_NEXT
    // asm: 	BNC	FLPL2			;do as many as there are
FLPL3:
    // asm: 	ADDI	1,AR5
    // asm: 	CMPI	9,AR5
    // asm: 	BLE	FLPL
    // asm: 	POP	AR5
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FLY_PLATES", 0, 0);
    UNIMPL();
}

void DISPLAY_HSTEXT(void)
{
    // asm: 	FLOAT	-910,R3
    // asm: 	BR	RACE_TEXT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DISPLAY_HSTEXT", 0, 0);
    UNIMPL();
}

void ENTER_HSTEXT(void)
{
    // asm: 	FLOAT	-910-2000,R3			;The marquee is moved up 2000
RACE_TEXT:
    // asm: 	LDI	*+AR7(RACE_NUMBER),AR2
    // asm: 	ADDI	@LEG_NAMESI,AR2
    // asm: 	LDI	*AR2,AR2
    // asm: 	FLOAT	-4700,R2
    // asm: 	FLOAT	-301,R4
    // asm: 	LDI	0,R6	;ID
    // asm: 	CALL	PRINT3D
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ENTER_HSTEXT", 0, 0);
    UNIMPL();
}

void INIT_LOGO(void)
{
    // asm: 	CALL	OBJ_GET
    // asm: 	BC	LOGOX
    // asm: 	LDI	AR0,AR4
    // asm: 	LDIL	logo,R0
    // asm: 	STI	R0,*+AR0(OROMDATA)
    // asm: 	LDI	O_IROT|O_NOROT|O_NOUROT|O_NOUNIV,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	FLOAT	LOGO_X,R0
    // asm: 	STF	R0,*+AR0(OPOSX)
    // asm: 	FLOAT	LOGO_Y,R0
    // asm: 	STF	R0,*+AR0(OPOSY)
    // asm: 	FLOAT	LOGO_Z,R0
    // asm: 	STF	R0,*+AR0(OPOSZ)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
LOGOX:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "INIT_LOGO", 0, 0);
    UNIMPL();
}

void RBMATTR_CHECK(void)
{
    // asm: 	LDI	@_MODE,R0
    // asm: 	TSTB	MHS,R0		;IN HS DISPLAY?
    // asm: 	BEQ	RBMCX
    // asm: 	LDI	@RADIO_HS_SWITCH,R0
    // asm: 	SUBI	1,R0
    // asm: 	STI	R0,@RADIO_HS_SWITCH
RBMCX:
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RBMATTR_CHECK", 0, 0);
    UNIMPL();
}

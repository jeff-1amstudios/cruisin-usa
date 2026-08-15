#include "hud.h"

#include "../core/machine.h"
#include "c30.h"
#include "cmos.h"
#include "dirq.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "motion.h"
#include "obj.h"
#include "pall.h"
#include "sndtab.h"
#include "sys.h"
#include "sysid.h"
#include "text.h"
#include "vunit.h"

/*
 * Source module: asm/HUD.ASM
 */

void MOVEIN_HUD_EQUIP(PROC* p);
void MOVEOUT_HUD_EQUIP(void);
void HUD(void);
void dealloc_section(tSECTION_ALLOC sec /*AR2*/);
static void TACHOMETER_ANIMATE(void);
void FILL_DITHER(int x /*R0*/, int y /*R1*/, int width /*R2*/, int height /*R3*/);
void FILL_PLOT(int x /*R0*/, int y /*R1*/, int width /*R2*/, int height /*R3*/);
static int GEARPAL[];

#define SECTION_PALETTE_ALLOC HARDalloc_section
#define alloc_section HARDalloc_section
#define COUNTDOWN_BUFI COUNTDOWN_BUF
#define MPH_BUFFERI MPH_BUFFER
#define STOPBUFFI STOPBUFFER

extern c3x_reg_t CHEAT;

typedef struct HUD_POSITION_ENTRY {
    u32 score_model;
    u32 suffix_model;
    int score_x_offset;
} HUD_POSITION_ENTRY;

/*
 *----------------------------------------------------------------------------
 *HEADS UP DISPLAY ROUTINES
 *
 */

#define SECTION_COUNTER 0 // 1 = TURN ON UL CRNR
/* asm: COUNTDOWN_BUF	.bss	COUNTDOWN_BUF,2 */
int COUNTDOWN_BUF[2];
/* asm: MPH_BUFFER	.bss	MPH_BUFFER,2 */
int MPH_BUFFER[2];
/* asm: _countdown	.bss	_countdown,1 */
int _countdown;
/* asm: _MPH	.bss	_MPH,1 */
int _MPH;
/* asm: SCORE	.bss	SCORE,1 */
int SCORE;
/* asm: POSITION	.bss	POSITION,1 */
int POSITION;
/* asm: MAXMPH_COUNT	.bss	MAXMPH_COUNT,1 */
c3x_f32_t MAXMPH_COUNT;
/* asm: LASTSEC	.bss	LASTSEC,1 */
int LASTSEC;
/* asm: STOPBUFFI	.word	STOPBUFFER */
#define STOPBUFFI STOPBUFFER
/* asm: STOPBUFFER	.bss	STOPBUFFER,4 */
static int STOPBUFFER[4];
static const char YOURLOSTTXT[] = "YOU ARE LOST";
static const char OFFROADTXT[] = "OFFROAD";
static const char MSAF[] = "SAFETY MAT ACTIVATED";
static const char BSAF[] = "SAFETY BEAM ACTIVATED";
static const char OSAF[] = "FAIL SAFE SWITCH ACTIVATED";
static const char MSSM[] = "MOTION STOP BUTTON HIT";
static const char* const SAFETS[] = {MSAF, BSAF, OSAF, MSSM};
static const char GEARN[] = ";";
static const char GEAR1[] = "1";
static const char GEAR2[] = "2";
static const char GEAR3[] = "3";
static const char GEAR4[] = "4";
static const char* const GEARS[] = {GEARN, GEAR1, GEAR2, GEAR3, GEAR4};
static const HUD_POSITION_ENTRY POS_TABLE[] = {
    {scred1_ROM, dst_ROM, 7},
    {scred2_ROM, dnd_ROM, 0},
    {scred3_ROM, drd_ROM, 0},
    {scred4_ROM, dth_ROM, 0},
    {scred5_ROM, dth_ROM, 0},
    {scred6_ROM, dth_ROM, 0},
    {scred7_ROM, dth_ROM, 0},
    {scred8_ROM, dth_ROM, 0},
    {scred9_ROM, dth_ROM, 0},
    {scred10_ROM, dth_ROM, -20},
};
/* asm: OFFROADBUFF	.bss	OFFROADBUFF,2 */
int OFFROADBUFF[2];
// *----------------------------------------------------------------------------
/* asm: MOVEIN_OFFSET	.bss	MOVEIN_OFFSET,1 */
int MOVEIN_OFFSET;

void MOVEIN_HUD_EQUIP(PROC* p) {
    switch (PROC_RESUME_STATE) {
    case 0:
        MAME_ASSERT_FUNCTION_ENTRY();
        break;
    case 1:
        goto PROC_RESUME_1;
    case 2:
        goto PROC_RESUME_2;
    }

    // asm 00009D0E: 	LDI	150,R0
    // asm 00009D0F: 	STI	R0,@MOVEIN_OFFSET
    MOVEIN_OFFSET = 150;
LIU8:
    // asm 00009D10: LDI	@_MODE,R0
    // asm 00009D11: 	TSTB	MHUD,R0
    // asm 00009D12: 	BNZ	M2L
    if ((_MODE & MHUD) != 0) {
        goto M2L;
    }
    // asm 00009D13: 	SLEEP	1
    SLEEP(1, 1);
    // asm 00009D15: 	BU	LIU8
M2L:
    // asm 00009D16: LDI	25-1,AR5
    p->ctx->MOVEIN_HUD_EQUIP_FRAME.loop_count = 25 - 1;
    // asm 00009D17: MIHEL
MIHEL:
    // asm 00009D17: 	LDI	@MOVEIN_OFFSET,R0
    // asm 00009D18: 	SUBI	6,R0
    // asm 00009D19: 	STI	R0,@MOVEIN_OFFSET
    MOVEIN_OFFSET -= 6;
    // asm 00009D1A: 	SLEEP	1
    SLEEP(1, 2);
    // asm 00009D1C: 	DBU	AR5,MIHEL
    if (p->ctx->MOVEIN_HUD_EQUIP_FRAME.loop_count-- > 0) {
        goto MIHEL;
    }
    // asm 00009D1D: 	CLRI	R0
    // asm 00009D1E: 	STI	R0,@MOVEIN_OFFSET
    MOVEIN_OFFSET = 0;
    // asm 00009D1F: 	DIE
    DIE();
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void MOVEOUT_HUD_EQUIP(void) {
    // asm 00009D20: 	LDI	25-1,AR5
    // asm 00009D21: MIHEL2
    // asm 00009D21: 	LDI	@MOVEIN_OFFSET,R0
    // asm 00009D22: 	ADDI	6,R0
    // asm 00009D23: 	STI	R0,@MOVEIN_OFFSET
    // asm 00009D24: 	SLEEP	1
    // asm 00009D26: 	DBU	AR5,MIHEL2
    // asm 00009D27: 	LDI	150,R0
    // asm 00009D28: 	STI	R0,@MOVEIN_OFFSET
    // asm 00009D29: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MOVEOUT_HUD_EQUIP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *HEADS UP DISPLAY ROUTINE
 *CALL FROM MAIN LOOP
 *
 *SHOULD ONLY BE CALLED DURING GAME PLAY
 *
 */
void HUD(void) {
    tSHADOW_TEXT shadow_text;
    tTEXT* text;
    int palette;
    int speed;
    const HUD_POSITION_ENTRY* position_entry;
    c3x_reg_t value;
    c3x_reg_t frame_count;

    // asm 00009D2A: 	CALL	TACHOMETER_ANIMATE
    TACHOMETER_ANIMATE();
    // asm 00009D2B: 	CALL	RADAR_PLOT
    RADAR_PLOT();
    // 	;
    // 	;MOTION SAFETY ON
    // 	;
    // asm 00009D2C: 	READAUD	ADJ_MOTION_PRESENT
    // asm 00009D2E: 	CMPI	0,R0
    // asm 00009D2F: 	BEQ	NOMOTIONMSGS
    if (READAUD(ADJ_MOTION_PRESENT) == 0) {
        goto NOMOTIONMSGS;
    }
    // asm 00009D30: 	LDI	@MOTION_STOP_HIT,R0
    // asm 00009D31: 	BNZ	NOMOTIONMSGS
    if (MOTION_STOP_HIT != 0) {
        goto NOMOTIONMSGS;
    }
    // asm 00009D32: 	LDI	@MOTION_SAFETY_ON,R0
    // asm 00009D33: 	BZ	BLAHBLAH
    if (MOTION_SAFETY_ON == 0) {
        goto BLAHBLAH;
    }
SAFETSI:
    // asm 00009D34: .word	SAFETS
SAFETS:
    // asm 00009D34: .word	MSAF,BSAF,OSAF,MSSM
    // asm 00009D34: 	LDI	@SAFETSI,AR2
    // asm 00009D35: 	ADDI	@MOTION_SAFETY_TYPE,AR2
    // asm 00009D36: 	LDI	*AR2,AR2
    // asm 00009D37: 	FLOAT	256,R2
    // asm 00009D38: 	FLOAT	310,R3
    // asm 00009D39: 	LDI	1,RC
    // asm 00009D3A: 	CALL	TEXT_ADDDS
    shadow_text = TEXT_ADDDS(SAFETS[MOTION_SAFETY_TYPE], C3X_FROM_INT(256), C3X_FROM_INT(310), 1);
    // asm 00009D3B: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    shadow_text.front->color |= TXT_CENTER;
    // asm 00009D3E: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    shadow_text.shadow->color |= TXT_CENTER;
BLAHBLAH:
    // 	;
    // 	;
NOMOTIONMSGS:
    // 	;-------OFF ROAD TIMER
    // 	;
    // 	;
    // asm 00009D41: 	LDI	@_MODE,R0
    // asm 00009D42: 	ANDN	MMODE,R0
    // asm 00009D43: 	CMPI	MGAME,R0
    // asm 00009D44: 	BNE	NOT_OFFROAD
    if ((_MODE & ~MMODE) != MGAME) {
        goto NOT_OFFROAD;
    }
    // asm 00009D45: 	LDI	@OFFROAD_TMR,R2
    // asm 00009D46: 	CMPI	10,R2
    // asm 00009D47: 	BGE	NOT_OFFROAD
    if (OFFROAD_TMR >= 10) {
        goto NOT_OFFROAD;
    }
    // asm 00009D48: 	CMPI	3,R2
    // asm 00009D49: 	BGT	NOTLOST
    if (OFFROAD_TMR > 3) {
        goto NOTLOST;
    }
    // asm 00009D4A: 	LDL	YOURLOSTTXT,AR2
    // asm 00009D4B: 	FLOAT	256,R2
    // asm 00009D4C: 	FLOAT	5,R3
    // asm 00009D4D: 	LDI	1,RC
    // asm 00009D4E: 	CALL	TEXT_ADDDS
    shadow_text = TEXT_ADDDS(YOURLOSTTXT, C3X_FROM_INT(256), C3X_FROM_INT(5), 1);
    // asm 00009D4F: 	CALL	SET18FONTDS
    SET18FONTDS(&shadow_text);
    // asm 00009D50: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    shadow_text.front->color |= TXT_CENTER;
    // asm 00009D53: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    shadow_text.shadow->color |= TXT_CENTER;
    // asm 00009D56: 	BU	OFJN
    goto OFJN;
NOTLOST:
    // asm 00009D57: 	LDL	OFFROADTXT,AR2
    // asm 00009D58: 	FLOAT	256,R2
    // asm 00009D59: 	FLOAT	5,R3
    // asm 00009D5A: 	LDI	1,RC
    // asm 00009D5B: 	CALL	TEXT_ADDDS
    shadow_text = TEXT_ADDDS(OFFROADTXT, C3X_FROM_INT(256), C3X_FROM_INT(5), 1);
    // asm 00009D5C: 	CALL	SET18FONTDS
    SET18FONTDS(&shadow_text);
    // asm 00009D5D: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    shadow_text.front->color |= TXT_CENTER;
    // asm 00009D60: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    shadow_text.shadow->color |= TXT_CENTER;
OFJN:
    // asm 00009D63: 	LDI	@OFFROAD_TMR,R2
    // asm 00009D64: 	LDL	OFFROADBUFF,AR2
    // asm 00009D65: 	CALL	_itoa
    _itoa((char*)OFFROADBUFF, OFFROAD_TMR);
    // asm 00009D66: 	FLOAT	256,R2
    // asm 00009D67: 	FLOAT	30,R3
    // asm 00009D68: 	LDI	1,RC
    // asm 00009D69: 	CALL	TEXT_ADDDS
    shadow_text = TEXT_ADDDS((char*)OFFROADBUFF, C3X_FROM_INT(256), C3X_FROM_INT(30), 1);
    // asm 00009D6A: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    shadow_text.front->color |= TXT_CENTER;
    // asm 00009D6D: 	ORM	TXT_CENTER,*+AR1(TEXT_COLOR)
    shadow_text.shadow->color |= TXT_CENTER;
NOT_OFFROAD:
    // 	;-------ELAPSED TIME,SPEED TEXT
    // 	;
    // 	;
    // asm 00009D70: 	LDL	rpm,AR2
    // asm 00009D71: 	LDI	420,R2			;R2	POS X
    // asm 00009D72: 	ADDI	@MOVEIN_OFFSET,R2
    // asm 00009D73: 	LDI	290,R3			;R3	POS Y
    // asm 00009D74: 	LDI	TM|ZS,R4
    // asm 00009D75: 	CALL	BLTMOD2D
    BLTMOD2D((const BLTMOD2D_MODEL*)ROM_PTR(rpm_ROM), 420 + MOVEIN_OFFSET, 290, TM | ZS, 0, 0);
    // asm 00009D76: 	LDI	@CHOSEN_TRANSMISSION,R0
    // asm 00009D77: 	CMPI	AUTO_TRANSMISSION,R0
    // asm 00009D78: 	BEQ	ISAUTOTRNS
    if (CHOSEN_TRANSMISSION == AUTO_TRANSMISSION) {
        goto ISAUTOTRNS;
    }
    // asm 00009D79: 	LDL	rpmman,AR2
    // asm 00009D7A: 	LDI	463,R2			;R2	POS X
    // asm 00009D7B: 	ADDI	@MOVEIN_OFFSET,R2
    // asm 00009D7C: 	LDI	380,R3			;R3	POS Y
    // asm 00009D7D: 	LDI	TM|ZS,R4
    // asm 00009D7E: 	CALL	BLTMOD2D
    BLTMOD2D((const BLTMOD2D_MODEL*)ROM_PTR(rpmman_ROM), 463 + MOVEIN_OFFSET, 380, TM | ZS, 0, 0);
    // asm 00009D7F: 	BU	DNDNL
    goto DNDNL;
ISAUTOTRNS:
    // asm 00009D80: 	LDL	rpmauto,AR2
    // asm 00009D81: 	LDI	470,R2			;R2	POS X
    // asm 00009D82: 	ADDI	@MOVEIN_OFFSET,R2
    // asm 00009D83: 	LDI	380,R3			;R3	POS Y
    // asm 00009D84: 	LDI	TM|ZS,R4
    // asm 00009D85: 	CALL	BLTMOD2D
    BLTMOD2D((const BLTMOD2D_MODEL*)ROM_PTR(rpmauto_ROM), 470 + MOVEIN_OFFSET, 380, TM | ZS, 0, 0);
DNDNL:
    // 	;show gear digit
    // 	;
    // 	;
    // asm 00009D86: 	LDI	@PLYCBLK,AR0
    // asm 00009D87: 	LDI	*+AR0(CARGEAR),AR2
    // asm 00009D88: 	ADDI	@GEARI,AR2
    // asm 00009D89: 	LDI	*AR2,AR2
    // asm 00009D8A: 	FLOAT	458,R2			;R2	POS X
    // asm 00009D8B: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm 00009D8C: 	ADDF	R0,R2
    // asm 00009D8D: 	FLOAT	360,R3			;R3	POS Y
    // asm 00009D8E: 	LDI	1,RC
    // asm 00009D8F: 	CALL	TEXT_ADD
    text = TEXT_ADD(GEARS[PLYCBLK->gear], C3X_ADD(C3X_FROM_INT(458), C3X_FROM_INT(MOVEIN_OFFSET)), C3X_FROM_INT(360), 1);
    // asm 00009D90: 	CALL	SETSMDIGITFONT
    SETSMDIGITFONT(text);
GEARI:
    // asm 00009D91: .word	GEARS
GEARS:
    // asm 00009D91: .word	GEARN,GEAR1,GEAR2,GEAR3,GEAR4
    // 	;show MPH or KPH
    // 	;
    // asm 00009D91: 	LDL	mph,AR2
    // asm 00009D92: 	LDI	0,R2			;R2	POS X
    // asm 00009D93: 	SUBI	@MOVEIN_OFFSET,R2
    // asm 00009D94: 	LDI	335,R3			;R3	POS Y
    // asm 00009D95: 	LDI	TM|ZS,R4
    // asm 00009D96: 	CALL	BLTMOD2D
    BLTMOD2D((const BLTMOD2D_MODEL*)ROM_PTR(mph_ROM), -MOVEIN_OFFSET, 335, TM | ZS, 0, 0);
    // asm 00009D97: 	READAUD	ADJ_MPHORKPM
    // asm 00009D99: 	CMPI	0,R0
    // asm 00009D9A: 	BEQ	ISMPHT
    if (READAUD(ADJ_MPHORKPM) == 0) {
        goto ISMPHT;
    }
    // asm 00009D9B: 	LDL	kph,AR2
    // asm 00009D9C: 	LDI	50,R2			;R2	POS X
    // asm 00009D9D: 	SUBI	@MOVEIN_OFFSET,R2
    // asm 00009D9E: 	LDI	385,R3			;R3	POS Y
    // asm 00009D9F: 	LDI	TM|ZS,R4
    // asm 00009DA0: 	CALL	BLTMOD2D
    BLTMOD2D((const BLTMOD2D_MODEL*)ROM_PTR(kph_ROM), 50 - MOVEIN_OFFSET, 385, TM | ZS, 0, 0);
    // asm 00009DA1: 	BU	ISDNN
    goto ISDNN;
ISMPHT:
    // asm 00009DA2: 	LDL	mphmph,AR2
    // asm 00009DA3: 	LDI	50,R2			;R2	POS X
    // asm 00009DA4: 	SUBI	@MOVEIN_OFFSET,R2
    // asm 00009DA5: 	LDI	385,R3			;R3	POS Y
    // asm 00009DA6: 	LDI	TM|ZS,R4
    // asm 00009DA7: 	CALL	BLTMOD2D
    BLTMOD2D((const BLTMOD2D_MODEL*)ROM_PTR(mphmph_ROM), 50 - MOVEIN_OFFSET, 385, TM | ZS, 0, 0);
ISDNN:
    // 	;-------time remaining
    // 	;
    // asm 00009DA8: 	LDL	time,AR2
    // asm 00009DA9: 	LDI	242,R2			;R2	POS X
    // asm 00009DAA: 	LDI	9,R3			;R3	POS Y
    // asm 00009DAB: 	SUBI	@MOVEIN_OFFSET,R3
    // asm 00009DAC: 	LDI	TM|ZS,R4
    // asm 00009DAD: 	CALL	BLTMOD2D_DS
    BLTMOD2D_DS((const BLTMOD2D_MODEL*)ROM_PTR(time_ROM), 242, 9 - MOVEIN_OFFSET, TM | ZS);
    // 	;-------TIME (_countdown)
    // 	;
    // 	;
    // asm 00009DAE: 	LDI	@_countdown,R2
    // asm 00009DAF: 	LDI	@COUNTDOWN_BUFI,AR2
    // asm 00009DB0: 	CALL	_itoa
    _itoa((char*)COUNTDOWN_BUFI, _countdown);
    // asm 00009DB1: 	FLOAT	256,R2
    // asm 00009DB2: 	FLOAT	24,R3
    // asm 00009DB3: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm 00009DB4: 	SUBF	R0,R3
    // asm 00009DB5: 	LDI	1,RC
    // asm 00009DB6: 	CALL	TEXT_ADD
    text = TEXT_ADD((char*)COUNTDOWN_BUFI, C3X_FROM_INT(256), C3X_SUB(C3X_FROM_INT(24), C3X_FROM_INT(MOVEIN_OFFSET)), 1);
    // asm 00009DB7: 	CALL	SETN43FONT
    SETN43FONT(text);
    // asm 00009DB8: 	ORM	TXT_CENTER,*+AR0(TEXT_COLOR)
    text->color |= TXT_CENTER;
    // asm 00009DBB: 	LDL	lgnum43_coolyelo,AR2
    // asm 00009DBC: 	CALL	PAL_FIND_RAW
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(lgnum43_coolyelo_ROM));
    // asm 00009DBD: 	STI	R0,*+AR0(TEXT_PAL)
    text->palette = palette;
    // 	;-------LAST 3/10 SECONDS OF TIME (_countdown)
    // 	;
    // 	;
    // asm 00009DBE: 	LDI	@_MODE,R0
    // asm 00009DBF: 	AND	MMODE,R0
    // asm 00009DC0: 	CMPI	MGAME,R0
    // asm 00009DC1: 	BNE	NBEEP
    if ((_MODE & MMODE) != MGAME) {
        goto NBEEP;
    }
    // asm 00009DC2: 	LDI	@_countdown,R0
    // asm 00009DC3: 	CMPI	10,R0
    // asm 00009DC4: 	BGT	NBEEP
    if (_countdown > 10) {
        goto NBEEP;
    }
    // asm 00009DC5: 	BLT	NOINITBP
    if (_countdown < 10) {
        goto NOINITBP;
    }
    // asm 00009DC6: 	LDI	@LASTSEC,R1
    // asm 00009DC7: 	CMPI	R1,R0
    // asm 00009DC8: 	BEQ	NBEEP
    if (_countdown == LASTSEC) {
        goto NBEEP;
    }
    // asm 00009DC9: 	STI	R0,@LASTSEC
    LASTSEC = _countdown;
    // asm 00009DCA: 	SOND1	GV_TENSECONDS
    SOND1(GV_TENSECONDS);
    // asm 00009DCC: 	BU	NBEEP
    goto NBEEP;
NOINITBP:
    // asm 00009DCD: 	LDI	@LASTSEC,R1
    // asm 00009DCE: 	CMPI	R1,R0
    // asm 00009DCF: 	BEQ	NBEEP
    if (_countdown == LASTSEC) {
        goto NBEEP;
    }
    // asm 00009DD0: 	STI	R0,@LASTSEC
    LASTSEC = _countdown;
    // asm 00009DD1: 	CMPI	3,R0
    // asm 00009DD2: 	BGT	NBEEP
    if (_countdown > 3) {
        goto NBEEP;
    }
    // asm 00009DD3: 	SOND1	BASICBEEPH
    SOND1(BASICBEEPH);
NBEEP:
    // 	;-------SPEED
    // 	;
    // 	;
    // 	;PLOT SPEED
    // asm 00009DD5: 	READAUD	ADJ_MPHORKPM
    // asm 00009DD7: 	CMPI	0,R0
    // asm 00009DD8: 	BEQ	ISMPH
    if (READAUD(ADJ_MPHORKPM) == 0) {
        goto ISMPH;
    }
    // asm 00009DD9: 	FLOAT	@_MPH,R2
    value = C3X_FROM_INT(_MPH);
    // asm 00009DDA: 	MPYF	1.6666,R2
    value = C3X_MUL(value, C3X_IMM_F32(1.6666));
    // asm 00009DDB: 	FIX	R2
    speed = C3X_FIX(value);
    // asm 00009DDC: 	BU	ISKPH
    goto ISKPH;
ISMPH:
    // asm 00009DDD: LDI	@_MPH,R2
    speed = _MPH;
ISKPH:
    // asm 00009DDE: LDI	@MPH_BUFFERI,AR2
    // asm 00009DDF: 	CALL	_itoa
    _itoa((char*)MPH_BUFFERI, speed);
    // asm 00009DE0: 	FLOAT	66,R2
    // asm 00009DE1: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm 00009DE2: 	SUBF	R0,R2
    // asm 00009DE3: 	FLOAT	346,R3
    // asm 00009DE4: 	LDI	1,RC
    // asm 00009DE5: 	CALL	TEXT_ADDDS
    shadow_text = TEXT_ADDDS((char*)MPH_BUFFERI, C3X_SUB(C3X_FROM_INT(66), C3X_FROM_INT(MOVEIN_OFFSET)), C3X_FROM_INT(346), 1);
    // asm 00009DE6: 	CALL	SETLGDIGITFONTDS
    SETLGDIGITFONTDS(&shadow_text);
    // asm 00009DE7: 	ORM	TXT_RIGHT,*+AR0(TEXT_COLOR)
    shadow_text.front->color |= TXT_RIGHT;
    // asm 00009DEA: 	ORM	TXT_RIGHT,*+AR1(TEXT_COLOR)
    shadow_text.shadow->color |= TXT_RIGHT;
    // asm 00009DED: 	LDL	dnums_amber,AR2
    // asm 00009DEE: 	CALL	PAL_FIND_RAW
    palette = PAL_FIND_RAW((tPAL*)ROM_PTR(dnums_amber_ROM));
    // asm 00009DEF: 	STI	R0,*+AR0(TEXT_PAL)
    shadow_text.front->palette = palette;
    // asm 00009DF0: 	STI	R0,*+AR1(TEXT_PAL)
    shadow_text.shadow->palette = palette;
    // 	;-------STOPWATCH (actual elapsed time)
    // 	;
    // 	;
    // asm 00009DF1: 	LDI	8,R0
    // asm 00009DF2: 	LDI	19,R1
    // asm 00009DF3: 	SUBI	@MOVEIN_OFFSET,R1
    // asm 00009DF4: 	LDI	106,R2
    // asm 00009DF5: 	LDI	30,R3
    // asm 00009DF6: 	CALL	FILL_DITHER
    FILL_DITHER(8, 19 - MOVEIN_OFFSET, 106, 30);
    // asm 00009DF7: 	LDL	elap,AR2
    // asm 00009DF8: 	LDI	10,R2			;R2	POS X
    // asm 00009DF9: 	LDI	20,R3			;R3	POS Y
    // asm 00009DFA: 	SUBI	@MOVEIN_OFFSET,R3
    // asm 00009DFB: 	LDI	TM|ZS,R4
    // asm 00009DFC: 	CALL	BLTMOD2D_DS
    BLTMOD2D_DS((const BLTMOD2D_MODEL*)ROM_PTR(elap_ROM), 10, 20 - MOVEIN_OFFSET, TM | ZS);
    // asm 00009DFD: 	LDI	@STOPWATCH,R0
    // asm 00009DFE: 	LDI	@STOPBUFFI,AR2
    // asm 00009DFF: 	CALL	TIME2STR
    TIME2STR((char*)STOPBUFFI, STOPWATCH);
    // asm 00009E00: 	FLOAT	20,R2
    // asm 00009E01: 	FLOAT	33,R3
    // asm 00009E02: 	FLOAT	@MOVEIN_OFFSET,R0
    // asm 00009E03: 	SUBF	R0,R3
    // asm 00009E04: 	LDI	1,RC
    // asm 00009E05: 	CALL	TEXT_ADDDS
    shadow_text = TEXT_ADDDS((char*)STOPBUFFI, C3X_FROM_INT(20), C3X_SUB(C3X_FROM_INT(33), C3X_FROM_INT(MOVEIN_OFFSET)), 1);
    // asm 00009E06: 	CALL	SETSMDIGITFONTDS
    SETSMDIGITFONTDS(&shadow_text);
    // 	;-------POSITION
    // 	;(PLAYERS RANK)
    // 	;
    // asm 00009E07: 	LDI	@POSITION,AR4
    // asm 00009E08: 	SUBI	1,AR4
    // asm 00009E09: 	MPYI	3,AR4
    // asm 00009E0A: 	ADDI	@POS_TABLEI,AR4
    position_entry = &POS_TABLE[POSITION - 1];
    // asm 00009E0B: 	LDI	*AR4++,AR2
    // asm 00009E0C: 	LDI	415,R2
    // asm 00009E0D: 	ADDI	*+AR4,R2
    // asm 00009E0E: 	LDI	20,R3
    // asm 00009E0F: 	SUBI	@MOVEIN_OFFSET,R3
    // asm 00009E10: 	LDI	TM|ZS,R4
    // asm 00009E11: 	CALL	BLTMOD2D_DS
    BLTMOD2D_DS((const BLTMOD2D_MODEL*)ROM_PTR(position_entry->score_model), 415 + position_entry->score_x_offset,
                20 - MOVEIN_OFFSET, TM | ZS);
    // asm 00009E12: 	LDI	*AR4++,AR2
    // asm 00009E13: 	LDI	450,R2
    // asm 00009E14: 	LDI	20,R3
    // asm 00009E15: 	SUBI	@MOVEIN_OFFSET,R3
    // asm 00009E16: 	LDI	TM|ZS,R4
    // asm 00009E17: 	CALL	BLTMOD2D_DS
    BLTMOD2D_DS((const BLTMOD2D_MODEL*)ROM_PTR(position_entry->suffix_model), 450, 20 - MOVEIN_OFFSET, TM | ZS);
    // asm 00009E18: POS_TABLE
#if SECTION_COUNTER
    // 	;-------DEBUGGING SECTION_COUNTER
    // 	;
    // 	;
    // asm: 	LDI	@PLYCBLK,AR0
    // asm: 	LDI	*+AR0(CARTRAK),AR0
    // asm: 	LDI	*+AR0(OUSR1),R2
    // asm: 	RS	8,R2
    // asm: 	LDL	SECIDX,AR2
    // asm: 	CALL	_itoa
    // asm: 	FLOAT	10,R2
    // asm: 	FLOAT	50,R3
    // asm: 	LDI	1,RC
    // asm: 	CALL	TEXT_ADD
#endif
    // 	;-------LOGIC: MAXMPH COMPUTATION
    // 	;(COMPUTE FOR BONUS SCREEN...)
    // 	;
    // 	;
    // asm 00009E18: 	FLOAT	@_MPH,R2
    value = C3X_FROM_INT(_MPH);
    // asm 00009E19: 	MPYF	@CHEAT,R2		;DO THE CHEAT THING !!!
    value = C3X_MUL(value, C3X_REG(CHEAT)); // DO THE CHEAT THING !!!
    // asm 00009E1A: 	FLOAT	@NFRAMES,R0
    frame_count = C3X_FROM_INT(NFRAMES);
    // asm 00009E1B: 	MPYF	R0,R2
    value = C3X_MUL(value, frame_count);
    // asm 00009E1C: 	ADDF	@MAXMPH,R2
    value = C3X_ADD(value, C3X_LDF(MAXMPH));
    // asm 00009E1D: 	STF	R2,@MAXMPH
    MAXMPH = C3X_STF(value);
    // asm 00009E1E: 	ADDF	@MAXMPH_COUNT,R0
    frame_count = C3X_ADD(frame_count, C3X_LDF(MAXMPH_COUNT));
    // ;	LDF	@MAXMPH_COUNT,R0
    // ;	INCF	R0
    // asm 00009E1F: 	STF	R0,@MAXMPH_COUNT
    MAXMPH_COUNT = C3X_STF(frame_count);
    // asm 00009E20: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "HUD", 0, 0);
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *ALLOCATE PALETTES FOR A SECTION
 *
 *PARAMETERS
 *	AR2	PTR TO SECTION HEADER
 *
 */
void HARDalloc_section(tSECTION_ALLOC sec) {
    int palette_code;
    int count;

    // asm 00009E21: 	LDI	*AR2++,AR6
    palette_code = sec.pal_index;
    // asm 00009E22: 	LDI	*AR2,AR5
    MAME_ASSERT_REG(0x00009E23, "AR5", &sec.count);
    // asm 00009E23: 	DEC	AR5
    count = sec.count - 1;
alloc_LPA:
    // asm 00009E24: 	LDI	AR6,AR2
    // asm 00009E25: 	CALL	PAL_ALLOC
    PAL_ALLOC(palette_code);
    // asm 00009E26: 	INC	AR6
    palette_code++;
    // asm 00009E27: 	DBU	AR5,alloc_LPA
    if (count-- > 0) {
        goto alloc_LPA;
    }
    // asm 00009E28: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DEALLOCATE PALETTES USED FOR A SECTION
 *
 *
 *PARAMETERS
 *	AR2	SECTION POINTER
 *
 */
void dealloc_section(tSECTION_ALLOC sec /*AR2*/) {
    int palette_code;
    int count;

    // asm 00009E29: 	PUSH	AR4
    // asm 00009E2A: 	PUSH	AR5
    // asm 00009E2B: 	PUSH	AR6
    // asm 00009E2C: 	LDI	*AR2++,AR6
    palette_code = sec.pal_index;
    // asm 00009E2D: 	LDI	*AR2,AR5
    // asm 00009E2E: 	DEC	AR5
    count = sec.count - 1;

deal_LP:
    // asm 00009E2F: 	LDI	AR6,AR2
    // asm 00009E30: 	CALL	PAL_DELETE
    PAL_DELETE(palette_code);
    // asm 00009E31: 	INC	AR6
    palette_code += 1;
    // asm 00009E32: 	DBU	AR5,deal_LP
    if (count-- > 0) {
        goto deal_LP;
    }
    // asm 00009E33: 	POP	AR6
    // asm 00009E34: 	POP	AR5
    // asm 00009E35: 	POP	AR4
    // asm 00009E36: 	RETS
}

// *----------------------------------------------------------------------------

/* asm: TACHOMETER_PAL	.bss	TACHOMETER_PAL,32 */
u32 TACHOMETER_PAL[32];

// *----------------------------------------------------------------------------
static void TACHOMETER_ANIMATE(void) {
    const u32* gradient;
    c3x_reg_t rpm;
    int lit_colors;
    int palette;
    int gear_palette_offset;

    // asm 00009E4D: 	LDL	TACH_GRADIENT,AR0
    gradient = (const u32*)ROM_PTR(TACH_GRADIENT_ROM);
    // asm 00009E4E: 	LDL	TACHOMETER_PAL,AR1
    // asm 00009E4F: 	LDI	22-1,RC
    // asm 00009E50: 	RPTB	LP89
    // asm 00009E51: 	LDI	*AR0++,R0
LP89:
    // asm 00009E52: STI	R0,*AR1++
    for (int i = 0; i < 22; i++) {
        TACHOMETER_PAL[i] = gradient[i];
    }
    // asm 00009E53: 	LDI	@PLYCBLK,AR0
    // asm 00009E54: 	LDF	*+AR0(CARRPM),R0
    rpm = C3X_LDF(PLYCBLK->rpm_x100);
    // asm 00009E55: 	MPYF	0.4583,R0		;22/48
    rpm = C3X_MUL(rpm, C3X_IMM_F32(0.4583)); // 22/48
    // ;	MPYF	0.729,R0		;35/48
    // ;	MPYF	0.6041,R0		;29/48
    // asm 00009E56: 	FIX	R0
    lit_colors = C3X_FIX(rpm);
    // asm 00009E57: 	CMPI	22,R0
    // asm 00009E58: 	LDIGT	22,R0
    if (lit_colors > 22) {
        lit_colors = 22;
    }
    // asm 00009E59: 	CMPI	0,R0
    // asm 00009E5A: 	LDILT	0,R0
    if (lit_colors < 0) {
        lit_colors = 0;
    }
    // asm 00009E5B: 	LDI	R0,RC
    // asm 00009E5C: 	SUBRI	22,RC
    // asm 00009E5D: 	LDL	TACHOMETER_PAL,AR1
    // asm 00009E5E: 	ADDI	R0,AR1
    // asm 00009E5F: 	CLRI	R1
    // asm 00009E60: 	RPTS	RC
    // asm 00009E61: 	STI	R1,*AR1++
    for (int i = lit_colors; i <= 22; i++) {
        TACHOMETER_PAL[i] = 0;
    }
    // asm 00009E62: 	LDI	rpm_p,AR2
    // asm 00009E63: 	CALL	PAL_FIND
    palette = PAL_FIND(rpm_p);
    // asm 00009E64: 	PUSH	R0
    // asm 00009E65: 	LDI	R0,R2
    // asm 00009E66: 	ADDI	256-22,R2
    // asm 00009E67: 	LDL	TACHOMETER_PAL,AR2
    // asm 00009E68: 	LDI	22,R3
    // asm 00009E69: 	CALL	PAL_SET
    PAL_SET(TACHOMETER_PAL, palette + (256 - 22), 22);
    // asm 00009E6A: 	LDI	@PLYCBLK,AR0
    // asm 00009E6B: 	LDI	*+AR0(CARGEAR),R0
    // asm 00009E6C: 	CLRI	R1		;ASSUME NEUTRAL
    gear_palette_offset = 0; // ASSUME NEUTRAL
    // asm 00009E6D: 	LDL	GEARPAL,AR2
    // asm 00009E6E: 	CMPI	1,R0
    // asm 00009E6F: 	LDIEQ	4,R1
    if (PLYCBLK->gear == 1) {
        gear_palette_offset = 4;
    }
    // asm 00009E70: 	CMPI	2,R0
    // asm 00009E71: 	LDIEQ	3,R1
    if (PLYCBLK->gear == 2) {
        gear_palette_offset = 3;
    }
    // asm 00009E72: 	CMPI	3,R0
    // asm 00009E73: 	LDIEQ	2,R1
    if (PLYCBLK->gear == 3) {
        gear_palette_offset = 2;
    }
    // asm 00009E74: 	CMPI	4,R0
    // asm 00009E75: 	LDIEQ	1,R1
    if (PLYCBLK->gear == 4) {
        gear_palette_offset = 1;
    }
    // asm 00009E76: 	POP	R2
    // asm 00009E77: 	ADDI	1,R2
    // asm 00009E78: 	ADDI	R1,AR2
    // asm 00009E79: 	LDI	4,R3
    // asm 00009E7A: 	CALL	PAL_SET
    PAL_SET((u32*)&GEARPAL[gear_palette_offset], palette + 1, 4);
    // asm 00009E7B: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TACHOMETER_ANIMATE", 0, 0);
}

/* asm: GEARPAL */
/* asm: .word	0 */
/* asm: .word	0 */
/* asm: .word	0 */
/* asm: .word	0 */
/* asm: RGB	0,255,255 */
/* asm: .word	0 */
/* asm: .word	0 */
/* asm: .word	0 */
static int GEARPAL[] = {
    0,
    0,
    0,
    0,
    RGB(0, 255, 255),
    0,
    0,
    0,
};
// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *PARAMETERS
 *	R0	X POS
 *	R1	Y POS
 *	R2	X WIDTH
 *	R3	Y WIDTH
 *
 *
 */
void FILL_DITHER(int x /*R0*/, int y /*R1*/, int width /*R2*/, int height /*R3*/) {
    // asm 00009E84: 	PUSH	R0
    // asm 00009E85: 	PUSH	R1
    // asm 00009E86: 	PUSH	R2
    // asm 00009E87: 	PUSH	R3
    // asm 00009E88: 	PUSH	R4
    // 	;background
    // asm 00009E89: 	LDI	CC|DITHER|8,R4
    // asm 00009E8A: 	BD	ENTER2
    // asm 00009E8B: 	STI	R4,@_ACNTL
    _ACNTL = CC | DITHER | 8;
    // asm 00009E8C: 	CLRI	R4
    // asm 00009E8D: 	STI	R4,@_ACMAP
    _ACMAP = 0;
    // 	;---->	BD	ENTER2
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    FILL_PLOT(x, y, width, height);
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FILL_DITHER", 0, 0);
}

void FILL_PLOT(int x /*R0*/, int y /*R1*/, int width /*R2*/, int height /*R3*/) {
    // asm 00009E8E: 	PUSH	R0
    // asm 00009E8F: 	PUSH	R1
    // asm 00009E90: 	PUSH	R2
    // asm 00009E91: 	PUSH	R3
    // asm 00009E92: 	PUSH	R4
    // ;	;background
    // ;	LDI	CC|DITHER|8,R4
    // ;	STI	R4,@_ACNTL
ENTER2:
    // asm 00009E93: 	STI	R0,@(_ARPS+(0*3))
    _ARPS[0] = x;
    // asm 00009E94: 	STI	R0,@(_ARPS+(3*3))
    _ARPS[9] = x;
    // asm 00009E95: 	ADDI	R2,R0
    x += width;
    // asm 00009E96: 	STI	R0,@(_ARPS+(1*3))
    _ARPS[3] = x;
    // asm 00009E97: 	STI	R0,@(_ARPS+(2*3))
    _ARPS[6] = x;
    // asm 00009E98: 	STI	R1,@(_ARPS+(0*3)+1)
    _ARPS[1] = y;
    // asm 00009E99: 	STI	R1,@(_ARPS+(1*3)+1)
    _ARPS[4] = y;
    // asm 00009E9A: 	ADDI	R3,R1,R0
    y += height;
    // asm 00009E9B: 	STI	R0,@(_ARPS+(3*3)+1)
    _ARPS[10] = y;
    // asm 00009E9C: 	STI	R0,@(_ARPS+(2*3)+1)
    _ARPS[7] = y;
    // asm 00009E9D: 	CLRI	R0
    // asm 00009E9E: 	STI	R0,@_ADDRL
    _ADDRL = 0;
    // asm 00009E9F: 	CALL	_stuff_fpga
    _stuff_fpga();
    // asm 00009EA0: 	POP	R4
    // asm 00009EA1: 	POP	R3
    // asm 00009EA2: 	POP	R2
    // asm 00009EA3: 	POP	R1
    // asm 00009EA4: 	POP	R0
    // asm 00009EA5: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FILL_PLOT", 0, 0);
}

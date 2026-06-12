#ifndef CMOS_H
#define CMOS_H

/* Generated from asm/CMOS.EQU. */

// CMOS.EQU
// 
// COPYRIGHT (C) 1994  BY TV GAMES, INC.
// ALL RIGHTS RESERVED
// 
// ----------------------------------------------------------------------------
// CMOS DEFINES
// 
// 8K X 8 CMOS RAM
// BITS ARE:	TTTT TTTT xxxx xxxx xxxx xxxx xxxx xxxx
// 
// ADJUSTMENTS ARE FIRST, THEN AUDITS
// SPACE FOR X AUDITS
// 
// THEN HSTD SPACE
// {
// RACE_TABLE
// } * NUM_RACES

// ----------------------------------------------------------------------------
// HSTD ENTRY INFORMATION
// 
// 
// asm: CMWS			.set	4		;CMOS WORD SIZE
#define CMWS 4 //CMOS WORD SIZE
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// AUDIT AND ADJUSTMENTS
// 
// 
// asm: ADJ_COINMODE		.set	0	;0-50,0
#define ADJ_COINMODE 0 //0-50,0
// asm: ADJ_GASMIN		.set	1	;0-255,0
#define ADJ_GASMIN 1 //0-255,0
// asm: ADJ_GASMAX		.set	2	;0-255,255
#define ADJ_GASMAX 2 //0-255,255
// asm: ADJ_STEERMIN		.set	3	;0-255,0
#define ADJ_STEERMIN 3 //0-255,0
// asm: ADJ_STEERMAX		.set	4	;0-255,255
#define ADJ_STEERMAX 4 //0-255,255
// asm: ADJ_STEERCENTER		.set	5	;0-255,128
#define ADJ_STEERCENTER 5 //0-255,128
// asm: ADJ_BRAKEMIN		.set	6	;0-255,0
#define ADJ_BRAKEMIN 6 //0-255,0
// asm: ADJ_BRAKEMAX		.set	7	;0-255,255
#define ADJ_BRAKEMAX 7 //0-255,255
// asm: ADJ_VOLUME		.set	8	;0-255,255
#define ADJ_VOLUME 8 //0-255,255

// asm: ADJ_COIN1_UNITS		.set	9	;0-100,1
#define ADJ_COIN1_UNITS 9 //0-100,1
// asm: ADJ_COIN2_UNITS		.set	10	;0-100,1
#define ADJ_COIN2_UNITS 10 //0-100,1
// asm: ADJ_COIN3_UNITS		.set	11	;0-100,1
#define ADJ_COIN3_UNITS 11 //0-100,1
// asm: ADJ_COIN4_UNITS		.set	12	;0-100,1
#define ADJ_COIN4_UNITS 12 //0-100,1
// asm: ADJ_UNITS_PER_CREDIT	.set	13	;0-100,1
#define ADJ_UNITS_PER_CREDIT 13 //0-100,1
// asm: ADJ_BONUS_UNITS		.set	14	;0-100,0
#define ADJ_BONUS_UNITS 14 //0-100,0
// asm: ADJ_UNITS_MIN		.set	15	;0-100,0
#define ADJ_UNITS_MIN 15 //0-100,0
// asm: ADJ_CREDITS_TO_START	.set	16	;0-100,3
#define ADJ_CREDITS_TO_START 16 //0-100,3
// asm: ADJ_CREDITS_TO_CONTINUE	.set	17	;0-100,3
#define ADJ_CREDITS_TO_CONTINUE 17 //0-100,3
// asm: ADJ_SHOW_FRAC		.set	18	;0-1,1
#define ADJ_SHOW_FRAC 18 //0-1,1

// asm: ADJ_COIN1_COUNTER	.set	19	;0-127,1
#define ADJ_COIN1_COUNTER 19 //0-127,1
// asm: ADJ_COIN2_COUNTER	.set	20	;0-127,1
#define ADJ_COIN2_COUNTER 20 //0-127,1
// asm: ADJ_COIN3_COUNTER	.set	21	;0-127,1
#define ADJ_COIN3_COUNTER 21 //0-127,1
// asm: ADJ_COIN4_COUNTER	.set	22	;0-127,4
#define ADJ_COIN4_COUNTER 22 //0-127,4

// asm: ADJ_STANDARD_PRICING	.set	23	;0-1,1
#define ADJ_STANDARD_PRICING 23 //0-1,1
// asm: ADJ_CUSTOM_PRICING	.set	24	;0-1,0
#define ADJ_CUSTOM_PRICING 24 //0-1,0
// asm: ADJ_FREE_PLAY		.set	25	;0-1,0
#define ADJ_FREE_PLAY 25 //0-1,0
// asm: ADJ_FREEGAME		.set	26	;0-1,1
#define ADJ_FREEGAME 26 //0-1,1
// asm: ADJ_DIFFICULTY		.set	27	;0-10,7
#define ADJ_DIFFICULTY 27 //0-10,7
// asm: ADJ_TIME_TO_START	.set	28	;0-6,6
#define ADJ_TIME_TO_START 28 //0-6,6
// asm: ADJ_CHECKPOINT_BONUS	.set	29	;0-10,7
#define ADJ_CHECKPOINT_BONUS 29 //0-10,7
// asm: ADJ_ATTRACT_MODE_SOUND	.set	30	;0-1,0
#define ADJ_ATTRACT_MODE_SOUND 30 //0-1,0
// asm: ADJ_HIGH_SCORE_ENTRY	.set	31	;0-1,1
#define ADJ_HIGH_SCORE_ENTRY 31 //0-1,1
// asm: ADJ_MIN_VOL_LEVEL	.set	32	;0-255,200
#define ADJ_MIN_VOL_LEVEL 32 //0-255,200
// asm: ADJ_ACTUALHSTDRESET	.set	33	;0-25000,5000
#define ADJ_ACTUALHSTDRESET 33 //0-25000,5000
// asm: ADJ_MPHORKPM		.set	34	;0-1,0
#define ADJ_MPHORKPM 34 //0-1,0
// asm: ADJ_ROADKILL		.set	35	;0-1,1
#define ADJ_ROADKILL 35 //0-1,1
// asm: ADJ_CLINTON		.set	36	;0-1,1
#define ADJ_CLINTON 36 //0-1,1
// asm: ADJ_GIRLS		.set	37	;0-1,1
#define ADJ_GIRLS 37 //0-1,1
// asm: ADJ_STEERING_SENSITIVITY	.set	38	;0-40,10
#define ADJ_STEERING_SENSITIVITY 38 //0-40,10
// asm: ADJ_HIGHSCORE_RESET	.set	39	;1000,25000,5000
#define ADJ_HIGHSCORE_RESET 39 //1000,25000,5000
// asm: ADJ_DIFF_LOCAL		.set	40	;0-100,50
#define ADJ_DIFF_LOCAL 40 //0-100,50
// asm: ADJ_OUTOFDIAG		.set	41	;have we just left diag???
#define ADJ_OUTOFDIAG 41 //have we just left diag???
// asm: ADJ_RAMP_PERCENTAGE	.set	42	;0-30,20
#define ADJ_RAMP_PERCENTAGE 42 //0-30,20
// asm: ADJ_RAMP_COUNT		.set	43	;0-5000,0
#define ADJ_RAMP_COUNT 43 //0-5000,0
// asm: ADJ_ENTER_INITS		.set	44	;0-1,1
#define ADJ_ENTER_INITS 44 //0-1,1
// asm: ADJ_MAX_CREDITS		.set	45	;10-50,30
#define ADJ_MAX_CREDITS 45 //10-50,30

// asm: NUM_ADJUSTMENTS		.set	46
#define NUM_ADJUSTMENTS 46

// asm: ADJ_WDOG		.set	50	;temp space
#define ADJ_WDOG 50 //temp space
// asm: ADJ_INITIALS		.set	51	;-1 if reset x|b|b|b
#define ADJ_INITIALS 51 //-1 if reset x|b|b|b
// asm: ADJ_MOTION_PRESENT	.set	52
#define ADJ_MOTION_PRESENT 52
// asm: ADJ_CHECKSUM		.set	53
#define ADJ_CHECKSUM 53

// asm: MAX_ADJUSTMENTS		.set	54
#define MAX_ADJUSTMENTS 54

// COIN BOOKEEPING
// asm: AUD_COIN1		.set	100	;*
#define AUD_COIN1 100 //*
// asm: AUD_COIN2		.set	101	;*
#define AUD_COIN2 101 //*
// asm: AUD_COIN3		.set	102	;*
#define AUD_COIN3 102 //*
// asm: AUD_COIN4		.set	103	;*
#define AUD_COIN4 103 //*

// asm: AUD_SERVICE_CREDITS	.set	104	;*
#define AUD_SERVICE_CREDITS 104 //*
// asm: AUD_PAID_CREDITS	.set	105	;*
#define AUD_PAID_CREDITS 105 //*
// asm: AUD_TOTAL_FREEGAMES	.set	106	;
#define AUD_TOTAL_FREEGAMES 106

// asm: AUD_FREE_TO_TOTAL	.set	107	; % FREE GAMES TO TOTAL PLAYS
#define AUD_FREE_TO_TOTAL 107 //% FREE GAMES TO TOTAL PLAYS
// asm: AUD_TOTAL_PLAYS		.set	108
#define AUD_TOTAL_PLAYS 108
// asm: AUD_TOTAL_STARTS	.set	109
#define AUD_TOTAL_STARTS 109
// asm: AUD_TOTAL_CONTINUES	.set	110
#define AUD_TOTAL_CONTINUES 110

// asm: AUD_GAMENUMBER		.set	111	;
#define AUD_GAMENUMBER 111
// asm: AUD_PCREDITS		.set	112	;partial credits
#define AUD_PCREDITS 112 //partial credits
// asm: AUD_CREDITS		.set	113	;actual credits
#define AUD_CREDITS 113 //actual credits
// asm: AUD_NUM_BUYINS		.set	114	;
#define AUD_NUM_BUYINS 114
// asm: AUD_TOTAL_TIME		.set	115	;AUD_TOTAL_TIME/AUD_NUM_BUYINS / 100
#define AUD_TOTAL_TIME 115 //AUD_TOTAL_TIME/AUD_NUM_BUYINS / 100
// asm: AUD_AVG_TIME		.set	116	;
#define AUD_AVG_TIME 116
// asm: AUD_NUM_UNFINISHED	.set	117	;
#define AUD_NUM_UNFINISHED 117

// asm: AUD_POWERON_TIME	.set	118	;Measured in seconds...
#define AUD_POWERON_TIME 118 //Measured in seconds...
// asm: AUD_GAMEON_TIME		.set	119	;Measured in seconds...
#define AUD_GAMEON_TIME 119 //Measured in seconds...
// asm: AUD_NUM_WATCHDOGS	.set	120
#define AUD_NUM_WATCHDOGS 120

// asm: AUD_TOTAL_UNLINKED_PLAYS	.set	121
#define AUD_TOTAL_UNLINKED_PLAYS 121
// asm: AUD_TOTAL_UNLINKED_1STS		.set	122
#define AUD_TOTAL_UNLINKED_1STS 122
// asm: AUD_UNLINKED_1ST_TO_TOTAL_UL	.set	123
#define AUD_UNLINKED_1ST_TO_TOTAL_UL 123
// asm: AUD_TOTAL_UNLINKED_STARTS	.set	124
#define AUD_TOTAL_UNLINKED_STARTS 124
// asm: AUD_TOTAL_UNLINKED_CONTINUES	.set	125
#define AUD_TOTAL_UNLINKED_CONTINUES 125
// asm: AUD_CONTINUES_TO_PLAYS_UNLINKED	.set	126
#define AUD_CONTINUES_TO_PLAYS_UNLINKED 126
// asm: AUD_TOTAL_UNLINKED_GAME_ENDS	.set	127
#define AUD_TOTAL_UNLINKED_GAME_ENDS 127

// asm: AUD_RESET_TOTALLY		.set	128
#define AUD_RESET_TOTALLY 128
// AUD_TOTAL_LINKED_PLAYS		.set	74
// AUD_TOTAL_LINKED_1STS		.set	75
// asm: AUD_LINKED_1ST_TO_TOTAL_UL	.set	129
#define AUD_LINKED_1ST_TO_TOTAL_UL 129
// asm: AUD_TOTAL_LINKED_STARTS		.set	130
#define AUD_TOTAL_LINKED_STARTS 130
// asm: AUD_TOTAL_LINKED_CONTINUES	.set	131
#define AUD_TOTAL_LINKED_CONTINUES 131
// asm: AUD_CONTINUES_TO_PLAYS_LINKED	.set	132
#define AUD_CONTINUES_TO_PLAYS_LINKED 132
// asm: AUD_TOTAL_LINKED_GAME_ENDS	.set	133
#define AUD_TOTAL_LINKED_GAME_ENDS 133

// asm: AUD_LINKED_PLAYS_TO_TOTAL_PLAYS	.set	134
#define AUD_LINKED_PLAYS_TO_TOTAL_PLAYS 134

// asm: AUD_TOTAL_ONTIME		.set	135
#define AUD_TOTAL_ONTIME 135
// asm: AUD_TOTAL_PLAYTIME		.set	136
#define AUD_TOTAL_PLAYTIME 136
// asm: AUD_TOTAL_LINKED_PLAYTIME	.set	137
#define AUD_TOTAL_LINKED_PLAYTIME 137
// asm: AUD_TOTAL_UNLINKED_PLAYTIME	.set	138
#define AUD_TOTAL_UNLINKED_PLAYTIME 138

// asm: AUD_AVG_TIME_PER_CREDIT		.set	139
#define AUD_AVG_TIME_PER_CREDIT 139
// asm: AUD_AVGCREDITS_PER_GAME		.set	140
#define AUD_AVGCREDITS_PER_GAME 140
// asm: AUD_COIN_DOOR_SLAMS		.set	141
#define AUD_COIN_DOOR_SLAMS 141
// asm: AUD_SUCCESSFULL_BURNINS		.set	142
#define AUD_SUCCESSFULL_BURNINS 142
// asm: AUD_UNFINISHED_GAMES		.set	143
#define AUD_UNFINISHED_GAMES 143
// asm: AUD_UNFINISHED_GAMES_FOUND	.set	144
#define AUD_UNFINISHED_GAMES_FOUND 144

// asm: AUD_VIEW1_TIME			.set	145
#define AUD_VIEW1_TIME 145
// asm: AUD_VIEW2_TIME			.set	146
#define AUD_VIEW2_TIME 146
// asm: AUD_VIEW3_TIME			.set	147
#define AUD_VIEW3_TIME 147

// asm: AUD_RADIO_HITS			.set	148	;*
#define AUD_RADIO_HITS 148 //*
// asm: AUD_AUTOMATIC_TRANS_SELECTED	.set	149	;*
#define AUD_AUTOMATIC_TRANS_SELECTED 149 //*
// asm: AUD_MANUAL_TRANS_SELECTED	.set	150	;*
#define AUD_MANUAL_TRANS_SELECTED 150 //*

// asm: AUD_GAMES_EXPIRED		.set	151
#define AUD_GAMES_EXPIRED 151

// asm: AUD_GENERAL_CHKSUM		.set	152
#define AUD_GENERAL_CHKSUM 152

// asm: AUD_START_GGATE			.set	153
#define AUD_START_GGATE 153
// asm: AUD_FINISH_GGATE		.set	154
#define AUD_FINISH_GGATE 154
// asm: AUD_START_SF			.set	155
#define AUD_START_SF 155
// asm: AUD_FINISH_SF			.set	156
#define AUD_FINISH_SF 156
// asm: AUD_START_US101			.set	157
#define AUD_START_US101 157
// asm: AUD_FINISH_US101		.set	158
#define AUD_FINISH_US101 158
// asm: AUD_START_REDWOOD		.set	159
#define AUD_START_REDWOOD 159
// asm: AUD_FINISH_REDWOOD		.set	160
#define AUD_FINISH_REDWOOD 160
// asm: AUD_START_BEVHILLS		.set	161
#define AUD_START_BEVHILLS 161
// asm: AUD_FINISH_BEVHILLS		.set	162
#define AUD_FINISH_BEVHILLS 162
// asm: AUD_START_LA			.set	163
#define AUD_START_LA 163
// asm: AUD_FINISH_LA			.set	164
#define AUD_FINISH_LA 164
// asm: AUD_START_DV			.set	165
#define AUD_START_DV 165
// asm: AUD_FINISH_DV			.set	166
#define AUD_FINISH_DV 166
// asm: AUD_START_ARIZONA		.set	167
#define AUD_START_ARIZONA 167
// asm: AUD_FINISH_ARIZONA		.set	168
#define AUD_FINISH_ARIZONA 168
// asm: AUD_START_GC			.set	169
#define AUD_START_GC 169
// asm: AUD_FINISH_GC			.set	170
#define AUD_FINISH_GC 170
// asm: AUD_START_IOWA			.set	171
#define AUD_START_IOWA 171
// asm: AUD_FINISH_IOWA			.set	172
#define AUD_FINISH_IOWA 172
// asm: AUD_START_CHICAGO		.set	173
#define AUD_START_CHICAGO 173
// asm: AUD_FINISH_CHICAGO		.set	174
#define AUD_FINISH_CHICAGO 174
// asm: AUD_START_INDIANA		.set	175
#define AUD_START_INDIANA 175
// asm: AUD_FINISH_INDIANA		.set	176
#define AUD_FINISH_INDIANA 176
// asm: AUD_START_APPALACHIA		.set	177
#define AUD_START_APPALACHIA 177
// asm: AUD_FINISH_APPALACHIA		.set	178
#define AUD_FINISH_APPALACHIA 178
// asm: AUD_START_DC			.set	179
#define AUD_START_DC 179
// asm: AUD_FINISH_GAME			.set	180
#define AUD_FINISH_GAME 180

// these are actually accumulators
// to get the actual value thou must
// / AUD_FINISH_<NAME>
// 
// asm: AUD_WIN_GGATE			.set	181
#define AUD_WIN_GGATE 181
// asm: AUD_WIN_SF			.set	182
#define AUD_WIN_SF 182
// asm: AUD_WIN_US101			.set	183
#define AUD_WIN_US101 183
// asm: AUD_WIN_REDWOOD			.set	184
#define AUD_WIN_REDWOOD 184
// asm: AUD_WIN_BEVHILLS		.set	185
#define AUD_WIN_BEVHILLS 185
// asm: AUD_WIN_LA			.set	186
#define AUD_WIN_LA 186
// asm: AUD_WIN_DV			.set	187
#define AUD_WIN_DV 187
// asm: AUD_WIN_ARIZONA			.set	188
#define AUD_WIN_ARIZONA 188
// asm: AUD_WIN_GC			.set	189
#define AUD_WIN_GC 189
// asm: AUD_WIN_IOWA			.set	190
#define AUD_WIN_IOWA 190
// asm: AUD_WIN_CHICAGO			.set	191
#define AUD_WIN_CHICAGO 191
// asm: AUD_WIN_INDIANA			.set	192
#define AUD_WIN_INDIANA 192
// asm: AUD_WIN_APPALACHIA		.set	193
#define AUD_WIN_APPALACHIA 193
// asm: AUD_WIN_DC			.set	194
#define AUD_WIN_DC 194
// asm: AUD_WIN_GAME			.set	195
#define AUD_WIN_GAME 195

// asm: AUD_GAMES_START			.set	210
#define AUD_GAMES_START 210
// asm: AUD_GAMES_CONTINUES		.set	211
#define AUD_GAMES_CONTINUES 211
// asm: AUD_GAMES_ENDS			.set	212
#define AUD_GAMES_ENDS 212
// asm: AUD_BURNINS			.set	213
#define AUD_BURNINS 213
// asm: AUD_VERTFRAME			.set	214
#define AUD_VERTFRAME 214
// asm: AUD_POLYFRAME			.set	215
#define AUD_POLYFRAME 215
// asm: AUD_OBJFRAME			.set	216
#define AUD_OBJFRAME 216
// asm: AUD_FRAMEFRAME			.set	217
#define AUD_FRAMEFRAME 217

// asm: AUD_REPEAT_REDWOOD		.set	218
#define AUD_REPEAT_REDWOOD 218
// asm: AUD_REPEAT_DEATHVALLEY		.set	219
#define AUD_REPEAT_DEATHVALLEY 219
// asm: AUD_REPEAT_GRANDCANYON		.set	220
#define AUD_REPEAT_GRANDCANYON 220
// asm: AUD_REPEAT_CHICAGO		.set	221
#define AUD_REPEAT_CHICAGO 221

// Misc internal counts
// asm: AUD_LOG_COUNT			.set	222
#define AUD_LOG_COUNT 222
// asm: AUD_LAST_LEG			.set	223		;LAST LEG SETUP
#define AUD_LAST_LEG 223 //LAST LEG SETUP
// asm: AUD_PLAYS_TO_HISCORE_RESET	.set	224
#define AUD_PLAYS_TO_HISCORE_RESET 224

// asm: AUD_VETTE_SELECTED		.set	225
#define AUD_VETTE_SELECTED 225
// asm: AUD_CAR_SELECTION		.set	AUD_VETTE_SELECTED
#define AUD_CAR_SELECTION AUD_VETTE_SELECTED
// asm: AUD_LABOMBA_SELECTED		.set	226
#define AUD_LABOMBA_SELECTED 226
// asm: AUD_DEVASTATOR_SELECTED		.set	227
#define AUD_DEVASTATOR_SELECTED 227
// asm: AUD_ITALIA_SELECTED		.set	228
#define AUD_ITALIA_SELECTED 228
// asm: AUD_HID_JEEP_SELECTED		.set	229
#define AUD_HID_JEEP_SELECTED 229
// asm: AUD_HID_SBUS_SELECTED		.set	230
#define AUD_HID_SBUS_SELECTED 230
// asm: AUD_HID_COPCAR_SELECTED		.set	231
#define AUD_HID_COPCAR_SELECTED 231
// asm: AUD_HID_DIESEL_SELECTED		.set	232
#define AUD_HID_DIESEL_SELECTED 232

// asm: AUD_FS_AZ			.set	233	;free select ARIZONA
#define AUD_FS_AZ 233 //free select ARIZONA
// asm: AUD_FS_US101			.set	234
#define AUD_FS_US101 234
// asm: AUD_FS_LA			.set	235
#define AUD_FS_LA 235
// asm: AUD_FS_IOWA			.set	236
#define AUD_FS_IOWA 236
// asm: AUD_FS_DV			.set	237
#define AUD_FS_DV 237
// asm: AUD_FS_USA			.set	238
#define AUD_FS_USA 238
// asm: AUD_FS_BH			.set	239
#define AUD_FS_BH 239
// asm: AUD_FS_APPL			.set	240
#define AUD_FS_APPL 240
// asm: AUD_FS_RW			.set	241
#define AUD_FS_RW 241
// asm: AUD_FS_RM			.set	242
#define AUD_FS_RM 242
// asm: AUD_FS_CH			.set	243
#define AUD_FS_CH 243
// asm: AUD_FS_HID_SF			.set	244
#define AUD_FS_HID_SF 244
// asm: AUD_FS_HID_IN			.set	245
#define AUD_FS_HID_IN 245
// asm: AUD_FS_HID_GG			.set	246
#define AUD_FS_HID_GG 246

// asm: AUD_BCREDITS			.set	247
#define AUD_BCREDITS 247
// asm: AUD_H2HGAMES			.set	248
#define AUD_H2HGAMES 248
// asm: NUM_AUDITS			.set	249
#define NUM_AUDITS 249

// asm: AUD_VERSION			.set	250
#define AUD_VERSION 250
// asm: AUD_HIGHSCORE_CHECKSUM		.set	251
#define AUD_HIGHSCORE_CHECKSUM 251
// asm: AUD_CHIPTEST_DEDICATED		.set	252
#define AUD_CHIPTEST_DEDICATED 252
// asm: AUD_CHECKSUM			.set	253
#define AUD_CHECKSUM 253
// asm: MAX_AUDITS			.set	254
#define MAX_AUDITS 254
// ----------------------------------------------------------------------------

// EDIAG.ASM

// CMOS.ASM

// DIAG.ASM

// HSTD.ASM

// ----------------------------------------------------------------------------
// CMOS STRUCTURES
// 
// 
// TOP RACE TIMES
// 
// STRUCT RACEENTRY
// asm: TE_TIME		.set	0	;TIME
#define TE_TIME 0 //TIME
// asm: TE_INIT1	.set	4	; }
#define TE_INIT1 4 //}
// asm: TE_INIT2	.set	5	;  } PLAYERS INITIALS
#define TE_INIT2 5 //} PLAYERS INITIALS
// asm: TE_INIT3	.set	6	; }
#define TE_INIT3 6 //}
// asm: TE_RANK		.set	7	;RANK
#define TE_RANK 7 //RANK
// ENDSTRUCT
// asm: TE_SIZE		.set	8
#define TE_SIZE 8

// asm: NUM_RACES		.set	15
#define NUM_RACES 15
// asm: NUM_ENTRIES_PER_RACE	.set	10
#define NUM_ENTRIES_PER_RACE 10
// asm: RACE_TABLE_SIZE		.set	NUM_ENTRIES_PER_RACE*TE_SIZE
#define RACE_TABLE_SIZE (NUM_ENTRIES_PER_RACE*TE_SIZE)

// 
// 
// 
// asm: CMOS_PAGE	.set	0800h			;page size (2048b in length, 512 words)
#define CMOS_PAGE 0x0800 //page size (2048b in length, 512 words)
// asm: HSTD_PAGE	.set	CMOS+(MAX_AUDITS*4)
#define HSTD_PAGE (CMOS+(MAX_AUDITS*4))
// asm: H280_RTABLE	.set	HSTD_PAGE
#define H280_RTABLE HSTD_PAGE
// asm: SF_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE)
#define SF_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE))
// asm: RW_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE*1)
#define RW_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE*1))
// asm: BEVLA_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE*2)
#define BEVLA_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE*2))
// asm: DVAZ_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE*3)
#define DVAZ_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE*3))
// asm: GC_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE*4)
#define GC_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE*4))
// asm: IO_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE*5)
#define IO_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE*5))
// asm: CH_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE*6)
#define CH_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE*6))
// asm: AP_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE*7)
#define AP_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE*7))
// asm: DC_RTABLE	.set	HSTD_PAGE+(RACE_TABLE_SIZE*8)
#define DC_RTABLE (HSTD_PAGE+(RACE_TABLE_SIZE*8))
// ----------------------------------------------------------------------------

#endif /* CMOS_H */

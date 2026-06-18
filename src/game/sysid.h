#ifndef SYSID_H
#define SYSID_H

#include "port.h"

/* Generated from asm/SYSID.EQU. */

// SYSID.EQU
// 
// COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// ALL RIGHTS RESERVED
// 
// ----------------------------------------------------------------------------
// SYSTEM IDS
// 
// THIS LIST IS FOR ALL OBJECTS AND PROCESSES
// THIS IS THE *ONLY* PLACE VALID IDS ARE DEFINED

// asm: CLASS_M		.set	00F00h		;BIT MASKS FOR SYSTEM
#define CLASS_M 0x00F00 //BIT MASKS FOR SYSTEM

// asm: TYPE_M		.set	000F0h		;
#define TYPE_M 0x000F0

// asm: SUBTYPE_M	.set	0000Fh		;
#define SUBTYPE_M 0x0000F

// asm: PLYR_C		.set	00100h		;into -> CAR_SUPP
#define PLYR_C 0x00100 //into -> CAR_SUPP

// asm: PLYR1_T		.set	00010h		;the players car and process
#define PLYR1_T 0x00010 //the players car and process

// asm: PLYR_FLAMES_S	.set	00001h		;This is used to sort the flames
#define PLYR_FLAMES_S 0x00001 //This is used to sort the flames

// asm: PLYR_SMOKE_S	.set	00002h		;This is used to sort the smoke
#define PLYR_SMOKE_S 0x00002 //This is used to sort the smoke

// asm: PLYR_SPLAT_S	.set	00003h		;This is the GOOSE SHIT
#define PLYR_SPLAT_S 0x00003 //This is the GOOSE SHIT

// asm: DRONE_C		.set	00200h
#define DRONE_C 0x00200

// asm: VEHICLE_T	.set	00010h
#define VEHICLE_T 0x00010

// asm: DEAD_VEH_T	.set	00020h		;DEAD (DRONE) VEHICLE
#define DEAD_VEH_T 0x00020 //DEAD (DRONE) VEHICLE

// asm: DELTAORIG_S	.set	00001h
#define DELTAORIG_S 0x00001

// asm: DELTA_ATTR_S	.set	00002h
#define DELTA_ATTR_S 0x00002

// asm: DRNE_RHO	.set	00003h
#define DRNE_RHO 0x00003

// asm: DRNE_SIGMA	.set	00004h
#define DRNE_SIGMA 0x00004

// asm: DRNE_COPCAR	.set	00005h
#define DRNE_COPCAR 0x00005

// asm: DRNE_RACER	.set	00006h
#define DRNE_RACER 0x00006

// asm: ANI_T		.set	00030h		;ANIMATION PROCESS
#define ANI_T 0x00030 //ANIMATION PROCESS

// asm: DRNE_CHOPPER_T	.set	00050h
#define DRNE_CHOPPER_T 0x00050

// asm: RAILROAD	.set	00060h
#define RAILROAD 0x00060

// asm: HELICOPTER	.set	00070h
#define HELICOPTER 0x00070

// RACER_T		.set	00080h
// asm: FLYER_T		.set	00090h
#define FLYER_T 0x00090

// asm: ROAD_C		.set	00300h		;into -> DRIVE_SUPP  (things to be driven over)
#define ROAD_C 0x00300 //into -> DRIVE_SUPP  (things to be driven over)

// asm: SHLDR_T		.set	00010h		;general shoulder classification
#define SHLDR_T 0x00010 //general shoulder classification

// asm: REPEL_SHLDR_T	.set	00020h		;repeling shoulder classification
#define REPEL_SHLDR_T 0x00020 //repeling shoulder classification

// asm: LOGRAV_T	.set	00030h		;low gravity piece
#define LOGRAV_T 0x00030 //low gravity piece

// asm: UTIL_C		.set	00500h		;these are unclassified and uncollidable
#define UTIL_C 0x00500 //these are unclassified and uncollidable

// asm: BACKGRND_T	.set	000AAh		;the background multi-processor
#define BACKGRND_T 0x000AA //the background multi-processor

// asm: COLLSCAN_T	.set	000BBh		;the collision scan process
#define COLLSCAN_T 0x000BB //the collision scan process

// asm: MONKEY_T	.set	000C0h		;monkeys and WAVEFLAG
#define MONKEY_T 0x000C0 //monkeys and WAVEFLAG

// asm: CHOOSECAR_T	.set	000C1h		;Choose Car in intro
#define CHOOSECAR_T 0x000C1 //Choose Car in intro

// asm: CHOOSETRANS_T	.set	000C2h		;Choose Transmission in intro
#define CHOOSETRANS_T 0x000C2 //Choose Transmission in intro

// asm: BONUS_SCREEN_T	.set	000C3h		;Bonus RackUp
#define BONUS_SCREEN_T 0x000C3 //Bonus RackUp

// asm: TEXTP_T		.set	000C4h		;Text Routines
#define TEXTP_T 0x000C4 //Text Routines

// asm: CHOOSERACE_T	.set	000C5h		;Choose Race Screen
#define CHOOSERACE_T 0x000C5 //Choose Race Screen

// asm: RADIOPROC_T	.set	000C6h		;Radio Button Proc
#define RADIOPROC_T 0x000C6 //Radio Button Proc

// like text, smoke, explosions, skid marks, etc.
// asm: SKID_T		.set	00010h
#define SKID_T 0x00010

// asm: SMOKE_T		.set	00020h
#define SMOKE_T 0x00020

// asm: TEXT_T		.set	00030h		;also logos and other stuff
#define TEXT_T 0x00030 //also logos and other stuff

// asm: DIAL_ST		.set	00001h
#define DIAL_ST 0x00001

// asm: BORDER_ST	.set	00002h
#define BORDER_ST 0x00002

// asm: SPARK_T		.set	00040h
#define SPARK_T 0x00040

// asm: LOGO_T		.set	00050h		;the logo
#define LOGO_T 0x00050 //the logo

// asm: DISPLAYHS_T	.set	00060h		;PROC THAT DISPLAYS THE HIGH SCORE
#define DISPLAYHS_T 0x00060 //PROC THAT DISPLAYS THE HIGH SCORE

// asm: FLASH_ST	.set	00001h		;SUBTYPE OF TYPE DISPLAYHS_T THAT FLASHES THE INITIALS
#define FLASH_ST 0x00001 //SUBTYPE OF TYPE DISPLAYHS_T THAT FLASHES THE INITIALS

// asm: TSIGN_C		.set	00600h		;TREEs & SIGNs (STUFF w/o thickness on SUPP LIST)
#define TSIGN_C 0x00600 //TREEs & SIGNs (STUFF w/o thickness on SUPP LIST)

// into -> TSIGN_SUPP
// asm: TSC_IMMOBILE	.set	00010h		;crash into
#define TSC_IMMOBILE 0x00010 //crash into

// asm: TSC_I_VIBRATE	.set	00001h		;
#define TSC_I_VIBRATE 0x00001

// asm: TSC_RUNOVER	.set	00020h		;car may run over
#define TSC_RUNOVER 0x00020 //car may run over

// asm: TSC_R_SAGE	.set	00001h		;	sage brush
#define TSC_R_SAGE 0x00001 //sage brush

// asm: TSC_R_POLE	.set	00002h		;	pole sign
#define TSC_R_POLE 0x00002 //pole sign

// asm: TSC_R_LAMPPOST	.set	00003h		;	light pole
#define TSC_R_LAMPPOST 0x00003 //light pole

// asm: TSC_R_TREE	.set	00004h		;	Tree
#define TSC_R_TREE 0x00004 //Tree

// asm: TSC_R_PHONEPOST	.set	00005h		;	Phone pole
#define TSC_R_PHONEPOST 0x00005 //Phone pole

// asm: TSC_FLYING	.set	00030h		;hit & goes flying
#define TSC_FLYING 0x00030 //hit & goes flying

// asm: TSC_V_PALM	.set	00005h		;PALM = TSIGN_C|TSC_IMMOBILE|TSC_V_PALM
#define TSC_V_PALM 0x00005 //PALM = TSIGN_C|TSC_IMMOBILE|TSC_V_PALM

// asm: TSC_HARD	.set	00040h		;MAY *NOT* GET KNOCKED DOWN (NO TOPPLE)
#define TSC_HARD 0x00040 //MAY *NOT* GET KNOCKED DOWN (NO TOPPLE)

// asm: TSC_ROADKILL	.set	00050h		;ROAD KILL
#define TSC_ROADKILL 0x00050 //ROAD KILL

// asm: TSC_COW_S	.set	00001h		;MOOOOO!
#define TSC_COW_S 0x00001 //MOOOOO!

// asm: TSC_DEER_S	.set	00002h		;a do a deer a female deer... ray a droplet full of sun
#define TSC_DEER_S 0x00002 //a do a deer a female deer... ray a droplet full of sun

// asm: TSC_PARTS_S	.set	00003h		;FLYING CHUNKS OF MEAT THAT CAN BE COLLIED WITH
#define TSC_PARTS_S 0x00003 //FLYING CHUNKS OF MEAT THAT CAN BE COLLIED WITH

// asm: TSC_IGNORE	.set	00060h		;SORT, BUT DO NOTHING...
#define TSC_IGNORE 0x00060 //SORT, BUT DO NOTHING...

// asm: TSC_SPARK_S    	.set	00006h		;Subcalss of TSC_IGNORE
#define TSC_SPARK_S 0x00006 //Subcalss of TSC_IGNORE

// asm: TSC_DUDE_S	.set	00007h		;SAL THE STUD
#define TSC_DUDE_S 0x00007 //SAL THE STUD

// asm: TSC_BABE_S	.set	00008h		;THE BABE AT THE END OF THE RACE
#define TSC_BABE_S 0x00008 //THE BABE AT THE END OF THE RACE

// asm: TSC_GGBRG_RAIL	.set	60h		;golden gate bridge railing
#define TSC_GGBRG_RAIL 0x60 //golden gate bridge railing

// asm: RDDEBRIS_C	.set	00700h		;ROAD DEBRIS (construction horses, 55gal drums, mail boxes)
#define RDDEBRIS_C 0x00700 //ROAD DEBRIS (construction horses, 55gal drums, mail boxes)

// asm: RDD_55GAL	.set	00001h		;55Gallon Drum
#define RDD_55GAL 0x00001 //55Gallon Drum

// asm: ROAD_SUBLIST_SS	.set	01000h		;ROAD SUBLIST SUPER SET IDENTIFIER
#define ROAD_SUBLIST_SS 0x01000 //ROAD SUBLIST SUPER SET IDENTIFIER

// asm: SPAWNER_C	.set	00800h		;PROCESS WHICH SPAWNS OTHERS
#define SPAWNER_C 0x00800 //PROCESS WHICH SPAWNS OTHERS

// asm: SPWN_SWITCH_T	.set	00010h		;SWITCH SCAN PROCESS
#define SPWN_SWITCH_T 0x00010 //SWITCH SCAN PROCESS

// asm: LOAD_REQ_T	.set	00020h		;SYSTEM LOAD REQUEST
#define LOAD_REQ_T 0x00020 //SYSTEM LOAD REQUEST

// asm: TRAFFIC_T	.set	00030h		;TRAFFIC DISPATCHER
#define TRAFFIC_T 0x00030 //TRAFFIC DISPATCHER

// asm: COLORCYC_T	.set	00040h		;COLOR CYCLER PROCESS
#define COLORCYC_T 0x00040 //COLOR CYCLER PROCESS

// asm: ANIMATION_T	.set	00050h		;ANIMATION PROCESS
#define ANIMATION_T 0x00050 //ANIMATION PROCESS

// asm: GROUND_C	.set	00900h		;GROUND (not ROAD or SHOULDER or BUILDING but GROUND)
#define GROUND_C 0x00900 //GROUND (not ROAD or SHOULDER or BUILDING but GROUND)

#endif /* SYSID_H */

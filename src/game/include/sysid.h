#ifndef SYSID_H
#define SYSID_H

/* Generated from asm/SYSID.EQU. */
/*
 * SYSID.EQU
 * COPYRIGHT (C) 1994 BY  TV GAMES, INC.
 * ALL RIGHTS RESERVED
 * ----------------------------------------------------------------------------
 * SYSTEM IDS
 * THIS LIST IS FOR ALL OBJECTS AND PROCESSES
 * THIS IS THE *ONLY* PLACE VALID IDS ARE DEFINED
 * RACER_T		.set	00080h
 * ...
 */

#define CLASS_M 0x00F00 /* BIT MASKS FOR SYSTEM */
#define TYPE_M 0x000F0
#define SUBTYPE_M 0x0000F
#define PLYR_C 0x00100 /* into -> CAR_SUPP */
#define PLYR1_T 0x00010 /* the players car and process */
#define PLYR_FLAMES_S 0x00001 /* This is used to sort the flames */
#define PLYR_SMOKE_S 0x00002 /* This is used to sort the smoke */
#define PLYR_SPLAT_S 0x00003 /* This is the GOOSE SHIT */
#define DRONE_C 0x00200
#define VEHICLE_T 0x00010
#define DEAD_VEH_T 0x00020 /* DEAD (DRONE) VEHICLE */
#define DELTAORIG_S 0x00001
#define DELTA_ATTR_S 0x00002
#define DRNE_RHO 0x00003
#define DRNE_SIGMA 0x00004
#define DRNE_COPCAR 0x00005
#define DRNE_RACER 0x00006
#define ANI_T 0x00030 /* ANIMATION PROCESS */
#define DRNE_CHOPPER_T 0x00050
#define RAILROAD 0x00060
#define HELICOPTER 0x00070
#define FLYER_T 0x00090
#define ROAD_C 0x00300 /* into -> DRIVE_SUPP  (things to be driven over) */
#define SHLDR_T 0x00010 /* general shoulder classification */
#define REPEL_SHLDR_T 0x00020 /* repeling shoulder classification */
#define LOGRAV_T 0x00030 /* low gravity piece */
#define UTIL_C 0x00500 /* these are unclassified and uncollidable */
#define BACKGRND_T 0x000AA /* the background multi-processor */
#define COLLSCAN_T 0x000BB /* the collision scan process */
#define MONKEY_T 0x000C0 /* monkeys and WAVEFLAG */
#define CHOOSECAR_T 0x000C1 /* Choose Car in intro */
#define CHOOSETRANS_T 0x000C2 /* Choose Transmission in intro */
#define BONUS_SCREEN_T 0x000C3 /* Bonus RackUp */
#define TEXTP_T 0x000C4 /* Text Routines */
#define CHOOSERACE_T 0x000C5 /* Choose Race Screen */
#define RADIOPROC_T 0x000C6 /* Radio Button Proc */
#define SKID_T 0x00010
#define SMOKE_T 0x00020
#define TEXT_T 0x00030 /* also logos and other stuff */
#define DIAL_ST 0x00001
#define BORDER_ST 0x00002
#define SPARK_T 0x00040
#define LOGO_T 0x00050 /* the logo */
#define DISPLAYHS_T 0x00060 /* PROC THAT DISPLAYS THE HIGH SCORE */
#define FLASH_ST 0x00001 /* SUBTYPE OF TYPE DISPLAYHS_T THAT FLASHES THE INITIALS */
#define TSIGN_C 0x00600 /* TREEs & SIGNs (STUFF w/o thickness on SUPP LIST) */
#define TSC_IMMOBILE 0x00010 /* crash into */
#define TSC_I_VIBRATE 0x00001
#define TSC_RUNOVER 0x00020 /* car may run over */
#define TSC_R_SAGE 0x00001 /* sage brush */
#define TSC_R_POLE 0x00002 /* pole sign */
#define TSC_R_LAMPPOST 0x00003 /* light pole */
#define TSC_R_TREE 0x00004 /* Tree */
#define TSC_R_PHONEPOST 0x00005 /* Phone pole */
#define TSC_FLYING 0x00030 /* hit & goes flying */
#define TSC_V_PALM 0x00005 /* PALM = TSIGN_C|TSC_IMMOBILE|TSC_V_PALM */
#define TSC_HARD 0x00040 /* MAY *NOT* GET KNOCKED DOWN (NO TOPPLE) */
#define TSC_ROADKILL 0x00050 /* ROAD KILL */
#define TSC_COW_S 0x00001 /* MOOOOO! */
#define TSC_DEER_S 0x00002 /* a do a deer a female deer... ray a droplet full of sun */
#define TSC_PARTS_S 0x00003 /* FLYING CHUNKS OF MEAT THAT CAN BE COLLIED WITH */
#define TSC_IGNORE 0x00060 /* SORT, BUT DO NOTHING... */
#define TSC_SPARK_S 0x00006 /* Subcalss of TSC_IGNORE */
#define TSC_DUDE_S 0x00007 /* SAL THE STUD */
#define TSC_BABE_S 0x00008 /* THE BABE AT THE END OF THE RACE */
#define TSC_GGBRG_RAIL 0x60 /* golden gate bridge railing */
#define RDDEBRIS_C 0x00700 /* ROAD DEBRIS (construction horses, 55gal drums, mail boxes) */
#define RDD_55GAL 0x00001 /* 55Gallon Drum */
#define ROAD_SUBLIST_SS 0x01000 /* ROAD SUBLIST SUPER SET IDENTIFIER */
#define SPAWNER_C 0x00800 /* PROCESS WHICH SPAWNS OTHERS */
#define SPWN_SWITCH_T 0x00010 /* SWITCH SCAN PROCESS */
#define LOAD_REQ_T 0x00020 /* SYSTEM LOAD REQUEST */
#define TRAFFIC_T 0x00030 /* TRAFFIC DISPATCHER */
#define COLORCYC_T 0x00040 /* COLOR CYCLER PROCESS */
#define ANIMATION_T 0x00050 /* ANIMATION PROCESS */
#define GROUND_C 0x00900 /* GROUND (not ROAD or SHOULDER or BUILDING but GROUND) */

#endif /* SYSID_H */

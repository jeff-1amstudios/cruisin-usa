/*
 * Auto-derived from struct declarations and comment-documented layouts in the
 * Cruis'n USA .ASM sources.
 *
 * Notes:
 * - All offsets in the ASM sources are word-based on the TMS320C3, so each
 *   field here is modeled as a 32-bit slot unless the source explicitly marks
 *   it as float.
 * - Several source "structs" pack multiple halfwords/bytes into one 32-bit
 *   word. Those are kept as raw packed fields here so the imported layout
 *   matches the ROM/RAM data.
 * - When the same layout appears in both .ASM and .EQU, the .EQU definition
 *   is treated as canonical.
 * - COMP.ASM's "dictionary" declaration is not an array-of-structs layout;
 *   it is represented here as the whole backing storage block.
 */

#ifndef CRUISIN_USA_TYPES_H
#define CRUISIN_USA_TYPES_H

#include <stdint.h>

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef float f32;

typedef u32 word_addr_t;
typedef u32 tPALETTE_CODE;

struct CARBLK;
struct PROC;
typedef void (*PROC_FUNC)(struct PROC*);
struct PROC_CONTEXT;

typedef struct tVIEWLIST_entry {
    void* init;
    int arg;
    int frames;
    void* view;
} tVIEWLIST_entry;

enum {
    DICTIONARY_TABLE_SIZE = 4421,
};

typedef struct tDICT {
    /*
     * COMP.ASM:
     *   word 0..TABLE_SIZE-1: packed parent/code values
     *   word TABLE_SIZE..(2*TABLE_SIZE)-1: character values
     */
    u32 parent_code_and_code_value[DICTIONARY_TABLE_SIZE];
    u32 character[DICTIONARY_TABLE_SIZE];
} tDICT;

typedef struct tagGAMETRAK {
    u32 elapsed_time;
    u32 position;
    u32 max_mph;
    u32 collisions;
} tagGAMETRAK;

typedef struct LEG {
    u32 pos_x;
    u32 pos_y_with_lane_flag;
    u32 pos_z;
    u32 id;
} LEG;

typedef struct ERROR_LOG_ENTRY {
    u32 proc_id;
    u32 wake_addr;
    u32 dp;
    u32 sp;
    u32 ar0;
    u32 ar1;
    u32 ar2;
    u32 ar3;
    u32 ar4;
    u32 ar5;
    u32 ar6;
    u32 ar7;
    u32 ar4_oid;
    u32 race_num;
    u32 unfinished_game;
    u32 close_trak;
} ERROR_LOG_ENTRY;

typedef struct CHIP_TEST_TABLE {
    u32 test_table;
    u32 min_x;
    u32 min_y;
    u32 max_x;
    u32 max_y;
    u32 u_number;
    u32 ramrom;
} CHIP_TEST_TABLE;

typedef struct SIGMA_STARTUP {
    u32 model;
    u32 flag;
} SIGMA_STARTUP;

typedef struct PALXFER {
    struct PALXFER* link;
    void* source_addr;
    void* dest_addr;
    u32 count;
} PALXFER;

typedef struct VEHTAB {
    uintptr_t model;
    uintptr_t model_palette;
    uintptr_t animation;
    uintptr_t degraded_model;
    uintptr_t degraded_model_level2;
    u32 taillight_color_count;
    f32 mass;
    uintptr_t passby;
    uintptr_t backnoise;
    uintptr_t palette_table;
    uintptr_t drone_dyna_table;
} VEHTAB;

typedef struct ROM_VERTEX {
    u32 x_y;
    s32 z;
} ROM_VERTEX;

/*
 * DIRQ.ASM documents the ROM object blob pointed to by OBJ.OROMDATA as:
 *   radius
 *   vertex_count | (polygon_count << 16)
 *   ROM_VERTEX[number_of_vertices]
 *   ROM_POLYGON[number_of_polygons]
 *
 * Keep only the fixed header here; the trailing arrays are variable-length.
 */
typedef struct OROMDATA {
    u32 radius;
    u16 polygon_count;
    u16 vertex_count;
} OROMDATA;

typedef struct ROM_POLYGON {
    u32 palnum_and_cntl;
    u32 vertices_4_3_2_1;
    u32 iv_0_1;
    u32 iv_2_3;
    u32 texture_map_addr;
} ROM_POLYGON;

typedef struct ROM_ILLUM_POLYGON {
    u32 cntl;
    f32 nx;
    f32 ny;
    f32 nz;
    u32 vertices_4_3_2_1;
} ROM_ILLUM_POLYGON;

typedef struct FONTENTRY {
    u32 pre_and_trail;
    u32 x_start;
    u32 x_end;
    u32 y_start;
} FONTENTRY;

typedef struct ADJUSTMENT_RANGE {
    int low;
    int high;
    int default_value;
} ADJUSTMENT_RANGE;

typedef struct tTEXT {
    struct tTEXT* link;
    char* ptr;
    u32 font;
    f32 posx;
    f32 posy;
    f32 velx;
    f32 vely;
    u32 tiks;
    u32 color;
    u32 height;
    FONTENTRY* text_addr;
    u32 image_addr;
    u32 palette;
} tTEXT;

typedef struct tSHADOW_TEXT {
    tTEXT *front, *shadow;
} tSHADOW_TEXT;

typedef struct OBJ {
    struct OBJ* link;
    f32 posx;
    f32 posy;
    f32 posz;

    /* 3x3 rotation matrix laid out exactly as OMAT00..OMAT22 in OBJ.EQU. */
    f32 mat00;
    f32 mat10;
    f32 mat20;
    f32 mat01;
    f32 mat11;
    f32 mat21;
    f32 mat02;
    f32 mat12;
    f32 mat22;

    u32 romdata;
    u32 flags;
    u32 id;
    u32 palette;

    f32 vel_x;
    f32 vel_y;
    f32 vel_z;

    f32 rad_x;
    f32 rad_y;
    f32 rad_z;

    u32 process_link; /* OPLINK / OBLINK4 alias */
    u32 degrade_rom;
    u32 degrade_rom2;
    u32 romdata2;          /* OROMDATA2 / ODYNALIST alias */
    struct CARBLK* carblk; /* OCARBLK / OANIBLK alias */
    s32 dist;              /* ODIST */
    s32 radius;            /* ORAD */
    u32 usr1;
    u32 link2; /* OUSR2 alias */
    u32 link3; /* OUSR3 alias */
    u32 link4;
} OBJ;

typedef struct PROC {
    struct PROC* link;
    // u32 stack_ptr;
    u32 state;
    u32 id;
    u32 sleep_ticks;
    PROC_FUNC func;
    // u32 r4;
    // u32 r5;
    // f32 r6;
    // f32 r7;
    // u32 ar4;
    // u32 ar5;
    // u32 ar6;
    // u32 data[35];       /* PDATA..PSDATA-1 */
    // u32 stack_data[15]; /* PSDATA..PRCSIZ-1 */
    struct PROC_CONTEXT* ctx; // jeff added to replace the proc-local register and stack
} PROC;

typedef struct TRACKSEL {
    u32 index;
} TRACKSEL;

typedef struct CAR_POINT {
    f32 x;
    f32 y;
    f32 z;
    f32 road_delta_y;
    f32 y_velocity;
    u32 collided_road_object;
} CAR_POINT;

typedef struct CARBLK {
    struct CARBLK* link; // jeffh added, previously center.x was re-used as the link pointer
    CAR_POINT center;
    CAR_POINT right_front;
    CAR_POINT left_front;
    CAR_POINT left_rear;
    CAR_POINT right_rear;
    u32 on_road;
    u32 front_airborne;
    u32 rear_airborne;
    f32 turn;
    f32 traction;
    f32 max_accel;
    f32 throttle;
    f32 skid;
    f32 speed;
    f32 dist;
    f32 x_momentum;
    f32 y_momentum;
    f32 z_momentum;
    f32 brake;
    f32 y_rotation;
    f32 y_velocity_rotation;
    f32 last_y_rotation;
    f32 over_rotation;
    f32 mass;
    u32 spin_flag;
    f32 spin_radians;
    u32 bump_flag;
    s32 track_piece_position;
    f32 track_piece_distance;
    u32 track_piece_rank;
    u32 closest_track_piece;
    u32 gear;
    f32 rpm_x100;
    f32 x_plus;
    f32 x_minus;
    f32 y_plus;
    f32 y_minus;
    f32 z_plus;
    f32 z_minus;
    u32 debug_car_id;
    u32 shadow_flag;
    f32 x_lean;
    f32 z_lean;
    f32 road_friction;
    f32 offroad_friction;
    f32 dist_to_center;
    f32 wheel_scan_offsets;
    u32 reserved_72_85[14];
    u32 transmission;
    u32 track_id;
    u32 other_machine_controls;
    u32 car_num;
    u32 updated_this_frame;
} CARBLK;

/*
 * LEG.ASM also documents a float-based leg layout; keep that as a separate
 * alternate type, but prefer LEG above from SYS.EQU for imports.
 */
typedef struct LEG_ASM {
    f32 pos_x;
    f32 pos_y;
    f32 pos_z;
    u32 id;
} LEG_ASM;

typedef struct LEG_MAP_ENTRY {
    u32 pos_x;
    u32 pos_y_with_lane_flag;
    u32 pos_z;
    u32 id;
} LEG_MAP_ENTRY;

typedef struct STRDGROUP {
    u32 head;
    u32 bin;
    u32 flex_start;
    u32 flag;
    u32 index;
} STRDGROUP;

typedef struct TYCOB {
    u32 flag;
    f32 pos_x;
    f32 pos_y;
    f32 pos_z;
    f32 rad_y;
    u32 group;
    u32 group_overlay;
    f32 reverse_pos_x;
    f32 reverse_pos_y;
    f32 reverse_pos_z;
    f32 reverse_rad_y;
    u32 over2;
} TYCOB;

typedef struct VECTOR {
    f32 X;
    f32 Y;
    f32 Z;
    // f32 w;
} VECTOR;

typedef struct EQUATION {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
} EQUATION;

typedef struct MATRIX {
    f32 a00;
    f32 a01;
    f32 a02;
    f32 a10;
    f32 a11;
    f32 a12;
    f32 a20;
    f32 a21;
    f32 a22;
} MATRIX;

typedef struct DYNAOBJ {
    struct DYNAOBJ* link;
    f32 center_x;
    f32 center_y;
    f32 center_z;
    f32 trans_x;
    f32 trans_y;
    f32 trans_z;
    f32 mat00;
    f32 mat10;
    f32 mat20;
    f32 mat01;
    f32 mat11;
    f32 mat21;
    f32 mat02;
    f32 mat12;
    f32 mat22;
    u32 nverts;
    struct DYNAOBJ* parent;
    u32 flag;
} DYNAOBJ;

typedef struct SOUNDSTR {
    u32 priority;
    u32 timer;
    u32 timer_countdown;
    u32 start_addr;
    u32 current_addr;
    s32 volume;
    u32 sound_index;
    u32 repeat_count;
    u32 status;
} SOUNDSTR;

typedef struct REGRAC {
    u32 position;
    u32 object;
    u32 carblock;
    u32 proc;
} REGRAC;

/*
 * DELTA.EQU defines this relative to PROC.PDATA, so this is the standalone
 * payload layout for that process-local storage region.
 */
typedef struct DELTA_PROC_DATA {
    u32 status;
    u32 rady_delta;
    s32 init;
    f32 throttle;
    u32 tracked_piece;
    u32 mode;
    u32 stealth_ptr;
    u32 last_oid;
    u32 player_status;
    u32 playit;
    f32 lane;
    f32 player_dist;
    f32 old_player_dist;
    u32 model;
    u32 lane_switch_timer;
    u32 xlane;
    f32 min_throttle;
    f32 max_throttle;
    u32 update_flag;
} DELTA_PROC_DATA;

typedef struct RACEENTRY {
    u32 time[4];
    u32 init1;
    u32 init2;
    u32 init3;
    u32 rank;
} RACEENTRY;

typedef struct RACER_DISPATCH {
    u32 model;
    u32 position;
    f32 xlane;
    f32 max_accel;
    f32 rel;
    u32 palette;
} RACER_DISPATCH;

typedef struct COINTAB_ENTRY {
    // Word 0
    uint8_t coin[4]; // coin1, coin2, coin3, coin4

    // Word 1
    uint8_t units_per_credit;
    uint8_t units_for_bonus;
    uint8_t min_units;
    uint8_t credits_to_start;

    // Word 2
    uint8_t credits_to_continue;
    uint8_t show_partial_credits;
    uint8_t unused0;
    uint8_t unused1;

    // Words 3-9
    char* message_lines[3]; // line 1 required; line 2/3 may be 0

    char* coin1_denom_string; // 0 = NULL$
    char* coin2_denom_string; // 0 = NULL$
    char* coin3_denom_string; // 0 = NULL$
    char* coin4_denom_string; // 0 = NULL$

    // Word 10
    uint8_t coin_denom[4]; // coin1_denom, coin2_denom, coin3_denom, coin4_denom
} COINTAB_ENTRY;

typedef struct tDEMO_THANKS {
    int unk1;
    const char* message;
} tDEMO_THANKS;

typedef struct tCHOOSE_CAR_ENTRY {
    s32 x;
    s32 y;
    s32 z;
    u32 romdata;
    f32 rady;
    u32 oid;
} tCHOOSE_CAR_ENTRY;

typedef struct tCARPARAM {
    float max_accel;
    float traction;
    float road_friction;
    float offroad_friction;
} tCARPARAM;

typedef struct tROADKILL_TAB_ENTRY {
    uintptr_t death_romdata;
    int hit_sound;
    uintptr_t parts_list;
    float width_left;
    float width_right;
} tROADKILL_TAB_ENTRY;

typedef struct tGEESE_DIR_ENTRY {
    int speed;
    int direction;
    float dir_rad;
} tGEESE_DIR_ENTRY;

typedef struct tPAL {
    int flags_and_count;
    u32 data[128];
} tPAL;

typedef struct tPALLIST_ENTRY {
    int ref_count_and_pal_code;
} tPALLIST_ENTRY;

typedef struct tSECTION_ALLOC {
    int pal_index;
    int count;
} tSECTION_ALLOC;

typedef struct LOAD_SECTION_REQ_ARG {
    u32* source_addr;
    u32* dest_addr;
} LOAD_SECTION_REQ_ARG;

typedef struct PROC_CONTEXT {
    union {
        struct {
            LOAD_SECTION_REQ_ARG* lsr;
        } REQWAIT;
    };

} PROC_CONTEXT;

#endif

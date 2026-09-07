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

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
#ifndef CRUSN_PORT_TYPES_H
#include "../core/port_types.h"
#endif

typedef s32* tyco_stream_t;

typedef struct BLTMOD2D_MODEL {
    s32 radius;
    s32 vertex_polygon_count;
    s32 vertices[8];
    s32 control_palette;
    s32 vertex_indices;
    s32 iv01;
    s32 iv23;
    s32 image_addr;
} BLTMOD2D_MODEL;

typedef void (*void_func_ptr)(void);

typedef u32 word_addr_t;
typedef u32 tPALETTE_CODE;

struct CARBLK;
struct PROC;
typedef struct PROC PROC;
typedef void (*PROC_FUNC)(struct PROC*);
struct tDDYNA_TABLE;
struct tCAR_PALETTE_LIST;
struct DYNATAB;
struct tDDYNA_TABLE;

typedef struct VECTOR {
    c3x_f32_t X;
    c3x_f32_t Y;
    c3x_f32_t Z;
    // c3x_f32_t w;
} VECTOR;

typedef struct EQUATION {
    c3x_f32_t a;
    c3x_f32_t b;
    c3x_f32_t c;
    c3x_f32_t d;
} EQUATION;

typedef struct LINE2D {
    c3x_f32_t a;
    c3x_f32_t b;
    c3x_f32_t c;
} LINE2D;

typedef struct MATRIX {
    c3x_f32_t a00;
    c3x_f32_t a01;
    c3x_f32_t a02;
    c3x_f32_t a10;
    c3x_f32_t a11;
    c3x_f32_t a12;
    c3x_f32_t a20;
    c3x_f32_t a21;
    c3x_f32_t a22;
} MATRIX;

typedef struct OBJ_MATRIX {
    c3x_f32_t mat00;
    c3x_f32_t mat10;
    c3x_f32_t mat20;
    c3x_f32_t mat01;
    c3x_f32_t mat11;
    c3x_f32_t mat21;
    c3x_f32_t mat02;
    c3x_f32_t mat12;
    c3x_f32_t mat22;
} OBJ_MATRIX;

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
    void* model;
    int model_palette;
    struct DYNATAB* animation;
    void* degraded_model;
    void* degraded_model_level2;
    u32 taillight_color_count;
    c3x_f32_t mass;
    int passby;
    int backnoise;
    struct tCAR_PALETTE_LIST* palette_table;
    struct tDDYNA_TABLE* drone_dyna_table;
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
    u16 vertex_count;
    u16 polygon_count;
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
    u32 nx;
    u32 ny;
    u32 nz;
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
    c3x_f32_t posx;
    c3x_f32_t posy;
    c3x_f32_t velx;
    c3x_f32_t vely;
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
    // c3x_f32_t posx;
    // c3x_f32_t posy;
    // c3x_f32_t posz;
    VECTOR pos;

    /* 3x3 rotation matrix laid out exactly as OMAT00..OMAT22 in OBJ.EQU. */
    OBJ_MATRIX omatrix;

    void* romdata;
    u32 flags;
    u32 id;
    u32 palette;

    c3x_f32_t vel_x;
    c3x_f32_t vel_y;
    c3x_f32_t vel_z;

    union {
        struct VECTOR rad;
        struct {
            c3x_f32_t radx;
            c3x_f32_t rady;
            union {
                c3x_f32_t radz;
                uintptr_t radz_ptr;
            };
        };
    };

    union {
        struct PROC* plink; /* OPLINK / OBLINK4 alias */
        uintptr_t blink4;
    };
    void* degrade_rom;
    void* degrade_rom2;
    union { /* OROMDATA2 / ODYNALIST alias */
        void* romdata2;
        void* dynalist;
    };
    union {
        struct CARBLK* carblk; /* OCARBLK / OANIBLK alias */
        void* aniblock;
    };
    s32 dist;   /* ODIST */
    s32 radius; /* ORAD */
    uintptr_t usr1;
    c3x_f32_t usr1_as_float;
    union {
        uintptr_t link2;
        uintptr_t usr2;
    };
    union {
        uintptr_t link3;
        uintptr_t usr3;
    };
    uintptr_t link4;
} OBJ;

_Static_assert(offsetof(OBJ, vel_y) == offsetof(OBJ, vel_x) + sizeof(c3x_f32_t),
    "OBJ velocity components must be contiguous");
_Static_assert(offsetof(OBJ, vel_z) == offsetof(OBJ, vel_y) + sizeof(c3x_f32_t),
    "OBJ velocity components must be contiguous");

extern OBJ OBJSTR[];

typedef u32 OBJREF;

/*
 * _CARV0 clears the mixed CARBLK with STF 0. A C30 floating-point zero is
 * 0x80000000 in memory, so object-reference slots retain this non-null value
 * until an integer store initializes them.
 */
#define OBJREF_STF_ZERO UINT32_C(0x80000000)

extern const OBJ C30_STF_ZERO_OBJREF_VIEW;

static inline int OBJREF_IS_STF_ZERO(OBJREF ref) {
    return ref == OBJREF_STF_ZERO;
}

static inline OBJ* OBJREF_STF_ZERO_VIEW(void) {
    return (OBJ*)&C30_STF_ZERO_OBJREF_VIEW;
}

static inline OBJREF OBJ_TO_REF(const OBJ* obj) {
    if (obj == 0) {
        return 0;
    }
    return (OBJREF)((obj - OBJSTR) + 1);
}

static inline OBJ* OBJREF_TO_PTR(OBJREF ref) {
    if (ref == 0) {
        return 0;
    }
    if (OBJREF_IS_STF_ZERO(ref)) {
        /* Callers must reproduce the particular C30 pre-initialization use. */
        assert(0 && "attempted to dereference an STF-zero OBJREF");
        return 0;
    }
    return &OBJSTR[ref - 1];
}

typedef struct DGROUP_ENTRY {
    u32 head;
    tyco_stream_t bin;
    OBJ* fstart;
    int flag;
    int idx;
} DGROUP_ENTRY;

#define PROC_RESUME_STACK_SIZE 10

typedef struct TRACKSEL {
    u32 index;
} TRACKSEL;

typedef struct CAR_POINT {
    c3x_f32_t x;
    c3x_f32_t y;
    c3x_f32_t z;
    c3x_f32_t road_delta_y;
    c3x_f32_t y_velocity;
    OBJREF collided_road_object;
} CAR_POINT;

typedef struct CARBLK {
    CAR_POINT center;
    CAR_POINT right_front;
    CAR_POINT left_front;
    CAR_POINT left_rear;
    CAR_POINT right_rear;
    u32 on_road;
    u32 front_airborne;
    u32 rear_airborne;
    c3x_f32_t turn;
    c3x_f32_t traction;
    c3x_f32_t max_accel;
    c3x_f32_t throttle;
    c3x_f32_t skid;
    c3x_f32_t speed;
    c3x_f32_t dist;
    c3x_f32_t x_momentum;
    c3x_f32_t y_momentum;
    c3x_f32_t z_momentum;
    c3x_f32_t brake;
    c3x_f32_t y_rotation;
    c3x_f32_t y_velocity_rotation;
    c3x_f32_t last_y_rotation;
    c3x_f32_t over_rotation;
    c3x_f32_t mass;
    u32 spin_flag;
    c3x_f32_t spin_radians;
    u32 bump_flag;
    s32 track_piece_position;
    c3x_f32_t track_piece_distance;
    u32 track_piece_rank;
    OBJREF closest_track_piece;
    u32 gear;
    c3x_f32_t rpm_x100;
    c3x_f32_t x_plus;
    c3x_f32_t x_minus;
    c3x_f32_t y_plus;
    c3x_f32_t y_minus;
    c3x_f32_t z_plus;
    c3x_f32_t z_minus;
    u32 debug_car_id;
    u32 shadow_flag;
    c3x_f32_t x_lean;
    c3x_f32_t z_lean;
    c3x_f32_t road_friction;
    c3x_f32_t offroad_friction;
    c3x_f32_t dist_to_center;
    VECTOR wheel_scan_offsets[5];
    u32 transmission;
    u32 track_id;
    u32 other_machine_controls;
    u32 car_num;
    u32 updated_this_frame;
} CARBLK;

typedef struct LEG_PAYLOAD {
    union {
        struct {
            VECTOR pos;
            u32 id;
        } as_float;

        struct {
            int pos_x;
            int pos_y_with_lane_flag;
            int pos_z;
            u32 id;
        } as_fixed;
    };
} LEG_PAYLOAD;

typedef struct LEG_SSLL_ENTRY {
    struct LEG_SSLL_ENTRY* leg_ssll_next_offset;
    LEG_PAYLOAD leg;
} LEG_SSLL_ENTRY;

typedef struct STRDGROUP {
    u32 head;
    u32 bin;
    u32 flex_start;
    u32 flag;
    u32 index;
} STRDGROUP;

typedef struct TYCOHEADER {
    u32 flag;
    VECTOR pos;
    c3x_f32_t rad_y;
    u32 group;
} TYCOHEADER;

typedef enum DYNAFLAG {
    DYNAF_SHADOW = -1,
    DYNAF_BODY = 0,
    DYNAF_REARWHEEL = 1,
    DYNAF_FRONTWHEEL = 2,
} DYNAFLAG;

typedef struct DYNATAB_ENTRY {
    VECTOR center;
    int verts_minus_1;
    DYNAFLAG flag;
} DYNATAB_ENTRY;

typedef struct DYNATAB {
    int count_minus_1;
    DYNATAB_ENTRY entries[10];
} DYNATAB;

typedef struct DYNAOBJ {
    struct DYNAOBJ* link;
    c3x_f32_t center_x;
    c3x_f32_t center_y;
    c3x_f32_t center_z;
    c3x_f32_t trans_x;
    c3x_f32_t trans_y;
    c3x_f32_t trans_z;
    OBJ_MATRIX omatrix;
    u32 nverts;
    struct OBJ* parent;
    DYNAFLAG flag;
} DYNAOBJ;

typedef struct SNDSTR_t {
    u32 priority;
    u32 timer;
    u32 timer_countdown;
    u32 start_addr;
    u32 current_addr;
    s32 volume;
    u32 sound_index;
    u32 repeat_count;
    u32 status;
} SNDSTR_t;

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
    c3x_f32_t throttle;
    u32 tracked_piece;
    u32 mode;
    u32 stealth_ptr;
    u32 last_oid;
    u32 player_status;
    u32 playit;
    c3x_f32_t lane;
    c3x_f32_t player_dist;
    c3x_f32_t old_player_dist;
    u32 model;
    u32 lane_switch_timer;
    u32 xlane;
    c3x_f32_t min_throttle;
    c3x_f32_t max_throttle;
    u32 update_flag;
} DELTA_PROC_DATA;

typedef struct RACEENTRY {
    u32 time;
    u32 pad[3];
    s32 init1;
    s32 init2;
    s32 init3;
    u32 rank;
} RACEENTRY;

typedef struct RACER_DISPATCH {
    u32 model;
    u32 position;
    c3x_f32_t xlane;
    c3x_f32_t max_accel;
    c3x_f32_t rel;
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
    c3x_f32_t rady;
    u32 oid;
} tCHOOSE_CAR_ENTRY;

typedef struct tCARPARAM {
    c3x_f32_t max_accel;
    c3x_f32_t traction;
    c3x_f32_t road_friction;
    c3x_f32_t offroad_friction;
} tCARPARAM;

typedef struct tROADKILL_TAB_ENTRY {
    uintptr_t death_romdata;
    int hit_sound;
    uintptr_t parts_list;
    c3x_f32_t width_left;
    c3x_f32_t width_right;
} tROADKILL_TAB_ENTRY;

typedef struct tGEESE_DIR_ENTRY {
    int speed;
    int direction;
    c3x_f32_t dir_rad;
} tGEESE_DIR_ENTRY;

typedef struct tPAL {
    int flags_and_count;
    u32 data[128];
} tPAL;

typedef struct tPALLIST_ENTRY {
    int ref_count_and_pal_code;
} tPALLIST_ENTRY;

typedef struct tPALETTE_OVERWRITE_ENTRY {
    u32 source_palette_rom;
    u32 destination_palette_index;
} tPALETTE_OVERWRITE_ENTRY;

typedef struct tCAR_PALETTE_LIST {
    int count;
    tPAL* palette_addr[10];
} tCAR_PALETTE_LIST;

typedef struct tDDYNA_TABLE_ENTRY {
    int vertices;
    struct {
        int X;
        int Y;
        int Z;
    } center;
} tDDYNA_TABLE_ENTRY;

typedef struct tDDYNA_TABLE {
    int axles;
    int vertices;
    tDDYNA_TABLE_ENTRY entries[3]; // max is 3 axles
} tDDYNA_TABLE;

typedef struct tSECTION_ALLOC {
    int pal_index;
    int count;
} tSECTION_ALLOC;

typedef struct LOAD_SECTION_REQ_ARG {
    u32* source_addr;
    u32* dest_addr;
} LOAD_SECTION_REQ_ARG;

typedef struct LOAD_SINGLE_SECTION_GROUP {
    u32 unused0;
    u32 count;
} LOAD_SINGLE_SECTION_GROUP;

typedef enum eDELTA_MODEL {
    VETTE_MOD = 0,
    HOTROD_MOD = 1,
    BULLET_MOD = 2,
    TESTOR_MOD = 3,
    GTRUCK_MOD = 4,
    FTRUCK_MOD = 5,
    CBUS_MOD = 6,
    COPCAR_MOD = 7,
    MUSCLE_MOD = 8,
    CARAVAN_MOD = 9,
    SBUS_MOD = 10,
    PTRUCKG_MOD = 11,
    MUSTANG_MOD = 12,
    // **reserved	.set	13
    JEEP_MOD = 14,
    PLYR_COPCAR_MOD = 15,
    PLYR_GTRUCK_MOD = 16,
    PLYR_SBUS_MOD = 17
} eDELTA_MODEL;

typedef struct RACER {
    eDELTA_MODEL model;
    u32 position;
    c3x_f32_t xlane;
    c3x_f32_t maxaccel;
    c3x_f32_t rel;
    tPAL* palette;
} RACER;

typedef struct VIEWLIST_ENTRY {
    void_func_ptr starting_func;
    int track_id;
    int frames;
    void_func_ptr view_func;
} VIEWLIST_ENTRY;

struct WAVEFLAG_ENTRY;

typedef struct PROC_CONTEXT {
    int pedal_released; // process-saved R5 used by INIT_PEDALCHK/PEDALCHK

    struct {
        OBJ* parent;
    } DROP_COCONUTS;

    struct {
        OBJ* obj;
        c3x_f32_t x_rate;
        c3x_f32_t y_rate;
        c3x_f32_t z_rate;
        c3x_f32_t x_total;
        c3x_f32_t y_total;
        c3x_f32_t z_total;
    } FLYCARP;

    struct {
        int loop_count;
        OBJ* cars[4]; // PDATA..PDATA+3 populated by GETTHECARS
    } CHOOSE_TRANSMISSION_FRAME;
    struct {
        int loop_count;
        OBJ* obj;
    } OPEN_DOOR_PROC_FRAME;
    struct {
        int selected_car;
        c3x_f32_t car_desired_y[4];
        c3x_f32_t car_start_y[4];
        c3x_f32_t lift_desired_y[4];
        c3x_f32_t lift_start_y[4];
    } ROUNDER;
    struct {
        OBJ* car;
        OBJ* lift;
        OBJ* tracking_obj;
        int loop_count;
    } ZOOMTOCAR_FRAME;
    struct {
        OBJ* car; // AR4 passed across the branch to PLYR_INTRO_ENTER
    } PLYR_INTRO_ENTER_FRAME;
    struct {
        int loop_count;
    } MOVEIN_HUD_EQUIP_FRAME;
    struct {
        int loop_count;
    } MOVEOUT_HUD_EQUIP_FRAME;
    struct {
        OBJ* obj;
        int loop_count;
        int script_index;
        int script_start;
    } BABE_TROPHY_FRAME;

    /* These coroutine frames may be nested below another logical process, so
       they cannot share the function-local union below. */
    struct {
        int track_selection;
        int wait_frames;
        OBJ* wait_objects[3];
    } ISSUE_STARTGAME_FRAME;
    struct {
        tTEXT* prompt;
        int watched_credits;
        int start_enabled;
        int start_delay;
        int result;
    } INSMORE_FRAME;
    struct {
        OBJ* initial_objs[3];
        int initial_chars[3];
        int place;
        int white_pal;
        int grey_pal;
        int race_number;
        int old_choice;
        int pedal_trigger;
        int character_index;
        int debounce_counter;
        int loading_clear_frames;
    } ENTER_INITIALS_FRAME;

    union {
        struct {
            int race_number;
            int oid_group;
            int flash_pal_index;
        } FLASH_LETTERS_PROC;
        struct {
            int wait_frames; // AR5 across ISSUE_STARTGAME's SLEEP
        } ISSUE_STARTGAME;
        struct {
            OBJ* small_cursor;
            OBJ* big_cursor;
            void* rom_small_cursor;
            void* rom_big_cursor;
            int old_index;
            int last_hidden_track;
            int loop_count;
            int feedback_count;
            int motion_recalibration;
            char motion_countdown[16];
        } TRACK_SELECTION;
        struct {
            int loop_count;
        } ZOOMINP;
        struct {
            int loop_count;
        } TSEL_ZOOMOUTP;
        struct {
            OBJ* obj;
            int loop_count;
        } CENTER_THEONE;
        struct {
            OBJ* obj;
            int loop_count;
            c3x_reg_t x_velocity;
        } TRANS_CENTER_THEONE;
        struct {
            OBJ* obj;
            int loop_count;
            c3x_reg_t y_velocity;
        } DROP_TRANS_ELEMENT;
        struct {
            int loop_count;
        } TILE_PIECES;
        struct {
            OBJ* left_obj;
            OBJ* right_obj;
            int loop_count;
        } MOVE_PUSH_BOX;
        struct {
            OBJ* obj;
            int script_index;
        } TRACKSEL_ANIMATION;
        struct {
            int race_number;
            int white_pal;
            struct PROC* flash_proc;
            int zoom_count;
        } DISPLAY_HS;
        struct {
            int attrwave;
            int sleep_ticks;
        } DISPLAY_HIGH_SCORES;
        struct {
            LOAD_SECTION_REQ_ARG* lsr;
        } REQWAIT;
        struct {
            int palette_code;
            int step_index;
        } CPOINT_LIGHT;
        struct {
            int step_index;
        } TRAFFIC_LIGHT;
        struct {
            tTEXT* front_text;
            tTEXT* shadow_text;
            c3x_f32_t posx;
            int loop_count;
        } SHOW_RACE_NAME;
        struct {
            int loop_count;
        } BLINK_FREEBE;
        struct {
            int background_color;
            int wait_count;
            int palette_index;
            int flag_index;
            c3x_reg_t flag_y;
        } BONSCRN2;
        struct {
            OBJ* objects[4];
            c3x_f32_t lead_x;
            c3x_f32_t lead_y;
            c3x_f32_t lead_z;
            c3x_f32_t theta[4];
            c3x_f32_t lead_theta;
            MATRIX lead_matrix;
            c3x_f32_t x_delta;
            c3x_f32_t y_delta;
            int loop_count;
        } MAP_ANIMATION;
        struct {
            c3x_reg_t target_x;
            c3x_reg_t target_z;
            int loop_count;
        } BACKUP_CAMERA;
        struct {
            int loop_count;
        } FREE_RACE_ANNOUNCE;
        struct {
            tTEXT* front_text[2];
            tTEXT* shadow_text[2];
            c3x_reg_t posx;
            int loop_count;
        } SHOWLEG_PROC;
        struct {
            tTEXT* front_text[2];
            tTEXT* shadow_text[2];
            c3x_reg_t posx;
            int loop_count;
        } SHOWNEXTLEG_PROC;
        struct {
            OBJ* obj;
            int loop_count;
        } PLACE_FLAG_PROC;
        struct {
            tTEXT* label_front;
            tTEXT* label_shadow;
            tTEXT* value_front;
            tTEXT* value_shadow;
            c3x_reg_t label_posx;
            c3x_reg_t value_posx;
            int loop_count;
            char buffer[32];
        } BONS_TEXT;
        struct {
            tTEXT* front;
            tTEXT* shadow;
            const char* string;
            int toggle;
        } BONS_HOTTIME;
        struct {
            int position_index;
            int ypos;
            int loop_count;
        } BONS_POSITION;
        struct {
            OBJ* objects[3];
            int loop_count;
        } DISPLAY_H2H_WINNER;
        struct {
            OBJ* obj;
            int loop_count;
            int script_index;
        } BABE_WAVEFLAG;
        struct {
            OBJ* obj;
            int script_start;
        } BABE_IBO;
        struct {
            int saved_mode;
            const struct WAVEFLAG_ENTRY* list_ptr;
            int list_count;
        } WAVEFLAG;
        struct {
            struct PROC* sigma_proc;
        } SIGMA_DISPATCHER;
        struct {
            OBJ* player_obj;
            c3x_f32_t initial_x;
            c3x_f32_t initial_z;
            int sleep_ticks;
        } RHO_DISPATCHER;
        struct {
            OBJ* obj;                 // AR4
            CARBLK* carblk;           // AR5
            c3x_f32_t body_x_radians; // R6
            c3x_f32_t old_car_speed;  // R7
            union {
                c3x_f32_t old_orady;
                c3x_f32_t wreck_x_rate;
            }; // PDATA
            union {
                c3x_f32_t body_z_radians;
                c3x_f32_t wreck_y_rate;
            }; // PDATA+1 / R5
            union {
                c3x_f32_t wheel_x_radians;
                c3x_f32_t wreck_z_rate;
            }; // PDATA+2
            c3x_f32_t wreck_x_total; // PDATA+3
            c3x_f32_t wreck_y_total; // PDATA+4
            c3x_f32_t wreck_z_total; // PDATA+5
            c3x_f32_t wreck_road_y;  // PDATA+20
            int wreck_old_view;      // PDATA+21
        } CARPROC;
        struct {
            OBJ* source_obj;
            OBJ* obj;
            int script_index;
            c3x_f32_t velocity_x;
            c3x_f32_t velocity_z;
            int smoke_counted;
        } PUFF_PROC;
        struct {
            OBJ* player_obj;      // AR6
            OBJ* left_flame;      // AR4
            OBJ* right_flame;     // AR5
            MATRIX* body_matrix;  // PDATA+2
            int frame_on;         // PDATA+1
            int frames_remaining; // R5
        } FLAME_PRC;
        struct {
            OBJ* car_obj;           // AR4
            CARBLK* carblk;         // AR5
            int num_smokes;         // PDATA
            int palette;            // PDATA+1
            OBJ* smoke_objs[20];    // PDATA+2..
            c3x_reg_t delay_frames; // R7
        } SMOKE_PROC;
        struct {
            VECTOR collision_offset;        // PDATA..PDATA+2
            int frame_on;                   // PDATA+3
            OBJ* car_obj;                   // PDATA+4
            CARBLK* carblk;                 // PDATA+5
            int num_sparks;                 // PDATA+6
            OBJ* spark_objs[6];             // PDATA+7..
            const int* spark_animations[6]; // host pointers represented separately from 32-bit OVELZ
            c3x_reg_t delay_frames;         // R7
        } SPARK_PROC;
        struct {
            OBJ* obj;        // AR4
            MATRIX rotation; // PDATA+2..
        } FLYCOLLP;
        struct {
            OBJ* obj;                 // AR4
            c3x_f32_t rotation_delta; // R7
            c3x_f32_t accumulated;    // R6
        } SIGNFALL;
        struct {
            OBJ* obj;                 // AR4
            c3x_f32_t rotation_delta; // R7
            int loop_count;           // AR6
        } TREESHAK;
        struct {
            OBJ* obj;          // AR4
            const int* script; // AR6
            int script_index;  // AR5 - AR6
        } BACKGRND_PLAINANI_PROC;
        struct {
            OBJ* obj;        // AR4
            c3x_f32_t speed; // R6
        } BACKGRND_CARFORWARD;
        struct {
            OBJ* obj;                     // AR4
            c3x_f32_t speed;              // R7
            c3x_f32_t remaining_distance; // R6
        } BACKGRND_PLANE_FWRD;
        struct {
            OBJ* obj;                     // AR4
            c3x_f32_t speed;              // R7
            c3x_f32_t remaining_distance; // R6
        } BACKGRND_TRAIN_FWRD;
        struct {
            int decomp_count;
            int attrwave;
            struct OBJ* obj;
            int sound_ticks;
        } MIDWAYSPIN;
        struct {
            int decomp_count;
        } MSLP_CHECK;
        struct {
            int decomp_count;
            struct OBJ* obj;
            int ticks;
        } SPIN_CAR;
        struct {
            int rank;       // R4
            OBJ* obj;       // AR4
            CARBLK* carblk; // AR5
            int delta_status;
            c3x_f32_t delta_radydelta;
            int delta_init;
            c3x_f32_t delta_throttle;
            OBJ* delta_tpiece;
            int delta_mode;
            LEG* delta_sptr;
            int delta_last_oid;
            int delta_pstat;
            int delta_playit;
            c3x_f32_t delta_lane;
            c3x_f32_t delta_plyrdist;
            c3x_f32_t delta_oplyrdist;
            int delta_model;
            int delta_lane_switch_tmr;
            c3x_f32_t delta_xlane;
            c3x_f32_t delta_min_throttle;
            c3x_f32_t delta_max_throttle;
            int delta_update_fl;
            c3x_f32_t road_offset;
            c3x_f32_t powersurge;
            c3x_f32_t powercatch;
            int surgetime;
            int catchtime;
            int stealthmode;
            c3x_f32_t relativity;
            RACER* initindex;
            int passcnt;
            c3x_f32_t finishdist;
            c3x_f32_t finishrot;
            int sigma_once;
            int sigma_yell;
            int sigma_flag;
            int rho_noise;
            int rho_yell;
            int rho_init;
            c3x_f32_t rho_theta;
            c3x_f32_t rho_theta_delta;
            c3x_f32_t rho_amp;
            c3x_f32_t rho_xhead;
            int rho_hit_smoke_count;
            int breakdown_count;
        } RACER_DRONE;
        struct {
            int background_color;
            int cut_pan;
            c3x_f32_t zoomvel;
            u32 camera_xyzr[4];
            u32 newcamera_xyzr[4];
            c3x_f32_t new_posx;
            c3x_f32_t new_posy;
            c3x_f32_t new_posz;
            c3x_f32_t new_radx;
            c3x_f32_t new_rady;
            c3x_f32_t new_radz;
            MATRIX new_matrix;
            int camera_view;
            c3x_f32_t camera_vel;
            OBJ* carobj;
            c3x_f32_t cardis;
            c3x_f32_t camera_accel;
            c3x_f32_t camera_infin;
            u32 temp_data[10];
            int list_num;
            c3x_f32_t camera_lane;
            OBJ* objins;
            c3x_f32_t camyoff;
            LEG_PAYLOAD* road_obj; // synthetic state used to preserve AR4 road segment across calls
            uintptr_t view_script; // synthetic state used to preserve AR6 across sleeps
            int frames_left;       // synthetic state used to preserve AR5 across sleeps
            c3x_f32_t watch_rady;  // synthetic state used to preserve R6 across WATCH_VIEW calls
        } ATTRACT_DELTA;
        struct {
            OBJ* obj;
            c3x_f32_t speed_factor;
            int frames_left;
        } LOGO_PROC;
        struct {
            OBJ* obj;
            c3x_f32_t radians;
        } ULTRA_PROC;
    };

} PROC_CONTEXT;

struct PROC {
    PROC* link;
    u32 resume_states[PROC_RESUME_STACK_SIZE];
    u32 current_resume_depth;
    u32 yielded;
    u32 id;
    u32 sleep_ticks;
    PROC_FUNC func;
    PROC_CONTEXT ctx;
};

#endif

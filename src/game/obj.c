#include "obj.h"
#include "error.h"
#include "globals.h"
#include "macs.h"
#include "mproc.h"
#include "pall.h"
#include "port.h"
#include "sys.h"
#include "sysid.h"
#include "validator.h"
#include "vunit.h"

/*
 * Source module: asm/OBJ.ASM
 */

void OBJ_INIT(void);
OBJ* OBJ_GETE(void* romdata);
OBJ* OBJ_GET(void);
static void GETDIST(OBJ* obj /*AR2*/);
void OBJ_INSERTP(OBJ* obj /*AR2*/);
void OBJ_INSERTLP(void);
void OBJ_INSERTHP(void);
void OBJ_INSERT(OBJ* obj /*AR2*/);
OBJ* OBJ_FIND_FIRST_PRIORITY(int oid /*AR2*/);
OBJ* OBJ_FIND_FIRST(int oid /*AR2*/);
void OBJ_FREE_GROUND(OBJ* obj /*AR2*/);
void OBJ_FREE_SIGN(OBJ* obj /*AR2*/);
void OBJ_FREE_DRIVE(OBJ* obj /*AR2*/);
void OBJ_FREE_PROC(OBJ* obj /*AR2*/);
static void OBJ_FREE_SUPPLEMENTAL(OBJ* obj, OBJ** list_head, int error_code);
void OBJ_DELETE(void);
void OBJ_DELETE_CLASS(void);
void OBJ_PULL(OBJ* obj /*AR2*/);
void OBJ_FREE(OBJ* obj /*AR2*/);
void ZSORTWT(void);
static void PLYRDLINK(void);
static void PLYRSORT(void);
static void DRONESORT(void);
void OSCAN(void);
void ISCAN(void);
void RESCAN(void);
void ZSORTPRIOR(void);
static void ZSORTACT(void);
void OBJ_MAKE(void);
OBJ* OBJ_QMAKE(void* romdata /*AR2*/, int posx /*R2*/, int posy /*R3*/, int posz /*RC*/);

#define OFREEI OFREE
#define OBJSTRI OBJSTR

static int ACTIVEHI;

/*
 *----------------------------------------------------------------------------
 *OBJECT SYSTEM
 *
 */

/* asm: OACTIVE	.bss	OACTIVE,1 */
OBJ* OACTIVE;
/* asm: OFREE	.bss	OFREE,1 */
OBJ* OFREE;
/* asm: IDLE_LIST	.bss	IDLE_LIST,1 */
OBJ* IDLE_LIST;
/* asm: OACTIVE_PRIORITY	.bss	OACTIVE_PRIORITY,1 */
OBJ* OACTIVE_PRIORITY;
/* asm: OLOW_PRIORITY	.bss	OLOW_PRIORITY,1 */
OBJ* OLOW_PRIORITY;
/* asm: OHIGH_PRIORITY	.bss	OHIGH_PRIORITY,1 */
OBJ* OHIGH_PRIORITY;
/* asm: OACTIVECNT	.bss	OACTIVECNT,1 */
int OACTIVECNT;
/* asm: OFREECNT	.bss	OFREECNT,1 */
int OFREECNT;
/* asm: OMAX_OBJECTS	.bss	OMAX_OBJECTS,1 */
int OMAX_OBJECTS;
OBJ OBJSTR[NUM_OBJECTS];
/* asm: COMM_DRONE_PTR	.bss	COMM_DRONE_PTR,1 */
OBJ* COMM_DRONE_PTR;

/* asm: PLYRTEMP	.BSS	PLYRTEMP,1 */
OBJ* PLYRTEMP;

/*
 *----------------------------------------------------------------------------
 *INITIALIZE OBJECT DATA STRUCTURES
 *
 */
void OBJ_INIT(void) {
    int i;

    // asm: 	PUSH	R0
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1

    // asm: 	LDI	NUM_OBJECTS,R0
    // asm: 	STI	R0,@OFREECNT
    OFREECNT = NUM_OBJECTS;

    // asm: 	LDI	0,R0
    // asm: 	STI	R0,@OACTIVE
    OACTIVE = NULL;
    // asm: 	STI	R0,@IDLE_LIST
    IDLE_LIST = NULL;
    // asm: 	STI	R0,@OACTIVE_PRIORITY
    OACTIVE_PRIORITY = NULL;
    // asm: 	STI	R0,@OACTIVECNT
    OACTIVECNT = 0;
    // asm: 	STI	R0,@OMAX_OBJECTS
    OMAX_OBJECTS = 0;
    // asm: 	STI	R0,@OLOW_PRIORITY
    OLOW_PRIORITY = NULL;
    // asm: 	STI	R0,@OHIGH_PRIORITY
    OHIGH_PRIORITY = NULL;

    // asm: 	STI	R0,@DRIVE_LIST
    DRIVE_LIST = NULL;
    // asm: 	STI	R0,@CAR_LIST
    CAR_LIST = NULL;
    // asm: 	STI	R0,@SIGN_LIST
    SIGN_LIST = NULL;
    // asm: 	STI	R0,@GROUND_LIST
    GROUND_LIST = NULL;

    // asm: 	STI	R0,@COMM_DRONE_PTR
    COMM_DRONE_PTR = NULL;

    // asm: 	LDI	@OFREEI,AR0	 	;GET FREE POINTER
    // asm: 	LDI	@OBJSTRI,AR1
    // asm: 	LDI	NUM_OBJECTS-1,RC
    OFREE = OBJSTR; // ;GET FREE POINTER

    // asm: 	RPTB	OINITL
    for (i = 0; i < NUM_OBJECTS - 1; i++) {
        // asm: 	STI	AR1,*AR0
        OBJSTR[i].link = &OBJSTR[i + 1];

        // asm: 	LDI	AR1,AR0
        // asm: OINITL	ADDI	OBJSIZ,AR1
    }

    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*AR0
    OBJSTR[NUM_OBJECTS - 1].link = NULL;

    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R0
    // asm: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *OBJ_GETE
 *	GET EXTENDED OBJECT
 *
 *PARAMETERS
 *	AR2	ROMDATA POINTER
 *
 *RETURNS
 *	(SUCCESSFUL)
 *	CARRY CLR
 *	AR0	OBJECT POINTER
 *	(UNSUCCESSFUL)
 *	CARRY SET
 *
 */
OBJ* OBJ_GETE(void* romdata /*AR2*/) {
    OBJ* obj;
    u32 control_word;

    // asm 00007035: 	CALL	OBJ_GET
    obj = OBJ_GET();

    // asm 00007036: 	RETSC				;NO OBJECTS AVAILABLE RETURN ERROR CODE
    if (obj == NULL) {
        return NULL; // ;NO OBJECTS AVAILABLE RETURN ERROR CODE
    }

    // asm 00007037: 	PUSH	R0
    // asm 00007038: 	PUSH	R1

    // asm 00007039: 	STI	AR2,*+AR0(OROMDATA)
    obj->romdata = romdata;

    // asm 0000703A: 	LDI	*AR2,R0			;get RADIUS of object
    // asm 0000703B: 	STI	R0,*+AR0(ORAD)		;SAVE THE SILLY RADIUS
    obj->radius = *(s32*)romdata; // ;get RADIUS of object / ;SAVE THE SILLY RADIUS
    // MAME_ASSERT_REG_AT_ADDR(0x0000703C, "R0", &obj->radius);

    // asm 0000703C: 	LDI	*-AR2,R0		;GET CONTROL WORD
    control_word = *((u32*)romdata - 1); // ;GET CONTROL WORD
    // MAME_ASSERT_REG_AT_ADDR(0x0000703D, "R0", &control_word);

    // asm 0000703D: 	TSTB	EOBJ_ILLUM,R0
    // asm 0000703E: 	BZ	NOT_ILLUM
    if ((control_word & EOBJ_ILLUM) != 0) {
        // asm 0000703F: 	LDI	*+AR0(OFLAGS),R1
        // asm 00007040: 	OR	O_ILLUM,R1
        // asm 00007041: 	STI	R1,*+AR0(OFLAGS)
        obj->flags |= O_ILLUM;
    }

NOT_ILLUM:
    // asm 00007042: 	TSTB	EOBJ_1PAL,R0
    // asm 00007043: 	BZ	NOT_1PAL
    if ((control_word & EOBJ_1PAL) != 0) {
        // asm 00007044: 	LDI	*+AR0(OFLAGS),R1
        // asm 00007045: 	OR	O_1PAL,R1
        // asm 00007046: 	STI	R1,*+AR0(OFLAGS)
        obj->flags |= O_1PAL;

        // asm 00007047: 	LDI	R0,AR2
        // asm 00007048: 	AND	0FFfh,AR2
        // asm 00007049: 	CALL	PAL_FIND
        // asm 0000704A: 	BC	$
        // asm 0000704B: 	STI	R0,*+AR0(OPAL)
        obj->palette = PAL_FIND(control_word & 0x0fff);
        // MAME_ASSERT_REG_AT_ADDR(0x0000704B, "R0", &obj->palette);
    }

NOT_1PAL:
    // asm 0000704C: 	CLRC
    // asm 0000704D: 	POP	R1
    // asm 0000704E: 	POP	R0
    // asm 0000704F: 	RETS
    return obj;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *OBJ_GET
 *	ALLOCATE AN OBJECT BLOCK FROM THE OBJECT FREE LIST
 *
 *RETURNS
 *	(SUCCESSFUL)
 *		CARRY CLR
 *		AR0	OBJECT POINTER
 *	(UNSUCCESSFUL)
 *		CARRY SET
 *
 *
 */
OBJ* OBJ_GET(void) {
    OBJ* obj;

    // asm 00007050: 	PUSH	R0
    // asm 00007051: 	LDI	@OFREE,R0
    // asm 00007052: 	LDI	R0,AR0
    obj = OFREE;

    // asm 00007053: 	BZ	NOOBJ			;lockup if out of objects
    if (obj == NULL) {
        goto NOOBJ; // ;lockup if out of objects
    }

    // asm 00007054: 	LDI	*AR0,R0
    // asm 00007055: 	STI	R0,@OFREE		;and update free list
    OFREE = obj->link; // ;and update free list

    // asm 00007056: 	LDI	@OFREECNT,R0
    // asm 00007057: 	SUBI	1,R0
    // asm 00007058: 	STI	R0,@OFREECNT
    OFREECNT -= 1;

    // asm 00007059: 	CLRI	R0
    // asm 0000705A: 	STI	R0,*+AR0(OFLAGS)
    obj->flags = 0;
    // asm 0000705B: 	STI	R0,*+AR0(OPLINK)
    obj->plink = 0;
    // asm 0000705C: 	STI	R0,*+AR0(OUSR1)
    obj->usr1 = 0;
    obj->usr1_as_float = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 0000705D: 	STI	R0,*+AR0(OID)
    obj->id = 0;
    // asm 0000705E: 	STI	R0,*+AR0(OLINK2)
    obj->link2 = 0;
    // asm 0000705F: 	STI	R0,*+AR0(OLINK3)
    obj->link3 = 0;
    // asm 00007060: 	STI	R0,*+AR0(OLINK4)
    obj->link4 = 0;

    // asm 00007061: 	CLRF	R0
    // asm 00007062: 	STF	R0,*+AR0(OPOSX)
    obj->pos.X = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 00007063: 	STF	R0,*+AR0(OPOSY)
    obj->pos.Y = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 00007064: 	STF	R0,*+AR0(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 00007065: 	STF	R0,*+AR0(OVELX)
    obj->vel_x = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 00007066: 	STF	R0,*+AR0(OVELY)
    obj->vel_y = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 00007067: 	STF	R0,*+AR0(OVELZ)
    obj->vel_z = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 00007068: 	STF	R0,*+AR0(ORADX)
    obj->rad.X = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 00007069: 	STF	R0,*+AR0(ORADY)		;CLEAR RADIANS TO AVOID LOCKUP
    obj->rad.Y = C3X_STF(C3X_REG(C3X_FROM_INT(0))); // ;CLEAR RADIANS TO AVOID LOCKUP
    // asm 0000706A: 	STF	R0,*+AR0(ORADZ)
    obj->rad.Z = C3X_STF(C3X_REG(C3X_FROM_INT(0)));

    // asm 0000706B: 	STF	R0,*+AR0(OMATRIX+1)
    obj->omatrix.mat10 = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 0000706C: 	STF	R0,*+AR0(OMATRIX+2)
    obj->omatrix.mat20 = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 0000706D: 	STF	R0,*+AR0(OMATRIX+3)
    obj->omatrix.mat01 = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 0000706E: 	STF	R0,*+AR0(OMATRIX+5)
    obj->omatrix.mat21 = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 0000706F: 	STF	R0,*+AR0(OMATRIX+6)
    obj->omatrix.mat02 = C3X_STF(C3X_REG(C3X_FROM_INT(0)));
    // asm 00007070: 	STF	R0,*+AR0(OMATRIX+7)
    obj->omatrix.mat12 = C3X_STF(C3X_REG(C3X_FROM_INT(0)));

    // asm 00007071: 	LDF	1.0,R0
    // asm 00007072: 	STF	R0,*+AR0(OMATRIX+0)
    obj->omatrix.mat00 = C3X_STF(C3X_REG(C3X_FROM_INT(1)));
    // asm 00007073: 	STF	R0,*+AR0(OMATRIX+4)
    obj->omatrix.mat11 = C3X_STF(C3X_REG(C3X_FROM_INT(1)));
    // asm 00007074: 	STF	R0,*+AR0(OMATRIX+8)
    obj->omatrix.mat22 = C3X_STF(C3X_REG(C3X_FROM_INT(1)));

    // asm 00007075: 	CLRC
    // asm 00007076: 	POP	R0
    // asm 00007077: 	RETS
    return obj;

NOOBJ:
    // asm 00007078: 	ERRON	U,EC_OBJ|ET_ALLOC
    ERRON(EC_OBJ | ET_ALLOC);

    // asm 00007080: 	SETC
    // asm 00007081: 	POP	R0
    // asm 00007082: 	RETS
    return NULL;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *COMPUTE DISTANCE OF OBJECT
 *
 *PARAMETERS
 *	AR2	OBJECT
 *
 *RETURNS
 *	R0	ODIST
 *		STORES IN *+AR2(ODIST)
 */
static void GETDIST(OBJ* obj /*AR2*/) {
    c3x_reg_t posx;
    c3x_reg_t posy;
    c3x_reg_t posz;

    // asm 00007083: 	PUSH	R1
    // asm 00007084: 	PUSH	R2
    // asm 00007085: 	PUSHF	R1
    // asm 00007086: 	PUSHF	R2
    // asm 00007087: 	LDF	*+AR2(OPOSX),R0
    // asm 00007088: 	LDF	*+AR2(OPOSY),R1
    // asm 00007089: 	LDF	*+AR2(OPOSZ),R2
    posx = C3X_LDF(obj->pos.X);
    posy = C3X_LDF(obj->pos.Y);
    posz = C3X_LDF(obj->pos.Z);

    // asm 0000708A: 	LDP	@_CAMERAPOS
    // asm 0000708B: 	SUBF	@_CAMERAPOS,R0	  	;ADJUST FOR UNIVERSE RELATIVE
    // asm 0000708C: 	SUBF	@_CAMERAPOS+1,R1
    // asm 0000708D: 	SUBF	@_CAMERAPOS+2,R2
    posx = C3X_SUB(posx, _CAMERAPOS.X); // ;ADJUST FOR UNIVERSE RELATIVE
    posy = C3X_SUB(posy, _CAMERAPOS.Y);
    posz = C3X_SUB(posz, _CAMERAPOS.Z);

    // 	;MULTIPLY BY ROTATION MATRIX TO FIND Z TERM
    // asm 0000708E: 	MPYF	@_CAMERAMATRIX+6,R0	;LAST COLUMN ONLY NEEDED
    // asm 0000708F: 	MPYF	@_CAMERAMATRIX+7,R1
    // asm 00007090: 	MPYF	@_CAMERAMATRIX+8,R2
    posx = C3X_MUL(posx, _CAMERAMATRIX.a20); // ;LAST COLUMN ONLY NEEDED
    posy = C3X_MUL(posy, _CAMERAMATRIX.a21);
    posz = C3X_MUL(posz, _CAMERAMATRIX.a22);

    // asm 00007091: 	SETDP
    // asm 00007092: 	ADDF	R0,R1
    // asm 00007093: 	ADDF	R1,R2
    posy = C3X_ADD(posy, posx);
    posz = C3X_ADD(posz, posy);

    // asm 00007094: 	FIX	R2,R0
    // asm 00007095: 	STI	R0,*+AR2(ODIST)
    obj->dist = FIX(posz);

    // asm 00007096: 	POPF	R2
    // asm 00007097: 	POPF	R1
    // asm 00007098: 	POP	R2
    // asm 00007099: 	POP	R1
    // asm 0000709A: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INSERT PRIORITY OBJECT ON LIST
 *
 *PARAMETERS
 *	AR2	OBJECT
 *
 */
void OBJ_INSERTP(OBJ* obj /*AR2*/) {
    OBJ** list_link;
    OBJ* next_obj;
    int obj_dist;

    // asm 0000709B: 	PUSH	R0
    // asm 0000709C: 	PUSH	R1
    // asm 0000709D: 	PUSH	AR0
    // asm 0000709E: 	PUSH	AR1
    // asm 0000709F: 	CALL	GETDIST	 		;SETUP (ODIST) FIELD
    GETDIST(obj);
    // asm 000070A0: 	LDI	*+AR2(OFLAGS),R0	;OR IN PROPER FLAG
    // asm 000070A1: 	OR	O_LIST3,R0
    // asm 000070A2: 	STI	R0,*+AR2(OFLAGS)
    obj->flags |= O_LIST3;
    // asm 000070A3: 	LDI	@OACTIVE_PRIORITYI,AR1	;INSERT TO HEAD OF PROCESS ACTIVE LIST
    // asm 000070A4: 	LDI	*+AR2(ODIST),R0		;GET CURRENT Z COORD OF OBJECT
    list_link = &OACTIVE_PRIORITYI;
    obj_dist = obj->dist; // ;GET CURRENT Z COORD OF OBJECT
INSOBJLP:
    // asm 000070A5: 	LDI	AR1,AR0			;AR0 IS PREVIOUS LINK
    // asm 000070A6: 	LDI	*AR1,R1			;CHECK END OF LIST
    next_obj = *list_link;
    // asm 000070A7: 	BZ	INS_AT_ENDP
    if (next_obj == NULL) {
        goto INS_AT_ENDP;
    }
    // asm 000070A8: 	LDI	R1,AR1
    // asm 000070A9: 	CMPI	*+AR1(ODIST),R0
    // asm 000070AA: 	BLE	INSOBJLP     		;KEEP GOING DUDE
    if (next_obj->dist <= obj_dist) {
        list_link = &next_obj->link;
        goto INSOBJLP; // ;KEEP GOING DUDE
    }
INS_AT_ENDP:
    // asm 000070AB: 	STI	R1,*AR2			;LINK TO NEXT
    // asm 000070AC: 	STI	AR2,*AR0		;LINK FROM PREVIOUS
    obj->link = next_obj; // ;LINK TO NEXT
    *list_link = obj;     // ;LINK FROM PREVIOUS
    // asm 000070AD: 	POP	AR1
    // asm 000070AE: 	POP	AR0
    // asm 000070AF: 	POP	R1
    // asm 000070B0: 	POP	R0
    // asm 000070B1: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INSERT LOW PRIORITY OBJECT
 *
 *THIS IS NOT THE SAME AS OTHER LISTS, FOR THIS LIST THE OBJECT MUST
 *BE DELINKED AND FREED BY THE USER
 *LIST IS NOT SORTED
 *
 *PARAMETERS
 *	AR2	OBJECT TO LINK IN
 *
 *
 */
void OBJ_INSERTLP(void) {
    // asm 000070B2: 	PUSH	R0
    // asm 000070B3: 	LDI	@OLOW_PRIORITY,R0
    // asm 000070B4: 	STI	R0,*+AR2(OLINK)
    // asm 000070B5: 	STI	AR2,@OLOW_PRIORITY
    // asm 000070B6: 	POP	R0
    // asm 000070B7: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_INSERTLP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *THIS IS NOT THE SAME AS OTHER LISTS, FOR THIS LIST THE OBJECT MUST
 *BE DELINKED AND FREED BY THE USER
 *LIST IS NOT SORTED
 *
 *PARAMETERS
 *	AR2	OBJECT TO LINK IN
 *
 */
void OBJ_INSERTHP(void) {
    // asm 000070B8: 	PUSH	R0
    // asm 000070B9: 	LDI	@OHIGH_PRIORITY,R0
    // asm 000070BA: 	STI	R0,*+AR2(OLINK)
    // asm 000070BB: 	STI	AR2,@OHIGH_PRIORITY
    // asm 000070BC: 	POP	R0
    // asm 000070BD: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_INSERTHP", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *INSERT OBJECT ON OBJECT LIST
 *
 *PARAMETERS
 *	AR2	OBJECT
 *
 */
void OBJ_INSERT(OBJ* obj /*AR2*/) {
    OBJ** list_link;
    OBJ* next_obj;
    int obj_dist;
    const int active_lo = -5000;

    // asm 000070BE: 	PUSH	R0
    // asm 000070BF: 	PUSHF	R0
    // asm 000070C0: 	PUSH	R1
    // asm 000070C1: 	PUSH	AR0
    // asm 000070C2: 	PUSH	AR1
    // asm 000070C3: 	CALL	GETDIST	 		;SETUP (ODIST) FIELD
    GETDIST(obj);

    // asm 000070C4: 	CMPI	ACTIVELO,R0		;TOO FAR BEHIND?
    // asm 000070C5: 	BLT	INSIDLE			;YES
    if (obj->dist < active_lo) {
        goto INSIDLE; // ;YES
    }

    // ;	ASH	-2,R0
    // asm 000070C6: 	CMPI	@ACTIVEHI,R0		;TOO FAR AHEAD?
    // asm 000070C7: 	BLT	INSACT			;NO...
    if (obj->dist < ACTIVEHI) {
        goto INSACT; // ;NO...
    }

    // asm 000070C8: INSIDLE			    		;INSERT ON IDLE LIST
INSIDLE:
    // asm 000070C8: 	LDI	*+AR2(OFLAGS),R0	;OR in proper flag
    // asm 000070C9: 	OR	O_LIST2,R0
    // asm 000070CA: 	STI	R0,*+AR2(OFLAGS)
    obj->flags |= O_LIST2;

    // asm 000070CB: 	LDI	@IDLE_LISTI,AR1		;insert to head of process active list
    // asm 000070CC: 	BD	INSOBJX
    // asm 000070CD: 	LDI	*AR1,R0			;INSERT AT HEAD OF LIST
    // asm 000070CE: 	STI	R0,*AR2
    // asm 000070CF: 	STI	AR2,*AR1		;PUT IT IN FRONT OF IDLE LIST
    obj->link = IDLE_LIST;
    IDLE_LIST = obj; // ;PUT IT IN FRONT OF IDLE LIST

    // 	;------>BD	INSOBJX
    // asm 000070D0: INSACT					;INSERT ON ACTIVE LIST
    // asm 000070DD: 	POP	AR1
    // asm 000070DE: 	POP	AR0
    // asm 000070DF: 	POP	R1
    // asm 000070E0: 	POPF	R0
    // asm 000070E1: 	POP	R0
    // asm 000070E2: 	RETS
    return;

    // asm 000070D0: INSACT					;INSERT ON ACTIVE LIST
INSACT:
    // asm 000070D0: 	LDI	*+AR2(OFLAGS),R0	;OR in proper flag
    // asm 000070D1: 	OR	O_LIST1,R0
    // asm 000070D2: 	STI	R0,*+AR2(OFLAGS)
    obj->flags |= O_LIST1;

    // asm 000070D3: 	LDI	@OACTIVEI,AR1		;insert to head of process active list
    // asm 000070D4: 	LDI	*+AR2(ODIST),R0		;GET CURRENT Z COORD OF OBJECT
    list_link = &OACTIVE;
    obj_dist = obj->dist; // ;GET CURRENT Z COORD OF OBJECT

INSOBJL:
    // asm 000070D5: 	LDI	AR1,AR0			;AR0 IS PREVIOUS LINK
    // asm 000070D6: 	LDI	*AR1,R1			;CHECK END OF LIST
    next_obj = *list_link;

    // asm 000070D7: 	BZ	INS_AT_END
    if (next_obj == NULL) {
        goto INS_AT_END;
    }

    // asm 000070D8: 	LDI	R1,AR1
    // asm 000070D9: 	CMPI	*+AR1(ODIST),R0
    // asm 000070DA: 	BLT	INSOBJL	     		;KEEP GOING DUDE
    if (next_obj->dist < obj_dist) {
        list_link = &next_obj->link;
        goto INSOBJL; // ;KEEP GOING DUDE
    }

INS_AT_END:
    // asm 000070DB: 	STI	R1,*AR2			;LINK TO NEXT
    // asm 000070DC: 	STI	AR2,*AR0		;LINK FROM PREVIOUS
    obj->link = next_obj; // ;LINK TO NEXT
    *list_link = obj;     // ;LINK FROM PREVIOUS

INSOBJX:
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *
 *
 *
 *PARAMETERS
 *	AR2	ID
 *RETURNS
 *	CARRY CLR NOT FOUND
 *	CARRY SET FOUND
 *	AR0	FIRST OBJECT FOUND
 *
 */

OBJ* OBJ_FIND_FIRST_tail(OBJ* start, int oid) {
    OBJ* obj;

    // obj = OACTIVE;
    obj = start;
L89:
    // asm 000070E8: LDI	*AR0,R0
    if (obj == NULL) {
        goto FF_ERR;
    }
    // asm 000070E9: 	BZ	FF_ERR
FF_LP:
    // asm 000070EA: 	LDI	R0,AR0
    // asm 000070EB: 	CMPI	*+AR0(OID),AR2
    if (obj->id == (u32)oid) {
        goto FF_OK;
    }
    // asm 000070EC: 	BEQ	FF_OK
    // asm 000070ED: 	LDI	*AR0,R0
    obj = obj->link;
    // asm 000070EE: 	BNZ	FF_LP
    if (obj != NULL) {
        goto FF_LP;
    }
FF_ERR:
    // asm 000070EF: 	CLRC
    // asm 000070F0: 	POP	R0
    // asm 000070F1: 	RETS
    return NULL;
FF_OK:
    // asm 000070F2: 	SETC
    // asm 000070F3: 	POP	R0
    // asm 000070F4: 	RETS
    return obj;
}

OBJ* OBJ_FIND_FIRST_PRIORITY(int oid /*AR2*/) {
    // asm 000070E3: 	PUSH	R0
    // asm 000070E4: 	LDI	@OACTIVE_PRIORITYI,AR0
    // asm 000070E5: 	BU	L89
    return OBJ_FIND_FIRST_tail(OACTIVE_PRIORITYI, oid);
}

OBJ* OBJ_FIND_FIRST(int oid /*AR2*/) {
    // asm 000070E6: 	PUSH	R0
    // asm 000070E7: 	LDI	@OACTIVEI,AR0
    return OBJ_FIND_FIRST_tail(OACTIVE, oid);
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void OBJ_FREE_GROUND(OBJ* obj /*AR2*/) {
    // asm 000070F5: 	PUSH	R1
    // asm 000070F6: 	BUD	DELSLP
    // asm 000070F7: 	PUSH	AR1
    // asm 000070F8: 	LDI	@GROUND_LISTI,R1		;we must find dead object to link around
    // asm 000070F9: 	SUBI	OLINK3,R1
    // 	;---->	BUD	DELSLP
    OBJ_FREE_SUPPLEMENTAL(obj, &GROUND_LISTI, EC_OBJ | 1);
}

void OBJ_FREE_SIGN(OBJ* obj /*AR2*/) {
    // asm 000070FA: 	PUSH	R1
    // asm 000070FB: 	PUSH	AR1
    // asm 000070FC: 	LDI	@SIGN_LISTI,R1		;we must find dead object to link around
    // asm 000070FD: 	SUBI	OLINK3,R1
    OBJ_FREE_SUPPLEMENTAL(obj, &SIGN_LISTI, EC_OBJ | 1);
}

static void OBJ_FREE_SUPPLEMENTAL(OBJ* obj, OBJ** list_head, int error_code) {
    OBJ* current = *list_head;
    OBJ* previous = NULL;

DELSLP:
    // asm 000070FE: LDI	R1,AR1
    // asm 000070FF: 	LDI	*+AR1(OLINK3),R1
    // asm 00007100: 	ERRON	Z,EC_OBJ|1		;lockup on end of list found
    if (current == NULL) {
        ERRON(error_code);
        goto NOT_ON_SUPPXLIST;
    }
    // asm 00007108: 	BZ	NOT_ON_SUPPXLIST
    // asm 00007109: 	CMPI	R1,AR2
    // asm 0000710A: 	BNE	DELSLP
    if (current != obj) {
        previous = current;
        current = (OBJ*)current->link3;
        goto DELSLP;
    }
    // asm 0000710B: 	LDI	*+AR2(OLINK3),R1
    // asm 0000710C: 	STI	R1,*+AR1(OLINK3)	;LINK AROUND
    if (previous == NULL) {
        *list_head = (OBJ*)obj->link3;
    } else {
        previous->link3 = obj->link3;
    }
NOT_ON_SUPPXLIST:
    // asm 0000710D: 	POP	AR1
    // asm 0000710E: 	POP	R1
    // asm 0000710F: 	RETS
    ;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void OBJ_FREE_DRIVE(OBJ* obj /*AR2*/) {
    OBJ* current;
    OBJ* previous;

    // asm 00007110: 	PUSH	R1
    // asm 00007111: 	PUSH	AR1
    // asm 00007112: 	LDI	@DRIVE_LISTI,R1		;we must find dead object to link around
    current = DRIVE_LISTI;
    previous = NULL;
    // asm 00007113: 	SUBI	OLINK3,R1
DELRLP:
    // asm 00007114: LDI	R1,AR1
    // asm 00007115: 	LDI	*+AR1(OLINK3),R1
    // asm 00007116: 	ERRON	Z,EC_OBJ|2		;lockup on end of list found
    if (current == NULL) {
        ERRON(EC_OBJ | 2);
        goto NOT_ON_SUPPROADLIST;
    }
    // asm 0000711E: 	BZ	NOT_ON_SUPPROADLIST
    // asm 0000711F: 	CMPI	R1,AR2
    // asm 00007120: 	BNE	DELRLP
    if (current != obj) {
        previous = current;
        current = (OBJ*)current->link3;
        goto DELRLP;
    }
    // asm 00007121: 	LDI	*+AR2(OLINK3),R1
    // asm 00007122: 	STI	R1,*+AR1(OLINK3)	;LINK AROUND
    if (previous == NULL) {
        DRIVE_LISTI = (OBJ*)obj->link3;
    } else {
        previous->link3 = obj->link3;
    }
NOT_ON_SUPPROADLIST:
    // asm 00007123: 	POP	AR1
    // asm 00007124: 	POP	R1
    // asm 00007125: 	RETS
    ;
}

// *----------------------------------------------------------------------------

// *----------------------------------------------------------------------------
void OBJ_FREE_PROC(OBJ* obj /*AR2*/) {
    // asm 00007126: 	PUSH	AR2
    // asm 00007127: 	LDI	*+AR2(OPLINK),AR2
    // asm 00007128: 	CALL	PRC_KILL
    PRC_KILL(obj->plink);
    // asm 00007129: 	POP	AR2
    // asm 0000712A: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *DELETE AN OBJECT
 *
 *PARAMETERS
 *	AR2	OBJECT
 *
 *NOTE
 *	If there is any major change to this code
 *	please check DELETE_GROUP in BACKGRND.ASM
 *	this is a speeded up version of this subroutine.
 *
 */
void OBJ_DELETE(void) {
    // asm 0000712B: 	PUSH	R0
    // asm 0000712C: 	PUSH	R1
    // asm 0000712D: 	PUSH	AR1
    // asm 0000712E: 	PUSH	AR2
#if DEBUG
    // asm: 	LDI	0C5C5h,R0		;RANDOM KNOWN #
    // asm: 	STI	R0,*+AR2(OUSR1)
    // asm: 	LDI	0AB5Bh,R0		;RANDOM KNOWN #
    // asm: 	STI	R0,*+AR2(OLINK4)
#endif
    // 	;
    // 	;eliminate any dynamic objects associated with the object...
    // 	;
    // asm 0000712F: 	LDI	*+AR2(OFLAGS),R0
    // asm 00007130: 	TSTB	O_DYNAMIC,R0
    // asm 00007131: 	BZ	NOTDYNAMIC
    // asm 00007132: 	PUSH	AR2
    // asm 00007133: 	LDI	*+AR2(ODYNALIST),AR0
DYNDEL:
    // asm 00007134: LDI	*AR0,R0			;LINK TO NEXT
    // asm 00007135: 	LDI	AR0,AR2
    // asm 00007136: 	CALL	DELDYNA
    // asm 00007137: 	LDI	R0,AR0
    // asm 00007138: 	CMPI	0,AR0
    // asm 00007139: 	BNE	DYNDEL
    // asm 0000713A: 	POP	AR2
    // ;	PUSH	AR2
    // ;	LDI	*+AR2(ORADZ),AR2
    // ;	CALL	KILL
    // ;	POP	AR2
NOTDYNAMIC:
    // ;	.if	DEBUG
    // ;	LDI	*+AR2(OFLAGS),R0
    // ;	AND	O_LIST_M,R0
    // ;	LOCKON	Z
    // ;	BZ	$			;this object has invalid list field
    // ;					;note this may change if we have more
    // ;					;then 3 lists
    // ;	.endif
    // 	;Mirror these checks in BACKGRND.ASM
    // 	;	'DELETE_GROUP'
    // 	;
    // asm 0000713B: 	LDI	*+AR2(OFLAGS),R0	;KILL PROC ASSOCIATED WITH OBJ
    // asm 0000713C: 	RS	(O_DEBRIS_B+1),R0
    // asm 0000713D: 	CALLC	FREE_RDDEBRIS
    // asm 0000713E: 	LDI	*+AR2(OFLAGS),R0	;KILL PROC ASSOCIATED WITH OBJ
    // asm 0000713F: 	RS	(O_PROC_B+1),R0
    // asm 00007140: 	CALLC	OBJ_FREE_PROC
    // asm 00007141: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF DRIVE LIST
    // asm 00007142: 	RS	(O_DRIVE_SUPP_B+1),R0
    // asm 00007143: 	CALLC	OBJ_FREE_DRIVE
    // asm 00007144: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF GROUND LIST
    // asm 00007145: 	RS	(O_GROUND_B+1),R0
    // asm 00007146: 	CALLC	OBJ_FREE_GROUND
    // asm 00007147: 	LDI	*+AR2(OFLAGS),R0	;DELINK IN CASE OF SIGN LIST
    // asm 00007148: 	RS	(O_SIGN_SUPP_B+1),R0
    // asm 00007149: 	CALLC	OBJ_FREE_SIGN
    // 	;THE OBJECT CAN BE EITHER ON THE ACTIVE LIST, THE IDLE LIST,
    // 	;OR THE ACTIVE PRIORITY LIST.  ANYTHING ELSE IS AN ERROR.
    // 	;
    // asm 0000714A: 	LDI	*+AR2(OFLAGS),R0
    // asm 0000714B: 	AND	O_LIST_M,R0
    // asm 0000714C: 	LDI	@OACTIVEI,R1
    // asm 0000714D: 	CMPI	O_LIST2,R0
    // asm 0000714E: 	LDIEQ	@IDLE_LISTI,R1
    // asm 0000714F: 	CMPI	O_LIST3,R0
    // asm 00007150: 	LDIEQ	@OACTIVE_PRIORITYI,R1
DELLP:
    // asm 00007151: LDI	R1,AR1		;WE MUST FIND DEAD OBJECT TO LINK AROUND
    // asm 00007152: 	LDI	*AR1,R1
    // asm 00007153: 	ERRON	Z,EC_OBJ|ET_DELETE
    // asm 0000715B: 	BZ	DELOBJX
    // asm 0000715C: 	CMPI	R1,AR2
    // asm 0000715D: 	BNE	DELLP
    // asm 0000715E: 	LDI	*AR2,R1
    // asm 0000715F: 	STI	R1,*AR1			;LINK AROUND
    // asm 00007160: 	LDI	@OFREE,R1
    // asm 00007161: 	STI	R1,*AR2
    // asm 00007162: 	STI	AR2,@OFREE
    // asm 00007163: 	LDI	@OFREECNT,R0		;INCREMENT FREE OBJECT COUNT
    // asm 00007164: 	ADDI	1,R0
    // asm 00007165: 	STI	R0,@OFREECNT
    // asm 00007166: 	CLRI	R0
    // asm 00007167: 	STI	R0,*+AR2(OLINK2)	;CLEAR SEARCH ID
    // asm 00007168: 	STI	R0,*+AR2(OFLAGS)
DELOBJX:
    // asm 00007169: 	POP	AR2
    // asm 0000716A: 	POP	AR1
    // asm 0000716B: 	POP	R1
    // asm 0000716C: 	POP	R0
    // asm 0000716D: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_DELETE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *OBJ_DELETE_CLASS
 *
 *SEARCHES THE ACTIVE LIST FOR ALL OCCURANCES OF SPECIFIED ID
 *AND DELETES THAT OBJECT
 *
 *PARAMETERS
 *	R0	ID
 *	R1	MASK
 *
 */
void OBJ_DELETE_CLASS(void) {
    // asm 0000716E: 	PUSH	AR0
    // asm 0000716F: 	PUSH	AR2
    // asm 00007170: 	PUSH	R0
    // asm 00007171: 	PUSH	R1
    // asm 00007172: 	PUSH	R2
    // asm 00007173: 	PUSH	R3
    // asm 00007174: 	AND	R1,R0
    // asm 00007175: 	LDI	@OACTIVEI,AR0
ODC:
    // asm 00007176: LDI	*AR0,R3
    // asm 00007177: 	BZ	ODCX
    // asm 00007178: 	LDI	R3,AR0
ODCL2:
    // asm 00007179: LDI	*+AR0(OID),R2
    // asm 0000717A: 	AND	R1,R2
    // asm 0000717B: 	CMPI	R0,R2
    // asm 0000717C: 	BNE	ODC
    // asm 0000717D: 	LDI	*AR0,R3
    // asm 0000717E: 	LDI	AR0,AR2
    // asm 0000717F: 	CALL	OBJ_DELETE
    // asm 00007180: 	LDI	R3,R3
    // asm 00007181: 	LDI	R3,AR0
    // asm 00007182: 	BNZ	ODCL2
ODCX:
    // asm 00007183: 	POP	R3
    // asm 00007184: 	POP	R2
    // asm 00007185: 	POP	R1
    // asm 00007186: 	POP	R0
    // asm 00007187: 	POP	AR2
    // asm 00007188: 	POP	AR0
    // asm 00007189: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_DELETE_CLASS", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *PULL AN OBJECT FROM ACTIVE LIST
 *
 *Most of the same effect as delobj, except that the object is not
 *inserted onto the free list, allowing the user to do hold onto
 *the object.
 *
 *PARAMETERS
 *	AR2	OBJECT
 *
 */
void OBJ_PULL(OBJ* obj /*AR2*/) {
    OBJ** list_link;

    // asm 0000718A: 	PUSH	R0
    // asm 0000718B: 	PUSH	R1
    // asm 0000718C: 	PUSH	AR1
    // asm 0000718D: 	PUSH	AR2
    // asm 0000718E: 	LDI	*+AR2(OFLAGS),R0
    // asm 0000718F: 	AND	O_LIST_M,R0
    // asm 00007190: 	LDI	@OACTIVEI,R1		;we must find dead object to link around
    list_link = &OACTIVE; // ;we must find dead object to link around

    // asm 00007191: 	CMPI	O_LIST2,R0
    // asm 00007192: 	LDIEQ	@IDLE_LISTI,R1		;we must find dead object to link around
    if ((obj->flags & O_LIST_M) == O_LIST2) {
        list_link = &IDLE_LIST; // ;we must find dead object to link around
    }

    // asm 00007193: 	CMPI	O_LIST3,R0
    // asm 00007194: 	LDIEQ	@OACTIVE_PRIORITYI,R1	;we must find dead object to link around
    if ((obj->flags & O_LIST_M) == O_LIST3) {
        list_link = &OACTIVE_PRIORITY; // ;we must find dead object to link around
    }

PULLP:
    // asm 00007195: 	LDI	R1,AR1
    // asm 00007196: 	LDI	*AR1,R1
    // asm 00007197: 	ERRON	Z,EC_OBJ|4
    // asm 0000719F: 	BZ	PULOBJ_X
    if (*list_link == NULL) {
        ERRON(EC_OBJ | 4);
        goto PULOBJ_X;
    }

    // asm 000071A0: 	CMPI	R1,AR2
    // asm 000071A1: 	BNE	PULLP
    if (*list_link != obj) {
        list_link = &(*list_link)->link;
        goto PULLP;
    }

    // asm 000071A2: 	LDI	*AR2,R1
    // asm 000071A3: 	STI	R1,*AR1			;LINK AROUND
    *list_link = obj->link; // ;LINK AROUND

    // asm 000071A4: 	LDI	*+AR2(OFLAGS),R0
    // asm 000071A5: 	ANDN	O_LIST_M,R0
    // asm 000071A6: 	STI	R0,*+AR2(OFLAGS)
    obj->flags &= ~O_LIST_M;

PULOBJ_X:
    // asm 000071A7: 	POP	AR2
    // asm 000071A8: 	POP	AR1
    // asm 000071A9: 	POP	R1
    // asm 000071AA: 	POP	R0
    // asm 000071AB: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *LINK OBJECT INTO FREE LIST
 *(NO ERROR CHECKING)
 *
 *PARAMETERS
 *	AR2	OBJECT
 *
 */
void OBJ_FREE(OBJ* obj /*AR2*/) {
    // asm 000071AC: 	PUSH	R0
    // asm 000071AD: 	LDI	@OFREE,R0
    // asm 000071AE: 	STI	R0,*AR2
    // asm 000071AF: 	STI	AR2,@OFREE
    obj->link = OFREE;
    OFREE = obj;

    // asm 000071B0: 	POP	R0
    // asm 000071B1: 	RETS
    return;
}

// *----------------------------------------------------------------------------

/*
 * SORT OBJECTS BY CURRENT Z
 * WAIT FOR NEXT INTERRUPT
 *
 * CLOBBERS
 *      R0-R4,AR0-AR5,DP,etc.
 * CALL BY MAINLINE CODE
 */
void ZSORTWT(void) {
    CLEARRDY = 1; /* READY FOR INTERRUPT */

restart_sort: {
    int changed = 0; /* FLAG FOR SORT (NOTHING SORTED YET) */

    OBJ** prev_link = &OACTIVE;
    OBJ* a = *prev_link; /* GET FIRST ELEMENT */

    if (a == NULL) {
        goto done_sort; /* NULL LIST */
    }

    OBJ* b = a->link; /* GET NEXT ELEMENT */

    if (b == NULL) {
        goto done_sort; /* ONLY ONE ELEMENT ON LIST */
    }

    while (b != NULL) {
        if (CLEARRDY == 0) {
            goto done_sort; /* DONE WHEN INT SIGNALS FRAME IS DONE */
        }

        /*
         * PRIORITY IS O.K.  (a is further than b)
         */
        if (a->dist >= b->dist) {
            prev_link = &a->link;
            a = b;
            b = b->link;
            continue;
        }

        /*
         * SWAP EM DUDES
         */
        changed = 1; /* the list has changed */

        *prev_link = b; /* POINT N-1 TO N+1 */

        OBJ* next = b->link; /* GET N+2 */
        a->link = next;      /* POINT N TO N+2 */
        b->link = a;         /* POINT N+1 TO N */

        prev_link = &b->link; /* NEW PREVIOUS(N-1) */
        b = next;             /* NEW NEXT(N+1) */
    }

    if (!changed) {     /* ANY SWAPS? */
        goto done_sort; /* NO DONE... */
    }

    goto restart_sort; /* START OVER AT THE BEGINNING */
}

done_sort:
    if (_MODE & MHS) {
        return; /* Don't do player and drones in HIGH SCORE MODE */
    }

    PLYRDLINK(); /* DELINK PLAYER */
    DRONESORT();
    DEBRIS_SORT();
    PLYRSORT(); /* LINK IN PLAYER */
    SORT_SMOKE();
    FLAMESORT();
}

// *-----------------------------------------------------------------------------

/*
 *-----------------------------------------------------------------------------
 *DELINK THE PLAYER --> SO NO INTERFERENCE WITH DRONESORT
 *
 *PLYRDLINK
 *
 */
static void PLYRDLINK(void) {
    OBJ** prev_link = &OACTIVE; /* GET OBJECT LIST POINTER */
    OBJ* obj;

    while ((obj = *prev_link) != NULL) {
        if (obj->id == PLYR_C) {
            /* FOUND THE PLYR, DELINK 'EM */

            *prev_link = obj->link; /* LINK AROUND THE DUDE */
            PLYRTEMP = obj;         /* SAVE THE DUDE */
            return;
        }

        prev_link = &obj->link;
    }

    /* NOBODY HOME */
    PLYRTEMP = NULL;
}

// *-----------------------------------------------------------------------------

/*
 *-----------------------------------------------------------------------------
 *SORT PLYR CAR WITH DRONE CARS INTO ROAD WITH PRIORITY
 *
 *PLAYER INSERTED BEFORE DRONE WITH CLOSER ODIST
 *IF NO DRONE CLOSER, INSERT AT END OF LIST
 *
 */
static void PLYRSORT(void) {
    OBJ* player;
    OBJ* current;
    OBJ* next;
    OBJ* player_tail;
    OBJ** previous_link;
    OBJ** scan_link;
    int player_dist;
    c3x_reg_t player_x;
    c3x_reg_t x_distance;

    // asm 000071EA: 	LDI	@PLYRTEMP,R0		;GET PLAYER
    player = PLYRTEMP;
    // asm 000071EB: 	BZD	PSORTX	     		;NO PLAYER, HANG IT UP...
    if (player == NULL) {
        goto PSORTX;
    }
    // asm 000071EC: 	LDI	R0,AR5
    // asm 000071ED: 	LDI	*+AR5(ODIST),R2	     	;GET PLAYER DISTANCE
    player_dist = player->dist;
    // asm 000071EE: 	LDF	*+AR5(OPOSX),R3	     	;GET PLAYER X COORD
    player_x = C3X_LDF(player->pos.X);
    // 	;-----> BZD	PSORTX	       	;NO PLAYER, HANG IT UP...
    // asm 000071EF: 	LDI	0,R0
    // asm 000071F0: 	STI	R0,*AR5			;ZERO OUT PLAYERS LINK
    player->link = NULL;
    // asm 000071F1: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
    scan_link = &OACTIVE;
    // asm 000071F2: 	BR	PSRT1NXT
    goto PSRT1NXT;
PSRT1L:
    // asm 000071F3: 	AND	CLASS_M|TYPE_M,R1	;CHECK FOR A DRONE
    // asm 000071F4: 	CMPI	DRONE_C|VEHICLE_T,R1
    // asm 000071F5: 	BNE	PSRT1NXT
    if ((current->id & (CLASS_M | TYPE_M)) != (DRONE_C | VEHICLE_T)) {
        goto PSRT1NXT;
    }
    // *FOUND A DRONE, COMPARE ODIST
    // asm 000071F6: 	CMPI	*+AR1(ODIST),R2	     	;GET DRONE DISTANCE
    // asm 000071F7: 	BLT	PSRT1NXT		;DRONE IN BACK, IGNORE HIM
    if (player_dist < current->dist) {
        goto PSRT1NXT;
    }
    // asm 000071F8: 	LDF	*+AR1(OPOSX),R1
    // asm 000071F9: 	SUBF	R3,R1
    x_distance = C3X_SUB(C3X_LDF(current->pos.X), player_x);
    // asm 000071FA: 	ABSF	R1
    x_distance = C3X_ABS(x_distance);
    // asm 000071FB: 	FIX	R1
    // asm 000071FC: 	CMPI	2000,R1			;MUST BE WITHIN X LIMIT (NOT OFFSCREEN)
    // asm 000071FD: 	BGT	PSRT1NXT		;DRONE IN FRONT, WERE DONE
    if (FIX(x_distance) > 2000) {
        goto PSRT1NXT;
    }
    // asm 000071FE: 	LDI	AR5,AR2
    player_tail = player;
PSRT2A:
    // asm 000071FF: 	LDI	*AR2,R1			;DRONE IN FRONT, LINK EM INTO CHAIN
    // asm 00007200: 	BZ	PSRT2
    if (player_tail->link == NULL) {
        goto PSRT2;
    }
    // asm 00007201: 	LDI	R1,AR2
    player_tail = player_tail->link;
    // asm 00007202: 	BR	PSRT2A
    goto PSRT2A;
PSRT2:
    // asm 00007203: 	LDI	*AR1,R0			;REMOVE DRONE FORM OBJECT LIST
    next = current->link;
    // asm 00007204: 	STI	R0,*AR0
    *previous_link = next;
    // asm 00007205: 	STI	AR1,*AR2		;LINK DRONE TO TEMP PLAYER LIST
    player_tail->link = current;
    // asm 00007206: 	STI	R1,*AR1			;ZERO OUT LAST LINK
    current->link = NULL;
    // asm 00007207: 	LDI	AR0,AR1
    scan_link = previous_link;
PSRT1NXT:
    // asm 00007208: 	LDI	*AR1,R0
    current = *scan_link;
    // asm 00007209: 	BNZD	PSRT1L
    // asm 0000720A: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    previous_link = scan_link;
    // asm 0000720B: 	LDI	R0,AR1
    if (current != NULL) {
        scan_link = &current->link;
    }
    // asm 0000720C: 	LDI	*+AR1(OID),R1
    // 	;------>BNZD	PSRT1L
    if (current != NULL) {
        goto PSRT1L;
    }
    // *INSERT HIM
    // asm 0000720D: 	STI	AR5,*AR0		;LINK IN PLAYER CHAIN
    *previous_link = player;
PSORTX:
    // asm 0000720E: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "PLYRSORT", 0, 0);
}

// *-----------------------------------------------------------------------------

/*
 *-----------------------------------------------------------------------------
 *SORT DRONE CARS INTO ROAD WITH PRIORITY
 *
 *FIND CLOSEST ROAD PIECE DRONE IS ON TOP
 *THEN ZSORT DRONE CLOSER BASED ON ODIST
 *
 *USES
 *	AR4	POINTER TO DRONES
 *
 */
static void DRONESORT(void) {
    OBJ* drone_list;
    OBJ** list_link;
    OBJ* obj;
    OBJ* next;
    OBJ* drone;
    OBJ* next_drone;
    OBJ* road_objects[5];
    OBJ* target_road;
    OBJ* candidate;
    OBJ** insertion_link;
    CARBLK* carblk;
    int target_dist;
    int drone_dist;
    u32 candidate_id;

    // 	;PULL LIST OF DRONES OFF OBJECT LIST
    // asm 0000720F: 	BUD	DSORTNXT
    // asm 00007210: 	NOP
    // ;	PUSH	R2
    // asm 00007211: 	LDI	0,AR4			;INIT DRONE LIST HEADER
    drone_list = NULL;
    // asm 00007212: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
    list_link = &OACTIVE;
    // 	;------>BD DSORTNXT     	;GO GET FIRST ELEMENT
    goto DSORTNXT;
DSORTL:
    // asm 00007213: 	AND	CLASS_M,R1
    // asm 00007214: 	CMPI	DRONE_C,R1
    // asm 00007215: 	BNE	DSORTNXT
    if ((obj->id & CLASS_M) != DRONE_C) {
        list_link = &obj->link;
        goto DSORTNXT;
    }
    // *FOUND A DRONE, DELINK 'EM
    // asm 00007216: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
    next = obj->link;
    // asm 00007217: 	STI	R0,*AR0
    *list_link = next;
    // asm 00007218: 	STI	AR4,*AR1		;LINK HIM INTO TEMP LIST
    obj->link = drone_list;
    // asm 00007219: 	LDI	AR1,AR4
    drone_list = obj;
    // asm 0000721A: 	LDI	AR0,AR1
DSORTNXT:
    // asm 0000721B: 	LDI	*AR1,R0
    obj = *list_link;
    // asm 0000721C: 	BNZD	DSORTL
    // asm 0000721D: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    // asm 0000721E: 	LDI	R0,AR1
    // asm 0000721F: 	LDI	*+AR1(OID),R1
    // 	;------>BNZD	DSORTL
    if (obj != NULL) {
        goto DSORTL;
    }
    // ;	LDI	R0,R0
    // ;	BNZ	DSORTL
    // asm 00007220: DSORTX
    // *PROCESS DRONE LIST
    // *FOR EACH DRONE PUT IT AFTER HIGHEST PRIORITY ROAD SEG INTERSECTED
    // asm 00007220: 	LDI	AR4,R0	    		;NULL LIST?
    // asm 00007221: 	BZ	DSORTXX			;YES, QUIT
    if (drone_list == NULL) {
        goto DSORTXX;
    }
NXTDRONE:
    // *GET ROAD COLLISION POINTERS
    // asm 00007222: 	LDI	AR4,AR5
    drone = drone_list;
    // asm 00007223: 	LDI	*AR4,AR4		;MOVE TO NEXT DRONE
    next_drone = drone->link;
    // asm 00007224: 	PUSH	AR4
    // asm 00007225: 	PUSH	AR5
    // asm 00007226: 	LDI	*+AR5(OCARBLK),AR0
    carblk = drone->carblk;
    // asm 00007227: 	LDI	*+AR0(CARPCOL),AR1		;ROAD COLL CNT
    road_objects[0] = carblk != NULL ? OBJREF_TO_PTR(carblk->center.collided_road_object) : NULL;
    // asm 00007228: 	LDI	*+AR0(CARVSIZ+CARPCOL),AR2	;ROAD COLL RF
    road_objects[1] = carblk != NULL ? OBJREF_TO_PTR(carblk->right_front.collided_road_object) : NULL;
    // asm 00007229: 	LDI	*+AR0((2*CARVSIZ)+CARPCOL),AR3	;ROAD COLL LF
    road_objects[2] = carblk != NULL ? OBJREF_TO_PTR(carblk->left_front.collided_road_object) : NULL;
    // asm 0000722A: 	LDI	*+AR0((3*CARVSIZ)+CARPCOL),AR4	;ROAD COLL LR
    road_objects[3] = carblk != NULL ? OBJREF_TO_PTR(carblk->left_rear.collided_road_object) : NULL;
    // asm 0000722B: 	LDI	*+AR0((4*CARVSIZ)+CARPCOL),AR5	;ROAD COLL RR
    road_objects[4] = carblk != NULL ? OBJREF_TO_PTR(carblk->right_rear.collided_road_object) : NULL;
    // asm 0000722C: 	LDI	*+AR1(ODIST),R1
    // asm 0000722D: 	LDI	*+AR2(ODIST),R2
    // asm 0000722E: 	LDI	*+AR3(ODIST),R3
    // asm 0000722F: 	LDI	*+AR4(ODIST),R4
    // asm 00007230: 	LDI	*+AR5(ODIST),R5
    // asm 00007231: 	LDI	7FFFH,R6
    // asm 00007232: 	LSH	16,R6			;GET +INFINITY
    // asm 00007233: 	LDI	AR1,R0			;LOAD INFINITY INTO NULL POINTERS
    // asm 00007234: 	LDIZ	R6,R1
    // asm 00007235: 	LDI	AR2,R0
    // asm 00007236: 	LDIZ	R6,R2
    // asm 00007237: 	LDI	AR3,R0
    // asm 00007238: 	LDIZ	R6,R3
    // asm 00007239: 	LDI	AR4,R0
    // asm 0000723A: 	LDIZ	R6,R4
    // asm 0000723B: 	LDI	AR5,R0
    // asm 0000723C: 	LDIZ	R6,R5
    target_road = NULL;
    target_dist = 0x7fff0000;
    for (int road_index = 0; road_index < 5; ++road_index) {
        if (road_objects[road_index] != NULL && road_objects[road_index]->dist < target_dist) {
            target_road = road_objects[road_index];
            target_dist = road_objects[road_index]->dist;
        }
    }
    // asm 0000723D: 	CMPI	R1,R2
    // asm 0000723E: 	LDIGT	AR1,AR2
    // asm 0000723F: 	LDIGT	R1,R2
    // asm 00007240: 	CMPI	R2,R3
    // asm 00007241: 	LDIGT	AR2,AR3
    // asm 00007242: 	LDIGT	R2,R3
    // asm 00007243: 	CMPI	R3,R4
    // asm 00007244: 	LDIGT	AR3,AR4
    // asm 00007245: 	LDIGT	R3,R4
    // asm 00007246: 	CMPI	R4,R5
    // asm 00007247: 	LDIGT	AR4,AR5
    // asm 00007248: 	LDI	AR5,AR3
    // asm 00007249: 	POP	AR5
    // asm 0000724A: 	POP	AR4
    // asm 0000724B: 	LDI	@OACTIVEI,AR1		;GET OBJECT LIST POINTER
    insertion_link = &OACTIVE;
    // asm 0000724C: 	LDI	AR3,R3		 	;CHECK NULL
    // asm 0000724D: 	BZ	DSORTL1X	 	;NULL DUDE, Z SORT ONLY
    if (target_road == NULL) {
        goto DSORTL1X;
    }
DSORTL1:
    // asm 0000724E: 	LDI	*AR1,R0			;CHECK END OF LIST
    candidate = *insertion_link;
    // asm 0000724F: 	BNZ	DSORTL2
    if (candidate != NULL) {
        goto DSORTL2;
    }
DSORTL1X:
    // asm 00007250: 	LDI	@OACTIVEI,AR1		;CANT FIND TRACK PIECE, Z SORT FROM TOP
    insertion_link = &OACTIVE;
    // asm 00007251: 	LDI	AR1,R1
    // asm 00007252: 	BNZ	DZSORTUP		;LIST NOT NULL
    if (OACTIVE != NULL) {
        goto DZSORTUP;
    }
    // asm 00007253: 	LDI	*AR2,R2
    // asm 00007254: 	STI	R2,*AR3			;OBJECT LIST IS NULL, PUT CAR AT HEAD
    // asm 00007255: 	STI	AR3,*AR2
    drone->link = OACTIVE;
    OACTIVE = drone;
    goto DSORTLLL;
DSORTL2:
    // asm 00007256: 	LDI	R0,AR1
    // asm 00007257: 	CMPI	R0,R3
    // asm 00007258: 	BNZ	DSORTL1
    if (candidate != target_road) {
        insertion_link = &candidate->link;
        goto DSORTL1;
    }
    insertion_link = &candidate->link;
    // *
    // *SORT PAST SHOULDER PIECES
    // *AR1=OBJECT TO INSERT AFTER IN OBJECT LIST
    // *AR5=DRONE
    // *
DZSORTUP:
    // asm 00007259: 	LDI	*+AR0(CARPCOL),AR3     	;ON THE ROAD?
    // asm 0000725A: 	LDI	*+AR3(OID),R0
    // asm 0000725B: 	CMPI	0300H,R0
    // asm 0000725C: 	BNZ	DZSORTUP1		;NOPE, FORGET SHOULDER JIVE
    if (road_objects[0] == NULL || road_objects[0]->id != 0x0300) {
        goto DZSORTUP1;
    }
    // asm 0000725D: 	LDI	*+AR5(ODIST),R1	     	;GET DRONE DISTANCE
    drone_dist = drone->dist;
    // asm 0000725E: 	LDI	ROAD_C+SHLDR_T,R2	;GET SOULDER ID
    // asm 0000725F: 	LDI	CLASS_M,R3
    // asm 00007260: 	BU	DSL11
    goto DSL11;
DSLP1:
    // asm 00007261: 	BGT	DSL000	 		;ROAD OR CAR, CHECK IT OUT..
    if ((ROAD_C + SHLDR_T) > candidate_id) {
        goto DSL000;
    }
    // asm 00007262: 	AND	R3,R4,R5
    // asm 00007263: 	CMPI	TSIGN_C,R5		;SIGN OR TREE, NEED TO CHECK PRIORITY ?
    // asm 00007264: 	BNZ	DSL00			;NOPE, BLOW IT OFF...
    if ((candidate_id & CLASS_M) != TSIGN_C) {
        goto DSL00;
    }
DSL000:
    // asm 00007265: 	CMPI	*+AR2(ODIST),R1
    // asm 00007266: 	BGE	DSDONE	 		;PRIORITY IS O.K., WERE DONE WITH DRONE
    if (drone_dist >= candidate->dist) {
        goto DSDONE;
    }
DSL00:
    // asm 00007267: 	LDI	AR2,AR1
    insertion_link = &candidate->link;
DSL11:
    // asm 00007268: 	LDI	*AR1,R0
    candidate = *insertion_link;
    // asm 00007269: 	BNZD	DSLP1
    // asm 0000726A: 	LDI	R0,AR2
    // asm 0000726B: 	LDI	*+AR2(OID),R4
    if (candidate != NULL) {
        candidate_id = candidate->id;
    }
    // asm 0000726C: 	CMPI	R4,R2
    // 	;----->	BNZD	DSLP1
    if (candidate != NULL) {
        goto DSLP1;
    }
    // asm 0000726D: 	B	DSDONE
    goto DSDONE;
    // *Z SORT IT UPWARDS
    // *AR1=OBJECT TO INSERT AFTER IN OBJECT LIST
    // *AR5=DRONE
DZSORTUP1:
    // asm 0000726E: 	LDI	*+AR5(ODIST),R1	     	;GET DRONE DISTANCE
    drone_dist = drone->dist;
    // asm 0000726F: 	BU	DSL1
    goto DSL1;
DSLP:
    // asm 00007270: 	BGE	DSDONE	 		;PRIORITY IS O.K., WERE DONE WITH DRONE
    if (drone_dist >= candidate->dist) {
        goto DSDONE;
    }
    // asm 00007271: 	LDI	AR2,AR1
    insertion_link = &candidate->link;
DSL1:
    // asm 00007272: 	LDI	*AR1,R0
    candidate = *insertion_link;
    // asm 00007273: 	BNZD	DSLP
    // asm 00007274: 	LDI	R0,AR2
    // asm 00007275: 	CMPI	*+AR2(ODIST),R1
    // asm 00007276: 	NOP
    // 	;----->	BNZD	DSLP
    if (candidate != NULL) {
        goto DSLP;
    }
DSDONE:
    // asm 00007277:         LDI	*AR1,R0			;WERE DONE... INSERT THE SUCKA
    next = *insertion_link;
    // asm 00007278: 	STI	AR5,*AR1
    *insertion_link = drone;
    // asm 00007279: 	STI	R0,*AR5
    drone->link = next;
    // *GET NEXT DRONE DUDES
DSORTLLL:
    // asm 0000727A: DSORTLLL
    // asm 0000727A:        	LDI	AR4,R0
    // asm 0000727B: 	BNZ	NXTDRONE
    drone_list = next_drone;
    if (drone_list != NULL) {
        goto NXTDRONE;
    }
DSORTXX:
    // asm 0000727C: 	RETS				;WE QUIT...
    TRACE_EVENT(&g_crusn_machine->trace, "function", "DRONESORT", 0, 0);
}

// *----------------------------------------------------------------------------

/* asm: ACTIVEHI1	.word	75000		;HI LIMIT FOR INACTIVE-ACTIVE */
static int ACTIVEHI1 = 75000;
/* asm: ACTIVEHI	.word	80000		;HI LIMIT FOR ACTIVE-INACTIVE */
static int ACTIVEHI = 80000;
#define ACTIVELO -5000 // LO LIMIT INACTIVE OBJECT LIST

/*
 *----------------------------------------------------------------------------
 *SCAN ACTIVE LIST
 *PUT DISTANT OBJECTS ON INACTIVE LIST
 *
 */
void OSCAN(void) {
    OBJ** prev_link;
    OBJ* obj;
    OBJ* next_obj;
    int obj_dist;

    // asm 0000727F: 	BUD	OSCANNXT
    // asm 00007280: 	LDI	@ACTIVEHI,R4 		;GET FAR LIMIT
    // asm 00007281: 	LDI	@IDLE_LISTI,AR5		;IN SAME PAGE
    // asm 00007282: 	LDI	@OACTIVEI,AR1
    // 	;------>BD OSCANNXT     	;GO GET FIRST ELEMENT
    prev_link = &OACTIVE;
    obj = *prev_link;
    while (obj != NULL) {
    OSCANL:
        // asm 00007283: 	LDI	*+AR1(ODIST),R0	    	;ODIST TOO NEGATIVE?
        obj_dist = obj->dist; // ;ODIST TOO NEGATIVE?
                              // asm 00007284: 	CMPI	ACTIVELO,R0
        if (obj_dist <= (int)ACTIVELO) {
            goto OSCANACT; // ;YES, BLOW IT OUT
        }
        // asm 00007285: 	BLE	OSCANACT		;YES, BLOW IT OUT
        // asm 00007286: 	SUBI	R4,R0			;ODIST TOO POSITIVE
        obj_dist -= ACTIVEHI; // ;ODIST TOO POSITIVE
                              // asm 00007287: 	BLE	OSCANNXT		;NO...
        if (obj_dist > 0) {
            // asm 00007288: 	SUBI	*+AR1(ORAD),R0		;CHECK RADIUS TO MAKE SURE
            obj_dist -= obj->radius; // ;CHECK RADIUS TO MAKE SURE
            // asm 00007289: 	BLE	OSCANNXT
            if (obj_dist > 0) {
                // asm 0000728A: 	NOP
                // *FOUND DISTANT ELEMENT, XSFER ACTIVE TO INACTIVE
            OSCANACT:
                // asm 0000728B: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
                next_obj = obj->link; // ;GET POINTER TO NEXT ELEMENT
                // asm 0000728C: 	STI	R0,*AR6
                *prev_link = next_obj;
                // asm 0000728D: 	LDI	*+AR1(OFLAGS),R0	;SWITCH LIST FLAG
                // asm 0000728E: 	XOR	O_LIST2+O_LIST1,R0
                // asm 0000728F: 	STI	R0,*+AR1(OFLAGS)
                obj->flags ^= O_LIST2 + O_LIST1; // ;SWITCH LIST FLAG
                // asm 00007290: 	LDI	*AR5,R0
                // asm 00007291: 	STI	R0,*AR1			;LINK HIM INTO INACTIVE LIST
                obj->link = IDLE_LIST; // ;LINK HIM INTO INACTIVE LIST
                // asm 00007292: 	STI	AR1,*AR5
                IDLE_LIST = obj;
                // asm 00007293: 	LDI	AR6,AR1
                obj = next_obj;
                continue;
            }
        }
    OSCANNXT:
        // asm 00007294: 	LDI	*AR1,R0
        next_obj = obj->link;
        // asm 00007295: 	BNZD	OSCANL
        if (next_obj != NULL) {
            // asm 00007296: 	LDI	AR1,AR6			;AR6=PREVIOUS-1 LINK
            prev_link = &obj->link; // ;AR6=PREVIOUS-1 LINK
            // asm 00007297: 	LDI	R0,AR1
            obj = next_obj;
            // asm 00007298: 	NOP
            continue;
        }
        // 	;------>BNZD	OSCANL
        // asm 00007299: 	RETS
        break;
    }
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *SCAN INACTIVE LIST FOR ACTIVES
 *PUT DISTANT OBJECTS ON INACTIVE LIST
 *IF RADIUS IS CLOSE, ACTIVATE
 *IF ANGLE COSINE POSITIVE (+-90 DEGREES), ACTIVATE
 *
 */
void ISCAN(void) {
    OBJ** prev_link;
    OBJ* obj;
    OBJ* next_obj;
    c3x_reg_t obj_vector_x;
    c3x_reg_t obj_vector_y;
    c3x_reg_t obj_vector_z;
    c3x_reg_t projected_dist;
    int obj_dist;

    // asm 0000729A: 	FLOAT	ACTIVELO,R3		;GET CLOSE LIMIT
    // asm 0000729B: 	LDI	@ACTIVEHI1,R4 		;GET FAR LIMIT
    // asm 0000729C: 	LDI	@CAMERAPOSI,AR4		;POINTER TO CAMERA STRUCT
    // asm 0000729D: 	LDI	@IDLE_LISTI,AR1		;IN SAME PAGE
    // asm 0000729E: 	LDI	@OACTIVEI,AR5
    // asm 0000729F: 	LDI	@CAMERAMATRIXI,AR3
    // asm 000072A0: 	ADDI	1,AR4 			;POINT TO Y ELEMENT
    // asm 000072A1: 	BUD	ISCANNXT
    // asm 000072A2: 	ADDI	7,AR3			;LAST COLUMN OF MATRIX
    // asm 000072A3: 	LDI	OPOSY,IR0
    // asm 000072A4: 	LDI	OPOSZ,IR1
    // 	;------>BD ISCANNXT     	;GO GET FIRST ELEMENT
    prev_link = &IDLE_LIST;
    obj = *prev_link;
    while (obj != NULL) {
        // asm 000072C1: 	SUBF	*-AR4(1),*+AR1(OPOSX),R5   	;GET LENGTH OF OBJ VECTOR
        obj_vector_x = C3X_SUB(obj->pos.X, _CAMERAPOS.X); // ;GET LENGTH OF OBJ VECTOR
    ISCANL:
        // asm 000072A5: 	SUBF	*AR4,*+AR1(IR0),R6	;OYPOS-CAMERAPOSY
        obj_vector_y = C3X_SUB(obj->pos.Y, _CAMERAPOS.Y); // ;OYPOS-CAMERAPOSY
                                                 // asm 000072A6: 	SUBF	*+AR4(1),*+AR1(IR1),R7	;OZPOS-CAMERAPOSZ
        obj_vector_z = C3X_SUB(obj->pos.Z, _CAMERAPOS.Z); // ;OZPOS-CAMERAPOSZ
                                                 // asm 000072A7: 	MPYF    *-AR3(1),R5,R0
        projected_dist = C3X_MUL(_CAMERAMATRIX.a20, obj_vector_x);
        // asm 000072A8: 	MPYF    *AR3,R6,R1
        projected_dist = C3X_ADD(projected_dist, C3X_MUL(_CAMERAMATRIX.a21, obj_vector_y));
        // asm 000072A9: 	MPYF    *+AR3(1),R7,R2
        projected_dist = C3X_ADD(projected_dist, C3X_MUL(_CAMERAMATRIX.a22, obj_vector_z));
        // asm 000072AA: 	ADDF	R1,R0
        // asm 000072AB: 	ADDF	R2,R0
        // asm 000072AC: 	CMPF	R3,R0
        if (C3X_GT(projected_dist, C3X_FROM_INT(ACTIVELO))) {
            // asm 000072AD: 	BLED	ISCANNXT
            // asm 000072AE: 	FIX	R0,R1
            obj_dist = FIX(projected_dist);
            // asm 000072AF: 	STI	R1,*+AR1(ODIST)		;SETUP ODIST
            obj->dist = obj_dist; // ;SETUP ODIST
            // asm 000072B0: 	NOP
            // 	;------>BLED	ISCANNXT
            // asm 000072B1: 	SUBI	R4,R1
            obj_dist -= ACTIVEHI1;
            // asm 000072B2: 	SUBI	*+AR1(ORAD),R1		;CHECK RADIUS TO MAKE SURE
            obj_dist -= obj->radius; // ;CHECK RADIUS TO MAKE SURE
            // asm 000072B3: 	BGT	ISCANNXT
            if (obj_dist <= 0) {
                // *FOUND CLOSE ELEMENT, XSFER INACTIVE TO ACTIVE
                // asm 000072B4: ISCANACT
                // asm 000072B4: 	LDI	*AR1,R0			;GET POINTER TO NEXT ELEMENT
                next_obj = obj->link; // ;GET POINTER TO NEXT ELEMENT
                // asm 000072B5: 	STI	R0,*AR6
                *prev_link = next_obj;
                // asm 000072B6: 	LDI	*+AR1(OFLAGS),R0	   	;SWITCH LIST FLAG
                // asm 000072B7: 	XOR	O_LIST2+O_LIST1,R0
                // asm 000072B8: 	STI	R0,*+AR1(OFLAGS)
                obj->flags ^= O_LIST2 + O_LIST1;
                // asm 000072B9: 	LDI	*AR5,R0
                // asm 000072BA: 	STI	R0,*AR1			;LINK HIM INTO INACTIVE LIST
                obj->link = OACTIVE; // ;LINK HIM INTO INACTIVE LIST
                // asm 000072BB: 	STI	AR1,*AR5
                OACTIVE = obj;
                // asm 000072BC: 	LDI	AR6,AR1
                obj = next_obj;
                continue;
            }
        }
    ISCANNXT:
        // asm 000072BD: 	LDI	*AR1,R0
        next_obj = obj->link;
        // asm 000072BE: 	BNZD	ISCANL
        if (next_obj == NULL) {
            break;
        }
        // asm 000072BF: 	LDI	AR1,AR6			;AR6=PREVIOUS-1 LINK
        prev_link = &obj->link; // ;AR6=PREVIOUS-1 LINK
                                // asm 000072C0: 	LDI	R0,AR1
        obj = next_obj;
        // 	;------>BNZD	ISCANL
        // asm 000072C2: 	RETS
    }
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *RESCAN	TOTALLY RESCAN ALL OBJECTS ACTIVE AND INACTIVE
 *	FOR CAMERA VIEW CHANGE
 *
 *PARAMETERS
 *	CAMERAPOS AND CAMERAMATRIX WITH NEW VALUES
 *
 */
void RESCAN(void) {
    OBJ* obj;
    OBJ* active_list;

    // asm 000072C3: 	PUSH	AR3
    // asm 000072C4: 	PUSH	AR4
    // asm 000072C5: 	PUSH	AR5
    // asm 000072C6: 	PUSH	AR6
    // *COMBINE ACTIVE INACTIVE LISTS
    // asm 000072C7: 	LDI	@IDLE_LISTI,AR5
    // asm 000072C8: 	LDI	@OACTIVEI,AR1
    // asm 000072C9: 	LDI	*AR1,R0
    active_list = OACTIVE;
    obj = active_list;
    // asm 000072CA: 	BZ	RESCAN1			;ACTIVE LIST NULL, FORGET IT
    if (obj != NULL) {
    RESCAN0:
        // asm 000072CB: 	LDI	R0,AR2
        // asm 000072CC: 	LDI	*+AR2(OFLAGS),R0       	;SWITCH LIST FLAG
        // asm 000072CD: 	XOR	O_LIST2+O_LIST1,R0
        // asm 000072CE: 	STI	R0,*+AR2(OFLAGS)
        obj->flags ^= O_LIST2 + O_LIST1; // ;SWITCH LIST FLAG
        // asm 000072CF: 	LDI	*AR2,R0
        // asm 000072D0: 	BNZ	RESCAN0
        if (obj->link != NULL) {
            obj = obj->link;
            goto RESCAN0;
        }
        // asm 000072D1: 	LDI	*AR5,R0			;FIRST ELEMENT INACTIVE LIST
        // asm 000072D2: 	STI	R0,*AR2			;LINK TO LAST ELEMENT OF ACTIVE LIST
        obj->link = IDLE_LIST; // ;LINK TO LAST ELEMENT OF ACTIVE LIST
        // asm 000072D3: 	LDI	*AR1,R0
        // asm 000072D4: 	STI	R0,*AR5			;POINT INACTIVE LIST TO ACTIVE LIST
        IDLE_LIST = active_list; // ;POINT INACTIVE LIST TO ACTIVE LIST
        // asm 000072D5: 	LDI	0,R0   			;CLEAR OUT ACTIVE LIST
        // asm 000072D6: 	STI	R0,*AR1
        OACTIVE = NULL;
    }
RESCAN1:
    // asm 000072D7: 	CALL	ISCAN			;FORM NEW ACTIVE INACTIVE LISTS
    ISCAN(); // ;FORM NEW ACTIVE INACTIVE LISTS
    // asm 000072D8: 	CALL	ZSORTACT		;BUBBLE SORT ACTIVE LIST
    ZSORTACT(); // ;BUBBLE SORT ACTIVE LIST
    // asm 000072D9: 	POP	AR6
    // asm 000072DA: 	POP	AR5
    // asm 000072DB: 	POP	AR4
    // asm 000072DC: 	POP	AR3
    // asm 000072DD: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *BUBBLE SORT PRIORITY LIST UNTIL DONE
 *
 */
void ZSORTPRIOR(void) {
ZSORTA1P:
    // asm 000072DE: 	LDI	0,R2			;CLEAR EXCHANGE FLAG
    // asm 000072DF: 	LDI	@OACTIVE_PRIORITYI,AR0
    // asm 000072E0: 	LDI	*AR0,AR1		;GET FIRST ELEMENT
    // asm 000072E1: 	LDI	AR1,R1
    // asm 000072E2: 	BZ	ZSORTXP		     	;NULL LIST
    // asm 000072E3: 	LDI	*AR1,AR2		;GET NEXT ELEMENT
    // asm 000072E4: 	LDI	AR2,R1
    // asm 000072E5: 	BZ	ZSORTXP			;ONLY ONE ELEMENT ON LIST
    // asm 000072E6: 	LDI	*+AR1(ODIST),R0
ZSLPP:
    // asm 000072E7: 	LDI	*+AR2(ODIST),R1
    // asm 000072E8: 	CMPI	R1,R0
    // asm 000072E9: 	BGE	PRIOKP	 		;PRIORITY IS O.K.
    // *SWAP EM DUDES
    // asm 000072EA: 	LDI	1,R2
    // asm 000072EB: 	STI	AR2,*AR0		;POINT N-1 TO N+1
    // asm 000072EC: 	LDI	*AR2,R1			;GET N+2
    // asm 000072ED: 	STI	R1,*AR1			;POINT N TO N+2
    // asm 000072EE: 	STI	AR1,*AR2		;POINT N+1 TO N
    // asm 000072EF: 	LDI	AR2,AR0		  	;NEW PREVIOUS(N-1)
    // asm 000072F0: 	LDI	R1,AR2			;NEW NEXT(N+1)
    // asm 000072F1: 	LDI	R1,R1
    // asm 000072F2: 	BNZ	ZSLPP
    // asm 000072F3: 	BR	ZSORTXP
PRIOKP:
    // asm 000072F4: 	LDI	R1,R0
    // asm 000072F5: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    // asm 000072F6: 	LDI	AR2,AR1			;AR0=PREVIOUS
    // asm 000072F7: 	LDI	*AR2,R1
    // asm 000072F8: 	LDI	R1,AR2
    // asm 000072F9: 	BNZ	ZSLPP
ZSORTXP:
    // asm 000072FA: LDI	R2,R2
    // asm 000072FB: 	BNZ	ZSORTA1P
    // asm 000072FC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "ZSORTPRIOR", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *BUBBLE SORT ACTIVE LIST UNTIL DONE
 *
 */
static void ZSORTACT(void) {
    int swapped;
    OBJ** prev_link;
    OBJ* current;
    OBJ* next;

ZSORTA1:
    // asm 000072FD: 	LDI	0,R2			;CLEAR EXCHANGE FLAG
    swapped = 0; // ;CLEAR EXCHANGE FLAG
    // asm 000072FE: 	LDI	@OACTIVEI,AR0
    // asm 000072FF: 	LDI	*AR0,AR1		;GET FIRST ELEMENT
    current = OACTIVE; // ;GET FIRST ELEMENT
    // asm 00007300: 	LDI	AR1,R1
    // asm 00007301: 	BZ	ZSORTX		     	;NULL LIST
    if (current == NULL) {
        goto ZSORTX; // ;NULL LIST
    }
    // asm 00007302: 	LDI	*AR1,AR2		;GET NEXT ELEMENT
    next = current->link; // ;GET NEXT ELEMENT
    // asm 00007303: 	LDI	AR2,R1
    // asm 00007304: 	BZ	ZSORTX			;ONLY ONE ELEMENT ON LIST
    if (next == NULL) {
        goto ZSORTX; // ;ONLY ONE ELEMENT ON LIST
    }
    // asm 00007305: 	LDI	*+AR1(ODIST),R0
    prev_link = &OACTIVE;
ZSLP:
    // asm 00007306: 	LDI	*+AR2(ODIST),R1
    // asm 00007307: 	CMPI	R1,R0
    // asm 00007308: 	BGE	PRIOK	 		;PRIORITY IS O.K.
    if (next->dist < current->dist) {
        // *SWAP EM DUDES
        // asm 00007309: 	LDI	1,R2
        swapped = 1;
        // asm 0000730A: 	STI	AR2,*AR0		;POINT N-1 TO N+1
        *prev_link = next;          // ;POINT N-1 TO N+1
                                    // asm 0000730B: 	LDI	*AR2,R1			;GET N+2
        current->link = next->link; // ;GET N+2
                                    // asm 0000730C: 	STI	R1,*AR1			;POINT N TO N+2
                                    // asm 0000730D: 	STI	AR1,*AR2		;POINT N+1 TO N
        next->link = current;       // ;POINT N+1 TO N
                                    // asm 0000730E: 	LDI	AR2,AR0		  	;NEW PREVIOUS(N-1)
        prev_link = &next->link;    // ;NEW PREVIOUS(N-1)
                                    // asm 0000730F: 	LDI	R1,AR2			;NEW NEXT(N+1)
        next = current->link;       // ;NEW NEXT(N+1)
                                    // asm 00007310: 	LDI	R1,R1
                                    // asm 00007311: 	BNZ	ZSLP
        if (next != NULL) {
            goto ZSLP;
        }
        // asm 00007312: 	BR	ZSORTX
        goto ZSORTX;
    }
PRIOK:
    // asm 00007313: 	LDI	R1,R0
    // asm 00007314: 	LDI	AR1,AR0			;AR4=PREVIOUS-1 LINK
    prev_link = &current->link; // ;AR4=PREVIOUS-1 LINK
    // asm 00007315: 	LDI	AR2,AR1			;AR0=PREVIOUS
    current = next; // ;AR0=PREVIOUS
    // asm 00007316: 	LDI	*AR2,R1
    next = next->link;
    // asm 00007317: 	LDI	R1,AR2
    // asm 00007318: 	BNZ	ZSLP
    if (next != NULL) {
        goto ZSLP;
    }
ZSORTX:
    // asm 00007319: 	LDI	R2,R2
    if (swapped != 0) {
        goto ZSORTA1;
    }
    // asm 0000731A: 	BNZ	ZSORTA1
    // asm 0000731B: 	RETS
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *OBJECTP	make_obj(int *rom,int px,int py,int pz,float rx,float ry)
 *
 *PARAMETERS
 *	AR2	ROMDATA					  ^
 *	RC	POS X					  |
 *	RS	POS Y					ignored
 *	RE	POS Z
 *	R2	RAD Y
 *RETURNS
 *	R0	OBJECT POINTER
 *
 *
 */
void OBJ_MAKE(void) {
    // asm 0000731C: 	CALL	OBJ_GET
    // asm 0000731D: 	ERRON	C,EC_OBJ|5
    // asm 00007325: 	RETSC
    // asm 00007326: 	STI	AR2,*+AR0(OROMDATA)
    // asm 00007327: 	STF	R3,*+AR0(ORADY)
    // asm 00007328: 	FLOAT	RC,R2
    // asm 00007329: 	STF	R2,*+AR0(OPOSX)
    // asm 0000732A: 	FLOAT	RS,R2
    // asm 0000732B: 	STF	R2,*+AR0(OPOSY)
    // asm 0000732C: 	FLOAT	RE,R2
    // asm 0000732D: 	STF	R2,*+AR0(OPOSZ)
    // asm 0000732E: 	LDF	*+AR0(ORADY),R2
    // asm 0000732F: 	LDI	AR0,AR2
    // asm 00007330: 	ADDI	OMATRIX,AR2
    // asm 00007331: 	CALL	FIND_YMATRIX
    // asm 00007332: 	LDI	AR0,R0
    // asm 00007333: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OBJ_MAKE", 0, 0);
    UNIMPL();
}

// *----------------------------------------------------------------------------

/*
 *----------------------------------------------------------------------------
 *QWICK MAKE OBJ
 *
 *PARAMETERS
 *	AR2	ROM POINTER
 *	R2	POSX
 *	R3	POSY
 *	RC	POSZ
 *
 *RETURNS
 *	AR0	OBJECT
 *
 */
OBJ* OBJ_QMAKE(void* romdata /*AR2*/, int posx /*R2*/, int posy /*R3*/, int posz /*RC*/) {
    OBJ* obj;

    // asm 00007334: 	CALL	OBJ_GET
    obj = OBJ_GET();
    // asm 00007335: 	RETSC
    if (obj == NULL) {
        return NULL;
    }
    // asm 00007336: 	STI	AR2,*+AR0(OROMDATA)
    obj->romdata = romdata;
    // asm 00007337: 	FLOAT	R2
    // asm 00007338: 	STF	R2,*+AR0(OPOSX)
    obj->pos.X = C3X_STF(C3X_REG(C3X_FROM_INT(posx)));
    // asm 00007339: 	FLOAT	R3
    // asm 0000733A: 	STF	R3,*+AR0(OPOSY)
    obj->pos.Y = C3X_STF(C3X_REG(C3X_FROM_INT(posy)));
    // asm 0000733B: 	FLOAT	RC,R2
    // asm 0000733C: 	STF	R2,*+AR0(OPOSZ)
    obj->pos.Z = C3X_STF(C3X_REG(C3X_FROM_INT(posz)));
    // asm 0000733D: 	CLRC
    // asm 0000733E: 	RETS
    return obj;
}

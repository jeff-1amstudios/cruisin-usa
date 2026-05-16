#include "NOT_IMPLEMENTED.h"
#include "TRACE.h"
#include "STUB.h"
#include "OBJ.h"

/* Scaffold generated from OBJ.ASM */

#define NUM_OBJECTS 256
#define OBJSIZ_WORDS 64

static int OFREECNT;
static int OACTIVE;
static int IDLE_LIST;
static int OACTIVE_PRIORITY;
static int OACTIVECNT;
static int OMAX_OBJECTS;
static int OLOW_PRIORITY;
static int OHIGH_PRIORITY;
static int DRIVE_LIST;
static int CAR_LIST;
static int SIGN_LIST;
static int GROUND_LIST;
static int COMM_DRONE_PTR;

static int OBJSTR[NUM_OBJECTS * OBJSIZ_WORDS];
static int OFREE_HEAD;
static int OBJ_CUR;

void OBJ_INIT(void) {
    TRACE();
    OFREECNT = NUM_OBJECTS;

    OACTIVE = 0;
    IDLE_LIST = 0;
    OACTIVE_PRIORITY = 0;
    OACTIVECNT = 0;
    OMAX_OBJECTS = 0;
    OLOW_PRIORITY = 0;
    OHIGH_PRIORITY = 0;
    DRIVE_LIST = 0;
    CAR_LIST = 0;
    SIGN_LIST = 0;
    GROUND_LIST = 0;
    COMM_DRONE_PTR = 0;

    /* Build OFREE single-linked list in object-store index space. */
    OFREE_HEAD = 0;
    for (int i = 0; i < NUM_OBJECTS - 1; i++) {
        OBJSTR[i * OBJSIZ_WORDS] = (i + 1) * OBJSIZ_WORDS;
    }
    OBJSTR[(NUM_OBJECTS - 1) * OBJSIZ_WORDS] = 0;
}

void OBJ_GETE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_GET(void) {
    TRACE();
    if (OFREE_HEAD == 0 && OFREECNT == 0) {
        NOT_IMPLEMENTED();
    }

    OBJ_CUR = OFREE_HEAD;
    if (OBJ_CUR < 0 || OBJ_CUR >= (NUM_OBJECTS * OBJSIZ_WORDS)) {
        NOT_IMPLEMENTED();
    }

    OFREE_HEAD = OBJSTR[OBJ_CUR];
    OFREECNT -= 1;

    for (int i = 0; i < OBJSIZ_WORDS; i++) {
        OBJSTR[OBJ_CUR + i] = 0;
    }

    /* Identity-like default for matrix slots in this simplified object layout. */
    OBJSTR[OBJ_CUR + 0] = 1;
    OBJSTR[OBJ_CUR + 1] = 1;
    OBJSTR[OBJ_CUR + 2] = 1;
}

void GETDIST(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_INSERTP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_INSERTLP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_INSERTHP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_INSERT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_FIND_FIRST_PRIORITY(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_FIND_FIRST(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_FREE_GROUND(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_FREE_SIGN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_FREE_DRIVE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_FREE_PROC(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_DELETE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_DELETE_CLASS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_PULL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_FREE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ZSORTWT(void) {
    TRACE();
    STUB();
}

void PLYRDLINK(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PLYRSORT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void DRONESORT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OSCAN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ISCAN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RESCAN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ZSORTPRIOR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ZSORTACT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_MAKE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void OBJ_QMAKE(void) {
    TRACE();
    /* OBJ.ASM: OBJ_GET; if fail return; else write ROM/X/Y/Z. */
    OBJ_GET();
    /* Pending exact AR0/object-layout mapping for OROMDATA/OPOSX/OPOSY/OPOSZ stores. */
    STUB();
}

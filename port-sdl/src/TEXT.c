#include "NOT_IMPLEMENTED.h"
#include "TRACE.h"
#include "STUB.h"
#include "TEXT.h"
#include "TEXT_DEBUG.h"

/* Scaffold generated from TEXT.ASM */

#define NUM_TEXTS 128
#define TEXT_SIZ_WORDS 16

static int TEXT_LIST[NUM_TEXTS * TEXT_SIZ_WORDS];
static int TEXT_FREE;
static int TEXT_ACTIVE;
static int TEXT_FREE_COUNT;
static int TEXT_FREEZE;

void TEXT_INIT(void) {
    TRACE();
    /* INIT_LINKED_LIST(TEXT_LIST, &TEXT_FREE, &TEXT_ACTIVE, NUM_TEXTS-1, TEXT_SIZ) */
    TEXT_ACTIVE = 0;
    TEXT_FREE = 0;
    for (int i = 0; i < NUM_TEXTS - 1; i++) {
        TEXT_LIST[i * TEXT_SIZ_WORDS] = (i + 1) * TEXT_SIZ_WORDS;
    }
    TEXT_LIST[(NUM_TEXTS - 1) * TEXT_SIZ_WORDS] = 0;
    TEXT_FREE_COUNT = NUM_TEXTS;
    TEXT_FREEZE = 0;
}

void TEXT_ADDDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TEXT_ADD1(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TEXT_ADD(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SETSMDIGITFONT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SETSMDIGITFONTDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SETLGDIGITFONT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SETLGDIGITFONTDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SETN43FONT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SETN43FONTDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SET40FONT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SET40FONTDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SET12FONT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SET12FONTDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SET18FONT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SET18FONTDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SETFIXEDFONT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SETFIXEDFONTDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void STRLEN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TEXT_OUTPUT(void) {
    TRACE();
    TEXT_DEBUG_DUMP_AND_CLEAR();
    STUB();
}

void STRCPY(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void STRCAT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void FIXEDFONT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void HIGHLIGHTN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

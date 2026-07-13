// NOEDIT
#include "texttab.h"

#include "../core/machine.h"
#include "../core/port.h"
#include "macs.h"
#include "text.h"

/*
 * Hand-maintained translation of asm/TEXTTAB.ASM.
 */

// asm: FONTDIGLGI	.word	FONTDIGLG
FONTENTRY* FONTDIGLGI = FONTDIGLG;

// ;size = 22
/* asm: FONTDIGLG: */
FONTENTRY FONTDIGLG[] = {
    FONTENT(1, 0, 17, 0, 0), // 0
    FONTENT(2, 19, 35, 0, 0),
    FONTENT(4, 37, 51, 0, 0),
    FONTENT(5, 53, 66, 0, 0),
    FONTENT(1, 68, 85, 0, 0),
    FONTENT(2, 87, 103, 0, 0),
    FONTENT(1, 105, 122, 0, 0),
    FONTENT(6, 124, 136, 0, 0),
    FONTENT(1, 138, 155, 0, 0),
    FONTENT(1, 157, 174, 0, 0), // 9
    FONTENT(2, 176, 179, 0, 1), // ':'
};

// asm: FONTDIGSMI	.word	FONTDIGSM
FONTENTRY* FONTDIGSMI = FONTDIGSM;

// ;size = 12
/* asm: FONTDIGSM: */
FONTENTRY FONTDIGSM[] = {
    FONTENT(2, 181, 190, 0, 0), // 0
    FONTENT(2, 192, 199, 0, 0),
    FONTENT(2, 201, 209, 0, 0),
    FONTENT(2, 211, 218, 0, 0),
    FONTENT(2, 220, 229, 0, 0),
    FONTENT(2, 231, 239, 0, 0),
    FONTENT(2, 241, 250, 0, 0),
    FONTENT(2, 181, 187, 14, 0),
    FONTENT(2, 189, 198, 14, 0),
    FONTENT(2, 200, 209, 14, 0), // 9
    FONTENT(3, 211, 212, 14, 2), // ':'
    FONTENT(0, 215, 224, 14, 0), // 'N'
};

// asm: FONTN43TABI	.word	FONTN43TAB
FONTENTRY* FONTN43TABI = FONTN43TAB;

// *leading,xstart,xend,ystart,trailing
/* asm: FONTN43TAB: */
FONTENTRY FONTN43TAB[] = {
    FONTENT(4, 26, 50, 0, 0),    // 0
    FONTENT(3, 50, 75, 0, 0),    // 1
    FONTENT(3, 76, 101, 0, 0),   // 2
    FONTENT(4, 102, 126, 0, 0),  // 3
    FONTENT(1, 5, 32, 42, 0),    // 4
    FONTENT(3, 34, 59, 42, 0),   // 5
    FONTENT(4, 61, 85, 42, 0),   // 6
    FONTENT(4, 87, 111, 42, 0),  // 7
    FONTENT(5, 113, 136, 42, 0), // 8
    FONTENT(5, 139, 162, 42, 0), // 9
    FONTENT(0, 4, 24, 0, 0),     // #
};

/*
 * FONT40TABI points 17 FONTENTRY records before FONT40_TAB in the shared
 * romdata stream. Model that as one backing block, then export pointers into
 * it instead of separate unrelated C arrays.
 */
static FONTENTRY FONT40_TAB_STORAGE[] = {
    FONTENT(2, 189, 198, 14, 0), // FONTDIGSM tail
    FONTENT(2, 200, 209, 14, 0), // FONTDIGSM tail
    FONTENT(3, 211, 212, 14, 2), // FONTDIGSM tail
    FONTENT(0, 215, 224, 14, 0), // FONTDIGSM tail
    FONTENT(4, 26, 50, 0, 0),    // FONTN43TAB 0
    FONTENT(3, 50, 75, 0, 0),    // FONTN43TAB 1
    FONTENT(3, 76, 101, 0, 0),   // FONTN43TAB 2
    FONTENT(4, 102, 126, 0, 0),  // FONTN43TAB 3
    FONTENT(1, 5, 32, 42, 0),    // FONTN43TAB 4
    FONTENT(3, 34, 59, 42, 0),   // FONTN43TAB 5
    FONTENT(4, 61, 85, 42, 0),   // FONTN43TAB 6
    FONTENT(4, 87, 111, 42, 0),  // FONTN43TAB 7
    FONTENT(5, 113, 136, 42, 0), // FONTN43TAB 8
    FONTENT(5, 139, 162, 42, 0), // FONTN43TAB 9
    FONTENT(0, 4, 24, 0, 0),     // FONTN43TAB #
    FONTENT(1, 233, 246, 44, 0), // .
    FONTENT(1, 229, 242, 44, 0), // !

    /* asm: FONT40_TAB: */
    FONTENT(1, 0, 27, 0, 0),     // A
    FONTENT(1, 28, 55, 0, 0),    // B
    FONTENT(1, 56, 75, 0, 0),    // C
    FONTENT(1, 76, 103, 0, 0),   // D
    FONTENT(1, 104, 125, 0, 0),  // E
    FONTENT(1, 126, 146, 0, 0),  // F
    FONTENT(1, 147, 174, 0, 0),  // G
    FONTENT(1, 175, 201, 0, 0),  // H
    FONTENT(1, 202, 215, 0, 0),  // I
    FONTENT(1, 216, 235, 0, 0),  // J
    FONTENT(1, 0, 28, 44, 0),    // K
    FONTENT(1, 29, 49, 44, 0),   // L
    FONTENT(1, 50, 92, 44, 0),   // M
    FONTENT(1, 93, 121, 44, 0),  // N
    FONTENT(1, 122, 149, 44, 0), // O
    FONTENT(1, 150, 177, 44, 0), // P
    FONTENT(1, 177, 204, 44, 0), // Q
    FONTENT(1, 205, 232, 44, 0), // R
    FONTENT(1, 0, 22, 88, 0),    // S
    FONTENT(1, 23, 46, 88, 0),   // T
    FONTENT(1, 46, 74, 88, 0),   // U
    FONTENT(1, 75, 103, 88, 0),  // V
    FONTENT(1, 104, 145, 88, 0), // W
    FONTENT(1, 146, 173, 88, 0), // X
    FONTENT(1, 174, 201, 96, 0), // Y
    FONTENT(1, 205, 228, 88, 0), // Z
};

FONTENTRY* FONT40_TAB = &FONT40_TAB_STORAGE[17];

/* asm: FONT40TABI	.word	FONT40_TAB-(17*FONTENT_SIZE) */
FONTENTRY* FONT40TABI = &FONT40_TAB_STORAGE[0];

// *SIZE = 18
/* asm: FONT18_TAB: */
FONTENTRY FONT18_TAB[] = {
    FONTENT(3, 54, 65, 18, 0),    // 0
    FONTENT(5, 66, 72, 18, 3),    // 1
    FONTENT(3, 73, 84, 18, 0),    // 2
    FONTENT(3, 85, 96, 18, 0),    // 3
    FONTENT(1, 97, 110, 18, 0),   // 4
    FONTENT(3, 111, 122, 18, 0),  // 5
    FONTENT(3, 123, 134, 18, 0),  // 6
    FONTENT(3, 135, 146, 18, 0),  // 7
    FONTENT(3, 147, 158, 18, 0),  // 8
    FONTENT(3, 159, 170, 18, 0),  // 9
    FONTENT(2, 214, 224, 18, 0),  // :
    FONTENT(2, 81, 98, 228, 0),   // .
    FONTENT(2, 220, 250, 189, 0), // #
    FONTENT(2, 227, 244, 126, 0), // !
    FONTENT(2, 227, 244, 126, 0), // !
    FONTENT(2, 227, 244, 126, 0), // !
    FONTENT(2, 179, 190, 18, 0),  // /
    FONTENT(2, 0, 11, 0, 0),      // A
    FONTENT(2, 12, 23, 0, 0),     // B
    FONTENT(2, 24, 33, 0, 0),     // C
    FONTENT(2, 34, 45, 0, 0),     // D
    FONTENT(2, 46, 55, 0, 0),     // E
    FONTENT(2, 56, 64, 0, 0),     // F
    FONTENT(2, 65, 76, 0, 0),     // G
    FONTENT(2, 77, 88, 0, 0),     // H
    FONTENT(2, 89, 93, 0, 0),     // I
    FONTENT(2, 94, 102, 0, 0),    // J
    FONTENT(2, 103, 114, 0, 0),   // K
    FONTENT(2, 115, 123, 0, 0),   // L
    FONTENT(2, 124, 142, 0, 0),   // M
    FONTENT(2, 143, 154, 0, 0),   // N
    FONTENT(2, 155, 166, 0, 0),   // O
    FONTENT(2, 167, 178, 0, 0),   // P
    FONTENT(2, 179, 190, 0, 0),   // Q
    FONTENT(2, 191, 202, 0, 0),   // R
    FONTENT(2, 203, 213, 0, 0),   // S
    FONTENT(2, 214, 224, 0, 0),   // T
    FONTENT(2, 225, 236, 0, 0),   // U
    FONTENT(2, 237, 248, 0, 0),   // V
    FONTENT(2, 0, 18, 18, 0),     // W
    FONTENT(2, 19, 30, 18, 0),    // X
    FONTENT(2, 31, 42, 18, 0),    // Y
    FONTENT(2, 43, 53, 18, 0),    // Z
};

// *SIZE = 12
/* asm: OGSMFONT_TAB: */
FONTENTRY OGSMFONT_TAB[] = {
    FONTENT(1, 66, 75, 13, 0),    // 0
    FONTENT(1, 76, 85, 13, 0),    // 1
    FONTENT(1, 86, 95, 13, 0),    // 2
    FONTENT(1, 96, 105, 13, 0),   // 3
    FONTENT(1, 106, 117, 13, 0),  // 4
    FONTENT(1, 118, 127, 13, 0),  // 5
    FONTENT(1, 128, 137, 13, 0),  // 6
    FONTENT(1, 138, 147, 13, 0),  // 7
    FONTENT(1, 148, 157, 13, 0),  // 8
    FONTENT(1, 158, 167, 13, 0),  // 9
    FONTENT(1, 168, 172, 17, 0),  // :
    FONTENT(1, 168, 172, 17, 0),  // .
    FONTENT(1, 220, 250, 189, 0), // #
    FONTENT(1, 227, 244, 126, 0), // !
    FONTENT(1, 227, 244, 126, 0), // !
    FONTENT(1, 227, 244, 126, 0), // !
    FONTENT(1, 209, 222, 13, 0),  // /
    FONTENT(1, 0, 9, 0, 0),       // A
    FONTENT(1, 10, 19, 0, 0),     // B
    FONTENT(1, 20, 28, 0, 0),     // C
    FONTENT(1, 29, 38, 0, 0),     // D
    FONTENT(1, 39, 47, 0, 0),     // E
    FONTENT(1, 48, 56, 0, 0),     // F
    FONTENT(1, 57, 66, 0, 0),     // G
    FONTENT(1, 67, 76, 0, 0),     // H
    FONTENT(1, 77, 81, 0, 0),     // I
    FONTENT(1, 82, 90, 0, 0),     // J
    FONTENT(1, 91, 100, 0, 0),    // K
    FONTENT(1, 101, 109, 0, 0),   // L
    FONTENT(1, 110, 124, 0, 0),   // M
    FONTENT(1, 125, 134, 0, 0),   // N
    FONTENT(1, 135, 144, 0, 0),   // O
    FONTENT(1, 145, 154, 0, 0),   // P
    FONTENT(1, 155, 164, 0, 0),   // Q
    FONTENT(1, 165, 174, 0, 0),   // R
    FONTENT(1, 175, 184, 0, 0),   // S
    FONTENT(1, 185, 193, 0, 0),   // T
    FONTENT(1, 0, 9, 13, 0),      // U
    FONTENT(1, 10, 19, 13, 0),    // V
    FONTENT(1, 20, 34, 13, 0),    // W
    FONTENT(1, 35, 44, 13, 0),    // X
    FONTENT(1, 45, 55, 13, 0),    // Y
    FONTENT(1, 56, 65, 13, 0),    // Z
};

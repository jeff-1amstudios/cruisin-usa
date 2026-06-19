// NOEDIT
#ifndef TEXTTAB_H
#define TEXTTAB_H

#include "port.h"

/*
 * Hand-maintained translation of asm/TEXTTAB.ASM.
 * Kept out of the skeleton generator because the generator does not currently
 * model these font tables as array-of-struct data.
 */

extern FONTENTRY FONTDIGLG[];
extern FONTENTRY *FONTDIGLGI;

extern FONTENTRY FONTDIGSM[];
extern FONTENTRY *FONTDIGSMI;

extern FONTENTRY FONTN43TAB[];
extern FONTENTRY *FONTN43TABI;

extern FONTENTRY *FONT40_TAB;
extern FONTENTRY *FONT40TABI;

extern FONTENTRY FONT18_TAB[];
extern FONTENTRY OGSMFONT_TAB[];

#endif /* TEXTTAB_H */

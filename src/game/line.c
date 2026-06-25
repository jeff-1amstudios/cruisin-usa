#include "../core/cpu.h"
#include "../core/machine.h"

/*
 * Source module: asm/LINE.ASM
 */

void _rectangle(int x1, int y1, int x2, int y2, int color);
static void EPI0_1(void);
void _line(int x0, int y0, int x1, int y1, int color);
static void EPI0_2(void);

void INV_F30(void);
void _pixel(int x, int y, int color);

#define FP AR3

/*
******************************************************
* FUNCTION DEF : _rectangle
******************************************************
*/
void _rectangle(int x1, int y1, int x2, int y2, int color)
{
    _line(x1, y1, x2, y1, color);
    _line(x1, y2, x2, y2, color);
    _line(x1, y1, x1, y2, color);
    _line(x2, y1, x2, y2, color);
}

static void EPI0_1(void)
{
    // asm 0000AC4D: 	LDI	*-FP(1),R1
    // asm 0000AC4E: 	LDI	*FP,FP
    // asm 0000AC4F: 	BD	R1
    // asm 0000AC50: 	POP	R5
    // asm 0000AC51: 	POP	R4
    // asm 0000AC52: 	SUBI	5,SP
    // ***	B	R1	;BRANCH OCCURS
    // ******************************************************
    // * FUNCTION DEF : _line
    // ******************************************************
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_1", 0, 0);
    UNIMPL();
}

void _line(int x0, int y0, int x1, int y1, int color)
{
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;

    dx = x1 > x0 ? x1 - x0 : x0 - x1;
    dy = y1 > y0 ? y1 - y0 : y0 - y1;
    sx = x0 < x1 ? 1 : -1;
    sy = y0 < y1 ? 1 : -1;
    err = dx - dy;
LINE_LOOP:
    _pixel(x0, y0, color);
    if (x0 == x1 && y0 == y1) {
        return;
    }
    e2 = err << 1;
    if (e2 > -dy) {
        err -= dy;
        x0 += sx;
    }
    if (e2 < dx) {
        err += dx;
        y0 += sy;
    }
    goto LINE_LOOP;
}

static void EPI0_2(void)
{
    // asm 0000ACF2: 	LDI	*-FP(1),R1
    // asm 0000ACF3: 	LDI	*FP,FP
    // asm 0000ACF4: 	POPF	R7
    // asm 0000ACF5: 	POPF	R6
    // asm 0000ACF6: 	BD	R1
    // asm 0000ACF7: 	POP	R5
    // asm 0000ACF8: 	POP	R4
    // asm 0000ACF9: 	SUBI	5,SP
    // ***	B	R1	;BRANCH OCCURS
    // ******************************************************
    // * UNDEFINED REFERENCES                               *
    // ******************************************************
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "EPI0_2", 0, 0);
    UNIMPL();
}

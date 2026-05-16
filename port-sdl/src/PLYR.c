#include "PLYR.h"

#include "STUB.h"
#include "TRACE.h"
#include "CMOS.h"

#include "NOT_IMPLEMENTED.h"

static float PEDALMN;
static float PEDALMX;
static float STEERMN;
static float STEERMX;
static float STEERCT;
static float BRAKEMN;
static float BRAKEMX;
static float STEERFR;
void _VIEW0(void) { TRACE(); STUB(); }
void _VIEW1(void) { TRACE(); STUB(); }
void _VIEW2(void) { TRACE(); STUB(); }

void CARPARAMTAB(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETCARPARAM(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _CARV0(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void BONUS_WAIT_LOOP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PLYR_CAR_INIT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PLYR_INTRO_ENTER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _PLYR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PLYRCAM(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CAMCHKL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CAMCHKR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CAMCHK0(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CAMCHKLR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CAMCHK(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETCAMPOS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CAMYADJ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PLYONRD(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void DRONEGO(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void DRONESTOP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETREV(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETRPM(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETSKID(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CKOFRD(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETDIR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CARSPIN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETCARROT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETSPD(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETBRAKE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _off_brake(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void _on_brake(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETPEDAL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETGEAR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETMAN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETAUTO(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETSTEER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ZOOM(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ZOOMUP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETTRAK(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CKBND(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void TUNCHK(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void INBOUNDZ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void DRONINBZ(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CURBCOL0(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CURBSPIN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CURBSPN(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void SOFTCURB(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETNXTRDIR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETRDCAR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void ROADIR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETRDIR(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PLYRWHL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PLYR_SNDS(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void MKFXSND(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void MKVFXSND(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void MKVFX1(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RANDSND(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void RANDVSND(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void DRONESND(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void GETCMOS_VALUES(void) {
    TRACE();
    /* PLYR.ASM: read 6 adjustments starting at ADJ_GASMIN into PEDALMN..BRAKEMX. */
    float *p = &PEDALMN;
    int adj = 1; /* ADJ_GASMIN */
    for (int i = 0; i < 6; i++, adj++) {
        const int v = ADJUSTMENT_READ(adj);
        *p++ = (float)v;
    }

    STEERFR = STEERMX - STEERMN;
}

void CAMMATSAV(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void CAMMATAVG(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

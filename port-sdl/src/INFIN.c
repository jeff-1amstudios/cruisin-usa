#include "NOT_IMPLEMENTED.h"
#include "TRACE.h"
#include "STUB.h"
#include "INFIN.h"
#include "GAME_STATE.h"

/* Scaffold generated from INFIN.ASM */

static float _CAMERARAD_Y;
static float CAMRADY;
static float HIGHEST_ROADY;
static float HIGHEST_ROADY_X;
static float INFIN_CORRECT = 0.0f;
static float SCRNHYI = 0.0f;
static float AMOUNT_CLIPPED;

static const float M_TWOPII = (float)(2.0 * 3.14159265358979323846);
static const float M_FORMULA = -244.4619926f; /* (6*256)/2PI */
static const int M_HIGHIVAL = 1536;
static const int M_LOIVAL = -768;

static int m_round_to_int(float v) {
    if (v >= 0.0f) {
        return (int)(v + 0.5f);
    }
    return (int)(v - 0.5f);
}

static void TRANS_PTS(void);
void WATER_INFINITY(void);

void FIND_HIGHEST_ROADY(void) {
    TRACE();
    STUB();
    HIGHEST_ROADY = 0.0f;
    HIGHEST_ROADY_X = 0.0f;
}

void INFINITY(void) {
    TRACE();
    uint32_t mode = _MODE;

    if ((mode & MINTUNNEL) != 0) {
        return;
    }

    if ((mode & MMODE) == MGAME) {
        FIND_HIGHEST_ROADY();
    }

    /* keep CAMERARAD.y in range [0, 2PI) */
    float cam_y = _CAMERARAD_Y;
    while (cam_y < 0.0f) {
        cam_y += M_TWOPII;
    }
    while (cam_y >= M_TWOPII) {
        cam_y -= M_TWOPII;
    }
    _CAMERARAD_Y = cam_y;
    CAMRADY = cam_y;

    /* horizon X offset */
    int r2 = m_round_to_int(M_FORMULA * cam_y);
    if (r2 >= M_HIGHIVAL) {
        r2 -= M_HIGHIVAL;
    }
    if (r2 <= M_LOIVAL) {
        r2 += M_HIGHIVAL;
    }
    (void)r2; /* R6 in asm; consumed by TRANS_PTS / raster prep. */

    /* amount clipped */
    float clip = HIGHEST_ROADY + 25.0f - (INFIN_CORRECT + SCRNHYI);
    if (clip < 0.0f) {
        clip = 0.0f;
    }
    if ((_MODE & MMODE) != MGAME) {
        clip = 0.0f;
    }
    AMOUNT_CLIPPED = clip;

    TRANS_PTS();

    /* Platform boundary: original pushes infinity polygons through FIFO raster hardware. */
    STUB();

    if ((_MODE & MWATER) != 0) {
        WATER_INFINITY();
    }
}

void TRANS_PTS(void) {
    TRACE();
    STUB();
}

void WATER_INFINITY(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

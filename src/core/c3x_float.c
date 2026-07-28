#include "c3x_float.h"

#include <math.h>

static uint16_t c3x_double_to_short_raw(double value) {
    int exponent;
    double mantissa;
    double fraction;
    int fraction_bits;
    uint16_t sign_bits;

    if (value == 0.0) {
        return 0x8000u;
    }

    mantissa = frexp(value, &exponent);
    mantissa = ldexp(mantissa, 1);
    exponent -= 1;

    if (value > 0.0) {
        fraction = (mantissa - 1.0) * 2048.0;
        fraction_bits = (int)floor(fraction + 0.5);
        sign_bits = 0x0000u;
    } else {
        fraction = (mantissa + 2.0) * 2048.0;
        fraction_bits = (int)floor(fraction + 0.5);
        sign_bits = 0x0800u;
    }

    if (fraction_bits >= 2048) {
        fraction_bits = 0;
        exponent += value > 0.0 ? 1 : -1;
    }

    if (exponent < -7) {
        return 0x8000u;
    }
    if (exponent > 7) {
        exponent = 7;
        fraction_bits = 0x07FF;
    }

    return (uint16_t)((((uint16_t)exponent) & 0x000fu) << 12) | sign_bits | ((uint16_t)fraction_bits & 0x07ffu);
}

static uint32_t c3x_double_to_single_raw_trunc(double value) {
    int exponent;
    double mantissa;
    double fraction;
    uint32_t mantissa_bits;
    uint64_t extended_fraction;

    if (value == 0.0) {
        return 0x80000000u;
    }

    mantissa = frexp(value, &exponent);
    mantissa = ldexp(mantissa, 1);
    exponent -= 1;

    if (exponent < -127) {
        return 0x80000000u;
    }
    if (exponent > 127) {
        return ((uint32_t)(uint8_t)127 << 24) | (value < 0.0 ? 0x00FFFFFFu : 0x007FFFFFu);
    }

    if (mantissa >= 0.0) {
        fraction = (mantissa - 1.0) * 2147483648.0;
        extended_fraction = (uint64_t)floor(fraction);
        mantissa_bits = (uint32_t)(extended_fraction >> 8);
    } else {
        if (mantissa == -1.0) {
            mantissa = -2.0;
            exponent -= 1;
        }
        fraction = (mantissa + 2.0) * 2147483648.0;
        extended_fraction = (uint64_t)floor(fraction);
        mantissa_bits = 0x00800000u | ((uint32_t)(extended_fraction >> 8) & 0x007FFFFFu);
    }

    return ((uint32_t)(uint8_t)(int8_t)exponent << 24) | mantissa_bits;
}

static float c3x_single_raw_to_float(uint32_t raw) {
    int exponent;
    uint32_t fraction;
    float mantissa;

    exponent = (int8_t)(raw >> 24);
    if (exponent == -128) {
        return 0.0f;
    }

    fraction = raw & 0x007fffffu;
    if ((raw & 0x00800000u) == 0) {
        mantissa = 1.0f + ((float)fraction / 8388608.0f);
    } else {
        mantissa = -2.0f + ((float)fraction / 8388608.0f);
    }

    return ldexpf(mantissa, exponent);
}

#ifdef C3X_USE_HOST_FLOAT

c3x_reg_t c3x_load(uint32_t raw) {
    return (c3x_reg_t){ c3x_single_raw_to_float(raw) };
}

c3x_reg_t c3x_load_short(uint16_t raw) {
    int exponent;
    uint16_t fraction;
    float mantissa;

    exponent = (int8_t)((raw & 0xF000u) >> 8) >> 4;
    if (exponent == -8 && (raw & 0x0FFFu) == 0) {
        return (c3x_reg_t){ 0.0f };
    }

    fraction = raw & 0x07FFu;
    if ((raw & 0x0800u) == 0) {
        mantissa = 1.0f + ((float)fraction / 2048.0f);
    } else {
        mantissa = -2.0f + ((float)fraction / 2048.0f);
    }

    return (c3x_reg_t){ ldexpf(mantissa, exponent) };
}

uint32_t c3x_store(c3x_reg_t value) {
    return c3x_double_to_single_raw_trunc(value.value);
}

c3x_reg_t c3x_from_double(double value) {
    return (c3x_reg_t){ (float)value };
}

float c3x_to_float(c3x_reg_t value) {
    return value.value;
}

c3x_reg_t c3x_rnd(c3x_reg_t value) {
    int exponent;
    double rounded_value;

    if (value.value == 0.0f) {
        return value;
    }

    frexp((double)value.value, &exponent);
    exponent -= 1;
    rounded_value = (double)value.value + ldexp(1.0, exponent - 24);
    return (c3x_reg_t){ c3x_single_raw_to_float(c3x_double_to_single_raw_trunc(rounded_value)) };
}

c3x_reg_t c3x_add(c3x_reg_t a, c3x_reg_t b) {
    return (c3x_reg_t){ a.value + b.value };
}

c3x_reg_t c3x_sub(c3x_reg_t a, c3x_reg_t b) {
    return (c3x_reg_t){ a.value - b.value };
}

c3x_reg_t c3x_mul(c3x_reg_t a, c3x_reg_t b) {
    return (c3x_reg_t){ a.value * b.value };
}

c3x_reg_t c3x_div(c3x_reg_t a, c3x_reg_t b) {
    return (c3x_reg_t){ a.value / b.value };
}

c3x_reg_t c3x_neg(c3x_reg_t value) {
    return (c3x_reg_t){ -value.value };
}

c3x_reg_t c3x_abs(c3x_reg_t value) {
    return (c3x_reg_t){ fabsf(value.value) };
}

int c3x_cmp(c3x_reg_t a, c3x_reg_t b) {
    return (a.value > b.value) - (a.value < b.value);
}

int c3x_fix(c3x_reg_t value) {
    return (int)floorf(value.value);
}

#else

#define C3X_EXP_MASK 0x000000ff00000000ull
#define C3X_MANT_MASK 0x00000000ffffffffull
#define C3X_ZERO_BITS 0x0000008000000000ull

static c3x_float c3x_pack(int exponent, uint32_t mantissa) {
    c3x_float value;

    value.bits = ((uint64_t)(uint8_t)(int8_t)exponent << 32) | mantissa;
    return value;
}

static int c3x_exponent(c3x_float value) {
    return (int8_t)((value.bits >> 32) & 0xffu);
}

static uint32_t c3x_mantissa(c3x_float value) {
    return (uint32_t)(value.bits & C3X_MANT_MASK);
}

static double c3x_to_double(c3x_float value) {
    int exponent;
    uint32_t mantissa_bits;
    uint32_t fraction;
    double mantissa;

    exponent = c3x_exponent(value);
    mantissa_bits = c3x_mantissa(value);
    if (exponent == -128) {
        return 0.0;
    }

    fraction = mantissa_bits & 0x7fffffffu;
    if ((mantissa_bits & 0x80000000u) == 0) {
        mantissa = 1.0 + ((double)fraction / 2147483648.0);
    } else {
        mantissa = -2.0 + ((double)fraction / 2147483648.0);
    }

    return ldexp(mantissa, exponent);
}

static c3x_float c3x_from_double_trunc(double value) {
    int exponent;
    double mantissa;
    double fraction;
    uint64_t fraction_bits;

    if (value == 0.0) {
        return c3x_pack(-128, 0);
    }

    mantissa = frexp(value, &exponent);
    mantissa = ldexp(mantissa, 1);
    exponent -= 1;

    if (exponent < -127) {
        return c3x_pack(-128, 0);
    }
    if (exponent > 127) {
        return c3x_pack(127, value < 0.0 ? 0xffffffffu : 0x7fffffffu);
    }

    if (mantissa >= 0.0) {
        fraction = (mantissa - 1.0) * 2147483648.0;
        fraction_bits = (uint64_t)floor(fraction);
        return c3x_pack(exponent, (uint32_t)(fraction_bits & 0x7fffffffu));
    }

    if (mantissa == -1.0) {
        mantissa = -2.0;
        exponent -= 1;
    }
    fraction = (mantissa + 2.0) * 2147483648.0;
    fraction_bits = (uint64_t)floor(fraction);
    return c3x_pack(exponent, 0x80000000u | ((uint32_t)fraction_bits & 0x7fffffffu));
}

c3x_reg_t c3x_load(uint32_t raw) {
    int exponent;
    uint32_t mantissa;

    exponent = (int8_t)(raw >> 24);
    if (exponent == -128) {
        return c3x_pack(-128, 0);
    }

    mantissa = (raw & 0x00ffffffu) << 8;
    return c3x_pack(exponent, mantissa);
}

c3x_reg_t c3x_load_short(uint16_t raw) {
    int exponent;
    uint16_t fraction;
    double mantissa;

    exponent = (int8_t)((raw & 0xF000u) >> 8) >> 4;
    if (exponent == -8 && (raw & 0x0FFFu) == 0) {
        return c3x_pack(-128, 0);
    }

    fraction = raw & 0x07FFu;
    if ((raw & 0x0800u) == 0) {
        mantissa = 1.0 + ((double)fraction / 2048.0);
    } else {
        mantissa = -2.0 + ((double)fraction / 2048.0);
    }

    return c3x_from_double_trunc(ldexp(mantissa, exponent));
}

uint32_t c3x_store(c3x_reg_t value) {
    return ((uint32_t)(value.bits >> 8) & 0x00ffffffu) | ((uint32_t)(value.bits >> 32) << 24);
}

c3x_reg_t c3x_from_double(double value) {
    return c3x_from_double_trunc(value);
}

float c3x_to_float(c3x_reg_t value) {
    return (float)c3x_to_double(value);
}

static __int128 c3x_extended_mantissa(c3x_reg_t value);
static c3x_reg_t c3x_from_extended_mantissa(int exponent, __int128 mantissa);

c3x_reg_t c3x_rnd(c3x_reg_t value) {
    int exponent;
    __int128 mantissa;

    exponent = c3x_exponent(value);
    if (exponent == -128) {
        return value;
    }

    mantissa = c3x_extended_mantissa(value);
    mantissa += 0x80u;
    mantissa &= ~((__int128)0xff);
    return c3x_from_extended_mantissa(exponent, mantissa);
}

static __int128 c3x_extended_mantissa(c3x_reg_t value) {
    uint32_t mantissa;
    __int128 magnitude;

    mantissa = c3x_mantissa(value);
    if (mantissa & 0x80000000u) {
        return (__int128)(mantissa & 0x7fffffffu) - ((__int128)1 << 32);
    }
    return ((__int128)1 << 31) + (mantissa & 0x7fffffffu);
}

static c3x_reg_t c3x_from_extended_mantissa(int exponent, __int128 mantissa) {
    __int128 min_normal;
    __int128 max_normal;
    uint32_t fraction;

    if (mantissa == 0) {
        return c3x_pack(-128, 0);
    }

    min_normal = (__int128)1 << 31;
    max_normal = (__int128)1 << 32;

    while (mantissa >= max_normal || mantissa <= -max_normal) {
        mantissa >>= 1;
        exponent++;
    }
    while ((mantissa > 0 && mantissa < min_normal) || (mantissa < 0 && mantissa >= -min_normal)) {
        mantissa <<= 1;
        exponent--;
    }

    if (exponent < -127) {
        return c3x_pack(-128, 0);
    }
    if (exponent > 127) {
        return c3x_pack(127, mantissa < 0 ? 0xffffffffu : 0x7fffffffu);
    }

    if (mantissa > 0) {
        fraction = (uint32_t)(mantissa - min_normal) & 0x7fffffffu;
        return c3x_pack(exponent, fraction);
    }

    fraction = (uint32_t)(mantissa + max_normal) & 0x7fffffffu;
    return c3x_pack(exponent, 0x80000000u | fraction);
}

static c3x_reg_t c3x_add_mantissas(c3x_reg_t a, c3x_reg_t b) {
    int aexp;
    int bexp;
    int exp;
    int diff;
    __int128 aman;
    __int128 bman;

    aexp = c3x_exponent(a);
    bexp = c3x_exponent(b);
    if (aexp == -128) {
        return b;
    }
    if (bexp == -128) {
        return a;
    }

    aman = c3x_extended_mantissa(a);
    bman = c3x_extended_mantissa(b);
    if (aexp >= bexp) {
        exp = aexp;
        diff = aexp - bexp;
        bman = diff >= 64 ? (bman < 0 ? -1 : 0) : (bman >> diff);
    } else {
        exp = bexp;
        diff = bexp - aexp;
        aman = diff >= 64 ? (aman < 0 ? -1 : 0) : (aman >> diff);
    }

    return c3x_from_extended_mantissa(exp, aman + bman);
}

c3x_reg_t c3x_add(c3x_reg_t a, c3x_reg_t b) {
    int64_t mantissa;
    int64_t aman;
    int64_t bman;
    int exp;
    int diff;

    if (c3x_exponent(a) == -128) {
        return b;
    }
    if (c3x_exponent(b) == -128) {
        return a;
    }

    aman = (int64_t)(int32_t)c3x_mantissa(a) ^ 0x80000000;
    bman = (int64_t)(int32_t)c3x_mantissa(b) ^ 0x80000000;
    if (c3x_exponent(a) > c3x_exponent(b)) {
        exp = c3x_exponent(a);
        diff = exp - c3x_exponent(b);
        if (diff >= 32) {
            return a;
        }
        bman >>= diff;
    } else {
        exp = c3x_exponent(b);
        diff = exp - c3x_exponent(a);
        if (diff >= 32) {
            return b;
        }
        aman >>= diff;
    }

    mantissa = aman + bman;

    if (mantissa == 0 || exp == -128) {
        exp = -128;
        mantissa = 0x80000000;
    } else if (mantissa >= (int64_t)0x100000000u || mantissa < (int64_t)0xffffffff00000000ull) {
        mantissa >>= 1;
        exp++;
    } else if (mantissa < (int64_t)0x80000000u && mantissa >= (int64_t)0xffffffff80000000ull) {
        if (mantissa > 0) {
            diff = __builtin_clz((uint32_t)mantissa);
            mantissa <<= diff;
            exp -= diff;
        } else {
            diff = __builtin_clz(~(uint32_t)mantissa);
            mantissa <<= diff;
            exp -= diff;
        }
    }

    if (exp <= -128) {
        return c3x_pack(-128, 0);
    }
    if (exp > 127) {
        return c3x_pack(127, mantissa < 0 ? 0xffffffffu : 0x7fffffffu);
    }

    return c3x_pack(exp, (uint32_t)mantissa ^ 0x80000000u);
}

c3x_reg_t c3x_sub(c3x_reg_t a, c3x_reg_t b) {
    int64_t mantissa;
    int64_t aman;
    int64_t bman;
    int exp;
    int diff;

    if (c3x_exponent(b) == -128) {
        return a;
    }

    aman = (int64_t)(int32_t)c3x_mantissa(a) ^ 0x80000000;
    bman = (int64_t)(int32_t)c3x_mantissa(b) ^ 0x80000000;
    if (c3x_exponent(a) > c3x_exponent(b)) {
        exp = c3x_exponent(a);
        diff = exp - c3x_exponent(b);
        if (diff >= 32) {
            return a;
        }
        bman >>= diff;
    } else {
        exp = c3x_exponent(b);
        diff = exp - c3x_exponent(a);
        if (diff >= 32) {
            return c3x_neg(b);
        }
        aman >>= diff;
    }

    mantissa = aman - bman;

    if (mantissa == 0 || exp == -128) {
        exp = -128;
        mantissa = 0x80000000;
    } else if (mantissa >= (int64_t)0x100000000u || mantissa < (int64_t)0xffffffff00000000ull) {
        mantissa >>= 1;
        exp++;
    } else if (mantissa < (int64_t)0x80000000u && mantissa >= (int64_t)0xffffffff80000000ull) {
        if (mantissa > 0) {
            diff = __builtin_clz((uint32_t)mantissa);
            mantissa <<= diff;
            exp -= diff;
        } else {
            diff = __builtin_clz(~(uint32_t)mantissa);
            mantissa <<= diff;
            exp -= diff;
        }
    }

    if (exp <= -128) {
        return c3x_pack(-128, 0);
    }
    if (exp > 127) {
        return c3x_pack(127, mantissa < 0 ? 0xffffffffu : 0x7fffffffu);
    }

    return c3x_pack(exp, (uint32_t)mantissa ^ 0x80000000u);
}

c3x_reg_t c3x_mul(c3x_reg_t a, c3x_reg_t b) {
    int aexp;
    int bexp;
    int exp;
    int32_t aman;
    int32_t bman;
    int64_t product;

    aexp = c3x_exponent(a);
    bexp = c3x_exponent(b);
    if (aexp == -128 || bexp == -128) {
        return c3x_pack(-128, 0);
    }

    aman = ((int32_t)c3x_mantissa(a) >> 8) ^ 0x800000;
    bman = ((int32_t)c3x_mantissa(b) >> 8) ^ 0x800000;
    product = (int64_t)aman * (int64_t)bman;
    exp = aexp + bexp;
    product >>= 15;

    if (product == 0) {
        return c3x_pack(-128, 0);
    }

    if (product >= (int64_t)0x100000000u) {
        product >>= 1;
        exp++;
        if (product >= (int64_t)0x100000000u) {
            product >>= 1;
            exp++;
        }
    } else if (product < (int64_t)0xffffffff00000000ull) {
        product >>= 1;
        exp++;
    }

    if (exp < -127) {
        return c3x_pack(-128, 0);
    }
    if (exp > 127) {
        return c3x_pack(127, product < 0 ? 0xffffffffu : 0x7fffffffu);
    }

    return c3x_pack(exp, (uint32_t)product ^ 0x80000000u);
}

c3x_reg_t c3x_div(c3x_reg_t a, c3x_reg_t b) {
    return c3x_from_double_trunc(c3x_to_double(a) / c3x_to_double(b));
}

c3x_reg_t c3x_neg(c3x_reg_t value) {
    int32_t mantissa;

    mantissa = (int32_t)c3x_mantissa(value);
    if (c3x_exponent(value) == -128) {
        return c3x_pack(-128, 0);
    }
    if ((mantissa & 0x7fffffffu) != 0) {
        return c3x_pack(c3x_exponent(value), (uint32_t)-mantissa);
    }
    if (mantissa == 0) {
        return c3x_pack(c3x_exponent(value) - 1, (uint32_t)mantissa ^ 0x80000000u);
    }

    return c3x_pack(c3x_exponent(value) + 1, (uint32_t)mantissa ^ 0x80000000u);
}

c3x_reg_t c3x_abs(c3x_reg_t value) {
    if (c3x_exponent(value) == -128) {
        return value;
    }

    return c3x_from_double_trunc(fabs(c3x_to_double(value)));
}

int c3x_cmp(c3x_reg_t a, c3x_reg_t b) {
    double da = c3x_to_double(a);
    double db = c3x_to_double(b);

    return (da > db) - (da < db);
}

int c3x_fix(c3x_reg_t value) {
    return (int)floor(c3x_to_double(value));
}

#endif

c3x_reg_t c3x_from_short_double(double value) {
    return c3x_load_short(c3x_double_to_short_raw(value));
}

c3x_f32_t c3x_stf(c3x_reg_t value) {
    return (c3x_f32_t){ c3x_single_raw_to_float(c3x_store(value)) };
}

c3x_reg_t c3x_ldf(c3x_f32_t value) {
    return c3x_from_double((double)value.value);
}

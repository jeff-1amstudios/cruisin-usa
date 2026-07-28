#ifndef C3X_FLOAT_H
#define C3X_FLOAT_H

#include <stdint.h>

typedef struct {
    uint64_t bits;
} c3x_float;

typedef struct {
    float value;
} c3x_f32_t;

#ifdef C3X_USE_HOST_FLOAT

typedef struct {
    float value;
} c3x_reg_t;

#define C3X_F32(value) (c3x_from_double((double)(value)))
#define C3X_IMM_F32(value) (c3x_from_short_double((double)(value)))
#define C3X_INIT(value, bits_value) { (float)(value) }
#define C3X_FROM_INT(value) (c3x_from_double((double)(value)))
#define C3X_TO_FLOAT(value) (c3x_to_float(value))
#define C3X_ADD(a, b) (c3x_add((a), (b)))
#define C3X_SUB(a, b) (c3x_sub((a), (b)))
#define C3X_MUL(a, b) (c3x_mul((a), (b)))
#define C3X_DIV(a, b) (c3x_div((a), (b)))
#define C3X_NEG(a) (c3x_neg(a))
#define C3X_ABS(a) (c3x_abs(a))
#define C3X_EQ(a, b) (c3x_cmp((a), (b)) == 0)
#define C3X_NE(a, b) (c3x_cmp((a), (b)) != 0)
#define C3X_LT(a, b) (c3x_cmp((a), (b)) < 0)
#define C3X_LE(a, b) (c3x_cmp((a), (b)) <= 0)
#define C3X_GT(a, b) (c3x_cmp((a), (b)) > 0)
#define C3X_GE(a, b) (c3x_cmp((a), (b)) >= 0)
#define C3X_RND(a) (c3x_rnd(a))
#define C3X_LOAD(raw) (c3x_load(raw))
#define C3X_STORE(value) (c3x_store(value))
#define C3X_STF(value) (c3x_stf(value))
#define C3X_LDF(value) (c3x_ldf(value))
#define C3X_F32_INIT(value) { (float)(value) }
#define C3X_FIX(value) (c3x_fix(value))
#define C3X_MPYF(src, dst) ((dst) = C3X_MUL((dst), (src)))
#define C3X_MPYF3(src2, src1, dst) ((dst) = C3X_MUL((src1), (src2)))
#define C3X_ADDF(src, dst) ((dst) = C3X_ADD((dst), (src)))
#define C3X_ADDF3(src2, src1, dst) ((dst) = C3X_ADD((src1), (src2)))
#define C3X_SUBF(src, dst) ((dst) = C3X_SUB((dst), (src)))
#define C3X_SUBF3(src2, src1, dst) ((dst) = C3X_SUB((src1), (src2)))

#else

typedef c3x_float c3x_reg_t;

#define C3X_F32(value) (c3x_from_double((double)(value)))
#define C3X_IMM_F32(value) (c3x_from_short_double((double)(value)))
#define C3X_INIT(value, bits_value) { (bits_value) }
#define C3X_FROM_INT(value) (c3x_from_double((double)(value)))
#define C3X_TO_FLOAT(value) (c3x_to_float(value))
#define C3X_ADD(a, b) (c3x_add((a), (b)))
#define C3X_SUB(a, b) (c3x_sub((a), (b)))
#define C3X_MUL(a, b) (c3x_mul((a), (b)))
#define C3X_DIV(a, b) (c3x_div((a), (b)))
#define C3X_NEG(a) (c3x_neg(a))
#define C3X_ABS(a) (c3x_abs(a))
#define C3X_EQ(a, b) (c3x_cmp((a), (b)) == 0)
#define C3X_NE(a, b) (c3x_cmp((a), (b)) != 0)
#define C3X_LT(a, b) (c3x_cmp((a), (b)) < 0)
#define C3X_LE(a, b) (c3x_cmp((a), (b)) <= 0)
#define C3X_GT(a, b) (c3x_cmp((a), (b)) > 0)
#define C3X_GE(a, b) (c3x_cmp((a), (b)) >= 0)
#define C3X_RND(a) (c3x_rnd(a))
#define C3X_LOAD(raw) (c3x_load(raw))
#define C3X_STORE(value) (c3x_store(value))
#define C3X_STF(value) (c3x_stf(value))
#define C3X_LDF(value) (c3x_ldf(value))
#define C3X_F32_INIT(value) { (float)(value) }
#define C3X_FIX(value) (c3x_fix(value))
#define C3X_MPYF(src, dst) ((dst) = C3X_MUL((dst), (src)))
#define C3X_MPYF3(src2, src1, dst) ((dst) = C3X_MUL((src1), (src2)))
#define C3X_ADDF(src, dst) ((dst) = C3X_ADD((dst), (src)))
#define C3X_ADDF3(src2, src1, dst) ((dst) = C3X_ADD((src1), (src2)))
#define C3X_SUBF(src, dst) ((dst) = C3X_SUB((dst), (src)))
#define C3X_SUBF3(src2, src1, dst) ((dst) = C3X_SUB((src1), (src2)))

#endif

c3x_reg_t c3x_load(uint32_t raw);
c3x_reg_t c3x_load_short(uint16_t raw);
c3x_reg_t c3x_from_short_double(double value);
uint32_t c3x_store(c3x_reg_t value);
c3x_reg_t c3x_stf(c3x_reg_t value);
c3x_reg_t c3x_ldf(c3x_f32_t value);
c3x_reg_t c3x_from_double(double value);
float c3x_to_float(c3x_reg_t value);
c3x_reg_t c3x_rnd(c3x_reg_t value);
c3x_reg_t c3x_add(c3x_reg_t a, c3x_reg_t b);
c3x_reg_t c3x_sub(c3x_reg_t a, c3x_reg_t b);
c3x_reg_t c3x_mul(c3x_reg_t a, c3x_reg_t b);
c3x_reg_t c3x_div(c3x_reg_t a, c3x_reg_t b);
c3x_reg_t c3x_neg(c3x_reg_t value);
c3x_reg_t c3x_abs(c3x_reg_t value);
int c3x_cmp(c3x_reg_t a, c3x_reg_t b);
int c3x_fix(c3x_reg_t value);

#endif /* C3X_FLOAT_H */

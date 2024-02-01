#ifndef FIXPT_H
#define FIXPT_H

#include <stdint.h>
#include <limits.h>
#include <errno.h>

#ifndef FIX_SHIFT
#define FIX_SHIFT 16
#endif
#define FIX_SHIFT_HALF (FIX_SHIFT / 2)

#define FLOAT_FRACTION_SZ 23
#define FLOAT_EXPONENT_SZ 8
#define FLOAT_EXPONENT_BIAS 127

#define DOUBLE_FRACTION_SZ 52
#define DOUBLE_EXPONENT_SZ 11
#define DOUBLE_EXPONENT_BIAS 1023

typedef int32_t errno_t;
typedef int64_t FixPT;

typedef union {
    float f;
    struct {
        uint32_t fraction: FLOAT_FRACTION_SZ;
        uint32_t exponent: FLOAT_EXPONENT_SZ;
        uint32_t sign: 1;
    } internal;
} Float;

typedef union {
    double f;
    struct {
        uint64_t fraction: 52;
        uint64_t exponent: 11;
        uint64_t sign: 1;
    } internal;
} Double;

uint8_t clzl(uint64_t x);

float Fix2Float(FixPT x);
FixPT Float2Fix(float x, errno_t *err);

double Fix2Double(FixPT x);
FixPT Double2Fix(double x, errno_t *err);

int64_t Fix2Int(FixPT x);
FixPT Int2Fix(int64_t x, errno_t *err);

#endif
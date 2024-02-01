#include "fixpt.h"

inline uint8_t clzl(uint64_t x) {
    #ifdef __NO_BUILTIN_CLZL
    for (int i = sizeof(uint64_t) * CHAR_BIT - 1; i >= 0; i--) {
        if ((x >> i) & 0b1) {
            return sizeof(uint64_t) * CHAR_BIT - 1 - i;
        }
    }
    return sizeof(uint64_t) * CHAR_BIT;
    #else
    return __builtin_clzl(x);
    #endif
}

inline float Fix2Float(FixPT x) {
    Float f;

    f.internal.sign = x < 0;
    x = x < 0 ? -x : x;

    uint8_t num_digits = sizeof(FixPT) * CHAR_BIT - 1 - clzl(x);
    f.internal.exponent = num_digits - FIX_SHIFT + FLOAT_EXPONENT_BIAS;

    int8_t rshift = num_digits - FLOAT_FRACTION_SZ;
    if (rshift >= 0) {
        f.internal.fraction = x >> rshift;
    } else {
        f.internal.fraction = x << -rshift;
    }

    return f.f;
}

inline FixPT Float2Fix(float x, errno_t *err) {
    Float *f = (Float*)&x;
    FixPT fix;

    int8_t rshift = FLOAT_FRACTION_SZ - FIX_SHIFT + FLOAT_EXPONENT_BIAS - f->internal.exponent;
    if (rshift > FLOAT_FRACTION_SZ || rshift <= 1 + FLOAT_FRACTION_SZ - (int8_t)sizeof(FixPT) * 8) {
        *err = ERANGE;
        return 0;
    }
    if (rshift >= 0) {
        fix = ((int64_t)f->internal.fraction + ((int64_t)1 << FLOAT_FRACTION_SZ)) >> rshift;
    } else {
        fix = ((int64_t)f->internal.fraction + ((int64_t)1 << FLOAT_FRACTION_SZ)) << -rshift;
    }

    return f->internal.sign ? -fix : fix;
}

inline double Fix2Double(FixPT x) {
    Double f;

    f.internal.sign = x < 0;
    x = x < 0 ? -x : x;

    uint8_t num_digits = sizeof(FixPT) * CHAR_BIT - 1 - clzl(x);
    f.internal.exponent = num_digits - FIX_SHIFT + DOUBLE_EXPONENT_BIAS;

    int8_t rshift = num_digits - DOUBLE_FRACTION_SZ;
    if (rshift >= 0) {
        f.internal.fraction = x >> rshift;
    } else {
        f.internal.fraction = x << -rshift;
    }

    return f.f;
}

inline FixPT Double2Fix(double x, errno_t *err) {
    Double *f = (Double*)&x;
    FixPT fix;

    int8_t rshift = DOUBLE_FRACTION_SZ - FIX_SHIFT + DOUBLE_EXPONENT_BIAS - f->internal.exponent;
    if (rshift > DOUBLE_FRACTION_SZ || rshift <= 1 + DOUBLE_FRACTION_SZ - (int8_t)sizeof(FixPT) * 8) {
        *err = ERANGE;
        return 0;
    }
    if (rshift >= 0) {
        fix = ((int64_t)f->internal.fraction + ((int64_t)1 << DOUBLE_FRACTION_SZ)) >> rshift;
    } else {
        fix = ((int64_t)f->internal.fraction + ((int64_t)1 << DOUBLE_FRACTION_SZ)) << -rshift;
    }

    return f->internal.sign ? -fix : fix;
}

inline FixPT Int2Fix(int64_t x, errno_t *err) {
    if (clzl(x >= 0 ? x : -x) < FIX_SHIFT) {
        *err = ERANGE;
        return 0;
    }
    return x << FIX_SHIFT;
}

inline int64_t Fix2Int(FixPT x) {
    return x >> FIX_SHIFT;
}
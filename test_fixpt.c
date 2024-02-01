#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fixpt.h"

char* bin(uint64_t x, uint8_t size) {
    char *s = malloc(size + 1);
    for (int i = 0; i < size; i++) {
        s[i] = '0' + ((x >> (size - 1 - i)) & 0b1);
    }
    return s;
}

int main(void) {
    printf("FIX_SHIFT = %d\n", FIX_SHIFT);
    printf("%ld <= FixPT <= %ld\n", INT64_MIN >> FIX_SHIFT, INT64_MAX >> FIX_SHIFT);
    putchar('\n');

    float a = 123123.123123;
    Float *af = (Float*)&a;

    printf("%f = (-1)^%d * 1.%s * 2^(%d-%d)\n",
        af->f, af->internal.sign,
        bin(af->internal.fraction, FLOAT_FRACTION_SZ),
        af->internal.exponent, FLOAT_EXPONENT_BIAS
    );

    FixPT a_fix = Float2Fix(a, NULL);

    printf("Float x bin: %s\n", bin(*(uint64_t*)&a, 64));
    printf("Fix   x bin: %s\n", bin(Float2Fix(a, NULL), 64));
    printf("Float x = %f; Float(Fix(x)) = %f\n", a, Fix2Float(Float2Fix(a, NULL)));
    putchar('\n');

    float x = 10.45;
    float y = -5.777;
    FixPT x_fix = Float2Fix(x, NULL);
    FixPT y_fix = Float2Fix(y, NULL);
    printf("x(x_fix) = %f(%ld), y(y_fix) = %f(%ld)\n", x, x_fix * (int64_t)pow(5, FIX_SHIFT), y, y_fix * (int64_t)powf(5, FIX_SHIFT));
    putchar('\n');
    printf("add: x     + y     = %f\n", x + y);
    printf("     x_fix + y_fix = %f\n", Fix2Float(x_fix + y_fix));
    putchar('\n');
    printf("mul: x     * y     = %f\n", x * y);
    printf("     x_fix * y_fix = %f\n", Fix2Float((x_fix >> FIX_SHIFT_HALF) * (y_fix >> FIX_SHIFT_HALF)));
    putchar('\n');
    printf("div: x     / y     = %f\n", x / y);
    printf("     x_fix / y_fix = %f\n", Fix2Float((x_fix << FIX_SHIFT) / y_fix));
    putchar('\n');
    printf("add: x     + 3     = %f\n", x + 3);
    printf("     x_fix + 3     = %f\n", Fix2Float(x_fix + Int2Fix(3, NULL)));
    putchar('\n');
    printf("mul: x     * y     = %f\n", x * 3);
    printf("     x_fix * 3     = %f\n", Fix2Float(x_fix * 3));
    putchar('\n');

    errno_t err = 0;
    float val_invalid = 10000000.1;
    FixPT _ = Float2Fix(val_invalid, &err);
    if (errno) {
      printf("out of range: '%f' cannot convert to FixPT\n", val_invalid);
      putchar('\n');
    }

    return 0;
}

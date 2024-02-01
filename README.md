# FixPT

浮動小数点と固定小数点を相互変換する簡易C言語ライブラリ．

リトルエンディアンのアーキテクチャのみ対応．
BPFなど浮動小数点演算をサポートしないアーキテクチャで近似演算を提供．

Simple C library for inter-conversion between floating-point and fixed-point. 

Supports little-endian architectures only.
Provides approximate arithmetic in environments that do not support floating-point arithmetic, such as BPF.

## Features

### Structures provided

- Float, Double (floating-point internal)
```c
typedef union {
    float f;
    struct {
        uint32_t fraction: FLOAT_FRACTION_SZ;
        uint32_t exponent: FLOAT_EXPONENT_SZ;
        uint32_t sign: 1;
    } internal;
} Float;
```

- FixPT (64-bit signed integer)
```c
typedef int64_t FixPT;
```

### Functions
Converts the structure without floating-point arithmetic. 

#### Float2Fix, Double2Fix, Int2Fix
  
If the cast fails, ERANGE is stored in the argument 'err'.
```c
FixPT Float2FIx(float x, error_t *err);
FixPT Double2FIx(double x, error_t *err);
FixPT Int2FIx(int64_t x, error_t *err);
```

#### Fix2Float, Fix2Double, Fix2Int
```c
float Fix2Float(FixPT x);
double Fix2Double(FixPT x);
int64_t Fix2Int(FixPT x);
```

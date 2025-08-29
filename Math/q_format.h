#ifndef Q_FORMAT_H
#define Q_FORMAT_H

#include <stdint.h>
#include <limits.h>

int32_t to_q_format(float value, uint8_t fractional_bits);
float from_q_format(int32_t value, uint8_t fractional_bits);

static inline int32_t q_add(int32_t a, int32_t b) {
    int64_t sum = (int64_t)a + (int64_t)b;
    if (sum > INT32_MAX) {
        return INT32_MAX;
    }
    if (sum < INT32_MIN) {
        return INT32_MIN;
    }
    return (int32_t)sum;
}

static inline int32_t q_mul(int32_t a, int32_t b, uint8_t fractional_bits) {
    int64_t prod = (int64_t)a * b;
    if (fractional_bits > 0) {
        int64_t remainder = prod & ((1LL << fractional_bits) - 1);
        if(remainder != 0) {
            int64_t rounding = (int64_t)1 << (fractional_bits - 1);
            prod = (prod >= 0) ? (prod + rounding) : (prod - rounding);
        }
        prod >>= fractional_bits;
    }
    if (prod > INT32_MAX) {
        return INT32_MAX;
    }
    if (prod < INT32_MIN) {
        return INT32_MIN;
    }
    return (int32_t)prod;
}

#define Q_ADD(a, b) q_add((a), (b))
#define Q_MUL(a, b, n) q_mul((a), (b), (n))

#endif // Q_FORMAT_H

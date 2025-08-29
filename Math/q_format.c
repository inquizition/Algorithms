#include "q_format.h"

int32_t to_q_format(float value, uint8_t fractional_bits) {
    double scaling = (double)(1u << fractional_bits);
    double max_val = (double)INT32_MAX / scaling;
    double min_val = (double)INT32_MIN / scaling;

    if (value >= max_val) {
        return INT32_MAX;
    }
    if (value <= min_val) {
        return INT32_MIN;
    }

    double scaled = value * scaling;
    if (scaled >= 0) {
        scaled += 0.5;
    } else {
        scaled -= 0.5;
    }

    return (int32_t)scaled;
}

float from_q_format(int32_t value, uint8_t fractional_bits) {
    double scaling = (double)(1u << fractional_bits);
    return (float)(value / scaling);
}

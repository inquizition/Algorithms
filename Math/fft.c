#include "fft.h"
#include "q_format.h"

#include <math.h>
#include <stdlib.h>

static inline int32_t q_neg(int32_t value) {
    if (value == INT32_MIN) {
        return INT32_MAX;
    }
    return -value;
}

static inline int32_t q_sub(int32_t a, int32_t b) {
    return q_add(a, q_neg(b));
}

static int32_t q_div_int(int32_t value, size_t divisor) {
    if (divisor == 0) {
        return 0;
    }

    int64_t temp = value;
    int64_t half = (int64_t)divisor / 2;
    if (temp >= 0) {
        temp += half;
    } else {
        temp -= half;
    }
    temp /= (int64_t)divisor;

    if (temp > INT32_MAX) {
        return INT32_MAX;
    }
    if (temp < INT32_MIN) {
        return INT32_MIN;
    }

    return (int32_t)temp;
}

static void fft_recursive(int32_t *real, int32_t *imag, size_t n, uint8_t fractional_bits) {
    if (n <= 1) {
        return;
    }

    size_t half = n / 2;
    int32_t *real_even = (int32_t *)malloc(half * sizeof(int32_t));
    int32_t *imag_even = (int32_t *)malloc(half * sizeof(int32_t));
    int32_t *real_odd = (int32_t *)malloc(half * sizeof(int32_t));
    int32_t *imag_odd = (int32_t *)malloc(half * sizeof(int32_t));

    if (!real_even || !imag_even || !real_odd || !imag_odd) {
        free(real_even);
        free(imag_even);
        free(real_odd);
        free(imag_odd);
        return;
    }

    for (size_t i = 0; i < half; ++i) {
        real_even[i] = real[2 * i];
        imag_even[i] = imag[2 * i];
        real_odd[i] = real[2 * i + 1];
        imag_odd[i] = imag[2 * i + 1];
    }

    fft_recursive(real_even, imag_even, half, fractional_bits);
    fft_recursive(real_odd, imag_odd, half, fractional_bits);

    for (size_t k = 0; k < half; ++k) {
        double angle = -2.0 * M_PI * (double)k / (double)n;
        int32_t cos_q = to_q_format((float)cos(angle), fractional_bits);
        int32_t sin_q = to_q_format((float)sin(angle), fractional_bits);

        int32_t cos_real = Q_MUL(cos_q, real_odd[k], fractional_bits);
        int32_t sin_imag = Q_MUL(sin_q, imag_odd[k], fractional_bits);
        int32_t tr = q_sub(cos_real, sin_imag);

        int32_t cos_imag = Q_MUL(cos_q, imag_odd[k], fractional_bits);
        int32_t sin_real = Q_MUL(sin_q, real_odd[k], fractional_bits);
        int32_t ti = q_add(cos_imag, sin_real);

        int32_t even_real = real_even[k];
        int32_t even_imag = imag_even[k];

        real[k] = q_add(even_real, tr);
        imag[k] = q_add(even_imag, ti);
        real[k + half] = q_sub(even_real, tr);
        imag[k + half] = q_sub(even_imag, ti);
    }

    free(real_even);
    free(imag_even);
    free(real_odd);
    free(imag_odd);
}

static int is_power_of_two(size_t n) {
    return n != 0 && (n & (n - 1)) == 0;
}

void fft(int32_t *real, int32_t *imag, size_t n) {
    if (!is_power_of_two(n)) {
        return;
    }

    fft_recursive(real, imag, n, FFT_Q_FRACTIONAL_BITS);
}

void ifft(int32_t *real, int32_t *imag, size_t n) {
    if (!is_power_of_two(n)) {
        return;
    }

    for (size_t i = 0; i < n; ++i) {
        imag[i] = q_neg(imag[i]);
    }

    fft_recursive(real, imag, n, FFT_Q_FRACTIONAL_BITS);

    for (size_t i = 0; i < n; ++i) {
        real[i] = q_div_int(real[i], n);
        imag[i] = q_neg(q_div_int(imag[i], n));
    }
}


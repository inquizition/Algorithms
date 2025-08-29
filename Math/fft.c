#include "fft.h"
#include <math.h>
#include <stdlib.h>

static void fft_recursive(double *real, double *imag, size_t n) {
    if (n <= 1) {
        return;
    }
    size_t half = n / 2;
    double *real_even = (double *)malloc(half * sizeof(double));
    double *imag_even = (double *)malloc(half * sizeof(double));
    double *real_odd = (double *)malloc(half * sizeof(double));
    double *imag_odd = (double *)malloc(half * sizeof(double));
    for (size_t i = 0; i < half; ++i) {
        real_even[i] = real[2 * i];
        imag_even[i] = imag[2 * i];
        real_odd[i] = real[2 * i + 1];
        imag_odd[i] = imag[2 * i + 1];
    }
    fft_recursive(real_even, imag_even, half);
    fft_recursive(real_odd, imag_odd, half);
    for (size_t k = 0; k < half; ++k) {
        double angle = -2.0 * M_PI * k / n;
        double cos_a = cos(angle);
        double sin_a = sin(angle);
        double tr = cos_a * real_odd[k] - sin_a * imag_odd[k];
        double ti = cos_a * imag_odd[k] + sin_a * real_odd[k];
        real[k] = real_even[k] + tr;
        imag[k] = imag_even[k] + ti;
        real[k + half] = real_even[k] - tr;
        imag[k + half] = imag_even[k] - ti;
    }
    free(real_even);
    free(imag_even);
    free(real_odd);
    free(imag_odd);
}

void fft(double *real, double *imag, size_t n) {
    fft_recursive(real, imag, n);
}

void ifft(double *real, double *imag, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        imag[i] = -imag[i];
    }
    fft_recursive(real, imag, n);
    for (size_t i = 0; i < n; ++i) {
        real[i] /= n;
        imag[i] = -imag[i] / n;
    }
}

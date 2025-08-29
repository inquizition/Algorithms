#ifndef M_FFT_H
#define M_FFT_H

#include <stddef.h>

void fft(double *real, double *imag, size_t n);
void ifft(double *real, double *imag, size_t n);

#endif // M_FFT_H

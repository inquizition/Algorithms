#ifndef M_FFT_H
#define M_FFT_H

#include <stddef.h>
#include <stdint.h>

#define FFT_Q_FRACTIONAL_BITS 15

void fft(int32_t *real, int32_t *imag, size_t n);
void ifft(int32_t *real, int32_t *imag, size_t n);

#endif // M_FFT_H

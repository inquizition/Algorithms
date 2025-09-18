#ifndef WIENER_FILTER_H
#define WIENER_FILTER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void wiener_filter(const double *input,
                   double *output,
                   size_t length,
                   size_t window_radius,
                   double noise_variance);

#ifdef __cplusplus
}
#endif

#endif /* WIENER_FILTER_H */

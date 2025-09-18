#include <math.h>
#include <stddef.h>
#include <stdio.h>

/**
 * Simple Wiener filter implementation for one-dimensional signals.
 *
 * The filter estimates the local mean and variance within a sliding window
 * centered on each sample and attenuates deviations from the mean using the
 * ratio between the signal variance and an assumed noise variance. When the
 * noise dominates the local window, the sample converges to the local mean;
 * otherwise the original value is preserved.
 */
void wiener_filter(const double *input,
                   double *output,
                   size_t length,
                   size_t window_radius,
                   double noise_variance) {
    if (input == NULL || output == NULL || length == 0) {
        return;
    }

    for (size_t i = 0; i < length; ++i) {
        size_t start = (i > window_radius) ? (i - window_radius) : 0;
        size_t end = i + window_radius + 1;
        if (end > length) {
            end = length;
        }

        size_t count = end - start;
        if (count == 0) {
            output[i] = input[i];
            continue;
        }

        double mean = 0.0;
        for (size_t j = start; j < end; ++j) {
            mean += input[j];
        }
        mean /= (double)count;

        double variance = 0.0;
        for (size_t j = start; j < end; ++j) {
            double diff = input[j] - mean;
            variance += diff * diff;
        }
        variance /= (double)count;

        double weight = 0.0;
        if (variance > 0.0) {
            weight = (variance - noise_variance) / variance;
            if (weight < 0.0) {
                weight = 0.0;
            } else if (weight > 1.0) {
                weight = 1.0;
            }
        }

        output[i] = mean + weight * (input[i] - mean);
    }
}

int main(void) {
    /* Example usage of the Wiener filter on a small noisy signal. */
    const double input_signal[] = {1.0, 1.2, 0.9, 5.0, 1.1, 0.95, 1.05};
    const size_t length = sizeof(input_signal) / sizeof(input_signal[0]);
    double filtered[length];

    const size_t window_radius = 1;   /* Three-sample window */
    const double noise_variance = 0.05; /* Estimated noise power */

    wiener_filter(input_signal, filtered, length, window_radius, noise_variance);

    printf("Input  -> Output\n");
    for (size_t i = 0; i < length; ++i) {
        printf("%6.3f -> %6.3f\n", input_signal[i], filtered[i]);
    }

    return 0;
}


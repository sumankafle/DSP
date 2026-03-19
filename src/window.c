/**
 * @file window.c
 * @brief Window function implementations for spectral analysis.
 * @author suman
 * @date 2024/2/28
 *
 * All window functions operate in-place on Complex arrays,
 * scaling both real and imaginary parts. A separate
 * window_generate() fills a float array with raw coefficients.
 */

#include "window.h"
#include <math.h>

void window_hamming(Complex *x, int n)
{
    double factor = TWO_PI / (n - 1);
    for (int i = 0; i < n; i++) {
        double w = 0.54 - 0.46 * cos(factor * i);
        x[i].real *= (float)w;
        x[i].imag *= (float)w;
    }
}

void window_hanning(Complex *x, int n)
{
    double factor = TWO_PI / (n - 1);
    for (int i = 0; i < n; i++) {
        double w = 0.5 - 0.5 * cos(factor * i);
        x[i].real *= (float)w;
        x[i].imag *= (float)w;
    }
}

void window_triangle(Complex *x, int n)
{
    double a = 2.0 / (n - 1);
    int mid = (n - 1) / 2;

    for (int i = 0; i <= mid; i++) {
        float w = (float)(i * a);
        x[i].real *= w;
        x[i].imag *= w;
    }
    for (int i = mid + 1; i < n; i++) {
        float w = (float)(2.0 - i * a);
        x[i].real *= w;
        x[i].imag *= w;
    }
}

void window_blackman(Complex *x, int n)
{
    double factor = TWO_PI / (n - 1);
    for (int i = 0; i < n; i++) {
        double w = 0.42 - 0.5 * cos(factor * i)
                        + 0.08 * cos(2.0 * factor * i);
        x[i].real *= (float)w;
        x[i].imag *= (float)w;
    }
}

void window_blackman_harris(Complex *x, int n)
{
    double factor = TWO_PI / n;
    for (int i = 0; i < n; i++) {
        double arg = factor * i;
        double w = 0.35875 - 0.48829 * cos(arg)
                            + 0.14128 * cos(2.0 * arg)
                            - 0.01168 * cos(3.0 * arg);
        x[i].real *= (float)w;
        x[i].imag *= (float)w;
    }
}

void window_apply(Complex *x, int n, WindowType type)
{
    switch (type) {
    case WIN_RECTANGULAR:
        break;
    case WIN_HAMMING:
        window_hamming(x, n);
        break;
    case WIN_HANNING:
        window_hanning(x, n);
        break;
    case WIN_TRIANGLE:
        window_triangle(x, n);
        break;
    case WIN_BLACKMAN:
        window_blackman(x, n);
        break;
    case WIN_BLACKMAN_HARRIS:
        window_blackman_harris(x, n);
        break;
    }
}

void window_generate(float *w, int n, WindowType type)
{
    double factor;

    switch (type) {
    case WIN_RECTANGULAR:
        for (int i = 0; i < n; i++)
            w[i] = 1.0f;
        break;

    case WIN_HAMMING:
        factor = TWO_PI / (n - 1);
        for (int i = 0; i < n; i++)
            w[i] = (float)(0.54 - 0.46 * cos(factor * i));
        break;

    case WIN_HANNING:
        factor = TWO_PI / (n - 1);
        for (int i = 0; i < n; i++)
            w[i] = (float)(0.5 - 0.5 * cos(factor * i));
        break;

    case WIN_TRIANGLE: {
        double a = 2.0 / (n - 1);
        int mid = (n - 1) / 2;
        for (int i = 0; i <= mid; i++)
            w[i] = (float)(i * a);
        for (int i = mid + 1; i < n; i++)
            w[i] = (float)(2.0 - i * a);
        break;
    }

    case WIN_BLACKMAN:
        factor = TWO_PI / (n - 1);
        for (int i = 0; i < n; i++)
            w[i] = (float)(0.42 - 0.5 * cos(factor * i)
                                + 0.08 * cos(2.0 * factor * i));
        break;

    case WIN_BLACKMAN_HARRIS:
        factor = TWO_PI / n;
        for (int i = 0; i < n; i++) {
            double arg = factor * i;
            w[i] = (float)(0.35875 - 0.48829 * cos(arg)
                                   + 0.14128 * cos(2.0 * arg)
                                   - 0.01168 * cos(3.0 * arg));
        }
        break;
    }
}

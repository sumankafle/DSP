/**
 * @file window.h
 * @brief Window function declarations for spectral analysis.
 * @author suman
 * @date 2024/2/28
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "dsp_types.h"

/**
 * @brief Available window types.
 */
typedef enum {
    WIN_RECTANGULAR = 0,
    WIN_HAMMING,
    WIN_HANNING,
    WIN_TRIANGLE,
    WIN_BLACKMAN,
    WIN_BLACKMAN_HARRIS
} WindowType;

/**
 * @brief Apply a window function to a complex array in-place.
 *
 * Scales both real and imaginary parts by the window coefficients.
 *
 * @param[in,out] x     Complex array to window.
 * @param[in]     n     Number of samples.
 * @param[in]     type  Window type to apply.
 */
void window_apply(Complex *x, int n, WindowType type);

/** @brief Apply Hamming window in-place. */
void window_hamming(Complex *x, int n);

/** @brief Apply Hanning (Hann) window in-place. */
void window_hanning(Complex *x, int n);

/** @brief Apply triangle (Bartlett) window in-place. */
void window_triangle(Complex *x, int n);

/** @brief Apply Blackman window in-place. */
void window_blackman(Complex *x, int n);

/** @brief Apply 4-term Blackman-Harris window in-place. */
void window_blackman_harris(Complex *x, int n);

/**
 * @brief Fill a float array with window coefficients.
 *
 * @param[out] w     Output coefficient array (n elements).
 * @param[in]  n     Window length.
 * @param[in]  type  Window type.
 */
void window_generate(float *w, int n, WindowType type);

#endif /* WINDOW_H */

/**
 * @file convolution.h
 * @brief Linear convolution of 1-D signals.
 * @author suman
 * @date 2024/2/28
 */

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "dsp_types.h"

/**
 * @brief Compute linear convolution of two signals.
 *
 * out[n] = sum_k { x[k] * h[n - k] }
 * Output length = x_len + h_len - 1.
 *
 * @param[in]  x        Input signal.
 * @param[in]  x_len    Length of input signal.
 * @param[in]  h        Impulse response / kernel.
 * @param[in]  h_len    Length of impulse response.
 * @param[out] out      Output array (x_len + h_len - 1 elements).
 * @param[in]  out_len  Allocated length of output (must be >= x_len + h_len - 1).
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus convolve(const float *x, int x_len,
                   const float *h, int h_len,
                   float *out, int out_len);

/**
 * @brief Compute convolution with "same" output size (centered).
 *
 * Returns the central x_len samples of the full convolution.
 *
 * @param[in]  x        Input signal.
 * @param[in]  x_len    Length of input signal.
 * @param[in]  h        Impulse response / kernel.
 * @param[in]  h_len    Length of impulse response.
 * @param[out] out      Output array (x_len elements).
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus convolve_same(const float *x, int x_len,
                        const float *h, int h_len,
                        float *out);

#endif /* CONVOLUTION_H */

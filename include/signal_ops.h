/**
 * @file signal_ops.h
 * @brief Element-wise signal operations (scale, add, normalize, copy).
 * @author suman
 * @date 2024/2/28
 */

#ifndef SIGNAL_OPS_H
#define SIGNAL_OPS_H

#include "dsp_types.h"

/**
 * @brief Scale a signal in-place: x[i] *= gain.
 *
 * @param[in,out] x     Signal array.
 * @param[in]     len   Number of samples.
 * @param[in]     gain  Scale factor.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus signal_scale(float *x, int len, float gain);

/**
 * @brief Add two signals element-wise: out[i] = a[i] + b[i].
 *
 * @param[in]  a    First input signal.
 * @param[in]  b    Second input signal.
 * @param[out] out  Output signal (may alias a or b).
 * @param[in]  len  Number of samples.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus signal_add(const float *a, const float *b, float *out, int len);

/**
 * @brief Subtract two signals element-wise: out[i] = a[i] - b[i].
 *
 * @param[in]  a    First input signal.
 * @param[in]  b    Second input signal.
 * @param[out] out  Output signal (may alias a or b).
 * @param[in]  len  Number of samples.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus signal_sub(const float *a, const float *b, float *out, int len);

/**
 * @brief Normalize a signal to [-1, +1] range in-place.
 *
 * Divides every sample by the peak absolute value.
 * If the signal is all zeros, it is left unchanged.
 *
 * @param[in,out] x    Signal array.
 * @param[in]     len  Number of samples.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus signal_normalize(float *x, int len);

/**
 * @brief Compute the energy of a signal: sum(x[i]^2).
 *
 * @param[in]  x      Signal array.
 * @param[in]  len    Number of samples.
 * @param[out] energy Output energy value.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus signal_energy(const float *x, int len, float *energy);

#endif /* SIGNAL_OPS_H */

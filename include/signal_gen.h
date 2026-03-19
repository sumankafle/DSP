/**
 * @file signal_gen.h
 * @brief Signal generation functions.
 * @author suman
 * @date 2024/2/28
 */

#ifndef SIGNAL_GEN_H
#define SIGNAL_GEN_H

#include "dsp_types.h"

/**
 * @brief Generate a signal as the sum of cosine waves.
 *
 * Sums cosine waves at the specified normalized frequencies
 * (sampling rate = 1). Result is normalized by the number of
 * frequency components.
 *
 * @param[out] out         Pre-allocated output array (length elements).
 * @param[in]  length      Number of samples.
 * @param[in]  freqs       Normalized frequencies (0 to 0.5).
 * @param[in]  num_freqs   Number of frequency components.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus signal_gen_cosines(float *out, int length,
                             const float *freqs, int num_freqs);

/**
 * @brief Generate a single cosine wave.
 *
 * @param[out] out       Pre-allocated output array.
 * @param[in]  length    Number of samples.
 * @param[in]  freq      Normalized frequency (0 to 0.5).
 * @param[in]  phase     Initial phase in radians.
 * @param[in]  amplitude Signal amplitude.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus signal_gen_cosine(float *out, int length,
                            float freq, float phase, float amplitude);

/**
 * @brief Generate a unit impulse (Dirac delta).
 *
 * @param[out] out     Pre-allocated output array (zeroed, then set at delay).
 * @param[in]  length  Number of samples.
 * @param[in]  delay   Sample index of the impulse.
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus signal_gen_impulse(float *out, int length, int delay);

#endif /* SIGNAL_GEN_H */

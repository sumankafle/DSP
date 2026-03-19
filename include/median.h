/**
 * @file median.h
 * @brief Conditional median filter for 1-D signals.
 * @author suman
 * @date 2024/2/28
 */

#ifndef MEDIAN_H
#define MEDIAN_H

#include "dsp_types.h"

/**
 * @brief Apply a conditional median filter to a float array.
 *
 * Replaces a sample with the median of its neighborhood if the
 * difference between the sample and the local median exceeds
 * the threshold. A threshold of 0 gives unconditional median filtering.
 *
 * Edge samples that can't be centered in the window are left unchanged.
 *
 * @param[in]  in          Input signal.
 * @param[out] out         Output signal (same length).
 * @param[in]  len         Signal length.
 * @param[in]  median_len  Window length (typically odd).
 * @param[in]  threshold   Conditional replacement threshold (>= 0).
 * @return DSP_SUCCESS, DSP_ERR_PARAM, or DSP_ERR_ALLOC.
 */
DspStatus median_filter(const float *in, float *out, int len,
                        int median_len, float threshold);

#endif /* MEDIAN_H */

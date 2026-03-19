/**
 * @file correlation.h
 * @brief Cross-correlation and autocorrelation functions.
 * @author suman
 * @date 2024/2/28
 */

#ifndef CORRELATION_H
#define CORRELATION_H

#include "dsp_types.h"

/**
 * @brief Compute cross-correlation between two signals.
 *
 * R_xy(d) = sum_{n=0}^{cor_len-1} x[n] * y[n + d]
 * for d = min_lag .. max_lag.
 *
 * Caller must ensure y has valid data from y[min_lag]
 * through y[cor_len + max_lag - 1].
 *
 * @param[in]  x         First signal array.
 * @param[in]  y         Second signal array.
 * @param[out] result    Output array (max_lag - min_lag + 1 elements).
 * @param[in]  cor_len   Number of samples to accumulate per lag.
 * @param[in]  min_lag   Minimum lag value.
 * @param[in]  max_lag   Maximum lag value (inclusive).
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus cross_correlate(const float *x, const float *y, float *result,
                          int cor_len, int min_lag, int max_lag);

/**
 * @brief Compute autocorrelation of a signal.
 *
 * Handles positive and negative lags safely by adjusting
 * summation bounds per lag.
 *
 * @param[in]  x         Input signal.
 * @param[out] result    Output array (max_lag - min_lag + 1 elements).
 * @param[in]  len       Signal length.
 * @param[in]  min_lag   Minimum lag.
 * @param[in]  max_lag   Maximum lag (inclusive).
 * @return DSP_SUCCESS or DSP_ERR_PARAM.
 */
DspStatus auto_correlate(const float *x, float *result,
                         int len, int min_lag, int max_lag);

#endif /* CORRELATION_H */

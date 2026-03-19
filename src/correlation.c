/**
 * @file correlation.c
 * @brief Cross-correlation and autocorrelation.
 * @author suman
 * @date 2024/2/28
 */

#include "correlation.h"

DspStatus cross_correlate(const float *x, const float *y, float *result,
                          int cor_len, int min_lag, int max_lag)
{
    if (!x || !y || !result || cor_len <= 0 || min_lag > max_lag)
        return DSP_ERR_PARAM;

    for (int d = min_lag; d <= max_lag; d++) {
        float sum = 0.0f;
        for (int n = 0; n < cor_len; n++)
            sum += x[n] * y[n + d];
        result[d - min_lag] = sum;
    }

    return DSP_SUCCESS;
}

DspStatus auto_correlate(const float *x, float *result,
                         int len, int min_lag, int max_lag)
{
    if (!x || !result || len <= 0 || min_lag > max_lag)
        return DSP_ERR_PARAM;

    for (int d = min_lag; d <= max_lag; d++) {
        float sum = 0.0f;
        /* adjust bounds so x[n] and x[n+d] both stay in [0, len) */
        int start = (d >= 0) ? 0 : -d;
        int end   = (d >= 0) ? len - d : len;
        for (int n = start; n < end; n++)
            sum += x[n] * x[n + d];
        result[d - min_lag] = sum;
    }

    return DSP_SUCCESS;
}

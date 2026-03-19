/**
 * @file convolution.c
 * @brief Linear convolution of 1-D signals.
 * @author suman
 * @date 2024/2/28
 */

#include "convolution.h"
#include <string.h>

DspStatus convolve(const float *x, int x_len,
                   const float *h, int h_len,
                   float *out, int out_len)
{
    if (!x || !h || !out || x_len <= 0 || h_len <= 0)
        return DSP_ERR_PARAM;

    int full_len = x_len + h_len - 1;
    if (out_len < full_len)
        return DSP_ERR_PARAM;

    memset(out, 0, (size_t)full_len * sizeof(float));

    for (int n = 0; n < full_len; n++) {
        int k_lo = (n - h_len + 1 > 0) ? n - h_len + 1 : 0;
        int k_hi = (n < x_len - 1) ? n : x_len - 1;
        for (int k = k_lo; k <= k_hi; k++)
            out[n] += x[k] * h[n - k];
    }

    return DSP_SUCCESS;
}

DspStatus convolve_same(const float *x, int x_len,
                        const float *h, int h_len,
                        float *out)
{
    if (!x || !h || !out || x_len <= 0 || h_len <= 0)
        return DSP_ERR_PARAM;

    int full_len = x_len + h_len - 1;
    int offset = (h_len - 1) / 2;

    for (int n = offset; n < offset + x_len; n++) {
        float sum = 0.0f;
        int k_lo = (n - h_len + 1 > 0) ? n - h_len + 1 : 0;
        int k_hi = (n < x_len - 1) ? n : x_len - 1;
        (void)full_len;
        for (int k = k_lo; k <= k_hi; k++)
            sum += x[k] * h[n - k];
        out[n - offset] = sum;
    }

    return DSP_SUCCESS;
}

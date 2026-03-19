/**
 * @file signal_ops.c
 * @brief Element-wise signal operations.
 * @author suman
 * @date 2024/2/28
 */

#include "signal_ops.h"
#include <math.h>

DspStatus signal_scale(float *x, int len, float gain)
{
    if (!x || len <= 0) return DSP_ERR_PARAM;

    for (int i = 0; i < len; i++)
        x[i] *= gain;
    return DSP_SUCCESS;
}

DspStatus signal_add(const float *a, const float *b, float *out, int len)
{
    if (!a || !b || !out || len <= 0) return DSP_ERR_PARAM;

    for (int i = 0; i < len; i++)
        out[i] = a[i] + b[i];
    return DSP_SUCCESS;
}

DspStatus signal_sub(const float *a, const float *b, float *out, int len)
{
    if (!a || !b || !out || len <= 0) return DSP_ERR_PARAM;

    for (int i = 0; i < len; i++)
        out[i] = a[i] - b[i];
    return DSP_SUCCESS;
}

DspStatus signal_normalize(float *x, int len)
{
    if (!x || len <= 0) return DSP_ERR_PARAM;

    float peak = 0.0f;
    for (int i = 0; i < len; i++) {
        float abs_val = fabsf(x[i]);
        if (abs_val > peak) peak = abs_val;
    }

    if (peak == 0.0f) return DSP_SUCCESS;

    float inv_peak = 1.0f / peak;
    for (int i = 0; i < len; i++)
        x[i] *= inv_peak;

    return DSP_SUCCESS;
}

DspStatus signal_energy(const float *x, int len, float *energy)
{
    if (!x || !energy || len <= 0) return DSP_ERR_PARAM;

    float sum = 0.0f;
    for (int i = 0; i < len; i++)
        sum += x[i] * x[i];

    *energy = sum;
    return DSP_SUCCESS;
}

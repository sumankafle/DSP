/**
 * @file stats.c
 * @brief Statistical analysis functions for signal data.
 * @author suman
 * @date 2024/2/28
 */

#include "stats.h"
#include <math.h>

float stats_min(const float *data, int len)
{
    float val = data[0];
    for (int i = 1; i < len; i++)
        if (data[i] < val) val = data[i];
    return val;
}

float stats_max(const float *data, int len)
{
    float val = data[0];
    for (int i = 1; i < len; i++)
        if (data[i] > val) val = data[i];
    return val;
}

float stats_mean(const float *data, int len)
{
    float sum = 0.0f;
    for (int i = 0; i < len; i++)
        sum += data[i];
    return sum / len;
}

float stats_variance(const float *data, int len)
{
    float sum = 0.0f;
    float sum2 = 0.0f;

    for (int i = 0; i < len; i++) {
        sum  += data[i];
        sum2 += data[i] * data[i];
    }

    float mean = sum / len;
    return (sum2 - sum * mean) / (len - 1);
}

float stats_rms(const float *data, int len)
{
    float sum2 = 0.0f;
    for (int i = 0; i < len; i++)
        sum2 += data[i] * data[i];
    return sqrtf(sum2 / len);
}

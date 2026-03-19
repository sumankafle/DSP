/**
 * @file median.c
 * @brief Conditional median filter implementation.
 * @author suman
 * @date 2024/2/28
 *
 * Performs 1-D median filtering with an optional threshold for
 * conditional replacement. Useful for spike/impulse removal
 * without the blurring artifacts of linear filters.
 */

#include "median.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/** Comparison function for qsort on float arrays. */
static int float_compare(const void *a, const void *b)
{
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    if (fa > fb) return  1;
    if (fa < fb) return -1;
    return 0;
}

DspStatus median_filter(const float *in, float *out, int len,
                        int median_len, float threshold)
{
    if (!in || !out || len <= 0 || median_len < 1 || median_len > len)
        return DSP_ERR_PARAM;

    float *sort_buf = malloc(median_len * sizeof(float));
    if (!sort_buf) return DSP_ERR_ALLOC;

    /* start with a copy -- edge samples stay untouched */
    memcpy(out, in, len * sizeof(float));

    int half = median_len / 2;

    for (int i = 0; i <= len - median_len; i++) {
        memcpy(sort_buf, in + i, median_len * sizeof(float));
        qsort(sort_buf, median_len, sizeof(float), float_compare);

        float med = sort_buf[half];
        if (fabsf(med - in[i + half]) > threshold)
            out[i + half] = med;
    }

    free(sort_buf);
    return DSP_SUCCESS;
}

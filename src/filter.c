/**
 * @file filter.c
 * @brief FIR and IIR digital filter implementations.
 * @author suman
 * @date 2024/2/28
 *
 * Includes block-mode FIR filtering (with edge handling),
 * sample-by-sample FIR for real-time use, integer FIR,
 * and cascaded direct-form-II biquad IIR filtering.
 */

#include "filter.h"
#include <stdlib.h>
#include <stdio.h>

/*--------------------------------------------------------------------
 * FIR block filter (center-aligned with edge handling)
 *
 * The filter starts and ends with reduced tap counts, centered
 * on the data. This gives zero-latency output without requiring
 * padding or overlap-save.
 *-------------------------------------------------------------------*/

void fir_filter_array(const float *in, float *out, int len,
                      const FirFilter *fir)
{
    const float *coef_start = fir->coef;
    int half = (fir->num_taps + 1) / 2;
    const float *in_end = in + len - 1;
    const float *in_ptr = in + half - 1;
    int acc_length = half;

    for (int i = 0; i < len; i++) {
        const float *data = in_ptr;
        const float *coef = coef_start;

        float acc = (*coef++) * (*data--);
        for (int j = 1; j < acc_length; j++)
            acc += (*coef++) * (*data--);

        *out++ = acc;

        if (in_ptr == in_end) {
            acc_length--;
            coef_start++;
        } else {
            if (acc_length < fir->num_taps)
                acc_length++;
            in_ptr++;
        }
    }
}

/*--------------------------------------------------------------------
 * FIR integer array filter
 *-------------------------------------------------------------------*/

void fir_filter_int(const int *in, int *out, int len,
                    const int *coef, int coef_len, int scale)
{
    const int *coef_start = coef;
    int half = (coef_len + 1) / 2;
    const int *in_end = in + len - 1;
    const int *in_ptr = in + half - 1;
    int acc_length = half;

    for (int i = 0; i < len; i++) {
        const int *data = in_ptr;
        const int *cp = coef_start;

        long acc = (long)(*cp++) * (*data--);
        for (int j = 1; j < acc_length; j++)
            acc += (long)(*cp++) * (*data--);

        *out++ = (int)(acc / scale);

        if (in_ptr == in_end) {
            acc_length--;
            coef_start++;
        } else {
            if (acc_length < coef_len)
                acc_length++;
            in_ptr++;
        }
    }
}

/*--------------------------------------------------------------------
 * FIR sample-by-sample (real-time)
 *
 * Maintains a delay line in fir->history. The output at time n is:
 *   y[n] = sum_{k=0}^{N-1} coef[k] * x[n-k]
 *-------------------------------------------------------------------*/

float fir_filter(float input, FirFilter *fir)
{
    /* allocate delay line on first call */
    if (!fir->history) {
        fir->history = calloc(fir->num_taps - 1, sizeof(float));
        if (!fir->history) {
            fprintf(stderr, "fir_filter: failed to allocate history\n");
            return 0.0f;
        }
    }

    float *hist = fir->history;
    float *prev = hist;
    const float *coef = fir->coef + fir->num_taps - 1;

    /* accumulate from oldest to newest in history */
    float output = *hist++ * (*coef--);
    for (int i = 2; i < fir->num_taps; i++) {
        *prev++ = *hist;       /* shift delay line */
        output += (*hist++) * (*coef--);
    }
    output += input * (*coef);  /* current input tap */
    *prev = input;              /* store new sample */

    return output;
}

/*--------------------------------------------------------------------
 * IIR filter (cascaded direct-form-II biquad sections)
 *
 * Each section computes:
 *   w[n] = x[n] - a1*w[n-1] - a2*w[n-2]
 *   y[n] = w[n] + b1*w[n-1] + b2*w[n-2]
 *
 * Coefficients: [gain, a1_0, a2_0, b1_0, b2_0, a1_1, ...]
 * State layout: [w1_0, w2_0, w1_1, w2_1, ...]
 *-------------------------------------------------------------------*/

float iir_filter(float input, IirFilter *iir)
{
    if (!iir->history) {
        iir->history = calloc(2 * iir->num_sections, sizeof(float));
        if (!iir->history) {
            fprintf(stderr, "iir_filter: failed to allocate state\n");
            return 0.0f;
        }
    }

    const float *coef = iir->coef;
    float *h1 = iir->history;
    float *h2 = h1 + 1;

    float output = input * (*coef++);   /* overall gain */

    for (int i = 0; i < iir->num_sections; i++) {
        float s1 = *h1;
        float s2 = *h2;

        output    = output - s1 * (*coef++);
        float w_n = output - s2 * (*coef++);        /* poles */

        output = w_n + s1 * (*coef++);
        output = output + s2 * (*coef++);           /* zeros */

        *h2++ = *h1;
        *h1++ = w_n;
        h1++;
        h2++;
    }

    return output;
}

/*--------------------------------------------------------------------
 * Cleanup
 *-------------------------------------------------------------------*/

void fir_filter_free(FirFilter *fir)
{
    if (fir && fir->history) {
        free(fir->history);
        fir->history = NULL;
    }
}

void iir_filter_free(IirFilter *iir)
{
    if (iir && iir->history) {
        free(iir->history);
        iir->history = NULL;
    }
}

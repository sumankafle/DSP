/**
 * @file signal_gen.c
 * @brief Signal generation utility functions.
 * @author suman
 * @date 2024/2/28
 */

#include "signal_gen.h"
#include <math.h>
#include <string.h>

DspStatus signal_gen_cosines(float *out, int length,
                             const float *freqs, int num_freqs)
{
    if (!out || length <= 0 || !freqs || num_freqs <= 0)
        return DSP_ERR_PARAM;

    memset(out, 0, length * sizeof(float));

    for (int j = 0; j < num_freqs; j++) {
        double w = TWO_PI * freqs[j];
        for (int i = 0; i < length; i++)
            out[i] += (float)cos(w * i);
    }

    /* normalize by number of components */
    float inv = 1.0f / num_freqs;
    for (int i = 0; i < length; i++)
        out[i] *= inv;

    return DSP_SUCCESS;
}

DspStatus signal_gen_cosine(float *out, int length,
                            float freq, float phase, float amplitude)
{
    if (!out || length <= 0)
        return DSP_ERR_PARAM;

    double w = TWO_PI * freq;
    for (int i = 0; i < length; i++)
        out[i] = amplitude * (float)cos(w * i + phase);

    return DSP_SUCCESS;
}

DspStatus signal_gen_impulse(float *out, int length, int delay)
{
    if (!out || length <= 0 || delay < 0 || delay >= length)
        return DSP_ERR_PARAM;

    memset(out, 0, length * sizeof(float));
    out[delay] = 1.0f;

    return DSP_SUCCESS;
}

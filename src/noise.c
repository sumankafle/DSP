/**
 * @file noise.c
 * @brief Random noise generator implementations.
 * @author suman
 * @date 2024/2/28
 *
 * Gaussian noise via Box-Muller transform. Uniform noise
 * from the standard rand() function.
 */

#include "noise.h"
#include <stdlib.h>
#include <math.h>

#ifndef RAND_MAX
#define RAND_MAX 32767
#endif

double noise_uniform(void)
{
    return (double)(rand() & RAND_MAX) / RAND_MAX - 0.5;
}

double noise_gaussian(void)
{
    static int ready = 0;
    static double stored;

    if (!ready) {
        double v1, v2, r;

        /* generate a pair inside the unit circle */
        do {
            v1 = 2.0 * noise_uniform();
            v2 = 2.0 * noise_uniform();
            r = v1 * v1 + v2 * v2;
        } while (r >= 1.0 || r == 0.0);

        double fac = sqrt(-2.0 * log(r) / r);
        stored = v1 * fac;
        ready = 1;
        return v2 * fac;
    }

    ready = 0;
    return stored;
}

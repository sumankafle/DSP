/**
 * @file demo_convolution.c
 * @brief Convolution demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Convolves an impulse train with a short rectangular pulse
 * to show how convolution "smears" the impulses.
 *
 * Output files (in data/):
 *   conv_input.csv  - input impulse train
 *   conv_output.csv - input vs convolved result
 */

#include "dsp_types.h"
#include "convolution.h"
#include "csv_writer.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define N_SIG   256
#define N_KERN  16
#define N_OUT   (N_SIG + N_KERN - 1)

int main(void)
{
    float sig[N_SIG];
    float kern[N_KERN];
    float out[N_OUT];

    memset(sig, 0, sizeof(sig));
    /* impulse train at sample 40, 100, 160, 220 */
    sig[40]  = 1.0f;
    sig[100] = 1.0f;
    sig[160] = 1.0f;
    sig[220] = 0.7f;

    /* rectangular pulse kernel (uniform averaging) */
    for (int i = 0; i < N_KERN; i++)
        kern[i] = 1.0f / N_KERN;

    convolve(sig, N_SIG, kern, N_KERN, out, N_OUT);

    mkdir("data", 0755);

    csv_write_single("data/conv_input.csv", sig, N_SIG, "amplitude");
    /* write first N_SIG samples of output for easy overlay */
    csv_write_double("data/conv_output.csv", sig, out, N_SIG,
                     "input", "convolved");

    printf("Convolution demo: wrote data/conv_input.csv, conv_output.csv\n");
    printf("  Input: impulse train, %d samples\n", N_SIG);
    printf("  Kernel: rectangular pulse, %d taps\n", N_KERN);
    return 0;
}

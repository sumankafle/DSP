/**
 * @file demo_fir.c
 * @brief FIR filter demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Generates a multitone signal (low + high frequency), applies a
 * low-pass FIR filter, and saves input/output to CSV for plotting.
 *
 * Output files (in data/):
 *   fir_input.csv   - original multitone signal
 *   fir_output.csv  - input vs filtered output
 */

#include "dsp_types.h"
#include "signal_gen.h"
#include "signal_ops.h"
#include "filter.h"
#include "csv_writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

/* Simple low-pass FIR (15-tap windowed sinc, cutoff ~ 0.15 normalized) */
static float lp_coefs[] = {
    -0.007f,  0.000f,  0.025f,  0.063f,  0.109f,
     0.147f,  0.166f,  0.166f,  0.147f,  0.109f,
     0.063f,  0.025f,  0.000f, -0.007f, -0.005f
};

#define N_TAPS  (int)(sizeof(lp_coefs) / sizeof(lp_coefs[0]))
#define N_SAMP  256

int main(void)
{
    float input[N_SAMP];
    float lo[N_SAMP], hi[N_SAMP];
    float output[N_SAMP];

    /* generate two-tone signal: 0.05 (low) + 0.35 (high) normalized freq */
    signal_gen_cosine(lo, N_SAMP, 0.05f, 0.0f, 0.5f);
    signal_gen_cosine(hi, N_SAMP, 0.35f, 0.0f, 0.5f);
    signal_add(lo, hi, input, N_SAMP);

    /* apply FIR low-pass filter */
    FirFilter lpf = { .num_taps = N_TAPS, .coef = lp_coefs, .history = NULL };
    fir_filter_array(input, output, N_SAMP, &lpf);

    /* ensure data/ directory exists */
    mkdir("data", 0755);

    /* write CSV files */
    csv_write_single("data/fir_input.csv", input, N_SAMP, "amplitude");
    csv_write_double("data/fir_output.csv", input, output, N_SAMP,
                     "input", "filtered");

    printf("FIR demo: wrote data/fir_input.csv and data/fir_output.csv\n");
    printf("  Signal: 0.05 + 0.35 normalized freq, %d samples\n", N_SAMP);
    printf("  Filter: %d-tap low-pass FIR\n", N_TAPS);
    printf("  Run: gnuplot scripts/plot_signal.gp\n");
    return 0;
}

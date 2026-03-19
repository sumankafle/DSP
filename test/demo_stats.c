/**
 * @file demo_stats.c
 * @brief Statistics demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Generates a noisy sine wave and computes running statistics
 * over a sliding window to visualize the signal envelope.
 *
 * Output files (in data/):
 *   stats_signal.csv  - original noisy signal
 *   stats_running.csv - running mean and RMS envelope
 */

#include "dsp_types.h"
#include "signal_gen.h"
#include "noise.h"
#include "stats.h"
#include "csv_writer.h"
#include <stdio.h>
#include <sys/stat.h>

#define N   512
#define WIN 32   /* sliding window for running stats */

int main(void)
{
    float sig[N], rmean[N], rrms[N];

    /* sine + noise */
    signal_gen_cosine(sig, N, 0.02f, 0.0f, 1.0f);
    for (int i = 0; i < N; i++)
        sig[i] += (float)noise_gaussian() * 0.3f;

    /* running mean and RMS over a sliding window */
    for (int i = 0; i < N; i++) {
        int lo = (i - WIN / 2 >= 0) ? i - WIN / 2 : 0;
        int hi = (i + WIN / 2 < N) ? i + WIN / 2 : N - 1;
        int wlen = hi - lo + 1;
        rmean[i] = stats_mean(&sig[lo], wlen);
        rrms[i]  = stats_rms(&sig[lo], wlen);
    }

    mkdir("data", 0755);

    csv_write_single("data/stats_signal.csv", sig, N, "amplitude");
    csv_write_double("data/stats_running.csv", rmean, rrms, N,
                     "running_mean", "running_rms");

    printf("Stats demo: wrote data/stats_signal.csv, stats_running.csv\n");
    printf("  Noisy sine: f=0.02, noise_std=0.3, %d samples\n", N);
    printf("  Running window: %d samples\n", WIN);
    return 0;
}

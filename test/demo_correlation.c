/**
 * @file demo_correlation.c
 * @brief Correlation demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Shows autocorrelation of a cosine (periodic peak pattern) and
 * cross-correlation detecting a time-shifted copy of a signal.
 *
 * Output files (in data/):
 *   corr_auto.csv  - autocorrelation of a cosine
 *   corr_cross.csv - cross-correlation showing delay detection
 */

#include "dsp_types.h"
#include "signal_gen.h"
#include "correlation.h"
#include "csv_writer.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define N     256
#define SHIFT 20

int main(void)
{
    float sig[N];
    signal_gen_cosine(sig, N, 0.08f, 0.0f, 1.0f);

    /* autocorrelation: lags -64 to +64 */
    int min_lag = -64, max_lag = 64;
    int n_lags = max_lag - min_lag + 1;
    float acorr[129];               /* 129 = 64+64+1 */
    auto_correlate(sig, acorr, N, min_lag, max_lag);

    /* cross-correlation: sig vs delayed copy */
    float delayed[N + SHIFT];
    memset(delayed, 0, sizeof(delayed));
    for (int i = 0; i < N; i++)
        delayed[i + SHIFT] = sig[i];

    float xcorr[129];
    cross_correlate(sig, delayed, xcorr, N, min_lag, max_lag);

    /* write using float arrays indexed by lag */
    mkdir("data", 0755);

    /* We need lag index as x-axis. csv_write_single uses 0..n as index,
       so offset the plot in gnuplot by min_lag. */
    csv_write_single("data/corr_auto.csv", acorr, n_lags, "R_xx");
    csv_write_single("data/corr_cross.csv", xcorr, n_lags, "R_xy");

    printf("Correlation demo: wrote data/corr_auto.csv, corr_cross.csv\n");
    printf("  Autocorrelation: cosine f=0.08, lags %d..%d\n", min_lag, max_lag);
    printf("  Cross-correlation: shift=%d samples\n", SHIFT);
    return 0;
}

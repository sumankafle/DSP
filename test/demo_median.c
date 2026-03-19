/**
 * @file demo_median.c
 * @brief Median filter demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Adds salt-and-pepper spike noise to a sine wave,
 * then removes spikes with a median filter.
 *
 * Output files (in data/):
 *   median_noisy.csv  - sine with spike noise
 *   median_output.csv - noisy vs median-filtered overlay
 */

#include "dsp_types.h"
#include "signal_gen.h"
#include "median.h"
#include "csv_writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define N 256

int main(void)
{
    float clean[N], noisy[N], filtered[N];

    signal_gen_cosine(clean, N, 0.04f, 0.0f, 1.0f);

    /* copy and add salt-and-pepper spikes */
    for (int i = 0; i < N; i++)
        noisy[i] = clean[i];

    srand(42);
    for (int i = 0; i < 20; i++) {
        int idx = rand() % N;
        noisy[idx] = (rand() % 2) ? 3.0f : -3.0f;
    }

    /* 5-point unconditional median filter (threshold=0) */
    median_filter(noisy, filtered, N, 5, 0.0f);

    mkdir("data", 0755);

    csv_write_single("data/median_noisy.csv", noisy, N, "amplitude");
    csv_write_double("data/median_output.csv", noisy, filtered, N,
                     "noisy", "median_filtered");

    printf("Median demo: wrote data/median_noisy.csv, median_output.csv\n");
    printf("  Signal: sine f=0.04 with 20 salt-and-pepper spikes\n");
    printf("  Filter: 5-point median, unconditional\n");
    return 0;
}

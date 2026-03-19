/**
 * @file demo_noise.c
 * @brief Noise generator demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Generates Gaussian and uniform noise sequences and saves to CSV.
 *
 * Output files (in data/):
 *   noise_gaussian.csv - Gaussian noise samples
 *   noise_uniform.csv  - uniform noise samples
 */

#include "dsp_types.h"
#include "noise.h"
#include "csv_writer.h"
#include <stdio.h>
#include <sys/stat.h>

#define N 512

int main(void)
{
    float gauss[N], uniform[N];

    for (int i = 0; i < N; i++) {
        gauss[i]   = (float)noise_gaussian();
        uniform[i] = (float)noise_uniform();
    }

    mkdir("data", 0755);

    csv_write_single("data/noise_gaussian.csv", gauss, N, "amplitude");
    csv_write_single("data/noise_uniform.csv", uniform, N, "amplitude");

    printf("Noise demo: wrote data/noise_gaussian.csv, noise_uniform.csv\n");
    return 0;
}

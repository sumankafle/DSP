/**
 * @file demo_signal_gen.c
 * @brief Signal generation demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Generates several basic waveforms and saves them to CSV.
 *
 * Output files (in data/):
 *   siggen_cosine.csv   - single cosine wave
 *   siggen_multitone.csv - sum of 3 cosines
 *   siggen_impulse.csv  - unit impulse
 */

#include "dsp_types.h"
#include "signal_gen.h"
#include "csv_writer.h"
#include <stdio.h>
#include <sys/stat.h>

#define N 256

int main(void)
{
    float cosine[N], multitone[N], impulse[N];

    signal_gen_cosine(cosine, N, 0.08f, 0.0f, 1.0f);

    float freqs[] = { 0.05f, 0.12f, 0.25f };
    signal_gen_cosines(multitone, N, freqs, 3);

    signal_gen_impulse(impulse, N, 32);

    mkdir("data", 0755);

    csv_write_single("data/siggen_cosine.csv", cosine, N, "amplitude");
    csv_write_single("data/siggen_multitone.csv", multitone, N, "amplitude");
    csv_write_single("data/siggen_impulse.csv", impulse, N, "amplitude");

    printf("Signal-gen demo: wrote data/siggen_cosine.csv, "
           "siggen_multitone.csv, siggen_impulse.csv\n");
    return 0;
}

/**
 * @file demo_fft.c
 * @brief FFT magnitude spectrum demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Generates a multitone signal, applies a Hamming window,
 * computes the FFT, and saves the time-domain signal and
 * magnitude spectrum to CSV files for Gnuplot visualization.
 *
 * Output files (in data/):
 *   fft_signal.csv   - windowed time-domain signal
 *   fft_spectrum.csv - magnitude spectrum (N/2 bins)
 */

#include "dsp_types.h"
#include "signal_gen.h"
#include "window.h"
#include "dft.h"
#include "csv_writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

#define M      8          /* log2(N) */
#define N      (1 << M)   /* 256 samples */

int main(void)
{
    Complex buf[N];

    /* generate 3-tone test signal: 0.05, 0.15, 0.30 normalized */
    float freqs[] = { 0.05f, 0.15f, 0.30f };
    float temp[N];
    signal_gen_cosines(temp, N, freqs, 3);

    /* copy to complex buffer */
    for (int i = 0; i < N; i++) {
        buf[i].real = temp[i];
        buf[i].imag = 0.0f;
    }

    /* apply Hamming window */
    window_hamming(buf, N);

    /* extract windowed signal for CSV */
    float windowed[N];
    for (int i = 0; i < N; i++)
        windowed[i] = buf[i].real;

    /* compute FFT in-place */
    fft(buf, M);

    /* ensure data/ directory exists */
    mkdir("data", 0755);

    /* write CSV files */
    csv_write_single("data/fft_signal.csv", windowed, N, "amplitude");
    csv_write_spectrum("data/fft_spectrum.csv", buf, N / 2);

    printf("FFT demo: wrote data/fft_signal.csv and data/fft_spectrum.csv\n");
    printf("  Signal: 3 tones at 0.05, 0.15, 0.30 normalized freq\n");
    printf("  Window: Hamming, %d points\n", N);
    printf("  Run: gnuplot scripts/plot_fft.gp\n");
    return 0;
}

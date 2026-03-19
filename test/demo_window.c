/**
 * @file demo_window.c
 * @brief Window function demonstration with CSV output.
 * @author suman
 * @date 2024/2/28
 *
 * Generates all supported window functions and saves the
 * coefficients to a multi-column CSV for shape comparison.
 *
 * Output files (in data/):
 *   window_shapes.csv - all window types side by side
 */

#include "dsp_types.h"
#include "window.h"
#include "csv_writer.h"
#include <stdio.h>
#include <sys/stat.h>

#define N 256

int main(void)
{
    float hamming[N], hanning[N], blackman[N], bh[N], triangle[N];

    window_generate(hamming,  N, WIN_HAMMING);
    window_generate(hanning,  N, WIN_HANNING);
    window_generate(blackman, N, WIN_BLACKMAN);
    window_generate(bh,       N, WIN_BLACKMAN_HARRIS);
    window_generate(triangle, N, WIN_TRIANGLE);

    mkdir("data", 0755);

    /* Write a 6-column CSV manually for all windows */
    FILE *fp = fopen("data/window_shapes.csv", "w");
    if (!fp) { printf("Error opening file\n"); return 1; }

    fprintf(fp, "index,hamming,hanning,blackman,blackman_harris,triangle\n");
    for (int i = 0; i < N; i++) {
        fprintf(fp, "%d,%g,%g,%g,%g,%g\n", i,
                (double)hamming[i], (double)hanning[i],
                (double)blackman[i], (double)bh[i],
                (double)triangle[i]);
    }
    fclose(fp);

    printf("Window demo: wrote data/window_shapes.csv\n");
    printf("  Windows: Hamming, Hanning, Blackman, Blackman-Harris, Triangle\n");
    printf("  Length: %d points\n", N);
    return 0;
}

# C DSP Algorithms

A collection of DSP (Digital Signal Processing) routines written in portable C11.
Started as a learning exercise based on classic DSP textbook code, cleaned up
and reorganized into something actually usable.

Covers the fundamentals: FFT, FIR/IIR filtering, windowing, basic signal
generation and analysis. Nothing fancy, but the implementations are correct
and readable enough to learn from.

## What's here

```
├── include/       Headers
├── src/           Core DSP implementations
├── common/        Shared utilities (vector ops, circular buffer, CSV writer)
├── test/          Unit tests (test_*) and CSV demos (demo_*)
├── scripts/       Gnuplot scripts for visualization
├── data/          Generated CSV data and plots (from make demos / make plot)
├── obj/           Compiled objects + static library
├── bin/           Test and demo executables
├── cdsp/          Original legacy code (reference)
└── Makefile
```

### Modules

| File | What it does |
|------|-------------|
| `dft.c` | DFT, FFT (radix-2 DIT), inverse FFT, real FFT |
| `window.c` | Hamming, Hanning, Blackman, Blackman-Harris, Triangle |
| `filter.c` | FIR (block + sample-by-sample), IIR (cascaded biquad) |
| `signal_ops.c` | Scale, add, subtract, normalize, energy |
| `convolution.c` | Linear convolution (full and same-size) |
| `noise.c` | Gaussian (Box-Muller) and uniform RNG |
| `stats.c` | Min, max, mean, variance, RMS |
| `signal_gen.c` | Cosine generation, impulse, sum of sinusoids |
| `median.c` | Conditional median filter |
| `correlation.c` | Cross-correlation and autocorrelation |

### Utilities (common/)

| File | What it does |
|------|-------------|
| `vector_ops.h` | Type-generic vector operation macros (add, sub, mul, dot, scale) |
| `circular_buffer.h/.c` | Fixed-size ring buffer for streaming / real-time DSP |
| `csv_writer.h/.c` | CSV file export for signals and spectra |

## Building

Requires GCC and GNU Make. Tested on Ubuntu but should work on any
Linux/macOS with a C11 compiler.

```bash
# release build (default, -O2, -Werror)
make

# debug build (symbols, no optimization)
make DEBUG=1

# clean everything
make clean
```

This produces `obj/libdsp.a` (static library), test binaries, and demo
binaries in `bin/`.

## Running tests

```bash
make test
```

Runs all test binaries and reports pass/fail. Example output:

```
=== FFT / DFT / Window Tests ===
  FFT of DC signal                             [PASS]
  FFT/IFFT roundtrip                           [PASS]
  FFT of impulse                               [PASS]
  DFT matches FFT (16-point)                   [PASS]
  DFT/IDFT roundtrip (16-point)                [PASS]
  Hamming window endpoints and center          [PASS]
  Hanning window zero endpoints                [PASS]
  window_generate matches in-place             [PASS]

```

## Demos and visualization

Demo programs generate CSV data files in `data/` that can be plotted
with Gnuplot.

```bash
# run all demo programs (generates CSV in data/)
make demos

# generate PNG plots from CSV data (requires gnuplot)
make plot

# or run individually
./bin/demo_signal_gen    # cosine, multitone, impulse waveforms
./bin/demo_noise         # Gaussian and uniform noise sequences
./bin/demo_fft           # FFT magnitude spectrum of a multitone signal
./bin/demo_fir           # FIR low-pass filter: input vs filtered
./bin/demo_convolution   # impulse train convolved with rectangular pulse
./bin/demo_correlation   # autocorrelation and cross-correlation
./bin/demo_stats         # running mean and RMS envelope of noisy sine
./bin/demo_median        # spike removal with 5-point median filter
./bin/demo_window        # all window function shapes side by side
```

### CSV format

All CSV files use the same simple format:
```
index,column_name
0,1.234
1,-0.567
...
```

Two-column variant (e.g. input vs output):
```
index,input,filtered
0,1.234,0.987
1,-0.567,-0.234
...
```

Easy to load in Python (`pandas.read_csv`), MATLAB (`readtable`), or any
spreadsheet.

### Gnuplot

Install Gnuplot if you don't have it:
```bash
sudo apt install gnuplot    # Ubuntu/Debian
brew install gnuplot         # macOS
```

Scripts in `scripts/` produce PNG files in `data/`.
To customize, edit the `.gp` files. Change `set terminal` to `qt` or
`x11` for interactive windows instead of PNG output.

### Plot gallery

After running `make plot`, the following PNG files appear in `data/`:

| Plot | Script | What it shows |
|------|--------|---------------|
| [`siggen_plot.png`](data/siggen_plot.png) | `plot_signal_gen.gp` | Three panels: single cosine, sum of 3 cosines, unit impulse |
| [`noise_plot.png`](data/noise_plot.png) | `plot_noise.gp` | Gaussian noise (zero-mean, unit variance) and uniform noise [-0.5, 0.5] |
| [`fft_plot.png`](data/fft_plot.png) | `plot_fft.gp` | Hamming-windowed 3-tone signal + FFT magnitude spectrum |
| [`fir_plot.png`](data/fir_plot.png) | `plot_signal.gp` | Two-tone input overlaid with 15-tap low-pass FIR output |
| [`conv_plot.png`](data/conv_plot.png) | `plot_convolution.gp` | Impulse train input vs convolved output (16-tap rect pulse) |
| [`corr_plot.png`](data/corr_plot.png) | `plot_correlation.gp` | Autocorrelation of cosine + cross-correlation with delayed copy |
| [`stats_plot.png`](data/stats_plot.png) | `plot_stats.gp` | Noisy sine with running mean and RMS envelope (32-sample window) |
| [`median_plot.png`](data/median_plot.png) | `plot_median.gp` | Salt-and-pepper spikes on sine, cleaned by 5-point median filter |
| [`window_plot.png`](data/window_plot.png) | `plot_window.gp` | Hamming, Hanning, Blackman, Blackman-Harris, Triangle shapes |

## Using the library

Link against `obj/libdsp.a` and include headers from `include/`.
All public types and functions go through the headers.

### Quick example: FFT of a signal

```c
#include "dft.h"
#include "window.h"
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int m = 8;              /* 2^8 = 256 points */
    int n = 1 << m;
    Complex *buf = calloc(n, sizeof(Complex));

    /* fill with test signal */
    for (int i = 0; i < n; i++)
        buf[i].real = /* your samples here */ 0;

    window_hamming(buf, n);     /* apply window */
    fft(buf, m);                /* in-place FFT */

    /* buf now contains the complex spectrum */
    for (int i = 0; i < n / 2; i++) {
        float mag = sqrtf(buf[i].real * buf[i].real
                        + buf[i].imag * buf[i].imag);
        printf("bin %d: magnitude = %f\n", i, mag);
    }

    free(buf);
    return 0;
}
```

Compile with:
```bash
gcc -std=c11 -Iinclude -O2 example.c -Lobj -ldsp -lm -o example
```

### Quick example: Low-pass filtering

```c
#include "filter.h"

int main(void)
{
    /* your FIR coefficients (designed externally) */
    float coefs[] = { 0.1f, 0.2f, 0.4f, 0.2f, 0.1f };
    FirFilter lpf = { .num_taps = 5, .coef = coefs, .history = NULL };

    /* sample-by-sample processing */
    float sample_in = 1.0f;
    float sample_out = fir_filter(sample_in, &lpf);

    fir_filter_free(&lpf);
    return 0;
}
```

## Learning path

If you're working through DSP for the first time, here's a reasonable
order to read the code:

1. **`signal_gen.c`** Start here. Simple cosine/impulse generation.
   Gets you comfortable with sampled signals and normalized frequency.

2. **`signal_ops.c`** Element-wise operations: scale, add, subtract,
   normalize. The building blocks for combining and manipulating signals.

3. **`stats.c`**  Basic signal analysis. Mean, variance, RMS — the
   stuff you need before anything else makes sense.

4. **`dft.c` (DFT only)** Read the `dft()` function first. It's
   the O(N²) brute-force transform, short enough to follow every
   multiplication. Then look at how the twiddle factors work.

5. **`window.c`** Window functions. Understand why we window before
   taking an FFT. Compare Hamming vs Hanning vs Blackman by looking
   at sidelobe levels in the frequency domain.

6. **`dft.c` (FFT)** Now tackle `fft()`. Radix-2 decimation-in-time
   with twiddle factor caching and bit-reversal. This is the core
   algorithm that makes everything else practical.

7. **`convolution.c`** Linear convolution. Understand the sum-of-
   products interpretation and why output is x_len + h_len - 1 long.

8. **`filter.c`** FIR and IIR filtering. Start with `fir_filter()`
   (sample-by-sample) to understand convolution. Then look at the
   IIR biquad cascade for recursive filtering.

9. **`noise.c`** Box-Muller transform for Gaussian noise from
   uniform samples. Short and elegant.

10. **`median.c`** Nonlinear filtering. The conditional median
    filter removes spikes that linear filters can't handle cleanly.

11. **`correlation.c`** Ties it together. Cross-correlation is
    fundamental to signal detection, timing estimation, and
    matched filtering.

The `common/vector_ops.h` macros show a C technique for type-generic
vector operations before `_Generic` existed. The `common/circular_buffer.h`
implements a ring buffer pattern common in real-time audio/DSP systems.
Both worth reading if you're interested in C idioms.

## Origins

Based on DSP routines from Paul Embree's *C Algorithms for Real-Time
DSP* (Prentice Hall). The original code (in `cdsp/`) targeted DOS-era
compilers with K&R C conventions. This version modernizes everything
to C11 while preserving the same algorithms and numerical behavior.

## License

Educational use. See original book for algorithm attribution.

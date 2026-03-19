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
├── common/        Shared macros and utilities
├── test/          Unit tests (one binary per module)
├── obj/           Compiled objects + static library
├── bin/           Test executables
├── cdsp/          Original legacy code (reference)
└── Makefile
```

### Modules

| File | What it does |
|------|-------------|
| `dft.c` | DFT, FFT (radix-2 DIT), inverse FFT, real FFT |
| `window.c` | Hamming, Hanning, Blackman, Blackman-Harris, Triangle |
| `filter.c` | FIR (block + sample-by-sample), IIR (cascaded biquad) |
| `noise.c` | Gaussian (Box-Muller) and uniform RNG |
| `stats.c` | Min, max, mean, variance, RMS |
| `signal_gen.c` | Cosine generation, impulse, sum of sinusoids |
| `median.c` | Conditional median filter |
| `correlation.c` | Cross-correlation and autocorrelation |

## Building

Requires GCC and GNU Make. Tested on Ubuntu but should work on any
Linux/macOS with a C11 compiler.

```bash
# release build (default, -O2)
make

# debug build (symbols, no optimization)
make DEBUG=1

# clean everything
make clean
```

This produces `obj/libdsp.a` (static library) and test binaries in `bin/`.

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

2. **`stats.c`**  Basic signal analysis. Mean, variance, RMS — the
   stuff you need before anything else makes sense.

3. **`dft.c` (DFT only)** Read the `dft()` function first. It's
   the O(N²) brute-force transform, short enough to follow every
   multiplication. Then look at how the twiddle factors work.

4. **`window.c`** Window functions. Understand why we window before
   taking an FFT. Compare Hamming vs Hanning vs Blackman by looking
   at sidelobe levels in the frequency domain.

5. **`dft.c` (FFT)** Now tackle `fft()`. Radix-2 decimation-in-time
   with twiddle factor caching and bit-reversal. This is the core
   algorithm that makes everything else practical.

6. **`filter.c`** FIR and IIR filtering. Start with `fir_filter()`
   (sample-by-sample) to understand convolution. Then look at the
   IIR biquad cascade for recursive filtering.

7. **`noise.c`** Box-Muller transform for Gaussian noise from
   uniform samples. Short and elegant.

8. **`median.c`** Nonlinear filtering. The conditional median
   filter removes spikes that linear filters can't handle cleanly.

9. **`correlation.c`** Ties it together. Cross-correlation is
   fundamental to signal detection, timing estimation, and
   matched filtering.

The `common/vector_ops.h` macros show a C technique for type-generic
vector operations before `_Generic` existed — worth reading if
you're interested in C idioms.

## Origins

Based on DSP routines from Paul Embree's *C Algorithms for Real-Time
DSP* (Prentice Hall). The original code (in `cdsp/`) targeted DOS-era
compilers with K&R C conventions. This version modernizes everything
to C11 while preserving the same algorithms and numerical behavior.

## License

Educational use. See original book for algorithm attribution.

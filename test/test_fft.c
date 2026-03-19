/**
 * @file test_fft.c
 * @brief Tests for FFT, DFT, IFFT, and window functions.
 * @author suman
 * @date 2024/2/28
 */

#include "test_framework.h"
#include "dft.h"
#include "window.h"
#include "signal_gen.h"
#include <stdlib.h>
#include <string.h>

#define N   256
#define M   8       /* 2^8 = 256 */
#define TOL 1e-4

/* -- ilog2 -- */

static void test_ilog2(void)
{
    TEST_BEGIN("ilog2 basic values");
    ASSERT_INT_EQ(ilog2(0), -1);
    ASSERT_INT_EQ(ilog2(1), 0);
    ASSERT_INT_EQ(ilog2(2), 1);
    ASSERT_INT_EQ(ilog2(4), 2);
    ASSERT_INT_EQ(ilog2(8), 3);
    ASSERT_INT_EQ(ilog2(256), 8);
    ASSERT_INT_EQ(ilog2(5), 3);     /* ceil: 2^3 = 8 >= 5 */
    ASSERT_INT_EQ(ilog2(1024), 10);
    TEST_PASS();
}

/* -- FFT of DC signal -- */

static void test_fft_dc(void)
{
    TEST_BEGIN("FFT of DC signal");
    Complex x[N];
    for (int i = 0; i < N; i++) {
        x[i].real = 1.0f;
        x[i].imag = 0.0f;
    }

    DspStatus s = fft(x, M);
    ASSERT_TRUE(s == DSP_SUCCESS);

    /* bin 0 = N, all others ~ 0 */
    ASSERT_NEAR(x[0].real, (float)N, TOL);
    ASSERT_NEAR(x[0].imag, 0.0f, TOL);
    for (int i = 1; i < N; i++) {
        ASSERT_NEAR(x[i].real, 0.0f, TOL);
        ASSERT_NEAR(x[i].imag, 0.0f, TOL);
    }
    TEST_PASS();
}

/* -- FFT / IFFT roundtrip -- */

static void test_fft_ifft_roundtrip(void)
{
    TEST_BEGIN("FFT/IFFT roundtrip");
    Complex orig[N], x[N];

    /* sum of two sinusoids */
    for (int i = 0; i < N; i++) {
        orig[i].real = (float)(cos(TWO_PI * 10.0 * i / N)
                             + 0.5 * cos(TWO_PI * 30.0 * i / N));
        orig[i].imag = 0.0f;
        x[i] = orig[i];
    }

    fft(x, M);
    ifft(x, M);

    for (int i = 0; i < N; i++) {
        ASSERT_NEAR(x[i].real, orig[i].real, TOL);
        ASSERT_NEAR(x[i].imag, orig[i].imag, TOL);
    }
    TEST_PASS();
}

/* -- FFT of impulse = flat spectrum -- */

static void test_fft_impulse(void)
{
    TEST_BEGIN("FFT of impulse");
    Complex x[N];
    memset(x, 0, sizeof(x));
    x[0].real = 1.0f;

    fft(x, M);

    for (int i = 0; i < N; i++) {
        ASSERT_NEAR(x[i].real, 1.0f, TOL);
        ASSERT_NEAR(x[i].imag, 0.0f, TOL);
    }
    TEST_PASS();
}

/* -- DFT vs FFT comparison (small size) -- */

static void test_dft_vs_fft(void)
{
    TEST_BEGIN("DFT matches FFT (16-point)");
    int n = 16;
    Complex in[16], dft_out[16], fft_in[16];

    for (int i = 0; i < n; i++) {
        in[i].real = (float)cos(TWO_PI * 3.0 * i / n);
        in[i].imag = 0.0f;
        fft_in[i] = in[i];
    }

    dft(in, dft_out, n);
    fft(fft_in, 4);

    for (int i = 0; i < n; i++) {
        ASSERT_NEAR(dft_out[i].real, fft_in[i].real, 0.01);
        ASSERT_NEAR(dft_out[i].imag, fft_in[i].imag, 0.01);
    }
    TEST_PASS();
}

/* -- DFT / IDFT roundtrip -- */

static void test_dft_idft_roundtrip(void)
{
    TEST_BEGIN("DFT/IDFT roundtrip (16-point)");
    int n = 16;
    Complex orig[16], freq[16], recov[16];

    for (int i = 0; i < n; i++) {
        orig[i].real = (float)sin(TWO_PI * 2.0 * i / n);
        orig[i].imag = 0.0f;
    }

    dft(orig, freq, n);
    idft(freq, recov, n);

    for (int i = 0; i < n; i++) {
        ASSERT_NEAR(recov[i].real, orig[i].real, 0.01);
        ASSERT_NEAR(recov[i].imag, orig[i].imag, 0.01);
    }
    TEST_PASS();
}

/* -- Hamming window endpoints -- */

static void test_window_hamming(void)
{
    TEST_BEGIN("Hamming window endpoints and center");
    Complex x[64];
    for (int i = 0; i < 64; i++) { x[i].real = 1.0f; x[i].imag = 0.0f; }

    window_hamming(x, 64);

    /* Hamming endpoints = 0.08 (not zero) */
    ASSERT_NEAR(x[0].real, 0.08f, 0.01);
    ASSERT_NEAR(x[63].real, 0.08f, 0.01);
    /* center ~ 1.0 */
    ASSERT_NEAR(x[32].real, 1.0f, 0.02);
    TEST_PASS();
}

/* -- Hanning window zero endpoints -- */

static void test_window_hanning(void)
{
    TEST_BEGIN("Hanning window zero endpoints");
    Complex x[64];
    for (int i = 0; i < 64; i++) { x[i].real = 1.0f; x[i].imag = 0.0f; }

    window_hanning(x, 64);

    ASSERT_NEAR(x[0].real, 0.0f, TOL);
    ASSERT_NEAR(x[63].real, 0.0f, TOL);
    TEST_PASS();
}

/* -- window_generate consistency -- */

static void test_window_generate(void)
{
    TEST_BEGIN("window_generate matches in-place");
    float coefs[64];
    Complex x[64];

    window_generate(coefs, 64, WIN_BLACKMAN);
    for (int i = 0; i < 64; i++) { x[i].real = 1.0f; x[i].imag = 0.0f; }
    window_blackman(x, 64);

    for (int i = 0; i < 64; i++)
        ASSERT_NEAR(coefs[i], x[i].real, 1e-6);
    TEST_PASS();
}

int main(void)
{
    TEST_SUITE_BEGIN("FFT / DFT / Window Tests");

    test_ilog2();
    test_fft_dc();
    test_fft_ifft_roundtrip();
    test_fft_impulse();
    test_dft_vs_fft();
    test_dft_idft_roundtrip();
    test_window_hamming();
    test_window_hanning();
    test_window_generate();

    TEST_SUITE_END();
}

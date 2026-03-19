/**
 * @file test_signal.c
 * @brief Tests for signal generation, noise, and correlation.
 * @author suman
 * @date 2024/2/28
 */

#include "test_framework.h"
#include "signal_gen.h"
#include "noise.h"
#include "correlation.h"
#include "stats.h"
#include <stdlib.h>
#include <string.h>

#define TOL 1e-4f
#define N   1024

/* -- cosine generator: known values at quarter-period -- */

static void test_cosine_gen(void)
{
    TEST_BEGIN("Cosine generation (f=0.25)");
    float out[N];
    signal_gen_cosine(out, N, 0.25f, 0.0f, 1.0f);

    /* period = 4 samples at f/fs = 0.25 */
    ASSERT_NEAR(out[0],  1.0f, TOL);
    ASSERT_NEAR(out[1],  0.0f, TOL);
    ASSERT_NEAR(out[2], -1.0f, TOL);
    ASSERT_NEAR(out[3],  0.0f, TOL);
    TEST_PASS();
}

/* -- impulse generation -- */

static void test_impulse_gen(void)
{
    TEST_BEGIN("Impulse generation");
    float out[64];
    signal_gen_impulse(out, 64, 10);

    ASSERT_NEAR(out[0],  0.0f, TOL);
    ASSERT_NEAR(out[10], 1.0f, TOL);
    ASSERT_NEAR(out[11], 0.0f, TOL);
    TEST_PASS();
}

/* -- sum of cosines stays bounded -- */

static void test_cosine_sum(void)
{
    TEST_BEGIN("Sum of cosines normalization");
    float freqs[] = { 0.1f, 0.2f };
    float out[N];
    signal_gen_cosines(out, N, freqs, 2);

    float mx = stats_max(out, N);
    float mn = stats_min(out, N);
    ASSERT_TRUE(mx <=  1.01f);
    ASSERT_TRUE(mn >= -1.01f);
    TEST_PASS();
}

/* -- error handling -- */

static void test_signal_errors(void)
{
    TEST_BEGIN("Signal gen error returns");
    float out[16];
    ASSERT_TRUE(signal_gen_cosine(NULL, 10, 0.1f, 0, 1) == DSP_ERR_PARAM);
    ASSERT_TRUE(signal_gen_impulse(out, 16, -1) == DSP_ERR_PARAM);
    ASSERT_TRUE(signal_gen_impulse(out, 16, 16) == DSP_ERR_PARAM);
    TEST_PASS();
}

/* -- Gaussian noise mean near zero -- */

static void test_noise_gaussian(void)
{
    TEST_BEGIN("Gaussian noise mean (N=10000)");
    double sum = 0;
    int count = 10000;
    for (int i = 0; i < count; i++)
        sum += noise_gaussian();

    double mean = sum / count;
    ASSERT_NEAR(mean, 0.0, 0.1);
    TEST_PASS();
}

/* -- uniform noise range -- */

static void test_noise_uniform(void)
{
    TEST_BEGIN("Uniform noise in [-0.5, 0.5]");
    double mn = 1.0, mx = -1.0;
    for (int i = 0; i < 10000; i++) {
        double v = noise_uniform();
        if (v < mn) mn = v;
        if (v > mx) mx = v;
    }

    ASSERT_TRUE(mn >= -0.5);
    ASSERT_TRUE(mx <=  0.5);
    ASSERT_TRUE(mx >  0.3);    /* should cover most of the range */
    ASSERT_TRUE(mn < -0.3);
    TEST_PASS();
}

/* -- autocorrelation peak at lag 0 -- */

static void test_autocorrelation(void)
{
    TEST_BEGIN("Autocorrelation peak at zero lag");
    float sig[N];
    signal_gen_cosine(sig, N, 0.1f, 0.0f, 1.0f);

    int max_lag = 10;
    int out_len = 2 * max_lag + 1;
    float result[21];

    auto_correlate(sig, result, N, -max_lag, max_lag);

    /* peak at lag 0 (index = max_lag in result array) */
    float peak = result[max_lag];
    ASSERT_TRUE(peak > 0.0f);
    for (int i = 0; i < out_len; i++)
        ASSERT_TRUE(result[i] <= peak + TOL);
    TEST_PASS();
}

/* -- cross-correlation of shifted copies -- */

static void test_cross_correlation(void)
{
    TEST_BEGIN("Cross-correlation peak at known shift");

    /* create x and y where y is x shifted by 5 samples */
    float x[128], y[128];
    memset(x, 0, sizeof(x));
    memset(y, 0, sizeof(y));
    for (int i = 10; i < 30; i++) x[i] = 1.0f;
    for (int i = 15; i < 35; i++) y[i] = 1.0f;  /* shifted by +5 */

    float result[11]; /* lags 0..10 */
    cross_correlate(x, y, result, 100, 0, 10);

    /* peak should be at lag 5 */
    int peak_lag = 0;
    float peak_val = result[0];
    for (int d = 1; d <= 10; d++) {
        if (result[d] > peak_val) {
            peak_val = result[d];
            peak_lag = d;
        }
    }
    ASSERT_INT_EQ(peak_lag, 5);
    TEST_PASS();
}

int main(void)
{
    TEST_SUITE_BEGIN("Signal / Noise / Correlation Tests");

    test_cosine_gen();
    test_impulse_gen();
    test_cosine_sum();
    test_signal_errors();
    test_noise_gaussian();
    test_noise_uniform();
    test_autocorrelation();
    test_cross_correlation();

    TEST_SUITE_END();
}

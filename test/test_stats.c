/**
 * @file test_stats.c
 * @brief Tests for statistics and median filter.
 * @author suman
 * @date 2024/2/28
 */

#include "test_framework.h"
#include "stats.h"
#include "median.h"
#include <string.h>

#define TOL 1e-5f

/* -- basic statistical measures -- */

static void test_stats_basic(void)
{
    TEST_BEGIN("Basic statistics (min/max/mean/var)");
    float data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

    ASSERT_NEAR(stats_min(data, 5),      1.0f, TOL);
    ASSERT_NEAR(stats_max(data, 5),      5.0f, TOL);
    ASSERT_NEAR(stats_mean(data, 5),     3.0f, TOL);
    ASSERT_NEAR(stats_variance(data, 5), 2.5f, TOL);
    TEST_PASS();
}

/* -- constant signal: zero variance -- */

static void test_stats_constant(void)
{
    TEST_BEGIN("Constant signal has zero variance");
    float data[] = { 7.0f, 7.0f, 7.0f, 7.0f };

    ASSERT_NEAR(stats_min(data, 4),      7.0f, TOL);
    ASSERT_NEAR(stats_max(data, 4),      7.0f, TOL);
    ASSERT_NEAR(stats_mean(data, 4),     7.0f, TOL);
    ASSERT_NEAR(stats_variance(data, 4), 0.0f, TOL);
    TEST_PASS();
}

/* -- RMS of a sinusoid ~ 1/sqrt(2) -- */

static void test_stats_rms(void)
{
    TEST_BEGIN("RMS of unit sine wave");
    float data[1000];
    for (int i = 0; i < 1000; i++)
        data[i] = (float)sin(2.0 * M_PI * i / 1000);

    float rms = stats_rms(data, 1000);
    ASSERT_NEAR(rms, 1.0f / sqrtf(2.0f), 0.002f);
    TEST_PASS();
}

/* -- median filter removes spike -- */

static void test_median_spike(void)
{
    TEST_BEGIN("Median filter spike removal");
    float in[11]  = { 1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1 };
    float out[11] = { 0 };

    DspStatus s = median_filter(in, out, 11, 3, 0.0f);
    ASSERT_TRUE(s == DSP_SUCCESS);

    /* spike replaced */
    ASSERT_NEAR(out[5], 1.0f, TOL);
    /* edges preserved */
    ASSERT_NEAR(out[0], 1.0f, TOL);
    ASSERT_NEAR(out[10], 1.0f, TOL);
    TEST_PASS();
}

/* -- conditional median: small deviation kept -- */

static void test_median_conditional(void)
{
    TEST_BEGIN("Conditional median threshold");
    float in[11]  = { 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1 };
    float out[11] = { 0 };

    /* threshold=5 keeps small deviations */
    median_filter(in, out, 11, 3, 5.0f);
    ASSERT_NEAR(out[5], 2.0f, TOL);

    /* threshold=0 replaces everything */
    median_filter(in, out, 11, 3, 0.0f);
    ASSERT_NEAR(out[5], 1.0f, TOL);
    TEST_PASS();
}

/* -- negative values -- */

static void test_stats_negative(void)
{
    TEST_BEGIN("Stats with negative values");
    float data[] = { -5.0f, -1.0f, 0.0f, 3.0f, 8.0f };

    ASSERT_NEAR(stats_min(data, 5),  -5.0f, TOL);
    ASSERT_NEAR(stats_max(data, 5),   8.0f, TOL);
    ASSERT_NEAR(stats_mean(data, 5),  1.0f, TOL);
    TEST_PASS();
}

int main(void)
{
    TEST_SUITE_BEGIN("Statistics / Median Tests");

    test_stats_basic();
    test_stats_constant();
    test_stats_rms();
    test_stats_negative();
    test_median_spike();
    test_median_conditional();

    TEST_SUITE_END();
}

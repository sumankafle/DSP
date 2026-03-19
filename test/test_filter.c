/**
 * @file test_filter.c
 * @brief Tests for FIR and IIR filter functions.
 * @author suman
 * @date 2024/2/28
 */

#include "test_framework.h"
#include "filter.h"
#include "signal_gen.h"
#include <stdlib.h>
#include <string.h>

#define TOL      1e-4f
#define SIG_LEN  256

/* asymmetric 3-tap filter for impulse response validation */
static float test_coef[3] = { 0.2f, 0.5f, 0.3f };

/* -- FIR impulse response should match coefficients -- */

static void test_fir_impulse_response(void)
{
    TEST_BEGIN("FIR impulse response (3-tap)");

    FirFilter fir = { .num_taps = 3, .coef = test_coef, .history = NULL };

    float out[3];
    out[0] = fir_filter(1.0f, &fir);
    out[1] = fir_filter(0.0f, &fir);
    out[2] = fir_filter(0.0f, &fir);

    /* h[n] = coef[n] */
    ASSERT_NEAR(out[0], test_coef[0], TOL);
    ASSERT_NEAR(out[1], test_coef[1], TOL);
    ASSERT_NEAR(out[2], test_coef[2], TOL);

    fir_filter_free(&fir);
    TEST_PASS();
}

/* -- DC through averaging filter should pass unchanged -- */

static float avg3_coef[3] = { 1.0f / 3, 1.0f / 3, 1.0f / 3 };

static void test_fir_dc_passthrough(void)
{
    TEST_BEGIN("FIR DC passthrough (3-tap avg)");

    FirFilter fir = { .num_taps = 3, .coef = avg3_coef, .history = NULL };

    float out = 0;
    for (int i = 0; i < 20; i++)
        out = fir_filter(1.0f, &fir);

    ASSERT_NEAR(out, 1.0f, TOL);

    fir_filter_free(&fir);
    TEST_PASS();
}

/* -- 35-tap lowpass: pass low freq, attenuate high freq -- */

static float lpf35_coef[35] = {
    -6.849167e-003f,  1.949014e-003f,  1.309874e-002f,  1.100677e-002f,
    -6.661435e-003f, -1.321869e-002f,  6.819504e-003f,  2.292400e-002f,
     7.732160e-004f, -3.153488e-002f, -1.384843e-002f,  4.054618e-002f,
     3.841148e-002f, -4.790497e-002f, -8.973017e-002f,  5.285565e-002f,
     3.126515e-001f,  4.454146e-001f,  3.126515e-001f,  5.285565e-002f,
    -8.973017e-002f, -4.790497e-002f,  3.841148e-002f,  4.054618e-002f,
    -1.384843e-002f, -3.153488e-002f,  7.732160e-004f,  2.292400e-002f,
     6.819504e-003f, -1.321869e-002f, -6.661435e-003f,  1.100677e-002f,
     1.309874e-002f,  1.949014e-003f, -6.849167e-003f
};

static void test_fir_lowpass(void)
{
    TEST_BEGIN("FIR lowpass attenuation check");

    FirFilter fir = { .num_taps = 35, .coef = lpf35_coef, .history = NULL };

    float lo_in[SIG_LEN], hi_in[SIG_LEN];
    float lo_out[SIG_LEN], hi_out[SIG_LEN];

    /* 0.1*fs should pass, 0.4*fs should be attenuated */
    signal_gen_cosine(lo_in, SIG_LEN, 0.1f, 0.0f, 1.0f);
    signal_gen_cosine(hi_in, SIG_LEN, 0.4f, 0.0f, 1.0f);

    fir_filter_array(lo_in, lo_out, SIG_LEN, &fir);
    fir_filter_array(hi_in, hi_out, SIG_LEN, &fir);

    /* compare power in the steady-state middle region */
    float lo_power = 0, hi_power = 0;
    for (int i = SIG_LEN / 4; i < 3 * SIG_LEN / 4; i++) {
        lo_power += lo_out[i] * lo_out[i];
        hi_power += hi_out[i] * hi_out[i];
    }

    ASSERT_TRUE(lo_power > 10.0f * hi_power);
    TEST_PASS();
}

/* -- FIR integer filter basic check -- */

static void test_fir_int(void)
{
    TEST_BEGIN("FIR integer filter");

    int in[8]  = { 100, 200, 300, 400, 500, 600, 700, 800 };
    int out[8] = { 0 };
    int coef[3] = { 1, 2, 1 };

    fir_filter_int(in, out, 8, coef, 3, 4);

    /* output should be a smoothed version, all values reasonable */
    for (int i = 0; i < 8; i++)
        ASSERT_TRUE(out[i] >= 0 && out[i] <= 1000);
    TEST_PASS();
}

/* -- IIR lowpass DC gain ~ 1.0 -- */

static float iir_lpf5_coef[13] = {
     0.0552961603f,
    -0.4363630712f,  0.0000000000f,  1.0000000000f,  0.0000000000f,
    -0.5233039260f,  0.8604439497f,  0.7039934993f,  1.0000000000f,
    -0.6965782046f,  0.4860509932f, -0.0103216320f,  1.0000000000f
};

static void test_iir_dc_gain(void)
{
    TEST_BEGIN("IIR elliptic LPF DC gain");

    IirFilter iir = { .num_sections = 3, .coef = iir_lpf5_coef,
                       .history = NULL };

    float out = 0;
    for (int i = 0; i < 300; i++)
        out = iir_filter(1.0f, &iir);

    /* DC gain should be ~1.0 for this lowpass filter */
    ASSERT_NEAR(out, 1.0f, 0.05f);

    iir_filter_free(&iir);
    TEST_PASS();
}

/* -- IIR highpass should block DC -- */

static float iir_hpf6_coef[13] = {
     0.0025892381f,
     0.5913599133f,  0.8879900575f, -2.0000000000f,  1.0000000000f,
     0.9156184793f,  0.6796731949f, -2.0000000000f,  1.0000000000f,
     1.3316441774f,  0.5193183422f, -2.0000000000f,  1.0000000000f
};

static void test_iir_highpass(void)
{
    TEST_BEGIN("IIR Chebyshev HPF blocks DC");

    IirFilter iir = { .num_sections = 3, .coef = iir_hpf6_coef,
                       .history = NULL };

    float out = 0;
    for (int i = 0; i < 300; i++)
        out = iir_filter(1.0f, &iir);

    /* HPF should reject DC */
    ASSERT_NEAR(out, 0.0f, 0.05f);

    iir_filter_free(&iir);
    TEST_PASS();
}

int main(void)
{
    TEST_SUITE_BEGIN("Filter Tests");

    test_fir_impulse_response();
    test_fir_dc_passthrough();
    test_fir_lowpass();
    test_fir_int();
    test_iir_dc_gain();
    test_iir_highpass();

    TEST_SUITE_END();
}

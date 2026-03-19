/**
 * @file filter.h
 * @brief FIR and IIR digital filter structures and functions.
 * @author suman
 * @date 2024/2/28
 */

#ifndef FILTER_H
#define FILTER_H

#include "dsp_types.h"

/**
 * @brief FIR filter state.
 *
 * Holds coefficients and delay line for sample-by-sample
 * or block-based FIR filtering.
 */
typedef struct {
    int num_taps;       /**< Number of filter coefficients */
    float *coef;        /**< Coefficient array [num_taps] */
    float *history;     /**< Delay line [num_taps - 1], allocated on first use */
} FirFilter;

/**
 * @brief IIR filter state (cascaded biquad sections).
 *
 * Direct form II second-order sections.
 * Coefficient layout: [gain, a1_0, a2_0, b1_0, b2_0, a1_1, a2_1, ...].
 */
typedef struct {
    int num_sections;   /**< Number of second-order sections */
    float *coef;        /**< Coefficient array [4 * num_sections + 1] */
    float *history;     /**< State buffer [2 * num_sections], allocated on first use */
} IirFilter;

/**
 * @brief Filter a float array using FIR convolution.
 *
 * Center-aligned filtering with edge handling.
 * Input and output arrays are the same length.
 *
 * @param[in]  in   Input signal array.
 * @param[out] out  Output signal array (same length as in).
 * @param[in]  len  Length of input/output arrays.
 * @param[in]  fir  FIR filter with coefficients set.
 */
void fir_filter_array(const float *in, float *out, int len, const FirFilter *fir);

/**
 * @brief FIR filter a single sample (real-time operation).
 *
 * Processes one input sample and returns one output sample.
 * History buffer is allocated and updated on each call.
 *
 * @param[in]     input  New input sample.
 * @param[in,out] fir    FIR filter state.
 * @return Filtered output sample.
 */
float fir_filter(float input, FirFilter *fir);

/**
 * @brief Filter integer arrays using FIR convolution.
 *
 * @param[in]  in        Input integer array.
 * @param[out] out       Output integer array.
 * @param[in]  len       Array length.
 * @param[in]  coef      Integer coefficient array.
 * @param[in]  coef_len  Number of coefficients.
 * @param[in]  scale     Divisor applied after accumulation.
 */
void fir_filter_int(const int *in, int *out, int len,
                    const int *coef, int coef_len, int scale);

/**
 * @brief IIR filter a single sample (cascaded biquad sections).
 *
 * @param[in]     input  New input sample.
 * @param[in,out] iir    IIR filter state.
 * @return Filtered output sample.
 */
float iir_filter(float input, IirFilter *iir);

/**
 * @brief Free FIR filter history buffer.
 *
 * Does not free the coefficient array (assumed externally owned).
 *
 * @param[in,out] fir  FIR filter to clean up. NULL-safe.
 */
void fir_filter_free(FirFilter *fir);

/** @brief Free IIR filter state buffer. NULL-safe. */
void iir_filter_free(IirFilter *iir);

#endif /* FILTER_H */

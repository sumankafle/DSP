/**
 * @file dft.h
 * @brief FFT, DFT, and related transform declarations.
 * @author suman
 * @date 2024/2/28
 */

#ifndef DFT_H
#define DFT_H

#include "dsp_types.h"

/**
 * @brief Discrete Fourier Transform (O(N^2)).
 *
 * Computes the N-point DFT of a complex input sequence.
 * Twiddle factors are cached internally -- reused if N stays the same.
 *
 * @param[in]  in  Input complex array (N elements).
 * @param[out] out Output complex array (N elements). Must not alias @p in.
 * @param[in]  n   Transform size.
 * @return DSP_SUCCESS on success, DSP_ERR_ALLOC on memory failure.
 *
 * @note Not thread-safe (uses static coefficient cache).
 */
DspStatus dft(const Complex *in, Complex *out, int n);

/**
 * @brief Inverse Discrete Fourier Transform (O(N^2)).
 *
 * Output is scaled by 1/N.
 *
 * @param[in]  in  Input complex array (N elements).
 * @param[out] out Output complex array (N elements). Must not alias @p in.
 * @param[in]  n   Transform size.
 * @return DSP_SUCCESS on success, DSP_ERR_ALLOC on memory failure.
 *
 * @note Not thread-safe.
 */
DspStatus idft(const Complex *in, Complex *out, int n);

/**
 * @brief In-place radix-2 decimation-in-time FFT.
 *
 * Computes the FFT of 2^m complex samples in-place.
 * Twiddle factors are cached and reused across calls with the same m.
 *
 * @param[in,out] x  Complex array of 2^m elements.
 * @param[in]     m  Log2 of the transform size (N = 2^m).
 * @return DSP_SUCCESS or DSP_ERR_ALLOC.
 *
 * @note Not thread-safe.
 */
DspStatus fft(Complex *x, int m);

/**
 * @brief In-place radix-2 inverse FFT.
 *
 * Output is scaled by 1/N.
 *
 * @param[in,out] x  Complex array of 2^m elements.
 * @param[in]     m  Log2 of the transform size.
 * @return DSP_SUCCESS or DSP_ERR_ALLOC.
 *
 * @note Not thread-safe.
 */
DspStatus ifft(Complex *x, int m);

/**
 * @brief Real-input FFT via trig recombination.
 *
 * Takes N = 2^m real samples and produces the lower N/2
 * complex spectral bins.
 *
 * @param[in]  x  Real input array (N = 2^m elements).
 *                Contents are destroyed (used as workspace).
 * @param[out] y  Complex output array (at least N/2 elements).
 * @param[in]  m  Log2 of the real sequence length.
 * @return DSP_SUCCESS or DSP_ERR_ALLOC.
 *
 * @note x is reinterpreted as Complex internally; ensure proper alignment.
 * @note Not thread-safe.
 */
DspStatus rfft(float *x, Complex *y, int m);

/**
 * @brief Integer base-2 logarithm (ceiling).
 *
 * Returns the smallest k such that 2^k >= x.
 *
 * @param[in] x Unsigned integer value.
 * @return Ceiling of log2(x), or -1 if x == 0.
 */
int ilog2(unsigned int x);

#endif /* DFT_H */

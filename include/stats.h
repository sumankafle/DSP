/**
 * @file stats.h
 * @brief Statistical functions for signal analysis.
 * @author suman
 * @date 2024/2/28
 */

#ifndef STATS_H
#define STATS_H

/**
 * @brief Find the minimum value in a float array.
 * @param[in] data  Input array.
 * @param[in] len   Number of elements.
 * @return Minimum value.
 */
float stats_min(const float *data, int len);

/**
 * @brief Find the maximum value in a float array.
 * @param[in] data  Input array.
 * @param[in] len   Number of elements.
 * @return Maximum value.
 */
float stats_max(const float *data, int len);

/**
 * @brief Compute the arithmetic mean.
 * @param[in] data  Input array.
 * @param[in] len   Number of elements.
 * @return Arithmetic mean.
 */
float stats_mean(const float *data, int len);

/**
 * @brief Compute the sample variance (unbiased, N-1 denominator).
 * @param[in] data  Input array.
 * @param[in] len   Number of elements (must be >= 2).
 * @return Sample variance.
 */
float stats_variance(const float *data, int len);

/**
 * @brief Compute the RMS (root-mean-square) value.
 * @param[in] data  Input array.
 * @param[in] len   Number of elements.
 * @return RMS value.
 */
float stats_rms(const float *data, int len);

#endif /* STATS_H */

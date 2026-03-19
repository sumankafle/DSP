/**
 * @file noise.h
 * @brief Random noise generator functions.
 * @author suman
 * @date 2024/2/28
 */

#ifndef NOISE_H
#define NOISE_H

/**
 * @brief Generate a zero-mean unit-variance Gaussian random number.
 *
 * Uses the Box-Muller transform of two uniform samples.
 *
 * @return Gaussian random value (double).
 *
 * @note Not thread-safe (caches one value per pair).
 */
double noise_gaussian(void);

/**
 * @brief Generate a zero-mean uniform random number in [-0.5, 0.5].
 *
 * @return Uniform random value (double).
 */
double noise_uniform(void);

#endif /* NOISE_H */

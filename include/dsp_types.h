/**
 * @file dsp_types.h
 * @brief Core type definitions and common macros for the DSP library.
 * @author suman
 * @date 2024/2/28
 *
 * Provides the fundamental Complex type, status codes, and utility
 * macros shared across all library modules.
 */

#ifndef DSP_TYPES_H
#define DSP_TYPES_H

#include <math.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TWO_PI (2.0 * M_PI)

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define ROUND(x) (((x) < 0) ? (int)((x) - 0.5) : (int)((x) + 0.5))

/**
 * @brief Complex number (single-precision).
 *
 * Standard representation for frequency-domain data and
 * complex arithmetic in FFT/DFT computations.
 */
typedef struct {
    float real;
    float imag;
} Complex;

/**
 * @brief Status codes returned by DSP library functions.
 */
typedef enum {
    DSP_SUCCESS       =  0,
    DSP_ERR_ALLOC     = -1,
    DSP_ERR_PARAM     = -2,
    DSP_ERR_OVERFLOW  = -3
} DspStatus;

#endif /* DSP_TYPES_H */

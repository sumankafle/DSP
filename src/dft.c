/**
 * @file dft.c
 * @brief DFT, FFT, IFFT, and RFFT implementations.
 * @author suman
 * @date 2024/2/28
 *
 * Radix-2 decimation-in-time FFT with twiddle factor caching.
 * Also includes the brute-force O(N^2) DFT for reference and
 * small-N use, plus a real-input FFT using trig recombination.
 */

#include "dft.h"
#include <stdlib.h>
#include <math.h>

/*--------------------------------------------------------------------
 * DFT (brute force, O(N^2))
 *-------------------------------------------------------------------*/

DspStatus dft(const Complex *in, Complex *out, int n)
{
    static int nstore = 0;
    static Complex *cf = NULL;

    if (n <= 0 || !in || !out)
        return DSP_ERR_PARAM;

    /* recompute twiddle factors when size changes */
    if (n != nstore) {
        free(cf);
        cf = calloc(n, sizeof(Complex));
        if (!cf) return DSP_ERR_ALLOC;

        double angle = TWO_PI / n;
        for (int i = 0; i < n; i++) {
            cf[i].real = (float)cos(angle * i);
            cf[i].imag = (float)(-sin(angle * i));
        }
        nstore = n;
    }

    for (int k = 0; k < n; k++) {
        out[k].real = in[0].real;
        out[k].imag = in[0].imag;

        for (int i = 1; i < n; i++) {
            int p = (int)((long)i * k % n);
            out[k].real += in[i].real * cf[p].real
                         - in[i].imag * cf[p].imag;
            out[k].imag += in[i].real * cf[p].imag
                         + in[i].imag * cf[p].real;
        }
    }

    return DSP_SUCCESS;
}

/*--------------------------------------------------------------------
 * Inverse DFT (brute force, O(N^2))
 *-------------------------------------------------------------------*/

DspStatus idft(const Complex *in, Complex *out, int n)
{
    static int nstore = 0;
    static Complex *cf = NULL;

    if (n <= 0 || !in || !out)
        return DSP_ERR_PARAM;

    if (n != nstore) {
        free(cf);
        cf = calloc(n, sizeof(Complex));
        if (!cf) return DSP_ERR_ALLOC;

        double angle = TWO_PI / n;
        double inv_n = 1.0 / n;
        for (int i = 0; i < n; i++) {
            cf[i].real = (float)(cos(angle * i) * inv_n);
            cf[i].imag = (float)(sin(angle * i) * inv_n);
        }
        nstore = n;
    }

    for (int k = 0; k < n; k++) {
        out[k].real = in[0].real * cf[0].real;
        out[k].imag = in[0].imag * cf[0].real;

        for (int i = 1; i < n; i++) {
            int p = (int)((long)i * k % n);
            out[k].real += in[i].real * cf[p].real
                         - in[i].imag * cf[p].imag;
            out[k].imag += in[i].real * cf[p].imag
                         + in[i].imag * cf[p].real;
        }
    }

    return DSP_SUCCESS;
}

/*--------------------------------------------------------------------
 * Radix-2 DIT FFT (in-place)
 *
 * Twiddle factors W_N^k are computed recursively on first call
 * (or when m changes) and cached in static storage.
 *-------------------------------------------------------------------*/

DspStatus fft(Complex *x, int m)
{
    static Complex *w = NULL;
    static int mstore = 0;
    static int n = 1;

    if (m < 0 || !x)
        return DSP_ERR_PARAM;
    if (m == 0)
        return DSP_SUCCESS;

    /* recompute twiddle factors if transform size changed */
    if (m != mstore) {
        free(w);
        mstore = m;
        n = 1 << m;
        int half = n / 2;

        w = calloc(half - 1, sizeof(Complex));
        if (!w) return DSP_ERR_ALLOC;

        /* W_N = exp(-j*2*pi/N), computed recursively for accuracy */
        double arg = M_PI / half;
        double wr = cos(arg);
        double wi = -sin(arg);
        double wr_acc = wr, wi_acc = wi;

        for (int j = 0; j < half - 1; j++) {
            w[j].real = (float)wr_acc;
            w[j].imag = (float)wi_acc;
            double tmp = wr_acc * wr - wi_acc * wi;
            wi_acc = wr_acc * wi + wi_acc * wr;
            wr_acc = tmp;
        }
    }

    /* butterfly passes */
    int le = n;
    int windex = 1;

    for (int l = 0; l < m; l++) {
        le /= 2;

        /* first butterfly per group: twiddle = 1 (no multiply) */
        for (int i = 0; i < n; i += 2 * le) {
            Complex *xi  = x + i;
            Complex *xip = xi + le;
            Complex t;
            t.real = xi->real + xip->real;
            t.imag = xi->imag + xip->imag;
            xip->real = xi->real - xip->real;
            xip->imag = xi->imag - xip->imag;
            *xi = t;
        }

        /* remaining butterflies with stored twiddle factors */
        Complex *wptr = w + windex - 1;
        for (int j = 1; j < le; j++) {
            Complex u = *wptr;
            for (int i = j; i < n; i += 2 * le) {
                Complex *xi  = x + i;
                Complex *xip = xi + le;
                Complex t, tm;

                t.real = xi->real + xip->real;
                t.imag = xi->imag + xip->imag;
                tm.real = xi->real - xip->real;
                tm.imag = xi->imag - xip->imag;

                xip->real = tm.real * u.real - tm.imag * u.imag;
                xip->imag = tm.real * u.imag + tm.imag * u.real;
                *xi = t;
            }
            wptr += windex;
        }
        windex *= 2;
    }

    /* bit-reversal permutation */
    int j = 0;
    for (int i = 1; i < n - 1; i++) {
        int k = n / 2;
        while (k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
        if (i < j) {
            Complex t = x[j];
            x[j] = x[i];
            x[i] = t;
        }
    }

    return DSP_SUCCESS;
}

/*--------------------------------------------------------------------
 * Radix-2 DIT Inverse FFT (in-place)
 *
 * Same structure as the forward FFT but with conjugate twiddle
 * factors and 1/N scaling at the end.
 *-------------------------------------------------------------------*/

DspStatus ifft(Complex *x, int m)
{
    static Complex *w = NULL;
    static int mstore = 0;
    static int n = 1;

    if (m < 0 || !x)
        return DSP_ERR_PARAM;
    if (m == 0)
        return DSP_SUCCESS;

    if (m != mstore) {
        free(w);
        mstore = m;
        n = 1 << m;
        int half = n / 2;

        w = calloc(half - 1, sizeof(Complex));
        if (!w) return DSP_ERR_ALLOC;

        /* conjugate twiddle: positive imaginary for inverse */
        double arg = M_PI / half;
        double wr = cos(arg);
        double wi = sin(arg);
        double wr_acc = wr, wi_acc = wi;

        for (int j = 0; j < half - 1; j++) {
            w[j].real = (float)wr_acc;
            w[j].imag = (float)wi_acc;
            double tmp = wr_acc * wr - wi_acc * wi;
            wi_acc = wr_acc * wi + wi_acc * wr;
            wr_acc = tmp;
        }
    }

    /* butterfly passes */
    int le = n;
    int windex = 1;

    for (int l = 0; l < m; l++) {
        le /= 2;

        for (int i = 0; i < n; i += 2 * le) {
            Complex *xi  = x + i;
            Complex *xip = xi + le;
            Complex t;
            t.real = xi->real + xip->real;
            t.imag = xi->imag + xip->imag;
            xip->real = xi->real - xip->real;
            xip->imag = xi->imag - xip->imag;
            *xi = t;
        }

        Complex *wptr = w + windex - 1;
        for (int j = 1; j < le; j++) {
            Complex u = *wptr;
            for (int i = j; i < n; i += 2 * le) {
                Complex *xi  = x + i;
                Complex *xip = xi + le;
                Complex t, tm;

                t.real = xi->real + xip->real;
                t.imag = xi->imag + xip->imag;
                tm.real = xi->real - xip->real;
                tm.imag = xi->imag - xip->imag;

                xip->real = tm.real * u.real - tm.imag * u.imag;
                xip->imag = tm.real * u.imag + tm.imag * u.real;
                *xi = t;
            }
            wptr += windex;
        }
        windex *= 2;
    }

    /* bit reversal */
    int j = 0;
    for (int i = 1; i < n - 1; i++) {
        int k = n / 2;
        while (k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
        if (i < j) {
            Complex t = x[j];
            x[j] = x[i];
            x[i] = t;
        }
    }

    /* scale by 1/N */
    float scale = 1.0f / n;
    for (int i = 0; i < n; i++) {
        x[i].real *= scale;
        x[i].imag *= scale;
    }

    return DSP_SUCCESS;
}

/*--------------------------------------------------------------------
 * Real-input FFT via trig recombination
 *
 * Performs an N/2 complex FFT on the real data (reinterpreted as
 * N/2 complex pairs), then recombines using trig identities to
 * produce N/2 spectral bins.
 *-------------------------------------------------------------------*/

DspStatus rfft(float *x, Complex *y, int m)
{
    static Complex *cf = NULL;
    static int mstore = 0;

    if (m <= 0 || !x || !y)
        return DSP_ERR_PARAM;

    int p = m - 1;
    int num = 1 << p; /* N/2: half the real sequence length */

    /* half-size complex FFT on real data reinterpreted as complex */
    Complex *cx = (Complex *)x;
    DspStatus st = fft(cx, p);
    if (st != DSP_SUCCESS) return st;

    /* compute recombination coefficients if needed */
    if (m != mstore) {
        free(cf);
        cf = calloc(num - 1, sizeof(Complex));
        if (!cf) return DSP_ERR_ALLOC;

        double factor = M_PI / num;
        for (int k = 1; k < num; k++) {
            double arg = factor * k;
            cf[k - 1].real = (float)cos(arg);
            cf[k - 1].imag = (float)sin(arg);
        }
        mstore = m;
    }

    /* DC bin: no twiddle multiply needed */
    y[0].real = cx[0].real + cx[0].imag;
    y[0].imag = 0.0f;

    /* recombine remaining bins */
    for (int k = 1; k < num; k++) {
        float rs = (cx[k].real + cx[num - k].real) * 0.5f;
        float is = (cx[k].imag + cx[num - k].imag) * 0.5f;
        float rd = (cx[k].real - cx[num - k].real) * 0.5f;
        float id = (cx[k].imag - cx[num - k].imag) * 0.5f;

        y[k].real = rs + cf[k - 1].real * is - cf[k - 1].imag * rd;
        y[k].imag = id - cf[k - 1].imag * is - cf[k - 1].real * rd;
    }

    return DSP_SUCCESS;
}

/*--------------------------------------------------------------------
 * Integer base-2 logarithm (ceiling)
 *-------------------------------------------------------------------*/

int ilog2(unsigned int x)
{
    if (x == 0) return -1;

    unsigned int v = x - 1;
    int bits = 0;
    while (v > 0) {
        v >>= 1;
        bits++;
    }
    return bits;
}

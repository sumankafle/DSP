# Discrete Fourier Transform and FFT in C
   
This **DFT.c** includes:

* Discrete Fourier Transform (DFT) and Inverse DFT (IDFT)
* In-place radix-2 Decimation-in-Time FFT and IFFT
* Real-input FFT using trigonometric recombination
* Common window functions used in DSP
* Utility function for base-2 logarithm

The code is designed for **for clarity and learning not for optimization of FFT** 
---

## Features Overview

| Category           | Functions                                 |
| ------------------ | ----------------------------------------- |
| Fourier Transforms | `dft`, `idft`, `fft`, `ifft`, `rfft`      |
| Window Functions   | `ham`, `han`, `triang`, `black`, `harris` |
| Utilities          | `log2`                                    |

All FFT-based routines operate on a custom `COMPLEX` data structure defined in `dft.h`.

---

## 📂 Expected Data Types

The code assumes the following complex type:

```c
// See dft.h
typedef struct {
    float real;
    float imag;
} COMPLEX;
```

---

##  Discrete Fourier Transform (DFT)

### `void dft(COMPLEX *Datain, COMPLEX *Dataout, int N)`

Performs a **direct N-point DFT** using the mathematical definition:

[
X[k] = \sum_{n=0}^{N-1} x[n] e^{-j 2\pi kn/N}
]

#### Implementation details:

* Time complexity: **O(N²)**
* Precomputes and caches twiddle factors
* Uses modulo indexing for phase reuse
* Suitable for small N or reference validation

---

### `void idft(COMPLEX *Datain, COMPLEX *Dataout, int N)`

Computes the **inverse DFT**, including scaling by `1/N`.

[
x[n] = \frac{1}{N} \sum_{k=0}^{N-1} X[k] e^{+j 2\pi kn/N}
]

---

##  Fast Fourier Transform (FFT)

### `void fft(COMPLEX *x, int m)`

Performs an **in-place radix-2 Decimation-in-Time FFT**.

* FFT size: `N = 2^m`
* Input overwritten by frequency-domain output
* Uses iterative butterfly stages
* Bit-reversal performed at the end

#### Key concepts implemented:

* Butterfly operations
* Twiddle factor recursion
* Cached sine/cosine tables
* O(N log₂ N) complexity

---

### `void ifft(COMPLEX *x, int m)`

Inverse FFT implementation:

* Uses conjugate twiddle factors
* Performs identical butterfly structure as FFT
* Final output scaled by `1/N`

These are the implementations based on lecture at university

---

##  Real FFT (Trig Recombination)

### `void rfft(float *x, COMPLEX *y, int m)`

Efficient FFT for **real-valued input signals**.

#### How it works:

1. Treats real input as complex
2. Computes half-size FFT (`2^(m-1)`)
3. Applies trigonometric recombination
4. Outputs only `N/2 + 1` unique frequency bins

This is commonly used in **audio, radar, and vibration analysis**.

---

##  Window Functions

Windowing reduces spectral leakage before FFT processing.

All window functions **scale both real and imaginary parts in-place**.

| Function | Window Type            |
| -------- | ---------------------- |
| `ham`    | Hamming                |
| `han`    | Hanning (Hann)         |
| `triang` | Triangular             |
| `black`  | Blackman               |
| `harris` | 4-term Blackman–Harris |

Example usage:

```c
ham(signal, N);
fft(signal, log2(N));
```

---

##  Utility Function

### `int log2(unsigned int x)`

Returns the smallest integer `m` such that:

[
2^m \ge x
]

Used to determine FFT stage count from array size.

---

##  Design Notes

* Static memory caching improves repeated FFT calls
* No external dependencies beyond `math.h`
* ANSI C compatible

---

##  Example Usage

```c
int N = 1024;
int m = log2(N);
COMPLEX x[N];

// Fill x with samples
han(x, N);
fft(x, m);
```

---

##  Limitations

* Radix-2 only (FFT size must be power of 2)
* No overflow protection


---

This implementation closely follows classic DSP textbook approaches to FFT, DFT, and windowing theory.

---



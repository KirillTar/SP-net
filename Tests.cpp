#include "Tests.hpp"
#include <cmath>
#include <complex>
#include <algorithm>

constexpr double M_PI = 3.14159265358979323846;   // pi

// ******************   autocorrelation   ******************

size_t next_power_of_two(size_t n) {
    size_t pow2 = 1;
    while (pow2 < n) {
        pow2 <<= 1;
    }
    return pow2;
}

// Cooley-Tukey FFT algorithm
void fft(const std::vector<std::complex<double>>& input, std::vector<std::complex<double>>& output, bool inverse = false) {
    size_t N = input.size();
    output = input;

    // Bit reversal permutation
    size_t bits = 0;
    while ((1U << bits) < N) {
        ++bits;
    }

    for (size_t i = 0; i < N; ++i) {
        size_t j = 0;
        for (size_t k = 0; k < bits; ++k) {
            if (i & (1U << k)) {
                j |= 1U << (bits - 1 - k);
            }
        }
        if (i < j) {
            std::swap(output[i], output[j]);
        }
    }

    // FFT computation
    for (size_t len = 2; len <= N; len <<= 1) {
        double angle = 2 * M_PI / len * (inverse ? 1 : -1);
        std::complex<double> wlen(std::cos(angle), std::sin(angle));

        for (size_t i = 0; i < N; i += len) {
            std::complex<double> w(1);
            for (size_t j = 0; j < len / 2; ++j) {
                std::complex<double> u = output[i + j];
                std::complex<double> v = output[i + j + len / 2] * w;
                output[i + j] = u + v;
                output[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    // Scaling for inverse transform
    if (inverse) {
        for (size_t i = 0; i < N; ++i) {
            output[i] /= N;
        }
    }
}

std::vector<double> autocorrelation_test(const std::vector<uchar>& data) {
    size_t N = data.size();

    size_t n = next_power_of_two(2 * N - 1);

    double mean = std::accumulate(data.begin(), data.end(), 0.0) / N;
    std::vector<std::complex<double>> x;
    x.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        x.emplace_back(data[i] - mean);
    }

    for (size_t i = N; i < n; ++i) {
        x.emplace_back(0.0);
    }

    std::vector<std::complex<double>> X;
    fft(x, X);

    // Compute the power spectrum (element-wise square of magnitude)
    std::vector<std::complex<double>> S(n);
    for (size_t i = 0; i < n; ++i) {
        S[i] = X[i] * std::conj(X[i]);
    }

    // Compute the inverse FFT of the power spectrum
    std::vector<std::complex<double>> R_complex;
    fft(S, R_complex, true); // inverse = true

    // Extract the real part of the autocorrelation function
    std::vector<double> R(N);
    for (size_t i = 0; i < N; ++i) {
        R[i] = R_complex[i].real();
    }

    // Normalize the ACF so that R[0] = 1
    double R0 = R[0];
    if (R0 != 0.0) {
        for (size_t i = 0; i < N; ++i) {
            R[i] /= R0;
        }
    }

    return R;
}

// ******************   autocorrelation end   *************


// ******************   histogramm  ***********************

std::vector<double> histogramm_test(const std::vector<uchar>& data) {
    std::vector<double> vec(256,0);
    for (auto num : data) {
        vec[num]++;
    }
    return vec;
}

// ******************   histogramm end   ******************


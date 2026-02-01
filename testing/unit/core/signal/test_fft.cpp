#include "../../../../src/core/signal/DSPKernel.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool float_eq(float a, float b, float epsilon = 1e-4) {
    return std::abs(a - b) < epsilon;
}

void test_fft_impulse() {
    std::cout << "Running FFT Impulse Test..." << std::endl;
    // Impulse at t=0 -> FFT is constant 1
    size_t size = 8;
    std::vector<float> real(size, 0.0f);
    std::vector<float> imag(size, 0.0f);
    real[0] = 1.0f;

    minni::signal::DSPKernel::fft(real.data(), imag.data(), size, false);

    for (size_t i = 0; i < size; ++i) {
        assert(float_eq(real[i], 1.0f));
        assert(float_eq(imag[i], 0.0f));
    }
    std::cout << "FFT Impulse Test Passed!" << std::endl;
}

void test_ifft_impulse() {
    std::cout << "Running IFFT Impulse Test..." << std::endl;
    // Constant 1 in freq domain -> Impulse at t=0
    size_t size = 8;
    std::vector<float> real(size, 1.0f);
    std::vector<float> imag(size, 0.0f);

    minni::signal::DSPKernel::fft(real.data(), imag.data(), size, true);

    assert(float_eq(real[0], 1.0f));
    assert(float_eq(imag[0], 0.0f));

    for (size_t i = 1; i < size; ++i) {
        assert(float_eq(real[i], 0.0f));
        assert(float_eq(imag[i], 0.0f));
    }
    std::cout << "IFFT Impulse Test Passed!" << std::endl;
}

void test_fft_sine() {
    std::cout << "Running FFT Sine Test..." << std::endl;
    // Sine wave: x[n] = cos(2*pi*k*n/N) where k=1
    // Should result in peaks at k=1 and k=N-1
    size_t size = 16;
    std::vector<float> real(size);
    std::vector<float> imag(size, 0.0f);

    for (size_t i = 0; i < size; ++i) {
        real[i] = std::cos(2.0f * M_PI * 1.0f * i / size);
    }

    minni::signal::DSPKernel::fft(real.data(), imag.data(), size, false);

    // Expected: Real part at index 1 and 15 should be size/2 (8.0), others 0
    assert(float_eq(real[1], 8.0f));
    assert(float_eq(real[size - 1], 8.0f));

    // Check DC is 0
    assert(float_eq(real[0], 0.0f));

    std::cout << "FFT Sine Test Passed!" << std::endl;
}

int main() {
    test_fft_impulse();
    test_ifft_impulse();
    test_fft_sine();
    return 0;
}

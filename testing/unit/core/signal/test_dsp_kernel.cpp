#include "../../../../src/core/signal/DSPKernel.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

bool approx_equal(float a, float b, float epsilon = 0.001f) {
    return std::abs(a - b) < epsilon;
}

void test_complex_magnitude() {
    std::cout << "Running Complex Magnitude Test..." << std::endl;

    // 3-4-5 triangles
    std::vector<float> real = {3.0f, 6.0f, 0.0f, -3.0f};
    std::vector<float> imag = {4.0f, 8.0f, 5.0f, 4.0f};
    std::vector<float> output(4);

    minni::signal::DSPKernel::complex_magnitude(real.data(), imag.data(), output.data(), 4);

    assert(approx_equal(output[0], 5.0f));
    assert(approx_equal(output[1], 10.0f));
    assert(approx_equal(output[2], 5.0f));
    assert(approx_equal(output[3], 5.0f));

    std::cout << "Complex Magnitude Test Passed!" << std::endl;
}

void test_fir_filter() {
    std::cout << "Running FIR Filter Test..." << std::endl;

    // Identity filter (impulse)
    std::vector<float> input = {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<float> taps = {1.0f, 0.0f}; // Should just pass input
    std::vector<float> output(4);

    minni::signal::DSPKernel::fir_filter(input.data(), 4, taps.data(), 2, output.data());

    assert(approx_equal(output[0], 1.0f));
    assert(approx_equal(output[1], 2.0f));
    assert(approx_equal(output[2], 3.0f));
    assert(approx_equal(output[3], 4.0f));

    // Simple moving average (0.5, 0.5)
    std::vector<float> avg_taps = {0.5f, 0.5f};
    minni::signal::DSPKernel::fir_filter(input.data(), 4, avg_taps.data(), 2, output.data());

    // out[0] = in[0]*0.5 = 0.5
    // out[1] = in[1]*0.5 + in[0]*0.5 = 1.0 + 0.5 = 1.5
    // out[2] = in[2]*0.5 + in[1]*0.5 = 1.5 + 1.0 = 2.5
    // out[3] = in[3]*0.5 + in[2]*0.5 = 2.0 + 1.5 = 3.5

    assert(approx_equal(output[0], 0.5f));
    assert(approx_equal(output[1], 1.5f));
    assert(approx_equal(output[2], 2.5f));
    assert(approx_equal(output[3], 3.5f));

    std::cout << "FIR Filter Test Passed!" << std::endl;
}

int main() {
    test_complex_magnitude();
    test_fir_filter();
    return 0;
}

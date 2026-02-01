#include "../../../../src/core/signal/SignalProcessor.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

// Helper
bool vec_approx_eq(const std::vector<float>& a, const std::vector<float>& b, float epsilon = 1e-3) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::abs(a[i] - b[i]) > epsilon) return false;
    }
    return true;
}

void test_processor_chain() {
    std::cout << "Running SignalProcessor Chain Test..." << std::endl;

    minni::signal::SignalProcessor processor;

    // 1. Load DC signal [1, 1, 1, 1]
    std::vector<float> input = {1.0f, 1.0f, 1.0f, 1.0f};
    processor.load(input);

    // 2. FFT -> Should be [4, 0, 0, 0] in Real part (DC component = sum)
    processor.fft(false);

    // 3. Magnitude -> sqrt(4^2 + 0) = 4, others 0
    processor.magnitude();

    std::vector<float> output = processor.get_output();
    std::vector<float> expected = {4.0f, 0.0f, 0.0f, 0.0f};

    assert(vec_approx_eq(output, expected));

    std::cout << "SignalProcessor Chain Test Passed!" << std::endl;
}

void test_processor_window() {
    std::cout << "Running SignalProcessor Window Test..." << std::endl;

    minni::signal::SignalProcessor processor;
    std::vector<float> input = {1.0f, 1.0f, 1.0f, 1.0f};
    std::vector<float> window = {0.5f, 0.5f, 0.5f, 0.5f};

    processor.load(input);
    processor.apply_window(window);

    std::vector<float> expected = {0.5f, 0.5f, 0.5f, 0.5f};
    assert(vec_approx_eq(processor.get_output(), expected));

    std::cout << "SignalProcessor Window Test Passed!" << std::endl;
}

int main() {
    test_processor_chain();
    test_processor_window();
    return 0;
}

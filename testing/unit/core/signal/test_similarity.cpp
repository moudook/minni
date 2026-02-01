#include "../../../../src/core/signal/DSPKernel.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

bool float_eq(float a, float b, float epsilon = 1e-4) {
    return std::abs(a - b) < epsilon;
}

void test_dot_product() {
    std::cout << "Running Dot Product Test..." << std::endl;
    std::vector<float> a = {1.0f, 2.0f, 3.0f};
    std::vector<float> b = {4.0f, 5.0f, 6.0f};
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    float res = minni::signal::DSPKernel::dot_product(a.data(), b.data(), a.size());
    assert(float_eq(res, 32.0f));
    std::cout << "Dot Product Test Passed!" << std::endl;
}

void test_cosine_similarity() {
    std::cout << "Running Cosine Similarity Test..." << std::endl;
    // Orthogonal vectors
    std::vector<float> a = {1.0f, 0.0f};
    std::vector<float> b = {0.0f, 1.0f};
    float sim = minni::signal::DSPKernel::cosine_similarity(a.data(), b.data(), a.size());
    assert(float_eq(sim, 0.0f));

    // Parallel vectors
    std::vector<float> c = {1.0f, 1.0f};
    std::vector<float> d = {2.0f, 2.0f};
    sim = minni::signal::DSPKernel::cosine_similarity(c.data(), d.data(), c.size());
    assert(float_eq(sim, 1.0f));

    std::cout << "Cosine Similarity Test Passed!" << std::endl;
}

int main() {
    test_dot_product();
    test_cosine_similarity();
    return 0;
}

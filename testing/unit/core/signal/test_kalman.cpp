#include "../../../../src/core/signal/KalmanFilter.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

void test_kalman_constant() {
    std::cout << "Running Kalman Constant Test..." << std::endl;
    // Expecting a constant value of 10.0, with noise
    minni::signal::KalmanFilter kf(2.0f, 1.0f, 0.01f);
    kf.reset(10.0f);

    std::vector<float> measurements = {10.2f, 9.8f, 10.1f, 9.9f, 10.0f};
    float last_est = 10.0f;

    for (float m : measurements) {
        float est = kf.update(m);
        // The estimate should stay close to 10
        assert(std::abs(est - 10.0f) < 0.5f);
        last_est = est;
    }
    std::cout << "Kalman Constant Test Passed!" << std::endl;
}

void test_kalman_convergence() {
    std::cout << "Running Kalman Convergence Test..." << std::endl;
    // Start at 0, true value is 100
    minni::signal::KalmanFilter kf(2.0f, 2.0f, 0.1f);
    kf.reset(0.0f);

    for (int i = 0; i < 50; ++i) {
        kf.update(100.0f); // Input clean 100
    }

    // Should have converged close to 100
    assert(std::abs(kf.get_estimate() - 100.0f) < 1.0f);
    std::cout << "Kalman Convergence Test Passed!" << std::endl;
}

int main() {
    test_kalman_constant();
    test_kalman_convergence();
    return 0;
}

#include "KalmanFilter.h"
#include <cmath>

namespace minni {
namespace signal {

KalmanFilter::KalmanFilter(float measurement_uncertainty, float estimation_uncertainty, float process_noise)
    : err_measure_(measurement_uncertainty),
      err_estimate_(estimation_uncertainty),
      q_(process_noise) {
}

KalmanFilter::~KalmanFilter() = default;

float KalmanFilter::update(float measurement) {
    kalman_gain_ = err_estimate_ / (err_estimate_ + err_measure_);
    current_estimate_ = last_estimate_ + kalman_gain_ * (measurement - last_estimate_);
    err_estimate_ = (1.0f - kalman_gain_) * err_estimate_ + std::abs(last_estimate_ - current_estimate_) * q_;
    last_estimate_ = current_estimate_;

    return current_estimate_;
}

float KalmanFilter::get_estimate() const {
    return current_estimate_;
}

float KalmanFilter::get_error_estimate() const {
    return err_estimate_;
}

void KalmanFilter::reset(float initial_value) {
    current_estimate_ = initial_value;
    last_estimate_ = initial_value;
    // We might want to reset uncertainties too, but usually parameters stay fixed
}

} // namespace signal
} // namespace minni

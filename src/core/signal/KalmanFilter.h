#ifndef MINNI_CORE_SIGNAL_KALMAN_FILTER_H_
#define MINNI_CORE_SIGNAL_KALMAN_FILTER_H_

namespace minni {
namespace signal {

/**
 * A simplified 1D Kalman Filter implementation.
 * Useful for smoothing noisy 1D sensor data (e.g. RSSI, distance, single axis acceleration).
 */
class KalmanFilter {
public:
    /**
     * @param measurement_uncertainty How much noise we expect in the measurement
     * @param estimation_uncertainty How much noise we expect in the system/process
     * @param process_noise How fast the value changes
     */
    KalmanFilter(float measurement_uncertainty, float estimation_uncertainty, float process_noise);
    ~KalmanFilter();

    /**
     * Update the filter with a new measurement.
     * @param measurement The raw sensor value
     * @return The filtered estimate
     */
    float update(float measurement);

    /**
     * Get the current estimate.
     */
    float get_estimate() const;

    /**
     * Get the current estimation uncertainty (error covariance).
     */
    float get_error_estimate() const;

    /**
     * Reset the filter state.
     */
    void reset(float initial_value = 0.0f);

private:
    float err_measure_;
    float err_estimate_;
    float q_; // Process noise variance

    float current_estimate_ = 0.0f;
    float last_estimate_ = 0.0f;
    float kalman_gain_ = 0.0f;
};

} // namespace signal
} // namespace minni

#endif // MINNI_CORE_SIGNAL_KALMAN_FILTER_H_

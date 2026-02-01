package com.minni.framework.core;

/**
 * Java wrapper for the native C++ Kalman Filter.
 * Used for smoothing 1D sensor data (e.g. RSSI, distance).
 */
public class KalmanFilter {
    private long nativeHandle;

    static {
        System.loadLibrary("minni_core");
    }

    /**
     * @param measurementUncertainty How much noise we expect in the measurement
     * @param estimationUncertainty How much noise we expect in the system/process
     * @param processNoise How fast the value changes
     */
    public KalmanFilter(float measurementUncertainty, float estimationUncertainty, float processNoise) {
        nativeInit(measurementUncertainty, estimationUncertainty, processNoise);
    }

    public void close() {
        nativeFree();
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            close();
        } finally {
            super.finalize();
        }
    }

    /**
     * Update the filter with a new measurement.
     * @param measurement The raw sensor value
     * @return The filtered estimate
     */
    public float update(float measurement) {
        return nativeUpdate(measurement);
    }

    /**
     * Get the current estimate.
     */
    public float getEstimate() {
        return nativeGetEstimate();
    }

    /**
     * Reset the filter state.
     */
    public void reset(float initialValue) {
        nativeReset(initialValue);
    }

    // Native methods
    private native void nativeInit(float measurementUncertainty, float estimationUncertainty, float processNoise);
    private native void nativeFree();
    private native float nativeUpdate(float measurement);
    private native float nativeGetEstimate();
    private native void nativeReset(float initialValue);
}

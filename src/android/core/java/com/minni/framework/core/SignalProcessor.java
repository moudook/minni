package com.minni.framework.core;

/**
 * High-level signal processor that chains DSP operations.
 * State is maintained in native memory to reduce JNI copying.
 */
public class SignalProcessor {
    private long nativeHandle;

    static {
        System.loadLibrary("minni_core");
    }

    public SignalProcessor() {
        nativeInit();
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
     * Load data into the processor. Resets state.
     */
    public void load(float[] data) {
        nativeLoad(data);
    }

    /**
     * Apply a window function (e.g., Hanning).
     * Window size must match data size.
     */
    public void applyWindow(float[] window) {
        nativeApplyWindow(window);
    }

    /**
     * Perform in-place FFT.
     */
    public void fft(boolean inverse) {
        nativeFft(inverse);
    }

    /**
     * Compute magnitude of the current complex signal.
     * Result replaces the real part; imaginary part becomes 0.
     */
    public void magnitude() {
        nativeMagnitude();
    }

    /**
     * Retrieve current processing result (real part).
     */
    public float[] getOutput() {
        return nativeGetOutput();
    }

    // Native methods
    private native void nativeInit();
    private native void nativeFree();
    private native void nativeLoad(float[] data);
    private native void nativeApplyWindow(float[] window);
    private native void nativeFft(boolean inverse);
    private native void nativeMagnitude();
    private native float[] nativeGetOutput();
}

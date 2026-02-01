package com.minni.framework.core;

/**
 * Java wrapper for stateless DSP functions.
 */
public class DSPKernel {
    static {
        System.loadLibrary("minni_core");
    }

    /**
     * Computes magnitude of complex vectors.
     * output[i] = sqrt(real[i]^2 + imag[i]^2)
     */
    public static native void complexMagnitude(float[] real, float[] imag, float[] output);

    /**
     * In-place Radix-2 FFT (Fast Fourier Transform).
     * Size must be a power of 2.
     * Real and imaginary parts are stored in separate arrays.
     *
     * @param real Real components (input/output)
     * @param imag Imaginary components (input/output)
     * @param inverse If true, perform Inverse FFT (IFFT)
     */
    public static native void fft(float[] real, float[] imag, boolean inverse);

    /**
     * Computes the Dot Product of two vectors.
     * sum(a[i] * b[i])
     */
    public static native float dotProduct(float[] a, float[] b);

    /**
     * Computes Cosine Similarity between two vectors.
     * dot(a, b) / (norm(a) * norm(b))
     */
    public static native float cosineSimilarity(float[] a, float[] b);

    /**
     * Calculates the root mean square (RMS) of a signal.
     * Helper Java-side implementation that might call native kernels later.
     */
    public static float calculateRMS(float[] signal) {
        float sum = 0;
        for (float v : signal) {
            sum += v * v;
        }
        return (float) Math.sqrt(sum / signal.length);
    }
}

package com.minni.framework.genai;

import java.io.Closeable;

/**
 * Java wrapper for the native C++ ModelRunner (LiteRT).
 * Handles loading and running TFLite models.
 */
public class ModelRunner implements Closeable {
    static {
        System.loadLibrary("minni_core");
    }

    private long nativeHandle;

    public ModelRunner() {
        nativeInit();
    }

    @Override
    public void close() {
        if (nativeHandle != 0) {
            nativeFree();
        }
    }

    /**
     * Load a TFLite model.
     * @param modelPath Path to .tflite file.
     * @param numThreads Number of threads to use.
     * @return true if successful.
     */
    public boolean loadModel(String modelPath, int numThreads) {
        return nativeLoadModel(modelPath, numThreads);
    }

    /**
     * Run inference.
     * @param inputs Array of input Tensors.
     * @return Array of output Tensors.
     */
    public Tensor[] predict(Tensor[] inputs) {
        return nativePredict(inputs);
    }

    // Native declarations
    private native void nativeInit();
    private native void nativeFree();
    private native boolean nativeLoadModel(String path, int numThreads);
    private native Tensor[] nativePredict(Tensor[] inputs);
}

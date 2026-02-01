package com.minni.framework.genai;

import java.util.Arrays;

/**
 * Represents a Tensor for GenAI operations.
 * Currently supports float32 data.
 */
public class Tensor {
    public String name;
    public int[] shape;
    public float[] data;

    public Tensor(String name, int[] shape, float[] data) {
        this.name = name;
        this.shape = shape;
        this.data = data;
    }

    public int flatSize() {
        if (shape == null || shape.length == 0) return 0;
        int size = 1;
        for (int dim : shape) {
            size *= dim;
        }
        return size;
    }

    @Override
    public String toString() {
        return "Tensor{" +
                "name='" + name + '\'' +
                ", shape=" + Arrays.toString(shape) +
                ", data_len=" + (data != null ? data.length : 0) +
                '}';
    }
}

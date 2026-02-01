package com.minni.framework.core;

import java.util.List;
import java.util.ArrayList;

/**
 * Java wrapper for the native C++ VectorStore.
 * Allows storing and retrieving high-dimensional vectors (embeddings)
 * using cosine similarity search.
 */
public class VectorStore {
    private long nativeHandle;

    static {
        System.loadLibrary("minni_core");
    }

    public VectorStore() {
        this(false);
    }

    /**
     * @param useQuantization If true, vectors are stored as 8-bit integers to save memory.
     */
    public VectorStore(boolean useQuantization) {
        nativeInit(useQuantization);
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
     * Add a vector to the store.
     * @param id Unique identifier.
     * @param vector The embedding float array.
     * @return true if successful.
     */
    public boolean addVector(String id, float[] vector) {
        return nativeAddVector(id, vector);
    }

    /**
     * Search for nearest neighbors.
     * @param query Query vector.
     * @param limit Max results.
     * @return List of SearchResult objects.
     */
    public List<SearchResult> search(float[] query, int limit) {
        return nativeSearch(query, limit);
    }

    /**
     * Get number of stored vectors.
     */
    public int size() {
        return nativeGetSize();
    }

    /**
     * Clear the store.
     */
    public void clear() {
        nativeClear();
    }

    /**
     * Save the store to a binary file.
     * @param path File path.
     * @return true if successful.
     */
    public boolean save(String path) {
        return nativeSave(path, null);
    }

    /**
     * Save the store to an encrypted binary file.
     * @param path File path.
     * @param encryptionKey Encryption key.
     * @return true if successful.
     */
    public boolean save(String path, String encryptionKey) {
        return nativeSave(path, encryptionKey);
    }

    /**
     * Load the store from a binary file.
     * @param path File path.
     * @return true if successful.
     */
    public boolean load(String path) {
        return nativeLoad(path, null);
    }

    /**
     * Load the store from an encrypted binary file.
     * @param path File path.
     * @param encryptionKey Encryption key.
     * @return true if successful.
     */
    public boolean load(String path, String encryptionKey) {
        return nativeLoad(path, encryptionKey);
    }

    // Result helper class
    public static class SearchResult {
        public final String id;
        public final float score;

        public SearchResult(String id, float score) {
            this.id = id;
            this.score = score;
        }

        @Override
        public String toString() {
            return id + " (" + String.format("%.3f", score) + ")";
        }
    }

    // Native methods
    private native void nativeInit(boolean useQuantization);
    private native void nativeFree();
    private native boolean nativeAddVector(String id, float[] vector);
    private native List<SearchResult> nativeSearch(float[] query, int limit);
    private native int nativeGetSize();
    private native void nativeClear();
    private native boolean nativeSave(String path, String encryptionKey);
    private native boolean nativeLoad(String path, String encryptionKey);
}

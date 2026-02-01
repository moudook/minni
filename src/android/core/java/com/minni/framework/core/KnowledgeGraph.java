package com.minni.framework.core;

import java.io.Closeable;

/**
 * Java wrapper for the C++ KnowledgeGraph.
 * Stores the native pointer in 'nativeHandle'.
 */
public class KnowledgeGraph implements Closeable {
    static {
        System.loadLibrary("minni_core");
    }

    private long nativeHandle;

    public KnowledgeGraph() {
        this(false);
    }

    /**
     * @param useQuantization If true, embeddings are stored as 8-bit integers to save memory.
     */
    public KnowledgeGraph(boolean useQuantization) {
        nativeInit(useQuantization);
    }

    @Override
    public void close() {
        if (nativeHandle != 0) {
            nativeFree();
        }
    }

    public void addFact(String subject, String predicate, String object) {
        if (subject == null || predicate == null || object == null) {
            throw new IllegalArgumentException("Arguments cannot be null");
        }
        nativeAddFact(subject, predicate, object);
    }

    public int getNumFacts() {
        return nativeGetNumFacts();
    }

    /**
     * Save the graph to a binary file.
     * @param path File path.
     * @return true if successful.
     */
    public boolean save(String path) {
        return nativeSave(path, null);
    }

    /**
     * Save the graph to an encrypted binary file.
     * @param path File path.
     * @param encryptionKey Encryption key.
     * @return true if successful.
     */
    public boolean save(String path, String encryptionKey) {
        return nativeSave(path, encryptionKey);
    }

    /**
     * Load the graph from a binary file.
     * @param path File path.
     * @return true if successful.
     */
    public boolean load(String path) {
        return nativeLoad(path, null);
    }

    /**
     * Load the graph from an encrypted binary file.
     * @param path File path.
     * @param encryptionKey Encryption key.
     * @return true if successful.
     */
    public boolean load(String path, String encryptionKey) {
        return nativeLoad(path, encryptionKey);
    }

    // Native declarations
    private native void nativeInit(boolean useQuantization);
    private native void nativeFree();
    private native void nativeAddFact(String sub, String pred, String obj);
    private native int nativeGetNumFacts();
    private native boolean nativeSave(String path, String encryptionKey);
    private native boolean nativeLoad(String path, String encryptionKey);
}

package com.minni.framework.core;

import java.util.Map;

/**
 * Java wrapper for the native C++ RuleEngine.
 * Executes logic rules exported from decision trees.
 */
public class RuleEngine {
    // Pointer to C++ object
    private long nativeHandle;

    static {
        System.loadLibrary("minni_core");
    }

    public RuleEngine() {
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
     * Load rules from a string (e.g. read from a file).
     * Format: "IF (var <= val) THEN result=val"
     */
    public boolean loadRules(String rulesContent) {
        return nativeLoadRules(rulesContent);
    }

    /**
     * Evaluate the rules against a set of inputs.
     * @param inputs Map of variable names to values (Double or String).
     * @return The result string if a rule matches, or null.
     */
    public String evaluate(Map<String, Object> inputs) {
        return nativeEvaluate(inputs);
    }

    // Native methods
    private native void nativeInit();
    private native void nativeFree();
    private native boolean nativeLoadRules(String rulesContent);
    private native String nativeEvaluate(Map<String, Object> inputs);
}

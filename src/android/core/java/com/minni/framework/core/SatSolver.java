package com.minni.framework.core;

import java.io.Closeable;
import java.util.Map;

/**
 * Java wrapper for the C++ SatSolver.
 * Provides an interface to solve boolean formulas (CNF).
 */
public class SatSolver implements Closeable {
    static {
        System.loadLibrary("minni_core");
    }

    private long nativeHandle;

    public SatSolver() {
        nativeInit();
    }

    @Override
    public void close() {
        if (nativeHandle != 0) {
            nativeFree();
        }
    }

    /**
     * Check satisfiability of a formula string.
     * Example: "(A | B) & (!A | C)"
     * @return true if SAT, false if UNSAT
     */
    public boolean check(String formula) {
        if (formula == null) return false;
        return checkNative(formula);
    }

    /**
     * Get the model assignment if the last check was SAT.
     * @return Map of Variable Name -> "true"/"false"
     */
    public Map<String, String> getModel() {
        return getModelNative();
    }

    // Native declarations
    private native void nativeInit();
    private native void nativeFree();
    private native boolean checkNative(String formula);
    private native Map<String, String> getModelNative();
}

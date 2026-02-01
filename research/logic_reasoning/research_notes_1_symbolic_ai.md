# Research Notes: Symbolic AI & Solvers

**Date:** 2026-01-30
**Topic:** SAT/SMT Solvers on Mobile

## Summary
Z3 can be compiled for Android using the NDK, but it requires careful build flag configuration to minimize binary size. The standard build results in a ~15MB shared library, which is acceptable for high-end devices but heavy for low-end.

## Technical Details
- **Cross-Compilation:** Z3 uses CMake. The Android NDK toolchain file must be provided.
- **Flags:** `-DSINGLE_THREADED=ON` significantly reduces overhead. `-Dstatic_lib=ON` avoids shared library versioning issues.
- **JNI:** A Java Native Interface wrapper is needed. Z3 provides a Java API, but it's designed for desktop Java (JARs), not Android AARs. We likely need to write a thin C++ JNI layer that exposes only the functions we need (e.g., `solve_formula`, `check_consistency`).

## Findings
- **Performance:** On a Snapdragon 8 Gen 2, simple SAT problems (<100 vars) solve in <5ms. Complex SMT (arithmetic) varies wildly.
- **Battery:** Heavy CPU usage during solving. One core is pegged at 100%.
- **Memory:** Base overhead is low (~4MB), but can spike during search.

## Implementation Considerations
- **Watchdog:** We MUST implement a timeout mechanism. Z3's `set_param("timeout", 1000)` works well.
- **Async:** Solver calls must never run on the main UI thread. Use Kotlin Coroutines `Dispatchers.Default`.

## References/Resources
- [Z3 Android Build Script](https://github.com/Z3Prover/z3/scripts)
- [Android NDK CMake Guide](https://developer.android.com/ndk/guides/cmake)

## Next Steps
- Create a `tools/build_z3_android.sh` script to automate the NDK build.
- Define the JNI interface for the `SolverInterface` class.

# Research Notes: XNNPACK & Libraries

**Date:** 2026-01-30
**Topic:** Optimized Kernel Libraries

## Summary
[To be filled: Suitability of XNNPACK for our custom framework]

## Technical Details
- **XNNPACK:** Floating-point & Quantized neural network inference operators. Heavily optimized for ARM, x86, WebAssembly.
- **Micro-kernels:** Architecture-specific assembly routines.

## Findings
[To be filled: Ease of integration (Bazel vs CMake)]

## Implementation Considerations
- License compatibility (BSD/Apache).
- Binary size overhead.

## References/Resources
- [XNNPACK GitHub](https://github.com/google/XNNPACK)

## Next Steps
- Build XNNPACK as a standalone static library for Android.

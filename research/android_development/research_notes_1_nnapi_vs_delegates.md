# Research Notes: NNAPI vs Custom Delegates

**Date:** 2026-01-30
**Topic:** Android Neural Networks API (NNAPI) compared to Custom Delegates (TFLite/PyTorch)

## Summary
[To be filled: Brief summary of performance/stability trade-offs between native NNAPI and framework-specific delegates]

## Technical Details
- **NNAPI:** Native Android C API for hardware acceleration.
- **GPU Delegate:** Uses OpenGL ES or OpenCL.
- **Hexagon Delegate:** specific for Qualcomm DSPs.
- **XNNPACK:** Highly optimized CPU implementation (often the baseline).

## Findings
[To be filled: Documentation of stability on different OEMs (Samsung, Pixel, Xiaomi)]

## Implementation Considerations
- How to implement a fallback mechanism (Hardware -> CPU).
- Driver version compatibility checks.

## References/Resources
- [Android NNAPI Documentation](https://developer.android.com/ndk/guides/neuralnetworks)
- [TFLite Delegates](https://www.tensorflow.org/lite/performance/delegates)

## Next Steps
- Benchmark specific models on NNAPI vs CPU.

# Research Notes: SIMD Acceleration

**Date:** 2026-01-30
**Topic:** ARM NEON and SVE Optimization

## Summary
[To be filled: Impact of NEON instructions on inference latency]

## Technical Details
- **NEON:** 128-bit wide registers (Q-registers).
- **SVE:** Variable vector length (SVE/SVE2) on newer ARM cores.
- **Intrinsics:** Using `<arm_neon.h>` vs inline assembly.

## Findings
[To be filled: Benchmark comparison of Scalar vs NEON for 4x4 Matrix Multiply]

## Implementation Considerations
- Detecting SVE support at runtime (to avoid crashing on older phones).
- Compiler flags (`-march=armv8-a+simd`).

## References/Resources
- [ARM NEON Programmer's Guide](https://developer.arm.com/documentation/den0018/a/)

## Next Steps
- Write a simple benchmark kernel in C++ (JNI).

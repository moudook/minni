# Research Notes: DSP on Android

**Date:** 2026-01-30
**Topic:** Hardware DSP vs CPU SIMD

## Summary
Accessing the Hexagon DSP directly on Android is difficult for third-party developers. The official Hexagon SDK requires signing NDAs or using specific OEM keys for some features. However, **NEON SIMD** on the main CPU is accessible to everyone and provides 4x-8x speedups over standard C++.

## Technical Details
- **Hexagon SDK:** Requires offloading code via FastRPC. This adds latency (context switching) which is bad for ultra-low latency audio (<5ms) unless processing large chunks.
- **NEON:** All ARMv7-A and ARMv8-A CPUs support NEON. It uses 128-bit registers (Q0-Q15) to process four floats at once.
- **AAudio + NEON:** This is the "sweet spot" for most Android audio apps.

## Findings
- **Power:** DSP is efficient for "Always-On" keywords (1mW vs 20mW on CPU). But for active processing (during a call), the CPU's efficiency cores are often sufficient.
- **Complexity:** Writing C++ with NEON intrinsics (`<arm_neon.h>`) is much easier than writing Hexagon assembly.

## Implementation Considerations
- **Strategy:** Build the `DSPKernel` class with a reference C++ implementation and a NEON-optimized specialization.
- **Detection:** Use `getauxval(AT_HWCAP)` on Linux/Android to confirm NEON availability (though practically guaranteed on modern Android).

## References/Resources
- [Arm Neon Intrinsics Reference](https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/intrinsics)
- [Oboe Guide](https://github.com/google/oboe/blob/master/docs/FullGuide.md)

## Next Steps
- Implement `DSPKernel.cpp` with a simple loop vs NEON comparison.

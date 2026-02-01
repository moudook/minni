# Signal Processing Implementation Plan

This module provides high-performance DSP capabilities.

## Planned Interfaces

### `DSPKernel.h`
SIMD-accelerated primitives.
```cpp
namespace dsp {
    void fft_forward(const float* input, float* complex_output, int size);
    void filter_fir(const float* input, float* output, const float* taps, int size);
}
```

### `SensorFusion.h`
Kalman filter implementation for combining accelerometer and gyroscope data.

## Optimization Strategy
- **NEON Intrinsics**: All hot loops must be hand-optimized for ARMv8 NEON.
- **Zero-Allocation**: Real-time loops must not allocate memory on the heap.

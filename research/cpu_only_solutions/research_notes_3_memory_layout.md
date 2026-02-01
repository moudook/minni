# Research Notes: Memory Layout (NHWC)

**Date:** 2026-01-30
**Topic:** Tensor Memory Layouts for CPU Cache

## Summary
[To be filled: Cache miss analysis of NCHW vs NHWC on ARM]

## Technical Details
- **Channel Last (NHWC):** Elements of the same pixel are contiguous. Better for dot products across channels (Conv2D).
- **Channel First (NCHW):** Planes of color are contiguous. Better for GPU.

## Findings
[To be filled: Overhead of transposing tensors at runtime]

## Implementation Considerations
- Ensuring model conversion tools (in `src/optimization`) export to NHWC by default for CPU models.

## References/Resources
- [TensorFlow Memory Layouts](https://www.tensorflow.org/guide/data_performance)

## Next Steps
- Check default output format of ONNX export from PyTorch.

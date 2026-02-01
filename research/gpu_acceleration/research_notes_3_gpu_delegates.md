# Research Notes: GPU Delegates

**Date:** 2026-01-30
**Topic:** TFLite GPU Delegate & Shader Caching

## Summary
[To be filled: Compilation time analysis for large models]

## Technical Details
- **Shader Compilation:** Generating GPU machine code from GLSL/SPIR-V.
- **Serialization:** Saving the compiled kernel to disk.

## Findings
[To be filled: Time saved on second launch using serialization]

## Implementation Considerations
- Managing the cache file path.
- Invalidation logic (if app updates or driver updates).

## References/Resources
- [TFLite GPU Delegate](https://www.tensorflow.org/lite/performance/gpu)

## Next Steps
- Analyze the serialized cache format.

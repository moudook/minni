# Research Notes: Activation Quantization

**Date:** 2026-01-30
**Topic:** Dynamic vs Static Activation Ranges

## Summary
[To be filled: Latency comparison of Dynamic Range vs Full Integer quantization]

## Technical Details
- **Dynamic:** Calculate min/max of input tensor at runtime.
- **Static:** Pre-computed min/max frozen in the model file.

## Findings
[To be filled: Performance delta on ARM NEON (Dynamic is often cheaper than expected)]

## Implementation Considerations
- Static requires representative dataset calibration.

## References/Resources
- [Quantization Spectrums](https://www.tensorflow.org/lite/performance/quantization_spec)

## Next Steps
- Compare inference time of Dynamic vs Static on Pixel 6.

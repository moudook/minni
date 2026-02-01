# Research Notes: Weight Clustering

**Date:** 2026-01-30
**Topic:** Weight Sharing / Palettization

## Summary
[To be filled: Compression ratios achieving using K-Means clustering on weights]

## Technical Details
- **Codebooks:** Look-up tables for weight values.
- **Sparse Formats:** Storing indices instead of values.

## Findings
[To be filled: Impact on zip compression (entropy reduction)]

## Implementation Considerations
- Decoding overhead at runtime (if hardware doesn't support indirect lookup).

## References/Resources
- [TensorFlow Model Optimization Toolkit](https://www.tensorflow.org/model_optimization)

## Next Steps
- Experiment with 16-cluster (4-bit) weights.

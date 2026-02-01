# Research Notes: Mixed Precision

**Date:** 2026-01-30
**Topic:** Hybrid INT8/FP16 Execution

## Summary
[To be filled: Identifying sensitive layers in Transformers]

## Technical Details
- **Outliers:** Specific weights/activations that destroy quantization resolution.
- **Fallbacks:** Running specific ops in Float while others run in Int.

## Findings
[To be filled: Perplexity score of Mixed Precision vs Pure Int8]

## Implementation Considerations
- Graph partitioning logic.

## References/Resources
- [GGUF Format](https://github.com/ggerganov/ggml)

## Next Steps
- Analyze outlier distributions in attention layers.

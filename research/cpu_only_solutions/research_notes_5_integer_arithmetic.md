# Research Notes: Integer Arithmetic

**Date:** 2026-01-30
**Topic:** Fixed Point Math Performance

## Summary
[To be filled: Speedup of INT8 vs FP32 on specific ARM Cortex revisions]

## Technical Details
- **Dot Product Instructions:** `SDOT` and `UDOT` (introduced in ARMv8.2-A).
- **Quantization Aware Training (QAT):** Preserving accuracy.

## Findings
[To be filled: Theoretical TOPS (Trillions of Operations Per Second) for Int8 vs Float]

## Implementation Considerations
- Handling overflow (accumulation in Int32).
- Dequantization cost at the output layer.

## References/Resources
- [Quantization in Neural Networks](https://arxiv.org/abs/1806.08342)

## Next Steps
- Review `research/model_quantization` for the algorithmic side.

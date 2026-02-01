# Research Notes: Quantization Aware Training

**Date:** 2026-01-30
**Topic:** QAT Implementation

## Summary
[To be filled: Framework support for QAT]

## Technical Details
- **FakeQuant Nodes:** Operations inserted into the graph during training to simulate rounding.
- **Straight Through Estimator (STE):** Allowing gradients to flow through non-differentiable rounding functions.

## Findings
[To be filled: Training time overhead of QAT]

## Implementation Considerations
- Requires access to original training data.

## References/Resources
- [PyTorch QAT](https://pytorch.org/docs/stable/quantization.html)

## Next Steps
- Test QAT on a small MobileNet.

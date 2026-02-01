# Research Notes: Pruning

**Date:** 2026-01-30
**Topic:** Structured vs Unstructured Pruning

## Summary
[To be filled: Sparsity support in standard inference engines]

## Technical Details
- **Sparse Tensor Support:** Does TFLite/XNNPACK support sparse matrix multiply? (Usually no, requiring structured pruning).
- **Lottery Ticket Hypothesis:** Finding winning subnetworks.

## Findings
[To be filled: Speedup realized by pruning 50% of channels in a CNN]

## Implementation Considerations
- Structured pruning changes the model topology (layer shapes), requiring a "clean" export.

## References/Resources
- [State of Sparsity](https://arxiv.org/abs/1902.09574)

## Next Steps
- Prune a ResNet-50 and measure apk size reduction.

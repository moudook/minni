# Research Notes: Operator Fusion

**Date:** 2026-01-30
**Topic:** Graph rewrites and Fusion

## Summary
[To be filled: Common fusion patterns supported by ONNX Runtime]

## Technical Details
- **Constant Folding:** Pre-calculating static parts of the graph.
- **Layer Fusion:** Conv+BN+Relu, MatMul+Add+Bias.

## Findings
[To be filled: Inference time reduction from running graph optimization passes]

## Implementation Considerations
- Most converters (TFLite Converter, ONNX Optimizer) do this automatically, but custom ops can block fusion.

## References/Resources
- [Graph Optimizations in ONNX Runtime](https://onnxruntime.ai/docs/performance/graph-optimizations.html)

## Next Steps
- Visualize a fused vs unfused graph in Netron.

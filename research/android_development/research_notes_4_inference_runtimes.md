# Research Notes: Inference Runtimes

**Date:** 2026-01-30
**Topic:** TFLite vs ONNX Runtime vs Executorch

## Summary
[To be filled: Feature matrix comparison of the three major mobile runtimes]

## Technical Details
- **TFLite:** Industry standard, mature, limited dynamic shape support.
- **ONNX Runtime:** Broad model support, good for GenAI.
- **Executorch:** New PyTorch mobile stack, beta but promising.

## Findings
[To be filled: Binary size impact of including each runtime]

## Implementation Considerations
- Conversion pipeline complexity (PyTorch -> ONNX -> ORT vs PyTorch -> TFLite).
- Operator support coverage.

## References/Resources
- [ONNX Runtime Mobile](https://onnxruntime.ai/docs/tutorials/mobile/)
- [Executorch](https://pytorch.org/executorch-overview)

## Next Steps
- Check support for Quantized LLMs (GGUF equivalent support).

# Research Notes: Post-Training Quantization

**Date:** 2026-01-30
**Topic:** PTQ Workflows

## Summary
[To be filled: Comparison of PTQ tools from TFLite, ONNX, and PyTorch]

## Technical Details
- **Calibration:** Feeding ~100 sample inputs to determine activation ranges.
- **Bias Correction:** Adjusting layer biases to compensate for weight shift.

## Findings
[To be filled: Accuracy drop of Llama-3-8B when converted to INT4 using PTQ]

## Implementation Considerations
- Needs a dataset loader in the conversion tool.

## References/Resources
- [TFLite PTQ](https://www.tensorflow.org/lite/performance/post_training_quantization)

## Next Steps
- Create a script to run PTQ on a vision model.

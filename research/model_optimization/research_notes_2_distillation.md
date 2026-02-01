# Research Notes: Knowledge Distillation

**Date:** 2026-01-30
**Topic:** Teacher-Student Training

## Summary
[To be filled: Best loss functions for distillation (KL Divergence vs MSE)]

## Technical Details
- **Temperature:** Softening the teacher's logits to reveal more information about the "dark knowledge" (incorrect class probabilities).
- **Data-Free Distillation:** If we don't have the original dataset.

## Findings
[To be filled: Accuracy of DistilBERT vs BERT on mobile]

## Implementation Considerations
- Requires access to a powerful server/GPU to run the Teacher during training.

## References/Resources
- [Distilling the Knowledge in a Neural Network](https://arxiv.org/abs/1503.02531)

## Next Steps
- Set up a training script for a student MobileBERT.

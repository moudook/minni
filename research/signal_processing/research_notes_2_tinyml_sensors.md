# Research Notes: TinyML for Sensors

**Date:** 2026-01-30
**Topic:** 1D CNNs and TCNs

## Summary
[To be filled: Architectures for Accelerometer/Gyroscope data]

## Technical Details
- **TCN (Temporal Convolutional Network):** Dilated convolutions to capture long history.
- **Quantization:** Crucial here (int8 is mandatory for microcontrollers, good for phones).

## Findings
[To be filled: Accuracy of fall detection using a 20KB model]

## Implementation Considerations
- Sampling rate consistency (Android sensor events can be jittery).

## References/Resources
- [TinyML Book](https://www.oreilly.com/library/view/tinyml/9781492052036/)

## Next Steps
- Collect a small dataset of "phone shaking" gestures.

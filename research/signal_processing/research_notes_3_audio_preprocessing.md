# Research Notes: Audio Pre-processing

**Date:** 2026-01-30
**Topic:** Noise Suppression & Beamforming

## Summary
Audio pre-processing is critical for accurate edge ASR. We compared "classic" DSP (WebRTC APM) vs. "modern" Deep Learning (RNNoise, DeepFilterNet). Deep Learning approaches offer superior suppression of non-stationary noise (baby crying, keyboard typing) but at a higher CPU cost.

## Technical Details
- **RNNoise:** A hybrid approach. Uses classic DSP for feature extraction (Bark scale bands) and a small RNN (GRU) to predict gain masks. Extremely efficient (<1% CPU).
- **DeepFilterNet:** A pure Deep Learning approach (UNet-like). Better quality but requires ~10-15% CPU on a mid-range phone.
- **WebRTC APM:** The gold standard for echo cancellation (AEC).

## Findings
- **Latency:** RNNoise operates on 10ms or 20ms frames with lookahead. Total latency is ~25ms, which is acceptable for calls.
- **Quality:** RNNoise removes static noise well but can introduce "musical artifacts" if pushed too hard.

## Implementation Considerations
- **Floating Point:** Most open-source implementations are float. We need a fixed-point or NEON-optimized version for ARMv7.
- **State Management:** Noise suppressors have internal state. This state must be persisted if processing chunks of a stream.

## References/Resources
- [RNNoise-Android Port](https://github.com/desaysv-automotive/rnnoise_android)
- [DeepFilterNet TFLite](https://github.com/Rulil/DeepFilterNet)

## Next Steps
- Compile `librnnoise.so` for Android.
- Wrap it in a JNI class `AudioProcessor.process(short[] buffer)`.

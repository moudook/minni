# Research Notes: Real-Time Latency

**Date:** 2026-01-30
**Topic:** Minimizing Audio/Sensor Lag

## Summary
[To be filled: Measured round-trip latency on Pixel vs Samsung]

## Technical Details
- **AAudio:** "Pro" audio API for Android.
- **Exclusive Mode:** Bypassing the mixer for lower latency.
- **Callback driven:** Avoiding blocking reads/writes.

## Findings
[To be filled: Can we achieve <10ms round trip?]

## Implementation Considerations
- Garbage Collection (GC) pauses in Java/Kotlin are fatal. Must use C++ (JNI) for the audio loop.

## References/Resources
- [Oboe](https://github.com/google/oboe)

## Next Steps
- Build a "Hello World" Oboe app.

# Research Notes: Thermal Throttling

**Date:** 2026-01-30
**Topic:** Sustained GPU Performance

## Summary
[To be filled: GPU frequency curves under load]

## Technical Details
- **Thermal APIs:** Android `PowerManager` and `ThermalService`.
- **Sustained Performance Mode:** `window.setSustainedPerformanceMode`.

## Findings
[To be filled: FPS drop over 10 minutes of inference]

## Implementation Considerations
- Implementing a "duty cycle" (sleep between frames) to manage heat.
- Lowering resolution dynamically.

## References/Resources
- [Android Thermal Mitigation](https://source.android.com/devices/thermal/mitigation)

## Next Steps
- Build a stress test app.

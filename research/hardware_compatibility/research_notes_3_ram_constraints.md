# Research Notes: RAM Constraints

**Date:** 2026-01-30
**Topic:** Memory Tiers and Model Selection

## Summary
[To be filled: Defining Low, Mid, and High tier thresholds]

## Technical Details
- **ActivityManager.getMemoryInfo():** Querying available RAM.
- **zRAM:** Compressed RAM usage on Android.

## Findings
[To be filled: Overhead of the OS on a 4GB device (usually only 1.5GB free for apps)]

## Implementation Considerations
- Dynamic model selection algorithm (load `small.bin` vs `large.bin` based on RAM).

## References/Resources
- [Android Performance Class](https://developer.android.com/guide/topics/media/media-performance-class)

## Next Steps
- Create a utility class for device tiering.

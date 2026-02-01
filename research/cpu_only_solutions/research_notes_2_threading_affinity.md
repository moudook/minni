# Research Notes: Threading & Affinity

**Date:** 2026-01-30
**Topic:** Thread Pooling on big.LITTLE Architectures

## Summary
[To be filled: Best practices for core binding on Android]

## Technical Details
- **big.LITTLE:** Mixing Cortex-X (Prime), Cortex-A7xx (Big), and Cortex-A5xx (Little) cores.
- **sched_setaffinity:** Linux syscall to pin threads.
- **Thread Migration:** Cost of OS moving threads between cores.

## Findings
[To be filled: Latency variance when running on Little cores vs Big cores]

## Implementation Considerations
- Determining the number of threads (often Number of Big Cores is optimal, not Total Cores).
- Battery impact of sustained Big core usage.

## References/Resources
- [Android CPU Performance](https://developer.android.com/topic/performance/threads)

## Next Steps
- Investigate `glibc` vs Android's Bionic libc threading quirks.

# Research Notes: Memory Management & LMK

**Date:** 2026-01-30
**Topic:** Low Memory Killer (LMK) interactions with Large Models

## Summary
[To be filled: Analysis of memory limits per app on standard Android devices]

## Technical Details
- **RSS vs PSS:** Understanding Resident Set Size vs Proportional Set Size.
- **LMK Thresholds:** How Android decides which process to kill.
- **Large Heap:** using `android:largeHeap="true"` in manifest.

## Findings
[To be filled: Max safe RAM usage for 4GB, 6GB, and 8GB devices]

## Implementation Considerations
- Model mapping (mmap) to avoid loading full weights into RAM.
- Streaming weights for inference.

## References/Resources
- [Android Memory Management](https://developer.android.com/topic/performance/memory-management)

## Next Steps
- Prototype a large memory allocation test to trigger LMK.

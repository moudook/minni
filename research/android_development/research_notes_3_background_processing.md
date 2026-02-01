# Research Notes: Background Processing

**Date:** 2026-01-30
**Topic:** WorkManager vs Foreground Services for Long-Running Inference

## Summary
[To be filled: Comparative analysis of execution limits for WorkManager and Foreground Services]

## Technical Details
- **WorkManager:** Guaranteed execution, but deferrable. Constraints (charging, storage).
- **Foreground Service:** User-visible notification, higher priority, less likely to be killed.
- **Doze Mode:** Impact on CPU throttling.

## Findings
[To be filled: Impact of thermal throttling on long-running background tasks]

## Implementation Considerations
- Notification requirement for Foreground Services.
- Handling process death and restart.

## References/Resources
- [Guide to Background Work](https://developer.android.com/guide/background)

## Next Steps
- Determine if "offline AI" counts as "immediate" or "deferred" work in most user flows.

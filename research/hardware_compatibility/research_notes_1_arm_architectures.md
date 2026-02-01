# Research Notes: ARM Architectures

**Date:** 2026-01-30
**Topic:** 32-bit vs 64-bit and ARMv9

## Summary
[To be filled: Market share of 32-bit only devices in 2026]

## Technical Details
- **armeabi-v7a:** Legacy 32-bit. Limited registers.
- **arm64-v8a:** Standard 64-bit. Double the registers, faster math.
- **PAC/BTI:** Pointer Authentication codes in newer chips.

## Findings
[To be filled: Performance penalty of running 32-bit code on 64-bit processors]

## Implementation Considerations
- Determining if we can drop `armeabi-v7a` to save space.

## References/Resources
- [Android ABI Management](https://developer.android.com/ndk/guides/abis)

## Next Steps
- Check top 100 device list stats.

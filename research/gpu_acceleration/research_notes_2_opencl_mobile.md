# Research Notes: OpenCL Mobile

**Date:** 2026-01-30
**Topic:** OpenCL Availability on Android

## Summary
[To be filled: List of devices/OEMs that ship OpenCL libraries (`libOpenCL.so`)]

## Technical Details
- **Dlopen:** Loading the library dynamically at runtime since it's not in the NDK.
- **Qualcomm Adreno:** Strong OpenCL support.
- **Google Pixel:** Historically blocks OpenCL.

## Findings
[To be filled: Success rate of loading libOpenCL.so on test devices]

## Implementation Considerations
- Must implement a robust "CheckAvailability" function to avoid crashes on Pixel phones.

## References/Resources
- [OpenCL on Android](https://github.com/rsnemmen/OpenCL-examples)

## Next Steps
- Create a simple checking script.

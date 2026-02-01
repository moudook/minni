# Research Notes: ABI Filters

**Date:** 2026-01-30
**Topic:** APK Size Optimization

## Summary
[To be filled: Impact of App Bundles (.aab) on delivery size]

## Technical Details
- **Split APKs:** Play Store serving specific slices.
- **Uncompressed Native Libs:** Setting `android:extractNativeLibs="false"` to save disk space (install time) vs download size.

## Findings
[To be filled: Size savings of removing x86 support (emulator only)]

## Implementation Considerations
- CI/CD pipeline configuration for splits.

## References/Resources
- [Android App Bundles](https://developer.android.com/guide/app-bundle)

## Next Steps
- Configure Gradle for per-ABI splitting.

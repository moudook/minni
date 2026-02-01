# Research Notes: Privacy & Storage

**Date:** 2026-01-30
**Topic:** Scoped Storage and Encrypted File Storage

## Summary
[To be filled: Best practices for storing large model files and sensitive user data]

## Technical Details
- **Scoped Storage:** App-specific directories vs Shared storage.
- **EncryptedFile:** Jetpack security library for at-rest encryption.
- **Access Performance:** Overhead of encryption on read speeds.

## Findings
[To be filled: I/O speed benchmarks of EncryptedFile vs standard FileInputStream]

## Implementation Considerations
- Storing models in `filesDir` (internal) vs `getExternalFilesDir` (external private).
- Avoiding media scanner indexing for model files.

## References/Resources
- [Data Security on Android](https://developer.android.com/topic/security/data)

## Next Steps
- Verify if Scoped Storage allows executing binaries or libraries (usually no).

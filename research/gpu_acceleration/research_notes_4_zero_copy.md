# Research Notes: Zero-Copy Memory

**Date:** 2026-01-30
**Topic:** AHardwareBuffer & Shared Memory

## Summary
[To be filled: Bandwidth comparison of Copy vs Zero-Copy]

## Technical Details
- **AHardwareBuffer:** Android NDK API for graphic buffers.
- **EGL Images:** Legacy method for OpenGL-CPU sharing.
- **Mapped Memory:** Vulkan host-visible memory types.

## Findings
[To be filled: Latency reduction in a camera-to-inference pipeline]

## Implementation Considerations
- Synchronization fences (waiting for GPU to finish reading before CPU writes).

## References/Resources
- [Android NDK AHardwareBuffer](https://developer.android.com/ndk/reference/group/a-hardware-buffer)

## Next Steps
- Prototype a camera frame buffer passing to GPU shader.

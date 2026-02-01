# Research Notes: WiFi Sensing

**Date:** 2026-01-30
**Topic:** Channel State Information (CSI) Access

## Summary
[To be filled: Feasibility on non-rooted production devices]

## Technical Details
- **CSI:** Detailed amplitude/phase info per subcarrier.
- **RTT (Round Trip Time):** WiFi RTT (802.11mc) for indoor positioning (supported by Android P+).

## Findings
[To be filled: Can we get raw CSI or just RTT?]

## Implementation Considerations
- Accessing raw CSI usually requires specialized firmware (Nexmon), making it hard for a general app.
- RTT is the standard alternative.

## References/Resources
- [Android Wi-Fi RTT](https://developer.android.com/guide/topics/connectivity/wifi-rtt)

## Next Steps
- Test the `WifiRttManager` API.

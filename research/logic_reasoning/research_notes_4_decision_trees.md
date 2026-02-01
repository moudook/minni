# Research Notes: Decision Trees

**Date:** 2026-01-30
**Topic:** Gradient Boosted Decision Trees (GBDT)

## Summary
[To be filled: Comparison of XGBoost vs LightGBM on Android]

## Technical Details
- **Tree Traversal:** Memory access patterns (often random access, bad for cache).
- **Compiling to Code:** Converting the tree structure into a massive if-else C function (Treelite).

## Findings
[To be filled: Binary size of a compiled 1000-tree model]

## Implementation Considerations
- JNI overhead for frequent small calls.

## References/Resources
- [Treelite](https://treelite.readthedocs.io/)

## Next Steps
- Compile an XGBoost model to a shared library (`.so`).

# Research Notes: KV-Cache Optimization

**Date:** 2026-01-30
**Topic:** Efficient Memory Management for LLMs

## Summary
[To be filled: Memory growth of KV Cache per token]

## Technical Details
- **PagedAttention:** Breaking cache into non-contiguous blocks (OS paging style).
- **GQA (Grouped Query Attention):** Architectural change in Llama 2/3 to reduce KV cache size by sharing keys/values across heads.

## Findings
[To be filled: Max context length on 8GB RAM with vs without optimizations]

## Implementation Considerations
- Requires writing custom kernels or using advanced runtimes (executorch/vllm).

## References/Resources
- [vLLM: Easy, Fast, and Cheap LLM Serving](https://vllm.ai/)

## Next Steps
- Analyze GQA impact on Llama 3 8B.

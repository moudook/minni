# Research Plan: Model Optimization for Edge AI

This document outlines the research strategy for optimizing neural network architectures and computation graphs, separate from quantization (which focuses on precision).

---

## 1. Pruning (Structured vs Unstructured)

### Description
Researching techniques to remove unnecessary connections (weights) from the neural network.

### Why interested
Large models often have "dead" neurons that contribute little to the output. Removing them reduces size and FLOPs.

### How it helps
**Unstructured pruning** makes matrices sparse (hard to speed up on standard CPUs). **Structured pruning** removes entire channels/filters, directly shrinking the model dimensions.

### Countering
We are countering **Computational Redundancy**. Why pay the compute cost for weights that are near zero?

### Alternatives
- **Magnitude-based pruning**: Simple thresholding.
- **Movement pruning**: Pruning during fine-tuning based on gradient changes.

---

## 2. Knowledge Distillation

### Description
Researching how to train a small "Student" model to mimic a large "Teacher" model.

### Why interested
We cannot run Llama-3-70B on a phone. But we might be able to run a 1B parameter model that was taught by the 70B model.

### How it helps
Preserves accuracy in small architectures better than training from scratch.

### Countering
We are countering **Model Size Limitations**.

### Alternatives
- **Logit-based distillation**: Matching output probabilities.
- **Feature-based distillation**: Matching intermediate layer activations.

---

## 3. Operator Fusion (Graph Optimization)

### Description
Researching compiler-level optimizations that merge multiple operations into one.

### Why interested
Inference is often bound by memory bandwidth, not compute. Reading/writing intermediate results for `Conv2D -> BatchNorm -> ReLU` is wasteful.

### How it helps
Fusing them into a single kernel (`Conv2D_BatchNorm_ReLU`) keeps data in registers/L1 cache, drastically reducing memory access.

### Countering
We are countering **Memory Bandwidth Bottlenecks**.

### Alternatives
- **Vertical Fusion**: Merging sequential ops.
- **Horizontal Fusion**: Merging parallel ops (e.g., in Multi-Head Attention).

---

## 4. Neural Architecture Search (NAS) for Mobile

### Description
Researching automated search strategies to find efficient network topologies (e.g., MobileNetV3 was found via NAS).

### Why interested
Hand-designed architectures are good, but AI-designed architectures often find better trade-offs for specific hardware constraints (latency vs accuracy).

### How it helps
Finding "efficient" blocks (like Inverted Residuals) that map well to ARM CPUs.

### Countering
We are countering **Suboptimal Topologies**.

### Alternatives
- **Once-for-All (OFA)**: Training a super-network that can be sliced into sub-networks.

---

## 5. KV-Cache Optimization (for LLMs)

### Description
Specific optimization for Large Language Models regarding the Key-Value Cache management.

### Why interested
On long context windows, the KV cache grows huge and becomes the bottleneck for memory and speed.

### How it helps
Techniques like "Paged Attention" (vLLM style) or "StreamingLLM" allow us to run longer conversations on limited RAM.

### Countering
We are countering **Context Window Limits**.

### Alternatives
- **Sliding Window Attention**: Forgetting old tokens.
- **Quantized KV Cache**: Storing the cache in INT8.

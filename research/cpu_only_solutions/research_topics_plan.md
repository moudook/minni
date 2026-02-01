# Research Plan: CPU-Only Optimization for Edge AI

This document outlines the research strategy for optimizing AI model inference on Android devices that lack powerful GPUs or NPUs. The priority is ensuring acceptable performance on standard ARM CPUs.

---

## 1. SIMD Acceleration (NEON & SVE)

### Description
Research into ARM NEON (Advanced SIMD) and SVE (Scalable Vector Extension) instruction sets.

### Why interested
Standard C++ loops are too slow for matrix multiplication. We must ensure our inference engine (or the one we choose, like XNNPACK) effectively utilizes NEON instructions which are present on almost all modern Android phones.

### How it helps
Provides the primary speedup for CPU inference (4x-8x faster than scalar code).

### Countering
We are countering **High Latency**. Without SIMD, even small models will feel sluggish and unusable.

### Alternatives
- **Auto-vectorization**: Relying on the compiler (often unreliable for complex kernels).
- **Hand-written Assembly**: Maximum performance but high maintenance.

---

## 2. Threading Models & Affinity

### Description
Investigating efficient multi-threading strategies for mobile CPUs, specifically big.LITTLE architectures.

### Why interested
Mobile CPUs have "Performance" cores and "Efficiency" cores. Naively throwing threads at the problem can cause overheating or schedule tasks on slow cores, degrading performance.

### How it helps
We need a threading pool that intelligently binds critical inference tasks to Performance cores while leaving UI tasks on Efficiency cores.

### Countering
We are countering **Thermal Throttling and Jitter**. Poor thread management heats up the phone, causing the OS to throttle the CPU speed.

### Alternatives
- **OpenMP**: Standard but heavy.
- **Pthread pools**: Lightweight, manual control.
- **Android Process API**: `Process.setThreadPriority`.

---

## 3. Memory Layout: NHWC vs NCHW

### Description
Researching optimal tensor memory layouts for CPU caches.

### Why interested
GPUs often prefer NCHW (Batch, Channels, Height, Width), but CPUs with NEON often perform better with NHWC (Channel Last) due to better cache locality during vectorization.

### How it helps
Avoids costly "transpose" operations during inference. If the model is trained in NCHW but the CPU kernel wants NHWC, we waste time shuffling data.

### Countering
We are countering **Memory Bandwidth Bottlenecks**. Mobile CPUs have limited cache; reducing cache misses is as important as raw compute speed.

### Alternatives
- **Runtime Reordering**: Transposing on the fly (slow).
- **Offline Conversion**: Pre-processing the model to the target layout.

---

## 4. XNNPACK vs. GEMM Libraries

### Description
Evaluating highly optimized CPU kernel libraries.

### Why interested
We don't want to write raw matrix multiplication code if a library like Google's XNNPACK or ARM's Compute Library (ACL) already solves it for our target architecture.

### How it helps
Provides a stable, maintained backend for our "CPU-first" goal.

### Countering
We are countering **Reinventing the Wheel**. Leveraging existing low-level optimizations allows us to focus on the high-level framework.

### Alternatives
- **Ruy**: Matrix multiplication library used in TFLite.
- **Eigen**: C++ template library (good but slower compile times).

---

## 5. Integer Arithmetic (Fixed Point)

### Description
Researching the performance difference between FP32 (float) and INT8 (integer) math on ARM CPUs.

### Why interested
CPU ALUs often execute integer operations faster and with less energy than floating point.

### How it helps
Justifies the need for quantization not just for size, but for speed.

### Countering
We are countering **Battery Drain**. Integer math consumes less power per operation.

### Alternatives
- **BF16 (BFloat16)**: Supported on newer ARMv8.2+ chips, offering float range with less precision.

# Research Plan: GPU Acceleration for Edge AI

This document outlines the research strategy for leveraging mobile GPUs (Adreno, Mali, PowerVR) to accelerate AI inference. This is an "optional enhancement" layer on top of our CPU-first framework.

---

## 1. Vulkan Compute Shaders

### Description
Research into using the cross-platform Vulkan API for general-purpose GPU computing (GPGPU).

### Why interested
Vulkan is the modern standard for Android graphics and compute. It offers lower overhead than OpenGL ES and more control than NNAPI.

### How it helps
Provides a high-performance backend that works across different GPU vendors (Qualcomm, ARM, Imagination).

### Countering
We are countering **Driver Fragmentation**. Unlike OpenCL (which is not supported on Pixel devices), Vulkan is mandatory for modern Android certification.

### Alternatives
- **OpenGL ES Compute Shaders**: Legacy, widely supported but higher overhead.
- **RenderScript**: Deprecated by Google.

---

## 2. OpenCL for Mobile

### Description
Investigating OpenCL support on Android devices.

### Why interested
OpenCL is the standard for GPGPU in the desktop world and is supported by many ARM Mali and Qualcomm Adreno drivers, even if not officially part of the Android framework.

### How it helps
Often provides the highest raw performance on specific hardware (e.g., Xiaomi/Samsung devices with specific drivers).

### Countering
We are countering **Performance Ceilings**. On devices that support it, OpenCL can outperform Vulkan for specific matrix math kernels.

### Alternatives
- **Proprietary SDKs**: Samsung Neural SDK, Qualcomm SNPE.

---

## 3. TFLite GPU Delegate vs. WebGPU

### Description
Comparing the standard TFLite GPU delegate against emerging web standards like WebGPU (native implementations).

### Why interested
The TFLite GPU delegate is a "black box" that chooses between OpenGL and OpenCL. We need to understand its limitations and initialization costs.

### How it helps
Determines if we can just wrap the standard delegate or if we need to write our own compute shaders.

### Countering
We are countering **Initialization Latency**. GPU delegates often take seconds to compile shaders on the first run; we need strategies to cache this.

### Alternatives
- **TVM**: Apache TVM compiler stack for mobile GPUs.

---

## 4. Zero-Copy Memory (Hardware Buffers)

### Description
Researching AHardwareBuffer and shared memory between CPU and GPU.

### Why interested
Moving data between CPU RAM and GPU VRAM is expensive. On mobile, they share physical RAM, but APIs often force copies.

### How it helps
"Zero-copy" allows the GPU to read the camera feed or input tensor directly from the same memory address the CPU wrote to.

### Countering
We are countering **IO Bottlenecks**. Fast inference means nothing if we spend 20ms copying data to the GPU.

### Alternatives
- **Fast RPC**: Qualcomm's mechanism for sharing memory with DSPs.

---

## 5. Thermal Throttling & Stability

### Description
Investigating the thermal impact of sustained GPU load compared to CPU load.

### Why interested
GPUs generate significant heat. Android will aggressively throttle the GPU frequency if the device gets hot.

### How it helps
We need a "sustained performance mode" or a duty-cycling strategy to keep framerates consistent over time (e.g., for AR apps).

### Countering
We are countering **Performance Degradation**. A framework that starts fast but slows down after 1 minute is useless for real-world apps.

### Alternatives
- **Dynamic Voltage and Frequency Scaling (DVFS)**: Monitoring OS thermal status.

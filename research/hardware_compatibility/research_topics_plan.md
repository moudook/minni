# Research Plan: Hardware Compatibility for Edge AI

This document outlines the research strategy for ensuring our framework runs on the widest possible range of Android devices, from low-end budget phones to high-end flagships.

---

## 1. ARM Architecture Variations (ARMv7, v8, v9)

### Description
Researching the instruction set differences and support requirements for 32-bit (armeabi-v7a) vs 64-bit (arm64-v8a).

### Why interested
While Google Play mandates 64-bit support, many IoT and older Android devices still run 32-bit. New ARMv9 chips have SVE2 but might drop 32-bit support entirely.

### How it helps
Defines our compiler flags and minimum supported device list.

### Countering
We are countering **Instruction Set Incompatibility**. Using NEON instructions that don't exist on older chips will crash the app.

### Alternatives
- **Fat Binaries**: Including code for all architectures (increases size).
- **Runtime Detection**: Checking CPU features at startup.

---

## 2. NPU Fragmentation (The "Wild West")

### Description
Mapping the landscape of Neural Processing Units (Qualcomm Hexagon, MediaTek APU, Samsung NPU, Google TPU).

### Why interested
Unlike GPUs (which use standard Vulkan/OpenCL), NPUs often require vendor-specific SDKs (SNPE, Neuron, EDEN).

### How it helps
We need to decide if we support NPUs via a unified API (NNAPI) or if we write specific backends for popular chips.

### Countering
We are countering **Vendor Lock-in**. We don't want to write separate code for every phone manufacturer.

### Alternatives
- **NNAPI**: The theoretical universal standard (often buggy).
- **Ignore NPUs**: Stick to CPU/GPU to ensure consistency.

---

## 3. RAM Constraints (2GB to 16GB)

### Description
Researching the "safe" working memory limits for different device tiers.

### Why interested
A model that runs fine on a Pixel 8 Pro (12GB RAM) will instantly crash a Moto G Play (3GB RAM).

### How it helps
We need to define "Device Tiers" and automatically load smaller/quantized models on lower-tier devices.

### Countering
We are countering **Out-of-Memory (OOM) Crashes**.

### Alternatives
- **Swap File**: Generally not available/usable for apps on Android.
- **Disk Streaming**: Loading weights on demand (slow).

---

## 4. Android API Levels (Fragmentation)

### Description
Investigating system limitations on older Android versions (Android 10/API 29 and below).

### Why interested
Newer features like "Updatable NNAPI Drivers" or "HardwareBuffer" might not exist on older OS versions.

### How it helps
Sets our `minSdkVersion`.

### Countering
We are countering **OS Obsolescence**. We want to support devices that people actually keep for years (often stuck on Android 10/11).

### Alternatives
- **Backports**: Using libraries that polyfill new features.

---

## 5. ABI Filters & APK Splits

### Description
Researching build configurations to minimize download size while maximizing compatibility.

### Why interested
Including native libraries (compiled C++) for x86, x86_64, armv7, and arm64 makes the APK huge.

### How it helps
We optimize the delivery mechanism so a user only downloads the binaries their specific phone needs.

### Countering
We are countering **Bloatware**. Users won't download a 200MB "framework" just to run a 50MB model.

### Alternatives
- **Dynamic Feature Modules**: Downloading the AI engine only when the user opens the AI feature.

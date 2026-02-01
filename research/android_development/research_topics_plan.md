# Research Plan: Android Development for Edge AI

This document outlines the specific research topics intended for the `research/android_development` module. The goal is to identify the most efficient methods for deploying AI models on Android devices, prioritizing CPU-only support with optional GPU acceleration.

---

## 1. Android Neural Networks API (NNAPI) vs. Custom Delegates

### Description
Research into the native Android Neural Networks API (NNAPI) compared to custom delegates provided by frameworks like TensorFlow Lite or PyTorch Mobile.

### Why interested
NNAPI is designed to abstract hardware acceleration on Android, but its stability and performance consistency across different manufacturers (Samsung, Xiaomi, Pixel) vary significantly.

### How it helps
It determines the foundational layer for our inference engine. If NNAPI is unreliable on low-end devices, we must default to a robust CPU delegate.

### Countering
We are countering the issue of **Hardware Fragmentation**. Android devices have vastly different hardware capabilities; relying blindly on one API can cause crashes or slow performance on unsupported devices.

### Alternatives
- **Vulkan / OpenGL ES**: Direct GPU programming.
- **Hexagon DSP**: Qualcomm specific delegates.
- **CPU Fallback**: Pure C++ implementations (slowest but most compatible).

---

## 2. Memory Management & The Low Memory Killer (LMK)

### Description
Investigation into how large language models (LLMs) or Vision Transformers interact with Android's Low Memory Killer system service.

### Why interested
Mobile devices have aggressive RAM management. Loading a 4GB model on a 6GB RAM phone often triggers the LMK, killing the app background or foreground process.

### How it helps
We need to design a "chunking" or streaming loading mechanism. This research will define the maximum safe memory footprint we can target for "universal" compatibility.

### Countering
We are countering **App Termination/Instability**. Ensuring the framework doesn't just "work" but stays alive during heavy inference.

### Alternatives
- **mmap (Memory Mapped Files)**: Keeping model on disk and paging in.
- **Model Quantization**: Reducing model size (covered in `model_quantization` research, but linked here for memory impact).

---

## 3. Background Processing: WorkManager vs. Foreground Services

### Description
Research into the best Android component for long-running inference tasks (e.g., processing a video or fine-tuning a model locally).

### Why interested
Android limits background execution to save battery. Standard threads are killed when the screen turns off.

### How it helps
We need to choose between `WorkManager` (deferrable tasks) and `Foreground Services` (immediate, user-visible tasks) to ensure the inference completes without interruption.

### Countering
We are countering **OS Execution Limits**. Android's Doze mode and App Standby buckets aggressively throttle background CPU usage.

### Alternatives
- **JobScheduler**: Older API, often superseded by WorkManager.
- **Kotlin Coroutines**: Good for concurrency, but doesn't guarantee process survival unlike Services.

---

## 4. TFLite vs. ONNX Runtime Mobile vs. Executorch

### Description
A comparative study of inference runtimes specifically for the Android ecosystem.

### Why interested
While TFLite is the standard, ONNX Runtime often provides better cross-platform support and performance for transformer architectures. Executorch is PyTorch's new mobile solution.

### How it helps
Selecting the core engine affects the entire conversion pipeline (`src/optimization`). We need the one that offers the best balance of speed and model support (specifically for GenAI).

### Countering
We are countering **Vendor Lock-in and Model Incompatibility**. We want to support the widest range of model architectures (Transformers, CNNs, RNNs).

### Alternatives
- **NCNN**: Tencent's mobile-optimized framework (very fast on CPU).
- **MNN**: Alibaba's mobile neural network.

---

## 5. Privacy & Sandboxing: Scoped Storage Implementation

### Description
Research into storing model files and user inference data securely using Android's Scoped Storage and potentially EncryptedFile APIs.

### Why interested
"Privacy-preserving AI" is a key feature of this project (`idea.md`). We must ensure that local inference data cannot be accessed by other malicious apps.

### How it helps
Defines the file I/O layer of the framework (`src/utils`).

### Countering
We are countering **Data Leakage**. Local AI is safer than cloud, but only if the local storage is secure.

### Alternatives
- **Internal Storage**: Private by default but limited in space.
- **External Storage**: ample space but harder to secure.

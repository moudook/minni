# Research Plan: Signal Processing & Sensor AI

This document outlines the research strategy for processing raw sensor data (audio, IMU, RF) directly on Android devices.

---

## 1. Digital Signal Processing (DSP) on Android

### Description
Researching the usage of the dedicated DSP hardware (Hexagon on Qualcomm) vs CPU SIMD for signal filtering (FFT, FIR/IIR filters).

### Why interested
Signal processing is math-heavy. Doing FFTs on the main CPU can be power hungry. Offloading to DSP saves massive battery.

### How it helps
Enables "always-on" sensing (e.g., step counting, sleep tracking, keyword spotting) without draining the battery.

### Countering
We are countering **Battery Drain**.

### Alternatives
- **Oboe / AAudio**: Low latency audio APIs.
- **Android Sensor Hub**: Hardware abstraction for sensors.

---

## 2. TinyML for Sensors

### Description
Researching ultra-small architectures (CNNs, RNNs) designed for 1D time-series data.

### Why interested
Standard models (ResNet, Transformer) are 2D/Image focused. Sensor data is 1D. We need specialized 1D Convs or TCNs (Temporal Convolutional Networks).

### How it helps
Detecting gestures, falls, or anomalies from accelerometer data with <50KB models.

### Countering
We are countering **Latency and Size**. These models must run in <10ms.

### Alternatives
- **TensorFlow Lite for Microcontrollers**: (Can we adapt this for Android DSP?).
- **Edge Impulse**: Platform for training sensor models.

---

## 3. Audio Pre-processing (Noise Suppression)

### Description
Researching classic vs AI-based noise suppression, echo cancellation, and beamforming.

### Why interested
Before an audio model (ASR) sees the data, it must be clean.

### How it helps
Improves accuracy of downstream capabilities (Speech-to-Text) in noisy environments (cafes, streets).

### Countering
We are countering **Environmental Noise**.

### Alternatives
- **WebRTC Audio Processing Module (APM)**: The industry standard (C++).
- **DeepFilterNet**: AI-based noise suppression.

---

## 4. WiFi Sensing (CSI)

### Description
Researching the ability to access WiFi Channel State Information (CSI) on Android (requires root or specific chipsets?).

### Why interested
WiFi signals can detect presence, breathing, or gestures without cameras (privacy preserving).

### How it helps
Adds "vision-like" capabilities without a lens.

### Countering
We are countering **Privacy Concerns**. No images are ever taken.

### Alternatives
- **Ultrasound sensing**: Using speaker/mic to do sonar.

---

## 5. Real-Time Latency Optimization

### Description
Researching the Android audio/sensor path latency (buffer sizes, scheduler jitter).

### Why interested
For things like hearing aids or live music processing, latency must be <10ms (round trip). Android historically struggles here.

### How it helps
Determines if our framework can support "Real-Time" applications or just "Offline" processing.

### Countering
We are countering **The "Android Latency Problem"**.

### Alternatives
- **Oboe**: Google's wrapper for AAudio/OpenSL ES to minimize latency.

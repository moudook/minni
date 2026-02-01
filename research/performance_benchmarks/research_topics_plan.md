# Research Plan: Performance Benchmarking

This document outlines how we measure the speed, accuracy, and efficiency of our edge AI models.

---

## 1. Latency Measurement (TTFT vs End-to-End)

### Description
Researching the correct way to measure latency on Android.

### Why interested
"Inference time" reported by TFLite benchmark tools often excludes pre-processing (bitmap to tensor) and post-processing (tensor to bounding box), which can take longer than the inference itself.

### How it helps
Provides realistic performance expectations for app developers.

### Countering
We are countering **Vanity Metrics**.

---

## 2. Power Profiling (Batterystats & Rail Monitoring)

### Description
Researching how to measure energy consumption per inference.

### Why interested
A model that runs fast but drains 1% battery per minute is unusable.

### How it helps
We can report "Inferences per Joule" as a key metric.

### Countering
We are countering **Battery Anxiety**.

---

## 3. Thermal Throttling Analysis

### Description
Researching how sustained load affects performance over time.

### Why interested
Benchmarking a cold phone gives different results than a warm phone.

### How it helps
Establishing a "Stress Test" protocol.

---

## 4. Memory Profiling (PSS vs RSS)

### Description
Researching accurate RAM measurement on Linux/Android.

### Why interested
Avoiding OOM kills.

---

## 5. Automated Regression Testing

### Description
Researching how to run these benchmarks automatically on a device farm (e.g., Firebase Test Lab).

### Why interested
Preventing performance regressions in CI/CD.

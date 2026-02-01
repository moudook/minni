# Research Plan: Model Quantization for Edge AI

This document outlines the research strategy for reducing model precision to fit large models onto constrained mobile hardware.

---

## 1. Post-Training Quantization (PTQ)

### Description
Researching techniques to quantize pre-trained models without re-training.

### Why interested
Most users will bring existing models (Llama 3, Mistral). We need a robust pipeline to convert these FP32/FP16 weights to INT8 or INT4.

### How it helps
Drastically reduces model size (4x smaller for INT8) and improves CPU inference speed.

### Countering
We are countering **Storage and Bandwidth Limits**. Users cannot download 20GB files on 4G connections.

### Alternatives
- **Naive Casting**: Simply rounding numbers (high accuracy loss).
- **Calibration**: Using a representative dataset to find min/max ranges.

---

## 2. Quantization Aware Training (QAT)

### Description
Simulating quantization errors during the training phase.

### Why interested
For small specialized models (e.g., wake word detection, specific image classification), PTQ often degrades accuracy too much. QAT recovers this.

### How it helps
Ensures the model learns to be robust to low-precision arithmetic.

### Countering
We are countering **Accuracy Degradation**. Edge AI is useless if the answers are wrong.

### Alternatives
- **Fine-tuning**: Retraining only the last layers.

---

## 3. Weight Clustering (Palettization)

### Description
Grouping weights into a fixed number of unique values (centroids) and storing indices.

### Why interested
Allows for high compression rates (e.g., 6-bit or 4-bit simulation) even if the hardware only supports 8-bit math.

### How it helps
Reduces the binary size on disk, even if runtime memory usage remains standard.

### Countering
We are countering **APK Size Limits**. Android apps have strict size limits on the Play Store.

### Alternatives
- **Huffman Coding**: Lossless compression of the weights.

---

## 4. Mixed Precision (INT8 + FP16)

### Description
Running sensitive layers (like the first and last layers of a transformer) in high precision while quantizing the rest.

### Why interested
Uniform quantization often breaks specific layers (e.g., Attention mechanisms).

### How it helps
Balances accuracy (where needed) and speed (where possible).

### Countering
We are countering **"Brain Damage"**. Preventing the model from becoming incoherent due to aggressive quantization.

### Alternatives
- **Dynamic Range Quantization**: Storing weights in INT8 but activating in FP32.

---

## 5. Activation Quantization (Dynamic vs Static)

### Description
Researching how to handle the data passing *between* layers.

### Why interested
Quantizing weights is easy; quantizing activations (the "thoughts" of the model) is hard because their range varies per input.

### How it helps
Static quantization (pre-calculating ranges) is faster but requires calibration data. Dynamic is safer but slower.

### Countering
We are countering **Runtime Latency**. Dynamic calculation of min/max ranges adds overhead to every inference step.

### Alternatives
- **Per-Channel vs Per-Tensor**: Granularity of the quantization scale.

# Adding New AI Models (GenAI / LiteRT)

Minni Framework supports running Generative AI and standard Deep Learning models on-device using the **LiteRT (TensorFlow Lite)** integration. This allows you to run models like MobileNet, EfficientNet, or even small Language Models (SLMs) completely offline.

## Overview

The `ModelRunner` class provides a high-level wrapper around the native TFLite interpreter, handling model loading, tensor allocation, and inference execution in C++ for maximum performance.

## Supported Formats

- **Format:** TensorFlow Lite (`.tflite`)
- **Quantization:** Float32, Int8 (supported by TFLite delegate)
- **Input/Output:** Multi-dimensional Tensors

## Step-by-Step Guide

### 1. Prepare Your Model

Convert your TensorFlow, PyTorch, or JAX model to `.tflite` format. Ensure you know the input and output signature (shapes and data types).

```bash
# Example: Converting a Keras model
tflite_convert --saved_model_dir=/path/to/model --output_file=model.tflite
```

### 2. Add Model to Assets or Storage

Place your `.tflite` file in your Android project's `assets/` folder, or download it at runtime to the app's internal file storage (`Context.getFilesDir()`).

### 3. Initialize ModelRunner

In your Kotlin/Java code (e.g., `InferenceService.kt`), create an instance of `ModelRunner`.

```kotlin
import com.minni.framework.genai.ModelRunner

val modelRunner = ModelRunner()
```

### 4. Load the Model

Load the model file. You specify the path and the number of threads to use for inference.

```kotlin
val modelPath = filesDir.absolutePath + "/my_model.tflite"
val numThreads = 4 // Adjust based on device capabilities

val success = modelRunner.loadModel(modelPath, numThreads)
if (!success) {
    Log.e("Minni", "Failed to load model")
}
```

### 5. Prepare Inputs

Wrap your input data in `Tensor` objects. You must match the shape expected by the model.

```kotlin
import com.minni.framework.genai.Tensor

// Example: Image input [1, 224, 224, 3]
val inputShape = intArrayOf(1, 224, 224, 3)
val inputData = FloatArray(1 * 224 * 224 * 3) { ... } // Fill with normalized pixel data

val inputTensor = Tensor("input_0", inputShape, inputData)
```

### 6. Run Inference

Call `predict()` with an array of input tensors.

```kotlin
val inputs = arrayOf(inputTensor)
val outputs = modelRunner.predict(inputs)
```

### 7. Process Outputs

The result is an array of `Tensor` objects representing the model output.

```kotlin
if (outputs != null && outputs.isNotEmpty()) {
    val outputTensor = outputs[0]
    val probabilities = outputTensor.data
    // Find argmax, apply softmax, etc.
}
```

### 8. Cleanup

Always close the `ModelRunner` when done to free native memory.

```kotlin
modelRunner.close()
```

## Advanced Configuration

### Threading
Use `num_threads` in `load_model` to balance latency vs. CPU usage.
- **2-4 threads:** Good for high-performance cores.
- **1 thread:** Better for background power efficiency.

### Memory
The `ModelRunner` is designed to be zero-copy where possible, but passing large arrays from Java to C++ involves some overhead. For massive inputs (e.g., video frames), future updates will support `HardwareBuffer` or direct `ByteBuffer` mapping.

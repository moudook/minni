# System Architecture

## Overview
The framework is divided into three layers:

### 1. The Core Layer (`src/core`)
C++ implementation of the inference engine. Handles memory mapping, tensor buffers, and delegation to runtimes.

### 2. The Android Layer (`src/android`)
Kotlin/Java wrappers (JNI) that expose the Core to the Android OS. Handles:
- `WorkManager` for background tasks.
- `Service` for keeping the process alive.
- `Activity` for UI.

### 3. The Optimization Layer (`src/optimization`)
Python tools that run *offline* to convert weights into the optimal format for the Core layer.

## Data Flow
`Raw File` -> `Optimization (Python)` -> `.bin / .tflite` -> `Android App` -> `Core (C++)` -> `NPU/GPU`

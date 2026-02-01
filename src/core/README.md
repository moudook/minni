# Core Source Module

This directory contains the core logic of the framework, independent of the Android UI.

## Structure
- `inference/`: Main C++ interface for model execution (TFLite/ONNX wrappers).
- `logic/`: Symbolic reasoning engines (Solvers, Knowledge Graphs).
- `signal/`: DSP and sensor processing algorithms.
- `loaders/`: Logic for loading and parsing model files.
- `tensors/`: Helpers for tensor manipulation.

# Research Plan: Logic & Reasoning Models

This document outlines the research strategy for implementing non-language-based reasoning engines on Android. This includes symbolic AI, solvers, and structured probabilistic models.

---

## 1. Symbolic AI & Solvers on Mobile

### Description
Researching the feasibility of running SAT solvers (Boolean Satisfiability), SMT solvers (Satisfiability Modulo Theories), or Prolog engines on Android.

### Why interested
LLMs are bad at strict logic puzzles or constraint satisfaction (e.g., "Schedule these 50 employees"). Symbolic solvers guarantee correctness.

### How it helps
Provides a "System 2" thinking component that can be called by the "System 1" LLM or used standalone for optimization problems.

### Countering
We are countering **LLM Hallucinations** and **Inconsistency**.

### Alternatives
- **Z3**: The standard SMT solver (can we compile it for Android?).
- **Clingo**: Answer Set Programming.

---

## 2. Graph Neural Networks (GNNs)

### Description
Researching GNN inference on mobile hardware.

### Why interested
Reasoning often involves relationships between entities (social networks, molecular structures, knowledge graphs). GNNs process this native structure better than text.

### How it helps
Enables features like fraud detection, recommendation systems, or drug discovery visualization on edge.

### Countering
We are countering **Data Structure Mismatch**. Flattening a graph into a text prompt for an LLM is inefficient.

### Alternatives
- **PyG (PyTorch Geometric)**: Can it be converted to TFLite/ONNX?
- **DGL (Deep Graph Library)**.

---

## 3. Bayesian Networks & Probabilistic Programming

### Description
Researching lightweight probabilistic reasoning on edge.

### Why interested
Mobile devices have noisy sensors. We need to infer the "true state" of the world (e.g., "Is the user driving?") from uncertain data with confidence intervals.

### How it helps
Provides robust decision making under uncertainty, which is critical for context-aware apps.

### Countering
We are countering **Uncertainty**. Neural networks usually give a single answer; Bayesian models give a distribution.

### Alternatives
- **Pyro / NumPyro**: PPL frameworks.
- **Micro-probabilistic frameworks**: Custom C++ implementations.

---

## 4. Decision Trees & Random Forests (GBDT)

### Description
Researching efficient execution of tree-based models (XGBoost, LightGBM) on Android.

### Why interested
For tabular data (e.g., battery usage stats, user behavior logs), GBDTs often beat Deep Learning in accuracy and speed.

### How it helps
Extremely fast inference (<1ms) and low battery usage for background classification tasks.

### Countering
We are countering **Over-engineering**. Don't use a Transformer when a Decision Tree works better.

### Alternatives
- **Treelite**: Compiles trees to C code for maximum speed.

---

## 5. Neuro-Symbolic Integration

### Description
Researching how to bridge the gap between neural perception (images/audio) and symbolic reasoning.

### Why interested
We want to interpret the world (Neural) and then reason about it (Symbolic). E.g., "I see a red light (Neural), therefore I must stop (Symbolic)."

### How it helps
Creates explainable AI systems.

### Countering
We are countering **Black Box decision making**.
